//*****************************************************************************
//
//                  ��Ȩ���� (C), 1998-2011, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mrs_srv_sta.c
//  �� �� ��   : V1.0
//  ��    ��   : liuxipeng/KF54842
//  ��������   : 2011-12-02
//  ��������   : STA�˴�������ʵ��
//
//  �����б�   : TODO: ...
//  �޸���ʷ   :
//  1.��    �� : 2011-12-02
//    ��    �� : liuxipeng/KF54842
//    �޸����� : �����ļ�
//
//*****************************************************************************
#include "mrs_common.h"
#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
#include "mrs_cmd_msg.h"
#include "mrs_fw_tools.h"
#include "mrs_fw_log.h"
#include "mrs_fw_n.h"
#include "mrs_fw_nv.h"
#include "mrs_fw_proto376_2_echo.h"
#include "mrs_fw_proto698_45.h"
#include "mrs_srv_common.h"
#include "mrs_srv_sta_queue.h"
#include "mrs_srv_sta_baudrate.h"
#include "mrs_srv_parallel_sta.h"
#include "mrs_srv_sta_event.h"
#include "mrs_time.h"
#include "mrs_srv_sta.h"
#include "mrs_srv_io.h"
#include "equip_dut_proc.h"
#include "dfx_sys.h"
#include "mrs_srv_collector.h"
#include "mrs_dfx.h"
#include "hi_mdm.h"
#include "mrs_srv_sta_searchmeter.h"
#include "mrs_srv_mrslog.h"
#include "mrs_srv_clti_searchmeter.h"
#include "mrs_srv_cltii_searchmeter.h"
#include "mrs_dfx_clt.h"
#include "mrs_srv_cltii_event.h"
#include "mrs_srv_sta_dutycycle.h"
#include "mrs_srv_collector_irda.h"
/* BEGIN: Added by fengxiaomin/00209182, 2015/10/23   ���ⵥ�ź�:DTS2015102710230 */
#include "mrs_srv_sta_meter.h"
/* END:   Added by fengxiaomin/00209182, 2015/10/23 */
#include "mrs_srv_clti_upgrade.h"
#include "hi_app.h"
#include "mrs_srv_capture.h"
#include "mrs_srv_baudrate_manage.h"
#include "mrs_srv_sta_testframe.h"
#include "mrs_prepare_test_mode.h"
#include "mrs_srv_cltii_power_on_off.h"
#include "mrs_transfomer_identify.h"
#include "mrs_srv_rf.h"
#include "mrs_fw_proto188.h"
#include "mrs_srv_dau.h"
#include "rf_main.h"


HI_U32 mrsStaTransmitCmd(HI_PBYTE, HI_U16, HI_U8);
HI_U32 mrsStaRS485TranData(HI_PBYTE data, HI_U16 len, HI_U8 from, HI_U16 usBaudRate);

HI_U32 SAL_SetEthCfg(HI_IN char * ucIfName,HI_IN HI_PVOID pCmdParam ,HI_U16 usCmdParamSize);
HI_U32 SAL_GetEthCfg(HI_IN char * ucIfName,HI_IN HI_PVOID pCmdParam ,HI_U16 usCmdParamSize);

HI_U32 mrsMsgStaPlcChlStatusNotify(HI_U16 usMsgId,HI_PVOID pMsgParam);
HI_U32 mrsMsgStaUartFrmRx_645(HI_U16 usMsgId, HI_PVOID pMsgParam);
HI_U32 mrsMsgStaUartFrmRx_698_45(HI_U16 usMsgId, HI_PVOID pMsgParam);
HI_U32 mrsMsgStaUartFrmRx_Trans(HI_U16 usMsgId, HI_PVOID pMsgParam);
HI_U32 mrsStaRxAfn13Pro(MRS_PLC_FRAME_DATA_STRU *frame, HI_PVOID *data, HI_U16 *len, MRS_STA_ITEM *item);
HI_U32 mrsStaRxAfn14Pro(MRS_PLC_FRAME_DATA_STRU *frame, HI_PVOID *data, HI_U16 *len, MRS_STA_ITEM *item);
HI_U32 mrsStaHeart(HI_VOID);
HI_U32 mrsCmdStaSerialTransmit(HI_IN HI_U16 id, HI_PVOID req_packet, HI_U16 req_packet_size);
HI_U32 mrsBroadcastCmdTransmit(HI_IN HI_U16 usId, HI_PVOID pReqPacket, HI_U16 usReqPacketSize);
HI_U32 mrsEventCmdTransmit(HI_IN HI_U16 usId, HI_PVOID pReqPacket, HI_U16 usReqPacketSize);
HI_U32 mrsTestCmdTransmit(HI_IN HI_U16 usId, HI_PVOID pReqPacket, HI_U16 usReqPacketSize);
HI_U32 mrsCmdStaDataTransmit(HI_IN HI_U16 usId, HI_PVOID pReqPacket, HI_U16 usReqPacketSize);
HI_U32 mrsCmdStaSearchMeter(HI_IN HI_U16 usId, HI_PVOID pReqPacket, HI_U16 usReqPacketSize);
HI_U32 mrsCmdStaGetMeterAddr(HI_IN HI_U16 usId,HI_PVOID pstReq,HI_U16 usReqPacketSize);
HI_U32 mrsCmdStaGetMeterList(HI_IN HI_U16 usId,HI_PVOID pstReq,HI_U16 usReqPacketSize);
HI_U32 mrsCmdStaStartSearchMeter(HI_IN HI_U16 id, HI_PVOID req_packet, HI_U16 req_packet_size);
HI_U32 mrsCmdStaStopSearchMeter(HI_IN HI_U16 id, HI_PVOID req_packet, HI_U16 req_packet_size);
HI_U32 mrsCmdStaPlcTestTransmit(HI_IN HI_U16 id, HI_PVOID req_packet, HI_U16 packet_size);

HI_VOID mrsStaHeartTimeoutCnt(HI_U16 id);
HI_U32 mrsMsgStaTimer(HI_U16 usMsgId, HI_IN HI_PVOID pMsgParam);
HI_U32 mrsStaReport2CCO(HI_VOID);

HI_U32 mrsStaReportSmResult2CCO(HI_U8 *pucSrcMac, HI_U8 *pucDstMac, HI_BOOL bFlag);
HI_U32 mrsCltReportSmResult2CCO(HI_U8 *pucSrcMac, HI_U8 *pucDstMac, hi_bool flag);


HI_U32 mrsStaCreateReadMeterPlc(MRS_PLC_FRAME_DATA_STRU *frame_data,MRS_STA_ITEM * item
                                            ,HI_U8 *data,HI_U16 len,HI_U8 protocol);

// ��ȡPLC�շ�ģʽ
HI_U8 mrsStaGetPlcRxTxMode(HI_VOID);

HI_BOOL mrsStaCheckDenyRetry(HI_U16 usId);

HI_BOOL mrsCheckTimingFrame(HI_PBYTE pFrame, HI_U16 usFrameLength);

HI_PRV HI_U32 mrsStaCheckDIValid(HI_U32 ulDestDI, MRS_645_FRAME_STRU *pst645Hrd);
HI_PRV HI_U32 mrsCltRmMeterSetMacProc(MRS_STA_ITEM *pstItem, MRS_645_FRAME_STRU *pst645Hrd);
HI_PRV HI_VOID mrsCltRmMeterSetMacTimeout(HI_VOID);

HI_PRV HI_U32 mrsStaFrameRxPreProc(MRS_STA_ITEM * pstItem, HI_U8 *pData, HI_U16 usLen, MRS_645_FRAME_STRU *pst645Hrd);

HI_U32 mrsStaUartFrameTimeout(HI_VOID);

HI_U32 mrsStaBroadCastBufFind(HI_U8 * frame_data, HI_U16 frame_len);

#if defined(PRODUCT_CFG_SUPPORT_MRS_DUOL_MODE_RF)
HI_U32 mrsStaHandleWaterCmd(MRS_PLC_METERLIST_DL_S * pstFrame,HI_U16 nDataLen);
#endif
//*****************************************************************************
//ȫ�ֱ���
//*****************************************************************************
HI_PRV MRS_STA_SRV_CTX_STRU* g_pstMrsStaSrvCtx = HI_NULL;
//*****************************************************************************
//��������
//*****************************************************************************

//��645֡ת����HSO
HI_PRV HI_U32 mrsStaDiag2HSO(HI_U8 * data, HI_U16 len,HI_U8 histance);

HI_PRV HI_U32 mrsStaCheckReport2Cco(HI_U8 report_status);

/* BEGIN: PN: DTS2016021906890 MODIFY\ADD\DEL by cuiate/00233580 at 2016/2/20 */
HI_PRV HI_U32 mrsStaReportDeviceInf(HI_U8 *pucSrcMac, HI_U8 *pucDstMac);
/* END:   PN: DTS2016021906890 MODIFY\ADD\DEL by cuiate/00233580 at 2016/2/20 */

HI_PRV HI_U32 mrsCheckCltRmMeterToMac(MRS_STA_ITEM *pstItem, HI_U8 ucFn);
HI_PRV HI_VOID mrsStaAddrUpdateProc(HI_U8 aucAddr[]);
HI_PRV HI_VOID mrsSta698AddrUpdateProc(HI_U8 *pucData, HI_U16 usDataSizse);


//*****************************************************************************
//����ʵ��
//*****************************************************************************
//�����ַ֡
HI_U8 MR_FRAME_645[] =
{
    0xFE, 0xFE, 0xFE, 0xFE, 0x68, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0x68, 0x13, 0x00, 0xDF, 0x16
};

HI_U8 MR_FRAME_698[] =
{
    0xFE, 0xFE, 0xFE, 0xFE, 0x68, 0x17, 0x00, 0x43, 0x45, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0x00,
    0x5B, 0x4F, 0x05, 0x01, 0x00, 0x40, 0x01, 0x02, 0x00, 0x00, 0xED, 0x03, 0x16
};

HI_U8* mrsGetSendAddrFrame(HI_U8 ucProto, HI_U16 * pusLength)
{
    if (MRS_SRV_PROTO_698_45 == ucProto)
    {
        *pusLength = sizeof(MR_FRAME_698);
        return MR_FRAME_698;
    }
    else
    {
        *pusLength = sizeof(MR_FRAME_645);
        return MR_FRAME_645;
    }
}

/* BEGIN: Added by fengxiaomin/00209182, 2014/5/28   ���ⵥ�ţ�DTS2014052808754 */
HI_VOID mrsGetStaMrParam(NV_APP_MR_CFG_STRU* cfg)
{
    MRS_STA_SRV_CTX_STRU *sta = mrsStaGetContext();

    sta->stFrameBufCtx.max_count = (HI_U32)cfg->ucFrameBufItemCount;
    MRS_DATA_VALID_CHECK(sta->stFrameBufCtx.max_count,
                            PRODUCT_CFG_STA_FRAME_BUF_ITEM_COUNT_MAX,
                            PRODUCT_CFG_STA_FRAME_BUF_ITEM_COUNT_DEFAULT,
                            PRODUCT_CFG_STA_FRAME_BUF_ITEM_COUNT_MIN);

    sta->stFrameBufCtx.timeout = (HI_U32)cfg->ucFrameBufTimeout;
    MRS_DATA_VALID_CHECK(sta->stFrameBufCtx.timeout,
                            PRODUCT_CFG_STA_FRAME_BUF_TIMEOUT_MAX,
                            PRODUCT_CFG_STA_FRAME_BUF_TIMEOUT_DEFAULT,
                            PRODUCT_CFG_STA_FRAME_BUF_TIMEOUT_MIN);

    sta->plc_rxtx_mode = cfg->plc_rxtx_mode;

    mrsGetFwCtx()->ulSimuChlTimeout = cfg->ulSimuChlTimeout;

    sta->addr_try_num = cfg->get_addr_try_num;
    if (sta->addr_try_num > MRS_STA_SEND_ADDR_FRAME_TIMES_MAX)
    {
        sta->addr_try_num = MRS_STA_SEND_ADDR_FRAME_TIMES_DEFAULT;
    }

    sta->addr_Fail_wait_time = cfg->get_addr_Fail_wait_time;
    MRS_DATA_VALID_CHECK(sta->addr_Fail_wait_time,
                            MRS_STA_WAIT_SEND_TIME_MAX,
                            MRS_STA_WAIT_SEND_TIME_DEFAULT,
                            MRS_STA_WAIT_SEND_TIME_MIN);

    sta->ucBindNetwork = cfg->ucBindNetwork;
    sta->usBindDuration = cfg->usBindDuration;

    if(cfg->ucSoftTfMode < SOFT_TF_MODE_INVALID)
    {
        sta->ucSoftTfMode = cfg->ucSoftTfMode;
        HI_MDM_SetSoftTfMode((HI_MDM_SOFT_TF_MODE_EN)sta->ucSoftTfMode);
    }

#if defined(PRODUCT_CFG_SUPPORT_TEST_CONCENTRATOR)
    mrsStaSetTestModeInf(cfg->usTestFrameInterval_usCrcInitValue, cfg->ucTestModeSnId_ucCsgParallelEnable);
#endif

    return;
}


/* END:   Added by fengxiaomin/00209182, 2014/5/28 */

//ȫ�ֳ�ʼ�����ʼ�ĳ�ʼ��
HI_VOID mrsStaGlobalInit(HI_VOID)
{
    mrsStaSaveContext(HI_NULL);

    if (!mrsToolsIsIICollector())
    {
        mrsRegEventOutCallBack(mrsStaEvtOutStatusNotify, MRS_STA_EVT_FROM_METER); //EVENT_OUT�ܽ��쳣ʱ��ע��ص�����(STA�˳�ʼ����)
    }
}

//*****************************************************************************
// ��������: mrsStaPowerPro
// ��������: MRSģ����STA�ϵ紦����
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
HI_VOID mrsStaPowerOnPro(HI_VOID)
{
    mrsNvSimuStateCfg(ID_MV_APP_SIMU_METER_INFO);
    mrsEvtOutResetInt();

    return;
}
/* END:   Added by fengxiaomin/00209182, 2014/1/14 */



//*****************************************************************************
// ��������: mrsStaUpdateTopAddrInf
// ��������: �����׵�ַ��Ϣ
//
// ����˵��:
//  ucAddr [in] : �����µı��ַ
//  ucProtocol [in] : ����Լ����
// �� �� ֵ:
//
// ����Ҫ��: ��
// ���þ���: ��
// ��    ��: liuxipeng/lkf54842 [2014-06-06]
//*****************************************************************************
HI_U32 mrsStaUpdateTopAddrInf(HI_IN HI_U8 ucAddr[HI_METER_ADDR_LEN], HI_IN HI_U8 ucProtocol)
{
    HI_U8 ucDevAddr[HI_METER_ADDR_LEN] = {0};
    HI_U8 ucType = STA_TYPE_METER;

    // �����׽ڵ���Ϣ
    mrsStaRefreshTopNodeInf(ucAddr, ucProtocol);

    if(mrsGetSyncMacFlg())
    {
        return HI_ERR_SUCCESS;
    }

    mrsSetSyncMacFlg(HI_TRUE);

   (hi_void)memcpy_s(ucDevAddr, HI_METER_ADDR_LEN, ucAddr, HI_METER_ADDR_LEN);
    mrsHexInvert(ucDevAddr, HI_METER_ADDR_LEN);    // ����

    HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1001, HI_DIAG_MT("Node Meter"), ucDevAddr, HI_METER_ADDR_LEN);
#if defined(PRODUCT_CFG_CLTI_SUPPORT_MRS_SWITCH)
    if (mrsToolsIsCLTI())
    {
        ucType = STA_TYPE_CLTI;
        mrsStaSetCltiType(HI_TRUE);
    }
#endif
    // ���õ���ַ��Mac��
    mrsStaSetMeterToMac(ucDevAddr, ucType, HI_MDM_METER_MAC);

    // ���õ�����͵�APP��
    mrsStaSetType(ucType);

    // �����豸��Ϣ
    HI_MDM_DevIdConfig(ucDevAddr);

    return HI_ERR_SUCCESS;
}


// ��ȡCCO֡�¾ɱ�־
HI_BOOL mrsIsOldFrame(HI_VOID)
{
    return mrsStaGetContext()->IsOldFrame;
}
// ����CCO֡�¾ɱ�־
HI_VOID mrsSetFrameFlg(HI_BOOL flg)
{
    mrsStaGetContext()->IsOldFrame = flg;
}

// ��ȡͬ��MAC��ַ������־
HI_BOOL mrsGetSyncMacFlg(HI_VOID)
{
    return mrsStaGetContext()->sync_mac_flg;
}

// ����ͬ��MAC��ַ������־
HI_VOID mrsSetSyncMacFlg(HI_BOOL flg)
{
    mrsStaGetContext()->sync_mac_flg = flg;
    return;
}

//*****************************************************************************
// ��������: mrsGetRepFlagNv
// ��������: ��ȡ�����ϱ����ַ����---NV��
//
// ����˵��:
//   HI_VOID
//
// �� �� ֵ:
//    HI_VOID
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: fengxiaomin/00209182 [2013-02-21]
//*****************************************************************************
HI_VOID mrsGetRepFlagNv(HI_VOID)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    NV_APP_REPORT_METER_INFO_STRU report_info = {0};
    MRS_STA_SRV_CTX_STRU * sta = mrsStaGetContext();

    ret = HI_MDM_NV_Read(ID_NV_APP_REPORT_METER_INFO, (HI_PVOID)&report_info, sizeof(NV_APP_REPORT_METER_INFO_STRU));
    if (ret != HI_ERR_SUCCESS)
    {
        sta->staRepFlag = MRS_STA_REPORT_METER_INFO_INVALID;
        sta->meterChangeFlag= MRS_STA_REPORT_METER_INFO_VALID;//Ĭ��ֵ:��
        sta->ucDutAdcFlag = HI_TRUE;
        return ;
    }

    sta->staRepFlag = report_info.ucStaRepFlag;
    sta->meterChangeFlag = report_info.ucMeterChangeFlag;
    sta->ucDutAdcFlag = report_info.ucDutAdcFlag;

    mrsDfxReportStatus(sta->staRepFlag);
    mrsDfxChangeStatus(sta->meterChangeFlag);

    return ;
}

//*****************************************************************************
// ��������: mrsStaInitCtx
// ��������: ��ʼ��STA�����ģ�ֻ���ڵ�һ�γ�ʼ����ʱ������ڴ�,
//           ͨѶģ����ܻ�������Σ�ÿ��������Ҫ��ʼ��������
//
// ����˵��:
//   HI_VOID
//
// �� �� ֵ:
//    HI_VOID
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: niesongsong/nkf62735 [2012-05-29]
//*****************************************************************************
HI_VOID mrsStaInitCtx(HI_VOID)
{
    HI_U8 macAddr[6] = {0};
    MRS_STA_SRV_CTX_STRU * sta;

    sta = mrsStaGetContext();
    if (!sta)
    {
        sta = (MRS_STA_SRV_CTX_STRU*)mrsToolsMalloc(sizeof(MRS_STA_SRV_CTX_STRU));
        if (sta == HI_NULL)
        {
            return ;
        }

        (hi_void)memset_s(sta, sizeof(MRS_STA_SRV_CTX_STRU), 0, sizeof(MRS_STA_SRV_CTX_STRU));

        //����������
        mrsStaSaveContext(sta);

        //��ʼ������
        mrsStaQueueInit(&sta->stQueModule);
    }

    // ����PLC״̬
    sta->plcState = HI_FALSE;

    // ����ͬ��MAC��ַ��־(δͬ��)
    mrsSetSyncMacFlg(HI_FALSE);

    // ���ö���
    mrsStaQueueReset(&sta->stQueModule);

    // �ѱ��ʼ��
    mrsStaSearchInit();

#if defined(PRODUCT_CFG_SYS_RST_SOLUTION)
    if(mrsToolsIsIICollector())
    {
        HI_MDM_RstRegisterNotify(mrsStaRxUartStat, HI_SYS_RST_CON_NOT_GET_METER_UART_DATA_CLT_II, 0);
    }
    else
    {
        HI_MDM_RstRegisterNotify(mrsStaRxUartStat, HI_SYS_RST_CON_NOT_GET_METER_UART_DATA_STA, 0);
    }

    HI_MDM_RstRegisterNotify(mrsRxRMPlcStat, HI_SYS_RST_CON_NOT_GET_METER_PLC, 0);
#endif

#if defined(PRODUCT_CFG_SUPPORT_TEST_CONCENTRATOR)
    // ����NV(0x401)��ʼ��֮ǰ
    mrsStaTestModeInit();
#endif

    // ��ʼ���ص�
    // ��������
    mrsInitStaMrNvInf();

    // Init sta rm config
    mrsNvStaRmCfg(ID_NV_MRS_RM_CFG);

    // ��ַ�����ϱ�
    mrsGetRepFlagNv();
    HI_MDM_NV_RegisterChangedNoitfyProc(HI_NV_FTM_COLLECT_PARAM_FILE,
                                     HI_NV_FTM_COLLECT_PARAM_FILE,
                                     mrsNvChangeMsgNotify);
    HI_MDM_NV_RegisterChangedNoitfyProc(ID_NV_APP_MR_CFG, ID_NV_APP_STA_SM_CFG_EXP, mrsNvChangeMsgNotify);
    HI_MDM_NV_RegisterChangedNoitfyProc(ID_NV_MRS_USER_VER, ID_NV_MRS_USER_VER, mrsNvChangeMsgNotify);

    // ��ʼ��PLC���˻���
    if(sta->stFrameBufCtx.item == HI_NULL)
    {
        HI_U32 nItemSize = sizeof(MRS_STA_FRAME_BUF_ITEM_STRU) * sta->stFrameBufCtx.max_count;
        sta->stFrameBufCtx.item = (MRS_STA_FRAME_BUF_ITEM_STRU *)mrsToolsMalloc(nItemSize);
        if (sta->stFrameBufCtx.item)
        {
            (hi_void)memset_s(sta->stFrameBufCtx.item, nItemSize, 0, nItemSize);
        }
    }

    // ��ȡģ���MAC��ַ
    HI_MDM_GetLocalMacAddr(macAddr);
    // ����ģ��MAC��ַ��Ϣ
    mrsStaSetMacAddr(macAddr);

    // ��ʼ����װ������
    mrsDutCtxInit();

    //ץ��������ʼ��ע��
    #ifdef PRODUCT_CFG_TTCN
    mrsSrvCaptureInit();
    #endif

    //����ַ֪ͨ�����ʼ���������mrsStaEvtCtxInit��mrsStaDutyCycleInitǰ������Ὣע��ĺ���ָ������
    //������������ע�ᣬҲ��Ҫ��ע�����mrsMeterChgNotifyProcInit��
/* BEGIN: Added by fengxiaomin/00209182, 2015/10/23   ���ⵥ�ź�:DTS2015102710230 */
    mrsMeterChgNotifyProcInit();
/* END:   Added by fengxiaomin/00209182, 2015/10/23 */
    mrsStaEvtCtxInit();
    mrsStaSetFrmProto(MRS_SRV_PROTO_645);

    if (EQUIP_DUT_IS_EQUIP_MODE())
    {
        mrsStaSetState(MRS_STA_STATE_MR);
        HI_MDM_LED_SetReadMeterAddrStatus(LED_READ_METER_SUCCESS);
    }
    else
    {
        //���ó�ʼ��״̬(δ̽�Ⲩ����)
        mrsStaSetState(MRS_STA_STATE_INIT);
        mrsStaSetMeterBaudrate(MRS_SRV_BAUD_RATE_DEFAULT);

        MRS_StartTimer(MRS_STA_TIMER_TEST, 10, HI_SYS_TIMER_ONESHOT);

    #if defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG) && defined(MRS_LOG_STA_SWITCH)
        // �����Լ�¼�ն���ͳ�ƶ�ʱ��
        MRS_StartTimer(MRS_STA_TIMER_DAYFREEZE_REC, MRS_STA_TIME_DAYFREEZE_REC, HI_SYS_TIMER_PERIODIC);
    #endif
#if defined(PRODUCT_CFG_CLTI_SUPPORT_MRS_SWITCH)
        if(!mrsToolsIsIICollector())
        {
            mrsStaDutyCycleInit();
        }
#endif
#if defined(PRODUCT_CFG_SUPPORT_MRS_UPG_DEMO)
        if (mrsToolsIsCLTI())
        {
            mrsCltIUpgModuleInit();
        }
#endif
#if defined(PRODUCT_CFG_SUPPORT_MRS_DUOL_MODE_RF)&&defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
	mrsRfInit();
#endif
    }
	mrsStaReloadNvVerCfg();
    mrsPoweOffOnInit();
}

/* BEGIN: Added by fengxiaomin/00209182, 2014/2/19   ���ⵥ��:DTS2014012301705 */
HI_U32 mrsSetCollectDevAddr(HI_U16 usId)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_NV_FTM_COLLECT_PARAM_FILE_S stCollectNV;
    HI_U8 ucAddr[HI_METER_ADDR_LEN] = {0};
    HI_U8 ucIPAddr[HI_IPV4_ADDR_LEN] = {0};

    if (!mrsToolsIsIICollector())
    {
        return ret;
    }

    MRS_NOT_USED(usId);
    (hi_void)memset_s(&stCollectNV, sizeof(stCollectNV), 0, sizeof(stCollectNV));
    (hi_void)memset_s(ucAddr, sizeof(ucAddr), 0, sizeof(ucAddr));

    do
    {
        ret = HI_MDM_NV_Read(HI_NV_FTM_COLLECT_PARAM_FILE, &stCollectNV, sizeof(stCollectNV));
        if (ret != HI_ERR_SUCCESS)
        {
            break;
        }

        mrsSetCollectorAddr(stCollectNV.CLTAddress);

        if (mrsToolsCheckBCD(stCollectNV.CLTAddress, HI_METER_ADDR_LEN))
        {
            mrsHexInvert(stCollectNV.CLTAddress, HI_METER_ADDR_LEN);
            HI_MDM_DevIdConfig(stCollectNV.CLTAddress);
            return ret;
        }
    } while (0);

    HI_MDM_NM_GetAddr(ucAddr,ucIPAddr);
    if (!mrsToolsCheckBCD(ucAddr, HI_METER_ADDR_LEN))
    {
        (hi_void)memset_s(ucAddr, sizeof(ucAddr), 0, sizeof(ucAddr));
    }

    ret = HI_MDM_DevIdConfig(ucAddr);
    return ret;
}
/* END:   Added by fengxiaomin/00209182, 2014/2/19 */

//*****************************************************************************
// ��������: mrsMsgStaPlcChlStatusNotify
// ��������: PLCͨ��״̬�ı�֪ͨ
//
// ����˵��:
//   HI_U16   usMsgId       ��ϢID(��������¿��Ժ���)
//   HI_PVOID pMsgParam     ֪ͨ����
// �� �� ֵ:
//    �ɹ�����  HI_ERR_SUCCESS
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: niesongsong/nkf62735 [2012-05-29]
//*****************************************************************************
HI_U32 mrsMsgStaPlcChlStatusNotify(HI_U16 usMsgId,HI_PVOID pMsgParam)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    MRS_MSG_PLC_CHL_STATUS_STRU * pstPlcStatus = HI_NULL;
    MRS_STA_SRV_CTX_STRU * sta = mrsStaGetContext();

    pstPlcStatus = (MRS_MSG_PLC_CHL_STATUS_STRU *)pMsgParam;
    if ((usMsgId != ID_MRS_MSG_PLC_CHL_STATUS) || !pstPlcStatus)
    {
        return HI_ERR_FAILURE;
    }

    if (pstPlcStatus->bAvail)
    {
        sta->plcState = HI_TRUE;

        //�ϱ����ַ��MAC��ַ
        mrsStaCheckReport2Cco(MRS_STA_REPORT_METER_ADDR_ACTIVE);

        // TODO��
        mrsStaSyncMacJoinNetAddr();
#if defined(PRODUCT_CFG_SUPPORT_MRS_DUOL_MODE_RF)&&defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
		mrsRfSetAddress();
		mrsRfSetSnidTei();
#endif
    }
    else
    {
        if(mrsStaGetPlcRxTxMode() == MRS_PLC_RX_TX_MODE_FORCE)
        {
            sta->plcState = HI_TRUE;
        }
        else
        {
            sta->plcState = HI_FALSE;
        }
    }

    mrsSrvChlNotifyProc(sta->plcState);

    return ret;
}

//*****************************************************************************
// ��������: mrsDutUartDataPro
// ��������: ���յ���װ�������ݵĴ���
//
// ����˵��:
//   MRS_645_FRAME_STRU pst645Hrd   645֡����
//
// �� �� ֵ:
//
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: fengxiaomin/00209128 [2013-05-30]
//*****************************************************************************
HI_U32 mrsDutUartDataPro(MRS_645_FRAME_STRU *pst645Hrd)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    MRS_DUT_645_FRAME_STRU st645Frame = { 0 };// �Ƿ��޸�MALLOC
    MRS_DUT_645_DI_FUN pDutDIPro = 0;

    //Ϊ��װ645֡��ʽ��ֵ
    st645Frame.ucFrameStart = 0x68;
   (hi_void)memcpy_s(st645Frame.ucAddr, HI_METER_ADDR_LEN, pst645Hrd->ucAddr, sizeof(pst645Hrd->ucAddr));
    st645Frame.ucFrameEnd = 0x68;
   (hi_void)memcpy_s(&st645Frame.stCtrl, sizeof(st645Frame.stCtrl), &pst645Hrd->stCtrl, sizeof(st645Frame.stCtrl));
    st645Frame.ucDataRealmLen = pst645Hrd->ucDataRealmLen;
/* BEGIN: Modified by fengxiaomin/00209182, 2014/6/19   ���ⵥDTS2014071004301 */
	if(MRS_PROTO645_DATA_CS_END_LEN_MAX >= pst645Hrd->ucDataRealmLen){
		(hi_void)memcpy_s(st645Frame.ucData, MRS_PROTO645_DATA_CS_END_LEN_MAX, pst645Hrd->ucDataRealm, pst645Hrd->ucDataRealmLen);	
	}   
    HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1003, HI_DIAG_MT("dut uart rx"), pst645Hrd->ucDataRealm, pst645Hrd->ucDataRealmLen);

    if ((st645Frame.ucDataRealmLen + 1) >= MRS_PROTO645_DATA_CS_END_LEN_MAX)
    {
        return HI_ERR_FAILURE;
    }

    st645Frame.ucData[st645Frame.ucDataRealmLen] =
        mrsToolsCalcCheckSum((HI_PBYTE)&st645Frame, (HI_U16)(st645Frame.ucDataRealmLen + MRS_PROTO645_DATAGRAM_HEADER_LEN_MAX - 2));
    st645Frame.ucData[st645Frame.ucDataRealmLen + 1] = 0x16;
/* END:   Modified by fengxiaomin/00209182, 2014/6/19 */
    pDutDIPro = mrsDutFramePro(&st645Frame);      //��ö�ӦDI��ʶ�Ĵ�����

    if (pDutDIPro != 0)
    {
        ret = pDutDIPro(&st645Frame);//DI��ʶ������
    }

    return ret;
}

// ��ȡ�豸����ID
HI_VOID mrsSta645GetDevId(HI_IN MRS_645_FRAME_STRU *pst645Hrd)
{
	HI_U32 ret = HI_ERR_SUCCESS;
	MRS_645_FRAME_STRU * pframe_info = HI_NULL;
	HI_U16 frame_len = 0;
	HI_PBYTE frame_buff = HI_NULL;  //����֡������
	HI_U8 id_len = 0;
	HI_U8 uc_addr[HI_METER_ADDR_LEN] = {0};
	HI_U8 frame_head[4] = {0};
	MRS_STA_SRV_CTX_STRU *sta = mrsStaGetContext();

    (hi_void)memset_s(uc_addr, HI_METER_ADDR_LEN, MRS_645_METER_ADDR_WILDCARDS, HI_METER_ADDR_LEN);
    (hi_void)memset_s(frame_head, 4, MRS_645_METER_FRAME_HEAD, 4); // 645֡Ҫ��4�ֽڵ�fe֡ͷ

	pframe_info = (MRS_645_FRAME_STRU *)mrsToolsMalloc(sizeof(MRS_645_FRAME_STRU));
	if (pframe_info == HI_NULL) {
		return ;
	}
    (hi_void)memset_s(pframe_info, sizeof(MRS_645_FRAME_STRU), 0,sizeof(MRS_645_FRAME_STRU));

	frame_buff = (HI_PBYTE)mrsToolsMalloc(MRS_PROTO645_DATAGRAM_LEN_MAX + 4);
	if (frame_buff == HI_NULL) {
		mrsToolsFree(pframe_info);
		return ;
	}
    (hi_void)memset_s(frame_buff, MRS_PROTO645_DATAGRAM_LEN_MAX + 4, 0, MRS_PROTO645_DATAGRAM_LEN_MAX + 4);
    (hi_void)memcpy_s(frame_buff, 4, frame_head, 4); // 645֡��4�ֽڵ�fe֡ͷ

	pframe_info->stCtrl.ucDir = MRS_645_FRAME_CONTROL_DIR_RESPONSION_BIT;
	pframe_info->stCtrl.ucFrameFlag = MRS_645_FRAME_CONTROL_AFER_FRAME_NON_BIT;
	pframe_info->stCtrl.ucFn = MRS_645_FRAME_CONTROL_MASK;
    (hi_void)memcpy_s(pframe_info->ucAddr, 6, sta->aucMeter, HI_METER_ADDR_LEN);

	do {
		// ��ȡ��ID���Ͳ�����,���ش���֡
		if (*(pst645Hrd->ucDataRealm) >= MRS_DEV_ID_MAX) {
			ret = MRS_STA_READ_ID_TYPE_ERR;
			break;
		}

		// ��ȡSTA,CLT I,CLT II��ID��Ϣʱ��ֻƥ���ַ������֡�����豸����
		if (memcmp(pst645Hrd->ucAddr, sta->aucMeter, HI_METER_ADDR_LEN) &&
			memcmp(pst645Hrd->ucAddr, uc_addr, HI_METER_ADDR_LEN)) {
			ret = MRS_STA_READ_ID_ADDR_ERR;
			break;
		}

		// ucDataRealm 1�ֽ�ID���� + 1�ֽ�ID���� + ID��Ϣ
		if(*(pst645Hrd->ucDataRealm) == MRS_DEV_ID_CHIP) {
			ret = mrsGetDevIdChip(pframe_info->ucDataRealm + 2, MRS_PROTO645_DATAGRAM_DATA_LEN_MAX - 2);
			//CJJ-HPLC-20190115:оƬID����
			mrsHexInvert(pframe_info->ucDataRealm + 2, MANAGEMENT_ID_SIZE);
			pframe_info->ucDataRealm[0] = MRS_DEV_ID_CHIP;
			id_len = MANAGEMENT_ID_SIZE;
		} else {
			ret = mrsGetDevIdDev(pframe_info->ucDataRealm + 2, MRS_PROTO645_DATAGRAM_DATA_LEN_MAX - 2);
			//CJJ-HPLC-20190115:оƬID����
			mrsHexInvert(pframe_info->ucDataRealm + 2, MRS_DEV_ID_SIZE);
			pframe_info->ucDataRealm[0] = MRS_DEV_ID_DEV;
			id_len = MRS_DEV_ID_SIZE;
		}

		if (ret != HI_ERR_SUCCESS) {
			ret = MRS_STA_READ_ID_FAIL_ERR;
			break;
		}

		// 1�ֽ�ID������Ϣ
		pframe_info->ucDataRealm[1] = id_len;

		//��֯����֡
		pframe_info->stCtrl.ucSlaveFlag = MRS_645_FRAME_CONTROL_RESPONSION_NOMAL_BIT;
		pframe_info->ucDataRealmLen = id_len + 2; //1�ֽ����� + 1�ֽڳ���

		ret = MRS_Proto645Enc(frame_buff + 4, &frame_len, pframe_info);
		if (ret != HI_ERR_SUCCESS) {
			ret = MRS_STA_READ_ID_CREATE_FREAM_ERR;
			break;
		}

		frame_len += 4; // 4�ֽڵ�fe
		ret = MRS_SendMrData(frame_buff, frame_len, HI_DMS_CHL_UART_PORT_APP); // ����"��Ӧ"֡
		if (ret != HI_ERR_SUCCESS) {
			ret = MRS_STA_READ_ID_SEND_FREAM_ERR;
		}
	} while (0);

    if (ret != HI_ERR_SUCCESS) {
    	//��֯����֡
		pframe_info->stCtrl.ucSlaveFlag = MRS_645_FRAME_CONTROL_RESPONSION_ABNORMITY_BIT;
		pframe_info->ucDataRealmLen = MRS_STA_645_READ_ID_INFO_LEN;
		pframe_info->ucDataRealm[0] = (HI_U8)ret;

		ret = MRS_Proto645Enc(frame_buff + 4, &frame_len, pframe_info);
        if (ret == HI_ERR_SUCCESS) {
            frame_len += 4; // 4�ֽڵ�fe
            MRS_SendMrData(frame_buff, frame_len, HI_DMS_CHL_UART_PORT_APP); // ����"��Ӧ"֡
        }
    }

    mrsToolsFree(pframe_info);
    mrsToolsFree(frame_buff);
}

//*****************************************************************************
// ��������: mrsMsgStaUartFrmRx_645
// ��������: ���յ�����ݣ�ת����CCO,��������ڳ�ʼ��ʱ��ע�ᣬ�ڵ��������
//           ���͸�������ʱ�����á�
//
// ����˵��:
//   HI_U16   usMsgId       ��ϢID(��������¿��Ժ���)
//   HI_PVOID pMsgParam     ��������
// �� �� ֵ:
//    �ɹ�����  HI_ERR_SUCCESS
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: niesongsong/nkf62735 [2012-05-29]
//*****************************************************************************
HI_U32 mrsMsgStaUartFrmRx_645(HI_IN HI_U16 usMsgId,HI_IN HI_PVOID pMsgParam)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    MRS_STA_SRV_CTX_STRU* sta = mrsStaGetContext();
    MRS_MSG_MR_645_DATA_STRU* pstData = HI_NULL;
    MRS_645_FRAME_STRU st645Hrd = { 0 };
    MRS_STA_ITEM* item;
    MRS_QUE_MODULE* sta_queue;
    HI_U8 dut_addr[6] = DUT_NORMAL_MODE_ADDR;

    HI_UNREF_PARAM(usMsgId);

    // �����ж�
    if (!pMsgParam) {
        return HI_ERR_FAILURE;
    }

#if defined(PRODUCT_CFG_SYS_RST_SOLUTION)
    sta->stStaResetInfo.ulRxUartTimes++;
#endif

    pstData = (MRS_MSG_MR_645_DATA_STRU*)pMsgParam;
    sta_queue = &sta->stQueModule;

    // ����645֡
    ret = MRS_Proto645Dec(pstData->pucData, pstData->usDataSizse, &st645Hrd);
    pstData->ucProtocol = st645Hrd.ucVer;

    if (ret != HI_ERR_SUCCESS) {
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_010, HI_DIAG_MT("645 err"));
        return HI_ERR_FAILURE;
    }

    // �������򳤶�Ϊ1��������Ϊ0x1F,�ж��Ƕ�ȡ�豸����ID��645֡
    if ((st645Hrd.ucDataRealmLen == 0x01) && (!st645Hrd.stCtrl.ucDir) && (!st645Hrd.stCtrl.ucSlaveFlag) &&
        (!st645Hrd.stCtrl.ucFrameFlag) && (st645Hrd.stCtrl.ucFn == 0x1F)
        && (st645Hrd.ucDataRealm[0] < MRS_DEV_ID_MAX)) //CJJ-HPLC-20190104:��RF��չ��������
    {
		mrsSta645GetDevId(&st645Hrd);
		return HI_ERR_SUCCESS;
    }

    // �ж��Ƿ��ǹ�װ�����Ĳ�ѯmac��ַ�Ͱ汾����Ϣ
    mrsHexInvert(dut_addr, sizeof(dut_addr));
    if (memcmp(st645Hrd.ucAddr, dut_addr, sizeof(st645Hrd.ucAddr)) == 0) {
        mrs645DataDecode(st645Hrd.ucDataRealm, st645Hrd.ucDataRealmLen);
        ret = mrsDutUartDataPro(&st645Hrd);
        return ret;
    }
	
#if defined(PRODUCT_CFG_SUPPORT_MRS_DUOL_MODE_RF)&&defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
	if((0x1F==st645Hrd.stCtrl.ucFn)||((st645Hrd.stCtrl.ucDir==0)&&(0x4==st645Hrd.stCtrl.ucFn)))
	{
		mrsRfDirectTransmit(pstData->pucData, pstData->usDataSizse);
		return ret;
	}
#endif

    item = mrsSrvQueueTop(&sta_queue->stMrQueue);

    if (item && ID_MRS_CMD_TF_PLC == item->id) {
        mrs645DataDecode(st645Hrd.ucDataRealm, st645Hrd.ucDataRealmLen);
        ret = mrsTfStaRxFrame645(&st645Hrd);
        if (HI_ERR_SUCCESS == ret) {
            mrsStaTryDeQueue(sta_queue,mrsStaQueueFree);
        }

        return ret;
    }
    if (mrsToolsIsIICollector()) {
        mrsUARTRTxLedPro();

        if (item
            && (MRS_STA_ITEM_ID_SEARCH_METER_CLT_II == item->id)) {
            if (MRS_CLTII_SEARCH_STATE_DETECT == mrsCltiiSearchGetState()) {
                HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_050, HI_DIAG_MT("645 rx cltii search rx proc"));
                mrsCltiiSearchDetectRxProc(pstData->pucData, pstData->usDataSizse);
                return ret;
            }
        }
        /* BEGIN: Modified by cuichunyan/c00276962 2016/11/10  ���ⵥ��:cltii R485 MultiConnect */
        else {
            if ((mrsCheckRS485Monitor() == HI_TRUE) && (MRS_645_FRAME_CONTROL_DIR_COMMAND_BIT == st645Hrd.stCtrl.ucDir)) {
                mrsRefreshRS485MultiConn();
            }
        }
        /* END  : Modified by cuichunyan/c00276962 2016/11/10  ���ⵥ��:cltii R485 MultiConnect */
    } else {
#if !defined(PRODUCT_CFG_SUPPORT_TEST_CONCENTRATOR)
        MRS_StartTimer(MRS_STA_TIMER_HEART, MRS_STA_TIME_HEART, HI_SYS_TIMER_PERIODIC);
#endif
    }


    do {
        if (item && item->id == MRS_STA_ITEM_ID_SEARCH_METER_CLT_I) {
            break;
        }

        if (st645Hrd.stCtrl.ucFn != 0x13) {
            break;
        }

        //�ж���13 00Ӧ��
        if ((item && (MRS_STA_ITEM_ID_DETECT == item->id))
            || ((MRS_STA_STATE_INIT == mrsStaGetState()) && (st645Hrd.ucDataRealmLen == HI_METER_ADDR_LEN))) {
            /* BEGIN: Added by fengxiaomin/00209182, 2015/10/23   ���ⵥ��:DTS2015102710230 */
            HI_U8 aucAddr[HI_METER_ADDR_LEN] = {0};

            if (HI_METER_ADDR_LEN != st645Hrd.ucDataRealmLen) {
                return HI_ERR_SUCCESS;
            }

            // ���볭��״̬
            MRS_StopTimer(MRS_STA_TIMER_TEST);
            HI_MDM_LED_SetReadMeterAddrStatus(LED_READ_METER_SUCCESS);
           (hi_void)memcpy_s(aucAddr, HI_METER_ADDR_LEN, st645Hrd.ucDataRealm, HI_METER_ADDR_LEN);
            mrs645DataDecode(aucAddr, HI_METER_ADDR_LEN);

            //�ж��ǵ��
            if (mrsToolsNormalAddr(st645Hrd.ucAddr)
                && mrsToolsMemEq(st645Hrd.ucAddr, aucAddr, HI_METER_ADDR_LEN)
                && (!mrsToolsIsCLTI())) {
                mrsStaUpdateAddr(st645Hrd.ucAddr, st645Hrd.ucVer);
                mrsStaCheckReport2Cco(MRS_STA_REPORT_METER_ADDR_ACTIVE);

                // ���½ڵ����ַ��ͬ����Mac��
                mrsStaUpdateTopAddrInf(st645Hrd.ucAddr, st645Hrd.ucVer);

                mrsStaSetMeterAddr(st645Hrd.ucAddr);
                mrsStaSetMeterProtocol(st645Hrd.ucVer);
                mrsStaSetMeterBaudrate((HI_U16)mrsGetCurBaudRate());

                /* ��װģʽ������645Э��ִ�У���̽��698.45 */
                if (st645Hrd.stCtrl.ucSlaveFlag) {
                    mrsStaMeterRxNotify(st645Hrd.ucAddr, st645Hrd.ucVer, HI_TRUE);
                } else {
                    /* ����̽��698.45�� */
                    mrsStaSetFrmProto(MRS_SRV_PROTO_698_45);

                    if (item) {
                        mrsStaStopTtlTimer(item);
                        mrsStaTryDeQueue(sta_queue, mrsStaQueueFree);
                    }

                    mrsStaSearchMeter_698();

                    return HI_ERR_SUCCESS;
                }

            }
#if defined(PRODUCT_CFG_CLTI_SUPPORT_MRS_SWITCH)
            else if (mrsToolsIsSTA()) { //�����ģ�����Ӧ���ַ�Ĳɼ����У�ģ��ʶ��ΪI��ģ��
                // CLT I
                mrsStaSetState(MRS_STA_STATE_MR);
                mrsCltiRx1300Proc(st645Hrd.ucAddr, &st645Hrd.stCtrl);
                mrsStaEvtOutProc();
            }
#endif
            mrsStaSetState(MRS_STA_STATE_MR);

            if (!item) {
                return HI_ERR_SUCCESS;
            }

            break;
        }
    } while (HI_FALSE);
#if defined(PRODUCT_CFG_CLTI_SUPPORT_MRS_SWITCH)
    if (item && item->id == MRS_STA_ITEM_ID_SEARCH_METER_CLT_I) {
        if (st645Hrd.stCtrl.ucFn == 0x13) {
            mrs645DataDecode(st645Hrd.ucDataRealm, st645Hrd.ucDataRealmLen);
            mrsCltiSmFrameRx(&st645Hrd);
            return HI_ERR_SUCCESS;
        } else if ((st645Hrd.stCtrl.ucFn == 0x11) || (st645Hrd.stCtrl.ucFn == 0x01)) {
            mrs645DataDecode(st645Hrd.ucDataRealm, st645Hrd.ucDataRealmLen);
            if (item->option == MRS_CLTI_FRAME_TYPE_POSITIVE_A) {
                mrsCltiTryReadPositiveARx(&st645Hrd);
            }

            return HI_ERR_SUCCESS;
        }
    }
#endif

#if !defined(PRODUCT_CFG_SUPPORT_TEST_CONCENTRATOR)
    // ���±��ַ
    /* BEGIN: PN: DTS2015040900760 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/7 */
    /* BEGIN: PN: DTS2015051601564 MODIFY\ADD\DEL by cuiate/00233580 at 2015/5/15 */
    if (MRS_645_FRAME_CONTROL_DIR_RESPONSION_BIT == st645Hrd.stCtrl.ucDir) {
        if ((METER_PROTO_645_1997 == st645Hrd.ucVer)
            && (MRS_645_FRAME_CONTROL_READ_DATA97 == st645Hrd.stCtrl.ucFn)
            && (st645Hrd.ucDataRealmLen >= sizeof(HI_U16))) {
            HI_U16 usDi97 = HI_MAKEU16(st645Hrd.ucDataRealm[0] - MRS_645_FRAME_HEX33, st645Hrd.ucDataRealm[1] - MRS_645_FRAME_HEX33);
            mrsStaUpdateAddrProc(st645Hrd.ucAddr, st645Hrd.ucVer, usDi97);
        } else {
#if defined(PRODUCT_CFG_CLTI_SUPPORT_MRS_SWITCH)
            if (!mrsIsRealClti() || (st645Hrd.stCtrl.ucFn != 0x13)) {
                mrsStaUpdateAddr(st645Hrd.ucAddr, st645Hrd.ucVer);
            }
#else
			if ((st645Hrd.stCtrl.ucFn != 0x13)) {
                mrsStaUpdateAddr(st645Hrd.ucAddr, st645Hrd.ucVer);
            }
#endif
        }
    }
    /* END:   PN: DTS2015051601564 MODIFY\ADD\DEL by cuiate/00233580 at 2015/5/15 */
    /* END:   PN: DTS2015040900760 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/7 */
#endif

    // �����ǿյģ��յ���������Ч
    if (!item) {
        return HI_ERR_SUCCESS;
    }

    mrsCltRmMeterSetMacProc(item, &st645Hrd);

    if (HI_TRUE == mrsStaCheckDenyRetry(item->id)) {
        HI_U8 ucRetry = mrsGet645DenyRetryCfg(MRS_STA_RM_CFG_AUTO);

        if (item->id == MRS_STA_ITEM_ID_SET_VER_CLT_I) {
            ucRetry = MRS_QUEUE_MAX_TRY - 1;
        }

        // �㳭���ֳ�ʱ���յ�����֡��Ϊ���Ϸ����ض�
        // �ض���Ϊ���ϣ����ϱ��÷��ϵ�CCO
        if ((st645Hrd.stCtrl.ucSlaveFlag) && (sta_queue->retries <= ucRetry)) {
            item->try_max = ucRetry;
            return HI_ERR_SUCCESS;
        }
    }

    if (HI_ERR_SUCCESS != mrsStaFrameRxPreProc(item, pstData->pucData, pstData->usDataSizse, &st645Hrd)) {
        return HI_ERR_CONTINUE;
    }

    // �رճ�ʱ��ʱ��
    mrsStaStopTtlTimer(item);

    switch (item->from) {
        case MRS_STA_QUEUE_FROM_STA:
            if (item->id == MRS_STA_ITEM_ID_EVENT) {
                mrsStaEvtProc(pstData->pucData, pstData->usDataSizse, &st645Hrd);
            } else if ((item->id == MRS_STA_ITEM_ID_CLTII_EVENT) || (item->id == MRS_STA_ITEM_ID_DUTY_CYCLE) || (item->id == MRS_STA_ITEM_ID_CLTI_EVENT)) {
                // �����Ѵ���
            }
#if defined(PRODUCT_CFG_CLTI_SUPPORT_MRS_SWITCH)	
			else if (item->id == MRS_STA_ITEM_ID_SET_VER_CLT_I) {
                mrsCltiSmStartTx();
            }
#endif
			else if (item->id == MRS_STA_ITEM_ID_HEART) {
                mrsStaAddrUpdateProc(st645Hrd.ucAddr);
            } else if (item->id == MRS_STA_ITEM_ID_RM_CLT_II) {
                mrsCltPowerOffRmRxProc();
            } else {
                mrsStaCheckReport2Cco(MRS_STA_REPORT_METER_ADDR_ACTIVE);
            }
            break;

        case MRS_STA_QUEUE_FROM_PLC:
            if (item->id == ID_MRS_CMD_PARALLEL_DATA_PLC) {
                mrsDfxPrUartRx();
                /* BEGIN: Modified by fengxiaomin/00209182, 2014/3/27   ���ⵥ��:DTS2014032906596 */
                mrsStaDfxRmUartRx(item->id, pstData->pucData, pstData->usDataSizse);
                mrsStaDfxRmRxDeny(item->id, st645Hrd.stCtrl.ucSlaveFlag);
                mrsStaDfxRmRxErrData(item->id, st645Hrd.ucDataRealm, st645Hrd.ucDataRealmLen);
                /* END:   Modified by fengxiaomin/00209182, 2014/3/27 */

                mrsParallelRefreshRespFlg(HI_TRUE);
                mrsParallelRefreshFrameStatus(st645Hrd.stCtrl.ucSlaveFlag);

                ret = mrsParallelMutliFrameProcess(item,
                                                   pstData->pucData,
                                                   pstData->usDataSizse,
                                                   pstData->ucProtocol);
                if (ret == HI_ERR_CONTINUE) {
                    return ret;
                }
            } else {
                if (item->id == ID_MRS_CMD_DATA_TRANSMIT_AFN13_PLC) {
                    mrsDfxXrUartRx();
                } else if (item->id == ID_MRS_CMD_DATA_TRANSMIT_AFN14_PLC) {
                    mrsDfxLrMeterRx();
                }

                /* BEGIN: Modified by fengxiaomin/00209182, 2014/3/27   ���ⵥ��:DTS2014032906596 */
                mrsStaDfxRmUartRx(item->id, pstData->pucData, pstData->usDataSizse);
                mrsStaDfxRmRxDeny(item->id, st645Hrd.stCtrl.ucSlaveFlag);
                mrsStaDfxRmRxErrData(item->id, st645Hrd.ucDataRealm, st645Hrd.ucDataRealmLen);
                /* END:   Modified by fengxiaomin/00209182, 2014/3/27 */

                mrsStaSerialRespRMPLC(item, pstData);
            }
            mrsStaAddrUpdateProc(st645Hrd.ucAddr);
            break;

        case MRS_STA_QUEUE_FROM_LOCAL:
            if (item->id == MRS_STA_ITEM_ID_TRAN_DATA_CLT_II) {
                mrsRS485TranDataInd(pstData->pucData,
                                    pstData->usDataSizse,
                                    HI_DIAG_CMD_INSTANCE_LOCAL);
            } else {
                HI_MDM_ReportPacket(HI_STA_TRANSMIT,
                                    HI_DIAG_CMD_INSTANCE_LOCAL,
                                    pstData->pucData,
                                    pstData->usDataSizse,
                                    HI_TRUE);
            }

            break;

        case MRS_STA_QUEUE_FROM_REMOTE:
            mrsStaDiag2HSO(pstData->pucData,
                           pstData->usDataSizse,
                           HI_DIAG_CMD_INSTANCE_IREMOTE);
            break;

        case MRS_STA_QUEUE_FROM_REMOTE_HSO:
            if (item->id == MRS_STA_ITEM_ID_TRAN_DATA_CLT_II) {

                mrsRS485TranDataInd(pstData->pucData,
                                    pstData->usDataSizse,
                                    HI_DIAG_CMD_INSTANCE_IREMOTE);
            } else {
                HI_MDM_ReportPacket(HI_STA_TRANSMIT,
                                    HI_DIAG_CMD_INSTANCE_IREMOTE,
                                    pstData->pucData,
                                    pstData->usDataSizse,
                                    HI_TRUE);
            }
            break;

        case MRS_STA_QUEUE_FROM_IR:
            /* BEGIN: PN: DTS2015102007386  MODIFY\ADD\DEL by cuichunyan/00276962 at 2015/10/08 */
            mrsIrTransmitUp(item->sub_id, pstData->pucData, pstData->usDataSizse);
            /* END: PN: DTS2015102007386  MODIFY\ADD\DEL by cuichunyan/00276962 at 2015/10/08 */

        break;
	case MRS_STA_QUEUE_FROM_RF:
		mrsRfDirectTransmit(pstData->pucData, pstData->usDataSizse);
		break;

        default:
            break;
    }

    // ������
    return mrsStaTryDeQueue(sta_queue, mrsStaQueueFree);
}


HI_U32 mrsStaFrameRxPreProc(MRS_STA_ITEM * pstItem, HI_U8 *pData, HI_U16 usLen, MRS_645_FRAME_STRU *pst645Hrd)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;

    if (!pstItem || !pst645Hrd)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    if (MRS_STA_QUEUE_FROM_STA == pstItem->from)
    {
        if (pstItem->id == MRS_STA_ITEM_ID_CLTII_EVENT)
        {
            ulRet = mrsCltiiEventFrameRx(pData, usLen);
        }
		#if defined(PRODUCT_CFG_CLTI_SUPPORT_MRS_SWITCH)
        else if (pstItem->id == MRS_STA_ITEM_ID_CLTI_EVENT)
        {
            ulRet = mrsCltiEventFrameRx(pData, usLen, pstItem->sub_id);
        }
		#endif
        else if (pstItem->id == MRS_STA_ITEM_ID_EVENT)
        {
            ulRet = mrsStaCheckDIValid(MRS_STA_EVT_STATUS_WORD_DI, pst645Hrd);
        }
        else if (pstItem->id == MRS_STA_ITEM_ID_DUTY_CYCLE)
        {
            ulRet = mrsStaDutyCycleFrameRx(pData, usLen);
        }
#if defined(PRODUCT_CFG_CLTI_SUPPORT_MRS_SWITCH)
        else if (pstItem->id == MRS_STA_ITEM_ID_SET_VER_CLT_I)
        {
            if (MRS_645_FRAME_CONTROL_READ_ADDR != pst645Hrd->stCtrl.ucFn)
            {
                return HI_ERR_BAD_DATA;
            }

            ulRet = mrsStaCheckDIValid(MRS_CLTI_SET_VER_DI, pst645Hrd);
        }
#endif
    }
    else if (MRS_STA_QUEUE_FROM_PLC == pstItem->from)
    {
        HI_U8 *pucFrame = pstItem->buf;
        HI_U16 usLength = pstItem->len;

        if (ID_MRS_CMD_PARALLEL_DATA_PLC == pstItem->id)
        {
            MRS_PARALLEL_STA_CTX * pstParallelCtx = mrsStaGetParallelCtx();

            MRS_PARALLEL_GET_TX_BUF(pstParallelCtx, pucFrame);
            MRS_PARALLEL_GET_TX_LEN(pstParallelCtx, usLength);
        }

        if (!mrsStaIsMrFrameMatch(pucFrame, usLength, pData, usLen, pstItem->ucProtocol))
        {
            if (pstItem->bAutoStrategy && pstItem->bTtlTimerFlg)
            {
                pstItem->bRcvByteFlg = HI_FALSE;
            }

            ulRet = HI_ERR_CONTINUE;
        }
    }

    return ulRet;
}

HI_VOID mrsStaAddrUpdateProc(HI_U8 aucAddr[])
{
	HI_U32 ret = HI_ERR_SUCCESS;
    HI_U8 ucIp[HI_IPV4_ADDR_LEN] = {0};
	HI_U8 ucCurrMac[HI_PLC_MAC_ADDR_LEN] = {0};

#if defined(PRODUCT_CFG_CLTI_SUPPORT_MRS_SWITCH)
	if (!mrsToolsIsMeter() || mrsIsRealClti())
	{
		return;
	}
#else
	if (!mrsToolsIsMeter())
	{
		return;
	}
#endif


    ret = HI_MDM_NM_GetAddr(ucCurrMac, ucIp);
    if(ret != HI_ERR_SUCCESS)
    {
        return;
    }

    mrsHexInvert(aucAddr, HI_METER_ADDR_LEN);

    if (HI_MDM_PTCL_VER_100 == HI_MDM_GetProtocolVer())
    {
        MRS_TOOLS_FE_TO_00(ucCurrMac[HI_PLC_MAC_ADDR_LEN - 1]);
    }

    if (mrsToolsMemEq(aucAddr, ucCurrMac, HI_PLC_MAC_ADDR_LEN))
	{
		return;
	}

    mrsStaSetMeterToMac(aucAddr, STA_TYPE_METER, HI_MDM_METER_MAC);
}

HI_VOID mrsSta698AddrUpdateProc(HI_U8 *pucData, HI_U16 usDataSizse)
{
	HI_U32 ulRet = HI_ERR_SUCCESS;
	HI_U8 aucMeter[HI_METER_ADDR_LEN] = {0};

	ulRet = mrsFind698MeterAddr(pucData, usDataSizse, aucMeter);
	if (HI_ERR_SUCCESS != ulRet)
	{
		return;
	}

	mrsStaAddrUpdateProc(aucMeter);
}

HI_U32 mrsMsgStaUartFrmRx_698_45(HI_U16 usMsgId, HI_PVOID pMsgParam)
{
    MRS_STA_SRV_CTX_STRU* pstStaCtx = mrsStaGetContext();
    MRS_QUE_MODULE* pstStaQueue = HI_NULL;
    MRS_STA_ITEM* pstItem = HI_NULL;
    HI_U32 ulRet = HI_ERR_SUCCESS;
    MRS_MSG_MR_645_DATA_STRU* pstData = HI_NULL;

    HI_UNREF_PARAM(usMsgId);

    if (!pMsgParam) {
        return HI_ERR_FAILURE;
    }

    HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_004, HI_DIAG_MT("** mrsMsgStaUartFrmRx_698_45 **"));

#if defined(PRODUCT_CFG_SYS_RST_SOLUTION)
    pstStaCtx->stStaResetInfo.ulRxUartTimes++;
#endif

    pstData = (MRS_MSG_MR_645_DATA_STRU*)pMsgParam;
    pstData->ucProtocol = METER_PROTO_698_45;

    pstStaQueue = &pstStaCtx->stQueModule;
    pstItem = mrsSrvQueueTop(&pstStaQueue->stMrQueue);

    if (mrsToolsIsIICollector()) {
        mrsUARTRTxLedPro();
        // TODO: II�ɴ���
    } else {
#if !defined(PRODUCT_CFG_SUPPORT_TEST_CONCENTRATOR)
        MRS_StartTimer(MRS_STA_TIMER_HEART, MRS_STA_TIME_HEART, HI_SYS_TIMER_PERIODIC);
#endif
    }

    if (!pstItem) {
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_005, HI_DIAG_MT("** mrsMsgStaUartFrmRx_698_45 err**"));
        return HI_ERR_FAILURE;
    }

    switch (pstItem->id) {
        case MRS_STA_ITEM_ID_DETECT: {
            HI_U8 aucMeter[HI_METER_ADDR_LEN] = {0};

            ulRet = mrsFind698MeterAddr(pstData->pucData, pstData->usDataSizse, aucMeter);
            if (HI_ERR_SUCCESS != ulRet) {
                return HI_ERR_FAILURE;
            }

            ulRet = mrsStaUpdateMeterInf698(aucMeter);
            if (HI_ERR_SUCCESS != ulRet) {
                return HI_ERR_FAILURE;
            }

            HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1006, HI_DIAG_MT("METER ADDR"), aucMeter, HI_METER_ADDR_LEN);

            MRS_StopTimer(MRS_STA_TIMER_TEST);
            mrsStaSetState(MRS_STA_STATE_MR);
        }
        break;

        default:
            break;
    }
//	mrsCltiiClearLastBcMeterInf();  //Matteo
	MRS_CLTII_LAST_BC_METER_INF *pstLastInf = mrsCltiiGetLastBcMeterInf(); //Matteo
	pstLastInf->ucCount = 0; //Matteo
	
    mrsStaStopTtlTimer(pstItem);

    switch (pstItem->from) {
        case MRS_STA_QUEUE_FROM_PLC:
            if (ID_MRS_CMD_PARALLEL_DATA_PLC == pstItem->id) {
                mrsDfxPrUartRx();
                mrsStaDfxRmUartRx(pstItem->id, pstData->pucData, pstData->usDataSizse);
                mrsParallelRefreshRespFlg(HI_TRUE);
                mrsParallelRefreshFrameStatus(0);

                ulRet = mrsParallelMutliFrameProcess(pstItem, pstData->pucData, pstData->usDataSizse, pstData->ucProtocol);
                if (HI_ERR_CONTINUE == ulRet) {
                    return ulRet;
                }
            } else {
                if (ID_MRS_CMD_DATA_TRANSMIT_AFN13_PLC == pstItem->id) {
                    mrsDfxXrUartRx();
                } else if (ID_MRS_CMD_DATA_TRANSMIT_AFN14_PLC == pstItem->id) {
                    mrsDfxLrMeterRx();
                }

                mrsStaDfxRmUartRx(pstItem->id, pstData->pucData, pstData->usDataSizse);
                mrsStaSerialRespRMPLC(pstItem, pstData);
            }
            mrsSta698AddrUpdateProc(pstData->pucData, pstData->usDataSizse);
            break;

        case MRS_STA_QUEUE_FROM_LOCAL:
            HI_MDM_ReportPacket(HI_STA_TRANSMIT, HI_DIAG_CMD_INSTANCE_LOCAL, pstData->pucData, pstData->usDataSizse, HI_TRUE);
            break;

        case MRS_STA_QUEUE_FROM_REMOTE:
            mrsStaDiag2HSO(pstData->pucData, pstData->usDataSizse, HI_DIAG_CMD_INSTANCE_IREMOTE);
            break;

        case MRS_STA_QUEUE_FROM_REMOTE_HSO:
            HI_MDM_ReportPacket(HI_STA_TRANSMIT, HI_DIAG_CMD_INSTANCE_IREMOTE, pstData->pucData, pstData->usDataSizse, HI_TRUE);
            break;

        case MRS_STA_QUEUE_FROM_IR:
            /* BEGIN: PN: DTS2015102007386  MODIFY\ADD\DEL by cuichunyan/00276962 at 2015/10/08 */
            mrsIrTransmitUp(pstItem->sub_id, pstData->pucData, pstData->usDataSizse);
            /* END: PN: DTS2015102007386  MODIFY\ADD\DEL by cuichunyan/00276962 at 2015/10/08 */

            break;

        case MRS_STA_QUEUE_FROM_STA:
            if (MRS_STA_ITEM_ID_HEART == pstItem->id) {
                mrsSta698AddrUpdateProc(pstData->pucData, pstData->usDataSizse);
            } else if (MRS_STA_ITEM_ID_RM_CLT_II == pstItem->id) {
                mrsCltPowerOffRmRxProc();
            }

            break;

        default:
            break;
    }

    return mrsStaTryDeQueue(pstStaQueue, mrsStaQueueFree);
}


HI_U32 mrsStaUpdateMeterInf698(HI_U8 aucAddr[HI_METER_ADDR_LEN])
{
    MRS_STA_SRV_CTX_STRU *pstStaCtx = HI_NULL;
    MRS_METER_LIST_S *pstMeterList = HI_NULL;
    MRS_METER_ITEM_S *pstMeterItem = HI_NULL;
    HI_U8 aucDevAddr[HI_METER_ADDR_LEN] = {0};

    if (!mrsToolsNormalAddr(aucAddr))
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    pstStaCtx = mrsStaGetContext();
    pstMeterList = &pstStaCtx->stMeterList;

    (hi_void)memset_s(pstMeterList, sizeof(MRS_METER_LIST_S), 0, sizeof(MRS_METER_LIST_S));
    pstMeterList->ucMeterNum = 1;
    pstMeterList->ucType = MRS_STA_TYPE_METER;

    pstMeterItem = &pstMeterList->stNodeMeter;
    pstMeterItem->ucValidFlag = 1;
    pstMeterItem->ucProtocol = METER_PROTO_698_45;
    pstMeterItem->usOption = MRS_STA_LOCAL_MAP_ENTRY_TTL_MAX;
   (hi_void)memcpy_s(pstMeterItem->ucMeterAddr, HI_METER_ADDR_LEN, aucAddr, HI_METER_ADDR_LEN);
    pstMeterItem->usBaudRate = (HI_U16)mrsGetCurBaudRate();

   (hi_void)memcpy_s(&pstMeterList->stMeterItem[0], sizeof(MRS_METER_ITEM_S), pstMeterItem, sizeof(MRS_METER_ITEM_S));

    mrsStaRefreshTopNodeInf(aucAddr, METER_PROTO_698_45);

    mrsSetSyncMacFlg(HI_TRUE);
   (hi_void)memcpy_s(aucDevAddr, HI_METER_ADDR_LEN, aucAddr, HI_METER_ADDR_LEN);
    mrsHexInvert(aucDevAddr, HI_METER_ADDR_LEN);
    mrsStaSetMeterToMac(aucDevAddr, STA_TYPE_METER, HI_MDM_METER_MAC);
    mrsStaSetType(STA_TYPE_METER);
    HI_MDM_DevIdConfig(aucDevAddr);

    mrsStaSetMeterAddr(aucAddr);
    mrsStaSetMeterProtocol(METER_PROTO_698_45);
    mrsStaMeterRxNotify(aucAddr, METER_PROTO_698_45, HI_FALSE);

    return HI_ERR_SUCCESS;
}


HI_U32 mrsMsgStaUartFrmRx_Trans(HI_U16 usMsgId, HI_PVOID pMsgParam)
{
    MRS_STA_SRV_CTX_STRU *pstStaCtx = mrsStaGetContext();
    MRS_QUE_MODULE * pstStaQueue = HI_NULL;
    MRS_STA_ITEM * pstItem = HI_NULL;
    HI_U32 ulRet = HI_ERR_SUCCESS;
    MRS_MSG_MR_645_DATA_STRU *pstData = HI_NULL;

    HI_UNREF_PARAM(usMsgId);

    if (!pMsgParam)
    {
        return HI_ERR_FAILURE;
    }

    HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_011, HI_DIAG_MT("** mrsMsgStaUartFrmRx_Trans **"));

#if defined(PRODUCT_CFG_SYS_RST_SOLUTION)
    pstStaCtx->stStaResetInfo.ulRxUartTimes++;
#endif

    pstData = (MRS_MSG_MR_645_DATA_STRU *)pMsgParam;
    pstData->ucProtocol = METER_PROTO_TRANSPARENT;

    pstStaQueue = &pstStaCtx->stQueModule;
    pstItem = mrsSrvQueueTop(&pstStaQueue->stMrQueue);

    if (mrsToolsIsIICollector())
    {
        mrsUARTRTxLedPro();
        // TODO: II�ɴ���
    }
    else
    {
#if !defined(PRODUCT_CFG_SUPPORT_TEST_CONCENTRATOR)
        MRS_StartTimer(MRS_STA_TIMER_HEART, MRS_STA_TIME_HEART, HI_SYS_TIMER_PERIODIC);
#endif
    }

    if (!pstItem)
    {
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_012, HI_DIAG_MT("** mrsMsgStaUartFrmRx_Trans err**"));
        return HI_ERR_FAILURE;
    }
	mrsCltiiClearLastBcMeterInf();
    mrsStaStopTtlTimer(pstItem);

    switch (pstItem->from)
    {
    case MRS_STA_QUEUE_FROM_PLC:
        if (ID_MRS_CMD_PARALLEL_DATA_PLC == pstItem->id)
        {
            mrsDfxPrUartRx();
            mrsStaDfxRmUartRx(pstItem->id, pstData->pucData, pstData->usDataSizse);
            mrsParallelRefreshRespFlg(HI_TRUE);
            mrsParallelRefreshFrameStatus(0);

            ulRet = mrsParallelMutliFrameProcess(pstItem, pstData->pucData, pstData->usDataSizse, pstData->ucProtocol);
            if (HI_ERR_CONTINUE == ulRet)
            {
                return ulRet;
            }
        }
        else
        {
            if (ID_MRS_CMD_DATA_TRANSMIT_AFN13_PLC == pstItem->id)
            {
                mrsDfxXrUartRx();
            }
            else if (ID_MRS_CMD_DATA_TRANSMIT_AFN14_PLC == pstItem->id)
            {
                mrsDfxLrMeterRx();
            }

            mrsStaDfxRmUartRx(pstItem->id, pstData->pucData, pstData->usDataSizse);
            mrsStaSerialRespRMPLC(pstItem, pstData);
        }
        break;

    case MRS_STA_QUEUE_FROM_LOCAL:
        HI_MDM_ReportPacket(HI_STA_TRANSMIT, HI_DIAG_CMD_INSTANCE_LOCAL, pstData->pucData, pstData->usDataSizse, HI_TRUE);
        break;

    case MRS_STA_QUEUE_FROM_REMOTE:
        mrsStaDiag2HSO(pstData->pucData, pstData->usDataSizse, HI_DIAG_CMD_INSTANCE_IREMOTE);
        break;

    case MRS_STA_QUEUE_FROM_REMOTE_HSO:
        HI_MDM_ReportPacket(HI_STA_TRANSMIT, HI_DIAG_CMD_INSTANCE_IREMOTE, pstData->pucData, pstData->usDataSizse, HI_TRUE);
        break;

    case MRS_STA_QUEUE_FROM_IR:
/* BEGIN: PN: DTS2015102007386  MODIFY\ADD\DEL by cuichunyan/00276962 at 2015/10/08 */
        mrsIrTransmitUp(pstItem->sub_id, pstData->pucData, pstData->usDataSizse);
/* END: PN: DTS2015102007386  MODIFY\ADD\DEL by cuichunyan/00276962 at 2015/10/08 */

        break;
	case MRS_STA_QUEUE_FROM_RF:
		mrsRfDirectTransmit(pstData->pucData, pstData->usDataSizse);
		break;


    default:
        break;
    }

    return mrsStaTryDeQueue(pstStaQueue, mrsStaQueueFree);
}


HI_U32 mrsCheckCltRmMeterToMac(MRS_STA_ITEM *pstItem, HI_U8 ucFn)
{
    MRS_STA_SRV_CTX_STRU *sta = mrsStaGetContext();

    if (EQUIP_DUT_IS_EQUIP_MODE() || (0x13 == ucFn) || sta->stRmMeterInfo.bValid)
    {
        return HI_ERR_SKIP;
    }
#if defined(PRODUCT_CFG_CLTI_SUPPORT_MRS_SWITCH)
    if ((!mrsIsRealClti() && !mrsToolsIsIICollector()) || (HI_TRUE == mrsGetSyncMacFlg()))
    {
        return HI_ERR_SKIP;
    }
#else
	if ( !mrsToolsIsIICollector() ||(HI_TRUE == mrsGetSyncMacFlg()))
	{
		return HI_ERR_SKIP;
	}
#endif


    if ((pstItem->id < ID_MRS_CMD_DATA_TRANSMIT_AFN13_PLC) || (pstItem->id > ID_MRS_CMD_PARALLEL_DATA_PLC))
    {
        return HI_ERR_SKIP;
    }

    return HI_ERR_SUCCESS;
}

HI_U32 mrsCltRmMeterSetMacProc(MRS_STA_ITEM *pstItem, MRS_645_FRAME_STRU *pst645Hrd)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U8 ucProtocol = mrsToolsGet645Ver(pst645Hrd->stCtrl.ucFn);
    HI_U8 aucMeter[HI_METER_ADDR_LEN] = {0};
    MRS_STA_SRV_CTX_STRU *sta = mrsStaGetContext();

    mrsMeterAddrPreProc(aucMeter, pst645Hrd->ucAddr, ucProtocol);

    //���BCD�뼰�㲥��ַ
    if (!mrsToolsNormalAddr(aucMeter))
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    ulRet = mrsCheckCltRmMeterToMac(pstItem, pst645Hrd->stCtrl.ucFn);
    if (HI_ERR_SUCCESS != ulRet)
    {
        return ulRet;
    }

    sta->stRmMeterInfo.bValid = HI_TRUE;
    sta->stRmMeterInfo.usId = pstItem->id;
   (hi_void)memcpy_s(sta->stRmMeterInfo.aucAddr, HI_METER_ADDR_LEN, pst645Hrd->ucAddr, HI_METER_ADDR_LEN);
    sta->stRmMeterInfo.ucProtocol = ucProtocol;
    if (METER_PROTO_645_1997 == sta->stRmMeterInfo.ucProtocol)
    {
       (hi_void)memcpy_s(&sta->stRmMeterInfo.us97DI, sizeof(HI_U16), pst645Hrd->ucDataRealm, MRS_645_FRAME_DATA_DI_SIZE_V97);
        mrs645DataDecode((HI_U8*)&sta->stRmMeterInfo.us97DI, MRS_645_FRAME_DATA_DI_SIZE_V97);
    }

    if (sta->ucDutFlag)
    {
        MRS_StartTimer(MRS_STA_TIMER_RM_METER_JOIN_NET, MRS_STA_TIME_RM_METER_JOIN_NET_DUT, HI_SYS_TIMER_ONESHOT);
        return HI_ERR_SUCCESS;
    }

    return MRS_StartTimer(MRS_STA_TIMER_RM_METER_JOIN_NET, MRS_STA_TIME_RM_METER_JOIN_NET, HI_SYS_TIMER_ONESHOT);
}

HI_VOID mrsCltRmMeterSetMacTimeout(HI_VOID)
{
    MRS_STA_SRV_CTX_STRU *sta = mrsStaGetContext();
    MRS_QUE_MODULE *pstModule = &sta->stQueModule;
    MRS_SRV_QUEUE *pstQueue = &pstModule->stMrQueue;
    MRS_STA_ITEM * pstItem = mrsSrvQueueTop(pstQueue);
    MRS_STA_RM_METER_INFO *pstRmMeterInfo = &sta->stRmMeterInfo;

    if (EQUIP_DUT_IS_EQUIP_MODE() || (HI_TRUE == mrsGetSyncMacFlg()))
    {
        return;
    }
#if defined(PRODUCT_CFG_CLTI_SUPPORT_MRS_SWITCH)
    if (mrsIsRealClti())
    {
        MRS_StopTimer(MRS_STA_TIMER_TEST);
        if (pstItem && (MRS_STA_ITEM_ID_DETECT == pstItem->id))
        {
            mrsStaTryDeQueue(pstModule, mrsStaQueueFree);
        }
    }
#endif
    mrsStaSearchSetMeter2Mac(pstRmMeterInfo->aucAddr, pstRmMeterInfo->ucProtocol, pstRmMeterInfo->us97DI);
}


//*****************************************************************************
// ��������: mrsStaCheckDIValid
// ��������: STA���յ����Ӧ��󣬼��DI�Ƿ���ȷ�����з���֡��Ϊ��ȷ
//
// ����˵��:
//   ulDestDI    Ԥ��DI
//   pst645Hrd   �յ���645��Ϣ
//
// �� �� ֵ:
//    HI_ERR_SUCCESS  ��ȷ
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: fengxiaomin/f00209182 [2015-10-30]
//*****************************************************************************
HI_U32 mrsStaCheckDIValid(HI_U32 ulDestDI, MRS_645_FRAME_STRU *pst645Hrd)
{
    HI_U32 ulDI = 0;

    if (pst645Hrd->stCtrl.ucSlaveFlag)
    {
        return HI_ERR_SUCCESS;
    }

   (hi_void)memcpy_s(&ulDI, sizeof(ulDI), pst645Hrd->ucDataRealm, sizeof(ulDI));
    mrs645DataDecode((HI_U8*)&ulDI, sizeof(ulDI));

    if (ulDI != ulDestDI)
    {
        return HI_ERR_BAD_DATA;
    }

    return HI_ERR_SUCCESS;

}

//*****************************************************************************
// ��������: mrsStaCheckDenyRetry
// ��������: STA���յ�����֡���Ƿ���Ҫ�ط�
//
// ����˵��:
//   usId     ������Ԫ��id
//
// �� �� ֵ:
//    ��Ҫ�ط�����  HI_TRUE
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: fengxiaomin/f00209182 [2015-07-18]
//*****************************************************************************
HI_BOOL mrsStaCheckDenyRetry(HI_U16 usId)
{
    if (mrsToolsIsIICollector() && (HI_TRUE == mrsIsCltiiTiming()))
    {
        return HI_FALSE;
    }
    else
    {
        if(usId == MRS_STA_ITEM_ID_SET_VER_CLT_I)
        {
            return HI_TRUE;
        }

        if(((usId == ID_MRS_CMD_DATA_TRANSMIT_AFN13_PLC) || (usId == ID_MRS_CMD_DATA_TRANSMIT_AFN14_PLC))
            && (mrsGet645DenyRetryCfg(MRS_STA_RM_CFG_AUTO)))
        {
            return HI_TRUE;
        }
    }

    return HI_FALSE;
}

//*****************************************************************************
// ��������: mrsStaSerialRespRMPLC
// ��������: �㳭�ֳ�ʱ�յ�����Ӧ���,�ϱ���CCO
//
// ����˵��:
//   pstItem      ����ͷ�ڵ���Ϣ
//   pstData      �յ��ĵ��������Ϣ
//
// �� �� ֵ:
//    �ɹ�����  HI_ERR_SUCCESS
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: fengxiaomin/00209182 [2015-07-20]
//*****************************************************************************
HI_U32 mrsStaSerialRespRMPLC(MRS_STA_ITEM *pstItem, MRS_MSG_MR_645_DATA_STRU* pstData)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U8 *pData = HI_NULL;
    HI_U16 usDataLen = 0;
    HI_U8 ucProtocol = 0;

    HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_053, HI_DIAG_MT("StaSerialRespRMPLC"));
    if (HI_TRUE == mrsIsCltiiTiming())
    {
        ulRet = mrsCreateTimingUpFrame(&pData, &usDataLen, pstData);
        HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1010, HI_DIAG_MT("TimingUpFrame"), pData, usDataLen);
        ucProtocol = mrsGetCollectorCtx()->ucProtoType;
        HI_DIAG_LOG_MSG_E3(MRS_FILE_LOG_FLAG_054, HI_DIAG_MT("timingRespPLC"),pstData,usDataLen,ucProtocol);
    }
    else
    {
        if (HI_NULL == pstData)
        {
            return HI_ERR_FAILURE;
        }

        pData = pstData->pucData;
        usDataLen = pstData->usDataSizse;
        ucProtocol = pstData->ucProtocol;
    }

    if (HI_ERR_SUCCESS == ulRet)
    {
        ulRet = mrsStaTransmit2CCO(pData, usDataLen, ucProtocol, pstItem);
        HI_DIAG_LOG_MSG_E3(MRS_FILE_LOG_FLAG_056, HI_DIAG_MT("send timingRespPLC"),pData,usDataLen,ulRet);
        mrsStaFrameBufInsert(pstItem, usDataLen, pData, ucProtocol);
#if defined(PRODUCT_CFG_SUPPORT_TEST_CONCENTRATOR)
        mrsStaTestModeWhiteListInsert(pData, usDataLen);
#endif
    }

    if (HI_TRUE == mrsIsCltiiTiming())
    {
        mrsToolsFree(pData);
    }

    return ulRet;
}

//*****************************************************************************
// ��������: mrsStaUpdateAddr
// ��������: STA�Ĳ�ѯ�õ��˵��Ļظ��Ӷ�����ȫ�������еĵ���ַ��¼
//
// ����˵��:
//   HI_U8 meterAddr[]      ����ַ
//   int len                ���ȣ�һ��Ϊ6
//   HI_U8 ucProtocol       ����Э������
// �� �� ֵ:
//    �ɹ�����  HI_ERR_SUCCESS
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: niesongsong/nkf62735 [2012-05-29]
//*****************************************************************************
/* BEGIN: PN: DTS2015051601564 MODIFY\ADD\DEL by cuiate/00233580 at 2015/5/15 */
HI_U32 mrsStaUpdateAddr(HI_U8 meterAddr[], HI_U8 ucProtocol)
{
    return mrsStaUpdateAddrProc(meterAddr, ucProtocol, 0);
}


HI_U32 mrsStaUpdateAddrProc(HI_U8 meterAddr[], HI_U8 ucProtocol, HI_U16 usDi97)
{
    MRS_METER_LIST_S * pMeterList;
    MRS_METER_ITEM_S * pMeter;
    MRS_STA_SRV_CTX_STRU *sta = mrsStaGetContext();
    HI_U8 index = 0;
    HI_U8 mintll = 0;
    HI_S8 found = -1;
    HI_PRV HI_BOOL first = HI_TRUE;
    HI_BOOL report_flag = HI_FALSE;
    HI_U8 aucMeterAddr[HI_METER_ADDR_LEN] = {0};
    HI_U8 aucTempAddr[HI_METER_ADDR_LEN] = {0};
    HI_U16 usBaudRate = 0;
    HI_BOOL bUpdated = HI_FALSE;

    if (meterAddr == HI_NULL)
    {
        return HI_ERR_FAILURE;
    }

    mrsMeterAddrPreProc(aucMeterAddr, meterAddr, ucProtocol);

    //���BCD�뼰�㲥��ַ
    if (!mrsToolsNormalAddr(aucMeterAddr))
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    //���ҵ��
    pMeterList = & sta->stMeterList;
    for (index = 0;index < PRODUCT_CFG_MRS_MAX_METER_IN_MAC_NUM;index++)
    {
        pMeter = & (pMeterList->stMeterItem[index]);

        mrsMeterAddrPreProc(aucTempAddr, pMeter->ucMeterAddr, pMeter->ucProtocol);
        if (mrsToolsMemEq(aucTempAddr, aucMeterAddr, HI_METER_ADDR_LEN))
        {
            found = (HI_S8)index;
        }

        if (pMeter->usOption < pMeterList->stMeterItem[mintll].usOption)
        {
            mintll = index;
        }

        //5.2 ��̭ԭ��,TTLά������ʹ�ô���Ϊά��ָ��
        if (pMeter->usOption != 0)
        {
            pMeter->usOption--;
        }
    }

    //������ڣ�ȡTTL��С��λ��
    if (found == -1)
    {
        found = (HI_S8)mintll;
        if (!first)
        {
            report_flag = HI_TRUE;
        }

        first = HI_FALSE;
        if (mrsToolsIsIICollector())
        {
            mrsStaSearchAddRmMeter(meterAddr, ucProtocol, usDi97);
            bUpdated = HI_TRUE;
        }
    }

    //���µ��
    pMeter = & pMeterList->stMeterItem[found];
   (hi_void)memcpy_s(pMeter->ucMeterAddr, HI_METER_ADDR_LEN, meterAddr, HI_METER_ADDR_LEN);

    //����TTLֵ
    //�������ַ��Ч
    if (pMeter->ucValidFlag == 0)
    {
        pMeterList->ucMeterNum ++;
        pMeter->ucValidFlag = 1;
    }

    pMeter->usOption = MRS_STA_LOCAL_MAP_ENTRY_TTL_MAX;
    if (ucProtocol > pMeter->ucProtocol)
    {
        pMeter->ucProtocol = ucProtocol;
    }
    usBaudRate = (HI_U16)mrsGetCurBaudRate();

    if ((!bUpdated) && mrsToolsIsIICollector() && (pMeter->usBaudRate != usBaudRate))
    {
        mrsStaSearchAddRmMeter(meterAddr, ucProtocol, usDi97);
    }

    pMeter->usBaudRate = usBaudRate;

    mrsCltiiClearLastBcMeterInf();

    if (report_flag)
    {
        //���ϱ����ַ�Ĳ���
        mrsStaCheckReport2Cco(MRS_STA_REPORT_METER_ADDR_CHANGE);
    }

    return HI_ERR_SUCCESS;
}
/* END:   PN: DTS2015051601564 MODIFY\ADD\DEL by cuiate/00233580 at 2015/5/15 */

//*****************************************************************************
// ��������: mrsStaHeart
// ��������: �������������Ͳ�ѯ��645֡
//
// ����˵��:
//   HI_U8 meterAddr[]      ����ַ
//   int len                ���ȣ�һ��Ϊ6
//   HI_U8 ucProtocol       ����Э������
// �� �� ֵ:
//    �ɹ�����  HI_ERR_SUCCESS
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: niesongsong/nkf62735 [2012-05-29]
//*****************************************************************************
HI_U32 mrsStaHeart(HI_VOID)
{
    MRS_STA_ITEM *item;
    MRS_STA_SRV_CTX_STRU *sta = mrsStaGetContext();
    HI_U16 ulSendLen = 0;
    HI_U8 *pSendFrame = HI_NULL;
    HI_U8 ucMeterProtocol = mrsStaGetMeterProtocol();
    HI_U8 ucFrameProtocol = (METER_PROTO_698_45 == ucMeterProtocol) ? MRS_SRV_PROTO_698_45 : MRS_SRV_PROTO_645;

    item = mrsToolsMalloc(sizeof(MRS_STA_ITEM));
    if (!item)
    {
        return HI_ERR_FAILURE;
    }

    (hi_void)memset_s(item, sizeof(MRS_STA_ITEM), 0, sizeof(MRS_STA_ITEM));

    pSendFrame = mrsGetSendAddrFrame(ucFrameProtocol, &ulSendLen);
    item->from = MRS_STA_QUEUE_FROM_STA;
    item->buf = pSendFrame;
    item->len = ulSendLen;
    item->id = MRS_STA_ITEM_ID_HEART;
    item->time_out = (HI_U8)MRS_MS_TO_100MS(mrsGet645FrameHeadTimeout(MRS_STA_RM_CFG_LOCAL));
    item->bAutoStrategy = HI_TRUE;
    item->ucFrameTimeout = (HI_U8)MRS_MS_TO_100MS(mrsGet645FrameInterval(MRS_STA_RM_CFG_LOCAL));
    item->ucProtocol = ucMeterProtocol;

    if (!mrsStaTryEnQueue(&sta->stQueModule,item))
    {
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_017, HI_DIAG_MT("heart fail"));
        mrsToolsFree(item);
        return HI_ERR_FAILURE;
    }

    mrsDfxHeartCnt(); // ��������ͳ��

    return HI_ERR_SUCCESS;
}

HI_U32 mrsStaRxAfn13Pro(MRS_PLC_FRAME_DATA_STRU *frame,HI_PVOID *data,HI_U16 *len
                                        ,MRS_STA_ITEM *item)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U16 payload_len = 0;
    HI_U16 local_len = 0;
    MRS_DATA_TRANSMIT_AFN13_DL_STRU *transmit = HI_NULL;
#if defined(PRODUCT_CFG_SUPPORT_TEST_CONCENTRATOR)
    MRS_CMD_DATA_TRANSMIT_STRU * pDataTransmit = HI_NULL;

    pDataTransmit = (MRS_CMD_DATA_TRANSMIT_STRU *)frame->pucPayload;
    transmit = (MRS_DATA_TRANSMIT_AFN13_DL_STRU *)pDataTransmit->aucPayload;
    payload_len = pDataTransmit->usPayloadLen;
#else
    transmit = (MRS_DATA_TRANSMIT_AFN13_DL_STRU *)frame->pucPayload;
    payload_len = frame->usPayloadLen;
#endif

    if(transmit->dir != MRS_PLC_DL_FLG)
    {
#if defined(PRODUCT_CFG_SUPPORT_TEST_CONCENTRATOR)
        mrsStaTestModeBlackListInsert(((HI_U8 *)transmit) + transmit->stru_size, transmit->data_len);
#endif
        return HI_ERR_BAD_FRAME;
    }
    *len = transmit->data_len;
    *data = ((HI_U8*)transmit) + transmit->stru_size;
    item->time_out= transmit->time_out;
    item->seq= transmit->seq;
    item->ucProtocol = (HI_U8)transmit->protocol;
    local_len = sizeof(MRS_DATA_TRANSMIT_AFN13_DL_STRU) + transmit->data_len;
    if (payload_len != transmit->data_len + transmit->stru_size)
    {
        HI_DIAG_LOG_MSG_E3(MRS_FILE_LOG_FLAG_019, HI_DIAG_MT("AFN13 len err [%d] [%d] [%d]"),
                        payload_len,transmit->data_len,transmit->stru_size);
        mrsDfxXrPlcLenErr();
        return HI_ERR_FAILURE;
    }

    switch (transmit->interface_ver)
    {
        case MRS_AFN13_DL_VER_R0:
        default:
            if (payload_len < local_len)
            {
                HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_021, HI_DIAG_MT("AFN-13 len err [%d] [%d]"),
                                    payload_len,transmit->stru_size);
                mrsDfxXrPlcLenErr();
                return HI_ERR_FAILURE;
            }
            break;
    }

    return ret;
}

HI_U32 mrsStaRxAfn14Pro(MRS_PLC_FRAME_DATA_STRU *frame,HI_PVOID *data,HI_U16 *len
                                        ,MRS_STA_ITEM *item)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U16 payload_len = 0;
    HI_U16 local_len = 0;
    MRS_DATA_TRANSMIT_AFN14_DL_STRU *transmit_data = HI_NULL;
#if defined(PRODUCT_CFG_SUPPORT_TEST_CONCENTRATOR)
    MRS_CMD_DATA_TRANSMIT_STRU * pDataTransmit = HI_NULL;

    pDataTransmit = (MRS_CMD_DATA_TRANSMIT_STRU *)frame->pucPayload;
    transmit_data = (MRS_DATA_TRANSMIT_AFN14_DL_STRU *)pDataTransmit->aucPayload;
    payload_len = pDataTransmit->usPayloadLen;

    if (transmit_data->dir != MRS_PLC_DL_FLG)
    {
        mrsStaTestModeBlackListInsert(((HI_U8 *)transmit_data) + transmit_data->stru_size, transmit_data->data_len);
        return HI_ERR_BAD_DATA;
    }
#else
    transmit_data = (MRS_DATA_TRANSMIT_AFN14_DL_STRU *)frame->pucPayload;
    payload_len = frame->usPayloadLen;
#endif

    *len = transmit_data->data_len;
    *data = ((HI_U8*)transmit_data) + transmit_data->stru_size;
    item->time_out= transmit_data->time_out;
    item->seq= transmit_data->seq;
    item->ucProtocol = (HI_U8)transmit_data->protocol;
    local_len = sizeof(MRS_DATA_TRANSMIT_AFN14_DL_STRU) + transmit_data->data_len;
    if (payload_len != transmit_data->data_len + transmit_data->stru_size)
    {
        HI_DIAG_LOG_MSG_E3(MRS_FILE_LOG_FLAG_022, HI_DIAG_MT("AFN14 len err [%d] [%d] [%d]"),
                        payload_len,transmit_data->data_len,transmit_data->stru_size);
        mrsDfxLrPlcLenErr();
        return HI_ERR_FAILURE;
    }

    switch (transmit_data->interface_ver)
    {
        case MRS_AFN14_DL_VER_R0:
        default:
            if (payload_len < local_len)
            {
                HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_024, HI_DIAG_MT("AFN-14 len err [%d] [%d]"),
                                    payload_len,transmit_data->stru_size);
                mrsDfxLrPlcLenErr();
                return HI_ERR_FAILURE;
            }
            break;
    }

    return ret;
}

//*****************************************************************************
// ��������: mrsCmdStaSerialTransmit
// ��������: �����е�ת������
//
// ����˵��:
//   HI_U16 usId                      ����ID���˲����󲿷�������ĺ���
//   MRS_PLC_FRAME_DATA_STRU* pstReq  �յ���CCO���ݣ���ת��
// �� �� ֵ:
//    �ɹ�����  HI_ERR_SUCCESS
//
// ����Ҫ��:
// ���þ���:
// ��    ��: niesongsong/nkf62735 [2012-05-29]
//*****************************************************************************
HI_U32 mrsCmdStaSerialTransmit(HI_IN HI_U16 id, HI_PVOID req_packet, HI_U16 req_packet_size)
{
    MRS_STA_ITEM * item = HI_NULL;
    MRS_STA_ITEM temp;
    MRS_STA_SRV_CTX_STRU *sta = mrsStaGetContext();
    HI_U32 ret = HI_ERR_FAILURE;
    MRS_PLC_FRAME_DATA_STRU * plc_Req_frame = HI_NULL;
    HI_U8 * payload = HI_NULL;
    HI_U16 payload_len = 0;
    HI_BOOL bTimingFlag = HI_FALSE;
#if defined(PRODUCT_CFG_SUPPORT_TEST_CONCENTRATOR)
    MRS_CMD_DATA_TRANSMIT_STRU *pDataTransmit = HI_NULL;
#endif
	HI_U32 ulWaterDataFlag=0;

    plc_Req_frame = (MRS_PLC_FRAME_DATA_STRU*)req_packet;
    MRS_NOT_USED(req_packet_size);

    (hi_void)memset_s(&temp, sizeof(temp), 0, sizeof(temp));
	temp.id = id;
    (hi_void)memcpy_s(temp.dev_mac, HI_PLC_MAC_ADDR_LEN, plc_Req_frame->ucMac, HI_PLC_MAC_ADDR_LEN);

    //������
    if (!plc_Req_frame || !plc_Req_frame->pucPayload)
    {
        return HI_ERR_FAILURE;
    }

    if (id == ID_MRS_CMD_DATA_TRANSMIT_AFN13_PLC)
    {
        mrsDfxXrPlcRx();
        ret = mrsStaRxAfn13Pro(plc_Req_frame, (HI_PVOID*)&payload, &payload_len, &temp);
#if defined(MRS_SRV_PROTOCOL_CJT188)
		MRS_DATA_TRANSMIT_AFN13_DL_STRU *transmit = (MRS_DATA_TRANSMIT_AFN13_DL_STRU *)plc_Req_frame->pucPayload;
		//HI_U16 us188Pos = 0;
    	//HI_U16 us188Length = 0;
		//us188Length = transmit->data_len;
		if(((transmit->protocol==WIRELESS_TRANSMISSION_PROTOCOL)))
			//&&(HI_ERR_SUCCESS == mrsFind188Frame(payload, (HI_S16)payload_len, &us188Pos, &us188Length)))
		{
			ulWaterDataFlag=transmit->data[0];
			/*
			�͹��ı�����ֵ����
			0 δ֪
			1-3 ����
			4 ���������ϱ�ģʽˮ��
			5 ��ͨģʽˮ������
			6 ����
			7 �ȱ�
			��������
			*/
			mrsGetRfPlcRunInfo()->ucWaterMeterType = ulWaterDataFlag;
			mrsGetRfPlcRunInfo()->usPlcSn=temp.seq;
			/*
			if(WIRELESS_SET_OWNERSHIP==ulWaterDataFlag)
			{
				//mrsToolsMemcpy_s(pRunPlcInfo->ucSrcMeter, HI_PLC_MAC_ADDR_LEN, transmit->ucDstMeter, HI_PLC_MAC_ADDR_LEN);
				//mrsToolsMemcpy_s(pRunPlcInfo->ucDstMeter, HI_PLC_MAC_ADDR_LEN, transmit->ucSrcMeter, HI_PLC_MAC_ADDR_LEN);

				// ����������ϵ
				mrsRfSetMeterOwnerShip(transmit->data_len/8,transmit->data+7,transmit->data+1);
				return HI_ERR_SUCCESS;
			}
			else
			{
				HI_U8	*pData = (HI_U8 *)mrsToolsMalloc(transmit->data_len+3);
				int i=0;
				if(pData!=HI_NULL)
				{
					pData[0]=0;
					pData[1]=0;
					
					mrsSetDauFrameSn((HI_U8)((temp.seq)&0xFF));

					for(i=0;i<transmit->data_len-1;i++)
					{
						if(transmit->data[i+1]!=0xFE)
						{
							break;
						}
					}
					pData[2]=transmit->data_len-1-i;
					mrsToolsMemcpy_s(pData+3,250,transmit->data+1+i,transmit->data_len-1-i);
					mrsRFDau0x4XEncodeFrame(pData, transmit->data_len+2-i,transmit->data+i+3,40,0x40);
					mrsToolsFree(pData);
					return HI_ERR_SUCCESS;
				}
				mrsToolsFree(pData);
			}
			*/
			MRS_RF_DAU_FRAME_STRU *pDauFrame = mrsRFDauDecode(transmit->data+1,transmit->data_len-1);
			if(pDauFrame!=HI_NULL)
			{
				HI_U8 *pData = (HI_U8 *)mrsToolsMalloc(256);
				if(pData!=HI_NULL)
				{
					pData[0]=0x24;
					pData[1]=pDauFrame->ucChannelIndex;
					pData[2]=pDauFrame->ucDataCmdId;
					mrsToolsMemcpy_s(pData+3,250,transmit->data+1,transmit->data_len-1);
					mrsRfTransmit(pData,transmit->data_len+2,40,0x24);
					mrsToolsFree(pData);
					return HI_ERR_SUCCESS;
				}
			}
			else
			{
				HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_090, HI_DIAG_MT("[RF]sta rx Not mrsRFDauDecode"));
			}
			
		}
#endif
    }
    else if (id == ID_MRS_CMD_DATA_TRANSMIT_AFN14_PLC)
    {
        mrsDfxLrPlcRx();
        ret = mrsStaRxAfn14Pro(plc_Req_frame, (HI_PVOID*)&payload, &payload_len, &temp);
    }

#if defined(PRODUCT_CFG_SUPPORT_TEST_CONCENTRATOR)
    pDataTransmit = (MRS_CMD_DATA_TRANSMIT_STRU *)plc_Req_frame->pucPayload;
    mrsDfxRmPlcDlRx(id, plc_Req_frame->pucPayload, HI_FALSE);
#else
/* BEGIN: Modified by fengxiaomin/00209182, 2014/3/27   ���ⵥ��:DTS2014032906596 */
    mrsDfxRmPlcDlRx(id, plc_Req_frame->pucPayload, HI_FALSE);
/* END:   Modified by fengxiaomin/00209182, 2014/3/27 */
#endif

    if (ret != HI_ERR_SUCCESS)
    {
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_026, HI_DIAG_MT("rx plc err"));
        return HI_ERR_FAILURE;
    }

#if !defined(PRODUCT_CFG_SUPPORT_TEST_CONCENTRATOR)
    if (mrsToolsIsIICollector())
    {
        if (ID_MRS_CMD_DATA_TRANSMIT_AFN13_PLC == id)
        {
            ret = mrsCltiiEventPlcProc(payload, payload_len, &temp);
            if (HI_ERR_SUCCESS == ret)
            {
                HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_011, HI_DIAG_MT("rx event proc"));
                return ret;
            }

            ret = mrsCltiiTimingPreProc(plc_Req_frame, (HI_PVOID*)&payload, &payload_len, &bTimingFlag);
            if (HI_ERR_SUCCESS != ret)
            {
                HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_059, HI_DIAG_MT("TimingPreProc ret=%d"),ret);
                return HI_ERR_FAILURE;
            }

            HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_060, HI_DIAG_MT("rx timing plc transfer bTimingFlag=%d"),bTimingFlag);
        }

        ret = mrsCltiiProtoProc(payload, payload_len, &temp);
        if (HI_ERR_SUCCESS == ret)
        {
            return ret;
        }
    }
#endif

    //�ж��������Ƿ�Ϊ��
    if (!payload)
    {
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_027, HI_DIAG_MT("payload null"));
        return HI_ERR_FAILURE;
    }

#if defined(PRODUCT_CFG_SYS_RST_SOLUTION)
    sta->stStaResetInfo.ulRxRMPlcTimes++;
#endif

#if defined(PRODUCT_CFG_SUPPORT_TEST_CONCENTRATOR)
    {
        // ��������վ�㣬���������
        HI_U8 aucMeterAddr[HI_METER_ADDR_LEN];

        if ((HI_ERR_SUCCESS != mrsGetFrame645MeterAddr(payload, payload_len, aucMeterAddr))
            || mrsStaTestModeIsInBlackList(aucMeterAddr))
        {
            return HI_ERR_FAILURE;
        }
    }
#endif

    //�����ݷ��͵������Ժ���
    item = mrsToolsMalloc(sizeof(MRS_STA_ITEM) + payload_len);
    if (item == HI_NULL)
    {
        return HI_ERR_FAILURE;
    }
    (hi_void)memset_s(item, sizeof(MRS_STA_ITEM) + payload_len, 0, sizeof(MRS_STA_ITEM) + payload_len);

    item->from     = MRS_STA_QUEUE_FROM_PLC;
    item->bcFlg    = plc_Req_frame->is_bc_frame;
    item->id       = id;
    item->len      = payload_len; // ��ȡ���ݳ���
    item->buf      = (HI_U8 *)item + sizeof(MRS_STA_ITEM);   // ƫ�����ݻ�����ָ��
    item->seq      = temp.seq;
    item->ucProtocol = temp.ucProtocol;
    item->time_out = temp.time_out;
    item->try_max = mrsGet645TimeoutRetryCfg(MRS_STA_RM_CFG_AUTO);
    item->bAutoStrategy = HI_TRUE;
    item->ucFrameTimeout = (HI_U8)MRS_MS_TO_100MS(mrsGet645FrameInterval(MRS_STA_RM_CFG_AUTO));
    (hi_void)memcpy_s(item->dev_mac, HI_PLC_MAC_ADDR_LEN, plc_Req_frame->ucMac, HI_PLC_MAC_ADDR_LEN);

   (hi_void)memcpy_s(item->buf, payload_len, payload, payload_len);     // ��������
    if (0 == item->time_out)
    {
        item->time_out = (HI_U8)((STA_TYPE_CLTI == mrsStaGetType()) ? MRS_MS_TO_100MS(MRS_CLTI_RM_FRAME_HEAD) : MRS_MS_TO_100MS(MRS_LOCAL_RM_FRAME_HEAD_DEFAULT));
    }

#if !defined(PRODUCT_CFG_SUPPORT_TEST_CONCENTRATOR)
    if ((ID_MRS_CMD_DATA_TRANSMIT_AFN13_PLC == id) && mrsCheckTimingFrame(payload, payload_len))
    {
        item->sub_id = MRS_STA_TRANSFER_TIMING_NORMAL;
    }

    if (mrsToolsIsIICollector()) //�����ǰΪII�Ͳɼ���
    {
        if (HI_TRUE == bTimingFlag)
        {
            item->sub_id = MRS_STA_TRANSFER_TIMING_CLTII_EX;
        }
    }
#endif

    // �����ǰΪ��װģʽ��ֱ�ӷ���PLCӦ��֡-�̶�����
    if(EQUIP_DUT_IS_EQUIP_MODE())
    {
        MRS_PLC_FRAME_DATA_STRU stPlcData = {0};
        HI_U8 respFrame[] = { 0x68, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
                              0x68, 0x91, 0x08, 0x33, 0x33, 0x34, 0x33,
                              0x75, 0x33, 0x33, 0x33, 0x45, 0x16 };

        mrsStaCreateReadMeterPlc(&stPlcData, item, respFrame, sizeof(respFrame), METER_PROTO_645_2007);
        MRS_SendPlcFrame(&stPlcData);

        mrsToolsFree(stPlcData.pucPayload);
        mrsToolsFree(item);
    }
    // �����ǰΪ����ģʽ������н��ж���
    else
    {
        //���Լ������
        if(!mrsStaTryEnQueue(&sta->stQueModule,item))
        {
            HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_029, HI_DIAG_MT("in queue fail"));

            mrsToolsFree(item);
        }
#if !defined(PRODUCT_CFG_SUPPORT_TEST_CONCENTRATOR)
        #if defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG) && defined(MRS_LOG_STA_SWITCH)
        else
        {
            //mrsLogFrameInfStatTx(item->buf, item->len, HI_FALSE);//optimized by weichao
        }
        #endif
