#include <vector>
#include <cmath>
#include <string>
#include <algorithm>
#include <complex>
#include <cmath>
#include "big_int.h"

using namespace std;

typedef long long int ll;

const double PI = 3.141592653589793238460;

vector<bool>add_helper
(const vector<bool>& fst, int fstart, int ffinish, const vector<bool>& scd, int sstart, int sfinish) {
    int n = ffinish - fstart;
    int m = sfinish - sstart;
    int total_sz = (n >= m ? n : m);
    vector<bool>answer(total_sz);
    bool carry = false;
    for (int i = 0; i < total_sz; ++i) {
        bool fst_ind = (n > i) && fst[fstart + i];
        bool scd_ind = (m > i) && scd[sstart + i];
        answer[i] = fst_ind ^ scd_ind ^ carry;
        carry = (fst_ind && scd_ind) || (fst_ind && carry) || (scd_ind && carry);
    }
    if (carry) {
        answer.push_back(carry);
    }
    return answer;
}

vector<bool>add(const vector<bool>& fst, const vector<bool>& scd) {
    return add_helper(fst, 0, fst.size(), scd, 0, scd.size());
}

//subtraction: require fst >= scd
vector<bool>sub(const vector<bool>& fst, const vector<bool>& scd) {
    int total_sz = (fst.size() >= scd.size() ? fst.size() : scd.size());
    vector<bool>answer(total_sz);
    bool borrow = false;
    for (int i = 0; i < total_sz; ++i) {
        bool fst_ind = (fst.size() > i) && fst[i];
        bool scd_ind = (scd.size() > i) && scd[i];
        answer[i] = fst_ind ^ scd_ind ^ borrow;
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

//still Karatsuba-like (will keep this even after I finished doing fft multiplication)
vector<bool>mult_helper
(const vector<bool>& fst, int fstart, int ffinish, const vector<bool>& scd, int sstart, int sfinish) {
    if (sfinish - sstart > ffinish - fstart) {
        return mult_helper(scd, sstart, sfinish, fst, fstart, ffinish);
    }
    if (sfinish - sstart == 1) {
        if (scd[sstart] == 0) {
            return vector<bool>{0};
        }
        else {
            return vector<bool>(fst.begin() + fstart, fst.begin() + ffinish);
        }
    }
    int split = (sfinish - sstart + 1) / 2;
    vector<bool>small = mult_helper(fst, fstart, fstart + split, scd, sstart, sstart + split);
    vector<bool>big = mult_helper(fst, fstart + split, ffinish, scd, sstart + split, sfinish);
    vector<bool>med_first = add_helper(fst, fstart, fstart + split, fst, fstart + split, ffinish);
    vector<bool>med_second = add_helper(scd, sstart, sstart + split, scd, sstart + split, sfinish);
    vector<bool>med = sub(sub(mult_helper(med_first, 0, med_first.size(), med_second, 0, med_second.size()), small), big);
    big = left_shift(big, 2 * split);
    med = left_shift(med, split);
    return add(add(small, big), med);
}

void fft(vector<complex<double>>& vec) {
    if (vec.size() == 1) {
        return;
    }
    //divide
    vector<complex<double>>even(vec.size() / 2);
    vector<complex<double>>odd(vec.size() / 2);
    for (int i = 0; i < vec.size() / 2; ++i) {
        even[i] = vec[2 * i];
        odd[i] = vec[2 * i + 1];
    }
    //conquer
    fft(even);
    fft(odd);
    //combine
    for (int k = 0; k < vec.size()/2; ++k)
    {
        complex<double> t = std::polar(1.0, -2 * PI * k / vec.size()) * odd[k];
        vec[k    ] = even[k] + t;
        vec[k+vec.size()/2] = even[k] - t;
    }
}

//inverse of the fft
void ifft(vector<complex<double>>& vec) {
    //conjugate it first
    if (vec.size() == 1) {
        return;
    }
    for (int i = 0; i < vec.size(); ++i) {
        vec[i] = conj(vec[i]);
    }
    fft(vec);
    for (int i = 0; i < vec.size(); ++i) {
        vec[i] = conj(vec[i]);
    }
    for (int i = 0; i < vec.size(); ++i) {
        vec[i] /= vec.size();
    }
}

//try fft and see?
vector<bool>mult_fft(const vector<bool>& fst, const vector<bool>& scd) {
    int total_sz = fst.size() + scd.size();
    int sz_target = 1;
    while (sz_target < total_sz) {
        sz_target *= 2;
    }
    //polynomial multiplication
    vector<complex<double>>fst_comp(sz_target);
    vector<complex<double>>scd_comp(sz_target);
    for (int i = 0; i < sz_target; ++i) {
        fst_comp[i] = (fst.size() > i) && fst[i];
        scd_comp[i] = (scd.size() > i) && scd[i];
    }
    fft(fst_comp);
    fft(scd_comp);
    vector<complex<double>>ans_comp(sz_target);
    for (int i = 0; i < sz_target; ++i) {
        ans_comp[i] = fst_comp[i] * scd_comp[i];
    }
    ifft(ans_comp);
    vector<bool>answer(sz_target, false);
    int carry = 0;
    for (int i = 0; i < sz_target; ++i) {
        answer[i] = (carry + int(abs(ans_comp[i]) + 0.5)) % 2;
        carry= (carry + int(abs(ans_comp[i]) + 0.5)) / 2;
    }
    while (answer.size() > 1 && !answer.back()) {
        answer.pop_back();
    }
    return answer;
}

vector<bool>mult(const vector<bool>& fst, const vector<bool>& scd) {
    return mult_fft(fst, scd);
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

// assume scd is not a zero vector
vector<bool>remain(vector<bool>fst, vector<bool> scd) {
    if (fst.size() < scd.size()) {
        return fst;
    }
    vector<bool>rem = fst;
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
        }
    }
    while (rem.size() > 1 && !rem.back()) {
        rem.pop_back();
    }
    return rem;
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
    return BigInt(remain(numVec, other.getVec()), sign);
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
        
