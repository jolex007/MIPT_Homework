//
//  BigInteger.hpp
//  Matrix
//
//  Created by Алексей Зерцалов on 11.03.2020.
//  Copyright © 2020 Алексей Зерцалов. All rights reserved.
//

#ifndef BigInteger_hpp
#define BigInteger_hpp

#include <vector>
#include <string>
#include <iostream>

enum SignType {
    POSITIVE = 0,
    NEGATIVE = 1
};

enum CompareType {
    LOWER,
    EQUAL,
    GREATER
};



class BigInteger;

BigInteger abs (const BigInteger &);
void swap (BigInteger &, BigInteger &);
BigInteger gcd (BigInteger, BigInteger);

class BigInteger
{
public:
    
    // Constructors
    BigInteger();
    BigInteger(int);
    
    // Streams overloading
    friend std::ostream & operator << (std::ostream &, const BigInteger &);
    friend std::istream & operator >> (std::istream &, BigInteger &);
    
    // Opertors overloading
    BigInteger & operator = (const BigInteger &);
    explicit operator bool();
    
    friend bool operator == (const BigInteger &, const BigInteger &);
    friend bool operator != (const BigInteger &, const BigInteger &);
    friend bool operator < (const BigInteger &, const BigInteger &);
    friend bool operator <= (const BigInteger &, const BigInteger &);
    friend bool operator > (const BigInteger &, const BigInteger &);
    friend bool operator >= (const BigInteger &, const BigInteger &);
    
    friend BigInteger operator - (const BigInteger &); // Unary minus
    
    friend BigInteger & operator ++ (BigInteger &);
    friend BigInteger operator ++ (BigInteger &, int);
    friend BigInteger & operator -- (BigInteger &);
    friend BigInteger operator -- (BigInteger &, int);
    
    friend BigInteger operator + (const BigInteger &, const BigInteger &);
    friend BigInteger operator - (const BigInteger &, const BigInteger &);
    friend BigInteger operator * (const BigInteger &, const BigInteger &);
    friend BigInteger operator / (const BigInteger &, const BigInteger &);
    friend BigInteger operator % (const BigInteger &, const BigInteger &);
    
    friend BigInteger & operator += (BigInteger &, const BigInteger &);
    friend BigInteger & operator -= (BigInteger &, const BigInteger &);
    friend BigInteger & operator *= (BigInteger &, const BigInteger &);
    friend BigInteger & operator /= (BigInteger &, const BigInteger &);
    friend BigInteger & operator %= (BigInteger &, const BigInteger &);
    
    friend class Rational;
    
    // BigInteger to std::string
    std::string toString() const;
    
private:
    // Class fields
    std :: vector <unsigned char> data_;
    SignType sign_;
    
    // Base number system
    static const unsigned char Base = 10;
    
    // Secondary functions
    void deleteLeadingZeros();
    void checkForZero();
    void normalize();
    void normalize(SignType);
    void fromString (const std::string &);
    
    static BigInteger& sumAndSub (BigInteger &, const BigInteger &, SignType, bool, bool = true);
    static BigInteger mult (const BigInteger &, const BigInteger &, SignType);
    static BigInteger divAndMod (const BigInteger &, const BigInteger &, SignType, bool = true);
    static CompareType compareByAbs (const BigInteger &, const BigInteger &);
    static CompareType compare (const BigInteger &, const BigInteger &);
};

#endif /* BigInteger_hpp */