#endif

        //֪ͨ����
        mrsStaNotifyQueue();

    }

    return ret;
}

HI_U32 mrsBroadcastCmdTransmit(hi_u16 id, hi_pvoid packet, hi_u16 packet_size)
{
    MRS_STA_ITEM * item = HI_NULL;
    MRS_STA_SRV_CTX_STRU *sta = mrsStaGetContext();
    hi_u32 ret = HI_ERR_SUCCESS;
    MRS_PLC_FRAME_DATA_STRU * plc = (MRS_PLC_FRAME_DATA_STRU *)packet;
    MRS_BROADCAST_STRU * transmit = HI_NULL;
    hi_u8 *payload = HI_NULL;
    hi_u16 payload_len = 0;
    hi_u16 start_pos = 0;
    hi_u16 frm_len = 0;

    HI_UNREF_PARAM(packet_size);

    /* ������ */
    if (plc == HI_NULL) {
        return HI_ERR_FAILURE;
    }

    if ((plc->pucPayload == HI_NULL) || (plc->usPayloadLen <= sizeof(MRS_BROADCAST_STRU))) {
        return HI_ERR_FAILURE;
    }

    transmit = (MRS_BROADCAST_STRU *)plc->pucPayload;
    if (plc->usPayloadLen != transmit->stru_len + transmit->data_len) {
        return HI_ERR_FAILURE;
    }

    /* ��ȡ������ת������ */
    payload = plc->pucPayload + transmit->stru_len;
    payload_len = transmit->data_len;

    if ((mrsFind645Frame(payload, (HI_S16)payload_len, &start_pos, &frm_len) != HI_ERR_SUCCESS) &&
        (mrsFind698Frame(payload, payload_len, &start_pos, &frm_len) != HI_ERR_SUCCESS)) {
        return HI_ERR_FAILURE;
    }

    payload += start_pos;
    payload_len = frm_len;

    /* �����ݷ��͵������Ժ��� */
    item = (MRS_STA_ITEM *)mrsToolsMalloc(sizeof(MRS_STA_ITEM) + payload_len);
    if (item == HI_NULL) {
        return HI_ERR_FAILURE;
    }

    (hi_void)memset_s(item, sizeof(MRS_STA_ITEM) + payload_len, 0, sizeof(MRS_STA_ITEM) + payload_len);
    item->buf = (hi_u8 *)item + sizeof(MRS_STA_ITEM);
    item->len = payload_len;
    item->from = MRS_STA_QUEUE_FROM_PLC;
    item->id = id;
    (hi_void)memcpy_s(item->buf, payload_len, payload, payload_len);

    if (mrsStaTryEnQueue(&sta->stQueModule,item) == HI_FALSE) {
        mrsToolsFree(item);
    }

    mrsStaNotifyQueue();

    return ret;
}


HI_U32 mrsTestCmdTransmit(hi_u16 id, hi_pvoid packet, hi_u16 packet_size)
{
    MRS_STA_ITEM * item = HI_NULL;
    MRS_STA_SRV_CTX_STRU *sta = mrsStaGetContext();
    MRS_PLC_FRAME_DATA_STRU *plc = (MRS_PLC_FRAME_DATA_STRU*)packet;
    MRS_TEST_TRANSMIT_STRU *transmit = HI_NULL;
    hi_u8 *payload = HI_NULL;
    hi_u16 payload_len = 0;

    MRS_NOT_USED(packet_size);

    if (plc == HI_NULL) {
        return HI_ERR_FAILURE;
    }

    if ((plc->pucPayload == HI_NULL) && (plc->usPayloadLen <= sizeof(MRS_TEST_TRANSMIT_STRU))) {
        return HI_ERR_FAILURE;
    }

    transmit = (MRS_TEST_TRANSMIT_STRU *)plc->pucPayload;
    if (transmit == HI_NULL || (transmit->data_len == 0) || (plc->usPayloadLen != transmit->stru_len + transmit->data_len)) {
        return HI_ERR_FAILURE;
    }

    payload = plc->pucPayload + transmit->stru_len;
    payload_len = transmit->data_len;

    item = (MRS_STA_ITEM *)mrsToolsMalloc(sizeof(MRS_STA_ITEM) + payload_len);
    if (item == HI_NULL) {
        return HI_ERR_NOT_ENOUGH_MEMORY;
    }

    (hi_void)memset_s(item, sizeof(MRS_STA_ITEM) + payload_len, 0, sizeof(MRS_STA_ITEM) + payload_len);

    item->buf = (HI_U8 *)item + sizeof(MRS_STA_ITEM);
    item->len = payload_len;
    item->from = MRS_STA_QUEUE_FROM_PLC;
    item->id = id;
    item->time_out = (HI_U8)transmit->timeout;
    (hi_void)memcpy_s(item->buf, payload_len, payload, payload_len);

    if (mrsStaTryEnQueue(&sta->stQueModule, item) == HI_FALSE) {
        mrsToolsFree(item);
    }

    mrsStaNotifyQueue();

    return HI_ERR_SUCCESS;
}

// ����ת������ַ�
HI_U32 mrsCmdStaDataTransmit(HI_IN HI_U16 usId, HI_PVOID pReqPacket, HI_U16 usReqPacketSize)
{
    HI_U32 ret = HI_ERR_SUCCESS;

#if defined(PRODUCT_CFG_SUPPORT_TEST_CONCENTRATOR)
    MRS_PLC_FRAME_DATA_STRU *pstReqFrame = (MRS_PLC_FRAME_DATA_STRU *)pReqPacket;
    MRS_CMD_DATA_TRANSMIT_STRU *pTransmit = HI_NULL;

    if (!pstReqFrame)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    pTransmit = (MRS_CMD_DATA_TRANSMIT_STRU *)pstReqFrame->pucPayload;
    if (!pTransmit)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    switch (pTransmit->usCmdId)
    {
    case ID_MRS_CMD_DATA_TRANSMIT_AFN13_PLC:
    case ID_MRS_CMD_DATA_TRANSMIT_AFN14_PLC:
        ret = mrsCmdStaSerialTransmit(pTransmit->usCmdId, pReqPacket, usReqPacketSize);
        break;

    default:
        ret = HI_ERR_NOT_SUPPORT;
        break;
    }

#else

    switch (usId)
    {
        case ID_MRS_CMD_PARALLEL_DATA_PLC:
            ret = mrsParallelCmdTransmit(usId, pReqPacket, usReqPacketSize);
            break;

        case ID_MRS_CMD_BROADCAST_TRANSMIT_PLC:
            ret = mrsBroadcastCmdTransmit(usId, pReqPacket, usReqPacketSize);
            break;

        case ID_MRS_CMD_EVENT_TRANSMIT_PLC:
            break;

        case ID_MRS_CMD_TEST_TRANSMIT_PLC:
            ret = mrsTestCmdTransmit(usId, pReqPacket, usReqPacketSize);
            break;

        default:
            ret = mrsCmdStaSerialTransmit(usId, pReqPacket, usReqPacketSize);
            break;
    }
#endif

    return ret;
}

// ��ȡվ��ͣ����Ϣ
HI_U32 mrsCmdStaGetPoweroffInfo(HI_IN HI_U16 usId, HI_PVOID pReqPacket, HI_U16 usReqPacketSize)
{
    MRS_PLC_FRAME_DATA_S * pstPlc = (MRS_PLC_FRAME_DATA_S *)pReqPacket;
    MRS_PLC_POWEROFF_INFO_DL_S * pstDlFrame = HI_NULL;
    MRS_STA_SRV_CTX_STRU * pstStaCtx = mrsStaGetContext();
    HI_MDM_POWEROFF_DETECT_MAC_LIST_S * pstMacList = HI_NULL;
    HI_U32 ulMacListSize = 0;
    HI_U32 ulIdx = 0;
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U16 sta_num = 0;

    HI_UNREF_PARAM(usId);
    HI_UNREF_PARAM(usReqPacketSize);

    if (!pstPlc || !pstPlc->pucPayload)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    if (!pstStaCtx)
    {
        return HI_ERR_FAILURE;
    }

    pstDlFrame = (MRS_PLC_POWEROFF_INFO_DL_S *)pstPlc->pucPayload;
    sta_num = pstDlFrame->sta_num;

    HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_001, HI_DIAG_MT("rcv get poweroff info cmd."), sta_num);

    if (pstDlFrame->detect_flag)
    {
        pstStaCtx->stPowerInfo.usRcvDetectCmdCnt++;
    }
    else
    {
        pstStaCtx->stPowerInfo.usRcvRecheckCmdCnt++;
    }

     /*
    �������ˡ�j00415756
    ������ˡ�l00212912

    �����������pstDlFrame->mac_list �ǿɱ䳤�������䳤��ͨ��sta_num���ƣ�
                ���sta_num ��ֵΪ0���������ѭ��
    ������Ӱ�졿codedex�޷�ȷ���ɱ䳤�������ȣ������澯��������
    */

    for (ulIdx = 0; ulIdx < pstDlFrame->sta_num; ulIdx++)
    {
        //�ж�mac��ַ�Ƿ�Ϊ��վ���mac��ַ��������ڱ�վ��mac��ַ��������Ӧ֡
        if (mrsSrvVerifyMac(pstDlFrame->mac_list + ulIdx * HI_PLC_MAC_ADDR_LEN))
        {
            if (pstDlFrame->detect_flag)
            {
                pstStaCtx->stPowerInfo.usRcvDetectCmdForMeCnt++;
            }
            else
            {
                pstStaCtx->stPowerInfo.usRcvRecheckCmdForMeCnt++;
            }

            ulRet = mrsStaReportPoweroffInfo(pstPlc->ucMac, pstDlFrame->seq);
            if (HI_ERR_SUCCESS != ulRet)
            {
                pstStaCtx->stPowerInfo.ucSendCmdFailCnt++;
                pstStaCtx->stPowerInfo.ulSendCmdErrCode = ulRet;
            }

            break;
        }
    }

    if (pstDlFrame->detect_flag)
    {
        ulMacListSize = sizeof(HI_MDM_POWEROFF_DETECT_MAC_LIST_S) + pstDlFrame->sta_num * HI_PLC_MAC_ADDR_LEN;
        pstMacList = mrsToolsMalloc(ulMacListSize);
        if (!pstMacList)
        {
            return HI_ERR_MALLOC_FAILUE;
        }

        (hi_void)memset_s(pstMacList, ulMacListSize, 0,ulMacListSize);
        pstMacList->ucStaNum = (HI_U8)pstDlFrame->sta_num;
       (hi_void)memcpy_s(pstMacList->aucMacList, pstDlFrame->sta_num * HI_PLC_MAC_ADDR_LEN,pstDlFrame->mac_list, pstDlFrame->sta_num * HI_PLC_MAC_ADDR_LEN);

        HI_DIAG_LOG_BUF(MRS_FILE_LOG_FLAG_008, HI_DIAG_MT("[Power_OFF-MR]-help_detect"), pstMacList->aucMacList, pstDlFrame->sta_num * HI_PLC_MAC_ADDR_LEN);
        HI_MDM_HelpNeighborAnswerDetect(pstMacList);

        mrsToolsFree(pstMacList);
    }

    return ulRet;
}

HI_U32 mrsStaReportPoweroffInfo(HI_U8 aucDstMac[HI_PLC_MAC_ADDR_LEN], HI_U32 ulSeq)
{
    MRS_PLC_FRAME_DATA_STRU stPlcFrame = {0};
    MRS_PLC_POWEROFF_INFO_UP_S * pstUpFrame = HI_NULL;
    HI_U32 ulPayloadSize = sizeof(MRS_PLC_POWEROFF_INFO_UP_S);
    MRS_STA_SRV_CTX_STRU * pstStaCtx = mrsStaGetContext();
    HI_U32 ulRet = HI_ERR_SUCCESS;

    HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1000, HI_DIAG_MT("report poweroff info: [dst_mac]"), aucDstMac, HI_PLC_MAC_ADDR_LEN);

    if (!aucDstMac || !pstStaCtx)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    pstUpFrame = mrsToolsMalloc(ulPayloadSize);
    if (!pstUpFrame)
    {
        return HI_ERR_MALLOC_FAILUE;
    }

    (hi_void)memset_s(pstUpFrame, ulPayloadSize, 0, ulPayloadSize);
    pstUpFrame->interface_ver = MRS_CMD_STRU_VER_R0;
    pstUpFrame->seq = ulSeq;
    pstUpFrame->stru_len = ulPayloadSize;
    pstUpFrame->last_poweroff_time = pstStaCtx->stPowerInfo.ulMeterPowerOffTime;
    pstUpFrame->last_poweron_time = pstStaCtx->stPowerInfo.ulMeterPowerOnTime;

    (hi_void)memset_s(&stPlcFrame, sizeof(MRS_PLC_FRAME_DATA_STRU), 0,sizeof(MRS_PLC_FRAME_DATA_STRU));
    stPlcFrame.usId = ID_MRS_CMD_GET_POWEROFF_INFO;
    stPlcFrame.pucPayload = (HI_PBYTE)pstUpFrame;
    stPlcFrame.usPayloadLen = (HI_U16)ulPayloadSize;
   (hi_void)memcpy_s(stPlcFrame.ucMac,  HI_PLC_MAC_ADDR_LEN, aucDstMac, HI_PLC_MAC_ADDR_LEN);

    ulRet = MRS_SendPlcFrame(&stPlcFrame);

    mrsToolsFree(pstUpFrame);

    return ulRet;
}


HI_U32 mrsCmdStaSearchMeter(HI_IN HI_U16 usId, HI_PVOID pReqPacket, HI_U16 usReqPacketSize)
{
    HI_U32 ret = HI_ERR_SUCCESS;
	//HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_051, HI_DIAG_MT("[mrsCmdStaSearchMeter]usId"),usId);	
    switch (usId) {
        case ID_MRS_CMD_GET_METERADDR_PLC:
            if (HI_MDM_GetProtocolVer() == HI_MDM_PTCL_VER_100) {
                mrsDfxMapFrameValid();
                return mrsStaCheckReport2Cco(MRS_STA_REPORT_METER_ADDR_RESPOND);
            }
            return HI_ERR_NOT_SUPPORT;

        case ID_MRS_CMD_GET_DEVICE_INF_PLC:
            ret = mrsCmdStaGetMeterAddr(usId, pReqPacket, usReqPacketSize);
            break;

        case ID_MRS_CMD_GET_METERLIST:
            ret = mrsCmdStaGetMeterList(usId, pReqPacket, usReqPacketSize);
            break;

        case ID_MRS_CMD_START_SEARCH_METER:
#if defined(PRODUCT_CFG_CLTI_SUPPORT_MRS_SWITCH)
            if (mrsCltiSearchIsEnable() || mrsCltiiSearchIsEnable())
#else
			if ( mrsCltiiSearchIsEnable())
#endif
            {
                ret = mrsCmdStaStartSearchMeter(usId, pReqPacket, usReqPacketSize);
            }
            break;

        case ID_MRS_CMD_STOP_SEARCH_METER:
#if defined(PRODUCT_CFG_CLTI_SUPPORT_MRS_SWITCH)
            if (mrsCltiSearchIsEnable() || mrsCltiiSearchIsEnable())
#else
			if (mrsCltiiSearchIsEnable())
#endif
            {
                ret = mrsCmdStaStopSearchMeter(usId, pReqPacket, usReqPacketSize);
            }
            break;

        default:
            ret = HI_ERR_NOT_FOUND;
            break;
    }

    return ret;
}


//*****************************************************************************
// ��������: mrsCmdStaGetMeterAddr
// ��������: ���͵���ַ��MAC��ַӳ���CCO����һ������ע�ắ�������յ�CCO
//           �Ļ�ȡ���ַ����˺����ᱻ���ã��������ַ��MACӳ����ϴ���
//
// ����˵��:
//   HI_U16 usId        ����ID
//   HI_PVOID pstReq    ����ṹ�����Ժ���
//   HI_U16 usReqPacketSize ����ṹ��С
// �� �� ֵ:
//    �ɹ�����  HI_ERR_SUCCESS
//
// ����Ҫ��:
// ���þ���:
//*****************************************************************************
hi_u32 mrsCmdStaGetMeterAddr(hi_u16 id, hi_pvoid req_packet, hi_u16 packet_size)
{
    MRS_PLC_FRAME_DATA_STRU * plc = (MRS_PLC_FRAME_DATA_STRU *)req_packet;
    MRS_GET_DEVICE_INF_DL_STRU * frame = HI_NULL;
    hi_u8 *src_mac = HI_NULL;
    hi_u8 *dst_mac = HI_NULL;

    HI_UNREF_PARAM(id);
    HI_UNREF_PARAM(packet_size);

    if (req_packet == HI_NULL) {
        return HI_ERR_INVALID_PARAMETER;
    }

    if (plc->pucPayload == HI_NULL) {
        return HI_ERR_INVALID_PARAMETER;
    }

    frame = (MRS_GET_DEVICE_INF_DL_STRU *)plc->pucPayload;
    if (frame->stru_len != plc->usPayloadLen) {
        return HI_ERR_INVALID_PARAMETER;
    }

    switch (frame->interface_ver) {
        case MRS_GET_DEVICE_INF_DL_VER_R0:
        case MRS_GET_DEVICE_INF_DL_VER_R1:
            if (frame->stru_len != sizeof(mrs_get_device_inf_dl_ver1)) {
                return HI_ERR_INVALID_PARAMETER;
            }
            break;

        case MRS_GET_DEVICE_INF_DL_VER_R2:
            if (frame->stru_len != sizeof(MRS_GET_DEVICE_INF_DL_STRU)) {
                return HI_ERR_INVALID_PARAMETER;
            }

            src_mac = frame->dst;
            dst_mac = frame->src;

            if (mrsSrvVerifyMac(src_mac) != HI_TRUE) {
                mrsDfxMapFrameInvalid();
                return HI_ERR_FAILURE;
            }
            break;

        default:
            return HI_ERR_NOT_SUPPORT;
    }

    if ((frame->reserved != 0) || (frame->reserved2 != 0)) {
        return HI_ERR_INVALID_PARAMETER;
    }

    mrsDfxMapFrameValid();

    return mrsStaReportDeviceInf(src_mac, dst_mac);
}

HI_U32 mrsStaFillMeterListUpFrameHead(HI_U8 *aucSrcMac, HI_U8 *aucDstMac, HI_U32 ulSeq, MRS_PLC_METERLIST_UP_S *pstUpFrame)
{
    MRS_METER_LIST_S *pstMeterList = &(mrsStaGetContext()->stMeterList);
    HI_U8 ucIPAddr[HI_IPV4_ADDR_LEN] = {0};

    if (!pstUpFrame || !aucSrcMac || !aucDstMac)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    pstUpFrame->interface_ver = MRS_GET_METERLIST_UP_VER;
    pstUpFrame->stru_len = sizeof(MRS_PLC_METERLIST_UP_S);
    pstUpFrame->meter_num = 0;
    pstUpFrame->seq = ulSeq;

    HI_MDM_GetLocalMacAddr(pstUpFrame->id);
    mrsHexInvert(pstUpFrame->id, sizeof(pstUpFrame->id));

   (hi_void)memcpy_s(pstUpFrame->src, HI_PLC_MAC_ADDR_LEN, aucSrcMac, HI_PLC_MAC_ADDR_LEN);
   (hi_void)memcpy_s(pstUpFrame->dst, HI_PLC_MAC_ADDR_LEN, aucDstMac, HI_PLC_MAC_ADDR_LEN);
#if defined(PRODUCT_CFG_CLTI_SUPPORT_MRS_SWITCH)
    if (mrsCltiSearchIsEnable() || mrsCltiiSearchIsEnable()) // �ɼ���
#else
	if (mrsCltiiSearchIsEnable()) // �ɼ���
#endif
    {
        HI_NV_FTM_COLLECT_PARAM_FILE_S stNv;

        pstUpFrame->type = MRS_STA_TYPE_COLLECTOR;

        (hi_void)memset_s(&stNv, sizeof(stNv), 0, sizeof(stNv));

        if (HI_ERR_SUCCESS == HI_MDM_NV_Read(HI_NV_FTM_COLLECT_PARAM_FILE, &stNv, sizeof(stNv)))
        {
           (hi_void)memcpy_s(pstUpFrame->asset, HI_METER_ADDR_LEN, stNv.CLTAddress, HI_METER_ADDR_LEN);
        }
        else
        {
            (hi_void)memset_s(pstUpFrame->asset, HI_METER_ADDR_LEN, 0xBB, HI_METER_ADDR_LEN);
        }
    }
    else // ���
    {
        pstUpFrame->type = pstMeterList->ucType;

        (HI_VOID)HI_MDM_NM_GetAddr(pstUpFrame->asset, ucIPAddr);
        mrsHexInvert(pstUpFrame->asset, sizeof(pstUpFrame->asset));
    }

    return HI_ERR_SUCCESS;
}

//*****************************************************************************
// ��������: mrsCmdStaGetMeterList
// ��������: ��ѯ�ɼ����ѱ�Ľ��
//
// ����˵��:
//   HI_U16 usId        ����ID
//   HI_PVOID pstReq    ����ṹ
//   HI_U16 usReqPacketSize ����ṹ��С
// �� �� ֵ:
//    �ɹ�����  HI_ERR_SUCCESS
//
// ����Ҫ��:
// ���þ���:
// ��    ��: niesongsong/nkf62735 [2012-05-29]
// ��    ��: cuiate/c00233580 [2013-08-16]
//*****************************************************************************
HI_U32 mrsCmdStaGetMeterList(HI_U16 usId,HI_PVOID pstReq,HI_U16 usReqPacketSize)
{
    MRS_PLC_FRAME_DATA_STRU * plc = (MRS_PLC_FRAME_DATA_STRU *)pstReq;
    MRS_PLC_METERLIST_DL_S * frame = HI_NULL;
    HI_U8 *pucSrcMac = HI_NULL, *pucDstMac = HI_NULL;
    MRS_STA_SEARCH_CTX * pstSearchCtx = mrsStaSearchCtx();

    HI_UNREF_PARAM(usId);
    HI_UNREF_PARAM(usReqPacketSize);

    if (!pstReq) {
        return HI_ERR_FAILURE;
    }

    frame = (MRS_PLC_METERLIST_DL_S *)plc->pucPayload;
    if (frame == HI_NULL) {
        return HI_ERR_FAILURE;
    }

    // �ж��Ƿ��Ƿ�����վ��ı���
    if (frame->stru_len >= sizeof(MRS_PLC_METERLIST_DL_S)) {
        pucSrcMac = frame->dst;
        pucDstMac = frame->src;
        if (!mrsSrvVerifyCcoMac(frame->src)) {
            mrsDfxSmFrameInvalid();
            return HI_ERR_FAILURE;
        }

        if (!mrsSrvVerifyMac(frame->dst)) {
            mrsDfxSmFrameInvalid();
            return HI_ERR_FAILURE;
        }
    }

    mrsDfxSmFrameValid();

    if (frame->option == MRS_SM_BIND_NETWORK) {
        return mrsStaHandleRcvBindNetwork(frame);
    }
#if defined(PRODUCT_CFG_SUPPORT_MRS_DUOL_MODE_RF)
	else if (MRS_SM_WATER_CMD == frame->option)
    {
    	HI_U16  nDataLen=0;
    	if(plc->usPayloadLen>sizeof(MRS_PLC_METERLIST_DL_S))
		{
			nDataLen = plc->usPayloadLen-sizeof(MRS_PLC_METERLIST_DL_S);
		}
        return mrsStaHandleWaterCmd(frame,nDataLen);
    }
#endif
    

    pstSearchCtx->stSearchCfg.ulSeq = frame->seq;
#if defined(PRODUCT_CFG_CLTI_SUPPORT_MRS_SWITCH)
    if (mrsToolsIsIICollector() || mrsIsRealClti()) {
        /* �ɼ��� */
#else
	if (mrsToolsIsIICollector()) {
#endif
#if defined(PRODUCT_CFG_CLTI_SUPPORT_MRS_SWITCH)
        if (!mrsCltiiSearchIsEnable() && !mrsCltiSearchIsEnable()) {
            return HI_ERR_NOT_SUPPORT;
        }
#else
		if (!mrsCltiiSearchIsEnable()) {
            return HI_ERR_NOT_SUPPORT;
        }
#endif

        mrsDfxCltSmPlcQueryRx(frame->seq);

        if (mrsStaSearchGetStatus() == MRS_SEARCH_STATUS_IN_PROGRESS) {
            if (frame->force_resp) {
                return mrsCltReportSmResult2CCO(pucSrcMac, pucDstMac, HI_TRUE);
            }
        } else {
            return mrsCltReportSmResult2CCO(pucSrcMac, pucDstMac, HI_FALSE);
        }
    } else {
        /* ����� or ����� */

        if (mrsStaGetState() == MRS_STA_STATE_INIT) {
            if (frame->force_resp) {
                return mrsStaReportSmResult2CCO(pucSrcMac, pucDstMac, HI_TRUE);
            }
        } else {
            return mrsStaReportSmResult2CCO(pucSrcMac, pucDstMac, HI_FALSE);
        }
    }

    return HI_ERR_SUCCESS;
}


// ������ʱͳ��
HI_VOID mrsStaHeartTimeoutCnt(HI_U16 id)
{
    if(id == MRS_STA_ITEM_ID_HEART)
    {
        mrsDfxHeartTimeoutCnt();    // ������ʱͳ��
    }

    return;
}

//*****************************************************************************
// ��������: mrsMsgStaTimer
// ��������: ��Ӧ��ʱ������Ϣ����������ڳ�ʼ����ʱ��ע�ᣬ�������Ķ�ʱ������
//           ���ᷢ����Ϣ��MRS���У����õ��ô˺�������
//
// ����˵��:
//   HI_U16 usMsgId             ����ID����IDΪע���
//   HI_IN HI_PVOID pMsgParam   ��ʱ�����ݣ���������ʱ��ID����Ϣ��
// �� �� ֵ:
//    �ɹ�����  HI_ERR_SUCCESS
//
// ����Ҫ��:
// ���þ���:
// ��    ��: niesongsong/nkf62735 [2012-05-29]
//*****************************************************************************
HI_U32 mrsMsgStaTimer(HI_U16 usMsgId, HI_IN HI_PVOID pMsgParam)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    MRS_MSG_MR_TIMER_STRU* pst = HI_NULL;
    MRS_STA_SRV_CTX_STRU *sta = mrsStaGetContext();
    HI_U8 ucTesTMode = HI_MDM_GetPhysicalTestMode();

    HI_UNREF_PARAM(usMsgId);

    if (!pMsgParam)
    {
        return HI_ERR_FAILURE;
    }

    pst = (MRS_MSG_MR_TIMER_STRU*)pMsgParam;

    switch (pst->ucTimerId)
    {
    case MRS_STA_TIMER_TEST:
        mrsStaSearchMeter();
        break;

    case MRS_STA_TIMER_HEART:
#if !defined(PRODUCT_CFG_SUPPORT_TEST_CONCENTRATOR)
        if (!EQUIP_DUT_IS_EQUIP_MODE())//������ǲ���ģʽ
        {
            if (!mrsToolsIsIICollector())
            {
                ret = mrsStaHeart();
            }
        }
#endif
        break;

    case MRS_STA_TIMER_TTL:
    case MRS_STA_TIMER_RM_FRAME_INTERVAL:
        {
            MRS_QUE_MODULE *pstModule = &sta->stQueModule;
            MRS_SRV_QUEUE *pstQueue = &pstModule->stMrQueue;
            MRS_STA_ITEM * pstItem = mrsSrvQueueTop(pstQueue);

            if (pstItem)
            {
                if (pst->ucTimerId == MRS_STA_TIMER_RM_FRAME_INTERVAL)
                {
                    if (!pstItem->bFrameTimerFlg)
                    {
                        break;
                    }

                    pstItem->bFrameTimerFlg = HI_FALSE;

                    MRS_WaitSem(EN_APP_SEM_SRV_UART, HI_SYS_WAIT_FOREVER);
                    ret = mrsStaUartFrameTimeout();
                    MRS_SignalSem(EN_APP_SEM_SRV_UART);
                    if ((HI_ERR_SUCCESS == ret)
                        || ((MRS_STA_ITEM_ID_DETECT == pstItem->id)))
                    {
                        break;
                    }
                }
                else
                {
                    if (pstItem->bAutoStrategy && pstItem->bRcvByteFlg)
                    {
                        MRS_StartTimer(MRS_STA_TIMER_RM_FRAME_INTERVAL,
                            MRS_100MS_TO_MS(pstItem->ucFrameTimeout),
                            HI_SYS_TIMER_ONESHOT);

                        pstItem->bFrameTimerFlg = HI_TRUE;
                        pstItem->bTtlTimerFlg = HI_FALSE;
                        pstItem->bRcvByteFlg = HI_FALSE;

                        break;
                    }

                    pstItem->bTtlTimerFlg = HI_FALSE;
                    pstItem->bRcvByteFlg = HI_FALSE;
                }
#if defined(PRODUCT_CFG_CLTI_SUPPORT_MRS_SWITCH)
                if (MRS_STA_ITEM_ID_SEARCH_METER_CLT_I == pstItem->id)
                {
                    return mrsCltiFrameTimeoutProc();
                }
#endif
                mrsStaHeartTimeoutCnt(pstItem->id);
                mrsDfxStaUartTimeOut(pstItem->id);
            }
            else
            {
                if (pst->ucTimerId == MRS_STA_TIMER_RM_FRAME_INTERVAL)
                {
                    MRS_WaitSem(EN_APP_SEM_SRV_UART, HI_SYS_WAIT_FOREVER);
                    (HI_VOID)mrsStaUartFrameTimeout();
                    MRS_SignalSem(EN_APP_SEM_SRV_UART);
                    break;
                }
            }

            mrsDfxRefreshRmQueueTopStatus(pstQueue, pstModule->lock); // �������״̬ˢ��
            mrsStaQueueUnLock(pstModule);
            mrsSetFrameFlg(HI_TRUE);
            mrsDfxRefreshRmQueueTopStatus(pstQueue, pstModule->lock); // �������״̬ˢ��
        }
        // û��break;

/* END:   Modified by fengxiaomin/00209182, 2014/3/27 */
    case MRS_STA_TIMER_QUEUE:
        mrsStaQueueProc(&sta->stQueModule);
        mrsDfxRefreshRmQueueTopStatus(&sta->stQueModule.stMrQueue,sta->stQueModule.lock); // �������״̬ˢ��
        mrsSetFrameFlg(HI_FALSE);
        break;
    case MRS_STA_TIMER_IO_EVENT_OUT://IO����ģ��EVENT_OUT��ʱ������
        mrsEventTimeOutProc();//EVENT_OUT�ܽŲ鿴״̬
        break;

    case MRS_STA_TIMER_RUN_LED:
        mrsRunLedPro(HI_FALSE);
        break;

    case MRS_COLLECTOR_TIMER_IRDA_RESET:
        HI_MDM_Reboot(HI_SYS_REBOOT_CAUSE_MRS1); //ִ��Ӳ����ʼ��
        break;

    case MRS_STA_TIMER_EVENT_REPORT:
        mrsStaEvtReportPlcTimeoutProc();
        break;

    case MRS_STA_TIMER_LOCAL_MONITOR:
        mrsStaEvtMonTimeOutProc();
        break;

    case MRS_STA_TIMER_RESET_PIN:
        mrsResetTimeOutProc();
        break;

    case MRS_STA_TIMER_SIMU_CHL:
        {
            HI_SAL_DFX_HOST_CTRL_S * ctrl = HI_MDM_DfxGetHostCtrl();
            HI_DMS_CHL_PORT_OBJ_S stChlPort = mrsGetChlPort();

            if (ctrl == HI_NULL)
            {
                break;
            }

            if (0 != ctrl->ucMrRedirect2HsoPort)
            {
            /* BEGIN: Added by liuxipeng/lkf54842 2014/01/15 ���ⵥ��: DTS2013121904065 */
                mrsFwChlInit(&stChlPort, HI_DMS_CHL_UART_PORT_APP);
            /* END  : Added by liuxipeng/lkf54842 2014/01/15 */
                (hi_void)memset_s(ctrl, sizeof(HI_SAL_DFX_HOST_CTRL_S), 0, sizeof(HI_SAL_DFX_HOST_CTRL_S));
                mrsDfxVmChlStatus(ctrl->ucMrRedirect2HsoPort);
            }
        }
        break;

    case MRS_STA_TIMER_SM_FRAME:
        mrsCltiiSearchFrameTimeoutProc(mrsStaSearchCtx());
        break;

    case MRS_STA_TIMER_SM_BYTE:
        mrsCltiiSearchByteTimeoutProc(mrsStaSearchCtx());
        break;

    case MRS_STA_TIMER_SM_VM:
        mrsCltiiSearchVMTimeoutProc();
        break;

    case MRS_STA_TIMER_SM_PERIOD:
    case MRS_STA_TIMER_SM_PERIOD_NO_METER:
        mrsStaSearchStart(HI_FALSE);
        break;

#if defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG) && defined(MRS_LOG_STA_SWITCH)
    case MRS_STA_TIMER_DAYFREEZE_REC:  // �����Լ�¼�ն���ͳ�ƶ�ʱ��
        mrsLogStaDayfreezeRecTimerProc();
        break;
#endif
#if defined(PRODUCT_CFG_CLTI_SUPPORT_MRS_SWITCH)
    case MRS_STA_TIMER_CLTI_POSITIVE_A:
        mrsCltiTryReadPositiveATx();
        break;
	
    case MRS_STA_TIMER_CLTI_SM_START:
        mrsCltiSmStartTx();
        break;

    case MRS_STA_TIMER_CLTI_SM_STOP:
        mrsCltiSmStopTx();
        break;

    case MRS_STA_TIMER_CLTI_SM_QUERY:
        mrsCltiSmQueryTxRetry();
        break;
#endif
    case MRS_STA_TIMER_VM_EVT_ACTIVE:
        mrsStaEvtOutProc();
        break;

    case MRS_STA_TIMER_ZERO_CROSS:
    case MRS_STA_TIMER_ZERO_CROSS_ADC:
        mrsZeroCrossCheckProc(HI_ERR_TIMEOUT, HI_NULL);
        break;

    case MRS_STA_TIMER_CLTII_EVENT:
        mrsCltiiEventStartMonitor();
        break;

    case MRS_STA_TIMER_CLTII_LOCAL_RM_INTERVAL:
        mrsCltiiEventMonitorSendMessage(MRS_CLTII_EVENT_MSG_NEXT_ITEM);
        break;

    case MRS_STA_TIMER_RM_METER_JOIN_NET:
        mrsCltRmMeterSetMacTimeout();
        break;

#if defined(PRODUCT_CFG_SUPPORT_MRS_UPG_DEMO)
    case MRS_STA_TIMER_CLTI_UPG:
        mrsCltIUpgTimerProc();
        break;
#endif
    /* BEGIN: Modified by liuying/00369907, 2017/2/13   */
    #ifdef PRODUCT_CFG_TTCN
    case MRS_STA_TIMER_TESTMODE:
        HI_MDM_ExitPlcTest();
        break;

    case MRS_STA_TIMER_ENTER_TESTMODE:
        if (PHYSICAL_TEST_MODE_APP_TO_COM_LOOP == ucTesTMode
            ||  PHYSICAL_TEST_MODE_EXIT == ucTesTMode)
        {
            return  ret;
        }
        mrsSrvCaptureDeinit();

        break;

    /* BEGIN: Modified by cuichunyan/00276962, 2017/4/21   */
    case MRS_STA_TIMER_PHYSICAL_TESTMODE://ҵ�񴮿ڷ���
        if (ucTesTMode == PHYSICAL_TEST_MODE_APP_TO_COM_LOOP)//�����͸��ת��
        {
            MRS_RestoreUartConfig(MRS_SRV_BAUD_RATE_DEFAULT, MRS_SRV_UART_PARITY_EVEN);
            HI_MDM_ExitPlcTest();
            mrsSrvCaptureDeinit();
            mrsDfxStaTestModeExit(PHYSICAL_TEST_MODE_APP_TO_COM_LOOP,ucTesTMode);
        }
        break;
    /* END: Modified by Modified by cuichunyan/00276962, 2017/4/21  */
    case MRS_STA_TIMER_MSDU_TESTMODE:
        if (PHYSICAL_TEST_MODE_EXIT == ucTesTMode)//�����͸��ת��
        {
            MRS_RestoreUartConfig(MRS_SRV_BAUD_RATE_DEFAULT, MRS_SRV_UART_PARITY_EVEN);
        	HI_MDM_ExitPlcTest();
            mrsSrvCaptureDeinit();
        }

        break;
	#endif

    //дcase����ʱ����Ӧ
    case MRS_STA_TIMER_POWERFAILURE:
        ret = mrsStaPowerFailureFrameProc();
        break;
	case MRS_STA_TIMER_POWERON:
		ret = mrs_local_power_on_report_frame();
		break;
    case MRS_STA_TIMER_WAIT_POWERFAILURE_ACK:
        ret = mrsStaHandleWaitPowerFailureAck();
        break;
	case MRS_STA_PULL_RST_DELAY_TIMER:
		ret = mrs_power_failure_juedge_pull_rst();
		break;
	case MRS_STA_DISABLE_POWER_FAILURE_TIMER:
		ret = mrs_power_failure_judge_disable_power_failure();
		break;
    /* END: Modified by Modified by liuying/00369907, 2017/2/13  */

    case MRS_STA_TIMER_CLT_SEND_PLC:
        ret = mrsCltPowerOffPlcTimeout();
        break;

#if defined(PRODUCT_CFG_SUPPORT_TEST_CONCENTRATOR)
    case MRS_STA_TIMER_TEST_MODE_FRAME:
        mrsStaTestModeTimeout();
        break;
#endif

    case MRS_STA_TIMER_NETWORK_LOCK_CHECK:
        mrsNetworkCheckTimeout();
        break;

    case MRS_STA_TIMER_REJOIN_AFTER_UNLOCK:
        mrsRejoinNetworkAfterUnlock();
        break;

    case MRS_STA_TIMER_RM_QUEUE_LOCK:
        ret = HI_MDM_Reboot(HI_SYS_REBOOT_CAUSE_MRS2);
        break;
	#if defined(PRODUCT_CFG_SUPPORT_MRS_DUOL_MODE_RF)&&defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
		case MRS_STA_TIMER_RF_TIME:
			mrsRfUartTimerFun();
			break;
	#endif
		//CJJ-HPLC:2018-9-7
	#if defined(SIGNAL_MCU)
		case MRS_STA_TIMER_RF_OVER_TIME:
			waitRfOverTimer();
			break;
			#endif
		//CJJ-HPLC:2018-9-7
    case MRS_STA_TIMER_POWERON_PLC_SEND:
        ret = mrsCltPowerOnPlcSendTimeOut();
        break;

    case MRS_STA_TIMER_CLT_POWEROFF_WAIT_METER:
        ret = mrsCltiiPowerOffRm();
        break;

    case MRS_STA_TIMER_COLLECT_VOLTAGE_BEGIN:
        mrsTfStaStartDataCollectProcess(MRS_PLC_TF_DATA_TYPE_VOLTAGE);
        break;
    case MRS_STA_TIMER_COLLECT_VOLTAGE:
        mrsTfStaRunDataCollectProcess(MRS_PLC_TF_DATA_TYPE_VOLTAGE);
        break;
    case MRS_STA_TIMER_COLLECT_FREQENCY_BEGIN:
        mrsTfStaStartDataCollectProcess(MRS_PLC_TF_DATA_TYPE_FREQUENCY);
        break;
    case MRS_STA_TIMER_COLLECT_FREQENCY:
        mrsTfStaRunDataCollectProcess(MRS_PLC_TF_DATA_TYPE_FREQUENCY);
        break;

    default:
        ret = HI_ERR_FAILURE;
        break;
    }

    return ret;
}


