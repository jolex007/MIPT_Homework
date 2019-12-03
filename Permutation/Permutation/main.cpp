#include "permutation.h"
#include <iostream>

using std::cin;
using std::cout;

int main() {
    
    int n;
    
    cin >> n;
    
    int * ar1;
    int * ar2;
    ar1 = new int[n];
    ar2 = new int[n];
    
    for (int i = 0; i < n; i++) {
        cin >> ar1[i];
    }
    
    for (int i = 0; i < n; i++) {
        cin >> ar2[i];
    }
    
    Permutation x(n, ar1), z;
    
    for (int i = 0; i < n; i++) {
        cout << x[i] << " ";
    }
    cout << '\n';
    
    z = x;
//    for (int i = 0; i < n; i++) {
//        cout << y.data[i] << " ";
//    }
//    cout << '\n';
    
    for (int i = 0; i < 121; i++) {
    
        for (int i = 0; i < n; i++) {
            cout << z[i] << " ";
        }
        z++;
        cout << "\n";
    }
    
    return 0;
}
