; clear17: clear bit-17 in eax (zero-based index)
; ======

section	.text
    global _start

_start:
    and eax, 0b11111111111111011111111111111111

done: