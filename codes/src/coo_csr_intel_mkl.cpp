#include <mkl.h>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;
typedef struct coo_d {
   float * vals;
   int * cols;
   int * rows;
   int nnz;
   int nr;
   int nc;
};

int main (int argc,char **argv){
	std::string file = std::string(argv[1]);
	int count = atoi(argv[2]);
	float time [count];
	MKL_INT job [6] = 
	{1 /*coo->csr*/,
	       	0 /* zero based indexing CSR*/,
	       	0/*zero based indexing COO*/,
		0,
		0,/*max number of non zero elements allowed*/
	};
	coo_d coo;
        read_sparse_coo(file,coo);
	
	//time conversion
	for (int i = 0; i <count ;i++){
	}

}

void read_sparse_coo (const std::string& filename, 
		coo_d& coo_data){
	bool past_comments = false;
	FILE *fp;
	fp = fopen(filename,"r");
        int i = 0;
	std::string buffer;
	ifstream mat_d (filename);
	if (mat_d){
	  while (getline(mat_d,buffer)){
	     if (!buffer.starts_with("%")){
                
	     }
	  }
	}
 	
}
