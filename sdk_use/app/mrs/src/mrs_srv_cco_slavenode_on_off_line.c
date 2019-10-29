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
#include "hi_mdm_mac.h"
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
#include "mrs_srv_cco.h"

#if defined (PRODUCT_OPTIMIZE_MATTEO)
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)

// ����ļ���CCO��������
#define mrsActivateChgFrameJoinCcoQueue()                           \
    do                                                              \
    {                                                               \
        HI_SYS_QUEUE_MSG_S stMsg = { 0 };                           \
        stMsg.ulMsgId = ID_MRS_MSG_MR_TIMER;                        \
        stMsg.ulParam[0] = EN_MRS_SRV_CCO_NODE_STATE_CHG_TIMER;     \
        (HI_VOID)mrsSendMessage2Queue(&stMsg);                               \
    }while(0)


// CCO�ӽڵ�����״̬�仯�ϱ�������ȫ�ֱ���
MRS_NODE_STATE_CHG_REPORT_CTX g_stChgReport = {0};


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
    g_stChgReport.pstCheckStateList = (MRS_POWEROFF_CHECK_LIST_S *)mrsToolsMalloc(sizeof(MRS_POWEROFF_CHECK_LIST_S));
    if (!g_stChgReport.pstCheckStateList)
    {
        return;
    }
    g_stChgReport.pstHistoryList = (MRS_POWEROFF_HISTORY_LIST_S *)mrsToolsMalloc(sizeof(MRS_POWEROFF_HISTORY_LIST_S));
    if (!g_stChgReport.pstHistoryList)
    {
        mrsToolsFree(g_stChgReport.pstCheckStateList);
        return;
    }
    (hi_void)memset_s(g_stChgReport.pstCheckStateList, sizeof(MRS_POWEROFF_CHECK_LIST_S), 0,sizeof(MRS_POWEROFF_CHECK_LIST_S));
    (hi_void)memset_s(g_stChgReport.pstHistoryList, sizeof(MRS_POWEROFF_HISTORY_LIST_S), 0,sizeof(MRS_POWEROFF_HISTORY_LIST_S));
	g_stChgReport.pstCheckStateList->bDetectComplete  = HI_TRUE;
    g_stChgReport.pstCheckStateList->bRecheckComplete = HI_TRUE;
    if (g_stChgReport.ulNoPassDetectPeriod > 0)
    {
        MRS_StartTimer(MRS_CCO_TIMER_POWEROFF_STABLE_CHECK, MRS_SEC_TO_MS(g_stChgReport.ulNoPassDetectPeriod),
                        HI_SYS_TIMER_ONESHOT);
    }
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
    MRS_CHG_ENTRY_QUEUE_ITEM * pstQueueNode = HI_NULL;
    HI_SYS_QUEUE_MSG_S stMsg = {0};

    if (HI_NULL == pstEntry || HI_NULL == pstCtx)
    {
        return;
    }

    //���յ��ϱ��󼴼�����mac�����У���������а����˸õ㣬���ϱ��õ㣬�Ӷ������Ƴ�
    HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_001, HI_DIAG_MT("chg callback, Mac[5]=%d, OfflineReason=%d"),pstEntry->aucMac[5],pstEntry->ucOfflineReason);

    //δ�����ǵ���ģ���ͣ�繦��ʱ�����յ��ϱ���������
    if(HI_FALSE == pstEntry->is_init_poweroff)
    {
        return;
    }

    MRS_WaitSem(EN_APP_SEM_MRS_ND_STATE_CHG, HI_SYS_WAIT_FOREVER);  // �ź���������
    do
    {
        // �����
        pstQueueNode = mrsToolsMalloc(sizeof(MRS_CHG_ENTRY_QUEUE_ITEM));
        if (!pstQueueNode)
        {
            break;
        }

        (hi_void)memset_s(pstQueueNode, sizeof(MRS_CHG_ENTRY_QUEUE_ITEM), 0,sizeof(MRS_CHG_ENTRY_QUEUE_ITEM));
        (hi_void)memcpy_s(&pstQueueNode->stEntry, sizeof(HI_MDM_NETWORK_TOPOCHG_ENTRY_S),pstEntry, sizeof(HI_MDM_NETWORK_TOPOCHG_ENTRY_S));
        mrsSrvEnQueue(&pstCtx->stMacCache.stQueue, &pstQueueNode->stLink);
        pstCtx->stMacCache.usNum++;
        HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_026, HI_DIAG_MT("[poweroff] en queue"), pstCtx->stMacCache.usNum);
        MRS_StopTimer(MRS_CCO_TIMER_POWEROFF_MAC_CACHE);
        if (pstCtx->stMacCache.usNum >= pstCtx->usMacReportCacheMaxNum)
        {
            stMsg.ulMsgId = EN_MRS_FW_MSG_MAC_ND_STATE_CHG;
            ulRet = mrsSendMessage2Queue(&stMsg);
            if (HI_ERR_SUCCESS != ulRet)
            {
                break;
            }

            HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_024, HI_DIAG_MT("[poweroff] send msg success"));
        }
        else
        {
            MRS_StartTimer(MRS_CCO_TIMER_POWEROFF_MAC_CACHE, MRS_SEC_TO_MS(pstCtx->ucMacReportCacheMaxTime),
                           HI_SYS_TIMER_ONESHOT);
            HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_025, HI_DIAG_MT("[poweroff] start timer success"),pstCtx->ucMacReportCacheMaxTime);
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

    MRS_CHG_ENTRY_QUEUE_ITEM* pstEntryItem = HI_NULL;
    MRS_POWEROFF_CHECK_ITEM_S * pstItem = HI_NULL;
    MRS_NODE_STATE_CHG_REPORT_CTX* pstCtx = mrsGetChgReportCtx();

    if (HI_NULL == pstCtx || HI_NULL == pstEntry)
    {
        return HI_ERR_FAILURE;
    }

    //�˴������ж��Ƿ���Ҫ������֡
    //�����ǰ����Ҫ�ж϶������Ƿ������վ����Ϣ
    if(mrsPowerOffCheckFrameQueue(pstEntry->aucMac) != HI_ERR_SUCCESS)
    {
        HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_055, HI_DIAG_MT("same mac in stmaccache queue"),pstEntry->aucMac[5]);
        pstItem = mrsGetPoweroffCheckItem(pstCtx, pstEntry->aucMac);

        if(HI_NULL == pstItem)
        {
            return HI_ERR_FAILURE;
        }

        pstItem->ucStatus = MRS_POWEROFF_DETECT_SUCCESS;
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
    HI_U8* pucCheckMac = HI_NULL;

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

    //���� mac ��ַ�ռ�
    pucCheckMac = mrsToolsMalloc(ucNum * HI_PLC_MAC_ADDR_LEN);
    if (HI_NULL == pucCheckMac)
    {
        return HI_ERR_MALLOC_FAILUE;
    }

    (hi_void)memset_s(pucCheckMac, ucNum * HI_PLC_MAC_ADDR_LEN, 0, ucNum * HI_PLC_MAC_ADDR_LEN);

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
        MRS_TOOLS_FE_TO_00(pstEntry->aucMac[0]);
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

    ulRet = mrsCreate3762UpFrame(&stAfnData, ppucFrame, pusFrameLen, HI_NULL, HI_NULL);
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

    mrsNodeStateChgPowerOffProc();
    return mrsChgFrameReport(pucFrame, usFrameLen, ucEntryNum);
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
HI_U32 mrsNodeStateChgPowerOffProc(HI_VOID)
{
    MRS_NODE_STATE_CHG_REPORT_CTX* pstCtx = mrsGetChgReportCtx();
    MRS_CHG_ENTRY_QUEUE_ITEM * pstQueueItem = HI_NULL;
    HI_U32  index = 0;
    HI_U32  ulOffNum = 0;
    HI_U32  ulOnNum  = 0;
    HI_U32  ulRet = HI_ERR_SUCCESS;

    if (!pstCtx || !pstCtx->ucReportEnable)
    {
        return HI_ERR_SUCCESS;
    }

    MRS_WaitSem(EN_APP_SEM_MRS_ND_STATE_CHG, HI_SYS_WAIT_FOREVER);  // �ź���������

    for (index = 0; index < pstCtx->stMacCache.usNum; index++)
    {
        pstQueueItem = (MRS_CHG_ENTRY_QUEUE_ITEM *)mrsSrvDeQueue(&pstCtx->stMacCache.stQueue);
        if (!pstQueueItem)
        {
            continue;
        }

        //�������ݳ�����

        if (HI_ND_STATE_CHANGE_OFFLINE_TO_ONLINE == pstQueueItem->stEntry.ucStateChange)
        {
            mrsPoweroffHandleNodeOn(pstCtx, &pstQueueItem->stEntry);
            ulOnNum++;
        }
        else
        {
            mrsPoweroffHandleNodeOff(pstCtx, &pstQueueItem->stEntry);
            ulOffNum++;
        }

        mrsToolsFree(pstQueueItem);
    }

    pstCtx->stMacCache.usNum = 0;

    MRS_SignalSem(EN_APP_SEM_MRS_ND_STATE_CHG); // �ź���������

    if (pstCtx->pstCheckStateList->bDetectComplete && ulOffNum > 0)
    {
        pstCtx->pstCheckStateList->bDetectComplete = HI_FALSE;
        pstCtx->pstCheckStateList->usDetectIdx = 0;

        if (pstCtx->pstCheckStateList->bRecheckComplete)
        {
            pstCtx->ulGetPoweroffInfoSeq++;
        }

        ulRet = mrsPoweroffRunDetectProcess(pstCtx);
    }

    //����ʱ����recheck
    if (pstCtx->pstCheckStateList->bRecheckComplete && ulOnNum > 0)
    {
        pstCtx->pstCheckStateList->usRecheckIdx = 0;
        pstCtx->pstCheckStateList->bRecheckComplete = HI_FALSE;

        MRS_StopTimer(MRS_CCO_TIMER_POWEROFF_RECHECK);
        MRS_StartTimer(MRS_CCO_TIMER_POWEROFF_RECHECK, MRS_SEC_TO_MS(pstCtx->ucPoweroffRecheckInterval), HI_SYS_TIMER_PERIODIC);
    }

    return ulRet;
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
HI_U32 mrsPowerOffCheckFrameQueue(HI_U8 *pstCheckMac)
{
    MRS_NODE_STATE_CHG_REPORT_CTX* pstCtx = mrsGetChgReportCtx();
    MRS_CHG_ENTRY_QUEUE_ITEM* pstQueueItem = HI_NULL;
    HI_MDM_NETWORK_TOPOCHG_ENTRY_S * pstEntry = HI_NULL;
    HI_U8 ucNum = 0;
    HI_U16 i = 0;
    HI_U32 ret = HI_ERR_SUCCESS;

    ucNum = pstCtx->stCache.ucNum;
    HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1010, HI_DIAG_MT("cco check queue mac_addr"),pstCheckMac,HI_PLC_MAC_ADDR_LEN);

    for(i = 0; i < ucNum; i++)
    {
        pstQueueItem = (MRS_CHG_ENTRY_QUEUE_ITEM*)mrsSrvDeQueue(&(pstCtx->stCache.stQueue));
        if (HI_NULL == pstQueueItem)
        {
            continue;
        }

        pstEntry = &(pstQueueItem->stEntry);

        HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1011, HI_DIAG_MT("queue mac_addr"),pstEntry->aucMac,HI_PLC_MAC_ADDR_LEN);

        //�����д��ڸ�վ�㣬�����к��ͷſռ䲻�ټ������
        if(memcmp(pstEntry->aucMac, pstCheckMac, HI_PLC_MAC_ADDR_LEN) == 0)
        {
            mrsToolsFree(pstQueueItem);
            pstCtx->stCache.ucNum--;
            ret = HI_ERR_FAILURE;
            break;
        }
        else
        {
            mrsSrvEnQueue(&(pstCtx->stCache.stQueue), &(pstQueueItem->stLink));
        }
    }

    return ret;
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

        //����õ�֡���뵽����֮��
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
        pstCcoItem->usTimeOut = (HI_U16)mrsToolsGetTimeOut(pstCtx->usReportTimeout);
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


