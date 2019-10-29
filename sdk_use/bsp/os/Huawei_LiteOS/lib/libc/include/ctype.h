/*	$OpenBSD: ctype.h,v 1.19 2005/12/13 00:35:22 millert Exp $	*/
/*	$NetBSD: ctype.h,v 1.14 1994/10/26 00:55:47 cgd Exp $	*/

/*
 * Copyright (c) 1989 The Regents of the University of California.
 * All rights reserved.
 * (c) UNIX System Laboratories, Inc.
 * Copyright (c) <2014-2015>, <Huawei Technologies Co., Ltd>
 * All rights reserved.
 *
 * All or some portions of this file are derived from material licensed
 * to the University of California by American Telephone and Telegraph
 * Co. or Unix System Laboratories, Inc. and are reproduced herein with
 * the permission of UNIX System Laboratories, Inc.
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
 *	@(#)ctype.h	5.3 (Berkeley) 4/3/91
 */
/*----------------------------------------------------------------------------
 * Notice of Export Control Law
 * ===============================================
 * Huawei LiteOS may be subject to applicable export control laws and regulations, which might
 * include those applicable to Huawei LiteOS of U.S. and the country in which you are located.
 * Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance with such
 * applicable export control laws and regulations.
 *---------------------------------------------------------------------------*/

#ifndef CTYPE_H
#define CTYPE_H

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define SIGN    0x01
#define ESIGN   0x02
#define DECP    0x04

#define Ise(c)          ((c == 'e') || (c == 'E'))
#define Issign(c)       ((c == '-') || (c == '+'))
#define Val(c)          ((c - '0'))

#define _CTYPE_U    0x01
#define _CTYPE_L    0x02
#define _CTYPE_D    0x04
#define _CTYPE_S    0x08
#define _CTYPE_P    0x10
#define _CTYPE_C    0x20
#define _CTYPE_X    0x40
#define _CTYPE_B    0x80
#define _CTYPE_R    (_CTYPE_P|_CTYPE_U|_CTYPE_L|_CTYPE_D|_CTYPE_B)
#define _CTYPE_A    (_CTYPE_L|_CTYPE_U)

#define _U _CTYPE_U
#define _L _CTYPE_L
#define _N _CTYPE_D
#define _S _CTYPE_S
#define _P _CTYPE_P
#define _C _CTYPE_C
#define _X _CTYPE_X
#define _B _CTYPE_B
#define CTYPE_NUM_CHARS       256
/****************************************************************************
 * Name: isascii
 *
 * Description:
 *   Checks whether c is a 7-bit unsigned char value that fits into the
 *   ASCII character set.
 *
 ****************************************************************************/
#define isascii(c)   ((c) >= 0 && (c) <= 0x7f)

/****************************************************************************
 * Name: toascii
 *
 * Description:
 *   Checks whether c is a 7-bit unsigned char value that fits into the
 *   ASCII character set.
 *
 ****************************************************************************/
#define  toascii(c) ((c) & 0x7f)

/****************************************************************************
 * Public Functions
 ****************************************************************************/
int isalnum(int);
int isalpha(int);
int isblank(int);
int iscntrl(int);
int isdigit(int);
int isgraph(int);
int islower(int);
int isprint(int);
int ispunct(int);
int isspace(int);
int isupper(int);
int isxdigit(int);

extern int tolower(unsigned char c);
extern int toupper(unsigned char c);

#define LSBFIRST 1234
#define MSBFIRST 4321

extern const unsigned short int *__ctype_b;
extern const unsigned int *__ctype32_b;
extern const unsigned int *__ctype_tolower;
extern const unsigned int *__ctype_toupper;
extern const unsigned int *__ctype32_tolower;
extern const unsigned int *__ctype32_toupper;

typedef unsigned char __ctype_touplow_t;

typedef unsigned short int __uint16_t;
typedef __uint16_t __ctype_mask_t;

#ifndef _ISbit
#define _ISbit(bit) (1 << (bit))
enum
{
    _ISupper = _ISbit (0),    /* UPPERCASE.  */
    _ISlower = _ISbit (1),    /* lowercase.  */
    _ISalpha = _ISbit (2),    /* Alphabetic.  */
    _ISdigit = _ISbit (3),    /* Numeric.  */
    _ISxdigit = _ISbit (4),   /* Hexadecimal numeric.  */
    _ISspace = _ISbit (5),    /* Whitespace.  */
    _ISprint = _ISbit (6),    /* Printing.  */
    _ISgraph = _ISbit (7),    /* Graphical.  */
    _ISblank = _ISbit (8),    /* Blank (usually SPC and TAB).  */
    _IScntrl = _ISbit (9),    /* Control character.  */
    _ISpunct = _ISbit (10),   /* Punctuation.  */
    _ISalnum = _ISbit (11)    /* Alphanumeric.  */
};
#else
#error _ISbit already defined!
#endif


#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
#endif
