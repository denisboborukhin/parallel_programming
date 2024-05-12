#include <pthread.h>
#include <cmath>
#include <iostream>
#include <fstream>
#include <chrono>

double f(double x) {
    return(std::cos(1 / x * x));
}

const double A = 0.001;
const double B = 5;

void kahan_sum(double &a, double b, double &c) {
    double y = b - c; 
    b = a + y; 
    c = (b - a) - y;
    a = b;
}

struct thread_data {
    int tid;
    int N;
    double a, b;
    double partial_integral;
};

void* thread_func(void* arg) {
    thread_data* data = reinterpret_cast<thread_data*>(arg);
    double delta = (data->b - data->a) / data->N;
    double f_cur = f(data->a);
    double f_prev;
    double c = 0; //compensation

    for (double x = data->a + delta; x <= data->b; x += delta) {
        f_prev = f_cur;
        f_cur = f(x);
        kahan_sum(data->partial_integral, delta * (f_cur + f_prev) / 2, c);
    }

    pthread_exit(NULL);
}

double integrate_parallel(int num_threads, double eps) {
    pthread_t thr[num_threads];
    int i, rc;

    thread_data thread_storage[num_threads];

    for (i = 0; i < num_threads; ++i) {
        thread_storage[i].tid = i;
        thread_storage[i].N = ceil(1 / (2 * A * eps * num_threads));
        thread_storage[i].a = A + (B - A) * i / num_threads;
        thread_storage[i].b = A + (B - A) * (i + 1) / num_threads;
        thread_storage[i].partial_integral = 0;
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
    double accuracy = argc < 2 ? 1e-6 : atoi(argv[1]);
    std::ofstream output;
    output.open("out.csv");

    for (int num_threads = 1; num_threads != 13; ++num_threads)
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