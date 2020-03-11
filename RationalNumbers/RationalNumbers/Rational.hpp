#ifndef Rational_hpp
#define Rational_hpp

#include <vector>
#include <string>
#include <iostream>
#include "BigInteger.hpp"

class Rational {
    
public:
    
    // Constructors
    Rational();
    Rational(BigInteger);
    Rational(int);
    
    // Opertors overloading
    Rational & operator = (const Rational &);
    
    friend Rational operator - (const Rational &);
    
    friend bool operator == (const Rational &, const Rational &);
    friend bool operator != (const Rational &, const Rational &);
    friend bool operator < (const Rational &, const Rational &);
    friend bool operator <= (const Rational &, const Rational &);
    friend bool operator > (const Rational &, const Rational &);
    friend bool operator >= (const Rational &, const Rational &);
    
    friend Rational operator + (const Rational &, const Rational &);
    friend Rational operator - (const Rational &, const Rational &);
    friend Rational operator * (const Rational &, const Rational &);
    friend Rational operator / (const Rational &, const Rational &);
    
    friend Rational & operator += (Rational &, const Rational &);
    friend Rational & operator -= (Rational &, const Rational &);
    friend Rational & operator *= (Rational &, const Rational &);
    friend Rational & operator /= (Rational &, const Rational &);
    
    friend std::ostream& operator<< (std::ostream&, Rational const&);
    friend std::istream& operator>> (std::istream&, Rational&);
    
    // Rational to std::string
    std::string toString() const;
    std::string asDecimal(size_t);
    
    // Rational to double
    explicit operator double();
    
    
private:
    
    // Class fields
    BigInteger numerator_;
    BigInteger denominator_;
    SignType sign_;
    
    
    // Secondary functions
    void normalize();
    void divisionForDecimal(std::string &, BigInteger &, BigInteger &, bool, size_t = 0);
    static CompareType compareByAbs(const Rational &, const Rational &);
    static CompareType compare(const Rational &, const Rational &);
    static Rational sumAndSub (const Rational &, const Rational &, SignType, bool);
    static Rational& multAndDiv (Rational &, const Rational &, SignType, bool = true);
};


#endif /* Rational_hpp */


