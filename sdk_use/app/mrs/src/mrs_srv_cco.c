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
#include "mrs_srv_capture.h"
#include "mrs_fw_proto698_45.h"
#include "mrs_srv_cltii_power_on_off.h"
#include "mrs_transfomer_identify.h"
#include "mrs_prepare_test_mode.h"
#include "mrs_fw_proto188.h"
#include "mrs_srv_rf.h"
#include "mrs_srv_dau.h"
#include "mrs_srv_rf.h"

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
HI_U32 mrsCmdCcoPlcTestTransmit(HI_IN HI_U16 id, HI_PVOID req_packet, HI_U16 packet_size);
HI_U32 mrsCmdCcoGetDeviceInf(HI_U16 id, HI_PVOID req_packet, HI_U16 req_packet_size);
HI_U32 mrsMsgCcoDeviceDataRx(HI_U16 usMsgId, HI_IN HI_PVOID pMsgParam);
HI_VOID mrsStaCommTimerProcess(MRS_CCO_SRV_CTX_STRU* pstCcoCtx);
HI_VOID mrsSearchMeterEndTimerProcess(MRS_CCO_SRV_CTX_STRU* pstCcoCtx);
HI_VOID mrsGetTimeTimerProcess( MRS_CCO_SRV_CTX_STRU* pstCcoCtx);
HI_U32 mrsMsgCcoTimer(HI_U16 usMsgId, HI_IN HI_PVOID pMsgParam);
HI_U32 mrsCcoArchivesSetMacList(HI_U16 usMeterNum, HI_MAC_VALID_MAC_LIST_STRU * pstMeterList);

// �������Ӵ��뺯��
HI_U32 mrsCmdCCORecvGetMeterDataInfo(HI_IN HI_U16 id, HI_PVOID req_packet, HI_U16 packet_size);
HI_U32 mrsSearchWaterMeterInit(HI_VOID);
HI_U32 mrsCcoSendStartFindOwner(HI_VOID);
HI_U32 mrsCcoSearchWaterMeterSetShip(MRS_CCO_SRV_CTX_STRU *cco,HI_U8 ucFlag);
HI_U32 mrsCcoCalcWaterMeterShip(MRS_CCO_SRV_CTX_STRU *cco);
HI_U32 mrsCcoCreateSetWaterMeterShip(MRS_CCO_SRV_CTX_STRU  * cco);
HI_U32 mrsCmdCCORecvWaterMeterSetShip(HI_PVOID pPayload, HI_U16 usPayloadLen);

//��ѯ�豸���� Matteo
HI_U32 mrsCcoSendStartQueryDeviceType(MRS_CCO_SRV_CTX_STRU  * cco);
HI_U32 mrsCcoFindTypeTimeoutHandle( MRS_CCO_SRV_CTX_STRU  * cco);
HI_U32 mrsCcoSendQueryDeviceTypeCmd(MRS_CCO_SRV_CTX_STRU  * cco,HI_U8 *pMeterAddr);



// ��ȡPLC�շ�ģʽ
HI_U8 mrsCcoGetPlcRxTxMode(HI_VOID);

hi_u32 mrs_cco_query_sta_dev_id_ind(MRS_PLC_FRAME_DATA_STRU *plc_data);
hi_u32 mrs_ndm_query_cco_dev_id_req(MRS_PLC_FRAME_DATA_STRU *plc_data);

hi_bool mrs_check_area_code(hi_u8 area_code)
{
    MRS_CCO_SRV_CTX_STRU * ctx = mrsCcoGetContext();

    if (ctx != HI_NULL) {
        if (ctx->area_code == area_code) {
            return HI_TRUE;
        }
    }

    return HI_FALSE;
}

// ��ȡCCO��ȫ�������ĵĽṹָ��
MRS_CCO_SRV_CTX_STRU* mrsCcoGetContext()
{
    return g_pstMrsCcoSrvCtx;
}
#if defined(MRS_SRV_PROTOCOL_CJT188)
PLC_OVERTIME_INF_STRU *mrsGetPlcOverTimeInfSavedAfn13(HI_VOID)
{
    return &g_pstMrsCcoSrvCtx->stAnytimeRMModel.stOverTimeInfSaved;
}
#endif

// ��ȡPLC��ʱ��Ϣ�ṹAFN-13
PLC_OVERTIME_INF_STRU *mrsGetPlcOverTimeInfAfn13(HI_VOID)
{
    return &mrsCcoGetContext()->stAnytimeRMModel.stOverTimeInf;
}

// ��ȡPLC��ʱ��Ϣ�ṹAFN-14
PLC_OVERTIME_INF_STRU *mrsGetPlcOverTimeInfAfn14(HI_VOID)
{
    if (mrsCcoIsTestMode())
    {
        return mrsGetPlcOverTimeInfAfn13();
    }
    else
    {
        return &mrsCcoGetContext()->stReadMeterModel.stOverTimeInf;
    }
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
        mrsSrvInitList(&(pstCcoCtx->stAutoSearchModel.stUnLockStaList));
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
    MRS_CCO_INIT_CTX_STRU_MEM(pstCcoCtx->pstStaIdInfo, sizeof(HI_MDM_NETWORK_STA_ID_S), HI_MDM_NETWORK_STA_ID_S);
	//Matteo
    MRS_CCO_INIT_CTX_STRU_MEM(pstCcoCtx->usIIArchive, sizeof(II_ARCHVICE), II_ARCHVICE);

    // ��NV����ز����ڵ��ַ
    MRS_CCO_INIT_CTX_FUNC(mrsCcoAddrInit(pstCcoCtx));
    HI_MDM_DevIdConfig(pstCcoCtx->ucMainNodeAddr);

    MRS_CCO_INIT_CTX_FUNC(mrsInitPlcTimeOutMaxNum());

    MRS_CCO_INIT_CTX_FUNC(mrsNvMaxTurnNumCfg(ID_NV_APP_MAX_TURN_NUM));

    MRS_CCO_INIT_CTX_FUNC(mrsInitAppMrNvInf());

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

    HI_MDM_RegisterReportPhaseListRefresh(mrsRefreshPhaseCallBack);

    mrsCcoEvtInit();
    mrsCcoQueueInit();
#if defined (PRODUCT_OPTIMIZE_MATTEO)
    mrsCcoNodeStateChgReportInit();
#endif
    mrsInitSustainAfn();

    //��ʼ��Զ������ģ��
    mrsCcoRemoteUpgInit();

    //��ʼ��ӳ���
    MRS_CCO_INIT_CTX_FUNC(mrsCcoMapInit(&pstCcoCtx->stMapModule));

    // ��ʼ������ģ��
    mrsRmModuleInit();

    //ץ��������ʼ��ע��
    #ifdef PRODUCT_CFG_TTCN
    mrsSrvCaptureInit();
    #endif

    MRS_CCO_INIT_CTX_FUNC(mrsRmCfgModuleInit());
    MRS_CCO_INIT_CTX_FUNC(mrsDfxMrRetryModuleInit());

    // ��ʼ����������
    mrsParallelReadMeterInit();

    // ����Ϊ��Ҫ��ȡ������ʱ��״̬
    mrsSetNeedGetTimeFlg(HI_FALSE);

    pstCcoCtx->bFirstRun = HI_TRUE;
    pstCcoCtx->stAnytimeRMModel.ulTimeOut = MRS_SRV_CCO_TIMER_DEFAULT_TIMEOUT_VALUE;  // ��ʼ���泭Ĭ�ϳ�ʱʱ��

    MRS_StartTimer(EN_MRS_SRV_CCO_GET_TIME_TIMER,
                        2000, HI_SYS_TIMER_ONESHOT); // ������ʱ��

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
    //mrsInitMrsLogModule();//optimized by weichao
#endif

    mrsCcoCheckMainAddrStart(pstCcoCtx);
    mrsPoweOffOnInit();
	mrsCcoReloadNvVerCfg();
#if defined(PRODUCT_CFG_SUPPORT_MRS_DUOL_MODE_RF)
	mrsRfCCOInit();
#endif

	mrsSearchWaterMeterInit();
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

    HI_MDM_SetMainNodeAddr(pstCcoCtx->ucMainNodeAddr);

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

    return;
}
/* END:   Added by fengxiaomin/00209182, 2014/1/14 */


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

	
//	HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_002, HI_DIAG_MT("[Matteo] TopoStatusChgProc"));
	mrsCcoSendStartQueryDeviceType(pstCcoCtx);//Matteo
	//Matteo
	if(HI_MDM_MAC_NETWORK_EVALUATE_END == mrsGetTopoStatus()&& HI_MDM_MAC_STA_JOIN_NET == pstMsg->ulParam[0])
	{
//		HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_055, HI_DIAG_MT("[Matteo] NETWORK_EVALUATE_END"));
		pstCcoCtx->stReadWaterMeterModel.usRfPlcSeq = HI_MDM_GetRandNum16(1,65535);
		mrsCcoSendStartFindOwner();
	}

    if ((HI_MDM_MAC_NETWORK_FORMED == pstMsg->ulParam[0]) || (HI_MDM_MAC_NETWORK_EVALUATE_END == pstMsg->ulParam[0]))
    {
    	
        mrsSetTopoStatus((HI_U8)pstMsg->ulParam[0]);
#if defined(PRODUCT_CFG_SUPPORT_MRS_DUOL_MODE_RF)
		HI_U32 ulCcoSnid = HI_MDM_GetWholeNetNid();
		AppCCOMDMQuerySn(HI_MDM_NM_SNID_QRY_ID);		
		mrs3762_RF_ReportAFN04_F5(ulCcoSnid);
#if defined(PRODUCT_CFG_SUPPORT_MRS_DUOL_MODE_RF_II)
		if(HI_MDM_MAC_NETWORK_EVALUATE_END == pstMsg->ulParam[0])
		{
      //CJJ-HPLC-20181023:ˮ�����0x42�㲥��ų�ʼ�����ֵ
      pstCcoCtx->stReadWaterMeterModel.usRfPlcSeq = HI_MDM_GetRandNum16(1,65535);
			//CJJ-HPLC-20181023
			mrsCcoSendStartFindOwner();
		}
#endif

#endif
    }

    mrsRefreshTopoInfo(pstCcoCtx);
    mrsCcoDetectTopoChgProc((HI_U8)pstMsg->ulParam[0]);
#if defined(PRODUCT_CFG_SUPPORT_MRS_DUOL_MODE_RF_II)
	if(HI_MDM_MAC_NETWORK_FORMED == mrsGetTopoStatus())
	{
		if(pstCcoCtx->ucRfSGDTimeSlot>0)
		{
			mrsCcoSGDTimeSlotFun();
		}
	}
#endif

    mrsTfCcoRefreshQueryList();
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

    HI_UNREF_PARAM(usMsgId);

    if(pstChlStatus == HI_NULL)
    {
        return HI_ERR_FAILURE;
    }

    if (!mrsCcoIsTestMode() && (pstCcoCtx->status & MRS_CCO_STATUS_NORMAL))
    {
        if ((pstCcoCtx->PlcChStatus == HI_FALSE) && (pstChlStatus->bAvail == HI_TRUE))
        {
            mrsReadMeterStateInit();    // ���¿�ʼ��������

            // ��ʱMRS_SRV_CCO_TIMER_READMETER_ACTIV_VALUEʱ������¼������ģ��
            MRS_StartTimer(EN_MRS_SRV_CCO_READMETER_ACTIV_TIMER, MRS_SRV_CCO_TIMER_READMETER_ACTIV_VALUE, HI_SYS_TIMER_ONESHOT);
        }
    }

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
            //mrsStartMrsLogTimer();//optimized by weichao
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
HI_U32 mrsCcoRxAfn13DataPro(HI_PVOID pPayload, HI_U16 usPayloadLen, HI_PVOID *ppData, HI_U16 *pusDataLen, HI_U8 ucMacAdd[HI_PLC_MAC_ADDR_LEN])
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

    if ((0 == *pusDataLen) || (usPayloadLen != pstTransmit->stru_size + *pusDataLen))
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

    return mrsAnyTimeReadMeterReceivePlcProcess(pstTransmit, ucMacAdd);
}


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
HI_U32 mrsCcoRxAfn14DataPro(HI_PVOID pPayload, HI_U16 usPayloadLen, HI_PVOID *ppData, HI_U16 *pusDataLen, HI_U8 ucMacAdd[HI_PLC_MAC_ADDR_LEN])
{
    MRS_DATA_TRANSMIT_AFN14_UP_STRU* pstTransmit = (MRS_DATA_TRANSMIT_AFN14_UP_STRU *)pPayload;
    HI_U16 usLocalLen = 0;

    if (pstTransmit->dir != MRS_PLC_UP_FLG)
    {
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_020, HI_DIAG_MT("afn 14 error plc dir."));
        return HI_ERR_BAD_FRAME;
    }

    *ppData = (HI_U8*)pPayload + pstTransmit->stru_size;
    *pusDataLen = pstTransmit->data_len;
    usLocalLen = sizeof(MRS_DATA_TRANSMIT_AFN14_UP_STRU) + pstTransmit->data_len;

    if ((0 == *pusDataLen) || (usPayloadLen != pstTransmit->stru_size + *pusDataLen))
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

    return mrsReadMeterReceivePlcProcess(pstTransmit, mrsCcoGetContext(), ucMacAdd);
}


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
    MRS_645_FRAME_STRU frame;
    HI_U8 * data_645 = HI_NULL;
    HI_U16 data_len = 0;
    HI_U8 * mac = HI_NULL;
    HI_U8 *pPayload = HI_NULL;
    HI_U16 usPayloadLen = 0;
    HI_U16 usId = id;
    MRS_DEVICE_INF_STRU stDevInf = {{0}, 0, 0, 0, 0};

    MRS_NOT_USED(packet_size);

    //������
    plc_data = (MRS_PLC_FRAME_DATA_STRU *)req_packet;

    if (plc_data->pucPayload == HI_NULL) {
        return HI_ERR_FAILURE;
    }

    mac = plc_data->ucMac;
   (hi_void)memcpy_s(cco_ctx->aucSrcMac, HI_PLC_MAC_ADDR_LEN, mac, HI_PLC_MAC_ADDR_LEN);

#if defined(PRODUCT_CFG_SYS_RST_SOLUTION)
    cco_ctx->stResetStatInfo.ulRMStatTimes++;
#endif

    pPayload = plc_data->pucPayload;
    usPayloadLen = plc_data->usPayloadLen;

    if (usId == ID_MRS_CMD_DATA_TRANSMIT_PLC)
    {
        MRS_CMD_DATA_TRANSMIT_STRU *pstTransmit = (MRS_CMD_DATA_TRANSMIT_STRU *)pPayload;

        /* ���ĳ���У�� */
        if (pstTransmit->usPayloadLen + sizeof(MRS_CMD_DATA_TRANSMIT_STRU) != usPayloadLen) {
            return HI_ERR_INVALID_PARAMETER;
        }

        switch (pstTransmit->usCmdId) {
            case ID_MRS_CMD_DATA_TRANSMIT_TEST_MODE:
                if (pstTransmit->usPayloadLen != sizeof(MRS_CMD_TEST_MODE_STRU)) {
                    return HI_ERR_INVALID_PARAMETER;
                }
                return mrsCcoTestModeFrameRx((MRS_CMD_TEST_MODE_STRU *)pstTransmit->aucPayload);

            case ID_MRS_CMD_DATA_TRANSMIT_AFN13_PLC:
            case ID_MRS_CMD_DATA_TRANSMIT_AFN14_PLC: {
                HI_U16 proto_ver = HI_MDM_GetProtocolVer();
                /* ����V100Э��֧�� */
                if (proto_ver != HI_MDM_PTCL_VER_100) {
                    return HI_ERR_NOT_SUPPORT;
                }

                if (mrsCcoIsTestMode()) {
                    pPayload = pstTransmit->aucPayload;
                    usPayloadLen = pstTransmit->usPayloadLen;
                    usId = pstTransmit->usCmdId;
                    break;
                }
            }
                // ��break;
            default:
                return HI_ERR_NOT_SUPPORT;
        }
    }

    (hi_void)memset_s(stDevInf.aucAddr, HI_METER_ADDR_LEN, 0xFF, HI_METER_ADDR_LEN);
    stDevInf.ucProtocol = METER_PROTO_TRANSPARENT;
    stDevInf.ucType = MRS_MAP_DEV_TYPE_METER;
    stDevInf.ucStatus = MRS_SEARCH_STATUS_UNKNOWN;

    switch (usId)
    {
    case ID_MRS_CMD_DATA_TRANSMIT_AFN13_PLC:
        mrsDfxXrPlcRx();
/* BEGIN: Modified by fengxiaomin/00209182, 2014/3/27   ���ⵥ��:DTS2014032906596 */
        mrsDfxRmPlcUpRx(ID_MRS_CMD_DATA_TRANSMIT_AFN13_PLC, pPayload, HI_FALSE);
/* END:   Modified by fengxiaomin/00209182, 2014/3/27 */
		MRS_DATA_TRANSMIT_AFN13_UP_STRU *pstTransmit = (MRS_DATA_TRANSMIT_AFN13_UP_STRU *)pPayload;

		HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_080, HI_DIAG_MT("[RF]TRANSMIT_AFN13_PLC"),pstTransmit->protocol,pstTransmit->data[0]);

		if((pstTransmit->protocol==WIRELESS_TRANSMISSION_PROTOCOL)
			&&(cco_ctx->stWaterMeterSearchModel.bSetMeterShipSendPLCFlag ==1))
		{
			cco_ctx->stWaterMeterSearchModel.bSetMeterShipSendPLCFlag=0;
			ret = mrsCmdCCORecvWaterMeterSetShip(pPayload,usPayloadLen);
			return ret;
		}
		if(pstTransmit->protocol==WIRELESS_TRANSMISSION_PROTOCOL)
		{
			// ��һ�δ�����Ϊ���л�ˮ��������ϵ׼����
			MR_WATER_METER_SEARCH_STRU *pWaterMeter=&(mrsCcoGetContext()->stWaterMeterSearchModel);
			pWaterMeter->stWaterMeterResult[pWaterMeter->ucReadWaterMeterDataIndex].ucWaterMaxTryCount=0;
		}
        if(cco_ctx->status & MRS_CCO_STATUS_ANYTIME_READMETER)
        {
            ret = mrsCcoRxAfn13DataPro(pPayload, usPayloadLen, (HI_PVOID*)&data_645, &data_len, mac);

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

           (hi_void)memcpy_s(stDevInf.aucAddr, HI_METER_ADDR_LEN, cco_ctx->stAnytimeRMModel.aucMeterAddr, HI_METER_ADDR_LEN);
            stDevInf.ucProtocol = cco_ctx->stAnytimeRMModel.ucProtocol;
        }
        break;

    case ID_MRS_CMD_DATA_TRANSMIT_AFN14_PLC:
        HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_016, HI_DIAG_MT("AFN-14-status=%d"), cco_ctx->status);
        mrsDfxLrPlcRx();
        mrsDfxRmPlcUpRx(ID_MRS_CMD_DATA_TRANSMIT_AFN14_PLC, pPayload, HI_FALSE);

        if (cco_ctx->status & MRS_CCO_STATUS_NORMAL)
        {
            MRS_ARCHIVES_NODE_STRU *pstNode = HI_NULL;
            ret = mrsCcoRxAfn14DataPro(pPayload, usPayloadLen, (HI_PVOID*)&data_645, &data_len, mac);
            HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_041, HI_DIAG_MT("AFN-14-pro-ret=%d"),ret);
            pstNode = mrsSrvArchivesQuery(cco_ctx->stReadMeterModel.usCurrNodeSeq);
            if (pstNode != HI_NULL)
            {
               (hi_void)memcpy_s(stDevInf.aucAddr, HI_METER_ADDR_LEN, pstNode->aucNodeAddr, HI_METER_ADDR_LEN);
                stDevInf.ucProtocol = pstNode->ucProtocol;
            }
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
            ret = mrsRmPlcPackageReceive(parallel_data, usPayloadLen, &stDevInf, mac);
            data_645 = (HI_U8 *)parallel_data + parallel_data->stru_size;
            data_len = parallel_data->data_len;
        }
        break;

    default:
        break;
    }

    if ((ret != HI_ERR_SUCCESS) || (0 == data_len))
    {
        HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_019, HI_DIAG_MT("plc data process fail-[%d]"),ret);
        return HI_ERR_FAILURE;
    }
	HI_U16 usStartPos=0;
	HI_U16 usDataLen=0;
    // ά��ӳ���
    (hi_void)memset_s(&frame, sizeof(frame), 0, sizeof(frame));
    if (MRS_Proto645Dec(data_645, data_len,&frame) == HI_ERR_SUCCESS)
    {
        if (MRS_645_FRAME_CONTROL_DIR_RESPONSION_BIT == frame.stCtrl.ucDir)
        {
            HI_U8 ucPhase = 0;

            ucPhase = ((PHASE_C == cco_ctx->ucPhase) ? MRS_PHASE_C : cco_ctx->ucPhase);
            mrsRmMapProc(&frame, &cco_ctx->stMapModule, mac, ucPhase);
        }
        else
        {
            HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_022, HI_DIAG_MT("data 645 error dir."));
        }

        return HI_ERR_SUCCESS;
    }
	else if(mrsFind188Frame(data_645, data_len,&usStartPos,&usDataLen) == HI_ERR_SUCCESS)
	{
        HI_U8  ucT188Type=mrsGetT188ProMeterType(data_645+usStartPos,usDataLen);

        ret = mrsMapAddMacMeter188(&cco_ctx->stMapModule,mac,data_645+usStartPos+2,ucT188Type);
		HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_062, HI_DIAG_MT("188!"),ret);
	}

     /* 698.45Э�����͸��Э�� */
    if (mrsToolsNormalAddr(stDevInf.aucAddr))
    {
        HI_U8 ucStatus = 0;
        HI_U8 dir = 0;
        HI_BOOL flag = HI_FALSE;

        if (HI_ERR_SUCCESS != mrsSetDeviceStatus(mac, &ucStatus))
        {
            return HI_ERR_SUCCESS;
        }

        if (METER_PROTO_698_45 != stDevInf.ucProtocol)
        {
            flag = HI_TRUE;
        }
        else
        {
            if (HI_ERR_SUCCESS == mrsFind698Dir(data_645, data_len, &dir))
            {
                if (MRS_698_FRAME_CONTROL_DIR_RESPONSION_BIT == dir)
                {
                    flag = HI_TRUE;
                }
                else
                {
                    HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_023, HI_DIAG_MT("data 698 error dir."));
                }
            }
        }

        if (HI_TRUE == flag)
        {
            HI_U8 ucPhase = 0;

            stDevInf.ucStatus = ucStatus;
            ucPhase = ((PHASE_C == cco_ctx->ucPhase) ? MRS_PHASE_C : cco_ctx->ucPhase);
            mrsMapAddMeter(&cco_ctx->stMapModule, mac, &stDevInf, ucPhase);
            mrsMapMarkMac(&cco_ctx->stMapModule, mac, stDevInf.ucStatus);
        }
    }

    return HI_ERR_SUCCESS;
}

//�������㳭ʱ�����æ������������ӵ�����������ӳɹ���ʱ2s
HI_U32 mrsCcoNdmPlcBusyAdd(AFN_FN_UL_PARAM * pst_afn_param, HI_U8 anytime_from)
{
	MRS_CCO_SRV_CTX_STRU * cco_ctx = mrsCcoGetContext();

	//��æ���ش���û�н���ʱ��AFN13 F1�������յ�һ����֡����֡����ԭ�����̷���(���ؿ�֡)
	if ((pst_afn_param == HI_NULL) || (cco_ctx->stCcoNdmPlcBusyInfo.pst_frame != HI_NULL))
	{
		return HI_ERR_SUCCESS;
	}

	if((pst_afn_param->ucNdmPlc && (cco_ctx->stAnytimeRMModel.from != XR_FROM_NDM_AFN13_F1)) ||
		((cco_ctx->stAnytimeRMModel.from == XR_FROM_NDM_AFN13_F1) && (!pst_afn_param->ucNdmPlc)))
	{
		cco_ctx->stCcoNdmPlcBusyInfo.pst_frame = pst_afn_param->pstFrame;
		cco_ctx->stCcoNdmPlcBusyInfo.busy_from = anytime_from;
		if (cco_ctx->stCcoNdmPlcBusyInfo.busy_from == XR_FROM_NDM_AFN13_F1)
		{
		    (hi_void)memcpy_s(&(cco_ctx->stCcoNdmPlcBusyInfo.ndm_plc_info), sizeof(MR_CCO_NDM_PLC_INFO_STRU),
				&(cco_ctx->stCcoNdmPlcInfo), sizeof(MR_CCO_NDM_PLC_INFO_STRU));
		}
		MRS_StartTimer(MRS_CCO_TIMER_NDM_PLC_READ_METER, MRS_CCO_NDM_PLC_BUSY_PROC, HI_TIMER_TYPE_ONCE);
		return HI_ERR_BUSY;
	}

	return HI_ERR_SUCCESS;
}

//�������㳭ʱ�����æ������ʱ2sʱ���ؿ�֡
HI_VOID mrsCcoNdmPlcTimeout(HI_VOID)
{
	HI_U32 ret = HI_ERR_SUCCESS;
	HI_PBYTE pout_buffer   = HI_NULL;
	HI_U16 buffer_outlen = 0;
	MRS_CCO_SRV_CTX_STRU * cco_ctx = mrsCcoGetContext();
	HI_U8 data_buff[12] = {0};
	HI_U8 anytime_from = cco_ctx->stAnytimeRMModel.from;

	if (cco_ctx->stCcoNdmPlcBusyInfo.pst_frame == HI_NULL)
	{
		return;
	}

	cco_ctx->stAnytimeRMModel.from = cco_ctx->stCcoNdmPlcBusyInfo.busy_from;
	ret = mrsCreateReadMeterFrame376_2(0, data_buff, 0, &pout_buffer, &buffer_outlen, HI_NULL);
	cco_ctx->stAnytimeRMModel.from = anytime_from;

	if ((ret == HI_ERR_SUCCESS) && (pout_buffer != HI_NULL))
	{
		if (cco_ctx->stCcoNdmPlcBusyInfo.busy_from == XR_FROM_NDM_AFN13_F1)
		{
			mrsMsgSendPlcToNdm(pout_buffer, buffer_outlen); // ����"��Ӧ"֡
		} else {
			MRS_SendMrData(pout_buffer, buffer_outlen, HI_DMS_CHL_UART_PORT_APP); // ����"��Ӧ"֡
		}
		mrsToolsFree(pout_buffer);
	}
	mrsToolsFree(cco_ctx->stCcoNdmPlcBusyInfo.pst_frame);
	cco_ctx->stCcoNdmPlcBusyInfo.pst_frame = HI_NULL;
}


//Matteo
HI_U32 mrsArchiveChangeQueryCltDeviceType(MRS_CCO_SRV_CTX_STRU* pstCcoCtx)
{
	HI_U32 ret=HI_ERR_SUCCESS;
	HI_U16 usMeterCount=0,usWaterMeterCount=0;
	HI_U32 i=0;
	
//	HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_003, HI_DIAG_MT("[Matteo] do mrsArchiveChangeQueryCltDeviceType"));
	
	if (mrsSrvArchivesIsChanged())
    {
		HI_U16 usMeterAllNum = mrsSrvArchivesMeterNum();
		MRS_ARCHIVES_NODE_STRU *pstNode = HI_NULL;
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
			   usWaterMeterCount++;
		   }
		   else
		   {
			   usMeterCount++;
		   }
	   }
	   
	   if(pstCcoCtx->LowPowerMeterLastNum != usWaterMeterCount)//�͹��ı����仯
	   {
		   ret =mrsCcoSendStartQueryDeviceType(pstCcoCtx);
		   if (HI_ERR_SUCCESS != ret)
		   {
			   HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_018, HI_DIAG_MT("QueryDeviceType err: [ret]"), ret);
		   }
		   pstCcoCtx->LowPowerMeterLastNum = usWaterMeterCount;
	   }
    }
}




//�������㳭ʱ�����æ�����㳭����ʱ��������
HI_VOID mrsCcoNdmPlcBusyProc(HI_VOID)
{
	HI_U32 ret = HI_ERR_SUCCESS;
	HI_PBYTE pout_buffer   = HI_NULL;
	HI_U16 buffer_outlen = 0;
	MRS_CCO_SRV_CTX_STRU * cco_ctx = mrsCcoGetContext();
	HI_BOOL is_ndm_plc = HI_FALSE;

	MRS_StopTimer(MRS_CCO_TIMER_NDM_PLC_READ_METER);
	if(cco_ctx->stCcoNdmPlcBusyInfo.pst_frame == HI_NULL)
	{
        return;
	}

	if (cco_ctx->stCcoNdmPlcBusyInfo.busy_from == XR_FROM_NDM_AFN13_F1)
	{
		is_ndm_plc = HI_TRUE;
	}

	ret = mrs3762AfnDispatch(cco_ctx->stCcoNdmPlcBusyInfo.pst_frame, &pout_buffer, &buffer_outlen, is_ndm_plc);
	if ((ret == HI_ERR_SUCCESS) && (pout_buffer != HI_NULL))
	{
		if (cco_ctx->stCcoNdmPlcBusyInfo.busy_from == XR_FROM_NDM_AFN13_F1)
		{
			mrsMsgSendPlcToNdm(pout_buffer, buffer_outlen); // ����"��Ӧ"֡
		} else {
			MRS_SendMrData(pout_buffer, buffer_outlen, HI_DMS_CHL_UART_PORT_APP); // ����"��Ӧ"֡
		}
		mrsToolsFree(pout_buffer);
	}
	mrsToolsFree(cco_ctx->stCcoNdmPlcBusyInfo.pst_frame);
	cco_ctx->stCcoNdmPlcBusyInfo.pst_frame = HI_NULL;
}

//��NDM��plc֡�㳭�����æ���ش�����ж�
HI_BOOL mrsCcoIsNdmPlcBusyBack(MRS_3762_FRAME_STRU * pst_frame)
{
	HI_U8 fn_index = 0;

	if ((pst_frame != HI_NULL) && (pst_frame->strUsrDataHrd.stAppDataRealm.usAppDataLen >= 2))
	{
		fn_index = mrs3762GetDtFnIndex(pst_frame->strUsrDataHrd.stAppDataRealm.pucAppDataContent);
		if ((pst_frame->strUsrDataHrd.stAppDataRealm.ucAfn == 0x02 && fn_index == 0x00) ||
			(pst_frame->strUsrDataHrd.stAppDataRealm.ucAfn == 0x13 && fn_index == 0x00))
		{
			return HI_TRUE;
		}
	}

	return HI_FALSE;
}

//��cco���ظ���������������װ��plc֡������
HI_U32 mrsMsgSendPlcToNdm(HI_PBYTE out_buffer, HI_U16 buffer_outlen)
{
	HI_U32 ret = HI_ERR_SUCCESS;
	MRS_PLC_FRAME_DATA_STRU frame_data = { 0 };
	HI_PBYTE pout_bufftemp = (HI_PBYTE)mrsToolsMalloc(buffer_outlen + 3);
 	MRS_CCO_SRV_CTX_STRU * cco_ctx = mrsCcoGetContext();

	if (pout_bufftemp)
	{
		frame_data.usId = ID_MRS_CMD_NDM_CTRL_CCO_PLC;
	    (hi_void)memcpy_s(frame_data.ucMac, HI_PLC_MAC_ADDR_LEN, cco_ctx->stCcoNdmPlcInfo.ndm_addr, HI_PLC_MAC_ADDR_LEN);
		frame_data.is_bc_frame = HI_FALSE;

		*pout_bufftemp = cco_ctx->stCcoNdmPlcInfo.pro_type;
		*(pout_bufftemp+1) = buffer_outlen & 0xff;
		*(pout_bufftemp+2) = (buffer_outlen >> 8) & 0xff;
	    (hi_void)memcpy_s(pout_bufftemp+3, buffer_outlen, out_buffer, buffer_outlen);

		frame_data.pucPayload = pout_bufftemp;
		frame_data.usPayloadLen = buffer_outlen + 3;

		mrsDfxChlNdmPlcTx(&frame_data);
		ret = MRS_SendPlcFrame(&frame_data);
		if (HI_ERR_SUCCESS != ret)
		{
			mrsDfxChlNdmPlcErrTx(&frame_data, ret);
		}

		mrsToolsFree(pout_bufftemp);
	}

	return ret;
}

//CCO���յ�NDM��PLC֡����
HI_U32 mrsCmdCcoNdmPlcProc(HI_IN HI_U16 id, HI_PVOID req_packet, HI_U16 packet_size)
{
	HI_U32 ret = HI_ERR_SUCCESS;
    MRS_PLC_FRAME_DATA_STRU *plc_data = HI_NULL;
	MRS_CCO_SRV_CTX_STRU * cco_ctx = mrsCcoGetContext();
	MRS_3762_FRAME_STRU* pst_frame = HI_NULL;
	HI_PBYTE pout_buffer   = HI_NULL;
	HI_U16 buffer_outlen = 0;

    HI_UNREF_PARAM(id);
    HI_UNREF_PARAM(packet_size);

    /* ����Э����չ֧����ά���ر��������汾��֧�� */
    if (mrs_check_area_code(APP_AREA_CODE_BEIJING) == HI_FALSE) {
        return HI_ERR_NOT_SUPPORT;
    }

    //������
    plc_data = (MRS_PLC_FRAME_DATA_STRU *)req_packet;
    if ((!plc_data) || (!plc_data->pucPayload) || (plc_data->usPayloadLen <= 3))
    {
        return HI_ERR_FAILURE;
    }

    mrsDfxChlNdmPlcRx(plc_data);
	ret = MRS_Proto3762Dec(plc_data->pucPayload+3, plc_data->usPayloadLen-3, &pst_frame);
	if (ret != HI_ERR_SUCCESS)
	{
		mrsDfxChlNdmPlcErrRx(plc_data, ret);
		return HI_ERR_SUCCESS;
	}

	//cco���յ�ndm��plc֡����Ҫ���ι㲥֡������AFNֵ��Ӧ��֡
	//���εĹ㲥֡��plc֡��Դ��Ŀ�ĵ�ַ��3762֡��Ŀ�ĵ�ַ��
	if ((pst_frame->strUsrDataHrd.stAppDataRealm.ucAfn == 0x01) || (pst_frame->strUsrDataHrd.stAppDataRealm.ucAfn == 0x05) ||
		(pst_frame->strUsrDataHrd.stAppDataRealm.ucAfn == 0x11) || (pst_frame->strUsrDataHrd.stAppDataRealm.ucAfn == 0x15) ||
		(HI_TRUE == plc_data->is_bc_frame) || mrsToolsBroadCastMac(plc_data->ucMac))
	{
		mrsDfxChlNdmPlcErrRx(plc_data, HI_ERR_NOT_SUPPORT);
		mrsToolsFree(pst_frame);
		return HI_ERR_SUCCESS;
	}

	cco_ctx->stCcoNdmPlcInfo.pro_type = *(plc_data->pucPayload);
	cco_ctx->stCcoNdmPlcInfo.frame_len = HI_MAKEU16(*(plc_data->pucPayload+1), *(plc_data->pucPayload+2));
    (hi_void)memcpy_s(cco_ctx->stCcoNdmPlcInfo.ndm_addr, HI_PLC_MAC_ADDR_LEN, plc_data->ucMac, HI_PLC_MAC_ADDR_LEN);
    (hi_void)memcpy_s(cco_ctx->stCcoNdmPlcInfo.src_addr, HI_PLC_MAC_ADDR_LEN,
		pst_frame->strUsrDataHrd.stAddrRealm.ucSrcAdd, HI_PLC_MAC_ADDR_LEN);

	ret = mrs3762AfnDispatch(pst_frame, &pout_buffer, &buffer_outlen, HI_TRUE);

	//ndm�·�plc֡�㳭��ɵ�æ���أ����ͷ�pstframe�ڴ�
	if ((ret == HI_ERR_BUSY) && (pst_frame->strUsrDataHrd.stAppDataRealm.ucAfn == 0x13))
	{
		return HI_ERR_SUCCESS;
	}

	if ((ret == HI_ERR_SUCCESS) && (pout_buffer != HI_NULL))
	{
		mrsMsgSendPlcToNdm(pout_buffer, buffer_outlen); // ����"��Ӧ"֡
	}

	//������plc�㳭���æ���ػ��崦��
	if((((ret == HI_ERR_SUCCESS) && (pout_buffer != HI_NULL)) || (ret != HI_ERR_SUCCESS)) &&
		(pst_frame->strUsrDataHrd.stAppDataRealm.ucAfn == 0x13))
	{
	    mrsCcoNdmPlcBusyProc();
	}

	mrsToolsFree(pout_buffer);
	mrsToolsFree(pst_frame);

    return HI_ERR_SUCCESS;
}

HI_U32 mrsCmdCcoPlcTestTransmit(HI_IN HI_U16 id, HI_PVOID req_packet, HI_U16 packet_size)
{
    MRS_PLC_FRAME_DATA_STRU *plc = (MRS_PLC_FRAME_DATA_STRU *)req_packet;
    MRS_TEST_TRANSMIT_AFNF0_STRU *trans = HI_NULL;
    PROTO376_2_FRAME_PRE_STRU frame = {0};

    HI_PBYTE buf = HI_NULL;
    HI_PBYTE payload = HI_NULL;
    HI_U16 buf_len = 0;
    HI_U16 payload_len = 0;
    HI_U32 ret = HI_ERR_SUCCESS;

    HI_UNREF_PARAM(id);
    HI_UNREF_PARAM(packet_size);

    if (req_packet == HI_NULL) {
        return HI_ERR_INVALID_PARAMETER;
    }

    if (plc->pucPayload == HI_NULL) {
        return HI_ERR_INVALID_PARAMETER;
    }

    trans = (MRS_TEST_TRANSMIT_AFNF0_STRU *)plc->pucPayload;
    if (plc->usPayloadLen != trans->stru_len + trans->expect_len) {
        return HI_ERR_INVALID_PARAMETER;
    }

    payload_len = sizeof(HI_U16) + trans->expect_len;
    payload = mrsToolsMalloc(payload_len);
    if (payload == HI_NULL) {
        return HI_ERR_MALLOC_FAILUE;
    }

    (hi_void)memcpy_s(payload, payload_len, &(trans->expect_len), sizeof(HI_U16));
    (hi_void)memcpy_s(payload + sizeof(HI_U16), payload_len - sizeof(HI_U16), plc->pucPayload + trans->stru_len, trans->expect_len);

    (hi_void)memset_s(&frame, sizeof(frame), 0, sizeof(frame));
    frame.ucModuleFlag = 1;
    (hi_void)memcpy_s(frame.ucSrcAdd, sizeof(frame.ucSrcAdd), plc->ucMac, HI_PLC_MAC_ADDR_LEN);
    mrsHexInvert(frame.ucSrcAdd, HI_PLC_MAC_ADDR_LEN);
    frame.stAfnData.ucAFN = MRS_AFN(0xF0);
    frame.stAfnData.ucDT[0] = MRS_AFN_FN_LO(1);
    frame.stAfnData.ucDT[1] = MRS_AFN_FN_HI(1);
    frame.stAfnData.usDataLen = payload_len;
    frame.stAfnData.pData = payload;

    ret = mrsCreate3762Frame(&frame, &buf, &buf_len, HI_NULL);
    if (HI_ERR_SUCCESS == ret) {
        ret = MRS_SendMrData(buf, buf_len, HI_DMS_CHL_UART_PORT_APP);
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
    HI_U8  ucPhase = 0;

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

    mrsMapDeleteMac(&pstCcoCtx->stMapModule, pstPlcFrame->ucMac, HI_FALSE);
    mrsMapMarkMac(&pstCcoCtx->stMapModule, pstPlcFrame->ucMac, pstUpFrame->stDeviceInf.ucStatus);
    ucPhase = mrsCcoGetStaPhase(pstPlcFrame->ucMac);
    ucPhase = ((PHASE_C == ucPhase) ? MRS_PHASE_C : ucPhase);

    if (MRS_MAP_DEV_TYPE_METER == pstUpFrame->stDeviceInf.ucType)
    {
        mrsMapAddMeter(&pstCcoCtx->stMapModule, pstPlcFrame->ucMac, &pstUpFrame->stDeviceInf, ucPhase);
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

            if (HI_ERR_SUCCESS != mrsMapAddMeter(&pstCcoCtx->stMapModule, pstPlcFrame->ucMac, &stDevInf, ucPhase))
            {
                break;
            }
        }
    }

    return HI_ERR_SUCCESS;
}


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

    HI_UNREF_PARAM(usMsgId);

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
#if defined(PRODUCT_CFG_SUPPORT_MRS_DUOL_MODE_RF)
			if((pstFrame->strUsrDataHrd.stAppDataRealm.ucAfn == 0x04) 
				&& (pstFrame->strUsrDataHrd.stAppDataRealm.pucAppDataContent[0]==0x08)
				&& (pstFrame->strUsrDataHrd.stAppDataRealm.pucAppDataContent[1]==0x0))
			{
				
				HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1015, HI_DIAG_MT("ҵ��ģ���յ�����ת��������ģ��"), pstData->pucData, pstData->usDataSizse);
				// ҵ��ģ���յ�����ת��������ģ��
				ret = MRS_SendMrData(pstData->pucData, pstData->usDataSizse, HI_DMS_CHL_UART_PORT_DIAG);
				mrsToolsFree(pstFrame);
				return ret;
			}
#endif
            ret = mrs3762AfnDispatch(pstFrame, &pOutBuffer, &usBufferOutLen, HI_FALSE);

			//������plc֡�㳭�����æ���ش���,���ͷ��ڴ�pstframe
			if((ret == HI_ERR_BUSY) && mrsCcoIsNdmPlcBusyBack(pstFrame))
			{
				return HI_ERR_SUCCESS;
			}

            if (ret == HI_ERR_SUCCESS)
            {
                if (pOutBuffer != HI_NULL)
                {
                    ret = MRS_SendMrData(pOutBuffer, usBufferOutLen, HI_DMS_CHL_UART_PORT_APP); // ����"��Ӧ"֡
                }
            }

			//������plc�㳭���æ���ػ��崦��
            if((((ret == HI_ERR_SUCCESS) && (pOutBuffer != HI_NULL)) || (ret != HI_ERR_SUCCESS)) && mrsCcoIsNdmPlcBusyBack(pstFrame))
			{
				mrsCcoNdmPlcBusyProc();
			}

            mrsToolsFree(pOutBuffer);
        }
        mrsToolsFree(pstFrame);
    }

    return ret;
}

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
        mrsReadMeterStaTimerProcess(pstCcoCtx);
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
        mrs3762_ReportAFN06_F3(MRS_CCO_SEARCH_METER_TASK_END);
    }
}

