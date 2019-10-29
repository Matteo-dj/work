/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2018. All rights reserved.
 * Description: 台区识别功能模块对外接口声明
 * Author: xiangchaobin/00378718
 * Create: 2018-12-29
 */
#ifndef __MRS_TRANSFOMER_IDENTIFY_H__
#define __MRS_TRANSFOMER_IDENTIFY_H__

HI_START_HEADER

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) || defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
HI_VOID mrsTfHandleMsg(HI_SYS_QUEUE_MSG_S* pstMsg);
HI_VOID mrsTfRcvMacReportCallback(HI_U8 ucOption, HI_U8 *pucData, HI_U16 usSize);

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
HI_BOOL mrsTfIsRcvNeighborBroadcastFrame(HI_PVOID pFrame, HI_U16 usSize);
HI_VOID mrsTfStaRunDataCollectProcess(MRS_PLC_TF_DATA_TYPE_E eDataType);
HI_U32  mrsTfStaRxFrame645(MRS_645_FRAME_STRU *pst645Hrd);
HI_VOID mrsTfStaStartDataCollectProcess(MRS_PLC_TF_DATA_TYPE_E eDataType);

#endif
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
HI_U32  mrsTfCcoSetNvPara(NV_APP_CCO_SM_CFG_EXP_STRU *pstNvCfg);
HI_U32  mrsTfCcoHandleRcvNotify(HI_U8 ucReportMode);
HI_U32  mrsTfCcoHandleStaReportPlcFrame(MRS_EVENT_INFO_STRU *pstEvtInfo, HI_U16 usEvtInfoLen, HI_U8 *pucSrcMac);

HI_U32  mrsTfCcoBeginQueryProcess(HI_VOID);
HI_U32  mrsTfCcoStopQueryProcess(HI_VOID);
HI_U32  mrsTfCcoStartNewQueryRound(HI_VOID);
HI_U32  mrsTfCcoRunQueryProcess(HI_VOID);

HI_U32  mrsTfCcoRefreshQueryList(HI_VOID);

HI_VOID mrsTfCcoHandleRcv05F130(HI_BOOL bSwitchStatus);
HI_BOOL mrsTfCcoIsEnable(HI_VOID);
#endif
#endif

HI_END_HEADER

#endif


