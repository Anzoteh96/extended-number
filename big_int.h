#ifndef BIG_INT_H
#define BIG_INT_H
#include <vector>
#include <iostream>

const long long int limit = 2147483648;

class BigInt {
    std::vector<unsigned int> numVec;
    bool sign;
    //private helper functions
    void removeLeadZero();
public:
    void shift(unsigned int m);
    BigInt(std::vector<unsigned int>numVec, bool sign);
    BigInt(unsigned int num, bool sign);
    //explicit BigInt(int num);
    //negation and comparators
    BigInt operator-() const;
    bool operator<(const BigInt& other) const;
    bool operator>(const BigInt& other) const;
    bool operator<=(const BigInt& other) const;
    bool operator>=(const BigInt& other) const;
    bool operator==(const BigInt& other) const;
    bool operator!=(const BigInt& other) const;
    //arithmetic operators
    BigInt operator+(const BigInt& other) const;
    BigInt operator-(const BigInt& other) const;
    BigInt operator*(const BigInt& other) const;
    BigInt operator/(const BigInt& other) const;
    BigInt operator%(const BigInt& other) const;
    BigInt abs(const BigInt& other) const;
    //BigInt max(BigInt other);
    std::vector<unsigned int> getVec() const;
    bool getSign() const;
};
#endif
