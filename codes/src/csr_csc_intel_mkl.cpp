#include <mkl.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sys/time.h>          
#include <stdlib.h>
#include <assert.h>
#include <sstream>
using namespace std;
struct coo_d {
   float * vals;
   int * cols;
   int * rows;
   int nnz;
   int nr;
   int nc;
};

struct csr_d{
   float *vals;
   int * cols;
   int * rptr;
   int nnz;
};


struct csc_d{
   float *vals;
   int * rows;
   int * cptr;
   int nnz;
};

void read_sparse_coo (const std::string&, coo_d&);

std::vector<std::string> split(const std::string &s, char delim);


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
	csr. cols = new int(coo.nnz);

	csr. vals = new float(coo.nnz);
	csr. rptr = new int (coo.nr +1 );
	csr.nnz = coo.nnz;
	cerr << "finished reading data to memory:\n\n";
        cerr << "converting coo to csr\n\n";
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
        assert(res1==1 && "something happened during conversion aborting");
        // CSC data structure
	csc_d csc;
	csc.rows = new int(coo.nnz);

	csc.vals = new float(coo.nnz);
	csc.cptr = new int (coo.nr +1 );
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
	   assert(res==1 && "something happened during conversion aborting");

           elapsed += (t1.tv_sec - t0.tv_sec) * 1000.0f + (t1.tv_usec - t0.tv_usec) / 1000.0f;
	}
	printf("%f",elapsed/count);
}



void read_sparse_coo (const std::string& filename, 
		coo_d& coo_data){
	bool past_comments = false;
	int i = 0;
	std::string buffer;
	ifstream mat_d (filename);
	if (mat_d){
	  while (getline(mat_d,buffer)){
	     if (buffer[0]!='%'){
	       vector<std::string>elems = split(buffer,' ');
               if(!past_comments){
		 coo_data.nr = stoi(elems.at(0));
		 coo_data.nc = stoi(elems.at(1));
		 coo_data.nnz = stoi(elems.at(2));
		 coo_data.rows = new int(coo_data.nnz);
		 coo_data.cols = new int (coo_data.nnz);
		 coo_data.vals = new float (coo_data.nnz);
		 past_comments = true;
	       }else{
                 coo_data.rows[i] = stoi(elems.at(0));
		 coo_data.cols[i] = stoi(elems.at(1));
		 coo_data.vals[i] = stoi(elems.at(2));
		 i++;
       }
	     }
	  }
	}
 	
}


std::vector<std::string> split(const std::string &s, char delim) {
  std::stringstream ss(s);
  std::string item;
  std::vector<std::string> elems;
  while (std::getline(ss, item, delim)) {
    elems.push_back(std::move(item));
  }
  return elems;
}
