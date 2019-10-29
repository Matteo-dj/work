//*****************************************************************************
//
//                  ��Ȩ���� (C), 1998-2011, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mrs_fw_main.c
//  �� �� ��   : V1.0
//  ��    ��   : shenhankun/00130424
//  ��������   : 2011-10-25
//  ��������   : MRS�������ʵ��
//
//  �����б�   :
//  �޸���ʷ   :
//  1.��    �� : 2011-10-25
//    ��    �� : shenhankun/00130424
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
#include "mrs_fw_n.h"
#include <app_nv.h>
#include <hi_usr_nv.h>
#include "mrs_fw_log.h"
#include "mrs_fw_tools.h"
#include "mrs_fw_proto376_2_echo.h"
#include "mrs_fw_proto376_2_frame.h"
#include "mrs_srv_cco_macmap.h"
// DUT_B
#include "mrs_srv_io.h"
#include "equip_dut_proc.h"
#include "hi_ft_nv.h"
#include "mrs_fw_cmd.h"
// DUT_E
#include "hi_config.h"
#include "dfx_sys.h"
#include "hi_mdm.h"
#include "mrs_dfx.h"
#include "mrs_fw_tools.h"
#include "mrs_srv_collector_irda.h"
#include "mrs_srv_sta_queue.h"
#include "mrs_srv_collector.h"
#include "mrs_srv_common.h"
#include "mrs_srv_res.h"
#include "mrs_srv_parallel_sta.h"
#include "mrs_srv_sta_event.h"
#include "mrs_srv_sta.h"
#include "mrs_srv_sta_searchmeter.h"
#include "mrs_srv_cltii_searchmeter.h"
#include "mrs_srv_cco_remote_upg.h"
#include "hi_mdm.h"
#include "mrs_srv_readmeter_manage.h"
#include "mrs_fw_nv.h"
#include "mrs_srv_cco_archives.h"
#include "mrs_dfx_readmeter_config.h"
#include "mrs_srv_cco_slavenode_on_off_line.h"
#include "mrs_srv_cltii_event.h"
#include "mrs_srv_cco_testmode.h"
#include "mrs_srv_cco_readmeter.h"
#include "mrs_srv_cco_detect_rm.h"
#include "mrs_dfx.h"
#include "mrs_srv_cco_mr_task.h"
//#include "mrs_srv_cipher.h"
#if (defined(PRODUCT_CFG_PRODUCT_TYPE_STA) && defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD))
#include "mrs_srv_clti_upg_csg.h"
#endif
#include "mrs_srv_baudrate_manage.h"
#include "mrs_srv_capture.h"
#include "mrs_srv_cco.h"
#include "mrs_power_failure.h"

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
HI_PRV HI_U32 g_ulMrsBaudRate = 0;
#endif

HI_PRV HI_U32 mrsFwInit(HI_VOID);
HI_PRV HI_U32 mrsTimerProc(HI_U32 ulTimerId);
HI_PRV HI_U32 mrsChlConnectMsgProc(HI_U16 usPort, HI_U8 ucOption);

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
HI_PRVL HI_U32 mrsMsgProcCcoFrameInput(HI_IN HI_U16 usId, HI_IN HI_SYS_QUEUE_MSG_S* pstMsg);
HI_PRVL HI_U32 mrsMsgProcMacMsg(HI_IN HI_SYS_QUEUE_MSG_S* pstMsg);
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
HI_PRVL HI_U32 mrsMsgProc645Input(HI_IN HI_SYS_QUEUE_MSG_S* pstMsg);
#endif

HI_PRVL HI_U32 mrsMsgProcMacInput(HI_IN HI_SYS_QUEUE_MSG_S* pstMsg);
HI_PRVL HI_U32 mrsMsgProcMrOutput(HI_IN HI_SYS_QUEUE_MSG_S* pstMsg);
HI_PRVL HI_U32 mrsMsgProcMacOutPut(HI_IN HI_SYS_QUEUE_MSG_S* pstMsg);
HI_PRV HI_U32 mrsMsgProc(HI_IN HI_SYS_QUEUE_MSG_S* pstMsg);
HI_PRV HI_U32 mrsCcoStaMsgProc(HI_IN HI_SYS_QUEUE_MSG_S* pstMsg);
HI_PRV HI_U32 msrFwDefaultProc(HI_IN HI_SYS_QUEUE_MSG_S* pstMsg);
HI_PRVL HI_U32 mrsDispatchCommand(HI_U16 usId, HI_IN HI_DMS_CHL_RX_S* psRx);
HI_PRVL HI_U32 mrsSend2MrDevice(HI_IN HI_SYS_QUEUE_MSG_S*);
HI_PRVL  HI_U32 mrsSend2MacLayer(HI_IN HI_SYS_QUEUE_MSG_S* pstMsg);
HI_PRV MRS_CMD_NODE_STRU* mrsFindCmdNode(HI_U16 usCmdId);
HI_PRV MRS_MSG_NODE_STRU* mrsFindMsgNode(HI_U16 usId);
HI_PRV HI_U32 DoMrRxProcess(HI_IN HI_DMS_CHL_RX_S *pstRx);

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
HI_PRV HI_U32 DoStaMrRxProcess(HI_IN HI_DMS_CHL_RX_S* pstRx);
HI_PRV HI_U32 mrsDispatchRx645Data(HI_IN MRS_ONE_RAW_FRAME_STRU* pstRx);
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
HI_PRV HI_U32 DoCltIMrRxProcess(HI_IN HI_DMS_CHL_RX_S* pstRx);
#endif
#endif

#if (defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) || defined(PRODUCT_CFG_PRODUCT_TYPE_STA))
HI_PRV HI_U32 DoCcoMrRxProcess(HI_IN HI_DMS_CHL_RX_S* pstRx, HI_U16 usFrameType);
HI_PRV HI_U32 mrsCcoDispatchRxFrameData(HI_IN HI_U16 usId, HI_IN MRS_ONE_RAW_FRAME_STRU* pstFrame);
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
HI_PRV HI_U32 AppDiagChlConnectNotify (HI_U16 usPortNum, HI_BOOL bConnect);

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO_BROAD_BAND_UNIT)
HI_PRV HI_U32 DoCcoMrRxProcessBbu(HI_IN HI_DMS_CHL_RX_S* pstRx);
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
HI_PRV HI_U32 DoCcoMrRxCsg(HI_IN HI_DMS_CHL_RX_S* pstRx);
#else
HI_PRV HI_U32 DoCcoMrRx3762(HI_IN HI_DMS_CHL_RX_S* pstRx);
#endif
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_NDM)
HI_PRV HI_U32 DoNdmMrRxProcess(HI_IN HI_DMS_CHL_RX_S* pstRx);
HI_PRV HI_BOOL mrsIsCcdDutMode(MRS_645_FRAME_STRU* pst645Hrd);
HI_PRV HI_U32 mrsCcdDutSend(HI_PBYTE pucDatagram, HI_U16 usDatagramSize, HI_U8* pDataRealm);
#endif

HI_VOID APP_MrsTaskInit(HI_VOID);
HI_VOID APP_MrsTaskBody(HI_U32 ulParam);
HI_PRV HI_BOOL mrsAllowJoinMsgQueue(HI_U32 ulMsgId);


//*****************************************************************************
// MRS ��������Ķ���
//*****************************************************************************
HI_PRV MRS_FW_CTX_STRU* g_pstMrsFwCtx = HI_NULL;
//*****************************************************************************

MRS_FW_CTX_STRU* mrsGetFwCtx(HI_VOID)
{
    return g_pstMrsFwCtx;
}

HI_PRVL MRS_FW_CTX_STRU* GetFwCtx(HI_VOID)
{
    return g_pstMrsFwCtx;
}


HI_U32 MRS_Init(HI_VOID)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    MRS_FW_CTX_STRU *pstFwCtx = HI_NULL;
    HI_S32 i = 0;
    HI_PCSTR szSemName[EN_APP_SEM_MAX] = APP_SEM_NAME_DEFINE;

    // ������ܳ�ʼ��
    if(HI_ERR_SUCCESS == mrsFwInit())
    {
        HI_MDM_ReportInitEvt(HI_DEID_APP_MRS_FW_INIT_OK, HI_DIAG_SYS_INIT_SET);
    }
    else
    {
        HI_MDM_ReportInitEvt(HI_DEID_APP_MRS_FW_INIT_FAIL, HI_DIAG_SYS_INIT_SET);
    }

    mrsRegistCommandInit();

    mrsDfxInit();

    pstFwCtx = GetFwCtx();
    
    // ���г�ʼ��
    ulRet = HI_MDM_MSG_CreateQueue(&(pstFwCtx->stResCfg.aulQueueId[EN_APP_QUEUE_MRS]), "MRS_Q", PRODUCT_CFG_Q_APP_MRS_MSG_NUM);
    if (HI_ERR_SUCCESS == ulRet)
    {
        HI_MDM_ReportInitEvt(HI_DEID_APP_QUEUE_INIT_OK, HI_DIAG_SYS_INIT_SET);
    }
    else
    {
        HI_MDM_ReportInitEvt(HI_DEID_APP_QUEUE_INIT_FAIL, HI_DIAG_SYS_INIT_SET);
    }


    // �����ʼ��
    APP_MrsTaskInit();

    // �ź�����ʼ��
    for (i = 0; i < EN_APP_SEM_MAX; i++)
    {
        ulRet = HI_MDM_SEM_BCreate(&(pstFwCtx->stResCfg.aulSemId[i]), (HI_CHAR *)szSemName[i], 1);
        if (HI_ERR_SUCCESS == ulRet)
        {
            HI_MDM_ReportInitEvt(HI_DEID_APP_QUEUE_INIT_OK, HI_DIAG_SYS_INIT_SET);
        }
        else
        {
            HI_MDM_ReportInitEvt(HI_DEID_APP_QUEUE_INIT_FAIL, HI_DIAG_SYS_INIT_SET);
        }
    }

    // ��������
    ulRet = HI_MDM_TASK_Create(&(pstFwCtx->stResCfg.aulTaskId[EN_APP_TASK_APP_MRS]), 
        "MRS_T", APP_MrsTaskBody, 0, PRODUCT_CFG_TASK_STACK_SIZE_APP_MRS, PRODUCT_CFG_TASK_PRIO_APP_MRS);
    if (HI_ERR_SUCCESS == ulRet)
    {
        HI_MDM_ReportInitEvt(HI_DEID_APP_QUEUE_INIT_OK, HI_DIAG_SYS_INIT_SET);
    }
    else
    {
        HI_MDM_ReportInitEvt(HI_DEID_APP_QUEUE_INIT_FAIL, HI_DIAG_SYS_INIT_SET);
    }


    return HI_ERR_SUCCESS;
}

HI_U32 MRS_SendMessage(HI_U32 ulQueueId, HI_SYS_QUEUE_MSG_S *pstMsg, HI_U32 ulTimeOut)
{
    MRS_FW_CTX_STRU* pstFwCtx = mrsGetFwCtx();

    if (!pstFwCtx)
    {
        return HI_ERR_NO_INITILIZATION;
    }

    if (ulQueueId >= EN_APP_Q_MAX)
    {
        return HI_ERR_INVALID_ID;
    }

    return HI_MDM_MSG_Send((HI_U32)pstFwCtx->stResCfg.aulQueueId[ulQueueId], pstMsg, ulTimeOut);
}

HI_U32 MRS_WaitMessage(HI_U32 ulQueueId, HI_SYS_QUEUE_MSG_S *pstMsg, HI_U32 ulTimeOut)
{
    MRS_FW_CTX_STRU* pstFwCtx = mrsGetFwCtx();

    if (!pstFwCtx)
    {
        return HI_ERR_NO_INITILIZATION;
    }

    if (ulQueueId >= EN_APP_Q_MAX)
    {
        return HI_ERR_INVALID_ID;
    }

    return HI_MDM_MSG_Wait((HI_U32)pstFwCtx->stResCfg.aulQueueId[ulQueueId], pstMsg, ulTimeOut);
}

HI_U32 MRS_GetQueueMsgNum(HI_U32 ulQueueId)
{
    MRS_FW_CTX_STRU* pstFwCtx = mrsGetFwCtx();

    if (!pstFwCtx)
    {
        return HI_ERR_MSG_INVALID_MSG_NUM;
    }

    if (ulQueueId >= EN_APP_Q_MAX)
    {
        return HI_ERR_MSG_INVALID_MSG_NUM;
    }

    return HI_MDM_MSG_GetCurrentMsgNum((HI_U32)pstFwCtx->stResCfg.aulQueueId[ulQueueId]);
}

HI_U32 MRS_WaitSem(HI_U32 ulSemId, HI_U32 ulTimeOut)
{
    MRS_FW_CTX_STRU* pstFwCtx = mrsGetFwCtx();

    if (!pstFwCtx)
    {
        return HI_ERR_NO_INITILIZATION;
    }

    if (ulSemId >= EN_APP_SEM_MAX)
    {
        return HI_ERR_INVALID_ID;
    }

    return HI_MDM_SEM_Wait(pstFwCtx->stResCfg.aulSemId[ulSemId], ulTimeOut);
}

HI_U32 MRS_SignalSem(HI_U32 ulSemId)
{
    MRS_FW_CTX_STRU* pstFwCtx = mrsGetFwCtx();

    if (!pstFwCtx)
    {
        return HI_ERR_NO_INITILIZATION;
    }

    if (ulSemId >= EN_APP_SEM_MAX)
    {
        return HI_ERR_INVALID_ID;
    }

    return HI_MDM_SEM_Signal(pstFwCtx->stResCfg.aulSemId[ulSemId]);
}


#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
HI_U32 AppDiagChlConnectNotify (HI_U16 usPortNum, HI_BOOL bConnect)
{
    MRS_NOT_USED(usPortNum);

    if (HI_FALSE == bConnect)
    {
        MRS_StartTimer(MRS_CCO_TIMER_SIMU_CHL, 0, HI_SYS_TIMER_ONESHOT);
    }

    return HI_ERR_SUCCESS;
}
#endif


//*****************************************************************************
// ��������: APP_MrsTaskInit
// ��������: MRS�����ʼ��
//
// ����˵��:
//  This function has no arguments.
//
// �� �� ֵ:
//    ��
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: shenhankun/00130424 [2011-11-08]
//*****************************************************************************
HI_VOID APP_MrsTaskInit(HI_VOID)
{
    HI_U8 buf_index = 0;

#if (defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) || defined(PRODUCT_CFG_PRODUCT_TYPE_STA))
    // 376.2Э�� ��ʼ��
    if(HI_ERR_SUCCESS == mrs3762ProtoInit(&(GetFwCtx()->st3762Proto)))
    {
        HI_MDM_ReportInitEvt(HI_DEID_APP_376_2_PROTO_INIT_OK, HI_DIAG_SYS_INIT_SET);
    }
    else
    {
        HI_MDM_ReportInitEvt(HI_DEID_APP_376_2_PROTO_INIT_FAIL, HI_DIAG_SYS_INIT_SET);
    }
#endif

    for(buf_index = 0; buf_index < MRS_645BUF_MAX; buf_index++)
    {
        // 645Э�� ��ʼ��
        if(HI_ERR_SUCCESS == mrs645ProtoInit(&(GetFwCtx()->st645Proto[buf_index])))
        {
            mrs645SetContext((HI_U32)(&(GetFwCtx()->st645Proto[buf_index])), buf_index);

            HI_MDM_ReportInitEvt(HI_DEID_APP_645_PROTO_INIT_OK, HI_DIAG_SYS_INIT_SET);
        }
        else
        {
            HI_MDM_ReportInitEvt(HI_DEID_APP_645_PROTO_INIT_FAIL, HI_DIAG_SYS_INIT_SET);
        }
    }

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    mrsCcoGlobalInit();
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
    // ͨ���ı�֪ͨ�����ʼ��
    mrsSrvChlNotifyProcInit();

    mrsStaGlobalInit();
    mrsNvInitBaudRateCfg(ID_NV_APP_BAUDRATE_CFG);
#endif

    // MR��������
    mrsSrvStart();

#if (defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) || defined(PRODUCT_CFG_PRODUCT_TYPE_STA))
    mrsInitIOCtx(); //IO����ģ���ʼ��
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    HI_MDM_DiagRegisterConnect(AppDiagChlConnectNotify);
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) || defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
    mrsSrvCaptureInit();
#endif
#endif
}

//*****************************************************************************
// ��������: mrsSetPlcForceMode
// ��������: MRSǿ��PLCͨ������ģʽ������
//
// ����˵��:
//  ��
//
// �� �� ֵ:
//    ��
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: fengxiaomin/00209182 [2014-1-14]
//*****************************************************************************
/* BEGIN: Added by fengxiaomin/00209182, 2014/1/14   ���ⵥ�ź�:DTS2014012607684  */
HI_VOID mrsSetPlcForceMode(HI_VOID)
{
    // ǿ��PLC����
    HI_MDM_ChlCtrl(HI_DMS_CHL_MAC_PORT_APP, HI_DMS_PORT_CTRL_OFFLINE_ONLINE_RX, HI_NULL, HI_NULL);
}


//*****************************************************************************
// ��������: mrsPowerOnPro
// ��������: MRSģ���ϵ紦����
//
// ����˵��:
//  pstFwCtx
//
// �� �� ֵ:
//    ��
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: fengxiaomin/00209182 [2014-1-14]
//*****************************************************************************
HI_VOID mrsPowerOnPro(HI_VOID)
{
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    mrsCcoPowerOnPro();
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
    mrsStaPowerOnPro();
#endif

    mrsSetPlcForceMode();

    return;
}
/* END:   Added by fengxiaomin/00209182, 2014/1/14 */

//*****************************************************************************
// ��������: APP_MrsTaskBody
// ��������: MRS ��������
//
// ����˵��:
//  This function has no arguments.
//
// �� �� ֵ:
//    ��
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: shenhankun/00130424 [2011-11-08]
//*****************************************************************************
HI_VOID APP_MrsTaskBody(HI_U32 ulParam)
{
    MRS_NOT_USED(ulParam);
    HI_MDM_ReportInitEvt(HI_DEID_APP_START_MRS_OK, HI_DIAG_SYS_INIT_SET);

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    mrsSrvArchivesLoad();
	mrsDetectWaitTopoInit();//��������ϵ��flash�ж�ȡ����֮��,��mrsSrvArchivesLoad����
#endif

    mrsPowerOnPro();

    for (;;)
    {
        HI_U32 ret                = HI_ERR_SUCCESS;
        HI_SYS_QUEUE_MSG_S stMsg = { 0 };

        ret = MRS_WaitMessage((HI_U32)EN_APP_QUEUE_MRS, &stMsg, HI_SYS_WAIT_FOREVER);
        if (HI_ERR_SUCCESS == ret)
        {
            HI_U32 start_time = 0;

            mrsDfxRefreshMsgQueueNum(); // ˢ�µ�ǰ�������

            start_time = HI_MDM_GetMilliSeconds();
            (HI_VOID)mrsMsgProc(&stMsg);
			mrsDfxMsgPrcoTimeConsuming(start_time, stMsg.ulMsgId); // ˢ����Ϣ����ʱ��
			mrsDfxRefreshMsgQueueNum(); // ˢ�µ�ǰ�������
        }
        else
        {
            HI_MDM_Sleep(HI_DMS_CHL_EXCEPTION_POLLING_WAIT);
        }
    }
}

// ������Ϣ�����ж�
// ����HI_TRUE:  �������
// ����HI_FALSE: �ܾ�����
HI_BOOL mrsAllowJoinMsgQueue(HI_U32 ulMsgId)
{
    if(ulMsgId != ID_MRS_MSG_MR_TIMER)
    {
        HI_U32 q_num = 0;

        q_num = MRS_GetQueueMsgNum(EN_APP_QUEUE_MRS);
        if(q_num < MRS_MSG_BASE_MAX_NUM)
        {
            return HI_TRUE;
        }
        else
        {
            return HI_FALSE;
        }
    }

    return HI_TRUE;
}


// ������Ϣ�� MRS ����
HI_U32 mrsSendMessage2Queue(HI_IN HI_SYS_QUEUE_MSG_S* pstMsg)
{
    HI_U32 ret = HI_ERR_SUCCESS;

    mrsDfxRefreshMsgQueueNum();         // ˢ�µ�ǰ�������
    mrsDfxMsgTxCnt(pstMsg->ulMsgId);    // ��Ϣ����ͳ��

    if(mrsAllowJoinMsgQueue(pstMsg->ulMsgId))
    {
        ret = MRS_SendMessage((HI_U32)EN_APP_QUEUE_MRS, pstMsg, 0);
    }
    else
    {
        ret = HI_ERR_QUEUE_FULL;
    }

    if (ret)
    {
        mrsDfxMsgTxFailCnt(pstMsg->ulMsgId);    // ��Ϣ����ʧ��ͳ��
        HI_MDM_ReportSysEvent(EID_APP_SEND_MSG_FAIL);
    }

    mrsDfxRefreshMsgQueueNum();    // ˢ�µ�ǰ�������

    return ret;
}

//*****************************************************************************
// ��������: mrsChlConnect
// ��������: PLCͨ������
//
// ����˵��:
//   usPort [in] PLCͨ���˿ں�
//   ucOption [in] ���ӺͶϿ���ȡֵΪ HI_DMS_CHL_CONNECT ���� HI_DMS_CHL_DISCONNECT
//
// �� �� ֵ:
//    TODO: ...
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: liuxipeng/lKF54842 [2013-11-05]
//*****************************************************************************
HI_PRV HI_U32 mrsChlConnect(HI_U16 usPort, HI_U8 ucOption)
{
    HI_U32 ret = HI_ERR_SUCCESS;

    if (HI_DMS_CHL_MAC_PORT_APP == usPort)
    {
        HI_SYS_QUEUE_MSG_S stMsg = { 0 };
        HI_U32 timestamp = HI_MDM_GetMilliSeconds();

        if (ucOption == HI_DMS_CHL_CONNECT)
        {
            mrsDfxChlChangeTrue(timestamp);
        }
        else
        {
            mrsDfxChlChangeFalse(timestamp);
        }

        stMsg.ulMsgId = ID_MRS_MSG_PLC_CHL_STATUS;
        stMsg.ulParam[0] = (HI_U32)usPort;
        stMsg.ulParam[1] = (HI_U32)ucOption;

        ret = mrsSendMessage2Queue(&stMsg);
        if(ret != HI_ERR_SUCCESS)
        {
            mrsDfxChlChangeMsgFail(ret);
        }
    }

    return ret;
}

// CCO����MR���ݴ���
#if (defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) || defined(PRODUCT_CFG_PRODUCT_TYPE_STA))
HI_U32 DoCcoMrRxProcess(HI_IN HI_DMS_CHL_RX_S* pstRx, HI_U16 usFrameType)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    MRS_ONE_RAW_FRAME_STRU* pstFrame = HI_NULL;

    mrs3762ProtoStreamInputCheck((HI_BOOL)pstRx->bCanDiscardOldData);
    mrs3762ProtoStream2Cache(pstRx->pPayload, pstRx->usPayloadLen);

    // ��ȡ��������֡����
    ret = mrs3762ProtoBuffer(&pstFrame, usFrameType);
    if(HI_ERR_SUCCESS == ret)
    {
        HI_SYS_QUEUE_MSG_S stMsg = { 0 };

        if (pstFrame)
        {
            HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1000, HI_DIAG_MT("[CCO-MR]-RX"), pstFrame->pucDatagram, pstFrame->usDatagramSize);

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
            MRS_BufLog("[CCO-COM][  Rx]: ", pstFrame->pucDatagram, pstFrame->usDatagramSize);
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
            MRS_Printf("[%d]", g_ulMrsBaudRate);
            MRS_BufLog("[STA-COM][  Rx]: ", pstFrame->pucDatagram, pstFrame->usDatagramSize);
#endif

            mrsDfxChlUartOkFrameCnt(pstFrame->usDatagramSize, HI_FALSE);    // ���ڽ�������֡ͳ��(��Ϣ����)
        }

        stMsg.ulMsgId = (usFrameType != MRS_CCO_FRAME_TYPE_3762) ? EN_MRS_FW_MSG_CSG_FRAME_INPUT : EN_MRS_FW_MSG_376_2_FRAME_INPUT;
        stMsg.ulParam[0] = (HI_U32)pstFrame;

        // ������Ϣ�� MRS����, ������Ϣ�ַ�����
        ret = mrsSendMessage2Queue(&stMsg);
        if(HI_ERR_SUCCESS != ret)
        {
            mrsDfxChlUartErrFrameCnt(HI_FALSE);   // ���ڽ�������֡����ͳ��(��Ϣ����)
            HI_MDM_ReportSysEvent(EID_APP_SEND_MSG_FAIL);

            mrsToolsFree(pstFrame);
        }
    }
    else if(HI_ERR_CONTINUE == ret)
    {
        // �������ݻ�ۺ�, �����ܺͲ���1֡����, ��Ҫ�����ȴ�����
    }
    else
    {
        mrsDfxChlUartErrRx(pstRx->usPayloadLen, ret, HI_FALSE); // ���ڽ��մ�����(��Ϣ����)
    }

    return ret;
}
#endif


#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
HI_U32 DoCcoMrRxCsg(HI_IN HI_DMS_CHL_RX_S* pstRx)
{
    return DoCcoMrRxProcess(pstRx, MRS_CCO_FRAME_TYPE_CSG);
}
#else
HI_U32 DoCcoMrRx3762(HI_IN HI_DMS_CHL_RX_S * pstRx)
{
    return DoCcoMrRxProcess(pstRx, MRS_CCO_FRAME_TYPE_3762);
}
#endif


#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO_BROAD_BAND_UNIT)
HI_U32 DoCcoMrRxProcessBbu(HI_IN HI_DMS_CHL_RX_S* pstRx)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    MRS_ONE_RAW_FRAME_STRU stFrame = {0};

    mrs645ProtoStreamInputCheck((HI_BOOL)pstRx->bCanDiscardOldData, ID_MRS_UART_645BUF);


    // ��ȡ��������֡����
    ret = mrs645ProtoStreamInput(pstRx->pPayload, pstRx->usPayloadLen, &stFrame, ID_MRS_UART_645BUF);

    if(HI_ERR_SUCCESS == ret)
    {
        HI_SYS_QUEUE_MSG_S stMsg = { 0 };
        MRS_ONE_RAW_FRAME_STRU* pstFrame = HI_NULL;

        if (stFrame.pucDatagram)
        {
            mrsDfxChlUartOkFrameCnt(stFrame.usDatagramSize, HI_FALSE);    // ���ڽ�������֡ͳ��(��Ϣ����)
        }

        // �жϽ��յ�645֡�Ƿ���(����֡��ͬ�����߱��ַΪ��BCD�붪���������buffer)
        if (HI_ERR_SUCCESS == mrsIsSameAsBufferBbu(stFrame.usDatagramSize, stFrame.pucDatagram, ID_MRS_BBUDA_645BUF))
        {
            // ��ͬ������
            mrsToolsFree(stFrame.pucDatagram);

            return HI_ERR_SUCCESS;
        }
        else if (!mrsToolsCheckBCD(stFrame.pucDatagram + 1, HI_METER_ADDR_LEN))
        {
            // ��ͬ������
            mrsToolsFree(stFrame.pucDatagram);
            mrsClear645ProtoBufferBbu(ID_MRS_BBUDA_645BUF);

            return HI_ERR_SUCCESS;
        }
        else if(mrsToolsCheckBroadCast(stFrame.pucDatagram, stFrame.usDatagramSize))
        {
            // ��buffer���ұ��Ĳ���Ҫ��ӵ�buffer��
            mrsClear645ProtoBufferBbu(ID_MRS_BBUDA_645BUF);
        }
        else
        {
            // ��ӵ�buffer��
            ret = mrsAddFrameToBufferBbu(stFrame.usDatagramSize, stFrame.pucDatagram, ID_MRS_BBUDA_645BUF);
            if (HI_ERR_SUCCESS != ret)
            {
                return ret;
            }
        }

        // ��645֡תΪ3762͸������֡
        ret = mrsCreateFrame3762Bbu(stFrame.usDatagramSize, stFrame.pucDatagram, &pstFrame);

        mrsToolsFree(stFrame.pucDatagram);

        if(HI_ERR_SUCCESS == ret)
        {
            stMsg.ulParam[0] = (HI_U32)pstFrame;
            stMsg.ulMsgId = EN_MRS_FW_MSG_376_2_FRAME_INPUT;

            // ������Ϣ�� MRS����, ������Ϣ�ַ�����
            ret = mrsSendMessage2Queue(&stMsg);
            if(HI_ERR_SUCCESS != ret)
            {
                mrsDfxChlUartErrFrameCnt(HI_FALSE);   // ���ڽ�������֡����ͳ��(��Ϣ����)
                HI_MDM_ReportSysEvent(EID_APP_SEND_MSG_FAIL);
                mrsToolsFree(pstFrame);
            }
            mrsInitMrStateBbu();
        }
    }
    else if(HI_ERR_CONTINUE == ret)
    {
        // �������ݻ�ۺ�, �����ܺͲ���1֡����, ��Ҫ�����ȴ�����
    }
    else
    {
        mrsDfxChlUartErrRx(pstRx->usPayloadLen, ret, HI_FALSE); // ���ڽ��մ�����(��Ϣ����)
    }

    return ret;
}
#endif

#endif

// STA����MR���ݴ���
#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
HI_U32 DoStaMrRxProcess(HI_IN HI_DMS_CHL_RX_S* pstRx)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    MRS_ONE_RAW_FRAME_STRU stFrame = {0};

    if (mrsToolsIsIICollector())
    {
        ret = mrsCltiiSearchRx(pstRx);
        if (HI_ERR_SUCCESS == ret)
        {
            return ret;
        }
    }

    /* Update meter-reading queue status when received data from UART. CNcomment: �յ���������֮����³������״̬. CNend */
    mrsStaUpdateQueueStatus();

    stFrame.bCanDiscardOldData = (HI_BOOL)pstRx->bCanDiscardOldData;

    mrs645ProtoStreamInputCheck((HI_BOOL)pstRx->bCanDiscardOldData, ID_MRS_UART_645BUF);

    // ��ȡ��������֡����
    ret = mrs645ProtoStreamInput(pstRx->pPayload, pstRx->usPayloadLen, &stFrame, ID_MRS_UART_645BUF);
    if(HI_ERR_SUCCESS == ret)
    {
        if (mrsToolsIsIICollector() && (HI_ERR_SUCCESS == mrsJudgeEqiupMode(stFrame.pucDatagram, stFrame.usDatagramSize)))
        {
            return ret;
        }

        if (EQUIP_DUT_IS_EQUIP_MODE())//����ǲ���ģʽ
        {
            mrsDutRxProcess(&stFrame);
            mrsToolsFree(stFrame.pucDatagram);
        }
        else
        {
            HI_SYS_QUEUE_MSG_S stMsg = { 0 };
            MRS_ONE_RAW_FRAME_STRU* pstFrame = (MRS_ONE_RAW_FRAME_STRU*)&stMsg.ulParam[0];

            HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1012, HI_DIAG_MT("[STA-MR]-RX"),stFrame.pucDatagram, stFrame.usDatagramSize);
            MRS_Printf("[%d]", g_ulMrsBaudRate);
            MRS_BufLog("[STA-COM][  Rx]: ", stFrame.pucDatagram, stFrame.usDatagramSize);

            mrsDfxChlUartOkFrameCnt(stFrame.usDatagramSize, HI_FALSE);    // ���ڽ�������֡ͳ��(��Ϣ����)

            (hi_void)memcpy_s(pstFrame, sizeof(MRS_ONE_RAW_FRAME_STRU), &stFrame, sizeof(MRS_ONE_RAW_FRAME_STRU));
            stMsg.ulMsgId = EN_MRS_FW_MSG_645_FRAME_INPUT;

            // ������Ϣ�� MRS����, ������Ϣ�ַ�����
            ret = mrsSendMessage2Queue(&stMsg);
            if (HI_ERR_SUCCESS != ret)
            {
                mrsToolsFree(stFrame.pucDatagram);

                mrsDfxChlUartErrFrameCnt(HI_FALSE);   // ���ڽ�������֡����ͳ��(��Ϣ����)
                HI_MDM_ReportSysEvent(EID_APP_SEND_MSG_FAIL);
            }
        }
    }
    else if(HI_ERR_CONTINUE == ret)
    {
        // �������ݻ�ۺ�, �����ܺͲ���1֡����, ��Ҫ�����ȴ�����
    }
    else
    {
        mrsDfxChlUartErrRx(pstRx->usPayloadLen, ret, HI_FALSE); // ���ڽ��մ�����(��Ϣ����)
    }

    return ret;
}

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
HI_U32 DoCltIMrRxProcess(HI_IN HI_DMS_CHL_RX_S* pstRx)
{
    /* Update meter-reading queue status when received data from UART. CNcomment: �յ���������֮����³������״̬. CNend */
    mrsStaUpdateQueueStatus();

    return DoCcoMrRxProcess(pstRx, MRS_CCO_FRAME_TYPE_CSG);
}
#endif

#endif

// ����������MR���ݴ���
#if defined(PRODUCT_CFG_PRODUCT_TYPE_NDM)
HI_U32 DoNdmMrRxProcess(HI_IN HI_DMS_CHL_RX_S* pstRx )
{
    HI_U32 ret = HI_ERR_SUCCESS;
    MRS_ONE_RAW_FRAME_STRU stFrame = {0};

    /* BEGIN: Added by liupeidong/00238134, 2014/12/10 ���ⵥ��:DTS2014121005392 */
    if (HI_MDM_IsNdmMeterAccess())
    {
        HI_MDM_NdmSerialInterfaceRxLedProc();
    }
    /* END: Added by liupeidong/00238134, 2014/12/10 ���ⵥ��:DTS2014121005392 */

    stFrame.bCanDiscardOldData = (HI_BOOL)pstRx->bCanDiscardOldData;

    mrs645ProtoStreamInputCheck((HI_BOOL)pstRx->bCanDiscardOldData, ID_MRS_UART_645BUF);

    // ��ȡ��������֡����
    ret = mrs645ProtoStreamInput(pstRx->pPayload, pstRx->usPayloadLen, &stFrame, ID_MRS_UART_645BUF);
    if(HI_ERR_SUCCESS == ret)
    {
        MRS_645_FRAME_STRU st645Hrd = { 0 };
        HI_NDM_645_FRAME_S * buf_645 = HI_NULL;
        HI_U8 MacAddr[HI_PLC_RELAY_NUM_MAX][HI_PLC_MAC_ADDR_LEN];

        HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1011, HI_DIAG_MT("[CCD-MR]-RX"), stFrame.pucDatagram, stFrame.usDatagramSize);

        (hi_void)memset_s(MacAddr, sizeof(MacAddr), 0, sizeof(MacAddr));

        mrsDfxChlUartOkFrameCnt(stFrame.usDatagramSize, HI_FALSE);    // ���ڽ�������֡ͳ��(��Ϣ����)

        // ����645֡
        ret = MRS_Proto645Dec(stFrame.pucDatagram,stFrame.usDatagramSize,&st645Hrd);
        if(HI_ERR_SUCCESS == ret)
        {
            if (mrsIsCcdDutMode(&st645Hrd))//�����Ⳮ��������1:8��װ
            {
                ret = mrsCcdDutSend(stFrame.pucDatagram, stFrame.usDatagramSize, st645Hrd.ucDataRealm);
                mrsToolsFree(stFrame.pucDatagram);
                return ret;
            }

            (hi_void)memset_s(MacAddr[0], HI_PLC_MAC_ADDR_LEN, 0xFF, HI_PLC_MAC_ADDR_LEN);// ʹ�ù㲥��ַ

            if((HI_MDM_IsNdmMeterAccess()==HI_TRUE)
                &&(mrsToolsNormalAddr(st645Hrd.ucAddr)))
            {
                (hi_void)memcpy_s(MacAddr[0], HI_PLC_MAC_ADDR_LEN, st645Hrd.ucAddr, HI_PLC_MAC_ADDR_LEN);
                mrsHexInvert(MacAddr[0], HI_PLC_MAC_ADDR_LEN);
                (hi_void)memcpy_s(MacAddr[1], HI_PLC_MAC_ADDR_LEN, MacAddr[0], HI_PLC_MAC_ADDR_LEN);
            }

            buf_645 = (HI_NDM_645_FRAME_S *)mrsToolsMalloc(sizeof(HI_NDM_645_FRAME_S));
            if(buf_645)
            {
                (hi_void)memset_s(buf_645, sizeof(HI_NDM_645_FRAME_S), 0, sizeof(HI_NDM_645_FRAME_S));
            }
            else
            {
                mrsToolsFree(stFrame.pucDatagram);
                return HI_ERR_FAILURE;
            }

            HI_ASSERT(stFrame.usDatagramSize < sizeof(stFrame.pucDatagram));

            (hi_void)memcpy_s(buf_645->aucFrame, sizeof(buf_645->aucFrame), stFrame.pucDatagram, stFrame.usDatagramSize);
            buf_645->usFrameSize = stFrame.usDatagramSize;

            // ����645����Diag��
            ret = HI_MDM_SendNdmReqFrame(HI_NDM_645_FRAME,(HI_U8 *)buf_645,sizeof(HI_NDM_645_FRAME_S),MacAddr,0);
            if(ret != HI_ERR_SUCCESS)
            {
                mrsDfxChlUartErrFrameCnt(HI_FALSE);   // ���ڽ�������֡����ͳ��(��Ϣ����)
            }

            mrsToolsFree(buf_645);
        }
    }
    else if(HI_ERR_CONTINUE == ret)
    {
        // �������ݻ�ۺ�, �����ܺͲ���1֡����, ��Ҫ�����ȴ�����
    }
    else
    {
        mrsDfxChlUartErrRx(pstRx->usPayloadLen, ret, HI_FALSE); // ���ڽ��մ�����(��Ϣ����)
    }

    mrsToolsFree(stFrame.pucDatagram);

    return ret;
}

HI_BOOL mrsIsCcdDutMode(MRS_645_FRAME_STRU* pst645Hrd)
{
    HI_U8 aucDutAddr[] = DUT_NORMAL_MODE_ADDR;

    mrsHexInvert(aucDutAddr, sizeof(aucDutAddr));
    if (!memcmp(pst645Hrd->ucAddr, aucDutAddr, HI_METER_ADDR_LEN)
        && (DUT_NORMAL_MODE_CTRL == pst645Hrd->stCtrl.ucFn))
    {
        return HI_TRUE;
    }

    return HI_FALSE;
}

HI_U32 mrsCcdDutSend(HI_PBYTE pucDatagram, HI_U16 usDatagramSize, HI_U8* pDataRealm)
{
    HI_MDM_MSG_CTRL_S stCtrl;

    (hi_void)memset_s(&stCtrl, sizeof(stCtrl), 0, sizeof(stCtrl));

    (hi_void)memcpy_s(stCtrl.aucMacAddr[0], HI_PLC_MAC_ADDR_LEN, pDataRealm+ MRS_645_FRAME_DATA_DI_SIZE, HI_PLC_MAC_ADDR_LEN);
    mrs645DataDecode(stCtrl.aucMacAddr[0], HI_PLC_MAC_ADDR_LEN);
    mrsHexInvert(stCtrl.aucMacAddr[0], HI_PLC_MAC_ADDR_LEN);

    if (!mrsToolsBroadCastMac(stCtrl.aucMacAddr[0]))
    {
        (hi_void)memcpy_s(stCtrl.aucMacAddr[1], HI_PLC_MAC_ADDR_LEN, stCtrl.aucMacAddr[0], HI_PLC_MAC_ADDR_LEN);
    }

    HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1010, HI_DIAG_MT("plc mac0"), stCtrl.aucMacAddr[0], 6);
    HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1008, HI_DIAG_MT("plc mac1"), stCtrl.aucMacAddr[1], 6);
    HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1009, HI_DIAG_MT("dut send"), pucDatagram, usDatagramSize);

    stCtrl.usDULink = HI_FALSE;

    return HI_MDM_SendNdmPing(HI_MDM_NDM_PING_ID_0, pucDatagram, usDatagramSize, &stCtrl);
}

#endif

/*lint -e527*/
HI_U32 DoMrRxProcess(HI_IN HI_DMS_CHL_RX_S *pstRx)
{
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO_BROAD_BAND_UNIT)
    if (mrsToolIsBbu() && (!mrsIsSimuChlOpened()))
    {
        return DoCcoMrRxProcessBbu(pstRx);
    }
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
    return DoCcoMrRxCsg(pstRx);
#else
    return DoCcoMrRx3762(pstRx);
#endif
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
    if (mrsToolsIsCLTI()
        && !EQUIP_DUT_IS_EQUIP_MODE()
        && !mrsStaGetContext()->ucDutFlag
        && !mrsStaGetSupportVM())
    {
        return DoCltIMrRxProcess(pstRx);
    }
#endif
    return DoStaMrRxProcess(pstRx);
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_NDM)
    return DoNdmMrRxProcess(pstRx);
#endif
}
/*lint +e527*/


//*****************************************************************************
// ��������: mrsChlRx
// ��������: ͨ������
//
// ����˵��:
//   usPort [in] PLCͨ���˿ں�
//   pstRx [in] ��������
//
// �� �� ֵ:
//    TODO: ...
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: shenhankun/00130424 [2011-11-09]
//*****************************************************************************
HI_PRV HI_U32 mrsChlRx(HI_U16 usPort, HI_IN HI_DMS_CHL_RX_S* pstRx)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_DMS_CHL_RX_S* pstDispRx = HI_NULL;
    HI_SYS_QUEUE_MSG_S stMsg = { 0 };

    // ��ȡ����֡
    if (HI_DMS_CHL_UART_PORT_APP == usPort)
    {
        mrsDfxChlUartRx(pstRx->pPayload, pstRx->usPayloadLen, HI_FALSE);    // ���ڽ���ͳ��(��Ϣ����)
        DoMrRxProcess(pstRx);
    }
    else if (HI_DMS_CHL_MAC_PORT_APP == usPort)
    {
        HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1003, HI_DIAG_MT("PLC_RX MAC"), pstRx->ucSrcMac, 6);

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
        MRS_BufLog("[CCO-PLC][  Rx]: ", pstRx->pPayload, pstRx->usPayloadLen);
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
        MRS_BufLog("[STA-PLC][  Rx]: ", pstRx->pPayload, pstRx->usPayloadLen);
#endif

        mrsDfxChlPlcRx(pstRx, HI_FALSE);    // PLCͨ������ͳ��(��Ϣ����)

        //
        // �����첽��ʽ,ģ�����Զ�����,���⽻��, ���㹦���ݽ�, ����ջ��С���ö���
        // �����ڴ涼һ�η���, ��������Ǵ�MAC�����������ڴ�, MAC���ͷ�, �������Ż�.
        //
        pstDispRx = (HI_DMS_CHL_RX_S*)mrsToolsMalloc(sizeof(HI_DMS_CHL_RX_S) + pstRx->usPayloadLen);
        if(HI_NULL == pstDispRx)
        {
            return HI_ERR_NO_MORE_MEMORY;
        }

        stMsg.ulMsgId     = EN_MRS_FW_MSG_MAC_FRAME_INPUT;
        stMsg.ulParam[0] = (HI_U32)pstDispRx;
        stMsg.ulParam[1] = 0;

        (hi_void)memcpy_s(pstDispRx, sizeof(HI_DMS_CHL_RX_S), pstRx, sizeof(HI_DMS_CHL_RX_S));
        pstDispRx->pPayload = (HI_PBYTE)pstDispRx + sizeof(HI_DMS_CHL_RX_S);
        (hi_void)memcpy_s(pstDispRx->pPayload, pstDispRx->usPayloadLen, pstRx->pPayload, pstRx->usPayloadLen);

        // ������Ϣ�� MRS����, ������Ϣ�ַ�����
        ret = mrsSendMessage2Queue(&stMsg);
        if (HI_ERR_SUCCESS != ret)
        {
            mrsDfxChlPlcErrRx(pstRx, ret, HI_FALSE);    // PLCͨ�����մ���ͳ��(��Ϣ����)
            HI_MDM_ReportSysEvent(EID_APP_SEND_MSG_FAIL);

            mrsToolsFree(pstDispRx);
        }
    }
#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
    else if (HI_DMS_CHL_UART_PORT_IR == usPort)
    {
        if (mrsToolsIsIICollector())
        {
            mrsCollectorIrDARx(pstRx);
        }
    }
#endif
    
    return ret;
}

//*****************************************************************************
// ��������: mrsChlMsgProcFromMac
// ��������: ��������Mac���ͨ����Ϣ
//
// ����˵��:
//   ulMsgId [in] ��ϢID
//   pMsgParam [in] ��Ϣ����
//
// �� �� ֵ:
//    �μ�����ֵ�б�
//
// ����Ҫ��:
// ���þ���:
// ��    ��: liuxipeng/lkf54842 [2012-11-13]
//*****************************************************************************
/* BEGIN: PN: DTS2015061008265 MODIFY\ADD\DEL by cuiate/00233580 at 2015/6/24 */
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
HI_PRV HI_U32 mrsChlMsgProcFromMac(HI_U32 ulMsgId, HI_PVOID pMsgParam)
{
    HI_SYS_QUEUE_MSG_S stMsg = { 0 };

    (hi_void)memset_s(&stMsg, sizeof(stMsg), 0, sizeof(stMsg));
    stMsg.ulMsgId = EN_MRS_FW_MSG_MAC_MSG;
    stMsg.ulParam[0] = ulMsgId;

    if (HI_CHL_MSG_TF_START_SEARCH_METER == ulMsgId)
    {
        HI_MDM_TF_START_SEARCH_METER_S *pStart = (HI_MDM_TF_START_SEARCH_METER_S *)pMsgParam;
        stMsg.ulParam[1] = (HI_U32)pStart->time;
    }

    return mrsSendMessage2Queue(&stMsg);
}


HI_PRVL HI_U32 mrsMsgProcMacMsg(HI_IN HI_SYS_QUEUE_MSG_S* pstMsg)
{
    switch(pstMsg->ulParam[0])
    {
    case HI_CHL_MSG_SYNC_WHITE_LIST:
        return mrsSyncMeterInfToWhiteList(HI_MAC_WHIST_LIST_CHANGE_REASON_MAC_REQ);

    case HI_CHL_MSG_TF_START_SEARCH_METER:
        return mrsCcoStartSearchMeterTransformer(pstMsg->ulParam[1]);

    case HI_CHL_MSG_TF_STOP_SEARCH_METER:
        return mrsCcoStopSearchMeterTransformer();

    default:
        break;
    }

    return HI_ERR_SUCCESS;
}
#endif
/* END:   PN: DTS2015061008265 MODIFY\ADD\DEL by cuiate/00233580 at 2015/6/24 */


//*****************************************************************************
// ��������: mrsChlMsgProc
// ��������: ͨ����Ϣ����
//
// ����˵��:
//   usPort [in] ͨ���˿ں�(ͨ������)
//   ulMsgId [in] ��ϢID
//   pMsgParam [in] ��Ϣ����
//
// �� �� ֵ:
//    �μ�����ֵ�б�
//
// ����Ҫ��:
// ���þ���:
// ��    ��: liuxipeng/lkf54842 [2012-11-13]
//*****************************************************************************
HI_PRV HI_U32 mrsChlMsgProc(HI_U16 usPort, HI_U32 ulMsgId, HI_PVOID pMsgParam)
{
    MRS_NOT_USED(usPort);
    MRS_NOT_USED(ulMsgId);
    MRS_NOT_USED(pMsgParam);

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    if (HI_DMS_CHL_MAC_PORT_APP == usPort)
    {
        return mrsChlMsgProcFromMac(ulMsgId, pMsgParam);
    }
#endif

    return HI_ERR_SUCCESS;
}


//*****************************************************************************
// ͨ���˿ڶ�������
//*****************************************************************************
HI_PRV HI_CONST HI_DMS_CHL_PORT_OBJ_S g_stMrsChlPort = { mrsChlConnect, mrsChlRx, HI_NULL, mrsChlMsgProc};

//��ȡͨ���˿�
HI_DMS_CHL_PORT_OBJ_S mrsGetChlPort(HI_VOID)
{
    return g_stMrsChlPort;
}

/* BEGIN: Added by liuxipeng/lkf54842 2014/01/15 ���ⵥ��: DTS2013121904065 */
//*****************************************************************************
// ��������: mrsFwChlInit
// ��������: MRS���ͨ����ʼ��
//           ��ɸ���ͨ���ĳ�ʼ����װ���ص�����ע�Ṧ��
//           ��ʶ��ΪUART�˿ڲ���ΪII�Ͳɼ���ʱ��UARTģʽ����Ϊ485
//           ��ʶ��ΪIR�˿ں�ģʽ����ΪIR
// ����˵��:
//          pstChlPort : ͨ���ص�����ʵ��
//          usPortNum  : ͨ���˿ں�
// �� �� ֵ:
//    HI_ERR_SUCCESS : �ɹ�
//    ����ֵΪ������
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: liuxipeng/lkf54842 [2014-01-15]
//*****************************************************************************
HI_U32 mrsFwChlInit(HI_CONST HI_DMS_CHL_PORT_OBJ_S * pstChlPort, HI_U16 usPortNum)
{
    HI_U32 ret = HI_ERR_SUCCESS;

    ret = HI_MDM_ChlSetup(pstChlPort, usPortNum);
    if(ret != HI_ERR_SUCCESS)
    {
        return ret;
    }

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
    if(usPortNum == HI_DMS_CHL_UART_PORT_APP)
    {
        if(mrsToolsIsIICollector())
        {
            HI_MDM_ChlCtrl(usPortNum, HI_DMS_PORT_CTRL_UART_MODE_485, HI_NULL, HI_NULL);
        }
    }
    else if(usPortNum == HI_DMS_CHL_UART_PORT_IR)
    {
        HI_MDM_ChlCtrl(usPortNum, HI_DMS_PORT_CTRL_UART_MODE_IR, HI_NULL, HI_NULL);
    }
#endif

    return ret;
}
/* END  : Added by liuxipeng/lkf54842 2014/01/15 */

//*****************************************************************************
// ��������: mrsFwInit
// ��������: MRS��ܳ�ʼ��
//
// ����˵��:
//  This function has no arguments.
//
// �� �� ֵ:
//    HI_ERR_SUCCESS : �ɹ�
//    ����ֵΪ������
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: shenhankun/00130424 [2011-11-09]
//*****************************************************************************
HI_PRV HI_U32 mrsFwInit(HI_VOID)
{
    HI_U32 ret = HI_ERR_SUCCESS;

    if (!g_pstMrsFwCtx)
    {
        g_pstMrsFwCtx = (MRS_FW_CTX_STRU *)mrsToolsMalloc(sizeof(MRS_FW_CTX_STRU));
    }

    if(!g_pstMrsFwCtx)
    {
        return HI_ERR_FAILURE;
    }

    (hi_void)memset_s(g_pstMrsFwCtx, sizeof(MRS_FW_CTX_STRU), 0, sizeof(MRS_FW_CTX_STRU));

	mrsNvUserVerCfg(ID_NV_MRS_USER_VER);

    ret = mrsFwChlInit(&g_stMrsChlPort, HI_DMS_CHL_MAC_PORT_APP);
#ifndef PRODUCT_CFG_SUPPORT_CCT3911
    ret |= mrsFwChlInit(&g_stMrsChlPort, HI_DMS_CHL_UART_PORT_APP);
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
    if (mrsToolsIsIICollector())
    {
        ret |= mrsFwChlInit(&g_stMrsChlPort, HI_DMS_CHL_UART_PORT_IR);
    }
#endif

    mrsFwEnableUartDbg(0);

    return ret;
}

HI_VOID mrsFwSetUserVer(NV_MRS_USER_VER_S* pstVer)
{
	if (mrsGetFwCtx())
	{
	    (hi_void)memcpy_s(mrsGetFwCtx()->aucUserVersion, sizeof(mrsGetFwCtx()->aucUserVersion), pstVer->aucUserVersion, HI_USER_VER_LENGTH);
	}
}

HI_VOID mrsFwGetUserVer(HI_CHAR* pucVer)
{
	if (mrsGetFwCtx())
	{
	    (hi_void)memcpy_s(pucVer, HI_USER_VER_LENGTH, mrsGetFwCtx()->aucUserVersion, HI_USER_VER_LENGTH);
	}
}

//*****************************************************************************
// ��������: MRS_CmdRegister
// ��������: ����ע��
//
// ����˵��:
//   pstTbl [in] �������ڱ�
//   usNum [in] �������ڸ���
//
// �� �� ֵ:
//    TODO: ...
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: shenhankun/00130424 [2011-11-04]
//*****************************************************************************
HI_U32 MRS_CmdRegister(HI_CONST MRS_FW_CMD_PROC_TBL_STRU* pstTbl, HI_U16 usNum)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    MRS_FW_CTX_STRU* pstFwCtx = GetFwCtx();

    pstFwCtx->pCmdList = pstTbl;
    pstFwCtx->usCmdNum = (HI_U16) usNum;

    return ret;
}


//*****************************************************************************
// ��������: MRS_MsgRegister
// ��������: ��Ϣע��
//
// ����˵��:
//   pstTbl [in] ��Ϣ������ڱ�
//   usNum [in] ��Ϣ������ڸ���
//
// �� �� ֵ:
//    TODO: ...
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: shenhankun/00130424 [2011-11-04]
//*****************************************************************************
HI_U32 MRS_MsgRegister(HI_CONST MRS_FW_MSG_PROC_TBL_STRU* pstTbl, HI_U16 usNum)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    MRS_FW_CTX_STRU* pstFwCtx = GetFwCtx();

    pstFwCtx->pMsgList = pstTbl;
    pstFwCtx->usMsgNum = (HI_U16) usNum;

    return ret;
}

//*****************************************************************************
// ��������: mrsChlConnectMsgProc
// ��������: ͨ�������Ϣ����
//
// ����˵��:
//   usPort [in] ͨ������
//   ucOption [in] ѡ���� ����/�Ͽ�
//
// �� �� ֵ:
//    HI_ERR_SUCCESS   : ����ɹ�
//    HI_ERR_NOT_FOUND : û���ҵ�ע�����Ϣ�������
//    ����Ϊ������
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: liuxipeng/lkf54842 [2013-08-30]
//*****************************************************************************
HI_PRV HI_U32 mrsChlConnectMsgProc(HI_U16 usPort, HI_U8 ucOption)
{
    HI_U32 ret = HI_ERR_SUCCESS;

    if (HI_DMS_CHL_MAC_PORT_APP == usPort)
    {
        MRS_MSG_NODE_STRU* pstNode = HI_NULL;

        GetFwCtx()->ucPlcChlStatus = ucOption;

        pstNode = mrsFindMsgNode(ID_MRS_MSG_PLC_CHL_STATUS);
        if ((pstNode) && (pstNode->pfnMsg))
        {
            MRS_MSG_PLC_CHL_STATUS_STRU stChlStatus = { 0 };

            stChlStatus.bAvail = (HI_BOOL) (ucOption == HI_DMS_CHL_CONNECT) ? HI_TRUE:HI_FALSE;
            pstNode->pfnMsg(ID_MRS_MSG_PLC_CHL_STATUS, (HI_PVOID)&stChlStatus);
        }
        else
        {
            ret = HI_ERR_NOT_FOUND;
        }
    }

    return ret;
}


#if (defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) || defined(PRODUCT_CFG_PRODUCT_TYPE_STA))
HI_PRVL HI_U32 mrsMsgProcCcoFrameInput(HI_IN HI_U16 usId, HI_IN HI_SYS_QUEUE_MSG_S* pstMsg)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    MRS_ONE_RAW_FRAME_STRU* pstFrame = HI_NULL;

    pstFrame = (MRS_ONE_RAW_FRAME_STRU*)pstMsg->ulParam[0];
    if (!pstFrame)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    mrsDfxChlUartRx(pstFrame->pucDatagram, pstFrame->usDatagramSize, HI_TRUE);  // ���ڽ���ͳ��(��Ϣ����)

    // ��Ϣ�ַ��� Service
    ret = mrsCcoDispatchRxFrameData(usId, pstFrame);
    if(HI_ERR_SUCCESS == ret)
    {
        mrsDfxChlUartOkFrameCnt(pstFrame->usDatagramSize, HI_TRUE);    // ���ڽ�������֡ͳ��(��Ϣ����)
    }
    else
    {
        mrsDfxChlUartErrRx(pstFrame->usDatagramSize, ret, HI_TRUE);     // ���ڽ��մ�����(��Ϣ����)
        mrsDfxChlUartErrFrameCnt(HI_TRUE);   // ���ڽ�������֡����ͳ��(��Ϣ����)
    }

    // �ַ�������պ�, ��Դ�ͷ�
    mrsToolsFree(pstFrame);

    return ret;
}
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
HI_PRVL HI_U32 mrsMsgProc645Input(HI_IN HI_SYS_QUEUE_MSG_S* pstMsg)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    MRS_ONE_RAW_FRAME_STRU* pstFrame = (MRS_ONE_RAW_FRAME_STRU*)&pstMsg->ulParam[0];

    mrsDfxChlUartRx(pstFrame->pucDatagram, pstFrame->usDatagramSize, HI_TRUE);    // ���ڽ���ͳ��(��Ϣ����)

    // �ַ���Service
    ret = mrsDispatchRx645Data(pstFrame);
    if(ret == HI_ERR_SUCCESS)
    {
        mrsDfxChlUartOkFrameCnt(pstFrame->usDatagramSize, HI_TRUE);    // ���ڽ�������֡ͳ��(��Ϣ����)
    }
    else
    {
        mrsDfxChlUartErrRx(pstFrame->usDatagramSize, ret, HI_TRUE); // ���ڽ��մ�����(��Ϣ����)
        mrsDfxChlUartErrFrameCnt(HI_TRUE);   // ���ڽ�������֡����ͳ��(��Ϣ����)
    }

    mrsToolsFree(pstFrame->pucDatagram);

    return ret;
}
#endif

HI_PRVL HI_U32 mrsMsgProcMacInput(HI_IN HI_SYS_QUEUE_MSG_S* pstMsg)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_DMS_CHL_RX_S* pstRx = (HI_DMS_CHL_RX_S*)pstMsg->ulParam[0];

    HI_ASSERT(pstRx);

    mrsDfxChlPlcRx(pstRx, HI_TRUE);    // PLCͨ������ͳ��(��Ϣ����)

    // CCO<-->STA ֮��ͨ�� ����֡�ַ�����
    ret = mrsDispatchCommand((HI_U16)pstMsg->ulParam[1], pstRx);
    if(ret != HI_ERR_SUCCESS)
    {
        mrsDfxChlPlcErrRx(pstRx, ret, HI_TRUE);    // PLCͨ�����մ���ͳ��(��Ϣ����)
    }

    mrsToolsFree(pstRx);

    return ret;
}

HI_PRVL HI_U32 mrsMsgProcMrOutput(HI_IN HI_SYS_QUEUE_MSG_S* pstMsg)
{
    HI_U32 ret = HI_ERR_SUCCESS;

    if(HI_DMS_CHL_UART_PORT_APP == pstMsg->ulParam[1])
    {
        mrsDfxChlUartTxMsgRx((HI_DMS_CHL_TX_DATA_S*)pstMsg->ulParam[0]);    // ���ڷ���ͳ��(��Ϣ����)
    }

    ret = mrsSend2MrDevice(pstMsg);
    /* BEGIN: Added by liupeidong/00238134, 2014/12/10 ���ⵥ��:DTS2014121005392 */
#if defined(PRODUCT_CFG_PRODUCT_TYPE_NDM)
    if (HI_MDM_IsNdmMeterAccess())
    {
        HI_MDM_NdmSerialInterfaceTxLedProc();
    }
#endif
    /* END: Added by liupeidong/00238134, 2014/12/10 ���ⵥ��:DTS2014121005392 */

    if(HI_DMS_CHL_UART_PORT_APP == pstMsg->ulParam[1])
    {
        if(HI_ERR_SUCCESS != ret)
        {
            HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_012, HI_DIAG_MT("TX MR Fail-[%d]"),ret);
            mrsDfxChlUartErrTxMsgRx((HI_DMS_CHL_TX_DATA_S*)pstMsg->ulParam[0], ret);    // ���ڷ���ʧ��ͳ��(��Ϣ����)
        }
        else
        {
            mrsDfxChlUartOkTxMsgRx((HI_DMS_CHL_TX_DATA_S*)pstMsg->ulParam[0]);  // ���ڷ���ͳ��(��Ϣ����)
        }
    }

    mrsToolsFree(pstMsg->ulParam[0]);

    return ret;
}

HI_PRVL HI_U32 mrsMsgProcMacOutPut(HI_IN HI_SYS_QUEUE_MSG_S* pstMsg)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_DMS_CHL_TX_DATA_S * pstData = (HI_DMS_CHL_TX_DATA_S *)pstMsg->ulParam[0];
    HI_DMS_CHL_TX_ADDR_S * pstAddr = (HI_DMS_CHL_TX_ADDR_S *)pstMsg->ulParam[1];

    mrsDfxChlPlcTx(pstData, pstAddr, HI_TRUE);    // PLC����ͳ��(��Ϣ����)

    ret = mrsSend2MacLayer(pstMsg);
    if(HI_ERR_SUCCESS != ret)
    {
        HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_013, HI_DIAG_MT("TX PLC Fail-[%d]"),ret);

        mrsDfxChlPlcErrTx(pstData, pstAddr, ret, HI_TRUE);    // PLC����ʧ��ͳ��(��Ϣ����)
        mrsDfxPlcBcTxErr(pstAddr);    // ͳ�ƹ㲥֡����ʧ��
    }
    else
    {
        mrsDfxPlcBcTx(pstAddr);   // ͳ�Ʒ��͹㲥֡
    }

    mrsToolsFree(pstData);
    mrsToolsFree(pstAddr);

    return ret;
}

//*****************************************************************************
// ��������: mrsMsgProc
// ��������: ������Ϣ����
//
// ����˵��:
//   pstMsg [in] ��Ϣ
//
// �� �� ֵ:
//    HI_ERR_SUCCESS   : ����ɹ�
//    HI_ERR_NOT_FOUND : û���ҵ�ע�����Ϣ�������
//    ����Ϊ������
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: shenhankun/00130424 [2011-12-26]
//*****************************************************************************
HI_PRV HI_U32 mrsMsgProc(HI_IN HI_SYS_QUEUE_MSG_S* pstMsg)
{
    HI_U32 ret = HI_ERR_SUCCESS;

    HI_ASSERT(pstMsg);

    mrsDfxMsgRxCnt(pstMsg); // ��Ϣ����ͳ��

    ret = mrsCcoStaMsgProc(pstMsg);
	if (HI_ERR_SUCCESS != ret)
	{
		return ret;
	}

    return ret;
}

HI_PRV HI_U32 mrsDiagCmdMsgProc(HI_IN HI_SYS_QUEUE_MSG_S* pstMsg)
{
	switch (pstMsg->ulParam[0])
	{
	case ID_DIAG_CMD_MRS_HSO_SET_CCO_MODE:
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
		mrsDiagCcoModeNotify(pstMsg);
		mrsToolsFree(pstMsg->ulParam[1]);
#endif
		break;

	default:
		break;
	}

	return HI_ERR_SUCCESS;
}

HI_PRV HI_U32 mrsCcoStaMsgProc(HI_IN HI_SYS_QUEUE_MSG_S* pstMsg)
{
    HI_U32 ret = HI_ERR_SUCCESS;

    HI_ASSERT(pstMsg);

    mrsDfxMsgRxCnt(pstMsg); // ��Ϣ����ͳ��

    switch (pstMsg->ulMsgId)
    {
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    case EN_MRS_FW_MSG_376_2_FRAME_INPUT:
        ret = mrsMsgProcCcoFrameInput((HI_U16)pstMsg->ulMsgId, pstMsg);
        break;
#endif

#if (defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) || defined(PRODUCT_CFG_PRODUCT_TYPE_STA))
    case EN_MRS_FW_MSG_CSG_FRAME_INPUT:
        ret = mrsMsgProcCcoFrameInput((HI_U16)pstMsg->ulMsgId, pstMsg);
        break;
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
    case EN_MRS_FW_MSG_645_FRAME_INPUT:
        ret = mrsMsgProc645Input(pstMsg);
        break;
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
    case EN_MRS_FW_MSG_MDM_INPUT:
        mrsCsgCltIUpgMsgProc(pstMsg);
        break;
#endif
#endif

    case EN_MRS_FW_MSG_MAC_FRAME_INPUT:
        ret = mrsMsgProcMacInput(pstMsg);
        break;

    case EN_MRS_FW_MSG_MR_FRAME_OUTPUT:
        ret = mrsMsgProcMrOutput(pstMsg);
        break;

    case EN_MRS_FW_MSG_MAC_FRAME_OUTPUT:
        ret = mrsMsgProcMacOutPut(pstMsg);
        break;
#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA) && defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
    //����ͣ���ϱ��¼�
    case EN_MRS_FW_MSG_POWER_FAILURE:
        ret = mrsHandlePowerFailure(pstMsg);
        break;
#endif

/* BEGIN: Added by fengxiaomin/00209182, 2014/1/10   ���ⵥ�ź�:DTS2014012607684  */
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
/* BEGIN: PN: DTS2015061008265 MODIFY\ADD\DEL by cuiate/00233580 at 2015/6/24 */
    case EN_MRS_FW_MSG_MAC_MSG:
        ret = mrsMsgProcMacMsg(pstMsg);
        break;
/* END:   PN: DTS2015061008265 MODIFY\ADD\DEL by cuiate/00233580 at 2015/6/24 */

    case EN_MRS_FW_MSG_MDM_INPUT:
        ret = mrsRemoteUpgMsgProc(pstMsg);
        break;

    case EN_MRS_FW_MSG_RM_PLC_SEND_REQ:
        ret = mrsRmPlcSendReqProc(pstMsg);
        break;

    case EN_MRS_FW_MSG_MAC_ND_STATE_CHG:
        ret = mrsNodeStateChgMsgProc(pstMsg);
        break;

	case EN_MRS_FW_MSG_MAC_JOIN_WHITE_LIST:
		ret = mrsMacJoinWhiteMsgProc(pstMsg);
		break;
#endif
/* END:   Added by fengxiaomin/00209182, 2014/1/10 */
    case EN_MRS_FW_MSG_NV_CHANGE_NOTIFY:
        ret = mrsNvChangeMsgProc(pstMsg);
        break;

    case EN_MRS_FW_MSG_ZERO_CROSS_NOTIFY:
        mrsZeroCrossCheckProc(HI_ERR_SUCCESS, (EQUIP_ZERO_CROSS_REQ_ACK_STRU *)pstMsg->ulParam[0]);
        break;

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    case EN_MRS_FW_MSG_MR_RETRY_DIAG_CMD:
        ret = mrsDfxMrRetryDiagCmdProc(pstMsg);
        break;

    case EN_MRS_FW_MSG_CCO_MR_TASK:
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
        mrsCcoMrTaskMsgProc(pstMsg);
#endif
        break;

    case EN_MRS_FW_MSG_PAUSE_RESUME_DETECT:
        mrsResumeDetectRmProc();
        break;

#endif

#if (defined(PRODUCT_CFG_PRODUCT_TYPE_STA) && !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD))
    case EN_MRS_FW_MSG_CLTII_EVENT_MON:
        mrsCltiiEventMsgProc(pstMsg);
        break;
#endif

    case EN_MRS_FW_MSG_DIAG_CMD_MSG:
        mrsDiagCmdMsgProc(pstMsg);
        break;

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    case EN_MRS_FW_MSG_TOPO_CHANGE_NOTIFY:
        mrsCcoTopoStatusChgProc(pstMsg);
        break;
#endif

    default:
        ret = msrFwDefaultProc(pstMsg);
        break;
    }

    return ret;
}

// Timer��Ϣ����
HI_PRV HI_U32 mrsTimerProc(HI_U32 ulTimerId)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    MRS_MSG_NODE_STRU* pstNode = mrsFindMsgNode(ID_MRS_MSG_MR_TIMER);

    if (pstNode)
    {
        // ���� ID_MRS_MSG_MR_TIMER ��Ϣ �� Service
        if (pstNode->pfnMsg)
        {
            MRS_MSG_MR_TIMER_STRU stTimer = { 0 };

            stTimer.ucTimerId = (HI_U8)ulTimerId;
            pstNode->pfnMsg(ID_MRS_MSG_MR_TIMER, (HI_PVOID)&stTimer);
        }
    }
    else
    {
        ret = HI_ERR_NOT_FOUND;
    }

    return ret;
}

// ȱʡ��Ϣ����
HI_PRV HI_U32 msrFwDefaultProc(HI_IN HI_SYS_QUEUE_MSG_S* pstMsg)
{
    HI_U32 ret = HI_ERR_SUCCESS;

    HI_ASSERT(pstMsg);

    switch (pstMsg->ulMsgId)
    {
    case ID_MRS_MSG_PLC_CHL_STATUS:
        ret = mrsChlConnectMsgProc((HI_U16)pstMsg->ulParam[0], (HI_U8)pstMsg->ulParam[1]);
        break;

    case ID_MRS_MSG_MR_TIMER:
        mrsTimerProc(pstMsg->ulParam[0]);
        break;

    default:
        break;
    }

    return ret;
}

// PLC����ַ�����
HI_PRV HI_INLINE HI_U32 mrsDispatchCommand(HI_U16 usId, HI_IN HI_DMS_CHL_RX_S* pstRx)
{
    HI_U32 ret  = HI_ERR_SUCCESS;
    HI_U16 usNodeId = (~0);
    MRS_CMD_NODE_STRU* pstNode = HI_NULL;

    if(HI_NULL == pstRx)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    usNodeId = pstRx->usId;
    pstNode  = mrsFindCmdNode(usNodeId);

    if (pstNode)
    {
        if (pstNode->pfnReq)
        {
            MRS_PLC_FRAME_DATA_S stFrameData;

            (hi_void)memset_s(&stFrameData, sizeof(MRS_PLC_FRAME_DATA_S), 0, sizeof(MRS_PLC_FRAME_DATA_S));

            (hi_void)memcpy_s(stFrameData.ucMac, sizeof(stFrameData.ucMac), pstRx->ucSrcMac, HI_PLC_MAC_ADDR_LEN);
            stFrameData.usId         = usId?usId:usNodeId;
            stFrameData.usPayloadLen = pstRx->usPayloadLen;
            stFrameData.pucPayload   = pstRx->pPayload;

            if(mrsToolsBroadCastMac(pstRx->ucDestMac))
            {
                stFrameData.is_bc_frame = HI_TRUE;
                mrsDfxPlcBcRx();  // ͳ�ƽ��չ㲥֡
            }

            ret = pstNode->pfnReq(stFrameData.usId, &stFrameData, 0);
        }
        else
        {
            ret = HI_ERR_NOT_REGISTER; // û��ע��
        }
    }
    else
    {
        ret = HI_ERR_NOT_FOUND;
    }

    return ret;
}

