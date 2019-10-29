	.cpu arm926ej-s
	.fpu softvfp
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 2
	.eabi_attribute 30, 2
	.eabi_attribute 34, 0
	.eabi_attribute 18, 4
	.file	"chip_auth.c"
	.text
	.align	2
	.global	chip_verify
	.type	chip_verify, %function
chip_verify:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r2, .L15
	ldr	r3, [r2, #16]
	and	r3, r3, #28
	cmp	r3, #4
	bne	.L6
	mov	ip, #0
	mov	r0, #1
	str	ip, [r2, #12]
	ldr	r3, .L15+4
	mov	r1, r2
	str	r0, [r2, #8]
	b	.L5
.L14:
	subs	r3, r3, #1
	beq	.L6
.L5:
	ldr	r2, [r1, #8]
	ldr	r0, .L15
	cmp	r2, #0
	bne	.L14
	ldr	r3, .L15+8
	ldr	r0, [r0, #20]
	ldr	r1, [r3]
	cmp	r1, #0
	ble	.L6
	ldrb	ip, [r3, #4]	@ zero_extendqisi2
	cmp	r0, ip
	beq	.L9
	add	r3, r3, #4
	sub	r1, r1, #1
	add	r1, r3, r1
	b	.L8
.L7:
	ldrb	r2, [r3, #1]!	@ zero_extendqisi2
	cmp	r0, r2
	beq	.L9
.L8:
	cmp	r3, r1
	bne	.L7
.L6:
	mvn	r0, #0
	bx	lr
.L9:
	mov	r0, #0
	bx	lr
.L16:
	.align	2
.L15:
	.word	270434304
	.word	1000000
	.word	.LANCHOR0
	.size	chip_verify, .-chip_verify
	.align	2
	.global	start_main
	.type	start_main, %function
start_main:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	stmfd	sp!, {r4, lr}
	bl	chip_verify
	cmp	r0, #0
	ldmnefd	sp!, {r4, pc}
	ldmfd	sp!, {r4, lr}
	b	start_cfunction
	.size	start_main, .-start_main
	.global	g_type_cnt
	.global	g_type_list
	.data
	.align	2
.LANCHOR0 = . + 0
	.type	g_type_cnt, %object
	.size	g_type_cnt, 4
g_type_cnt:
	.word	6
	.type	g_type_list, %object
	.size	g_type_list, 8
g_type_list:
	.byte	17
	.byte	18
	.byte	33
	.byte	34
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.ident	"GCC: (Hisilicon_v500_20160223) 4.9.4 20150629 (prerelease)"
	.section	.note.GNU-stack,"",%progbits
