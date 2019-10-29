//*****************************************************************************
//
//                  ��Ȩ���� (C), 1998-2011, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mrs_srv_cco_event.h
//  �� �� ��   : V1.0
//  ��    ��   : cuiate/00233580
//  ��������   : 2013-03-21
//  ��������   : �¼��ϱ��������ӿڶ���
//
//  �����б�   : TODO: ...
//  �޸���ʷ   :
//  1.��    �� : 2013-03-21
//    ��    �� : cuiate/00233580
//    �޸����� : �����ļ�
//  2.��    �� : 2014-08-20
//    ��    �� : fengxiaomin/00209182
//    �޸����� : �޸��ļ�
//
//*****************************************************************************

//*****************************************************************************
// PROJECT   :
// SUBSYSTEM :
// MODULE    :
// OWNER     :
//*****************************************************************************

#ifndef __MRS_SRV_CCO_EVENT_H__
#define __MRS_SRV_CCO_EVENT_H__
HI_START_HEADER


#define MRS_CCO_FIND_REPEAT_EVT_SUCCESS (1)
#define MRS_CCO_FIND_REPEAT_EVT_FAIL    (0)
#define MRS_CCO_DETECT_POWER_FAILURE_ENABLE (1)
#define mrsActiveEvtReport()                                              \
    do {                                                                \
            HI_SYS_QUEUE_MSG_S stMsg = { 0 };                           \
            stMsg.ulMsgId = ID_MRS_MSG_MR_TIMER;                        \
            stMsg.ulParam[0] = EN_MRS_SRV_CCO_EVENT_ACTIVE_TIMER;    \
            mrsSendMessage2Queue(&stMsg);                               \
    }while(0)


typedef struct
{
	HI_U8 ucListNum;
	HI_U8 reserved[3];
	MRS_SRV_LIST stEvtFilterList;//ָ��MRS_CCO_EVT_REPORTED_FILTER_ITEM
}MRS_CCO_EVT_REPORTED_FILTER_LIST;

typedef struct
{
	MRS_SRV_LINK link;
	HI_U8 ucMeterAddr[HI_METER_ADDR_LEN];
	HI_U16 usPlcSeq;
	HI_U32 ulTime; //��λ:��
}MRS_CCO_EVT_REPORTED_FILTER_ITEM, *P_MRS_CCO_EVT_REPORTED_FILTER_ITEM;

typedef struct
{
    HI_U8   ucQueueLock;
    HI_U8   reserved;
    HI_U16  usItemNum;
    MRS_SRV_QUEUE stQueue;//ָ��MRS_EVT_QUEUE_ITEM
}MRS_CCO_EVT_QUEUE;

typedef struct
{
    HI_BOOL bACDFlg;
    HI_U8   ucReportEnable;
    HI_U16  usTimeOut;
    HI_U8   ucMaxTry;
    HI_U8   reserved;
    HI_U16  usFilterAgedTime;//��λ:����
    HI_U16  usEvtBufSize;
    HI_U16  usStatBakTime;   //��λ:����
    MRS_CCO_EVT_QUEUE stEvtQueue;
    MRS_CCO_EVT_REPORTED_FILTER_LIST stEvtReportedList;
} MRS_CCO_EVT_CTX_STRU, *P_MRS_CCO_EVT_CTX_STRU;

typedef struct
{
    MRS_SRV_NODE link;
    HI_U8   ucMeterAddr[HI_METER_ADDR_LEN];
    HI_U16  usPlcSeq;
    HI_U16  usDataLen;
    HI_U8   reserved[2];
    HI_U8   pucData[0];
} MRS_EVT_QUEUE_ITEM, *P_MRS_EVT_QUEUE_ITEM;

P_MRS_CCO_EVT_CTX_STRU mrsGetCcoEvtCtx(HI_VOID);
HI_VOID mrsCcoEvtInit(HI_VOID);
HI_U32 mrsCmdCcoEvtTransmit(HI_IN HI_U16 usId, HI_PVOID pReqPacket, HI_U16 usPacketSize);
HI_U32 mrsCcoEvtReportMsgProc(HI_VOID);
HI_U32 mrsEvtReportTimeoutFunc(HI_VOID * param);
HI_VOID mrsCcoEvtEnQueue(MRS_CCO_EVT_QUEUE* pstEvtQueue, P_MRS_EVT_QUEUE_ITEM pstEvtItem);
HI_U32 mrsCcoEvtSendToSta(MRS_EVENT_INFO_STRU* pstEvtTransmit, HI_U8 *ucMac, HI_U8 ucFunCode);




HI_END_HEADER
#endif //__MRS_SRV_CCO_EVENT_H__

