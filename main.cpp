#include <iostream>
#include <omp.h>

bool esPrimo(int num) {
    if (num <= 1) {
        return false;
    }

    for (int i = 2; i * i <= num; ++i) {
        if (num % i == 0) {
            return false;
        }
    }

    return true;
}

int main() {
    std::cout << "Números primos entre 1 y 2,000,000:" << std::endl;

    #pragma omp parallel for
    for (int i = 2; i <= 2000000; ++i) {
        if (esPrimo(i)) {
            #pragma omp critical
            {
                std::cout << i << std::endl;
            }
        }
    }

    return 0;
}
