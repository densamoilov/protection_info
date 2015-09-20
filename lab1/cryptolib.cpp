#include <cstdio>

typedef long long llong;

llong pow_module(llong a, llong d, llong m)
{
    llong res = 1;
    llong tmp = a;
    // Order of traversal bits from right to left
    for (size_t i = 0; i < sizeof(llong) * 8; ++i) {
        if ((d >> i) & 0x1) {
            res = res * tmp % m;
        }
        tmp = tmp * tmp % m;
    }
    return res;
}

llong generic_euclid(llong a, llong b, llong &gcd, llong &x, llong &y)
{return 0;}