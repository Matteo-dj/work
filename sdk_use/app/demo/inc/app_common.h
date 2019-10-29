//*****************************************************************************
//
//                  版权所有 (C), 2001-2011, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : app_common.h
//  版 本 号   : V1.0 
//  作    者   : 沈汉坤/00130424
//  生成日期   : 2011年09月06日
//  功能描述   : ASL共用接口定义
//  函数列表   : 无
//  修改历史   : 
//  1.日    期 : 2011年09月06日
//    作    者 : 沈汉坤/00130424
//    修改内容 : 创建文件 
//*****************************************************************************

#ifndef __APP_COMMON_H__
#define __APP_COMMON_H__

//*****************************************************************************
#include <hi_ft_nv.h>
#include <hi_mdm.h>

#include "app_config.h"
#include "dfx_sys.h"
#include "dfx_usr.h"
#include "hi_driver.h"
//#include "plc/os/hi_ssi.h"
//*****************************************************************************
#include <app_res_cfg.h>


typedef struct
{
    HI_U32 aulTaskId[EN_APP_TASK_MAX];
    HI_U32 aulQueueId[EN_APP_Q_MAX];
    HI_TIMER_HANDLE_S astTimerHandle[EN_APP_TIMER_MAX];
    HI_U32 aulSemId[EN_APP_SEM_MAX];
} DEMO_RES_CFG_S;

HI_EXTERN DEMO_RES_CFG_S g_stDemoOsRes;


//*****************************************************************************
// CTS_PRINT0 常量上报 
// CTS_PRINT1 变参1 字符串支持最大128字节
//
//*****************************************************************************


//*****************************************************************************

//*****************************************************************************

HI_VOID demo_fast_send_init(HI_VOID);
HI_VOID DEMO_MainTaskBody(HI_U32 unused);
HI_PRV HI_U32 demo_ObjUsrInProc(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption);

#endif // __APP_COMMON_H__

