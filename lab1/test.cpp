#include <iostream>
#include <omp.h>

#include "cryptolib.h"

int main()
{
    llong Zab, Zba;

    diffie_hellman(1, Zab, Zba);
    
    std::cout << "Zab = " << Zab << std::endl;
    std::cout << "Zba = " << Zba << std::endl;
    return 0;


}