HI_VOID mrsGetTimeTimerProcess( MRS_CCO_SRV_CTX_STRU* pstCcoCtx)
{
    if (pstCcoCtx->bFirstRun == HI_TRUE)
    {
        pstCcoCtx->bFirstRun = HI_FALSE;

        if (pstCcoCtx->ucProtoVer == PROTO_VER_13762_2013)
        {
            mrs3762_AFN03_F10_UP();
        }

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

    case EN_MRS_SRV_CCO_MUSTER_TIMER:   // �뼯����ͨѶ��ʱ������
        mrsReadMeterTotalMusterTimerProcess(HI_NULL);
        break;

    case EN_MRS_SRV_CCO_READMETER_ACTIV_TIMER:
        mrsReadMeterActiveTimer(pstCcoCtx);
        break;

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
		#if defined (PRODUCT_OPTIMIZE_MATTEO)
        mrsChgFrameJoinCcoQueue();
		#endif
        break;

    case MRS_CCO_TIMER_NODE_STATECHG_ENTRY_CACHE:
		#if defined (PRODUCT_OPTIMIZE_MATTEO)
        mrsChgCacheTimeoutProc();
		#endif
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

    case MRS_CCO_TIMER_NOTIFY_STA_LOCK_NETWORK:
        ret = mrsCcoSearchMeterNotifyStaBindNetwork(pstCcoCtx);
        break;

    case MRS_CCO_TIMER_TOPO_QUERY:
        ret = mrsCcoSearchMeterTopoQueryProc(pstCcoCtx);
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

    case MRS_CCO_TIMER_CHECK_MAIN_NODE_ADDR:
        mrsCcoHandleCheckMainAddrTimeout(pstCcoCtx);
        break;

#if defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG)
    case MRS_CCO_TIMER_ACQUIRE_CLOCK:
        mrsCcoRequestClockTimeout();
        break;

    case MRS_CCO_TIMER_MRS_LOG:
        //mrsLogTimerProc();//optimized by weichao
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
        mrsUpgStatusEndPro(mrsGetRemoteUpgCtx());
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
    case MRS_CCO_TIMER_POWEROFF_DETECT:
		#if defined (PRODUCT_OPTIMIZE_MATTEO)
        mrsPoweroffRunDetectProcess(mrsGetChgReportCtx());
		#endif
        break;
    case MRS_CCO_TIMER_POWEROFF_RECHECK:
		#if defined (PRODUCT_OPTIMIZE_MATTEO)
        mrsPoweroffRunRecheckProcess(mrsGetChgReportCtx());
		#endif
        break;
    case MRS_CCO_TIMER_POWEROFF_STABLE_CHECK:
		#if defined (PRODUCT_OPTIMIZE_MATTEO)
        mrsPoweroffStableCheck(mrsGetChgReportCtx());
		#endif
        break;
    case MRS_CCO_TIMER_POWEROFF_MAC_CACHE:
		#if defined (PRODUCT_OPTIMIZE_MATTEO)
        mrsNodeStateChgPowerOffProc();
		#endif
        break;
    case MRS_CCO_TIMER_POWEROFF_DEAL_SCALE_OFF:
		#if defined (PRODUCT_OPTIMIZE_MATTEO)
        mrsPowerOffDealScaleOff();
		#endif
        break;
    /* BEGIN: Modified by liuying/00369907, 2017/2/13   */
    #ifdef PRODUCT_CFG_TTCN
    case MRS_CCO_TIMER_TESTMODE:
        HI_MDM_ExitPlcTest();
        break;

    case MRS_CCO_TIMER_ENTER_TESTMODE:
        if (PHYSICAL_TEST_MODE_APP_TO_COM_LOOP == HI_MDM_GetPhysicalTestMode()
            || PHYSICAL_TEST_MODE_EXIT == HI_MDM_GetPhysicalTestMode())
        {
           return  ret;
        }
        mrsSrvCaptureDeinit();
        break;

    /* END: Modified by Modified by liuying/00369907, 2017/2/13  */
    case MRS_CCO_TIMER_PHYSICAL_TESTMODE://ҵ�񴮿ڷ���
    	{
	    	HI_U8 ucMode = HI_MDM_GetPhysicalTestMode();
	        if (ucMode == PHYSICAL_TEST_MODE_APP_TO_COM_LOOP)//�����͸��ת��
	        {
                MRS_RestoreUartConfig(MRS_SRV_BAUD_RATE_9600, MRS_SRV_UART_PARITY_EVEN);

            	HI_MDM_ExitPlcTest();
	            mrsSrvCaptureDeinit();
	            mrsDfxStaTestModeExit(PHYSICAL_TEST_MODE_APP_TO_COM_LOOP,ucMode);
	        }
    	}
        break;
    case MRS_CCO_TIMER_MSDU_TESTMODE:
        {
			HI_U8 ucMode = HI_MDM_GetPhysicalTestMode();
            if (ucMode == PHYSICAL_TEST_MODE_EXIT)//�����͸��ת��
            {
                MRS_RestoreUartConfig(MRS_SRV_BAUD_RATE_9600, MRS_SRV_UART_PARITY_EVEN);

                HI_MDM_ExitPlcTest();
                mrsSrvCaptureDeinit();
                mrsDfxStaTestModeExit(PHYSICAL_TEST_MODE_EXIT,ucMode);
            }

        }
        break;
    #endif
    //����ͣ���ϱ��¼�
    case MRS_CCO_TIMER_POWERFAILURE:
        mrsCcoRspPowerFailureEvt();
        break;

    case MRS_CCO_TIMER_POWERON:
        mrsCcoRspPowerOnEvt();
        break;
    //����cco�ϱ�����
    case MRS_CCO_TIMER_POWERFAILURE_ERASE_DATA:
        mrsEraseBitmap();
        break;
	case MRS_CCO_TIMER_POWERON_ERASE_DATA:
		mrs_cco_erase_power_on();
		break;

    case MRS_CCO_TIMER_CLT_POWEOFF_ERASE_DATA:
        mrsCcoDelCltPowerListItem();
        break;

    case MRS_CCO_TIMER_CLT_POWER_REPORT:
        mrsCcoReportCltPowerOffOnEvt();
        break;

    case MRS_CCO_TIMER_TF_RESULT_QUERY_NEXT:
        mrsTfCcoRunQueryProcess();
        break;

    case MRS_CCO_TIMER_TF_RESULT_QUERY_ROUND:
        mrsTfCcoStartNewQueryRound();
        break;

    case MRS_CCO_TIMER_TF_RESULT_QUERY_BEGIN:
        mrsTfCcoBeginQueryProcess();
        break;

		case MRS_CCO_TIMER_RF_TIME:
		mrsCcoSendStartFindOwner();
		break;
	case MRS_CCO_TIMER_RF_READ_WATERMETER:
		mrsReadWaterMeterActiveTimer(pstCcoCtx);
		break;
	case MRS_CCO_TIMER_GET_WATER_METERLIST:
		if(pstCcoCtx->stWaterMeterSearchModel.bStartReadMeterFlag)
		{
			mrs3762_RF_ReportAFN04_F7(0);
			
			mrsCcoSearchMeterGetWaterMeterlist(pstCcoCtx,HI_TRUE);
			pstCcoCtx->stWaterMeterSearchModel.bStartReadMeterFlag=0;
			mrsSrvListClear(&(pstCcoCtx->stWaterMeterSearchModel.stGetWaterMeterList), RfFreeWaterMeterListNode, HI_NULL);
		}
		else
		{
			mrsCcoSearchMeterGetWaterMeterlist(pstCcoCtx,HI_FALSE);
		}
		break;
	case MRS_CCO_TIMER_SET_WATER_METERLIST_SHIP:
		pstCcoCtx->stWaterMeterSearchModel.bSetMeterShipSendPLCFlag=0;
		if(pstCcoCtx->stWaterMeterSearchModel.bStartSetMeterShipFlag)
		{
			pstCcoCtx->stWaterMeterSearchModel.bStartSetMeterShipFlag = HI_FALSE;
			mrsCcoCalcWaterMeterShip(pstCcoCtx);
			mrsCcoRFDau0x44EncodeFrame();
			mrsCcoSearchWaterMeterSetShip(pstCcoCtx,HI_TRUE);
		}
		else
		{
			mrsCcoSearchWaterMeterSetShip(pstCcoCtx,HI_FALSE);
		}
		break;
	case MRS_CCO_TIMER_RF_UART_TIME:
		mrsCcoRfUartTimerFun();
		break;
	case MRS_CCO_TIMER_SGD_RF_TIME:
		mrsCcoSGDTimeSlotFun();
		break;
		
	case MRS_CCO_TIMER_NDM_PLC_READ_METER:
		mrsCcoNdmPlcTimeout();
		break;

	case MRS_CCO_TIMER_STA_PLC_READ_DEV_ID:
		mrs_plc_detect_id_timeout();
		break;

	//Matteo
	case MRS_CCO_TIMER_ARCHIVE_CHANGE_CHECK:
		mrsArchiveChangeQueryCltDeviceType(pstCcoCtx);
		mrsCcoFindTypeTimeoutHandle(pstCcoCtx);
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

/* BEGIN: PN: DTSYYYYMMDDXXXXX MODIFY\ADD\DEL by cuiate/00233580 at 2015/6/23 */
    if (pstCcoCtx->status & MRS_CCO_STATUS_SEARCH_METER_TRANSFORMER)
    {
        mrsCcoSetState(MRS_CCO_STATUS_NORMAL | MRS_CCO_STATUS_SEARCH_METER_TRANSFORMER);
    }
    else
    {
        mrsCcoSetState(MRS_CCO_STATUS_NORMAL);
    }
/* END:   PN: DTSYYYYMMDDXXXXX MODIFY\ADD\DEL by cuiate/00233580 at 2015/6/23 */

    mrsReadMeterStateInit();    // ��ʼ������״̬
}


//��ȡ��ǰ��ά����MAC
HI_U32 mrsMapGetCurrentMac(MRS_MAP_MODULE *module,HI_U8 mac[])
{
    if ((module != HI_NULL) && (mac != HI_NULL))
    {
        if (module->stNewMacList.number > 1)
        {
           (hi_void)memcpy_s(&module->stNewMacList.mac[0], HI_PLC_MAC_ADDR_LEN, mac, HI_PLC_MAC_ADDR_LEN);
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
    pstCtx->stAutoSearchModel.stSearchCfg.ucBindNetwork = cfg->ucBindNetwork;

    if(cfg->ucSoftTfMode < SOFT_TF_MODE_INVALID)
    {
        pstCtx->ucSoftTfMode = cfg->ucSoftTfMode;
    }

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) && defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG)
    //mrsLogSetLowRatioThreshold(cfg->usLowRatio);//optimized by weichao
#endif

    mrsCcoSetTestModeCtrlInfo(((cfg->ucTestModeEnable) ? HI_TRUE : HI_FALSE), cfg->ucTestFrameTimeout);

    pstCtx->ucFlashSaveDelay = cfg->ucFlashSaveDelay;

    pstCtx->stReadMeterModel.ucAfn06F2UpRetryMax = cfg->ucAfn06F2UpRetryMax;
    pstCtx->stReadMeterModel.usAfn06F2UpTimeOut = cfg->usAfn06F2UpTimeOut;
    pstCtx->stReadMeterModel.ucAfn14F1UpRetryMax = cfg->ucAfn14F1UpRetryMax;
    pstCtx->stReadMeterModel.usAfn14F1UpTimeOut = cfg->usAfn14F1UpTimeOut;

    return;
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
    HI_BOOL bCltIDelay = HI_FALSE;

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
            bCltIDelay = mrsRmBcTimeoutCltIDelayFlag();
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
            bCltIDelay = ((stTopoEntry.product_type == HI_ND_TYPE_CLTI) ? HI_TRUE : HI_FALSE);
            meter_time = (HI_U32)(inf->MeterTimeOutNv
                                    + inf->IsCtrlCmdFlg * inf->CtrlCmdDelayNv
                                    + bCltIDelay * inf->CltIDelayNv);
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
        *m_time = (HI_U32)(inf->MeterTimeOutNv
                            + inf->IsCtrlCmdFlg * inf->CtrlCmdDelayNv
                            + MRS_RM_CLTI_DELAY_DEFAULT * bCltIDelay); // �㲥��ַĬ�ϲ���I�ɼ�����ʱ
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
 HI_U32 mrsSyncMeterInfToWhiteList(HI_MAC_WHIST_LIST_CHANGE_REASON_ENUM enReason)
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

    mrsResumeDetectRmSendMsg();

    HI_MDM_SetTfNtbSendProcessCtrl(HI_MDM_NTB_SEND_PROCESS_RESUME, HI_MDM_NTB_SEND_CTRL_REASON_READ_METER);
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
                sizeof(MR_NODE_METER_ADD_STRU),0, sizeof(MR_NODE_METER_ADD_STRU));
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
    else if ((pstCcoCtx->status & MRS_CCO_STATUS_NORMAL) && (mrsCcoIsTestMode() || pstCcoCtx->PlcChStatus))
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

// ��� վ���Ƿ������˱���
HI_BOOL mrsCheckStaIsInTopoInf(HI_U8 aucMac[HI_PLC_MAC_ADDR_LEN])
{
    MRS_CCO_SRV_CTX_STRU *pstCcoCtx = mrsCcoGetContext();
    HI_U32 ulIdx = 0;

    if (!pstCcoCtx || !aucMac)
    {
        return HI_FALSE;
    }

    for (ulIdx = 0; ulIdx < pstCcoCtx->stTopoInf.num; ulIdx++)
    {
        if (mrsToolsMemEq(pstCcoCtx->stTopoInf.entry[ulIdx].mac, aucMac, HI_PLC_MAC_ADDR_LEN))
        {
            return HI_TRUE;
        }
    }

    return HI_FALSE;
}


// ���� ���ڵ��ַ�������
HI_VOID mrsCcoCheckMainAddrStart(MRS_CCO_SRV_CTX_STRU * pstCcoCtx)
{
    if (!pstCcoCtx)
    {
        return;
    }

    if (mrsCcoGetSoftTfMode() == SOFT_TF_ON_WITH_STA_JOIN_BY_TF_RESULT && pstCcoCtx->stAutoSearchModel.stSearchCfg.ucCheckMainNodeAddr)
    {
        pstCcoCtx->stAutoSearchModel.bCheckingMainAddr = HI_TRUE;
        MRS_StartTimer(MRS_CCO_TIMER_CHECK_MAIN_NODE_ADDR,
            MRS_SEC_TO_MS(pstCcoCtx->stAutoSearchModel.stSearchCfg.usCheckMainNodeTimeout), HI_SYS_TIMER_ONESHOT);
    }
    else
    {
        pstCcoCtx->stAutoSearchModel.bCheckingMainAddr = HI_FALSE;
        HI_MDM_SetUseLastTfStatus(HI_TRUE);
    }
}


// ���� ���ڵ��ַ�ı�
HI_VOID mrsCcoHandleMainAddrChange(MRS_CCO_SRV_CTX_STRU * pstCcoCtx)
{
    if (!pstCcoCtx)
    {
        return;
    }

    pstCcoCtx->stAutoSearchModel.bCheckingMainAddr = HI_FALSE;
    MRS_StopTimer(MRS_CCO_TIMER_CHECK_MAIN_NODE_ADDR);
    HI_MDM_SetUseLastTfStatus(HI_FALSE);
}


// ���� ���ڵ��ַ��鳬ʱ
HI_VOID mrsCcoHandleCheckMainAddrTimeout(MRS_CCO_SRV_CTX_STRU * pstCcoCtx)
{
    if (!pstCcoCtx)
    {
        return;
    }

    pstCcoCtx->stAutoSearchModel.bCheckingMainAddr = HI_FALSE;
    HI_MDM_SetUseLastTfStatus(HI_TRUE);
}

HI_U8 mrsCcoGetStaPhase(HI_U8 *pMac)
{
    HI_MAC_NETWORK_TOPO_ENTRY_S stTopoEntry;
    HI_U8 ucPhase = 0;
    HI_U32 ret = HI_ERR_SUCCESS;

    (hi_void)memset_s(&stTopoEntry, sizeof(stTopoEntry), 0, sizeof(stTopoEntry));

    ret = HI_MDM_QueryMacAttr(pMac, &stTopoEntry);
    if ((HI_ERR_SUCCESS == ret) && ((stTopoEntry.phase_result >= PHASE_A) && (stTopoEntry.phase_result <= PHASE_C)))
    {
        ucPhase = stTopoEntry.phase_result;
    }

    HI_DIAG_LOG_MSG_E2(__LINE__, HI_DIAG_MT("phase ret-phase"),ret, ucPhase);
    return ucPhase;
}

HI_U8* mrsCcoGetSrcMac(HI_VOID)
{
    return mrsCcoGetContext()->aucSrcMac;
}

HI_VOID mrsCcoSetCtxPhase(HI_U8 ucPhase)
{
    mrsCcoGetContext()->ucPhase = ucPhase;
}

HI_S16 mrsSrvDutGetNtbAdjustBase(HI_VOID)
{
    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();
    HI_S16 sNtbAdjustBase = 0;

    if (HI_NULL != pstCcoCtx)
    {
        sNtbAdjustBase = pstCcoCtx->sDutNtbAjustBase;
    }

    return sNtbAdjustBase;
}

HI_U16 mrsCcoGetTeiByMac(HI_U8 *pMac)
{
    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();
    HI_U32 index = 0;

    if (!pMac)
    {
        return 0;
    }

    for (index = 0; index < HI_MAC_NETWORK_TOPO_ENTRY_MAX; index++)
    {
        if (mrsToolsMemEq(pMac, pstCcoCtx->stTopoInf.entry[index].mac, HI_PLC_MAC_ADDR_LEN))
        {
            return pstCcoCtx->stTopoInf.entry[index].tei;
        }
    }

    return 0;
}

HI_U32 mrsCcoGetMacByTei(HI_U16 usTei, HI_U8 *pucMac)
{
    MRS_CCO_SRV_CTX_STRU *pstCcoCtx = mrsCcoGetContext();
    HI_U32 index = 0;

    if (!pucMac) {
        return HI_ERR_FAILURE;
    }

    for (index = 0; index < HI_MAC_NETWORK_TOPO_ENTRY_MAX; index++) {
        if (usTei == pstCcoCtx->stTopoInf.entry[index].tei) {
            memcpy_s(pucMac, HI_PLC_MAC_ADDR_LEN, pstCcoCtx->stTopoInf.entry[index].mac, HI_PLC_MAC_ADDR_LEN);
            return HI_ERR_SUCCESS;
        }
    }

    return HI_ERR_FAILURE;
}

// ��ȡ����̨��ʶ��ģʽ
HI_MDM_SOFT_TF_MODE_EN mrsCcoGetSoftTfMode(HI_VOID)
{
    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();

    return (HI_MDM_SOFT_TF_MODE_EN)pstCcoCtx->ucSoftTfMode;
}

/*
HI_U32 mrsCmdCCORecvGetMeterDataInfo(HI_IN HI_U16 id, HI_PVOID req_packet, HI_U16 packet_size)
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


    MRS_NOT_USED(packet_size);

    //������
    plc_data = (MRS_PLC_FRAME_DATA_STRU *)req_packet;

    if (!plc_data || !plc_data->pucPayload)
    {
        return HI_ERR_FAILURE;
    }

    mac = plc_data->ucMac;
    mrsToolsMemcpy_s(cco_ctx->aucSrcMac, HI_PLC_MAC_ADDR_LEN, mac, HI_PLC_MAC_ADDR_LEN);


    pPayload = plc_data->pucPayload;
    usPayloadLen = plc_data->usPayloadLen;

    // ά��ӳ���
    
    if (MRS_Proto645Dec(data_645, data_len,&frame) == HI_ERR_SUCCESS)
    {
        if (MRS_645_FRAME_CONTROL_DIR_RESPONSION_BIT == frame.stCtrl.ucDir)
        {
            HI_U8 ucPhase = 0;

            ucPhase = ((PHASE_C == cco_ctx->ucPhase) ? MRS_PHASE_C : cco_ctx->ucPhase);
            mrsRmMapProc(&frame, &cco_ctx->stMapModule, mac, ucPhase);
        }
        else
        {
            HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_022, HI_DIAG_MT("data 645 error dir."));
        }
        
        return HI_ERR_SUCCESS;
    }
	
    MRS_PLC_RF_GET_METER_INFO_UP_S *pstTransmit = (MRS_PLC_RF_GET_METER_INFO_UP_S *)pPayload;
    HI_U8 * data_buff = HI_NULL;

    if (pstTransmit->seq != cco_ctx->stReadWaterMeterModel.usRfPlcSeq)
    {
        HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_029, HI_DIAG_MT("GetMeterDataInfo seq err [%d][%d]"),
                        pstTransmit->seq,cco_ctx->stReadWaterMeterModel.usRfPlcSeq);
        return HI_ERR_FAILURE;
    }
	
    
    data_buff = (HI_U8*)pstTransmit + pstTransmit->stru_len;   // ��ȡת������
    data_len = pstTransmit->ucMeterInfoCount*25;   // ��ȡת�����ݳ���
	if(data_len>usPayloadLen)
	{
		HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_077, HI_DIAG_MT("GetMeterDataInfo data_len,usPayloadLen"),data_len,usPayloadLen);
	}
   // �ͷű��ݵ�Plc����֡����
	if(cco_ctx->stReadWaterMeterModel.pstRfPlcDataDl != HI_NULL)
    {
        if(cco_ctx->stReadWaterMeterModel.pstRfPlcDataDl->pucPayload != HI_NULL)
        {
            mrsToolsFree(cco_ctx->stReadWaterMeterModel.pstRfPlcDataDl->pucPayload);   // �ͷ�ת�����ݿ�   
            cco_ctx->stReadWaterMeterModel.pstRfPlcDataDl->pucPayload = HI_NULL;
        }
        
        mrsToolsFree(cco_ctx->stReadWaterMeterModel.pstRfPlcDataDl);   // �ͷ�PLC֡����    
        cco_ctx->stReadWaterMeterModel.pstRfPlcDataDl = HI_NULL;
    }
	MRS_StopTimer(MRS_CCO_TIMER_RF_READ_WATERMETER);
	cco_ctx->stReadWaterMeterModel.enCurrRfState=3;
	HI_U8 *pDataTemp=HI_NULL;
	do
	{
		if(data_len!=0)
		{
		 	// ȥ�ظ������ݡ����ϱ�������
			HI_U16 usMeterAllNum = mrsSrvArchivesMeterNum();
			MRS_ARCHIVES_NODE_STRU *pstNode = HI_NULL;
			HI_U32 i=0,j=0;
			HI_U16 usDataTempLen=0;
			pDataTemp = mrsToolsMalloc(data_len+2);
		    if (HI_NULL == pDataTemp)
		    {
		        break;
		    }
			mrsToolsMemZero_s(pDataTemp, data_len+2, data_len+2);
			pDataTemp[usDataTempLen++] = pstTransmit->seq;
			usDataTempLen++;// ˮ���ϱ��ĸ���
			//�ڴ˴��ҵ���ǰ�����Ϣ�е�һ����Ч�ı���Ϣ����ţ�
			for(j=0;j<pstTransmit->ucMeterInfoCount;j++)
			{
				for (i = 0; i < usMeterAllNum; i++)
				{
					pstNode = mrsSrvArchivesQuery(i);
					if (pstNode == HI_NULL)
					{
						break;
					}

					if ((pstNode->ucProtocol>=0x10)&& pstNode->bValid &&(pstNode->ucRfReportFlag==0)) // ���ҵ�һ����Ч�ı���Ϣ
					{
						
						HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1008, HI_DIAG_MT("[RF]ˮ���ַ"), pstNode->aucNodeAddr,6);
						if(mrsToolsMemcmp(data_buff+j*25,pstNode->aucNodeAddr,6)==0)
						{
							pstNode->ucRfReportFlag=1;
							mrsToolsMemcpy_s(pDataTemp+usDataTempLen, 25, data_buff+j*25, 25);
							usDataTempLen+=25;
							pDataTemp[1] += 1;
							break;
						}
					}
				}
			}
			HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_078, HI_DIAG_MT("ȥ���ظ������ݳ���"),usDataTempLen);
			if(usDataTempLen <= 2)
			{
				break;
			}
			ret =mrs3762_ReportAFN20_F3(pDataTemp,usDataTempLen,mrsWaterMeterModelRxRespProc,mrsWaterMeterModelAllTimeOutProc);
			if(ret!=HI_ERR_SUCCESS)
			{
				break;
			}
			mrsToolsFree(pDataTemp);
			return HI_ERR_SUCCESS;
		}
	}while(0);
	mrsToolsFree(pDataTemp);
	MRS_StartTimer(MRS_CCO_TIMER_RF_READ_WATERMETER, 1000, HI_SYS_TIMER_ONESHOT);
	return HI_ERR_SUCCESS;
}
*/
HI_U32 mrsSearchWaterMeterInit(HI_VOID)
{
	MRS_CCO_SRV_CTX_STRU * cco_ctx = mrsCcoGetContext();
	mrsToolsMemZero_s(&(cco_ctx->stWaterMeterSearchModel),sizeof(MR_WATER_METER_SEARCH_STRU),sizeof(MR_WATER_METER_SEARCH_STRU));
	mrsSrvInitList(&(cco_ctx->stWaterMeterSearchModel.stGetWaterMeterList));
	cco_ctx->stWaterMeterSearchModel.stSearchWaterFrame.retry_max = 2;
	
	return HI_ERR_SUCCESS;
}
/*
HI_U32 mrsCmdCCORecvGetMeterAddressInfo(HI_IN HI_U16 id, HI_PVOID req_packet, HI_U16 packet_size)
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

	HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_031, HI_DIAG_MT("CCORecvGetMeterAddressInfo"));

    MRS_NOT_USED(packet_size);

    //������
    plc_data = (MRS_PLC_FRAME_DATA_STRU *)req_packet;

    if (!plc_data || !plc_data->pucPayload)
    {
        return HI_ERR_FAILURE;
    }

    mac = plc_data->ucMac;
    mrsToolsMemcpy_s(cco_ctx->aucSrcMac, HI_PLC_MAC_ADDR_LEN, mac, HI_PLC_MAC_ADDR_LEN);


    pPayload = plc_data->pucPayload;
    usPayloadLen = plc_data->usPayloadLen;

    MRS_PLC_RF_GET_ADDRESS_INFO_UP_S *pstTransmit = (MRS_PLC_RF_GET_ADDRESS_INFO_UP_S *)pPayload;
    HI_U8 * data_buff = HI_NULL;

    if (pstTransmit->seq != cco_ctx->ucSeq)
    {
        HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_030, HI_DIAG_MT("GetMeterDataInfo seq err [%d][%d]"),
                        pstTransmit->seq,cco_ctx->ucSeq);
        return HI_ERR_FAILURE;
    }
	
    
    data_buff = ((HI_U8*)pstTransmit) + pstTransmit->stru_len;   // ��ȡת������
    data_len = pstTransmit->ucMeterCount*8;   // ��ȡת�����ݳ���
    if(data_len>usPayloadLen)
    {
    	 HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_079, HI_DIAG_MT("GetMeterDataInfo,DataLen,usPayloadLen"),
                        data_len,usPayloadLen);
    }

   // �ͷű��ݵ�Plc����֡����
	if(cco_ctx->stWaterMeterSearchModel.stSearchWaterFrame.plc!= HI_NULL)
    {
        if(cco_ctx->stWaterMeterSearchModel.stSearchWaterFrame.plc->pucPayload != HI_NULL)
        {
            mrsToolsFree(cco_ctx->stWaterMeterSearchModel.stSearchWaterFrame.plc->pucPayload);   // �ͷ�ת�����ݿ�   
            cco_ctx->stWaterMeterSearchModel.stSearchWaterFrame.plc->pucPayload = HI_NULL;
        }
        
        mrsToolsFree(cco_ctx->stWaterMeterSearchModel.stSearchWaterFrame.plc);   // �ͷ�PLC֡����    
        cco_ctx->stWaterMeterSearchModel.stSearchWaterFrame.plc = HI_NULL;
    }
	if(data_len>0)
	{
		HI_U16 usAddrDataLen = sizeof(MRS_SEARCH_WATER_METER_LIST)+data_len;
		MRS_SEARCH_WATER_METER_LIST *node = (MRS_SEARCH_WATER_METER_LIST*)mrsToolsMalloc(usAddrDataLen);
		if(!node)
		{
			return HI_ERR_MALLOC_FAILUE;
		}
		mrsToolsMemZero_s(node,usAddrDataLen,usAddrDataLen);
		mrsToolsMemcpy_s(node->ucaStaMac, 6,pstTransmit->ucSrcMeter, 6);
		node->ucMeterNum = pstTransmit->ucMeterCount;
		mrsToolsMemcpy_s(node->pMeterData, data_len,data_buff, data_len);
		mrsSrvListInsTail(&(cco_ctx->stWaterMeterSearchModel.stGetWaterMeterList),&(node->link));
		HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_032, HI_DIAG_MT("CCORecvGetMeterAddressInfo Add Node"),node->ucMeterNum);
	}
	MRS_StopTimer(MRS_CCO_TIMER_GET_WATER_METERLIST);
	cco_ctx->stWaterMeterSearchModel.usCurrNum++;
	cco_ctx->stWaterMeterSearchModel.stSearchWaterFrame.send_retry=0;
	// ������һ��
	mrsCcoSearchMeterGetWaterMeterlist(cco_ctx,HI_TRUE);
}
*/
HI_U32 mrsCcoSendStartFindOwner(HI_VOID)
{
	HI_U32 ret=HI_ERR_SUCCESS;
	HI_U16 usMaxTei=1;
	HI_U16 usMeterCount=0,usWaterMeterCount=0,usBothWaterMeterCount=0;
	HI_U16 usOffent=0;
	HI_U32 i=0;
	MRS_CCO_SRV_CTX_STRU *pCtx = mrsCcoGetContext();
	HI_U16 usMeterAllNum = mrsSrvArchivesMeterNum();
	for(i=0;i<pCtx->stTopoInf.num;i++)
	{
		if(pCtx->stTopoInf.entry[i].tei >usMaxTei)
		{
			usMaxTei = pCtx->stTopoInf.entry[i].tei;
		}
	}
	
	HI_U16 BitMapLen=(usMaxTei-1)/8+1;
    MRS_ARCHIVES_NODE_STRU *pstNode = HI_NULL;
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
            usWaterMeterCount++;
            if(pstNode->ucProtocol > METER_PROTO_SINGLEWAY_WATERMETER)
            {
              usBothWaterMeterCount++;
            }
        }
		else
		{
			usMeterCount++;
		}
    }
	if(0==usBothWaterMeterCount)
	{
		HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_094, HI_DIAG_MT("[RF]usBothWaterMeterCount==0"));
		return HI_ERR_SUCCESS;
	}

		//CJJ-HPLC-20181204:����ˮ������״̬Ϊ��������
		setWaterMeterNetStatue(WATER_METER_START_NET);

	
    MRS_PLC_FRAME_DATA_STRU plc;
    MRS_PLC_METERLIST_DL_S * meterlist = HI_NULL;
    HI_U8 * payload = HI_NULL;
    HI_U16 payload_len = sizeof(MRS_PLC_METERLIST_DL_S)+9+BitMapLen;
    HI_U8 aucIp[HI_IPV4_ADDR_LEN];
	
    payload = mrsToolsMalloc(payload_len);
    if (HI_NULL == payload)
    {
        return HI_ERR_MALLOC_FAILUE;
    }
	pCtx->stReadWaterMeterModel.usRfPlcSeq++;
    mrsToolsMemZero_s(payload, payload_len, payload_len);
    meterlist = (MRS_PLC_METERLIST_DL_S *)payload;
    meterlist->interface_ver = MRS_GET_METERLIST_DL_VER;
    meterlist->stru_len = sizeof(MRS_PLC_METERLIST_DL_S);
    meterlist->option = MRS_SM_WATER_CMD;
    meterlist->seq = pCtx->stReadWaterMeterModel.usRfPlcSeq;
    meterlist->force_resp = 0;
    
    (HI_VOID)HI_MDM_NM_GetAddr(meterlist->src, aucIp);
    mrsToolsMemset_s(meterlist->dst, HI_PLC_MAC_ADDR_LEN, 0xFF, HI_PLC_MAC_ADDR_LEN);
	
	meterlist->waterMeter_Data[usOffent++]=1;
	meterlist->waterMeter_Data[usOffent++] = pCtx->stReadWaterMeterModel.usRfPlcSeq&0xFF;

	meterlist->waterMeter_Data[usOffent++]=usMaxTei&0xFF;
	meterlist->waterMeter_Data[usOffent++]=(usMaxTei>>8)&0xFF;
	
	meterlist->waterMeter_Data[usOffent++]=usWaterMeterCount&0xFF;
	meterlist->waterMeter_Data[usOffent++]=(usWaterMeterCount>>8)&0xFF;

	meterlist->waterMeter_Data[usOffent++]=0;

	meterlist->waterMeter_Data[usOffent++]=BitMapLen&0xFF;
	meterlist->waterMeter_Data[usOffent++]=(BitMapLen>>8)&0xFF;

	for(i=0;i<pCtx->stTopoInf.num;i++)
	{
		mrsSetBitMap(meterlist->waterMeter_Data+usOffent,pCtx->stTopoInf.entry[i].tei-1);
	}
	
    mrsToolsMemZero_s(&plc, sizeof(plc), sizeof(plc));
    plc.usId = ID_MRS_CMD_GET_METERLIST;
    plc.pucPayload = payload;
    plc.usPayloadLen = payload_len;
    mrsToolsMemset_s(plc.ucMac, HI_PLC_MAC_ADDR_LEN, 0xFF, HI_PLC_MAC_ADDR_LEN);
    HI_U32 waterBoardTimer = 2500 * usMaxTei + 300 * usWaterMeterCount + 20000;
    ret =mrsCcoSendTimeSlotPlcFrame((HI_U8 *)&plc,waterBoardTimer);
    return ret;
}


//Matteo
HI_U32 mrsCcoFindTypeTimeoutHandle( MRS_CCO_SRV_CTX_STRU  * cco)
{
//	HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_098, HI_DIAG_MT("[Matteo] do mrsCcoFindTypeTimeoutHandle"));

	HI_U32 i = 0;
	HI_U32 ret = HI_ERR_NOT_FOUND;
	for(i=0;i<cco->usIIArchive->num;i++)
	{
		if (cco->usIIArchive->IICltmac[i].query != 1 && cco->usIIArchive->IICltmac[i].result < 5)
        {
        	cco->usIIArchive->IICltmac[i].result++;
			mrsCcoSendQueryDeviceTypeCmd(cco,cco->usIIArchive->IICltmac[i].mac);
        }
	}
	return ret;

}



//���յ�ַ����II���豸����  Matteo
HI_U32 mrsCcoSaveIICltDeviceType( MRS_CCO_SRV_CTX_STRU  * cco,HI_PBYTE pMeterAddr,HI_U8 Type)
{
//	HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_004, HI_DIAG_MT("[Matteo] do mrsCcoSaveIICltDeviceType"));

	HI_U32 i = 0;
	HI_U32 ret = HI_ERR_NOT_FOUND;
	for(i=0;i<cco->usIIArchive->num;i++)
	{
		if (mrsToolsMemEq(cco->usIIArchive->IICltmac[i].mac, pMeterAddr, HI_METER_ADDR_LEN))
        {
            cco->usIIArchive->IICltmac[i].query = 1;
			cco->usIIArchive->IICltmac[i].type = Type;
			return HI_ERR_SUCCESS;
        }
	}
	return ret;

}


//��ѯ�Ƿ�Ϊ����II��  Matteo
HI_U32 mrsCcoFindNewIIClt( MRS_CCO_SRV_CTX_STRU  * cco,HI_PBYTE pMeterAddr)
{
//	HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_005, HI_DIAG_MT("[Matteo] do mrsCcoFindNewIIClt"));
	HI_U32 i = 0;

	for(i=0;i<cco->usIIArchive->num;i++)
	{
//		HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1022, HI_DIAG_MT("[Matteo]usIIArchive.IICltmac[i].mac"), cco->usIIArchive->IICltmac[i].mac, HI_METER_ADDR_LEN);
		
//		HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1018, HI_DIAG_MT("[Matteo]pMeterAddr"), pMeterAddr, HI_METER_ADDR_LEN);
		if (!(mrsToolsMemEq(cco->usIIArchive->IICltmac[i].mac, pMeterAddr, HI_METER_ADDR_LEN)))
        {
			continue;
        }
		else
		{
			if(cco->usIIArchive->current < cco->usIIArchive->num && cco->usIIArchive->current !=0)
			{
				cco->usIIArchive->IICltmac[i].valid = HI_TRUE;
			}
			else
			{
				cco->usIIArchive->IICltmac[i].valid = HI_FALSE;
			}
			HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_028, HI_DIAG_MT("[Matteo] OLD II"));
			return i+1;
		}
		
	}
	return HI_ERR_SUCCESS;

}

//����ǰ��
HI_VOID mrsCcoDataUpdata(MRS_CCO_SRV_CTX_STRU  * cco,HI_U32 start,HI_U32 num)
{
	for (; start < num; start++)
	{	
		(hi_void)memcpy_s(cco->usIIArchive->IICltmac[start].mac, HI_PLC_MAC_ADDR_LEN,cco->usIIArchive->IICltmac[start+1].mac,HI_PLC_MAC_ADDR_LEN);
		cco->usIIArchive->IICltmac[start].query = cco->usIIArchive->IICltmac[start+1].query;
		cco->usIIArchive->IICltmac[start].valid= cco->usIIArchive->IICltmac[start+1].valid;
		cco->usIIArchive->IICltmac[start].type= cco->usIIArchive->IICltmac[start+1].type;
		cco->usIIArchive->IICltmac[start].result= cco->usIIArchive->IICltmac[start+1].result;
	}
	return; 
}

//��ѯ�豸���� Matteo
HI_U32 mrsCcoSendStartQueryDeviceType(MRS_CCO_SRV_CTX_STRU  * cco)
{

//	HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_008, HI_DIAG_MT("[Matteo] do mrsCcoSendStartQueryDeviceType"));
  	HI_U32 ret = HI_ERR_SUCCESS;
	HI_U32 ulIndex = 0;

    HI_ASSERT(HI_NULL != cco);
	mrsRefreshTopoInfo(cco);
	

	// �����е�1���ڵ���CCO������
	for (ulIndex = 1; ulIndex < cco->stTopoInf.num; ulIndex++)
	{
		if (HI_ND_TYPE_CLTII == cco->stTopoInf.entry[ulIndex].product_type)
		{		
			mrsCcoSendQueryDeviceTypeCmd(cco,cco->stTopoInf.entry[ulIndex].mac);
			ret++;
		}
	}
	cco->usIIArchive->current=ret;
	if(cco->usIIArchive->current < cco->usIIArchive->num)//��������ˢ�±����II��
	{
		for (ulIndex = 1; ulIndex < cco->stTopoInf.num; ulIndex++)
		{
			if (HI_ND_TYPE_CLTII == cco->stTopoInf.entry[ulIndex].product_type)
			{		
				mrsCcoSendQueryDeviceTypeCmd(cco,cco->stTopoInf.entry[ulIndex].mac);
			}
		}
		for (ulIndex = 0; ulIndex < cco->usIIArchive->num; ulIndex++)
		{
			if (cco->usIIArchive->IICltmac[ulIndex].valid == HI_FALSE)
			{
			 	mrsCcoDataUpdata(cco,ulIndex--,(cco->usIIArchive->num--)-1);
			}
		}
	}
    return ret;
}

//Matteo
HI_U32 mrsCcoSendQueryDeviceTypeCmd(MRS_CCO_SRV_CTX_STRU  * cco,HI_U8 *pMeterAddr)
{

//	HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_099, HI_DIAG_MT("[Matteo] do mrsCcoSendQueryDeviceTypeCmd"));

  	HI_U32 ret = HI_ERR_SUCCESS;
    MRS_PLC_FRAME_DATA_STRU plc;
    MRS_PLC_METERLIST_DL_S * meterlist = HI_NULL;
    HI_U8 * payload = HI_NULL;
    HI_U16 payload_len = sizeof(MRS_PLC_METERLIST_DL_S) +1;
    HI_U8 aucIp[HI_IPV4_ADDR_LEN];
	
	payload = mrsToolsMalloc(payload_len);
    if (HI_NULL == payload)
    {
        return HI_ERR_MALLOC_FAILUE;
    }

    (hi_void)memset_s(payload, payload_len, 0, payload_len);
    meterlist = (MRS_PLC_METERLIST_DL_S *)payload;
    meterlist->interface_ver = MRS_GET_METERLIST_DL_VER;
    meterlist->stru_len = sizeof(MRS_PLC_METERLIST_DL_S);
    meterlist->option = MRS_SM_WATER_CMD;
    meterlist->seq = cco->stAutoSearchModel.stSearchCfg.seq++;
    meterlist->force_resp = 1;

    (HI_VOID)HI_MDM_NM_GetAddr(meterlist->src, aucIp);
    (hi_void)memcpy_s(meterlist->dst, HI_PLC_MAC_ADDR_LEN, pMeterAddr,HI_PLC_MAC_ADDR_LEN);
	
	//����II�ɵ�ַ����
	ret = mrsCcoFindNewIIClt(cco,pMeterAddr);
	if( ret == HI_ERR_SUCCESS)
	{
    	(hi_void)memcpy_s(cco->usIIArchive->IICltmac[cco->usIIArchive->num++].mac, HI_PLC_MAC_ADDR_LEN, pMeterAddr, HI_PLC_MAC_ADDR_LEN);
		//if(cco->usIIArchive->IICltmac[cco->usIIArchive->num -1].result >= 5 || cco->usIIArchive->IICltmac[cco->usIIArchive->num -1].query == 1)
		//{
		//	mrsToolsFree(payload);
		//	return HI_ERR_SUCCESS;
		//}
		cco->usIIArchive->IICltmac[cco->usIIArchive->num-1].query = 0;
		cco->usIIArchive->IICltmac[cco->usIIArchive->num-1].valid= 0;
		cco->usIIArchive->IICltmac[cco->usIIArchive->num-1].type= 0;
		cco->usIIArchive->IICltmac[cco->usIIArchive->num-1].result= 0;
		
			
	}
	else
	{
		if(cco->usIIArchive->IICltmac[ret-1].result >= 5 || cco->usIIArchive->IICltmac[ret-1].query ==1)
		{
			mrsToolsFree(payload);
			return HI_ERR_SUCCESS;
		}
	}
	meterlist->waterMeter_Data[0] = 4;

	
    (hi_void)memset_s(&plc, sizeof(plc), 0, sizeof(plc));
    plc.usId = ID_MRS_CMD_GET_METERLIST;
    plc.pucPayload = payload;
    plc.usPayloadLen = payload_len;
   (hi_void)memcpy_s(plc.ucMac, HI_PLC_MAC_ADDR_LEN, pMeterAddr, HI_PLC_MAC_ADDR_LEN);
	HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1020, HI_DIAG_MT("[Matteo]down query frame"), plc.pucPayload, plc.usPayloadLen);

    ret = MRS_SendPlcFrame(&plc);
	
	mrsToolsFree(payload);
    return ret;
}




