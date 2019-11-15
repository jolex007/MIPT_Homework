#ifndef rational_h
#define rational_h

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

BigInteger::BigInteger()
{
    data_.clear();
    data_.push_back(0);
    sign_ = POSITIVE;
}

BigInteger::BigInteger(int element)
{
    data_.clear();
    if (element < 0) {
        element = -element;
        sign_ = NEGATIVE;
    } else {
        sign_ = POSITIVE;
    }
    
    if (element == 0) {
        data_.push_back(0);
        return;
    }
    
    while (element > 0) {
        data_.push_back(element % BigInteger::Base);
        element /= BigInteger::Base;
    }
}


void BigInteger::deleteLeadingZeros()
{
    while (data_.size() > 1 && data_.back() == 0) {
        data_.pop_back();
    }
}

void BigInteger::checkForZero()
{
    if (data_.size() == 1 && data_[0] == 0) {
        sign_ = POSITIVE;
    }
}

void BigInteger::normalize()
{
    deleteLeadingZeros();
    checkForZero();
}

void BigInteger::normalize(SignType need_sign_)
{
    deleteLeadingZeros();
    sign_ = need_sign_;
    checkForZero();
}

void BigInteger::fromString (const std::string & str)
{
    if (str.size() == 0) {
        return;
    }
    sign_ = (str[0] == '-' ? NEGATIVE : POSITIVE);
    
    data_.clear();
    data_.reserve(str.size());
    
    for (int i = static_cast<int>(str.size()) - 1, j = 0; i >= sign_; i--, j++) {
        data_.push_back(str[i] - '0');
    }
    
    normalize();
}

BigInteger& BigInteger::sumAndSub (BigInteger & first, const BigInteger & second, SignType result_sign, bool is_sum, bool is_first_greater)
{
    first.sign_ = result_sign;
    unsigned char carry = 0;
    
    size_t size = std::max(first.data_.size(), second.data_.size()) + 1;
    first.data_.resize(size, 0);
    
    size_t i;
    for (i = 0; i < size; i++) {
        unsigned char first_term = (i < first.data_.size() ? first.data_[i] : 0);
        unsigned char second_term = (i < second.data_.size() ? second.data_[i] : 0);
        
        first.data_[i] = (is_sum ?
                          first_term + second_term + carry :
                          (is_first_greater ? first_term - second_term - carry : second_term - first_term - carry)
                          );
        
        if (first.data_[i] >= BigInteger::Base) {
            first.data_[i] += (is_sum ? -BigInteger::Base : BigInteger::Base);
            carry = 1;
        } else {
            carry = 0;
        }
    }
    
    if (carry && is_sum) {
        first.data_[i] = carry;
    }
    
    first.normalize();
    
    return first;
}

BigInteger BigInteger::mult (const BigInteger & first, const BigInteger & second, SignType result_sign_)
{
    BigInteger result;
    size_t size = first.data_.size() * second.data_.size();
    result.sign_ = result_sign_;
    result.data_.clear();
    result.data_.reserve(size + 1);
    
    unsigned char carry = 0;
    
    for (size_t i = 0; i < size + 1; i++) {
        result.data_.push_back(carry);
        carry = 0;
        for (size_t j = 0; j <= i; j++) {
            unsigned char first_term = (j < first.data_.size() ? first.data_[j] : 0);
            unsigned char second_term = (i - j < second.data_.size() ? second.data_[i - j] : 0);
            
            result.data_[i] += first_term * second_term;
            
            if (result.data_[i] >= BigInteger::Base) {
                carry += result.data_[i] / BigInteger::Base;
                result.data_[i] %= BigInteger::Base;
            }
        }
    }
    
    result.normalize();
    
    return result;
}

BigInteger BigInteger::divAndMod (const BigInteger & first, const BigInteger & second, SignType result_sign_, bool is_div)
{
    if (second == 0) {
        std::cerr << "Devision by zero";
    }
    
    BigInteger result = 0;
    
    BigInteger num = abs(first), denom = abs(second), degree;
    
    while (num >= denom) {
        degree = 1;
        
        while (num >= denom) {
            denom *= BigInteger::Base;
            degree *= BigInteger::Base;
        }
        
        if (degree > 0) {
            denom.data_.erase(denom.data_.begin());
            degree.data_.erase(degree.data_.begin());
        }
        
        while (num >= denom) {
            num -= denom;
            result += degree;
        }
        
        denom = abs(second);
    }
    
    if (is_div) {
        result.normalize(result_sign_);
        return result;
    } else {
        num.normalize(first.sign_);
        return num;
    }
}

CompareType BigInteger::compareByAbs (const BigInteger & first, const BigInteger & second)
{
    if (first.data_.size() < second.data_.size()) {
        return LOWER;
    } else if (first.data_.size() > second.data_.size()) {
        return GREATER;
    }
    
    size_t comp_size = first.data_.size();
    
    for (int i = static_cast<int>(comp_size) - 1; i >= 0; i--) {
        if (first.data_[i] < second.data_[i]) {
            return LOWER;
        } else if (first.data_[i] > second.data_[i]) {
            return GREATER;
        }
    }
    
    return EQUAL;
}

