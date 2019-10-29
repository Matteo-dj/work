/*
 * Copyright (C) 2008 The Android Open Source Project
 * All rights reserved.
 * Copyright (c) <2014-2015>, <Huawei Technologies Co., Ltd>
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

 /*----------------------------------------------------------------------------
 * Notice of Export Control Law
 * ===============================================
 * Huawei LiteOS may be subject to applicable export control laws and regulations, which might
 * include those applicable to Huawei LiteOS of U.S. and the country in which you are located.
 * Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance with such
 * applicable export control laws and regulations.
 *---------------------------------------------------------------------------*/
/** @defgroup time Time
  * @ingroup posix
  */
#ifndef __TIME_H__
#define __TIME_H__

#include "los_typedef.h"
#include "los_swtmr.h"
#include "signal.h"
#include "float.h"
#include "pthread.h"
#include "clock.h"
#include "sys/types.h"
#include "sys/time.h"
#include "hisoc/timer.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */


#define LOS_POSIX_TIMERS

/**
 *  @ingroup time
 *  The up-limit of number of timer.
 */
#define _POSIX_TIMER_MAX	32

/**
 *  @ingroup time
 *  Maximum length of a timezone name (element of `tzname').
 */
#define TZ_PATH_LENTGH 100

/*---------------------------------------------------------------------------*/
/* Types for timers and clocks */

/**
 *  @ingroup time
 *  Define clockid_t as an integer.
 */
typedef int clockid_t;

/**
 *  @ingroup time
 *  Define time_t as an integer.
 */
typedef int time_t;

/**
 *  @ingroup time
 *  Define clock_t as a long integer.
 */
typedef long clock_t;

/**
 *  @ingroup time
 *  Define timer_t as a pointer to software timer control structure.
 */
typedef struct tagSwTmrCtrl *timer_t;

/**
 *  @ingroup time
 *  Signal struct.
 */
struct sigevent;

/*---------------------------------------------------------------------------*/
/* Structures */

/**
 *  @ingroup time
 *  Time struct.
 */
struct timespec
{
    int         tv_sec;      /**< second */
    long        tv_nsec;     /**< nanosecond */
};

/**
 *  @ingroup time
 *  Time struct of timer.
 */
struct itimerspec
{
    struct timespec     it_interval;/**< timer interval */
    struct timespec     it_value;   /**< initial expiration */
};

/*
 * Structure returned by gettimeofday(2) system call,
 * and used in other calls.
 */
/**
 * @ingroup time
 * Time struct.
 */
struct timeval {
    long tv_sec;    /**< second */
    long tv_usec;   /**< microseconds */
};

/**
 *  @ingroup time
 *  Time zone struct.
 */
struct timezone {
    int tz_minuteswest; /**< time difference compare with Greenwich time in mimutes */
    int tz_dsttime;     /**< time zone number */
};

/**
 *  @ingroup time
 *  Time zone number : no-use.
 */
#define DST_NONE    0
/**
 *  @ingroup time
 *  Time zone number : U.S.A.
 */
#define DST_USA     1
/**
 *  @ingroup time
 *  Time zone number : Australia.
 */
#define DST_AUST    2
/**
 *  @ingroup time
 *  Time zone number : Western Europe.
 */
#define DST_WET     3
/**
 *  @ingroup time
 *  Time zone number : Central Europe.
 */
#define DST_MET     4
/**
 *  @ingroup time
 *  Time zone number : Eastern Europe.
 */
#define DST_EET     5
/**
 *  @ingroup time
 *  Time zone number : Canada.
 */
#define DST_CAN     6

/* Operations on timevals. */
#define timerclear(a)   \
        ((a)->tv_sec = (a)->tv_usec = 0)

#define timerisset(a)    \
        ((a)->tv_sec != 0 || (a)->tv_usec != 0)

#define timercmp(a, b, op)               \
        ((a)->tv_sec == (b)->tv_sec      \
        ? (a)->tv_usec op (b)->tv_usec   \
        : (a)->tv_sec op (b)->tv_sec)

#define timeradd(a, b, res)                           \
    do {                                              \
        (res)->tv_sec  = (a)->tv_sec  + (b)->tv_sec;  \
        (res)->tv_usec = (a)->tv_usec + (b)->tv_usec; \
        if ((res)->tv_usec >= 1000000) {              \
            (res)->tv_usec -= 1000000;                \
            (res)->tv_sec  += 1;                      \
        }                                             \
    } while (0)

#define timersub(a, b, res)                           \
    do {                                              \
        (res)->tv_sec  = (a)->tv_sec  - (b)->tv_sec;  \
        (res)->tv_usec = (a)->tv_usec - (b)->tv_usec; \
        if ((res)->tv_usec < 0) {                     \
            (res)->tv_usec += 1000000;                \
            (res)->tv_sec  -= 1;                      \
        }                                             \
    } while (0)


/**
 *@ingroup time
 *@brief Get the current time.
 *
 *@par Description:
 *<ul>
 *<li> This API is used to gets the time value of the current time,the time value is equal to the number of seconds in January 1,1970 to the present.</li>
 *</ul>
 *@attention
 *<ul>
 *<li> The time to get is relative.</li>
 *<li> The use of the timezone structure is obsolete, the tz argument should normally be specified as NULL.</li>
 *<li> If either tv or tz is NULL, the corresponding structure is not set or returned.</li>
 *</ul>
 *
 *@param tv   [OUT] Time structure.
 *@param tz   [OUT] Time zone structure.
 *
 *@retval  0    Succeed.
 *@retval -1    Failed, with any of the following error codes in the errno:
                #EINVAL     : Invalid parameter.
 *@par Dependency:
 *<ul><li>time.h The header file that contains the API declaration.</li></ul>
 *@see settimeofday
 *@since Huawei LiteOS V100R001C00
 */
int gettimeofday(struct timeval * tv, struct timezone * tz);

struct tms {
  long tms_utime;
  long tms_stime;
  long tms_cutime;
  long tms_cstime;
};

#define PROP_VALUE_MAX  92

/**
 * @ingroup time
 * time zone of China.
 */
#ifndef CHAR_BIT
#define CHAR_BIT        8
#endif

#define TYPE_BIT(type)  (sizeof(type) * CHAR_BIT)

#define TYPE_INTEGRAL(type) ((((type) 0.5) != 0.5) ? 1 : 0)
#define TYPE_SIGNED(type)   ((((type) -1) < 0) ? 1 : 0)

/* The minimum and maximum finite time values.  */
static time_t const time_t_min =
  (TYPE_SIGNED(time_t)
   ? (time_t) -1 << (CHAR_BIT * sizeof (time_t) - 1)
   : 0);
static time_t const time_t_max =
  (TYPE_SIGNED(time_t)
   ? - (~ 0 < 0) - ((time_t) -1 << (CHAR_BIT * sizeof (time_t) - 1))
   : -1);

extern time_t time(time_t *);
//extern int      nanosleep(const struct timespec *, struct timespec *);

//extern char *strtotimeval(const char *str, struct timeval *tv);

/**
 * @ingroup time
 * time structure
 */
struct tm {
   int     tm_sec;         /**< The number of seconds after the minute, normally in the range 0 to 59, but can be up to 60 to allow for leap seconds */
   int     tm_min;         /**< The number of minutes after the hour, in the range 0 to 59 */
   int     tm_hour;        /**< The number of hours past midnight, in the range 0 to 23 */
   int     tm_mday;        /**< The day of the month, in the range 1 to 31 */
   int     tm_mon;         /**< The number of months since January, in the range 0 to 11 */
   int     tm_year;        /**< The number of years since 1900. */
   int     tm_wday;        /**< The number of days since Sunday, in the range 0 to 6 */
   int     tm_yday;        /**< The number of days since January 1, in the range 0 to 365 */
   int     tm_isdst;       /**< A flag that indicates whether daylight saving time is in effect at the time described. The value is positive if daylight saving time is in effect, zero if it is not, and negative if the information is not available */

   long int tm_gmtoff;     /**< timezone number */
   const char *tm_zone;    /**< timezone */
};

/* defining TM_ZONE indicates that we have a "timezone abbreviation" field in
 * struct tm, the value should be the field name
 */
#define   TM_ZONE   tm_zone


/**
 *@ingroup time
 *@brief Converts a broken-down time structure, expressed as local time, to calendar time representation.
 *
 *@par Description:
 *<ul>
 *<li> This API is used to converts the calendar time t into a null-terminated string of the form "Wed Jun 30 21:49:08 1993\n".</li>
 *</ul>
 *@attention
 *<ul>
 *<li> This API return a pointer to static data and hence are not thread-safe, Thread-safe versions is asctime_r().</li>
 *</ul>
 *
 *@param __tp   [IN] Time structure.
 *
 *@retval char*     Succeed, return a null-terminated string, with the same format as ctime().
 *@retval NULL      Failed, with any of the following error codes in the errno:
                    #EINVAL     : Invalid parameter.
 *@par Dependency:
 *<ul><li>time.h The header file that contains the API declaration.</li></ul>
 *@see asctime_r
 *@since Huawei LiteOS V100R001C00
 */
extern char *asctime (const struct tm *__tp);

/**
 *@ingroup time
 *@brief Converts a broken-down time structure, expressed as local time, to calendar time representation.
 *
 *@par Description:
 *<ul>
 *<li> This API is used to converts the calendar time t into a null-terminated string of the form "Wed Jun 30 21:49:08 1993\n".</li>
 *</ul>
 *@attention
 *<ul>
 *<li> The passed-in t should be a valid point to struct tm.</li>
 *<li> The passed-in buff should have at least 26 bytes memory.</li>
 *</ul>
 *
 *@param t     [IN]  Time structure.
 *@param buf   [OUT] Point to the user-supplied buffer where to store the string.
 *
 *@retval char*  Succeed, return a null-terminated string, with the same format as ctime().
 *@retval NULL   Failed, with any of the following error codes in the errno:
                    #EINVAL     : Invalid parameter.
 *@par Dependency:
 *<ul><li>time.h The header file that contains the API declaration.</li></ul>
 *@see asctime
 *@since Huawei LiteOS V100R001C00
 */
extern char* asctime_r(const struct tm* t, char* buf);


/**
 *@ingroup time
 *@brief Converts a broken-down time structure, expressed as local time, to calendar time representation.
 *
 *@par Description:
 *<ul>
 *<li> This API is used to converts a broken-down time structure, expressed as local time, to calendar time representation.</li>
 *</ul>
 *@attention
 *<ul>
 *<li> The function ignores the values supplied by the caller in the tm_wday and tm_yday fields, and tm_wday and tm_yday are set to values determined from the contents of the other fields.</li>
 *<li> Calling mktime() also sets the external variable tzname with information about the current timezone. </li>
 *<li> The function also sets the external variables tzname, timezone, and daylight with information about the current timezone. </li>
 *</ul>
 *
 *@param tmp   [IN] Time structure need to be convertted.
 *
 *@retval  time_t    Return the number of seconds elapsed since the Epoch, 1970-01-01 00:00:00 +0000 (UTC).
 *@retval  -1        Failed, if the specified broken-down time cannot be represented as calendar time.
 *@par Dependency:
 *<ul><li>time.h The header file that contains the API declaration.</li></ul>
 *@see asctime_r
 *@since Huawei LiteOS V100R001C00
 */
extern time_t mktime (struct tm *tmp);

/**
 *@ingroup time
 *@brief Converts the calendar time timep to broken-down time representation.
 *
 *@par Description:
 *<ul>
 *<li> This API is used to converts the calendar time timep to broken-down time representation, expressed relative to the user's specified timezone.</li>
 *</ul>
 *@attention
 *<ul>
 *<li> The return value points to a statically allocated struct which might be overwritten by subsequent calls to any of the date and time functions, Thread-safe versions is localtime_r().</li>
 *</ul>
 *
 *@param timep   [IN] Represents calendar time, When interpreted as an absolute time value, it represents the number of seconds elapsed since the Epoch, 1970-01-01 00:00:00 +0000 (UTC).
 *
 *@retval tm*       Succeed, points to a statically allocated tm struct.
 *@retval NULL      Failed.
 *@par Dependency:
 *<ul><li>time.h The header file that contains the API declaration.</li></ul>
 *@see localtime_r, asctime
 *@since Huawei LiteOS V100R001C00
 */
extern struct tm*  localtime(const time_t *timep);

/**
 *@ingroup time
 *@brief Converts the calendar time timep to broken-down time representation.
 *
 *@par Description:
 *<ul>
 *<li> This API is used to converts the calendar time timep to broken-down time representation, expressed relative to the user's specified timezone.</li>
 *</ul>
 *@attention
 *<ul>
 *<li> The user-supplied struct specified by result can not be NULL.</li>
 *</ul>
 *
 *@param timep   [IN] Represents calendar time, When interpreted as an absolute time value, it represents the number of seconds elapsed since the Epoch, 1970-01-01 00:00:00 +0000 (UTC).
 *@param result  [OUT] Point to the user-supplied struct where to stores the data.
 *
 *@retval tm*       Succeed, points to the user-supplied tm struct.
 *@retval NULL      Failed.
 *@par Dependency:
 *<ul><li>time.h The header file that contains the API declaration.</li></ul>
 *@see localtime, asctime
 *@since Huawei LiteOS V100R001C00
 */
extern struct tm*  localtime_r(const time_t *timep, struct tm *result);

/**
 *@ingroup time
 *@brief Converts the calendar time timep to broken-down time representation.
 *
 *@par Description:
 *<ul>
 *<li> This API is used to converts the calendar time timep to broken-down time representation, expressed in Coordinated Universal Time (UTC).</li>
 *</ul>
 *@attention
 *<ul>
 *<li> It may return NULL when the year does not fit into an integer.</li>
 *<li> The return value points to a statically allocated struct which might be overwritten by subsequent calls to any of the date and time functions, Thread-safe versions is gmtime_r().</li>
 *</ul>
 *
 *@param timep   [IN] Represents calendar time, When interpreted as an absolute time value, it represents the number of seconds elapsed since the Epoch, 1970-01-01 00:00:00 +0000 (UTC).
 *
 *@retval tm*       Succeed, points to a statically allocated struct.
 *@retval NULL      Failed.
 *@par Dependency:
 *<ul><li>time.h The header file that contains the API declaration.</li></ul>
 *@see gmtime_r, asctime
 *@since Huawei LiteOS V100R001C00
 */
extern struct tm*  gmtime(const time_t *timep);

/**
 *@ingroup time
 *@brief Converts the calendar time timep to broken-down time representation.
 *
 *@par Description:
 *<ul>
 *<li> This API is used to converts the calendar time timep to broken-down time representation, expressed in Coordinated Universal Time (UTC).</li>
 *</ul>
 *@attention
 *<ul>
 *<li> It may return NULL when the year does not fit into an integer.</li>
 *<li> The user-supplied struct specified by result can not be NULL.</li>
 *</ul>
 *
 *@param timep   [IN]  Represents calendar time, When interpreted as an absolute time value, it represents the number of seconds elapsed since the Epoch, 1970-01-01 00:00:00 +0000 (UTC).
 *@param result  [OUT] Point to the user-supplied struct where to stores the data.
 *
 *@retval tm*       Succeed, points to the user-supplied tm struct.
 *@retval NULL      Failed.
 *@par Dependency:
 *<ul><li>time.h The header file that contains the API declaration.</li></ul>
 *@see gmtime, asctime
 *@since Huawei LiteOS V100R001C00
 */
extern struct tm*  gmtime_r(const time_t *timep, struct tm *result);

//extern char*       strptime(const char *buf, const char *fmt, struct tm *tm);

/**
 *@ingroup time
 *@brief Format date and time.
 *
 *@par Description:
 *<ul>
 *<li> This API is used to formats the broken-down time tm according to the format specification format and places the result in the character array s of size max.</li>
 *</ul>
 *
 *@param s        [OUT] Where to store the output string.
 *@param max      [IN]  Max length of the buffer specified by s.
 *@param format   [IN]  The specified format.
 *@param tm       [IN]  Point to the tm structure need to be formatted.
 *
 *@retval  nozero  On succeed, returns the number of bytes (excluding the terminating  null byte) placed in the array s.
 *@retval  0       Failed, The length of the result string (including the terminating null byte) would exceed max bytes.
 *@par Dependency:
 *<ul><li>time.h The header file that contains the API declaration.</li></ul>
 *@see ctime
 *@since Huawei LiteOS V100R001C00
 */
extern size_t      strftime(char *s, size_t max, const char *format, const struct tm *tm);


/**
 *@ingroup time
 *@brief Converts the calendar time_t into a null-terminated string of the form "Wed Jun 30 21:49:08 1993\n".
 *
 *@par Description:
 *<ul>
 *<li> This API is used to Converts the calendar time t into a null-terminated string of the form "Wed Jun 30 21:49:08 1993\n".</li>
 *</ul>
 *@attention
 *<ul>
 *<li> This API return a pointer to static data and hence are not thread-safe, Thread-safe versions is ctime_r().</li>
 *</ul>
 *
 *@param timep   [IN] Represents calendar time, When interpreted as an absolute time value, it represents the number of seconds elapsed since the Epoch, 1970-01-01 00:00:00 +0000 (UTC).
 *
 *@retval char*     Succeed, points to a statically allocated buffer where to store a null-terminated string.
 *@retval NULL      Failed, with any of the following error codes in the errno:
                    #EINVAL     : Invalid parameter.
 *@par Dependency:
 *<ul><li>time.h The header file that contains the API declaration.</li></ul>
 *@see ctime_r, asctime
 *@since Huawei LiteOS V100R001C00
 */
extern char *ctime(const time_t *timep);

/**
 *@ingroup time
 *@brief Converts the calendar time_t into a null-terminated string of the form "Wed Jun 30 21:49:08 1993\n".
 *
 *@par Description:
 *<ul>
 *<li> This API is used to Converts the calendar time t into a null-terminated string of the form "Wed Jun 30 21:49:08 1993\n".</li>
 *</ul>
 *@attention
 *<ul>
 *<li> The API stores the string in a user-supplied buffer which should have room for at least 26 bytes.</li>
 *</ul>
 *
 *@param timep   [IN]  Represents calendar time, it represents the number of seconds elapsed since the Epoch, 1970-01-01 00:00:00 +0000 (UTC).
 *@param buf     [OUT] Point to the user-supplied buffer where to store the string.
 *
 *@retval char*     Succeed, points to user-supplied buffer where to store a null-terminated string.
 *@retval NULL      Failed, with any of the following error codes in the errno:
                    #EINVAL     : Invalid parameter.
 *@par Dependency:
 *<ul><li>time.h The header file that contains the API declaration.</li></ul>
 *@see ctime_r, asctime
 *@since Huawei LiteOS V100R001C00
 */
extern char *ctime_r(const time_t *timep, char *buf);

/**
 *@ingroup time
 *@brief Initialize time conversion information.
 *
 *@par Description:
 *<ul>
 *<li> This API is used to initializes the tzname variable from the TZ environment variable, This function is automatically called by the other time conversion functions that depend on the timezone.</li>
 *</ul>
 *
 *@param None.
 *
 *@retval  None.
 *@par Dependency:
 *<ul><li>time.h The header file that contains the API declaration.</li></ul>
 *@see ctime
 *@since Huawei LiteOS V100R001C00
 */
extern void  tzset(void);

/* global includes */
extern char*     tzname[];
extern int       daylight;
extern long int  timezone;

#define CLOCKS_PER_SEC              1000000L


/* Identifier for system-wide realtime clock.  */
#define CLOCK_REALTIME              0
/* Monotonic system-wide clock.  */
#define CLOCK_MONOTONIC             1
/* High-resolution timer from the CPU.  */
#define CLOCK_PROCESS_CPUTIME_ID    2
/* Thread-specific CPU-time clock.  */
#define CLOCK_THREAD_CPUTIME_ID     3
#define CLOCK_MONOTONIC_RAW         4
#define CLOCK_REALTIME_COARSE       5
#define CLOCK_MONOTONIC_COARSE      6
#define CLOCK_BOOTTIME              7
#define CLOCK_REALTIME_ALARM        8
#define CLOCK_BOOTTIME_ALARM        9

#define TM_SUNDAY                   0
#define TM_MONDAY                   1
#define TM_TUESDAY                  2
#define TM_WEDNESDAY                3
#define TM_THURSDAY                 4
#define TM_FRIDAY                   5
#define TM_SATURDAY                 6

#define TM_YEAR_BASE                1900

#define EPOCH_YEAR                  1970
#define EPOCH_WDAY                  TM_THURSDAY

#define SECSPERMIN                  60
#define MINSPERHOUR                 60
#define HOURSPERDAY                 24
#define DAYSPERWEEK                 7
#define DAYSPERNYEAR                365
#define DAYSPERLYEAR                366
#define SECSPERHOUR                 (SECSPERMIN * MINSPERHOUR)
#define SECSPERDAY                  ((int) SECSPERHOUR * HOURSPERDAY)
#define MONSPERYEAR                 12

#ifndef TZ_MAX_CHARS
#define TZ_MAX_CHARS                50  /* Maximum number of abbreviation characters */
                                        /* (limited by what unsigned chars can hold) */
#endif /* !defined TZ_MAX_CHARS */

#define TZDEFRULE           "posixrules"

#define TZDEFAULT           "localtime"

#define GRANDPARENTED       "Local time zone must be set--see zic manual page"

#define TZ_MAX_TIMES        1200

#define SECSPERREPEAT_BITS  34  /* ceil(log2(SECSPERREPEAT)) */

#ifndef SECSPERREPEAT
#define SECSPERREPEAT       ((int) YEARSPERREPEAT * (long long) AVGSECSPERYEAR)
#endif /* !defined SECSPERREPEAT */

#ifndef TZ_MAX_LEAPS
#define TZ_MAX_LEAPS        50  /* Maximum number of leap second corrections */
#endif /* !defined TZ_MAX_LEAPS */

struct tzhead {
    char tzh_magic[4];      /* TZ_MAGIC */
    char tzh_version[1];    /* '\0' or '2' as of 2005 */
    char tzh_reserved[15];  /* reserved--must be zero */
    char tzh_ttisgmtcnt[4]; /* coded number of trans. time flags */
    char tzh_ttisstdcnt[4]; /* coded number of trans. time flags */
    char tzh_leapcnt[4];    /* coded number of leap seconds */
    char tzh_timecnt[4];    /* coded number of transition times */
    char tzh_typecnt[4];    /* coded number of local time types */
    char tzh_charcnt[4];    /* coded number of abbr. chars */
};

extern time_t timelocal(struct tm *tm);
extern time_t timegm(struct tm* tm);
extern time_t time2posix(time_t ti);
extern time_t posix2time(time_t ti);
extern int check_tm(const struct tm *tms_s);


/**
 *@ingroup time
 *@brief Delete a software timer.
 *
 *@par Description:
 *<ul>
 *<li> This API is used to deletes the timer whose ID is given in timerid.</li>
 *</ul>
 *@attention
 *<ul>
 *<li> Muli-delete one timer does return error, not suggest do like this. </li>
 *</ul>
 *
 *@param timer_id   [IN] Timer id.
 *
 *@retval  0    Delete succeed.
 *@retval -1    Delete failed, with any of the following error codes in the errno:
                #EINVAL     : Invalid parameter.
 *@par Dependency:
 *<ul><li>time.h The header file that contains the API declaration.</li></ul>
 *@see timer_create
 *@since Huawei LiteOS V100R001C00
 */
extern int  timer_delete(timer_t timerid);

/**
 *@ingroup time
 *@brief Arm/disarm a software timer.
 *
 *@par Description:
 *<ul>
 *<li> This API arms or disarms the timer identified by timerid.</li>
 *</ul>
 *@attention
 *<ul>
 *<li> If value->it_value specifies a nonzero value(i.e., either subfield is nonzero), then timer_settime() arms(starts) the timer, setting it to initially expire at the given time. (If the timer was already armed, then the previous settings are overwritten.)
 *     If value->it_value specifies a zero value(i.e., both subfields are zero), then the timer is disarmed.</li>
 *<li> The value->it_interval field specifies the period of the timer, in seconds and nanoseconds.
 *     If this field is nonzero, then each time that an armed timer expires, the timer is reloaded from the value specified in value->it_interval.
 *     If this field is zero, then the timer expires just once, at the time specified by it_value.</li>
 *</ul>
 *
 *@param timerid    [IN]  Timer id.
 *@param flags      [IN]  This field indicated the time specified by value is absolute time or relative time. This flags is never be used.
 *@param value      [IN]  pointer to an itimerspec structure that specifies the new initial value and the new interval for the timer.
 *@param ovalue     [OUT] points to a itimerspec structure that is used to return the previous interval of the timer and the amount of time until the timer would previously have next expired, always NULL.
 *
 *@retval  0    Success.
 *@retval -1    Failed, with any of the following error codes in the errno:
                #EINVAL         : Invalid parameter.
 *@par Dependency:
 *<ul><li>time.h The header file that contains the API declaration.</li></ul>
 *@see timer_gettime
 *@since Huawei LiteOS V100R001C00
 */
extern int  timer_settime(timer_t timerid, int flags, const struct itimerspec *value, struct itimerspec *ovalue);

/**
 *@ingroup time
 *@brief Fetch state of software timer.
 *
 *@par Description:
 *<ul>
 *<li> This API returns the time until next expiration, and the interval, for the timer specified by timerid.</li>
 *</ul>
 *
 *@param timer_id   [IN]  Timer id.
 *@param value      [OUT] Point the timer struct where to store the return value.
 *
 *@retval  0    Success.
 *@retval -1    Failed, with any of the following error codes in the errno:
                #EINVAL         : Invalid parameter.
 *@par Dependency:
 *<ul><li>time.h The header file that contains the API declaration.</li></ul>
 *@see timer_settime
 *@since Huawei LiteOS V100R001C00
 */
extern int  timer_gettime(timer_t timerid, struct itimerspec *value);

/**
 *@ingroup time
 *@brief Get the repeat timing times for a software timer.
 *
 *@par Description:
 *<ul>
 *<li> This API returns the "repeat timing times" for the timer referred to by timerid.</li>
 *</ul>
 *@attention
 *<ul>
 *<li> This API in Huawei LiteOS is different with POSIX standard. </li>
 *<li> In Huawei LiteOS, The timer overrun count is the expirations times of a periodic software timer. </li>
 *<li> In POSIX, The timer overrun count is the number of additional timer expirations that occurred between the time when the signal was generated and when it was delivered or accepted. </li>
 *<li> In POSIX, The timer overruns can also occur when expiration notifications are delivered via invocation of a thread, since there may be an arbitrary delay between  an expiration of the timer and the invocation of the notification thread, and in that delay interval, additional timer expirations may occur </li>
 *</ul>
 *
 *@param timer_id   [IN] Timer id.
 *
 *@retval  int   On success, returns the repeat timing times of the specified timer, this count may be 0 if no overruns have occurred.
 *@retval -1    Failed, with any of the following error codes in the errno:
                #EINVAL         : Invalid parameter.
                #DELAYTIMER_MAX : Overrun count exceed than DELAYTIMER_MAX.
 *@par Dependency:
 *<ul><li>time.h The header file that contains the API declaration.</li></ul>
 *@see timer_create
 *@since Huawei LiteOS V100R001C00
 */
extern int  timer_getoverrun(timer_t  timerid);

/**
 *@ingroup time
 *@brief Set the time of the specified clock clock_id.
 *
 *@par Description:
 *<ul>
 *<li> This API is used to Set the time of the specified clock clock_id.</li>
 *</ul>
 *
 *@param clock_id   [IN]  The identifier of the particular clock on which to act, only support CLOCK_REALTIME.
 *@param tp         [IN]  Point to the timespec structure.
 *
 *@retval  0    Succeed.
 *@retval -1    Failed, with any of the following error codes in the errno:
                #EINVAL     : Invalid parameter.
 *@par Dependency:
 *<ul><li>time.h The header file that contains the API declaration.</li></ul>
 *@see clock_gettime
 *@since Huawei LiteOS V100R001C00
 */
extern int clock_settime( clockid_t clock_id, const struct timespec *tp );

/**
 *@ingroup time
 *@brief Retrieve the time of the specified clock clock_id.
 *
 *@par Description:
 *<ul>
 *<li> This API is used to Retrieve the time of the specified clock clock_id.</li>
 *</ul>
 *
 *@param clock_id   [IN]  The identifier of the particular clock on which to act.
 *@param tp         [OUT] Point to the timespec structure.
 *
 *@retval  0    Succeed.
 *@retval -1    Failed, with any of the following error codes in the errno:
                #EINVAL     : Invalid parameter.
 *@par Dependency:
 *<ul><li>time.h The header file that contains the API declaration.</li></ul>
 *@see clock_settime
 *@since Huawei LiteOS V100R001C00
 */
extern int clock_gettime( clockid_t clock_id, struct timespec *tp );

/**
 *@ingroup time
 *@brief Get the precision of the specified clock clock_id.
 *
 *@par Description:
 *<ul>
 *<li> This API is used to Get the precision of the specified clock clock_id, and stores it in the struct timespec pointed to by tp.</li>
 *</ul>
 *@attention
 *<ul>
 *<li> In Huawei LiteOS, the clock type only support CLOCK_REALTIME.</li>
 *</ul>
 *
 *@param clock_id   [IN]  Specifies the clock type, only support CLOCK_REALTIME.
 *@param tp         [OUT] Where to store the precision.
 *
 *@retval  0    Succeed.
 *@retval -1    Failed, with any of the following error codes in the errno:
                #EINVAL     : Invalid parameter.
 *@par Dependency:
 *<ul><li>time.h The header file that contains the API declaration.</li></ul>
 *@see gettimeofday
 *@since Huawei LiteOS V100R001C00
 */
extern int clock_getres( clockid_t clock_id, struct timespec *tp );

/**
 *@ingroup time
 *@brief Create a software timer.
 *
 *@par Description:
 *<ul>
 *<li> This API is used to create a software timer that has specified timing duration, timeout handling function, and trigger mode, and to return a handle by which the software timer can be referenced.</li>
 *</ul>
 *@attention
 *<ul>
 *<li> Do not use the delay interface in the callback function that handles software timer timeout.</li>
 *</ul>
 *
 *@param clock_id   [IN]  Specifies the clock type that the new timer uses to measure time, only support CLOCK_REALTIME.
 *@param evp        [IN]  Points to a sigevent structure that specifies how the caller should be notified when the timer expires, The sevp.sigev_notify field only support the value of SIGEV_THREAD.
 *@param timer_id   [OUT] Where to place the new timer id.
 *
 *@retval  0    Create succeed.
 *@retval -1    Create failed, with any of the following error codes in the errno:
                #EINVAL     : Invalid parameter.
                #EAGAIN     : The number of software timers exceeds the configured permitted maximum number.
 *@par Dependency:
 *<ul><li>time.h The header file that contains the API declaration.</li></ul>
 *@see timer_delete
 *@since Huawei LiteOS V100R001C00
 */
extern int timer_create(clockid_t clock_id, struct sigevent *evp, timer_t *timer_id);

/**
 *@ingroup time
 *@brief Calculate time difference.
 *
 *@par Description:
 *<ul>
 *<li> This API returns the number of seconds elapsed between time time2 and time time1, represented as a double. Each of the times is specified in calendar time, which means its value is a measurement (in seconds) relative to the Epoch, 1970-01-01 00:00:00 +0000 (UTC).</li>
 *</ul>
 *
 *@param time2   [IN]  minuend time_t.
 *@param time1   [IN]  subtrahend time_t.
 *
 *@retval  double The time difference in seconds.
 *@par Dependency:
 *<ul><li>time.h The header file that contains the API declaration.</li></ul>
 *@see gettimeofday
 *@since Huawei LiteOS V100R001C00
 */
extern double difftime(time_t time2, time_t time1);

/**
 *@ingroup time
 *@brief Calculate an approximation of processor time used by the program.
 *
 *@par Description:
 *<ul>
 *<li> This API is used to calculate an approximation of processor time used by the program.</li>
 *</ul>
 *@attention
 *<ul>
 *<li> Call this function before and after a code segment or a function, then calculate the difference between the two can get the function or the code segment running time.</li>
 *</ul>
 *
 *@param None
 *
 *@retval clock_t  The value returned is the CPU time used so far as a millisecond.
 *@par Dependency:
 *<ul><li>time.h The header file that contains the API declaration.</li></ul>
 *@see clock_gettime
 *@since Huawei LiteOS V100R001C00
 */
extern clock_t clock(VOID);

extern clock_t times(struct tms * buf);

/**
 * @ingroup time
 * @brief suspend execution for nanosecond intervals
 *
 * @par Description:
 * The nanosleep() function suspends execution of the calling thread for (at least) nanoseconds.
 * The sleep may be lengthened slightly by any system activity or by the time spent processing the call or by the granularity of system timers.
 *
 * @attention
 * <ul>
 * <li>If 0 is passed in and the thread scheduling is not locked, execute the next thread in the queue of thread with the same priority of the current thread.
 * If no ready thread with the priority of the current thread is available, the thread scheduling will not occur, and the current thread continues to be executed.</li>
 * <li>The function that if the call is interrupted by a signal handler, nanosleep() returns -1, sets errno to EINTR, and writes the remaining time into  the
 *structure pointed to by rem unless rem is NULL doesn't support.
 * <li>The minimum precision of system timer is one tick(10ms), if the passed-in nanoseconds value is less than 10000000, the calling thread will be suspend at least 10000000 nanoseconds(10ms).</li>
 * </ul>
 *
 * @param  rqtp          [IN]  pointer to an timespec structure that specifies the new initial nanoseconds to be suspended.
 * @param  rmtp          [OUT] points to a timespec structure that is used to return the previous interval of remaining time and the amount of time until the timer would previously have next expired, always NULL.
 *
 * @retval #0                              Success.
 * @retval #LOS_ERRNO_SWTMR_HWI_ACTIVE     An interrupt is active.
 * @retval #-1                             On error.
 *
 * @par Dependency:
 * <ul><li>time.h: the header file that contains the API declaration.</li></ul>
 * @see usleep
 *
 * @since Huawei LiteOS V100R001C00
 */
extern int nanosleep(const struct timespec *rqtp, struct timespec *rmtp);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif
