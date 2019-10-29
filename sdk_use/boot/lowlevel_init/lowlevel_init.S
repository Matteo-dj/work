	.file	"lowlevel_init.c"
	.text
	.align	2
	.global	init_registers
	.type	init_registers, %function
init_registers:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r2, [r0, #0]
	cmp	r2, #0
	mov	r7, #1
	bne	.L45    @寄存器地址不为0，则跳转到 .L43
.L2:
	ldr	r3, [r0, #4]
	cmp	r3, #0
	bne	.L45
	ldr	r3, [r0, #8]
	cmp	r3, #0
	ldrne	ip, [r0, #12]
	bne	.L3
	ldr	ip, [r0, #12]
	cmp	ip, #0
	beq	.L6  @所有值（4个）均为0时，退出函数，否则(至少有一个不为0)则跳到.L3
.L3:
	cmp	r1, #0
	beq	.L7  @绝对跳转到 .L7
.L47:
	tst	ip, #2
	beq	.L8
	and	r4, ip, #248
	mov	r4, r4, lsr #3
	add	r4, r4, #1
	cmp	r4, #32
	movne	r4, r7, asl r4
	andne	ip, ip, #63488
	ldr	r5, [r2, #0]
	movne	ip, ip, lsr #11
	subne	r4, r4, #1
	bicne	r5, r5, r4, asl ip
	ldrne	r4, [r0, #4]
	ldreq	ip, [r0, #4]
	orrne	ip, r5, r4, asl ip
	cmp	r3, #0
	str	ip, [r2, #0]
	beq	.L11
.L31:
@ 42 "lowlevel_init.c" 1
	nop
@ 0 "" 2
	subs	r3, r3, #1
	bne	.L31
.L11:
	add	r0, r0, #16 @下一个“块”，4个字    “一个轮回的开始”
.L48:
	ldr	r2, [r0, #0]
	cmp	r2, #0
	beq	.L2
.L45:
	cmp	r1, #0
	ldr	r3, [r0, #8]
	ldr	ip, [r0, #12]
	bne	.L47
.L7:
	tst	ip, #4
	beq	.L18
	and	r4, ip, #248
	mov	r4, r4, lsr #3
	add	r4, r4, #1
	cmp	r4, #32
	movne	r4, r7, asl r4
	andne	ip, ip, #63488
	ldr	r5, [r2, #0]
	movne	ip, ip, lsr #11
	subne	r4, r4, #1
	bicne	r5, r5, r4, asl ip
	ldrne	r4, [r0, #4]
	ldreq	ip, [r0, #4]
	orrne	ip, r5, r4, asl ip
	cmp	r3, #0
	str	ip, [r2, #0]
	beq	.L11
.L34:
@ 42 "lowlevel_init.c" 1
	nop
@ 0 "" 2
	subs	r3, r3, #1
	bne	.L34
	add	r0, r0, #16
	b	.L48
.L8:
	tst	ip, #131072
	bne	.L13
	cmp	r3, #0
	beq	.L11
.L33:
@ 42 "lowlevel_init.c" 1
	nop
@ 0 "" 2
	subs	r3, r3, #1
	bne	.L33
	add	r0, r0, #16
	b	.L48
.L18:
	tst	ip, #262144
	bne	.L22
	cmp	r3, #0
	beq	.L11
.L36:
@ 42 "lowlevel_init.c" 1
	nop
@ 0 "" 2
	subs	r3, r3, #1
	bne	.L36
	add	r0, r0, #16
	b	.L48
.L13:
	and	r4, ip, #16252928
	mov	r4, r4, lsr #19
	add	r4, r4, #1
	mov	r6, r7, asl r4
	ldr	r5, [r0, #4]
	mov	r8, ip, lsr #27
	sub	r6, r6, #1
.L16:
	ldr	ip, [r2, #0]
	cmp	r4, #32
	andne	ip, r6, ip, lsr r8
@ 42 "lowlevel_init.c" 1
	nop
@ 0 "" 2
	cmp	ip, r5
	bne	.L16
	cmp	r3, #0
	beq	.L11
.L32:
@ 42 "lowlevel_init.c" 1
	nop
@ 0 "" 2
	subs	r3, r3, #1
	bne	.L32
	add	r0, r0, #16
	b	.L48
.L22:
	and	r6, ip, #16252928
	mov	r6, r6, lsr #19
	add	r6, r6, #1
	mov	r8, r7, asl r6
	and	r5, ip, #131072
	sub	r8, r8, #1
	mov	sl, ip, lsr #27
.L27:
	ldr	ip, [r2, #0]
	cmp	r6, #32
	andne	ip, r8, ip, lsr sl
	cmp	r5, #0
	mov	r4, r5
	beq	.L26
	ldr	r4, [r0, #4]
	subs	r4, r4, ip
	movne	r4, #1
.L26:
@ 42 "lowlevel_init.c" 1
	nop
@ 0 "" 2
	cmp	r4, #0
	bne	.L27
	cmp	r3, #0
	beq	.L11
.L35:
@ 42 "lowlevel_init.c" 1
	nop
@ 0 "" 2
	subs	r3, r3, #1
	bne	.L35
	add	r0, r0, #16
	b	.L48
.L6:
@ 42 "lowlevel_init.c" 1
	nop
@ 0 "" 2
	bx	lr
	.size	init_registers, .-init_registers
	.ident	"GCC: (GNU) 4.4.0"
