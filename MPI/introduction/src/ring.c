#include <mpi.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
    int commsize = 0;
    int my_rank = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &commsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    
    int token = 0;
    if (my_rank != 0) {
        MPI_Recv(&token, 1, MPI_INT, my_rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process %d received token %d from process %d\n", my_rank, token, my_rank - 1);
    } else {
        token = 1;
    }

    token *= 10;
    MPI_Send(&token, 1, MPI_INT, (my_rank + 1) % commsize, 0, MPI_COMM_WORLD);

    if (my_rank == 0) {
        MPI_Recv(&token, 1, MPI_INT, commsize - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process %d received token %d from process %d\n", my_rank, token, commsize - 1);
    }

    MPI_Finalize();
}