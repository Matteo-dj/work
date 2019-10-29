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

#ifndef _LOS_CONFIG_H
#define _LOS_CONFIG_H

#include "los_typedef.h"
#include "hisoc/clock.h"
#include "los_tick.h"
#include "board.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

typedef UINT32 (*user_get_timebase_us)(UINT32 * time);
typedef UINT32 (*user_get_timebase_ms)(UINT32 * time);
typedef UINT32 (*user_get_timebase_s)(UINT32 * time);

extern user_get_timebase_us  g_func_get_cur_us;
extern user_get_timebase_ms  g_func_get_cur_ms;
extern user_get_timebase_s   g_func_get_cur_s;
/**
 * @ingroup los_config
 * int stack start addr
 */
extern char  __int_stack_start;

/****************************** 配置系统时钟模块 ****************************/

/**
 * @ingroup los_config
 * 系统时钟，单位:HZ
 */
#define OS_SYS_CLOCK                           25000000                  //主频,单位：Hz

/**
 * @ingroup los_config
 * timer1 clock (unit: HZ)
 */
#define OS_TIME_TIMER_CLOCK                       OS_SYS_CLOCK

/**
* @ingroup los_config
* limit addr range when search for  'func local(frame pointer)' or 'func name'
*/
#define OS_SYS_FUNC_ADDR_START      (unsigned int)&__int_stack_start
#define OS_SYS_FUNC_ADDR_END        g_sys_mem_addr_end

/**
* @ingroup los_config
* .int stack size
*/
#define OS_UNDEF_STACK_SIZE     32
#define OS_ABT_STACK_SIZE   32
#define OS_IRQ_STACK_SIZE   64
#define OS_FIQ_STACK_SIZE   64
#define OS_SVC_STACK_SIZE   1024

/**
* @ingroup los_config
* .startup stack size
*/
#define OS_STARTUP_STACK_SIZE   512

/**
 * @ingroup los_config
 * 每秒的tick数，单位:个
 */
#define LOSCFG_BASE_CORE_TICK_PER_SECOND                     100
#define CYCLES_PER_TICK (OS_SYS_CLOCK / LOSCFG_BASE_CORE_TICK_PER_SECOND)
#define MS_PER_TICK (OS_SYS_MS_PER_SECOND / LOSCFG_BASE_CORE_TICK_PER_SECOND)
#define TICKS_MAX (0xFFFFFFFF/CYCLES_PER_TICK)

/**
 * @ingroup los_config
 * 外置定时器裁剪开关
 */
#define LOSCFG_BASE_CORE_TICK_HW_TIME                  YES

#ifdef LOSCFG_KERNEL_TICKLESS
/**
 * @ingroup los_config
 * Number of minimum sleep ticks when system enter tickless mode
 */
#define LOSCFG_MIN_SLEEP_TICKS                    10
#endif

/****************************** 配置硬中断模块 ******************************/
#define OS_MEM_WATERLINE  YES

/**
 * @ingroup los_config
 * 硬中断裁剪开关
 */
#define LOSCFG_PLATFORM_HWI                         YES

/**
 * @ingroup los_config
 * 硬中断使用最大数，包含tick定时器中断
 */
#define LOSCFG_PLATFORM_HWI_LIMIT                    32              // 硬中断使用最大数，包含tick定时器中断

/****************************** 配置任务模块 ********************************/

/**
 * @ingroup los_config
 * Minimum stack size.
 *
 * 0x400 bytes, aligned on a boundary of 4.
 */
#define LOS_TASK_MIN_STACK_SIZE                     (ALIGN(0x400, 4))

/**
 * @ingroup los_config
 * 缺省的任务优先级
 */
#define LOSCFG_BASE_CORE_TSK_DEFAULT_PRIO                10

#define LOSCFG_BASE_CORE_TSK_MAX                         64
/**
 * @ingroup los_config
 * 默认支持的任务数，包含IDLE任务
 */
#define LOSCFG_BASE_CORE_TSK_DEFAULT_NUM                         32

/**
 * @ingroup los_config
 * IDLE任务栈的大小
 */
#define LOSCFG_BASE_CORE_TSK_IDLE_STACK_SIZE                 SIZE(0x400)

/**
 * @ingroup los_config
 * 缺省的任务栈大小
 */
#define LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE          SIZE(0x1000)      //SIZE(0x6000)

/**
 * @ingroup los_config
 * 任务Robin模块裁剪开关,相同优先级时间片轮转
 */
#define LOSCFG_BASE_CORE_TIMESLICE                        YES

/**
 * @ingroup los_config
 * 任务同优先级任务执行的最长时间
 */
#define LOSCFG_BASE_CORE_TIMESLICE_TIMEOUT                        1   //单位tick

/**
 * @ingroup los_config
 * 任务(栈)监控模块裁剪开关
 */
#define LOSCFG_BASE_CORE_TSK_MONITOR                          YES

/**
 * @ingroup los_config
 * Configuration item for task perf task filter hook
 */
#define OS_PERF_TSK_FILTER                      NO    //本版本不支持

/****************************** 配置信号量模块 ******************************/

/**
 * @ingroup los_config
 * 信号量模块裁剪开关
 */
#define LOSCFG_BASE_IPC_SEM                         YES

/**
 * @ingroup los_config
 * 默认支持的信号量数
 */
#define LOSCFG_BASE_IPC_SEM_DEFAULT_NUM               100

/****************************** 配置互斥锁模块 ******************************/

/**
 * @ingroup los_config
 * 互斥锁模块裁剪开关
 */
#define LOSCFG_BASE_IPC_MUX                         YES

/**
 * @ingroup los_config
 * 默认支持的互斥锁数
 */
#define LOSCFG_BASE_IPC_MUX_DEFAULT_NUM                50
/****************************** 配置队列模块 ********************************/

/**
 * @ingroup los_config
 * 队列模块裁剪开关
 */
#define LOSCFG_BASE_IPC_QUEUE                       YES

#define LOSCFG_BASE_IPC_QUEUE_MAX                   64
/**
 * @ingroup los_config
 * 默认支持的队列数
 */
#define LOSCFG_BASE_IPC_QUEUE_DEFAULT_NUM                32


/****************************** 配置软件定时器模块 **************************/
#if (LOSCFG_BASE_IPC_QUEUE == YES)

/**
 * @ingroup los_config
 * 软件定时器模块裁剪开关
 */
#define LOSCFG_BASE_CORE_SWTMR                       YES

/**
 * @ingroup los_config
 * 默认支持的软件定时器数
 */
#define LOSCFG_BASE_CORE_SWTMR_DEFAULT_NUM               80
/**
 * @ingroup los_config
 * Max number of software timers ID
 */
#define OS_SWTMR_MAX_TIMERID                         ((65535/LOSCFG_BASE_CORE_SWTMR_LIMIT) * LOSCFG_BASE_CORE_SWTMR_LIMIT)
/**
 * @ingroup los_config
 * 软件定时器队列最大长度
 */
#define OS_SWTMR_HANDLE_QUEUE_SIZE             (LOSCFG_BASE_CORE_SWTMR_LIMIT + 0)
#endif

/****************************** 配置工作队列模块 **************************/
#define LOSCFG_COMPAT_LINUX_WORKQ_DEFAULT_NUM              256

/****************************** 配置内存模块 **************************/

/**
 * @ingroup los_config
 * 内存起始地址
 */
#define OS_SYS_MEM_ADDR                         &m_aucSysMem0[0]

/**
 * @ingroup los_config
 * 内存大小
 */
#define OS_SYS_MEM_SIZE                           (g_sys_mem_addr_end - OS_SYS_NOCACHEMEM_SIZE - ((((unsigned long)&__bss_end) + (64 - 1)) & ~(64 - 1)))


/****************************** 配置内存模块 **************************/

/**
 * @ingroup los_config
 * 非对齐内存大小
 */
#define OS_SYS_NOCACHEMEM_SIZE              0x0
#if OS_SYS_NOCACHEMEM_SIZE

/**
 * @ingroup los_config
 * 非对齐内存起始地址
 */
#define OS_SYS_NOCACHEMEM_ADDR              &m_aucSysNoCacheMem0[0]
#endif
//#define OS_SYS_NOCACHEMEM_CHECK               NO
//#define OS_SYS_NOCACHEMEM_NUM                 20

/****************************** 配置fw Interface **************************/

/**
 * @ingroup los_config
 * 任务通讯监测开关 ，消息队列调试开关，需要使用cmsis接口才支持
 */
#define LOSCFG_COMPAT_CMSIS_FW                         YES

/**
 * @ingroup los_config
 * Version number
 */
#define _T(x)                                   x
#define HW_LITEOS_SYSNAME                       "Huawei LiteOS"
#define HW_LITEOS_SEP                           " "
#define _V(v)                                   _T(HW_LITEOS_SYSNAME)_T(HW_LITEOS_SEP)_T(v)

#define HW_LITEOS_VERSION                       "V200R001C10B013"
#define VER                                     _V(HW_LITEOS_VERSION)

/**
 * @ingroup los_config
 * The Version number of Public
 */
#define HW_LITEOS_OPEN_VERSION                 "2.2.4"
#define OPEN_VER                               _V(HW_LITEOS_OPEN_VERSION)

/**
 * @ingroup los_config
 * hi_uart.c hi3911 代码开关
 */
#define CONFIG_HI3911

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */


#endif /* _LOS_CONFIG_H */

