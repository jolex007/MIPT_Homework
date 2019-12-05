//
//  LeftistHeap.h
//  Heaps
//
//  Created by Алексей Зерцалов on 05.12.2019.
//  Copyright © 2019 Алексей Зерцалов. All rights reserved.
//

#ifndef LeftistHeap_h
#define LeftistHeap_h

#include "IHeap.h"

template <typename T = int, typename Compare = std::less<T>>
class LeftistHeap : public IHeap<T, Compare> {
    
private:
    
// Private functions and classes
    
    class _HeapNode;
    
    using _HeapNodePtr = _HeapNode*;
    
    static std::function <bool(const T&, const T&)> _Compare;
    
    static _HeapNodePtr _Merge(_HeapNodePtr, _HeapNodePtr);
    
    static void _Merge(LeftistHeap&, LeftistHeap&);
    
    static size_t _GetRank(_HeapNodePtr);
    
    static void _Update(_HeapNodePtr);
    
    static void _CopyTree(_HeapNodePtr&, const _HeapNodePtr&);
    
    static void _ClearTree(_HeapNodePtr&);
    
// Struct fields
    
    _HeapNodePtr _root; // Root of Tree
    size_t _size;       // Size of Tree
    
public:
    
// Public methods
    
    LeftistHeap();
    LeftistHeap(const T&);
    LeftistHeap(const LeftistHeap&);
    
    ~LeftistHeap() override;
    
    void Insert(const T&) override;
    
    T GetMin() const override;

    T ExtractMin() override;
    
    void Meld(IHeap<T, Compare>&) override;
    
    bool Empty() const override;
    
    size_t Size() const override;
    
    LeftistHeap& operator = (const LeftistHeap&);
};

template <typename T, typename Compare>
class LeftistHeap<T, Compare>::_HeapNode {
public:
    
// Struct fields
    
    T _key_;                // Key
    _HeapNodePtr _left_;    // Left son
    _HeapNodePtr _right_;   // Right son
    size_t _rank_;          // Rank of vertex
    
// Constructors and Destructor
    
    _HeapNode();
    
    _HeapNode(const T&);
    
    ~_HeapNode() = default;
};

//
// Constructors and Desctuctors
//

template <typename T, typename Compare>
LeftistHeap<T, Compare>::_HeapNode::_HeapNode() : _left_(nullptr), _right_(nullptr), _rank_(1) {}


template <typename T, typename Compare>
LeftistHeap<T, Compare>::_HeapNode::_HeapNode(const T& key) : _key_(key), _left_(nullptr), _right_(nullptr), _rank_(1) {}

template <typename T, typename Compare>
LeftistHeap<T, Compare>::LeftistHeap() : _root(nullptr), _size(0) {}

template <typename T, typename Compare>
LeftistHeap<T, Compare>::LeftistHeap(const T& element) : _root(new _HeapNode(element)), _size(1) {}

template <typename T, typename Compare>
LeftistHeap<T, Compare>::LeftistHeap(const LeftistHeap& rhs) : _root(nullptr), _size(0)
{
    (*this) = rhs;
}

template <typename T, typename Compare>
LeftistHeap<T, Compare>::~LeftistHeap()
{
    _ClearTree(_root);
    _size = 0;
}


//
// Private functions and methods
//

template <typename T, typename Compare>
size_t LeftistHeap<T, Compare>::_GetRank(_HeapNodePtr ptr)
{
    return ptr ? ptr->_rank_ : 0;
}

template <typename T, typename Compare>
void LeftistHeap<T, Compare>::_Update(_HeapNodePtr ptr)
{
    if (!ptr) { return; }
    
    ptr->_rank_ = std::min(_GetRank(ptr->_left_), _GetRank(ptr->_right_), _Compare) + 1;
}

template <typename T, typename Compare>
void LeftistHeap<T, Compare>::_CopyTree(_HeapNodePtr & lhs, const _HeapNodePtr & rhs)
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
void LeftistHeap<T, Compare>::_ClearTree(_HeapNodePtr & ptr)
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
std::function<bool(const T&, const T&)> LeftistHeap<T, Compare>::_Compare = Compare();


template <typename T, typename Compare>
typename LeftistHeap<T, Compare>::_HeapNodePtr LeftistHeap<T, Compare>::_Merge(_HeapNodePtr lhs, _HeapNodePtr rhs)
{
    if (!lhs) {
        return rhs;
    } else if (!rhs) {
        return lhs;
    }
    
    if (_Compare(lhs->_key_, rhs->_key_)) {
        lhs->_right_ = _Merge(lhs->_right_, rhs);
        _Update(lhs);
        
        if (_GetRank(lhs->_left_) < _GetRank(lhs->_right_)) {
            std::swap(lhs->_left_, lhs->_right_);
        }
        
        return lhs;
    } else {
        rhs->_left_ = _Merge(lhs, rhs->_left_);
        _Update(rhs);
        
        if (_GetRank(rhs->_right_) < _GetRank(rhs->_left_)) {
            std::swap(rhs->_right_, rhs->_left_);
        }
        
        return rhs;
    }
}

template <typename T, typename Compare>
void LeftistHeap<T, Compare>::_Merge(LeftistHeap& lhs, LeftistHeap& rhs)
{
    lhs._root = _Merge(lhs._root, rhs._root);
    lhs._size += rhs._size;
    rhs._root = nullptr;
}

template <typename T, typename Compare>
size_t LeftistHeap<T, Compare>::Size() const
{
    return _size;
}

template <typename T, typename Compare>
bool LeftistHeap<T, Compare>::Empty() const
{
    return _size == 0;
}


//
// Public functions and methods
//

template <typename T, typename Compare>
LeftistHeap<T, Compare>& LeftistHeap<T, Compare>::operator = (const LeftistHeap<T, Compare>& rhs)
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
T LeftistHeap<T, Compare>::GetMin() const
{
    if (!_root) {
        throw std::range_error("No elements in Heap");
    }
    return _root->_key_;
}

template <typename T, typename Compare>
T LeftistHeap<T, Compare>::ExtractMin()
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
void LeftistHeap<T, Compare>::Meld(IHeap<T, Compare>& rhs)
{
    try {
        _Merge((*this), dynamic_cast<LeftistHeap<T, Compare>&>(rhs));
    } catch (const std::bad_cast& error) {
        std::cerr << error.what();
    };
}

template <typename T, typename Compare>
void LeftistHeap<T, Compare>::Insert(const T& element)
{
    LeftistHeap<T, Compare> rhs(element);
    _Merge((*this), rhs);
}


#endif /* LeftistHeap_h */
