#include <cmath>
#include <mpi/mpi.h>

#include "../helper.hpp"

int main(int argc, char **argv) {
    int x = ISIZE;
    int y = JSIZE;
    if (argc >= 3) {
        x = atoi(argv[1]);
        y = atoi(argv[2]);
    }

    MPI_Init(&argc, &argv);
    int commsize = 0;
    int rank = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &commsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    auto **a = getArrMPI(x, y);

    const int diff_J = y / commsize;
    int start_J = diff_J * rank;
    int end_J = diff_J * (rank + 1);

    if (y % commsize) {
        if (rank < y % commsize) {
            start_J += rank;
            end_J += rank + 1;
        } else {
            start_J += (y % commsize);
            end_J += (y % commsize);
        }
    }

    if (rank == commsize - 1) {
        end_J = y - 1;
    }

    for (int i = 0; i < x; ++i) {
        for (int j = 0; j < y; ++j) {
            a[i][j] = 10 * i + j;
        }
    }

    int localArraySize = end_J - start_J;

    double *localArray = new double[y];
    int *recvcnts = new int[commsize];
    int *displs = new int[commsize];

    for (int k = 0; k < commsize; ++k) {
        MPI_Gather(&localArraySize, 1, MPI_INT, &recvcnts[0], 1, MPI_INT, k,
                   MPI_COMM_WORLD);
    }

    displs[0] = 0;
    for (int i = 1; i < commsize; ++i) {
        displs[i] = displs[i - 1] + recvcnts[i - 1];
    }

    double start = MPI_Wtime();
    MPI_Barrier(MPI_COMM_WORLD);

    if (commsize > 1) {
        for (int i = 1; i < x; ++i) {
            for (int j = start_J; j < end_J; ++j) {
                localArray[j] = std::sin(2 * a[i - 1][j + 1]);
            }

            MPI_Allgatherv(&localArray[start_J], localArraySize, MPI_DOUBLE,
                           &a[i][0], &recvcnts[0], &displs[0], MPI_DOUBLE,
                           MPI_COMM_WORLD);
        }
    } else {
        for (int i = 1; i < x; ++i) {
            for (int j = 0; j < y - 1; ++j) {
                a[i][j] = std::sin(2 * a[i - 1][j + 1]);
            }
        }
    }

    if (!rank) {
        auto end = MPI_Wtime();
        printInfo(commsize, end - start);
        saveToFile("app1_mpi", commsize, a, x, y);
    }

    deleteArrMPI(a, x);

    delete[] localArray;
    delete[] recvcnts;
    delete[] displs;

    MPI_Finalize();
}