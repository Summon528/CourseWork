.syntax unified
.cpu cortex-m4
.thumb

.text
	.global main
	.equ RCC_AHB2ENR,   0x4002104C
	.equ GPIOB_MODER,   0x48000400
	.equ GPIOB_OSPEEDR, 0x48000408
	.equ GPIOB_ODR,     0x48000414
	.equ GPIOC_MODER,   0x48000800
	.equ GPIOC_IDR,     0x48000810
	.equ PASS_WORD,     0xF

main:
    bl   GPIO_init
    ldr  r1, =GPIOB_ODR
    ldr  r2, =(0xF<<3)
    ldr  r3, =0
    strh r2, [r1]

    loop:
    	bl  WaitBtn
    	ldr  r4, =GPIOC_IDR
    	ldr  r5, [r4]
    	ands r5, #(0xF<<5)
    	lsr  r5, #5
    	mvn  r5, r5
    	and  r5, #0xF
    	mov  r6, #PASS_WORD

    	strh r3, [r1]
    	bl Delay

    	strh r2, [r1]
    	cmp  r5, r6
    	bne  loop

    	bl Delay
    	strh r3, [r1]
    	bl Delay
    	strh r2, [r1]
    	bl Delay
    	strh r3, [r1]
    	bl Delay
    	strh r2, [r1]
		b loop


WaitBtn:
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
		blt  WaitBtn
		bx lr

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

	//Set PC5, 6, 7, 8, 13 as input mode
	ldr  r1, =GPIOC_MODER
	ldr  r2, [r1]
	ldr  r4, =0xF3FC03FF
	and  r2, r4
	str  r2, [r1]

	//Set PA3, 4, 5, 6 as high speed mode
	movs r0, #0x2A80
	ldr  r1, =GPIOB_OSPEEDR
	strh r0, [r1]

	BX LR

Delay:
	mov r7, #0
	ldr r8, =100000
  	DelayLoop:
		add r7, #1
		nop
		cmp r7, r8
		blt  DelayLoop
	BX LR
