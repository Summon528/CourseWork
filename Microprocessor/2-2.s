.syntax unified
.cpu cortex-m4
.thumb

.data
	user_stack_bottom: .zero 128
	result: .word 0
	max_size: .word 0

.text
	m: .word 0x5E
	n: .word 0x60
	.global main

GCD:
cbz r0, ba

cbz r1, bb

orr r3, r0, r1
ubfx r3, r3, #0, #1
cbz r3, bc

ubfx r3, r0, #0, #1
cbz r3, bd

ubfx r3, r1, #0, #1
cbz r3, be

cmp r0, r1
bgt bf

b bg
ba:
	ldr r3, =user_stack_bottom
	adds r4, sp, #0
	adds r3, #128
	subs r3, r4
	lsr r3, #2
	ldr r4, =max_size
	str r3, [r4]
	movs r3, r1
	bx lr

bb:
	ldr r3, =user_stack_bottom
	adds r4, sp, #0
	adds r3, #128
	subs r3, r4
	lsr r3, #2
	ldr r4, =max_size
	str r3, [r4]
	movs r3, r0
	bx lr

bc:
	push {lr}
	lsr r0, #1
	lsr r1, #1
	bl GCD
	pop {lr}
	mul r3, r2
	bx lr

bd:
	push {lr}
	lsr r0, #1
	bl GCD
	pop {lr}
	bx lr

be:
	push {lr}
	lsr r1, #1
	bl GCD
	pop {lr}
	bx lr

bf:
	push {lr}
	subs r0, r1
	bl GCD
	pop {lr}
	bx lr

bg:
	push {lr}
	movs r4, r0
	subs r3, r1, r0
	movs r0, r3
	movs r1, r4
	bl GCD
	pop {lr}
	bx lr


main:
ldr r0, =m
ldr r1, =n
ldr r0, [r0]
ldr r1, [r1]
ldr sp, =user_stack_bottom
adds sp, #128
movs r2, #2

BL GCD

ldr r0, =result
str r3, [r0]

program_end:
B program_end

