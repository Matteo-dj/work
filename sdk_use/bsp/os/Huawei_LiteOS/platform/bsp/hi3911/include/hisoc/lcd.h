#ifndef	__HISOC_LCD_H__
#define	__HISOC_LCD_H__

#include "asm/platform.h"

#ifdef DV_BOARD
//muxctrl_reg5 , 0x0:GPIO6_1 0x1:VO_DAT14
#define REG_MUXCTRL_LCD_ENABLE_MAP      IO_ADDRESS(IO_MUX_REG_BASE + 0x014)

//muxctrl_reg7 , 0x0:GPIO6_3 0x1:VO_DAT12
#define REG_MUXCTRL_LCD_RST_MAP     IO_ADDRESS(IO_MUX_REG_BASE + 0x01C)

#define ENABLE_GPIO_GROUP       6
#define ENABLE_GPIO_OFFSET      1

#define RST_GPIO_GROUP          6
#define RST_GPIO_OFFSET         3

#else

//muxctrl_reg55, 0x0: GPIO0_2, 0x1:pwm5
#define REG_MUXCTRL_LCD_RST_MAP     IO_ADDRESS(IO_MUX_REG_BASE + 0x0DC)

#define RST_GPIO_GROUP          0
#define RST_GPIO_OFFSET         2

#endif


#endif

