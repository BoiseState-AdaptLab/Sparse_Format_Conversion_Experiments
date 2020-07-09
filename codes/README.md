### OVERVIEW

Code builds format conversion routines using Intel MKL or SparsKit


### Dependencies

Requires Intel-MKL installed with ${MKL_ROOT} path set in environment.

MKL_ROOT can be set by running the following command from mkl install directory

```
mklvars.sh ia32
```


SparseKIT

sparse kit ships with this code so no installation is required. Sparsekit implementation
is written in Fortran and could be used for fortran conversion routines

### Build instructions

```
make

```