//*****************************************************************************
// ��������: mrsStaReport2CCO
// ��������: �ϱ�ģ��͵���ַӳ����CCO
//
// ����˵��:
//
// �� �� ֵ:
//    �ɹ�����  HI_ERR_SUCCESS
//
// ����Ҫ��:
// ���þ���:
// ��    ��: niesongsong/nkf62735 [2012-05-29]
//*****************************************************************************
HI_U32 mrsStaReport2CCO(HI_VOID)
{
    MRS_PLC_FRAME_DATA_STRU plc = {0};
    MRS_METER_LIST_S * meter_list = HI_NULL;
    MRS_GET_METETADDR_UP_STRU * report = HI_NULL;
    MRS_METER_ITEM_S * meter_item = HI_NULL;
    MRS_STA_SRV_CTX_STRU * sta = mrsStaGetContext();
    HI_U8 index = 0;
    HI_U8 * buf = HI_NULL;
    HI_U16 buf_len = 0;
    MRS_CMD_METER_INF * meter_inf = HI_NULL;
    HI_U8 num = 0;

    meter_list = &(sta->stMeterList);

    buf_len = sizeof(MRS_GET_METETADDR_UP_STRU);
    buf_len += meter_list->ucMeterNum * sizeof(MRS_CMD_METER_INF);

    buf = mrsToolsMalloc(buf_len);
    if (!buf)
    {
        return HI_ERR_MALLOC_FAILUE;
    }

    (hi_void)memset_s(buf, buf_len, 0, buf_len);

    report = (MRS_GET_METETADDR_UP_STRU *)buf;
    report->interface_ver = MRS_GET_DEVICE_INF_UP_VER_R1;
    report->stru_len = sizeof(MRS_GET_METETADDR_UP_STRU);
    report->meter_num = meter_list->ucMeterNum;

    meter_inf = (MRS_CMD_METER_INF *)report->meter_list;

    for (index = 0; index < PRODUCT_CFG_MRS_MAX_METER_IN_MAC_NUM; index++)
    {
        meter_item = &(meter_list->stMeterItem[index]);

        if (meter_item->ucValidFlag == 0)
        {
            continue;
        }

        if (++num > meter_list->ucMeterNum)
        {
            break;
        }

       (hi_void)memcpy_s(meter_inf->addr, HI_METER_ADDR_LEN, meter_item->ucMeterAddr, HI_METER_ADDR_LEN);
        meter_inf->type = MRS_STA_TYPE_METER;
        meter_inf->protocol = meter_item->ucProtocol;
        meter_inf->tx_num = 0;
        meter_inf->rx_num = 0;
        meter_inf++;
    }

    (hi_void)memset_s(plc.ucMac, sizeof(plc.ucMac), 0, sizeof(plc.ucMac));
    plc.usId = ID_MRS_CMD_GET_METERADDR_PLC;
    plc.pucPayload = buf;
    plc.usPayloadLen = buf_len;

    MRS_SendPlcFrame(&plc);

    mrsToolsFree(buf);

    return HI_ERR_SUCCESS;
}

HI_U32 mrsCltReportSmResult2CCO(HI_U8 *pucSrcMac, HI_U8 *pucDstMac, hi_bool flag)
{
    MRS_STA_SEARCH_CTX * pstSmCtx = mrsStaSearchCtx();
    MRS_SEARCH_METERLIST_STRU * pstMeterList = &(pstSmCtx->stMeterList);
    MRS_PLC_FRAME_DATA_STRU plc = {0};
    MRS_PLC_METERLIST_UP_S * report = HI_NULL;
    HI_U8 * payload = HI_NULL;
    HI_U16 payload_len = 0;
    MRS_CMD_METERLIST_ITEM *item = HI_NULL;
    HI_U8 i = 0;

    payload_len = sizeof(MRS_PLC_METERLIST_UP_S);
    payload_len += pstMeterList->ucMeterNum * sizeof(MRS_CMD_METERLIST_ITEM);

    payload = mrsToolsMalloc(payload_len);
    if (!payload)
    {
        return HI_ERR_MALLOC_FAILUE;
    }

    (hi_void)memset_s(payload, payload_len, 0, payload_len);

    report = (MRS_PLC_METERLIST_UP_S *)payload;
    report->interface_ver = MRS_GET_METERLIST_UP_VER;
    report->stru_len = sizeof(MRS_PLC_METERLIST_UP_S);
    report->option = MRS_SM_SEARCH_RESULT;
    report->status = flag;
    report->meter_num = pstMeterList->ucMeterNum;

    if (mrsToolsIsIICollector())
    {
        report->type = EN_MRS_STA_TYPE_CLT_II;
    }
    else
    {
        report->type = EN_MRS_STA_TYPE_CLT_I;
    }

    report->seq = pstSmCtx->stSearchCfg.ulSeq;

    {
        HI_NV_FTM_COLLECT_PARAM_FILE_S nv;

        (hi_void)memset_s(&nv, sizeof(nv), 0, sizeof(nv));

        if (HI_ERR_SUCCESS == HI_MDM_NV_Read(HI_NV_FTM_COLLECT_PARAM_FILE, &nv, sizeof(nv)))
        {
           (hi_void)memcpy_s(report->asset, HI_METER_ADDR_LEN, nv.CLTAddress, HI_METER_ADDR_LEN);
        }
        else
        {
            (hi_void)memset_s(report->asset, HI_METER_ADDR_LEN, 0xBB, HI_METER_ADDR_LEN);
        }
    }

    HI_MDM_GetLocalMacAddr(report->id);
    mrsHexInvert(report->id, HI_PLC_MAC_ADDR_LEN);

    if (pucSrcMac && pucDstMac)
    {
       (hi_void)memcpy_s(report->src, HI_PLC_MAC_ADDR_LEN, pucSrcMac, HI_PLC_MAC_ADDR_LEN);
       (hi_void)memcpy_s(report->dst, HI_PLC_MAC_ADDR_LEN, pucDstMac, HI_PLC_MAC_ADDR_LEN);
    }
    else
    {
       (hi_void)memcpy_s(report->src, HI_PLC_MAC_ADDR_LEN, MRS_BROADCAST_MAC, HI_PLC_MAC_ADDR_LEN);
       (hi_void)memcpy_s(report->dst, HI_PLC_MAC_ADDR_LEN, MRS_BROADCAST_MAC, HI_PLC_MAC_ADDR_LEN);
    }

    item = (MRS_CMD_METERLIST_ITEM *)report->meter_list;
    for (i = 0; i < pstMeterList->ucMeterNum; i++)
    {
       (hi_void)memcpy_s(item->addr, HI_METER_ADDR_LEN, pstMeterList->astMeter[i].ucMeterAddr, HI_METER_ADDR_LEN);
        item->protocol = pstMeterList->astMeter[i].ucProtocol;
        item->type = EN_MRS_STA_TYPE_METER;

        item++;
    }

    (hi_void)memset_s(&plc, sizeof(plc), 0, sizeof(plc));
    plc.usId = ID_MRS_CMD_GET_METERLIST;
    plc.pucPayload = payload;
    plc.usPayloadLen = payload_len;

    MRS_SendPlcFrame(&plc);
    mrsDfxCltSmPlcQueryTx();

    mrsToolsFree(payload);

    return HI_ERR_SUCCESS;
}



HI_U32 mrsStaReportSmResult2CCO(HI_U8 *pucSrcMac, HI_U8 *pucDstMac, HI_BOOL bFlag)
{
    MRS_STA_SRV_CTX_STRU * sta = mrsStaGetContext();
    MRS_STA_SEARCH_CTX * pstSmCtx = mrsStaSearchCtx();
    MRS_PLC_FRAME_DATA_STRU plc = {0};
    MRS_PLC_METERLIST_UP_S * report = HI_NULL;
    MRS_CMD_METERLIST_ITEM * meter_inf = HI_NULL;
    MRS_METER_LIST_S * pstMeterList = &(sta->stMeterList);
    MRS_METER_ITEM_S * meter_item = HI_NULL;
    HI_U8 * payload = HI_NULL;
    HI_U16 payload_len = 0;
    HI_U8 index = 0;
    HI_U8 num = 0;
    HI_U8 ucIPAddr[HI_IPV4_ADDR_LEN] = {0};

    if (!mrsToolsIsSTA() && !mrsToolsIsTSTA())
    {
        return HI_ERR_INVALID_TYPE;
    }

    payload_len = sizeof(MRS_PLC_METERLIST_UP_S);
    payload_len += pstMeterList->ucMeterNum * sizeof(MRS_CMD_METERLIST_ITEM);

    payload = mrsToolsMalloc(payload_len);
    if (!payload)
    {
        return HI_ERR_MALLOC_FAILUE;
    }

    (hi_void)memset_s(payload, payload_len, 0, payload_len);

    report = (MRS_PLC_METERLIST_UP_S *)payload;
    report->interface_ver = MRS_GET_METERLIST_UP_VER;
    report->stru_len = sizeof(MRS_PLC_METERLIST_UP_S);
    report->option = MRS_SM_SEARCH_RESULT;
    report->status = bFlag;
    report->meter_num = pstMeterList->ucMeterNum;
    report->type = pstMeterList->ucType;
    report->seq = pstSmCtx->stSearchCfg.ulSeq;

    HI_MDM_NM_GetAddr(report->asset, ucIPAddr);
    mrsHexInvert(report->asset, sizeof(report->asset));

    HI_MDM_GetLocalMacAddr(report->id);
    mrsHexInvert(report->id, sizeof(report->id));

/* BEGIN: PN: DTS2016021906890 MODIFY\ADD\DEL by cuiate/00233580 at 2016/2/20 */
    if (pucSrcMac && pucDstMac)
    {
       (hi_void)memcpy_s(report->src, HI_PLC_MAC_ADDR_LEN, pucSrcMac, HI_PLC_MAC_ADDR_LEN);
       (hi_void)memcpy_s(report->dst, HI_PLC_MAC_ADDR_LEN, pucDstMac, HI_PLC_MAC_ADDR_LEN);
    }
    else
    {
       (hi_void)memcpy_s(report->src, HI_PLC_MAC_ADDR_LEN, MRS_BROADCAST_MAC, HI_PLC_MAC_ADDR_LEN);
       (hi_void)memcpy_s(report->dst, HI_PLC_MAC_ADDR_LEN, MRS_BROADCAST_MAC, HI_PLC_MAC_ADDR_LEN);
    }
/* END:   PN: DTS2016021906890 MODIFY\ADD\DEL by cuiate/00233580 at 2016/2/20 */

    meter_inf = (MRS_CMD_METERLIST_ITEM *)report->meter_list;
    for (index = 0; (index < PRODUCT_CFG_MRS_MAX_METER_IN_MAC_NUM) && (num < report->meter_num); index++)
    {
        meter_item = &pstMeterList->stMeterItem[index];

        if (0 == meter_item->ucValidFlag)
        {
            continue;
        }

       (hi_void)memcpy_s(meter_inf->addr, HI_METER_ADDR_LEN, meter_item->ucMeterAddr, HI_METER_ADDR_LEN);
        meter_inf->protocol = meter_item->ucProtocol;
        meter_inf->type = EN_MRS_STA_TYPE_METER;
        num++;

        if (num == 1)
        {
           (hi_void)memcpy_s(report->asset, HI_METER_ADDR_LEN, meter_inf->addr, HI_METER_ADDR_LEN);
        }

        meter_inf++;
    }

    (hi_void)memset_s(&plc, sizeof(plc), 0, sizeof(plc));
    plc.usId = ID_MRS_CMD_GET_METERLIST;
    plc.pucPayload = payload;
    plc.usPayloadLen = payload_len;

    MRS_SendPlcFrame(&plc);
    mrsDfxCltSmPlcQueryTx();

    mrsToolsFree(payload);

    return HI_ERR_SUCCESS;
}


//*****************************************************************************
// ��������: mrsStaCheckReport2Cco
// ��������: ����Ƿ���Ҫ�����ַ�ϱ���CCO
//           ����������������mrsStaReport2CCO���������ϱ�
//           ��������������򷵻�
//
// ����˵��:
//   report_status  �ϱ����������Ǳ�����Ӧ
// �� �� ֵ:
//    �ɹ�����  HI_ERR_SUCCESS
//
// ����Ҫ��:
// ���þ���:
// ��    ��: liuxipeng/lkf54842 [2013-03-05]
//*****************************************************************************
HI_U32 mrsStaCheckReport2Cco(HI_U8 report_status)
{
    MRS_STA_SRV_CTX_STRU * sta = mrsStaGetContext();

    if ((report_status == MRS_STA_REPORT_METER_ADDR_ACTIVE)
                            && (sta->staRepFlag == MRS_STA_REPORT_METER_INFO_INVALID))
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    if ((report_status == MRS_STA_REPORT_METER_ADDR_CHANGE)
                            && (sta->meterChangeFlag == MRS_STA_REPORT_METER_INFO_INVALID))
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    return mrsStaReport2CCO();
}

HI_U32 mrsStaCreateReadMeterPlc(MRS_PLC_FRAME_DATA_STRU *frame_data,MRS_STA_ITEM * item
                                            ,HI_U8 *data,HI_U16 len,HI_U8 protocol)
{
    HI_U8 ucVer = MRS_AFN14_UP_VER;
    HI_U16 usDataLen = 0;
    MRS_DATA_TRANSMIT_UP_STRU *pstData = HI_NULL;
    HI_U8 *pucPayload = HI_NULL;

#if defined(PRODUCT_CFG_SUPPORT_TEST_CONCENTRATOR)
    MRS_CMD_DATA_TRANSMIT_STRU *pstTrans = HI_NULL;

    usDataLen = sizeof(MRS_DATA_TRANSMIT_UP_STRU) + len + sizeof(MRS_CMD_DATA_TRANSMIT_STRU);
    pucPayload = (HI_U8 *)mrsToolsMalloc(usDataLen);
    pstTrans = (MRS_CMD_DATA_TRANSMIT_STRU *)pucPayload;
#else
    usDataLen = sizeof(MRS_DATA_TRANSMIT_UP_STRU) + len;
    pucPayload = (HI_U8 *)mrsToolsMalloc(usDataLen);
    pstData = (MRS_DATA_TRANSMIT_UP_STRU *)pucPayload;
#endif

    if (!pucPayload)
    {
        return HI_ERR_NO_MORE_MEMORY;
    }

    (hi_void)memset_s(frame_data, sizeof(MRS_PLC_FRAME_DATA_STRU), 0, sizeof(MRS_PLC_FRAME_DATA_STRU));
    (hi_void)memset_s(pucPayload, usDataLen, 0, usDataLen);
    (hi_void)memcpy_s(frame_data->ucMac, HI_PLC_MAC_ADDR_LEN, item->dev_mac, HI_PLC_MAC_ADDR_LEN);

    if(item->id == ID_MRS_CMD_DATA_TRANSMIT_AFN13_PLC)
    {
        HI_PRV HI_U8 need_bc_cnt = 0;

        // ���α���Ϊ���й㲥
        if (item->bcFlg)
        {
            // ÿ��һ�����й㲥�Ž������й㲥
            if (need_bc_cnt)
            {
                (hi_void)memset_s(frame_data->ucMac, HI_PLC_MAC_ADDR_LEN, 0xFF, HI_PLC_MAC_ADDR_LEN);
            }

            need_bc_cnt = (need_bc_cnt + 1) & 0x01;
        }

        ucVer = MRS_AFN13_UP_VER;
    }
    else if(item->id != ID_MRS_CMD_DATA_TRANSMIT_AFN14_PLC)
    {
        mrsToolsFree(pucPayload);
        return HI_ERR_FAILURE;
    }

#if defined(PRODUCT_CFG_SUPPORT_TEST_CONCENTRATOR)
    pstData = (MRS_DATA_TRANSMIT_UP_STRU *)pstTrans->aucPayload;
#endif

    pstData->interface_ver = ucVer;
    pstData->stru_size     = sizeof(MRS_DATA_TRANSMIT_UP_STRU);
    pstData->protocol      = protocol;
    pstData->data_len      = len;
    pstData->seq           = item->seq;
    pstData->dir           = MRS_PLC_UP_FLG;

    // ��������
   (hi_void)memcpy_s(pstData->data, len, data, len);

#if defined(PRODUCT_CFG_SUPPORT_TEST_CONCENTRATOR)
    pstTrans->usCmdId = item->id;
    pstTrans->usPayloadLen = sizeof(MRS_DATA_TRANSMIT_UP_STRU) + len;
    frame_data->usId = ID_MRS_CMD_DATA_TRANSMIT_PLC;
#else
    frame_data->usId         = item->id;    // ��ȡҵ��ID
#endif
    frame_data->usPayloadLen = usDataLen;
    frame_data->pucPayload   = (HI_PBYTE)pucPayload;

#if defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG) && defined(MRS_LOG_STA_SWITCH)
    //mrsLogFrameInfStatRx(pstData->data, pstData->data_len, item->buf, item->len, 1);//optimized by weichao
#endif

    return HI_ERR_SUCCESS;
}

//*****************************************************************************
// ��������: mrsStaTransmit2CCO
// ��������: ת��PLC֡��ֻ������ID_MRS_CMD_DATA_TRANSMIT_PLC��
//
// ����˵��:
//   HI_U8 * data    ��������
//   HI_U16 len      ���ݳ���
//   HI_U8  protocol 645֡Э��
// �� �� ֵ:
//    �ɹ�����  HI_ERR_SUCCESS
//
// ����Ҫ��: �˺�������ϻỰ��Ϣ
// ���þ���:
// ��    ��: niesongsong/nkf62735 [2012-05-29]
//*****************************************************************************
HI_U32 mrsStaTransmit2CCO(HI_U8 * data, HI_U16 len, HI_U8 protocol, MRS_STA_ITEM * item)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    MRS_PLC_FRAME_DATA_STRU frame_data = { 0 };

    if (!item)
    {
        return HI_ERR_FAILURE;
    }

    //�������͹�Լ����
    if (!data || !mrsCheckValidProto(protocol))
    {
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_040, HI_DIAG_MT("param invalid"));
        return HI_ERR_FAILURE;
    }

    ret = mrsStaCreateReadMeterPlc(&frame_data, item, data, len, protocol);

    if (ret != HI_ERR_SUCCESS)
    {
        HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_041, HI_DIAG_MT("create plc err -[%d]"),ret);
        return HI_ERR_FAILURE;
    }

#if defined(PRODUCT_CFG_SUPPORT_TEST_CONCENTRATOR)
    HI_MDM_Sleep(100);
#endif

    ret = MRS_SendPlcFrame(&frame_data);
    if (ret != HI_ERR_SUCCESS)
    {
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_042, HI_DIAG_MT("tx plc fail"));
    }
    else
    {
        if(item->id == ID_MRS_CMD_DATA_TRANSMIT_AFN13_PLC)
        {
            mrsDfxXrPlcTx();
        }
        else if(item->id == ID_MRS_CMD_DATA_TRANSMIT_AFN14_PLC)
        {
            mrsDfxLrPlcTx();
        }
    }

/* BEGIN: Modified by fengxiaomin/00209182, 2014/3/27   ���ⵥ��:DTS2014032906596 */
    mrsDfxRmPlcUpAfn(item->id, frame_data.pucPayload, ret);
/* END:   Modified by fengxiaomin/00209182, 2014/3/27 */
    mrsToolsFree(frame_data.pucPayload);

    return ret;
}



//*****************************************************************************
// ��������: mrsStaGetMacAddr
// ��������: ��ȡģ��MAC��ַ��
//
// ����˵��:
//   HI_U8 macAddr[6]    �����ģ���ַ
// �� �� ֵ:
//    �ɹ�����  ģ���ַָ��
//
// ����Ҫ��:
// ���þ���:
// ��    ��: niesongsong/nkf62735 [2012-05-29]
//*****************************************************************************
HI_U8 * mrsStaGetMacAddr(HI_U8 macAddr[6])
{
    MRS_STA_SRV_CTX_STRU* pstaCtx = mrsStaGetContext();

    //�Ƿ�����Ч��MAC��ַ
    if (mrsToolsInvalidMac(pstaCtx->ucMacAddr))
    {
        HI_MDM_GetProductId(pstaCtx->ucMacAddr, HI_NULL, HI_NULL, HI_NULL);
    }

   (hi_void)memcpy_s(macAddr, HI_PLC_MAC_ADDR_LEN, pstaCtx->ucMacAddr, HI_PLC_MAC_ADDR_LEN);
    return pstaCtx->ucMacAddr;
}

//*****************************************************************************
// ��������: mrsStaSetMacAddr
// ��������: ����ģ��MAC��ַ��
//
// ����˵��:
//   HI_U8 macAddr[6]    �����ģ���ַ
// �� �� ֵ:
//    �ɹ�����  ģ���ַָ��
//
// ����Ҫ��:
// ���þ���:
// ��    ��: niesongsong/nkf62735 [2012-05-29]
//*****************************************************************************
HI_U8 * mrsStaSetMacAddr(HI_U8 macAddr[6])
{
    MRS_STA_SRV_CTX_STRU* pstaCtx = mrsStaGetContext();

    //ģ���ַ������ȫ0��㲥��ַ
    if (mrsToolsInvalidMac(macAddr))
    {
        return HI_NULL;
    }

    if (mrsToolsBroadCastMac(macAddr))
    {
        return HI_NULL;
    }
   (hi_void)memcpy_s(pstaCtx->ucMacAddr, HI_PLC_MAC_ADDR_LEN, macAddr, HI_PLC_MAC_ADDR_LEN);
#if defined(PRODUCT_CFG_SUPPORT_MRS_DUOL_MODE_RF)&&defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
	mrsOneSetRfSetAddress();
#endif
    return pstaCtx->ucMacAddr;
}

MRS_STA_SRV_CTX_STRU * mrsStaGetContext()
{
    return g_pstMrsStaSrvCtx;
}
MRS_STA_SRV_CTX_STRU * mrsStaSaveContext(MRS_STA_SRV_CTX_STRU *x)
{
    g_pstMrsStaSrvCtx=x;
    return g_pstMrsStaSrvCtx;
}

HI_U8 mrsStaGetState(HI_VOID)
{
    MRS_STA_SRV_CTX_STRU *sta = mrsStaGetContext();
    return sta->ucState;
}

HI_U8 mrsStaSetState(HI_U8 state)
{
    MRS_STA_SRV_CTX_STRU *sta = mrsStaGetContext();

    mrsDfxStaStatus(state);
    sta->ucState = state;
    return state;
}


MRS_PARALLEL_STA_CTX * mrsStaGetParallelCtx(HI_VOID)
{
    return &mrsStaGetContext()->stParallelCtx;
}

//�����Ļظ���������
HI_U32 mrsStaDiag2HSO(HI_U8 * data, HI_U16 len,HI_U8 histance)
{
    HI_NDM_645_FRAME_S pbuf = { 0 };

    if (!data)
    {
        return HI_ERR_FAILURE;
    }

    len = MRS_MIN(len, 256);
   (hi_void)memcpy_s(pbuf.aucFrame, sizeof(pbuf.aucFrame), data, len);
    pbuf.usFrameSize = len;

    HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1002, HI_DIAG_MT("To Ctrl"), pbuf.aucFrame, pbuf.usFrameSize);
    return HI_MDM_ReportPacket(HI_NDM_645_FRAME,histance,(HI_PBYTE)&pbuf,sizeof(HI_NDM_645_FRAME_S),HI_TRUE);
}

//������ע��
HI_U32 mrsStaTransmitRemoteCmd(HI_U16 ulId, HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption)
{
    HI_NDM_645_FRAME_S *pbuf = HI_NULL;

    HI_UNREF_PARAM(ulOption);

    if(!pCmdParam)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    if(ulId != HI_NDM_645_FRAME)
    {
        HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_047, HI_DIAG_MT("id err [0x%04X]"),ulId);
        return HI_ERR_INVALID_ID;
    }

    if(usCmdParamSize != sizeof(HI_NDM_645_FRAME_S))
    {
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_048, HI_DIAG_MT("len err"));
        return HI_ERR_BAD_DATA;
    }

    pbuf = (HI_NDM_645_FRAME_S *)pCmdParam;

    mrsStaTransmitCmd(pbuf->aucFrame, pbuf->usFrameSize ,MRS_STA_QUEUE_FROM_REMOTE);
    return HI_ERR_CONSUMED;
}

//HSOע��(����-Զ��)
HI_U32 mrsStaTransmitHsoCmd(HI_U16 ulId, HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption)
{
    DIAG_MRS_TRANSMIT_STRU * buf = HI_NULL;

    HI_UNREF_PARAM(ulId);

    buf = (DIAG_MRS_TRANSMIT_STRU *)pCmdParam;

    if((buf->ulLen + sizeof(buf->ulLen)) > (usCmdParamSize))
    {
        HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_049, HI_DIAG_MT("len err [%d]-[%d]"), buf->ulLen, usCmdParamSize);
        return HI_ERR_BAD_DATA;
    }

    if(ulOption == HI_DIAG_CMD_INSTANCE_IREMOTE)
    {
        mrsStaTransmitCmd(buf->data,(HI_U16)buf->ulLen,MRS_STA_QUEUE_FROM_REMOTE_HSO);
    }
    else
    {
        mrsStaTransmitCmd(buf->data,(HI_U16)buf->ulLen,MRS_STA_QUEUE_FROM_LOCAL);
    }
    return HI_ERR_CONSUMED;
}

#if defined(PRODUCT_CFG_HUNAN)
HI_U32 mrsGetModuleIdProc(HI_U8 *pData, HI_U16 usDataLen, HI_U16 usId, HI_U16 usSeq)
{
	HI_U32 ulRet = HI_ERR_SUCCESS;
	HI_U16 usPos = 0, usFrameLen = 0;
	MRS_645_FRAME_STRU st645Frame = {0};
		
	ulRet = MRS_Proto645Dec(pData, usDataLen, &st645Frame);
	if(ulRet != HI_ERR_SUCCESS)
	{
		return ulRet;
	}
	mrs645DataDecode(st645Frame.ucDataRealm, st645Frame.ucDataRealmLen);
	do
	{
		HI_U8 meter[6] = {0};
		HI_U8 ctrl = 0x1F;
		mrsStaGetMeterAddr(meter);
		if (!mrsMeterAddrMatch(st645Frame.ucAddr, meter))
		{
			ulRet = HI_ERR_FAILURE;
			break;
		}
		if (mrsToolsMemcmp(&st645Frame.stCtrl, &ctrl, sizeof(MRS_645_CTRL_STRU)) == 0 && st645Frame.ucDataRealmLen == 0)
		{
			HI_U8 module_id[11];
			MRS_STA_SRV_CTX_STRU *pstStaCtx = mrsStaGetContext();			
			MRS_PROTO645_FRAME_INF stFrameInf;
			HI_U8 *p645Data = HI_NULL;
    		HI_U16 us645DataLen = 0;
			mrsToolsMemZero_s(&stFrameInf, sizeof(stFrameInf), sizeof(stFrameInf));
		    mrsToolsMemcpy_s(stFrameInf.aucAddr, 6,st645Frame.ucAddr, 6);
			stFrameInf.ucCtrl = 0x9F;
			stFrameInf.ucDiSize = 0;
			mrsToolsMemcpy_s(module_id, sizeof(pstStaCtx->aucModuleId), pstStaCtx->aucModuleId, sizeof(pstStaCtx->aucModuleId));
			stFrameInf.ucDataLength = 11;
			stFrameInf.pData = module_id;
		    ulRet = mrsGen645Frame(&stFrameInf,&p645Data, &us645DataLen);
			if(ulRet == HI_ERR_SUCCESS)
			{
				MRS_STA_ITEM stItem;
				mrsToolsMemZero_s(&stItem, sizeof(stItem), sizeof(stItem));
    			stItem.id = usId;
    			stItem.seq = usSeq;    
    			ulRet = mrsStaTransmit2CCO(p645Data, us645DataLen, METER_PROTO_645_2007, &stItem);
			}
			mrsToolsFree(p645Data);
		}
		else
			ulRet = HI_ERR_SKIP;
	} while (0);

	return ulRet;
}

HI_U32 mrsGetModuleIdProcFromDiag(HI_U8 *pData, HI_U16 usDataLen)
{
	HI_U32 ulRet = HI_ERR_SUCCESS;
	HI_U16 usPos = 0, usFrameLen = 0;
	MRS_645_FRAME_STRU st645Frame = {0};
		
	ulRet = MRS_Proto645Dec(pData, usDataLen, &st645Frame);
	if(ulRet != HI_ERR_SUCCESS)
	{
		return ulRet;
	}
	mrs645DataDecode(st645Frame.ucDataRealm, st645Frame.ucDataRealmLen);
	do
	{
		HI_U8 meter[6] = {0};
		HI_U8 ctrl = 0x1F;
		mrsStaGetMeterAddr(meter);
		if (!mrsMeterAddrMatch(st645Frame.ucAddr, meter))
		{
			ulRet = HI_ERR_FAILURE;
			break;
		}
		if (mrsToolsMemcmp(&st645Frame.stCtrl, &ctrl, sizeof(MRS_645_CTRL_STRU)) == 0 && st645Frame.ucDataRealmLen == 0)
		{
			HI_U8 module_id[11];
			MRS_STA_SRV_CTX_STRU *pstStaCtx = mrsStaGetContext();			
			MRS_PROTO645_FRAME_INF stFrameInf;
			HI_U8 *p645Data = HI_NULL;
    		HI_U16 us645DataLen = 0;
			mrsToolsMemZero_s(&stFrameInf, sizeof(stFrameInf), sizeof(stFrameInf));
		    mrsToolsMemcpy_s(stFrameInf.aucAddr, 6,st645Frame.ucAddr, 6);
			stFrameInf.ucCtrl = 0x9F;
			stFrameInf.ucDiSize = 0;
			mrsToolsMemcpy_s(module_id, sizeof(pstStaCtx->aucModuleId), pstStaCtx->aucModuleId, sizeof(pstStaCtx->aucModuleId));
			stFrameInf.ucDataLength = 11;
			stFrameInf.pData = module_id;
		    ulRet = mrsGen645Frame(&stFrameInf,&p645Data, &us645DataLen);
			if(ulRet == HI_ERR_SUCCESS)
			{
				mrsStaDiag2HSO(p645Data, us645DataLen, HI_DIAG_CMD_INSTANCE_IREMOTE);
			}
			mrsToolsFree(p645Data);
		}
		else
			ulRet = HI_ERR_SKIP;
	} while (0);

	return ulRet;
}
#endif

