; addsub2:
; final = val1 + val2 - val3
; ======
; val1 @ 0x600000-600004
; val2 @ 0x600004-600008
; val3 @ 0x600008-60000c
; final @ 0x60000c-600010
; ======

section	.text
    global _start

_start:
    mov r8d, [0x600000] ; load val1
    mov r9d, [0x600004] ; load val2
    add r8d, r9d        ; val1 + val2
    mov r9d, [0x600008] ; load val3
    sub r8d, r9d        ; val1 + val2 - val3
    mov [0x60000c], r8d ; store the value back