//
//  main.cpp
//  RationalNumbers
//
//  Created by Алексей Зерцалов on 27.10.2019.
//  Copyright © 2019 Алексей Зерцалов. All rights reserved.
//

//#include <iostream>
#include "rational.h"
#include <iomanip>

using std::cin;
using std::cout;

int main() {
    
    Rational a = 1;
    Rational b = 2;
    Rational c = a / 2;
    cout << c.toString();
    
    
    
    return 0;
}
