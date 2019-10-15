#include <iostream>
#include <complex>
#include <vector>
#include <algorithm>
#include <math.h>

using cld = std::complex <long double>;
const long double PI = acosl(-1.0);

inline size_t findUpperDegreeOfTwo( size_t v )
{
    size_t n = 1;
    while ( n < v ) {
        n *= 2;
    }
    return n;
}

std::vector <cld> makeComplexVector( const std::vector<int> & a, size_t n )
{
    std::vector <cld> ca( n, cld( 0.0, 0.0 ) );
    for ( size_t i = 0; i < a.size(); i++ ) {
        ca[i] = cld( a[i], 0.0 );
    }
    return ca;
}

std::vector <cld> makeGeneralFFT(std::vector <cld> a, cld q)
{
    if (a.size() == 1)
        return a;
    std::vector <cld> a0, a1;
    for (size_t i = 0; i < a.size(); i += 2) {
        a0.push_back(a[i]);
        a1.push_back(a[i + 1]);
    }
    
    a0 = makeGeneralFFT(a0, q * q);
    a1 = makeGeneralFFT(a1, q * q);

    cld w (1.0, 0.0);
    
    for (size_t i = 0; i < a.size() / 2; i++) {
        cld u = a0[i];
        cld v = w * a1[i];
        a[i] = u + v;
        a[i + a.size() / 2] = u - v;
        w *= q;
    }
    
    return a;
}

std::vector <cld> makeFFT(const std::vector <cld> & a)
{
    long double ang = 2.0 * PI / a.size();
    return makeGeneralFFT(a, cld(std::cosl(ang), std::sinl(ang)));
}

std::vector <cld> makeInverseFFT(std::vector <cld> a)
{
    long double ang = 2.0 * PI / a.size();
    a = makeGeneralFFT(a, cld(std::cosl(ang), -std::sinl(ang)));
    for (size_t i = 0; i < a.size(); i++) {
        a[i] /= a.size();
    }
    return a;
}

std::vector <int> makeIntVector(const std::vector<cld> & a)
{
    std::vector <int> ans(a.size());
    for (size_t i = 0; i < a.size(); i++) {
        ans[i] = floorl(a[i].real() + 0.5);
    }
    return ans;
}

std::vector <int>  multiplicatePolynoms( const std::vector <int> & a, const std::vector <int> & b )
{
    size_t n = findUpperDegreeOfTwo( std::max(a.size(), b.size()) );
    n *= 2;
    
    std::vector <cld> ca = makeComplexVector(a, n);
    std::vector <cld> cb = makeComplexVector(b, n);
    
    ca = makeFFT(ca);
    cb = makeFFT(cb);
    
    for ( size_t i = 0; i < n; i++ ) {
        ca[i] *= cb[i];
    }
    
    std::vector <cld> cc = makeInverseFFT(ca);
    
    return makeIntVector(cc);
}

std::vector <int> readVector()
{
    int n;
    std::cin >> n;
    std::vector <int> a(n);
    for (int i = 0; i < n; i++) {
        std::cin >> a[i];
    }
    return a;
}

int main() {
    
    std::vector <int> a = readVector();
    std::vector <int> b = readVector();
    
    std::vector <int> c = multiplicatePolynoms(a, b);
    
    for (size_t i = 0; i < c.size(); i++) {
        std::cout << c[i] << ' ';
    }
    
    return 0;
}
