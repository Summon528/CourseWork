; math5: 32-bit signed arithmetic
;         var3 = (var1 * -var2) / (var3 - ebx)
;         note: overflowed part should be truncated
; ======
;       var1 @ 0x600000-600004
;       var2 @ 0x600004-600008
;       var3 @ 0x600008-60000c
; ======


section	.text
    global _start

_start:
    mov r8d, [0x600008] ; var3
    sub r8d, ebx        ; var3 - ebx
    mov ebx, [0x600004] ; var2
    neg ebx             ; -var2
    mov eax, [0x600000] ; var1
    imul ebx            ; var1 * -var2
    cdq
    idiv r8d
    mov [0x600008], eax
done: