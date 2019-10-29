/* $OpenBSD: limits.h,v 1.6 2005/12/13 00:35:23 millert Exp $ */
/*	$OpenBSD: limits.h,v 1.13 2005/12/31 19:29:38 millert Exp $	*/
/*	$NetBSD: limits.h,v 1.7 1994/10/26 00:56:00 cgd Exp $	*/

/* Copyright (c) 2002 Marc Espie.
 * Copyright (c) 1988 The Regents of the University of California.
 * All rights reserved.
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
 *	@(#)limits.h	5.9 (Berkeley) 4/3/91
 */

/****************************************************************************
 * arch/arm/include/limits.h
 *
 *   Copyright (C) 2007-2009, 2012 Gregory Nutt. All rights reserved.
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
#ifndef __ARCH_ARM_INCLUDE_LIMITS_H
#define __ARCH_ARM_INCLUDE_LIMITS_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include "stdint.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/****************************************************************************
 * Definitions
 ****************************************************************************/

#define NAME_MIN                            5
#define	CHAR_BIT	8		    /* number of bits in a char */

#define	SCHAR_MAX	0x7f		/* max value for a signed char */
#define SCHAR_MIN	(-0x7f-1)	/* min value for a signed char */

#define	UCHAR_MAX	0xffU		/* max value for an unsigned char */
#ifdef __CHAR_UNSIGNED__
# define CHAR_MIN	0		    /* min value for a char */
# define CHAR_MAX	0xff		/* max value for a char */
#else
# define CHAR_MAX	0x7f
# define CHAR_MIN	(-0x7f-1)
#endif

#define	USHRT_MAX	0xffffU		                 /* max value for an unsigned short */
#define	SHRT_MAX	0x7fff		                 /* max value for a short */
#define SHRT_MIN        (-0x7fff-1)             /* min value for a short */

#define	UINT_MAX	0xffffffffU	                /* max value for an unsigned int */
#define	INT_MAX		0x7fffffff	                /* max value for an int */
#define	INT_MIN		(-0x7fffffff-1)	            /* min value for an int */

/* These change on 32-bit and 64-bit platforms */
#define ULONG_MAX	0xffffffffUL	            /* max value for an unsigned long */
#define LONG_MAX	0x7fffffffL	                /* max value for a long */
#define LONG_MIN	(-0x7fffffffL-1)            /* min value for a long */

#define ULLONG_MAX	0xffffffffffffffffULL       /* max value for unsigned long long */
#define LLONG_MAX	0x7fffffffffffffffLL        /* max value for a signed long long */
#define LLONG_MIN	(-0x7fffffffffffffffLL-1)   /* min value for a signed long long */

#define LONG_BIT	32

/* A pointer is 4 bytes */
#define PTR_MIN                             (-PTR_MAX - 1)
#define PTR_MAX                             2147483647
#define UPTR_MAX                            4294967295U

/* New code should use sysconf(_SC_PAGE_SIZE) instead. */
#ifndef PAGE_SIZE
#define PAGE_SIZE 4096
#endif
#ifndef PAGESIZE
#define  PAGESIZE  PAGE_SIZE
#endif


/* Configurable limits required by POSIX ****************************************
 *
 * Required for all implementations:
 *
 *   _POSIX_ARG_MAX        Total length of string arguments
 *   _POSIX_CHILD_MAX      Number of child tasks active
 *   _POSIX_LINK_MAX       The number of links a file can have
 *   _POSIX_MAX_CANON      Number bytes in TTY canonical input queue
 *   _POSIX_MAX_INPUT      Number bytes in TTY canonical input queue
 *   _POSIX_NAME_MAX       Number of bytes in a file or pathname component
 *   _POSIX_NGROUPS_MAX    Number supplementary group IDs
 *   _POSIX_OPEN_MAX       Number of files a task can have open at once
 *   _POSIX_PATH_MAX       Number of bytes in a full pathname (including NULL)
 *   _POSIX_PIPE_BUF       Number of bytes for atomic write into pipe
 *   _POSIX_SSIZE_MAX      Largest filesystem write; also max value of ssize_t
 *   _POSIX_STREAM_MAX     Number of std I/O streams open at once
 *   _POSIX_TZNAME_MAX     Max number of bytes of a timezone name
 *
 * Required for sigqueue
 *
 *   _POSIX_RTSIG_MAX      Difference between SIGRTMIN and SIGRTMAX
 *   _POSIX_SIGQUEUE_MAX   Max number signals a task can queue
 *
 * Required for POSIX timers
 *
 *   _POSIX_DELAYTIMER_MAX Max number timer overruns
 *   _POSIX_TIMER_MAX      Max number of timers per task
 *   _POSIX_CLOCKRES_MIN   Clock resolution in nanoseconds
 *
 * Required for asynchronous I/O
 *
 *   _POSIX_AIO_LISTIO_MAX Max number of AIOs in single listio call
 *   _POSIX_AIO_MAX        Max number of simultaneous AIO operations
 *
 * Required for POSIX message passing
 *
 *   _POSIX_MQ_OPEN_MAX    Max number message queues task may open (mq_open)
 *   _POSIX_MQ_PRIO_MAX    Max message priority (mq_send)
 *
 * Required for POSIX semaphores
 *
 *   _POSIX_SEM_NSEMS_MAX  Max number of open semaphores per task
 *   _POSIX_SEM_VALUE_MAX  Max value a semaphore may have
 */

/* Maximum length of arguments to `execve', including environment.  */
#define _POSIX_ARG_MAX                      4096

/* Maximum simultaneous processes per real user ID.  */
#define _POSIX_CHILD_MAX                    6

#define _POSIX_CLK_TICK                     time((time_t *)NULL)

/* Minimal number of timer expiration overruns.  */
#define _POSIX_DELAYTIMER_MAX               32

#define _POSIX_GETGR_R_SIZE_MAX             0

#define _POSIX_GETPW_R_SIZE_MAX             0

/* Maximum length of login name.  */
#define _POSIX_LOGIN_NAME_MAX               9

/* Maximum number of message queues open for a process.  */
#define _POSIX_MQ_OPEN_MAX                  LOSCFG_BASE_IPC_QUEUE_LIMIT

/* Maximum number of supported message priorities.  */
#define _POSIX_MQ_PRIO_MAX                  MQ_PRIO_MAX

/* Number of simultaneous supplementary group IDs per process.  */
#define _POSIX_NGROUPS_MAX                  0

/* Number of files one process can have open at once.  */
#ifdef LOSCFG_FS_VFS
#define _POSIX_OPEN_MAX                     CONFIG_NFILE_DESCRIPTORS
#else
#define _POSIX_OPEN_MAX                     255
#endif
#define _POSIX_PAGESIZE                     PAGE_SIZE

/* Minimal number of realtime signals reserved for the application.  */
#define _POSIX_RTSIG_MAX                    8

/* Number of semaphores a process can have.  */
#define _POSIX_SEM_NSEMS_MAX                SEM_NSEMS_MAX

/* Maximal value of a semaphore.  */
#define _POSIX_SEM_VALUE_MAX                SEM_VALUE_MAX

/* Number of pending realtime signals.  */
#define _POSIX_SIGQUEUE_MAX                 32

/* Number of streams a process can have open at once.  */
#define _POSIX_STREAM_MAX                   8

/* We support user-defined stack sizes.  */
#define _POSIX_THREAD_ATTR_STACKSIZE        LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE

/* Controlling the iterations of destructors for thread-specific data.  */
#define _POSIX_THREAD_DESTRUCTOR_ITERATIONS 4

/* The number of data keys per process.  */
#define _POSIX_THREAD_KEYS_MAX              128

/* We support priority inheritence.  */
#define _POSIX_PTHREAD_PRIO_INHERIT         PTHREAD_PRIO_INHERIT

/* We support priority protection, though only for non-robust
     mutexes.  */
#define _POSIX_PTHREAD_PRIO_PROTECT         PTHREAD_PRIO_PROTECT

/* We provide priority scheduling for threads.  */
#define _POSIX_THREAD_PRIORITY_SCHEDULING   SCHED_OTHER

/* Thread process-shared synchronization is supported.  */
#define _POSIX_PTHREAD_PROCESS_SHARED       PTHREAD_PROCESS_SHARED

/* Minimum size for a thread.  We are free to choose a reasonable value.  */
#define _POSIX_THREAD_STACK_MIN             LOS_TASK_MIN_STACK_SIZE

/* The number of threads per process.  */
#define _POSIX_THREAD_THREADS_MAX           LOSCFG_BASE_CORE_TSK_LIMIT + 1

