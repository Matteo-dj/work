//*****************************************************************************
//
//                  版权所有 (C), 1998-2011, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : app_res_cfg.h
//  版 本 号   : V1.0
//  作    者   : shenhankun/00130424
//  生成日期   : 2011-10-18
//  功能描述   : TODO: ...
//
//  函数列表   : TODO: ...
//  修改历史   :
//  1.日    期 : 2011-10-18
//    作    者 : shenhankun/00130424
//    修改内容 : 创建文件
//
//*****************************************************************************
#ifndef __HI_MDM_H__
#error "please include hi_mdm.h instead of this file"
#endif

#define SAL_DUMMY()

//////////////////////////////////////////////////////////////////////////
// 任务配置
// 注意枚举顺和 TASK_CFG_DEFINED/INIT_PROC_CFG_DEFINED 配置定义行需要严格一致
//////////////////////////////////////////////////////////////////////////
#ifdef SAL_HAVE_RES_CFG_TYPES
// 任务ID定义, 必须使用默认赋值方式, 即起始为0, 且所有值保持连续
typedef enum
{
    EN_APP_TASK_APP_MRS,   // 集中器/电表/采集器的协议处理

#ifdef PRODUCT_CFG_PRODUCT_TYPE_NDM
    EN_APP_TASK_NDM_BQ_UPG,
#endif
    // TODO ...

    EN_APP_TASK_MAX
} APP_TASK_ID_ENUM;
#endif // SAL_HAVE_RES_CFG_TYPES
#ifdef SAL_HAVE_TASK_STACK_GLOBALS
#include <hi_res_types.h>
// 按顺序执行初始化入口 、任务体


//  TODO:在此添加其它的初始化、任务

#endif // SAL_HAVE_TASK_STACK_GLOBALS
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// 消息队列配置
// 注意枚举顺和配置定义行严格一致
//////////////////////////////////////////////////////////////////////////
#ifdef SAL_HAVE_RES_CFG_TYPES
// 队列ID定义, 必须使用默认赋值方式, 即起始为0, 且所有值保持连续
typedef enum
{
    EN_APP_QUEUE_MRS,       // 抄表队列
#ifdef PRODUCT_CFG_PRODUCT_TYPE_NDM
    EN_APP_QUEUE_NDM_BQ_UPG,
#endif
    // TODO ...

    EN_APP_Q_MAX
} APP_QUEUE_ID_ENUM;
#endif // SAL_HAVE_RES_CFG_TYPES
#ifdef SAL_HAVE_QUEUE_GLOBALS
#include <hi_res_types.h>
// 队列大小定义
SAL_Q_DEFINED(APP_QUEUE_MRS, PRODUCT_CFG_Q_APP_MRS_MSG_NUM)
#ifdef PRODUCT_CFG_PRODUCT_TYPE_NDM
SAL_Q_DEFINED(APP_NDM_BQ_UPG, PRODUCT_CFG_Q_APP_NDM_BQUPG_MSG_NUM)
#endif
//  TODO:  在此添加其它的消息队列

#endif // SAL_HAVE_QUEUE_GLOBALS
//////////////////////////////////////////////////////////////////////////


#ifdef SAL_HAVE_RES_CFG_TYPES
//////////////////////////////////////////////////////////////////////////
// 信号量配置
// 注意枚举顺和配置定义行严格一致
//////////////////////////////////////////////////////////////////////////
typedef enum
{
    EN_APP_SEM_1,
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
	EN_APP_SEM_2,
	EN_APP_SEM_3,
#endif
    EN_APP_SEM_MAX
} APP_SEM_ENUM;
#ifdef PRODUCT_CFG_PRODUCT_TYPE_CCO
#define APP_SEM_NAME_DEFINE {\
    "APP_1_SEM", \
    "APP_2_SEM", \
    "APP_3_SEM", \
    }
#else
#define APP_SEM_NAME_DEFINE {\
    "APP_1_SEM", \
    }
#endif // PRODUCT_CFG_PRODUCT_TYPE_CCO
#endif // SAL_HAVE_RES_TYPES

#ifdef SAL_HAVE_RES_CFG_TYPES
//////////////////////////////////////////////////////////////////////////
// TASK Timer 配置
//////////////////////////////////////////////////////////////////////////
typedef enum
{
    EN_APP_TASK_TIMER_MRS_1,
    EN_APP_TASK_TIMER_MRS_2,
    EN_APP_TASK_TIMER_MRS_3,
    EN_APP_TASK_TIMER_MRS_4,
    EN_APP_TASK_TIMER_MRS_5,
    EN_APP_TASK_TIMER_MRS_6,
    EN_APP_TASK_TIMER_MRS_7,
    EN_APP_TASK_TIMER_MRS_8,
    EN_APP_TASK_TIMER_MRS_9,
    EN_APP_TASK_TIMER_MRS_10,
    EN_APP_TASK_TIMER_MRS_11,
    EN_APP_TASK_TIMER_MRS_12,
    EN_APP_TASK_TIMER_MRS_13,
    EN_APP_TASK_TIMER_MRS_14,
    EN_APP_TASK_TIMER_MRS_15,
    EN_APP_TASK_TIMER_MRS_16,
    EN_APP_TASK_TIMER_MRS_17,
    EN_APP_TASK_TIMER_MRS_18,
    EN_APP_TASK_TIMER_MRS_19,
    EN_APP_TASK_TIMER_MRS_20,
    EN_APP_TASK_TIMER_MRS_21,
    EN_APP_TASK_TIMER_MRS_22,
    EN_APP_TASK_TIMER_MRS_23,
    EN_APP_TASK_TIMER_MRS_24,
    EN_APP_TASK_TIMER_MRS_25,
    EN_APP_TASK_TIMER_MRS_26,
    EN_APP_TASK_TIMER_MRS_27,
    EN_APP_TASK_TIMER_MRS_28,
    EN_APP_TASK_TIMER_MRS_29,
    EN_APP_TASK_TIMER_MRS_30,
    EN_APP_TASK_TIMER_MRS_31,
    EN_APP_TASK_TIMER_MRS_32,
    EN_APP_TASK_TIMER_MRS_33,
    EN_APP_TASK_TIMER_MRS_34,
    EN_APP_TASK_TIMER_MRS_35,
    EN_APP_TASK_TIMER_MRS_36,
    EN_APP_TASK_TIMER_MRS_37,
    EN_APP_TASK_TIMER_MRS_38,
    EN_APP_TASK_TIMER_MRS_39,
    EN_APP_TASK_TIMER_MRS_40,
#ifdef PRODUCT_CFG_PRODUCT_TYPE_CCO
	EN_APP_TASK_TIMER_MRS_41,
	EN_APP_TASK_TIMER_MRS_42,
    EN_APP_TASK_TIMER_MRS_43,
    EN_APP_TASK_TIMER_MRS_44,
    EN_APP_TASK_TIMER_MRS_45,
    EN_APP_TASK_TIMER_MRS_46,
    EN_APP_TASK_TIMER_MRS_47,
    EN_APP_TASK_TIMER_MRS_48,
    EN_APP_TASK_TIMER_MRS_49,
    EN_APP_TASK_TIMER_MRS_50,
    EN_APP_TASK_TIMER_MRS_51,
    EN_APP_TASK_TIMER_MRS_52,
    EN_APP_TASK_TIMER_MRS_53,
    EN_APP_TASK_TIMER_MRS_54,
    EN_APP_TASK_TIMER_MRS_55,
    EN_APP_TASK_TIMER_MRS_56,
    EN_APP_TASK_TIMER_MRS_57,
    EN_APP_TASK_TIMER_MRS_58,
    EN_APP_TASK_TIMER_MRS_59,
    EN_APP_TASK_TIMER_MRS_60,
    EN_APP_TASK_TIMER_MRS_61,
    EN_APP_TASK_TIMER_MRS_62,
#endif
    EN_APP_TIMER_MAX
} APP_TIMER_ENUM;

#endif // SAL_HAVE_RES_TYPES

