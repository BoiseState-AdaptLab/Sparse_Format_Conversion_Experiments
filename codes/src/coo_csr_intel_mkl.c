#include <mkl.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>          
#include <stdlib.h>
#include <assert.h>
typedef struct coo_d {
   float * vals;
   int * cols;
   int * rows;
   int nnz;
   int nr;
   int nc;
} coo_d;

typedef struct csr_d{
   float *vals;
   int * cols;
   int * rptr;
   int nnz;
} csr_d;

void read_sparse_coo (const char *, coo_d *);

void split(char *, char*,char* [3] );


int main (int argc,char **argv){
	char * file = argv[1];
	int count = atoi(argv[2]);
        float elapsed =0;           
	MKL_INT job [6] = 
	{1 /*coo.csr*/,
	       	0 /* zero based indexing CSR*/,
	       	0/*zero based indexing COO*/,
		0,
		0,/*max number of non zero elements allowed*/
		1, /*fill up acsr, ja and ia when converting to CSR*/
	};
	coo_d coo;
	fprintf(stderr, "reading data to memory:\n\n");
        read_sparse_coo(file,&coo);
	MKL_INT dimension [2] = { coo.nr /*row count*/,
				  coo.nc /*col count*/ };
	csr_d csr;
	csr. cols = (int *) malloc (sizeof(*csr.cols) * coo.nnz); 
	csr. vals = (float *) malloc (sizeof(*csr.vals) * coo.nnz);
	csr. rptr = (int *) malloc (sizeof(*csr.rptr)
		       	* (coo.nr +1));
        csr.nnz = coo.nnz;
	fprintf(stderr,"finished reading data to memory:\n\n");
	//time conversion
	for (int i = 0; i <count ;i++){
           struct timeval t0;
           struct timeval t1;
   

           gettimeofday(&t0, 0);  
	   MKL_INT res;
           mkl_scsrcoo(job 
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
			    &res);
           gettimeofday(&t1, 0);
	   assert(res==1 && "something happened during conversion aborting");

           elapsed += (t1.tv_sec - t0.tv_sec) * 1000.0f +
		   (t1.tv_usec - t0.tv_usec) / 1000.0f;
	}
	printf("%f",elapsed/count);
}



void read_sparse_coo (const char * filename, 
		coo_d * coo_data){
	int past_comments = 0;
	int i = 0;
	char buffer [1024];
	FILE * mat_d = fopen (filename,"r");
	if (mat_d){
	  while (fgets(buffer,sizeof(buffer),mat_d)){
	     // takes out comments from the 
	     // file
             if (buffer[0]!='%'){
	       char * elems[3];
	       split(buffer," ",elems);
               if(!past_comments){
		 coo_data->nr = atoi(elems[0]);
		 coo_data->nc = atoi(elems[1]);
		 coo_data->nnz = atoi(elems[2]);
		 coo_data->rows = (int *) malloc(coo_data->nnz* sizeof(int));
		 coo_data->cols = (int*) malloc (coo_data->nnz * sizeof(int));
		 coo_data->vals = (float*) malloc (coo_data->nnz * sizeof(float));
		 past_comments = 1;
	       }else{
                 coo_data->rows[i] = atoi(elems[0]);
		 coo_data->cols[i] = atoi(elems[1]);
		 coo_data->vals[i] = atoi(elems[2]);
		 i++;
	       }
	     }
	  }
	}
 	
}


void split(char *s, char* delim, char* result [3]) {
   char * tok = strtok(s,delim);
   int i = 0;
   while (tok!=NULL){
      printf("%s \n", tok);
      result[i] = tok;
      strtok(NULL,delim);
      i++;
   }
}
