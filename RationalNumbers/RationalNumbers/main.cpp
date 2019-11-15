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
    BigInteger a1;
    BigInteger b1;
    Rational a;
    
    cin >> a1 >> b1;
    a = a1;
    a /= b1;
    
    cout << a.toString() << "\n" << a.asDecimal(5);
    
    
    
    return 0;
}
