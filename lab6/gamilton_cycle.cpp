#include <iostream>
#include <fstream>
#include <list>
#include <algorithm>
#include <set>
#include <chrono>
#include <thread>

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


class Item_list_edges {
private:
	int a_;
	int b_;
	int value_;
public:
	Item_list_edges(int a, int b, int value = 1)
	: a_(a), b_(b), value_(value)
	{}
	
	Item_list_edges()
	{}

	Item_list_edges operator = (const Item_list_edges& rhs)
	{
		if (this == &rhs) {
			return (*this);
		}
		a_ = this->a_;
		b_ = this->b_;
		value_ = this->value_;

		return (*this);
	}

	int get_first() const
	{
		return a_;
	}

	int get_second() const
	{
		return b_;
	}

	void set_first(int val)
	{
		a_ = val;
	}

	void set_second(int val)
	{
		b_ = val;
	}

	void set_value(int val)
	{
		value_ = val;
	}

	int get_value() const
	{
		return value_;
	}
};

class Graph {
private:
	llong m_; // number of edges
	llong n_; // number of vertex

    list<Item_list_edges> edges_;
public:
	Graph(const char* filename)
	{
		ifstream input_file(filename);

        if (!input_file.is_open()) {
   		    cerr << "Cannot open file: " << filename << endl;
   		    exit(1);
   		}

        int a, b;

        input_file >> n_; // reading number of vertex
        input_file >> m_; // reading number of edges

   
	    for (int i = 0; i < m_; ++i) {
	    	input_file >> a;
	    	input_file >> b;
	    	edges_.push_back(Item_list_edges(a, b));
	    }
	    input_file.close();
	}

	Graph(const list<Item_list_edges>& edges,int m, int n)
	{
		edges_ = edges;
		m_ = m;
		n_ = n;
	}
	
	Graph()
	{}
	
	Graph operator = (const Graph& rhs)
	{
		if (this == &rhs) {
		    return *this;
		}
		
	    this->m_ = rhs.m_;
	    this->n_ = rhs.n_;
	    
	    this->edges_ = rhs.edges_;
	    
	    return *this;
	}

	list<Item_list_edges> get_list_edges()
	{
		return edges_;   
	}

	void show_edges()
	{
		for (const auto& val : edges_) {
  		  	cout << "v = " << val.get_first()  << " : " <<  "u = " << val.get_second() << " value =  " << val.get_value() << endl;
  		}
	}

	int get_n()
	{
		return n_;
	}

	int get_m()
	{
		return m_;
	}
};

class Alice {
private:
	// Closed keys
    llong P_;
    llong Q_;
    llong c_;
    llong fi_;
    // Opened keys
	llong N_;
	llong d_;

	vector<int> gamilton_cycle_;
	vector<int> permutation_;
	vector<int> num_for_concat_;
	
	list<Item_list_edges> H_;
	list<Item_list_edges> F_;

	int m_;
	int n_;
public:
	Alice(const char* filename)
	{
		// Prepare RSA
		P_ = get_prime_number(1, 30000);
        Q_ = get_prime_number(1, 30000);

        N_ = P_ * Q_;
    	fi_ = (P_ - 1) * (Q_ - 1);

    	// Get c and d
	    llong gcd{0}, x{0}, y{0};

	    while (gcd != 1) {
	        d_ = get_prime_number(1, fi_ -1);
	        generalized_euclid(d_, fi_, gcd, x, y);
	    }
	    generalized_euclid(d_, fi_, gcd, x, y);

	    if (x < 0) {
	        c_ = fi_ + x;
	    } else {
	        c_ = x;
	    }

	    // Reading Hamiltonian cycle
		ifstream input_file(filename);

        if (!input_file.is_open()) {
   		    cerr << "Cannot open file: " << filename << endl;
   		    exit(1);
   		}

        int tmp;
        char buf[256];

        input_file >> n_; // reading number of vertex
        input_file >> m_; // reading number of edges

 		// m + 1 because first time getline reading new line symbol
	    for (int i = 0; i < m_ + 1; ++i) {
	    	input_file.getline(buf, 256);
	    }

	    while (input_file >> tmp) {
	    	gamilton_cycle_.push_back(tmp);
	    }
	    input_file.close();

	    // Filling permutation vector
	    for (int i = 0; i < n_; ++i) {
	    	permutation_.push_back(i + 1);
	    }
	}

