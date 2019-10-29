//*****************************************************************************
//
//                  版权所有 (C), 2001-2014, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_srv_cco_csg_plc.h
//  版 本 号   : V1.0
//  作    者   : cuiate/c00233580
//  生成日期   : 2016-01-09
//  功能描述   : TODO: ...
//
//  函数列表   : TODO: ...
//  修改历史   :
//  1.日    期 : 2016-01-09
//    作    者 : cuiate/c00233580
//    修改内容 : 创建文件
//
//*****************************************************************************

#ifndef _MRS_SRV_CCO_CSG_PLC_H_
#define _MRS_SRV_CCO_CSG_PLC_H_
HI_START_HEADER

#if (defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) && defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD))
//*****************************************************************************
//*****************************************************************************

HI_U32 mrsCcoCsgPlcModuleInit(HI_VOID);
HI_U32 mrsCcoCsgPlcModuleDeinit(HI_VOID);

HI_U32 mrsCcoCsgPlcAckFrame(HI_U8 *pucMacAddr, HI_U16 usSeq);
HI_U32 mrsCcoCsgPlcDenyFrame(HI_U8 *pucMacAddr, HI_U16 usSeq, HI_U8 ucDeny);

HI_U32 mrsCcoCsgAckConfirmProc(HI_PVOID pParam, HI_U16 usSize);
HI_U32 mrsCcoCsgAckDenyProc(HI_PVOID pParam, HI_U16 usSize);

HI_U32 mrsCcoCsgTransmitXrProc(HI_PVOID pParam, HI_U16 usSize);

HI_U32 mrsCcoCsgCmdSmResultProc(HI_PVOID pParam, HI_U16 usSize);
HI_U32 mrsCcoCsgCmdTransFileProc(HI_PVOID pParam, HI_U16 usSize);
HI_U32 mrsCcoCsgCmdQueryInfoProc(HI_PVOID pParam, HI_U16 usSize);
HI_U32 mrsCcoCsgCmdSetEventFlagProc(HI_PVOID pParam, HI_U16 usSize);
HI_U32 mrsCcoCsgCmdRebootProc(HI_PVOID pParam, HI_U16 usSize);
HI_U32 mrsCcoCsgCmdBindProc(HI_PVOID pParam, HI_U16 usSize);

HI_U32 mrsCcoCsgReportEventProc(HI_PVOID pParam, HI_U16 usSize);

//CCO收到PLC灵敏度测试报文
HI_U32 mrsCcoCsgPlcTestProc(HI_PVOID pParam, HI_U16 usSize);

//*****************************************************************************
//*****************************************************************************
#endif

HI_END_HEADER
#endif
