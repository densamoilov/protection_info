#include <random>
#include <iostream>
#include <vector>
#include <fstream>


#include "../lab1/cryptolib.h"

typedef long long llong;

llong get_prime_number(llong lower_bound, llong upper_bound)
{
    static std::default_random_engine gen;
    static std::uniform_int_distribution<llong> random(100, upper_bound);
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
    std::cout << p << std::endl;
    return p;
}


void shamir()
{
    llong m = 1234; // message

    std::default_random_engine gen;
    std::uniform_int_distribution<llong> random(100, 50'000);
    llong p = get_prime_number(1, 30'000);

    llong Ca, Da;

    while (true) {
        llong gcd{0}, x{0}, y{0};
        Ca = get_prime_number(1, 30'000);
        generalized_euclid(Ca, p - 1, gcd, x, y);
        if (gcd == 1) {
            break;
        }
    }
    // reuse
    llong gcd{0}, x{0}, y{0};
    generalized_euclid(Ca, p - 1, gcd, x, y);

    if (x < 0) {
        Da = p - 1 + x;
    } else {
        Da = x;
    }


    llong Cb, Db;

    while (true) {
        llong gcd{0}, x{0}, y{0};
        Cb = get_prime_number(1, 30'000);
        generalized_euclid(Cb, p - 1, gcd, x, y);
        if (gcd == 1) {
            break;
        }
    }
    generalized_euclid(Cb, p - 1, gcd, x, y);

    if (x < 0) {
        Db = p - 1 + x;
    } else {
        Db = x;
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
}


void el_gamal()
{
    std::ifstream msg("t", std::ios::binary);

    if (!msg) {
        std::cerr << "File not found" << std::endl;
        exit(1);
    }


    llong p{0}, d{0};
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
    std::uniform_int_distribution<llong> random_cl_key(2, p - 2);
    // Closed keys
    llong Cb = random_cl_key(gen_cl_key);
    // Opened keys
    llong Db = pow_module(g, Cb, p);

    std::default_random_engine gen_tr;
    std::uniform_int_distribution<llong> rand_tr(1, p - 2);

    std::ofstream out("el_gamal_encode", std::ios::binary | std::ios::out);

    char buf;
    // Encode
    while (msg.read(&buf, sizeof(char))) {
        llong k = rand_tr(gen_tr);
        // Send message to B from A
        llong r = pow_module(g, k, p);
        llong e = (buf * (pow_module(Db, k, p))) % p;

        out.write((char*)&e, sizeof(llong));
        out.write((char*)&r, sizeof(llong));
    }
    msg.close();
    out.close();

    // Decode
    llong e{0}, r{0};

    std::ofstream decmsg("el_gamal_decode", std::ios::binary | std::ios::out);
    std::ifstream tmp("el_gamal_encode", std::ios::binary | std::ios::in);
    while (tmp.read((char*)&e, sizeof(llong))) {
        tmp.read((char*)&r, sizeof(llong));
        unsigned short mrecd = (e * (pow_module(r, p - 1 - Cb, p))) % p;
        decmsg.write((char*)&mrecd, sizeof(char));
    }
    decmsg.close();
    tmp.close();
}

void vernama()
{

    std::ifstream msg("t", std::ios::binary | std::ios::in);
    if (!msg.is_open()) {
        std::cerr << "File not found" << std::endl;
    }

    // Get size of file in chars
    msg.seekg (0, std::ios_base::end);
    int size = msg.tellg();
    msg.seekg (0, std::ios_base::beg);

    std::default_random_engine gen;
    std::uniform_int_distribution<llong> random(0, 255);

    // Create keys
    std::ofstream keys_out("vernama_keys", std::ios::binary | std::ios::out);
    for (size_t i = 0; i < static_cast<size_t>(size); ++i) {
        llong tmp = random(gen);
        keys_out.write((char*)&tmp, sizeof(llong));
    }
    keys_out.close();

    llong tmp;
    llong buf;
    std::ofstream out("vernama_encode", std::ios::binary | std::ios::out);
    std::ifstream keys_in("vernama_keys", std::ios::binary | std::ios::in);

    for (int i = 0; msg.read((char*)&buf, sizeof(char)); ++i) {
        llong key;
        keys_in.read((char*)&key, sizeof(llong));
        tmp = buf ^ key;
        out.write((char*)&tmp, sizeof(llong));
    }
    msg.close();
    out.close();
    // Move pointer in begin of file
    keys_in.seekg(0, std::ios::beg);

    std::ofstream decmsg("vernama_decode", std::ios::binary | std::ios::out);
    std::ifstream tmp2("vernama_encode", std::ios::binary | std::ios::in);
    if (!tmp2.is_open()) {
        std::cerr << "File with name ""vernama_encode"" not found" << std::endl;
        exit(1);
    }

    for (int i = 0; tmp2.read((char*)&buf, sizeof(llong)); ++i) {
        llong key;
        keys_in.read((char*)&key, sizeof(llong));
        tmp = buf ^ key;
        decmsg.write((char*)&tmp, sizeof(char));
    }
    keys_in.close();
    decmsg.close();
    tmp2.close();
}

//TODO: Impement RSA
void RSA()
{}

int main()
{
    //shamir();
    //el_gamal();
    vernama();

    return 0;
}

