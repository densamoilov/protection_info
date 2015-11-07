#include <iostream>
#include <random>
#include <vector>
#include <fstream>
#include <algorithm>
#include <iterator>

#include "../lab1/cryptolib.h"

#include <openssl/md5.h>

using namespace std;

typedef long long llong;

llong get_prime_number(llong lower_bound, llong upper_bound)
{
    static std::default_random_engine gen(time(0));
    static std::uniform_int_distribution<llong> random(lower_bound, upper_bound);
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
    return p;
}

void RSA_signature(string filename)
{
    llong p = get_prime_number(1, 30'000);
    llong q = get_prime_number(1, 30'000);

    llong n = p * q;
    llong fi = (p - 1) * (q - 1);

    // Get c and d
    llong d{0};
    llong gcd{0}, x{0}, y{0}, c{0};

    while (true) {
        d = get_prime_number(1, fi -1);
        generalized_euclid(d, fi, gcd, x, y);
        if (gcd == 1) {
            break;
        }
    }
    generalized_euclid(d, fi, gcd, x, y);

    if (x < 0) {
        c = fi + x;
    } else {
        c = x;
    }
    // Now we have c and d values
    // Reading content of the file to the buffer
    string content;
    ifstream file_in(filename, ios::in);

    if (!file_in.is_open()) {
        cerr << "File not found" << endl;
        exit(1);
    } else {
        while (!file_in.eof()) {
            content.push_back(file_in.get());
        }
    }
    file_in.close();
    // Init MD5
    MD5_CTX context;
    unsigned char ch;
    unsigned char md_buf[MD5_DIGEST_LENGTH];

    MD5_Init(&context);

    // Calculation hash
    for (auto it = content.begin(); it < content.end(); ++it) {
        ch = (*it);
        // 1 byte
        MD5_Update(&context, &ch, 1);
    }
    // Get hash
    MD5_Final(md_buf, &context);

    // Get 16 signatures
    llong s[MD5_DIGEST_LENGTH] = {0};
    for (int i = 0; i < MD5_DIGEST_LENGTH; ++i) {
        s[i] = pow_module(md_buf[i], c, n);
    }

    // Write signatures to file
    ofstream file_sig("signature_rsa", ios::out);

    for (int i = 0; i < MD5_DIGEST_LENGTH; ++i) {
        file_sig << s[i] << endl;
    }
    file_sig.close();

    // Check signature
    unsigned char w[MD5_DIGEST_LENGTH] = {0};
    for (int i = 0; i < MD5_DIGEST_LENGTH; ++i) {
        w[i] = pow_module(s[i], d, n);
    }

    bool is_valid(true);

    for (int i = 0; i < MD5_DIGEST_LENGTH; ++i) {
        if (md_buf[i] != w[i]) {
            is_valid = false;
            break;
        }
    }

    if (is_valid) {
        cout << "Successful" << endl;
    } else {
        cout << "Error" << endl;
    }
}

void el_gamal(string filename)
{
#ifdef _DEBUG
#define MD5_DIGEST_LENGTH 1
#endif

    // Reading content of the file to the buffer
    string content;
    ifstream file_in(filename, ios::in);

    if (!file_in.is_open()) {
        cerr << "File not found" << endl;
        exit(1);
    } else {
        while (!file_in.eof()) {
            content.push_back(file_in.get());
        }
    }
    file_in.close();

    llong p{0}, q{0};
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
        // if m is prime, check q
        if (isprime) {
            q = (p - 1) / 2;
            for (size_t i = 2; i <= static_cast<size_t>(sqrt(q)); ++i) {
                if (q % i == 0) {
                    isprime = false;
                    break;
                }
            }
        }
    }

    llong g = 2;
    while (pow_module(g, q, p) == 1) {
        ++g;
    }
    // Now we have q, g and p values
#ifdef _DEBUG
p = 23;
q = 11;
g = 5;
#endif

    default_random_engine gen_cl_key;
    uniform_int_distribution<llong> random_cl_key(2, p - 2);

    // Generation closed key
    llong x = random_cl_key(gen_cl_key);

    // Generation opened key
    llong y = pow_module(g, x, p);

#ifdef _DEBUG
x = 7;
y = 17;
#endif

    // Init MD5
    MD5_CTX context;
    unsigned char md_buf[MD5_DIGEST_LENGTH];

    MD5_Init(&context);

    // Computing hash
    for_each(content.begin(), content.end(), [&](char ch){
        MD5_Update(&context, &ch, 1);
    });

    // Get hash
    MD5_Final(md_buf, &context);

    // Hash value must be between 1 and p (1 < h < p)
    for_each(md_buf, md_buf + MD5_DIGEST_LENGTH, [=](unsigned char& ch) {
        ch = ch % p + 1;
    });

    // Get k
    llong k{0};
    llong gcd{0}, x_e{0}, y_e{0};

    while (true) {
        k = get_prime_number(2, p - 2);
        generalized_euclid(k, p - 1, gcd, x_e, y_e);
        if (gcd == 1) {
            break;
        }
    }
#ifdef _DEBUG
md_buf[0] = 3;
k = 5;
#endif
    // Get r
    llong r = pow_module(g, k, p);
    // to do: page 56 - computing u and s
    llong u[MD5_DIGEST_LENGTH] = {0};
    for (int i = 0; i < MD5_DIGEST_LENGTH; ++i) {
        u[i] = (md_buf[i] - x * r) % (p - 1);

        while (u[i] < 0) {
            u[i] += p - 1;
        }
    }

#ifdef _DEBUG
r = 20;
#endif

    // Computing k^-1
    llong k_inv{0};
    generalized_euclid(p - 1, k, gcd, x_e, y_e);
    if (y_e < 0) {
        k_inv = (p - 1) + y_e;
    } else {
        k_inv = y_e;
    }

#ifdef _DEBUG
k_inv = 9;
#endif
    // Get signatures
    llong s[MD5_DIGEST_LENGTH] = {0};
    for (int i = 0; i < MD5_DIGEST_LENGTH; ++i) {
        s[i] = (k_inv * u[i]) % (p - 1);
    }

    // Write signatures to file
    ofstream file_sig_out("signature_elgamal", ios::out);

    file_sig_out << r << endl;

    for_each(s, s + MD5_DIGEST_LENGTH, [&](const llong& val) {
        file_sig_out << val << endl;
    });
    file_sig_out.close();

    // Check signature
    llong r_read{0};
    llong s_read[MD5_DIGEST_LENGTH];

    ifstream file_sig_in("signature_elgamal", ios::in);
    if (!file_sig_in.is_open()) {
        cerr << "File ""signature_elgamal"" not found!" << endl;
        exit(1);
    } else {
        file_sig_in >> r_read;

        for (int i = 0; file_sig_in; ++i) {
            file_sig_in >> s_read[i];
        }
    }
    file_sig_in.close();

    // y^r * r^s = g^h mod p
    llong yr = pow_module(y, r_read, p);
    llong rs[MD5_DIGEST_LENGTH] = {0};
    llong gh[MD5_DIGEST_LENGTH] = {0};

    for (int i = 0; i < MD5_DIGEST_LENGTH; ++i) {
        rs[i] = pow_module(r_read, s_read[i], p);
        gh[i] = pow_module(g, md_buf[i], p);
    }

    llong lhs[MD5_DIGEST_LENGTH] = {0};
    for (int i = 0; i < MD5_DIGEST_LENGTH; ++i) {
       lhs[i] = (yr * rs[i]) % p;
    }

    bool is_valid = true;
    for (int i = 0; i < MD5_DIGEST_LENGTH; ++i) {
        if (lhs[i] != gh[i]) {
            is_valid = false;
            //break;
        }
    }

    if (is_valid) {
        cout << "Successful" << endl;
    } else {
        cout << "Error" << endl;
    }
}

void gost(string filename)
{
    // Reading content of the file to the buffer
    string content;
    ifstream file_in(filename, ios::in);

    if (!file_in.is_open()) {
        cerr << "File not found" << endl;
        exit(1);
    } else {
        while (!file_in.eof()) {
            content.push_back(file_in.get());
        }
    }
    file_in.close();


    llong q = get_prime_number(100000, 999999);

    llong b = []() -> llong {
        default_random_engine gen(time(0));
        uniform_int_distribution<llong> random(100, 999);
        return random(gen);
    }();

    auto lambda_check_for_prime = [](long p) -> bool {
        bool is_prime{true};
        // Check for prime
        for (size_t i = 2; i <= static_cast<size_t>(sqrt(p)); ++i) {
            if (p % i == 0) {
                 is_prime = false;
                 break;
            }
        }
        return is_prime;
    };

    llong  p{4};
    while (!lambda_check_for_prime(p)) {
        p = (b++) * q + 1;
    }

    // Computing a
    llong g{0}, a{0};
    while (true) {
        g = get_prime_number(2, p);
        a = pow_module(g, b, p);
        if (pow_module(a, q, p) == 1) {
            break;
        }
    }
    /*** Now we have p q b and a values ***/

    // Computing x - closed key
    llong x = [=]() -> llong {
        default_random_engine gen(time(0));
        uniform_int_distribution<llong> random(1, q - 1);
        return random(gen);
    }();

    // Computing y - opened key
    llong y = pow_module(a, x, p);

    /*** Now all needed parameters identified ***/

    // Init MD5
    MD5_CTX context;
    unsigned char md_buf[MD5_DIGEST_LENGTH];

    MD5_Init(&context);

    // Computing hash
    for_each(content.begin(), content.end(), [&](char ch){
        MD5_Update(&context, &ch, 1);
    });

    // Get hash
    MD5_Final(md_buf, &context);

    // Hash value must be between 0 and q (0 < h < q)
    for_each(md_buf, md_buf + MD5_DIGEST_LENGTH, [=](unsigned char& ch) {
        ch = ch % q;
    });

    // Computing r, s and k
    default_random_engine gen(time(0));
    uniform_int_distribution<llong> random(1, q - 1);

    llong r{-1}, k{0}, s[MD5_DIGEST_LENGTH] = {-1};
    bool is_snegative{true};

    // If r or s < 0 - computing r and s again
    while (r < 0 && is_snegative) {
        is_snegative = false;
        k = random(gen);
        r = pow_module(a, k, p) % q;
        for ( int i = 0; i < MD5_DIGEST_LENGTH; ++i) {
            s[i] = ( (k * md_buf[i]) % q + (x * r) % q ) % q;
            if (s[i] < 0) {
               is_snegative = true;
            }
        }
    }

    // Writing signatures to file
    ofstream file_sig_out("signature_gost", ios::out);

    file_sig_out << r << endl;

    for_each(s, s + MD5_DIGEST_LENGTH, [&](const llong& val) {
        file_sig_out << val << endl;
    });
    file_sig_out.close();

    // Reading signatures from the file
    llong r_read{0};
    llong s_read[MD5_DIGEST_LENGTH];

    ifstream file_sig_in("signature_gost", ios::in);
    if (!file_sig_in.is_open()) {
        cerr << "File ""signature_gost"" not found!" << endl;
        exit(1);
    } else {
        file_sig_in >> r_read;

        for (int i = 0; file_sig_in; ++i) {
            file_sig_in >> s_read[i];
        }
    }
    file_sig_in.close();

    // Check signature
    if ( (r_read < 0 || r_read > q) ) {
        cerr << "Error" << endl;
        exit(1);
    }

    for (int i = 0; i < MD5_DIGEST_LENGTH; ++i) {
        if (s[i] < 0 || s[i] > q) {
            cerr << "Error" << endl;
            exit(1);
        }
    }
    llong gcd{0}, x_e{0}, y_e{0};
    llong h_inv[MD5_DIGEST_LENGTH];

    // Computing h_inv
    for (int i = 0; i < MD5_DIGEST_LENGTH; ++i) {
        generalized_euclid(q, md_buf[i], gcd, x_e, y_e);
        if (y_e < 0) {
            h_inv[i] = q + y_e;
        } else {
           h_inv[i] = y_e;
        }
    }
    // Computing u1 u2
    llong u1[MD5_DIGEST_LENGTH] = {0};
    llong u2[MD5_DIGEST_LENGTH] = {0};
    for (int i = 0; i < MD5_DIGEST_LENGTH; i++) {
        u1[i] = (s[i] * h_inv[i]) % q;
        u2[i] = (-r * h_inv[i]) % q;
    }

    for (int i = 0; i < MD5_DIGEST_LENGTH; i++) {
        while (u2[i] < 0) {
            u2[i] += q;
        }
    }

    // Computing v
    llong v[MD5_DIGEST_LENGTH] = {0};
    for (int i = 0; i < MD5_DIGEST_LENGTH; i++) {
        v[i] = ((pow_module(a, u1[i], p) * pow_module(y, u2[i], p)) % p) % q;
    }

    // Verify v = r
    for (int i = 0; i < MD5_DIGEST_LENGTH; i++) {
        if (r_read != v[i]) {
            cerr << "Error" << endl;
            exit(1);
        }
    }
    cout << "Successful" << endl;
}

int main()
{
    RSA_signature("aaa");
    el_gamal("aaa");
    gost("aaa");

    return 0;
}
