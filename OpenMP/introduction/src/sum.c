#include <stdio.h>
#include <omp.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        return 0;
    }

    const int n = atoi(argv[1]);
    double total_sum = 0;
    
    #pragma omp parallel
    {
        double partial_sum = 0;
        #pragma omp for
        for (int i = 1; i != n + 1; ++i) {
            partial_sum += 1.0 / i;
        }
        
        #pragma omp critical
        {
            total_sum += partial_sum;
        }
    }

    printf("Sum from 1 to %d: %f\n", n, total_sum);
    return 0;
}