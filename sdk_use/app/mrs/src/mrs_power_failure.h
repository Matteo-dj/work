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
#include "hi_mdm_msg.h"
#include "hi_sal.h"


#ifdef __cplusplus
extern "C" {
#endif

/************************************************************
                               �궨��
************************************************************/
#define MAX_REPORT_LEN          255         //���͵�������ݳ���
#define DATA_START_TEI_POSITION         (1)
#define DATA_END_TEI_POSITION           (2)
#define DATA_FIX_SIZE                   (3)
#define POWER_DATA_FIX_SIZE             (1)
#define MRS_US_TO_S_MASK                 1000
#define MRS_COMM_UNIT_TYPE_POWER_FAILURE  (1)
#define MIN_START_TEI                   (2)
#define MAX_REPORT_TEI_CNT              41
#define CCO_ERASE_BITMAP                (8)

#define STA_POWER_FAILURE               (1)
#define STA_POWER_ON                    (2)
#define STA_EQUIP_CHECK_MODE								1
#define STA_DEFAULT_SEND_POWER_ON_CNT						10
#define TSTA_CHECK_DISABLE_POWER_FAILURE_TIME				5

/************************************************************
                             ���ݽṹ����
************************************************************/
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) || defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
typedef struct
{
    HI_U8 power_failure_bitmap[POWER_FAILURE_BITMAP_LEN];           //ͣ��㼯��bitmap
    HI_U8 power_on_bitmap[POWER_FAILURE_BITMAP_LEN];                //�ϵ�㼯��bitmap

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    hi_u8 cco_diff_bitmap[POWER_FAILURE_BITMAP_LEN];                //��Ҫ���ϱ���bitmap
    hi_u8 cco_last_send_bitmap[POWER_FAILURE_BITMAP_LEN];           //cco�ϴ��ϱ���������bitmap����

    HI_U8 cco_wait_erase;                                           //cco�ȴ��������ݴ���
    HI_BOOL cco_erease_bitmap_flag;                                 //cco����bitmap��־λ
    HI_U8 cco_power_on_wait_erase;                                           //cco�ȴ��������ݴ���
    HI_BOOL cco_erease_power_on_bitmap_flag;                                 //cco����bitmap��־λ;

	hi_u8 cco_power_on_diff_bitmap[POWER_FAILURE_BITMAP_LEN];                //��Ҫ���ϱ���bitmap
    hi_u8 cco_power_on_last_send_bitmap[POWER_FAILURE_BITMAP_LEN];           //cco�ϴ��ϱ���������bitmap����
#endif

    HI_BOOL isRedayToSendPowerFailure;                              //���뷢��ͣ��֡״̬
    HI_BOOL isRedayToSendPowerOn;                                   //���뷢���ϵ�֡״̬
    HI_U8  sta_send_times;                                          //sta����ͣ���ϱ�֡����
    HI_BOOL ucWaitAckFlag;                                          //�ȴ�ack��־λ
    HI_U8   ucSendReportTimes;
    HI_U8  ucWaitAckType;                                           // ���ֵȴ��ظ�ack����
    HI_U16 usCurrSeq;                                               // �ȴ�ack�����

	hi_u8	sta_state;
	hi_u8 is_rcv_power_down;
	hi_u16 pad2;

	hi_u32 power_on_reset_time;
	hi_u32 last_rcv_power_on_time;

}POWER_FAILURE_CTRL_S;

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
typedef struct
{
    hi_u8 deviceType;                     // �豸����
    hi_u8 eventType;                      // �¼�����
    hi_u8 pad;
    hi_u8 pad1;
}PowerEventReportInfo;
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)

typedef struct
{
    hi_u16 my_tei;
    hi_bool device_power_failure;
    hi_bool device_pull_reset;

	hi_bool is_plug_module;
	hi_u8 pad;
	hi_u16 pad1;
}POWER_FAILURE_RCV_MAC_REPORT_S;

/************************************************************
                             �ӿں�������
 ************************************************************/
HI_U32 mrsPowerFailureCallback(HI_U8 ucType, HI_MDM_POWER_FAILURE_REPORT_ST *pCmdData, HI_U32 ulDataSize);
HI_U32 mrsHandlePowerFailure(HI_IN HI_SYS_QUEUE_MSG_S* pstMsg);
HI_U32 mrsHandleLocalPowerFailure(HI_VOID);
#endif
HI_U32 mrsPhasePowerFailureCallback(HI_U8 ucType, HI_MDM_PHASE_POWER_FAILURE_REPORT_ST *pCmdData, HI_U32 ulDataSize);
HI_VOID mrsSetStaPowerFailureNvCtrl(NV_APP_MRS_EVT_CFG_STRU *nv);
HI_VOID mrsGetOthersPowerFailureFrameData(MRS_EVENT_INFO_STRU * pstRcvEvtInfo, HI_U8 *pMac, HI_BOOL bLocalBcFlag);
#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
HI_U32 mrsStaCreatePowerFailureFrame(HI_U8 ucEventType);
HI_VOID mrsStaGetOthersPowerFailureFrameData(MRS_EVENT_INFO_STRU * pstRcvEvtInfo);
HI_VOID mrsHandlePowerOn(HI_VOID);
HI_VOID mrsHandleLocalPowerOn(HI_VOID);
HI_U32 mrsHandleOthersPowerFailureFrame(HI_VOID);
POWER_FAILURE_RCV_MAC_REPORT_S *mrsGetPowerFailureMacInfo(HI_VOID);
HI_U32 mrsLocalPowerFailureWaitSend(HI_VOID);
HI_U8 mrsPowerFailureGetCurrTurn(HI_VOID);
HI_U32 mrsStaPowerFailureFrameProc(HI_VOID);
HI_U32 mrsPowerFailureStaRcvAck(HI_U16 usSeq);
HI_U32 mrsStaHandleWaitPowerFailureAck(HI_VOID);
hi_void mrs_power_failure_set_equipmode_flag(hi_u8 sta_state);
hi_u32 mrs_power_failure_get_power_on_time(hi_void);
hi_u32 mrs_local_power_on_report_frame(hi_void);
hi_u32 mrs_local_power_on_wait_send(hi_void);
hi_u32 mrs_power_failure_juedge_pull_rst(hi_void);
hi_u32 mrs_power_failure_judge_disable_power_failure(hi_void);
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
HI_U32 mrsCcoEvtReport(hi_u8 eventType);
HI_U32 mrsCcoCreateReportFrame(HI_PBYTE *ppBuf, HI_U16 *usBufLen, HI_U16 *startTeiPos,
                                       PowerEventReportInfo *powerEventReportInfo);
hi_u32 mrsCcoRspPowerFailureEvt(hi_void);
hi_u32 mrsCcoRspPowerOnEvt(hi_void);
HI_VOID mrsEraseBitmap(HI_VOID);
hi_void mrs_cco_erase_power_on(hi_void);
HI_BOOL mrsCcoNotReplyAck(HI_U8 ucRcvMac[HI_PLC_MAC_ADDR_LEN], HI_U8 eventType);
HI_VOID mrsCcoCreatePowerFailureRspAck(MRS_EVENT_INFO_STRU* pstEvtTransmit, HI_U8 ucRcvMac[HI_PLC_MAC_ADDR_LEN]);
HI_U32 mrsCcoPowerFailureCallback(HI_U8 ucType, HI_MDM_POWER_FAILURE_REPORT_ST *pCmdData, HI_U32 ulDataSize);
hi_void mrs_set_last_rcv_power_on_time(hi_u32 power_on_time);
#endif
NV_APP_MRS_EVT_CFG_STRU *mrsGetPowerFailureNvCtrl(HI_VOID);
POWER_FAILURE_CTRL_S *mrsGetPowerFailureCtrlInfo(HI_VOID);

#endif

#ifdef __cplusplus
}
#endif

#if defined(PRODUCT_CFG_SUPPORT_MRS_DUOL_MODE_RF)
#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
HI_U32 mrsRfSetPowerFailureInfo(HI_VOID *pstInf,HI_U32 *nNeedRfTransmitPowerInfo);
HI_U8 *mrsRfGetPowerFailureInfo(HI_U16 *pLen,HI_U32 nNeedRfTransmit);
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
HI_U32 mrsCcoRfSetPowerFailureInfo(MRS_EVENT_INFO_STRU *pstEvtInf);
#endif

#endif

HI_END_HEADER

#endif


