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
#include "mrs_srv_clti_upgrade.h"
#include "mrs_srv_baudrate_manage.h"
#include "mrs_fw_proto698_45.h"
#include "mrs_power_failure.h"
#include "mrs_srv_sta_testframe.h"
#include "mrs_srv_cco.h"
#include "mrs_transfomer_identify.h"
#include "mrs_srv_ndm_detectid.h"
#include "mrs_srv_rf.h"
#include "rf_main.h"

#if defined(MRS_SGD_INNER_VER)
#define MRS_INNER_VER_LEN 8
HI_PRV HI_U8 g_ucMrsInnerVer[MRS_INNER_VER_LEN] = {0}; 
#endif
HI_PRV HI_U32 g_ulMrsBaudRate = 0;

HI_PRV HI_U32 mrsFwInit(HI_VOID);
HI_PRV HI_U32 mrsTimerProc(HI_U32 ulTimerId);
HI_PRV HI_U32 mrsChlConnectMsgProc(HI_U16 usPort, HI_U8 ucOption);

#if (defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) || defined(PRODUCT_CFG_PRODUCT_TYPE_NDM))
HI_PRVL HI_U32 mrsMsgProc376_2Input(HI_IN HI_SYS_QUEUE_MSG_S* pstMsg);
#endif
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
HI_PRVL HI_U32 mrsMsgProcMacMsg(HI_IN HI_SYS_QUEUE_MSG_S* pstMsg);
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
HI_PRVL HI_U32 mrsMsgProcUartInput(HI_U16 usId, HI_SYS_QUEUE_MSG_S* pstMsg);
#endif
#if defined(PRODUCT_CFG_PRODUCT_TYPE_NDM)
HI_U32 mrsNdmPlcProcess(HI_IN HI_U16 usId, HI_PVOID pReqPacket, HI_U16 usReqPacketSize);
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

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
HI_PRV HI_U32 DoStaMrRxProcess(HI_IN HI_DMS_CHL_RX_S* pstRx);
#endif

#if (defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) || defined(PRODUCT_CFG_PRODUCT_TYPE_NDM))
HI_PRV HI_U32 mrsDispatchRx3762Data(HI_IN MRS_ONE_RAW_FRAME_STRU* pstFrame);
#endif
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
HI_PRV HI_U32 DoCcoMrRxProcess(HI_IN HI_DMS_CHL_RX_S* pstRx);
HI_PRV HI_U32 AppDiagChlConnectNotify (HI_U16 usPortNum, HI_BOOL bConnect);

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO_BROAD_BAND_UNIT)
HI_PRV HI_U32 DoCcoMrRxProcessBbu(HI_IN HI_DMS_CHL_RX_S* pstRx);
#endif
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_NDM)
HI_PRV HI_U32 DoNdmMrRxProcess(HI_IN HI_DMS_CHL_RX_S* pstRx);
HI_PRV HI_U32 DoNdmMrRxProcessInterConnect(HI_IN MRS_PLC_FRAME_DATA_STRU* pstRx);
HI_PRV HI_BOOL mrsIsCcdDutMode(MRS_645_FRAME_STRU* pst645Hrd);
HI_PRV HI_U32 mrsCcdDutSend(HI_PBYTE pucDatagram, HI_U16 usDatagramSize, HI_U8* pDataRealm);
#endif

HI_VOID APP_MrsTaskInit(HI_VOID);
HI_VOID APP_MrsTaskBody(HI_U32 ulParam);
HI_PRV HI_BOOL mrsAllowJoinMsgQueue(HI_U32 ulMsgId);
#if defined(MRS_SGD_INNER_VER)
HI_VOID mrsSrvInnerVerInit(HI_VOID);
#endif
#if (defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) || defined(PRODUCT_CFG_PRODUCT_TYPE_NDM))
hi_u32 mrs_rx_handle_13762(HI_DMS_CHL_RX_S *rx);
#endif
#if defined(PRODUCT_CFG_SUPPORT_MRS_DUOL_MODE_RF)
HI_U32 mrsDiagUartInit(HI_VOID);
#endif


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
    HI_U32 ret = HI_ERR_SUCCESS;

    MRS_NOT_USED(usPortNum);

    if (HI_FALSE == bConnect)
    {
        HI_SYS_QUEUE_MSG_S msg;
        msg.ulMsgId     = ID_MRS_MSG_MR_TIMER;
        msg.ulParam[0]  = MRS_CCO_TIMER_SIMU_CHL;
        ret = mrsSendMessage2Queue(&msg);
    }

    return ret;
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

#if (defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) || defined(PRODUCT_CFG_PRODUCT_TYPE_NDM))
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
#if defined(MRS_SGD_INNER_VER)	
	mrsSrvInnerVerInit();
#endif	
    mrs698Crc16Init();
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
//CJJ-HPLC-20190103
#if defined(SIGNAL_MCU)&&defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
	if (!EQUIP_DUT_IS_EQUIP_MODE())//������ǲ���ģʽ--weichao
    {
		rf_main_init();
	}
#else
 #if defined(PRODUCT_CFG_SUPPORT_MRS_DUOL_MODE_RF)
	 mrsDiagUartInit();
 #endif
#endif 
//CJJ-HPLC-20190103

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

#if (defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) || defined(PRODUCT_CFG_PRODUCT_TYPE_NDM))
hi_u32 mrs_rx_handle_13762(HI_DMS_CHL_RX_S *rx)
{
    hi_u32 ret = HI_ERR_SUCCESS;
    MRS_ONE_RAW_FRAME_STRU *frame = HI_NULL;

    mrs3762ProtoStreamInputCheck((hi_bool)rx->bCanDiscardOldData);

    /* ��ȡ��������֡���� */
    ret = mrs3762ProtoStreamInput(rx->pPayload, rx->usPayloadLen, &frame);
    if (ret == HI_ERR_SUCCESS) {
        HI_SYS_QUEUE_MSG_S msg;

        if (frame != HI_NULL) {
        	HI_U16 usOffset = 0;
		HI_S16 usLen = (HI_S16)frame->usDatagramSize;
		while(usLen > 0)
		{
            HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1000, HI_DIAG_MT("[CCO-MR]-RX"), frame->pucDatagram, frame->usDatagramSize);
			usLen -= 128;
			usOffset += 128;
		}
            mrsDfxChlUartOkFrameCnt(frame->usDatagramSize, HI_FALSE);    /* ���ڽ�������֡ͳ��(��Ϣ����) */
        }

        (hi_void)memset_s(&msg, sizeof(msg), 0, sizeof(msg));
        msg.ulMsgId = EN_MRS_FW_MSG_376_2_FRAME_INPUT;
        msg.ulParam[0] = (uintptr_t)frame;

        ret = mrsSendMessage2Queue(&msg);
        if (ret != HI_ERR_SUCCESS) {
            mrsDfxChlUartErrFrameCnt(HI_FALSE);   /* ���ڽ�������֡����ͳ��(��Ϣ����) */
            HI_MDM_ReportSysEvent(EID_APP_SEND_MSG_FAIL);
            mrsToolsFree(frame);
        }
    } else if (ret != HI_ERR_CONTINUE) {
         mrsDfxChlUartErrRx(rx->usPayloadLen, ret, HI_FALSE); /* ���ڽ��մ�����(��Ϣ����) */
    }

    return ret;
}
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
// CCO����MR���ݴ���
HI_U32 DoCcoMrRxProcess(HI_IN HI_DMS_CHL_RX_S* pstRx)
{
    return mrs_rx_handle_13762(pstRx);
}



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
    HI_U32 ulRet = HI_ERR_SUCCESS;
    MRS_ONE_RAW_FRAME_STRU stFrame = {0};

    if (mrsToolsIsIICollector())
    {
        ulRet = mrsCltiiSearchRx(pstRx);
        if (HI_ERR_SUCCESS == ulRet)
        {
            return ulRet;
        }
    }

    // �յ�����֮����³������״̬
    mrsStaUpdateQueueStatus();

    mrs645ProtoStreamInputCheck((HI_BOOL)pstRx->bCanDiscardOldData, ID_MRS_UART_645BUF);

    // ��ȡ��������֡����
    if (MRS_SRV_PROTO_645 != mrsStaGetFrmProto())
    {
        ulRet = mrs645ProtoStream2Buffer(pstRx->pPayload, pstRx->usPayloadLen, ID_MRS_UART_645BUF);
        if (HI_ERR_SUCCESS != ulRet)
        {
            mrsDfxChlUartErrRx(pstRx->usPayloadLen, ulRet, HI_FALSE); // ���ڽ��մ�����(��Ϣ����)
        }

        return ulRet;
    }

    ulRet = mrs645ProtoStreamInput(pstRx->pPayload, pstRx->usPayloadLen, &stFrame, ID_MRS_UART_645BUF);
    if(HI_ERR_SUCCESS == ulRet)
    {
        if (mrsToolsIsIICollector() && (HI_ERR_SUCCESS == mrsJudgeEqiupMode(stFrame.pucDatagram, stFrame.usDatagramSize)))
        {
            mrsToolsFree(stFrame.pucDatagram);
            return ulRet;
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

		HI_U16 usOffset = 0;
		HI_S16 usLen = (HI_S16)stFrame.usDatagramSize;
		while(usLen > 0)
		{
			HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1001, HI_DIAG_MT("[STA-MR]-RX"),stFrame.pucDatagram + usOffset,(usLen > 128)?128:usLen);
			usLen -= 128;
			usOffset += 128;
		}
  

            mrsDfxChlUartOkFrameCnt(stFrame.usDatagramSize, HI_FALSE);    // ���ڽ�������֡ͳ��(��Ϣ����)

            (hi_void)memcpy_s(pstFrame, sizeof(MRS_ONE_RAW_FRAME_STRU), &stFrame, sizeof(MRS_ONE_RAW_FRAME_STRU));
            stMsg.ulMsgId = EN_MRS_FW_MSG_645_FRAME_INPUT;

            // ������Ϣ�� MRS����, ������Ϣ�ַ�����
            ulRet = mrsSendMessage2Queue(&stMsg);
            if (HI_ERR_SUCCESS != ulRet)
            {
                mrsToolsFree(stFrame.pucDatagram);

                mrsDfxChlUartErrFrameCnt(HI_FALSE);   // ���ڽ�������֡����ͳ��(��Ϣ����)
                HI_MDM_ReportSysEvent(EID_APP_SEND_MSG_FAIL);
            }
        }
    }
    else if(HI_ERR_CONTINUE == ulRet)
    {
        // �������ݻ�ۺ�, �����ܺͲ���1֡����, ��Ҫ�����ȴ�����
    }
    else
    {
        mrsDfxChlUartErrRx(pstRx->usPayloadLen, ulRet, HI_FALSE); // ���ڽ��մ�����(��Ϣ����)
    }

    return ulRet;
}
#endif

// ����������MR���ݴ���
#if defined(PRODUCT_CFG_PRODUCT_TYPE_NDM)
HI_U32 DoNdmMrRxProcess(HI_IN HI_DMS_CHL_RX_S* pstRx )
{
    HI_U32 ret = HI_ERR_SUCCESS;
    MRS_ONE_RAW_FRAME_STRU stFrame = {0};

    HI_MDM_NdmSerialInterfaceRxLedProc();

    mrs645ProtoStreamInputCheck((HI_BOOL)pstRx->bCanDiscardOldData, ID_MRS_UART_645BUF);

    // ��ȡ��������֡����
    ret = mrs645ProtoStreamInput(pstRx->pPayload, pstRx->usPayloadLen, &stFrame, ID_MRS_UART_645BUF);
    if(HI_ERR_SUCCESS == ret)
    {
        MRS_645_FRAME_STRU st645Hrd = { 0 };
        HI_NDM_645_FRAME_S * buf_645 = HI_NULL;
        HI_U8 MacAddr[HI_PLC_RELAY_NUM_MAX][HI_PLC_MAC_ADDR_LEN];

        HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1002, HI_DIAG_MT("[CCD-MR]-RX"), stFrame.pucDatagram, stFrame.usDatagramSize);

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

            (hi_void)memcpy_s(buf_645->aucFrame, sizeof(buf_645->aucFrame), stFrame.pucDatagram, stFrame.usDatagramSize);
            buf_645->usFrameSize = stFrame.usDatagramSize;

            (hi_void)memset_s(MacAddr[0], HI_PLC_MAC_ADDR_LEN, 0xFF, HI_PLC_MAC_ADDR_LEN);// ʹ�ù㲥��ַ
            if (mrsToolsNormalAddr(st645Hrd.ucAddr))
            {
                if (st645Hrd.ucAddr[0] != 0x00)
                {
                    (hi_void)memcpy_s(MacAddr[0], HI_PLC_MAC_ADDR_LEN, st645Hrd.ucAddr, HI_PLC_MAC_ADDR_LEN);
                    mrsHexInvert(MacAddr[0], HI_PLC_MAC_ADDR_LEN);
                    (hi_void)memcpy_s(MacAddr[1], HI_PLC_MAC_ADDR_LEN, MacAddr[0], HI_PLC_MAC_ADDR_LEN);
                }

                // ����645����Diag��
                ret = HI_MDM_SendNdmReqFrame(HI_NDM_645_FRAME,(HI_U8 *)buf_645,sizeof(HI_NDM_645_FRAME_S),MacAddr,0);
            }
            else
            {
                // ����645����Diag��
                ret = HI_MDM_SendNdmReqFrame(HI_NDM_645_FRAME,(HI_U8 *)buf_645,sizeof(HI_NDM_645_FRAME_S),MacAddr,0);
            }

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
    HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1019, HI_DIAG_MT("dut send"), pucDatagram, usDatagramSize);

    stCtrl.usDULink = HI_FALSE;

    return HI_MDM_SendNdmPing(HI_MDM_NDM_PING_ID_0, pucDatagram, usDatagramSize, &stCtrl);
}

#endif

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

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO_BROAD_BAND_UNIT)
        // ���NV��֧�ֽ��հ汾����������Ӧ����Ϊҵ�񴮿�
        if (mrsToolIsBbu() && (!mrsIsSimuChlOpened()))
        {
            // ���հ汾����������Ӧ && ҵ�񴮿�
            ret = DoCcoMrRxProcessBbu(pstRx);
        }
        else
#endif
        {
            // ģ��ͨ�� || �ǽ��հ汾Լ����������Ӧ
            ret = DoCcoMrRxProcess(pstRx);
        }
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
        MRS_WaitSem(EN_APP_SEM_SRV_UART,HI_SYS_WAIT_FOREVER);
        ret = DoStaMrRxProcess(pstRx);
        MRS_SignalSem(EN_APP_SEM_SRV_UART); // �ź���������
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_NDM)
    if (HI_MDM_IsNdmMeterAccess()) {
        ret = DoNdmMrRxProcess(pstRx);
    } else {
        ret = mrs_rx_handle_13762(pstRx);
    }
#endif
    }
    else if (HI_DMS_CHL_MAC_PORT_APP == usPort)
    {
    	HI_U16 usOffset = 0;
	HI_S16 usLen = (HI_S16)pstRx->usPayloadLen;
        HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1003, HI_DIAG_MT("PLC_RX MAC"), pstRx->ucSrcMac, 6);
	HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1003, HI_DIAG_MT("PLC_RX MAC"), pstRx, sizeof(HI_DMS_CHL_RX_S));
	while(usLen > 0)
	{
        HI_DIAG_LOG_BUF(0, HI_DIAG_MT("PLC_RX FRAME"),pstRx->pPayload + usOffset,(usLen > 128)?128:usLen);
		usLen -= 128;
		usOffset += 128;
	}
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
#if defined(PRODUCT_CFG_SUPPORT_MRS_DUOL_MODE_RF)
	else if(HI_DMS_CHL_UART_PORT_DIAG == usPort)
	{
#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
		mrsStaRFRecvDealFun(pstRx);
#endif
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
		mrsCcoRFRecvDealFun(pstRx);
#endif

	}
#endif
    return ret;
}

//NDM����plc֡����
#if defined(PRODUCT_CFG_PRODUCT_TYPE_NDM)

#define MR_CCO_NDM_INTERCONNECT_FRAME_HEAD_SIZE 3
HI_U32 mrsNdmPlcProcess(HI_IN HI_U16 usId, HI_PVOID pReqPacket, HI_U16 usReqPacketSize)
{
	HI_U32 ret = HI_ERR_SUCCESS;

    HI_UNREF_PARAM(usId);
    HI_UNREF_PARAM(usReqPacketSize);

    MRS_PLC_FRAME_DATA_STRU *pstRx = (MRS_PLC_FRAME_DATA_STRU *)pReqPacket;
	if(!mrsToolsBroadCastMac(pstRx->ucMac)) //�㲥֡����
	{
		ret = DoNdmMrRxProcessInterConnect(pstRx);
	}

	return ret;
}

HI_PRV HI_U32 DoNdmMrRxProcessInterConnect(HI_IN MRS_PLC_FRAME_DATA_STRU* pstRx)
{
    HI_U32 ret = HI_ERR_SUCCESS;

    if(!pstRx)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    switch(pstRx->usId)
    {
    case ID_MRS_CMD_NDM_CTRL_CCO_PLC:
    {


        if(pstRx->usPayloadLen < MR_CCO_NDM_INTERCONNECT_FRAME_HEAD_SIZE)
        {
            return HI_ERR_INVALID_PARAMETER;
        }
        /*0x0040-������-CCO���С����б��ĸ�ʽ˵��*/
        /*��        �ֽں�  ��С(����)           */
        /*Э������  0       8                    */
        /*���ĳ���  1-2     16                   */
        /*��������  3-N                          */
        ndm_interconnect_info *pndm_interconnect_info = ndm_get_interconnect_info();
        HI_PBYTE pstInterConnectFrame = (HI_PBYTE)pstRx->pucPayload;
        HI_U32 ulFrameLen = 0;
        HI_U8 ucProtoType = 0;

        ucProtoType = *pstInterConnectFrame;
        pstInterConnectFrame++;
        MRS_NOT_USED(ucProtoType);
        ulFrameLen =  *pstInterConnectFrame;
        pstInterConnectFrame++;
        ulFrameLen +=( *pstInterConnectFrame)<<8;
        pstInterConnectFrame++;

        HI_U32 ulLen = ulFrameLen + sizeof(HI_NDM_INTERCONNECT_CMD_STRU);
        HI_NDM_INTERCONNECT_CMD_STRU *pstInterConnectCmd = (HI_NDM_INTERCONNECT_CMD_STRU*)mrsToolsMalloc(ulLen);
        if(!pstInterConnectCmd)
        {
            return HI_ERR_NOT_ENOUGH_MEMORY;
        }
        (hi_void)memset_s(pstInterConnectCmd, ulLen, 0,ulLen);

        pstInterConnectCmd->cmd_id = pndm_interconnect_info->cmd_id;
        pstInterConnectCmd->ulDataSize = ulFrameLen;
        (hi_void)memcpy_s(pstInterConnectCmd->aucData,ulFrameLen,pstInterConnectFrame,ulFrameLen);

        ret = HI_MDM_ReportPacket(HI_ND_NDM_CMD_PACKET, HI_DIAG_CMD_INSTANCE_DEFAULT, (HI_PBYTE)pstInterConnectCmd, (HI_U16)ulLen, HI_TRUE);
        HI_DIAG_LOG_BUF(__LINE__, HI_DIAG_MT("DoNdmMrRxProcessInterConnect ID_MRS_CMD_NDM_CTRL_CCO_PLC"),pstInterConnectCmd ,ulLen);

    	mrsToolsFree(pstInterConnectCmd);
    }
        break;
    case ID_MRS_CMD_DATA_TRANSMIT_AFN13_PLC:
    {
        if(pstRx->usPayloadLen < sizeof(MRS_DATA_TRANSMIT_AFN13_UP_STRU))
        {
            return HI_ERR_INVALID_PARAMETER;
        }
        MRS_DATA_TRANSMIT_AFN13_UP_STRU *pstInterConnectFrame = (MRS_DATA_TRANSMIT_AFN13_UP_STRU*)pstRx->pucPayload;
        HI_U32 ulLen = pstInterConnectFrame->data_len+ sizeof(MRS_DATA_TRANSMIT_AFN13_UP_STRU);
        HI_NDM_INTERCONNECT_CMD_STRU *pstInterConnectCmd = (HI_NDM_INTERCONNECT_CMD_STRU*)mrsToolsMalloc(ulLen);
        if(!pstInterConnectCmd)
        {
            return HI_ERR_NOT_ENOUGH_MEMORY;
        }
        (hi_void)memset_s(pstInterConnectCmd, ulLen, 0,ulLen);

        ndm_interconnect_info *pndm_interconnect_info = ndm_get_interconnect_info();
        pstInterConnectCmd->cmd_id = pndm_interconnect_info->cmd_id;
        pstInterConnectCmd->ulDataSize = pstInterConnectFrame->data_len;
        (hi_void)memcpy_s(pstInterConnectCmd->aucData,pstInterConnectFrame->data_len,pstInterConnectFrame->data,pstInterConnectFrame->data_len);

        ret = HI_MDM_ReportPacket(HI_ND_NDM_CMD_PACKET, HI_DIAG_CMD_INSTANCE_DEFAULT, (HI_PBYTE)pstInterConnectCmd, (HI_U16)ulLen, HI_TRUE);
        HI_DIAG_LOG_BUF(0, HI_DIAG_MT("DoNdmMrRxProcessInterConnect ID_MRS_CMD_DATA_TRANSMIT_AFN13_PLC"),(HI_PBYTE)pstInterConnectCmd, ulLen);

    	mrsToolsFree(pstInterConnectCmd);
    }
        break;

    default:
        break;
    }

    return ret;
}
#endif

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


#if (defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) || defined(PRODUCT_CFG_PRODUCT_TYPE_NDM))
HI_PRVL HI_U32 mrsMsgProc376_2Input(HI_IN HI_SYS_QUEUE_MSG_S* pstMsg)
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
    ret = mrsDispatchRx3762Data(pstFrame);
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
HI_PRVL HI_U32 mrsMsgProcUartInput(HI_U16 usId, HI_SYS_QUEUE_MSG_S* pstMsg)
{
    MRS_ONE_RAW_FRAME_STRU* pstFrame = (MRS_ONE_RAW_FRAME_STRU*)&pstMsg->ulParam[0];

    mrsDfxChlUartRx(pstFrame->pucDatagram, pstFrame->usDatagramSize, HI_TRUE);    // ���ڽ���ͳ��(��Ϣ����)
    MRS_MSG_NODE_STRU * pstNode = mrsFindMsgNode(usId);

    if (pstNode != HI_NULL && pstNode->pfnMsg)
    {
        MRS_MSG_MR_645_DATA_STRU stData = {0};
        stData.pucData = pstFrame->pucDatagram;
        stData.usDataSizse = pstFrame->usDatagramSize;
        pstNode->pfnMsg(usId, &stData);

        mrsDfxChlUartOkFrameCnt(pstFrame->usDatagramSize, HI_TRUE);    // ���ڽ�������֡ͳ��(��Ϣ����)
        mrsToolsFree(pstFrame->pucDatagram);
        return HI_ERR_SUCCESS;
    }

    mrsDfxChlUartErrRx(pstFrame->usDatagramSize, HI_ERR_FAILURE, HI_TRUE); // ���ڽ��մ�����(��Ϣ����)
    mrsDfxChlUartErrFrameCnt(HI_TRUE);   // ���ڽ�������֡����ͳ��(��Ϣ����)

    mrsToolsFree(pstFrame->pucDatagram);

    return HI_ERR_FAILURE;
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
#if (defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) || defined(PRODUCT_CFG_PRODUCT_TYPE_NDM))
    case EN_MRS_FW_MSG_376_2_FRAME_INPUT:
        ret = mrsMsgProc376_2Input(pstMsg);
        break;
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
    case EN_MRS_FW_MSG_645_FRAME_INPUT:
        ret = mrsMsgProcUartInput(ID_MRS_MSG_MR_DATA_645, pstMsg);
        break;

    case EN_MRS_FW_MSG_698_FRAME_INPUT:
        ret = mrsMsgProcUartInput(ID_MRS_MSG_MR_DATA_698_45, pstMsg);
        break;

    case EN_MRS_FW_MSG_TRANSPARENT_FRAME_INPUT:
        ret = mrsMsgProcUartInput(ID_MRS_MSG_MR_DATA_TRANSPARENT, pstMsg);
        break;
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
#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
     //����ͣ���ϱ��¼�
    case EN_MRS_FW_MSG_POWER_FAILURE:
        ret = mrsHandlePowerFailure(pstMsg);
        break;
#endif
#if defined(PRODUCT_CFG_PRODUCT_TYPE_NDM)
    case EN_MRS_FW_MSG_DETECTID:
        ret = mrs_ndm_handle_detect_id(pstMsg);
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
		#if defined (PRODUCT_OPTIMIZE_MATTEO)
        ret = mrsNodeStateChgMsgProc(pstMsg);
		#endif
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

    case EN_MRS_FW_MSG_PAUSE_RESUME_DETECT:
        mrsResumeDetectRmProc();
        break;

    case EN_MRS_FW_MSG_TOPO_CHANGE_NOTIFY:
        mrsCcoTopoStatusChgProc(pstMsg);
        break;
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
    case EN_MRS_FW_MSG_CLTII_EVENT_MON:
        mrsCltiiEventMsgProc(pstMsg);
        break;

#if defined(PRODUCT_CFG_SUPPORT_MRS_UPG_DEMO)
    case EN_MRS_FW_MSG_MDM_INPUT:
        mrsCltIUpgMsgProc(pstMsg);
        break;
#endif
#endif

    case EN_MRS_FW_MSG_DIAG_CMD_MSG:
        mrsDiagCmdMsgProc(pstMsg);
        break;

    case EN_MRS_FW_MSG_TF:
        #if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) || defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
        mrsTfHandleMsg(pstMsg);
        #endif
        break;

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

		//CJJ-HPLC:2018-9-7
		#if defined(SIGNAL_MCU)&&defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
	    case ID_RF_WIRE_LESS_REPORT:
	        rf_main_task(pstMsg->ulParam);
	        break;
		#endif		
    //CJJ-HPLC:2018-9-7

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
            stFrameData.usId          = usId?usId:usNodeId;
            stFrameData.usPayloadLen  = pstRx->usPayloadLen;
            stFrameData.pucPayload    = pstRx->pPayload;
            stFrameData.bIsMyNetFrame = pstRx->bIsMyNetFrame;
            stFrameData.ucRcvFreq     = pstRx->ucRcvFreq;
            stFrameData.ulRcvNid      = pstRx->ulRcvNid;

            if(mrsToolsBroadCastMac(pstRx->ucDestMac))
            {
                stFrameData.is_bc_frame = HI_TRUE;
                mrsDfxPlcBcRx();  // ͳ�ƽ��չ㲥֡
            }
#if defined(PRODUCT_CFG_SUPPORT_MRS_DUOL_MODE_RF)
            else if (mrsToolsLocalBroadCastMac(pstRx->ucDestMac))
            {
                stFrameData.is_local_bc_frame = HI_TRUE;
            }
#endif
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

// 376.2���պ�� �ַ�����
#if (defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) || defined(PRODUCT_CFG_PRODUCT_TYPE_NDM))
HI_PRV HI_U32 mrsDispatchRx3762Data(HI_IN MRS_ONE_RAW_FRAME_STRU* pstFrame)
{
    HI_U32 ret  = HI_ERR_SUCCESS;
    MRS_MSG_NODE_STRU* pstNode = mrsFindMsgNode(ID_MRS_MSG_MR_DATA);

    if (pstNode)
    {
        if(pstNode->pfnMsg)
        {
            MRS_MSG_MR_3762_DATA_STRU stData;

            (hi_void)memset_s(&stData, sizeof(stData), 0, sizeof(stData));
            stData.pucData = pstFrame->pucDatagram;
            stData.usDataSizse = pstFrame->usDatagramSize;

            pstNode->pfnMsg(ID_MRS_MSG_MR_DATA, (HI_PVOID)&stData);
        }
    }
    else
    {
        ret = HI_ERR_NOT_FOUND;
    }

    return ret;
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

//CJJ-HPLC-20190103
#if defined(PRODUCT_CFG_SUPPORT_MRS_DUOL_MODE_RF)
#if defined(SIGNAL_MCU)
    if (ucChlType == HI_DMS_CHL_UART_PORT_APP)
    {
        //DO
    }
#else
		if ((ucChlType == HI_DMS_CHL_UART_PORT_APP) || (ucChlType ==HI_DMS_CHL_UART_PORT_DIAG))
    {
        //DO
    }
#endif
#else
if (ucChlType == HI_DMS_CHL_UART_PORT_APP)
{
		//DO
}
#endif
//CJJ-HPLC-20190103


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
    	HI_U16 usOffset = 0;
	HI_S16 usLen = (HI_S16)pstFrameData->usDataSize;
        HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1004, HI_DIAG_MT("TX PLC OK"), pstFrameAddr->ucDestMac, HI_PLC_MAC_ADDR_LEN);
	while(usLen > 0)
	{
		HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1009, HI_DIAG_MT("TX PLC Payload"),pstFrameData->ucData + usOffset,(usLen > 128)?128:usLen);
		usLen -= 128;
		usOffset += 128;
	}
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

	if(pstFrameData == HI_NULL)
	{
		return HI_ERR_INVALID_PARAMETER;
	}
    pstAddr = (HI_DMS_CHL_TX_ADDR_S *)mrsToolsMalloc(sizeof(HI_DMS_CHL_TX_ADDR_S));
    if (!pstAddr)
    {
        return HI_ERR_NO_MORE_MEMORY;
    }

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
    pstAddr->usSn       = pstFrameData->ucOption;
    pstAddr->ucDuration = 0;
    pstAddr->ucMaxResendCount = 0;
    pstAddr->ucPhase = 0;
    pstAddr->bIsBroadCast = pstFrameData->is_broadcast_send;

#if (defined(PRODUCT_CFG_PRODUCT_TYPE_STA) && defined(PRODUCT_CFG_SUPPORT_TEST_CONCENTRATOR))
    pstAddr->ucCtrl = HI_DMS_CHL_TX_CTRL_FORCE;
    (hi_void)memset_s(pstAddr->ucDestMac, HI_PLC_MAC_ADDR_LEN, 0xFF, HI_PLC_MAC_ADDR_LEN);
    mrsStaAdjustTestModeSnId(&(pstAddr->usSn));
#endif

    //Ĭ�����ȼ�Ϊ3(3Ϊ������ȼ�,0Ϊ������ȼ�)
    pstAddr->ucPrio = 3;
    if ((pstFrameData->usId >= ID_MRS_CMD_GET_METERLIST) && (pstFrameData->usId <= ID_MRS_CMD_STOP_SEARCH_METER))
    {
        pstAddr->ucPrio = 2;
    }

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
    if(HI_TRUE == deviceState->device_power_failure)
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

#endif

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
HI_U32 MRS_ChangeBaudRate(HI_U32 ulRate, HI_U8 ucParity)
{
    HI_U32 ret = HI_ERR_SUCCESS;

    if (g_ulMrsBaudRate != ulRate)
    {
        HI_DMS_NV_FTM_DEVICE_CFG_S stDevCfg;

        g_ulMrsBaudRate = ulRate;

        (hi_void)memset_s(&stDevCfg, sizeof(stDevCfg), 0, sizeof(stDevCfg));
        ret = HI_MDM_ReadDevCfg(&stDevCfg);
        if(ret == HI_ERR_SUCCESS)
        {
            HI_FTM_DEVICE_UART_CFG_S *pstUartCfg = HI_NULL;
#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
            pstUartCfg = &stDevCfg.astDevItem[HI_NV_FTM_DEVICE_CFG_STA].astUartCfg[HI_DMS_CHL_UART_PORT_APP];
#else
        	pstUartCfg = &stDevCfg.astDevItem[HI_NV_FTM_DEVICE_CFG_CCO].astUartCfg[HI_DMS_CHL_UART_PORT_APP];
#endif
            pstUartCfg->ulBaudRate = ulRate;
        	pstUartCfg->ucParity = ucParity;
#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
            ret = HI_MDM_ChlUartPortRefresh(pstUartCfg, HI_FTM_CHIP_TYPE_3911T, HI_DMS_CHL_UART_PORT_APP);
#else
        	ret = HI_MDM_ChlUartPortRefresh(pstUartCfg, HI_FTM_CHIP_TYPE_3911C, HI_DMS_CHL_UART_PORT_APP);
#endif
            if (ret == HI_ERR_SUCCESS)
            {
                ret = mrsFwChlInit(&g_stMrsChlPort, HI_DMS_CHL_UART_PORT_APP);
            }
        }
    }

    return ret;
}


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
    		HI_U16 usOffset = 0;
		HI_S16 usLen = (HI_S16)usDataSize;
		
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
	if(HI_DMS_CHL_UART_PORT_DIAG == ucChlType)
	{
        HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1014, HI_DIAG_MT("[CCO-MR]-DIAG_TX"), pucData, usDataSize);
	}
	else
	{
		while(usLen > 0)
		{
			HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1005, HI_DIAG_MT("[CCO-MR]-TX"),pucData + usOffset,(usLen > 128)?128:usLen);
			usLen -= 128;
			usOffset += 128;
		}    
	}
    #endif

    #if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
	if(HI_DMS_CHL_UART_PORT_DIAG == ucChlType)
	{
        HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1014, HI_DIAG_MT("[STA-MR]-DIAG_TX"), pucData, usDataSize);
	}
	else
	{
		while(usLen > 0)
		{
			HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1006, HI_DIAG_MT("[STA-MR]-TX"),pucData + usOffset,(usLen > 128)?128:usLen);
			usLen -= 128;
			usOffset += 128;
		}
	}
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

