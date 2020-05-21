; swapreg: swap the values in RAX and RBX
; ======

section	.text
    global _start

_start:
    ; straightforward implementation 
    mov r8b, [0x600000]
    and r8b, 0b11011111       ; toupper case
    mov [0x600001], r8b
done: