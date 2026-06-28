// SPDX-FileCopyrightText: 2010 CSC - IT Center for Science Ltd. <www.csc.fi>
//
// SPDX-License-Identifier: MIT

#include <cstdio>
#include <omp.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    int rank, size;
    int required = MPI_THREAD_FUNNELED;
    int provided;

    MPI_Init_thread(&argc, &argv, required, &provided);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        printf("MPI size: %d\n", size);
        printf("MPI thread support: %d (required: %d)\n", provided, required);
    }
    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        printf("Hello from thread %d in process %d!\n", tid, rank);
    }

    MPI_Finalize();

    return 0;
}
