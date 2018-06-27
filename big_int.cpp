#include <vector>
#include <cmath>
#include <string>
#include <algorithm>
#include "big_int.h"

using namespace std;

typedef long long int ll;

vector<bool>add(const vector<bool>& fst, const vector<bool>& scd) {
    int total_sz = (fst.size() >= scd.size() ? fst.size() : scd.size());
    vector<bool>answer;
    bool carry = false;
    for (int i = 0; i < total_sz; ++i) {
        bool fst_ind = (fst.size() > i ? fst[i] : false);
        bool scd_ind = (scd.size() > i ? scd[i] : false);
        answer.push_back(fst_ind ^ scd_ind ^ carry);
        carry = (fst_ind && scd_ind) || (fst_ind && carry) || (scd_ind && carry);
    }
    if (carry) {
        answer.push_back(carry);
    }
    return answer;
}

//subtraction: require fst >= scd
vector<bool>sub(const vector<bool>& fst, const vector<bool>& scd) {
    int total_sz = (fst.size() >= scd.size() ? fst.size() : scd.size());
    vector<bool>answer;
    bool borrow = false;
    for (int i = 0; i < total_sz; ++i) {
        bool fst_ind = (fst.size() > i ? fst[i] : false);
        bool scd_ind = (scd.size() > i ? scd[i] : false);
        answer.push_back(fst_ind ^ scd_ind ^ borrow);
        borrow = (scd_ind && borrow) || (!fst_ind && scd_ind) || (!fst_ind && borrow);
    }
    while (answer.size() > 1 && !answer.back()) {
        answer.pop_back();
    }
    return answer;
}

vector<bool>left_shift(const vector<bool>& fst, int m) {
    vector<bool>answer(fst.size() + m);
    copy(fst.begin(), fst.end(), answer.begin() + m);
    return answer;
}

vector<bool>mult(vector<bool>fst, vector<bool> scd) {
    if (fst == vector<bool>{0} || scd == vector<bool>{0}) {
        return vector<bool>{0};
    }
    if (scd.size() > fst.size()) {
        return mult(scd, fst);
    }
    if (scd.size() == 1) {
        return fst;
    }
    int n = fst.size();
    int m = scd.size();
    int split = (scd.size() + 1) / 2;
    vector<bool>a0(split);
    vector<bool>a1(n - split);
    vector<bool>a2(split);
    vector<bool>a3(m - split);
    copy(fst.begin(), fst.begin() + split, a0.begin());
    copy(fst.begin() + split, fst.end(), a1.begin());
    copy(scd.begin(), scd.begin() + split, a2.begin());
    copy(scd.begin() + split, scd.end(), a3.begin());
    vector<bool> small = mult(a0, a2);
    vector<bool> big = mult(a1, a3);
    vector<bool> med = sub(sub(mult(add(a0, a1), add(a2, a3)), small), big);
    big = left_shift(big, 2 * split);
    med = left_shift(med, split);
    return add(add(small, big), med);
}

// assume scd is not a zero vector
vector<bool>div(vector<bool>fst, vector<bool> scd) {
    if (fst.size() < scd.size()) {
        return vector<bool>{0};
    }
    vector<bool>rem = fst;
    vector<bool>answer(fst.size() - scd.size() + 1);
    for (int i = fst.size() - scd.size(); i >= 0; --i) {
        vector<bool>temp = rem;
        bool borrow = false;
        for (int j = 0; j < scd.size(); ++j) {
            bool fst_ind = temp[i + j];
            bool scd_ind = scd[j];
            temp[i + j] = (fst_ind ^ scd_ind ^ borrow);
            borrow = (scd_ind && borrow) || (!fst_ind && scd_ind) || (!fst_ind && borrow);
        }
        for (int j = scd.size(); i + j < fst.size(); ++j) {
            bool fst_ind = temp[i + j];
            temp[i + j] = (fst_ind ^ borrow);
            borrow = (!fst_ind && borrow);
        }
        if (!borrow) {
            rem = temp;
            answer[i] = 1;
        }
    }
    while (answer.size() > 1 && !answer.back()) {
        answer.pop_back();
    }
    return answer;
}

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

BigInt::BigInt(): numVec{vector<bool>{0}}, sign{true} {}

BigInt::BigInt(vector<bool>numVec, bool sign):
numVec{numVec}, sign{(numVec == vector<bool>{0}) ? true : sign} {
    this->removeLeadZero();
    if (numVec == vector<bool>{0}) {
        this->sign = true;
    }
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
    //For simplicity we just consider the case of both positive since the other case can be dealt similarly
    if (other == BigInt(0)) {
        return *this;
    }
    if (sign != other.sign) {
        return *this - (-other);
    }
    return BigInt(add(numVec, other.getVec()), sign);
}

BigInt BigInt::operator-(const BigInt& other) const {
    if (sign != other.sign) {
        return *this + (-other);
    }
    if (abs(*this) < abs(other)) {
        return - (other - (*this));
    }
    return BigInt(sub(numVec, other.getVec()), sign);
}

BigInt BigInt::operator*(const BigInt& other) const {
    return BigInt(mult(numVec, other.getVec()), sign == other.getSign());
}

BigInt BigInt::operator/(const BigInt& other) const {
    //try iterative instead of recursion. Might speed up?
    if (other == BigInt(0)) {
        throw std::overflow_error("Divide by zero exception");
    }
    return BigInt(div(numVec, other.getVec()), sign == other.getSign());
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

BigInt BigInt::operator<<(int m) const {
    int n = numVec.size();
    vector<bool>answer(n + m);
    copy(numVec.begin(), numVec.end(), answer.begin() + m);
    return BigInt(answer, sign);
}

BigInt BigInt::operator>>(int m) const {
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
    BigInt now = BigInt(1);
    for (int i = n - 1; i >= 0; --i) {
        if (s[i] == '-') break;
        int k = int(s[i]) - int('0');
        for (int j = 0; j <= 3; ++j) {
            if ((k >> j) % 2) {
                bi += (now << j);
            }
        }
        now = ((now << 1) + (now << 3));
    }
    /*
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
    }*/
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
        
