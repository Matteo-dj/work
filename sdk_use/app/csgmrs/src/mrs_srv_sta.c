//*****************************************************************************
//
//                  版权所有 (C), 1998-2011, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_srv_sta.c
//  版 本 号   : V1.0
//  作    者   : liuxipeng/KF54842
//  生成日期   : 2011-12-02
//  功能描述   : STA端处理函数的实现
//
//  函数列表   : TODO: ...
//  修改历史   :
//  1.日    期 : 2011-12-02
//    作    者 : liuxipeng/KF54842
//    修改内容 : 创建文件
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
#include "mrs_srv_sta_meter.h"
#include "mrs_srv_clti_searchmeter_csg.h"
#include "mrs_fw_proto_csg_echo.h"
#include "mrs_fw_proto_csg_echo_sta.h"
#include "mrs_srv_csg_plc.h"
#include "mrs_srv_csg_plc_sta.h"
#include "mrs_srv_baudrate_manage.h"
#include "mrs_srv_clti_upg_csg.h"
#include "mrs_srv_capture.h"
#include "mrs_power_failure.h"

//尝试波特率
#define mrsStaNotifyTest()  MRS_StartTimer(MRS_STA_TIMER_TEST, 0, HI_SYS_TIMER_ONESHOT)

#define MRS_STA_TIME_SET_CLT_MAC    (15000)


HI_U32 mrsStaTransmitCmd(HI_PBYTE, HI_U16, HI_U8);
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
HI_U32 mrsStaTransmitCltICmd(HI_PBYTE, HI_U16, HI_U8);
#endif


HI_U32 mrsMsgStaPlcChlStatusNotify(HI_U16 usMsgId,HI_PVOID pMsgParam);
HI_U32 mrsMsgStaDeviceDataRx(HI_IN HI_U16 usMsgId,HI_IN HI_PVOID pMsgParam);

#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
HI_U32 mrsStaRxAfn13Pro(MRS_PLC_FRAME_DATA_STRU *frame, HI_PVOID *data, HI_U16 *len, MRS_STA_ITEM *item);
HI_U32 mrsStaRxAfn14Pro(MRS_PLC_FRAME_DATA_STRU *frame, HI_PVOID *data, HI_U16 *len, MRS_STA_ITEM *item);
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
#endif

HI_U32 mrsStaHeart(HI_VOID);

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
HI_U32 mrsMsgStaDeviceDataRxCsg(HI_U16 usId, HI_IN HI_PVOID);
HI_U32 mrsCmdStaCsgDataTransmit(HI_U16 usId, HI_PVOID pReqPacket, HI_U16 usPacketSize);
#endif


HI_VOID mrsStaHeartTimeoutCnt(HI_U16 id);
HI_U32 mrsMsgStaTimer(HI_U16 usMsgId, HI_IN HI_PVOID pMsgParam);
HI_U32 mrsStaReport2CCO(HI_VOID);

HI_U32 mrsStaReportSmResult2CCO(HI_U8 *pucSrcMac, HI_U8 *pucDstMac);
HI_U32 mrsCltReportSmResult2CCO(HI_U8 *pucSrcMac, HI_U8 *pucDstMac);

HI_U32 mrsStaCreateReadMeterPlc(MRS_PLC_FRAME_DATA_STRU *frame_data,MRS_STA_ITEM * item
                                            ,HI_U8 *data,HI_U16 len,HI_U8 protocol);

// 获取PLC收发模式
HI_U8 mrsStaGetPlcRxTxMode(HI_VOID);

HI_BOOL mrsStaCheckDenyRetry(HI_U16 usId);

HI_BOOL mrsCheckTimingFrame(HI_PBYTE pFrame, HI_U16 usFrameLength);

HI_PRV HI_U32 mrsStaCheckDIValid(HI_U32 ulDestDI, MRS_645_FRAME_STRU *pst645Hrd);
#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
HI_PRV HI_U32 mrsCltRmMeterSetMacProc(MRS_STA_ITEM *pstItem, MRS_645_FRAME_STRU *pst645Hrd);
HI_PRV HI_VOID mrsCltRmMeterSetMacTimeout(HI_VOID);
#endif

HI_PRV HI_U32 mrsStaFrameRxPreProc(MRS_STA_ITEM * pstItem, HI_U8 *pData, HI_U16 usLen, MRS_645_FRAME_STRU *pst645Hrd);
HI_PRV HI_VOID mrsStaAddrUpdateProc(HI_U8 aucAddr[]);


//*****************************************************************************
//全局变量
//*****************************************************************************
HI_PRV MRS_STA_SRV_CTX_STRU* g_pstMrsStaSrvCtx = HI_NULL;
//*****************************************************************************
//函数声明
//*****************************************************************************

HI_PRV HI_U32 mrsStaCheckReport2Cco(HI_U8 report_status);

#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
HI_PRV HI_U32 mrsStaReportDeviceInf(HI_U8 *pucSrcMac, HI_U8 *pucDstMac);
HI_PRV HI_U32 mrsCheckCltRmMeterToMac(MRS_STA_ITEM *pstItem, HI_U8 ucFn);
#endif

//*****************************************************************************
//函数实现
//*****************************************************************************
//读表地址帧
HI_U8 maddr_frame[] =
{
    0xFE,0xFE,0xFE,0xFE,
    0x68,
    0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
    0x68,
    0x13, 0x00, 0xDF, 0x16
};

HI_U8* mrsGetSendAddrFrame(HI_U16 * len)
{
    *len = sizeof(maddr_frame);

    return maddr_frame;
}

/* BEGIN: Added by fengxiaomin/00209182, 2014/5/28   问题单号：DTS2014052808754 */
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

    if(cfg->ucSoftTfMode < SOFT_TF_MODE_INVALID)
    {
        sta->ucSoftTfMode = cfg->ucSoftTfMode;
        HI_MDM_SetSoftTfMode((HI_MDM_SOFT_TF_MODE_EN)sta->ucSoftTfMode);
    }

    sta->usBindDuration = cfg->usBindDuration;

    mrsFwEnableUartDbg(0);

    return;
}


/* END:   Added by fengxiaomin/00209182, 2014/5/28 */

//全局初始化，最开始的初始化
HI_VOID mrsStaGlobalInit(HI_VOID)
{
    mrsStaSaveContext(HI_NULL);

    if (!mrsToolsIsIICollector())
    {
        mrsRegEventOutCallBack(mrsStaEvtOutStatusNotify, MRS_STA_EVT_FROM_METER); //EVENT_OUT管脚异常时的注册回调函数(STA端初始化中)
    }
}

//*****************************************************************************
// 函数名称: mrsStaPowerPro
// 功能描述: MRS模块中STA上电处理函数
//
// 参数说明:
//  无
//
// 返 回 值:
//    无
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: fengxiaomin/00209182 [2014-1-14]
//*****************************************************************************
/* BEGIN: Added by fengxiaomin/00209182, 2014/1/14   问题单号号:DTS2014012607684 */
HI_VOID mrsStaPowerOnPro(HI_VOID)
{
    mrsNvSimuStateCfg(ID_MV_APP_SIMU_METER_INFO);
    mrsEvtOutResetInt();

    return;
}
/* END:   Added by fengxiaomin/00209182, 2014/1/14 */



//*****************************************************************************
// 函数名称: mrsStaUpdateTopAddrInf
// 功能描述: 设置首地址信息
//
// 参数说明:
//  ucAddr [in] : 待更新的表地址
//  ucProtocol [in] : 电表规约类型
// 返 回 值:
//
// 调用要求: 无
// 调用举例: 无
// 作    者: liuxipeng/lkf54842 [2014-06-06]
//*****************************************************************************
HI_U32 mrsStaUpdateTopAddrInf(HI_IN HI_U8 ucAddr[HI_METER_ADDR_LEN], HI_IN HI_U8 ucProtocol)
{
    HI_U8 ucDevAddr[HI_METER_ADDR_LEN] = {0};
    HI_U8 ucType = STA_TYPE_METER;

    // 更新首节点信息
    mrsStaRefreshTopNodeInf(ucAddr, &ucProtocol);

    if(mrsGetSyncMacFlg())
    {
        return HI_ERR_SUCCESS;
    }

    mrsSetSyncMacFlg(HI_TRUE);

    (hi_void)memcpy_s(ucDevAddr, HI_METER_ADDR_LEN, ucAddr, HI_METER_ADDR_LEN);
    mrsHexInvert(ucDevAddr, HI_METER_ADDR_LEN);    // 逆序

    HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1001, HI_DIAG_MT("Node Meter"), ucDevAddr, HI_METER_ADDR_LEN);

    if (mrsToolsIsCLTI())
    {
        ucType = STA_TYPE_CLTI;
        mrsStaSetCltiType(HI_TRUE);
    }

    // 设置电表地址到Mac层
    mrsStaSetMeterToMac(ucDevAddr, ucType, HI_MDM_METER_MAC);

    // 设置电表类型到APP层
    mrsStaSetType(ucType);

    // 更新设备信息
    HI_MDM_DevIdConfig(ucDevAddr);

    return HI_ERR_SUCCESS;
}


// 获取CCO帧新旧标志
HI_BOOL mrsIsOldFrame(HI_VOID)
{
    return mrsStaGetContext()->IsOldFrame;
}
// 设置CCO帧新旧标志
HI_VOID mrsSetFrameFlg(HI_BOOL flg)
{
    mrsStaGetContext()->IsOldFrame = flg;
}

// 获取同步MAC地址结束标志
HI_BOOL mrsGetSyncMacFlg(HI_VOID)
{
    return mrsStaGetContext()->sync_mac_flg;
}

// 设置同步MAC地址结束标志
HI_VOID mrsSetSyncMacFlg(HI_BOOL flg)
{
    mrsStaGetContext()->sync_mac_flg = flg;
    return;
}

//*****************************************************************************
// 函数名称: mrsGetRepFlagNv
// 功能描述: 获取主动上报表地址开关---NV项
//
// 参数说明:
//   HI_VOID
//
// 返 回 值:
//    HI_VOID
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: fengxiaomin/00209182 [2013-02-21]
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
        sta->meterChangeFlag= MRS_STA_REPORT_METER_INFO_VALID;//默认值:打开
        return ;
    }

    sta->staRepFlag = report_info.ucStaRepFlag;
    sta->meterChangeFlag = report_info.ucMeterChangeFlag;
    //sta->ucDuration = report_info.ucDuration;
    mrsDfxReportStatus(sta->staRepFlag);
    mrsDfxChangeStatus(sta->meterChangeFlag);

    return ;
}

//*****************************************************************************
// 函数名称: mrsStaInitCtx
// 功能描述: 初始化STA上下文，只会在第一次初始化的时候分配内存,
//           通讯模块可能会启动多次，每次启动都要初始化上下文
//
// 参数说明:
//   HI_VOID
//
// 返 回 值:
//    HI_VOID
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: niesongsong/nkf62735 [2012-05-29]
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

        //保存上下文
        mrsStaSaveContext(sta);

        //初始化队列
        mrsStaQueueInit(&sta->stQueModule);
    }

    // 设置PLC状态
    sta->plcState = HI_FALSE;

    // 设置同步MAC地址标志(未同步)
    mrsSetSyncMacFlg(HI_FALSE);

    // 重置队列
    mrsStaQueueReset(&sta->stQueModule);

    // 搜表初始化
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

    // 初始化回调
    // 基本配置
    mrsInitStaMrNvInf();

    /* Init STA meter-reading config NV. CNcoment: 初始化STA抄表配置NV。CNend */
    mrsNvStaRmCfg(ID_NV_MRS_RM_CFG);

    // 地址主动上报
    mrsGetRepFlagNv();
    HI_MDM_NV_RegisterChangedNoitfyProc(HI_NV_FTM_COLLECT_PARAM_FILE,
                                     HI_NV_FTM_COLLECT_PARAM_FILE,
                                     mrsNvChangeMsgNotify);
    HI_MDM_NV_RegisterChangedNoitfyProc(ID_NV_APP_MR_CFG, ID_NV_APP_STA_SM_CFG_EXP, mrsNvChangeMsgNotify);
    HI_MDM_NV_RegisterChangedNoitfyProc(ID_NV_MRS_USER_VER, ID_NV_MRS_USER_VER, mrsNvChangeMsgNotify);

    // 初始化PLC过滤缓存
    if(sta->stFrameBufCtx.item == HI_NULL)
    {
		HI_U32 iItemSize = sizeof(MRS_STA_FRAME_BUF_ITEM_STRU) * sta->stFrameBufCtx.max_count;
        sta->stFrameBufCtx.item = (MRS_STA_FRAME_BUF_ITEM_STRU *)mrsToolsMalloc(iItemSize);
		if (sta->stFrameBufCtx.item)
		{
		    (hi_void)memset_s(sta->stFrameBufCtx.item, iItemSize, 0, iItemSize);
		}
    }

    // 获取模块的MAC地址
    HI_MDM_GetLocalMacAddr(macAddr);
    // 设置模块MAC地址信息
    mrsStaSetMacAddr(macAddr);

    // 初始化工装上下文
    mrsDutCtxInit();

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
    mrsStaProtoCsgEchoInit();
    mrsStaCsgPlcModuleInit();
#endif

    //电表地址通知处理初始化必须放在mrsStaEvtCtxInit和mrsStaDutyCycleInit前，否则会将注册的函数指针清零
    //后续如有增加注册，也需要将注册放在mrsMeterChgNotifyProcInit后
/* BEGIN: Added by fengxiaomin/00209182, 2015/10/23   问题单号号:DTS2015102710230 */
    mrsMeterChgNotifyProcInit();
/* END:   Added by fengxiaomin/00209182, 2015/10/23 */
    mrsStaEvtCtxInit();

    if (EQUIP_DUT_IS_EQUIP_MODE())
    {
        mrsStaSetState(MRS_STA_STATE_MR);
        HI_MDM_LED_SetReadMeterAddrStatus(LED_READ_METER_SUCCESS);
    }
    else
    {
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
        HI_U32 ulTime = 0;
#endif

        //设置初始化状态(未探测波特率)
        mrsStaSetState(MRS_STA_STATE_INIT);

        mrsStaNotifyTest();

    #if defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG) && defined(MRS_LOG_STA_SWITCH)
        // 周期性记录日冻结统计定时器
        MRS_StartTimer(MRS_STA_TIMER_DAYFREEZE_REC, MRS_STA_TIME_DAYFREEZE_REC, HI_SYS_TIMER_PERIODIC);
    #endif

        if(!mrsToolsIsIICollector())
        {
            mrsStaDutyCycleInit();
        }

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
        ulTime = HI_MDM_GetMilliSeconds();
        if (MRS_STA_TIME_SET_CLT_MAC >= ulTime)
        {
            ulTime = MRS_STA_TIME_SET_CLT_MAC - ulTime;
            MRS_StartTimer(MRS_STA_TIMER_SET_CLT_MAC, ulTime, HI_SYS_TIMER_ONESHOT);
        }

        if (mrsToolsIsCLTI())
        {
            mrsCsgCltIUpgInit();
        }
#endif
    }
}

/* BEGIN: Added by fengxiaomin/00209182, 2014/2/19   问题单号:DTS2014012301705 */
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
    if (mrsGetSyncMacFlg())
    {
        MRS_TOOLS_FE_TO_00(ucAddr[MRS_MAC_ADDR_INDEX_5]);
    }

    if (!mrsToolsCheckBCD(ucAddr, HI_METER_ADDR_LEN))
    {
        (hi_void)memset_s(ucAddr, sizeof(ucAddr), 0, sizeof(ucAddr));
    }

    ret = HI_MDM_DevIdConfig(ucAddr);
    return ret;
}
/* END:   Added by fengxiaomin/00209182, 2014/2/19 */

//*****************************************************************************
// 函数名称: mrsMsgStaPlcChlStatusNotify
// 功能描述: PLC通道状态改变通知
//
// 参数说明:
//   HI_U16   usMsgId       消息ID(正常情况下可以忽略)
//   HI_PVOID pMsgParam     通知内容
// 返 回 值:
//    成功返回  HI_ERR_SUCCESS
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: niesongsong/nkf62735 [2012-05-29]
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

        //上报表地址和MAC地址
        mrsStaCheckReport2Cco(MRS_STA_REPORT_METER_ADDR_ACTIVE);

        // TODO：
        mrsStaSyncMacJoinNetAddr();
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
// 函数名称: mrsDutUartDataPro
// 功能描述: 接收到工装串口数据的处理。
//
// 参数说明:
//   MRS_645_FRAME_STRU pst645Hrd   645帧内容
//
// 返 回 值:
//
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: fengxiaomin/00209128 [2013-05-30]
//*****************************************************************************
HI_U32 mrsDutUartDataPro(MRS_645_FRAME_STRU *pst645Hrd)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    MRS_DUT_645_FRAME_STRU st645Frame = { 0 };// 是否修改MALLOC
    MRS_DUT_645_DI_FUN pDutDIPro = 0;

    //为工装645帧格式赋值
    st645Frame.ucFrameStart = 0x68;
    (hi_void)memcpy_s(st645Frame.ucAddr, HI_METER_ADDR_LEN, pst645Hrd->ucAddr, sizeof(pst645Hrd->ucAddr));
    st645Frame.ucFrameEnd = 0x68;
    (hi_void)memcpy_s(&st645Frame.stCtrl, sizeof(st645Frame.stCtrl), &pst645Hrd->stCtrl, sizeof(st645Frame.stCtrl));
    st645Frame.ucDataRealmLen = pst645Hrd->ucDataRealmLen;
/* BEGIN: Modified by fengxiaomin/00209182, 2014/6/19   问题单DTS2014071004301 */
	if(MRS_PROTO645_DATA_CS_END_LEN_MAX >= pst645Hrd->ucDataRealmLen ){
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
    pDutDIPro = mrsDutFramePro(&st645Frame);      //获得对应DI标识的处理函数

    if (pDutDIPro != 0)
    {
        ret = pDutDIPro(&st645Frame);//DI标识处理函数
    }

    return ret;
}


//*****************************************************************************
// 函数名称: mrsMsgStaDeviceDataRx
// 功能描述: 接收电表数据，转发给CCO,这个函数在初始化时候被注册，在电表有数据
//           发送给集中器时被调用。
//
// 参数说明:
//   HI_U16   usMsgId       消息ID(正常情况下可以忽略)
//   HI_PVOID pMsgParam     数据内容
// 返 回 值:
//    成功返回  HI_ERR_SUCCESS
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: niesongsong/nkf62735 [2012-05-29]
//*****************************************************************************
HI_U32 mrsMsgStaDeviceDataRx(HI_IN HI_U16 usMsgId,HI_IN HI_PVOID pMsgParam)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    MRS_STA_SRV_CTX_STRU *sta = mrsStaGetContext();
    MRS_MSG_MR_645_DATA_STRU* pstData = HI_NULL;
    MRS_645_FRAME_STRU st645Hrd = { 0 };
    MRS_STA_ITEM * item;
    MRS_QUE_MODULE *sta_queue;
    HI_U8 dut_addr[6] = DUT_NORMAL_MODE_ADDR;

    HI_UNREF_PARAM(usMsgId);

    // 参数判断
    if (!pMsgParam)
    {
        return HI_ERR_FAILURE;
    }

#if defined(PRODUCT_CFG_SYS_RST_SOLUTION)
    sta->stStaResetInfo.ulRxUartTimes++;
#endif

    pstData = (MRS_MSG_MR_645_DATA_STRU*)pMsgParam;
    sta_queue = &sta->stQueModule;

    // 解析645帧
    ret = MRS_Proto645Dec(pstData->pucData, pstData->usDataSizse, &st645Hrd);
    pstData->ucProtocol = st645Hrd.ucVer;

    if (ret != HI_ERR_SUCCESS)
    {
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_010, HI_DIAG_MT("645 err"));
        return HI_ERR_FAILURE;
    }

    // 判断是否是工装发来的查询mac地址和版本号信息
    mrsHexInvert(dut_addr, sizeof(dut_addr));
    if (memcmp(st645Hrd.ucAddr, dut_addr, sizeof(st645Hrd.ucAddr)) == 0)
    {
        mrs645DataDecode(st645Hrd.ucDataRealm, st645Hrd.ucDataRealmLen);
        ret = mrsDutUartDataPro(&st645Hrd);
        return ret;
    }

    if (mrsToolsIsIICollector())
    {
        mrsUARTRTxLedPro();
    }
    else
    {
        MRS_StartTimer(MRS_STA_TIMER_HEART,
                       MRS_STA_TIME_HEART,
                       HI_SYS_TIMER_PERIODIC);
    }

    item = mrsSrvQueueTop(&sta_queue->stMrQueue);

    do
    {
#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
        if (item && item->id == MRS_STA_ITEM_ID_SEARCH_METER_CLT_I)
        {
            break;
        }
#endif

        if (st645Hrd.stCtrl.ucFn != 0x13)
        {
            break;
        }

        //判定是13 00应答
        if ((MRS_STA_STATE_INIT == mrsStaGetState()) && (st645Hrd.ucDataRealmLen == HI_METER_ADDR_LEN))
        {
/* BEGIN: Added by fengxiaomin/00209182, 2015/10/23   问题单号:DTS2015102710230 */
            HI_U8 aucAddr[HI_METER_ADDR_LEN] = {0};

            // 进入抄表状态
            mrsStaSetState(MRS_STA_STATE_MR);
            MRS_StopTimer(MRS_STA_TIMER_TEST);
            HI_MDM_LED_SetReadMeterAddrStatus(LED_READ_METER_SUCCESS);
            (hi_void)memcpy_s(aucAddr, HI_METER_ADDR_LEN, st645Hrd.ucDataRealm, HI_METER_ADDR_LEN);
            mrs645DataDecode(aucAddr, HI_METER_ADDR_LEN);

            //判定是电表
            if (mrsToolsNormalAddr(st645Hrd.ucAddr)
                && mrsToolsMemEq(st645Hrd.ucAddr, aucAddr, HI_METER_ADDR_LEN)
                && (!mrsToolsIsCLTI()))
            {
                if (!item)
                {
                    mrsStaUpdateAddr(st645Hrd.ucAddr, st645Hrd.ucVer);
                    mrsStaCheckReport2Cco(MRS_STA_REPORT_METER_ADDR_ACTIVE);
                }

                mrsStaMeterRxNotify(st645Hrd.ucAddr, st645Hrd.ucVer, &st645Hrd.stCtrl);
/* END:   Added by fengxiaomin/00209182, 2015/10/23 */
                // 更新节点电表地址并同步到Mac层
                mrsStaUpdateTopAddrInf(st645Hrd.ucAddr,st645Hrd.ucVer);
            }
#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
            else if (mrsToolsIsSTA())//当电表模块插在应答地址的采集器中，模块识别为I采模块
            {
                // CLT I
                mrsCltiRx1300Proc(st645Hrd.ucAddr, &st645Hrd.stCtrl);
                mrsStaEvtOutProc();
            }
#endif

            if (!item)
            {
                return HI_ERR_SUCCESS;
            }

            break;
        }
    } while (HI_FALSE);

#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
    if (item && item->id == MRS_STA_ITEM_ID_SEARCH_METER_CLT_I)
    {
        if (st645Hrd.stCtrl.ucFn == 0x13)
        {
            mrs645DataDecode(st645Hrd.ucDataRealm, st645Hrd.ucDataRealmLen);
            mrsCltiSmFrameRx(&st645Hrd);
            return HI_ERR_SUCCESS;
        }
        else if ((st645Hrd.stCtrl.ucFn == 0x11) || (st645Hrd.stCtrl.ucFn == 0x01))
        {
            mrs645DataDecode(st645Hrd.ucDataRealm, st645Hrd.ucDataRealmLen);
            if (item->option == MRS_CLTI_FRAME_TYPE_POSITIVE_A)
            {
                mrsCltiTryReadPositiveARx(&st645Hrd);
            }

            return HI_ERR_SUCCESS;
        }
    }
#endif

    // 更新表地址
    if (MRS_645_FRAME_CONTROL_DIR_RESPONSION_BIT == st645Hrd.stCtrl.ucDir)
    {
        if ((MRS_PROTO645_VER_1997 == st645Hrd.ucVer)
            && (MRS_645_FRAME_CONTROL_READ_DATA97 == st645Hrd.stCtrl.ucFn)
            && (st645Hrd.ucDataRealmLen >= sizeof(HI_U16)))
        {
            HI_U16 usDi97 = HI_MAKEU16(st645Hrd.ucDataRealm[0] - MRS_645_FRAME_HEX33, st645Hrd.ucDataRealm[1] - MRS_645_FRAME_HEX33);
            mrsStaUpdateAddrProc(st645Hrd.ucAddr, st645Hrd.ucVer, usDi97);
        }
        else
        {
            if (!mrsIsRealClti() || (st645Hrd.stCtrl.ucFn != 0x13))
            {
                mrsStaUpdateAddr(st645Hrd.ucAddr, st645Hrd.ucVer);
            }
        }
    }

    // 队列是空的，收到的数据无效
    if (!item)
    {
        return HI_ERR_SUCCESS;
    }

