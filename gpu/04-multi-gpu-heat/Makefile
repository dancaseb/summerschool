ifeq ($(COMP),)
COMP=lumi
endif

COMMONDIR=../../common

ifeq ($(COMP),gnu)
CXX=mpicxx
CC=gcc
CCFLAGS=-g -O3 -march=native -Wall -I$(COMMONDIR) -DNDEBUG
LDFLAGS=
LIBS=-lpng
endif

ifeq ($(COMP),intel)
CXX=mpicxx
CC=icc
CCFLAGS=-g -O3 -xHost -I$(COMMONDIR) -DNDEBUG
LDFLAGS=
LIBS=-lpng
endif

ifeq ($(COMP),lumi)
LIBPNG_DIR=/appl/lumi/SW/LUMI-22.12/C/EB/libpng/1.6.38-cpeCray-22.12
CXX=CC
CC=cc
CCFLAGS=-O3 -Wall -I$(COMMONDIR) -I$(LIBPNG_DIR)/include
LDFLAGS=-L$(LIBPNG_DIR)/lib -Wl,-rpath=$(LIBPNG_DIR)/lib
LIBS=-lpng
endif

ifeq ($(COMP),mahti)
LIBPNG_DIR=./libpng/usr
HOP_ROOT=./hop
CXX=mpiCC
CC=mpicc
CUCC=nvcc
CCFLAGS=-g -DUSE_GPU -x cu -O3 -I$(COMMONDIR) -I$(LIBPNG_DIR)/include/libpng16 -I$(HOP_ROOT) -I$(HOP_ROOT)/source/hip -DHOP_TARGET_CUDA
CFLAGS=-g -O3 -Wall -I$(COMMONDIR) -I$(LIBPNG_DIR)/include/libpng16 -I$(HOP_ROOT) -I$(HOP_ROOT)/source/hip -DHOP_TARGET_CUDA
CUFLAGS=-g -DUSE_GPU -O3 -Xcompiler -Wall -I$(COMMONDIR) -I$(LIBPNG_DIR)/include/libpng16 -I$(HOP_ROOT) -I$(HOP_ROOT)/source/hip -DHOP_TARGET_CUDA
LDFLAGS=-cuda -L$(LIBPNG_DIR)/lib64 -Wl,-rpath=$(LIBPNG_DIR)/lib
LIBS=-lpng
endif


EXE=heat_mpi
#OBJS=main.o heat.o core.o setup.o io.o utilities.o pngwriter.o heat-gpu.o
OBJS=main.o heat.o io.o utilities.o pngwriter.o heat-gpu.o


all: $(EXE)

utilities.o: utilities.cpp heat.hpp
heat.o: heat.cpp heat.hpp matrix.hpp
setup.o: setup.cpp heat.hpp
io.o: io.cpp heat.hpp matrix.hpp
main.o: main.cpp heat.hpp
pngwriter.o: pngwriter.c
heat-gpu.o: heat-gpu.cu heat-gpu.hpp

$(EXE): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS) $(LIBS)


%.o: $(COMMONDIR)/%.c
	$(CC) $(CCFLAGS) -c $< -o $@

%.o: %.cpp
	$(CXX) $(CCFLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.cu
	$(CUCC) $(CUFLAGS) -c $< -o $@

.PHONY: clean
clean:
	-/bin/rm -f $(EXE) $(OBJS) a.out *.o *.png *~
