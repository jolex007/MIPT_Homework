#include <iostream>
#include "biginteger.h"

using std::cin;
using std::cout;

int main()
{
    BigInteger a, b;
    cin >> a >> b;
    
    cout << (a > b);
    
    return 0;
}