// ��ȡ վ���Ӧ��ͣ����ڵ�
MRS_POWEROFF_CHECK_ITEM_S * mrsGetPoweroffCheckItem(MRS_NODE_STATE_CHG_REPORT_CTX * pstCtx, HI_U8 aucMac[HI_PLC_MAC_ADDR_LEN])
{
    MRS_POWEROFF_CHECK_ITEM_S * pstItems = HI_NULL;
    HI_U32 ulHashKey = 0;
    HI_U32 ulKeyIdx = 0;
    HI_U32 ulIdx = 0;

    if (!pstCtx || !pstCtx->pstCheckStateList || !aucMac)
    {
        return HI_NULL;
    }

    pstItems = pstCtx->pstCheckStateList->astItems;

    ulHashKey = (((HI_U32)aucMac[MRS_MAC_ADDR_INDEX_4] << MRS_BIT_PER_BYTE) + aucMac[MRS_MAC_ADDR_INDEX_5]) % PRODUCT_CFG_MRS_MAX_TOPO_NUM;

    for (ulKeyIdx = ulHashKey; ulKeyIdx < ulHashKey + PRODUCT_CFG_MRS_MAX_TOPO_NUM; ulKeyIdx++)
    {
        ulIdx = ulKeyIdx % PRODUCT_CFG_MRS_MAX_TOPO_NUM;
        if (mrsToolsMemEq(pstItems[ulIdx].aucMac, aucMac, HI_PLC_MAC_ADDR_LEN))
        {
            return &pstItems[ulIdx];
        }

        if (mrsToolsZeroAddr(pstItems[ulIdx].aucMac))
        {
            (hi_void)memset_s(&pstItems[ulIdx], sizeof(MRS_POWEROFF_CHECK_ITEM_S), 0,sizeof(MRS_POWEROFF_CHECK_ITEM_S));
            (hi_void)memcpy_s(pstItems[ulIdx].aucMac, HI_PLC_MAC_ADDR_LEN,aucMac, HI_PLC_MAC_ADDR_LEN);
            pstItems[ulIdx].ucReportCheckScore = pstCtx->ucReviewScoreFull;
            return &pstItems[ulIdx];
        }
    }

    // û�ҵ�MAC, ���޿�λ�á���һ���������˱��ڵ�
    for (ulKeyIdx = ulHashKey; ulKeyIdx < ulHashKey + PRODUCT_CFG_MRS_MAX_TOPO_NUM; ulKeyIdx++)
    {
        ulIdx = ulKeyIdx % PRODUCT_CFG_MRS_MAX_TOPO_NUM;

        if (!mrsCheckStaIsInTopoInf(pstItems[ulIdx].aucMac))
        {
            (hi_void)memset_s(&pstItems[ulIdx], sizeof(MRS_POWEROFF_CHECK_ITEM_S), 0,sizeof(MRS_POWEROFF_CHECK_ITEM_S));
            (hi_void)memcpy_s(pstItems[ulIdx].aucMac, HI_PLC_MAC_ADDR_LEN,aucMac, HI_PLC_MAC_ADDR_LEN);
            pstItems[ulIdx].ucReportCheckScore = pstCtx->ucReviewScoreFull;
            return &pstItems[ulIdx];
        }
    }

    return HI_NULL;
}


// ���� MAC���ϱ���վ������
HI_U32 mrsPoweroffHandleNodeOff(MRS_NODE_STATE_CHG_REPORT_CTX * pstCtx, HI_MDM_NETWORK_TOPOCHG_ENTRY_S * pstEntry)
{
    MRS_POWEROFF_CHECK_ITEM_S * pstItem = HI_NULL;

    if (!pstCtx || !pstCtx->pstCheckStateList || !pstEntry)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    //���ϱ�վ����Ϣ���뵽mac��ַ��
    pstItem = mrsGetPoweroffCheckItem(pstCtx, pstEntry->aucMac);
    if (!pstItem)
    {
        return HI_ERR_FAILURE;
    }

    HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1003, HI_DIAG_MT("Poweroff Handle Node Off [MAC]"), pstItem->aucMac, HI_PLC_MAC_ADDR_LEN);
    HI_DIAG_LOG_MSG_E4(MRS_FILE_LOG_FLAG_015,
        HI_DIAG_MT("Poweroff Node Statue: [detect_stat] [detect_complete] [recheck_stat] [recheck_complete] !"),
        pstItem->ucStatus, pstCtx->pstCheckStateList->bDetectComplete,
        pstItem->ucReCheckStatus, pstCtx->pstCheckStateList->bRecheckComplete);

    // ���ϴ����߻��ڸ�����: ��Ϊ����ʧ�ܣ�������ʷ��¼
    if (MRS_POWERON_RECHECKING == pstItem->ucReCheckStatus)
    {
        pstItem->ucReCheckStatus = MRS_POWERON_RECHECKED;
        pstItem->ucRecheckFailCnt++;
        pstCtx->usRecheckFailSum++;
        mrsPoweroffSaveHistory(pstCtx, pstItem);
    }

    pstItem->ucStatus = MRS_POWEROFF_WAIT_DETECTING;
    pstItem->ulLastMacReportOffTime = HI_MDM_GetRealTime();
    pstItem->ucDetectQueryCnt = 0;
    pstItem->usMacReportOffCnt++;
    pstCtx->usMacReportOffSum++;
    HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_042,
        HI_DIAG_MT("power off time "),pstItem->ulLastMacReportOffTime);

    return HI_ERR_SUCCESS;
}


// ����MAC���ϱ�վ������
HI_U32 mrsPoweroffHandleNodeOn(MRS_NODE_STATE_CHG_REPORT_CTX * pstCtx, HI_MDM_NETWORK_TOPOCHG_ENTRY_S * pstEntry)
{
    MRS_POWEROFF_CHECK_ITEM_S * pstItem = HI_NULL;

    if (!pstCtx || !pstCtx->pstCheckStateList || !pstEntry)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    pstItem = mrsGetPoweroffCheckItem(pstCtx, pstEntry->aucMac);
    if (!pstItem)
    {
        return HI_ERR_FAILURE;
    }

    HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1002, HI_DIAG_MT("Poweroff Handle Node On [MAC]"), pstItem->aucMac, HI_PLC_MAC_ADDR_LEN);
    HI_DIAG_LOG_MSG_E4(MRS_FILE_LOG_FLAG_016,
        HI_DIAG_MT("Poweroff Node Statue: [detect_stat] [detect_complete] [recheck_stat] [recheck_complete] !"),
        pstItem->ucStatus, pstCtx->pstCheckStateList->bDetectComplete,
        pstItem->ucReCheckStatus, pstCtx->pstCheckStateList->bRecheckComplete);

    // ����̽���У�ֱ����Ϊ̽��ɹ�
    if (MRS_POWEROFF_DETECTING == pstItem->ucStatus)
    {
        pstItem->ucStatus = MRS_POWEROFF_DETECT_SUCCESS;
        pstCtx->usDetectSuccessSum++;
    }

    pstItem->ulLastMacReportOnTime = HI_MDM_GetRealTime();
    pstItem->usMacReportOnCnt++;
    pstCtx->usMacReportOnSum++;

    // ���ô��������ϱ������������ϱ�����
    if (MRS_POWEROFF_REPORTED == pstItem->ucStatus)
    {
        mrsPoweroffReportStateChange(pstEntry);
        pstItem->ulLastReportOnTime = pstItem->ulLastMacReportOnTime;
        pstCtx->us3762ReportOnSum++;
    }

    // ��Ϊ������
    pstItem->ucReCheckStatus = MRS_POWERON_RECHECKING;
    pstItem->ucRecheckQueryCnt = 0;

    return HI_ERR_SUCCESS;
}

//ͨ���жϸ�վ��̽�����ȷ����վ���Ƿ����ߣ�5min��̽��ɹ�������Ϊ��վ�����
HI_U32 mrsCheckNodeOffByPcoDetect(HI_U8 *poweroff_mac)
{
    HI_U32 ulindex = 0;
    HI_U16 pco_tei = 0;
    HI_U32 cur_time = 0;
    MRS_POWEROFF_CHECK_ITEM_S * pstItem = HI_NULL;
    MRS_POWEROFF_CHECK_ITEM_S * pstPcoItem = HI_NULL;
    MRS_NODE_STATE_CHG_REPORT_CTX* pstCtx = mrsGetChgReportCtx();
    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();
    HI_MAC_NETWORK_TOPO_ENTRY_S *topo_entry = pstCcoCtx->stTopoInf.entry;

    cur_time = HI_MDM_GetRealTime();

    for(ulindex = 1; ulindex < HI_MAC_NETWORK_TOPO_ENTRY_MAX; ulindex++)
    {
        if(memcmp(topo_entry[ulindex].mac, poweroff_mac,HI_PLC_MAC_ADDR_LEN) == 0)
        {
            //�ҵ�����վ��tei
            pco_tei = topo_entry[ulindex].proxy_tei;
            break;
        }

    }

    if(pco_tei < 1)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1008, HI_DIAG_MT("Poweroff  Pco Sta Detect [MAC]"), topo_entry[pco_tei - 1].mac, HI_PLC_MAC_ADDR_LEN);
    pstPcoItem = mrsGetPoweroffCheckItem(pstCtx, topo_entry[pco_tei - 1].mac);
    pstItem = mrsGetPoweroffCheckItem(pstCtx, poweroff_mac);

    if (!pstItem || !pstPcoItem)
    {
        return HI_ERR_FAILURE;
    }

    //����վ�㴦��̽��ɹ�״̬��ͬʱʱ���ڹ涨��ʱ���ڣ��ж�վ���ͣ��״̬
    if(MRS_POWEROFF_DETECT_SUCCESS == pstPcoItem->ucStatus &&
       mrsTimeSubDuration(cur_time, pstPcoItem->ulLastMacReportOffTime) < MRS_DETECT_PCO_SUCCESS_MAX_INTERVAL)
    {
        //�ⲿ���ø�վ���״̬
        HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_048, HI_DIAG_MT("power off pco detected before not power off "),pstPcoItem->ulLastMacReportOffTime);
        return HI_ERR_SUCCESS;
    }

    return HI_ERR_FAILURE;
}

HI_VOID mrsPowerOffDealScaleOff(HI_VOID)
{
    HI_U16 usIdx = 0;
    HI_U32 detect_success_num = 0;
    HI_U32 detecting_num = 0;
    HI_U32 cur_time = 0;
    MRS_POWEROFF_CHECK_LIST_S * pstList = HI_NULL;
    MRS_NODE_STATE_CHG_REPORT_CTX *pstCtx = mrsGetChgReportCtx();

    pstList = pstCtx->pstCheckStateList;
    cur_time = HI_MDM_GetRealTime();
    pstCtx->is_scale_power_off = HI_FALSE;

    //ͳ��̽��ɹ���̽���е���Ŀ
    //���ܴ�������̽��ɹ�����Ŀ����ʵ�ʵ����
    for (usIdx = 0; usIdx < PRODUCT_CFG_MRS_MAX_TOPO_NUM; usIdx++)
    {
        //̽��ɹ�״̬����ʱ����С��100s ȷ��Ϊ����̽��ɹ���
        if (MRS_POWEROFF_DETECT_SUCCESS == pstList->astItems[usIdx].ucStatus
            && mrsTimeSubDuration(cur_time, pstList->astItems[usIdx].ulLastMacReportOffTime) < MRS_DEAL_SCALE_OFF_INTERVAL)
        {
            detect_success_num++;
        }

        if(MRS_POWEROFF_DETECTING == pstList->astItems[usIdx].ucStatus)
        {
            detecting_num++;
        }
    }

    HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_053, HI_DIAG_MT("power off scale node off "),detect_success_num, detecting_num);
    //̽��ɹ���Ŀ����50%,����վ������Ϊ̽��ɹ������뱣��״̬
    if(detect_success_num > detecting_num)
    {
        for (usIdx = 0; usIdx < PRODUCT_CFG_MRS_MAX_TOPO_NUM; usIdx++)
        {
            if(MRS_POWEROFF_DETECTING == pstList->astItems[usIdx].ucStatus
                || MRS_POWEROFF_DETECT_SUCCESS == pstList->astItems[usIdx].ucStatus)
            {
                HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1007, HI_DIAG_MT("Poweroff Scale Off Protecting mac"), pstList->astItems[usIdx].aucMac, HI_PLC_MAC_ADDR_LEN);
                pstList->astItems[usIdx].ucStatus = MRS_POWEROFF_DETECT_SUCCESS;
                HI_MDM_NM_PowerOffSetNodeProtectingState(pstList->astItems[usIdx].aucMac);
            }
        }
    }
}

// ִ�� ̽������
HI_U32 mrsPoweroffRunDetectProcess(MRS_NODE_STATE_CHG_REPORT_CTX * pstCtx)
{
    MRS_POWEROFF_CHECK_LIST_S * pstList = HI_NULL;
    MRS_PLC_POWEROFF_INFO_DL_S * pstFrame = HI_NULL;
    HI_U8 aucDstMac[HI_PLC_MAC_ADDR_LEN] = {0};
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U16 usIdx = 0;

    HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_004, HI_DIAG_MT("Poweroff Into New Detect Round !"));

    if (!pstCtx || !pstCtx->pstCheckStateList)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    ulRet = mrsCcoCreatePoweroffInfoFrame(pstCtx, &pstFrame);
    if (HI_ERR_SUCCESS != ulRet)
    {
        return ulRet;
    }

    MRS_StopTimer(MRS_CCO_TIMER_POWEROFF_DETECT);
    pstList = pstCtx->pstCheckStateList;

    for (usIdx = 0;
        usIdx < PRODUCT_CFG_MRS_MAX_TOPO_NUM && pstFrame->sta_num < MRS_PLC_POWEROFF_INFO_DL_MAC_NUM; usIdx++)
    {
        //���ô��ģ̽����Ժ���������վ��Ϊ̽��ɹ�
        if (MRS_POWEROFF_WAIT_DETECTING != pstList->astItems[usIdx].ucStatus
            && MRS_POWEROFF_DETECTING != pstList->astItems[usIdx].ucStatus)
        {
            continue;
        }

        HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1001, HI_DIAG_MT("Poweroff Detecting [MAC]"),
            pstList->astItems[usIdx].aucMac, HI_PLC_MAC_ADDR_LEN);

        // ���Գ���������: ̽��ʧ��
        if (mrsPoweroffCheckIsDetectFail(pstCtx, &pstList->astItems[usIdx]))
        {
            ulRet = mrsPoweroffHandleDetectFail(pstCtx, &pstList->astItems[usIdx]);

            HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_043, HI_DIAG_MT("Poweroff Detect Fail: [ulRet]"), ulRet);
            continue;
        }

        // ���뵽��Ҫ���͵�̽��֡��ȥ, ����վ��״̬Ϊ̽����״̬
        (hi_void)memcpy_s(pstFrame->mac_list + pstFrame->sta_num * HI_PLC_MAC_ADDR_LEN, HI_PLC_MAC_ADDR_LEN,pstList->astItems[usIdx].aucMac, HI_PLC_MAC_ADDR_LEN);
        pstList->astItems[usIdx].ucStatus = MRS_POWEROFF_DETECTING;
        pstFrame->sta_num++;
        pstList->astItems[usIdx].ucDetectQueryCnt++;

        HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_013, HI_DIAG_MT("Poweroff Detecting Add To Frame: [sta_num] [query_cnt]"),
            pstFrame->sta_num, pstList->astItems[usIdx].ucDetectQueryCnt);
    }

    //��̽��վ����ĿΪ10��ʱ���������������ߴ������
    if(MRS_PLC_POWEROFF_INFO_DL_MAC_NUM == pstFrame->sta_num && HI_FALSE == pstCtx->is_scale_power_off)
    {
        HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_051, HI_DIAG_MT("Poweroff Deal SCALE Nodes OFF: [num]"), pstCtx->ucScalePowerOffDetectInterval);
        MRS_StartTimer(MRS_CCO_TIMER_POWEROFF_DEAL_SCALE_OFF, MRS_SEC_TO_MS(pstCtx->ucScalePowerOffDetectInterval), HI_SYS_TIMER_ONESHOT);
        pstCtx->is_scale_power_off= HI_TRUE;
    }

    if (pstFrame->sta_num > 0)
    {
        pstFrame->detect_flag = HI_TRUE;
        (hi_void)memcpy_s(aucDstMac, HI_PLC_MAC_ADDR_LEN,MRS_BROADCAST_MAC, HI_PLC_MAC_ADDR_LEN);
        ulRet = mrsCcoGetPoweroffInfo(pstFrame, aucDstMac);

        MRS_StartTimer(MRS_CCO_TIMER_POWEROFF_DETECT, MRS_SEC_TO_MS(pstCtx->ucPoweroffDetectInterval), HI_SYS_TIMER_PERIODIC);
    }
    else
    {
        pstList->bDetectComplete = HI_TRUE;
        ulRet = HI_ERR_SUCCESS;
    }

    mrsToolsFree(pstFrame);

    HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_014, HI_DIAG_MT("Poweroff Detect Round Finish: [ulRet]"), ulRet);

    return ulRet;
}


// ��� �Ƿ�̽��ʧ��
HI_BOOL mrsPoweroffCheckIsDetectFail(MRS_NODE_STATE_CHG_REPORT_CTX * pstCtx, MRS_POWEROFF_CHECK_ITEM_S * pstItem)
{
    HI_U8 ucMaxTries = 0;

    if (!pstCtx || !pstItem)
    {
        return HI_TRUE;
    }

    ucMaxTries = pstCtx->ucPoweroffDetectMaxTries;
    if (pstItem->ulLastMacReportWrongTime > 0
        && mrsTimeSubDuration(HI_MDM_GetSeconds(), pstItem->ulLastMacReportWrongTime) <= pstCtx->ulMacReportWrongPeriodTh)
    {
        ucMaxTries += pstCtx->ucPoweroffDetectExpTries;
    }

    return (pstItem->ucDetectQueryCnt >= ucMaxTries);
}


// ���� վ��̽��ʧ��
HI_U32 mrsPoweroffHandleDetectFail(MRS_NODE_STATE_CHG_REPORT_CTX * pstCtx, MRS_POWEROFF_CHECK_ITEM_S * pstItem)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_MDM_NETWORK_TOPOCHG_ENTRY_S stEntry;

    if (!pstCtx || !pstItem)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    if (MRS_POWEROFF_DETECTING != pstItem->ucStatus)
    {
        return HI_ERR_FAILURE;
    }

    pstItem->bPassedDetect = HI_TRUE;

    //�鿴�Ƿ�̽�����վ��Ĵ���վ�㣬����ڹ涨ʱ����̽�����վ����Ϣ������Ϊ��վ��û��ͣ�磬��������
    //����success ˵��ģ���趨̽��ɹ�״̬�����ٽ���̽��
    ret = mrsCheckNodeOffByPcoDetect(pstItem->aucMac);

    if(HI_ERR_SUCCESS == ret)
    {
        pstItem->ucStatus = MRS_POWEROFF_INTERCEPT_BY_PCO_DETECT;
        pstCtx->usInterceptSum++;
        HI_MDM_NM_PowerOffSetNodeProtectingState(pstItem->aucMac);
        return HI_ERR_FAILURE;
    }

    HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_044, HI_DIAG_MT("check score report checkscore full score"),pstItem->ucReportCheckScore,pstCtx->ucReviewScoreFull);
    // �����ֲ������֣����ϱ�; �����ϱ���������
    if (pstItem->ucReportCheckScore < pstCtx->ucReviewScoreFull)
    {
        pstItem->ucStatus = MRS_POWEROFF_INTERCEPT_BY_RECORD;
        pstCtx->usInterceptSum++;
        return HI_ERR_SUCCESS;
    }
    else
    {
        pstItem->ucStatus = MRS_POWEROFF_REPORTED;
        pstItem->ulLastReportOffTime = HI_MDM_GetRealTime();
        pstCtx->us3762ReportOffSum++;

        (hi_void)memset_s(&stEntry, sizeof(stEntry), 0, sizeof(stEntry));
        (hi_void)memcpy_s(stEntry.aucMac, HI_PLC_MAC_ADDR_LEN,pstItem->aucMac, HI_PLC_MAC_ADDR_LEN);
        stEntry.ucStateChange = HI_ND_STATE_CHANGE_ONLINE_TO_OFFLINE;
        return mrsPoweroffReportStateChange(&stEntry);
    }
}


// ִ�� ��������
HI_U32 mrsPoweroffRunRecheckProcess(MRS_NODE_STATE_CHG_REPORT_CTX * pstCtx)
{
    MRS_POWEROFF_CHECK_LIST_S * pstList = HI_NULL;
    MRS_PLC_POWEROFF_INFO_DL_S * pstFrame = HI_NULL;
    HI_U8 aucDstMac[HI_PLC_MAC_ADDR_LEN] = {0};
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U16 usIdx = 0;

    HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_017, HI_DIAG_MT("Poweroff Into New Recheck Round !"));

    if (!pstCtx || !pstCtx->pstCheckStateList)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    ulRet = mrsCcoCreatePoweroffInfoFrame(pstCtx, &pstFrame);
    if (HI_ERR_SUCCESS != ulRet)
    {
        return ulRet;
    }

    MRS_StopTimer(MRS_CCO_TIMER_POWEROFF_RECHECK);
    pstList = pstCtx->pstCheckStateList;

    for (usIdx = pstList->usRecheckIdx;
        usIdx < PRODUCT_CFG_MRS_MAX_TOPO_NUM && pstFrame->sta_num < MRS_PLC_POWEROFF_INFO_DL_MAC_NUM; usIdx++)
    {
        if (MRS_POWERON_RECHECKING != pstList->astItems[usIdx].ucReCheckStatus)
        {
            continue;
        }

        HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1004, HI_DIAG_MT("Poweroff Rechecking [MAC]"),
            pstList->astItems[usIdx].aucMac, HI_PLC_MAC_ADDR_LEN);

        // ���˳��Գ���������������ʧ��
        if (pstList->astItems[usIdx].ucRecheckQueryCnt >= pstCtx->ucPoweroffRecheckMaxTries)
        {
            pstList->astItems[usIdx].ucReCheckStatus = MRS_POWERON_RECHECKED;
            pstList->astItems[usIdx].ucRecheckFailCnt++;
            pstCtx->usRecheckFailSum++;
            mrsPoweroffSaveHistory(pstCtx, &pstList->astItems[usIdx]);

            HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_018, HI_DIAG_MT("Poweroff Recheck Fail: [ulRet]"));
            continue;
        }

        (hi_void)memcpy_s(pstFrame->mac_list + pstFrame->sta_num * HI_PLC_MAC_ADDR_LEN, HI_PLC_MAC_ADDR_LEN,pstList->astItems[usIdx].aucMac, HI_PLC_MAC_ADDR_LEN);
        pstFrame->sta_num++;
        pstList->astItems[usIdx].ucRecheckQueryCnt++;

        HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_019, HI_DIAG_MT("Poweroff Rechecking Add To Frame: [sta_num] [query_cnt]"),
            pstFrame->sta_num, pstList->astItems[usIdx].ucRecheckQueryCnt);
    }

    if (pstFrame->sta_num > 0)
    {
        pstFrame->detect_flag = HI_FALSE;
        (hi_void)memcpy_s(aucDstMac, HI_PLC_MAC_ADDR_LEN,MRS_BROADCAST_MAC, HI_PLC_MAC_ADDR_LEN);
        ulRet = mrsCcoGetPoweroffInfo(pstFrame, aucDstMac);

        if (pstList->usDetectIdx >= PRODUCT_CFG_MRS_MAX_TOPO_NUM)
        {
            pstList->usDetectIdx = 0;
        }

        MRS_StartTimer(MRS_CCO_TIMER_POWEROFF_RECHECK, MRS_SEC_TO_MS(pstCtx->ucPoweroffRecheckInterval), HI_SYS_TIMER_PERIODIC);
    }
    else
    {
        pstList->bRecheckComplete = HI_TRUE;
        ulRet = HI_ERR_SUCCESS;
    }

    mrsToolsFree(pstFrame);

    HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_020, HI_DIAG_MT("Poweroff Recheck Round Finish: [ulRet]"), ulRet);

    return ulRet;
}


// ִ�� վ���ϱ�����
HI_U32 mrsPoweroffRecheckItem(MRS_NODE_STATE_CHG_REPORT_CTX * pstCtx, MRS_PLC_POWEROFF_INFO_UP_S * pstUpFrame,
                              MRS_POWEROFF_CHECK_ITEM_S * pstItem)
{
    HI_U32 ulTimeDiff = 0;

    if (!pstCtx || !pstUpFrame || !pstItem)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    do
    {
        if (MRS_POWERON_RECHECKING != pstItem->ucReCheckStatus)
        {
            break;
        }

        HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_023, HI_DIAG_MT("[poweroff-mrs] Meter Poweroff Time: [RtcTimeSec]"),
                pstUpFrame->last_poweroff_time, pstItem->ulMeterLastPoweroffTime);
        // TODO: ���������
        // У����ͣ��ʱ���Ƿ�Ϸ�
        if (!mrsTimeCheckRtcYearIsAfterBase(pstUpFrame->last_poweroff_time, MRS_YEAR_BASE))
        {
            HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_038, HI_DIAG_MT("[poweroff-mrs] Meter Poweroff Time Invalid: [RtcTimeSec]"),
                pstUpFrame->last_poweroff_time);
            break;
        }

        //�Ƚϵ����ϴ��ϱ�ʱ��͵��ʵ��ͣ��ʱ�䣬�ϴ��ϱ�ʱ���ɵ��ʵ��ͣ���¼�ˢ��
        ulTimeDiff = mrsTimeSubDuration(pstUpFrame->last_poweroff_time, pstItem->ulMeterLastPoweroffTime);
        // MAC�󱨣����ʵ����û�з���ͣ��
        if (0 == ulTimeDiff)
        {
            HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_034, HI_DIAG_MT("[poweroff-mrs] ulTimeDiff zero"));
            mrsPoweroffHandleMACReportWrong(pstCtx, pstItem);
            break;
        }

        // У��CCO��RTCʱ���Ƿ���ȷ
        if (!mrsTimeCheckRtcYearIsAfterBase(pstItem->ulLastMacReportOffTime, MRS_YEAR_BASE))
        {
            HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_039, HI_DIAG_MT("[poweroff-mrs] RTC time err"),pstItem->ulLastMacReportOffTime,pstUpFrame->last_poweroff_time);
            break;
        }


        //�Ƚ�mac�ϱ�ʱ��͵��ͣ���¼��Ĳ�ֵ
        ulTimeDiff = mrsTimeSubDuration(pstItem->ulLastMacReportOffTime, pstUpFrame->last_poweroff_time);

        // �����ͣ��ʱ����CCO����֮ǰ����Ϊ��MAC��
        if (ulTimeDiff > HI_MDM_GetSeconds())
        {
            HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_035, HI_DIAG_MT("ulTimeDiff biger"), ulTimeDiff);
            mrsPoweroffHandleMACReportWrong(pstCtx, pstItem);
            break;
        }

        // MAC��ȷ�ϱ�: MAC�ϱ�ʱ�� - ���ͣ��ʱ�� <= ���ޣ�������20min
        if (ulTimeDiff <= pstCtx->ulReportMaxDelayTime)
        {
            HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_036, HI_DIAG_MT("mac report correct"),pstItem->ucStatus,ulTimeDiff);
            mrsPoweroffHandleMACReportCorrect(pstCtx, pstItem);
        }
        // MAC©���� ģ�鷢����ͣ���macû���ϱ�,��Ҫ���ӻ���
        // ��������ŵ�ԭ��վ�����ߣ���������ʱ�������ӻ���
        else
        {
            // �Ƿ��״��ж� last_poweroff_time �� mac�ϱ�ʱ���п��ܼ���ϴ�����ж�Ϊ��
            if(0 == pstItem->ulMeterLastPoweroffTime)
            {
                HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_045, HI_DIAG_MT("[poweroff-mrs] Meter Poweroff ulMeterLastPoweroffTime Zero"),
                    pstUpFrame->last_poweroff_time);

                mrsPoweroffHandleMACReportWrong(pstCtx, pstItem);
                break;

            }

            HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_037, HI_DIAG_MT("miss report"),pstCtx->ulReportMaxDelayTime,ulTimeDiff);
            mrsPoweroffHandleMACReportMiss(pstCtx, pstItem);
        }

    } while(0);

    pstItem->ucReCheckStatus = MRS_POWERON_RECHECKED;
    pstItem->ulMeterLastPoweroffTime = pstUpFrame->last_poweroff_time;
    pstItem->ulMeterLastPoweronTime = pstUpFrame->last_poweron_time;

    return HI_ERR_SUCCESS;
}


// ���� ��
HI_U32 mrsPoweroffHandleMACReportWrong(MRS_NODE_STATE_CHG_REPORT_CTX * pstCtx, MRS_POWEROFF_CHECK_ITEM_S * pstItem)
{
    pstItem->ulLastMacReportWrongTime = HI_MDM_GetSeconds();
    pstItem->ucMacReportWrongCnt++;
    pstCtx->usMacReportWrongSum++;

    // ̽����: δ̽��ɹ���û����ס
    // ̽��û��̽�⵽վ�㣬�����Ѿ��ϱ������ܱ����أ�վ���ʱ�Ѿ����ߣ�
    if (MRS_POWEROFF_DETECT_SUCCESS != pstItem->ucStatus)
    {
        pstCtx->usDetectReportWrongSum++;
        pstItem->ucDetectReportWrongCnt++;
        pstItem->ucReportCheckScore -= MRS_MIN(pstItem->ucReportCheckScore, pstCtx->ucWrongReportReduceScore);
        HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_046, HI_DIAG_MT("wrong report, check score=%d"),pstItem->ucReportCheckScore);
    }

    // 3762��: ���һ��Ҳû��ס���ϱ����˼�����
    if (MRS_POWEROFF_REPORTED == pstItem->ucStatus)
    {
        pstItem->uc3762ReportWrongCnt++;
        pstCtx->us3762ReportWrongSum++;
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_047, HI_DIAG_MT("wrong report"));
    }

    return HI_ERR_SUCCESS;
}


// ���� ��ȷ�ϱ�
HI_U32 mrsPoweroffHandleMACReportCorrect(MRS_NODE_STATE_CHG_REPORT_CTX * pstCtx, MRS_POWEROFF_CHECK_ITEM_S * pstItem)
{
    pstItem->ucMacReportCorrectCnt++;
    pstCtx->usMacReportCorrectSum++;

    // ̽����ȷ�ϱ�: ̽�ⲻ�ɹ�
    if (MRS_POWEROFF_DETECT_SUCCESS != pstItem->ucStatus)
    {
        pstCtx->usDetectReportCorrectSum++;
        pstItem->ucDetectReportCorrectCnt++;
        pstItem->ucReportCheckScore = MRS_MIN(pstCtx->ucReviewScoreFull,
                                        pstItem->ucReportCheckScore + pstCtx->ucCorrectReportPlusScore);

        // 3762��ȷ�ϱ�: ������δ���أ��ϱ����˼�����
        if (MRS_POWEROFF_REPORTED == pstItem->ucStatus)
        {
            pstCtx->us3762ReportCorrectSum++;
            pstItem->uc3762ReportCorrectCnt++;
        }
        // ����: ©���������� ���ڱ�����û���ϱ���վ����Ϣ����Ҫ����վ����֣���֤վ���ܹ��ϱ�
        else
        {
            pstCtx->us3762ReportMissSum++;
            pstItem->uc3762ReportMissCnt++;
            pstItem->ucReportCheckScore = MRS_MIN(pstCtx->ucReviewScoreFull,
                                            pstItem->ucReportCheckScore + pstCtx->ucMissReportPlusScore);
        }
    }
    // ����: ̽��©����˵����վ����Ӧ̽��֡ վ���Ѿ����ߣ�˵��վ��������̽�⵽��Ӧ֡��û���ϱ���������
    else
    {
        pstCtx->usDetectReportMissSum++;
        pstItem->ucDetectReportMissCnt++;
        pstItem->ucReportCheckScore = MRS_MIN(pstCtx->ucReviewScoreFull,
                                        pstItem->ucReportCheckScore + pstCtx->ucMissReportPlusScore);
    }

    return HI_ERR_SUCCESS;
}


// ���� ©��
HI_U32 mrsPoweroffHandleMACReportMiss(MRS_NODE_STATE_CHG_REPORT_CTX * pstCtx, MRS_POWEROFF_CHECK_ITEM_S * pstItem)
{
    pstCtx->usMacReportMissSum++;
    pstItem->ucMacReportMissCnt++;
    pstItem->ucReportCheckScore = MRS_MIN(pstCtx->ucReviewScoreFull,
                                    pstItem->ucReportCheckScore + pstCtx->ucMissReportPlusScore);

    return HI_ERR_SUCCESS;
}


// ���� ��ʷ��¼
HI_U32 mrsPoweroffSaveHistory(MRS_NODE_STATE_CHG_REPORT_CTX * pstCtx, MRS_POWEROFF_CHECK_ITEM_S * pstItem)
{
    MRS_POWEROFF_HISTORY_ITEM_S * pstRecord = HI_NULL;

    if (!pstCtx || !pstCtx->pstHistoryList || !pstItem)
    {
        return  HI_ERR_INVALID_PARAMETER;
    }

    pstRecord = &pstCtx->pstHistoryList->astRecords[pstCtx->pstHistoryList->ucIndex];
    (hi_void)memcpy_s(pstRecord->aucMac, HI_PLC_MAC_ADDR_LEN,pstItem->aucMac, HI_PLC_MAC_ADDR_LEN);
    pstRecord->ucStatus                 = pstItem->ucStatus;
    pstRecord->ulMacReportOffTime       = pstItem->ulLastMacReportOffTime;
    pstRecord->ulMacReportOnTime        = pstItem->ulLastMacReportOnTime;
    pstRecord->ulMeterLastPoweroffTime  = pstItem->ulMeterLastPoweroffTime;
    pstRecord->ulMeterLastPoweronTime   = pstItem->ulMeterLastPoweronTime;

    pstCtx->pstHistoryList->ucIndex++;
    pstCtx->pstHistoryList->ucIndex %= MRS_POWEROFF_CHECK_HISTORY_NUM;

    return HI_ERR_SUCCESS;
}


// ��� �����ڵ���ȶ���
HI_U32 mrsPoweroffStableCheck(MRS_NODE_STATE_CHG_REPORT_CTX * pstCtx)
{
    HI_U32 ulIdx = 0;
    MRS_POWEROFF_CHECK_ITEM_S * pstItems = HI_NULL;

    if (!pstCtx || !pstCtx->pstCheckStateList)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    pstItems = pstCtx->pstCheckStateList->astItems;

    for (ulIdx = 0; ulIdx < PRODUCT_CFG_MRS_MAX_TOPO_NUM; ulIdx++)
    {
        if (MRS_POWEROFF_DEFAULT != pstItems[ulIdx].ucStatus && !pstItems[ulIdx].bPassedDetect)
        {
            pstItems[ulIdx].ucReportCheckScore = HI_MIN(pstCtx->ucReviewScoreFull,
                                                    pstItems[ulIdx].ucReportCheckScore + pstCtx->ucNoPassDetectPlusScore);
        }

        pstItems[ulIdx].bPassedDetect = HI_FALSE;
    }

    if (pstCtx->ulNoPassDetectPeriod > 0)
    {
        MRS_StartTimer(MRS_CCO_TIMER_POWEROFF_STABLE_CHECK, MRS_SEC_TO_MS(pstCtx->ulNoPassDetectPeriod), HI_SYS_TIMER_ONESHOT);
    }

    return HI_ERR_SUCCESS;
}


// ���� һ����ѯͣ����Ϣ����(������mac_list)
HI_U32 mrsCcoCreatePoweroffInfoFrame(MRS_NODE_STATE_CHG_REPORT_CTX * pstCtx, MRS_PLC_POWEROFF_INFO_DL_S ** pstFrame)
{
    HI_U32 ulFrameLen = sizeof(MRS_PLC_POWEROFF_INFO_DL_S) + HI_PLC_MAC_ADDR_LEN * MRS_PLC_POWEROFF_INFO_DL_MAC_NUM;

    if (!pstCtx || !pstFrame)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    *pstFrame = mrsToolsMalloc(ulFrameLen);
    if (!(*pstFrame))
    {
        return HI_ERR_MALLOC_FAILUE;
    }

    (hi_void)memset_s(*pstFrame, ulFrameLen, 0, ulFrameLen);
    (*pstFrame)->interface_ver = MRS_CMD_STRU_VER_R0;
    (*pstFrame)->stru_len      = sizeof(MRS_PLC_POWEROFF_INFO_DL_S);
    (*pstFrame)->seq           = pstCtx->ulGetPoweroffInfoSeq;

    return HI_ERR_SUCCESS;
}


// ���� ��ѯͣ����Ϣ����
HI_U32 mrsCcoGetPoweroffInfo(MRS_PLC_POWEROFF_INFO_DL_S * pstFrame, HI_U8 aucDstMac[HI_PLC_MAC_ADDR_LEN])
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    MRS_PLC_FRAME_DATA_STRU plc = {0};

    if (!pstFrame || !aucDstMac || 0 == pstFrame->sta_num)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    (hi_void)memset_s(&plc, sizeof(MRS_PLC_FRAME_DATA_STRU), 0,sizeof(MRS_PLC_FRAME_DATA_STRU));
    plc.usId = ID_MRS_CMD_GET_POWEROFF_INFO;
    plc.pucPayload = (HI_U8 *)pstFrame;
    plc.usPayloadLen = sizeof(MRS_PLC_POWEROFF_INFO_DL_S) + pstFrame->sta_num * HI_PLC_MAC_ADDR_LEN;
    plc.is_bc_frame = HI_TRUE;
    (hi_void)memcpy_s(plc.ucMac, HI_PLC_MAC_ADDR_LEN,aucDstMac, HI_PLC_MAC_ADDR_LEN);

    ulRet = MRS_SendPlcFrame(&plc);

    return ulRet;
}


// ���� STA���͸�CCO�Ĳ�ѯͣ����Ϣ����
HI_U32 mrsCmdCcoGetPoweroffInfo(HI_U16 usId, HI_PVOID req_packet, HI_U16 packet_size)
{
    MRS_PLC_FRAME_DATA_S * plc = (MRS_PLC_FRAME_DATA_S *)req_packet;
    MRS_PLC_POWEROFF_INFO_UP_S * pstUpFrame = HI_NULL;
    MRS_NODE_STATE_CHG_REPORT_CTX* pstCtx = mrsGetChgReportCtx();
    MRS_POWEROFF_CHECK_ITEM_S * pstItem = HI_NULL;

    HI_UNREF_PARAM(packet_size);
	HI_UNREF_PARAM(usId);

    if (!plc || !plc->pucPayload)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1005, HI_DIAG_MT("[power] rcv cmd: [sta_mac]"), plc->ucMac, HI_PLC_MAC_ADDR_LEN);

    if (!pstCtx || !pstCtx->pstCheckStateList)
    {
        return HI_ERR_SUCCESS;
    }

    // У�鱨�����к�
    pstUpFrame = (MRS_PLC_POWEROFF_INFO_UP_S *)plc->pucPayload;
    if (pstUpFrame->seq != pstCtx->ulGetPoweroffInfoSeq)
    {
        HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_021, HI_DIAG_MT("[power] seq check fail: [cmd_seq] [my_seq]"),
            pstUpFrame->seq, pstCtx->ulGetPoweroffInfoSeq);
        return HI_ERR_FAILURE;
    }

    pstItem = mrsGetPoweroffCheckItem(pstCtx, plc->ucMac);
    if (!pstItem)
    {
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_022, HI_DIAG_MT("[power] item get fail"));
        return HI_ERR_SUCCESS;
    }

    // ��վ���ڸ����У����и���
    if (MRS_POWERON_RECHECKING == pstItem->ucReCheckStatus)
    {
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_032, HI_DIAG_MT("[power] rechecking"));
        mrsPoweroffRecheckItem(pstCtx, pstUpFrame, pstItem);
        mrsPoweroffSaveHistory(pstCtx, pstItem);
    }

    // ��վ����̽���У���Ϊ̽��ɹ�
    if (MRS_POWEROFF_DETECTING == pstItem->ucStatus)
    {
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_033, HI_DIAG_MT("[power] detecting"));
        pstItem->ucStatus = MRS_POWEROFF_DETECT_SUCCESS;
        pstCtx->usDetectSuccessSum++;
    }

    return HI_ERR_SUCCESS;
}


// �ϱ� ״̬�仯��Ϣ��������
HI_U32 mrsPoweroffReportStateChange(HI_MDM_NETWORK_TOPOCHG_ENTRY_S* pstEntry)
{
    MRS_NODE_STATE_CHG_REPORT_CTX* pstCtx = mrsGetChgReportCtx();
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U8* pucFrame = HI_NULL;
    HI_U16 usFrameLen = 0;
    HI_U8  ucEntryNum = 0;

    if (HI_NULL == pstEntry || HI_NULL == pstCtx)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_040, HI_DIAG_MT("mrsPoweroffReportStateChange: Mac[5]=%d, OfflineReason=%d"),pstEntry->aucMac[5],pstEntry->ucOfflineReason);

    do
    {
        ulRet = mrsSaveChgEntry(pstEntry);
        if (HI_ERR_SUCCESS != ulRet)
        {
            return ulRet;
        }

        if (pstCtx->stCache.ucNum >= pstCtx->ucCacheNumMax)
        {
            ulRet = mrsAssembleAllCacheEntries(&pucFrame, &usFrameLen, &ucEntryNum);
            if (HI_ERR_SUCCESS != ulRet)
            {
                break;
            }

            MRS_StopTimer(MRS_CCO_TIMER_NODE_STATECHG_ENTRY_CACHE);

            ulRet = mrsChgFrameReport(pucFrame, usFrameLen, ucEntryNum);
            if (HI_ERR_SUCCESS != ulRet)
            {
                break;
            }

            HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_041, HI_DIAG_MT("mrsPoweroffReportStateChange: report"));
        }
        else
        {
            //��ʱʱ����5s
            MRS_StartTimer(MRS_CCO_TIMER_NODE_STATECHG_ENTRY_CACHE,
                           pstCtx->usCacheTimeout * MRS_TIME_100_MILLI_SEC,
                           HI_SYS_TIMER_ONESHOT);
            HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_003, HI_DIAG_MT("mrsPoweroffReportStateChange: start timer"));
        }
    }while (0);

    return ulRet;
}


// ��ȡ ̽����վ�����
HI_U16 mrsPoweroffGetDetectingNum(MRS_NODE_STATE_CHG_REPORT_CTX * pstCtx)
{
    HI_U16 usDetectingNum = 0;
    HI_U32 ulIdx = 0;

    if (!pstCtx || !pstCtx->pstCheckStateList)
    {
        return 0;
    }

    for (ulIdx = 0; ulIdx < PRODUCT_CFG_MRS_MAX_TOPO_NUM; ulIdx++)
    {
        if (MRS_POWEROFF_DETECTING == pstCtx->pstCheckStateList->astItems[ulIdx].ucStatus)
        {
            usDetectingNum++;
        }
    }

    return usDetectingNum;
}


// ��ȡ ������վ�����
HI_U16 mrsPoweroffGetRecheckingNum(MRS_NODE_STATE_CHG_REPORT_CTX * pstCtx)
{
    HI_U16 usRecheckingNum = 0;
    HI_U32 ulIdx = 0;

    if (!pstCtx || !pstCtx->pstCheckStateList)
    {
        return 0;
    }

    for (ulIdx = 0; ulIdx < PRODUCT_CFG_MRS_MAX_TOPO_NUM; ulIdx++)
    {
        if (MRS_POWERON_RECHECKING == pstCtx->pstCheckStateList->astItems[ulIdx].ucReCheckStatus)
        {
            usRecheckingNum++;
        }
    }

    return usRecheckingNum;
}

#endif
#endif


