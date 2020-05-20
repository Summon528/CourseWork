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
    and ax, 0b0000111111100000 ; mask required bits
    shr ax, 5                  ; shift 5 bits
    mov [0x600000], al         ; store the result back (use al to avoid polluting)

done: