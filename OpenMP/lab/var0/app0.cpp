#include <cmath>
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
    int i, j;

    for (i = 0; i < x; i++) {
        for (j = 0; j < y; j++) {
            a[i][j] = 10 * i + j;
        }
    }

    for (i = 0; i < x; i++) {
        for (j = 0; j < y; j++) {
            a[i][j] = std::sin(2 * a[i][j]);
        }
    }

    saveToFile("app0", 1, a, x, y);
    deleteArrMPI(a, x);
}