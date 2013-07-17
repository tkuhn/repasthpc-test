#!/bin/bash

if [ "$INCLUDE" = "" ]; then
  INCLUDE="-I/usr/include -I/usr/local/include"
fi

if [ "$LIB" = "" ]; then
  LIB="-L/usr/lib -L/usr/local/lib"
fi

if [ "$ARGS" = "" ]; then
  ARGS="-lmpi_cxx -lrepast_hpc-1.0.1 -ldl -lboost_mpi -lboost_serialization -lnetcdf_c++ -lboost_system -lboost_filesystem -lnetcdf -lmpi -lm"
fi

mkdir -p bin

mpicxx -c $INCLUDE src/fooagent.cpp -o src/fooagent.o
mpicxx -c $INCLUDE src/foomodel.cpp -o src/foomodel.o
mpicxx -c $INCLUDE src/foo.cpp -o src/foo.o
mpicxx $LIB src/*.o $ARGS -o bin/foo
