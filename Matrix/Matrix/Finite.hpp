//
//  Finite.hpp
//  Matrix
//
//  Created by Алексей Зерцалов on 27.02.2020.
//  Copyright © 2020 Алексей Зерцалов. All rights reserved.
//

#ifndef Finite_hpp
#define Finite_hpp

#include <iostream>
#include <vector>


// Prime checkers
template <unsigned N, unsigned DIV>
struct RecursivePrimeCheck
{
    static const bool value = (N % DIV != 0) && RecursivePrimeCheck<N, DIV - 1>::value;
};

template <unsigned N>
struct RecursivePrimeCheck<N, 1>
{
    static const bool value = true;
};

template <unsigned N>
struct RecursivePrimeCheck<N, 0>
{
    static const bool value = true;
};

template <unsigned N>
struct PrimeNumberCheck
{
    static const bool is_prime = RecursivePrimeCheck<N, N - 1>::value;
};

template <unsigned N>
class Finite
{
private:
    
    using _type_ = Finite<N>;
    unsigned _value_;
    
    
    // This is for checking is N a prime number
    template <bool IsPrime = PrimeNumberCheck<N>::is_prime>
    struct _NonPrimeCrash_ {
        static void check() {};
    };
    
    template <>
    struct _NonPrimeCrash_<false> {
        static void check();
    };
    
    // This function will crash programm on compilation if N is not prime
    static void _check_for_prime_() { _NonPrimeCrash_<>::check(); }
    
    template <unsigned _N_>
    friend std::ostream& operator<< (std::ostream&, Finite<_N_> const&);
    
    template <unsigned _N_>
    friend std::istream& operator>> (std::istream&, Finite<_N_>&);
    
    
public:
    
    // Constructors
    Finite();
    Finite(unsigned);
    Finite(int);
    Finite(const Finite<N>&);
    
    // Operators overloading
    
    _type_ operator-() const;
    
    _type_ operator+ (const _type_&) const;
    _type_& operator+= (const _type_&);
    _type_ operator- (const _type_&) const;
    _type_& operator-= (const _type_&);
    _type_ operator* (const _type_&) const;
    _type_& operator*= (const _type_&);
    _type_ operator^ (unsigned) const;
    _type_& operator^= (unsigned);
    
    bool operator!= (const _type_&) const;
    
    
    // Only if N is prime
    _type_ Inverse () const;
    _type_ operator/ (const _type_&) const;
    _type_& operator/= (const _type_&);
    
    unsigned get_value() const;
};

template <unsigned N>
Finite<N>::Finite () : _value_(0) {}

template <unsigned N>
Finite<N>::Finite (unsigned value) : _value_(value % N) {}

template <unsigned N>
Finite<N>::Finite (int value) : _value_((value % N == 0) ? 0 : ((value % N) + N)) {}

template <unsigned N>
Finite<N>::Finite (const Finite<N>& finite) : _value_(finite._value_) {}




template <unsigned N>
Finite<N> Finite<N>::operator- () const
{
    return Finite<N>(N - _value_);
}

template <unsigned N>
Finite<N> Finite<N>::operator+ (const _type_& rhs) const
{
    return Finite<N>((_value_ + rhs._value_) % N);
}

template <unsigned N>
Finite<N>& Finite<N>::operator+= (const _type_& rhs)
{
    _value_ = (_value_ + rhs._value_) % N;
    return (*this);
}

template <unsigned N>
Finite<N> Finite<N>::operator- (const _type_& rhs) const
{
    return Finite<N>((_value_ + N - rhs._value_) % N);
}

template <unsigned N>
Finite<N>& Finite<N>::operator-= (const _type_& rhs)
{
    _value_ = (_value_ + N - rhs._value_) % N;
    return (*this);
}

template <unsigned N>
Finite<N> Finite<N>::operator* (const _type_& rhs) const
{
    return Finite<N>((_value_ * rhs._value_) % N);
}

template <unsigned N>
Finite<N>& Finite<N>::operator*= (const _type_& rhs)
{
    _value_ = (_value_ * rhs._value_) % N;
    return (*this);
}

template <unsigned N>
Finite<N> Finite<N>::operator^ (unsigned pow) const
{
    if (pow == 0) { return Finite<N>(1); }
    if (pow == 1) { return (*this); }
    if (pow % 2 == 0) {
        Finite<N> result = (*this) ^ (pow / 2);
        result *= result;
        return result;
    } else {
        Finite<N> result = (*this) ^ (pow - 1);
        result *= (*this);
        return result;
    }
}

template <unsigned N>
Finite<N>& Finite<N>::operator^= (unsigned pow)
{
    return (*this) = (*this) ^ pow;
}

template <unsigned N>
bool Finite<N>::operator!= (const _type_& rhs) const
{
    return _value_ != rhs._value_;
}


template <unsigned N>
Finite<N> Finite<N>::Inverse() const
{
    _check_for_prime_();
    return (*this) ^ (N - 2);
}

template <unsigned N>
Finite<N> Finite<N>::operator/ (const _type_& rhs) const
{
    _check_for_prime_();
    return ((rhs) ^ (N - 2)) * (*this);
}

template <unsigned N>
Finite<N>& Finite<N>::operator/= (const _type_& rhs)
{
    _check_for_prime_();
    return (*this) = ((rhs) ^ (N - 2)) * (*this);
}

template <unsigned N>
unsigned Finite<N>::get_value() const
{
    return _value_;
}

template <unsigned N>
std::ostream& operator<< (std::ostream& out, Finite<N> const& elem)
{
    out << elem.get_value();
    return out;
}

template <unsigned N>
std::istream& operator>> (std::istream& in, Finite<N>& elem)
{
    in >> elem._value_;
    elem._value_ %= N;
    return in;
}




#endif /* Finite_hpp */
