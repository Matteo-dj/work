/****************************************************************************
 * include/sys/select.h
 *
 *   Copyright (C) 2008-2009, 2011 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/
/** @defgroup select Select
 *  @ingroup filesystem
 */
#ifndef __INCLUDE_SYS_SELECT_H
#define __INCLUDE_SYS_SELECT_H

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include "time.h"
#ifdef LOSCFG_FS_VFS
#include "vfs_config.h"
#include "stdint.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#ifdef LOSCFG_FS_VFS
#if CONFIG_NFILE_DESCRIPTORS > 0 || CONFIG_NSOCKET_DESCRIPTORS > 0

/****************************************************************************
 * Pre-Processor Definitions
 ****************************************************************************/

/* Get the total number of descriptors that we will have to support */

#define __SELECT_NDESCRIPTORS (CONFIG_NFILE_DESCRIPTORS + CONFIG_NSOCKET_DESCRIPTORS)

/* We will use a 32-bit bitsets to represent the set of descriptors.  How
 * many uint32_t's do we need to span all descriptors?
 */


#if __SELECT_NDESCRIPTORS <= 32
#  define __SELECT_NUINT32 1
#elif __SELECT_NDESCRIPTORS <= 64
#  define __SELECT_NUINT32 2
#elif __SELECT_NDESCRIPTORS <= 96
#  define __SELECT_NUINT32 3
#elif __SELECT_NDESCRIPTORS <= 128
#  define __SELECT_NUINT32 4
#elif __SELECT_NDESCRIPTORS <= 160
#  define __SELECT_NUINT32 5
#elif __SELECT_NDESCRIPTORS <= 192
#  define __SELECT_NUINT32 6
#elif __SELECT_NDESCRIPTORS <= 224
#  define __SELECT_NUINT32 7
#elif __SELECT_NDESCRIPTORS <= 256
#  define __SELECT_NUINT32 8
#elif __SELECT_NDESCRIPTORS <= 288
#  define __SELECT_NUINT32 9
#elif __SELECT_NDESCRIPTORS <= 320
#  define __SELECT_NUINT32 10
#elif __SELECT_NDESCRIPTORS <= 352
#  define __SELECT_NUINT32 11
#elif __SELECT_NDESCRIPTORS <= 384
#  define __SELECT_NUINT32 12
#elif __SELECT_NDESCRIPTORS <= 416
#  define __SELECT_NUINT32 13
#elif __SELECT_NDESCRIPTORS <= 448
#  define __SELECT_NUINT32 14
#elif __SELECT_NDESCRIPTORS <= 480
#  define __SELECT_NUINT32 15
#elif __SELECT_NDESCRIPTORS <= 512
#  define __SELECT_NUINT32 16
#elif __SELECT_NDESCRIPTORS <= 1024
#  define __SELECT_NUINT32 32
/*#elif __SELECT_NDESCRIPTORS <= 2048
#  define __SELECT_NUINT32 64*/

#else
#  warning "Large fd_set needed"
#endif

/* These macros map a file descripto to an index and bit number */

#define _FD_NDX(fd) ((fd) >> 5)
#define _FD_BIT(fd) ((fd) & 0x1f)

/* Standard helper macros */
#define FD_CLR(fd,set)    (((uint32_t*)(set))[_FD_NDX(fd)] &= ~(1 << _FD_BIT(fd)))
#define FD_SET(fd,set)    (((uint32_t*)(set))[_FD_NDX(fd)] |= (1 << _FD_BIT(fd)))
#define FD_ISSET(fd,set)  ((((uint32_t*)(set))[_FD_NDX(fd)] & (1 << _FD_BIT(fd))) != 0)
#define FD_ZERO(set)      memset_s(set, sizeof(fd_set), 0, sizeof(fd_set))

/****************************************************************************
 * Type Definitions
 ****************************************************************************/

typedef uint32_t fd_set[__SELECT_NUINT32];

#endif /* CONFIG_NFILE_DESCRIPTORS || CONFIG_NSOCKET_DESCRIPTORS */

#else
#ifndef FD_SET
#undef  FD_SETSIZE
/* Make FD_SETSIZE match NUM_SOCKETS in lwipopts.h */
#define FD_SETSIZE    10
#define FD_SET(n, p)  ((p)->fd_bits[(n)/8] |=  (1 << ((n) & 7)))
#define FD_CLR(n, p)  ((p)->fd_bits[(n)/8] &= ~(1 << ((n) & 7)))
#define FD_ISSET(n,p) ((p)->fd_bits[(n)/8] &   (1 << ((n) & 7)))
#define FD_ZERO(p)    (void)memset_s((void*)(p), sizeof(*(p)), 0, sizeof(*(p)))

typedef struct fd_set {
      unsigned char fd_bits [(FD_SETSIZE+7)/8];
    } fd_set;
#endif

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/**
* @ingroup  select
* @brief select() is provided for compatibility and
*   is simply a layer of added logic on top of poll().
*
* @par Description:
*select() is provided for compatibility and
*   is simply a layer of added logic on top of poll().
*
* @attention
* <ul>
* <li>The wait time is a relative time.</li>
* </ul>
*
* @param nfds [IN]	Type #int the maximum fd number (+1) of any descriptors in any of the three sets.
* @param readfds   [IN]  Type #FAR fd_set * the set of descriptors to monitor for read-ready events.
* @param writefds  [IN]  Type #FAR fd_set * the set of descriptors to monitor for write-ready events.
* @param exceptfds [IN]  Type #FAR fd_set * the set of descriptors to monitor for error events.
* @param timeout   [IN]  Type #FAR struct timeval * Return at this time if none of these events of interest occur.
*
*
* @retval #0    Timer expired.
* @retval #int  The number of bits set in the three sets of descriptors.
* @retval #-1   An error occurred (errno will be set appropriately).
*
* @par Dependency:
* <ul><li>select.h: the header file that contains the API declaration.</li></ul>
 * @see none
* @since Huawei LiteOS V100R001C00
*/

extern int select(int nfds, fd_set *readfds, fd_set *writefds,
                  fd_set *exceptfds, struct timeval *timeout);

#endif /* LOSCFG_FS_VFS */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
#endif /* __INCLUDE_SYS_SELECT_H */
