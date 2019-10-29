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

#include "sys/types.h"
#include "codes.h"
#include "unistd.h"
#include "limits.h"
#include "string.h"
#include "stdio.h"
#include "mqueue.h"
#include "semaphore.h"
#include "utsname.h"

// -------------------------------------------------------------------------
// Supply some suitable values for constants that may not be present
// in all configurations.
#define SC_ENABLE                       1
#define SC_DISABLE                     -1

#define CONF_CASE(_name,_val) \
    case (_name): \
        return (_val)

int uname(struct utsname *name)
{
    int ret = 0;

    strncpy(name->sysname, HW_LITEOS_SYSNAME, sizeof(name->sysname));
    strncpy(name->nodename, "hisilicon", sizeof(name->nodename));
    ret = snprintf(name->release, sizeof(name->release), "%s", VER);
    if (ret < 0) {
        return -1;
    }
    ret = snprintf(name->version, sizeof(name->version), "%s %s %s", HW_LITEOS_OPEN_VERSION, __DATE__, __TIME__);
    if (ret < 0) {
        return -1;
    }
    strncpy(name->machine, "", sizeof(name->machine));

    return 0;
}

long sysconf(int name)
{
    switch (name)
    {
        CONF_CASE(_SC_AIO_LISTIO_MAX,                    SC_DISABLE);
        CONF_CASE(_SC_AIO_MAX,                           SC_DISABLE);
        CONF_CASE(_SC_AIO_PRIO_DELTA_MAX,                SC_DISABLE);
        CONF_CASE(_SC_ARG_MAX,                           ARG_MAX);
        CONF_CASE(_SC_ASYNCHRONOUS_IO,                   SC_DISABLE);
        CONF_CASE(_SC_CHILD_MAX,                         CHILD_MAX);
        CONF_CASE(_SC_CLK_TCK,                           SYS_CLK_TCK);
        CONF_CASE(_SC_DELAYTIMER_MAX,                    DELAYTIMER_MAX);
        CONF_CASE(_SC_FSYNC,                             SC_DISABLE);
        CONF_CASE(_SC_GETGR_R_SIZE_MAX,                  GETGR_R_SIZE_MAX);
        CONF_CASE(_SC_GETPW_R_SIZE_MAX,                  GETPW_R_SIZE_MAX);
        CONF_CASE(_SC_JOB_CONTROL,                       SC_DISABLE);
        CONF_CASE(_SC_LOGIN_NAME_MAX,                    LOGIN_NAME_MAX);
        CONF_CASE(_SC_MAPPED_FILES,                      SC_DISABLE);
        CONF_CASE(_SC_MEMLOCK,                           SC_DISABLE);
        CONF_CASE(_SC_MEMLOCK_RANGE,                     SC_DISABLE);
        CONF_CASE(_SC_MEMORY_PROTECTION,                 SC_DISABLE);
        CONF_CASE(_SC_MESSAGE_PASSING,                   SC_DISABLE);
#ifdef LOSCFG_BASE_IPC_QUEUE
        CONF_CASE(_SC_MQ_OPEN_MAX,                       MQ_OPEN_MAX);
        CONF_CASE(_SC_MQ_PRIO_MAX,                       MQ_PRIO_MAX);
#endif
        CONF_CASE(_SC_NGROUPS_MAX,                       NGROUPS_MAX);
        CONF_CASE(_SC_OPEN_MAX,                          OPEN_MAX);
        CONF_CASE(_SC_PAGESIZE,                          0x1000);
        CONF_CASE(_SC_PRIORITIZED_IO,                    SC_DISABLE);
        CONF_CASE(_SC_PRIORITY_SCHEDULING,               SC_DISABLE);
        CONF_CASE(_SC_REALTIME_SIGNALS,                  SC_DISABLE);
        CONF_CASE(_SC_RTSIG_MAX,                         RTSIG_MAX);
        CONF_CASE(_SC_SAVED_IDS,                         SC_DISABLE);

#ifdef LOSCFG_BASE_IPC_SEM
        CONF_CASE(_SC_SEMAPHORES,                        SC_ENABLE);
        CONF_CASE(_SC_SEM_NSEMS_MAX,                     SEM_NSEMS_MAX);
        CONF_CASE(_SC_SEM_VALUE_MAX,                     SEM_VALUE_MAX);
#endif

        CONF_CASE(_SC_SHARED_MEMORY_OBJECTS,             SC_DISABLE);
        CONF_CASE(_SC_SIGQUEUE_MAX,                      SIGQUEUE_MAX);
        CONF_CASE(_SC_STREAM_MAX,                        STREAM_MAX);
        CONF_CASE(_SC_SYNCHRONIZED_IO,                   SC_DISABLE);
        CONF_CASE(_SC_THREADS,                           SC_ENABLE);
        CONF_CASE(_SC_THREAD_ATTR_STACKADDR,             SC_ENABLE);
        CONF_CASE(_SC_THREAD_ATTR_STACKSIZE,             PTHREAD_ATTR_STACKSIZE);
        CONF_CASE(_SC_THREAD_DESTRUCTOR_ITERATIONS,      PTHREAD_DESTRUCTOR_ITERATIONS);
        CONF_CASE(_SC_THREAD_KEYS_MAX,                   PTHREAD_KEYS_MAX);
        CONF_CASE(_SC_THREAD_PRIO_INHERIT,               PTHREAD_PRIO_INHERIT);
        CONF_CASE(_SC_THREAD_PRIO_PROTECT,               PTHREAD_PRIO_PROTECT);
        CONF_CASE(_SC_THREAD_PRIORITY_SCHEDULING,        PTHREAD_PRIORITY_SCHEDULING);
        CONF_CASE(_SC_THREAD_PROCESS_SHARED,             PTHREAD_PROCESS_SHARED);
        CONF_CASE(_SC_THREAD_SAFE_FUNCTIONS,             SC_DISABLE);
        CONF_CASE(_SC_THREAD_STACK_MIN,                  PTHREAD_STACK_MIN);
        CONF_CASE(_SC_THREAD_THREADS_MAX,                PTHREAD_THREADS_MAX);
        CONF_CASE(_SC_TIMERS,                            TIMERS);
        CONF_CASE(_SC_TIMER_MAX,                         TIMER_MAX);
        CONF_CASE(_SC_TTY_NAME_MAX,                      TTY_NAME_MAX);
        CONF_CASE(_SC_TZNAME_MAX,                        TZNAME_MAX);
        CONF_CASE(_SC_VERSION,                           POSIX_VERSION);

    default:
        set_errno(EINVAL);
        return -1;
    }
}

//==========================================================================
// Some trivial compatibility functions.
// These are merely present to permit existing code to be ported a little
// more easily, and to provide adequate standards compatibility.

pthread_t getpid(void)
{
    return g_stLosTask.pstRunTask->uwTaskID;
}

