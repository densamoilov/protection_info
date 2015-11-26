#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <set>

#include "../lab1/cryptolib.h"

using namespace std;

typedef long long llong;

llong get_prime_number(llong lower_bound, llong upper_bound)
{
    static set<llong> used_numbers;

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
        if (is_prime) {
            if (used_numbers.count(p) == 1)
                is_prime = false;
            else
                used_numbers.insert(p);
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
    const llong nnominals_;

    llong p_;
    llong q_;
    vector<llong> c_;

public:
    llong N;
    vector<llong> d;

public:
	Bank()
	: nnominals_{9}, p_{0}, q_{0}, c_(nnominals_, 0), d(nnominals_, 0)
	{
        // Generate p and q
     	p_ = get_prime_number(1, 30000);
     	q_ = get_prime_number(1, 30000);

        N = p_ * q_;

        llong fi = (p_ - 1) * (q_ - 1);
 
        llong gcd{0}, x{0}, y{0};
        
        for (int i = 0; i < nnominals_; ++i) {
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
/*

		    if (((c_[i] * d[i]) % fi) != 1) {
		        cerr << "Invalid numbers d or c" << endl;
		    }  else {
		        cout << "All seems to be fine" << endl;
		    }
*/
        }
    }

    void show_parametres()
    {
        string nominals[9] = {"1", "2", "5", "10", "50", "100", "500", "1000", "5000"};
                cout << "---------------------- Bank info -------------------------------" << endl;
        for (int i = 0; i < nnominals_; ++i) {
            cout << nominals[i] << " c = " << c_[i] << ":" << " d = " << d[i] << endl;
        }

        cout << endl << "N = " << N << " p = " << p_ << " q = " << q_ << endl << endl;
        cout << "---------------------------------------------------------------" << endl;
    }

    // Providing info for bank customer
    vector<llong> get_d()
    {
        return d;
    }

    llong get_n()
    {
        return N;
    }

    llong authentication_request_client(llong hh, llong num_banknote)
    {
        return pow_module(hh, c_[num_banknote], N);
    }

    bool authentication_request_company(llong n, llong s)
    {
        cout << "\nRequest company" << endl; 
        hash<llong> hash_cpp;

        llong h = hash_cpp(n);
        h %= N;
        cout << " h = " << h << endl;// " n = " << n << endl;
        
        bool is_equal{false};
        llong tmp{0};
        for (int i = 0; i < 9; ++i) {
            tmp = pow_module(s, d[i], N);
            if (h == tmp) {
                is_equal = true;
                break;
            } else {
                is_equal = false;
            }
        }
        if (is_equal) {
        	cout << " tmp (" << tmp << ") " << "equal " << h << endl; 
        	return true;
        } else {
     		return false;
        }
        //return (is_equal) ? true : false;
    }
};

class Company {
public:
    bool verify_info(llong n, llong s, Bank& bank)
    {
        return (bank.authentication_request_company(n, s)) ? true : false;
    }
};


class Client {
private:
    // Money for buy
    llong money_;
    // Info for bank customer
    llong N_;
    vector<llong> d_;
public:
    Client(Bank& bank, llong money)
    : money_(money)
    {
        d_ = bank.get_d();
        N_ = bank.get_n();
    }

    void perform_transaction(Bank& bank, llong num_banknote, Company& company)
    {

    	llong nominals[9] = {1, 2, 5, 10, 50, 100, 500, 1000, 5000};   // for output info

        static default_random_engine gen(time(0));
        static uniform_int_distribution<llong> random(1, N_ - 1);
        static set<llong> used_numbers;
        hash<llong> hash_cpp;
        
        llong n{0};
        while (true) {
            n = random(gen);
            if (used_numbers.count(n) != 1) {
            	used_numbers.insert(n);
                break;
            }
        }


        llong h = hash_cpp(n);
        h %= N_;

        //cout << " n = " << n << " for " << nominals[num_banknote];
        cout << " h = " << h << " for " << nominals[num_banknote] << endl;

        llong gcd{0}, x{0}, y{0}, r{0};

        while (gcd != 1) {
            r = get_prime_number(1, N_ - 1); // replace on simple get_rand_number w/ check for repeat
            generalized_euclid(r, N_, gcd, x, y);
        }
        llong hh = (h * (pow_module(r, d_[num_banknote], N_))) % N_;
        llong ss = bank.authentication_request_client(hh, num_banknote);

        cout << " r = " << r << " for " << nominals[num_banknote] << endl;
        cout << " hh = " << hh << " for " << nominals[num_banknote] << endl;
        cout << " ss (request_client) = " << ss << " for " << nominals[num_banknote] << endl;

        generalized_euclid(r, N_, gcd, x, y);
        if (x < 0) {
            r = N_ + x;
        } else {
            r = x;
        }

        llong s = (ss * r) % N_;
        cout << " s = " << s << " for " << nominals[num_banknote] << endl;

        // to do verify in shop
        if (company.verify_info(n, s, bank)) {
            cout << "\nTransaction is passed" << endl;
            cout << "-------------------------------------------------------------" << endl;
        } else {
            cout << "\nTransaction is not passed" << endl;
            cout << "-------------------------------------------------------------" << endl;
        }
    }

    void make_purchase(Bank& bank, Company& company)
    {
        llong nominals[9] = {1, 2, 5, 10, 50, 100, 500, 1000, 5000};  
        llong m = money_;
        llong tmp_m = 0;

        for (int i = 0; i < 9; ++i) {
            if (m < nominals[i] || i == 8) {
                --i;
                while (tmp_m != m) {
                    if (tmp_m + nominals[i] <= m) {
                        // Perform transaction
                        cout << "--Perform transaction " << nominals[i] << endl;
                        perform_transaction(bank, i, company); // to do
                        tmp_m += nominals[i];
                    } else {
                        --i;
                    }
                    cout << endl;
                }
                break;
            }
        }
    } 
};



int main()
{
    Bank bank;
    Company company;
    Client client(bank, 340);

    bank.show_parametres();
    
    client.make_purchase(bank, company);


	return 0;
}