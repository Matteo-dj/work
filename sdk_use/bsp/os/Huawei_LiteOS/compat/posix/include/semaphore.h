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

/**@defgroup semaphore Semaphore
 * @ingroup posix
 */

#ifndef _HWLITEOS_POSIX_SEMAPHORE_H
#define _HWLITEOS_POSIX_SEMAPHORE_H

#include "time.h"

#include "los_sem.ph"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */


#ifdef SEM_VALUE_MAX
#undef SEM_VALUE_MAX
#endif

/**
 * @ingroup semaphore
 * Maximum semaphore value.
 */
#define  SEM_VALUE_MAX      OS_SEM_COUNT_MAX

#ifdef SEM_NSEMS_MAX
#undef SEM_NSEMS_MAX
#endif

/**
 * @ingroup semaphore
 * Maximum number of supported semaphores.
 */
#define  SEM_NSEMS_MAX      LOSCFG_BASE_IPC_SEM_LIMIT

/**
 * @ingroup semaphore
 * API parameter sructure
 */

typedef struct posix_sem
{
    SEM_CB_S* sem;        /**< Semaphore attribute structure*/
}sem_t;

/**
 * @ingroup semaphore
 * Value returned when an named semaphore fails to be initialized and opened or created.
 */
#define SEM_FAILED      ((sem_t *)NULL)

/**
 *@ingroup semaphore
 *@brief Initialize an unnamed semaphore.
 *
 *@par Description:
 *This API is used to initialize a semaphore structure and set the semaphore value.
 *@attention
 *<ul>
 *<li>Initializing a semaphore that has already been initialized will result in undefined behavior.</li>
 *<li>If the semaphore value is 0 when the semaphore is being initialized, and some tasks are requesting this semaphore, the tasks will be blocked.</li>
 *</ul>
 *
 *@param sem        [IN] Semaphore handle.
 *@param pshared    [IN] This parameter is not supported. Users can pass in any integer.
 *@param value      [IN] Semaphore value. The range is [0,65534].
 *
 *@retval   0 The semaphore is successfully initialized.
 *@retval   -1 The semaphore fails to be initialized, and errno is set to indicate the error. The error causes might lie in that the value of the value parameter exceeds the maximum semaphore value, or that no semaphore is available, or other reasons,with any of the following error codes in errno:
               #EINVAL     invalid parameter.
               #ENOSPC     insufficient memory.
 *@par Dependency:
 *<ul>
 *<li>semaphore.h: the header file that contains the API declaration.</li>
 *</ul>
 *@see sem_destroy
 *@since Huawei LiteOS V100R001C00
 */
extern int sem_init(sem_t *sem, int pshared, unsigned int value);

/**
 *@ingroup semaphore
 *@brief Destroy an unnamed semaphore.
 *
 *@par Description:
 *<li>This API is used to destroy a specified semaphore.
 *@attention
 *<ul>
 *<li>Destroying a semaphore that other tasks are currently blocked on will return -1.</li>
 *</ul>
 *
 *@param sem     [IN] Semaphore handle.
 *
 *@retval 0 The semaphore is successfully destroyed.
 *@retval -1 The semaphore fails to be destroyed, and errno is set to indicate the error. The error causes might lie in that sem is an invalid semaphore, or that other tasks are currently blocked on the semaphore, or other reasons.
              #EINVAL    Invalid parameter.
              #EBUSY     The sem is being in use.
 *@par Dependency:
 *<ul><li>semaphore.h: the header file that contains the API declaration.</li></ul>
 *@see sem_init
 *@since Huawei LiteOS V100R001C00
 */
extern int sem_destroy(sem_t *sem);

/**
 *@ingroup semaphore
 *@brief Request an unnamed semaphore.
 *
 *@par Description:
 *This API is used to request an unnamed semaphore. If the semaphore value is not 0, it will be decremented by 1 and the API successfully returns. If the semaphore value is 0, the calling task will be blocked and wait for other tasks to release the semaphore.
 *@attention
 *<ul>
 *<li>None.</li>
 *</ul>
 *
 *@param sem     [IN] Semaphore handle.
 *
 *@retval 0 The requesting task successfully obtains the semaphore.
 *@retval -1 The API fails and errno is set to indicate the error. The error might lie in that sem is an invalid semaphore or other reasons,with any of the following error codes in errno:
              #EINVAL       invalid parameter.
              #EPERM        The task is unable to request a semaphore because task scheduling is locked.
              #EINTR        The API is called during an interrupt, which is forbidden.
              #ETIMEDOUT    The request for a semaphore times out.
 *@par Dependency:
 *<ul><li>semaphore.h: the header file that contains the API declaration.</li></ul>
 *@see sem_post
 *@since Huawei LiteOS V100R001C00
 */
extern int sem_wait(sem_t *sem);

/**
 *@ingroup semaphore
 *@brief Time-out request for an unnamed semaphore.
 *
 *@par Description:
 *This API is used to request an existing unnamed semaphore. If the task requesting a semaphore is blocked, the time for the task to wait for the semaphore to become available can be set by setting the timeout parameter.
 *@attention
 *<ul>
*<li> The waiting time is a relative time.</li>
 *<li> The timeout interval cannot be set to a past time period.</li>
 *<li>If the semaphore value is not 0, it will be decremented by 1 and the API successfully returns. If the semaphore value is 0, the calling task will be blocked and wait for the semaphore to be released. If the waiting time exceeds the set timeout interval, the API will fail.</li>
 *</ul>
 *
 *@param sem        [IN] Semaphore handle.
 *@param timeout    [IN] Timeout interval structure.
 *
 *@retval 0 The requesting task successfully obtains the semaphore.
 *@retval -1 The API fails and errno is set to indicate the error. The error causes might lie in that sem is an invalid semaphore, or that the value range of the timeout parameter is wrong, or other reasons,with any of the following error codes in errno:
              #EINVAL       invalid parameter.
              #EAGAIN       The task is unable to request a semaphore because task scheduling is locked.
              #EINTR        The API is called during an interrupt, which is forbidden.
              #ETIMEDOUT    The request for a semaphore times out.
 *@par Dependency:
 *<ul><li>semaphore.h: the header file that contains the API declaration.</li></ul>
 *@see sem_post
 *@since Huawei LiteOS V100R001C00
 */
extern int sem_timedwait(sem_t *sem, const struct timespec *timeout);

/**
 *@ingroup semaphore
 *@brief Conditionally request an unnamed semaphore.
 *
 *@par Description:
 *This API is used to request an existing unnamed semaphore. The request is accepted only if the value of the called semaphore is greater than 0, or no tasks are blocked on this semaphore.
 *@attention
 *<ul>
 *<li>None.</li>
 *</ul>
 *
 *@param sem    [IN] Semaphore handle.
 *
 *@retval 0 The requesting task successfully obtains the semaphore.
 *@retval -1 The request is not accepted and errno is set to indicate the error. The error causes might lie in that sem is an invalid semaphore, or that some tasks are blocked on the semaphore, or other reasons,with any of the following error codes in errno:
              #EINVAL       invalid parameter.
              #EAGAIN       The task is unable to request a semaphore because task scheduling is locked.
              #EINTR        The API is called during an interrupt, which is forbidden.
              #ETIMEDOUT    The request for a semaphore times out.
 *@par Dependency:
 *<ul><li>semaphore.h: the header file that contains the API declaration.</li></ul>
 *@see sem_post
 *@since Huawei LiteOS V100R001C00
 */
extern int sem_trywait(sem_t *sem);

/**
 *@ingroup semaphore
 *@brief Release a specified unnamed semaphore.
 *
 *@par Description:
 *This API is used to request an existing semaphore. If no tasks are blocked on the semaphore, the semaphore value is incremented by 1 and the API successfully returns. If some tasks are blocked on the semaphore, the first blocked task is released.
 *@attention
 *<ul>
 *<li>None.</li>
 *</ul>
 *
 *@param sem     [IN] Semaphore handle.
 *
 *@retval 0 The semaphore is successfully released.
 *@retval -1 The semaphore fails to be released and errno is set to indicate the error. The error causes might lie in that sem is an invalid semaphore, or other reasons,with any of the following error codes in errno:
              #EINVAL    invalid parameter.
              #EAGAIN    The task is unable to request a semaphore because task scheduling is locked.
 *@par Dependency:
 *<ul><li>semaphore.h: the header file in which the interface statement is specified.</li></ul>
 *@see sem_wait
 *@since Huawei LiteOS V100R001C00
 */
