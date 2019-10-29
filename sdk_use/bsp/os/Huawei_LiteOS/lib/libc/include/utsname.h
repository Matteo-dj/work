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
/** @defgroup utsname UtsName
 *  @ingroup posix
 */

#ifndef _HWLITEOS_POSIX_UTSNAME_H
#define _HWLITEOS_POSIX_UTSNAME_H

#include <stddef.h>
#include <limits.h>
#include <sys/types.h>
#include <sched.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */


#define POSIX_UTSNAME_LENGTH    64
#define POSIX_UTSNAME_NODENAME_LENGTH  32

struct utsname
{
    char sysname[POSIX_UTSNAME_LENGTH];
    char nodename[POSIX_UTSNAME_NODENAME_LENGTH];
    char release[POSIX_UTSNAME_LENGTH];
    char version[POSIX_UTSNAME_LENGTH];
    char machine[POSIX_UTSNAME_LENGTH];
};

/* Get name and information about current kernel. */
/**
 *@ingroup utsname
 *@brief Get name and information about current kernel.
 *
 *@par Description:
 *This API is used to get name and information about current kernel.
 *@attention
 *<ul>
 *<li>The information of current machine is empty. It will be filled later on. </li>
 *</ul>
 *
 *@param name   [OUT] Pointer to an attributes object of current kernel information.
 *
 *@retval   0  Get name and information about current kernel success.
 *@retval  -1  Invalid information.
 *@par Dependency:
 *<ul>
 *<li>utsname.h: the header file that contains the API declaration.</li>
 *</ul>
 *@since Huawei LiteOS V100R001C00
 */
int uname( struct utsname *name );


#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif
