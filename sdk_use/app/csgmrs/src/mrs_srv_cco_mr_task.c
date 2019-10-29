//*****************************************************************************
//
//                  版权所有 (C), 2001-2014, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_srv_cco_mr_task.c
//  版 本 号   : V1.0
//  作    者   : cuiate/c00233580
//  生成日期   : 2015-12-09
//  功能描述   : TODO: ...
//
//  函数列表   : TODO: ...
//  修改历史   :
//  1.日    期 : 2015-12-09
//    作    者 : cuiate/c00233580
//    修改内容 : 创建文件
//
//*****************************************************************************

#include "mrs_common.h"
#include "mrs_fw_tools.h"
#include "mrs_fw_log.h"
#include "mrs_fw_n.h"
#include "mrs_fw_proto376_2_echo.h"
#include "mrs_srv_res.h"
#include "mrs_srv_cco_queue.h"
#include "mrs_srv_cco_macmap.h"
#include "mrs_srv_cco_broadcast.h"
#include "mrs_srv_list.h"
#include "mrs_srv_parallel_cco.h"
#include "mrs_srv_cco.h"
#include "mrs_fw_proto_csg_echo.h"
#include "mrs_fw_proto_csg.h"
#include "mrs_srv_cco_mr_task.h"
#include "mrs_srv_cco_mr_task_n.h"
#include "mrs_srv_anytime_readmeter.h"
#include "mrs_srv_cco_archives.h"
#include "mrs_srv_cco_testmode.h"
#include "mrs_time.h"
#include "mrs_srv_readmeter_manage.h"
HI_START_HEADER


#if (defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) && defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD))

HI_PRV MRS_CCO_MR_TASK_CONTEXT g_stCcoMrTaskCtx = {0};


HI_VOID mrsCcoMrTaskItemFree(HI_VOID *pNode, HI_VOID *pParam)
{
    MRS_CCO_MR_TASK_ITEM_STRU *pItem = (MRS_CCO_MR_TASK_ITEM_STRU *)pNode;

    HI_UNREF_PARAM(pParam);

    if (pItem)
    {
        if (MRS_CCO_MR_TASK_TYPE_MULTICAST == pItem->ucType)
        {
            mrsToolsFree(pItem->unParam.stMulticast.pSubNode);
            mrsSrvFreeBits(&pItem->unParam.stMulticast.pBitmap);
        }

        mrsToolsFree(pItem->pFrame);
        mrsToolsFree(pItem);
    }
}


MRS_CCO_MR_TASK_CONTEXT *mrsCcoGetMrTaskCtx(HI_VOID)
{
    return &g_stCcoMrTaskCtx;
}


HI_U32 mrsCcoMrTaskInit(HI_VOID)
{
    (hi_void)memset_s(&g_stCcoMrTaskCtx, sizeof(g_stCcoMrTaskCtx), 0, sizeof(g_stCcoMrTaskCtx));
    g_stCcoMrTaskCtx.usTaskMax = MRS_CCO_MR_TASK_MAX;
    g_stCcoMrTaskCtx.usTaskNum = 0;
    g_stCcoMrTaskCtx.pstProcItem = HI_NULL;

    mrsSrvInitList(&g_stCcoMrTaskCtx.stList);

    g_stCcoMrTaskCtx.ucStatus = MRS_CCO_MR_TASK_ENABLE;

    return HI_ERR_SUCCESS;
}


HI_U32 mrsCcoMrTaskDeinit(HI_VOID)
{
    mrsCcoMrTaskDeleteAll();

    g_stCcoMrTaskCtx.usTaskNum = 0;
    g_stCcoMrTaskCtx.pstProcItem = HI_NULL;

    return HI_ERR_SUCCESS;
}


HI_BOOL mrsCcoMrTaskIsFull(HI_VOID)
{
    MRS_CCO_MR_TASK_CONTEXT *pstCtx = mrsCcoGetMrTaskCtx();
    return (HI_BOOL)(pstCtx->usTaskNum == pstCtx->usTaskMax);
}


HI_BOOL mrsCcoMrTaskIsInList(HI_U16 usId)
{
    return (HI_BOOL)(HI_NULL != mrsCcoMrTaskGetItem(usId));
}


HI_U32 mrsCcoMrTaskInsertUnicast(MRS_CSG_FRAME_STRU *pFrameData, HI_U8 *pucDenyCode)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    MRS_CCO_MR_TASK_ITEM_STRU *pItem = HI_NULL;
    HI_U8 *pAppData = pFrameData->aucAppData;
    HI_U16 usAppDataLen = pFrameData->usAppDataLen;
    HI_U16 usId = 0;
    MRS_CSG_MR_TASK_UNICAST_STRU *pstFrame = (MRS_CSG_MR_TASK_UNICAST_STRU *)pAppData;

    if ((0 == pstFrame->ucFrameLen) || (usAppDataLen < sizeof(MRS_CSG_MR_TASK_UNICAST_STRU) + pstFrame->ucFrameLen))
    {
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_001, HI_DIAG_MT("[CSG TASK] invalid param."));
        *pucDenyCode = MRS_CSG_DENY_ERROR_LENGTH;
        return HI_ERR_BAD_DATA;
    }

    if (!pFrameData->ucAddrFlag)
    {
        *pucDenyCode = MSG_CSG_DENY_ERROR_FORMAT;
        return HI_ERR_BAD_DATA;
    }

    if (mrsCcoMrTaskIsFull())
    {
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_002, HI_DIAG_MT("[CSG TASK] queue is full."));
        *pucDenyCode = MSG_CSG_DENY_TASK_NOT_ENOUGH;
        return HI_ERR_FULL;
    }

    usId = MRS_COMBIN16(pstFrame->ucIdLo, pstFrame->ucIdHi);

    if (usId > MRS_CCO_MR_TASK_ID_MAX)
    {
        *pucDenyCode = MSG_CSG_DENY_TASK_ID_NOT_EXIST;
        return HI_ERR_FAILURE;
    }

    if (mrsCcoMrTaskIsInList(usId))
    {
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_003, HI_DIAG_MT("[CSG TASK] usId is in queue."));
        *pucDenyCode = MSG_CSG_DENY_TASK_REPEAT;
        return HI_ERR_EXIST;
    }

    do
    {
        pItem = (MRS_CCO_MR_TASK_ITEM_STRU *)mrsToolsMalloc(sizeof(MRS_CCO_MR_TASK_ITEM_STRU));
        if (!pItem)
        {
            HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_004, HI_DIAG_MT("[CSG TASK] malloc failed."));
            ulRet = HI_ERR_NOT_ENOUGH_MEMORY;
            *pucDenyCode = MSG_CSG_DENY_OTHER;
            break;
        }

        (hi_void)memset_s(pItem, sizeof(MRS_CCO_MR_TASK_ITEM_STRU), 0, sizeof(MRS_CCO_MR_TASK_ITEM_STRU));
        pItem->pFrame = (HI_U8 *)mrsToolsMalloc(pstFrame->ucFrameLen);
        if (!pItem->pFrame)
        {
            HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_005, HI_DIAG_MT("[CSG TASK] malloc failed."));
            ulRet = HI_ERR_NOT_ENOUGH_MEMORY;
            *pucDenyCode = MSG_CSG_DENY_OTHER;
            break;
        }

        (hi_void)memcpy_s(pItem->pFrame, pstFrame->ucFrameLen, pstFrame->aucFrame, pstFrame->ucFrameLen);
        pItem->ucFrameLen = pstFrame->ucFrameLen;

        pItem->usId = usId;
        pItem->unParam.stUnicast.usTimeout = MRS_COMBIN16(pstFrame->ucTimeoutLo, pstFrame->ucTimeoutHi);
        pItem->ulTime = HI_MDM_GetMilliSeconds();
        pItem->ucType = MRS_CCO_MR_TASK_TYPE_UNICAST;
        pItem->ucRespFlag = pstFrame->ucRespFlag;
        pItem->ucPriority = pstFrame->ucPriority;

        (hi_void)memcpy_s(pItem->unParam.stUnicast.aucSrcAddr, HI_METER_ADDR_LEN, pFrameData->aucSrcAddr, HI_METER_ADDR_LEN);
        (hi_void)memcpy_s(pItem->unParam.stUnicast.aucDstAddr, HI_METER_ADDR_LEN, pFrameData->aucDstAddr, HI_METER_ADDR_LEN);

        if (mrsToolsBroadCastAddr(pItem->unParam.stUnicast.aucDstAddr))
        {
            if (pItem->ucRespFlag == PROTOCSG_CTRL_RESP_FLG_Y)
            {
                ulRet = HI_ERR_BAD_DATA;
                *pucDenyCode = MRS_CSG_DENY_INVALID_DATA;
                break;
            }

            pItem->unParam.stUnicast.ucType = MRS_CCO_MR_TASK_UNI_BROADCAST;
        }
        else
        {
            if (pItem->ucRespFlag == PROTOCSG_CTRL_RESP_FLG_N)
            {
                ulRet = HI_ERR_BAD_DATA;
                *pucDenyCode = MRS_CSG_DENY_INVALID_DATA;
                break;
            }

            pItem->unParam.stUnicast.ucType = MRS_CCO_MR_TASK_UNI_NORMAL;
        }

        ulRet = mrsCcoMrTaskInsert(pItem);
        if (HI_ERR_SUCCESS != ulRet)
        {
            *pucDenyCode = MRS_CSG_DENY_INVALID_DATA;
            break;
        }
    } while (0);

    if (HI_ERR_SUCCESS != ulRet)
    {
        mrsCcoMrTaskItemFree((HI_VOID *)pItem, HI_NULL);
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_007, HI_DIAG_MT("[CSG TASK] insert failed."));
    }

    return ulRet;
}


HI_U32 mrsCcoMrTaskInsertMulticast(MRS_CSG_FRAME_STRU *pFrameData, HI_U8 *pucDenyCode)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    MRS_CCO_MR_TASK_ITEM_STRU *pItem = HI_NULL;
    HI_U8 *pAppData = pFrameData->aucAppData;
    HI_U16 usAppDataLen = pFrameData->usAppDataLen;
    MRS_CSG_MR_TASK_MULTICAST_STRU *pstFrame = (MRS_CSG_MR_TASK_MULTICAST_STRU *)pAppData;
    HI_U16 usExpectLen = 0;
    HI_U16 usAddrNum = 0;
    HI_U16 usFrameLenOffset = 0;
    HI_U16 usTimeoutOffset = 0;
    HI_U16 usId = 0;
    HI_U8 ucFrameLen = 0;
    HI_U8 *pFrame = HI_NULL;

    usAddrNum = MRS_COMBIN16(pstFrame->ucAddrNumLo, pstFrame->ucAddrNumHi);
    if (MRS_CCO_MR_TASK_NUM_MAX  == usAddrNum)
    {
        usTimeoutOffset = (HI_U16)(sizeof(MRS_CSG_MR_TASK_MULTICAST_STRU));
    }
    else
    {
        usTimeoutOffset = (HI_U16)(sizeof(MRS_CSG_MR_TASK_MULTICAST_STRU) + usAddrNum * HI_METER_ADDR_LEN);
    }

    usFrameLenOffset = usTimeoutOffset + sizeof(HI_U16);
    ucFrameLen = pAppData[usFrameLenOffset];
    usExpectLen = usFrameLenOffset + sizeof(ucFrameLen) + ucFrameLen;
    if ((0 == ucFrameLen) || (usAppDataLen < usExpectLen))
    {
        *pucDenyCode = MRS_CSG_DENY_ERROR_LENGTH;
        return HI_ERR_BAD_DATA;
    }

    if (pstFrame->ucRespFlag == PROTOCSG_CTRL_RESP_FLG_N)
    {
        *pucDenyCode = MRS_CSG_DENY_INVALID_DATA;
        return HI_ERR_BAD_DATA;
    }

    if (mrsCcoMrTaskIsFull())
    {
        *pucDenyCode = MSG_CSG_DENY_TASK_NOT_ENOUGH;
        return HI_ERR_FULL;
    }

    usId = MRS_COMBIN16(pstFrame->ucIdLo, pstFrame->ucIdHi);
    if (mrsCcoMrTaskIsInList(usId))
    {
        *pucDenyCode = MSG_CSG_DENY_TASK_REPEAT;
        return HI_ERR_EXIST;
    }

    do
    {
        HI_U8 ucType = MRS_CCO_MR_TASK_MULTI_NORMAL;
        pFrame = pAppData + usFrameLenOffset + sizeof(ucFrameLen);

        if (0 == usAddrNum)
        {
            *pucDenyCode = MSG_CSG_DENY_METER_NOT_EXIST;
            return HI_ERR_BAD_DATA;
        }

        if (MRS_CCO_MR_TASK_NUM_MAX == usAddrNum)
        {
            ucType = MRS_CCO_MR_TASK_MULTI_ALLMETER;
            usAddrNum = mrsSrvArchivesMeterNum();
        }
        else if (usAddrNum >= PRODUCT_CFG_MRS_MAX_NODE_NUM)
        {
            *pucDenyCode = MSG_CSG_DENY_OTHER;
            return HI_ERR_BAD_DATA;
        }

        pItem = (MRS_CCO_MR_TASK_ITEM_STRU *)mrsToolsMalloc(sizeof(MRS_CCO_MR_TASK_ITEM_STRU));
        if (!pItem)
        {
            ulRet = HI_ERR_NOT_ENOUGH_MEMORY;
            break;
        }

        (hi_void)memset_s(pItem, sizeof(MRS_CCO_MR_TASK_ITEM_STRU), 0, sizeof(MRS_CCO_MR_TASK_ITEM_STRU));
        pItem->pFrame = (HI_U8 *)mrsToolsMalloc(ucFrameLen);
        if (!pItem->pFrame)
        {
            ulRet = HI_ERR_NOT_ENOUGH_MEMORY;
            break;
        }

        if (MRS_CCO_MR_TASK_MULTI_ALLMETER != ucType)
        {
/* BEGIN: PN: DTS2016081300132 MODIFY\ADD\DEL by cuiate/00233580 at 2016/8/13 */
            if (!mrsCcoMrTaskIsMultiListValid(pstFrame->aucAddrInf, usAddrNum))
            {
                *pucDenyCode = MRS_CSG_DENY_INVALID_DATA;
                mrsCcoMrTaskItemFree((HI_VOID *)pItem, HI_NULL);
                return HI_ERR_BAD_DATA;
            }
/* END:   PN: DTS2016081300132 MODIFY\ADD\DEL by cuiate/00233580 at 2016/8/13 */

            pItem->unParam.stMulticast.pSubNode = (HI_U8 *)mrsToolsMalloc(HI_METER_ADDR_LEN * usAddrNum);
            if (!pItem->unParam.stMulticast.pSubNode)
            {
                ulRet = HI_ERR_NOT_ENOUGH_MEMORY;
                break;
            }

            (hi_void)memcpy_s(pItem->unParam.stMulticast.pSubNode, HI_METER_ADDR_LEN * usAddrNum, pstFrame->aucAddrInf, HI_METER_ADDR_LEN * usAddrNum);
        }

        pItem->unParam.stMulticast.usSubNodeNum = usAddrNum;
        pItem->unParam.stMulticast.ucType = ucType;

        (hi_void)memcpy_s(&pItem->unParam.stMulticast.usTimeout, sizeof(HI_U16), pAppData + usTimeoutOffset, sizeof(HI_U16));
        (hi_void)memcpy_s(pItem->pFrame, ucFrameLen, pFrame, ucFrameLen);
        pItem->ucFrameLen = ucFrameLen;

        pItem->usId = usId;
        pItem->ucType = MRS_CCO_MR_TASK_TYPE_MULTICAST;
        pItem->ucRespFlag = pstFrame->ucRespFlag;
        pItem->ucPriority = pstFrame->ucPriority;
        pItem->ulTime = HI_MDM_GetMilliSeconds();

        ulRet = mrsCcoMrTaskInsert(pItem);
    } while (0);

    if (HI_ERR_SUCCESS != ulRet)
    {
        *pucDenyCode = MSG_CSG_DENY_OTHER;
        mrsCcoMrTaskItemFree((HI_VOID *)pItem, HI_NULL);
    }

    return ulRet;
}


HI_U32 mrsCcoMrTaskInsert(MRS_CCO_MR_TASK_ITEM_STRU *pstNode)
{
    MRS_CCO_MR_TASK_CONTEXT *pstCtx = mrsCcoGetMrTaskCtx();
    MRS_SRV_LINK *pLink = HI_NULL;
    MRS_SRV_LINK *pNewNode = (MRS_SRV_LINK *)(&pstNode->stLink);

    if (pstNode->ucPriority >= MRS_CCO_MR_TASK_PRIORITY_MAX)
    {
        HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_008, HI_DIAG_MT("[CSG TASK] Priority error."), pstNode->ucPriority);
        return HI_ERR_INVALID_PARAMETER;
    }

    for (pLink = pstCtx->stList.next; pLink != &pstCtx->stList; pLink = pLink->next)
    {
        MRS_CCO_MR_TASK_ITEM_STRU *pItem = (MRS_CCO_MR_TASK_ITEM_STRU *)pLink;

        // 按优先级插入：0最高，3最低
        if (pItem->ucPriority > pstNode->ucPriority)
        {
            break;
        }
    }

    pNewNode->next = pLink;
    pNewNode->prev = pLink->prev;

    pNewNode->next->prev = pNewNode;
    pNewNode->prev->next = pNewNode;

    pstCtx->usTaskNum++;

    if ((1 == pstCtx->usTaskNum) || (pstCtx->stList.next == pNewNode))
    {
        pstCtx->pstProcItem = HI_NULL;
        mrsCcoMrTaskProc();
    }

    return HI_ERR_SUCCESS;
}


HI_BOOL mrsCcoMrTaskTraverseCallback(HI_VOID * pstNode, HI_VOID * pParam)
{
    MRS_CCO_MR_TASK_ITEM_STRU *pstItem = (MRS_CCO_MR_TASK_ITEM_STRU *)pstNode;
    MRS_CCO_MR_TASK_TRAVERSE_PARAM *pstExtra = (MRS_CCO_MR_TASK_TRAVERSE_PARAM *)pParam;

    if (pstItem->usId == pstExtra->usId)
    {
        pstExtra->pstItem = pstItem;
        return HI_TRUE;
    }

    return HI_FALSE;
}


MRS_CCO_MR_TASK_ITEM_STRU *mrsCcoMrTaskGetItem(HI_U16 usId)
{
    MRS_CCO_MR_TASK_CONTEXT *pstCtx = mrsCcoGetMrTaskCtx();
    MRS_CCO_MR_TASK_TRAVERSE_PARAM stParam;

    (hi_void)memset_s(&stParam, sizeof(stParam), 0, sizeof(stParam));
    stParam.usId = usId;

    (HI_VOID)mrsSrvTraverseList(&pstCtx->stList, mrsCcoMrTaskTraverseCallback, &stParam);
    return stParam.pstItem;
}


HI_U32 mrsCcoMrTaskDelete(HI_U16 usId)
{
    MRS_CCO_MR_TASK_CONTEXT *pstCtx = mrsCcoGetMrTaskCtx();
    MRS_CCO_MR_TASK_ITEM_STRU *pstItem = HI_NULL;
    MRS_SRV_LINK *pstLink = HI_NULL;

    pstItem = mrsCcoMrTaskGetItem(usId);
    if (HI_NULL == pstItem)
    {
        return HI_ERR_NOT_FOUND;
    }

    pstLink = &pstItem->stLink;
    pstLink->next->prev = pstLink->prev;
    pstLink->prev->next = pstLink->next;

    if (pstItem == pstCtx->pstProcItem)
    {
        mrsBroadcastRegFinishNotify(HI_NULL, HI_NULL);
        mrsAnyTimeReadMeterFinishProc();
        pstCtx->pstProcItem = HI_NULL;
        mrsCcoMrTaskNotify();
    }

    mrsRmRemoveItemFromPool((HI_VOID *)pstItem);

    mrsCcoMrTaskItemFree(pstItem, HI_NULL);
    pstItem = HI_NULL;
    pstCtx->usTaskNum--;

    return HI_ERR_SUCCESS;
}


HI_BOOL mrsCcoMrTaskIsReportItem(HI_VOID *pvItem)
{
    MRS_SRV_CCO_ITEM *pstNode = (MRS_SRV_CCO_ITEM *)pvItem;

    if (!pstNode)
    {
        return HI_FALSE;
    }

    return (HI_BOOL)((pstNode->ucAfn == MRS_CSG_AFN_REPORT_05H)
        && ((pstNode->usFn == (HI_U16)MRS_CSG_DI_REPORT_TASK_DATA) || (pstNode->usFn == (HI_U16)MRS_CSG_DI_REPORT_TASK_STATUS)));
}


HI_U32 mrsCcoMrTaskDeleteAll(HI_VOID)
{
    MRS_CCO_MR_TASK_CONTEXT *pstCtx = mrsCcoGetMrTaskCtx();

    mrsSrvListClear(&pstCtx->stList, mrsCcoMrTaskItemFree, HI_NULL);
    pstCtx->usTaskNum = 0;
    pstCtx->pstProcItem = HI_NULL;

    mrsBroadcastRegFinishNotify(HI_NULL, HI_NULL);
    mrsCcoMrTaskSetFlowCtrl(HI_FALSE);

    // 初始化抄表流程
    mrsAnyTimeReadMeterFinishProc();
    mrsRmModuleReset();

    mrsCcoQueueDeleteItemDefault(mrsCcoMrTaskIsReportItem);

    return HI_ERR_SUCCESS;
}


HI_U16 mrsCcoMrTaskMax(HI_VOID)
{
    MRS_CCO_MR_TASK_CONTEXT *pstCtx = mrsCcoGetMrTaskCtx();
    return pstCtx->usTaskMax;
}


HI_U16 mrsCcoMrTaskRemain(HI_VOID)
{
    MRS_CCO_MR_TASK_CONTEXT *pstCtx = mrsCcoGetMrTaskCtx();
    return pstCtx->usTaskMax - pstCtx->usTaskNum;
}


HI_U16 mrsCcoMrTaskNum(HI_VOID)
{
    MRS_CCO_MR_TASK_CONTEXT *pstCtx = mrsCcoGetMrTaskCtx();
    return pstCtx->usTaskNum;
}


HI_U32 mrsCcoMrTaskMsgProc(HI_SYS_QUEUE_MSG_S *pstMsg)
{
    switch (pstMsg->ulParam[0])
    {
    case MRS_CCO_MR_TASK_MSG_NOTIFY:
        mrsCcoMrTaskProc();
        break;

    default:
        break;
    }

    return HI_ERR_SUCCESS;
}


HI_U32 mrsCcoMrTaskActive(HI_VOID)
{
    // 正常轮转时就不启用该定时器了
    if (!mrsCcoMrTaskIsEnabled()
        || (!mrsCcoMrTaskIsChlAvailable() && (HI_ERR_SUCCESS != mrsCheckMeterNum())))
    {
        mrsCcoMrTaskNotify();
    }

    return HI_ERR_SUCCESS;
}


HI_U32 mrsCcoMrTaskNotify(HI_VOID)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;

    if (mrsCcoMrTaskNum() > 0)
    {
        if (mrsCcoQueueGetLength() < MRS_CCO_REPORTLIST_THRESHOLD)
        {
            ulRet = mrsCcoMrTaskMsgSend(MRS_CCO_MR_TASK_MSG_NOTIFY);
            mrsCcoMrTaskSetFlowCtrl(HI_FALSE);
        }
        else
        {
            mrsCcoMrTaskSetFlowCtrl(HI_TRUE);
        }
    }

    return ulRet;
}


HI_U32 mrsCcoMrTaskMsgSend(HI_U32 ulMsgId)
{
    HI_SYS_QUEUE_MSG_S stMsg;

    (hi_void)memset_s(&stMsg, sizeof(stMsg), 0, sizeof(stMsg));
    stMsg.ulMsgId = EN_MRS_FW_MSG_CCO_MR_TASK;
    stMsg.ulParam[0] = ulMsgId;

    return mrsSendMessage2Queue(&stMsg);
}


HI_U32 mrsCcoMrTaskReportNoResponse(HI_VOID)
{
    MRS_CCO_MR_TASK_CONTEXT *pstMrTaskCtx = mrsCcoGetMrTaskCtx();
    MRS_CCO_MR_TASK_ITEM_STRU *pstItem = pstMrTaskCtx->pstProcItem;
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U8 ucErrCode = MRS_CSG_TASK_STATUS_NO_RESPONSE;
    HI_U8 *pucAddr = HI_NULL;
    HI_BOOL bRemove = HI_TRUE;

    if (!pstItem)
    {
        mrsCcoMrTaskNotify();
        return HI_ERR_INVALID_PARAMETER;
    }

    do
    {
        if (MRS_CCO_MR_TASK_TYPE_UNICAST == pstItem->ucType)
        {
            pucAddr = pstItem->unParam.stUnicast.aucDstAddr;
            break;
        }
        else
        {
            MRS_CCO_MR_TASK_PARAM_MULTICAST *pstMulti = &pstItem->unParam.stMulticast;
            MRS_ARCHIVES_NODE_STRU *pstNode = HI_NULL;

            if (pstMulti->usIndex >= pstMulti->usSubNodeNum)
            {
                ucErrCode = MRS_CSG_TASK_STATUS_OTHER_ERR;
                break;
            }

            if (MRS_CCO_MR_TASK_MULTI_NORMAL == pstMulti->ucType)
            {
                pucAddr = pstMulti->pSubNode + HI_METER_ADDR_LEN * pstMulti->usIndex;
                pstMulti->usIndex++;
                bRemove = HI_FALSE;
                break;
            }

            pstNode = mrsSrvArchivesQuery(pstMulti->usIndex);
            if ((pstNode == HI_NULL) || !pstNode->bValid)
            {
                ucErrCode = MRS_CSG_TASK_STATUS_OTHER_ERR;
                break;
            }

            pucAddr = pstNode->aucNodeAddr;
            pstMulti->usIndex++;
            bRemove = HI_FALSE;
        }
    } while (0);

    ulRet = mrsCcoMrTaskReportItemNak(pstItem, pucAddr, ucErrCode);

    if (bRemove)
    {
        mrsCcoMrTaskRemoveNode(pstItem);
        pstMrTaskCtx->pstProcItem = HI_NULL;
    }

    mrsCcoMrTaskNotify();

    return ulRet;
}


HI_VOID mrsCcoMrTaskComRx(HI_VOID *pParam)
{
    MRS_NOT_USED(pParam);

    mrsCcoDfxRmUartRx(HI_NULL, 0, HI_FALSE);

    if (mrsCcoMrTaskIsFlowCtrl())
    {
        mrsCcoMrTaskNotify();
    }
}


HI_U32 mrsCcoMrTaskEveryTimeOut(HI_VOID *pParam)
{
    P_MRS_SRV_CCO_QUEUE pstCcoQueue = mrsGetCcoQueue();
    MRS_SRV_CCO_ITEM *pstCcoItem = HI_NULL;

    MRS_NOT_USED(pParam);

    pstCcoItem = mrsSrvQueueTop(&pstCcoQueue->stCcoQueue);
    if (pstCcoItem)
    {
        mrsDfxRmUartTx(pstCcoItem->pData, pstCcoItem->usDataLen, HI_FALSE);
    }

    return HI_ERR_CONTINUE;
}


HI_U32 mrsCcoMrTaskComMaxtryTimeout(HI_VOID *pParam)
{
    MRS_NOT_USED(pParam);

    if (mrsCcoMrTaskIsFlowCtrl())
    {
        mrsCcoMrTaskNotify();
    }

    return HI_ERR_SUCCESS;
}


HI_U32 mrsCcoMrTaskReportXrFrame(HI_U8 *pucFrame, HI_U16 usLength)
{
    MRS_CCO_MR_TASK_CONTEXT *pstMrTaskCtx = mrsCcoGetMrTaskCtx();
    MRS_CCO_MR_TASK_ITEM_STRU *pstItem = pstMrTaskCtx->pstProcItem;

    mrsCcoMrTaskReportFrameProc(pstItem, pucFrame, usLength);

    if (MRS_CCO_MR_TASK_TYPE_UNICAST == pstItem->ucType)
    {
        mrsCcoMrTaskRemoveNode(pstItem);
        pstMrTaskCtx->pstProcItem = HI_NULL;
    }
    else
    {
        MRS_CCO_MR_TASK_PARAM_MULTICAST *pstMulti = &pstItem->unParam.stMulticast;
        pstMulti->usIndex++;

        if (pstMulti->usIndex >= pstMulti->usSubNodeNum)
        {
/* BEGIN: PN: DTS2016081300132 MODIFY\ADD\DEL by cuiate/00233580 at 2016/8/13 */
            mrsCcoMrTaskReportItemNak(pstItem, HI_NULL, MRS_CSG_TASK_STATUS_SUCCESS);
/* END:   PN: DTS2016081300132 MODIFY\ADD\DEL by cuiate/00233580 at 2016/8/13 */
            mrsCcoMrTaskRemoveNode(pstItem);
            pstMrTaskCtx->pstProcItem = HI_NULL;
        }
    }

    mrsCcoMrTaskNotify();

    return HI_ERR_SUCCESS;
}


HI_U32 mrsCcoMrTaskReportFrameProc(MRS_CCO_MR_TASK_ITEM_STRU *pstItem, HI_U8 *pucFrame, HI_U16 usLength)
{
    MRS_CCO_SRV_CTX_STRU *pstCcoCtx = mrsCcoGetContext();
    MRS_CSG_IND_FRAME_STRU stIndFrame = {0};
    MRS_SRV_CCO_ITEM *pstCcoItem = HI_NULL;

    HI_U8 *pucAppData = HI_NULL;

    HI_U8 *pucPayload = HI_NULL;
    HI_U16 usPayloadLen = 0;

    HI_U32 ulRet = HI_ERR_SUCCESS;

    (hi_void)memset_s(&stIndFrame, sizeof(stIndFrame), 0, sizeof(stIndFrame));

    do
    {
        if (!pstItem)
        {
            HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_016, HI_DIAG_MT("pstMrTaskCtx->pstProcItem NULL"));
            ulRet = HI_ERR_FAILURE;
            break;
        }

        stIndFrame.ucAfn = MRS_CSG_AFN_REPORT_05H;
        stIndFrame.ulDI = MRS_CSG_DI_REPORT_TASK_DATA;
        stIndFrame.ucSeq = ++(pstCcoCtx->ucSeq);
        stIndFrame.ucPrm = 1;
        stIndFrame.ucAddrFlag = 0;

        if (MRS_CCO_MR_TASK_TYPE_UNICAST == pstItem->ucType)
        {
            stIndFrame.ucAddrFlag = 1;
            (hi_void)memcpy_s(stIndFrame.aucDstAddr, HI_METER_ADDR_LEN, pstItem->unParam.stUnicast.aucSrcAddr, HI_METER_ADDR_LEN);
            (hi_void)memcpy_s(stIndFrame.aucSrcAddr, HI_METER_ADDR_LEN, pstItem->unParam.stUnicast.aucDstAddr, HI_METER_ADDR_LEN);
        }

        stIndFrame.usAppDataLen = sizeof(pstItem->usId) + sizeof(HI_U8) + usLength;
        stIndFrame.pAppData = mrsToolsMalloc(stIndFrame.usAppDataLen);
        if (!stIndFrame.pAppData)
        {
            HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_022, HI_DIAG_MT("Malloc failed."));
            ulRet = HI_ERR_NOT_ENOUGH_MEMORY;
            break;
        }

        pucAppData = stIndFrame.pAppData;
        (hi_void)memcpy_s(pucAppData, sizeof(pstItem->usId), &pstItem->usId, sizeof(pstItem->usId));
        pucAppData += sizeof(pstItem->usId);
        *pucAppData++ = (HI_U8)usLength;
        (hi_void)memcpy_s(pucAppData, usLength, pucFrame, usLength);

        if (HI_ERR_SUCCESS != mrsCreateCsgFrame(&stIndFrame, &pucPayload, &usPayloadLen))
        {
            HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_018, HI_DIAG_MT("create CSG frame failed."));
            mrsToolsFree(stIndFrame.pAppData);
            ulRet = HI_ERR_FAILURE;
            break;
        }

        mrsToolsFree(stIndFrame.pAppData);

        pstCcoItem = (MRS_SRV_CCO_ITEM *)mrsToolsMalloc(sizeof(MRS_SRV_CCO_ITEM) + usPayloadLen);
        if (!pstCcoItem)
        {
            HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_019, HI_DIAG_MT("Malloc failed."));
            mrsDfxCcoQueueMallocFailCnt();
            ulRet = HI_ERR_NOT_ENOUGH_MEMORY;
            break;
        }

        (hi_void)memset_s(pstCcoItem, sizeof(MRS_SRV_CCO_ITEM) + usPayloadLen, 0, sizeof(MRS_SRV_CCO_ITEM) + usPayloadLen);
        pstCcoItem->bValid = HI_TRUE;
        pstCcoItem->ucAfn = MRS_CSG_AFN_REPORT_05H;
        pstCcoItem->usFn = (HI_U16)(MRS_CSG_DI_REPORT_TASK_DATA & 0xFFFF);
        pstCcoItem->usTimeOut = MRS_CCO_MR_TASK_MASTER_TIME;
        pstCcoItem->ucMaxTry = MRS_CCO_MR_TASK_MASTER_MAXTRY;
        pstCcoItem->usDataLen = usPayloadLen;
        (hi_void)memcpy_s(pstCcoItem->pData, usPayloadLen, pucPayload, usPayloadLen);
        pstCcoItem->pParam = HI_NULL;
        pstCcoItem->MrsCcoRxRespProc = mrsCcoMrTaskComRx;
        pstCcoItem->MrsCcoRxEveryTimeOutProc = mrsCcoMrTaskEveryTimeOut;
        pstCcoItem->MrsCcoRxTotalTimeOutProc = mrsCcoMrTaskComMaxtryTimeout;

        mrsCcoJoinQueue(pstCcoItem);

        mrsDfxRmUartTx(pucPayload, usPayloadLen, HI_FALSE);
        mrsActiveCcoQueue();
    } while (0);

    mrsToolsFree(pucPayload);

    return ulRet;
}



HI_U32 mrsCcoMrTaskGetIdBuffer(HI_U16 usStart, HI_U8 ucTaskNum, HI_U8 **ppBuffer, HI_U16 *pusLength)
{
    MRS_CCO_MR_TASK_CONTEXT *pstMrTaskCtx = mrsCcoGetMrTaskCtx();
    MRS_SRV_LINK *pLink = HI_NULL;
    HI_U16 usNum = ucTaskNum;
    HI_U8 *pData = HI_NULL;
    HI_U16 usDataLen = 0;
    HI_U16 usOffset = 0;
    HI_U16 i = 0;

    if (usStart >= pstMrTaskCtx->usTaskNum)
    {
        usNum = 0;
    }
    else if (usStart + ucTaskNum > pstMrTaskCtx->usTaskNum)
    {
        usNum = pstMrTaskCtx->usTaskNum - usStart;
    }

    usDataLen = (HI_U16)(sizeof(HI_U16) * (usNum + 1));
    pData = mrsToolsMalloc(usDataLen);
    if (!pData)
    {
        return HI_ERR_NOT_ENOUGH_MEMORY;
    }

    (hi_void)memset_s(pData, usDataLen, 0, usDataLen);
    *ppBuffer = pData;
    *pusLength = usDataLen;

    (hi_void)memcpy_s(pData, sizeof(HI_U16), &usNum, sizeof(HI_U16));
    pData += sizeof(HI_U16);

    for (pLink = pstMrTaskCtx->stList.next; pLink != &pstMrTaskCtx->stList; pLink = pLink->next)
    {
        if (usOffset >= usStart)
        {
            MRS_CCO_MR_TASK_ITEM_STRU *pItem = (MRS_CCO_MR_TASK_ITEM_STRU *)pLink;

            if (i >= usNum)
            {
                break;
            }

            (hi_void)memcpy_s(pData, sizeof(HI_U16), &pItem->usId, sizeof(HI_U16));
            pData += sizeof(HI_U16);
            i++;
        }

        usOffset++;
    }

    return HI_ERR_SUCCESS;
}


HI_U32 mrsCcoMrTaskGetTaskInfo(HI_U16 usId, HI_U8 **ppBuffer, HI_U16 *pusLength)
{
    MRS_CCO_MR_TASK_ITEM_STRU *pResult = HI_NULL;
    MRS_CCO_TASK_DETAIL_HEAD_S *pTaskHead = HI_NULL;
    HI_U16 usDataLen = 0;
    HI_U8 *pData = HI_NULL;

    pResult = mrsCcoMrTaskGetItem(usId);
    if (!pResult)
    {
        return HI_ERR_NOT_FOUND;
    }

    usDataLen = sizeof(MRS_CCO_TASK_DETAIL_HEAD_S) + sizeof(pResult->ucFrameLen) + pResult->ucFrameLen;
    if (MRS_CCO_MR_TASK_TYPE_UNICAST == pResult->ucType)
    {
        usDataLen += HI_METER_ADDR_LEN;
    }
    else
    {
        if (pResult->unParam.stMulticast.usSubNodeNum != MRS_CCO_MR_TASK_NUM_MAX)
        {
            usDataLen += (HI_U16)(HI_METER_ADDR_LEN * pResult->unParam.stMulticast.usSubNodeNum);
        }
    }

    pData = mrsToolsMalloc(usDataLen);
    if (!pData)
    {
        return HI_ERR_NOT_ENOUGH_MEMORY;
    }

    (hi_void)memset_s(pData, usDataLen, 0, usDataLen);

    *ppBuffer = pData;
    *pusLength = usDataLen;

    pTaskHead = (MRS_CCO_TASK_DETAIL_HEAD_S *)pData;
    pTaskHead->usId = pResult->usId;
    pTaskHead->ucPrio = pResult->ucPriority;
    pTaskHead->ucResp = pResult->ucRespFlag;

    if (MRS_CCO_MR_TASK_TYPE_UNICAST == pResult->ucType)
    {
        pTaskHead->usDestAddrNum = 1;
        (hi_void)memcpy_s(pTaskHead->aucDestAddrList, HI_METER_ADDR_LEN, pResult->unParam.stUnicast.aucDstAddr, HI_METER_ADDR_LEN);
    }
    else
    {
        if (pResult->unParam.stMulticast.usSubNodeNum != MRS_CCO_MR_TASK_NUM_MAX)
        {
            pTaskHead->usDestAddrNum = pResult->unParam.stMulticast.usSubNodeNum;
            (hi_void)memcpy_s(pTaskHead->aucDestAddrList, HI_METER_ADDR_LEN * pTaskHead->usDestAddrNum, pResult->unParam.stMulticast.pSubNode, HI_METER_ADDR_LEN * pResult->unParam.stMulticast.usSubNodeNum);
        }
    }

    pData += usDataLen - sizeof(pResult->ucFrameLen) - pResult->ucFrameLen;
    *pData++ = pResult->ucFrameLen;
    (hi_void)memcpy_s(pData, pResult->ucFrameLen, pResult->pFrame, pResult->ucFrameLen);

    return HI_ERR_SUCCESS;
}


HI_VOID mrsCcoMrTaskBcNotify(HI_VOID *pParam)
{
    MRS_CCO_MR_TASK_CONTEXT *pstMrTaskCtx = mrsCcoGetMrTaskCtx();
    MRS_CCO_MR_TASK_ITEM_STRU *pstItem = pstMrTaskCtx->pstProcItem;
    MRS_CCO_MR_TASK_ITEM_STRU *pstTemp = (MRS_CCO_MR_TASK_ITEM_STRU *)pParam;

    if (!pstItem || (pstItem != pstTemp))
    {
        return;
    }

    mrsCcoMrTaskReportItemNak(pstItem, HI_NULL, MRS_CSG_TASK_STATUS_SUCCESS);
    mrsCcoMrTaskRemoveNode(pstItem);
    mrsCcoMrTaskNotify();
}


HI_VOID mrsCcoMrTaskSetStatus(HI_U8 ucStatus)
{
    MRS_CCO_MR_TASK_CONTEXT *pstMrTaskCtx = mrsCcoGetMrTaskCtx();

    if ((MRS_CCO_MR_TASK_PAUSE == pstMrTaskCtx->ucStatus) && (MRS_CCO_MR_TASK_ENABLE == ucStatus))
    {
        mrsCcoMrTaskNotify();
    }
    else if ((MRS_CCO_MR_TASK_ENABLE == pstMrTaskCtx->ucStatus) && (MRS_CCO_MR_TASK_PAUSE == ucStatus))
    {
        mrsBroadcastRegFinishNotify(HI_NULL, HI_NULL);
        mrsCcoMrTaskSetFlowCtrl(HI_FALSE);

        mrsAnyTimeReadMeterFinishProc();
        pstMrTaskCtx->pstProcItem = HI_NULL;

        mrsRmModuleReset();

        MRS_StartTimer(MRS_CCO_TIMER_MR_TASK_ACTIVE, MRS_CCO_TIME_MR_TASK_ACTIVE, HI_SYS_TIMER_ONESHOT);
    }

    pstMrTaskCtx->ucStatus = ucStatus;
}


HI_BOOL mrsCcoMrTaskIsEnabled(HI_VOID)
{
    MRS_CCO_MR_TASK_CONTEXT *pstMrTaskCtx = mrsCcoGetMrTaskCtx();
    return (MRS_CCO_MR_TASK_ENABLE == pstMrTaskCtx->ucStatus);
}


HI_VOID mrsCcoMrTaskSetFlowCtrl(HI_BOOL bEnable)
{
    MRS_CCO_MR_TASK_CONTEXT *pstMrTaskCtx = mrsCcoGetMrTaskCtx();
    pstMrTaskCtx->bFlowCtrl = bEnable;
}


HI_BOOL mrsCcoMrTaskIsFlowCtrl(HI_VOID)
{
    MRS_CCO_MR_TASK_CONTEXT *pstMrTaskCtx = mrsCcoGetMrTaskCtx();
    return pstMrTaskCtx->bFlowCtrl;
}


HI_BOOL mrsCcoMrTaskIsChlAvailable(HI_VOID)
{
    return (HI_BOOL)(mrsGetCcoPlcChlStatus() || mrsCcoIsTestMode());
}


