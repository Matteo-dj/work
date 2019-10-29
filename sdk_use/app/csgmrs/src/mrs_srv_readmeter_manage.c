//*****************************************************************************
//
//                  ��Ȩ���� (C), 1998-2011, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mrs_srv_readmeter_manage.c
//  ��    ��   : liuxipeng/KF54842
//  ��������   : 2014-02-28
//  ��������   : �������ģ�麯��ʵ��
//
//  �����б�   :
//  �޸���ʷ   :
//  1.��    �� : 2014-02-28
//    ��    �� : liuxipeng/KF54842
//    �޸����� : �����ļ�
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
#include "mrs_srv_cco_mr_task.h"
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)


// �������ģ�������Ķ���
HI_PRV MRS_READMETER_MODULE g_ReadMeterCtx;

// ��ȡ����ģ��������
HI_PRVL P_MRS_READMETER_MODULE mrsRmGetModuleCtx(HI_VOID)
{
    return &g_ReadMeterCtx;
}

// ͨ���ش����úŻ�ȡ�ش�����
HI_PRV P_MRS_RETRY_CFG_S mrsRmGetRetryCfg(HI_U8 ucIndex)
{
    return &(mrsRmGetModuleCtx()->stRetryCfg[ucIndex % MRS_RETRY_CFG_MAX_NUM]);
}

// ͨ������غŻ�ȡ�����ָ��
HI_PRV P_MRS_RM_REQ_POOL mrsRmGetPool(HI_U8 ucPoolNo)
{
    return &(mrsRmGetModuleCtx()->stPool[ucPoolNo % MRS_RM_POOL_MAX_NUM]);
}

// ͨ�����ԺŻ�ȡ�������
HI_PRV P_MRS_RM_STRATEGY_REG mrsRmGetStrategy(HI_U8 ucStrategyNo)
{
    return &(mrsRmGetModuleCtx()->stStrategyReg[ucStrategyNo % MRS_RM_STRATEGY_REG_MAX_NUM]);
}


#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
// ��ȡPLC֡���
HI_PRV HI_U32 mrsRmGetPlcSeq(HI_VOID)
{
    return mrsRmGetModuleCtx()->ulPlcSeq;
}

// PLC֡�������
HI_PRV HI_VOID mrsRmPlcSeqAddSelf(HI_VOID)
{
    mrsRmGetModuleCtx()->ulPlcSeq++;
    mrsDfxRmPlcSeq(mrsRmGetModuleCtx()->ulPlcSeq);
    return;
}
#endif

// ��ȡ����ʱ��鶨ʱ������״̬
HI_PRVL HI_U8 mrsRmGetTimerStatus(HI_VOID)
{
    return mrsRmGetModuleCtx()->ucTimerStatus;
}

// ���ö���ʱ��鶨ʱ������״̬
HI_PRVL HI_VOID mrsRmSetTimerStatus(HI_U8 ucStatus)
{
    mrsRmGetModuleCtx()->ucTimerStatus = ucStatus;
    mrsDfxRmTimerStatus(ucStatus);
    return;
}

// ���û���ع���ģʽ
HI_PRVL HI_VOID mrsRmSetPoolMode(P_MRS_RM_REQ_POOL pstPool, HI_U8 ucMode)
{
    pstPool->ucMode = ucMode;
    mrsDfxRmPoolMode((HI_U32)pstPool, ucMode);
    return;
}


// �ش����ò�����Ч�Լ��
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

    // �����ʱʱ��Ϊ0, ����Ĭ��ֵ����
    if (0 == pstCfg->MaxServiceTime)
    {
        pstCfg->MaxServiceTime = MRS_RM_MAX_SERVICE_TIME_DEFAULT;
    }

    return;
}

// ��ʼ���ش����Ʋ���
HI_VOID mrsRmRetryCfgInit(P_MRS_RETRY_CFG_S pstCfg, NV_MRS_RETRY_CFG_S * pstNvCfg)
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
        pstCfg->MeterMaxRetryNv     = MRS_RM_METER_RETRY_CNT_DEFAULT;
        pstCfg->MeterRetryCfgNv     = MRS_RM_METER_RETRY_CFG_DEFAULT;
        pstCfg->PlcMaxRetryLoop     = MRS_RM_PLC_RETRY_LOOP_DEFAULT;
        pstCfg->MaxServiceTime      = MRS_RM_MAX_SERVICE_TIME_DEFAULT;
    }
    else
    {
        // �����ش����ò���
        (hi_void)memcpy_s(pstCfg, sizeof(MRS_RETRY_CFG_S), pstNvCfg, sizeof(MRS_RETRY_CFG_S));

        // �ش����ò�����Ч�Լ��
        mrsRmRetryCfgValidtyCheck(pstCfg);
    }

    return;
}


// ��ʼ�������������
HI_PRV HI_VOID mrsRmParallelCfgInit(P_MRS_RM_REQ_POOL pstPool, P_MRS_PARALLEL_CFG_S pstCfg, P_NV_MRS_PARALLEL_CFG_S pstNvCfg)
{
    if(pstNvCfg == HI_NULL)
    {
        pstCfg->usMaxParallelNum = MRS_PARALLEL_NUM_DEFAULT;
        pstCfg->usMaxFrame = MRS_FRAME_NUM_DEFAULT;
        pstCfg->usMaxTotalItem = MRS_ITEM_NUM_DEFAULT;
        pstCfg->ucCheckRepeatMode = MRS_RM_ADDR_MATCH;
    }
    else
    {
        HI_U16 usMaxParallelNum = pstCfg->usMaxParallelNum;

        // ���²������ò���
        (hi_void)memcpy_s(pstCfg, sizeof(MRS_PARALLEL_CFG_S), pstNvCfg, sizeof(MRS_PARALLEL_CFG_S));

        if (HI_MDM_CHECK_MODE == mrsGetCheckMode())
        {
            pstCfg->usMaxParallelNum = usMaxParallelNum;
        }

        MRS_DATA_BOUNDARY_CHECK(pstCfg->usMaxParallelNum,
                                MRS_PARALLEL_NUM_MAX,
                                MRS_PARALLEL_NUM_MIN);

        MRS_DATA_BOUNDARY_CHECK(pstCfg->usMaxFrame,
                                MRS_FRAME_NUM_MAX,
                                MRS_FRAME_NUM_MIN);

        MRS_DATA_BOUNDARY_CHECK(pstCfg->usMaxTotalItem,
                                MRS_ITEM_NUM_MAX,
                                MRS_ITEM_NUM_MIN);
        if((pstCfg->ucCheckRepeatMode != MRS_RM_CRC_MATCH)
            && (pstCfg->ucCheckRepeatMode != MRS_RM_ADDR_MATCH))
        {
            pstCfg->ucCheckRepeatMode = MRS_RM_ADDR_MATCH;
        }
    }

    pstPool->usMaxParalNumNv = pstCfg->usMaxParallelNum;

    return;
}

HI_VOID mrsSetParallelNum(HI_U16 usNum)
{
    P_MRS_RM_REQ_POOL pstPool = mrsRmGetPool(1);
    P_MRS_PARALLEL_CFG_S pstCfg = &pstPool->stParallelCfg;

    pstCfg->usMaxParallelNum = usNum;
}

HI_U16 mrsGetNvParallelNum(HI_VOID)
{
    P_MRS_RM_REQ_POOL pstPool = mrsRmGetPool(1);

    return pstPool->usMaxParalNumNv;
}

/* BEGIN: PN: DTS2015033100426 MODIFY\ADD\DEL by cuiate/00233580 at 2015/3/26 */
// ��ʼ�������ش�����
HI_VOID mrsRmCfgInit(NV_MRS_RETRY_CFG_S *pstParam)
{
    HI_U8 i = 0;

    for(i = 0; i < MRS_RETRY_CFG_MAX_NUM; i++)
    {
        P_MRS_RETRY_CFG_S pstCfg = mrsRmGetRetryCfg(i);

        if (pstParam)
        {
            mrsRmRetryCfgInit(pstCfg, pstParam + i);
        }
        else
        {
            mrsRmRetryCfgInit(pstCfg, HI_NULL);
        }
    }
}

// ��ʼ��������������
HI_VOID mrsRmCfgParallelInit(NV_MRS_PARALLEL_CFG_S *pstParam)
{
    HI_S32 i = 0;

    for(i = 0; i < MRS_RM_POOL_MAX_NUM; i++)
    {
        P_MRS_RM_REQ_POOL pstPool = mrsRmGetPool((HI_U8)i);
        P_MRS_PARALLEL_CFG_S pstCfg = &pstPool->stParallelCfg;

        if (pstParam)
        {
            mrsRmParallelCfgInit(pstPool, pstCfg, pstParam + i);
        }
        else
        {
            mrsRmParallelCfgInit(pstPool, pstCfg, HI_NULL);
        }
    }
}
/* END:   PN: DTS2015033100426 MODIFY\ADD\DEL by cuiate/00233580 at 2015/3/26 */

// ���������ҵ��������Ͳ��Ҷ�Ӧ�Ĳ���
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


// ����һ�����������ڵ�
HI_PRV P_MRS_RM_NODE mrsRmAddNode(P_MRS_RM_REQ_POOL pstPool, HI_U8 *ucMac)
{
    P_MRS_RM_NODE pstNode = HI_NULL;

    pstNode = (P_MRS_RM_NODE)mrsToolsMalloc(sizeof(MRS_RM_NODE));
    if(pstNode != HI_NULL)
    {
        (hi_void)memset_s(pstNode, sizeof(MRS_RM_NODE), 0, sizeof(MRS_RM_NODE));

        HI_DIAG_LOG_BUF(__LINE__, HI_DIAG_MT("�����ڵ㲢��������"), ucMac, HI_PLC_MAC_ADDR_LEN);

        (hi_void)memcpy_s(pstNode->ucDestMac, HI_PLC_MAC_ADDR_LEN, ucMac, HI_PLC_MAC_ADDR_LEN);
        mrsSrvInitQueue(&pstNode->stItemQueue);

        pstNode->ucState = MRS_RM_NODE_UNTREATED;

        // ���뻺�����������β��
        mrsSrvListInsTail(&pstPool->stNodeList, &pstNode->link);

        return pstNode;
    }
    else
    {
        return HI_NULL;
    }
}


// ����һ������غ���Ԫ��
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

        HI_DIAG_LOG_BUF(__LINE__, HI_DIAG_MT("�ڽڵ�����м����������"), pstData->ucAddr, HI_METER_ADDR_LEN);

        (hi_void)memcpy_s(pstItem->ucMeterAddr, HI_METER_ADDR_LEN, pstData->ucAddr, HI_METER_ADDR_LEN);
        pstItem->ucMrSeq        = pstData->ucMrSeq;
        pstItem->ucStrategyNo   = ucStrategyNo;
        pstItem->ucProtocol     = pstData->ucProtocol;
        pstItem->ucFrameNum     = pst645Inf->ucNum;
        pstItem->bCtrlCmd       = pst645Inf->bCtrlCmd;
        pstItem->usDataLen      = pstData->usDataLen;
        pstItem->ulDataCrc      = pst645Inf->ulCrc;
        (hi_void)memcpy_s(pstItem->pucData, pstItem->usDataLen, pstData->pucData, pstData->usDataLen);

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
        pstItem->pvParam        = pstData->pvParam;
#endif

        mrsSrvEnQueue(&pstNode->stItemQueue, &pstItem->link);

        pstNode->ucItemNum++;

        return HI_ERR_SUCCESS;
    }
    else
    {
        return HI_ERR_NO_MORE_MEMORY;
    }
}


// ��������Ԫ�س�����(�ڵ㴦��ʱ��������Ӧ������)
HI_PRV HI_U32 mrsRmItemDeQueue(P_MRS_RM_NODE pstNode, HI_BOOL bSwitchFlg)
{
    P_MRS_RM_ITEM pstItem = HI_NULL;

    pstItem = mrsSrvDeQueue(&pstNode->stItemQueue);
    if(pstItem)
    {
        HI_DIAG_LOG_BUF(__LINE__, HI_DIAG_MT("�������������"), pstItem->ucMeterAddr, HI_METER_ADDR_LEN);

        pstNode->ucItemNum--;
        mrsToolsFree(pstItem);
    }

    // ��λ�ڵ�������(�ͷű����PLC��������;�������������)
    mrsRmNodeCtxReset(pstNode);

    // ��ʼ���ڵ�������(������в�Ϊ�������������һ����������)
    return mrsRmNodeCtxInit(pstNode, bSwitchFlg);
}


// �ڻ�����в���ָ��MAC��ַ��Ӧ�Ľڵ�(δ�ҵ�����HI_NULL)
HI_PRV P_MRS_RM_NODE mrsRmFindNode(P_MRS_RM_REQ_POOL pstPool, HI_U8 *ucMac)
{
    MRS_SRV_LINK * it;
    MRS_SRV_LIST * list;
    P_MRS_RM_NODE node = HI_NULL;

    list = &pstPool->stNodeList;

    // ��������
    for (it = list->next; it != list; it = it->next)
    {
        node = (P_MRS_RM_NODE)it;

        if(mrsToolsMemEq(node->ucDestMac, ucMac, HI_PLC_MAC_ADDR_LEN))
        {
            HI_DIAG_LOG_BUF(__LINE__, HI_DIAG_MT("���������ҵ��˶�Ӧ�ڵ�"), ucMac, HI_PLC_MAC_ADDR_LEN);
            return node;
        }
    }

    return HI_NULL;
}


// �ڻ���ص�һ�ڵ�����в����Ѵ��ڵĶ�������
// ucOpt:
// MRS_RM_ADDR_MATCH ʹ�ñ��ַƥ�����
// MRS_RM_CRC_MATCH  ʹ��CRCֵƥ�����
HI_PRV HI_BOOL mrsRmFindReqOfQueue(MRS_SRV_QUEUE *pstQueue, HI_U32 ulID, HI_U8 ucOpt)
{
    MRS_SRV_NODE * it;
    P_MRS_RM_ITEM item = HI_NULL;

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

    return HI_FALSE;
}

// �ڻ�����в����Ѵ��ڵĶ�������(�ҵ��󷵻�HI_TRUE; ���򷵻�HI_FALSE;)
// ucOpt:
// MRS_RM_ADDR_MATCH ʹ�ñ��ַƥ�����
// MRS_RM_CRC_MATCH  ʹ��CRCֵƥ�����
HI_PRV HI_BOOL mrsRmFindReqOfPool(P_MRS_RM_REQ_POOL pstPool, HI_U32 ulID, HI_U8 ucOpt)
{
    MRS_SRV_LINK * it;
    MRS_SRV_LIST * list;
    P_MRS_RM_NODE node = HI_NULL;

    list = &pstPool->stNodeList;

    // ��������
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


// �ͷŻ�����ж�������Ԫ��
HI_PRVL HI_VOID mrsRmQueueFree(HI_VOID * p)
{
    mrsToolsFree(p);
    return;
}


// �ͷŽڵ�
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


// ��ջ����
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


// �Ƴ��ڵ�(�ڻ�����н�ָ���ڵ��Ƴ�)
HI_PRV HI_VOID mrsRmRemoveNode(MRS_SRV_LIST *pstList, P_MRS_RM_NODE pstNode)
{
    HI_U32 ret = HI_ERR_SUCCESS;

    HI_DIAG_LOG_BUF(__LINE__, HI_DIAG_MT("���������Ƴ��ڵ�"), pstNode->ucDestMac, HI_PLC_MAC_ADDR_LEN);

    ret = mrsSrvListRemove(pstList, &pstNode->link);
    if(ret == HI_ERR_SUCCESS)
    {
        mrsRmNodeFree((HI_VOID *)pstNode, HI_NULL);
    }
    else
    {
        // �������
        // TODO:......
    }

    return;
}


// �ڵ������Ĵ���-�״δ���ڵ�ʱʹ��
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
        HI_DIAG_LOG_BUF(__LINE__, HI_DIAG_MT("�ڵ������Ĵ���"), pstNode->ucDestMac, HI_PLC_MAC_ADDR_LEN);

        (hi_void)memset_s(pstNode->pstNodeCtx, sizeof(MRS_NODE_CTX), 0, sizeof(MRS_NODE_CTX));
        pstNode->ucState = MRS_RM_NODE_UNINITIALIZED;
        return HI_ERR_SUCCESS;
    }
    else
    {
        return HI_ERR_NO_MORE_MEMORY;
    }
}


// �ڵ������ĸ�λ(����)-�������һ���ڵ����Ԫ��ʱʹ��
HI_PRV HI_VOID mrsRmNodeCtxReset(P_MRS_RM_NODE pstNode)
{
    P_MRS_NODE_CTX pstNodeCtx = pstNode->pstNodeCtx;

    if(pstNodeCtx)
    {
        HI_DIAG_LOG_BUF(__LINE__, HI_DIAG_MT("�ڵ������ĸ�λ"), pstNode->ucDestMac, HI_PLC_MAC_ADDR_LEN);

        // �ͷ�ת����PLC����
        mrsToolsFree(pstNodeCtx->stPlcData.pucPayload);
        // �ڵ�����������
        (hi_void)memset_s(pstNodeCtx, sizeof(MRS_NODE_CTX), 0, sizeof(MRS_NODE_CTX));

        pstNode->ucState = MRS_RM_NODE_UNINITIALIZED;
    }

    return;
}


// �ڵ��������ͷ�-�ڵ����ж���Ԫ�ش�����Ϻ�ʹ��
HI_PRV HI_VOID mrsRmNodeCtxFree(P_MRS_NODE_CTX pstNodeCtx)
{
    if(pstNodeCtx)
    {
        // �ͷ�ת����PLC����
        mrsToolsFree(pstNodeCtx->stPlcData.pucPayload);
        // �ڵ��������ͷ�
        mrsToolsFree(pstNodeCtx);
    }
    return;
}


// �ڵ������ĳ�ʼ��
HI_PRV HI_U32 mrsRmNodeCtxInit(P_MRS_RM_NODE pstNode, HI_BOOL bSwitchFlg)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    P_MRS_RM_ITEM pstItem = HI_NULL;
    P_MRS_RM_STRATEGY_REG pstStrategy = HI_NULL;
    P_MRS_RETRY_CFG_S pstTryCfg = HI_NULL;

    HI_DIAG_LOG_BUF(__LINE__, HI_DIAG_MT("�ڵ������ĳ�ʼ��"), pstNode->ucDestMac, HI_PLC_MAC_ADDR_LEN);

#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
    pstItem = mrsSrvQueueTop(&pstNode->stItemQueue);
    if(pstItem == HI_NULL)
    {
        HI_DIAG_LOG_BUF(__LINE__, HI_DIAG_MT("�ڵ����Ϊ��"), pstNode->ucDestMac, HI_PLC_MAC_ADDR_LEN);
        return HI_ERR_QUEUE_EMPTY;
    }
#else
    for (;;)
    {
        pstItem = mrsSrvQueueTop(&pstNode->stItemQueue);
        if(pstItem == HI_NULL)
        {
            HI_DIAG_LOG_BUF(__LINE__, HI_DIAG_MT("�ڵ����Ϊ��"), pstNode->ucDestMac, HI_PLC_MAC_ADDR_LEN);
            return HI_ERR_QUEUE_EMPTY;
        }

        if (HI_NULL != pstItem->pvParam)
        {
            break;
        }

        pstItem = mrsSrvDeQueue(&pstNode->stItemQueue);
        pstNode->ucItemNum--;
        mrsToolsFree(pstItem);

        mrsCcoMrTaskNotify();
    }
#endif

    HI_DIAG_LOG_BUF(__LINE__, HI_DIAG_MT("��ʼ���µĶ�������"), pstItem->ucMeterAddr, HI_METER_ADDR_LEN);

    pstStrategy = mrsRmGetStrategy(pstItem->ucStrategyNo);
    pstTryCfg = mrsRmGetRetryCfg(pstStrategy->ucRetryCfgNo);

    // �ش����Ʋ�����ʼ��
    mrsRmNodeRetryParamInit(pstNode, pstTryCfg, HI_FALSE);

    // ����PLC֡
    ret = mrsRmPlcPackCreate(pstNode, pstItem, pstTryCfg);
    if(ret == HI_ERR_SUCCESS)
    {
        HI_DIAG_LOG_MSG_E1(__LINE__, HI_DIAG_MT("����PLC����֡-���"), pstNode->pstNodeCtx->ulPlcSeq);
        if(bSwitchFlg)
        {
            HI_DIAG_LOG_BUF(__LINE__, HI_DIAG_MT("�ڵ�״̬Ϊ-�ȴ�����"), pstNode->ucDestMac, HI_PLC_MAC_ADDR_LEN);
            pstNode->ucState = MRS_RM_NODE_WAIT_SEND;
        }
        else
        {
            pstNode->ucState = MRS_RM_NODE_SEND_REQ;
        }
    }

    return ret;
}


#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
// ��֯����ת������
HI_PRVL HI_VOID mrsRmSetupTransmitData(P_MRS_RETRY_CFG_S pstTryCfg,
                                                P_MRS_RM_ITEM pstItem,
                                                P_MRS_NODE_CTX pstNodeCtx,
                                                MRS_DATA_TRANSMIT_AFNF1_DL_STRU *pstData)
{
    pstData->interface_ver   = MRS_AFN_F1_DL_VER;    // ��ȡ�汾��
    pstData->stru_size       = sizeof(MRS_DATA_TRANSMIT_AFNF1_DL_STRU);
    pstData->noresp_try_flag = (pstTryCfg->MeterRetryCfgNv & 0x01)? HI_TRUE : HI_FALSE; // ��Ӧ�����Ա�־-������ΪNV��
    pstData->nak_try_flag    = (pstTryCfg->MeterRetryCfgNv & 0x02)? HI_TRUE : HI_FALSE; // ����Ӧ�����Ա�־-������ΪNV��
    pstData->try_max   = pstTryCfg->MeterMaxRetryNv;    // ���Դ���(STA�Ե��)
    pstData->protocol  = pstItem->ucProtocol;           // ��ȡ��Լ����
    pstData->data_len  = pstItem->usDataLen;            // ��ȡ��ת�����ĳ���
    pstData->seq       = (HI_U16)pstNodeCtx->ulPlcSeq;  // ��ȡ֡���
    pstData->time_out  = (HI_U8)MRS_MS_TO_100MS(pstNodeCtx->ulOverTimeMeter);  // ��ȡ���ʱʱ��(100ms)
    pstData->gap_timer = (HI_U8)MRS_MS_TO_10MS(pstTryCfg->MeterFrameInterval);  // ֡���ʱ��
    (hi_void)memcpy_s(pstData->data, pstData->data_len, pstItem->pucData, pstItem->usDataLen); // ��ȡ��ת������������
    return;
}


// ����PLC���ݰ�
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
    (hi_void)memset_s(pstData,usLen, 0,usLen);

    pstNodeCtx = pstNode->pstNodeCtx;

    mrsRmPlcSeqAddSelf();
    pstNodeCtx->ulPlcSeq = mrsRmGetPlcSeq();

    // ��֯ת�����ݰ�
    mrsRmSetupTransmitData(pstTryCfg, pstItem, pstNodeCtx, pstData);

    pstPlc = &pstNodeCtx->stPlcData;
    (hi_void)memset_s(pstPlc, sizeof(MRS_PLC_FRAME_DATA_STRU), 0, sizeof(MRS_PLC_FRAME_DATA_STRU));

    // ��֯PLC��
    pstPlc->usId = ID_MRS_CMD_PARALLEL_DATA_PLC;    // ��ȡPLC֡����ID
    (hi_void)memcpy_s(pstPlc->ucMac, HI_PLC_MAC_ADDR_LEN, pstNode->ucDestMac, HI_PLC_MAC_ADDR_LEN);  // ��ȡMac��ַ
    pstPlc->usPayloadLen = usLen;                   // ��ȡת�����ݰ�����
    pstPlc->pucPayload   = (HI_U8 *)pstData;        // ��ȡת�����ݰ�ָ��

    // ���¹㲥֡��־
    pstNodeCtx->ucIsBC = mrsToolsBroadCastMac(pstNode->ucDestMac);

#if defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG)
    mrsLogFrameInfStatTx(pstItem->pucData, pstItem->usDataLen, pstNodeCtx->ucIsBC);
#endif

    return HI_ERR_SUCCESS;
}
#endif


// ��ȡĿ��MACվ������(�㼶;�Ƿ��I�Ͳɼ�����ʱ����)
HI_PRV HI_VOID mrsRmGetDestMacAttr(HI_U8 *pucMac, HI_U8 *pucLayer, HI_BOOL *pbCltIDelay)
{
    // Ĭ��ʹ�ù㲥�㼶
    *pucLayer = MRS_RM_DEFAULT_LAYER_BACST;
    // Ĭ�ϼ�I�Ͳɼ�����ʱ����
    *pbCltIDelay   = HI_TRUE;

    if (mrsToolsBroadCastMac(pucMac))
    {
        // �㲥�������˲���I�ɣ���ʱʱ�䲻��I�ɲ���
        *pbCltIDelay = mrsRmBcTimeoutCltIDelayFlag();
    }
    else
    {
        // �ǹ㲥MAC��ַ
        HI_U32 ret = HI_ERR_SUCCESS;
        HI_MAC_NETWORK_TOPO_ENTRY_S stAttr;

        (hi_void)memset_s(&stAttr, sizeof(stAttr), 0, sizeof(stAttr));
        ret = HI_MDM_QueryMacAttr(pucMac, &stAttr);
        if (ret == HI_ERR_SUCCESS)
        {
            // ��ȡPLC�㼶
            *pucLayer = stAttr.level;
            MRS_DATA_BOUNDARY_CHECK(*pucLayer, MRS_RM_MAC_MAX_LAYER, MRS_RM_MAC_MIN_LAYER);

            // �����I�Ͳɼ��������I�ɳ�ʱ����
            *pbCltIDelay = (stAttr.product_type == HI_ND_TYPE_CLTI)? HI_TRUE : HI_FALSE;
        }
    }

    return;
}

