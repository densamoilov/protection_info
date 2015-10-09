#include <random>
#include <iostream>
#include <vector>

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
TODO: Euclid and check using Ca*Da % (p - 1) == 1
Ca and Da must be relatievely prime
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

/*--------------------------- El gamal -------------------*
    llong p{0}, d{0}, m{1234};
    srand(time(0));
    bool isprime = false;
    // Generation a prime number
    while (!isprime) {
        isprime = true;
        p = rand() % 30000 + 5000;
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

    std::default_random_engine gen_cl_key;
    std::uniform_int_distribution<llong> random_cl_key(2, p-2);
    // Closed keys
    llong Cb = random_cl_key(gen_cl_key);
    // Opened keys
    llong Db = pow_module(g, Cb, p);

    std::default_random_engine gen_tr;
    std::uniform_int_distribution<llong> rand_tr(1, p - 2);

    llong k = rand_tr(gen_tr);

    // Send message to B from A
    llong r = pow_module(g, k, p);
    llong e = m * pow_module(Db, k, p) % p;

    llong mrecd = e * pow_module(r, p - 1 - Cb, p) % p;

    std::cout << "Sent message: " << m << std::endl;
    std::cout << "Received message: " << mrecd << std::endl;
-----------------------------------------------------------------*/

/*--------------------------Vernama ---------------------------

    llong size = 100;

    std::default_random_engine gen;
    std::uniform_int_distribution<llong> random(0, 255);

    std::vector<llong> keys;
    std::vector<llong> e;
    std::vector<llong> m;
    std::vector<llong> m_;

    keys.reserve(size);
    e.reserve(size);
    m.reserve(size);
    m_.reserve(size);

    // Gen m
    for (size_t i = 0 ; i < size; ++i) {
        m.push_back(i);
    }

    // Create keys
    for (size_t i = 0; i < size; ++i) {
        keys.push_back(random(gen));
    }
    // Encode
    for (size_t i = 0; i < size; ++i) {
        e[i] = m[i] ^ keys[i];
    }
    // Decode
    for (size_t i = 0; i < size; ++i) {
        m_.push_back(e[i] ^ keys[i]);
    }
    // Validate
    if (m_ == m) {
        std::cout << "Allright" << std::endl;
    } else {
        std::cout << "Error" << std::endl;
    }
------------------------------------------------------------*/

/*-------------------------- RSA ---------------------------*/




    return 0;
}

