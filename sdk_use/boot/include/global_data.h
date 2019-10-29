/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * (C) Copyright 2002-2010
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 */

#ifndef	__ASM_GBL_DATA_H
#define __ASM_GBL_DATA_H

#include <environment.h>


typedef	struct	global_data {
	HI_ENV_DATA_EXTEND_ST env_data;//为了kernel后续扩展，全部缓存
	unsigned int	env_valid;	/* Checksum of Environment valid? */
	unsigned int	env_addr1;
	unsigned int	env_addr2;
} gd_t;

//#include <asm-generic/global_data_flags.h>

#endif /* __ASM_GBL_DATA_H */
