/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * (C) Copyright 2009
 * Marvell Semiconductor <www.marvell.com>
 * Written-by: Prafulla Wadaskar <prafulla@marvell.com>
 */

#ifndef _ASM_CACHE_H
#define _ASM_CACHE_H

//#include <asm/system.h>

#ifndef CONFIG_ARM64

/*
 * Invalidate L2 Cache using co-proc instruction
 */
#ifdef CONFIG_SYS_THUMB_BUILD
void invalidate_l2_cache(void);
#else
static inline void invalidate_l2_cache(void)
{
	unsigned int val=0;

	asm volatile("mcr p15, 1, %0, c15, c11, 0 @ invl l2 cache"
		: : "r" (val) : "cc");
	isb();
}
#endif

int check_cache_range(unsigned long start, unsigned long stop);

void l2_cache_enable(void);
void l2_cache_disable(void);
void set_section_dcache(int section, enum dcache_option option);

void arm_init_before_mmu(void);
void arm_init_domains(void);
void cpu_cache_initialization(void);
void dram_bank_mmu_setup(int bank);

#endif

#endif /* _ASM_CACHE_H */
