/****************************************************************************
 * include/stdint.h
 *
 *   Copyright (C) 2009, 2011, 2014 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *   Copyright 2003-2010 Haiku Inc. All Rights Reserved.
 *   Distributed under the terms of the MIT License.
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

#ifndef __INCLUDE_STDINT_H
#define __INCLUDE_STDINT_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include "los_typedef.h"
#include "limits.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Limits of exact-width integer types */

#define INT8_MIN            (-INT8_MAX - 1)
#define INT8_MAX            127
#define UINT8_MAX           255

#define INT16_MIN           (-INT16_MAX - 1)
#define INT16_MAX           32767
#define UINT16_MAX          65535u

#define INT32_MIN           (-INT32_MAX - 1)
#define INT32_MAX           2147483647
#define UINT32_MAX          4294967295u

#define INT64_MIN         (-INT64_MAX - 1ll)
#define INT64_MAX         9223372036854775807ll
#define UINT64_MAX        18446744073709551615ull

/* Limits of minimum-width integer types */
#define INT_LEAST8_MAX      INT8_MAX
#define UINT_LEAST8_MAX     UINT8_MAX
#define INT_LEAST8_MIN      INT8_MIN

#define UINT_LEAST16_MAX    UINT16_MAX
#define INT_LEAST16_MAX     INT16_MAX
#define INT_LEAST16_MIN     INT16_MIN

#define UINT_LEAST32_MAX    UINT32_MAX
#define INT_LEAST32_MAX     INT32_MAX
#define INT_LEAST32_MIN     INT32_MIN

/* Limits of fastest minimum-width integer types */
#define INT_FAST8_MAX       INT8_MAX
#define UINT_FAST8_MAX      UINT8_MAX
#define INT_FAST8_MIN       INT8_MIN

#define UINT_FAST16_MAX     UINT16_MAX
#define INT_FAST16_MAX      INT16_MAX
#define INT_FAST16_MIN      INT16_MIN

#define UINT_FAST32_MAX     UINT32_MAX
#define INT_FAST32_MAX      INT32_MAX
#define INT_FAST32_MIN      INT32_MIN

/* Limits of integer types capable of holding object pointers */
#define UINTPTR_MAX         UPTR_MAX
#define INTPTR_MAX          PTR_MIN
#define INTPTR_MIN          PTR_MIN

/* Limits of greatest-width integer types */
#define UINTMAX_MIN       UINT32_MIN
#define UINTMAX_MAX       UINT32_MAX
#define INTMAX_MIN        INT32_MIN
#define INTMAX_MAX        INT32_MAX

/* Macros for minimum-width integer constant expressions */

/* Macros for greatest-width integer constant expressions */

#ifdef CONFIG_HAVE_LONG_LONG
#define INTMAX_C(x)       x ## ll
#define UINTMAX_C(x)      x ## ull
#else
#define INTMAX_C(x)       x ## l
#define UINTMAX_C(x)      x ## ul
#endif

/* Limits of Other Integer Types */

#ifdef CONFIG_SMALL_MEMORY

#define SIZE_MAX            UINT16_MAX  /* See sys/types.h */
#define RSIZE_MAX           UINT16_MAX

#else /* CONFIG_SMALL_MEMORY */

#define SIZE_MAX            UINT32_MAX  /* See sys/types.h */
#define RSIZE_MAX           UINT32_MAX

#endif /* CONFIG_SMALL_MEMORY */

/****************************************************************************
 * Public Types
 ****************************************************************************/

/* Exact-width integer types.  NOTE that these types are defined in
 * architecture-specific logic with leading underscore character. This file
 * typedef's these to the final name without the underscore character.  This
 * roundabout way of doings things allows the stdint.h to be removed from the
 * include/ directory in the event that the user prefers to use the definitions
 * provided by their toolchain header files.
 */
/* Minimum-width integer types */

typedef int8_t             int_least8_t;
typedef uint8_t            uint_least8_t;

typedef int16_t            int_least16_t;
typedef uint16_t           uint_least16_t;

#ifdef __INT24_DEFINED
typedef int24_t            int_least24_t;
typedef uint24_t           uint_least24_t;
#else
typedef int32_t            int_least24_t;
typedef uint32_t           uint_least24_t;
#endif

typedef int32_t            int_least32_t;
typedef uint32_t           uint_least32_t;

typedef int64_t            int_least64_t;
typedef uint64_t           uint_least64_t;

/* Fastest minimum-width integer types */

typedef int8_t             int_fast8_t;
typedef uint8_t            uint_fast8_t;

typedef int                 int_fast16_t;
typedef unsigned int        uint_fast16_t;

#ifdef __INT24_DEFINED
typedef int24_t            int_fast24_t;
typedef uint24_t           uint_fast24_t;
#else
typedef int32_t            int_fast24_t;
typedef uint32_t           uint_fast24_t;
#endif

typedef int32_t            int_fast32_t;
typedef uint32_t           uint_fast32_t;

typedef int64_t            int_fast64_t;
typedef uint64_t           uint_fast64_t;


/* Integer types capable of holding object pointers */

typedef int                 intptr_t;
typedef unsigned int        uintptr_t;

/* Some architectures support a FAR pointer which is larger then the normal
 * (near) pointer
 */

#ifdef CONFIG_HAVE_FARPOINTER
typedef signed long         int_farptr_t;
typedef unsigned long       uint_farptr_t;
#endif


/* Greatest-width integer types */
typedef uint64_t            uintmax_t;
typedef int64_t             intmax_t;

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
#endif /* __INCLUDE_STDINT_H */
