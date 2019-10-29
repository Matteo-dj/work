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

/**@defgroup sched Sched
 * @ingroup posix
 */

#ifndef _HWLITEOS_POSIX_SCHED_H
#define _HWLITEOS_POSIX_SCHED_H

#include "sys/types.h"
#include "time.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */


/* Scheduling Policys */

#define SCHED_OTHER 0
#define SCHED_FIFO  1
#define SCHED_RR    2

/* Process scheduling functions. */

/* Force current thread to relinquish the processor. */

/**
 * @ingroup  sched
 * @brief Yield the processor.
 *
 * @par Description:
 * The sched_yield() function shall force the running thread to relinquish the processor until it again becomes the head of its thread list. It takes no arguments.
 *
 * @attention None.
 *
 * @retval 0    The sched_yield() function shall return 0 if it completes successfully.
 * @retval -1  Otherwise, it shall return a value of -1.
 * @par Dependency:
 * <ul><li>sched.h: the header file that contains the API declaration.</li></ul>
 * @see
 * @since Huawei LiteOS V100R001C00
 */
extern int sched_yield(void);

/**
 * @ingroup  sched
 * @brief Get priority limits.
 *
 * @par Description:
 * The sched_get_priority_max() functions shall return the appropriate maximum for the scheduling policy specified by policy.
 *
 * @attention None.
 *
 * @param  policy   [IN] The value of policy shall be one of the scheduling policy values defined in <sched.h>.
 *
 * @retval int maximum If successful, the sched_get_priority_max() function shall return the appropriate maximum value.
 * @retval -1  If unsuccessful, the sched_get_priority_max() function shall return a value of -1.
 * @par Dependency:
 * <ul><li>sched.h: the header file that contains the API declaration.</li></ul>
 * @see
 * @since Huawei LiteOS V100R001C00
 */
extern int sched_get_priority_max(int policy);

/**
 * @ingroup  sched
 * @brief Get priority limits.
 *
 * @par Description:
 * The sched_get_priority_min() functions shall return the appropriate minimum for the scheduling policy specified by policy.
 *
 * @attention None.
 *
 * @param  policy   [IN] The value of policy shall be one of the scheduling policy values defined in <sched.h>.
 *
 * @retval int minimum If successful, the sched_get_priority_min() function shall return the appropriate minimum value.
 * @retval -1  If unsuccessful, the sched_get_priority_min() function shall return a value of -1.
 * @par Dependency:
 * <ul><li>sched.h: the header file that contains the API declaration.</li></ul>
 * @see
 * @since Huawei LiteOS V100R001C00
 */
extern int sched_get_priority_min(int policy);


#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif
