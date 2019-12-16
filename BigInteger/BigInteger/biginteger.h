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
    
    enum _SignType {
        POSITIVE = 0,
        NEGATIVE = 1
    };
    
    enum _CompareType {
        LOWER,
        EQUAL,
        GREATER
    };
    
    std :: vector <unsigned char> _data;
    _SignType _sign;
    
    static const unsigned char BASE = 10;
    
    // Secondary functions
    void _delete_leading_zeros();
    void _check_for_zero();
    void _normalize();
    void _normalize(_SignType);
    void _fromString (const std::string &);
    
    static BigInteger& _sum_and_sub (BigInteger &, const BigInteger &, _SignType, bool, bool = true);
    static BigInteger& _mult (BigInteger &, const BigInteger &, _SignType);
    static BigInteger& _div_and_mod (BigInteger &, const BigInteger &, _SignType, bool = true);
    static BigInteger::_CompareType _compare_by_abs (const BigInteger &, const BigInteger &);
    static BigInteger::_CompareType _compare (const BigInteger &, const BigInteger &);
};

BigInteger::BigInteger()
{
    _data.clear();
    _data.push_back(0);
    _sign = POSITIVE;
}

BigInteger::BigInteger(int element)
{
    _data.clear();
    if (element < 0) {
        element = -element;
        _sign = NEGATIVE;
    } else {
        _sign = POSITIVE;
    }
    
    if (element == 0) {
        _data.push_back(0);
        return;
    }
    
    while (element > 0) {
        _data.push_back(element % BigInteger::BASE);
        element /= BigInteger::BASE;
    }
}


void BigInteger::_delete_leading_zeros()
{
    while (_data.size() > 1 && _data.back() == 0) {
        _data.pop_back();
    }
}

void BigInteger::_check_for_zero()
{
    if (_data.size() == 1 && _data[0] == 0) {
        _sign = POSITIVE;
    }
}

void BigInteger::_normalize()
{
    _delete_leading_zeros();
    _check_for_zero();
}

void BigInteger::_normalize(BigInteger::_SignType need_sign_)
{
    _delete_leading_zeros();
    _sign = need_sign_;
    _check_for_zero();
}

void BigInteger::_fromString (const std::string & str)
{
    if (str.size() == 0) {
        return;
    }
    _sign = (str[0] == '-' ? NEGATIVE : POSITIVE);
    
    _data.clear();
    _data.reserve(str.size());
    
    for (int i = static_cast<int>(str.size()) - 1, j = 0; i >= _sign; i--, j++) {
        _data.push_back(str[i] - '0');
    }
    
    _normalize();
}

BigInteger& BigInteger::_sum_and_sub (BigInteger & first, const BigInteger & second, BigInteger::_SignType result_sign, bool is_sum, bool is_first_greater)
{
    first._sign = result_sign;
    unsigned char carry = 0;
    
    size_t size = std::max(first._data.size(), second._data.size()) + 1;
    first._data.resize(size, 0);
    
    size_t i;
    for (i = 0; i < size; i++) {
        unsigned char first_term = (i < first._data.size() ? first._data[i] : 0);
        unsigned char second_term = (i < second._data.size() ? second._data[i] : 0);
        
        first._data[i] = (is_sum ?
                          first_term + second_term + carry :
                          (is_first_greater ? first_term - second_term - carry : second_term - first_term - carry)
                          );
        
        if (first._data[i] >= BigInteger::BASE) {
            first._data[i] += (is_sum ? -BigInteger::BASE : BigInteger::BASE);
            carry = 1;
        } else {
            carry = 0;
        }
    }
    
    if (carry && is_sum) {
        first._data[i] = carry;
    }
    
    first._normalize();
    
    return first;
}

BigInteger& BigInteger::_mult (BigInteger & first, const BigInteger & second, BigInteger::_SignType result_sign_)
{
    BigInteger result;
    size_t size = first._data.size() * second._data.size();
    result._sign = result_sign_;
    result._data.clear();
    result._data.reserve(size + 1);
    
    unsigned char carry = 0;
    
    for (size_t i = 0; i < size + 1; i++) {
        result._data.push_back(carry);
        carry = 0;
        for (size_t j = 0; j <= i; j++) {
            unsigned char first_term = (j < first._data.size() ? first._data[j] : 0);
            unsigned char second_term = (i - j < second._data.size() ? second._data[i - j] : 0);
            
            result._data[i] += first_term * second_term;
            
            if (result._data[i] >= BigInteger::BASE) {
                carry += result._data[i] / BigInteger::BASE;
                result._data[i] %= BigInteger::BASE;
            }
        }
    }
    
    result._normalize();
    
    first = result;
    
    return first;
}

BigInteger& BigInteger::_div_and_mod (BigInteger & first, const BigInteger & second, BigInteger::_SignType result_sign_, bool is_div)
{
    if (second == 0) {
        std::cerr << "Devision by zero";
    }
    
    BigInteger result = 0;
    
    BigInteger num = abs(first), denom = abs(second), degree;
    
    while (num >= denom) {
        degree = 1;
        
        while (num >= denom) {
            denom *= BigInteger::BASE;
            degree *= BigInteger::BASE;
        }
        
        if (degree > 0) {
            denom._data.erase(denom._data.begin());
            degree._data.erase(degree._data.begin());
        }
        
        while (num >= denom) {
            num -= denom;
            result += degree;
        }
        
        denom = abs(second);
    }
    
    if (is_div) {
        result._normalize(result_sign_);
        first = result;
        return first;
    } else {
        num._normalize(first._sign);
        first = num;
        return first;
    }
}

