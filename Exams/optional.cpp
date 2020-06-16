#include <iostream>

/*
 Этот тип означает, что в optional нет объекта
 */
struct nullopt_t {
    explicit nullopt_t(int) {}
};

struct bad_optional_access {
    
};


/*
 Простой аналог std::optional
 
 Идея этого класса в том, что он может как хранить объект, так и не хранить
 Например, если нам нужно какое-то зарезервированное число, то мы часто используем -1 или INT_MIN
 Для этого и есть optional. Он может хранить flag чтобы понять есть ли в нем объект или нет
 
 Различия с нормальной реализацией:
 - Нет поддержки наследования и тому подобного
 - Нет конструктора от элементов optional(Args&&... args)
*/
template <typename T>
class optional {
private:
    bool flag;
    T* elem;
    
public:
    
    // Constructors
    optional();
    optional( nullopt_t );
    
    optional( const optional& );
    optional( optional&& );
    
    optional( const T& );
    optional( T&& );
    
    ~optional() = default;
    
    // operator =
    optional& operator=( nullopt_t );
    
    optional& operator=( const optional& );
    optional& operator=( optional&& );
    
    optional& operator=( const T& );
    optional& operator=( T&& );
    
    // Main methods
    
    T* operator->();
    const T* operator->() const;
    T& operator*() &;
    const T& operator*() const&;
    T&& operator*() &&;
    const T&& operator*() const&&;
    
    operator bool() const;
    bool has_value() const;
    
    T& value() &;
    const T& value() const&;
    T&& value() &&;
    const T&& value() const&&;
    
    void reset();
};

/*
 Constructors
 */

template <typename T>
optional<T>::optional() : flag(false), elem(nullptr)
{}

template <typename T>
optional<T>::optional( nullopt_t ) : optional()
{}

template <typename T>
optional<T>::optional(const optional& other) : flag(other.flag), elem(new T(other.elem))
{}

template <typename T>
optional<T>::optional(optional&& other) : flag(other.flag), elem(other.elem) {
    other.elem = nullptr;
}

template <typename T>
optional<T>::optional(const T& element) : flag(true), elem(new T(element))
{}

template <typename T>
optional<T>::optional(T&& element) : flag(true), elem(new T(std::move(element)))
{}

/*
 Operator =
 */

template <typename T>
optional<T>& optional<T>::operator=( nullopt_t ) {
    flag = false;
    elem = nullptr;
    
    return *this;
}

template <typename T>
optional<T>& optional<T>::operator=(const optional<T>& other) {
    flag = other.flag;
    elem = new T(*other.elem);
    
    return *this;
}

template <typename T>
optional<T>& optional<T>::operator=(optional<T>&& other) {
    flag = other.flag;
    elem = other.elem;
    other.elem = nullptr;
    
    return *this;
}

template <typename T>
optional<T>& optional<T>::operator=(const T& element) {
    flag = true;
    elem = new T(*element);
    
    return *this;
}

template <typename T>
optional<T>& optional<T>::operator=(T&& element) {
    flag = true;
    elem = new T(std::move(element));
    
    return *this;
}

template <typename T>
T* optional<T>::operator->() {
    return elem;
}

template <typename T>
const T* optional<T>::operator->() const {
    return elem;
}

template <typename T>
T& optional<T>::operator*() & {
    return value();
}

template <typename T>
const T& optional<T>::operator*() const& {
    return value();
}

template <typename T>
T&& optional<T>::operator*() && {
    return value();
}

template <typename T>
const T&& optional<T>::operator*() const&& {
    return value();
}

template <typename T>
optional<T>::operator bool() const {
    return flag;
}

template <typename T>
bool optional<T>::has_value() const {
    return flag;
}

template <typename T>
T& optional<T>::value() & {
    if (has_value()) {
        return *elem;
    } else {
        throw bad_optional_access();
    }
}

template <typename T>
const T& optional<T>::value() const& {
    if (has_value()) {
        return *elem;
    } else {
        throw bad_optional_access();
    }
}

template <typename T>
T&& optional<T>::value() && {
    if (has_value()) {
        return *elem;
    } else {
        throw bad_optional_access();
    }
}

template <typename T>
const T&& optional<T>::value() const&& {
    if (has_value()) {
        return *elem;
    } else {
        throw bad_optional_access();
    }
}

template <typename T>
void optional<T>::reset() {
    delete elem;
    elem = nullptr;
    flag = false;
}

int main()
{

    return 0;
}
