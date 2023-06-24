#include <iostream>
#include <cmath>
#include <omp.h>

bool esPrimo(int numero)
{
    if (numero <= 1)
    {
        return false;
    }

    int limite = std::sqrt(numero);
    for (int i = 2; i <= limite; ++i)
    {
        if (numero % i == 0)
        {
            return false;
        }
    }

    return true;
}

int main()
{
    const int limiteSuperior = 500000000;

#pragma omp parallel for
    for (int numero = 1; numero <= limiteSuperior; ++numero)
    {
        if (esPrimo(numero))
        {
#pragma omp critical
            std::cout << numero << " ";
        }
    }

    std::cout << std::endl;

    return 0;
}
