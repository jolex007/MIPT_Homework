//
//  BinomialHeap.h
//  Heaps
//
//  Created by Алексей Зерцалов on 05.12.2019.
//  Copyright © 2019 Алексей Зерцалов. All rights reserved.
//

#ifndef BinomialHeap_h
#define BinomialHeap_h

#include "IHeap.h"

template <typename T = int, typename Compare = std::less<T>>
class BinomialHeap : public IHeap<T, Compare> {
    
private:
// Private functions and classes
        
    class _HeapNode;
    
    using _HeapNodePtr = _HeapNode*;
    
    static void _ClearNodes(_HeapNodePtr&);
    
    template <typename T2>
    static T2 _FindMin(T2);
    
    static std::function<bool(const T&, const T&)> _Compare;
    
    static void _Merge(BinomialHeap<T, Compare>&, BinomialHeap<T, Compare>&);

    static void _Copy(_HeapNodePtr&, const _HeapNodePtr&);
    
// Struct fields
    
    _HeapNodePtr _root; // Tree root
    size_t _size;       // Size of tree
    
public:
// Public methods
    
    BinomialHeap();
    BinomialHeap(const T&);
    BinomialHeap(const BinomialHeap&);
    
    ~BinomialHeap() override;
    
    void Insert(const T&) override;
    
    T GetMin() const override;
    
    T ExtractMin() override;
    
    void Meld(IHeap<T, Compare>&) override;
    
    bool Empty() const override;
    
    size_t Size() const override;
    
    BinomialHeap& operator = (const BinomialHeap&);
};

template <typename T, typename Compare>
class BinomialHeap<T, Compare>::_HeapNode {
public:
// Class Fields
    
    T _key_;                // Key
    _HeapNodePtr _child_;   // Left Child
    _HeapNodePtr _sibling_; // Brother
    size_t _degree_;        // Degree of this vertex
    
// Constructors and Destructor
    
    _HeapNode();
    
    _HeapNode(const T&);
    
    ~_HeapNode() = default;
};

template <typename T, typename Compare>
std::function<bool(const T&, const T&)> BinomialHeap<T, Compare>::_Compare = Compare();

template <typename T, typename Compare>
void BinomialHeap<T, Compare>::_ClearNodes(_HeapNodePtr& vertex)
{
    if (!vertex) {
        return;
    }
    
    _ClearNodes(vertex->_child_);
    _ClearNodes(vertex->_sibling_);
    
    delete vertex;
    
    vertex = nullptr;
}

template <typename T, typename Compare>
template <typename T2>
T2 BinomialHeap<T, Compare>::_FindMin(T2 ptr)
{
    if (!ptr->_sibling_) {
        return ptr;
    }
    T2 minimum = _FindMin<T2>(ptr->_sibling_);
    if (_Compare(ptr->_key_, minimum->_key_)) {
        return ptr;
    } else {
        return minimum;
    }
}

template <typename T, typename Compare>
void BinomialHeap<T, Compare>::_Merge(BinomialHeap<T, Compare>& lhs, BinomialHeap<T, Compare>& rhs)
{
    if (lhs.Empty()) {
        lhs._root = rhs._root;
        lhs._size = rhs._size;
        rhs._root = nullptr;
        return;
    } else if (rhs.Empty()) {
        return;
    }

    _HeapNodePtr result = nullptr;
    _HeapNodePtr * ptr = &result;
    _HeapNodePtr l_ptr = lhs._root;
    _HeapNodePtr r_ptr = rhs._root;
    
    
    while (l_ptr || r_ptr) {
        if (l_ptr && (!r_ptr || l_ptr->_degree_ < r_ptr->_degree_)) {
            (*ptr) = l_ptr;
            l_ptr = l_ptr->_sibling_;
            ptr = &(*ptr)->_sibling_;
        } else {
            (*ptr) = r_ptr;
            r_ptr = r_ptr->_sibling_;
            ptr = &(*ptr)->_sibling_;
        }
    }
    
    (*ptr) = nullptr;
    
    ptr = &result;
    
    while ((*ptr)->_sibling_) {
        if ((*ptr)->_degree_ != (*ptr)->_sibling_->_degree_ ||
            ((*ptr)->_sibling_->_sibling_ && (*ptr)->_sibling_->_degree_ == (*ptr)->_sibling_->_sibling_->_degree_)) {
            ptr = &(*ptr)->_sibling_;
            continue;
        }
        if (_Compare((*ptr)->_key_, (*ptr)->_sibling_->_key_)) {
            _HeapNodePtr temp = (*ptr)->_sibling_;
            (*ptr)->_sibling_ = temp->_sibling_;
            temp->_sibling_ = (*ptr)->_child_;
            (*ptr)->_child_ = temp;
            (*ptr)->_degree_++;
        } else {
            _HeapNodePtr temp = (*ptr)->_sibling_;
            (*ptr)->_sibling_ = temp->_child_;
            temp->_child_ = (*ptr);
            temp->_degree_++;
            (*ptr) = temp;
        }
    }
    
    lhs._root = result;
    lhs._size += rhs._size;
    
    rhs._root = nullptr;
}

