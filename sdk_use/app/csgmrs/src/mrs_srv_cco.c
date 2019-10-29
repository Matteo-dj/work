//*****************************************************************************
//
//                  ��Ȩ���� (C), 1998-2011, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mrs_srv_cco.c
//  �� �� ��   : V1.0
//  ��    ��   : liuxipeng/KF54842
//  ��������   : 2011-11-29
//  ��������   : CCO�˴�������ʵ��
//
//  �����б�   :
//  �޸���ʷ   :
//  1.��    �� : 2011-11-29
//    ��    �� : liuxipeng/KF54842
//    �޸����� : �����ļ�
//
//*****************************************************************************
#include "mrs_common.h"
#include "mrs_cmd_msg.h"
#include "mrs_fw_proto376_2.h"
#include "mrs_fw_proto645.h"
#include "mrs_fw_tools.h"
#include "mrs_fw_proto376_2_echo.h"
#include "mrs_fw_proto_csg_echo.h"
#include "mrs_fw_proto_csg_echo_cco.h"
#include "mrs_fw_proto376_2_frame.h"
#include "mrs_fw_log.h"
#include "mrs_fw_n.h"
#include "mrs_srv_common.h"
#include "mrs_fw_nv.h"
#include "mrs_srv_cco_macmap.h"
#include "mrs_srv_parallel_cco.h"
#include "mrs_srv_cco_broadcast.h"
#include "mrs_srv_list.h"
#include "mrs_srv_cco_event.h"
#include "mrs_srv_cco_remote_upg.h"
#include "mrs_srv_cco.h"
#include "mrs_srv_cco_readmeter.h"
#include "mrs_srv_anytime_readmeter.h"
#include "mrs_srv_cco_searchmeter.h"
#include "mrs_srv_cco_archives.h"
#include "mrs_srv_io.h"
#include <app_nv.h>
#include "app_config.h"
#include "mrs_dfx.h"
#include "hi_mdm.h"
#if defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG)
#include "mrs_srv_mrslog.h"
#endif
#include "mrs_srv_readmeter_manage.h"
#include "mrs_srv_cco_queue.h"
#include "mrs_srv_readmeter_config.h"
#include "mrs_dfx_readmeter_config.h"
#include "mrs_srv_cco_slavenode_on_off_line.h"
#include "mrs_srv_cco_testmode.h"
#include "mrs_srv_io_dev.h"
#include "equip_dut_proc.h"
#include "mrs_srv_cco_detect_rm.h"
#include "mrs_srv_cco_mr_task.h"
#include "mrs_srv_csg_plc.h"
#include "mrs_srv_csg_plc_cco.h"
#include "mrs_dfx_cco_remote_upg.h"
#include "hi_phy_nv_v200.h"
#include "mrs_power_failure.h"

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)

#if defined(PRODUCT_CFG_FEATURE_UT)
#define HI_SDM_VERIFY_RA(_addr, _v3, _v4)
#endif


#define MRS_CCO_INIT_CTX_STRU_MEM(ptr, size, type)  \
    do { \
        ptr = (type *)mrsToolsMalloc(size); \
        if (ptr) \
        { \
            (hi_void)memset_s((ptr), (size), 0, (size)); \
        } \
        else \
        { \
            HI_SDM_VERIFY_RA((ptr), 0, 0);\
        } \
    } while (0)

#define MRS_CCO_INIT_CTX_FUNC(x) \
    do { \
        if (HI_ERR_SUCCESS != (x)) \
        { \
            HI_MDM_ReportInitEvt(HI_DEID_APP_READ_NV_FAIL, HI_DIAG_SYS_INIT_SET); \
        } \
        else \
        { \
            HI_MDM_ReportInitEvt(HI_DEID_APP_READ_NV_OK, HI_DIAG_SYS_INIT_SET); \
        } \
    } while (0)


//*****************************************************************************
// ȫ��������
//*****************************************************************************
HI_PRV MRS_CCO_SRV_CTX_STRU* g_pstMrsCcoSrvCtx = HI_NULL;

HI_PRV HI_U32 mrsCcoAddrInit(MRS_CCO_SRV_CTX_STRU *pstCcoCtx);

HI_U32 mrsMsgCcoPlcChlStatusNotify(HI_U16 usMsgId, HI_IN HI_PVOID pMsgParam);
HI_U32 mrsRmTopoSceneProc(HI_MAC_NETWORK_TOPO_S* pstTopoInf);
HI_U32 mrsSetRmTopoScene(HI_U8 ucScene);
HI_U32 mrsCmdCcoDataTransmit(HI_IN HI_U16 id, HI_PVOID req_packet, HI_U16 packet_size);
HI_U32 mrsCmdCcoSearchMeter(HI_IN HI_U16 id, HI_PVOID req_packet, HI_U16 packet_size);
HI_U32 mrsCmdCcoPlcTestTransmit(HI_IN HI_U16 id, HI_PVOID req_packet, HI_U16 packet_size);
HI_U32 mrsCmdCcoGetDeviceInf(HI_U16 id, HI_PVOID req_packet, HI_U16 req_packet_size);
HI_U32 mrsMsgCcoDeviceDataRx(HI_U16 usMsgId, HI_IN HI_PVOID pMsgParam);
HI_VOID mrsStaCommTimerProcess(MRS_CCO_SRV_CTX_STRU* pstCcoCtx);
HI_VOID mrsSearchMeterEndTimerProcess(MRS_CCO_SRV_CTX_STRU* pstCcoCtx);
HI_VOID mrsGetTimeTimerProcess( MRS_CCO_SRV_CTX_STRU* pstCcoCtx);
HI_U32 mrsMsgCcoTimer(HI_U16 usMsgId, HI_IN HI_PVOID pMsgParam);
HI_U32 mrsCcoArchivesSetMacList(HI_U16 usMeterNum, HI_MAC_VALID_MAC_LIST_STRU * pstMeterList);
HI_VOID mrsSetCheckMode(HI_VOID);
HI_VOID mrsSetPowerAfe(HI_VOID);
HI_VOID mrsRecoverPowerAfe(HI_VOID);

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
HI_U32 mrsMsgCcoDeviceDataRxCsg(HI_U16 usMsgId, HI_IN HI_PVOID pMsgParam);
HI_U32 mrsCmdCcoCsgDataTransmit(HI_U16 usId, HI_PVOID pReqPacket, HI_U16 usPacketSize);
#endif


// ��ȡPLC�շ�ģʽ
HI_U8 mrsCcoGetPlcRxTxMode(HI_VOID);

// ��ȡCCO��ȫ�������ĵĽṹָ��
MRS_CCO_SRV_CTX_STRU* mrsCcoGetContext()
{
    return g_pstMrsCcoSrvCtx;
}

// ��ȡPLC��ʱ��Ϣ�ṹAFN-13
PLC_OVERTIME_INF_STRU *mrsGetPlcOverTimeInfAfn13(HI_VOID)
{
    return &mrsCcoGetContext()->stAnytimeRMModel.stOverTimeInf;
}

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
PLC_OVERTIME_INF_STRU *mrsGetCsgPlcOverTimeInf(HI_VOID)
{
    return &mrsCcoGetContext()->stAnytimeRMModel.stCsgOverTimeInf;
}

HI_VOID mrsSetCsgPlcOverTimeInf(HI_U8 ucPriority)
{
    PLC_OVERTIME_INF_STRU *pstInfo = mrsGetPlcOverTimeInfAfn13();
    PLC_OVERTIME_INF_STRU *pstCsgInfo= mrsGetCsgPlcOverTimeInf() ;

    HI_U8 ucMaxTimeout = MRS_RM_CSG_MAX_SERVICE_TIME_DEFAULT;

    if (ucPriority < MRS_CCO_MR_TASK_PRIORITY_3)
    {
        ucMaxTimeout = pstCsgInfo->MaxTimeout;
    }

    pstInfo->MaxTimeout = ucMaxTimeout;
}
#endif

// ��ȡPLC��ʱ��Ϣ�ṹAFN-14
PLC_OVERTIME_INF_STRU *mrsGetPlcOverTimeInfAfn14(HI_VOID)
{
#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
    if (mrsCcoIsTestMode())
    {
        return mrsGetPlcOverTimeInfAfn13();
    }
#endif

    return &mrsCcoGetContext()->stReadMeterModel.stOverTimeInf;
}


MRS_CCO_BROADCAST_STRU *mrsGetBroadcastCtx(HI_VOID)
{
    return &mrsCcoGetContext()->stBroadcast;
}

HI_U32 mrsCcoSetState(HI_U32 state)
{
    MRS_CCO_SRV_CTX_STRU * pstCtx = mrsCcoGetContext();
    HI_U32 old = pstCtx->status;

    pstCtx->status = state;
    mrsDfxCcoStatus((HI_U8)state);

    return old;
}

// ǿ��ͨ������
HI_VOID mrsCcoForcePlcConnect(HI_VOID)
{
    MRS_CCO_SRV_CTX_STRU * pstCtx = mrsCcoGetContext();
    pstCtx->PlcChStatus = HI_TRUE;
}


HI_U32 mrsCcoGetState()
{
    MRS_CCO_SRV_CTX_STRU * pstCtx = mrsCcoGetContext();
    return pstCtx->status;
}


HI_BOOL mrsGetNeedGetTimeFlg()
{
    return mrsCcoGetContext()->bNeedGetTime;
}

HI_VOID mrsSetNeedGetTimeFlg(HI_BOOL flg)
{
    mrsCcoGetContext()->bNeedGetTime = flg;
}

HI_VOID mrsCcoGlobalInit(HI_VOID)
{
    g_pstMrsCcoSrvCtx = HI_NULL;
}

