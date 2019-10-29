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

#include "stdarg.h"
#include "los_exc.h"
#include "los_task.ph"
#include "los_printf.ph"
#include "los_hw.ph"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define OS_MAX_BACKTRACE 15

VOID osExcHook(UINT32 uwExcType, EXC_CONTEXT_S *puwExcBufAddr);
VOID osExcStackInfo(VOID);
VOID BackTrace(UINT32 uwFP);

EXC_PROC_FUNC g_pExcHook = (EXC_PROC_FUNC)osExcHook;
LITE_OS_SEC_BSS_MINOR EXC_INFO_S m_stExcInfo;
LITE_OS_SEC_DATA_MINOR UINT32 g_uwCurNestCount = 0;/*lint -e552*/


static UINT32 osShellCmdTskInfoGetRef(UINT32 uwTaskID, UINT32 TransId) __attribute__((weakref("osShellCmdTskInfoGet"))); /*lint -e402*/

/*****************************************************************************
Function   : LOS_ExcRegHook
Description: register user hook function of EXC
Input   : pfnHook --- user hook function
Output  : None
Return  : None
Other   : None
*****************************************************************************/
LITE_OS_SEC_TEXT_INIT UINT32 LOS_ExcRegHook( EXC_PROC_FUNC pfnHook )
{
    UINTPTR uvIntSave;
    uvIntSave = LOS_IntLock();
    g_pExcHook = pfnHook;

    LOS_IntRestore(uvIntSave);
    return LOS_OK;
}

VOID osExcHook(UINT32 uwExcType, EXC_CONTEXT_S *puwExcBufAddr)
{
    UINT32 i;

    dprintf("uwExcType = 0x%x\n", uwExcType);

    dprintf("pc = 0x%x\n", puwExcBufAddr->uwR[15]);
    dprintf("lr = 0x%x\n", puwExcBufAddr->uwR[14]);
    dprintf("sp = 0x%x\n", puwExcBufAddr->uwR[13]);
    dprintf("fp = 0x%x\n", puwExcBufAddr->uwR[11]);

    /*undefinited exception handling*/
    if (uwExcType == OS_EXCEPT_UNDEF_INSTR)
    {
        PRINTK("step into undef\n");

        if (((puwExcBufAddr->uwCPSR) & 0x01000020) == 0) /*work status: ARM*/
        {
            puwExcBufAddr->uwR[15] = puwExcBufAddr->uwR[15] - 4;
        }
        else if (((puwExcBufAddr->uwCPSR) & 0x01000020) == 0x20) /*work status: Thumb*/
        {
            puwExcBufAddr->uwR[15] = puwExcBufAddr->uwR[15] - 2;
        }
    }

    BackTrace(puwExcBufAddr->uwR[11]);

    for(i = 0; i < 13; i++)
    {
        PRINTK("R%d = 0x%x\n", i, puwExcBufAddr->uwR[i]);
    }
    PRINTK("SP  = 0x%x\n", puwExcBufAddr->uwR[13]);
    PRINTK("LR  = 0x%x\n", puwExcBufAddr->uwR[14]);
    PRINTK("PC  = 0x%x\n", puwExcBufAddr->uwR[15]);

    dprintf("CPSR       = 0x%x\n", puwExcBufAddr->uwCPSR);
    dprintf("pcTaskName = %s\n", g_stLosTask.pstRunTask->pcTaskName);
    PRINTK("TaskID = %d\n", g_stLosTask.pstRunTask->uwTaskID);
    dprintf("Task StackSize = %d\n", g_stLosTask.pstRunTask->uwStackSize);
    PRINTK("system mem addr:0x%x\n",m_aucSysMem0);
    osExcStackInfo();
}

extern UINT8 *m_aucSysMem0;

