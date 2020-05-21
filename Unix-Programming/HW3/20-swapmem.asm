; swapmem: swap the values in val1 and val2
; ======
;       val1 @ 0x600000-600008
;       val2 @ 0x600008-600010
; ======


section	.text
    global _start

_start:
    ; straightforward implementation 
    mov r8d, [0x600000]
    mov r9d, [0x600008]
    mov [0x600000], r9d
    mov [0x600008], r8d

done: