; math3: 32-bit unsigned arithmetic
;         var4 = (var1 * 5) / (var2 - 3)
;         note: overflowed part should be truncated
; ======
;       var1 @ 0x600000-600004
;       var2 @ 0x600004-600008
;       var4 @ 0x600008-60000c
; ======

section	.text
    global _start

_start:
    ; straightforward implementation 
    mov eax, [0x600000]
    mov ebx, 5
    mul ebx
    mov ebx, [0x600004]
    sub ebx, 3
    cdq
    div ebx
    mov [0x600008], eax
done: