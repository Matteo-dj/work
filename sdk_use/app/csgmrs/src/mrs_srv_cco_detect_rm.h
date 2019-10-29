//*****************************************************************************
//
//                  版权所有 (C), 1998-2016, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_srv_cco_detect_rm.h
//  版 本 号   : V1.0 
//  作    者   : fengxiaomin/00209182
//  生成日期   : 2016-01-13
//  功能描述   : 采集器入网全网抄表功能函数及接口声明
//               
//  函数列表   : TODO: ...
//  修改历史   : 
//  1.日    期 : 2016-01-13
//    作    者 : fengxiaomin/00209182
//    修改内容 : 创建文件 
//
//*****************************************************************************

#ifndef _MRS_SRV_CCO_DETECT_RM_H_
#define _MRS_SRV_CCO_DETECT_RM_H_

HI_START_HEADER

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)

//*****************************************************************************
//*****************************************************************************
#define MRS_CCO_DETECT_MAC_NUM_MAX          (100)
#define MRS_CCO_DETECT_MAC_BUFFER_MAX       (MRS_CCO_DETECT_MAC_NUM_MAX * HI_PLC_MAC_ADDR_LEN)


typedef struct 
{
    MRS_SRV_LINK stLink;
    HI_U8 aucMac[HI_PLC_MAC_ADDR_LEN];
	HI_U8 reserved[2];
    HI_U32 ulTimeStamp; //单位:毫秒
} MRS_CCO_DETECT_BLACK_LIST_ITEM;

typedef struct
{
    HI_U16 usNum;                      // 队列元素个数
    HI_U8  aucPad[2];                  // 保留位
    MRS_SRV_LIST stList;              // 指向MRS_CCO_DETECT_BLACK_LIST_ITEM
} MRS_CCO_DETECT_BLACK_LIST;

typedef struct 
{
    MRS_SRV_NODE stLink;
	HI_BOOL bValid;
    HI_U8 reserved;
    HI_U8 aucMac[HI_PLC_MAC_ADDR_LEN];
    HI_U32 ulTimeStamp;
} MRS_CCO_DETECT_WHITE_QUEUE_ITEM;

typedef struct
{
    HI_U16 usNum;                      // 队列元素个数
    HI_U16 usValidNum;                  
    MRS_SRV_QUEUE stQueue;             // 队列
} MRS_CCO_DETECT_WHITE_QUEUE;

typedef struct
{
	HI_U8 ucEnable;
	HI_U8 ucTopoWaitMaxTime;//如果CCO一直处于组网中且只有CCO一个站点,20分钟后收到拒绝入网通知后将拒绝采集器站点加入表档案白名单,前提是表档案非空,单位分钟
	HI_U16 usTimeOut; //将以mac地址入网的采集器加入白名单后，如果ucWhilteAddMaxTime时间内没有加入白名单的采集器站点入网,删除添加的白名单
	HI_U8 ucMaxTurn;
	HI_U8 ucJoinNetMaxTime;//探测机制中缓存在追加白名单中的站点入网时间,超时后启动探测抄表,单位:分钟
    HI_U8 ucBlackValidTime;//探测机制中黑名单有效时间 单位:分钟
	HI_U8 ucStaJoinWhiteTime;//被拒绝的站点加入到白名单的时间 单位:分钟
	HI_U16 aus97DI[MRS_BROADCAST_RM_97_DI_NUM];
	HI_U32 ul07DI;
    HI_U8 ucSingleMeterTime; //每块表预留探测时间 单位:秒
    HI_U8 ucTurnTime; //发送电表地址列表Plc两轮时间间隔 单位:分钟
    HI_U8 reserved[2];
}MRS_CCO_DETECT_PARAM_S;

typedef struct
{
	HI_U8 ucJoinWhiteFlag : 1; // 0-不允许加入白名单  1-允许加入白名单
	HI_U8 ucSetWaitTopoFlag : 2; // 0-未知  1-需要在表档案通知MAC时设置20分钟等待站点入网,表档案由空变为非空时会设置该标志 2-表档案被清空通知MAC时设置,之后表档案一旦添加上该标志设置为1
	HI_U8 ucRmFlag : 1; //0-不在探测透抄抄表 1-正在探测透抄,已发送透抄帧
	HI_U8 ucExternFlag : 1; //外部操作探测抄表机制使能 0-外部无操作  1-外部有操作
	HI_U8 ucRefreshWhiteFlag : 1;  //刷新白名单定时器监控标志  0-定时器关闭,不在监控  1-定时器开启标志,在监控
	HI_U8 reserved : 2;
	HI_U8 ucStatus;
	HI_U8 ucCurrTurn;
	HI_U8 uc97DIIndex;
	MRS_ARCHIVES_NODE_STRU stNode;
	HI_U16 usSeq; //CCO发送探测表地址列表给采集器模块的plc序号
	HI_U8 ucOriginalEnable; //外部使能前的原始开关
	HI_U8 ucCacheMacNum;
	HI_U8 pMacBuf[MRS_CCO_DETECT_MAC_BUFFER_MAX]; //临时存放被拒站点mac地址缓存区 可临时存20个
}MRS_CCO_DETECT_RUNNING_S;

typedef struct
{
	MRS_CCO_DETECT_RUNNING_S stRunning;
	MRS_CCO_DETECT_WHITE_QUEUE stWhiteListQueue;
	MRS_CCO_DETECT_BLACK_LIST stBlackList;
	MRS_CCO_DETECT_PARAM_S stParam;
}MRS_CCO_DETECT_RM_CTX_S;


MRS_CCO_DETECT_RM_CTX_S* mrsCcoGetDetectRmCtx(HI_VOID);
HI_U8 mrsGetDetectRmEnable(HI_VOID);
HI_U8 mrsGetDetectRmStatus(HI_VOID);
HI_U32 mrsCcoDetectRmInit(HI_VOID);
HI_VOID mrsCcoRejectNodeCallback (HI_IN HI_MDM_NETWORK_REJECT_ENTRY_S *);
HI_BOOL mrsIsDetectRm(HI_VOID);
HI_U32 mrsDetectRmRespProc(HI_U8 *pucMeter);
HI_VOID mrsDetectRmBreakProc(HI_VOID);
HI_VOID mrsResumeDetectRmProc(HI_VOID);
HI_U32 mrsMacJoinWhiteMsgProc(HI_SYS_QUEUE_MSG_S *pstMsg);
HI_U32 mrsDetectWhiteListAddTimeout(HI_VOID);
HI_U32 mrsDetectWhiteListKeepTimeOut(HI_VOID);
MRS_CCO_DETECT_WHITE_QUEUE_ITEM* mrsDetectWhiteItemQuery(HI_U16 usIndex);
HI_VOID mrsDetectWaitTopoInit(HI_VOID);
HI_U32 mrsDetectWaitTopoTimeOut(HI_VOID);
HI_U32 mrsCcoDetectMeterChgToMacProc(HI_VOID);
HI_VOID mrsCcoDetectEmptyArchivesAddProc(HI_VOID);
HI_U32 mrsCcoDetectTopoChgProc(HI_U8 ucStatus);
HI_U32 mrsDetectJoinNetTimeout(HI_VOID);
HI_U32 mrsCcoDetectRmReset(HI_VOID);
HI_VOID mrsCcoDetectPlcDisconnectProc(HI_VOID);
HI_VOID mrsDetectLrRmTimeout(MRS_ARCHIVES_NODE_STRU *pstNode);
HI_U16 mrsGetDetectWhiteListNum(HI_VOID);
HI_U32 mrsDetectRxModeNotify(HI_U8 ucMode);
HI_U32 mrsCcoDetectRmNodeProc(HI_VOID);
HI_U32 mrsCheckDetectEndProc(HI_VOID);
HI_U32 mrsCcoDetectRmTimeoutProc(HI_U8 *pucMeter);
HI_U32 mrsResumeDetectRmSendMsg(HI_VOID);
HI_VOID mrsSetDetectRmFlag(HI_U8 ucFlag);
HI_VOID mrsDetectBlackListTimeout(HI_VOID);
HI_U32 mrsCcoDetectWhiteSetMacList(HI_U16 usNum, HI_MAC_VALID_MAC_LIST_STRU * pstMeterList);
HI_U16 mrsGetDetectWhiteListValidNum(HI_VOID);
HI_U32 mrsDetectWhiteListCheckProc(HI_VOID);
HI_U32 mrsCcoDetectRmStart(HI_VOID);

/**
 @brief  Detect module timout function, called when parallel meter-reading timeout. 
 @brief  CNcomment: 探测模块超时处理函数，并发抄表超时时调用。CNend
 @param[in] pMeterAddr  Meter address.  CNcomment: 电表地址。CNend
 */
HI_VOID mrsDetectPrRmTimeout(HI_U8 *pucMeter);


//*****************************************************************************
//*****************************************************************************


#endif // defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)

HI_END_HEADER

#endif // _MRS_SRV_CCO_DETECT_RM_H_


