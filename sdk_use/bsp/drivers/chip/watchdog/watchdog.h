/*
 * for watchdog driver .
 *
 * History: 
 *      2012-02-16 create by z00193780
 */ 
 
#ifndef __WATCHDOGH_H__
#define __WATCHDOGH_H__

//#define USE_ONEBUILD /*used in integrator.h*/
//#include "integrator.h"
#ifndef __VX_PORTING_WATCHDOG__
#include <hi_platform.h>
#include <hi_drv_com.h>
#endif /* __VX_PORTING_WATCHDOG__ */

#include "los_hwi.h"
/* BEGIN: Added by zhangna, 2012/5/26   PN:DTS2012052604463*/
#define WDG_RESET_TIME_MAX ((HI_U32)((((HI_U32)1 << 31)/g_cfg_apb_clock)* 1000))  //0xDFB2(ms)
/* END:   Added by zhangna, 2012/5/26 */ 

//wdg reg offset
#define WDT_CR   (0x00)
#define WDT_TORR   (0x04)
#define WDT_CRR    (0x0c)

//macros

#define WDG_CLK_mS (g_cfg_apb_clock/1000)
#define WDG_BIT_0_IS_1  (0x1)
#endif

