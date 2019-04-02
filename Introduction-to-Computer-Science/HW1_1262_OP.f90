real function f(x)
    implicit none
    real:: x
    f = x ** 2 + 2 * x
    return
end function f

real function g(x)
    implicit none
    real:: x
    g = x ** 3 / 3 +  x ** 2
    return
end function g

real function tra(lower_bound,upper_bound,division)
    implicit none
    real :: upper_bound, lower_bound, now, step, f
    integer :: i, division
    tra = 0.0
    now = lower_bound
    step = (upper_bound-lower_bound)/division
    do i=0, division
        if (i /= 0 .and. i/= division) then
            tra = tra + 2 * f(now)
        else
            tra = tra + f(now)
        end if
    now = now + step
    end do
    tra = tra * step / 2
end function tra

real function sim(lower_bound,upper_bound,division)
    implicit none
    real :: upper_bound, lower_bound, now, step, f
    integer :: i, division
    sim = 0.0
    now = lower_bound
    step = (upper_bound-lower_bound)/division
    do i=0, division
        if (i == 0 .or. i== division) then
            sim = sim + 1 * f(now)
        else if (mod(i,2) == 0) then
            sim = sim + 2 * f(now)
        else
            sim = sim + 4 * f(now)
        end if
    now = now + step
    end do
    sim = sim * step / 3
end function sim

program HW1
    implicit none
    real :: exact_value, upper_bound, lower_bound, g,  &
            tra_1 = 0.0, tra_2=0.0, sim_1=0.0, sim_2=0.0, tra, sim

    open (11,file="input.txt",status = "old")
    open (12,file="out.txt",status = "replace")

    read *, lower_bound, upper_bound
    exact_value = g(upper_bound) - g(lower_bound)

    tra_1 = tra(lower_bound,upper_bound,4)
    tra_2 = tra(lower_bound,upper_bound,6)

    sim_1 = sim(lower_bound,upper_bound,4)
    sim_2 = sim(lower_bound,upper_bound,6)

    write (*,*) "Integration range = [", lower_bound,',',upper_bound,']'
    write (*,*) "Exact value of integration =", exact_value,NEW_LINE('A')
    write (*,*) "The approximate value of",4,"division is",tra_1
    write (*,*) "The approximate value of",6,"division is",tra_2,NEW_LINE('A')
    write (*,*) "The approximate value of",4,"division is",sim_1
    write (*,*) "The approximate value of",6,"division is",sim_2
end program HW1
