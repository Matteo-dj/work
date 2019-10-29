#ifndef	__HISOC_I2C_H__
#define	__HISOC_I2C_H__

#include "asm/platform.h"

#define REG_MUXCTRL_CHG_GPIO_MAP        (IO_MUX_REG_BASE + 0x1dc)  //muxctrl_reg119, 00：GPIO13_4
#define REG_MUXCTRL_AC_GPIO_MAP         (IO_MUX_REG_BASE + 0x1e0)  //muxctrl_reg120, 00：GPIO13_5
#define REG_MUXCTRL_I2C2_SCL_MAP        (IO_MUX_REG_BASE + 0x074)  //muxctrl_reg29, 0：GPIO9_1；1：I2C2_SCL。
#define REG_MUXCTRL_I2C2_SDA_MAP        (IO_MUX_REG_BASE + 0x070)  //muxctrl_reg28, 0：GPIO9_0；1：I2C2_SDA。

#define REG_MUXCTRL_I2C0_SCL_MAP        (IO_MUX_REG_BASE + 0x050)
#define REG_MUXCTRL_I2C0_SDA_MAP        (IO_MUX_REG_BASE + 0x054)

#endif

