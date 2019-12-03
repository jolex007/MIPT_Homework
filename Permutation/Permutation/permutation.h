#ifndef permutation_h
#define permutation_h

#include <iostream> // std::cerr
#include <cstring>  // memcpy

class Permutation {
public:

// Constuctors and Destructors
    Permutation();
    explicit Permutation(unsigned int);
    Permutation(unsigned int, int *);
    Permutation(const Permutation &);
    
    ~Permutation();
    
// Public functions
    Permutation next();
    Permutation previous();
    Permutation inverse();
    
// Operators overloading
    friend Permutation & operator *= (Permutation &, const Permutation &);
    friend Permutation operator * (const Permutation &, const Permutation &);
    Permutation & operator = (const Permutation &);
    
    friend Permutation & operator ++ (Permutation &);
    friend Permutation operator ++ (Permutation &, int);
    friend Permutation & operator -- (Permutation &);
    friend Permutation operator -- (Permutation &, int);
    
    friend bool operator > (const Permutation &, const Permutation &);
    friend bool operator >= (const Permutation &, const Permutation &);
    friend bool operator < (const Permutation &, const Permutation &);
    friend bool operator <= (const Permutation &, const Permutation &);
    friend bool operator == (const Permutation &, const Permutation &);
    friend bool operator != (const Permutation &, const Permutation &);
    
    const  int& operator[] (const  int);
    void operator() (int *);
    
private:

// Structure fields
    size_t _size;
    int * _data;
    
// Private functions and enum types
    enum CompareType { LOWER, EQUAL, GREATER };
    enum ActionType { NEXT, PREVIOUS };
    
    static void Reverse(Permutation &, int, int);
    static Permutation & NextAndPreviousPermutation(Permutation &, ActionType);
    static CompareType Compare(const Permutation &, const Permutation &);
};

Permutation::Permutation() : _size(0), _data(nullptr) {}

Permutation::Permutation(unsigned int n) : _size(n)
{
    _data = new int[n];
    for (int i = 0; i < static_cast<int>(n); i++) {
        _data[i] = i;
    }
}

Permutation::Permutation(unsigned int n, int * array) : _size(n)
{
    _data = new int[n];
    memcpy(_data, array, n * sizeof(int));
}

Permutation::Permutation(const Permutation & perm) : _size(perm._size)
{
    _data = new int[_size];
    memcpy(_data, perm._data, _size * sizeof(int));
}

Permutation::~Permutation()
{
    delete[] _data;
}

void Permutation::Reverse(Permutation & perm, int begin, int end)
{
    for (; begin < end; begin++, end--) {
        std::swap(perm._data[begin], perm._data[end]);
    }
}

Permutation & Permutation::NextAndPreviousPermutation(Permutation & perm, ActionType TypeOfAction)
{
    for (int i = static_cast<int>(perm._size) - 2; i >= 0; i--) {
        if (TypeOfAction == NEXT ? perm._data[i] < perm._data[i + 1] :
                                   perm._data[i] > perm._data[i + 1]) {
            
            unsigned int minimum = i + 1;
            
            for (int j = i + 1; j < static_cast<int>(perm._size); j++) {
                if (TypeOfAction == NEXT ? perm._data[j] < perm._data[minimum] && perm._data[j] > perm._data[i] :
                                           perm._data[j] > perm._data[minimum] && perm._data[j] < perm._data[i]) {
                    minimum = j;
                }
            }
            std::swap(perm._data[i], perm._data[minimum]);
            
            Reverse(perm, i + 1, static_cast<int>(perm._size) - 1);
            
            break;
        }
    }
    
    return perm;
}

Permutation & Permutation::operator = (const Permutation & rhs)
{
    if (this == &rhs) {
        return (*this);
    }
    
    delete[] _data;
        
    _size = rhs._size;
    _data = new int[_size];
    memcpy(_data, rhs._data, _size * sizeof(int));
    
    return (*this);
}

