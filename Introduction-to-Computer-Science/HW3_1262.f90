
module score      !每筆輸入的資料
    implicit none
    save
    character(len=2) :: cate
    integer :: mode
    logical :: wrong_cate,wrong_mode
    real:: a,b,c,d,e,wei_score
end module score

module total     !統計資料
    implicit none
    save
    integer :: total_number = 0,cA1 = 0,cB2 = 0,cC3 = 0,error_category = 0,error_mode = 0, &
                cA1M1 = 0,cA1M2 = 0,cA1M3 = 0,cB2M1 = 0,cB2M2 = 0,cC3M1 = 0,cC3M2 = 0,cC3M3 = 0,cC3M4 = 0
end module total


program HW3
    use score
    implicit none
    call input
    call finish
end program HW3


subroutine input
    use score
    open(unit=17, file="input.txt", iostat=ios, status="old", action="read")
    open(unit=18, file="out.txt", status="replace")
    do          !輸入直到 EOF
        read(17,*,iostat = istat) cate,mode,a,b,c,d,e
        if ( istat /= 0 ) exit
        call solve
    end do
end subroutine

subroutine solve
    use score
    use total
    wrong_cate = .false.
    wrong_mode = .false.
    !計算加權後成績
    if ( cate ==  "A1") then
        select case (mode)
            case (1)
                wei_score  = a * 1.5 + b * 1.5 + c + d + e
                cA1M1 = cA1M1 + 1
                cA1 = cA1 + 1
            case (2)
                wei_score  = a * 1.75 + b * 2 + c * 1.25 + d * 1.25
                cA1M2 = cA1M2 + 1
                cA1 = cA1 + 1
            case (3)
                wei_score  = a * 2 + b * 2 + c
                cA1M3 = cA1M3 + 1
                cA1 = cA1 + 1
            case default
                wrong_mode = .true.
        end select
    elseif ( cate == 'B2' ) then
        select case (mode)
            case (1)
                wei_score  = a * 1.25 + b * 1.5 + c * 1.75 + d * 1.5 + e * 1.5
                cB2M1 = cB2M1 + 1
                cB2 = cB2 + 1
            case (2)
                wei_score  = a * 1 + b * 2 + c * 2 + d * 1.25 + e * 1.25
                cB2M2 = cB2M2 + 1
                cB2 = cB2 + 1
            case default
                wrong_mode = .true.
        end select
    elseif ( cate == 'C3' ) then
        select case (mode)
            case (1)
                wei_score  = a * 1.25 + b * 1.5 + c * 1.5 + d * 1.25 + e * 1.25
                cC3M1 = cC3M1 + 1
                cC3 = cC3 + 1
            case (2)
                wei_score  = a * 1.25 + b * 1.5 + c * 1.75 + d * 1.5 + e * 1.5
                cC3M2 = cC3M2 + 1
                cC3 = cC3 + 1
            case (3)
                wei_score  = a * 1.25 + b * 1.75 + c * 1.75 + d * 2 + e
                cC3M3 = cC3M3 + 1
                cC3 = cC3 + 1
            case (4)
                wei_score  = a  + b * 2 + c * 2 + d * 2
                cC3M4 = cC3M4 + 1
                cC3 = cC3 + 1
            case default
                wrong_mode = .true.
        end select
    else
        wrong_cate = .true.
    end if
    total_number = total_number + 1

    !輸出該筆成績狀態
    if ( wrong_mode ) then
        error_mode = error_mode + 1
        write (18,'(I3,A4,I5,5F7.2," ",A)') total_number, cate, mode, a, b, c, d, e, "wrong mode"
    elseif ( wrong_cate ) then
        error_category = error_category + 1
        write (18,'(I3,A4,I5,5F7.2," ",A)') total_number, cate, mode, a, b, c, d, e, "wrong category"
    else
        write (18,'(I3,A4,I5,6F7.2)') total_number, cate, mode, a, b, c, d, e, wei_score
    endif
end subroutine

subroutine finish       !輸出統計資料
    use total
    write(18,*)
    write(18,*) "total number =", total_number
    write(18,*) "category A1 =",cA1,", B2 =",cB2,", C3 =",cC3
    write(18,*) "error category =",error_category,", error mode =",error_mode
    write(18,*)
    write(18,*) "category A1, mode 1 =",cA1M1,", mode 2 =",cA1M2,", mode 3 =",cA1M3
    write(18,*) "category B2, mode 1 =",cB2M1,", mode 2 =",cB2M2
    write(18,*) "category C3, mode 1 =",cC3M1,", mode 2 =",cC3M2,", mode 3 =",cC3M3,", mode 4 =",cC3M4


end subroutine
