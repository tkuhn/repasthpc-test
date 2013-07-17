#!/bin/bash

INCLUDE="-I/usr/include -I/usr/local/include"
LIB="-L/usr/lib -L/usr/local/lib"
ARGS="-lmpi_cxx -lrepast_hpc-1.0.1 -ldl -lboost_mpi-mt -lboost_serialization-mt -lnetcdf_c++ -lboost_system-mt -lnetcdf -lmpi -lm"

mkdir -p bin

mpicxx -c $INCLUDE src/fooagent.cpp -o src/fooagent.o
mpicxx -c $INCLUDE src/foomodel.cpp -o src/foomodel.o
mpicxx -c $INCLUDE src/foo.cpp -o src/foo.o
mpicxx $LIB src/*.o $ARGS -o bin/foo
