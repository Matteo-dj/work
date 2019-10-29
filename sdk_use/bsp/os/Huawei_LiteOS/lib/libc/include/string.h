/****************************************************************************
 * include/string.h
 *
 *   Copyright (C) 2007-2012, 2014 Gregory Nutt. All rights reserved.
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
#ifndef __INCLUDE_STRING_H
#define __INCLUDE_STRING_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include "los_typedef.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/****************************************************************************
 * Global Function Prototypes
 ****************************************************************************/


extern void bzero(void *s, size_t n);

extern void *memcpy(void *dst, const void *src, size_t n);

extern void *memmove(void *dst, const void *src, size_t n);

extern void *memset(void *dst, int c, size_t n);

extern char *strcpy(char *dst, const char *src);

extern size_t strlcpy(char *dst, const char *src, size_t n);

extern char *strncpy(char *dst, const char *src, size_t n);

extern char *strcat(char *dst, const char *src);

extern char *strncat(char *dst, const char *src, size_t n);

extern int memcmp(const void *str1, const void *str2, size_t n);

extern int strcasecmp(const char *s1, const char *s2);

extern int strncasecmp(const char *s1, const char *s2, size_t n);

extern int strcmp(const char *str1, const char *str2);

extern int strcoll(const char *str1, const char *str2);

extern int strncmp(const char *str1, const char *str2, size_t n);

extern size_t strxfrm(char *dst, const char *src, size_t n);

extern void *memchr(const void *src, int c, size_t n);

extern char *strchr(const char *src, int c);

extern size_t strcspn(const char *str1, const char *str2);

extern char *strpbrk(const char *str1, const char *str2);

extern char *strrchr(const char *src, int c);

extern size_t strspn(const char *str1, const char *str2);

extern char *strstr(const char *str1, const char *str2);

extern char *strtok_r( char *str,  const char *delim,  char **saveptr);

extern  char *strtok(char *src, const char *delim);

extern size_t strlen(const char *src);

extern char *strdup(const char *src);

extern char* strerror(int error_number);

extern char *strcasestr(const char * s, const char * find);

extern char *strsep(char **stringp, const char *delim);
extern int snprintf(char *str, size_t n, const char *fmt, ...);
#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* __INCLUDE_STRING_H */
