#include <iostream>
#include <omp.h>

#include "cryptolib.h"

int main()
{
    llong a = 2;
    llong d = 1000000006;
    llong m = 1000000007;

    llong res = pow_module(a, d, m);
    std::cout << res << std::endl;

    return 0;
}