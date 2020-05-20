; math2: signed arithmetic
;         eax = (-var1 * var2) + var3
; ======
;       var1 @ 0x600000-600004
;       var2 @ 0x600004-600008
;       var3 @ 0x600008-60000c
; ======

section	.text
    global _start

_start:
    ; straightforward implementation 
    mov eax, [0x600000]
    mov ebx, -1
    imul ebx
    mov ebx, [0x600004]
    imul ebx
    mov ebx, [0x600008]
    add eax, ebx
done: