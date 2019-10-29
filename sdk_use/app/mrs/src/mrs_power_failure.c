//*****************************************************************************
//
//                  版权所有 (C), 2017-2019, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_power_failure.c
//  版 本 号   : V1.0
//  作    者   : jingyankai/00130424
//  生成日期   : 2017-8-16
//  功能描述   : 停电事件处理
//
//  函数列表   :
//  修改历史   :
//  1.日    期 :
//    作    者 :
//    修改内容 :
//
//*****************************************************************************
#include "mrs_common.h"
#include "mrs_cmd_msg.h"
#include "mrs_dfx.h"
#include "mrs_fw_proto376_2_frame.h"
#include "mrs_fw_proto376_2_echo.h"
#include "mrs_fw_log.h"
#include "mrs_srv_common.h"
#include "mrs_srv_cco_macmap.h"
#include "mrs_srv_parallel_cco.h"
#include "mrs_srv_cltii_power_on_off.h"
#include "mrs_power_failure.h"
#include "mrs_fw_n.h"
#include "mrs_srv_sta_event.h"
#include "mrs_srv_cco.h"
#include "mrs_srv_cco_event.h"
#include "mrs_fw_tools.h"
#include "mrs_srv_cco_event.h"
#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
#include "mrs_srv_sta_queue.h"
#include "mrs_srv_sta_baudrate.h"
#include "mrs_srv_parallel_sta.h"
#include "mrs_srv_sta.h"
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) || defined(PRODUCT_CFG_PRODUCT_TYPE_STA)

//#define BIT_SET(x,i,b)  ((x)=(HI_U8)((b)?((1<<(i))|(x)):((~(HI_U8)(1<<(i)))&(x))))
#define BIT_SET(x,i)  ((x)=(HI_U8)(((1<<(i))|(x))))
#define BIT_RESET(x,i) ((x)=(HI_U8)((~(1<<(i))&(x))))
#define BIT_GET(x,i) (((1<<(i)) & (x))?1:0)

HI_VOID mrsPowerFailureBitSet(HI_U8 *pBitmap, HI_U16 pos);
HI_BOOL mrsPowerFailureBitTest(HI_U8 *pBitmap, HI_U16 pos);

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
hi_void mrsStaNormalBitmap(hi_u8 *bitmapIn, hi_u8 *bitmapOut, hi_u16 bitmapLen);
HI_U16 mrsCalcMinTei(HI_U8 *bitmapIn);
HI_U16 mrsCalcMaxTei(HI_U8 *bitmapIn);
HI_U32 mrsStaSendPowerFailureFrame(MRS_PLC_FRAME_DATA_S *pstReportData);
HI_VOID mrsStaPowerFailureSendEnd(POWER_FAILURE_CTRL_S* pstPowerFailureCtrl);
HI_U32 mrsStaHandleLocalPowerFailure(HI_VOID);
hi_u32 mrs_power_failure_handle_not_power_down(hi_void);
#endif
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
hi_void mrsCcoReportBitmap(hi_u8 event_type);
HI_U16 mrsCcoCalcMinTei(hi_u8 *bitmapIn);
HI_U16 mrsCcoCalcMaxTei(hi_u8 *bitmapIn);
HI_U32 mrsCcoGetBitmapTeiCnt(HI_U16 bitmapMinTei, HI_U16 bitmapMaxTei, hi_u8 *bitmapIn);
HI_VOID mrsPowerFailureDfxInfo(HI_VOID);
#endif
HI_VOID mrsGetStaOthersPowerFailureFrameData(HI_IN MRS_EVENT_INFO_STRU * pstRcvEvtInfo);
#if defined(PRODUCT_CFG_SUPPORT_MRS_DUOL_MODE_RF)
#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
extern HI_VOID mrsRfSetStateInfo(HI_U8 nFlag);
extern HI_VOID mrsRfSetNeedRfTransmitPowerInfo(HI_U32 ulNeedSendCount);
extern HI_VOID mrsRfClearPowerState(HI_VOID);
#endif
#endif

/************************************************************
                   本文件定义的全局变量
************************************************************/
NV_APP_MRS_EVT_CFG_STRU g_power_failure_nv = {0};

//停电上报相关控制信息
POWER_FAILURE_CTRL_S g_power_failure_ctrl = {
    {0}, {0},
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    {0}, {0}, 0, HI_FALSE, 0, HI_FALSE,{0}, {0},
#endif
    HI_FALSE, HI_FALSE, 0, HI_FALSE, 0, 0, 0
};

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
//mac传递上来的状态信息
POWER_FAILURE_RCV_MAC_REPORT_S g_rcv_mac_transmit_info = {0};
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
CCO_POWER_FAILURE_STATISTIC_S g_power_failure_mrs_dfx_info = {0};
#endif
HI_U32 mrsPhasePowerFailureCallback(HI_U8 ucType, HI_MDM_PHASE_POWER_FAILURE_REPORT_ST *pCmdData, HI_U32 ulDataSize)
{
    //HI_DIAG_LOG_MSG_E4(MRS_FILE_LOG_FLAG_047, HI_DIAG_MT("MRS_GET_PHASE_POWERFAIULRE_FROM_MAC in call back"),
                    //ucType,pCmdData->phase[0],pCmdData->phase[1],pCmdData->phase[2]);
    return HI_ERR_SUCCESS;
}
HI_U32 mrsCcoPowerFailureCallback(HI_U8 ucType, HI_MDM_POWER_FAILURE_REPORT_ST *pCmdData, HI_U32 ulDataSize)
{
    //HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_048, HI_DIAG_MT("MRS_GET_CCO_POWERFAIULRE_FROM_MAC in call back"),
                    //pCmdData->device_power_failure,pCmdData->my_tei);
    return HI_ERR_SUCCESS;
}

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
//*****************************************************************************
// 函数名称: mrsPowerFailureCallback
// 功能描述: MRS的回调函数
//
// 参数说明:
//  This function has no arguments.
//
// 返 回 值:
//    TODO: ...
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: j00415756
//*****************************************************************************
HI_U32 mrsPowerFailureCallback(HI_U8 ucType, HI_MDM_POWER_FAILURE_REPORT_ST *pCmdData, HI_U32 ulDataSize)

{
    HI_U32 ret = HI_ERR_FAILURE;
    HI_SYS_QUEUE_MSG_S stMsg = { 0 };

    //id编号
    stMsg.ulMsgId = EN_MRS_FW_MSG_POWER_FAILURE;
    stMsg.ulParam[0] = (HI_U32)ucType;
    stMsg.ulParam[1] = (HI_U32)(ulDataSize);
    //存入站点tei,设备状态标识，拉动管脚信息
    (hi_void)memcpy_s(&stMsg.ulParam[2], sizeof(HI_U32),pCmdData, sizeof(HI_MDM_POWER_FAILURE_REPORT_ST));

    //HI_DIAG_LOG_MSG_E3(MRS_FILE_LOG_FLAG_007, HI_DIAG_MT("MRS_GET_DATA_FROM_MAC in call back"),
                    //pCmdData->device_power_failure,pCmdData->device_not_pull_reset, pCmdData->my_tei);
    //HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_008, HI_DIAG_MT("MRS_GET_DATA_FROM_MAC in call back copy"),
                    //stMsg.ulParam[2]);
    //发送进入队列中等待处理
    ret = mrsSendMessage2Queue(&stMsg);

    return ret;
}
#endif
//位运算函数
HI_VOID mrsPowerFailureBitSet(HI_U8 *pBitmap, HI_U16 pos)
{
    HI_U8 ucPos = (HI_U8)(pos / 8);
    HI_U8 ucBit = pos % 8;

    BIT_SET(pBitmap[ucPos], ucBit);
}

HI_BOOL mrsPowerFailureBitTest(HI_U8 *pBitmap, HI_U16 pos)
{
    HI_U8 ucPos = (HI_U8)(pos / 8);
    HI_U8 ucBit = pos % 8;

    return (HI_BOOL)(1 == BIT_GET(pBitmap[ucPos], ucBit));
}
/*****************************************************************************
 函 数 名  : mrsGetPowerFailureCtrlInfo
 功能描述  : 获取停电上报事件的相关变量
 输出参数  : 无
 返 回 值  : power_failure_bitmap_st *
 本函数调用的函数  :
 调用本函数的函数  :
 访问的全局变量  :
 修改的全局变量  :
 修改的BBP寄存器列表  :

 修改历史      :
  1.日    期   : 2017年8月18日
    作    者   : test
    修改内容   : 新生成函数

*****************************************************************************/
POWER_FAILURE_CTRL_S *mrsGetPowerFailureCtrlInfo(HI_VOID)
{
    return &g_power_failure_ctrl;
}

/*****************************************************************************
 函 数 名  : mrsGetPowerFailureNvCtrl
 功能描述  : 获取停电控制变量
 输出参数  : 无
 返 回 值  : HI_U32
 本函数调用的函数  :
 调用本函数的函数  :
 访问的全局变量  :
 修改的全局变量  :
 修改的BBP寄存器列表  :

 修改历史      :
  1.日    期   : 2017年8月16日
    作    者   : test
    修改内容   : 新生成函数

*****************************************************************************/
NV_APP_MRS_EVT_CFG_STRU *mrsGetPowerFailureNvCtrl(HI_VOID)
{
    return &g_power_failure_nv;
}

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
/*****************************************************************************
 函 数 名  : mrsGetPowerFailureRcv
 功能描述  : 获取mac传递上来的信息
 输出参数  : 无
 返 回 值  : POWER_FAILURE_RCV_MAC_REPORT_S *
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
POWER_FAILURE_RCV_MAC_REPORT_S *mrsGetPowerFailureMacInfo(HI_VOID)
{
    return &g_rcv_mac_transmit_info;
}
#endif
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
/*****************************************************************************
 函 数 名  : mrsGetPowerFailureDfxInfo
 功能描述  : 获取维测信息函数
 输入参数  : HI_VOID
 输出参数  : 无
 返 回 值  : CCO_POWER_FAILURE_STATISTIC_S *
 本函数调用的函数  :
 调用本函数的函数  :
 访问的全局变量  :
 修改的全局变量  :
 修改的BBP寄存器列表  :

 修改历史      :
  1.日    期   : 2017年10月19日
    作    者   : test
    修改内容   : 新生成函数

*****************************************************************************/
CCO_POWER_FAILURE_STATISTIC_S *mrsGetPowerFailureDfxInfo(HI_VOID)
{
    return &g_power_failure_mrs_dfx_info;
}
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
/*****************************************************************************
 函 数 名  : mrsHandlePowerFailure
 功能描述  : 停电处理入口函数
 输入参数  : HI_IN HI_SYS_QUEUE_MSG_S* pstMsg
 输出参数  : 无
 返 回 值  : HI_U32
 本函数调用的函数  :
 调用本函数的函数  :
 访问的全局变量  :
 修改的全局变量  :
 修改的BBP寄存器列表  :

 修改历史      :
  1.日    期   : 2017年8月16日
    作    者   : j00415756
    修改内容   : 新生成函数

*****************************************************************************/
HI_U32 mrsHandlePowerFailure(HI_IN HI_SYS_QUEUE_MSG_S* pstMsg)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U32 eventType = 0;
	HI_MDM_POWER_FAILURE_REPORT_ST *report_info = HI_NULL;
    POWER_FAILURE_RCV_MAC_REPORT_S *power_failure_rcv_info = mrsGetPowerFailureMacInfo();
    HI_SYS_QUEUE_MSG_S *rcvPstmsg = pstMsg;

    if(HI_NULL == pstMsg)
    {
        return ret;
    }

    eventType = rcvPstmsg->ulParam[0];
	report_info = (HI_MDM_POWER_FAILURE_REPORT_ST *)&rcvPstmsg->ulParam[2];
	power_failure_rcv_info->my_tei = report_info->my_tei;
	power_failure_rcv_info->device_power_failure = report_info->device_power_failure;
	power_failure_rcv_info->device_pull_reset = report_info->device_not_pull_reset;
	power_failure_rcv_info->is_plug_module = report_info->is_plug_module;

    HI_DIAG_LOG_MSG_E4(MRS_FILE_LOG_FLAG_013, HI_DIAG_MT("MRS_GET_DATA_FROM_MAC"),
                    power_failure_rcv_info->device_power_failure,power_failure_rcv_info->is_plug_module,power_failure_rcv_info->device_pull_reset, eventType);

    switch(eventType)
    {
        case HI_MDM_POWER_FAILURE_EVENT_LOCAL:
            ret = mrsHandleLocalPowerFailure();
#if defined(PRODUCT_CFG_SUPPORT_MRS_DUOL_MODE_RF)&&defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
			mrsRfSetStateInfo(1);
    		mrsRfSetNeedRfTransmitPowerInfo(3);
#endif
            break;
        case HI_MDM_POWER_FAILURE_EVNET_POWERON:
            mrsHandlePowerOn();
#if defined(PRODUCT_CFG_SUPPORT_MRS_DUOL_MODE_RF)&&defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
			mrsRfSetStateInfo(0);
#endif
            break;
        case HI_MDM_POWER_ON_EVNET_REPORT:
            //mrsHandleLocalPowerOn();
            break;
		case HI_MDM_POWER_FAILURE_EVNET_NOT_POWER_DOWN:
			ret = mrs_power_failure_handle_not_power_down();
			break;
        default:
            ret = HI_ERR_FAILURE;
            break;
    }

    return ret;
}

/*****************************************************************************
 函 数 名  : mrsHandlePowerOn
 功能描述  : 停电再恢复事件处理函数
 输入参数  : HI_VOID
 输出参数  : 无
 返 回 值  : HI_VOID
 本函数调用的函数  :
 调用本函数的函数  :
 访问的全局变量  :
 修改的全局变量  :
 修改的BBP寄存器列表  :

 修改历史      :
  1.日    期   : 2017年8月18日
    作    者   : test
    修改内容   : 新生成函数

*****************************************************************************/
HI_VOID mrsHandlePowerOn(HI_VOID)
{
	hi_u32 ret  = 0;
	HI_SYS_REBOOT_CAUSE_E reset_reason;
	POWER_FAILURE_CTRL_S *power_failure_ctrl_info = mrsGetPowerFailureCtrlInfo();
    POWER_FAILURE_RCV_MAC_REPORT_S *rcv_mac_info = mrsGetPowerFailureMacInfo();

	reset_reason = (rcv_mac_info->is_plug_module == HI_TRUE) ? HI_SYS_REBOOT_CAUSE_MRS15 : HI_SYS_REBOOT_CAUSE_MRS11;
	//osreset软重启
    HI_SYS_RstDelayDisable(HI_SYS_RST_DELAY_CON_LOCK_NETWORK);
    HI_SYS_RstDelayDisable(HI_SYS_RST_DELAY_CON_TF_IDENTIFY);
    HI_SYS_RstDelayDisable(HI_SYS_RST_DELAY_CON_NDM_METER_CHECK);
	HI_SYS_RstDelayDisable(HI_SYS_RST_DELAY_ENTER_PHY_TEST_MODE);

	power_failure_ctrl_info->power_on_reset_time = HI_MDM_GetSeconds();
	ret = HI_MDM_Reboot(reset_reason);

    //HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_001, HI_DIAG_MT("MRS_REBOOT"), ret);
}

hi_u32 mrs_power_failure_get_power_on_time(hi_void)
{	
	POWER_FAILURE_CTRL_S *power_failure_ctrl_info = mrsGetPowerFailureCtrlInfo();

	return power_failure_ctrl_info->power_on_reset_time;
}
hi_void mrs_power_failure_set_equipmode_flag(hi_u8 sta_state)
{	
    POWER_FAILURE_CTRL_S *power_failure_ctrl_info = mrsGetPowerFailureCtrlInfo();
	power_failure_ctrl_info->sta_state = sta_state;
}
HI_VOID mrsHandleLocalPowerOn(HI_VOID)
{
    POWER_FAILURE_CTRL_S *powerFailureCtrlInfo = mrsGetPowerFailureCtrlInfo();
    POWER_FAILURE_RCV_MAC_REPORT_S *rcvMacInfo = mrsGetPowerFailureMacInfo();

	return;

    if (mrsToolsIsIICollector()) {
        mrsCltiiPowerOnproc();
        return;
    }

	// 工装模式下不进行停电上报
    if (powerFailureCtrlInfo->sta_state == STA_EQUIP_CHECK_MODE) {
		return;
	}

    powerFailureCtrlInfo->power_on_bitmap[rcvMacInfo->my_tei / BIT_WIZE] |= 1 << (rcvMacInfo->my_tei % BIT_WIZE);
    //HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_046, HI_DIAG_MT("MRS_LOCALPOWER_ON"), rcvMacInfo->my_tei);
	mrs_local_power_on_wait_send();
}

hi_u32 mrs_power_failure_handle_not_power_down(hi_void)
{	
    POWER_FAILURE_CTRL_S *power_failure_ctrl_info = mrsGetPowerFailureCtrlInfo();

	// 不重复处理电压未跌落事件上报， 启动5s定时器，定时器到期后，判断是否需要关闭停电功能
	if (power_failure_ctrl_info->is_rcv_power_down == HI_FALSE ) {
		
		HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_063, HI_DIAG_MT("rcv power not down"));
		power_failure_ctrl_info->is_rcv_power_down = HI_TRUE; 		
		MRS_StartTimer(MRS_STA_DISABLE_POWER_FAILURE_TIMER,  TSTA_CHECK_DISABLE_POWER_FAILURE_TIME * MRS_US_TO_S_MASK, HI_SYS_TIMER_ONESHOT);
	}

	return HI_ERR_SUCCESS;
}
#endif

/*****************************************************************************
 函 数 名  : mrsGetOthersPowerFailureFrameData
 功能描述  : 获取其他站点的停电上报帧的数据域的信息
 输入参数  : MRS_EVENT_INFO_STRU * pstRcvEvtInfo
             HI_U8 *pstRcvData
             HI_BOOL flag
 输出参数  : 无
 返 回 值  : HI_VOID
 本函数调用的函数  :
 调用本函数的函数  :
 访问的全局变量  :
 修改的全局变量  :
 修改的BBP寄存器列表  :

 修改历史      :
  1.日    期   : 2017年8月17日
    作    者   : test
    修改内容   : 新生成函数

*****************************************************************************/
HI_VOID mrsGetOthersPowerFailureFrameData(HI_IN MRS_EVENT_INFO_STRU * pstRcvEvtInfo, HI_U8 *pMac, HI_BOOL bLocalBcFlag)
{

    //HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1006, HI_DIAG_MT("rx clt up pw off plc data"), pstRcvEvtInfo->ucData, HI_MIN(pstRcvEvtInfo->ulDataLen,200));
	//HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_067, HI_DIAG_MT("[rx clt up pw off plc ulFunCode]"), pstRcvEvtInfo->ulFunCode);
	
    if (MRS_EVENT_FUN_REPORT_CLT == pstRcvEvtInfo->ulFunCode)   
    {
        mrsCltPowerOffOnPlcProc(pstRcvEvtInfo, pMac, bLocalBcFlag);
    }
    else if (MRS_EVENT_FUN_REPORT_MODUL == pstRcvEvtInfo->ulFunCode)
    {
        mrsGetStaOthersPowerFailureFrameData(pstRcvEvtInfo);
    }
}

HI_VOID mrsGetStaOthersPowerFailureFrameData(HI_IN MRS_EVENT_INFO_STRU * pstRcvEvtInfo)
{
    hi_u8 *refreshBitmap = HI_NULL;
    HI_U16 i = 0;
    HI_U16 *dataTei = 0;
    HI_U8 rcvTei[DATA_END_TEI_POSITION] = {0};
    HI_U16 teiTemp = 0;
	//Matteo
	HI_U8 addr[HI_METER_ADDR_LEN] = {0};
	
    POWER_FAILURE_CTRL_S *powerFailureCtrl = mrsGetPowerFailureCtrlInfo();
#if defined(PRODUCT_CFG_SUPPORT_MRS_DUOL_MODE_RF)&&defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
	POWER_FAILURE_RCV_MAC_REPORT_S *deviceInfo = mrsGetPowerFailureMacInfo();
#endif

    if (pstRcvEvtInfo->ucData[0] == STA_POWER_ON) {
        return;
    }

    refreshBitmap = (pstRcvEvtInfo->ucData[0] == STA_POWER_FAILURE) ? powerFailureCtrl->power_failure_bitmap :
                                                                                powerFailureCtrl->power_on_bitmap;
    //需要将tei转换为位图
    for(i = 0; i < DATA_END_TEI_POSITION; i++)
    {
        rcvTei[i] = pstRcvEvtInfo->ucData[i + 1];
    }
    //获取最小tei
    dataTei = (HI_U16*)rcvTei;

    //HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_066, HI_DIAG_MT("[PW god, check tei=%d]"),*dataTei, pstRcvEvtInfo->ucData[0]);
	HI_U8 ucPos = 0;
	HI_U8 ucBit =0;
	HI_U8 ucRecvPos = 0;
	HI_U8 ucRecvBit =0;
	HI_U16 usPosTemp=0;
	

	for(i = 0; i < (pstRcvEvtInfo->ulDataLen - DATA_FIX_SIZE)*BIT_WIZE; i++)
	{
		usPosTemp = i+(*dataTei);
		ucPos = (HI_U8)(usPosTemp / 8);
		ucBit = usPosTemp % 8;
		ucRecvPos = (HI_U8)(i / 8);
		ucRecvBit = i % 8;
#if defined(PRODUCT_CFG_SUPPORT_MRS_DUOL_MODE_RF)
#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
		if((0 == BIT_GET(powerFailureCtrl->power_failure_bitmap[ucPos],ucBit))     //weichao powerFailureBitmap-->powerFailureCtrl
			&&(1 == BIT_GET(pstRcvEvtInfo->ucData[DATA_FIX_SIZE+ucRecvPos],ucRecvBit)))
		{
			mrsRfSetNeedRfTransmitPowerInfo(3);
			if(HI_FALSE == deviceInfo->device_power_failure)
			{
				mrsRfSetStateInfo(2);
			}
			break;
		}
#endif
#endif
	}
    //将报文中的tei刷新到全局bitmap之中
    for(i = 0; i < (pstRcvEvtInfo->ulDataLen - DATA_FIX_SIZE) * BIT_WIZE; i++)
    {
        if(HI_TRUE == mrsPowerFailureBitTest(&pstRcvEvtInfo->ucData[DATA_FIX_SIZE],i))
        {
            teiTemp = (*dataTei) + i;
//Matteo   删掉II采无线发送的已有停电上报位图
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
			MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();
			if(HI_ND_TYPE_CLTII == pstCcoCtx->stTopoInf.entry[teiTemp-1].product_type)
			{	
				(hi_void)memcpy_s(addr, HI_PLC_MAC_ADDR_LEN, pstCcoCtx->stTopoInf.entry[teiTemp-1].mac,HI_PLC_MAC_ADDR_LEN);
				mrsHexInvert(addr, sizeof(addr));
				if (HI_TRUE == mrsCltIsExistPowerList(addr, MRS_CLT_POWER_OFF_TYPE)) 
				{
//					HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_008, HI_DIAG_MT("[Matteo] II poweroff repeat ignore "));
					HI_U8 ucPos = (HI_U8)(teiTemp / 8);
    				HI_U8 ucBit = teiTemp % 8;
    				BIT_RESET(refreshBitmap[ucPos], ucBit);
					continue;
				}
			}	
#endif
            mrsPowerFailureBitSet(refreshBitmap, teiTemp);
        }

    }

}

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
/*****************************************************************************
 函 数 名  : mrsHandleOthersPowerFailureFrame
 功能描述  : 处理其他模块的停电上报帧
 输出参数  : 无
 返 回 值  : HI_U32
 本函数调用的函数  :
 调用本函数的函数  :
 访问的全局变量  :
 修改的全局变量  :
 修改的BBP寄存器列表  :

 修改历史      :
  1.日    期   : 2017年8月17日
    作    者   : test
    修改内容   : 新生成函数

*****************************************************************************/
HI_U32 mrsHandleOthersPowerFailureFrame(HI_VOID)
{
    NV_APP_MRS_EVT_CFG_STRU *powerFailureNvCtrl = HI_NULL;
    HI_U32 ret = HI_ERR_SUCCESS;
    POWER_FAILURE_CTRL_S *powerFailureBitmap = mrsGetPowerFailureCtrlInfo();

    powerFailureNvCtrl = mrsGetPowerFailureNvCtrl();
    powerFailureBitmap->isRedayToSendPowerFailure = HI_TRUE;
    mrsCltPowerSetSendFlag(HI_TRUE);
    powerFailureBitmap->sta_send_times = 1;
    mrsDfxStaCurrPowerOffPlcTxTurn(powerFailureBitmap->sta_send_times);

    //HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_040, HI_DIAG_MT("[PCO-hdl oth pff=%d]"),powerFailureNvCtrl->ucStaConvergeTime);
    MRS_StartTimer(MRS_STA_TIMER_POWERFAILURE, powerFailureNvCtrl->ucStaConvergeTime * MRS_US_TO_S_MASK, HI_SYS_TIMER_ONESHOT);

    return ret;
}

/*****************************************************************************
 函 数 名  : mrsStaNormalBitmap
 功能描述  :将不标准的bitmap转换为标准bitmap
 输入参数  : 无
 输出参数  : 无
 返 回 值  :
 本函数调用的函数  :
 调用本函数的函数  :
 访问的全局变量  :
 修改的全局变量  :
 修改的BBP寄存器列表  :

 修改历史      :
  1.日    期   : 2017年8月19日
    作    者   : test
    修改内容   : 新生成函数

*****************************************************************************/
hi_void mrsStaNormalBitmap(hi_u8 *bitmapIn, hi_u8 *bitmapOut, hi_u16 bitmapLen)
{
    //传入的是需要转换的位图，和位图长度
    hi_u16 i = 0;
    hi_u16 j = 0;
    hi_u16 bitmapMinTei = 0;
    hi_u16 bitmapMaxTei = 0;

    //直接获取全局变量后获取对应范围内的tei的值
	bitmapMinTei = mrsCalcMinTei(bitmapIn);
    bitmapMaxTei = mrsCalcMaxTei(bitmapIn);

    //HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_052, HI_DIAG_MT("[GET_send_bitmap,maxtei]"),bitmapMaxTei);
    //HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_053, HI_DIAG_MT("[GET_send_bitmap,mintei]"),bitmapMinTei);

    for (i = bitmapMinTei, j = 0; i <= bitmapMaxTei && j < bitmapLen * BIT_WIZE; i++,j++)
    {
        //判断最小最大tei间哪些tei发生断电，在新的buf中将相关位置1
        if (HI_TRUE == mrsPowerFailureBitTest(bitmapIn, i))
        {
            mrsPowerFailureBitSet(bitmapOut, j);
        }
    }
}
/*****************************************************************************
 函 数 名  : mrsCalcMinTeiDataLen
 功能描述  : 计算最小tei以及数据长度
 输入参数  : HI_OUT HI_U16 *tei
             HI_OUT HI_U16 *dataLen
 输出参数  : 无
 返 回 值  : HI_VOID
 本函数调用的函数  :
 调用本函数的函数  :
 访问的全局变量  :
 修改的全局变量  :
 修改的BBP寄存器列表  :

 修改历史      :
  1.日    期   : 2017年8月18日
    作    者   : test
    修改内容   : 新生成函数

*****************************************************************************/
HI_U16 mrsCalcMinTei(HI_U8 *bitmapIn)
{
    HI_U16 i = 0;
    HI_U16 ret = 0;
    HI_U16 bitmapMinTei = 0;


    //计算最小tei,直接返回所在位置
    for(i = MIN_START_TEI; i < POWER_FAILURE_BITMAP_LEN * BIT_WIZE; i++)
    {
        if (HI_TRUE == mrsPowerFailureBitTest(bitmapIn, i))
        {
            bitmapMinTei = i;
            break;
        }

    }

    ret = bitmapMinTei;

    return ret;
}

/*****************************************************************************
 函 数 名  : mrsCalcMaxTei
 功能描述  : 计算最大tei以及数据长度
 输入参数  : HI_OUT HI_U16 *tei
             HI_OUT HI_U16 *dataLen
 输出参数  : 无
 返 回 值  : HI_VOID
 本函数调用的函数  :
 调用本函数的函数  :
 访问的全局变量  :
 修改的全局变量  :
 修改的BBP寄存器列表  :

 修改历史      :
  1.日    期   : 2017年8月18日
    作    者   : test
    修改内容   : 新生成函数

*****************************************************************************/
HI_U16 mrsCalcMaxTei(HI_U8 *bitmapIn)
{
    HI_U16 i = 0;
    HI_U16 ret = 0;
    HI_U16 bitmapMaxTei = 0;


    for(i = (POWER_FAILURE_BITMAP_LEN - 1) * BIT_WIZE; i >= 1; i--)
    {
        if (HI_TRUE == mrsPowerFailureBitTest(bitmapIn, i))
        {
            bitmapMaxTei = i;
            break;
        }
    }

    ret = bitmapMaxTei;
    return ret;
}

HI_U32 mrsStaPowerFailureFrameProc(HI_VOID)
{
    POWER_FAILURE_CTRL_S *powerFailureCtrlInfo = mrsGetPowerFailureCtrlInfo();
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U16 usMeterNum = mrsCltPowerOffGetMeterNum();
    POWER_FAILURE_RCV_MAC_REPORT_S *deviceStateInfo = mrsGetPowerFailureMacInfo();
    HI_PRV HI_BOOL bFirst = HI_FALSE;

    if (HI_TRUE == deviceStateInfo->device_power_failure)
    {
        if (HI_FALSE == bFirst)
        {
            mrsCltLocalPowerOffPreProc();
            bFirst = HI_TRUE;
        }

        if ((0 == usMeterNum) || (HI_TRUE == mrsCltPowerGetTurnEndFlag()))
        {
            powerFailureCtrlInfo->sta_send_times++;
            mrsDfxStaCurrPowerOffPlcTxTurn(powerFailureCtrlInfo->sta_send_times);
        }
    }

    if (!mrsToolsMemIsZero(powerFailureCtrlInfo->power_failure_bitmap, POWER_FAILURE_BITMAP_LEN)
        && (HI_TRUE == mrsCltPowerGetTurnEndFlag()))
    {
        ret = mrsStaCreatePowerFailureFrame(STA_POWER_FAILURE);
    }

    if (usMeterNum)
    {
        ret |= mrsCltSendPowerOffFrameProc();
    }

    return ret;
}

HI_U8 mrsPowerFailureGetCurrTurn(HI_VOID)
{
    POWER_FAILURE_CTRL_S *powerFailureCtrlInfo = mrsGetPowerFailureCtrlInfo();

    return powerFailureCtrlInfo->sta_send_times;
}
hi_void mrsPowerSetSendInfo(HI_U8 ucEventType, MRS_PLC_FRAME_DATA_S *pstPowerFrame)
{
    hi_u32 index = 0;
    POWER_FAILURE_CTRL_S *powerFailureCtrlInfo = mrsGetPowerFailureCtrlInfo();
    NV_APP_MRS_EVT_CFG_STRU *powerFailureNvCtrl = mrsGetPowerFailureNvCtrl();
    POWER_FAILURE_RCV_MAC_REPORT_S *deviceStateInfo = mrsGetPowerFailureMacInfo();

    // 上电上报类型或模块未停电时，单播发送
    if(ucEventType == STA_POWER_ON || deviceStateInfo->device_power_failure == HI_FALSE)
    {
        //本地模块未停电，不填充地址，不需要上下文,直接发送出去
        MRS_SendFrameProc(pstPowerFrame);
        MRS_StartTimer(MRS_STA_TIMER_WAIT_POWERFAILURE_ACK, powerFailureNvCtrl->usSendInterval ,HI_SYS_TIMER_ONESHOT);
        powerFailureCtrlInfo->ucWaitAckFlag = HI_TRUE;
        powerFailureCtrlInfo->ucWaitAckType = ucEventType;

        //STA发送完成后,接受到ack就擦除全部数据
        //HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_055, HI_DIAG_MT("[PCO-MR-PW-sendover]"), powerFailureNvCtrl->usSendInterval);

        return;
    }

    //如果本模块停电，填充地址为本地广播地址
    pstPowerFrame->ucMac[index] = 0;
    (hi_void)memset_s(pstPowerFrame->ucMac + 1, HI_PLC_MAC_ADDR_LEN - 1, 0xFF, HI_PLC_MAC_ADDR_LEN - 1);

    //填充信息后发送,多次发送，在内部将全局变量清零
    mrsStaSendPowerFailureFrame(pstPowerFrame);
}

/*****************************************************************************
 函 数 名  : mrsCreatePowerFailureFrame
 功能描述  : 创建停电上报帧
 输入参数  : HI_U16 tei
 输出参数  : 无
 返 回 值  : HI_U32
 本函数调用的函数  :
 调用本函数的函数  :
 访问的全局变量  :
 修改的全局变量  :
 修改的BBP寄存器列表  :

 修改历史      :
  1.日    期   : 2017年8月17日
    作    者   : test
    修改内容   : 新生成函数

*****************************************************************************/
HI_U32 mrsStaCreatePowerFailureFrame(HI_U8 ucEventType)
{
    HI_U32 ret = HI_ERR_FAILURE;
    HI_U16 usPayloadLen = 0;
    HI_U16 pwBitmapMinTei = 0;
    HI_U16 pwBitmapMaxTei = 0;
    HI_U16 usBitmapLen = 0;
    HI_U8 *teiPoint = HI_NULL;
    HI_U8 *pucPayload = HI_NULL;
    HI_U8 *bitmapIn = HI_NULL;
    HI_U8 index = 0;
    MRS_EVENT_INFO_STRU *pstEvtInfo = HI_NULL;
    MRS_PLC_FRAME_DATA_S pstPowerFrame = {0};
    P_MRS_STA_EVT_CTX pstCtx = mrsStaGetEvtCtx();
	MRS_STA_SRV_CTX_STRU *sta_srv_ctx_info = mrsStaGetContext();
    POWER_FAILURE_CTRL_S *powerFailureCtrlInfo = mrsGetPowerFailureCtrlInfo();

    if (HI_NULL == pstCtx)
    {
        return HI_ERR_NO_INITILIZATION;
    }

    bitmapIn = (ucEventType == STA_POWER_FAILURE) ? powerFailureCtrlInfo->power_failure_bitmap :
                                                               powerFailureCtrlInfo->power_on_bitmap;
    //计算最小,最大tei，数据长度
    pwBitmapMinTei = mrsCalcMinTei(bitmapIn);
    pwBitmapMaxTei = mrsCalcMaxTei(bitmapIn);

	//Matteo
    HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_050, HI_DIAG_MT("[PCO-tei-range]"), pwBitmapMinTei, pwBitmapMaxTei);

    usBitmapLen = (pwBitmapMaxTei - pwBitmapMinTei + BIT_WIZE) / BIT_WIZE;

    teiPoint = (HI_U8*)(&pwBitmapMinTei);

    usPayloadLen = sizeof(MRS_EVENT_INFO_STRU) + DATA_FIX_SIZE + usBitmapLen;
    pucPayload = (HI_U8*)mrsToolsMalloc(usPayloadLen);
    if(HI_NULL == pucPayload)
    {
        //HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_009, HI_DIAG_MT("Malloc failed"));
        return HI_ERR_MALLOC_FAILUE;
    }
    (hi_void)memset_s(pucPayload, usPayloadLen, 0, usPayloadLen);

    //填充帧中的固定部分
    pstEvtInfo = (MRS_EVENT_INFO_STRU *)(pucPayload);
    pstEvtInfo->ulInterfaceVer = MRS_EVENT_INF_UP_VER;
    pstEvtInfo->ulStructLen = sizeof(MRS_EVENT_INFO_STRU);
    pstEvtInfo->ulDirBit = MRS_PLC_UP_FLG;
    pstEvtInfo->ulFrmBit = MRS_EVENT_FRM_BIT_FROM;
    pstEvtInfo->ulFunCode = MRS_EVENT_FUN_REPORT_MODUL;
    pstEvtInfo->ulDataLen = DATA_FIX_SIZE + usBitmapLen;
	(hi_void)memcpy_s(pstEvtInfo->ucMeter, HI_PLC_MAC_ADDR_LEN, sta_srv_ctx_info->aucMeter, HI_PLC_MAC_ADDR_LEN);

    pstCtx->usSeq++;

    mrsDfxStaEvtReportSeq(pstCtx->usSeq);
    pstEvtInfo->usSeq = pstCtx->usSeq;
    powerFailureCtrlInfo->usCurrSeq = pstCtx->usSeq;

    //可变区域内容拷入
    pstEvtInfo->ucData[index] = ucEventType;
    (hi_void)memcpy_s(&pstEvtInfo->ucData[DATA_START_TEI_POSITION], sizeof(HI_U8), teiPoint, sizeof(HI_U8));
    (hi_void)memcpy_s(&pstEvtInfo->ucData[DATA_END_TEI_POSITION], sizeof(HI_U8), teiPoint + 1, sizeof(HI_U8));

    mrsStaNormalBitmap(bitmapIn,&pstEvtInfo->ucData[DATA_FIX_SIZE], usBitmapLen);

    //HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_054, HI_DIAG_MT("[PCO-MR-PW=%d]"),pstEvtInfo->ulDataLen);
    //HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1000, HI_DIAG_MT("[PCO-MR]-PW-senddata"), pstEvtInfo->ucData, pstEvtInfo->ulDataLen);

    //根据此id选择处理函数
    pstPowerFrame.usId = ID_MRS_CMD_EVENT_INF_PLC;
    pstPowerFrame.pucPayload = pucPayload;
    pstPowerFrame.usPayloadLen = usPayloadLen;

    mrsPowerSetSendInfo(ucEventType, &pstPowerFrame);
    mrsToolsFree(pucPayload);

    return ret;
}

// 处理等待ack定时器到期函数
HI_U32 mrsStaHandleWaitPowerFailureAck(HI_VOID)
{
    POWER_FAILURE_RCV_MAC_REPORT_S *deviceStateInfo = mrsGetPowerFailureMacInfo();
    POWER_FAILURE_CTRL_S *powerFailureCtrlInfo = mrsGetPowerFailureCtrlInfo();
    NV_APP_MRS_EVT_CFG_STRU *powerFailureNvCtrl = mrsGetPowerFailureNvCtrl();

    powerFailureCtrlInfo->ucSendReportTimes++;

    // 模块停电，不再处理ack
    if(HI_TRUE == deviceStateInfo->device_power_failure)
    {
        powerFailureCtrlInfo->ucWaitAckFlag= HI_FALSE;
        return HI_ERR_SUCCESS;
    }

    if(HI_FALSE == powerFailureCtrlInfo->ucWaitAckFlag || powerFailureCtrlInfo->ucSendReportTimes == (powerFailureNvCtrl->ucSendTimes + STA_DEFAULT_SEND_POWER_ON_CNT))
    {
        mrsStaPowerFailureSendEnd(powerFailureCtrlInfo);
        return HI_ERR_SUCCESS;
    }

    // 未能等到ack，需要再次发送。
    mrsStaCreatePowerFailureFrame(powerFailureCtrlInfo->ucWaitAckType);

    return HI_ERR_FAILURE;
}


/*****************************************************************************
 函 数 名  : mrsStaSendEvtReportPowerFailure
 功能描述  : 发送停电上报报文
 输出参数  : 无
 返 回 值  : HI_U32
 本函数调用的函数  :
 调用本函数的函数  :
 访问的全局变量  :
 修改的全局变量  :
 修改的BBP寄存器列表  :

 修改历史      :
  1.日    期   : 2017年8月17日
    作    者   : test
    修改内容   : 新生成函数

*****************************************************************************/
HI_U32 mrsStaSendPowerFailureFrame(MRS_PLC_FRAME_DATA_S *pstReportData)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    NV_APP_MRS_EVT_CFG_STRU *powerFailureNvCtrl = HI_NULL;
    POWER_FAILURE_CTRL_S *powerFailureCtrlInfo = HI_NULL;

    //只管发送
    powerFailureNvCtrl = mrsGetPowerFailureNvCtrl();
    powerFailureCtrlInfo = mrsGetPowerFailureCtrlInfo();

    if (!pstReportData)
    {
        return HI_ERR_BAD_DATA;
    }

    ret = MRS_SendFrameProc(pstReportData);

    //HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_032, HI_DIAG_MT("PW_FAILURE_STA,Sendtime,interval")
                        //,powerFailureCtrlInfo->sta_send_times,powerFailureNvCtrl->usSendInterval);

    //发送成功后等待时间间隔后重新继续发送
    if(powerFailureCtrlInfo->sta_send_times <= powerFailureNvCtrl->ucSendTimes)
    {
         MRS_StartTimer(MRS_STA_TIMER_POWERFAILURE, powerFailureNvCtrl->usSendInterval ,HI_SYS_TIMER_ONESHOT);
    }
    else
    {
#if defined(PRODUCT_CFG_SUPPORT_MRS_DUOL_MODE_RF)
    	mrsRfClearPowerState();
#endif
        //发送次数足够后，擦除bitmap信息
        //HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_045, HI_DIAG_MT("POWER_FAILURE_SendTime_ENOUGH"));
        mrsStaPowerFailureSendEnd(powerFailureCtrlInfo);
    }
    mrsDfxStaPlcEvtReportTimeStamp(HI_MDM_GetMilliSeconds());

    return ret;
}

HI_VOID mrsStaPowerFailureSendEnd(POWER_FAILURE_CTRL_S* pstPowerFailureCtrl)
{
    HI_U8 sendTurn = pstPowerFailureCtrl->sta_send_times;

    //发送次数足够后，擦除bitmap信息
    (hi_void)memset_s(pstPowerFailureCtrl, sizeof(POWER_FAILURE_CTRL_S), 0, sizeof(POWER_FAILURE_CTRL_S));
	if (mrsCltPowerOffGetMeterNum()) {		
		pstPowerFailureCtrl->sta_send_times = sendTurn;
	}
}

// 站点处理ack
HI_U32 mrsPowerFailureStaRcvAck(HI_U16 usSeq)
{
    POWER_FAILURE_CTRL_S *powerFailureCtrlInfo = mrsGetPowerFailureCtrlInfo();

    if (HI_TRUE != powerFailureCtrlInfo->ucWaitAckFlag)
    {
        return HI_ERR_SKIP;
    }

    //HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_060, HI_DIAG_MT("[PCO-MR-PW,in rcv ack]"),usSeq,powerFailureCtrlInfo->usCurrSeq);

    // 收到ack 后清除标记位
    if(powerFailureCtrlInfo->usCurrSeq == usSeq)
    {
        powerFailureCtrlInfo->ucSendReportTimes = 0;
        powerFailureCtrlInfo->ucWaitAckFlag = HI_FALSE;
        return HI_ERR_SUCCESS;
    }

    return HI_ERR_FAILURE;
}
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)

// cco是否需要回复ack
HI_BOOL mrsCcoNotReplyAck(HI_U8 ucRcvMac[HI_PLC_MAC_ADDR_LEN], HI_U8 eventType)
{
    HI_U16 i = 0;
    HI_U16 usCheckTei = 0;
    HI_MAC_NETWORK_TOPO_ENTRY_S *topo_entry = HI_NULL;
    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();
    POWER_FAILURE_CTRL_S *powerFailureCtrlInfo = mrsGetPowerFailureCtrlInfo();

    // 复电事件需要回复ack
    if(eventType == STA_POWER_ON)
    {
        return HI_FALSE;
    }

    mrsRefreshTopoInfo(pstCcoCtx);
    topo_entry = pstCcoCtx->stTopoInf.entry;

    for(i = 0; i < HI_MAC_NETWORK_TOPO_ENTRY_MAX; i++)
    {
        if(memcmp(topo_entry[i].mac, ucRcvMac, HI_PLC_MAC_ADDR_LEN) == 0)
        {
            usCheckTei = topo_entry[i].tei;
            break;
        }
    }

    // 刷新bitmap 后再进行查看
    return mrsPowerFailureBitTest(powerFailureCtrlInfo->power_failure_bitmap, usCheckTei);
}

// cco创建回复ack帧
HI_VOID mrsCcoCreatePowerFailureRspAck(MRS_EVENT_INFO_STRU* pstEvtTransmit, HI_U8 ucRcvMac[HI_PLC_MAC_ADDR_LEN])
{
    // 创建回复帧给该站点
    mrsCcoEvtSendToSta(pstEvtTransmit, ucRcvMac, MRS_EVENT_FUN_ACK);
}
/*****************************************************************************
 函 数 名  : mrsGetReportMacAddr
 功能描述  : 获取停电tei的mac地址
 输入参数  : HI_U16 powerFailureTei
             HI_U8 *macAddrSave
 输出参数  : 无
 返 回 值  : HI_U32
 本函数调用的函数  :
 调用本函数的函数  :
 访问的全局变量  :
 修改的全局变量  :
 修改的BBP寄存器列表  :

 修改历史      :
  1.日    期   : 2017年8月20日
    作    者   : test
    修改内容   : 新生成函数

*****************************************************************************/
HI_VOID mrsCcoReportMacAddr(HI_U8 *bitmapIn, HI_U8 *macAddr, HI_U32 macAllocLen,
                                    HI_IN HI_U16 startPos, HI_OUT HI_U16 *nextStartPos)
{
    HI_U16 i = 0, j = 0;
    HI_U16 pwBitmapMaxTei = 0;
    HI_U16 queryIndex = 0;
    HI_U8 cpyMacAddrCnt = 0;
    HI_U8 validMacAddr[HI_PLC_MAC_ADDR_LEN] = {0};
    HI_MAC_NETWORK_TOPO_ENTRY_S *topo_entry = HI_NULL;

    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();

    pwBitmapMaxTei = mrsCcoCalcMaxTei(bitmapIn);

    mrsRefreshTopoInfo(pstCcoCtx);
    topo_entry = pstCcoCtx->stTopoInf.entry;

    for(i = startPos; (i <= pwBitmapMaxTei) && (cpyMacAddrCnt < macAllocLen); i++)
    {
        //对最大，最小tei之间的tei进行检测
        if (HI_FALSE == mrsPowerFailureBitTest(bitmapIn, i))
        {
            continue;
        }

        for(j = queryIndex; j < HI_MAC_NETWORK_TOPO_ENTRY_MAX; j++)
        {
            //mac地址有效性判断在计算tei cnt 时候完成，保证空间分配的准确性
            if(topo_entry[j].tei == i)
            {
                //此处需要进行00，fe的转换
                if(0xFE == topo_entry[j].mac[HI_PLC_MAC_ADDR_LEN - 1])
                {
                    (hi_void)memcpy_s(validMacAddr, HI_PLC_MAC_ADDR_LEN, topo_entry[j].mac, HI_PLC_MAC_ADDR_LEN);
                    MRS_TOOLS_FE_TO_00(validMacAddr[HI_PLC_MAC_ADDR_LEN - 1]);
                    (hi_void)memcpy_s(macAddr + cpyMacAddrCnt, HI_PLC_MAC_ADDR_LEN, validMacAddr, HI_PLC_MAC_ADDR_LEN);
                }
                else
                {
                    (hi_void)memcpy_s(macAddr + cpyMacAddrCnt, HI_PLC_MAC_ADDR_LEN, topo_entry[j].mac, HI_PLC_MAC_ADDR_LEN);
                }

                queryIndex = j + 1;
                mrsHexInvert(macAddr + cpyMacAddrCnt, HI_PLC_MAC_ADDR_LEN);
                cpyMacAddrCnt += HI_PLC_MAC_ADDR_LEN;
                //已经上报的最后tei的位置
                *nextStartPos = i;
                break;
            }
        }
    }

    //tei位置后移一位
    *nextStartPos += 1;
}
/*****************************************************************************
 函 数 名  : mrsCcoGetBitmapTeiCnt
 功能描述  : 获取bitmap中的tei个数
 输入参数  : 无
 输出参数  : 无
 返 回 值  : HI_U32
 本函数调用的函数  :
 调用本函数的函数  :
 访问的全局变量  :
 修改的全局变量  :
 修改的BBP寄存器列表  :

 修改历史      :
  1.日    期   : 2017年8月19日
    作    者   : test
    修改内容   : 新生成函数

*****************************************************************************/
HI_U32 mrsCcoGetBitmapTeiCnt(HI_U16 bitmapMinTei, HI_U16 bitmapMaxTei, hi_u8 *bitmapIn)
{
    HI_U32 ret = 0;
    HI_U16 i = 0, j = 0;
    HI_U16 queryIndex = 0;
    HI_U8 invalidMacAddr[HI_PLC_MAC_ADDR_LEN] = {0};
    HI_MAC_NETWORK_TOPO_ENTRY_S *topo_entry = HI_NULL;
    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();

    mrsRefreshTopoInfo(pstCcoCtx);
    topo_entry = pstCcoCtx->stTopoInf.entry;

    //做mac地址的检查，保证准确性
    for(i = bitmapMinTei; i <= bitmapMaxTei; i++)
    {
        if (HI_FALSE == mrsPowerFailureBitTest(bitmapIn, i))
        {
            continue;
        }

        for(j = queryIndex; j < HI_MAC_NETWORK_TOPO_ENTRY_MAX; j++)
        {
            //增加对mac地址有效性的判断
            if(topo_entry[j].tei == i && (0 != memcmp(topo_entry[j].mac, invalidMacAddr, HI_PLC_MAC_ADDR_LEN)))
            {
                ret++;
                queryIndex = j + 1;
                break;
            }
        }

    }

    return ret;
}


/*****************************************************************************
 函 数 名  : mrsCCOCalcMaxTei
 功能描述  : 计算最大tei以及数据长度
 输入参数  : HI_OUT HI_U16 *tei
             HI_OUT HI_U16 *dataLen
 输出参数  : 无
 返 回 值  : HI_VOID
 本函数调用的函数  :
 调用本函数的函数  :
 访问的全局变量  :
 修改的全局变量  :
 修改的BBP寄存器列表  :

 修改历史      :
  1.日    期   : 2017年8月18日
    作    者   : test
    修改内容   : 新生成函数

*****************************************************************************/
HI_U16 mrsCcoCalcMaxTei(hi_u8 *bitmapIn)
{
    HI_U16 i = 0;
    HI_U16 bitmapMaxTei = 0;

    //计算最小tei以及data数据域中的数据所占空间的大小
    for(i = (POWER_FAILURE_BITMAP_LEN - 1) * BIT_WIZE; i >= 1; i--)
    {
        if (HI_TRUE == mrsPowerFailureBitTest(bitmapIn, i))
        {
            bitmapMaxTei = i;
            break;
        }
    }

    return bitmapMaxTei;
}
/*****************************************************************************
 函 数 名  : mrsCalcMinTeiDataLen
 功能描述  : 计算最小tei以及数据长度
 输入参数  : HI_OUT HI_U16 *tei
             HI_OUT HI_U16 *dataLen
 输出参数  : 无
 返 回 值  : HI_VOID
 本函数调用的函数  :
 调用本函数的函数  :
 访问的全局变量  :
 修改的全局变量  :
 修改的BBP寄存器列表  :

 修改历史      :
  1.日    期   : 2017年8月18日
    作    者   : test
    修改内容   : 新生成函数

*****************************************************************************/
HI_U16 mrsCcoCalcMinTei(hi_u8 *bitmapIn)
{
    HI_U16 i = 0;
    HI_U16 ret = 0;
    HI_U16 bitmapMinTei = 0;

    //计算最小tei,直接返回所在位置
    for(i = MIN_START_TEI; i < POWER_FAILURE_BITMAP_LEN * BIT_WIZE; i++)
    {
        if (HI_TRUE == mrsPowerFailureBitTest(bitmapIn, i))
        {
            bitmapMinTei = i;
            break;
        }
    }

    ret = bitmapMinTei;

    return ret;
}


/*****************************************************************************
 函 数 名  : mrsCcoCreateReportFrame
 功能描述  : cco响应定时器，创建上报报文
 输出参数  : 无
 返 回 值  : HI_U32
 本函数调用的函数  :
 调用本函数的函数  :
 访问的全局变量  :
 修改的全局变量  :
 修改的BBP寄存器列表  :

 修改历史      :
  1.日    期   : 2017年8月19日
    作    者   : test
    修改内容   : 新生成函数

*****************************************************************************/
HI_U32 mrsCcoCreateReportFrame(HI_PBYTE *ppBuf, HI_U16 *usBufLen, HI_U16 *startTeiPos,
                                       PowerEventReportInfo *powerEventReportInfo)
{

    HI_U32 ulRet = HI_ERR_SUCCESS;
    MRS_CCO_SRV_CTX_STRU *pstCtx = mrsCcoGetContext();
    HI_PBYTE pucData = HI_NULL;
    HI_U16 usDataLen = 0;
    PROTO376_2_FRAME_PRE_STRU stFrame;
    POWER_FAILURE_CTRL_S *ccoReportCtrl = HI_NULL;
    HI_U16 pwRepDataLen = 0;
    HI_U16 pwBitmapMaxTei = 0;
    HI_U32 bitmapTeiCnt = 0;
    HI_U16 nextStartTeiPos = 0;
    HI_U8 *bitmapIn = HI_NULL;

    ccoReportCtrl = mrsGetPowerFailureCtrlInfo();

    bitmapIn = (powerEventReportInfo->eventType == STA_POWER_FAILURE) ? ccoReportCtrl->cco_diff_bitmap :
                                ccoReportCtrl->cco_power_on_diff_bitmap;

    pwBitmapMaxTei = mrsCcoCalcMaxTei(bitmapIn);

    //获取位图中的tei信息
    bitmapTeiCnt = mrsCcoGetBitmapTeiCnt(*startTeiPos, pwBitmapMaxTei, bitmapIn);

    //放入队列中的数据长度,超过最大数据长度时，需要多次上报
    if(bitmapTeiCnt > MAX_REPORT_TEI_CNT)
    {
        pwRepDataLen = MAX_REPORT_TEI_CNT * HI_PLC_MAC_ADDR_LEN;
    }
    else
    {
        pwRepDataLen = (HI_U16)(bitmapTeiCnt * HI_PLC_MAC_ADDR_LEN);
    }

    do
    {
        //填充可变部分
        // 设备类型(1) + 规约类型(1) + 报文长度(1) + 报文(pstData->data_len)
        usDataLen = DATA_FIX_SIZE + 1 + pwRepDataLen ;
        pucData = (HI_U8*)mrsToolsMalloc(usDataLen);
        if (!pucData)
        {
            ulRet = HI_ERR_MALLOC_FAILUE;
            break;
        }

        //填充固定部分
        pucData[0] = powerEventReportInfo->deviceType; // 设备类型
        pucData[1] = MRS_POWER_FAILURE_COMMUNICATE_TYPE;   //MRS_DEVICE_TYPE_COLLECTOR; // 规约类型
        pucData[2] = (HI_U8)pwRepDataLen + 1;

        //填充可变部分,通信单元事件类型,根据tei获取mac地址
        pucData[DATA_FIX_SIZE] = powerEventReportInfo->eventType;
        mrsCcoReportMacAddr(bitmapIn, pucData + DATA_FIX_SIZE + 1,
                            pwRepDataLen, *startTeiPos, &nextStartTeiPos);

        //HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_031, HI_DIAG_MT("cco report evt datalen"),pwRepDataLen);
        //HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1001, HI_DIAG_MT("cco report evt,mac_addr"),pucData + DATA_FIX_SIZE + 1,pwRepDataLen);

        //下次开始的tei位置
        *startTeiPos = nextStartTeiPos;

        (hi_void)memset_s(&stFrame, sizeof(stFrame), 0, sizeof(stFrame));
        stFrame.ucPrm = 1;
        stFrame.ucModuleFlag = 0;
        stFrame.ucIs645 = 0;
        stFrame.ucSeq = ++(pstCtx->ucSeq);

        (hi_void)memcpy_s(stFrame.ucDstAdd, sizeof(stFrame.ucDstAdd), pstCtx->ucMainNodeAddr, HI_METER_ADDR_LEN);
        mrsHexInvert(stFrame.ucDstAdd, sizeof(stFrame.ucDstAdd));

        stFrame.stAfnData.ucAFN = MRS_AFN(0x06);
        stFrame.stAfnData.ucDT[0] = MRS_AFN_FN_LO(5);
        stFrame.stAfnData.ucDT[1] = MRS_AFN_FN_HI(5);
        stFrame.stAfnData.pData = pucData;
        stFrame.stAfnData.usDataLen = usDataLen;

        ulRet = mrsCreate3762Frame(&stFrame, ppBuf, usBufLen, HI_NULL);
        if (HI_ERR_SUCCESS != ulRet)
        {
            break;
        }
    } while (0);

    mrsToolsFree(pucData);

    return ulRet;
}
hi_u32 mrsCcoCreateRspPowerFrame(hi_u8 eventType, hi_u8 deviceType)
{
    hi_u32 ret = HI_ERR_FAILURE;
    hi_u8 *pucUpData = HI_NULL;
    hi_u16 usDataLen = 0;
    hi_u32 i = 0;
    hi_u32 pk_num = 0;
    hi_u16 pwBitmapMinTei = 0;
    hi_u16 pwBitmapMaxTei = 0;
    hi_u32 bitmapTeiCnt = 0;
    hi_u16 startTei = 0;
    hi_u8 *bitmapIn = HI_NULL;
    P_MRS_EVT_QUEUE_ITEM pstEvtItem = HI_NULL;
    P_MRS_CCO_EVT_CTX_STRU pstEvtCtx = mrsGetCcoEvtCtx();
    POWER_FAILURE_CTRL_S *powerFailureCtrlInfo = mrsGetPowerFailureCtrlInfo();
    PowerEventReportInfo powerEventReportInfo;

	memset_s(&powerEventReportInfo, sizeof(PowerEventReportInfo),0, sizeof(PowerEventReportInfo));

    //获取要发送的bitmap
    mrsCcoReportBitmap(eventType);

    //CCO端计算diffbitmap中的最大值和最小值
    bitmapIn = (eventType == STA_POWER_FAILURE) ? powerFailureCtrlInfo->cco_diff_bitmap :
                                powerFailureCtrlInfo->cco_power_on_diff_bitmap;
    pwBitmapMinTei = mrsCcoCalcMinTei(bitmapIn);
    pwBitmapMaxTei = mrsCcoCalcMaxTei(bitmapIn);

    HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_060, HI_DIAG_MT("cco pwevt min max tei"), pwBitmapMinTei,pwBitmapMaxTei);

    bitmapTeiCnt = mrsCcoGetBitmapTeiCnt(pwBitmapMinTei, pwBitmapMaxTei, bitmapIn);
    startTei = pwBitmapMinTei;
    powerEventReportInfo.deviceType = deviceType;
    powerEventReportInfo.eventType = eventType;

    pk_num = (bitmapTeiCnt + MAX_REPORT_TEI_CNT - 1) / MAX_REPORT_TEI_CNT;

    for(i = 0; i < pk_num; i++)
    {
        ret = mrsCcoCreateReportFrame(&pucUpData, &usDataLen, &startTei, &powerEventReportInfo);

        if(HI_ERR_SUCCESS != ret)
        {
            break;
        }
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_059, HI_DIAG_MT("cco pwevt assemble ok,join evt queue"));
        pstEvtItem = (P_MRS_EVT_QUEUE_ITEM)mrsToolsMalloc(sizeof(MRS_EVT_QUEUE_ITEM) + usDataLen);
        if (!pstEvtItem)
        {
            mrsToolsFree(pucUpData);
            return HI_ERR_NO_MORE_MEMORY;
        }

        (hi_void)memset_s(pstEvtItem, sizeof(MRS_EVT_QUEUE_ITEM) + usDataLen, 0, sizeof(MRS_EVT_QUEUE_ITEM) + usDataLen);

        pstEvtItem->usDataLen = usDataLen;
        (hi_void)memcpy_s(pstEvtItem->pucData, pstEvtItem->usDataLen, pucUpData, usDataLen);

        mrsCcoEvtEnQueue(&pstEvtCtx->stEvtQueue, pstEvtItem);
        pstEvtCtx->stEvtQueue.usItemNum++;

        mrsToolsFree(pucUpData);
    }

    pstEvtCtx->bACDFlg = HI_TRUE;
    mrsActiveEvtReport();

    mrsDfxCcoEvtQueueItemNum(pstEvtCtx->stEvtQueue.usItemNum);
    mrsDfxCcoEvtFlag(pstEvtCtx->bACDFlg);

    //维测信息
    mrsPowerFailureDfxInfo();
  
    return HI_ERR_SUCCESS;
}
/*****************************************************************************
 函 数 名  : mrsCcoRspPowerFailureEvt
 功能描述  : cco响应事件上报定时器
 输入参数  : HI_VOID
 输出参数  : 无
 返 回 值  : HI_VOID
 本函数调用的函数  :
 调用本函数的函数  :
 访问的全局变量  :
 修改的全局变量  :
 修改的BBP寄存器列表  :

 修改历史      :
  1.日    期   : 2017年8月19日
    作    者   : test
    修改内容   : 新生成函数

*****************************************************************************/
hi_u32 mrsCcoRspPowerFailureEvt(hi_void)
{
    POWER_FAILURE_CTRL_S *powerFailureCtrlInfo = mrsGetPowerFailureCtrlInfo();
	
    NV_APP_MRS_EVT_CFG_STRU *powerFailureNvCtrl = mrsGetPowerFailureNvCtrl();

    HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_042, HI_DIAG_MT("cco rsp power failure"),powerFailureCtrlInfo->isRedayToSendPowerOn);

    mrsCcoCreateRspPowerFrame(STA_POWER_FAILURE, MRS_DEVICE_TYPE_BROAD_CARRIER_COMM_UNIT);
    powerFailureCtrlInfo->isRedayToSendPowerFailure = HI_FALSE;
	
	//擦除差异bitmap，开放接收标识
	(hi_void)memset_s(powerFailureCtrlInfo->cco_diff_bitmap, POWER_FAILURE_BITMAP_LEN, 0, POWER_FAILURE_BITMAP_LEN);

	if(HI_FALSE == powerFailureCtrlInfo->cco_erease_bitmap_flag)
	{
		//起定时器4min 后擦除数据
		HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_007, HI_DIAG_MT("cco begin erase data"), powerFailureNvCtrl->ucCcoConvergeTime * MRS_US_TO_S_MASK * CCO_ERASE_BITMAP);
		MRS_StartTimer(MRS_CCO_TIMER_POWERFAILURE_ERASE_DATA, powerFailureNvCtrl->ucCcoConvergeTime * MRS_US_TO_S_MASK * CCO_ERASE_BITMAP, HI_SYS_TIMER_ONESHOT);
		powerFailureCtrlInfo->cco_erease_bitmap_flag = HI_TRUE;
	}

    return HI_ERR_SUCCESS;
}

hi_u32 mrsCcoRspPowerOnEvt(hi_void)
{
    POWER_FAILURE_CTRL_S *powerFailureCtrlInfo = mrsGetPowerFailureCtrlInfo();
    NV_APP_MRS_EVT_CFG_STRU *powerFailureNvCtrl = mrsGetPowerFailureNvCtrl();

    HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_041, HI_DIAG_MT("cco rsp power on"),powerFailureCtrlInfo->isRedayToSendPowerOn);
    mrsCcoCreateRspPowerFrame(STA_POWER_ON, MRS_DEVICE_TYPE_BROAD_CARRIER_COMM_UNIT);
    powerFailureCtrlInfo->isRedayToSendPowerOn = HI_FALSE;
	
	(hi_void)memset_s(powerFailureCtrlInfo->cco_power_on_diff_bitmap, POWER_FAILURE_BITMAP_LEN, 0, POWER_FAILURE_BITMAP_LEN);
	
	if(HI_FALSE == powerFailureCtrlInfo->cco_erease_power_on_bitmap_flag)
	{
		//起定时器4min 后擦除数据
		HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_007, HI_DIAG_MT("cco begin erase power on data"), powerFailureNvCtrl->ucCcoConvergeTime * MRS_US_TO_S_MASK * CCO_ERASE_BITMAP);
		MRS_StartTimer(MRS_CCO_TIMER_POWERON_ERASE_DATA, powerFailureNvCtrl->ucCcoConvergeTime * MRS_US_TO_S_MASK * CCO_ERASE_BITMAP, HI_SYS_TIMER_ONESHOT);
		powerFailureCtrlInfo->cco_erease_power_on_bitmap_flag = HI_TRUE;
	}

    return HI_ERR_SUCCESS;
}


/*****************************************************************************
 函 数 名  : mrsEraseBitmap
 功能描述  : cco擦除数据
 输出参数  : 无
 返 回 值  : HI_VOID
 本函数调用的函数  :
 调用本函数的函数  :
 访问的全局变量  :
 修改的全局变量  :
 修改的BBP寄存器列表  :

 修改历史      :
  1.日    期   : 2017年9月15日
    作    者   : test
    修改内容   : 新生成函数

*****************************************************************************/
HI_VOID mrsEraseBitmap(HI_VOID)
{
    POWER_FAILURE_CTRL_S *powerFailureInfo = mrsGetPowerFailureCtrlInfo();
    HI_U8 ccoTempDiffBitmap[POWER_FAILURE_BITMAP_LEN]= {0};

    HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_004, HI_DIAG_MT("cco erase data"));

    //先获取差异数据
    mrsCcoReportBitmap(STA_POWER_FAILURE);
    (hi_void)memcpy_s(ccoTempDiffBitmap,POWER_FAILURE_BITMAP_LEN, powerFailureInfo->cco_diff_bitmap, POWER_FAILURE_BITMAP_LEN);

    //上报数据时间间隔满足90s后,擦除全局变量中的数据
    memset_s(powerFailureInfo->power_failure_bitmap, POWER_FAILURE_BITMAP_LEN, 0,POWER_FAILURE_BITMAP_LEN);
    memset_s(powerFailureInfo->cco_diff_bitmap, POWER_FAILURE_BITMAP_LEN, 0,POWER_FAILURE_BITMAP_LEN);
    memset_s(powerFailureInfo->cco_last_send_bitmap, POWER_FAILURE_BITMAP_LEN, 0,POWER_FAILURE_BITMAP_LEN);
    
    powerFailureInfo->cco_wait_erase = HI_FALSE;
    powerFailureInfo->cco_erease_bitmap_flag = HI_FALSE;

    //考入数据到全局变量中
    (hi_void)memcpy_s(powerFailureInfo->power_failure_bitmap,POWER_FAILURE_BITMAP_LEN, ccoTempDiffBitmap, POWER_FAILURE_BITMAP_LEN);
    (hi_void)memcpy_s(powerFailureInfo->cco_diff_bitmap,POWER_FAILURE_BITMAP_LEN, ccoTempDiffBitmap, POWER_FAILURE_BITMAP_LEN);

}

hi_void mrs_cco_erase_power_on(hi_void)
{
	POWER_FAILURE_CTRL_S *power_failure_info = mrsGetPowerFailureCtrlInfo();
	HI_U8 cco_temp_diff_bitmap[POWER_FAILURE_BITMAP_LEN]= {0};	
	hi_u32 cur_time = HI_MDM_GetSeconds();	
    NV_APP_MRS_EVT_CFG_STRU *powerFailureNvCtrl = mrsGetPowerFailureNvCtrl();
	hi_u32 erease_data_th = powerFailureNvCtrl->ucCcoConvergeTime * MRS_US_TO_S_MASK * CCO_ERASE_BITMAP;
	hi_u32 valid_data_th = powerFailureNvCtrl->ucCcoConvergeTime * CCO_ERASE_BITMAP;

	// 4分钟内有数据上报，重新启动4分钟等待定时器
	if(mrsTimeSubDuration(cur_time, power_failure_info->last_rcv_power_on_time) < valid_data_th){
		MRS_StartTimer(MRS_CCO_TIMER_POWERON_ERASE_DATA, erease_data_th, HI_SYS_TIMER_ONESHOT);
		return;
	}
	
	HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_004, HI_DIAG_MT("cco erase power_on data"),power_failure_info->last_rcv_power_on_time,cur_time);

	//先获取差异数据
	mrsCcoReportBitmap(STA_POWER_ON);
	(hi_void)memcpy_s(cco_temp_diff_bitmap,POWER_FAILURE_BITMAP_LEN, power_failure_info->cco_power_on_diff_bitmap, POWER_FAILURE_BITMAP_LEN);

	//上报数据时间间隔满足90s后,擦除全局变量中的数据
	memset_s(power_failure_info->power_on_bitmap, POWER_FAILURE_BITMAP_LEN, 0,POWER_FAILURE_BITMAP_LEN);
	memset_s(power_failure_info->cco_power_on_diff_bitmap, POWER_FAILURE_BITMAP_LEN, 0,POWER_FAILURE_BITMAP_LEN);
	memset_s(power_failure_info->cco_power_on_last_send_bitmap, POWER_FAILURE_BITMAP_LEN, 0,POWER_FAILURE_BITMAP_LEN);
	
	power_failure_info->cco_power_on_wait_erase = HI_FALSE;
	power_failure_info->cco_erease_power_on_bitmap_flag = HI_FALSE;

	//考入数据到全局变量中
	(hi_void)memcpy_s(power_failure_info->power_on_bitmap,POWER_FAILURE_BITMAP_LEN, cco_temp_diff_bitmap, POWER_FAILURE_BITMAP_LEN);
	(hi_void)memcpy_s(power_failure_info->cco_power_on_diff_bitmap,POWER_FAILURE_BITMAP_LEN, cco_temp_diff_bitmap, POWER_FAILURE_BITMAP_LEN);
}

HI_VOID mrsPowerFailureDfxInfo(HI_VOID)
{
    HI_U16 i = 0;
    HI_U16 powerFailureTeiCnt = 0;
    POWER_FAILURE_CTRL_S *powerFailureInfo = mrsGetPowerFailureCtrlInfo();
    CCO_POWER_FAILURE_STATISTIC_S *powerFailureDfxInfo = mrsGetPowerFailureDfxInfo();

    for(i = 0; i < POWER_FAILURE_BITMAP_LEN * BIT_WIZE; i++)
    {
        //维测信息存储
        if(HI_TRUE == mrsPowerFailureBitTest(powerFailureInfo->cco_diff_bitmap, i))
        {
            mrsPowerFailureBitSet(powerFailureDfxInfo->cco_statistic_pwf_bitmap,i);
        }

        if(HI_TRUE == mrsPowerFailureBitTest(powerFailureDfxInfo->cco_statistic_pwf_bitmap, i))
        {
            powerFailureTeiCnt++;
        }
    }

    powerFailureDfxInfo->pwf_tei_cnt = powerFailureTeiCnt;
    powerFailureDfxInfo->rcv_pwf_frame_time = HI_MDM_GetSeconds();

    HI_MDM_NM_PowerFailurCcoDfxInfo(powerFailureDfxInfo);
}

/*****************************************************************************
 函 数 名  : mrsGetReportBitmap
 功能描述  : 获取未发送过的bitmap
 输出参数  : 无
 返 回 值  : HI_VOID
 本函数调用的函数  :
 调用本函数的函数  :
 访问的全局变量  :
 修改的全局变量  :
 修改的BBP寄存器列表  :

 修改历史      :
  1.日    期   : 2017年9月14日
    作    者   : test
    修改内容   : 新生成函数

*****************************************************************************/
hi_void mrsCcoReportBitmap(hi_u8 event_type)
{
	hi_u8 *bitmap_diff = HI_NULL;
	hi_u8 *bitmap_last_send = HI_NULL;	
	hi_u8 *bitmap_curr = HI_NULL;
    POWER_FAILURE_CTRL_S *powerFailureInfo = mrsGetPowerFailureCtrlInfo();
    HI_U16 i = 0;

	bitmap_diff = (event_type == STA_POWER_FAILURE) ? powerFailureInfo->cco_diff_bitmap : powerFailureInfo->cco_power_on_diff_bitmap;	
	bitmap_last_send = (event_type == STA_POWER_FAILURE) ? powerFailureInfo->cco_last_send_bitmap : powerFailureInfo->cco_power_on_last_send_bitmap;
	bitmap_curr = (event_type == STA_POWER_FAILURE) ? powerFailureInfo->power_failure_bitmap : powerFailureInfo->power_on_bitmap;

    for(i = 0; i < POWER_FAILURE_BITMAP_LEN * BIT_WIZE; i++)
    {
        //新上报的事件tei,设置diff_bitmap对应位为1
        if(HI_TRUE == mrsPowerFailureBitTest(bitmap_curr, i)
           && HI_FALSE == mrsPowerFailureBitTest(bitmap_last_send, i))
        {
            mrsPowerFailureBitSet(bitmap_diff,i);
        }
    }

    (hi_void)memcpy_s(bitmap_last_send, POWER_FAILURE_BITMAP_LEN,
                         bitmap_curr, POWER_FAILURE_BITMAP_LEN);
}

/*****************************************************************************
 函 数 名  : mrsCcoEvtReport
 功能描述  : cco上报事件到集中器模块
 输入参数  : MRS_EVENT_INFO_STRU *pstEvtTransmit
            接收到的完整帧
 输出参数  : 无
 返 回 值  : HI_U32
 本函数调用的函数  :
 调用本函数的函数  :
 访问的全局变量  :
 修改的全局变量  :
 修改的BBP寄存器列表  :

 修改历史      :
  1.日    期   : 2017年8月18日
    作    者   : test
    修改内容   : 新生成函数

*****************************************************************************/
HI_U32 mrsCcoEvtReport(hi_u8 eventType)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    NV_APP_MRS_EVT_CFG_STRU *powerFailureNvCtrl = HI_NULL;
    POWER_FAILURE_CTRL_S *powerFailureInfo = HI_NULL;
    powerFailureInfo = mrsGetPowerFailureCtrlInfo();
    powerFailureNvCtrl = mrsGetPowerFailureNvCtrl();

    HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_043, HI_DIAG_MT("cco evt report"), eventType, powerFailureNvCtrl->ucCcoConvergeTime);

    if(eventType == STA_POWER_FAILURE)
    {
        powerFailureInfo->isRedayToSendPowerFailure = HI_TRUE;
        //30s的定时器，在到期后开放接收功能部分
        ulRet = MRS_StartTimer(MRS_CCO_TIMER_POWERFAILURE, powerFailureNvCtrl->ucCcoConvergeTime * MRS_US_TO_S_MASK, HI_SYS_TIMER_ONESHOT);
    }/*
	else if(eventType == STA_POWER_ON){

        powerFailureInfo->isRedayToSendPowerOn = HI_TRUE;
        //30s的定时器，在到期后开放接收功能部分
        ulRet = MRS_StartTimer(MRS_CCO_TIMER_POWERON, powerFailureNvCtrl->ucCcoConvergeTime * MRS_US_TO_S_MASK, HI_SYS_TIMER_ONESHOT);

    }*/

    return ulRet;
}
hi_void mrs_set_last_rcv_power_on_time(hi_u32 power_on_time)
{
	POWER_FAILURE_CTRL_S *power_failure_info = mrsGetPowerFailureCtrlInfo();
	power_failure_info->last_rcv_power_on_time = power_on_time;
}
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
/*****************************************************************************
 函 数 名  : mrsHandleLocalPowerFailure
 功能描述  : 处理本地停电
 输入参数  : HI_U16 pow_failure_tei
 输出参数  : 无
 返 回 值  : HI_VOID
 本函数调用的函数  :
 调用本函数的函数  :
 访问的全局变量  :
 修改的全局变量  :
 修改的BBP寄存器列表  :

 修改历史      :
  1.日    期   : 2017年8月16日
    作    者   : j00415756
    修改内容   : 新生成函数

*****************************************************************************/
HI_U32 mrsHandleLocalPowerFailure(HI_VOID)
{
    HI_U32 ret = HI_ERR_SUCCESS;

    if (mrsToolsIsIICollector())
    {
        ret = mrsCltiiHandleLocalPowerOff();
    }
    else
    {
        ret = mrsStaHandleLocalPowerFailure();
    }

    return ret;
}


HI_U32 mrsStaHandleLocalPowerFailure(HI_VOID)
{
    POWER_FAILURE_CTRL_S *powerFailureInfo = HI_NULL;
    HI_U32 ret = HI_ERR_SUCCESS;
    POWER_FAILURE_RCV_MAC_REPORT_S *rcvMacInfo = HI_NULL;

    powerFailureInfo = mrsGetPowerFailureCtrlInfo();
    rcvMacInfo = mrsGetPowerFailureMacInfo();

    //在外部完成tei的刷新到全局bitmap之中
    powerFailureInfo->power_failure_bitmap[rcvMacInfo->my_tei / BIT_WIZE] |= 1 << (rcvMacInfo->my_tei % BIT_WIZE);

    ret = mrsLocalPowerFailureWaitSend();

    return ret;
}

HI_U32 mrsLocalPowerFailureWaitSend(HI_VOID)
{
    //获取nv，获取发送时间后随机出一个值
    NV_APP_MRS_EVT_CFG_STRU *powerFailureNvCtrl = HI_NULL;
    HI_U32 randomWaitSendTime = 0;
    HI_U32 ret = HI_ERR_SUCCESS;

    powerFailureNvCtrl = mrsGetPowerFailureNvCtrl();
    HI_MDM_GetRealRandom(powerFailureNvCtrl->usRandomMin, powerFailureNvCtrl->usRandomMax,
                         &randomWaitSendTime);
    HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_036, HI_DIAG_MT("APP_RCV_PWF_Evt"),
                       powerFailureNvCtrl->usRandomMin,powerFailureNvCtrl->usRandomMax);
    //设置定时器，在random_wait_time时间后 先判断是否收到其他模块的报文 创建上报帧并发送
    ret = MRS_StartTimer(MRS_STA_TIMER_POWERFAILURE, randomWaitSendTime, HI_SYS_TIMER_ONESHOT);

    return ret;
}

hi_u32 mrs_local_power_on_wait_send(hi_void)
{
    //获取nv，获取发送时间后随机出一个值
    NV_APP_MRS_EVT_CFG_STRU *powerFailureNvCtrl = HI_NULL;
    HI_U32 randomWaitSendTime = 0;
    HI_U32 ret = HI_ERR_SUCCESS;

    powerFailureNvCtrl = mrsGetPowerFailureNvCtrl();
    HI_MDM_GetRealRandom(powerFailureNvCtrl->usRandomMin, powerFailureNvCtrl->usRandomMax,
                         &randomWaitSendTime);
    //HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_036, HI_DIAG_MT("APP_RCV_PWF_Evt"),
                       //powerFailureNvCtrl->usRandomMin,powerFailureNvCtrl->usRandomMax);
    //设置定时器，在random_wait_time时间后 先判断是否收到其他模块的报文 创建上报帧并发送
    ret = MRS_StartTimer(MRS_STA_TIMER_POWERON, randomWaitSendTime, HI_SYS_TIMER_ONESHOT);

    return ret;
}

hi_u32 mrs_local_power_on_report_frame(hi_void)
{
	mrsStaCreatePowerFailureFrame(STA_POWER_ON);
	return HI_ERR_SUCCESS;
}

// 停电判定是否响应拉动rst管脚
hi_u32 mrs_power_failure_juedge_pull_rst(hi_void)
{	
    POWER_FAILURE_RCV_MAC_REPORT_S *power_failure_rcv_info = mrsGetPowerFailureMacInfo();

	if (power_failure_rcv_info->device_power_failure == HI_FALSE) {
		HI_SYS_RstDelayDisable(HI_SYS_RST_DELAY_ENTER_PHY_TEST_MODE);
        HI_MDM_Reboot(HI_SYS_REBOOT_CAUSE_MRS10);
		return HI_ERR_SUCCESS;
	}

	return HI_ERR_FAILURE;
}

// 判断是否需要关闭停电功能。
hi_u32 mrs_power_failure_judge_disable_power_failure(hi_void)
{
	POWER_FAILURE_RCV_MAC_REPORT_S *power_failure_rcv_info = mrsGetPowerFailureMacInfo();	
    POWER_FAILURE_CTRL_S *power_failure_ctrl_info = mrsGetPowerFailureCtrlInfo();
	hi_bool enable_power_failure = HI_FALSE;

	// 没有收到停电上报时，关闭停电上报功能
	if (power_failure_rcv_info->device_power_failure == HI_FALSE) {		
		HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_062, HI_DIAG_MT("close power failure"));
		power_failure_ctrl_info->is_rcv_power_down = HI_FALSE;
		HI_MDM_NM_PowerFailurReportEnable(&enable_power_failure);
		return HI_ERR_SUCCESS;
	}

	return HI_ERR_FAILURE;
}

#endif

HI_VOID mrsSetStaPowerFailureNvCtrl(NV_APP_MRS_EVT_CFG_STRU *nv)
{

    NV_APP_MRS_EVT_CFG_STRU *powerFailureNvCtrl = mrsGetPowerFailureNvCtrl();
    //HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_037, HI_DIAG_MT("handle nv"));
    //根据需求更新获取内容
    powerFailureNvCtrl->ucCcoConvergeTime = nv->ucCcoConvergeTime;
    powerFailureNvCtrl->ucStaConvergeTime = nv->ucStaConvergeTime;
    powerFailureNvCtrl->usRandomMin = nv->usRandomMin;
    powerFailureNvCtrl->usRandomMax = nv->usRandomMax;
    powerFailureNvCtrl->usSendInterval = nv->usSendInterval;
    powerFailureNvCtrl->ucSendTimes = nv->ucSendTimes;
    powerFailureNvCtrl->ucPowerFailureReportEnable = nv->ucPowerFailureReportEnable;
    powerFailureNvCtrl->ucPowerOffMeterTimeDelay = nv->ucPowerOffMeterTimeDelay;
}
#if defined(PRODUCT_CFG_SUPPORT_MRS_DUOL_MODE_RF)
#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)

HI_U32 mrsRfSetPowerFailureInfo(HI_VOID *pstInf,HI_U32 *nNeedRfTransmitPowerInfo)
{
	//HI_U32 i;
	MRS_EVENT_INFO_STRU *pstEvtInf = (MRS_EVENT_INFO_STRU *)pstInf;
	
	if(HI_NULL==pstEvtInf)
	{
		return HI_ERR_BAD_FRAME;
	}
	POWER_FAILURE_RCV_MAC_REPORT_S *deviceStateInfo = mrsGetPowerFailureMacInfo();
	POWER_FAILURE_CTRL_S *powerFailureBitmap = mrsGetPowerFailureCtrlInfo();

	HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1005, HI_DIAG_MT("[RF][PCO-MR]-PW get other data"),
						pstEvtInf->ucData,pstEvtInf->ulDataLen);

	if(pstEvtInf->ulDataLen <= DATA_FIX_SIZE)
	{
		return HI_ERR_BAD_FRAME;
	}

	// 收到RF 传递过来的停电模块，和HPLC的传递参数对比
	/*for(i=0;i<POWER_FAILURE_BITMAP_LEN && ((i+DATA_FIX_SIZE)<pstEvtInf->ulDataLen);i++)
	{
		HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_029, HI_DIAG_MT("[RF]参数"),pstEvtInf->ucData[DATA_FIX_SIZE+i],powerFailureBitmap->power_failure_bitmap[i]);
		if(pstEvtInf->ucData[DATA_FIX_SIZE+i] > powerFailureBitmap->power_failure_bitmap[i])
		{
			// RF 收到多于HPLC传输的
			*nNeedRfTransmitPowerInfo=0x3;
			HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_069, HI_DIAG_MT("[RF]发送次数 3"));
			break;
		}
	}
	*/
	//HI_DIAG_LOG_MSG_E1(__LINE__, HI_DIAG_MT("pstEvtInf->ulFunCode="),
						//pstEvtInf->ulFunCode);
	//mrsGetOthersPowerFailureFrameData(pstEvtInf,HI_NULL,HI_FALSE);	//weichao
	mrsGetStaOthersPowerFailureFrameData(pstEvtInf);
	
	if(HI_FALSE == deviceStateInfo->device_power_failure && HI_FALSE == powerFailureBitmap->isRedayToSendPowerFailure)//weichao isRedayToSend-->isRedayToSendPowerFailure
	{
		HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_065, HI_DIAG_MT("[RF][PCO-MR-PW,in handle other pwf]"));

		mrsHandleOthersPowerFailureFrame();
	}
	return HI_ERR_SUCCESS;
}
HI_U8 *mrsRfGetPowerFailureInfo(HI_U16 *pLen,HI_U32 nNeedRfTransmit)
{
	HI_U16 usPayloadLen = 0;
	HI_U16 pwBitmapMinTei = 0;
	HI_U16 pwBitmapMaxTei = 0;
	HI_U16 usBitmapLen = 0;
	P_MRS_STA_EVT_CTX pstCtx = mrsStaGetEvtCtx();
	HI_U8 *pucPayload = HI_NULL;
	HI_U8 index = 0;
	POWER_FAILURE_RCV_MAC_REPORT_S *deviceStateInfo = HI_NULL;
	MRS_EVENT_INFO_STRU *pstEvtInfo = HI_NULL;
	POWER_FAILURE_CTRL_S *powerFailureBitmapInfo = HI_NULL;
	HI_U8 *bitmapIn = HI_NULL;	
		
	if (HI_NULL == pstCtx)
	{
		return HI_NULL;
	}
	
	//接收bit图信息,获取设备状态
	powerFailureBitmapInfo = mrsGetPowerFailureCtrlInfo();
	//跟着消息一起传递上来
	deviceStateInfo = mrsGetPowerFailureMacInfo();
	//weichao 将自身停电位图加入汇集信息中
	
	if (mrsToolsIsIICollector())//Matteo
	{
		if(MRS_CLT_POWER_ON_TO_OFF_STATUS == mrsCltGetPowerStatus())
		{
		
			powerFailureBitmapInfo->power_failure_bitmap[deviceStateInfo->my_tei / BIT_WIZE] |= 1 << (deviceStateInfo->my_tei % BIT_WIZE);
		}
	}

	//weichao
//	HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_010, HI_DIAG_MT("[Matteo][mrsCltGetPowerStatus]"), mrsCltGetPowerStatus(), mrsCltGetPowerStatus());//Matteo

	bitmapIn = powerFailureBitmapInfo->power_failure_bitmap;
	
	//计算最小,最大tei，数据长度
	pwBitmapMinTei = mrsCalcMinTei(bitmapIn);
	pwBitmapMaxTei = mrsCalcMaxTei(bitmapIn);
	
	HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_060, HI_DIAG_MT("[RF][PCO-tei-range]"), pwBitmapMinTei, pwBitmapMaxTei);
	
	usBitmapLen = (pwBitmapMaxTei - pwBitmapMinTei + BIT_WIZE) / BIT_WIZE;
	
	usPayloadLen = sizeof(MRS_EVENT_INFO_STRU) + DATA_FIX_SIZE + usBitmapLen;
	pucPayload = (HI_U8*)mrsToolsMalloc(usPayloadLen);
	if(HI_NULL == pucPayload)
	{
		//HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_061, HI_DIAG_MT("[RF]Malloc failed"),usPayloadLen);
		return HI_NULL;
	}
	mrsToolsMemZero_s(pucPayload, usPayloadLen, usPayloadLen);
	
	 //Flag(取值0/1) 是否发送停电报文（0表示否/1表示是）
	 /*
	 *(pucPayload+0)= 0x10;
	 if(nNeedRfTransmit>0)
	 {
	 	*(pucPayload+1)= 0x1;
	 }
	 else
	 {
	 	*(pucPayload+1)= 0;
	 }
	*/
	 //  Data	 HPLC应用层标准停电报文(变长)
	//填充帧中的固定部分
	pstEvtInfo = (MRS_EVENT_INFO_STRU *)((HI_U8 *)(pucPayload));
	
	pstEvtInfo->ulInterfaceVer = MRS_EVENT_INF_UP_VER;
	pstEvtInfo->ulStructLen = sizeof(MRS_EVENT_INFO_STRU);
	pstEvtInfo->ulDirBit = MRS_PLC_UP_FLG;
	pstEvtInfo->ulFrmBit = MRS_EVENT_FRM_BIT_FROM;
	pstEvtInfo->ulFunCode = MRS_EVENT_FUN_REPORT_MODUL;
	pstEvtInfo->ulDataLen = DATA_FIX_SIZE + usBitmapLen;
	
	pstCtx->usSeq++;
	//mrsDfxStaEvtReportSeq(pstCtx->usSeq);
	pstEvtInfo->usSeq = pstCtx->usSeq;
	
	//可变区域内容拷入
	pstEvtInfo->ucData[index] = SINGLE_AMMETER_POWER_FAILURE;
	pstEvtInfo->ucData[DATA_START_TEI_POSITION]= pwBitmapMinTei&0xFF;
	pstEvtInfo->ucData[DATA_END_TEI_POSITION]=(pwBitmapMinTei>>8)&0xFF;
	
	//直接将数据域拷入
	mrsStaNormalBitmap(bitmapIn,&pstEvtInfo->ucData[DATA_FIX_SIZE], usBitmapLen);
	//HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_062, HI_DIAG_MT("[RF][PCO-MR-PW=%d]"),pstEvtInfo->ulDataLen);
	//HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1003, HI_DIAG_MT("[RF][PCO-MR]-PW-senddata"), pstEvtInfo->ucData, pstEvtInfo->ulDataLen);

	*pLen=usPayloadLen;
	//weichao
	if (mrsToolsIsIICollector())
		powerFailureBitmapInfo->power_failure_bitmap[deviceStateInfo->my_tei / BIT_WIZE] &= ~(1 << (deviceStateInfo->my_tei % BIT_WIZE));
	//weichao
	return pucPayload;
}
#endif
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
HI_U32 mrsCcoRfSetPowerFailureInfo(MRS_EVENT_INFO_STRU *pstEvtInf)
{
	//HI_U32 i;
	HI_U32 ulRet = HI_ERR_SUCCESS;
	if(HI_NULL==pstEvtInf)
	{
		return HI_ERR_BAD_FRAME;
	}
	POWER_FAILURE_CTRL_S *powerFailureBitmap = mrsGetPowerFailureCtrlInfo();

	HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1007, HI_DIAG_MT("[RF][PCO-MR]-PW get other data"),
						pstEvtInf->ucData,pstEvtInf->ulDataLen);

	if(pstEvtInf->ulDataLen <= DATA_FIX_SIZE)
	{
		return HI_ERR_BAD_FRAME;
	}
	//接收到模块功能后，获取数据信息到全局变量中，进行整合
//weichao-20190320	
	mrsGetOthersPowerFailureFrameData(pstEvtInf,HI_NULL,HI_FALSE);


	//if(HI_FALSE == powerFailureBitmap->isRedayToSend)
	{
		//HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_066, HI_DIAG_MT("[CCO-MR-PW]"));
		//mrsCcoEvtReport();
	}

	 if(HI_FALSE == powerFailureBitmap->isRedayToSendPowerFailure &&
                pstEvtInf->ucData[0] == STA_POWER_FAILURE)
     {
         HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_066, HI_DIAG_MT("[CCO-MR-PW]"));
         ulRet = mrsCcoEvtReport(pstEvtInf->ucData[0]);
     }
//weichao-20190320

	return ulRet;
}
#endif
#endif

#endif