HI_VOID mrsCcoInitCtx(HI_VOID)
{
    MRS_CCO_SRV_CTX_STRU *pstCcoCtx = HI_NULL;

    if (g_pstMrsCcoSrvCtx)
    {
        // ���� ���ö�ʱ����ʱMRS_SRV_CCO_TIMER_READMETER_INIT_ACTIV_VALUEʱ��󼤻����ģ��
        MRS_StartTimer(EN_MRS_SRV_CCO_READMETER_ACTIV_TIMER, MRS_SRV_CCO_TIMER_READMETER_REACTIV_VALUE, HI_SYS_TIMER_ONESHOT);
        return;
    }

    pstCcoCtx = (MRS_CCO_SRV_CTX_STRU *)mrsToolsMalloc(sizeof(MRS_CCO_SRV_CTX_STRU));
    if (pstCcoCtx)
    {
        (hi_void)memset_s(pstCcoCtx, sizeof(MRS_CCO_SRV_CTX_STRU), 0, sizeof(MRS_CCO_SRV_CTX_STRU));
        mrsSrvInitList(&(pstCcoCtx->stAutoSearchModel.stAbnormalCltii));
        mrsSrvInitList(&(pstCcoCtx->stAutoSearchModel.stSearchResultMeter));
        mrsSrvInitList(&(pstCcoCtx->stAutoSearchModel.stSearchResultClt));
    }
    else
    {
        HI_MDM_ReportInitEvt(HI_DEID_APP_CCO_CTX_INIT_FAIL, HI_DIAG_SYS_INIT_SET);
        HI_SDM_VERIFY_RA(pstCcoCtx, 0, 0);
        return;
    }

    g_pstMrsCcoSrvCtx = pstCcoCtx;

    MRS_CCO_INIT_CTX_FUNC(mrsSrvArchivesInit());

    // �����ϱ�����Ϣ�����ڴ�ռ�
    MRS_CCO_INIT_CTX_STRU_MEM(  pstCcoCtx->stAutoSearchModel.pReportedNodeInf,
                                sizeof(MR_NODE_METER_ADD_STRU) * PRODUCT_CFG_MRS_MAX_NODE_NUM,
                                MR_NODE_METER_ADD_STRU);

    MRS_CCO_INIT_CTX_STRU_MEM(  pstCcoCtx->stReadFail.pReportedNodeInf,
                                sizeof(MR_NODE_METER_ADD_STRU) * PRODUCT_CFG_MRS_MAX_NODE_NUM,
                                MR_NODE_METER_ADD_STRU);

    MRS_CCO_INIT_CTX_STRU_MEM(pstCcoCtx->pTopoIntegerInfo, sizeof(HI_MAC_NETWORK_TOPO_S), HI_U8);

    // ��NV����ز����ڵ��ַ
    MRS_CCO_INIT_CTX_FUNC(mrsCcoAddrInit(pstCcoCtx));
    HI_MDM_DevIdConfig(pstCcoCtx->ucMainNodeAddr);

    MRS_CCO_INIT_CTX_FUNC(mrsInitPlcTimeOutMaxNum());

    MRS_CCO_INIT_CTX_FUNC(mrsNvMaxTurnNumCfg(ID_NV_APP_MAX_TURN_NUM));

    MRS_CCO_INIT_CTX_FUNC(mrsInitAppMrNvInf());

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
    MRS_CCO_INIT_CTX_FUNC(mrsCcoProtoCsgEchoInit());
    MRS_CCO_INIT_CTX_FUNC(mrsCcoCsgPlcModuleInit());
#endif

    mrsNvDutFlagCfg(ID_NV_APP_MR_DUT);
    mrsNvCcoSearchMeterCfg(ID_NV_APP_SEARCHMETER_CFG);
    mrsNvCcoSearchMeterCfgExp(ID_NV_APP_CCO_SM_CFG_EXP);

    HI_MDM_NV_RegisterChangedNoitfyProc(HI_NV_FTM_CCO_PARAM_ID, HI_NV_FTM_CCO_PARAM_ID, mrsNvChangeMsgNotify);
    HI_MDM_NV_RegisterChangedNoitfyProc(ID_NV_APP_MR_CFG, ID_NV_APP_CCO_SM_CFG_EXP, mrsNvChangeMsgNotify);
    HI_MDM_NV_RegisterChangedNoitfyProc(ID_NV_MRS_USER_VER, ID_NV_MRS_USER_VER, mrsNvChangeMsgNotify);

#if defined(PRODUCT_CFG_SYS_RST_SOLUTION)
    HI_MDM_RstRegisterNotify(mrsCcoRMPlcStat, HI_SYS_RST_CON_ANY_MR_FAIL, 0);
    HI_MDM_RstRegisterNotify(mrsCcoRxUartStat, HI_SYS_RST_CON_NOT_GET_FARME, 0);
#endif

    mrsCcoEvtInit();
    mrsCcoQueueInit();
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
    mrsCcoMrTaskInit();
#endif

    mrsCcoNodeStateChgReportInit();

#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
    mrsInitSustainAfn();
#endif

    //��ʼ��Զ������ģ��
    mrsCcoRemoteUpgInit();

    //��ʼ��ӳ���
    MRS_CCO_INIT_CTX_FUNC(mrsCcoMapInit(&pstCcoCtx->stMapModule));

    // ��ʼ������ģ��
    mrsRmModuleInit();

    MRS_CCO_INIT_CTX_FUNC(mrsRmCfgModuleInit());
    MRS_CCO_INIT_CTX_FUNC(mrsDfxMrRetryModuleInit());

    // ��ʼ����������
    mrsParallelReadMeterInit();

    // ����Ϊ��Ҫ��ȡ������ʱ��״̬
    mrsSetNeedGetTimeFlg(HI_FALSE);

    pstCcoCtx->bFirstRun = HI_TRUE;
    pstCcoCtx->stAnytimeRMModel.ulTimeOut = MRS_SRV_CCO_TIMER_DEFAULT_TIMEOUT_VALUE;  // ��ʼ���泭Ĭ�ϳ�ʱʱ��

    MRS_StartTimer(EN_MRS_SRV_CCO_GET_TIME_TIMER, 2000, HI_SYS_TIMER_ONESHOT); // ������ʱ��

    mrsCcoSetState(MRS_CCO_STATUS_CREATE_MAP);
    mrsCcoStatusInit(pstCcoCtx);
    mrsCcoDetectRmInit();
    HI_MDM_RegisterReportTopoStatusChg(mrsCcoTopoStatusChgEntryCallback);

    HI_MDM_ReportInitEvt(HI_DEID_APP_CCO_CTX_INIT_OK, HI_DIAG_SYS_INIT_SET);

    if(pstCcoCtx->plc_rxtx_mode == MRS_PLC_RX_TX_MODE_FORCE)
    {
        // ǿ��PLCͨ������
        mrsCcoForcePlcConnect();
    }

#if defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG)
    mrsInitMrsLogModule();
#endif

    return;
}

/* BEGIN: Added by fengxiaomin/00209182, 2013/12/11   ���ⵥ��:��:DTS2013122609670 */
HI_VOID mrsCcoStatusInit(MRS_CCO_SRV_CTX_STRU *pstCcoCtx)
{
    if (MRS_CCO_LR_MODE_SINGLE == pstCcoCtx->ucLrModeFlag)
    {
/* BEGIN: Modified by fengxiaomin/00209182, 2015/9/7   ���ⵥ�ź�:DTS2015091503701 */
        HI_U32 status = mrsCcoGetState();

        status &= ~MRS_CCO_STATUS_NORMAL;
        mrsCcoSetState(status);

        pstCcoCtx->bPauseFlag = HI_TRUE;
        mrsDfxLrRoute((HI_U8)pstCcoCtx->bPauseFlag);
/* END:   Modified by fengxiaomin/00209182, 2015/9/7 */
    }

    return;
}
/* END:   Added by fengxiaomin/00209182, 2013/12/11 */

/* BEGIN: Modified by fengxiaomin/00209182, 2014/3/28   ���ⵥ��:DTS2014032708937 */
HI_U32 mrsCcoAddrInit(MRS_CCO_SRV_CTX_STRU *pstCcoCtx)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_NV_FTM_CCO_PARAM_ID_S stCcoParam;

    (hi_void)memset_s(&stCcoParam, sizeof(stCcoParam), 0, sizeof(stCcoParam));
    ret = HI_MDM_NV_Read(HI_NV_FTM_CCO_PARAM_ID, &stCcoParam, sizeof(HI_NV_FTM_CCO_PARAM_ID_S));
    if (ret != HI_ERR_SUCCESS)
    {
        return ret;
    }

    (hi_void)memcpy_s(pstCcoCtx->ucMainNodeAddr, sizeof(pstCcoCtx->ucMainNodeAddr), stCcoParam.ucDevAddr, sizeof(pstCcoCtx->ucMainNodeAddr));

    return ret;
}
/* END:   Modified by fengxiaomin/00209182, 2014/3/28 */

//*****************************************************************************
// ��������: mrsCcoPowerPro
// ��������: MRSģ����CCO�ϵ紦����
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
/* BEGIN: Added by fengxiaomin/00209182, 2014/1/14   ���ⵥ�ź�:DTS2014012607684 */
HI_VOID mrsCcoPowerOnPro(HI_VOID)
{
/* BEGIN: Modified by fengxiaomin/00209182, 2014/3/28   ���ⵥ��:DTS2014032708937 */
    mrsSyncMeterInfToWhiteList(HI_MAC_WHIST_LIST_CHANGE_REASON_APP_INIT);
/* END:   Modified by fengxiaomin/00209182, 2014/3/28 */
    mrsPowerOnGetUpgStatus();

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO_BROAD_BAND_UNIT)
    if (mrsToolIsBbu())
    {
        MRS_StartTimer(MRS_CCO_TIMER_RUN_LED_BBU, MRS_STA_TIME_RUN_LED_0_50HZ, HI_SYS_TIMER_PERIODIC);
    }
#endif

    mrsSetCheckMode();
    mrsSetPowerAfe();

    return;
}
/* END:   Added by fengxiaomin/00209182, 2014/1/14 */

HI_VOID mrsSetPowerAfe(HI_VOID)
{
    return;
}

HI_VOID mrsRecoverPowerAfe(HI_VOID)
{
    return;
}

//when check mode is valid,set check mode and modify pallel num;otherwise,return
HI_U32 HI_MDM_GetCheckMode(HI_OUT HI_U8 *check_mode)
{
    if (!check_mode)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    *check_mode = HI_MDM_UNCHECK_MODE;

    return HI_ERR_SUCCESS;
}

HI_VOID mrsSetCheckMode(HI_VOID)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    MRS_CCO_SRV_CTX_STRU * cco_ctx = mrsCcoGetContext();

    ret = HI_MDM_GetCheckMode(&cco_ctx->ucCheckMode);
    if (HI_ERR_SUCCESS != ret)
    {
        cco_ctx->ucCheckMode = 0;
        return;
    }

    if (HI_MDM_CHECK_MODE == cco_ctx->ucCheckMode)
    {
        mrsSetParallelNum(MRS_PARALLEL_NUM_CHECK_MODE);
    }
}

HI_U8 mrsGetCheckMode(HI_VOID)
{
    MRS_CCO_SRV_CTX_STRU * cco_ctx = mrsCcoGetContext();

    return cco_ctx->ucCheckMode;
}

//�жϵ�ǰ��������С�ڵ���7,�����,��֧�������г���������е�������;���������ӱ����в���Ч
HI_U32 mrsCheckMeterNum(HI_VOID)
{
    HI_U16 usNum = mrsSrvArchivesMeterNum();

    if (MRS_CCO_ARCHIVES_STATUS_WAITING == mrsGetArchivesStatus())
    {
        return HI_ERR_FAILURE;
    }

    if (usNum <= MRS_CCO_ARCHIVES_NUM_MINI)
    {
        return HI_ERR_SUCCESS;
    }

    return HI_ERR_FAILURE;
}

// �ϱ�topo��ǰ״̬��Ϣ(ϵͳ�������Ϣ)
HI_PRV HI_VOID mrsReportMacTopoStatus(HI_MAC_NETWORK_TOPO_S * topo_inf)
{
    HI_DMID_APP_MACTOPO_STATUS_S * pbuf = HI_NULL;
    HI_U16 buflen = sizeof(HI_DMID_APP_MACTOPO_STATUS_S);
    HI_U16 i = 0;

    pbuf = (HI_DMID_APP_MACTOPO_STATUS_S *)mrsToolsMalloc(buflen);
    if(!pbuf)
    {
        return;
    }

    (hi_void)memset_s(pbuf, buflen, 0, buflen);

    for(i = 0; i < MRS_MIN(topo_inf->num, HI_ARRAY_COUNT(pbuf->mac_list)); i++)
    {
        (hi_void)memcpy_s(pbuf->mac_list[i].mac, sizeof(pbuf->mac_list[i].mac), topo_inf->entry[i].mac, HI_PLC_MAC_ADDR_LEN);
    }

    pbuf->num = (HI_U16)topo_inf->num;

    HI_MDM_ReportSysMessage(HI_DMID_APP_MACTOPO_STATUS,
                                HI_MOD_ID_SAL_DMS,
                                HI_MOD_ID_APP_COMMON,
                                (HI_PVOID)pbuf, buflen,
                                HI_SYS_DMID_BUSINESS_L2);

    mrsToolsFree(pbuf);

    return;
}

// Map��ά������
// option : MRS_MAP_MAINTAIN_MODE_ACTIVE          ����ά��
// option : MRS_MAP_MAINTAIN_MODE_PASSIVE         ӳ���ά��ģʽ(����)-Topo�������
// option : MRS_MAP_MAINTAIN_MODE_REFRESH         ӳ���ά��ģʽ(����ˢ��)-��ʱ������
HI_U32 mrsMapMaintainProcess(HI_U8 option)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_MAC_NETWORK_TOPO_S * topo_inf = HI_NULL;
    MRS_CCO_SRV_CTX_STRU * cco_ctx = mrsCcoGetContext();

    topo_inf = &cco_ctx->stTopoInf;

    if(option == MRS_MAP_MAINTAIN_MODE_PASSIVE) // Topo�������
    {
        mrsReportMacTopoStatus(topo_inf);
    }

    if(option != MRS_MAP_MAINTAIN_MODE_REFRESH)
    {
        MRS_StopTimer(EN_MRS_SRV_CCO_MAP_REFRESH_TIMER);
        cco_ctx->stMapModule.curr_turn = 0;
    }

    if (topo_inf->num > 0)
    {
        HI_MAC_NETWORK_TOPO_ENTRY_S * entry = &topo_inf->entry[1]; // �ӵڶ�����ʼ(��һ��ΪCCO)
        HI_U16 num = (HI_U16)(topo_inf->num - 1);

        if (cco_ctx->stMapModule.ucMapEnable)
        {
            //���TOPO��ı䣬����NewMacList
            mrsMapRecheckMacMeter(&cco_ctx->stMapModule, entry, num);

            // �Ƚϳ��ı�����,����ӳ���ά��
            ret = mrsMapMaintain(&cco_ctx->stMapModule);
        }
        else
        {
            MRS_StopTimer(MRS_CCO_TIMER_MAP);

            if (cco_ctx->stMapModule.stNewMacList.number != 0)
            {
                cco_ctx->stMapModule.stNewMacList.number = 0;
                cco_ctx->stMapModule.stNewMacList.current = 0;
                (hi_void)memset_s(cco_ctx->stMapModule.stNewMacList.mac,
									sizeof(MRS_MAC_INFO) * PRODUCT_CFG_MRS_MAX_TOPO_NUM,0,
									sizeof(MRS_MAC_INFO) * PRODUCT_CFG_MRS_MAX_TOPO_NUM);
            }

            if (mrsCcoGetState() & MRS_CCO_STATUS_CREATE_MAP)
            {
                mrsCcoMapToNormalPro();
            }
        }
    }

    return ret;
}

HI_VOID mrsCcoTopoStatusChgEntryCallback(HI_MDM_TOPO_STATUS_CHG_ENTRY_S* pstEntry)
{
    HI_SYS_QUEUE_MSG_S stMsg = { 0 };

    stMsg.ulMsgId = EN_MRS_FW_MSG_TOPO_CHANGE_NOTIFY;
    stMsg.ulParam[0] = pstEntry->ucStatus;
    (HI_VOID)mrsSendMessage2Queue(&stMsg);
}

HI_VOID mrsCcoTopoStatusChgProc(HI_SYS_QUEUE_MSG_S* pstMsg)
{
    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();

    if ((HI_MDM_MAC_NETWORK_FORMED == pstMsg->ulParam[0]) || (HI_MDM_MAC_NETWORK_EVALUATE_END == pstMsg->ulParam[0]))
    {
        mrsSetTopoStatus((HI_U8)pstMsg->ulParam[0]);
    }

    mrsRefreshTopoInfo(pstCcoCtx);
    mrsCcoDetectTopoChgProc((HI_U8)pstMsg->ulParam[0]);

    //���������ģ�޸Ĳ���������
    //�����ģС��nv���õĲ�����ʱ����nvΪ׼;������5С�ڵ���7ʱ,վ������Ϊ������,����ߵ��Ժ���Գ�������
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
    if ((pstCcoCtx->stTopoInf.num <= (MRS_CCO_ARCHIVES_NUM_MINI + 1))
        && ((pstCcoCtx->stTopoInf.num - 1) > mrsGetNvParallelNum()))
    {
        mrsSetParallelNum((HI_U16)(pstCcoCtx->stTopoInf.num - 1));
    }
    else
    {
        mrsSetParallelNum(mrsGetNvParallelNum());
    }
#endif
}

//*****************************************************************************
// ��������: mrsMsgCcoPlcChlStatusNotify
// ��������: ͨ��������Ϣ������
//
// ����˵��:
//   usMsgId [in] ��ϢID
//                ...
//   pMsgParam [in] ��Ϣ�ṹ
//
// �� �� ֵ:
//    HI_ERR_SUCCESS �ɹ�
//    ������
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: liuxipeng/KF54842 [2011-12-08]
// ��    ��: liupeidong/00238134 [2015-02-16]
//*****************************************************************************
HI_U32 mrsMsgCcoPlcChlStatusNotify(HI_U16 usMsgId, HI_IN HI_PVOID pMsgParam)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();
    MRS_MSG_PLC_CHL_STATUS_STRU * pstChlStatus = pMsgParam;
    HI_BOOL bPlcStatus = pstCcoCtx->PlcChStatus;

    HI_UNREF_PARAM(usMsgId);

    if(pstChlStatus == HI_NULL)
    {
        return HI_ERR_FAILURE;
    }

#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
    if (!mrsCcoIsTestMode() && (pstCcoCtx->status & MRS_CCO_STATUS_NORMAL))
    {
        if ((pstCcoCtx->PlcChStatus == HI_FALSE) && (pstChlStatus->bAvail == HI_TRUE))
        {
            mrsReadMeterStateInit();    // ���¿�ʼ��������

            // ��ʱMRS_SRV_CCO_TIMER_READMETER_ACTIV_VALUEʱ������¼������ģ��
            MRS_StartTimer(EN_MRS_SRV_CCO_READMETER_ACTIV_TIMER, MRS_SRV_CCO_TIMER_READMETER_ACTIV_VALUE, HI_SYS_TIMER_ONESHOT);
        }
    }
#endif

    mrsRefreshTopoInfo(pstCcoCtx);
    mrsRmTopoSceneProc(&pstCcoCtx->stTopoInf);

    if (pstChlStatus->bAvail) // ͨ������
    {
        MRS_REMOTE_UPG_MODULE *pstMrsUpgCtx = mrsGetRemoteUpgCtx();
        pstCcoCtx->PlcChStatus = HI_TRUE;

        if(!pstCcoCtx->stMapModule.first_notify)
        {
            // �����״��������ʱ��
            pstCcoCtx->stMapModule.first_notify = HI_MDM_GetMilliSeconds();
#if defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG)
            mrsStartMrsLogTimer();
#endif
        }
        ret = mrsMapMaintainProcess(MRS_MAP_MAINTAIN_MODE_PASSIVE);

        if (MRS_CCO_NEED_START_UPG_PLC_BAD == pstMrsUpgCtx->ucStartUpgFlag)
        {
            mrsUpgLocalPro(pstMrsUpgCtx);
        }
    }
    else
    {
        if(mrsCcoGetPlcRxTxMode() == MRS_PLC_RX_TX_MODE_FORCE)
        {
            pstCcoCtx->PlcChStatus = HI_TRUE;
            ret = HI_ERR_SUCCESS;
        }
        else
        {
            pstCcoCtx->PlcChStatus = HI_FALSE;
            ret = HI_ERR_FAILURE;  // ʧ��
        }

        mrsSetTopoStatus(0);
        mrsCcoDetectPlcDisconnectProc();
    }

    if (pstCcoCtx->PlcChStatus)
    {
        if ((pstCcoCtx->status & MRS_CCO_STATUS_SEARCH_METER_MASK) && (pstCcoCtx->ucSearchWaitFlag))
        {
            pstCcoCtx->ucSearchWaitFlag = HI_FALSE;
            mrsSearchMeterProc(pstCcoCtx);
        }

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
        if ((HI_MDM_CHECK_MODE != mrsGetCheckMode()) && (HI_FALSE == bPlcStatus) && mrsCcoMrTaskIsEnabled())
        {
            mrsCcoMrTaskNotify();
        }
#endif
    }

    return ret;
}


//*****************************************************************************
// ��������: mrsRmTopoSceneProc
// ��������: ��������˳������ô���
//
// ����˵��:
//   pstTopoInf [in] CCO����������е�������Ϣ
//
//
// �� �� ֵ:
//    HI_ERR_SUCCESS �ɹ�
//    HI_ERR_FAILURE ʧ��
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: liupeidong/00238134 [2015-02-16]
//*****************************************************************************
HI_U32 mrsRmTopoSceneProc(HI_MAC_NETWORK_TOPO_S* pstTopoInf)
{
    HI_U32 ulIndex = 0;
    HI_U8 ucProductType = 0;
    HI_U8 ucTopoScene = MRS_RM_SCENE_TOPO_HAS_NO_CLT; // Ĭ�ϳ�������: ���˲����ɼ�����ȫ�ز�

    // �����е�1���ڵ���CCO������
    for (ulIndex = 1; ulIndex < pstTopoInf->num; ulIndex++)
    {
        ucProductType = pstTopoInf->entry[ulIndex].product_type;
        if (HI_ND_TYPE_CLTI == ucProductType)
        {
            // ���˺�I�ɣ���������
            ucTopoScene = MRS_RM_SCENE_TOPO_HAS_CLTI;
            break;
        }
        else if (HI_ND_TYPE_CLTII == ucProductType)
        {
            // ���˺�II�ɣ���Ĭ�ϲ���I�ɣ���������
            ucTopoScene = MRS_RM_SCENE_TOPO_HAS_CLTII_NO_CLTI;
        }
    }

    return mrsSetRmTopoScene(ucTopoScene);
}


//*****************************************************************************
// ��������: mrsSetRmTopoScene
// ��������: �趨����ʱ�����˳�������
//
// ����˵��:
//   ucScene [in] ��������
//
//
// �� �� ֵ:
//    HI_ERR_SUCCESS �ɹ�
//    HI_ERR_FAILURE ʧ��
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: liupeidong/00238134 [2015-02-16]
//*****************************************************************************
HI_U32 mrsSetRmTopoScene(HI_U8 ucScene)
{
    MRS_CCO_SRV_CTX_STRU* pstCcoCtx = mrsCcoGetContext();
    pstCcoCtx->ucTopoScene = ucScene;
    return HI_ERR_SUCCESS;
}


//*****************************************************************************
// ��������: mrsGetRmTopoScene
// ��������: ��ȡ����ʱ�����˳�������
//
// ����˵��:
//   ��
//
//
// �� �� ֵ:
//    ��ǰ�������д�Ķ�����
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: liupeidong/00238134 [2015-02-16]
//*****************************************************************************
HI_U8 mrsGetRmTopoScene(HI_VOID)
{
    MRS_CCO_SRV_CTX_STRU* pstCcoCtx = mrsCcoGetContext();
    return pstCcoCtx->ucTopoScene;
}


//*****************************************************************************
// ��������: mrsRmBcTimeoutCltIDelayFlag
// ��������: �㲥����ĳ�ʱʱ���Ƿ��I�ɲ����ı�ʶ
//
// ����˵��:
//   ��
//
//
// �� �� ֵ:
//    HI_TRUE  ��I�ɲ���    (δ֪��������I�ɵĳ���)
//    HI_FALSE ����I�ɲ���  (��II����I�ɡ��޲ɼ���)
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: liupeidong/00238134 [2015-03-03]
//*****************************************************************************
HI_BOOL mrsRmBcTimeoutCltIDelayFlag(HI_VOID)
{
    HI_U8 ucTopoScene = mrsGetRmTopoScene();

    if ((MRS_RM_SCENE_TOPO_HAS_CLTII_NO_CLTI == ucTopoScene)
        || (MRS_RM_SCENE_TOPO_HAS_NO_CLT == ucTopoScene))
    {
        return HI_FALSE;
    }

    return HI_TRUE;
}


//*****************************************************************************
// ��������: mrsCcoRxAfn13DataPro
// ��������: �㳭��CCO�յ�PLC���ݰ���Ĵ���
//
// ����˵��:
//   pPayload       [in]   AFN-13���ݰ�
//   usPayloadLen   [in]   AFN-13���ݰ�����
//   ppData         [out]  ת���������е�����
//   pusDataLen     [out]  ת���������е����ݳ���
//
// �� �� ֵ:
//   HI_ERR_SUCCESS  �ɹ�
//   HI_ERR_FAILURE  ʧ��
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: fengxiaomin/f00209182 [2013-02-04]
// ��    ��: cuiate/00233580 [2015-09-16]
//*****************************************************************************
HI_U32 mrsCcoRxAfn13DataPro(HI_PVOID pPayload, HI_U16 usPayloadLen, HI_PVOID *ppData, HI_U16 *pusDataLen)
{
    HI_U16 usLocalLen = 0;
    MRS_DATA_TRANSMIT_AFN13_UP_STRU *pstTransmit = (MRS_DATA_TRANSMIT_AFN13_UP_STRU *)pPayload;

    if (pstTransmit->dir != MRS_PLC_UP_FLG)
    {
        return HI_ERR_BAD_FRAME;
    }

    *ppData = (HI_U8*)pstTransmit + pstTransmit->stru_size;
    *pusDataLen = pstTransmit->data_len;
    usLocalLen = sizeof(MRS_DATA_TRANSMIT_AFN13_UP_STRU) + pstTransmit->data_len;

    if (usPayloadLen != pstTransmit->stru_size + *pusDataLen)
    {
        HI_DIAG_LOG_MSG_E3(MRS_FILE_LOG_FLAG_007, HI_DIAG_MT("XR len err [%d][%d][%d]"),
            usPayloadLen, pstTransmit->stru_size, *pusDataLen);
        mrsDfxXrPlcLenErr();
        return HI_ERR_FAILURE;
    }

    switch (pstTransmit->interface_ver)
    {
    case MRS_AFN13_UP_VER_R0:
    default:
        if (usPayloadLen < usLocalLen)
        {
            HI_DIAG_LOG_MSG_E3(MRS_FILE_LOG_FLAG_009, HI_DIAG_MT("len err [%d][%d][%d]"),
                usPayloadLen, pstTransmit->stru_size, *pusDataLen);
            mrsDfxXrPlcLenErr();
            return HI_ERR_FAILURE;
        }
        break;
    }

    return mrsAnyTimeReadMeterReceivePlcProcess(pstTransmit);
}


#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
//*****************************************************************************
// ��������: mrsCcoRxAfn14DataPro
// ��������: �ֳ���CCO�յ�PLC���ݰ���Ĵ���
//
// ����˵��:
//   pPayload       [in]   AFN-14���ݰ�
//   usPayloadLen   [in]   AFN-14���ݰ�����
//   ppData         [out]  ת���������е�����
//   pusDataLen     [out]  ת���������е����ݳ���
//
// �� �� ֵ:
//   HI_ERR_SUCCESS  �ɹ�
//   HI_ERR_FAILURE  ʧ��
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: fengxiaomin/f00209182 [2013-02-04]
// ��    ��: cuiate/00233580 [2015-09-16]
//*****************************************************************************
HI_U32 mrsCcoRxAfn14DataPro(HI_PVOID pPayload, HI_U16 usPayloadLen, HI_PVOID *ppData, HI_U16 *pusDataLen)
{
    MRS_DATA_TRANSMIT_AFN14_UP_STRU* pstTransmit = (MRS_DATA_TRANSMIT_AFN14_UP_STRU *)pPayload;
    HI_U16 usLocalLen = 0;

    *ppData = (HI_U8*)pPayload + pstTransmit->stru_size;
    *pusDataLen = pstTransmit->data_len;
    usLocalLen = sizeof(MRS_DATA_TRANSMIT_AFN14_UP_STRU) + pstTransmit->data_len;

    if (usPayloadLen != pstTransmit->stru_size + *pusDataLen)
    {
        HI_DIAG_LOG_MSG_E3(MRS_FILE_LOG_FLAG_010, HI_DIAG_MT("LR len err [%d][%d][%d]"),
            usPayloadLen, pstTransmit->stru_size, *pusDataLen);
        mrsDfxLrPlcLenErr();
        return HI_ERR_FAILURE;
    }

    switch (pstTransmit->interface_ver)
    {
    case MRS_AFN14_UP_VER_R0:
    default:
        if (usPayloadLen < usLocalLen)
        {
            HI_DIAG_LOG_MSG_E3(MRS_FILE_LOG_FLAG_012, HI_DIAG_MT("len err [%d][%d][%d]"),
                usPayloadLen, pstTransmit->stru_size, *pusDataLen);
            mrsDfxLrPlcLenErr();
            return HI_ERR_FAILURE;
        }
        break;
    }

    return mrsReadMeterReceivePlcProcess(pstTransmit, mrsCcoGetContext());
}
#endif


//*****************************************************************************
// ��������: mrsIdCcoStaDataTransmit
// ��������: ����ת��(PLCͨ��)����
//
// ����˵��:
//   usId [in] ����ID
//   pstReq [in] PLC����֡
//
// �� �� ֵ:
//    HI_ERR_SUCCESS �ɹ�
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: liuxipeng/KF54842 [2011-11-24]
//*****************************************************************************
HI_U32 mrsCmdCcoDataTransmit(HI_IN HI_U16 id, HI_PVOID req_packet, HI_U16 packet_size)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    MRS_PLC_FRAME_DATA_STRU *plc_data = HI_NULL;
    MRS_CCO_SRV_CTX_STRU * cco_ctx = mrsCcoGetContext();
    MRS_645_FRAME_STRU frame = {0};
    HI_U8 * data_645 = HI_NULL;
    HI_U16 data_len = 0;
    HI_U8 * mac = HI_NULL;
    HI_U8 *pPayload = HI_NULL;
    HI_U16 usPayloadLen = 0;
    HI_U16 usId = id;

    MRS_NOT_USED(packet_size);

    //������
    plc_data = (MRS_PLC_FRAME_DATA_STRU *)req_packet;

    if (!plc_data || !plc_data->pucPayload)
    {
        return HI_ERR_FAILURE;
    }

    mac = plc_data->ucMac;
#if defined(PRODUCT_CFG_SYS_RST_SOLUTION)
    cco_ctx->stResetStatInfo.ulRMStatTimes++;
#endif

    pPayload = plc_data->pucPayload;
    usPayloadLen = plc_data->usPayloadLen;

    if (usId == ID_MRS_CMD_DATA_TRANSMIT_PLC)
    {
        MRS_CMD_DATA_TRANSMIT_STRU *pstTransmit = (MRS_CMD_DATA_TRANSMIT_STRU *)pPayload;

        switch (pstTransmit->usCmdId)
        {
        case ID_MRS_CMD_DATA_TRANSMIT_TEST_MODE:
            return mrsCcoTestModeFrameRx((MRS_CMD_TEST_MODE_STRU *)pstTransmit->aucPayload);

        case ID_MRS_CMD_DATA_TRANSMIT_AFN13_PLC:
        case ID_MRS_CMD_DATA_TRANSMIT_AFN14_PLC:
            if (mrsCcoIsTestMode())
            {
                pPayload = pstTransmit->aucPayload;
                usPayloadLen = pstTransmit->usPayloadLen;
                usId = pstTransmit->usCmdId;
                break;
            }
            // ��break;
        default:
            return HI_ERR_NOT_SUPPORT;
        }
    }

    switch (usId)
    {
    case ID_MRS_CMD_DATA_TRANSMIT_AFN13_PLC:
        mrsDfxXrPlcRx();
/* BEGIN: Modified by fengxiaomin/00209182, 2014/3/27   ���ⵥ��:DTS2014032906596 */
        mrsDfxRmPlcUpRx(ID_MRS_CMD_DATA_TRANSMIT_AFN13_PLC, pPayload, HI_FALSE);
/* END:   Modified by fengxiaomin/00209182, 2014/3/27 */
        if(cco_ctx->status & MRS_CCO_STATUS_ANYTIME_READMETER)
        {
            ret = mrsCcoRxAfn13DataPro(pPayload, usPayloadLen, (HI_PVOID*)&data_645, &data_len);

            if(plc_data->is_bc_frame == HI_TRUE)
            {
                mrsDfxBcRmOkCnt();
            }
            else
            {
                if(cco_ctx->stAnytimeRMModel.bIsBC)
                {
                    mrsDfxBcRmNonBcRespCnt();
                }
            }
        }
        break;

#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
    case ID_MRS_CMD_DATA_TRANSMIT_AFN14_PLC:
        HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_016, HI_DIAG_MT("AFN-14-status=%d"), cco_ctx->status);
        mrsDfxLrPlcRx();
        mrsDfxRmPlcUpRx(ID_MRS_CMD_DATA_TRANSMIT_AFN14_PLC, pPayload, HI_FALSE);

        if (cco_ctx->status & MRS_CCO_STATUS_NORMAL)
        {
            ret = mrsCcoRxAfn14DataPro(pPayload, usPayloadLen, (HI_PVOID*)&data_645, &data_len);
            HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_041, HI_DIAG_MT("AFN-14-pro-ret=%d"),ret);
        }
        break;

    case ID_MRS_CMD_PARALLEL_DATA_PLC:
        {
            MRS_DATA_TRANSMIT_AFNF1_UP_STRU * parallel_data = HI_NULL;

            mrsDfxPrPlcRx();
/* BEGIN: Modified by fengxiaomin/00209182, 2014/3/27   ���ⵥ��:DTS2014032906596 */
            mrsDfxRmPlcUpRx(ID_MRS_CMD_PARALLEL_DATA_PLC, pPayload, HI_FALSE);
/* END:   Modified by fengxiaomin/00209182, 2014/3/27 */
            parallel_data = (MRS_DATA_TRANSMIT_AFNF1_UP_STRU *)pPayload;
            ret = mrsRmPlcPackageReceive(parallel_data, usPayloadLen);
            data_645 = (HI_U8 *)parallel_data + parallel_data->stru_size;
            data_len = parallel_data->data_len;
        }
        break;

    case ID_MRS_CMD_EVENT_TRANSMIT_PLC:
        return ret; //�ɰ汾���¼�ID,�Ѿ�����,Ϊ��֤�¾ɰ汾����ҵ���������������ȱ���
#endif

    case ID_MRS_CMD_BROADCAST_TRANSMIT_PLC:
    case ID_MRS_CMD_TEST_TRANSMIT_PLC:
    default:
        break;
    }

    if (ret != HI_ERR_SUCCESS)
    {
        HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_019, HI_DIAG_MT("plc data process fail-[%d]"),ret);
        return HI_ERR_FAILURE;
    }

    // ά��ӳ���
    if (MRS_Proto645Dec(data_645, data_len,&frame) == HI_ERR_SUCCESS)
    {
        mrsRmMapProc(&frame, &cco_ctx->stMapModule, mac);
    }

    return ret;
}


HI_U32 mrsCmdCcoSearchMeter(HI_IN HI_U16 id, HI_PVOID req_packet, HI_U16 packet_size)
{
    HI_U32 ret = HI_ERR_SUCCESS;

    switch (id)
    {
    case ID_MRS_CMD_GET_DEVICE_INF_PLC:
        ret = mrsCmdCcoGetDeviceInf(id, req_packet, packet_size);
        break;

    case ID_MRS_CMD_GET_METERLIST:
        ret = mrsCmdCcoGetMeterList(id, req_packet, packet_size);
        break;

    case ID_MRS_CMD_START_SEARCH_METER:
    case ID_MRS_CMD_STOP_SEARCH_METER:
        break;

    default:
        ret = HI_ERR_NOT_FOUND;
    }

    return ret;
}


HI_U32 mrsCmdCcoPlcTestTransmit(HI_IN HI_U16 id, HI_PVOID req_packet, HI_U16 packet_size)
{
    MRS_PLC_FRAME_DATA_STRU *plc = (MRS_PLC_FRAME_DATA_STRU *)req_packet;
    MRS_TEST_TRANSMIT_AFNF0_STRU *trans = HI_NULL;
    PROTO376_2_FRAME_PRE_STRU frame = {0};

    HI_PBYTE buf = HI_NULL;
    HI_PBYTE payload = HI_NULL;
    HI_U16 bufLen = 0;
    HI_U16 payloadLen = 0;
    HI_U32 ret = HI_ERR_SUCCESS;

    HI_UNREF_PARAM(id);
    HI_UNREF_PARAM(packet_size);

    if (!req_packet)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    trans = (MRS_TEST_TRANSMIT_AFNF0_STRU *)plc->pucPayload;

    payloadLen = sizeof(HI_U16) + trans->expect_len;
    payload = mrsToolsMalloc(payloadLen);
    if (!payload)
    {
        return HI_ERR_MALLOC_FAILUE;
    }

    (hi_void)memcpy_s(payload, payloadLen, &(trans->expect_len), sizeof(HI_U16));
    (hi_void)memcpy_s(payload + sizeof(HI_U16), payloadLen - sizeof(HI_U16), plc->pucPayload + trans->stru_len, trans->expect_len);

    (hi_void)memset_s(&frame, sizeof(frame), 0, sizeof(frame));
    frame.ucModuleFlag = 1;
    (hi_void)memcpy_s(frame.ucSrcAdd, sizeof(frame.ucSrcAdd), plc->ucMac, HI_PLC_MAC_ADDR_LEN);
    mrsHexInvert(frame.ucSrcAdd, HI_PLC_MAC_ADDR_LEN);
    frame.stAfnData.ucAFN = MRS_AFN(0xF0);
    frame.stAfnData.ucDT[0] = MRS_AFN_FN_LO(1);
    frame.stAfnData.ucDT[1] = MRS_AFN_FN_HI(1);
    frame.stAfnData.usDataLen = payloadLen;
    frame.stAfnData.pData = payload;

    ret = mrsCreate3762Frame(&frame, &buf, &bufLen, HI_NULL);
    if (HI_ERR_SUCCESS == ret)
    {
        ret = MRS_SendMrData(buf, bufLen, HI_DMS_CHL_UART_PORT_APP);
    }

    mrsToolsFree(buf);
    mrsToolsFree(payload);

    return ret;
}


//*****************************************************************************
// ��������: mrsCmdCcoGetMeterAdd
// ��������: ��ȡ����ַ(PLCͨ��)����
//
// ����˵��:
//   usId [in] ����ID
//   pstReq [in/out] PLC����֡
//
// �� �� ֵ:
//    HI_ERR_SUCCESS �ɹ�
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: liuxipeng/KF54842 [2011-11-24]
//*****************************************************************************
HI_U32 mrsCmdCcoGetDeviceInf(HI_U16 id, HI_PVOID req_packet, HI_U16 req_packet_size)
{
    MRS_CCO_SRV_CTX_STRU *pstCcoCtx = mrsCcoGetContext();
    MRS_PLC_FRAME_DATA_STRU *pstPlcFrame = (MRS_PLC_FRAME_DATA_STRU *)req_packet;
    MRS_GET_DEVICE_INF_UP_STRU *pstUpFrame = HI_NULL;
    HI_U16 usDataLength = 0;

    HI_UNREF_PARAM(req_packet_size);
    HI_UNREF_PARAM(id);

    if (!pstPlcFrame || !pstPlcFrame->pucPayload)
    {
        return HI_ERR_FAILURE;
    }

    HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1000, HI_DIAG_MT("mac"), pstPlcFrame->ucMac, HI_PLC_MAC_ADDR_LEN);
    HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1001, HI_DIAG_MT("GET DEVICE INF PAYLOAD"), pstPlcFrame->pucPayload, pstPlcFrame->usPayloadLen);

    pstUpFrame = (MRS_GET_DEVICE_INF_UP_STRU *)pstPlcFrame->pucPayload;

/* BEGIN: PN: DTS2016021906890 MODIFY\ADD\DEL by cuiate/00233580 at 2016/2/21 */
    if ((pstUpFrame->interface_ver >= MRS_GET_METERLIST_UP_VER_R2) && !mrsSrvVerifyMac(pstUpFrame->dst))
    {
        mrsDfxMapFrameInvalid();
        return HI_ERR_FAILURE;
    }

    mrsDfxMapFrameValid();

    usDataLength = pstUpFrame->stru_len + sizeof(MRS_METER_INF_STRU) * pstUpFrame->meter_num;
    if (pstPlcFrame->usPayloadLen != usDataLength)
    {
        return HI_ERR_FAILURE;
    }
