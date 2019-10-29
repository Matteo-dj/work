//*****************************************************************************
// 
//                  ��Ȩ���� (C), 1998-2015, ��Ϊ�������޹�˾
// 
//*****************************************************************************
//  �� �� ��   : mrs_srv_sta_meter.h
//  �� �� ��   : V1.0 
//  ��    ��   : fengxiaomin/f00209182
//  ��������   : 2015-10-23
//  ��������   : STA�˵���ַģ��
//               
//  �����б�   : TODO: ...
//  �޸���ʷ   : 
//  1.��    �� : 2015-10-23
//    ��    �� : fengxiaomin/f00209182
//    �޸����� : �����ļ� 
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
