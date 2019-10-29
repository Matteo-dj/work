//*****************************************************************************
// 
//                  ��Ȩ���� (C), 1998-2011, ��Ϊ�������޹�˾
// 
//*****************************************************************************
//  �� �� ��   : mrs_srv_meter.c
//  �� �� ��   : V1.0 
//  ��    ��   : niesongsong
//  ��������   : 2012-08-16
//  ��������   : ���ģ��
//               
//  �����б�   : 
//  �޸���ʷ   : 
//  1.��    �� : 2012-08-16
//    ��    �� : niesongsong/kf62735
//    �޸����� : �����ļ� 
// 
//*****************************************************************************
#ifndef __MRS_SIMU_METER_H__
#define __MRS_SIMU_METER_H__

//��������
#include "mrs_srv_list.h"

HI_START_HEADER

#define METER_NUM_MAX   PRODUCT_CFG_MRS_MAX_METER_IN_MAC_NUM

#define MRS_STA_SIM_F1_RATE_MODULUS  (124000 / 62)
#define MRS_STA_SIM_F10_RATE_MODULUS  (124000 / 12)
typedef HI_VOID * (*simu_alloc_fun)(HI_U32 sz);
typedef HI_VOID (*simu_free_fun)(HI_VOID *p);

typedef struct _MRS_SIMU_METER 
{
    HI_U8 ucVer;            /*�汾,1997*/
    HI_U8 ucMeterAddr[6];   /*��ַ*/
    HI_U8 padding[1];

    HI_U32 uiRate[4];       /*4������*/
    HI_U32 uiKWH;           /*�����й��ܵ���*/     
    HI_U32 uiTime;          /*����ʱ���ʽ*/
   
    HI_U16 usWord1;         /*״̬��1*/
    HI_U16 usWord3;         /*״̬��3*/
    HI_U32 ulOpenTimes : 24;     /*���Ǵ���*/
    HI_U32 ulPowerOffTimes : 24; /*�������*/
    HI_U32 ulNeutralWire : 24;  /*���ߵ���*/
    HI_U32 ulRunTimes : 8;
    HI_U32 ulLiveWire : 24;   /*���ߵ���*/
}MRS_SIMU_METER,*PMRS_SIMU_METER;

//ģ����ģ��
typedef struct 
{
    HI_U16 usNum;    /*������*/
    HI_U8  padding[2];
    MRS_SIMU_METER *pstMList[METER_NUM_MAX]; /*����б����64�����*/
    simu_alloc_fun pfnMalloc;
    simu_free_fun  pfnFree;
}MRS_MET_MODULE;

//��������Ļ���
typedef struct 
{
    HI_U8 * ucBuf;        
    HI_U32  uiLen;
}MRS_MEM_BUF;

typedef struct 
{
    MRS_SRV_LINK link;
    MRS_MEM_BUF buf;
}MRS_MEM_LINK;

//��ʼʼ�����ģ��
HI_PUBLIC HI_U32 mrsSrvMeterInit(MRS_MET_MODULE*,HI_U16,simu_alloc_fun,simu_free_fun);

//��ӵ��
HI_PUBLIC HI_U32 mrsSrvMeterAdd(MRS_MET_MODULE*,HI_U8 * maddr);

//���ظ�
HI_PUBLIC HI_U32 mrsSrvMetersReply(MRS_MET_MODULE*,MRS_MEM_BUF *,MRS_SRV_LIST *);

// ͬ��������ַ��Mac��
HI_PUBLIC HI_VOID mrsSyncVMAddrToMac(HI_VOID);

HI_U32 mrsStaSend2Meter(MRS_MET_MODULE *,HI_PBYTE, HI_U16);
HI_U32 mrsStaGetSupportVM(HI_VOID);
HI_U32 mrsStaSetSupportVM(HI_U32 flag);
HI_VOID mrsStaSetVMActiveDelay(HI_U32 ulTime);
HI_U32 mrsStaGetVMActiveDelay(HI_VOID);

HI_END_HEADER

#endif