/* END:   PN: DTS2016021906890 MODIFY\ADD\DEL by cuiate/00233580 at 2016/2/21 */

    mrsMapDeleteMac(&pstCcoCtx->stMapModule, pstPlcFrame->ucMac);
    mrsMapMarkMac(&pstCcoCtx->stMapModule, pstPlcFrame->ucMac, pstUpFrame->stDeviceInf.ucStatus);

    if (MRS_MAP_DEV_TYPE_METER == pstUpFrame->stDeviceInf.ucType)
    {
        mrsMapAddMeter(&pstCcoCtx->stMapModule, pstPlcFrame->ucMac, &pstUpFrame->stDeviceInf);
    }
    else
    {
        HI_U8 i = 0;
        MRS_METER_INF_STRU *pstMeterInf = (MRS_METER_INF_STRU *)(pstPlcFrame->pucPayload + pstUpFrame->stru_len);
        MRS_DEVICE_INF_STRU stDevInf;

        mrsMapAddCollector(&pstCcoCtx->stMapModule, pstPlcFrame->ucMac, &pstUpFrame->stDeviceInf);
        (hi_void)memset_s(&stDevInf, sizeof(stDevInf), 0, sizeof(stDevInf));
        stDevInf.ucStatus = pstUpFrame->stDeviceInf.ucStatus;

        if (pstUpFrame->meter_num > PRODUCT_CFG_MRS_MAX_METER_IN_MAC_NUM)
        {
            return HI_ERR_FAILURE;
        }

        for (i = 0; i < pstUpFrame->meter_num; i++, pstMeterInf++)
        {
            mrsMeterSetDevInf(&stDevInf, pstMeterInf);

            if (HI_ERR_SUCCESS != mrsMapAddMeter(&pstCcoCtx->stMapModule, pstPlcFrame->ucMac, &stDevInf))
            {
                break;
            }
        }
    }

    return HI_ERR_SUCCESS;
}

#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
//*****************************************************************************
// ��������: mrsMsgCcoDeviceDataRx
// ��������: ���ռ��������� ��Ϣ����
//
// ����˵��:
//   usMsgId [in] ��ϢID
//   pMsgParam [in] ��Ϣ����
// �� �� ֵ:
//    HI_ERR_SUCCESS  �ɹ�
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: liuxipeng/KF54842 [2011-12-09]
//*****************************************************************************
HI_U32 mrsMsgCcoDeviceDataRx(HI_U16 usMsgId, HI_IN HI_PVOID pMsgParam)
{
    HI_U32 ret = HI_ERR_NOT_FOUND;

    MRS_MSG_MR_3762_DATA_STRU* pstData = HI_NULL;
    MRS_CCO_SRV_CTX_STRU* pstCcoCtx = mrsCcoGetContext();

    if (pMsgParam == HI_NULL)
    {
        return HI_ERR_FAILURE;
    }

    //CCO���յ�UART���ݣ�ͳ������1
#if defined(PRODUCT_CFG_SYS_RST_SOLUTION)
    pstCcoCtx->stResetStatInfo.ulCcoRxUartTimes++;
#endif

    pstData = (MRS_MSG_MR_3762_DATA_STRU*)pMsgParam;
    if (pstData->pucData)
    {
        HI_PBYTE pucDatagram  = pstData->pucData;
        HI_U16 usDatagramSize = pstData->usDataSizse;

        MRS_3762_FRAME_STRU* pstFrame = HI_NULL;

        ret = MRS_Proto3762Dec(pucDatagram, usDatagramSize, &pstFrame);
        if ((ret == HI_ERR_SUCCESS) && (pstFrame != HI_NULL))
        {
            HI_PBYTE pOutBuffer   = HI_NULL;
            HI_U16 usBufferOutLen = 0;

            ret = mrs3762AfnDispatch(pstFrame, &pOutBuffer, &usBufferOutLen);
            if (ret == HI_ERR_SUCCESS)
            {
                if (pOutBuffer != HI_NULL)
                {
                    ret = MRS_SendMrData(pOutBuffer, usBufferOutLen, HI_DMS_CHL_UART_PORT_APP); // ����"��Ӧ"֡
                }
            }
            mrsToolsFree(pOutBuffer);
        }
        mrsToolsFree(pstFrame);
    }

    return ret;
}
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
HI_U32 mrsMsgCcoDeviceDataRxCsg(HI_U16 usMsgId, HI_IN HI_PVOID pMsgParam)
{
    HI_U32 ulRet = HI_ERR_NOT_FOUND;

    MRS_MSG_MR_3762_DATA_STRU* pstData = HI_NULL;
    MRS_CCO_SRV_CTX_STRU* pstCcoCtx = mrsCcoGetContext();

    HI_UNREF_PARAM(usMsgId);

    if (pMsgParam == HI_NULL)
    {
        return HI_ERR_FAILURE;
    }

    //CCO���յ�UART���ݣ�ͳ������1
#if defined(PRODUCT_CFG_SYS_RST_SOLUTION)
    pstCcoCtx->stResetStatInfo.ulCcoRxUartTimes++;
#endif

    pstData = (MRS_DATA_BUFFER_STRU*)pMsgParam;
    if (pstData->pucData)
    {
        HI_PBYTE pucDatagram  = pstData->pucData;
        HI_U16 usDatagramSize = pstData->usDataSizse;

        MRS_CSG_FRAME_STRU* pstFrame = HI_NULL;

        ulRet = MRS_ProtoCsgDec(pucDatagram, usDatagramSize, &pstFrame);
        if ((ulRet == HI_ERR_SUCCESS) && (pstFrame != HI_NULL))
        {
            HI_PBYTE pOutBuffer   = HI_NULL;
            HI_U16 usBufferOutLen = 0;

            ulRet = mrsCsgAfnDispatch(pstFrame, &pOutBuffer, &usBufferOutLen);
            if (ulRet == HI_ERR_SUCCESS)
            {
                if (pOutBuffer != HI_NULL)
                {
                    ulRet = MRS_SendMrData(pOutBuffer, usBufferOutLen, HI_DMS_CHL_UART_PORT_APP); // ����"��Ӧ"֡
                }
            }
            mrsToolsFree(pOutBuffer);
        }
        mrsToolsFree(pstFrame);
    }

    return ulRet;
}
#endif


//*****************************************************************************
// ��������: mrsStaCommTimerProcess
// ��������: PLCͨѶ��ʱ����ʱ����
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
HI_VOID mrsStaCommTimerProcess(MRS_CCO_SRV_CTX_STRU* pstCcoCtx)
{
    if (pstCcoCtx->status & MRS_CCO_STATUS_NORMAL)
    {
#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
        mrsReadMeterStaTimerProcess(pstCcoCtx);
#endif
    }
    else if (pstCcoCtx->status & MRS_CCO_STATUS_ANYTIME_READMETER)
    {
        mrsAnyTimeReadMeterPlcTimeOutProcess(pstCcoCtx);
    }

    return;
}

HI_VOID mrsSearchMeterEndTimerProcess(MRS_CCO_SRV_CTX_STRU* pstCcoCtx)
{
    HI_U32 ulStatus = mrsCcoGetState(); // stop�ӿ��л�ı�status�������Ȱ�״̬ȡ����

    HI_UNREF_PARAM(pstCcoCtx);

    mrsCcoStopSearchMeterNormal((HI_U8)MRS_CCO_SEARCH_FINISH_TIMEOUT);

    if (ulStatus & MRS_CCO_STATUS_SEARCH_METER)
    {
        mrsReportRouterInfs(MRS_CCO_SEARCH_METER_TASK_END);
    }
}

HI_VOID mrsGetTimeTimerProcess( MRS_CCO_SRV_CTX_STRU* pstCcoCtx)
{
    if (pstCcoCtx->bFirstRun == HI_TRUE)
    {
        pstCcoCtx->bFirstRun = HI_FALSE;

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
        mrsCsg_AFN03_E8000302_UP();
#else
        if (pstCcoCtx->ucProtoVer == PROTO_VER_13762_2013)
        {
            mrs3762_AFN03_F10_UP();
        }
#endif
        mrsSetNeedGetTimeFlg(HI_FALSE);

        return;
    }

    return;
}

