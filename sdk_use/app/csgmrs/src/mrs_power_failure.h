/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : mrs_power_failure.h
  �� �� ��   : ����
  ��    ��   : jingyankai/00415756
  ��������   : 2017��8��16��
  ����޸�   :
  ��������   : appͣ����ģ��
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2017��6��1��
    ��    ��   : likunhe/00194117
    �޸�����   : �����ļ�

******************************************************************************/
#ifndef MRS_POWER_FAILURE_H
#define MRS_POWER_FAILURE_H

HI_START_HEADER


/************************************************************
                     ��������ģ���ͷ�ļ�
************************************************************/
#include "hi_mdm.h"

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************
                               �궨��
************************************************************/
#define MAX_REPORT_LEN          255         //���͵�������ݳ���
#define SAVE_POSITION                   (1) //����λλ��
#define REPORT_EVENT_TYPE               (2) //�ϱ��¼�����λ��
#define DATA_START_TEI_POSITION         (1)
#define DATA_END_TEI_POSITION           (2)
#define DATA_FIX_SIZE                   (5)
#define MRS_US_TO_S_MASK                 1000
#define MRS_COMM_UNIT_TYPE_POWER_FAILURE  (1)
#define MIN_START_TEI                   (2)
#define MAX_REPORT_TEI_CNT              41
#define CCO_ERASE_BITMAP                (3)
#define BIT_WIZE                        (8)
#define SINGLE_AMMETER_POWER_FAILURE    (1)
#define CCO_REPORT_EVENT_DATA           (3)


/************************************************************
                             ���ݽṹ����
************************************************************/
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) || defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
typedef struct
{
    HI_U8 power_failure_bitmap[POWER_FAILURE_BITMAP_LEN];           //�㼯��bitmap

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    HI_U8 cco_diff_bitmap[POWER_FAILURE_BITMAP_LEN];                //��Ҫ���ϱ���bitmap
    HI_U8 cco_last_send_bitmap[POWER_FAILURE_BITMAP_LEN];           //cco�ϴ��ϱ���������bitmap����

    HI_U8 cco_wait_erase;                                           //cco�ȴ��������ݴ���
    HI_BOOL cco_erease_bitmap_flag;                                 //cco����bitmap��־λ
    HI_U16 pad1;
#endif

    HI_BOOL isRedayToSend;                                          //���뷢��״̬
    HI_U8  sta_send_times;                                          //sta����ͣ���ϱ�֡����
    HI_U16 pad2;                                                     //����λ

}POWER_FAILURE_CTRL_S;

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)

typedef struct
{
    HI_U16 usMyTei;
    HI_BOOL devicePowerFailure;
    HI_BOOL devicePullReset;

}POWER_FAILURE_RCV_MAC_REPORT_S;

/************************************************************
                             �ӿں�������
 ************************************************************/
HI_U32 mrsPowerFailureCallback(HI_U8 ucType, HI_MDM_POWER_FAILURE_REPORT_ST *pCmdData, HI_U32 ulDataSize);
HI_U32 mrsHandlePowerFailure(HI_IN HI_SYS_QUEUE_MSG_S* pstMsg);
HI_U32 mrsHandleLocalPowerFailure(HI_VOID);
#endif
HI_VOID mrsSetStaPowerFailureNvCtrl(NV_APP_MRS_EVT_CFG_STRU *nv);
HI_VOID mrsGetOthersPowerFailureFrameData(MRS_CSG_PLC_FRAME_HEAD * pstRcvEvtInfo);
#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
HI_U32 mrsCreatePowerFailureFrame(HI_VOID);
HI_U32 mrsHandleOthersPowerFailureFrame(HI_VOID);
HI_VOID mrsHandlePowerOn(HI_VOID);
POWER_FAILURE_RCV_MAC_REPORT_S *mrsGetPowerFailureMacInfo(HI_VOID);
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
HI_U32 mrsCcoEvtReport(HI_VOID);
HI_U32 mrsCcoCreateReportFrame(HI_PBYTE *ppBuf, HI_U16 *usBufLen, HI_U16 *startTeiPos);
HI_U32 mrsCcoRspPowerFailureEvt(HI_VOID);
HI_VOID mrsEraseBitmap(HI_VOID);
#endif
NV_APP_MRS_EVT_CFG_STRU *mrsGetPowerFailureNvCtrl(HI_VOID);
POWER_FAILURE_CTRL_S *mrsGetPowerFailureCtrlInfo(HI_VOID);

#endif

#ifdef __cplusplus
}
#endif

HI_END_HEADER

#endif


