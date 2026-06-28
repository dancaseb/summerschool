// SPDX-FileCopyrightText: 2010 CSC - IT Center for Science Ltd. <www.csc.fi>
//
// SPDX-License-Identifier: MIT

#include <cstdio>
#include <cstdlib>
#include <mpi.h>
#include <omp.h>

int main(int argc, char *argv[])
{
    int rank, ntasks;
    int required = MPI_THREAD_MULTIPLE;
    int provided;
    MPI_Init_thread(&argc, &argv, required, &provided);

    if (provided < required)
    {
        printf("MPI does not support required thread support level\n");
        MPI_Abort(MPI_COMM_WORLD, -1);
        return 0;
    }

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &ntasks);

    int tid = 0;
    int msg = -1;
    int tag = 123;
    int color;
    MPI_Comm sub_comm;
    int sub_rank;
    MPI_Comm *sub_comms;
    sub_comms = (MPI_Comm *)malloc(ntasks * sizeof(MPI_Comm));
    int i, j;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_dup(MPI_COMM_WORLD, &sub_comm);
    MPI_Comm_rank(sub_comm, &sub_rank);
    sub_comms[rank] = sub_comm;
    int thread_recv = -1;
    // use duplicate communicator -- create new mpi worlds, save into array and then I can loop them
#pragma omp parallel private(color, sub_comm, sub_rank) shared(sub_comms, rank, ntasks, thread_recv)
    {


        int total_threads = omp_get_num_threads();
        if (rank == 0)
        {
            #pragma omp for private(i,j)
            for (int i = 0; i < total_threads; i++)
            {
                int thread_id = omp_get_thread_num();
                for (int j = 1; j < ntasks; j++)
                {
                    MPI_Send(&thread_id, 1, MPI_INT, j, thread_id, sub_comms[rank]);
                }
            }
        }
        else
        {
           for (int i = 0; i < total_threads; i++){
                int thread_id = omp_get_thread_num();
                MPI_Recv(&thread_recv, 1, MPI_INT, 0, thread_id, sub_comms[rank], MPI_STATUS_IGNORE);
           } 
        }
    }

    MPI_Finalize();
    return 0;
}
