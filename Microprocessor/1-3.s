.syntax unified
.cpu cortex-m4
.thumb

.data
	arr1: .byte 0x19, 0x34, 0x14, 0x32, 0x52, 0x23, 0x61, 0x29
	arr2: .byte 0x18, 0x17, 0x33, 0x16, 0xFA, 0x20, 0x55, 0xAC

.text
	.global main

do_sort:
	movs r1, #7
	for1:
		cbz r1, exit1
		movs r2, #0
		for2:
			cmp r1, r2
			beq exit2
			mov r3, r2
			add r3, #1
			ldrb r4, [r0, r2]
			ldrb r5, [r0, r3]
			cmp r4, r5
			blt no_swap
			strb r5, [r0, r2]
			strb r4, [r0, r3]
			no_swap:
				add r2, #1
				b for2
		exit2:
			sub r1, #1
			b for1
	 exit1:
		bx lr

main:
	ldr r0, =arr1
	bl do_sort
	ldr r0, =arr2
	bl do_sort
L: b L
