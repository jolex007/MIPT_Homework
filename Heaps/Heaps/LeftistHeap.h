//
//  LeftistHeap.h
//  Heaps
//
//  Created by Алексей Зерцалов on 05.12.2019.
//  Copyright © 2019 Алексей Зерцалов. All rights reserved.
//

#ifndef LeftistHeap_h
#define LeftistHeap_h

#include <iostream>
#include "IHeap.h"
#include "SkewHeap.h"

template <typename T = int, typename Compare = std::less<T> >
class LeftistHeap : public SkewHeap<T, Compare>
{

protected:

    class _HeapNode : public SkewHeap<T, Compare>::_HeapNode
    {
    public:
    // Struct fields

        size_t _rank_;

    // Constructors and Destructor

        _HeapNode();

        _HeapNode(const T&);

        ~_HeapNode() = default;
    };

    // static void _ClearTree(_HeapNode* &);

    static void _CopyTree(typename SkewHeap<T, Compare>::_HeapNode* &, const typename SkewHeap<T, Compare>::_HeapNode*);

    static size_t _GetRank(typename SkewHeap<T, Compare>::_HeapNode*);
    
    static void _Update(typename SkewHeap<T, Compare>::_HeapNode* ptr);

    static typename SkewHeap<T, Compare>::_HeapNode* _Merge(typename SkewHeap<T, Compare>::_HeapNode*, typename SkewHeap<T, Compare>::_HeapNode*);

    static void _Merge(LeftistHeap&, LeftistHeap&);

public:

    LeftistHeap();
    LeftistHeap(const T&);
    LeftistHeap(const LeftistHeap&);
    

    LeftistHeap& operator = (const LeftistHeap&);

    T ExtractMin() override;
    
    void Meld(IHeap<T, Compare>&) override;

    void Insert(const T&) override;
};

template <typename T, typename Compare>
LeftistHeap<T, Compare>::_HeapNode::_HeapNode() : SkewHeap<T, Compare>::_HeapNode(), _rank_(1) {}

template <typename T, typename Compare>
LeftistHeap<T, Compare>::_HeapNode::_HeapNode(const T& key) : SkewHeap<T, Compare>::_HeapNode(key), _rank_(1) {}

template <typename T, typename Compare>
LeftistHeap<T, Compare>::LeftistHeap() : SkewHeap<T, Compare>::SkewHeap() {}

template <typename T, typename Compare>
LeftistHeap<T, Compare>::LeftistHeap(const T& element) : SkewHeap<T, Compare>::SkewHeap(element)
{
    this->_root = dynamic_cast<typename SkewHeap<T, Compare>::_HeapNode*> (new LeftistHeap<T, Compare>::_HeapNode(element));
}

template <typename T, typename Compare>
LeftistHeap<T, Compare>::LeftistHeap(const LeftistHeap& rhs)
{
    this->_root = nullptr;
    (*this) = rhs;
}

template <typename T, typename Compare>
size_t LeftistHeap<T, Compare>::_GetRank(typename SkewHeap<T, Compare>::_HeapNode* ptr)
{
    return ptr ? dynamic_cast<typename LeftistHeap<T, Compare>::_HeapNode*> (ptr)->_rank_ : 0;
}

template <typename T, typename Compare>
void LeftistHeap<T, Compare>::_Update(typename SkewHeap<T, Compare>::_HeapNode* ptr)
{
    if (!ptr) { return; }
    
    dynamic_cast<typename LeftistHeap<T, Compare>::_HeapNode*> (ptr)->_rank_ = std::min(_GetRank(ptr->_left_),
                                                                                        _GetRank(ptr->_right_), 
                                                                                        SkewHeap<T, Compare>::_Compare) + 1;
}

template <typename T, typename Compare>
typename SkewHeap<T, Compare>::_HeapNode* LeftistHeap<T, Compare>::_Merge(typename SkewHeap<T, Compare>::_HeapNode* lhs, typename SkewHeap<T, Compare>::_HeapNode* rhs)
{
    if (!lhs) {
        return rhs;
    } else if (!rhs) {
        return lhs;
    }
    
    if (SkewHeap<T, Compare>::_Compare(lhs->_key_, rhs->_key_)) {
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
void LeftistHeap<T, Compare>::_CopyTree(typename SkewHeap<T, Compare>::_HeapNode* & lhs, const typename SkewHeap<T, Compare>::_HeapNode* rhs)
{
    if (rhs == nullptr) {
        lhs = nullptr;
        return;
    }
    
    lhs = dynamic_cast<typename SkewHeap<T, Compare>::_HeapNode*> (new LeftistHeap<T, Compare>::_HeapNode(rhs->_key_));
    
    LeftistHeap<T, Compare>::_CopyTree(lhs->_left_, rhs->_left_);
    LeftistHeap<T, Compare>::_CopyTree(lhs->_right_, rhs->_right_);
}

template <typename T, typename Compare>
LeftistHeap<T, Compare>& LeftistHeap<T, Compare>::operator = (const LeftistHeap<T, Compare>& rhs)
{
    if (this == &rhs) {
        return (*this);
    }
    
    this->_ClearTree(this->_root);
    this->_size = rhs.Size();
    LeftistHeap<T, Compare>::_CopyTree(this->_root, rhs._root);
    return (*this);
}

template <typename T, typename Compare>
void LeftistHeap<T, Compare>::_Merge(LeftistHeap& lhs, LeftistHeap& rhs)
{
    lhs._root = _Merge(lhs._root, rhs._root);
    lhs._size += rhs._size;
    rhs._root = nullptr;
}

template <typename T, typename Compare>
T LeftistHeap<T, Compare>::ExtractMin()
{
    if (!this->_root) {
        throw std::range_error("No elements in Heap");
    }
    typename SkewHeap<T, Compare>::_HeapNode* result = this->_root;
    
    this->_root = _Merge(this->_root->_left_, this->_root->_right_);
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
