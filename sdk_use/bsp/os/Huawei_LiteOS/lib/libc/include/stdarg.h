/*----------------------------------------------------------------------------
 *      Huawei - Huawei LiteOS
 *----------------------------------------------------------------------------
 *      Name:    STDARG.H
 *      Purpose: stdarg header file.
 *      Rev.:    V1.00
 *----------------------------------------------------------------------------
 *

 * Copyright (c) 2014, Huawei Technologies. Co., Ltd.
 * All rights reserved.
 *---------------------------------------------------------------------------*/
#ifndef LIBC_STDARG_H
#define LIBC_STDARG_H

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */


#ifndef __va_list__
#define __va_list__
typedef __builtin_va_list __gnuc_va_list;
typedef __gnuc_va_list va_list;
#endif

#define va_start(v,l)     __builtin_va_start(v,l)
#define va_end(v)        __builtin_va_end(v)
#define va_arg(v,l)       __builtin_va_arg(v,l)
#define va_copy(d,s)    __builtin_va_copy(d,s)


#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif  /* LIBC_STDARG_H */
