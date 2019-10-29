//*****************************************************************************
//
//                  版权所有 (C), 1998-2011, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_srv_readmeter_manage.c
//  作    者   : liuxipeng/KF54842
//  生成日期   : 2014-02-28
//  功能描述   : 读表管理模块函数实现
//
//  函数列表   :
//  修改历史   :
//  1.日    期 : 2014-02-28
//    作    者 : liuxipeng/KF54842
//    修改内容 : 创建文件
//
//*****************************************************************************

//*****************************************************************************
// PROJECT   :
// SUBSYSTEM :
// MODULE    :
// OWNER     :
//*****************************************************************************

#include "mrs_common.h"
#include "mrs_cmd_msg.h"
#include "mrs_srv_common.h"
#include "hi_mdm.h"
#include "app_nv.h"
#include "mrs_dfx.h"
#include "mrs_fw_n.h"
#include "mrs_fw_log.h"
#include "mrs_fw_proto645.h"
#include "mrs_fw_tools.h"
#include "mrs_srv_cco_macmap.h"
#include "mrs_srv_mrslog.h"
#include "mrs_srv_queue.h"
#include "mrs_srv_list.h"
#include "mrs_srv_rm_n.h"
#include "mrs_fw_nv.h"
#include "mrs_fw_proto376_2_echo.h"
#include "mrs_srv_cco.h"
#include "mrs_srv_cco_detect_rm.h"
#include "mrs_srv_readmeter_config.h"

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)

// 读表管理模块上下文定义
HI_PRV MRS_READMETER_MODULE g_ReadMeterCtx;

// 获取读表模块上下文
HI_PRVL P_MRS_READMETER_MODULE mrsRmGetModuleCtx(HI_VOID)
{
    return &g_ReadMeterCtx;
}

// 通过重传配置号获取重传参数
HI_PRV P_MRS_RETRY_CFG_S mrsRmGetRetryCfg(HI_U8 ucIndex)
{
    return &(mrsRmGetModuleCtx()->stRetryCfg[ucIndex % MRS_RETRY_CFG_MAX_NUM]);
}

// 通过缓存池号获取缓存池指针
HI_PRV P_MRS_RM_REQ_POOL mrsRmGetPool(HI_U8 ucPoolNo)
{
    return &(mrsRmGetModuleCtx()->stPool[ucPoolNo % MRS_RM_POOL_MAX_NUM]);
}

// 通过策略号获取读表策略
HI_PRV P_MRS_RM_STRATEGY_REG mrsRmGetStrategy(HI_U8 ucStrategyNo)
{
    return &(mrsRmGetModuleCtx()->stStrategyReg[ucStrategyNo % MRS_RM_STRATEGY_REG_MAX_NUM]);
}

// 获取PLC帧序号
HI_PRV HI_U32 mrsRmGetPlcSeq(HI_VOID)
{
    return mrsRmGetModuleCtx()->ulPlcSeq;
}

// PLC帧序号自增
HI_PRV HI_VOID mrsRmPlcSeqAddSelf(HI_VOID)
{
    mrsRmGetModuleCtx()->ulPlcSeq++;
    mrsDfxRmPlcSeq(mrsRmGetModuleCtx()->ulPlcSeq);
    return;
}

// 获取读表超时检查定时器工作状态
HI_PRVL HI_U8 mrsRmGetTimerStatus(HI_VOID)
{
    return mrsRmGetModuleCtx()->ucTimerStatus;
}

// 设置读表超时检查定时器工作状态
HI_PRVL HI_VOID mrsRmSetTimerStatus(HI_U8 ucStatus)
{
    mrsRmGetModuleCtx()->ucTimerStatus = ucStatus;
    mrsDfxRmTimerStatus(ucStatus);
    return;
}

// 设置缓冲池工作模式
HI_PRVL HI_VOID mrsRmSetPoolMode(P_MRS_RM_REQ_POOL pstPool, HI_U8 ucMode)
{
    pstPool->ucMode = ucMode;
    mrsDfxRmPoolMode((HI_U32)pstPool, ucMode);
    return;
}


// 重传配置参数有效性检查
HI_PRVL HI_VOID mrsRmRetryCfgValidtyCheck(P_MRS_RETRY_CFG_S pstCfg)
{

    MRS_DATA_VALID_CHECK(pstCfg->PlcLayerDelayNv,
                        MRS_RM_PLC_LAYER_DELAY_MAX,
                        MRS_RM_PLC_LAYER_DELAY_DEFAULT,
                        MRS_RM_PLC_LAYER_DELAY_MIN);

    MRS_DATA_VALID_CHECK(pstCfg->MeterOverTimeNv,
                        MRS_RM_METER_OVERTIME_MAX,
                        MRS_RM_METER_OVERTIME_DEFAULT,
                        MRS_RM_METER_OVERTIME_MIN);

    MRS_DATA_VALID_CHECK(pstCfg->PlcRevisoryNv,
                        MRS_RM_PLC_REVISORY_MAX,
                        MRS_RM_PLC_REVISORY_DEFAULT,
                        MRS_RM_PLC_REVISORY_MIN);

    MRS_DATA_VALID_CHECK(pstCfg->CtrlCmdDelayNv,
                        MRS_RM_CTRL_CMD_DELAY_MAX,
                        MRS_RM_CTRL_CMD_DELAY_DEFAULT,
                        MRS_RM_CTRL_CMD_DELAY_MIN);

    MRS_DATA_VALID_CHECK(pstCfg->CltIDelayNv,
                        MRS_RM_CLTI_DELAY_MAX,
                        MRS_RM_CLTI_DELAY_DEFAULT,
                        MRS_RM_CLTI_DELAY_MIN);

    if (pstCfg->MeterFrameInterval > MRS_RM_METER_FRAME_INTERVAL_MAX)
    {
        pstCfg->MeterFrameInterval = MRS_RM_METER_FRAME_INTERVAL_MAX;
    }

    MRS_DATA_VALID_CHECK(pstCfg->PlcMaxRetryNvS,
                        MRS_RM_PLC_RETRY_CNT_MAX,
                        MRS_RM_PLC_RETRY_CNT_DEFAULT,
                        MRS_RM_PLC_RETRY_CNT_MIN);

    MRS_DATA_VALID_CHECK(pstCfg->PlcMaxRetryNvB,
                        MRS_RM_PLC_BC_RETRY_CNT_MAX,
                        MRS_RM_PLC_BC_RETRY_CNT_DEFAULT,
                        MRS_RM_PLC_BC_RETRY_CNT_MIN);

    MRS_DATA_VALID_CHECK(pstCfg->PlcMaxRetryLoop,
                        MRS_RM_PLC_RETRY_LOOP_MAX,
                        MRS_RM_PLC_RETRY_LOOP_DEFAULT,
                        MRS_RM_PLC_RETRY_LOOP_MIN);

    pstCfg->MeterMaxRetryNv = ((pstCfg->MeterMaxRetryNv > MRS_RM_METER_RETRY_CNT_MAX) ? MRS_PRM_METER_RETRY_CNT_DEFAULT : pstCfg->MeterMaxRetryNv);

    // 如果超时时间为0, 则按照默认值处理
    if (0 == pstCfg->MaxServiceTime)
    {
        pstCfg->MaxServiceTime = MRS_RM_MAX_SERVICE_TIME_DEFAULT;
    }

    return;
}

// 初始化重传控制参数
HI_VOID mrsRmRetryCfgInit(P_MRS_RETRY_CFG_S pstCfg, NV_MRS_RETRY_CFG_S * pstNvCfg, HI_U8 ucRmType)
{
    if(pstNvCfg == HI_NULL)
    {
        pstCfg->PlcLayerDelayNv     = MRS_RM_PLC_LAYER_DELAY_DEFAULT;
        pstCfg->MeterOverTimeNv     = MRS_RM_METER_OVERTIME_DEFAULT;
        pstCfg->PlcRevisoryNv       = MRS_RM_PLC_REVISORY_DEFAULT;
        pstCfg->CtrlCmdDelayNv      = MRS_RM_CTRL_CMD_DELAY_DEFAULT;
        pstCfg->CltIDelayNv         = MRS_RM_CLTI_DELAY_DEFAULT;
        pstCfg->MeterFrameInterval  = MRS_RM_METER_FRAME_INTERVAL_DEFAULT;
        pstCfg->PlcMaxRetryNvS      = MRS_RM_PLC_RETRY_CNT_DEFAULT;
        pstCfg->PlcMaxRetryNvB      = MRS_RM_PLC_BC_RETRY_CNT_DEFAULT;
        pstCfg->MeterMaxRetryNv     = (MRS_RM_PR == ucRmType) ? MRS_PRM_METER_RETRY_CNT_DEFAULT : MRS_RM_METER_RETRY_CNT_DEFAULT;
        pstCfg->MeterRetryCfgNv     = MRS_RM_METER_RETRY_CFG_DEFAULT;
        pstCfg->PlcMaxRetryLoop     = MRS_RM_PLC_RETRY_LOOP_DEFAULT;
        pstCfg->MaxServiceTime      = MRS_RM_MAX_SERVICE_TIME_DEFAULT;
    }
    else
    {
        // 更新重传配置参数
        (hi_void)memcpy_s(pstCfg, sizeof(MRS_RETRY_CFG_S), pstNvCfg, sizeof(MRS_RETRY_CFG_S));

        // 重传配置参数有效性检查
        mrsRmRetryCfgValidtyCheck(pstCfg);
    }

    return;
}


// 初始化并发管理参数
HI_PRV HI_VOID mrsRmParallelCfgInit(P_MRS_PARALLEL_CFG_S pstCfg, P_NV_MRS_PARALLEL_CFG_S pstNvCfg)
{
    if(pstNvCfg == HI_NULL)
    {
        pstCfg->usMaxParallelNum = MRS_PARALLEL_NUM_DEFAULT;
        pstCfg->usMaxFrame = MRS_FRAME_NUM_DEFAULT;
        pstCfg->usMaxTotalItem = MRS_ITEM_NUM_DEFAULT;
    /* BEGIN: Modified by liuxipeng/lKF54842,2014/4/26 问题单号:DTS2014042407354 */
        pstCfg->ucCheckRepeatMode = MRS_RM_ADDR_MATCH;
    /* END  : Modified by liuxipeng/lKF54842,2014/4/26 问题单号:DTS2014042407354 */
    }
    else
    {
        // 更新并发配置参数
        (hi_void)memcpy_s(pstCfg, sizeof(MRS_PARALLEL_CFG_S), pstNvCfg, sizeof(MRS_PARALLEL_CFG_S));

        MRS_DATA_BOUNDARY_CHECK(pstCfg->usMaxParallelNum,
                                MRS_PARALLEL_NUM_MAX,
                                MRS_PARALLEL_NUM_MIN);

        MRS_DATA_BOUNDARY_CHECK(pstCfg->usMaxFrame,
                                MRS_FRAME_NUM_MAX,
                                MRS_FRAME_NUM_MIN);

        MRS_DATA_BOUNDARY_CHECK(pstCfg->usMaxTotalItem,
                                MRS_ITEM_NUM_MAX,
                                MRS_ITEM_NUM_MIN);
    /* BEGIN: Modified by liuxipeng/lKF54842,2014/4/26 问题单号:DTS2014042407354 */
        if((pstCfg->ucCheckRepeatMode != MRS_RM_CRC_MATCH)
            && (pstCfg->ucCheckRepeatMode != MRS_RM_ADDR_MATCH))
        {
            pstCfg->ucCheckRepeatMode = MRS_RM_ADDR_MATCH;
        }
    /* END  : Modified by liuxipeng/lKF54842,2014/4/26 问题单号:DTS2014042407354 */
    }

    return;
}


/* BEGIN: PN: DTS2015033100426 MODIFY\ADD\DEL by cuiate/00233580 at 2015/3/26 */
// 初始化读表重传配置
HI_VOID mrsRmCfgInit(NV_MRS_RETRY_CFG_S *pstParam)
{
    HI_U8 i = 0;

    for(i = 0; i < MRS_RETRY_CFG_MAX_NUM; i++)
    {
        P_MRS_RETRY_CFG_S pstCfg = mrsRmGetRetryCfg(i);

        if (pstParam)
        {
            mrsRmRetryCfgInit(pstCfg, pstParam + i, i);//注意:这里的第三个参数实际是MRS_RM_XR/MRS_RM_LR/MRS_RM_PR,同i这里同值
        }
        else
        {
            mrsRmRetryCfgInit(pstCfg, HI_NULL, i);//注意:这里的第三个参数实际是MRS_RM_XR/MRS_RM_LR/MRS_RM_PR,同i这里同值
        }
    }
}

// 初始化并发控制配置
HI_VOID mrsRmCfgParallelInit(NV_MRS_PARALLEL_CFG_S *pstParam)
{
    HI_S32 i = 0;

    for(i = 0; i < MRS_RM_POOL_MAX_NUM; i++)
    {
        P_MRS_RM_REQ_POOL pstPool = mrsRmGetPool((HI_U8)i);
        P_MRS_PARALLEL_CFG_S pstCfg = &pstPool->stParallelCfg;

        if (pstParam)
        {
            mrsRmParallelCfgInit(pstCfg, pstParam + i);
        }
        else
        {
            mrsRmParallelCfgInit(pstCfg, HI_NULL);
        }
    }
}
/* END:   PN: DTS2015033100426 MODIFY\ADD\DEL by cuiate/00233580 at 2015/3/26 */

// 根据输入的业务读表类型查找对应的策略
HI_PRV HI_U32 mrsRmFindStrategy(HI_IN HI_U16 usType,
                                HI_OUT P_MRS_RM_STRATEGY_REG *ppstStrategy,
                                HI_OUT HI_U8 *pucStrategyNo)
{
    P_MRS_READMETER_MODULE pstRmCtx = mrsRmGetModuleCtx();
    P_MRS_RM_STRATEGY_REG pstStrategy = HI_NULL;
    HI_U8 i = 0;

    pstStrategy = pstRmCtx->stStrategyReg;

    for(i = 0; i < MRS_RM_STRATEGY_REG_MAX_NUM; i++)
    {
        if(pstStrategy->usRmType == usType)
        {
            if(pucStrategyNo)
            {
                *pucStrategyNo = i;
            }

            if(ppstStrategy)
            {
                *ppstStrategy = pstStrategy;
            }

            return HI_ERR_SUCCESS;
        }

        pstStrategy++;
    }

    return HI_ERR_NOT_FOUND;
}


// 增加一个缓存池纵向节点
HI_PRV P_MRS_RM_NODE mrsRmAddNode(P_MRS_RM_REQ_POOL pstPool, HI_U8 *ucMac)
{
    P_MRS_RM_NODE pstNode = HI_NULL;

    pstNode = (P_MRS_RM_NODE)mrsToolsMalloc(sizeof(MRS_RM_NODE));
    if(pstNode != HI_NULL)
    {
        (hi_void)memset_s(pstNode, sizeof(MRS_RM_NODE), 0, sizeof(MRS_RM_NODE));

        HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1000, HI_DIAG_MT("创建节点并加入链表"), ucMac, HI_PLC_MAC_ADDR_LEN);

        (hi_void)memcpy_s(pstNode->ucDestMac, HI_PLC_MAC_ADDR_LEN, ucMac, HI_PLC_MAC_ADDR_LEN);
        mrsSrvInitQueue(&pstNode->stItemQueue);

        pstNode->ucState = MRS_RM_NODE_UNTREATED;

        // 插入缓存池纵向链表尾部
        mrsSrvListInsTail(&pstPool->stNodeList, &pstNode->link);

        return pstNode;
    }
    else
    {
        return HI_NULL;
    }
}


// 增加一个缓冲池横向元素
HI_PRV HI_U32 mrsRmAddItem(P_MRS_RM_NODE pstNode, P_MRS_RM_REQ_DATA pstData,
                                    HI_U8 ucStrategyNo, MRS_645BUF_INF *pst645Inf)
{
    P_MRS_RM_ITEM pstItem = HI_NULL;
    HI_U16 usItemSize = 0;

    if(!pstNode || !pstData)
    {
        return HI_ERR_FAILURE;
    }

    usItemSize = sizeof(MRS_RM_ITEM) + pstData->usDataLen;

    pstItem = (P_MRS_RM_ITEM)mrsToolsMalloc(usItemSize);
    if(pstItem != HI_NULL)
    {
        (hi_void)memset_s(pstItem, usItemSize, 0, usItemSize);

        HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1001, HI_DIAG_MT("在节点队列中加入读表请求"), pstData->ucAddr, HI_METER_ADDR_LEN);

        if (pstData->ucAddr)
        {
            (hi_void)memcpy_s(pstItem->ucMeterAddr, HI_METER_ADDR_LEN, pstData->ucAddr, HI_METER_ADDR_LEN);
        }
        pstItem->ucMrSeq      = pstData->ucMrSeq;
        pstItem->ucStrategyNo = ucStrategyNo;
        pstItem->ucProtocol   = pstData->ucProtocol;
        pstItem->ucFrameNum   = pst645Inf->ucNum;
        pstItem->bCtrlCmd     = pst645Inf->bCtrlCmd;
        pstItem->usDataLen    = pstData->usDataLen;
    /* BEGIN: Modified by liuxipeng/lKF54842,2014/4/26 问题单号:DTS2014042407354 */
        pstItem->ulDataCrc    = pst645Inf->ulCrc;
    /* END  : Modified by liuxipeng/lKF54842,2014/4/26 问题单号:DTS2014042407354 */
        (hi_void)memcpy_s(pstItem->pucData, pstItem->usDataLen, pstData->pucData, pstData->usDataLen);

        mrsSrvEnQueue(&pstNode->stItemQueue, &pstItem->link);

        pstNode->ucItemNum++;

        return HI_ERR_SUCCESS;
    }
    else
    {
        return HI_ERR_NO_MORE_MEMORY;
    }
}

/* BEGIN: Modified by liuxipeng/lKF54842,2014/4/29 问题单号:DTS2014042306517 */
// 读表请求元素出队列(节点处理超时或是数据应答后调用)
HI_PRV HI_U32 mrsRmItemDeQueue(P_MRS_RM_NODE pstNode, HI_BOOL bSwitchFlg)
{
    P_MRS_RM_ITEM pstItem = HI_NULL;

    pstItem = mrsSrvDeQueue(&pstNode->stItemQueue);
    if(pstItem)
    {
        HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1002, HI_DIAG_MT("读表请求出队列"), pstItem->ucMeterAddr, HI_METER_ADDR_LEN);

        pstNode->ucItemNum--;
        mrsToolsFree(pstItem);
    }

    // 复位节点上下文(释放保存的PLC下行数据;清空上下文内容)
    mrsRmNodeCtxReset(pstNode);

    // 初始化节点上下文(如果队列不为空则继续处理下一个读表请求)
    return mrsRmNodeCtxInit(pstNode, bSwitchFlg);
}
/* END  : Modified by liuxipeng/lKF54842,2014/4/29 问题单号:DTS2014042306517 */


// 在缓冲池中查找指定MAC地址对应的节点(未找到返回HI_NULL)
HI_PRV P_MRS_RM_NODE mrsRmFindNode(P_MRS_RM_REQ_POOL pstPool, HI_U8 *ucMac)
{
    MRS_SRV_LINK * it;
    MRS_SRV_LIST * list;
    P_MRS_RM_NODE node = HI_NULL;

    list = &pstPool->stNodeList;

    // 遍历链表
    for (it = list->next; it != list; it = it->next)
    {
        node = (P_MRS_RM_NODE)it;

        if(mrsToolsMemEq(node->ucDestMac, ucMac, HI_PLC_MAC_ADDR_LEN))
        {
            HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1003, HI_DIAG_MT("在链表中找到了对应节点"), ucMac, HI_PLC_MAC_ADDR_LEN);
            return node;
        }
    }

    return HI_NULL;
}

/* BEGIN: Modified by liuxipeng/lKF54842,2014/4/26 问题单号:DTS2014042407354 */
// 在缓冲池单一节点队列中查找已存在的读表请求
// ucOpt:
// MRS_RM_ADDR_MATCH 使用表地址匹配查找
// MRS_RM_CRC_MATCH  使用CRC值匹配查找
HI_PRV HI_BOOL mrsRmFindReqOfQueue(MRS_SRV_QUEUE *pstQueue, HI_U32 ulID, HI_U8 ucOpt)
{
    MRS_SRV_NODE * it;
    P_MRS_RM_ITEM item = HI_NULL;

/* BEGIN: PN: DTS2015042907341 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/29 */
    if (MRS_RM_ADDR_MATCH == ucOpt )
    {
        P_MRS_RM_REQ_DATA pData = (P_MRS_RM_REQ_DATA)ulID;
        HI_U8 aucMeterAddr[HI_METER_ADDR_LEN] = {0};
        HI_U8 aucTempAddr[HI_METER_ADDR_LEN] = {0};

        mrsMeterAddrPreProc(aucMeterAddr, pData->ucAddr, pData->ucProtocol);

        for (it = pstQueue->front; it != HI_NULL; it = it->next)
        {
            item = (P_MRS_RM_ITEM)it;
            mrsMeterAddrPreProc(aucTempAddr, item->ucMeterAddr, item->ucProtocol);
            if (mrsToolsMemEq(aucTempAddr, aucMeterAddr, HI_METER_ADDR_LEN))
            {
                return HI_TRUE;
            }
        }
    }
    else
    {
        for (it = pstQueue->front; it != HI_NULL; it = it->next)
        {
            item = (P_MRS_RM_ITEM)it;

            if(ulID == item->ulDataCrc)
            {
                return HI_TRUE;
            }
        }
    }
/* END:   PN: DTS2015042907341 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/29 */

    return HI_FALSE;
}

// 在缓冲池中查找已存在的读表请求(找到后返回HI_TRUE; 否则返回HI_FALSE;)
// ucOpt:
// MRS_RM_ADDR_MATCH 使用表地址匹配查找
// MRS_RM_CRC_MATCH  使用CRC值匹配查找
HI_PRV HI_BOOL mrsRmFindReqOfPool(P_MRS_RM_REQ_POOL pstPool, HI_U32 ulID, HI_U8 ucOpt)
{
    MRS_SRV_LINK * it;
    MRS_SRV_LIST * list;
    P_MRS_RM_NODE node = HI_NULL;

    list = &pstPool->stNodeList;

    // 遍历链表
    for (it = list->next; it != list; it = it->next)
    {
        node = (P_MRS_RM_NODE)it;

        if(mrsRmFindReqOfQueue(&node->stItemQueue, ulID, ucOpt))
        {
            return HI_TRUE;
        }
    }

    return HI_FALSE;
}
/* END  : Modified by liuxipeng/lKF54842,2014/4/26 问题单号:DTS2014042407354 */

// 释放缓冲池中读表请求元素
HI_PRVL HI_VOID mrsRmQueueFree(HI_VOID * p)
{
    mrsToolsFree(p);
    return;
}


// 释放节点
HI_PRV HI_VOID mrsRmNodeFree(HI_VOID *link, HI_VOID *param)
{
    P_MRS_RM_NODE pstNode = (P_MRS_RM_NODE)link;

    MRS_NOT_USED(param);

    if(pstNode)
    {
        mrsSrvEmptyQueue(&pstNode->stItemQueue, mrsRmQueueFree);
        mrsRmNodeCtxFree(pstNode->pstNodeCtx);
        mrsToolsFree(pstNode);
    }

    return;
}


// 清空缓冲池
HI_PRV HI_VOID mrsRmEmptyPool(P_MRS_RM_REQ_POOL pstPool)
{
    mrsSrvListClear(&pstPool->stNodeList, mrsRmNodeFree, HI_NULL);

    mrsRmSetPoolMode(pstPool, MRS_RM_NORMAL_MODE);
    pstPool->usParallelNum  = 0;
    pstPool->usItemTotalNum = 0;

    mrsDfxRmPalallelNum((HI_U32)pstPool, pstPool->usParallelNum);
    mrsDfxRmItemNum((HI_U32)pstPool, pstPool->usItemTotalNum);

    mrsSrvInitList(&pstPool->stNodeList);

    return;
}


// 移除节点(在缓冲池中将指定节点移除)
HI_PRV HI_VOID mrsRmRemoveNode(MRS_SRV_LIST *pstList, P_MRS_RM_NODE pstNode)
{
    HI_U32 ret = HI_ERR_SUCCESS;

    HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1004, HI_DIAG_MT("在链表中移除节点"), pstNode->ucDestMac, HI_PLC_MAC_ADDR_LEN);

    ret = mrsSrvListRemove(pstList, &pstNode->link);
    if(ret == HI_ERR_SUCCESS)
    {
        mrsRmNodeFree((HI_VOID *)pstNode, HI_NULL);
    }
    else
    {
        // 链表错误
        // TODO:......
    }

    return;
}


// 节点上下文创建-首次处理节点时使用
HI_PRV HI_U32 mrsRmNodeCtxCreate(P_MRS_RM_NODE pstNode)
{
    P_MRS_RM_ITEM pstItem = HI_NULL;

    pstItem = mrsSrvQueueTop(&pstNode->stItemQueue);
    if(pstItem == HI_NULL)
    {
        return HI_ERR_QUEUE_EMPTY;
    }

    pstNode->pstNodeCtx = mrsToolsMalloc(sizeof(MRS_NODE_CTX));
    if(pstNode->pstNodeCtx)
    {
        HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1005, HI_DIAG_MT("节点上下文创建"), pstNode->ucDestMac, HI_PLC_MAC_ADDR_LEN);

        (hi_void)memset_s(pstNode->pstNodeCtx, sizeof(MRS_NODE_CTX), 0, sizeof(MRS_NODE_CTX));
        pstNode->ucState = MRS_RM_NODE_UNINITIALIZED;
        return HI_ERR_SUCCESS;
    }
    else
    {
        return HI_ERR_NO_MORE_MEMORY;
    }
}


// 节点上下文复位(清零)-处理完成一个节点队列元素时使用
HI_PRV HI_VOID mrsRmNodeCtxReset(P_MRS_RM_NODE pstNode)
{
    P_MRS_NODE_CTX pstNodeCtx = pstNode->pstNodeCtx;

    if(pstNodeCtx)
    {
        HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1006, HI_DIAG_MT("节点上下文复位"), pstNode->ucDestMac, HI_PLC_MAC_ADDR_LEN);

        // 释放转发的PLC数据
        mrsToolsFree(pstNodeCtx->stPlcData.pucPayload);
        // 节点上下文清零
        (hi_void)memset_s(pstNodeCtx, sizeof(MRS_NODE_CTX), 0, sizeof(MRS_NODE_CTX));

        pstNode->ucState = MRS_RM_NODE_UNINITIALIZED;
    }

    return;
}


// 节点上下文释放-节点所有队列元素处理完毕后使用
HI_PRV HI_VOID mrsRmNodeCtxFree(P_MRS_NODE_CTX pstNodeCtx)
{
    if(pstNodeCtx)
    {
        // 释放转发的PLC数据
        mrsToolsFree(pstNodeCtx->stPlcData.pucPayload);
        // 节点上下文释放
        mrsToolsFree(pstNodeCtx);
    }
    return;
}

/* BEGIN: Modified by liuxipeng/lKF54842,2014/4/29 问题单号:DTS2014042306517 */
// 节点上下文初始化
HI_PRV HI_U32 mrsRmNodeCtxInit(P_MRS_RM_NODE pstNode, HI_BOOL bSwitchFlg)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    P_MRS_RM_ITEM pstItem = HI_NULL;
    P_MRS_RM_STRATEGY_REG pstStrategy = HI_NULL;
    P_MRS_RETRY_CFG_S pstTryCfg = HI_NULL;

    HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1007, HI_DIAG_MT("节点上下文初始化"), pstNode->ucDestMac, HI_PLC_MAC_ADDR_LEN);

    pstItem = mrsSrvQueueTop(&pstNode->stItemQueue);
    if(pstItem == HI_NULL)
    {
        HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1008, HI_DIAG_MT("节点队列为空"), pstNode->ucDestMac, HI_PLC_MAC_ADDR_LEN);
        return HI_ERR_QUEUE_EMPTY;
    }

    HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1009, HI_DIAG_MT("初始化新的读表请求"), pstItem->ucMeterAddr, HI_METER_ADDR_LEN);

    pstStrategy = mrsRmGetStrategy(pstItem->ucStrategyNo);
    pstTryCfg = mrsRmGetRetryCfg(pstStrategy->ucRetryCfgNo);

    // 重传控制参数初始化
    mrsRmNodeRetryParamInit(pstNode, pstTryCfg, HI_FALSE);

    // 创建PLC帧
    ret = mrsRmPlcPackCreate(pstNode, pstItem, pstTryCfg);
    if(ret == HI_ERR_SUCCESS)
    {
        HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_001, HI_DIAG_MT("创建PLC读表帧-序号"), pstNode->pstNodeCtx->ulPlcSeq);
        if(bSwitchFlg)
        {
            HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1010, HI_DIAG_MT("节点状态为-等待发送"), pstNode->ucDestMac, HI_PLC_MAC_ADDR_LEN);
            pstNode->ucState = MRS_RM_NODE_WAIT_SEND;
        }
        else
        {
            pstNode->ucState = MRS_RM_NODE_SEND_REQ;
        }
    }

    return ret;
}

//并发抄表 从异常电表链表中获取节点
HI_PRV HI_VOID mrsParallelRmFindFailNode(HI_U8 *MeterAddr, MRS_RM_FAIL_FIND_LINK* pstFailLink)
{
    P_MRS_READMETER_MODULE ReadMeterCtx = mrsRmGetModuleCtx();

    (hi_void)memset_s(pstFailLink, sizeof(MRS_RM_FAIL_FIND_LINK), 0, sizeof(MRS_RM_FAIL_FIND_LINK));
    (hi_void)memcpy_s(pstFailLink->ucMeterAddr,HI_PLC_MAC_ADDR_LEN,MeterAddr,HI_PLC_MAC_ADDR_LEN);//表地址
    mrsSrvTraverseList(&ReadMeterCtx->stParallelRmList, mrsRmFindMeterListNode, pstFailLink);
}
// 并发抄表 向异常电表缓冲池中增加一个节点
HI_VOID mrsRmAddFailNode(HI_U8 *MeterAddr)
{
    P_MRS_RM_FAIL_NODE pstNode = HI_NULL;
    P_MRS_READMETER_MODULE ReadMeterCtx = mrsRmGetModuleCtx();
    MRS_RM_FAIL_FIND_LINK stFailLink;

    mrsParallelRmFindFailNode(MeterAddr, &stFailLink);
    if(HI_NULL == stFailLink.pstLink)
    {
        pstNode = (P_MRS_RM_FAIL_NODE)mrsToolsMalloc(sizeof(MRS_RM_FAIL_NODE));
        if(HI_NULL != pstNode)
        {
            (hi_void)memset_s(pstNode,sizeof(MRS_RM_FAIL_NODE), 0,sizeof(MRS_RM_FAIL_NODE));
            (hi_void)memcpy_s(pstNode->ucMeterAddr, HI_PLC_MAC_ADDR_LEN, MeterAddr, HI_PLC_MAC_ADDR_LEN);
            mrsSrvListInsTail(&ReadMeterCtx->stParallelRmList,&pstNode->link);
            pstNode->ucFailNum++;
            pstNode->ucAllNum++;
        }
    }
    else
    {
        pstNode = (P_MRS_RM_FAIL_NODE)(stFailLink.pstLink);
        if(pstNode->ucFailNum < MRS_RM_PARALLEL_FAIL_CNT_MAX)
        {
            pstNode->ucFailNum++;
        }
    }
}

// 并发抄表 异常电表链表查询函数调用函数
HI_BOOL mrsRmFindMeterListNode(HI_VOID * pstMeterListNode, HI_VOID * pstExist)
{
    P_MRS_RM_FAIL_NODE pstNode = (P_MRS_RM_FAIL_NODE)pstMeterListNode;
    MRS_RM_FAIL_FIND_LINK* pstFailLink = (MRS_RM_FAIL_FIND_LINK*)pstExist;

    if ((!pstMeterListNode) || (!pstExist))
     {
        return HI_TRUE;
     }

    if (mrsToolsMemEq(pstNode->ucMeterAddr, pstFailLink->ucMeterAddr, HI_PLC_MAC_ADDR_LEN))
    {
        pstFailLink->pstLink = &(pstNode->link);
        return HI_TRUE;
    }

    return HI_FALSE;
 }

// 并发抄表 异常电表链表删除节点
HI_VOID mrsParallelRmFailDel(HI_U8* MeterAddr)
{
    P_MRS_RM_FAIL_NODE pstFailNode = HI_NULL;
    P_MRS_READMETER_MODULE ReadMeterCtx = mrsRmGetModuleCtx();
    MRS_RM_FAIL_FIND_LINK stFailLink;

    //异常电表链表中的mac地址是倒序的
    mrsParallelRmFindFailNode(MeterAddr, &stFailLink);
    if(HI_NULL != stFailLink.pstLink)
    {
        pstFailNode = (P_MRS_RM_FAIL_NODE)(stFailLink.pstLink);
        mrsSrvListRemove(&ReadMeterCtx->stParallelRmList,&pstFailNode->link);
        mrsToolsFree(pstFailNode);
    }
}

// 并发抄表 异常电表返回
HI_U32 mrsParallelFailMeterBack(P_MRS_RM_REQ_DATA pstData)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    P_MRS_RM_FAIL_NODE pstFailNode = HI_NULL;
    P_MRS_READMETER_MODULE ReadMeterCtx = mrsRmGetModuleCtx();
    MRS_RM_FAIL_FIND_LINK stFailLink;

    if(ReadMeterCtx->stParallelRmList.next != &ReadMeterCtx->stParallelRmList)
    {
        mrsParallelRmFindFailNode(pstData->ucAddr, &stFailLink);
        pstFailNode = (P_MRS_RM_FAIL_NODE)(stFailLink.pstLink);
        if((HI_NULL != pstFailNode) && (pstFailNode->ucFailNum >= MRS_RM_PARALLEL_FAIL_CNT_MAX))
        {
            if(pstFailNode->ucAllNum < MRS_RM_PARALLEL_FAIL_SEND_ONE)
            {
                pstData->usDataLen  = 0;
                pstData->pucData    = HI_NULL;
                mrsParallelPlcRespProc(pstData);
                pstFailNode->ucAllNum++;
                ret = HI_ERR_EXIST;
            }
            else
            {
                pstFailNode->ucAllNum = 1;
            }
        }
    }
    return ret;
}

/* END  : Modified by liuxipeng/lKF54842,2014/4/29 问题单号:DTS2014042306517 */

// 组织数据转发数据
HI_PRVL HI_VOID mrsRmSetupTransmitData(P_MRS_RETRY_CFG_S pstTryCfg,
                                                P_MRS_RM_ITEM pstItem,
                                                P_MRS_NODE_CTX pstNodeCtx,
                                                MRS_DATA_TRANSMIT_AFNF1_DL_STRU *pstData)
{
    pstData->interface_ver   = MRS_AFN_F1_DL_VER;    // 获取版本号
    pstData->stru_size       = sizeof(MRS_DATA_TRANSMIT_AFNF1_DL_STRU);
    pstData->noresp_try_flag = (pstTryCfg->MeterRetryCfgNv & 0x01)? HI_TRUE : HI_FALSE; // 无应答重试标志-后续改为NV项
    pstData->nak_try_flag    = (pstTryCfg->MeterRetryCfgNv & 0x02)? HI_TRUE : HI_FALSE; // 否认应答重试标志-后续改为NV项
    pstData->try_max   = pstTryCfg->MeterMaxRetryNv;    // 重试次数(STA对电表)
    pstData->protocol  = pstItem->ucProtocol;           // 获取规约类型
    pstData->data_len  = pstItem->usDataLen;            // 获取待转发报文长度
    pstData->seq       = (HI_U16)pstNodeCtx->ulPlcSeq;  // 获取帧序号
    pstData->time_out  = (HI_U8)MRS_MS_TO_100MS(pstNodeCtx->ulOverTimeMeter);  // 获取电表超时时间(100ms)
    pstData->gap_timer = (HI_U8)MRS_MS_TO_10MS(pstTryCfg->MeterFrameInterval);  // 帧间隔时间
    (hi_void)memcpy_s(pstData->data, pstData->data_len, pstItem->pucData, pstItem->usDataLen); // 获取待转发的数据内容
    return;
}

// 创建PLC数据包
HI_PRV HI_U32 mrsRmPlcPackCreate(P_MRS_RM_NODE pstNode,
                                        P_MRS_RM_ITEM pstItem,
                                        P_MRS_RETRY_CFG_S pstTryCfg)
{
    P_MRS_NODE_CTX pstNodeCtx = HI_NULL;
    MRS_PLC_FRAME_DATA_STRU *pstPlc = HI_NULL;
    MRS_DATA_TRANSMIT_AFNF1_DL_STRU *pstData = HI_NULL;
    HI_U16 usLen = 0;

    if((!pstNode) || (!pstItem))
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    usLen = sizeof(MRS_DATA_TRANSMIT_AFNF1_DL_STRU) + pstItem->usDataLen;
    pstData = (MRS_DATA_TRANSMIT_AFNF1_DL_STRU *)mrsToolsMalloc(usLen);
    if(!pstData)
    {
        return HI_ERR_NO_MORE_MEMORY;
    }
    (hi_void)memset_s(pstData, usLen, 0, usLen);

    pstNodeCtx = pstNode->pstNodeCtx;

    mrsRmPlcSeqAddSelf();
    pstNodeCtx->ulPlcSeq = mrsRmGetPlcSeq();

    // 组织转发数据包
    mrsRmSetupTransmitData(pstTryCfg, pstItem, pstNodeCtx, pstData);

    pstPlc = &pstNodeCtx->stPlcData;
    (hi_void)memset_s(pstPlc, sizeof(MRS_PLC_FRAME_DATA_STRU), 0, sizeof(MRS_PLC_FRAME_DATA_STRU));

    // 组织PLC包
    pstPlc->usId = ID_MRS_CMD_PARALLEL_DATA_PLC;    // 获取PLC帧命令ID
    (hi_void)memcpy_s(pstPlc->ucMac, HI_PLC_MAC_ADDR_LEN, pstNode->ucDestMac, HI_PLC_MAC_ADDR_LEN);  // 获取Mac地址
    pstPlc->usPayloadLen = usLen;                   // 获取转发数据包长度
    pstPlc->pucPayload   = (HI_U8 *)pstData;        // 获取转发数据包指针

    // 更新广播帧标志
    pstNodeCtx->ucIsBC = mrsToolsBroadCastMac(pstNode->ucDestMac);

#if defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG)
    //mrsLogFrameInfStatTx(pstItem->pucData, pstItem->usDataLen, pstNodeCtx->ucIsBC);//optimized by weichao
#endif

    return HI_ERR_SUCCESS;
}


// 获取目的MAC站点属性(层级;是否加I型采集器超时补偿)
HI_PRV HI_VOID mrsRmGetDestMacAttr(HI_U8 *pucMac, HI_U8 *pucLayer, HI_BOOL *pbCltIDelay)
{
    // 默认使用广播层级
    *pucLayer = MRS_RM_DEFAULT_LAYER_BACST;
    // 默认加I型采集器超时补偿
    *pbCltIDelay   = HI_TRUE;

    if (mrsToolsBroadCastMac(pucMac))
    {
        // 广播，若拓扑不含I采，则超时时间不加I采补偿
        *pbCltIDelay = mrsRmBcTimeoutCltIDelayFlag();
    }
    else
    {
        // 非广播MAC地址
        HI_U32 ret = HI_ERR_SUCCESS;
        HI_MAC_NETWORK_TOPO_ENTRY_S stAttr;

        (hi_void)memset_s(&stAttr, sizeof(stAttr), 0, sizeof(stAttr));
        ret = HI_MDM_QueryMacAttr(pucMac, &stAttr);
        if (ret == HI_ERR_SUCCESS)
        {
            // 获取PLC层级
            *pucLayer = stAttr.level;
            MRS_DATA_BOUNDARY_CHECK(*pucLayer, MRS_RM_MAC_MAX_LAYER, MRS_RM_MAC_MIN_LAYER);

            // 如果是I型采集器，则加I采超时补偿
            *pbCltIDelay = (stAttr.product_type == HI_ND_TYPE_CLTI)? HI_TRUE : HI_FALSE;
        }
    }

    return;
}

// 节点重传参数初始化-(最大重试次数;超时时间)
HI_PRV HI_VOID mrsRmNodeRetryParamInit(P_MRS_RM_NODE pstNode,
                                                P_MRS_RETRY_CFG_S pstTryCfg,
                                                HI_BOOL bForceBc)
{
    P_MRS_RM_ITEM pstItem     = HI_NULL;
    P_MRS_NODE_CTX pstNodeCtx = HI_NULL;
    HI_U8   ucLayer = MRS_RM_DEFAULT_LAYER_BACST;
    HI_BOOL bCltIDelay   = HI_TRUE;  // 是否加I采补偿
    HI_U32 ulPlcDelay   = 0;
    HI_U32 ulMeterTotal = 0; // 多帧情况下电表总超时时间

    pstItem = mrsSrvQueueTop(&pstNode->stItemQueue);
    if(pstItem == HI_NULL)
    {
        return;
    }

    pstNodeCtx = pstNode->pstNodeCtx;
    if(pstNodeCtx == HI_NULL)
    {
        return;
    }

    // 获取目的MAC站点属性
    mrsRmGetDestMacAttr(pstNode->ucDestMac, &ucLayer, &bCltIDelay);

    if (bForceBc || mrsToolsBroadCastMac(pstNode->ucDestMac))
    {
        ucLayer = MRS_RM_DEFAULT_LAYER_BACST;
        pstNodeCtx->ucMaxTry = (HI_U8)pstTryCfg->PlcMaxRetryNvB;
    }
    else
    {
        pstNodeCtx->ucMaxTry = (HI_U8)pstTryCfg->PlcMaxRetryNvS;
    }

    // 计算PLC线路消耗时间
    ulPlcDelay = (HI_U16)pstTryCfg->PlcLayerDelayNv * ucLayer  // 层级超时
               + (HI_U16)pstTryCfg->PlcRevisoryNv;             // 修正时间

    // 计算STA与电表之间单次通讯的超时时间
    pstNodeCtx->ulOverTimeMeter = (HI_U16)pstTryCfg->MeterOverTimeNv                     // 电表超时
                                + (HI_U16)pstTryCfg->CltIDelayNv * bCltIDelay            // I型采集器补偿
                                + (HI_U16)pstTryCfg->CtrlCmdDelayNv * pstItem->bCtrlCmd; // 费控命令补偿

    // 计算STA与电表间的总耗时
    ulMeterTotal = pstNodeCtx->ulOverTimeMeter * pstItem->ucFrameNum
                 + pstTryCfg->MeterFrameInterval * (pstItem->ucFrameNum - 1);    // STA端帧间隔补偿

    // 如果电表端重传配置有效-补偿重传耗时
    if((pstTryCfg->MeterRetryCfgNv != 0)
        && (pstItem->ucFrameNum <= MRS_CHECK_METER_FAULT_FRAME_NUM))
    {
        ulMeterTotal = ulMeterTotal * (1 + pstTryCfg->MeterMaxRetryNv);
    }

    // 计算CCO侧单次PLC通讯总超时时间
    pstNodeCtx->ulOverTimeTotal = ulPlcDelay    // PLC时延
                                + ulMeterTotal  // STA端电表超时
                                + MRS_RM_CHECK_TIMER_UNIT_BASE / 2;   // 修正定时器偏差

    if (bCltIDelay)
    {
        pstNodeCtx->ulOverTimeMeter = (HI_U16)pstTryCfg->MeterOverTimeNv                     // 电表超时
                                    + (HI_U16)MRS_RM_CLTI_DELAY_DEFAULT * bCltIDelay            // I型采集器补偿
                                    + (HI_U16)pstTryCfg->CtrlCmdDelayNv * pstItem->bCtrlCmd; // 费控命令补偿
    }

    return;
}


// 发送PLC数据包
HI_PRV HI_U32 mrsRmSendPlcPackage(P_MRS_NODE_CTX pstNodeCtx)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U8 mac[HI_PLC_MAC_ADDR_LEN] = {0};

    if(!pstNodeCtx)
    {
        return HI_ERR_BAD_DATA;
    }

    HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1011, HI_DIAG_MT("PLC目的MAC"), pstNodeCtx->stPlcData.ucMac, HI_PLC_MAC_ADDR_LEN);
    if (pstNodeCtx->ucIsBC)
    {
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_002, HI_DIAG_MT("[广播]用全F替换目的MAC"));
        (hi_void)memcpy_s(mac, HI_PLC_MAC_ADDR_LEN, pstNodeCtx->stPlcData.ucMac, HI_PLC_MAC_ADDR_LEN);
        (hi_void)memset_s(pstNodeCtx->stPlcData.ucMac, HI_PLC_MAC_ADDR_LEN, 0xFF, HI_PLC_MAC_ADDR_LEN);
    }

    ret = MRS_SendPlcFrame(&pstNodeCtx->stPlcData);
    mrsDfxPrPlcTx();

/* BEGIN: Modified by fengxiaomin/00209182, 2014/3/27   问题单号:DTS2014032906596 */
    {
        MRS_DATA_TRANSMIT_AFNF1_DL_STRU *pstAfnF1 = (MRS_DATA_TRANSMIT_AFNF1_DL_STRU*)(pstNodeCtx->stPlcData.pucPayload);
        if(pstNodeCtx->stPlcData.usPayloadLen == pstAfnF1->data_len + sizeof(MRS_DATA_TRANSMIT_AFNF1_DL_STRU))
        {
            mrsDfxRmPlcTx((HI_U8*)pstAfnF1 + sizeof(MRS_DATA_TRANSMIT_AFNF1_DL_STRU), pstAfnF1->data_len, ret);
        }
    }
/* END:   Modified by fengxiaomin/00209182, 2014/3/27 */

    if(pstNodeCtx->ucIsBC)
    {
        (hi_void)memcpy_s(pstNodeCtx->stPlcData.ucMac, HI_PLC_MAC_ADDR_LEN, mac, HI_PLC_MAC_ADDR_LEN);
    }

    return ret;
}

/* BEGIN: Modified by liuxipeng/lKF54842,2014/4/26 问题单号:DTS2014042407354 */
// 判断是否允许加入缓存池(返回HI_ERR_SUCCESS允许加入;其它不允许加入)
HI_PRV HI_U32 mrsRmAllowJoinPool(P_MRS_RM_REQ_POOL pstPool,
                                        P_MRS_RM_REQ_DATA pstData,
                                        MRS_645BUF_INF *pst645Inf)
{
    P_MRS_PARALLEL_CFG_S pstCfg = HI_NULL;
    HI_U32 ulID  = 0;

    pstCfg = &pstPool->stParallelCfg;

    // 缓冲池读表请求元素已达到最大值-不允许加入
    if(pstPool->usItemTotalNum >= pstCfg->usMaxTotalItem)
    {
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_003, HI_DIAG_MT("缓存池满-拒绝加入"));
        return HI_ERR_FULL;
    }

    if (mrsCheck645Proto(pstData->ucProtocol))
    {
        // 获取业务读表缓冲区中645帧信息
        mrsCountBuf645Inf(pstData->pucData, pstData->usDataLen, pst645Inf);
    }
    else if (0 != pstData->usDataLen)
    {
        pst645Inf->ucNum = 1;
        pst645Inf->pucAddr = pstData->ucAddr;
        pst645Inf->pucHead = pstData->pucData;
        pst645Inf->ulCrc = HI_MDM_Crc32(0, pstData->pucData, pstData->usDataLen);
    }

/* BEGIN: Modified by liuxipeng/lKF54842,2014/5/15 问题单号:DTS2014051308231 */
    // 业务读表缓冲区中没有645帧-不允许加入
    if(pst645Inf->ucNum == 0)
    {
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_004, HI_DIAG_MT("有效抄表帧数为0"));
        return HI_ERR_BAD_DATA;
    }

    // 业务读表缓冲区中超过最大帧数限制-不允许加入
    if(pst645Inf->ucNum > pstCfg->usMaxFrame)
    {
        HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_005, HI_DIAG_MT("超过最大多帧数"), pst645Inf->ucNum);
        return HI_ERR_NOT_SUPPORT;
    }

    HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_006, HI_DIAG_MT("当前读表请求CRC校验"), pst645Inf->ulCrc);
/* END  : Modified by liuxipeng/lKF54842,2014/5/15 问题单号:DTS2014051308231 */

    // 如果业务读表没有提供地址则使用645帧地址
    if(pstData->ucAddr == HI_NULL)
    {
        pstData->ucAddr = pst645Inf->pucAddr;
    }

    if(pstCfg->ucCheckRepeatMode == MRS_RM_ADDR_MATCH)
    {
/* BEGIN: PN: DTS2015042907341 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/29 */
        // 使用电表地址作为重复匹配项
        ulID = (HI_U32)pstData;
/* END:   PN: DTS2015042907341 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/29 */
    }
    else
    {
        // 使用CRC作为重复匹配项
        ulID = pst645Inf->ulCrc;
    }

    // 缓冲池中如果已存在此读表请求-不允许加入
    if(mrsRmFindReqOfPool(pstPool, ulID, pstCfg->ucCheckRepeatMode))
    {
        HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1012, HI_DIAG_MT("此电表正在抄读中"), pstData->ucAddr, HI_METER_ADDR_LEN);
        return HI_ERR_BUSY;
    }

    return HI_ERR_SUCCESS;
}
/* END  : Modified by liuxipeng/lKF54842,2014/4/26 问题单号:DTS2014042407354 */

// 激活发送处理
HI_PRV HI_U32 mrsRmSendReqProcActive(P_MRS_RM_REQ_POOL pstPool)
{
    HI_SYS_QUEUE_MSG_S stMsg = {0};

    (hi_void)memset_s(&stMsg, sizeof(stMsg), 0, sizeof(stMsg));

    stMsg.ulMsgId = EN_MRS_FW_MSG_RM_PLC_SEND_REQ;
    stMsg.ulParam[0] = (HI_U32)pstPool;

    HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_007, HI_DIAG_MT("激活发送请求处理..."));

    return mrsSendMessage2Queue(&stMsg);
}


// 节点状态处理(返回HI_TRUE发送PLC;返回HI_FALSE不允许发送)
// 此函数中每个if分支都有可能在一次函数调用时执行到,不可随意调整前后顺序
HI_PRV HI_BOOL mrsRmNodeStatusProc(MRS_SRV_LIST *list, P_MRS_RM_NODE node)
{
    HI_U32 ret = HI_ERR_SUCCESS;

    // 如果是"请求发送"状态-直接返回发送允许
    if(node->ucState == MRS_RM_NODE_SEND_REQ)
    {
        return HI_TRUE;
    }

    // 如果为"未处理"状态-创建节点上下文
    if(node->ucState == MRS_RM_NODE_UNTREATED)
    {
        ret = mrsRmNodeCtxCreate(node);
        if(ret != HI_ERR_SUCCESS)
        {
            // 节点上下文创建失败-返回不允许发送
            return HI_FALSE;
        }
    }

    // 如果是节点上下文"未初始化"状态-初始化节点上下文
    if(node->ucState == MRS_RM_NODE_UNINITIALIZED)
    {
    /* BEGIN: Modified by liuxipeng/lKF54842,2014/4/29 问题单号:DTS2014042306517 */
        ret = mrsRmNodeCtxInit(node, HI_FALSE);
    /* END  : Modified by liuxipeng/lKF54842,2014/4/29 问题单号:DTS2014042306517 */
        if(ret == HI_ERR_SUCCESS)
        {
            // 节点上下文初始化成功-返回发送允许
            return HI_TRUE;
        }
        else if(ret == HI_ERR_QUEUE_EMPTY)
        {
            // 节点队列为空-移除节点
            mrsRmRemoveNode(list, node);
        }
    }

    return HI_FALSE;
}


// 将PLC应答的数据回传给业务读表(PLC数据未响应时将pucData置空;usLen置零)
HI_PRV HI_U32 mrsRmReturnRespData(P_MRS_RM_STRATEGY_REG pstStrategy,
                                            P_MRS_RM_ITEM pstItem,
                                            HI_U8 *pucData,
                                            HI_U16 usLen,
                                            HI_U8 ucMacAdd[HI_PLC_MAC_ADDR_LEN])
{
    HI_U32 ret = HI_ERR_SUCCESS;

    if(pstStrategy->pfnRespProc)
    {
        MRS_RM_RESP_DATA stData = {0};

        stData.ucAddr     = pstItem->ucMeterAddr;
        stData.usRmType   = pstStrategy->usRmType;
        stData.ucProtocol = pstItem->ucProtocol;
        stData.ucMrSeq    = pstItem->ucMrSeq;
        stData.usDataLen  = usLen;
        stData.pucData    = pucData;
        (hi_void)memcpy_s(stData.ucMacAdd, HI_PLC_MAC_ADDR_LEN, ucMacAdd,  HI_PLC_MAC_ADDR_LEN);

        //异常电表处理
        if(usLen)
        {
            mrsParallelRmFailDel(pstItem->ucMeterAddr);
        }

        // 调用业务读表PLC应答处理函数
        ret = pstStrategy->pfnRespProc(&stData);

    /* BEGIN: Modified by fengxiaomin/00209182, 2014/3/27   问题单号:DTS2014032906596 */
        mrsDfxRmUartTx(pucData, usLen, HI_FALSE);
    /* END:   Modified by fengxiaomin/00209182, 2014/3/27 */
    }

    return ret;
}


// PLC重传处理(策略1-适用于点抄"AFN=13H-F1")，目前未使用
HI_PRV HI_U32 mrsRmPlcRetryProcStrategy01(P_MRS_RM_NODE pstNode, P_MRS_RETRY_CFG_S pstTryCfg)
{
    MRS_NOT_USED(pstNode);
    MRS_NOT_USED(pstTryCfg);

    // TODO:.....

    // 不需要重传
    return HI_ERR_SUCCESS;
}


// PLC重传处理(策略2-可适用于轮抄、并发、数据转发)，目前只用于并发
HI_PRV HI_U32 mrsRmPlcRetryProcStrategy02(P_MRS_RM_NODE pstNode, P_MRS_RETRY_CFG_S pstTryCfg)
{
    P_MRS_NODE_CTX pstNodeCtx = pstNode->pstNodeCtx;

    // 如果不是广播报文
    if(pstNodeCtx->ucIsBC == HI_FALSE)
    {
        // 判断是否需要切换为广播方式重试
        if((pstNodeCtx->ucTry >= pstNodeCtx->ucMaxTry)
            && (pstTryCfg->PlcMaxRetryNvB))
        {
            HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_008, HI_DIAG_MT("单播强制切换为广播"));

            pstNodeCtx->ucTry = 0;
            pstNodeCtx->ucIsBC = HI_TRUE;

            // 重新初始化重传参数(使用强制广播)
            mrsRmNodeRetryParamInit(pstNode, pstTryCfg, HI_TRUE);
        }
    }

    // 判断有无超过最大重试次数
    if(pstNodeCtx->ucTry < pstNodeCtx->ucMaxTry)
    {
        mrsDfxPrPlcTryCnt();    // 统计PLC重发次数

        pstNodeCtx->ucTry++;
        pstNode->ucState = MRS_RM_NODE_SEND_REQ;

        // 需要重传
        return HI_ERR_CONTINUE;
    }

    // 不需要重传
    return HI_ERR_SUCCESS;
}


// PLC超时处理
HI_PRV HI_U32 mrsRmPlcOverTimeProc(P_MRS_RM_REQ_POOL pstPool, P_MRS_RM_NODE pstNode)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    P_MRS_RM_ITEM pstItem  = HI_NULL;
    P_MRS_RM_STRATEGY_REG pstStrategy = HI_NULL;
    P_MRS_RETRY_CFG_S pstTryCfg   = HI_NULL;
    P_MRS_NODE_CTX pstNodeCtx     = pstNode->pstNodeCtx;

    mrsDfxPrPlcTimeout();
/* BEGIN: Modified by fengxiaomin/00209182, 2014/3/27   问题单号:DTS2014032906596 */
    mrsDfxRmPlcDlRx(ID_MRS_CMD_PARALLEL_DATA_PLC, pstNodeCtx->stPlcData.pucPayload, HI_TRUE);
/* END:   Modified by fengxiaomin/00209182, 2014/3/27 */

    pstNodeCtx->ulRunOutTime = 0;

    // 当前并发数自减
    pstPool->usParallelNum--;
    mrsDfxRmPalallelNum((HI_U32)pstPool, pstPool->usParallelNum);

    if(pstNodeCtx->ucIsBC)
    {
        mrsRmSetPoolMode(pstPool, MRS_RM_NORMAL_MODE);
    }

    pstItem = mrsSrvQueueTop(&pstNode->stItemQueue);
    if(pstItem == HI_NULL)
    {
        return HI_ERR_QUEUE_EMPTY;
    }

    pstStrategy = mrsRmGetStrategy(pstItem->ucStrategyNo);
    pstTryCfg = mrsRmGetRetryCfg(pstStrategy->ucRetryCfgNo);

    if(pstStrategy->ucRetryStrategy == EN_MRS_RM_RETRY_STRATEGY_01)
    {
        ret = mrsRmPlcRetryProcStrategy01(pstNode, pstTryCfg);
    }
    else
    {
        ret = mrsRmPlcRetryProcStrategy02(pstNode, pstTryCfg);
    }

    if(ret == HI_ERR_CONTINUE)
    {
        // 需要重传
        HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1013, HI_DIAG_MT("PLC帧需要重传"), pstItem->ucMeterAddr, HI_METER_ADDR_LEN);
        return ret;
    }

    HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1014, HI_DIAG_MT("PLC读表失败"), pstItem->ucMeterAddr, HI_METER_ADDR_LEN);

    //异常电表处理
    mrsRmAddFailNode(pstItem->ucMeterAddr);

    mrsDfxPrFailCnt();
    //mrsLogPlcOverTimeStat(pstItem->pucData, pstItem->usDataLen, 0, HI_NULL);//optimized by weichao

    // 通知业务读表PLC超时
    mrsRmReturnRespData(pstStrategy, pstItem, HI_NULL, 0, pstNode->ucDestMac);
    mrsDetectPrRmTimeout(pstItem->ucMeterAddr);
    mrsDecreaseMrTTL(&(mrsCcoGetContext()->stMapModule), pstItem->ucMeterAddr);

    // 总读表请求个数自减
    pstPool->usItemTotalNum--;
    mrsDfxRmItemNum((HI_U32)pstPool, pstPool->usItemTotalNum);

/* BEGIN: Modified by liuxipeng/lKF54842,2014/4/29 问题单号:DTS2014042306517 */
    // 读表请求出队列(如果队列不为空则初始化后续请求)
    return mrsRmItemDeQueue(pstNode, HI_FALSE);
/* END  : Modified by liuxipeng/lKF54842,2014/4/29 问题单号:DTS2014042306517 */
}


/* BEGIN: Modified by liuxipeng/lKF54842,2014/4/29 问题单号:DTS2014042306517 */
// PLC超时检查
HI_PRV HI_U32 mrsRmPlcOverTimeCheck(P_MRS_RM_REQ_POOL pstPool,
                                                P_MRS_RM_NODE pstNode,
                                                HI_BOOL *bOverTime)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    P_MRS_NODE_CTX pstNodeCtx = HI_NULL;

    pstNodeCtx = pstNode->pstNodeCtx;

    pstNodeCtx->ulRunOutTime += MRS_RM_CHECK_TIMER_UNIT_BASE;

    // 并发节点超时判断
    if(pstNodeCtx->ulRunOutTime >= pstNodeCtx->ulOverTimeTotal)
    {
        *bOverTime = HI_TRUE;

        HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1015, HI_DIAG_MT("处理PLC超时-MAC地址"), pstNode->ucDestMac, HI_PLC_MAC_ADDR_LEN);

        // 并发节点超时处理
        ret = mrsRmPlcOverTimeProc(pstPool, pstNode);

        if(ret == HI_ERR_QUEUE_EMPTY)
        {
            mrsRmRemoveNode(&pstPool->stNodeList, pstNode);
        }

        HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_009, HI_DIAG_MT("当前并发数-当前读表请求数"), pstPool->usParallelNum, pstPool->usItemTotalNum);
    }

    return ret;
}


// PLC发送等待到期检查
HI_PRV HI_U32 mrsRmPlcSendWaitCheck(P_MRS_RM_REQ_POOL pstPool,
                                                P_MRS_RM_NODE pstNode,
                                                HI_U32 *pulWaitSendNum,
                                                HI_BOOL *bOverTime)
{
    P_MRS_NODE_CTX pstNodeCtx = HI_NULL;

    pstNodeCtx = pstNode->pstNodeCtx;

    pstNodeCtx->ulWaitSendTime += MRS_RM_CHECK_TIMER_UNIT_BASE;

    // 电表切换间隔判断
    if(pstNodeCtx->ulWaitSendTime >= (HI_U32)(pstPool->stParallelCfg.ucMeterSwitchInterval * MRS_RM_METER_SWITCH_TIME_UNIT))
    {
        HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1016, HI_DIAG_MT("节点状态切换为-请求发送"), pstNode->ucDestMac, HI_PLC_MAC_ADDR_LEN);

        pstNodeCtx->ulWaitSendTime = 0;
        *bOverTime = HI_TRUE;
        pstNode->ucState = MRS_RM_NODE_SEND_REQ;
    }
    else
    {
        (*pulWaitSendNum)++;
    }

    return HI_ERR_SUCCESS;
}
/* END  : Modified by liuxipeng/lKF54842,2014/4/29 问题单号:DTS2014042306517 */


// 缓存池PLC接收处理
HI_PRV HI_U32 mrsRmPoolPlcRxProc(P_MRS_RM_REQ_POOL pstPool, HI_U8 *pucData,
    MRS_DATA_TRANSMIT_AFNF1_UP_STRU *pstPackage, HI_BOOL *bFind, MRS_DEVICE_INF_STRU *pstDevInf, HI_U8 ucMacAdd[HI_PLC_MAC_ADDR_LEN])
{
    HI_U32 ret = HI_ERR_SUCCESS;
    MRS_SRV_LINK *it, *next;
    MRS_SRV_LIST *list;

    list = &pstPool->stNodeList;

    it = list->next;
    while(it != list)   // 遍历链表
    {
        P_MRS_RM_NODE node = (P_MRS_RM_NODE)it;
        P_MRS_NODE_CTX pstNodeCtx = HI_NULL;

        if(node->ucState != MRS_RM_NODE_WAIT_RESP)
        {
            it = it->next;
            continue;
        }

        next = it->next;

        pstNodeCtx = node->pstNodeCtx;

        if(pstPackage->seq == (HI_U16)pstNodeCtx->ulPlcSeq)
        {
            *bFind = HI_TRUE;

            pstPool->usParallelNum--;
            pstPool->usItemTotalNum--;

            mrsDfxRmPalallelNum((HI_U32)pstPool, pstPool->usParallelNum);
            mrsDfxRmItemNum((HI_U32)pstPool, pstPool->usItemTotalNum);

            if(pstNodeCtx->ucIsBC)
            {
                HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_010, HI_DIAG_MT("广播抄表应答成功-缓存池模式修改为[普通]"));
                mrsRmSetPoolMode(pstPool, MRS_RM_NORMAL_MODE);
            }

            HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_011, HI_DIAG_MT("处理PLC应答数据包-[并发数][请求数]"), pstPool->usParallelNum, pstPool->usItemTotalNum);

            ret = mrsRmPlcRespProc(node, pucData, pstPackage, pstDevInf, ucMacAdd);
            if(ret == HI_ERR_QUEUE_EMPTY)
            {
                mrsRmRemoveNode(list, node);
            }

            break;
        }

        it = next;
    }

    if(pstPool->usItemTotalNum)
    {
        mrsRmSendReqProcActive(pstPool);
    }
    else
    {
        // PR is over, start detect RM.
        MRS_StartTimer(MRS_CCO_TIMER_RESUME_DETECT_RM, MRS_CCO_TIME_RESUME_DETECT_RM_DEFAULT, HI_SYS_TIMER_ONESHOT);

        HI_MDM_SetTfNtbSendProcessCtrl(HI_MDM_NTB_SEND_PROCESS_RESUME, HI_MDM_NTB_SEND_CTRL_REASON_READ_METER);
    }

    return ret;
}

/* BEGIN: Modified by liuxipeng/lKF54842,2014/4/29 问题单号:DTS2014042306517 */
// 缓存池超时检查定时器处理
HI_PRV HI_U32 mrsRmPoolCheckTimerProc(P_MRS_RM_REQ_POOL pstPool,
                                                HI_U32 *pulPlcNum, HI_U32 *pulWaitSendNum)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    MRS_SRV_LINK *it, *next;
    MRS_SRV_LIST *list;
    HI_BOOL bOverTime = HI_FALSE;

    list = &pstPool->stNodeList;

    it = list->next;
    while(it != list)   // 遍历链表
    {
        P_MRS_RM_NODE node = (P_MRS_RM_NODE)it;

        next = it->next;

        switch(node->ucState)
        {
        case MRS_RM_NODE_WAIT_SEND:
            ret = mrsRmPlcSendWaitCheck(pstPool, node, pulWaitSendNum, &bOverTime);
            break;

        case MRS_RM_NODE_WAIT_RESP:
            ret = mrsRmPlcOverTimeCheck(pstPool, node, &bOverTime);
            break;

        default:
            break;
        }

        it = next;
    }

    *pulPlcNum += pstPool->usParallelNum;

    // 缓存区中存在未处理完的读表请求
    if (bOverTime)
    {
        if (pstPool->usItemTotalNum)
        {
            mrsRmSendReqProcActive(pstPool);
        }
        else
        {
            // PR is over, start detect RM.
            MRS_StartTimer(MRS_CCO_TIMER_RESUME_DETECT_RM, MRS_CCO_TIME_RESUME_DETECT_RM_DEFAULT, HI_SYS_TIMER_ONESHOT);

            HI_MDM_SetTfNtbSendProcessCtrl(HI_MDM_NTB_SEND_PROCESS_RESUME, HI_MDM_NTB_SEND_CTRL_REASON_READ_METER);
        }
    }

    return ret;
}
/* END  : Modified by liuxipeng/lKF54842,2014/4/29 问题单号:DTS2014042306517 */


// 协议兼容性处理
HI_PRV HI_U32 mrsRmPlcUpCompatibleProc(MRS_DATA_TRANSMIT_AFNF1_UP_STRU *pstPackage,
                                                HI_U16 ucLen, HI_PBYTE *pucData)
{
    HI_U16 usPackageSize = 0;

    if((!pstPackage) || (!pucData))
    {
        return HI_ERR_FAILURE;
    }

    usPackageSize = pstPackage->stru_size + pstPackage->data_len;

    if(usPackageSize != ucLen)
    {
        *pucData = HI_NULL;
        mrsDfxPrPlcLenErr();
        return HI_ERR_BAD_DATA;
    }

    switch(pstPackage->interface_ver)
    {
    case MRS_AFN_F1_UP_VER_R0:
    default:
        usPackageSize = sizeof(MRS_DATA_TRANSMIT_AFNF1_UP_STRU) + pstPackage->data_len;
        if(ucLen < usPackageSize)
        {
            mrsDfxPrPlcLenErr();
            return HI_ERR_BAD_DATA;
        }

        break;
    }

    *pucData = (HI_U8 *)pstPackage + pstPackage->stru_size;

    return HI_ERR_SUCCESS;
}


// PLC帧应答处理
HI_PRV HI_U32 mrsRmPlcRespProc(P_MRS_RM_NODE pstNode, HI_U8 *pucData,
    MRS_DATA_TRANSMIT_AFNF1_UP_STRU *pstPackage, MRS_DEVICE_INF_STRU *pstDevInf, HI_U8 ucMacAdd[HI_PLC_MAC_ADDR_LEN])
{
    P_MRS_RM_ITEM pstItem  = HI_NULL;
    P_MRS_RM_STRATEGY_REG pstStrategy = HI_NULL;

    pstItem = mrsSrvQueueTop(&pstNode->stItemQueue);
    if(pstItem == HI_NULL)
    {
        return HI_ERR_QUEUE_EMPTY;
    }

    //将探测抄表中表档案中抄读成功表的探测标志清除
    mrsDetectRmRespProc(pstItem->ucMeterAddr);

    pstStrategy = mrsRmGetStrategy(pstItem->ucStrategyNo);

    mrsDfxPrOkCnt();
#if 0//optimized by weichao
#if defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG)
    mrsLogFrameInfStatRx(pucData, pstPackage->data_len,
                            pstItem->pucData, pstItem->usDataLen,
                            pstPackage->resp_status);
#endif
#endif
    // 发送读表应答数据到业务读表
    mrsRmReturnRespData(pstStrategy, pstItem, pucData, pstPackage->data_len, ucMacAdd);

    if (pstDevInf)
    {
        (hi_void)memcpy_s(pstDevInf->aucAddr, HI_METER_ADDR_LEN, pstItem->ucMeterAddr, HI_METER_ADDR_LEN);
        pstDevInf->ucProtocol = pstItem->ucProtocol;
    }

/* BEGIN: Modified by liuxipeng/lKF54842,2014/4/29 问题单号:DTS2014042306517 */
    // 读表请求出队列(如果队列不为空则初始化后续请求)
    return mrsRmItemDeQueue(pstNode, HI_TRUE);
/* END  : Modified by liuxipeng/lKF54842,2014/4/29 问题单号:DTS2014042306517 */
}


// 读表管理模块初始化
HI_U32 mrsRmModuleInit(HI_VOID)
{
    P_MRS_READMETER_MODULE pstRmCtx = mrsRmGetModuleCtx();
    HI_U8 i = 0;

    (hi_void)memset_s(pstRmCtx, sizeof(MRS_READMETER_MODULE), 0, sizeof(MRS_READMETER_MODULE));

    for(i = 0; i < MRS_RM_POOL_MAX_NUM; i++)
    {
        P_MRS_RM_REQ_POOL pstPool = HI_NULL;

        pstPool = mrsRmGetPool(i);

        mrsSrvInitList(&pstPool->stNodeList);

        mrsDfxRmPoolId((HI_U32)pstPool, i);
    }

    //并发抄表 异常电表链表初始化
    mrsSrvInitList(&pstRmCtx->stParallelRmList);

    return HI_ERR_SUCCESS;
}


// 业务读表策略注册
HI_U32 mrsRmRegisterStrategy(HI_U16 usType,
                                    MRS_RETRY_CFG_NUM_ENUM enTryCfgNo,
                                    MRS_RM_RETRY_STRATEGY_ENUM enTryTacNo,
                                    MRS_PARALLEL_POOL_ENUM enPoolNo,
                                    MRS_RM_DATA_RESP_PROC_F pfn)
{
    P_MRS_READMETER_MODULE pstRmCtx = mrsRmGetModuleCtx();
    P_MRS_RM_STRATEGY_REG pstStrategy = HI_NULL;
    HI_U32 i = 0;

    pstStrategy = pstRmCtx->stStrategyReg;

    for(i = 0; i < MRS_RM_STRATEGY_REG_MAX_NUM; i++)
    {
        if(pstStrategy->pfnRespProc == HI_NULL)
        {
            pstStrategy->usRmType = usType;
            pstStrategy->ucRetryCfgNo = enTryCfgNo;
            pstStrategy->ucRetryStrategy = enTryTacNo;
            pstStrategy->ucPoolNo = enPoolNo;
            pstStrategy->pfnRespProc = pfn;

            return HI_ERR_SUCCESS;
        }

        pstStrategy++;
    }

    return HI_ERR_FULL;
}


