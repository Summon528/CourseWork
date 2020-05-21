; mulbyshift: multiply val1 by 26 and store the result in val2
; ======
;       val1 @ 0x600000-600004
;       val2 @ 0x600004-600008
; ======

section	.text
    global _start

_start:
    ; val2 = val1 * 16 + va1 * 8 + var * 2
    mov eax, [0x600000]
    mov ebx, eax
    shl ebx, 3
    mov ecx, eax
    shl ecx, 1
    shl eax, 4
    add eax, ebx
    add eax, ecx
    mov [0x600004], eax
done: