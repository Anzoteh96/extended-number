#ifndef BIG_INT_H
#define BIG_INT_H
#include <vector>
#include <iostream>
#include <string>

const long long int limit = 2147483648;

class BigInt {
    std::vector<unsigned int> numVec;
    bool sign;
    //private helper functions
    void removeLeadZero();
    std::string prettyPrint() const;
public:
    void shift(unsigned int m);
    BigInt();
    BigInt(std::vector<unsigned int>numVec, bool sign);
    BigInt(unsigned int num, bool sign);
    explicit BigInt(int num);
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
    //BigInt max(BigInt other);
    std::vector<unsigned int> getVec() const;
    bool getSign() const;
    friend std::ostream &operator<<(std::ostream &out, const BigInt &bi);
    friend std::istream &operator>>(std::istream &in, BigInt &bi);
};

BigInt abs(const BigInt& other);
#endif
