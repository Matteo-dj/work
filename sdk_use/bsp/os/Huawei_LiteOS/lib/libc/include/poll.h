/****************************************************************************
 * include/poll.h
 *
 *   Copyright (C) 2008-2009 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *   Copyright (c) <2014-2015>, <Huawei Technologies Co., Ltd>
 *   All rights reserved.
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
/*----------------------------------------------------------------------------
 * Notice of Export Control Law
 * ===============================================
 * Huawei LiteOS may be subject to applicable export control laws and regulations, which might
 * include those applicable to Huawei LiteOS of U.S. and the country in which you are located.
 * Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance with such
 * applicable export control laws and regulations.
 *---------------------------------------------------------------------------*/

/** @defgroup poll Poll
 *  @ingroup filesystem
 */

#ifndef __INCLUDE_POLL_H
#define __INCLUDE_POLL_H

/****************************************************************************
 * Included Files
 ****************************************************************************/


#include "stdint.h"
#include "semaphore.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */


#define FAR
/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Poll event definitions:
 *
 *   POLLIN
 *     Data other than high-priority data may be read without blocking.
 *   POLLRDNORM
 *     Normal data may be read without blocking.
 *   POLLRDBAND
 *     Priority data may be read without blocking.
 *   POLLPRI
 *     High priority data may be read without blocking.
 *
 *   POLLOUT
 *     Normal data may be written without blocking.
 *   POLLWRNORM
 *     Equivalent to POLLOUT.
 *   POLLWRBAND
 *     Priority data may be written.
 *
 *   POLLERR
 *     An error has occurred (revents only).
 *   POLLHUP
 *     Device has been disconnected (revents only).
 *   POLLNVAL
 *     Invalid fd member (revents only).
 */

#define POLLIN       (0x001)    /* There is data to read.  */
#define POLLPRI      (0x002)    /* There is urgent data to read.  */
#define POLLOUT      (0x004)    /* Writing now will not block.  */

#define POLLRDNORM   (0x040)    /* Normal data may be read.  */
#define POLLRDBAND   (0x080)    /* Priority data may be read.  */
#define POLLWRNORM   (0x100)    /* Writing now will not block.  */
#define POLLWRBAND   (0x200)    /* Priority data may be written.  */

#define POLLERR      (0x008)    /* Error condition.  */
#define POLLHUP      (0x010)    /* Hung up.  */
#define POLLNVAL     (0x020)    /* Invalid polling request.  */

/****************************************************************************
 * Public Type Definitions
 ****************************************************************************/

/* The number of poll descriptors (required by poll() specification */

typedef unsigned long int nfds_t;

/* poll events.  */
typedef short int pollevent_t;

/* This is the Nuttx variant of the standard pollfd structure. */

struct pollfd
{
  int         fd;       /* The descriptor being polled */
  sem_t      *sem;      /* Pointer to semaphore used to post output event */
  pollevent_t events;   /* The input event flags */
  pollevent_t revents;  /* The output event flags */
  FAR void   *priv;     /* For use by drivers */
};

/****************************************************************************
 * Public Variables
 ****************************************************************************/

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/
struct tag_poll_wait_entry;

typedef struct tag_poll_wait_entry * poll_wait_head;

typedef struct tag_poll_table
{
    poll_wait_head wait;
} poll_table;


/**
* @ingroup  poll
* @brief The current file pointer is hung from the device's internal definition queue
*
*
* @par Description:
*   poll() waits for one of a set of file descriptors to become ready to
*   perform I/O.  If none of the events requested (and no error) has
*   occurred for any of  the  file  descriptors,  then  poll() blocks until
*   one of the events occurs.
*
*
* @attention
* <ul>
* <li>The wait time is a relative time.</li>
* </ul>
*
* @param fds [IN]   Type #FAR struct pollfd *  List of structures describing file descriptors to be monitored.
* @param nfds [IN]  Type #nfds_t The number of entries in the list.
* @param timeout [IN]  Type #int Specifies an upper limit on the time for which poll will
*     block in milliseconds.  A negative value of timeout means an infinite
*     timeout.
*
*
* @retval #0    Success.
* @retval #-1   Failed and errno is set appropriately.
*
* @par Dependency:
* <ul><li>poll.h: the header file that contains the API declaration.</li></ul>
* @see
* @since Huawei LiteOS V100R001C00
*/
extern int poll(FAR struct pollfd *fds, nfds_t nfds, int timeout);

struct wait_queue_head;

typedef struct wait_queue_head wait_queue_head_t;

/**
* @ingroup  poll
* @brief wake up the task in the waiting queue
*
*
* @par Description:
* This API is used to wake up the task in the waiting queue.
*
*
* @attention
* <ul>
* <li>parameter wait_address should be non-null pointer.</li>
* </ul>
*
* @param wait_address [IN]   Type #wait_queue_head_t *  waiting queue.
*
*
*
* @par Dependency:
* <ul><li>poll.h: the header file that contains the API declaration.</li></ul>
* @see
* @since Huawei LiteOS V100R001C00
*/
extern void notify_poll(wait_queue_head_t *wait_address);


#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* __INCLUDE_POLL_H */
