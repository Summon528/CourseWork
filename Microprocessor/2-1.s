.syntax unified
.cpu cortex-m4
.thumb


.data
	user_stack_bottom: .zero 128
	expr_result: .word 0

.text
	.global main
	postfix_expr: .asciz "-100 10 20 + - 10 +"

main:
ldr sp, =user_stack_bottom
adds sp, #128
LDR R0, =postfix_expr
movs r2, #1
movs r3, #0
movs r5, #10
loop:
ldrb r1, [r0], #1 // r1 store tmp char
cmp r1, ' '
beq space
cmp r1, '0'
blt operator
digit:
	subs r1, '0'
	mla r3, r3, r5, r1
	b loop
neg:
	movs r2, #-1 // r2 isneg
	b loop
space:
	mul r3, r2
	push {r3}
	movs r2, #1
	movs r3, #0
	b loop
operator:
	ldrb r6, [r0]
	cmp r6, '0'
	bge neg

	pop {r3}
	movs r4, r3
	pop {r3}

	cmp r1, '+'
	bne sub
	add:
		adds r3, r4
		b op_con
	sub:
		subs r3, r4
	op_con:
		adds R0, #1
		push {r3}
		movs r3, #0
		cmp r6, 0
		beq finish
		b loop
finish:
	ldr r0, =expr_result
	pop {r3}
	str r3, [r0]
	b program_end

program_end:
B program_end

