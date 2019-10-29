/* SPDX-License-Identifier: GPL-2.0+ */
/*
 *  armboot - Startup Code for ARM926EJS CPU-core
 *
 *  Copyright (c) 2003  Texas Instruments
 *
 *  ----- Adapted for OMAP1610 OMAP730 from ARM925t code ------
 *
 *  Copyright (c) 2001	Marius Gröger <mag@sysgo.de>
 *  Copyright (c) 2002	Alex Züpke <azu@sysgo.de>
 *  Copyright (c) 2002	Gary Jennejohn <garyj@denx.de>
 *  Copyright (c) 2003	Richard Woodruff <r-woodruff2@ti.com>
 *  Copyright (c) 2003	Kshitij <kshitij@ti.com>
 *  Copyright (c) 2010	Albert Aribaud <albert.u.boot@aribaud.net>
 */

#include "platform.inc"
/*
 *************************************************************************
 *
 * Startup Code (reset vector)
 *
 * do important init only if we don't start from memory!
 * setup Memory and board specific bits prior to relocation.
 * relocate armboot to ram
 * setup stack
 *
 *************************************************************************
 */
	.globl boot_start
/*0x0*/
boot_start:
	b	reset
	b	hard_reset
	b	hard_reset
	b	hard_reset
	b	hard_reset
	b	hard_reset
	b	hard_reset
	b	hard_reset
/*0x24*/	
	b	hard_reset
	.balignl 8,0x0000A000  /*default enviroment addr*/
	.balignl 32,0xFFFFFFFF /*reseave*/

/*0x44*/    
__blank_zone_start:
	.fill (BOOT_FILLSIZE),1,0
__blank_zone_end:

.globl _blank_zone_start
_blank_zone_start:
	.word __blank_zone_start

.globl _blank_zone_end
_blank_zone_end:
	.word __blank_zone_end

.globl _armboot_start
_armboot_start:
	.word boot_start

.globl _bss_start
_bss_start:
	.word __bss_start

.globl _bss_end
_bss_end:
	.word _end 
        
	.globl	reset
reset:
	/*
	 * set the cpu to SVC32 mode
	 */
	mrs	r0,cpsr
	bic	r0,r0,#0x1f
	orr	r0,r0,#0xd3
	msr	cpsr,r0

	/*
	 * flush D cache before disabling it
	 */
	mov	r0, #0
flush_dcache:
	mrc	p15, 0, r15, c7, c10, 3
	bne	flush_dcache

	mcr	p15, 0, r0, c8, c7, 0	/* invalidate TLB */
	mcr	p15, 0, r0, c7, c5, 0	/* invalidate I Cache */

	/*
	 * disable MMU and D cache
	 * enable I cache if CONFIG_SYS_ICACHE_OFF is not defined
	 */
	mrc	p15, 0, r0, c1, c0, 0
	bic	r0, r0, #0x00000300	/* clear bits 9:8 (---- --RS) */
	bic	r0, r0, #0x00000087	/* clear bits 7, 2:0 (B--- -CAM) */
#if 0
#ifdef CONFIG_SYS_EXCEPTION_VECTORS_HIGH
	orr	r0, r0, #0x00002000	/* set bit 13 (--V- ----) */
#else
	bic	r0, r0, #0x00002000	/* clear bit 13 (--V- ----) */
#endif
#endif
	orr	r0, r0, #0x00000002	/* set bit 1 (A) Align */
#ifndef CONFIG_SYS_ICACHE_OFF
	orr	r0, r0, #0x00001000	/* set bit 12 (I) I-Cache */
#endif
	mcr	p15, 0, r0, c1, c0, 0

	/*check if run in ram(addr:0x7xxxxxxx)*/
	mov	r0, pc, lsr#28
	cmp	r0, #0x7
	beq	clrmap

