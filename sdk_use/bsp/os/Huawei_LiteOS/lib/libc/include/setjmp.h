/*	$OpenBSD: setjmp.h,v 1.5 2005/12/13 00:35:22 millert Exp $	*/
/*	$NetBSD: setjmp.h,v 1.11 1994/12/20 10:35:44 cgd Exp $	*/

/*-
 *  Copyright (c) 1990, 1993
 *	The Regents of the University of California.  All rights reserved.
 *  Copyright (c) <2014-2015>, <Huawei Technologies Co., Ltd>
 *  All rights reserved.
 * (c) UNIX System Laboratories, Inc.
 * All or some portions of this file are derived from material licensed
 * to the University of California by American Telephone and Telegraph
 * Co. or Unix System Laboratories, Inc. and are reproduced herein with
 * the permission of UNIX System Laboratories, Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)setjmp.h	8.2 (Berkeley) 1/21/94
 */
 /*----------------------------------------------------------------------------
 * Notice of Export Control Law
 * ===============================================
 * Huawei LiteOS may be subject to applicable export control laws and regulations, which might
 * include those applicable to Huawei LiteOS of U.S. and the country in which you are located.
 * Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance with such
 * applicable export control laws and regulations.
 *---------------------------------------------------------------------------*/

#ifndef __SETJMP_H__
#define __SETJMP_H__

#include "stdio.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */


#define JMP_BUF_SIZE 16  // Actually 11, but some room left over
typedef UINT32 jmp_buf[JMP_BUF_SIZE];

extern int los_setjmp(jmp_buf env);
extern int los_longjmp(jmp_buf env, int t);

/**
* @ingroup setjmp
* @brief save calling environment for long jump.
*
* @par Description:
* This API is used to fill env with information about the current state of the calling environment
* in that point of code execution, so that it can be restored by a later call to longjmp.
*
* @attention None
*
* @param env [IN] Type #jmp_buf Object of type jmp_buf where the environment information is stored.
*
* @retval  int : return more than once: A first time, on its direct invocation; In this case it always returns zero.When longjmp is called with the information set to env,
*   the macro returns again; this time it returns the value passed to longjmp as second argument if this is different from zero, or 1 if it is zero.
* @par Dependency:
* <ul><li>setjmp.h: the header file that contains the API declaration.</li></ul>
*@see longjmp
* @since Huawei LiteOS V100R001C00
*/
extern int setjmp(jmp_buf env);

/**
* @ingroup setjmp
* @brief long jump.
*
* @par Description:
* This API is used to restores the environment to the state indicated by env, evaluating the setjmp expression that filled env as t.
* @attention None
*
* @param env [IN] Type #jmp_buf Object of type jmp_buf filled by a previous call to setjmp that contains information to restore the environment to that point.
* @param t     [IN] Type #int         Value to which the setjmp expression evaluates.If this is zero, the expression evaluates as 1.
* @retval  int  : 1, if t is 0
* @retval  int  : t
* @par Dependency:
* <ul><li>setjmp.h: the header file that contains the API declaration.</li></ul>
*@see setjmp
* @since Huawei LiteOS V100R001C00
*/
extern int longjmp(jmp_buf env, int t);


#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif
