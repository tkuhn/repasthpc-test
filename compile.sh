#!/bin/bash

mkdir -p bin

mpicxx -c -I/usr/include -I/usr/local/include src/fooagent.cpp -o src/fooagent.o
mpicxx -c -I/usr/include -I/usr/local/include src/foomodel.cpp -o src/foomodel.o
mpicxx -c -I/usr/include -I/usr/local/include src/foo.cpp -o src/foo.o
mpicxx -L/usr/lib -L/usr/local/lib src/*.o \
  -lmpi_cxx -lrepast_hpc-1.0.1 -ldl -lboost_mpi-mt -lboost_serialization-mt \
  -lnetcdf_c++ -lboost_system-mt -lnetcdf -lmpi -lm \
  -o bin/foo
