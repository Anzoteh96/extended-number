#include "big_int.h"

class NumRing {
    //ring of m mod n
    const BigInt base;
    BigInt num;
public:
    NumRing (const BigInt& base, const BigInt& num = BigInt(0));
    NumRing operator+(const NumRing& other) const;
    NumRing operator-(const NumRing& other) const;
    NumRing operator*(const NumRing& other) const;
    NumRing operator/(const NumRing& other) const;
    BigInt getNum() const;
    BigInt getBase() const;
};

NumRing exp (const NumRing& base, const BigInt& bi);
