program pb
    implicit none
    integer :: a
    read *,a
    !print '(4(I0, A))',a," seconds is ",a/3600," hours, ",mod(a,3600)/60," minutes, ", mod(a,60)," seconds"
    print *, a/3600, mod(a,3600)/60, mod(a,60)
end program pb
