#include <iostream>
#include <vector>
#include <string>

using namespace std;

class BigInteger {
public:
    BigInteger();
    BigInteger(int n);
    BigInteger(const string& s);

    explicit operator bool();

    bool operator==(BigInteger const &n) const;
    bool operator!=(BigInteger const &n) const;
    bool operator<(BigInteger const &n) const;
    bool operator>(BigInteger const &n) const;
    bool operator<=(BigInteger const &n) const;
    bool operator>=(BigInteger const &n) const;

    BigInteger& operator = (const BigInteger &n);

    friend BigInteger operator-(const BigInteger &n);
    friend BigInteger& operator++(BigInteger &n);
    friend BigInteger operator++(BigInteger &n, int);
    friend BigInteger& operator--(BigInteger &n);
    friend BigInteger operator--(BigInteger &n, int);

    friend BigInteger operator+(const BigInteger &n, const BigInteger &m);
    friend BigInteger operator-(const BigInteger &n, const BigInteger &m);

    friend BigInteger& operator+=(BigInteger &n, const BigInteger &m);
    friend BigInteger& operator-=(BigInteger &n, const BigInteger &m);

    string toString() const;

    friend ostream & operator<<(ostream &, const BigInteger &n);
    friend istream & operator>>(istream &, BigInteger &n);

    static BigInteger Abs(const BigInteger& n);

    friend class Rational;

private:

    enum Sign {
        S_positive = 1,
        S_negative = 0
    };

    vector<unsigned int> number;
    Sign sign;

    unsigned int base = 10;

private:

    void DeleteLeadingZeros();
    void CheckZeroSign();
    void Standardize();
    static BigInteger& SumAndDif(BigInteger& n1, const BigInteger& n2, BigInteger::Sign resultSign, bool sum);
};

BigInteger::BigInteger() {
    number.clear();
    number.push_back(0);
    sign = S_positive;
}

BigInteger::BigInteger(int n) {
    if (n < 0) {
        n = -n;
        sign = S_negative;
    } else {
        sign = S_positive;
    }

    if (n == 0) {
        number.push_back(0);
    }

    while (n > 0) {
        number.push_back(n % 10);
        n /= 10;
    }
}

BigInteger::BigInteger(const string& s) {
    if (s.empty()) {
        return;
    }

    number.clear();
    number.reserve(s.size());

    int n = static_cast<int>(s.size()) - 1;

    for (int i = n; i > 0; i--) {
        number.push_back(s[i] - '0');
    }

    if (s[0] == '-') {
        sign = S_negative;
    } else {
        sign = S_positive;
        number.push_back(s[0] - '0');
    }
}

//Auxiliary functions

void BigInteger::DeleteLeadingZeros() {
    while (number.size() > 1 && number.back() == 0) {
        number.pop_back();
    }
}

void BigInteger::CheckZeroSign() {
    if (number.size() == 1 && number[0] == 0) {
        sign = S_positive;
    }
}

void BigInteger::Standardize() {
    DeleteLeadingZeros();
    CheckZeroSign();
}

BigInteger BigInteger::Abs(const BigInteger &n) {
    return (n.sign == S_negative ? -n : n);
}

BigInteger& BigInteger::SumAndDif(BigInteger &n1, const BigInteger &n2, BigInteger::Sign resultSign, bool sum) {
    size_t maxSize;
    n1.sign = resultSign;

    if (n1.number.size() > n2.number.size()) {
        maxSize = n1.number.size();
    } else {
        maxSize = n2.number.size();
    }

    n1.number.resize(maxSize, 0);

    unsigned int carry = 0;
    unsigned int i;

    for (i = 0; i < maxSize; i++) {
        unsigned int first = (i < n1.number.size() ? n1.number[i] : 0);
        unsigned int second = (i < n2.number.size() ? n2.number[i] : 0);

        n1.number[i] = (sum ? first + second + carry : (Abs(n1) > Abs(n2) ? first - second - carry : second - first - carry));

        if (n1.number[i] >= 10) {
            if (sum) {
                n1.number[i] -= 10;
            } else {
                n1.number[i] += 10;
            }

            carry = 1;
        } else {
            carry = 0;
        }
    }

    if (carry && sum) {
        n1.number.push_back(carry);
    }

    n1.DeleteLeadingZeros();
    n1.CheckZeroSign();

    return n1;
}

//Streams overloading

istream& operator>>(istream& in, BigInteger& n) {
    string input;
    in >> input;
    n = BigInteger(input);
    return in;
}

ostream& operator<<(ostream& out, const BigInteger& n) {
    out << n.toString();
    return out;
}

//Comparison operators

bool BigInteger::operator<(BigInteger const &n) const {
    if (sign < n.sign) {
        return true;
    }

    if (sign > n.sign) {
        return false;
    }

    if (number.size() < n.number.size()) {
        return (sign == S_positive);
    }

    if (number.size() > n.number.size()) {
        return (sign == S_negative);
    }

    int i = static_cast<int>(number.size()) - 1;

    while (i >= 0) {
        if (number[i] < n.number[i]) {
            return (sign == S_positive);
        }

        if (number[i] > n.number[i]) {
            return (sign == S_negative);
        }

        --i;
    }

    return false;
}

bool BigInteger::operator>(BigInteger const &n) const {
    return (n < *this);
}

bool BigInteger::operator<=(BigInteger const &n) const {
    return !(*this > n);
}

bool BigInteger::operator>=(BigInteger const &n) const {
    return !(*this < n);
}

bool BigInteger::operator==(BigInteger const &n) const {
    return ((*this <= n) && (*this >= n));
}

bool BigInteger::operator!=(BigInteger const &n) const {
    return ((*this < n) || (*this > n));
}

//Operator bool

BigInteger::operator bool() {
    return (*this != 0);
}

//Assignment

BigInteger& BigInteger::operator=(const BigInteger &n) {
    if (this == &n) {
        return *this;
    }

    this->number = n.number;
    this->sign = n.sign;
    return *this;
}

//Unary minus

BigInteger operator-(const BigInteger &n) {
    BigInteger tmp = n;
    if ((n.number.size() == 1) && (n.number[0] == 0)) {
        tmp.sign = BigInteger::S_positive;
        return tmp;
    }

    tmp.sign = (n.sign == BigInteger::S_negative ? BigInteger::S_positive : BigInteger::S_negative);
    return tmp;
}

//Prefix increment and decrement

BigInteger& operator++(BigInteger &n) {
    return n += 1;
};

BigInteger& operator--(BigInteger &n) {
    return n -= 1;
};

//Postfix increment and decrement

BigInteger operator++(BigInteger &n, int) {
    BigInteger prev = n;
    n += 1;
    return prev;
};

BigInteger operator--(BigInteger &n, int) {
    BigInteger prev;
    n -= 1;
    return prev;
};

//Math

BigInteger operator+(const BigInteger &n, const BigInteger &m) {
    BigInteger sum = n;
    return sum += m;
}

BigInteger operator-(const BigInteger &n, const BigInteger &m) {
    BigInteger dif = n;
    return dif -= m;
}

//Compound assignments

BigInteger& operator+=(BigInteger &n, const BigInteger &m) {
    if (n.sign == m.sign) {
        return BigInteger::SumAndDif(n, m, n.sign, true);
    }

    if (BigInteger::Abs(n) >= BigInteger::Abs(m)) {
        return BigInteger::SumAndDif(n, m, n.sign, false);
    } else {
        return BigInteger::SumAndDif(n, m, m.sign, false);
    }
}

BigInteger& operator-=(BigInteger &n, const BigInteger &m) {

    if (n.sign != m.sign) {
        return BigInteger::SumAndDif(n, m, n.sign, true);
    }

    if (BigInteger::Abs(n) >= BigInteger::Abs(m)) {
        return BigInteger::SumAndDif(n, m, n.sign, false);
    } else {
        return BigInteger::SumAndDif(n, m, (m.sign == BigInteger::S_positive ? BigInteger::S_negative : BigInteger::S_positive), false);
    }
}

//ToString

string BigInteger::toString() const {
    string outcome;
    outcome.reserve(number.size());

    if (sign == S_negative) {
        outcome.reserve(number.size() + 1);
        outcome.push_back('-');
    }

    for (int i = static_cast<int>(number.size()) - 1; i >= 0; i--) {
        outcome.push_back(number[i] + '0');
    }

    return outcome;
}
