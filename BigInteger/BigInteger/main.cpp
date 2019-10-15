#include <iostream>
#include "biginteger.h"


void solve()
{
    for (int i = 0; i < 10000; i++) {
        int a = rand() % (256 * 256);
        a *= (rand() % 2 == 0 ? 1 : -1);
        int b = rand() % (256 * 256);
        b *= (rand() % 2 == 0 ? 1 : -1);
        BigInteger x(a), y(b), c;
        c = a + b;
        if (a + b != (c.sign == 0 ? 1 : -1) * ((int)c.data[0] + (int)c.data[1] * 256 + (int)c.data[2] * 256 * 256 + (int)c.data[3] * 256 * 256 * 256)) {
            std::cout << a << " " << b << "\n";
        }
    }
}


int main()
{
//    solve();
    BigInteger a(1), b(-3), d(-4), c;
    c = a - b + d;
    std::cout << (int)c.sign << '\n';
    std::cout << (int)c.data[0] << " " << (int)c.data[1] << "\n\n";
    
    return 0;
}
