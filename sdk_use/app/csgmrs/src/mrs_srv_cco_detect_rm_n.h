//*****************************************************************************
//
//                  版权所有 (C), 1998-2016, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_srv_cco_detect_rm_n.h
//  版 本 号   : V1.0 
//  作    者   : fengxiaomin/00209182
//  生成日期   : 2016-01-13
//  功能描述   : 采集器入网全网抄表功能函数及接口内部声明
//               
//  函数列表   : TODO: ...
//  修改历史   : 
//  1.日    期 : 2016-01-13
//    作    者 : fengxiaomin/00209182
//    修改内容 : 创建文件 
//
//*****************************************************************************

#ifndef _MRS_SRV_CCO_DETECT_RM_N_H_
#define _MRS_SRV_CCO_DETECT_RM_N_H_

HI_START_HEADER

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)

//*****************************************************************************
//*****************************************************************************

#define MRS_CCO_DETECT_RM_IDLE      (0)
#define MRS_CCO_WHITE_ADD_NO_RM     (1)
#define MRS_CCO_DETECT_RM_WAITING   (2)
#define MRS_CCO_DETECT_RM_DOING     (3)

#define MRS_CCO_DETECT_DI_VER2007     (0x05060101)
#define MRS_CCO_DETECT_DI_VER1997A    (0x9010)
#define MRS_CCO_DETECT_DI_VER1997B    (0x901F)

#define MRS_CCO_DETECT_ENABLE_DEFAULT         (1)
#define MRS_CCO_WHITE_ADD_MAX_TIME_DEFAULT    (300)
#define MRS_CCO_DETECT_TURN_DEFAULT           (2)
#define MRS_CCO_ALLOW_WHITE_MAX_TIME_DEFAULT  (20)
#define MRS_CCO_STA_JOIN_NET_MAX_TIME_DEFAULT (20)
#define MRS_CCO_BLACK_LIST_VALID_TIME_DEFAULT (5) //5分钟
#define MRS_CCO_JOIN_WHITE_LIST_TIME_DEFAULT  (3) //3分钟
#define MRS_CCO_METER_DETECT_TIME_DEFAULT     (60)//60秒
#define MRS_CCO_DETECT_TURN_TIME_DEFAULT      (3)//3分钟

#define MRS_CCO_DETECT_MAC_CACHE_TIME       (30 * 1000)  //APP收到MAC拒绝入网信息后最大缓存的时间

#define MRS_CCO_FIND_REPEAT_MAC_SUCCESS  (1)
#define MRS_CCO_FIND_REPEAT_MAC_FAIL     (0)

#define MRS_CCO_DETECT_ALLOW_JOIN_WHITE_LIST    (1)
#define MRS_CCO_DETECT_FORBID_JOIN_WHITE_LIST   (0)

#define MRS_CCO_DETECT_WAIT_TOPO_IDLE_STATUS    (0)
#define MRS_CCO_DETECT_NEED_WAIT_TOPO_STATUS    (1)
#define MRS_CCO_DETECT_METER_EMPTY_STATUS       (2)

HI_U32 mrsCcoDetectRmNvInit(HI_VOID);
HI_VOID mrsSetDetectRmEnable(HI_U8 ucEnable);
HI_VOID mrsSetDetectRmStatus(HI_U8 ucStatus);
HI_U8 mrsGetDetectRmExterStatus(HI_VOID);
HI_VOID mrsSetDetectRmExterStatus(HI_U8 ucFlag);
HI_U8 mrsGetDetectOriginalEnable(HI_VOID);
HI_VOID mrsSetDetectOriginalEnable(HI_U8 ucEnable);
HI_U8 mrsGetDetectRmFlag(HI_VOID);
HI_VOID mrsResetDetectRmDIIndex(HI_VOID);
HI_VOID mrsDetectRmDIIndexAddSelf(HI_VOID);
    HI_U8 mrsGetDetectRmDIIndex(HI_VOID);
HI_VOID mrsResetDetectRmTurn(HI_VOID);
HI_VOID mrsDetectRMTurnAddSelf(HI_VOID);
HI_VOID mrsCacheMacNumAddSelf(HI_VOID);
HI_VOID mrsResetCacheMacInfo(HI_VOID);
HI_U8 mrsGetCacheMacNum(HI_VOID);
HI_VOID mrsDetectWhiteListAddSelf(HI_VOID);
HI_VOID mrsResetDetectWhiteListNum(HI_VOID);
HI_VOID mrsSetAllowJoinWhiteFlag(HI_U8 ucFlag);
HI_U8 mrsGetAllowJoinWhiteFlag(HI_VOID);
HI_VOID mrsSetWaitTopoFlag(HI_U8 ucFlag);
HI_U8 mrsGetWaitTopoFlag(HI_VOID);
HI_U16 mrsGetDetectPlcSeq(HI_VOID);
HI_VOID mrsDetectPlcSeqAdd(HI_VOID);
HI_BOOL mrsDetectMeterRespProc(HI_U8 *pucMeter);
HI_U32 mrsCcoRejectNodeSendMsg(MRS_CCO_DETECT_RUNNING_S* pstRunning);
HI_U32 mrsDetectRefreshWhiteList(HI_U8 *pData, HI_U32 ulDataLen);
HI_U32 mrsCcoSaveCltInfo(HI_U8 *pData, HI_U32 ulDataLen);
HI_BOOL mrsCcoFindRepeatMacItem(HI_VOID *node, HI_VOID *param);
HI_U32 mrsCheckAllowJoinWhiteList(HI_IN HI_MDM_NETWORK_REJECT_ENTRY_S *pstEntry);
HI_VOID mrsDetectTopoChgPreProc(HI_U8 ucStatus);
HI_U32 mrsDetectRmStartProc(HI_U8 ucStatus);
HI_U32 mrsDetectCheckStart(HI_VOID);
HI_U32 mrsCheckTopoDetectWhiteExist(HI_VOID);
HI_U32 mrsCcoCheckArchivesExistMap(HI_VOID);
HI_U32 mrsCcoCheckArchivesMapProc(HI_VOID);
HI_U32 mrsDetectRmTransmitProc(HI_VOID);
HI_U32 mrsDetectAssemble645Frame(MRS_ARCHIVES_NODE_STRU *pstNode, HI_U8 **ppData, HI_U16 *pusDataLen);
HI_U32 mrsDetectRmFindValidNode(HI_VOID);
HI_U32 mrsCheckRmStatus(HI_VOID);
HI_U32 mrsCcoDetectRmEnd(HI_VOID);
HI_VOID mrsArchivesDetectInfoReset(HI_VOID);
HI_VOID mrsDetectQueueFree(HI_VOID * p);
HI_VOID mrsDetectRmExterDisable(HI_VOID);
HI_VOID mrsDetectArchivesMeterTurnAdd(HI_U8* pucMeter);
HI_VOID mrsDetectRmExterResume(HI_VOID);
HI_U32 mrsCcoDetectSaveBlackList(HI_VOID);
HI_U32 mrsCcoDetectBlackAdd(HI_U8 *pucMac);
MRS_CCO_DETECT_BLACK_LIST_ITEM* mrsDetectBlackInfoQuery(HI_U16 usIndex);
HI_BOOL mrsDetectIsStaExistBlackList(HI_U8 *pucMacAddr);
HI_BOOL mrsDetectBlackListFindRepeatItem(HI_VOID *node, HI_VOID *param);
HI_U16 mrsGetDetectBlackListNum(HI_VOID);
HI_VOID mrsDetectBlackListAddSelf(HI_VOID);
HI_VOID mrsDetectBlackListDecSelf(HI_VOID);
HI_U8 mrsGetBlackValidTime(HI_VOID);
HI_U32 mrsDetectBlackRemove(MRS_CCO_DETECT_BLACK_LIST_ITEM* pstItem);
HI_VOID mrsResetDetectWhiteListValidNum(HI_VOID);
HI_VOID mrsDetectWhiteListValidAddSelf(HI_VOID);
HI_U8 mrsGetJoinWhiteTime(HI_VOID);
HI_U8 mrsGetRefreshWhiteFlag(HI_VOID);
HI_VOID mrsSetRefreshWhiteFlag(HI_U8 ucFlag);
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
HI_U32 mrsDetectSendPlcProc(HI_VOID);
HI_U32 mrsDetectGetMeterList(APP_CSG_METER_INFO **ppInfo, HI_U8 *pucNum);
#endif


//*****************************************************************************
//*****************************************************************************


#endif // defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)

HI_END_HEADER

#endif // _MRS_SRV_CCO_DETECT_RM_N_H_


