; eval1:
;         Rval = -Xval + (Yval – Zval)
; ======
;       Xval @ 0x600000-600004
;       Yval @ 0x600004-600008
;       Zval @ 0x600008-60000c
;       Rval @ 0x60000c-600010
; ======

section	.text
    global _start

_start:
    mov r8d, [0x600004]
    mov r9d, [0x600008]
    sub r8d, r9d ; Yval - Zval
    mov eax, [0x600000]
    mov r9d, -1
    imul r9d     ; -Xval
    add eax, r8d
    mov [0x60000c], eax

done: