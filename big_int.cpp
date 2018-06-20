#include <vector>
#include <cmath>
#include <string>
#include <algorithm>
#include "big_int.h"

using namespace std;

typedef long long int ll;

void BigInt::removeLeadZero() {
    while (numVec.size() > 1 && numVec.back() == 0) {
        numVec.pop_back();
    }
}

string BigInt::prettyPrint() const {
    if (!sign) {
        return "-" + abs(*this).prettyPrint();
    }
    BigInt quotient = (*this) / BigInt(10);
    BigInt remainder = (*this) % BigInt(10);
    string answer = "";
    if (quotient != BigInt(0)) {
        answer += quotient.prettyPrint();
    }
    answer += to_string(remainder.getVec()[0]);
    return answer;
}

void BigInt::shift(unsigned int m) {
    reverse(numVec.begin(), numVec.end());
    for (int i = 0; i < m; ++i) {
        numVec.push_back(0);
    }
    reverse(numVec.begin(), numVec.end());
}

BigInt::BigInt(): numVec{vector<unsigned int>{0}}, sign{true} {}

BigInt::BigInt(vector<unsigned int>numVec, bool sign):
numVec{numVec}, sign{(numVec == vector<unsigned int>{0}) ? true : sign} {
    removeLeadZero();
}

BigInt::BigInt(unsigned int num, bool sign):
numVec{vector<unsigned int>{num}}, sign{!num ? true : sign} {}


BigInt::BigInt(int num):
numVec{vector<unsigned int>{(num >= 0 ? (unsigned int) num : (unsigned int) (-num))}}, sign{(num >= 0)} {}

 
BigInt BigInt::operator-() const {
    return BigInt(numVec, !sign);
}

bool BigInt::operator==(const BigInt& other) const {
    //cout << numVec.size() << "vec" << other.getVec().size() << endl;
    if (other.getVec().size() != numVec.size()) {
        return false;
    }
    if (other.getSign() != sign) {
        if (numVec.size() > 1 || numVec[0] != 0) {
            return false;
        }
    }
    for (int i = 0; i < numVec.size(); ++i) {
        if (numVec[i] != other.getVec()[i]) {
            return false;
        }
    }
    return true;
}

bool BigInt::operator!=(const BigInt& other) const {
    return !(*this == other);
}

bool BigInt::operator<(const BigInt& other) const {
    //eliminate different signs first
    if (!sign && other.getSign()) {
        return true;
    }
    if (sign && !other.getSign()) {
        return false;
    }
    if (!sign && !other.getSign()) {
        return (-other < -(*this));
    }
    if (numVec.size() < other.getVec().size()) {
        return true;
    }
    if (numVec.size() > other.getVec().size()) {
        return false;
    }
    for (int i = numVec.size() - 1; i >= 0; --i) {
        if (numVec[i] < other.getVec()[i]) {
            return true;
        }
        if (numVec[i] > other.getVec()[i]) {
            return false;
        }
    }
    //equality case finally
    return false;
}

bool BigInt::operator>(const BigInt& other) const {
    return (other < *this);
}

bool BigInt::operator<=(const BigInt& other) const {
    return (*this < other || *this == other);
}

bool BigInt::operator>=(const BigInt& other) const {
    return (other <= *this);
}

BigInt BigInt::operator+(const BigInt& other) const {
    int sz = numVec.size();
    int other_sz = other.getVec().size();
    int total_sz = (sz >= other_sz ? sz : other_sz);
    long long int carry = 0;
    vector<unsigned int>answer;
    //For simplicity we just consider the case of both positive since the other case can be dealt similarly
    if (other == BigInt(0)) {
        return *this;
    }
    if (sign != other.sign) {
        return *this - (-other);
    }
    for (int i = 0; i < total_sz; ++i) {
        long long int k = (numVec.size() > i ? numVec[i] : 0);
        long long int l = (other.getVec().size() > i ? other.getVec()[i] : 0);
        answer.push_back((k + l + carry) % limit);
        carry = (k + l + carry) / limit;
    }
    answer.push_back(carry);
    return BigInt(answer, sign);
}

BigInt BigInt::operator-(const BigInt& other) const {
    if (sign != other.sign) {
        return *this + (-other);
    }
    if (abs(*this) < abs(other)) {
        return - (other - (*this));
    }
    int sz = numVec.size();
    int other_sz = other.getVec().size();
    int total_sz = (sz >= other_sz ? sz : other_sz);
    vector<unsigned int> answer;
    long long int carry = 0;
    for (int i = 0; i < total_sz; ++i) {
        long long int k = (numVec.size() > i ? numVec[i] : 0);
        long long int l = (other.getVec().size() > i ? other.getVec()[i] : 0);
        if (k - l - carry >= 0) {
            answer.push_back(k - l - carry);
            carry = 0;
        }
        else {
            long long int temp = k - l - carry;
            long long int temp_carry = 0;
            while (temp < 0) {
                temp += limit;
                ++temp_carry;
            }
            answer.push_back(temp);
            carry = temp_carry;
        }
    }
    return BigInt(answer, sign);
}

