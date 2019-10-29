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

/**@defgroup pthread Thread
 * @ingroup posix
 */

/**
 *@defgroup posix POSIX
 */

#ifndef _HWLITEOS_POSIX_PTHREAD_H
#define _HWLITEOS_POSIX_PTHREAD_H

#include "sys/types.h"
#include "pthread_cond.h"
#include "sched.h"
#include "mutex.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */


/* General thread operations */

/* Thread creation and management. */

/* Create a thread. */
/**
 *@ingroup pthread
 *@brief Create a thread.
 *
 *@par Description:
 *This API is used to create a thread according to input parameters.
 *@attention
 *<ul>
 *<li>The parameter value and return value of the thread entrance function must be void.*</li>
 *<li>A smaller priority value indicates a higher priority.</li>
 *</ul>
 *
 *@param pthread          [OUT] ID of the created thread.
 *@param attr             [IN]  Pointer to a thread attributes object. If the parameter value is NULL, the default thread attributes are applied.
 *@param start_routine    [IN]  Function entry of the thread.
 *@param arg              [IN]  Parameter of the thread entrance function.
 *
 *@retval   #EINVAL    The thread fails to be created due to incorrect parameters.
 *@retval   #ENOSPC    The thread fails to be created because No free task control block is available ,or the SEM control block is not enough.
 *@retval   #ENOMEM    The thread fails to be created because the maximum number of threads is exceeded, or memory allocation fails, or an interrupt in underway.
 *@retval   #ENOERR    The thread is successfully created.
 *@par Dependency:
 *<ul>
 *<li>pthread.h: the header file that contains the API declaration.</li>
 *</ul>
 *@see pthread_exit
 *@since Huawei LiteOS V100R001C00
 */
extern int pthread_create(pthread_t *pthread,
        const pthread_attr_t *attr,
        void *(*start_routine)(void *),
        void *arg);

/* Terminate current thread. */
/**
 *@ingroup pthread
 *@brief Terminate the current thread.
 *
 *@par Description:
 *This API is used to terminate the current thread, and the return value contains only one parameter.
 *@attention
 *<ul>
 *<li></li>
 *</ul>
 *
 *@param   retval  [OUT] Pointer to the thread return value.
 *
 *@retval   None.
 *@par Dependency:
 *<ul>
 *<li>pthread.h: the header file that contains the API declaration.</li>
 *</ul>
 *@see pthread_creat
 *@since Huawei LiteOS V100R001C00
 */
extern void pthread_exit(void *retval);

/* Wait for the thread to terminate. If thread_return is not NULL then
   the retval from the thread's call to pthread_exit() is stored at thread_return.
   */
/**
 *@ingroup pthread
 *@brief Wait for a thread to terminate.
 *
 *@par Description:
 *This API is used to wait for a thread to terminate and reclaim its resources.
 *@attention
 *<ul>
 *<li>A thread cannot be waited for by multiple threads. If a thread is waited for by multiple threads, ESRCH will be returned.</li>
 *</ul>
 *
 *@param   pthread          [IN]  ID of the target thread (the waited thread).
 *@param   thread_return    [OUT] Return value sent to the waiting thread.
 *
 *@retval   #ESRCH      The target thread is not joinable (the thread does not exist, or has exited, or is waited for by another thread).
 *@retval   #EDEADLK    A deadlock results because a thread is waiting for itself to terminate.
 *@retval   #EINVAL     The target thread is not joinable.
 *@retval   #ENOERR     The target thread is successfully joined with.
 *@par Dependency:
 *<ul>
 *<li>pthread.h: the header file that contains the API declaration.</li>
 *</ul>
 *@see pthread_detach
 *@since Huawei LiteOS V100R001C00
 */
extern int pthread_join(pthread_t pthread, void **thread_return);

/* Set the detachstate of the thread to "detached". The thread then does not
   need to be joined and its resources will be freed when it exits.
   */
/**
 *@ingroup pthread
 *@brief Mark a thread as detached.
 *
 *@par Description:
 *This API is used to mark a thread as detached. When a detached thread terminates, its resources are automatically released back to Huawei LiteOS without the need for another thread to join with the terminated thread.
 *@attention
 *<ul>
 *<li>None.</li>
 *</ul>
 *
 *@param    pthread   [IN]  ID of the thread to be marked as detached.
 *
 *@retval   #ESRCH      The target thread cannot be marked (the thread does not exist or has exited).
 *@retval   #EINVAL     The target thread is already marked as detached.
 *@retval   #ENOERR     The thread is successfully marked.
 *@par Dependency:
 *<ul>
 *<li>pthread.h: the header file that contains the API declaration.</li>
 *</ul>
 *@see pthread_join
 *@since Huawei LiteOS V100R001C00
 */
extern int pthread_detach(pthread_t pthread);

/* Get current thread id. */
/**
 *@ingroup pthread
 *@brief Acquire the ID of a calling thread.
 *
 *@par Description:
 *This API is used to acquire the thread identifier of the calling thread.
 *@attention
 *<ul>
 *<li>None.</li>
 *</ul>
 *
 *@param  NULL.
 *
 *@retval pthread_t Thread ID [0,LOSCFG_BASE_CORE_TSK_LIMIT].
 *@par Dependency:
 *<ul>
 *<li>pthread.h: the header file that contains the API declaration.</li>
 *</ul>
 *@see None.
 *@since Huawei LiteOS V100R001C00
 */
extern pthread_t pthread_self(void);

/* Compare two thread identifiers. */
/**
 *@ingroup pthread
 *@brief Compare two thread IDs.
 *
 *@par Description:
 *This API is used to compare whether two thread IDs belong to the same thread.
 *@attention
 *<ul>
 *<li>None.</li>
 *</ul>
 *
 *@param thread1  [IN]  ID of the thread 1 being compared.
 *@param thread2  [IN]  ID of the thread 2 being compared.
 *
 *@retval   int Any value other than 0  mean that two threads are equal.
 *@retval   0  The two threads are unequal.
 *@par Dependency:
 *<ul>
 *<li>pthread.h: the header file that contains the API declaration.</li>
 *</ul>
 *@see None.
 *@since Huawei LiteOS V100R001C00
 */
extern int pthread_equal(pthread_t thread1, pthread_t thread2);

/* Thread attribute handling. */

/* Initialize attributes object with default attributes:
   detachstate          == PTHREAD_JOINABLE
   scope                == PTHREAD_SCOPE_SYSTEM
   inheritsched         == PTHREAD_EXPLICIT_SCHED
   schedpolicy          == SCHED_OTHER
   schedparam           == unset
   stackaddr            == unset
   stacksize            == 0
   */
/**
 *@ingroup pthread
 *@brief Initialize a thread attributes object.
 *
 *@par Description:
 *This API is used to initialize a thread attributes object into default attributes.
 *@attention
 *<ul>
 *<li>Default thread attributes</li>
 *<li>detachstate          = PTHREAD_JOINABLE</li>
 *<li>scope                = PTHREAD_SCOPE_SYSTEM</li>
 *<li>inheritsched         = PTHREAD_EXPLICIT_SCHED</li>
 *<li>schedpolicy          = SCHED_OTHER</li>
 *<li>schedparam           = unset</li>
 *<li>stackaddr            = unset</li>
 *<li>stacksize            = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE</li>
 *</ul>
 *
 *@param   attr    [IN] Pointer to the thread attributes object to be initialized.
 *
 *@retval  #EINVAL    The input parameter is NULL.
 *@retval  #ENOERR    The thread attributes object is successfully initialized.
 *@par Dependency:
 *<ul>
 *<li>pthread.h: the header file that contains the API declaration.</li>
 *</ul>
 *@see pthread_attr_destroy
 *@since Huawei LiteOS V100R001C00
 */
extern int pthread_attr_init(pthread_attr_t *attr);

/* Destroy thread attributes object */
/**
 *@ingroup pthread
 *@brief Destroy a thread attributes object.
 *
 *@par Dependency:
 *This API is used to destroy a thread attributes object.
 *@attention
 *<ul>
 *<li>This API does not take effect on Huawei LiteOS Kernel.</li>
 *</ul>
 *
 *@param   attr    [IN] Pointer to the thread attributes object to be destroyed.
 *
 *@retval  #EINVAL    The input parameter is NULL.
 *@retval  #ENOERR    The thread attributes object is successfully destroyed.
 *@par Dependency:
 *<ul>
 *<li>pthread.h: the header file that contains the API declaration.</li>
 *</ul>
 *@see pthread_attr_init
 *@since Huawei LiteOS V100R001C00
 */
extern int pthread_attr_destroy(pthread_attr_t *attr);

/* Set the detachstate attribute */
/**
 *@ingroup pthread
 *@brief Set the detach state attribute of a thread attributes object.
 *
 *@par Description:
 *This API is used to set the detach state attribute of a thread attributes object.
 *@attention
 *<ul>
 *<li>The detach state attribute must be either PTHREAD_CREATE_JOINABLE or PTHREAD_CREATE_DETACHED. Otherwise, the attempt to set the detach state attribute will fail.</li>
 *</ul>
 *
 *@param   attr          [OUT] Pointer to the thread attributes object to be set.
 *@param   detachstate   [IN]  Detach state attribute.
 *
 *@retval  #EINVAL    invalid parameter.
 *@retval  #ENOERR    The detach state attribute is successfully set.
 *@par Dependency:
 *<ul>
 *<li>pthread.h: the header file that contains the API declaration.</li>
 *</ul>
 *@see pthread_attr_getdetachstate
 *@since Huawei LiteOS V100R001C00
 */
extern int pthread_attr_setdetachstate(pthread_attr_t *attr, int detachstate);

/* Get the detachstate attribute */
/**
 *@ingroup pthread
 *@brief Acquire the detach state attribute of a thread attributes object.
 *
 *@par Description:
 *This API is used to acquire the detach state attribute of a thread attributes object.
 *@attention
 *<ul>
 *<li>None.</li>
 *</ul>
 *
 *@param   attr          [IN]  Pointer to the thread attributes object to be read.
 *@param   detachstate   [OUT] Pointer to the acquired detach state attribute.
 *
 *@retval  #EINVAL    invalid parameter.
 *@retval  #ENOERR    The detach state attribute is successfully acquired.
 *@par Dependency:
 *<ul>
 *<li>pthread.h: the header file that contains the API declaration.</li>
 *</ul>
 *@see pthread_attr_setdetachstate
 *@since Huawei LiteOS V100R001C00
 */
extern int pthread_attr_getdetachstate(const pthread_attr_t *attr, int *detachstate);

/* Set scheduling contention scope */
/**
 *@ingroup pthread
 *@brief Set the contention scope attribute of a thread attributes object.
 *
 *@par Description:
 *This API is used to set the contention scope attribute of a thread attributes object.
 *@attention
 *<ul>
 *<li>The contention scope attribute can only be PTHREAD_SCOPE_SYSTEM. PTHREAD_SCOPE_PROCES is not supported.</li>
 *</ul>
 *
 *@param   attr      [OUT] Pointer to the thread attributes object to be set.
 *@param   scope	 [IN]  Contention scope attribute.
 *
 *@retval  #EINVAL    invalid parameter.
 *@retval  #ENOTSUP   The specified contention scope attribute value is not supported.
 *@retval  #ENOERR    The contention scope attribute is successfully set.
 *@par Dependency:
 *<ul>
 *<li>pthread.h: the header file that contains the API declaration.</li>
 *</ul>
 *@see pthread_attr_getscope
 *@since Huawei LiteOS V100R001C00
 */
extern int pthread_attr_setscope(pthread_attr_t *attr, int scope);

/* Get scheduling contention scope */
/**
 *@ingroup pthread
 *@brief Acquire the contention scope attribute of a thread attributes object.
 *
 *@par Description:
 *This API is used to acquire the contention scope attribute of a thread attributes object.
 *@attention
 *<ul>
 *<li>None.</li>
 *</ul>
 *
 *@param   attr    [IN]  Pointer to the thread attributes object to be read.
 *@param   scope   [OUT] Pointer to the acquired contention scope attribute.
 *
 *@retval  #EINVAL    invalid parameter.
 *@retval  #ENOERR    The contention scope attribute is successfully acquired.
 *@par Dependency:
 *<ul>
 *<li>pthread.h: the header file that contains the API declaration.</li>
 *</ul>
 *@see pthread_attr_setscope
 *@since Huawei LiteOS V100R001C00
 */
extern int pthread_attr_getscope(const pthread_attr_t *attr, int *scope);

/* Set scheduling inheritance attribute */
/**
 *@ingroup pthread
 *@brief Set the inherit scheduler attribute of a thread attributes object.
 *
 *@par Description:
 *This API is used to set the inherit scheduler attribute of a thread attributes object.
 *@attention
 *<ul>
 *<li>The inherit scheduler attribute must be either PTHREAD_INHERIT_SCHED or PTHREAD_EXPLICIT_SCHED.</li>
 *</ul>
 *
 *@param   attr       [OUT] Pointer to the thread attributes object to be set.
 *@param   inherit	  [IN]  Inherit scheduler attribute.
 *
 *@retval  #EINVAL    invalid parameter.
 *@retval  #ENOERR    The inherit scheduler attribute is successfully set.
 *@par Dependency:
 *<ul>
 *<li>pthread.h: the header file that contains the API declaration.</li>
 *</ul>
 *@see pthread_attr_getinheritsched
 *@since Huawei LiteOS V100R001C00
 */
extern int pthread_attr_setinheritsched (pthread_attr_t *attr, int inherit);

