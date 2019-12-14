//
//  Header.h
//  Heaps
//
//  Created by Алексей Зерцалов on 05.12.2019.
//  Copyright © 2019 Алексей Зерцалов. All rights reserved.
//

#ifndef Header_h
#define Header_h

#include <iostream>
#include <algorithm>

template <typename T = int, typename Compare = std::less<T>>
class IHeap {
    
public:
    
    IHeap() = default;
    
    virtual ~IHeap() = default;
    
    virtual void Insert(const T&) = 0;
    
    virtual T GetMin() const = 0;
    
    virtual T ExtractMin() = 0;
    
    virtual void Meld(IHeap&) = 0;
    
    virtual bool Empty() const = 0;
    
    virtual size_t Size() const = 0;
};

#endif /* Header_h */
