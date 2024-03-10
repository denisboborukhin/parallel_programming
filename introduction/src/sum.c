#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    int commsize = 0;
    int my_rank = 0;
    
    if (argc < 2) {
        return 0;
    }
    
    int n = atoi(argv[1]);

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &commsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    int process_n = n / commsize;
    double process_sum = 0.0;
    if (my_rank != commsize - 1) {
        for (int i = my_rank * process_n + 1; i != (my_rank + 1) * process_n + 1; ++i) {
            process_sum += 1.0 / i;
        }
    } else {
        for (int i = my_rank * process_n + 1; i != n + 1; ++i) {
            process_sum += 1.0 / i;
        }
    }

    double res = 0.0;
    MPI_Reduce(&process_sum, &res, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (my_rank == 0) {
        printf("Hello world! communicator size = %d, my rank = %d; sum = %f\n", commsize, my_rank, res);
    }

    MPI_Finalize();
}