#include <vector>
#include <cmath>
#include "big_int.h"

using namespace std;

void BigInt::removeLeadZero() {
    while (numVec.size() > 1 && numVec.back() == 0) {
        numVec.pop_back();
    }
}

void BigInt::shift(unsigned int m) {
    reverse(numVec.begin(), numVec.end());
    for (int i = 0; i < m; ++i) {
        numVec.push_back(0);
    }
    reverse(numVec.begin(), numVec.end());
}

 BigInt::BigInt(vector<unsigned int>numVec, bool sign):
numVec{numVec}, sign{(numVec == vector<unsigned int>{0}) ? true : sign} {
    removeLeadZero();
}

BigInt::BigInt(unsigned int num, bool sign):
numVec{vector<unsigned int>{1, num}}, sign{!num ? true : sign} {}

/* compile error...will come to that later
BigInt::BigInt(int num):
numVec{vector<unsigned int>{1, (unsigned int)(0)}}, sign{(num >= 0)} {}
*/
 
BigInt BigInt::operator-() const {
    return BigInt(numVec, !sign);
}

bool BigInt::operator==(const BigInt& other) const {
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
    return (other > *this);
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
            carry = 0;
            answer.push_back(k - l - carry);
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
    int two_split = min(n, m);
    if (two_split > 1) {
        int split = two_split / 2;
        vector<unsigned int>a0(numVec.begin(), numVec.begin() + split);
        vector<unsigned int>a1(numVec.begin() + split, numVec.end());
        vector<unsigned int>a2(other.getVec().begin(), other.getVec().begin() + split);
        vector<unsigned int>a3(other.getVec().begin() + split, other.getVec().end());
        BigInt small = BigInt(a0, sign) * BigInt(a2, other.getSign());
        BigInt big = BigInt(a1, sign) * BigInt(a3, other.getSign());
        BigInt middle
        = (BigInt(a0, sign) + BigInt(a1, sign)) * (BigInt(a2, other.getSign()) +
                                                   BigInt(a3, other.getSign())) - small - big;
        big.shift(2 * m);
        middle.shift(m);
        return small + big + middle;
    }
    if (n < m) {
        return (other * (*this));
    }
    long long int carry = 0;
    for (int i = 0; i < n; ++i) {
        answer.push_back((carry + numVec[i] * other.getVec()[0]) % limit);
        carry = (carry + numVec[i] * other.getVec()[0]) / limit;
    }
    return BigInt(answer, sign == !other.getSign());
}

BigInt BigInt::operator/(const BigInt& other) const {
    if (abs(*this) < abs(other)) {
        return BigInt(0, true);
    }
    if (other == BigInt(0, true)) {
        throw std::overflow_error("Divide by zero exception");
    }
    int m = other.getVec().size();
    BigInt target = BigInt(vector<unsigned int>(numVec.end() - m, numVec.end()), sign);
    if (abs(target) < abs(other)) {
        target = BigInt(vector<unsigned int>(numVec.end() - m - 1, numVec.end()), sign);
    }
    //now target / other is one 'bigDigit'
    long long int lo = 1;
    long long int hi = limit - 1;
    while (lo < hi) {
        long long int mid = lo + (hi -  lo + 1) / 2;
        BigInt testing = BigInt(mid, sign == other.getSign());
        if (abs(testing * other) <= abs(target)) {
            lo = mid;
        }
        else {
            hi = mid - 1;
        }
    }
    //now that lo = hi
    BigInt digit = BigInt(hi, sign == other.getSign());
    return digit + (target - (digit * other)) / other;
}

BigInt BigInt::operator%(const BigInt& other) const {
    return *this - other * (*this / other);
}

BigInt BigInt::abs(const BigInt& other) const {
    return BigInt(numVec, true);
}

vector<unsigned int> BigInt::getVec() const {
    return numVec;
}

bool BigInt::getSign() const {
    return sign;
}
        
