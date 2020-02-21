/**
CSC to DIA
There is no direct conversion from 
CSC to Dia in intel MKL.
CSC -> CSR
then CSR -> DIA
**/
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
	MKL_INT dimension [2] = { coo.nr /*row count*/,
				  coo.nc /*col count*/ };
	csr_d csr;
	csr. cols = new int[coo.nnz];

	csr. vals = new float[coo.nnz];
	csr. rptr = new int [coo.nr +1 ];
	csr.nnz = coo.nnz;
	cerr << "finished reading data to memory:\n\n";
        cerr << "converting csC to dia\n\n";
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
        // CSC data structure
	
        // convert csr to csc first
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

	   assert(res==0 && "something happened during conversion aborting");

        
        // performing conversion
	MKL_INT csr_dia_job [6] = 
	{0 /*csr->dia*/,
	       	0 /* zero based indexing CSR*/,
	       	0/*zero based indexing COO*/,
		0,
		0,/*max number of non zero elements allowed*/
		10, /*diagonals are selected internally, and csr_rem, ja_rem,
			 ia_rem are NOT FILLED for the output storage.*/
	};
        
	//timing
	for (int i = 0; i <count ;i++){
           struct timeval t0;
           struct timeval t1;
   

           gettimeofday(&t0, 0);
	   //timing CSC -> CSR
           csr_d rem_csr;
	   rem_csr.cols = new int[coo.nnz];

	   rem_csr.vals = new float[coo.nnz];
	   rem_csr.rptr = new int [coo.nr +1 ];
	   rem_csr.nnz = coo.nnz;
	   
           MKL_INT csc_csr_job [6] = 
		{1 /*csr->csc*/,
	       		0 /* zero based indexing CSR*/,
	       		0/*zero based indexing COO*/,
			0,
			0,/*max number of non zero elements allowed*/
			1, /*fill up acsr, ja and ia when converting to CSR*/
		};
           
	   MKL_INT res_c;
	   mkl_scsrcsc(csc_csr_job 
	   	       /*job instructions check documentation for 
	                          specification 
			   https://software.intel.com/en-us/mkl-developer-reference-c-mkl-csrcsc */
		       ,dimension
		       /*dimension pointer for the matrix, seems to require
		        * square matrix. any way i placed both the row and col
		        * in an array structure*/
		       ,
		       rem_csr.vals /*output: result of the nnz csr vector*/,
		       (MKL_INT *) rem_csr.cols, /*output: col component of csr.sorted*/
		       (MKL_INT *) rem_csr.rptr, /*output: row ptr component of csr*/
		       csc.vals, /*input: non zero csr values of matrix*/
		       csc.rows, /*input: row indices*/
		       csc.cptr, /*input: col ptr component of csc*/
		       &res_c);

	   assert(res_c==0 && "something happened during conversion aborting");
        dia_d dia;
        dia.ndiag = 0;
        // We need to count the number of diagonals with NNZ
        //code extracted from: http://berenger.eu/blog/cc-sparse-mkl-examples-c00-csr-dia-bcsr-gemv-and-conversions/ 
       {
        unsigned* usedDiag = new unsigned[coo.nr*2-1];
        memset(usedDiag, 0, sizeof(unsigned)*(coo.nr*2-1));
 
        for(int idxRow = 0 ; idxRow < coo.nr ; ++idxRow){
            for(int idxVal = csr.rptr[idxRow] ; idxVal < csr.rptr[idxRow+1] ; ++idxVal){
                const int idxCol = csr.cols[idxVal];
                const int diag = coo.nr-idxRow+idxCol-1;
                assert(0 <= diag && diag < coo.nr*2-1);
                if(usedDiag[diag] == 0){
                    usedDiag[diag] = 1;
                    dia.ndiag += 1;
                }
            }
        }
 
        delete[] usedDiag;
        }
	unsigned long ndiag = dia.ndiag;
	unsigned long dim = coo.nr;
	unsigned long long val_size = ndiag * dim;
        dia.vals = new float[val_size];
        dia.idiag = new int[dia.ndiag];
	dia.offsets = new int [dia.ndiag];
	   mkl_scsrdia(csr_dia_job 
	   	       /*job instructions check documentation for 
	                          specification 
			   https://software.intel.com/en-us/mkl-developer-reference-c-mkl-csrdia */
		       ,dimension
		       /*dimension pointer for the matrix, seems to require
		        * square matrix. any way i placed both the row and col
		        * in an array structure*/
		       ,
		       csr.vals /*input: result of the nnz csr vector*/,
		       (MKL_INT *) csr.cols, /*input: col component of csr.sorted*/
		       (MKL_INT *) csr.rptr, /*input: row ptr component of csr*/
		       dia.vals, /*outupt: diagonals of dia*/
		       (MKL_INT *) &dia.ndiag, /*specifies the leading dimension of the array adia*/
		       dia.offsets,
		       (MKL_INT*)dia.idiag, /*output: col ptr component of csc*/
		       rem_csr.vals, /*outupt: non zero csr values of matrix remainder*/
		       rem_csr.cols, /*output: col indices remainder*/
		       rem_csr.rptr, /*output: row ptr component of remainder*/
		       &res_c);

           gettimeofday(&t1, 0);
	   assert(res_c==0 && "something happened during conversion aborting");
	   
           elapsed += (t1.tv_sec - t0.tv_sec) * 1000.0f + (t1.tv_usec - t0.tv_usec) / 1000.0f;
	   delete [] dia.offsets;
	   delete [] dia.idiag;
           delete [] dia.vals;
	   delete [] rem_csr.vals;
           delete [] rem_csr.cols;
           delete [] rem_csr.rptr;
	}
	printf("%f",elapsed/count);
}