/* Get scheduling inheritance attribute */
/**
 *@ingroup pthread
 *@brief Acquire the inherit scheduler attribute of a thread attributes object.
 *
 *@par Description:
 *This API is used to acquire the inherit scheduler attribute of a thread attributes object.
 *@attention
 *<ul>
 *<li>None.</li>
 *</ul>
 *
 *@param   attr      [IN]  Pointer to the thread attributes object to be read.
 *@param   inherit   [OUT] Pointer to the acquired inherit scheduler attribute.
 *
 *@retval  #EINVAL    invalid parameter.
 *@retval  #ENOERR    The inherit scheduler attribute is successfully acquired.
 *@par Dependency:
 *<ul>
 *<li>pthread.h: the header file that contains the API declaration.</li>
 *</ul>
 *@see pthread_attr_setinheritsched
 *@since Huawei LiteOS V100R001C00
 */
extern int pthread_attr_getinheritsched(const pthread_attr_t *attr, int *inherit);

/* Set scheduling policy */
/**
 *@ingroup pthread
 *@brief Set the scheduling policy attribute of a thread attributes object.
 *
 *@par Description:
 *This API is used to set the scheduling policy attribute of a thread attributes object.
 *@attention
 *<ul>
 *<li>The scheduling policy attribute is SCHED_OTHER, SCHED_FIFO, or SCHED_RR.</li>
 *<li>Only SCHED_RR is supported now. An error code will be returned if the value is set to the other two scheduling policies.</li>
 *</ul>
 *
 *@param   attr    [OUT] Pointer to the thread attributes object to be set.
 *@param   policy  [IN]  Scheduling policy attribute.
 *
 *@retval  #EINVAL    invalid parameter.
 *@retval  #ENOERR    The scheduling policy attribute is successfully set.
 *@par Dependency:
 *<ul>
 *<li>pthread.h: the header file that contains the API declaration.</li>
 *</ul>
 *@see pthread_attr_getschedpolicy
 *@since Huawei LiteOS V100R001C00
 */
extern int pthread_attr_setschedpolicy(pthread_attr_t *attr, int policy);

/* Get scheduling policy */
/**
 *@ingroup pthread
 *@brief Acquire the scheduling policy attribute of a thread attributes object.
 *
 *@par Description:
 *This API is used to acquire the scheduling policy attribute of a thread attributes object.
 *@attention
 *<ul>
 *<li>None.</li>
 *</ul>
 *
 *@param   attr      [IN]  Pointer to the thread attributes object to be read.
 *@param   policy    [OUT] Pointer to the acquired scheduling policy attribute.
 *
 *@retval  #EINVAL    invalid parameter.
 *@retval  #ENOERR    The scheduling policy attribute is successfully obtained.
 *@par Dependency:
 *<ul>
 *<li>pthread.h: the header file that contains the API declaration.</li>
 *</ul>
 *@see pthread_attr_setschedpolicy
 *@since Huawei LiteOS V100R001C00
 */
extern int pthread_attr_getschedpolicy(const pthread_attr_t *attr, int *policy);

/* Set scheduling parameters */
/**
 *@ingroup pthread
 *@brief Set the scheduling parameter attributes of a thread attributes object.
 *
 *@par Description:
 *This API is used to set the scheduling parameter attributes of a thread attributes object. The scheduling parameter attributes indicate thread priorities.
 *@attention
 *<ul>
 *<li>The priority of the scheduling parameter attributes must be in the range of [OS_TSK_PRIORITY_LOWEST,OS_TSK_PRIORITY_HIGHEST].</li>
 *</ul>
 *
 *@param   attr    [OUT] Pointer to the thread attributes object to be set.
 *@param   param   [IN]  Pointer to the scheduling parameter attributes object.
 *
 *@retval  #EINVAL    invalid parameter.
 *@retval  #ENOERR    The scheduling parameter attributes are successfully set.
 *@par Dependency:
 *<ul>
 *<li>pthread.h: the header file that contains the API declaration.</li>
 *</ul>
 *@see pthread_attr_getschedparam
 *@since Huawei LiteOS V100R001C00
 */
extern int pthread_attr_setschedparam(pthread_attr_t *attr, const struct sched_param *param);

/* Get scheduling parameters */
/**
 *@ingroup pthread
 *@brief Acquire the scheduling parameter attributes of a thread attributes object.
 *
 *@par Description:
 *This API is used to acquire the scheduling parameter attributes of a thread attributes object. The scheduling parameter attributes indicate the thread priorities.
 *@attention
 *<ul>
 *<li>None.</li>
 *</ul>
 *
 *@param   attr    [IN]  Pointer to the thread attributes object to be read.
 *@param   param   [OUT] Pointer to the acquired scheduling parameter attributes object.
 *
 *@retval  #EINVAL    invalid parameter.
 *@retval  #ENOERR    The scheduling parameter attributes are successfully acquired.
 *@par Dependency:
 *<ul>
 *<li>pthread.h: the header file that contains the API declaration.</li>
 *</ul>
 *@see pthread_attr_setschedparam
 *@since Huawei LiteOS V100R001C00
 */
extern int pthread_attr_getschedparam(const pthread_attr_t *attr, struct sched_param *param);

/* Set starting address of stack. Whether this is at the start or end of
   the memory block allocated for the stack depends on whether the stack
   grows up or down.
   */
/**
 *@ingroup pthread
 *@brief Set the stack address attribute of a thread attributes object.
 *
 *@par Description:
 *This API is used to set the stack address attribute of a thread attributes object. This attribute specifies the start address of a stack.
 *@attention
 *<ul>
 *<li>Critical errors will occur if the stack address is incorrectly set. Call this API with caution.</li>
 *<li>This API does not take effect on Huawei LiteOS Kernel.</li>
 *</ul>
 *
 *@param   attr        [OUT] Pointer to the thread attributes object to be set.
 *@param   stackaddr   [IN]  Specified stack address.
 *
 *@retval  #EINVAL    invalid parameter.
 *@retval  #ENOERR    The stack address attribute is successfully set.
 *@par Dependency:
 *<ul>
 *<li>pthread.h: the header file that contains the API declaration.</li>
 *</ul>
 *@see pthread_attr_getstackaddr
 *@since Huawei LiteOS V100R001C00
 */
extern int pthread_attr_setstackaddr(pthread_attr_t *attr, void *stackaddr);

/* Get any previously set stack address. */
/**
 *@ingroup pthread
 *@brief Acquire the stack address attribute of a thread attributes object.
 *
 *@par Description:
 *This API is used to acquire the stack address attribute of a thread attributes object. This attribute specifies the starting address of a stack.
 *@attention
 *<ul>
  *<li>This API does not take effect on Huawei LiteOS Kernel.</li>
 *</ul>
 *
 *@param   attr        [IN]  Pointer to the thread attributes object to be read.
 *@param   stackaddr   [OUT] Pointer to the acquired starting address of stack.
 *
 *@retval  #EINVAL    invalid parameter.
 *@retval  #ENOERR    The stack address attribute is successfully acquired.
 *@par Dependency:
 *<ul>
 *<li>pthread.h: the header file that contains the API declaration.</li>
 *</ul>
 *@see pthread_attr_setstackaddr
 *@since Huawei LiteOS V100R001C00
 */
extern int pthread_attr_getstackaddr(const pthread_attr_t *attr, void **stackaddr);

/* Set minimum creation stack size. */
/**
 *@ingroup pthread
 *@brief Set the stack size attribute of a thread attribute object.
 *
 *@par Description:
 *This API is used to set the stack size attribute of a thread attribute object.
 *@attention
 *<ul>
 *<li>The stack size must fall within an appropriate range and be greater than PTHREAD_STACK_MIN. If the stack size attribute is left unspecified, the default stack size will be used.</li>
 *</ul>
 *
 *@param   attr       [OUT] Pointer to the thread attributes object to be set.
 *@param   stacksize  [IN]  Specified stack size.
 *
 *@retval  #EINVAL   invalid parameter.
 *@retval  #ENOERR   The stack size attribute is successfully set.
 *@par Dependency:
 *<ul>
 *<li>pthread.h: the header file that contains the API declaration.</li>
 *</ul>
 *@see pthread_attr_getstacksize
 *@since Huawei LiteOS V100R001C00
 */
extern int pthread_attr_setstacksize(pthread_attr_t *attr, size_t stacksize);

/* Get current minimal stack size. */
/**
 *@ingroup pthread
 *@brief Acquire the stack size attribute of a thread attribute object.
 *
 *@par Description:
 *This API is used to acquire the stack size attribute of a thread attribute object.
 *@attention
 *<ul>
 *<li>None.</li>
 *</ul>
 *
 *@param   attr        [IN]  Pointer to the thread attributes object to be read.
 *@param   stacksize   [OUT] Pointer to the acquired stack size.
 *
 *@retval  #EINVAL   invalid parameter, or the stack size left unspecified.
 *@retval  #ENOERR   The stack size attribute is successfully acquired.
 *@par Dependency:
 *<ul>
 *<li>pthread.h: the header file that contains the API declaration.</li>
 *</ul>
 *@see pthread_attr_setstacksize
 *@since Huawei LiteOS V100R001C00
 */
extern int pthread_attr_getstacksize(const pthread_attr_t *attr, size_t *stacksize);

/* Thread scheduling controls */

/* Set scheduling policy and parameters for the thread */
/**
 *@ingroup pthread
 *@brief Set the scheduling policy and priority of a thread.
 *
 *@par Description:
 *This API is used to set the scheduling policy and priority of a thread.
 *@attention
 *<ul>
 *<li>The scheduling policy must be SCHED_OTHER, SCHED_FIFO, or SCHED_RR.</li>
 *<li>The content priority specified by the param parameter must fall within the OS priority range[OS_TSK_PRIORITY_LOWEST,OS_TSK_PRIORITY_HIGHEST].</li>
 *<li>A smaller priority value indicates a higher priority.</li>
 *</ul>
 *@param   pthread   [IN] ID of the thread whose scheduling policy is to be set.
 *@param   policy    [IN] Specified scheduling policy.
 *@param   param     [IN] Pointer to the scheduling priority.
 *
 *@retval  #EINVAL   invalid parameter.
 *@retval  #ENOERR   The scheduling policy and priority of the thread is successfully set.
 *@par Dependency:
 *<ul>
 *<li>pthread.h: the header file that contains the API declaration.</li>
 *</ul>
 *@see pthread_getschedparam
 *@since Huawei LiteOS V100R001C00
 */
extern int pthread_setschedparam(pthread_t pthread, int policy, const struct sched_param *param);

/* Get scheduling policy and parameters for the thread */
/**
 *@ingroup pthread
 *@brief Acquire the scheduling policy and priority of a thread.
 *
 *@par Description:
 *This API is used to acquire the scheduling policy and priority of a thread.
 *@attention
 *<ul>
 *<li>None.</li>
 *</ul>
 *@param   pthread   [IN]  ID of the thread whose scheduling policy is to be read.
 *@param   policy    [OUT] Acquired scheduling policy.
 *@param   param     [OUT] Acquired scheduling priority.
 *
 *@retval  #EINVAL   invalid parameter.
 *@retval  #ENOERR   The scheduling policy and priority of the thread is successfully acquired.
 *@par Dependency:
 *<ul>
 *<li>pthread.h: the header file that contains the API declaration.</li>
 *</ul>
 *@see pthread_setschedparam
 *@since Huawei LiteOS V100R001C00
 */
extern int pthread_getschedparam(pthread_t pthread, int *policy, struct sched_param *param);

/* Dynamic package initialization */

/* Initializer for pthread_once_t instances */
/**
 * @ingroup pthread
 * Define the initial value of pthread_once_t.
 */
#define PTHREAD_ONCE_INIT   0

/* Call init_routine just the once per control variable. */
/**
 *@ingroup pthread
 *@brief Call the initialization routine at most once.
 *
 *@par  Decription:
 *This API is used to call the initialization routine at most once.
 *@attention
 *<ul>
 *<li>The control variable once_control must be statically initialized using PTHREAD_ONCE_INIT. Otherwise, this API will not work as expected.</li>
 *</ul>
 *@param   once_control    [IN/OUT]  Once-control parameter.
 *@param   init_routine    [IN]      Initialization routine function.
 *
 *@retval  #EINVAL  : One or more parameters are invalid.
 *@retval  #1 The thread has already been initialized.
 *@retval  #0 The initialization routine is successfully called.
 *@par Dependency:
 *<ul>
 *<li>pthread.h: the header file that contains the API declaration.</li>
 *</ul>
 *@see None.
 *@since Huawei LiteOS V100R001C00
 */
extern int pthread_once(pthread_once_t *once_control, void (*init_routine)(void));

/* Thread specific data */

/* Create a key to identify a location in the thread specific data area.
   Each thread has its own distinct thread-specific data area but all are
   addressed by the same keys. The destructor function is called whenever a
   thread exits and the value associated with the key is non-NULL.
   */
/**
 *@ingroup pthread
 *@brief Create a thread-specific data key.
 *
 *@par Description:
 *This API is used to create a key that indentifies the thread-specific data.
 *@attention
 *<ul>
 *<li>Not support.</li>
 *</ul>
 *@param   key           [OUT]  Pointer to a key value.
 *@param   destructor    [IN]   Destructor function.
 *
 *@retval  #EINVAL         invalid parameter.
 *@retval  #ENOERR         The thread-specific data key is successfully created.
 *@par Dependency:
 *<ul>
 *<li>pthread.h: the header file that contains the API declaration.</li>
 *</ul>
 *@see pthread_key_delete
 *@since Huawei LiteOS V100R001C00
 */
extern int pthread_key_create(pthread_key_t *key, void (*destructor)(void *));

/* Delete key. */
/**
 *@ingroup pthread
 *@brief Delete a thread-specific data key.
 *
 *@par Description:
 *This API is used to delete the thread-specific key created by pthread_key_create.
 *@attention
 *<ul>
 *<li>Not support.</li>
 *</ul>
 *@param   key      [IN]  Key to be deleted.
 *
 *@retval  #ENOERR     The thread-specific data key is successfully deleted.
 *@par Dependency:
 *<ul>
 *<li>pthread.h: the header file that contains the API declaration</li>.
 *</ul>
 *@see pthread_key_create
 *@since Huawei LiteOS V100R001C00
 */
extern int pthread_key_delete(pthread_key_t key);

/* Store the pointer value in the thread-specific data slot addressed
   by the key.
   */
/**
 *@ingroup pthread
 *@brief Set the sharing of specific data.
 *
 *@par Description:
 *This API is used to set a key as the location of the shared memory.
 *@attention
 *<ul>
 *<li>Not support.</li>
 *</ul>
 *@param   key           [IN]  Key to be set.
 *@param   pointer       [IN]  Location of the shared memory.
 *
 *@retval  #EINVAL     invalid parameter.
 *@retval  #ENOERR     The location of the shared memory is successfully set.
 *@par Dependency:
 *<ul>
 *<li>pthread.h: the header file that contains the API declaration.</li>
 *</ul>
 *@see pthread_getspecific
 *@since Huawei LiteOS V100R001C00
 */
extern int pthread_setspecific(pthread_key_t key, const void *pointer);

/* Retrieve the pointer value in the thread-specific data slot addressed
   by the key.
   */
/**
 *@ingroup pthread
 *@brief Acquire the shared memory addressed by a key.
 *
 *@par Description:
 *This API is used to acquire the shared memory addressed by a key.
 *@attention
 *<ul>
 *<li>Not support.</li>
 *</ul>
 *@param   key  [IN]  Key by which the share memory to be acquired is addressed.
 *
 *@retval void* Location of the shared memory.
 *@par Dependency:
 *<ul>
 *<li>pthread.h: the header file that contains the API declaration.</li>
 *</ul>
 *@see pthread_setspecific
 *@since Huawei LiteOS V100R001C00
 */
extern void *pthread_getspecific(pthread_key_t key);

/* Thread Cancellation */

/* Data structure used to manage cleanup functions */
/**
 * @ingroup pthread
 * Structure for managing cleanup functions
 */
struct pthread_cleanup_buffer
{
    struct pthread_cleanup_buffer *prev;        /**< Pointer to the buffer to be cleaned*/
    void (*routine)(void *);     	        /**< General execution function*/
    void *arg;				        /**< Parameter of the execution function*/
};

/* Thread cancelled return value.
   This is a value returned as the retval in pthread_join() of a
   thread that has been cancelled. By making it the address of a
   location we define we can ensure that it differs from NULL and any
   other valid pointer (as required by the standard).
   */
/**
 * @ingroup pthread
 * Declare a global variable that will be used to store the return value of pthread_cancele.
 */
extern int pthread_canceled_dummy_var;

/**
 * @ingroup pthread
 * Define the return value of pthread_cancele.
 */
#define PTHREAD_CANCELED    -1

/* Cancelability enable and type */
/**
 * @ingroup pthread
 * Enable cancellation.
 */
#define PTHREAD_CANCEL_ENABLE   1

/**
 * @ingroup pthread
 * Disable cancellation.
 */
#define PTHREAD_CANCEL_DISABLE  2

/**
 * @ingroup pthread
 * Asynchronous cancellation.
 */
#define PTHREAD_CANCEL_ASYNCHRONOUS 1

/**
 * @ingroup pthread
 * Deferred cancellation.
 */
#define PTHREAD_CANCEL_DEFERRED 2

/* Functions */

/* Set cancel state of current thread to ENABLE or DISABLE.
   Returns old state in *oldstate.
   */
/**
 *@ingroup pthread
 *@brief Set the cancellation state (PTHREAD_CANCEL_ENABLE or PTHREAD_CANCEL_DISABLE) of a thread.
 *
 *@par Description:
 *This API is used to set the cancellation state (PTHREAD_CANCEL_ENABLE or PTHREAD_CANCEL_DISABLE) of a thread.
 *@attention
 *<ul>
 *<li>The cancellation state of a thread must be set to PTHREAD_CANCEL_ENABLE or PTHREAD_CANCEL_DISABLE. If it is set to neither of the two states, EINVAL will be returned.</li>
 *<li>The value of the oldstate parameter can be NULL, indicating that the original cancellation state will not be stored.</li>
 *</ul>
 *@param   state     [IN]   Cancellation state to be set.
 *@param   oldstate  [OUT]  Pointer to the original cancellation state.
 *
 *@retval  #EINVAL      invalid parameter.
 *@retval  #ENOERR      The cancellation state is successfully set.
 *@par Dependency:
 *<ul>
 *<li>pthread.h: the header file that contains the API declaration.</li>
 *</ul>
 *@see None.
 *@since Huawei LiteOS V100R001C00
 */
