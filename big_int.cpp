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
//Just treat it as a base 2 to base 10 conversion exercise
string BigInt::prettyPrint() const {
    if (!sign) {
        return "-" + abs(*this).prettyPrint();
    }
    vector<int>answer{0};
    vector<int>now{1};
    vector<bool>ref = numVec;
    for (int i = 0; i < ref.size(); ++i) {
        if (ref[i]) {
            //now always >= answer
            vector<int>temp;
            int carry = 0;
            for (int j = 0; j < now.size(); ++j) {
                int dgt_1 = (answer.size() > j ? answer[j] : 0);
                int dgt_2 = (now.size() > j ? now[j] : 0);
                int total = dgt_1 + dgt_2 + carry;
                temp.push_back(total % 10);
                carry = total / 10;
            }
            if (carry) {
                temp.push_back(carry);
            }
            answer = temp;
        }
        //multiply now by 2
        vector<int>temp_mult;
        int carry_mult = 0;
        for (int j = 0; j < now.size(); ++j) {
            int total = (now[j] * 2) + carry_mult;
            temp_mult.push_back(total % 10);
            carry_mult = total/ 10;
        }
        if (carry_mult) {
            temp_mult.push_back(carry_mult);
        }
        now = temp_mult;
    }
    string target = "";
    if (!sign) {
        target += "-";
    }
    for (int i = answer.size() - 1; i >= 0; --i) {
        target += to_string(answer[i]);
    }
    return target;
}

void BigInt::shift(unsigned int m) {
    reverse(numVec.begin(), numVec.end());
    for (int i = 0; i < m; ++i) {
        numVec.push_back(0);
    }
    reverse(numVec.begin(), numVec.end());
}

BigInt::BigInt(): numVec{vector<bool>{0}}, sign{true} {}

BigInt::BigInt(vector<bool>numVec, bool sign):
numVec{numVec}, sign{(numVec == vector<bool>{0}) ? true : sign} {
    removeLeadZero();
}

BigInt::BigInt(unsigned int num, bool sign):
numVec{vector<bool>(0)}, sign{!num ? true : sign} {
    if (num == 0) {
        numVec = vector<bool> {0};
    }
    else {
        unsigned int num1 = num;
        while (num1) {
            numVec.push_back(num1 % 2);
            num1 /= 2;
        }
    }
}


BigInt::BigInt(int num):
numVec{vector<bool>(0)}, sign{(num >= 0) ? true : false} {
    if (num == 0) {
        numVec = vector<bool> {0};
    }
    else {
        unsigned int num1 = abs(num);
        while (num1) {
            numVec.push_back(num1 % 2);
            num1 /= 2;
        }
    }
}

 
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
    int carry = 0;
    vector<bool>answer;
    //For simplicity we just consider the case of both positive since the other case can be dealt similarly
    if (other == BigInt(0)) {
        return *this;
    }
    if (sign != other.sign) {
        return *this - (-other);
    }
    for (int i = 0; i < total_sz; ++i) {
        int k = (numVec.size() > i ? numVec[i] : 0);
        int l = (other.getVec().size() > i ? other.getVec()[i] : 0);
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
    vector<bool> answer;
    int borrow = 0;
    for (int i = 0; i < total_sz; ++i) {
        int k = (numVec.size() > i ? numVec[i] : 0);
        int l = (other.getVec().size() > i ? other.getVec()[i] : 0);
        int temp = k - l - borrow;
        int temp_borrow = 0;
        while (temp < 0) {
            temp += limit;
            ++temp_borrow;
        }
        answer.push_back(temp);
        borrow = temp_borrow;
    }
    return BigInt(answer, sign);
}

BigInt BigInt::operator*(const BigInt& other) const {
    //we shall use Karatsuba's algorithm to speed up computations
    vector<bool>answer;
    int n = numVec.size();
    int m = other.getVec().size();
    vector<bool> other_vec = other.getVec();
    int two_split = min(n, m);
    if (two_split > 1) {
        int split = two_split / 2;
        vector<bool>a0(split);
        vector<bool>a1(n - split);
        vector<bool>a2(split);
        vector<bool>a3(m - split);
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
    int carry = 0;
    for (int i = 0; i < n; ++i) {
        //cout << "PRODUCT" << numVec[i] << " " << other.getVec()[0] << endl;
        int prod = (ll)numVec[i] * (ll)other.getVec()[0];
        //cout << prod << endl;
        answer.push_back((carry + prod) % limit);
        carry = (carry + prod) / limit;
    }
    //cout << "carry" << carry << endl;
    if (carry) {
        answer.push_back(carry);
    }
    return BigInt(answer, sign == other.getSign());
}

BigInt BigInt::operator/(const BigInt& other) const {
    if (abs(*this) < abs(other)) {
        return BigInt(0);
    }
    if (other == BigInt(0)) {
        throw std::overflow_error("Divide by zero exception");
    }
    int n = numVec.size();
    int m = other.getVec().size();
    int shift_amount = n - m;
    BigInt target = BigInt(vector<bool>(numVec.end() - m, numVec.end()), sign);
    if (abs(target) < abs(other)) {
        target = BigInt(vector<bool>(numVec.end() - m - 1, numVec.end()), sign);
        shift_amount = n - m - 1;
    }
    //now that lo = hi
    BigInt digit = BigInt(1, sign == other.getSign());
    digit.shift(shift_amount);
    BigInt prod = digit * other;
    return ((*this - prod) / other) + digit;
}

BigInt BigInt::operator%(const BigInt& other) const {
    return (*this - (other * (*this / other)));
}

BigInt& BigInt::operator+=(const BigInt& other) {
    *this = (*this + other);
    return *this;
}

BigInt& BigInt::operator-=(const BigInt& other) {
    *this = (*this - other);
    return *this;
}

BigInt& BigInt::operator*=(const BigInt& other) {
    *this = (*this * other);
    return *this;
}

BigInt& BigInt::operator/=(const BigInt& other) {
    *this = (*this / other);
    return *this;
}

BigInt& BigInt::operator%=(const BigInt& other) {
    *this = (*this % other);
    return *this;
}

BigInt BigInt::operator<<(int m) {
    int n = numVec.size();
    vector<bool>answer(n + m);
    copy(numVec.begin(), numVec.end(), answer.begin() + m);
    return BigInt(answer, sign);
}

BigInt BigInt::operator>>(int m) {
    if (numVec.size() <= m) {
        return BigInt(0);
    }
    int n = numVec.size();
    vector<bool>answer(n - m);
    copy(numVec.begin() + m, numVec.end(), answer.begin());
    return BigInt(answer, sign);
}

BigInt abs(const BigInt& other) {
    return BigInt(other.getVec(), true);
}

vector<bool> BigInt::getVec() const {
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
    if (bi.numVec == vector<bool>{0}) {
        bi.sign = true;
    }
    return in;
}

BigInt gcd(const BigInt& b1, const BigInt& b2) {
    if (b1 < BigInt(0) || b2 < BigInt(0)) {
        return gcd(abs(b1), abs(b2));
    }
    if (b1 < b2) {
        return gcd(b2, b1);
    }
    //b2 <= b1
    if (b2 == BigInt(0)) {
        return b1;
    }
    return gcd(b2, b1 % b2);
}
        
