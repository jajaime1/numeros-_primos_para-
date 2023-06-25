#include <iostream>
#include <vector>
#include <cmath>
#include <pthread.h>
#include <chrono>

struct ThreadData {
    int inicio;
    int fin;
    int contador;
};

bool numprimos(int num) {
    if (num <= 2) {
        return (num == 2);
    }
    if (num % 2 == 0) {
        return false;
    }

    int limite = static_cast<int>(std::sqrt(num));
    for (int i = 3; i <= limite; i += 2) {
        if (num % i == 0) {
            return false;
        }
    }

    return true;
}

void cribaAtkin(int inicio, int fin, std::vector<bool>& primos) {
    int limite = static_cast<int>(std::sqrt(fin));

    for (int x = 1; x <= limite; ++x) {
        for (int y = 1; y <= limite; ++y) {
            int n = 4 * x * x + y * y;
            if (n <= fin && (n % 12 == 1 || n % 12 == 5)) {
                primos[n] = false;
            }

            n = 3 * x * x + y * y;
            if (n <= fin && n % 12 == 7) {
                primos[n] = false;
            }

            n = 3 * x * x - y * y;
            if (x > y && n <= fin && n % 12 == 11) {
                primos[n] = false;
            }
        }
    }

    for (int n = 5; n <= limite; ++n) {
        if (primos[n]) {
            int k = n * n;
            for (int i = k; i <= fin; i += k) {
                primos[i] = false;
            }
        }
    }
}

void* buscarPrimos(void* arg) {
    ThreadData* data = static_cast<ThreadData*>(arg);
    int inicio = data->inicio;
    int fin = data->fin;
    int contador = 0;

    std::vector<bool> primos(fin + 1, true);
    cribaAtkin(inicio, fin, primos);

    for (int i = inicio; i <= fin; ++i) {
        if (primos[i]) {
            ++contador;
            std::cout << i << std::endl;
        }
    }

    data->contador = contador;

    pthread_exit(NULL);
}

int main() {
    const int n = 500000000;
    const int num_threads = 10;
    const int chunk_size = n / num_threads;

    std::vector<pthread_t> threads(num_threads);
    std::vector<ThreadData> thread_data(num_threads);

    auto start_time = std::chrono::steady_clock::now();

    for (int i = 0; i < num_threads; ++i) {
        thread_data[i].inicio = i * chunk_size + 1;
        thread_data[i].fin = (i == num_threads - 1) ? n : (i + 1) * chunk_size;
        thread_data[i].contador = 0;

        pthread_create(&threads[i], NULL, buscarPrimos, &thread_data[i]);
    }

    for (int i = 0; i < num_threads; ++i) {
        pthread_join(threads[i], NULL);
    }

    auto end_time = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::minutes>(end_time - start_time);

    int contador_total = 0;
    for (int i = 0; i < num_threads; ++i) {
        contador_total += thread_data[i].contador;
    }

    std::cout << "Total de números primos encontrados: " << contador_total << std::endl;
    std::cout << "Tiempo de ejecución: " << duration.count() << " minutos." << std::endl;

    return 0;
}