// �ڵ��ش�������ʼ��-(������Դ���;��ʱʱ��)
HI_PRV HI_VOID mrsRmNodeRetryParamInit(P_MRS_RM_NODE pstNode,
                                                P_MRS_RETRY_CFG_S pstTryCfg,
                                                HI_BOOL bForceBc)
{
    P_MRS_RM_ITEM pstItem     = HI_NULL;
    P_MRS_NODE_CTX pstNodeCtx = HI_NULL;
    HI_U8   ucLayer = MRS_RM_DEFAULT_LAYER_BACST;
    HI_BOOL bCltIDelay   = HI_TRUE;  // �Ƿ��I�ɲ���
    HI_U32 ulPlcDelay   = 0;
    HI_U32 ulMeterTotal = 0; // ��֡����µ���ܳ�ʱʱ��

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

    // ��ȡĿ��MACվ������
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

    // ����PLC��·����ʱ��
    ulPlcDelay = (HI_U16)pstTryCfg->PlcLayerDelayNv * ucLayer  // �㼶��ʱ
               + (HI_U16)pstTryCfg->PlcRevisoryNv;             // ����ʱ��

    // ����STA����֮�䵥��ͨѶ�ĳ�ʱʱ��
    pstNodeCtx->ulOverTimeMeter = (HI_U16)pstTryCfg->MeterOverTimeNv                     // ���ʱ
                                + (HI_U16)pstTryCfg->CltIDelayNv * bCltIDelay            // I�Ͳɼ�������
                                + (HI_U16)pstTryCfg->CtrlCmdDelayNv * pstItem->bCtrlCmd; // �ѿ������

    // ����STA�������ܺ�ʱ
    ulMeterTotal = pstNodeCtx->ulOverTimeMeter * pstItem->ucFrameNum
                 + pstTryCfg->MeterFrameInterval * (pstItem->ucFrameNum - 1);    // STA��֡�������

    // ��������ش�������Ч-�����ش���ʱ
    if((pstTryCfg->MeterRetryCfgNv != 0)
        && (pstItem->ucFrameNum <= MRS_CHECK_METER_FAULT_FRAME_NUM))
    {
        ulMeterTotal = ulMeterTotal * (1 + pstTryCfg->MeterMaxRetryNv);
    }

    // ����CCO�൥��PLCͨѶ�ܳ�ʱʱ��
    pstNodeCtx->ulOverTimeTotal = ulPlcDelay    // PLCʱ��
                                + ulMeterTotal  // STA�˵��ʱ
                                + MRS_RM_CHECK_TIMER_UNIT_BASE / 2;   // ������ʱ��ƫ��

    return;
}


// ����PLC���ݰ�
HI_PRV HI_U32 mrsRmSendPlcPackage(P_MRS_NODE_CTX pstNodeCtx)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U8 mac[HI_PLC_MAC_ADDR_LEN] = {0};

    if(!pstNodeCtx)
    {
        return HI_ERR_BAD_DATA;
    }

    if (pstNodeCtx->ucIsBC)
    {
        (hi_void)memcpy_s(mac, HI_PLC_MAC_ADDR_LEN, pstNodeCtx->stPlcData.ucMac, HI_PLC_MAC_ADDR_LEN);
        (hi_void)memset_s(pstNodeCtx->stPlcData.ucMac, HI_PLC_MAC_ADDR_LEN, 0xFF, HI_PLC_MAC_ADDR_LEN);
    }

    ret = MRS_SendPlcFrame(&pstNodeCtx->stPlcData);
    mrsDfxPrPlcTx();

    {
        MRS_DATA_TRANSMIT_AFNF1_DL_STRU *pstAfnF1 = (MRS_DATA_TRANSMIT_AFNF1_DL_STRU*)(pstNodeCtx->stPlcData.pucPayload);
        mrsDfxRmPlcTx((HI_U8*)pstAfnF1 + sizeof(MRS_DATA_TRANSMIT_AFNF1_DL_STRU), pstAfnF1->data_len, ret);
    }

    if(pstNodeCtx->ucIsBC)
    {
        (hi_void)memcpy_s(pstNodeCtx->stPlcData.ucMac, HI_PLC_MAC_ADDR_LEN, mac, HI_PLC_MAC_ADDR_LEN);
    }

    return ret;
}


// �ж��Ƿ�������뻺���(����HI_ERR_SUCCESS�������;�������������)
HI_PRV HI_U32 mrsRmAllowJoinPool(P_MRS_RM_REQ_POOL pstPool,
                                        P_MRS_RM_REQ_DATA pstData,
                                        MRS_645BUF_INF *pst645Inf)
{
    P_MRS_PARALLEL_CFG_S pstCfg = HI_NULL;
    HI_U32 ulID  = 0;

    pstCfg = &pstPool->stParallelCfg;

    // ����ض�������Ԫ���Ѵﵽ���ֵ-���������
    if(pstPool->usItemTotalNum >= pstCfg->usMaxTotalItem)
    {
        HI_DIAG_LOG_MSG_E0(__LINE__, HI_DIAG_MT("�������-�ܾ�����"));
        return HI_ERR_FULL;
    }

    // ��ȡҵ�����������645֡��Ϣ
    mrsCountBuf645Inf(pstData->pucData, pstData->usDataLen, pst645Inf);

    // ҵ�����������û��645֡-���������
    if(pst645Inf->ucNum == 0)
    {
        HI_DIAG_LOG_MSG_E0(__LINE__, HI_DIAG_MT("��Ч645֡��Ϊ0"));

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
        pst645Inf->pucAddr = pstData->ucAddr;
        pst645Inf->pucHead = pstData->pucData;
        pst645Inf->ulCrc = HI_MDM_Crc32(0, (HI_CONST HI_U8 *)pstData->pucData, (HI_U32)pstData->usDataLen);
        pst645Inf->ucNum = 1;
        pst645Inf->bCtrlCmd = HI_FALSE;
#else
        return HI_ERR_BAD_DATA;
#endif
    }

    // ҵ����������г������֡������-���������
    if(pst645Inf->ucNum > pstCfg->usMaxFrame)
    {
        HI_DIAG_LOG_MSG_E1(__LINE__, HI_DIAG_MT("��������֡��"), pst645Inf->ucNum);
        return HI_ERR_NOT_SUPPORT;
    }

    HI_DIAG_LOG_MSG_E1(__LINE__, HI_DIAG_MT("��ǰ��������CRCУ��"), pst645Inf->ulCrc);

    // ���ҵ�����û���ṩ��ַ��ʹ��645֡��ַ
    if(pstData->ucAddr == HI_NULL)
    {
        pstData->ucAddr = pst645Inf->pucAddr;
    }

    if(pstCfg->ucCheckRepeatMode == MRS_RM_ADDR_MATCH)
    {
        // ʹ�õ���ַ��Ϊ�ظ�ƥ����
        ulID = (HI_U32)pstData;
    }
    else
    {
        // ʹ��CRC��Ϊ�ظ�ƥ����
        ulID = pst645Inf->ulCrc;
    }

    // �����������Ѵ��ڴ˶�������-���������
    if(mrsRmFindReqOfPool(pstPool, ulID, pstCfg->ucCheckRepeatMode))
    {
        HI_DIAG_LOG_BUF(__LINE__, HI_DIAG_MT("�˵�����ڳ�����"), pstData->ucAddr, HI_METER_ADDR_LEN);
        return HI_ERR_BUSY;
    }

    return HI_ERR_SUCCESS;
}


// ����ʹ���
HI_PRV HI_U32 mrsRmSendReqProcActive(P_MRS_RM_REQ_POOL pstPool)
{
    HI_SYS_QUEUE_MSG_S stMsg = {0};

    (hi_void)memset_s(&stMsg, sizeof(stMsg), 0, sizeof(stMsg));

    stMsg.ulMsgId = EN_MRS_FW_MSG_RM_PLC_SEND_REQ;
    stMsg.ulParam[0] = (HI_U32)pstPool;

    HI_DIAG_LOG_MSG_E0(__LINE__, HI_DIAG_MT("�����������..."));

    return mrsSendMessage2Queue(&stMsg);
}


// �ڵ�״̬����(����HI_TRUE����PLC;����HI_FALSE��������)
// �˺�����ÿ��if��֧���п�����һ�κ�������ʱִ�е�,�����������ǰ��˳��
HI_PRV HI_BOOL mrsRmNodeStatusProc(MRS_SRV_LIST *list, P_MRS_RM_NODE node)
{
    HI_U32 ret = HI_ERR_SUCCESS;

    // �����"������"״̬-ֱ�ӷ��ط�������
    if(node->ucState == MRS_RM_NODE_SEND_REQ)
    {
        return HI_TRUE;
    }

    // ���Ϊ"δ����"״̬-�����ڵ�������
    if(node->ucState == MRS_RM_NODE_UNTREATED)
    {
        ret = mrsRmNodeCtxCreate(node);
        if(ret != HI_ERR_SUCCESS)
        {
            // �ڵ������Ĵ���ʧ��-���ز�������
            return HI_FALSE;
        }
    }

    // ����ǽڵ�������"δ��ʼ��"״̬-��ʼ���ڵ�������
    if(node->ucState == MRS_RM_NODE_UNINITIALIZED)
    {
        ret = mrsRmNodeCtxInit(node, HI_FALSE);
        if(ret == HI_ERR_SUCCESS)
        {
            // �ڵ������ĳ�ʼ���ɹ�-���ط�������
            return HI_TRUE;
        }
        else if(ret == HI_ERR_QUEUE_EMPTY)
        {
            // �ڵ����Ϊ��-�Ƴ��ڵ�
            mrsRmRemoveNode(list, node);
        }
    }

    return HI_FALSE;
}


// ��PLCӦ������ݻش���ҵ�����(PLC����δ��Ӧʱ��pucData�ÿ�;usLen����)
HI_PRV HI_U32 mrsRmReturnRespData(P_MRS_RM_STRATEGY_REG pstStrategy,
                                            P_MRS_RM_ITEM pstItem,
                                            HI_U8 *pucData,
                                            HI_U16 usLen)
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

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
        stData.pvParam = pstItem->pvParam;
#endif

        // ����ҵ�����PLCӦ������
        ret = pstStrategy->pfnRespProc(&stData);

        mrsDfxRmUartTx(pucData, usLen, HI_FALSE);
    }

    return ret;
}


// PLC�ش�����(����1-�����ڵ㳭"AFN=13H-F1")��Ŀǰδʹ��
HI_PRV HI_U32 mrsRmPlcRetryProcStrategy01(P_MRS_RM_NODE pstNode, P_MRS_RETRY_CFG_S pstTryCfg)
{
    MRS_NOT_USED(pstNode);
    MRS_NOT_USED(pstTryCfg);

    // TODO:.....

    // ����Ҫ�ش�
    return HI_ERR_SUCCESS;
}


// PLC�ش�����(����2-���������ֳ�������������ת��)��Ŀǰֻ���ڲ���
HI_PRV HI_U32 mrsRmPlcRetryProcStrategy02(P_MRS_RM_NODE pstNode, P_MRS_RETRY_CFG_S pstTryCfg)
{
    P_MRS_NODE_CTX pstNodeCtx = pstNode->pstNodeCtx;

    // ������ǹ㲥����
    if(pstNodeCtx->ucIsBC == HI_FALSE)
    {
        // �ж��Ƿ���Ҫ�л�Ϊ�㲥��ʽ����
        if((pstNodeCtx->ucTry >= pstNodeCtx->ucMaxTry)
            && (pstTryCfg->PlcMaxRetryNvB))
        {
            HI_DIAG_LOG_MSG_E0(__LINE__, HI_DIAG_MT("����ǿ���л�Ϊ�㲥"));

            pstNodeCtx->ucTry = 0;
            pstNodeCtx->ucIsBC = HI_TRUE;

            // ���³�ʼ���ش�����(ʹ��ǿ�ƹ㲥)
            mrsRmNodeRetryParamInit(pstNode, pstTryCfg, HI_TRUE);
        }
    }

    // �ж����޳���������Դ���
    if(pstNodeCtx->ucTry < pstNodeCtx->ucMaxTry)
    {
        mrsDfxPrPlcTryCnt();    // ͳ��PLC�ط�����

        pstNodeCtx->ucTry++;
        pstNode->ucState = MRS_RM_NODE_SEND_REQ;

        // ��Ҫ�ش�
        return HI_ERR_CONTINUE;
    }

    // ����Ҫ�ش�
    return HI_ERR_SUCCESS;
}


// PLC��ʱ����
HI_PRV HI_U32 mrsRmPlcOverTimeProc(P_MRS_RM_REQ_POOL pstPool, P_MRS_RM_NODE pstNode)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    P_MRS_RM_ITEM pstItem  = HI_NULL;
    P_MRS_RM_STRATEGY_REG pstStrategy = HI_NULL;
    P_MRS_RETRY_CFG_S pstTryCfg   = HI_NULL;
    P_MRS_NODE_CTX pstNodeCtx     = pstNode->pstNodeCtx;

    mrsDfxPrPlcTimeout();
    mrsDfxRmPlcDlRx(ID_MRS_CMD_PARALLEL_DATA_PLC, pstNodeCtx->stPlcData.pucPayload, HI_TRUE);

    pstNodeCtx->ulRunOutTime = 0;

    // ��ǰ�������Լ�
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

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
    if (!mrsCcoMrTaskIsItemTimeout((MRS_CCO_MR_TASK_ITEM_STRU *)pstItem->pvParam))
#endif
    {
        if(pstStrategy->ucRetryStrategy == EN_MRS_RM_RETRY_STRATEGY_01)
        {
            ret = mrsRmPlcRetryProcStrategy01(pstNode, pstTryCfg);
        }
        else
        {
            ret = mrsRmPlcRetryProcStrategy02(pstNode, pstTryCfg);
        }
    }

    if(ret == HI_ERR_CONTINUE)
    {
        // ��Ҫ�ش�
        HI_DIAG_LOG_BUF(__LINE__, HI_DIAG_MT("PLC֡��Ҫ�ش�"), pstItem->ucMeterAddr, HI_METER_ADDR_LEN);
        return ret;
    }

    HI_DIAG_LOG_BUF(__LINE__, HI_DIAG_MT("PLC����ʧ��"), pstItem->ucMeterAddr, HI_METER_ADDR_LEN);
    mrsDfxPrFailCnt();
#if defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG)
    mrsLogPlcOverTimeStat(pstItem->pucData, pstItem->usDataLen, 0, HI_NULL);
#endif

    // ֪ͨҵ�����PLC��ʱ
    mrsRmReturnRespData(pstStrategy, pstItem, HI_NULL, 0);
    mrsDetectPrRmTimeout(pstItem->ucMeterAddr);
    mrsDecreaseMrTTL(&(mrsCcoGetContext()->stMapModule), pstItem->ucMeterAddr);

    // �ܶ�����������Լ�
    pstPool->usItemTotalNum--;
    mrsDfxRmItemNum((HI_U32)pstPool, pstPool->usItemTotalNum);

    // �������������(������в�Ϊ�����ʼ����������)
    return mrsRmItemDeQueue(pstNode, HI_FALSE);
}


// PLC��ʱ���
HI_PRV HI_U32 mrsRmPlcOverTimeCheck(P_MRS_RM_REQ_POOL pstPool,
                                                P_MRS_RM_NODE pstNode,
                                                HI_BOOL *bOverTime)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    P_MRS_NODE_CTX pstNodeCtx = pstNode->pstNodeCtx;
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
    P_MRS_RM_ITEM pstItem = mrsSrvQueueTop(&pstNode->stItemQueue);

    if (!pstItem)
    {
        return HI_ERR_QUEUE_EMPTY;
    }
#endif

    pstNodeCtx->ulRunOutTime += MRS_RM_CHECK_TIMER_UNIT_BASE;

    // �����ڵ㳬ʱ�ж�
    if((pstNodeCtx->ulRunOutTime >= pstNodeCtx->ulOverTimeTotal)
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
        || mrsCcoMrTaskIsItemTimeout((MRS_CCO_MR_TASK_ITEM_STRU *)pstItem->pvParam)
#endif
        )
    {
        *bOverTime = HI_TRUE;

        HI_DIAG_LOG_BUF(__LINE__, HI_DIAG_MT("����PLC��ʱ-MAC��ַ"), pstNode->ucDestMac, HI_PLC_MAC_ADDR_LEN);

        // �����ڵ㳬ʱ����
        ret = mrsRmPlcOverTimeProc(pstPool, pstNode);

        if(ret == HI_ERR_QUEUE_EMPTY)
        {
            mrsRmRemoveNode(&pstPool->stNodeList, pstNode);
        }

        HI_DIAG_LOG_MSG_E2(__LINE__, HI_DIAG_MT("��ǰ������-��ǰ����������"), pstPool->usParallelNum, pstPool->usItemTotalNum);
    }

    return ret;
}


// PLC���͵ȴ����ڼ��
HI_PRV HI_U32 mrsRmPlcSendWaitCheck(P_MRS_RM_REQ_POOL pstPool,
                                                P_MRS_RM_NODE pstNode,
                                                HI_U32 *pulWaitSendNum,
                                                HI_BOOL *bOverTime)
{
    P_MRS_NODE_CTX pstNodeCtx = HI_NULL;

    pstNodeCtx = pstNode->pstNodeCtx;

    pstNodeCtx->ulWaitSendTime += MRS_RM_CHECK_TIMER_UNIT_BASE;

    // ����л�����ж�
    if(pstNodeCtx->ulWaitSendTime >= (HI_U32)(pstPool->stParallelCfg.ucMeterSwitchInterval * MRS_RM_METER_SWITCH_TIME_UNIT))
    {
        HI_DIAG_LOG_BUF(__LINE__, HI_DIAG_MT("�ڵ�״̬�л�Ϊ-������"), pstNode->ucDestMac, HI_PLC_MAC_ADDR_LEN);

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


#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
// �����PLC���մ���
HI_PRV HI_U32 mrsRmPoolPlcRxProc(P_MRS_RM_REQ_POOL pstPool, HI_U8 *pucData,
                                        MRS_DATA_TRANSMIT_AFNF1_UP_STRU *pstPackage, HI_BOOL *bFind)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    MRS_SRV_LINK *it, *next;
    MRS_SRV_LIST *list;

    list = &pstPool->stNodeList;

    it = list->next;
    while(it != list)   // ��������
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
                HI_DIAG_LOG_MSG_E0(__LINE__, HI_DIAG_MT("�㲥����Ӧ��ɹ�-�����ģʽ�޸�Ϊ[��ͨ]"));
                mrsRmSetPoolMode(pstPool, MRS_RM_NORMAL_MODE);
            }

            HI_DIAG_LOG_MSG_E2(__LINE__, HI_DIAG_MT("����PLCӦ�����ݰ�-[������][������]"), pstPool->usParallelNum, pstPool->usItemTotalNum);

            ret = mrsRmPlcRespProc(node, pucData, pstPackage);
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

    return ret;
}
#endif


