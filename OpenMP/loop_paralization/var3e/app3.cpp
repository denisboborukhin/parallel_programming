#include <cmath>
#include <omp.h>
#include <stdlib.h>

#include "../helper.hpp"

int main(int argc, char **argv) {
    int x = ISIZE;
    int y = JSIZE;
    if (argc >= 3) {
        x = atoi(argv[1]);
        y = atoi(argv[2]);
    }

    auto a = getArrMPI(x, y);
    auto b = getArrMPI(x, y);
    int i, j;

    for (i = 0; i < x; i++) {
        for (j = 0; j < y; j++) {
            a[i][j] = 10 * i + j;
            b[i][j] = 0;
        }
    }

    auto start = omp_get_wtime();
    for (i = 0; i < x; i++) {
        for (j = 0; j < y; j++) {
            a[i][j] = std::sin(0.002 * a[i][j]);
        }
    }

    for (i = 0; i < x - 4; i++) {
        for (j = 1; j < y; j++) {
            b[i][j] = a[i + 4][j - 1] * 1.5;
        }
    }

    auto end = omp_get_wtime();
    printInfo(1, end - start);
    saveToFile("app3", 1, b, x, y);
    deleteArrMPI(a, x);
    deleteArrMPI(b, x);
}