extern int sem_post(sem_t *sem);

/**
 *@ingroup semaphore
 *@brief Obtain a semaphore value.
 *
 *@par Description:
 *This API is used to obtain the value of a specified semaphore and place it into the in the memory pointed to by the sval parameter.
 *@attention
 *<ul>
 *<li>None.</li>
 *</ul>
 *
 *@param sem    [IN] Semaphore handle.
 *@param sval   [OUT] Integer pointer to the memory that stores the obtained semaphore values.
 *
 *@retval 0 The semaphore value is successfully obtained.
 *@retval -1 The semaphore value fails to be obtained and errno is set to indicate the error. The error causes might lie in that sem is an invalid semaphore, or other reasons,with any of the following error codes in errno:
              #EINVAL     invalid parameter.
              #ENOMEM     The times of semaphore release exceed the maximum times permitted.
 *@par Dependency:
 *<ul><li>semaphore.h: the header file that contains the API declaration.</li></ul>
 *@see None.
 *@since Huawei LiteOS V100R001C00
 */
extern int sem_getvalue(sem_t *sem, int *sval);

/**
 *@ingroup semaphore
 *@brief Initialize and open or create a named semaphore.
 *
 *@par Description:
 *This API is used to initialize and open or create a named semaphore.
 *@attention
 *<ul>
 *<li>This API has not been achieved in Huawei LiteOS.</li>
 *</ul>
 *
 *@param name    [IN] Semaphore name.
 *@param oflag   [IN] Flag of the semaphore to be created or opened. The value range is [O_CREAT, O_EXCL].
 *@param mode        [IN] Semaphore mode (variadic argument). When oflag is O_CREAT, it a third and a fourth argument: mode, which is of type mode_t, and value, which is of type unsigned. The semaphore is created with an initial value of value.
 *@param value        [IN] Semaphore value (variadic argument). Valid initial values for semaphores are less than or equal to SEM_VALUE_MAX.
 *
 *@retval #SEM_FAILED          (sem_t *)NULL The semaphore fails to be initialized and opened or created with a null pointer to the semaphore structure returned. Errno is set to indicate the error.
 *@retval sem_t* Semaphore address.  The API successfully returns.
 *@par Dependency:
 *<ul><li>semaphore.h: the header file that contains the API declaration.</li></ul>
 *@see sem_close.
 *@since Huawei LiteOS V100R001C00
 */
extern sem_t *sem_open(const char *name, int oflag, ...);

/**
 *@ingroup semaphore
 *@brief Close a specified named semaphore.
 *
 *@par Description:
 *This API is used to close a specified named semaphore.
 *@attention
 *<ul>
 *<li>This API has not been achieved in Huawei LiteOS.</li>
 *</ul>
 *
 *@param sem       [IN] Semaphore handle.
 *
 *@retval ENOSYS Not achieved.
 *@par Dependency:
 *<ul><li>semaphore.h: the header file that contains the API declaration.</li></ul>
 *@see sem_open.
 *@since Huawei LiteOS V100R001C00
 */
extern int sem_close(sem_t *sem);

/**
 *@ingroup semaphore
 *@brief Unlink a specified named semaphore.
 *
 *@par Description:
 *This API is used to unlink a specified named semaphore.
 *@attention
 *<ul>
 *<li>This API has not been achieved in Huawei LiteOS.</li>
 *</ul>
 *
 *@param name      [IN] Semaphore name.
 *
 *@retval ENOSYS Not achieved.
 *@par Dependency:
 *<ul><li>semaphore.h: the header file that contains the API declaration.</li></ul>
 *@see None.
 *@since Huawei LiteOS V100R001C00
 */
extern int sem_unlink(const char *name);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif

