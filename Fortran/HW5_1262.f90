module s_data         !要用到的變數
    implicit none
    integer, allocatable, dimension(:) :: score1, score2, score3, score4 !分數
    integer :: total = 0 !總人數
    integer, dimension(4):: sum_, square_sum !分數總合 平方和
    real, dimension(4):: stddev, mean !標準差 平均值
end module s_data


program HW5
    implicit none
    open(unit=12, file="input.txt", status="old")
    open(unit=13, file="output.txt", status="replace")
    call input
    call sovle1
    call sovle2
    call sovle3
    call sovle4
    call output
end program HW5

subroutine input
    use s_data
    implicit none
    integer:: ios, i
    do        !計算輸入總數量
        total = total + 1
        read(12, *, iostat = ios)
        if (ios /= 0) exit
    enddo
    total = total - 1
    allocate(score1(total))
    allocate(score2(total))
    allocate(score3(total))
    allocate(score4(total))
    rewind (12)
    do i = 1, total  !輸入原始成績順便計算總合及平方和
        read(12, *, iostat = ios) score1(i)
        sum_(1) = sum_(1) + score1(i)
        square_sum(1) = score1(i) * score1(i) + square_sum(1)
    enddo

end subroutine input

subroutine sovle1    !計算原始成績平均、標準差
    use s_data
    implicit none
    mean(1) = real(sum_(1)) / total
    stddev(1) = (real(square_sum(1)) / total - mean(1) * mean(1)) ** 0.5
end subroutine

subroutine sovle2    !計算mode1平均、標準差
    use s_data
    implicit none
    integer :: i
    !加分
    do i = 1, total
        score2(i) = min(100, score1(i) + 20)
        sum_(2) = sum_(2) + score2(i)
        square_sum(2) = score2(i) * score2(i) + square_sum(2)
    enddo
    !平均、標準差
    mean(2) = real(sum_(2)) / total
    stddev(2) = (real(square_sum(2)) / total - mean(2) * mean(2)) ** 0.5
end subroutine

subroutine sovle3  !計算mode2平均、標準差
    use s_data
    implicit none
    integer :: tmp, i
    !加分
    if (mean(1) < 75) then
        tmp = mean(1)
        tmp = 75 - tmp
        do i = 1, total
            score3(i) = min(100, score1(i) + tmp)
            sum_(3) = sum_(3) + score3(i)
            square_sum(3) = score3(i) * score3(i) + square_sum(3)
        enddo
        !平均、標準差
        mean(3) = real(sum_(3)) / total
        stddev(3) = (real(square_sum(3)) / total - mean(3) * mean(3)) ** 0.5
    endif
end subroutine

subroutine sovle4  !計算mode3平均、標準差
    use s_data
    implicit none
    integer :: i
    !加分
    do i = 1, total
        select case (score1(i))
            case (90:)
                score4(i) = min(100, score1(i) + 5)
            case (80:89)
                score4(i) = min(100, score1(i) + 4)
            case (70:79)
                score4(i) = min(100, score1(i) + 3)
            case (60:69)
                score4(i) = min(100, score1(i) + 1)
            case default
                score4(i) = score1(i)
        end select
        sum_(4) = sum_(4) + score4(i)
        square_sum(4) = score4(i) * score4(i) + square_sum(4)
    enddo
    !平均、標準差
    mean(4) = real(sum_(4)) / total
    stddev(4) = (real(square_sum(4)) / total - mean(4) * mean(4)) ** 0.5
end subroutine


subroutine output   !輸出資料
    use s_data
    implicit none
    integer :: i
    write(13, "('total number of students', I4, /)") total
    do i = 1, total
        write(13, "(I3, 4I7)")  i, score1(i), score2(i), score3(i), score4(i)
    enddo
    write (13, "(/,'mean and standard deviation of original score = ', 2F7.3)") mean(1),stddev(1)
    write (13, "('mean and standard deviation of mode 1(score1) = ', 2F7.3)") mean(2),stddev(2)
    write (13, "('mean and standard deviation of mode 2(score2) = ', 2F7.3)") mean(3),stddev(3)
    write (13, "('mean and standard deviation of mode 3(score3) = ', 2F7.3)") mean(4),stddev(4)
end subroutine output
