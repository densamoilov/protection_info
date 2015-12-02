#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

int main(int argc, char** argv)
{
    if (argc != 3) {
    	cout << "Usage: " << argv[0] << " <m>" << " <n>" << endl;
    	return 1;
    }

    int n = atoi(argv[2]); // number of vertex
    int m = atoi(argv[1]); // number of edge

    if (m < n) {
    	cerr << "m must be great then n" << endl;
    	return 1;
    }

    ofstream output_file("data");
    if (!output_file.is_open()) {
    	cerr << "Cannot open file 'data'" << endl;
    	return 1;
    }

    default_random_engine gen(time(0));
    uniform_int_distribution<int> random(1, n);

    output_file << n << " " << m << endl;

    // Generate graph ring
    for (int i = 1; i <= n; ++i) {
        output_file << i << " " << i % n + 1 << endl;
    }

    for (int i = 1; i <= n; ++i) {
    	output_file << i << " ";
    }
    output_file << endl;	
/*
    // Add additional edges
    for (int i = n; i <= m; ++i) {
    	output_file << random(gen) << " " << random(gen) << endl;
    }
*/
    output_file.close();

	return 0;
}
