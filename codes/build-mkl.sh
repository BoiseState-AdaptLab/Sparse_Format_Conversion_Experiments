#!/bin/bash
source  /opt/intel/compilers_and_libraries_2018.2.199/linux/mkl/bin/mklvars.sh ia32
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${MKLROOT}/lib/intel64
make
