//*****************************************************************************
//
//                  ��Ȩ���� (C), 1998-2015, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mrs_srv_cco_slavenode_on_off_line.c
//  �� �� ��   : V1.0 
//  ��    ��   : liupeidong/00238134
//  ��������   : 2015-03-10
//  ��������   : CCO�ز��ӽڵ�����������ϱ������Ķ��塢������
//               
//  �����б�   : TODO: ...
//  �޸���ʷ   : 
//  1.��    �� : 2015-03-10
//    ��    �� : liupeidong/00238134
//    �޸����� : �����ļ� 
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

// ����ļ���CCO��������
#define mrsActivateChgFrameJoinCcoQueue()   MRS_StartTimer(EN_MRS_SRV_CCO_NODE_STATE_CHG_TIMER, 0, HI_SYS_TIMER_ONESHOT)

// CCO�ӽڵ�����״̬�仯�ϱ�������ȫ�ֱ���
MRS_NODE_STATE_CHG_REPORT_CTX g_stChgReport;


//*****************************************************************************
// ��������: mrsGetChgReportCtx
// ��������: ��ȡ�ӽڵ�����״̬�仯������ȫ�ֱ�����ַ
//
// ����˵��:
//   ��
//
// �� �� ֵ:
//   ָ��ӽڵ�����״̬�仯������ȫ�ֱ�����ָ��
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: liupeidong/00238134 [2015-03-23]
//*****************************************************************************
MRS_NODE_STATE_CHG_REPORT_CTX* mrsGetChgReportCtx(HI_VOID)
{
    return &g_stChgReport;
}


//*****************************************************************************
// ��������: mrsCcoNodeStateChgReportInit
// ��������: �ϵ��ʼ��CCO�ӽڵ�����״̬�仯�ϱ�������ȫ�ֱ���������
//           ����0x408 NV�ϱ��������ã�ע��/��ע��MAC�ṩ�Ľӿ�
// ����˵��:
//   ��
//
// �� �� ֵ:
//   ��
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: liupeidong/00238134 [2015-03-23]
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
// ��������: mrsMACReportChgEntryCallback
// ��������: ͨ��MAC�ṩ�Ľӿ�ע�ᵽMAC�Ļص�������������MAC�����������MAC��
//           ����һ���ӽڵ�״̬�仯��Ϣ��Ȼ����ݻ���Ԫ�ظ����ж���һ���Ĵ���
//           ����������ޣ��򽫻�����������Ϣ���һ֡������Ϣ��MRS����; ������
//           ��ʱ��ʱ����
// ����˵��:
//   HI_MAC_NETWORK_TOPOCHG_ENTRY_S* pstEntry MAC����һ���ӽڵ�״̬�仯��Ϣ
//
// �� �� ֵ:
//   ��
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: liupeidong/00238134 [2015-03-23]
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
    
    MRS_WaitSem(EN_APP_SEM_MRS_ND_STATE_CHG, HI_SYS_WAIT_FOREVER);  // �ź���������
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
    MRS_SignalSem(EN_APP_SEM_MRS_ND_STATE_CHG); // �ź���������

    return;
}


//*****************************************************************************
// ��������: mrsSaveChgEntry
// ��������: ����һ���ӽڵ�״̬�仯��Ϣ������
// 
// ����˵��:
//   HI_MAC_NETWORK_TOPOCHG_ENTRY_S* pstEntry MAC����һ���ӽڵ�״̬�仯��Ϣ
//
// �� �� ֵ:
//   HI_ERR_FAILURE         ����ʧ��
//   HI_ERR_MALLOC_FAILUE   ���붯̬�ڴ�ʧ��
//   HI_ERR_SUCCESS         ����ɹ�
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: liupeidong/00238134 [2015-03-23]
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
// ��������: mrsAssembleAllCacheEntries
// ��������: �����������е���Ŀ���һ֡
// 
// ����˵��:
//   HI_OUT HI_U8** ppucFrame    ��ɵ�֡�ĵ�ַ
//   HI_OUT HI_U16* pusFrameLen  ��ɵ�֡���ĵ�ַ
//   HI_OUT HI_U8* pucEntryNum   ֡���ŵ���Ŀ��
//
// �� �� ֵ:
//   HI_ERR_FAILURE         �����������Ĵ���
//   HI_ERR_MALLOC_FAILUE   ���붯̬�ڴ�ʧ��
//   HI_ERR_SUCCESS         ��֡�ɹ�
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: liupeidong/00238134 [2015-03-23]
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
    // �ϱ��ӽڵ�������
    (hi_void)memcpy_s(pucData, usDataLen, &usTotal, sizeof(usTotal));
    usOffset += sizeof(usTotal);
    // ��֡�ϱ��Ĵӽڵ�����
    pucData[usOffset++] = ucNum;
    // ��֡�ϱ��ĵ�һ���ӽڵ��������е���ʼ��ţ���0��ʼ
    (hi_void)memcpy_s(pucData + usOffset, usDataLen - usOffset, &usStartIndex, sizeof(usStartIndex));
    usOffset += sizeof(usStartIndex);
    // �ӽڵ�״̬�仯��Ϣ
    for (ucIndex = 0; ucIndex < ucNum; ucIndex++)
    {
        pstQueueItem = (MRS_CHG_ENTRY_QUEUE_ITEM*)mrsSrvDeQueue(&(pstCtx->stCache.stQueue));
        if (HI_NULL == pstQueueItem)
        {
            continue;
        }
        
        pstEntry = &(pstQueueItem->stEntry);
        // �ӽڵ�MAC��ַ
        mrsHexInvert(pstEntry->aucMac, HI_PLC_MAC_ADDR_LEN);
        (hi_void)memcpy_s(pucData + usOffset, usDataLen - usOffset, pstEntry->aucMac, HI_PLC_MAC_ADDR_LEN);
        usOffset += HI_PLC_MAC_ADDR_LEN;
        // �ӽڵ�״̬�仯
        pucData[usOffset++] = pstEntry->ucStateChange;
        // �ӽڵ�����ʱ��
        if (HI_ND_STATE_CHANGE_ONLINE_TO_OFFLINE == pstEntry->ucStateChange)
        {
            pstEntry->ulOfflineDuration = MRS_ONLINE_TO_OFFLINE_DURATION_DEFAULT;
            pstEntry->ucOfflineReason = 0;
        }
        (hi_void)memcpy_s(pucData + usOffset, usDataLen - usOffset, &pstEntry->ulOfflineDuration, sizeof(pstEntry->ulOfflineDuration));
        usOffset += sizeof(pstEntry->ulOfflineDuration);
        // �ӽڵ�����ԭ��
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
// ��������: mrsOfflineReasonFilter
// ��������: ����NV���õĿ��ؾ����Ƿ�ĳ������ԭ��ĳ�δ֪
// 
// ����˵��:
//   HI_U8 ucPowerOffReasonSwitch, ����ԭ�򿪹أ��������ϱ����رս�ԭ��ĳ�δ֪
//   HI_U8 ucPlcChlReasonSwitch,   �ŵ�ԭ�򿪹أ��������ϱ����رս�ԭ��ĳ�δ֪
//   HI_U8 ucChange,               ����״̬�ı仯
//   HI_OUT HI_U8* pucReason       ����ԭ��ĵ�ַ
//
// �� �� ֵ:
//   ��
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: liupeidong/00238134 [2015-03-23]
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
// ��������: mrsNodeStateChgSendMsg
// ��������: ��MAC���������еĻص������з���Ϣ��MRS������ɵ�֡�ͳ��ȴ���MRS����
// 
// ����˵��:
//   HI_U32 ulFramePtr, ��ɵ�֡�ĵ�ַ
//   HI_U32 ulFrameLen, ��ɵ�֡��
//   HI_U32 ulEntryNum, ��ɵ�֡�е���Ŀ��
//   HI_U32 ulParam4    �����ģ�����Ҫ4������
//   
// �� �� ֵ:
//   HI_ERR_SUCCESS         ����Ϣ��MRS�ɹ�
//   ����                   ����Ϣ��MRSʧ��
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: liupeidong/00238134 [2015-03-23]
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
// ��������: mrsNodeStateChgMsgProc
// ��������: MRS�յ�MAC���񷢴ӽڵ�״̬�仯�ϱ���Ϣ�Ĵ���
// 
// ����˵��:
//   HI_SYS_QUEUE_MSG_S* pstMsg  ��Ϣ�ĵ�ַ
//   
// �� �� ֵ:
//   HI_ERR_FAILURE         �����������Ĵ���������ʧ��
//   HI_ERR_MALLOC_FAILUE   ���붯̬�ڴ�ʧ��
//   HI_ERR_SUCCESS         ����CCO���гɹ�                 
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: liupeidong/00238134 [2015-03-23]
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
// ��������: mrsChgCacheTimeoutProc
// ��������: ����Ԫ�ظ���δ�����ޣ��յ�һ����Ϣ����Ķ�ʱ����Ĵ���
// 
// ����˵��:
//   ��
//   
// �� �� ֵ:
//   HI_ERR_FAILURE         �����������Ĵ������֡ʧ�ܻ������ʧ��
//   HI_ERR_MALLOC_FAILUE   ���붯̬�ڴ�ʧ��
//   HI_ERR_SUCCESS         ����CCO���гɹ�     
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: liupeidong/00238134 [2015-03-23]
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

    MRS_WaitSem(EN_APP_SEM_MRS_ND_STATE_CHG, HI_SYS_WAIT_FOREVER);  // �ź���������
    HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_006, HI_DIAG_MT("chg cache timeout, ucCacheNum = %d"), pstCtx->stCache.ucNum);
    ulRet = mrsAssembleAllCacheEntries(&pucFrame, &usFrameLen, &ucEntryNum);
    MRS_SignalSem(EN_APP_SEM_MRS_ND_STATE_CHG); // �ź���������
    if (HI_ERR_SUCCESS != ulRet)
    {
        return ulRet;
    }
    
    return mrsChgFrameReport(pucFrame, usFrameLen, ucEntryNum);
}


//*****************************************************************************
// ��������: mrsChgFrameReport
// ��������: �����֡�ϱ�: �ȼ����ϱ����У��ټ���CCO����
// 
// ����˵��:
//   HI_U8* pucFrame,   ֡�ĵ�ַ
//   HI_U16 usFrameLen  ֡��
//   HI_U8  ucEntryNum  ֡�е���Ŀ��
//   
// �� �� ֵ:
//   HI_ERR_FAILURE         �����������Ĵ���������ʧ��
//   HI_ERR_MALLOC_FAILUE   ���붯̬�ڴ�ʧ��
//   HI_ERR_SUCCESS         ����CCO���гɹ�
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: liupeidong/00238134 [2015-03-23]
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
            // pucFrame֮�������������Ҫ��֤�ͷ�pucFrame�ڴ�
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
        // ��������в��ɹ����������Ҫ�ͷű����ڴ�
        mrsToolsFree(pucFrame);
    }
    else
    {
        ulRet = mrsChgFrameJoinCcoQueue();
    }
    
    return ulRet;
}


//*****************************************************************************
// ��������: mrsChgFrameJoinCcoQueue
// ��������: ����Ĵӽڵ�����״̬�仯֡����CCO����
// 
// ����˵��:
//   ��
//   
// �� �� ֵ:
//   HI_ERR_FAILURE         �����������Ĵ���
//   HI_ERR_MALLOC_FAILUE   ���붯̬�ڴ�ʧ��
//   HI_ERR_SUCCESS         ��CCO���гɹ�
//   HI_ERR_BUSY            CCO�������������ϱ��ı仯֡����ʱ���������
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: liupeidong/00238134 [2015-03-23]
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
        // �Ѿ�������CCO���������е�һ֡վ������״̬�仯֡û�����꣬����ʧ��
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_009, HI_DIAG_MT("chg JoinCcoQueueLock is ON, join fail"));
        ulRet = HI_ERR_BUSY;
    }
    
    return ulRet;
}


//*****************************************************************************
// ��������: mrsChgReportRxRespProc
// ��������: CCO�ϱ����������յ�ȷ��֡/����֡
// 
// ����˵��:
//   HI_VOID* pParam  ռλ�Ĳ�����ʵ������
//   
// �� �� ֵ:
//   ��
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: liupeidong/00238134 [2015-03-23]
//*****************************************************************************
HI_VOID mrsChgReportRxRespProc(HI_VOID* pParam)
{
    HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_010, HI_DIAG_MT("chg report rx resp"));
    MRS_NOT_USED(pParam);
    
    mrsDfxCcoChgReportRxResp();
    mrsReportNextChgFrame();
}


//*****************************************************************************
// ��������: mrsChgReportTimeoutProc
// ��������: CCO�ϱ������������еķ����ڳ�ʱʱ���ڶ�û���յ�Ӧ��Ĵ���
// 
// ����˵��:
//   HI_VOID* pParam  ռλ�Ĳ�����ʵ������
//   
// �� �� ֵ:
//   HI_ERR_FAILURE         �����Ĵ���
//   HI_ERR_SUCCESS         �ɹ�������һ֡��CCO����
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: liupeidong/00238134 [2015-03-23]
//*****************************************************************************
HI_U32 mrsChgReportTimeoutProc(HI_VOID* pParam)
{
    HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_011, HI_DIAG_MT("chg report timeout"));
    MRS_NOT_USED(pParam);

    mrsDfxCcoChgReportTimeout();
    return mrsReportNextChgFrame();
}


//*****************************************************************************
// ��������: mrsReportNextChgFrame
// ��������: �����֡�ϱ�: �ȼ����ϱ����У��ټ���CCO����
// 
// ����˵��:
//   ��
//   
// �� �� ֵ:
//   HI_ERR_FAILURE         �����Ĵ���
//   HI_ERR_SUCCESS         �ɹ�������һ֡��CCO����
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: liupeidong/00238134 [2015-03-23]
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

