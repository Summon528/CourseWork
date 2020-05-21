; swapreg: swap the values in RAX and RBX
; ======

section	.text
    global _start

_start:
    ; straightforward implementation 
    mov rcx, rax
    mov rax, rbx
    mov rbx, rcx
done: