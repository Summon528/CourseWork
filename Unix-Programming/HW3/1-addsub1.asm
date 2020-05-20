; addsub1:
;         eax = 0x70fb17ed
;         eax = eax + 0xe17f0204
;         eax = eax - 0xa6d1b6cd

section	.text
    global _start
 
_start:
    ; straightforward implementation 
    mov eax, 0x70fb17ed
    add eax, 0xe17f0204
    sub eax, 0xa6d1b6cd