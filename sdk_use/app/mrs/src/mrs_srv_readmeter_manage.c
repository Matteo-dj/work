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

    pstCfg->MeterMaxRetryNv = ((pstCfg->MeterMaxRetryNv > MRS_RM_METER_RETRY_CNT_MAX) ? MRS_PRM_METER_RETRY_CNT_DEFAULT : pstCfg->MeterMaxRetryNv);

    // �����ʱʱ��Ϊ0, ����Ĭ��ֵ����
    if (0 == pstCfg->MaxServiceTime)
    {
        pstCfg->MaxServiceTime = MRS_RM_MAX_SERVICE_TIME_DEFAULT;
    }

    return;
}

// ��ʼ���ش����Ʋ���
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
        // �����ش����ò���
        (hi_void)memcpy_s(pstCfg, sizeof(MRS_RETRY_CFG_S), pstNvCfg, sizeof(MRS_RETRY_CFG_S));

        // �ش����ò�����Ч�Լ��
        mrsRmRetryCfgValidtyCheck(pstCfg);
    }

    return;
}


// ��ʼ�������������
HI_PRV HI_VOID mrsRmParallelCfgInit(P_MRS_PARALLEL_CFG_S pstCfg, P_NV_MRS_PARALLEL_CFG_S pstNvCfg)
{
    if(pstNvCfg == HI_NULL)
    {
        pstCfg->usMaxParallelNum = MRS_PARALLEL_NUM_DEFAULT;
        pstCfg->usMaxFrame = MRS_FRAME_NUM_DEFAULT;
        pstCfg->usMaxTotalItem = MRS_ITEM_NUM_DEFAULT;
    /* BEGIN: Modified by liuxipeng/lKF54842,2014/4/26 ���ⵥ��:DTS2014042407354 */
        pstCfg->ucCheckRepeatMode = MRS_RM_ADDR_MATCH;
    /* END  : Modified by liuxipeng/lKF54842,2014/4/26 ���ⵥ��:DTS2014042407354 */
    }
    else
    {
        // ���²������ò���
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
    /* BEGIN: Modified by liuxipeng/lKF54842,2014/4/26 ���ⵥ��:DTS2014042407354 */
        if((pstCfg->ucCheckRepeatMode != MRS_RM_CRC_MATCH)
            && (pstCfg->ucCheckRepeatMode != MRS_RM_ADDR_MATCH))
        {
            pstCfg->ucCheckRepeatMode = MRS_RM_ADDR_MATCH;
        }
    /* END  : Modified by liuxipeng/lKF54842,2014/4/26 ���ⵥ��:DTS2014042407354 */
    }

    return;
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
            mrsRmRetryCfgInit(pstCfg, pstParam + i, i);//ע��:����ĵ���������ʵ����MRS_RM_XR/MRS_RM_LR/MRS_RM_PR,ͬi����ֵͬ
        }
        else
        {
            mrsRmRetryCfgInit(pstCfg, HI_NULL, i);//ע��:����ĵ���������ʵ����MRS_RM_XR/MRS_RM_LR/MRS_RM_PR,ͬi����ֵͬ
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
            mrsRmParallelCfgInit(pstCfg, pstParam + i);
        }
        else
        {
            mrsRmParallelCfgInit(pstCfg, HI_NULL);
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

        HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1000, HI_DIAG_MT("�����ڵ㲢��������"), ucMac, HI_PLC_MAC_ADDR_LEN);

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

        HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1001, HI_DIAG_MT("�ڽڵ�����м����������"), pstData->ucAddr, HI_METER_ADDR_LEN);

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
    /* BEGIN: Modified by liuxipeng/lKF54842,2014/4/26 ���ⵥ��:DTS2014042407354 */
        pstItem->ulDataCrc    = pst645Inf->ulCrc;
    /* END  : Modified by liuxipeng/lKF54842,2014/4/26 ���ⵥ��:DTS2014042407354 */
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

/* BEGIN: Modified by liuxipeng/lKF54842,2014/4/29 ���ⵥ��:DTS2014042306517 */
// ��������Ԫ�س�����(�ڵ㴦��ʱ��������Ӧ������)
HI_PRV HI_U32 mrsRmItemDeQueue(P_MRS_RM_NODE pstNode, HI_BOOL bSwitchFlg)
{
    P_MRS_RM_ITEM pstItem = HI_NULL;

    pstItem = mrsSrvDeQueue(&pstNode->stItemQueue);
    if(pstItem)
    {
        HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1002, HI_DIAG_MT("�������������"), pstItem->ucMeterAddr, HI_METER_ADDR_LEN);

        pstNode->ucItemNum--;
        mrsToolsFree(pstItem);
    }

    // ��λ�ڵ�������(�ͷű����PLC��������;�������������)
    mrsRmNodeCtxReset(pstNode);

    // ��ʼ���ڵ�������(������в�Ϊ�������������һ����������)
    return mrsRmNodeCtxInit(pstNode, bSwitchFlg);
}
/* END  : Modified by liuxipeng/lKF54842,2014/4/29 ���ⵥ��:DTS2014042306517 */


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
            HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1003, HI_DIAG_MT("���������ҵ��˶�Ӧ�ڵ�"), ucMac, HI_PLC_MAC_ADDR_LEN);
            return node;
        }
    }

    return HI_NULL;
}

/* BEGIN: Modified by liuxipeng/lKF54842,2014/4/26 ���ⵥ��:DTS2014042407354 */
// �ڻ���ص�һ�ڵ�����в����Ѵ��ڵĶ�������
// ucOpt:
// MRS_RM_ADDR_MATCH ʹ�ñ��ַƥ�����
// MRS_RM_CRC_MATCH  ʹ��CRCֵƥ�����
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
/* END  : Modified by liuxipeng/lKF54842,2014/4/26 ���ⵥ��:DTS2014042407354 */

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

    HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1004, HI_DIAG_MT("���������Ƴ��ڵ�"), pstNode->ucDestMac, HI_PLC_MAC_ADDR_LEN);

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
        HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1005, HI_DIAG_MT("�ڵ������Ĵ���"), pstNode->ucDestMac, HI_PLC_MAC_ADDR_LEN);

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
        HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1006, HI_DIAG_MT("�ڵ������ĸ�λ"), pstNode->ucDestMac, HI_PLC_MAC_ADDR_LEN);

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

/* BEGIN: Modified by liuxipeng/lKF54842,2014/4/29 ���ⵥ��:DTS2014042306517 */
// �ڵ������ĳ�ʼ��
HI_PRV HI_U32 mrsRmNodeCtxInit(P_MRS_RM_NODE pstNode, HI_BOOL bSwitchFlg)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    P_MRS_RM_ITEM pstItem = HI_NULL;
    P_MRS_RM_STRATEGY_REG pstStrategy = HI_NULL;
    P_MRS_RETRY_CFG_S pstTryCfg = HI_NULL;

    HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1007, HI_DIAG_MT("�ڵ������ĳ�ʼ��"), pstNode->ucDestMac, HI_PLC_MAC_ADDR_LEN);

    pstItem = mrsSrvQueueTop(&pstNode->stItemQueue);
    if(pstItem == HI_NULL)
    {
        HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1008, HI_DIAG_MT("�ڵ����Ϊ��"), pstNode->ucDestMac, HI_PLC_MAC_ADDR_LEN);
        return HI_ERR_QUEUE_EMPTY;
    }

    HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1009, HI_DIAG_MT("��ʼ���µĶ�������"), pstItem->ucMeterAddr, HI_METER_ADDR_LEN);

    pstStrategy = mrsRmGetStrategy(pstItem->ucStrategyNo);
    pstTryCfg = mrsRmGetRetryCfg(pstStrategy->ucRetryCfgNo);

    // �ش����Ʋ�����ʼ��
    mrsRmNodeRetryParamInit(pstNode, pstTryCfg, HI_FALSE);

    // ����PLC֡
    ret = mrsRmPlcPackCreate(pstNode, pstItem, pstTryCfg);
    if(ret == HI_ERR_SUCCESS)
    {
        HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_001, HI_DIAG_MT("����PLC����֡-���"), pstNode->pstNodeCtx->ulPlcSeq);
        if(bSwitchFlg)
        {
            HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1010, HI_DIAG_MT("�ڵ�״̬Ϊ-�ȴ�����"), pstNode->ucDestMac, HI_PLC_MAC_ADDR_LEN);
            pstNode->ucState = MRS_RM_NODE_WAIT_SEND;
        }
        else
        {
            pstNode->ucState = MRS_RM_NODE_SEND_REQ;
        }
    }

    return ret;
}

//�������� ���쳣��������л�ȡ�ڵ�
HI_PRV HI_VOID mrsParallelRmFindFailNode(HI_U8 *MeterAddr, MRS_RM_FAIL_FIND_LINK* pstFailLink)
{
    P_MRS_READMETER_MODULE ReadMeterCtx = mrsRmGetModuleCtx();

    (hi_void)memset_s(pstFailLink, sizeof(MRS_RM_FAIL_FIND_LINK), 0, sizeof(MRS_RM_FAIL_FIND_LINK));
    (hi_void)memcpy_s(pstFailLink->ucMeterAddr,HI_PLC_MAC_ADDR_LEN,MeterAddr,HI_PLC_MAC_ADDR_LEN);//���ַ
    mrsSrvTraverseList(&ReadMeterCtx->stParallelRmList, mrsRmFindMeterListNode, pstFailLink);
}
// �������� ���쳣������������һ���ڵ�
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

// �������� �쳣��������ѯ�������ú���
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

// �������� �쳣�������ɾ���ڵ�
HI_VOID mrsParallelRmFailDel(HI_U8* MeterAddr)
{
    P_MRS_RM_FAIL_NODE pstFailNode = HI_NULL;
    P_MRS_READMETER_MODULE ReadMeterCtx = mrsRmGetModuleCtx();
    MRS_RM_FAIL_FIND_LINK stFailLink;

    //�쳣��������е�mac��ַ�ǵ����
    mrsParallelRmFindFailNode(MeterAddr, &stFailLink);
    if(HI_NULL != stFailLink.pstLink)
    {
        pstFailNode = (P_MRS_RM_FAIL_NODE)(stFailLink.pstLink);
        mrsSrvListRemove(&ReadMeterCtx->stParallelRmList,&pstFailNode->link);
        mrsToolsFree(pstFailNode);
    }
}

// �������� �쳣�����
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

/* END  : Modified by liuxipeng/lKF54842,2014/4/29 ���ⵥ��:DTS2014042306517 */

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
    (hi_void)memset_s(pstData, usLen, 0, usLen);

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
    //mrsLogFrameInfStatTx(pstItem->pucData, pstItem->usDataLen, pstNodeCtx->ucIsBC);//optimized by weichao
#endif

    return HI_ERR_SUCCESS;
}


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

    if (bCltIDelay)
    {
        pstNodeCtx->ulOverTimeMeter = (HI_U16)pstTryCfg->MeterOverTimeNv                     // ���ʱ
                                    + (HI_U16)MRS_RM_CLTI_DELAY_DEFAULT * bCltIDelay            // I�Ͳɼ�������
                                    + (HI_U16)pstTryCfg->CtrlCmdDelayNv * pstItem->bCtrlCmd; // �ѿ������
    }

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

    HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1011, HI_DIAG_MT("PLCĿ��MAC"), pstNodeCtx->stPlcData.ucMac, HI_PLC_MAC_ADDR_LEN);
    if (pstNodeCtx->ucIsBC)
    {
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_002, HI_DIAG_MT("[�㲥]��ȫF�滻Ŀ��MAC"));
        (hi_void)memcpy_s(mac, HI_PLC_MAC_ADDR_LEN, pstNodeCtx->stPlcData.ucMac, HI_PLC_MAC_ADDR_LEN);
        (hi_void)memset_s(pstNodeCtx->stPlcData.ucMac, HI_PLC_MAC_ADDR_LEN, 0xFF, HI_PLC_MAC_ADDR_LEN);
    }

    ret = MRS_SendPlcFrame(&pstNodeCtx->stPlcData);
    mrsDfxPrPlcTx();

/* BEGIN: Modified by fengxiaomin/00209182, 2014/3/27   ���ⵥ��:DTS2014032906596 */
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

/* BEGIN: Modified by liuxipeng/lKF54842,2014/4/26 ���ⵥ��:DTS2014042407354 */
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
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_003, HI_DIAG_MT("�������-�ܾ�����"));
        return HI_ERR_FULL;
    }

    if (mrsCheck645Proto(pstData->ucProtocol))
    {
        // ��ȡҵ�����������645֡��Ϣ
        mrsCountBuf645Inf(pstData->pucData, pstData->usDataLen, pst645Inf);
    }
    else if (0 != pstData->usDataLen)
    {
        pst645Inf->ucNum = 1;
        pst645Inf->pucAddr = pstData->ucAddr;
        pst645Inf->pucHead = pstData->pucData;
        pst645Inf->ulCrc = HI_MDM_Crc32(0, pstData->pucData, pstData->usDataLen);
    }

/* BEGIN: Modified by liuxipeng/lKF54842,2014/5/15 ���ⵥ��:DTS2014051308231 */
    // ҵ�����������û��645֡-���������
    if(pst645Inf->ucNum == 0)
    {
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_004, HI_DIAG_MT("��Ч����֡��Ϊ0"));
        return HI_ERR_BAD_DATA;
    }

    // ҵ����������г������֡������-���������
    if(pst645Inf->ucNum > pstCfg->usMaxFrame)
    {
        HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_005, HI_DIAG_MT("��������֡��"), pst645Inf->ucNum);
        return HI_ERR_NOT_SUPPORT;
    }

    HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_006, HI_DIAG_MT("��ǰ��������CRCУ��"), pst645Inf->ulCrc);
/* END  : Modified by liuxipeng/lKF54842,2014/5/15 ���ⵥ��:DTS2014051308231 */

    // ���ҵ�����û���ṩ��ַ��ʹ��645֡��ַ
    if(pstData->ucAddr == HI_NULL)
    {
        pstData->ucAddr = pst645Inf->pucAddr;
    }

    if(pstCfg->ucCheckRepeatMode == MRS_RM_ADDR_MATCH)
    {
/* BEGIN: PN: DTS2015042907341 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/29 */
        // ʹ�õ���ַ��Ϊ�ظ�ƥ����
        ulID = (HI_U32)pstData;
/* END:   PN: DTS2015042907341 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/29 */
    }
    else
    {
        // ʹ��CRC��Ϊ�ظ�ƥ����
        ulID = pst645Inf->ulCrc;
    }

    // �����������Ѵ��ڴ˶�������-���������
    if(mrsRmFindReqOfPool(pstPool, ulID, pstCfg->ucCheckRepeatMode))
    {
        HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1012, HI_DIAG_MT("�˵�����ڳ�����"), pstData->ucAddr, HI_METER_ADDR_LEN);
        return HI_ERR_BUSY;
    }

    return HI_ERR_SUCCESS;
}
/* END  : Modified by liuxipeng/lKF54842,2014/4/26 ���ⵥ��:DTS2014042407354 */