HI_U32 mrsCcoCreateSetWaterMeterShip(MRS_CCO_SRV_CTX_STRU  * cco)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    MRS_PLC_FRAME_DATA_STRU *plc=HI_NULL;
    MRS_DATA_TRANSMIT_AFN13_DL_STRU * meterlist = HI_NULL;
    HI_U8 * payload = HI_NULL;
    HI_U16 payload_len = 0;
    //HI_U8 aucIp[HI_IPV4_ADDR_LEN];
    MRS_SRV_LIST * list;
	MRS_SEARCH_WATER_METER_LIST *node=HI_NULL;
	HI_U8 i=0;//,j=0;
	HI_SYS_CALENDAR_TIME_S stime;
	HI_U16 usOffent=0;
	HI_U8 ucData[32]={0};
	//HI_U8 ucMeterMac[7]={0};
	
    list = &(cco->stWaterMeterSearchModel.stGetWaterMeterList);
	node = (MRS_SEARCH_WATER_METER_LIST *)list->next;
	
	while(HI_TRUE)
	{
		if(mrsSrvIsListEmpty(list))
		{
			//CJJ-HPLC-20181204:ˮ���������
			setWaterMeterNetStatue(WATER_METER_COMPLETE_NET);
		
			HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_048, HI_DIAG_MT("[RF]CreateSetWaterMeterShip list Empty"));
			return HI_ERR_INVALID_PARAMETER;
		}
		node = (MRS_SEARCH_WATER_METER_LIST *)list->next;
		HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1003, HI_DIAG_MT("node->ucaStaMac"), node->ucaStaMac, 6);
		
		HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_049, HI_DIAG_MT("[RF]CreateSetWaterMeterShip"),node->ucMeterNum);
		if((node->ucMeterNum ==0)||(node->ucMeterNum <=node->ucSendIndex))
		{
			mrsSrvListRemove(list,&(node->link));
			mrsToolsFree(node);
		}
		else
		{
			break;
		}
	}
	// ��֡
	//HI_U16 ucDataLen=0;
	HI_U16 ucDauDataLen=0;
	HI_U16 noffent=0;
	HI_U16 usStaTei=0;
	HI_U8 *pDauData=HI_NULL;

	for(i=0;i<cco->stTopoInf.num;i++)
	{
		if(0 ==mrsToolsMemcmp(cco->stTopoInf.entry[i].mac,node->ucaStaMac,6))
		{
			usStaTei = cco->stTopoInf.entry[i].tei;
			break;
		}
	}
	noffent=0;
	ucData[noffent++]=(cco->ulCcoNetSnid%32+1);
	mrsToolsMemcpy_s(ucData+noffent,6,node->ucaStaMac,6);
	mrsHexInvert(ucData+noffent,6);
	noffent+=6;
	ucData[noffent++]=(usStaTei&0xFF);
	ucData[noffent++]=((usStaTei>>8) &0xFF);

	ucData[noffent++]=1; // ʱ϶
	ucData[noffent++]=0;

	HI_MDM_GetCalendarTime(&stime);
	// ��������Ϊ6���ֽڵ�ʱ�� + 8���ֽ�*������
    mrsIntToBcd((HI_U32)stime.sec, ucData+noffent, sizeof(HI_U8));
	noffent++;
	mrsIntToBcd((HI_U32)stime.min, ucData+noffent, sizeof(HI_U8));
	noffent++;
	mrsIntToBcd((HI_U32)stime.hour, ucData+noffent, sizeof(HI_U8));
	noffent++;
	mrsIntToBcd((HI_U32)stime.day, ucData+noffent, sizeof(HI_U8));
	noffent++;
	mrsIntToBcd((HI_U32)stime.month, ucData+noffent, sizeof(HI_U8));
	noffent++;
	mrsIntToBcd((HI_U32)stime.year, ucData+noffent, sizeof(HI_U8));
	noffent++;
	if(ucData[noffent-1]>0x60)
	{
		ucData[noffent++]=0x19;
	}
	else
	{
		ucData[noffent++]=0x20;
	}
		
	ucData[noffent++]=1; // ����ģʽ
		
	ucData[noffent++]=0xFF; // ����ʱ��
	ucData[noffent++]=0xFF;
	ucData[noffent++]=0xFF;
	
	ret=mrsCCORFDauEncodeFrame(&pDauData,&ucDauDataLen,ucData,noffent,node->ucaStaMac,node->pMeterData+node->ucSendIndex*7,0x44,0);
	if(ret !=HI_ERR_SUCCESS)
	{
		return ret;
	}
	node->ucSendIndex++;

	payload_len = sizeof(MRS_DATA_TRANSMIT_AFN13_DL_STRU)+1+ucDauDataLen;

    payload = mrsToolsMalloc(payload_len);
    if (HI_NULL == payload)
    {
    	mrsToolsFree(pDauData);
    	HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_070, HI_DIAG_MT("[RF]CreateSetWaterMeterShip payload_len"),payload_len);
        return HI_ERR_MALLOC_FAILUE;
    }
	plc = mrsToolsMalloc(sizeof(MRS_PLC_FRAME_DATA_STRU));
	if (HI_NULL == plc)
    {
    	HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_071, HI_DIAG_MT("[RF]CreateSetWaterMeterShip len"),sizeof(MRS_PLC_FRAME_DATA_STRU));
    	mrsToolsFree(payload);
		mrsToolsFree(pDauData);
        return HI_ERR_MALLOC_FAILUE;
    }
	if(cco->stWaterMeterSearchModel.stSearchWaterFrame.plc!= HI_NULL)
    {
        if(cco->stWaterMeterSearchModel.stSearchWaterFrame.plc->pucPayload != HI_NULL)
        {
            mrsToolsFree(cco->stWaterMeterSearchModel.stSearchWaterFrame.plc->pucPayload);   // �ͷ�ת�����ݿ�   
            cco->stWaterMeterSearchModel.stSearchWaterFrame.plc->pucPayload = HI_NULL;
        }
        
        mrsToolsFree(cco->stWaterMeterSearchModel.stSearchWaterFrame.plc);   // �ͷ�PLC֡����    
        cco->stWaterMeterSearchModel.stSearchWaterFrame.plc = HI_NULL;
    }
    mrsToolsMemZero_s(payload, payload_len, payload_len);
    meterlist = (MRS_DATA_TRANSMIT_AFN13_DL_STRU *)payload;
	cco->stWaterMeterSearchModel.usWaterSearchSeq++;
	// ��֯ת�����ݰ�
    meterlist->interface_ver = MRS_AFN13_DL_VER;
    meterlist->stru_size     = sizeof(MRS_DATA_TRANSMIT_AFN13_DL_STRU);
    meterlist->protocol      = WIRELESS_TRANSMISSION_PROTOCOL;        // ��ȡ��Լ����
    meterlist->data_len      = ucDauDataLen+1;        // ��ȡ��ת�����ĳ���    
    meterlist->seq           = cco->stWaterMeterSearchModel.usWaterSearchSeq;
    meterlist->time_out      = (HI_U8)MRS_MS_TO_100MS(1000); // ��λΪ�ٺ���
    meterlist->dir           = MRS_PLC_DL_FLG;  // ������֡��־    
	usOffent=0;

	meterlist->data[usOffent++]=METER_PROTO_BOTHWAY_WATERMETER;

	mrsToolsMemcpy_s(meterlist->data+usOffent, ucDauDataLen, pDauData, ucDauDataLen);
	usOffent+=ucDauDataLen;
    mrsToolsMemZero_s(plc, sizeof(plc), sizeof(plc));
    plc->usId = ID_MRS_CMD_DATA_TRANSMIT_AFN13_PLC;
    plc->pucPayload = payload;
    plc->usPayloadLen = payload_len;
    mrsToolsMemcpy_s(plc->ucMac, HI_PLC_MAC_ADDR_LEN, node->ucaStaMac, HI_PLC_MAC_ADDR_LEN);
	cco->stWaterMeterSearchModel.stSearchWaterFrame.plc = plc;
	cco->stWaterMeterSearchModel.stSearchWaterFrame.send_retry=0;
	cco->stWaterMeterSearchModel.ucSetOwnerShipMeterNum = 1;//node->ucMeterNum;
	HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_067, HI_DIAG_MT("[RF]CreateSetWaterMeterShip end"));
	return HI_ERR_SUCCESS;
}

HI_U32 mrsCcoCreateGetWaterMeterList(MRS_CCO_SRV_CTX_STRU  * cco, HI_U8 mac[6])
{
    //HI_U32 ret = HI_ERR_SUCCESS;
    MRS_PLC_FRAME_DATA_STRU *plc=HI_NULL;
    MRS_PLC_METERLIST_DL_S * meterlist = HI_NULL;
    HI_U8 * payload = HI_NULL;
    HI_U16 payload_len = sizeof(MRS_PLC_METERLIST_DL_S)+4;
    HI_U8 aucIp[HI_IPV4_ADDR_LEN];
	HI_U16 usOffent=0;
	//Matteo
	HI_U32 i = 0;
	for(i=0;i<cco->usIIArchive->num;i++)
	{
		if (mrsToolsMemEq(cco->usIIArchive->IICltmac[i].mac, mac, HI_METER_ADDR_LEN))
        {
        	
            if(cco->usIIArchive->IICltmac[i].query == 1)
            {
				HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1021, HI_DIAG_MT("[Matteo]broadcast cmd"), mac, HI_METER_ADDR_LEN);
				break;
				
			}
			else
			{
				return HI_ERR_SUCCESS;
			}
        }
	}
	HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1003, HI_DIAG_MT("[Matteo]WATERGETSSICMD ADDR"), mac, HI_METER_ADDR_LEN);

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
	if(cco->stWaterMeterSearchModel.stSearchWaterFrame.plc!= HI_NULL)
    {
        if(cco->stWaterMeterSearchModel.stSearchWaterFrame.plc->pucPayload != HI_NULL)
        {
            mrsToolsFree(cco->stWaterMeterSearchModel.stSearchWaterFrame.plc->pucPayload);   // �ͷ�ת�����ݿ�   
            cco->stWaterMeterSearchModel.stSearchWaterFrame.plc->pucPayload = HI_NULL;
        }
        
        mrsToolsFree(cco->stWaterMeterSearchModel.stSearchWaterFrame.plc);   // �ͷ�PLC֡����    
        cco->stWaterMeterSearchModel.stSearchWaterFrame.plc = HI_NULL;
    }
	
	cco->stReadWaterMeterModel.usRfPlcSeq++;
    mrsToolsMemZero_s(payload, payload_len, payload_len);
    meterlist = (MRS_PLC_METERLIST_DL_S *)payload;
    meterlist->interface_ver = MRS_GET_METERLIST_DL_VER;
    meterlist->stru_len = sizeof(MRS_PLC_METERLIST_DL_S);
    meterlist->option = MRS_SM_WATER_CMD;
    meterlist->seq = cco->stWaterMeterSearchModel.usWaterSearchSeq;

   	meterlist->waterMeter_Data[usOffent++]=2;
	meterlist->waterMeter_Data[usOffent++]=0;
	meterlist->waterMeter_Data[usOffent++]=0;
	meterlist->waterMeter_Data[usOffent++]=48;
    (HI_VOID)HI_MDM_NM_GetAddr(meterlist->src, aucIp);
    mrsToolsMemcpy_s(meterlist->dst, HI_PLC_MAC_ADDR_LEN, mac, HI_PLC_MAC_ADDR_LEN);

    mrsToolsMemZero_s(plc, sizeof(plc), sizeof(plc));
    plc->usId = ID_MRS_CMD_GET_METERLIST;
    plc->pucPayload = payload;
    plc->usPayloadLen = payload_len;
    mrsToolsMemcpy_s(plc->ucMac, HI_PLC_MAC_ADDR_LEN, mac, HI_PLC_MAC_ADDR_LEN);
	cco->stWaterMeterSearchModel.stSearchWaterFrame.plc = plc;
	cco->stWaterMeterSearchModel.stSearchWaterFrame.send_retry=0;
	return HI_ERR_SUCCESS;
}
HI_U32  mrsCcoSendSetWaterMeterShip(MRS_CCO_SRV_CTX_STRU  * cco)
{
	HI_U32 ret = HI_ERR_SUCCESS;
	if(cco->stWaterMeterSearchModel.stSearchWaterFrame.plc)
	{	
		ret = MRS_SendPlcFrame(cco->stWaterMeterSearchModel.stSearchWaterFrame.plc);
		MRS_StartTimer(MRS_CCO_TIMER_SET_WATER_METERLIST_SHIP, 1000 * (10), HI_SYS_TIMER_ONESHOT);
		cco->stWaterMeterSearchModel.stSearchWaterFrame.send_retry++;
	}
	else
	{
		ret = HI_ERR_INVALID_PARAMETER;
	}
	return ret;
}

HI_U32  mrsCcoSendGetWaterMeter(MRS_CCO_SRV_CTX_STRU  * cco)
{
	HI_U32 ret = HI_ERR_SUCCESS;
	if(cco->stWaterMeterSearchModel.stSearchWaterFrame.plc)
	{	
		ret = MRS_SendPlcFrame(cco->stWaterMeterSearchModel.stSearchWaterFrame.plc);
		MRS_StartTimer(MRS_CCO_TIMER_GET_WATER_METERLIST, 1000 * 5, HI_SYS_TIMER_ONESHOT);
		cco->stWaterMeterSearchModel.stSearchWaterFrame.send_retry++;
	}
	else
	{
		ret = HI_ERR_INVALID_PARAMETER;
	}
	return ret;
}

HI_U32 mrsCcoSearchMeterGetWaterMeterlist(MRS_CCO_SRV_CTX_STRU *cco,HI_U8 ucFlag)
{
    HI_U32 ret = HI_ERR_SUCCESS;
	HI_U8 ucTempFlag=0;

    //HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_033, HI_DIAG_MT("[CCO-SM] GetWaterMeterlist"),ucFlag);

    do
    {
		if(ucFlag)
		{
			if(cco->stTopoInf.entry[cco->stWaterMeterSearchModel.usCurrNum].tei==1)
			{
				cco->stWaterMeterSearchModel.usCurrNum++;
			}
			ucTempFlag=ucFlag;
		}
		else
		{
			if(cco->stWaterMeterSearchModel.stSearchWaterFrame.send_retry >=cco->stWaterMeterSearchModel.stSearchWaterFrame.retry_max)
			{
				ucTempFlag = 1;
				cco->stWaterMeterSearchModel.stSearchWaterFrame.send_retry=0;
				HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_057, HI_DIAG_MT("[CCO-SM-Water] stSearchWaterFrame.send_retry"),cco->stWaterMeterSearchModel.stSearchWaterFrame.send_retry);
				cco->stWaterMeterSearchModel.usCurrNum++;
				if(cco->stTopoInf.entry[cco->stWaterMeterSearchModel.usCurrNum].tei==1)
				{
					cco->stWaterMeterSearchModel.usCurrNum++;
				}
			}
		}
    	HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_056, HI_DIAG_MT("[CCO-SM-Water] usCurrNum,Allnum"),cco->stWaterMeterSearchModel.usCurrNum,cco->stTopoInf.num);
		if(cco->stWaterMeterSearchModel.usCurrNum >= cco->stTopoInf.num)
		{
			cco->stWaterMeterSearchModel.bQueryComplete = HI_TRUE;
			cco->stWaterMeterSearchModel.bStartSetMeterShipFlag = HI_TRUE;
			
			cco->stWaterMeterSearchModel.stSearchWaterFrame.send_retry=0;
			MRS_StartTimer(MRS_CCO_TIMER_SET_WATER_METERLIST_SHIP, 1000 * 3, HI_SYS_TIMER_ONESHOT);
			return HI_ERR_SUCCESS;
		}
		if(ucTempFlag)
		{
			ret = mrsCcoCreateGetWaterMeterList(cco, cco->stTopoInf.entry[cco->stWaterMeterSearchModel.usCurrNum].mac);
		}
		//HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1002, HI_DIAG_MT("GetWaterMeterlist MAC"), cco->stTopoInf.entry[cco->stWaterMeterSearchModel.usCurrNum].mac, 6);
		if (HI_ERR_SUCCESS != ret)
	    {
	        HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_035, HI_DIAG_MT("mrsCcoGetMeterList ret"), ret);
			cco->stWaterMeterSearchModel.usCurrNum++;
			cco->stWaterMeterSearchModel.stSearchWaterFrame.send_retry++;
			MRS_StartTimer(MRS_CCO_TIMER_GET_WATER_METERLIST, 1000, HI_SYS_TIMER_ONESHOT);
			break;
	    }
		mrsCcoSendGetWaterMeter(cco);
    } while (0);

    return ret;
}