/* We have POSIX timers.  */
#define _POSIX_TIMERS                       LOSCFG_BASE_CORE_SWTMR

/* Maximum number of characters in a tty name.  */
#define _POSIX_TTY_NAME_MAX                 9

/* Maximum length of a timezone name (element of `tzname').  */
#define _POSIX_TZNAME_MAX                   TZ_PATH_LENTGH

/* POSIX Standard approved as ISO/IEC 9945-1 as of December 2001.  */
#define _POSIX_VERSION                      199506L

/* Maximum link count of a file.  */
#define _POSIX_LINK_MAX                     8

/* Number of bytes in a terminal canonical input queue.  */
#define _POSIX_MAX_CANON                    255

/* Number of bytes for which space will be
   available in a terminal input queue.  */
#define _POSIX_MAX_INPUT                    255

/* Number of bytes in a filename.  */
#define _POSIX_NAME_MAX                     255

/* Number of bytes in a pathname.  */
#define _POSIX_PATH_MAX                     260

/* Number of bytes than can be written atomically to a pipe.  */
#define _POSIX_PIPE_BUF                     512

/* Largest value of a `ssize_t'.  */
#define _POSIX_SSIZE_MAX                    32767

/* Maximum clock resolution in nanoseconds.  */
#define _POSIX_CLOCKRES_MIN                 20000000

#define _POSIX_SSIZE_MIN                    -32768

#define ARG_MAX                             _POSIX_ARG_MAX
#define CHILD_MAX                           _POSIX_CHILD_MAX
#define SYS_CLK_TCK                         _POSIX_CLK_TICK
#define DELAYTIMER_MAX                      _POSIX_DELAYTIMER_MAX
#define GETGR_R_SIZE_MAX                    _POSIX_GETGR_R_SIZE_MAX
#define GETPW_R_SIZE_MAX                    _POSIX_GETPW_R_SIZE_MAX
#define LOGIN_NAME_MAX                      _POSIX_LOGIN_NAME_MAX
#define MQ_OPEN_MAX                         _POSIX_MQ_OPEN_MAX
#define NGROUPS_MAX                         _POSIX_NGROUPS_MAX
#define OPEN_MAX                            _POSIX_OPEN_MAX
#define RTSIG_MAX                           _POSIX_RTSIG_MAX
#define SIGQUEUE_MAX                        _POSIX_SIGQUEUE_MAX
#define STREAM_MAX                          _POSIX_STREAM_MAX
#define PTHREAD_ATTR_STACKSIZE              _POSIX_THREAD_ATTR_STACKSIZE
#define PTHREAD_DESTRUCTOR_ITERATIONS       _POSIX_THREAD_DESTRUCTOR_ITERATIONS
#define PTHREAD_KEYS_MAX                    _POSIX_THREAD_KEYS_MAX
#define PTHREAD_PRIORITY_SCHEDULING         _POSIX_THREAD_PRIORITY_SCHEDULING
#define PTHREAD_STACK_MIN                   _POSIX_THREAD_STACK_MIN
#define PTHREAD_THREADS_MAX                 _POSIX_THREAD_THREADS_MAX
#define TIMERS                              _POSIX_TIMERS
#define TIMER_MAX                           _POSIX_TIMER_MAX
#define TTY_NAME_MAX                        _POSIX_TTY_NAME_MAX
#define TZNAME_MAX                          _POSIX_TZNAME_MAX
#define POSIX_VERSION                       _POSIX_VERSION


#define PTHREAD_STACK_OVERHEAD              (0x180 + (PTHREAD_KEYS_MAX * sizeof(void *)))
#define MAX_CANON                           _POSIX_MAX_CANON
#define MAX_INPUT                           _POSIX_MAX_INPUT
#define PIPE_BUF                            _POSIX_PIPE_BUF


#define SSIZE_MAX                           LONG_MAX
#define LINK_MAX                            _POSIX_LINK_MAX
#define NAME_MAX                            _POSIX_NAME_MAX
#define PATH_MAX                            _POSIX_PATH_MAX

#define CONFIG_NAME_MAX                     _POSIX_NAME_MAX
#define CONFIG_PATH_MAX                     _POSIX_PATH_MAX


#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* __ARCH_ARM_INCLUDE_LIMITS_H */
