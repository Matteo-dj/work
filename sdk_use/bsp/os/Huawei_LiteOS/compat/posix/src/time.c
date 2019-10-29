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


#include "asm/hal_platform_ints.h"
#include "time.h"
#include "unistd.h"
#include "fcntl.h"
#include "stdio.h"
#include "string.h"
#include "stdint.h"
#include "los_swtmr.ph"
#include "los_sys.ph"
#include "los_tick.ph"

extern unsigned int arch_timer_rollback(void);
extern unsigned long long hi_sched_clock(void);
// Do a time package defined return. This requires the error code
// to be placed in errno, and if it is non-zero, -1 returned as the
// result of the function. This also gives us a place to put any
// generic tidyup handling needed for things like signal delivery and
// cancellation.
#define TIME_RETURN(err)    \
    do {    \
        int __retval = 0;   \
        if (err != 0) {    \
            __retval = -1; \
            errno = err;   \
        }                   \
        return __retval;    \
    } while (0) //lint -e506

#define NANO_SECONDS_PER_TICK   (1000000000 / LOSCFG_BASE_CORE_TICK_PER_SECOND)

//==========================================================================
// internal functions

static inline BOOL valid_timespec(const struct timespec *tp)
{
    /* Fail a NULL pointer */
    if (tp == NULL) {
        return FALSE;
    }

    /* Fail illegal nanosecond values */
    if (tp->tv_nsec < 0 || tp->tv_nsec > 1000000000) {
        return FALSE;
    }

    return TRUE;
}

//==========================================================================
// Clock functions

unsigned long long localseconds = 0;
unsigned long long settimeseconds = 0;

int clock_settime(clockid_t clock_id, const struct timespec *tp)
{
    long now_sec = 0;
    unsigned int intr_Save;
    
    if (clock_id != CLOCK_REALTIME) {
        TIME_RETURN(EINVAL);
    }

    if (!valid_timespec(tp)) {
        TIME_RETURN(EINVAL);
    }

    localseconds = tp->tv_sec;
    intr_Save = LOS_IntLock();
    now_sec = LOS_TickCountGet() / LOSCFG_BASE_CORE_TICK_PER_SECOND;
    LOS_IntRestore(intr_Save);    
    settimeseconds = now_sec;

    TIME_RETURN(0);
}

static void get_monotonic_time(struct timeval* tv)
{
    unsigned int timer = 0;
    unsigned int usecs;
    unsigned int intr_Save;
    
    intr_Save = LOS_IntLock();
    tv->tv_sec = LOS_TickCountGet() / LOSCFG_BASE_CORE_TICK_PER_SECOND;
    tv->tv_usec = LOS_TickCountGet() % LOSCFG_BASE_CORE_TICK_PER_SECOND * (MS_PER_TICK * 1000); /*lint !e647*/

    LOS_IntRestore(intr_Save);
    if (arch_timer_rollback()){
		tv->tv_usec += MS_PER_TICK * 1000;
    }
	
    READ_UINT32(timer, TIMER_TICK_REG_BASE + TIMER_VALUE);
    usecs = ((CYCLES_PER_TICK - timer)) * (MS_PER_TICK * 1000) / CYCLES_PER_TICK;
    tv->tv_usec += usecs;  
    
    if (tv->tv_usec >= 1000000){
       tv->tv_sec++;
       tv->tv_usec -= 1000000;
    }    
}

int gettimeofday(struct timeval* tv, struct timezone* tz)
{
    struct timeval time_mono;
    
    if (tv == NULL)
        TIME_RETURN(EINVAL);

    get_monotonic_time(&time_mono);

    tv->tv_sec = localseconds + time_mono.tv_sec - settimeseconds;
    tv->tv_usec = time_mono.tv_usec;

    return 0;
}

//-----------------------------------------------------------------------------
// Get the clocks current time
int clock_gettime(clockid_t clock_id, struct timespec *tp)
{
    struct timeval tv;


    if (clock_id > CLOCK_MONOTONIC_RAW || clock_id < CLOCK_REALTIME){
            goto errout;
    }

    if (tp == NULL) {
            goto errout;
    }

    if (clock_id == CLOCK_MONOTONIC){
        get_monotonic_time(&tv);
    }
    else{
        if (gettimeofday(&tv, (struct timezone*)0)) {
            goto errout;
        }
    }

    tp->tv_sec = tv.tv_sec;
    tp->tv_nsec = tv.tv_usec * 1000;
    return 0;
errout:
    errno = EINVAL;
    return -1;
}

//-----------------------------------------------------------------------------
// Get the clocks resolution
int clock_getres(clockid_t clock_id, struct timespec *tp)
{
    if (clock_id != CLOCK_REALTIME) {
        TIME_RETURN(EINVAL);
    }

    if (tp == NULL) {
        TIME_RETURN(EINVAL);
    }

    /* the accessable rtc resolution */
    tp->tv_nsec = 1000; /* the precision of clock_gettime is 1us */
    tp->tv_sec = 0;

    TIME_RETURN(0);
}

//==========================================================================
// Timer functions

//-----------------------------------------------------------------------------
// Create a timer based on the given clock.
int timer_create(clockid_t clock_id, struct sigevent *evp, timer_t *timer_id)
{
    SWTMR_CTRL_S *pstSwtmr = (SWTMR_CTRL_S *)NULL;
    UINT32 uwRet;
    UINT16 usSwTmrID;

    if ((clock_id != CLOCK_REALTIME) || (timer_id == NULL || evp == NULL)) {
        errno = EINVAL;
        return -1;
    }

    if (evp->sigev_notify != SIGEV_THREAD) {
        errno = EAGAIN;
        return -1;
    }

    uwRet = LOS_SwtmrCreate(1, LOS_SWTMR_MODE_PERIOD,
            (SWTMR_PROC_FUNC)evp->sigev_notify_function,
            &usSwTmrID, (UINT32)evp->sigev_value.sival_int);

    if (uwRet != LOS_OK) {
        errno = EAGAIN;
        return -1;
    }

    pstSwtmr = OS_SWT_FROM_SID(usSwTmrID);

    *timer_id = pstSwtmr;

    return 0;
}

//-----------------------------------------------------------------------------
// Delete the timer
int timer_delete(timer_t timerid)
{
    SWTMR_CTRL_S *pstSwtmr;

    if (timerid == NULL) {
        goto errout;
    }

    pstSwtmr = (SWTMR_CTRL_S *)timerid;

    if (LOS_SwtmrDelete(pstSwtmr->usTimerID)) {
         goto errout;
    }

    return 0;

errout:
    errno = EINVAL;
    return -1;
}

//-----------------------------------------------------------------------------
// Set the expiration time of the timer.
int timer_settime(timer_t timerid, int flags,
        const struct itimerspec *value, /* new value */
        struct itimerspec *ovalue) /* old value to return, always 0 */
{
    SWTMR_CTRL_S *pstSwtmr;
    UINT32 uwInterval, uwExpiry, millisec, uwRet;
    UINT64 tmp;

    if (value == NULL || timerid == NULL) {
        errno = EINVAL;
        return -1;
    }

    if (value->it_value.tv_nsec >= 1000000000 ||
            value->it_interval.tv_nsec >= 1000000000 ||
            value->it_value.tv_nsec < 0 ||
            value->it_value.tv_sec < 0 ||
            value->it_interval.tv_nsec < 0 ||
            value->it_interval.tv_sec < 0) {
        errno = EINVAL;
        return -1;
    }

    pstSwtmr = (SWTMR_CTRL_S *)timerid;
    if (OS_SWTMR_STATUS_TICKING == pstSwtmr->ucState) {
        uwRet = LOS_SwtmrStop(pstSwtmr->usTimerID);
        if (LOS_OK != uwRet) {
            errno = EINVAL;
            return -1;
        }
    }

    tmp = value->it_value.tv_nsec / 1000000;
    if ((value->it_value.tv_nsec < (1000000 * (1000 / LOSCFG_BASE_CORE_TICK_PER_SECOND)))
            && (value->it_value.tv_nsec) != 0) {
        tmp = 1000 / LOSCFG_BASE_CORE_TICK_PER_SECOND;
    }

    millisec = (UINT32)tmp;
    millisec += value->it_value.tv_sec * 1000;
    uwExpiry = LOS_MS2Tick(millisec);

    tmp = value->it_interval.tv_nsec / 1000000;
    if ((value->it_interval.tv_nsec < (1000000 * (1000 / LOSCFG_BASE_CORE_TICK_PER_SECOND))) &&
            (value->it_interval.tv_nsec) != 0) {
        tmp = 1000 / LOSCFG_BASE_CORE_TICK_PER_SECOND;
    }
    millisec = (UINT32)tmp;
    millisec += value->it_interval.tv_sec * 1000;
    uwInterval = LOS_MS2Tick(millisec);

    if (uwExpiry == 0) {
     /*
    1) when uwExpiry is 0, means timer should be stopped.
    2) If timer is ticking, stopping timer is already done before.
    2) If timer is created but not ticking, return 0 as well.
    */
    return 0;
    } else {
        if (uwInterval == 0) {
            pstSwtmr->ucMode = LOS_SWTMR_MODE_ONCE;
        } else {
            pstSwtmr->ucMode = LOS_SWTMR_MODE_OPP;
        }
    }

    pstSwtmr->uwExpiry = uwExpiry;
    pstSwtmr->uwInterval = uwInterval;
    pstSwtmr->ucOverrun = 0;
    if (LOS_SwtmrStart(pstSwtmr->usTimerID)) {
        errno = EINVAL;
        return -1;
    }

    return 0;
}

//-----------------------------------------------------------------------------
// Get current timer values
int timer_gettime(timer_t timerid, struct itimerspec *value)
{
    UINT32 uwTick = 0;
    UINT32 uwMillisec = 0;
    UINT32 temp = 0;

    /* expire time */
    if (value == NULL || timerid == NULL ||
        (LOS_OK != LOS_SwtmrTimeGet(timerid->usTimerID, &uwTick))) {
        errno = EINVAL;
        return -1;
    }

    uwMillisec = LOS_Tick2MS(uwTick);
    temp = uwMillisec / 1000;
    value->it_value.tv_sec = temp;
    value->it_value.tv_nsec = (uwMillisec - temp * 1000) * 1000000;

    /* interval time */
    uwTick = timerid->uwInterval;
    uwMillisec = LOS_Tick2MS(uwTick);
    temp = uwMillisec / 1000;
    value->it_interval.tv_sec = temp;
    value->it_interval.tv_nsec = (uwMillisec - temp * 1000) * 1000000;

    return 0;
}

//-----------------------------------------------------------------------------
// Get number of missed triggers
int timer_getoverrun(timer_t timerid)
{
    if ((SWTMR_CTRL_S *)timerid == NULL) {
        errno = EINVAL;
        return -1;
    }

    if (timerid->usTimerID >= OS_SWTMR_MAX_TIMERID ) {
        errno = EINVAL;
        return -1;
    }

    if ((timerid->ucOverrun) >= (unsigned char)DELAYTIMER_MAX) {
        return (int)DELAYTIMER_MAX;
    } else {
        return (int)(timerid->ucOverrun);
    }
}

static int __sleep(unsigned int useconds)
{
    UINT32 uwInterval = 0;
    UINT32 uwRet = 0;

    if (OS_INT_ACTIVE) {
        return LOS_ERRNO_SWTMR_HWI_ACTIVE;
    }

    if (useconds == 0) {
        uwInterval = 0;
    } else {
        uwInterval = LOS_MS2Tick(useconds);
        if (uwInterval == 0) {
             uwInterval = 1;
        }
    }

    uwRet = LOS_TaskDelay(uwInterval);

    if (uwRet == LOS_OK) {
        return uwRet;
    } else {
        return -1;
    }
}

int usleep(unsigned useconds)
{
    unsigned int uwInterval;

    /* the values not less than per Millisecond */
    if (useconds < OS_SYS_MS_PER_SECOND) {
        if(useconds != 0) {
            uwInterval = OS_SYS_MS_PER_SECOND/LOSCFG_BASE_CORE_TICK_PER_SECOND;
        } else {
            uwInterval = 0;
        }
        return __sleep(uwInterval);
    }

    return __sleep(useconds / 1000);
}

