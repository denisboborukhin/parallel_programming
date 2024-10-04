#include <functional>
#include <iostream>
#include <list>
#include <omp.h>
#include <ctime>
#include <threads.h>
#include <vector>

#define NUM_ITERATIONS 2000000
#define NUM_THREADS 4

void print_schedule_info(const std::string& schedule_type, const int chunk_size) {
    std::cout << "Schedule Type: " << schedule_type << ", Chunk Size: " << chunk_size << std::endl;
}

void print_results(const std::vector<std::vector<int>>& threads_balance) {
    for (auto i = 0; i != NUM_THREADS; ++i) {
        std::cout << "thread " << i << ": ";
        for (const auto& iter: threads_balance[i]) {
            std::cout << iter << ";";
        }
        std::cout << std::endl;
    }
}

double balancer_loop(const int chunk_size) {
    print_schedule_info("none", 0);
    std::vector<std::vector<int>> threads_balance{};
    threads_balance.reserve(NUM_THREADS);
    threads_balance.resize(NUM_THREADS);

    double total_sum = 0;
    #pragma omp parallel num_threads(NUM_THREADS)
    {
        double partial_sum = 0;
        #pragma omp for
        for (auto i = 1; i != NUM_ITERATIONS; ++i) {
            threads_balance[omp_get_thread_num()].push_back(i);
            partial_sum += 1.0 / i;
        }
        
        #pragma omp critical
        {
            total_sum += partial_sum;
        }
    }
     
    // print_results(threads_balance);
    return total_sum;
}

double balancer_loop_static(const int chunk_size) {
    print_schedule_info("static", chunk_size);
    std::vector<std::vector<int>> threads_balance{};
    threads_balance.reserve(NUM_THREADS);
    threads_balance.resize(NUM_THREADS);

    double total_sum = 0;
    #pragma omp parallel num_threads(NUM_THREADS)
    {
        double partial_sum = 0;
        #pragma omp for schedule(static, chunk_size)
        for (auto i = 1; i != NUM_ITERATIONS; ++i) {
            threads_balance[omp_get_thread_num()].push_back(i);
            partial_sum += 1.0 / i;
        }
        
        #pragma omp critical
        {
            total_sum += partial_sum;
        }
    }
     
    // print_results(threads_balance);
    return total_sum;
}

double balancer_loop_dynamic(const int chunk_size) {
    print_schedule_info("dynamic", chunk_size);
    std::vector<std::vector<int>> threads_balance{};
    threads_balance.reserve(NUM_THREADS);
    threads_balance.resize(NUM_THREADS);

    double total_sum = 0;
    #pragma omp parallel num_threads(NUM_THREADS)
    {
        double partial_sum = 0;
        #pragma omp for schedule(dynamic)
        for (auto i = 1; i != NUM_ITERATIONS; ++i) {
            threads_balance[omp_get_thread_num()].push_back(i);
            partial_sum += 1.0 / i;
        }
        
        #pragma omp critical
        {
            total_sum += partial_sum;
        }
    }
     
    // print_results(threads_balance);
    return total_sum;
}

double balancer_loop_guided(const int chunk_size) {
    print_schedule_info("guided", chunk_size);
    std::vector<std::vector<int>> threads_balance{};
    threads_balance.reserve(NUM_THREADS);
    threads_balance.resize(NUM_THREADS);

    double total_sum = 0;
    #pragma omp parallel num_threads(NUM_THREADS)
    {
        double partial_sum = 0;
        #pragma omp for schedule(guided, chunk_size)
        for (auto i = 1; i != NUM_ITERATIONS; ++i) {
            threads_balance[omp_get_thread_num()].push_back(i);
            partial_sum += 1.0 / i;
        }
        
        #pragma omp critical
        {
            total_sum += partial_sum;
        }
    }
     
    // print_results(threads_balance);
    return total_sum;
}

int main() {
    const std::vector<int> chunk_sizes{2, 4};
    std::list<std::function<int(const int)>> funcs;
    funcs.emplace_back(balancer_loop);
    funcs.emplace_back(balancer_loop_static);
    funcs.emplace_back(balancer_loop_dynamic);
    funcs.emplace_back(balancer_loop_guided);

    for (const auto& func: funcs) {
        for (const auto chunk_size: chunk_sizes) {
            const double start = omp_get_wtime(); 
            func(chunk_size);            
            std::cerr << "time: " << (omp_get_wtime() - start) << std::endl;
        }
    }

    return 0;
}