HI_U32 mrsCcoSearchWaterMeterSetShip(MRS_CCO_SRV_CTX_STRU *cco,HI_U8 ucFlag)
{
    HI_U32 ret = HI_ERR_SUCCESS;
	
	//MRS_SRV_LIST * list;
	//MRS_SEARCH_WATER_METER_LIST *node=HI_NULL;

    do
    {
		if(ucFlag)
		{
			ret = mrsCcoCreateSetWaterMeterShip(cco);
		}
		else
		{
			if(cco->stWaterMeterSearchModel.stSearchWaterFrame.send_retry >=cco->stWaterMeterSearchModel.stSearchWaterFrame.retry_max)
			{
				/*
				list = &(cco->stWaterMeterSearchModel.stGetWaterMeterList);
				node = (MRS_SEARCH_WATER_METER_LIST *)list->next;
				mrsSrvListRemove(list,&(node->link));
				mrsToolsFree(node);
				*/
				ret = mrsCcoCreateSetWaterMeterShip(cco);
			}
		}
		if (HI_ERR_SUCCESS != ret)
	    {
	        HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_036, HI_DIAG_MT("mrsCcoSearchWaterMeterSetShip ret"), ret);
			break;
	    }
		ret =mrsCcoSendSetWaterMeterShip(cco);
		if (HI_ERR_SUCCESS != ret)
		{
	        HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_069, HI_DIAG_MT("mrsCcoSendGetWaterMeter ret"), ret);
		}
		cco->stWaterMeterSearchModel.bSetMeterShipSendPLCFlag =1;
    } while (0);

    return ret;
}


HI_U32 mrsCmdCCORecvWaterMeterSetShip(HI_PVOID pPayload, HI_U16 usPayloadLen)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    //MRS_PLC_FRAME_DATA_STRU *plc_data = HI_NULL;
    MRS_CCO_SRV_CTX_STRU * cco_ctx = mrsCcoGetContext();
	HI_U16 data_len=0;

    // ά��ӳ���
    MRS_DATA_TRANSMIT_AFN13_UP_STRU *pstTransmit = (MRS_DATA_TRANSMIT_AFN13_UP_STRU *)pPayload;
    HI_U8 * data_buff = HI_NULL;

    if (pstTransmit->seq != cco_ctx->stWaterMeterSearchModel.usWaterSearchSeq)
    {
        HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_030, HI_DIAG_MT("GetMeterDataInfo seq err [%d][%d]"),
                        pstTransmit->seq,cco_ctx->stWaterMeterSearchModel.usWaterSearchSeq);
        return HI_ERR_FAILURE;
    }
    if(pstTransmit->data_len<1)
    {
    	return HI_ERR_FAILURE;
    }
    data_buff = (HI_U8*)pstTransmit + pstTransmit->stru_size;   // ��ȡת������
    data_len = pstTransmit->data_len;   // ��ȡת�����ݳ���

   // �ͷű��ݵ�Plc����֡����
	if(cco_ctx->stWaterMeterSearchModel.stSearchWaterFrame.plc!= HI_NULL)
    {
        if(cco_ctx->stWaterMeterSearchModel.stSearchWaterFrame.plc->pucPayload != HI_NULL)
        {
            mrsToolsFree(cco_ctx->stWaterMeterSearchModel.stSearchWaterFrame.plc->pucPayload);   // �ͷ�ת�����ݿ�   
            cco_ctx->stWaterMeterSearchModel.stSearchWaterFrame.plc->pucPayload = HI_NULL;
        }
        
        mrsToolsFree(cco_ctx->stWaterMeterSearchModel.stSearchWaterFrame.plc);   // �ͷ�PLC֡����    
        cco_ctx->stWaterMeterSearchModel.stSearchWaterFrame.plc = HI_NULL;
    }
	//HI_U16 usAddrDataLen = sizeof(MRS_SEARCH_WATER_METER_LIST)+data_len;
	//
	//����������ϵ���յ�Ӧ��Ӧ����δ���
	MRS_RF_DAU_FRAME_STRU *pDauFrame=HI_NULL;
	//MRS_RF_ALL_WATER_METER_INFO *pWaterMeterInfo =mrsRfGetCCOAllWaterMeterInfo();
	MR_WATER_METER_SEARCH_STRU *pWaterMeter=&(cco_ctx->stWaterMeterSearchModel);
	pDauFrame = mrsRFDauDecode(data_buff+1,data_len-1);
	if(pDauFrame!=HI_NULL)
	{
		HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_090, HI_DIAG_MT("[RF]RecvWaterMeterSetShip Cmd ID"),pDauFrame->ucDataCmdId);
		if(pDauFrame->ucDataCmdId == 0x45)
		{
			// �յ�����ˮ���Ӧ��ɹ�����¼���ŵ���
			int i=0;
			for(i=0;i<pWaterMeter->usWaterMeterResultNum;i++)
			{
				if(0==mrsToolsMemcmp(pDauFrame->ucaDataSrcAddr,pWaterMeter->stWaterMeterResult[i].ucaWaterMac,7))
				{
					pWaterMeter->stWaterMeterResult[i].ucRFChannelInfoNum = pDauFrame->ucapDataData[0];
					break;
				}
			}
		}
	}
	else
	{
		HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_091, HI_DIAG_MT("[RF]RecvWaterMeterSetShip mrsRFDauDecode Err"));
	}
	//
	MRS_StopTimer(MRS_CCO_TIMER_SET_WATER_METERLIST_SHIP);
	cco_ctx->stWaterMeterSearchModel.bSetMeterShipSendPLCFlag=0;
	MRS_SRV_LIST * list;
	//MRS_SEARCH_WATER_METER_LIST *node=HI_NULL;
	
    list = &(cco_ctx->stWaterMeterSearchModel.stGetWaterMeterList);
	if(mrsSrvIsListEmpty(list))
	{
		//CJJ-HPLC-20181204:ˮ���������
		setWaterMeterNetStatue(WATER_METER_COMPLETE_NET);
		HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_058, HI_DIAG_MT("[RF]RecvWaterMeterSetShip list Empty"));
		return ret;
	}
	/*
	node = (MRS_SEARCH_WATER_METER_LIST *)list->next;
	mrsSrvListRemove(list,&(node->link));
	mrsToolsFree(node);
	*/
	cco_ctx->stWaterMeterSearchModel.stSearchWaterFrame.send_retry=0;
	// ������һ��
	mrsCcoSearchWaterMeterSetShip(cco_ctx,HI_TRUE);
	return ret;
}

HI_U32 mrsCcoCalcWaterMeterShip(MRS_CCO_SRV_CTX_STRU *cco)
{
	MR_WATER_METER_SEARCH_STRU *pWaterMeter=&(cco->stWaterMeterSearchModel);
	MRS_SRV_LINK * it;
    MRS_SRV_LIST * list;
	MRS_SEARCH_WATER_METER_LIST *node=HI_NULL;
	HI_U8 i=0,j=0,k=0;
	//MRS_CCO_SRV_CTX_STRU *pCtx = mrsCcoGetContext();
	HI_U16 usMeterAllNum = mrsSrvArchivesMeterNum();
	MRS_ARCHIVES_NODE_STRU *pstNode = HI_NULL;
	MRS_RF_ALL_WATER_METER_INFO * pWaterMeterInfo = mrsRfGetCCOAllWaterMeterInfo();
	HI_U16 usMeterLen=0;

    list = &(pWaterMeter->stGetWaterMeterList);
	if(pWaterMeterInfo->ucWaterMeterCount>0)
	{
		for(i=0;i<pWaterMeterInfo->ucWaterMeterCount;i++)
		{
			usMeterLen +=8;
		}

		HI_U16 usAddrDataLen = sizeof(MRS_SEARCH_WATER_METER_LIST)+usMeterLen;
		node = (MRS_SEARCH_WATER_METER_LIST*)mrsToolsMalloc(usAddrDataLen);
		if(node!=HI_NULL)
		{
			mrsToolsMemZero_s(node,usAddrDataLen,usAddrDataLen);
			node->ucAddressType =1;
			mrsToolsMemcpy_s(node->ucaStaMac, 6,cco->ucMainNodeAddr, 6);
			node->ucMeterNum = pWaterMeterInfo->ucWaterMeterCount;
			for(i=0;i<pWaterMeterInfo->ucWaterMeterCount;i++)
			{
				mrsToolsMemcpy_s(node->pMeterData+i*8, usMeterLen-i*8,pWaterMeterInfo->saWaterMeterInfo[i].ucaWaterMeterAddr, 7);
				node->pMeterData[i*8+7]=pWaterMeterInfo->saWaterMeterInfo[i].ucFieldValue;
			}
			
			mrsSrvListInsTail(list,&(node->link));
			HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_092, HI_DIAG_MT("CCO Meter Address Info Add Node->ucMeterNum"),node->ucMeterNum);
		}
	}	
	
	// ��������
    for (it = list->next; it != list; it = it->next)
    {
    	// ��������ṹ
        node = (MRS_SEARCH_WATER_METER_LIST *)it;
		
		HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1004, HI_DIAG_MT("����ַ,�ж�Ԥ���ϵ"), node->ucaStaMac, 6);
		HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_068, HI_DIAG_MT("[RF]��ȡ����ucMeterNum"),node->ucMeterNum);
		for(i=0;i<node->ucMeterNum;i++)
		{
			HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1010, HI_DIAG_MT("ˮ���ַWater MAC"), node->pMeterData+i*8, 7);
			//�ڴ˴��ҵ���ǰ�����Ϣ�е�һ����Ч�ı���Ϣ����ţ�
		    for (k = 0; k < usMeterAllNum; k++)
		    {
		        pstNode = mrsSrvArchivesQuery(k);
		        if (pstNode == HI_NULL)
		        {
		            break;
		        }

		        if (pstNode->bValid && (pstNode->ucProtocol>=METER_PROTO_SINGLEWAY_WATERMETER)) // ���ҵ�һ����Ч�ı���Ϣ
		        {
		        	
					if(0 ==mrsToolsMemcmp(pstNode->aucNodeAddr,node->pMeterData+i*8,6))
					{
						HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1011, HI_DIAG_MT("ƥ�䵽�����е�ַ"), pstNode->aucNodeAddr, 6);
						break;
					}
		        }
		    }
			if(k>=usMeterAllNum)
			{
				HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_083, HI_DIAG_MT("[RF]CalcWaterMeterShip���ڱ�����,ɾ��"));
				continue;
			}
			for(j=0;j<pWaterMeter->usWaterMeterResultNum;j++)
			{
				if(0 ==mrsToolsMemcmp(pWaterMeter->stWaterMeterResult[j].ucaWaterMac,node->pMeterData+i*8,6))
				{
					if(pWaterMeter->stWaterMeterResult[j].ucaFieldValue[0] > node->pMeterData[i*8+7])
					{
						if(node->ucAddressType)
						{
							pWaterMeter->stWaterMeterResult[j].ucCcoWaterMeterFlag=1;
						}
						pWaterMeter->stWaterMeterResult[j].ucaFieldValue[0] = node->pMeterData[i*8+7];
						HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1012, HI_DIAG_MT("�滻"), node->ucaStaMac, 6);
						// �ѳ�ǿ��Ϣ��õ���Ϣcopy���ڶ��ߵĵ�ַ
						pWaterMeter->stWaterMeterResult[j].ucaFieldValue[1]=pWaterMeter->stWaterMeterResult[j].ucaFieldValue[0];
						mrsToolsMemcpy_s(pWaterMeter->stWaterMeterResult[j].ucaStaMac[1], HI_PLC_MAC_ADDR_LEN, pWaterMeter->stWaterMeterResult[j].ucaStaMac[0], HI_PLC_MAC_ADDR_LEN);
						mrsToolsMemcpy_s(pWaterMeter->stWaterMeterResult[j].ucaStaMac[0], HI_PLC_MAC_ADDR_LEN, node->ucaStaMac, HI_PLC_MAC_ADDR_LEN);
					}
					else if(pWaterMeter->stWaterMeterResult[j].ucaFieldValue[1] > node->pMeterData[i*8+7])
					{
						// ��ǿ��Ϣ�ڶ��ߵĵ�ַ
						pWaterMeter->stWaterMeterResult[j].ucaFieldValue[1] = node->pMeterData[i*8+7];
						mrsToolsMemcpy_s(pWaterMeter->stWaterMeterResult[j].ucaStaMac[1], HI_PLC_MAC_ADDR_LEN, node->ucaStaMac, HI_PLC_MAC_ADDR_LEN);
					}
					break;
				}

			}
			if((j>= pWaterMeter->usWaterMeterResultNum)
				&&(pWaterMeter->usWaterMeterResultNum<MRS_WATER_METER_MAX_NUM))
			{
				mrsToolsMemcpy_s(pWaterMeter->stWaterMeterResult[j].ucaWaterMac,7,node->pMeterData+i*8,7);
				mrsToolsMemcpy_s(pWaterMeter->stWaterMeterResult[j].ucaStaMac[0], HI_PLC_MAC_ADDR_LEN, node->ucaStaMac, HI_PLC_MAC_ADDR_LEN);
				pWaterMeter->stWaterMeterResult[j].ucaFieldValue[0] = node->pMeterData[i*8+7];
				pWaterMeter->usWaterMeterResultNum++;
				if(node->ucAddressType)
				{
					pWaterMeter->stWaterMeterResult[j].ucCcoWaterMeterFlag=1;
				}
				HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1013, HI_DIAG_MT("���ӵ�ӳ�����"), pWaterMeter->stWaterMeterResult[j].ucaWaterMac, 7);
			}	
		}
    }
	
	for (it = list->next; it != list; it = it->next)
	{
		 node = (MRS_SEARCH_WATER_METER_LIST *)it;
		 node->ucMeterNum=0;
		 node->ucSendIndex=0;
	}

	
	for(j=0;j<pWaterMeter->usWaterMeterResultNum;j++)
	{
		for (it = list->next; it != list; it = it->next)
		{
			 node = (MRS_SEARCH_WATER_METER_LIST *)it;
			 if(0 ==mrsToolsMemcmp(pWaterMeter->stWaterMeterResult[j].ucaStaMac[0], node->ucaStaMac, HI_PLC_MAC_ADDR_LEN))
		 	{
				mrsToolsMemcpy_s(node->pMeterData+(node->ucMeterNum*7),7,pWaterMeter->stWaterMeterResult[j].ucaWaterMac,7);
				//node->pMeterData[node->ucMeterNum*7+6]=0;
				node->ucMeterNum++;
				break;
		 	}
			 
		}
		
		HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1005, HI_DIAG_MT("���ˮ���ַ"), pWaterMeter->stWaterMeterResult[j].ucaWaterMac,7);
		HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1006, HI_DIAG_MT("��Ӧ�ĵ���ַ"), pWaterMeter->stWaterMeterResult[j].ucaStaMac[0], 7);
	}
	
}
void RfFreePLCData(MRS_PLC_FRAME_DATA_STRU *pstPlcDataDl)
{
	if(pstPlcDataDl != HI_NULL)
   {
	   if(pstPlcDataDl != HI_NULL)
	   {
		   mrsToolsFree(pstPlcDataDl->pucPayload);	// �ͷ�ת�����ݿ�	
		   pstPlcDataDl->pucPayload = HI_NULL;
	   }
	   
	   mrsToolsFree(pstPlcDataDl);	// �ͷ�PLC֡����	
	   pstPlcDataDl = HI_NULL;
   }
}

HI_VOID RfFreeWaterMeterListNode(HI_VOID * pstNode, HI_VOID * pstCb)
{
    MRS_NOT_USED(pstCb);

    if (!pstNode)
    {
        return;
    }
    mrsToolsFree(pstNode);
}
HI_U32 mrsRfGetWaterMeterMAC(HI_U8 *pucDeviceAddr, HI_U8 *pucMacAddr)
{
	HI_U32 i=0;
	MR_WATER_METER_SEARCH_STRU *pWaterMeter=&(mrsCcoGetContext()->stWaterMeterSearchModel);
	for(i=0;i<pWaterMeter->usWaterMeterResultNum;i++)
	{
		if(0==mrsToolsMemcmp(pucDeviceAddr,pWaterMeter->stWaterMeterResult[i].ucaWaterMac,6))
		{
			pWaterMeter->ucReadWaterMeterDataIndex = i;
			if(pWaterMeter->stWaterMeterResult[i].ucWaterMaxTryCount>5)
			{
				pWaterMeter->stWaterMeterResult[i].ucWaterMeterIndex = 1-pWaterMeter->stWaterMeterResult[i].ucWaterMeterIndex;
			}
			mrsToolsMemcpy_s(pucMacAddr,6,pWaterMeter->stWaterMeterResult[i].ucaStaMac[pWaterMeter->stWaterMeterResult[i].ucWaterMeterIndex%2],6);
			if(pWaterMeter->stWaterMeterResult[i].ucCcoWaterMeterFlag)
			{
				return HI_ERR_CCO_DATA;
			}
			else
			{
				//mrsToolsMemcpy_s(pucMacAddr,6,pWaterMeter->stWaterMeterResult[i].ucaStaMac[0],6);
				HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1007, HI_DIAG_MT("RfGetWaterMeterMAC"), pucMacAddr, 6);
				return HI_ERR_SUCCESS;
			}
		}
	}
	return HI_ERR_NOT_FOUND;
}
HI_VOID mrsCcoFreeReportDataList(HI_VOID)
{
	MRS_RF_ALL_WATER_METER_INFO *pCcoWaterInfo = mrsRfGetCCOAllWaterMeterInfo();
    MRS_SRV_LINK * it;
    MRS_SRV_LIST * list;
	MRS_RF_REPORT_DATA_NODE *node=HI_NULL;
	HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_042, HI_DIAG_MT("CcoFreeReportDataList"));

    list = &(pCcoWaterInfo->pStaReportDataList);
	
	// ��������
    for (it = list->next; it != list; it = it->next)
    {
        node = (MRS_RF_REPORT_DATA_NODE *)it;
		pCcoWaterInfo->ucReportDataCount--;
		mrsSrvListRemove(list,(MRS_SRV_LINK *)node);
		mrsToolsFree(node);
    }
}
HI_VOID mrsCcoRecvWaterDataSendRxRespProc(HI_VOID * pAckParam)
{
	//CJJ-HPLC-20181127:�ϱ���ɾ������
	//mrsCcoFreeReportDataList();
}
HI_U32 mrsCcoRecvWaterDataSendAllTimeOutProc(HI_VOID * pAckParam)
{
	HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_083, HI_DIAG_MT("RecvWaterDataSendAllTimeOutProc"));
	//CJJ-HPLC-20181127:�ϱ���ɾ������
	return HI_ERR_SUCCESS;
    
}

HI_U32 mrsReportCcoUartRecvWaterMeterData(HI_VOID)
{
	// ȥ�ظ������ݡ����ϱ�������
	HI_U32 ret=HI_ERR_SUCCESS;
	HI_U16 usMeterAllNum = mrsSrvArchivesMeterNum();
	MRS_CCO_SRV_CTX_STRU* pCcoCtx =mrsCcoGetContext();
	MRS_RF_ALL_WATER_METER_INFO *pCcoWaterInfo = mrsRfGetCCOAllWaterMeterInfo();
	MRS_ARCHIVES_NODE_STRU *pstNode = HI_NULL;
	HI_U32 i=0;//,j=0;
	HI_U16 usDataTempLen=0;
	HI_U8 *pDataTemp=HI_NULL;
	HI_U8 nCountTemp=0;
	HI_U32 ulNowTime=HI_MDM_GetSeconds();

	MRS_SRV_LINK * it;
    MRS_SRV_LIST * list;
	MRS_RF_REPORT_DATA_NODE *node=HI_NULL;

    list = &(pCcoWaterInfo->pStaReportDataList);
	
	// ��������
    for (it = list->next; it != list; it = it->next)
    {
        node = (MRS_RF_REPORT_DATA_NODE *)it;
		if(1== (node->ucReportDataState)
			|| (ulNowTime - node->ulStartTime)>259200)////CJJ-HPLC-20181127:��ౣ��3��
		{
			pCcoWaterInfo->ucReportDataCount--;
			mrsSrvListRemove(list,(MRS_SRV_LINK *)node);
			mrsToolsFree(node);
			HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_084, HI_DIAG_MT("[RF]Del one ucaStaReportData,Count"),pCcoWaterInfo->ucReportDataCount);			
			continue;
		}
		else
		{
			usDataTempLen+=node->ucReportDataLen;
			nCountTemp++;
		}
    }

	pDataTemp = mrsToolsMalloc(usDataTempLen+2);
    if (HI_NULL == pDataTemp)
    {
        return HI_ERR_MALLOC_FAILUE;
    }
	usDataTempLen = 0;
	pDataTemp[usDataTempLen++] = pCcoCtx->ucSeq++;
	pDataTemp[usDataTempLen++] = nCountTemp;
	//�ڴ˴��ҵ���ǰ�����Ϣ�е�һ����Ч�ı���Ϣ����ţ�
	for (it = list->next; it != list; it = it->next)
    {
        node = (MRS_RF_REPORT_DATA_NODE *)it;
		
		HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1009, HI_DIAG_MT("[RF]uartˮ���ַ"), node->ucPStaReportData,6);
		for (i = 0; i < usMeterAllNum; i++)
		{
			pstNode = mrsSrvArchivesQuery(i);
			if (pstNode == HI_NULL)
			{
				break;
			}

			//CJJ-HPLC-20181127:�ϱ���ȥ��
			//if ((pstNode->ucProtocol>=METER_PROTO_SINGLEWAY_WATERMETER)&& pstNode->bValid &&(pstNode->ucRfReportFlag==0)) // ���ҵ�һ����Ч�ı���Ϣ
			if ((pstNode->ucProtocol>=METER_PROTO_SINGLEWAY_WATERMETER)&& pstNode->bValid) // ���ҵ�һ����Ч�ı���Ϣ
			{
				
				if(mrsToolsMemcmp(node->ucPStaReportData,pstNode->aucNodeAddr,6)==0)
				{
					pstNode->ucRfReportFlag=1;

					//CJJ-HPLC-20181127
					#if 1
					mrsToolsMemcpy_s(pDataTemp+usDataTempLen, node->ucReportDataLen, node->ucPStaReportData,7);
					usDataTempLen += 7;
					mrsToolsMemcpy_s(pDataTemp+usDataTempLen, node->ucReportDataLen, node->ucPStaReportData+10,0x12);
					usDataTempLen += 0x12;
					#else
					mrsToolsMemcpy_s(pDataTemp+usDataTempLen, node->ucReportDataLen, node->ucPStaReportData, node->ucReportDataLen);
					usDataTempLen+=node->ucReportDataLen;
					#endif
					break;
				}
			}
		}
	}
	HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_088, HI_DIAG_MT("uartȥ���ظ������ݳ���"),usDataTempLen);
	if(usDataTempLen <= 2)
	{
		mrsToolsFree(pDataTemp);
		return HI_ERR_SUCCESS;
	}
	ret =mrs3762_ReportAFN20_F3(pDataTemp,usDataTempLen,mrsCcoRecvWaterDataSendRxRespProc,mrsCcoRecvWaterDataSendAllTimeOutProc);
	if(ret!=HI_ERR_SUCCESS)
	{
		HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_089, HI_DIAG_MT("uart mrs3762_ReportAFN20_F3"),ret);
	}
	mrsToolsFree(pDataTemp);
	return HI_ERR_SUCCESS;
}
HI_U32 mrs3762_ReportAFN20_F3(HI_U8 *pData,HI_U16 usDataLen,MRS_SEND_CCO_PROC_RX_F pRxRespProc,MRS_SEND_CCO_PROC_F pTotalTimeOutProc)
{
    HI_U32 ret = HI_ERR_SUCCESS;

    MRS_CCO_SRV_CTX_STRU* pstCcoCtx = mrsCcoGetContext(); // ����ָ��ȫ�������Ľṹ��ָ��
    PROTO376_2_FRAME_AFN_AND_DATA_STRU stAfnData;
    HI_U8 *pucOutBuf = HI_NULL;
    HI_U16 usBufLen = 0;
    MRS_SRV_CCO_ITEM *pstCcoItem = HI_NULL;

    if ((pData == HI_NULL)||(usDataLen==0))
    {
    	ret = HI_ERR_INVALID_PARAMETER;
        return ret;
    }

    mrsToolsMemZero_s(&stAfnData, sizeof(stAfnData), sizeof(stAfnData));
    stAfnData.ucAFN = MRS_AFN(0x20); // AFN = 20H �����ϱ���
    stAfnData.ucDT[0] = MRS_AFN_FN_LO(3);
    stAfnData.ucDT[1] = MRS_AFN_FN_HI(3);
    stAfnData.pData = pData;
    stAfnData.usDataLen = usDataLen;
    stAfnData.ucPhaseFlag = HI_TRUE;

    ret = mrsCreate3762UpFrame(&stAfnData, &pucOutBuf, &usBufLen, pstCcoCtx->stReadWaterMeterModel.stRfCurrNode.aucNodeAddr, pstCcoCtx->ucMainNodeAddr);
    if (ret != HI_ERR_SUCCESS)
    {
        mrsToolsFree(pucOutBuf);
        return ret;
    }

    pstCcoItem = (MRS_SRV_CCO_ITEM *)mrsToolsMalloc(sizeof(MRS_SRV_CCO_ITEM) + usBufLen);
    if (!pstCcoItem)
    {
        mrsToolsFree(pucOutBuf);
        return HI_ERR_MALLOC_FAILUE;
    }

    mrsToolsMemZero_s(pstCcoItem, sizeof(MRS_SRV_CCO_ITEM) + usBufLen, sizeof(MRS_SRV_CCO_ITEM) + usBufLen);
    pstCcoItem->bValid = HI_TRUE;
    pstCcoItem->ucAfn = 0x20;
    pstCcoItem->usFn = 0x03;
    pstCcoItem->usTimeOut = (HI_U16)mrsToolsGetTimeOut(pstCcoCtx->stReadWaterMeterModel.usAfn20F3UpTimeOut);//��λΪms
    pstCcoItem->ucMaxTry = pstCcoCtx->stReadWaterMeterModel.ucAfn20F3UpRetryMax;
    pstCcoItem->usDataLen = usBufLen;
    mrsToolsMemcpy_s(pstCcoItem->pData, pstCcoItem->usDataLen, pucOutBuf, usBufLen);
    pstCcoItem->MrsCcoRxRespProc = pRxRespProc;
    pstCcoItem->MrsCcoRxTotalTimeOutProc = pTotalTimeOutProc;
    pstCcoItem->MrsCcoRxEveryTimeOutProc = HI_NULL;
    
    mrsCcoJoinQueue(pstCcoItem);

    mrsActiveCcoQueue();
    mrsSetReadMeterState(EN_MRS_SRV_CCO_READMETER_WAIT_UPDATA_ACK);   // �ȴ�������ȷ��

    mrsToolsFree(pucOutBuf);

    return HI_ERR_SUCCESS;
}

//*****************************************************************************
// ��������: mrsCmdCcoGetDevId
// ��������: ��ȡ���豸����ID(оƬid���豸id)�����豸PLC֡���ش���
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
// ��    ��: [2018-12-10]
//*****************************************************************************
HI_U32 mrsCmdCcoPlcGetDevId(HI_IN HI_U16 id, HI_PVOID req_packet, HI_U16 packet_size)
{
    MRS_PLC_FRAME_DATA_STRU *plc_data = (MRS_PLC_FRAME_DATA_STRU *)req_packet;
    mrs_plc_dev_id_info_dl *id_info = HI_NULL;

    HI_UNREF_PARAM(id);
	HI_UNREF_PARAM(packet_size);

    if ((plc_data == HI_NULL) || (plc_data->pucPayload == HI_NULL)) {
        return HI_ERR_INVALID_PARAMETER;
    }

    /*
     * ���������б�ʶλȷ�����յ�NDM�����Ļ���STAӦ����
     * �����б���ǰ����ͬ��ʹ�����б��ĸ�ʽ�еķ���λ
     */
    id_info = (mrs_plc_dev_id_info_dl *)plc_data->pucPayload;

    if (id_info->dir_flag == MRS_PLC_UP_FLG) {
        return mrs_cco_query_sta_dev_id_ind(plc_data);
    } else {
        return mrs_ndm_query_cco_dev_id_req(plc_data);
    }
}


hi_u32 mrs_cco_query_sta_dev_id_ind(MRS_PLC_FRAME_DATA_STRU *plc_data)
{
	mrs_plc_dev_id_info_up *dev_info = HI_NULL;
	afn_10_f40_ul_data_param ul_data;
	hi_pbyte frame_buf = HI_NULL;
	hi_u16 frame_len = 0;
    hi_u16 up_len = 0;
    hi_u32 ret = HI_ERR_SUCCESS;
    PROTO376_2_FRAME_PRE_STRU *read_id_frame = HI_NULL;

    dev_info = (mrs_plc_dev_id_info_up *)plc_data->pucPayload;

    if (dev_info->id_len > MANAGEMENT_ID_SIZE) {
        return HI_ERR_INVALID_PARAMETER;
    }

    up_len = sizeof(mrs_plc_dev_id_info_up) + dev_info->id_len - MANAGEMENT_ID_SIZE;
    if (up_len != plc_data->usPayloadLen) {
        return HI_ERR_INVALID_PARAMETER;
    }

    read_id_frame = (PROTO376_2_FRAME_PRE_STRU *)mrs_get_read_id_frame();
    if (read_id_frame == HI_NULL) {
        return HI_ERR_FAILURE;
    }

    MRS_StopTimer(MRS_CCO_TIMER_STA_PLC_READ_DEV_ID);

    /* ��֯1376.2Ӧ��֡ */
    (hi_void)memset_s(&ul_data, sizeof(ul_data), 0, sizeof(ul_data));
    ul_data.ul_param.dev_type = dev_info->id_info[dev_info->id_len];
    ul_data.ul_param.id_type = dev_info->id_type;
    (hi_void)memcpy_s(ul_data.ul_param.node_addr, sizeof(ul_data.ul_param.node_addr), plc_data->ucMac, sizeof(plc_data->ucMac));
    mrsHexInvert(ul_data.ul_param.node_addr, sizeof(ul_data.ul_param.node_addr));

	ul_data.id_len = dev_info->id_len;
    (hi_void)memcpy_s(ul_data.id_info, sizeof(ul_data.id_info), dev_info->id_info, dev_info->id_len);

    (hi_void)memcpy_s(read_id_frame->ucSrcAdd, HI_PLC_MAC_ADDR_LEN, ul_data.ul_param.node_addr, HI_PLC_MAC_ADDR_LEN);
	read_id_frame->stAfnData.usDataLen = sizeof(ul_data.ul_param) + sizeof(ul_data.id_len) + ul_data.id_len;
	read_id_frame->stAfnData.pData = (HI_U8*)&ul_data;

	ret = mrsCreate3762Frame(read_id_frame, &frame_buf, &frame_len, HI_NULL);
	if(ret == HI_ERR_SUCCESS) {
    	ret = MRS_SendMrData(frame_buf, frame_len, HI_DMS_CHL_UART_PORT_APP);
	}

    mrsToolsFree(frame_buf);

	mrs_free_read_id_frame();

    return ret;
}

hi_u32 mrs_ndm_query_cco_dev_id_req(MRS_PLC_FRAME_DATA_STRU *plc_frame)
{
	hi_u32 ret = HI_ERR_SUCCESS;
	mrs_plc_dev_id_info_dl * plc_dl_info = HI_NULL;
	mrs_plc_dev_id_info_up plc_up_info;
	MRS_PLC_FRAME_DATA_STRU plc_ind_frame;

    if (plc_frame->usPayloadLen != sizeof(mrs_plc_dev_id_info_dl)) {
        return HI_ERR_FAILURE;
    }

    plc_dl_info = (mrs_plc_dev_id_info_dl*)plc_frame->pucPayload;

	/* ��ȡ��ID���Ͳ�����,���ش���֡ */
	if (plc_dl_info->id_type >= MRS_DEV_ID_MAX) {
		return HI_ERR_INVALID_PARAMETER;
	}

    (hi_void)memset_s(&plc_up_info, sizeof(plc_up_info), 0, sizeof(plc_up_info));
	if (plc_dl_info->id_type == MRS_DEV_ID_CHIP) {
		ret = mrsGetDevIdChip(plc_up_info.id_info, sizeof(plc_up_info.id_info));
		plc_up_info.id_len = MANAGEMENT_ID_SIZE;
	} else {
		ret = mrsGetDevIdDev(plc_up_info.id_info, sizeof(plc_up_info.id_info));
		plc_up_info.id_len = MRS_DEV_ID_SIZE;
	}

	if (ret != HI_ERR_SUCCESS) {
		return ret;
	}

	plc_up_info.stru_ver = plc_dl_info->stru_ver;
	plc_up_info.stru_len = sizeof(plc_up_info) + plc_up_info.id_len - MANAGEMENT_ID_SIZE;
	plc_up_info.id_type = plc_dl_info->id_type;
	plc_up_info.dir_flag = MRS_PLC_UP_FLG;
	plc_up_info.seq = plc_dl_info->seq;
	plc_up_info.id_info[plc_up_info.id_len] = MRS_DEV_TYPE_CCO;

    (hi_void)memset_s(&plc_ind_frame, sizeof(plc_ind_frame), 0, sizeof(plc_ind_frame));
    (hi_void)memcpy_s(plc_ind_frame.ucMac, sizeof(plc_ind_frame.ucMac), plc_frame->ucMac, sizeof(plc_frame->ucMac));
	plc_ind_frame.usId = plc_frame->usId;
	plc_ind_frame.usPayloadLen = plc_up_info.stru_len;
	plc_ind_frame.pucPayload = (HI_PBYTE)(&plc_up_info);

	return MRS_SendPlcFrame(&plc_ind_frame);
}

#endif