Permutation & operator *= (Permutation & lhs, const Permutation & rhs)
{
    if (lhs._size != rhs._size) {
        std::cerr << "Different Permutation _sizes";
        return lhs;
    }
    
    int * buffer = new int[lhs._size];
    memcpy(buffer, lhs._data, lhs._size * sizeof(int));
    
    for (size_t i = 0; i < lhs._size; i++) {
        lhs._data[i] = buffer[rhs._data[i]];
    }
    
    delete[] buffer;
    
    return lhs;
}

Permutation operator * (const Permutation & lhs, const Permutation & rhs)
{
    Permutation result(lhs);
    result *= rhs;
    return result;
}

Permutation Permutation::next()
{
    Permutation nextPermutation(*this);
    return NextAndPreviousPermutation(nextPermutation, NEXT);
}

Permutation Permutation::previous()
{
    Permutation previousPermutation(*this);
    return NextAndPreviousPermutation(previousPermutation, PREVIOUS);
}

Permutation Permutation::inverse()
{
    Permutation inversePermutattion(static_cast<unsigned int>(_size));
    
    for (size_t i = 0; i < _size; i++) {
        inversePermutattion._data[_data[i]] = static_cast<int>(i);
    }
    
    return inversePermutattion;
}

Permutation::CompareType Permutation::Compare(const Permutation &lhs, const Permutation &rhs)
{
    if (lhs._size != rhs._size) {
        std::cerr << "Different Permutation _sizes";
        return EQUAL;
    }
    
    for (size_t i = 0; i < lhs._size; i++) {
        if (lhs._data[i] != rhs._data[i]) {
            return lhs._data[i] < rhs._data[i] ? LOWER : GREATER;
        }
    }
    
    return EQUAL;
}

// Perfix increment
Permutation & operator ++ (Permutation & rhs)
{
    return Permutation::NextAndPreviousPermutation(rhs, Permutation::NEXT);
}

// Postfix increment
Permutation operator ++ (Permutation & lhs, int)
{
    Permutation old_value(lhs);
    Permutation::NextAndPreviousPermutation(lhs, Permutation::NEXT);
    return old_value;
}

// Perfix decrement
Permutation & operator -- (Permutation & rhs)
{
    return Permutation::NextAndPreviousPermutation(rhs, Permutation::PREVIOUS);
}

// Postfix decrement
Permutation operator -- (Permutation & lhs, int)
{
    Permutation old_value(lhs);
    Permutation::NextAndPreviousPermutation(lhs, Permutation::PREVIOUS);
    return old_value;
}

bool operator > (const Permutation & lhs, const Permutation & rhs)
{
    return Permutation::Compare(lhs, rhs) == Permutation::GREATER;
}

bool operator >= (const Permutation & lhs, const Permutation & rhs)
{
    return Permutation::Compare(lhs, rhs) != Permutation::LOWER;
}

bool operator < (const Permutation & lhs, const Permutation & rhs)
{
    return Permutation::Compare(lhs, rhs) == Permutation::LOWER;
}

bool operator <= (const Permutation & lhs, const Permutation & rhs)
{
    return Permutation::Compare(lhs, rhs) != Permutation::GREATER;
}

bool operator == (const Permutation & lhs, const Permutation & rhs)
{
    return Permutation::Compare(lhs, rhs) == Permutation::EQUAL;
}

bool operator != (const Permutation & lhs, const Permutation & rhs)
{
    return Permutation::Compare(lhs, rhs) != Permutation::EQUAL;
}

const  int& Permutation::operator[] (const  int index)
{
    return _data[index];
}

void Permutation::operator() (int * array) {
    int * buffer = new int[_size];
    
    memcpy(buffer, array, _size * sizeof(int));
    
    for (size_t i = 0; i < _size; i++) {
        array[_data[i]] = buffer[i];
    }
    
    delete[] buffer;
}


#endif /* permutation_h */
