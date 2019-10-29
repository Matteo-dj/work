#ifndef	__HISOC_TIMER_H__
#define	__HISOC_TIMER_H__

#include "asm/platform.h"

#define REG_TIMER_CONTROL		0x8
#define REG_TIMER_RELOAD		0x0
#define REG_TIMER_VALUE			0x4

#define TIMER_LOAD			0x0
#define TIMER_VALUE			0x4
#define TIMER_CONTROL		0x8
#define TIMER_INT_CLR		0xc
#define TIMER_RIS			0x10

#endif

