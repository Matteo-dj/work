//*****************************************************************************
// 
//                  版权所有 (C), 1998-2015, 华为技术有限公司
// 
//*****************************************************************************
//  文 件 名   : mrs_srv_sta_meter.h
//  版 本 号   : V1.0 
//  作    者   : fengxiaomin/f00209182
//  生成日期   : 2015-10-23
//  功能描述   : STA端电表地址模块
//               
//  函数列表   : TODO: ...
//  修改历史   : 
//  1.日    期 : 2015-10-23
//    作    者 : fengxiaomin/f00209182
//    修改内容 : 创建文件 
// 
//*****************************************************************************
#ifndef __MRS_SRV_STA_METER_H__
#define __MRS_SRV_STA_METER_H__

HI_START_HEADER

typedef HI_U32 (* MRS_METER_CHG_NOTIFY_PROC_F)(HI_BOOL, HI_U8, HI_U32);

HI_VOID mrsMeterChgNotifyProcInit(HI_VOID);
HI_U32 mrsMeterChgNotifyProcReg(MRS_METER_CHG_NOTIFY_PROC_F pfProc, HI_U32 ulParam);
HI_U32 mrsMeterChgNotifyProc(HI_BOOL bChg, HI_U8 ucSlaveFlag);
HI_VOID mrsStaMeterRxNotify(HI_U8 *pucMeter, HI_U8 ucProtocol, HI_U8 ucSlaveFlag);

HI_VOID mrsStaGetMeterAddr(HI_U8 *pucMeter);
HI_VOID mrsStaSetMeterAddr(HI_U8 *pucMeter);

HI_VOID mrsStaSetMeterProtocol(HI_U8 ucProtocol);
HI_U8 mrsStaGetMeterProtocol(HI_VOID);

HI_VOID mrsStaSetMeterBaudrate(HI_U16 usBaudrate);
HI_U16 mrsStaGetMeterBaudrate(HI_VOID);

HI_END_HEADER
#endif
