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

#include "los_hw.ph"
#include "los_task.ph"
#include "los_memory.h"
#include "los_membox.h"
#include "los_priqueue.ph"
#include "asm/dma.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

VOID osSchedule(VOID)
{
    osTaskSchedule();
}
LITE_OS_SEC_TEXT_MINOR VOID LOS_Schedule(VOID)
{
    UINTPTR uvIntSave;
    uvIntSave = LOS_IntLock();
    g_stLosTask.pstNewTask = LOS_DL_LIST_ENTRY(osPriqueueTop(), LOS_TASK_CB, stPendList);/*lint !e413*/
    if (g_stLosTask.pstRunTask != g_stLosTask.pstNewTask)
    {
        if ((!g_usLosTaskLock) && (!OS_INT_ACTIVE))
        {
            (VOID)LOS_IntRestore(uvIntSave);
            osTaskSchedule();
            return;
        }
    }

    (VOID)LOS_IntRestore(uvIntSave);
}

LITE_OS_SEC_TEXT_INIT VOID osTaskExit(void)
{
    __asm__ __volatile__("swi  0");
}

/*****************************************************************************
 Function    : osTskStackInit
 Description : task stack initialization
 Input       : pfnTaskEntry -- task handler
               uwStackSize  -- task stack size
               pTopStack    -- stack top of task (low address)
 Output      : None
 Return      : OS_SUCCESS on success or error code on failure
 *****************************************************************************/
LITE_OS_SEC_TEXT_INIT VOID *osTskStackInit(UINT32 uwTaskID, UINT32 uwStackSize, VOID *pTopStack)
{
    UINT32 uwIdx;
    TSK_CONTEXT_S  *pstContext;

    /*initialize the task stack, write magic num to stack top*/
    for (uwIdx = 0; uwIdx < (uwStackSize/sizeof(UINT32)); uwIdx++)
    {
        *((UINT32 *)pTopStack + uwIdx) = OS_TASK_STACK_INIT;
    }
    *((UINT32 *)(pTopStack)) = OS_TASK_MAGIC_WORD;

    pstContext    = (TSK_CONTEXT_S *)(((UINT32)pTopStack + uwStackSize) - sizeof(TSK_CONTEXT_S));

    /*initialize the task context*/
    pstContext->uwPC    = (UINT32)osTaskEntry;
    pstContext->uwLR    = (UINT32)osTaskExit;             /* LR should be kept, to distinguish it's THUMB instruction or ARM instruction*/
    pstContext->auwR[0 ] = uwTaskID;      /* R0  */
    pstContext->auwR[1 ] = 0x01010101;      /* R1  */
    pstContext->auwR[2 ] = 0x02020202;      /* R2  */
    pstContext->auwR[3 ] = 0x03030303;      /* R3  */
    pstContext->auwR[4 ] = 0x04040404;      /* R4  */
    pstContext->auwR[5 ] = 0x05050505;      /* R5  */
    pstContext->auwR[6 ] = 0x06060606;      /* R6  */
    pstContext->auwR[7 ] = 0x07070707;      /* R7  */
    pstContext->auwR[8 ] = 0x08080808;      /* R8  */
    pstContext->auwR[9 ] = 0x09090909;      /* R9  */
    pstContext->auwR[10] = 0x10101010;      /* R10 */
    pstContext->auwR[11] = 0x11111111;      /* R11 */
    pstContext->auwR[12] = 0x12121212;      /* R12 */
#ifdef LOSCFG_INTERWORK_THUMB
    pstContext->uwPSR = PSR_MODE_SVC_THUMB;    /* CPSR  (Enable IRQ and FIQ interrupts, THUMNB-mode)    */
#else
    pstContext->uwPSR = PSR_MODE_SVC_ARM;    /* CPSR  (Enable IRQ and FIQ interrupts, ARM-mode)    */
#endif

    return (VOID *)pstContext;
}

void sev(void)
{

}

void wfe(void)
{

}

void wfi(void)
{
    __asm__ __volatile__ ("mcr p15, 0, %0, c7, c0, 4" : : "r" (0) : "memory");
}

void dmb(void)
{
    __asm__ __volatile__ ("mcr p15, 0, %0, c7, c10, 5" : : "r" (0) : "memory");
}

void dsb(void)
{
    __asm__ __volatile__ ("mcr p15, 0, %0, c7, c10, 4" : : "r" (0) : "memory");
}

void isb(void)
{
    __asm__ __volatile__ ("mcr p15, 0, %0, c7, c5, 4" : : "r" (0) : "memory");
}

void flush_icache(void)
{
    __asm__ __volatile__ ("mcr p15, 0, %0, c7, c5, 0" : : "r" (0) : "memory");
}

void flush_dcache(int start, int end)
{
    arm926_dma_clean_range(start, end);
}


#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */


