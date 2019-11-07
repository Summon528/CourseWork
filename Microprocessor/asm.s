.syntax unified
.cpu cortex-m4
.thumb


.text
	.global GPIO_init
	.global max7219_send
	.global max7219_init
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

GPIO_init:
	push {r0-r2}
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
	pop {r0-r2}
	BX LR


max7219_send:
	push {r0-r9}
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
		pop {r0-r9}
		BX LR

max7219_init:
	push {r0-r1, lr}
	ldr  r0, =#SHUTDOWN
	ldr  r1, =0x1
	bl   max7219_send

	ldr  r0, =#DECODE_MODE
	ldr  r1, =0xFF
	bl   max7219_send

	ldr  r0, =#INTENSITY
	ldr  r1, =0xF
	bl   max7219_send

	ldr  r0, =#SCAN_LIMIT
	ldr  r1, =0x6
	bl   max7219_send

	ldr  r0, =#DISPLAY_TEST
	ldr  r1, =0x0
	bl   max7219_send

	pop  {r0-r1, lr}
	BX LR