	llong get_N()
	{
		return N_;
	}

	llong get_d()
	{
		return d_;
	}
    
    list<Item_list_edges> get_F(Graph& G) 
    {
    	H_ = G.get_list_edges();

   		// Generate permutation vector
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    	this_thread::sleep_for(1ms);

    	shuffle(permutation_.begin(), permutation_.end(), default_random_engine(seed));

    	// Perform permutation for list edges
    	for (auto& x : H_) {
    		x.set_first(permutation_[x.get_first() - 1]);
    		x.set_second(permutation_[x.get_second() - 1]);
    	}
    	// Perform permutation for Hamiltonian cycle
    	for (auto& x : gamilton_cycle_) {
    		x = permutation_[x - 1];
    	}

    	default_random_engine gen(seed);
        uniform_int_distribution<llong> random(1000, 50000);
        
        // Concatenate
        for (auto& x : H_) {
        	llong r = random(gen);
        	num_for_concat_.push_back(r);
        	llong tmp = x.get_value();
        	tmp = (r << 1) | tmp;
        	x.set_value(tmp);
        }

        // Encode H
        F_ = H_;

        
        for (auto& x : F_) {
        	llong tmp = x.get_value();
        	tmp = pow_module(tmp, d_, N_);
        	x.set_value(tmp);
        }

        return F_;
    	/*
    	cout << endl;
    	for (const auto& val : H_) {
  		  	cout << "v = " << val.get_first()  << " : " <<  "u = " << val.get_second() << endl;
  		}

  		cout << "Hamiltonian cycle:" << endl;
  		for (const auto& x : gamilton_cycle_) {
  			cout << x << " ";
  		}
  		cout << endl;
  		*/
	}

    void show_gamilton_cycle()
    {
    	for (const llong& val : gamilton_cycle_) {
    		cout << val << " ";
    	}
    	cout << endl;
    }

    void show_permutation()
    {
    	for (const llong& val : permutation_) {
    		cout << val << " ";
    	}
    	cout << endl;
    }
    
    // What is a Hamiltonian cycle for H
    Graph ask_question1()
    {
        // Decode Hamiltonian cycle in graph F
        llong first, second;

        for (int i = 0; i < static_cast<int>(gamilton_cycle_.size() - 1); ++i) {
        	for (auto& val : F_) {
        		first = val.get_first();
        		second = val.get_second();

            	if ( (first == gamilton_cycle_[i] && second == gamilton_cycle_[i + 1]) ||
            	     (second == gamilton_cycle_[i] && first == gamilton_cycle_[i + 1]) ) {
            		llong tmp = val.get_value();
            		tmp = pow_module(tmp, c_, N_);
            		val.set_value(tmp);
            		break;
            	}
        	}
        }
        return Graph(F_, m_, n_);
    }
    
    Graph ask_question2()
    {
    }
    

};


class Bob {
public:

};



int main(int argc, char** argv)
{
    if (argc != 2) {
    	cerr << "Usage: " << argv[0] << " <filename>" << endl;
    	return 1;
    }

    Graph G(argv[1]);
    cout << "G" << endl;
    G.show_edges();

    Alice alice(argv[1]);
    //alice.show_gamilton_cycle();
 
    Graph F(alice.get_F(G), G.get_m(), G.get_n());
       Graph HH = alice.ask_question2();
    cout << "HH" << endl;
    HH.show_edges();

    cout << "F" << endl;
    F.show_edges();
    Graph tmp;
    tmp = alice.ask_question1();
    cout << "tmp" << endl;
    tmp.show_edges();
   
    //G.show_edges();
    //F.show_edges();
 	
    //alice.show_gamilton_cycle();
    //alice.show_permutation(); 
       

   


	return 0;
}
