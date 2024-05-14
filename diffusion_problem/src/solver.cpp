#include <cmath>
#include <math.h>
#include <fstream>
#include <mpi.h>
#include <vector>
#include "../include/solver.h"

double f_0 (double x)
{
    return std::cos(M_PI * x);
}

double y_0(double x)
{
    return std::exp(-x);
}

ConvectionDiffusionProblem::ConvectionDiffusionProblem(double a, double L, double T):
                                                         a{a}, L{L}, T{T} { }

double ConvectionDiffusionProblem::y_exact(double x, double t) {
    // ???
}

Solver::Solver(int size, int rank, int N, int K, ConvectionDiffusionProblem problem):
               size{size}, rank{rank}, N{N}, K{K}, problem{problem} {
    change_N_K(N, K);
}

void Solver::change_N(int N_new) {
    N = N_new;
    left = (rank * (N + 1)) / size;
    right = ((rank + 1) * (N + 1)) / size;
    length = right - left;
    y_cur = std::vector<double>(length, 0.0);
    y_prev = std::vector<double>(length, 0.0);
    y_prev_2 = std::vector<double>(length, 0.0);

    h = problem.L / N;
    c = problem.a * tau / h;

    x = std::vector<double>(length, 0.0);
    for (int i = 0; i < length; i++) {
        x[i] = (problem.L * (i + left)) / N;
    }
}

void Solver::change_N_K(int N_new, int K_new) {
    change_N(N_new);
    K = K_new;
    tau = problem.T / K;
    c = problem.a * tau / h;
    time_coef = problem.T / K;
}

double Solver::get_error(double t) {
    double max_error = 0;
    double error = 0;

    for (int i = 0; i < length; i++) {
        error = std::abs(y_cur[i] - problem.y_exact(x[i], t));
        if(error > max_error) {
            max_error = error;
        }
    }

    return max_error;
}

double Solver::solve(int frames) {
    MPI_Barrier(MPI_COMM_WORLD);

    double max_error = 0;
    double error = 0;

    for (int i = 0; i < length; i++) {
        y_cur[i] = f_0(x[i]);
    }

    //initializing writing to a file
    std::ofstream output;
    std::ofstream output_exact;
    
    //calculating
    for(int j = 1; j <= K; j++) {
        make_step(j);

        error = get_error(time_coef * j);
        if (error > max_error) {
            max_error = error;
        }
    }

    //closing a file
    if (frames != 0) {
        output.close();
        output_exact.close();
    }
 
    return max_error;
}

void Solver::make_step(int j) {
    // y_prev_2 = y_prev;
    // y_cur = y_prev_2;
    // auto temp = y_cur;
    // y_prev = temp;
    std::swap(y_prev, y_prev_2);
    std::swap(y_cur, y_prev);

    double y_left; double y_right;

    if (j == 1) {
        if (rank == size - 1) {
            y_cur[0] = y_0(time_coef * j);
        } else {
            y_cur[0] = y_prev[0] - c * (y_prev[0] - f_0(x[0] - h));
        }
        for (int i = 1; i < length; i++) {
            y_cur[i] = y_prev[i] - c * (y_prev[i] - y_prev[i - 1]);
        }
    } else {

        if(size > 1) {
            if (rank == 0) {
                MPI_Sendrecv(&y_prev[length - 1], 1, MPI_DOUBLE, 1, j - 1, 
                             &y_right, 1, MPI_DOUBLE, 1, K + j - 1,
                             MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            
            } else if (rank < size - 1) {
                MPI_Sendrecv(&y_prev[length - 1], 1, MPI_DOUBLE, rank + 1, j - 1, 
                             &y_left, 1, MPI_DOUBLE, rank - 1, j - 1,
                             MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                MPI_Sendrecv(&y_prev[0], 1, MPI_DOUBLE, rank - 1, K + j - 1, 
                             &y_right, 1, MPI_DOUBLE, rank + 1, K + j - 1,
                             MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            } else {
                MPI_Sendrecv(&y_prev[0], 1, MPI_DOUBLE, rank - 1, K + j - 1, 
                             &y_left, 1, MPI_DOUBLE, rank - 1, j - 1,
                             MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
        }

        if (rank == size - 1) {
            y_cur[0] = y_0(time_coef * j);
        } else {
            y_cur[0] = y_prev_2[0] - c * (y_prev[1] - y_left);
        }

        for(int i = 1; i < length - 1; i++) {
            y_cur[i] = y_prev_2[i] - c * (y_prev[i + 1] - y_prev[i - 1]);
        }

        if (rank == size - 1) {
            y_cur[length - 1] = y_prev[length - 1] - c * (y_prev[length - 1] - y_prev[length - 2]);
        } else {
            y_cur[length - 1] = y_prev_2[length - 1] - c * (y_right - y_prev[length - 2]);
        }
    }
}