/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * (C) Copyright 2011
 * Ilya Yanok, EmCraft Systems
 */
 
#include <types.h>
#include <chip.h>



/*
void invalidate_dcache_range(unsigned long start, unsigned long stop)
{
	if (!check_cache_range(start, stop))
		return;

	while (start < stop) {
		__asm__ volatile("mcr p15, 0, %0, c7, c6, 1\n" : : "r"(start));
		start += CACHE_LINE_SIZE;
	}
}

void flush_dcache_range(unsigned long start, unsigned long stop)
{
	if (!check_cache_range(start, stop))
		return;

	while (start < stop) {
		__asm__ volatile("mcr p15, 0, %0, c7, c14, 1\n" : : "r"(start));
		start += CACHE_LINE_SIZE;
	}

	__asm__ volatile("mcr p15, 0, %0, c7, c10, 4\n" : : "r"(0));
}
*/
