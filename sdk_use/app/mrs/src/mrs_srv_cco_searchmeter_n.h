//*****************************************************************************
//
//                  版权所有 (C), 1998-2011, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_srv_cco_searchmeter_n.h
//  版 本 号   : V1.0 
//  作    者   : 
//  生成日期   : 
//  功能描述   : CCO搜表模块内部函数及接口定义
//               
//  函数列表   : TODO: ...
//  修改历史   : 
//  1.日    期 : 2015-11-23
//    作    者 : cuiate/c00233580
//    修改内容 : 创建文件 
//
//*****************************************************************************

#ifndef __MRS_SRV_CCO_SEARCHMETER_N__H__
#define __MRS_SRV_CCO_SEARCHMETER_N__H__
HI_START_HEADER

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
//*****************************************************************************
//*****************************************************************************

#define MRS_SM_STABLE_TIME_RESET(cco)       do {(cco)->stAutoSearchModel.usStableTimes = 0;} while(0)
#define MRS_SM_STABLE_TIME_INCREASE(cco)    do {(cco)->stAutoSearchModel.usStableTimes++;} while(0)


// 从节点数量(1个字节)
// 从节点通信地址(6个字节)
// 通信协议(1个字节)
// 序号(2个字节)
// 设备类型(1个字节)
// 下接从节点数量(1个字节)
// 本次上报数节点数量(1个字节)
#define SIZEOF_REPORT_FRAME_STRU    (1+6+1+2+1+1+1)
#define MRS_REPORT_NODE_MAX         (32)


typedef struct
{
    HI_U8   aucMac[HI_PLC_MAC_ADDR_LEN];
    HI_U8   aucAsset[HI_METER_ADDR_LEN];
    HI_BOOL bExist;
    HI_U8   padding;
} MRS_CCO_SEARCH_QUEUE_VISIT_STRU;


HI_PRV HI_U32 mrsCcoSearchAssembleReportFrame(MRS_CCO_SRV_CTX_STRU* cco, METER_NODE* node);

HI_PRV HI_VOID mrsCcoSearchReportFrameEnQueue(HI_VOID);

HI_PRV HI_U32 Add2ReportedList(MRS_CCO_SRV_CTX_STRU * cco, MRS_CMD_METERLIST_ITEM * item);

HI_PRV HI_U32 mrsCcoSearchMeterReset(MRS_CCO_SRV_CTX_STRU * cco);

HI_PRV HI_U32 mrsCcoCreateStartSearchMeterPlcFrame(MRS_CCO_SRV_CTX_STRU * cco);

HI_PRV HI_U32 mrsCcoCreateStopSearchMeterPlcFrame(MRS_CCO_SRV_CTX_STRU * cco);

HI_PRV HI_U32 mrsCcoSearchMeterMacListInit(MRS_CCO_SRV_CTX_STRU *cco, HI_MAC_NETWORK_TOPO_S * topo);

HI_PRV HI_VOID mrsCcoSearchMeterMacListRefresh(MRS_CCO_SRV_CTX_STRU *cco, HI_MAC_NETWORK_TOPO_S * topo);

HI_PRV HI_U32 mrsCcoSearchMeterGetTopo(MRS_CCO_SRV_CTX_STRU * cco, HI_MAC_NETWORK_TOPO_S ** topo);

HI_PRV HI_U32 mrsCcoSearchMeterSaveTopo(HI_MAC_NETWORK_TOPO_S * saved_topo, HI_MAC_NETWORK_TOPO_S * new_topo);

HI_PRV HI_U32 mrsCcoCheckTopoChanged(HI_MAC_NETWORK_TOPO_S * old_topo, HI_MAC_NETWORK_TOPO_S * new_topo, HI_BOOL * changed);

HI_PRV HI_U32 mrsCcoCheckSearchFinished(MRS_CCO_SRV_CTX_STRU * cco);

HI_PRV HI_U32 mrsCcoUpdateSearchResult(MRS_CCO_SRV_CTX_STRU * cco, HI_U8 * mac);

HI_PRV HI_U32 mrsCcoUpdateSearchQueryInfo(MRS_CCO_SRV_CTX_STRU * cco, HI_U8 * mac, HI_BOOL bRetFlag);

HI_PRV HI_U32 mrsCcoGetMeterList(MRS_CCO_SRV_CTX_STRU  * cco, MAC_ITEM * item);

HI_PRV HI_VOID mrsCcoQueueFree(HI_VOID * p);

// 检查所有站点搜表结果是否查询完毕
HI_PRV HI_U32 mrsCcoCheckQueryComplete(MRS_CCO_SRV_CTX_STRU * cco);

HI_PRV HI_VOID mrsCcoSearchReportSlaveNodeInfoNormal(HI_VOID);

HI_PRV HI_VOID mrsCcoSearchReportSlaveNodeInfoTransformer(HI_VOID);

HI_PRV HI_U32 mrsStopSearchMeterTransformer(HI_VOID);

HI_PRV HI_U16 mrsCcoSearchMeterMacListDelete(MRS_SEARCH_METER_MAC_LIST *pMacList, HI_MAC_NETWORK_TOPO_S *pTopo);

HI_PRV HI_VOID mrsCcoSearchMeterMacListRealign(MRS_SEARCH_METER_MAC_LIST *pMacList);

HI_PRV HI_VOID mrsCcoSearchMeterMacListAdd(MRS_SEARCH_METER_MAC_LIST *pMacList, HI_MAC_NETWORK_TOPO_S *pTopo);

HI_PRV HI_BOOL mrsCcoSearchMacRepeatCheck(MRS_CCO_SRV_CTX_STRU *pCcoCtx, HI_U8 *pMacAddr, HI_U8 *pAssetAddr);

HI_PRV HI_BOOL mrsCcoSearchMeterIsValidProduct(HI_U8 product_type);

HI_PRV HI_U32 mrsGetCltNodeType(HI_U8 *pucMacAddr, HI_U8 *pucType);

HI_PRV HI_BOOL mrsCcoSearchVisitQueue(HI_VOID *p, HI_VOID *q);

HI_PRV HI_BOOL mrsCcoSearchVisitCltList(HI_VOID *p, HI_VOID *q);

HI_PRV HI_BOOL mrsCcoSearchVisitMeterList(HI_VOID *p, HI_VOID *q);

// CCO端是否开启锁定功能
HI_PRV HI_BOOL mrsCcoBindNetworkEnable(HI_VOID);

// 是否等待收到锁定回复才上报
HI_PRV HI_BOOL mrsCcoSmReportIsWaitBindMsg(HI_VOID);

// 检查所有站点是否锁定完毕
HI_PRV HI_U32 mrsCcoCheckLockComplete(MRS_CCO_SRV_CTX_STRU * pstCcoCtx);

// 向指定站点发送锁定通知
HI_PRV HI_U32 mrsCcoNotifyStaBindNetwork(MRS_CCO_SRV_CTX_STRU *pstCcoCtx, METER_NODE * node);

// 处理站点的锁定回复: 将对应节点从未锁定站点列表，转移到未上报队列
HI_PRV HI_U32 mrsCcoHandleBindNetworkReply(MRS_CCO_SRV_CTX_STRU *pstCcoCtx, HI_U8 * aucMac);

// METER_LIST_NODE 的增删改查操作
HI_VOID mrsCcoSearchMeterFreeMeterListNode(HI_VOID * pstNode, HI_VOID * pstCb);
HI_U32 mrsCcoSearchMeterInsertMeterListNode(MRS_SRV_LIST * pstMeterListHead, METER_NODE * pstMeterNode, HI_U32 buf_len);
HI_BOOL mrsCcoSearchMeterFindMeterListNode(HI_VOID * pstMeterListNode, HI_VOID * pstExist);
HI_BOOL mrsCcoSearchMeterGetMeterListNode(HI_VOID * pstMeterListNode, HI_VOID * pstResult);

// 恢复白名单开关到搜表前状态
HI_PRV HI_U32 mrsCcoRecoverWhiteSwitch(MRS_CCO_SRV_CTX_STRU* pstCcoCtx);
//*****************************************************************************
//*****************************************************************************
#endif // defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)

HI_END_HEADER
#endif //__MRS_SRV_CCO_SEARCHMETER_N__H__

