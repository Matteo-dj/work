#ifndef __HISOC_SPI_H__
#define __HISOC_SPI_H__

#include "asm/platform.h"
#include "hisoc/clock.h"

#define REG_MUXCTRL_SPI0_SCLK_MAP       IO_ADDRESS(IO_MUX_REG_BASE + 0x050)     //muxctrl_reg20 , 0x1:SPI0_SCLK
#define REG_MUXCTRL_SPI0_SDO_MAP        IO_ADDRESS(IO_MUX_REG_BASE + 0x054)     //muxctrl_reg21 , 0x1:SPI0_SDO
#define REG_MUXCTRL_SPI0_SDI_MAP        IO_ADDRESS(IO_MUX_REG_BASE + 0x058)     //muxctrl_reg22 , 0x1:SPI0_SDI
#define REG_MUXCTRL_SPI0_CSN_MAP        IO_ADDRESS(IO_MUX_REG_BASE + 0x05c)     //muxctrl_reg23 , 0x1:SPI0_CSN
#define REG_MUXCTRL_SPI1_SCLK_MAP       IO_ADDRESS(IO_MUX_REG_BASE + 0x060)     //muxctrl_reg24 , 0x1:SPI1_CLK
#define REG_MUXCTRL_SPI1_SDO_MAP        IO_ADDRESS(IO_MUX_REG_BASE + 0x064)     //muxctrl_reg25 , 0x1:SPI1_SDO
#define REG_MUXCTRL_SPI1_SDI_MAP        IO_ADDRESS(IO_MUX_REG_BASE + 0x068)     //muxctrl_reg26 , 0x1:SPI1_SDI
#define REG_MUXCTRL_SPI1_CSN0_MAP       IO_ADDRESS(IO_MUX_REG_BASE + 0x06c)     //muxctrl_reg27 , 0x1:SPI1_CSN0
#define REG_MUXCTRL_SPI1_CSN1_MAP       IO_ADDRESS(IO_MUX_REG_BASE + 0x004)     //muxctrl_reg1  , 0x2:SPI1_CSN1
#define REG_MUXCTRL_SPI1_CSN2_MAP       IO_ADDRESS(IO_MUX_REG_BASE + 0x008)     //muxctrl_reg2  , 0x2:SPI1_CSN1


#define SPI0_NUM_CS     1
#define SPI1_NUM_CS     2

#define REG_CRG57_MAP           (CRG_REG_BASE + 0x0e4)
#define CRG57_SSP1_CKEN_SHIFT   (14)
#define CRG57_SSP0_CKEN_SHIFT   (13)
#define CRG57_SSP1_RST_SHIFT    (6)
#define CRG57_SSP0_RST_SHIFT    (5)
#define CRG57_SSP1_CKEN         (0x1 << 14) /* 0:close clk, 1:open clk */
#define CRG57_SSP0_CKEN         (0x1 << 13) /* 0:close clk, 1:open clk */
#define CRG57_SSP1_RST          (0x1 << 6) /* 0:cancel reset, 1:reset */
#define CRG57_SSP0_RST          (0x1 << 5) /* 0:cancel reset, 1:reset */

#define REG_MISC_CTRL1_MAP		(MISC_REG_BASE + 0x04)
#define MISC_CTRL1_SSP1_CS_SHIFT        (26)
#define MISC_CTRL1_SSP1_CS              (0x3 << 26) /* 00:cs0, 01:cs1, 10:cs2 */

static int spi_cfg_cs(uint16_t bus_num, uint8_t csn)
{
	unsigned int value;
	switch (bus_num) {
	case 0:
		if ((csn + 1) > SPI0_NUM_CS) {
			dprintf("%s, %s, %d line: error\n",
					__FILE__, __func__, __LINE__);
			return -1;
		}
		break;

	case 1:
		if (csn < SPI1_NUM_CS) {
			value = readl(REG_MISC_CTRL1_MAP);
			value &= ~MISC_CTRL1_SSP1_CS;
			value |= (csn << MISC_CTRL1_SSP1_CS_SHIFT);
			writel(value, REG_MISC_CTRL1_MAP); /* set spi1 csn */
		} else {
			dprintf("%s, %s, %d line: error\n",
					__FILE__, __func__, __LINE__);
			return -1;
		}
		break;

	default:
		dprintf("%s, %s, %d line: error\n",
				__FILE__, __func__, __LINE__);
		return -1;

	}
	return 0;
}

static int spi_hw_init_cfg(uint16_t bus_num)
{
	unsigned int value;

	switch (bus_num) {
	case 0:
		value = readl(REG_CRG57_MAP);
		value &= ~CRG57_SSP0_RST;
		value |= CRG57_SSP0_CKEN;
		writel(value, REG_CRG57_MAP); /* open spi0 clk */
		break;

	case 1:
		value = readl(REG_CRG57_MAP);
		value &= ~CRG57_SSP1_RST;
		value |= CRG57_SSP1_CKEN;
		writel(value, REG_CRG57_MAP); /* open spi1 clk */
		break;

	default:
		dprintf("%s, %s, %d line: error\n",
				__FILE__, __func__, __LINE__);
		return -1;
	}
	return 0;
}

static int spi_hw_exit_cfg(uint16_t bus_num)
{
	unsigned int value;

	switch (bus_num) {
	case 0:
		value = readl(REG_CRG57_MAP);
		value |= CRG57_SSP0_RST;
		value &= ~CRG57_SSP0_CKEN;
		writel(value, REG_CRG57_MAP); /* close spi0 clk */
		break;

	case 1:
		value = readl(REG_CRG57_MAP);
		value |= CRG57_SSP1_RST;
		value &= ~CRG57_SSP1_CKEN;
		writel(value, REG_CRG57_MAP);/* close spi1 clk */
		break;

	default:
		dprintf("%s, %s, %d line: error\n",
				__FILE__, __func__, __LINE__);
		return -1;
	}
	return 0;
}
#endif

