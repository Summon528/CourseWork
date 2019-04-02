module para      !星座、月份資料
    implicit none
    character(len=3), dimension(12), parameter :: m_name = (/ 'Jan', 'Feb', 'Mar', 'Apr', 'May', 'Jun', &
                                                 'Jul', 'Aug', 'Sep', 'Oct', 'Nov', 'Dec' /)
    integer, dimension(12), parameter :: days = (/ 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 /)
    integer, dimension(14), parameter :: sign_idx = (/0, 120, 219, 321, 420, 521, 621, 723, 823, 923, 1023, 1122, 1222 , 5000/)
    character(len=12), dimension(13), parameter :: sign_name = (/'Capricornus', 'Aquarius   ', 'Pisces     ', &
                                                                 'Aries      ', 'Taurus     ', 'Gemini     ', &
                                                                 'Cancer     ', 'Leo        ', 'Virgo      ', &
                                                                 'Libra      ', 'Scorpio    ', 'Sagittarius', &
                                                                 'Capricornus' /)

end module para


module s_data       !統計資料
    implicit none
    save
    integer, dimension(13) :: cnt !各星座人數
    integer:: m_error = 0, d_error = 0, date, idx = 1
              !月份錯誤      日期錯誤      日期  總人數
    character(len = 5) month !月份
end module s_data


program HW4
    implicit none
    open(unit=12, file="input.txt", status="old")
    open(unit=13, file="output.txt", status="replace")
    call input
    call final
end program HW4

subroutine input
    use s_data
    implicit none
    integer:: ios
    do   !輸入直到EOF
        read(12,*, iostat = ios) month, date
        if (ios /= 0) exit
        call sovle
    end do


end subroutine input

subroutine sovle
    use s_data
    use para
    implicit none
    logical :: flag
    integer :: i, j, tmp
    character(len=20) :: sign
    flag = .true.
    !month是否在資料中
    do i = 1, 12
        if(month == m_name(i))then
            flag = .false.
            exit
        endif
    end do


    if(flag) then
        m_error = m_error + 1
        sign = "--check month data"
    elseif (days(i) < date) then     !日期是否正確
        d_error = d_error + 1
        sign = "--check day data"
    else
        tmp = 100 * i + date         !尋找正確的星座
        do i = 1, 14
            if (tmp < sign_idx(i)) then
                sign = sign_name(i-1)
                cnt(i-1) = cnt(i-1) + 1
                exit
            endif
        enddo
    endif

    !輸出該筆資料
    write(13,"(I2, '  ', A4, I4, ' ', A)") idx,month,date,sign
    idx = idx + 1

end subroutine sovle

subroutine final    !輸出統計資料
    use para
    use s_data
    implicit none
    integer:: i
    write(13,"(/,'Total = ',I3,/)") idx - 1
    write(13,"(A, A3, I3)") sign_name(1), ' = ', cnt(1)+cnt(13)  !1 13 都存Capricornus人數 最後加總
    do i = 2, 12
        write(13,"(A, A3, I3)") sign_name(i), ' = ', cnt(i)
    enddo
    write(13,"(/,'month errors = ', I3)") m_error
    write(13,"('day errors = ', I3)") d_error

end subroutine
