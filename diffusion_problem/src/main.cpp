#include <iostream>
#include <fstream>
#include <math.h>
#include <mpi.h>
// #include <stdio.h>

#include "../include/solver.h"

int main(int argc, char **argv)
{
    //initializing MPI
    MPI_Init(&argc, &argv);

    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double a = 0.5; //parameter in equation
    double L = 4; double T = 4; //space and time sizes of study area
                                //
                                //
    auto rectangle = ConvectionDiffusionProblem(a, L, T);

    //initial paraneters for solver
    int N = 1000; int K = 1000;

    //setting up solver
    auto rect_solver = Solver(size, rank, N, K, rectangle);

    //making animation
    // rect_solver.solve(180);
    //
    //plotting time depending on number of cells in a mesh
    int deg_min = 10; int deg_max = 10;
    std::ofstream output;

    if (rank == 0) {
        output.open("../out/times/" + std::to_string(size) + ".csv");
        output << "n,t\n";
    }
   
    for(int i = deg_min; i <= deg_max; i++) {
        rect_solver.change_N(std::pow(2, i));
        double t_1, t_2;
        t_1 = MPI_Wtime();
        rect_solver.solve(0);
        t_2 = MPI_Wtime();
        if(rank == 0) {
            std::cout << "time = " << t_2 - t_1 << "\n";
            output << std::pow(2, i) << "," << t_2 - t_1 << '\n';
        }
    }

    if (rank == 0) {
        output.close();
    }

    MPI_Finalize();

    return 0;
}
