//
//  SkewHeap.h
//  Heaps
//
//  Created by Алексей Зерцалов on 06.12.2019.
//  Copyright © 2019 Алексей Зерцалов. All rights reserved.
//

#ifndef SkewHeap_h
#define SkewHeap_h

#include "IHeap.h"

template <typename T = int, typename Compare = std::less<T>>
class SkewHeap : public IHeap<T, Compare> {
    
private:
    
// Private functions and classes
    
    class _HeapNode;
    
    using _HeapNodePtr = _HeapNode*;
    
    static std::function <bool(const T&, const T&)> _Compare;
    
    static _HeapNodePtr _Merge(_HeapNodePtr, _HeapNodePtr);
    
    static void _Merge(SkewHeap&, SkewHeap&);
    
    static void _CopyTree(_HeapNodePtr&, const _HeapNodePtr&);
    
    static void _ClearTree(_HeapNodePtr&);
    
// Struct fields
    
    _HeapNodePtr _root; // Root of Tree
    size_t _size;       // Size of Tree
    
public:
    
// Public methods
    
    SkewHeap();
    SkewHeap(const T&);
    SkewHeap(const SkewHeap&);
    
    ~SkewHeap() override;
    
    void Insert(const T&) override;
    
    T GetMin() const override;

    T ExtractMin() override;
    
    void Meld(IHeap<T, Compare>&) override;
    
    bool Empty() const override;
    
    size_t Size() const override;
    
    SkewHeap& operator = (const SkewHeap&);
};

template <typename T, typename Compare>
class SkewHeap<T, Compare>::_HeapNode {
public:
    
// Struct fields
    
    T _key_;                // Key
    _HeapNodePtr _left_;    // Left son
    _HeapNodePtr _right_;   // Right son
    
// Constructors and Destructor
    
    _HeapNode();
    
    _HeapNode(const T&);
    
    ~_HeapNode() = default;
};

//
// Constructors and Desctuctors
//

template <typename T, typename Compare>
SkewHeap<T, Compare>::_HeapNode::_HeapNode() : _left_(nullptr), _right_(nullptr) {}


template <typename T, typename Compare>
SkewHeap<T, Compare>::_HeapNode::_HeapNode(const T& key) : _key_(key), _left_(nullptr), _right_(nullptr) {}

template <typename T, typename Compare>
SkewHeap<T, Compare>::SkewHeap() : _root(nullptr), _size(0) {}

template <typename T, typename Compare>
SkewHeap<T, Compare>::SkewHeap(const T& element) : _root(new _HeapNode(element)), _size(1) {}

template <typename T, typename Compare>
SkewHeap<T, Compare>::SkewHeap(const SkewHeap& rhs) : _root(nullptr), _size(rhs.Size())
{
    (*this) = rhs;
}

template <typename T, typename Compare>
SkewHeap<T, Compare>::~SkewHeap()
{
    _ClearTree(_root);
    _size = 0;
}


//
// Private functions and methods
//


template <typename T, typename Compare>
void SkewHeap<T, Compare>::_CopyTree(_HeapNodePtr & lhs, const _HeapNodePtr & rhs)
{
    if (rhs == nullptr) {
        lhs = nullptr;
        return;
    }
    
    lhs = new _HeapNode(rhs->_key_);
    
    _CopyTree(lhs->_left_, rhs->_left_);
    _CopyTree(lhs->_right_, rhs->_right_);
}

template <typename T, typename Compare>
void SkewHeap<T, Compare>::_ClearTree(_HeapNodePtr & ptr)
{
    if (ptr == nullptr) {
        return;
    }
    
    _ClearTree(ptr->_left_);
    _ClearTree(ptr->_right_);
    
    delete ptr;
    
    ptr = nullptr;
}

template <typename T, typename Compare>
std::function<bool(const T&, const T&)> SkewHeap<T, Compare>::_Compare = Compare();


template <typename T, typename Compare>
typename SkewHeap<T, Compare>::_HeapNodePtr SkewHeap<T, Compare>::_Merge(_HeapNodePtr lhs, _HeapNodePtr rhs)
{
    if (!lhs) {
        return rhs;
    } else if (!rhs) {
        return lhs;
    }
    
    if (_Compare(lhs->_key_, rhs->_key_)) {
        lhs->_right_ = _Merge(lhs->_right_, rhs);
        
        std::swap(lhs->_left_, lhs->_right_);
        
        return lhs;
    } else {
        rhs->_left_ = _Merge(lhs, rhs->_left_);
        
        std::swap(rhs->_right_, rhs->_left_);
        
        return rhs;
    }
}

template <typename T, typename Compare>
void SkewHeap<T, Compare>::_Merge(SkewHeap& lhs, SkewHeap& rhs)
{
    lhs._root = _Merge(lhs._root, rhs._root);
    lhs._size += rhs._size;
    rhs._root = nullptr;
}

template <typename T, typename Compare>
size_t SkewHeap<T, Compare>::Size() const
{
    return _size;
}

template <typename T, typename Compare>
bool SkewHeap<T, Compare>::Empty() const
{
    return _size == 0;
}


//
// Public functions and methods
//

template <typename T, typename Compare>
SkewHeap<T, Compare>& SkewHeap<T, Compare>::operator = (const SkewHeap<T, Compare>& rhs)
{
    if (this == &rhs) {
        return (*this);
    }
    
    _ClearTree(_root);
    _size = rhs.Size();
    _CopyTree(_root, rhs._root);
    
    return (*this);
}

template <typename T, typename Compare>
T SkewHeap<T, Compare>::GetMin() const
{
    if (!_root) {
        throw std::range_error("No elements in Heap");
    }
    return _root->_key_;
}

template <typename T, typename Compare>
T SkewHeap<T, Compare>::ExtractMin()
{
    if (!_root) {
        throw std::range_error("No elements in Heap");
    }
    _HeapNodePtr result = _root;
    
    _root = _Merge(_root->_left_, _root->_right_);
    T answer = result->_key_;
    
    delete result;
    
    return answer;
}

template <typename T, typename Compare>
void SkewHeap<T, Compare>::Meld(IHeap<T, Compare>& rhs)
{
    try {
        _Merge((*this), dynamic_cast<SkewHeap<T, Compare>&>(rhs));
    } catch (const std::bad_cast& error) {
        std::cerr << error.what();
    };
}

template <typename T, typename Compare>
void SkewHeap<T, Compare>::Insert(const T& element)
{
    SkewHeap<T, Compare> rhs(element);
    _Merge((*this), rhs);
}

#endif /* SkewHeap_h */
