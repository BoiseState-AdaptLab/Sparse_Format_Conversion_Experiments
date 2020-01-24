	CC = g++
#-------Define names of all the object files in this project
	INTELOBJS = intel-mkl.o

#-------Define the name of the resulting released product
	INTEL = intel-mkl

#-------Define options passed by make to the compiler
	CFLAGS = -Wall -std=c++20
	
#-------Setup tags for source instruction for mkl library	
	MKLSRCTAGS =  /opt/intel/compilers_and_libraries_2018.2.199/linux/mkl/bin/mklvars.sh ia32

#-------Define target "all" for building the executable(s)
#-------Include the rules for rebuilding each *.o file
all:    coo_csr_intel_mkl 

rules.d: Makefile $(wildcard *.cpp) $(wildcard *.h)
	g++ -MM $(wildcard *.cpp) >rules.d

-include rules.d

coo_csr_intel_mkl:
	g++ $(CFLAGS) -m32 coo_csr_intel_mkl.cpp -lmkl_intel_thread \
	       	-lmkl_core -liomp5 -lpthread -lm -o $@


clean:
	rm -f $(OBJS) $(EXE) *.d  *~