#if defined(MRS_SRV_MODULE_ID)
HI_U32 mrsGetDeviceIdProcFromDiag(HI_U8 *pData, HI_U16 usDataLen)
{
	HI_U32 ulRet = HI_ERR_SUCCESS;
	//HI_U16 usPos = 0,usFrameLen = 0;
	MRS_645_FRAME_STRU st645Frame = {0};
		
	ulRet = MRS_Proto645Dec(pData, usDataLen, &st645Frame);
	if(ulRet != HI_ERR_SUCCESS)
	{
		return ulRet;
	}
	//mrs645DataDecode(st645Frame.ucDataRealm, st645Frame.ucDataRealmLen);
	do
	{
		HI_U8 meter[6] = {0};
		HI_U8 ctrl = 0x1F;
		HI_U8 *p645Data = HI_NULL;
    		HI_U16 us645DataLen = 0;
		HI_U8 ucIdLen = 0;
		HI_U16 offset = 0;
		mrsStaGetMeterAddr(meter);
#if defined(PRODUCT_CFG_CLTI_SUPPORT_MRS_SWITCH)
		if(mrsIsRealClti())
      		mrsToolsMemcpy_s(meter, HI_METER_ADDR_LEN, mrsGetCollectorAddr(), HI_METER_ADDR_LEN);
#endif
		if (!mrsMeterAddrMatch(st645Frame.ucAddr, meter))
		{
			ulRet = HI_ERR_FAILURE;
			break;
		}
		if (mrsToolsMemcmp(&st645Frame.stCtrl, &ctrl, sizeof(MRS_645_CTRL_STRU)) == 0 && st645Frame.ucDataRealmLen ==1)
		{
			
			if(st645Frame.ucDataRealm[0] == 1 || st645Frame.ucDataRealm[0] == 2)
			{
				if(st645Frame.ucDataRealm[0] == 1)//оƬID
				{
					ucIdLen = 24;
				}
				else
				{
					ucIdLen = 11;
				}
				us645DataLen = MRS_645_FRAME_LENGTH_MIN + 2 + ucIdLen;
				p645Data = mrsToolsMalloc(us645DataLen);
				if(!p645Data)
				{
					break;
				}
				mrsToolsMemZero_s(p645Data,us645DataLen,us645DataLen);
				p645Data[offset++] = 0x68;
				mrsToolsMemcpy_s(p645Data+offset, 6, meter,6);
				offset += 6;
				p645Data[offset++] = 0x68;
				p645Data[offset++] = 0x9f;
				p645Data[offset++] = 2+ucIdLen;
				p645Data[offset++] = st645Frame.ucDataRealm[0] ;
				p645Data[offset++] = ucIdLen;
				if(st645Frame.ucDataRealm[0] == 1)
				{
					#if 0
					EQUIP_CHIP_ID_R_STRU stInfo = {0};
					//HI_MDM_GetMyManagementId(p645Data+offset);
					mrsReadIDProc(&stInfo);
					mrsToolsMemcpy_s(p645Data+offset, 24, stInfo.chipId, 24);
					mrsHexInvert(p645Data+offset, 24);
					offset += ucIdLen;
					#else
				
					ulRet = mrsGetDevIdChip(p645Data+offset,MRS_PROTO645_DATAGRAM_DATA_LEN_MAX - offset);//weichao
					mrsHexInvert(p645Data+offset, MANAGEMENT_ID_SIZE);
					offset += ucIdLen;
					#endif
				}
				else
				{
					#if 0
					mrsToolsMemcpy_s(p645Data+offset, ucIdLen, mrsStaGetContext()->aucModuleId, ucIdLen);
					offset += ucIdLen;
					#else
					ulRet = mrsGetDevIdDev(p645Data+offset,MRS_PROTO645_DATAGRAM_DATA_LEN_MAX - offset);//weichao
					mrsHexInvert(p645Data+offset, MANAGEMENT_ID_SIZE);
					offset += ucIdLen;
					#endif
				}
				p645Data[offset] = mrsToolsCalcCheckSum(p645Data, offset);
				offset++;
				p645Data[offset++] = 0x16;
			}
			else
			{
				us645DataLen = MRS_645_FRAME_LENGTH_MIN + 1;
				p645Data = mrsToolsMalloc(us645DataLen);
				if(!p645Data)
				{
					break;
				}
				mrsToolsMemZero_s(p645Data,us645DataLen,us645DataLen);
				p645Data[offset++] = 0x68;
				mrsToolsMemcpy_s(p645Data+offset, 6, meter,6);
				offset += 6;
				p645Data[offset++] = 0x68;
				p645Data[offset++] = 0xdf;
				p645Data[offset++] = 1;
				p645Data[offset++] = 1;
				p645Data[offset] = mrsToolsCalcCheckSum(p645Data, offset);
				offset++;
				p645Data[offset++] = 0x16;
			}
			
			mrsStaDiag2HSO(p645Data, us645DataLen, HI_DIAG_CMD_INSTANCE_IREMOTE);
			mrsToolsFree(p645Data);
		}
		else
			ulRet = HI_ERR_SKIP;
	} while (0);

	return ulRet;
}
#endif

//HSO�볭�����������������
HI_U32 mrsStaTransmitCmd(HI_PBYTE data, HI_U16 len, HI_U8 from)
{
    MRS_STA_ITEM *item;
    MRS_STA_SRV_CTX_STRU *sta = mrsStaGetContext();

    if (!data)
    {
        return HI_ERR_FAILURE;
    }

    item = mrsToolsMalloc(sizeof(MRS_STA_ITEM)+len);
    if (!item)
    {
        return HI_ERR_FAILURE;
    }
    (hi_void)memset_s(item, sizeof(MRS_STA_ITEM), 0, sizeof(MRS_STA_ITEM));

    item->from = from;
    item->buf = (HI_U8*)item + sizeof(MRS_STA_ITEM);
    item->len = len;
    item->id = MRS_STA_ITEM_ID_TRANSMIT;
    item->time_out = (HI_U8)MRS_MS_TO_100MS(mrsGet645FrameHeadTimeout(MRS_STA_RM_CFG_AUTO));
    item->bAutoStrategy = HI_TRUE;
    item->ucFrameTimeout = (HI_U8)MRS_MS_TO_100MS(mrsGet645FrameInterval(MRS_STA_RM_CFG_AUTO));
    item->ucProtocol = METER_PROTO_TRANSPARENT;

   (hi_void)memcpy_s(item->buf, len, data, len);

    if (!mrsStaTryEnQueue(&sta->stQueModule,item))
    {
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_052, HI_DIAG_MT("en queue"));
        mrsToolsFree(item);
        return HI_ERR_FAILURE;
    }

    //֪ͨ����
    mrsStaNotifyQueue();

    return HI_ERR_SUCCESS;
}

HI_U32 mrsStaRS485TranData(HI_PBYTE data, HI_U16 len, HI_U8 from, HI_U16 usBaudRate)
{
    MRS_STA_ITEM *item;
    MRS_STA_SRV_CTX_STRU *sta = mrsStaGetContext();

    if (!data)
    {
        return HI_ERR_FAILURE;
    }

    item = mrsToolsMalloc(sizeof(MRS_STA_ITEM)+len);
    if (!item)
    {
        return HI_ERR_FAILURE;
    }
    (hi_void)memset_s(item, sizeof(MRS_STA_ITEM), 0, sizeof(MRS_STA_ITEM));

    item->from = from;
    item->buf = (HI_U8*)item + sizeof(MRS_STA_ITEM);
    item->len = len;
    item->id = MRS_STA_ITEM_ID_TRAN_DATA_CLT_II;
    item->time_out = MRS_MS_TO_100MS(MRS_RM_METER_OVERTIME_DEFAULT);
    item->option = (HI_U8)(usBaudRate/100);
#if defined(PRODUCT_CFG_CLTI_SUPPORT_MRS_SWITCH)
    if (mrsIsRealClti())
    {
        item->time_out += MRS_MS_TO_100MS(MRS_RM_METER_OVERTIME_DEFAULT);
    }
#endif
    item->bAutoStrategy = HI_TRUE;
    item->ucFrameTimeout = (HI_U8)MRS_MS_TO_100MS(mrsGet645FrameInterval(MRS_STA_RM_CFG_LOCAL));
    item->ucProtocol = METER_PROTO_TRANSPARENT;

   (hi_void)memcpy_s(item->buf,len,data,len);

    if (!mrsStaTryEnQueue(&sta->stQueModule,item))
    {
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_052, HI_DIAG_MT("en queue"));
        mrsToolsFree(item);
        return HI_ERR_FAILURE;
    }

    //֪ͨ����
    mrsStaNotifyQueue();

    return HI_ERR_SUCCESS;

}


// ���ұ��ַ�б�-�ҵ���ͬ�ĵ�ַ����HI_TRUE�������Ӧ�������±�
HI_BOOL mrsFindMeterList(MRS_METER_LIST_S * pMeterList,
                            HI_U8 addr[HI_METER_ADDR_LEN], HI_U16 * index)
{
    HI_U16 i = 0;
    MRS_METER_ITEM_S * pMeter = HI_NULL;
/* BEGIN: PN: DTS2015042907341 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/13 */
    HI_U8 aucMeterAddr[HI_METER_ADDR_LEN] = {0};
    HI_U8 aucTempAddr[HI_METER_ADDR_LEN] = {0};

    // ��ַ������97���ַ�����Ѹ��ֽ�AAHתΪ00H
    mrsMeterAddrPreProc(aucMeterAddr, addr, METER_PROTO_645_1997);

    pMeter = pMeterList->stMeterItem;
    for(i = 0; i < PRODUCT_CFG_MRS_MAX_METER_IN_MAC_NUM; i++)
    {
        if(pMeter[i].ucValidFlag)
        {
            mrsMeterAddrPreProc(aucTempAddr, pMeter[i].ucMeterAddr, pMeter[i].ucProtocol);
            if (mrsToolsMemEq(aucMeterAddr, aucTempAddr, HI_METER_ADDR_LEN))
            {
                *index = i;
                return HI_TRUE;
            }
        }
    }
/* END:   PN: DTS2015042907341 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/13 */

    return HI_FALSE;
}


#if defined(PRODUCT_CFG_SYS_RST_SOLUTION)
HI_U32 mrsStaRxUartStat(HI_VOID)
{
    MRS_STA_SRV_CTX_STRU * sta = mrsStaGetContext();
    return sta->stStaResetInfo.ulRxUartTimes;
}

HI_U32 mrsRxRMPlcStat(HI_VOID)
{
    MRS_STA_SRV_CTX_STRU * sta = mrsStaGetContext();
    return sta->stStaResetInfo.ulRxRMPlcTimes;
}

#endif

// ���ݻ������еı��������⴦��:
// 13/14���¼���ѯ����: �ڻ������л����ظ��ı���
// �㲥����: ����CCO�·��ı���
// �Ƚ�ʱ����id��seq�Ƚϣ��������ͬ�ı��ģ���ֱ�ӷ��س������(13/14���¼���ѯ)�򲻴���(�㲥����)
// ����HI_TRUE:�ҵ��ظ�PLC֡ ����HI_FALSE:δ�ҵ��ظ�PLC֡
HI_BOOL mrsStaPlcRetryFiltrate(MRS_STA_ITEM * item)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U8 * buf = HI_NULL;
    HI_U16 len = 0;
    HI_U8 protocol = 0;

    if (item->id == ID_MRS_CMD_BROADCAST_TRANSMIT_PLC)
    {
        ret = mrsStaBroadCastBufFind(item->buf, item->len);
        if(HI_ERR_SUCCESS != ret)
        {
            return HI_FALSE;
        }

        return HI_TRUE;
    }

    ret = mrsStaFrameBufFind(item, &len, &buf, &protocol);
    if (HI_ERR_SUCCESS != ret)
    {
        return HI_FALSE;
    }

    switch (item->id)
    {
    case ID_MRS_CMD_DATA_TRANSMIT_AFN13_PLC:
        if (MRS_STA_TRANSFER_TIMING_NORMAL == item->sub_id)
        {
            break;
        }
        // ��break
    case ID_MRS_CMD_DATA_TRANSMIT_AFN14_PLC:
        mrsStaTransmit2CCO(buf, len, protocol, item);
        break;

    case ID_MRS_CMD_BROADCAST_TRANSMIT_PLC:
        break;

    default:
        return HI_FALSE;
    }

    return HI_TRUE;
}


HI_U32 mrsStaFrameBufInsert(MRS_STA_ITEM *pstItem, HI_U16 frame_len, HI_U8 * frame_data, HI_U8 protocol)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    MRS_STA_SRV_CTX_STRU * sta = mrsStaGetContext();
    MRS_STA_FRAME_BUF_CTX * ctx = HI_NULL;
    MRS_STA_FRAME_BUF_ITEM_STRU * item = HI_NULL;

    if (!frame_data || (frame_len > MRS_STA_FRAME_BUF_ITEM_BUFSIZE))
    {
        return HI_ERR_FAILURE;
    }

    ctx = &(sta->stFrameBufCtx);
    item = &(ctx->item[ctx->current]);
    item->id = pstItem->id;
    item->seq = pstItem->seq;
    item->valid = 1;
    item->frame_len = frame_len;
    item->protocol = protocol;
    item->timestamp = HI_MDM_GetSeconds();
   (hi_void)memcpy_s(item->frame_buf, frame_len, frame_data, frame_len);
    (hi_void)memcpy_s(item->dev_mac, HI_PLC_MAC_ADDR_LEN, pstItem->dev_mac, HI_PLC_MAC_ADDR_LEN);

    ctx->current += 1;
    if (ctx->current >= ctx->max_count)
    {
        ctx->current = 0;
    }


    return ret;
}

HI_U32 mrsStaBroadCastBufFind(HI_U8 * frame_data, HI_U16 frame_len)
{
    HI_U32 ret = HI_ERR_FAILURE;
    HI_U32 i = 0;
    HI_U32 timestamp = 0;
    MRS_STA_FRAME_BUF_CTX * ctx = HI_NULL;
    MRS_STA_SRV_CTX_STRU * sta = mrsStaGetContext();
    MRS_STA_FRAME_BUF_ITEM_STRU * item = HI_NULL;

    timestamp = HI_MDM_GetSeconds();
    ctx = &(sta->stFrameBufCtx);
    for (i = 0; i < ctx->max_count; i++)
    {
        item = &(ctx->item[i]);
        if (!item->valid)
        {
            continue;
        }

        if (timestamp > item->timestamp + ctx->timeout)
        {
            item->valid = 0;
            continue;
        }

        if ((ID_MRS_CMD_BROADCAST_TRANSMIT_PLC == item->id)
            && (frame_len == item->frame_len)
            && (memcmp(item->frame_buf, frame_data, frame_len) == 0))
        {
            ret = HI_ERR_SUCCESS;
            item->timestamp = HI_MDM_GetSeconds();

            return ret;
        }

    }

    return ret;
}



HI_U32 mrsStaFrameBufFind(MRS_STA_ITEM * pst_item, HI_U16 *frame_len, HI_U8 ** frame_data, HI_U8 * protocol)
{
    HI_U32 ret = HI_ERR_FAILURE;
    HI_U32 i;
    HI_U32 timestamp = 0;
    MRS_STA_SRV_CTX_STRU * sta = mrsStaGetContext();
    MRS_STA_FRAME_BUF_CTX * ctx = HI_NULL;
    MRS_STA_FRAME_BUF_ITEM_STRU * item = HI_NULL;

    if (!frame_data || !frame_len)
    {
        return ret;
    }

    timestamp = HI_MDM_GetSeconds();
    ctx = &(sta->stFrameBufCtx);
    for (i = 0; i < ctx->max_count; i++)
    {
        item = &(ctx->item[i]);
        if (!item->valid)
        {
            continue;
        }

        if (timestamp > item->timestamp + ctx->timeout)
        {
            item->valid = 0;
            continue;
        }

        if (pst_item->id == item->id && pst_item->seq == item->seq &&
			mrsToolsMemEq(pst_item->dev_mac,item->dev_mac,HI_PLC_MAC_ADDR_LEN))
        {
            *frame_len = item->frame_len;
            *frame_data = item->frame_buf;
            *protocol = item->protocol;
            ret = HI_ERR_SUCCESS;
            item->timestamp = HI_MDM_GetSeconds();

            return ret;
        }
    }

    return ret;
}


// ��ȡPLC�շ�ģʽ
HI_U8 mrsStaGetPlcRxTxMode(HI_VOID)
{
    MRS_STA_SRV_CTX_STRU * sta = mrsStaGetContext();
    return sta->plc_rxtx_mode;
}


HI_U32 mrsCmdStaStartSearchMeter(HI_U16 usId, HI_PVOID pstReq, HI_U16 usReqPacketSize)
{
    MRS_STA_SEARCH_CTX * pstSearchCtx = mrsStaSearchCtx();
    MRS_PLC_FRAME_DATA_STRU *plc = (MRS_PLC_FRAME_DATA_STRU *)pstReq;
    MRS_PLC_START_SEARCH_METER_S *start = HI_NULL;

    if ((HI_NULL == plc) || (HI_NULL == plc->pucPayload))
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    MRS_NOT_USED(usId);
    MRS_NOT_USED(usReqPacketSize);

    start = (MRS_PLC_START_SEARCH_METER_S *)plc->pucPayload;

    switch (start->interface_ver)
    {
        case MRS_START_SEARCH_DL_VER_R0:
        default:
            if (plc->usPayloadLen != start->stru_len)
            {
                return HI_ERR_INVALID_PARAMETER;
            }

            if (MRS_SM_START_SEARCH != start->option)
            {
                return HI_ERR_INVALID_PARAMETER;
            }

            break;
    }

    if (pstSearchCtx->stSearchCfg.ulSeq == start->seq)
    {
        mrsDfxCltSmPlcStartRx(start->seq);
        return HI_ERR_SUCCESS;
    }

    pstSearchCtx->stSearchCfg.ulSeq = start->seq;
    mrsDfxCltSmPlcStartRx(start->seq);

    if (MRS_SEARCH_STATUS_IN_PROGRESS == mrsStaSearchGetStatus())
    {
        return HI_ERR_BUSY;
    }

    return mrsStaSearchStart(HI_FALSE);
}


HI_U32 mrsCmdStaStopSearchMeter(HI_U16 usId, HI_PVOID pstReq, HI_U16 usReqPacketSize)
{
    MRS_STA_SEARCH_CTX * pstSearchCtx = mrsStaSearchCtx();
    MRS_PLC_FRAME_DATA_STRU *plc = (MRS_PLC_FRAME_DATA_STRU *)pstReq;
    MRS_PLC_STOP_SEARCH_METER_S *stop = HI_NULL;

    HI_ASSERT(ID_MRS_CMD_STOP_SEARCH_METER == usId);

    if ((HI_NULL == plc) || (HI_NULL == plc->pucPayload))
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    MRS_NOT_USED(usId);
    MRS_NOT_USED(usReqPacketSize);

    stop = (MRS_PLC_STOP_SEARCH_METER_S *)plc->pucPayload;

    switch (stop->interface_ver)
    {
        case MRS_STOP_SEARCH_DL_VER_R0:
        default:
            if (plc->usPayloadLen != stop->stru_len)
            {
                return HI_ERR_INVALID_PARAMETER;
            }
            break;
    }

    if (MRS_SEARCH_STATUS_IN_PROGRESS != mrsStaSearchGetStatus())
    {
        return HI_ERR_SUCCESS;
    }

    if (pstSearchCtx->stSearchCfg.ulSeq != stop->seq)
    {
        return HI_ERR_FAILURE;
    }

    return mrsStaSearchStop();
}


HI_U32 mrsCmdStaPlcTestTransmit(hi_u16 id, hi_pvoid packet, hi_u16 packet_size)
{
    MRS_PLC_FRAME_DATA_STRU *plc_rx = (MRS_PLC_FRAME_DATA_STRU *)packet;
    MRS_PLC_FRAME_DATA_STRU plc_tx = {0};
    MRS_TEST_TRANSMIT_AFNF0_STRU *frame_rx = HI_NULL;
    MRS_TEST_TRANSMIT_AFNF0_STRU *frame_tx = HI_NULL;

    hi_pbyte payload = HI_NULL;
    hi_u16 payload_len = 0;

    HI_UNREF_PARAM(id);
    HI_UNREF_PARAM(packet_size);

    if (packet == HI_NULL) {
        return HI_ERR_INVALID_PARAMETER;
    }

    if (plc_rx->pucPayload == HI_NULL) {
        return HI_ERR_INVALID_PARAMETER;
    }

    frame_rx = (MRS_TEST_TRANSMIT_AFNF0_STRU *)plc_rx->pucPayload;
    /*
     * ���ĳ��ȼ��: �ܳ��� = ͷ���� + ���ݳ��ȣ��������Ȳ�����512�ֽ�
     */
    if ((plc_rx->usPayloadLen != frame_rx->stru_len + frame_rx->data_len) || (frame_rx->expect_len >= 512)) {
        return HI_ERR_INVALID_PARAMETER;
    }

    payload_len = sizeof(MRS_TEST_TRANSMIT_AFNF0_STRU) + frame_rx->expect_len;
    payload = mrsToolsMalloc(payload_len);
    if (payload == HI_NULL) {
        return HI_ERR_MALLOC_FAILUE;
    }

    (hi_void)memset_s(payload, payload_len, 0, payload_len);
    frame_tx = (MRS_TEST_TRANSMIT_AFNF0_STRU *)payload;
    frame_tx->interface_ver = MRS_TEST_AFNF0_UP_VER;
    frame_tx->stru_len = sizeof(MRS_TEST_TRANSMIT_AFNF0_STRU);
    frame_tx->data_len = frame_rx->data_len;
    frame_tx->expect_len = frame_rx->expect_len;
    (hi_void)memcpy_s(payload + frame_tx->stru_len, frame_tx->expect_len, plc_rx->pucPayload + frame_rx->stru_len,
        MRS_MIN(frame_rx->data_len, frame_rx->expect_len));

    (hi_void)memset_s(&plc_tx, sizeof(plc_tx), 0, sizeof(plc_tx));
    plc_tx.usId = ID_MRS_CMD_TEST_TRANSMIT_AFNF0_PLC;
    plc_tx.pucPayload = payload;
    plc_tx.usPayloadLen = payload_len;

    MRS_SendPlcFrame(&plc_tx);

    mrsToolsFree(payload);

    return HI_ERR_SUCCESS;
}

// ˢ���׽ڵ���Ϣ
// STA:��������ַ
// �ɼ���:��������ַ
HI_VOID mrsStaRefreshTopNodeInf(HI_U8 *pucAddr, HI_U8 ucProtocol)
{
    MRS_STA_SRV_CTX_STRU * sta  = mrsStaGetContext();
    MRS_METER_ITEM_S * NodeAddr = HI_NULL;

    if(!mrsToolsNormalAddr(pucAddr))
    {
        return;
    }

    NodeAddr = &sta->stMeterList.stNodeMeter;

   (hi_void)memcpy_s(NodeAddr->ucMeterAddr, HI_METER_ADDR_LEN, pucAddr, HI_METER_ADDR_LEN);

    if (ucProtocol > NodeAddr->ucProtocol)
    {
        NodeAddr->ucProtocol = ucProtocol;
    }

    NodeAddr->ucValidFlag = HI_TRUE;

    return;
}


// ���ñ��ַ��Mac�㲢���²�Ʒ����
HI_U32 mrsStaSetMeterToMac(HI_U8 *pucMeter, HI_U8 ucProductType, HI_U8 ucMacType)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_MAC_ATTR_STRU *pstMacInf = HI_NULL;
    HI_U16 usInfLen = 0;
    HI_U8 ucMacAddr[HI_PLC_MAC_ADDR_LEN] = {0};

    do
    {
        HI_MDM_SetSTAProductType(ucProductType);

       (hi_void)memcpy_s(ucMacAddr, HI_PLC_MAC_ADDR_LEN, pucMeter, HI_PLC_MAC_ADDR_LEN);

        usInfLen = sizeof(HI_MAC_ATTR_STRU) + sizeof(HI_MAC_ENTRY_STRU);
        pstMacInf = mrsToolsMalloc(usInfLen);
        if(!pstMacInf)
        {
            return HI_ERR_NO_MORE_MEMORY;
        }

        (hi_void)memset_s(pstMacInf, usInfLen, 0, usInfLen);
        pstMacInf->list_num = 1;
        pstMacInf->clear_flag = HI_TRUE;
        pstMacInf->type = ucMacType;
       (hi_void)memcpy_s(pstMacInf->mac_list[0].mac, HI_PLC_MAC_ADDR_LEN, ucMacAddr, HI_PLC_MAC_ADDR_LEN);

        ret = HI_MDM_SetBackupMacAddr(pstMacInf);
        mrsDfxStaSetAddr2Mac(ucMacAddr);

        mrsToolsFree(pstMacInf);

    } while (0);

    if((ret == HI_ERR_SUCCESS)
        && (!mrsToolsBroadCastMac(ucMacAddr)))
    {
        // ���óɹ�����±��ر����Mac��ַ
        mrsStaSetMacAddr(ucMacAddr);
    }

    return ret;
}


// ͬ��Mac��������ַ
HI_U32 mrsStaSyncMacJoinNetAddr(HI_VOID)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U8 ucLocalMac[HI_PLC_MAC_ADDR_LEN] = {0};
    HI_U8 ucCurrMac[HI_PLC_MAC_ADDR_LEN] = {0};
    HI_U8 ucIp[HI_IPV4_ADDR_LEN] = {0};

    ret = HI_MDM_NM_GetAddr(ucCurrMac, ucIp);
    if(ret != HI_ERR_SUCCESS)
    {
        return ret;
    }

    mrsStaGetMacAddr(ucLocalMac);

    if(!mrsToolsMemEq(ucLocalMac, ucCurrMac, HI_PLC_MAC_ADDR_LEN))
    {
        // ����Sta��������Mac��ַ
        mrsStaSetMacAddr(ucCurrMac);

        if(mrsToolsIsMacAddrFromMeter(ucCurrMac))
        {
            // ����Nv������ַ
            mrsStaSaveJoinMeterAddr(ucCurrMac, METER_PROTO_645_2007, 0);

            if (mrsToolsIsIICollector())
            {
                mrsSetCollectDevAddr(HI_NV_FTM_COLLECT_PARAM_FILE);
                return HI_ERR_SUCCESS;
            }

            // ����"DevId"��ַ
            HI_MDM_DevIdConfig(ucCurrMac);
        }
    }

    return HI_ERR_SUCCESS;
}

// ���ѱ����б��л�ȡָ�����ַ��Ϣ
MRS_METER_ITEM_S* mrsStaGetMeterInfFromSmResult(HI_U8 *pucMac)
{
    MRS_STA_SEARCH_CTX *pstSmCtx = mrsStaSearchCtx();
    MRS_SEARCH_METERLIST_STRU *pstMList = &(pstSmCtx->stMeterList);
    HI_U8 aucTempMac[HI_PLC_MAC_ADDR_LEN] = { 0 };
    HI_U8 i = 0;

    for(i = 0; i < pstMList->ucMeterNum; i++)
    {
        mrsToolsConvertMeter2Mac(pstMList->astMeter[i].ucMeterAddr,
                                 pstMList->astMeter[i].ucProtocol,
                                 aucTempMac);

        if(mrsToolsMemEq(aucTempMac, pucMac, HI_PLC_MAC_ADDR_LEN))
        {
            return &pstMList->astMeter[i];
        }
    }

    return HI_NULL;
}

// �洢�������ַ��Nv��
HI_VOID mrsStaSaveJoinMeterAddr(HI_U8 *pucMac, HI_U8 ucProtocol, HI_U16 usDi97)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    MRS_STA_SEARCH_CTX *pstSmCtx = mrsStaSearchCtx();
    MRS_METER_ITEM_S *pstMeterInf = HI_NULL;
    HI_NV_FTM_COLLECT_PARAM_FILE_S nv;
    HI_U8 aucTempMeter[HI_METER_ADDR_LEN] = { 0 };
    HI_U16 usBaudRate = MRS_SRV_BAUD_RATE_DEFAULT;
    HI_U8 ucIdx = MRS_SRV_BAUD_RATE_IDX_DEFAULT;
#if defined(PRODUCT_CFG_CLTI_SUPPORT_MRS_SWITCH)
    if (!mrsIsRealClti() && !mrsToolsIsIICollector())
    {
        return;
    }
#else
	if (!mrsToolsIsIICollector())
	{
		return;
	}
#endif

    pstMeterInf = mrsStaGetMeterInfFromSmResult(pucMac);
    if(pstMeterInf)
    {
       (hi_void)memcpy_s(aucTempMeter, HI_METER_ADDR_LEN, pstMeterInf->ucMeterAddr, HI_METER_ADDR_LEN);
        ucProtocol = pstMeterInf->ucProtocol;
        usDi97 = pstMeterInf->usOption;
        usBaudRate = pstMeterInf->usBaudRate;
    }
    else
    {
        MRS_SRV_BAUDRATE_ID_E eId = MRS_SRV_ID_BAUDRATE_SM_07;
        P_MRS_SRV_BAUDRATE_CFG_S pstCfg = mrsSrvGetBaudRateCfg(eId);

        switch (ucProtocol)
        {
        case METER_PROTO_698_45:
            eId = MRS_SRV_ID_BAUDRATE_SM_698;
            break;

        case METER_PROTO_645_1997:
            eId = MRS_SRV_ID_BAUDRATE_SM_97;
            break;

        default:
            eId = MRS_SRV_ID_BAUDRATE_SM_07;
            break;
        }

       (hi_void)memcpy_s(aucTempMeter, HI_METER_ADDR_LEN, pucMac, HI_METER_ADDR_LEN);
        mrsHexInvert(aucTempMeter, HI_METER_ADDR_LEN);

        if (pstCfg)
        {
            usBaudRate = pstCfg->usBaudRateList[0];
        }
    }

    if (HI_ERR_SUCCESS != mrsSrvBaudRate2Index(usBaudRate, &ucIdx))
    {
        ucIdx = MRS_SRV_BAUD_RATE_IDX_DEFAULT;
    }

   (hi_void)memcpy_s(pstSmCtx->stSearchCfg.aucNvMeter, HI_METER_ADDR_LEN, aucTempMeter, HI_METER_ADDR_LEN);
    pstSmCtx->stSearchCfg.ucNvProtocol = ucProtocol;
    pstSmCtx->stSearchCfg.usNvDI97 = usDi97;
    pstSmCtx->stSearchCfg.ucBaudRateIndex = ucIdx;

    (hi_void)memset_s(&nv, sizeof(nv), 0, sizeof(nv));
    ret = HI_MDM_NV_Read(HI_NV_FTM_COLLECT_PARAM_FILE, &nv, (HI_U16)sizeof(nv));
    if ((HI_ERR_SUCCESS == ret)
        && (!mrsToolsMemEq(nv.meter, aucTempMeter, HI_METER_ADDR_LEN)
            || (nv.protocol != ucProtocol)
            || (nv.DI_v97 != usDi97)
            || (nv.baudrate_index != ucIdx)))
    {
       (hi_void)memcpy_s(nv.meter, HI_METER_ADDR_LEN, aucTempMeter, HI_METER_ADDR_LEN);
        nv.protocol = ucProtocol;
        nv.DI_v97 = usDi97;
        nv.baudrate_index = ucIdx;
        (HI_VOID)HI_MDM_NV_Write(HI_NV_FTM_COLLECT_PARAM_FILE, &nv, (HI_U16)sizeof(nv));
    }
#if defined(PRODUCT_CFG_CLTI_SUPPORT_MRS_SWITCH)
    if (mrsIsRealClti())
    {
        mrsHexInvert(aucTempMeter, HI_METER_ADDR_LEN);
        HI_MDM_DevIdConfig(aucTempMeter);
    }
#endif
}

// ����STA��Ʒ����(��ͨ���ģ��:I�Ͳɼ���ģ��)
HI_VOID mrsStaSetType(HI_U8 ucType)
{
    if(mrsToolsIsSTA() || mrsToolsIsTSTA())
    {
        MRS_STA_SRV_CTX_STRU *pstStaCtx = mrsStaGetContext();

        pstStaCtx->sta_type = ucType;
        pstStaCtx->stMeterList.ucType = (ucType == STA_TYPE_METER)? \
                                        MRS_STA_TYPE_METER : MRS_STA_TYPE_COLLECTOR;
    }

    return;
}

// ��ȡSTA��Ʒ����
HI_U8 mrsStaGetType(HI_VOID)
{
    if(mrsToolsIsSTA() || mrsToolsIsTSTA())
    {
        return mrsStaGetContext()->sta_type;
    }

    return STA_TYPE_INVALID;
}

// ��ȡSTA��PLCͨ��״̬
HI_BOOL mrsStaGetPlcChlStatus(HI_VOID)
{
    return mrsStaGetContext()->plcState;
}


/**
* @brief STA�����������籨��
*
* @par Description:
*     �����ǰSTAδ����������������������Ϣ�ɹ��ͻظ�����Ӧ��
*     �����ǰSTA�Ѿ�����:
*          1. ���ѱ����кź��ϴ������ɹ����ѱ����кŲ�һ��������������������Ϣ�ɹ��ͻظ�����Ӧ��
*          2. ����ֱ�ӻظ�����Ӧ��;
*/
HI_U32 mrsStaHandleRcvBindNetwork(MRS_PLC_METERLIST_DL_S * pstFrame)
{
    MRS_STA_SEARCH_CTX * pstSearchCtx = mrsStaSearchCtx();
    HI_MDM_LOCK_INFO_STRU stLockInfo = {0};
    HI_U32 ulRet = HI_ERR_SUCCESS;

    HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_020, HI_DIAG_MT("�յ�����Ϣ"));

    // �����I�ɻ���II�ɣ��������кŶԲ��ϣ�������
#if defined(PRODUCT_CFG_CLTI_SUPPORT_MRS_SWITCH)
    if (mrsCltiSearchIsEnable() || mrsCltiiSearchIsEnable())
#else
    if (mrsCltiiSearchIsEnable())
#endif
    {
        if (pstSearchCtx->stSearchCfg.ulSeq != pstFrame->seq)
        {
            HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_002, HI_DIAG_MT("������ŶԲ��� [cfg_seq] [frm_seq]"),
                pstSearchCtx->stSearchCfg.ulSeq, pstFrame->seq);
            return HI_ERR_FAILURE;
        }
    }

    // ��ѯ��ǰ������Ϣ
    ulRet = HI_MDM_GetLockInfo(&stLockInfo);
    if (HI_ERR_SUCCESS != ulRet)
    {
        HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_003, HI_DIAG_MT("��ѯ������Ϣʧ�� [ret]"), ulRet);
        return ulRet;
    }

    if (HI_MDM_UNLOCK_NETWORK_STATUS == stLockInfo.lock_status || pstSearchCtx->stSearchCfg.ulLastLockSuccSeq != pstFrame->seq)
    {
        ulRet = mrsStaBindNetwork();
        if (HI_ERR_SUCCESS == ulRet)
        {
            pstSearchCtx->stSearchCfg.ulLastLockSuccSeq = pstFrame->seq;
            mrsStaBindNetworkReply(pstFrame);
        }

        return ulRet;
    }
    else
    {
        HI_DIAG_LOG_MSG_E3(MRS_FILE_LOG_FLAG_004, HI_DIAG_MT("������ֱ�ӻظ�����Ӧ�� [lock_stat] [last_seq] [cur_seq]"),
            stLockInfo.lock_status, pstSearchCtx->stSearchCfg.ulLastLockSuccSeq, pstFrame->seq);
        return mrsStaBindNetworkReply(pstFrame);
    }
}


// �Ƿ�������������
HI_BOOL mrsStaBindNetworkEnable(HI_VOID)
{
    MRS_STA_SRV_CTX_STRU *pstStaCtx = mrsStaGetContext();

    if (pstStaCtx && SOFT_TF_ON_WITH_STA_JOIN_BY_TF_RESULT == pstStaCtx->ucSoftTfMode && pstStaCtx->ucBindNetwork)
    {
        return HI_TRUE;
    }
    else
    {
        return HI_FALSE;
    }
}


HI_U32 mrsStaBindNetwork(HI_VOID)
{
    MRS_STA_SRV_CTX_STRU *pstStaCtx = mrsStaGetContext();
    HI_U32 ulRet = HI_ERR_SUCCESS;

    if (mrsStaBindNetworkEnable())
    {
        HI_U32 ulDuration = (0xFFFF == pstStaCtx->usBindDuration) ? 0xFFFFFFFF : ((HI_U32)pstStaCtx->usBindDuration * 60);

        ulRet = HI_MDM_LockNetwork(ulDuration);
        if (HI_ERR_SUCCESS != ulRet)
        {
            HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_005, HI_DIAG_MT("����������Ϣʧ�� [ret] [duration]"), ulRet, ulDuration);
            return ulRet;
        }
    }

    return HI_ERR_SUCCESS;
}


HI_U32 mrsStaBindNetworkReply(MRS_PLC_METERLIST_DL_S * pstFrame)
{
    MRS_PLC_FRAME_DATA_STRU stPlcFrame;
    MRS_PLC_METERLIST_UP_S *pstUpFrame = HI_NULL;
    HI_U8 * aucPayload = HI_NULL;
    HI_U16 usPayloadLen = 0;

    usPayloadLen = sizeof(MRS_PLC_METERLIST_UP_S);
    aucPayload = mrsToolsMalloc(usPayloadLen);
    if (!aucPayload)
    {
        return HI_ERR_MALLOC_FAILUE;
    }

    (hi_void)memset_s(aucPayload, usPayloadLen, 0, usPayloadLen);

    pstUpFrame = (MRS_PLC_METERLIST_UP_S *)aucPayload;

    mrsStaFillMeterListUpFrameHead(pstFrame->dst, pstFrame->src, pstFrame->seq, pstUpFrame);
    pstUpFrame->option = MRS_SM_BIND_NETWORK;

    (hi_void)memset_s(&stPlcFrame, sizeof(stPlcFrame), 0, sizeof(stPlcFrame));
    stPlcFrame.usId = ID_MRS_CMD_GET_METERLIST;
    stPlcFrame.pucPayload = aucPayload;
    stPlcFrame.usPayloadLen = usPayloadLen;

    MRS_SendPlcFrame(&stPlcFrame);

    mrsToolsFree(aucPayload);

    return HI_ERR_SUCCESS;
}


/**
* @brief ��������ʱ�����޼�飬�ﵽ���޾�����������
*
* @par Description
*     ������ʱ���������ޣ����ڴ�����ʱ���ڣ�����ʱ��ռ�ȴ�������ʱ��
* ��������������Ϊ����Ե����磬Ȼ�������Ȼ��������
*
*/
HI_U32 mrsNetworkCheckTimeout(HI_VOID)
{
    HI_MDM_LOCK_INFO_STRU stLockInfo = {0};
    HI_U32 ulRet = HI_ERR_SUCCESS;

    HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_035, HI_DIAG_MT("�ж�����ʱ������"));

    // ��ѯ������Ϣ
    ulRet = HI_MDM_GetLockInfo(&stLockInfo);
    if (HI_ERR_SUCCESS != ulRet)
    {
        HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_034, HI_DIAG_MT("mrsNetworkCheckTimeout: ��ѯ������Ϣʧ�� [ret]"), ulRet);
        return ulRet;
    }

    if (HI_MDM_UNLOCK_NETWORK_STATUS == stLockInfo.lock_status)
    {
        return HI_ERR_SUCCESS;
    }

    if (mrsCheckReachLeaveThInLockNet(&stLockInfo))
    {
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_032, HI_DIAG_MT("�ﵽ����ʱ������"));

        // ���ͽ�����Ϣ
        HI_MDM_UnlockNetwork();

        // �ȴ������������������
        MRS_StartTimer(MRS_STA_TIMER_REJOIN_AFTER_UNLOCK, MRS_STA_TIME_REJOIN_AFTER_UNLOCK, HI_SYS_TIMER_ONESHOT);
    }

    return HI_ERR_SUCCESS;
}

/**
* @brief �ж��Ƿ�ﵽ��������ʱ������
*
* @par Description:
*     �жϱ�׼: ����ʱ���ﵽ���ޣ����ң��ڴ�����ʱ���ڣ�����ʱ���ﵽ����
*
*/
HI_BOOL mrsCheckReachLeaveThInLockNet(HI_MDM_LOCK_INFO_STRU * pstLockInfo)
{
    MRS_STA_SRV_CTX_STRU *pstStaCtx = mrsStaGetContext();

    if (!pstLockInfo || !pstStaCtx || HI_MDM_UNLOCK_NETWORK_STATUS == pstLockInfo->lock_status)
    {
        return HI_FALSE;
    }

    HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_036, HI_DIAG_MT("��ǰ����ʱ��ͳ��״̬: [leave_time_len] [usLeaveTimeLenTh]"),
        pstLockInfo->leave_time_len, MRS_MIN_TO_SEC((HI_U32)pstStaCtx->usLeaveTimeLenTh));

    if (!MRS_CHECK_LEAVE_TIME_LEN_TH_VALID(pstStaCtx->usLeaveTimeLenTh))
    {
        return HI_FALSE;
    }

    if (pstLockInfo->leave_time_len >= MRS_MIN_TO_SEC((HI_U32)pstStaCtx->usLeaveTimeLenTh))
    {
        return HI_TRUE;
    }

    return HI_FALSE;
}


/**
* @brief �����ɹ��󣬷�����Ϣ��������
*
* @retval HI_ERR_SUCCESS ����ִ�гɹ�(���ܼ����ڵȴ���Ҳ���ܳɹ���������������Ϣ)��
* @retval HI_ERR_FAILURE ��������������Ϣʧ�ܣ�վ��δ������������������
* @retval HI_ERR_INVALID_PARAMETER ������Ϣ��ѯʱ�����δ������۲�����֣������������BUG��
*/
HI_U32 mrsRejoinNetworkAfterUnlock(HI_VOID)
{
    HI_MDM_LOCK_INFO_STRU stLockInfo = {0};
    HI_U32 ulRet = HI_ERR_SUCCESS;

    // ��ѯ������Ϣ
    ulRet = HI_MDM_GetLockInfo(&stLockInfo);
    if (HI_ERR_SUCCESS != ulRet)
    {
        return HI_ERR_FAILURE;
    }

    // �����ɹ���������
    if (HI_MDM_UNLOCK_NETWORK_STATUS == stLockInfo.lock_status)
    {
        // ������Ϣ��������
        return HI_MDM_RejoinNetwork();
    }

    return HI_ERR_SUCCESS;
}

HI_BOOL mrsCheckTimingFrame(HI_PBYTE pFrame, HI_U16 usFrameLength)
{
    HI_U16 usPos = 0;
    HI_U16 usLength = 0;

    if (HI_ERR_SUCCESS != mrsFind645Frame(pFrame, (HI_S16)usFrameLength, &usPos, &usLength))
    {
        return HI_FALSE;
    }

    if ((pFrame[usPos + MRS_645_FRAME_CTRL_OFFSET] & MRS_645_FRAME_CONTROL_MASK) == MRS_COLLECTOR_BOARDCAST_TIMEING_CONTROL)
    {
        return HI_TRUE;
    }

    return HI_FALSE;
}


HI_U32 mrsStaReportDeviceInf(HI_U8 *pucSrcMac, HI_U8 *pucDstMac)
{
    MRS_PLC_FRAME_DATA_STRU stPlcFrame = {0};
    MRS_GET_DEVICE_INF_UP_STRU *pstUpFrame = HI_NULL;
    MRS_STA_SRV_CTX_STRU *pstStaCtx = mrsStaGetContext();
    MRS_METER_LIST_S *pstMeterList = HI_NULL;
    MRS_METER_ITEM_S *pstMeter = HI_NULL;
    HI_U8 ucMeterNum = 0;
    HI_U8 * pucPayload = HI_NULL;
    HI_U16 usPayloadLen = 0;

    pstMeterList = (MRS_METER_LIST_S *)mrsToolsMalloc(sizeof(MRS_METER_LIST_S));
    if (!pstMeterList)
    {
        return HI_ERR_MALLOC_FAILUE;
    }

   (hi_void)memcpy_s(pstMeterList, sizeof(MRS_METER_LIST_S), &pstStaCtx->stMeterList, sizeof(MRS_METER_LIST_S));

    if (MRS_STA_TYPE_COLLECTOR == pstMeterList->ucType)
    {
        MRS_STA_SEARCH_CTX * pstSmCtx = mrsStaSearchCtx();

        ucMeterNum = pstMeterList->ucMeterNum;
        if (!ucMeterNum)
        {
            ucMeterNum = pstSmCtx->stMeterList.ucMeterNum;
            pstMeterList->ucMeterNum = ucMeterNum;
           (hi_void)memcpy_s(pstMeterList->stMeterItem, sizeof(pstMeterList->stMeterItem),
                pstSmCtx->stMeterList.astMeter, sizeof(pstMeterList->stMeterItem));
        }
    }

    usPayloadLen = sizeof(MRS_GET_DEVICE_INF_UP_STRU) + sizeof(MRS_METER_INF_STRU) * ucMeterNum;
    pucPayload = (HI_U8 *)mrsToolsMalloc(usPayloadLen);
    if (!pucPayload)
    {
        mrsToolsFree(pstMeterList);
        return HI_ERR_NOT_ENOUGH_MEMORY;
    }

    (hi_void)memset_s(pucPayload, usPayloadLen, 0, usPayloadLen);

    pstUpFrame = (MRS_GET_DEVICE_INF_UP_STRU *)pucPayload;
    pstUpFrame->interface_ver = MRS_GET_DEVICE_INF_UP_VER;
    pstUpFrame->stru_len = sizeof(MRS_GET_DEVICE_INF_UP_STRU);
    pstUpFrame->meter_num = ucMeterNum;

    if (pucSrcMac && pucDstMac)
    {
       (hi_void)memcpy_s(pstUpFrame->src, HI_PLC_MAC_ADDR_LEN, pucSrcMac, HI_PLC_MAC_ADDR_LEN);
       (hi_void)memcpy_s(pstUpFrame->dst, HI_PLC_MAC_ADDR_LEN, pucDstMac, HI_PLC_MAC_ADDR_LEN);
    }
    else
    {
       (hi_void)memcpy_s(pstUpFrame->src, HI_PLC_MAC_ADDR_LEN, MRS_BROADCAST_MAC, HI_PLC_MAC_ADDR_LEN);
       (hi_void)memcpy_s(pstUpFrame->dst, HI_PLC_MAC_ADDR_LEN, MRS_BROADCAST_MAC, HI_PLC_MAC_ADDR_LEN);
    }

    if (MRS_STA_TYPE_COLLECTOR == pstMeterList->ucType)
    {
        MRS_METER_INF_STRU *pstDevInf = (MRS_METER_INF_STRU *)pstUpFrame->meter_list;
        HI_U8 ucIndex = 0;
        HI_U8 ucNum = 0;

       (hi_void)memcpy_s(pstUpFrame->stDeviceInf.aucAddr, HI_METER_ADDR_LEN, mrsGetCollectorAddr(), HI_METER_ADDR_LEN);
        HI_DIAG_LOG_MSG_E3(MRS_FILE_LOG_FLAG_033, HI_DIAG_MT("device addr"), pstUpFrame->stDeviceInf.aucAddr[0],pstUpFrame->stDeviceInf.aucAddr[1],pstUpFrame->stDeviceInf.aucAddr[5]);
        if (mrsToolsIsIICollector())
        {
            pstUpFrame->stDeviceInf.ucType = MRS_MAP_DEV_TYPE_CLT_II;
        }
        else
        {
            pstUpFrame->stDeviceInf.ucType = MRS_MAP_DEV_TYPE_CLT_I;
        }

        pstUpFrame->stDeviceInf.ucStatus = mrsStaSearchCtx()->stRunning.ucSearchStatus;

        for (ucIndex = 0; ucIndex < PRODUCT_CFG_MRS_MAX_METER_IN_MAC_NUM; ucIndex++)
        {
            pstMeter = pstMeterList->stMeterItem + ucIndex;

            if (!pstMeter->ucValidFlag)
            {
                continue;
            }

            if (++ucNum > ucMeterNum)
            {
                break;
            }

           (hi_void)memcpy_s(pstDevInf->aucAddr, HI_METER_ADDR_LEN, pstMeter->ucMeterAddr, HI_METER_ADDR_LEN);
            pstDevInf->ucProtocol = pstMeter->ucProtocol;
            pstDevInf->ucType = MRS_STA_TYPE_METER;
            pstDevInf++;
        }
    }
    else
    {
        HI_U8 ucIndex = 0;

        for (ucIndex = 0; ucIndex < PRODUCT_CFG_MRS_MAX_METER_IN_MAC_NUM; ucIndex++)
        {
            pstMeter = pstMeterList->stMeterItem + ucIndex;

            if (pstMeter->ucValidFlag)
            {
               (hi_void)memcpy_s(pstUpFrame->stDeviceInf.aucAddr, HI_METER_ADDR_LEN, pstMeter->ucMeterAddr, HI_METER_ADDR_LEN);
                pstUpFrame->stDeviceInf.ucProtocol = pstMeter->ucProtocol;
                pstUpFrame->stDeviceInf.ucType = MRS_STA_TYPE_METER;
                break;
            }
        }

        if (PRODUCT_CFG_MRS_MAX_METER_IN_MAC_NUM == ucIndex)
        {
            mrsToolsFree(pucPayload);
            mrsToolsFree(pstMeterList);

            return HI_ERR_NOT_FOUND;
        }
    }

    (hi_void)memset_s(&stPlcFrame, sizeof(stPlcFrame), 0, sizeof(stPlcFrame));
    stPlcFrame.usId = ID_MRS_CMD_GET_DEVICE_INF_PLC;
    stPlcFrame.pucPayload = pucPayload;
    stPlcFrame.usPayloadLen = usPayloadLen;

    MRS_SendPlcFrame(&stPlcFrame);

    mrsToolsFree(pucPayload);
    mrsToolsFree(pstMeterList);

    return HI_ERR_SUCCESS;
}


//HSOע��(����-Զ��)
HI_U32 mrsRS485TranData(HI_U16 ulId, HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption)
{
    DIAG_MRS_RS485_TRANSMIT_REQ_STRU * pstReq = (DIAG_MRS_RS485_TRANSMIT_REQ_STRU *)pCmdParam;
    DIAG_MRS_RS485_TRANSMIT_IND_STRU stInd = {0};

    do
    {
        if (!mrsToolsIsIICollector())//��֧��II�ɲ�Ʒ��̬
        {
            stInd.ucErrCode = HI_MRS_ERR_CODE_INVALID_TYPE;
            HI_DIAG_LOG_MSG_E0(__LINE__, HI_DIAG_MT("err type"));
            break;
        }

        if((pstReq->stTransData.ulLen + pstReq->ucStructSize) != usCmdParamSize)
        {
            stInd.ucErrCode = HI_MRS_ERR_CODE_LENGTH;
            break;
        }
    }while(HI_FALSE);

    if (stInd.ucErrCode != HI_MRS_ERR_CODE_SUCCESS)
    {
        stInd.ucVer = MRS_CMD_STRU_VER_R1;
        stInd.ucStructSize = sizeof(DIAG_MRS_RS485_TRANSMIT_IND_STRU);
        HI_MDM_ReportPacket(ulId, (HI_U8)ulOption, (HI_PBYTE)&stInd,stInd.ucStructSize, HI_TRUE);
        return HI_ERR_BAD_DATA;
    }

    if(ulOption == HI_DIAG_CMD_INSTANCE_IREMOTE)
    {
        mrsStaRS485TranData((HI_PBYTE)pCmdParam + pstReq->ucStructSize,(HI_U16)(pstReq->stTransData.ulLen),MRS_STA_QUEUE_FROM_REMOTE_HSO, (HI_U16)pstReq->stComCfg.ulBaudRate);
    }
    else
    {
        mrsStaRS485TranData((HI_PBYTE)pCmdParam + pstReq->ucStructSize ,(HI_U16)(pstReq->stTransData.ulLen),MRS_STA_QUEUE_FROM_LOCAL, (HI_U16)pstReq->stComCfg.ulBaudRate);
    }

    return HI_ERR_CONSUMED;
}


// Return the specified meter information of the local meter list.
// pucMeter - Meter address
MRS_METER_ITEM_S * mrsStaGetLocalMeterInf(HI_U8 *pucMeter)
{
    MRS_STA_SRV_CTX_STRU *pstStaCtx = mrsStaGetContext();
    MRS_METER_ITEM_S *pstMeterList = pstStaCtx->stMeterList.stMeterItem;
    HI_U16 i = 0;

    for (i = 0; i < PRODUCT_CFG_MRS_MAX_METER_IN_MAC_NUM; i++)
    {
        if (!pstMeterList[i].ucValidFlag)
        {
            continue;
        }

        if (mrsToolsMemEq(pstMeterList[i].ucMeterAddr, pucMeter, HI_METER_ADDR_LEN))
        {
            return &pstMeterList[i];
        }
    }

    return HI_NULL;
}


// Return 645 frame head timeout by type.
// ucType : MRS_STA_RM_CFG_LOCAL/MRS_STA_RM_CFG_CLTI/MRS_STA_RM_CFG_AUTO
HI_U16 mrsGet645FrameHeadTimeout(HI_U8 ucType)
{
    MRS_STA_SRV_CTX_STRU *pstStaCtx = mrsStaGetContext();
    HI_U8 ucIndex = ucType;

    if (MRS_STA_RM_CFG_AUTO == ucType)
    {
        ucIndex = ((STA_TYPE_CLTI == mrsStaGetType()) ? MRS_STA_RM_CFG_CLTI : MRS_STA_RM_CFG_LOCAL);
    }

    if (ucIndex >= MRS_STA_RM_CFG_MAX)
    {
        return 0;
    }

    return pstStaCtx->stRmCfg.astRmCfg[ucIndex].usFrameHead;
}

// Return 645 frame interval timeout by type.
// ucType : MRS_STA_RM_CFG_LOCAL/MRS_STA_RM_CFG_CLTI/MRS_STA_RM_CFG_AUTO
HI_U16 mrsGet645FrameInterval(HI_U8 ucType)
{
    MRS_STA_SRV_CTX_STRU *pstStaCtx = mrsStaGetContext();
    HI_U8 ucIndex = ucType;

    if (MRS_STA_RM_CFG_AUTO == ucType)
    {
        ucIndex = ((STA_TYPE_CLTI == mrsStaGetType()) ? MRS_STA_RM_CFG_CLTI : MRS_STA_RM_CFG_LOCAL);
    }

    if (ucIndex >= MRS_STA_RM_CFG_MAX)
    {
        return 0;
    }

    return pstStaCtx->stRmCfg.astRmCfg[ucIndex].usFrameInterval;
}

// Return parallel-reading 645 interval timeout by type.
// ucType : MRS_STA_RM_CFG_LOCAL/MRS_STA_RM_CFG_CLTI/MRS_STA_RM_CFG_AUTO
HI_U16 mrsGet645MeterInterval(HI_U8 ucType)
{
    MRS_STA_SRV_CTX_STRU *pstStaCtx = mrsStaGetContext();
    HI_U8 ucIndex = ucType;

    if (MRS_STA_RM_CFG_AUTO == ucType)
    {
        ucIndex = ((STA_TYPE_CLTI == mrsStaGetType()) ? MRS_STA_RM_CFG_CLTI : MRS_STA_RM_CFG_LOCAL);
    }

    if (ucIndex >= MRS_STA_RM_CFG_MAX)
    {
        return 0;
    }
    return pstStaCtx->stRmCfg.astRmCfg[ucIndex].usMeterInterval;
}

// Return 645 CTRL frame correction by type.
// ucType : MRS_STA_RM_CFG_LOCAL/MRS_STA_RM_CFG_CLTI/MRS_STA_RM_CFG_AUTO
HI_U16 mrsGet645CtrlCmdDelay(HI_U8 ucType)
{
    MRS_STA_SRV_CTX_STRU *pstStaCtx = mrsStaGetContext();
    HI_U8 ucIndex = ucType;

    if (MRS_STA_RM_CFG_AUTO == ucType)
    {
        ucIndex = ((STA_TYPE_CLTI == mrsStaGetType()) ? MRS_STA_RM_CFG_CLTI : MRS_STA_RM_CFG_LOCAL);
    }

    if (ucIndex >= MRS_STA_RM_CFG_MAX)
    {
        return 0;
    }

    return pstStaCtx->stRmCfg.astRmCfg[ucIndex].usCtrlCmdDelay;
}

// Return STA queue max length.
HI_U8 mrsGetStaQueueMax(HI_VOID)
{
    MRS_STA_SRV_CTX_STRU *pstStaCtx = mrsStaGetContext();
    HI_U8 ucIndex = ((STA_TYPE_CLTI == mrsStaGetType()) ? MRS_STA_RM_CFG_CLTI : MRS_STA_RM_CFG_LOCAL);

    return pstStaCtx->stRmCfg.astRmCfg[ucIndex].ucQueueMax;
}

HI_U8 mrsGet645RetryCfg(HI_U8 ucType)
{
    MRS_STA_SRV_CTX_STRU *pstStaCtx = mrsStaGetContext();
    HI_U8 ucIndex = ucType;

    if (MRS_STA_RM_CFG_AUTO == ucType)
    {
        ucIndex = ((STA_TYPE_CLTI == mrsStaGetType()) ? MRS_STA_RM_CFG_CLTI : MRS_STA_RM_CFG_LOCAL);
    }

    if (ucIndex >= MRS_STA_RM_CFG_MAX)
    {
        return 0;
    }

    return pstStaCtx->stRmCfg.astRmCfg[ucIndex].ucRetryCfg;
}

HI_U8 mrsGet645DenyRetryCfg(HI_U8 ucType)
{
    return (HI_U8)((mrsGet645RetryCfg(ucType) & 0x0C) >> 2);
}

HI_U8 mrsGet645TimeoutRetryCfg(HI_U8 ucType)
{
    return (HI_U8)(mrsGet645RetryCfg(ucType) & 0x03);
}

#define MRS_IS_NEED_CHECK_DI(x) \
    ( (MRS_645_FRAME_CONTROL_READ_DATA == (x)) \
        || (MRS_645_FRAME_CONTROL_READ_FOLLOW == (x)) \
        || (MRS_645_FRAME_CONTROL_READ_DATA97 == (x)) \
        || (MRS_645_FRAME_CONTROL_READ_FOLLOW97 == (x)) \
        || (MRS_645_FRAME_CONTROL_SAFE == (x)))

// Compare Dl & Ul frame, check wheher Ul frame match DL frame or not.
HI_BOOL mrsStaIsMrFrameMatch(HI_U8 * pDlFrame, HI_U16 usDlLength, HI_U8 * pUlFrame, HI_U16 usUlLength, HI_U8 ucProtocol)
{
    if (METER_PROTO_698_45 == ucProtocol)
    {
        return mrsStaIsMrFrameMatch698(pDlFrame, usDlLength, pUlFrame, usUlLength);
    }
    else
    {
        return mrsStaIsMrFrameMatch645(pDlFrame, usDlLength, pUlFrame, usUlLength);
    }
}


HI_BOOL mrsStaIsMrFrameMatch645(HI_U8 * pDnFrame, HI_U16 usDnLength, HI_U8 * pUpFrame, HI_U16 usUpLength)
{
    HI_U16 usDnPos = 0, usDnFrameLen = 0;
    HI_U16 usUpPos = 0, usUpFrameLen = 0;
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U8 ucDiSize = MRS_645_FRAME_DATA_DI_SIZE;
    HI_BOOL bIsMeter97 = HI_FALSE;
    MRS_645_CTRL_STRU stDnCtrl;
    MRS_645_CTRL_STRU stUpCtrl;

    ulRet = mrsFind645Frame(pDnFrame, (HI_S16)usDnLength, &usDnPos, &usDnFrameLen);
    if (HI_ERR_SUCCESS != ulRet)
    {
        // Dl frame is NOT 645 frame, don't check. return HI_TRUE
        return HI_TRUE;
    }

    ulRet = mrsFind645Frame(pUpFrame, (HI_S16)usUpLength, &usUpPos, &usUpFrameLen);
    if (HI_ERR_SUCCESS != ulRet)
    {
        // NOT found 645 frame, return HI_FALSE
        return HI_FALSE;
    }

    (hi_void)memset_s(&stDnCtrl, sizeof(stDnCtrl), 0, sizeof(stDnCtrl));
    (hi_void)memset_s(&stUpCtrl, sizeof(stUpCtrl), 0, sizeof(stUpCtrl));

   (hi_void)memcpy_s(&stDnCtrl, sizeof(stDnCtrl), &pDnFrame[usDnPos + MRS_645_FRAME_CTRL_OFFSET], sizeof(stDnCtrl));
   (hi_void)memcpy_s(&stUpCtrl, sizeof(stUpCtrl), &pUpFrame[usUpPos + MRS_645_FRAME_CTRL_OFFSET], sizeof(stUpCtrl));

    bIsMeter97 = (HI_BOOL)(METER_PROTO_645_1997 == mrsToolsGet645Ver(stUpCtrl.ucFn));

    // check ADDR & DI
    if (!mrsMeterAddrMatch(&pDnFrame[usDnPos + MRS_645_FRAME_METERADD_OFFSET], &pUpFrame[usUpPos + MRS_645_FRAME_METERADD_OFFSET])
        && !(bIsMeter97 && mrsToolsBroadCastAddr(&pDnFrame[usDnPos + MRS_645_FRAME_METERADD_OFFSET])))
    {
        return HI_FALSE;
    }

    // Fn = 03H: 645-97, needn't check up frame's Fn & DI.
    if ((MRS_645_FRAME_CONTROL_SAFE == stDnCtrl.ucFn) && (usDnFrameLen < MRS_645_FRAME_LENGTH_MIN + MRS_645_FRAME_DATA_DI_SIZE))
    {
        return HI_TRUE;
    }

    // Fn
    if (stUpCtrl.ucFn != stDnCtrl.ucFn)
    {
        return HI_FALSE;
    }
	// ���ͷ��� 0-����վ����������֡
    if (stUpCtrl.ucDir == 0)
    {
        return HI_FALSE;
    }

    // Deny frame, non 01H/02H/03H/11H/12H
    if (stUpCtrl.ucSlaveFlag || !MRS_IS_NEED_CHECK_DI(stUpCtrl.ucFn))
    {
        return HI_TRUE;
    }

    if (bIsMeter97)
    {
        ucDiSize = MRS_645_FRAME_DATA_DI_SIZE_V97;
    }

    if ((usDnFrameLen >= MRS_645_FRAME_LENGTH_MIN + ucDiSize) && (usUpFrameLen >= MRS_645_FRAME_LENGTH_MIN + ucDiSize))
    {
        return mrsToolsMemEq(pDnFrame + usDnPos + MRS_645_FRAME_DATA_OFFSET,
            pUpFrame + usUpPos + MRS_645_FRAME_DATA_OFFSET, ucDiSize);
    }

    return HI_FALSE;
}


HI_BOOL mrsStaIsMrFrameMatch698(HI_U8 * pDnFrame, HI_U16 usDnLength, HI_U8 * pUpFrame, HI_U16 usUpLength)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U8 aucDnAddr[HI_METER_ADDR_LEN] = {0};
    HI_U8 aucUpAddr[HI_METER_ADDR_LEN] = {0};

    ulRet = mrsFind698MeterAddr(pDnFrame, usDnLength, aucDnAddr);
    if (HI_ERR_SUCCESS != ulRet)
    {
        /* ���б��ķ�698�����Ƚ� */
        return HI_TRUE;
    }

    ulRet = mrsFind698MeterAddr(pUpFrame, usUpLength, aucUpAddr);
    if (HI_ERR_SUCCESS != ulRet)
    {
        /* ���з�645���ģ�ʧ�� */
        return HI_FALSE;
    }

    return mrsMeterAddrMatch(aucDnAddr, aucUpAddr);
}


/**
 @brief     Add meter to local meterlist.  CNcomment: ��ӵ�������б�CNend
 @param[in] pstItem  meter information pointer.  CNcomment: �����Ϣָ�롣CNend
 */
HI_VOID mrsStaAddMeter2LocalList(MRS_METER_ITEM_S *pstItem)
{
    MRS_STA_SRV_CTX_STRU *pstStaCtx = mrsStaGetContext();
    MRS_METER_LIST_S *pstMeterList = &(pstStaCtx->stMeterList);
    MRS_METER_ITEM_S *pstMeter = HI_NULL;
    HI_U8 i = 0;

    if (!pstItem)
    {
        return;
    }

    for (i = 0; i < PRODUCT_CFG_MRS_MAX_METER_IN_MAC_NUM; i++)
    {
        pstMeter = &(pstMeterList->stMeterItem[i]);
        if (pstMeter->ucValidFlag && mrsToolsMemEq(pstMeter->ucMeterAddr, pstItem->ucMeterAddr, HI_METER_ADDR_LEN))
        {
            pstMeter->usBaudRate = pstItem->usBaudRate;
            pstMeter->usOption = MRS_STA_LOCAL_MAP_ENTRY_TTL_MAX;
            if (pstItem->ucProtocol > pstMeter->ucProtocol)
            {
                pstMeter->ucProtocol = pstItem->ucProtocol;
            }
            return;
        }
    }

    for (i = 0; i < PRODUCT_CFG_MRS_MAX_METER_IN_MAC_NUM; i++)
    {
        pstMeter = &(pstMeterList->stMeterItem[i]);
        if (!pstMeter->ucValidFlag)
        {
           (hi_void)memcpy_s(pstMeter, sizeof(MRS_METER_ITEM_S), pstItem, sizeof(MRS_METER_ITEM_S));
            pstMeter->usOption = MRS_STA_LOCAL_MAP_ENTRY_TTL_MAX;
            pstMeterList->ucMeterNum++;
            return;
        }
    }
}


/* BEGIN: PN: DTS2017_698.45 MODIFY\ADD\DEL by cuiate/00233580 at 2017/6/28 */

// ���õ�ǰ�����Э������: 645������
HI_VOID mrsStaSetFrmProto(HI_U8 ucFrmProto)
{
    MRS_STA_SRV_CTX_STRU *pstStaCtx = mrsStaGetContext();

    switch (ucFrmProto)
    {
    case MRS_SRV_PROTO_645:
    case MRS_SRV_PROTO_698_45:
    case MRS_SRV_PROTO_TRANSPARENT:
        pstStaCtx->ucFrmProto = ucFrmProto;
        break;

    default:
        pstStaCtx->ucFrmProto = MRS_SRV_PROTO_645;
        break;
    }

    HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_007, HI_DIAG_MT("**mrsStaSetFrmProto**"), pstStaCtx->ucFrmProto);
}

// ��ȡ��ǰ�����Э������: 645������
HI_U8 mrsStaGetFrmProto(HI_VOID)
{
    return mrsStaGetContext()->ucFrmProto;
}


/* ���ݽ���֡�����ʱ */
HI_U32 mrsStaUartFrameTimeout(HI_VOID)
{
    MRS_645_PROTO_CTX_STRU * pstProtoCtx = mrs645GetContext(ID_MRS_UART_645BUF);
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U8 ucFrmProto = mrsStaGetFrmProto();

    HI_U32 ulMsgId = EN_MRS_FW_MSG_TRANSPARENT_FRAME_INPUT;
    HI_U16 usStartPos = 0;
    HI_U16 usLength = 0;

    if (0 == pstProtoCtx->usRcvBufferSize)
    {
        return HI_ERR_BAD_DATA;
    }

    switch (ucFrmProto)
    {
    case MRS_SRV_PROTO_645:
        ulRet = mrsFind645Frame(pstProtoCtx->pucRcvBuffer, (HI_S16)pstProtoCtx->usRcvBufferSize, &usStartPos, &usLength);
        ulMsgId = EN_MRS_FW_MSG_645_FRAME_INPUT;
        break;

    case MRS_SRV_PROTO_698_45:
        ulRet = mrsFind698Frame(pstProtoCtx->pucRcvBuffer, pstProtoCtx->usRcvBufferSize, &usStartPos, &usLength);
        ulMsgId = EN_MRS_FW_MSG_698_FRAME_INPUT;
        break;

    case MRS_SRV_PROTO_TRANSPARENT:
    default:
        usStartPos = 0;
        usLength = pstProtoCtx->usRcvBufferSize;
        ulMsgId = EN_MRS_FW_MSG_TRANSPARENT_FRAME_INPUT;
        break;
    }

    if (HI_ERR_SUCCESS == ulRet)
    {
        MRS_ONE_RAW_FRAME_STRU *pstFrame = HI_NULL;
        HI_SYS_QUEUE_MSG_S stMsg = {0};

        pstFrame = (MRS_ONE_RAW_FRAME_STRU *)&stMsg.ulParam[0];

        pstFrame->pucDatagram = (HI_PBYTE)mrsToolsMalloc(usLength);
        if (!pstFrame->pucDatagram)
        {
            mrs645ProtoStreamInputCheck(HI_TRUE, ID_MRS_UART_645BUF);
            return HI_ERR_NO_MORE_MEMORY;
        }

        pstFrame->usDatagramSize = usLength;
       (hi_void)memcpy_s(pstFrame->pucDatagram, pstFrame->usDatagramSize, pstProtoCtx->pucRcvBuffer + usStartPos, usLength);

        HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1005, HI_DIAG_MT("[STA-MR]-RX"), pstFrame->pucDatagram, pstFrame->usDatagramSize);
        mrsDfxChlUartOkFrameCnt(pstFrame->usDatagramSize, HI_FALSE);  /* ���ڽ�������֡ͳ��(��Ϣ����) */

        stMsg.ulMsgId = ulMsgId;

        ulRet = mrsSendMessage2Queue(&stMsg);
        if (HI_ERR_SUCCESS != ulRet)
        {
            mrsToolsFree(pstFrame->pucDatagram);

            mrsDfxChlUartErrFrameCnt(HI_FALSE);     /* ���ڽ�������֡����ͳ��(��Ϣ����) */
            HI_MDM_ReportSysEvent(EID_APP_SEND_MSG_FAIL);
        }
    }

    mrs645ProtoStreamInputCheck(HI_TRUE, ID_MRS_UART_645BUF);

    return ulRet;
}

/* END:   PN: DTS2017_698.45 MODIFY\ADD\DEL by cuiate/00233580 at 2017/6/28 */
// ά��: ��ȡվ���±��ͣ����Ϣ
HI_U32 mrsDfxStaGetPoweroffInfo(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption)
{
    MRS_STA_SRV_CTX_STRU * pstStaCtx = mrsStaGetContext();
    DIAG_CMD_MRS_STA_GET_POWEROFF_INFO_IND_STRU stInd = {0};

    HI_UNREF_PARAM(pCmdParam);
    HI_UNREF_PARAM(usCmdParamSize);

    (hi_void)memset_s(&stInd, sizeof(DIAG_CMD_MRS_STA_GET_POWEROFF_INFO_IND_STRU), 0,sizeof(DIAG_CMD_MRS_STA_GET_POWEROFF_INFO_IND_STRU));

    if (!pstStaCtx)
    {
        stInd.ulRet = HI_ERR_FAILURE;
    }
    else
    {
        stInd.ulRet = HI_ERR_SUCCESS;

        stInd.ulLastPoweroffTimeSec     = pstStaCtx->stPowerInfo.ulMeterPowerOffTime;
        stInd.ulLastPoweronTimeSec      = pstStaCtx->stPowerInfo.ulMeterPowerOnTime;
        HI_MDM_MakeSecondToCalendar(pstStaCtx->stPowerInfo.ulMeterPowerOffTime,  &stInd.stLastPoweroffTime);
        HI_MDM_MakeSecondToCalendar(pstStaCtx->stPowerInfo.ulMeterPowerOnTime,   &stInd.stLastPoweronTime);

        stInd.usRcvRecheckCmdCnt        = pstStaCtx->stPowerInfo.usRcvRecheckCmdCnt;
        stInd.usRcvRecheckCmdForMeCnt   = pstStaCtx->stPowerInfo.usRcvRecheckCmdForMeCnt;
        stInd.usRcvDetectCmdCnt         = pstStaCtx->stPowerInfo.usRcvDetectCmdCnt;
        stInd.usRcvDetectCmdForMeCnt    = pstStaCtx->stPowerInfo.usRcvDetectCmdForMeCnt;
        stInd.ucSendCmdFailCnt          = pstStaCtx->stPowerInfo.ucSendCmdFailCnt;
        stInd.ulSendCmdErrCode          = pstStaCtx->stPowerInfo.ulSendCmdErrCode;
        stInd.ulLastReadPowerInfoTime   = pstStaCtx->stPowerInfo.ulLastReadPowerInfoTime;
        stInd.ulStopWhere               = pstStaCtx->stPowerInfo.ulStopWhere;
    }

    HI_MDM_ReportPacket(usId, (HI_U8)ulOption, (HI_PBYTE)&stInd,
        sizeof(DIAG_CMD_MRS_STA_GET_POWEROFF_INFO_IND_STRU), HI_FALSE);

    return HI_ERR_SUCCESS;
}

#if defined(PRODUCT_CFG_SUPPORT_MRS_DUOL_MODE_RF_II)
HI_U32 mrsCmdStaSetTimeSlot(HI_IN HI_U16 usId, HI_PVOID pReqPacket, HI_U16 usReqPacketSize)
{
	HI_U32 ulRet = HI_ERR_SUCCESS;
    MRS_PLC_FRAME_DATA_STRU *pstPlcFrame = (MRS_PLC_FRAME_DATA_STRU *)pReqPacket;
    MRS_PLC_SET_TIME_SLOT_S *pstTimeSlot = HI_NULL;

    HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_018, HI_DIAG_MT("[RF]SetTimeSlot"));

    if (HI_NULL == pstPlcFrame)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    if (HI_NULL == pstPlcFrame->pucPayload)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    pstTimeSlot = (MRS_PLC_SET_TIME_SLOT_S *)pstPlcFrame->pucPayload;

    HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_065, HI_DIAG_MT("[RF]TimeSlot,MaxTei,data."), pstTimeSlot->usTimeSlot, pstTimeSlot->usMaxTei);
	mrsRfSetMaxTei(pstTimeSlot->usMaxTei);
	mrsRfSetTimeslot(pstTimeSlot->usTimeSlot);
    return ulRet;
}
#endif
/*
HI_U32 mrsCmdStaStartFindOwnerShip(HI_IN HI_U16 usId, HI_PVOID pReqPacket, HI_U16 usReqPacketSize)
{
	HI_U32 ulRet = HI_ERR_SUCCESS;
    MRS_PLC_FRAME_DATA_STRU *pstPlcFrame = (MRS_PLC_FRAME_DATA_STRU *)pReqPacket;
    MRS_PLC_RF_START_FIND_OWNER_S *pstStartFind = HI_NULL;
	HI_U8 Payload[16];
	HI_U8 ucOffent=0;
	HI_U16 usTei=mrsGetTei();
	static HI_U8 s_ucBrocastSn=0;

    HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_066, HI_DIAG_MT("[RF]StartFindOwnerShip"));

    if (HI_NULL == pstPlcFrame)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    if (HI_NULL == pstPlcFrame->pucPayload)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    pstStartFind = (MRS_PLC_RF_START_FIND_OWNER_S *)pstPlcFrame->pucPayload;

    HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_067, HI_DIAG_MT("[RF]usMeterCount,MaxTei,data."), pstStartFind->usMeterCount, pstStartFind->usMaxTei);
	
	Payload[ucOffent++]=s_ucBrocastSn;
	Payload[ucOffent++]=(pstStartFind->usMaxTei &0xFF);
	Payload[ucOffent++]=((pstStartFind->usMaxTei>>8) &0xFF);
	Payload[ucOffent++]=(usTei&0xFF);
	Payload[ucOffent++]=(usTei>>8 &0xFF);
	Payload[ucOffent++]=(pstStartFind->usMeterCount&0xFF);
	Payload[ucOffent++]=((pstStartFind->usMeterCount>>8)&0xFF);
	
	s_ucBrocastSn++;
	mrsRFDau0x42EncodeFrame(Payload,ucOffent);
    return ulRet;
}
*/
/*
HI_U32 mrsCmdStaGetAddressInfo(HI_IN HI_U16 usId, HI_PVOID pReqPacket, HI_U16 usReqPacketSize)
{
	HI_U32 i=0;
    MRS_PLC_FRAME_DATA_STRU * plc = (MRS_PLC_FRAME_DATA_STRU *)pReqPacket;
    MRS_PLC_RF_GET_ADDRESS_INFO_DOWN_S * frame = HI_NULL;
    HI_U8 *pucSrcMac = HI_NULL, *pucDstMac = HI_NULL;
	MRS_RF_ALL_WATER_METER_INFO *pWaterMeterInfo =mrsRfGetAllWaterMeterInfo();

    if (!pReqPacket)
    {
        return HI_ERR_FAILURE;
    }

    frame = (MRS_PLC_RF_GET_ADDRESS_INFO_DOWN_S *)plc->pucPayload;
    if (frame == HI_NULL)
    {
        return HI_ERR_FAILURE;
    }

    // �ж��Ƿ��Ƿ�����վ��ı���
    if (frame->stru_len >= sizeof(MRS_PLC_RF_GET_ADDRESS_INFO_DOWN_S))
    {
        pucSrcMac = frame->ucDstMeter;
        pucDstMac = frame->ucSrcMeter;
        if (!mrsSrvVerifyCcoMac(frame->ucSrcMeter))
        {
        	HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_068, HI_DIAG_MT("[RF]CCO not conform"));
            return HI_ERR_FAILURE;
        }

        if (!mrsSrvVerifyMac(frame->ucDstMeter))
        {
        	HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_069, HI_DIAG_MT("[RF]STA not conform"));
            return HI_ERR_FAILURE;
        }
    }
    
    MRS_PLC_FRAME_DATA_STRU plc_frame = {0};
    MRS_PLC_RF_GET_ADDRESS_INFO_UP_S *pReportInfo =HI_NULL;
	HI_U16 usDataLen=sizeof(MRS_PLC_RF_GET_ADDRESS_INFO_UP_S)+pWaterMeterInfo->ucWaterMeterCount*8;
	pReportInfo = (MRS_PLC_RF_GET_ADDRESS_INFO_UP_S *)mrsToolsMalloc(usDataLen);
	if(pReportInfo==HI_NULL)
	{
		return HI_ERR_MALLOC_FAILUE;
	}
    mrsToolsMemZero_s(pReportInfo, usDataLen, usDataLen);
    pReportInfo->interface_ver = 1;
    pReportInfo->stru_len = sizeof(MRS_PLC_RF_GET_ADDRESS_INFO_UP_S);
    pReportInfo->ucMeterCount = pWaterMeterInfo->ucWaterMeterCount;
    pReportInfo->seq = frame->seq;

    if (pucSrcMac && pucDstMac)
    {
        mrsToolsMemcpy_s(pReportInfo->ucSrcMeter, HI_PLC_MAC_ADDR_LEN, pucSrcMac, HI_PLC_MAC_ADDR_LEN);
        mrsToolsMemcpy_s(pReportInfo->ucDstMeter, HI_PLC_MAC_ADDR_LEN, pucDstMac, HI_PLC_MAC_ADDR_LEN);
    }
    else
    {
        mrsToolsMemcpy_s(pReportInfo->ucSrcMeter, HI_PLC_MAC_ADDR_LEN, MRS_BROADCAST_MAC, HI_PLC_MAC_ADDR_LEN);
        mrsToolsMemcpy_s(pReportInfo->ucDstMeter, HI_PLC_MAC_ADDR_LEN, MRS_BROADCAST_MAC, HI_PLC_MAC_ADDR_LEN);
    }
	
	for(i=0;i<pWaterMeterInfo->ucWaterMeterCount;i++)
	{
		mrsToolsMemcpy_s(pReportInfo->ucData+i*8,7,pWaterMeterInfo->saWaterMeterInfo[i].ucaWaterMeterAddr,7);
		pReportInfo->ucData[i*8+7]=pWaterMeterInfo->saWaterMeterInfo[i].ucFieldValue;
	}
	
    mrsToolsMemZero_s(&plc_frame, sizeof(plc_frame), sizeof(plc_frame));
    plc_frame.usId = ID_MRS_CMD_RF_GET_ADDRESS_INFO;
    plc_frame.pucPayload = (HI_U8 *)pReportInfo;
    plc_frame.usPayloadLen = usDataLen;

    MRS_SendPlcFrame(&plc_frame);

	if (!pReportInfo)
    {
        mrsToolsFree(pReportInfo);
    }
	return HI_ERR_SUCCESS;
		
}

HI_U32 mrsCmdStaSetMeterOwnerShip(HI_IN HI_U16 usId, HI_PVOID pReqPacket, HI_U16 usReqPacketSize)
{
	HI_U32 ulRet = HI_ERR_SUCCESS;
    MRS_PLC_FRAME_DATA_STRU *pstPlcFrame = (MRS_PLC_FRAME_DATA_STRU *)pReqPacket;
    MRS_PLC_RF_SET_METER_OWNERSHIP_DOWN_S *pstSetOwnerShip = HI_NULL;
	MRS_RF_PLC_RUN_INFO_STRU *pRunPlcInfo = mrsGetRfPlcRunInfo();

    HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_070, HI_DIAG_MT("[RF]SetMeterOwnerShip"));

    if (HI_NULL == pstPlcFrame)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    if (HI_NULL == pstPlcFrame->pucPayload)
    {
        return HI_ERR_INVALID_PARAMETER;
    }
	
    pstSetOwnerShip = (MRS_PLC_RF_SET_METER_OWNERSHIP_DOWN_S *)pstPlcFrame->pucPayload;

    HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_071, HI_DIAG_MT("[RF]usMeterCount"), pstSetOwnerShip->ucMeterCount);
	pRunPlcInfo->usPlcSn = pstSetOwnerShip->seq;
	mrsToolsMemcpy_s(pRunPlcInfo->ucSrcMeter, HI_PLC_MAC_ADDR_LEN, pstSetOwnerShip->ucDstMeter, HI_PLC_MAC_ADDR_LEN);
	mrsToolsMemcpy_s(pRunPlcInfo->ucDstMeter, HI_PLC_MAC_ADDR_LEN, pstSetOwnerShip->ucSrcMeter, HI_PLC_MAC_ADDR_LEN);

	// ����������ϵ
	mrsRfSetMeterOwnerShip(pstSetOwnerShip->ucMeterCount,pstSetOwnerShip->ucData,pstSetOwnerShip->ucRtcTime);
	
    return ulRet;
}
*/

/*
HI_U32 mrsCmdStaGetMeterDataInfo(HI_IN HI_U16 usId, HI_PVOID pReqPacket, HI_U16 usReqPacketSize)
{
	HI_U32 i=0,nCountTemp=0,ulMeterDataLen=0;
    MRS_PLC_FRAME_DATA_STRU * plc = (MRS_PLC_FRAME_DATA_STRU *)pReqPacket;
    MRS_PLC_RF_GET_METER_INFO_DOWN_S * frame = HI_NULL;
    HI_U8 *pucSrcMac = HI_NULL, *pucDstMac = HI_NULL;
	MRS_RF_ALL_WATER_METER_INFO *pWaterMeterInfo =mrsRfGetAllWaterMeterInfo();

    if (!pReqPacket)
    {
        return HI_ERR_FAILURE;
    }

    frame = (MRS_PLC_RF_GET_METER_INFO_DOWN_S *)plc->pucPayload;
    if (frame == HI_NULL)
    {
        return HI_ERR_FAILURE;
    }
	pucSrcMac = frame->ucDstMeter;
    pucDstMac = frame->ucSrcMeter;

    // �ж��Ƿ��Ƿ�����վ��ı���
    if (frame->stru_len >= sizeof(MRS_PLC_RF_GET_METER_INFO_DOWN_S))
    {
        if (!mrsSrvVerifyCcoMac(frame->ucSrcMeter))
        {
        	HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_072, HI_DIAG_MT("[RF]CCO not conform"));
            return HI_ERR_FAILURE;
        }

        if (!mrsSrvVerifyMac(frame->ucDstMeter))
        {
        	HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_073, HI_DIAG_MT("[RF]STA not conform"));
            return HI_ERR_FAILURE;
        }
    }
    
    MRS_PLC_FRAME_DATA_STRU plc_frame = {0};
    MRS_PLC_RF_GET_METER_INFO_UP_S *pReportInfo =HI_NULL;
	HI_U16 usDataLen=sizeof(MRS_PLC_RF_GET_METER_INFO_UP_S);
	
	MRS_SRV_LINK * it;
    MRS_SRV_LIST * list;
	MRS_RF_REPORT_DATA_NODE *node=HI_NULL;

    list = &(pWaterMeterInfo->pStaReportDataList);
	
	// ��������
    for (it = list->next; it != list; it = it->next)
    {
        node = (MRS_RF_REPORT_DATA_NODE *)it;
		if(1== (node->ucReportDataState))
		{
			pWaterMeterInfo->ucReportDataCount--;
			mrsSrvListRemove(list,(MRS_SRV_LINK *)node);
			mrsToolsFree(node);
			HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_074, HI_DIAG_MT("[RF]Del one ucaStaReportData,Count"),pWaterMeterInfo->ucReportDataCount);			
			continue;
		}
		else
		{
			ulMeterDataLen+=node->ucReportDataLen;
			nCountTemp++;
		}
    }
	usDataLen +=ulMeterDataLen;
	
	HI_DIAG_LOG_MSG_E3(MRS_FILE_LOG_FLAG_075, HI_DIAG_MT("[RF]usDataLen,ulMeterDataLen,nCountTemp"),usDataLen,ulMeterDataLen,nCountTemp); 		
	pReportInfo = (MRS_PLC_RF_GET_METER_INFO_UP_S *)mrsToolsMalloc(usDataLen);
	if(pReportInfo==HI_NULL)
	{
		return HI_ERR_MALLOC_FAILUE;
	}
    mrsToolsMemZero_s(pReportInfo, usDataLen, usDataLen);
    pReportInfo->interface_ver = 1;
    pReportInfo->stru_len = sizeof(MRS_PLC_RF_GET_METER_INFO_UP_S);
    pReportInfo->ucMeterInfoCount = nCountTemp;
    pReportInfo->seq = frame->seq;

    if (pucSrcMac && pucDstMac)
    {
        mrsToolsMemcpy_s(pReportInfo->ucSrcMeter, HI_PLC_MAC_ADDR_LEN, pucSrcMac, HI_PLC_MAC_ADDR_LEN);
        mrsToolsMemcpy_s(pReportInfo->ucDstMeter, HI_PLC_MAC_ADDR_LEN, pucDstMac, HI_PLC_MAC_ADDR_LEN);
    }
    else
    {
        mrsToolsMemcpy_s(pReportInfo->ucSrcMeter, HI_PLC_MAC_ADDR_LEN, MRS_BROADCAST_MAC, HI_PLC_MAC_ADDR_LEN);
        mrsToolsMemcpy_s(pReportInfo->ucDstMeter, HI_PLC_MAC_ADDR_LEN, MRS_BROADCAST_MAC, HI_PLC_MAC_ADDR_LEN);
    }
	// ��������
	nCountTemp=0;
    for (it = list->next; it != list; it = it->next)
    {
        node = (MRS_RF_REPORT_DATA_NODE *)it;
		if(0== (node->ucReportDataState))
		{
			mrsToolsMemcpy_s(pReportInfo->ucData+nCountTemp,ulMeterDataLen-nCountTemp,node->ucPStaReportData,node->ucReportDataLen);
			nCountTemp +=node->ucReportDataLen;
			node->ucReportDataState=1;
			
			HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_076, HI_DIAG_MT("[RF]nCountTemp,ucReportDataLen"),nCountTemp,node->ucReportDataLen); 		
		}
    }
	
    mrsToolsMemZero_s(&plc_frame, sizeof(plc_frame), sizeof(plc_frame));
    plc_frame.usId = ID_MRS_CMD_RF_GET_METER_INFO;
    plc_frame.pucPayload = (HI_U8 *)pReportInfo;
    plc_frame.usPayloadLen = usDataLen;

    MRS_SendPlcFrame(&plc_frame);

	if (!pReportInfo)
    {
        mrsToolsFree(pReportInfo);
    }
	return HI_ERR_SUCCESS;
}
*/
#if defined(PRODUCT_CFG_SUPPORT_MRS_DUOL_MODE_RF)
HI_U32 mrsStaHandleWaterCmd(MRS_PLC_METERLIST_DL_S * pstFrame,HI_U16 nDataLen)
{
	//HI_U32 ulRet = HI_ERR_SUCCESS;
	HI_U8 Payload[16];
	HI_U8 ucOffent=0;
	HI_U16 usTei=mrsGetTei();
	HI_U32 i=0;
	HI_U8 *pucSrcMac = HI_NULL, *pucDstMac = HI_NULL;
	MRS_PLC_METERLIST_UP_S * report = HI_NULL;
	MRS_RF_ALL_WATER_METER_INFO *pWaterMeterInfo =mrsRfGetAllWaterMeterInfo();
	HI_U8 ucIPAddr[HI_IPV4_ADDR_LEN] = {0};
	MRS_PLC_FRAME_DATA_STRU plc = {0};
	HI_U16 usStartSn=0;
	HI_U8 ucCount=0;
	HI_U16 payload_len=0;
	HI_U8 *payload = HI_NULL;
	
	if(nDataLen<1)
	{
		return HI_ERR_INVALID_PARAMETER;
	}
	HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_081, HI_DIAG_MT("[RF]Water Meter Cmd"),pstFrame->waterMeter_Data[0]);
//	1 �㲥����ˮ��������
	if(pstFrame->waterMeter_Data[0]==1)
	{
    	HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_066, HI_DIAG_MT("[RF]StartFindOwnerShip"));
	    if (nDataLen<6)
	    {
	        return HI_ERR_INVALID_PARAMETER;
	    }

		//�㲥���DAU   1byte 2byte 2byte 2byte
		Payload[ucOffent++]=pstFrame->waterMeter_Data[1];
		
		//ת���㲥��ģDAU
		Payload[ucOffent++]=pstFrame->waterMeter_Data[2];
		Payload[ucOffent++]=pstFrame->waterMeter_Data[3];
    HI_U16 dauNum = ((pstFrame->waterMeter_Data[3]) << 8) | (pstFrame->waterMeter_Data[2]);

		//��ǰʱ϶
		Payload[ucOffent++]=(usTei&0xFF);
		Payload[ucOffent++]=(usTei>>8 &0xFF);
		
		//�͹��ı��ת����ģ
		Payload[ucOffent++]=pstFrame->waterMeter_Data[4];
		Payload[ucOffent++]=pstFrame->waterMeter_Data[5];
    HI_U16 lwpNum = ((pstFrame->waterMeter_Data[5]) << 8) | (pstFrame->waterMeter_Data[4]);

    HI_U16 usSend0x25Timer = 25*dauNum + 3*lwpNum + 100;
		mrsRFDau0x42EncodeFrame(Payload,ucOffent,usSend0x25Timer);
	}
	else if(pstFrame->waterMeter_Data[0]==2)//	2 ˮ����ǿ�ռ�����
	{
		// �ж��Ƿ��Ƿ�����վ��ı���
		if (nDataLen<4)
		{
			return HI_ERR_INVALID_PARAMETER;
		}
		pucSrcMac = pstFrame->dst;
		pucDstMac = pstFrame->src;
		if (!mrsSrvVerifyCcoMac(pstFrame->src))
		{
			HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_068, HI_DIAG_MT("[RF]CCO not conform"));
		    return HI_ERR_FAILURE;
		}

		if (!mrsSrvVerifyMac(pstFrame->dst))
		{
			HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_069, HI_DIAG_MT("[RF]STA not conform"));
		    return HI_ERR_FAILURE;
		}
		usStartSn = pstFrame->waterMeter_Data[1]|(pstFrame->waterMeter_Data[2]<<8);
		ucCount = pstFrame->waterMeter_Data[3];
		if(ucCount >(pWaterMeterInfo->ucWaterMeterCount-usStartSn))
		{
			ucCount=pWaterMeterInfo->ucWaterMeterCount-usStartSn;
		}
		if(pWaterMeterInfo->ucWaterMeterCount<usStartSn)
		{
			ucCount=0;
		}
		payload_len = sizeof(MRS_PLC_METERLIST_UP_S);
		payload_len += ucCount*8+4;

		payload = mrsToolsMalloc(payload_len);
		if (!payload)
		{
		   return HI_ERR_MALLOC_FAILUE;
		}

		mrsToolsMemZero_s(payload, payload_len, payload_len);

		report = (MRS_PLC_METERLIST_UP_S *)payload;
		report->interface_ver = MRS_GET_METERLIST_UP_VER;
		report->stru_len = sizeof(MRS_PLC_METERLIST_UP_S);
		report->option = MRS_SM_WATER_CMD;
		report->status = pstFrame->force_resp;
		report->meter_num = pWaterMeterInfo->ucWaterMeterCount;
		report->type = 0;
		report->seq = pstFrame->seq;

		HI_MDM_NM_GetAddr(report->asset, ucIPAddr);
		mrsHexInvert(report->asset, sizeof(report->asset));

		HI_MDM_GetLocalMacAddr(report->id);
		mrsHexInvert(report->id, sizeof(report->id));

		if (pucSrcMac && pucDstMac)
		{
		   mrsToolsMemcpy_s(report->src, HI_PLC_MAC_ADDR_LEN, pucSrcMac, HI_PLC_MAC_ADDR_LEN);
		   mrsToolsMemcpy_s(report->dst, HI_PLC_MAC_ADDR_LEN, pucDstMac, HI_PLC_MAC_ADDR_LEN);
		}
		else
		{
		   mrsToolsMemcpy_s(report->src, HI_PLC_MAC_ADDR_LEN, MRS_BROADCAST_MAC, HI_PLC_MAC_ADDR_LEN);
		   mrsToolsMemcpy_s(report->dst, HI_PLC_MAC_ADDR_LEN, MRS_BROADCAST_MAC, HI_PLC_MAC_ADDR_LEN);
		}

		report->meter_list[0]=2;
		report->meter_list[1]=pWaterMeterInfo->ucWaterMeterCount&0xFF;
		report->meter_list[2]=(pWaterMeterInfo->ucWaterMeterCount>>8)&0xFF;
		report->meter_list[3]=ucCount;
		for(i=0;i<ucCount;i++)
		{
			mrsToolsMemcpy_s(report->meter_list+4+i*8,7,pWaterMeterInfo->saWaterMeterInfo[i+usStartSn].ucaWaterMeterAddr,7);
			report->meter_list[4+i*8+7]=pWaterMeterInfo->saWaterMeterInfo[i+usStartSn].ucFieldValue;
		}
		mrsToolsMemZero_s(&plc, sizeof(plc), sizeof(plc));
		plc.usId = ID_MRS_CMD_GET_METERLIST;
		plc.pucPayload = payload;
		plc.usPayloadLen = payload_len;

		MRS_SendPlcFrame(&plc);

		mrsToolsFree(payload);
	
	}
	else if(pstFrame->waterMeter_Data[0]==3)//	3 �ռ�ˮ����Ϣ����
	{
		if (nDataLen<4)
		{
			return HI_ERR_INVALID_PARAMETER;
		}
		pucSrcMac = pstFrame->dst;
    	pucDstMac = pstFrame->src;
		// �ж��Ƿ��Ƿ�����վ��ı���
	    if (!mrsSrvVerifyCcoMac(pstFrame->src))
	    {
	    	HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_072, HI_DIAG_MT("[RF]CCO not conform"));
	        return HI_ERR_FAILURE;
	    }

	    if (!mrsSrvVerifyMac(pstFrame->dst))
	    {
	    	HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_073, HI_DIAG_MT("[RF]STA not conform"));
	        return HI_ERR_FAILURE;
	    }
		usStartSn = pstFrame->waterMeter_Data[1]|(pstFrame->waterMeter_Data[2]<<8);
		ucCount = pstFrame->waterMeter_Data[3];
		
		MRS_SRV_LINK * it;
	    MRS_SRV_LIST * list;
		MRS_RF_REPORT_DATA_NODE *node=HI_NULL;

	    list = &(pWaterMeterInfo->pStaReportDataList);
		HI_U16 ulMeterDataLen=0;
		HI_U16 nAllCount=0;
		HI_U16 uReportCount=0;
		HI_U32 ulNowTime=HI_MDM_GetSeconds();
		// ��������
	    for (it = list->next; it != list; it = it->next)
	    {
	        node = (MRS_RF_REPORT_DATA_NODE *)it;
			if((1== (node->ucReportDataState))
				||((ulNowTime - node->ulStartTime)>259200))
			{
				pWaterMeterInfo->ucReportDataCount--;
				mrsSrvListRemove(list,(MRS_SRV_LINK *)node);
				mrsToolsFree(node);
				HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_074, HI_DIAG_MT("[RF]Del one ucaStaReportData,Count"),pWaterMeterInfo->ucReportDataCount);			
				continue;
			}
			else
			{
				nAllCount++;
				if(nAllCount<ucCount)
				{
					uReportCount++;
					ulMeterDataLen+=node->ucReportDataLen+6+2;
				}
				
			}
	    }
	
		payload_len = sizeof(MRS_PLC_METERLIST_UP_S)+ulMeterDataLen+4;

	HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_077, HI_DIAG_MT("[RF]payload_len,ulMeterDataLen"),payload_len,ulMeterDataLen); 		
		payload = mrsToolsMalloc(payload_len);
		if (!payload)
		{
		   return HI_ERR_MALLOC_FAILUE;
		}
		
		mrsToolsMemZero_s(payload, payload_len, payload_len);
		report = (MRS_PLC_METERLIST_UP_S *)payload;
		report->interface_ver = MRS_GET_METERLIST_UP_VER;
		report->stru_len = sizeof(MRS_PLC_METERLIST_UP_S);
		report->option = MRS_SM_WATER_CMD;
		report->status = pstFrame->force_resp;
		report->meter_num = pWaterMeterInfo->ucWaterMeterCount;
		report->type = 0;
		report->seq = pstFrame->seq;

		HI_MDM_NM_GetAddr(report->asset, ucIPAddr);
		mrsHexInvert(report->asset, sizeof(report->asset));

		HI_MDM_GetLocalMacAddr(report->id);
		mrsHexInvert(report->id, sizeof(report->id));

		if (pucSrcMac && pucDstMac)
		{
		   mrsToolsMemcpy_s(report->src, HI_PLC_MAC_ADDR_LEN, pucSrcMac, HI_PLC_MAC_ADDR_LEN);
		   mrsToolsMemcpy_s(report->dst, HI_PLC_MAC_ADDR_LEN, pucDstMac, HI_PLC_MAC_ADDR_LEN);
		}
		else
		{
		   mrsToolsMemcpy_s(report->src, HI_PLC_MAC_ADDR_LEN, MRS_BROADCAST_MAC, HI_PLC_MAC_ADDR_LEN);
		   mrsToolsMemcpy_s(report->dst, HI_PLC_MAC_ADDR_LEN, MRS_BROADCAST_MAC, HI_PLC_MAC_ADDR_LEN);
		}
		HI_U16 nCountTemp=0,usOffet=0;
		report->meter_list[usOffet++]=3;
		report->meter_list[usOffet++]=nAllCount&0xFF;
		report->meter_list[usOffet++]=(nAllCount>>8)&0xFF;
		report->meter_list[usOffet++]=uReportCount;
		// ��������
	    for (it = list->next; it != list; it = it->next)
	    {
	        node = (MRS_RF_REPORT_DATA_NODE *)it;
			if(0== (node->ucReportDataState))
			{
				nCountTemp++;
				// ˮ���ַ
				mrsToolsMemcpy_s(report->meter_list+usOffet,ulMeterDataLen-usOffet,node->ucRecvWaterMeterAddr,6);
				usOffet +=6;
				report->meter_list[usOffet++]=node->ucReportDataLen&0xFF;
				report->meter_list[usOffet++]=(node->ucReportDataLen>>8)&0xFF;
				mrsToolsMemcpy_s(report->meter_list+usOffet,node->ucReportDataLen,node->ucPStaReportData,node->ucReportDataLen);
				usOffet +=node->ucReportDataLen;
				//node->ucReportDataState=1;  �ϱ��ɹ�Ҳ��ɾ��
				
				HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_076, HI_DIAG_MT("[RF]usOffet,ucReportDataLen"),usOffet,node->ucReportDataLen); 		
				if(nCountTemp>=uReportCount)
				{
					break;
				}
			}
	    }
	
	    mrsToolsMemZero_s(&plc, sizeof(plc), sizeof(plc));
	    plc.usId = ID_MRS_CMD_GET_METERLIST;
	    plc.pucPayload = payload;
	    plc.usPayloadLen = payload_len;

	    MRS_SendPlcFrame(&plc);

		if (!payload)
	    {
	        mrsToolsFree(payload);
	    }
	}
	else if(pstFrame->waterMeter_Data[0]==4)//4/��ѯ�豸����//Matteo
	{
		payload_len = sizeof(MRS_PLC_METERLIST_UP_S) +2;
	    payload = mrsToolsMalloc(payload_len);
	    if (!payload)
	    {
	        return HI_ERR_MALLOC_FAILUE;
	    }

	    (hi_void)memset_s(payload, payload_len, 0, payload_len);

	    report = (MRS_PLC_METERLIST_UP_S *)payload;
	    report->interface_ver = MRS_GET_METERLIST_UP_VER;
	    report->stru_len = sizeof(MRS_PLC_METERLIST_UP_S);
	    report->option = MRS_SM_WATER_CMD;
	    report->status = pstFrame->force_resp;
	    report->meter_num = pWaterMeterInfo->ucWaterMeterCount;
		if (mrsToolsIsIICollector())
    	{
			HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_097, HI_DIAG_MT("[Matteo] get find cmd"));
	        report->type = EN_MRS_STA_TYPE_CLT_II;
			report->meter_list[0] = 4;
			report->meter_list[1] = 2;
			report->seq = pstFrame->seq;
			
			HI_NV_FTM_COLLECT_PARAM_FILE_S nv;
			(hi_void)memset_s(&nv, sizeof(nv), 0, sizeof(nv));
			if (HI_ERR_SUCCESS == HI_MDM_NV_Read(HI_NV_FTM_COLLECT_PARAM_FILE, &nv, sizeof(nv)))
			{
			   (hi_void)memcpy_s(report->asset, HI_METER_ADDR_LEN, nv.CLTAddress, HI_METER_ADDR_LEN);
			}
			else
			{
				(hi_void)memset_s(report->asset, HI_METER_ADDR_LEN, 0xBB, HI_METER_ADDR_LEN);
			}
		
			HI_MDM_GetLocalMacAddr(report->id);
			mrsHexInvert(report->id, HI_PLC_MAC_ADDR_LEN);
		
		   (hi_void)memcpy_s(report->src, HI_PLC_MAC_ADDR_LEN, pstFrame->dst, HI_PLC_MAC_ADDR_LEN);
		   (hi_void)memcpy_s(report->dst, HI_PLC_MAC_ADDR_LEN, pstFrame->src, HI_PLC_MAC_ADDR_LEN);
		  
		
			(hi_void)memset_s(&plc, sizeof(plc), 0, sizeof(plc));
			plc.usId = ID_MRS_CMD_GET_METERLIST;
			plc.pucPayload = payload;
			plc.usPayloadLen = payload_len;
		
			MRS_SendPlcFrame(&plc);

			mrsToolsFree(payload);
    	}

		
	}
	else
	{
		return HI_ERR_INVALID_PARAMETER;
	}
		
	return HI_ERR_SUCCESS;
}

#endif


//*****************************************************************************
// ��������: mrsCmdCcoGetDevId
// ��������: ���豸��ȡ����ID��Ϣ(оƬid���豸id)�����豸���յ�PLC֡�Ĵ���
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
HI_U32 mrsCmdStaPlcGetDevId(HI_IN HI_U16 id, HI_PVOID req_packet, HI_U16 packet_size)
{
	hi_u32 ret = HI_ERR_SUCCESS;
	MRS_PLC_FRAME_DATA_STRU *plc_frame = (MRS_PLC_FRAME_DATA_STRU *)req_packet;
	mrs_plc_dev_id_info_dl * plc_dl_info = HI_NULL;
	mrs_plc_dev_id_info_up plc_up_info;
	MRS_PLC_FRAME_DATA_STRU plc_ind_frame;

    HI_UNREF_PARAM(id);
    HI_UNREF_PARAM(packet_size);

    if ((plc_frame == HI_NULL) || (plc_frame->pucPayload == HI_NULL) ||
        (plc_frame->usPayloadLen != sizeof(mrs_plc_dev_id_info_dl))) {
        return HI_ERR_FAILURE;
    }

    plc_dl_info = (mrs_plc_dev_id_info_dl*)plc_frame->pucPayload;

	/* ��ȡ��ID���Ͳ�����,���ش���֡ */
	if (plc_dl_info->id_type >= MRS_DEV_ID_MAX) {
		return HI_ERR_INVALID_PARAMETER;
	}

    if (plc_dl_info->dir_flag != MRS_PLC_DL_FLG) {
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

	/* ��ȡ�豸���� */
	if (mrsToolsIsIICollector()) {
		plc_up_info.id_info[plc_up_info.id_len] = MRS_DEV_TYPE_CLT_II;
	} else if (mrsToolsIsCLTI()) {
		plc_up_info.id_info[plc_up_info.id_len] = MRS_DEV_TYPE_CLT_I;
	} else if (mrsToolsIsTSTA()) {
		plc_up_info.id_info[plc_up_info.id_len] = MRS_DEV_TYPE_THREE_STA;
	} else {
		plc_up_info.id_info[plc_up_info.id_len] = MRS_DEV_TYPE_STA;
	}

    (hi_void)memset_s(&plc_ind_frame, sizeof(plc_ind_frame), 0, sizeof(plc_ind_frame));
    (hi_void)memcpy_s(plc_ind_frame.ucMac, sizeof(plc_ind_frame.ucMac), plc_frame->ucMac, sizeof(plc_frame->ucMac));
	plc_ind_frame.usId = plc_frame->usId;
	plc_ind_frame.usPayloadLen = plc_up_info.stru_len;
	plc_ind_frame.pucPayload = (HI_PBYTE)(&plc_up_info);

	return MRS_SendPlcFrame(&plc_ind_frame);
}

MRS_METER_ITEM_S *mrsStaGetFirstValidMeter(HI_VOID)
{
    MRS_STA_SRV_CTX_STRU *pstStaCtx = mrsStaGetContext();
    HI_U16 i = 0;

    for (i = 0; i < pstStaCtx->stMeterList.ucMeterNum; i++)
    {
        if (HI_TRUE == pstStaCtx->stMeterList.stMeterItem[i].ucValidFlag)
        {
            return &pstStaCtx->stMeterList.stMeterItem[i];
        }
    }

    return HI_NULL;
}

#endif


