/****************************************************************************
 * include/clock.h
 *
 *   Copyright (C) 2007-2009, 2011-2012, 2014 Gregory Nutt. All rights reserved.
 *   Copyright (c) <2014-2015>, <Huawei Technologies Co., Ltd>
 *   All rights reserved.
 *
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
/****************************************************************************
 * Notice of Export Control Law
 * ===============================================
 * Huawei LiteOS may be subject to applicable export control laws and regulations,
 * which might include those applicable to Huawei LiteOS of U.S. and the country in
 * which you are located.
 * Import, export and usage of Huawei LiteOS in any manner by you shall be in
 * compliance with such applicable export control laws and regulations.
 ****************************************************************************/

#ifndef _INCLUDE_CLOCK_H
#define _INCLUDE_CLOCK_H

/****************************************************************************
 * Included Files
 ****************************************************************************/


#include "stdint.h"
#include "time.h"


#include "los_config.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/* Configuration ************************************************************/
/* Efficient, direct access to OS global timer variables will be supported
 * if the execution environment has direct access to kernel global data.
 * The code in this execution context can access the kernel global data
 * directly if:
 *
 * 1. We are not running tick-less (in which case there is no global timer
 *    data),
 * 2. This is an un-protected, non-kernel build,
 * 3. This is a protected build, but this code is being built for execution
 *    within the kernel space.
 * 4. It we are building with SYSCALLs enabled, but not in a kernel build,
 *    then we can't know a priori whether the code has access to the
 *    global variables or not.  In that case we have to assume not.
 */
/* Timing constants *********************************************************/

#define NSEC_PER_SEC          1000000000
#define USEC_PER_SEC             1000000
#define MSEC_PER_SEC                1000
#define DSEC_PER_SEC                  10
#define NSEC_PER_DSEC          100000000
#define USEC_PER_DSEC             100000
#define MSEC_PER_DSEC                100
#define NSEC_PER_MSEC            1000000
#define USEC_PER_MSEC               1000
#define NSEC_PER_USEC               1000

/* If CONFIG_SCHED_TICKLESS is not defined, then the interrupt interval of
 * the system timer is given by USEC_PER_TICK.  This is the expected number
 * of microseconds between calls from the processor-specific logic to
 * sched_process_timer().  The default value of USEC_PER_TICK is 10000
 * microseconds (100KHz).  However, this default setting can be overridden
 * by defining the interval in microseconds as CONFIG_USEC_PER_TICK in the
 * NuttX configuration file.
 *
 * The following calculations are only accurate when (1) there is no
 * truncation involved and (2) the underlying system timer is an even
 * multiple of microseconds.  If (2) is not true, you will probably want
 * to redefine all of the following.
 */
# define USEC_PER_TICK        (USEC_PER_SEC/LOSCFG_BASE_CORE_TICK_PER_SECOND)

/* MSEC_PER_TICK can be very inaccurate if CONFIG_USEC_PER_TICK is not an
 * even multiple of milliseconds.  Calculations using USEC_PER_TICK are
 * preferred for that reason (at the risk of overflow)
 */

#define TICK_PER_DSEC         (USEC_PER_DSEC / USEC_PER_TICK)            /* Truncates! */
#define TICK_PER_SEC          (USEC_PER_SEC  / USEC_PER_TICK)            /* Truncates! */
#define TICK_PER_MSEC         (USEC_PER_MSEC / USEC_PER_TICK)            /* Truncates! */
#define MSEC_PER_TICK         (USEC_PER_TICK / USEC_PER_MSEC)            /* Truncates! */
#define NSEC_PER_TICK         (USEC_PER_TICK * NSEC_PER_USEC)            /* Exact */

/**
 * @ingroup libc
 * Convert time value from nanoseconds to ticks.
 */
#define NSEC2TICK(nsec)       (((nsec)+(NSEC_PER_TICK/2))/NSEC_PER_TICK) /* Rounds */

/**
 * @ingroup libc
 * Convert time value from microseconds to ticks.
 */
#define USEC2TICK(usec)       (((usec)+(USEC_PER_TICK/2))/USEC_PER_TICK) /* Rounds */

/**
 * @ingroup libc
 * Convert time value from milliseconds to ticks.
 */
#if (MSEC_PER_TICK * USEC_PER_MSEC) == USEC_PER_TICK
#define MSEC2TICK(msec)     (((msec)+(MSEC_PER_TICK/2))/MSEC_PER_TICK) /* Rounds */
#else
#define MSEC2TICK(msec)     USEC2TICK(msec * 1000)                     /* Rounds */
#endif

/**
 * @ingroup libc
 * Convert time value from deciseconds to ticks.
 */
#define DSEC2TICK(dsec)       MSEC2TICK((dsec) * MSEC_PER_DSEC)          /* Rounds */

/**
 * @ingroup libc
 * Convert time value from seconds to ticks.
 */
#define SEC2TICK(sec)         MSEC2TICK((sec)  * MSEC_PER_SEC)           /* Rounds */

/**
 * @ingroup libc
 * Convert time value from ticks to nanoseconds.
 */
#define TICK2NSEC(tick)       ((tick) * NSEC_PER_TICK)                   /* Exact */

/**
 * @ingroup libc
 * Convert time value from ticks to microseconds.
 */
#define TICK2USEC(tick)       ((tick) * USEC_PER_TICK)                   /* Exact */

/**
 * @ingroup libc
 * Convert time value from ticks to milliseconds.
 */
#if (MSEC_PER_TICK * USEC_PER_MSEC) == USEC_PER_TICK
#define TICK2MSEC(tick)     ((tick)*MSEC_PER_TICK)                     /* Exact */
#else
#define TICK2MSEC(tick)     (((tick)*USEC_PER_TICK)/USEC_PER_MSEC)     /* Rounds */
#endif

/**
 * @ingroup libc
 * Convert time value from ticks to deciseconds.
 */
#define TICK2DSEC(tick)       (((tick)+(TICK_PER_DSEC/2))/TICK_PER_DSEC) /* Rounds */

/**
 * @ingroup libc
 * Convert time value from ticks to seconds.
 */
#define TICK2SEC(tick)        (((tick)+(TICK_PER_SEC/2))/TICK_PER_SEC)   /* Rounds */

#endif /* _INCLUDE_CLOCK_H */