init_reg:  
	ldr r0,_blank_zone_start
	ldr	r1,=BOOTLOADER_START_ADDR
	sub	r0,r0,r1
	ldr	r1,=SFC_MEM_BASE
	add	r0,r0,r1
	mov	r1,#0	/* flags: 0->normal 1->pm */
	
	/*
	 * Go setup Memory and board specific bits prior to relocation.
	 */
	bl 	init_registers /* go setup pll,mux,memory */

    /*run in flash(addr:0x3xxxxxxx)*/
	ldr r0,=clrmap
	ldr r1,=SFC_MEM_BASE
	add r0,r0,r1
	ldr r1,=BOOTLOADER_START_ADDR
	sub r0,r0,r1
	mov pc,r0

clrmap:
    /* enable itcm: 4KB */
    /* Size[6-2] SC[1] En[0] */
    ldr     r1, =0x0d
    mcr     p15, 0, r1, c9, c1, 1 
    /* remap */        
    ldr     r2, =REG_BASE_SCTL
    ldr     r1, [r2]
    orr     r1, r1, #0x100
    str     r1, [r2]

	/*check RAM size 2M|8MB*/
    ldr   r0,=CFG_2M_ADDR
    mov   r1,#0x0
    str   r1,[r0] 
    ldr   r2,=CFG_RAM_ADDR
    ldr   r1,=TEST_RAM_SIZE
    str   r1, [r2]  
    ldr   r2, [r0]
    cmp r2,r1
    beq relocate
    
    ldr r0,=REG_MDDRC_DYNAMIC_CONFIG0
    ldr r1,=CFG_8M_SIZE
    str   r1, [r0] 

relocate:				/* relocate U-Boot to RAM	    */
	adrl	r0, boot_start		/* r0 <- current position of code   */
	ldr	r1, =BOOTLOADER_START_ADDR	/* test if we run from flash or RAM */
	cmp     r0, r1          /* don't reloc during debug         */
	beq     stack_setup

	ldr	r2, _armboot_start
	ldr	r3, _bss_start
	sub	r2, r3, r2		/* r2 <- size of armboot            */
	add	r2, r0, r2		/* r2 <- source end address         */

	/* copy code to ddr _TEXT_BASE start place */
copy_loop:				/* copy 32 bytes at a time */
	ldmia	r0!, {r3 - r10}		/* copy from source address [r0] */
	stmia	r1!, {r3 - r10}		/* copy to   target address [r1] */
	cmp	r0, r2			/* until source end addreee [r2] */
	ble	copy_loop

	/* Set up the stack */
stack_setup:
    ldr     r0, =BOOTLOADER_STACK
    mov     sp, r0
    and     sp, sp, #~7             /* 8 byte alinged for (ldr/str)d */

	/* Clear BSS (if any). Is below tx (watch load addr - need space) */
	ldr	r0, _bss_start		/* find start of bss segment */
	ldr	r1, _bss_end		/* stop here */
	mov	r2, #0x00000000		/* clear value */
clear_bss:
	str	r2, [r0]		/* clear BSS location */
	cmp	r0, r1			/* are we at the end yet */
	add	r0, r0, #4		/* increment clear index pointer */
	bne	clear_bss		/* keep clearing till at end */
	ldr     pc, =start_main
    .balign 4

hard_reset:
    ldr     r2, =0x101e003c   //bit3 wdg rst enable
    ldr     r1, [r2]
    orr     r1, r1, #0x8
    str     r1, [r2] 

    ldr     r2, =0x101e0000   //bit23 wdg clk enable
    ldr     r1, [r2]
    orr     r1, r1, #0x800000
    str     r1, [r2] 

    ldr     r2, =0x101e1004   //timeout time
    mov     r1, #0x0
    str     r1, [r2] 

    ldr     r2, =0x101e1000   //enable wdg
    mov     r1, #0x1D
    str     r1, [r2]     

    ldr     r2, =0x101e100C   //feed wdg
    mov     r1, #0x76
    str     r1, [r2] 
    b .
    