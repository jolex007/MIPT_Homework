#include <iostream>

template <typename T, typename U>
struct is_same {
    static const bool value = false;
};

template <typename T>
struct is_same<T, T> {
    static const bool value = true;
};


template <typename Class, typename... Args>
struct is_constructible {
private:
    template <typename T, typename... Args2>
    static decltype(T(std::declval<Args2>()...), char()) f(int x) {
        return 0;
    }
    
    template <typename...>
    static int f(...) {
        return 0;
    }
    
public:
    static const bool value = is_same<decltype(f<Class, Args...>(0)), char>::value;
};

struct S {
    S(int, int);
};

int main()
{
    std::cout << is_constructible<S, int, int>::value;
    
    return 0;
}
