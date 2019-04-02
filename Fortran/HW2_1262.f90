    !this program calculate the approximate value and exact value of
    !integration of f(x) = x^2 + 2x from 1.6 to 3.8


module m_HW2
implicit none
!variable deceleration
save
type:: integration
    real :: exact_value ! exact value of the integration
    real :: upper_bound ! upper_bound of the integration
    real :: lower_bound ! lower_bound of the integration
    real :: tra_1 !store the answer of Trapezoidal Rule with 4 divisions
    real :: tra_2 !store the answer of Trapezoidal Rule with 6 divisions
    real :: sim_1 !store the answer of Simpson's Rule with 4 divisions
    real :: sim_2 !store the answer of Simpson's Rule with 6 divisions
    integer:: four, six
end type
type(integration):: t_HW2
end module m_HW2

program HW2
    use m_HW2
    implicit none
    call input
    call output  
end program HW2

subroutine input
    use m_HW2
    implicit none
    !fileIO deceleration
    open (11,file="input.txt",status = "old")
    open (12,file="out.txt",status = "replace")
    !input
    read(11,*) t_HW2%lower_bound,t_HW2%upper_bound
    read(11,*) t_HW2%four,t_HW2%six
end subroutine input

subroutine calc_exact_value
    use m_HW2
    implicit none
    !calculate exact_value using g(x) = x^3/3 + x^2 
    !exact_value = g(upper_bound) - g(lower_bound)
    t_HW2%exact_value =  t_HW2%upper_bound ** 3 / 3 +  t_HW2%upper_bound ** 2 - &
                        (t_HW2%lower_bound ** 3 / 3 +  t_HW2%lower_bound ** 2)
end subroutine calc_exact_value

function calc_step(accu) result(step)
    use m_HW2
    implicit none
    integer, intent(in) :: accu
    real :: step
    !calculate the value of every increment for x if using accu divisions
    step = (t_HW2%upper_bound - t_HW2%lower_bound) / accu
end function calc_step

subroutine calc_tra1
    use m_HW2
    implicit none
    !calculate approximate value using Trapezoidal Rule with 4 divisions
    real :: step, calc_step
    step = calc_step(t_HW2%four)
    t_HW2%tra_1 = (1*((t_HW2%lower_bound) ** 2 +  (t_HW2%lower_bound) * 2) + &
            2*((t_HW2%lower_bound + 1*step) ** 2 +  (t_HW2%lower_bound + 1*step) * 2) + &
            2*((t_HW2%lower_bound + 2*step) ** 2 +  (t_HW2%lower_bound + 2*step) * 2) + &
            2*((t_HW2%lower_bound + 3*step) ** 2 +  (t_HW2%lower_bound + 3*step) * 2) + &
            1*((t_HW2%upper_bound) ** 2 +  (t_HW2%upper_bound) * 2))* step /2  
end subroutine calc_tra1

subroutine calc_sim1
    use m_HW2
    implicit none
    !calculate approximate value using Simpson's Rule with 4 divisions
    real :: step, calc_step
    step = calc_step(t_HW2%four)
    t_HW2%sim_1 = (1*((t_HW2%lower_bound) ** 2 +  (t_HW2%lower_bound) * 2) + &
            4*((t_HW2%lower_bound + 1*step) ** 2 +  (t_HW2%lower_bound + 1*step) * 2) + &
            2*((t_HW2%lower_bound + 2*step) ** 2 +  (t_HW2%lower_bound + 2*step) * 2) + &
            4*((t_HW2%lower_bound + 3*step) ** 2 +  (t_HW2%lower_bound + 3*step) * 2) + &
            1*((t_HW2%upper_bound) ** 2 +  (t_HW2%upper_bound) * 2)) * step /3
end subroutine calc_sim1


subroutine calc_tra2
    use m_HW2
    implicit none
    real :: step, calc_step
    step = calc_step(t_HW2%six )
    !calculate approximate value using Trapezoidal Rule with 6 divisions
    t_HW2%tra_2 = (1*((t_HW2%lower_bound) ** 2 +  (t_HW2%lower_bound) * 2) + &
            2*((t_HW2%lower_bound + 1*step) ** 2 +  (t_HW2%lower_bound + 1*step) * 2) + &
            2*((t_HW2%lower_bound + 2*step) ** 2 +  (t_HW2%lower_bound + 2*step) * 2) + &
            2*((t_HW2%lower_bound + 3*step) ** 2 +  (t_HW2%lower_bound + 3*step) * 2) + &
            2*((t_HW2%lower_bound + 4*step) ** 2 +  (t_HW2%lower_bound + 4*step) * 2) + &
            2*((t_HW2%lower_bound + 5*step) ** 2 +  (t_HW2%lower_bound + 5*step) * 2) + &
            1*((t_HW2%upper_bound) ** 2 +  (t_HW2%upper_bound) * 2)) * step /2 
end subroutine calc_tra2

subroutine calc_sim2
    use m_HW2
    implicit none
    !calculate approximate value using Simpson's Rule with 6 divisions
    real :: step, calc_step
    step = calc_step(t_HW2%six)
    t_HW2%sim_2 = (1*((t_HW2%lower_bound) ** 2 +  (t_HW2%lower_bound) * 2) + &
            4*((t_HW2%lower_bound + 1*step) ** 2 +  (t_HW2%lower_bound + 1*step) * 2) + &
            2*((t_HW2%lower_bound + 2*step) ** 2 +  (t_HW2%lower_bound + 2*step) * 2) + &
            4*((t_HW2%lower_bound + 3*step) ** 2 +  (t_HW2%lower_bound + 3*step) * 2) + &
            2*((t_HW2%lower_bound + 4*step) ** 2 +  (t_HW2%lower_bound + 4*step) * 2) + &
            4*((t_HW2%lower_bound + 5*step) ** 2 +  (t_HW2%lower_bound + 5*step) * 2) + &
            1*((t_HW2%upper_bound) ** 2 +  (t_HW2%upper_bound) * 2)) * step /3
end subroutine calc_sim2


subroutine calc
    use m_HW2
    implicit none
    call calc_exact_value
    call calc_tra1
    call calc_tra2
    call calc_sim1
    call calc_sim2
end subroutine calc
 
subroutine output
    use m_HW2
    implicit none
    call calc
    !output
    write (12,*) "Integration range = [", t_HW2%lower_bound,',',t_HW2%upper_bound,']'
    write (12,*) "Exact value of integration =", t_HW2%exact_value,NEW_LINE('A')
    write (12,*) "Trapezoidal Rule"
    write (12,*) "The approximate value of",4,"division is",t_HW2%tra_1
    write (12,*) "The approximate value of",6,"division is",t_HW2%tra_2,NEW_LINE('A')
    write (12,*) "Simpson's Rule"
    write (12,*) "The approximate value of",4,"division is",t_HW2%sim_1
    write (12,*) "The approximate value of",6,"division is",t_HW2%sim_2
end subroutine output
