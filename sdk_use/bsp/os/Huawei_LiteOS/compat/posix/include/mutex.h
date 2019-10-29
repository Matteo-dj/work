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

/*Note:
  1. To apply both INHERIT and PROTECT mutex protocols, enable them in  PTHREAD_PRIO_INHERIT and PTHREAD_PRIO_PROTECT in the header file.
  2. Before changing the default mutex protocol, ensure that POSIX_MUTEX_DEFAULT_INHERIT and POSIX_MUTEX_DEFAULT_PROTECT can be user defined.
  3. The default mutex type in Huawei LiteOS is recursive INHERIT mutex. Posix mutexes support the change of mutex type (by default, the mutex type is normal mutex, and the mutex protocol is INHERIT).
  */

/**@defgroup mutex Mutex
 * @ingroup posix
 */

#ifndef _HWLITEOS_POSIX_MUTEX_H
#define _HWLITEOS_POSIX_MUTEX_H

#include "muttypes.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */


/**
 *@ingroup mutex
 *@brief Initialize a mutex.
 *
 *@par Description:
 *<ul>
 *<li>This API is used to initialize a mutex has a specified mutex attribute.</li>
 *</ul>
 *@attention
 *<ul>
 *<li>Do not initialize a mutex that has been initialized or destroyed.</li>
 *</ul>
 *
 *@param mutex   [OUT] Mutex object.
 *@param attr    [IN] Mutex attributes object. A mutex attribute value can be the default value or a user-defined value.
 *
 *@retval #EINVAL   invalid parameter.
 *@retval #ENOERR   The mutex is successfully initialized.
 *@par Dependency:
 *<ul><li>mutex.h: the header file that contains the API declaration.</li></ul>
 *@see pthread_mutex_destroy
 *@since Huawei LiteOS V100R001C00
 */
int pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr);

/**
 *@ingroup mutex
 *@brief Destroy a mutex.
 *
 *@par Description:
 *<ul>
 *<li>This API is used to destroy a mutex.</li>
 *</ul>
 *@attention
 *<ul>
 *<li>None.</li>
 *</ul>
 *
 *@param mutex   [IN] Mutex object.
 *
 *@retval #EINVAL  invalid parameter.
 *@retval #EBUSY   The mutex is being in use.
 *@retval #ENOERR  The mutex is successfully destroyed.
 *@par Dependency:
 *<ul><li>mutex.h: the header file that contains the API declaration.</li></ul>
 *@see pthread_mutex_init
 *@since Huawei LiteOS V100R001C00
 */
int pthread_mutex_destroy(pthread_mutex_t *mutex);

/**
 *@ingroup mutex
 *@brief Lock a mutex.
 *
 *@par Description:
 *<ul>
 *<li>This API is used to lock a mutex. The lock operation varies with the mutex type and protocol.</li>
 *</ul>
 *@attention
 *<ul>
 *<li>A recursive mutex can be locked more than once. A normal mutex does not check errors, so a deadlock will result when a normal mutex is relocked. An error checking mutex is able to check errors. Error code will be returned if a mutex is relocked.</li>
 *<li>Do not call this API in software timer callback. </li>
 *</ul>
 *
 *@param mutex   [IN] Mutex object.
 *
 *@retval #EINVAL   invalid parameter.
 *@retval #EPERM    The operation is not allowed during an interrupt.
 *@retval #EDEADLK  Relocking a mutex causes a deadlock.
 *@retval #ENOERR   The mutex is successfully locked.
 *@par Dependency:
 *<ul>
 *<li>mutex.h: the header file that contains the API declaration.</li>
 *</ul>
 *@see pthread_mutex_destroy
 *@since Huawei LiteOS V100R001C00
 */
int pthread_mutex_lock(pthread_mutex_t *mutex);

/**
 *@ingroup mutex
 *@brief Unlock a mutex.
 *
 *@par Description:
 *<ul>
 *<li>This API is used to unlock a mutex.</li>
 *</ul>
 *@attention
 *<ul>
 *<li>Do not call this API in software timer callback. </li>
 *</ul>
 *
 *@param mutex   [IN] Mutex object.
 *
 *@retval #EINVAL invalid parameter.
 *@retval #EPERM  The operation is not allowed during an interrupt.
 *@retval #ENOERR The mutex is successfully unlocked, or the mutex being unlocked has never been locked before.
 *@par Dependency:
 *<ul><li>mutex.h: the header file that contains the API declaration.</li></ul>
 *@see pthread_mutex_lock
 *@since Huawei LiteOS V100R001C00
 */
int pthread_mutex_unlock(pthread_mutex_t *mutex);

/**
 *@ingroup mutex
 *@brief Try to lock a mutex.
 *
 *@par Description:
 *<ul>
 *<li>If you try to lock a mutex when it is already locked, a deadlock will result. This API is used to try to lock the mutex.</li>
 *</ul>
 *@attention
 *<ul>
 *<li>Do not call this API in software timer callback. </li>
 *</ul>
 *
 *@param mutex   [IN] Mutex object.
 *
 *@retval #EINVAL  invalid parameter.
 *@retval #EPERM   The operation is not allowed during an interrupt.
 *@retval #EBUSY   The lock is already locked. In this case, EBUSY will be returned if you try to lock a non-recursive mutex. If you try to lock a recursive mutex, it will be locked.
 *@retval #ENOERR  The operation is successful.
 *@retval #EDEADLK The mutex is being locked during a mutex task.
 *@par Dependency:
 *<ul><li>mutex.h: the header file that contains the API declaration.</li></ul>
 *@see pthread_mutex_lock,pthread_mutex_unlock
 *@since Huawei LiteOS V100R001C00
 */
int pthread_mutex_trylock(pthread_mutex_t *mutex);

/**
 *@ingroup mutex
 *@brief Obtain the mutex type.
 *
 *@par Description:
 *<ul>
 *<li>This API is used to obtain the mutex type.</li>
 *</ul>
 *@attention
 *<ul>
 *<li></li>
 *</ul>
 *
 *@param attr   [IN] Mutex attributes.
 *@param type   [OUT] Mutex type.
 *
 *@retval #EINVAL  invalid parameter.
 *@retval #ENOERR  The mutex type is successfully obtained.
 *@par Dependency:
 *<ul>
 *<li>mutex.h: the header file that contains the API declaration.</li>
 *<li>muttypes.h: the header file that contains the mutex attributes.</li>
 *</ul>
 *@see pthread_mutexattr_settype
 *@since Huawei LiteOS V100R001C00
 */
int pthread_mutexattr_gettype(const pthread_mutexattr_t *attr, int *type);

/**
 *@ingroup mutex
 *@brief Set the mutex type.
 *
 *@par Description:
 *<ul>
 *<li>This API is used to set the mutex type.</li>
 *</ul>
 *@attention
 *<ul>
 *<li>None.</li>
 *</ul>
 *
 *@param attr   [OUT] Mutex attributes.
 *@param type   [IN] Mutex type, [0, 2].
 *
 *@retval #EINVAL  invalid parameter.
 *@retval #ENOERR  The mutex type is successfully set.
 *@par Dependency:
 *<ul>
 *<li>mutex.h: the header file that contains the API declaration.</li>
 *<li>muttypes.h: the header file that contains the mutex attributes.</li>
 *</ul>
 *@see pthread_mutexattr_gettype
 *@since Huawei LiteOS V100R001C00
 */
int pthread_mutexattr_settype(pthread_mutexattr_t *attr, int type);

/**
 *@ingroup mutex
 *@brief Initialize mutex attributes.
 *
 *@par Description:
 *<ul>
 *<li>This API is used to initialize mutex attributes.</li>
 *</ul>
 *@attention
 *<ul>
 *<li>The default mutex protocol is _POSIX_THREAD_PRIO_INHERITand is assigned the lowest mutex priority and the default mutex type.</li>
 *</ul>
 *
 *@param attr   [IN/OUT] Mutex attributes.
 *
 *@retval #EINVAL  invalid parameter.
 *@retval #ENOERR  The mutex attributes are successfully initialized.
 *@par Dependency:
 *<ul>
 *<li>mutex.h: the header file that contains the API declaration.</li>
 *<li>muttypes.h: the header file that contains the mutex attributes.</li>
 *</ul>
 *@see pthread_mutexattr_destroy
 *@since Huawei LiteOS V100R001C00
 */
int pthread_mutexattr_init(pthread_mutexattr_t *attr);

/**
 *@ingroup mutex
 *@brief Destroy mutex attributes.
 *
 *@par Description:
 *<ul>
 *<li>This API is used to destroy mutex attributes.</li>
 *</ul>
 *@attention
 *<ul>
 *<li>None.</li>
 *</ul>
 *
 *@param attr   [IN] Mutex attributes.
 *
 *@retval #EINVAL  invalid parameter.
 *@retval #ENOERR  The mutex attributes are successfully destroyed.
 *@par Dependency:
 *<ul>
 *<li>mutex.h: the header file that contains the API declaration.</li>
 *<li>muttypes.h: the header file that contains the mutex attributes.</li>
 *</ul>
 *@see pthread_mutexattr_init
 *@since Huawei LiteOS V100R001C00
 */
int pthread_mutexattr_destroy(pthread_mutexattr_t *attr);

/**
 *@ingroup mutex
 *@brief Set the protocol of a mutex for defining mutex attributes.
 *
 *@par Description:
 *<ul>
 *<li>This API is used to set the protocol for defining mutex attributes.</li>
 *</ul>
 *@attention
 *<ul>
 *<li>None.</li>
 *</ul>
 *
 *@param attr       [OUT] Mutex attributes.
 *@param protocol   [IN] Protocol for defining mutex attributes, and the value range is [0, 2].
 *
 *@retval #EINVAL  invalid parameter.
 *@retval #ENOERR  The protocol is successfully set.
 *@par Dependency:
 *<ul>
 *<li>mutex.h: the header file that contains the API declaration.</li>
 *<li>muttypes.h: the header file that contains the mutex attributes.</li>
 *</ul>
 *@see pthread_mutexattr_getprotocol
 *@since Huawei LiteOS V100R001C00
 */
int pthread_mutexattr_setprotocol(pthread_mutexattr_t *attr,int protocol);

/**
 *@ingroup mutex
 *@brief Obtain the mutex protocol in mutex attributes.
 *
 *@par Description:
 *<ul>
 *<li>This API is used to obtain the mutex protocol in mutex attributes.</li>
 *</ul>
 *@attention
 *<ul>
 *<li>None.</li>
 *</ul>
 *
 *@param attr       [IN] Mutex attributes.
 *@param protocol   [OUT] Mutex protocol in mutex attributes.
 *
 *@retval #EINVAL  invalid parameter.
 *@retval #ENOERR  The mutex protocol in mutex attributes is successfully obtained.
 *@par Dependency:
 *<ul>
 *<li>mutex.h: the header file that contains the API declaration.</li>
 *<li>muttypes.h: the header file that contains the mutex attributes.</li>
 *</ul>
 *@see pthread_mutexattr_setprotocol
 *@since Huawei LiteOS V100R001C00
 */
int pthread_mutexattr_getprotocol(pthread_mutexattr_t *attr, int *protocol);


/**
 *@ingroup mutex
 *@brief Set the upper priority limit in mutex attributes.
 *
 *@par Description:
 *<ul>
 *<li>This API is used to set the upper priority limit in mutex attributes.</li>
 *</ul>
 *@attention
 *<ul>
 *<li>The default priority is the lowest priority. If the upper priority limit is below the default priority, priority inversion will occur.</li>
 *</ul>
 *
 *@param attr          [OUT] Mutex attributes.
 *@param prioceiling   [IN] Upper priority limit in mutex attributes. The priority range is [0, 31].
 *
 *@retval #EINVAL  invalid parameter.
 *@retval #ENOERR  The upper priority limit in mutex attributes is successfully set.
 *@par Dependency:
 *<ul>
 *<li>mutex.h: the header file that contains the API declaration.</li>
 *<li>muttypes.h: the header file that contains the mutex attributes.</li>
 *</ul>
 *@see pthread_mutexattr_getprioceiling
 *@since Huawei LiteOS V100R001C00
 */
