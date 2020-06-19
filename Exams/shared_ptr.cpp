#include <iostream>

struct Counter {
    void* ptr;
    size_t count;
    
    template <typename T>
    Counter(T* ptr) : ptr(static_cast<void*>(ptr)), count(1) {
    }
};


template <typename T>
class SharedPtr {
public:
    SharedPtr() : counter(nullptr) {
    }
    
    SharedPtr(const SharedPtr& other) : counter(other.counter) {
        if (counter == nullptr) {
            return;
        }
        ++counter->count;
    }
    
    SharedPtr(SharedPtr&& other) : counter(other.counter) {
        other.counter = nullptr;
    }
    
    SharedPtr(T* ptr) : counter(new Counter(ptr)) {
    }
    
    SharedPtr& operator=(const SharedPtr& other) {
        counter = other.counter;
        ++counter->count;
    }
    
    SharedPtr& operator=(SharedPtr&& other) {
        counter = other.counter;
        other.counter = nullptr;
    }
    
    ~SharedPtr() {
        if (counter == nullptr) {
            return;
        }
        if (counter->count == 1) {
            delete static_cast<T*>(counter->ptr);
            delete counter;
        } else {
            --counter->count;
        }
    }
    
    void reset() {
        if (counter == nullptr) {
            return;
        }
        if (counter->count == 1) {
            delete static_cast<T*>(counter->ptr);
            delete counter;
        } else {
            --counter->count;
        }
        counter = nullptr;
    }
    
    T* get() const {
        return static_cast<T*>(counter->ptr);
    }
    
    size_t use_count() const {
        return (counter ? counter->count : 0);
    }
    
    T& operator*() const {
        return *get();
    }
    
    T* operator->() const {
        return get();
    }
    
    operator bool() const {
        return counter != nullptr;
    }
    
private:
    
    Counter* counter;
};

template <typename T, typename... Args>
SharedPtr<T> make_shared(Args&&... args) {
    T* ptr = new T(std::forward<Args>(args)...);
    return SharedPtr<T>(ptr);
}

template <typename T>
SharedPtr<T> make_shared() {
    return SharedPtr<T>();
}

int main() {
    
    return 0;
}