//
//  Testing.h
//  Heaps
//
//  Created by Алексей Зерцалов on 06.12.2019.
//  Copyright © 2019 Алексей Зерцалов. All rights reserved.
//

#ifndef Testing_h
#define Testing_h

#include <iostream>
#include <algorithm>
#include <time.h>
#include <chrono>
#include "gtest/gtest.h"
#include "BinomialHeap.h"
#include "LeftistHeap.h"
#include "SkewHeap.h"
#include "StlHeap.h"

namespace
{

using KeyType = int;
using HeapCompare = std::less<KeyType>;

template <typename T>
class HeapTesting : public ::testing::Test
{
protected:

    void SetUp() override; // Before all tests

    void TearDown() override; // After every test

    void AddHeap(const KeyType &key);
    void AddHeapStl(const KeyType &key);

    void Insert(size_t index, const KeyType &key);
    void InsertStl(size_t index, const KeyType &key);

    KeyType GetMin(size_t index);
    KeyType GetMinStl(size_t index);

    KeyType ExtractMin(size_t index);
    KeyType ExtractMinStl(size_t index);

    void Meld(size_t index1, size_t index2);
    void MeldStl(size_t index1, size_t index2);

public:
    unsigned int summaryTime = 0;
    std::vector <int> typeOfTests;

    std::vector <T> _heap;
    std::vector <StlHeap<KeyType, HeapCompare>> _stlHeap;
};

template <typename T>
void HeapTesting<T>::SetUp()
{
    _heap.clear();
    _stlHeap.clear();
    summaryTime = 0;
    typeOfTests.resize(5, 0);
}

template <typename T>
void HeapTesting<T>::TearDown()
{
    _heap.clear();
    _stlHeap.clear();
    if (summaryTime != 0) {
        std::cout << summaryTime << " - time of " << typeid(T).name() << "\n\n";
        std::cout << "Where:\n";
        std::cout << typeOfTests[0] << " - number of \"AddHeap\" tests\n";
        std::cout << typeOfTests[1] << " - number of \"Insert\" tests\n";
        std::cout << typeOfTests[2] << " - number of \"GetMin\" tests\n";
        std::cout << typeOfTests[3] << " - number of \"ExtractMin\" tests\n";
        std::cout << typeOfTests[4] << " - number of \"Meld\" tests\n";
    }
}

template <typename T>
void HeapTesting<T>::AddHeap(const KeyType &key)
{
    _heap.push_back(T(key));
}

template <typename T>
void HeapTesting<T>::AddHeapStl(const KeyType &key)
{
    _stlHeap.push_back(StlHeap<KeyType, HeapCompare>(key));
}

template <typename T>
void HeapTesting<T>::Insert(size_t index, const KeyType &key)
{
    _heap[index].Insert(key);
}

template <typename T>
void HeapTesting<T>::InsertStl(size_t index, const KeyType &key)
{
    _stlHeap[index].Insert(key);
}

template <typename T>
KeyType HeapTesting<T>::GetMin(size_t index)
{
    return _heap[index].GetMin();
}

template <typename T>
KeyType HeapTesting<T>::GetMinStl(size_t index)
{
    return _stlHeap[index].GetMin();
}

template <typename T>
KeyType HeapTesting<T>::ExtractMin(size_t index)
{
    return _heap[index].ExtractMin();
}

template <typename T>
KeyType HeapTesting<T>::ExtractMinStl(size_t index)
{
    return _stlHeap[index].ExtractMin();
}

template <typename T>
void HeapTesting<T>::Meld(size_t index1, size_t index2)
{
    _heap[index1].Meld(_heap[index2]);
    _heap.erase(_heap.begin() + index2);
}

template <typename T>
void HeapTesting<T>::MeldStl(size_t index1, size_t index2)
{
    _stlHeap[index1].Meld(_stlHeap[index2]);
    _stlHeap.erase(_stlHeap.begin() + index2);
}


} // end of namespace

void CreateTests(const size_t);

#endif /* Testing_h */