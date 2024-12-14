#include <cassert>
#include <cmath>

#include <omp.h>

#include "../helper.hpp"


int main(int argc, char **argv) {
    int x = ISIZE; 
    int y = JSIZE;
    if (argc >= 3) {
        x = atoi(argv[1]);
        y = atoi(argv[2]);
    }

    auto a = getArrOMP(x, y);

    for (auto i = int{}; i < x; ++i) {
        for (auto j = int{}; j < y; ++j) {
            a[i][j] = 10 * i + j;
        }
    }

    int num_threads = 1;
    if (argc >= 4) {
        num_threads = std::atoi(argv[3]);
        if (num_threads <= 0) {
            return 0;
        }
    }

    omp_set_dynamic(0);
    omp_set_num_threads(num_threads);

    auto start = omp_get_wtime();

#pragma omp parallel for
    for (auto i = int{}; i < x; ++i) {
        for (auto j = int{}; j < y; ++j) {
            a[i][j] = std::sin(2 * a[i][j]);
        }
    }

    printInfo(num_threads, omp_get_wtime() - start);
    saveToFile("app0_omp", num_threads,  a, x, y);
    return 0;
}