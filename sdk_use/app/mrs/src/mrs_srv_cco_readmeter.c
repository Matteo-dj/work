//*****************************************************************************

//
//                  ��Ȩ���� (C), 1998-2011, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mrs_srv_cco_readmeter.c
//  �� �� ��   : V1.0
//  ��    ��   : liuxipeng/KF54842
//  ��������   : 2011-12-13
//  ��������   : ����ģ�麯��ʵ��
//
//  �����б�   :
//  �޸���ʷ   :
//  1.��    �� : 2011-12-13
//    ��    �� : liuxipeng/KF54842
//    �޸����� : �����ļ�
//
//*****************************************************************************
#include "mrs_common.h"
#include "mrs_cmd_msg.h"
#include "mrs_fw_proto645.h"
#include "mrs_fw_proto376_2.h"
#include "mrs_fw_proto376_2_echo.h"
#include "mrs_fw_proto376_2_frame.h"
#include "mrs_fw_tools.h"
#include "mrs_fw_log.h"
#include "mrs_srv_common.h"
#include "mrs_srv_cco_macmap.h"
#include "mrs_srv_parallel_cco.h"
#include "mrs_srv_list.h"
#include "mrs_srv_cco_event.h"
#include "mrs_srv_cco_remote_upg.h"
#include "mrs_srv_cco.h"
#include "mrs_srv_cco_readmeter.h"
#include "mrs_fw_n.h"
#include "mrs_dfx.h"
#include "mrs_srv_cco_searchmeter.h"
#include "mrs_srv_mrslog.h"
#include "mrs_srv_cco_queue.h"
#include "mrs_srv_cco_archives.h"
#include "mrs_srv_cco_testmode.h"
#include "mrs_srv_cco_detect_rm.h"

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)

HI_VOID mrsSetReadMeterState(MRS_MRV_CCO_READMETER_STATE_ENUM state)
{
    MRS_CCO_SRV_CTX_STRU *cco_ctx = mrsCcoGetContext();

    cco_ctx->stReadMeterModel.enCurrState = state;
    mrsDfxLrState(state);
}

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO_BROAD_BAND_UNIT)
HI_VOID mrsInitMrStateBbu(HI_VOID)
{
    HI_U32 status = mrsCcoGetState();

    status &= ~MRS_CCO_STATUS_NORMAL;
    mrsCcoSetState(status);
}
#endif

//*****************************************************************************
// ��������: mrsReadMeterFirstRunProcess
// ��������: ����ģ���һ�����д���(�ع�)
//
// ����˵��:
//   pstCcoCtx [in/out] ��ǰ״̬
//
// �� �� ֵ:
//    HI_ERR_SUCCESS �ɹ�
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: liuxipeng/KF54842 [2011-12-22]
//*****************************************************************************
HI_U32 mrsReadMeterFirstRunProcess(MRS_CCO_SRV_CTX_STRU* pstCcoCtx)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U16 i = 0; //����ӵı�ʶ
    HI_U16 usMeterNum = mrsSrvArchivesMeterNum();

    if (pstCcoCtx == HI_NULL)
    {
        ret = HI_ERR_FAILURE;
        return ret;
    }

    pstCcoCtx->stReadMeterModel.enMeterInfState = EN_MRS_SRV_CCO_METERINF_STATE_NORMAL;

    mrsReadMeterStateInit();  // ����ģ��״̬��ʼ��

    if (usMeterNum == 0)
    {        
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_001, HI_DIAG_MT("no meter"));
        return HI_ERR_NOT_FOUND;   // �����û�е��
    }

    //�ڴ˴��ҵ���ǰ�����Ϣ�е�һ����Ч�ı���Ϣ����ţ�
    for (i = 0; i < PRODUCT_CFG_MRS_MAX_NODE_NUM; i++)
    {
        MRS_ARCHIVES_NODE_STRU *pstNode = HI_NULL;

        pstNode = mrsSrvArchivesQuery(i);
        if (pstNode == HI_NULL)
        {
            break;
        }

        if (pstNode->bValid && !pstNode->ucRead) // ���ҵ�һ����Ч�ı���Ϣ
        {
            pstCcoCtx->stReadMeterModel.usCurrNodeSeq = i;
            (hi_void)memcpy_s(&pstCcoCtx->stReadMeterModel.stCurrNode, sizeof(pstCcoCtx->stReadMeterModel.stCurrNode),
                pstNode, sizeof(MRS_ARCHIVES_NODE_STRU));
            pstCcoCtx->stReadMeterModel.ulTimeOutCount = 0;   // �ҵ��½ڵ��ʼ����ʱ����
            break;
        }
    }

    return HI_ERR_SUCCESS;
}


//*****************************************************************************
// ��������: mrsReadterModelCco
// ��������: ����ģ�鴦����ں���
//
// ����˵��:
//  This function has no arguments.
//
// �� �� ֵ:
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: liuxipeng/KF54842 [2011-12-15]
//*****************************************************************************
HI_VOID mrsReadterModelCco(HI_VOID * pAckParam)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    MRS_CCO_SRV_CTX_STRU* pstCcoCtx = mrsCcoGetContext(); // ����ָ��ȫ�������Ľṹ��ָ��
    MRS_ARCHIVES_NODE_STRU *pstNode = HI_NULL;
    HI_BOOL bLoopFlg = HI_TRUE;  // ѭ��
    HI_U16 usMeterNum = mrsSrvArchivesMeterNum();

    MRS_NOT_USED(pAckParam);

    MRS_StopTimer(EN_MRS_SRV_CCO_READMETER_ACTIV_TIMER);    // ������ִ��(�����ʱ��ֹͣ)
#if 0//optimized by weichao
    if (HI_FALSE == mrsCcoClockStatusAllow())
    {
        return;
    }
#endif 
    if (pstCcoCtx->stReadMeterModel.enMeterInfState == EN_MRS_SRV_CCO_METERINF_STATE_FIRST)   // �״��ϵ�
    {
        ret = mrsReadMeterFirstRunProcess(pstCcoCtx);
        if (ret != HI_ERR_SUCCESS)
        {
            // ��ʱMRS_SRV_CCO_TIMER_READMETER_ACTIV_VALUEʱ������¼������ģ��
            MRS_StartTimer(EN_MRS_SRV_CCO_READMETER_ACTIV_TIMER, MRS_SRV_CCO_TIMER_READMETER_ACTIV_VALUE,
                           HI_SYS_TIMER_ONESHOT);

            return;
        }
    }

    if (0 == usMeterNum)   //û�е��
    {
        (hi_void)memset_s(&pstCcoCtx->stReadMeterModel.stCurrNode, sizeof(MRS_ARCHIVES_NODE_STRU), 0, sizeof(MRS_ARCHIVES_NODE_STRU));
        pstCcoCtx->stReadMeterModel.usCurrNodeSeq = 0;  //(��Ϊ����)
        mrsReadMeterStateInit();

        // ��ʱMRS_SRV_CCO_TIMER_READMETER_ACTIV_VALUEʱ������¼������ģ��
        MRS_StartTimer(EN_MRS_SRV_CCO_READMETER_ACTIV_TIMER, MRS_SRV_CCO_TIMER_READMETER_ACTIV_VALUE * 25,
                       HI_SYS_TIMER_ONESHOT);

        return;
    }

    if (0 == (pstCcoCtx->status & MRS_CCO_STATUS_NORMAL)) // ��������ֳ�״̬
    {
        return;   // ���ֳ�״̬(��ִ�г�������)
    }

    pstNode = mrsSrvArchivesQuery(pstCcoCtx->stReadMeterModel.usCurrNodeSeq);
    if (pstNode == HI_NULL || pstNode->ucRead)
    {
        pstCcoCtx->stReadMeterModel.ucCurrNodeInquireTries = 0;
        pstCcoCtx->stReadMeterModel.usCurrNodeSeq++;
        ret = mrsReadMeterFindValidNode(pstCcoCtx);
        mrsReadMeterStateInit();  // ����ģ��״̬��ʼ��

        if (HI_ERR_SUCCESS != ret)
        {
            return;
        }
    }

    if (!mrsCurrNodeValid())
    {
        ret = mrsReadMeterFindValidNode(pstCcoCtx);

        mrsReadMeterStateInit();  // ����ģ��״̬��ʼ��
        
        if (HI_ERR_SUCCESS != ret)
        {
            return;
        }
    }

    while (bLoopFlg)
    {
        bLoopFlg = mrsReadMeterStateProcess(pstCcoCtx);     // ����״̬����
        HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_022, HI_DIAG_MT("bLoopFlg-%d"),bLoopFlg);
    }

    return;
}

//*****************************************************************************
// ��������: mrsWaitAskReplyProcess
// ��������: �ȴ�ѯ��Ӧ����(�ع�)
//
// ����˵��:
//   pstCcoCtx [in/out] ��ǰ״̬
//   bLoopFlg [out] �Ƿ�ѭ����־
//
// �� �� ֵ:
//    ��
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: liuxipeng/KF54842 [2011-12-22]
//*****************************************************************************
HI_VOID mrsWaitAskReplyProcess(MRS_CCO_SRV_CTX_STRU* pstCcoCtx, HI_BOOL *bLoopFlg)
{
    if (pstCcoCtx->stReadMeterModel.pstAfn14F1 != HI_NULL)
    {
        pstCcoCtx->stReadMeterModel.ucCurrNodeInquireTries = 0;
        mrsSetReadMeterState(EN_MRS_SRV_CCO_READMETER_SEND_PLC);    // ��Ϊ���Ͷ�������
        MRS_StopTimer(EN_MRS_SRV_CCO_MUSTER_TIMER);     // �����յ�����-���뼯����ͨѶ��ʱ��ֹͣ
    }
    else
    {
        *bLoopFlg = HI_FALSE;      // ��Ҫ�ȴ�
    }

    return;
}

//*****************************************************************************
// ��������: mrsWaitPlcReplyProcess
// ��������: �ȴ�PLCӦ����(�ع�)
//
// ����˵��:
//   pstCcoCtx [in/out] ��ǰ״̬
//   bLoopFlg [out] �Ƿ�ѭ����־
//
// �� �� ֵ:
//    ��
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: liuxipeng/KF54842 [2011-12-22]
//*****************************************************************************
HI_VOID mrsWaitPlcReplyProcess(MRS_CCO_SRV_CTX_STRU* pstCcoCtx, HI_BOOL *bLoopFlg)
{
    if (pstCcoCtx->stReadMeterModel.pstUpReadMeterData != HI_NULL)
    {
        if (pstCcoCtx->stReadMeterModel.pstAfn14F1 != HI_NULL)
        {
            pstCcoCtx->stReadMeterModel.pstAfn14F1->MRS_AFN14_FREE_FUN(pstCcoCtx->stReadMeterModel.pstAfn14F1);
            pstCcoCtx->stReadMeterModel.pstAfn14F1 = HI_NULL;
        }

        pstCcoCtx->stReadMeterModel.ulTimeOutCount = 0;   // �յ��µ�PLC���ݳ�ʼ����ʱ����

        mrsSetReadMeterState(EN_MRS_SRV_CCO_READMETER_UPDATA);     // ��Ϊ�ϴ���������
        MRS_StopTimer(EN_MRS_SRV_CCO_STA_TIMER);     // �����յ�����-����STAͨѶ��ʱ��ֹͣ
    }
    else
    {
        *bLoopFlg = HI_FALSE;      // ��Ҫ�ȴ�   2015-08-18:���ִ�в���
    }

    if(pstCcoCtx->stReadMeterModel.pstPlcDataDl != HI_NULL)
    {
        if(pstCcoCtx->stReadMeterModel.pstPlcDataDl->pucPayload != HI_NULL)
        {
            mrsToolsFree(pstCcoCtx->stReadMeterModel.pstPlcDataDl->pucPayload);   // �ͷ�ת�����ݿ�   
            pstCcoCtx->stReadMeterModel.pstPlcDataDl->pucPayload = HI_NULL;
        }
        
        mrsToolsFree(pstCcoCtx->stReadMeterModel.pstPlcDataDl);   // �ͷ�PLC֡����    
        pstCcoCtx->stReadMeterModel.pstPlcDataDl = HI_NULL;
    }
    
    return;
}

//*****************************************************************************
// ��������: mrsWaitUpDataAckProcess
// ��������: �ȴ������ϴ�Ӧ����(�ع�)
//
// ����˵��:
//   pstCcoCtx [in/out] ��ǰ״̬
//   bLoopFlg [out] �Ƿ�ѭ����־
//
// �� �� ֵ:
//    ��
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: liuxipeng/KF54842 [2011-12-22]
//*****************************************************************************
HI_VOID mrsWaitUpDataAckProcess(MRS_CCO_SRV_CTX_STRU* pstCcoCtx, HI_BOOL *bLoopFlg)
{
    PLC_OVERTIME_INF_STRU *pstAfn14 = mrsGetPlcOverTimeInfAfn14();
	
	HI_UNREF_PARAM(pstCcoCtx);
	
    mrsReadMeterStateInit();  // ����ģ��״̬��ʼ��(���¿�ʼѯ��)

    // �������ģ��
    MRS_StartTimer(EN_MRS_SRV_CCO_READMETER_ACTIV_TIMER, 
                        (HI_U32)pstAfn14->MeterFrameInterval,
                        HI_SYS_TIMER_ONESHOT);
    mrsDfxLrOkCnt();    // ͳ���ֳ������ɹ�����

    *bLoopFlg = HI_FALSE;      // ��Ҫ�ȴ�

    return;
}

//*****************************************************************************
// ��������: mrsReadMeterStateProcess
// ��������: ����״̬����(�ع�)
//
// ����˵��:
//   pstCcoCtx [IN/OUT] ����״̬��Ϣ
//
// �� �� ֵ:
//    HI_TRUE ����ִ��
//    HI_FALSE �ȴ�״̬
//
// ����Ҫ��:
// ���þ���:
// ��    ��: liuxipeng/KF54842 [2011-12-20]
//*****************************************************************************
HI_BOOL mrsReadMeterStateProcess(HI_INOUT MRS_CCO_SRV_CTX_STRU* pstCcoCtx)
{
    HI_BOOL bLoopFlg = HI_TRUE;  // ѭ����־(HI_FALSE:��Ҫ�ȴ� HI_TRUE:����ִ��)

    switch (pstCcoCtx->stReadMeterModel.enCurrState)
    {
    case EN_MRS_SRV_CCO_READMETER_IDLE:              // ����
        if(mrsGetNeedGetTimeFlg())
        {
            mrsSetNeedGetTimeFlg(HI_FALSE);

            MRS_StartTimer(EN_MRS_SRV_CCO_READMETER_ACTIV_TIMER, MRS_SRV_CCO_TIMER_READMETER_ACTIV_VALUE * 5,
               HI_SYS_TIMER_ONESHOT);

            return HI_FALSE;
        }
        else
        {
            mrsSetReadMeterState(EN_MRS_SRV_CCO_READMETER_ASK);     // ��Ϊѯ��״̬
        }
        break;

    case EN_MRS_SRV_CCO_READMETER_ASK:               // ѯ��
        // ��ȡCCO����״̬(HI_TRUE : �Ѿ������Զ���; HI_FALSE : δ����)
        if (mrsIsCcoUpgStatus())
        {
            // ��ǰCCOδ����-������ѯ�ʶ���
            mrsSetReadMeterState(EN_MRS_SRV_CCO_READMETER_IDLE);   // ����
            MRS_StartTimer(EN_MRS_SRV_CCO_READMETER_ACTIV_TIMER, 
                                MRS_SRV_CCO_TIMER_READMETER_REACTIV_VALUE,
                                HI_SYS_TIMER_ONESHOT);
        }
        else
        {
            mrsInquireReadMeter();  // ����ѯ�ʶ�������(����������)
        }
        bLoopFlg = HI_FALSE;      // ��Ҫ�ȴ�
        break;

    case EN_MRS_SRV_CCO_READMETER_WAIT_ASK_REPLY:    // �ȴ�ѯ�ʵ�Ӧ��
        mrsWaitAskReplyProcess(pstCcoCtx, &bLoopFlg);
        break;

    case EN_MRS_SRV_CCO_READMETER_WAIT_REVISE_RESP:    // �ȴ���ʱ����Ӧ��
        mrsWaitAskReplyProcess(pstCcoCtx, &bLoopFlg);
        break;
        
    case EN_MRS_SRV_CCO_READMETER_SEND_PLC:          // ���Ͷ�������
        mrsAssembleReadMeterData(); //��֯14-01����֡
        mrsTransmit14F1FramePlc(); //ת�����ݵ�STA
        bLoopFlg = HI_FALSE;      // ��Ҫ�ȴ�
        break;

    case EN_MRS_SRV_CCO_READMETER_WAIT_PLC_REPLY:    // �ȴ���������Ӧ��
        HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_032, HI_DIAG_MT("bLoopFlg-%d"),bLoopFlg);
        mrsWaitPlcReplyProcess(pstCcoCtx, &bLoopFlg);
        break;

    case EN_MRS_SRV_CCO_READMETER_REPEAT_PLC:        // �ط���������
        mrsDfxLrPlcTryCnt();    // ͳ���ط�����
        mrsTransmit14F1FramePlc();     // ת�����ݵ�STA
        bLoopFlg = HI_FALSE;      // ��Ҫ�ȴ�
        break;

    case EN_MRS_SRV_CCO_READMETER_UPDATA:            // �ϴ���������(�ϴ���������)
        mrsUpReterMetreData();  // �ϴ���������
        bLoopFlg = HI_FALSE;      // ��Ҫ�ȴ�
        break;

    case EN_MRS_SRV_CCO_READMETER_WAIT_UPDATA_ACK:   // �ȴ��ϴ���������ȷ��
        mrsWaitUpDataAckProcess(pstCcoCtx, &bLoopFlg);
        break;

    case EN_MRS_SRV_CCO_READMETER_REPEAT_UPDATA:     // �ط��ϴ���������
        {
            P_MRS_SRV_CCO_QUEUE pstCcoQueue = mrsGetCcoQueue();
            MRS_SRV_CCO_ITEM *pstCcoItem = HI_NULL;
    
            pstCcoItem = mrsSrvQueueTop(&pstCcoQueue->stCcoQueue);
            if (pstCcoItem)
            {
                mrsDfxRmUartTx(pstCcoItem->pData, pstCcoItem->usDataLen, HI_TRUE);
            }
        }
    
        mrsDfxLrUpDataTryCnt();
        mrsDfxLrUpDataCnt();
    /* BEGIN: Modified by fengxiaomin/00209182, 2014/3/27   ���ⵥ��:DTS2014032906596 */
    /* END:   Modified by fengxiaomin/00209182, 2014/3/27 */
        mrsSetReadMeterState(EN_MRS_SRV_CCO_READMETER_WAIT_UPDATA_ACK);   // �ȴ�������ȷ��
        //mrsUpReterMetreData();  // �ϴ���������
        bLoopFlg = HI_FALSE;      // ��Ҫ�ȴ�
        break;

    default:
        mrsReadMeterStateInit();  // ����ģ��״̬��ʼ��(���¿�ʼѯ��)

        // ��ʱMRS_SRV_CCO_TIMER_READMETER_ACTIV_VALUEʱ������¼������ģ��
        MRS_StartTimer(EN_MRS_SRV_CCO_READMETER_ACTIV_TIMER,
                            1000 * 60, HI_SYS_TIMER_ONESHOT);

        bLoopFlg = HI_FALSE;      // ��Ҫ�ȴ�
        break;
    }

    return bLoopFlg;
}

//*****************************************************************************
// ��������: mrsInquireReadMeter
// ��������: ѯ���Ƿ񳭱�
//
// ����˵��:
//  This function has no arguments.
//
// �� �� ֵ:
//    ��
// ����Ҫ��:
// ���þ���:
// ��    ��: liuxipeng/KF54842 [2011-12-16]
// ��    ��: cuiate/00233580   [2013-04-01]
//*****************************************************************************
HI_VOID mrsInquireReadMeter(HI_VOID)
{
    HI_U32 ret = HI_ERR_SUCCESS;

    MRS_CCO_SRV_CTX_STRU* pstCcoCtx = mrsCcoGetContext(); // ����ָ��ȫ�������Ľṹ��ָ��
    MRS_ARCHIVES_NODE_STRU *pstNode = HI_NULL;
    HI_U16 usLen = 0;
    HI_PBYTE pucOutBuf = HI_NULL;
    HI_U16 usCurrSeq = 0;

    if (pstCcoCtx->stReadMeterModel.stCurrNode.bValid == HI_FALSE)
    {
        // ��ʱMRS_SRV_CCO_TIMER_READMETER_ACTIV_VALUEʱ������¼������ģ��
        MRS_StartTimer(EN_MRS_SRV_CCO_READMETER_ACTIV_TIMER, 
                            MRS_SRV_CCO_TIMER_READMETER_ACTIV_VALUE, HI_SYS_TIMER_ONESHOT);
        return;
    } 

    pstNode = mrsSrvArchivesQuery(pstCcoCtx->stReadMeterModel.usCurrNodeSeq);
    if (pstNode == HI_NULL || pstNode->ucRead)
    {
        // �������ģ��
        mrsAcivReadMeter();

        return;
    } 
    
    // ��֯AFN14-F1����֡
    do
    {
        PROTO376_2_FRAME_AFN_AND_DATA_STRU stAfnData;
        HI_U8 ucBuf[9] = { 0 };

        ucBuf[usLen++] = 0x00; // δ֪��λ
        (hi_void)memcpy_s(&ucBuf[usLen], sizeof(ucBuf) - usLen, pstCcoCtx->stReadMeterModel.stCurrNode.aucNodeAddr, HI_METER_ADDR_LEN);
        usLen += HI_METER_ADDR_LEN;
        usCurrSeq = pstCcoCtx->stReadMeterModel.usCurrNodeSeq;
        if (MRS_CCO_SEQ_START_FROM_1 == pstCcoCtx->ucSeqControl)
        {
            usCurrSeq++;
        }
        
        (hi_void)memcpy_s(&ucBuf[usLen], sizeof(ucBuf) - usLen, &usCurrSeq, sizeof(HI_U16));
        usLen += 2;

        (hi_void)memset_s(&stAfnData, sizeof(stAfnData), 0, sizeof(stAfnData));
        stAfnData.ucAFN = MRS_AFN(0x14);
        stAfnData.ucDT[0] = MRS_AFN_FN_LO(1);
        stAfnData.ucDT[1] = MRS_AFN_FN_HI(1);
        stAfnData.pData = ucBuf;
        stAfnData.usDataLen = usLen;

        usLen = 0;
        ret = mrsCreate3762UpFrame(&stAfnData, &pucOutBuf, &usLen, pstCcoCtx->stReadMeterModel.stCurrNode.aucNodeAddr, HI_NULL);
        if (ret != HI_ERR_SUCCESS)
        {
            break;
        }

        ret = MRS_SendMrData(pucOutBuf, usLen, HI_DMS_CHL_UART_PORT_APP); // ����ѯ�ʶ���֡
    } while (0);

    mrsToolsFree(pucOutBuf);

    if (ret == HI_ERR_SUCCESS)
    {
        pstCcoCtx->stReadMeterModel.ucCurrNodeInquireTries++;
        MRS_StartTimer(EN_MRS_SRV_CCO_MUSTER_TIMER, 
                            mrsToolsGetTimeOut(pstCcoCtx->stReadMeterModel.usAfn14F1UpTimeOut), 
                            HI_SYS_TIMER_ONESHOT); // ������ʱ��
        mrsSetReadMeterState(EN_MRS_SRV_CCO_READMETER_WAIT_ASK_REPLY);    // ��Ϊ�ȴ�ѯ��Ӧ������
        mrsDfxLrReqCnt();
    }
    else
    {
        mrsReadMeterStateInit();  // ����ģ�������

        MRS_StartTimer(EN_MRS_SRV_CCO_READMETER_ACTIV_TIMER, 
                            1000 * 60, HI_SYS_TIMER_ONESHOT);
    }

    return;
}

//*****************************************************************************
// ��������: mrsAssembleReadMeterData
// ��������:��֯PLC��������֡
//           
// ����˵��:
//   ��
// �� �� ֵ: 
//    ��
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: liuxipeng/KF54842 [2011-12-16]
//*****************************************************************************
HI_VOID mrsAssembleReadMeterData(HI_VOID)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    MRS_CCO_SRV_CTX_STRU* cco_ctx = mrsCcoGetContext(); // ����ָ��ȫ�������Ľṹ��ָ��
    AFN_14_F1_DL_PARAM * dl_param = HI_NULL;
    AFN_14_F1_DL * dl_data = HI_NULL;
    MRS_PLC_FRAME_DATA_STRU *plc_frame_data = HI_NULL;
    MRS_DATA_TRANSMIT_AFN14_DL_STRU *transmit_data = HI_NULL;
    MRS_ARCHIVES_NODE_STRU *pstNode = HI_NULL;

    if (cco_ctx->stReadMeterModel.stCurrNode.bValid == HI_FALSE)
    {
        // ��ʱMRS_SRV_CCO_TIMER_READMETER_ACTIV_VALUEʱ������¼������ģ��
        MRS_StartTimer(EN_MRS_SRV_CCO_READMETER_ACTIV_TIMER,
                            MRS_SRV_CCO_TIMER_READMETER_ACTIV_VALUE,HI_SYS_TIMER_ONESHOT);
        return;
    }

    pstNode = mrsSrvArchivesQuery(cco_ctx->stReadMeterModel.usCurrNodeSeq);
    if (pstNode == HI_NULL || pstNode->ucRead)
    {
        // ��ʱMRS_SRV_CCO_TIMER_READMETER_ACTIV_VALUEʱ������¼������ģ��
        MRS_StartTimer(EN_MRS_SRV_CCO_READMETER_ACTIV_TIMER,
                            MRS_SRV_CCO_TIMER_READMETER_ACTIV_VALUE,HI_SYS_TIMER_ONESHOT);
        return;
    } 

    dl_param = cco_ctx->stReadMeterModel.pstAfn14F1;  // ��ȡAFN14-F1����
    if(dl_param == HI_NULL)
    {
        mrsReadMeterStateInit();  // ����ģ�������

        // ��ʱMRS_SRV_CCO_TIMER_READMETER_ACTIV_VALUEʱ������¼������ģ��
        MRS_StartTimer(EN_MRS_SRV_CCO_READMETER_ACTIV_TIMER,
                            1000 * 60, HI_SYS_TIMER_ONESHOT);
        return;
    }    

    dl_data = dl_param->pstAFN_14_F1_DL;    // ��ȡAFN01-F1��������

    if(dl_data->ucReadFlg == MRS_ALLOW_READ_CCO) // ���Գ���
    {
        HI_U16 transmit_len = 0;
        HI_U8 mac[HI_PLC_MAC_ADDR_LEN] = {0};
        HI_U8 aucMeter[HI_METER_ADDR_LEN] = {0};
        HI_U32 meter_time = 0;

        mrsMeterAddrPreProc(aucMeter, cco_ctx->stReadMeterModel.stCurrNode.aucNodeAddr, cco_ctx->stReadMeterModel.stCurrNode.ucProtocol);
        
        // ��ȡ����ַ��Ӧ�Ľڵ�MAC��ַ
        ret = mrsMapGetMAC(aucMeter, mac);
        if(ret == HI_ERR_SUCCESS)
        {
            HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1000, HI_DIAG_MT("MAP-MAC"), mac, HI_PLC_MAC_ADDR_LEN);
        }
        else
        {
            (hi_void)memset_s(mac, HI_PLC_MAC_ADDR_LEN, 0xFF, HI_PLC_MAC_ADDR_LEN);   // û���ҵ���Ӧ��MAC��ַ(ʹ�ù㲥MAC)
        }

        // ��ʼ����ʱ�����Դ���    
        mrsGetMaxTryAfn14(mac);
        mrsGetPlcTimeOutAfn14(mac, HI_FALSE, &meter_time);
        
        // �����ڴ�
        transmit_len = sizeof(MRS_DATA_TRANSMIT_AFN14_DL_STRU) + dl_data->ucDataLen;        
        transmit_data = (MRS_DATA_TRANSMIT_AFN14_DL_STRU *)mrsToolsMalloc(transmit_len);
        if(!transmit_data)
        {
            return;
        }
        else
        {
            (hi_void)memset_s(transmit_data, transmit_len, 0, transmit_len);
        }

        // PLC֡�������
        cco_ctx->stReadMeterModel.seq++;
        
        // ��֯����ת����
        transmit_data->interface_ver = MRS_AFN14_DL_VER_R1;
        transmit_data->stru_size     = sizeof(MRS_DATA_TRANSMIT_AFN14_DL_STRU);
        transmit_data->protocol      = pstNode->ucProtocol;
        transmit_data->data_len      = dl_data->ucDataLen; // ���ת�����ݵĳ���
        transmit_data->seq           = cco_ctx->stReadMeterModel.seq;
        transmit_data->time_out      = (HI_U8)MRS_MS_TO_100MS(meter_time);
        (hi_void)memcpy_s(transmit_data->data, transmit_data->data_len, dl_data->pucData, dl_data->ucDataLen); // ��Ҫת�������ݿ�����ת��������

        plc_frame_data = mrsToolsMalloc(sizeof(MRS_PLC_FRAME_DATA_STRU));
        if(!plc_frame_data)
        {
            mrsToolsFree(transmit_data);
            return;
        }
        else
        {
            (hi_void)memset_s(plc_frame_data, sizeof(MRS_PLC_FRAME_DATA_STRU), 0, sizeof(MRS_PLC_FRAME_DATA_STRU));
        }
        
        plc_frame_data->usId         = ID_MRS_CMD_DATA_TRANSMIT_AFN14_PLC;
        (hi_void)memcpy_s(plc_frame_data->ucMac, HI_PLC_MAC_ADDR_LEN, mac, HI_PLC_MAC_ADDR_LEN);
        plc_frame_data->usPayloadLen = transmit_len;              // PLC֡���ݳ���
        plc_frame_data->pucPayload   = (HI_PBYTE)transmit_data; // PLC֡����
        
        cco_ctx->stReadMeterModel.pstPlcDataDl = plc_frame_data; 

    #if defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG)
        //mrsLogFrameInfStatTx(dl_data->pucData, dl_data->ucDataLen, (HI_U8)(mrsToolsBroadCastMac(mac)));//optimized by weichao
    #endif
    }
    else    // ����ʧ�ܻ򳭶��ɹ�
    {
        if(dl_data->ucReadFlg == MRS_READ_FAILED_CCO)
        {
            HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_014, HI_DIAG_MT("LR readmeter fail"));
        }
        else
        {
            pstNode->ucRead = MRS_READ_METER_FLAG_BIT;
            cco_ctx->stReadMeterModel.usSuccNum++;
        }

        cco_ctx->stReadMeterModel.usCurrNodeSeq++; // (��Ϊ�˾�) �ƶ�����һ�ڵ�
        ret = mrsReadMeterFindValidNode(cco_ctx);  // ������Ч�ڵ�
        
        mrsReadMeterStateInit();  // ����ģ���ʼ��

        if (ret == HI_ERR_SUCCESS)
        {
            // �������ģ��
            mrsAcivReadMeter();
        }
    }

    return;
}

//*****************************************************************************
// ��������: mrsTransmit14F1FramePlc
// ��������: ת��PLC��������
//           
// ����˵��:
//   ��
// �� �� ֵ: 
//    ��
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: liuxipeng/KF54842 [2011-12-16]
//*****************************************************************************
HI_VOID mrsTransmit14F1FramePlc(HI_VOID)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    MRS_CCO_SRV_CTX_STRU* pstCcoCtx = mrsCcoGetContext(); // ����ָ��ȫ�������Ľṹ��ָ��
    MRS_PLC_FRAME_DATA_STRU *pstPlcFrameData = pstCcoCtx->stReadMeterModel.pstPlcDataDl;
    PLC_OVERTIME_INF_STRU* TimeOutInf = mrsGetPlcOverTimeInfAfn14();

    if (pstPlcFrameData == HI_NULL)
    {
        return;
    }

    ret = MRS_SendPlcFrame(pstPlcFrameData);

    {//ͳ�ƻ�·���Ժ���������֡ʱ��Ҫ�õ�PLC֡�е�payload,������ʧ��ʱ,���ڴ�ᱻ�ͷ�,���ͳ����Ϣ�����жϷ���ֵ֮ǰ��
        MRS_DATA_TRANSMIT_AFN14_DL_STRU *pstAfn14 = (MRS_DATA_TRANSMIT_AFN14_DL_STRU*)(pstPlcFrameData->pucPayload);
        mrsDfxRmPlcTx((HI_U8*)pstAfn14 + sizeof(MRS_DATA_TRANSMIT_AFN14_DL_STRU), pstAfn14->data_len, ret);
    }

    if(ret == HI_ERR_SUCCESS)
    {
        MRS_StartTimer(EN_MRS_SRV_CCO_STA_TIMER, (HI_U32)TimeOutInf->OverTime, HI_SYS_TIMER_ONESHOT); // ������ʱ��
        mrsSetReadMeterState(EN_MRS_SRV_CCO_READMETER_WAIT_PLC_REPLY);  // �ȴ�PLCӦ��״̬

        mrsDfxLrPlcTx();
    }
    else
    {
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_017, HI_DIAG_MT("tx LR plc fail"));

        mrsReadMeterStateInit();  // ����ģ�������

        // ��ʱMRS_SRV_CCO_TIMER_READMETER_ACTIV_VALUEʱ������¼������ģ��
        MRS_StartTimer(EN_MRS_SRV_CCO_READMETER_ACTIV_TIMER,
                            1000 * 60, HI_SYS_TIMER_ONESHOT);
    }

   return;
}

//*****************************************************************************
// ��������: mrsUpReterMetreData
// ��������: �ϱ���������
//
// ����˵��:
//    ��
//
// �� �� ֵ:
//    ��
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: liuxipeng/KF54842 [2011-12-17]
// ��    ��: cuiate/00233580   [2013-04-01]
//*****************************************************************************
HI_VOID mrsUpReterMetreData(HI_VOID)
{
    HI_U32 ret = HI_ERR_SUCCESS;

    MRS_CCO_SRV_CTX_STRU* pstCcoCtx = mrsCcoGetContext(); // ����ָ��ȫ�������Ľṹ��ָ��
    UP_READMETER_DATA * pstUpReadMeter = HI_NULL;
    HI_U8 *pucBuf = HI_NULL;
    HI_U16 usDataLen = 0;

    PROTO376_2_FRAME_AFN_AND_DATA_STRU stAfnData;
    HI_U8 *pucOutBuf = HI_NULL;
    HI_U16 usBufLen = 0;
    HI_U16 offset = 0;
    HI_U16 usCurrSeq = 0;

    MRS_SRV_CCO_ITEM *pstCcoItem = HI_NULL;


    pstUpReadMeter = pstCcoCtx->stReadMeterModel.pstUpReadMeterData;  // ��ȡ"�ϱ���������"
    if (pstUpReadMeter == HI_NULL)
    {
        mrsReadMeterStateInit();  // ����ģ�������

        // ��ʱMRS_SRV_CCO_TIMER_READMETER_ACTIV_VALUEʱ������¼������ģ��
        MRS_StartTimer(EN_MRS_SRV_CCO_READMETER_ACTIV_TIMER,
                            1000 * 60, HI_SYS_TIMER_ONESHOT);

        return;
    }

    // 4 = �ӽڵ����2 + ��Լ����1 + ���ݳ���1
    // ��ǰ���ı���ͨ������ʱ��(2)
    usDataLen = pstUpReadMeter->usDataLen + 4;
    if (pstCcoCtx->ucProtoVer != PROTO_VER_3762_2009)
    {
        usDataLen += 2;
    }

    pucBuf = mrsToolsMalloc(usDataLen);
    if (!pucBuf)
    {
        return;
    }

    usCurrSeq = pstCcoCtx->stReadMeterModel.usCurrNodeSeq;
    if (MRS_CCO_SEQ_START_FROM_1 == pstCcoCtx->ucSeqControl)
    {
        usCurrSeq++;
    }
    
    // �ӽڵ����
    (hi_void)memcpy_s(pucBuf + offset, usDataLen - offset, &usCurrSeq, 2);
    offset += 2;

    // ��Լ����
    pucBuf[offset++] = pstCcoCtx->stReadMeterModel.stCurrNode.ucProtocol;

    if (pstCcoCtx->ucProtoVer != PROTO_VER_3762_2009)
    {
        pucBuf[offset++] = (MRS_AFN_COMM_DELAY % 256);
        pucBuf[offset++] = (MRS_AFN_COMM_DELAY / 256);
    }

    // ���ĳ���
    pucBuf[offset++] = (HI_U8)((pstUpReadMeter->usDataLen > 255) ? 255 : pstUpReadMeter->usDataLen);
    (hi_void)memcpy_s(pucBuf + offset, usDataLen - offset, pstUpReadMeter->ucData, pstUpReadMeter->usDataLen);

    (hi_void)memset_s(&stAfnData, sizeof(stAfnData), 0, sizeof(stAfnData));
    stAfnData.ucAFN = MRS_AFN(0x06); // AFN = 06H �����ϱ���
    stAfnData.ucDT[0] = MRS_AFN_FN_LO(2);
    stAfnData.ucDT[1] = MRS_AFN_FN_HI(2);
    stAfnData.pData = pucBuf;
    stAfnData.usDataLen = usDataLen;
    stAfnData.ucPhaseFlag = HI_TRUE;

    ret = mrsCreate3762UpFrame(&stAfnData, &pucOutBuf, &usBufLen, pstCcoCtx->stReadMeterModel.stCurrNode.aucNodeAddr, pstCcoCtx->stReadMeterModel.aucMacAdd);

    if (ret != HI_ERR_SUCCESS)
    {
        mrsToolsFree(pucBuf);
        mrsToolsFree(pucOutBuf);
        return;
    }

    pstCcoItem = (MRS_SRV_CCO_ITEM *)mrsToolsMalloc(sizeof(MRS_SRV_CCO_ITEM) + usBufLen);
    if (!pstCcoItem)
    {
        mrsToolsFree(pucBuf);
        mrsToolsFree(pucOutBuf);
        mrsDfxCcoQueueMallocFailCnt();
        return;
    }

    (hi_void)memset_s(pstCcoItem, sizeof(MRS_SRV_CCO_ITEM) + usBufLen, 0, sizeof(MRS_SRV_CCO_ITEM) + usBufLen);
    pstCcoItem->bValid = HI_TRUE;
    pstCcoItem->ucAfn = 0x06;
    pstCcoItem->usFn = 0x02;
    pstCcoItem->usTimeOut = (HI_U16)mrsToolsGetTimeOut(pstCcoCtx->stReadMeterModel.usAfn06F2UpTimeOut);//��λΪms
    pstCcoItem->ucMaxTry = pstCcoCtx->stReadMeterModel.ucAfn06F2UpRetryMax;
    pstCcoItem->usDataLen = usBufLen;
    (hi_void)memcpy_s(pstCcoItem->pData, pstCcoItem->usDataLen, pucOutBuf, usBufLen);
    pstCcoItem->MrsCcoRxRespProc = mrsReadterModelCco;
    pstCcoItem->MrsCcoRxTotalTimeOutProc = mrsReadMeterTotalMusterTimerProcess;
    pstCcoItem->MrsCcoRxEveryTimeOutProc = mrsReadMeterMusterTimerProcess;
    
    mrsCcoJoinQueue(pstCcoItem);

    mrsActiveCcoQueue();
    mrsDfxLrUpDataCnt();
    mrsDfxRmUartTx(pucOutBuf, usBufLen, HI_TRUE);
    mrsSetReadMeterState(EN_MRS_SRV_CCO_READMETER_WAIT_UPDATA_ACK);   // �ȴ�������ȷ��

    mrsToolsFree(pucBuf);
    mrsToolsFree(pucOutBuf);

    return;
}

//*****************************************************************************
// ��������: mrsReadMeterStateInit
// ��������: ����ģ��״̬��ʼ��
//
// ����˵��:
//    ��
// �� �� ֵ:
//    ��
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: liuxipeng/KF54842 [2011-12-16]
//*****************************************************************************
HI_VOID mrsReadMeterStateInit(HI_VOID)
{
    MRS_CCO_SRV_CTX_STRU* pstCcoCtx = mrsCcoGetContext(); // ����ָ��ȫ�������Ľṹ��ָ��
    PLC_OVERTIME_INF_STRU* TimeOutInf = mrsGetPlcOverTimeInfAfn14();

    // ֹͣ�볭����صĶ�ʱ��
    MRS_StopTimer(EN_MRS_SRV_CCO_MUSTER_TIMER);
    MRS_StopTimer(EN_MRS_SRV_CCO_STA_TIMER);

    mrsSetReadMeterState(EN_MRS_SRV_CCO_READMETER_IDLE);  // ��ǰ״̬��Ϊ����

    if (pstCcoCtx->stReadMeterModel.pstAfn14F1 != HI_NULL)
    {
        pstCcoCtx->stReadMeterModel.pstAfn14F1->MRS_AFN14_FREE_FUN(pstCcoCtx->stReadMeterModel.pstAfn14F1);   // �ͷ��ڴ�
    }

    pstCcoCtx->stReadMeterModel.pstAfn14F1 = HI_NULL;

    if(pstCcoCtx->stReadMeterModel.pstPlcDataDl != HI_NULL)
    {
        if(pstCcoCtx->stReadMeterModel.pstPlcDataDl->pucPayload != HI_NULL)
        {
            mrsToolsFree(pstCcoCtx->stReadMeterModel.pstPlcDataDl->pucPayload);   // �ͷ�ת�����ݿ�   
            pstCcoCtx->stReadMeterModel.pstPlcDataDl->pucPayload = HI_NULL;
        }
        
        mrsToolsFree(pstCcoCtx->stReadMeterModel.pstPlcDataDl);   // �ͷ�PLC֡����    
        pstCcoCtx->stReadMeterModel.pstPlcDataDl = HI_NULL;
    }
    if (pstCcoCtx->stReadMeterModel.pstUpReadMeterData != HI_NULL)
    {
        mrsToolsFree(pstCcoCtx->stReadMeterModel.pstUpReadMeterData);   // �ͷ��ڴ�
    }

    pstCcoCtx->stReadMeterModel.pstUpReadMeterData = HI_NULL;
    mrsCcoQueueSetItemInvalid(0x06,0x02);

    TimeOutInf->Try = 0;
    TimeOutInf->MaxTry = TimeOutInf->MaxTryNvS;

    return;
}

//*****************************************************************************
// ��������: mrsReadMeterStaTimerProcess
// ��������: ��������״̬��STAͨѶ��ʱ����ʱ����
//           
// ����˵��:
//   pstCcoCtx [in/out] ��ǰ״̬
//
// �� �� ֵ: 
//    ��
//
// ����Ҫ��: 
// ���þ���: 
// ��    ��: liuxipeng/KF54842 [2012-06-08]
//*****************************************************************************
HI_VOID mrsReadMeterStaTimerProcess( MRS_CCO_SRV_CTX_STRU* pstCcoCtx ) 
{
    PLC_OVERTIME_INF_STRU* TimeOutInf = mrsGetPlcOverTimeInfAfn14();
    HI_U32 num = 0;
/* BEGIN: Modified by fengxiaomin/00209182, 2013/12/11   ���ⵥ��:DTS2013122609670 */
    HI_U32 ret = HI_ERR_SUCCESS;
/* END:   Modified by fengxiaomin/00209182, 2013/12/11 */
    
    if(pstCcoCtx == HI_NULL)
    {
        return;
    }

    HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_020, HI_DIAG_MT("LR- wait plc resp timeout"));

    mrsDfxLrPlcTimeout();
/* BEGIN: Modified by fengxiaomin/00209182, 2014/3/27   ���ⵥ��:DTS2014032906596 */
    if(pstCcoCtx->stReadMeterModel.pstPlcDataDl)
    {
        mrsDfxRmPlcDlRx(ID_MRS_CMD_DATA_TRANSMIT_AFN14_PLC, pstCcoCtx->stReadMeterModel.pstPlcDataDl->pucPayload, HI_TRUE);
    }
/* END:   Modified by fengxiaomin/00209182, 2014/3/27 */
    
    if(pstCcoCtx->stReadMeterModel.enCurrState == EN_MRS_SRV_CCO_READMETER_WAIT_PLC_REPLY)
    {
        HI_U8* mac = HI_NULL;

        if(HI_NULL == pstCcoCtx->stReadMeterModel.pstPlcDataDl)
        {
            HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_021, HI_DIAG_MT("LR - recv frm when timeout"));
            return;
        }
        
        mac = pstCcoCtx->stReadMeterModel.pstPlcDataDl->ucMac;
        
        if (!mrsToolsBroadCastMac(mac))
        {
            if((TimeOutInf->Try >= TimeOutInf->MaxTry) && (TimeOutInf->MaxTryNvB))
            {                
                TimeOutInf->Try = 0;
                
                mrsGetMaxTryAfn14(HI_NULL); // �����Դ�����Ϊ�㲥���Դ���(�ɷǹ㲥ת�㲥) 
                mrsGetPlcTimeOutAfn14(mac, HI_TRUE, HI_NULL);
                
                (hi_void)memset_s(mac, HI_PLC_MAC_ADDR_LEN, 0xFF, HI_PLC_MAC_ADDR_LEN);   // ��Ŀ�ĵ�ַ����Ϊ�㲥��ַ
            }
        }
        
        if(TimeOutInf->Try < TimeOutInf->MaxTry)
        {
            TimeOutInf->Try++;
            
            mrsSetReadMeterState(EN_MRS_SRV_CCO_READMETER_REPEAT_PLC); // �ط���������
            
            mrsReadterModelCco(HI_NULL);   // ������ں�������
            
            return;
        }

        pstCcoCtx->stReadMeterModel.ulTimeOutCount++;   // ��ʱ��������
        mrsDfxLrFailCnt();
#if 0//optimized by weichao
    #if defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG)
        if(pstCcoCtx->stReadMeterModel.pstPlcDataDl->pucPayload != HI_NULL)
        {
            MRS_DATA_TRANSMIT_AFN14_DL_STRU * plcDlData = HI_NULL;
            
            plcDlData = (MRS_DATA_TRANSMIT_AFN14_DL_STRU *)pstCcoCtx->stReadMeterModel.pstPlcDataDl->pucPayload;
            mrsLogPlcOverTimeStat(plcDlData->data, plcDlData->data_len, 0, HI_NULL);
        }
    #endif
#endif
    }    

    {
        MRS_ARCHIVES_NODE_STRU *pstNode = HI_NULL;

        pstNode = mrsSrvArchivesQuery(pstCcoCtx->stReadMeterModel.usCurrNodeSeq);
        
        if (pstNode != HI_NULL)
        {
            mrsCcoReadFailInsert(pstNode->aucNodeAddr, pstNode->ucProtocol);
            mrsDetectLrRmTimeout(pstNode);
            mrsDecreaseMrTTL(&pstCcoCtx->stMapModule, pstNode->aucNodeAddr);
        }
    }
    
    num = mrsGetPlcTimeOutMaxNum();
    if(num < 0xFFFFFFFF)  // �����Դ���Ϊ���ֵʱ��������ת�ӽڵ����
    {
        if(pstCcoCtx->stReadMeterModel.ulTimeOutCount >= num)
        {
            pstCcoCtx->stReadMeterModel.usCurrNodeSeq++; //(��Ϊ�˾�) �ƶ�����һ�ڵ�
            ret = mrsReadMeterFindValidNode(pstCcoCtx);  //������Ч�ڵ�
        }
    }

    mrsReadMeterStateInit();    // ���¿�ʼ��������

    if (ret == HI_ERR_SUCCESS)
    {
        // �������ģ��
        mrsAcivReadMeter();
    }

    return;
}

