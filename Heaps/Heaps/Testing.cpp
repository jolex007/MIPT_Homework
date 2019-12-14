#include <iostream>
#include <algorithm>
#include "Testing.h"

using Heaps = testing::Types<BinomialHeap<KeyType, HeapCompare>, LeftistHeap<KeyType, HeapCompare>, SkewHeap<KeyType, HeapCompare> >;
TYPED_TEST_SUITE(HeapTesting, Heaps);

TYPED_TEST(HeapTesting, AddHeapTest)
{
    const size_t numberOfTests = 1000;

    for (size_t i = 0; i < numberOfTests; i++) {
        KeyType key = rand();

        ASSERT_NO_THROW(HeapTesting<TypeParam>::AddHeap(key));
        ASSERT_NO_THROW(HeapTesting<TypeParam>::AddHeapStl(key));
    }
}

TYPED_TEST(HeapTesting, Insert)
{
    const size_t numberOfTests = 1000;

    HeapTesting<TypeParam>::AddHeap(0);
    HeapTesting<TypeParam>::AddHeapStl(0);

    for (size_t i = 0; i < numberOfTests; i++) {
        KeyType key = rand();

        ASSERT_NO_THROW(HeapTesting<TypeParam>::Insert(0, key));
        ASSERT_NO_THROW(HeapTesting<TypeParam>::InsertStl(0, key));
    }
}

TYPED_TEST(HeapTesting, GetMin)
{
    const size_t numberOfTests = 1000;

    HeapTesting<TypeParam>::AddHeap(0);
    HeapTesting<TypeParam>::AddHeapStl(0);

    for (size_t i = 0; i < numberOfTests; i++) {
        KeyType key = rand();

        ASSERT_NO_THROW(HeapTesting<TypeParam>::Insert(0, key));
        ASSERT_NO_THROW(HeapTesting<TypeParam>::InsertStl(0, key));
        ASSERT_EQ(HeapTesting<TypeParam>::GetMin(0), HeapTesting<TypeParam>::GetMinStl(0));
    }
}

TYPED_TEST(HeapTesting, ExtractMin)
{
    const size_t numberOfTests = 1000;

    HeapTesting<TypeParam>::AddHeap(0);
    HeapTesting<TypeParam>::AddHeapStl(0);

    for (size_t i = 0; i < numberOfTests; i++) {
        KeyType key = rand();

        ASSERT_NO_THROW(HeapTesting<TypeParam>::Insert(0, key));
        ASSERT_NO_THROW(HeapTesting<TypeParam>::InsertStl(0, key));
    }

    for (size_t i = 0; i < numberOfTests; i++) {
        ASSERT_EQ(HeapTesting<TypeParam>::ExtractMin(0), HeapTesting<TypeParam>::ExtractMinStl(0));
    }
}

TYPED_TEST(HeapTesting, Meld)
{
    const size_t numberOfTests = 1000;

    for (size_t i = 0; i < numberOfTests + 1; i++) {
        KeyType key = rand();

        ASSERT_NO_THROW(HeapTesting<TypeParam>::AddHeap(key));
        ASSERT_NO_THROW(HeapTesting<TypeParam>::AddHeapStl(key));
    }

    for (size_t i = 0; i < numberOfTests; i++) {
        size_t index1 = rand() % HeapTesting<TypeParam>::_heap.size();
        size_t index2 = rand() % HeapTesting<TypeParam>::_heap.size();
        while (index2 == index1) {
            index2 = rand() % HeapTesting<TypeParam>::_heap.size();
        }

        ASSERT_NO_THROW(HeapTesting<TypeParam>::Meld(index1, index2));
        ASSERT_NO_THROW(HeapTesting<TypeParam>::MeldStl(index1, index2));
    }
}

TYPED_TEST(HeapTesting, AllTesting)
{
    const size_t numberOfTests = 1000;

    HeapTesting<TypeParam>::_heap.emplace_back();
    HeapTesting<TypeParam>::_stlHeap.emplace_back();

    for (size_t i = 0; i < numberOfTests; i++) {
        int operationType = rand() % 5;

        KeyType key;
        size_t index;
        size_t index1, index2;

        bool flag;

        switch(operationType)
        {
        case 0: // AddHeap

            key = rand();

            ASSERT_NO_THROW(HeapTesting<TypeParam>::AddHeap(key));
            ASSERT_NO_THROW(HeapTesting<TypeParam>::AddHeapStl(key));
        break;

        case 1: // Insert
        
            key = rand();
            index = rand() % HeapTesting<TypeParam>::_heap.size();

            ASSERT_NO_THROW(HeapTesting<TypeParam>::Insert(index, key));
            ASSERT_NO_THROW(HeapTesting<TypeParam>::InsertStl(index, key));
        break;

        case 2: // GetMin
        
            index = rand() % HeapTesting<TypeParam>::_heap.size();

            flag = true;

            try {
                key = HeapTesting<TypeParam>::GetMinStl(index);
            } catch (...) {
                flag = false;
            }
            if (flag) {
                ASSERT_EQ(HeapTesting<TypeParam>::GetMin(index), key);
            } else {
                ASSERT_ANY_THROW(HeapTesting<TypeParam>::GetMin(index));
            }
        break;

        case 3: // ExtractMin

            index = rand() % HeapTesting<TypeParam>::_heap.size();

            flag = true;

            try {
                key = HeapTesting<TypeParam>::ExtractMinStl(index);
            } catch (...) {
                flag = false;
            }

            if (flag) {
                ASSERT_EQ(HeapTesting<TypeParam>::ExtractMin(index), key);
            } else {
                ASSERT_ANY_THROW(HeapTesting<TypeParam>::ExtractMin(index));
            }

        break;

        case 4: // Meld
            
            if (HeapTesting<TypeParam>::_heap.size() <= 1) {
                break;
            }

            index1 = rand() % HeapTesting<TypeParam>::_heap.size();
            index2 = rand() % HeapTesting<TypeParam>::_heap.size();
            while (index2 == index1) {
                index2 = rand() % HeapTesting<TypeParam>::_heap.size();
            }

            ASSERT_NO_THROW(HeapTesting<TypeParam>::Meld(index1, index2));
            ASSERT_NO_THROW(HeapTesting<TypeParam>::MeldStl(index1, index2));
        break;
        }
    }

}