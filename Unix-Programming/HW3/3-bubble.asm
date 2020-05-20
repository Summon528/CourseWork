; bubble: bubble sort for 10 integers
; ======
;       a[0] @ 0x600000-600004
;       a[1] @ 0x600004-600008
;       a[2] @ 0x600008-60000c
;       a[3] @ 0x60000c-600010
;       a[4] @ 0x600010-600014
;       a[5] @ 0x600014-600018
;       a[6] @ 0x600018-60001c
;       a[7] @ 0x60001c-600020
;       a[8] @ 0x600020-600024
;       a[9] @ 0x600024-600028
; ======


; for (int i = 0; i < len - 1; i++) {
;   for (int j = 0; j < len - i - 1; j++) {
;       if (a[j] > a[j + 1]) swap(a[j], a[j + 1]);
;   }
; }
section	.text
    global _start

_start:
    mov r8d, 0                                   ; i
    oloop:
        mov r9d, 0                               ; j
        iloop:
            mov r10d, [0x600000 + r9d * 4]       ; a[j]
            mov r11d, [0x600000 + (r9d + 1) * 4] ; a[j+1]
            cmp r10d, r11d
            jna iloop_exit                       ; if(a[j] <= a[j+1]) don't swap
            mov [0x600000 + r9d * 4], r11d       ; swap
            mov [0x600000 + (r9d + 1) * 4], r10d
        iloop_exit:
            inc r9d                              ; j++
            mov r10d, 9                          ; r10 = len - 1
            sub r10d, r8d                        ; r10 = len - i - 1
            cmp r9d, r10d
            jb  iloop                            ; if (j < len - i - 1) continue
        inc r8d                                  ; i++
        cmp r8d, 9
        jb oloop                                 ; if (i < len - 1) continue

done:

