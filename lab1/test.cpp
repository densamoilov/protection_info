#include <iostream>
#include <omp.h>

#include "cryptolib.h"

int main()
{
    llong a = 2;
    llong d = 1000000006;
    llong m = 1000000007;

    llong res = pow_module(a, d, m);
    std::cout << "res = " << res << std::endl;

    llong a2 = 3;
    llong b = 19;
    llong x;
    llong y;
    llong gcd;
    generalized_euclid(a2, b, gcd, x, y);
    std::cout << "gcd = " << gcd << " x = " << x << " y = " << y << std::endl;
    return 0;
}