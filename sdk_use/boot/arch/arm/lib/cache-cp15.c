/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * (C) Copyright 2002
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 */

#include <types.h>
#include <system.h>
//#include <asm/cache.h>
#include <sizes.h>
#include <chip.h>

extern void mmu_pagedir_init(int pdt);
extern void mmu_pagedir_cached_range(int pdt, int ram, int size);
extern void mmu_startup(int pdt);
extern void mmu_turnoff(void);

void invalidate_dcache_all(void)
{
	__asm__ volatile("mcr p15, 0, %0, c7, c6, 0\n" : : "r"(0));
}

void flush_dcache_all(void)
{
	__asm__ volatile(
		"0:"
		"mrc p15, 0, r15, c7, c14, 3\n"
		"bne 0b\n"
		"mcr p15, 0, %0, c7, c10, 4\n"
		 : : "r"(0) : "memory"
	);
}

static void cp_delay (void)
{
	volatile int i;

	/* copro seems to need some delay between reading and writing */
	for (i = 0; i < 100; i++)
		nop();
	__asm__ volatile("" : : : "memory");
}

/* cache_bit must be either CR_I or CR_C */
static void cache_enable(uint32_t cache_bit)
{
	uint32_t reg;

	reg = get_cr();	/* get control reg. */
	cp_delay();
	set_cr(reg | cache_bit);
}

/* cache_bit must be either CR_I or CR_C */
static void cache_disable(uint32_t cache_bit)
{
	uint32_t reg;

	reg = get_cr();
	cp_delay();

	if (cache_bit == CR_C) {
		/* if cache isn;t enabled no need to disable */
		if ((reg & CR_C) != CR_C)
			return;
		/* if disabling data cache, disable mmu too */
		cache_bit |= CR_M;
	}
	reg = get_cr();
	cp_delay();
	if (cache_bit == (CR_C | CR_M))
		flush_dcache_all();
	set_cr(reg & ~cache_bit);
}

void icache_enable(void)
{
	cache_enable(CR_I);
}

void icache_disable(void)
{
	cache_disable(CR_I);
}

int icache_status(void)
{
	return (get_cr() & CR_I) != 0;
}

void dcache_enable(void)
{
	cache_enable(CR_C);
}

void dcache_disable(void)
{
	cache_disable(CR_C);
}

int dcache_status(void)
{
	return (get_cr() & CR_C) != 0;
}

void enable_mmu(void)
{
    /* BEGIN: Modified by wuxianfeng 190206, 2012/10/24   PN:DTS2012101903272*/
	//unsigned long pdt = CFG_RAM_ADDR + SZ_64K;
    unsigned long pdt = HI_RAM_MEM_BASE;
    /* END:   Modified by wuxianfeng 190206, 2012/10/24 */
    
	mmu_pagedir_init((int)pdt);
	mmu_pagedir_cached_range((int)pdt, HI_RAM_MEM_BASE, SZ_8M);
	mmu_startup((int)pdt);
}

void stop_mmu(void)
{
	//dcache_inv_all();
	mmu_turnoff();
}
