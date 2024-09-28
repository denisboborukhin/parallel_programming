#include <pthread.h>
#include <cmath>
#include <iostream>
#include <fstream>
#include <chrono>

double func(const double x) {
    return cos(1 / (x * x));
}

const double A = 0.001;
const double B = 5;

double simpson_integration(double a, double b, double eps) {
    int n = 2; 
    double h = (b - a) / n;
    double result = (func(a) + 4 * func(a + h) + func(b)) * h / 3.0;
    double previous_result;
    
    do {
        previous_result = result;
        n *= 2;
        h = (b - a) / n;
        result = 0.0;
        for (int i = 0; i <= n; i++) {
            if (i == 0 || i == n) {
                result += func(a + i * h);
            } else if (i % 2 == 0) {
                result += 2 * func(a + i * h);
            } else {
                result += 4 * func(a + i * h);
            }
        }
        result *= h / 3.0;
    } while (fabs(result - previous_result) > eps);

    return result;
}

struct thread_data {
    int tid;
    double eps;
    double a, b;
    double partial_integral;
};

void* thread_func(void* arg) {
    thread_data* data = reinterpret_cast<thread_data*>(arg);
    data->partial_integral= simpson_integration(data->a, data->b, data->eps);

    pthread_exit(NULL);
}

double integrate_parallel(int num_threads, double eps) {
    pthread_t thr[num_threads];
    int i, rc;

    thread_data thread_storage[num_threads];

    for (i = 0; i < num_threads; ++i) {
        thread_storage[i].tid = i;
        thread_storage[i].a = A + (B - A) * i / num_threads;
        thread_storage[i].b = A + (B - A) * (i + 1) / num_threads;
        thread_storage[i].partial_integral = 0;
        thread_storage[i].eps = eps / num_threads;
        pthread_create(&thr[i], NULL, thread_func, &thread_storage[i]);
    }

    double integral = 0;
    for (i = 0; i < num_threads; ++i) {
        pthread_join(thr[i], NULL);
        integral += thread_storage[i].partial_integral;
    }

    return integral;
}

int main(int argc, char **argv) {
    double accuracy = argc < 2 ? 1e-4 : atoi(argv[1]);
    std::ofstream output;
    output.open("out.csv");

    for (int num_threads = 1; num_threads != 5; ++num_threads)
    {
        auto begin = std::chrono::high_resolution_clock::now();
        double integral = integrate_parallel(num_threads, accuracy);
        auto end = std::chrono::high_resolution_clock::now();

        double t = (end - begin).count();
        output << num_threads << ',' << t << ',' << integral << '\n';
    }
    
    output.close();   
    return 0;
}