BigInt BigInt::operator*(const BigInt& other) const {
    //we shall use Karatsuba's algorithm to speed up computations
    vector<unsigned int>answer;
    int n = numVec.size();
    int m = other.getVec().size();
    vector<unsigned int> other_vec = other.getVec();
    int two_split = min(n, m);
    if (two_split > 1) {
        int split = two_split / 2;
        vector<unsigned int>a0(split);
        vector<unsigned int>a1(n - split);
        vector<unsigned int>a2(split);
        vector<unsigned int>a3(m - split);
        copy(numVec.begin(), numVec.begin() + split, a0.begin());
        copy(numVec.begin() + split, numVec.end(), a1.begin());
        copy(other_vec.begin(), other_vec.begin() + split, a2.begin());
        copy(other_vec.begin() + split, other_vec.end(), a3.begin());
        BigInt small = BigInt(a0, sign) * BigInt(a2, other.getSign());
        BigInt big = BigInt(a1, sign) * BigInt(a3, other.getSign());
        BigInt middle
        = (BigInt(a0, sign) + BigInt(a1, sign)) * (BigInt(a2, other.getSign()) +
                                                   BigInt(a3, other.getSign())) - small - big;
        big.shift(2 * split);
        middle.shift(split);
        return small + big + middle;
    }
    if (n < m) {
        return (other * (*this));
    }
    long long int carry = 0;
    for (int i = 0; i < n; ++i) {
        //cout << "PRODUCT" << numVec[i] << " " << other.getVec()[0] << endl;
        long long int prod = (ll)numVec[i] * (ll)other.getVec()[0];
        //cout << prod << endl;
        answer.push_back((carry + prod) % limit);
        carry = (carry +prod) / limit;
    }
    //cout << "carry" << carry << endl;
    if (carry) {
        answer.push_back(carry);
    }
    return BigInt(answer, sign == other.getSign());
}

BigInt BigInt::operator/(const BigInt& other) const {
    if (abs(*this) < abs(other)) {
        return BigInt(0, true);
    }
    if (other == BigInt(0, true)) {
        throw std::overflow_error("Divide by zero exception");
    }
    int n = numVec.size();
    int m = other.getVec().size();
    int shift_amount = n - m;
    BigInt target = BigInt(vector<unsigned int>(numVec.end() - m, numVec.end()), sign);
    if (abs(target) < abs(other)) {
        target = BigInt(vector<unsigned int>(numVec.end() - m - 1, numVec.end()), sign);
        shift_amount = n - m - 1;
    }
    //now target / other is one 'bigDigit'
    long long int lo = 1;
    long long int hi = limit - 1;
    while (lo < hi) {
        long long int mid = lo + (hi -  lo + 1) / 2;
        BigInt testing = BigInt(mid, sign == other.getSign());
        /*
        cout << hi << " " << lo << " " << mid << endl;
        cout << (testing * other).getVec().size() << " " << target.getVec().size() << " ";
        cout << abs(testing * other).getVec().size() << " " << abs(target).getVec().size() << endl;
        cout << (abs(testing * other) <= abs(target)) << endl;
         */
        if (abs(testing * other) <= abs(target)) {
            lo = mid;
        }
        else {
            hi = mid - 1;
        }
        //cout << lo << " " << hi << endl;
    }
    //cout << "done" << endl;
    //now that lo = hi
    BigInt digit = BigInt(hi, sign == other.getSign());
    digit.shift(shift_amount);
    BigInt prod = digit * other;
    return ((*this - prod) / other) + digit;
}

BigInt BigInt::operator%(const BigInt& other) const {
    return (*this - (other * (*this / other)));
}

BigInt abs(const BigInt& other) {
    
    return BigInt(other.getVec(), true);
}

vector<unsigned int> BigInt::getVec() const {
    return numVec;
}

bool BigInt::getSign() const {
    return sign;
}

ostream &operator<<(ostream &out, const BigInt &bi) {
    out << bi.prettyPrint();
    return out;
}

istream &operator>>(istream &in, BigInt &bi) {
    string s;
    cin >> s;
    int n = s.length();
    bi = BigInt(0);
    for (int i = 0; i < n; ++i) {
        if (i == 0 && s[i] == '-') {
            bi.sign = false;
            continue;
        }
        if ((s[i] < '0' || s[i] > '9') && (i > 0 || s[i] != '-')) {
            break;
        }
        int dgt = int(s[i]) - int('0');
        bi = (bi * BigInt(10));
        bi = (bi + BigInt(dgt));
    }
    if (s[0] == '-') {
        bi.sign = false;
    }
    if (bi.numVec == vector<unsigned int>{0}) {
        bi.sign = true;
    }
    return in;
}
        
