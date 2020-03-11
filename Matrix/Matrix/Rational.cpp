//
//  Rational.cpp
//  Matrix
//
//  Created by Алексей Зерцалов on 11.03.2020.
//  Copyright © 2020 Алексей Зерцалов. All rights reserved.
//

#include "Rational.hpp"

Rational::Rational()
{
    numerator_ = 0;
    denominator_ = 1;
    sign_ = POSITIVE;
}

Rational::Rational(BigInteger number)
{
    numerator_ = abs(number);
    denominator_ = 1;
    sign_ = number.sign_;
}

Rational::Rational(int number)
{
    numerator_ = (number < 0 ? -number : number);
    denominator_ = 1;
    sign_ = (number < 0 ? NEGATIVE : POSITIVE);
}

Rational::operator double()
{
    return std::stod(asDecimal(1000));
}

void Rational::normalize()
{
    BigInteger GCD = gcd(numerator_, denominator_);
    numerator_ /= GCD;
    denominator_ /= GCD;
    
    if (numerator_ == 0) {
        sign_ = POSITIVE;
        denominator_ = 1;
    }
}

std::string Rational::toString() const
{
    return (sign_ == POSITIVE ? "" : "-") + numerator_.toString() + (denominator_ == 1 ? "" : "/" + denominator_.toString());
}

CompareType Rational::compareByAbs(const Rational & first, const Rational & second)
{
    if (first.numerator_ == second.numerator_ && first.denominator_ == second.denominator_) {
        return EQUAL;
    }
    return BigInteger::compareByAbs(first.numerator_ * second.denominator_, second.numerator_ * first.denominator_);
}

CompareType Rational::compare(const Rational & first, const Rational & second)
{
    if (first.sign_ != second.sign_) {
        return (first.sign_ == NEGATIVE ? LOWER : GREATER);
    }
    if (first.numerator_ == second.numerator_ && first.denominator_ == second.denominator_) {
        return EQUAL;
    }
    return (first.sign_ == POSITIVE ? compareByAbs(first, second) : compareByAbs(second, first));
}

void Rational::divisionForDecimal(std::string & answer, BigInteger & num, BigInteger & denom, bool is_integer_part, size_t precision)
{
    BigInteger degree;
    BigInteger result = 0;

    for (size_t i = 0; (is_integer_part ? num >= denom : i < precision) ; i++) {
        degree = 1;

        if (!is_integer_part) {
            result = 0;
            num *= BigInteger::Base;
        }

        while (num >= denom) {
            denom *= BigInteger::Base;
            degree *= BigInteger::Base;
        }

        if (degree > 1) {
            denom.data_.erase(denom.data_.begin());
            degree.data_.erase(degree.data_.begin());
        }

        while (num >= denom) {
            num -= denom;
            result += degree;
        }

        if (!is_integer_part) {
            answer += result.toString();
        }

        denom = abs(denominator_);
    }

    if (is_integer_part) {
        result.normalize();
        answer += result.toString();
    }
}

std::string Rational::asDecimal(size_t precision = 0)
{
    BigInteger num = abs(numerator_);
    BigInteger denom = abs(denominator_);

    std::string answer = (this -> sign_ == POSITIVE ? "" : "-");

    divisionForDecimal(answer, num, denom, true);

    if (precision > 0) {
        answer += '.';
        divisionForDecimal(answer, num, denom, false, precision);
    }

    return answer;
}

Rational Rational::sumAndSub (const Rational & first, const Rational & second, SignType result_sign, bool is_sum)
{
    Rational result;
    result.numerator_ = first.numerator_ * second.denominator_ + second.numerator_ * first.denominator_ * (is_sum ? 1 : -1);
    result.denominator_ = first.denominator_ * second.denominator_;
    result.sign_ = result_sign;
    result.normalize();
    return result;
}

Rational& Rational::multAndDiv (Rational & first, const Rational & second, SignType result_sign, bool is_mult)
{
    if (!is_mult && second == 0) {
        std::cerr << "Division by zero!";
    }
    first.numerator_ *= (is_mult ? second.numerator_ : second.denominator_);
    first.denominator_ *= (is_mult ? second.denominator_ : second.numerator_);
    first.sign_ = result_sign;
    first.normalize();
    return first;
}

Rational & Rational::operator = (const Rational & second)
{
    this -> numerator_ = second.numerator_;
    this -> denominator_ = second.denominator_;
    this -> sign_ = second.sign_;
    return *this;
}

bool operator == (const Rational & first, const Rational & second)
{
    return Rational::compare(first, second) == EQUAL;
}

bool operator != (const Rational & first, const Rational & second)
{
    return Rational::compare(first, second) != EQUAL;
}

bool operator > (const Rational & first, const Rational & second)
{
    return Rational::compare(first, second) == GREATER;
}

bool operator >= (const Rational & first, const Rational & second)
{
    return Rational::compare(first, second) != LOWER;
}

bool operator < (const Rational & first, const Rational & second)
{
    return Rational::compare(first, second) == LOWER;
}

bool operator <= (const Rational & first, const Rational & second)
{
    return Rational::compare(first, second) != GREATER;
}

Rational operator - (const Rational & element)
{
    Rational result = element;
    result.sign_ = (result.sign_ == POSITIVE ? NEGATIVE : POSITIVE);
    return result;
}

Rational operator + (const Rational & first, const Rational & second)
{
    if (first.sign_ == second.sign_) {
        return Rational::sumAndSub(first, second, first.sign_, true);
    }
    
    if (Rational::compareByAbs(first, second) != LOWER) {
        return Rational::sumAndSub(first, second, first.sign_, false);
    } else {
        return Rational::sumAndSub(second, first, second.sign_, false);
    }
}

Rational operator - (const Rational & first, const Rational & second)
{
    if (first.sign_ != second.sign_) {
        return Rational::sumAndSub(first, second, first.sign_, true);
    }
    
    if (Rational::compareByAbs(first, second) != LOWER) {
        return Rational::sumAndSub(first, second, first.sign_, false);
    } else {
        return Rational::sumAndSub(second, first, second.sign_ == POSITIVE ? NEGATIVE : POSITIVE, false);
    }
}

Rational operator * (const Rational & first, const Rational & second)
{
    Rational result = first;
    return result *= second;
}

Rational operator / (const Rational & first, const Rational & second)
{
    Rational result = first;
    return result /= second;
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
    return Rational::multAndDiv(first, second, first.sign_ == second.sign_ ? POSITIVE : NEGATIVE, true);
}

Rational & operator /= (Rational & first, const Rational & second)
{
    return Rational::multAndDiv(first, second, first.sign_ == second.sign_ ? POSITIVE : NEGATIVE, false);
}

std::ostream& operator<< (std::ostream& out, Rational const& elem)
{
    out << elem.toString();
    return out;
}

std::istream& operator>> (std::istream& in, Rational& elem)
{
    BigInteger inp;
    in >> inp;
    elem = inp;
    return in;
}
