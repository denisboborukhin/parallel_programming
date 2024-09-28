#ifndef SOLVER
#define SOLVER

#include <vector>
#include <functional>
#include <string>

struct ConvectionDiffusionProblem //equation of form u_t + au_x = 0 with initial values f_0 and boundary conditions y_0
{
    double a; //parameter in equation
    double L, T; //space and time sizes of study area
    
    //constructor
    ConvectionDiffusionProblem(double, double, double);

    //method that gives exact solution y(x, t)
    double y_exact(double, double);
};

class Solver
{
    private:
    int rank, size; //rank and size of MPI_Comm

    int N, K; //cells in computational mesh (n - space, k - time)
    int left, right, length; //vertexes passed to this process
    double h, tau, c, time_coef; //coordinate and time steps and current parameter

    std::vector<double> y_cur; //2 last calculation steps
    std::vector<double> y_prev; 
    std::vector<double> y_prev_2;
    std::vector<double> x;

    ConvectionDiffusionProblem problem; //problem to solve

    public:
    //constructor
    Solver(int, int, int, int, ConvectionDiffusionProblem);

    //common functions
    virtual void change_N(int);
    void change_N_K(int, int);
    double get_error(double t);
    double solve(int);
    //specific for scheme functions
    virtual void make_step(int);
};

#endif