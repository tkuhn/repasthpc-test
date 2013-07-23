#!/bin/bash

mkdir -p logs

DIVX=`cat conf/model.props | egrep '^division.x = ' | sed 's/^division.x = //'`
DIVY=`cat conf/model.props | egrep '^division.y = ' | sed 's/^division.y = //'`
N=$(($DIVX*$DIVY))

echo "Running on $N nodes"

mpirun -n $N bin/foo conf/config.props conf/model.props