// ����ʹ���
HI_PRV HI_U32 mrsRmSendReqProcActive(P_MRS_RM_REQ_POOL pstPool)
{
    HI_SYS_QUEUE_MSG_S stMsg = {0};

    (hi_void)memset_s(&stMsg, sizeof(stMsg), 0, sizeof(stMsg));

    stMsg.ulMsgId = EN_MRS_FW_MSG_RM_PLC_SEND_REQ;
    stMsg.ulParam[0] = (HI_U32)pstPool;

    HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_007, HI_DIAG_MT("�����������..."));

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
    /* BEGIN: Modified by liuxipeng/lKF54842,2014/4/29 ���ⵥ��:DTS2014042306517 */
        ret = mrsRmNodeCtxInit(node, HI_FALSE);
    /* END  : Modified by liuxipeng/lKF54842,2014/4/29 ���ⵥ��:DTS2014042306517 */
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

        //�쳣�����
        if(usLen)
        {
            mrsParallelRmFailDel(pstItem->ucMeterAddr);
        }

        // ����ҵ�����PLCӦ������
        ret = pstStrategy->pfnRespProc(&stData);

    /* BEGIN: Modified by fengxiaomin/00209182, 2014/3/27   ���ⵥ��:DTS2014032906596 */
        mrsDfxRmUartTx(pucData, usLen, HI_FALSE);
    /* END:   Modified by fengxiaomin/00209182, 2014/3/27 */
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
            HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_008, HI_DIAG_MT("����ǿ���л�Ϊ�㲥"));

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
/* BEGIN: Modified by fengxiaomin/00209182, 2014/3/27   ���ⵥ��:DTS2014032906596 */
    mrsDfxRmPlcDlRx(ID_MRS_CMD_PARALLEL_DATA_PLC, pstNodeCtx->stPlcData.pucPayload, HI_TRUE);
/* END:   Modified by fengxiaomin/00209182, 2014/3/27 */

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
        // ��Ҫ�ش�
        HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1013, HI_DIAG_MT("PLC֡��Ҫ�ش�"), pstItem->ucMeterAddr, HI_METER_ADDR_LEN);
        return ret;
    }

    HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1014, HI_DIAG_MT("PLC����ʧ��"), pstItem->ucMeterAddr, HI_METER_ADDR_LEN);

    //�쳣�����
    mrsRmAddFailNode(pstItem->ucMeterAddr);

    mrsDfxPrFailCnt();
    //mrsLogPlcOverTimeStat(pstItem->pucData, pstItem->usDataLen, 0, HI_NULL);//optimized by weichao

    // ֪ͨҵ�����PLC��ʱ
    mrsRmReturnRespData(pstStrategy, pstItem, HI_NULL, 0, pstNode->ucDestMac);
    mrsDetectPrRmTimeout(pstItem->ucMeterAddr);
    mrsDecreaseMrTTL(&(mrsCcoGetContext()->stMapModule), pstItem->ucMeterAddr);

    // �ܶ�����������Լ�
    pstPool->usItemTotalNum--;
    mrsDfxRmItemNum((HI_U32)pstPool, pstPool->usItemTotalNum);

/* BEGIN: Modified by liuxipeng/lKF54842,2014/4/29 ���ⵥ��:DTS2014042306517 */
    // �������������(������в�Ϊ�����ʼ����������)
    return mrsRmItemDeQueue(pstNode, HI_FALSE);
/* END  : Modified by liuxipeng/lKF54842,2014/4/29 ���ⵥ��:DTS2014042306517 */
}


/* BEGIN: Modified by liuxipeng/lKF54842,2014/4/29 ���ⵥ��:DTS2014042306517 */
// PLC��ʱ���
HI_PRV HI_U32 mrsRmPlcOverTimeCheck(P_MRS_RM_REQ_POOL pstPool,
                                                P_MRS_RM_NODE pstNode,
                                                HI_BOOL *bOverTime)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    P_MRS_NODE_CTX pstNodeCtx = HI_NULL;

    pstNodeCtx = pstNode->pstNodeCtx;

    pstNodeCtx->ulRunOutTime += MRS_RM_CHECK_TIMER_UNIT_BASE;

    // �����ڵ㳬ʱ�ж�
    if(pstNodeCtx->ulRunOutTime >= pstNodeCtx->ulOverTimeTotal)
    {
        *bOverTime = HI_TRUE;

        HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1015, HI_DIAG_MT("����PLC��ʱ-MAC��ַ"), pstNode->ucDestMac, HI_PLC_MAC_ADDR_LEN);

        // �����ڵ㳬ʱ����
        ret = mrsRmPlcOverTimeProc(pstPool, pstNode);

        if(ret == HI_ERR_QUEUE_EMPTY)
        {
            mrsRmRemoveNode(&pstPool->stNodeList, pstNode);
        }

        HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_009, HI_DIAG_MT("��ǰ������-��ǰ����������"), pstPool->usParallelNum, pstPool->usItemTotalNum);
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
        HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1016, HI_DIAG_MT("�ڵ�״̬�л�Ϊ-������"), pstNode->ucDestMac, HI_PLC_MAC_ADDR_LEN);

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
/* END  : Modified by liuxipeng/lKF54842,2014/4/29 ���ⵥ��:DTS2014042306517 */


// �����PLC���մ���
HI_PRV HI_U32 mrsRmPoolPlcRxProc(P_MRS_RM_REQ_POOL pstPool, HI_U8 *pucData,
    MRS_DATA_TRANSMIT_AFNF1_UP_STRU *pstPackage, HI_BOOL *bFind, MRS_DEVICE_INF_STRU *pstDevInf, HI_U8 ucMacAdd[HI_PLC_MAC_ADDR_LEN])
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
                HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_010, HI_DIAG_MT("�㲥����Ӧ��ɹ�-�����ģʽ�޸�Ϊ[��ͨ]"));
                mrsRmSetPoolMode(pstPool, MRS_RM_NORMAL_MODE);
            }

            HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_011, HI_DIAG_MT("����PLCӦ�����ݰ�-[������][������]"), pstPool->usParallelNum, pstPool->usItemTotalNum);

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

/* BEGIN: Modified by liuxipeng/lKF54842,2014/4/29 ���ⵥ��:DTS2014042306517 */
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
/* END  : Modified by liuxipeng/lKF54842,2014/4/29 ���ⵥ��:DTS2014042306517 */


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
    MRS_DATA_TRANSMIT_AFNF1_UP_STRU *pstPackage, MRS_DEVICE_INF_STRU *pstDevInf, HI_U8 ucMacAdd[HI_PLC_MAC_ADDR_LEN])
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
#if 0//optimized by weichao
#if defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG)
    mrsLogFrameInfStatRx(pucData, pstPackage->data_len,
                            pstItem->pucData, pstItem->usDataLen,
                            pstPackage->resp_status);
#endif
#endif
    // ���Ͷ���Ӧ�����ݵ�ҵ�����
    mrsRmReturnRespData(pstStrategy, pstItem, pucData, pstPackage->data_len, ucMacAdd);

    if (pstDevInf)
    {
        (hi_void)memcpy_s(pstDevInf->aucAddr, HI_METER_ADDR_LEN, pstItem->ucMeterAddr, HI_METER_ADDR_LEN);
        pstDevInf->ucProtocol = pstItem->ucProtocol;
    }

/* BEGIN: Modified by liuxipeng/lKF54842,2014/4/29 ���ⵥ��:DTS2014042306517 */
    // �������������(������в�Ϊ�����ʼ����������)
    return mrsRmItemDeQueue(pstNode, HI_TRUE);
/* END  : Modified by liuxipeng/lKF54842,2014/4/29 ���ⵥ��:DTS2014042306517 */
}


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

    //�������� �쳣��������ʼ��
    mrsSrvInitList(&pstRmCtx->stParallelRmList);

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

        // �ж��Ƿ���Լ��뻺���
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

        // ����ӳ���ϵ

        if(ret != HI_ERR_SUCCESS)
        {
            (hi_void)memset_s(ucMac, HI_PLC_MAC_ADDR_LEN, 0xFF, HI_PLC_MAC_ADDR_LEN);   // û���ҵ���Ӧ��MAC��ַ(ʹ�ù㲥MAC)
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
            HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_012, HI_DIAG_MT("������뻺��سɹ�"), pstPool->usItemTotalNum);
        }
    }while(0);

    if(ret != HI_ERR_SUCCESS)
    {
        mrsDfxRmJoinFail((HI_U32)pstPool, ret);
    }

    return ret;
}


// PLC����������
HI_U32 mrsRmPlcSendReqProc(HI_SYS_QUEUE_MSG_S* pstMsg)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    P_MRS_RM_REQ_POOL pstPool   = (P_MRS_RM_REQ_POOL)pstMsg->ulParam[0];
    P_MRS_PARALLEL_CFG_S pstCfg = &pstPool->stParallelCfg;
    MRS_SRV_LINK *it, *next;
    MRS_SRV_LIST *list;

    HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_013, HI_DIAG_MT("����������..."));

    if(pstPool->ucMode != MRS_RM_NORMAL_MODE)
    {
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_014, HI_DIAG_MT("����ع���ģʽΪ[�㲥]..."));
        if (MRS_RM_SCENE_TOPO_HAS_NO_CLT != mrsGetRmTopoScene())
        {
            HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_015, HI_DIAG_MT("[�㲥]��ȫ�س���-������һ·����..."));
            return HI_ERR_SUCCESS;
        }
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_016, HI_DIAG_MT("[�㲥]ȫ�س���-�����·����..."));
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
            HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_017, HI_DIAG_MT("������󲢷�����-��ͣ����"), pstPool->usParallelNum);
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

            HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_018, HI_DIAG_MT("����PLC֡�ɹ�"), pstPool->usParallelNum);
            mrsDfxRmPalallelNum((HI_U32)pstPool, pstPool->usParallelNum);

            if(pstNodeCtx->ucIsBC)
            {
                HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_019, HI_DIAG_MT("���͹㲥֡-�����ģʽ���Ϊ[�㲥]"));
                mrsRmSetPoolMode(pstPool, MRS_RM_BROADCAST_MODE);
                mrsDfxBcRmCnt();
                break;
            }
        }

        it = next;
    }

    if(mrsRmGetTimerStatus() == MRS_RM_TIMER_INACTIVE)
    {
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_020, HI_DIAG_MT("������ʱ��鶨ʱ��"));
        MRS_StartTimer(EN_MRS_SRV_CCO_RM_CHECK_TIMER,
                            MRS_CCO_TIME_PARALLEL_CHECK,
                            HI_SYS_TIMER_PERIODIC);

        mrsRmSetTimerStatus(MRS_RM_TIMER_ACTIVE);
    }

    return HI_ERR_SUCCESS;
}


// PLC������
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

    HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_021, HI_DIAG_MT("����PLC���ݰ�-���"), pstPackage->seq);

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
        HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_022, HI_DIAG_MT("����PLC���ݰ�-֡��Ŵ���"), pstPackage->seq);
        mrsDfxPrPlcSeqErr();    // ͳ�ƶ�����PLC֡(���ڵ�)

    /* BEGIN: Modified by fengxiaomin/00209182, 2014/3/27   ���ⵥ��:DTS2014032906596 */
        mrsDfxRmPlcUpRx(ID_MRS_CMD_PARALLEL_DATA_PLC, (HI_U8*)pstPackage, HI_TRUE);
    /* END:   Modified by fengxiaomin/00209182, 2014/3/27 */
        ret = HI_ERR_FAILURE;
    }

    return ret;
}

/* BEGIN: Modified by liuxipeng/lKF54842,2014/4/29 ���ⵥ��:DTS2014042306517 */
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
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_023, HI_DIAG_MT("ֹͣ��ʱ��鶨ʱ��"));

        MRS_StopTimer(EN_MRS_SRV_CCO_RM_CHECK_TIMER);
        mrsRmSetTimerStatus(MRS_RM_TIMER_INACTIVE);
    }

    return;
}
/* END  : Modified by liuxipeng/lKF54842,2014/4/29 ���ⵥ��:DTS2014042306517 */


// ��λ�������ģ��(�������·�AFN=12-F1·������ʱ����)
HI_VOID mrsRmModuleReset(HI_VOID)
{
    HI_U8 ucPoolNo = 0;

    HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_024, HI_DIAG_MT("��λ�������ģ��-��ջ����"));

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

