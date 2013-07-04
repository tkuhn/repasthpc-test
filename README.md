repasthpc-test
==============

This is some code to test the Repast HPC:
http://repast.sourceforge.net/repast_hpc.html


Setup
-----

In addition to `cpp` and `make`, you need to install Boost and NetCDF. For
Ubuntu:

    $ sudo apt-get install libboost-all-dev
    $ sudo apt-get install libnetcdf-dev

Then install Repast HPC according to its installation instructions. Running
`make` for version 1.0.1 might result in an a number of errors (using Ubuntu
13.04 and possibly other operating systems).

getItems error:

    error: 'getItems' was not declared in this scope [...]

To resolve this, replace `getItems(` by `this->getItems(` in the following two
files:

    src/repast_hpc/DirectedVertex.h
    src/repast_hpc/UndirectedVertex.h

zombie and rumor errors:

    zombie_model-Human.o: undefined reference to symbol [...]
    rumor_model-RumorModel.o: undefined reference to symbol [...]

This is resolved by adding ` $(BOOST_SYSTEM_LIBS)` to the end of the line
starting with `zombie_model_LDADD = ` in the file `src/zombie/Makefile.in` and
to the end of the line starting with `rumor_model_LDADD = ` in the file
`src/rumor/Makefile.in`.


Status
------

The current code compiles, but does not do anything yet...
