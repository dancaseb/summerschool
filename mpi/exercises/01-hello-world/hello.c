/*
 * SPDX-FileCopyrightText: 2010 CSC - IT Center for Science Ltd. <www.csc.fi>
 *
 * SPDX-License-Identifier: MIT
 */

#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {

    // TODO: say hello! in parallel
    int size, rank;
    char proc_name[30];
    int name_len;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    printf("My rank is %d\n", rank);

    if (rank == 0) {
        printf("Total number of processes is %d\n", size);
    }

    if (rank == size - 1) {
        printf("I'm the last but not least\n");
    }

    if (rank == 42) {
        printf("I'm the answer to life, the universe and everything\n");
    }

    printf("Hello!\n");

    MPI_Get_processor_name(proc_name, &name_len);
    printf("Process name is %s\n", proc_name);

    MPI_Finalize();
}