HI_U32 mrsCcoMrTaskReportItemNak(MRS_CCO_MR_TASK_ITEM_STRU *pstItem, HI_U8 *pucAddr, HI_U8 ucErrCode)
{
    MRS_CCO_SRV_CTX_STRU *pstCcoCtx = mrsCcoGetContext();
    MRS_CCO_MR_TASK_REPORT_STATUS_S stStatus;
    MRS_CSG_IND_FRAME_STRU stIndFrame;

    MRS_SRV_CCO_ITEM *pstCcoItem = HI_NULL;

    HI_U8 *pucPayload = HI_NULL;
    HI_U16 usPayloadLen = 0;

    (hi_void)memset_s(&stStatus, sizeof(stStatus), 0, sizeof(stStatus));
    stStatus.usId = pstItem->usId;
    stStatus.ucStatus = ucErrCode;

    if (pucAddr)
    {
        (hi_void)memcpy_s(stStatus.aucNodeAddr, HI_METER_ADDR_LEN, pucAddr, HI_METER_ADDR_LEN);
    }

    (hi_void)memset_s(&stIndFrame, sizeof(stIndFrame), 0, sizeof(stIndFrame));
    stIndFrame.ucAfn = MRS_CSG_AFN_REPORT_05H;
    stIndFrame.ulDI = MRS_CSG_DI_REPORT_TASK_STATUS;
    stIndFrame.ucSeq = ++(pstCcoCtx->ucSeq);
    stIndFrame.ucPrm = PROTOCSG_CTRL_PRM_BIT_REQ;
    stIndFrame.ucAddrFlag = PROTOCSG_CTRL_ADD_BIT_N;
    stIndFrame.pAppData = (HI_U8 *)&stStatus;
    stIndFrame.usAppDataLen = (HI_U16)sizeof(stStatus);

    if (HI_ERR_SUCCESS != mrsCreateCsgFrame(&stIndFrame, &pucPayload, &usPayloadLen))
    {
        return HI_ERR_FAILURE;
    }

    pstCcoItem = (MRS_SRV_CCO_ITEM *)mrsToolsMalloc(sizeof(MRS_SRV_CCO_ITEM) + usPayloadLen);
    if (!pstCcoItem)
    {
        mrsToolsFree(pucPayload);
        mrsDfxCcoQueueMallocFailCnt();
        return HI_ERR_NOT_ENOUGH_MEMORY;
    }

    (hi_void)memset_s(pstCcoItem, sizeof(MRS_SRV_CCO_ITEM) + usPayloadLen, 0, sizeof(MRS_SRV_CCO_ITEM) + usPayloadLen);

    pstCcoItem->bValid = HI_TRUE;
    pstCcoItem->ucAfn = MRS_CSG_AFN_REPORT_05H;
    pstCcoItem->usFn = (HI_U16)(MRS_CSG_DI_REPORT_TASK_STATUS & 0xFFFF);
    pstCcoItem->usTimeOut = MRS_CCO_MR_TASK_MASTER_TIME;
    pstCcoItem->ucMaxTry = MRS_CCO_MR_TASK_MASTER_MAXTRY;
    pstCcoItem->usDataLen = usPayloadLen;
    (hi_void)memcpy_s(pstCcoItem->pData, usPayloadLen, pucPayload, usPayloadLen);

    pstCcoItem->pParam = HI_NULL;
    pstCcoItem->MrsCcoRxRespProc = mrsCcoMrTaskComRx;
    pstCcoItem->MrsCcoRxEveryTimeOutProc = mrsCcoMrTaskEveryTimeOut;
    pstCcoItem->MrsCcoRxTotalTimeOutProc = mrsCcoMrTaskComMaxtryTimeout;

    mrsCcoJoinQueue(pstCcoItem);

    mrsDfxRmUartTx(pucPayload, usPayloadLen, HI_FALSE);
    mrsActiveCcoQueue();

    mrsToolsFree(pucPayload);
    return HI_ERR_SUCCESS;
}


HI_VOID mrsCcoMrTaskRemoveNode(MRS_CCO_MR_TASK_ITEM_STRU *pstItem)
{
    MRS_CCO_MR_TASK_CONTEXT *pstMrTaskCtx = mrsCcoGetMrTaskCtx();

    (HI_VOID)mrsSrvListRemove(&pstMrTaskCtx->stList, &pstItem->stLink);
    mrsCcoMrTaskItemFree(pstItem, HI_NULL);
    pstMrTaskCtx->usTaskNum--;
}


HI_BOOL mrsCcoMrTaskIsItemTimeout(MRS_CCO_MR_TASK_ITEM_STRU *pstItem)
{
    HI_U32 ulTimeStamp = HI_MDM_GetMilliSeconds();
    HI_U32 ulTimeout = 0;

    if (!pstItem)
    {
        return HI_TRUE;
    }

    if (MRS_CCO_MR_TASK_TYPE_UNICAST == pstItem->ucType)
    {
        ulTimeout = MRS_SEC_TO_MS((HI_U32)pstItem->unParam.stUnicast.usTimeout);
    }
    else
    {
        ulTimeout = MRS_SEC_TO_MS((HI_U32)pstItem->unParam.stMulticast.usTimeout);
    }

    // 此处比较时考虑到反转，用当前时间减去接收到报文时间，算出来的时段再跟超时时间比较
    return (HI_BOOL)((ulTimeStamp - pstItem->ulTime) >= ulTimeout);
}


HI_U32 mrsCcoMrTaskCheckTimeout(MRS_CCO_MR_TASK_CONTEXT *pstCtx)
{
    MRS_SRV_LINK *pstLink = HI_NULL;

    pstLink = pstCtx->stList.next;
    while (pstLink != &pstCtx->stList)
    {
        MRS_CCO_MR_TASK_ITEM_STRU *pstItem = (MRS_CCO_MR_TASK_ITEM_STRU *)pstLink;

        if (mrsCcoQueueGetLength() >= MRS_CCO_REPORTLIST_THRESHOLD)
        {
            mrsCcoMrTaskSetFlowCtrl(HI_TRUE);
            return HI_ERR_FULL;
        }

        pstLink = pstLink->next;

        if (mrsCcoMrTaskIsItemTimeout(pstItem))
        {
            // 上报超时，进入上报流程
            if (MRS_CCO_MR_TASK_TYPE_UNICAST == pstItem->ucType)
            {
                mrsCcoMrTaskReportItemNak(pstItem, pstItem->unParam.stUnicast.aucDstAddr, MRS_CSG_TASK_STATUS_OTHER_ERR);
            }
            else
            {
                mrsCcoMrTaskReportItemNak(pstItem, HI_NULL, MRS_CSG_TASK_STATUS_OTHER_ERR);
            }

            mrsRmRemoveItemFromPool((HI_VOID *)pstItem);
            mrsCcoMrTaskRemoveNode(pstItem);
            continue;
        }
    }

    return HI_ERR_SUCCESS;
}


HI_BOOL mrsCcoMrTaskIsBcItem(MRS_CCO_MR_TASK_ITEM_STRU *pstItem)
{
    return (HI_BOOL)((MRS_CCO_MR_TASK_TYPE_UNICAST == pstItem->ucType)
        && (MRS_CCO_MR_TASK_UNI_BROADCAST == pstItem->unParam.stUnicast.ucType));
}


HI_BOOL mrsCcoMrTaskIsXrItem(MRS_CCO_MR_TASK_ITEM_STRU *pstItem)
{
    MRS_CCO_SRV_CTX_STRU *pstCtx = mrsCcoGetContext();

    if (mrsCcoMrTaskIsBcItem(pstItem))
    {
        return HI_FALSE;
    }

    return (HI_BOOL)(mrsCcoIsTestMode()
        || (!pstCtx->bCsgParallel)
        || (pstItem->ucPriority <= MRS_CCO_MR_TASK_PRIORITY_2));
}


MRS_CCO_MR_TASK_PROC_MODE_E mrsCcoMrTaskGetMode(HI_VOID)
{
    MRS_CCO_MR_TASK_CONTEXT *pstCtx = mrsCcoGetMrTaskCtx();
    MRS_CCO_MR_TASK_ITEM_STRU *pstItem = (MRS_CCO_MR_TASK_ITEM_STRU *)pstCtx->stList.next;

    if (&pstItem->stLink == &pstCtx->stList)
    {
        return MRS_CCO_MR_TASK_MODE_NA;
    }

    if (mrsCcoMrTaskIsBcItem(pstItem))
    {
        return MRS_CCO_MR_TASK_MODE_BC;
    }

    if (mrsCcoMrTaskIsXrItem(pstItem))
    {
        return MRS_CCO_MR_TASK_MODE_XR;
    }

    return MRS_CCO_MR_TASK_MODE_PR;
}