// ����س�ʱ��鶨ʱ������
HI_PRV HI_U32 mrsRmPoolCheckTimerProc(P_MRS_RM_REQ_POOL pstPool,
                                                HI_U32 *pulPlcNum, HI_U32 *pulWaitSendNum)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    MRS_SRV_LINK *it, *next;
    MRS_SRV_LIST *list;
    HI_BOOL bOverTime = HI_FALSE;

    list = &pstPool->stNodeList;

    it = list->next;
    while(it != list)   // ��������
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

    // �������д���δ������Ķ�������
    if(bOverTime)
    {
        if(pstPool->usItemTotalNum)
        {
            mrsRmSendReqProcActive(pstPool);
        }
        else
        {
            HI_MDM_SetTfNtbSendProcessCtrl(HI_MDM_NTB_SEND_PROCESS_RESUME, HI_MDM_NTB_SEND_CTRL_REASON_READ_METER);
        }
    }

    return ret;
}


#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
// Э������Դ���
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


// PLC֡Ӧ����
HI_PRV HI_U32 mrsRmPlcRespProc(P_MRS_RM_NODE pstNode, HI_U8 *pucData,
                                        MRS_DATA_TRANSMIT_AFNF1_UP_STRU *pstPackage)
{
    P_MRS_RM_ITEM pstItem  = HI_NULL;
    P_MRS_RM_STRATEGY_REG pstStrategy = HI_NULL;

    pstItem = mrsSrvQueueTop(&pstNode->stItemQueue);
    if(pstItem == HI_NULL)
    {
        return HI_ERR_QUEUE_EMPTY;
    }

    //��̽�Ⳮ���б����г����ɹ����̽���־���
    mrsDetectRmRespProc(pstItem->ucMeterAddr);

    pstStrategy = mrsRmGetStrategy(pstItem->ucStrategyNo);

    mrsDfxPrOkCnt();
#if defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG)
    mrsLogFrameInfStatRx(pucData, pstPackage->data_len,
                            pstItem->pucData, pstItem->usDataLen,
                            pstPackage->resp_status);
#endif

    // ���Ͷ���Ӧ�����ݵ�ҵ�����
    mrsRmReturnRespData(pstStrategy, pstItem, pucData, pstPackage->data_len);

    // �������������(������в�Ϊ�����ʼ����������)
    return mrsRmItemDeQueue(pstNode, HI_TRUE);
}
#endif


// �������ģ���ʼ��
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

    return HI_ERR_SUCCESS;
}


// ҵ��������ע��
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


// ����������뻺���
HI_U32 mrsRmReqJoinPool(P_MRS_RM_REQ_DATA pstData)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    P_MRS_RM_STRATEGY_REG pstStrategy = HI_NULL;
    P_MRS_RM_REQ_POOL pstPool = HI_NULL;
    MRS_645BUF_INF st645Inf = {0};
    P_MRS_RM_NODE pstNode = HI_NULL;
    HI_U8 ucMac[HI_PLC_MAC_ADDR_LEN] = {0};
    HI_U8 ucStrategyNo = 0;
    HI_U8 aucMeter[HI_METER_ADDR_LEN] = {0};

    do
    {
        ret = mrsRmFindStrategy(pstData->usRmType, &pstStrategy, &ucStrategyNo);
        if(HI_ERR_SUCCESS != ret)
        {
            break;
        }

        pstPool = mrsRmGetPool(pstStrategy->ucPoolNo);

        // �ж��Ƿ���Լ��뻺���
        ret = mrsRmAllowJoinPool(pstPool, pstData, &st645Inf);
        if(ret != HI_ERR_SUCCESS)
        {
            break;
        }

        mrsMeterAddrPreProc(aucMeter, pstData->ucAddr, pstData->ucProtocol);

        // ����ӳ���ϵ
        ret = mrsMapGetMAC(aucMeter, ucMac);
        if(ret != HI_ERR_SUCCESS)
        {
            (hi_void)memset_s(ucMac, HI_PLC_MAC_ADDR_LEN, 0xFF, HI_PLC_MAC_ADDR_LEN);   // û���ҵ���Ӧ��MAC��ַ(ʹ�ù㲥MAC)
            if ((HI_MDM_CHECK_MODE == mrsGetCheckMode())
                || (!mrsGetCcoPlcChlStatus() && (HI_ERR_SUCCESS == mrsCheckMeterNum())))
            {
                mrsGetMacFromTopo(aucMeter, ucMac);
            }
        }

        // �ڻ�����в��Ҷ�ӦMAC��ַ�Ľڵ�(�������������)
        pstNode = mrsRmFindNode(pstPool, ucMac);
        if(pstNode == HI_NULL)
        {
            // δ�ҵ���Ӧ�ڵ�-��Ӷ�ӦMAC��ַ���½ڵ㵽�����(�������������)
            pstNode = mrsRmAddNode(pstPool, ucMac);

            // �¼���ڵ�-�����������
            mrsRmSendReqProcActive(pstPool);
        }

        // ���ҵ��Ľڵ��м��뱾�ζ�������Ԫ��(����غ������)
        ret = mrsRmAddItem(pstNode, pstData, ucStrategyNo, &st645Inf);
        if(ret == HI_ERR_SUCCESS)
        {
            pstPool->usItemTotalNum++;
            mrsDfxRmItemNum((HI_U32)pstPool, pstPool->usItemTotalNum);
            HI_DIAG_LOG_MSG_E1(__LINE__, HI_DIAG_MT("������뻺��سɹ�"), pstPool->usItemTotalNum);
        }
    }while(0);

    if(ret != HI_ERR_SUCCESS)
    {
        mrsDfxRmJoinFail((HI_U32)pstPool, ret);
    }

    return ret;
}

HI_U32 mrsGetMacFromTopo(HI_U8 *pucMeterAddr, HI_U8 *pucMacAddr)
{
    HI_MAC_NETWORK_TOPO_ENTRY_S stEntry;
    HI_U8 aucTemp[HI_PLC_MAC_ADDR_LEN] = {0};

    (hi_void)memset_s(&stEntry, sizeof(stEntry), 0, sizeof(stEntry));
    mrsToolsConvertMeter2Mac(pucMeterAddr, MRS_PROTO645_VER_2007, aucTemp);

    if (HI_ERR_SUCCESS == HI_MDM_QueryMacAttr(aucTemp, &stEntry))
    {
        (hi_void)memcpy_s(pucMacAddr, HI_PLC_MAC_ADDR_LEN, stEntry.mac, HI_PLC_MAC_ADDR_LEN);
        return HI_ERR_SUCCESS;
    }

    return HI_ERR_NOT_FOUND;
}

// PLC����������
HI_U32 mrsRmPlcSendReqProc(HI_SYS_QUEUE_MSG_S* pstMsg)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    P_MRS_RM_REQ_POOL pstPool   = (P_MRS_RM_REQ_POOL)pstMsg->ulParam[0];
    P_MRS_PARALLEL_CFG_S pstCfg = &pstPool->stParallelCfg;
    MRS_SRV_LINK *it, *next;
    MRS_SRV_LIST *list;

    HI_DIAG_LOG_MSG_E0(__LINE__, HI_DIAG_MT("����������..."));

    if(pstPool->ucMode != MRS_RM_NORMAL_MODE)
    {
        return HI_ERR_SUCCESS;
    }

    list = &pstPool->stNodeList;

    it = list->next;
    while(it != list)    // ��������
    {
        P_MRS_RM_NODE node = HI_NULL;
        P_MRS_NODE_CTX pstNodeCtx = HI_NULL;

        next = it->next;
        node = (P_MRS_RM_NODE)it;

        if(pstPool->usParallelNum >= pstCfg->usMaxParallelNum)
        {
            HI_DIAG_LOG_MSG_E1(__LINE__, HI_DIAG_MT("������󲢷�����-��ͣ����"), pstPool->usParallelNum);
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

            HI_DIAG_LOG_MSG_E1(__LINE__, HI_DIAG_MT("����PLC֡�ɹ�"), pstPool->usParallelNum);
            mrsDfxRmPalallelNum((HI_U32)pstPool, pstPool->usParallelNum);

            if(pstNodeCtx->ucIsBC)
            {
                HI_DIAG_LOG_MSG_E0(__LINE__, HI_DIAG_MT("���͹㲥֡-�����ģʽ���Ϊ[�㲥]"));
                mrsRmSetPoolMode(pstPool, MRS_RM_BROADCAST_MODE);
                mrsDfxBcRmCnt();
                break;
            }
        }

        it = next;
    }

    if(mrsRmGetTimerStatus() == MRS_RM_TIMER_INACTIVE)
    {
        HI_DIAG_LOG_MSG_E0(__LINE__, HI_DIAG_MT("������ʱ��鶨ʱ��"));
        MRS_StartTimer(EN_MRS_SRV_CCO_RM_CHECK_TIMER,
                            MRS_CCO_TIME_PARALLEL_CHECK,
                            HI_SYS_TIMER_PERIODIC);

        mrsRmSetTimerStatus(MRS_RM_TIMER_ACTIVE);
    }

    return HI_ERR_SUCCESS;
}


