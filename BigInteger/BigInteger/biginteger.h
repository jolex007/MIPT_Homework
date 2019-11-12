#ifndef BigInteger_h
#define BigInteger_h

#include <vector>
#include <string>
#include <iostream>

class BigInteger;
BigInteger abs (const BigInteger &);

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
    
    // BigInteger to std::string
    std::string toString() const;
    
private:
    
    enum Sign {
        POSITIVE = 0,
        NEGATIVE = 1
    };
    
    enum Comp {
        LOWER,
        EQUAL,
        GREATER
    };
    
    std :: vector <unsigned char> data_;
    Sign sign_;
    
    static const unsigned char Base = 10;
    
    // Secondary functions
    void delete_leading_zeros();
    void check_for_zero();
    void normalize();
    void normalize(Sign);
    void fromString (const std::string &);
    
    static BigInteger& sum_and_sub (BigInteger &, const BigInteger &, Sign, bool, bool = true);
    static BigInteger mult (const BigInteger &, const BigInteger &, Sign);
    static BigInteger div_and_mod (const BigInteger &, const BigInteger &, Sign, bool = true);
    static BigInteger::Comp compare_by_abs (const BigInteger &, const BigInteger &);
    static BigInteger::Comp compare (const BigInteger &, const BigInteger &);
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


void BigInteger::delete_leading_zeros()
{
    while (data_.size() > 1 && data_.back() == 0) {
        data_.pop_back();
    }
}

void BigInteger::check_for_zero()
{
    if (data_.size() == 1 && data_[0] == 0) {
        sign_ = POSITIVE;
    }
}

void BigInteger::normalize()
{
    delete_leading_zeros();
    check_for_zero();
}

void BigInteger::normalize(BigInteger::Sign need_sign_)
{
    delete_leading_zeros();
    sign_ = need_sign_;
    check_for_zero();
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

BigInteger& BigInteger::sum_and_sub (BigInteger & first, const BigInteger & second, BigInteger::Sign result_sign, bool is_sum, bool is_first_greater)
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

BigInteger BigInteger::mult (const BigInteger & first, const BigInteger & second, BigInteger::Sign result_sign_)
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

BigInteger BigInteger::div_and_mod (const BigInteger & first, const BigInteger & second, BigInteger::Sign result_sign_, bool is_div)
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

BigInteger::Comp BigInteger::compare_by_abs (const BigInteger & first, const BigInteger & second)
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

BigInteger::Comp BigInteger::compare (const BigInteger & first, const BigInteger & second)
{
    if (first.sign_ > second.sign_) {
        return LOWER;
    } else if (first.sign_ < second.sign_) {
        return GREATER;
    }
    
    return (first.sign_ ? BigInteger::compare_by_abs(second, first) : BigInteger::compare_by_abs(first, second));
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
    return BigInteger::compare(first, second) == BigInteger::EQUAL;
}

bool operator != (const BigInteger & first, const BigInteger & second)
{
    return BigInteger::compare(first, second) != BigInteger::EQUAL;
}

bool operator < (const BigInteger & first, const BigInteger & second)
{
     return BigInteger::compare(first, second) == BigInteger::LOWER;
}

bool operator <= (const BigInteger & first, const BigInteger & second)
{
    return BigInteger::compare(first, second) != BigInteger::GREATER;
}

bool operator > (const BigInteger & first, const BigInteger & second)
{
    return BigInteger::compare(first, second) == BigInteger::GREATER;
}

bool operator >= (const BigInteger & first, const BigInteger & second)
{
    return BigInteger::compare(first, second) != BigInteger::LOWER;
}

// Unary minus
BigInteger operator - (const BigInteger & number)
{
    BigInteger result = number;
    result.sign_ = (result.sign_ == BigInteger::NEGATIVE || (result.data_.size() == 1 && result.data_[0] == 0) ? BigInteger::POSITIVE : BigInteger::NEGATIVE);
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
    return BigInteger::mult(first, second, first.sign_ == second.sign_ ? BigInteger::POSITIVE : BigInteger::NEGATIVE);
}

BigInteger operator / (const BigInteger & first, const BigInteger & second)
{
    return BigInteger::div_and_mod(first, second, first.sign_ == second.sign_ ? BigInteger::POSITIVE : BigInteger::NEGATIVE, true);
}

BigInteger operator % (const BigInteger & first, const BigInteger & second)
{
    return BigInteger::div_and_mod(first, second, BigInteger::POSITIVE, false);
}


BigInteger & operator += (BigInteger & first, const BigInteger & second)
{
    if (first.sign_ == second.sign_) {
        return BigInteger::sum_and_sub(first, second, first.sign_, true);
    }
    
    if (BigInteger::compare_by_abs(first, second) != BigInteger::LOWER) {
        return BigInteger::sum_and_sub(first, second, first.sign_, false);
    } else {
        return BigInteger::sum_and_sub(first, second, second.sign_, false, false);
    }
}

BigInteger & operator -= (BigInteger & first, const BigInteger & second)
{
    if (first.sign_ != second.sign_) {
        return BigInteger::sum_and_sub(first, second, first.sign_, true);
    }
    
    if (BigInteger::compare_by_abs(first, second) != BigInteger::LOWER) {
        return BigInteger::sum_and_sub(first, second, first.sign_, false);
    } else {
        return BigInteger::sum_and_sub(first, second, second.sign_ == 0 ? BigInteger::NEGATIVE : BigInteger::POSITIVE, false, false);
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

#endif /* BigInteger_h */
