; swapreg: swap the values in RAX and RBX
; ======

section	.text
    global _start

_start:
    xor ch, 0b00100000       ; toupper, tolower
done: