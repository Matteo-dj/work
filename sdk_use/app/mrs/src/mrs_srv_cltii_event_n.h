//*****************************************************************************
//
//                  版权所有 (C), 1998-2015, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_srv_cltii_event_n.h
//  版 本 号   : V1.0 
//  作    者   : cuiate/00233580
//  生成日期   : 2015-07-17
//  功能描述   : II采下挂电表事件 函数及接口声明(内部)
//               
//  函数列表   : TODO: ...
//  修改历史   : 
//  1.日    期 : 2015-07-17
//    作    者 : cuiate/00233580
//    修改内容 : 创建文件 
//
//*****************************************************************************

#ifndef _MRS_SRV_CLTII_EVENT_N_H_
#define _MRS_SRV_CLTII_EVENT_N_H_

HI_START_HEADER
#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)


#define MRS_CLTII_EVT_REPORT_645_CTRL           (0x9E)
#define MRS_CLTII_EVT_REPORT_645_FLAG           (0xC3)

#define MRS_CLTII_EVT_645_FLAG_OFFSET           (0)
#define MRS_CLTII_EVT_METER_TOTAL_NUM_OFFSET    (1)
#define MRS_CLTII_EVT_REPORT_METER_NUM_OFFSET   (2)

#define MRS_CLTII_EVT_REPORT_INFO_FIX_LEN       (3)
#define MRS_CLTII_EVT_REPORT_INFO_SINGLE_LEN    (8)


#define MRS_CLTII_EVT_MONITOR_DI_EVT_1_V97      (0xB212)
#define MRS_CLTII_EVT_MONITOR_DI_EVT_2_V97      (0xB213)
#define MRS_CLTII_EVT_MONITOR_DI_EVT_3_V97      (0xB310)
#define MRS_CLTII_EVT_MONITOR_DI_EVT_4_V97      (0xC021)

#define MRS_CLTII_EVT_MONITOR_DI_EVT_1_V07      (0x04001501)
#define MRS_CLTII_EVT_MONITOR_DI_EVT_2_V07      (0x19010001)
#define MRS_CLTII_EVT_MONITOR_DI_EVT_3_V07      (0x03300D00)
#define MRS_CLTII_EVT_MONITOR_DI_EVT_4_V07      (0x03300100)

#define MRS_CLTII_EVT_MONITOR_DI_EVT_1_FLAG     (0x01)
#define MRS_CLTII_EVT_MONITOR_DI_EVT_2_FLAG     (0x02)
#define MRS_CLTII_EVT_MONITOR_DI_EVT_3_FLAG     (0x04)
#define MRS_CLTII_EVT_MONITOR_DI_EVT_4_FLAG     (0x08)

#define MRS_CLTII_EVT_MONITOR_EVT_1_FLAG        (0x01)
#define MRS_CLTII_EVT_MONITOR_EVT_2_FLAG        (0x02)
#define MRS_CLTII_EVT_MONITOR_EVT_3_FLAG        (0x04)
#define MRS_CLTII_EVT_MONITOR_EVT_4_FLAG        (0x08)

#define MRS_CLTII_EVENT_STATUS_REPORT_SUPPORT   (0x01)
#define MRS_CLTII_EVENT_STATUS_REPORT_FLAG      (0x02)

#define MRS_CLTII_EVENT_TIMES_WIDTH             (3)
#define MRS_CLTII_EVENT_TIMES_WIDTH97           (2)
#define MRS_CLTII_EVENT_INVALID_TIMES           (0xFFFFFFFF)
#define MRS_CLTII_EVENT_INVALID_NV_TIMES        (0x00FFFFFF)
#define MRS_CLTII_EVENT_INVALID_NV_TIMES97      (0x0000FFFF)

#define MRS_CLTII_EVENT_NV_INFO_NUM             (4)
#define MRS_CLTII_EVENT_NV_NEED_SAVE            (1)

#define MRS_CLTII_EVENT_RECORD_INDEX_MAX        (3)
#define MRS_CLTII_EVENT_07_PROTOCOL_FLAG_BASE   (2)
#define MRS_CLTII_EVENT_97_PROTOCOL_FLAG_BASE   (1)

#define MRS_CLTII_EVENT_ITEM_VALID_MASK         (0x80)
#define MRS_CLTII_EVENT_97_PROTOCOL_EVENT_MASK  (0x07)

#define MRS_CLTII_EVENT_RECORD_MASK             (0xFFFFFFFF)

#define MRS_CLTII_EVENT_CLEAR_CTRL_DOWN         (0x1E)
#define MRS_CLTII_EVENT_QUERY_FLAG              (0xC3)
#define MRS_CLTII_EVENT_CLEAR_FLAG              (0xC4)
#define MRS_CLTII_EVENT_CLEAR_FIX_LEN           (8)
#define MRS_CLTII_EVENT_QUERY_UP_FIX_LEN        (3)
#define MRS_CLTII_EVENT_QUERY_REPORT_NUM_OFFSET (2) //相对于数据域第一个字节偏移为0计算的
#define MRS_CLTII_EVENT_QUERY_INDEX_OFFSET      (1)
#define MRS_CLTII_EVENT_QUERY_NUM_OFFSET        (2)
#define MRS_CLTII_EVENT_CLEAR_ADDR_OFFSET       (1)
#define MRS_CLTII_EVENT_CLEAR_FLAG_OFFSET       (7)
#define MRS_CLTII_EVENT_CLEAR_CTRL_UP           (0x9E)

#define MRS_CLTII_LOCAL_RM_INTERVAL_TIME        (200)

#define MRS_CLTII_EVENT_RX_PLC_ACK              (0)
#define MRS_CLTII_EVENT_RX_PLC_CLEAR            (1)
#define MRS_CLTII_EVENT_LOCAL_MONITOR_END       (2)

typedef struct
{
	HI_U8 ucProtocolBase;
	HI_U8 ucIndex;
	HI_U8 ucMeterIndex;
	HI_U8 ucWidth;
}MRS_CLTII_EVENT_UPDATE_INF;


HI_U32 mrsCltiiEventMonitorCfgInit(MRS_CLTII_EVENT_MON_CFG *pstCfg);

HI_U32 mrsCltiiEventMonitorInit(MRS_CLTII_EVENT_CTX_STRU *pstEvtCtx);

HI_U32 mrsCltiiEventMonitorProc(MRS_CLTII_EVENT_CTX_STRU *pstEvtCtx);

HI_U32 mrsCltiiEventMonitorFlagInitProc(HI_U8 ucFlag, MRS_CLTII_EVENT_MON_CFG *pstCfg); 

HI_U32 mrsCltiiEventMonitorRunStatusInit(MRS_CLTII_EVENT_RUNNING *pstRun); 

HI_U32 mrsCltiiEventReportParamInit(MRS_CLTII_EVENT_PARAM *pstParam);

HI_U32 mrsCltiiEventMonitorMeterFlagReset(MRS_CLTII_EVENT_METERLIST *pstMeterList);

HI_U32 mrsCltiiEventGenMonitorFrame(HI_IN MRS_PROTO645_FRAME_INF *pFrameInf, HI_OUT HI_U8 **ppFrame, HI_OUT HI_U16 *pFrameLen);

HI_U32 mrsCltiiEventSendFrame(HI_U8 *pFrame, HI_U16 usFrameLength, HI_U8 ucProtocol);

HI_U32 mrsCltiiEventMonitorItemProc(MRS_CLTII_EVENT_CTX_STRU *pstEvtCtx);

HI_U32 mrsCltiiEventMonitorNextItem(HI_VOID);
HI_U32 mrsCltiiEventMonitorNextItemProc(HI_VOID);

HI_U32 mrsCltiiEventMonitorFinish(HI_VOID);
HI_U32 mrsCltiiEventMonitorFinishProc(HI_VOID);

HI_U32 mrsCltiiEventLoadMeterList(MRS_CLTII_EVENT_CTX_STRU *pstEvtCtx);

HI_U32 mrsCltiiEventSyncInfFromNv(MRS_CLTII_EVENT_CTX_STRU *pstEvtCtx, MRS_CLTII_EVENT_METER_INF *pMeterInf);

HI_U32 mrsCltiiEventSyncMeterListInf(MRS_CLTII_EVENT_METERLIST *pstSrc, MRS_CLTII_EVENT_METERLIST *pstDst);

HI_U32 mrsCltiiEventFrameProcStatusWord(HI_IN HI_U8 *pFrame, HI_IN HI_U16 usFrameLength);
HI_U32 mrsCltiiEventFrameProcNetStatus(HI_IN HI_U8 *pFrame, HI_IN HI_U16 usFrameLength);
HI_U32 mrsCltiiEventFrameProcTimesV07(HI_IN HI_U8 *pFrame, HI_IN HI_U16 usFrameLength);
HI_U32 mrsCltiiEventFrameProcTimesV97(HI_IN HI_U8 *pFrame, HI_IN HI_U16 usFrameLength);

HI_U32 mrsCltiiEventRegisterCmd(HI_IN MRS_CLTII_EVENT_CMD_OBJ *pCmdList, HI_IN HI_U16 usCmdNum);

HI_U32 mrsCltiiSearchStartMsgProc(HI_VOID);

HI_U32 mrsCltiiSearchFinishMsgProc(HI_U32 ulOption);

HI_U32 mrsCltiiEventUpdateResult(MRS_CLTII_EVENT_METERLIST *pstMeterList);

HI_BOOL mrsCltiiEventNeedReport(HI_VOID);

HI_BOOL mrsCltiiSearchIsMeterListEmpty(HI_VOID);


MRS_CLTII_EVENT_CMD_FUNC mrsCltiiEventFindCmdFunc(HI_IN HI_U32 ulDi);

MRS_CLTII_EVENT_NV_METER_INF *mrsCltiiEventFindNvMeterInf(MRS_CLTII_EVENT_NV_INF *pNvInf, MRS_CLTII_EVENT_METER_INF *pMeterInf);


HI_VOID mrsCltiiEventInfoLoadNv(HI_VOID);

HI_U32 mrsCltiiEventNvMeterInsert(NV_APP_CLTII_EVENT_ITEM *pstItem);

HI_U32 mrsCltiiEventCreateEvt645(HI_OUT HI_U8 **ppData, HI_OUT HI_U16 *pusDataLen);

HI_U32 mrsCltiiEventSetReportEvtInfo(MRS_CLTII_EVENT_CTX_STRU *pstEvtCtx, HI_U8 *pData);

HI_U32 mrsCltiiEventNvDelSMNonExistInfo(HI_VOID);

HI_U32 mrsCltiiEventGetNvMeterInfo(HI_OUT HI_PVOID *ppData);

HI_U32 mrsCltiiEventDelNvInfo(NV_APP_MRS_SM_NODE *pstMeter, NV_APP_CLTII_EVENT_ITEM *pstEvent);

HI_U32 mrsCltiiEventCompareSMProc(NV_APP_MRS_SM_NODE *pstMeter, NV_APP_CLTII_EVENT_ITEM *pstEvent, 
                                              HI_U8 pucChangeFlag[]);

HI_U32 mrsCltiiEventInfoWriteNv(NV_APP_CLTII_EVENT_ITEM *pstEvent, HI_U8 pucChangeFlag[]);

HI_U32 mrsCltiiEventItemRxAckUpdate(NV_APP_CLTII_EVENT_ITEM *pstEvent, HI_U8 ucMeterIndex, HI_U8 ucEventIndex, HI_U8 pucChangeFlag[]);

HI_U32 mrsCltiiEventItemMonUpdate(NV_APP_CLTII_EVENT_ITEM *pstEvent, HI_U8 ucMeterIndex, HI_U8 ucEventIndex, HI_U8 pucChangeFlag[]);

HI_U32 mrsCltiiEventNewItemAdd(NV_APP_CLTII_EVENT_ITEM *pstEvent, HI_U8 ucMeterIndex, HI_U8 ucEventIndex, HI_U8 pucChangeFlag[], HI_U8 ucFrom);

HI_U32 mrsCltiiEvent07ItemUpdate(NV_APP_CLTII_EVENT_ITEM *pstEvent, HI_U8 ucMeterIndex, HI_U8 ucEventIndex, HI_U8 pucChangeFlag[]);

HI_U32 mrsCltiiEvent97ItemUpdate(NV_APP_CLTII_EVENT_ITEM *pstEvent, HI_U8 ucMeterIndex, HI_U8 ucEventIndex, HI_U8 pucChangeFlag[]);

HI_U32 mrsCltiiEventAddMeterInfo(NV_APP_CLTII_EVENT_ITEM *pstEvent, HI_U8 ucMeterIndex, HI_U8 pucChangeFlag[], HI_U8 ucFrom);

HI_U32 mrsCltiiEventMonInfoSaveNv(HI_VOID);

HI_U32 mrsCltiiEventRespCCO(HI_U8 *pData, HI_U16 usDataLen, MRS_STA_ITEM* pstItem);

HI_U32 mrsCltiiEventCreateUpFrame(HI_U8 **pp645Frame, HI_U16 *pusFrameLen, HI_U8 *pData, HI_U8 ucDataLen, HI_U8 *pAddr);

HI_U32 mrsCltiiEventCheck(HI_U8 *pData, HI_U16 usDataLen);

HI_U32 mrsCltiiEventRxClearSaveNv(HI_U8 ucMeterIndex);

HI_U32 mrsCltiiEventRespProc(HI_U8 *pFrame, HI_U16 usFrameLen, MRS_STA_ITEM* pstItem);

HI_U32 mrsCltiiEventQueryProc(HI_U8 *pFrame, HI_PVOID *ppData, HI_U8 *pucDataLen);

HI_U32 mrsCltiiEventClearProc(HI_U8 *pFrame);

#endif // defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
HI_END_HEADER
#endif //_MRS_SRV_CLTII_EVENT_N_H_

