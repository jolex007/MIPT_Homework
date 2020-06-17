#include <iostream>

using namespace std;

template <typename T, std::size_t N>
class Array {
    T buffer[N];

public:
    T& operator[](std::size_t indx) {
        return buffer[indx];
    }

    T operator[](std::size_t indx) const {
        return buffer[indx];
    }
    
    std::size_t size() const {
        return N;
    }

    T& front() {
        return buffer[0];
    }

    T front() const {
        return buffer[0];
    }

    T& back() {
        return buffer[size() - 1];
    }

    T back() const {
        return buffer[size() - 1];
    }

    bool isEmpty() {
        return size() == 0;
    }

    void fill(const T& value) {
        for (size_t i = 0; i < size(); ++i) {
            buffer[i] = value;
        }
    }

    T* data() {
        return buffer;
    }

    const T* data() const {
        return buffer;
    }
};

template <typename T, std::size_t N>
void swap(Array<T, N>& first, Array<T, N>& second) noexcept {
    for (size_t i = 0; i < N; ++i) {
        std::swap(first.buffer[i], second.buffer[i]);
    }
}

template <typename T, std::size_t N>
bool operator==(const Array<T, N>& first, const Array<T, N>& second) {
    for (std::size_t i = 0; i < N; ++i) {
        if (first.buffer[i] != second.buffer[i]) {
            return false;
        }
    }

    return true;
}

template <typename T, std::size_t N>
bool operator!=(const Array<T, N>& first, const Array<T, N>& second) {
    return !(first == second);
}

int main() {
    
    return 0;
}