// 645���պ�� �ַ�����
#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
HI_PRV HI_U32 mrsDispatchRx645Data(HI_IN MRS_ONE_RAW_FRAME_STRU* pstRx)
{
    HI_U32 ret  = HI_ERR_SUCCESS;
    HI_U16 usId = ID_MRS_MSG_MR_DATA;
    MRS_MSG_NODE_STRU* pstNode = mrsFindMsgNode(usId);

    if(HI_NULL == pstRx)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    if(pstNode)
    {
        if(pstNode->pfnMsg)
        {
            MRS_MSG_MR_645_DATA_STRU stData;

            stData.pucData     = pstRx->pucDatagram;
            stData.usDataSizse = pstRx->usDatagramSize;
            pstNode->pfnMsg(usId, (HI_PVOID)&stData);
        }
    }
    else
    {
        ret = HI_ERR_NOT_FOUND;
    }

    return ret;
}
#endif

// 376.2���պ�� �ַ�����
#if (defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) || defined(PRODUCT_CFG_PRODUCT_TYPE_STA))
HI_PRV HI_U32 mrsCcoDispatchRxFrameData(HI_IN HI_U16 usId, HI_IN MRS_ONE_RAW_FRAME_STRU* pstFrame)
{
    HI_U16 usNodeId = (usId != EN_MRS_FW_MSG_376_2_FRAME_INPUT) ? ID_MRS_MSG_MR_DATA_CSG : ID_MRS_MSG_MR_DATA;
    MRS_MSG_NODE_STRU* pstNode = mrsFindMsgNode(usNodeId);

    if (pstNode)
    {
        if(pstNode->pfnMsg)
        {
            MRS_MSG_MR_3762_DATA_STRU stData;

            (hi_void)memset_s(&stData, sizeof(stData), 0, sizeof(stData));
            stData.pucData = pstFrame->pucDatagram;
            stData.usDataSizse = pstFrame->usDatagramSize;

            pstNode->pfnMsg(usNodeId, (HI_PVOID)&stData);
        }
    }
    else
    {
        return HI_ERR_NOT_FOUND;
    }

    return HI_ERR_SUCCESS;
}
#endif

// ���������ڵ�
HI_PRV MRS_CMD_NODE_STRU* mrsFindCmdNode(HI_U16 usCmdId)
{
    HI_U16 i = 0;
    MRS_FW_CTX_STRU* pstFwCtx = GetFwCtx();

    // ����ע�������REQ�������
    for (i = 0; i < pstFwCtx->usCmdNum; i++)
    {
        HI_CONST MRS_FW_CMD_PROC_TBL_STRU* pCmdList = (MRS_FW_CMD_PROC_TBL_STRU*)(pstFwCtx->pCmdList + i);
        HI_U32 ulId = usCmdId;

        if ((ulId >= pCmdList->usMinId) && (ulId <= pCmdList->usMaxId))
        {
            pstFwCtx->stCmdNode.usCmdId = usCmdId;
            pstFwCtx->stCmdNode.pfnReq = pCmdList->pfnReq;
            return (&pstFwCtx->stCmdNode);
        }
    }

    return HI_NULL;
}


// ������Ϣ����ڵ�
HI_PRV MRS_MSG_NODE_STRU* mrsFindMsgNode(HI_U16 usId)
{
    HI_U32 i = 0;
    MRS_FW_CTX_STRU* pstFwCtx = GetFwCtx();

    // ����ע�������REQ�������
    for (i = 0; i < pstFwCtx->usMsgNum; i++)
    {
        HI_CONST MRS_FW_MSG_PROC_TBL_STRU* pMsgList = (MRS_FW_MSG_PROC_TBL_STRU*)(pstFwCtx->pMsgList + i);

        if ((usId >= pMsgList->usMinId) && (usId <= pMsgList->usMaxId))
        {
            pstFwCtx->stMsgNode.usMsgId = usId;
            pstFwCtx->stMsgNode.pfnMsg  = pMsgList->pfnMsg;
            return (&pstFwCtx->stMsgNode);
        }
    }

    return HI_NULL;
}


// �������ݻ�д�������豸��
HI_PRVL HI_U32 mrsSend2MrDevice(HI_IN HI_SYS_QUEUE_MSG_S* pstMsg)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_DMS_CHL_TX_DATA_S* pstFrameData = (HI_DMS_CHL_TX_DATA_S*)pstMsg->ulParam[0];

    HI_U16 ucChlType = (HI_U16)(pstMsg->ulParam[1]);
    if (ucChlType == HI_DMS_CHL_UART_PORT_APP)
    {
        //DO
    }
#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
    else if (ucChlType == HI_DMS_CHL_UART_PORT_IR)
    {
        //DO
    }
#endif
    else
    {
        return HI_ERR_FAILURE;
    }

    // TODO: Ŀǰ�汾��֧�� UART  (shenhankun/2011-12-29)
    ret = HI_MDM_ChlTx(ucChlType, pstFrameData, HI_NULL);
    if(HI_ERR_SUCCESS != ret)
    {
        HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_014, HI_DIAG_MT("UART TX Fail-[%d]"), ret);
    }

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    MRS_BufLog("[CCO-COM][Tx  ]: ", pstFrameData->ucData, pstFrameData->usDataSize);
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
    MRS_Printf("[%d]", g_ulMrsBaudRate);
    MRS_BufLog("[STA-COM][Tx  ]: ", pstFrameData->ucData, pstFrameData->usDataSize);
#endif

    return ret;
}

// PLC֡����
HI_PRVL HI_U32 mrsSend2MacLayer(HI_IN HI_SYS_QUEUE_MSG_S* pstMsg)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_DMS_CHL_TX_DATA_S* pstFrameData = (HI_DMS_CHL_TX_DATA_S*)pstMsg->ulParam[0];
    HI_DMS_CHL_TX_ADDR_S* pstFrameAddr = (HI_DMS_CHL_TX_ADDR_S*)pstMsg->ulParam[1];

    // Ŀǰ�汾��֧�� MAC ͨ��  (shenhankun/2011-12-29)
    ret = HI_MDM_ChlTx(HI_DMS_CHL_MAC_PORT_APP, pstFrameData, pstFrameAddr);
    if (HI_ERR_SUCCESS == ret)
    {
        HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1004, HI_DIAG_MT("TX PLC OK"), pstFrameAddr->ucDestMac, HI_PLC_MAC_ADDR_LEN);
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
        MRS_BufLog("[CCO-PLC][Tx  ]: ", pstFrameData->ucData, pstFrameData->usDataSize);
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
        MRS_BufLog("[STA-PLC][Tx  ]: ", pstFrameData->ucData, pstFrameData->usDataSize);
#endif
    }

    return ret;
}

/*****************************************************************************
 �� �� ��  : MRS_SendFrameProc
 ��������  : ����֡�Ĺ�����ȡ����
 �������  : HI_IN MRS_PLC_FRAME_DATA_S* pstFrameData
 �������  : ��
 �� �� ֵ  : HI_U32
 ���������õĺ���  :
 ���ñ������ĺ���  :
 ���ʵ�ȫ�ֱ���  :
 �޸ĵ�ȫ�ֱ���  :
 �޸ĵ�BBP�Ĵ����б�  :

 �޸���ʷ      :
  1.��    ��   : 2017��9��19��
    ��    ��   : test
    �޸�����   : �����ɺ���

*****************************************************************************/

HI_U32 MRS_SendFrameProc(HI_IN MRS_PLC_FRAME_DATA_S* pstFrameData)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_SYS_QUEUE_MSG_S stMsg = {0};
    HI_DMS_CHL_TX_DATA_S *pstFrame = HI_NULL;
    HI_DMS_CHL_TX_ADDR_S *pstAddr = HI_NULL;

    pstAddr = (HI_DMS_CHL_TX_ADDR_S *)mrsToolsMalloc(sizeof(HI_DMS_CHL_TX_ADDR_S));
    if (!pstAddr)
    {
        return HI_ERR_NO_MORE_MEMORY;
    }
    (hi_void)memset_s(pstAddr, sizeof(HI_DMS_CHL_TX_ADDR_S), 0, sizeof(HI_DMS_CHL_TX_ADDR_S));

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    if (mrsCcoIsTestMode()
        && ((ID_MRS_CMD_DATA_TRANSMIT_AFN13_PLC == pstFrameData->usId)
        || (ID_MRS_CMD_DATA_TRANSMIT_AFN14_PLC == pstFrameData->usId)))
    {
        MRS_CMD_DATA_TRANSMIT_STRU *pTransmit = HI_NULL;

        pstFrame = (HI_DMS_CHL_TX_DATA_S *)mrsToolsMalloc(sizeof(HI_DMS_CHL_TX_DATA_S)
            + pstFrameData->usPayloadLen
            + sizeof(MRS_CMD_DATA_TRANSMIT_STRU));
        if (!pstFrame)
        {
            mrsToolsFree(pstAddr);
            return HI_ERR_NO_MORE_MEMORY;
        }

        pTransmit = (MRS_CMD_DATA_TRANSMIT_STRU *)pstFrame->ucData;
        pTransmit->usCmdId = pstFrameData->usId;
        pTransmit->usPayloadLen = pstFrameData->usPayloadLen;
        (hi_void)memcpy_s(pTransmit->aucPayload, pTransmit->usPayloadLen, pstFrameData->pucPayload, pstFrameData->usPayloadLen);

        pstFrame->usDataSize = pstFrameData->usPayloadLen + sizeof(MRS_CMD_DATA_TRANSMIT_STRU);

        pstAddr->usId = ID_MRS_CMD_DATA_TRANSMIT_PLC;
    }
    else
#endif
    {
        pstFrame = (HI_DMS_CHL_TX_DATA_S *)mrsToolsMalloc(sizeof(HI_DMS_CHL_TX_DATA_S) + pstFrameData->usPayloadLen);
        if (!pstFrame)
        {
            mrsToolsFree(pstAddr);
            return HI_ERR_NO_MORE_MEMORY;
        }

        pstFrame->usDataSize = pstFrameData->usPayloadLen;
        (hi_void)memcpy_s(pstFrame->ucData, pstFrame->usDataSize, pstFrameData->pucPayload, pstFrameData->usPayloadLen);

        pstAddr->usId = pstFrameData->usId;
    }

    (hi_void)memcpy_s(pstAddr->ucDestMac, sizeof(pstAddr->ucDestMac), pstFrameData->ucMac, HI_PLC_MAC_ADDR_LEN);
    pstAddr->ucCtrl     = HI_DMS_CHL_TX_CTRL_NA;
    pstAddr->ucType     = HI_DMS_CHL_TX_TYPE_MR;
    pstAddr->usSn       = 0;
    pstAddr->ucDuration = 0;
    pstAddr->ucMaxResendCount = 0;
    pstAddr->ucPhase = 0;
    pstAddr->bIsBroadCast = HI_FALSE;

    //Ĭ�����ȼ�Ϊ3(3Ϊ������ȼ�,0Ϊ������ȼ�)
    pstAddr->ucPrio = 3;

    mrsDfxChlPlcTx(pstFrame, pstAddr, HI_FALSE);    // PLC����ͳ��(��Ϣ����)

    stMsg.ulMsgId       = EN_MRS_FW_MSG_MAC_FRAME_OUTPUT;
    stMsg.ulParam[0]    = (HI_U32) pstFrame;
    stMsg.ulParam[1]    = (HI_U32) pstAddr;

    // ������Ϣ�� MRS����, ������Ϣ�ַ�����
    ulRet = mrsSendMessage2Queue(&stMsg);
    if(HI_ERR_SUCCESS != ulRet)
    {
        mrsDfxChlPlcErrTx(pstFrame, pstAddr, ulRet, HI_FALSE);    // PLC����ʧ��ͳ��(��Ϣ����)

        HI_MDM_ReportSysEvent(EID_APP_SEND_MSG_FAIL);

        mrsToolsFree(pstFrame);
        mrsToolsFree(pstAddr);
    }

    return ulRet;
}

