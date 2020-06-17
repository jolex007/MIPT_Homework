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
struct has_method_name {
private:
    template <typename T>
    static decltype(Class().name(Args()...), bool()) f(T) {
        return 0;
    }
    
    template <typename...>
    static char f(...) {
        return 0;
    }
    
public:
    static const bool value = is_same<decltype(f(0)), char>::value;
};


int main()
{
    
    
    return 0;
}
