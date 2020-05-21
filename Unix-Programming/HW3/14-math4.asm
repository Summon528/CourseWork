; math4: 32-bit signed arithmetic
;         var4 = (var1 * -5) / (-var2 % var3)
;         note: overflowed part should be truncated
; ======
;       var1 @ 0x600000-600004
;       var2 @ 0x600004-600008
;       var3 @ 0x600008-60000c
;       var4 @ 0x60000c-600010
; ======

section	.text
    global _start

_start:
    ; straightforward implementation
    mov eax, [0x600004] ; var2
    neg eax             ; -var2
    cdq
    mov ebx, [0x600008] ; var3
    idiv ebx            ; -var2 / var3
    mov ecx, edx        ; remainder
    mov eax, [0x600000] ; var1
    mov ebx, -5         ; -5
    imul ebx            ; var1 * -5
    cdq
    idiv ecx
    mov [0x60000c], eax
done: