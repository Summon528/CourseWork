.syntax unified
.cpu cortex-m4
.thumb

.data
	pattern: .byte 0xE, 0xC, 0x9, 0x3, 0x7, 0x3, 0x9, 0xC
	leds: .byte 0x0
	on: .byte 0x1
	preBtn: .byte 0x0

.text
	.global main
	.equ RCC_AHB2ENR,   0x4002104C
	.equ GPIOB_MODER,   0x48000400
	.equ GPIOB_OSPEEDR, 0x48000408
	.equ GPIOB_ODR,     0x48000414
	.equ GPIOC_MODER,   0x48000800
	.equ GPIOC_IDR,     0x48000810

main:
    bl  GPIO_init

StartLED:
	ldr  r4, =on
	ldr  r5, =0x1
	strb r5, [r4]
	ldr  r0, =GPIOB_ODR
	ldr  r1, =leds
	ldr  r2, =pattern
	movs r3, 0
	Loop:
		ldrh  r4, [r2, r3]
		strb  r4, [r1]
		add   r3, #1
		cmp   r3, #8
		bne   NoReset
		movs r3, 0
		NoReset:
		BL DisplayLED
		BL Delay
		B Loop

Delay:
	mov r8, #0
	ldr r9, =500
	CheckBtn:
	ldr  r4, =GPIOC_IDR
	movs r6, 0
	movs r7, 0
	CheckBtnLoop:
		ldr  r5, [r4]
		ands r5, #(1<<13)
		adds r6, #1
		cmp  r6, #1000
		beq  CheckBtnExit
		cmp  r5, 0
		bne  CheckBtnLoop
		adds r7, #1
		b    CheckBtnLoop
	CheckBtnExit:
		ldr  r4, =980
		cmp  r7, r4
		bgt  Press
		cmp  r7, #20
		blt  Release
		b    Exit
	Press:
		ldr  r4, =preBtn
		ldrb r5, [r4]
		cmp  r5, 0
		ldr  r5, =0x1
		strb r5, [r4]
		beq  Toggle
		b Exit
	Release:
		ldr  r4, =preBtn
		ldr  r5, =0x0
		strb r5, [r4]
		b Exit
	Exit:
		add r8, #1
		cmp r8, r9
		blt  CheckBtn
		BX LR
	Toggle:
		ldr  r4, =on
		ldrb r4, [r4]
		cbnz  r4, TurnOff
		b    StartLED
	TurnOff:
		ldr  r4, =leds
		ldr  r5, =0xFFFFFFFF
		strb r5, [r4]
		bl   DisplayLED
		ldr  r4, =on
		ldr  r5, =0x0
		strb r5, [r4]
		WaitBtn:
			bl   CheckBtn
			b    WaitBtn


GPIO_init:
	//Enable AHB2 clock
	movs r0, #0x6
	ldr  r1, =RCC_AHB2ENR
	str  r0, [r1]

	//Set PB3, 4, 5, 6 as output mode
	movs r0, #0x1540
	ldr  r1, =GPIOB_MODER
	ldr  r2, [r1]
	and  r2, #0xFFFFC03F
	orrs r2, r2, r0
	str  r2, [r1]

	//Set PC13 as input mode
	ldr  r1, =GPIOC_MODER
	ldr  r2, [r1]
	and  r2, #0xF3FFFFFF
	str  r2, [r1]

	//Set PA3, 4, 5, 6 as high speed mode
	movs r0, #0x2A80
	ldr  r1, =GPIOB_OSPEEDR
	strh r0, [r1]

	BX LR

DisplayLED:
	ldr  r4, =leds
	ldrh  r4, [r4]
	lsl  r4, #3
	ldr  r5, =GPIOB_ODR
	strh r4, [r5]
	BX LR


