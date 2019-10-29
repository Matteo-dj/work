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

#include "los_atomic.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

INT32 LOS_AtomicAdd(volatile INT32 *pswVal, INT32 swAddVal)
{
    INT32 swValue = 0;
    UINTPTR uvIntSave;
    UINTPTR uvTemp;

    __asm__ __volatile__(
    "MRS %0,cpsr\n"
    "ORR %1,%0,#0xc0\n"
    "MSR cpsr_c,%1"
    : "=r" (uvIntSave), "=r" (uvTemp)
    :
    : "memory");

    __asm__ __volatile__(
    "ldr   %0, [%1]\n"
    "add  %0, %0, %2\n"
    "str   %0, [%1]"
    : "=&r" (swValue)
    : "r" (pswVal), "r" (swAddVal)
    : "cc");

    __asm__ __volatile__(
    "MSR     cpsr_c, %0"
    :: "r" (uvIntSave) );

    return swValue;
} /*lint !e529*/

VOID LOS_AtomicInc(volatile INT32 *pswAddr)
{
    INT32 swValue;
    UINTPTR uvIntSave;
    UINTPTR uvTemp;

    __asm__ __volatile__(
    "MRS %0,cpsr\n"
    "ORR %1,%0,#0xc0\n"
    "MSR cpsr_c,%1"
    : "=r" (uvIntSave),  "=r" (uvTemp)
    :
    : "memory");

    __asm__ __volatile__(
    "ldr   %0, [%2]\n"
    "add  %0, %0, #1\n"
    "str   %0, [%2]"
    : "=&r" (swValue), "+m" (*pswAddr)
    : "r" (pswAddr)
    : "cc");

    __asm__ __volatile__(
    "MSR     cpsr_c, %0"
    :: "r" (uvIntSave) );
} /*lint !e529*/

VOID LOS_AtomicDec(volatile INT32 *pswAddr)
{
    INT32 swValue;
    UINTPTR uvIntSave;
    UINTPTR uvTemp;

    __asm__ __volatile__(
    "MRS %0,cpsr\n"
    "ORR %1,%0,#0xc0\n"
    "MSR cpsr_c,%1"
    : "=r" (uvIntSave),  "=r" (uvTemp)
    :
    : "memory");

    __asm__ __volatile__(
    "ldr   %0, [%2]\n"
    "sub  %0, %0, #1\n"
    "str   %0, [%2]"
    : "=&r" (swValue), "+m" (*pswAddr)
    : "r" (pswAddr)
    : "cc");

    __asm__ __volatile__(
    "MSR     cpsr_c, %0"
    :: "r" (uvIntSave) );
} /*lint !e529*/

INT32 LOS_AtomicIncRet(volatile INT32 *pswAddr)
{
    INT32 swValue = 0;
    UINTPTR uvIntSave;
    UINTPTR uvTemp;

    __asm__ __volatile__(
    "MRS %0,cpsr\n"
    "ORR %1,%0,#0xc0\n"
    "MSR cpsr_c,%1"
    : "=r" (uvIntSave),  "=r" (uvTemp)
    :
    : "memory");

    __asm__ __volatile__(
    "ldr   %0, [%2]\n"
    "add  %0, %0, #1\n"
    "str   %0, [%2]"
    : "=&r" (swValue), "+m" (*pswAddr)
    : "r" (pswAddr)
    : "cc");

    __asm__ __volatile__(
    "MSR     cpsr_c, %0"
    :: "r" (uvIntSave) );

    return swValue;
} /*lint !e529*/

INT32 LOS_AtomicDecRet(volatile INT32 *pswAddr)
{
    INT32 swValue = 0;
    UINTPTR uvIntSave;
    UINTPTR uvTemp;

    __asm__ __volatile__(
    "MRS %0,cpsr\n"
    "ORR %1,%0,#0xc0\n"
    "MSR cpsr_c,%1"
    : "=r" (uvIntSave),  "=r" (uvTemp)
    :
    : "memory");

    __asm__ __volatile__(
    "ldr   %0, [%2]\n"
    "sub  %0, %0, #1\n"
    "str   %0, [%2]"
    : "=&r" (swValue), "+m" (*pswAddr)
    : "r" (pswAddr)
    : "cc");

    __asm__ __volatile__(
    "MSR     cpsr_c, %0"
    :: "r" (uvIntSave) );

    return swValue;
} /*lint !e529*/

UINT32 LOS_AtomicXchgByte(volatile UINT8 *pucAddr, UINT32 uwValue)
{
    UINT32 uwPrevVal = 0;
    UINTPTR uvIntSave;
    UINTPTR uvTemp;

    __asm__ __volatile__(
    "MRS %0,cpsr\n"
    "ORR %1,%0,#0xc0\n"
    "MSR cpsr_c,%1"
    : "=r" (uvIntSave),  "=r" (uvTemp)
    :
    : "memory");

    __asm__ __volatile__(
    "ldrb   %0, [%2]\n"
    "strb   %3, [%2]"
    : "=&r" (uwPrevVal), "+m" (*pucAddr)
    : "r" (pucAddr), "r" (uwValue)
    : "cc");

    __asm__ __volatile__(
    "MSR     cpsr_c, %0"
    :: "r" (uvIntSave) );

     return uwPrevVal;
} /*lint !e529*/