#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
// PLC������
HI_U32 mrsRmPlcPackageReceive(MRS_DATA_TRANSMIT_AFNF1_UP_STRU *pstPackage, HI_U16 usLen)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U8 *pucData = HI_NULL;
    HI_U8 ucPoolNo = 0;
    HI_BOOL find = HI_FALSE;

    if(!pstPackage)
    {
        return HI_ERR_FAILURE;
    }

    ret = mrsRmPlcUpCompatibleProc(pstPackage, usLen, &pucData);
    if(ret != HI_ERR_SUCCESS)
    {
        return ret;
    }

    HI_DIAG_LOG_MSG_E1(__LINE__, HI_DIAG_MT("����PLC���ݰ�-���"), pstPackage->seq);

    for(ucPoolNo = 0; ucPoolNo < MRS_RM_POOL_MAX_NUM; ucPoolNo++)
    {
        P_MRS_RM_REQ_POOL pstPool = mrsRmGetPool(ucPoolNo);
        mrsRmPoolPlcRxProc(pstPool, pucData, pstPackage, &find);
        if(find)
        {
            break;
        }
    }

    if(find == HI_FALSE)
    {
        HI_DIAG_LOG_MSG_E1(__LINE__, HI_DIAG_MT("����PLC���ݰ�-֡��Ŵ���"), pstPackage->seq);
        mrsDfxPrPlcSeqErr();    // ͳ�ƶ�����PLC֡(���ڵ�)

    /* BEGIN: Modified by fengxiaomin/00209182, 2014/3/27   ���ⵥ��:DTS2014032906596 */
        mrsDfxRmPlcUpRx(ID_MRS_CMD_PARALLEL_DATA_PLC, (HI_U8*)pstPackage, HI_TRUE);
    /* END:   Modified by fengxiaomin/00209182, 2014/3/27 */
        ret = HI_ERR_FAILURE;
    }

    return ret;
}
#endif


// ����ģ�鳬ʱ��鶨ʱ������
HI_VOID mrsRmCheckTimerProc(HI_VOID)
{
    HI_U32 ulLinePlcNum = 0;    // ��ǰ��·�ϵ�PLC֡������
    HI_U32 ulWaitSendNum = 0;
    HI_U8 ucPoolNo = 0;

    for(ucPoolNo = 0; ucPoolNo < MRS_RM_POOL_MAX_NUM; ucPoolNo++)
    {
        P_MRS_RM_REQ_POOL pstPool = mrsRmGetPool(ucPoolNo);
        mrsRmPoolCheckTimerProc(pstPool, &ulLinePlcNum, &ulWaitSendNum);
    }

    if((ulLinePlcNum == 0) && (ulWaitSendNum == 0))
    {
        HI_DIAG_LOG_MSG_E0(__LINE__, HI_DIAG_MT("ֹͣ��ʱ��鶨ʱ��"));

        MRS_StopTimer(EN_MRS_SRV_CCO_RM_CHECK_TIMER);
        mrsRmSetTimerStatus(MRS_RM_TIMER_INACTIVE);
    }

    return;
}


// ��λ�������ģ��(�������·�AFN=12-F1·������ʱ����)
HI_VOID mrsRmModuleReset(HI_VOID)
{
    HI_U8 ucPoolNo = 0;

    HI_DIAG_LOG_MSG_E0(__LINE__, HI_DIAG_MT("��λ�������ģ��-��ջ����"));

    for(ucPoolNo = 0; ucPoolNo < MRS_RM_POOL_MAX_NUM; ucPoolNo++)
    {
        P_MRS_RM_REQ_POOL pstPool = mrsRmGetPool(ucPoolNo);
        mrsRmEmptyPool(pstPool);
    }

    MRS_StopTimer(EN_MRS_SRV_CCO_RM_CHECK_TIMER);
    mrsRmSetTimerStatus(MRS_RM_TIMER_INACTIVE);

    HI_MDM_SetTfNtbSendProcessCtrl(HI_MDM_NTB_SEND_PROCESS_RESUME, HI_MDM_NTB_SEND_CTRL_REASON_READ_METER);

    return;
}



#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)

HI_PRV HI_U32 mrsRmPlcPackCreate(P_MRS_RM_NODE pstNode, P_MRS_RM_ITEM pstItem, P_MRS_RETRY_CFG_S pstTryCfg)
{
    MRS_CCO_SRV_CTX_STRU *      pstCcoCtx = mrsCcoGetContext();
    MRS_PLC_FRAME_DATA_STRU *   pstPlcFrame = HI_NULL;
    MRS_CSG_PLC_FRAME_HEAD *    pstFrameHead = HI_NULL;
    MRS_CSG_TRANS_XR_DL_STRU *  pstTransData = HI_NULL;
    MRS_CCO_MR_TASK_ITEM_STRU * pstTaskItem = HI_NULL;

    P_MRS_NODE_CTX pstNodeCtx = HI_NULL;
    HI_U8 *pucPayload = HI_NULL;
    HI_U16 usPayloadLen = 0;

    HI_UNREF_PARAM(pstTryCfg);

    if (!pstNode || !pstItem)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    pstTaskItem = (MRS_CCO_MR_TASK_ITEM_STRU *)pstItem->pvParam;
    if (!pstTaskItem)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    usPayloadLen = (HI_U16)(sizeof(MRS_CSG_PLC_FRAME_HEAD) + sizeof(MRS_CSG_TRANS_XR_DL_STRU) + pstItem->usDataLen);
    pucPayload = (HI_U8 *)mrsToolsMalloc(usPayloadLen);
    if (!pucPayload)
    {
        return HI_ERR_NO_MORE_MEMORY;
    }

    pstNodeCtx = pstNode->pstNodeCtx;

    (hi_void)memset_s(pucPayload, usPayloadLen, 0, usPayloadLen);

    pstFrameHead = (MRS_CSG_PLC_FRAME_HEAD *)pucPayload;
    pstFrameHead->usFrameType = MRS_CSG_PLC_CTRL_TYPE_TRANS;
    pstFrameHead->usExtendBit = MRS_CSG_PLC_CTRL_EXTEND_BIT_N;
    pstFrameHead->usRespBit = MRS_CSG_PLC_CTRL_RESP_BIT_Y;
    pstFrameHead->usPrm = MRS_CSG_PLC_CTRL_PRM_HOST;
    pstFrameHead->usDir = MRS_CSG_PLC_CTRL_DIR_DL;
    pstFrameHead->ucSrvId = MRS_CSG_PLC_SRV_ID_TRANS_XR;
    pstFrameHead->ucVersion = MRS_CSG_PLC_PROTO_VERSION;
    pstFrameHead->usSeq = pstCcoCtx->usPlcSeq++;
    pstFrameHead->usDataLength = sizeof(MRS_CSG_TRANS_XR_DL_STRU) + pstItem->usDataLen;

    pstTransData = (MRS_CSG_TRANS_XR_DL_STRU *)pstFrameHead->aucData;
    if (MRS_CCO_MR_TASK_TYPE_MULTICAST == pstTaskItem->ucType)
    {
        (hi_void)memcpy_s(pstTransData->aucSrcAddr, HI_METER_ADDR_LEN, pstCcoCtx->ucMainNodeAddr, HI_METER_ADDR_LEN);
    }
    else
    {
        (hi_void)memcpy_s(pstTransData->aucSrcAddr, HI_METER_ADDR_LEN, pstTaskItem->unParam.stUnicast.aucSrcAddr, HI_METER_ADDR_LEN);
    }
    (hi_void)memcpy_s(pstTransData->aucDstAddr, HI_METER_ADDR_LEN, pstItem->ucMeterAddr, HI_METER_ADDR_LEN);
    pstTransData->ucTimeout = (HI_U8)MRS_MS_TO_100MS(pstNodeCtx->ulOverTimeMeter);
    pstTransData->usDataLen = pstItem->usDataLen;
    (hi_void)memcpy_s(pstTransData->aucData, pstTransData->usDataLen, pstItem->pucData, pstItem->usDataLen);

    pstNodeCtx->ulPlcSeq = (HI_U32)pstFrameHead->usSeq;
    pstNodeCtx->ucIsBC = mrsToolsBroadCastMac(pstNode->ucDestMac);

    pstPlcFrame = &pstNodeCtx->stPlcData;
    (hi_void)memset_s(pstPlcFrame, sizeof(MRS_PLC_FRAME_DATA_STRU), 0, sizeof(MRS_PLC_FRAME_DATA_STRU));
    pstPlcFrame->usId = ID_MRS_CMD_CSG_DATA_TRANSMIT_PLC;
    pstPlcFrame->usPayloadLen = usPayloadLen;
    pstPlcFrame->pucPayload = pucPayload;
    (hi_void)memcpy_s(pstPlcFrame->ucMac, HI_PLC_MAC_ADDR_LEN, pstNode->ucDestMac, HI_PLC_MAC_ADDR_LEN);

#if defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG)
    mrsLogFrameInfStatTx(pstItem->pucData, pstItem->usDataLen, pstNodeCtx->ucIsBC);
#endif

    return HI_ERR_SUCCESS;
}


HI_U32 mrsRmPlcPackageReceiveCsg(HI_U8 *pucPayload, HI_U16 usPayloadLen)
{
    MRS_CSG_PLC_FRAME_HEAD *pstFrameHead = (MRS_CSG_PLC_FRAME_HEAD *)pucPayload;
    MRS_CSG_TRANS_XR_UL_STRU *pstXrFrame = (MRS_CSG_TRANS_XR_UL_STRU *)pstFrameHead->aucData;
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U8 ucPoolNo = 0;
    HI_BOOL bFind = HI_FALSE;

    if (!pucPayload || 0 == usPayloadLen)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    if ((pstFrameHead->usDataLength + sizeof(MRS_CSG_PLC_FRAME_HEAD) != usPayloadLen)
        || (pstFrameHead->usDataLength != sizeof(MRS_CSG_TRANS_XR_UL_STRU) + pstXrFrame->usDataLen))
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    for (ucPoolNo = 0; ucPoolNo < MRS_RM_POOL_MAX_NUM; ucPoolNo++)
    {
        P_MRS_RM_REQ_POOL pstPool = mrsRmGetPool(ucPoolNo);
        mrsRmPoolPlcRxProcCsg(pstPool, pucPayload, usPayloadLen, &bFind);
        if (bFind)
        {
            break;
        }
    }

    if (!bFind)
    {
        mrsDfxPrPlcSeqErr();
        mrsDfxRmPlcUpRx(ID_MRS_CMD_CSG_DATA_TRANSMIT_PLC, (HI_U8 *)pstXrFrame, HI_TRUE);
        ulRet = HI_ERR_FAILURE;
    }

    return ulRet;
}


HI_U32 mrsRmPoolPlcRxProcCsg(P_MRS_RM_REQ_POOL pstPool, HI_U8 *pucPayload, HI_U16 usPayloadLen, HI_BOOL *pbFind)
{
    MRS_CSG_PLC_FRAME_HEAD *pstFrameHead = (MRS_CSG_PLC_FRAME_HEAD *)pucPayload;
    MRS_SRV_LIST *pstList = &pstPool->stNodeList;
    MRS_SRV_LINK *pstIter = pstList->next;
    MRS_SRV_LINK *pstNext = HI_NULL;
    HI_U32 ulRet = HI_ERR_SUCCESS;

    while (pstIter != pstList)
    {
        P_MRS_RM_NODE pstNode = (P_MRS_RM_NODE)pstIter;
        P_MRS_NODE_CTX pstNodeCtx = HI_NULL;

        if (pstNode->ucState != MRS_RM_NODE_WAIT_RESP)
        {
            pstIter = pstIter->next;
            continue;
        }

        pstNext = pstIter->next;
        pstNodeCtx = pstNode->pstNodeCtx;

        if (pstNodeCtx->ulPlcSeq == (HI_U32)pstFrameHead->usSeq)
        {
            *pbFind = HI_TRUE;

            pstPool->usParallelNum--;
            pstPool->usItemTotalNum--;

            mrsDfxRmPalallelNum((HI_U32)pstPool, pstPool->usParallelNum);
            mrsDfxRmItemNum((HI_U32)pstPool, pstPool->usItemTotalNum);

            if (pstNodeCtx->ucIsBC)
            {
                HI_DIAG_LOG_MSG_E0(__LINE__, HI_DIAG_MT("�㲥����Ӧ��ɹ�-�����ģʽ�޸�Ϊ[��ͨ]"));
                mrsRmSetPoolMode(pstPool, MRS_RM_NORMAL_MODE);
            }

            HI_DIAG_LOG_MSG_E2(__LINE__, HI_DIAG_MT("����PLCӦ�����ݰ�-[������][������]"), pstPool->usParallelNum, pstPool->usItemTotalNum);
            ulRet = mrsRmPlcRespProcCsg(pstNode, pucPayload, usPayloadLen);
            if (HI_ERR_QUEUE_EMPTY == ulRet)
            {
                mrsRmRemoveNode(pstList, pstNode);
            }

            break;
        }

        pstIter = pstNext;
    }

    if (pstPool->usItemTotalNum)
    {
        mrsRmSendReqProcActive(pstPool);
    }
    else
    {
        HI_MDM_SetTfNtbSendProcessCtrl(HI_MDM_NTB_SEND_PROCESS_RESUME, HI_MDM_NTB_SEND_CTRL_REASON_READ_METER);
    }

    return ulRet;
}


HI_U32 mrsRmPlcRespProcCsg(P_MRS_RM_NODE pstNode, HI_U8 *pucPayload, HI_U16 usPayloadLen)
{
    P_MRS_RM_ITEM pstItem = HI_NULL;
    P_MRS_RM_STRATEGY_REG pstStrategy = HI_NULL;
    MRS_CSG_PLC_FRAME_HEAD *pstFrameHead = (MRS_CSG_PLC_FRAME_HEAD *)pucPayload;
    MRS_CSG_TRANS_XR_UL_STRU *pstXrUlFrame = (MRS_CSG_TRANS_XR_UL_STRU *)pstFrameHead->aucData;

    HI_UNREF_PARAM(usPayloadLen);

    pstItem = mrsSrvQueueTop(&pstNode->stItemQueue);
    if (!pstItem)
    {
        return HI_ERR_QUEUE_EMPTY;
    }

    pstStrategy = mrsRmGetStrategy(pstItem->ucStrategyNo);

    mrsDfxPrOkCnt();
#if defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG)
    {
        MRS_CSG_PLC_FRAME_HEAD *pstPlcDlData = (MRS_CSG_PLC_FRAME_HEAD *)&pstNode->pstNodeCtx->stPlcData.pucPayload;
        MRS_CSG_TRANS_XR_DL_STRU *pstXrDlFrame = (MRS_CSG_TRANS_XR_DL_STRU *)pstPlcDlData->aucData;

        mrsLogFrameInfStatRx(pstXrUlFrame->aucData, pstXrUlFrame->usDataLen, pstXrDlFrame->aucData, pstXrDlFrame->usDataLen, 1);
    }
#endif

    // ���Ͷ���Ӧ�����ݵ�ҵ�����
    mrsRmReturnRespData(pstStrategy, pstItem, pstXrUlFrame->aucData, pstXrUlFrame->usDataLen);

    // �������������(������в�Ϊ�����ʼ����������)
    return mrsRmItemDeQueue(pstNode, HI_TRUE);
}


HI_VOID mrsRmRemoveItemFromPool(HI_VOID *pParam)
{
    HI_U8 ucPoolNo = 0;

    for (ucPoolNo = 0; ucPoolNo < MRS_RM_POOL_MAX_NUM; ucPoolNo++)
    {
        P_MRS_RM_REQ_POOL pstPool = mrsRmGetPool(ucPoolNo);
        MRS_SRV_LIST *pstList = &pstPool->stNodeList;
        MRS_SRV_LINK *pstIter = HI_NULL;

        for (pstIter = pstList->next; pstIter != pstList; pstIter = pstIter->next)
        {
            P_MRS_RM_NODE pstNode = (P_MRS_RM_NODE)pstIter;
            MRS_SRV_NODE *pstItNode = HI_NULL;

            for (pstItNode = pstNode->stItemQueue.front; pstItNode != HI_NULL; pstItNode = pstItNode->next)
            {
                P_MRS_RM_ITEM pstTemp = (P_MRS_RM_ITEM)pstItNode;
                if (pstTemp->pvParam == pParam)
                {
                    pstTemp->pvParam = HI_NULL;
                }
            }
        }
    }
}


#endif


#endif