#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
    mrsCltRmMeterSetMacProc(item, &st645Hrd);
#endif

    if (HI_TRUE == mrsStaCheckDenyRetry(item->id))
    {
        // 点抄和轮抄时，收到否认帧判为不合法，重读
        // 重读仍为否认，则上报该否认到CCO
        if((st645Hrd.stCtrl.ucSlaveFlag)
            && ((sta_queue->retries + 1) <= MRS_QUEUE_MAX_TRY))
        {
            return HI_ERR_SUCCESS;
        }
    }

    if (HI_ERR_SUCCESS != mrsStaFrameRxPreProc(item, pstData->pucData, pstData->usDataSizse, &st645Hrd))
    {
        return HI_ERR_CONTINUE;
    }

    // 关闭超时定时器
    mrsStaStopTtlTimer(item);

    switch (item->from)
    {
    case MRS_STA_QUEUE_FROM_STA:
        if (item->id == MRS_STA_ITEM_ID_EVENT)
        {
            mrsStaEvtProc(pstData->pucData, pstData->usDataSizse, &st645Hrd);
        }
#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
        else if ((item->id == MRS_STA_ITEM_ID_CLTII_EVENT) || (item->id == MRS_STA_ITEM_ID_DUTY_CYCLE) || (item->id == MRS_STA_ITEM_ID_CLTI_EVENT))
        {
            // 上面已处理
        }
        else if (item->id == MRS_STA_ITEM_ID_SET_VER_CLT_I)
        {
            mrsCltiSmStartTx();
        }
#endif
		else if (item->id == MRS_STA_ITEM_ID_HEART)
		{
			mrsStaAddrUpdateProc(st645Hrd.ucAddr);
		}
		else
		{
			mrsStaCheckReport2Cco(MRS_STA_REPORT_METER_ADDR_ACTIVE);
		}
        break;

    case MRS_STA_QUEUE_FROM_PLC:
#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
        if(item->id == ID_MRS_CMD_PARALLEL_DATA_PLC)
        {
            mrsDfxPrUartRx();
/* BEGIN: Modified by fengxiaomin/00209182, 2014/3/27   问题单号:DTS2014032906596 */
            mrsStaDfxRmUartRx(item->id, pstData->pucData,pstData->usDataSizse);
            mrsStaDfxRmRxDeny(item->id, st645Hrd.stCtrl.ucSlaveFlag);
            mrsStaDfxRmRxErrData(item->id, st645Hrd.ucDataRealm, st645Hrd.ucDataRealmLen);
/* END:   Modified by fengxiaomin/00209182, 2014/3/27 */

            mrsParallelRefreshRespFlg(HI_TRUE);
            mrsParallelRefreshFrameStatus(st645Hrd.stCtrl.ucSlaveFlag);

            ret = mrsParallelMutliFrameProcess(item,
                                pstData->pucData,
                                pstData->usDataSizse,
                                pstData->ucProtocol);
            if(ret == HI_ERR_CONTINUE)
            {
                return ret;
            }
        }
        else
#endif
        {
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
            if (item->id == MRS_STA_ITEM_ID_CSG_TRANS_XR)
            {
                mrsDfxXrUartRx();
            }
#else
            if (item->id == ID_MRS_CMD_DATA_TRANSMIT_AFN13_PLC)
            {
                mrsDfxXrUartRx();
            }
            else if(item->id == ID_MRS_CMD_DATA_TRANSMIT_AFN14_PLC)
            {
                mrsDfxLrMeterRx();
            }
#endif

/* BEGIN: Modified by fengxiaomin/00209182, 2014/3/27   问题单号:DTS2014032906596 */
            mrsStaDfxRmUartRx(item->id, pstData->pucData,pstData->usDataSizse);
            mrsStaDfxRmRxDeny(item->id, st645Hrd.stCtrl.ucSlaveFlag);
            mrsStaDfxRmRxErrData(item->id, st645Hrd.ucDataRealm, st645Hrd.ucDataRealmLen);
/* END:   Modified by fengxiaomin/00209182, 2014/3/27 */

            mrsStaSerialRespRMPLC(item, pstData);
        }
		mrsStaAddrUpdateProc(st645Hrd.ucAddr);
        break;

    case MRS_STA_QUEUE_FROM_LOCAL:
        HI_MDM_ReportPacket(HI_STA_TRANSMIT,
                             HI_DIAG_CMD_INSTANCE_LOCAL,
                             pstData->pucData,
                             pstData->usDataSizse,
                             HI_TRUE);

        break;

    case MRS_STA_QUEUE_FROM_REMOTE:
        mrsStaDiag2HSO(pstData->pucData,
                       pstData->usDataSizse,
                       HI_DIAG_CMD_INSTANCE_IREMOTE);
        break;

    case MRS_STA_QUEUE_FROM_REMOTE_HSO:
        HI_MDM_ReportPacket(HI_STA_TRANSMIT,
                             HI_DIAG_CMD_INSTANCE_IREMOTE,
                             pstData->pucData,
                             pstData->usDataSizse,
                             HI_TRUE);
        break;

    case MRS_STA_QUEUE_FROM_IR:
/* BEGIN: PN: DTS2015102007386  MODIFY\ADD\DEL by cuichunyan/00276962 at 2015/10/08 */
        mrsIrTransmitUp(item->sub_id, pstData->pucData, pstData->usDataSizse);
/* END: PN: DTS2015102007386  MODIFY\ADD\DEL by cuichunyan/00276962 at 2015/10/08 */

        break;

    default:
        break;
    }

    // 出队列
    return mrsStaTryDeQueue(sta_queue,mrsStaQueueFree);
}

HI_VOID mrsStaAddrUpdateProc(HI_U8 aucAddr[])
{
	HI_U32 ret = HI_ERR_SUCCESS;
    HI_U8 ucIp[HI_IPV4_ADDR_LEN] = {0};
	HI_U8 ucCurrMac[HI_PLC_MAC_ADDR_LEN] = {0};

	if (!mrsToolsIsMeter())
	{
		return;
	}

    ret = HI_MDM_NM_GetAddr(ucCurrMac, ucIp);
    if(ret != HI_ERR_SUCCESS)
    {
        return;
    }

    mrsHexInvert(aucAddr, HI_METER_ADDR_LEN);
	if (mrsToolsMemEq(aucAddr, ucCurrMac, HI_PLC_MAC_ADDR_LEN))
	{
		return;
	}

	mrsStaSetMeterToMac(aucAddr, STA_TYPE_METER, HI_MDM_METER_MAC);
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
        switch (pstItem->id)
        {
        case MRS_STA_ITEM_ID_EVENT:
            ulRet = mrsStaCheckDIValid(MRS_STA_EVT_STATUS_WORD_DI, pst645Hrd);
            break;

        case MRS_STA_ITEM_ID_DUTY_CYCLE:
            ulRet = mrsStaDutyCycleFrameRx(pData, usLen);
            break;

#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)

        case MRS_STA_ITEM_ID_SET_VER_CLT_I:
            if (MRS_645_FRAME_CONTROL_READ_ADDR != pst645Hrd->stCtrl.ucFn)
            {
                return HI_ERR_BAD_DATA;
            }

            ulRet = mrsStaCheckDIValid(MRS_CLTI_SET_VER_DI, pst645Hrd);
            break;

        case MRS_STA_ITEM_ID_CLTII_EVENT:
            ulRet = mrsCltiiEventFrameRx(pData, usLen);
            break;

        case MRS_STA_ITEM_ID_CLTI_EVENT:
            ulRet = mrsCltiiEventFrameRx(pData, usLen);
            break;
#endif

        default:
            break;
        }
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

        if (!mrsStaIsMr645FrameMatch(pucFrame, usLength, pData, usLen))
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


#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
HI_U32 mrsMsgStaDeviceDataRxCsg(HI_U16 usId, HI_IN HI_PVOID pMsgParam)
{
    HI_U32 ulRet = HI_ERR_NOT_FOUND;

    MRS_MSG_MR_3762_DATA_STRU* pstData = HI_NULL;
    MRS_STA_SRV_CTX_STRU *pstStaCtx = mrsStaGetContext();

    HI_UNREF_PARAM(usId);

    if (pMsgParam == HI_NULL)
    {
        return HI_ERR_FAILURE;
    }

    //CCO端收到UART数据，统计量加1
#if defined(PRODUCT_CFG_SYS_RST_SOLUTION)
    pstStaCtx->stStaResetInfo.ulRxUartTimes++;
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
                    ulRet = MRS_SendMrData(pOutBuffer, usBufferOutLen, HI_DMS_CHL_UART_PORT_APP); // 发送"响应"帧
                }
            }
            mrsToolsFree(pOutBuffer);
        }
        mrsToolsFree(pstFrame);
    }

    return ulRet;
}
#endif


#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
HI_U32 mrsCheckCltRmMeterToMac(MRS_STA_ITEM *pstItem, HI_U8 ucFn)
{
    MRS_STA_SRV_CTX_STRU *sta = mrsStaGetContext();

    if (EQUIP_DUT_IS_EQUIP_MODE() || (0x13 == ucFn) || sta->stRmMeterInfo.bValid)
    {
        return HI_ERR_SKIP;
    }

    if ((!mrsIsRealClti() && !mrsToolsIsIICollector()) || (HI_TRUE == mrsGetSyncMacFlg()))
    {
        return HI_ERR_SKIP;
    }

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

    //检查BCD码及广播地址
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
    if (MRS_PROTO645_VER_1997 == sta->stRmMeterInfo.ucProtocol)
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
    MRS_STA_RM_METER_INFO *pstRmMeterInfo = &mrsStaGetContext()->stRmMeterInfo;

    if (EQUIP_DUT_IS_EQUIP_MODE() || (HI_TRUE == mrsGetSyncMacFlg()))
    {
        return;
    }

    if (mrsIsRealClti())
    {
        MRS_StopTimer(MRS_STA_TIMER_TEST);
    }

    mrsStaSearchSetMeter2Mac(pstRmMeterInfo->aucAddr, pstRmMeterInfo->ucProtocol, pstRmMeterInfo->us97DI);
}
#endif


//*****************************************************************************
// 函数名称: mrsStaCheckDIValid
// 功能描述: STA接收到电表应答后，检查DI是否正确，其中否认帧认为正确
//
// 参数说明:
//   ulDestDI    预期DI
//   pst645Hrd   收到的645信息
//
// 返 回 值:
//    HI_ERR_SUCCESS  正确
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: fengxiaomin/f00209182 [2015-10-30]
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
// 函数名称: mrsStaCheckDenyRetry
// 功能描述: STA接收到否认帧后，是否需要重发
//
// 参数说明:
//   usId     队列首元素id
//
// 返 回 值:
//    需要重发返回  HI_TRUE
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: fengxiaomin/f00209182 [2015-07-18]
//*****************************************************************************
HI_BOOL mrsStaCheckDenyRetry(HI_U16 usId)
{
    if (mrsToolsIsIICollector() && (HI_TRUE == mrsIsCltiiTiming()))
    {
        return HI_FALSE;
    }
    else
    {
        if((usId == ID_MRS_CMD_DATA_TRANSMIT_AFN13_PLC)
            || (usId == ID_MRS_CMD_DATA_TRANSMIT_AFN14_PLC)
            || (usId == MRS_STA_ITEM_ID_SET_VER_CLT_I))
        {
            return HI_TRUE;
        }
    }

    return HI_FALSE;
}

//*****************************************************************************
// 函数名称: mrsStaSerialRespRMPLC
// 功能描述: 点抄轮抄时收到电表表应答后,上报给CCO
//
// 参数说明:
//   pstItem      队列头节点信息
//   pstData      收到的电表数据信息
//
// 返 回 值:
//    成功返回  HI_ERR_SUCCESS
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: fengxiaomin/00209182 [2015-07-20]
//*****************************************************************************
HI_U32 mrsStaSerialRespRMPLC(MRS_STA_ITEM *pstItem, MRS_MSG_MR_645_DATA_STRU* pstData)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
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
        mrsStaFrameBufInsert(pstItem->id, pstItem->seq, usDataLen, pData, ucProtocol);
    }

    if (HI_TRUE == mrsIsCltiiTiming())
    {
        mrsToolsFree(pData);
    }

#else

    if (pstItem->id == MRS_STA_ITEM_ID_CSG_TRANS_XR)
    {
        ulRet = mrsStaCsgTransmitXr2CCO(pstData->pucData, pstData->usDataSizse, pstItem);
        mrsStaFrameBufInsert(pstItem->id, pstItem->seq, pstData->usDataSizse, pstData->pucData, MRS_PROTO645_VER_2007);
    }

#endif

    return ulRet;
}

//*****************************************************************************
// 函数名称: mrsStaUpdateAddr
// 功能描述: STA的查询得到了电表的回复从而更新全局上文中的电表地址记录
//
// 参数说明:
//   HI_U8 meterAddr[]      电表地址
//   int len                长度，一般为6
//   HI_U8 ucProtocol       电表的协议类型
// 返 回 值:
//    成功返回  HI_ERR_SUCCESS
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: niesongsong/nkf62735 [2012-05-29]
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

    //检查BCD码及广播地址
    if (!mrsToolsNormalAddr(aucMeterAddr))
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    //查找电表
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

        //5.2 淘汰原则,TTL维护，以使用次数为维护指标
        if (pMeter->usOption > 0)
        {
            pMeter->usOption--;
        }
    }

    //电表不存在，取TTL最小的位置
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

    //更新电表
    pMeter = & pMeterList->stMeterItem[found];
    (hi_void)memcpy_s(pMeter->ucMeterAddr, HI_METER_ADDR_LEN, meterAddr, HI_METER_ADDR_LEN);

    //更新TTL值
    //如果电表地址无效
    if (pMeter->ucValidFlag == 0)
    {
        pMeterList->ucMeterNum ++;
        pMeter->ucValidFlag = 1;
    }

    pMeter->usOption = MRS_STA_LOCAL_MAP_ENTRY_TTL_MAX;
    pMeter->ucProtocol = ucProtocol;
    usBaudRate = (HI_U16)mrsGetCurBaudRate();

    if (!bUpdated && mrsToolsIsIICollector() && (pMeter->usBaudRate != usBaudRate))
    {
        mrsStaSearchAddRmMeter(meterAddr, ucProtocol, usDi97);
    }
    pMeter->usBaudRate = usBaudRate;

    mrsCltiiClearLastBcMeterInf();

    if (report_flag)
    {
        //做上报表地址的操作
        mrsStaCheckReport2Cco(MRS_STA_REPORT_METER_ADDR_CHANGE);
    }

    return HI_ERR_SUCCESS;
}
/* END:   PN: DTS2015051601564 MODIFY\ADD\DEL by cuiate/00233580 at 2015/5/15 */

//*****************************************************************************
// 函数名称: mrsStaHeart
// 功能描述: 心跳函数，发送查询的645帧
//
// 参数说明:
//   HI_U8 meterAddr[]      电表地址
//   int len                长度，一般为6
//   HI_U8 ucProtocol       电表的协议类型
// 返 回 值:
//    成功返回  HI_ERR_SUCCESS
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: niesongsong/nkf62735 [2012-05-29]
//*****************************************************************************
HI_U32 mrsStaHeart(HI_VOID)
{
    MRS_STA_ITEM *item;
    MRS_STA_SRV_CTX_STRU *sta = mrsStaGetContext();
    HI_U16 ulSendLen = 0;
    HI_U8 *pSendFrame = HI_NULL;

    item = mrsToolsMalloc(sizeof(MRS_STA_ITEM));
    if (!item)
    {
        return HI_ERR_FAILURE;
    }

    (hi_void)memset_s(item, sizeof(MRS_STA_ITEM), 0, sizeof(MRS_STA_ITEM));

    pSendFrame = mrsGetSendAddrFrame(&ulSendLen);
    item->from = MRS_STA_QUEUE_FROM_STA;
    item->buf = pSendFrame;
    item->len = ulSendLen;
    item->id = MRS_STA_ITEM_ID_HEART;
    item->time_out = (HI_U8)MRS_MS_TO_100MS(mrsGet645FrameHeadTimeout(MRS_STA_RM_CFG_LOCAL));
    item->bAutoStrategy = HI_TRUE;
    item->ucFrameTimeout = (HI_U8)MRS_MS_TO_100MS(mrsGet645FrameInterval(MRS_STA_RM_CFG_LOCAL));

    if (!mrsStaTryEnQueue(&sta->stQueModule,item))
    {
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_017, HI_DIAG_MT("heart fail"));
        mrsToolsFree(item);
        return HI_ERR_FAILURE;
    }

    mrsDfxHeartCnt(); // 心跳次数统计

    return HI_ERR_SUCCESS;
}


#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
HI_U32 mrsStaRxAfn13Pro(MRS_PLC_FRAME_DATA_STRU *frame,HI_PVOID *data,HI_U16 *len
                                        ,MRS_STA_ITEM *item)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U16 payload_len = 0;
    HI_U16 local_len = 0;
    MRS_DATA_TRANSMIT_AFN13_DL_STRU *transmit = HI_NULL;

    transmit = (MRS_DATA_TRANSMIT_AFN13_DL_STRU *)frame->pucPayload;

    if(transmit->dir != MRS_PLC_DL_FLG)
    {
        return HI_ERR_BAD_FRAME;
    }
    *len = transmit->data_len;
    *data = (HI_U8*)frame->pucPayload + transmit->stru_size;
    item->time_out= transmit->time_out;
    item->seq= transmit->seq;
    payload_len = frame->usPayloadLen;
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

    transmit_data = (MRS_DATA_TRANSMIT_AFN14_DL_STRU *)frame->pucPayload;
    *len = transmit_data->data_len;
    *data = (HI_U8*)frame->pucPayload + transmit_data->stru_size;
    item->time_out= transmit_data->time_out;
    item->seq= transmit_data->seq;
    payload_len = frame->usPayloadLen;
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
// 函数名称: mrsCmdStaSerialTransmit
// 功能描述: 处理串行的转发命令
//
// 参数说明:
//   HI_U16 usId                      命令ID，此参数大部分情况可心忽略
//   MRS_PLC_FRAME_DATA_STRU* pstReq  收到的CCO数据，待转发
// 返 回 值:
//    成功返回  HI_ERR_SUCCESS
//
// 调用要求:
// 调用举例:
// 作    者: niesongsong/nkf62735 [2012-05-29]
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
    HI_U16 usTimeout = mrsGet645FrameHeadTimeout(MRS_STA_RM_CFG_AUTO);

    HI_U8 *pFrame = HI_NULL;
    HI_U8 *pTempFrame = HI_NULL;
    HI_U16 usFrameLength = 0;

    plc_Req_frame = (MRS_PLC_FRAME_DATA_STRU*)req_packet;
    MRS_NOT_USED(req_packet_size);

    (hi_void)memset_s(&temp, sizeof(temp), 0, sizeof(temp));

    //检查参数
    if (!plc_Req_frame || !plc_Req_frame->pucPayload)
    {
        return HI_ERR_FAILURE;
    }

    if (id == ID_MRS_CMD_DATA_TRANSMIT_AFN13_PLC)
    {
        HI_U16 usPos = 0;
        HI_U16 usLength = 0;

        mrsDfxXrPlcRx();
        ret = mrsStaRxAfn13Pro(plc_Req_frame, (HI_PVOID*)&payload, &payload_len, &temp);

        // CTRL command
        if ((HI_ERR_SUCCESS == mrsFind645Frame(payload, (HI_S16)payload_len, &usPos, &usLength))
            && MRS_645_IS_CTRL_CMD(payload[usPos + MRS_645_FRAME_CTRL_OFFSET]))
        {
            usTimeout += mrsGet645CtrlCmdDelay(MRS_STA_RM_CFG_AUTO);
        }
    }
    else if (id == ID_MRS_CMD_DATA_TRANSMIT_AFN14_PLC)
    {
        mrsDfxLrPlcRx();
        ret = mrsStaRxAfn14Pro(plc_Req_frame, (HI_PVOID*)&payload, &payload_len, &temp);
    }

/* BEGIN: Modified by fengxiaomin/00209182, 2014/3/27   问题单号:DTS2014032906596 */
    mrsDfxRmPlcDlRx(id, plc_Req_frame->pucPayload, HI_FALSE);
/* END:   Modified by fengxiaomin/00209182, 2014/3/27 */

    if (ret != HI_ERR_SUCCESS)
    {
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_026, HI_DIAG_MT("rx plc err"));
        return HI_ERR_FAILURE;
    }

#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
    if (mrsToolsIsIICollector())
    {
        if (ID_MRS_CMD_DATA_TRANSMIT_AFN13_PLC == id)
        {
            ret = mrsCltiiEventPlcProc(payload, payload_len, id, temp.seq);
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

        ret = mrsCltiiProtoProc(payload, payload_len, id, temp.seq);
        if (HI_ERR_SUCCESS == ret)
        {
            return ret;
        }
    }
#endif

    //判断数据域是否为空
    if (!payload)
    {
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_027, HI_DIAG_MT("payload null"));
        return HI_ERR_FAILURE;
    }

#if defined(PRODUCT_CFG_SYS_RST_SOLUTION)
    sta->stStaResetInfo.ulRxRMPlcTimes++;
#endif

    pFrame = payload;
    usFrameLength = payload_len;

    //将数据发送到队列以后处理
    item = mrsToolsMalloc(sizeof(MRS_STA_ITEM) + usFrameLength);
    if (item == HI_NULL)
    {
        mrsToolsFree(pTempFrame);
        return HI_ERR_FAILURE;
    }
    (hi_void)memset_s(item,sizeof(MRS_STA_ITEM) + usFrameLength, 0,sizeof(MRS_STA_ITEM) + usFrameLength);

    item->from     = MRS_STA_QUEUE_FROM_PLC;
    item->bcFlg    = plc_Req_frame->is_bc_frame;
    item->id       = id;
    item->len      = usFrameLength; // 获取数据长度
    item->buf      = (HI_U8 *)item + sizeof(MRS_STA_ITEM);   // 偏移数据缓冲区指针
    item->seq      = temp.seq;
    item->time_out = (HI_U8)MRS_MS_TO_100MS(usTimeout);
    item->bAutoStrategy = HI_TRUE;
    item->ucFrameTimeout = (HI_U8)MRS_MS_TO_100MS(mrsGet645FrameInterval(MRS_STA_RM_CFG_AUTO));

    (hi_void)memcpy_s(item->buf, item->len, pFrame, usFrameLength);     // 报文内容

    if ((ID_MRS_CMD_DATA_TRANSMIT_AFN13_PLC == id) && mrsCheckTimingFrame(payload, payload_len))
    {
        item->sub_id = MRS_STA_TRANSFER_TIMING_NORMAL;
    }

    if (mrsToolsIsIICollector()) //如果当前为II型采集器
    {
        if (HI_TRUE == bTimingFlag)
        {
            item->sub_id = MRS_STA_TRANSFER_TIMING_CLTII_EX;
        }
    }

    // 如果当前为工装模式则直接发送PLC应答帧-固定内容
    if(EQUIP_DUT_IS_EQUIP_MODE())
    {
        MRS_PLC_FRAME_DATA_STRU stPlcData = {0};
        HI_U8 respFrame[] = { 0x68, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
                              0x68, 0x91, 0x08, 0x33, 0x33, 0x34, 0x33,
                              0x75, 0x33, 0x33, 0x33, 0x45, 0x16 };

        mrsStaCreateReadMeterPlc(&stPlcData, item, respFrame, sizeof(respFrame), MRS_PROTO645_VER_2007);
        MRS_SendPlcFrame(&stPlcData);

        mrsToolsFree(stPlcData.pucPayload);
        mrsToolsFree(item);
    }
    // 如果当前为正常模式则入队列进行读表
    else
    {
        //尝试加入队列
        if(!mrsStaTryEnQueue(&sta->stQueModule,item))
        {
            HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_029, HI_DIAG_MT("in queue fail"));

            mrsToolsFree(item);
        }
        #if defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG) && defined(MRS_LOG_STA_SWITCH)
        else
        {
            mrsLogFrameInfStatTx(item->buf, item->len, HI_FALSE);
        }
        #endif

        //通知队列
        mrsStaNotifyQueue();
    }

    mrsToolsFree(pTempFrame);

    return ret;
}


HI_U32 mrsBroadcastCmdTransmit(HI_IN HI_U16 usId, HI_PVOID pReqPacket, HI_U16 usReqPacketSize)
{
    MRS_STA_ITEM * item = HI_NULL;
    MRS_STA_SRV_CTX_STRU *sta = mrsStaGetContext();
    HI_U32 ret = HI_ERR_SUCCESS;
    MRS_PLC_FRAME_DATA_STRU * pstReq;
    MRS_BROADCAST_STRU * pstTransmit = 0;
    HI_U16 payloadlen = 0;
    HI_U8 *payload = HI_NULL;

    pstReq = (MRS_PLC_FRAME_DATA_STRU*)pReqPacket;
    MRS_NOT_USED(usReqPacketSize);

    //检查参数
    if (!pstReq || !pstReq->pucPayload)
    {
        return HI_ERR_FAILURE;
    }

    if (pstReq->usPayloadLen <= sizeof(MRS_BROADCAST_STRU))
    {
        return HI_ERR_FAILURE;
    }

    pstTransmit = (MRS_BROADCAST_STRU *)pstReq->pucPayload;

    //获取真正的转发数据,
    payloadlen = pstTransmit->data_len;
    payload = pstReq->pucPayload + pstTransmit->stru_len;

    //将数据发送到队列以后处理
    item = mrsToolsMalloc(sizeof(MRS_STA_ITEM) + payloadlen);
    if (item == HI_NULL)
    {
        return HI_ERR_FAILURE;
    }
    (hi_void)memset_s(item,sizeof(MRS_STA_ITEM) + payloadlen, 0,sizeof(MRS_STA_ITEM) + payloadlen);

    item->buf = (HI_U8 *)item + sizeof(MRS_STA_ITEM);   // 偏移数据缓冲区指针
    item->len = payloadlen; // 获取数据长度
    item->from = MRS_STA_QUEUE_FROM_PLC;
    item->id = usId;
    item->seq = pstTransmit->seq;

    (hi_void)memcpy_s(item->buf, item->len, payload, payloadlen);     // 报文内容

    //尝试加入队列
    if(!mrsStaTryEnQueue(&sta->stQueModule,item))
    {
        mrsToolsFree(item);
    }

    //通知队列
    mrsStaNotifyQueue();

    return ret;
}



HI_U32 mrsTestCmdTransmit(HI_IN HI_U16 usId, HI_PVOID pReqPacket, HI_U16 usReqPacketSize)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    MRS_STA_ITEM * item = HI_NULL;
    MRS_STA_SRV_CTX_STRU *sta = mrsStaGetContext();
    MRS_PLC_FRAME_DATA_STRU *pstReq = (MRS_PLC_FRAME_DATA_STRU*)pReqPacket;
    MRS_TEST_TRANSMIT_STRU *pstTransmit = HI_NULL;
    HI_U16 usPayloadlen = 0;
    HI_U8 *pucPayload = HI_NULL;

    MRS_NOT_USED(usReqPacketSize);

    if (pstReq->usPayloadLen <= sizeof(MRS_TEST_TRANSMIT_STRU))
    {
        return HI_ERR_FAILURE;
    }

    pstTransmit = (MRS_TEST_TRANSMIT_STRU *)((HI_U8 *)pstReq->pucPayload);
    usPayloadlen = pstTransmit->data_len;
    pucPayload = pstReq->pucPayload + pstTransmit->stru_len;

    if (0 == usPayloadlen)
    {
        ret = HI_ERR_FAILURE;
        return ret;
    }


    item = (MRS_STA_ITEM *)mrsToolsMalloc(sizeof(MRS_STA_ITEM) + usPayloadlen);
    if (!item)
    {
        ret = HI_ERR_NOT_ENOUGH_MEMORY;
        return ret;
    }

    (hi_void)memset_s(item, sizeof(MRS_STA_ITEM) + usPayloadlen, 0, sizeof(MRS_STA_ITEM) + usPayloadlen);

    item->buf = (HI_U8 *)item + sizeof(MRS_STA_ITEM);
    item->len = usPayloadlen;
    item->from = MRS_STA_QUEUE_FROM_PLC;
    item->id = usId;
    item->time_out = (HI_U8)pstTransmit->timeout;

    (hi_void)memcpy_s(item->buf, usPayloadlen, pucPayload, usPayloadlen);

    if (!mrsStaTryEnQueue(&sta->stQueModule, item))
    {
        mrsToolsFree(item);
    }

    //通知队列
    mrsStaNotifyQueue();

    return ret;
}


// 数据转发命令分发
HI_U32 mrsCmdStaDataTransmit(HI_IN HI_U16 usId, HI_PVOID pReqPacket, HI_U16 usReqPacketSize)
{
    HI_U32 ret = HI_ERR_SUCCESS;

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

    return ret;
}


HI_U32 mrsCmdStaSearchMeter(HI_IN HI_U16 usId, HI_PVOID pReqPacket, HI_U16 usReqPacketSize)
{
    HI_U32 ret = HI_ERR_SUCCESS;

    switch (usId)
    {
        case ID_MRS_CMD_GET_METERADDR_PLC:
        case ID_MRS_CMD_GET_DEVICE_INF_PLC:
            ret = mrsCmdStaGetMeterAddr(usId, pReqPacket, usReqPacketSize);
            break;

        case ID_MRS_CMD_GET_METERLIST:
            mrsStaBindNetwork();
            ret = mrsCmdStaGetMeterList(usId, pReqPacket, usReqPacketSize);
            break;

        case ID_MRS_CMD_START_SEARCH_METER:
            if (mrsIsRealClti() || mrsToolsIsIICollector())
            {
                ret = mrsCmdStaStartSearchMeter(usId, pReqPacket, usReqPacketSize);
            }
            break;

        case ID_MRS_CMD_STOP_SEARCH_METER:
            if (mrsIsRealClti() || mrsToolsIsIICollector())
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
// 函数名称: mrsCmdStaGetMeterAddr
// 功能描述: 发送电表地址和MAC地址映射表到CCO。这一个命令注册函数。当收到CCO
//           的获取表地址命令，此函数会被调用，并将电表址和MAC映射表上传。
//
// 参数说明:
//   HI_U16 usId        命令ID
//   HI_PVOID pstReq    命令结构，可以忽略
//   HI_U16 usReqPacketSize 命令结构大小
// 返 回 值:
//    成功返回  HI_ERR_SUCCESS
//
// 调用要求:
// 调用举例:
// 作    者: niesongsong/nkf62735 [2012-05-29]
// 修    改: cuiate/c00233580 [2013-08-16]
//*****************************************************************************
/* BEGIN: PN: DTS2016021906890 MODIFY\ADD\DEL by cuiate/00233580 at 2016/2/20 */
HI_U32 mrsCmdStaGetMeterAddr(HI_U16 usId, HI_PVOID pstReq, HI_U16 usReqPacketSize)
{
    MRS_PLC_FRAME_DATA_STRU * pstPlc = (MRS_PLC_FRAME_DATA_STRU *)pstReq;
    MRS_GET_DEVICE_INF_DL_STRU * pstFrame = HI_NULL;
    HI_U8 *pucSrcMac = HI_NULL, *pucDstMac = HI_NULL;

    if (!pstReq)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    MRS_NOT_USED(usReqPacketSize);

    switch (usId)
    {
        case ID_MRS_CMD_GET_METERADDR_PLC:
            mrsDfxMapFrameValid();
            return mrsStaCheckReport2Cco(MRS_STA_REPORT_METER_ADDR_RESPOND);

        case ID_MRS_CMD_GET_DEVICE_INF_PLC:
        default:
            break;
    }

    pstFrame = (MRS_GET_DEVICE_INF_DL_STRU *)pstPlc->pucPayload;
    if (pstFrame->interface_ver >= MRS_GET_DEVICE_INF_DL_VER_R2)
    {
        pucSrcMac = pstFrame->dst;
        pucDstMac = pstFrame->src;

        if (!mrsSrvVerifyMac(pstFrame->dst))
        {
            mrsDfxMapFrameInvalid();
            return HI_ERR_FAILURE;
        }
    }

    mrsDfxMapFrameValid();

    return mrsStaReportDeviceInf(pucSrcMac, pucDstMac);
}
/* END:   PN: DTS2016021906890 MODIFY\ADD\DEL by cuiate/00233580 at 2016/2/20 */


//*****************************************************************************
// 函数名称: mrsCmdStaGetMeterList
// 功能描述: 查询采集器搜表的结果
//
// 参数说明:
//   HI_U16 usId        命令ID
//   HI_PVOID pstReq    命令结构
//   HI_U16 usReqPacketSize 命令结构大小
// 返 回 值:
//    成功返回  HI_ERR_SUCCESS
//
// 调用要求:
// 调用举例:
// 作    者: niesongsong/nkf62735 [2012-05-29]
// 修    改: cuiate/c00233580 [2013-08-16]
//*****************************************************************************
HI_U32 mrsCmdStaGetMeterList(HI_U16 usId,HI_PVOID pstReq,HI_U16 usReqPacketSize)
{
/* BEGIN: PN: DTS2016021906890 MODIFY\ADD\DEL by cuiate/00233580 at 2016/2/20 */
    MRS_PLC_FRAME_DATA_STRU * plc = (MRS_PLC_FRAME_DATA_STRU *)pstReq;
    MRS_PLC_METERLIST_DL_S * frame = HI_NULL;
    HI_U8 *pucSrcMac = HI_NULL, *pucDstMac = HI_NULL;

    HI_ASSERT(ID_MRS_CMD_GET_METERLIST == usId);

    if (!pstReq)
    {
        return HI_ERR_FAILURE;
    }

    MRS_NOT_USED(usReqPacketSize);

    frame = (MRS_PLC_METERLIST_DL_S *)plc->pucPayload;

    // VER2及以上版本，判断是否是发给本站点的报文
    if (frame->interface_ver >= MRS_GET_METERLIST_DL_VER_R2)
    {
        pucSrcMac = frame->dst;
        pucDstMac = frame->src;

        if (!mrsSrvVerifyMac(frame->dst))
        {
            mrsDfxSmFrameInvalid();
            return HI_ERR_FAILURE;
        }
    }

    mrsDfxSmFrameValid();

    if (mrsIsRealClti() || mrsToolsIsIICollector())
    {
        MRS_STA_SEARCH_CTX * pstSearchCtx = mrsStaSearchCtx();
/* END:   PN: DTS2016021906890 MODIFY\ADD\DEL by cuiate/00233580 at 2016/2/20 */

        mrsDfxCltSmPlcQueryRx(frame->seq);

        switch (mrsStaSearchGetStatus())
        {
            case MRS_SEARCH_STATUS_IN_PROGRESS:
                pstSearchCtx->stSearchCfg.ulSeq = frame->seq;

                if (frame->force_resp)
                {
                    MRS_PLC_FRAME_DATA_STRU plc_frame = {0};
                    MRS_PLC_METERLIST_UP_S report = {0};

                    (hi_void)memset_s(&report, sizeof(report), 0, sizeof(report));
                    report.interface_ver = MRS_GET_METERLIST_UP_VER;
                    report.stru_len = sizeof(report);
                    report.status = 1;
                    report.option = MRS_SM_SEARCH_RESULT;
                    report.meter_num = 0;
                    report.type = MRS_STA_TYPE_COLLECTOR;
                    report.seq = frame->seq;
                    if (mrsToolsIsIICollector())
                    {
                        report.abnormal = mrsDfxCltiiGetSmAbnormal();
                    }

/* BEGIN: PN: DTS2016021906890 MODIFY\ADD\DEL by cuiate/00233580 at 2016/2/20 */
                    if (pucSrcMac && pucDstMac)
                    {
                        (hi_void)memcpy_s(report.src, HI_PLC_MAC_ADDR_LEN, pucSrcMac, HI_PLC_MAC_ADDR_LEN);
                        (hi_void)memcpy_s(report.dst, HI_PLC_MAC_ADDR_LEN, pucDstMac, HI_PLC_MAC_ADDR_LEN);
                    }
                    else
                    {
                        (hi_void)memcpy_s(report.src, HI_PLC_MAC_ADDR_LEN, MRS_BROADCAST_MAC, HI_PLC_MAC_ADDR_LEN);
                        (hi_void)memcpy_s(report.dst, HI_PLC_MAC_ADDR_LEN, MRS_BROADCAST_MAC, HI_PLC_MAC_ADDR_LEN);
                    }
/* END:   PN: DTS2016021906890 MODIFY\ADD\DEL by cuiate/00233580 at 2016/2/20 */

                    {
                        HI_NV_FTM_COLLECT_PARAM_FILE_S nv;
                        if (HI_ERR_SUCCESS == HI_MDM_NV_Read(HI_NV_FTM_COLLECT_PARAM_FILE, &nv, sizeof(nv)))
                        {
                            (hi_void)memcpy_s(report.asset, HI_METER_ADDR_LEN, nv.CLTAddress, HI_METER_ADDR_LEN);
                        }
                        else
                        {
                            (hi_void)memset_s(report.asset, HI_METER_ADDR_LEN, 0xBB, HI_METER_ADDR_LEN);
                        }
                    }

                    HI_MDM_GetLocalMacAddr(report.id);
                    mrsHexInvert(report.id, HI_PLC_MAC_ADDR_LEN);

                    (hi_void)memset_s(&plc_frame, sizeof(plc_frame), 0, sizeof(plc_frame));
                    plc_frame.usId = ID_MRS_CMD_GET_METERLIST;
                    plc_frame.pucPayload = (HI_U8 *)&report;
                    plc_frame.usPayloadLen = sizeof(report);

                    MRS_SendPlcFrame(&plc_frame);
                    mrsDfxCltSmPlcQueryTx();
                }

                return HI_ERR_SUCCESS;

            default:
                if (frame->seq != pstSearchCtx->stSearchCfg.ulSeq)
                {
                    pstSearchCtx->stSearchCfg.ulSeq = frame->seq;
                    mrsDfxCltSmPlcQueryRx(frame->seq);

                    return mrsStaSearchStart(HI_FALSE);
                }
                break;
        }

/* BEGIN: PN: DTS2016021906890 MODIFY\ADD\DEL by cuiate/00233580 at 2016/2/20 */
        return mrsCltReportSmResult2CCO(pucSrcMac, pucDstMac);
/* END:   PN: DTS2016021906890 MODIFY\ADD\DEL by cuiate/00233580 at 2016/2/20 */
    }

/* BEGIN: PN: DTS2016021906890 MODIFY\ADD\DEL by cuiate/00233580 at 2016/2/20 */
    return mrsStaReportSmResult2CCO(pucSrcMac, pucDstMac);
/* END:   PN: DTS2016021906890 MODIFY\ADD\DEL by cuiate/00233580 at 2016/2/20 */
}
#endif


// 心跳超时统计
HI_VOID mrsStaHeartTimeoutCnt(HI_U16 id)
{
    if(id == MRS_STA_ITEM_ID_HEART)
    {
        mrsDfxHeartTimeoutCnt();    // 心跳超时统计
    }

    return;
}

//*****************************************************************************
// 函数名称: mrsMsgStaTimer
// 功能描述: 响应定时器的消息。这个函数在初始化的时候被注册，当开启的定时器触发
//           将会发送消息给MRS队列，并用调用此函数处理。
//
// 参数说明:
//   HI_U16 usMsgId             命令ID，此ID为注册的
//   HI_IN HI_PVOID pMsgParam   定时器内容，包函数定时器ID等信息。
// 返 回 值:
//    成功返回  HI_ERR_SUCCESS
//
// 调用要求:
// 调用举例:
// 作    者: niesongsong/nkf62735 [2012-05-29]
//*****************************************************************************
HI_U32 mrsMsgStaTimer(HI_U16 usMsgId, HI_IN HI_PVOID pMsgParam)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    MRS_MSG_MR_TIMER_STRU* pst = HI_NULL;
    MRS_STA_SRV_CTX_STRU *sta = mrsStaGetContext();

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
        if (!EQUIP_DUT_IS_EQUIP_MODE())//如果不是测试模式
        {
            if (!mrsToolsIsIICollector())
            {
                ret = mrsStaHeart();
            }
        }
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

#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
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
                    break;
                }
            }

            mrsDfxRefreshRmQueueTopStatus(pstQueue, pstModule->lock); // 读表队列状态刷新
            mrsStaQueueUnLock(pstModule);
            mrsSetFrameFlg(HI_TRUE);
            mrsDfxRefreshRmQueueTopStatus(pstQueue, pstModule->lock); // 读表队列状态刷新
        }
        // no break;

    case MRS_STA_TIMER_QUEUE:
        mrsStaQueueProc(&sta->stQueModule);
        mrsDfxRefreshRmQueueTopStatus(&sta->stQueModule.stMrQueue,sta->stQueModule.lock); // 读表队列状态刷新
        mrsSetFrameFlg(HI_FALSE);
        break;
    case MRS_STA_TIMER_IO_EVENT_OUT://IO控制模块EVENT_OUT超时处理函数
        mrsEventTimeOutProc();//EVENT_OUT管脚查看状态
        break;

    case MRS_STA_TIMER_RUN_LED:
        mrsRunLedPro(HI_FALSE);
        break;

    case MRS_COLLECTOR_TIMER_IRDA_RESET:
        HI_MDM_Reboot(HI_SYS_REBOOT_CAUSE_MRS1); //执行硬件初始化
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
            /* BEGIN: Added by liuxipeng/lkf54842 2014/01/15 问题单号: DTS2013121904065 */
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
        if (mrsIsRealClti() || mrsToolsIsIICollector())
        {
            mrsStaSearchStart(HI_FALSE);
        }
        break;

#if defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG) && defined(MRS_LOG_STA_SWITCH)
    case MRS_STA_TIMER_DAYFREEZE_REC:  // 周期性记录日冻结统计定时器
        mrsLogStaDayfreezeRecTimerProc();
        break;
#endif

#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
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
        mrsZeroCrossCheckProc(HI_ERR_TIMEOUT, HI_NULL);
        break;

#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
    case MRS_STA_TIMER_CLTII_EVENT:
        mrsCltiiEventStartMonitor();
        break;

    case MRS_STA_TIMER_CLTII_LOCAL_RM_INTERVAL:
        mrsCltiiEventMonitorSendMessage(MRS_CLTII_EVENT_MSG_NEXT_ITEM);
        break;

    case MRS_STA_TIMER_RM_METER_JOIN_NET:
        mrsCltRmMeterSetMacTimeout();
        break;
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
    case MRS_STA_TIMER_CSG_CLTI_SM_QUERY:
        mrsCsgCltISearchQuery();
        break;

    case MRS_STA_TIMER_CSG_CLTI_QUEUE:
        mrsCsgCltIFrameTimeout();
        break;

    case MRS_STA_TIMER_CSG_QRY_DETECT_STATUS:
        mrsCsgCltIQueryDetectStatus();
        break;

    case MRS_STA_TIMER_SET_CLT_MAC:
        mrsSetMacTimeout();
        break;

    case MRS_STA_TIMER_CSG_CLTI_UPG:
        mrsCsgCltIUpgTimeout();
        break;

    case MRS_STA_TIMER_MODE_PHYSICAL_TEST:
        mrsPhyTestTimeoutProc();
        break;

    case MRS_STA_TIMER_ENTER_TESTMODE:
        mrsEnterTestModeProc();
        break;

/* BEGIN: Modified by cuichunyan/00276962, 2017/1/17   reboot  */
    case MRS_STA_TIMER_CSG_REBOOT:
        ret = HI_MDM_Reboot(HI_SYS_REBOOT_CAUSE_MRS0);
        break;
/* END: Modified by cuichunyan/00276962, 2017/1/17  */
    //写case处理定时器响应
    case MRS_STA_TIMER_POWERFAILURE:
        ret = mrsCreatePowerFailureFrame();
        break;
#endif

    case MRS_STA_TIMER_RM_QUEUE_LOCK:
        ret = HI_MDM_Reboot(HI_SYS_REBOOT_CAUSE_MRS2);
        break;
    case MRS_STA_TIMER_NETWORK_LOCK_CHECK:
        mrsNetworkCheckTimeout();
        break;

    case MRS_STA_TIMER_REJOIN_AFTER_UNLOCK:
        mrsRejoinNetworkAfterUnlock();
        break;

    default:
        ret = HI_ERR_FAILURE;
        break;
    }

    return ret;
}

//*****************************************************************************
// 函数名称: mrsStaReport2CCO
// 功能描述: 上报模块和电表地址映射表给CCO
//
// 参数说明:
//
// 返 回 值:
//    成功返回  HI_ERR_SUCCESS
//
// 调用要求:
// 调用举例:
// 作    者: niesongsong/nkf62735 [2012-05-29]
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
/* BEGIN: PN: DTS2016021906890 MODIFY\ADD\DEL by cuiate/00233580 at 2016/2/20 */
    report->interface_ver = MRS_GET_DEVICE_INF_UP_VER_R1;
/* END:   PN: DTS2016021906890 MODIFY\ADD\DEL by cuiate/00233580 at 2016/2/20 */
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


#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
/* BEGIN: PN: DTS2016021906890 MODIFY\ADD\DEL by cuiate/00233580 at 2016/2/20 */
HI_U32 mrsCltReportSmResult2CCO(HI_U8 *pucSrcMac, HI_U8 *pucDstMac)
/* END:   PN: DTS2016021906890 MODIFY\ADD\DEL by cuiate/00233580 at 2016/2/20 */
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
    report->meter_num = pstMeterList->ucMeterNum;
    report->type = MRS_STA_TYPE_COLLECTOR;
    report->seq = pstSmCtx->stSearchCfg.ulSeq;

    if (mrsToolsIsIICollector())
    {
        report->abnormal = mrsDfxCltiiGetSmAbnormal();
    }

    {
        HI_NV_FTM_COLLECT_PARAM_FILE_S nv;
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

    item = (MRS_CMD_METERLIST_ITEM *)report->meter_list;
    for (i = 0; i < pstMeterList->ucMeterNum; i++)
    {
        (hi_void)memcpy_s(item->addr, HI_METER_ADDR_LEN, pstMeterList->astMeter[i].ucMeterAddr, HI_METER_ADDR_LEN);
        item->protocol = pstMeterList->astMeter[i].ucProtocol;
        item->type = MRS_STA_TYPE_METER;

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


/* BEGIN: PN: DTS2016021906890 MODIFY\ADD\DEL by cuiate/00233580 at 2016/2/20 */
HI_U32 mrsStaReportSmResult2CCO(HI_U8 *pucSrcMac, HI_U8 *pucDstMac)
/* END:   PN: DTS2016021906890 MODIFY\ADD\DEL by cuiate/00233580 at 2016/2/20 */
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
    report->meter_num = pstMeterList->ucMeterNum;
    report->type = pstMeterList->ucType;
    report->seq = pstSmCtx->stSearchCfg.ulSeq;

    HI_MDM_NM_GetAddr(report->asset, ucIPAddr);
    mrsHexInvert(report->asset, sizeof(report->asset));
    MRS_TOOLS_FE_TO_00(report->asset[0]);

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
        meter_inf->type = MRS_STA_TYPE_METER;
        meter_inf++;
        num++;

        if (num == 1)
        {
            (hi_void)memcpy_s(report->asset, HI_METER_ADDR_LEN, meter_inf->addr, HI_METER_ADDR_LEN);
        }
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
#endif


//*****************************************************************************
// 函数名称: mrsStaCheckReport2Cco
// 功能描述: 检查是否需要将表地址上报到CCO
//           如果符合条件则调用mrsStaReport2CCO函数进行上报
//           如果不符合条件则返回
//
// 参数说明:
//   report_status  上报是主动还是被动响应
// 返 回 值:
//    成功返回  HI_ERR_SUCCESS
//
// 调用要求:
// 调用举例:
// 作    者: liuxipeng/lkf54842 [2013-03-05]
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

    usDataLen = sizeof(MRS_DATA_TRANSMIT_UP_STRU) + len;
    pstData = (MRS_DATA_TRANSMIT_UP_STRU *)mrsToolsMalloc(usDataLen);
    if (!pstData)
    {
        return HI_ERR_NO_MORE_MEMORY;
    }

    (hi_void)memset_s(frame_data, sizeof(MRS_PLC_FRAME_DATA_STRU), 0, sizeof(MRS_PLC_FRAME_DATA_STRU));
    (hi_void)memset_s(pstData, usDataLen, 0, usDataLen);

    if(item->id == ID_MRS_CMD_DATA_TRANSMIT_AFN13_PLC)
    {
        HI_PRV HI_U8 need_bc_cnt = 0;

        // 本次必须为下行广播
        if (item->bcFlg)
        {
            // 每隔一个下行广播才进行上行广播
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
        mrsToolsFree(pstData);
        return HI_ERR_FAILURE;
    }

    pstData->interface_ver = ucVer;
    pstData->stru_size     = sizeof(MRS_DATA_TRANSMIT_UP_STRU);
    pstData->protocol      = protocol;
    pstData->data_len      = len;
    pstData->seq           = item->seq;
    pstData->dir           = MRS_PLC_UP_FLG;

    // 复制数据
    (hi_void)memcpy_s(pstData->data, len, data, len);

    frame_data->usId         = item->id;    // 获取业务ID
    frame_data->usPayloadLen = usDataLen;
    frame_data->pucPayload   = (HI_PBYTE)pstData;

#if defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG) && defined(MRS_LOG_STA_SWITCH)
    mrsLogFrameInfStatRx(pstData->data, pstData->data_len, item->buf, item->len, 1);
#endif

    return HI_ERR_SUCCESS;
}

//*****************************************************************************
// 函数名称: mrsStaTransmit2CCO
// 功能描述: 转发PLC帧，只适用于ID_MRS_CMD_DATA_TRANSMIT_PLC。
//
// 参数说明:
//   HI_U8 * data    数据内容
//   HI_U16 len      数据长度
//   HI_U8  protocol 645帧协议
// 返 回 值:
//    成功返回  HI_ERR_SUCCESS
//
// 调用要求: 此函数会加上会话信息
// 调用举例:
// 作    者: niesongsong/nkf62735 [2012-05-29]
//*****************************************************************************
HI_U32 mrsStaTransmit2CCO(HI_U8 * data, HI_U16 len, HI_U8 protocol, MRS_STA_ITEM * item)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    MRS_PLC_FRAME_DATA_STRU frame_data = { 0 };

    if (!item)
    {
        return HI_ERR_FAILURE;
    }

    //检查参数和规约类型
    if (!data || !mrsCheck645Proto(protocol))
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

/* BEGIN: Modified by fengxiaomin/00209182, 2014/3/27   问题单号:DTS2014032906596 */
    mrsDfxRmPlcUpAfn(item->id, frame_data.pucPayload, ret);
/* END:   Modified by fengxiaomin/00209182, 2014/3/27 */
    mrsToolsFree(frame_data.pucPayload);

    return ret;
}



//*****************************************************************************
// 函数名称: mrsStaGetMacAddr
// 功能描述: 获取模块MAC地址。
//
// 参数说明:
//   HI_U8 macAddr[6]    输出的模块地址
// 返 回 值:
//    成功返回  模块地址指针
//
// 调用要求:
// 调用举例:
// 作    者: niesongsong/nkf62735 [2012-05-29]
//*****************************************************************************
HI_U8 * mrsStaGetMacAddr(HI_U8 macAddr[6])
{
    MRS_STA_SRV_CTX_STRU* pstaCtx = mrsStaGetContext();

    //是否是有效的MAC地址
    if (mrsToolsInvalidMac(pstaCtx->ucMacAddr))
    {
        HI_MDM_GetProductId(pstaCtx->ucMacAddr, HI_NULL, HI_NULL, HI_NULL);
    }

    (hi_void)memcpy_s(macAddr,HI_PLC_MAC_ADDR_LEN,pstaCtx->ucMacAddr,HI_PLC_MAC_ADDR_LEN);
    return pstaCtx->ucMacAddr;
}

//*****************************************************************************
// 函数名称: mrsStaSetMacAddr
// 功能描述: 设置模块MAC地址。
//
// 参数说明:
//   HI_U8 macAddr[6]    输入的模块地址
// 返 回 值:
//    成功返回  模块地址指针
//
// 调用要求:
// 调用举例:
// 作    者: niesongsong/nkf62735 [2012-05-29]
//*****************************************************************************
HI_U8 * mrsStaSetMacAddr(HI_U8 macAddr[6])
{
    MRS_STA_SRV_CTX_STRU* pstaCtx = mrsStaGetContext();

    //模块地址不能是全0与广播地址
    if (mrsToolsInvalidMac(macAddr))
    {
        return HI_NULL;
    }

    if (mrsToolsBroadCastMac(macAddr))
    {
        return HI_NULL;
    }
    (hi_void)memcpy_s(pstaCtx->ucMacAddr,HI_PLC_MAC_ADDR_LEN,macAddr,HI_PLC_MAC_ADDR_LEN);
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

//将报文回复给抄控器
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

//抄控器注册
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

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
    if (mrsToolsIsCLTI() && !mrsStaGetSupportVM())
    {
        mrsStaTransmitCltICmd(pbuf->aucFrame, pbuf->usFrameSize ,MRS_STA_QUEUE_FROM_REMOTE);
        return HI_ERR_CONSUMED;
    }
#endif

    mrsStaTransmitCmd(pbuf->aucFrame, pbuf->usFrameSize ,MRS_STA_QUEUE_FROM_REMOTE);
    return HI_ERR_CONSUMED;
}

//HSO注册(本地-远程)
HI_U32 mrsStaTransmitHsoCmd(HI_U16 ulId, HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption)
{
    DIAG_MRS_TRANSMIT_STRU * buf = HI_NULL;
    HI_U8 from = (ulOption == HI_DIAG_CMD_INSTANCE_IREMOTE) ? MRS_STA_QUEUE_FROM_REMOTE_HSO : MRS_STA_QUEUE_FROM_LOCAL;

    HI_UNREF_PARAM(ulId);

    buf = (DIAG_MRS_TRANSMIT_STRU *)pCmdParam;

    if((buf->ulLen + sizeof(buf->ulLen)) > (usCmdParamSize))
    {
        HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_049, HI_DIAG_MT("len err [%d]-[%d]"), buf->ulLen, usCmdParamSize);
        return HI_ERR_BAD_DATA;
    }

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
    if (mrsToolsIsCLTI() && !mrsStaGetSupportVM())
    {
        mrsStaTransmitCltICmd(buf->data, (HI_U16)buf->ulLen, from);
        return HI_ERR_CONSUMED;
    }
#endif

    mrsStaTransmitCmd(buf->data, (HI_U16)buf->ulLen, from);
    return HI_ERR_CONSUMED;
}

//HSO与抄控器公用数据入队列
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
    (hi_void)memset_s(item, sizeof(MRS_STA_ITEM)+len, 0, sizeof(MRS_STA_ITEM)+len);

    item->from = from;
    item->buf = (HI_U8*)item + sizeof(MRS_STA_ITEM);
    item->len = len;
    item->id = MRS_STA_ITEM_ID_TRANSMIT;
    item->time_out = (HI_U8)MRS_MS_TO_100MS(mrsGet645FrameHeadTimeout(MRS_STA_RM_CFG_AUTO));
    item->bAutoStrategy = HI_TRUE;
    item->ucFrameTimeout = (HI_U8)MRS_MS_TO_100MS(mrsGet645FrameInterval(MRS_STA_RM_CFG_AUTO));

    (hi_void)memcpy_s(item->buf, len, data, len);

    if (!mrsStaTryEnQueue(&sta->stQueModule,item))
    {
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_052, HI_DIAG_MT("en queue"));
        mrsToolsFree(item);
        return HI_ERR_FAILURE;
    }

    //通知队列
    mrsStaNotifyQueue();

    return HI_ERR_SUCCESS;
}


#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
HI_U32 mrsStaTransmitCltICmd(HI_PBYTE pucData, HI_U16 usDataLen, HI_U8 ucFrom)
{
    HI_PBYTE pucPayload = pucData;
    HI_U16 usPayloadLen = usDataLen;
    HI_U16 usPos = 0;
    HI_U16 usLength = 0;
    HI_U32 ulRet = HI_ERR_SUCCESS;

    if (HI_ERR_SUCCESS == mrsFind645Frame(pucData, (HI_S16)usDataLen, &usPos, &usLength))
    {
        MRS_CSG_TRANS_XR_DL_STRU *pstXrFrame = (MRS_CSG_TRANS_XR_DL_STRU *)mrsToolsMalloc(sizeof(MRS_CSG_TRANS_XR_DL_STRU) + usDataLen);
        if (!pstXrFrame)
        {
            return HI_ERR_MALLOC_FAILUE;
        }

        (hi_void)memset_s(pstXrFrame, sizeof(MRS_CSG_TRANS_XR_DL_STRU) + usDataLen, 0, sizeof(MRS_CSG_TRANS_XR_DL_STRU) + usDataLen);
        (hi_void)memcpy_s(pstXrFrame->aucDstAddr, HI_METER_ADDR_LEN, pucData + usPos + MRS_645_FRAME_METERADD_OFFSET, HI_METER_ADDR_LEN);
        (hi_void)memcpy_s(pstXrFrame->aucData, usDataLen, pucData, usDataLen);
        pstXrFrame->usDataLen = usDataLen;

        ulRet = mrsCsgCltIGenTransmitFrame(pstXrFrame, &pucPayload, &usPayloadLen);
        mrsToolsFree(pstXrFrame);
        if (HI_ERR_SUCCESS != ulRet)
        {
            return HI_ERR_FAILURE;
        }
    }

    ulRet = mrsStaTransmitCmd(pucPayload, usPayloadLen, ucFrom);

    if (pucPayload != pucData)
    {
        mrsToolsFree(pucPayload);
    }

    return ulRet;
}
#endif



// 查找表地址列表-找到相同的地址返回HI_TRUE并输出对应的数组下标
HI_BOOL mrsFindMeterList(MRS_METER_LIST_S * pMeterList,
                            HI_U8 addr[HI_METER_ADDR_LEN], HI_U16 * index)
{
    HI_U16 i = 0;
    MRS_METER_ITEM_S * pMeter = HI_NULL;
/* BEGIN: PN: DTS2015042907341 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/13 */
    HI_U8 aucMeterAddr[HI_METER_ADDR_LEN] = {0};
    HI_U8 aucTempAddr[HI_METER_ADDR_LEN] = {0};

    // 地址都按照97表地址处理，把高字节AAH转为00H
    mrsMeterAddrPreProc(aucMeterAddr, addr, MRS_PROTO645_VER_1997);

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

// 根据缓冲区中的报文做特殊处理:
// 13/14和事件查询报文: 在缓冲区中缓存表回复的报文
// 广播报文: 缓存CCO下发的报文
// 比较时根据id和seq比较，如果有相同的报文，则直接返回抄读结果(13/14和事件查询)或不处理(广播报文)
// 返回HI_TRUE:找到重复PLC帧 返回HI_FALSE:未找到重复PLC帧
HI_BOOL mrsStaPlcRetryFiltrate(MRS_STA_ITEM * item)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U8 * buf = HI_NULL;
    HI_U16 len = 0;
    HI_U8 protocol = 0;

    ret = mrsStaFrameBufFind(item->id, item->seq, &len, &buf, &protocol);
    if (HI_ERR_SUCCESS != ret)
    {
        return HI_FALSE;
    }

    switch (item->id)
    {
#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
    case ID_MRS_CMD_DATA_TRANSMIT_AFN13_PLC:
        if (MRS_STA_TRANSFER_TIMING_NORMAL != item->sub_id)
        {
            mrsStaTransmit2CCO(buf, len, protocol, item);
        }
        break;

    case ID_MRS_CMD_DATA_TRANSMIT_AFN14_PLC:
        mrsStaTransmit2CCO(buf, len, protocol, item);
        break;

    case ID_MRS_CMD_BROADCAST_TRANSMIT_PLC:
        break;
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
    case MRS_STA_ITEM_ID_CSG_TRANS_XR:
        if (item->option)
        {
            mrsStaCsgTransmitXr2CCO(buf, len, item);
        }
        break;
#endif

    default:
        return HI_FALSE;
    }

    return HI_TRUE;
}


HI_U32 mrsStaFrameBufInsert(HI_U32 id, HI_U16 seq, HI_U16 frame_len, HI_U8 * frame_data, HI_U8 protocol)
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
    item->id = id;
    item->seq = seq;
    item->valid = 1;
    item->frame_len = frame_len;
    item->protocol = protocol;
    item->timestamp = HI_MDM_GetSeconds();
    (hi_void)memcpy_s(item->frame_buf, frame_len, frame_data, frame_len);

    ctx->current += 1;
    if (ctx->current >= ctx->max_count)
    {
        ctx->current = 0;
    }


    return ret;
}

HI_U32 mrsStaFrameBufFind(HI_U32 id, HI_U16 seq, HI_U16 *frame_len, HI_U8 ** frame_data, HI_U8 * protocol)
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

        if (id == item->id && seq == item->seq)
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


// 获取PLC收发模式
HI_U8 mrsStaGetPlcRxTxMode(HI_VOID)
{
    MRS_STA_SRV_CTX_STRU * sta = mrsStaGetContext();
    return sta->plc_rxtx_mode;
}


#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
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


HI_U32 mrsCmdStaPlcTestTransmit(HI_IN HI_U16 usId, HI_PVOID pReqPacket, HI_U16 usReqPacketSize)
{
    MRS_PLC_FRAME_DATA_STRU *recvPlc = (MRS_PLC_FRAME_DATA_STRU *)pReqPacket;
    MRS_PLC_FRAME_DATA_STRU sendPlc = {0};
    MRS_TEST_TRANSMIT_AFNF0_STRU *recvFrame = HI_NULL;
    MRS_TEST_TRANSMIT_AFNF0_STRU *sendFrame = HI_NULL;

    HI_PBYTE payload = HI_NULL;
    HI_U16 payloadLen = 0;

    HI_ASSERT(ID_MRS_CMD_TEST_TRANSMIT_AFNF0_PLC == usId);

    if (!pReqPacket)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    MRS_NOT_USED(usReqPacketSize);

    recvFrame = (MRS_TEST_TRANSMIT_AFNF0_STRU *)recvPlc->pucPayload;
    payloadLen = sizeof(MRS_TEST_TRANSMIT_AFNF0_STRU) + recvFrame->expect_len;
    payload = mrsToolsMalloc(payloadLen);
    if (!payload)
    {
        return HI_ERR_MALLOC_FAILUE;
    }

    (hi_void)memset_s(payload, payloadLen, 0, payloadLen);
    sendFrame = (MRS_TEST_TRANSMIT_AFNF0_STRU *)payload;
    sendFrame->interface_ver = MRS_TEST_AFNF0_UP_VER;
    sendFrame->stru_len = sizeof(MRS_TEST_TRANSMIT_AFNF0_STRU);
    sendFrame->data_len = recvFrame->data_len;
    sendFrame->expect_len = recvFrame->expect_len;
    (hi_void)memcpy_s(payload + sendFrame->stru_len, recvFrame->expect_len, recvPlc->pucPayload + recvFrame->stru_len,
        MRS_MIN(recvFrame->data_len, recvFrame->expect_len));

    (hi_void)memset_s(&sendPlc, sizeof(sendPlc), 0, sizeof(sendPlc));
    sendPlc.usId = ID_MRS_CMD_TEST_TRANSMIT_AFNF0_PLC;
    sendPlc.pucPayload = payload;
    sendPlc.usPayloadLen = payloadLen;

    MRS_SendPlcFrame(&sendPlc);

    mrsToolsFree(payload);

    return HI_ERR_SUCCESS;
}
#endif


// 刷新首节点信息
// STA:所属电表地址
// 采集器:入网电表地址
HI_VOID mrsStaRefreshTopNodeInf(HI_U8 *pucAddr, HI_U8 *pucProtocol)
{
    MRS_STA_SRV_CTX_STRU * sta  = mrsStaGetContext();
    MRS_METER_ITEM_S * NodeAddr = HI_NULL;

    if(!mrsToolsNormalAddr(pucAddr))
    {
        return;
    }

    NodeAddr = &sta->stMeterList.stNodeMeter;

    (hi_void)memcpy_s(NodeAddr->ucMeterAddr, HI_METER_ADDR_LEN, pucAddr, HI_METER_ADDR_LEN);

    if(pucProtocol)
    {
        NodeAddr->ucProtocol = *pucProtocol;
    }

    NodeAddr->ucValidFlag = HI_TRUE;

    return;
}


// 设置表地址到Mac层并更新产品类型
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
        // 设置成功后更新本地保存的Mac地址
        mrsStaSetMacAddr(ucMacAddr);
    }

    return ret;
}


// 同步Mac层入网地址
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
        // 更新Sta上下文中Mac地址
        mrsStaSetMacAddr(ucCurrMac);

        if(mrsToolsIsMacAddrFromMeter(ucCurrMac))
        {
            // 更新Nv保存表地址
            mrsStaSaveJoinMeterAddr(ucCurrMac, MRS_PROTO645_VER_2007, 0);

            if (mrsToolsIsIICollector())
            {
                mrsSetCollectDevAddr(HI_NV_FTM_COLLECT_PARAM_FILE);
                return HI_ERR_SUCCESS;
            }

            // 更新"DevId"地址
            MRS_TOOLS_FE_TO_00(ucCurrMac[MRS_MAC_ADDR_INDEX_5]);
            HI_MDM_DevIdConfig(ucCurrMac);
        }
    }

    return HI_ERR_SUCCESS;
}

// 在搜表结果列表中获取指定表地址信息
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

// 存储入网表地址到Nv项
HI_VOID mrsStaSaveJoinMeterAddr(HI_U8 *pucMac, HI_U8 ucProtocol, HI_U16 usDi97)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    MRS_STA_SEARCH_CTX *pstSmCtx = mrsStaSearchCtx();
    MRS_METER_ITEM_S *pstMeterInf = HI_NULL;
    HI_NV_FTM_COLLECT_PARAM_FILE_S nv;
    HI_U8 aucTempMeter[HI_METER_ADDR_LEN] = { 0 };
    HI_U16 usBaudRate = MRS_SRV_BAUD_RATE_DEFAULT;
    HI_U8 ucIdx = MRS_SRV_BAUD_RATE_IDX_DEFAULT;

    if (!mrsIsRealClti() && !mrsToolsIsIICollector())
    {
        return;
    }

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
        MRS_SRV_BAUDRATE_ID_E eId = (MRS_PROTO645_VER_1997 == ucProtocol) ? MRS_SRV_ID_BAUDRATE_SM_97 : MRS_SRV_ID_BAUDRATE_SM_07;
        P_MRS_SRV_BAUDRATE_CFG_S pstCfg = mrsSrvGetBaudRateCfg(eId);

        (hi_void)memcpy_s(aucTempMeter, HI_METER_ADDR_LEN, pucMac, HI_METER_ADDR_LEN);
        MRS_TOOLS_FE_TO_00(aucTempMeter[MRS_MAC_ADDR_INDEX_5]);
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

    if (mrsIsRealClti())
    {
        mrsHexInvert(aucTempMeter, HI_METER_ADDR_LEN);
        HI_MDM_DevIdConfig(aucTempMeter);
    }
}

// 设置STA产品类型(普通电表模块:I型采集器模块)
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

// 获取STA产品类型
HI_U8 mrsStaGetType(HI_VOID)
{
    if(mrsToolsIsSTA() || mrsToolsIsTSTA())
    {
        return mrsStaGetContext()->sta_type;
    }

    return STA_TYPE_INVALID;
}

// 获取STA端PLC通道状态
HI_BOOL mrsStaGetPlcChlStatus(HI_VOID)
{
    return mrsStaGetContext()->plcState;
}

// 是否允许锁定网络
HI_BOOL mrsStaBindNetworkEnable(HI_VOID)
{
    MRS_STA_SRV_CTX_STRU *pstStaCtx = mrsStaGetContext();

    if (pstStaCtx && SOFT_TF_ON_WITH_STA_JOIN_BY_TF_RESULT == pstStaCtx->ucSoftTfMode)
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
    HI_U32 ulRet = HI_ERR_FAILURE;

    if (mrsStaBindNetworkEnable() == HI_TRUE)
    {
        HI_U32 ulDuration = (0xFFFF == pstStaCtx->usBindDuration) ? 0xFFFFFFFF : ((HI_U32)pstStaCtx->usBindDuration * 60);

        ulRet = HI_MDM_LockNetwork(ulDuration);
        if (HI_ERR_SUCCESS != ulRet)
        {
            HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_005, HI_DIAG_MT("发送锁定消息失败 [ret] [duration]"), ulRet, ulDuration);
        }
    }

    return ulRet;
}

/**
* @brief 锁定离网时间门限检查，达到门限就离网解锁。
*
* @par Description
*     若锁定时长大于门限，且在此锁定时段内，离网时间占比大于门限时，
* 将锁定网络设置为最后尝试的网络，然后解锁，然后离网。
*
*/
HI_U32 mrsNetworkCheckTimeout(HI_VOID)
{
    HI_MDM_LOCK_INFO_STRU stLockInfo = {0};
    HI_U32 ulRet = HI_ERR_SUCCESS;

    HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_035, HI_DIAG_MT("判断离网时间门限"));

    // 查询锁定信息
    ulRet = HI_MDM_GetLockInfo(&stLockInfo);
    if (HI_ERR_SUCCESS != ulRet)
    {
        HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_034, HI_DIAG_MT("mrsNetworkCheckTimeout: 查询锁定信息失败 [ret]"), ulRet);
        return ulRet;
    }

    if (HI_MDM_UNLOCK_NETWORK_STATUS == stLockInfo.lock_status)
    {
        return HI_ERR_SUCCESS;
    }

    if (mrsCheckReachLeaveThInLockNet(&stLockInfo))
    {
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_032, HI_DIAG_MT("达到离网时长门限"));

        // 发送解锁消息
        HI_MDM_UnlockNetwork();

        // 等待锁定解除后，重新入网
        MRS_StartTimer(MRS_STA_TIMER_REJOIN_AFTER_UNLOCK, MRS_STA_TIME_REJOIN_AFTER_UNLOCK, HI_SYS_TIMER_ONESHOT);
    }

    return HI_ERR_SUCCESS;
}

/**
* @brief 判断是否达到锁定离网时间门限
*
* @par Description:
*     判断标准: 锁定时长达到门限，并且，在此锁定时段内，离网时长达到门限
*
*/
HI_BOOL mrsCheckReachLeaveThInLockNet(HI_MDM_LOCK_INFO_STRU * pstLockInfo)
{
    MRS_STA_SRV_CTX_STRU *pstStaCtx = mrsStaGetContext();

    if (!pstLockInfo || !pstStaCtx || HI_MDM_UNLOCK_NETWORK_STATUS == pstLockInfo->lock_status)
    {
        return HI_FALSE;
    }

    HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_036, HI_DIAG_MT("当前离网时长统计状态: [leave_time_len] [usLeaveTimeLenTh]"),
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
* @brief 解锁成功后，发送消息重新入网
*
* @retval HI_ERR_SUCCESS 函数执行成功(可能继续在等待，也可能成功发送重新入网消息)。
* @retval HI_ERR_FAILURE 发送重新入网消息失败，站点未入网或者正在离网。
* @retval HI_ERR_INVALID_PARAMETER 锁定信息查询时，传参错误。理论不会出现，出现则程序有BUG。
*/
HI_U32 mrsRejoinNetworkAfterUnlock(HI_VOID)
{
    HI_MDM_LOCK_INFO_STRU stLockInfo = {0};
    HI_U32 ulRet = HI_ERR_SUCCESS;

    // 查询锁定信息
    ulRet = HI_MDM_GetLockInfo(&stLockInfo);
    if (HI_ERR_SUCCESS != ulRet)
    {
        return HI_ERR_FAILURE;
    }

    // 解锁成功，软重启
    if (HI_MDM_UNLOCK_NETWORK_STATUS == stLockInfo.lock_status)
    {
        // 发送消息重新入网
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


#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
/* BEGIN: PN: DTS2016021906890 MODIFY\ADD\DEL by cuiate/00233580 at 2016/2/20 */
HI_U32 mrsStaReportDeviceInf(HI_U8 *pucSrcMac, HI_U8 *pucDstMac)
/* END:   PN: DTS2016021906890 MODIFY\ADD\DEL by cuiate/00233580 at 2016/2/20 */
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

/* BEGIN: PN: DTS2016021906890 MODIFY\ADD\DEL by cuiate/00233580 at 2016/2/20 */
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
/* END:   PN: DTS2016021906890 MODIFY\ADD\DEL by cuiate/00233580 at 2016/2/20 */

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
#endif


#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
HI_U32 mrsCmdStaCsgDataTransmit(HI_U16 usId, HI_PVOID pReqPacket, HI_U16 usPacketSize)
{
    HI_UNREF_PARAM(usId);

    return mrsCsgPlcFrameProc(pReqPacket, usPacketSize);
}
#endif


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


#define MRS_IS_NEED_CHECK_DI(x) \
    ( (MRS_645_FRAME_CONTROL_READ_DATA == (x)) \
        || (MRS_645_FRAME_CONTROL_READ_FOLLOW == (x)) \
        || (MRS_645_FRAME_CONTROL_READ_DATA97 == (x)) \
        || (MRS_645_FRAME_CONTROL_READ_FOLLOW97 == (x)) \
        || (MRS_645_FRAME_CONTROL_SAFE == (x)))

// Compare Dl & Ul frame, check wheher Ul frame match DL frame or not.
HI_BOOL mrsStaIsMr645FrameMatch(HI_U8 * pDnFrame, HI_U16 usDnLength, HI_U8 * pUpFrame, HI_U16 usUpLength)
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

    bIsMeter97 = (HI_BOOL)(MRS_PROTO645_VER_1997 == mrsToolsGet645Ver(stUpCtrl.ucFn));

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


/**
 @brief     Add meter to local meterlist.  CNcomment: 添加电表到本地列表。CNend
 @param[in] pstItem  meter information pointer.  CNcomment: 电表信息指针。CNend
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
            pstMeter->ucProtocol = pstItem->ucProtocol;
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

#endif