extern int pthread_setcancelstate(int state, int *oldstate);

/* Set cancel type of current thread to ASYNCHRONOUS or DEFERRED.
   Returns old type in *oldtype.
   */
/**
 *@ingroup pthread
 *@brief Set the cancellation type of a thread.
 *
 *@par Description:
 *This API is used to set the cancellation type (PHREAD_CANCEL_ASYNCHRONOUS or PTHREAD_CANCEL_DEFERRED) of a thread.
 *@attention
 *<ul>
 *<li>The cancellation type of a thread must be set to PHREAD_CANCEL_ASYNCHRONOUS or PTHREAD_CANCEL_DEFERRED. If it is set to neither of the two types, EINVAL will be returned.</li>
 *<li>The value of the oldtype parameter can be NULL, indicating that the original cancellation type will not be stored.</li>
 *</ul>
 *@param   type      [IN]   Cancellation type to be set.
 *@param   oldtype   [OUT]  Pointer to the original cancellation type.
 *
 *@retval  #EINVAL      invalid parameter.
 *@retval  #ENOERR      The cancellation type is successfully set.
 *@par Dependency:
 *<ul>
 *<li>pthread.h: the header file that contains the API declaration.</li>
 *</ul>
 *@see None.
 *@since Huawei LiteOS V100R001C00
 */
extern int pthread_setcanceltype(int type, int *oldtype);

/* Cancel the thread. */
/**
 *@ingroup pthread
 *@brief Cancel a thread.
 *
 *@par Description:
 *This API is used to cancel a thread.
 *@attention
 *<ul>
 *<li>Before the pthread_cancel API is called, the PTHREAD_CANCEL_ASYNCHRONOUS attribute must be set.</li>
 *<li>The input parameter must be a legal thread id.</li>
 *</ul>
 *@param   pthread  [IN]  Thread to be canceled.
 *
 *@retval  #ESRCH      The thread does not exist.
 *@retval  #ENOERR     The thread is successfully canceled.
 *@par Dependency:
 *<ul>
 *<li>pthread.h: the header file that contains the API declaration.</li>
 *</ul>
 *@see None.
 *@since Huawei LiteOS V100R001C00
 */
extern int pthread_cancel(pthread_t pthread);

/* Test for a pending cancellation for the current thread and terminate
   the thread if there is one.
   */
/**
 *@ingroup pthread
 *@brief Check whether the current thread is in the Canceled state. The thread will exit if it is in the Canceled state.
 *
 *@par Description:
 *This API is used to check whether the current thread is in the Canceled state. If in Canceled state, the thread will exit. If not in Canceled state, no further action will be taken.
 *@attention
 *<ul>
 *<li>The current system does not support the Canceled point.</li>
 *</ul>
 *@param   None.
 *
 *@retval  None.
 *@par Dependency:
 *<ul>
 *<li>pthread.h: the header file that contains the API declaration.</li>
 *</ul>
 *@see None.
 *@since Huawei LiteOS V100R001C00
 */
extern void pthread_testcancel(void);

/* Install a cleanup routine.
   Note that pthread_cleanup_push() and pthread_cleanup_pop() are macros that
   must be used in matching pairs and at the same brace nesting level.
   */

/* NOTE: The involved function is not currently supported. So the macro become invalid too. */
#define pthread_cleanup_push(__routine, __arg)  \
{   \
    struct pthread_cleanup_buffer _buffer_; \
    pthread_cleanup_push_inner (&_buffer_, (__routine), (__arg));

/* NOTE: The involved function is not currently supported. So the macro become invalid too. */
#define pthread_cleanup_pop(__execute)  \
    pthread_cleanup_pop_inner (&_buffer_, (__execute)); \
}

/* NOTE: This function is not currently supported. */
extern void pthread_cleanup_push_inner(struct pthread_cleanup_buffer *buffer, void (*routine)(void *), void *arg);
/* NOTE: This function is not currently supported. */
extern void pthread_cleanup_pop_inner(struct pthread_cleanup_buffer *buffer, int execute);

/* NOTE: This function is not currently supported. */
extern int pthread_mutex_timedlock(pthread_mutex_t *__mutex, const struct timespec * __abstime);


#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif
