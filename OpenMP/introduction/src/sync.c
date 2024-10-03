#include <stdio.h>
#include <omp.h>

int main() {
    int shared_variable = 0;
    
    #pragma omp parallel
    {
        const int thread_id = omp_get_thread_num();
        const int num_threads = omp_get_num_threads();

        for (int i = 0; i < num_threads; i++) {
            if (thread_id == i) {
                shared_variable += 10;
                printf("Thread %d: value: %d\n", thread_id, shared_variable);
            }
            #pragma omp barrier
        }
    }

    return 0;
}