#ifndef BIG_INT_H
#define BIG_INT_H
#include <vector>
#include <iostream>
#include <string>

const int limit = 2;
//New revolution: put everything into 0 and 1
//For one, closer to the machine code arithmetic mechanism;
//For two, easier to do bitwise: needed for the Num_Ring later

class BigInt {
    std::vector<bool> numVec;
    bool sign;
    //private helper functions
    void removeLeadZero();
    std::string prettyPrint() const;
public:
    BigInt();
    BigInt(std::vector<bool>numVec, bool sign);
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
    BigInt& operator+=(const BigInt& other);
    BigInt& operator-=(const BigInt& other);
    BigInt& operator*=(const BigInt& other);
    BigInt& operator/=(const BigInt& other);
    BigInt& operator%=(const BigInt& other);
    BigInt operator<<(int m) const;
    BigInt operator>>(int m) const;
    std::vector<bool> getVec() const;
    bool getSign() const;
    friend std::ostream &operator<<(std::ostream &out, const BigInt &bi);
    friend std::istream &operator>>(std::istream &in, BigInt &bi);
};

BigInt abs(const BigInt& other);
BigInt gcd(const BigInt& b1, const BigInt& b2);
#endif
