#ifndef	__HISOC_SYSCTRL_H__
#define	__HISOC_SYSCTRL_H__

#include "asm/platform.h"

#define REG_SC_CTRL			0
#define REG_SC_SYSRES			0x4   /*系统软复位控制器*/
#define PERIPHCTRL24			0x94

#define REG_SC_GEN0			0x0138
#define REG_SC_GEN1			0x013c
#define REG_SC_GEN2			0x0140
#define REG_SC_GEN3			0x0144
#define REG_SC_GEN4			0x0148
//#define REG_SC_PERI_CLKEN                (0x20)
//#define REG_SC_PERI_CLKSEL              (0x28)
//#define REG_SC_WDG_RST_CTRL         (0x3c)

#define REG_SC_DDRT0			0x0090
#define NORMAL_BOOTMODE_OFFSET		9
#define NORMAL_BOOTMODE_MASK		3

/********** Communication Register and flag used by bootrom *************/
#define REG_START_FLAG         (SYS_CTRL_REG_BASE + REG_SC_GEN1)
#define CONFIG_START_MAGIC     (0x444f574e)
#define SYS_CTRL_SYSSTAT			0x8c
#endif

