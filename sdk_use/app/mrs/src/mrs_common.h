//*****************************************************************************
//
//                  版权所有 (C), 2001-2011, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_common.h
//  版 本 号   : V1.0
//  作    者   : 沈汉坤/00130424
//  生成日期   : 2011年09月06日
//  功能描述   : MRS内部共用的接口定义
//  函数列表   : NA
//  修改历史   :
//  1.日    期 : 2011年09月06日
//    作    者 : 沈汉坤/00130424
//    修改内容 : 创建文件
//*****************************************************************************


#ifndef __MRS_COMMON_H__
#define __MRS_COMMON_H__
#include <app_common.h>
#include "app_config.h"
#include "mrs_fw.h"
#include "mrs_time.h"

HI_START_HEADER

typedef struct
{
    HI_U16 usDatagramSize; // 报文长度,单位字节
    HI_U16 reserved;
    HI_PBYTE pucDatagram;  // 报文buffer
} MRS_ONE_RAW_FRAME_STRU;

//*****************************************************************************
// 函数名称: mrsSrvStart mrsSrvStop
// 功能描述: MRS 的SRV 初始化(模块启动和停止)，包括SRV的上下文, 资源申请等
//
// 参数说明:
//  This function has no parameters.
//
// 返 回 值:
//    TODO: ...
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: shenhankun/00130424 [2011-11-10]
//*****************************************************************************
HI_U32 mrsSrvStart(HI_VOID);
HI_U32 mrsSrvStop(HI_VOID);

HI_U32 mrsDfxInit(HI_VOID);

// 初始化
HI_VOID mrsCcoGlobalInit(HI_VOID);
HI_VOID mrsCcoInitCtx(HI_VOID);
HI_VOID mrsStaInitCtx(HI_VOID);
HI_VOID mrsStaGlobalInit(HI_VOID);

//工装读取ID
HI_U32 mrsReadIDProc(EQUIP_CHIP_ID_R_STRU* idInfo);
HI_U32 mrsReadMac(HI_U8 *);

// 供获取设备各类ID的功能函数调用
HI_U32 mrsGetDevIdChip(HI_PBYTE pid_info, HI_U16 length);
HI_U32 mrsGetDevIdDev(HI_PBYTE pid_info, HI_U16 length);

HI_END_HEADER
#endif // __MRS_COMMON_H__
