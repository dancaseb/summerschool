#include "matrix.hpp"

struct Field<DEVICE_ONLY> : FieldInfo {

    Matrix<double, DEVICE_ONLY> temperature;

    void setup(int nx_in, int ny_in, ParallelData parallel);

    void generate(ParallelData parallel);

    // standard (i,j) syntax for setting elements
    double& operator()(int i, int j) {return temperature(i, j);}

    // standard (i,j) syntax for getting elements
    const double& operator()(int i, int j) const {return temperature(i, j);}

};

// Function declarations
void initialize(int argc, char *argv[], Field<DEVICE_ONLY>& current,
                Field<DEVICE_ONLY>& previous, int& nsteps, ParallelData parallel);

void exchange(Field<DEVICE_ONLY>& field, const ParallelData parallel);

template <storage_spec DEVICE_ONLY>
void evolve(Field<DEVICE_ONLY>& curr, const Field<DEVICE_ONLY>& prev, const double a, const double dt);

template <storage_spec DEVICE_ONLY>
void write_field(const Field<DEVICE_ONLY>& field, const int iter, const ParallelData parallel);

template <storage_spec DEVICE_ONLY>
void read_field(Field<DEVICE_ONLY>& field, std::string filename,
                const ParallelData parallel);

template <storage_spec DEVICE_ONLY>
double average(const Field<DEVICE_ONLY>& field, const ParallelData parallel);
