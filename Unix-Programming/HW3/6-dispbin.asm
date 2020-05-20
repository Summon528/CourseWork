; dispbin:
;         given a number in AX, store the corresponding bit string in str1.
;         for example, if AX = 0x1234, the result should be:
;         str1 = 0001001000111000
; ======
;       str1 @ 0x600000-600014
; ======


section	.text
    global _start

_start:
    mov r9d, 0xf              ; we want to chars into the str in reverse order
    go:
    mov r8b, al               ; copy a byte from ax
    and r8b, 0x1              ; get last bit
    add r8b, '0'              ; add '0'
    mov [0x600000 + r9d], r8b ; put it in str1[r9d]
    shr ax, 1                 ; remove last bit from ax
    dec r9d
    cmp r9d, 0
    jge go

done: