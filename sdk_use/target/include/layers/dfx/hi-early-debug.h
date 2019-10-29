
/**@defgroup hct_print 打印  
 * @ingroup dfx
 */

/** 
* @file hi-early-debug.h
* 
*  Copyright (C) HiSilicon Technologies Co., Ltd. 2017. All rights reserved.  \n
* 
* 描述：该文件描述串口调试打印接口。\n
* @li 串口调试打印接口默认通过UART1打印，通信参数:波特率115200，数据位8位，无校验，1位停止位。
* @li 用户可以通过修改BOOTUP_UART_BASE选择用哪个调试串口进行打印。
* @li 使用串口调试助手即可接收到打印信息，HSO的console无法显示打印的信息。
* @li 用户可以通过edb_set_print_level修改打印等级，决定是否进行打印。系统默认在初始化完成之后将打印优先级修改为0，在发生死机时将打印优先级修改为1。
* @li 串口打印为耗时操作，因此请不要在正常业务代码(特别是频繁调用的流程)中调用串口打印接口。
* @li printf，dprintf接口，后续可能增加信号量保护，因此禁止在中断和关中断时调用printf，dprintf接口。
*
*/

#ifndef __HI_EARLY_DEBUG_H__
#define __HI_EARLY_DEBUG_H__

/**
* @ingroup  hct_print
* @brief  打印字符串
*
* @par 描述:
* 打印字符串，不带换行符号。
* @attention 
* @li 该接口支持在锁任务上下文中使用，但串口打印是耗时操作，会降低任务调度实时性。
* @li 该接口支持在LISR上下文中使用，但串口打印是耗时操作，会降低任务和中断调度实时性。
* @li 该接口支持在关中断上下文中使用，但串口打印是耗时操作，会降低任务和中断调度实时性。
*
* @param str [IN] 字符串。
*
* @par Dependency:
* @li hi-early-debug.h: 该接口声明所在的头文件。
* @see edb_put_str_p0
* @since HiMDL_V200R001C00
*/
extern void edb_put_str_only(const char *str);

/**
* @ingroup  hct_print
* @brief  打印字符串。
*
* @par 描述:
* 打印字符串，带换行符。
* @attention 
* @li 该接口支持在锁任务上下文中使用，但串口打印是耗时操作，会降低任务调度实时性。
* @li 该接口支持在LISR上下文中使用，但串口打印是耗时操作，会降低任务和中断调度实时性。
* @li 该接口支持在关中断上下文中使用，但串口打印是耗时操作，会降低任务和中断调度实时性。
*
* @param str [IN] 字符串。
*
* @par Dependency:
* @li hi-early-debug.h: 该接口声明所在的头文件。
* @see edb_put_str_only
* @since HiMDL_V200R001C00
*/
extern void edb_put_str_p0(const char *str);

/**
* @ingroup  hct_print
* @brief  打印字符串加一个整数。
*
* @par 描述:
* 打印字符串加一个整数，带换行。
* @attention 
* @li 该接口支持在锁任务上下文中使用，但串口打印是耗时操作，会降低任务调度实时性。
* @li 该接口支持在LISR上下文中使用，但串口打印是耗时操作，会降低任务和中断调度实时性。
* @li 该接口支持在关中断上下文中使用，但串口打印是耗时操作，会降低任务和中断调度实时性。
*
* @param str [IN] 字符串。
* @param p1 [IN] 第一个整数。
*
* @par Dependency:
* @li hi-early-debug.h: 该接口声明所在的头文件。
* @see 无
* @since HiMDL_V200R001C00
*/
extern void edb_put_str_p1(const char *str, const unsigned int p1);

/**
* @ingroup  hct_print
* @brief  打印字符串加两个整数。
*
* @par 描述:
* 打印字符串加两个整数，带换行。
* @attention 
* @li 该接口支持在锁任务上下文中使用，但串口打印是耗时操作，会降低任务调度实时性。
* @li 该接口支持在LISR上下文中使用，但串口打印是耗时操作，会降低任务和中断调度实时性。
* @li 该接口支持在关中断上下文中使用，但串口打印是耗时操作，会降低任务和中断调度实时性。
*
* @param str [IN] 字符串。
* @param p1 [IN] 第一个整数。
* @param p2 [IN] 第二个整数。
*
* @par Dependency:
* @li hi-early-debug.h: 该接口声明所在的头文件。
* @see 无
* @since HiMDL_V200R001C00
*/
extern void edb_put_str_p2(const char *str, const unsigned int p1, const unsigned int p2);

/**
* @ingroup  hct_print
* @brief  打印字符串加三个整数。
*
* @par 描述:
* 打印字符串加三个整数，带换行。
* @attention 
* @li 该接口支持在锁任务上下文中使用，但串口打印是耗时操作，会降低任务调度实时性。
* @li 该接口支持在LISR上下文中使用，但串口打印是耗时操作，会降低任务和中断调度实时性。
* @li 该接口支持在关中断上下文中使用，但串口打印是耗时操作，会降低任务和中断调度实时性。
*
* @param str [IN] 字符串。
* @param p1 [IN] 第一个整数。
* @param p2 [IN] 第二个整数。
* @param p3 [IN] 第三个整数。
*
* @par Dependency:
* @li hi-early-debug.h: 该接口声明所在的头文件。
* @see 无
* @since HiMDL_V200R001C00
*/
extern void edb_put_str_p3(const char *str, const unsigned int p1, const unsigned int p2, const unsigned int p3);

/**
* @ingroup  hct_print
* @brief  打印字符串加四个整数。
*
* @par 描述:
* 打印字符串加四个整数，带换行。
* @attention 
* @li 该接口支持在锁任务上下文中使用，但串口打印是耗时操作，会降低任务调度实时性。
* @li 该接口支持在LISR上下文中使用，但串口打印是耗时操作，会降低任务和中断调度实时性。
* @li 该接口支持在关中断上下文中使用，但串口打印是耗时操作，会降低任务和中断调度实时性。
*
* @param str [IN] 字符串。
* @param p1 [IN] 第一个整数。
* @param p2 [IN] 第二个整数。
* @param p3 [IN] 第三个整数。
* @param p4 [IN] 第四个整数。
*
* @par Dependency:
* @li hi-early-debug.h: 该接口声明所在的头文件。
* @see 无
* @since HiMDL_V200R001C00
*/
extern void edb_put_str_p4(const char *str, const unsigned int p1, const unsigned int p2, const unsigned int p3, const unsigned int p4);

/**
* @ingroup  hct_print
* @brief 打印BUF数据。
*
* @par 描述:
* 按照特定格式打印BUF。
* @attention 
* @li 该接口支持在锁任务上下文中使用，但串口打印是耗时操作，会降低任务调度实时性。
* @li 该接口支持在LISR上下文中使用，但串口打印是耗时操作，会降低任务和中断调度实时性。
* @li 该接口支持在关中断上下文中使用，但串口打印是耗时操作，会降低任务和中断调度实时性。
*
* @param buf [IN] buf指针。
* @param len [IN] buf大小。
*
* @par Dependency:
* @li hi-early-debug.h: 该接口声明所在的头文件。
* @see 无
* @since HiMDL_V200R001C00
*/
extern void edb_put_buf(const char *buf, const unsigned int len);

/**
* @ingroup  hct_print
* @brief 格式化打印。
*
* @par 描述:
* 格式化打印。
* @attention 函数中会有多于380Byte栈空间使用。
*
* @param fmt [IN] 格式化字符串。
* @param ... [IN] 格式化参数。
*
* @par Dependency:
* @li hi-early-debug.h: 该接口声明所在的头文件。
* @see 无
* @since HiMDL_V200R001C00
*/
extern int  printf(const char *fmt, ...);

/**
* @ingroup  hct_print
* @brief 格式化打印。
*
* @par 描述:
* 格式化打印。
* @attention 1.函数中会有多于380Byte栈空间使用。\n
*            2.废弃函数，建议客户使用printf。
* @param fmt [IN] 格式化字符串。
* @param ... [IN] 格式化参数。
*
* @par Dependency:
* @li hi-early-debug.h: 该接口声明所在的头文件。
* @see 无
* @since HiMDL_V200R001C00
*/

extern void dprintf(const char *fmt, ...);

/**
* @ingroup  hct_print
* @brief 设置打印优先级。
*
* @par 描述:
* 设置打印优先级。
* @attention 无。
*
* @param level [IN] 打印优先级。输入值为0:关闭打印功能；输入值为1:打开打印功能。
*
* @par Dependency:
* @li hi-early-debug.h: 该接口声明所在的头文件。
* @see 无
* @since HiMDL_V200R001C00
*/
extern void edb_set_print_level(unsigned int level);

#endif

