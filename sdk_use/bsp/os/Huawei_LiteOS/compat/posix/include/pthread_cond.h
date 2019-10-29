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

/**@defgroup pthread_cond Condition variable
 * @ingroup posix
 */

#ifndef _HWLITEOS_POSIX_PTHREAD_COND_H
#define _HWLITEOS_POSIX_PTHREAD_COND_H

#include "sys/types.h"
#include "codes.h"
#include "los_event.h"
#include "mutex.h"
#include "time.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */


/**
 * @ingroup pthread_cond
 * Statically initialize a condition variable.
 */
#define PTHREAD_COND_INITIALIZER    {-1, {0, {NULL, NULL}} , NULL}

/**
 * @ingroup pthread_cond
 * Define a condition variable to be shared between threads within the same process.
 */
#define PTHREAD_PROCESS_PRIVATE 0

/**
 * @ingroup pthread_cond
 * Define a condition variable to be shared among multiple processes.
 */
#define PTHREAD_PROCESS_SHARED  1

/**
 * @ingroup pthread_cond
 * Condition variable attribute.
 */
typedef int pthread_condattr_t;

/**
 * @ingroup pthread_cond
 * Condition variable structure.
 */
typedef struct pthread_cond
{
    int  volatile value; /**< Condition variable state value*/
    EVENT_CB_S event; /**< Event object*/
    pthread_mutex_t *mutex; /**< Mutex locker for condition variable protection */
} pthread_cond_t;

/**
 *@ingroup pthread_cond
 *@brief Initialize a condition variable.
 *
 *@par Description:
 *<ul>
 *<li> This API is used to initialize a condition variable.</li>
 *</ul>
 *@attention
 *<ul>
 *<li>A condition variable cannot be initialized by multiple threads. When a condition variable needs to be reinitialized, it must not have been used.</li>
 *</ul>
 *
 *@param cond       [OUT] Condition variable object.
 *@param attr         [IN]    Condition variable attribute. The passed value and the default value of this parameter can be only set to PTHREAD_PROCESS_PRIVATE.
 *
 *@retval   #EINVAL      One or more parameters are invalid.
 *@retval   #ENOERR      The condition variable is successfully initialized.
 *@retval   #ENOMEM      Failed to allocate in-memory resources for the operation.
 *@par Dependency:
 *<ul><li>pthread_cond.h: the header file that contains the API declaration.</li></ul>
 *@see pthread_cond_destroy
 *@since Huawei LiteOS V100R001C00
 */
extern int pthread_cond_init(pthread_cond_t *cond, const pthread_condattr_t *attr);

/**
 *@ingroup pthread_cond
 *@brief Destroy a condition variable.
 *
 *@par Description:
 *<ul>
 *<li> This API is used to destroy a condition variable.</li>
 *</ul>
 *@attention
 *<ul>
 *<li>The condition variable is using memory and not destroyed.</li>
 *</ul>
 *
 *@param cond       [IN]    Condition variable object.
 *
 *@retval   #ENVAIL  The parameter is invalid.
 *@retval   #EBUSY   The condition variable is being in use.
 *@retval   #ENOERR  The condition variable is successfully destroyed.
 *@par Dependency:
 *<ul><li>pthread_cond.h: the header file that contains the API declaration.</li></ul>
 *@see pthread_cond_init
 *@since Huawei LiteOS V100R001C00
 */
extern int pthread_cond_destroy(pthread_cond_t * cond);

/**
 *@ingroup pthread_cond
 *@brief Unblock a thread blocked on a condition variable.
 *
 *@par Description:
 *<ul>
 *<li>This API is used to unblock a thread blocked on a condition variable and wakes this thread.</li>
 *</ul>
 *@attention
 *<ul>
 *<li>DO NOT call this API in software timer callback. </li>
 *</ul>
 *
 *@param cond       [IN]    Condition variable object.
 *
 *@retval   #ENVAIL   The parameter is invalid.
 *@retval   #ENOERR   The thread is successfully unlocked.
 *@par Dependency:
 *<ul><li>pthread_cond.h: the header file that contains the API declaration.</li></ul>
 *@see pthread_cond_wait
 *@since Huawei LiteOS V100R001C00
 */
extern int pthread_cond_signal(pthread_cond_t *cond);

/**
 *@ingroup pthread_cond
 *@brief Unblock all threads blocked on a condition variable.
 *
 *@par Description:
 *<ul>
 *<li> This API is used to unblock all threads blocked on a condition variable and wake all these threads.</li>
 *</ul>
 *@attention
 *<ul>
 *<li>DO NOT call this API in software timer callback. </li>
 *</ul>
 *
 *@param cond       [IN]    Condition variable object.
 *
 *@retval   #ENVAIL  The parameter is invalid.
 *@retval   #ENOERR  All threads blocked on this condition variable are successfully unblocked.
 *@par Dependency:
 *<ul><li>pthread_cond.h: the header file that contains the API declaration.</li></ul>
 *@see pthread_cond_wait
 *@since Huawei LiteOS V100R001C00
 */
extern int pthread_cond_broadcast(pthread_cond_t *cond);

/**
 *@ingroup pthread_cond
 *@brief Blocking of a condition variable.
 *
 *@par Description:
 *<ul>
 *<li> A thread has been being blocked on a condition variable and waits to be awoken by the condition variable.</li>
 *</ul>
 *@attention
 *<ul>
 *<li>DO NOT call this API in software timer callback. </li>
 *</ul>
 *
 *@param cond       [IN]    Condition variable object.
 *@param mutex     [IN]    Mutex object.
 *
 *@retval   #ENVAIL  One or more parameters are invalid.
 *@retval   #ENOERR  The thread is successfully awoken.
 *@par Dependency:
 *<ul><li>pthread_cond.h: the header file that contains the API declaration.</li></ul>
 *@see pthread_cond_signal pthread_cond_broadcast
 *@since Huawei LiteOS V100R001C00
 */
extern int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);

/**
 *@ingroup pthread_cond
 *@brief Time-out blocking of a condition variable.
 *
 *@par Description:
 *<ul>
 *<li> A thread has been being blocked on a condition variable and is awoken until the set relative time has passed or the thread obtains a condition variable.</li>
 *</ul>
 *@attention
 *<ul>
 *<li>The waiting time is a relative time.</li>
 *<li> Setting the timeout interval to a past time period is not supported.</li>
 *<li>DO NOT call this API in software timer callback. </li>
 *</ul>
 *
 *@param cond       [IN]    Condition variable object.
 *@param mutex     [IN]     Mutex object.
 *@param abstime    [IN]    Time object.
 *
 *@retval   #ENOERR     The thread receives a signal and is successfully awoken.
 *@retval   #ENVAIL     One or more parameters are invalid.
 *@retval   #ETIMEDOUT  The waiting time has passed.
 *@retval   #ENOMEM     Failed to allocate in-memory resources for the operation.
 *@par Dependency:
 *<ul><li>pthread_cond.h: the header file that contains the API declaration.</li></ul>
 *@see pthread_cond_wait
 *@since Huawei LiteOS V100R001C00
 */
extern int pthread_cond_timedwait(pthread_cond_t *cond, pthread_mutex_t *mutex, struct timespec *abstime);

/**
 *@ingroup pthread_cond
 *@brief Obtain the condition variable attribute.
 *
 *@par Description:
 *<ul>
 *<li> This API is used to obtain the condition variable attribute.</li>
 *</ul>
 *@attention
 *<ul>
 *<li>The condition variable attribute can be only set to PTHREAD_PROCESS_PRIVATE.</li>
 *<li>This API does not task effect on Huawei LiteOS Kernel.</li>
 *</ul>
 *
 *@param attr       [IN]    Condition variable attribute.
 *@param pshared   [OUT]    Obtained condition variable attribute, which is always PTHREAD_PROCESS_PRIVATE.
 *
 *@retval   #ENVAIL  One or more parameters are invalid.
 *@retval   #ENOERR  The condition variable attribute is successfully obtained.
 *@par Dependency:
 *<ul><li>pthread_cond.h: the header file that contains the API declaration.</li></ul>
 *@see pthread_condattr_setpshared
 *@since Huawei LiteOS V100R001C00
 */
extern int pthread_condattr_getpshared(const pthread_condattr_t *attr, int *pshared);

/**
 *@ingroup pthread_cond
 *@brief Set the condition variable attribute.
 *
 *@par Description:
 *<ul>
 *<li> This API is used to set the condition variable attribute.</li>
 *</ul>
 *@attention
 *<ul>
 *<li>The condition variable attribute can be only set to PTHREAD_PROCESS_PRIVATE.</li>
 *<li>This API does not task effect on Huawei LiteOS Kernel.</li>
 *</ul>
 *
 *@param attr       [IN]    Condition variable attribute.
 *@param pshared  [OUT]    Condition variable attribute, which is always PTHREAD_PROCESS_PRIVATE.
 *
 *@retval   #ENVAIL  One or more parameters are invalid.
 *@retval   #ENOERR  The condition variable attribute is successfully set.
 *@par Dependency:
 *<ul><li>pthread_cond.h: the header file that contains the API declaration.</li></ul>
 *@see pthread_condattr_getpshared
 *@since Huawei LiteOS V100R001C00
 */
extern int pthread_condattr_setpshared(pthread_condattr_t *attr, int pshared);

/**
 *@ingroup pthread_cond
 *@brief Destroy the condition variable attribute.
 *
 *@par Description:
 *<ul>
 *<li> This API is used to destroy the condition variable attribute.</li>
 *</ul>
 *@attention
 *<ul>
 *<li>The condition variable attribute can be only set to PTHREAD_PROCESS_PRIVATE.</li>
 *<li>This API does not task effect on Huawei LiteOS Kernel.</li>
 *</ul>
 *
 *@param attr   [IN]    Condition variable attribute.
 *
 *@retval   #ENVAIL     The parameter is invalid.
 *@retval   #ENOERR     The condition variable attribute is successfully destroied.
 *@par Dependency:
 *<ul><li>pthread_cond.h: the header file that contains the API declaration.</li></ul>
 *@see pthread_condattr_init
 *@since Huawei LiteOS V100R001C00
 */
extern int pthread_condattr_destroy(pthread_condattr_t *attr);

/**
 *@ingroup pthread_cond
 *@brief Initialize the condition variable attribute.
 *
 *@par Description:
 *<ul>
 *<li> This API is used to initialize the condition variable attribute.</li>
 *<li>This API does not task effect on Huawei LiteOS Kernel.</li>
 *</ul>
 *@attention
 *<ul>
 *<li>The condition variable attribute can be only set to PTHREAD_PROCESS_PRIVATE.</li>
 *</ul>
 *
 *@param attr       [OUT]    Condition variable attribute.
 *
 *@retval   #ENVAIL  The parameter is invalid.
 *@retval   #ENOERR  The condition variable attribute is successfully initialized.
 *@par Dependency:
 *<ul><li>pthread_cond.h: the header file that contains the API declaration.</li></ul>
 *@see pthread_condattr_setpshared pthread_condattr_getpshared
 *@since Huawei LiteOS V100R001C00
 */
extern int pthread_condattr_init(pthread_condattr_t *attr);


#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif
