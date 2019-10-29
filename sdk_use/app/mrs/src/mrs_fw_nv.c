//*****************************************************************************
//
//                  ��Ȩ���� (C), 1998-2011, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mrs_fw_nv.c
//  �� �� ��   : V1.0
//  ��    ��   : fengxiaomin/f00209182
//  ��������   : 2014-7-17
//  ��������   : NV����ش��������ӿڶ���
//
//  �����б�   : TODO: ...
//  �޸���ʷ   :
//  1.��    �� : 2014-7-17
//    ��    �� : fengxiaomin/f00209182
//    �޸����� : �����ļ�
//
//*****************************************************************************

#include "mrs_common.h"
#include "mrs_cmd_msg.h"
#include "mrs_fw_proto376_2_echo.h"
#include "mrs_fw_tools.h"
#include "mrs_srv_common.h"
#include "mrs_srv_res.h"
#include "app_nv.h"
#include "mrs_srv_cco_macmap.h"
#include "mrs_srv_parallel_cco.h"
#include "mrs_srv_readmeter_manage.h"
#include "mrs_srv_cco_remote_upg.h"
#include "mrs_srv_list.h"
#include "mrs_srv_cco_event.h"
#include "mrs_srv_cco.h"
#include "hi_mdm.h"
#include "mrs_srv_sta_queue.h"
#include "mrs_srv_parallel_sta.h"
#include "mrs_srv_sta_event.h"
#include "mrs_srv_sta.h"
#include "mrs_srv_sta_searchmeter.h"
#include "mrs_srv_clti_searchmeter.h"
#include "mrs_srv_baudrate_manage.h"
#include "mrs_fw_n.h"
#include "mrs_fw_nv.h"
#include "mrs_srv_collector.h"
#include "mrs_srv_readmeter_config.h"
#include "mrs_srv_cco_slavenode_on_off_line.h"
#include "mrs_srv_cltii_event.h"
#include "mrs_fw_log.h"
#include "mrs_power_failure.h"
#include "mrs_transfomer_identify.h"
#include "mrs_fw_sgd_flash.h"
#include "mrs_srv_mrslog.h"

typedef HI_U32 (*nv_id_proc)(HI_U16 id);

typedef struct {
    HI_U16 id_min;
    HI_U16 id_max;
    nv_id_proc aproc;
}nv_id_struct;

HI_VOID mrsNvModuleId(HI_U16 usId);
HI_PRV nv_id_proc mrsNvGetIndex(HI_U16 id);
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) || defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
HI_PRV HI_U32 MRS_GetAppMrNvCfg(HI_OUT NV_APP_MR_CFG_STRU* pstCfg);
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
HI_PRV HI_U32 mrsNvCcoMrCfg(HI_U16 usId);
HI_PRV HI_U32 mrsInitSearchMeterNvDefault(NV_APP_SEARCHMETER_CFG_STRU * cfg);
HI_PRV HI_U32 mrsInitSearchMeterExpNvDefault(NV_APP_CCO_SM_CFG_EXP_STRU * pstNvCfg);
HI_PRV HI_U32 mrsNvPlcOverTimeCfg(HI_U16 usId);
HI_PRV HI_U32 mrsNvPlcTimeOutNumCfg(HI_U16 usId);
//NV���е�ӳ���ά��ʹ�ܿ��غͷ��Ͷ����ַPLC֡ʱ�����ı�֪ͨ
HI_PRV HI_U32 mrsNvMapInfoCfg(HI_U16 usId);
HI_U32 mrsNvCcoClientIdCfg(NV_MRS_USER_VER_S * pInfo);
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
HI_PRV HI_U32 mrsNvStaMrCfg(HI_U16 usId);
HI_PRV HI_U32 SmInitCfgNvDefault(NV_APP_SEARCHMETER_CFG_STRU * cfg);
HI_PRV HI_U32 InitSmCfgExpNvValue(NV_APP_STA_SM_CFG_EXP_STRU * cfg);
HI_PRV HI_U32 mrsNvStaReportFlagCfg(HI_U16 usId);
HI_PRV HI_U32 mrsNvCollectDevAddrCfg(HI_U16 usId);
HI_U32 mrsNvStaClietIdCfg(NV_MRS_USER_VER_S * pInfo);
#endif

//NV��ID�����б�
nv_id_struct nv_data[] = {
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    {ID_NV_APP_MR_CFG,ID_NV_APP_MR_CFG,mrsNvCcoMrCfg},
    {ID_NV_APP_SEARCHMETER_CFG,ID_NV_APP_SEARCHMETER_CFG,mrsNvCcoSearchMeterCfg},
    {ID_NV_APP_CCO_SM_CFG_EXP,ID_NV_APP_CCO_SM_CFG_EXP,mrsNvCcoSearchMeterCfgExp},
    {ID_NV_APP_EVENT_CFG,ID_NV_APP_EVENT_CFG,mrsNvCcoEvtCfg},
    {ID_NV_APP_STATE_CHG_REPORT_CFG,ID_NV_APP_STATE_CHG_REPORT_CFG,mrsNvNodeStateChgReportCfg},
    {ID_NV_APP_PLC_OVERTIME_INFO,ID_NV_APP_PLC_OVERTIME_INFO,mrsNvPlcOverTimeCfg},
    {ID_MV_APP_MAX_PLC_TIMEOUT_NUM,ID_MV_APP_MAX_PLC_TIMEOUT_NUM,mrsNvPlcTimeOutNumCfg},
    {ID_NV_APP_MAX_TURN_NUM,ID_NV_APP_MAX_TURN_NUM,mrsNvMaxTurnNumCfg},
    {ID_NV_MRS_RM_CFG,ID_NV_MRS_RM_CFG,mrsNvRmSyncCfg},
    {ID_NV_APP_MR_DUT,ID_NV_APP_MR_DUT,mrsNvDutFlagCfg},
    {ID_NV_APP_MAC_MAP_INFO,ID_NV_APP_MAC_MAP_INFO,mrsNvMapInfoCfg},
    {ID_NV_MRS_UPG_PARAM_INFO,ID_NV_MRS_UPG_PARAM_INFO,mrsNvRemoteUpgInfCfg},
    {HI_NV_FTM_CCO_PARAM_ID, HI_NV_FTM_CCO_PARAM_ID, mrsNvCcoFtParam},
#endif
    {ID_NV_MRS_USER_VER, ID_NV_MRS_USER_VER, mrsNvUserVerCfg},
#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
    {ID_NV_APP_MR_CFG,ID_NV_APP_MR_CFG,mrsNvStaMrCfg},
    {ID_NV_APP_SEARCHMETER_CFG,ID_NV_APP_SEARCHMETER_CFG,mrsNvStaSmCfg},
    {ID_NV_APP_STA_SM_CFG_EXP,ID_NV_APP_STA_SM_CFG_EXP,mrsNvStaSmCfgExp},
    {ID_NV_APP_BAUDRATE_CFG,ID_NV_APP_BAUDRATE_CFG,mrsNvInitBaudRateCfg},
    {ID_NV_APP_CTLII_CFG,ID_NV_APP_CTLII_CFG,mrsNvCltIIRefresh},
    {ID_NV_APP_EVENT_CFG,ID_NV_APP_EVENT_CFG,mrsNvStaEvtCfg},
    {ID_MV_APP_SIMU_METER_INFO,ID_MV_APP_SIMU_METER_INFO,mrsNvSimuStateCfg},
    {ID_NV_APP_REPORT_METER_INFO,ID_NV_APP_REPORT_METER_INFO,mrsNvStaReportFlagCfg},
    {HI_NV_FTM_COLLECT_PARAM_FILE,HI_NV_FTM_COLLECT_PARAM_FILE,mrsNvCollectDevAddrCfg},
    {ID_NV_MRS_STA_RM_CFG, ID_NV_MRS_STA_RM_CFG, mrsNvStaRmCfg},
#endif
};


//*****************************************************************************
// ��������: mrsNvChangeMsgNotify
// ��������: NV��ı���Ϣ֪ͨ
//
// ����˵��:
//   usId [in] NV��ID
//
// �� �� ֵ:
//    HI_ERR_SUCCESS: �ɹ�������Ϣ
//    ����������
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: fengxiaomin/00209182 [2014-07-16]
//*****************************************************************************
HI_U32 mrsNvChangeMsgNotify(HI_U16 usId)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_SYS_QUEUE_MSG_S stMsg = {0};

    (hi_void)memset_s(&stMsg, sizeof(HI_SYS_QUEUE_MSG_S), 0, sizeof(HI_SYS_QUEUE_MSG_S));
    stMsg.ulMsgId = EN_MRS_FW_MSG_NV_CHANGE_NOTIFY;
    stMsg.ulParam[0] = (HI_U32)usId;

    ret = mrsSendMessage2Queue(&stMsg);

    return ret;
}

//*****************************************************************************
// ��������: mrsNvChangeMsgProc
// ��������: NV��ı���Ϣ����
//
// ����˵��:
//   pstMsg [in] NV��ı���Ϣ����
//
// �� �� ֵ:
//    HI_ERR_SUCCESS: �ɹ�����
//    ����������
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: fengxiaomin/00209182 [2014-07-16]
//*****************************************************************************
HI_U32 mrsNvChangeMsgProc(HI_SYS_QUEUE_MSG_S* pstMsg)
{
    HI_U32 ret =HI_ERR_SUCCESS;
    HI_U16 id = (HI_U16)pstMsg->ulParam[0];
    nv_id_proc proc;

    proc = mrsNvGetIndex(id);
    if (!proc)
    {
        return HI_ERR_INVALID_ID;
    }

    ret = proc(id);

    return ret;
}

//*****************************************************************************
// ��������: mrsNvGetIndex
// ��������: ��ȡ�б���NV��ID��Ӧ�Ĵ�����
//
// ����˵��:
//   usId [in] NV��ID
//
// �� �� ֵ:
//    �ǿ�: �ɹ��ҵ�NV�б���ID��Ӧ�Ĵ�����
//    ����������
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: fengxiaomin/00209182 [2014-07-16]
//*****************************************************************************
nv_id_proc mrsNvGetIndex(HI_U16 id)
{
    HI_U32 i = 0;

    for (i = 0; i < MRS_TOOLS_ALEN(nv_data); i++)
    {
        if ((id >= nv_data[i].id_min) && (id <= nv_data[i].id_max))
        {
            return nv_data[i].aproc;
        }
    }

    return HI_NULL;
}

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) || defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
HI_U32 MRS_GetAppMrNvCfg( HI_OUT NV_APP_MR_CFG_STRU* pstCfg)
{
    HI_ASSERT(pstCfg);

#ifdef PRODUCT_CFG_FEATURE_NVM
    if(HI_ERR_SUCCESS != HI_MDM_NV_Read(ID_NV_APP_MR_CFG, (HI_PVOID)pstCfg, sizeof(NV_APP_MR_CFG_STRU)))
#endif
    {
        // Ĭ��ֵ
        pstCfg->ucBcmInterval = 1;
        pstCfg->ucBcmRetryMax = 5;

        pstCfg->ucProtoVer = PROTO_VER_3762_2009;

        pstCfg->ucAfn06F2UpRetryMax = MRS_SRV_CCO_TIMER_MUSTER_RETRY_NUM_DEFAULT;
        pstCfg->usAfn06F2UpTimeOut = MRS_SRV_CCO_TIMER_MUSTER_TIMEOUT_DEFAULT;
        pstCfg->ucAfn14F1UpRetryMax = MRS_SRV_CCO_TIMER_MUSTER_RETRY_NUM_DEFAULT;
        pstCfg->usAfn14F1UpTimeOut = MRS_SRV_CCO_TIMER_MUSTER_TIMEOUT_DEFAULT;

        pstCfg->ucTestModeSnId_ucCsgParallelEnable = 2;

        pstCfg->ucFrameBufItemCount = PRODUCT_CFG_STA_FRAME_BUF_ITEM_COUNT_DEFAULT;
        pstCfg->ucFrameBufTimeout = PRODUCT_CFG_STA_FRAME_BUF_TIMEOUT_DEFAULT;

        pstCfg->ucSendTimeCount = 3;

        pstCfg->plc_rxtx_mode = MRS_PLC_RX_TX_MODE_NORMAL;

        pstCfg->ulSimuChlTimeout = 10 * 60 * 1000;
        pstCfg->get_addr_try_num = 7;
        pstCfg->get_addr_Fail_wait_time = 60; //��λ:s

        pstCfg->usLowRatio = 9500;
        pstCfg->ucCcoFreqEnable = PRODECT_CFG_MRS_CCO_FREQ_ENABLE_DEFAULT;

        pstCfg->ucBindNetwork = MRS_SRV_STA_BIND_NETWORK;
        pstCfg->usBindDuration = MRS_SRV_STA_BIND_DURATION;
        pstCfg->ucSoftTfMode = MRS_SRV_SOFT_TF_SWITCH_CLOSE;

        pstCfg->ucWhitelistDelay = MRS_CCO_TIME_SEARCH_METER_WHITELIST_DEFAULT;
        /* BEGIN: PN:  MODIFY\ADD\DEL by cuichunyan/00276962 at 2015/11/11 */
        pstCfg->ucFlashSaveDelay = MRS_SRV_CCO_TIMER_SAVEMETERS_FLASH_ACTIVE_VALUE;
        /* END: PN:  MODIFY\ADD\DEL by cuichunyan/00276962 at 2015/11/11 */
    }

    if (0 == pstCfg->ulSimuChlTimeout)
    {
        pstCfg->ulSimuChlTimeout = 10 * 60 * 1000;
    }

    if (pstCfg->ucCcoFreqEnable)
    {
        pstCfg->ucCcoFreqEnable = PRODECT_CFG_MRS_CCO_FREQ_ENABLE_DEFAULT;
    }
    /* BEGIN: PN:  MODIFY\ADD\DEL by cuichunyan/00276962 at 2015/11/11 */
    if (0 == pstCfg->ucFlashSaveDelay)
    {
        pstCfg->ucFlashSaveDelay = MRS_SRV_CCO_TIMER_SAVEMETERS_FLASH_ACTIVE_VALUE;
    }
    /* END: PN:  MODIFY\ADD\DEL by cuichunyan/00276962 at 2015/11/11 */
    return HI_ERR_SUCCESS;
}

// ��ȡ�¼����NV��
HI_U32 mrsSrvGetEvtNvCfg(NV_APP_MRS_EVT_CFG_STRU *pstEvtNv)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;

    ulRet = HI_MDM_NV_Read(ID_NV_APP_EVENT_CFG,
                            (HI_PVOID)pstEvtNv,
                            sizeof(NV_APP_MRS_EVT_CFG_STRU));

    if(HI_ERR_SUCCESS != ulRet)
    {
        pstEvtNv->ucReportEnable = HI_FALSE;
        pstEvtNv->ucCcoMaxTry = MRS_CCO_EVT_REPORT_RETRY_TIMES_MAX;
        pstEvtNv->usCcoTimeOut = MRS_CCO_EVT_REPORT_TIMEOUT;
        pstEvtNv->usCcoFilterAgedTIme = MRS_CCO_EVT_FILTER_AGED_TIME;
        pstEvtNv->usCcoEvtBufSize = MRS_CCO_EVT_BUFFER_SIZE;
        pstEvtNv->usCcoStatBakTime = MRS_CCO_EVT_STAT_BAK_TIME;

        pstEvtNv->ucStaConsDenyTimes = MRS_STA_CONTINUOUS_DENY_TIMES_MAX;
        pstEvtNv->ucCltIIEnable = HI_FALSE;
        pstEvtNv->ucStaEvtEnable = HI_FALSE;
        pstEvtNv->ucStaMaxTry = MRS_STA_EVT_REPORT_TIMES_MAX;
        pstEvtNv->ucEvtTimeOutMax = MRS_STA_EVT_REPORT_TIMEOUT_MAX;
        pstEvtNv->ucStaTimeOut = MRS_STA_EVT_REPORT_TIMEOUT_DEFAULT;//��λΪ��
        pstEvtNv->ucStaMonitorInterval = MRS_STA_EVT_MONITOR_INTERVAL_DEFAULT; //��λ:����
/* BEGIN: Added by fengxiaomin/00209182, 2015/10/23   ���ⵥ�ź�:DTS2015102710230 */
        pstEvtNv->ucMeterEvtType = MRS_STA_EVT_METER_TYPE_UNKNOWN;

        pstEvtNv->ucCltIEnable = HI_FALSE;
    }

    if ((MRS_STA_EVT_METER_TYPE_13_OLD != pstEvtNv->ucMeterEvtType)
        && (MRS_STA_EVT_METER_TYPE_UNKNOWN != pstEvtNv->ucMeterEvtType))
    {
        pstEvtNv->ucMeterEvtType = MRS_STA_EVT_METER_TYPE_UNKNOWN;
    }
/* END:   Added by fengxiaomin/00209182, 2015/10/23 */

    return HI_ERR_SUCCESS;
}

#endif

HI_U32 mrsGetUserVerNvCfg(NV_MRS_USER_VER_S* pstVer)
{
    HI_ASSERT(pstVer);

#ifdef PRODUCT_CFG_FEATURE_NVM
    if(HI_ERR_SUCCESS != HI_MDM_NV_Read(ID_NV_MRS_USER_VER, (HI_PVOID)pstVer, sizeof(NV_MRS_USER_VER_S)))
#endif
    {
        (hi_void)memset_s(pstVer->aucUserVersion, HI_USER_VER_LENGTH, 0, HI_USER_VER_LENGTH);
        return HI_ERR_FAILURE;
    }

    return HI_ERR_SUCCESS;
}

HI_U32 mrsNvUserVerCfg(HI_U16 usId)
{
    NV_MRS_USER_VER_S stVer;

    HI_UNREF_PARAM(usId);

    (hi_void)memset_s(&stVer, sizeof(NV_MRS_USER_VER_S), 0, sizeof(NV_MRS_USER_VER_S));
    (HI_VOID)mrsGetUserVerNvCfg(&stVer);
    mrsFwSetUserVer(&stVer);
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
	mrsNvCcoClientIdCfg(&stVer);
#elif defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
	mrsNvStaClietIdCfg(&stVer);
#endif

    return HI_ERR_SUCCESS;
}

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
HI_U32 mrsNvCcoMrCfg(HI_U16 usId)
{
    NV_APP_MR_CFG_STRU cfg;

    HI_UNREF_PARAM(usId);
/* BEGIN: Added by fengxiaomin/00209182, 2015/10/23   ���ⵥ�ź�:DTS2015102710230 */
    (hi_void)memset_s(&cfg, sizeof(NV_APP_MR_CFG_STRU), 0, sizeof(NV_APP_MR_CFG_STRU));
/* END:   Added by fengxiaomin/00209182, 2015/10/23 */
    MRS_GetAppMrNvCfg(&cfg);
    mrsGetCcoMrParam(&cfg);
    mrsNvCcoFtParam(HI_NV_FTM_CCO_PARAM_ID);

    return HI_ERR_SUCCESS;
}

HI_U32 mrsNvCcoFtParam(HI_U16 usId)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_NV_FTM_CCO_PARAM_ID_S stCcoParam;
    MRS_CCO_SRV_CTX_STRU *pstCcoCtx = mrsCcoGetContext();
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO_BROAD_BAND_UNIT)
    P_MRS_CCO_EVT_CTX_STRU pstEvtCtx = mrsGetCcoEvtCtx();
#if defined (PRODUCT_OPTIMIZE_MATTEO)
    MRS_NODE_STATE_CHG_REPORT_CTX* pstStateCtx = mrsGetChgReportCtx();
#endif
#endif

    HI_UNREF_PARAM(usId);

    (hi_void)memset_s(&stCcoParam, sizeof(HI_NV_FTM_CCO_PARAM_ID_S), 0, sizeof(HI_NV_FTM_CCO_PARAM_ID_S));
    ulRet = HI_MDM_NV_Read(HI_NV_FTM_CCO_PARAM_ID, &stCcoParam, sizeof(HI_NV_FTM_CCO_PARAM_ID_S));
    if (ulRet != HI_ERR_SUCCESS)
    {
        (hi_void)memset_s(&stCcoParam, sizeof(HI_NV_FTM_CCO_PARAM_ID_S), 0, sizeof(HI_NV_FTM_CCO_PARAM_ID_S));
    }

    if (PROTO_VER_3762_2009 == pstCcoCtx->ucProtoVer)
    {
        pstCcoCtx->ucSeqControl = (stCcoParam.ucSeqCtrl == 0) ? MRS_CCO_SEQ_START_FROM_0 : MRS_CCO_SEQ_START_FROM_1;
        pstCcoCtx->ucLrModeFlag = (stCcoParam.ucLrMode == 1) ? MRS_CCO_LR_MODE_CONTINUOUS : MRS_CCO_LR_MODE_SINGLE;
    }
    else
    {
        pstCcoCtx->ucSeqControl = MRS_CCO_SEQ_START_FROM_1;
        pstCcoCtx->ucLrModeFlag = MRS_CCO_LR_MODE_SINGLE;
    }

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO_BROAD_BAND_UNIT)

    if (mrsToolCheckBbu(stCcoParam.ucFuncCfg))
    {
    	#if defined (PRODUCT_OPTIMIZE_MATTEO)
        if ((pstStateCtx != HI_NULL) &&(pstEvtCtx != HI_NULL))
        {
            // �¼����ܹر�
            pstStateCtx->ucReportEnable = HI_FALSE;
            pstEvtCtx->ucReportEnable = HI_FALSE;
        }
		#endif
    }
    else
    {
        mrsNvCcoEvtCfg(ID_NV_APP_EVENT_CFG);
        mrsNvNodeStateChgReportCfg(ID_NV_APP_STATE_CHG_REPORT_CFG);
    }

    // ������հ汾��NV�б䶯������buffer
    if ((pstCcoCtx->ucFuncCfg != stCcoParam.ucFuncCfg)
        && (mrsToolCheckBbu(stCcoParam.ucFuncCfg)
            || mrsToolIsBbu()))
    {
        mrsClear645ProtoBufferBbu(ID_MRS_BBUDA_645BUF);
    }

    if (!mrsToolIsBbu()
        && mrsToolCheckBbu(stCcoParam.ucFuncCfg))
    {
        MRS_StartTimer(MRS_CCO_TIMER_RUN_LED_BBU, MRS_STA_TIME_RUN_LED_0_50HZ, HI_SYS_TIMER_PERIODIC);
    }

    pstCcoCtx->ucFuncCfg = stCcoParam.ucFuncCfg;
    pstCcoCtx->ucMaxByteIntervalTimeBbu = stCcoParam.ucMaxByteIntervalTimeBbu;
#endif

    return HI_ERR_SUCCESS;
}

HI_U32 mrsInitAppMrNvInf(HI_VOID)
{
    NV_APP_MR_CFG_STRU cfg;

/* BEGIN: Added by fengxiaomin/00209182, 2015/10/23   ���ⵥ�ź�:DTS2015102710230 */
    (hi_void)memset_s(&cfg, sizeof(NV_APP_MR_CFG_STRU), 0, sizeof(NV_APP_MR_CFG_STRU));
/* END:   Added by fengxiaomin/00209182, 2015/10/23 */
    MRS_GetAppMrNvCfg(&cfg);
    mrsGetCcoMrParam(&cfg);
    mrsNvCcoFtParam(HI_NV_FTM_CCO_PARAM_ID);

    return HI_ERR_SUCCESS;
}

HI_U32 mrsInitSearchMeterNvDefault(NV_APP_SEARCHMETER_CFG_STRU * cfg)
{
    HI_ASSERT(HI_NULL != cfg);

    (hi_void)memset_s(cfg, sizeof(NV_APP_SEARCHMETER_CFG_STRU), 0, sizeof(NV_APP_SEARCHMETER_CFG_STRU));

    cfg->bc_send_interval = MRS_SM_BC_SEND_INTERVAL;
    cfg->bc_loop_max = MRS_SM_BC_LOOP_MAX;
    cfg->query_max_way = MRS_SM_QUERY_MAX_WAY;
    cfg->query_interval = MRS_SM_QUERY_INTERVAL;
    cfg->query_force_times = MRS_SM_QUERY_FORCE_TIMES;
    cfg->query_except_times = MRS_SM_QUERY_EXCEPT_TIMES;
    cfg->topo_query_interval = MRS_SM_TOPO_QUERY_INTERVAL;
    cfg->topo_stable_times = MRS_SM_TOPO_STABLE_TIMES;
    cfg->report_AA_2_master = MRS_SM_REPORT_AA_2_MASTER;
    cfg->ucAfn06F4UpRetryMax = MRS_SRV_CCO_TIMER_MUSTER_RETRY_NUM_DEFAULT;
    cfg->usAfn06F4UpTimeOut = MRS_SRV_CCO_TIMER_MUSTER_TIMEOUT_DEFAULT;

    return HI_ERR_SUCCESS;
}


HI_U32 mrsInitSearchMeterExpNvDefault(NV_APP_CCO_SM_CFG_EXP_STRU * pstNvCfg)
{
    HI_ASSERT(HI_NULL != pstNvCfg);

    (hi_void)memset_s(pstNvCfg, sizeof(NV_APP_CCO_SM_CFG_EXP_STRU), 0, sizeof(NV_APP_CCO_SM_CFG_EXP_STRU));

    pstNvCfg->usSMDurationDefault   = MRS_SM_DURATION_DEFALUT;
    pstNvCfg->ucRecoverWhiteSwitch  = HI_TRUE;
    pstNvCfg->ucBindCmdSendInterVal = MRS_SM_BIND_CMD_INTERVAL;
    pstNvCfg->ucCheckMainNodeAddr   = MRS_SM_CHECK_MAIN_NODE_ADDR;
    pstNvCfg->ucAllowSMInTf         = MRS_SM_ALLOW_SM_IN_TF;
    pstNvCfg->usCheckMainNodeTimeout= MRS_SM_CHECK_MAIN_MODE_TIMEOUT;
    pstNvCfg->ucSmReportWaitLockMsg = MRS_SM_REPORT_WAIT_LOCK_MSG;
    pstNvCfg->ucCloseWhiteWhenSM    = HI_TRUE;
    pstNvCfg->ucCcoRstPinEnable     = HI_TRUE;
    pstNvCfg->ucBindCmdSendMaxWay   = MRS_SM_BIND_CMD_SEND_MAX_WAY;
    pstNvCfg->ucBindCmdSendMaxTimes = MRS_SM_BIND_CMD_SEND_MAX_TIMES;
    pstNvCfg->ucReportModeInit      = 0;
    pstNvCfg->bUseNotifyMode        = HI_TRUE;
    pstNvCfg->bCloseWhiteTfBegin    = HI_TRUE;
    pstNvCfg->bRecoverWhiteTfEnd    = HI_TRUE;
    pstNvCfg->bCareReportSwitch     = HI_FALSE;
    pstNvCfg->bReportUnknownMainNode= HI_TRUE;

    (hi_void)memset_s(pstNvCfg->aucMainNodeAddrUnknown, HI_PLC_MAC_ADDR_LEN, 0xFF, HI_PLC_MAC_ADDR_LEN);

    pstNvCfg->ucQueryInterval       = MRS_TF_QUERY_INTERVAL;
    pstNvCfg->ucQueryMaxTry         = MRS_TF_QUERY_MAX_TRY;
    pstNvCfg->ucQueryMaxWay         = MRS_TF_QUERY_MAX_WAY;
    pstNvCfg->usQueryBeginDelayTIme = MRS_TF_QUERY_BEGIN_DELAY_TIME;
    pstNvCfg->usQueryRoundInterval  = MRS_TF_QUERY_ROUND_INTERVAL;
    pstNvCfg->ucFlagInArcInBelong   = 0;
    pstNvCfg->ucFlagInArcNoBelong   = 1;
    pstNvCfg->ucFlagNoArcInBelong   = 2;
    pstNvCfg->ucFlagNoArcNoBelong   = 3;
    pstNvCfg->area_code = APP_AREA_CODE_DEFAULT;

    return HI_ERR_SUCCESS;
}

#define SEARCH_SEQ_MIN  (0x100)
#define SEARCH_SEQ_MAX  (0x400)
HI_U32 mrsNvCcoSearchMeterCfg(HI_U16 usId)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    NV_APP_SEARCHMETER_CFG_STRU nv_cfg;
    MRS_CCO_SRV_CTX_STRU *cco = mrsCcoGetContext();
    MRS_SEARCH_METER_CFG_S *cfg = HI_NULL;
    HI_PRV HI_U8 first = HI_TRUE;

    (hi_void)memset_s(&nv_cfg, sizeof(nv_cfg), 0, sizeof(nv_cfg));
    ret = HI_MDM_NV_Read(usId, &nv_cfg, (HI_U16)sizeof(nv_cfg));
    if (HI_ERR_SUCCESS != ret)
    {
        mrsInitSearchMeterNvDefault(&nv_cfg);
    }

    cfg = &(cco->stAutoSearchModel.stSearchCfg);
    cfg->bc_send_interval = nv_cfg.bc_send_interval;
    cfg->bc_loop_max = nv_cfg.bc_loop_max;
    cfg->query_max_way = nv_cfg.query_max_way;
    cfg->query_interval = nv_cfg.query_interval;
    cfg->query_force_times = nv_cfg.query_force_times;
    cfg->query_except_times = nv_cfg.query_except_times;
    cfg->topo_query_interval = nv_cfg.topo_query_interval;
    cfg->topo_stable_times = nv_cfg.topo_stable_times;
    if (first) {
        /* ĳ�������к�ֻ֧��16λ���ʲ���ʹ��32λ������������Χ�̶�[0x100, 0x400] */
        HI_MDM_GetRealRandom(SEARCH_SEQ_MIN, SEARCH_SEQ_MAX, &cfg->seq);
        first = HI_FALSE;
    }

    cfg->report_AA_2_master = nv_cfg.report_AA_2_master;
    cfg->ucAfn06F4UpRetryMax = nv_cfg.ucAfn06F4UpRetryMax;
    cfg->usAfn06F4UpTimeOut = nv_cfg.usAfn06F4UpTimeOut;

    return HI_ERR_SUCCESS;
}


HI_U32 mrsNvCcoSearchMeterCfgExp(HI_U16 usId)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    NV_APP_CCO_SM_CFG_EXP_STRU stNvCfg = {0};
    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();
    MRS_SEARCH_METER_CFG_S * pstSearchMeterCfg = HI_NULL;

    if (HI_NULL == pstCcoCtx)
    {
        return HI_ERR_FAILURE;
    }

    (hi_void)memset_s(&stNvCfg, sizeof(NV_APP_CCO_SM_CFG_EXP_STRU), 0, sizeof(NV_APP_CCO_SM_CFG_EXP_STRU));

    ulRet = HI_MDM_NV_Read(usId, &stNvCfg, (HI_U16)sizeof(NV_APP_CCO_SM_CFG_EXP_STRU));
    if (HI_ERR_SUCCESS != ulRet)
    {
        mrsInitSearchMeterExpNvDefault(&stNvCfg);
    }

    pstSearchMeterCfg = &(pstCcoCtx->stAutoSearchModel.stSearchCfg);
    pstSearchMeterCfg->usSMDurationDefault      = stNvCfg.usSMDurationDefault;
    pstSearchMeterCfg->ucRecoverWhiteSwitch     = stNvCfg.ucRecoverWhiteSwitch;
    pstSearchMeterCfg->ucCloseWhiteWhenSM       = stNvCfg.ucCloseWhiteWhenSM;
    pstSearchMeterCfg->ucBindCmdSendInterval    = stNvCfg.ucBindCmdSendInterVal;
    pstSearchMeterCfg->ucCheckMainNodeAddr      = stNvCfg.ucCheckMainNodeAddr;
    pstSearchMeterCfg->ucAllowSMInTf            = stNvCfg.ucAllowSMInTf;
    pstSearchMeterCfg->usCheckMainNodeTimeout   = stNvCfg.usCheckMainNodeTimeout;
	pstCcoCtx->ucRfTimeSlot = stNvCfg.usNvRfTimeSlot;
	pstCcoCtx->ucRfSGDTimeSlot = stNvCfg.usNvRfSGDTimeSlot;
    if (0 == pstSearchMeterCfg->usCheckMainNodeTimeout)
    {
         pstSearchMeterCfg->usCheckMainNodeTimeout = MRS_SM_CHECK_MAIN_MODE_TIMEOUT;
    }

    pstSearchMeterCfg->ucSmReportWaitLockMsg    = stNvCfg.ucSmReportWaitLockMsg;
    pstCcoCtx->ucCcoRstPinEnable = stNvCfg.ucCcoRstPinEnable;
    pstCcoCtx->area_code = stNvCfg.area_code;

    mrsTfCcoSetNvPara(&stNvCfg);

    HI_DIAG_LOG_MSG_E3(MRS_FILE_LOG_FLAG_001,
        HI_DIAG_MT("����CCO�ѱ���չNV: [usSMDurationDefault] [ucRecoverWhiteSwitch] [ucBindCmdSendInterVal]"),
        stNvCfg.usSMDurationDefault, stNvCfg.ucRecoverWhiteSwitch, stNvCfg.ucBindCmdSendInterVal);

    return HI_ERR_SUCCESS;
}

HI_U32 mrsNvCcoEvtCfg(HI_U16 usId)
{
    P_MRS_CCO_EVT_CTX_STRU pstEvtCtx = mrsGetCcoEvtCtx();
    NV_APP_MRS_EVT_CFG_STRU stEvtCfg;
	MRS_CCO_SRV_CTX_STRU *pstCtx = mrsCcoGetContext();

    MRS_NOT_USED(usId);

    (hi_void)memset_s(&stEvtCfg, sizeof(stEvtCfg), 0, sizeof(stEvtCfg));

    //NV�Ѿ���ȡ��stNvCfg�У����䴫�ݳ���
    mrsSrvGetEvtNvCfg(&stEvtCfg);

    // NV EVENT
    mrsSetStaPowerFailureNvCtrl(&stEvtCfg);


    HI_MDM_NM_PowerFailurReportEnable(&stEvtCfg.ucPowerFailureReportEnable);

    pstEvtCtx->ucMaxTry = stEvtCfg.ucCcoMaxTry;
    pstEvtCtx->ucReportEnable = stEvtCfg.ucReportEnable;
    pstEvtCtx->usTimeOut = stEvtCfg.usCcoTimeOut;
    pstEvtCtx->usFilterAgedTime = stEvtCfg.usCcoFilterAgedTIme;
    pstEvtCtx->usEvtBufSize = stEvtCfg.usCcoEvtBufSize;
    pstEvtCtx->usStatBakTime = stEvtCfg.usCcoStatBakTime;
#if defined(MRS_SRV_PROTOCOL_CJT188) 	
	pstCtx->stAnytimeRMModel.ucAdditionTime188 = 6;
	pstCtx->stAnytimeRMModel.ucPlcMaxReTryB188 = 0;
	pstCtx->stAnytimeRMModel.ucPlcMaxReTryS188 = 1; 
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO_BROAD_BAND_UNIT)
    if (mrsToolIsBbu())
    {
        pstEvtCtx->ucReportEnable = HI_FALSE;
    }
    else
    {
        pstEvtCtx->ucReportEnable = stEvtCfg.ucReportEnable;
    }
#endif

    // Event NV������


    return HI_ERR_SUCCESS;
}

HI_U32 mrsNvPlcOverTimeCfg(HI_U16 usId)
{
/* BEGIN: PN: DTS2015033100426 MODIFY\ADD\DEL by cuiate/00233580 at 2015/3/26 */
    MRS_NOT_USED(usId);
    return mrsRmCfgFastNvInit();
/* END:   PN: DTS2015033100426 MODIFY\ADD\DEL by cuiate/00233580 at 2015/3/26 */
}

HI_U32 mrsNvPlcTimeOutNumCfg(HI_U16 usId)
{
    MRS_NOT_USED(usId);
    return mrsInitPlcTimeOutMaxNum();
}

// ��ȡ����ִ����Դ���Nv��
HI_U32 mrsNvMaxTurnNumCfg(HI_U16 usId)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    NV_APP_MAX_TURN_NUM stTurnNum = {0};

    MRS_NOT_USED(usId);

    ret = HI_MDM_NV_Read(ID_NV_APP_MAX_TURN_NUM,
                        &stTurnNum, sizeof(NV_APP_MAX_TURN_NUM));

    if(ret == HI_ERR_SUCCESS)
    {
        if(mrsGetMaxTurnNum() == 0)
        {
            mrsSetCurrTurnNum(0);   // ���NV���ڸĶ�֮ǰΪ0;��Ķ�֮�󽫵�ǰ�ִ�����
        }
        mrsSetMaxTurnNum(stTurnNum.ulTurnNum);
    }
    else
    {
        mrsSetMaxTurnNum(PRODUCT_CFG_MRS_TURN_NUM_DEFAULT);
    }

    return ret;
}

// ͬ��NV������
HI_U32 mrsNvRmSyncCfg(HI_U16 usId)
{
/* BEGIN: PN: DTS2015033100426 MODIFY\ADD\DEL by cuiate/00233580 at 2015/3/26 */
    MRS_NOT_USED(usId);
    return mrsRmCfgDefaultNvInit();
/* END:   PN: DTS2015033100426 MODIFY\ADD\DEL by cuiate/00233580 at 2015/3/26 */
}

HI_U32 mrsNvDutFlagCfg(HI_U16 usId)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    NV_APP_MR_DUT_STRU stDutCfg = {0};
    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();

    MRS_NOT_USED(usId);

    ret = HI_MDM_NV_Read(ID_NV_APP_MR_DUT, &stDutCfg, sizeof(stDutCfg));
    if (ret != HI_ERR_SUCCESS)
    {
        (hi_void)memset_s(&stDutCfg, sizeof(stDutCfg), 0, sizeof(stDutCfg));
    }

    pstCcoCtx->ucDutBoardFlag = stDutCfg.ucDutFlag ? 1 : 0;
    pstCcoCtx->sDutNtbAjustBase = stDutCfg.sNtbAjustBase;

    return ret;
}

//NV���е�ӳ���ά��ʹ�ܿ��غͷ��Ͷ����ַPLC֡ʱ�����ı�֪ͨ
HI_U32 mrsNvMapInfoCfg(HI_U16 usId)
{
    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();

    HI_UNREF_PARAM(usId);

    mrsGetNvMacMapInfo(&pstCcoCtx->stMapModule);
    return HI_ERR_SUCCESS;
}

HI_U32 mrsNvRemoteUpgInfCfg(HI_U16 usId)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    NV_APP_MRS_UPG_PARAM_STRU stNvCfg;
    MRS_REMOTE_UPG_MODULE *pstMrsUpgCtx = mrsGetRemoteUpgCtx();
    HI_NV_FTM_CCO_PARAM_ID_S stNVUpg;

    (hi_void)memset_s(&stNvCfg, sizeof(stNvCfg), 0, sizeof(stNvCfg));
    (hi_void)memset_s(&stNVUpg, sizeof(stNVUpg), 0, sizeof(stNVUpg));

    ret = HI_MDM_NV_Read(usId, &stNvCfg, (HI_U16)sizeof(stNvCfg));
    if (ret != HI_ERR_SUCCESS)
    {
        mrsInitRemoteUpgNvDefault(&stNvCfg);
    }

    (hi_void)memcpy_s(&pstMrsUpgCtx->stUpgParam, sizeof(pstMrsUpgCtx->stUpgParam), &stNvCfg, sizeof(NV_APP_MRS_UPG_PARAM_STRU));
    MRS_DATA_VALID_CHECK(pstMrsUpgCtx->stUpgParam.ucFileCheckMsgMaxTimes,
                            MRS_CCO_UPG_FILE_CHECK_MSG_MAX_TIMES_MAX,
                            MRS_CCO_UPG_FILE_CHECK_MSG_MAX_TIMES_DEFAULT,
                            MRS_CCO_UPG_FILE_CHECK_MSG_MAX_TIMES_MIN);

    MRS_DATA_VALID_CHECK(pstMrsUpgCtx->stUpgParam.ucUpgCmdMsgMaxTimes,
                            MRS_CCO_UPG_CMD_MSG_MAX_TIMES_MAX,
                            MRS_CCO_UPG_CMD_MSG_MAX_TIMES_DEFAULT,
                            MRS_CCO_UPG_CMD_MSG_MAX_TIMES_MIN);

    MRS_DATA_VALID_CHECK(pstMrsUpgCtx->stUpgParam.usMrsUpgFileRxTimeOut,
                            MRS_CCO_UPG_FILE_RECEIVE_TIME_OUT_MAX,
                            MRS_CCO_UPG_FILE_RECEIVE_TIME_OUT_DEFAULT,
                            MRS_CCO_UPG_FILE_RECEIVE_TIME_OUT_MIN);

    MRS_DATA_VALID_CHECK(pstMrsUpgCtx->stUpgParam.usMrsUpgStatusQueryTime,
                            MRS_CCO_UPG_STATUS_QUERY_TIME_MAX,
                            MRS_CCO_UPG_STATUS_QUERY_TIME_DEFAULT,
                            MRS_CCO_UPG_STATUS_QUERY_TIME_MIN);

    MRS_DATA_VALID_CHECK(pstMrsUpgCtx->stUpgParam.ucPowerOnStatusMaxTimes,
                            MRS_CCO_UPG_STATUS_QUERY_POWERON_TIMES_MAX,
                            MRS_CCO_UPG_STATUS_QUERY_POWERON_TIMES_DEFAULT,
                            MRS_CCO_UPG_STATUS_QUERY_POWERON_TIMES_MIN);

/* BEGIN: PN: DTS2015042907476 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/29 */
    if (0 == pstMrsUpgCtx->stUpgParam.ucWaitUpgStartTimeMax)
    {
        pstMrsUpgCtx->stUpgParam.ucWaitUpgStartTimeMax = MRS_CCO_UPG_CMD_WAIT_PLC_AVAILABLE_TIME;
    }
/* END:   PN: DTS2015042907476 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/29 */

    ret = HI_MDM_NV_Read(HI_NV_FTM_CCO_PARAM_ID, &stNVUpg, sizeof(HI_NV_FTM_CCO_PARAM_ID_S));
    if (HI_ERR_SUCCESS != ret)
    {
        stNVUpg.ucUpgId = 1;
    }

    pstMrsUpgCtx->ucUpgId = stNVUpg.ucUpgId;

    return HI_ERR_SUCCESS;
}

//*****************************************************************************
// ��������: mrsNvNodeStateChgReportCfg
// ��������: CCO�ز��ӽڵ�����״̬�仯�ϱ���ز���NV���ã��޸�NV��ʵʱ��Ч
//
// ����˵��:
//   HI_U16 usId �ӽڵ�����״̬�仯�ϱ�����NV��ID 0x408
//
// �� �� ֵ:
//   HI_ERR_FAILURE         �����Ĵ�����NVʧ��
//   HI_ERR_SUCCESS         �ɹ���NV���õ�������
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: liupeidong/00238134 [2015-03-23]
//*****************************************************************************
HI_U32 mrsNvNodeStateChgReportCfg(HI_U16 usId)
{
#if defined (PRODUCT_OPTIMIZE_MATTEO)

    MRS_NODE_STATE_CHG_REPORT_CTX* pstCtx = mrsGetChgReportCtx();
    NV_APP_MRS_STATECHG_REPORT_CFG_STRU stNv;
    HI_U32 ulRet = HI_ERR_SUCCESS;

    MRS_NOT_USED(usId);

    if (HI_NULL == pstCtx)
    {
        return HI_ERR_FAILURE;
    }

    (hi_void)memset_s(&stNv, sizeof(stNv), 0, sizeof(stNv));
    ulRet = HI_MDM_NV_Read(ID_NV_APP_STATE_CHG_REPORT_CFG, &stNv, sizeof(stNv));
    if (HI_ERR_SUCCESS != ulRet)
    {
        pstCtx->ucReportEnable = MRS_NODE_STATE_CHG_REPORT_ENABLE_DEFAULT;
        pstCtx->ucPowerOffReasonSwitch = MRS_NODE_STATE_CHG_REPORT_REASON_POWER_OFF_DEFAULT;
        pstCtx->ucPlcChlReasonSwitch = MRS_NODE_STATE_CHG_REPORT_REASON_PLC_CHL_DEFAULT;
        pstCtx->ucCacheNumMax = MRS_NODE_STATE_CHG_REPORT_CACHE_NUM_MAX_DEFAULT;
        pstCtx->usCacheTimeout = MRS_NODE_STATE_CHG_REPORT_CACHE_TIMEOUT_DEFAULT;
        pstCtx->usReportTimeout = MRS_NODE_STATE_CHG_REPORT_FRAME_TIMEOUT_DEFAULT;
        pstCtx->ucReportRetryMax = MRS_NODE_STATE_CHG_REPORT_FRAME_MAX_TRY_DEFAULT;

        pstCtx->ucPoweroffDetectMaxTries = MRS_POWEROFF_DETECT_MAX_TRIES_DEFAULT;
        pstCtx->ucPoweroffDetectInterval = MRS_POWEROFF_DETECT_INTERVAL_DEFAULT;
        pstCtx->ucPoweroffRecheckMaxTries= MRS_POWEROFF_RECHECK_MAX_TRIES_DEFAULT;
        pstCtx->ucPoweroffRecheckInterval= MRS_POWEROFF_RECHECK_INTERVAL_DEFAULT;
        pstCtx->ucReviewScoreFull        = MRS_POWEROFF_REVIEW_SCORE_FULL_DEFAULT;
        pstCtx->ucWrongReportReduceScore = MRS_POWEROFF_WRONG_REPORT_REDUCE_SCORE_DEFAULT;
        pstCtx->ucMissReportPlusScore    = MRS_POWEROFF_MISS_REPORT_PLUS_SCORE_DEFAULT;
        pstCtx->ucCorrectReportPlusScore = MRS_POWEROFF_CORRECT_REPORT_PLUS_SCORE_DEFAULT;
        pstCtx->ucNoPassDetectPlusScore  = MRS_POWEROFF_NONE_PASS_DETECT_PLUS_SCORE_DEFAULT;
        pstCtx->ulNoPassDetectPeriod     = MRS_POWEROFF_NONE_PASS_DETECT_PERIOD_DEFAULT;
        pstCtx->ulReportMaxDelayTime     = MRS_POWEROFF_REPORT_MAX_DELAY_TIME_DEFAULT;
        pstCtx->usMacReportCacheMaxNum   = MRS_POWEROFF_MAC_REPORT_CACHE_MAX_NUM_DEFAULT;
        pstCtx->ucMacReportCacheMaxTime  = MRS_POWEROFF_MAC_REPORT_CACHE_MAX_TIME_DEFAULT;
        pstCtx->ucScalePowerOffDetectInterval = MRS_POWEROFF_SCALE_POWER_OFF_DETECT_DEFAULT;
        return ulRet;
    }
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO_BROAD_BAND_UNIT)
    if (mrsToolIsBbu())
    {
        pstCtx->ucReportEnable = HI_FALSE;
    }
    else
    {
        pstCtx->ucReportEnable = stNv.ucReportEnable;
    }
#endif
    pstCtx->ucPowerOffReasonSwitch = stNv.ucPowerOffReasonSwitch;
    pstCtx->ucPlcChlReasonSwitch = stNv.ucPlcChlReasonSwitch;
    pstCtx->ucCacheNumMax = stNv.ucCacheNumMax;
    pstCtx->usCacheTimeout = stNv.usCacheTimeout;
    pstCtx->usReportTimeout = stNv.usReportTimeout;
    pstCtx->ucReportRetryMax = stNv.ucReportRetryMax;

    MRS_DATA_VALID_CHECK(pstCtx->ucCacheNumMax,
                         MRS_NODE_STATE_CHG_REPORT_CACHE_NUM_MAX_UPPERBOUND,
                         MRS_NODE_STATE_CHG_REPORT_CACHE_NUM_MAX_DEFAULT,
                         MRS_NODE_STATE_CHG_REPORT_CACHE_NUM_MAX_LOWERBOUND);

    pstCtx->ucPoweroffDetectMaxTries    = stNv.ucPoweroffDetectMaxTries;
    pstCtx->ucPoweroffDetectInterval    = stNv.ucPoweroffDetectInterval;
    pstCtx->ucPoweroffRecheckMaxTries   = stNv.ucPoweroffRecheckMaxTries;
    pstCtx->ucPoweroffRecheckInterval   = stNv.ucPoweroffRecheckInterval;
    pstCtx->ucReviewScoreFull           = stNv.ucReviewScoreFull;
    pstCtx->ucWrongReportReduceScore    = stNv.ucWrongReportReduceScore;
    pstCtx->ucMissReportPlusScore       = stNv.ucMissReportPlusScore;
    pstCtx->ucCorrectReportPlusScore    = stNv.ucCorrectReportPlusScore;
    pstCtx->ucNoPassDetectPlusScore     = stNv.ucNoPassDetectPlusScore;
    pstCtx->ulNoPassDetectPeriod        = MRS_MIN_TO_SEC(stNv.usNoPassDetectPeriod);
    pstCtx->ulReportMaxDelayTime        = MRS_MIN_TO_SEC(stNv.usReportMaxDelayTime);
    pstCtx->ulMacReportWrongPeriodTh    = MRS_MIN_TO_SEC(stNv.usMacReportWrongPeriodTh);
    pstCtx->ucPoweroffDetectExpTries    = stNv.ucPoweroffDetectExpTries;
    pstCtx->usMacReportCacheMaxNum      = stNv.usMacReportCacheMaxNum;
    pstCtx->ucMacReportCacheMaxTime     = stNv.ucMacReportCacheMaxTime;
	pstCtx->ucScalePowerOffDetectInterval = stNv.ucScalePowerOffDetectInterval;
    // �ϱ����أ��ص�����ע��
    if (pstCtx->ucReportEnable)
    {
        //mrsLogSendAfn14F2();//optimized by weichao
        HI_MDM_RegisterReportTopoChangeNode(mrsMACReportChgEntryCallback);
    }
#endif
    return HI_ERR_SUCCESS;
}



HI_U32 mrsCcoSetProto(HI_U8 ucProto)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    NV_APP_MR_CFG_STRU stNv;
    (hi_void)memset_s(&stNv, sizeof(stNv), 0, sizeof(stNv));
    MRS_CCO_SRV_CTX_STRU *pstCtx = mrsCcoGetContext();

    do
    {
        if ((PROTO_VER_13762_2013 != ucProto) && (PROTO_VER_3762_2009 != ucProto))
        {
            ulRet = HI_ERR_INVALID_PARAMETER;
            HI_DIAG_LOG_MSG_E1(__LINE__, HI_DIAG_MT("��ERR��invalid proto"),ucProto);
            break;
        }

        ulRet = HI_MDM_NV_Read(ID_NV_APP_MR_CFG, &stNv, sizeof(stNv));
        if (HI_ERR_SUCCESS != ulRet)
        {
            HI_DIAG_LOG_MSG_E1(__LINE__, HI_DIAG_MT("��ERR��read proto nv"),ulRet);
            break;
        }

        if (ucProto == stNv.ucProtoVer)
        {
            ulRet = HI_ERR_SUCCESS;
            HI_DIAG_LOG_MSG_E0(__LINE__, HI_DIAG_MT("��INFO��proto same"));
            break;
        }

        stNv.ucProtoVer = ucProto;

        ulRet = HI_MDM_NV_Write(ID_NV_APP_MR_CFG, &stNv, sizeof(stNv));
        if (HI_ERR_SUCCESS != ulRet)
        {
            HI_DIAG_LOG_MSG_E1(__LINE__, HI_DIAG_MT("��ERR��wr proto nv"),ulRet);
            break;
        }

        pstCtx->ucProtoVer = ucProto;

    }while(HI_FALSE);

    return ulRet;
}

HI_U32 mrsNvSetUpgId(HI_U8 ucUpgId)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_NV_FTM_CCO_PARAM_ID_S stNVUpg;

    (hi_void)memset_s(&stNVUpg, sizeof(stNVUpg), 0, sizeof(stNVUpg));

    ulRet = HI_MDM_NV_Read(HI_NV_FTM_CCO_PARAM_ID, (HI_PVOID)&stNVUpg, sizeof(stNVUpg));
    if (HI_ERR_SUCCESS != ulRet)
    {
        return ulRet;
    }

    stNVUpg.ucUpgId = ucUpgId;

    ulRet = HI_MDM_NV_Write(HI_NV_FTM_CCO_PARAM_ID, (HI_PVOID)&stNVUpg, sizeof(stNVUpg));
    if (HI_ERR_SUCCESS != ulRet)
    {
        return ulRet;
    }

    return ulRet;
}

HI_U32 mrsNvCcoClientIdCfg(NV_MRS_USER_VER_S * pInfo)
{
	HI_U32 ulRet = HI_ERR_SUCCESS;
   	FLASH_USR_CFG_INFO_S stCfg;
    	MRS_CCO_SRV_CTX_STRU *pstCcoCtx = mrsCcoGetContext();
	HI_U8 codeFlg = 0;
	(hi_void)memset_s(&stCfg, sizeof(stCfg), 0,sizeof(stCfg));
	if(!pstCcoCtx ||((pstCcoCtx->aucCode[0] == pInfo->reserved[21]) &&(pstCcoCtx->aucCode[1] == pInfo->reserved[22]) ) )
	{
		return ulRet;
	}
	do
	{
    		ulRet = mrsSgdFlashRead(ID_FLASH_USR_CFG, &stCfg, sizeof(stCfg));
		if(ulRet == HI_ERR_SUCCESS)
		{
			if((pstCcoCtx->aucCode[0] != pInfo->reserved[21]) ||(pstCcoCtx->aucCode[1] != pInfo->reserved[22]))
			{
				codeFlg = 1;
				stCfg.aucCode[0] = pInfo->reserved[21];
				stCfg.aucCode[1] = pInfo->reserved[22];
			}
		}
	}while(0);
	if(codeFlg)
	{
		ulRet = mrsSgdFlashWrite(ID_FLASH_USR_CFG, &stCfg, sizeof(stCfg));		
	}
	return ulRet;
}

HI_U32 mrsCcoReloadNvVerCfg(HI_VOID)
{
	HI_U32 ulRet = HI_ERR_SUCCESS;
   	FLASH_USR_CFG_INFO_S stCfg;
    	MRS_CCO_SRV_CTX_STRU *pstCcoCtx = mrsCcoGetContext();
	NV_MRS_USER_VER_S stNv;

	do
	{
		(hi_void)memset_s(&stNv,sizeof(NV_MRS_USER_VER_S),0,sizeof(NV_MRS_USER_VER_S));
		(hi_void)memset_s(&stCfg, sizeof(stCfg), 0,sizeof(stCfg));
    		ulRet = mrsSgdFlashRead(ID_FLASH_USR_CFG, &stCfg, sizeof(stCfg));
		if(ulRet != HI_ERR_SUCCESS)
		{
			 HI_DIAG_LOG_MSG_E1(__LINE__, HI_DIAG_MT("��ERR��Read flash =%d"),ulRet);
			break;
    		}
		 ulRet = mrsGetUserVerNvCfg(&stNv);
        	if (HI_ERR_SUCCESS != ulRet)
        	{
           		 HI_DIAG_LOG_MSG_E1(__LINE__, HI_DIAG_MT("��ERR��Read 8009 =%d"),ulRet);
            		break;
        	}
		if(pstCcoCtx)
		{
			(hi_void)memcpy_s(pstCcoCtx->aucCode, sizeof(pstCcoCtx->aucCode), stCfg.aucCode,sizeof(pstCcoCtx->aucCode));
		}
		if(stNv.reserved[21] != stCfg.aucCode[0] || stNv.reserved[22] != stCfg.aucCode[1] )
		{
			stNv.reserved[21] = stCfg.aucCode[0];
			stNv.reserved[22] = stCfg.aucCode[1];
			ulRet = HI_MDM_NV_Write(ID_NV_MRS_USER_VER, &stNv, sizeof(stNv));
        		if (HI_ERR_SUCCESS != ulRet)
        		{
            			HI_DIAG_LOG_MSG_E1(__LINE__, HI_DIAG_MT("��ERR��SET 8009 ret"),ulRet);
            			break;
        		}
		}
	}while(0);
	return ulRet;
}
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) || defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
HI_U32 mrsNVGetszId(HI_NV_FTM_MANUFACOTR_ID_S * pstInfo)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_ASSERT(pstInfo);

    ulRet = HI_MDM_NV_Read(HI_NV_FTM_MANUFACOTR_ID, (HI_PVOID)pstInfo, sizeof(HI_NV_FTM_MANUFACOTR_ID_S));
    if (HI_ERR_SUCCESS != ulRet)
    {
        HI_DIAG_LOG_MSG_E1(__LINE__, HI_DIAG_MT("��ERR��rd sz nv"),ulRet);
        return HI_ERR_FAILURE;
    }

    return HI_ERR_SUCCESS;
}

HI_U32 mrsNVGetClientId(HI_VOID *pstInfo)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_ASSERT(pstInfo);
	ulRet = mrsSgdFlashRead(ID_FLASH_USR_CFG,(HI_PVOID)pstInfo,sizeof(FLASH_USR_CFG_INFO_S));
    //ulRet = HI_MDM_NV_Read(HI_NV_FTM_RESERVED1_ID, (HI_PVOID)pstInfo, sizeof(HI_NV_FTM_RESERVED1_ID_S));
    if (HI_ERR_SUCCESS != ulRet)
    {
        HI_DIAG_LOG_MSG_E1(__LINE__, HI_DIAG_MT("��ERR��rd sz nv"),ulRet);
        return HI_ERR_FAILURE;
    }
    return HI_ERR_SUCCESS;
}

HI_U32 mrsNVSetszId(HI_CHAR szId[3])
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_NV_FTM_MANUFACOTR_ID_S stNv;

    do
    {
        (hi_void)memset_s(&stNv, sizeof(stNv), 0, sizeof(stNv));

        ulRet = mrsNVGetszId(&stNv);
        if (HI_ERR_SUCCESS != ulRet)
        {
            break;
        }

        if (memcmp(stNv.szId, szId, HI_MANU_CODE_LENGTH) == 0)
        {
            ulRet = HI_ERR_SUCCESS;
            break;
        }

        (hi_void)memcpy_s(stNv.szId,HI_MANU_CODE_LENGTH, szId, HI_MANU_CODE_LENGTH);

        ulRet = HI_MDM_NV_Write(HI_NV_FTM_MANUFACOTR_ID, &stNv, sizeof(stNv));
        if (HI_ERR_SUCCESS != ulRet)
        {
            break;
        }

    }while(HI_FALSE);

    return ulRet;
}

HI_U32 mrsNVSetClientId(HI_CHAR szId[3])
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    FLASH_USR_CFG_INFO_S stNv;


    do
    {
        (hi_void)memset_s(&stNv, sizeof(stNv),0, sizeof(stNv));

        ulRet = mrsNVGetClientId(&stNv);
    	if (HI_ERR_SUCCESS != ulRet)
    	{
        	HI_DIAG_LOG_MSG_E1(__LINE__, HI_DIAG_MT("��ERR��rd sz nv"),ulRet);
        	break;
    	}

        if (memcmp(&stNv.aucCode, szId, HI_MANU_CODE_LENGTH) == 0)
        {
            ulRet = HI_ERR_SUCCESS;
            HI_DIAG_LOG_MSG_E0(__LINE__, HI_DIAG_MT("��INFO��SZ SAME"));
            break;
        }

        (hi_void)memcpy_s(&stNv.aucCode,HI_MANU_CODE_LENGTH, szId, HI_MANU_CODE_LENGTH);

        ulRet = mrsSgdFlashWrite(ID_FLASH_USR_CFG, &stNv, sizeof(stNv));
        if (HI_ERR_SUCCESS != ulRet)
        {
            HI_DIAG_LOG_MSG_E1(__LINE__, HI_DIAG_MT("��ERR��wr SZ nv"),ulRet);
            break;
        }

    }while(HI_FALSE);

    return ulRet;
}

HI_U32 mrsNVSetUserVerInfo(NV_MRS_USER_VER_S * pstInfo)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    NV_MRS_USER_VER_S stNv;

    do
    {
        (hi_void)memset_s(&stNv,sizeof(NV_MRS_USER_VER_S), 0,sizeof(NV_MRS_USER_VER_S));

        ulRet = mrsGetUserVerNvCfg(&stNv);
        if (HI_ERR_SUCCESS != ulRet)
        {
            HI_DIAG_LOG_MSG_E1(__LINE__, HI_DIAG_MT("��ERR��Read 8009 =%d"),ulRet);
            break;
        }

        if (memcmp(pstInfo ,&stNv, sizeof(stNv)) == 0)
        {
            ulRet = HI_ERR_SUCCESS;
            break;
        }

        (hi_void)memcpy_s(&stNv,sizeof(NV_MRS_USER_VER_S),pstInfo,sizeof(NV_MRS_USER_VER_S));

        ulRet = HI_MDM_NV_Write(ID_NV_MRS_USER_VER, &stNv, sizeof(stNv));
        if (HI_ERR_SUCCESS != ulRet)
        {
            break;
        }

    }while(HI_FALSE);

    return ulRet;
}
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
HI_U32 mrsInitStaMrNvInf(HI_VOID)
{
    NV_APP_MR_CFG_STRU cfg;

    (hi_void)memset_s(&cfg, sizeof(cfg), 0, sizeof(cfg));
    MRS_GetAppMrNvCfg(&cfg);
    mrsGetStaMrParam(&cfg);

    return HI_ERR_SUCCESS;
}

HI_PRV HI_U32 mrsNvStaMrCfg(HI_U16 usId)
{
    NV_APP_MR_CFG_STRU cfg;

    MRS_NOT_USED(usId);

    (hi_void)memset_s(&cfg, sizeof(cfg), 0, sizeof(cfg));
    MRS_GetAppMrNvCfg(&cfg);
    mrsGetStaMrParam(&cfg);

    return HI_ERR_SUCCESS;
}


HI_U32 mrsNvStaRmCfg(HI_U16 usId)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    MRS_STA_SRV_CTX_STRU *pstStaCtx = mrsStaGetContext();
    NV_MRS_STA_RM_CFG_STRU *pstCfg = &(pstStaCtx->stRmCfg);
    HI_U8 ucDenyRetry = 0, ucTimeoutRetry = 0;
    HI_U8 ucIndex = 0;

    HI_UNREF_PARAM(usId);

    ulRet = HI_MDM_NV_Read(ID_NV_MRS_STA_RM_CFG, pstCfg, sizeof(NV_MRS_STA_RM_CFG_STRU));
    if (HI_ERR_SUCCESS != ulRet)
    {
        pstCfg->astRmCfg[MRS_STA_RM_CFG_LOCAL].usFrameHead = MRS_LOCAL_RM_FRAME_HEAD;
        pstCfg->astRmCfg[MRS_STA_RM_CFG_LOCAL].usFrameInterval = MRS_LOCAL_RM_FRAME_INTERVAL;
        pstCfg->astRmCfg[MRS_STA_RM_CFG_LOCAL].usCtrlCmdDelay = MRS_LOCAL_RM_CTRL_CMD_DELAY;
        pstCfg->astRmCfg[MRS_STA_RM_CFG_LOCAL].usMeterInterval = MRS_LOCAL_RM_METER_INTERVAL;
        pstCfg->astRmCfg[MRS_STA_RM_CFG_LOCAL].ucQueueMax = MRS_LOCAL_RM_QUEUE_MAX;
        pstCfg->astRmCfg[MRS_STA_RM_CFG_LOCAL].ucRetryCfg = MRS_LOCAL_RM_METER_TIMEOUT_RETRY | (MRS_LOCAL_RM_METER_DENY_RETRY << 2);

        pstCfg->astRmCfg[MRS_STA_RM_CFG_CLTI].usFrameHead = MRS_CLTI_RM_FRAME_HEAD;
        pstCfg->astRmCfg[MRS_STA_RM_CFG_CLTI].usFrameInterval = MRS_CLTI_RM_FRAME_INTERVAL;
        pstCfg->astRmCfg[MRS_STA_RM_CFG_CLTI].usCtrlCmdDelay = MRS_CLTI_RM_CTRL_CMD_DELAY;
        pstCfg->astRmCfg[MRS_STA_RM_CFG_CLTI].usMeterInterval = MRS_CLTI_RM_METER_INTERVAL;
        pstCfg->astRmCfg[MRS_STA_RM_CFG_CLTI].ucQueueMax = MRS_CLTI_RM_QUEUE_MAX;
        pstCfg->astRmCfg[MRS_STA_RM_CFG_CLTI].ucRetryCfg = MRS_CLTI_RM_METER_TIMEOUT_RETRY | (MRS_CLTI_RM_METER_DENY_RETRY << 2);
        return ulRet;
    }

    MRS_SM_00_TO_DEFAULT(pstCfg->astRmCfg[MRS_STA_RM_CFG_LOCAL].usFrameHead, MRS_LOCAL_RM_FRAME_HEAD);
    MRS_SM_00_TO_DEFAULT(pstCfg->astRmCfg[MRS_STA_RM_CFG_LOCAL].usFrameInterval, MRS_LOCAL_RM_FRAME_INTERVAL);
    MRS_SM_00_TO_DEFAULT(pstCfg->astRmCfg[MRS_STA_RM_CFG_LOCAL].usCtrlCmdDelay, MRS_LOCAL_RM_CTRL_CMD_DELAY);
    MRS_SM_00_TO_DEFAULT(pstCfg->astRmCfg[MRS_STA_RM_CFG_LOCAL].usMeterInterval, MRS_LOCAL_RM_METER_INTERVAL);
    MRS_SM_00_TO_DEFAULT(pstCfg->astRmCfg[MRS_STA_RM_CFG_LOCAL].ucQueueMax, MRS_LOCAL_RM_QUEUE_MAX);

    MRS_SM_00_TO_DEFAULT(pstCfg->astRmCfg[MRS_STA_RM_CFG_CLTI].usFrameHead, MRS_CLTI_RM_FRAME_HEAD);
    MRS_SM_00_TO_DEFAULT(pstCfg->astRmCfg[MRS_STA_RM_CFG_CLTI].usFrameInterval, MRS_CLTI_RM_FRAME_INTERVAL);
    MRS_SM_00_TO_DEFAULT(pstCfg->astRmCfg[MRS_STA_RM_CFG_CLTI].usCtrlCmdDelay, MRS_CLTI_RM_CTRL_CMD_DELAY);
    MRS_SM_00_TO_DEFAULT(pstCfg->astRmCfg[MRS_STA_RM_CFG_CLTI].usMeterInterval, MRS_CLTI_RM_METER_INTERVAL);
    MRS_SM_00_TO_DEFAULT(pstCfg->astRmCfg[MRS_STA_RM_CFG_CLTI].ucQueueMax, MRS_CLTI_RM_QUEUE_MAX);

    for (ucIndex = MRS_STA_RM_CFG_LOCAL; ucIndex < MRS_STA_RM_CFG_MAX; ucIndex++)
    {
        ucTimeoutRetry = mrsGet645TimeoutRetryCfg(ucIndex);
        ucDenyRetry = mrsGet645DenyRetryCfg(ucIndex);

        if (ucTimeoutRetry > MRS_RM_METER_RETRY_CNT_MAX)
        {
            pstCfg->astRmCfg[MRS_STA_RM_CFG_LOCAL].ucRetryCfg &= (MRS_RM_TIMEOUT_RETRY_CNT_DEFAULT | 0x0C);
        }

        if (ucDenyRetry > MRS_RM_METER_RETRY_CNT_MAX)
        {
            pstCfg->astRmCfg[MRS_STA_RM_CFG_LOCAL].ucRetryCfg &= ((MRS_RM_DENY_RETRY_CNT_DEFAULT << 2) | 0x03);
        }
    }

    return ulRet;
}


HI_U32 mrsNvStaSmCfg(HI_U16 usId)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    NV_APP_SEARCHMETER_CFG_STRU nv_cfg = {0};
    MRS_STA_SRV_CTX_STRU *pstSta = mrsStaGetContext();
    MRS_STA_SEARCH_CTX *pstSmCtx = mrsStaSearchCtx();
    MRS_SEARCH_CFG_STRU *pstSmCfg = &(pstSmCtx->stSearchCfg);

    HI_ASSERT(usId == ID_NV_APP_SEARCHMETER_CFG);

    (hi_void)memset_s(&nv_cfg, sizeof(nv_cfg), 0, sizeof(nv_cfg));
    ret = HI_MDM_NV_Read(usId, &nv_cfg, (HI_U16)sizeof(nv_cfg));
    if (HI_ERR_SUCCESS != ret)
    {
        SmInitCfgNvDefault(&nv_cfg);
    }

    pstSmCfg->ulDI07 = nv_cfg.search_di_07;
    pstSmCfg->ausDI97[0] = nv_cfg.search_di_97;
    pstSmCfg->ausDI97[1] = nv_cfg.search_di_97b;
    pstSmCfg->ausDI97[2] = nv_cfg.search_di_97c;
    pstSmCfg->usFrameTimeout = nv_cfg.frame_timeout;
    pstSmCfg->ucByteTimeout = nv_cfg.byte_timeout;
    pstSmCfg->ucMeterMax = nv_cfg.meter_num_max;
    pstSmCfg->bEnableBcSearch97 = (0 == nv_cfg.search_bc_enable) ? HI_FALSE : HI_TRUE;
    pstSmCfg->ucSearchRetry = nv_cfg.search_retry;
    pstSmCfg->ucInvalidFrmLen07 = nv_cfg.invalid_frm_len_07;
    pstSmCfg->ucInvalidFrmLen97 = nv_cfg.invalid_frm_len_97;
    pstSmCfg->usSearchPeriod = nv_cfg.search_period;
    pstSmCfg->bCltiSmEnable = (MRS_SEARCH_ENABLE_FLAG_I & nv_cfg.ucSearchEnableFlag_ucCltIPlcThreshold) ? HI_TRUE : HI_FALSE;
    pstSmCfg->bCltiiSmEnable = (MRS_SEARCH_ENABLE_FLAG_II & nv_cfg.ucSearchEnableFlag_ucCltIPlcThreshold) ? HI_TRUE : HI_FALSE;

    if (nv_cfg.explore_byte_count_max > MRS_CLTII_SM_EXPLORE_BYTE_COUNT_MAX_UPPERBOUND)
    {
        pstSmCfg->ucExploreByteCountMax = MRS_CLTII_SM_EXPLORE_BYTE_COUNT_MAX_DEFAULT;
    }
    else
    {
        pstSmCfg->ucExploreByteCountMax = nv_cfg.explore_byte_count_max;
    }

    pstSta->detect_loop_max = nv_cfg.detect_loop_max;
    pstSta->detect_timeout = nv_cfg.detect_timeout;
    pstSta->detect_period = nv_cfg.detect_period;
    pstSta->read_positive_A_times = nv_cfg.read_positive_A_times;
    pstSta->read_positive_A_interval = nv_cfg.read_positive_A_interval;
    pstSta->query_sm_interval = nv_cfg.query_sm_interval;
    pstSta->is_real_clti = HI_FALSE;

    if (pstSmCfg->ucMeterMax > MRS_MAX_METERNUM)
    {
        pstSmCfg->ucMeterMax = MRS_MAX_METERNUM;
    }

    MRS_SM_00_TO_DEFAULT(pstSmCfg->ucInvalidFrmLen07, MRS_SM_INVALID_FRM_LEN_07);
    MRS_SM_00_TO_DEFAULT(pstSmCfg->ucInvalidFrmLen97, MRS_SM_INVALID_FRM_LEN_97);

    MRS_SM_00_TO_DEFAULT(pstSta->detect_loop_max, MRS_DETECT_BAUDRATE_LOOP_MAX);
    MRS_SM_00_TO_DEFAULT(pstSta->detect_timeout, MRS_DETECT_BAUDRATE_TIMEOUT);
    MRS_SM_00_TO_DEFAULT(pstSta->detect_period, MRS_DETECT_BAUDRATE_PERIOD);
    MRS_SM_00_TO_DEFAULT(pstSta->read_positive_A_times, MRS_CLTI_READ_POSITIVE_A_MAX);
    MRS_SM_00_TO_DEFAULT(pstSta->read_positive_A_interval, MRS_CLTI_READ_POSITIVE_A_INTERVAL);
    MRS_SM_00_TO_DEFAULT(pstSta->query_sm_interval, MRS_CLTI_SM_QUERY_INTERVAL);

/* BEGIN: PN: DTS2015042907633 MODIFY\ADD\DEL by cuiate/00233580 at 2015/5/2 */
    mrsStaRefreshDefaultDi97();
/* END:   PN: DTS2015042907633 MODIFY\ADD\DEL by cuiate/00233580 at 2015/5/2 */

    return HI_ERR_SUCCESS;
}


HI_U32 mrsNvStaSmCfgExp(HI_U16 usId)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    NV_APP_STA_SM_CFG_EXP_STRU nv_cfg;
    MRS_STA_SRV_CTX_STRU *pstSta = mrsStaGetContext();

    HI_ASSERT(usId == ID_NV_APP_STA_SM_CFG_EXP);

    (hi_void)memset_s(&nv_cfg, sizeof(NV_APP_STA_SM_CFG_EXP_STRU), 0, sizeof(NV_APP_STA_SM_CFG_EXP_STRU));

    ret = HI_MDM_NV_Read(usId, &nv_cfg, (HI_U16)sizeof(NV_APP_STA_SM_CFG_EXP_STRU));
    if (HI_ERR_SUCCESS != ret)
    {
        InitSmCfgExpNvValue(&nv_cfg);
    }

    pstSta->usLeaveTimeLenTh  = nv_cfg.usLeaveTimeLenTh;
    pstSta->ucStaRstPinEnable = nv_cfg.ucStaRstPinEnable;
    pstSta->ulRstShielTime    = nv_cfg.ulRstShielTime;

    return HI_ERR_SUCCESS;
}


HI_U32 mrsNvStaEvtCfg(HI_U16 usId)
{
    P_MRS_STA_EVT_CTX pstCtx = mrsStaGetEvtCtx();
    NV_APP_MRS_EVT_CFG_STRU stNvCfg = {0};

    HI_UNREF_PARAM(usId);

    //NV�Ѿ���ȡ��stNvCfg�У����䴫�ݳ���
    mrsSrvGetEvtNvCfg(&stNvCfg);
    // NV EVENT
    mrsSetStaPowerFailureNvCtrl(&stNvCfg);

    HI_MDM_NM_PowerFailurReportEnable(&stNvCfg.ucPowerFailureReportEnable);

    if (NV_SIMU_METER_ENABLE == mrsStaGetSupportVM())
    {
        return HI_ERR_SKIP;
    }

    pstCtx->ucTimeout = stNvCfg.ucStaTimeOut;
    pstCtx->ucMaxRetry = stNvCfg.ucStaMaxTry;
    pstCtx->ucTimeOutMax = stNvCfg.ucEvtTimeOutMax;
    pstCtx->stStaMonInf.ucMonitorInterval = stNvCfg.ucStaMonitorInterval;
    pstCtx->stFilterInf.ucConDentMaxTimes = stNvCfg.ucStaConsDenyTimes;
#if defined(PRODUCT_CFG_CLTI_SUPPORT_MRS_SWITCH)
    if ((mrsIsRealClti() && (!pstCtx->ucCltiEnable) && (stNvCfg.ucCltIEnable))
        || ((STA_TYPE_METER == mrsStaGetType()) && (!pstCtx->ucStaEnable) && (stNvCfg.ucStaEvtEnable)))
    {
        pstCtx->ucStaEnable = stNvCfg.ucStaEvtEnable;
        pstCtx->ucCltiEnable = stNvCfg.ucCltIEnable;
        mrsStaEvtOutProc();
        return HI_ERR_SUCCESS;
    }
#else
	if (((STA_TYPE_METER == mrsStaGetType()) && (!pstCtx->ucStaEnable) && (stNvCfg.ucStaEvtEnable)))
	{
		pstCtx->ucStaEnable = stNvCfg.ucStaEvtEnable;
		pstCtx->ucCltiEnable = stNvCfg.ucCltIEnable;
		mrsStaEvtOutProc();
		return HI_ERR_SUCCESS;
	}
#endif

    pstCtx->ucStaEnable = stNvCfg.ucStaEvtEnable;
    pstCtx->ucCltiEnable = stNvCfg.ucCltIEnable;

    return HI_ERR_SUCCESS;
}

HI_U32 mrsStaSearchFtmNvInit(HI_VOID)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_NV_FTM_COLLECT_PARAM_FILE_S nv;
    MRS_STA_SEARCH_CTX *pstSmCtx = mrsStaSearchCtx();

    (hi_void)memset_s(&nv, sizeof(nv), 0, sizeof(nv));
    ret = HI_MDM_NV_Read(HI_NV_FTM_COLLECT_PARAM_FILE, &nv, sizeof(nv));
    if (HI_ERR_SUCCESS != ret)
    {
        return HI_ERR_FAILURE;
    }

    if (mrsToolsMemIsZero(nv.CCTAddress, sizeof(nv.CCTAddress)))
    {
        (hi_void)memset_s(nv.CCTAddress, sizeof(nv.CCTAddress), 0xFF, sizeof(nv.CCTAddress));
        nv.CCT_register = 0;
    }

    if (mrsToolsMemIsZero(nv.meter, sizeof(nv.meter)))
    {
        (hi_void)memset_s(nv.meter, sizeof(nv.meter), 0xFF, sizeof(nv.meter));
        nv.protocol = 0;
        nv.baudrate_index = 0;
        nv.DI_v97 = 0;
    }

    // ȡ�ô洢���ѵ����ױ��ַ
    (hi_void)memcpy_s(pstSmCtx->stSearchCfg.aucNvMeter, sizeof(pstSmCtx->stSearchCfg.aucNvMeter), nv.meter, HI_METER_ADDR_LEN);
    pstSmCtx->stSearchCfg.ucNvProtocol = nv.protocol;
    pstSmCtx->stSearchCfg.usNvDI97 = nv.DI_v97;
    pstSmCtx->stSearchCfg.ucBaudRateIndex = nv.baudrate_index;

    return ret;
}


HI_U32 SmInitCfgNvDefault(NV_APP_SEARCHMETER_CFG_STRU * cfg)
{
    HI_ASSERT(HI_NULL != cfg);

    (hi_void)memset_s(cfg, sizeof(NV_APP_SEARCHMETER_CFG_STRU), 0, sizeof(NV_APP_SEARCHMETER_CFG_STRU));

    cfg->search_di_07 = MRS_SM_DI_VER2007;
    cfg->search_di_97 = MRS_SM_DI_VER1997;
    cfg->search_di_97b = MRS_SM_DI_VER1997B;
    cfg->search_di_97c = MRS_SM_DI_VER1997C;
    cfg->frame_timeout = MRS_SM_FRAME_TIMEOUT;
    cfg->byte_timeout = MRS_SM_BYTE_TIMEOUT;
    cfg->meter_num_max = MRS_SM_MAX_METERMUM;
    cfg->search_bc_enable = MRS_SM_SEARCH_BC_ENABLE;
    cfg->search_retry = MRS_SM_SEARCH_RETRY;

    cfg->detect_loop_max = MRS_DETECT_BAUDRATE_LOOP_MAX;
    cfg->detect_timeout = MRS_DETECT_BAUDRATE_TIMEOUT;
    cfg->detect_period = MRS_DETECT_BAUDRATE_PERIOD;
    cfg->query_sm_interval = MRS_CLTI_SM_QUERY_INTERVAL;
    cfg->read_positive_A_times = MRS_CLTI_READ_POSITIVE_A_MAX;
    cfg->read_positive_A_interval = MRS_CLTI_READ_POSITIVE_A_INTERVAL;

    cfg->explore_byte_count_max = MRS_CLTII_SM_EXPLORE_BYTE_COUNT_MAX_DEFAULT;

    cfg->invalid_frm_len_07 = MRS_SM_INVALID_FRM_LEN_07;
    cfg->invalid_frm_len_97 = MRS_SM_INVALID_FRM_LEN_97;

    cfg->search_period = MRS_SM_SEARCH_PERIOD;
    cfg->ucDetectTimeOut_ucCltIQueryThreshold = MRS_CLTII_SM_DETECT_TIMEOUT;

    cfg->ucSearchEnableFlag_ucCltIPlcThreshold = MRS_SEARCH_ENABLE_FLAG_I | MRS_SEARCH_ENABLE_FLAG_II;

    return HI_ERR_SUCCESS;
}


HI_U32 InitSmCfgExpNvValue(NV_APP_STA_SM_CFG_EXP_STRU * cfg)
{
    HI_ASSERT(HI_NULL != cfg);

    (hi_void)memset_s(cfg, sizeof(NV_APP_STA_SM_CFG_EXP_STRU), 0, sizeof(NV_APP_STA_SM_CFG_EXP_STRU));

    cfg->usLeaveTimeLenTh   = MRS_SM_LEAVE_TIME_LEN_TH;
    cfg->ucStaRstPinEnable  = HI_TRUE;
    cfg->ulRstShielTime     = 0;

    return HI_ERR_SUCCESS;
}


