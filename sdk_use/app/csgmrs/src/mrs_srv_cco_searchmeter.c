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
#include "mrs_dfx.h"
#include "hi_mdm.h"
#include "mrs_srv_cco_searchmeter.h"
#include "mrs_srv_cco_queue.h"
#include "mrs_dfx_cco.h"
#include "mrs_srv_cco_searchmeter_n.h"
#include "app_config.h"
#include "mrs_fw_proto_csg.h"
#include "mrs_fw_proto_csg_echo.h"


HI_START_HEADER
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)

HI_PRV HI_MAC_NETWORK_TOPO_S g_stBackupTopo;
#define GetBackupTopo()     &g_stBackupTopo

HI_PRV HI_BOOL mrsIsSearchReportItem(HI_VOID *pvItem);


#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)

HI_U32 mrsCsg_ReportRouterInfs(HI_U8 status)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U16 len = 0;
    HI_PBYTE buf = HI_NULL;
    MRS_SRV_CCO_ITEM *pstCcoItem = HI_NULL;

    MRS_CSG_IND_FRAME_STRU stFrame;
    MRS_CCO_SRV_CTX_STRU* pstCcoCtx = mrsCcoGetContext();

    MRS_NOT_USED(status);

    do
    {
        (hi_void)memset_s(&stFrame, sizeof(stFrame), 0, sizeof(stFrame));
        stFrame.ucAfn = MRS_AFN(MRS_CSG_AFN_REPORT_05H);
        stFrame.ulDI = MRS_CSG_DI_REPORT_SEARCH_END;
        stFrame.ucSeq =  ++(pstCcoCtx->ucSeq);
        stFrame.usAppDataLen = 0;

        ret = mrsCreateCsgFrame(&stFrame, &buf, &len);
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
        pstCcoItem->ucAfn = MRS_AFN(MRS_CSG_AFN_REPORT_05H);
        pstCcoItem->usFn= (HI_U16)(stFrame.ulDI&0xFFFF);
        pstCcoItem->usTimeOut = 200;//单位为ms
        pstCcoItem->ucMaxTry = 0;
        pstCcoItem->usDataLen = len;
        (hi_void)memcpy_s(pstCcoItem->pData, pstCcoItem->usDataLen, buf, len);
        pstCcoItem->MrsCcoRxRespProc = HI_NULL;
        pstCcoItem->MrsCcoRxTotalTimeOutProc = HI_NULL;
        pstCcoItem->MrsCcoRxEveryTimeOutProc = HI_NULL;

        mrsCcoJoinQueue(pstCcoItem);
        mrsActiveCcoQueue();

    }while(HI_FALSE);

    mrsToolsFree(buf);
    return ret;
}


HI_U32 mrsCsgSearchAssembleReportFrame(MRS_CCO_SRV_CTX_STRU * pstCco, METER_NODE* pstNode)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;

    MRS_CSG_IND_FRAME_STRU stFrame = {0};
    HI_U8 * pucData = HI_NULL;
    HI_U16 usDataLen = 0;
    HI_U8 * pucPayload = HI_NULL;
    HI_U16 usPayloadLen = 0;
    HI_U16 usOffset = 0;
    HI_U32 i = 0;
    MRS_CMD_METERLIST_ITEM * pstItem = (MRS_CMD_METERLIST_ITEM *)pstNode->item;

    if (pstCco->stAutoSearchModel.pReportFrame)
    {
        mrsToolsFree(pstCco->stAutoSearchModel.pReportFrame);
    }

    pstCco->stAutoSearchModel.pReportFrame = HI_NULL;
    pstCco->stAutoSearchModel.usFrameLen = 0;

    usPayloadLen = sizeof(HI_U8) + HI_METER_ADDR_LEN * pstNode->num;
    pucPayload = mrsToolsMalloc(usPayloadLen);
    if (HI_NULL == pucPayload)
    {
        return HI_ERR_NOT_ENOUGH_MEMORY;
    }

    (hi_void)memset_s(pucPayload, usPayloadLen, 0, usPayloadLen);
    pucPayload[usOffset++] = pstNode->num;

    for (i = 0; i < pstNode->num; i++)
    {
        (hi_void)memcpy_s(pucPayload + usOffset, HI_METER_ADDR_LEN, pstItem[i].addr, HI_METER_ADDR_LEN);
        usOffset += HI_METER_ADDR_LEN;
    }

    (hi_void)memset_s(&stFrame, sizeof(stFrame), 0, sizeof(MRS_CSG_IND_FRAME_STRU));
    stFrame.ucAfn = MRS_AFN(MRS_CSG_AFN_REPORT_05H);
    stFrame.ulDI = MRS_CSG_DI_REPORT_NODE_INF;
    stFrame.pAppData = pucPayload;
    stFrame.usAppDataLen = usPayloadLen;

    ulRet = mrsCreateCsgFrame(&stFrame, &pucData, &usDataLen);
    if (HI_ERR_SUCCESS != ulRet)
    {
        mrsToolsFree(pucPayload);
        return ulRet;
    }

    pstCco->stAutoSearchModel.pReportFrame = pucData;
    pstCco->stAutoSearchModel.usFrameLen = usDataLen;

    mrsToolsFree(pucPayload);

    return ulRet;
}
#endif


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

    if (ulSearchMode == MRS_CCO_STATUS_SEARCH_METER)
    {
        status &= ~MRS_CCO_STATUS_NORMAL;
        status |= MRS_CCO_STATUS_SEARCH_METER;
    }
    else
    {
        status |= MRS_CCO_STATUS_SEARCH_METER_TRANSFORMER;
    }

    mrsCcoSetState(status);

    return ret;
}


HI_U32 mrsSearchAssembleReportFrame(MRS_CCO_SRV_CTX_STRU * pstCco, METER_NODE* pstNode)
{
    HI_U32 ret = HI_ERR_SUCCESS;

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
    ret = mrsCsgSearchAssembleReportFrame(pstCco,pstNode);
#else
    ret = mrsCcoSearchAssembleReportFrame(pstCco,pstNode);
#endif

    return ret;
}


#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD) || defined(PRODUCT_CFG_FEATURE_UT)
HI_U32 mrsCcoSearchAssembleReportFrame(MRS_CCO_SRV_CTX_STRU* pstCco, METER_NODE* pstNode)
{
    MRS_CMD_METERLIST_ITEM * item = (MRS_CMD_METERLIST_ITEM *)pstNode->item;
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
    HI_U8 reportAA = pstCco->stAutoSearchModel.stSearchCfg.report_AA_2_master;

    if (pstCco->stAutoSearchModel.pReportFrame)
    {
        mrsToolsFree(pstCco->stAutoSearchModel.pReportFrame);
    }

    pstCco->stAutoSearchModel.pReportFrame = HI_NULL;
    pstCco->stAutoSearchModel.usFrameLen = 0;

    payload_len = SIZEOF_REPORT_FRAME_STRU;
    if (pstNode->type == MRS_DEVICE_TYPE_COLLECTOR)
    {
        payload_len += ucInfoLen * pstNode->num;
    }

    payload = mrsToolsMalloc(payload_len);
    if (HI_NULL == payload)
    {
        return HI_ERR_NOT_ENOUGH_MEMORY;
    }

    (hi_void)memset_s(payload, payload_len, 0, payload_len);
    payload[0] = MRS_SEARCH_REPORT_NODE_NUM;
    offset += 1;

    if (MRS_DEVICE_TYPE_METER == pstNode->type)
    {
            (hi_void)memcpy_s(&payload[offset], payload_len - offset, item->addr, HI_METER_ADDR_LEN);
        offset += HI_METER_ADDR_LEN;
        payload[offset++] = item->protocol;
    }
    else
    {
        if (mrsToolsCheckBCD(pstNode->asset, HI_METER_ADDR_LEN))
        {
            (hi_void)memcpy_s(&payload[offset], payload_len - offset, pstNode->asset, HI_METER_ADDR_LEN);
        }
        else
        {
            (hi_void)memcpy_s(&payload[offset], payload_len - offset, pstNode->id, HI_METER_ADDR_LEN);
        }

        offset += HI_METER_ADDR_LEN;
        payload[offset++] = MRS_COLLECTOR_DEFAULT_PROTOCOL;
    }

    usNum = pstCco->stAutoSearchModel.usCurrNum + 1;
    (hi_void)memcpy_s(&payload[offset], payload_len - offset, &usNum, 2);
    offset += sizeof(usNum);
    payload[offset++] = pstNode->type;

    if (MRS_DEVICE_TYPE_COLLECTOR == pstNode->type)
    {
        payload[offset++] = pstNode->num;
        payload[offset++] = pstNode->num;
        for (i = 0; i < pstNode->num; i++)
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
    }

    (hi_void)memset_s(&afn_data, sizeof(afn_data), 0, sizeof(afn_data));
    afn_data.ucAFN = MRS_AFN(0x06);
    afn_data.ucDT[0] = MRS_AFN_FN_LO(4);
    afn_data.ucDT[1] = MRS_AFN_FN_HI(4);
    afn_data.pData = payload;
    afn_data.usDataLen = payload_len;
    ret = mrsCreate3762UpFrame(&afn_data, &buf, &buf_len, HI_NULL);
    if (HI_ERR_SUCCESS != ret)
    {
        mrsToolsFree(payload);
        return ret;
    }

    pstCco->stAutoSearchModel.pReportFrame = buf;
    pstCco->stAutoSearchModel.usFrameLen = buf_len;

    mrsToolsFree(payload);
    return HI_ERR_SUCCESS;
}
#endif


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

#if (defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) && defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD))
    pstCcoItem->ucAfn = MRS_AFN(MRS_CSG_AFN_REPORT_05H);
    pstCcoItem->usFn = (HI_U16)(MRS_CSG_DI_REPORT_NODE_INF & 0xFFFF);
#else
    pstCcoItem->ucAfn = 0x06;
    pstCcoItem->usFn = 0x04;
#endif

    pstCcoItem->usTimeOut = MRS_SRV_CCO_TIMER_SEARCH_REPORT_VALUE;//单位为ms
    pstCcoItem->ucMaxTry = PRODUCT_CFG_MRS_MAX_MUSTER_COMM_RENUM;
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

    if ((node->num == 0)
#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
     &&(MRS_DEVICE_TYPE_METER == node->type)
#endif
        )
    {
        mrsSrvDeQueue(&(cco->stAutoSearchModel.stMeterQueue));
        mrsToolsFree(node);
        MRS_StartTimer(MRS_CCO_TIMER_SEARCH_REPORT, SCAN_MAP_CYCLE_TIMER_VAL, HI_SYS_TIMER_ONESHOT);
        return;
    }

    /* BEGIN: Modified by cuichunyan/00276962, 2015/12/12  问题单号:*/
    mrsSearchAssembleReportFrame(cco, node);
    /* END:   Modified by cuichunyan/00276962, 2015/12/12*/

    mrsCcoSearchReportFrameEnQueue();

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


