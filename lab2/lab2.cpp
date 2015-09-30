#include <random>
#include <iostream>

#include "../lab1/cryptolib.h"

typedef long long llong;

int main()
{
    llong m = 1234; // message

    std::default_random_engine gen;
    std::uniform_int_distribution<llong> random(100, 50000);

    bool is_prime = false;

    // Generation a prime number
    llong p{0};

    while (!is_prime) {
        is_prime = true;
        p = random(gen);
        // Check for prime
        for (size_t i = 2; i <= static_cast<size_t>(sqrt(p)); ++i) {
            if (p % i == 0) {
                 is_prime = false;
                 break;
            }
        }
    }

    llong Ca = random(gen);
    llong Da = random(gen);

    for (size_t i = 0; (Ca * Da) % (p - 1) != 1; ++i) {
        if (i % 2 == 0) {
            ++Ca;
        } else {
            ++Da;
        }
    }

    llong Cb = random(gen);
    llong Db = random(gen);

    for (size_t i = 0; (Cb * Db) % (p - 1) != 1; ++i) {
        if (i % 2 == 0) {
            ++Cb;
        } else {
            ++Db;
        }
    }
    llong x1, x2, x3, x4;

    x1 = pow_module(m, Ca, p);
    x2 = pow_module(x1, Cb, p);
    x3 = pow_module(x2, Da, p);
    x4 = pow_module(x3, Db, p);


    std::cout << "x1 = " << x1 << std::endl;
    std::cout << "x2 = " << x2 << std::endl;
    std::cout << "x3 = " << x3 << std::endl;
    std::cout << "x4 = " << x4 << std::endl;
    std::cout << "m = " << m << std::endl;

    return 0;
}

