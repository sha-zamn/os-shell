#include <omp.h>
#include <stdio.h>
#include <unistd.h>

#define THREADS 16
#define N 100

int main() {

        #pragma omp parallel for schedule(static) num_threads(THREADS)
        for (int i = 0; i < N; i++) { 
                sleep(i);
                printf("Thread %d is executing iteration %d.\n", omp_get_thread_num(), i);
        }
return 0;
}