//*****************************************************************************
// ��������: MRS_SendPlcFrame
// ��������: ����PLC֡�����ݸ�CCO/STA
//
// ����˵��:
//   pstFrameData [in] MR֡����
//
// �� �� ֵ:
//    HI_ERR_SUCCESS: �ɹ�д��ϵͳ������, �ɵײ�ģ�鸺���͵�����ͨ��ģ��.
//    ����������
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: shenhankun/00130424 [2011-11-04]
//*****************************************************************************
HI_U32 MRS_SendPlcFrame(HI_IN MRS_PLC_FRAME_DATA_S* pstFrameData)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;

    #if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
    POWER_FAILURE_RCV_MAC_REPORT_S *deviceState = mrsGetPowerFailureMacInfo();
    //ͣ���ж�,����ͣ��󣬲������ʹ���
    if(HI_TRUE == deviceState->devicePowerFailure)
    {
        return HI_ERR_FAILURE;
    }
    #endif

    ulRet = MRS_SendFrameProc(pstFrameData);

    return ulRet;
}

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)

HI_U32 mrsGetCurBaudRate(HI_VOID)
{
    if (0 == g_ulMrsBaudRate)
    {
        HI_U32 ulRet = HI_ERR_SUCCESS;
        HI_DMS_NV_FTM_DEVICE_CFG_S stDevCfg;

        ulRet = HI_MDM_ReadDevCfg(&stDevCfg);
        if (HI_ERR_SUCCESS != ulRet)
        {
            return MRS_SRV_BAUD_RATE_DEFAULT;
        }

        g_ulMrsBaudRate = stDevCfg.astDevItem[HI_NV_FTM_DEVICE_CFG_STA].astUartCfg[HI_DMS_CHL_UART_PORT_APP].ulBaudRate;
        return g_ulMrsBaudRate;
    }

    return g_ulMrsBaudRate;
}

//*****************************************************************************
// ��������: MRS_ChangeBaudRate
// ��������: ���Ĳ�����
//
// ����˵��:
//   ulRate ������
//
// �� �� ֵ:
//    HI_ERR_SUCCESS: �ɹ��޸Ĳ�����
//    ����������
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: shenhankun/00130424 [2012-09-22]
//*****************************************************************************
HI_U32 MRS_ChangeBaudRate(HI_U32 ulRate)
{
    HI_U32 ret = HI_ERR_SUCCESS;

    if (g_ulMrsBaudRate != ulRate)
    {
        HI_DMS_NV_FTM_DEVICE_CFG_S stDevCfg;

        g_ulMrsBaudRate = ulRate;

        ret = HI_MDM_ReadDevCfg(&stDevCfg);
        if(ret == HI_ERR_SUCCESS)
        {
            HI_FTM_DEVICE_UART_CFG_S *pstUartCfg = HI_NULL;

            pstUartCfg = &stDevCfg.astDevItem[HI_NV_FTM_DEVICE_CFG_STA].astUartCfg[HI_DMS_CHL_UART_PORT_APP];
            pstUartCfg->ulBaudRate = ulRate;
            ret = HI_MDM_ChlUartPortRefresh(pstUartCfg, HI_FTM_CHIP_TYPE_3911T, HI_DMS_CHL_UART_PORT_APP);
            if (ret == HI_ERR_SUCCESS)
            {
                ret = mrsFwChlInit(&g_stMrsChlPort, HI_DMS_CHL_UART_PORT_APP);
            }
        }
    }

    return ret;
}
#endif


//*****************************************************************************
// ��������: MRS_SendMrData
// ��������: ���ͳ������ݸ����/������
//
// ����˵��:
//   pucData [in] MR����
//   usDataSize [in] MR���ݴ�С,��λ�ֽ�
//
// �� �� ֵ:
//    HI_ERR_SUCCESS: �ɹ�д��DMS������, ��DMSд��MR�豸��
//    ����������
//
// ����Ҫ��: ��֧���Զ����ǰ���ֽڵȹ���,���ṩͨ�����ܷ������ݵĹ���
// ���þ���: TODO: ...
// ��    ��: shenhankun/00130424 [2011-11-04]
//*****************************************************************************
HI_U32 MRS_SendMrData(HI_IN HI_PBYTE pucData, HI_U16 usDataSize, HI_U16 ucChlType)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_SYS_QUEUE_MSG_S stMsg = { 0 };
    HI_DMS_CHL_TX_DATA_S* pstFrameData = HI_NULL;

    if(0 == usDataSize)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    pstFrameData = (HI_DMS_CHL_TX_DATA_S*)mrsToolsMalloc(usDataSize + sizeof(HI_DMS_CHL_TX_DATA_S));
    if(HI_NULL == pstFrameData)
    {
        return HI_ERR_NO_MORE_MEMORY;
    }

    // ֻ����ҵ�񴮿����ݵ�ͳ��
    if(HI_DMS_CHL_UART_PORT_APP == ucChlType)
    {
        mrsDfxChlUartTxMsgTx(pucData, usDataSize);  // ���ڷ���ͳ��(��Ϣ����)
    }

    pstFrameData->usDataSize = usDataSize;
    (hi_void)memcpy_s(pstFrameData->ucData, pstFrameData->usDataSize, pucData, usDataSize);

    stMsg.ulMsgId     = EN_MRS_FW_MSG_MR_FRAME_OUTPUT;
    stMsg.ulParam[0]  = (HI_U32)pstFrameData;
    stMsg.ulParam[1] = ucChlType;

    // ������Ϣ�� MRS����, ������Ϣ�ַ�����
    ret = mrsSendMessage2Queue(&stMsg);
    if(HI_ERR_SUCCESS != ret)
    {
        // ֻ����ҵ�񴮿����ݵ�ͳ��
        if(HI_DMS_CHL_UART_PORT_APP == ucChlType)
        {
            mrsDfxChlUartErrTxMsgTx(usDataSize, ret);
        }

        HI_MDM_ReportSysEvent(EID_APP_SEND_MSG_FAIL);

        mrsToolsFree(pstFrameData);
    }
    else
    {
        // ֻ����ҵ�񴮿����ݵ�ͳ��
        if(HI_DMS_CHL_UART_PORT_APP == ucChlType)
        {
            mrsDfxChlUartOkTxMsgTx(usDataSize); // ���ڷ��ͳɹ�ͳ��(��Ϣ����)

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
            if (mrsToolsIsIICollector())
            {
                mrsUARTRTxLedPro(); // UART���� LED����
            }
#endif
        }

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
        HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1005, HI_DIAG_MT("[CCO-MR]-TX"), pucData, usDataSize);
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
        HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1006, HI_DIAG_MT("[STA-MR]-TX"), pucData, usDataSize);
#endif
    }

    return ret;
}


//*****************************************************************************
// ��������: MRS_UartWrite
// ��������: ���ͳ������ݸ�����(����Ϣ����ֱ��д����)
//
// ����˵��:
//   pucData [in] MR����
//   usDataSize [in] MR���ݴ�С,��λ�ֽ�
//
// �� �� ֵ:
//    HI_ERR_SUCCESS: �ɹ�д��Uart��
//    ����������
//
// ����Ҫ��: ��֧���Զ����ǰ���ֽڵȹ���,���ṩͨ�����ܷ������ݵĹ���
// ���þ���: TODO: ...
// ��    ��: liuxipeng/lkf54842 [2014-01-15]
//*****************************************************************************
#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
HI_U32 MRS_UartWrite(HI_IN HI_PBYTE pucData, HI_U16 usDataSize, HI_U16 ucChlType)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_DMS_CHL_TX_DATA_S* pstFrameData = HI_NULL;


    mrsDfxChlUartTxMsgTx(pucData, usDataSize);  // ���ڷ���ͳ��(��Ϣ����)
    mrsDfxChlUartOkTxMsgTx(usDataSize);         // ���ڷ��ͳɹ�ͳ��(��Ϣ����)

    if(0 == usDataSize)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    pstFrameData = (HI_DMS_CHL_TX_DATA_S*)mrsToolsMalloc(usDataSize + sizeof(HI_DMS_CHL_TX_DATA_S));
    if(HI_NULL == pstFrameData)
    {
        return HI_ERR_NO_MORE_MEMORY;
    }

    // ֻ����ҵ�񴮿����ݵ�ͳ��
    if(HI_DMS_CHL_UART_PORT_APP == ucChlType)
    {
        mrsDfxChlUartTxMsgRx(pstFrameData);  // ���ڷ���ͳ��(��Ϣ����)
    }

    pstFrameData->usDataSize = usDataSize;
    (hi_void)memcpy_s(pstFrameData->ucData, pstFrameData->usDataSize, pucData, usDataSize);

    ret = HI_MDM_ChlTx(ucChlType, pstFrameData, HI_NULL);

    if(HI_ERR_SUCCESS != ret)
    {
        // ֻ����ҵ�񴮿����ݵ�ͳ��
        if(HI_DMS_CHL_UART_PORT_APP == ucChlType)
        {
            mrsDfxChlUartErrTxMsgRx(pstFrameData, ret);
        }
    }
    else
    {
        // ֻ����ҵ�񴮿����ݵ�ͳ��
        if(HI_DMS_CHL_UART_PORT_APP == ucChlType)
        {
            mrsDfxChlUartOkTxMsgRx(pstFrameData); // ���ڷ��ͳɹ�ͳ��(��Ϣ����)

            if (mrsToolsIsIICollector())
            {
                mrsUARTRTxLedPro();
            }
        }

        HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1007, HI_DIAG_MT("[UART]-TX"), pucData, usDataSize);
        MRS_Printf("[%d]", g_ulMrsBaudRate);
        MRS_BufLog("[STA-COM][Tx  ]: ", pucData, usDataSize);
    }

    mrsToolsFree(pstFrameData);

    return ret;
}
#endif //defined(PRODUCT_CFG_PRODUCT_TYPE_STA)


HI_U8 mrsGetPlcChlStatus(HI_VOID)
{
    return GetFwCtx()->ucPlcChlStatus;
}

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
HI_U8 mrsGetTopoStatus(HI_VOID)
{
    return GetFwCtx()->ucPlcTopoStatus;
}

HI_VOID mrsSetTopoStatus(HI_U8 ucStatus)
{
    GetFwCtx()->ucPlcTopoStatus = ucStatus;
    mrsDfxCcoTopoStatus(ucStatus);
}
#endif

HI_VOID mrsFreePlcFrameDataStru(MRS_PLC_FRAME_DATA_STRU *pstPlcFrameDataStru)
{
    if (pstPlcFrameDataStru)
    {
        if (pstPlcFrameDataStru->pucPayload)
        {
            mrsToolsFree(pstPlcFrameDataStru->pucPayload);
        }
        mrsToolsFree(pstPlcFrameDataStru);
    }
}


HI_VOID mrsFwEnableUartDbg(HI_U8 ucEnable)
{
    GetFwCtx()->ucEnableUartDbg = ucEnable;
}

HI_BOOL mrsIsUartDbgEnabled(HI_VOID)
{
    return (HI_BOOL)(GetFwCtx()->ucEnableUartDbg);
}


