#include "big_int.h"
#include "num_ring.h"

using namespace std;



BigInt Inverse (const BigInt& num, const BigInt& div) {
    //assert: num / div
    pair<BigInt, BigInt> left = make_pair(BigInt(1), BigInt(0));
    pair<BigInt, BigInt> right = make_pair(BigInt(0), BigInt(1));
    BigInt num1 = num;
    BigInt div1 = div;
    while (div != BigInt(0)) {
        BigInt quo = num1 / div1;
        BigInt rem = num1 % div1;
        BigInt neo_first = left.first - quo * right.first;
        BigInt neo_second = left.second - quo * right.second;
        pair<BigInt, BigInt> neo = make_pair(neo_first, neo_second);
        num1 = div1;
        div1 = rem;
        left = right;
        right = neo;
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

NumRing::NumRing(BigInt base, BigInt num) : base{abs(base)}, num{num} {
    if (base == BigInt(0)) {
        throw "division by zero";
    }
    num %= abs(base);
    if (num < BigInt(0)) {
        num += base;
    }
}

NumRing NumRing::operator+(const NumRing& other) const {
    if (base != other.getBase()) {
        throw "invalid bases";
    }
    return NumRing((num + other.getNum()), base);
}

NumRing NumRing::operator-(const NumRing& other) const {
    if (base != other.getBase()) {
        throw "invalid bases";
    }
    return NumRing((num - other.getNum()), base);
}

NumRing NumRing::operator*(const NumRing& other) const {
    if (base != other.getBase()) {
        throw "invalid bases";
    }
    return NumRing((num * other.getNum()), base);
}

NumRing NumRing::operator/(const NumRing& other) const {
    if (base != other.getBase()) {
        throw "invalid bases";
    }
    BigInt div = other.getNum();
    BigInt inv = Inverse(div, base);
    return NumRing((num * inv), base);
}

NumRing exp (const NumRing& theBase, const BigInt& bi) {
    int sz = bi.getVec().size();
    BigInt num = theBase.getNum();
    BigInt now = BigInt(num);
    BigInt answer = BigInt(1);
    for (int i = 0; i < sz; ++i) {
        if (bi.getVec()[i]) {
            answer *= now;
            answer %= theBase.getBase();
        }
        now = (now * now);
        now %= theBase.getBase();
    }
    return NumRing(answer, theBase.getBase());
}

BigInt NumRing::getNum() const {
    return base;
}

BigInt NumRing::getBase() const {
    return num;
}
