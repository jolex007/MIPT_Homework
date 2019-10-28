//
//  rational.h
//  RationalNumbers
//
//  Created by Алексей Зерцалов on 27.10.2019.
//  Copyright © 2019 Алексей Зерцалов. All rights reserved.
//

#ifndef rational_h
#define rational_h

#include <vector>
#include <string>
#include <iostream>

class BigInteger;
BigInteger abs (BigInteger element);
void swap (BigInteger &, BigInteger &);

class BigInteger
{
public:
    
    friend std::ostream & operator << (std::ostream &, const BigInteger &);
    friend std::istream & operator >> (std::istream &, BigInteger &);
    
    BigInteger & operator = (const BigInteger &);
    explicit operator bool();
    
    friend bool operator == (const BigInteger &, const BigInteger &);
    friend bool operator != (const BigInteger &, const BigInteger &);
    friend bool operator < (const BigInteger &, const BigInteger &);
    friend bool operator <= (const BigInteger &, const BigInteger &);
    friend bool operator > (const BigInteger &, const BigInteger &);
    friend bool operator >= (const BigInteger &, const BigInteger &);
    
    // Unary minus
    friend BigInteger operator - (const BigInteger &);
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
    
    BigInteger()
    {
        data.clear();
        data.push_back(0);
        sign = 0;
    }
    
    BigInteger(int element)
    {
        data.clear();
        if (element < 0) {
            element = -element;
            sign = 1;
        } else {
            sign = 0;
        }
        
        if (element == 0) {
            data.push_back(0);
            return;
        }
        
        while (element > 0) {
            data.push_back(element % BigInteger::base);
            element /= BigInteger::base;
        }
    }
    std::string toString () const;
    
private:
    
    std :: vector <unsigned char> data;
    char sign;     // 0 - if positive
                   // 1 - if negative
    
    static const unsigned char base = 10;
    
    void delete_leading_zeros();
    void check_for_zero();
    void normalize();
    void normalize(char);
    void fromString (const std::string & str);
    
    static BigInteger sum (const BigInteger & first, const BigInteger & second, char result_sign);
    static BigInteger substraction (const BigInteger & first, const BigInteger & second, char result_sign);
    static BigInteger mult (const BigInteger & first, const BigInteger & second, char result_sign);
    static BigInteger div (const BigInteger & first, const BigInteger & second, char result_sign);
    static BigInteger mod (const BigInteger & first, const BigInteger & second);
    static char compare_by_abs (const BigInteger & first, const BigInteger & second);
};


void BigInteger::delete_leading_zeros()
{
    while (data.size() > 1 && data.back() == 0) {
        data.pop_back();
    }
}

void BigInteger::check_for_zero()
{
    if (data.size() == 1 && data[0] == 0) {
        sign = 0;
    }
}

void BigInteger::normalize()
{
    delete_leading_zeros();
    check_for_zero();
}

void BigInteger::normalize(char need_sign)
{
    delete_leading_zeros();
    sign = need_sign;
    check_for_zero();
}

void BigInteger::fromString (const std::string & str)
{
    if (str.size() == 0) {
        return;
    }
    sign = (str[0] == '-' ? 1 : 0);
    
    data.clear();
    data.reserve(str.size());
    
    for (int i = static_cast<int>(str.size()) - 1, j = 0; i >= sign; i--, j++) {
        data.push_back(str[i] - '0');
    }
    
    normalize();
}

BigInteger BigInteger::sum (const BigInteger & first, const BigInteger & second, char result_sign)
{
    BigInteger result;
    size_t size = std::max(first.data.size(), second.data.size());
    result.sign = result_sign;
    result.data.clear();
    result.data.reserve(size + 1);
    
    unsigned char carry = 0;
    
    
    for (size_t i = 0; i < size; i++) {
        unsigned char first_term = (i < first.data.size() ? first.data[i] : 0);
        unsigned char second_term = (i < second.data.size() ? second.data[i] : 0);
        
        result.data.push_back(first_term + second_term + carry);
        
        if (result.data[i] >= BigInteger::base) {
            carry = 1;
            result.data[i] -= BigInteger::base;
        } else {
            carry = 0;
        }
    }
    if (carry) {
        result.data.push_back(carry);
    }
    
    result.normalize();
    
    return result;
}

BigInteger BigInteger::substraction (const BigInteger & first, const BigInteger & second, char result_sign)
{
    BigInteger result;
    size_t size = std::max(first.data.size(), second.data.size());
    result.sign = result_sign;
    result.data.clear();
    result.data.reserve(size + 1);
    
    unsigned char carry = 0;
    
    for (size_t i = 0; i < size; i++) {
        unsigned char first_term = (i < first.data.size() ? first.data[i] : 0);
        unsigned char second_term = (i < second.data.size() ? second.data[i] : 0);
        
        result.data.push_back(first_term - second_term - carry);
        
        if (result.data[i] >= BigInteger::base) {
            result.data[i] += BigInteger::base;
            carry = 1;
        } else {
            carry = 0;
        }
    }
    
    result.normalize();
    
    return result;
}

BigInteger BigInteger::mult (const BigInteger & first, const BigInteger & second, char result_sign)
{
    BigInteger result;
    size_t size = first.data.size() * second.data.size();
    result.sign = result_sign;
    result.data.clear();
    result.data.reserve(size + 1);
    
    unsigned char carry = 0;
    
    for (size_t i = 0; i < size + 1; i++) {
        result.data.push_back(carry);
        carry = 0;
        for (size_t j = 0; j <= i; j++) {
            unsigned char first_term = (j < first.data.size() ? first.data[j] : 0);
            unsigned char second_term = (i - j < second.data.size() ? second.data[i - j] : 0);
            
            result.data[i] += first_term * second_term;
            
            if (result.data[i] >= BigInteger::base) {
                carry += result.data[i] / BigInteger::base;
                result.data[i] %= BigInteger::base;
            }
        }
    }
    
    result.normalize();
    
    return result;
}

BigInteger BigInteger::div (const BigInteger & first, const BigInteger & second, char result_sign)
{
    if (second == 0) {
        
    }
    
    BigInteger result = 0;
    
    BigInteger num = abs(first), denom = abs(second), degree;
    
    while (num >= denom) {
        degree = 1;
        
        while (num >= denom) {
            denom *= BigInteger::base;
            degree *= BigInteger::base;
        }
        
        if (degree > 0) {
            denom.data.erase(denom.data.begin());
            degree.data.erase(degree.data.begin());
        }
        
        while (num >= denom) {
            num -= denom;
            result += degree;
        }
        
        denom = abs(second);
    }
    
    result.normalize(result_sign);
    
    return result;
}

BigInteger BigInteger::mod (const BigInteger & first, const BigInteger & second)
{
    if (second == 0) {

    }
    
    BigInteger result = 0;

    
    BigInteger num = abs(first), denom = abs(second), degree;
    
    while (num >= denom) {
        degree = 1;
        
        while (num >= denom) {
            denom *= BigInteger::base;
            degree *= BigInteger::base;
        }
        
        if (degree > 0) {
            denom.data.erase(denom.data.begin());
            degree.data.erase(degree.data.begin());
        }
        
        while (num >= denom) {
            num -= denom;
            result += degree;
        }
        
        denom = abs(second);
    }
    
    num.normalize(first.sign);
    
    return num;
}

// 0 - if equal
// -1 - if first < second
// 1 - if first > seocnd
char BigInteger::compare_by_abs (const BigInteger & first, const BigInteger & second)
{
    if (first.data.size() < second.data.size()) {
        return -1;
    } else if (first.data.size() > second.data.size()) {
        return 1;
    }
    
    size_t comp_size = first.data.size();
    
    for (int i = static_cast<int>(comp_size) - 1; i >= 0; i--) {
        if (first.data[i] < second.data[i]) {
            return -1;
        } else if (first.data[i] > second.data[i]) {
            return 1;
        }
    }
    
    return 0;
}

std::string BigInteger::toString () const
{
    std::string result;
    result.reserve(data.size() + 1);
    
    if (sign == 1) {
        result += '-';
    }
    
    for (int i = static_cast<int>(data.size()) - 1; i >= 0; i--) {
        result += data[i] + '0';
    }
    
    return result;
}
 
BigInteger abs (BigInteger element)
{
    return (element < 0 ? -element : element);
}

void swap (BigInteger & first, BigInteger & second)
{
    BigInteger t = first;
    first = second;
    second = t;
}

std::ostream & operator << (std::ostream & out, const BigInteger & number)
{
    out << number.toString();
    return out;
}

std::istream & operator >> (std::istream & in, BigInteger & number)
{
    std::string str;
    in >> str;
    number.fromString(str);
    return in;
}

BigInteger & BigInteger::operator = (const BigInteger & second)
{
    this -> data = second.data;
    this -> sign = second.sign;
    return *this;
}

BigInteger::operator bool()
{
    return (*this) != 0;
}

bool operator == (const BigInteger & first, const BigInteger & second)
{
    if (first.sign != second.sign) {
        return false;
    }
    return BigInteger::compare_by_abs(first, second) == 0;
}

bool operator != (const BigInteger & first, const BigInteger & second)
{
    if (first.sign != second.sign) {
        return true;
    }
    return BigInteger::compare_by_abs(first, second) != 0;
}

bool operator < (const BigInteger & first, const BigInteger & second)
{
    if (first.sign < second.sign) {
        return false;
    } else if (first.sign > second.sign) {
        return true;
    }
    
    if (first.sign == 0) {
        return BigInteger::compare_by_abs(first, second) == -1;
    } else {
        return BigInteger::compare_by_abs(first, second) == 1;
    }
}

bool operator <= (const BigInteger & first, const BigInteger & second)
{
    if (first.sign < second.sign) {
        return false;
    } else if (first.sign > second.sign) {
        return true;
    }
    
    if (first.sign == 0) {
        return BigInteger::compare_by_abs(first, second) <= 0;
    } else {
        return BigInteger::compare_by_abs(first, second) >= 0;
    }
}

bool operator > (const BigInteger & first, const BigInteger & second)
{
    if (first.sign > second.sign) {
        return false;
    } else if (first.sign < second.sign) {
        return true;
    }
    
    if (first.sign == 0) {
        return BigInteger::compare_by_abs(first, second) == 1;
    } else {
        return BigInteger::compare_by_abs(first, second) == -1;
    }
}

bool operator >= (const BigInteger & first, const BigInteger & second)
{
    if (first.sign > second.sign) {
        return false;
    } else if (first.sign < second.sign) {
        return true;
    }
    
    if (first.sign == 0) {
        return BigInteger::compare_by_abs(first, second) >= 0;
    } else {
        return BigInteger::compare_by_abs(first, second) <= 0;
    }
}

// Unary minus
BigInteger operator - (const BigInteger & number)
{
    BigInteger result = number;
    result.sign = (result.sign == 1 || (result.data.size() == 1 && result.data[0] == 0) ? 0 : 1);
    return result;
}

BigInteger & operator ++ (BigInteger & element)
{
    return element = element + 1;
}

BigInteger operator ++ (BigInteger & element, int)
{
    BigInteger old_value = element;
    element = element + 1;
    return old_value;
}

BigInteger & operator -- (BigInteger & element)
{
    return element = element - 1;
}

BigInteger operator -- (BigInteger & element, int)
{
    BigInteger old_value = element;
    element = element - 1;
    return old_value;
}



BigInteger operator + (const BigInteger & first, const BigInteger & second)
{
    if (first.sign == second.sign) {
        return BigInteger::sum(first, second, first.sign);
    }
    
    if (BigInteger::compare_by_abs(first, second) >= 0) {
        return BigInteger::substraction(first, second, first.sign);
    } else {
        return BigInteger::substraction(second, first, second.sign);
    }
}

BigInteger operator - (const BigInteger & first, const BigInteger & second)
{
    if (first.sign != second.sign) {
        return BigInteger::sum(first, second, first.sign);
    }
    
    if (BigInteger::compare_by_abs(first, second) >= 0) {
        return BigInteger::substraction(first, second, first.sign);
    } else {
        return BigInteger::substraction(second, first, second.sign == 0 ? 1 : 0);
    }
}

BigInteger operator * (const BigInteger & first, const BigInteger & second)
{
    return BigInteger::mult(first, second, first.sign == second.sign ? 0 : 1);
}

BigInteger operator / (const BigInteger & first, const BigInteger & second)
{
    return BigInteger::div(first, second, first.sign == second.sign ? 0 : 1);
}

BigInteger operator % (const BigInteger & first, const BigInteger & second)
{
    return BigInteger::mod(first, second);
}

BigInteger & operator += (BigInteger & first, const BigInteger & second)
{
    return first = first + second;
}

BigInteger & operator -= (BigInteger & first, const BigInteger & second)
{
    return first = first - second;
}

BigInteger & operator *= (BigInteger & first, const BigInteger & second)
{
    return first = first * second;
}

BigInteger & operator /= (BigInteger & first, const BigInteger & second)
{
    return first = first / second;
}

BigInteger & operator %= (BigInteger & first, const BigInteger & second)
{
    return first = first % second;
}

//
//
//
//
// RATIONAL NUMBERS
//
//
//
//


class Rational {
    
public:
    
//    friend int main();
    
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
    
    
    
    std::string toString();
    std::string asDecimal(size_t);
    
    Rational()
    {
        numerator = 0;
        denominator = 1;
        sign = 0;
    }
    
    Rational(BigInteger number)
    {
        numerator = abs(number);
        denominator = 1;
        sign = number.sign;
    }
    
    Rational(int number)
    {
        numerator = (number < 0 ? -number : number);
        denominator = 1;
        sign = (number < 0 ? 1 : 0);
    }
    
    explicit operator double()
    {
        return std::stod(asDecimal(10000));
    }
    
    
    
private:
    BigInteger numerator;
    BigInteger denominator;
    char sign;  // 0 - if positive
                // 1 - if negative
    
    BigInteger gcd(BigInteger a, BigInteger b);
    void normalize();
    static char compare_by_abs(const Rational &, const Rational &);
    static char compare(const Rational &, const Rational &);
    static BigInteger sum(const Rational &, const Rational &);
    
    static Rational sum (const Rational & first, const Rational & second, char result_sign);
    static Rational substraction (const Rational & first, const Rational & second, char result_sign);
    static Rational mult (const Rational & first, const Rational & second, char result_sign);
    static Rational div (const Rational & first, const Rational & second, char result_sign);
};

BigInteger Rational::gcd(BigInteger a, BigInteger b)
{
    while (b != 0) {
        a %= b;
        swap (a, b);
    }
    return a;
}

void Rational::normalize()
{
    BigInteger GCD = gcd(numerator, denominator);
    numerator /= GCD;
    denominator /= GCD;
    
    if (numerator == 0) {
        sign = 0;
        denominator = 1;
    }
}

std::string Rational::toString()
{
    return (sign == 0 ? "" : "-") + numerator.toString() + (denominator == 1 ? "" : "/" + denominator.toString());
}

char Rational::compare_by_abs(const Rational & first, const Rational & second)
{
    if (first.numerator == second.numerator && first.denominator == second.denominator) {
        return 0;
    }
    BigInteger num1 = first.numerator;
    BigInteger denom1 = first.denominator;
    BigInteger num2 = second.numerator;
    BigInteger denom2 = second.denominator;
    num1 *= denom2;
    num2 *= denom1;
    return BigInteger::compare_by_abs(num1, num2);
}

char Rational::compare(const Rational & first, const Rational & second)
{
    if (first.sign != second.sign) {
        return (first.sign == 1 ? -1 : 1);
    }
    if (first.numerator == second.numerator && first.denominator == second.denominator) {
        return 0;
    }
    return (first.sign == 0 ? compare_by_abs(first, second) : compare_by_abs(second, first));
}

std::string Rational::asDecimal(size_t precision = 0)
{
    BigInteger result = 0;
    
    BigInteger num = abs(numerator), denom = abs(denominator), degree;
    
    while (num >= denom) {
        degree = 1;
        
        while (num >= denom) {
            denom *= BigInteger::base;
            degree *= BigInteger::base;
        }
        
        if (degree > 1) {
            denom.data.erase(denom.data.begin());
            degree.data.erase(degree.data.begin());
        }
        
        while (num >= denom) {
            num -= denom;
            result += degree;
        }
        
        denom = abs(denominator);
    }
    
    result.normalize();
    
    std::string answer = (this -> sign == 0 ? "" : "-") + result.toString() + ".";
    
    for (size_t i = 0; i < precision; i++) {
        degree = 1;
        num *= BigInteger::base;
        
        while (num >= denom) {
            denom *= BigInteger::base;
            degree *= BigInteger::base;
        }
        
        if (degree > 1) {
            denom.data.erase(denom.data.begin());
            degree.data.erase(degree.data.begin());
        }
        
        BigInteger t = 0;
        
        while (num >= denom) {
            num -= denom;
            t += degree;
        }
        
        answer += t.toString();
        
        denom = abs(denominator);
    }
    
    return answer;
}

Rational Rational::sum (const Rational & first, const Rational & second, char result_sign)
{
    Rational result;
    result.numerator = first.numerator * second.denominator + second.numerator * first.denominator;
    result.denominator = first.denominator * second.denominator;
    result.sign = result_sign;
    result.normalize();
    return result;
}

Rational Rational::substraction (const Rational & first, const Rational & second, char result_sign)
{
    Rational result;
    result.numerator = first.numerator * second.denominator - second.numerator * first.denominator;
    result.denominator = first.denominator * second.denominator;
    result.sign = result_sign;
    result.normalize();
    return result;
}

Rational Rational::mult (const Rational & first, const Rational & second, char result_sign)
{
    Rational result;
    result.numerator = first.numerator * second.numerator;
    result.denominator = first.denominator * second.denominator;
    result.sign = result_sign;
    result.normalize();
    return result;
}

Rational Rational::div (const Rational & first, const Rational & second, char result_sign)
{
    if (second == 0) {
        
    }
    Rational result;
    result.numerator = first.numerator * second.denominator;
    result.denominator = first.denominator * second.numerator;
    result.sign = result_sign;
    result.normalize();
    return result;
}

Rational & Rational::operator = (const Rational & second)
{
    this -> numerator = second.numerator;
    this -> denominator = second.denominator;
    this -> sign = second.sign;
    return *this;
}

bool operator == (const Rational & first, const Rational & second)
{
    return Rational::compare(first, second) == 0;
}

bool operator != (const Rational & first, const Rational & second)
{
    return Rational::compare(first, second) != 0;
}

bool operator > (const Rational & first, const Rational & second)
{
    return Rational::compare(first, second) > 0;
}

bool operator >= (const Rational & first, const Rational & second)
{
    return Rational::compare(first, second) >= 0;
}

bool operator < (const Rational & first, const Rational & second)
{
    return Rational::compare(first, second) < 0;
}

bool operator <= (const Rational & first, const Rational & second)
{
    return Rational::compare(first, second) <= 0;
}



Rational operator - (const Rational & element)
{
    Rational result = element;
    result.sign = (result.sign == 0 ? 1 : 0);
    return result;
}

Rational operator + (const Rational & first, const Rational & second)
{
    if (first.sign == second.sign) {
        return Rational::sum(first, second, first.sign);
    }
    
    if (Rational::compare_by_abs(first, second) >= 0) {
        return Rational::substraction(first, second, first.sign);
    } else {
        return Rational::substraction(second, first, second.sign);
    }
}


Rational operator - (const Rational & first, const Rational & second)
{
    if (first.sign != second.sign) {
        return Rational::sum(first, second, first.sign);
    }
    
    if (Rational::compare_by_abs(first, second) >= 0) {
        return Rational::substraction(first, second, first.sign);
    } else {
        return Rational::substraction(second, first, second.sign == 0 ? 1 : 0);
    }
}

Rational operator * (const Rational & first, const Rational & second)
{
    return Rational::mult(first, second, first.sign == second.sign ? 0 : 1);
}

Rational operator / (const Rational & first, const Rational & second)
{
    return Rational::div(first, second, first.sign == second.sign ? 0 : 1);
}

Rational & operator += (Rational & first, const Rational & second)
{
    return first = first + second;
}

Rational & operator -= (Rational & first, const Rational & second)
{
    return first = first - second;
}

Rational & operator *= (Rational & first, const Rational & second)
{
    return first = first * second;
}

Rational & operator /= (Rational & first, const Rational & second)
{
    return first = first / second;
}


#endif /* rational_h */
