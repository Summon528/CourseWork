module s_data  !共享變數
    implicit none
    type datas
        integer :: term(2), quiz(4), hw(5), att,  final,    id
                   !期中     小考     作業    出席   最終成績   座號
        real :: score(2) !調分前後成績
    end type
    save
    type(datas), allocatable :: stu(:)
    integer :: total = 0, sum_final = 0, plus   
               !總人數      最終成績總和    調分量
    real :: sum_ = 0.0 !原始成績總和
end module s_data


program HW6
    implicit none
    open(unit=12, file="input.txt", status="old")
    open(unit=13, file="output.txt", status="replace")
    call input
    call solve
    call output
end program HW6

subroutine input
    use s_data
    implicit none
    integer:: i, ios
    do   !計算輸入檔總筆數
        total = total + 1
        read(12,*,iostat = ios)
        if (ios /= 0) exit
    enddo
    total = total - 1
    allocate(stu(total))
    rewind(12)
    do i = 1, total
        !輸入原始成績 編號資料
        read(12,*) stu(i)%term(1), stu(i)%term(2), &
                   stu(i)%quiz(1), stu(i)%quiz(2), stu(i)%quiz(3), stu(i)%quiz(4), &
                   stu(i)%hw(1), stu(i)%hw(2), stu(i)%hw(3), stu(i)%hw(4), stu(i)%hw(5), &
                   stu(i)%att
        stu(i)%id = i
    enddo
end subroutine

subroutine solve
    use s_data
    implicit none
    real:: tmp
    integer:: i
    do i = 1, total
        !計算總成績
        tmp =  (stu(i)%term(1) + stu(i)%term(2)) * 0.15
        tmp = tmp + real(sum(stu(i)%quiz) - min(stu(i)%quiz(1),stu(i)%quiz(2),stu(i)%quiz(3),stu(i)%quiz(4)))/3 * 0.35
        tmp = tmp + sum(stu(i)%hw)
        select case (stu(i)%att)
            case (0:2)
                tmp = tmp + 10
            case (3:4)
                tmp = tmp + 9
            case (5:6)
                tmp = tmp + 8
            case (7:8)
                tmp = tmp + 7
        end select
        stu(i)%score(1) = tmp
        sum_ = sum_ + tmp
    enddo

    !加分
    plus = 78 - sum_ / total
    do i = 1, total
        stu(i)%score(2) = stu(i)%score(1) + plus
        if (stu(i)%score(2) > 100.0) stu(i)%score(2) = 100.0
        !四捨五入到整數
        stu(i)%final = stu(i)%score(2) + 0.5
        sum_final = sum_final + stu(i)%final
    enddo

end subroutine

subroutine output
    use s_data
    implicit none
    integer :: i, j
    type(datas) tmp
    !輸出統計資料 & 每位學生資料
    write(13,"('Total number of students = ',I4)") total
    write(13,"('The average of original score = ',F5.2)") sum_ / total
    write(13,"('All students Plus',I4,' points')") plus
    write(13,"('The average of final score = ',F5.2,/)") real(sum_final) / total

    do i = 1, total
        write(13,"(I2,12I4,2F7.2,I4)") i, stu(i)%term(1), stu(i)%term(2), &
                            stu(i)%quiz(1), stu(i)%quiz(2), stu(i)%quiz(3), stu(i)%quiz(4), &
                            stu(i)%hw(1), stu(i)%hw(2), stu(i)%hw(3), stu(i)%hw(4), stu(i)%hw(5), &
                            stu(i)%att, stu(i)%score(1), stu(i)%score(2), stu(i)%final
    enddo

    !排序 Bubble sort O(n^2)
    do i = 1, total
        do j = 1, total - i
            if (stu(j)%final > stu(j+1)%final) then
                tmp = stu(j); stu(j) = stu(j+1); stu(j+1) = tmp
            endif
        enddo
    enddo

    !根據排序後結果輸出資料
    write(13,"(/,'score >= 90, Student number :')", advance = 'NO')
    i = total
    do while (stu(i)%final >= 90)
        write(13, "(I3)", advance = 'NO') stu(i)%id
        i = i - 1
        if (i < 0) exit
    enddo

    write(13,"(/,'score < 60, Student number :')", advance = 'NO')
    i = 1
    do while (stu(i)%final < 60)
        write(13, "(I3)", advance = 'NO') stu(i)%id
        i = i + 1
        if (i > total) exit
    enddo

    write(13,"(//,'Top 5 Student number :')", advance = 'NO')
    do i = 0, 4
        write(13, "(I3)", advance = 'NO') stu(total-i)%id
    enddo
    write(13,"(/,'Bottom 5 Student number :')", advance = 'NO')
    do i = 1, 5
        write(13, "(I3)", advance = 'NO') stu(i)%id
    enddo
end subroutine
