#ifndef __HRTIMER_DEF_H__
#define __HRTIMER_DEF_H__

/* On board HI3911 FPGA, we use timer2 as a test hrtimer 
 * which register base is 0x101e2014, clock 37.5MHz, irqnr is 5
 */
//#include <vxWorks.h>
//#define USE_ONEBUILD /*used in integrator.h*/
//#include "integrator.h"
#if 1
#include <hi_platform.h>
#include <hi_drv_com.h>
#include <timer.h>
#define DRV_DFX_SYS_VAL_DECLARE
#include <drv_dfx_sys_val.h>
#undef DRV_DFX_SYS_VAL_DECLARE
//#include "int/esal_ge_defs.h"
//#include <hisoc/timer.h>
#endif /* __VX_PORTING_HRTIMER__ */

/* precision set to 5 us 
 * timers expires delta value less than 5 us 
 * will be expired at the same time.
 */
#define PRECISION_DELT	5
#define HRTIMER_REG_BASE	(TIMER0_REG_BASE+TIMER_LOAD)
/* BEGIN: Added by wuxianfeng 190206, 2012/7/10   PN:DTS2012070702350*/
#define HRDELAY_REG_BASE	(TIMER2_REG_BASE+TIMER_LOAD)
/* END:   Added by wuxianfeng 190206, 2012/7/10 */

#define HRTIMER_IRQNR		HI_IRQ_TIMER0

#define         HIS_TMR_OS_CLOCK_BASE              HI_XTAL_CLOCK_BASE
#define         HIS_TMR_OS_CLOCK_RATE              (HIS_TMR_OS_CLOCK_BASE * 1000000)

#define HRTIMER_CLOCK_RATE	HIS_TMR_OS_CLOCK_RATE

#define LOADCOUNT_REG		(0x00)
#define CURRCOUNT_REG		(0x04)
#define CONTROL_REG			(0x08)
#define EOI_REG				(0x0c)
#define INTSTATUS_REG		(0x10)


#define  ENABLE_BIT		(1 << 0)
#define  ONESHOT_BIT	(1 << 1)
#define  INTEN_BIT		(1 << 2)
#define  LOCK_BIT		(1 << 3)
/* BEGIN: Added by wuxianfeng 190206, 2012/7/10   PN:DTS2012070702350*/
#define  HAVE_INT		(1 << 0)
/* END:   Added by wuxianfeng 190206, 2012/7/10 */
#define HRTIMER_TEST 0

#undef  OK
#define OK 0

#endif
