#ifndef fastallocator_h
#define fastallocator_h

#include <vector>

//
//
// FIXED ALLOCATOR
//
//

template <size_t CHUNK_SIZE>
struct FixedAllocator {
private:
    
    size_t _capacity_ = 32;
    size_t _size_ = 0;
    
    std::vector <void*> _chunk_array_;
    
    void _supplement_memory_();
    
    static FixedAllocator<CHUNK_SIZE>* _allocator_;
    
    FixedAllocator() = default;
    
public:
    
    static FixedAllocator<CHUNK_SIZE>* getFixedAllocator();
    
    ~FixedAllocator();
    
    void* allocate();
    void deallocate();
};

template <size_t CHUNK_SIZE>
FixedAllocator<CHUNK_SIZE>* FixedAllocator<CHUNK_SIZE>::_allocator_ = nullptr;

template <size_t CHUNK_SIZE>
void FixedAllocator<CHUNK_SIZE>::_supplement_memory_() {
    void* new_chunk = ::operator new(_capacity_ * CHUNK_SIZE);
    _capacity_ *= 2;
    _chunk_array_.push_back(new_chunk);
    _size_ = 0;
}

template <size_t CHUNK_SIZE>
FixedAllocator<CHUNK_SIZE>* FixedAllocator<CHUNK_SIZE>::getFixedAllocator() {
    if (_allocator_ == nullptr) {
        _allocator_ = new FixedAllocator<CHUNK_SIZE>();
    }
    return _allocator_;
}

template <size_t CHUNK_SIZE>
void* FixedAllocator<CHUNK_SIZE>::allocate() {
    if (_size_ == _capacity_)
        _supplement_memory_();
    
    return reinterpret_cast<char*>(_chunk_array_.back()) + _size_ * CHUNK_SIZE;
}


template <size_t CHUNK_SIZE>
void FixedAllocator<CHUNK_SIZE>::deallocate() {}


template <size_t CHUNK_SIZE>
FixedAllocator<CHUNK_SIZE>::~FixedAllocator() {
    for (size_t i = 0; i < _chunk_array_.size(); i++) {
        delete _chunk_array_[i];
    }
}

//
//
// FAST ALLOCATOR
//
//



template <typename T>
struct FastAllocator {
private:
    static const size_t maxSize = 8;
    static FixedAllocator<sizeof(T)>* fixedAllocator;
    
public:
    
    FastAllocator() = default;
    
    T* allocate(size_t);
    void deallocate(T*, size_t);
    
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    
    template <typename U>
    struct rebind;
};

template <typename T>
FixedAllocator<sizeof(T)>* FastAllocator<T>::fixedAllocator = FixedAllocator<sizeof(T)>::getFixedAllocator();

template <typename T>
T* FastAllocator<T>::allocate(size_t n) {
    if (sizeof(T) <= maxSize && n <= 1) {
        return reinterpret_cast<T*>(fixedAllocator->allocate());
    } else {
        return reinterpret_cast<T*>(::operator new(n * sizeof(T)));
    }
}

template <typename T>
void FastAllocator<T>::deallocate(T *point, size_t n) {
    if (sizeof(T) > maxSize || n > 1) {
        ::operator delete(point);
    }
}

template <typename T>
template <typename U>
struct FastAllocator<T>::rebind {
    typedef FastAllocator<U> other;
};


//
//
// LIST
//
//

template <typename T, typename Allocator = std::allocator<T>>
struct List {
public:
    
    struct Node {
        T a;
        Node* next;
        Node* prev;
        Node(const T& value) : a(value) {
            next = prev = nullptr;
        }
    };
    
    explicit List(const Allocator& alloc = Allocator());
    List(size_t count, const T& value = T(), const Allocator& alloc = Allocator());
    List(const List& right);
    List& operator=(const List& right);
    ~List();

    size_t size() const;
    void pop_front();
    void pop_back();
    
    void push_front(const T& value);
    void push_back(const T& value);
    
    void insert_before(Node*, const T& value);
    void insert_after(Node*, const T&value);
    
    void erase(Node*);
    
private:
    
    size_t _size_ = 0;
    
    Node* _begin_ = nullptr;
    Node* _end_ = nullptr;
    
    void _copy_(const List<T, Allocator>&);
    
    Allocator _alloc_;
    
    using _node_allocator_type_ = typename std::allocator_traits<Allocator>::template rebind_alloc<Node>;
    _node_allocator_type_ _node_alloc_;
    using _alloc_traits_ = std::allocator_traits<_node_allocator_type_>;
};


template <typename T, typename Allocator>
List<T, Allocator>::List(const Allocator& alloc) {
    _alloc_ = alloc;
}

template <typename T, typename Allocator>
List<T, Allocator>& List<T, Allocator>::operator= (const List<T, Allocator>& rhs) {
    if (this == &rhs) {
        return *this;
    }
    
    while (_size_ > 0) {
        pop_back();
    }
    _copy_(rhs);
    
    return *this;
}

template <typename T, typename Allocator>
List<T, Allocator>::List(const List<T, Allocator>& rhs) {
    _copy_(rhs);
}

template <typename T, typename Allocator>
List<T, Allocator>::List(size_t count, const T& value, const Allocator& alloc) : List(alloc) {
    for (size_t i = 0; i < count; i++) {
        push_back(value);
    }
}

template <typename T, typename Allocator>
List<T, Allocator>::~List() {
    while (_size_ > 0) {
        pop_back();
    }
}

template <typename T, typename Allocator>
void List<T, Allocator>::_copy_(const List<T, Allocator>& rhs) {
    Node* start = rhs._begin_;
    for (size_t i = 0; i < rhs._size_; i++) {
        push_back(start->a);
        start = start->next;
    }
}

template <typename T, typename Allocator>
size_t List<T, Allocator>::size() const {
    return this->_size_;
}

template <typename T, typename Allocator>
void List<T, Allocator>::pop_front() {
    if (!_size_) {
        return;
    }
    
    erase(_begin_);
}

template <typename T, typename Allocator>
void List<T, Allocator>::pop_back() {
    if (!_size_) {
        return;
    }
    
    erase(_end_);
}

template <typename T, typename Allocator>
void List<T, Allocator>::push_front(const T& value) {
    Node* newElem = _alloc_traits_::allocate(_node_alloc_, 1);
    _alloc_traits_::construct(_node_alloc_, newElem, value);
    
    newElem->next = _begin_;
    
    if (_begin_) {
        _begin_->prev = newElem;
    } else {
        _end_ = newElem;
    }
    
    _begin_ = newElem;
    _size_++;
}

template <typename T, typename Allocator>
void List<T, Allocator>::push_back(const T &value) {
    Node* new_element = _alloc_traits_::allocate(_node_alloc_, 1);
    _alloc_traits_::construct(_node_alloc_, new_element, value);
    
    new_element->prev = _end_;
    
    if (_end_) {
        _end_->next = new_element;
    } else {
        _begin_ = new_element;
    }
    
    _end_ = new_element;
    _size_++;
}

template <typename T, typename Allocator>
void List<T, Allocator>::insert_before(Node* pointer, const T& value) {
    Node* new_element = _alloc_traits_::allocate(_node_alloc_, 1);
    _alloc_traits_::construct(_node_alloc_, new_element, value);
    
    new_element->next = pointer;
    new_element->prev = pointer->prev;
    
    if (pointer->prev){
        pointer->prev->next = new_element;
    } else {
        _begin_ = new_element;
    }
    
    pointer->prev = new_element;
    _size_++;
}

template <typename T, typename Allocator>
void List<T, Allocator>::insert_after(Node* pointer, const T& value) {
    Node* new_element = _alloc_traits_::allocate(_node_alloc_, 1);
    _alloc_traits_::construct(_node_alloc_, new_element, value);
    
    new_element->prev = pointer;
    new_element->next = pointer->next;
    
    if (pointer->next){
        pointer->next->prev = new_element;
    } else {
        _end_ = new_element;
    }
    
    pointer->next = new_element;
    _size_++;
}

template <typename T, typename Allocator>
void List<T, Allocator>::erase(Node* pointer) {
    if (pointer->next) {
        pointer->next->prev = pointer->prev;
    } else {
        _end_ = pointer->prev;
    }
    
    if (pointer->prev) {
        pointer->prev->next = pointer->next;
    } else {
        _begin_ = pointer->next;
    }
    
    _alloc_traits_::destroy(_node_alloc_, pointer);
    _alloc_traits_::deallocate(_node_alloc_, pointer, 1);
    
    _size_--;
}




#endif /* fastallocator_h */
