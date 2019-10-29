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

/**@defgroup muttypes Mutex attributes
 * @ingroup posix
 */

#ifndef _HWLITEOS_POSIX_MUTTYPES_H
#define _HWLITEOS_POSIX_MUTTYPES_H

#include "los_mux.h"

#include "los_task.ph"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */


/**
 * @ingroup muttypes
 * Define the default mutex protocol as POSIX_MUTEX_DEFAULT_INHERIT.
 */
#ifndef POSIX_MUTEX_DEFAULT_INHERIT
#define POSIX_MUTEX_DEFAULT_INHERIT
#endif

/**
 * @ingroup muttypes
 * Define the default mutex protocol as POSIX_MUTEX_DEFAULT_PROTECT.
 */

/**
 * @ingroup muttypes
 * Mutex types
 */
enum {
    PTHREAD_MUTEX_NORMAL = 0,                                /**< Normal mutex*/
    PTHREAD_MUTEX_RECURSIVE = 1,                             /**< Recursive mutex*/
    PTHREAD_MUTEX_ERRORCHECK = 2,                            /**< Error checking mutex*/

    PTHREAD_MUTEX_ERRORCHECK_NP = PTHREAD_MUTEX_ERRORCHECK,  /**< Error checking mutex*/
    PTHREAD_MUTEX_RECURSIVE_NP  = PTHREAD_MUTEX_RECURSIVE,   /**< Recursive mutex*/

    PTHREAD_MUTEX_DEFAULT = PTHREAD_MUTEX_NORMAL             /**< Default mutex*/
};

/**
 * @ingroup muttypes
 * Mutex attributes object
 */
typedef struct
{
    UINT8 protocol;     /**< Mutex protocol. The value range is [0, 2].*/
    UINT8 prioceiling;  /**< Upper priority limit of a mutex. The value range is [0, 31].*/
    UINT8 type;         /**< Mutex type. The value range is [0, 2].*/
    UINT8 reserved;     /**< Reserved.*/
} pthread_mutexattr_t;

/**
 * @ingroup muttypes
 * Mutex operation object
 */
typedef struct
{
    LOS_DL_LIST     stMuxList;      /**< Mutex blocking linked list*/
    LOS_TASK_CB     *pstOwner;      /**< Task that involves the mutex*/
    UINT16          usMuxCount;     /**< Mutex counter value*/
} MUX_CB_P;

/**
 * @ingroup muttypes
 * Mutex object
 */
typedef struct pthread_mutex
{
    pthread_mutexattr_t stAttr; /**< Mutex attributes object*/
    MUX_CB_P stLock;            /**< Mutex operation object*/
} pthread_mutex_t;


/**
 * @ingroup muttypes
 *Mutex protocol
 */
enum
{
    PTHREAD_PRIO_NONE = 0,  /**< No protocol*/
    PTHREAD_PRIO_INHERIT,   /**< Priority inheritance protocol*/
    PTHREAD_PRIO_PROTECT   /**< Priority protection protocol*/
};


#if defined POSIX_MUTEX_DEFAULT_INHERIT
/**
 * @ingroup muttypes
 * Statically initialize a recursive mutex that applies the priority inheritance protocol
 */
#define PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP \
    {{PTHREAD_PRIO_INHERIT, OS_TASK_PRIORITY_LOWEST, PTHREAD_MUTEX_RECURSIVE_NP, 0} , \
    {{(struct LOS_DL_LIST *)NULL, (struct LOS_DL_LIST *)NULL}, \
    (LOS_TASK_CB *)NULL, 0}}
/**
 * @ingroup muttypes
 * Statically initialize a normal mutex that applies the priority inheritance protocol
 */
#define PTHREAD_MUTEX_INITIALIZER \
    {{PTHREAD_PRIO_INHERIT, OS_TASK_PRIORITY_LOWEST, 0, 0} , \
    {{(struct LOS_DL_LIST *)NULL, (struct LOS_DL_LIST *)NULL}, \
    (LOS_TASK_CB *)NULL, 0}}
#elif defined POSIX_MUTEX_DEFAULT_PROTECT
/**
 * @ingroup muttypes
 * Statically initialize a recursive mutex that applies the priority protection protocol
 */
#define PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP \
{{PTHREAD_PRIO_PROTECT, OS_TASK_PRIORITY_LOWEST, PTHREAD_MUTEX_RECURSIVE_NP, 0} , \
    {{(struct LOS_DL_LIST *)NULL, (struct LOS_DL_LIST *)NULL}, \
    (LOS_TASK_CB *)NULL, 0}}
/**
 * @ingroup muttypes
 * Statically initialize a normal mutex that applies the priority protection protocol
 */
#define PTHREAD_MUTEX_INITIALIZER {{PTHREAD_PRIO_PROTECT, OS_TASK_PRIORITY_LOWEST, 0, 0} ,  \
    {{(struct LOS_DL_LIST *)NULL, (struct LOS_DL_LIST *)NULL}, \
    (LOS_TASK_CB *)NULL, 0}}
#else
/**
 * @ingroup muttypes
 * Statically initialize a recursive mutex that applies no priority protocol
 */
#define PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP \
{{PTHREAD_PRIO_NONE, OS_TASK_PRIORITY_LOWEST, PTHREAD_MUTEX_RECURSIVE_NP, 0} , \
    {{(struct LOS_DL_LIST *)NULL, (struct LOS_DL_LIST *)NULL}, \
    (LOS_TASK_CB *)NULL, 0}}
/**
 * @ingroup muttypes
 * Statically initialize a normal mutex that applies no priority protocol
 */
#define PTHREAD_MUTEX_INITIALIZER {{PTHREAD_PRIO_NONE, OS_TASK_PRIORITY_LOWEST, 0, 0} , \
    {{(struct LOS_DL_LIST *)NULL, (struct LOS_DL_LIST *)NULL}, \
    (LOS_TASK_CB *)NULL, 0}}
#endif


#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /*_PTHREAD_MUTTYPES_H */
