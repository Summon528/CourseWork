; posneg: test if registers are positive or negative.
;         if ( eax >= 0 ) { var1 = 1 } else { var1 = -1 }
;         if ( ebx >= 0 ) { var2 = 1 } else { var2 = -1 }
;         if ( ecx >= 0 ) { var3 = 1 } else { var3 = -1 }
;         if ( edx >= 0 ) { var4 = 1 } else { var4 = -1 }
; ======
;       var1 @ 0x600000-600004
;       var2 @ 0x600004-600008
;       var3 @ 0x600008-60000c
;       var4 @ 0x60000c-600010
; ======

section	.text
    global _start

_start:
    mov [0x600000], DWORD PTR -1 ; set every var to -1
    mov [0x600004], DWORD PTR -1
    mov [0x600008], DWORD PTR -1
    mov [0x60000c], DWORD PTR -1
    cmp eax, 0
    jnge go1                     ; if eax < 0 skip
    mov [0x600000], DWORD PTR 1  ; else set var to 1
    go1:                         ; repeat 4 times
    cmp ebx, 0
    jnge go2
    mov [0x600004], DWORD PTR 1
    go2:
    cmp ecx, 0
    jnge go3
    mov [0x600008], DWORD PTR 1
    go3:
    cmp edx, 0
    jnge go4
    mov [0x60000c], DWORD PTR 1
go4:
done: