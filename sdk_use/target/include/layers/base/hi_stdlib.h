/**@defgroup hct_stdlib 安全C
 * @ingroup os
 * 安全C接口主要是基于标准C接口增加了一个参数，即写操作地址空间上限，从而防止写越界。
 */
//****************************************************************************
//
//                  版权所有 (C), 2001-2011, 华为技术有限公司
//
//****************************************************************************
//  文 件 名   : hi_stdlib.h
//  版 本 号   : V1.0
//  生成日期   : 2017年04月14日
//  功能描述   : C库接口
//  函数列表   :
//****************************************************************************
#ifndef __HI_STDLIB_H__
#define __HI_STDLIB_H__

//****************************************************************************
#include <hi_types.h>
#include <hi_mdm_types.h>
#if !defined(MAC_V200_LLT)
#include <los_typedef.h>
#endif
HI_START_HEADER

//****************************************************************************
//*****************************************************************************
// 备注: 当前版本仅定义了安全C库接口分支，下个版本会增加C库函数分支
#ifndef errno_t
typedef int errno_t;
#endif

/**
* @ingroup  hct_stdlib
* @brief  复制源字符串到目的缓冲区。
*
* @par 描述:
* 复制源字符串到目的缓冲区。
* @attention 无。
*
* @param dest [OUT] 类型#char *。 目的缓冲区。
* @param dest_max [IN] 类型#size_t。 目的缓冲区大小。
* @param src [IN] 类型#const #char * 。源缓冲区。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败。
*
* @par 依赖:
* @li hi_stdlib.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
extern errno_t strcpy_s(char *dest, size_t dest_max, const char *src);

/**
* @ingroup  hct_stdlib
* @brief  复制指定长度源字符串到目的缓冲区。
*
* @par 描述:
* 复制指定长度源字符串到目的缓冲区。
* @attention 无。
*
* @param dest [IN] 类型#char *。 目的缓冲区。
* @param dest_max [IN] 类型#size_t。 目的缓冲区大小。
* @param src [IN] 类型#const #char * 。源缓冲区。
* @param count [IN] 类型#size_t。从源缓冲区中复制的字符数。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败。
*
* @par 依赖:
* @li hi_stdlib.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
extern errno_t strncpy_s(char *dest, size_t dest_max, const char *src, size_t count);

/**
* @ingroup  hct_stdlib
* @brief  将源字符串连接到目的字符串后面
*
* @par 描述:
* 复制指定长度源字符串到目的缓冲区。
* @attention 无。
*
* @param dest [IN] 类型#char *。 目的缓冲区。
* @param dest [IN] 类型#size_t。 目的缓冲区大小。
* @param src [IN] 类型#const #char * 。源缓冲区。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败。
*
* @par 依赖:
* @li hi_stdlib.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
extern errno_t strcat_s(char *dest, size_t dest_max, const char *src);

/**
* @ingroup  hct_stdlib
* @brief  将指定长度的源字符串连接到目的字符串后面。
*
* @par 描述:
* 将指定长度的源字符串连接到目的字符串后面。
* @attention 无。
*
* @param dest [IN] 类型#char *。 目的缓冲区。
* @param dest_max [IN] 类型#size_t。 目的缓冲区大小。
* @param src [IN] 类型#const #char * 。源缓冲区。
* @param count [IN] 类型#size_t。源缓冲区中要连接的字符数。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败。
*
* @par 依赖:
* @li hi_stdlib.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
extern errno_t strncat_s(char *dest, size_t dest_max, const char *src, size_t count);

/**
* @ingroup  hct_stdlib
* @brief  复制源缓冲区的数据到目的缓冲区。
*
* @par 描述:
* 复制源缓冲区的数据到目的缓冲区。
* @attention 无。
*
* @param dest [IN] 类型#void *。 目的缓冲区。
* @param dest_max [IN] 类型#size_t。 目的缓冲区大小。
* @param src [IN] 类型#const #void * 。源缓冲区。
* @param count [IN] 类型#HI_U32。从源缓冲区中复制字符数。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败。
*
* @par 依赖:
* @li hi_stdlib.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
extern errno_t memcpy_s(void *dest, size_t dest_max, const void *src, size_t count);

/**
* @ingroup  hct_stdlib
* @brief  设置目的缓冲区为特定值。
*
* @par 描述:
* 设置目的缓冲区为特定值。
* @attention 无。
*
* @param dest [IN] 类型#void *。 目的缓冲区。
* @param dest_max [IN] 类型#size_t。 目的缓冲区大小。
* @param c [IN] 类型#int 。要设置目的缓冲区中的字符。
* @param count [IN] 类型#size_t。要设置目的缓冲区的符数个数。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败。
*
* @par 依赖:
* @li hi_stdlib.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
extern errno_t memset_s(void *dest, size_t dest_max, int c, size_t count);

/**
* @ingroup  hct_stdlib
* @brief  移动源缓冲区的数据到目的缓冲区。
*
* @par 描述:
* 移动源缓冲区的数据到目的缓冲区。
* @attention 无。
*
* @param dest [IN] 类型#void *。 目的缓冲区。
* @param dest_max [IN] 类型#size_t。 目的缓冲区大小。
* @param src [IN] 类型#const #void* 。源缓冲区。
* @param count [IN] 类型#size_t。从源缓冲区中移动的字符数。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败。
*
* @par 依赖:
* @li hi_stdlib.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
extern errno_t memmove_s(void *dest, size_t dest_max, const void *src, size_t count);

/**
* @ingroup  hct_stdlib
* @brief  将字符串按照指定的分隔符分割成子字符串。
*
* @par 描述:
* 移动源将字符串按照指定的分隔符分割成子字符串。
* @attention 无。
*
* @param token [IN] 类型#char *。 要分割的字符串。
* @param delimit [IN] 类型#const char *。 字符串分隔符。
* @param context [IN] 类型#char* 。保存调用HI_strtok_s后的位置信息。
*
* @retval #char*类型指针 指向在token中的下一个token。
* @retval #HI_NULL 没有找到指定的子字符串或者发生错误。
*
* @par 依赖:
* @li hi_stdlib.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
extern char *strtok_s(char *token, const char *delimit, char **context);

/**
* @ingroup  hct_stdlib
* @brief  将数据格式化输出到目的缓冲区。
*
* @par 描述:
* 将数据格式化输出到目的缓冲区。
* @attention 无。
*
* @param dest [IN] 类型#char *。 目的缓冲区。
* @param dest_max [IN] 类型#size_t。 目的缓冲区大小。
* @param format [IN] 类型#const #char *。 格式化控制字符串。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 错误码。
*
* @par 依赖:
* @li hi_stdlib.h: 该接口声明所在的头文件。
* @see 无。
* @since HiMDL_V200R001C00
*/
extern int sprintf_s(char *dest, size_t dest_max, const char *format, ...);

/**
* @ingroup  hct_stdlib
* @brief 将数据按照指定长度格式化输出到目的缓冲区。
*
* @par 描述:
* 将数据按照指定长度格式化输出到目的缓冲区。
* @attention 无。
*
* @param dest [OUT] 类型#char *。 目的缓冲区。
* @param dest_max [IN] 类型#size_t。 目的缓冲区大小。
* @param size_t [IN] 类型#size_t。 要输出到目的缓冲区的格式化字符个数。
* @param format [IN] 类型#const #char *。 格式化控制字符串。
* @param ... [IN] 可选参数。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 错误码。
*
* @par 依赖:
* @li hi_stdlib.h: 该接口声明所在的头文件。
* @see 无。
* @since HiMDL_V200R001C00
*/
extern int snprintf_s(char *dest, size_t dest_max, size_t count, const char *format, ...);

/*
非安全接口
*/

/**
* @ingroup  hct_stdlib
* @brief  计算字符串长度。
*
* @par 描述:
* 计算字符串长度。
* @attention 无。
*
* @param str [IN] 类型#const #char *。 字符串。
*
* @retval #HI_U32类型数值 字符串长度。
*
* @par 依赖:
* @li hi_stdlib.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
extern size_t strlen(const char *str);

/**
* @ingroup  hct_stdlib
* @brief 比较 字符串是否相同。
*
* @par 描述:
* 比较 字符串是否相同。
* @attention 无。
*
* @param buf1 [IN] 类型#const #void *。 字符串1。
* @param buf2 [IN] 类型#const #void *。 字符串2。
* @param count [IN] 类型#size_t。 待比较的长度。
*
* @retval 0 两字符串相等。
* @retval 非0 两字符串不相等。
*
* @par 依赖:
* @li hi_stdlib.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
extern int memcmp(const void *buf1, const void *buf2, size_t count);

/**
* @ingroup  hct_stdlib
* @brief 将参数nptr字符串根据参数base来转换成无符号的长整型数。
*
* @par 描述:
* 将参数nptr字符串根据参数ibase来转换成无符号的长整型数。
* @attention 无。
*
* @param nptr [IN] 类型#const #char *。要转换的字符串。
* @param endptr [IN] 类型#char **。 第一个不能转换的字符的指针。
* @param base [IN] 类型#int。字符串 str 所采用的进制。
*
* @retval unsigned long型数值 转换后的无符号长整型数。
*
* @par 依赖:
* @li hi_stdlib.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
extern unsigned long strtoul(const char *nptr, char **endptr, int base);

/**
* @ingroup  hct_stdlib
* @brief 判断字符串s2是否是s1的子串。
*
* @par 描述:
* 判断字符串s2是否是s1的子串。
* @attention 无。
*
* @param s1 [IN] 类型#const #char *。源字符串。
* @param s2 [IN] 类型#const #char *。子字符串。
*
* @retval #char *指针 s2在s1中首次出现的地址。
* @retval #HI_NULL 不是子串。
*
* @par 依赖:
* @li hi_stdlib.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
#ifndef MAC_V200_LLT
extern char *strstr(const char *s1, const char *s2);
#endif

/**
* @ingroup  hct_stdlib
* @brief 在s1的前len个字符中查找s2。
*
* @par 描述:
* 在s1的前len个字符中查找s2。
* @attention 无。
*
* @param s1 [IN] 类型#const #char *。源字符串。
* @param s2 [IN] 类型#const #char *。子字符串。
* @param len [IN] 类型#size_t。在前len个字符中查找。
*
* @retval #char *指针 s2在s1中首次出现的地址。
* @retval #HI_NULL 没有找到，不是子串。
*
* @par Dependency:
* @li hi_stdlib.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
extern char *strnstr(const char *s1, const char *s2, size_t len);

/**
* @ingroup  hct_stdlib
* @brief 在源字符串中查找字符。
*
* @par 描述:
* 在源字符串中查找字符。
* @attention
* @li 待查找的字符为#int型，接口将其转换为字符后再查找。
* @li 源字符串中的结束符'\0'也为待查找字符。
*
* @param s1 [IN] 类型#const #char *。源字符串。
* @param s2 [IN] 类型#int。待查找字符。
*
* @retval #HI_CHAR 找到该字符，返回字符在源字符串中的地址。
* @retval #HI_NULL 没有找到。
*
* @par 依赖:
* @li hi_stdlib.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
#ifndef MAC_V200_LLT
extern char *strchr(const char *s1, int s2);
#endif

extern int atoi(const char *str);

HI_END_HEADER

#endif //__HI_STDLIB_H__

