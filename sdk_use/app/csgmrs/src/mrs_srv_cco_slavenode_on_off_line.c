//*****************************************************************************
//
//                  版权所有 (C), 1998-2015, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_srv_cco_slavenode_on_off_line.c
//  版 本 号   : V1.0 
//  作    者   : liupeidong/00238134
//  生成日期   : 2015-03-10
//  功能描述   : CCO载波从节点的上线离线上报函数的定义、声明等
//               
//  函数列表   : TODO: ...
//  修改历史   : 
//  1.日    期 : 2015-03-10
//    作    者 : liupeidong/00238134
//    修改内容 : 创建文件 
//
//*****************************************************************************
#include "hi_mdm.h"
#include "mrs_common.h"
#include "mrs_fw_n.h"
#include "mrs_fw_tools.h"
#include "mrs_fw_nv.h"
#include "mrs_srv_queue.h"
#include "mrs_srv_res.h"
#include "mrs_srv_cco_queue.h"
#include "mrs_fw_proto376_2_frame.h"
#include "mrs_fw_proto376_2_echo.h"
#include "mrs_srv_cco_slavenode_on_off_line.h"
#include "mrs_fw_log.h"


#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)

// 激活报文加入CCO公共队列
#define mrsActivateChgFrameJoinCcoQueue()   MRS_StartTimer(EN_MRS_SRV_CCO_NODE_STATE_CHG_TIMER, 0, HI_SYS_TIMER_ONESHOT)

// CCO从节点在线状态变化上报上下文全局变量
MRS_NODE_STATE_CHG_REPORT_CTX g_stChgReport;


//*****************************************************************************
// 函数名称: mrsGetChgReportCtx
// 功能描述: 获取从节点在线状态变化上下文全局变量地址
//
// 参数说明:
//   无
//
// 返 回 值:
//   指向从节点在线状态变化上下文全局变量的指针
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: liupeidong/00238134 [2015-03-23]
//*****************************************************************************
MRS_NODE_STATE_CHG_REPORT_CTX* mrsGetChgReportCtx(HI_VOID)
{
    return &g_stChgReport;
}


//*****************************************************************************
// 函数名称: mrsCcoNodeStateChgReportInit
// 功能描述: 上电初始化CCO从节点在线状态变化上报上下文全局变量的内容
//           加载0x408 NV上报参数配置，注册/反注册MAC提供的接口
// 参数说明:
//   无
//
// 返 回 值:
//   无
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: liupeidong/00238134 [2015-03-23]
//*****************************************************************************
HI_VOID mrsCcoNodeStateChgReportInit(HI_VOID)
{
    (hi_void)memset_s(&g_stChgReport, sizeof(g_stChgReport), 0, sizeof(g_stChgReport));    
    mrsSrvInitQueue(&g_stChgReport.stCache.stQueue);
    mrsSrvInitQueue(&g_stChgReport.stFrame.stQueue);
    MRS_LOCK_INIT(g_stChgReport.ucJoinCcoQueueLock);
    mrsNvNodeStateChgReportCfg(ID_NV_APP_STATE_CHG_REPORT_CFG);
}


//*****************************************************************************
// 函数名称: mrsMACReportChgEntryCallback
// 功能描述: 通过MAC提供的接口注册到MAC的回调函数，运行在MAC的任务里。保存MAC提
//           供的一条从节点状态变化信息，然后根据缓存元素个数判断下一步的处理。
//           如果超过上限，则将缓存中所有信息组成一帧，发消息给MRS处理; 否则起
//           超时定时器。
// 参数说明:
//   HI_MAC_NETWORK_TOPOCHG_ENTRY_S* pstEntry MAC给的一条从节点状态变化信息
//
// 返 回 值:
//   无
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: liupeidong/00238134 [2015-03-23]
//*****************************************************************************
HI_VOID mrsMACReportChgEntryCallback(HI_IN HI_MDM_NETWORK_TOPOCHG_ENTRY_S* pstEntry)
{
    MRS_NODE_STATE_CHG_REPORT_CTX* pstCtx = mrsGetChgReportCtx();
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U8* pucFrame = HI_NULL;
    HI_U16 usFrameLen = 0;
    HI_U8  ucEntryNum = 0;

    if (HI_NULL == pstEntry || HI_NULL == pstCtx)
    {
        return;
    }

    HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_001, HI_DIAG_MT("chg callback, Mac[5]=%d, OfflineReason=%d"),pstEntry->aucMac[5],pstEntry->ucOfflineReason);
    
    MRS_WaitSem(EN_APP_SEM_MRS_ND_STATE_CHG, HI_SYS_WAIT_FOREVER);  // 信号量，加锁
    do 
    {
        ulRet = mrsSaveChgEntry(pstEntry);
        if (HI_ERR_SUCCESS != ulRet)
        {
            break;
        }
        
        if (pstCtx->stCache.ucNum >= pstCtx->ucCacheNumMax)
        {
            ulRet = mrsAssembleAllCacheEntries(&pucFrame, &usFrameLen, &ucEntryNum);
            if (HI_ERR_SUCCESS != ulRet)
            {
                break;
            }
            MRS_StopTimer(MRS_CCO_TIMER_NODE_STATECHG_ENTRY_CACHE);
            ulRet = mrsNodeStateChgSendMsg((HI_U32)pucFrame, usFrameLen, ucEntryNum, 0);
            if (HI_ERR_SUCCESS != ulRet)
            {
                mrsToolsFree(pucFrame);
                break;                
            }
            HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_002, HI_DIAG_MT("chg callback send msg success"));
        }
        else
        {
            MRS_StartTimer(MRS_CCO_TIMER_NODE_STATECHG_ENTRY_CACHE, 
                           pstCtx->usCacheTimeout * MRS_TIME_100_MILLI_SEC, 
                           HI_SYS_TIMER_ONESHOT);
            HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_003, HI_DIAG_MT("chg callback start timer"));
        }
    }while (0);
    MRS_SignalSem(EN_APP_SEM_MRS_ND_STATE_CHG); // 信号量，解锁

    return;
}


//*****************************************************************************
// 函数名称: mrsSaveChgEntry
// 功能描述: 保存一条从节点状态变化信息到缓存
// 
// 参数说明:
//   HI_MAC_NETWORK_TOPOCHG_ENTRY_S* pstEntry MAC给的一条从节点状态变化信息
//
// 返 回 值:
//   HI_ERR_FAILURE         保存失败
//   HI_ERR_MALLOC_FAILUE   申请动态内存失败
//   HI_ERR_SUCCESS         保存成功
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: liupeidong/00238134 [2015-03-23]
//*****************************************************************************
HI_U32 mrsSaveChgEntry(HI_MDM_NETWORK_TOPOCHG_ENTRY_S* pstEntry)
{
    MRS_NODE_STATE_CHG_REPORT_CTX* pstCtx = mrsGetChgReportCtx();
    MRS_CHG_ENTRY_QUEUE_ITEM* pstEntryItem = HI_NULL;

    if (HI_NULL == pstCtx || HI_NULL == pstEntry)
    {
        return HI_ERR_FAILURE;
    }

    pstEntryItem = mrsToolsMalloc(sizeof(MRS_CHG_ENTRY_QUEUE_ITEM));
    if (HI_NULL == pstEntryItem)
    {
        return HI_ERR_MALLOC_FAILUE;
    }

    (hi_void)memset_s(pstEntryItem, sizeof(MRS_CHG_ENTRY_QUEUE_ITEM), 0, sizeof(MRS_CHG_ENTRY_QUEUE_ITEM));
    (hi_void)memcpy_s(&pstEntryItem->stEntry, sizeof(HI_MDM_NETWORK_TOPOCHG_ENTRY_S), pstEntry, sizeof(HI_MDM_NETWORK_TOPOCHG_ENTRY_S));
    mrsSrvEnQueue(&(pstCtx->stCache.stQueue), &(pstEntryItem->stLink));
    pstCtx->stCache.ucNum++;
    mrsDfxCcoSaveMACReportChgEntry(pstEntry);
    
    return HI_ERR_SUCCESS;
}


//*****************************************************************************
// 函数名称: mrsAssembleAllCacheEntries
// 功能描述: 将缓存中所有的条目组成一帧
// 
// 参数说明:
//   HI_OUT HI_U8** ppucFrame    组成的帧的地址
//   HI_OUT HI_U16* pusFrameLen  组成的帧长的地址
//   HI_OUT HI_U8* pucEntryNum   帧里存放的条目数
//
// 返 回 值:
//   HI_ERR_FAILURE         参数或上下文错误
//   HI_ERR_MALLOC_FAILUE   申请动态内存失败
//   HI_ERR_SUCCESS         组帧成功
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: liupeidong/00238134 [2015-03-23]
//*****************************************************************************
HI_U32 mrsAssembleAllCacheEntries(HI_OUT HI_U8** ppucFrame, HI_OUT HI_U16* pusFrameLen, HI_OUT HI_U8* pucEntryNum)
{
    MRS_NODE_STATE_CHG_REPORT_CTX* pstCtx = mrsGetChgReportCtx();
    MRS_CHG_ENTRY_QUEUE_ITEM* pstQueueItem = HI_NULL;
    HI_MDM_NETWORK_TOPOCHG_ENTRY_S* pstEntry = HI_NULL;
    PROTO376_2_FRAME_AFN_AND_DATA_STRU stAfnData = { 0 };
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U8  ucNum = 0;
    HI_U8* pucData = HI_NULL;
    HI_U16 usDataLen = 0;
    HI_U16 usOffset = 0;
    HI_U16 usTotal = 0;
    HI_U16 usStartIndex = 0;
    HI_U8  ucIndex = 0;
    
    if (HI_NULL == pstCtx || HI_NULL == ppucFrame || HI_NULL == pusFrameLen || HI_NULL == pucEntryNum)
    {
        return HI_ERR_FAILURE;
    }

    ucNum = pstCtx->stCache.ucNum;
    *pucEntryNum = ucNum;
    
    usDataLen = MRS_NODE_STATE_CHG_DATA_HEADER_LEN + ucNum * MRS_ONE_NODE_STATE_CHG_INFO_LEN;
    usTotal = (HI_U16)ucNum;

    pucData = mrsToolsMalloc(usDataLen);
    if (HI_NULL == pucData)
    {
        return HI_ERR_MALLOC_FAILUE;
    }

    (hi_void)memset_s(pucData, usDataLen, 0, usDataLen);
    // 上报从节点总数量
    (hi_void)memcpy_s(pucData, usDataLen, &usTotal, sizeof(usTotal));
    usOffset += sizeof(usTotal);
    // 本帧上报的从节点数量
    pucData[usOffset++] = ucNum;
    // 本帧上报的第一个从节点在总数中的起始序号，从0开始
    (hi_void)memcpy_s(pucData + usOffset, usDataLen - usOffset, &usStartIndex, sizeof(usStartIndex));
    usOffset += sizeof(usStartIndex);
    // 从节点状态变化信息
    for (ucIndex = 0; ucIndex < ucNum; ucIndex++)
    {
        pstQueueItem = (MRS_CHG_ENTRY_QUEUE_ITEM*)mrsSrvDeQueue(&(pstCtx->stCache.stQueue));
        if (HI_NULL == pstQueueItem)
        {
            continue;
        }
        
        pstEntry = &(pstQueueItem->stEntry);
        // 从节点MAC地址
        mrsHexInvert(pstEntry->aucMac, HI_PLC_MAC_ADDR_LEN);
        (hi_void)memcpy_s(pucData + usOffset, usDataLen - usOffset, pstEntry->aucMac, HI_PLC_MAC_ADDR_LEN);
        usOffset += HI_PLC_MAC_ADDR_LEN;
        // 从节点状态变化
        pucData[usOffset++] = pstEntry->ucStateChange;
        // 从节点离线时长
        if (HI_ND_STATE_CHANGE_ONLINE_TO_OFFLINE == pstEntry->ucStateChange)
        {
            pstEntry->ulOfflineDuration = MRS_ONLINE_TO_OFFLINE_DURATION_DEFAULT;
            pstEntry->ucOfflineReason = 0;
        }
        (hi_void)memcpy_s(pucData + usOffset, usDataLen - usOffset, &pstEntry->ulOfflineDuration, sizeof(pstEntry->ulOfflineDuration));
        usOffset += sizeof(pstEntry->ulOfflineDuration);
        // 从节点离线原因
        mrsOfflineReasonFilter(pstCtx->ucPowerOffReasonSwitch, pstCtx->ucPlcChlReasonSwitch, 
                               pstEntry->ucStateChange, &pstEntry->ucOfflineReason);
        pucData[usOffset++] = pstEntry->ucOfflineReason;

        mrsToolsFree(pstQueueItem);
        pstCtx->stCache.ucNum--;
    }

    (hi_void)memset_s(&stAfnData, sizeof(stAfnData), 0, sizeof(stAfnData));
    stAfnData.ucAFN = MRS_AFN(0x06);
    stAfnData.ucDT[0] = MRS_AFN_FN_LO(10);
    stAfnData.ucDT[1] = MRS_AFN_FN_HI(10);
    stAfnData.pData = pucData;
    stAfnData.usDataLen = usDataLen;

    ulRet = mrsCreate3762UpFrame(&stAfnData, ppucFrame, pusFrameLen, HI_NULL);
    if (HI_ERR_SUCCESS == ulRet)
    {
        HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1000, HI_DIAG_MT("chg assemble frame success"), *ppucFrame, *pusFrameLen);
    }

    mrsToolsFree(pucData);
    return ulRet;
}


