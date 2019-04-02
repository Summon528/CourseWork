def GCD(inA, inB):
    A = inA
    B = inB
    done = 0
    while not done:
        if A < B:
            A, B = B - A, A
        elif B:
            A = A - B
        else:
            done = 1
        print(A, B)
    return(A)


GCD(36, 15)
