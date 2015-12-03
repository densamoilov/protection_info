#include <iostream>
#include <fstream>
#include <list>
#include <algorithm>
#include <set>
#include <chrono>
#include <thread>

#include "../lab1/cryptolib.h"

using namespace std;

// 2^-t

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
public:
	Item_list_edges(int a, int b)
	: a_(a), b_(b)
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

		return (*this);
	}

	bool operator == (const Item_list_edges& rhs)
	{
		if (a_ != rhs.get_first() || b_ != rhs.get_second()) {
			return false;
		}
		return true;
	}

	bool operator != (Item_list_edges& rhs)
	{
		if (a_ == rhs.get_first() || b_ == rhs.get_second()) {
			return false;
		}
		return true;
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

	Graph(const list<Item_list_edges>& edges, int m, int n)
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

	bool operator == (Graph& rhs)
	{
		list<Item_list_edges> tmp;
		tmp = rhs.get_list_edges();

		if (edges_.size() != tmp.size()) {
			return false;
		}

		auto it1 = edges_.begin();
		auto it2 = tmp.begin();

		for ( ; it1 != edges_.end(); it1++, it2++) {
			
			if ((*it1) != (*it2)) {
				return (false);
			}
		}
		return (true);
	}

	list<Item_list_edges> get_list_edges()
	{
		return edges_;   
	}

	void show_edges()
	{
		//cout << endl;
		for (const auto& val : edges_) {
  		  	cout << "v = " << val.get_first()  << " : " <<  "u = " << val.get_second() << endl;
  		}
  		cout << endl;
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
    llong fi_;
    // Opened keys
	llong N_;

	vector<int> gamilton_cycle_;
	vector<int> permutation_;
	vector<int> num_for_concat_;
	
	list<Item_list_edges> H_;
	list<Item_list_edges> F_;

	vector<llong> d_;
	vector<llong> c_;

	int m_;
	int n_;
public:
	Alice(const char* filename)
	{
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

		// Prepare RSA
		P_ = get_prime_number(1, 30000);
        Q_ = get_prime_number(1, 30000);

        N_ = P_ * Q_;
    	fi_ = (P_ - 1) * (Q_ - 1);

    	// Get c and d
	    llong gcd{0}, x{0}, y{0};
	    llong tmp_d{0};

	    for (int i = 0; i < m_; ++i) {
		    while (gcd != 1) {
		        tmp_d = get_prime_number(1, fi_ -1);
		        generalized_euclid(tmp_d, fi_, gcd, x, y);
		    }

		    d_.push_back(tmp_d);
		    generalized_euclid(tmp_d, fi_, gcd, x, y);

		    if (x < 0) {
		        c_.push_back(fi_ + x);
		    } else {
		        c_.push_back(x);
		    }
		    gcd = 0;
		}
		/*
		cout << "d:" << endl;
		for (auto& x : d_) {
			cout << x << " ";
		}
		cout << endl;
		*/
	}

// для каждого ребра Ci Di 
// шифруем ребро с вершинами U и V 
// (U + 100)^Di % N
// (V + 100)^Di % N
	llong get_N()
	{
		return N_;
	}

	llong get_d(int index)
	{
		return d_[index];
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
   		// H is a isomorphic graph
   		// F is a encoded graph H
   		F_ = H_;

   		// Encode edges H
   		int i{0};
   		for (auto& val : F_) {
   			int tmp_first = val.get_first();
   			int tmp_second = val.get_second();

   			tmp_first = pow_module(tmp_first + 100, d_[i], N_);
   			tmp_second = pow_module(tmp_second + 100, d_[i], N_);
   			/*
   			cout << "i = " << i << " : " << "first " << tmp_first << endl;
   			cout << "i = " << i << " : " << "tmp_second " << tmp_second << endl;
   			*/
   			val.set_first(tmp_first);
   			val.set_second(tmp_second);
   			++i;
   		}   
		/*
   		cout << "H:" << endl;
   		for (auto& x : H_) {
   			cout << x.get_first() << " : " << x.get_second() << endl;
   		} 
 		*/  		

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
    
    // What is a Hamiltonian cycle for H?
    Graph ask_question1(vector<int>& gamilton_cycle)
    {
        // Decode Hamiltonian cycle in graph F
        llong first_H, second_H;
        llong first_F, second_F;
       
       	auto it_F = F_.begin();
       	auto it_H = H_.begin();
       	int i, j;

        for (i = 0; i < static_cast<int>(gamilton_cycle_.size() - 1); ++i) {

        	for (j = 0, it_F = F_.begin(), it_H = H_.begin(); it_F != F_.end(); ++it_F, ++it_H, ++j) {

        		first_H = it_H->get_first();
        		second_H = it_H->get_second();

            	if ( (first_H == gamilton_cycle_[i] && second_H == gamilton_cycle_[i + 1]) ||
            	     (second_H == gamilton_cycle_[i] && first_H == gamilton_cycle_[i + 1]) ) {
            		
            		first_F = it_F->get_first();
            		second_F = it_F->get_second();

            		first_F = pow_module(first_F, c_[j], N_) - 100;
            		second_F = pow_module(second_F, c_[j], N_) - 100;

            		it_F->set_first(first_F);
            		it_F->set_second(second_F);
            		// Return Hamiltonian cycle for F
            		gamilton_cycle = gamilton_cycle_;
            		break;
            	}
        	}
        }
        return Graph(F_, m_, n_);
    }
    
    // Is a graph H isomorphic to G?
    Graph ask_question2(vector<int>& permutation)
    {
    	permutation = permutation_;

    	return Graph(H_, m_, n_);
    }
    

};


int main(int argc, char** argv)
{
    if (argc != 2) {
    	cerr << "Usage: " << argv[0] << " <filename>" << endl;
    	return 1;
    }

    // Filling graph G from file
    Graph G(argv[1]);
    cout << "G:" << endl;
    G.show_edges();

    // Reading hamiltonian cycle from file 
    Alice alice(argv[1]);
    //alice.show_gamilton_cycle();
 
 	// Bob receives graph F
    Graph F(alice.get_F(G), G.get_m(), G.get_n());
    cout << "F:" << endl;
    F.show_edges();
    

    //Detrmine # of question
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    srand(seed);
    int num_question = rand() % 2;

    if (num_question) { /* question #1 */
    	cout << "Question #1" << endl;
	    vector<int> gamilton_cycle_F;
	    Graph tmp = alice.ask_question1(gamilton_cycle_F);
	    cout << "Graph F with decoded hamiltonian cycle:" << endl;
	    tmp.show_edges();

// 3 condition: 
// - firts and last items is equal
// - number of items in cycle equal number of vertexes + 1
// - no repeated items 

	    // check 1 condition
	    if (gamilton_cycle_F[0] != gamilton_cycle_F[gamilton_cycle_F.size() - 1]) {
	    	cout << "Question #1 false" << endl;
	    	cout << "Wrong hamiltonian cycle" << endl;
	    	cout << "condition #1" << endl;
	    	return 0;
	    }
		
		// check 2 condition
	    if (static_cast<int>(gamilton_cycle_F.size()) != G.get_n() + 1) {
	    	cout << "Question #1 false" << endl;
	    	cout << "Wrong hamiltonian cycle" << endl;
	    	cout << "condition #2" << endl;
	    	return 0;
	    }
		
		// check 3 condition
	    for (int i = 1; i < static_cast<int>(gamilton_cycle_F.size()); ++i) {
	    	for (int j = i + 1; j < static_cast<int>(gamilton_cycle_F.size()); ++j) {
	    		if (gamilton_cycle_F[i] == gamilton_cycle_F[j]) {
	    			cout << "Question #1 false" << endl;
	    			cout << "Wrong hamiltonian cycle" << endl;
	    			cout << "condition #3" << endl;
	    			return 0;
	    		}
	    	}
	    }
	    cout << "Hamiltonian cycle corectly!" << endl << endl;
	    // Encode Hamiltonian cycle in graph F
	    llong first, second;
		list<Item_list_edges> F_edges = tmp.get_list_edges();

	    int i, j;
	    auto it = F_edges.begin();
	    for (i = 0; i < static_cast<int>(gamilton_cycle_F.size() - 1); ++i) {
	    	
	      	for (it = F_edges.begin(), j = 0; it != F_edges.end(); ++it, ++j) {
	       		first = it->get_first();
	       		second = it->get_second();

	           	if ( (first == gamilton_cycle_F[i] && second == gamilton_cycle_F[i + 1]) ||
	           	     (second == gamilton_cycle_F[i] && first == gamilton_cycle_F[i + 1]) ) {
	           		//cout << alice.get_d(j) << " j " << j << endl;
	           		first = pow_module(first + 100, alice.get_d(j), alice.get_N());
	           		second = pow_module(second + 100, alice.get_d(j), alice.get_N());
	           		//cout << tmp_first << " : " << tmp_second << endl;
	           		
	           		it->set_first(first);
	           		it->set_second(second);
	           		break;
	           	}
	       	}
	    }
	    Graph res(F_edges, G.get_m(), G.get_n());
	    cout << "Graph F with encoded hamiltonian cycle" << endl;
	    res.show_edges();

	    if (F == res) {
	    	cout << "Qustion #1 true" << endl;
	    } else {
			cout << "Question #1 false" << endl;
	    }

	} else { /* question #2 */
		vector<int> permutation;
		// Get graph H and vector permutation for G
	   	Graph tmp = alice.ask_question2(permutation);

	   	cout << "H:" << endl;
	   	tmp.show_edges();

	   	// Convert permutation - index to value
	   	vector<int> ind_to_val = permutation;
	   	int i = 0;
	   	for (auto& val : permutation) {
	   		ind_to_val[val - 1] = i + 1;
	   		i++;
	   	}

	   	// Apply reverse permutaion to H to give graph G 
	   	auto t = tmp.get_list_edges();
	   	for (auto& val : t) {
	    	val.set_first(ind_to_val[val.get_first() - 1]);
	    	val.set_second(ind_to_val[val.get_second() - 1]);
	    }
	    
	    Graph tt(t, G.get_m(), G.get_n());
	    cout << "Graph G obtained after apply permutation" << endl;
	    tt.show_edges();
	    if (tt == G) {
	    	cout << "Question #2 true" << endl;
	    } else {
	    	cout << "Question #2 false" << endl;
	    }
	}

	return 0;
}