HI_BOOL mrsIsSearchReportItem(HI_VOID *pvItem)
{
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
    return mrsCcoQueueMatchItem(pvItem, MRS_CSG_AFN_REPORT_05H, (HI_U16)MRS_CSG_DI_REPORT_NODE_INF);
#else
    return mrsCcoQueueMatchItem(pvItem, 0x06, 0x04);
#endif
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
    }

    if ((status & MRS_CCO_STATUS_SEARCH_METER_MASK) == MRS_CCO_STATUS_SEARCH_METER)
    {
        MRS_StopTimer(MRS_CCO_TIMER_SEARCH_REPORT);   // 关闭开始搜表通讯定时器停止(ID3)
        MRS_StopTimer(MRS_CCO_TIMER_TOPO_QUERY);
        MRS_StopTimer(MRS_CCO_TIMER_GET_METERLIST);

        if (pstCcoCtx->stAutoSearchModel.ucWhiteListDelayTime > 0)
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

    mrsCcoQueueDeleteItemDefault(mrsIsSearchReportItem);

    status &= ~MRS_CCO_STATUS_SEARCH_METER;
    mrsCcoSetState(status);

    return ret;
}


// AFN06 F3 主动上报路由工况变动信息
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

        ret = mrsCreate3762UpFrame(&stAfnData, &buf, &len, HI_NULL);
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


// 路由变动时，主动上报状态
HI_U32 mrsReportRouterInfs(HI_U8 status)
{
    HI_U32 ret = HI_ERR_SUCCESS;

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
    ret = mrsCsg_ReportRouterInfs(status);
#else
    ret = mrs3762_ReportAFN06_F3(status);
#endif

    return ret;
}


HI_VOID mrsSearchMeterPrepare(MRS_CCO_SRV_CTX_STRU* pstCcoCtx)
{
    mrsCcoSearchMeterInit(pstCcoCtx, MRS_CCO_STATUS_SEARCH_METER);

    if (0 == pstCcoCtx->usSearchDuration)
    {
        pstCcoCtx->usSearchDuration = pstCcoCtx->stAutoSearchModel.stSearchCfg.usSMDurationDefault;
        HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_002, HI_DIAG_MT("[CCO-SM]-搜表持续时间为0,设置为默认时间N分钟"), pstCcoCtx->usSearchDuration);
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


HI_U32 mrsSearchMeterProc(MRS_CCO_SRV_CTX_STRU* pstCcoCtx)
{
    // 开启搜表定时器
    MRS_StartTimer(MRS_CCO_TIMER_SEARCH_REPORT, MRS_SRV_CCO_TIMER_SEARCH_ACTIV_VALUE * 10, HI_SYS_TIMER_ONESHOT);

    return mrsCcoStartSearchMeter(pstCcoCtx);
}


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

    mrsSrvEmptyQueue(&(search->stMeterQueue), mrsCcoQueueFree);
    mrsSrvInitQueue(&(search->stMeterQueue));

    return HI_ERR_SUCCESS;
}


#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
HI_U32 mrsCcoCreateStartSearchMeterPlcFrame(MRS_CCO_SRV_CTX_STRU * cco)
{
    MRS_PLC_FRAME_DATA_STRU *plc = HI_NULL;
    MRS_PLC_START_SEARCH_METER_S *start = HI_NULL;
    MR_AUTO_SEARCH_STRU * search = HI_NULL;
	NV_APP_SEARCHMETER_CFG_STRU nv_cfg;

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

    (hi_void)memset_s(plc, sizeof(MRS_PLC_FRAME_DATA_STRU), 0, sizeof(MRS_PLC_FRAME_DATA_STRU));
    plc->usId = ID_MRS_CMD_START_SEARCH_METER;
    plc->pucPayload = payload;
    plc->usPayloadLen = payload_len;
    (hi_void)memset_s(plc->ucMac, HI_PLC_MAC_ADDR_LEN, 0xFF, HI_PLC_MAC_ADDR_LEN);

    search->stSearchFrame.plc = plc;

    (hi_void)memset_s(&nv_cfg, sizeof(nv_cfg), 0, sizeof(nv_cfg));
    if (HI_ERR_SUCCESS == HI_MDM_NV_Read(ID_NV_APP_SEARCHMETER_CFG, &nv_cfg, (HI_U16)sizeof(nv_cfg)))
    {
        nv_cfg.seq = search->stSearchCfg.seq;
        (HI_VOID)HI_MDM_NV_Write(ID_NV_APP_SEARCHMETER_CFG, &nv_cfg, (HI_U16)sizeof(nv_cfg));
    }

    return HI_ERR_SUCCESS;
}
#endif


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

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
        MRS_StartTimer(MRS_CCO_TIMER_GET_METERLIST, cco->stAutoSearchModel.stSearchCfg.query_interval * 1000, HI_SYS_TIMER_ONESHOT);
        MRS_StopTimer(MRS_CCO_TIMER_SEND_BIND_CMD);
        if (SOFT_TF_ON_WITH_STA_JOIN_BY_TF_RESULT == cco->ucSoftTfMode && (cco->status & MRS_CCO_STATUS_SEARCH_METER) > 0)
        {
            MRS_StartTimer(MRS_CCO_TIMER_SEND_BIND_CMD, cco->stAutoSearchModel.stSearchCfg.ucBindCmdSendInterVal * 1000, HI_SYS_TIMER_ONESHOT);
        }
