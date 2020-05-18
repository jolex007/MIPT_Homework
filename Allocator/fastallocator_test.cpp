#include <chrono>
#include <stdexcept>
#include <string>
#include <list>
#include <vector>
#include <iostream>
#include <cassert>

#include "fastallocator.h"

/*
template<typename T, typename Alloc = std::allocator<T>>
using List = std::list<T, Alloc>;

template<typename T>
using FastAllocator = std::allocator<T>;
*/

struct VerySpecialType {
    int x = 0;
    explicit VerySpecialType(int x): x(x) {}
};

struct NotDefaultConstructible {
    NotDefaultConstructible() = delete;
    NotDefaultConstructible(VerySpecialType x): x(x) {}
    VerySpecialType x;
};

template <class List>
int test_list(List&& l) {
    using namespace std::chrono;

    auto start = high_resolution_clock::now();
    for (int i = 0; i < 4000000; ++i) {
        l.push_back(i);
    }
    for (int i = 0; i < 2500000; ++i) {
        l.pop_front();
    }
    for (int i = 0; i < 1000000; ++i) {
        l.push_front(i);
    }
    for (int i = 0; i < 2500000; ++i) {
        l.pop_back();
    }
    auto finish = high_resolution_clock::now();
    return duration_cast<milliseconds>(finish - start).count();
}

template <class T, class Alloc>
void test_vector(std::vector<T, Alloc>&& v) {
    // auto start = high_resolution_clock::now();
    v.reserve(5000000);
    v.push_back(1);
    // std::cout << v.front();
    v.resize(2500000);
    // std::cout << v.back();
    v.pop_back();
    v.shrink_to_fit();
    for (int i = 0; i < 5000000; ++i) {
        v.push_back(i);
    }
    for (int i = 0; i < 5000000; ++i) {
        v.pop_back();
    }
    // auto finish = high_resolution_clock::now();
    // std::cout << duration_cast<milliseconds>(finish - start).count() << "ms" << std::endl;
}

struct Accountant {
    // Some field of strange size
    char arr[40];

    static size_t counter;
    Accountant() {
        ++counter;
    }
    Accountant(const Accountant&) {
        ++counter;
    }
    ~Accountant() {
        --counter;
    }
};

size_t Accountant::counter = 0;

template <typename Alloc = std::allocator<NotDefaultConstructible>>
void TestNotDefaultConstructible() {
    List<NotDefaultConstructible, Alloc> lst;
    assert(lst.size() == 0);
    lst.push_back(VerySpecialType(0));
    assert(lst.size() == 1);
    lst.pop_front();
    assert(lst.size() == 0);
}

template<typename Alloc = std::allocator<Accountant>>
void TestAccountant() {
    {
        List<Accountant, Alloc> lst(5);
        assert(lst.size() == 5);
        List<Accountant, Alloc> another = lst;
        assert(another.size() == 5);
        assert(Accountant::counter == 10);

        another.pop_back();
        another.pop_front();
        lst = another;
        assert(another.size() == 3);
        assert(lst.size() == 3);
        assert(Accountant::counter == 6);
    }
    
    assert(Accountant::counter == 0);
}

void TestConst() {
    const List<int> lst(5, 0);
    assert(lst.size() == 5);
    //assert(!lst.empty());
}

int main() {

    TestNotDefaultConstructible<>();
    TestNotDefaultConstructible<FastAllocator<NotDefaultConstructible>>();

    TestAccountant<>();
    TestAccountant<FastAllocator<Accountant>>();

    TestConst();

    auto first = test_list(std::list<int>());
    auto second = test_list(std::list<int, FastAllocator<int>>());
    std::cerr << first << ' ' << second << std::endl;
    if (first < second) {
        throw std::runtime_error("std::list with FastAllocator expected to be faster than with std::allocator, but there "
                "were " + std::to_string(second) + " ms instead of " + std::to_string(first) + "...\n");
    }
    test_vector(std::vector<char>());
    test_vector(std::vector<char, FastAllocator<char>>());
    first = test_list(List<int>());
    second = test_list(List<int, FastAllocator<int>>());
    std::cerr << first << ' ' << second << std::endl;
    if (first < second) {
        throw std::runtime_error("Custom List with FastAllocator expected to be faster than with std::allocator, but there "
                "were " + std::to_string(second) + " milliseconds instead of " + std::to_string(first) + "...\n");
    }
    std::cout << 0;
}
