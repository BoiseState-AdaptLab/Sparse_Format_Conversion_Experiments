      program splitstr

      character(len=100) :: string = 'This;is;a test;hello;world!'
      integer   :: n
      character(80), allocatable :: strarray(:)
      n = count(transfer(string, 'a', len(string)) == ";")
      allocate(strarray(n+1))
      read(string, *) strarray(1:n+1) !N+1 because one more parts than semicolon
      print *, 'nvalues=', n+1
      print '(a)', strarray(1:n+1)

      end program

