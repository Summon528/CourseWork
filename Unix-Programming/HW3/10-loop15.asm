; loop15:
;         str1 is a string contains 15 lowercase and uppercase alphbets.
;         implement a loop to convert all alplabets to lowercase,
;         and store the result in str2.
; ======
;       str1 @ 0x600000-600010
;       str2 @ 0x600010-600020
; ======

section	.text
    global _start

_start:
    mov r8d, 0
    go:
        mov r9b, [0x600000 + r8d] ; read a char
        test r9b, r9b             ; test if it's the end of string
        je  end                   ; jump to the end if it is
        or  r9b, 0b00100000       ; tolower case
        mov [0x600010 + r8d], r9b ; store result back
        inc r8d
        jmp go
    end:
        mov [0x600010 + r8d], BYTE PTR 0

done: