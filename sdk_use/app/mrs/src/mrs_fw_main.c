//*****************************************************************************
//
//                  版权所有 (C), 1998-2011, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_fw_main.c
//  版 本 号   : V1.0
//  作    者   : shenhankun/00130424
//  生成日期   : 2011-10-25
//  功能描述   : MRS基础框架实现
//
//  函数列表   :
//  修改历史   :
//  1.日    期 : 2011-10-25
//    作    者 : shenhankun/00130424
//    修改内容 : 创建文件
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
// MRS 框架上下文定义
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

    // 基础框架初始化
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

    // 队列初始化
    ulRet = HI_MDM_MSG_CreateQueue(&(pstFwCtx->stResCfg.aulQueueId[EN_APP_QUEUE_MRS]), "MRS_Q", PRODUCT_CFG_Q_APP_MRS_MSG_NUM);
    if (HI_ERR_SUCCESS == ulRet)
    {
        HI_MDM_ReportInitEvt(HI_DEID_APP_QUEUE_INIT_OK, HI_DIAG_SYS_INIT_SET);
    }
    else
    {
        HI_MDM_ReportInitEvt(HI_DEID_APP_QUEUE_INIT_FAIL, HI_DIAG_SYS_INIT_SET);
    }


    // 任务初始化
    APP_MrsTaskInit();

    // 信号量初始化
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

    // 创建任务
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
// 函数名称: APP_MrsTaskInit
// 功能描述: MRS任务初始化
//
// 参数说明:
//  This function has no arguments.
//
// 返 回 值:
//    无
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: shenhankun/00130424 [2011-11-08]
//*****************************************************************************
HI_VOID APP_MrsTaskInit(HI_VOID)
{
    HI_U8 buf_index = 0;

#if (defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) || defined(PRODUCT_CFG_PRODUCT_TYPE_NDM))
    // 376.2协议 初始化
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
        // 645协议 初始化
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
    // 通道改变通知处理初始化
    mrsSrvChlNotifyProcInit();

    mrsStaGlobalInit();
    mrsNvInitBaudRateCfg(ID_NV_APP_BAUDRATE_CFG);
#endif

    // MR服务启动
    mrsSrvStart();

#if (defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) || defined(PRODUCT_CFG_PRODUCT_TYPE_STA))
    mrsInitIOCtx(); //IO控制模块初始化
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
// 函数名称: mrsSetPlcForceMode
// 功能描述: MRS强制PLC通道可用模式处理函数
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
/* BEGIN: Added by fengxiaomin/00209182, 2014/1/14   问题单号号:DTS2014012607684  */
HI_VOID mrsSetPlcForceMode(HI_VOID)
{
    // 强制PLC接收
    HI_MDM_ChlCtrl(HI_DMS_CHL_MAC_PORT_APP, HI_DMS_PORT_CTRL_OFFLINE_ONLINE_RX, HI_NULL, HI_NULL);
}


//*****************************************************************************
// 函数名称: mrsPowerOnPro
// 功能描述: MRS模块上电处理函数
//
// 参数说明:
//  pstFwCtx
//
// 返 回 值:
//    无
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: fengxiaomin/00209182 [2014-1-14]
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
// 函数名称: APP_MrsTaskBody
// 功能描述: MRS 主控任务
//
// 参数说明:
//  This function has no arguments.
//
// 返 回 值:
//    无
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: shenhankun/00130424 [2011-11-08]
//*****************************************************************************
HI_VOID APP_MrsTaskBody(HI_U32 ulParam)
{
    MRS_NOT_USED(ulParam);
    HI_MDM_ReportInitEvt(HI_DEID_APP_START_MRS_OK, HI_DIAG_SYS_INIT_SET);

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    mrsSrvArchivesLoad();
	mrsDetectWaitTopoInit();//必须放在上电从flash中读取表档案之后,即mrsSrvArchivesLoad后面
#endif

    mrsPowerOnPro();
//CJJ-HPLC-20190103
#if defined(SIGNAL_MCU)&&defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
	if (!EQUIP_DUT_IS_EQUIP_MODE())//如果不是测试模式--weichao
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

            mrsDfxRefreshMsgQueueNum(); // 刷新当前队列深度

            start_time = HI_MDM_GetMilliSeconds();
            (HI_VOID)mrsMsgProc(&stMsg);
            mrsDfxMsgPrcoTimeConsuming(start_time, stMsg.ulMsgId); // 刷新消息处理时间

            mrsDfxRefreshMsgQueueNum(); // 刷新当前队列深度
        }
        else
        {
            HI_MDM_Sleep(HI_DMS_CHL_EXCEPTION_POLLING_WAIT);
        }
    }
}

// 加入消息队列判断
// 返回HI_TRUE:  允许加入
// 返回HI_FALSE: 拒绝加入
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


// 发送消息给 MRS 队列
HI_U32 mrsSendMessage2Queue(HI_IN HI_SYS_QUEUE_MSG_S* pstMsg)
{
    HI_U32 ret = HI_ERR_SUCCESS;

    mrsDfxRefreshMsgQueueNum();         // 刷新当前队列深度
    mrsDfxMsgTxCnt(pstMsg->ulMsgId);    // 消息发送统计

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
        mrsDfxMsgTxFailCnt(pstMsg->ulMsgId);    // 消息发送失败统计
        HI_MDM_ReportSysEvent(EID_APP_SEND_MSG_FAIL);
    }

    mrsDfxRefreshMsgQueueNum();    // 刷新当前队列深度

    return ret;
}

//*****************************************************************************
// 函数名称: mrsChlConnect
// 功能描述: PLC通道连接
//
// 参数说明:
//   usPort [in] PLC通道端口号
//   ucOption [in] 连接和断开，取值为 HI_DMS_CHL_CONNECT 或是 HI_DMS_CHL_DISCONNECT
//
// 返 回 值:
//    TODO: ...
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: liuxipeng/lKF54842 [2013-11-05]
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

    /* 获取到完整的帧数据 */
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
            mrsDfxChlUartOkFrameCnt(frame->usDatagramSize, HI_FALSE);    /* 串口接收完整帧统计(消息发送) */
        }

        (hi_void)memset_s(&msg, sizeof(msg), 0, sizeof(msg));
        msg.ulMsgId = EN_MRS_FW_MSG_376_2_FRAME_INPUT;
        msg.ulParam[0] = (uintptr_t)frame;

        ret = mrsSendMessage2Queue(&msg);
        if (ret != HI_ERR_SUCCESS) {
            mrsDfxChlUartErrFrameCnt(HI_FALSE);   /* 串口接收完整帧错误统计(消息发送) */
            HI_MDM_ReportSysEvent(EID_APP_SEND_MSG_FAIL);
            mrsToolsFree(frame);
        }
    } else if (ret != HI_ERR_CONTINUE) {
         mrsDfxChlUartErrRx(rx->usPayloadLen, ret, HI_FALSE); /* 串口接收错误处理(消息发送) */
    }

    return ret;
}
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
// CCO接收MR数据处理
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


    // 获取到完整的帧数据
    ret = mrs645ProtoStreamInput(pstRx->pPayload, pstRx->usPayloadLen, &stFrame, ID_MRS_UART_645BUF);

    if(HI_ERR_SUCCESS == ret)
    {
        HI_SYS_QUEUE_MSG_S stMsg = { 0 };
        MRS_ONE_RAW_FRAME_STRU* pstFrame = HI_NULL;

        if (stFrame.pucDatagram)
        {
            mrsDfxChlUartOkFrameCnt(stFrame.usDatagramSize, HI_FALSE);    // 串口接收完整帧统计(消息发送)
        }

        // 判断接收的645帧是否丢弃(缓存帧相同，或者表地址为非BCD码丢弃，并清空buffer)
        if (HI_ERR_SUCCESS == mrsIsSameAsBufferBbu(stFrame.usDatagramSize, stFrame.pucDatagram, ID_MRS_BBUDA_645BUF))
        {
            // 相同，丢弃
			mrsToolsFree(stFrame.pucDatagram);
            return HI_ERR_SUCCESS;
        }
        else if (!mrsToolsCheckBCD(stFrame.pucDatagram + 1, HI_METER_ADDR_LEN))
        {
            // 相同，丢弃
            mrsToolsFree(stFrame.pucDatagram);
            mrsClear645ProtoBufferBbu(ID_MRS_BBUDA_645BUF);
            return HI_ERR_SUCCESS;
        }
        else if(mrsToolsCheckBroadCast(stFrame.pucDatagram, stFrame.usDatagramSize))
        {
            // 清buffer，且报文不需要添加到buffer中
            mrsClear645ProtoBufferBbu(ID_MRS_BBUDA_645BUF);
        }
        else
        {
            // 添加到buffer中
            ret = mrsAddFrameToBufferBbu(stFrame.usDatagramSize, stFrame.pucDatagram, ID_MRS_BBUDA_645BUF);
            if (HI_ERR_SUCCESS != ret)
            {
                return ret;
            }
        }

        // 将645帧转为3762透抄抄表帧
        ret = mrsCreateFrame3762Bbu(stFrame.usDatagramSize, stFrame.pucDatagram, &pstFrame);

		mrsToolsFree(stFrame.pucDatagram);

        if(HI_ERR_SUCCESS == ret)
        {
            stMsg.ulParam[0] = (HI_U32)pstFrame;
            stMsg.ulMsgId = EN_MRS_FW_MSG_376_2_FRAME_INPUT;

            // 发送消息给 MRS队列, 进行消息分发调度
            ret = mrsSendMessage2Queue(&stMsg);
            if(HI_ERR_SUCCESS != ret)
            {
                mrsDfxChlUartErrFrameCnt(HI_FALSE);   // 串口接收完整帧错误统计(消息发送)
                HI_MDM_ReportSysEvent(EID_APP_SEND_MSG_FAIL);
				mrsToolsFree(pstFrame);
            }
            mrsInitMrStateBbu();
        }
    }
    else if(HI_ERR_CONTINUE == ret)
    {
        // 所有数据汇聚后, 发现总和不足1帧数据, 需要继续等待数据
    }
    else
    {
        mrsDfxChlUartErrRx(pstRx->usPayloadLen, ret, HI_FALSE); // 串口接收错误处理(消息发送)
    }

    return ret;
}
#endif

#endif

// STA接收MR数据处理
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

    // 收到数据之后更新抄表队列状态
    mrsStaUpdateQueueStatus();

    mrs645ProtoStreamInputCheck((HI_BOOL)pstRx->bCanDiscardOldData, ID_MRS_UART_645BUF);

    // 获取到完整的帧数据
    if (MRS_SRV_PROTO_645 != mrsStaGetFrmProto())
    {
        ulRet = mrs645ProtoStream2Buffer(pstRx->pPayload, pstRx->usPayloadLen, ID_MRS_UART_645BUF);
        if (HI_ERR_SUCCESS != ulRet)
        {
            mrsDfxChlUartErrRx(pstRx->usPayloadLen, ulRet, HI_FALSE); // 串口接收错误处理(消息发送)
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

        if (EQUIP_DUT_IS_EQUIP_MODE())//如果是测试模式
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
  

            mrsDfxChlUartOkFrameCnt(stFrame.usDatagramSize, HI_FALSE);    // 串口接收完整帧统计(消息发送)

            (hi_void)memcpy_s(pstFrame, sizeof(MRS_ONE_RAW_FRAME_STRU), &stFrame, sizeof(MRS_ONE_RAW_FRAME_STRU));
            stMsg.ulMsgId = EN_MRS_FW_MSG_645_FRAME_INPUT;

            // 发送消息给 MRS队列, 进行消息分发调度
            ulRet = mrsSendMessage2Queue(&stMsg);
            if (HI_ERR_SUCCESS != ulRet)
            {
                mrsToolsFree(stFrame.pucDatagram);

                mrsDfxChlUartErrFrameCnt(HI_FALSE);   // 串口接收完整帧错误统计(消息发送)
                HI_MDM_ReportSysEvent(EID_APP_SEND_MSG_FAIL);
            }
        }
    }
    else if(HI_ERR_CONTINUE == ulRet)
    {
        // 所有数据汇聚后, 发现总和不足1帧数据, 需要继续等待数据
    }
    else
    {
        mrsDfxChlUartErrRx(pstRx->usPayloadLen, ulRet, HI_FALSE); // 串口接收错误处理(消息发送)
    }

    return ulRet;
}
#endif

// 抄控器接收MR数据处理
#if defined(PRODUCT_CFG_PRODUCT_TYPE_NDM)
HI_U32 DoNdmMrRxProcess(HI_IN HI_DMS_CHL_RX_S* pstRx )
{
    HI_U32 ret = HI_ERR_SUCCESS;
    MRS_ONE_RAW_FRAME_STRU stFrame = {0};

    HI_MDM_NdmSerialInterfaceRxLedProc();

    mrs645ProtoStreamInputCheck((HI_BOOL)pstRx->bCanDiscardOldData, ID_MRS_UART_645BUF);

    // 获取到完整的帧数据
    ret = mrs645ProtoStreamInput(pstRx->pPayload, pstRx->usPayloadLen, &stFrame, ID_MRS_UART_645BUF);
    if(HI_ERR_SUCCESS == ret)
    {
        MRS_645_FRAME_STRU st645Hrd = { 0 };
        HI_NDM_645_FRAME_S * buf_645 = HI_NULL;
        HI_U8 MacAddr[HI_PLC_RELAY_NUM_MAX][HI_PLC_MAC_ADDR_LEN];

        HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1002, HI_DIAG_MT("[CCD-MR]-RX"), stFrame.pucDatagram, stFrame.usDatagramSize);

        (hi_void)memset_s(MacAddr, sizeof(MacAddr), 0, sizeof(MacAddr));

        mrsDfxChlUartOkFrameCnt(stFrame.usDatagramSize, HI_FALSE);    // 串口接收完整帧统计(消息发送)

        // 解析645帧
        ret = MRS_Proto645Dec(stFrame.pucDatagram,stFrame.usDatagramSize,&st645Hrd);
        if(HI_ERR_SUCCESS == ret)
        {
            if (mrsIsCcdDutMode(&st645Hrd))//整表检测抄控器适配1:8工装
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

            (hi_void)memset_s(MacAddr[0], HI_PLC_MAC_ADDR_LEN, 0xFF, HI_PLC_MAC_ADDR_LEN);// 使用广播地址
            if (mrsToolsNormalAddr(st645Hrd.ucAddr))
            {
                if (st645Hrd.ucAddr[0] != 0x00)
                {
                    (hi_void)memcpy_s(MacAddr[0], HI_PLC_MAC_ADDR_LEN, st645Hrd.ucAddr, HI_PLC_MAC_ADDR_LEN);
                    mrsHexInvert(MacAddr[0], HI_PLC_MAC_ADDR_LEN);
                    (hi_void)memcpy_s(MacAddr[1], HI_PLC_MAC_ADDR_LEN, MacAddr[0], HI_PLC_MAC_ADDR_LEN);
                }

                // 发送645报文Diag包
                ret = HI_MDM_SendNdmReqFrame(HI_NDM_645_FRAME,(HI_U8 *)buf_645,sizeof(HI_NDM_645_FRAME_S),MacAddr,0);
            }
            else
            {
                // 发送645报文Diag包
                ret = HI_MDM_SendNdmReqFrame(HI_NDM_645_FRAME,(HI_U8 *)buf_645,sizeof(HI_NDM_645_FRAME_S),MacAddr,0);
            }

            if(ret != HI_ERR_SUCCESS)
            {
                 mrsDfxChlUartErrFrameCnt(HI_FALSE);   // 串口接收完整帧错误统计(消息发送)
            }

            mrsToolsFree(buf_645);
        }
    }
    else if(HI_ERR_CONTINUE == ret)
    {
        // 所有数据汇聚后, 发现总和不足1帧数据, 需要继续等待数据
    }
    else
    {
        mrsDfxChlUartErrRx(pstRx->usPayloadLen, ret, HI_FALSE); // 串口接收错误处理(消息发送)
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
// 函数名称: mrsChlRx
// 功能描述: 通道接收
//
// 参数说明:
//   usPort [in] PLC通道端口号
//   pstRx [in] 接收数据
//
// 返 回 值:
//    TODO: ...
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: shenhankun/00130424 [2011-11-09]
//*****************************************************************************
HI_PRV HI_U32 mrsChlRx(HI_U16 usPort, HI_IN HI_DMS_CHL_RX_S* pstRx)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_DMS_CHL_RX_S* pstDispRx = HI_NULL;
    HI_SYS_QUEUE_MSG_S stMsg = { 0 };

    // 获取数据帧
    if (HI_DMS_CHL_UART_PORT_APP == usPort)
    {
        mrsDfxChlUartRx(pstRx->pPayload, pstRx->usPayloadLen, HI_FALSE);    // 串口接收统计(消息发送)

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO_BROAD_BAND_UNIT)
        // 如果NV项支持江苏版本波特率自适应，且为业务串口
        if (mrsToolIsBbu() && (!mrsIsSimuChlOpened()))
        {
            // 江苏版本波特率自适应 && 业务串口
            ret = DoCcoMrRxProcessBbu(pstRx);
        }
        else
#endif
        {
            // 模拟通道 || 非江苏版本约波特率自适应
            ret = DoCcoMrRxProcess(pstRx);
        }
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
        MRS_WaitSem(EN_APP_SEM_SRV_UART,HI_SYS_WAIT_FOREVER);
        ret = DoStaMrRxProcess(pstRx);
        MRS_SignalSem(EN_APP_SEM_SRV_UART); // 信号量，解锁
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
        mrsDfxChlPlcRx(pstRx, HI_FALSE);    // PLC通道接收统计(消息发送)

        //
        // 采用异步方式,模块间调试独立化,避免交叉, 方便功能演进, 任务栈大小设置独立
        // 存在内存都一次分配, 解决方法是从MAC过来的数据内存, MAC不释放, 后续待优化.
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

        // 发送消息给 MRS队列, 进行消息分发调度
        ret = mrsSendMessage2Queue(&stMsg);
        if (HI_ERR_SUCCESS != ret)
        {
            mrsDfxChlPlcErrRx(pstRx, ret, HI_FALSE);    // PLC通道接收错误统计(消息发送)
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

//NDM接收plc帧处理
#if defined(PRODUCT_CFG_PRODUCT_TYPE_NDM)

#define MR_CCO_NDM_INTERCONNECT_FRAME_HEAD_SIZE 3
HI_U32 mrsNdmPlcProcess(HI_IN HI_U16 usId, HI_PVOID pReqPacket, HI_U16 usReqPacketSize)
{
	HI_U32 ret = HI_ERR_SUCCESS;

    HI_UNREF_PARAM(usId);
    HI_UNREF_PARAM(usReqPacketSize);

    MRS_PLC_FRAME_DATA_STRU *pstRx = (MRS_PLC_FRAME_DATA_STRU *)pReqPacket;
	if(!mrsToolsBroadCastMac(pstRx->ucMac)) //广播帧屏蔽
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
        /*0x0040-抄控器-CCO下行、上行报文格式说明*/
        /*域        字节号  大小(比特)           */
        /*协议类型  0       8                    */
        /*报文长度  1-2     16                   */
        /*报文内容  3-N                          */
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
// 函数名称: mrsChlMsgProcFromMac
// 功能描述: 处理来自Mac层的通道消息
//
// 参数说明:
//   ulMsgId [in] 消息ID
//   pMsgParam [in] 消息参数
//
// 返 回 值:
//    参见返回值列表
//
// 调用要求:
// 调用举例:
// 作    者: liuxipeng/lkf54842 [2012-11-13]
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
// 函数名称: mrsChlMsgProc
// 功能描述: 通道消息处理
//
// 参数说明:
//   usPort [in] 通道端口号(通道类型)
//   ulMsgId [in] 消息ID
//   pMsgParam [in] 消息参数
//
// 返 回 值:
//    参见返回值列表
//
// 调用要求:
// 调用举例:
// 作    者: liuxipeng/lkf54842 [2012-11-13]
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
// 通道端口对象设置
//*****************************************************************************
HI_PRV HI_CONST HI_DMS_CHL_PORT_OBJ_S g_stMrsChlPort = { mrsChlConnect, mrsChlRx, HI_NULL, mrsChlMsgProc};

//获取通道端口
HI_DMS_CHL_PORT_OBJ_S mrsGetChlPort(HI_VOID)
{
    return g_stMrsChlPort;
}

/* BEGIN: Added by liuxipeng/lkf54842 2014/01/15 问题单号: DTS2013121904065 */
//*****************************************************************************
// 函数名称: mrsFwChlInit
// 功能描述: MRS框架通道初始化
//           完成各个通道的初始化安装、回调函数注册功能
//           当识别为UART端口并且为II型采集器时将UART模式设置为485
//           当识别为IR端口后将模式设置为IR
// 参数说明:
//          pstChlPort : 通道回调函数实例
//          usPortNum  : 通道端口号
// 返 回 值:
//    HI_ERR_SUCCESS : 成功
//    其它值为错误码
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: liuxipeng/lkf54842 [2014-01-15]
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
// 函数名称: mrsFwInit
// 功能描述: MRS框架初始化
//
// 参数说明:
//  This function has no arguments.
//
// 返 回 值:
//    HI_ERR_SUCCESS : 成功
//    其它值为错误码
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: shenhankun/00130424 [2011-11-09]
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
// 函数名称: MRS_CmdRegister
// 功能描述: 命令注册
//
// 参数说明:
//   pstTbl [in] 命令处理入口表
//   usNum [in] 命令处理入口个数
//
// 返 回 值:
//    TODO: ...
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: shenhankun/00130424 [2011-11-04]
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
// 函数名称: MRS_MsgRegister
// 功能描述: 消息注册
//
// 参数说明:
//   pstTbl [in] 消息处理入口表
//   usNum [in] 消息处理入口个数
//
// 返 回 值:
//    TODO: ...
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: shenhankun/00130424 [2011-11-04]
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
// 函数名称: mrsChlConnectMsgProc
// 功能描述: 通道变更消息处理
//
// 参数说明:
//   usPort [in] 通道类型
//   ucOption [in] 选项字 连接/断开
//
// 返 回 值:
//    HI_ERR_SUCCESS   : 处理成功
//    HI_ERR_NOT_FOUND : 没有找到注册的消息处理入口
//    其它为错误码
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: liuxipeng/lkf54842 [2013-08-30]
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

    mrsDfxChlUartRx(pstFrame->pucDatagram, pstFrame->usDatagramSize, HI_TRUE);  // 串口接收统计(消息处理)

    // 消息分发给 Service
    ret = mrsDispatchRx3762Data(pstFrame);
    if(HI_ERR_SUCCESS == ret)
    {
        mrsDfxChlUartOkFrameCnt(pstFrame->usDatagramSize, HI_TRUE);    // 串口接收完整帧统计(消息处理)
    }
    else
    {
        mrsDfxChlUartErrRx(pstFrame->usDatagramSize, ret, HI_TRUE);     // 串口接收错误处理(消息处理)
        mrsDfxChlUartErrFrameCnt(HI_TRUE);   // 串口接收完整帧错误统计(消息处理)
    }

    // 分发处理接收后, 资源释放
    mrsToolsFree(pstFrame);

    return ret;
}
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
HI_PRVL HI_U32 mrsMsgProcUartInput(HI_U16 usId, HI_SYS_QUEUE_MSG_S* pstMsg)
{
    MRS_ONE_RAW_FRAME_STRU* pstFrame = (MRS_ONE_RAW_FRAME_STRU*)&pstMsg->ulParam[0];

    mrsDfxChlUartRx(pstFrame->pucDatagram, pstFrame->usDatagramSize, HI_TRUE);    // 串口接收统计(消息处理)
    MRS_MSG_NODE_STRU * pstNode = mrsFindMsgNode(usId);

    if (pstNode != HI_NULL && pstNode->pfnMsg)
    {
        MRS_MSG_MR_645_DATA_STRU stData = {0};
        stData.pucData = pstFrame->pucDatagram;
        stData.usDataSizse = pstFrame->usDatagramSize;
        pstNode->pfnMsg(usId, &stData);

        mrsDfxChlUartOkFrameCnt(pstFrame->usDatagramSize, HI_TRUE);    // 串口接收完整帧统计(消息处理)
        mrsToolsFree(pstFrame->pucDatagram);
        return HI_ERR_SUCCESS;
    }

    mrsDfxChlUartErrRx(pstFrame->usDatagramSize, HI_ERR_FAILURE, HI_TRUE); // 串口接收错误处理(消息处理)
    mrsDfxChlUartErrFrameCnt(HI_TRUE);   // 串口接收完整帧错误统计(消息处理)

    mrsToolsFree(pstFrame->pucDatagram);

    return HI_ERR_FAILURE;
}
#endif

HI_PRVL HI_U32 mrsMsgProcMacInput(HI_IN HI_SYS_QUEUE_MSG_S* pstMsg)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_DMS_CHL_RX_S* pstRx = (HI_DMS_CHL_RX_S*)pstMsg->ulParam[0];

    HI_ASSERT(pstRx);

    mrsDfxChlPlcRx(pstRx, HI_TRUE);    // PLC通道接收统计(消息处理)

    // CCO<-->STA 之间通信 命令帧分发处理
    ret = mrsDispatchCommand((HI_U16)pstMsg->ulParam[1], pstRx);
    if(ret != HI_ERR_SUCCESS)
    {
        mrsDfxChlPlcErrRx(pstRx, ret, HI_TRUE);    // PLC通道接收错误统计(消息处理)
    }

    mrsToolsFree(pstRx);

    return ret;
}

HI_PRVL HI_U32 mrsMsgProcMrOutput(HI_IN HI_SYS_QUEUE_MSG_S* pstMsg)
{
    HI_U32 ret = HI_ERR_SUCCESS;

    if(HI_DMS_CHL_UART_PORT_APP == pstMsg->ulParam[1])
    {
        mrsDfxChlUartTxMsgRx((HI_DMS_CHL_TX_DATA_S*)pstMsg->ulParam[0]);    // 串口发送统计(消息处理)
    }

    ret = mrsSend2MrDevice(pstMsg);
/* BEGIN: Added by liupeidong/00238134, 2014/12/10 问题单号:DTS2014121005392 */
#if defined(PRODUCT_CFG_PRODUCT_TYPE_NDM)
    if (HI_MDM_IsNdmMeterAccess())
    {
        HI_MDM_NdmSerialInterfaceTxLedProc();
    }
#endif
/* END: Added by liupeidong/00238134, 2014/12/10 问题单号:DTS2014121005392 */

    if(HI_DMS_CHL_UART_PORT_APP == pstMsg->ulParam[1])
    {
        if(HI_ERR_SUCCESS != ret)
        {
            HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_012, HI_DIAG_MT("TX MR Fail-[%d]"),ret);
            mrsDfxChlUartErrTxMsgRx((HI_DMS_CHL_TX_DATA_S*)pstMsg->ulParam[0], ret);    // 串口发送失败统计(消息处理)
        }
        else
        {
            mrsDfxChlUartOkTxMsgRx((HI_DMS_CHL_TX_DATA_S*)pstMsg->ulParam[0]);  // 串口发送统计(消息处理)
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

    mrsDfxChlPlcTx(pstData, pstAddr, HI_TRUE);    // PLC发送统计(消息处理)

    ret = mrsSend2MacLayer(pstMsg);
    if(HI_ERR_SUCCESS != ret)
    {
        HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_013, HI_DIAG_MT("TX PLC Fail-[%d]"),ret);

        mrsDfxChlPlcErrTx(pstData, pstAddr, ret, HI_TRUE);    // PLC发送失败统计(消息处理)
        mrsDfxPlcBcTxErr(pstAddr);    // 统计广播帧发送失败
    }
    else
    {
        mrsDfxPlcBcTx(pstAddr);   // 统计发送广播帧
    }

    mrsToolsFree(pstData);
    mrsToolsFree(pstAddr);

    return ret;
}

//*****************************************************************************
// 函数名称: mrsMsgProc
// 功能描述: 任务消息处理
//
// 参数说明:
//   pstMsg [in] 消息
//
// 返 回 值:
//    HI_ERR_SUCCESS   : 处理成功
//    HI_ERR_NOT_FOUND : 没有找到注册的消息处理入口
//    其它为错误码
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: shenhankun/00130424 [2011-12-26]
//*****************************************************************************
HI_PRV HI_U32 mrsMsgProc(HI_IN HI_SYS_QUEUE_MSG_S* pstMsg)
{
    HI_U32 ret = HI_ERR_SUCCESS;

    HI_ASSERT(pstMsg);

    mrsDfxMsgRxCnt(pstMsg); // 消息接收统计

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

    mrsDfxMsgRxCnt(pstMsg); // 消息接收统计

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
     //处理停电上报事件
    case EN_MRS_FW_MSG_POWER_FAILURE:
        ret = mrsHandlePowerFailure(pstMsg);
        break;
#endif
#if defined(PRODUCT_CFG_PRODUCT_TYPE_NDM)
    case EN_MRS_FW_MSG_DETECTID:
        ret = mrs_ndm_handle_detect_id(pstMsg);
        break;
#endif
/* BEGIN: Added by fengxiaomin/00209182, 2014/1/10   问题单号号:DTS2014012607684  */
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

// Timer消息处理
HI_PRV HI_U32 mrsTimerProc(HI_U32 ulTimerId)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    MRS_MSG_NODE_STRU* pstNode = mrsFindMsgNode(ID_MRS_MSG_MR_TIMER);

    if (pstNode)
    {
        // 产生 ID_MRS_MSG_MR_TIMER 消息 给 Service
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

// 缺省消息处理
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

// PLC命令分发处理
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
                mrsDfxPlcBcRx();  // 统计接收广播帧
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
            ret = HI_ERR_NOT_REGISTER; // 没有注册
        }
    }
    else
    {
        ret = HI_ERR_NOT_FOUND;
    }

    return ret;
}

// 376.2接收后的 分发处理
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

// 查找命令处理节点
HI_PRV MRS_CMD_NODE_STRU* mrsFindCmdNode(HI_U16 usCmdId)
{
    HI_U16 i = 0;
    MRS_FW_CTX_STRU* pstFwCtx = GetFwCtx();

    // 查找注册的命令REQ处理入口
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


// 查找消息处理节点
HI_PRV MRS_MSG_NODE_STRU* mrsFindMsgNode(HI_U16 usId)
{
    HI_U32 i = 0;
    MRS_FW_CTX_STRU* pstFwCtx = GetFwCtx();

    // 查找注册的命令REQ处理入口
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


// 抄表数据会写到抄表设备中
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

    // TODO: 目前版本仅支持 UART  (shenhankun/2011-12-29)
    ret = HI_MDM_ChlTx(ucChlType, pstFrameData, HI_NULL);
    if(HI_ERR_SUCCESS != ret)
    {
        HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_014, HI_DIAG_MT("UART TX Fail-[%d]"), ret);
    }

    return ret;
}

// PLC帧发送
HI_PRVL HI_U32 mrsSend2MacLayer(HI_IN HI_SYS_QUEUE_MSG_S* pstMsg)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_DMS_CHL_TX_DATA_S* pstFrameData = (HI_DMS_CHL_TX_DATA_S*)pstMsg->ulParam[0];
    HI_DMS_CHL_TX_ADDR_S* pstFrameAddr = (HI_DMS_CHL_TX_ADDR_S*)pstMsg->ulParam[1];

    // 目前版本仅支持 MAC 通道  (shenhankun/2011-12-29)
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
 函 数 名  : MRS_SendFrameProc
 功能描述  : 发送帧的功能提取函数
 输入参数  : HI_IN MRS_PLC_FRAME_DATA_S* pstFrameData
 输出参数  : 无
 返 回 值  : HI_U32
 本函数调用的函数  :
 调用本函数的函数  :
 访问的全局变量  :
 修改的全局变量  :
 修改的BBP寄存器列表  :

 修改历史      :
  1.日    期   : 2017年9月19日
    作    者   : test
    修改内容   : 新生成函数

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

    //默认优先级为3(3为最高优先级,0为最低优先级)
    pstAddr->ucPrio = 3;
    if ((pstFrameData->usId >= ID_MRS_CMD_GET_METERLIST) && (pstFrameData->usId <= ID_MRS_CMD_STOP_SEARCH_METER))
    {
        pstAddr->ucPrio = 2;
    }

    mrsDfxChlPlcTx(pstFrame, pstAddr, HI_FALSE);    // PLC发送统计(消息发送)

    stMsg.ulMsgId       = EN_MRS_FW_MSG_MAC_FRAME_OUTPUT;
    stMsg.ulParam[0]    = (HI_U32) pstFrame;
    stMsg.ulParam[1]    = (HI_U32) pstAddr;

    // 发送消息给 MRS队列, 进行消息分发调度
    ulRet = mrsSendMessage2Queue(&stMsg);
    if(HI_ERR_SUCCESS != ulRet)
    {
        mrsDfxChlPlcErrTx(pstFrame, pstAddr, ulRet, HI_FALSE);    // PLC发送失败统计(消息发送)

        HI_MDM_ReportSysEvent(EID_APP_SEND_MSG_FAIL);

        mrsToolsFree(pstFrame);
        mrsToolsFree(pstAddr);
    }

    return ulRet;
}


//*****************************************************************************
// 函数名称: MRS_SendPlcFrame
// 功能描述: 发送PLC帧的数据给CCO/STA
//
// 参数说明:
//   pstFrameData [in] MR帧数据
//
// 返 回 值:
//    HI_ERR_SUCCESS: 成功写入系统队列中, 由底层模块负责发送到其它通信模块.
//    其它错误码
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: shenhankun/00130424 [2011-11-04]
//*****************************************************************************
HI_U32 MRS_SendPlcFrame(HI_IN MRS_PLC_FRAME_DATA_S* pstFrameData)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;

    #if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
    POWER_FAILURE_RCV_MAC_REPORT_S *deviceState = mrsGetPowerFailureMacInfo();
    //停电判断,发生停电后，不做发送处理
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
// 函数名称: MRS_ChangeBaudRate
// 功能描述: 更改波特率
//
// 参数说明:
//   ulRate 波特率
//
// 返 回 值:
//    HI_ERR_SUCCESS: 成功修改波特率
//    其它错误码
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: shenhankun/00130424 [2012-09-22]
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
// 函数名称: MRS_SendMrData
// 功能描述: 发送抄表数据给电表/集中器
//
// 参数说明:
//   pucData [in] MR数据
//   usDataSize [in] MR数据大小,单位字节
//
// 返 回 值:
//    HI_ERR_SUCCESS: 成功写入DMS队列中, 由DMS写入MR设备中
//    其它错误码
//
// 调用要求: 不支持自动填充前导字节等功能,仅提供通道功能发送数据的功能
// 调用举例: TODO: ...
// 作    者: shenhankun/00130424 [2011-11-04]
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

    // 只进行业务串口数据的统计
    if(HI_DMS_CHL_UART_PORT_APP == ucChlType)
    {
        mrsDfxChlUartTxMsgTx(pucData, usDataSize);  // 串口发送统计(消息发送)
    }

    pstFrameData->usDataSize = usDataSize;
    (hi_void)memcpy_s(pstFrameData->ucData, pstFrameData->usDataSize, pucData, usDataSize);

    stMsg.ulMsgId     = EN_MRS_FW_MSG_MR_FRAME_OUTPUT;
    stMsg.ulParam[0]  = (HI_U32)pstFrameData;
    stMsg.ulParam[1] = ucChlType;

    // 发送消息给 MRS队列, 进行消息分发调度
    ret = mrsSendMessage2Queue(&stMsg);
    if(HI_ERR_SUCCESS != ret)
    {
        // 只进行业务串口数据的统计
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
		
        // 只进行业务串口数据的统计
        if(HI_DMS_CHL_UART_PORT_APP == ucChlType)
        {
            mrsDfxChlUartOkTxMsgTx(usDataSize); // 串口发送成功统计(消息发送)

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
            if (mrsToolsIsIICollector())
            {
                mrsUARTRTxLedPro(); // UART发送 LED灯闪
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
// 函数名称: MRS_UartWrite
// 功能描述: 发送抄表数据给串口(无消息传递直接写串口)
//
// 参数说明:
//   pucData [in] MR数据
//   usDataSize [in] MR数据大小,单位字节
//
// 返 回 值:
//    HI_ERR_SUCCESS: 成功写入Uart中
//    其它错误码
//
// 调用要求: 不支持自动填充前导字节等功能,仅提供通道功能发送数据的功能
// 调用举例: TODO: ...
// 作    者: liuxipeng/lkf54842 [2014-01-15]
//*****************************************************************************
#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
HI_U32 MRS_UartWrite(HI_IN HI_PBYTE pucData, HI_U16 usDataSize, HI_U16 ucChlType)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_DMS_CHL_TX_DATA_S* pstFrameData = HI_NULL;


    mrsDfxChlUartTxMsgTx(pucData, usDataSize);  // 串口发送统计(消息发送)
    mrsDfxChlUartOkTxMsgTx(usDataSize);         // 串口发送成功统计(消息发送)

    if(0 == usDataSize)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    pstFrameData = (HI_DMS_CHL_TX_DATA_S*)mrsToolsMalloc(usDataSize + sizeof(HI_DMS_CHL_TX_DATA_S));
    if(HI_NULL == pstFrameData)
    {
        return HI_ERR_NO_MORE_MEMORY;
    }

    // 只进行业务串口数据的统计
    if(HI_DMS_CHL_UART_PORT_APP == ucChlType)
    {
        mrsDfxChlUartTxMsgRx(pstFrameData);  // 串口发送统计(消息处理)
    }

    pstFrameData->usDataSize = usDataSize;
    (hi_void)memcpy_s(pstFrameData->ucData, pstFrameData->usDataSize, pucData, usDataSize);

    ret = HI_MDM_ChlTx(ucChlType, pstFrameData, HI_NULL);

    if(HI_ERR_SUCCESS != ret)
    {
        // 只进行业务串口数据的统计
        if(HI_DMS_CHL_UART_PORT_APP == ucChlType)
        {
            mrsDfxChlUartErrTxMsgRx(pstFrameData, ret);
        }
    }
    else
    {
        // 只进行业务串口数据的统计
        if(HI_DMS_CHL_UART_PORT_APP == ucChlType)
        {
            mrsDfxChlUartOkTxMsgRx(pstFrameData); // 串口发送成功统计(消息处理)

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

	//读客户版本信息
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
