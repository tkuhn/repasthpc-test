#!/bin/bash

mkdir -p bin

# compile Test.cpp
mpicxx -c -I/usr/include -I/usr/local/include src/Test.cpp -o src/Test.o
mpicxx -L/usr/lib -L/usr/local/lib src/Test.o \
  -lmpi_cxx -lrepast_hpc-1.0.1 -ldl -lboost_mpi-mt -lboost_serialization-mt \
  -lnetcdf_c++ -lboost_system-mt -lnetcdf -lmpi -lm \
  -o bin/test

# compile foo.cpp
mpicxx -c -I/usr/include -I/usr/local/include src/foo.cpp -o src/foo.o
mpicxx -L/usr/lib -L/usr/local/lib src/foo.o \
  -lmpi_cxx -lrepast_hpc-1.0.1 -ldl -lboost_mpi-mt -lboost_serialization-mt \
  -lnetcdf_c++ -lboost_system-mt -lnetcdf -lmpi -lm \
  -o bin/foo
