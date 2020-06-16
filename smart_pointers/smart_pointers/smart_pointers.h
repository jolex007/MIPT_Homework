#ifndef smart_pointers_h
#define smart_pointers_h

#include <memory>
#include <type_traits>

/*
 ControlBlock
 */

struct ControlBlock {
    void* ptr;
    size_t shared_count;
    size_t weak_count;
    
    template <typename T>
    ControlBlock(T*, size_t = 1, size_t = 0);
    
    virtual ~ControlBlock() = default;
    virtual void destroy();
};

template <typename T>
ControlBlock::ControlBlock(T* ptr, size_t shared_count, size_t weak_count) :
    ptr(static_cast<void*>(ptr)), shared_count(shared_count), weak_count(weak_count) {}

void ControlBlock::destroy() {
    ::operator delete(ptr);
    delete this;
}

template <typename Alloc>
struct ControlBlockWithAllocator : public ControlBlock {
    Alloc alloc;
    const size_t size;
    
    using alloc_traits = std::allocator_traits<Alloc>;
    
    template <typename T>
    ControlBlockWithAllocator(T*, size_t = 1, size_t = 0);
    
    ~ControlBlockWithAllocator() override = default;
    void destroy() override;
};

template <typename Alloc>
template <typename T>
ControlBlockWithAllocator<Alloc>::ControlBlockWithAllocator(T* ptr, size_t shared_count, size_t weak_count) :
    ControlBlock(ptr, shared_count, weak_count), alloc(), size(sizeof(T)) {}

template <typename Alloc>
void ControlBlockWithAllocator<Alloc>::destroy() {
    using alloc_to_deall = typename std::allocator_traits<Alloc>::template rebind_alloc<char>;
    alloc_to_deall allocator;
    using alloc_trait = std::allocator_traits<alloc_to_deall>;
    alloc_trait::deallocate(allocator, reinterpret_cast<char*>(this), size + sizeof(ControlBlockWithAllocator<Alloc>));
}

/*
 Smart pointers
 */

template <typename T>
class SharedPtr;

template <typename T>
class WeakPtr;

template <typename T>
struct EnableSharedFromThis;

/*
 SharedPtr
 */

template <typename T>
class SharedPtr {
public:
    using value_t = T;
    using ptr_t = T*;
    using ref_t = T&;
    
    // Constructors
    SharedPtr();
    
    template <typename U>
    SharedPtr(U*);
    
    SharedPtr(const SharedPtr&);
    
    template <typename U>
    SharedPtr(const SharedPtr<U>&);
    
    template <typename U>
    SharedPtr(SharedPtr<U>&&);
    
    template <typename U>
    SharedPtr(SharedPtr&&);
    
    template <typename U>
    SharedPtr<T>& operator=(U&&);
    
    // Destructor
    ~SharedPtr<T>();
    
    // Other methods/operators
    size_t use_count() const;
    
    void reset();
    template <typename U>
    void reset(U*);
    
    void swap(SharedPtr<T>&);
    
    T* get() const;
    T& operator*() const;
    T* operator->() const;
    explicit operator bool() const;
    
    template <typename U>
    friend class SharedPtr;
    template <typename U>
    friend class WeakPtr;
    
    template <typename U, typename... Args>
    friend SharedPtr<U> makeShared(Args&&... args);
    
    template <typename U, typename Alloc, typename... Args>
    friend SharedPtr<U> allocateShared(const Alloc&, Args&&...);
    
private:
    ptr_t ptr;
    ControlBlock* counter;
    
    // For make_shared / allocate_shared
    
    SharedPtr(T*, ControlBlock*);
    
    template <typename Alloc, typename... Args>
    SharedPtr(const Alloc&, Args&& ...args);
};

template <typename T>
SharedPtr<T>::SharedPtr() : ptr(nullptr), counter(nullptr) {}

template <typename T>
template <typename U>
SharedPtr<T>::SharedPtr(U* ptr) : ptr(ptr), counter(new ControlBlock(ptr)) {
    if constexpr (std::is_base_of_v<EnableSharedFromThis<U>, U>) {
        ptr->weak_ptr = WeakPtr<T>(ptr);
    }
}

template <typename T>
template <typename U>
SharedPtr<T>::SharedPtr(const SharedPtr<U>& other) : ptr(other.ptr), counter(other.counter) {
    counter->shared_count++;
}

template <typename T>
SharedPtr<T>::SharedPtr(const SharedPtr<T>& other) : ptr(other.ptr), counter(other.counter) {
    if (counter == nullptr) {
        return;
    }
    counter->shared_count++;
}

template <typename T>
template <typename U>
SharedPtr<T>::SharedPtr(SharedPtr<T>&& other) : ptr(other.ptr), counter(other.counter) {
    other.counter = nullptr;
    other.ptr = nullptr;
}

template <typename T>
template <typename U>
SharedPtr<T>::SharedPtr(SharedPtr<U>&& other) : ptr(other.ptr), counter(other.counter) {
    other.ptr = nullptr;
    other.counter = nullptr;
}

template <typename T>
SharedPtr<T>::SharedPtr(T* elem_ptr, ControlBlock* counter) : ptr(elem_ptr), counter(counter) {
    counter->shared_count++;
}

template <typename T>
template <typename Alloc, typename... Args>
SharedPtr<T>::SharedPtr(const Alloc&, Args&&... args) {
    Alloc alloc;
    using alloc_traits = typename std::allocator_traits<Alloc>;
    
    using alloc_char_t = typename std::allocator_traits<Alloc>::template rebind_alloc<char>;
    alloc_char_t alloc_char;
    using alloc_char_traits = std::allocator_traits<alloc_char_t>;
    
    using alloc_counter_t = typename std::allocator_traits<Alloc>::template rebind_alloc<ControlBlockWithAllocator<Alloc>>;
    alloc_counter_t alloc_counter;
    using alloc_counter_traits = std::allocator_traits<alloc_counter_t>;
    
    char* all_ptr = alloc_char_traits::allocate(alloc_char, sizeof(T) + sizeof(ControlBlockWithAllocator<Alloc>));
    ptr = reinterpret_cast<T*>(all_ptr + sizeof(ControlBlockWithAllocator<Alloc>));
    auto* counter_ = reinterpret_cast<ControlBlockWithAllocator<Alloc>*>(all_ptr);
    
    alloc_traits::construct(alloc, ptr, std::forward<Args>(args)...);
    alloc_counter_traits::construct(alloc_counter, counter_, ptr);
    
    counter = dynamic_cast<ControlBlock*>(counter_);
    
    if constexpr (std::is_base_of_v<EnableSharedFromThis<T>, T>) {
        ptr->weak_ptr = WeakPtr<T>(*this);
    }
}

template <typename T>
template <typename U>
SharedPtr<T>& SharedPtr<T>::operator=(U&& other) {
    SharedPtr<T>(std::forward<U>(other)).swap(*this);
    return *this;
}


template <typename T>
SharedPtr<T>::~SharedPtr() {
    if (counter == nullptr) {
        return;
    }
    
    if (counter->shared_count == 1) {
        ptr->~T();
        if (counter->weak_count == 0) {
            counter->destroy();
        } else {
            --counter->shared_count;
        }
    } else {
        --counter->shared_count;
    }
}

template <typename T>
size_t SharedPtr<T>::use_count() const {
    return (counter ? counter->shared_count : 0);
}

template <typename T>
void SharedPtr<T>::reset() {
    SharedPtr<T>().swap(*this);
}

template <typename T>
template <typename U>
void SharedPtr<T>::reset(U* ptr) {
    SharedPtr<T>().swap(*this);
    SharedPtr<T>(ptr).swap(*this);
}

template <typename T>
void SharedPtr<T>::swap(SharedPtr<T>& other) {
    std::swap(counter, other.counter);
    std::swap(ptr, other.ptr);
}

template <typename T>
T* SharedPtr<T>::get() const {
    return ptr;
}

template <typename T>
T& SharedPtr<T>::operator*() const {
    return *ptr;
}

template <typename T>
T* SharedPtr<T>::operator->() const {
    return ptr;
}

/*
 functions to create SharedPtr
 */

template <typename T, typename... Args>
SharedPtr<T> makeShared(Args&&... args) {
    return SharedPtr<T>(std::allocator<T>(), std::forward<Args>(args)...);
}

template <typename T, typename Alloc, typename... Args>
SharedPtr<T> allocateShared(const Alloc& alloc, Args&&... args) {
    return SharedPtr<T>(alloc, std::forward<Args>(args)...);
}

/*
 WeakPtr
 */

template <typename T>
class WeakPtr {
public:
    using value_t = T;
    using ptr_t = T*;
    using ref_t = T&;
    
    // Constructors
    WeakPtr();
    
    WeakPtr(const WeakPtr&);
    
    template <typename U>
    WeakPtr(const WeakPtr<U>&);
    
    template <typename U>
    WeakPtr(WeakPtr<U>&&);
    
    WeakPtr(WeakPtr<T>&&);
    
    template <typename U>
    WeakPtr(const SharedPtr<U>&);
    
    // Destructor
    ~WeakPtr<T>();
    
    // Assigments
    template <typename U>
    WeakPtr<T>& operator=(const SharedPtr<U>&);
    template <typename Other>
    WeakPtr<T>& operator=(Other&& other);
    
    
    // Other methods/operators
    size_t use_count() const;
    
    void reset();
    template <typename U>
    void reset(U*);
    template <typename U, typename Alloc>
    void reset(U*, Alloc);
    
    void swap(WeakPtr<T>&);
    
    T* get() const;
    T& operator*() const;
    T* operator->() const;
    explicit operator bool() const;
    
    SharedPtr<T> lock() const;
    bool expired() const;
    
    template <typename U>
    friend class WeakPtr;
    
    template <typename U>
    friend class SharedPtr;
    
private:
    ptr_t ptr;
    ControlBlock* counter;
    
    // For make_shared / allocate_shared
    WeakPtr(T*, char*);
    
    template <typename Alloc>
    WeakPtr(T*, char*, Alloc);
};

template <typename T>
WeakPtr<T>::WeakPtr() : ptr(nullptr), counter(nullptr) {}

template <typename T>
template <typename U>
WeakPtr<T>::WeakPtr(const WeakPtr<U>& other) : ptr(other.ptr), counter(other.counter) {
    if (counter == nullptr) {
        return;
    }
    counter->weak_count++;
}

template <typename T>
WeakPtr<T>::WeakPtr(const WeakPtr<T>& other) : ptr(other.ptr), counter(other.counter) {
    if (counter == nullptr) {
        return;
    }
    counter->weak_count++;
}

template <typename T>
template <typename U>
WeakPtr<T>::WeakPtr(const SharedPtr<U>& other) : ptr(other.ptr), counter(other.counter) {
    counter->weak_count++;
}

template <typename T>
template <typename U>
WeakPtr<T>::WeakPtr(WeakPtr<U>&& other) : ptr(other.ptr), counter(other.counter) {
    other.ptr = nullptr;
    other.counter = nullptr;
}

template <typename T>
WeakPtr<T>::WeakPtr(WeakPtr<T>&& other) : ptr(other.ptr), counter(other.counter) {
    other.ptr = nullptr;
    other.counter = nullptr;
}

template <typename T>
WeakPtr<T>::~WeakPtr() {
    if (counter == nullptr) {
        return;
    }
    --counter->weak_count;
    if (counter->weak_count == 0 && counter->shared_count == 0) {
        counter->destroy();
    }
}

template <typename T>
template <typename U>
WeakPtr<T>& WeakPtr<T>::operator=(const SharedPtr<U>& other) {
    WeakPtr<T>(other).swap(*this);
    return *this;
}

template <typename T>
template <typename Other>
WeakPtr<T>& WeakPtr<T>::operator=(Other &&other) {
    WeakPtr<T>(std::forward<Other>(other)).swap(*this);
    return *this;
}

template <typename T>
size_t WeakPtr<T>::use_count() const {
    return (counter ? counter->shared_count : 0);
}

template <typename T>
void WeakPtr<T>::reset() {
    WeakPtr().swap(*this);
}

template <typename T>
template <typename U>
void WeakPtr<T>::reset(U* ptr) {
    WeakPtr().swap(*this);
    WeakPtr(ptr).swap(*this);
}

template <typename T>
template <typename U, typename Alloc>
void WeakPtr<T>::reset(U* ptr, Alloc alloc) {
    WeakPtr().swap(*this);
    WeakPtr(ptr, alloc).swap(*this);
}

template <typename T>
void WeakPtr<T>::swap(WeakPtr<T>& other) {
    std::swap(counter, other.counter);
    std::swap(ptr, other.ptr);
}

template <typename T>
T* WeakPtr<T>::get() const {
    return ptr;
}

template <typename T>
bool WeakPtr<T>::expired() const {
    return counter->shared_count == 0;
}

template <typename T>
SharedPtr<T> WeakPtr<T>::lock() const {
    return expired() ? SharedPtr<T>() : SharedPtr<T>(ptr, counter);
}

template <typename T>
T& WeakPtr<T>::operator*() const {
    return *ptr;
}

template <typename T>
T* WeakPtr<T>::operator->() const {
    return ptr;
}


/*
 EnableSharedFromThis
 */

template <typename T>
struct EnableSharedFromThis {
private:
    WeakPtr<T> weak_ptr = WeakPtr<T>();
    
public:
    template <typename U>
    friend class SharedPtr;
    
    virtual SharedPtr<T> shared_from_this();
};

template <typename T>
SharedPtr<T> EnableSharedFromThis<T>::shared_from_this() {
    if (weak_ptr.use_count() == 0) {
        throw std::bad_weak_ptr();
    } else {
        return weak_ptr.lock();
    }
}



#endif /* smart_pointers_h */
