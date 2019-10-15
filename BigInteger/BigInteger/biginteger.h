//
//  biginteger.h
//  BigInteger
//
//  Created by Алексей Зерцалов on 09/10/2019.
//  Copyright © 2019 Алексей Зерцалов. All rights reserved.
//

#include <iostream>

#ifndef biginteger_h
#define biginteger_h

/*
 
 Идея -
 Как складывать два пофигу каких числа
 Если оно отрицательное или вычитаем, то переводим в обратное и складываем
 
 */

class BigInteger
{
    friend int main();
    friend void solve();
    
    
    friend const BigInteger operator + (const BigInteger &, const BigInteger &);
    friend const BigInteger operator - (const BigInteger &, const BigInteger &);
    
private:
    
    unsigned char * data;
    size_t capacity;
    size_t size;
    char sign;
    // 0 if positive
    // 1 if negative
    
    static const size_t BEGIN_CAPACITY = 4;
    
    
    // Resize the capacity
    void resize (size_t new_capacity)
    {
        capacity = new_capacity;
        unsigned char * new_data = new unsigned char [capacity];
        memcpy(new_data, data, size);
        memset(new_data + size, 0, capacity - size);
        data = new_data;
    }
    
    
    // Adds one to number by module
    inline void plusOneByModule () const
    {
        for (size_t index = 0; index < size + 1; index++) {
            data[index]++;
            if (data[index] != 0) {
                break;
            }
        }
    }
    
    
    // Translating number to writeback record
    void writebackTranslation (size_t need_size) const
    {
        for (size_t indx = 0; indx < need_size; indx++) {
            data[indx] = (~data[indx]);
        }
        plusOneByModule();
    }
    
    
    
    static BigInteger SumOfTwoBigInteger(const BigInteger & first, const BigInteger & second, size_t sum_size, bool is_sub = false)
    {
        BigInteger result(std::max(first.capacity, second.capacity) + 1);
        
        if (first.sign) {
            first.writebackTranslation(sum_size);
        }
        if ( (static_cast<unsigned char> (second.sign) + static_cast<unsigned char>(is_sub)) % 2 != 0 ) {
            second.writebackTranslation(sum_size);
        }
        
        unsigned char carry = 0;
        
        for (size_t index = 0; index < sum_size; index++) {
            unsigned char first_term = (index < first.capacity ? first.data[index] : 0);
            unsigned char second_term = (index < second.capacity ? second.data[index] : 0);
            
            result.data[index] = first_term + second_term + carry;
            if ((result.data[index] == first_term && carry) ||
                (result.data[index] < first_term)) {
                
                carry = 1;
            } else {
                carry = 0;
            }
                
            if (result.data[index] != 0) {
                result.size = index;
            }
        }
        
        result.size++;
        
        if (result.data[sum_size - 1] > 1) {
            result.writebackTranslation(sum_size);
            result.sign = 1;
        } else {
            result.sign = 0;
        }
        
        if (first.sign) {
            first.writebackTranslation(sum_size);
        }
        if ( (static_cast<unsigned char> (second.sign) + static_cast<unsigned char>(is_sub)) % 2 != 0 ) {
            second.writebackTranslation(sum_size);
        }
        
        return result;
    }
    
    
    
public:

    BigInteger ()
    {
        capacity = BEGIN_CAPACITY;
        data = new unsigned char[capacity];
        memset(data, 0, capacity * sizeof(unsigned char));
        size = 0;
        sign = 1;
    }
    
    BigInteger (size_t start_capacity)
    {
        capacity = start_capacity;
        data = new unsigned char[capacity];
        memset(data, 0, capacity * sizeof(unsigned char));
        size = 0;
        sign = 1;
    }
    
    BigInteger (int element)
    {
        capacity = 8;
        data = new unsigned char[capacity];
        memset(data, 0, capacity * sizeof(unsigned char));
        sign = (element < 0 ? 1 : 0);
        element *= (element < 0 ? -1 : 1);
        for (int i = 0; i < 4; i++) {
            data[i] = element % 256;
            element /= 256;
            if (data[i] != 0) {
                size = i + 1;
            }
        }
    }
    
};

const BigInteger operator + (const BigInteger & first, const BigInteger & second)
{
    size_t sum_size = std::max(first.size, second.size) + 1;
    
    return BigInteger :: SumOfTwoBigInteger(first, second, sum_size);
}

const BigInteger operator - (const BigInteger & first, const BigInteger & second)
{
    size_t sum_size = std::max(first.size, second.size) + 1;
    
    return BigInteger :: SumOfTwoBigInteger(first, second, sum_size, true);
}

#endif /* biginteger_h */
