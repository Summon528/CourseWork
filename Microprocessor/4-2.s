.syntax unified
.cpu cortex-m4
.thumb

.data
	student_id: .byte 0, 6, 1, 1, 2, 6, 2

.text
	.global main
	.equ RCC_AHB2ENR,   0x4002104C
	.equ GPIOB_MODER,   0x48000400
	.equ GPIOB_OSPEEDR, 0x48000408
	.equ GPIOB_ODR,     0x48000414
	.equ GPIOB_BSR,     0x48000418
	.equ GPIOB_BRR,     0x48000428
	.equ DIN,           0x8
	.equ CS,            0x10
	.equ CLK,           0x20
	.equ DECODE_MODE,	0x09
	.equ SHUTDOWN,	0x0C
	.equ INTENSITY,	0x0A
	.equ SCAN_LIMIT, 0x0B
	.equ DISPLAY_TEST, 0x0F


main:
	BL GPIO_init
	BL max7219_init
	ldr r0, =0x1
	ldr r2, =student_id
	loop:
		ldr  r3, =7
		sub  r3, r3, r0
		ldrb r1, [r2, r3]
		push {r0, r2}
		bl MAX7219Send
		pop {r0, r2}
		add r0, #1
		cmp r0, #8
		bne loop
	end: b end

GPIO_init:
	//Enable AHB2 clock
	movs r0, #0x2
	ldr  r1, =RCC_AHB2ENR
	str  r0, [r1]

	//Set PB3, 4, 5 as output mode
	movs r0, #0x540
	ldr  r1, =GPIOB_MODER
	ldr  r2, [r1]
	and  r2, #0xFFFFF03F
	orrs r2, r2, r0
	str  r2, [r1]

	//Set PA3, 4, 5 as high speed mode
	movs r0, #0x0A80
	ldr  r1, =GPIOB_OSPEEDR
	strh r0, [r1]

	BX LR


MAX7219Send:
	lsl r0, r0, #8
	orr r0, r0, r1
	ldr r2, =#CS
	ldr r3, =#DIN
	ldr r4, =#CLK
	ldr r5, =#GPIOB_BSR
	ldr r6, =#GPIOB_BRR
	mov r7, #16//r7 = i
	max7219send_loop:
		mov r8, #1
		sub r9, r7, #1
		lsl r8, r8, r9 // r8 = mask
		str r4, [r6]//HAL_GPIO_WritePin(GPIOA, CLOCK, 0);
		tst r0, r8
		beq bit_not_set//bit not set
		str r3, [r5]
		b if_done
	bit_not_set:
		str r3, [r6]
	if_done:
		str r4, [r5]
		subs r7, r7, #1
		bgt max7219send_loop
		str r2, [r6]
		str r2, [r5]
		BX LR

max7219_init:
	push {lr}
	ldr  r0, =#SHUTDOWN
	ldr  r1, =0x1
	bl   MAX7219Send

	ldr  r0, =#DECODE_MODE
	ldr  r1, =0xFF
	bl   MAX7219Send

	ldr  r0, =#INTENSITY
	ldr  r1, =0xF
	bl   MAX7219Send

	ldr  r0, =#SCAN_LIMIT
	ldr  r1, =0x6
	bl   MAX7219Send

	ldr  r0, =#DISPLAY_TEST
	ldr  r1, =0x0
	bl   MAX7219Send

	pop  {lr}
	BX LR
