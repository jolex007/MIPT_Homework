//
//  main.cpp
//  Heaps
//
//  Created by Алексей Зерцалов on 05.12.2019.
//  Copyright © 2019 Алексей Зерцалов. All rights reserved.
//

#include "BinomialHeap.h"
#include "LeftistHeap.h"
#include "SkewHeap.h"
#include "StlHeap.h"
#include "Testing.h"
#include "gtest/gtest.h"

int main(int argc, char ** argv) {

    CreateTests(1000);

    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
