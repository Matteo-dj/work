#ifndef __HISOC_NAND_H__
#define __HISOC_NAND_H__

#include "asm/platform.h"

#define PERI_CRG52                      (CRG_REG_BASE + 0x00D0)
#define PERI_CRG52_CLK_EN               (1U << 1)
#define PERI_CRG52_CLK_SEL_198M		(1U << 2)

#define REG_SYSSTAT			0x008C
#define BOOT_FROM_NAND			2

static void hinfc620_clk_enable(int enable)
{
	unsigned int reg_val = readl(PERI_CRG52);

	if (enable)
		reg_val |= (PERI_CRG52_CLK_EN | PERI_CRG52_CLK_SEL_198M);
	else
		reg_val &= ~PERI_CRG52_CLK_EN;

	writel(reg_val, (PERI_CRG52));
}
#define check_boot_type() ((readl(SYS_CTRL_REG_BASE + REG_SYSSTAT) >> 4) & 0x3);
#endif

