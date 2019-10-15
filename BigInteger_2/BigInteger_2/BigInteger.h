//
//  BigInteger.h
//  BigInteger_2
//
//  Created by Алексей Зерцалов on 14/10/2019.
//  Copyright © 2019 Алексей Зерцалов. All rights reserved.
//
#include <iostream>
#include <string>

#ifndef BigInteger_h
#define BigInteger_h

struct BigInteger
{
    friend int main();
    
    friend std::ostream& operator<< (std::ostream &, const BigInteger &);
    friend std::istream& operator>> (std::istream &, BigInteger &);
    
private:
    unsigned char * data;
    unsigned char sign;
    // 0 - if positive
    // 1 - if negative
    size_t size;
    size_t capacity;
    
    void fromString (const std::string & str)
    {
        if (str.size() == 0) {
            return;
        }
        sign = (str[0] == '-' ? 1 : 0);
        free(data);
        
        size = 1;
        capacity = (str.size() - sign) * 2;
        data = (unsigned char *) malloc(capacity * sizeof(unsigned char));
        memset(data, 0, capacity);
        
        for (int i = static_cast<int>(str.size()) - 1, j = 0; i >= sign; i--, j++) {
            data[j] = str[i] - '0';
            if (data[j] != 0) {
                size = j + 1;
            }
        }
    }
    
    
    BigInteger sum (const BigInteger & first, const BigInteger & second)
    {
        BigInteger result;
        
        
        return result;
    }
    
public:
    
    
    std::string toString () const
    {
        std::string result;
        result.reserve(size + 1);
        
        if (sign == 1) {
            result += '-';
        }
        
        for (int i = static_cast<int>(size) - 1; i >= 0; i--) {
            result += data[i] + '0';
        }
        
        return result;
    }
    
    BigInteger()
    {
        capacity = 4;
        size = 1;
        data = (unsigned char *) malloc(capacity * sizeof(unsigned char));
        memset(data, 0, capacity);
        sign = true;
    }
    
    BigInteger(int element)
    {
        capacity = 8;
        data = (unsigned char *) malloc(capacity * sizeof(unsigned char));
        memset(data, 0, capacity);
        sign = (element < 0 ? 1 : 0);
        if (element == 0) {
            size = 1;
        } else {
            for (; element > 0; size++) {
                data[size] = element % 10;
                element /= 10;
            }
        }
    }
    
    
    
};

std::ostream& operator<< (std::ostream &out, const BigInteger & number)
{
    out << number.toString();
    return out;
}

std::istream& operator>> (std::istream &in, BigInteger & number)
{
    std::string str;
    in >> str;
    number.fromString(str);
    return in;
}


#endif /* BigInteger_h */
