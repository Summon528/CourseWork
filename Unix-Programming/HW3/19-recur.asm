; recur: implement a recursive function

;    r(n) = 0, if n <= 0
;         = 1, if n == 1
;         = 2*r(n-1) + 3*r(n-2), otherwise

;    please call r(23) and store the result in RAX
; ======


section	.text
    global _start

_start:
    mov rbx, 18
    call r
    jmp end

r:
    push rbx                 ; preserve registers
    push rcx
    cmp rbx, 0
    jnle r2
    mov rax, 0               ; 0, if n <= 0
    jmp r_ret
    r2:
    cmp rbx, 1
    jne r3
    mov rax, 1               ; 1, if n == 1
    jmp r_ret
    r3:
    dec rbx                  ; n - 1
    call r                   ; r(n-1)
    shl rax, 1               ; 2*r(n-1)
    mov rcx, rax             ; rcx = 2*r(n-1)
    dec rbx                  ; n - 2
    call r                   ; r(n-2)
    lea rax, [rax * 2 + rax] ; 3*r(n-2)
    add rax, rcx
r_ret:
    pop rcx
    pop rbx
    ret

end:
done: