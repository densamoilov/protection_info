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

llong generalized_euclid(llong a, llong b, llong &gcd, llong &x, llong &y)
{
    llong u[3] = {a, 1, 0};
    llong v[3] = {b, 0, 1};
    llong tmp[3] = {0};
    llong q{0};

    while (v[0]) {
        q = u[0] / v[0];
        tmp[0] = u[0] % v[0];
        tmp[1] = u[1] - q * v[1];
        tmp[2] = u[2] - q * v[2];

        // Assignment: U <-- V; V <-- T
        u[0] = v[0];
        u[1] = v[1];
        u[2] = v[2];

        v[0] = tmp[0];
        v[1] = tmp[1];
        v[2] = tmp[2];
    }
    // Get result
    gcd = u[0];
    x = u[1];
    y = u[2];

    return 0;
}