extern UINT32 __startup_stack_top;
extern UINT32 __irq_stack_top;
extern UINT32 __fiq_stack_top;
extern UINT32 __svc_stack_top;
extern UINT32 __abt_stack_top;
extern UINT32 __undef_stack_top;
typedef struct StackInfo
{
   CHAR acStackName[8];
   UINT32 uwStackTop;
   UINT32 uwStackSize;
}STACK_INFO;
void osExcStackInfo(void)
{
    UINT32 uwFigNum = 0, uwI = 0;
    UINT32 uwIndex;
    UINT8 *p_t;

    STACK_INFO apstStackInfo[6] = {
    {"undef",  (UINT32)&__undef_stack_top,  OS_UNDEF_STACK_SIZE},
    {"abt  ",  (UINT32)&__abt_stack_top,  OS_ABT_STACK_SIZE},
    {"irq  ",  (UINT32)&__irq_stack_top, OS_IRQ_STACK_SIZE},
    {"fiq  ",  (UINT32)&__fiq_stack_top, OS_FIQ_STACK_SIZE},
    {"svc  ",  (UINT32)&__svc_stack_top,  OS_SVC_STACK_SIZE},
    {"start",  (UINT32)&__startup_stack_top, OS_STARTUP_STACK_SIZE}
    };

    PRINTK("\r\nstack   stack addr total size  used size\r\n"
             "------  ---------  ---------   --------\r\n");
    for(uwIndex = 0; uwIndex < 6; uwIndex++)
    {
        p_t = (UINT8 *)apstStackInfo[uwIndex].uwStackTop;
        for (uwI = 0, uwFigNum =0 ;uwI<apstStackInfo[uwIndex].uwStackSize;uwI++)
        {
            if (*(p_t-uwI) != 0)  /*lint !e676*///todo
                ++uwFigNum;
        }
        PRINTK("%s   0x%-10x 0x%-8x 0x%-4x\r\n",apstStackInfo[uwIndex].acStackName, p_t, uwI, uwFigNum);
    }
} /*lint !e438  !e550*/
#define UNUSED(a) ((void)(a))

LITE_OS_SEC_TEXT_MINOR void BackTrace(UINT32 uwFP)
{
    UINT32 uwBackFP = uwFP;
    UINT32 uwtmpFP;
    UINT32 uwBackLR;
    UINT32 uwCount = 0;
    PRINTK("*******backtrace begin*******\n");    
    while( uwBackFP>OS_SYS_FUNC_ADDR_START && uwBackFP<OS_SYS_FUNC_ADDR_END )
    {
        uwtmpFP = uwBackFP;
        uwBackLR = *((UINT32 *)(uwtmpFP));
        uwBackFP = *((UINT32 *)(uwtmpFP-4));
        PRINTK("trace %d -- lr = 0x%x\n", uwCount, uwBackLR);
        PRINTK("trace %d -- fp = 0x%x\n", uwCount, uwBackFP);
        uwCount++;
        if(uwCount == OS_MAX_BACKTRACE)
            break;
    }
	UNUSED(uwBackLR);
    if (osShellCmdTskInfoGetRef) /*lint !e506*/
    {
        (VOID)osShellCmdTskInfoGetRef(0xffffffff, 0);
    }

} /*lint !e438  !e550*/

LITE_OS_SEC_TEXT_MINOR void osTaskBackTrace(UINT32 uwTaskID)
{
    LOS_TASK_CB *pstTaskCB;

    if(uwTaskID >= g_uwTskMaxNum)
    {
        PRINT_ERR("\r\nTask PID is invalid!\n");
        return;
    }
    pstTaskCB = OS_TCB_FROM_TID(uwTaskID);

    if((pstTaskCB->usTaskStatus & OS_TASK_STATUS_UNUSED) \
            || (pstTaskCB->pfnTaskEntry == NULL)\
            || (pstTaskCB->pcTaskName == NULL))
    {
        PRINT_ERR("\r\nThe task is not created!\n");
        return;
    }
    PRINTK("TaskName = %s\n", pstTaskCB->pcTaskName);
    PRINTK("TaskID = 0x%x\n", pstTaskCB->uwTaskID);

    BackTrace(((TSK_CONTEXT_S  *)(pstTaskCB->pStackPointer))->auwR[11]); //0x130 is the offset of R11 relate to SP
}

LITE_OS_SEC_TEXT_MINOR void osBackTrace(void)
{
    UINT32 uwFp = Get_Fp();
    PRINTK("osBackTrace fp = 0x%x\n",uwFp);
    PRINTK("RunTask Name = %s\n", g_stLosTask.pstRunTask->pcTaskName);
    PRINTK("RunTask uwTaskID = %d\n", g_stLosTask.pstRunTask->uwTaskID);
    BackTrace(uwFp);
}

/*****************************************************************************
 Function    : osExcHandleEntry
 Description : EXC handler entry
 Input       : puwExcBufAddr ---address of EXC buf
 Output      : None
 Return      : None
 *****************************************************************************/
LITE_OS_SEC_TEXT_INIT VOID osExcHandleEntry(UINT32 uwExcType, EXC_CONTEXT_S *  puwExcBufAddr)
{
    g_uwCurNestCount++;
    if(g_pExcHook != NULL)
    {
        g_pExcHook(uwExcType, puwExcBufAddr);
    }
    while(1)
    {
        ;
    }
}


/************stack protector**************/
unsigned long __stack_chk_guard = 0xd00a0dff;

void __stack_chk_fail(void)
{
    LOS_Panic("[%u]stack-protector: Kernel stack is corrupted in: %p\n",
                100,__builtin_return_address(0)); /*lint -e628*/
    //__builtin_return_address is a builtin function, building in gcc
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
