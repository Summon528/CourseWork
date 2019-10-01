.syntax unified
.cpu cortex-m4
.thumb

.text
    .global main
    .equ N, 0

fib:
    cmp R0, #100
    bgt ofr
    cmp R0, #1
    blt ofr
    mov R4, #1
    mov R5, #0

loop:
    sub R0, #1
    cbz R0, ret
    adds R5, R5, R4
    bvs ovf
    eor R5, R4
    eor R4, R5
    eor R5, R4
    b loop
    ofr:
        movs R4, #0
        mvn R4, R4
        b ret
    ovf:
        movs R4, #1
        mvn R4, R4
    ret:
        bx lr

main:
    movs R0, #N
    bl fib
L: b L
