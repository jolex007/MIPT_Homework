#include <iostream>

template <typename T, typename U>
struct is_same {
    static const bool value = false;
};

template <typename T>
struct is_same<T, T> {
    static const bool value = true;
};



/*
 name - name of method
 */
template <typename Class, typename... Args>
struct has_method {
    
    template <typename Class2, typename... Args2>
    static decltype(std::declval<Class2>().method(std::declval<Args2>()...), int()) f(int) {
        return 0;
    }
    
    template <typename...>
    static char f(...) {
        return 0;
    }
  
    static const bool value = is_same<decltype(f<Class, Args...>(0)), int>::value;
};


int main()
{
    
    
    return 0;
}