// 处理抄表列表: 透抄方式
HI_U32 mrsCcoMrTaskProcListXr(MRS_CCO_MR_TASK_CONTEXT *pstCtx)
{
    MRS_CCO_SRV_CTX_STRU *pstCcoCtx = (MRS_CCO_SRV_CTX_STRU *)mrsCcoGetContext();
    MRS_CCO_MR_TASK_ITEM_STRU *pstItem = (MRS_CCO_MR_TASK_ITEM_STRU *)pstCtx->stList.next;
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U8 *pucSrcAddr = HI_NULL;
    HI_U8 *pucDstAddr = HI_NULL;
    HI_U16 usPos = 0;
    HI_U16 usLength = 0;
    HI_U32 ulTimeStamp = HI_MDM_GetMilliSeconds();
    HI_U32 ulTimeout = 0;
    HI_U32 ucState = mrsCcoGetState();
    HI_BOOL bReplace = HI_FALSE;
    HI_U8 aucMeterAddr[HI_METER_ADDR_LEN] = {0};

    // 停止并发
    mrsRmModuleReset();

    HI_MDM_SetTfNtbSendProcessCtrl(HI_MDM_NTB_SEND_FREQUENCY_REDUCE, HI_MDM_NTB_SEND_CTRL_REASON_READ_METER);

    if (MRS_CCO_MR_TASK_TYPE_MULTICAST == pstItem->ucType)
    {
        MRS_CCO_MR_TASK_PARAM_MULTICAST *pstMulti = &pstItem->unParam.stMulticast;

        do
        {
            if (MRS_CCO_MR_TASK_MULTI_NORMAL == pstMulti->ucType)
            {
                if (pstMulti->usIndex >= pstMulti->usSubNodeNum)
                {
                    ulRet = HI_ERR_NOT_FOUND;
                    break;
                }

                pucDstAddr = pstMulti->pSubNode + HI_METER_ADDR_LEN * pstMulti->usIndex;
            }
            else
            {
                MRS_ARCHIVES_NODE_STRU *pstNode = HI_NULL;

                if (pstMulti->usIndex >= mrsSrvArchivesMeterNum())
                {
                    ulRet = HI_ERR_NOT_FOUND;
                    break;
                }

                pstNode = mrsSrvArchivesQuery(pstMulti->usIndex);
                if ((pstNode == HI_NULL) || !pstNode->bValid)
                {
                    ulRet = HI_ERR_NOT_FOUND;
                    break;
                }

                pucDstAddr = pstNode->aucNodeAddr;
            }
        } while (0);

        if (HI_ERR_SUCCESS != ulRet)
        {
            mrsCcoMrTaskReportItemNak(pstItem, HI_NULL, MRS_CSG_TASK_STATUS_OTHER_ERR);
            mrsRmRemoveItemFromPool((HI_VOID *)pstItem);
            mrsCcoMrTaskRemoveNode(pstItem);
            mrsCcoMrTaskNotify();
            HI_MDM_SetTfNtbSendProcessCtrl(HI_MDM_NTB_SEND_PROCESS_RESUME, HI_MDM_NTB_SEND_CTRL_REASON_READ_METER);
            return HI_ERR_SUCCESS;
        }

        if (HI_ERR_SUCCESS == mrsFind645Frame(pstItem->pFrame, pstItem->ucFrameLen, &usPos, &usLength))
        {
            bReplace = HI_TRUE;
            (hi_void)memcpy_s(aucMeterAddr, HI_METER_ADDR_LEN, pstItem->pFrame + usPos + MRS_645_FRAME_METERADD_OFFSET, HI_METER_ADDR_LEN);
            (hi_void)memcpy_s(pstItem->pFrame + usPos + MRS_645_FRAME_METERADD_OFFSET, HI_METER_ADDR_LEN, pucDstAddr, HI_METER_ADDR_LEN);
            pstItem->pFrame[usPos + usLength - 2] = mrsToolsCalcCheckSum(pstItem->pFrame + usPos, usLength - 2);
        }

        pucSrcAddr = pstCcoCtx->ucMainNodeAddr;

        ulTimeout = pstItem->ulTime + MRS_SEC_TO_MS((HI_U32)pstMulti->usTimeout) - ulTimeStamp;
    }
    else
    {
        pucSrcAddr = pstItem->unParam.stUnicast.aucSrcAddr;
        pucDstAddr = pstItem->unParam.stUnicast.aucDstAddr;

        ulTimeout = pstItem->ulTime + MRS_SEC_TO_MS((HI_U32)pstItem->unParam.stUnicast.usTimeout) - ulTimeStamp;
    }

    pstCtx->pstProcItem = pstItem;
    pstCcoCtx->stAnytimeRMModel.seq_1376_2 = 0;
    pstCcoCtx->stAnytimeRMModel.ucProtocol = MRS_PROTO645_VER_1997;
    pstCcoCtx->stAnytimeRMModel.from = XR_FROM_PROTO_CSG;

    mrsSetCsgPlcOverTimeInf(pstItem->ucPriority);
    mrsAnyTimeReadMeterInit();
    ulRet = mrsCsgAnyTimeReadMeterProcess(pucSrcAddr, pucDstAddr, pstItem->pFrame, pstItem->ucFrameLen);
    if (HI_ERR_SUCCESS == ulRet)
    {
        ulTimeout = MRS_MIN(MRS_SEC_TO_MS((HI_U32)pstCcoCtx->stAnytimeRMModel.stCsgOverTimeInf.MaxTimeout), ulTimeout);

        MRS_StartTimer(EN_MRS_SRV_CCO_SERVICE_END_TIMER, ulTimeout, HI_SYS_TIMER_ONESHOT);

        ucState &= ~MRS_CCO_STATUS_NORMAL;
        ucState |= MRS_CCO_STATUS_ANYTIME_READMETER;
        mrsCcoSetState(ucState);
    }
    else
    {
        mrsAnyTimeReadMeterInit();
        mrsCcoMrTaskNotify();
        HI_MDM_SetTfNtbSendProcessCtrl(HI_MDM_NTB_SEND_PROCESS_RESUME, HI_MDM_NTB_SEND_CTRL_REASON_READ_METER);
    }

    if (bReplace)
    {
        (hi_void)memcpy_s(pstItem->pFrame + usPos + MRS_645_FRAME_METERADD_OFFSET, HI_METER_ADDR_LEN, aucMeterAddr, HI_METER_ADDR_LEN);
        pstItem->pFrame[usPos + usLength - 2] = mrsToolsCalcCheckSum(pstItem->pFrame + usPos, usLength - 2);
    }

    return ulRet;
}


HI_U32 mrsCcoMrTaskProcListPrUnicast(MRS_CCO_MR_TASK_CONTEXT *pstCtx)
{
    MRS_SRV_LINK *pstLink = pstCtx->stList.next;
    MRS_CCO_MR_TASK_ITEM_STRU *pstItem = (MRS_CCO_MR_TASK_ITEM_STRU *)pstLink;
    HI_U32 ulRet = HI_ERR_SUCCESS;


    while (pstLink != &pstCtx->stList)
    {
        pstItem = (MRS_CCO_MR_TASK_ITEM_STRU *)pstLink;
        pstLink = pstLink->next;

        if (MRS_CCO_MR_TASK_TYPE_MULTICAST == pstItem->ucType)
        {
            return HI_ERR_CONTINUE;
        }

        // 加入并发列表
        ulRet = mrsParallelReadMeterProcCsg(pstItem->pFrame, (HI_U16)pstItem->ucFrameLen, pstItem->unParam.stUnicast.aucDstAddr, (HI_PVOID)pstItem);
        if (HI_ERR_FULL == ulRet)
        {
            break;
        }

        HI_MDM_SetTfNtbSendProcessCtrl(HI_MDM_NTB_SEND_FREQUENCY_REDUCE, HI_MDM_NTB_SEND_CTRL_REASON_READ_METER);
    }

    return HI_ERR_SUCCESS;
}

HI_U32 mrsCcoMrTaskProcListPrMulticast(MRS_CCO_MR_TASK_ITEM_STRU *pstItem)
{
    MRS_CCO_MR_TASK_PARAM_MULTICAST *pstMulti = (MRS_CCO_MR_TASK_PARAM_MULTICAST *)&pstItem->unParam.stMulticast;
    HI_U16 usMeterNum = pstMulti->usSubNodeNum;
    HI_U16 usIdx;
    HI_U16 usPos = 0;
    HI_U16 usLength = 0;
    HI_BOOL bReplace = HI_FALSE;
    HI_U8 aucMeterAddr[HI_METER_ADDR_LEN] = {0};
    HI_U32 ulRet = HI_ERR_SUCCESS;


    if (0 == usMeterNum)
    {
        mrsCcoMrTaskReportItemNak(pstItem, HI_NULL, MRS_CSG_TASK_STATUS_OTHER_ERR);
        mrsCcoMrTaskRemoveNode(pstItem);
        mrsCcoMrTaskNotify();
        return HI_ERR_SUCCESS;
    }

    if (!pstMulti->pBitmap)
    {
        if (HI_ERR_SUCCESS != mrsSrvAllocBits(&pstMulti->pBitmap, usMeterNum))
        {
            mrsCcoMrTaskReportItemNak(pstItem, HI_NULL, MRS_CSG_TASK_STATUS_OTHER_ERR);
            mrsCcoMrTaskRemoveNode(pstItem);
            mrsCcoMrTaskNotify();
            return HI_ERR_NO_MORE_MEMORY;
        }
    }

    HI_MDM_SetTfNtbSendProcessCtrl(HI_MDM_NTB_SEND_PROCESS_RESUME, HI_MDM_NTB_SEND_CTRL_REASON_READ_METER);

    ulRet = mrsFind645Frame(pstItem->pFrame, pstItem->ucFrameLen, &usPos, &usLength);
    if (HI_ERR_SUCCESS == ulRet)
    {
        bReplace = HI_TRUE;
        (hi_void)memcpy_s(aucMeterAddr, HI_METER_ADDR_LEN, pstItem->pFrame + usPos + MRS_645_FRAME_METERADD_OFFSET, HI_METER_ADDR_LEN);
    }

    for (usIdx = 0; usIdx < usMeterNum; usIdx++)
    {
        HI_U8 *pucAddr = HI_NULL;
        MRS_ARCHIVES_NODE_STRU *pstNode = HI_NULL;

        if (mrsSrvBitTest(pstMulti->pBitmap, usIdx))
        {
            continue;
        }

        if (MRS_CCO_MR_TASK_MULTI_ALLMETER == pstMulti->ucType)
        {
            pstNode = mrsSrvArchivesQuery(usIdx);
            if ((pstNode == HI_NULL) || !pstNode->bValid)
            {
                mrsSrvBitSet(pstMulti->pBitmap, usIdx);
                continue;
            }

            pucAddr = pstNode->aucNodeAddr;
        }
        else
        {
            pucAddr = pstMulti->pSubNode + HI_METER_ADDR_LEN * usIdx;
        }

        if (bReplace)
        {
            (hi_void)memcpy_s(pstItem->pFrame + usPos + MRS_645_FRAME_METERADD_OFFSET, HI_METER_ADDR_LEN, pucAddr, HI_METER_ADDR_LEN);
            pstItem->pFrame[usPos + usLength - 2] = mrsToolsCalcCheckSum(pstItem->pFrame + usPos, usLength - 2);
        }

        ulRet = mrsParallelReadMeterProcCsg(pstItem->pFrame, (HI_U16)pstItem->ucFrameLen, pucAddr, (HI_PVOID)pstItem);
        if (HI_ERR_FULL == ulRet)
        {
            break;
        }
    }

    if (bReplace)
    {
        (hi_void)memcpy_s(pstItem->pFrame + usPos + MRS_645_FRAME_METERADD_OFFSET, HI_METER_ADDR_LEN, aucMeterAddr, HI_METER_ADDR_LEN);
        pstItem->pFrame[usPos + usLength - 2] = mrsToolsCalcCheckSum(pstItem->pFrame + usPos, usLength - 2);
    }

    return HI_ERR_SUCCESS;
}


// 处理抄表列表: 并发方式
HI_U32 mrsCcoMrTaskProcListPr(MRS_CCO_MR_TASK_CONTEXT *pstCtx)
{
    MRS_CCO_MR_TASK_ITEM_STRU *pstItem = (MRS_CCO_MR_TASK_ITEM_STRU *)pstCtx->stList.next;

    if (MRS_CCO_MR_TASK_TYPE_MULTICAST == pstItem->ucType)
    {
        return mrsCcoMrTaskProcListPrMulticast(pstItem);
    }

    return mrsCcoMrTaskProcListPrUnicast(pstCtx);
}


// 处理抄表列表: 广播校时
HI_U32 mrsCcoMrTaskProcListBc(MRS_CCO_MR_TASK_CONTEXT *pstCtx)
{
    MRS_CCO_MR_TASK_ITEM_STRU *pstItem = (MRS_CCO_MR_TASK_ITEM_STRU *)pstCtx->stList.next;
    HI_U32 ulRet = HI_ERR_SUCCESS;

    // 停止透抄、并发
    mrsAnyTimeReadMeterFinishProc();
    mrsRmModuleReset();

    pstCtx->pstProcItem = pstItem;

    mrsBroadcastRegFinishNotify(mrsCcoMrTaskBcNotify, pstItem);
    ulRet = mrsBroadcastProcess(pstItem->pFrame, pstItem->ucFrameLen);
    if (HI_ERR_SUCCESS == ulRet)
    {
        return HI_ERR_SUCCESS;
    }

    mrsBroadcastRegFinishNotify(HI_NULL, HI_NULL);
    mrsCcoMrTaskReportItemNak(pstItem, HI_NULL, MRS_CSG_TASK_STATUS_OTHER_ERR);
    mrsCcoMrTaskRemoveNode(pstItem);
    mrsCcoMrTaskNotify();

    return HI_ERR_SUCCESS;
}


// 处理抄表列表
HI_U32 mrsCcoMrTaskProcList(MRS_CCO_MR_TASK_CONTEXT *pstCtx)
{
    MRS_CCO_MR_TASK_PROC_MODE_E eMode = mrsCcoMrTaskGetMode();

    switch (eMode)
    {
    case MRS_CCO_MR_TASK_MODE_BC:
        mrsCcoMrTaskProcListBc(pstCtx);
        break;

    case MRS_CCO_MR_TASK_MODE_XR:
        mrsCcoMrTaskProcListXr(pstCtx);
        break;

    case MRS_CCO_MR_TASK_MODE_PR:
        mrsCcoMrTaskProcListPr(pstCtx);
        break;

    default:
        break;
    }

    return HI_ERR_SUCCESS;
}


HI_U32 mrsCcoMrTaskProc(HI_VOID)
{
    MRS_CCO_MR_TASK_CONTEXT *pstMrTaskCtx = mrsCcoGetMrTaskCtx();

    MRS_StopTimer(MRS_CCO_TIMER_MR_TASK_ACTIVE);

    // 遍历超时任务，上报任务状态
    if (HI_ERR_SUCCESS != mrsCcoMrTaskCheckTimeout(pstMrTaskCtx))
    {
        return HI_ERR_FULL;
    }

    // 抄表暂停/通道不可用：任务不执行
    if (!mrsCcoMrTaskIsEnabled()
        || (!mrsCcoMrTaskIsChlAvailable() && (HI_ERR_SUCCESS != mrsCheckMeterNum())))
    {
        MRS_StartTimer(MRS_CCO_TIMER_MR_TASK_ACTIVE, MRS_CCO_TIME_MR_TASK_ACTIVE, HI_SYS_TIMER_ONESHOT);
        return HI_ERR_SUCCESS;
    }

    return mrsCcoMrTaskProcList(pstMrTaskCtx);
}


HI_U32 mrsSrvAllocBits(MRS_SRV_BITMAP **pstBitmap, HI_U16 usSize)
{
    HI_U16 usMallocSize = (usSize + 7) / 8;
    HI_U8 *pucData = HI_NULL;
    MRS_SRV_BITMAP *pstTemp = HI_NULL;

    usMallocSize += sizeof(MRS_SRV_BITMAP);

    pucData = (HI_U8 *)mrsToolsMalloc(usMallocSize);
    if (!pucData)
    {
        return HI_ERR_NO_MORE_MEMORY;
    }

    (hi_void)memset_s(pucData, usMallocSize, 0, usMallocSize);
    pstTemp = (MRS_SRV_BITMAP *)pucData;
    pstTemp->usSize = usSize;
    pstTemp->usCnt = 0;

    *pstBitmap = pstTemp;

    return HI_ERR_SUCCESS;
}


HI_VOID mrsSrvFreeBits(MRS_SRV_BITMAP **pstBitmap)
{
    mrsToolsFree(*pstBitmap);
    *pstBitmap = HI_NULL;
}


HI_U32 mrsSrvBitSet(MRS_SRV_BITMAP *pstBitmap, HI_U16 usPos)
{
    HI_U16 usBase = usPos / 8;
    HI_U8 usIdx = (usPos % 8);

    if (usPos >= pstBitmap->usSize)
    {
        return HI_ERR_FAILURE;
    }

    if (!mrsSrvBitTest(pstBitmap, usPos))
    {
        BIT8_SET(pstBitmap->pBitmap[usBase], usIdx);
        pstBitmap->usCnt++;
    }

    return HI_ERR_SUCCESS;
}


HI_BOOL mrsSrvBitTest(MRS_SRV_BITMAP *pstBitmap, HI_U16 usPos)
{
    HI_U16 usBase = usPos / 8;
    HI_U8 usIdx = (usPos % 8);

    if (usPos >= pstBitmap->usSize)
    {
        return HI_FALSE;
    }

    return (HI_BOOL)BIT8_GET(pstBitmap->pBitmap[usBase], usIdx);
}


HI_BOOL mrsSrvBitIsFull(MRS_SRV_BITMAP *pstBitmap)
{
    return (HI_BOOL)(pstBitmap->usCnt == pstBitmap->usSize);
}


/* BEGIN: PN: DTS2016081300132 MODIFY\ADD\DEL by cuiate/00233580 at 2016/8/13 */
HI_BOOL mrsCcoMrTaskIsMultiListValid(HI_U8 *pucMeterList, HI_U16 usMeterNum)
{
    HI_U16 usIndex = 0;

    for (usIndex = 0; usIndex < usMeterNum; usIndex++)
    {
        HI_U8 *pucMeter = pucMeterList + usIndex * HI_METER_ADDR_LEN;
        MRS_BufLog("Meter:", pucMeter, HI_METER_ADDR_LEN);

        if (mrsToolsBroadCastAddr(pucMeter) || mrsCcoMrTaskIsMeterRepeat(pucMeterList, usIndex, usMeterNum))
        {
            return HI_FALSE;
        }
    }

    return HI_TRUE;
}


HI_BOOL mrsCcoMrTaskIsMeterRepeat(HI_U8 *pucMeterList, HI_U16 usMeterIndex, HI_U16 usMeterNum)
{
    HI_U16 usIndex = 0;
    HI_U8 *pucMeter = pucMeterList + usMeterIndex * HI_METER_ADDR_LEN;
    HI_U8 *pucTemp = HI_NULL;

    for (usIndex = usMeterIndex + 1; usIndex < usMeterNum; usIndex++)
    {
        pucTemp = pucMeterList + usIndex * HI_METER_ADDR_LEN;
        if (mrsToolsMemEq(pucMeter, pucTemp, HI_METER_ADDR_LEN))
        {
            return HI_TRUE;
        }
    }

    return HI_FALSE;
}
/* END:   PN: DTS2016081300132 MODIFY\ADD\DEL by cuiate/00233580 at 2016/8/13 */


#endif


HI_END_HEADER
