#pragma once
#include <string>
#include "matrix.hpp"
#include <mpi.h>
#include <iostream>
#include <utility>

// Class for basic parallelization information
struct ParallelData {
    int size;            // Number of MPI tasks
    int rank;
    int nup, ndown;      // Ranks of neighbouring MPI tasks

    ParallelData() {      // Constructor

      MPI_Comm_size(MPI_COMM_WORLD, &size);
      MPI_Comm_rank(MPI_COMM_WORLD, &rank);

      nup = rank - 1;
      ndown = rank + 1;

      if (nup < 0) {
        nup = MPI_PROC_NULL;
      }
      if (ndown > size - 1) {
        ndown = MPI_PROC_NULL;
      }
    };

};

struct FieldInfo {
    // nx and ny are the true dimensions of the field. The temperature matrix
    // contains also ghost layers, so it will have dimensions nx+2 x ny+2
    int nx;                     // Local dimensions of the field
    int ny;
    int nx_full;                // Global dimensions of the field
    int ny_full;                // Global dimensions of the field
    double dx = 0.01;           // Grid spacing
    double dy = 0.01;
};

// Class for temperature field
template <storage_spec mem_location>
struct Field : FieldInfo{};

template<>
struct Field<HOST_ONLY> : FieldInfo
{
        Matrix<double, HOST_ONLY> temperature;

        Field<HOST_ONLY>();

        friend void swap(Field<HOST_ONLY>& a, Field<HOST_ONLY>& b) {
                using std::swap;
                swap(static_cast<FieldInfo&>(a), static_cast<FieldInfo&>(b));
                swap(a.temperature, b.temperature);
        };

        void setup(int nx_in, int ny_in, ParallelData parallel);

        void generate(ParallelData parallel);

        // standard (i,j) syntax for setting elements
        double& operator()(int i, int j) {return temperature(i, j);}

        // standard (i,j) syntax for getting elements
        const double& operator()(int i, int j) const {return temperature(i, j);}
};

// Function declarations
void initialize(int argc, char *argv[], Field<HOST_ONLY>& current,
                Field<HOST_ONLY>& previous, int& nsteps, ParallelData parallel);

void exchange(Field<HOST_ONLY>& field, const ParallelData parallel);

void evolve(Field<HOST_ONLY>& curr, const Field<HOST_ONLY>& prev, const double a, const double dt);

void write_field(const Field<HOST_ONLY>& field, const int iter, const ParallelData parallel);

void read_field(Field<HOST_ONLY>& field, std::string filename,
                const ParallelData parallel);

double average(const Field<HOST_ONLY>& field, const ParallelData parallel);

double average(const Field<HOST_ONLY>& field, const ParallelData parallel);
