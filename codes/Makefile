	CC = g++
#-------Define names of all the object files in this project
	INTELOBJS = intel-mkl.o
	BIN = bin
	LIB =lib
	OBJ = obj
        
	SRC = src
#-------Define the name of the resulting released product
	INTEL = intel-mkl

#-------Define options passed by make to the compiler
	CFLAGS = -std=c++11 -g
	
	FFLAGS = -std=f2003  
#-------Setup tags for source instruction for mkl library mkl must be installed	
	MKLSRCTAGS =  /opt/intel/compilers_and_libraries_2018.2.199/linux/mkl/bin/mklvars.sh ia32
	SPARSKIT= $(LIB)/SPARSKIT2
	LFFLAGS = -L$(SPARSKIT)/ -L$(LIB)/

#-------Define target "all" for building the executable(s)
#-------Include the rules for rebuilding each *.o file
all:    setup coo_csr_intel_mkl csr_csc_intel_mkl  csr_dia_intel_mkl coo_csr_sparskit csc_dia_intel_mkl

rules.d: Makefile $(wildcard $(SRC)/*.cpp) $(wildcard *.h)
	g++ -MM $(wildcard $(SRC)/*.cpp) >rules.d

-include rules.d

coo_csr_intel_mkl:
	g++ $(CFLAGS) $(SRC)/coo_csr_intel_mkl.cpp $(SRC)/sparse_format.cpp \
		 -L${MKLROOT}/lib/intel64 -I${MKLROOT}/include $(SRC)/sparse_format.h \
	       	-Wl,--no-as-needed\
	       	-lmkl_intel_lp64 -lmkl_intel_thread -lmkl_core \
	       	-liomp5 -lpthread -lm -ldl -o $(BIN)/$@
csr_csc_intel_mkl:
	g++ $(CFLAGS) $(SRC)/csr_csc_intel_mkl.cpp  $(SRC)/sparse_format.cpp \
		 -L${MKLROOT}/lib/intel64 -I${MKLROOT}/include $(SRC)/sparse_format.h \
	       	-Wl,--no-as-needed\
	       	-lmkl_intel_lp64 -lmkl_intel_thread -lmkl_core \
	       	-liomp5 -lpthread -lm -ldl -o $(BIN)/$@
csr_dia_intel_mkl:
	g++ $(CFLAGS) $(SRC)/csr_dia_intel_mkl.cpp $(SRC)/sparse_format.cpp \
		 -L${MKLROOT}/lib/intel64 -I${MKLROOT}/include $(SRC)/sparse_format.h \
	       	-Wl,--no-as-needed\
	       	-lmkl_intel_lp64 -lmkl_intel_thread -lmkl_core \
	       	-liomp5 -lpthread -lm -ldl -o $(BIN)/$@
csc_dia_intel_mkl:
	g++ $(CFLAGS) $(SRC)/csc_dia_intel_mkl.cpp $(SRC)/sparse_format.cpp \
		 -L${MKLROOT}/lib/intel64 -I${MKLROOT}/include $(SRC)/sparse_format.h \
	       	-Wl,--no-as-needed\
	       	-lmkl_intel_lp64 -lmkl_intel_thread -lmkl_core \
	       	-liomp5 -lpthread -lm -ldl -o $(BIN)/$@
libsparse.a:
	gfortran $(SRC)/sparse.f -c -o $(OBJ)/sparse.o
	ar rcv $(LIB)/$@ $(OBJ)/sparse.o
	ranlib $(LIB)/$@
 
coo_csr_sparskit:libsparse.a
	gfortran $(FFLAGS) $(SRC)/coo_csr_sparskit.f\
                 $(LFFLAGS) -lsparse -lskit -o $(BIN)/$@ 

csr_csc_sparskit:libsparse.a
	gfortran $(FFLAGS) $(SRC)/csr_csc_sparskit.f\
                $(LFFLAGS) -lsparse -lskit -o $(BIN)/$@ 
setup:
	mkdir -p $(BIN) $(OBJ)

clean:
	rm -f $(LIB)/libsparse.a  $(OBJ)/* $(BIN)/*  *.d  *~