UINT32 LOS_AtomicXchg16bits(volatile UINT16 *puwAddr, UINT32 uwValue)
{
    UINT32 uwPrevVal = 0;
    UINTPTR uvIntSave;
    UINTPTR uvTemp;

    __asm__ __volatile__(
    "MRS %0,cpsr\n"
    "ORR %1,%0,#0xc0\n"
    "MSR cpsr_c,%1"
    : "=r" (uvIntSave),  "=r" (uvTemp)
    :
    : "memory");

    __asm__ __volatile__(
    "ldrh   %0, [%2]\n"
    "strh   %3, [%2]"
    : "=&r" (uwPrevVal), "+m" (*puwAddr)
    : "r" (puwAddr), "r" (uwValue)
    : "cc");

    __asm__ __volatile__(
    "MSR     cpsr_c, %0"
    :: "r" (uvIntSave) );

     return uwPrevVal;
} /*lint !e529*/

UINT32 LOS_AtomicXchg32bits(volatile UINT32 *puwAddr, UINT32 uwValue)
{
    UINT32 uwPrevVal = 0;
    UINTPTR uvIntSave;
    UINTPTR uvTemp;

    __asm__ __volatile__(
    "MRS %0,cpsr\n"
    "ORR %1,%0,#0xc0\n"
    "MSR cpsr_c,%1"
    : "=r" (uvIntSave),  "=r" (uvTemp)
    :
    : "memory");

    __asm__ __volatile__(
    "ldr   %0, [%2]\n"
    "str   %3, [%2]"
    : "=&r" (uwPrevVal), "+m" (*puwAddr)
    : "r" (puwAddr), "r" (uwValue)
    : "cc");

     __asm__ __volatile__(
    "MSR     cpsr_c, %0"
    :: "r" (uvIntSave) );

     return uwPrevVal;
} /*lint !e529*/

BOOL LOS_AtomicCmpXchgByte(volatile UINT8 *puwAddr, UINT32 uwNewVal, UINT32 uwOldVal)
{
    UINT32 uwPrevVal = 0;
    UINTPTR uvIntSave;
    UINTPTR uvTemp;

    __asm__ __volatile__(
    "MRS %0,cpsr\n"
    "ORR %1,%0,#0xc0\n"
    "MSR cpsr_c,%1"
    : "=r" (uvIntSave),  "=r" (uvTemp)
    :
    : "memory");

    __asm__ __volatile__(
    "ldrb  %0, [%2]\n"
    "teq   %0, %3\n"
    "bne  1f\n"
    "strb  %4, [%2]\n"
    "1:"
    : "=&r" (uwPrevVal), "+m"(*puwAddr)
    : "r" (puwAddr), "r" (uwOldVal), "r" (uwNewVal)
    : "cc");

     __asm__ __volatile__(
    "MSR     cpsr_c, %0"
    :: "r" (uvIntSave) );

     return uwPrevVal != uwOldVal;
} /*lint !e529*/

BOOL LOS_AtomicCmpXchg16bits(volatile UINT16 *puwAddr, UINT32 uwNewVal, UINT32 uwOldVal)
{
    UINT32 uwPrevVal = 0;
    UINTPTR uvIntSave;
    UINTPTR uvTemp;

    __asm__ __volatile__(
    "MRS %0,cpsr\n"
    "ORR %1,%0,#0xc0\n"
    "MSR cpsr_c,%1"
    : "=r" (uvIntSave),  "=r" (uvTemp)
    :
    : "memory");

    __asm__ __volatile__(
    "ldrh  %0, [%2]\n"
    "teq   %0, %3\n"
    "bne  1f\n"
    "strh  %4, [%2]\n"
    "1:"
    : "=&r" (uwPrevVal), "+m"(*puwAddr)
    : "r" (puwAddr), "r" (uwOldVal), "r" (uwNewVal)
    : "cc");

     __asm__ __volatile__(
    "MSR     cpsr_c, %0"
    :: "r" (uvIntSave) );

     return uwPrevVal != uwOldVal;
} /*lint !e529*/

BOOL LOS_AtomicCmpXchg32bits(volatile UINT32 *puwAddr, UINT32 uwNewVal, UINT32 uwOldVal)
{
    UINT32 uwPrevVal = 0;
    UINTPTR uvIntSave;
    UINTPTR uvTemp;

    __asm__ __volatile__(
    "MRS %0,cpsr\n"
    "ORR %1,%0,#0xc0\n"
    "MSR cpsr_c,%1"
    : "=r" (uvIntSave),  "=r" (uvTemp)
    :
    : "memory");

    __asm__ __volatile__(
    "ldr   %0, [%2]\n"
    "teq  %0, %3\n"
    "streq  %4, [%2]"
    : "=&r" (uwPrevVal), "+m"(*puwAddr)
    : "r" (puwAddr), "r" (uwOldVal), "r" (uwNewVal)
    : "cc");

     __asm__ __volatile__(
    "MSR     cpsr_c, %0"
    :: "r" (uvIntSave) );

     return uwPrevVal != uwOldVal;
} /*lint !e529*/

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
