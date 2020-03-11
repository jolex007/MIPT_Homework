//
//  main.cpp
//  Matrix
//
//  Created by Алексей Зерцалов on 27.02.2020.
//  Copyright © 2020 Алексей Зерцалов. All rights reserved.
//

#include <iostream>
#include <vector>
#include "Finite.hpp"
#include "Matrix.hpp"
#include "Rational.hpp"

int main() {
    Matrix<2, 2, Rational> a;
    std::cin >> a;
    std::cout << a.inverted();
    
    
    return 0;
}
