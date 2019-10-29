/*******************************************************************************
* Copyright @ Huawei Technologies Co., Ltd. 1998-2014. All rights reserved.
* File name: securec.h
* Decription:
*             the user of this secure c library should include this header file
*             in you source code. This header file declare all supported API
*             prototype of the library, such as memcpy_s, strcpy_s, wcscpy_s,
*             strcat_s, strncat_s, sprintf_s, scanf_s, and so on.
* History:
*     1. Date:
*         Author:
*         Modification:
********************************************************************************
*/

#ifndef __SECUREC_H__5D13A042_DC3F_4ED9_A8D1_882811274C27
#define __SECUREC_H__5D13A042_DC3F_4ED9_A8D1_882811274C27

#include "string.h"


#ifndef TWO_MIN
#define TWO_MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

#define LIKELY(x)   (x)
#define UNLIKELY(x) (x)

#define SECUREC_MEM_MAX_LEN     (0x7fffffffUL)
#define SECUREC_STRING_MAX_LEN  (0x7fffffffUL)

#define BOOT_SUCCESS    (0)
#define BOOT_FAILURE    (-1)

#ifndef NULL
#define NULL	0
#endif

typedef unsigned char UINT8T;
/* 类型定义 */
typedef unsigned int size_t;
typedef int errno_t;
typedef void hi_void;
typedef int hi_s32;
typedef char hi_char;

errno_t memcpy_s(hi_void* dest, size_t destMax, const hi_void* src, size_t count);
errno_t memset_s(hi_void* dest, size_t destMax, hi_s32 c, size_t count);
errno_t strncpy_s(void* dest, size_t destMax, const void* src, size_t count);


#endif //__SECUREC_H__5D13A042_DC3F_4ED9_A8D1_882811274C27