#else
        ret = mrsCcoCreateStartSearchMeterPlcFrame(cco);
        if (HI_ERR_SUCCESS != ret)
        {
            HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_011, HI_DIAG_MT("[Failed] mrsCcoCreateStartSearchMeterPlcFrame"));
            break;
        }

        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_012, HI_DIAG_MT("Send start Search Plc Frame"));
        ret = mrsCcoSendSearchMeterPlcFrame(cco);
#endif
    } while (0);

    if (HI_ERR_SUCCESS != ret)
    {
        mrsStopSearchMeter();
        mrsStopSearchMeterTransformer();
        mrsCcoSearchMeterReset(cco);
    }
    else
    {
        mrsMapAgingPause();
        mrsMapTtlZero(&(cco->stMapModule));

        mrsCcoSearchFastGetMeterList(cco, HI_FALSE);
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


HI_U32 mrsCcoGetMeterList(MRS_CCO_SRV_CTX_STRU *pstCcoCtx, MAC_ITEM *pstItem)
{
    MRS_PLC_FRAME_DATA_STRU stPlcData = {0};
    HI_U8 *pucPayload = HI_NULL;
    HI_U16 usPayloadLen = 0;
    HI_U32 ulRet = HI_ERR_SUCCESS;

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
    MRS_CSG_PLC_FRAME_HEAD *pstFrameHead = HI_NULL;
    usPayloadLen = sizeof(MRS_CSG_PLC_FRAME_HEAD);
#else
    MRS_PLC_METERLIST_DL_S *pstDlData = HI_NULL;
    MR_AUTO_SEARCH_STRU *pstSearchCtx = &(pstCcoCtx->stAutoSearchModel);
    /* BEGIN: PN: DTS2016021906890 MODIFY\ADD\DEL by cuiate/00233580 at 2016/2/20 */
    HI_U8 aucIp[HI_IPV4_ADDR_LEN];
    /* END:   PN: DTS2016021906890 MODIFY\ADD\DEL by cuiate/00233580 at 2016/2/20 */

    usPayloadLen = sizeof(MRS_PLC_METERLIST_DL_S);
#endif

    if (!pstItem)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    pucPayload = (HI_U8 *)mrsToolsMalloc(usPayloadLen);
    if (!pucPayload)
    {
        return HI_ERR_MALLOC_FAILUE;
    }

    (hi_void)memset_s(&stPlcData, sizeof(stPlcData), 0, sizeof(stPlcData));
    (hi_void)memset_s(pucPayload, usPayloadLen, 0, usPayloadLen);

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
    pstFrameHead = (MRS_CSG_PLC_FRAME_HEAD *)pucPayload;
    pstFrameHead->usFrameType = MRS_CSG_PLC_CTRL_TYPE_CMD;
    pstFrameHead->usExtendBit = MRS_CSG_PLC_CTRL_EXTEND_BIT_N;
    pstFrameHead->usRespBit = MRS_CSG_PLC_CTRL_RESP_BIT_Y;
    pstFrameHead->usPrm = MRS_CSG_PLC_CTRL_PRM_HOST;
    pstFrameHead->usDir = MRS_CSG_PLC_CTRL_DIR_DL;
    pstFrameHead->ucSrvId = MRS_CSG_PLC_SRV_ID_CMD_SM_RESULT;
    pstFrameHead->ucVersion = MRS_CSG_PLC_PROTO_VERSION;
    pstFrameHead->usSeq = pstCcoCtx->usPlcSeq++;
    pstFrameHead->usDataLength = 0;
    stPlcData.usId = ID_MRS_CMD_CSG_DATA_TRANSMIT_PLC;
#else
    pstDlData->interface_ver = MRS_GET_METERLIST_DL_VER;
    pstDlData->stru_len = sizeof(MRS_PLC_METERLIST_DL_S);
    pstDlData->option = MRS_SM_SEARCH_RESULT;
    pstDlData->seq = pstSearchCtx->stSearchCfg.seq;
    if (pstItem->query >= pstSearchCtx->stSearchCfg.query_force_times)
    {
        pstDlData->force_resp = 1;
    }

    (HI_VOID)HI_MDM_NM_GetAddr(pstDlData->src, aucIp);
    (hi_void)memcpy_s(pstDlData->dst, HI_PLC_MAC_ADDR_LEN, pstItem->mac, HI_PLC_MAC_ADDR_LEN);

    stPlcData.usId = ID_MRS_CMD_GET_METERLIST;
#endif

    stPlcData.pucPayload = pucPayload;
    stPlcData.usPayloadLen = usPayloadLen;
    (hi_void)memcpy_s(stPlcData.ucMac, HI_PLC_MAC_ADDR_LEN, pstItem->mac, HI_PLC_MAC_ADDR_LEN);

    ulRet = MRS_SendPlcFrame(&stPlcData);
    mrsToolsFree(pucPayload);

    return ulRet;
}


HI_U32 mrsCmdCcoGetMeterList(HI_U16 id, HI_PVOID req_packet, HI_U16 req_packet_size)
{
    MRS_CCO_SRV_CTX_STRU * cco = mrsCcoGetContext();
    MRS_PLC_FRAME_DATA_S * plc = (MRS_PLC_FRAME_DATA_S *)req_packet;
    MRS_PLC_METERLIST_UP_S * up_frame = HI_NULL;
    MRS_CMD_METERLIST_ITEM * inf = HI_NULL;
    METER_NODE * node = HI_NULL;
    HI_U16 i;
    HI_U8 * buf = HI_NULL;
    HI_U16 buf_len = 0;
    HI_U32 ret = HI_ERR_SUCCESS;

    MRS_NOT_USED(id);
    MRS_NOT_USED(req_packet_size);

    HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_016, HI_DIAG_MT("[CCO-SM]-PLC Get Meter list"));

    if ((HI_NULL == plc) || (HI_NULL == plc->pucPayload))
    {
        return HI_ERR_FAILURE;
    }

    HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1000, HI_DIAG_MT("=>[CCO-SM]-PLC plc"), plc, sizeof(MRS_PLC_FRAME_DATA_S));
    HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1001, HI_DIAG_MT("=>[CCO-SM]-PLC payload"), plc->pucPayload, plc->usPayloadLen);

    up_frame = (MRS_PLC_METERLIST_UP_S *)plc->pucPayload;

    if (plc->usPayloadLen < (up_frame->stru_len + sizeof(MRS_CMD_METERLIST_ITEM) * up_frame->meter_num))
    {
        return HI_ERR_FAILURE;
    }

    /* BEGIN: PN: DTS2016021906890 MODIFY\ADD\DEL by cuiate/00233580 at 2016/2/20 */
    if ((up_frame->interface_ver >= MRS_GET_METERLIST_UP_VER_R2) && !mrsSrvVerifyMac(up_frame->dst))
    {
        mrsDfxSmFrameInvalid();
        return HI_ERR_FAILURE;
    }

    mrsDfxSmFrameValid();
    /* END:   PN: DTS2016021906890 MODIFY\ADD\DEL by cuiate/00233580 at 2016/2/20 */

    if ((up_frame->interface_ver >= MRS_GET_METERLIST_UP_VER_R1) && (up_frame->abnormal > 0))
    {
        mrsCcoDfxUpdateAbnormalCltiiNodeList(up_frame);
    }

    // 若搜表未完成，则不上报
    if (up_frame->status)
    {
        mrsCcoUpdateSearchQueryTimes(cco, plc->ucMac);
        return HI_ERR_SUCCESS;
    }

    if (up_frame->meter_num > PRODUCT_CFG_MRS_MAX_METER_IN_MAC_NUM)
    {
        return HI_ERR_FAILURE;
    }
    ret = mrsCcoUpdateSearchResult(cco, plc->ucMac);
    if (HI_ERR_SUCCESS != ret)
    {
        return HI_ERR_FAILURE;
    }

    buf_len = sizeof(METER_NODE);
    buf_len += sizeof(MRS_CMD_METERLIST_ITEM) * up_frame->meter_num;
    buf = mrsToolsMalloc(buf_len);
    if (!buf)
    {
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
    node->type = (up_frame->type == MRS_STA_TYPE_METER) ? MRS_DEVICE_TYPE_METER : MRS_DEVICE_TYPE_COLLECTOR;

    HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1002, HI_DIAG_MT("[CCO-GM] Query Result: Mac"), node->mac, HI_PLC_MAC_ADDR_LEN);

    // 添加到映射表
    mrsMapDeleteMac(&cco->stMapModule, node->mac);
    mrsMapMarkMac(&cco->stMapModule, node->mac, MRS_SEARCH_STATUS_IDLE);

    if (MRS_DEVICE_TYPE_COLLECTOR == node->type)
    {
        HI_U8 ucType = MRS_MAP_DEV_TYPE_CLT_II;

        if (HI_ERR_SUCCESS == mrsGetCltNodeType(node->mac, &ucType))
        {
            MRS_DEVICE_INF_STRU stCltInfo;

            (hi_void)memset_s(&stCltInfo, sizeof(stCltInfo), 0, sizeof(stCltInfo));
            (hi_void)memcpy_s(stCltInfo.aucAddr, HI_METER_ADDR_LEN, node->asset, HI_METER_ADDR_LEN);
            stCltInfo.ucType = ucType;
            mrsMapAddCollector(&cco->stMapModule, node->mac, &stCltInfo);
        }
    }

    inf = (MRS_CMD_METERLIST_ITEM *)node->item;
    for (i = 0; i < (HI_U16)node->num; i++)
    {
        MRS_DEVICE_INF_STRU stMeterInfo;

        (hi_void)memset_s(&stMeterInfo, sizeof(stMeterInfo), 0, sizeof(stMeterInfo));
        mrsMeterAddrPreProc(stMeterInfo.aucAddr, inf->addr, inf->protocol);
        stMeterInfo.ucProtocol = inf->protocol;
        stMeterInfo.ucType = MRS_MAP_DEV_TYPE_METER;
        mrsMapAddMeter(&cco->stMapModule, node->mac, &stMeterInfo);

        inf++;
    }

    ret = HI_ERR_EXIST;
    if (!mrsCcoSearchMacRepeatCheck(cco, node->mac))
    {
        mrsSrvEnQueue(&(cco->stAutoSearchModel.stMeterQueue), (MRS_SRV_NODE *)node);
    }
	else
	{
        mrsToolsFree(node);
	}

    return ret;
    /* END:   PN: DTS2015111905511 MODIFY\ADD\DEL by cuiate/00233580 at 2015/11/23 */
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
            pMacList->mac[pMacList->num].result = MRS_CCO_SM_RESULT_QUERYING;
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
    HI_ASSERT(HI_NULL != cco);

    if (cco->stAutoSearchModel.usStableTimes < cco->stAutoSearchModel.stSearchCfg.topo_stable_times)
    {
        return HI_ERR_FAILURE;
    }

    mrsCcoCheckQueryComplete(cco);

    if (cco->stAutoSearchModel.bQueryComplete)
    {
        METER_NODE * node = (METER_NODE *)mrsSrvQueueTop(&(cco->stAutoSearchModel.stMeterQueue));
        if (!node)
        {
            return HI_ERR_SUCCESS;
        }
    }

    return HI_ERR_FAILURE;
}


/* BEGIN: PN: DTS2015061008265 MODIFY\ADD\DEL by cuiate/00233580 at 2015/6/26 */
HI_BOOL mrsCcoSearchMacRepeatCheck(MRS_CCO_SRV_CTX_STRU *pCcoCtx, HI_U8 *pMacAddr)
{
    /* BEGIN: PN: DTS2015111905511 MODIFY\ADD\DEL by cuiate/00233580 at 2015/11/23 */
    MRS_CCO_SEARCH_QUEUE_VISIT_STRU stVisit;
    MRS_SRV_LIST* pList = HI_NULL;

    (hi_void)memset_s(&stVisit, sizeof(stVisit), 0, sizeof(stVisit));
    (hi_void)memcpy_s(stVisit.aucMac, HI_PLC_MAC_ADDR_LEN, pMacAddr, HI_PLC_MAC_ADDR_LEN);

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


HI_VOID mrsCcoSearchFastMeterEnQueue(MRS_CCO_SRV_CTX_STRU *pCcoCtx, HI_U8 *pMacAddr)
{
    METER_NODE *pNode = HI_NULL;
    MRS_CMD_METERLIST_ITEM *pMeterItem = HI_NULL;
    HI_U8 *pBuffer = HI_NULL;
    HI_U32 ulBufLen = sizeof(METER_NODE) + sizeof(MRS_CMD_METERLIST_ITEM);

    pBuffer = mrsToolsMalloc(ulBufLen);
    if (!pBuffer)
    {
        return;
    }

    (hi_void)memset_s(pBuffer, ulBufLen, 0, ulBufLen);
    pNode = (METER_NODE *)pBuffer;
    pMeterItem = (MRS_CMD_METERLIST_ITEM *)(pBuffer + sizeof(METER_NODE));

    (hi_void)memcpy_s(pMeterItem->addr, HI_PLC_MAC_ADDR_LEN, pMacAddr, HI_PLC_MAC_ADDR_LEN);
    MRS_TOOLS_FE_TO_00(pMeterItem->addr[MRS_MAC_ADDR_INDEX_5]);
    mrsHexInvert(pMeterItem->addr, HI_METER_ADDR_LEN);
    pMeterItem->type = MRS_STA_TYPE_METER;
    pMeterItem->protocol = MRS_PROTO645_VER_2007;

    (hi_void)memcpy_s(pNode->mac, HI_PLC_MAC_ADDR_LEN, pMacAddr, HI_PLC_MAC_ADDR_LEN);
    (hi_void)memcpy_s(pNode->asset, HI_METER_ADDR_LEN, pMeterItem->addr, HI_METER_ADDR_LEN);
    (hi_void)memcpy_s(pNode->id, HI_PLC_MAC_ADDR_LEN, pMeterItem->addr, HI_METER_ADDR_LEN);
    pNode->num = 1;
    pNode->type = MRS_DEVICE_TYPE_METER;

    /* BEGIN: PN: DTS2015111905511 MODIFY\ADD\DEL by cuiate/00233580 at 2015/11/23 */
    mrsSrvEnQueue(&(pCcoCtx->stAutoSearchModel.stMeterQueue), (MRS_SRV_NODE*)pNode);

    /* END:   PN: DTS2015111905511 MODIFY\ADD\DEL by cuiate/00233580 at 2015/11/23 */
}


HI_U32 mrsCcoSearchFastGetMeterList(MRS_CCO_SRV_CTX_STRU *pCcoCtx, HI_BOOL bRepeatCheck)
{
    MRS_SEARCH_METER_MAC_LIST *pMacList = &(pCcoCtx->stAutoSearchModel.stMacList);
    HI_U16 i = 0;

    for (i = 0; i < pMacList->num; i++)
    {
        MAC_ITEM *pItem = (MAC_ITEM *)(pMacList->mac + i);

        if ((!pItem->valid) || (pItem->result != MRS_CCO_SM_RESULT_QUERYING))
        {
            continue;
        }

        /* BEGIN: PN: DTS2015111905511 MODIFY\ADD\DEL by cuiate/00233580 at 2015/11/23 */
        // 非载波表模块 非三相表模块
        if ((HI_ND_TYPE_STA != pItem->type) && (HI_ND_TYPE_THREE_STA != pItem->type))
        {
            continue;
        }

        pItem->result = MRS_CCO_SM_RESULT_QUERY_SUC;
        pItem->query = 0;
        if (bRepeatCheck && mrsCcoSearchMacRepeatCheck(pCcoCtx, pItem->mac))
        {
            continue;
        }
        /* END:   PN: DTS2015111905511 MODIFY\ADD\DEL by cuiate/00233580 at 2015/11/23 */

        if (mrsToolsIsMacAddrFromMeter(pItem->mac))
        {
            mrsCcoSearchFastMeterEnQueue(pCcoCtx, pItem->mac);
        }
    }

    return HI_ERR_SUCCESS;
}
/* END:   PN: DTS2015061008265 MODIFY\ADD\DEL by cuiate/00233580 at 2015/6/26 */


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

            if ((mac_list->mac[i].result != MRS_CCO_SM_RESULT_QUERYING)
                || (mac_list->mac[i].query >= cfg->query_force_times + cfg->query_except_times))
            {
                continue;
            }

            ret = mrsCcoGetMeterList(cco, &(mac_list->mac[i]));
            if (HI_ERR_SUCCESS != ret)
            {
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
            mrsReportRouterInfs(MRS_CCO_SEARCH_METER_TASK_END);
            HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_026, HI_DIAG_MT("上报路由工况"));
            mrsCcoSearchMeterSaveFinishReason(cco, (HI_U8)MRS_CCO_SEARCH_FINISH_NORMAL);
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
    /* BEGIN: PN: DTS2015061008265 MODIFY\ADD\DEL by cuiate/00233580 at 2015/6/26 */
    mrsCcoSearchFastGetMeterList(cco, HI_TRUE);
    /* END:   PN: DTS2015061008265 MODIFY\ADD\DEL by cuiate/00233580 at 2015/6/26 */

    if (cco->stAutoSearchModel.bQueryComplete)
    {
        cco->stAutoSearchModel.bQueryComplete = HI_FALSE;
        mrsCcoSearchMeterGetMeterlist(cco);
    }

    return ret;
}


HI_U32 mrsCcoSendBindCmd(MRS_CCO_SRV_CTX_STRU *pstCcoCtx, MAC_ITEM *pstItem)
{
    MRS_PLC_FRAME_DATA_STRU stPlcData = {0};
    MRS_CSG_PLC_FRAME_HEAD *pstFrameHead = HI_NULL;
    HI_U8 *pucPayload = HI_NULL;
    HI_U16 usPayloadLen = sizeof(MRS_CSG_PLC_FRAME_HEAD);
    HI_U32 ulRet = HI_ERR_SUCCESS;

    if (!pstItem)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    pucPayload = (HI_U8 *)mrsToolsMalloc(usPayloadLen);
    if (!pucPayload)
    {
        return HI_ERR_MALLOC_FAILUE;
    }

    (hi_void)memset_s(pucPayload, usPayloadLen, 0, usPayloadLen);
    pstFrameHead = (MRS_CSG_PLC_FRAME_HEAD *)pucPayload;
    pstFrameHead->usFrameType = MRS_CSG_PLC_CTRL_TYPE_CMD;
    pstFrameHead->usExtendBit = MRS_CSG_PLC_CTRL_EXTEND_BIT_N;
    pstFrameHead->usRespBit = MRS_CSG_PLC_CTRL_RESP_BIT_Y;
    pstFrameHead->usPrm = MRS_CSG_PLC_CTRL_PRM_HOST;
    pstFrameHead->usDir = MRS_CSG_PLC_CTRL_DIR_DL;
    pstFrameHead->ucSrvId = MRS_CSG_PLC_SRV_ID_CMD_BIND;
    pstFrameHead->ucVersion = MRS_CSG_PLC_PROTO_VERSION;
    pstFrameHead->usSeq = pstCcoCtx->usPlcSeq++;
    pstFrameHead->usDataLength = 0;
    stPlcData.usId = ID_MRS_CMD_CSG_DATA_TRANSMIT_PLC;

    stPlcData.pucPayload = pucPayload;
    stPlcData.usPayloadLen = usPayloadLen;
    (hi_void)memcpy_s(stPlcData.ucMac, HI_PLC_MAC_ADDR_LEN, pstItem->mac, HI_PLC_MAC_ADDR_LEN);

    ulRet = MRS_SendPlcFrame(&stPlcData);
    mrsToolsFree(pucPayload);

    return ulRet;
}


HI_U32 mrsCcoSearchMeterSendBindCmd(MRS_CCO_SRV_CTX_STRU *cco)
{
    HI_U32 ret = HI_ERR_SUCCESS;

    HI_ASSERT(HI_NULL != cco);

    if (SOFT_TF_ON_WITH_STA_JOIN_BY_TF_RESULT != cco->ucSoftTfMode)
    {
        return HI_ERR_NOT_SUPPORT;
    }

    do
    {
        MRS_SEARCH_METER_MAC_LIST * mac_list = &(cco->stAutoSearchModel.stMacList);
        MRS_SEARCH_METER_CFG_S * cfg = &(cco->stAutoSearchModel.stSearchCfg);
        HI_U16 i, count = 0;

        for (i = mac_list->bind_current; i < mac_list->num && count < cfg->ucBindCmdSendMaxWay; i++)
        {
            HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_027, HI_DIAG_MT("[CCO-SM]-BIND rlt-[%d] query-[%d]"), mac_list->mac[i].result, mac_list->mac[i].query);
            HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1006, HI_DIAG_MT("[CCO-SM]-BIND MAC"), mac_list->mac[i].mac, 6);

            if (MRS_CCO_SM_RESULT_QUERY_SUC != mac_list->mac[i].result || mac_list->mac[i].query >= cfg->ucBindCmdSendMaxTimes)
            {
                continue;
            }

            ret = mrsCcoSendBindCmd(cco, &(mac_list->mac[i]));
            if (HI_ERR_SUCCESS != ret)
            {
                break;
            }

            mac_list->mac[i].query++;
            count++;
        }

        if (HI_TRUE == cco->stAutoSearchModel.bQueryComplete && 0 == count && 0 == mac_list->bind_current)
        {
            ret = HI_ERR_SUCCESS;
            break;
        }

        if (i >= mac_list->num)
        {
            mac_list->bind_current = 0;
            MRS_StartTimer(MRS_CCO_TIMER_SEND_BIND_CMD, 1000 * 60, HI_SYS_TIMER_ONESHOT);
        }
        else
        {
            mac_list->bind_current = i;
            MRS_StartTimer(MRS_CCO_TIMER_SEND_BIND_CMD, cfg->ucBindCmdSendInterVal * 1000, HI_SYS_TIMER_ONESHOT);
        }
    } while (0);

    return ret;
}

HI_U32 mrsCcoSearchMeterSaveFinishReason(MRS_CCO_SRV_CTX_STRU *cco, HI_U8 reason)
{
    HI_U8 finish_reason = reason;

    HI_ASSERT(HI_NULL != cco);

    if (finish_reason != cco->stAutoSearchModel.stSearchCfg.search_finish_reason)
    {
        NV_APP_SEARCHMETER_CFG_STRU nv_cfg;
        HI_U32 ret = HI_ERR_SUCCESS;

        ret = HI_MDM_NV_Read(ID_NV_APP_SEARCHMETER_CFG, &nv_cfg, sizeof(nv_cfg));
        if (HI_ERR_SUCCESS != ret)
        {
            return ret;
        }

        nv_cfg.finish_reason = finish_reason;

        ret = HI_MDM_NV_Write(ID_NV_APP_SEARCHMETER_CFG, &nv_cfg, sizeof(nv_cfg));
        if (HI_ERR_SUCCESS != ret)
        {
            return ret;
        }
    }

    return HI_ERR_SUCCESS;
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
            if (mac_list->mac[i].result != MRS_CCO_SM_RESULT_QUERYING)
            {
                return HI_ERR_EXIST;
            }

            mac_list->mac[i].result = MRS_CCO_SM_RESULT_QUERY_SUC;
            mac_list->mac[i].query = 0;
            return HI_ERR_SUCCESS;
        }
    }

    /* BEGIN: PN: DTS2015061008265 MODIFY\ADD\DEL by cuiate/00233580 at 2015/6/24 */
    return HI_ERR_NOT_FOUND;
    /* END:   PN: DTS2015061008265 MODIFY\ADD\DEL by cuiate/00233580 at 2015/6/24 */
}


HI_VOID mrsCcoUpdateBindResult(MRS_CCO_SRV_CTX_STRU * cco, HI_U8 * mac)
{
    MRS_SEARCH_METER_MAC_LIST * mac_list = HI_NULL;
    HI_U16 i;

    HI_ASSERT(HI_NULL != cco);

    if (!mac)
    {
        return;
    }

    mac_list = &(cco->stAutoSearchModel.stMacList);
    for (i = 0; i < mac_list->num; i++)
    {
        if (mrsToolsMemEq(mac_list->mac[i].mac, mac, HI_PLC_MAC_ADDR_LEN))
        {
            if (MRS_CCO_SM_RESULT_QUERY_SUC == mac_list->mac[i].result)
            {
                mac_list->mac[i].result = MRS_CCO_SM_RESULT_BIND_SUC;
            }

            break;
        }
    }
}


HI_U32 mrsCcoUpdateSearchQueryTimes(MRS_CCO_SRV_CTX_STRU * cco, HI_U8 * mac)
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
            mac_list->mac[i].query = 0;
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
        if ((mac_list->mac[i].result != MRS_CCO_SM_RESULT_QUERYING)
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

    if (ulStatus & MRS_CCO_STATUS_SEARCH_METER && pstCcoCtx->stAutoSearchModel.stSearchCfg.ucCloseWhiteWhenSM)
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

    if (ulStatus & MRS_CCO_STATUS_SEARCH_METER_MASK)
    {
        mrsStopSearchMeter();

        if ((ulStatus & MRS_CCO_STATUS_SEARCH_METER_MASK) == MRS_CCO_STATUS_SEARCH_METER)
        {
            mrsCcoStopSearchMeter(mrsCcoGetContext());
        }

        mrsCcoSearchMeterSaveFinishReason(mrsCcoGetContext(), ucReason);
    }

    return HI_ERR_SUCCESS;
}
/* END:   PN: DTS2015061008265 MODIFY\ADD\DEL by cuiate/00233580 at 2015/6/23 */

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

    return HI_FALSE;
}
/* END:   PN: DTS2015111905511 MODIFY\ADD\DEL by cuiate/00233580 at 2015/11/23 */

// 恢复白名单开关到搜表前状态: 直接恢复
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

