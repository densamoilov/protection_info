#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

#include "../lab1/cryptolib.h"

using namespace std;

typedef long long llong;


llong get_prime_number(llong lower_bound, llong upper_bound)
{
    static default_random_engine gen(time(0));
    static uniform_int_distribution<llong> random(lower_bound, upper_bound);
    bool is_prime{false};

    // Generation a prime number
    llong p{0};

    while (!is_prime) {
        is_prime = true;
        p = random(gen);
        // Check for prime
        for (llong i = 2; i <= static_cast<llong>(sqrt(p)); ++i) {
            if (p % i == 0) {
                 is_prime = false;
                 break;
            }
        }
    }
    return p;
}

llong get_random_number(llong lower_bound, llong upper_bound)
{
	static default_random_engine gen(time(0));
    static uniform_int_distribution<llong> random;

    return (random(gen));
}


class Bank {
private:
    const llong nbanknotes_;

    llong p_;
    llong q_;
    vector<llong> c_;

public:
    llong n;
    vector<llong> d;

public:
	Bank()
	: nbanknotes_{9}, p_{0}, q_{0}, c_(nbanknotes_), d(nbanknotes_)
	{
        // Generate p and q
        while (p_ == q_) {
     	    p_ = get_prime_number(1, 30000);
     	    q_ = get_prime_number(1, 30000);
        }

        n = p_ * q_;

        llong fi = (p_ - 1) * (q_ - 1);
 
        llong gcd{0}, x{0}, y{0};
        
        for (int i = 0; i < nbanknotes_; ++i) {
            while (gcd != 1) {
                d[i] = get_prime_number(1, fi - 1);
                generalized_euclid(d[i], fi, gcd, x, y);
            }
            if (x < 0) {
                c_[i] = fi + x;
            } else {
                c_[i] = x;
            }
            gcd = 0;


		    if (((c_[i] * d[i]) % fi) != 1) {
		        cerr << "Invalid numbers d or c" << endl;
		    }  else {
		        cout << "All seems to be fine" << endl;
		    }
        }
    }
};

int main()
{
    Bank bank;


	return 0;
}