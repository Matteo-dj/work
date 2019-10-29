//*****************************************************************************
//
//                  版权所有 (C), 2001-2014, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_srv_sta_csg_plc.h
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

#ifndef _MRS_SRV_STA_CSG_PLC_H_
#define _MRS_SRV_STA_CSG_PLC_H_
HI_START_HEADER

#if (defined(PRODUCT_CFG_PRODUCT_TYPE_STA) && defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD))
//*****************************************************************************
//*****************************************************************************

HI_U32 mrsStaCsgPlcModuleInit(HI_VOID);
HI_U32 mrsStaCsgPlcModuleDeinit(HI_VOID);

HI_U32 mrsStaCsgAckConfirmProc(HI_PVOID pParam, HI_U16 usSize);
HI_U32 mrsStaCsgAckDenyProc(HI_PVOID pParam, HI_U16 usSize);

HI_U32 mrsStaCsgTransmitXrProc(HI_PVOID pParam, HI_U16 usSize);

HI_U32 mrsStaCsgCmdSmResultProc(HI_PVOID pParam, HI_U16 usSize);
HI_U32 mrsStaCsgCmdDlMeterListProc(HI_PVOID pParam, HI_U16 usSize);
HI_U32 mrsStaCsgCmdTransFileProc(HI_PVOID pParam, HI_U16 usSize);
HI_U32 mrsStaCsgCmdQueryInfoProc(HI_PVOID pParam, HI_U16 usSize);
HI_U32 mrsStaCsgCmdSetEventFlagProc(HI_PVOID pParam, HI_U16 usSize);
HI_U32 mrsStaCsgCmdRebootProc(HI_PVOID pParam, HI_U16 usSize);
HI_U32 mrsStaCsgCmdBindProc(HI_PVOID pParam, HI_U16 usSize);
HI_U32 mrsStaCsgCmdTestProc(HI_PVOID pParam, HI_U16 usSize);


HI_U32 mrsStaCsgTransmitXr2CCO(HI_U8 *pucData, HI_U16 usDataLen, MRS_STA_ITEM *pstItem);

HI_U8 mrsStaGetPLCDenyCode(HI_U32 ulRet);

HI_U32 mrsStaCsgPlcTestProc(HI_PVOID pParam, HI_U16 usSize);
HI_U32 mrsStaCsgEvtProc(HI_PVOID pParam, HI_U16 usSize);


//*****************************************************************************
//*****************************************************************************
#endif

HI_END_HEADER
#endif