HI_PUBLIC HI_VOID mrsSimuSetOpenCoverEvt(HI_U8 ucEvtFlg);

HI_U32 mrsNvSimuStateCfg(HI_U16 usId)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    MV_APP_SIMU_METER_INFO minfo = {0};

    //��ȡNV��
    ulRet = HI_MDM_NV_Read(usId,(HI_PVOID)&minfo,sizeof(minfo));
    if (HI_ERR_SUCCESS != ulRet)
    {
        minfo.ucMeterFlag = NV_SIMU_METER_DISABLE;
    }

    if ((minfo.ucMeterFlag & 0xF) == NV_SIMU_METER_ENABLE)
    {
        HI_U8 ucProtocol = METER_PROTO_645_2007;
        MRS_STA_SRV_CTX_STRU * pstStaCtx = mrsStaGetContext();
        MRS_SRV_QUEUE *pstQueue = &pstStaCtx->stQueModule.stMrQueue;
        MRS_STA_ITEM *pstItem = mrsSrvQueueTop(pstQueue);
        if (pstItem != HI_NULL)
        {
            if (MRS_STA_ITEM_ID_DETECT == pstItem->id)
            {
                MRS_StopTimer(MRS_STA_TIMER_TEST);
                mrsStaTryDeQueue(&pstStaCtx->stQueModule, mrsStaQueueFree);
            }
        }

        if (METER_PROTO_698_45 == ((minfo.ucMeterFlag >> 4) & 0xF))
        {
            /* ����698.45�� */
            ucProtocol = METER_PROTO_698_45;
        }

        mrsStaSetSupportVM(NV_SIMU_METER_ENABLE);

        mrsStaEvtSetEvtOut(0);

        mrsSyncVMAddrToMac(ucProtocol);

        mrsStaSetVMActiveDelay((HI_U32)minfo.ucDelay);
        if(minfo.ucEvtOutFlg)
        {
            mrsSimuSetOpenCoverEvt(HI_TRUE);
            mrsStaEvtOutStatusNotify(MRS_STA_EVT_FROM_SIMU, 1);
        }
        else
        {
            mrsStaEvtOutStatusNotify(MRS_STA_EVT_FROM_SIMU, 0);
        }

        mrsStaSetState(MRS_STA_STATE_MR);
        MRS_StartTimer(MRS_STA_TIMER_HEART, MRS_STA_TIME_HEART, HI_SYS_TIMER_PERIODIC);
        HI_MDM_LED_SetReadMeterAddrStatus(LED_READ_METER_SUCCESS);
    }
    else if(minfo.ucMeterFlag == NV_SIMU_METER_DISABLE)
    {

        mrsStaSetSupportVM(NV_SIMU_METER_DISABLE);
    }

    mrsDfxVmMeterStatus(minfo.ucMeterFlag, HI_TRUE);

    return HI_ERR_SUCCESS;
}

//���������ϱ����ַ����---NV��ı�֪ͨ
HI_U32 mrsNvStaReportFlagCfg(HI_U16 usId)
{
    MRS_NOT_USED(usId);

    //��ȡ��ǰ�����ϱ�����NV���ֵ
    mrsGetRepFlagNv();

    return HI_ERR_SUCCESS;
}

HI_U32 mrsNvCollectDevAddrCfg(HI_U16 usId)
{
    HI_U32 ret = HI_ERR_SUCCESS;

    ret = mrsSetCollectDevAddr(usId);

    return ret;
}


HI_U32 mrsNvInitBaudRateCfg(HI_U16 usId)
{
    return mrsSrvInitBaudRateCfgTable(usId);
}


HI_U32 mrsNvCltIIRefresh(HI_U16 usId)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    NV_APP_CLTII_CFG_STRU stCfg = {0};
    MRS_STA_SEARCH_CTX *pstSmCtx = mrsStaSearchCtx();

    MRS_NOT_USED(usId);

    ret = HI_MDM_NV_Read(ID_NV_APP_CTLII_CFG, &stCfg, sizeof(stCfg));

    if(ret != HI_ERR_SUCCESS)
    {
        stCfg.usTimingSendInterval = MRS_COLLECTOR_SEND_BOARD_INTERVAL_TIME;
        stCfg.usEvtMonitorInterval = MRS_CLTII_EVT_MONITOR_INTERVAL_DEFAULT;
        stCfg.ucEvtMonitorFlag = MRS_CLTII_EVT_MONITOR_FLAG_DEFAULT;
/* BEGIN: PN:  MODIFY\ADD\DEL by cuichunyan/00276962 at 2015/10/08 */
        stCfg.ucIrTrasmit645 = HI_FALSE;//Ĭ�Ϻ���͸�����ܹر�
/* END:   PN:  MODIFY\ADD\DEL by cuichunyan/00276962 at 2015/10/08 */
        stCfg.ucPowerSearchEnable = HI_TRUE;
/* BEGIN: Modified by cuichunyan/c00276962 2016/11/10  ���ⵥ��:cltii R485 MultiConnect */
        stCfg.ucR485Monitor = HI_TRUE;
/* END  : Modified by cuichunyan/c00276962 2016/11/10  ���ⵥ��:cltii R485 MultiConnect */
        stCfg.area_version = 0;

    }

    pstSmCtx->stSearchCfg.ucJSAreaVer = stCfg.area_version & 0x01;
    {
        MRS_COLLECTOR_CTX_STRU *pstCltIICtx = mrsGetCollectorCtx();

        pstCltIICtx->ulTimingSendInterval = (HI_U32)stCfg.usTimingSendInterval;
        if(pstCltIICtx->ulTimingSendInterval == 0)
        {
            pstCltIICtx->ulTimingSendInterval = MRS_COLLECTOR_SEND_BOARD_INTERVAL_TIME;
        }
/* BEGIN: PN: DTS2015102007386  MODIFY\ADD\DEL by cuichunyan/00276962 at 2015/10/21 */
        pstCltIICtx->ucIrTrasmit645 = stCfg.ucIrTrasmit645;
/* END: PN: DTS2015102007386  MODIFY\ADD\DEL by cuichunyan/00276962 at 2015/10/21 */
        pstCltIICtx->ucPowerSearchEnable = stCfg.ucPowerSearchEnable;
        pstCltIICtx->ucRunLedMode = stCfg.ucRunLedMode;
/* BEGIN: Modified by cuichunyan/c00276962 2016/11/10  ���ⵥ��:cltii R485 MultiConnect */
        pstCltIICtx->ucR485Monitor = stCfg.ucR485Monitor;
/* END  : Modified by cuichunyan/c00276962 2016/11/10  ���ⵥ��:cltii R485 MultiConnect */    }

    {
        MRS_CLTII_EVENT_CTX_STRU *pstCtliiEvtCtx = mrsGetCltiiEventCtx();

        if (pstCtliiEvtCtx)
        {
            pstCtliiEvtCtx->stNvInf.ucMonFlag = stCfg.ucEvtMonitorFlag;
            pstCtliiEvtCtx->stNvInf.usMonInterval = stCfg.usEvtMonitorInterval;

            if (pstCtliiEvtCtx->stNvInf.usMonInterval < MRS_CLTII_EVT_MONITOR_INTERVAL_MIN)
            {
                pstCtliiEvtCtx->stNvInf.usMonInterval = MRS_CLTII_EVT_MONITOR_INTERVAL_DEFAULT;
            }
        }
    }

    return ret;
}
HI_U32 mrsStaReloadNvVerCfg(HI_VOID)
{
	HI_U32 ulRet = HI_ERR_SUCCESS;
   	FLASH_USR_CFG_INFO_S stCfg;
    	MRS_STA_SRV_CTX_STRU *pstStaCtx = mrsStaGetContext();
	NV_MRS_USER_VER_S stNv;

	do
	{
		(hi_void)memset_s(&stNv,sizeof(NV_MRS_USER_VER_S),0,sizeof(NV_MRS_USER_VER_S));
		(hi_void)memset_s(&stCfg, sizeof(stCfg), 0,sizeof(stCfg));
    		ulRet = mrsSgdFlashRead(ID_FLASH_USR_CFG, &stCfg, sizeof(stCfg));
		if(ulRet != HI_ERR_SUCCESS)
		{
			 HI_DIAG_LOG_MSG_E1(__LINE__, HI_DIAG_MT("��ERR��Read flash =%d"),ulRet);
			break;
    		}
		if(pstStaCtx)
		{
			(hi_void)memcpy_s(pstStaCtx->aucCode, sizeof(pstStaCtx->aucCode), stCfg.aucCode,sizeof(pstStaCtx->aucCode));
		}
		 ulRet = mrsGetUserVerNvCfg(&stNv);
        	if (HI_ERR_SUCCESS != ulRet)
        	{
           		 HI_DIAG_LOG_MSG_E1(__LINE__, HI_DIAG_MT("��ERR��Read 8009 =%d"),ulRet);
            		break;
        	}
		if(stNv.reserved[21] != stCfg.aucCode[0] || stNv.reserved[22] != stCfg.aucCode[1] )
		{
			stNv.reserved[21] = stCfg.aucCode[0];
			stNv.reserved[22] = stCfg.aucCode[1];
			ulRet = HI_MDM_NV_Write(ID_NV_MRS_USER_VER, &stNv, sizeof(stNv));
        		if (HI_ERR_SUCCESS != ulRet)
        		{
            			HI_DIAG_LOG_MSG_E1(__LINE__, HI_DIAG_MT("��ERR��SET 8009 ret"),ulRet);
            			break;
        		}
		}
	}while(0);
	return ulRet;
}
HI_U32 mrsNvStaClietIdCfg(NV_MRS_USER_VER_S * pInfo)
{
	HI_U32 ulRet = HI_ERR_SUCCESS;
   	FLASH_USR_CFG_INFO_S stCfg;
    	MRS_STA_SRV_CTX_STRU *pstStaCtx = mrsStaGetContext();
	HI_U8 codeFlg = 0;
	(hi_void)memset_s(&stCfg, sizeof(stCfg),0, sizeof(stCfg));
	if(!pstStaCtx ||((pstStaCtx->aucCode[0] == pInfo->reserved[21]) &&(pstStaCtx->aucCode[1] == pInfo->reserved[22])))
	{
		return ulRet;
	}
	do
	{
    		ulRet = mrsSgdFlashRead(ID_FLASH_USR_CFG, &stCfg, sizeof(stCfg));
		if(ulRet == HI_ERR_SUCCESS)
		{
			
			if((pstStaCtx->aucCode[0] != pInfo->reserved[21]) ||(pstStaCtx->aucCode[1] != pInfo->reserved[22]))
			{
				codeFlg = 1;
				stCfg.aucCode[0] = pInfo->reserved[21];
				stCfg.aucCode[1] = pInfo->reserved[22];
			}
		}
	}while(0);
	if(codeFlg)
	{
		ulRet = mrsSgdFlashWrite(ID_FLASH_USR_CFG, &stCfg, sizeof(stCfg));
	}
	return ulRet;
}

#if defined(MRS_SRV_TSTA_PHASE_RECOGNITION)
HI_VOID mrsNvGetStaType(HI_VOID)
{
	HI_U32 ulRet = HI_ERR_SUCCESS;
	HI_NV_FTM_COLLECT_PARAM_FILE_S stParam;
	MRS_STA_SRV_CTX_STRU *pstStaCtx = mrsStaGetContext();
	
	(hi_void)memset_s(&stParam, sizeof(stParam),0, sizeof(stParam));
	ulRet = HI_MDM_NV_Read(HI_NV_FTM_COLLECT_PARAM_FILE, &stParam, sizeof(HI_NV_FTM_COLLECT_PARAM_FILE_S));
	if (HI_ERR_SUCCESS != ulRet)
	{
		return;
	}
	pstStaCtx->isTsta = stParam.isTsta;
}
#endif

#endif