template <typename T, typename Compare>
void BinomialHeap<T, Compare>::_Copy(_HeapNodePtr& lhs, const _HeapNodePtr& rhs)
{
    if (rhs == nullptr) {
        lhs = nullptr;
        return;
    }
    
    lhs = new _HeapNode(rhs->_key_);
    lhs->_degree_ = rhs->_degree_;
    
    _Copy(lhs->_child_, rhs->_child_);
    _Copy(lhs->_sibling_, rhs->_sibling_);
}


//
// Constructors and Destrcutor
//

template <typename T, typename Compare>
BinomialHeap<T, Compare>::_HeapNode::_HeapNode() : _child_(nullptr), _sibling_(nullptr), _degree_(0) {}


template <typename T, typename Compare>
BinomialHeap<T, Compare>::_HeapNode::_HeapNode(const T& key) : _key_(key), _child_(nullptr), _sibling_(nullptr), _degree_(0) {}


template <typename T, typename Compare>
BinomialHeap<T, Compare>::BinomialHeap() : _root(nullptr), _size(0) {}


template <typename T, typename Compare>
BinomialHeap<T, Compare>::BinomialHeap(const T& key) : _root(new _HeapNode(key)), _size(1) {}


template <typename T, typename Compare>
BinomialHeap<T, Compare>::BinomialHeap(const BinomialHeap<T, Compare>& rhs) : _root(nullptr), _size(0)
{
    (*this) = rhs;
}


template <typename T, typename Compare>
BinomialHeap<T, Compare>::~BinomialHeap()
{
    _ClearNodes(_root);
    _size = 0;
}

//
// Public methods
//

template <typename T, typename Compare>
BinomialHeap<T, Compare>& BinomialHeap<T, Compare>::operator = (const BinomialHeap<T, Compare>& rhs)
{
    if (this == &rhs) {
        return (*this);
    }
    
    _ClearNodes(_root);
    _size = rhs.Size();
    _Copy(_root, rhs._root);
    
    return (*this);
}

template <typename T, typename Compare>
size_t BinomialHeap<T, Compare>::Size() const
{
    return _size;
}

template <typename T, typename Compare>
bool BinomialHeap<T, Compare>::Empty() const
{
    return _size == 0;
}

template <typename T, typename Compare>
void BinomialHeap<T, Compare>::Insert(const T& element)
{
    BinomialHeap<T, Compare> rhs(element);
    _Merge((*this), rhs);
}

template <typename T, typename Compare>
T BinomialHeap<T, Compare>::GetMin() const
{
    if (!_root) {
        throw std::range_error("No elements in Heap");
    }
    return BinomialHeap<T, Compare>::_FindMin(_root)->_key_;
}

template <typename T, typename Compare>
T BinomialHeap<T, Compare>::ExtractMin()
{
    if (!_root) {
        throw std::range_error("No elements in Heap");
    }
    
    _HeapNodePtr& minimum = BinomialHeap<T, Compare>::_FindMin<_HeapNodePtr&>(_root);
    
    BinomialHeap rhs;
    rhs._size = (1 << minimum->_degree_) - 1;
    _size -= (1 << minimum->_degree_);
    
    _HeapNodePtr ptr = minimum->_child_;
    
    while (ptr != nullptr) {
        _HeapNodePtr next = ptr->_sibling_;
        
        ptr->_sibling_ = rhs._root;
        rhs._root = ptr;
        ptr = next;
    }
    
    _HeapNodePtr result_node = minimum;
    minimum = minimum->_sibling_;
    
    _Merge((*this), rhs);
    
    T result = result_node->_key_;
    delete result_node;
    
    return result;
}

template <typename T, typename Compare>
void BinomialHeap<T, Compare>::Meld(IHeap<T, Compare> & rhs)
{
    try {
        _Merge((*this), dynamic_cast<BinomialHeap<T, Compare>&>(rhs));
    } catch (const std::bad_cast& error) {
        std::cerr << error.what();
    };
}

#endif /* BinomialHeap_h */
