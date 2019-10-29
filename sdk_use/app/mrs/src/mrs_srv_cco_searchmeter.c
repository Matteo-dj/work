//*****************************************************************************
//
//                  版权所有 (C), 1998-2011, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_srv_cco_searchmeter.h
//  版 本 号   : V1.0
//  作    者   :
//  生成日期   :
//  功能描述   : 搜表模块函数及接口定义
//
//  函数列表   : TODO: ...
//  修改历史   :
//  1.日    期 : 2012-05-18
//    作    者 : fengxiaomin/f00209182
//    修改内容 : 创建文件
//
//*****************************************************************************

#include "mrs_common.h"
#include "mrs_cmd_msg.h"
#include "mrs_fw_proto645.h"
#include "mrs_fw_proto376_2_echo.h"
#include "mrs_fw_proto376_2_frame.h"
#include "mrs_fw_tools.h"
#include "mrs_fw_log.h"
#include "mrs_fw_n.h"
#include "mrs_srv_cco_macmap.h"
#include "mrs_srv_list.h"
#include "mrs_srv_common.h"
#include "mrs_srv_res.h"
#include "mrs_srv_cco.h"
#include "mrs_srv_mrslog.h"
#include "mrs_dfx.h"
#include "hi_mdm.h"
#include "mrs_srv_cco_searchmeter.h"
#include "mrs_srv_cco_queue.h"
#include "mrs_dfx_cco.h"
#include "mrs_srv_cco_searchmeter_n.h"
#include "app_config.h"
#include "mrs_srv_cco_readmeter.h"
#include "mrs_fw_proto188.h"
#include "mrs_srv_cco_archives.h"

HI_START_HEADER
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)

HI_PRV hi_void search_result_to_macmap(MRS_MAP_MODULE *map_ctx, METER_NODE *node, hi_u8 status, hi_bool repeat);

HI_PRV HI_MAC_NETWORK_TOPO_S g_stBackupTopo;
#define GetBackupTopo()     &g_stBackupTopo


//*****************************************************************************
// 函数名称: mrsCcoSearchMeterInit
// 功能描述: 搜表模块初始化
//
// 参数说明:
//   pstCcoCtx [in/out] 当前状态
//
// 返 回 值:
//   HI_ERR_SUCCESS 成功
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: fengxiaomin/f00209182 [2012-05-24]
// 修    改: cuiate/c00233580 [2013-08-14]
// 修    改: liupeidong/l00238134 [2015-01-18]
//*****************************************************************************
HI_U32 mrsCcoSearchMeterInit(MRS_CCO_SRV_CTX_STRU * pstCcoCtx, HI_U32 ulSearchMode)
{
    MR_AUTO_SEARCH_STRU *search = HI_NULL;
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U32 status = mrsCcoGetState();

    HI_ASSERT(HI_NULL != pstCcoCtx);

    (HI_VOID)mrsCcoSearchMeterReset(pstCcoCtx);

    search = &(pstCcoCtx->stAutoSearchModel);

    search->usCurrNum = 0;
    search->usCurrSeq = 0;
    search->usFrameLen = 0;
    search->usReportedNum = 0;
    search->usStableTimes = 0;
    search->bQueryComplete = HI_FALSE;
    search->bLockComplete  = HI_FALSE;

    if (search->pReportFrame)
    {
        mrsToolsFree(search->pReportFrame);
        search->pReportFrame = HI_NULL;
    }

    if (search->pReportedNodeInf)
    {
        (hi_void)memset_s(search->pReportedNodeInf,
            sizeof(MR_NODE_METER_ADD_STRU) * PRODUCT_CFG_MRS_MAX_NODE_NUM,0,
            sizeof(MR_NODE_METER_ADD_STRU) * PRODUCT_CFG_MRS_MAX_NODE_NUM);
    }

    mrsCcoDfxClearList(&search->stAbnormalCltii);
    mrsCcoDfxClearList(&search->stSearchResultMeter);
    mrsCcoDfxClearList(&search->stSearchResultClt);
    search->stSearchDfx.usTotal = 0;
    search->stSearchDfx.usMeterEntryNum = 0;
    search->stSearchDfx.usCltEntryNum = 0;
    search->pstCurLockNode = HI_NULL;

/* BEGIN: PN: DTS2015061008265 MODIFY\ADD\DEL by cuiate/00233580 at 2015/6/23 */
    if (ulSearchMode == MRS_CCO_STATUS_SEARCH_METER)
    {
        status &= ~MRS_CCO_STATUS_NORMAL;
        status |= MRS_CCO_STATUS_SEARCH_METER;
    }
    else
    {
        status |= MRS_CCO_STATUS_SEARCH_METER_TRANSFORMER;
    }
/* END:   PN: DTS2015061008265 MODIFY\ADD\DEL by cuiate/00233580 at 2015/6/23 */

    mrsCcoSetState(status);

    return ret;
}


HI_U32 mrsCcoSearchAssembleReportFrame(MRS_CCO_SRV_CTX_STRU * cco, METER_NODE* node)
{
    MRS_CMD_METERLIST_ITEM * item = (MRS_CMD_METERLIST_ITEM *)node->item;
    PROTO376_2_FRAME_AFN_AND_DATA_STRU afn_data;
    HI_U8 ucInfoLen = 7;  //通信地址(6个字节)+协议类型(1个字节)
    HI_U8 * buf = HI_NULL;
    HI_U16 buf_len = 0;
    HI_U8 * payload = HI_NULL;
    HI_U16 payload_len = 0;
    HI_U16 offset = 0;
    HI_U16 usNum = 0;
    HI_U32 i;
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U8 reportAA = cco->stAutoSearchModel.stSearchCfg.report_AA_2_master;
    HI_U8 ucReportNum = 0;

    if (cco->stAutoSearchModel.pReportFrame)
    {
        mrsToolsFree(cco->stAutoSearchModel.pReportFrame);
    }

    cco->stAutoSearchModel.pReportFrame = HI_NULL;
    cco->stAutoSearchModel.usFrameLen = 0;

    payload_len = SIZEOF_REPORT_FRAME_STRU;
    if (node->type == MRS_DEVICE_TYPE_COLLECTOR)
    {
        ucReportNum = MRS_MIN(node->num - cco->stAutoSearchModel.ucCurNodeReported, MRS_REPORT_NODE_MAX);
        payload_len += ucInfoLen * ucReportNum;
    }

    payload = mrsToolsMalloc(payload_len);
    if (HI_NULL == payload)
    {
        return HI_ERR_NOT_ENOUGH_MEMORY;
    }

    (hi_void)memset_s(payload, payload_len, 0, payload_len);
    payload[0] = MRS_SEARCH_REPORT_NODE_NUM;
    offset += 1;

    if (MRS_DEVICE_TYPE_METER == node->type)
    {
        if (node->num == 0)
        {
            HI_U8  aucMac[HI_PLC_MAC_ADDR_LEN] = {0};
           (hi_void)memcpy_s(aucMac, HI_PLC_MAC_ADDR_LEN, node->mac, HI_METER_ADDR_LEN);
            mrsHexInvert(aucMac,HI_PLC_MAC_ADDR_LEN);

           (hi_void)memcpy_s(&payload[offset], payload_len - offset, aucMac, HI_METER_ADDR_LEN);
            offset += HI_METER_ADDR_LEN;
            payload[offset++] = METER_PROTO_TRANSPARENT;//未知规约
        }
        else
        {
           (hi_void)memcpy_s(&payload[offset], payload_len - offset, item->addr, HI_METER_ADDR_LEN);
            offset += HI_METER_ADDR_LEN;
            payload[offset++] = item->protocol;
        }
    }
    else
    {
        if (mrsToolsCheckBCD(node->asset, HI_METER_ADDR_LEN))
        {
           (hi_void)memcpy_s(&payload[offset], payload_len - offset, node->asset, HI_METER_ADDR_LEN);
        }
        else
        {
           (hi_void)memcpy_s(&payload[offset], payload_len - offset, node->id, HI_METER_ADDR_LEN);
        }

        offset += HI_METER_ADDR_LEN;
        payload[offset++] = MRS_COLLECTOR_DEFAULT_PROTOCOL;
    }

    usNum = cco->stAutoSearchModel.usCurrNum + 1;
   (hi_void)memcpy_s(&payload[offset], payload_len - offset, &usNum, 2);
    offset += sizeof(usNum);
    payload[offset++] = node->type;

    if (MRS_DEVICE_TYPE_COLLECTOR == node->type)
    {
        payload[offset++] = node->num;
        payload[offset++] = ucReportNum;
        for (i = cco->stAutoSearchModel.ucCurNodeReported; i < cco->stAutoSearchModel.ucCurNodeReported + ucReportNum; i++)
        {
            if (reportAA)
            {
               (hi_void)memcpy_s(&payload[offset], payload_len - offset, item[i].addr, HI_METER_ADDR_LEN);
            }
            else
            {
                HI_U8 aucAddr[HI_METER_ADDR_LEN] = {0};

                mrsMeterAddrPreProc(aucAddr, item[i].addr, item[i].protocol);
               (hi_void)memcpy_s(&payload[offset], payload_len - offset, aucAddr, HI_METER_ADDR_LEN);
            }

            offset += HI_METER_ADDR_LEN;
            if (offset < payload_len)
            {
                payload[offset++] = item[i].protocol;
            }
        }

        cco->stAutoSearchModel.ucCurNodeReported += ucReportNum;
    }

    (hi_void)memset_s(&afn_data, sizeof(afn_data), 0, sizeof(afn_data));
    afn_data.ucAFN = MRS_AFN(0x06);
    afn_data.ucDT[0] = MRS_AFN_FN_LO(4);
    afn_data.ucDT[1] = MRS_AFN_FN_HI(4);
    afn_data.pData = payload;
    afn_data.usDataLen = payload_len;
    ret = mrsCreate3762UpFrame(&afn_data, &buf, &buf_len, HI_NULL, HI_NULL);
    if (HI_ERR_SUCCESS != ret)
    {
        mrsToolsFree(payload);
        return ret;
    }

    cco->stAutoSearchModel.pReportFrame = buf;
    cco->stAutoSearchModel.usFrameLen = buf_len;

    mrsToolsFree(payload);
    return HI_ERR_SUCCESS;
}


//*****************************************************************************
// 函数名称: mrsCcoSearchReportFrameEnQueue
// 功能描述: 将上报从节点表信息帧加入CCO公共队列
//
// 参数说明:
//   This function has no arguments.
//
// 返 回 值:
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: fengxiaomin/f00209182 [2012-05-22]
// 修    改: fengxiaomin/f00209182 [2014-08-19]
// 修    改: liupeidong/l00238134  [2015-01-06]
//*****************************************************************************
HI_VOID mrsCcoSearchReportFrameEnQueue(HI_VOID)
{
    MRS_CCO_SRV_CTX_STRU * cco = mrsCcoGetContext();
    MRS_SRV_CCO_ITEM *pstCcoItem = HI_NULL;

    pstCcoItem = (MRS_SRV_CCO_ITEM *)mrsToolsMalloc(sizeof(MRS_SRV_CCO_ITEM) + cco->stAutoSearchModel.usFrameLen);
    if (!pstCcoItem)
    {
        mrsToolsFree(cco->stAutoSearchModel.pReportFrame);
        mrsDfxCcoQueueMallocFailCnt();
        return;
    }

    (hi_void)memset_s(pstCcoItem,
        sizeof(MRS_SRV_CCO_ITEM) + cco->stAutoSearchModel.usFrameLen,0,
        sizeof(MRS_SRV_CCO_ITEM) + cco->stAutoSearchModel.usFrameLen);
    pstCcoItem->bValid = HI_TRUE;
    pstCcoItem->ucAfn = 0x06;
    pstCcoItem->usFn = 0x04;
    pstCcoItem->usTimeOut = (HI_U16)mrsToolsGetTimeOut(cco->stAutoSearchModel.stSearchCfg.usAfn06F4UpTimeOut);//单位为ms
    pstCcoItem->ucMaxTry = cco->stAutoSearchModel.stSearchCfg.ucAfn06F4UpRetryMax;
    pstCcoItem->usDataLen = cco->stAutoSearchModel.usFrameLen;
   (hi_void)memcpy_s(pstCcoItem->pData, pstCcoItem->usDataLen, cco->stAutoSearchModel.pReportFrame, pstCcoItem->usDataLen);
    pstCcoItem->MrsCcoRxRespProc = mrsAckFun;
    pstCcoItem->MrsCcoRxTotalTimeOutProc = mrsReportTotalTimeOutFun;
    pstCcoItem->MrsCcoRxEveryTimeOutProc = mrsReportTimeOutFun;

    mrsCcoJoinQueue(pstCcoItem);

    mrsActiveCcoQueue();
    mrsToolsFree(cco->stAutoSearchModel.pReportFrame);
}

//*****************************************************************************
// 函数名称: mrsCcoSearchReportSlaveNodeInfo
// 功能描述: 上报从节点信息和设备类型处理函数
//
// 参数说明:
//   This function has no arguments.
//
// 返 回 值:
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: fengxiaomin/f00209182 [2012-05-18]
// 修    改: liupeidong/l00238134  [2015-01-06]
//*****************************************************************************
HI_VOID mrsCcoSearchReportSlaveNodeInfoNormal(HI_VOID)
{
    MRS_CCO_SRV_CTX_STRU * cco = mrsCcoGetContext();
    METER_NODE * node = HI_NULL;
    MRS_CMD_METERLIST_ITEM * item = HI_NULL;
    HI_U8 i;

    node = (METER_NODE *)mrsSrvQueueTop(&(cco->stAutoSearchModel.stMeterQueue));
    if (HI_NULL == node)
    {
        MRS_StartTimer(MRS_CCO_TIMER_SEARCH_REPORT, SCAN_MAP_CYCLE_TIMER_VAL, HI_SYS_TIMER_ONESHOT);
        return;
    }

    if (MRS_DEVICE_TYPE_METER == node->type)
    {
        if (node->num == 0)
        {
            mrsSrvDeQueue(&(cco->stAutoSearchModel.stMeterQueue));
            mrsToolsFree(node);
            MRS_StartTimer(MRS_CCO_TIMER_SEARCH_REPORT, SCAN_MAP_CYCLE_TIMER_VAL, HI_SYS_TIMER_ONESHOT);
            return;
        }
    }

    // 采集器上报时，如果电表数量超过8块，则需要分帧上报，每帧最多上报8块
    mrsCcoSearchAssembleReportFrame(cco, node);
    mrsCcoSearchReportFrameEnQueue();

    if (MRS_DEVICE_TYPE_COLLECTOR == node->type)
    {
        if (cco->stAutoSearchModel.ucCurNodeReported < node->num)
        {
            // 本节点下挂电表没上报完，下次继续上报本节点下挂电表
            return;
        }
        else
        {
            cco->stAutoSearchModel.ucCurNodeReported = 0;
        }
    }

    node = (METER_NODE *)mrsSrvDeQueue(&(cco->stAutoSearchModel.stMeterQueue));

    if (node == HI_NULL)
    {
        return;
    }

    item = (MRS_CMD_METERLIST_ITEM *)node->item;
    for (i = 0; i < node->num; i++)
    {
        Add2ReportedList(cco, &item[i]);
    }

    mrsCcoDfxUpdateSearchResultList(node, cco->stAutoSearchModel.usCurrNum + 1);
    mrsToolsFree(node);

    cco->stAutoSearchModel.usCurrNum++;
}


HI_VOID mrsCcoSearchReportSlaveNodeInfoTransformer(HI_VOID)
{
    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();
    MRS_SRV_QUEUE *pstQueue = &(pstCcoCtx->stAutoSearchModel.stMeterQueue);

    for (;;)
    {
        METER_NODE *pstNode = (METER_NODE *)mrsSrvQueueTop(pstQueue);
        if (!pstNode)
        {
            break;
        }

        if ((MRS_DEVICE_TYPE_COLLECTOR == pstNode->type) || (0 != pstNode->num))
        {
            // 采集器 or STA节点(无电表不加入列表)
            mrsCcoDfxUpdateSearchResultList(pstNode, ++pstCcoCtx->stAutoSearchModel.usCurrNum);
        }

        mrsSrvDeQueue(pstQueue);
        mrsToolsFree(pstNode);
    }

    MRS_StartTimer(MRS_CCO_TIMER_SEARCH_REPORT, SCAN_MAP_CYCLE_TIMER_VAL, HI_SYS_TIMER_ONESHOT);
}


HI_VOID mrsCcoSearchReportSlaveNodeInfo(HI_VOID)
{
    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();

    if (0 == (pstCcoCtx->status & MRS_CCO_STATUS_SEARCH_METER_MASK))
    {
        return;
    }

    if (pstCcoCtx->status & MRS_CCO_STATUS_SEARCH_METER)
    {
        mrsCcoSearchReportSlaveNodeInfoNormal();
    }
    else
    {
        mrsCcoSearchReportSlaveNodeInfoTransformer();
    }
}


//*****************************************************************************
// 函数名称: mrsReportTimeOutFun
// 功能描述: 上报超时处理函数
//
// 参数说明:
//   pstCcoCtx [in/out] 当前状态
//
// 返 回 值:
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: fengxiaomin/f00209182 [2012-05-22]
//*****************************************************************************
HI_U32 mrsReportTimeOutFun(HI_VOID * param)
{
    MRS_CCO_SRV_CTX_STRU* pstCcoCtx = mrsCcoGetContext(); // 定义指向全局上下文结构的指针

    MRS_NOT_USED(param);

    if ((pstCcoCtx->status & MRS_CCO_STATUS_SEARCH_METER) == 0)
    {
        return HI_ERR_FAILURE;
    }

    return HI_ERR_CONTINUE;
}


HI_U32 mrsReportTotalTimeOutFun(HI_VOID * param)
{
    MRS_NOT_USED(param);
    MRS_StartTimer(MRS_CCO_TIMER_SEARCH_REPORT, MRS_SRV_CCO_TIMER_SEARCH_ACTIV_VALUE, HI_SYS_TIMER_ONESHOT);
    return HI_ERR_SUCCESS;
}


//*****************************************************************************
// 函数名称: mrsAckFun
// 功能描述: 上报确认处理函数
//
// 参数说明:
//   This function has no arguments.
//
// 返 回 值:
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: fengxiaomin/f00209182 [2012-05-22]
//*****************************************************************************
HI_U32 Add2ReportedList(MRS_CCO_SRV_CTX_STRU * cco, MRS_CMD_METERLIST_ITEM * item)
{
    MR_AUTO_SEARCH_STRU * search = &(cco->stAutoSearchModel);
    HI_U16 i;

    if (search->usCurrSeq >= PRODUCT_CFG_MRS_MAX_NODE_NUM)
    {
        return HI_ERR_FULL;
    }

    for (i = 0; i < search->usCurrSeq; i++)
    {
        if (0 == memcmp(item->addr, search->pReportedNodeInf[i].ucMeterAdd, HI_METER_ADDR_LEN))
        {
            return HI_ERR_EXIST;
        }
    }

   (hi_void)memcpy_s(search->pReportedNodeInf[search->usCurrSeq].ucMeterAdd, HI_METER_ADDR_LEN, item->addr, HI_METER_ADDR_LEN);
    search->pReportedNodeInf[search->usCurrSeq].protocol = item->protocol;
    search->usCurrSeq++;
    search->usReportedNum++;

    return HI_ERR_SUCCESS;
}


HI_VOID mrsAckFun(HI_VOID * pAckParam)
{
    MRS_CCO_SRV_CTX_STRU * cco = mrsCcoGetContext();

    MRS_NOT_USED(pAckParam);

    mrsToolsFree(cco->stAutoSearchModel.pReportFrame);
    MRS_StartTimer(MRS_CCO_TIMER_SEARCH_REPORT, MRS_SRV_CCO_TIMER_SEARCH_ACTIV_VALUE, HI_SYS_TIMER_ONESHOT);
}


//*****************************************************************************
// 函数名称: mrsStopSearchMeter
// 功能描述: 停止搜表流程
//
// 参数说明:
//   This function has no arguments.
//
// 返 回 值:
//   HI_ERR_SUCCESS 查找到有效节点
//   0xFFFFFFFF地址映射表错误或读取失败
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: fengxiaomin/f00209182 [2012-05-24]
// 修    改: cuiate/c00233580 [2013-08-14]
//*****************************************************************************
HI_U32 mrsStopSearchMeter(HI_VOID)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();
    HI_U32 status = mrsCcoGetState();

    if (status & MRS_CCO_STATUS_SEARCH_METER)
    {
        MRS_StopTimer(MRS_CCO_TIMER_SEARCH_METER_END);   // 关闭集中器搜表定时器

        mrsCcoDfxSearchEndTime(&(pstCcoCtx->stAutoSearchModel.stSearchDfx));

        MRS_StopTimer(MRS_CCO_TIMER_NOTIFY_STA_LOCK_NETWORK);
        mrsSrvListClear(&(pstCcoCtx->stAutoSearchModel.stUnLockStaList), mrsCcoSearchMeterFreeMeterListNode, HI_NULL);
    }

    if ((status & MRS_CCO_STATUS_SEARCH_METER_MASK) == MRS_CCO_STATUS_SEARCH_METER)
    {
        MRS_StopTimer(MRS_CCO_TIMER_SEARCH_REPORT);   // 关闭开始搜表通讯定时器停止(ID3)
        MRS_StopTimer(MRS_CCO_TIMER_TOPO_QUERY);
        MRS_StopTimer(MRS_CCO_TIMER_GET_METERLIST);

        if (pstCcoCtx->stAutoSearchModel.ucWhiteListDelayTime > 0
            && pstCcoCtx->stAutoSearchModel.stSearchCfg.ucRecoverWhiteSwitch)
        {
            pstCcoCtx->stAutoSearchModel.bWhiteListDelayTimer = HI_TRUE;
            MRS_StartTimer(MRS_CCO_TIMER_SEARCH_METER_WHITELIST, pstCcoCtx->stAutoSearchModel.ucWhiteListDelayTime * 60 * 1000, HI_SYS_TIMER_ONESHOT);
        }
        else
        {
            mrsCcoRecoverWhiteSwitch(pstCcoCtx);
        }

        mrsMapAgingResume();
        pstCcoCtx->ucSearchWaitFlag = HI_FALSE;
    }

    mrsCcoQueueSetItemInvalid(0x06,0x04);

    status &= ~MRS_CCO_STATUS_SEARCH_METER;
    mrsCcoSetState(status);

    return ret;
}

//AFN06 F3 主动上报路由工况变动信息
HI_U32 mrs3762_ReportAFN06_F3(HI_U8 status)
{
    PROTO376_2_FRAME_AFN_AND_DATA_STRU stAfnData;
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_PBYTE buf = HI_NULL;
    HI_U16 len = 0;

    do
    {
        MRS_SRV_CCO_ITEM *pstCcoItem = HI_NULL;
        (hi_void)memset_s(&stAfnData, sizeof(stAfnData), 0, sizeof(stAfnData));
        stAfnData.ucAFN = MRS_AFN(0x06); // AFN=06H
        stAfnData.ucDT[0] = MRS_AFN_FN_LO(3);
        stAfnData.ucDT[1] = MRS_AFN_FN_HI(3);
        stAfnData.pData = &status;
        stAfnData.usDataLen = sizeof(status);

        ret = mrsCreate3762UpFrame(&stAfnData, &buf, &len, HI_NULL, HI_NULL);
        if (HI_ERR_SUCCESS != ret)
        {
            break;
        }

        pstCcoItem = (MRS_SRV_CCO_ITEM *)mrsToolsMalloc(sizeof(MRS_SRV_CCO_ITEM) + len);
        if (!pstCcoItem)
        {
            mrsDfxCcoQueueMallocFailCnt();
            break;
        }

        (hi_void)memset_s(pstCcoItem, sizeof(MRS_SRV_CCO_ITEM) + len, 0, sizeof(MRS_SRV_CCO_ITEM) + len);
        pstCcoItem->bValid = HI_TRUE;
        pstCcoItem->ucAfn = 0x06;
        pstCcoItem->usFn = 0x03;
        pstCcoItem->usTimeOut = 200;//单位为ms
        pstCcoItem->ucMaxTry = 0;
        pstCcoItem->usDataLen = len;
       (hi_void)memcpy_s(pstCcoItem->pData, pstCcoItem->usDataLen, buf, len);
        pstCcoItem->MrsCcoRxRespProc = HI_NULL;
        pstCcoItem->MrsCcoRxTotalTimeOutProc = HI_NULL;
        pstCcoItem->MrsCcoRxEveryTimeOutProc = HI_NULL;

        mrsCcoJoinQueue(pstCcoItem);

        mrsActiveCcoQueue();
    } while (0);

    mrsToolsFree(buf);

    return ret;
}


/* BEGIN: PN: DTS2015042907476 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/29 */
HI_VOID mrsSearchMeterPrepare(MRS_CCO_SRV_CTX_STRU* pstCcoCtx)
{
    mrsCcoSearchMeterInit(pstCcoCtx, MRS_CCO_STATUS_SEARCH_METER);

    if (0 == pstCcoCtx->usSearchDuration)
    {
        pstCcoCtx->usSearchDuration = pstCcoCtx->stAutoSearchModel.stSearchCfg.usSMDurationDefault;
        HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_002, HI_DIAG_MT("[CCO-SM]-搜表持续时间为0,设置为默认时间: [default]"),
            pstCcoCtx->usSearchDuration);
    }

    if (pstCcoCtx->stAutoSearchModel.bWhiteListDelayTimer)
    {
        MRS_StopTimer(MRS_CCO_TIMER_SEARCH_METER_WHITELIST);
        pstCcoCtx->stAutoSearchModel.bWhiteListDelayTimer = HI_FALSE;
        mrsCcoRecoverWhiteSwitch(pstCcoCtx);
    }

    if (pstCcoCtx->stAutoSearchModel.stSearchCfg.ucCloseWhiteWhenSM)
    {
        // 关闭白名单
        HI_MDM_GetWhiteListSwitch(&pstCcoCtx->stAutoSearchModel.bWhiteListSwitch);
        HI_MDM_SetWhiteListSwitch(HI_FALSE, HI_FALSE, HI_MAC_WHIST_LIST_CHANGE_REASON_APP_START_SEARCH);
    }

    MRS_StartTimer(MRS_CCO_TIMER_SEARCH_METER_END, pstCcoCtx->usSearchDuration * 60 * 1000, HI_SYS_TIMER_ONESHOT);
}
/* END:   PN: DTS2015042907476 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/29 */


/* BEGIN: PN: DTS2015042907476 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/29 */
HI_U32 mrsSearchMeterProc(MRS_CCO_SRV_CTX_STRU* pstCcoCtx)
{
    // 开启搜表定时器
/* BEGIN: Modified by fengxiaomin/00209182, 2015/9/7   问题单号号:DTS2015091503701 */
    MRS_StartTimer(MRS_CCO_TIMER_SEARCH_REPORT, MRS_SRV_CCO_TIMER_SEARCH_ACTIV_VALUE * 10, HI_SYS_TIMER_ONESHOT);
/* END:   Modified by fengxiaomin/00209182, 2015/9/7 */

    return mrsCcoStartSearchMeter(pstCcoCtx);
}
/* END:   PN: DTS2015042907476 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/29 */


HI_U32 mrsCcoSearchMeterReset(MRS_CCO_SRV_CTX_STRU * cco)
{
    MR_AUTO_SEARCH_STRU * search = HI_NULL;

    HI_ASSERT(cco != HI_NULL);

    search = &(cco->stAutoSearchModel);
    search->stSearchFrame.send_retry = 0;
    search->stSearchFrame.retry_max = search->stSearchCfg.bc_loop_max;

    if (search->stSearchFrame.plc)
    {
        mrsToolsFree(search->stSearchFrame.plc->pucPayload);
        mrsToolsFree(search->stSearchFrame.plc);
    }

    (hi_void)memset_s(&(search->stMacList), sizeof(search->stMacList), 0, sizeof(search->stMacList));
    mrsSrvListClear(&(search->stUnLockStaList), mrsCcoSearchMeterFreeMeterListNode, HI_NULL);
    mrsSrvInitList(&(search->stUnLockStaList));
    search->pstCurLockNode = HI_NULL;
    mrsSrvEmptyQueue(&(search->stMeterQueue), mrsCcoQueueFree);
    mrsSrvInitQueue(&(search->stMeterQueue));

    return HI_ERR_SUCCESS;
}


HI_U32 mrsCcoCreateStartSearchMeterPlcFrame(MRS_CCO_SRV_CTX_STRU * cco)
{
    MRS_PLC_FRAME_DATA_STRU *plc = HI_NULL;
    MRS_PLC_START_SEARCH_METER_S *start = HI_NULL;
    MR_AUTO_SEARCH_STRU * search = HI_NULL;

    HI_U8 *payload = HI_NULL;
    HI_U16 payload_len = sizeof(MRS_PLC_START_SEARCH_METER_S);


    HI_ASSERT(cco != HI_NULL);

    payload = mrsToolsMalloc(payload_len);
    if (HI_NULL == payload)
    {
        return HI_ERR_MALLOC_FAILUE;
    }

    plc = (MRS_PLC_FRAME_DATA_STRU *)mrsToolsMalloc(sizeof(MRS_PLC_FRAME_DATA_STRU));
    if (HI_NULL == plc)
    {
        mrsToolsFree(payload);
        return HI_ERR_MALLOC_FAILUE;
    }

    search = &(cco->stAutoSearchModel);
    search->stSearchCfg.seq++;

    (hi_void)memset_s(payload, payload_len, 0, payload_len);
    start = (MRS_PLC_START_SEARCH_METER_S *)payload;
    start->interface_ver = MRS_START_SEARCH_DL_VER;
    start->stru_len = sizeof(MRS_PLC_START_SEARCH_METER_S);
    start->option = MRS_SM_START_SEARCH;
    start->seq = search->stSearchCfg.seq;
    HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_066, HI_DIAG_MT("[mrsCcoCreateStartSearchMeterPlcFrame] seq"), start->seq);

    (hi_void)memset_s(plc, sizeof(MRS_PLC_FRAME_DATA_STRU), 0, sizeof(MRS_PLC_FRAME_DATA_STRU));
    plc->usId = ID_MRS_CMD_START_SEARCH_METER;
    plc->pucPayload = payload;
    plc->usPayloadLen = payload_len;
    (hi_void)memset_s(plc->ucMac, HI_PLC_MAC_ADDR_LEN, 0xFF, HI_PLC_MAC_ADDR_LEN);

    search->stSearchFrame.plc = plc;

    return HI_ERR_SUCCESS;
}


HI_U32 mrsCcoCreateStopSearchMeterPlcFrame(MRS_CCO_SRV_CTX_STRU * cco)
{
    MRS_PLC_FRAME_DATA_STRU *plc = HI_NULL;
    MRS_PLC_STOP_SEARCH_METER_S *stop = HI_NULL;
    HI_U16 payload_len = sizeof(MRS_PLC_STOP_SEARCH_METER_S);
    HI_U8 *payload = HI_NULL;

    HI_ASSERT(cco != HI_NULL);

    payload = mrsToolsMalloc(payload_len);
    if (HI_NULL == payload)
    {
        return HI_ERR_MALLOC_FAILUE;
    }

    plc = (MRS_PLC_FRAME_DATA_STRU *)mrsToolsMalloc(sizeof(MRS_PLC_FRAME_DATA_STRU));
    if (HI_NULL == plc)
    {
        mrsToolsFree(payload);
        return HI_ERR_MALLOC_FAILUE;
    }

    (hi_void)memset_s(payload, payload_len, 0, payload_len);
    stop = (MRS_PLC_STOP_SEARCH_METER_S *)payload;
    stop->interface_ver = MRS_STOP_SEARCH_DL_VER;
    stop->stru_len = sizeof(MRS_PLC_STOP_SEARCH_METER_S);
    stop->seq = cco->stAutoSearchModel.stSearchCfg.seq;

    (hi_void)memset_s(plc, sizeof(MRS_PLC_FRAME_DATA_STRU), 0, sizeof(MRS_PLC_FRAME_DATA_STRU));
    plc->usId = ID_MRS_CMD_STOP_SEARCH_METER;
    plc->pucPayload = payload;
    plc->usPayloadLen = payload_len;
    (hi_void)memset_s(plc->ucMac, HI_PLC_MAC_ADDR_LEN, 0xFF, HI_PLC_MAC_ADDR_LEN);

    cco->stAutoSearchModel.stSearchFrame.plc = plc;

    return HI_ERR_SUCCESS;
}


HI_U32 mrsCcoSendSearchMeterPlcFrame(MRS_CCO_SRV_CTX_STRU * cco)
{
    HI_U32 ret = HI_ERR_SUCCESS;

    HI_ASSERT(cco != HI_NULL);

    do
    {
        MR_AUTO_SEARCH_STRU * search = &(cco->stAutoSearchModel);

        if (HI_NULL == search->stSearchFrame.plc)
        {
            break;
        }

        if (search->stSearchFrame.send_retry >= search->stSearchFrame.retry_max)
        {
            if (ID_MRS_CMD_START_SEARCH_METER == search->stSearchFrame.plc->usId)
            {
                MRS_StartTimer(MRS_CCO_TIMER_GET_METERLIST, search->stSearchCfg.query_interval * 1000, HI_SYS_TIMER_ONESHOT);
            }

            mrsToolsFree(search->stSearchFrame.plc->pucPayload);
            mrsToolsFree(search->stSearchFrame.plc);

            break;
        }

        (HI_VOID)MRS_SendPlcFrame(search->stSearchFrame.plc);

        search->stSearchFrame.send_retry++;
        if (search->stSearchFrame.send_retry < search->stSearchFrame.retry_max)
        {
            MRS_StartTimer(MRS_CCO_TIMER_SEARCHMETER_SEND_FRAME, search->stSearchCfg.bc_send_interval * 100, HI_SYS_TIMER_ONESHOT);
            break;
        }

        if (ID_MRS_CMD_START_SEARCH_METER == search->stSearchFrame.plc->usId)
        {
            MRS_StartTimer(MRS_CCO_TIMER_GET_METERLIST, search->stSearchCfg.query_interval * 1000, HI_SYS_TIMER_ONESHOT);
        }

        mrsToolsFree(search->stSearchFrame.plc->pucPayload);
        mrsToolsFree(search->stSearchFrame.plc);
    } while (0);

    return ret;
}


HI_U32 mrsCcoStartSearchMeter(MRS_CCO_SRV_CTX_STRU * cco)
{
    HI_U32 ret = HI_ERR_SUCCESS;

    HI_ASSERT(HI_NULL != cco);

/* BEGIN: PN: DTS2015042907476 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/29 */
// DELETE 关闭白名单代码
/* END:   PN: DTS2015042907476 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/29 */

    do
    {
        HI_MAC_NETWORK_TOPO_S * topo = HI_NULL;

        ret = mrsCcoSearchMeterGetTopo(cco, &topo);
        if (HI_ERR_SUCCESS != ret)
        {
            HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_008, HI_DIAG_MT("[Failed] mrsCcoSearchMeterGetTopo"));
            break;
        }

        ret = mrsCcoSearchMeterMacListInit(cco, topo);
        if (HI_ERR_SUCCESS != ret)
        {
            HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_009, HI_DIAG_MT("[Failed] mrsCcoSearchMeterMacListInit"));
            break;
        }

        ret = mrsCcoCreateStartSearchMeterPlcFrame(cco);
        if (HI_ERR_SUCCESS != ret)
        {
            HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_011, HI_DIAG_MT("[Failed] mrsCcoCreateStartSearchMeterPlcFrame"));
            break;
        }

        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_012, HI_DIAG_MT("Send start Search Plc Frame"));
        ret = mrsCcoSendSearchMeterPlcFrame(cco);
    } while (0);

    if (HI_ERR_SUCCESS != ret)
    {
        mrsStopSearchMeter();
        mrsStopSearchMeterTransformer();
        (HI_VOID)mrsCcoSearchMeterReset(cco);
    }
    else
    {
        mrsMapAgingPause();
        mrsMapTtlZero(&(cco->stMapModule));

    }

    return ret;
}


HI_U32 mrsCcoStopSearchMeter(MRS_CCO_SRV_CTX_STRU * cco)
{
    HI_U32 ret = HI_ERR_SUCCESS;

    HI_ASSERT(HI_NULL != cco);

    do
    {
        (HI_VOID)mrsCcoSearchMeterReset(cco);

        ret = mrsCcoCreateStopSearchMeterPlcFrame(cco);
        if (HI_ERR_SUCCESS != ret)
        {
            HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_014, HI_DIAG_MT("[Failed]mrsCcoCreateStopSearchMeterPlcFrame"));
            break;
        }

        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_015, HI_DIAG_MT("Send stop Search Plc Frame"));
        ret = mrsCcoSendSearchMeterPlcFrame(cco);
    } while (0);

    return ret;
}


HI_U32 mrsCcoGetMeterList(MRS_CCO_SRV_CTX_STRU  * cco, MAC_ITEM * item)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    MRS_PLC_FRAME_DATA_STRU plc;
    MRS_PLC_METERLIST_DL_S * meterlist = HI_NULL;
    MR_AUTO_SEARCH_STRU * search = HI_NULL;
    HI_U8 * payload = HI_NULL;
    HI_U16 payload_len = sizeof(MRS_PLC_METERLIST_DL_S);
/* BEGIN: PN: DTS2016021906890 MODIFY\ADD\DEL by cuiate/00233580 at 2016/2/20 */
    HI_U8 aucIp[HI_IPV4_ADDR_LEN];
/* END:   PN: DTS2016021906890 MODIFY\ADD\DEL by cuiate/00233580 at 2016/2/20 */

    HI_ASSERT(HI_NULL != cco);

    if (!item)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    payload = mrsToolsMalloc(payload_len);
    if (HI_NULL == payload)
    {
        return HI_ERR_MALLOC_FAILUE;
    }

    search = &(cco->stAutoSearchModel);

    (hi_void)memset_s(payload, payload_len, 0, payload_len);
    meterlist = (MRS_PLC_METERLIST_DL_S *)payload;
    meterlist->interface_ver = MRS_GET_METERLIST_DL_VER;
    meterlist->stru_len = payload_len;
    meterlist->option = MRS_SM_SEARCH_RESULT;
    meterlist->seq = cco->stAutoSearchModel.stSearchCfg.seq;
    if (item->query >= search->stSearchCfg.query_force_times)
    {
        meterlist->force_resp = 1;
    }

/* BEGIN: PN: DTS2016021906890 MODIFY\ADD\DEL by cuiate/00233580 at 2016/2/20 */
    (HI_VOID)HI_MDM_NM_GetAddr(meterlist->src, aucIp);
   (hi_void)memcpy_s(meterlist->dst, HI_PLC_MAC_ADDR_LEN, item->mac, HI_PLC_MAC_ADDR_LEN);
/* END:   PN: DTS2016021906890 MODIFY\ADD\DEL by cuiate/00233580 at 2016/2/20 */

    (hi_void)memset_s(&plc, sizeof(plc), 0, sizeof(plc));
    plc.usId = ID_MRS_CMD_GET_METERLIST;
    plc.pucPayload = payload;
    plc.usPayloadLen = payload_len;
   (hi_void)memcpy_s(plc.ucMac, HI_PLC_MAC_ADDR_LEN, item->mac, HI_PLC_MAC_ADDR_LEN);

    ret = MRS_SendPlcFrame(&plc);

    mrsToolsFree(payload);

    return ret;
}


/* 添加到映射表 */
hi_void search_result_to_macmap(MRS_MAP_MODULE *map_ctx, METER_NODE *node, hi_u8 status, hi_bool repeat)
{
    MRS_CMD_METERLIST_ITEM *item = (MRS_CMD_METERLIST_ITEM *)node->item;
    hi_u8 phase = PHASE_A;
    hi_u8 i = 0;

    mrsMapDeleteMac(map_ctx, node->mac, repeat);
    mrsMapMarkMac(map_ctx, node->mac, status);

    if (node->type == MRS_DEVICE_TYPE_COLLECTOR) {
        hi_u8 type = MRS_MAP_DEV_TYPE_CLT_II;

        if (mrsGetCltNodeType(node->mac, &type) == HI_ERR_SUCCESS) {
            MRS_DEVICE_INF_STRU clt_inf;

            (hi_void)memset_s(&clt_inf, sizeof(clt_inf), 0, sizeof(clt_inf));
            (hi_void)memcpy_s(clt_inf.aucAddr, sizeof(clt_inf.aucAddr), node->asset, sizeof(node->asset));
            clt_inf.ucType = type;
            mrsMapAddCollector(map_ctx, node->mac, &clt_inf);
        }
    }

    phase = mrsCcoGetStaPhase(node->mac);
    if (phase == PHASE_C) {
        phase = MRS_PHASE_C;
    }

    for (i = 0; i < node->num; i++, item++) {
        MRS_DEVICE_INF_STRU meter;

        (hi_void)memset_s(&meter, sizeof(meter), 0, sizeof(meter));
        mrsMeterAddrPreProc(meter.aucAddr, item->addr, item->protocol);
        meter.ucProtocol = item->protocol;
        meter.ucType = MRS_MAP_DEV_TYPE_METER;
        meter.ucStatus = status;
        mrsMapAddMeter(map_ctx, node->mac, &meter, phase);
    }
}

HI_U32 mrsCmdCCORecvGetWaterMeterAddressInfo(MRS_PLC_METERLIST_UP_S *pData,HI_U16 usDataLen)
{
    //HI_U32 ret = HI_ERR_SUCCESS;
    //MRS_PLC_FRAME_DATA_STRU *plc_data = HI_NULL;
    MRS_CCO_SRV_CTX_STRU * cco_ctx = mrsCcoGetContext();
    //MRS_645_FRAME_STRU frame = {0};
    //HI_U8 * data_645 = HI_NULL;
    HI_U16 data_len = 0;
    //HI_U8 * mac = HI_NULL;
    //HI_U8 *pPayload = HI_NULL;
    //HI_U16 usPayloadLen = 0;

	HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_031, HI_DIAG_MT("CCORecvGetMeterAddressInfo"));
	
    //检查参数
    if (pData->seq != cco_ctx->stWaterMeterSearchModel.usWaterSearchSeq)
    {
        HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_030, HI_DIAG_MT("GetMeterDataInfo seq err [%d][%d]"),
                        pData->seq,cco_ctx->stWaterMeterSearchModel.usWaterSearchSeq);
        return HI_ERR_FAILURE;
    }
	cco_ctx->stWaterMeterSearchModel.usSignleWaterAllNum=pData->meter_list[1]|(pData->meter_list[2]<<8);
	cco_ctx->stWaterMeterSearchModel.ucSignleReportNum=pData->meter_list[3];
	if((pData->meter_list[0]!=2) || (usDataLen<cco_ctx->stWaterMeterSearchModel.ucSignleReportNum*7+4)
		||(cco_ctx->stWaterMeterSearchModel.ucSignleReportNum>cco_ctx->stWaterMeterSearchModel.usSignleWaterAllNum))
	{
		return HI_ERR_FAILURE;
	}
    
	MRS_StopTimer(MRS_CCO_TIMER_GET_WATER_METERLIST);
   // 释放备份的Plc下行帧数据
	if(cco_ctx->stWaterMeterSearchModel.stSearchWaterFrame.plc!= HI_NULL)
    {
        if(cco_ctx->stWaterMeterSearchModel.stSearchWaterFrame.plc->pucPayload != HI_NULL)
        {
            mrsToolsFree(cco_ctx->stWaterMeterSearchModel.stSearchWaterFrame.plc->pucPayload);   // 释放转发数据块   
            cco_ctx->stWaterMeterSearchModel.stSearchWaterFrame.plc->pucPayload = HI_NULL;
        }
        
        mrsToolsFree(cco_ctx->stWaterMeterSearchModel.stSearchWaterFrame.plc);   // 释放PLC帧备份    
        cco_ctx->stWaterMeterSearchModel.stSearchWaterFrame.plc = HI_NULL;
    }
	if(cco_ctx->stWaterMeterSearchModel.ucSignleReportNum>0)
	{
		HI_U16 usAddrDataLen = sizeof(MRS_SEARCH_WATER_METER_LIST)+cco_ctx->stWaterMeterSearchModel.usSignleWaterAllNum*8;
		MRS_SEARCH_WATER_METER_LIST *node = (MRS_SEARCH_WATER_METER_LIST*)mrsToolsMalloc(usAddrDataLen);
		if(!node)
		{
			return HI_ERR_MALLOC_FAILUE;
		}
		mrsToolsMemZero_s(node,usAddrDataLen,usAddrDataLen);
		mrsToolsMemcpy_s(node->ucaStaMac, 6,pData->src, 6);
		node->ucMeterNum = cco_ctx->stWaterMeterSearchModel.ucSignleReportNum;
		data_len=node->ucMeterNum*8;
		mrsToolsMemcpy_s(node->pMeterData, data_len,pData->meter_list+4, data_len);
		mrsSrvListInsTail(&(cco_ctx->stWaterMeterSearchModel.stGetWaterMeterList),&(node->link));
		HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_032, HI_DIAG_MT("CCORecvGetMeterAddressInfo Add Node"),node->ucMeterNum);
	}
	cco_ctx->stWaterMeterSearchModel.usCurrNum++;
	cco_ctx->stWaterMeterSearchModel.stSearchWaterFrame.send_retry=0;
	// 搜索下一块
	mrsCcoSearchMeterGetWaterMeterlist(cco_ctx,HI_TRUE);
}
HI_U32 mrsCmdCCORecvGetOneWayMeterDataInfo(MRS_PLC_METERLIST_UP_S *pData,HI_U16 usDataLen)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    MRS_PLC_FRAME_DATA_STRU *plc_data = HI_NULL;
    MRS_CCO_SRV_CTX_STRU * cco_ctx = mrsCcoGetContext();
    //MRS_645_FRAME_STRU frame = {0};
    HI_U8 * data_buff = HI_NULL;
    HI_U16 data_len = 0;
    HI_U8 * mac = HI_NULL;
	
    //检查参数
    //检查参数
    if (pData->seq != cco_ctx->stReadWaterMeterModel.usRfPlcSeq)
    {
        HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_030, HI_DIAG_MT("GetMeterDataInfo seq err [%d][%d]"),
                        pData->seq,cco_ctx->stReadWaterMeterModel.usRfPlcSeq);
        return HI_ERR_FAILURE;
    }
	//HI_U16 usSignleWaterAllNum=pData->meter_list[1]|(pData->meter_list[2]<<8);
	HI_U16 ucSignleReportNum=pData->meter_list[3];
	if((pData->meter_list[0]!=3) || (usDataLen< 4))
	{
		return HI_ERR_FAILURE;
	}

    mac = plc_data->ucMac;
    mrsToolsMemcpy_s(cco_ctx->aucSrcMac, HI_PLC_MAC_ADDR_LEN, mac, HI_PLC_MAC_ADDR_LEN);
    
    data_buff = pData->meter_list+4;   // 获取转发数据
    data_len = usDataLen-4;;   // 获取转发数据长度
	
   // 释放备份的Plc下行帧数据
	if(cco_ctx->stReadWaterMeterModel.pstRfPlcDataDl != HI_NULL)
    {
        if(cco_ctx->stReadWaterMeterModel.pstRfPlcDataDl->pucPayload != HI_NULL)
        {
            mrsToolsFree(cco_ctx->stReadWaterMeterModel.pstRfPlcDataDl->pucPayload);   // 释放转发数据块   
            cco_ctx->stReadWaterMeterModel.pstRfPlcDataDl->pucPayload = HI_NULL;
        }
        
        mrsToolsFree(cco_ctx->stReadWaterMeterModel.pstRfPlcDataDl);   // 释放PLC帧备份    
        cco_ctx->stReadWaterMeterModel.pstRfPlcDataDl = HI_NULL;
    }
	MRS_StopTimer(MRS_CCO_TIMER_RF_READ_WATERMETER);
	cco_ctx->stReadWaterMeterModel.enCurrRfState=3;
	HI_U8 *pDataTemp=HI_NULL;
	do
	{
		if(data_len!=0)
		{
		 	// 去重复的数据、已上报的数据
			HI_U16 usMeterAllNum = mrsSrvArchivesMeterNum();
			MRS_ARCHIVES_NODE_STRU *pstNode = HI_NULL;
			HI_U32 i=0,j=0;
			HI_U16 usOffent=0;
			HI_U16 usDataLenTemp=0;
			HI_U16 usSigleDataLen=0;
			HI_U8 ucWaterMeter[6]={0};
			HI_U16 usPos=0;
			HI_U16 us188Len=0;
			pDataTemp = mrsToolsMalloc(data_len+2);
		    if (HI_NULL == pDataTemp)
		    {
		        break;
		    }
			mrsToolsMemZero_s(pDataTemp, data_len+2, data_len+2);
			pDataTemp[usOffent++] = pData->seq;
			usOffent++;// 水表上报的个数,预留的位置
			//在此处找到当前电表信息中第一个有效的表信息（序号）
			for(j=0;j<ucSignleReportNum;j++)
			{
				if(usDataLenTemp+8>data_len)
				{
					break;
				}
				mrsToolsMemcpy_s(ucWaterMeter,6,data_buff+usDataLenTemp,6);
				usDataLenTemp+=6;
				HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1009, HI_DIAG_MT("[RF]收到水表地址"), ucWaterMeter,6);
				usSigleDataLen = (data_buff[usDataLenTemp]|(data_buff[usDataLenTemp+1]<<8));
				usDataLenTemp+=2;
				if(usDataLenTemp+usSigleDataLen>data_len)
				{
					HI_DIAG_LOG_MSG_E3(MRS_FILE_LOG_FLAG_080, HI_DIAG_MT("usDataLenTemp usSigleDataLen data_len"),
                        usDataLenTemp,usSigleDataLen,data_len);
					break;
				}

				for (i = 0; i < usMeterAllNum; i++)
				{
					pstNode = mrsSrvArchivesQuery((HI_U16)i);
					if (pstNode == HI_NULL)
					{
						break;
					}

					//if ((pstNode->ucProtocol>=METER_PROTO_SINGLEWAY_WATERMETER)&& pstNode->bValid &&(pstNode->ucRfReportFlag==0)) // 查找到一个有效的表信息
					if ((pstNode->ucProtocol>=METER_PROTO_SINGLEWAY_WATERMETER)&& pstNode->bValid)
					{
						HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1008, HI_DIAG_MT("[RF]水表地址"), pstNode->aucNodeAddr,6);
						if(mrsToolsMemcmp(ucWaterMeter,pstNode->aucNodeAddr,6)==0)
						{
							pstNode->ucRfReportFlag=1;
							if(HI_ERR_SUCCESS == mrsFind188Frame(data_buff+usDataLenTemp, usSigleDataLen,&usPos,&us188Len))
							{
								mrsToolsMemcpy_s(pDataTemp+usOffent, 7, mrsGetT188ProMeterAddress(data_buff+usDataLenTemp+usPos,us188Len), 7);
								usOffent+=7;
								mrsToolsMemcpy_s(pDataTemp+usOffent, usSigleDataLen-7, mrsGetT188ProData(data_buff+usDataLenTemp+usPos,us188Len)+1, mrsGetT188ProDataLen(data_buff+usDataLenTemp+usPos,us188Len)-2);
								usOffent+=mrsGetT188ProDataLen(data_buff+usDataLenTemp+usPos,us188Len)-2;
							}
							else if(0xC6==data_buff[usDataLenTemp+11])
							{
								mrsToolsMemcpy_s(pDataTemp+usOffent, 7, data_buff+usDataLenTemp+2, 7);
								usOffent+=7;
								mrsToolsMemcpy_s(pDataTemp+usOffent, usSigleDataLen-7,data_buff+usDataLenTemp+12, 0x12);
								usOffent+=0x12;
							}
							else
							{
								mrsToolsMemcpy_s(pDataTemp+usOffent, usSigleDataLen, data_buff+usDataLenTemp, usSigleDataLen);
								usOffent+=usSigleDataLen;
							}
							HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_081, HI_DIAG_MT("长度usOffent"),usOffent);
							pDataTemp[1] += 1;
							break;
						}
					}
				}
        usDataLenTemp += usSigleDataLen;
      }
			HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_078, HI_DIAG_MT("去除重复后数据长度"),usOffent);
			if(usOffent <= 2)
			{
				break;
			}
			ret =mrs3762_ReportAFN20_F3(pDataTemp,usOffent,mrsWaterMeterModelRxRespProc,mrsWaterMeterModelAllTimeOutProc);
			if(ret!=HI_ERR_SUCCESS)
			{
				break;
			}
			mrsToolsFree(pDataTemp);
			return HI_ERR_SUCCESS;
		}
	}while(0);
	mrsToolsFree(pDataTemp);
	MRS_StartTimer(MRS_CCO_TIMER_RF_READ_WATERMETER, 100, HI_SYS_TIMER_ONESHOT);
	return HI_ERR_SUCCESS;
}

HI_U32 mrsCmdCcoGetMeterList(HI_U16 id, HI_PVOID req_packet, HI_U16 req_packet_size)
{
    MRS_CCO_SRV_CTX_STRU * cco = mrsCcoGetContext();
    MRS_PLC_FRAME_DATA_S * plc = (MRS_PLC_FRAME_DATA_S *)req_packet;
    MRS_PLC_METERLIST_UP_S * up_frame = HI_NULL;
    METER_NODE * node = HI_NULL;
    HI_U8 * buf = HI_NULL;
    HI_U16 buf_len = 0;
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_BOOL bRepeat = HI_FALSE;

    MRS_NOT_USED(id);
    MRS_NOT_USED(req_packet_size);

    HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_016, HI_DIAG_MT("[CCO-SM]-PLC Get Meter list"));

    if ((HI_NULL == plc) || (HI_NULL == plc->pucPayload)) {
        return HI_ERR_FAILURE;
    }

    HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1000, HI_DIAG_MT("=>[CCO-SM]-PLC plc"), plc, sizeof(MRS_PLC_FRAME_DATA_S));
    HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1001, HI_DIAG_MT("=>[CCO-SM]-PLC payload"), plc->pucPayload, plc->usPayloadLen);

    up_frame = (MRS_PLC_METERLIST_UP_S *)plc->pucPayload;

	if(up_frame->option==MRS_SM_WATER_CMD)
	{
		HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_083, HI_DIAG_MT("[RF]Water Meter Cmd"),up_frame->meter_list[0]);
		if(up_frame->meter_list[0]==2)
		{
			ret = mrsCmdCCORecvGetWaterMeterAddressInfo(up_frame,plc->usPayloadLen-sizeof(MRS_PLC_METERLIST_UP_S));
		}
		else if(up_frame->meter_list[0]==3)
		{
			ret = mrsCmdCCORecvGetOneWayMeterDataInfo(up_frame,plc->usPayloadLen-sizeof(MRS_PLC_METERLIST_UP_S));
		}
		else if(up_frame->meter_list[0]==4)//Matteo
		{
//		HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1020, HI_DIAG_MT("[Matteo]up query frame data"), up_frame, up_frame->stru_len);
		
			HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1021, HI_DIAG_MT("[Matteo]up query frame data"), up_frame->meter_list, 2);
			mrsCcoSaveIICltDeviceType(cco,up_frame->src,up_frame->meter_list[1]);
		}
		else
		{
			return HI_ERR_FAILURE;
		}
		return ret;
	}

    if (plc->usPayloadLen < (up_frame->stru_len + sizeof(MRS_CMD_METERLIST_ITEM) * up_frame->meter_num)) {
        mrsDfxSmFrameInvalid();
        return HI_ERR_FAILURE;
    }

    if (up_frame->option >= MRS_SM_OPTION_MAX) {
        mrsDfxSmFrameInvalid();
        return HI_ERR_FAILURE;
    }

    if (up_frame->type >= EN_MRS_STA_TYPE_MAX) {
        mrsDfxSmFrameInvalid();
        HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_069, HI_DIAG_MT("=>[CCO-SM]-type"), up_frame->type);
        return HI_ERR_FAILURE;
    }

    if ((up_frame->type != EN_MRS_STA_TYPE_METER) && (cco->stAutoSearchModel.stSearchCfg.seq != up_frame->seq)) {
        mrsDfxSmFrameInvalid();
        HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_068, HI_DIAG_MT("=>[CCO-SM]-seq - frame"), cco->stAutoSearchModel.stSearchCfg.seq, up_frame->seq);
        return HI_ERR_FAILURE;
    }

    if ((up_frame->stru_len >= sizeof(MRS_PLC_METERLIST_UP_S))
        && (!mrsSrvVerifyMac(up_frame->dst))
        && (!mrsToolsBroadCastMac(up_frame->dst))) {
        mrsDfxSmFrameInvalid();
        return HI_ERR_FAILURE;
    }

    mrsDfxSmFrameValid();

    if (up_frame->meter_num > PRODUCT_CFG_MRS_MAX_METER_IN_MAC_NUM)
    {
        return HI_ERR_FAILURE;
    }

    buf_len = sizeof(METER_NODE);
    buf_len += sizeof(MRS_CMD_METERLIST_ITEM) * up_frame->meter_num;
    buf = mrsToolsMalloc(buf_len);
    if (!buf) {
        return HI_ERR_MALLOC_FAILUE;
    }

    (hi_void)memset_s(buf, buf_len, 0, buf_len);

    node = (METER_NODE *)buf;

   (hi_void)memcpy_s(node->mac, HI_PLC_MAC_ADDR_LEN, plc->ucMac, HI_PLC_MAC_ADDR_LEN);
   (hi_void)memcpy_s(node->asset, HI_METER_ADDR_LEN, up_frame->asset, HI_METER_ADDR_LEN);
   (hi_void)memcpy_s(node->id, HI_PLC_MAC_ADDR_LEN, up_frame->id, HI_PLC_MAC_ADDR_LEN);

   (hi_void)memcpy_s(node->item, sizeof(MRS_CMD_METERLIST_ITEM) * up_frame->meter_num,
        (HI_U8 *)up_frame + up_frame->stru_len, sizeof(MRS_CMD_METERLIST_ITEM) * up_frame->meter_num);
    node->num = up_frame->meter_num;
    node->type = (up_frame->type == EN_MRS_STA_TYPE_METER) ? MRS_DEVICE_TYPE_METER : MRS_DEVICE_TYPE_COLLECTOR;

    /* 若搜表未完成，则不上报搜表结果给集中器 */
    if (up_frame->status) {
        HI_BOOL bRetFlag = HI_FALSE;
        if ((up_frame->type == EN_MRS_STA_TYPE_METER) && (up_frame->meter_num == 0)) {
            bRetFlag = HI_TRUE;
        }

        mrsCcoUpdateSearchQueryInfo(cco, plc->ucMac, bRetFlag);
        search_result_to_macmap(&cco->stMapModule, node, MRS_SEARCH_STATUS_IN_PROGRESS, HI_FALSE);
        mrsToolsFree(node);
        return HI_ERR_SUCCESS;
    }

    HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_040, HI_DIAG_MT("处理搜表结果上行消息: [option]"), up_frame->option);

    if (mrsCcoBindNetworkEnable() && MRS_SM_BIND_NETWORK == up_frame->option) {
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_041, HI_DIAG_MT("Lock Msg !"));
        ret = mrsCcoHandleBindNetworkReply(cco, plc->ucMac);

        if (mrsCcoSmReportIsWaitBindMsg())
        {
            mrsToolsFree(node);
            return ret;
        }
    }

    /* 映射表维护状态，不执行搜表相关处理，仅在搜表流程执行该处理 */
    if ((mrsCcoGetState() & MRS_CCO_STATUS_SEARCH_METER_MASK) != 0) {
        ret = mrsCcoUpdateSearchResult(cco, plc->ucMac);
        if (HI_ERR_SUCCESS != ret) {
            HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_042, HI_DIAG_MT("Search result Twice !"));
            mrsCcoHandleBindNetworkReply(cco, plc->ucMac);
            mrsToolsFree(node);
            return HI_ERR_FAILURE;
        }

        HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1002, HI_DIAG_MT("[CCO-GM] Query Result: Mac"), node->mac, HI_PLC_MAC_ADDR_LEN);

        ret = HI_ERR_EXIST;
        if (!mrsCcoSearchMacRepeatCheck(cco, node->mac, node->asset)) {
            if (mrsCcoBindNetworkEnable()) {
                mrsCcoSearchMeterInsertMeterListNode(&(cco->stAutoSearchModel.stUnLockStaList), node, buf_len);
            }

            if (!mrsCcoSmReportIsWaitBindMsg()) {
                mrsSrvEnQueue(&(cco->stAutoSearchModel.stMeterQueue), (MRS_SRV_NODE *)node);
            }

            bRepeat = HI_FALSE;
            ret = HI_ERR_SUCCESS;
        } else {
            mrsToolsFree(node);
            bRepeat = HI_TRUE;
        }
    }

    if (node != HI_NULL)
    {
        // 添加到映射表
        search_result_to_macmap(&cco->stMapModule, node, MRS_SEARCH_STATUS_IDLE, bRepeat);

        if (mrsCcoSmReportIsWaitBindMsg())
        {
            mrsToolsFree(node);
        }
    }

    return ret;
}


/* BEGIN: Modified by fengxiaomin/00209182, 2014/5/26   问题单DTS2013122000310 */
HI_BOOL mrsCcoSearchMeterIsValidProduct(HI_U8 product_type)
{
    switch (product_type)
    {
        case HI_ND_TYPE_STA:
        case HI_ND_TYPE_CLTI:
        case HI_ND_TYPE_CLTII:
        case HI_ND_TYPE_THREE_STA:
            return HI_TRUE;
        default:
            break;
    }
    return HI_FALSE;
}
/* END:   Modified by fengxiaomin/00209182, 2014/5/26 */
HI_U32 mrsCcoSearchMeterMacListInit(MRS_CCO_SRV_CTX_STRU *cco, HI_MAC_NETWORK_TOPO_S * topo)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    MRS_SEARCH_METER_MAC_LIST * mac_list = &(cco->stAutoSearchModel.stMacList);
    HI_U32 i, index = 0;

    HI_ASSERT(HI_NULL != cco);

    if (HI_NULL == topo)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    (hi_void)memset_s(mac_list, sizeof(MRS_SEARCH_METER_MAC_LIST), 0, sizeof(MRS_SEARCH_METER_MAC_LIST));

    for (i = 0; i < topo->num; i++)
    {
        if (!mrsCcoSearchMeterIsValidProduct(topo->entry[i].product_type))
        {
            continue;
        }

       (hi_void)memcpy_s(mac_list->mac[index].mac, HI_PLC_MAC_ADDR_LEN, topo->entry[i].mac, HI_PLC_MAC_ADDR_LEN);
        mac_list->mac[index].valid = HI_TRUE;
        mac_list->mac[index].type = topo->entry[i].product_type;
        index++;
    }

    mac_list->num = (HI_U16)index;

    cco->stAutoSearchModel.usStableTimes = 0;
    cco->stAutoSearchModel.ucCurNodeReported = 0;

   (hi_void)memcpy_s(GetBackupTopo(), sizeof(HI_MAC_NETWORK_TOPO_S), topo, sizeof(HI_MAC_NETWORK_TOPO_S));

    MRS_StartTimer(MRS_CCO_TIMER_TOPO_QUERY, cco->stAutoSearchModel.stSearchCfg.topo_query_interval * 60 * 1000, HI_SYS_TIMER_PERIODIC);

    return ret;
}


/* BEGIN: PN: DTS2015061008265 MODIFY\ADD\DEL by cuiate/00233580 at 2015/6/24 */
HI_U16 mrsCcoSearchMeterMacListDelete(MRS_SEARCH_METER_MAC_LIST *pMacList, HI_MAC_NETWORK_TOPO_S * pTopo)
{
    HI_U16 i = 0;
    HI_U16 usDelCnt = 0;

    // 删除不在TOPO中的站点
    for (i = 0; i < pMacList->num; i++)
    {
        MAC_ITEM *pItem = (MAC_ITEM *)(pMacList->mac + i);

        if (pItem->valid)
        {
            HI_U32 j = 0;

            for (j = 0; j < pTopo->num; j++)
            {
                if (!mrsCcoSearchMeterIsValidProduct(pTopo->entry[j].product_type))
                {
                    continue;
                }

                if (mrsToolsMemEq(pItem->mac, pTopo->entry[j].mac, HI_PLC_MAC_ADDR_LEN))
                {
                    break;
                }
            }

            if (j >= pTopo->num)
            {
                pItem->valid = HI_FALSE;
                usDelCnt++;
            }
        }
    }

    return usDelCnt;
}


HI_VOID mrsCcoSearchMeterMacListRealign(MRS_SEARCH_METER_MAC_LIST *pMacList)
{
    // realign
    HI_U16 usDstIdx = 0;
    HI_U16 usSrcIdx = 0;
    HI_U16 usMax = pMacList->num;

    while (usDstIdx < usMax)
    {
        if (usDstIdx >= PRODUCT_CFG_MRS_MAX_TOPO_NUM)
        {
            break;
        }

        MAC_ITEM *pDstItem = (MAC_ITEM *)(pMacList->mac + usDstIdx);
        if (!pDstItem->valid)
        {
            usSrcIdx = MRS_MAX(usSrcIdx + 1, usDstIdx + 1);
            while (usSrcIdx < usMax)
            {
                if (usSrcIdx >= PRODUCT_CFG_MRS_MAX_TOPO_NUM)
                {
                    break;
                }

                MAC_ITEM *pSrcItem = (MAC_ITEM *)(pMacList->mac + usSrcIdx);

                if (pSrcItem->valid)
                {
                   (hi_void)memcpy_s(pDstItem, sizeof(MAC_ITEM), pSrcItem, sizeof(MAC_ITEM));
                    (hi_void)memset_s(pSrcItem, sizeof(MAC_ITEM), 0, sizeof(MAC_ITEM));
                    break;
                }

                usSrcIdx++;
            }

            if (usSrcIdx >= usMax)
            {
                break;
            }
        }

        usDstIdx++;
    }
}


HI_VOID mrsCcoSearchMeterMacListAdd(MRS_SEARCH_METER_MAC_LIST *pMacList, HI_MAC_NETWORK_TOPO_S * pTopo)
{
    HI_U32 i = 0;
    HI_U16 j = 0;
    HI_U16 num = pMacList->num;

    for (i = 1; (i < pTopo->num) && (pMacList->num < PRODUCT_CFG_MRS_MAX_TOPO_NUM); i++)
    {
        if (!mrsCcoSearchMeterIsValidProduct(pTopo->entry[i].product_type))
        {
            continue;
        }

        for (j = 0; j < num; j++)
        {
            if (mrsToolsMemEq(pTopo->entry[i].mac, pMacList->mac[j].mac, HI_PLC_MAC_ADDR_LEN))
            {
                break;
            }
        }

        if (j == num)
        {
           (hi_void)memcpy_s(pMacList->mac[pMacList->num].mac, HI_PLC_MAC_ADDR_LEN, pTopo->entry[i].mac, HI_PLC_MAC_ADDR_LEN);
            pMacList->mac[pMacList->num].query = 0;
            pMacList->mac[pMacList->num].result = HI_FALSE;
            pMacList->mac[pMacList->num].valid = HI_TRUE;
            pMacList->mac[pMacList->num].type = pTopo->entry[i].product_type;
            pMacList->num++;
        }
    }
}


HI_VOID mrsCcoSearchMeterMacListRefresh(MRS_CCO_SRV_CTX_STRU *pCcoCtx, HI_MAC_NETWORK_TOPO_S * pTopo)
{
    HI_U16 usDelCnt = 0;
    MRS_SEARCH_METER_MAC_LIST *pMacList = &(pCcoCtx->stAutoSearchModel.stMacList);
    if (pTopo == HI_NULL)
    {
        return;
    }

    usDelCnt = mrsCcoSearchMeterMacListDelete(pMacList, pTopo);
    if (usDelCnt > 0)
    {
        mrsCcoSearchMeterMacListRealign(pMacList);
        pMacList->num -= usDelCnt;
        pMacList->current = 0;
    }

    mrsCcoSearchMeterMacListAdd(pMacList, pTopo);
}
/* END:   PN: DTS2015061008265 MODIFY\ADD\DEL by cuiate/00233580 at 2015/6/24 */


HI_U32 mrsCcoSearchMeterGetTopo(MRS_CCO_SRV_CTX_STRU * cco, HI_MAC_NETWORK_TOPO_S ** topo)
{
    HI_MAC_CONFIG_INFO_S info;
    HI_U32 ret = HI_ERR_SUCCESS;

    HI_ASSERT(!cco);

    if (!topo)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    info.pTopo = &(cco->stTopoInf);
    ret = HI_MDM_QueryInfo(&info);
    if (HI_ERR_SUCCESS != ret)
    {
        return ret;
    }

    *topo = info.pTopo;

    return HI_ERR_SUCCESS;
}


HI_U32 mrsCcoSearchMeterSaveTopo(HI_MAC_NETWORK_TOPO_S * saved_topo, HI_MAC_NETWORK_TOPO_S * new_topo)
{
    HI_U32 ret = HI_ERR_SUCCESS;

    if (!saved_topo || !new_topo)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

   (hi_void)memcpy_s(saved_topo, sizeof(HI_MAC_NETWORK_TOPO_S), new_topo, sizeof(HI_MAC_NETWORK_TOPO_S));

    return ret;
}


HI_U32 mrsCcoCheckTopoChanged(HI_MAC_NETWORK_TOPO_S * old_topo, HI_MAC_NETWORK_TOPO_S * new_topo, HI_BOOL * changed)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U32 i, j;

    if (!old_topo || !new_topo || !changed)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    if (old_topo->num != new_topo->num)
    {
        *changed = HI_TRUE;
        return ret;
    }

    for (i = 0; i < new_topo->num; i++)
    {
        for (j = 0; j < old_topo->num; j++)
        {
            if (mrsToolsMemEq(new_topo->entry[i].mac, old_topo->entry[j].mac, HI_PLC_MAC_ADDR_LEN))
            {
                break;
            }
        }

        if (j == old_topo->num)
        {
            *changed = HI_TRUE;
            return ret;
        }
    }

    *changed = HI_FALSE;

    return ret;
}


HI_U32 mrsCcoCheckSearchFinished(MRS_CCO_SRV_CTX_STRU * cco)
{
    METER_NODE * node = HI_NULL;

    HI_ASSERT(HI_NULL != cco);

    if (cco->stAutoSearchModel.usStableTimes < cco->stAutoSearchModel.stSearchCfg.topo_stable_times)
    {
        return HI_ERR_FAILURE;
    }

    mrsCcoCheckQueryComplete(cco);
    if (!cco->stAutoSearchModel.bQueryComplete)
    {
        return HI_ERR_FAILURE;
    }

    mrsCcoCheckLockComplete(cco);
    if (!cco->stAutoSearchModel.bLockComplete)
    {
        return HI_ERR_FAILURE;
    }

    node = (METER_NODE *)mrsSrvQueueTop(&(cco->stAutoSearchModel.stMeterQueue));
    if (node)
    {
        return HI_ERR_FAILURE;
    }

    return HI_ERR_SUCCESS;
}


/* BEGIN: PN: DTS2015061008265 MODIFY\ADD\DEL by cuiate/00233580 at 2015/6/26 */
HI_BOOL mrsCcoSearchMacRepeatCheck(MRS_CCO_SRV_CTX_STRU *pCcoCtx, HI_U8 *pMacAddr, HI_U8 *pAssetAddr)
{
/* BEGIN: PN: DTS2015111905511 MODIFY\ADD\DEL by cuiate/00233580 at 2015/11/23 */
    MRS_CCO_SEARCH_QUEUE_VISIT_STRU stVisit;
    MRS_SRV_LIST* pList = HI_NULL;

    (hi_void)memset_s(&stVisit, sizeof(stVisit), 0, sizeof(stVisit));

   (hi_void)memcpy_s(stVisit.aucMac, HI_PLC_MAC_ADDR_LEN, pMacAddr, HI_PLC_MAC_ADDR_LEN);
    if (pAssetAddr != HI_NULL)
    {
       (hi_void)memcpy_s(stVisit.aucAsset, HI_METER_ADDR_LEN, pAssetAddr, HI_METER_ADDR_LEN);
    }

    // 在未锁定列表中查找
    mrsSrvTraverseList(&(pCcoCtx->stAutoSearchModel.stUnLockStaList), mrsCcoSearchMeterFindMeterListNode, &stVisit);
    if (stVisit.bExist)
    {
        return HI_TRUE;
    }

    // 在未上报队列查找
    mrsSrvTraverseQueue(&pCcoCtx->stAutoSearchModel.stMeterQueue, mrsCcoSearchVisitQueue, &stVisit);
    if (stVisit.bExist)
    {
        return HI_TRUE;
    }

    // 在已上报列表查找
    pList = mrsCcoDfxSearchResultCltList();
    mrsSrvTraverseList(pList, mrsCcoSearchVisitCltList, &stVisit);
    if (stVisit.bExist)
    {
        return HI_TRUE;
    }

    pList = mrsCcoDfxSearchResultMeterList();
    mrsSrvTraverseList(pList, mrsCcoSearchVisitMeterList, &stVisit);
    return stVisit.bExist;
/* END:   PN: DTS2015111905511 MODIFY\ADD\DEL by cuiate/00233580 at 2015/11/23 */
}


HI_U32 mrsCcoSearchMeterGetMeterlist(MRS_CCO_SRV_CTX_STRU *cco)
{
    HI_U32 ret = HI_ERR_SUCCESS;

    HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_023, HI_DIAG_MT("[CCO-SM] Get Meter List"));

    HI_ASSERT(HI_NULL != cco);

    MRS_StopTimer(MRS_CCO_TIMER_GET_METERLIST);

    do
    {
        MRS_SEARCH_METER_MAC_LIST * mac_list = &(cco->stAutoSearchModel.stMacList);
        MRS_SEARCH_METER_CFG_S * cfg = &(cco->stAutoSearchModel.stSearchCfg);
        HI_U16 i, count = 0;

        HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_024, HI_DIAG_MT("[CCO-SM]-GML cur-[%d] num-[%d]"), mac_list->current, mac_list->num);

        for (i = mac_list->current; i < mac_list->num && count < cfg->query_max_way; i++)
        {
            HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_025, HI_DIAG_MT("[CCO-SM]-GML rlt-[%d] query-[%d]"), mac_list->mac[i].result, mac_list->mac[i].query);
            HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1005, HI_DIAG_MT("[CCO-SM]-GML MAC"), mac_list->mac[i].mac, 6);

            if ((mac_list->mac[i].result != 0)
                || (mac_list->mac[i].query >= cfg->query_force_times + cfg->query_except_times))
            {
                continue;
            }

            ret = mrsCcoGetMeterList(cco, &(mac_list->mac[i]));
            if (HI_ERR_SUCCESS != ret)
            {
                HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_050, HI_DIAG_MT("mrsCcoGetMeterList 执行失败: [ret]"), ret);
                break;
            }

            mac_list->mac[i].query++;
            count++;
        }

        if ((0 == count) && (0 == mac_list->current))
        {
            cco->stAutoSearchModel.bQueryComplete = HI_TRUE;
            ret = HI_ERR_SUCCESS;
            break;
        }

        if (i >= mac_list->num)
        {
            // TODO: 查询完一轮，间隔60秒再查询
            mac_list->current = 0;
            MRS_StartTimer(MRS_CCO_TIMER_GET_METERLIST, 1000 * 60, HI_SYS_TIMER_PERIODIC);
        }
        else
        {
            mac_list->current = i;
            MRS_StartTimer(MRS_CCO_TIMER_GET_METERLIST, cfg->query_interval * 1000, HI_SYS_TIMER_PERIODIC);
        }
    } while (0);

    return ret;
}


HI_U32 mrsCcoSearchMeterTopoQueryProc(MRS_CCO_SRV_CTX_STRU *cco)
{
    HI_MAC_NETWORK_TOPO_S * topo = HI_NULL;
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_BOOL changed = HI_FALSE;

    HI_ASSERT(HI_NULL != cco);

    ret = mrsCcoSearchMeterGetTopo(cco, &topo);
    if (HI_ERR_SUCCESS != ret)
    {
        return ret;
    }

    if (topo == HI_NULL)
    {
        return HI_ERR_FAILURE;
    }

    ret = mrsCcoCheckTopoChanged(GetBackupTopo(), topo, &changed);
    if (HI_ERR_SUCCESS != ret)
    {
        return ret;
    }

    if (!changed)
    {
        MRS_SM_STABLE_TIME_INCREASE(cco);

        ret = mrsCcoCheckSearchFinished(cco);
        if (HI_ERR_SUCCESS == ret)
        {
            mrsStopSearchMeter();

            if (MRS_CCO_STATUS_SEARCH_METER_TRANSFORMER != (mrsCcoGetState() & MRS_CCO_STATUS_SEARCH_METER_MASK))
            {
                mrs3762_ReportAFN06_F3(MRS_CCO_SEARCH_METER_TASK_END);
                HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_026, HI_DIAG_MT("上报路由工况"));
            }

            cco->stAutoSearchModel.stSearchCfg.search_finish_reason = (HI_U8)MRS_CCO_SEARCH_FINISH_NORMAL;
        }

        return HI_ERR_SUCCESS;
    }

    ret = mrsCcoSearchMeterSaveTopo(GetBackupTopo(), topo);
    if (HI_ERR_SUCCESS != ret)
    {
        return ret;
    }

    MRS_SM_STABLE_TIME_RESET(cco);
    mrsCcoSearchMeterMacListRefresh(cco, topo);

    if (cco->stAutoSearchModel.bQueryComplete)
    {
        cco->stAutoSearchModel.bQueryComplete = HI_FALSE;
        cco->stAutoSearchModel.bLockComplete = HI_FALSE;
        mrsCcoSearchMeterGetMeterlist(cco);
    }

    return ret;
}

HI_U32 mrsCcoUpdateSearchResult(MRS_CCO_SRV_CTX_STRU * cco, HI_U8 * mac)
{
    MRS_SEARCH_METER_MAC_LIST * mac_list = HI_NULL;
    HI_U16 i;

    HI_ASSERT(HI_NULL != cco);

    if (!mac)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    mac_list = &(cco->stAutoSearchModel.stMacList);
    for (i = 0; i < mac_list->num; i++)
    {
        if (mrsToolsMemEq(mac_list->mac[i].mac, mac, HI_PLC_MAC_ADDR_LEN))
        {
            if (mac_list->mac[i].result)
            {
                return HI_ERR_EXIST;
            }

            mac_list->mac[i].result = 1;
            return HI_ERR_SUCCESS;
        }
    }

/* BEGIN: PN: DTS2015061008265 MODIFY\ADD\DEL by cuiate/00233580 at 2015/6/24 */
    return HI_ERR_NOT_FOUND;
/* END:   PN: DTS2015061008265 MODIFY\ADD\DEL by cuiate/00233580 at 2015/6/24 */
}

HI_U32 mrsCcoUpdateSearchQueryInfo(MRS_CCO_SRV_CTX_STRU * cco, HI_U8 * mac, HI_BOOL bRetFlag)
{
    MRS_SEARCH_METER_MAC_LIST * mac_list = HI_NULL;
    HI_U16 i;

    HI_ASSERT(HI_NULL != cco);

    if (!mac)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    mac_list = &(cco->stAutoSearchModel.stMacList);
    for (i = 0; i < mac_list->num; i++)
    {
        if (mrsToolsMemEq(mac_list->mac[i].mac, mac, HI_PLC_MAC_ADDR_LEN))
        {
            if (bRetFlag == HI_TRUE)
            {
                mac_list->mac[i].result = HI_TRUE;
            }
            else
            {
                mac_list->mac[i].query = 0;
            }

            return HI_ERR_SUCCESS;
        }
    }

    return HI_ERR_NOT_FOUND;
}


HI_VOID mrsCcoQueueFree(HI_VOID * p)
{
    mrsToolsFree(p);
}


// 检查所有站点搜表结果是否查询完毕
HI_U32 mrsCcoCheckQueryComplete(MRS_CCO_SRV_CTX_STRU * cco)
{
    MR_AUTO_SEARCH_STRU * sm_ctx = HI_NULL;
    MRS_SEARCH_METER_MAC_LIST * mac_list = HI_NULL;
    MRS_SEARCH_METER_CFG_S * cfg = HI_NULL;
    HI_U16 i = 0;

    HI_ASSERT(cco);

    sm_ctx = &(cco->stAutoSearchModel);
    mac_list = &(sm_ctx->stMacList);
    cfg = &(sm_ctx->stSearchCfg);

    for(i = 0; i < mac_list->num; i++)
    {
        if ((mac_list->mac[i].result != 0)
                || (mac_list->mac[i].query >= cfg->query_force_times + cfg->query_except_times))
        {
            continue;
        }

        return HI_ERR_CONTINUE;
    }

    sm_ctx->bQueryComplete = HI_TRUE;

    return HI_ERR_SUCCESS;
}


