//*****************************************************************************
//
//                  版权所有 (C), 1998-2011, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_srv_cco_event.h
//  版 本 号   : V1.0
//  作    者   : cuiate/00233580
//  生成日期   : 2013-03-21
//  功能描述   : 事件上报函数及接口定义
//
//  函数列表   : TODO: ...
//  修改历史   :
//  1.日    期 : 2013-03-21
//    作    者 : cuiate/00233580
//    修改内容 : 创建文件
//  2.日    期 : 2014-08-20
//    作    者 : fengxiaomin/00209182
//    修改内容 : 修改文件
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

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
//*****************************************************************************
//*****************************************************************************

#define MRS_CCO_FIND_REPEAT_EVT_SUCCESS     (1)
#define MRS_CCO_FIND_REPEAT_EVT_FAIL        (0)
#define MRS_CCO_DETECT_POWER_FAILURE_ENABLE (1)
#define mrsActiveEvtReport()    MRS_StartTimer(EN_MRS_SRV_CCO_EVENT_ACTIVE_TIMER, 0, HI_SYS_TIMER_ONESHOT)


#define MRS_CCO_EVT_FLAG_NOTICE_INTERVAL    (1)
#define MRS_CCO_EVT_FLAG_NOTICE_RETRYMAX    (5)


typedef struct
{
    HI_U8 ucListNum;
    HI_U8 reserved[3];
    MRS_SRV_LIST stEvtFilterList;//指向MRS_CCO_EVT_REPORTED_FILTER_ITEM
}MRS_CCO_EVT_REPORTED_FILTER_LIST;

typedef struct
{
    MRS_SRV_LINK link;
    HI_U8 ucMeterAddr[HI_METER_ADDR_LEN];
    HI_U16 usPlcSeq;
    HI_U32 ulTime; //单位:秒
}MRS_CCO_EVT_REPORTED_FILTER_ITEM, *P_MRS_CCO_EVT_REPORTED_FILTER_ITEM;

typedef struct
{
    HI_U8   ucQueueLock;
    HI_U8   reserved;
    HI_U16  usItemNum;
    MRS_SRV_QUEUE stQueue;//指向MRS_EVT_QUEUE_ITEM
}MRS_CCO_EVT_QUEUE;

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
typedef struct
{
    HI_U8 ucInterval;
    HI_U8 ucRetryMax;
    HI_U8 ucRetry;
    HI_U8 reserved;
    MRS_PLC_FRAME_DATA_S *pstPlcFrame;
} MRS_CCO_EVT_NOTICE_STRU;
#endif


typedef struct
{
    HI_BOOL bACDFlg;
    HI_U8   ucReportEnable;
    HI_U16  usTimeOut;
    HI_U8   ucMaxTry;
    HI_U8   reserved;
    HI_U16  usFilterAgedTime;//单位:分钟
    HI_U16  usEvtBufSize;
    HI_U16  usStatBakTime;   //单位:分钟
    MRS_CCO_EVT_QUEUE stEvtQueue;
    MRS_CCO_EVT_REPORTED_FILTER_LIST stEvtReportedList;

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
    MRS_CCO_EVT_NOTICE_STRU stNotice;
#endif
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


#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
HI_PUBLIC HI_VOID mrsCcoEvtNoticeReset(HI_VOID);
HI_PUBLIC HI_U32  mrsCcoEvtFlagNotice(HI_IN HI_U8 ucFlag);
HI_PUBLIC HI_U32  mrsCcoEvtFlagDisableNotice(HI_IN HI_U8 *pucMacAddr);
HI_PUBLIC HI_U32  mrsCcoEvtCreateNoticeFrame(HI_IN HI_U8 ucFlag, HI_IN HI_U8 *pucMacAddr, HI_OUT MRS_PLC_FRAME_DATA_S **ppstPlcFrame);
HI_PUBLIC HI_U32  mrsCcoEvtNoticeSendFrame(HI_VOID);
HI_PUBLIC HI_U32  mrsCcoEvtPlcFrameProc(MRS_PLC_FRAME_DATA_S *pstPlcData);
HI_PUBLIC HI_U32  mrsCcoEvtFilterCheck(HI_U8 *pucMeterAddr, HI_U16 usSeq);
HI_PUBLIC HI_U32  mrsCcoEvtCreateReportFrame(MRS_CSG_PLC_FRAME_HEAD *pstFrameHead, HI_U8 **ppOutFrame, HI_U16 *pusOutLength);

#endif


//*****************************************************************************
//*****************************************************************************
#endif

HI_END_HEADER
#endif //__MRS_SRV_CCO_EVENT_H__

