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

/**@defgroup los_err Error handling
 * @ingroup kernel
 */

#ifndef _LOS_TRACE_H
#define _LOS_TRACE_H

#include "los_typedef.h"
#include "los_config.h"
#include "los_hwi.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

typedef enum
{
    TRACE_ISR_FLAG   =   0x10,
    TRACE_IRQ_FLAG   =   0x20,
    TRACE_TASK_FLAG  =   0x40,
    TRACE_WORKQ_FLAG =   0x80,
    TRACE_SWTMR_FLAG =   0x100,
    TRACE_MUX_FLAG   =   0x200,
    TRACE_SEM_FLAG   =   0x400,
    TRACE_QUE_FLAG   =   0x800,
    TRACE_EVENT_FLAG =   0x1000
}LOS_TRACE_MASK;

typedef enum
{
    TRAP_INT        = TRACE_ISR_FLAG,

    ENABLE_IRQ      = TRACE_IRQ_FLAG|0,
    DISABLE_IRQ     = TRACE_IRQ_FLAG|1,
   
    CREATE_TSK      = TRACE_TASK_FLAG|0,
    SCHEDULE_TSK    = TRACE_TASK_FLAG|1,
    DELETE_TSK      = TRACE_TASK_FLAG|2,
    
    ENQUEUE_WORKQ   = TRACE_WORKQ_FLAG|0,
    SCHEDULE_WORKQ  = TRACE_WORKQ_FLAG|1,
    DEQUEUE_WORKQ   = TRACE_WORKQ_FLAG|2,

    CREATE_SWTMR    = TRACE_SWTMR_FLAG|0,
    START_SWTMR     = TRACE_SWTMR_FLAG|1,
    MODIFY_SWTMR    = TRACE_SWTMR_FLAG|2,
    SCHDING_SWTMR   = TRACE_SWTMR_FLAG|3,
    SCHDED_SWTMR    = TRACE_SWTMR_FLAG|4,
    STOP_SWTMR      = TRACE_SWTMR_FLAG|5,
    DEL_SWTMR       = TRACE_SWTMR_FLAG|6,
    DROP_SWTMR      = TRACE_SWTMR_FLAG|7,

    MUX_CREATE      = TRACE_MUX_FLAG|0,
    MUX_PEND        = TRACE_MUX_FLAG|1,
    MUX_POST        = TRACE_MUX_FLAG|2,
    MUX_DEL         = TRACE_MUX_FLAG|3,

    SEM_CREATE      = TRACE_SEM_FLAG|0,
    SEM_PEND        = TRACE_SEM_FLAG|1,
    SEM_POST        = TRACE_SEM_FLAG|2,
    SEM_DEL         = TRACE_SEM_FLAG|3,
    
    QUE_CREATE      = TRACE_QUE_FLAG|0,
    QUE_WRITE       = TRACE_QUE_FLAG|1,
    QUE_READ        = TRACE_QUE_FLAG|2,
    QUE_DEL         = TRACE_QUE_FLAG|3,

    EVENT_CREATE    = TRACE_EVENT_FLAG|0,
    EVENT_WRITE     = TRACE_EVENT_FLAG|1,
    EVENT_READ      = TRACE_EVENT_FLAG|2,
    EVENT_CLEAR     = TRACE_EVENT_FLAG|3,
    EVENT_DEL       = TRACE_EVENT_FLAG|4,
}LOS_TRACE_TYPE;

/**
 * @ingroup los_config
 * 支持用户自定义的cpu占用率统计开关
 */
#define LOSCFG_KERNEL_USER_CPUP                               YES
#if (LOSCFG_KERNEL_USER_CPUP == YES)

#define LOSCFG_WORKQ_TRACE_ENABLE           YES
#define LOSCFG_SWTMR_TRACE_ENABLE           YES
#define LOSCFG_TASK_TRACE_ENABLE            YES
#define LOSCFG_ISR_TRACE_ENABLE             YES
#define LOSCFG_SEM_TRACE_ENABLE             YES
#define LOSCFG_MUX_TRACE_ENABLE             YES
#define LOSCFG_QUEUE_TRACE_ENABLE           YES
#define LOSCFG_EVENT_TRACE_ENABLE           YES
#define LOSCFG_IRQ_TRACE_ENABLE             YES

typedef VOID (*USERCPUPHOOK)(UINT32 trace_type, UINT32 data0, UINT32 data1, UINT32 entry);
extern USERCPUPHOOK  g_pfnUserCpupHook;
extern LOS_TRACE_MASK  g_trace_type;
/**
 *@ingroup los_task
 *@brief Register an cpu usage statistics handling hook.
 *
 *@par Description:
 *This API is used to register an cpu usage statistics handling hook.
 *@attention If the hook is registered for multiple times, the hook registered at the last time is effective.
 *@attention The hook can be registered as NULL, indicating that the hook registration is canceled.
 *@param pfnHook [IN] Type #USERCPUPHOOK: hook function.
 *
 *@retval #LOS_OK                      The exception handling hook is successfully registered.
 *
 *@par Dependency:
 *los_task.h: the header file that contains the API declaration.
 *@see None.
 *@since Huawei LiteOS V200R001C00
 */
extern UINT32 LOS_RegCpupHook(USERCPUPHOOK pfnHook);

/**
 *@ingroup los_task
 *@set the trace type.
 *
 *@par Description:
 *This API is used to set the trace type.
 *@param type   [IN]   trace type.
 *
 *@retval None.
 *
 *@par Dependency:
 *los_trace.h: the header file that contains the API declaration.
 *@see None.
 *@since Huawei LiteOS V200R001C00
 */
void LOS_SetTraceType(LOS_TRACE_MASK type);

