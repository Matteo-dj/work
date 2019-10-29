//*****************************************************************************
//
//                  ��Ȩ���� (C), 1998-2011, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mrs_srv_cco_queue.c
//  �� �� ��   : V1.0 
//  ��    ��   : fengxiaomin/00209182
//  ��������   : 2014-8-16
//  ��������   : CCO�˴�������ʵ��
//               
//  �����б�   : 
//  �޸���ʷ   : 
//  1.��    �� : 2014-8-16
//    ��    �� : fengxiaomin/00209182
//    �޸����� : �����ļ� 
//
//*****************************************************************************

#include "mrs_common.h"
#include "mrs_cmd_msg.h"
#include "hi_mdm_types.h"
#include "app_mrs.h"
#include "mrs_fw_tools.h"
#include "mrs_fw_log.h"
#include "mrs_fw_n.h"
#include "mrs_fw_proto376_2_echo.h"
#include "mrs_fw_proto376_2_frame.h"
#include "mrs_fw_proto645.h"
#include "mrs_srv_common.h"
#include "mrs_srv_cco_macmap.h"
#include "mrs_srv_queue.h"
#include "mrs_srv_list.h"
#include "mrs_srv_cco_event.h"
#include "mrs_srv_list.h"
#include "mrs_fw_nv.h"
#include "mrs_srv_cco.h"
#include "mrs_srv_cco_queue.h"

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)

typedef struct
{
    HI_U8  ucAfn; 
    HI_U8  reserved;
    HI_U16 usFn;     
}MRS_CCO_QUEUE_FIND_RESULT, *P_MRS_CCO_QUEUE_FIND_RESULT;

MRS_SRV_CCO_QUEUE g_CcoQueueCtx;

HI_PRV HI_VOID mrsCcoQueueFreeNode(HI_VOID *pvNode);


HI_VOID mrsCcoQueueInit(HI_VOID)
{
    MRS_LOCK_INIT(g_CcoQueueCtx.ucLock);
    mrsSrvInitQueue(&g_CcoQueueCtx.stCcoQueue);
}

P_MRS_SRV_CCO_QUEUE mrsGetCcoQueue(HI_VOID)
{
    return &g_CcoQueueCtx;
}

HI_VOID mrsCcoSetQueueStatus(HI_U8 ucStatus)
{
    g_CcoQueueCtx.ucStatus = ucStatus;
}

HI_U8 mrsCcoGetQueueStatus(HI_VOID)
{
    return g_CcoQueueCtx.ucStatus;
}

HI_VOID mrsCcoJoinQueue(MRS_SRV_CCO_ITEM *pstCcoItem)
{
    P_MRS_SRV_CCO_QUEUE pstCcoQueue = mrsGetCcoQueue();
    
    HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_002, HI_DIAG_MT("join cco queue afn fn"),pstCcoItem->ucAfn,pstCcoItem->usFn);
    mrsSrvEnQueue(&pstCcoQueue->stCcoQueue, &pstCcoItem->link);
    pstCcoQueue->usItemNum++;
    mrsDfxCcoQueueCurrItem(pstCcoQueue->usItemNum);
    mrsDfxCcoQueueJoinCnt();

}

HI_U32 mrsCcoToMasterMsgPro(HI_VOID)
{
    P_MRS_SRV_CCO_QUEUE pstCcoQueue = mrsGetCcoQueue();
    MRS_SRV_CCO_ITEM *pstCcoItem = HI_NULL;

    
    HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_003, HI_DIAG_MT("cco queue pro lock status"),pstCcoQueue->ucLock);
    if (MRS_TRY_LOCK(pstCcoQueue->ucLock))
    {
        pstCcoItem = mrsSrvQueueTop(&pstCcoQueue->stCcoQueue);
        if (!pstCcoItem)
        {
            MRS_UNLOCK(pstCcoQueue->ucLock);
            return HI_ERR_FAILURE;
        }

        if (HI_FALSE == pstCcoItem->bValid)
        {
            HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_004, HI_DIAG_MT("cco queue item invalid skip"),pstCcoItem->bValid);
            mrsCcoQueueEnd(pstCcoQueue);
            return HI_ERR_SUCCESS;
        }

        HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_005, HI_DIAG_MT("cco queue pro afn fn"),pstCcoItem->ucAfn,pstCcoItem->usFn);
        mrsDfxCcoQueueCurrAfn(pstCcoItem->ucAfn);
        mrsDfxCcoQueueCurrFn(pstCcoItem->usFn);
        MRS_SendMrData(pstCcoItem->pData, pstCcoItem->usDataLen, HI_DMS_CHL_UART_PORT_APP);
        mrsCcoSetQueueStatus(MRS_CCO_QUEUE_WAIT_ACK_STATUS);
        MRS_StartTimer(MRS_CCO_TIMER_MASTER_REPORT, pstCcoItem->usTimeOut, HI_SYS_TIMER_ONESHOT); 
        mrsDfxCcoQueueReportCnt();
        pstCcoQueue->ucRetry = 0;
        mrsDfxCcoQueueCurrRetry(pstCcoQueue->ucRetry);
    }

    return HI_ERR_SUCCESS;
}

HI_U32 mrsCcoQueueTimeOutPro(HI_VOID)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    P_MRS_SRV_CCO_QUEUE pstCcoQueue = mrsGetCcoQueue();
    MRS_SRV_CCO_ITEM *pstCcoItem = HI_NULL;
    MRS_SRV_CCO_ITEM *pstCcoItemTemp = HI_NULL;

    mrsDfxCcoQueueReportTimeOutCnt();
    pstCcoItem = mrsSrvQueueTop(&pstCcoQueue->stCcoQueue);
    if (!pstCcoItem)
    {
        MRS_UNLOCK(pstCcoQueue->ucLock);
        return HI_ERR_FAILURE;
    }

    HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_006, HI_DIAG_MT("retry maxtry"),pstCcoQueue->ucRetry,pstCcoItem->ucMaxTry);
    if (pstCcoQueue->ucRetry < pstCcoItem->ucMaxTry)
    {
        if (pstCcoItem->MrsCcoRxEveryTimeOutProc)
        {
            HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_007, HI_DIAG_MT("mrsCcoQueueTimeOutPro"));
            ulRet = pstCcoItem->MrsCcoRxEveryTimeOutProc(pstCcoItem->pParam);
            if (HI_ERR_CONTINUE != ulRet)
            {
                HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_008, HI_DIAG_MT("every timeout ret"),ulRet);
                mrsCcoQueueEnd(pstCcoQueue);
                return ulRet;
            }
        }
        
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_009, HI_DIAG_MT("retry send up"));
        pstCcoQueue->ucRetry++;
        mrsDfxCcoQueueCurrRetry(pstCcoQueue->ucRetry);
        MRS_SendMrData(pstCcoItem->pData, pstCcoItem->usDataLen, HI_DMS_CHL_UART_PORT_APP);
        mrsCcoSetQueueStatus(MRS_CCO_QUEUE_WAIT_ACK_STATUS);
        MRS_StartTimer(MRS_CCO_TIMER_MASTER_REPORT, pstCcoItem->usTimeOut, HI_SYS_TIMER_ONESHOT); 
        mrsDfxCcoQueueReportCnt();

        return HI_ERR_SUCCESS;
    }

    if (pstCcoItem->MrsCcoRxTotalTimeOutProc)
    {
        pstCcoItem->MrsCcoRxTotalTimeOutProc(pstCcoItem->pParam);
    }
    
    pstCcoItemTemp = mrsSrvQueueTop(&pstCcoQueue->stCcoQueue);
    if (pstCcoItem == pstCcoItemTemp)
    {
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_010, HI_DIAG_MT("timeout end"));
        mrsCcoQueueEnd(pstCcoQueue);
    }

    return ulRet;
}

HI_U32 mrsCcoQueueEnd(P_MRS_SRV_CCO_QUEUE pstCcoQueue)
{
    MRS_SRV_CCO_ITEM *pstCcoItem = HI_NULL;
    
    MRS_UNLOCK(pstCcoQueue->ucLock);
    mrsCcoSetQueueStatus(MRS_CCO_QUEUE_IDLE_STATUS);
    pstCcoItem = mrsCcoDeQueue(&pstCcoQueue->stCcoQueue);
    if (!pstCcoItem)
    {
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_059, HI_DIAG_MT("pstCcoItem null"));
        return HI_ERR_FAILURE;
    }
    
    HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_058, HI_DIAG_MT("pstCcoItem afn fn"), pstCcoItem->ucAfn,pstCcoItem->usFn);
    mrsToolsFree(pstCcoItem);
    pstCcoQueue->usItemNum--;
    pstCcoQueue->ucRetry = 0;
    mrsDfxCcoQueueCurrRetry(pstCcoQueue->ucRetry);
    mrsDfxCcoQueueCurrItem(pstCcoQueue->usItemNum);
    mrsDfxCcoQueueCurrAfn(0);
    mrsDfxCcoQueueCurrFn(0);

    HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_001, HI_DIAG_MT("this report finish,curr item num=%d"), pstCcoQueue->usItemNum);

    mrsActiveCcoQueue();
    
    return HI_ERR_SUCCESS;
}

HI_VOID* mrsCcoDeQueue(MRS_SRV_QUEUE *pstQueue)
{
    MRS_SRV_NODE * pstCcoNode = HI_NULL;

    pstCcoNode = mrsSrvDeQueue(pstQueue);

    return pstCcoNode;
}

HI_BOOL mrsCcoQueueFindRepeatItem(HI_VOID *pNode, HI_VOID *pParam)
{
    MRS_SRV_CCO_ITEM *pstItem = (MRS_SRV_CCO_ITEM *)pNode;
    P_MRS_CCO_QUEUE_FIND_RESULT pstMatchItem = (P_MRS_CCO_QUEUE_FIND_RESULT)pParam;
    
    if ((pstItem->ucAfn == pstMatchItem->ucAfn)
        && (pstItem->usFn== pstMatchItem->usFn))
    {
        HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_057, HI_DIAG_MT("pstItem afn fn"), pstItem->ucAfn,pstItem->usFn);
        pstItem->bValid = HI_FALSE;
    }

    return HI_FALSE;
}


HI_VOID mrsCcoQueueFreeNode(HI_VOID *pvNode)
{
    mrsToolsFree(pvNode);
}


HI_BOOL mrsCcoQueueMatchItem(HI_VOID *pvItem, HI_U8 ucAfn, HI_U16 usFn)
{
    MRS_SRV_CCO_ITEM *pstNode = (MRS_SRV_CCO_ITEM *)pvItem;

    if (!pstNode)
    {
        return HI_FALSE;
    }

    return (HI_BOOL)((pstNode->ucAfn == ucAfn) && (pstNode->usFn == usFn));
}


HI_VOID mrsCcoQueueDeleteItemDefault(HI_BOOL (*pfnCheckItem)(HI_VOID *))
{
    mrsCcoQueueDeleteItem(pfnCheckItem, mrsCcoQueueFreeNode);
}


HI_VOID mrsCcoQueueDeleteItem(HI_BOOL (*pfnCheckItem)(HI_VOID *), HI_VOID (*pfnFreeNode)(HI_VOID *))
{
    MRS_SRV_CCO_QUEUE *pstCcoQueue = mrsGetCcoQueue();
    MRS_SRV_QUEUE *pstQueue = &(pstCcoQueue->stCcoQueue);
    HI_VOID *pstItem = HI_NULL;

    if (!pfnCheckItem || !pfnFreeNode)
    {
        return;
    }

    pstItem = mrsSrvQueueTop(pstQueue);
    if (!pstItem)
    {
        MRS_UNLOCK(pstCcoQueue->ucLock);
        return;
    }

    if (pfnCheckItem(pstItem))
    {
        MRS_StopTimer(MRS_CCO_TIMER_MASTER_REPORT);
        mrsCcoQueueEnd(pstCcoQueue);
    }

    // ��ɾ�����������Ľ��
    // 1.  ��ͷ��㿪ʼ��ɾ���������������Ľ��
    while (!mrsSrvQueueIsEmpty(pstQueue))
    {
        pstItem = mrsSrvQueueTop(pstQueue);
        if (!pfnCheckItem(pstItem))
        {
            break;
        }

        pstItem = mrsSrvDeQueue(pstQueue);
        pfnFreeNode(pstItem);

        pstCcoQueue->usItemNum--;
    }

    // 2.  �ӵ�1�������������Ľ�㿪ʼ��ɾ�������ķ��������Ľ��
    if (!mrsSrvQueueIsEmpty(pstQueue))
    {
        MRS_SRV_NODE *pstNode = (MRS_SRV_NODE *)mrsSrvQueueTop(pstQueue);

        while (pstNode->next)
        {
            MRS_SRV_NODE *pstTemp = pstNode->next;

            if (pfnCheckItem(pstTemp))
            {
                pstNode->next = pstTemp->next;
                pfnFreeNode(pstTemp);
                pstCcoQueue->usItemNum--;
            }
            else
            {
                pstNode = pstTemp;
            }
        }

        pstQueue->rear = pstNode;
    }  

    mrsDfxCcoQueueCurrItem(pstCcoQueue->usItemNum);
}


HI_U32 mrsCcoQueueGetLength(HI_VOID)
{
    return mrsGetCcoQueue()->usItemNum;
}

#endif

