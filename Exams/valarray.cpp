#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

template <typename T>
class valarray {
public:
    valarray() = default;
    valarray(size_t, T = T());
    valarray(const valarray&) = default;
    valarray(valarray&&) = default;
    
    valarray<T>& operator=(const valarray<T>&) = default;
    valarray<T>& operator=(valarray&&) = default;
    
    valarray<T>& operator+=(const valarray<T>&);
    valarray<T>& operator*=(const valarray<T>&);
    /* ... */
    
    valarray<T> operator+(const valarray<T>&) const;
    valarray<T> operator*(const valarray<T>&) const;
    
    valarray<bool> operator==(const valarray<T>&) const;
    valarray<bool> operator!=(const valarray<T>&) const;
    
    size_t size() const;
    
    const T& operator[](size_t) const;
    T& operator[](size_t);
    
    friend valarray<T> sin(const valarray<T>& array);
    friend valarray<T> cos(const valarray<T>& array);

    
private:
    
    inline static valarray<T>& operation_with_arr(valarray<T>&, const valarray<T>&, std::function<void(T&, const T&)>);
    inline static valarray<T> operation_with_arr(const valarray<T>&, const valarray<T>&, std::function<bool(const T&, const T&)>);
    inline static valarray<T> operation_with_arr(const valarray<T>&, std::function<T(T)>);
    
    vector<T> arr;
};

template <typename T>
valarray<T>::valarray(size_t size, T elem) : arr(size, elem)
{}

template <typename T>
size_t valarray<T>::size() const {
    return arr.size();
}

template <typename T>
const T& valarray<T>::operator[](size_t pos) const {
    return arr[pos];
}

template <typename T>
T& valarray<T>::operator[](size_t pos) {
    return arr[pos];
}

template <typename T>
valarray<T>& valarray<T>::operation_with_arr(valarray<T>& lhs, const valarray<T>& rhs, std::function<void(T&, const T&)> func) {
    for (size_t i = 0; i < lhs.size(); i++) {
        func(lhs[i], rhs[i]);
    }
    
    return lhs;
}

template <typename T>
valarray<T> valarray<T>::operation_with_arr(const valarray<T>& lhs, const valarray<T>& rhs, std::function<bool(const T&, const T&)> func) {
    valarray<bool> result(lhs.size());
    
    for (size_t i = 0; i < lhs.size(); i++) {
        func(lhs[i], rhs[i]);
    }
    
    return lhs;
}

template <typename T>
valarray<T>& valarray<T>::operator+=(const valarray<T>& other) {
    return operation_with_arr(*this, other, [](T& lhs, const T& rhs) -> void {
        lhs += rhs;
    });
}

template <typename T>
valarray<T>& valarray<T>::operator*=(const valarray<T>& other) {
    return operation_with_arr(*this, other, [](T& lhs, const T& rhs) -> void {
        lhs *= rhs;
    });
}

template <typename T>
valarray<T> valarray<T>::operator+(const valarray<T>& rhs) const {
    return valarray<T>(*this) += rhs;
}

template <typename T>
valarray<T> valarray<T>::operator*(const valarray<T>& rhs) const {
    return valarray<T>(*this) *= rhs;
}

template <typename T>
valarray<bool> valarray<T>::operator==(const valarray<T>& other) const {
    return operation_with_arr(*this, other, [](const T& lhs, const T& rhs) -> bool {
        return lhs == rhs;
    });
}

template <typename T>
valarray<bool> valarray<T>::operator!=(const valarray<T>& other) const {
    return operation_with_arr(*this, other, [](const T& lhs, const T& rhs) -> bool {
        return lhs != rhs;
    });
}


/*
 Other functions
 */

template <typename T>
valarray<T> sin(const valarray<T>& array) {
    return operation_with_arr(array, std::sin);
}

template <typename T>
valarray<T> cos(const valarray<T>& array) {
    return operation_with_arr(array, std::cos);
}


int main() {
    
    
    return 0;
}
