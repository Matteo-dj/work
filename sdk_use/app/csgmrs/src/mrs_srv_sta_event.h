//*****************************************************************************
//
//                  版权所有 (C), 1998-2011, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_srv_sta_event.h
//  版 本 号   : V1.0
//  作    者   : cuiate/00233580
//  生成日期   : 2013-03-20
//  功能描述   : 事件上报函数及接口定义
//
//  函数列表   : TODO: ...
//  修改历史   :
//  1.日    期 : 2013-03-20
//    作    者 : cuiate/00233580
//    修改内容 : 创建文件
//
//  2.日    期 : 2014-08-20
//    作    者 : liuxipeng/lKF54842
//    修改内容 : 修改文件
//
//*****************************************************************************

//*****************************************************************************
// PROJECT   :
// SUBSYSTEM :
// MODULE    :
// OWNER     :
//*****************************************************************************

#ifndef __MRS_SRV_STA_EVENT_H__
#define __MRS_SRV_STA_EVENT_H__
HI_START_HEADER

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
//*****************************************************************************
//*****************************************************************************

#define MRS_STA_EVT_FROM_SIMU    (0)    // 事件触发来自于模拟表
#define MRS_STA_EVT_FROM_METER   (1)    // 事件触发来自于真实表
#define MRS_STA_DETECT_POWER_FAILURE_ENABLE (1)     //STA能够进行停电检测


typedef enum
{
    MRS_STA_EVT_STATUS_IDLE,        // 空闲
    MRS_STA_EVT_STATUS_QUERY,       // 查询事件
    MRS_STA_EVT_STATUS_REPORT,      // 上报事件
} MRS_STA_EVT_STATUS_E;

#define MRS_STA_EVT_MONITOR_DOING    (1)
#define MRS_STA_EVT_MONITOR_IDLE     (0)

#define MRS_STA_EVT_STATUS_WORD_FLAG_LEN   (12)
#define MRS_STA_EVT_STATUS_WORD_DI         (0x04001501)
#define MRS_STA_EVT_STATUS_WORD_LEN_MIN    (14)

#define MRS_STA_EVT_METER_TYPE_UNKNOWN    (0)
#define MRS_STA_EVT_METER_TYPE_13_OLD     (1)
#define MRS_STA_EVT_METER_TYPE_13_NEW     (2)
#define MRS_STA_EVT_METER_TYPE_09         (3)

typedef struct
{
    HI_U8  ucStatus;  //0:不在监控, 1:正在监控
    HI_U8  ucMonitorInterval;//监控间隔,单位:分钟
    HI_U8  bWaitPlcFlag; //事件上报时PLC通道不可用发生标志
    HI_U8  ucDataLen; //电表应答的主动上报状态字报文中的数据域长度
    HI_U8  *pData;    //电表应答的主动上报状态字报文中的数据域内容
}MRS_STA_EVT_MONITOR_INF;

typedef struct
{
    HI_U8  ucMeterEvtType;        //0:默认值  1:13规范非新事件表  2:13规范新事件表  3:09规范电表
    HI_U8  ucStaConsDenyTimes; //STA连续收到电表否认应答次数，依此判定电表是否为09规范表
    HI_U8  ucConDentMaxTimes;  //STA连续收到电表否认应答次数N后，判定电表为09规范表
    HI_U8  padding;
}MRS_STA_EVT_FILTER_INF;


typedef enum
{
    MRS_CLTI_EVT_STATUS_IDLE = 0,
    MRS_CLTI_EVT_STATUS_WAIT_QUERY_REPLY,
    MRS_CLTI_EVT_STATUS_WAIT_PLC_REPLY,
} MRS_CLTI_EVT_STATUS_E;

typedef HI_U32 (*EVT_FRAME_RX)(HI_U8 *, HI_U16);
typedef HI_U32 (*EVT_FRAME_TIMEOUT)(HI_VOID);

typedef struct
{
    HI_U8 ucTotal;
    HI_U8 ucQuery;
    HI_U8 ucStatus;
    HI_U8 ucFrameLen;
    HI_U8 *pucFrame;
    EVT_FRAME_RX        pfnRx;
    EVT_FRAME_TIMEOUT   pfnTimeout;
} MRS_CLTI_EVT_INF;

typedef struct
{
    HI_BOOL bHardEvtOut;
    HI_BOOL bEvtOut;
    HI_U8  ucCltiEnable;
    HI_U8  padding;
    HI_U8  ucCltiiEnable;
    HI_U8  ucStaEnable;
    HI_U8  ucTimeout;
    HI_U8  ucMaxRetry;
    HI_U8  ucTimeOutMax;
    HI_U8  ucRetry;
    HI_U16 usSeq;
    MRS_PLC_FRAME_DATA_S *pstReportData;
    MRS_STA_EVT_MONITOR_INF stStaMonInf;
    MRS_STA_EVT_FILTER_INF  stFilterInf;
#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
    MRS_CLTI_EVT_INF        stCltiEvtInf;
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
    HI_U32 (*MrsStaRxRespProc)(HI_VOID *, HI_U16 usSize);
#endif

} MRS_STA_EVT_CTX, *P_MRS_STA_EVT_CTX;


// 事件上下文初始化
HI_U32 mrsStaEvtCtxInit(HI_VOID);

HI_U32 mrsStaEvtCtxReset(HI_VOID);

HI_VOID mrsStaEvtSetMeterType(HI_U8 ucType);

HI_U8 mrsStaEvtGetMeterType(HI_VOID);

HI_VOID mrsStaEvtConsDenyTimesReset(HI_VOID);

// EVENT_OUT管脚状态处理
HI_U32 mrsStaEvtOutStatusNotify(HI_U32, HI_BOOL);

// 事件上报处理
HI_U32 mrsStaEvtReportProc(HI_U8 *, HI_U16 );

// 发送事件上报PLC帧
HI_U32 mrsStaSendEvtReportPlc(HI_VOID);

// 事件上报超时定时器处理
HI_U32 mrsStaEvtReportPlcTimeoutProc(HI_VOID);

// 事件PLC命令帧接收处理
HI_U32 mrsCmdStaEvtInfProc(HI_U16, HI_PVOID, HI_U16);

P_MRS_STA_EVT_CTX mrsStaGetEvtCtx(HI_VOID);

// 事件管脚处理
HI_VOID mrsStaEvtOutProc(HI_VOID);

HI_BOOL mrsStaEvtSetEvtOut(HI_BOOL bEvtFlg);

HI_U32 mrsStaEvtMonTimeOutProc(HI_VOID);

HI_U32 mrsStaEvtProc(HI_U8 *pData, HI_U16 usLen, MRS_645_FRAME_STRU *pst645Hrd);

HI_VOID mrsStaEvtSetMonWaitPlcFlag(HI_BOOL bFlag);

HI_U32 mrsCltiEventFrameRx(HI_U8 *pFrame, HI_U16 usFrameLength, HI_U8 ucSubId);

HI_U32 mrsCltiEventFrameTimeout(HI_VOID);



#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)

HI_PUBLIC HI_U32 mrsStaEvtPlcAck(MRS_CSG_PLC_FRAME_HEAD *pstFrameHead);
HI_PUBLIC HI_U32 mrsStaEvtPlcDeny(MRS_CSG_PLC_FRAME_HEAD *pstFrameHead);
HI_PUBLIC HI_U32 mrsStaEvtPlcTimeout(HI_VOID);

#endif

//*****************************************************************************
//*****************************************************************************
#endif

HI_END_HEADER
#endif //__MRS_SRV_STA_EVENT_H__