//*****************************************************************************
// 函数名称: mrsOfflineReasonFilter
// 功能描述: 根据NV配置的开关决定是否将某类离线原因改成未知
// 
// 参数说明:
//   HI_U8 ucPowerOffReasonSwitch, 掉电原因开关，打开允许上报，关闭将原因改成未知
//   HI_U8 ucPlcChlReasonSwitch,   信道原因开关，打开允许上报，关闭将原因改成未知
//   HI_U8 ucChange,               在线状态的变化
//   HI_OUT HI_U8* pucReason       离线原因的地址
//
// 返 回 值:
//   无
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: liupeidong/00238134 [2015-03-23]
//*****************************************************************************
HI_VOID mrsOfflineReasonFilter(HI_U8 ucPowerOffReasonSwitch, HI_U8 ucPlcChlReasonSwitch, 
                               HI_U8 ucChange, HI_OUT HI_U8* pucReason)
{
    HI_U8 bFilter = HI_FALSE;
    
    if (HI_ND_STATE_CHANGE_OFFLINE_TO_ONLINE == ucChange)
    {
        if (!ucPowerOffReasonSwitch && (HI_ND_OFFLINE_REASON_METER_POWEROFF == *pucReason))
        {
            bFilter = HI_TRUE;
        }

        if (!ucPlcChlReasonSwitch && (HI_ND_OFFLINE_REASON_PLC_CHANNEL == *pucReason))
        {
            bFilter = HI_TRUE;
        }
    }

    if (bFilter)
    {
        *pucReason = HI_ND_OFFLINE_REASON_UNKNOWN;
    }
}


//*****************************************************************************
// 函数名称: mrsNodeStateChgSendMsg
// 功能描述: 在MAC任务里运行的回调函数中发消息给MRS，将组成的帧和长度传给MRS处理
// 
// 参数说明:
//   HI_U32 ulFramePtr, 组成的帧的地址
//   HI_U32 ulFrameLen, 组成的帧长
//   HI_U32 ulEntryNum, 组成的帧中的条目数
//   HI_U32 ulParam4    凑数的，至少要4个参数
//   
// 返 回 值:
//   HI_ERR_SUCCESS         发消息给MRS成功
//   其他                   发消息给MRS失败
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: liupeidong/00238134 [2015-03-23]
//*****************************************************************************
HI_U32 mrsNodeStateChgSendMsg(HI_U32 ulFramePtr, HI_U32 ulFrameLen, HI_U32 ulEntryNum, HI_U32 ulParam4)
{
    HI_SYS_QUEUE_MSG_S stMsg = {0};

    (hi_void)memset_s(&stMsg, sizeof(stMsg), 0, sizeof(stMsg));
    stMsg.ulMsgId = EN_MRS_FW_MSG_MAC_ND_STATE_CHG;
    stMsg.ulParam[0] = ulFramePtr;
    stMsg.ulParam[1] = ulFrameLen;
    stMsg.ulParam[2] = ulEntryNum;
    stMsg.ulParam[3] = ulParam4;

    return mrsSendMessage2Queue(&stMsg);
}