BigInteger::_CompareType BigInteger::_compare_by_abs (const BigInteger & first, const BigInteger & second)
{
    if (first._data.size() < second._data.size()) {
        return LOWER;
    } else if (first._data.size() > second._data.size()) {
        return GREATER;
    }
    
    size_t comp_size = first._data.size();
    
    for (int i = static_cast<int>(comp_size) - 1; i >= 0; i--) {
        if (first._data[i] < second._data[i]) {
            return LOWER;
        } else if (first._data[i] > second._data[i]) {
            return GREATER;
        }
    }
    
    return EQUAL;
}

BigInteger::_CompareType BigInteger::_compare (const BigInteger & first, const BigInteger & second)
{
    if (first._sign > second._sign) {
        return LOWER;
    } else if (first._sign < second._sign) {
        return GREATER;
    }
    
    return (first._sign ? BigInteger::_compare_by_abs(second, first) : BigInteger::_compare_by_abs(first, second));
}

std::string BigInteger::toString () const
{
    std::string result;
    result.reserve(_data.size() + 1);
    
    if (_sign == NEGATIVE) {
        result += '-';
    }
    
    for (int i = static_cast<int>(_data.size()) - 1; i >= 0; i--) {
        result += _data[i] + '0';
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
    number._fromString(str);
    return in;
}

BigInteger & BigInteger::operator = (const BigInteger & second)
{
    this -> _data = second._data;
    this -> _sign = second._sign;
    return *this;
}

BigInteger::operator bool()
{
    return (*this) != 0;
}

bool operator == (const BigInteger & first, const BigInteger & second)
{
    return BigInteger::_compare(first, second) == BigInteger::EQUAL;
}

bool operator != (const BigInteger & first, const BigInteger & second)
{
    return BigInteger::_compare(first, second) != BigInteger::EQUAL;
}

bool operator < (const BigInteger & first, const BigInteger & second)
{
     return BigInteger::_compare(first, second) == BigInteger::LOWER;
}

bool operator <= (const BigInteger & first, const BigInteger & second)
{
    return BigInteger::_compare(first, second) != BigInteger::GREATER;
}

bool operator > (const BigInteger & first, const BigInteger & second)
{
    return BigInteger::_compare(first, second) == BigInteger::GREATER;
}

bool operator >= (const BigInteger & first, const BigInteger & second)
{
    return BigInteger::_compare(first, second) != BigInteger::LOWER;
}

// Unary minus
BigInteger operator - (const BigInteger & number)
{
    BigInteger result = number;
    result._sign = (result._sign == BigInteger::NEGATIVE || (result._data.size() == 1 && result._data[0] == 0) ? BigInteger::POSITIVE : BigInteger::NEGATIVE);
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
    BigInteger result = first;
    return result *= second;
}

BigInteger operator / (const BigInteger & first, const BigInteger & second)
{
    BigInteger result = first;
    return result /= second;
}

BigInteger operator % (const BigInteger & first, const BigInteger & second)
{
    BigInteger result = first;
    return result %= second;
}


BigInteger & operator += (BigInteger & first, const BigInteger & second)
{
    if (first._sign == second._sign) {
        return BigInteger::_sum_and_sub(first, second, first._sign, true);
    }
    
    if (BigInteger::_compare_by_abs(first, second) != BigInteger::LOWER) {
        return BigInteger::_sum_and_sub(first, second, first._sign, false);
    } else {
        return BigInteger::_sum_and_sub(first, second, second._sign, false, false);
    }
}

BigInteger & operator -= (BigInteger & first, const BigInteger & second)
{
    if (first._sign != second._sign) {
        return BigInteger::_sum_and_sub(first, second, first._sign, true);
    }
    
    if (BigInteger::_compare_by_abs(first, second) != BigInteger::LOWER) {
        return BigInteger::_sum_and_sub(first, second, first._sign, false);
    } else {
        return BigInteger::_sum_and_sub(first, second, second._sign == 0 ? BigInteger::NEGATIVE : BigInteger::POSITIVE, false, false);
    }
}

BigInteger & operator *= (BigInteger & first, const BigInteger & second)
{
    return BigInteger::_mult(first, second, first._sign == second._sign ? BigInteger::POSITIVE : BigInteger::NEGATIVE);
}

BigInteger & operator /= (BigInteger & first, const BigInteger & second)
{
    return BigInteger::_div_and_mod(first, second, first._sign == second._sign ? BigInteger::POSITIVE : BigInteger::NEGATIVE, true);
}

BigInteger & operator %= (BigInteger & first, const BigInteger & second)
{
    return BigInteger::_div_and_mod(first, second, BigInteger::POSITIVE, false);
}

#endif /* BigInteger_h */
