.syntax unified
.cpu cortex-m4
.thumb

.data
	pattern: .byte 0xE, 0xC, 0x9, 0x3, 0x7, 0x3, 0x9, 0xC
	leds: .byte 0x0

.text
	.global main
	.equ RCC_AHB2ENR,   0x4002104C
	.equ GPIOB_MODER,   0x48000400
	.equ GPIOB_OSPEEDR, 0x48000408
	.equ GPIOB_ODR,     0x48000414

main:
    bl   GPIO_init
	ldr  r0, =GPIOB_ODR
	ldr  r1, =leds
	ldr  r2, =pattern
	movs r3, 0

	Loop:
		ldrh  r4, [r2, r3]
		strh  r4, [r1]
		add   r3, #1
		cmp   r3, #8
		bne   NoReset
		movs r3, 0
		NoReset:
		BL DisplayLED
		BL Delay
		B Loop

GPIO_init:
	//Enable AHB2 clock
	movs r0, #0x2
	ldr  r1, =RCC_AHB2ENR
	str  r0, [r1]

	//Set PB3, 4, 5, 6 as output mode
	movs r0, #0x1540
	ldr  r1, =GPIOB_MODER
	ldr  r2, [r1]
	and  r2, #0xFFFFC03F
	orrs r2, r2, r0
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

Delay:
	mov r4, #0
	ldr r5, =1000000
  	DelayLoop:
		add r4, #1
		cmp r4, r5
		blt  DelayLoop
	BX LR