/* BEGIN: PN: DTS2015061008265 MODIFY\ADD\DEL by cuiate/00233580 at 2015/6/23 */
HI_U32 mrsCcoStartSearchMeterTransformer(HI_U32 ulDuration)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    MRS_CCO_SRV_CTX_STRU *pstCcoCtx = mrsCcoGetContext();

    if (pstCcoCtx->status & MRS_CCO_STATUS_SEARCH_METER_TRANSFORMER)
    {
        return HI_ERR_SUCCESS;
    }

    if (ulDuration == 0)
    {
        ulDuration = pstCcoCtx->stAutoSearchModel.stSearchCfg.usSMDurationDefault;
    }

    MRS_StartTimer(MRS_CCO_TIMER_SEARCH_METER_END_TF, ulDuration * 60 * 1000, HI_SYS_TIMER_ONESHOT);

    if (pstCcoCtx->status & MRS_CCO_STATUS_SEARCH_METER)
    {
        mrsCcoSetState(pstCcoCtx->status | MRS_CCO_STATUS_SEARCH_METER_TRANSFORMER);
        mrsCcoRecoverWhiteSwitch(pstCcoCtx);
        pstCcoCtx->stAutoSearchModel.bWhiteListDelayTimer = HI_FALSE;
        return HI_ERR_SUCCESS;
    }

    mrsCcoSearchMeterInit(pstCcoCtx, MRS_CCO_STATUS_SEARCH_METER_TRANSFORMER);

    if (mrsGetCcoPlcChlStatus())
    {
        pstCcoCtx->ucSearchWaitFlag = HI_FALSE;
        ulRet = mrsSearchMeterProc(pstCcoCtx);
    }
    else
    {
        pstCcoCtx->ucSearchWaitFlag = HI_TRUE;
    }

    return ulRet;
}


HI_U32 mrsCcoStopSearchMeterTransformer(HI_VOID)
{
    HI_U32 ulStatus = mrsCcoGetState();
    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();

    if (ulStatus & MRS_CCO_STATUS_SEARCH_METER_MASK)
    {
        mrsStopSearchMeterTransformer();

        if ((ulStatus & MRS_CCO_STATUS_SEARCH_METER_MASK) == MRS_CCO_STATUS_SEARCH_METER_TRANSFORMER)
        {
            mrsCcoStopSearchMeter(mrsCcoGetContext());
        }
    }

    if ((ulStatus & MRS_CCO_STATUS_SEARCH_METER) && pstCcoCtx->stAutoSearchModel.stSearchCfg.ucCloseWhiteWhenSM)
    {
        HI_MDM_GetWhiteListSwitch(&pstCcoCtx->stAutoSearchModel.bWhiteListSwitch);
        HI_MDM_SetWhiteListSwitch(HI_FALSE, HI_FALSE, HI_MAC_WHIST_LIST_CHANGE_REASON_APP_START_SEARCH);
    }

    return HI_ERR_SUCCESS;
}


HI_U32 mrsStopSearchMeterTransformer(HI_VOID)
{
    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();
    HI_U32 ulStatus = mrsCcoGetState();

    if (ulStatus & MRS_CCO_STATUS_SEARCH_METER_TRANSFORMER)
    {
        MRS_StopTimer(MRS_CCO_TIMER_SEARCH_METER_END_TF);
    }

    if ((ulStatus & MRS_CCO_STATUS_SEARCH_METER_MASK) == MRS_CCO_STATUS_SEARCH_METER_TRANSFORMER)
    {
/* BEGIN: Modified by fengxiaomin/00209182, 2015/9/7   问题单号号:DTS2015091503701 */
        MRS_StopTimer(MRS_CCO_TIMER_SEARCH_REPORT);   // 关闭开始搜表通讯定时器停止(ID3)
/* END:   Modified by fengxiaomin/00209182, 2015/9/7 */
        MRS_StopTimer(MRS_CCO_TIMER_TOPO_QUERY);
        MRS_StopTimer(MRS_CCO_TIMER_GET_METERLIST);

        mrsMapAgingResume();
        pstCcoCtx->ucSearchWaitFlag = HI_FALSE;
    }

    ulStatus &= ~MRS_CCO_STATUS_SEARCH_METER_TRANSFORMER;
/* BEGIN: Deleted by fengxiaomin/00209182, 2015/9/7   问题单号号:DTS2015091503701 */
    //删除轮抄状态的置位
/* END:   Deleted by fengxiaomin/00209182, 2015/9/7 */

    mrsCcoSetState(ulStatus);

    return HI_ERR_SUCCESS;
}


HI_U32 mrsCcoStopSearchMeterNormal(HI_U8 ucReason)
{
    HI_U32 ulStatus = mrsCcoGetState();
    MRS_CCO_SRV_CTX_STRU *pstCcoCtx = mrsCcoGetContext();

    if (ulStatus & MRS_CCO_STATUS_SEARCH_METER_MASK)
    {
        mrsStopSearchMeter();

        if ((ulStatus & MRS_CCO_STATUS_SEARCH_METER_MASK) == MRS_CCO_STATUS_SEARCH_METER)
        {
            mrsCcoStopSearchMeter(mrsCcoGetContext());
        }
        pstCcoCtx->stAutoSearchModel.stSearchCfg.search_finish_reason = ucReason;
    }

    return HI_ERR_SUCCESS;
}

/* END:   PN: DTS2015061008265 MODIFY\ADD\DEL by cuiate/00233580 at 2015/6/23 */


// 恢复白名单到搜表前状态: 延迟恢复，没有启动延迟定时器则什么都不干
HI_VOID mrsCcoSmRestoreWhitelistSwitch(HI_VOID)
{
    MRS_CCO_SRV_CTX_STRU *pstCcoCtx = mrsCcoGetContext();
    MR_AUTO_SEARCH_STRU *pstSmCtx = &(pstCcoCtx->stAutoSearchModel);

    MRS_StopTimer(MRS_CCO_TIMER_SEARCH_METER_WHITELIST);

    if (pstSmCtx->bWhiteListDelayTimer)
    {
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_036, HI_DIAG_MT("恢复白名单开关"));
        HI_MDM_SetWhiteListSwitch(pstSmCtx->bWhiteListSwitch, HI_FALSE,HI_MAC_WHIST_LIST_CHANGE_REASON_APP_STOP_SEARCH);
        pstSmCtx->bWhiteListDelayTimer = HI_FALSE;
    }
}


HI_VOID mrsCcoSmWhitelistSwitchTimerReset(HI_VOID)
{
    MRS_CCO_SRV_CTX_STRU *pstCcoCtx = mrsCcoGetContext();

    if (pstCcoCtx->stAutoSearchModel.bWhiteListDelayTimer)
    {
        MRS_StartTimer(MRS_CCO_TIMER_SEARCH_METER_WHITELIST, pstCcoCtx->stAutoSearchModel.ucWhiteListDelayTime * 60 * 1000, HI_SYS_TIMER_ONESHOT);
    }
}


HI_U32 mrsGetCltNodeType(HI_U8 *pucMacAddr, HI_U8 *pucType)
{
    MRS_CCO_SRV_CTX_STRU *pstCcoCtx = mrsCcoGetContext();
    MRS_SEARCH_METER_MAC_LIST *pMacList = &pstCcoCtx->stAutoSearchModel.stMacList;
    MAC_ITEM *pItem = pMacList->mac;
    HI_U16 i = 0;

    for (i = 0; i < pMacList->num; i++, pItem++)
    {
        if (pItem->valid && mrsToolsMemEq(pItem->mac, pucMacAddr, HI_PLC_MAC_ADDR_LEN))
        {
            switch (pItem->type)
            {
            case HI_ND_TYPE_CLTI:
                *pucType = MRS_MAP_DEV_TYPE_CLT_I;
                break;

            case HI_ND_TYPE_CLTII:
                *pucType = MRS_MAP_DEV_TYPE_CLT_II;
                break;

            default:
                return HI_ERR_FAILURE;
            }

            return HI_ERR_SUCCESS;
        }
    }

    return HI_ERR_FAILURE;
}

HI_BOOL mrsCcoSearchVisitQueueCompaire(HI_U8 * srcMac,HI_U8 *distMac)
{
    if ((srcMac == HI_NULL) ||(distMac == HI_NULL))
    {
        return HI_FALSE;
    }

    if (mrsToolsMemEq(srcMac, distMac, (HI_PLC_MAC_ADDR_LEN - 1))
        &&((srcMac[MRS_MAC_ADDR_INDEX_5] == 0x00)||(srcMac[MRS_MAC_ADDR_INDEX_5] == 0xFE))
        &&((distMac[MRS_MAC_ADDR_INDEX_5] == 0x00)||(distMac[MRS_MAC_ADDR_INDEX_5] == 0xFE)))
    {
        return HI_TRUE;

    }

    return HI_FALSE;
}

/* BEGIN: PN: DTS2015111905511 MODIFY\ADD\DEL by cuiate/00233580 at 2015/11/23 */
HI_BOOL mrsCcoSearchVisitQueue(HI_VOID *p, HI_VOID *q)
{
    METER_NODE *pstNode = (METER_NODE *)p;
    MRS_CCO_SEARCH_QUEUE_VISIT_STRU *pParam = (MRS_CCO_SEARCH_QUEUE_VISIT_STRU *)q;

    if (mrsToolsMemEq(pstNode->mac, pParam->aucMac, HI_PLC_MAC_ADDR_LEN))
    {
        pParam->bExist = HI_TRUE;
        return HI_TRUE;
    }

    if (mrsCcoSearchVisitQueueCompaire(pstNode->mac, pParam->aucMac)
        && mrsToolsMemEq(pstNode->asset, pParam->aucAsset, HI_METER_ADDR_LEN))
    {
        pParam->bExist = HI_TRUE;
        return HI_TRUE;
    }

    return HI_FALSE;
}


HI_BOOL mrsCcoSearchVisitCltList(HI_VOID *p, HI_VOID *q)
{
    MRS_CLT_NODE *pstNode = (MRS_CLT_NODE *)p;
    MRS_CCO_SEARCH_QUEUE_VISIT_STRU *pParam = (MRS_CCO_SEARCH_QUEUE_VISIT_STRU *)q;

    if (mrsToolsMemEq(pstNode->aucMac, pParam->aucMac, HI_PLC_MAC_ADDR_LEN))
    {
        pParam->bExist = HI_TRUE;
        return HI_TRUE;
    }

    if (mrsCcoSearchVisitQueueCompaire(pstNode->aucMac, pParam->aucMac)
        && mrsToolsMemEq(pstNode->aucAsset, pParam->aucAsset, HI_METER_ADDR_LEN))
    {

        pParam->bExist = HI_TRUE;
        return HI_TRUE;
    }

    return HI_FALSE;
}


HI_BOOL mrsCcoSearchVisitMeterList(HI_VOID *p, HI_VOID *q)
{
    MRS_CARRIER_METER_NODE *pstNode = (MRS_CARRIER_METER_NODE *)p;
    MRS_CCO_SEARCH_QUEUE_VISIT_STRU *pParam = (MRS_CCO_SEARCH_QUEUE_VISIT_STRU *)q;
    HI_U8 aucMac[HI_PLC_MAC_ADDR_LEN];

    (hi_void)memset_s(aucMac, sizeof(aucMac), 0, sizeof(aucMac));
    mrsToolsConvertMeter2Mac(pstNode->aucMeterAddr, pstNode->ucProtoType, aucMac);

    if (mrsToolsMemEq(aucMac, pParam->aucMac, HI_PLC_MAC_ADDR_LEN))
    {
        pParam->bExist = HI_TRUE;
        return HI_TRUE;
    }

    if (mrsCcoSearchVisitQueueCompaire(aucMac, pParam->aucMac)
        && mrsToolsMemEq(pstNode->aucAsset, pParam->aucAsset, HI_METER_ADDR_LEN))
    {
        pParam->bExist = HI_TRUE;
        return HI_TRUE;
    }

    return HI_FALSE;
}
/* END:   PN: DTS2015111905511 MODIFY\ADD\DEL by cuiate/00233580 at 2015/11/23 */

HI_BOOL mrsCcoBindNetworkEnable(HI_VOID)
{
    MRS_CCO_SRV_CTX_STRU * cco = mrsCcoGetContext();

    // 无扰台区识别打开，允许绑定，并且是在普通搜表的时候，才允许锁定命令
    if (SOFT_TF_ON_WITH_STA_JOIN_BY_TF_RESULT == cco->ucSoftTfMode
        && cco->stAutoSearchModel.stSearchCfg.ucBindNetwork
        && (cco->status & MRS_CCO_STATUS_SEARCH_METER_MASK) == MRS_CCO_STATUS_SEARCH_METER)
    {
        return HI_TRUE;
    }
    else
    {
        return HI_FALSE;
    }
}


HI_BOOL mrsCcoSmReportIsWaitBindMsg(HI_VOID)
{
    MRS_CCO_SRV_CTX_STRU * cco = mrsCcoGetContext();

    if (mrsCcoBindNetworkEnable() && cco->stAutoSearchModel.stSearchCfg.ucSmReportWaitLockMsg)
    {
        return HI_TRUE;
    }
    else
    {
        return HI_FALSE;
    }
}


