#include <mkl.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sys/time.h>          
#include <stdlib.h>
#include <assert.h>
#include <sstream>
#include "sparse_format.h"
using namespace std;

int main (int argc,char **argv){
	std::string file = std::string(argv[1]);
	int count = atoi(argv[2]);
        float elapsed =0;           
	MKL_INT coo_csr_job [6] = 
	{1 /*coo.csr*/,
	       	0 /* zero based indexing CSR*/,
	       	0/*zero based indexing COO*/,
		0,
		0,/*max number of non zero elements allowed*/
		1, /*fill up acsr, ja and ia when converting to CSR*/
	};
        
	coo_d coo;
	cerr<< "reading data to memory:\n\n";
        read_sparse_coo(file,coo);
        cout << "format,time";
        cerr << "Performing spmv on coo";
        elapsed = 0;
        int * vector = new int [coo.nr];
	int * res = new int [coo.nr];
        for (int i = 0; i < coo.nr; i++){
	   vector[i] = rand()%100;
	   resi
	}
        for (int i = 0; i < count ; i++ ){
           
        }
	MKL_INT dimension [2] = { coo.nr /*row count*/,
				  coo.nc /*col count*/ };
	csr_d csr;
	csr. cols = new int[coo.nnz];

	csr. vals = new float[coo.nnz];
	csr. rptr = new int [coo.nr +1 ];
	csr.nnz = coo.nnz;
	cerr << "finished reading data to memory:\n\n";
        cerr << "converting csr to csc\n\n";
	MKL_INT res1;
	mkl_scsrcoo(coo_csr_job 
		    /*job instructions check documentation for 
	                       specification 
			https://software.intel.com/en-us/mkl-developer-reference-c-mkl-csrcoo */
		    ,dimension
		    /*dimension pointer for the matrix, seems to require
		     * square matrix. any way i placed both the row and col
		     * in an array structure*/
		    ,
		    csr.vals /*output: result of the nnz csr vector*/,
		    (MKL_INT *) csr.cols, /*output: col component of csr.sorted*/
		    (MKL_INT *) csr.rptr, /*output: row ptr component of csr*/
		    
		    (MKL_INT *)&coo.nnz /*
		    *specifies number of non zero important for conversion from
		    coo.csr but not necessarry for csr.coo
		    * */
		    ,
		    coo.vals, /*input: non zero coo values of matrix*/
		    coo.rows, /*input: row indices*/
		    coo.cols, /*input: col indices*/
		    &res1);
        assert(res1==0 && "something happened during conversion aborting");
        
        cerr << "Performing spmv on csr";
        // CSC data structure
	csc_d csc;
	csc.rows = new int[coo.nnz];

	csc.vals = new float[coo.nnz];
	csc.cptr = new int [coo.nr +1 ];
	csc.nnz = coo.nnz;
	MKL_INT csr_csc_job [6] = 
	{0 /*csr->csc*/,
	       	0 /* zero based indexing CSR*/,
	       	0/*zero based indexing COO*/,
		0,
		0,/*max number of non zero elements allowed*/
		1, /*fill up acsr, ja and ia when converting to CSR*/
	};
        
	//timing
	for (int i = 0; i <count ;i++){
           struct timeval t0;
           struct timeval t1;
   

           gettimeofday(&t0, 0);  
	   MKL_INT res;
	   mkl_scsrcsc(csr_csc_job 
	   	       /*job instructions check documentation for 
	                          specification 
			   https://software.intel.com/en-us/mkl-developer-reference-c-mkl-csrcsc */
		       ,dimension
		       /*dimension pointer for the matrix, seems to require
		        * square matrix. any way i placed both the row and col
		        * in an array structure*/
		       ,
		       csr.vals /*input: result of the nnz csr vector*/,
		       (MKL_INT *) csr.cols, /*input: col component of csr.sorted*/
		       (MKL_INT *) csr.rptr, /*input: row ptr component of csr*/
		       csc.vals, /*outupt: non zero csr values of matrix*/
		       csc.rows, /*output: row indices*/
		       csc.cptr, /*output: col ptr component of csc*/
		       &res);

           gettimeofday(&t1, 0);
	   assert(res==0 && "something happened during conversion aborting");

           elapsed += (t1.tv_sec - t0.tv_sec) * 1000.0f + (t1.tv_usec - t0.tv_usec) / 1000.0f;
	}
	printf("%f",elapsed/count);
}

