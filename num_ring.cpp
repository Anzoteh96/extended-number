#include "big_int.h"
#include "num_ring.h"

using namespace std;



BigInt Inverse (const BigInt& num, const BigInt& div) {
    //assert: num / div
    pair<BigInt, BigInt> left = make_pair(BigInt(1), BigInt(0));
    pair<BigInt, BigInt> right = make_pair(BigInt(0), BigInt(1));
    BigInt num1 = num;
    BigInt div1 = div;
    while (div1 != BigInt(0)) {
        BigInt quo = num1 / div1;
        BigInt rem = num1 % div1;
        BigInt neo_first = left.first - quo * right.first;
        BigInt neo_second = left.second - quo * right.second;
        pair<BigInt, BigInt> neo = make_pair(neo_first, neo_second);
        num1 = div1;
        div1 = rem;
        left = right;
        right = neo;
        //cout << num1 << " " << div1 << " " << left.first << " " << left.second << endl;
    }
    left.second %= num;
    if (left.second < BigInt(0)) {
        left.second += num;
    }
    
    if (num1 != BigInt(1)) {
        throw "gcd not 1";
    }
    return left.second;
}

NumRing::NumRing(const BigInt& base, const BigInt& num) :
base{abs(base)}, num{((num % base) + abs(base)) % abs(base)} {
    if (base == BigInt(0)) {
        std::overflow_error("Divide by zero exception");
    }
    this->num %= base;
    if (this->num < BigInt(0)) {
        this->num += base;
    }
}

NumRing NumRing::operator+(const NumRing& other) const {
    //cout << base << " " << other.getBase() << endl;
    if (base != other.getBase()) {
        throw "invalid bases";
    }
    return NumRing(base, (num + other.getNum()));
}

NumRing NumRing::operator-(const NumRing& other) const {
    if (base != other.getBase()) {
        throw "invalid bases";
    }
    return NumRing(base, (num - other.getNum()));
}

NumRing NumRing::operator*(const NumRing& other) const {
    if (base != other.getBase()) {
        throw "invalid bases";
    }
    return NumRing(base, (num * other.getNum()));
}

NumRing NumRing::operator/(const NumRing& other) const {
    if (base != other.getBase()) {
        throw "invalid bases";
    }
    BigInt div = other.getNum();
    
    BigInt inv = Inverse(base, div);
    return NumRing(base, (num * inv));
}

NumRing exp (const NumRing& theBase, const BigInt& bi) {
    int sz = bi.getVec().size();
    BigInt num = theBase.getNum();
    BigInt now = BigInt(num);
    BigInt answer = BigInt(1);
    for (int i = 0; i < sz; ++i) {
        //cout << i << endl;
        if (bi.getVec()[i]) {
            answer *= now;
            answer %= theBase.getBase();
        }
        now = (now * now);
        now %= theBase.getBase();
    }
    return NumRing(theBase.getBase(), answer);
}

BigInt NumRing::getNum() const {
    return num;
}

BigInt NumRing::getBase() const {
    return base;
}
