c library for sparse routines
      subroutine read_coo_f(filename,row,col,vals,nr,nc,nnz)
         integer nr,nc,nnz,readflag,first
         integer i,u,res
         integer, intent(out):: row(*), col(*)
         character comment
         real (8), intent(out):: vals(*)
         parameter(comment = '%')
         character(250) string_buff
         
         character(len=80)::elems(3)
         character, intent(in):: filename * 250
         readflag = 0
         first=0
         i = 1
         open(u,FILE=filename,STATUS='OLD',IOSTAT=res)
         if (res /= 0) then
            write(0,*) 'Error opening file, status: ',res
            stop
         end if
         do while (readflag.eq.0) 
            read(u,'(A)',IOSTAT=readflag) string_buff
            if (string_buff(1:1) .ne. '%') then
c               call splitstr(string_buff,' ',elems)
               read(string_buff,*) elems(1:3)
               if(first .eq. 0 ) then
c                extract nr, nc, nnz information
                 read(elems(1),*) nr
                 read(elems(2),*) nc
                 read(elems(3),*) nnz
                 first = 1
               else
                 read(elems(1),*) row(i)
                 read(elems(2),*) col(i)
                 read(elems(3),*) vals(i)
                 i = i + 1
               end if               
            end if
         end do   
         close(u) 
         return
      end
      
        
