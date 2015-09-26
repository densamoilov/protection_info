#include <cstdio>
#include <map>
#include <cmath>
#include <iostream>

typedef long long llong;

llong pow_module(llong a, llong d, llong m)
{
    llong res = 1;
    // Order of traversal bits from right to left
    for (size_t i = 0; i < sizeof(llong) * 8; ++i) {
        if ((d >> i) & 0x1) {
            res = res * a % m;
        }
        a = a * a % m;
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

llong diffie_hellman(llong d, llong &Zab, llong &Zba)
{
    llong m{0};
    srand(time(0));
    bool isprime = false;
    // Generation a prime number
    while (!isprime) {
        isprime = true;
        m = rand() % 30000;
        // Check for prime
        for (size_t i = 2; i <= static_cast<size_t>(sqrt(m)); ++i) {
            if (m % i == 0) {
                 isprime = false;
                 break;
            }
        }
        // if m is prime, check d
        if (isprime) {
            d = (m - 1) / 2;
            for (size_t i = 2; i <= static_cast<size_t>(sqrt(d)); ++i) {
                if (d % i == 0) {
                    isprime = false;
                    break;
                }
            }
        }
    }
    llong g = 2;
    while (pow_module(g, d, m) == 1) {
        ++g;
    }

    // Closed key
    llong Xa = rand() % (m - 1) + 2;
    llong Xb = rand() % (m - 1) + 2;
    // Opened key
    llong Ya = pow_module(g, Xa, m);
    llong Yb = pow_module(g, Xb, m);

    Zab = pow_module(Yb, Xa, m);
    Zba = pow_module(Ya, Xb, m);

    return 0;
}

llong baby_step_giant_step(llong a, llong m, llong y, llong &x)
{
    llong k1, k2;
    std::map<llong, llong>::iterator it;

    k1 = k2 = sqrt(m) + 1;

    std::map<llong, llong> tmp;

    for (int i = 1; i <= k1; ++i) {
        tmp.insert(std::pair<llong, llong>(pow_module(a, i*k1, m), i));
    }

    for (int j = 0; j < k2; ++j) {
       llong tmploc = pow_module(y, 1, m) * pow_module(a, j, m) % m;
       if ((it = tmp.find(tmploc)) != tmp.end()) {
           x = a * it->second - j;
           break;
       }
    }
    return 0;
}
