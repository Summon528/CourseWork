.syntax unified
.cpu cortex-m4
.thumb

.data
    result: .byte 0
.text
    .global main
    .equ X, 0x55AA
    .equ Y, 0xAA55

hamm:
    eor R0, R1
    movs R1, #0
    pop_cnt:
        cbz R0, ham_exit
        ubfx R3, R0, #0, #1
        lsr R0, #1
        add R1, R3
        b pop_cnt
    ham_exit:
        bx lr

main:
    movw R0, #X
    movw R1, #Y
    ldr R2, =result
    bl hamm
    str R1, [R2]
L: b L
