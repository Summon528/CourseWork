; dec2ascii: convert the value (0-9) in AL to its ASCII character
; ======

section	.text
    global _start

_start:
    add al, '0'

done: