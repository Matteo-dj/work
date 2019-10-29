//*****************************************************************************
//
//                  版权所有 (C), 1998-2015, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_dfx_cltii_event.h
//  版 本 号   : V1.0 
//  作    者   : cuiate/00233580
//  生成日期   : 2015-07-21
//  功能描述   : II采下挂电表事件维测信息 函数及接口声明
//               
//  函数列表   : TODO: ...
//  修改历史   : 
//  1.日    期 : 2015-07-21
//    作    者 : cuiate/00233580
//    修改内容 : 创建文件 
//
//*****************************************************************************

#ifndef _MRS_DFX_CLTII_EVENT_H_
#define _MRS_DFX_CLTII_EVENT_H_
HI_START_HEADER
#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)


HI_U32 mrsDfxGetCltiiEventStatus(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption);

HI_U32 mrsDfxGetCltiiEventInfo(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption);

HI_U32 mrsDfxCltiiEventInit(HI_VOID);

HI_VOID mrsDfxCltiiEventMonitorMeterInf(HI_U8 *pucMeter, HI_U8 ucProtocol);

HI_VOID mrsDfxCltiiEventMonitorStart(HI_VOID);

HI_VOID mrsDfxCltiiEventMonitorEnd(HI_VOID);

HI_VOID mrsDfxCltiiEventLastCount(HI_U8 ucCount);

HI_VOID mrsDfxCltiiEventLastReported(HI_U8 ucCount);

HI_VOID mrsDfxCltiiEventLastCleared(HI_VOID);

#endif // defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
HI_END_HEADER
#endif


