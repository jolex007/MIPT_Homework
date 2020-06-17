#include <iostream>

//если на вход дан итератор, который поддерживает прибавление числа, то отработает за О(1)
//иначе - за О(n)

//нельзя писать в одной функции через if, шаблонный диспатч обязателен - иначе будет CE на строчке it += n
//поэтому делаем специализацию для true, т.е. для случая, когда переданный итератор - RA

template<bool is_RA>
void advance_impl(Iterator& it, int n) {
    for (int i = 0; i < n; ++i) {
        ++it;
    }
}

template<>
void advance_impl<true>(Iterator& it, int n) {
    it += n;
}

template<typename Iterator>
void advance(Iterator& it, int n) {
    advance_impl<std::is_same<typename std::iterator_traits<Iterator>::iterator_category,
                 std::random_access_iterator_tag>::value>(it, n);
}

//distance - ищет разницу между двумя указателями, работает за О(1), если на вход подан RA итератор, и за О(n), если
//на вход подается любой другой итератор

template<bool is_RA>
int distance_impl(Iterator first, Iterator second) {
    int k = 0;

    while (first != second) {
        ++first;
        ++k;
    }

    return k;
}

template<>
int distance_impl<true>(Iterator first, Iterator second) {
    return second - first;
}

template<typename Iterator>
int distance(Iterator first, Iterator second) {
    distance_impl<std::is_same<typename std::iterator_traits<Iterator>::iterator_category,
                  std::random_access_iterator_tag>::value>(first, second);
}
