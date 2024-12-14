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

    const auto diff_I = int{x / commsize};
    auto start_I = int{diff_I * rank};
    auto end_I = int{diff_I * (rank + 1)};

    if (x % commsize) {
        if (rank < x % commsize) {
            start_I += rank;
            end_I += rank + 1;
        } else {
            start_I += (x % commsize);
            end_I += (x % commsize);
        }
    }

    for (auto i = start_I; i < end_I; ++i) {
        for (auto j = 0; j < y; ++j) {
            a[i][j] = 10 * i + j;
        }
    }

    auto start = MPI_Wtime();
    MPI_Barrier(MPI_COMM_WORLD);

    for (auto i = start_I; i < end_I; ++i) {
        for (auto j = int{}; j < y; ++j) {
            a[i][j] = std::sin(2 * a[i][j]);
        }
    }

    if (rank) {
        MPI_Request myRequest;

        auto sendStart = start_I;
        int sendSize = (end_I - start_I) * y;
        MPI_Isend(&sendStart, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &myRequest);
        MPI_Isend(&sendSize, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &myRequest);
        MPI_Isend(&a[start_I][0], sendSize, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD,
                  &myRequest);
    } else {
        for (auto k = 1; k < commsize; ++k) {
            int recvStart = 0;
            int recvSize = 0;

            MPI_Recv(&recvStart, 1, MPI_INT, k, 0, MPI_COMM_WORLD,
                     MPI_STATUS_IGNORE);
            MPI_Recv(&recvSize, 1, MPI_INT, k, 0, MPI_COMM_WORLD,
                     MPI_STATUS_IGNORE);
            MPI_Recv(&a[recvStart][0], recvSize, MPI_DOUBLE, k, 0,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
    }
    MPI_Barrier(MPI_COMM_WORLD);

    if (!rank) {
        auto end = MPI_Wtime();
        printInfo(commsize, end - start);
        saveToFile("app0_mpi", commsize, a, x, y);
    }

    deleteArrMPI(a, x);
    MPI_Finalize();
    return 0;
}