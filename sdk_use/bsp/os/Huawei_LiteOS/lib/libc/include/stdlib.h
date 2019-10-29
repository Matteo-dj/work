/*
 * Copyright (C) 2008 The Android Open Source Project
 * All rights reserved.
 *
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

/****************************************************************************
 * include/stdlib.h
 *
 *   Copyright (C) 2007-2014 Gregory Nutt. All rights reserved.
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
/** @defgroup stdlib Stdlib
 *  @ingroup lib
*/
#ifndef LIBC_STDLIB_H
#define LIBC_STDLIB_H
#include "los_config.h"
#include "los_typedef.h"
#include "ctype.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */


/* String to binary conversions */

extern long strtol(const char *, char **, int);
extern unsigned long strtoul(const char *, char **, int);
extern long long strtoll(const char *, char **, int);
extern unsigned long long strtoull(const char *, char **, int);
extern double strtod(const char *, char **);
extern float strtof(const char*, char**);
extern long double strtold(const char*, char**);

extern double atof(const char *);
extern int atoi(const char *);
extern long atol(const char *);
extern long long atoll(const char *);


/* Misc */
extern int abs(int j);
extern long int labs(long int j);
extern long long int llabs(long long int j);


/****
Multibyte Functions
****/
#ifndef __cplusplus
#ifndef WCHAR_T
#define WCHAR_T
typedef unsigned int wchar_t;
#endif
#endif

/****
Environment Functions
****/
//system not support
extern int system(const char *string);
extern char *getenv(const char *name);
extern void abort(void);
extern void exit(int status);
extern void _exit(int status);

/**
* @ingroup stdlib
* @brief Allocate and zero-initialize array
*
* @par Description:
* This API is used to allocates a block of memory for an array of nitems elements, each of them size bytes long, and initializes all its bits to zero.

* @attention None.
*
* @param nitems [IN] Type #size_t Number of elements to allocate.
* @param size [IN] Type #size_t Size of each element.
*
* @retval A pointer to the memory block allocated by the function on success, or NULL for allocating the requested block of memory failed.
* @par Dependency:
* <ul><li>stdlib.h: the header file that contains the API declaration.</li></ul>
*@see free
* @since Huawei LiteOS V100R001C00
*/
extern void *calloc(size_t nitems, size_t size);

/**
* @ingroup stdlib
* @brief Deallocate memory block
*
* @par Description:
* This API is used to deallocate a block of memory previously allocated by a call to malloc, calloc or realloc, making it available again for further allocations.

* @attention None.
*
* @param ptr [IN] Type #void * Pointer to a memory block previously allocated with malloc, calloc or realloc.
*
* @retval None
* @par Dependency:
* <ul><li>stdlib.h: the header file that contains the API declaration.</li></ul>
*@see malloc, calloc or realloc
* @since Huawei LiteOS V100R001C00
*/
extern void free(void *ptr);

/**
* @ingroup stdlib
* @brief Allocate memory block
*
* @par Description:
* This API is used to allocate a block of size bytes of memory, returning a pointer to the beginning of the block.

* @attention None.
*
* @param size [IN] Type #size_t Size of the memory block, in bytes.
*
* @retval A pointer to the memory block allocated by the function on success, or NULL for allocating the requested block of memory failed.
* @par Dependency:
* <ul><li>stdlib.h: the header file that contains the API declaration.</li></ul>
*@see free
* @since Huawei LiteOS V100R001C00
*/
extern void *malloc(size_t size);

/**
* @ingroup stdlib
* @brief Reallocate memory block
*
* @par Description:
* This API is used to change the size of the memory block pointed to by ptr.

* @attention The function may move the memory block to a new location (whose address is returned by the function).
*
* @param ptr [IN] Type #void * Pointer to a memory block previously allocated with malloc, calloc or realloc. Alternatively, this can be a null pointer, in which case a new block is allocated (as if malloc was called).
* @param size [IN] Type #size_t New size for the memory block, in bytes.
*
* @retval A pointer to the memory block reallocated by the function on success, or NULL for reallocating the requested block of memory failed.
* @par Dependency:
* <ul><li>stdlib.h: the header file that contains the API declaration.</li></ul>
* @see malloc, free
* @since Huawei LiteOS V100R001C00
*/
extern void *realloc(void *ptr, size_t size);

/**
 *@ingroup libc
 *@brief Allocate dynamic memory.
 *
 *@par Description:
 *<ul>
 *<li>This API is used to allocate memory for an array of the passed-in size of size bytes and returns a pointer to the allocated memory.
 The memory is set to zero.</li>
 *</ul>
 *@attention If the passed-in size is 0, then zalloc() returns NULL.
 *<ul>
 *<li>The size of the input parameter size can not be greater than the memory size of the system.</li>
 *<li>The size of the input parameter size must be four byte-aligned.</li>
 *</ul>
 *
 *@param  size   [IN]Type #size_t Size of the memory block to be allocated (unit: byte).
 *
 *@retval NULL          The memory fails to be allocated.
 *@retval void*         The memory is successfully allocated with the starting address of the allocated memory block returned.
 *@par Dependency:
 *<ul><li>stdlib.h: the header file that contains the API declaration.</li></ul>
 *@see malloc | realloc | free
 *@since Huawei LiteOS V100R001C00
 */
extern void *zalloc(size_t size);

/**
 *@ingroup libc
 *@brief Allocate aligned memory.
 *
 *@par Description:
 *<ul>
 *<li>This API is used to allocate memory blocks of specified size and align on a specified boundary.</li>
 *</ul>
 *@attention
 *<ul>
 *<li>The size of the input parameter size can not be greater than the memory size of the system.</li>
 *<li>The alignment parameter value must be a power of 2 with the minimum value being 4.</li>
 *</ul>
 *
 *@param  boundary  [IN]Type #size_t Boundary, the returned memory address will be a multiple of boundary.
 *@param  size      [IN]Type #size_t Spcified the size need to allocate.
 *
 *@retval NULL          The memory fails to be allocated.
 *@retval void*         The memory is successfully allocated.
 *@par Dependency:
 *<ul><li>stdlib.h: the header file that contains the API declaration.</li></ul>
 *@see None
 *@since Huawei LiteOS V100R001C00
 */
extern void * memalign (size_t boundary, size_t size);

#if OS_SYS_NOCACHEMEM_SIZE
/*memory alloc in no cache eare*/
extern void *nocache_calloc(size_t nitems, size_t size);

extern void nocache_free(void *ptr);

extern void *nocache_malloc(size_t size);

extern void *nocache_zalloc(size_t size);

extern void *nocache_realloc(void *ptr, size_t size);
#endif

unsigned int arc4random(void);

/**
 *@ingroup libc
 *@brief Generator a random number.
 *
 *@par Description:
 *<ul>
 *<li>This API is used to generator a random number in the range from 0 to upper_bound - 1.</li>
 *</ul>
 *@attention
 *<ul>
 *<li>If the passed-in upper_bound is less than 2, this api returns 0, else it returns a number range for 0 to upper_bound - 1.</li>
 *</ul>
 *
 *@param upper_bound [IN]Type #uint32_t uppper boundary of the random number.
 *
 *@retval uint32_t a random value between 0 and upper_bound - 1.
 *@par Dependency:
 *<ul><li>stdlib.h: the header file that contains the API declaration.</li></ul>
 *@see arc4random
 *@since Huawei LiteOS V100R001C00
 */
uint32_t arc4random_uniform(uint32_t upper_bound);

/****
Searching and Sorting Functions
****/
extern void qsort(void* base,size_t nmemb,size_t size,int (*compar)(const void*,const void*));


#define RAND_MAX 0x7fffffff

/* Integer expression whose value is the maximum number of
 * bytes in a character specified by the current locale.
 */
#define MB_CUR_MAX 1

int rand(void);
void srand(unsigned int seed);
void srandom(unsigned int seed);
int mkstemp(char *path);

/**
* @ingroup stdlib
* @brief allocate node from Memory pool with aligned size
*
* @par Description:
* This API is used to align size then allocate node from Memory pool

* @attention The function may wast at most align size byte memory.
*
* @param memptr   [OUT] Type #void **  Pointer to allocated memory node.
* @param alignment [IN]    Type #size_t    Size of memory in bytes to allocate.
* @param size         [IN]    Type #size_t    Align form.
*
* @retval #EINVAL          Invalid argument, not aligned.
* @retval #ENOMEM        Alloc failed.
* @retval #ENOERR         Aligned and alloc success.
* @par Dependency:
* <ul><li>stdlib.h: the header file that contains the API declaration.</li></ul>
* @see free
* @since Huawei LiteOS V100R001C00
*/
int posix_memalign(void * * memptr, size_t alignment, size_t size);

#if 1   /* Huawei LiteOS adapt */
// gccinclude/stdlib.h
/* Returned by `div'.  */
typedef struct
{
    int quot;   /* Quotient.  */
    int rem;    /* Remainder.  */
} div_t;

/* Returned by `ldiv'.  */
#ifndef __ldiv_t_defined
typedef struct
{
    long int quot;  /* Quotient.  */
    long int rem;   /* Remainder.  */
} ldiv_t;
# define __ldiv_t_defined   1
#endif

#ifndef __COMPAR_FN_T
# define __COMPAR_FN_T
typedef int (*__compar_fn_t)(const void *, const void *);
#endif

/* Register a function to be called when `exit' is called.  */
extern int atexit(void (*__func)(void));

/* Do a binary search for KEY in BASE, which consists of NMEMB elements
   of SIZE bytes each, using COMPAR to perform the comparisons.  */
extern void *bsearch(const void *__key, const void *__base,
        size_t __nmemb, size_t __size, __compar_fn_t __compar);

/* Return the `div_t', `ldiv_t' or `lldiv_t' representation
   of the value of NUMER over DENOM. */
/* GCC may have built-ins for these someday.  */
extern div_t div(int __numer, int __denom);
extern ldiv_t ldiv(long int __numer, long int __denom);

/* Return the length of the multibyte character
   in S, which is no longer than N.  */
extern int mblen(const char *__s, size_t __n);

/* Convert a multibyte string to a wide char string.  */
extern size_t mbstowcs(wchar_t *__pwcs, const char *__s, size_t __n);
/* Convert a wide char string to multibyte string.  */
extern size_t wcstombs(char *__s, const wchar_t *__pwcs, size_t __n);
/* Put the multibyte character represented
   by WCHAR in S, returning its length.  */
extern int wctomb(char *__s, wchar_t __wchar);

/* Return the length of the given multibyte character,
   putting its `wchar_t' representation in *PWC.  */
extern int mbtowc (wchar_t *__pwc, const char *__s, size_t __n);
#endif


#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif
