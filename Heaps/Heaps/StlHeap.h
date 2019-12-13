//
//  StlHeap.h
//  Heaps
//
//  Created by Алексей Зерцалов on 06.12.2019.
//  Copyright © 2019 Алексей Зерцалов. All rights reserved.
//

#ifndef StlHeap_h
#define StlHeap_h

#include <set>

template <typename T = int, typename Compare = std::less<T>>
class StlHeap : public IHeap<T, Compare>
{
    
private:
    
    std::multiset<T, Compare> _heap;
    
    
public:
    
    StlHeap() = default;
    StlHeap(const T&);
    StlHeap(const StlHeap&);
    
    ~StlHeap() override = default;
    
    void Insert(const T&) override;
    
    T GetMin() const override;

    T ExtractMin() override;
    
    void Meld(IHeap<T, Compare>&) override;
    
    bool Empty() const override; // TODO: Empty
    
    size_t Size() const override; // TODO: Size
    
    StlHeap& operator = (const StlHeap&);
};



template <typename T, typename Compare>
StlHeap<T, Compare>::StlHeap(const T& key) : _heap({key}) {}

template <typename T, typename Compare>
StlHeap<T, Compare>::StlHeap(const StlHeap& rhs) : _heap(rhs._heap) {}


template <typename T, typename Compare>
void StlHeap<T, Compare>::Insert(const T& key)
{
    _heap.insert(key);
}

template <typename T, typename Compare>
T StlHeap<T, Compare>::GetMin() const
{
    if (_heap.empty()) {
        throw std::range_error("No elements in Heap");
    }
    return (*_heap.begin());
}

template <typename T, typename Compare>
T StlHeap<T, Compare>::ExtractMin()
{
    if (_heap.empty()) {
        throw std::range_error("No elements in Heap");
    }
    T result = (*_heap.begin());
    _heap.erase(_heap.begin());
    return result;
}

template <typename T, typename Compare>
void StlHeap<T, Compare>::Meld(IHeap<T, Compare> & rhs)
{
    _heap.merge(dynamic_cast<StlHeap<T, Compare>&>(rhs)._heap);
}

template <typename T, typename Compare>
bool StlHeap<T, Compare>::Empty() const
{
    return _heap.empty();
}

template <typename T, typename Compare>
size_t StlHeap<T, Compare>::Size() const
{
    return _heap.size();
}


template <typename T, typename Compare>
StlHeap<T, Compare>& StlHeap<T, Compare>::operator=(const StlHeap<T, Compare> & rhs)
{
    _heap = rhs._heap;
    return (*this);
}



#endif /* StlHeap_h */