//*****************************************************************************
// ��������: mrsReadMeterMusterTimerProcess
// ��������: ��������״̬�¼�����ͨѶ��ʱ����ʱ����
//           
// ����˵��:
//   pstCcoCtx [in/out] ��ǰ״̬
//
// �� �� ֵ: 
//    ��
//
// ����Ҫ��: 
// ���þ���: 
// ��    ��: liuxipeng/KF54842 [2012-06-08]
//*****************************************************************************
HI_U32 mrsReadMeterMusterTimerProcess(HI_VOID *param) 
{
    MRS_CCO_SRV_CTX_STRU* pstCcoCtx = mrsCcoGetContext(); // ����ָ��ȫ�������Ľṹ��ָ��

    MRS_NOT_USED(param);
    
    if ((pstCcoCtx->status & MRS_CCO_STATUS_NORMAL) == 0)
    {
        return HI_ERR_FAILURE;
    }

    if(pstCcoCtx->stReadMeterModel.enCurrState == EN_MRS_SRV_CCO_READMETER_WAIT_UPDATA_ACK)        // ���Ϊ�ȴ��ϴ�����ȷ��״̬
    {
        mrsSetReadMeterState(EN_MRS_SRV_CCO_READMETER_REPEAT_UPDATA); // �ط��ϴ���������
        mrsReadterModelCco(HI_NULL);   // ������ں�������

        return HI_ERR_CONTINUE;
    }
    else
    {
        mrsReadMeterTotalMusterTimerProcess(pstCcoCtx);
    }

    return HI_ERR_SUCCESS;
}

HI_U32 mrsReadMeterTotalMusterTimerProcess(HI_VOID *param)
{
    MRS_CCO_SRV_CTX_STRU* pstCcoCtx = mrsCcoGetContext(); // ����ָ��ȫ�������Ľṹ��ָ��

    MRS_NOT_USED(param);
    
    if ((pstCcoCtx->status & MRS_CCO_STATUS_NORMAL) == 0)
    {
        return HI_ERR_FAILURE;
    }

    // 14F1ѯ�ʺ�ȴ�Ӧ�𣬳���������Դ�������Ҫ�е���һ��������ѯ�ʳ���δ��������������Ҫ����ѯ�ʳ���
    if(pstCcoCtx->stReadMeterModel.enCurrState == EN_MRS_SRV_CCO_READMETER_WAIT_ASK_REPLY)
    {
        if(pstCcoCtx->stReadMeterModel.ucCurrNodeInquireTries < pstCcoCtx->stReadMeterModel.ucAfn14F1UpRetryMax + 1)
        {
            mrsReadMeterStateInit();    // ���¿�ʼ��������
            mrsReadterModelCco(HI_NULL);   // ������ں�������
            return HI_ERR_SUCCESS;
        }
        pstCcoCtx->stReadMeterModel.usCurrNodeSeq++;
    }
    
    //���¿�ʼ������������
    mrsReadMeterStateInit();    // ���¿�ʼ��������
        
    // ��ʱMRS_SRV_CCO_TIMER_READMETER_ACTIV_VALUEʱ������¼������ģ��
    MRS_StartTimer(EN_MRS_SRV_CCO_READMETER_ACTIV_TIMER, 
                        MRS_SRV_CCO_TIMER_READMETER_ACTIV_VALUE,HI_SYS_TIMER_ONESHOT);

    return HI_ERR_SUCCESS;
}

//*****************************************************************************
// ��������: mrsReadMeterActiveTimer
// ��������: ����ģ�鼤�ʱ������
//           
// ����˵��:
//   pstCcoCtx [in/out] ��ǰ״̬
//
// �� �� ֵ: 
//    ��
//
// ����Ҫ��: 
// ���þ���: 
// ��    ��: liuxipeng/KF54842 [2012-06-08]
//*****************************************************************************
HI_VOID mrsReadMeterActiveTimer( MRS_CCO_SRV_CTX_STRU* pstCcoCtx ) 
{
/* BEGIN: Added by fengxiaomin/00209182, 2015/9/7   ���ⵥ�ź�:DTS2015091503701 */
    if (0 == (pstCcoCtx->status & MRS_CCO_STATUS_NORMAL))
    {
        return;
    }
/* END:   Added by fengxiaomin/00209182, 2015/9/7 */

    if(pstCcoCtx->stReadMeterModel.enCurrState == EN_MRS_SRV_CCO_READMETER_IDLE)
    {
        pstCcoCtx->stReadMeterModel.ucCurrNodeInquireTries = 0;
        mrsReadterModelCco(HI_NULL);   // ������ں�������
    }
    else
    {
        mrsReadMeterStateInit();    // ���¿�ʼ��������
        
        // �������ģ��
        mrsAcivReadMeter();
    }
    
    return;
}

//*****************************************************************************
// ��������: mrsReadMeterFindValidNode
// ��������: ������Ч�ڵ㣺������ҳɹ���ֱ�ӷ��أ����򽫵�ǰ����ڵ���Ϣ��Ϊ��
//
// ����˵��:
//   pstCcoCtx [in/out] ��ǰ״̬
//
// �� �� ֵ:
//   HI_ERR_SUCCESS ���ҵ���Ч�ڵ�
//   HI_ERR_NOT_FOUND û�в��ҵ���Ч�ڵ�
//   0xFFFFFFFF��ַӳ��������ȡʧ��
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: fengxiaomin/f00209182 [2012-05-26]
//*****************************************************************************
HI_U32 mrsReadMeterFindValidNode(MRS_CCO_SRV_CTX_STRU * pstCcoCtx)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U16 CurrNodeNum = 0;

    if (pstCcoCtx == HI_NULL)
    {
        ret = HI_ERR_FAILURE;

        return ret;
    }

    CurrNodeNum = pstCcoCtx->stReadMeterModel.usCurrNodeSeq;
    if (CurrNodeNum == PRODUCT_CFG_MRS_MAX_NODE_NUM)
    {
        if(mrsGetMaxTurnNum() != 0)
        {
            mrsCurrTurnGrow();  // ��ǰ�ִ�����
        }
        pstCcoCtx->stReadMeterModel.usCurrNodeSeq = 0;
        CurrNodeNum = 0; //��ֹ��ǰ��ų����ڵ�����
    }

    do
    {
        MRS_ARCHIVES_NODE_STRU *pstNode = HI_NULL;
        
        if((mrsGetMaxTurnNum() != 0)
            && (mrsGetCurrTurnNum() >= mrsGetMaxTurnNum()))
        {
            break;
        }

        pstNode = mrsSrvArchivesQuery(CurrNodeNum);
        
        if (pstNode != HI_NULL && pstNode->bValid && !pstNode->ucRead)
        {
            pstCcoCtx->stReadMeterModel.usCurrNodeSeq = CurrNodeNum; //(��Ϊ�˾�)��ǰ����Ĵӽڵ����
            (hi_void)memcpy_s(&pstCcoCtx->stReadMeterModel.stCurrNode, sizeof(pstCcoCtx->stReadMeterModel.stCurrNode), 
                pstNode, sizeof(MRS_ARCHIVES_NODE_STRU)); //��ǰ����Ĵӽڵ���Ϣ
            pstCcoCtx->stReadMeterModel.ulTimeOutCount = 0;   // �ҵ��½ڵ��ʼ����ʱ����

            return ret;
        }

        CurrNodeNum++;

        if (CurrNodeNum == PRODUCT_CFG_MRS_MAX_NODE_NUM)
        {
            if(mrsGetMaxTurnNum() != 0)
            {
                mrsCurrTurnGrow();  // ��ǰ�ִ�����
            }
            CurrNodeNum = 0;
        }
    } while (CurrNodeNum != pstCcoCtx->stReadMeterModel.usCurrNodeSeq); //(��Ϊ�˾�)

    mrsSetCurrTurnNum(0);   // ��ǰ�ִ�����

    if (MRS_CCO_LR_MODE_CONTINUOUS == pstCcoCtx->ucLrModeFlag)
    {
        mrsSrvArchivesReadFlagClear();
        ret = HI_ERR_SUCCESS;
    }
    else
    {
        mrsForceSwitchPause();
        mrs3762_ReportAFN06_F3(MRS_CCO_READ_METER_TASK_END); //�����������
        ret = HI_ERR_NOT_FOUND;
    }

    (hi_void)memset_s(&pstCcoCtx->stReadMeterModel.stCurrNode, sizeof(MRS_ARCHIVES_NODE_STRU), 0, sizeof(MRS_ARCHIVES_NODE_STRU));
    pstCcoCtx->stReadMeterModel.usCurrNodeSeq = 0;

    return ret;
}

//*****************************************************************************
// ��������: mrsCurrNodeValid
// ��������: �жϳ�������е�ǰ�ڵ��Ƿ���Ч
//
// ����˵��:
//
// �� �� ֵ:
//   HI_FALSE ��ǰ�ڵ���Ч
//   HI_TRUE ��ǰ��Ч�ڵ���Ч
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: fengxiaomin/f00209182 [2012-09-04]
//*****************************************************************************
HI_BOOL mrsCurrNodeValid(HI_VOID)
{
    MRS_CCO_SRV_CTX_STRU* pstCcoCtx = mrsCcoGetContext();
    MRS_ARCHIVES_NODE_STRU *pstNode = HI_NULL;
        
    pstNode = mrsSrvArchivesQuery(pstCcoCtx->stReadMeterModel.usCurrNodeSeq);
    if (pstNode == HI_NULL)
    {
        return HI_FALSE;
    }
    if(pstNode->ucRead)
	{
		return HI_FALSE;
	}
    if (!pstCcoCtx->stReadMeterModel.stCurrNode.bValid || !pstNode->bValid)
    {
        return HI_FALSE;
    }
    else if (!mrsToolsMemEq(pstCcoCtx->stReadMeterModel.stCurrNode.aucNodeAddr, pstNode->aucNodeAddr, HI_METER_ADDR_LEN))
    {
        return HI_FALSE;
    }

    return HI_TRUE;
}


