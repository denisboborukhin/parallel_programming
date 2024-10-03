#include <stdio.h>
#include <omp.h>

int main() {
    omp_set_nested(1);
    int num_threads_level[3] = {};

    #pragma omp parallel num_threads(2)
    {
        #pragma omp atomic
        num_threads_level[0]++;
        printf("level 1: thread %d: num threads on this level - %d\n", omp_get_thread_num(), num_threads_level[0]);

        #pragma omp parallel num_threads(2)
        {
            #pragma omp atomic
            num_threads_level[1]++;
            printf("level 2: thread %d: num threads on this level - %d\n", omp_get_thread_num(), num_threads_level[1]);

            #pragma omp parallel num_threads(2)
            {
                #pragma omp atomic
                num_threads_level[2]++;
                printf("level 3: thread %d: num threads on this level - %d\n", omp_get_thread_num(), num_threads_level[2]);
            }
        }
    }

    printf("\nTotal threads on each level:\n");
    printf("level 1: %d\n", num_threads_level[0]);
    printf("level 2: %d\n", num_threads_level[1]);
    printf("level 3: %d\n", num_threads_level[2]);

    return 0;
}