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
private:
    
    unsigned char * data;
    size_t capacity;
    size_t size;
    char sign;
    // 1 if positive
    // -1 if negative
    
    static const size_t BEGIN_CAPACITY = 4;
    
    // Sum of two BigInteger elements by module
    BigInteger Sum (const BigInteger & first_element, const BigInteger & second_element)
    {
        BigInteger result(std::max(first_element.capacity, second_element.capacity) + 1);
        
        size_t size = std::max(first_element.size, second_element.size) + 1;
        
        unsigned char carry = 0;
        
        for (size_t i = 0; i < size; i++) {
            result.data[i] = first_element.data[i] + second_element.data[i] + carry;
            if ((result.data[i] == first_element.data[i] && carry) ||
                (result.data[i] < first_element.data[i])) {
                
                carry = 1;
            } else {
                carry = 0;
            }
            if (result.data[i]) {
                result.size = i + 1;
            }
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
    
    
    
};

#endif /* biginteger_h */
