/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : mrs_srv_cltii_power_on_off.h
  �� �� ��   : V1.0
  ��    ��   : f00209182/00209182
  ��������   : 2018-4-02
  ����޸�   :
  ��������   : II��ͣ���ϵ��ϱ�����
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 
    ��    ��   :
    �޸�����   : 

******************************************************************************/
#ifndef MRS_SRV_CLTII_POWER_ON_OFF_H
#define MRS_SRV_CLTII_POWER_ON_OFF_H

HI_START_HEADER


/************************************************************
                     ��������ģ���ͷ�ļ�
************************************************************/
#include "hi_mdm_msg.h"
#include "hi_sal.h"
#include "mrs_srv_list.h"


#ifdef __cplusplus
extern "C" {
#endif

/************************************************************
                               �궨��
************************************************************/
#define POWER_OFF_QUERY_METER_NUM         (6)
#define EVT_TYPE_POSITION                 (0)
#define METER_NUM_POSITION                (1)
#define DATA_METER_POSITION               (0)
#define DATA_STATUS_POSITION              (6)
#define DATA_CLT_FIX_SIZE                 (3)
#define DATA_CLT_SINGLE_METER_SIZE        (7)
#define DATA_CLT_MIN_SIZE                 (DATA_CLT_FIX_SIZE + DATA_CLT_SINGLE_METER_SIZE)

#define CLT_POWER_INFO_DATA_LEN_MAX       (478)
#define TEN_MILLI_SECOND_UNIT             (10)  // 10����
#define MRS_CLT_FRAME_SEND_INTERVAL       (100) // ����

#define MRS_CLT_POWER_OFF_TYPE            (3)
#define MRS_CLT_POWER_ON_TYPE             (4)

#define MRS_CCO_REPORT_POWER_OFF_TYPE     (1)
#define MRS_CCO_REPORT_POWER_ON_TYPE      (2)

#define MRS_CLT_POWER_IDLE_STATUS          (0)
#define MRS_CLT_POWER_ON_WAIT_STATUS       (1)
#define MRS_CLT_POWER_ON_PLC_CREATE_STATUS (2)
#define MRS_CLT_POWER_ON_PLC_SEND_STATUS   (3)
#define MRS_CLT_POWER_ON_TO_OFF_STATUS     (4)

#define MRS_CLT_METER_POWER_ON_STATUS     (1)
#define MRS_CLT_METER_POWER_OFF_STATUS    (0)

#define MRS_CCO_POWER_ITEM_OLD_TIME       (240) // ���յ��ظ����ַ��Ϣʱ���ϱ�����������������Ŀ�ϻ�ʱ�� ��λ:��
#define MRS_CCO_POWER_ITEM_OLD_TIME_ADD   (1100) // ��������Ŀ�ϻ�ʱ��ĸ��� ��λ:����
#define MRS_CCO_REPORT_CLT_METER_NUM      (36)

#define MRS_CCO_REPORT_POWER_PROTOCOL     (4)

#define MRS_CLT_POWER_ON_PLC_SEND_TIMES_MAX (20)

#define MRS_CLT_POWER_OFF_97_DI_10        (0x9010)
#define MRS_CLT_POWER_OFF_97_DI_1F        (0x901F)
/************************************************************
                             ���ݽṹ����
************************************************************/
typedef struct
{
    HI_U32 ulMeterBitMap;  //�����Ƿ���Ҫ��ͬbit0:��Ӧ��astNvMeter�е�[0]����Ƿ���Գ�ͨ�ı�־ bit1:��Ӧ��astNvMeter�е�[1]����Ƿ���Գ�ͨ�ı�־,����... 1-���Գ�ͨ 0-����ͨ
    HI_U32 ulReportBitMap; //bit0:��Ӧ��astNvMeter�е�[0]���û��ͨ�Ƿ��ϱ��ı�־ bit1:��Ӧ��astNvMeter�е�[1]���û��ͨ�Ƿ��ϱ��ı�־,����... 1-�ϱ� 0-���ϱ�
    HI_U8  ucPlcSendTimes;
    HI_U8  ucPowerStatus     : 4;
    HI_U8  ucSendPlcWaitFlag : 1;
    HI_U8  ucWaitPlcAckFlag  : 1;
    HI_U8  pad               : 2;
    HI_U8  ucReportNum;
    HI_U8  pad2;
    HI_U16 usCurrPlcSeq;
    HI_U16 usOtherPlcSeq;
    MRS_PLC_FRAME_DATA_S *pstReportData;
    MRS_PLC_FRAME_DATA_S *pstOtherReportData;
}MRS_POWER_METER_INFO_STRU;

typedef struct
{
    HI_U16 usOffMeterNum;
    HI_U8  is_send_flag    : 1;
    HI_U8  turn_end_flag   : 1; // plc���ͷ�֡ʱÿ����֡���
    HI_U8  ucPowerOnFlag   : 1;
    HI_U8  pad             : 5;
    HI_U8  pad2;
    MRS_SRV_LIST stPowerOffList;  // ָ��POWER_INFO_CLT_ITEM
#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
    MRS_POWER_METER_INFO_STRU stMeterPowerInfo;
#endif
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    HI_U16 usOnMeterNum;
    HI_U8 pad3[2];
    MRS_SRV_LIST stPowerOnList;  // ָ��POWER_INFO_CLT_ITEM cco��Ҫ ����ʹ��
#endif
}POWER_INFO_CLT_INFO_S;

typedef struct
{
    MRS_SRV_LINK link;
    HI_U8 aucMeter[HI_METER_ADDR_LEN];
    HI_U8 ucStatus;
    HI_U8 send_turn;
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    HI_BOOL bReported; //Ŀǰ����CCO��ʹ�� �Ѿ��ϱ����ı�־
    HI_U8 pad[3];
    HI_U32 ulJoinTime; //��λΪ�� 
#endif
}POWER_INFO_CLT_ITEM, *P_POWER_INFO_CLT_ITEM;

HI_VOID mrsPoweOffOnInit(HI_VOID);
POWER_INFO_CLT_INFO_S* mrsGetCltPowerInfo(HI_VOID);
HI_U32 mrsCltPowerOffOnPlcProc(HI_IN MRS_EVENT_INFO_STRU * pstRcvEvtInfo, HI_U8* pMac, HI_BOOL bLocalBcFlag);
HI_U32 mrsGetCltOthersPowerFrameData(HI_U8 *pData, HI_U16 usMeterNum);
MRS_SRV_LIST *mrsGetCltPowerList(POWER_INFO_CLT_INFO_S *pstCltPowerInfo, HI_U8 ucPowerType);
HI_VOID mrsCltOtherAddPowerList(HI_U8 pucMeter[], HI_U8 ucStatus, HI_U8 ucPowerType);
HI_BOOL mrsCltIsExistPowerList(HI_U8 pucMeter[], HI_U8 ucPowerType);
HI_U16 mrsCltPowerOffGetMeterNum(HI_VOID);
HI_BOOL mrsCltPowerGetSendFlag(HI_VOID);
HI_VOID mrsCltPowerSetSendFlag(HI_BOOL bSendFlag);
#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
HI_VOID mrsCltSetPowerStatus(HI_U8 ucStatus);
HI_U8 mrsCltPowerGetSendTimes(HI_VOID);
HI_BOOL mrsCltPowerGetTurnEndFlag(HI_VOID);
HI_VOID mrsCltPowerSetTurnEndFlag(HI_BOOL bTurnEndFlag);
HI_U8 mrsCltPowerOnGetSendPlcFlag(HI_VOID);
HI_VOID mrsCltPowerOnSetSendPlcFlag(HI_BOOL bWaitSendFlag);
HI_U8 mrsCltPowerOffGetPlcAckFlag(HI_VOID);
HI_VOID mrsCltPowerOffSetPlcAckFlag(HI_BOOL bWaitPlcAckFlag);
HI_U16 mrsCltGetPlcDataSize(POWER_INFO_CLT_INFO_S *pstCltInfo, HI_OUT HI_U16 *pusNum);
HI_VOID mrsCltSetPowerOffData(HI_U8 *pData, POWER_INFO_CLT_INFO_S *pstCltInfo, HI_U16 usSendMeterNum);
HI_VOID mrsPowerOffFreeCltListNode(HI_VOID * pstNode, HI_VOID * pstCb);
HI_U32 mrsCltSendPowerOffFrameProc(HI_VOID);
HI_U32 mrsCltPowerOffPlcTimeout(HI_VOID);
HI_U32 mrsCltSendOtherPowerFrame(MRS_PLC_FRAME_DATA_S* pstReportData, HI_U16 usSeq);
HI_VOID mrsCltPowerOffOtherFrameFree(MRS_POWER_METER_INFO_STRU *pstMeterPowerInfo);
HI_U32 mrsCltSendPowerOffPlcFrame(MRS_PLC_FRAME_DATA_S *pstReportData);
HI_U32 mrsCltiiHandleLocalPowerOff(HI_VOID);
HI_VOID mrsCltPowerSetRmStatus(HI_U8 ucStatus);
HI_U32 mrsCltiiPowerOffRm(HI_VOID);
HI_U32 mrsCltPowerOff97FrameProc(HI_U8 *pData, HI_U16 ucLen);
HI_U32 mrsCltiiRmSendFrame(HI_U8 *pBuf, HI_U16 usLen, HI_U16 usId, HI_U8 ucProtocol);
HI_VOID mrsCltPowerOffRmRspProc(HI_BOOL bRmOkFlag);
HI_VOID mrsCltPowerOffRmRxProc(HI_VOID);
HI_VOID mrsCltPowerOffRmTimeOutProc(HI_VOID);
HI_VOID mrsCltLocalPowerOffPreProc(HI_VOID);
HI_VOID mrsCltPowerOffSendOtherEnd(HI_VOID);
HI_VOID mrsCltiiSearchDetectNvPowerProc(HI_BOOL bResult, HI_U8 ucCurrIndex, NV_APP_MRS_SM_NODE *pstNode);
HI_U32 mrsCltiiSearchPowerOnProc(NV_APP_MRS_SM_NODE *pstNvMeter);
HI_U32 mrsCltSetPowerOnUpData(HI_U8* pData, NV_APP_MRS_SM_NODE *pstNvMeter);
HI_U32 mrsCltSendPowerOnEvtFrame(HI_VOID);
HI_U32 mrsCltPowerOnChlNotifyProc(HI_BOOL bChlStatus, HI_U32 ulOpt, HI_BOOL *bOneShot);
HI_U32 mrsCltPowerOffRcvAck(HI_U16 usSeq);
HI_U32 mrsCltPowerOnOffRcvAck(HI_U16 usSeq);
HI_U32 mrsCltPowerOnRcvAck(HI_U16 usSeq);
HI_VOID mrsCltPowerOnEnd(HI_VOID);
HI_U32 mrsCltPowerOnPlcSendTimeOut(HI_VOID);
HI_U32 mrsCltCreatePowerOnEvtFrame(NV_APP_MRS_SM_NODE* pstNvMeter);
HI_VOID mrsCltiiPowerOnproc(HI_VOID);
#endif
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
HI_VOID mrsCcoRespCltPowerOffOn(HI_U8* pMac, MRS_EVENT_INFO_STRU * pstRcvEvtInfo, HI_BOOL bLocalBcFlag);
HI_U16 mrsCcoGetCltPowerReportNum(MRS_SRV_LIST *pstPowerList);
HI_U32 mrsCcoReportCltPowerFrame(MRS_SRV_LIST *pstPowerList, HI_U16 usMeterNum, HI_U8 ucType);
HI_U32 mrsCcoCreateCltPowerFrameProc(MRS_SRV_LIST *pstPowerList, HI_U16 usReportTotalNum, 
                                                            HI_U8 ucType, HI_U16 *pusSendPkNum);
HI_VOID mrsCcoDelCltPowerOldItem(MRS_SRV_LIST *pstPowerList, HI_U16 *pusMeterNum);
HI_VOID mrsCcoDelCltPowerListItem(HI_VOID);
HI_U32 mrsCcoCreateCltPowerFrame(MRS_SRV_LIST *pstPowerList, HI_U8 ucSendNum, HI_U8 **ppBuf, 
                                                    HI_U16 *pusBufLen, HI_U8 ucType);
HI_VOID mrsCcoSetReportCltPowerData(MRS_SRV_LIST *pstPowerList, HI_PBYTE pData, HI_U16 usOffset, 
                                                        HI_U8 ucSendNum);
HI_U32 mrsCcoReportCltPowerOffOnEvt(HI_VOID);
#endif

#ifdef __cplusplus
}
#endif

HI_END_HEADER

#endif