// 读表请求加入缓存池
HI_U32 mrsRmReqJoinPool(P_MRS_RM_REQ_DATA pstData)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    P_MRS_RM_STRATEGY_REG pstStrategy = HI_NULL;
    P_MRS_RM_REQ_POOL pstPool = HI_NULL;
    MRS_645BUF_INF st645Inf = {0};
    P_MRS_RM_NODE pstNode = HI_NULL;
    HI_U8 ucMac[HI_PLC_MAC_ADDR_LEN] = {0};
    HI_U8 ucStrategyNo = 0;
/* BEGIN: PN: DTS2015040900760 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/7 */
    HI_U8 aucMeter[HI_METER_ADDR_LEN] = {0};
/* END:   PN: DTS2015040900760 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/7 */

    do
    {
        ret = mrsRmFindStrategy(pstData->usRmType, &pstStrategy, &ucStrategyNo);
        if(HI_ERR_SUCCESS != ret)
        {
            break;
        }

        pstPool = mrsRmGetPool(pstStrategy->ucPoolNo);

        // 判断是否可以加入缓存池
        ret = mrsRmAllowJoinPool(pstPool, pstData, &st645Inf);
        if(ret != HI_ERR_SUCCESS)
        {
            break;
        }

        if (pstData->ucAddr)
        {
            mrsMeterAddrPreProc(aucMeter, pstData->ucAddr, pstData->ucProtocol);
            ret = mrsMapGetMAC(aucMeter, ucMac);
        }
        else
        {
            ret = HI_ERR_FAILURE;
        }

        // 查找映射关系

        if(ret != HI_ERR_SUCCESS)
        {
            (hi_void)memset_s(ucMac, HI_PLC_MAC_ADDR_LEN, 0xFF, HI_PLC_MAC_ADDR_LEN);   // 没有找到对应的MAC地址(使用广播MAC)
        }

        // 在缓存池中查找对应MAC地址的节点(缓存池纵向链表)
        pstNode = mrsRmFindNode(pstPool, ucMac);
        if(pstNode == HI_NULL)
        {
            // 未找到对应节点-添加对应MAC地址的新节点到缓存池(缓存池纵向链表)
            pstNode = mrsRmAddNode(pstPool, ucMac);

            // 新加入节点-激活发送请求处理
            mrsRmSendReqProcActive(pstPool);
        }

        // 在找到的节点中加入本次读表请求元素(缓存池横向队列)
        ret = mrsRmAddItem(pstNode, pstData, ucStrategyNo, &st645Inf);
        if(ret == HI_ERR_SUCCESS)
        {
            pstPool->usItemTotalNum++;
            mrsDfxRmItemNum((HI_U32)pstPool, pstPool->usItemTotalNum);
            HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_012, HI_DIAG_MT("请求加入缓存池成功"), pstPool->usItemTotalNum);
        }
    }while(0);

    if(ret != HI_ERR_SUCCESS)
    {
        mrsDfxRmJoinFail((HI_U32)pstPool, ret);
    }

    return ret;
}


// PLC发送请求处理
HI_U32 mrsRmPlcSendReqProc(HI_SYS_QUEUE_MSG_S* pstMsg)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    P_MRS_RM_REQ_POOL pstPool   = (P_MRS_RM_REQ_POOL)pstMsg->ulParam[0];
    P_MRS_PARALLEL_CFG_S pstCfg = &pstPool->stParallelCfg;
    MRS_SRV_LINK *it, *next;
    MRS_SRV_LIST *list;

    HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_013, HI_DIAG_MT("处理发送请求..."));

    if(pstPool->ucMode != MRS_RM_NORMAL_MODE)
    {
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_014, HI_DIAG_MT("缓存池工作模式为[广播]..."));
        if (MRS_RM_SCENE_TOPO_HAS_NO_CLT != mrsGetRmTopoScene())
        {
            HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_015, HI_DIAG_MT("[广播]非全载场景-仅允许一路并发..."));
            return HI_ERR_SUCCESS;
        }
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_016, HI_DIAG_MT("[广播]全载场景-允许多路并发..."));
    }

    list = &pstPool->stNodeList;

    it = list->next;
    while(it != list)    // 遍历链表
    {
        P_MRS_RM_NODE node = HI_NULL;
        P_MRS_NODE_CTX pstNodeCtx = HI_NULL;

        next = it->next;
        node = (P_MRS_RM_NODE)it;

        if(pstPool->usParallelNum >= pstCfg->usMaxParallelNum)
        {
            HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_017, HI_DIAG_MT("超过最大并发限制-暂停发送"), pstPool->usParallelNum);
            break;
        }

        if(mrsRmNodeStatusProc(list, node) == HI_FALSE)
        {
            it = next;
            continue;
        }

        pstNodeCtx = node->pstNodeCtx;

        ret = mrsRmSendPlcPackage(pstNodeCtx);
        if(ret == HI_ERR_SUCCESS)
        {
            node->ucState = MRS_RM_NODE_WAIT_RESP;
            pstPool->usParallelNum++;

            HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_018, HI_DIAG_MT("发送PLC帧成功"), pstPool->usParallelNum);
            mrsDfxRmPalallelNum((HI_U32)pstPool, pstPool->usParallelNum);

            if(pstNodeCtx->ucIsBC)
            {
                HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_019, HI_DIAG_MT("发送广播帧-缓存池模式变更为[广播]"));
                mrsRmSetPoolMode(pstPool, MRS_RM_BROADCAST_MODE);
                mrsDfxBcRmCnt();
                break;
            }
        }

        it = next;
    }

    if(mrsRmGetTimerStatus() == MRS_RM_TIMER_INACTIVE)
    {
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_020, HI_DIAG_MT("开启超时检查定时器"));
        MRS_StartTimer(EN_MRS_SRV_CCO_RM_CHECK_TIMER,
                            MRS_CCO_TIME_PARALLEL_CHECK,
                            HI_SYS_TIMER_PERIODIC);

        mrsRmSetTimerStatus(MRS_RM_TIMER_ACTIVE);
    }

    return HI_ERR_SUCCESS;
}


// PLC包接收
HI_U32 mrsRmPlcPackageReceive(MRS_DATA_TRANSMIT_AFNF1_UP_STRU *pstPackage, HI_U16 usLen, MRS_DEVICE_INF_STRU *pstDevInf, HI_U8 ucMacAdd[HI_PLC_MAC_ADDR_LEN])
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U8 *pucData = HI_NULL;
    HI_U8 ucPoolNo = 0;
    HI_BOOL find = HI_FALSE;

    if(!pstPackage || (0 == pstPackage->data_len))
    {
        return HI_ERR_FAILURE;
    }

    ret = mrsRmPlcUpCompatibleProc(pstPackage, usLen, &pucData);
    if(ret != HI_ERR_SUCCESS)
    {
        return ret;
    }

    HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_021, HI_DIAG_MT("接收PLC数据包-序号"), pstPackage->seq);

    for(ucPoolNo = 0; ucPoolNo < MRS_RM_POOL_MAX_NUM; ucPoolNo++)
    {
        P_MRS_RM_REQ_POOL pstPool = mrsRmGetPool(ucPoolNo);
        mrsRmPoolPlcRxProc(pstPool, pucData, pstPackage, &find, pstDevInf, ucMacAdd);
        if(find)
        {
            break;
        }
    }

    if(find == HI_FALSE)
    {
        HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_022, HI_DIAG_MT("接收PLC数据包-帧序号错误"), pstPackage->seq);
        mrsDfxPrPlcSeqErr();    // 统计丢弃的PLC帧(过期的)

    /* BEGIN: Modified by fengxiaomin/00209182, 2014/3/27   问题单号:DTS2014032906596 */
        mrsDfxRmPlcUpRx(ID_MRS_CMD_PARALLEL_DATA_PLC, (HI_U8*)pstPackage, HI_TRUE);
    /* END:   Modified by fengxiaomin/00209182, 2014/3/27 */
        ret = HI_ERR_FAILURE;
    }

    return ret;
}

/* BEGIN: Modified by liuxipeng/lKF54842,2014/4/29 问题单号:DTS2014042306517 */
// 读表模块超时检查定时器处理
HI_VOID mrsRmCheckTimerProc(HI_VOID)
{
    HI_U32 ulLinePlcNum = 0;    // 当前线路上的PLC帧总数量
    HI_U32 ulWaitSendNum = 0;
    HI_U8 ucPoolNo = 0;

    for(ucPoolNo = 0; ucPoolNo < MRS_RM_POOL_MAX_NUM; ucPoolNo++)
    {
        P_MRS_RM_REQ_POOL pstPool = mrsRmGetPool(ucPoolNo);
        mrsRmPoolCheckTimerProc(pstPool, &ulLinePlcNum, &ulWaitSendNum);
    }

    if((ulLinePlcNum == 0) && (ulWaitSendNum == 0))
    {
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_023, HI_DIAG_MT("停止超时检查定时器"));

        MRS_StopTimer(EN_MRS_SRV_CCO_RM_CHECK_TIMER);
        mrsRmSetTimerStatus(MRS_RM_TIMER_INACTIVE);
    }

    return;
}
/* END  : Modified by liuxipeng/lKF54842,2014/4/29 问题单号:DTS2014042306517 */


// 复位读表管理模块(集中器下发AFN=12-F1路由重启时调用)
HI_VOID mrsRmModuleReset(HI_VOID)
{
    HI_U8 ucPoolNo = 0;

    HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_024, HI_DIAG_MT("复位读表管理模块-清空缓存池"));

    for(ucPoolNo = 0; ucPoolNo < MRS_RM_POOL_MAX_NUM; ucPoolNo++)
    {
        P_MRS_RM_REQ_POOL pstPool = mrsRmGetPool(ucPoolNo);
        mrsRmEmptyPool(pstPool);
    }

    MRS_StopTimer(EN_MRS_SRV_CCO_RM_CHECK_TIMER);
    mrsRmSetTimerStatus(MRS_RM_TIMER_INACTIVE);

    // PR is over, start detect RM.
    MRS_StartTimer(MRS_CCO_TIMER_RESUME_DETECT_RM, MRS_CCO_TIME_RESUME_DETECT_RM_DEFAULT, HI_SYS_TIMER_ONESHOT);

    HI_MDM_SetTfNtbSendProcessCtrl(HI_MDM_NTB_SEND_PROCESS_RESUME, HI_MDM_NTB_SEND_CTRL_REASON_READ_METER);

    return;
}


#endif

