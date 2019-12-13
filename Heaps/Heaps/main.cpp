//
//  main.cpp
//  Heaps
//
//  Created by Алексей Зерцалов on 05.12.2019.
//  Copyright © 2019 Алексей Зерцалов. All rights reserved.
//

#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <set>
#include <algorithm>
#include "BinomialHeap.h"
#include "LeftistHeap.h"
#include "SkewHeap.h"
#include "StlHeap.h"
#include "Testing.h"
#include "gtest/gtest.h"


template <typename T = int, typename Comp = std::less<T>, typename HeapType = LeftistHeap<T, Comp>>
bool is_equal(HeapType heap, const std::multiset<T, Comp>& st) {
    if (heap.Size() != st.size()) {
        std::cerr << "SIZE!: " << heap.Size() << " " << st.size() << std::endl;
        return false;
    }
    for (const T& elem : st) {
        T min_elem = heap.ExtractMin();
        if (min_elem != elem) {
            std::cout << "elem: " << elem << " | heap: " << min_elem << "\n";
            std::cerr << "MIN!" << std::endl;
            return false;
        }
     }
//    std::cout << "LOL!!!\n";
     return true;
}

template <typename T = int, typename Comp = std::less<T>, typename HeapType = LeftistHeap<T, Comp>>
void test_heap(int num) {
     std::vector<std::multiset<T, Comp>> st(num);
     std::vector<HeapType> heap;

     for (int i = 0; i < num; ++i) {
         heap.push_back(HeapType());
     }

     auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();

     std::cerr << "SEED: " << seed << std::endl;

     std::mt19937 rng(seed);

     std::uniform_int_distribution<> get(0, 200);
     std::uniform_int_distribution<> number(1, 1000);

     for (int i = 0; i < num; ++i) {
         int size = get(rng);
         for (int j = 0; j < size; ++j) {
             int cur = number(rng);
             std::cout << cur << "\n";
             st[i].insert(cur);
             heap[i].Insert(cur);
             is_equal<T, Comp, HeapType>(heap[i], st[i]);
         }
     }
     for (int i = 1; i < num; ++i) {
         is_equal<T, Comp, HeapType>(heap.front(), st.front());
         heap.front().Meld(heap[i]);
         st.front().insert(st[i].begin(), st[i].end());
         is_equal<T, Comp, HeapType>(heap.front(), st.front());
     }
     std::cerr << "OK" << std::endl;
}

int main(int argc, char ** argv) {
    
    // test_heap<int, std::less<int>, StlHeap<int, std::less<int>>>(20);
    
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