//*****************************************************************************
// ��������: mrsReadMeterReceivePlcProcess
// ��������: ���ն���PLC���ݴ���
//
// ����˵��:
//          pstData ת������������
//          pstCcoCtx ȫ��������
// �� �� ֵ:
//          �μ������б�
//
// ����Ҫ��: ��
// ���þ���: ��
// ��    ��: liuxipeng/lKF54842 [2012-09-17]
//*****************************************************************************
HI_U32 mrsReadMeterReceivePlcProcess(MRS_DATA_TRANSMIT_AFN14_UP_STRU * transmit_data,
                                                MRS_CCO_SRV_CTX_STRU * cco_ctx, HI_U8 ucMacAdd[HI_PLC_MAC_ADDR_LEN])
{
    UP_READMETER_DATA * upload_data = HI_NULL;  //�ϱ�������
    HI_U16 upload_size = 0;
    HI_U8 *buffer = HI_NULL;
    MRS_ARCHIVES_NODE_STRU *pstNode = HI_NULL;

    //��ǰӦ���ǵȴ�����Ӧ��״̬
    if(cco_ctx->stReadMeterModel.enCurrState != EN_MRS_SRV_CCO_READMETER_WAIT_PLC_REPLY)
    {   
        mrsDfxLrPlcExceedCnt();
        mrsDfxRmPlcUpRx(ID_MRS_CMD_DATA_TRANSMIT_AFN14_PLC, (HI_U8*)transmit_data, HI_TRUE);
        return HI_ERR_FAILURE;
    }

    if (transmit_data->seq != cco_ctx->stReadMeterModel.seq)
    {   
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_031, HI_DIAG_MT("recv plc seq err, abandon"));
        mrsDfxLrPlcSeqErr();    // ͳ�ƶ�����PLC֡����
        mrsDfxRmPlcUpRx(ID_MRS_CMD_DATA_TRANSMIT_AFN14_PLC, (HI_U8*)transmit_data, HI_TRUE);
        return HI_ERR_FAILURE;
    }
    
    //���ʹ����û���ͷ��ϱ������ڴ棬ѡ���ͷ�
    if(cco_ctx->stReadMeterModel.pstUpReadMeterData != HI_NULL)
    {
        upload_data = cco_ctx->stReadMeterModel.pstUpReadMeterData;
        mrsToolsFree(upload_data);
    }

    pstNode = mrsSrvArchivesQuery(cco_ctx->stReadMeterModel.usCurrNodeSeq);
    buffer = (HI_U8 *)transmit_data + transmit_data->stru_size;

    if (mrsCcoIsTestMode() && pstNode != HI_NULL)
    {
        HI_U16 usPos = 0;
        HI_U16 usFrameLen = 0;

        if ((HI_ERR_SUCCESS != mrsFind645Frame(buffer, (HI_S16)transmit_data->data_len, &usPos, &usFrameLen))
            || (!mrsMeterAddrMatch(pstNode->aucNodeAddr, buffer + usPos + MRS_645_FRAME_METERADD_OFFSET)))
        {
            HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_013, HI_DIAG_MT("[LR]����ģʽ����ַ��ƥ��"));
            return HI_ERR_FAILURE;
        }
    }

    //��̽�Ⳮ���б����г����ɹ����̽���־���
    if (HI_NULL != pstNode)
    {
        mrsDetectRmRespProc(pstNode->aucNodeAddr);
    }
    
    //׼���ϴ�����
    upload_size = sizeof(UP_READMETER_DATA) + transmit_data->data_len;
    upload_data = (UP_READMETER_DATA *)mrsToolsMalloc(upload_size);
    cco_ctx->stReadMeterModel.pstUpReadMeterData = upload_data;

    if(upload_data != HI_NULL)
    {
        (hi_void)memset_s(upload_data, upload_size, 0, upload_size);
        upload_data->usDataLen = transmit_data->data_len;
        (hi_void)memcpy_s(upload_data->ucData, upload_data->usDataLen, buffer, transmit_data->data_len);
    }
    else
    {
        mrsReadMeterStateInit();
 
        MRS_StartTimer(EN_MRS_SRV_CCO_READMETER_ACTIV_TIMER,
                            1000 * 60,
                            HI_SYS_TIMER_ONESHOT);
        
        return HI_ERR_NOT_ENOUGH_MEMORY;
    } 

    if (HI_NULL != pstNode)
    {
        mrsCcoReadFailRemove(pstNode->aucNodeAddr, pstNode->ucProtocol);
    }
#if 0//optimized by weichao
#if defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG)
    if(cco_ctx->stReadMeterModel.pstPlcDataDl != HI_NULL)
    {
        MRS_DATA_TRANSMIT_AFN14_DL_STRU * plcDlData = HI_NULL;
        
        plcDlData = (MRS_DATA_TRANSMIT_AFN14_DL_STRU *)cco_ctx->stReadMeterModel.pstPlcDataDl->pucPayload;
        if(plcDlData != HI_NULL)
        {
            mrsLogFrameInfStatRx(buffer, transmit_data->data_len, plcDlData->data, plcDlData->data_len, 1);
        }
    }
#endif
#endif
    if (ucMacAdd == HI_NULL)
    {
        (hi_void)memset_s(cco_ctx->stReadMeterModel.aucMacAdd, HI_PLC_MAC_ADDR_LEN, 0, HI_PLC_MAC_ADDR_LEN);
    }
    else
    {
        (hi_void)memcpy_s(cco_ctx->stReadMeterModel.aucMacAdd, HI_PLC_MAC_ADDR_LEN,ucMacAdd, HI_PLC_MAC_ADDR_LEN);
    }
    
    mrsReadterModelCco(HI_NULL);

    return HI_ERR_SUCCESS;
}

//*****************************************************************************
// ��������: mrsReadMeterResume
// ��������: �ָ��ֳ�����
//
// ����˵��:
//          ��
// �� �� ֵ:
//          ��
//
// ����Ҫ��: ��
// ���þ���: ��
// ��    ��: fengxiaomin/00209182 [2015-09-07]
//*****************************************************************************
HI_VOID mrsReadMeterResume(HI_VOID)
{
    HI_U32 status = mrsCcoGetState();
    MRS_CCO_SRV_CTX_STRU *pstCcoCtx = mrsCcoGetContext();

    if (HI_TRUE == pstCcoCtx->bPauseFlag)
    {
        return;
    }
    
    status |= MRS_CCO_STATUS_NORMAL;
    mrsCcoSetState(status);
    
    mrsReadMeterStateInit();    // ��ʱ����ģ���ʼ��
    // ��ʱMRS_SRV_CCO_TIMER_READMETER_ACTIV_VALUEʱ������¼������ģ��
    MRS_StartTimer(EN_MRS_SRV_CCO_READMETER_ACTIV_TIMER,
                        MRS_SRV_CCO_TIMER_READMETER_ACTIV_VALUE,HI_SYS_TIMER_ONESHOT);
}
HI_VOID mrsReadWaterMeterStateInit(HI_VOID)
{
    MRS_CCO_SRV_CTX_STRU* pstCcoCtx = mrsCcoGetContext(); // ����ָ��ȫ�������Ľṹ��ָ��
    //PLC_OVERTIME_INF_STRU* TimeOutInf = mrsGetPlcOverTimeInfAfn14();//weichao
	HI_U16 usMeterAllNum = mrsSrvArchivesMeterNum();
	MRS_ARCHIVES_NODE_STRU *pstNode = HI_NULL;
	HI_U32 i=0;
	
	pstCcoCtx->stReadWaterMeterModel.enCurrRfState =1;

    if(pstCcoCtx->stReadWaterMeterModel.pstRfPlcDataDl != HI_NULL)
    {
        if(pstCcoCtx->stReadWaterMeterModel.pstRfPlcDataDl->pucPayload != HI_NULL)
        {
            mrsToolsFree(pstCcoCtx->stReadWaterMeterModel.pstRfPlcDataDl->pucPayload);   // �ͷ�ת�����ݿ�   
            pstCcoCtx->stReadWaterMeterModel.pstRfPlcDataDl->pucPayload = HI_NULL;
        }
        
        mrsToolsFree(pstCcoCtx->stReadWaterMeterModel.pstRfPlcDataDl);   // �ͷ�PLC֡����    
        pstCcoCtx->stReadWaterMeterModel.pstRfPlcDataDl = HI_NULL;
    }
    if (pstCcoCtx->stReadWaterMeterModel.pstRfUpReadMeterData != HI_NULL)
    {
        mrsToolsFree(pstCcoCtx->stReadWaterMeterModel.pstRfUpReadMeterData);   // �ͷ��ڴ�
    }

    pstCcoCtx->stReadWaterMeterModel.pstRfUpReadMeterData = HI_NULL;

    pstCcoCtx->stReadWaterMeterModel.ucCurrNodeInquireTries=0;
	 pstCcoCtx->stReadWaterMeterModel.usPLCTimeOut = 8000;

	pstCcoCtx->stReadWaterMeterModel.usAfn20F3UpTimeOut = 3000;
	pstCcoCtx->stReadWaterMeterModel.ucAfn20F3UpRetryMax =2;
	pstCcoCtx->stReadWaterMeterModel.ulTimeOutMaxNumNv =2;

   
	//�ڴ˴��ҵ���ǰ�����Ϣ�е�һ����Ч�ı���Ϣ����ţ�
    for (i = 0; i < usMeterAllNum; i++)
    {
        pstNode = mrsSrvArchivesQuery(i);
        if (pstNode == HI_NULL)
        {
            break;
        }

        if (pstNode->bValid && (pstNode->ucProtocol>=METER_PROTO_SINGLEWAY_WATERMETER)) // ���ҵ�һ����Ч�ı���Ϣ
        {
            pstNode->ucRfReportFlag=0;
        }
    }
    return;
}
HI_U32 mrsMapGetWaterMeterMAC(HI_U8 *pucDeviceAddr, HI_U8 *pucMacAddr)
{
    MRS_CCO_SRV_CTX_STRU *pCco = mrsCcoGetContext();
    HI_U32 ulRet = HI_ERR_SUCCESS;
	HI_U16 i=0;

	for(i=0;i<pCco->stWaterMeterSearchModel.usWaterMeterResultNum;i++)
	{
		if(0 ==mrsToolsMemcmp(pCco->stWaterMeterSearchModel.stWaterMeterResult[i].ucaWaterMac,pucDeviceAddr,6))
		{
			mrsToolsMemcpy_s(pucMacAddr, HI_PLC_MAC_ADDR_LEN, pCco->stWaterMeterSearchModel.stWaterMeterResult[i].ucaStaMac, HI_PLC_MAC_ADDR_LEN);
			return ulRet;
		}
	}
    return HI_ERR_NOT_FOUND;
}

