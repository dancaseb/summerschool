// SPDX-FileCopyrightText: 2019 CSC - IT Center for Science Ltd. <www.csc.fi>
//
// SPDX-License-Identifier: MIT

#include <cstdio>
#include <cmath>
#include <mpi.h>

constexpr int n = 10;

int main(int argc, char** argv)
{

  printf("Computing approximation to pi with N=%d\n", n);

  int istart;
  int istop = n;
  double total_sum = 0.0;
  double pi = 0.0;
  double temp_sum = 0.0;

  int rank, size;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

//   if (rank == 0){
//     istart = 1;
//     istop = n/2;
//   }
//   if (rank == 1){
//     istart = n/2 + 1;
//     istop = n;
//   }
  int base, rem;
  base = n / size;
  rem = n % size;
  int count = base + (rank < rem ? 1 : 0);
  istart = rank * base + (rank < rem ? rank : rem) + 1;
  istop = istart + count - 1 ;
  printf("Rank %d: istart=%d, istop=%d\n", rank, istart, istop);

  for (int i=istart; i <= istop; i++) {
    double x = (i - 0.5) / n;
    pi += 1.0 / (1.0 + x*x);
  }

  if (rank > 0){
    MPI_Send(&pi, 1, MPI_DOUBLE, 0, rank, MPI_COMM_WORLD );
  }

  if (rank == 0){
    total_sum = pi;
    for (int i=1; i < size; i++){
        MPI_Recv(&temp_sum, 1, MPI_DOUBLE, i, i, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        total_sum += temp_sum;
    }
    total_sum *= 4.0 / n;
    printf("Approximate pi=%18.16f (exact pi=%10.8f)\n", total_sum, M_PI);
  }

  MPI_Finalize();
}
