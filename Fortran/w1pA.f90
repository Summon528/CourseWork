program pa
    implicit none
    integer :: a, b, c, d 
    open (87,file="in.txt",status = "old")
    open (88,file="out.txt",status = "replace")
    read (87,*) a,b,c
    d = a*3600+b*60+c
    write (88,'(2(I0.2, 1X), I0.2, A, I0, A, F4.2, A)') a,b,c," is ",d, " seconds, or ", d/86400.0, " days"
end program pa
