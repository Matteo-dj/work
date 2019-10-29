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

/**@defgroup los_exc Exception handling
  *@ingroup kernel
*/

#ifndef _LOS_EXC_H
#define _LOS_EXC_H

#include "los_hwi.h"
#include "los_task.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/* Define the phase in which an exception occurs */
//#define OS_EXC_IN_INIT              0
//#define OS_EXC_IN_TASK              1
//#define OS_EXC_IN_HWI               2


/**
 *@ingroup los_exc
 * Register information structure
 *
 * Description: register information stored when an exception occurs on an LPC2458 platform.
 *
 * Note: The following register names without uw are the register names used in the chip manual.
 */
typedef struct tagExcContext
{
    UINT32 uwCPSR;        /**< Current program status register (CPSR) */
    UINT32 uwR[16];       /**< Register R0 - R12, SP, LR, PC          */
}EXC_CONTEXT_S;
/**
 *@ingroup los_exc
 * Exception information structure
 *
 * Description: exception information stored when an exception occurs on an LPC2458 platform.
 *
 */
typedef struct tagExcInfo
{
    UINT16 usPhase;                    /**<  Phase in which an exception occurs*/
    UINT16 usType;                     /**<  Exception type */
    UINT16 usNestCnt;                  /**<  Count of nested exception */
    UINT16 usReserved;                 /**<  Reserved for alignment */
    EXC_CONTEXT_S * pstContext;        /**<  Hardware context when an exception occurs */
}EXC_INFO_S;

/**
 *@ingroup los_exc
 *ARM7 exception type: undefined instruction exception.
 */
#define  OS_EXCEPT_UNDEF_INSTR                 1
/**
 *@ingroup los_exc
 *ARM7 exception type: software interrupt.
 */
#define  OS_EXCEPT_SWI                         2
/**
 *@ingroup los_exc
 *ARM7 exception type: prefetch abort exception.
 */
#define  OS_EXCEPT_PREFETCH_ABORT              3
/**
 *@ingroup los_exc
 *ARM7 exception type: data abort exception.
 */
#define  OS_EXCEPT_DATA_ABORT                  4
/**
 *@ingroup los_exc
 *ARM7 exception type: FIQ exception.
 */
#define  OS_EXCEPT_FIQ                         5

/**
 *@ingroup los_exc
 *@brief Kernel FP Register address obtain function.
 *
 *@par Description:
 *The API is used to obtain the FP Register address.
 *@attention None.
 *
 *@param  None.
 *
 *@retval #UINTPTR The FP Register address.
 *
 *@par Dependency:
 *los_exc.h: the header file that contains the API declaration.
 *@see None.
 *@since Huawei LiteOS V100R001C00
*/
INLINE UINTPTR Get_Fp(VOID)
{
    UINTPTR uvFpSave;

    __asm__ __volatile__(
        "mov %0, fp"
        : "=r"(uvFpSave));

    return uvFpSave; /*lint !e530*/
}

/**
*@ingroup los_exc
*@brief Define an exception handling function hook.
*
*@par Description:
*This API is used to define the exception handling function hook based on the type of the exception handling function and record exceptions.
*@attention None.
*
*@param None.
*
*@retval None.
*
*@par Dependency:
*los_exc.h: the header file that contains the API declaration.
*@see None.
*@since Huawei LiteOS V100R001C00
*/
typedef VOID (* EXC_PROC_FUNC)(UINT32, EXC_CONTEXT_S *);

/**
 *@ingroup los_exc
 *@brief Register an exception handling hook.
 *
 *@par Description:
 *This API is used to register an exception handling hook.
 *@attention If the hook is registered for multiple times, the hook registered at the last time is effective.
 *@attention The hook can be registered as NULL, indicating that the hook registration is canceled.
 *@param pfnHook [IN] Type #EXC_PROC_FUNC: hook function.
 *
 *@retval #LOS_OK                      The exception handling hook is successfully registered.
 *
 *@par Dependency:
 *los_exc.h: the header file that contains the API declaration.
 *@see None.
 *@since Huawei LiteOS V100R001C00
 */
extern UINT32 LOS_ExcRegHook( EXC_PROC_FUNC pfnHook );

/**
 *@ingroup los_exc
 *@brief Kernel panic function.
 *
 *@par Description:
 *Stack function that prints kernel panics.
 *@attention After this function is called and stack information is printed, the system will fail to respond.
 *@attention The input parameter can be NULL.
 *@param fmt [IN] Type #char* : variadic argument.
 *
 *@retval #None.
 *
 *@par Dependency:
 *los_exc.h: the header file that contains the API declaration.
 *@see None.
 *@since Huawei LiteOS V100R001C00
*/
void LOS_Panic(const char * fmt, ...);

/**
 *@ingroup los_exc
 *@brief Kernel backtrace function.
 *
 *@par Description:
 *Backtrace function that prints task call stack information traced from the running task.
 *@attention None.
 *
 *@param None.
 *
 *@retval #None.
 *
 *@par Dependency:
 *los_exc.h: the header file that contains the API declaration.
 *@see None.
 *@since Huawei LiteOS V100R001C00
*/
extern VOID osBackTrace(VOID);

/**
 *@ingroup los_exc
 *@brief Kernel task backtrace function.
 *
 *@par Description:
 *Backtrace function that prints task call stack information traced from the input task.
 *@attention
 *<ul>
 *<li>The input uwTaskID should be valid.</li>
 *</ul>
 *
 *@param  uwTaskID [IN] Type #UINT32 Task ID.
 *
 *@retval #None.
 *
 *@par Dependency:
 *los_exc.h: the header file that contains the API declaration.
 *@see None.
 *@since Huawei LiteOS V100R001C00
*/
extern void osTaskBackTrace(UINT32 uwTaskID);

//extern UINT32 g_vuwIntCount;
//extern EXC_INFO_S m_stExcInfo;

//#define OS_EXC_MAX_NEST_DEPTH       1

//extern EXC_PROC_FUNC g_pExcHook;


extern void LOS_GetLastContextInLisr(EXC_CONTEXT_S *pConText);


#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _LOS_EXC_H */
