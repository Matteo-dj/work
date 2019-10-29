#ifndef	__HISOC_PWM_H__
#define	__HISOC_PWM_H__

#include "asm/platform.h"

#define REG_MUXCTRL_PWM0      IO_ADDRESS(IO_MUX_REG_BASE + 0x14c)  //muxctrl_reg83
#define REG_MUXCTRL_PWM1      IO_ADDRESS(IO_MUX_REG_BASE + 0x150)  //muxctrl_reg84
#define REG_MUXCTRL_PWM2      IO_ADDRESS(IO_MUX_REG_BASE + 0x154)  //muxctrl_reg85
#define REG_MUXCTRL_PWM3      IO_ADDRESS(IO_MUX_REG_BASE + 0x158)  //muxctrl_reg86
#define REG_MUXCTRL_PWM4      IO_ADDRESS(IO_MUX_REG_BASE + 0x0a4)  //muxctrl_reg41
#define REG_MUXCTRL_PWM5      IO_ADDRESS(IO_MUX_REG_BASE + 0x0dc)  //muxctrl_reg55
#define REG_MUXCTRL_PWM6      IO_ADDRESS(IO_MUX_REG_BASE + 0x0e0)  //muxctrl_reg56
#define REG_MUXCTRL_PWM7      IO_ADDRESS(IO_MUX_REG_BASE + 0x0e4)  //muxctrl_reg57

#define REG_CRG_PWM           IO_ADDRESS(CRG_REG_BASE + 0x38)             //PERI_CRG14

#define PWM_REG_BASE0         PWM03_REG_BASE
#define PWM_REG_BASE4         PWM47_REG_BASE

#define PWM_REG_BASE(n) \
	    (((n < 3) ? (PWM_REG_BASE0):(PWM_REG_BASE4)) + (n-4)*0x20)


#endif

