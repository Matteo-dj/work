//*****************************************************************************
//
//                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mrs_srv_sta_queue.h
//  �� �� ��   : V1.0 
//  ��    ��   : ������/KF62735
//  ��������   : 2012-07-11
//  ��������   : ����
//  �����б�   : NA
//  �޸���ʷ   : 
//  1.��    �� : 2012-07-11
//    ��    �� : ������/KF62735
//    �޸����� : �����ļ� 
//*****************************************************************************
#ifndef __MRS_SRV_STA_QUEUE_H__
#define __MRS_SRV_STA_QUEUE_H__

//������ͨ�ö��кͻỰģ��
#include "mrs_srv_queue.h"
#include "mrs_srv_meter.h"
#include "mrs_fw_n.h"

HI_CPP_START

//֪ͨ����
#define mrsStaNotifyQueue()     MRS_StartTimer(MRS_STA_TIMER_QUEUE, 0, HI_SYS_TIMER_ONESHOT)

#define MRS_QUEUE_MAX_TRY 2     // �������������Դ���

//4.1 STA�������
typedef struct 
{
    MRS_SRV_QUEUE stMrQueue;  //����
    HI_U8 retries;            //���Դ���
    HI_U8 lock;               //��  
    HI_U8 padding[2];
    MRS_MET_MODULE stMetModule;
}MRS_QUE_MODULE;

//�������Ԫ��
typedef struct
{
    MRS_SRV_NODE link;  //������ڵ�һλ
    HI_U8   from;       //��Դ�����أ���������HSO
    HI_U8   bcFlg;      //�㲥��ַ��־(Դ��ַΪ�㲥��ַ-ʹ�ù㲥��ַ��Ӧ)
    HI_U8   time_out;   //STA�˳�ʱʱ�� -��λ:�ٺ���
    HI_U8   option;     // I���ѱ����֡����: 1��ʼ�ѱ� 2ֹͣ�ѱ� 3��ѯ�ѱ���ǰ16�� 4��ѯ�ѱ�����16�� 5�ϵ糢���ϴ�������ַ
    HI_U16  id;         // ҵ������(plc id)
    HI_U16  len;        // ����������
    HI_U8 * buf;        // �������ݻ�����
    HI_U8   ucbaudrate_index;      // ���������� 00-Ĭ�ϲ�����   
   
    HI_U8   noresp_try_flag : 1;  // δӦ�����Ա�־-��CCO����-STA�ɴ��ж�δӦ����Ƿ���Ҫ����
    HI_U8   nak_try_flag    : 1;  // �������Ա�־-��CCO����-STA�ɴ��ж����յ�����֡���Ƿ���Ҫ����
    HI_U8   try_max  : 2;   // ������Դ���
    HI_U8   sub_id : 4;     //ҵ��id�µķ���,1-͸��id�µ�Уʱ����
    HI_U16  gap_timer;  // ֡����ʱ��
    HI_U16  seq;        // ��¼����֡����� ��֯����֡ʱȡ�����
    HI_U8 aucSrcAddr[HI_METER_ADDR_LEN];
    HI_U8 aucDstAddr[HI_METER_ADDR_LEN];

    HI_U8   bAutoStrategy  : 1;     // Auto detect strategy switch, 0 - total frame timeout strategy (old strategy), 1 - frame head & interval timeout strategy.
    HI_U8   bTtlTimerFlg   : 1;     // TTL timer started flag, when TTL timer alarm, if rx buf isn't empty, start frame interval timer.
    HI_U8   bFrameTimerFlg : 1;     // Frame interval timer started flag
    HI_U8   bRcvByteFlg    : 1;     // Rx data flag, whether received data after TTL timer.
    HI_U8   ucFrameTimeout : 4;     // Frame interval, unit 100ms.
    HI_U8   pad;
}MRS_STA_ITEM;

//STA���Ͷ��е�һЩ�ӿ�
HI_U32 mrsStaQueueInit(MRS_QUE_MODULE *);   //��ʼ������
HI_U32 mrsStaQueueReset(MRS_QUE_MODULE *);  //���ö���
HI_BOOL mrsStaTryEnQueue(MRS_QUE_MODULE *,MRS_STA_ITEM *);//�����
HI_BOOL mrsStaTryDeQueue(MRS_QUE_MODULE *,HI_VOID (*)(HI_VOID *));//���Գ�����
HI_VOID mrsStaQueueProc(MRS_QUE_MODULE *);  //���д����� 
HI_VOID mrsStaQueueFree(HI_VOID *);         //�ͷŽ��
HI_VOID * mrsStaQueueAlloc(HI_U32 size);    //�����ڴ�
HI_BOOL mrsStaQueueUnLock(MRS_QUE_MODULE *);
HI_U32 mrsStaSendTestMeter(HI_U8 ucBcFlag, HI_PBYTE pucData, HI_U16 usDataSize);
HI_VOID mrsStaUartTxStat(HI_U16 usId, HI_U8* pData, HI_U16 pDataLen);

// Update STA queue status
HI_VOID mrsStaUpdateQueueStatus(HI_VOID);

// Start TTL timer
HI_VOID mrsStaStartTtlTimer(MRS_STA_ITEM *pstItem, HI_U32 ulTTL);

// Stop TTL timer
HI_VOID mrsStaStopTtlTimer(MRS_STA_ITEM *pstItem);


HI_CPP_END
#endif