CompareType BigInteger::compare (const BigInteger & first, const BigInteger & second)
{
    if (first.sign_ > second.sign_) {
        return LOWER;
    } else if (first.sign_ < second.sign_) {
        return GREATER;
    }
    
    return (first.sign_ ? BigInteger::compareByAbs(second, first) : BigInteger::compareByAbs(first, second));
}

std::string BigInteger::toString () const
{
    std::string result;
    result.reserve(data_.size() + 1);
    
    if (sign_ == NEGATIVE) {
        result += '-';
    }
    
    for (int i = static_cast<int>(data_.size()) - 1; i >= 0; i--) {
        result += data_[i] + '0';
    }
    
    return result;
}
 
BigInteger abs (const BigInteger & element)
{
    return (element < 0 ? -element : element);
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
    this -> data_ = second.data_;
    this -> sign_ = second.sign_;
    return *this;
}

BigInteger::operator bool()
{
    return (*this) != 0;
}

bool operator == (const BigInteger & first, const BigInteger & second)
{
    return BigInteger::compare(first, second) == EQUAL;
}

bool operator != (const BigInteger & first, const BigInteger & second)
{
    return BigInteger::compare(first, second) != EQUAL;
}

bool operator < (const BigInteger & first, const BigInteger & second)
{
     return BigInteger::compare(first, second) == LOWER;
}

bool operator <= (const BigInteger & first, const BigInteger & second)
{
    return BigInteger::compare(first, second) != GREATER;
}

bool operator > (const BigInteger & first, const BigInteger & second)
{
    return BigInteger::compare(first, second) == GREATER;
}

bool operator >= (const BigInteger & first, const BigInteger & second)
{
    return BigInteger::compare(first, second) != LOWER;
}

// Unary minus
BigInteger operator - (const BigInteger & number)
{
    BigInteger result = number;
    result.sign_ = (result.sign_ == NEGATIVE || (result.data_.size() == 1 && result.data_[0] == 0) ? POSITIVE : NEGATIVE);
    return result;
}

// Prefix increment
BigInteger & operator ++ (BigInteger & element)
{
    return element += 1;
}

// Postfix increment
BigInteger operator ++ (BigInteger & element, int)
{
    BigInteger old_value = element;
    element += 1;
    return old_value;
}

// Prexif decrement
BigInteger & operator -- (BigInteger & element)
{
    return element -= 1;
}

// Postfix decrement
BigInteger operator -- (BigInteger & element, int)
{
    BigInteger old_value = element;
    element -= 1;
    return old_value;
}

BigInteger operator + (const BigInteger & first, const BigInteger & second)
{
    BigInteger result = first;
    return result += second;
}

BigInteger operator - (const BigInteger & first, const BigInteger & second)
{
    BigInteger result = first;
    return result -= second;
}

BigInteger operator * (const BigInteger & first, const BigInteger & second)
{
    return BigInteger::mult(first, second, first.sign_ == second.sign_ ? POSITIVE : NEGATIVE);
}

BigInteger operator / (const BigInteger & first, const BigInteger & second)
{
    return BigInteger::divAndMod(first, second, first.sign_ == second.sign_ ? POSITIVE : NEGATIVE, true);
}

BigInteger operator % (const BigInteger & first, const BigInteger & second)
{
    return BigInteger::divAndMod(first, second, POSITIVE, false);
}


BigInteger & operator += (BigInteger & first, const BigInteger & second)
{
    if (first.sign_ == second.sign_) {
        return BigInteger::sumAndSub(first, second, first.sign_, true);
    }
    
    if (BigInteger::compareByAbs(first, second) != LOWER) {
        return BigInteger::sumAndSub(first, second, first.sign_, false);
    } else {
        return BigInteger::sumAndSub(first, second, second.sign_, false, false);
    }
}

BigInteger & operator -= (BigInteger & first, const BigInteger & second)
{
    if (first.sign_ != second.sign_) {
        return BigInteger::sumAndSub(first, second, first.sign_, true);
    }
    
    if (BigInteger::compareByAbs(first, second) != LOWER) {
        return BigInteger::sumAndSub(first, second, first.sign_, false);
    } else {
        return BigInteger::sumAndSub(first, second, second.sign_ == 0 ? NEGATIVE : POSITIVE, false, false);
    }
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

void swap (BigInteger & first, BigInteger & second)
{
    BigInteger t = first;
    first = second;
    second = t;
}

BigInteger gcd(BigInteger a, BigInteger b)
{
    while (b != 0) {
        a %= b;
        swap(a, b);
    }
    return a;
}


//
//
// RATIONAL NUMBERS
//
//


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
    
    // Rational to std::string
    std::string toString();
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
    return std::stod(asDecimal(10000));
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

std::string Rational::toString()
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


#endif /* rational_h */
