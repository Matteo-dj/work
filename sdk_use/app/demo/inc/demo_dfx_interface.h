//*****************************************************************************
//
//                  版权所有 (C), 2001-2011, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : cts_debug.h
//  版 本 号   : V1.0
//  作    者   :
//  生成日期   : 2014年05月09日
//  功能描述   :
//  函数列表   : 无
//  修改历史   :
//*****************************************************************************

#ifndef __CTS_DEBUG_H__
#define __CTS_DEBUG_H__

//*****************************************************************************
#include "app_common.h"


#define ID_DIAG_CMD_DRV_MEM_READ     0X3001
#define ID_DIAG_CMD_DRV_MEM_WRITE    0X3002

HI_VOID DemoDfxFastSendPeriodPolicyStart(HI_VOID);
HI_VOID DemoDfxFastSendPeriodPolicySTOP(HI_VOID);
HI_VOID DemoDfxFastSendSinglePolicy(HI_VOID);
HI_U32 mrsDfxOsm(HI_U16 usId, HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption);
HI_U32 mrsDfxOssm(HI_U16 usId, HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption);


#endif // __CTS_DEBUG_H__

