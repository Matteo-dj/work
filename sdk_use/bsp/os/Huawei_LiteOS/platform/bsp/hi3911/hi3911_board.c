/*----------------------------------------------------------------------------
 * Copyright (c) <2013-2015>, <Huawei Technologies Co., Ltd>
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific prior written
 * permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *---------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
 * Notice of Export Control Law
 * ===============================================
 * Huawei LiteOS may be subject to applicable export control laws and regulations, which might
 * include those applicable to Huawei LiteOS of U.S. and the country in which you are located.
 * Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance with such
 * applicable export control laws and regulations.
 *---------------------------------------------------------------------------*/

#include "los_tick.ph"
#include "los_typedef.h"
#include "los_printf.h"
#include "los_hwi.h"
#include "los_trace.h"
#include "hisoc/timer.h"
#include "hisoc/cpu.h"
#include "asm/hal_platform_ints.h"
#include "asm/platform.h"
#include "los_memory.h"
#include "los_hw_tick_minor.h"
#include "linux/interrupt.h"
#ifdef LOSCFG_LIB_LIBC
#include "string.h"
#endif /* LOSCFG_LIB_LIBC */
#ifdef LOSCFG_KERNEL_TICKLESS
#include "los_tickless.ph"
#endif
#include "los_exc.h"

/*lint -e40*/
__attribute__((weak)) void board_config(void)
{
    g_sys_mem_addr_end = 0x70800000;
}
//----------------------------------------------------------------------------
// Platform specific initialization
static UINT32 timestamp;
static UINT32 lastdec;

UINT32 read_timer(UINT32 index)
{
    UINT32 offset = index*0x14;
    WRITE_UINT32(((1<<0)|(1<<1)|(1<< 3)), TIMER0_REG_BASE + offset + TIMER_CONTROL);
    while(((*(volatile unsigned long *) (TIMER0_REG_BASE + offset + TIMER_CONTROL)) & (1<< 3)) != 0);
    return *(volatile unsigned long *)(TIMER0_REG_BASE + offset + TIMER_VALUE);
}
       
/*lint -e40*/
#define READ_TIMER  (read_timer(SYS_TIMER_NUM))
    

VOID reset_timer_masked(VOID)
{
	lastdec = READ_TIMER;
	timestamp = 0;
}

UINT32 get_timer_masked(VOID)
{
	UINT32 now = READ_TIMER;
	if (lastdec >= now) {
		/* not roll back */
		timestamp += lastdec - now;
	} else {
		/* rollback */
		timestamp += lastdec + (g_uwSysClock / LOSCFG_BASE_CORE_TICK_PER_SECOND) - now;
	}

	lastdec = now;
	return timestamp;
}


UINT32 time_clk_read(VOID)
{
    UINT32 value = read_timer(2);

    value = 0UL - value;

    return value;
}

unsigned int arch_timer_rollback(VOID)
{
	UINT32 flag;

    READ_UINT32(flag, TIMER_TICK_REG_BASE + TIMER_RIS);
	return flag;
}

UINT32 GetTimer2Value(VOID)
{
    return 0;
}

VOID los_bss_init(unsigned int bss_start,unsigned int bss_end)
{
    memset((unsigned int *)bss_start, 0, (bss_end - bss_start));
}

VOID hi3911_start_timer(UINT32 timer_index, UINT32 period, UINT32 mode, UINT32 enable)
{
    
    UINT32 offset = timer_index*0x14;
      /* disable first */
    WRITE_UINT32(0x0, TIMER0_REG_BASE + offset + TIMER_CONTROL);
    WRITE_UINT32(period, TIMER0_REG_BASE + offset + TIMER_LOAD);

    WRITE_UINT32(mode|enable, TIMER0_REG_BASE + offset + TIMER_CONTROL);
}

#ifdef LOSCFG_KERNEL_TICKLESS
VOID tickless_start(UINT32 period)
{
    hi3911_start_timer(SYS_TIMER_NUM, period, TIMER_MODE_PERIODIC, TIMER_ENABLE);
}
#endif

VOID hal_clock_initialize(UINT32 period)
{
    UINT32 uwRet = 0;
    
    hi3911_start_timer(SYS_TIMER_NUM, period, TIMER_MODE_PERIODIC, 0);
    
    if (uwRet != LOS_HwiCreate(NUM_HAL_INTERRUPT_TIMER, 0xa0, 0, (HWI_PROC_FUNC)osTickHandler, 0))
    {
        PRINT_ERR("LOS_HwiCreate error");
        return;
    }
}

VOID hrtimer_clock_irqclear(VOID)
{
    UINT32 eio = 0;/*lint -e438*/
    READ_UINT32(eio,TIMER3_REG_BASE + TIMER_INT_CLR);
    ((void)(eio));
}


VOID hrtimer_clock_start(UINT32 period)
{
    hi3911_start_timer(HR_TIMER_NUM, period, TIMER_MODE_PERIODIC, TIMER_ENABLE);
}

VOID hrtimer_clock_stop(VOID)
{
   UINT32 temp;
   READ_UINT32(temp,TIMER3_REG_BASE + TIMER_CONTROL);
   temp = temp & (~1);
   WRITE_UINT32(temp, TIMER3_REG_BASE + TIMER_CONTROL);
}

UINT32 get_hrtimer_clock_value(VOID)
{
    return read_timer(HR_TIMER_NUM);
}

VOID hrtimer_clock_initialize(VOID)
{
    WRITE_UINT32(0x0, TIMER3_REG_BASE + TIMER_CONTROL);
}

VOID hal_clock_irqclear(VOID)
{
    UINT32 eio = 0;/*lint -e438*/
    READ_UINT32(eio,TIMER_TICK_REG_BASE + TIMER_INT_CLR);
    ((void)(eio));
}

VOID hal_clock_enable(VOID)
{
    /*open HI3911 TIMER0*/
     GET_UINT32(TIMER_TICK_REG_BASE + TIMER_CONTROL) = GET_UINT32(TIMER_TICK_REG_BASE + TIMER_CONTROL) | (1 << 0);
}

// Delay for some number of micro-seconds

UINT32 get_timer(UINT32 base)
{
	return get_timer_masked() - base;
}

VOID hal_delay_us(UINT32 usecs)
{
    UINT32 tmo = 0;
    UINT32 tmp = 0;
    UINT32 uwRet;
    uwRet = LOS_IntLock();

    if (usecs >= 1000) {
        /* start to normalize for usec to ticks per sec */
        tmo = usecs / 1000;

        /* tmo *= (50000000 / 16)/ 1000; */
        tmo *= g_uwSysClock / 1000;//largest msecond 1374389

        /* usecs < 1000 */
        usecs -= usecs / 1000 * 1000;
    }

    /* usecs < 1000 */
    if (usecs) {
        tmo += (usecs * g_uwSysClock) / 1000 / 1000;
    }

    /* reset "advancing" timestamp to 0, set lastdec value */
    reset_timer_masked();

    tmp = get_timer(0);     /* get current timestamp */

    /* set advancing stamp wake up time */
    tmo += tmp;

    while (get_timer_masked() < tmo);

    LOS_IntRestore(uwRet);
}

VOID LOS_Udelay(UINT32 usecs)
{
    hal_delay_us(usecs);
}

VOID LOS_Mdelay(UINT32 msecs)
{
    LOS_Udelay(msecs * 1000);
}

// -------------------------------------------------------------------------
// This routine is called to respond to a hardware interrupt (IRQ).  It
// should interrogate the hardware and return the IRQ vector number.

extern UINT32 g_vuwHwiFormCnt[OS_HWI_MAX_NUM];
extern HWI_HANDLE_FORM_S m_astHwiForm[];
extern VOID LOS_SysTimeUpdate(UINT32 uwSleepTicks);
void hal_interrupt_acknowledge(unsigned int vector);
UINT32 CLZ(UINT32 uvIntSave) /*lint -e533*/
{
    UINT32 uwRet = 0;/*lint -e438*/
    __asm__ __volatile__("CLZ   %0, %1":"=r"(uwRet):"r"(uvIntSave): );
    
    return uwRet;/*lint -e529*/
}
UINT32 g_uwreservecycle =0;

#ifdef LOSCFG_KERNEL_TICKLESS
VOID tickless_update(UINT32 irqnum)
{
    UINT32 cycles,ticks;
    
    if(irqnum == NUM_HAL_INTERRUPT_TIMER)
    {
        LOS_SysTimeUpdate(g_uwSleepTicks);
        g_uwreservecycle = 0;        
    }
    else
    {
        cycles = read_timer(SYS_TIMER_NUM);
        cycles = ((g_uwSleepTicks * CYCLES_PER_TICK) - cycles);
        ticks = cycles / CYCLES_PER_TICK;
        cycles = cycles % CYCLES_PER_TICK;
        LOS_SysTimeUpdate(ticks + 1);
        tickless_start(CYCLES_PER_TICK - cycles);
        g_uwreservecycle = 1;        
    }
    g_uwSleepTicks = 0;
}
#endif

typedef struct 
{
    UINT32 cpsr;
    UINT32 reg[13];
    UINT32 lr;
    UINT32 pc;
}LAST_TASK_LISR_CONTEXT_S;

EXC_CONTEXT_S g_stLastTaskLisrContext;

void hal_save_last_context(UINT32 ulSp,UINT32 ulIntCount)
{
    LAST_TASK_LISR_CONTEXT_S * pStack=(LAST_TASK_LISR_CONTEXT_S *)ulSp;
    g_stLastTaskLisrContext.uwCPSR=pStack->cpsr;
    memcpy(g_stLastTaskLisrContext.uwR,pStack->reg,sizeof(UINT32)*13);
    g_stLastTaskLisrContext.uwR[13]=ulSp+sizeof(LAST_TASK_LISR_CONTEXT_S);
    g_stLastTaskLisrContext.uwR[14]=pStack->lr;
    g_stLastTaskLisrContext.uwR[15]=pStack->pc;
}

void LOS_GetLastContextInLisr(EXC_CONTEXT_S *pConText)
{
    if(pConText!=NULL)
        memcpy(pConText,&g_stLastTaskLisrContext,sizeof(EXC_CONTEXT_S));
}

void hal_IRQ_handler(UINT32 ulSp,UINT32 ulIntCount)
{
    INT32 int_num;
    UINT32  irqstatus;
    HWI_HANDLE_FORM_S *pstHwiForm;
    
    READ_UINT32(irqstatus, IRQ_FINALSTATUS_L); 

    if (irqstatus == 0)
    {
        return;
    }

    //看门狗中断
    if((irqstatus&0x1))
    {
        hal_save_last_context( ulSp, ulIntCount);
        pstHwiForm = &m_astHwiForm[0];
        if (NULL != pstHwiForm->pfnHook) {
            (pstHwiForm->pfnHook)(pstHwiForm->uwParam);
        }
        return;
    }

    //非看门狗中断,配置中断优先级为1
    (*(volatile unsigned *)(IRQ_PLEVEL))=0x1;
    LOS_IntUnLock();
    
    irqstatus = irqstatus & ((irqstatus - 1)^-1);
    int_num = OS_HWI_MAX_NUM - CLZ(irqstatus)- 1;

    pstHwiForm = &m_astHwiForm[int_num];
    los_trace_isr(TRAP_INT, int_num);
    
#ifdef LOSCFG_KERNEL_TICKLESS
    if(g_uwSleepTicks > g_uwMinSleepTicks)
    {
        tickless_update(int_num);
    }
#endif

    if (NULL != pstHwiForm->pfnHook)
    {
        (pstHwiForm->pfnHook)(pstHwiForm->uwParam);
    }
    
#ifdef LOSCFG_SHELL
    ++g_vuwHwiFormCnt[int_num];
#endif

#ifdef LOSCFG_TEST
    hal_interrupt_acknowledge(int_num);
#endif

    //恢复中断优先级为0
    LOS_IntLock();
    (*(volatile unsigned *)(IRQ_PLEVEL))=0x0;
}
//----------------------------------------------------------------------------
// Interrupt control
void hal_interrupt_mask(unsigned int vector)
{
    UINT32 imr = 1 << vector;
    UINT32 status;

    READ_UINT32(status, IRQ_INTMASK_L);
    WRITE_UINT32(imr | status, IRQ_INTMASK_L);
}

void hal_interrupt_unmask(unsigned int vector)
{
    UINT32 imr = 1 << vector;
    UINT32 status;

    READ_UINT32(status, IRQ_INTEN_L);
    WRITE_UINT32(imr | status, IRQ_INTEN_L);

    imr = ~(1 << vector);/*lint !e502*/
    READ_UINT32(status, IRQ_INTMASK_L);
    WRITE_UINT32(imr & status, IRQ_INTMASK_L);
}

#ifdef LOSCFG_TEST
void hal_interrupt_acknowledge(unsigned int vector)
{
    UINT32 imr = ~(1 << vector);/*lint !e502*/
    UINT32 status = 0;

    if (vector > OS_USER_HWI_MAX || vector < OS_USER_HWI_MIN) /*lint !e685 !e568*/
    {
        return;
    }

    READ_UINT32(status, IRQ_INTFORCE_L);
    WRITE_UINT32(imr & status, IRQ_INTFORCE_L);
}
#endif
void hal_interrupt_configure(unsigned int vector, int level, int up)
{
}

void hal_interrupt_set_level(unsigned int vector, int level)
{
}

VOID hal_interrupt_pending(unsigned int vector)
{
    UINT32 imr = 1 << vector;
    UINT32 status;

    READ_UINT32(status, IRQ_INTFORCE_L);
    WRITE_UINT32(imr | status, IRQ_INTFORCE_L);
}

void hal_interrupt_init(void)
{
    WRITE_UINT32(0xffffffff, IRQ_INTEN_L);      /* enable all irq */
    WRITE_UINT32(0xffffffff, IRQ_INTMASK_L);    /* mask all irq */
}
/*restore*/

UINT32 random_stack_guard(VOID)
{
    UINT32 uwHwCycle = SysTick->uwVALUE;
    return uwHwCycle;
}

VOID tick_interrupt_mask(VOID)
{
    hal_interrupt_mask(NUM_HAL_INTERRUPT_TIMER);
}

VOID tick_interrupt_unmask(VOID)
{
    hal_interrupt_unmask(NUM_HAL_INTERRUPT_TIMER);
}