HI_U32 mrsReadWaterMeterEncode(HI_VOID)
{
    //HI_U32 ret = HI_ERR_SUCCESS;
    MRS_CCO_SRV_CTX_STRU* pstCcoCtx = mrsCcoGetContext(); // ����ָ��ȫ�������Ľṹ��ָ��
	MRS_PLC_FRAME_DATA_STRU *plc=HI_NULL;
	MRS_PLC_METERLIST_DL_S * meterlist = HI_NULL;
	HI_U8 * payload = HI_NULL;
	HI_U16 payload_len = sizeof(MRS_PLC_METERLIST_DL_S)+4;
	HI_U8 aucIp[HI_IPV4_ADDR_LEN];
	HI_U16 usOffent=0;

	if(pstCcoCtx->stTopoInf.entry[pstCcoCtx->stReadWaterMeterModel.usCurrRfNodeSeq].tei ==1)
	{
		pstCcoCtx->stReadWaterMeterModel.usCurrRfNodeSeq++;
	}
	
	HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_025, HI_DIAG_MT("ReadWaterMeterEncode usCurrRfNodeSeq,All"),pstCcoCtx->stReadWaterMeterModel.usCurrRfNodeSeq,pstCcoCtx->stTopoInf.num);
    if (pstCcoCtx->stReadWaterMeterModel.usCurrRfNodeSeq >= pstCcoCtx->stTopoInf.num)   //û�е��
    {
        pstCcoCtx->stReadWaterMeterModel.usCurrRfNodeSeq = 0;  //(��Ϊ����)
        pstCcoCtx->stReadWaterMeterModel.ulTimeOutCount = 0;   // �ҵ��½ڵ��ʼ����ʱ����
        mrs3762_ReportAFN20_F4();
		RfFreePLCData(pstCcoCtx->stReadWaterMeterModel.pstRfPlcDataDl);
        return HI_ERR_NOT_FOUND;
    } 
   
    // ��ȡ����ַ��Ӧ�Ľڵ�MAC��ַ
    // ��ʼ����ʱ�����Դ���    
    // �����ڴ�
    payload = (HI_U8 *)mrsToolsMalloc(payload_len);
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

    // PLC֡�������
    pstCcoCtx->stReadWaterMeterModel.usRfPlcSeq++;
	
    mrsToolsMemZero_s(payload, payload_len, payload_len);
    meterlist = (MRS_PLC_METERLIST_DL_S *)payload;
    meterlist->interface_ver = MRS_GET_METERLIST_DL_VER;
    meterlist->stru_len = sizeof(MRS_PLC_METERLIST_DL_S);
    meterlist->option = MRS_SM_WATER_CMD;
    meterlist->seq = pstCcoCtx->stReadWaterMeterModel.usRfPlcSeq;

   	meterlist->waterMeter_Data[usOffent++]=3;
	meterlist->waterMeter_Data[usOffent++]=0;
	meterlist->waterMeter_Data[usOffent++]=0;
	meterlist->waterMeter_Data[usOffent++]=48;
    (HI_VOID)HI_MDM_NM_GetAddr(meterlist->src, aucIp);
    mrsToolsMemcpy_s(meterlist->dst, HI_PLC_MAC_ADDR_LEN, pstCcoCtx->stTopoInf.entry[pstCcoCtx->stReadWaterMeterModel.usCurrRfNodeSeq].mac, HI_PLC_MAC_ADDR_LEN);

    mrsToolsMemZero_s(plc, sizeof(plc), sizeof(plc));
    plc->usId = ID_MRS_CMD_GET_METERLIST;
    plc->pucPayload = payload;
    plc->usPayloadLen = payload_len;
    mrsToolsMemcpy_s(plc->ucMac, HI_PLC_MAC_ADDR_LEN, meterlist->dst, HI_PLC_MAC_ADDR_LEN);
	
    // ��֯����ת����
    if(pstCcoCtx->stReadWaterMeterModel.pstRfPlcDataDl != HI_NULL)
    {
        if(pstCcoCtx->stReadWaterMeterModel.pstRfPlcDataDl->pucPayload != HI_NULL)
        {
            mrsToolsFree(pstCcoCtx->stReadWaterMeterModel.pstRfPlcDataDl->pucPayload);   // �ͷ�ת�����ݿ�   
            pstCcoCtx->stReadWaterMeterModel.pstRfPlcDataDl->pucPayload = HI_NULL;
        }
        
        mrsToolsFree(pstCcoCtx->stReadWaterMeterModel.pstRfPlcDataDl);   // �ͷ�PLC֡����    
        pstCcoCtx->stReadWaterMeterModel.pstRfPlcDataDl = HI_NULL;
    }
    pstCcoCtx->stReadWaterMeterModel.pstRfPlcDataDl = plc;
    HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_027, HI_DIAG_MT("ReadWaterMeterEncode end"));
    return HI_ERR_SUCCESS;

}
HI_VOID mrsRfTransmitFramePlcData(HI_VOID)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    MRS_CCO_SRV_CTX_STRU* pstCcoCtx = mrsCcoGetContext(); // ����ָ��ȫ�������Ľṹ��ָ��
    MRS_PLC_FRAME_DATA_STRU *pstPlcFrameData = pstCcoCtx->stReadWaterMeterModel.pstRfPlcDataDl;
    //PLC_OVERTIME_INF_STRU* TimeOutInf = mrsGetPlcOverTimeInfAfn14();//weichao

    if (pstPlcFrameData == HI_NULL)
    {
        return;
    }

    ret = MRS_SendPlcFrame(pstPlcFrameData);
    if(ret == HI_ERR_SUCCESS)
    {
        MRS_StartTimer(MRS_CCO_TIMER_RF_READ_WATERMETER, pstCcoCtx->stReadWaterMeterModel.usPLCTimeOut, HI_SYS_TIMER_ONESHOT); // ������ʱ��
        pstCcoCtx->stReadWaterMeterModel.enCurrRfState=2; // �ȴ�PLCӦ��״̬
    }
    else
    {
        mrsReadWaterMeterStateInit();  // ����ģ�������

        // ��ʱMRS_SRV_CCO_TIMER_READMETER_ACTIV_VALUEʱ������¼������ģ��
        MRS_StartTimer(MRS_CCO_TIMER_RF_READ_WATERMETER,1000 * 60, HI_SYS_TIMER_ONESHOT);
    }
	HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_028, HI_DIAG_MT("mrsRfTransmitFramePlcData"),ret);

   return;
}

HI_VOID mrsReadWaterMeterActiveTimer( MRS_CCO_SRV_CTX_STRU* pstCcoCtx ) 
{
	HI_U32 ret=HI_ERR_SUCCESS;
    if (0 == (pstCcoCtx->status & MRS_CCO_STATUS_NORMAL))
    {
		//HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_090, HI_DIAG_MT("ReadWaterMeterActiveTimer NOT MRS_CCO_STATUS_NORMAL"),pstCcoCtx->status);
        //return;
    }
	HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_023, HI_DIAG_MT("ReadWaterMeterActiveTimer enCurrRfState"),pstCcoCtx->stReadWaterMeterModel.enCurrRfState);

	switch(pstCcoCtx->stReadWaterMeterModel.enCurrRfState)
	{
		case 1:
			ret = mrsReadWaterMeterEncode();
			if(ret==HI_ERR_SUCCESS)
			{
				mrsRfTransmitFramePlcData();
			}
			break;
		case 2:
			pstCcoCtx->stReadWaterMeterModel.ulTimeOutCount++;
			if(pstCcoCtx->stReadWaterMeterModel.ulTimeOutCount> pstCcoCtx->stReadWaterMeterModel.ulTimeOutMaxNumNv)
			{
				pstCcoCtx->stReadWaterMeterModel.ulTimeOutCount=0;
				pstCcoCtx->stReadWaterMeterModel.enCurrRfState =1;
				pstCcoCtx->stReadWaterMeterModel.usCurrRfNodeSeq++;
				
				HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_024, HI_DIAG_MT("ReadWaterMeterActiveTimer ��ʱ��һ��"));
				MRS_StartTimer(MRS_CCO_TIMER_RF_READ_WATERMETER, 100, HI_SYS_TIMER_ONESHOT); // ������ʱ��
				break;
			}
			mrsRfTransmitFramePlcData();
			break;
		case 3:
			pstCcoCtx->stReadWaterMeterModel.usCurrRfNodeSeq++;
			pstCcoCtx->stReadWaterMeterModel.ulTimeOutCount=0;
			pstCcoCtx->stReadWaterMeterModel.enCurrRfState =1;
			MRS_StartTimer(MRS_CCO_TIMER_RF_READ_WATERMETER, 100, HI_SYS_TIMER_ONESHOT); // ������ʱ��
			break;
		case 0:
			mrsReadWaterMeterStateInit();    // ���¿�ʼ��������
        	pstCcoCtx->stReadWaterMeterModel.enCurrRfState =1;
			break;
	}
	
    return;
}

HI_VOID mrsWaterMeterModelRxRespProc(HI_VOID * pAckParam)
{
	MRS_CCO_SRV_CTX_STRU* pstCcoCtx = mrsCcoGetContext();
	
	HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_042, HI_DIAG_MT("MeterModelRxRespProc"));
    mrsReadWaterMeterActiveTimer(pstCcoCtx);
}
HI_U32 mrsWaterMeterModelAllTimeOutProc(HI_VOID * pAckParam)
{
	HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_043, HI_DIAG_MT("ModelAllTimeOutProc"));
    return mrs3762_ReportAFN20_F4();
}
#endif

