program HW1
    implicit none

    !this program calculate the approximate value and exact value of
    !integration of f(x) = x^2 + 2x from 1.6 to 3.8

    !variable deceleration
    real :: exact_value ! exact value of the integration
    real :: upper_bound ! upper_bound of the integration
    real :: lower_bound ! lower_bound of the integration
    real :: tra_1 !store the answer of Trapezoidal Rule with 4 divisions
    real :: tra_2 !store the answer of Trapezoidal Rule with 6 divisions
    real :: sim_1 !store the answer of Simpson's Rule with 4 divisions
    real :: sim_2 !store the answer of Simpson's Rule with 6 divisions
    real :: step !the value of every increment for x

    !fileIO deceleration
    open (11,file="in.txt",status = "old")
    open (12,file="out.txt",status = "replace")

    !input
    read(11,*) lower_bound,upper_bound

    !calculate exact_value using g(x) = x^3/3 + x^2
    !exact_value = g(upper_bound) - g(lower_bound)
    exact_value =  upper_bound ** 3 / 3 +  upper_bound ** 2 - &
                   (lower_bound ** 3 / 3 +  lower_bound ** 2)

    !calculate the value of every increment for x if using 4 divisions
    step = (upper_bound - lower_bound) / 4

    !calculate approximate value using Trapezoidal Rule with 4 divisions
    tra_1 = (1*((lower_bound) ** 2 +  (lower_bound) * 2) + &
            2*((lower_bound + 1*step) ** 2 +  (lower_bound + 1*step) * 2) + &
            2*((lower_bound + 2*step) ** 2 +  (lower_bound + 2*step) * 2) + &
            2*((lower_bound + 3*step) ** 2 +  (lower_bound + 3*step) * 2) + &
            1*((upper_bound) ** 2 +  (upper_bound) * 2))* step /2

    !calculate approximate value using Simpson's Rule with 4 divisions
    sim_1 = (1*((lower_bound) ** 2 +  (lower_bound) * 2) + &
            4*((lower_bound + 1*step) ** 2 +  (lower_bound + 1*step) * 2) + &
            2*((lower_bound + 2*step) ** 2 +  (lower_bound + 2*step) * 2) + &
            4*((lower_bound + 3*step) ** 2 +  (lower_bound + 3*step) * 2) + &
            1*((upper_bound) ** 2 +  (upper_bound) * 2)) * step /3

    !calculate the value of every increment for x if using 6 divisions
    step = (upper_bound - lower_bound) / 6

    !calculate approximate value using Trapezoidal Rule with 6 divisions
    tra_2 = (1*((lower_bound) ** 2 +  (lower_bound) * 2) + &
            2*((lower_bound + 1*step) ** 2 +  (lower_bound + 1*step) * 2) + &
            2*((lower_bound + 2*step) ** 2 +  (lower_bound + 2*step) * 2) + &
            2*((lower_bound + 3*step) ** 2 +  (lower_bound + 3*step) * 2) + &
            2*((lower_bound + 4*step) ** 2 +  (lower_bound + 4*step) * 2) + &
            2*((lower_bound + 5*step) ** 2 +  (lower_bound + 5*step) * 2) + &
            1*((upper_bound) ** 2 +  (upper_bound) * 2)) * step /2

    !calculate approximate value using Simpson's Rule with 6 divisions
    sim_2 = (1*((lower_bound) ** 2 +  (lower_bound) * 2) + &
            4*((lower_bound + 1*step) ** 2 +  (lower_bound + 1*step) * 2) + &
            2*((lower_bound + 2*step) ** 2 +  (lower_bound + 2*step) * 2) + &
            4*((lower_bound + 3*step) ** 2 +  (lower_bound + 3*step) * 2) + &
            2*((lower_bound + 4*step) ** 2 +  (lower_bound + 4*step) * 2) + &
            4*((lower_bound + 5*step) ** 2 +  (lower_bound + 5*step) * 2) + &
            1*((upper_bound) ** 2 +  (upper_bound) * 2)) * step /3

    !output
    write (12,*) "Integration range = [", lower_bound,',',upper_bound,']'
    write (12,*) "Exact value of integration =", exact_value,NEW_LINE('A')
    write (12,*) "Trapezoidal Rule"
    write (12,*) "The approximate value of",4,"division is",tra_1
    write (12,*) "The approximate value of",6,"division is",tra_2,NEW_LINE('A')
    write (12,*) "Simpson's Rule"
    write (12,*) "The approximate value of",4,"division is",sim_1
    write (12,*) "The approximate value of",6,"division is",sim_2
end program HW1