//*****************************************************************************
// ��������: mrsMsgCcoTimer
// ��������: ��ʱ����Ϣ�������
//
// ����˵��:
//   usMsgId [in] ��ϢID
//   pMsgParam [in] ��ϢID�ṹ
//
// �� �� ֵ:
//    HI_ERR_SUCCESS �ɹ�
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: liuxipeng/KF54842 [2011-12-09]
//*****************************************************************************
HI_U32 mrsMsgCcoTimer(HI_U16 usMsgId, HI_IN HI_PVOID pMsgParam)
{
    HI_U32 ret = HI_ERR_SUCCESS;

    MRS_MSG_MR_TIMER_STRU* pst = (MRS_MSG_MR_TIMER_STRU*)pMsgParam;
    MRS_CCO_SRV_CTX_STRU* pstCcoCtx = mrsCcoGetContext(); // ����ָ��ȫ�������Ľṹ��ָ��

    MRS_NOT_USED(usMsgId);

    if(!pst)
    {
        return HI_ERR_FAILURE;
    }

    switch (pst->ucTimerId)
    {
    case EN_MRS_SRV_CCO_MAP_VIN_TIMER:  // Mapά��ģ�鶨ʱ������
        mrsMapTimeout(&pstCcoCtx->stMapModule);
        break;

    case EN_MRS_SRV_CCO_STA_TIMER:      // ��STAͨѶ��ʱ������
        mrsStaCommTimerProcess(pstCcoCtx);
        break;

#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
    case EN_MRS_SRV_CCO_MUSTER_TIMER:   // �뼯����ͨѶ��ʱ������
        mrsReadMeterTotalMusterTimerProcess(HI_NULL);
        break;

    case EN_MRS_SRV_CCO_READMETER_ACTIV_TIMER:
        mrsReadMeterActiveTimer(pstCcoCtx);
        break;
#endif

    case MRS_CCO_TIMER_SEARCH_REPORT:
        mrsCcoSearchReportSlaveNodeInfo();
        break;

    case EN_MRS_SRV_CCO_SERVICE_END_TIMER:  // ����ֹͣ��ʱ������
        mrsAnyTimeReadMeterServiceTimeout(pstCcoCtx);
        break;

    case EN_MRS_SRV_CCO_WRITE_FLASH_ACTIV_TIMER: // �������̼��ʱ������
#if defined(PRODUCT_CFG_SYS_RST_SOLUTION)
        HI_MDM_RstDelayEnable(HI_SYS_RST_DELAY_CON_WR_MR_DOC,HI_SYS_RST_DELAY_CON_WR_MR_DOC_TO);
#endif
        mrsSrvArchivesSave(); //���ڵ���Ϣд��FLASH
#if defined(PRODUCT_CFG_SYS_RST_SOLUTION)
        HI_MDM_RstDelayDisable(HI_SYS_RST_DELAY_CON_WR_MR_DOC);
#endif
        mrsCcoDetectMeterChgToMacProc();//�������mrsSyncMeterInfToWhiteList֮ǰ,��Ϊ̽�Ⳮ������ձ���ʱ,�ڴ˲Ż����̽�������
        mrsSyncMeterInfToWhiteList(HI_MAC_WHIST_LIST_CHANGE_REASON_APP_CON);
        mrsCcoSmRestoreWhitelistSwitch();
        mrsSetPowerAfe();

        mrsSetArchivesStatus(MRS_CCO_ARCHIVES_STATUS_EFFECT);
        break;

    case EN_MRS_SRV_CCO_GET_TIME_TIMER:     // ��ʱ��ȡ������ʱ��
        mrsGetTimeTimerProcess(pstCcoCtx);  // ��ʱ��ȡ������ʱ�Ӵ���
        break;

    case EN_MRS_SRV_CCO_SYSTEM_RESET_TIMER:
        HI_MDM_Reboot(HI_SYS_REBOOT_CAUSE_MRS0);  //���Ӳ����ʼ������
        break;

    case EN_MRS_SRV_CCO_RM_CHECK_TIMER:
        mrsRmCheckTimerProc();
        break;

    case EN_MRS_SRV_CCO_BROADCAST_TIMER:
        // ֻת�����ģ����Ա��Ľ���У��
        mrsSendBroadcastFramePlc();
        break;

    case EN_MRS_SRV_CCO_NODE_STATE_CHG_TIMER:
        mrsChgFrameJoinCcoQueue();
        break;

    case MRS_CCO_TIMER_NODE_STATECHG_ENTRY_CACHE:
        mrsChgCacheTimeoutProc();
        break;

    case EN_MRS_SRV_CCO_EVENT_ACTIVE_TIMER:
        mrsCcoEvtReportMsgProc();
        break;

    case EN_MRS_SRV_CCO_RESET_PIN_TIMER:
        mrsResetTimeOutProc();
        break;

    case EN_MRS_SRV_CCO_SIMU_CHL_NOTIFY_MONITOR:
        {
            HI_SAL_DFX_HOST_CTRL_S * ctrl = HI_MDM_DfxGetHostCtrl();
            HI_DMS_CHL_PORT_OBJ_S stChlPort = mrsGetChlPort();

            if (ctrl == HI_NULL)
            {
                break;
            }

            if (0 != ctrl->ucMrRedirect2HsoPort)
            {
                mrsFwChlInit(&stChlPort, HI_DMS_CHL_UART_PORT_APP);
                (hi_void)memset_s(ctrl, sizeof(HI_SAL_DFX_HOST_CTRL_S), 0, sizeof(HI_SAL_DFX_HOST_CTRL_S));
                mrsDfxVmChlStatus(ctrl->ucMrRedirect2HsoPort);

                mrsRmCfgSetSimuChlStatus((HI_BOOL)ctrl->ucMrRedirect2HsoPort);
                mrsDetectRxModeNotify(DIAG_CMD_MRS_MODE_NORMAL);
            }
        }
        break;

    case MRS_CCO_TIMER_SEARCHMETER_SEND_FRAME:
        ret = mrsCcoSendSearchMeterPlcFrame(pstCcoCtx);
        break;

    case MRS_CCO_TIMER_GET_METERLIST:
        ret = mrsCcoSearchMeterGetMeterlist(pstCcoCtx);
        break;

    case MRS_CCO_TIMER_TOPO_QUERY:
        ret = mrsCcoSearchMeterTopoQueryProc(pstCcoCtx);
        break;

    case MRS_CCO_TIMER_SEND_BIND_CMD:
        ret = mrsCcoSearchMeterSendBindCmd(pstCcoCtx);
        break;

    case EN_MRS_SRV_CCO_CHECK_MAC_MAP_TTL_TIMER:
        mrsMapTtlTimeoutFunc(&pstCcoCtx->stMapModule);
        break;

    case EN_MRS_SRV_CCO_MAP_REFRESH_TIMER:
        if(pstCcoCtx->stMapModule.curr_turn < pstCcoCtx->stMapModule.max_turn_num)
        {
            ret = mrsMapMaintainProcess(MRS_MAP_MAINTAIN_MODE_REFRESH);
            pstCcoCtx->stMapModule.curr_turn++;
        }
        else
        {
            pstCcoCtx->stMapModule.curr_turn = 0;
        }
        break;

    case MRS_CCO_TIMER_SEARCH_METER_END:
        mrsSearchMeterEndTimerProcess(pstCcoCtx);
        break;

    case MRS_CCO_TIMER_SEARCH_METER_END_TF:
        mrsCcoStopSearchMeterTransformer();
        break;

#if defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG)
    case MRS_CCO_TIMER_ACQUIRE_CLOCK:
        mrsCcoRequestClockTimeout();
        break;

    case MRS_CCO_TIMER_MRS_LOG:
        mrsLogTimerProc();
        break;
#endif

    case MRS_CCO_TIMER_UPG_STOP_RETRY_ACTIVE:
        mrsUpgStopTimeOutPro();
        break;

    case MRS_CCO_TIMER_UPG_STATUS_QUERY_ACTIVE:
        mrsUpgStatusTimeOutPro();
        break;

    case MRS_CCO_TIMER_UPG_FILE_RECEIVE:
        mrsUpgRxFileTimeOutPro();
        break;

    case MRS_CCO_TIMER_UPG_STATUS_QUERY_TIMEOUT:
        mrsPowerOnStatusTimeOut();
        break;

    case MRS_CCO_TIMER_UPG_WAIT_PLC_AVAILABLE:
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
        mrsCsgUpgFailedProc();
#endif
        mrsUpgStatusEndPro(MRS_DFX_CCO_UPG_REASON_PLC_UNAVAILABLE);
        break;

    case MRS_CCO_TIMER_MASTER_ACTIVE:
        mrsCcoToMasterMsgPro();
        break;

    case MRS_CCO_TIMER_MASTER_REPORT:
        mrsCcoQueueTimeOutPro();
        break;

    case MRS_CCO_TIMER_SEARCH_METER_WHITELIST:
        mrsCcoSmRestoreWhitelistSwitch();
        break;

    case MRS_CCO_TIMER_TEST_MODE_FRAME:
        mrsCcoTestModeFrameTimeout();
        break;

    case MRS_CCO_TIMER_ZERO_CROSS:
        mrsZeroCrossCheckProc(HI_ERR_TIMEOUT, HI_NULL);
        break;

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO_BROAD_BAND_UNIT)
    case MRS_CCO_TIMER_RUN_LED_BBU:
        mrsRunLedProBbu();
        break;
#endif

    case MRS_CCO_TIMER_DETECT_REJECT_CLT_CACHE:
        mrsDetectWhiteListAddTimeout();
        break;

    case MRS_CCO_TIMER_DETECT_REFRESH_WHITE:
        mrsDetectWhiteListCheckProc();
        break;

    case MRS_CCO_TIMER_DETECT_WAIT_TOPO:
        mrsDetectWaitTopoTimeOut();
        break;

    case MRS_CCO_TIMER_DETECT_WHITE_LIST_KEEP:
        mrsDetectWhiteListKeepTimeOut();
        break;

    case MRS_CCO_TIMER_DETECT_JOIN_NET_CACHE:
        mrsDetectJoinNetTimeout();
        break;

    case MRS_CCO_TIMER_DETECT_BLACK_LIST:
        mrsDetectBlackListTimeout();
        break;

    case MRS_CCO_TIMER_RESUME_DETECT_RM:
        mrsResumeDetectRmProc();
        break;

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
    case MRS_CCO_TIMER_SEND_METER_LIST_PLC:
        mrsCcoDetectRmStart();
        break;

    case MRS_CCO_TIMER_EVT_FLAG_NOTICE:
        mrsCcoEvtNoticeSendFrame();
        break;

    case MRS_CCO_TIMER_UPG_QUERY_INFO_TIMEOUT:
        mrsUpgInfoTimeOutPro();
        break;

    case MRS_CCO_TIMER_MR_TASK_ACTIVE:
        mrsCcoMrTaskActive();
        break;

    case MRS_CCO_TIMER_MODE_PHYSICAL_TEST:
        mrsPhyTestTimeoutProc();
        break;

    case MRS_CCO_TIMER_ENTER_TESTMODE:
        mrsEnterTestModeProc();
        break;

    case MRS_CCO_TIMER_RECOVER_POWER_AFE:
        mrsRecoverPowerAfe();
        break;
#endif

    //����ͣ���ϱ��¼�
    case MRS_CCO_TIMER_POWERFAILURE:
        mrsCcoRspPowerFailureEvt();
        break;
    //����cco�ϱ�����
    case MRS_CCO_TIMER_POWERFAILURE_ERASE_DATA:
        mrsEraseBitmap();
        break;
    default:
        break;
    }

    return ret;
}

// CCO������״̬��(������ӳ���״̬)�ָ�Ϊ��ͨ״̬
HI_VOID mrsRecoverNormalState(MRS_CCO_SRV_CTX_STRU* pstCcoCtx)
{
    if (!pstCcoCtx)
    {
        return ;
    }

    if (pstCcoCtx->status & MRS_CCO_STATUS_ANYTIME_READMETER)
    {
        mrsAnyTimeReadMeterInit();
    }

    if (pstCcoCtx->status & MRS_CCO_STATUS_SEARCH_METER_TRANSFORMER)
    {
        mrsCcoSetState(MRS_CCO_STATUS_NORMAL | MRS_CCO_STATUS_SEARCH_METER_TRANSFORMER);
    }
    else
    {
        mrsCcoSetState(MRS_CCO_STATUS_NORMAL);
    }

#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
    mrsReadMeterStateInit();    // ��ʼ������״̬
#endif
}


//��ȡ��ǰ��ά����MAC
HI_U32 mrsMapGetCurrentMac(MRS_MAP_MODULE *module,HI_U8 mac[])
{
    if ((module != HI_NULL) && (mac != HI_NULL))
    {
        if (module->stNewMacList.number > 1)
        {
            (hi_void)memcpy_s(&module->stNewMacList.mac[0],HI_PLC_MAC_ADDR_LEN, mac, HI_PLC_MAC_ADDR_LEN);
            return HI_ERR_SUCCESS;
        }
    }
    return HI_ERR_FAILURE;
}


HI_VOID mrsGetCcoMrParam(NV_APP_MR_CFG_STRU* cfg)
{
    MRS_CCO_SRV_CTX_STRU *pstCtx = mrsCcoGetContext();

    // �㲥�����������
    pstCtx->stBroadcast.stParam.ucRetryMax  = cfg->ucBcmRetryMax;
    pstCtx->stBroadcast.stParam.ucInterval  = cfg->ucBcmInterval;

    pstCtx->ucFreqEnable = cfg->ucCcoFreqEnable;

    mrsGetFwCtx()->ulSimuChlTimeout = cfg->ulSimuChlTimeout;

    // ��Լ�汾
    switch (cfg->ucProtoVer)
    {
    case PROTO_VER_3762_2009:
        pstCtx->ucProtoVer = cfg->ucProtoVer;
        break;

    case PROTO_VER_13762_2013:
    default:
        pstCtx->ucProtoVer = PROTO_VER_13762_2013;
        break;
    }

    if (PROTO_VER_3762_2009 != pstCtx->ucProtoVer)
    {
        pstCtx->ucSeqControl = MRS_CCO_SEQ_START_FROM_1;
        pstCtx->ucLrModeFlag = MRS_CCO_LR_MODE_SINGLE;
    }

    pstCtx->stAutoSearchModel.ucSendTimeCount = cfg->ucSendTimeCount;//�ѱ��з���ѯ��ʱ���ܴ����ļ���

    pstCtx->plc_rxtx_mode = cfg->plc_rxtx_mode;
    pstCtx->stAutoSearchModel.ucWhiteListDelayTime = cfg->ucWhitelistDelay;

    if(cfg->ucSoftTfMode < SOFT_TF_MODE_INVALID)
    {
        pstCtx->ucSoftTfMode = cfg->ucSoftTfMode;
        HI_MDM_SetSoftTfMode((HI_MDM_SOFT_TF_MODE_EN)pstCtx->ucSoftTfMode);
    }

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) && defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG)
    mrsLogSetLowRatioThreshold(cfg->usLowRatio);
#endif

    mrsCcoSetTestModeCtrlInfo(((cfg->ucTestModeEnable) ? HI_TRUE : HI_FALSE), cfg->ucTestFrameTimeout);

    pstCtx->ucFlashSaveDelay = cfg->ucFlashSaveDelay;

    pstCtx->stReadMeterModel.ucAfn06F2UpRetryMax = cfg->ucAfn06F2UpRetryMax;
    pstCtx->stReadMeterModel.usAfn06F2UpTimeOut = cfg->usAfn06F2UpTimeOut;
    pstCtx->stReadMeterModel.ucAfn14F1UpRetryMax = cfg->ucAfn14F1UpRetryMax;
    pstCtx->stReadMeterModel.usAfn14F1UpTimeOut = cfg->usAfn14F1UpTimeOut;

    pstCtx->bCsgParallel = cfg->ucTestModeSnId_ucCsgParallelEnable ? HI_TRUE : HI_FALSE;

    mrsFwEnableUartDbg(0);

    return;
}

HI_U8 mrsGetArchivesStatus(HI_VOID)
{
    return mrsCcoGetContext()->ucArchiveStatus;
}

HI_VOID mrsSetArchivesStatus(HI_U8 ucStatus)
{
    if ((ucStatus != mrsGetArchivesStatus()) && (HI_FALSE == mrsCcoMrTaskIsChlAvailable()) && mrsCcoMrTaskIsEnabled())
    {
        mrsCcoMrTaskNotify();
    }

    mrsCcoGetContext()->ucArchiveStatus = ucStatus;
}

//*****************************************************************************
// ��������: mrsGetPlcTimeOut
// ��������: ��ȡPLCͨ����ʱʱ��
// ����˵��: mac : Ŀ�ĵ�ַ
//           inf : PLC��ʱʱ����Ϣ�ṹ
// �� �� ֵ: ��ʱʱ��(����)
//
// ����Ҫ��:
// ���þ���:
// ��    ��: liuxipeng/lkf54842 [2012-09-24]
//*****************************************************************************
HI_PRV HI_U32 mrsGetPlcTimeOut(HI_U8 mac[6], HI_BOOL force_bc, HI_U32 *m_time, PLC_OVERTIME_INF_STRU *inf)
{
#define MRS_MAX_LAYER 8            // 8ΪMac֧�ֵ����㼶,���Mac���㼶һ��
#define MRS_DEFAULT_LAYER_BACST 3  // �㲥Ĭ�ϲ㼶�����ڼ���㲥��ʱ
    HI_U32 plc_time      = 0;
    HI_U32 plc_time_bc   = 0;
    HI_U32 meter_time    = MRS_RM_METER_OVERTIME_DEFAULT + MRS_RM_CLTI_DELAY_DEFAULT;
    HI_U32 total_timeout = MRS_RM_PLC_BC_OVERTIME_DEFAULT
                            + MRS_RM_METER_OVERTIME_DEFAULT
                            + MRS_RM_CLTI_DELAY_DEFAULT
                            + MRS_RM_PLC_REVISORY_DEFAULT;
    if(!inf)
    {
        return total_timeout;
    }

    do
    {
        plc_time_bc = (HI_U32)(inf->LayTimeOutNv * MRS_DEFAULT_LAYER_BACST + inf->RevisoryNv);

        if(mrsToolsBroadCastMac(mac))   // �㲥��ַ
        {
            // ��ȡ�㲥����ʱʱ���Ƿ��I�ɲ����ı�ʶ
            HI_BOOL bCltIDelay = mrsRmBcTimeoutCltIDelayFlag();

            meter_time = (HI_U32)(inf->MeterTimeOutNv
                                    + inf->IsCtrlCmdFlg * inf->CtrlCmdDelayNv
                                    + inf->CltIDelayNv * bCltIDelay); // �㲥��ַĬ�ϲ���I�ɼ�����ʱ


            total_timeout = plc_time_bc + meter_time;

            MRS_DATA_VALID_CHECK(total_timeout,
                                MRS_RM_PLC_BC_OVERTIME_MAX,
                                MRS_RM_PLC_BC_OVERTIME_DEFAULT,
                                MRS_RM_PLC_BC_OVERTIME_MIN);
            break;
        }
        else
        {
            HI_U32 ret = HI_ERR_SUCCESS;
            HI_MAC_NETWORK_TOPO_ENTRY_S stTopoEntry;
            HI_U8 layer = 0;

            (hi_void)memset_s(&stTopoEntry, sizeof(stTopoEntry), 0, sizeof(stTopoEntry));
            ret = HI_MDM_QueryMacAttr(mac, &stTopoEntry);
            if (ret != HI_ERR_SUCCESS)
            {
                HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_060, HI_DIAG_MT("get mac info fail"));
                break;
            }

            layer = stTopoEntry.level;
            if(layer == 0)
            {
                break;
            }

            if (layer > MRS_MAX_LAYER)
            {
                layer = MRS_MAX_LAYER;
            }

            HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_059, HI_DIAG_MT("Node Type"), stTopoEntry.product_type);

            plc_time = (HI_U32)(inf->LayTimeOutNv * layer + inf->RevisoryNv);
            meter_time = (HI_U32)(inf->MeterTimeOutNv
                                    + inf->IsCtrlCmdFlg * inf->CtrlCmdDelayNv
                                    + ((stTopoEntry.product_type == HI_ND_TYPE_CLTI)? inf->CltIDelayNv : 0));
            if(force_bc)
            {
                total_timeout = plc_time_bc + meter_time;
            }
            else
            {
                total_timeout = plc_time + meter_time;
            }
        }

    }while(0);

    inf->OverTime = (HI_S32)total_timeout;

    if(m_time != HI_NULL)
    {
        *m_time = meter_time;
    }

    return total_timeout;

#undef MRS_MAX_LAYER
#undef MRS_DEFAULT_LAYER_BACST
}

HI_U32 mrsGetPlcTimeOutAfn13(HI_U8 *mac, HI_BOOL force_bc, HI_U32 *m_time)
{
    PLC_OVERTIME_INF_STRU *pstAfn13 = mrsGetPlcOverTimeInfAfn13();
    HI_U32 timeout = 0;

    timeout = mrsGetPlcTimeOut(mac, force_bc, m_time, pstAfn13);

    HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_050, HI_DIAG_MT("XR_Timer : Total[%d]-Meter[%d]"),
                        timeout, (m_time? *m_time : 0xEEEEEEEE));

    return timeout;
}

HI_U32 mrsGetPlcTimeOutAfn14(HI_U8 *mac, HI_BOOL force_bc, HI_U32 *m_time)
{
    PLC_OVERTIME_INF_STRU *pstAfn14 = mrsGetPlcOverTimeInfAfn14();
    HI_U32 timeout = 0;

    timeout = mrsGetPlcTimeOut(mac, force_bc, m_time, pstAfn14);

    HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_051, HI_DIAG_MT("LR_Timer : Total[%d]-Meter[%d]"),
                        timeout, (m_time? *m_time : 0xEEEEEEEE));

    return timeout;
}


//*****************************************************************************
// ��������: mrsGetPlcMaxRetry
// ��������: ��ȡPLCͨ��������Դ���
// ����˵��: mac : Ŀ�ĵ�ַ
//
// �� �� ֵ: ���ʱ����
//
// ����Ҫ��:
// ���þ���:
// ��    ��: liuxipeng/lkf54842 [2012-09-24]
//*****************************************************************************
HI_PRV HI_U8 mrsGetPlcMaxRetry(HI_U8 *mac, PLC_OVERTIME_INF_STRU* inf)
{
    HI_U8 maxtry = MRS_RM_PLC_RETRY_CNT_DEFAULT;

    if(inf == HI_NULL)
    {
        return maxtry;
    }

    do
    {
        if(mac == HI_NULL)  // ǿ��ʹ�ù㲥��ʽ
        {
            maxtry = (HI_U8)inf->MaxTryNvB;
            break;
        }

        maxtry = (HI_U8)inf->MaxTryNvS;

    }while(0);

    inf->MaxTry = (HI_S8)maxtry;

    return maxtry;
}

HI_U8 mrsGetMaxTryAfn13(HI_U8 *mac)
{
    PLC_OVERTIME_INF_STRU *pstAfn13 = mrsGetPlcOverTimeInfAfn13();
    HI_U8 max_try = 0;

    max_try = mrsGetPlcMaxRetry(mac, pstAfn13);

    HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_052, HI_DIAG_MT("XR_TryNum : max[%d]"), max_try);

    return max_try;
}

HI_U8 mrsGetMaxTryAfn14(HI_U8 *mac)
{
    PLC_OVERTIME_INF_STRU *pstAfn14 = mrsGetPlcOverTimeInfAfn14();
    HI_U8 max_try = 0;

    max_try = mrsGetPlcMaxRetry(mac, pstAfn14);

    HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_053, HI_DIAG_MT("LR_TryNum : max[%d]"), max_try);

    return max_try;
}

//*****************************************************************************
// ��������: mrsInitPlcTimeOutMaxNum
// ��������: ��ȡ���ڵ������ʱ����NV���ʼ��
// ����˵��: ��
//
// �� �� ֵ: ����ֵ�б�
//
// ����Ҫ��:
// ���þ���:
// ��    ��: liuxipeng/lkf54842 [2012-09-24]
//*****************************************************************************
HI_U32 mrsInitPlcTimeOutMaxNum(HI_VOID)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    MV_APP_MAX_PLC_TIMEOUT_NUM stTimeOutNum = {0};
    MRS_CCO_SRV_CTX_STRU* pstCcoCtx = mrsCcoGetContext();  // ��ȡȫ�������Ľṹָ��

    ret = HI_MDM_NV_Read(ID_MV_APP_MAX_PLC_TIMEOUT_NUM,
                        &stTimeOutNum, sizeof(MV_APP_MAX_PLC_TIMEOUT_NUM));

    if((HI_ERR_SUCCESS != ret)
        || (stTimeOutNum.ulMaxNum < PRODUCT_CFG_MRS_PLC_TIMEOUT_NUM_MIN))
    {
        pstCcoCtx->stReadMeterModel.ulTimeOutMaxNumNv = PRODUCT_CFG_MRS_PLC_TIMEOUT_NUM_DEFAULT;
    }
    else
    {
        // ulTimeOutMaxNumNvֵΪȫFFʱ����NV��ܹرգ��ٴο�������Ҫ��յ�ǰ������
        if(pstCcoCtx->stReadMeterModel.ulTimeOutMaxNumNv == (HI_U32)-1)
        {
            pstCcoCtx->stReadMeterModel.ulTimeOutCount = 0;
        }

        pstCcoCtx->stReadMeterModel.ulTimeOutMaxNumNv = stTimeOutNum.ulMaxNum;
    }

    return ret;
}

// ��ȡ�ڵ�Plc��ʱ������(������ת�ڵ�)
HI_U32 mrsGetPlcTimeOutMaxNum(HI_VOID)
{
    MRS_CCO_SRV_CTX_STRU* pstCcoCtx = mrsCcoGetContext();  // ��ȡȫ�������Ľṹָ��

    return pstCcoCtx->stReadMeterModel.ulTimeOutMaxNumNv;
}

//*****************************************************************************
// ��������: mrsSyncMeterInfToWhiteList
// ��������: ͬ��������Ϣ��������
// ����˵��: ��
//
// �� �� ֵ: ����ֵ�б�
//
// ����Ҫ��:
// ���þ���:
// ��    ��: liuxipeng/lkf54842 [2012-11-10]
//*****************************************************************************
/* BEGIN: Modified by fengxiaomin/00209182, 2014/3/28   ���ⵥ��:DTS2014032708937 */
HI_U32 mrsSyncMeterInfToWhiteList(HI_MAC_WHIST_LIST_CHANGE_REASON_ENUM enReason)
/* END:   Modified by fengxiaomin/00209182, 2014/3/28 */
{
    HI_MAC_VALID_MAC_LIST_STRU * pstMeterList = HI_NULL;
    HI_U16 usArchivesMeterNum = mrsSrvArchivesMeterNum();
    HI_U16 usDetectNum = mrsGetDetectWhiteListValidNum();
    HI_U16 usMeterNum = MRS_MIN(usArchivesMeterNum + usDetectNum, HI_MAC_VALID_MAC_LIST_MAX);

    pstMeterList = (HI_MAC_VALID_MAC_LIST_STRU *) mrsToolsMalloc(
                                    sizeof(HI_MAC_VALID_MAC_LIST_STRU));
    if(pstMeterList == HI_NULL)
    {
        return HI_ERR_NO_MORE_MEMORY;
    }

    (hi_void)memset_s(pstMeterList, sizeof(HI_MAC_VALID_MAC_LIST_STRU), 0, sizeof(HI_MAC_VALID_MAC_LIST_STRU));

    HI_DIAG_LOG_MSG_E3(MRS_FILE_LOG_FLAG_045, HI_DIAG_MT("sync white list"), usMeterNum, usDetectNum, usArchivesMeterNum);
    mrsCcoArchivesSetMacList(MRS_MIN(usArchivesMeterNum, usMeterNum), pstMeterList);

    if (usMeterNum > usArchivesMeterNum)
    {
        mrsCcoDetectWhiteSetMacList(usMeterNum - usArchivesMeterNum, pstMeterList);
    }

    HI_MDM_SetValidMacList(pstMeterList, enReason);

    mrsToolsFree(pstMeterList);

    return HI_ERR_SUCCESS;
}

HI_U32 mrsCcoArchivesSetMacList(HI_U16 usMeterNum, HI_MAC_VALID_MAC_LIST_STRU * pstMeterList)
{
    HI_U16 usIndex = 0;

    HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_066, HI_DIAG_MT("sync meter white list"), usMeterNum);

    for(usIndex = 0; usIndex < usMeterNum; usIndex++)
    {
        MRS_ARCHIVES_NODE_STRU * pstNode = HI_NULL;

        pstNode = mrsSrvArchivesQuery(usIndex);
        if (pstNode != HI_NULL)
        {
            mrsToolsConvertMeter2Mac(pstNode->aucNodeAddr,
                                    pstNode->ucProtocol,
                                    pstMeterList->mac_list[usIndex].mac);
            pstMeterList->list_num++;
        }
    }

    return HI_ERR_SUCCESS;
}


// ��ȡ��������ִ�����
HI_U32 mrsGetMaxTurnNum(HI_VOID)
{
    MRS_CCO_SRV_CTX_STRU* pstCcoCtx = mrsCcoGetContext();  // ��ȡȫ�������Ľṹָ��
    return pstCcoCtx->stReadMeterModel.ulTurnNumNv;
}


// ���ö�������ִ�����
HI_VOID mrsSetMaxTurnNum(HI_U32 x)
{
    MRS_CCO_SRV_CTX_STRU* pstCcoCtx = mrsCcoGetContext();  // ��ȡȫ�������Ľṹָ��
    pstCcoCtx->stReadMeterModel.ulTurnNumNv = x;
}


// ��ȡ��ǰ�����ִ���
HI_U32 mrsGetCurrTurnNum(HI_VOID)
{
    MRS_CCO_SRV_CTX_STRU* pstCcoCtx = mrsCcoGetContext();  // ��ȡȫ�������Ľṹָ��
    return pstCcoCtx->stReadMeterModel.ulCurrTurn;
}


// ���õ�ǰ�����ִ���
HI_VOID mrsSetCurrTurnNum(HI_U32 x)
{
    MRS_CCO_SRV_CTX_STRU* pstCcoCtx = mrsCcoGetContext();  // ��ȡȫ�������Ľṹָ��
    pstCcoCtx->stReadMeterModel.ulCurrTurn = x;
}


// ����ǰ�ִ�����
HI_VOID mrsCurrTurnGrow(HI_VOID)
{
    MRS_CCO_SRV_CTX_STRU* pstCcoCtx = mrsCcoGetContext();  // ��ȡȫ�������Ľṹָ��
    pstCcoCtx->stReadMeterModel.ulCurrTurn++;
}


// ǿ���л�Ϊ·����ͣ״̬
HI_VOID mrsForceSwitchPause(HI_VOID)
{
#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
    MRS_CCO_SRV_CTX_STRU* pstCcoCtx = mrsCcoGetContext();  // ��ȡȫ�������Ľṹָ��

    if (HI_TRUE == pstCcoCtx->bPauseFlag)
    {
        return;
    }

    pstCcoCtx->bPauseFlag = HI_TRUE;
    mrsDfxLrRoute((HI_U8)pstCcoCtx->bPauseFlag);

    if (pstCcoCtx->status & MRS_CCO_STATUS_NORMAL)
    {
        HI_U32 status = mrsCcoGetState();

        if(pstCcoCtx->stReadMeterModel.enCurrState != EN_MRS_SRV_CCO_READMETER_IDLE)
        {
            // �����ǰ���ǿ���״̬���������״̬(��ǰ�ڵ㲻�仯)
            mrsReadMeterStateInit();  // ����ģ��״̬��ʼ��
        }

        status &= ~MRS_CCO_STATUS_NORMAL;
        mrsCcoSetState(status);
    }
#endif

    mrsResumeDetectRmSendMsg();
}


HI_BOOL mrsGetCcoPlcChlStatus(HI_VOID)
{
    MRS_CCO_SRV_CTX_STRU *pstCcoCtx = mrsCcoGetContext();
    HI_PRV HI_U8 ucLastPlcRxTxMode = MRS_PLC_RX_TX_MODE_NORMAL;

    if (MRS_PLC_RX_TX_MODE_FORCE == pstCcoCtx->plc_rxtx_mode)
    {
        if (MRS_PLC_RX_TX_MODE_NORMAL == ucLastPlcRxTxMode)
        {
            pstCcoCtx->PlcChStatus = HI_TRUE;

            if (mrsCcoGetState() & MRS_CCO_STATUS_CREATE_MAP)
            {
                mrsCcoMapToNormalPro();
            }
        }

        ucLastPlcRxTxMode = pstCcoCtx->plc_rxtx_mode;

        return HI_TRUE;
    }

    ucLastPlcRxTxMode = pstCcoCtx->plc_rxtx_mode;

    return pstCcoCtx->PlcChStatus;
}


// ��ȡCCO����״̬(HI_TRUE : �Ѿ������Զ���; HI_FALSE : δ����)
//*****************************************************************************
// ��������: mrsGetCcoReadyStatus
// ��������: ��ȡCCO��PLCҵ���Ƿ����״̬
// ����˵��:
// ucFlag  : ��ǰҵ����Զ���������⣬ֵΪHI_TRUE, ��ֵ֮ΪHI_FALSE;
//
// �� �� ֵ: ����ֵ�б�
//
// ����Ҫ��:
// ���þ���:
// ��    ��: liuxipeng/lkf54842 [2012-11-10]
// ��    ��: fengxiaomin/f00209182 [2014-08-14]
//*****************************************************************************
HI_BOOL mrsGetCcoReadyStatus(HI_U8 ucFlag)
{
    if (HI_TRUE != mrsGetCcoPlcChlStatus())
    {
        return HI_FALSE;
    }

    if (ucFlag)
    {
        if (mrsIsCcoUpgStatus())
        {
            return HI_FALSE;
        }
    }

    return HI_TRUE;
}


// �� �� ֵ: PLC�������ݰ�ͳ����
//
// ����Ҫ��:
// ���þ���:
// ��    ��: fengxiaomin/00209182 [2013-03-02]
//*****************************************************************************
#if defined(PRODUCT_CFG_SYS_RST_SOLUTION)
HI_U32 mrsCcoRMPlcStat(HI_VOID)
{
    MRS_CCO_SRV_CTX_STRU * cco = mrsCcoGetContext();
    return cco->stResetStatInfo.ulRMStatTimes;
}
#endif


//*****************************************************************************
// ��������: mrsCcoRxUartStat
// ��������: CCO���ռ��������ݰ�ͳ��
// ����˵��: ��
//
// �� �� ֵ: CCO���ռ���������ͳ����
//
// ����Ҫ��:
// ���þ���:
// ��    ��: fengxiaomin/00209182 [2013-03-02]
//*****************************************************************************
#if defined(PRODUCT_CFG_SYS_RST_SOLUTION)
HI_U32 mrsCcoRxUartStat(HI_VOID)
{
    MRS_CCO_SRV_CTX_STRU * cco = mrsCcoGetContext();
    return cco->stResetStatInfo.ulCcoRxUartTimes;
}
#endif


HI_U32 mrsCcoReadFailInsert(HI_U8 MeterAddr[HI_METER_ADDR_LEN], HI_U8 protocol)
{
    HI_U16 i;
    MRS_CCO_SRV_CTX_STRU * cco = mrsCcoGetContext();
    HI_U8 aucMeterAddr[HI_METER_ADDR_LEN] = {0};
    HI_U8 aucTempAddr[HI_METER_ADDR_LEN] = {0};

    mrsMeterAddrPreProc(aucMeterAddr, MeterAddr, protocol);

    for (i = 0; i < cco->stReadFail.usReadFailNum; i++)
    {
        mrsMeterAddrPreProc(aucTempAddr, cco->stReadFail.pReportedNodeInf[i].ucMeterAdd, cco->stReadFail.pReportedNodeInf[i].protocol);
        if (0 == memcmp(aucTempAddr, aucMeterAddr, HI_METER_ADDR_LEN))
        {
            return HI_ERR_SUCCESS;
        }
    }

    if (cco->stReadFail.usReadFailNum < PRODUCT_CFG_MRS_MAX_NODE_NUM)
    {
        (hi_void)memcpy_s(cco->stReadFail.pReportedNodeInf[i].ucMeterAdd, HI_METER_ADDR_LEN, MeterAddr, HI_METER_ADDR_LEN);
        cco->stReadFail.pReportedNodeInf[i].protocol = protocol;
        cco->stReadFail.usReadFailNum++;
        return HI_ERR_SUCCESS;
    }

    return HI_ERR_FULL;
}


HI_U32 mrsCcoReadFailRemove(HI_U8 MeterAddr[HI_METER_ADDR_LEN], HI_U8 protocol)
{
    HI_U16 i;
    MRS_CCO_SRV_CTX_STRU * cco = mrsCcoGetContext();
    HI_U8 aucMeterAddr[HI_METER_ADDR_LEN] = {0};
    HI_U8 aucTempAddr[HI_METER_ADDR_LEN] = {0};

    mrsMeterAddrPreProc(aucMeterAddr, MeterAddr, protocol);

    for (i = 0; i < cco->stReadFail.usReadFailNum; i++)
    {
        mrsMeterAddrPreProc(aucTempAddr, cco->stReadFail.pReportedNodeInf[i].ucMeterAdd, cco->stReadFail.pReportedNodeInf[i].protocol);
        if (0 == memcmp(aucTempAddr, aucMeterAddr, HI_METER_ADDR_LEN))
        {
            if (i != cco->stReadFail.usReadFailNum - 1)
            {
                (hi_void)memcpy_s(&cco->stReadFail.pReportedNodeInf[i], sizeof(MR_NODE_METER_ADD_STRU),
                    &cco->stReadFail.pReportedNodeInf[cco->stReadFail.usReadFailNum - 1], sizeof(MR_NODE_METER_ADD_STRU));
            }

            (hi_void)memset_s(&cco->stReadFail.pReportedNodeInf[cco->stReadFail.usReadFailNum - 1],
								sizeof(MR_NODE_METER_ADD_STRU),0,
								sizeof(MR_NODE_METER_ADD_STRU));
            cco->stReadFail.usReadFailNum--;

            break;
        }
    }

    return HI_ERR_SUCCESS;
}


HI_VOID mrsCcoReadFailReset(HI_VOID)
{
    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();

    pstCcoCtx->stReadFail.usReadFailNum = 0;
    (hi_void)memset_s(pstCcoCtx->stReadFail.pReportedNodeInf,
						sizeof(MR_NODE_METER_ADD_STRU) * PRODUCT_CFG_MRS_MAX_NODE_NUM,0,
						sizeof(MR_NODE_METER_ADD_STRU) * PRODUCT_CFG_MRS_MAX_NODE_NUM);
}


// ��ȡPLC�շ�ģʽ
HI_U8 mrsCcoGetPlcRxTxMode(HI_VOID)
{
    MRS_CCO_SRV_CTX_STRU * cco = mrsCcoGetContext();
    return cco->plc_rxtx_mode;
}


/* BEGIN: Added by fengxiaomin/00209182, 2013/12/11   ���ⵥ��:��:DTS2013122609670 */
//*****************************************************************************
// ��������: mrsGetWorkFlag
// ��������: ��ѯ����״̬
// ����˵��:
//
// �� �� ֵ:
//   ����״̬
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: fengxiaomin/f00209182 [2012-12-11]
//           fengxiaomin/f00209182 [2012-12-28]
//*****************************************************************************
HI_U8 mrsGetCcoWorkFlag(HI_VOID)
{
    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();
    HI_U8 ucCcoStatus = MRS_CCO_NON_WORKING_STATUS;

    if (mrsIsCcoUpgStatus())
    {
        ucCcoStatus = MRS_CCO_WORK_SWITCH_REMOTE_UPGRADE;
    }
#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
    else if ((pstCcoCtx->status & MRS_CCO_STATUS_NORMAL) && (mrsCcoIsTestMode() || pstCcoCtx->PlcChStatus))
#else
    else if ((pstCcoCtx->status & MRS_CCO_STATUS_NORMAL) && (pstCcoCtx->PlcChStatus))
#endif
    {
        ucCcoStatus = MRS_CCO_WORK_SWITCH_LOCAL_READ_METER;
    }
    else if (pstCcoCtx->status & MRS_CCO_STATUS_SEARCH_METER)
    {
        ucCcoStatus = MRS_CCO_WORK_SWITCH_SEARCH_METER;
    }
    else if (pstCcoCtx->status & MRS_CCO_STATUS_SEARCH_METER_TRANSFORMER)
    {
        ucCcoStatus = MRS_CCO_WORK_SWITCH_OTHER;
    }

    return ucCcoStatus;
}

/* BEGIN: Added by fengxiaomin/00209182, 2014/1/13   ���ⵥ�ź�:DTS2014012607684 */
//*****************************************************************************
// ��������: mrsRefreshTopoInfo
// ��������: ˢ��������Ϣ
//
// ����˵��:
//    pstCcoCtx cco������
//
// �� �� ֵ:
//    �汾��
//
// ����Ҫ��:
// ���þ���:
// ��    ��: fengxiaomin/00209182 [2014-1-13]
//*****************************************************************************
HI_U32 mrsRefreshTopoInfo(MRS_CCO_SRV_CTX_STRU *pstCcoCtx)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_MAC_CONFIG_INFO_S mac_inf = {0};

    mac_inf.pTopo = &pstCcoCtx->stTopoInf;
    ret = HI_MDM_QueryInfo(&mac_inf);  // ��ȡmac���˱���Ϣ

    return ret;
}
/* END:   Added by fengxiaomin/00209182, 2014/1/13 */

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO_BROAD_BAND_UNIT)
HI_VOID mrsRunLedProBbu(HI_VOID)
{
}
#endif

HI_U32 mrsDiagCcoModeNotify(HI_SYS_QUEUE_MSG_S* pstMsg)
{
    DIAG_CMD_MRS_HSO_SET_MODE_IND_S stInd;
    DIAG_CMD_MRS_HSO_SET_MODE_REQ_S *pstReq = (DIAG_CMD_MRS_HSO_SET_MODE_REQ_S*)pstMsg->ulParam[1];

    stInd.ulRet = mrsDetectRxModeNotify(pstReq->ucMode);

    return HI_MDM_ReportPacket(pstMsg->ulParam[0], (HI_U8)pstMsg->ulParam[3], (HI_PBYTE)&stInd, sizeof(DIAG_CMD_MRS_HSO_SET_MODE_IND_S), HI_FALSE);
}


#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
HI_U32 mrsCmdCcoCsgDataTransmit(HI_U16 usId, HI_PVOID pReqPacket, HI_U16 usPacketSize)
{
    HI_UNREF_PARAM(usId);
    return mrsCsgPlcFrameProc(pReqPacket, usPacketSize);
}
#endif


#endif