//*****************************************************************************
// 函数名称: mrsNodeStateChgMsgProc
// 功能描述: MRS收到MAC任务发从节点状态变化上报消息的处理
// 
// 参数说明:
//   HI_SYS_QUEUE_MSG_S* pstMsg  消息的地址
//   
// 返 回 值:
//   HI_ERR_FAILURE         参数或上下文错误或进队列失败
//   HI_ERR_MALLOC_FAILUE   申请动态内存失败
//   HI_ERR_SUCCESS         加入CCO队列成功                 
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: liupeidong/00238134 [2015-03-23]
//*****************************************************************************
HI_U32 mrsNodeStateChgMsgProc(HI_SYS_QUEUE_MSG_S* pstMsg)
{
    HI_U8* pucFrame = HI_NULL;
    HI_U16 usFrameLen = 0;
    HI_U8  ucEntryNum = 0;

    HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_005, HI_DIAG_MT("chg rcv callback msg"));

    pucFrame = (HI_U8*)pstMsg->ulParam[0];
    usFrameLen = (HI_U16)pstMsg->ulParam[1];
    ucEntryNum = (HI_U8)pstMsg->ulParam[2];

    return mrsChgFrameReport(pucFrame, usFrameLen, ucEntryNum);
}


//*****************************************************************************
// 函数名称: mrsChgCacheTimeoutProc
// 功能描述: 缓存元素个数未到上限，收到一条信息后起的定时器响的处理
// 
// 参数说明:
//   无
//   
// 返 回 值:
//   HI_ERR_FAILURE         参数或上下文错误或组帧失败或进队列失败
//   HI_ERR_MALLOC_FAILUE   申请动态内存失败
//   HI_ERR_SUCCESS         加入CCO队列成功     
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: liupeidong/00238134 [2015-03-23]
//*****************************************************************************
HI_U32 mrsChgCacheTimeoutProc(HI_VOID)
{
    MRS_NODE_STATE_CHG_REPORT_CTX* pstCtx = mrsGetChgReportCtx();
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U8* pucFrame = HI_NULL;
    HI_U16 usFrameLen = 0;
    HI_U8  ucEntryNum = 0;

    if (HI_NULL == pstCtx)
    {
        return HI_ERR_FAILURE;
    }

    MRS_WaitSem(EN_APP_SEM_MRS_ND_STATE_CHG, HI_SYS_WAIT_FOREVER);  // 信号量，加锁
    HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_006, HI_DIAG_MT("chg cache timeout, ucCacheNum = %d"), pstCtx->stCache.ucNum);
    ulRet = mrsAssembleAllCacheEntries(&pucFrame, &usFrameLen, &ucEntryNum);
    MRS_SignalSem(EN_APP_SEM_MRS_ND_STATE_CHG); // 信号量，解锁
    if (HI_ERR_SUCCESS != ulRet)
    {
        return ulRet;
    }
    
    return mrsChgFrameReport(pucFrame, usFrameLen, ucEntryNum);
}


//*****************************************************************************
// 函数名称: mrsChgFrameReport
// 功能描述: 组出的帧上报: 先加入上报队列，再加入CCO队列
// 
// 参数说明:
//   HI_U8* pucFrame,   帧的地址
//   HI_U16 usFrameLen  帧长
//   HI_U8  ucEntryNum  帧中的条目数
//   
// 返 回 值:
//   HI_ERR_FAILURE         参数或上下文错误或进队列失败
//   HI_ERR_MALLOC_FAILUE   申请动态内存失败
//   HI_ERR_SUCCESS         加入CCO队列成功
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: liupeidong/00238134 [2015-03-23]
//*****************************************************************************
HI_U32 mrsChgFrameReport(HI_U8* pucFrame, HI_U16 usFrameLen, HI_U8 ucEntryNum)
{
    MRS_NODE_STATE_CHG_REPORT_CTX* pstCtx = mrsGetChgReportCtx();
    MRS_CHG_FRAME_QUEUE_ITEM* pstFrameItem = HI_NULL;
    HI_U32 ulRet = HI_ERR_SUCCESS;

    do
    {
        if (HI_NULL == pstCtx || HI_NULL == pucFrame || 0 == usFrameLen || 0 == ucEntryNum)
        {
            // pucFrame之外的条件成立，要保证释放pucFrame内存
            ulRet = HI_ERR_FAILURE;
            break;
        }

        pstFrameItem = mrsToolsMalloc(sizeof(MRS_CHG_FRAME_QUEUE_ITEM));
        if (HI_NULL == pstFrameItem)
        {
            ulRet = HI_ERR_MALLOC_FAILUE;
            break;
        }

        (hi_void)memset_s(pstFrameItem, sizeof(MRS_CHG_FRAME_QUEUE_ITEM), 0, sizeof(MRS_CHG_FRAME_QUEUE_ITEM));
        pstFrameItem->pucFrame = pucFrame;
        pstFrameItem->usFrameLen = usFrameLen;
        pstFrameItem->ucEntryNum = ucEntryNum;
        
        mrsSrvEnQueue(&(pstCtx->stFrame.stQueue), &(pstFrameItem->stLink));
        pstCtx->stFrame.usNum++;
        mrsDfxCcoChgFrameQueueStats(HI_TRUE, ucEntryNum);
      
    } while (0);

    if (HI_ERR_SUCCESS != ulRet)
    {
        // 所有入队列不成功的情况，都要释放报文内存
        mrsToolsFree(pucFrame);
    }
    else
    {
        ulRet = mrsChgFrameJoinCcoQueue();
    }
    
    return ulRet;
}


//*****************************************************************************
// 函数名称: mrsChgFrameJoinCcoQueue
// 功能描述: 组出的从节点在线状态变化帧加入CCO队列
// 
// 参数说明:
//   无
//   
// 返 回 值:
//   HI_ERR_FAILURE         参数或上下文错误
//   HI_ERR_MALLOC_FAILUE   申请动态内存失败
//   HI_ERR_SUCCESS         加CCO队列成功
//   HI_ERR_BUSY            CCO队列中有正在上报的变化帧，暂时不允许加入
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: liupeidong/00238134 [2015-03-23]
//*****************************************************************************
HI_U32 mrsChgFrameJoinCcoQueue(HI_VOID)
{
    MRS_NODE_STATE_CHG_REPORT_CTX* pstCtx = mrsGetChgReportCtx();
    MRS_CHG_FRAME_QUEUE_ITEM* pstFrameItem = HI_NULL;
    MRS_SRV_CCO_ITEM *pstCcoItem = HI_NULL;
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U16 usFrameLen = 0;

    if (HI_NULL == pstCtx)
    {
        return HI_ERR_FAILURE;
    }
    
    pstFrameItem = (MRS_CHG_FRAME_QUEUE_ITEM*)mrsSrvQueueTop(&(pstCtx->stFrame.stQueue));
    if (HI_NULL == pstFrameItem)
    {
        return HI_ERR_FAILURE;
    }

    if (MRS_TRY_LOCK(pstCtx->ucJoinCcoQueueLock))
    {
        HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_007, HI_DIAG_MT("chg JoinCcoQueueLock OFF->ON, usFrameQueueLen=%d"), pstCtx->stFrame.usNum);

        usFrameLen = pstFrameItem->usFrameLen;
        pstCcoItem = (MRS_SRV_CCO_ITEM*)mrsToolsMalloc(sizeof(MRS_SRV_CCO_ITEM) + usFrameLen);
        if (HI_NULL == pstCcoItem)
        {
            MRS_UNLOCK(pstCtx->ucJoinCcoQueueLock);
            return HI_ERR_MALLOC_FAILUE;
        }

        (hi_void)memset_s(pstCcoItem, sizeof(MRS_SRV_CCO_ITEM) + usFrameLen, 0, sizeof(MRS_SRV_CCO_ITEM) + usFrameLen);
        pstCcoItem->bValid = HI_TRUE;
        pstCcoItem->ucAfn = 0x06;
        pstCcoItem->usFn = 0x10;
        pstCcoItem->usTimeOut = pstCtx->usReportTimeout;
        pstCcoItem->ucMaxTry = pstCtx->ucReportRetryMax;
        pstCcoItem->usDataLen = usFrameLen;
        (hi_void)memcpy_s(pstCcoItem->pData, pstCcoItem->usDataLen, pstFrameItem->pucFrame, pstCcoItem->usDataLen);
        pstCcoItem->MrsCcoRxRespProc = mrsChgReportRxRespProc;
        pstCcoItem->MrsCcoRxTotalTimeOutProc = mrsChgReportTimeoutProc;

        mrsCcoJoinQueue(pstCcoItem);

        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_008, HI_DIAG_MT("chg join cco queue success"));
        mrsDfxCcoChgJoinCcoQueueStats(pstFrameItem->ucEntryNum);
        pstFrameItem = (MRS_CHG_FRAME_QUEUE_ITEM*)mrsSrvDeQueue(&(pstCtx->stFrame.stQueue));
        if (pstFrameItem)
        {
            mrsDfxCcoChgFrameQueueStats(HI_FALSE, pstFrameItem->ucEntryNum);
            mrsToolsFree(pstFrameItem->pucFrame);
            mrsToolsFree(pstFrameItem);
            pstCtx->stFrame.usNum--;
        }

        mrsActiveCcoQueue();
    }
    else
    {
        // 已经有锁，CCO公共队列中的一帧站点在线状态变化帧没处理完，加锁失败
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_009, HI_DIAG_MT("chg JoinCcoQueueLock is ON, join fail"));
        ulRet = HI_ERR_BUSY;
    }
    
    return ulRet;
}


