// SPDX-FileCopyrightText: 2010 CSC - IT Center for Science Ltd. <www.csc.fi>
//
// SPDX-License-Identifier: MIT

#include <cstdio>
#include <omp.h>

int main()
{
    printf("Hello world!\n");

#ifdef _OPENMP
    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        int total_threads = omp_get_num_threads();
        #pragma omp single
        printf("Total number of threads: %d\n", total_threads);

        printf("Hello from thread %d!\n", tid);
    }
#else
    printf("Hello from thread!\n");
#endif

    return 0;
}
