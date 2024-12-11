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
    auto b = getArrOMP(x, y);

    for (int i = 0; i < x; ++i) {
        for (int j = 0; j < y; ++j) {
            a[i][j] = 10 * i + j;
            b[i][j] = 0;
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
    int nRows = x / num_threads;

    int threadsNum = omp_get_num_threads();
    int rank = omp_get_thread_num();
    int startRow = rank * nRows;
    if (rank == threadsNum - 1) {
        nRows = x - startRow;
    }

#pragma omp parallel for schedule(static, 8)
    for (int i = startRow; i < startRow + nRows - 4; ++i) {
        for (int j = 1; j < y; j++) {
            b[i][j] = std::sin(0.002 * a[i + 4][j - 1]) * 1.5;
        }
    }

    printInfo(num_threads, omp_get_wtime() - start);
    saveToFile("app3_omp", num_threads, b, x, y);
    return 0;
}