int pthread_mutexattr_setprioceiling(pthread_mutexattr_t *attr, int prioceiling);

/**
 *@ingroup mutex
 *@brief Obtain the upper priority limit in mutex attributes.
 *
 *@par Decription:
 *<ul>
 *<li>This API is used to obtain the upper priority limit in mutex attributes.</li>
 *</ul>
 *@attention
 *<ul>
 *<li>None.</li>
 *</ul>
 *
 *@param attr          [IN] Mutex attributes.
 *@param prioceiling   [OUT] Upper priority limit in mutex attributes.
 *
 *@retval #EINVAL  invalid parameter.
 *@retval #ENOERR  The upper priority limit in mutex attributes is successfully obtained.
 *@par Dependency:
 *<ul>
 *<li>mutex.h: the header file that contains the API declaration.</li>
 *<li>muttypes.h: the header file that contains the mutex attributes.</li>
 *</ul>
 *@see pthread_mutexattr_setprioceiling
 *@since Huawei LiteOS V100R001C00
 */
int pthread_mutexattr_getprioceiling(pthread_mutexattr_t *attr, int *prioceiling);

/**
*@ingroup mutex
*@brief Set the upper priority limit of a mutex.
*
*@par Description:
*<ul>
*<li>This API is used to set the upper priority limit of a mutex. The difference between this API and pthread_mutexattr_setprioceiling is that the mutex might be in use when this API is called.</li>
*</ul>
*@attention
*<ul>
*<li>The default priority is the lowest priority. If the upper priority limit is below the default priority, priority inversion will occur.</li>
*</ul>
*
*@param mutex [IN] Mutex.
*@param prioceiling [OUT] New upper priority limit of the mutex. The priority range is [0, 31].
*@param old_ceiling [OUT] Original upper priority limit of the mutex.
*
*@retval #ENOERR The upper priority limit of the mutex is successfully set.
*@retval #EINVAL Invalid parameter.
*@retval #EPERM The operation is not allowed during an interrupt.
*@retval #EDEADLK Relocking a mutex causes a deadlock.
*
*@par Dependency:
*<ul>
*<li>mutex.h: the header file that contains the API declaration.</li>
*<li>muttypes.h: the header file that contains the mutex attributes.</li>
*</ul>
*@see pthread_mutex_getprioceiling
*@since Huawei LiteOS V100R001C00
*/
int pthread_mutex_setprioceiling(pthread_mutex_t *mutex, int prioceiling, int *old_ceiling);

/**
 *@ingroup mutex
 *@brief Obtain the upper priority limit of a mutex.
 *
 *@par Description:
 *<ul>
 *<li>This API is used to obtain the upper priority limit of a mutex.</li>
 *</ul>
 *@attention
 *<ul>
 *<li>None.</li>
 *</ul>
 *
 *@param mutex         [IN] Mutex.
 *@param prioceiling   [OUT] Upper priority limit of the mutex.
 *
 *@retval #EINVAL  invalid parameter.
 *@retval #ENOERR  The upper priority limit of the mutex is successfully obtained.
 *@par Dependency:
 *<ul>
 *<li>mutex.h: the header file that contains the API declaration.</li>
 *<li>muttypes.h: the header file that contains the mutex attributes.</li>
 *</ul>
 *@see pthread_mutex_setprioceiling
 *@since Huawei LiteOS V100R001C00
 */
int pthread_mutex_getprioceiling(pthread_mutex_t *mutex, int *prioceiling);


#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif
