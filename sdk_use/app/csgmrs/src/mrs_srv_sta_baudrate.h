//*****************************************************************************
//
//                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mrs_srv_sta_baudrate.h
//  �� �� ��   : V1.0 
//  ��    ��   : ������/KF62735
//  ��������   : 2012-07-11
//  ��������   : ���Բ�����
//  �����б�   : NA
//  �޸���ʷ   : 
//  1.��    �� : 2012-07-11
//    ��    �� : ������/KF62735
//    �޸����� : �����ļ� 
//*****************************************************************************
#ifndef __MRS_SRV_STA_BAUDRATE_H__
#define __MRS_SRV_STA_BAUDRATE_H__
HI_START_HEADER

typedef struct
{
    HI_U16  usBaudRate;
    HI_U8   ucRetry;
    HI_U8   ucPad;
}MRS_STA_DETECT_STRU;

MRS_STA_DETECT_STRU* mrsStaDetectGetContext(HI_VOID);

HI_VOID mrsStaDetectBaudRate(HI_VOID);

HI_VOID mrsCltIDetectBaudRate(HI_VOID);

// �ѱ�
HI_PUBLIC HI_VOID mrsStaSearchMeter(HI_VOID);

HI_END_HEADER
#endif