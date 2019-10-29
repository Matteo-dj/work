#ifndef __WATCHDOGH_H__
#define __WATCHDOGH_H__

#include <types.h>
#include <product.h>
/*!!!!need to delete   wuxianfeng*/
#define sysctrl_base_addr HI_PERIPHERAL_BASE
#define WDG_CLK CONFIG_PL011_CLOCK
#define WDG_CLK_uS (CONFIG_PL011_CLOCK/1000000)
#define WDG_BIT_0_IS_1  (0x1)

u32 watchdog_enable(u32 u32over_time);
void watchdog_disable(void);
void watchdog_feed(void);
#endif