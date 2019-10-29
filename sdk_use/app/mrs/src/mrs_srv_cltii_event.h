//*****************************************************************************
//
//                  版权所有 (C), 1998-2015, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_srv_cltii_event.h
//  版 本 号   : V1.0 
//  作    者   : cuiate/00233580
//  生成日期   : 2015-07-17
//  功能描述   : II采下挂电表事件 函数及接口声明
//               
//  函数列表   : TODO: ...
//  修改历史   : 
//  1.日    期 : 2015-07-17
//    作    者 : cuiate/00233580
//    修改内容 : 创建文件 
//
//*****************************************************************************

#ifndef _MRS_SRV_CLTII_EVENT_H_
#define _MRS_SRV_CLTII_EVENT_H_
HI_START_HEADER
#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)

#include "mrs_srv_sta_queue.h"
#define MRS_CLTII_EVT_MONITOR_INTERVAL_DEFAULT  (3600)
#define MRS_CLTII_EVT_MONITOR_INTERVAL_MIN      (120)
#define MRS_CLTII_EVT_MONITOR_FLAG_DEFAULT      (0xF7)

#define MRS_CLTII_EVENT_ITEM_MAX                (4)
#define MRS_CLTII_EVENT_NV_ITEM_VALID           (0x80)
#define MRS_CLTII_EVENT_NV_PROTO_MASK           (0x0F)

#define MRS_CLTII_EVT_REPORT_INFO_NUM_MAX       (8)


typedef enum
{
    MRS_CLTII_EVENT_MSG_START = 0,
    MRS_CLTII_EVENT_MSG_STOP,
    MRS_CLTII_EVENT_MSG_NEXT_ITEM,
    MRS_CLTII_EVENT_MSG_FINISH,
} MRS_CLTII_EVENT_MSG_E;


typedef struct
{
    HI_U8 aucMeterAddr[HI_METER_ADDR_LEN];
    HI_U8 ucProtocol;
    HI_U8 ucReserved;
    HI_U32 aulEvent[3];
} MRS_CLTII_EVENT_NV_METER_INF;


typedef struct
{
    HI_U16 usMonInterval;
    HI_U8  ucMonFlag;
    HI_U8  ucMeterNum;
    MRS_CLTII_EVENT_NV_METER_INF astMeterList[MRS_MAX_METERNUM];
} MRS_CLTII_EVENT_NV_INF;


typedef struct
{
    HI_BOOL bEnableEvent[MRS_CLTII_EVENT_ITEM_MAX];
    HI_U16 ausDiList[MRS_CLTII_EVENT_ITEM_MAX];
} MRS_CLTII_EVENT_MON_INF_97;


typedef struct
{
    HI_BOOL bEnableEvent[MRS_CLTII_EVENT_ITEM_MAX];
    HI_U32 aulDiList[MRS_CLTII_EVENT_ITEM_MAX];
} MRS_CLTII_EVENT_MON_INF_07;


typedef struct 
{
    MRS_CLTII_EVENT_MON_INF_97 stMonInf97; 
    MRS_CLTII_EVENT_MON_INF_07 stMonInf07; 
} MRS_CLTII_EVENT_MON_CFG;


typedef struct
{
    HI_U32 aulRecordInf[3]; // 事件记录(97表:事件1~3; 07表:事件2~4)
} MRS_CLTII_EVENT_RECORD_INF;


typedef struct
{
    HI_U8 aucAddr[HI_METER_ADDR_LEN];
    HI_U8 ucProtocol;
    HI_U8 ucOption;     // 97表: 事件4记录 
                        // 07表: D0: 支持主动上报标识
                        //       D1: 主动上报标识(1:有事件)
                        //       D2~D7: 000000b
    MRS_CLTII_EVENT_RECORD_INF stRecordInf;
} MRS_CLTII_EVENT_METER_INF;


typedef struct
{
    HI_U8 ucMeterTotal;     // 电表总数
    HI_U8 aucReserved[2];
    HI_U8 ucEventCount;     // 监控到的有事件的电表数量
    HI_U8 aucEventInf[MRS_MAX_METERNUM];    // 电表事件标识信息，每个电表占一个字节，按位表示事件信息
                                            // D0: 事件1 0-无 1-有
                                            // D1: 事件2 0-无 1-有
                                            // D2: 事件3 0-无 1-有
                                            // D3: 事件4 0-无 1-有
                                            // D4~D7: 0000b
    MRS_CLTII_EVENT_METER_INF astMeterInf[MRS_MAX_METERNUM];
} MRS_CLTII_EVENT_METERLIST;


typedef enum
{
    MRS_CLTII_EVENT_MONITOR_IDLE,
    MRS_CLTII_EVENT_MONITOR_WORKING,
} MRS_CLTII_EVENT_MONITOR_STATUS_E;


typedef struct
{
    MRS_CLTII_EVENT_MONITOR_STATUS_E eMonStatus;  // 本地监控事件状态 0-空闲, 1-正在监控
    HI_U8   ucCurrEvtIndex;  // 当前处理到的事件标识信息
    HI_U8   ucCltiiAddr[HI_METER_ADDR_LEN];  // 采集器地址--资产编号
    HI_U8   ucCurrReportNum;  // 本次上报的数量
    HI_U8   aucCurrReportIndex[MRS_CLTII_EVT_REPORT_INFO_NUM_MAX];  // 本次上报的表信息的标识 
} MRS_CLTII_EVENT_PARAM;



typedef HI_U32 (*MRS_CLTII_EVENT_CMD_FUNC)(HI_U8 *pFrame, HI_U16 usFrameLength);

typedef struct
{
    HI_U32 ulDi;
    MRS_CLTII_EVENT_CMD_FUNC pFunc;
} MRS_CLTII_EVENT_CMD_OBJ;


typedef struct
{
    HI_U16 usCmdNum;
    HI_U16 usReserved;
    MRS_CLTII_EVENT_CMD_OBJ *pCmdList;
} MRS_CLTII_EVENT_CMD_HEAD;


typedef struct
{
    HI_U8 ucMeterIndex;
    HI_U8 ucDiIndex;
    HI_U8 aucReserved[2];

    MRS_CLTII_EVENT_CMD_HEAD stCmdHdr;
} MRS_CLTII_EVENT_RUNNING;


typedef struct
{
    MRS_CLTII_EVENT_NV_INF      stNvInf; 
    MRS_CLTII_EVENT_MON_CFG     stCfg;
    MRS_CLTII_EVENT_METERLIST   stMeterList;
    MRS_CLTII_EVENT_PARAM       stParam;
    MRS_CLTII_EVENT_RUNNING     stRun;
} MRS_CLTII_EVENT_CTX_STRU;


typedef struct
{
    HI_U8 aucMeterAddr[6];  // 电表地址
    HI_U8 ucOption;         // [0:3]: 电表规约类型；1-97规约，2-07规约，其它-无效
                            // [4:6]: 保留
                            // [7:7]: 有效性标识；1-有效；0-无效
    HI_U8 aucEventInf[9];   // 事件信息
                            // 97规约: BYTE[0-1] EVENT1; [2-3] EVENT2; [4-5] EVENT3; [6-8] 保留
                            // 07规约: BYTE[0-2] EVENT2; [3-5] EVENT3; [6-8] EVENT4;
} NV_APP_CLTII_EVENT_ITEM;


// 采集器事件存储使用数据存储NV项0x40C-0x40F，结构大小与结构NV_APP_NVM_DATA_STRU相同
// 在本模块处理时，统一使用本模块自定义的结构NV_APP_CLTII_EVENT_INF_STRU
typedef struct
{
    NV_APP_CLTII_EVENT_ITEM astEventItem[8];
} NV_APP_CLTII_EVENT_INF_STRU;



MRS_CLTII_EVENT_CTX_STRU * mrsGetCltiiEventCtx(HI_VOID);

HI_VOID mrsCltiiEventSetCltAddr(HI_U8 pucAddr[HI_METER_ADDR_LEN]);

HI_U32 mrsCltiiEventModuleInit(HI_VOID);

HI_U32 mrsCltiiEventModuleDeinit(HI_VOID);

HI_U32 mrsCltiiEventStartMonitor(HI_VOID);

HI_U32 mrsCltiiEventFrameRx(HI_U8 *pFrame, HI_U16 usFrameLength);

HI_U32 mrsCltiiEventFrameTimeout(HI_VOID);

HI_U32 mrsCltiiEventMsgProc(HI_SYS_QUEUE_MSG_S *pstMsg);

MRS_CLTII_EVENT_MONITOR_STATUS_E mrsCltiiEventGetMonitorStatus(HI_VOID);

HI_VOID mrsCltiiEventSetMonitorStatus(MRS_CLTII_EVENT_MONITOR_STATUS_E eStatus);

HI_U32 mrsCltiiEventReport(HI_VOID);

HI_U32 mrsCltiiEventInfoRxAckSaveNv(HI_VOID);

HI_U32 mrsCltiiEventPlcProc(HI_U8 *pData, HI_U16 usDataLen, MRS_STA_ITEM* pstItem);

HI_U32 mrsCltiiEventMonitorFlagInit(HI_VOID);

HI_U32 mrsCltiiEventMonitorSendMessage(HI_U32 ulMsgId);


#endif // defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
HI_END_HEADER
#endif //_MRS_SRV_CLTII_EVENT_H_