// 检查所有站点是否锁定完毕
HI_U32 mrsCcoCheckLockComplete(MRS_CCO_SRV_CTX_STRU * pstCcoCtx)
{
    MR_AUTO_SEARCH_STRU * pstAutoSearch = HI_NULL;
    MRS_SEARCH_METER_CFG_S * pstSearchCfg = HI_NULL;
    MRS_SRV_LINK * pstLink = HI_NULL;
    METER_LIST_NODE * pstNode = HI_NULL;

    HI_ASSERT(pstCcoCtx);

    pstAutoSearch = &(pstCcoCtx->stAutoSearchModel);
    pstSearchCfg = &(pstAutoSearch->stSearchCfg);

    for (pstLink = pstAutoSearch->stUnLockStaList.next; pstLink != &(pstAutoSearch->stUnLockStaList); pstLink = pstLink->next)
    {
        pstNode = (METER_LIST_NODE *)pstLink;

        if (pstNode->query_times >= pstSearchCfg->query_force_times + pstSearchCfg->query_except_times)
        {
            continue;
        }

        return HI_ERR_CONTINUE;
    }

    pstAutoSearch->bLockComplete = HI_TRUE;

    return HI_ERR_SUCCESS;
}


/**
* @brief 向未锁定列表中的站点发送锁定命令
*
* @par Description:
*     链表中所有节点发一遍，然后等待一段时间间隔(NV可配)继续发。
*/
HI_U32 mrsCcoSearchMeterNotifyStaBindNetwork(MRS_CCO_SRV_CTX_STRU *pstCcoCtx)
{
    MRS_SEARCH_METER_CFG_S * pstSearchCfg = HI_NULL;
    MR_AUTO_SEARCH_STRU *pstAutoSearch = HI_NULL;
    MRS_SRV_LINK *pstLink = HI_NULL;
    METER_LIST_NODE * pstNode = HI_NULL;
    HI_U32 ulCount = 0;
    HI_U32 ulRet = HI_ERR_SUCCESS;

    HI_ASSERT(HI_NULL != pstCcoCtx);
    MRS_StopTimer(MRS_CCO_TIMER_NOTIFY_STA_LOCK_NETWORK);

    if (0 == (pstCcoCtx->status & MRS_CCO_STATUS_SEARCH_METER_MASK))
    {
        HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_047, HI_DIAG_MT("[fail] mrsCcoSearchMeterNotifyStaBindNetwork: [status]"), pstCcoCtx->status);
        return HI_ERR_SUCCESS;
    }

    pstAutoSearch = &(pstCcoCtx->stAutoSearchModel);
    pstSearchCfg = &(pstAutoSearch->stSearchCfg);

    if (mrsSrvIsListEmpty(&(pstAutoSearch->stUnLockStaList)))
    {
        pstAutoSearch->pstCurLockNode = HI_NULL;
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_048, HI_DIAG_MT("[fail] mrsCcoSearchMeterNotifyStaBindNetwork: list empty"));
        return HI_ERR_SUCCESS;
    }

    if (!pstAutoSearch->pstCurLockNode
        || (pstAutoSearch->pstCurLockNode->next == &(pstAutoSearch->stUnLockStaList)))
    {
        pstAutoSearch->pstCurLockNode = &(pstAutoSearch->stUnLockStaList);
    }

    for (pstLink = pstAutoSearch->pstCurLockNode->next;
        pstLink != &(pstAutoSearch->stUnLockStaList) && ulCount <= pstSearchCfg->query_max_way;
        pstLink = pstLink->next)
    {
        pstNode = (METER_LIST_NODE *)pstLink;

        if (pstNode->query_times >= pstSearchCfg->query_force_times + pstSearchCfg->query_except_times)
        {
            continue;
        }

        HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1004, HI_DIAG_MT("mrsCcoNotifyStaBindNetwork [mac]"), pstNode->pstMeterNode->mac, 6);
        HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_049, HI_DIAG_MT("mrsCcoNotifyStaBindNetwork [query_times]"), pstNode->query_times);

        ulRet = mrsCcoNotifyStaBindNetwork(pstCcoCtx, pstNode->pstMeterNode);
        if (HI_ERR_SUCCESS != ulRet)
        {
            HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_051, HI_DIAG_MT("mrsCcoNotifyStaBindNetwork Fail: [ret]"), ulRet);
            break;
        }

        HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_052, HI_DIAG_MT("mrsCcoNotifyStaBindNetwork OK: [ret]"), ulRet);

        pstNode->query_times++;

        ulCount++;
        pstAutoSearch->pstCurLockNode = pstLink;
    }

    if (ulCount == 0)
    {
        pstAutoSearch->pstCurLockNode = &(pstAutoSearch->stUnLockStaList);
    }

    MRS_StartTimer(MRS_CCO_TIMER_NOTIFY_STA_LOCK_NETWORK, MRS_SEC_TO_MS(pstSearchCfg->ucBindCmdSendInterval), HI_SYS_TIMER_PERIODIC);

    return ulRet;
}


/**
* @brief 向STA发送锁定命令
*/
HI_U32 mrsCcoNotifyStaBindNetwork(MRS_CCO_SRV_CTX_STRU *pstCcoCtx, METER_NODE * pstNode)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    MRS_PLC_FRAME_DATA_STRU stPlcFrame;
    MRS_PLC_METERLIST_DL_S * pstLockFrame = HI_NULL;
    MRS_SEARCH_METER_CFG_S * pstSearchCfg = HI_NULL;
    HI_U8 * aucPayload = HI_NULL;
    HI_U16 usPayloadLen = sizeof(MRS_PLC_METERLIST_DL_S);
    HI_U8 aucIp[HI_IPV4_ADDR_LEN];

    HI_ASSERT(HI_NULL != pstCcoCtx);

    if (HI_NULL == pstNode)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    aucPayload = mrsToolsMalloc(usPayloadLen);
    if (HI_NULL == aucPayload)
    {
        return HI_ERR_MALLOC_FAILUE;
    }

    pstSearchCfg = &(pstCcoCtx->stAutoSearchModel.stSearchCfg);

    (hi_void)memset_s(aucPayload, usPayloadLen, 0, usPayloadLen);
    pstLockFrame = (MRS_PLC_METERLIST_DL_S *)aucPayload;
    pstLockFrame->interface_ver = MRS_GET_METERLIST_DL_VER;
    pstLockFrame->stru_len = usPayloadLen;
    pstLockFrame->option = MRS_SM_BIND_NETWORK;
    pstLockFrame->seq = pstSearchCfg->seq;
    pstLockFrame->force_resp = 1;

    (HI_VOID)HI_MDM_NM_GetAddr(pstLockFrame->src, aucIp);
   (hi_void)memcpy_s(pstLockFrame->dst, HI_PLC_MAC_ADDR_LEN, pstNode->mac, HI_PLC_MAC_ADDR_LEN);

    (hi_void)memset_s(&stPlcFrame, sizeof(MRS_PLC_FRAME_DATA_STRU), 0, sizeof(MRS_PLC_FRAME_DATA_STRU));
    stPlcFrame.usId = ID_MRS_CMD_GET_METERLIST;
    stPlcFrame.pucPayload = aucPayload;
    stPlcFrame.usPayloadLen = usPayloadLen;
   (hi_void)memcpy_s(stPlcFrame.ucMac, HI_PLC_MAC_ADDR_LEN, pstNode->mac, HI_PLC_MAC_ADDR_LEN);

    ret = MRS_SendPlcFrame(&stPlcFrame);
    HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1005, HI_DIAG_MT("发送锁定报文: [mac]"), stPlcFrame.ucMac, HI_PLC_MAC_ADDR_LEN);
    mrsToolsFree(aucPayload);

    return ret;
}


/**
* @brief CCO处理锁定命令回复
*/
HI_U32 mrsCcoHandleBindNetworkReply(MRS_CCO_SRV_CTX_STRU *pstCcoCtx, HI_U8 * aucMac)
{
    MRS_METER_LIST_NODE_GET_STRU pstQueryRe = {{0}, 0, HI_NULL};
    METER_LIST_NODE * pstMeterListNode = HI_NULL;

    HI_ASSERT(HI_NULL != pstCcoCtx);

    if (HI_NULL == aucMac)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1006, HI_DIAG_MT("处理锁定回复: [mac]"), aucMac, HI_PLC_MAC_ADDR_LEN);

   (hi_void)memcpy_s(pstQueryRe.aucMac, HI_PLC_MAC_ADDR_LEN, aucMac, HI_PLC_MAC_ADDR_LEN);
    pstQueryRe.link = HI_NULL;

    mrsSrvTraverseList(&(pstCcoCtx->stAutoSearchModel.stUnLockStaList), mrsCcoSearchMeterGetMeterListNode, &pstQueryRe);

    if (pstQueryRe.link)
    {
        if (pstQueryRe.link == pstCcoCtx->stAutoSearchModel.pstCurLockNode)
        {
            pstCcoCtx->stAutoSearchModel.pstCurLockNode = pstQueryRe.link->next;
        }

        pstMeterListNode = (METER_LIST_NODE *)pstQueryRe.link;
        mrsSrvListRemove(&(pstCcoCtx->stAutoSearchModel.stUnLockStaList), pstQueryRe.link);

        if (mrsCcoSmReportIsWaitBindMsg())
        {
            mrsSrvEnQueue(&(pstCcoCtx->stAutoSearchModel.stMeterQueue), (MRS_SRV_NODE *)pstMeterListNode->pstMeterNode);
        }
        else
        {
            mrsToolsFree(pstMeterListNode->pstMeterNode);
        }

        mrsToolsFree(pstMeterListNode);
    }

    return HI_ERR_SUCCESS;
}


HI_VOID mrsCcoSearchMeterFreeMeterListNode(HI_VOID * pstNode, HI_VOID * pstCb)
{
    MRS_NOT_USED(pstCb);

    if (!pstNode)
    {
        return;
    }

    mrsToolsFree(((METER_LIST_NODE *)pstNode)->pstMeterNode);
    mrsToolsFree(pstNode);
}

HI_U32 mrsCcoSearchMeterInsertMeterListNode(MRS_SRV_LIST * pstMeterListHead, METER_NODE * pstMeterNode, HI_U32 buf_len)
{
    METER_LIST_NODE * pstMeterListNode = HI_NULL;

    if (!pstMeterNode)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    pstMeterListNode = mrsToolsMalloc(sizeof(METER_LIST_NODE));
    if (!pstMeterListNode)
    {
        return HI_ERR_MALLOC_FAILUE;
    }

    pstMeterListNode->pstMeterNode = mrsToolsMalloc(buf_len);
    if (!pstMeterListNode->pstMeterNode)
    {
        mrsToolsFree(pstMeterListNode);
        return HI_ERR_MALLOC_FAILUE;
    }

   (hi_void)memcpy_s(pstMeterListNode->pstMeterNode, buf_len, pstMeterNode, buf_len);
    pstMeterListNode->query_times = 0;

    mrsSrvListInsTail(pstMeterListHead, (MRS_SRV_LINK *)pstMeterListNode);

    if (MRS_SM_START_NOTIFY_STA_NUM == mrsSrvListLength(pstMeterListHead))
    {
        mrsCcoSearchMeterNotifyStaBindNetwork(mrsCcoGetContext());
    }

    return HI_ERR_SUCCESS;
}

HI_BOOL mrsCcoSearchMeterFindMeterListNode(HI_VOID * pstMeterListNode, HI_VOID * pstExist)
{
    METER_LIST_NODE * pstNode = (METER_LIST_NODE *)pstMeterListNode;
    MRS_CCO_SEARCH_QUEUE_VISIT_STRU * pstVisit = (MRS_CCO_SEARCH_QUEUE_VISIT_STRU *)pstExist;

    if (!pstMeterListNode || !pstExist || !pstNode->pstMeterNode)
    {
        return HI_TRUE;
    }

    if (mrsToolsMemEq(pstNode->pstMeterNode->mac, pstVisit->aucMac, HI_PLC_MAC_ADDR_LEN))
    {
        pstVisit->bExist = HI_TRUE;
        return HI_TRUE;
    }

    return HI_FALSE;
}


HI_BOOL mrsCcoSearchMeterGetMeterListNode(HI_VOID * pstMeterListNode, HI_VOID * pstResult)
{
    METER_LIST_NODE * pstNode = (METER_LIST_NODE *)pstMeterListNode;
    MRS_METER_LIST_NODE_GET_STRU * pstRe = (MRS_METER_LIST_NODE_GET_STRU *)pstResult;

    if (!pstNode || !pstRe || !pstNode->pstMeterNode)
    {
        return HI_TRUE;
    }

    if (mrsToolsMemEq(pstNode->pstMeterNode->mac, pstRe->aucMac, HI_PLC_MAC_ADDR_LEN))
    {
        pstRe->link = (MRS_SRV_LINK *)pstMeterListNode;
        return HI_TRUE;
    }

    return HI_FALSE;
}


// 恢复白名单开关到搜表前状态: 直接回复
HI_PRV HI_U32 mrsCcoRecoverWhiteSwitch(MRS_CCO_SRV_CTX_STRU* pstCcoCtx)
{
    if (!pstCcoCtx)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_044, HI_DIAG_MT("恢复白名单开关: [ucRecoverWhiteSwitch] [bWhiteListSwitch]"),
        pstCcoCtx->stAutoSearchModel.stSearchCfg.ucRecoverWhiteSwitch, pstCcoCtx->stAutoSearchModel.bWhiteListSwitch);

    if (pstCcoCtx->stAutoSearchModel.stSearchCfg.ucRecoverWhiteSwitch)
    {
        HI_MDM_SetWhiteListSwitch(pstCcoCtx->stAutoSearchModel.bWhiteListSwitch, HI_FALSE,
            HI_MAC_WHIST_LIST_CHANGE_REASON_APP_STOP_SEARCH);
    }

    return HI_ERR_SUCCESS;
}
#endif // CCO
HI_END_HEADER

