#include "big_int.h"
#include "num_ring.h"

using namespace std;

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
    
}

BigInt NumRing::getNum() const {
    return base;
}

BigInt NumRing::getBase() const {
    return num;
}
