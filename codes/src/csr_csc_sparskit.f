      program csr_csc
      integer n,num,argc
c    making a constant n because we know max dimension is 1.3M      
      parameter(n= 20000000,argc=2,u=2)
      character filename * 250, arg_buf *250 
      integer nr,nc,nnz,row(n),col(n)
      real (4)::starttime,endtime,elapsed
      real(8) vals(n)
      real(8), allocatable:: csr_vals(:)
      integer, allocatable:: csr_col(:),csr_rpt(:)
      real(8), allocatable:: csc_vals(:)
      integer, allocatable:: csc_row(:),csc_cpt(:)
      

c get command line arguments
      do 10 i=0, argc
c get_command argument from fortran 2003         
         call get_command_argument(i,arg_buf)
         if (i .eq. 1) then
            filename = arg_buf
         else if(i .eq. 2) then
c cast second argument into an integer            
            read(arg_buf, '(I250)') num
         end if
   10 continue
      write(0,*) 'reading coo from file'
      call read_coo_f(filename,row,col,vals,nr,nc,nnz)
      write(0,*) 'done reading coo from file'
      elapsed = 0;
      allocate(csr_col(nnz))
      allocate(csr_rpt(nnz))
      allocate(csr_vals(nnz))
      allocate(csc_vals(nnz))
      allocate(csc_row(nnz))
      allocate(csc_cpt(nnz))
      call coocsr(nr,nnz,vals,row,col,csr_vals,csr_col,csr_rpt)     
      do 11 i =0,num
         call cpu_time(starttime)
         
         call csrcsc2(nr,1,1,csr_vals,csr_col,csr_rpt,
     csc_row, csc_cpt)
         call cpu_time(endtime)
         elapsed =elapsed + (endtime -  starttime)
   11 continue
      print *,elapsed/num
      stop
      end

