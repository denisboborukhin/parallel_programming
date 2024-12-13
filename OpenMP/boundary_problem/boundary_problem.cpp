#include <cmath>
#include <fstream>
#include <iostream>
#include <omp.h>
#include <vector>

const double X_MIN = -10.0;
const double X_MAX = 10.0;
const double TOL = 1e-6;
const int MAX_ITER = 100000;

// #define HARD

double f(double y, double a) {
#ifdef HARD
    return a * (y * y * y - y);
#else
    return a * (y - y * y * y);
#endif
}

double d_f(double y, double a) {
#ifdef HARD
    return a * (3 * y * y - 1);
#else
    return a * (1 - 3 * y * y);
#endif
}

void thomas_algorithm(const std::vector<double> &a,
                      const std::vector<double> &b,
                      const std::vector<double> &c,
                      const std::vector<double> &d, std::vector<double> &y) {
    int N = d.size();
    std::vector<double> c_star(N, 0.0);
    std::vector<double> d_star(N, 0.0);

    c_star[0] = c[0] / b[0];
    d_star[0] = d[0] / b[0];
    for (int i = 1; i < N; ++i) {
        double m = b[i] - a[i] * c_star[i - 1];
        c_star[i] = c[i] / m;
        d_star[i] = (d[i] - a[i] * d_star[i - 1]) / m;
    }

    y[N - 1] = d_star[N - 1];
    for (int i = N - 2; i >= 0; --i) {
        y[i] = d_star[i] - c_star[i] * y[i + 1];
    }
}

void solve_numerov_newton(int N, double a, double y0, double yN,
                          const std::string &output_file) {
    N *= sqrt(a) / 10;
    double h = (X_MAX - X_MIN) / N;
    std::vector<double> x(N), y(N, 0.0), f_values(N, 0.0);

    for (int i = 0; i < N; ++i) {
        x[i] = X_MIN + i * h;
    }

    y[0] = y0;
    y[N - 1] = yN;

    omp_set_dynamic(0);
    // std::cout << omp_get_max_threads() << std::endl;
    omp_set_num_threads(omp_get_max_threads());

    for (int iter = 0; iter != MAX_ITER; ++iter) {
        std::vector<double> a_diag(N - 2, 1.0);
        std::vector<double> b_diag(N - 2, -2.0);
        std::vector<double> c_diag(N - 2, 1.0);
        std::vector<double> rhs(N - 2, 0.0);

#pragma omp parallel for
        for (int i = 1; i < N - 1; ++i) {
            double f_val = f(y[i], a);
            double f_derivative = d_f(y[i], a);
            double correction =
                (y[i - 1] - 2 * y[i] + y[i + 1]) / (h * h) - f_val;

            if (i > 1)
                a_diag[i - 1] = 1.0;
            b_diag[i - 1] = -2.0 - h * h * f_derivative;
            if (i < N - 2)
                c_diag[i - 1] = 1.0;

            rhs[i - 1] = -correction * h * h;
        }

        std::vector<double> delta_y(N - 2, 0.0);
        thomas_algorithm(a_diag, b_diag, c_diag, rhs, delta_y);

#pragma omp parallel for
        for (int i = 1; i < N - 1; ++i) {
            y[i] += delta_y[i - 1];
        }

        double max_error = 0.0;
#pragma omp parallel for reduction(max : max_error)
        for (int i = 1; i < N - 1; ++i) {
            double f_val =
                (y[i - 1] - 2 * y[i] + y[i + 1]) / (h * h) - f(y[i], a);
            max_error = std::max(max_error, std::fabs(f_val));
        }

        if (max_error < TOL) {
            break;
        }
    }

    std::ofstream results(output_file);
    if (results.is_open()) {
        for (const auto &xi : x) {
            results << xi << " ";
        }
        results << "\n";
        for (const auto &yi : y) {
            results << yi << " ";
        }
        results.close();
    }
}

int main(int argc, char **argv) {
    int N = 2000;
    double a = 100000.0;
    if (argc >= 3) {
        N = atoi(argv[1]);
        a = atof(argv[2]);
    }

    double y0 = sqrt(2.0);
    double yN = sqrt(2.0);

    solve_numerov_newton(N, a, y0, yN, "results.txt");

    return 0;
}
