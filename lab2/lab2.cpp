#include <random>
#include <iostream>

#include "../lab1/cryptolib.h"

typedef long long llong;

int main()
{
/*---------------------- Shamir --------------------
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
--------------------------------------------------------*/

/*--------------------------- El gamal -------------------*/
    llong p{0}, d{0}, m{1234};
    srand(time(0));
    bool isprime = false;
    // Generation a prime number
    while (!isprime) {
        isprime = true;
        p = rand() % 30000;
        // Check for prime
        for (size_t i = 2; i <= static_cast<size_t>(sqrt(p)); ++i) {
            if (p % i == 0) {
                 isprime = false;
                 break;
            }
        }
        // if m is prime, check d
        if (isprime) {
            d = (p - 1) / 2;
            for (size_t i = 2; i <= static_cast<size_t>(sqrt(d)); ++i) {
                if (d % i == 0) {
                    isprime = false;
                    break;
                }
            }
        }
    }
    llong g = 2;
    while (pow_module(g, d, p) == 1) {
        ++g;
    }
    std::default_random_engine genn;
    std::uniform_int_distribution<llong> randomm(2, p - 2);
    // Secret number
    llong Ca = randomm(genn);
    llong Cb = randomm(genn);

    llong Da = pow_module(g, Ca, p);
    llong Db = pow_module(g, Cb, p);

    std::default_random_engine trans;
    std::uniform_int_distribution<llong> randtr(1, p - 2);

    llong k = randtr(trans);

    llong r = pow_module(g, k, p);
    llong e = m * pow_module(Db, k, p);

    llong mrecd = e * pow_module(r, p - 1 - Cb, p);

    std::cout << "Sent message: " << m << std::endl;
    std::cout << "Received message: " << mrecd << std::endl;

    return 0;
}

