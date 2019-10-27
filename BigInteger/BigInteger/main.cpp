#include <iostream>
#include "biginteger.h"

using std::cin;
using std::cout;

int main()
{
    int aa, bb;
    BigInteger a, b;
    cin >> aa >> bb;
    a = aa; b = bb;
    
    cout << aa % bb <<  " " << a % b;
    
    return 0;
}
