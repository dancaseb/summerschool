#pragma once
#include <vector>
#include <cassert>
#include <utility>

#ifdef USE_GPU
#define __HIP_PLATFORM_AMD__
#include <hip/hip_runtime.h>
#include "../error_checking.hpp"
#define DEVICE_CALLABLE __host__ __device__
#else
#define DEVICE_CALLABLE
#endif

// Generic 2D matrix array class.
//
// Internally data is stored in 1D vector but is
// accessed using index function that maps i and j
// indices to an element in the flat data vector.
// Row major storage is used
// For easier usage, we overload parentheses () operator
// for accessing matrix elements in the usual (i,j)
// format.

enum storage_spec {
#ifdef USE_GPU
  DEVICE_ONLY,
#endif
  HOST_ONLY,
};

template<typename T, storage_spec mem_location>
struct MatrixView {
  T* _data;
  const size_t nrows, ncols;
  MatrixView(T* _data, size_t nrows, size_t ncols) : _data(_data), nrows(nrows), ncols(ncols){};
  DEVICE_CALLABLE T& operator()(size_t i, size_t j) {
    return _data[i*ncols+j]; 
  }
};

template <typename T, storage_spec mem_location>
class Allocator;

template <typename T >
struct Allocator<T, HOST_ONLY> {
  static void allocate(T** data, size_t count) { *data = new T[count]; }
  static void free(T* data){ delete[] data; };
};

#ifdef USE_GPU
template <typename T >
struct Allocator<T, DEVICE_ONLY> {
  static void allocate(T** data, size_t count) { HIP_ERRCHK(hipMalloc(data, count*sizeof(T))); }
  static void free(T* data) { HIP_ERRCHK(hipFree(data)); }
};
#endif

template <typename T, storage_spec dest_spec, storage_spec source_spec>
void copymatrix_data(T* dest, const T* source, size_t count) {
  if constexpr (dest_spec != source_spec || source_spec != HOST_ONLY) {
#ifdef USE_GPU
    hipMemcpy(dest, source, count*sizeof(T), hipMemcpyDefault); 
#endif
    return;
  }
  memcpy(dest, source, count*sizeof(T)); 
};


template<typename T, storage_spec mem_location>
class Matrix
{

private:

        // Internal storage
        T* _data = nullptr;

        // Internal 1D indexing (row major)
        DEVICE_CALLABLE int indx(int i, int j) const {
                return i * ncols + j;
        }

        void allocate(){
                Allocator<T, mem_location>::allocate(&this->_data, this->nrows*this->ncols);
        }
        void free() {
                if (_data != nullptr)
                        Allocator<T, mem_location>::free(this->_data);
        }

public:

        size_t nrows, ncols;

        // Create a non-owning view of the data
        MatrixView<T, mem_location> view() const { return MatrixView<T, mem_location>(_data, nrows, ncols); }

        // Make a deep copy from a view
        template <storage_spec view_loc>
        Matrix(MatrixView<T,view_loc> view) : nrows(view.nrows), ncols(view.ncols) {
                copymatrix_data<T, mem_location, view_loc>(_data, view.data, view.nrows*view.ncols);
        }

        // Allocate at the time of construction
        Matrix(size_t nrows, size_t ncols) : nrows(nrows), ncols(ncols) {
                allocate();
        }

        Matrix() : _data(nullptr), nrows(0), ncols(0) {}

        template <storage_spec source_loc>
        void copy(const Matrix<T,source_loc>& other) {
                if(other.nrows*other.ncols != nrows*ncols) {
                        free();
                        nrows = other.nrows;
                        ncols = other.ncols;
                        allocate();
                } else { 
                        nrows = other.nrows; 
                        ncols = other.ncols; 
                }
                copymatrix_data<T, mem_location, source_loc>(_data, other._data, nrows*ncols);
        }

        Matrix& operator=(const Matrix<T,mem_location>& other) = delete;

        Matrix(const Matrix<T,mem_location>& other) = delete; 


        template <storage_spec source_loc>
        Matrix& operator=(Matrix<T, source_loc>&& other) {
                if constexpr(source_loc != mem_location) {
                        this->copy(other);
                        other.nrows = 0;
                        other.ncols = 0;
                        other._data = nullptr;
                        return *this;
                }
                _data = other._data;
                nrows = other.nrows;
                ncols = other.ncols;
                other.nrows = 0;
                other.ncols = 0;
                other._data = nullptr;
                return *this;
        }

        template <storage_spec source_loc>
        Matrix(Matrix<T, source_loc> &&other) {
                *this = std::move(other);
        }

        ~Matrix() {
                if (_data != nullptr) Allocator<T,mem_location>::free(_data);
        }

        friend void swap(Matrix<T, mem_location>& a, Matrix<T, mem_location>& b) {
                using std::swap;
                swap(a._data, b._data);
                swap(a.nrows, b.nrows);
                swap(a.ncols, b.ncols);
        }

        // standard (i,j) syntax for setting elements
        DEVICE_CALLABLE inline T& operator()(int i, int j) {
                return _data[ indx(i, j) ];
        }

        // standard (i,j) syntax for getting elements
        DEVICE_CALLABLE inline const T& operator()(int i, int j) const {
                return _data[ indx(i, j) ];
        }

        // provide possibility to get raw pointer for data at index (i,j) (needed for MPI)
        DEVICE_CALLABLE inline T* data(int i=0, int j=0) {return _data + indx(i,j);}
};

