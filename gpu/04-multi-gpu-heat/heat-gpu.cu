#include "heat.hpp"
#include "matrix.hpp"
//#include "../error_checking.hpp"
#include <iostream>
#include "heat-gpu.hpp"
#include <hip/hip_runtime.h>


// Implementations
template<typename T>
__global__ void generate_kernel(MatrixView<T, DEVICE_ONLY> view, int nx_full) {

}

void Field<DEVICE_ONLY>::setup(int nx_in, int ny_in, ParallelData parallel) {
        nx_full = nx_in;
        ny_full = ny_in;

        nx = nx_full / parallel.size;
        if (nx * parallel.size != nx_full) {
                std::cout << "Cannot divide grid evenly to processors" << std::endl;
                MPI_Abort(MPI_COMM_WORLD, -2);
        }
        ny = ny_full;

        // matrix includes also ghost layers
        temperature = Matrix<double, DEVICE_ONLY> (nx + 2, ny + 2);
}

void Field<DEVICE_ONLY>::generate(ParallelData parallel) {
        constexpr size_t threads_per_block = 1024;
        dim3 blocks((nx*ny+threads_per_block-1)/threads_per_block, 1, 1);
        dim3 threads(threads_per_block, 1, 1);
        generate_kernel<<<blocks,threads,0,0>>>(temperature.view(), nx_full);
        HIP_ERRCHK( hipGetLastError());
}