int nanosleep(const struct timespec *rqtp, struct timespec *rmtp)
{
    unsigned int useconds;
    int uwRet = -1;
    int uwMax_tv_sec = 0;

    /* expire time */
    if (rqtp == NULL)
    {
        errno = EINVAL;
        return uwRet;
    }

    uwMax_tv_sec = (int) ((UINT32_MAX - INT32_MAX / 1000) /1000000);
    if (rqtp->tv_nsec >= 1000000000 || rqtp->tv_nsec < 0 || rqtp->tv_sec < 0 || rqtp->tv_sec > uwMax_tv_sec)
    {
        errno = EINVAL;
        return uwRet;
    }
    useconds = (rqtp->tv_sec * 1000000 + rqtp->tv_nsec / 1000);
    if (useconds == 0 && rqtp->tv_nsec != 0)
    {
        useconds = 1;
    }
    uwRet = usleep(useconds);

    return uwRet;
}

unsigned int sleep(unsigned int seconds)
{
    UINT32 uwInterval = 0;
    UINT32 uwRet = 0;

    if (OS_INT_ACTIVE) {
        printf("Error!!!the function is prohibited to use in the interrupt\n");
        return seconds;
    }

    uwInterval = LOS_MS2Tick(seconds * 1000);
    uwRet = LOS_TaskDelay(uwInterval);

    if (uwRet == LOS_OK) {
        return uwRet;
    } else {
        return seconds;
    }
}

double difftime(time_t time2, time_t time1)
{
    return (double)(time2 - time1);
}

long int timezone;

void settimezone(char *buff)
{
    int num = 0;
    if(NULL == buff)
        return;

    if (buff[3] == '-' || buff[3] == '+')
    {
        if (buff[6] == ':') {
            num = (buff[4] - '0') * 10 + buff[5] - '0';
        }
        if (buff[5] == ':') {
            num = buff[4] - '0';
        }
        if (num < 0 || num > 12) {
            PRINT_ERR("TZ file data error\n");
            return;
        }
        if (buff[3] == '-') {
            timezone = -num * 3600;
        }
        if (buff[3] == '+') {
            timezone = num * 3600;
        }
    }
    else
    {
        PRINT_ERR("TZ file data error\n");
    }

}

int setlocalseconds(int seconds)
{
    unsigned long long now_ns = 0;

    localseconds = seconds;
    now_ns = hi_sched_clock();
    settimeseconds = (unsigned int)(now_ns / 1000000000L);

    return 0;
}

clock_t clock(VOID)
{
    UINT32 temp, temp1;
    clock_t clockvalue = 0;
    double tickcount = 0;
    unsigned int intr_Save;
    unsigned int systick2ms = OS_SYS_MS_PER_SECOND / LOSCFG_BASE_CORE_TICK_PER_SECOND;

    intr_Save = LOS_IntLock();
    clockvalue += g_ullTickCount * systick2ms;

    READ_UINT32(temp, TIMER_TICK_REG_BASE + TIMER_VALUE);
    tickcount = (double)(g_uwSysClock / LOSCFG_BASE_CORE_TICK_PER_SECOND - temp) / (g_uwSysClock / LOSCFG_BASE_CORE_TICK_PER_SECOND);
    clockvalue += (UINT32)(tickcount * systick2ms);

    if (arch_timer_rollback())
    {
        READ_UINT32(temp1, TIMER_TICK_REG_BASE + TIMER_VALUE);
        if (temp1 < temp)
            clockvalue += systick2ms;
    }
    LOS_IntRestore(intr_Save);

    return clockvalue;
}

clock_t times(struct tms *buf)
{
    clock_t clockvalue;

    clockvalue = g_ullTickCount;
    if (buf != NULL) {
        buf->tms_cstime = clockvalue;
        buf->tms_cutime = clockvalue;
        buf->tms_stime  = clockvalue;
        buf->tms_utime  = clockvalue;
    }

    return clockvalue;
}
