#include <omp.h>
#include <stdio.h>
#include <unistd.h>

#define THREADS 16
#define N 100

double run_static(int threads) {
    double start = omp_get_wtime();

    #pragma omp parallel for schedule(static) num_threads(THREADS)
    for (int i = 0; i < N; i++) { 
        sleep(i);
        printf("Thread %d is executing iteration %d.\n", omp_get_thread_num(), i);
    }
    return omp_get_wtime() - start;
}

double run_guided(int threads) {
    double start = omp_get_wtime();

    #pragma omp parallel for schedule(guided) num_threads(THREADS)
    for (int i = 0; i < N; i++) { 
        sleep(i);
        printf("Thread %d is executing iteration %d.\n", omp_get_thread_num(), i);
    }
    return omp_get_wtime() - start;
}

int main() {
    int threads;
    double time_static  = run_static(threads);
    printf("Static time: %f\n", time_static);

    double time_dynamic  = run_static(threads);
    printf("Dynamic time: %f\n", time_dynamic);    

    double time_guided  = run_static(threads);
    printf("Guided time: %f\n", time_guided);    

return 0;
}