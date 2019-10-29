//*****************************************************************************
//
//                  版权所有 (C), 1998-2011, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : os_scres_cfg.h
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

#define SAL_DUMMY()  

//////////////////////////////////////////////////////////////////////////
// 任务配置
// 注意枚举顺和 TASK_CFG_DEFINED/INIT_PROC_CFG_DEFINED 配置定义行需要严格一致
//////////////////////////////////////////////////////////////////////////
#ifdef SAL_HAVE_RES_CFG_TYPES
typedef enum
{    
    EN_SC_TASK_MAINMONITOR,
    EN_SC_TASK_EXPMONITOR,
    
    EN_SC_TASK_MAX
} SC_TASK_ID_ENUM;
#endif // SAL_HAVE_RES_CFG_TYPES

//////////////////////////////////////////////////////////////////////////
#if !defined(SAL_HAVE_NO_RES_DEFINED)
#ifdef SAL_HAVE_TASK_STACK_GLOBALS
#include <hi_res_types.h>

// 按顺序执行初始化入口 、创建 任务体 
SAL_DUMMY()
SC_TASK_CFG_DEFINED(SC_TASK_MAINMONITOR,  PRODUCT_CFG_TASK_STACK_SIZE_SC_MAINMONITOR, PRODUCT_CFG_TASK_PRIO_SC_MAINMONITOR, SAL_ScMainTaskInit,  SAL_ScMainTaskBody,  HI_DLL_LIB_NAME("sal"))
#if defined(PRODUCT_CFG_AUTO_RESET_SYSTEM_TIMEOUT) && defined(PRODUCT_CFG_TASK_STACK_SIZE_SC_EXPMONITOR) && defined(PRODUCT_CFG_TASK_PRIO_SC_EXPMONITOR)
SC_TASK_CFG_DEFINED(SC_TASK_EXPMONITOR,  PRODUCT_CFG_TASK_STACK_SIZE_SC_EXPMONITOR, PRODUCT_CFG_TASK_PRIO_SC_EXPMONITOR, SAL_ScExpWdMonitorTaskInit,  SAL_ScExpWdMonitorTaskBody,  HI_DLL_LIB_NAME("sal"))
#endif 
//SC_INIT_PROC_CFG_DEFINED(PHY_INIT         ,   0,      0,  PHY_Init ,  HI_NULL  ,  HI_DLL_LIB_NAME("phy"))
#endif // SAL_HAVE_TASK_STACK_GLOBALS
//////////////////////////////////////////////////////////////////////////
#endif // SAL_HAVE_NO_RES_DEFINED
//////////////////////////////////////////////////////////////////////////