//*****************************************************************************
// 函数名称: mrsChgReportRxRespProc
// 功能描述: CCO上报集中器后收到确认帧/否认帧
// 
// 参数说明:
//   HI_VOID* pParam  占位的参数，实际无用
//   
// 返 回 值:
//   无
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: liupeidong/00238134 [2015-03-23]
//*****************************************************************************
HI_VOID mrsChgReportRxRespProc(HI_VOID* pParam)
{
    HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_010, HI_DIAG_MT("chg report rx resp"));
    MRS_NOT_USED(pParam);
    
    mrsDfxCcoChgReportRxResp();
    mrsReportNextChgFrame();
}


//*****************************************************************************
// 函数名称: mrsChgReportTimeoutProc
// 功能描述: CCO上报集中器后，所有的发送在超时时间内都没有收到应答的处理
// 
// 参数说明:
//   HI_VOID* pParam  占位的参数，实际无用
//   
// 返 回 值:
//   HI_ERR_FAILURE         上下文错误
//   HI_ERR_SUCCESS         成功激活下一帧进CCO队列
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: liupeidong/00238134 [2015-03-23]
//*****************************************************************************
HI_U32 mrsChgReportTimeoutProc(HI_VOID* pParam)
{
    HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_011, HI_DIAG_MT("chg report timeout"));
    MRS_NOT_USED(pParam);

    mrsDfxCcoChgReportTimeout();
    return mrsReportNextChgFrame();
}


//*****************************************************************************
// 函数名称: mrsReportNextChgFrame
// 功能描述: 组出的帧上报: 先加入上报队列，再加入CCO队列
// 
// 参数说明:
//   无
//   
// 返 回 值:
//   HI_ERR_FAILURE         上下文错误
//   HI_ERR_SUCCESS         成功激活下一帧进CCO队列
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: liupeidong/00238134 [2015-03-23]
//*****************************************************************************
HI_U32 mrsReportNextChgFrame(HI_VOID)
{
    MRS_NODE_STATE_CHG_REPORT_CTX* pstCtx = mrsGetChgReportCtx();

    if (HI_NULL == pstCtx)
    {
        return HI_ERR_FAILURE;
    }

    MRS_UNLOCK(pstCtx->ucJoinCcoQueueLock);
    mrsActivateChgFrameJoinCcoQueue();
    HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_012, HI_DIAG_MT("chg JoinCcoQueueLock ON->OFF, activate join"));
    
    return HI_ERR_SUCCESS;
}


#endif