#if defined(MRS_SGD_INNER_VER)
HI_VOID mrsSrvSetInnerVerToNv(HI_VOID)
{
	HI_U32 ret;
	NV_MRS_USER_VER_S stUserVer;//={0}; //weichao

	HI_U8 aucBuf[HI_USER_VER_LENGTH] = {0};
	HI_U8 temp = 0;

	//���ͻ��汾��Ϣ
   	 ret = HI_MDM_NV_Read(ID_NV_MRS_USER_VER, &stUserVer, sizeof(NV_MRS_USER_VER_S));
	if(ret != HI_ERR_SUCCESS)
	{
		return;
	}
	aucBuf[0] = 'V';
    for(HI_U8 i = 0; i < 1; i++)
    {
    	temp = (g_ucMrsInnerVer[i] & 0xF0) >> 4;
		if(temp >= 0 && temp <=9)
		{
			aucBuf[1+i*2] = '0' + temp;
		}
		else
		{
			aucBuf[1+i*2] = 'A' + temp - 10;
		}
		temp = (g_ucMrsInnerVer[i] & 0x0F);
		if(temp >= 0 && temp <=9)
		{
			aucBuf[1+i*2+1] = '0' + temp;
		}
		else
		{
			aucBuf[1+i*2+1] = 'A' + temp - 10;
		}
    }
	aucBuf[3] = 'S';
	for(HI_U8 i = 1; i < 2; i++)
    {
    	temp = (g_ucMrsInnerVer[i] & 0xF0) >> 4;
		if(temp >= 0 && temp <=9)
		{
			aucBuf[2+i*2] = '0' + temp;
		}
		else
		{
			aucBuf[2+i*2] = 'A' + temp - 10;
		}
		temp = (g_ucMrsInnerVer[i] & 0x0F);
		if(temp >= 0 && temp <=9)
		{
			aucBuf[2+i*2+1] = '0' + temp;
		}
		else
		{
			aucBuf[2+i*2+1] = 'A' + temp - 10;
		}
    }
	aucBuf[6] = 'L';
	aucBuf[7] = g_ucMrsInnerVer[2];
	aucBuf[8] = g_ucMrsInnerVer[3];
	aucBuf[9] = 'T';
	for(HI_U8 i = 4; i < 6; i++)
    {
    	temp = (g_ucMrsInnerVer[i] & 0xF0) >> 4;
		if(temp >= 0 && temp <=9)
		{
			aucBuf[i*2 +2] = '0' + temp;
		}
		else
		{
			aucBuf[i*2 +2] = 'A' + temp - 10;
		}
		temp = (g_ucMrsInnerVer[i] & 0x0F);
		if(temp >= 0 && temp <=9)
		{
			aucBuf[i*2 + 3] = '0' + temp;
		}
		else
		{
			aucBuf[i*2 + 3] = 'A' + temp - 10;
		}
    }
	/*
	aucBuf[14] = 'R';
	for(HI_U8 i = 6; i < 8; i++)
    {
    	temp = (g_ucMrsInnerVer[i] & 0xF0) >> 4;
		if(temp >= 0 && temp <=9)
		{
			aucBuf[i*2 +3] = '0' + temp;
		}
		else
		{
			aucBuf[i*2 +3] = 'A' + temp - 10;
		}
		temp = (g_ucMrsInnerVer[i] & 0x0F);
		if(temp >= 0 && temp <=9)
		{
			aucBuf[i*2 + 4] = '0' + temp;
		}
		else
		{
			aucBuf[i*2 + 4] = 'A' + temp - 10;
		}
    }*/
		if(memcmp(stUserVer.aucUserVersion, aucBuf,HI_USER_VER_LENGTH) != 0)
		{
			(hi_void)memcpy_s(stUserVer.aucUserVersion, HI_USER_VER_LENGTH,aucBuf,HI_USER_VER_LENGTH);
			HI_MDM_NV_Write(ID_NV_MRS_USER_VER,&stUserVer, sizeof(NV_MRS_USER_VER_S));
		}
}
HI_VOID mrsSrvInnerVerInit(HI_VOID)
{
	HI_U8 ucChipType = 0;
	//HI_U8 ucBase = 0;
	//HI_U8 ucVerNo = 0;
	//HI_U16 usFun = 0;//weichao
	HI_PCSTR pChar = (HI_PCSTR)LOCAL_VER;
		HI_MDM_SYS_INFO_S *pstVerInfo = HI_NULL;
	
    	pstVerInfo = (HI_MDM_SYS_INFO_S *)HI_MDM_GetSysInfo();
	if (pstVerInfo == HI_NULL)
    	{
        	HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_015, HI_DIAG_MT("mrsSrvInnerVerInit Fail-"));
        	return ;
    	}
	
	ucChipType =  (HI_U8)((pstVerInfo->aucVer[0]/100 - 1) % 9 + 1);
	//ucBase = (HI_U8)(pstVerInfo->aucVer[1]% 10);
	//ucBase = ucBase * 10 + (pstVerInfo->ausVer[1]%100)/10;
	//ucVerNo = pstVerInfo->ausVer[1]%10;
	g_ucMrsInnerVer[0] = (HI_U8)((ucChipType << 5) | BASE_VER);
	g_ucMrsInnerVer[1] = (HI_U8)((CSG_STG_VER << 6) | VER_NO);

	//usFun = (HI_U16)FUN_CODE;
	//mrsToolsMemcpy_s(g_ucMrsInnerVer+2, 2, &usFun, 2);
	(hi_void)memcpy_s(g_ucMrsInnerVer+2, 2,pChar,2);
	if (HI_MDM_IsProductCon(HI_MDM_GetProductType()))
		g_ucMrsInnerVer[4] = 0;
	else if (HI_MDM_IsProductMeter(HI_MDM_GetProductType()))
		g_ucMrsInnerVer[4] = 0x01;
	else if (HI_MDM_IsProductTSta(HI_MDM_GetProductType()))
		g_ucMrsInnerVer[4] = 0x02;
	else if (HI_MDM_IsProductMeterI(HI_MDM_GetProductType()))
		g_ucMrsInnerVer[4] = 0x03;
	else if (HI_MDM_IsProductMeterII(HI_MDM_GetProductType()))
		g_ucMrsInnerVer[4] = 0x04;
	else if (HI_MDM_IsProductNdm(HI_MDM_GetProductType()))
		g_ucMrsInnerVer[4] = 0x05;
	mrsSrvSetInnerVerToNv();
}

HI_U32 mrsSrvGetInnerVer(HI_U8 aucVer[8])
{
	if(!aucVer)
	{
		return HI_ERR_FAILURE;
	}
	(hi_void)memcpy_s(aucVer,MRS_INNER_VER_LEN, g_ucMrsInnerVer,MRS_INNER_VER_LEN);

	return HI_ERR_SUCCESS;
}
#endif

#if defined(PRODUCT_CFG_SUPPORT_MRS_DUOL_MODE_RF)
HI_U32 mrsDiagUartInit(HI_VOID)
{
	HI_U32	ret = HI_ERR_SUCCESS;
	HI_S32 fd;
 
	HI_UART_INIT_TYPE_S stUrtInitCfg = {0};
	HI_DMS_NV_FTM_DEVICE_CFG_S stDevCfg;
	mrsToolsMemZero_s(&stDevCfg,sizeof(HI_DMS_NV_FTM_DEVICE_CFG_S),sizeof(HI_DMS_NV_FTM_DEVICE_CFG_S));
	HI_FTM_DEVICE_UART_CFG_S *pstUartCfg = HI_NULL;

	HI_MDM_UART_Getfd(HI_DMS_CHL_UART_PORT_DIAG, &fd);
	stUrtInitCfg.baud_rate = EN_SAL_UART_BAUD_RATE_9600;
	stUrtInitCfg.data_bits = HI_UART_DATA8BIT;
	stUrtInitCfg.stop_bits = HI_UART_STOP1;
	stUrtInitCfg.parity = HI_UART_EVEN;

	HI_MDM_UART_Close(fd);

		ret = HI_MDM_UART_Open(fd,HI_UART_485);
		if(ret==HI_ERR_SUCCESS)
		{
				(HI_VOID)HI_MDM_UART_Ioctl(fd,UART_CFG_SET_ATTR, (HI_U32)(&stUrtInitCfg));
		}
	HI_MDM_ReadDevCfg(&stDevCfg);
	pstUartCfg = mrsToolsMalloc(sizeof(HI_FTM_DEVICE_UART_CFG_S));
	if (!pstUartCfg)
	{
		return HI_ERR_MALLOC_FAILUE;
	}
	mrsToolsMemZero_s(pstUartCfg,sizeof(HI_FTM_DEVICE_UART_CFG_S), sizeof(HI_FTM_DEVICE_UART_CFG_S));

	pstUartCfg = &stDevCfg.astDevItem[HI_NV_FTM_DEVICE_CFG_STA].astUartCfg[HI_DMS_CHL_UART_PORT_DIAG];
	pstUartCfg->ulBaudRate = EN_SAL_UART_BAUD_RATE_9600;
	pstUartCfg->ucParity = HI_UART_EVEN;
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
	HI_MDM_ChlUartPortRefresh(pstUartCfg, HI_FTM_CHIP_TYPE_3911C, HI_DMS_CHL_UART_PORT_DIAG);
#else
	HI_MDM_ChlUartPortRefresh(pstUartCfg, HI_FTM_CHIP_TYPE_3911T, HI_DMS_CHL_UART_PORT_DIAG);
#endif
	mrsFwChlInit(&g_stMrsChlPort, HI_DMS_CHL_UART_PORT_DIAG);
	mrsToolsFree(pstUartCfg);

	return HI_ERR_SUCCESS;
}

#endif
