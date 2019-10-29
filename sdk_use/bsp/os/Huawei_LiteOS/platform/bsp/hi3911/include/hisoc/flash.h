#ifndef __HISOC_FLASH_H__
#define __HISOC_FLASH_H__

#include "asm/platform.h"
#include "hisoc/sys_ctrl.h"

#define SYSSTAT_SPI_DEVICE_TYPE		(1 << 3)
#define SPI_DEVICE_TYPE_SHIFT		3

#define PERI_CRG48			0xc0
#define PERI_CLK_SEL_MASK		(0x3 << 2)
#define PERI_CRG48_CLK_SEL(_clk)	(((_clk) & 0x3) << 2)
#define PERI_CLK_SEL_24M                PERI_CRG48_CLK_SEL(0x0)
#define PERI_CLK_SEL_148_5M             PERI_CRG48_CLK_SEL(0x1)
#define PERI_CLK_SEL_125M               PERI_CRG48_CLK_SEL(0x2)
#define PERI_CLK_SEL_198M               PERI_CRG48_CLK_SEL(0x3)
#define PERI_CRG48_CLK_EN               (1 << 1)
#define PERI_CRG48_SOFT_RST_REQ         (1 << 0)

#define GET_FMC_BOOT_MODE ({ \
		unsigned int boot_mode = 0, base = (SYS_CTRL_REG_BASE); \
		boot_mode = readl((void *)(base + SYS_CTRL_SYSSTAT)); \
		boot_mode &= SPI_NOR_ADDRESS_MODE_MASK; \
		boot_mode; })

#define SPI_DEVICE_TYPE_SHIFT                   3
#define MULTIMODE_SHIFT                         7
/* bit[3]=0; bit[7]:SPI nor address mode; bit[7]=(0:3-Byte | 1:4-Byte) */
#define SPI_NOR_ADDRESS_MODE_MASK               (0x1 << MULTIMODE_SHIFT)
/* bit[3]=1; bit[7]: SPI nand I/O widthe; bit[7]=(0: 1-I/O | 1: 4-I/O */
#define SPI_NAND_IO_WIDTHE_MASK                 (0x1 << MULTIMODE_SHIFT)

/*---------------------------------------------------------------------------*/
/* hifmc100_set_fmc_system_clock */
/*---------------------------------------------------------------------------*/
static void hifmc100_set_fmc_system_clock(int clock, int clk_en)
{
	unsigned int base = (CRG_REG_BASE);
	unsigned int old_val, regval = readl((void *)(base + PERI_CRG48));
	old_val = regval;

	regval &= ~PERI_CLK_SEL_MASK;
	regval |= PERI_CLK_SEL_24M;		/* Default Clock */

	regval &= ~PERI_CLK_SEL_MASK;
	regval |= clock & PERI_CLK_SEL_MASK;

	if (clk_en)
		regval |= PERI_CRG48_CLK_EN;
	else
		regval &= ~PERI_CRG48_CLK_EN;

	if (regval != old_val)
		writel(regval, (void *)(base + PERI_CRG48));
}

/*---------------------------------------------------------------------------*/
/* hifmc100_get_fmc_best_clock */
/*---------------------------------------------------------------------------*/
static void hifmc100_get_fmc_best_clock(unsigned int *clock)
{
	int ix;
	int clk_reg;

#define CLK_2X(_clk)	(((_clk) + 1) >> 1)
	unsigned int sysclk[] = {
		CLK_2X(24), PERI_CLK_SEL_24M,
		CLK_2X(125), PERI_CLK_SEL_125M,
		CLK_2X(148), PERI_CLK_SEL_148_5M,
		CLK_2X(198), PERI_CLK_SEL_198M,
		0,		0,
		0, 0,
	};
#undef CLK_2X

	clk_reg = PERI_CLK_SEL_24M;
	for (ix = 0; sysclk[ix]; ix += 2) {
		if (*clock < sysclk[ix])
			break;
		clk_reg = sysclk[ix + 1];
	}

	*clock = clk_reg;
}

#endif