/**
 *@ingroup los_task
 *@get the trace type.
 *
 *@par Description:
 *This API is used to get the current trace type.
 *@param None.
 *
 *@retval #LOS_TRACE_MASK.
 *
 *@par Dependency:
 *los_trace.h: the header file that contains the API declaration.
 *@see None.
 *@since Huawei LiteOS V200R001C00
 */

LOS_TRACE_MASK LOS_GetTraceType(void);

#endif

/****************************************************************/
#if defined(LOSCFG_IRQ_TRACE_ENABLE)&&(LOSCFG_IRQ_TRACE_ENABLE == YES)
#define los_trace_irq(type, lr) \
do { \
    if ((g_pfnUserCpupHook != NULL) && (type&(UINT32)g_trace_type)) \
    { \
        g_pfnUserCpupHook(type, 0, 0,lr); \
    }\
}while(0)
#else
#define los_trace_irq(type, lr)
#endif
/****************************************************************/
#if defined(LOSCFG_WORKQ_TRACE_ENABLE)&&(LOSCFG_WORKQ_TRACE_ENABLE == YES)
#define los_trace_workq(type,work_prio,work_entry) \
do { \
    if ((g_pfnUserCpupHook != NULL) && (type&g_trace_type)) \
    { \
        g_pfnUserCpupHook(type, work_prio, 0,work_entry); \
    }\
}while(0)
#else
#define los_trace_workq(type,work_prio,work_entry)
#endif

/****************************************************************/
#if defined(LOSCFG_TASK_TRACE_ENABLE)&&(LOSCFG_TASK_TRACE_ENABLE == YES)
#define los_trace_task(type, task_id) \
do { \
    if ((g_pfnUserCpupHook != NULL)) \
    { \
        g_pfnUserCpupHook(type, task_id, 0, 0); \
    }\
}while(0)
#else
#define los_trace_task(type,task_id)
#endif

/***************************************************************/
#if defined(LOSCFG_ISR_TRACE_ENABLE)&&(LOSCFG_ISR_TRACE_ENABLE == YES)
#define los_trace_isr(type, int_vector) \
do { \
    if ((g_pfnUserCpupHook != NULL)) \
    { \
        g_pfnUserCpupHook(type, int_vector, 0, 0); \
    }\
}while(0)
#else
#define los_trace_isr(type,int_vector)
#endif
/****************************************************************/

#if defined(LOSCFG_SEM_TRACE_ENABLE)&&(LOSCFG_SEM_TRACE_ENABLE == YES)
#define los_trace_sem(type, sem_id, sem_cnt) \
do { \
    if ((g_pfnUserCpupHook != NULL) && (type&g_trace_type)) \
    { \
        UINT32 entry; \
        __asm__ __volatile__("MOV %0, LR": "=r"(entry)); \
        g_pfnUserCpupHook(type, sem_id, sem_cnt, entry); \
    }\
}while(0)
#else
#define los_trace_sem(type, sem_id, sem_cnt)
#endif
/****************************************************************/

#if defined(LOSCFG_SWTMR_TRACE_ENABLE)&&(LOSCFG_SWTMR_TRACE_ENABLE == YES)
#define los_trace_swtmr_safe(type, tmr_id, time, entry) \
do { \
    UINT32 uvIntSave; \
    uvIntSave = LOS_IntLock(); \
    if ((g_pfnUserCpupHook != NULL) && (type&g_trace_type)) \
    { \
        g_pfnUserCpupHook(type, tmr_id, time, entry); \
    }\
    LOS_IntRestore(uvIntSave);\
}while(0)
#define los_trace_swtmr(type, tmr_id, time, entry) \
do { \
    if ((g_pfnUserCpupHook != NULL) && (type&g_trace_type)) \
    { \
        g_pfnUserCpupHook(type, tmr_id, time, entry); \
    }\
}while(0)
#else
#define los_trace_swtmr_safe(type, tmr_id, time, entry)
#endif

/****************************************************************/
#if defined(LOSCFG_QUEUE_TRACE_ENABLE)&&(LOSCFG_QUEUE_TRACE_ENABLE == YES)
#define los_trace_queue(type, ret) \
do { \
    if ((g_pfnUserCpupHook != NULL) && (type&g_trace_type)) \
    { \
        UINT32 entry; \
        __asm__ __volatile__("MOV %0, LR": "=r"(entry)); \
        g_pfnUserCpupHook(type, ret, 0,entry); \
    }\
}while(0)
#else
#define los_trace_queue(type,ret)
#endif
/****************************************************************/
#if defined(LOSCFG_EVENT_TRACE_ENABLE)&&(LOSCFG_EVENT_TRACE_ENABLE == YES)
#define los_trace_event(type, value) \
do { \
    if ((g_pfnUserCpupHook != NULL) && (type&g_trace_type)) \
    { \
        UINT32 entry; \
        __asm__ __volatile__("MOV %0, LR": "=r"(entry)); \
        g_pfnUserCpupHook(type, value, 0,entry); \
    }\
}while(0)
#else
#define los_trace_event(type,value)
#endif
/****************************************************************/
#if defined(LOSCFG_MUX_TRACE_ENABLE)&&(LOSCFG_MUX_TRACE_ENABLE == YES)
#define los_trace_mux(type, id, cnt) \
do { \
    if ((g_pfnUserCpupHook != NULL) && (type&g_trace_type)) \
    { \
        UINT32 entry; \
        __asm__ __volatile__("MOV %0, LR": "=r"(entry)); \
        g_pfnUserCpupHook(type, id, cnt,entry); \
    }\
}while(0)
#else
#define los_trace_mux(type, id, cnt)
#endif
/****************************************************************/

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _LOS_ERR_H */
