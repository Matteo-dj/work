//*****************************************************************************
//
//                  版权所有 (C), 1998-2011, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_fw_cmd.c
//  版 本 号   : V1.0
//  作    者   : fengxiaomin/f00209182
//  生成日期   : 2014-7-17
//  功能描述   : 命令注册相关处理函数及接口定义
//
//  函数列表   : TODO: ...
//  修改历史   :
//  1.日    期 : 2014-7-17
//    作    者 : fengxiaomin/f00209182
//    修改内容 : 创建文件
//
//*****************************************************************************

#include "mrs_common.h"
#include "mrs_cmd_msg.h"
#include "hi_mdm.h"
#include "app_simu_adp.h"
#include "mrs_fw_tools.h"
#include "mrs_fw_proto376_2_echo.h"
#include "mrs_fw_n.h"
#include "mrs_srv_sta_queue.h"
#include "mrs_srv_cco_macmap.h"
#include "mrs_srv_parallel_cco.h"
#include "mrs_srv_cco_event.h"
#include "mrs_srv_cco.h"
#include "mrs_srv_parallel_sta.h"
#include "mrs_srv_sta_event.h"
#include "mrs_srv_sta.h"
#include "mrs_dfx_cco.h"
#include "mrs_dfx.h"
#include "mrs_srv_mrslog.h"
#include "mrs_time.h"
#include "mrs_srv_sta_searchmeter.h"
#include "mrs_srv_clti_searchmeter.h"
#include "mrs_dfx_clt.h"
#include "mrs_srv_copy_ctrl_device.h"
#include "mrs_fw_cmd.h"
#include "mrs_dfx_readmeter_config.h"
#include "mrs_dfx_cltii_event.h"
#include "mrs_srv_cco_detect_rm.h"
#include "drv_dfx.h"
#include "mrs_srv_collector.h"
#include "mrs_dfx_diagcmd.h"
#include "mrs_fw_log.h"
#include "mrs_fw_nv.h"
#include "equip_dut_proc.h"
#include "mrs_dfx_cco.h"

HI_PRV HI_U32 ChlStatusQry(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    DIAG_CMD_MRS_CHL_QRY_STRU stAck;

    MRS_NOT_USED(usId);
    MRS_NOT_USED(pCmdParam);
    MRS_NOT_USED(usCmdParamSize);
    MRS_NOT_USED(ulOption);


    stAck.enChipType       = HI_MDM_GetChipType();
    stAck.enProductType    = HI_MDM_GetProductType();
    stAck.enHwPlatformType = HI_MDM_GetHwType();
    stAck.ulPlcChlStatus   = mrsGetPlcChlStatus();

    (hi_void)memset_s(stAck.stProductId.ucPlcMac, sizeof(stAck.stProductId.ucPlcMac), 0, sizeof(stAck.stProductId.ucPlcMac));

    HI_MDM_GetProductId(stAck.stProductId.ucPlcMac,
        &stAck.stProductId.enChipType,
        &stAck.stProductId.enProductType,
        &stAck.stProductId.enHwPlatform );

    (hi_void)memcpy_s(&stAck.stHostCtrlInfo, sizeof(stAck.stHostCtrlInfo), (HI_PVOID)HI_MDM_DfxGetHostCtrl(), sizeof(stAck.stHostCtrlInfo));

    stAck.bEnable          = HI_FALSE;

    // 发送给HSO
    ret = HI_MDM_SendAckPacket(ID_DIAG_CMD_MRS_CHL_STATUS_QRY, &stAck, sizeof(DIAG_CMD_MRS_CHL_QRY_STRU), HI_TRUE);

    MRS_NOT_USED(ret);

    return HI_ERR_CONSUMED;
}

// Cco端状态及信息查询
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
HI_PRV HI_U32 DfxCcoInf(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption)
{
    HI_U32 ret = HI_ERR_SUCCESS;

    MRS_NOT_USED(pCmdParam);
    MRS_NOT_USED(usCmdParamSize);
    MRS_NOT_USED(ulOption);

    switch(usId)
    {
    case ID_DIAG_CMD_MRS_CCO_GET_MACMAP:  // 获取当前Mac地址映射表
    case ID_DIAG_CMD_MRS_CCO_GET_CLT_MACMAP:
        ret = mrsCcoDfxSendMapToHso(usId, ulOption);
        break;

    case ID_DIAG_CMD_MRS_CCO_GET_METER_INF:  // 获取当前电表信息(表库)
        ret = mrsCcoDfxSendMeterInfToHso(ulOption);
        break;

    case ID_DIAG_CMD_MRS_CCO_GET_COLLECTOR_MAP:
        ret = mrsCcoDfxSendCollectorMapToHso(pCmdParam, ulOption);
        break;

/* BEGIN: PN:  MODIFY\ADD\DEL by cuichunyan/00276962 at 2015/11/11 */
    case ID_DIAG_CMD_MRS_CCO_GET_PARAM: //获取表档案修改存盘时间
        ret = mrsCcoDfxSendTimerSaveFlashToHso(ulOption);
        break;
/* END: PN:  MODIFY\ADD\DEL by cuichunyan/00276962 at 2015/11/11 */

    //case ID_DIAG_CMD_MRS_CCO_PLC_STATUS:  // 获取PLC通道状态
    //case ID_DIAG_CMD_MRS_CCO_GET_CURR_NODE:  // 获取当前抄表处理的节点及相关信息
    default:
        break;
    }

    return ret;
}

// 全网搜表相关DIAG命令上报
HI_PRV HI_U32 mrsDfxCcoSearchMeter(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;

    MRS_NOT_USED(usCmdParamSize);

    switch(usId)
    {
    case ID_DIAG_CMD_MRS_GET_CLT_SEARCH_ABNORMAL:       // 获取全网搜表有异常的II型采集器信息
        ulRet = mrsCcoDfxSendCltiiSearchErrorToHso(usId, ulOption);
        break;

    case ID_DIAG_CMD_MRS_GET_CCO_SEARCH_DFX:            // 获取全网搜表可维可测信息
        ulRet = mrsCcoDfxSendSearchInfoToHso(usId, ulOption);
        break;

    case ID_DIAG_CMD_MRS_GET_CCO_SLAVENODE_INFO:        // 获取全网搜表结果
        ulRet = mrsCcoDfxSendSlaveNodeInfoToHso(usId, pCmdParam, ulOption);
        break;

    case ID_DIAG_CMD_MRS_CCO_SEARCH_INFO_EXP:           // 全网搜表维测信息，扩展
        ulRet = mrsCcoDfxSendSearchInfoExpToHso(usId, ulOption);
        break;

    default:
        break;
    }

    return ulRet;
}

HI_PRV HI_U32 mrsDfxDiagCmdMsgSend(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_SYS_QUEUE_MSG_S stMsg = {0};
    HI_U8 *pData = HI_NULL;

    pData = (HI_U8*)mrsToolsMalloc(usCmdParamSize);
    if (!pData)
    {
        return HI_ERR_FAILURE;
    }


    (hi_void)memcpy_s(pData, usCmdParamSize, pCmdParam, usCmdParamSize);
    (hi_void)memset_s(&stMsg, sizeof(HI_SYS_QUEUE_MSG_S), 0, sizeof(HI_SYS_QUEUE_MSG_S));
    stMsg.ulMsgId = EN_MRS_FW_MSG_DIAG_CMD_MSG;
    stMsg.ulParam[0] = (HI_U32)usId;
    stMsg.ulParam[1] = (HI_U32)pData;
    stMsg.ulParam[2] = (HI_U32)usCmdParamSize;
    stMsg.ulParam[3] = ulOption;

    ulRet = mrsSendMessage2Queue(&stMsg);
    if (HI_ERR_SUCCESS != ulRet)
    {
        mrsToolsFree(pData);
    }

    return ulRet;
}

//CCO DIAG运行模式通知
HI_PRV HI_U32 mrsDfxCcoModeNotify(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption)
{
	return mrsDfxDiagCmdMsgSend(usId, pCmdParam, usCmdParamSize, ulOption);
}

// 停电检测DIAG命令上报
HI_PRV HI_U32 mrsDfxPoweroffCheckInfo(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;

    MRS_NOT_USED(usCmdParamSize);

    switch(usId)
    {
        case ID_DIAG_CMD_MRS_CCO_GET_POWEROFF_INFO:
            ulRet = mrsCcoDfxSendPoweroffInfoToHso(usId, pCmdParam, ulOption);
            break;

        case ID_DIAG_CMD_MRS_CCO_GET_STA_POWEROFF_INFO:
            ulRet = mrsCcoDfxSendStaPoweroffInfoToHso(usId, pCmdParam, ulOption);
            break;

        case ID_DIAG_CMD_MRS_CCO_GET_POWEROFF_HISTORY:
            ulRet = mrsCcoDfxSendPoweroffHistoryToHso(usId, pCmdParam, ulOption);
            break;

        case ID_DIAG_CMD_MRS_CCO_GET_POWEROFFING_INFO:
            ulRet = mrsCcoDfxSendPoweroffingInfoToHso(usId, pCmdParam, ulOption);
            break;

        default:
            break;
    }

    return ulRet;
}

//获取抄控器plc最近一次报文信息
HI_PRV HI_U32 macCcoGetNdmPlcRevInfo(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption)
{
	MRS_DFX_MODULE *dfx = mrsDfxGetCcoNdmPlcInfo();
	PLC_CHL_INF_S ndm_plc_rx_tx = {0};

    MRS_NOT_USED(usId);
    MRS_NOT_USED(pCmdParam);
    MRS_NOT_USED(usCmdParamSize);

    (hi_void)memset_s(&ndm_plc_rx_tx, sizeof(PLC_CHL_INF_S), 0, sizeof(PLC_CHL_INF_S));
    (hi_void)memcpy_s(&ndm_plc_rx_tx, sizeof(PLC_CHL_INF_S), &dfx->app_cco_ndm_plc_rx_tx, sizeof(PLC_CHL_INF_S));

    HI_MDM_ReportPacket(ID_DIAG_CMD_MRS_CCO_GET_NDMPLC_INFO, (HI_U8)ulOption, (HI_PBYTE)&ndm_plc_rx_tx,
                        sizeof(PLC_CHL_INF_S), HI_FALSE);

    return HI_ERR_SUCCESS;
}
#endif

HI_PRV HI_U32 mrsDfxGetSysTime(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption)
{
    DIAG_CMD_MRS_GET_SYS_TIME_IND_STRU time = {0};
    HI_SYS_CALENDAR_TIME_S stTime = {0};
    HI_U32 secTime = 0;

    MRS_NOT_USED(usId);
    MRS_NOT_USED(pCmdParam);
    MRS_NOT_USED(usCmdParamSize);

    secTime = HI_MDM_GetRealTime();
    HI_MDM_MakeSecondToCalendar(secTime, &stTime);

    time.second = secTime;
    time.timeStamp = HI_MDM_GetMilliSeconds();

    time.tmTime.tmYear = stTime.year;
    time.tmTime.tmMon = stTime.month;
    time.tmTime.tmMday = stTime.day;
    time.tmTime.tmHour = stTime.hour;
    time.tmTime.tmMin = stTime.min;
    time.tmTime.tmSec = stTime.sec;
    time.tmTime.tmWday = 0;
    time.tmTime.tmYday = 0;

    HI_MDM_ReportPacket(usId, (HI_U8)ulOption, (HI_PBYTE)&time, sizeof(time), HI_TRUE);

    return HI_ERR_SUCCESS;
}


HI_PRV HI_U32 mrsDfxGetHwId(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption)
{
    HI_DFX_HW_PRODUCT_ID_QRY_REQ_S stHwId;

    MRS_NOT_USED(pCmdParam);
    MRS_NOT_USED(usCmdParamSize);

    (hi_void)memset_s(&stHwId, sizeof(stHwId), 0, sizeof(stHwId));
    stHwId.ulRet = HI_MDM_GetHwProductId(&(stHwId.enChipType), &(stHwId.enProductType));
    HI_MDM_ReportPacket(usId, (HI_U8)ulOption, (HI_PBYTE)(&stHwId), (HI_U16)sizeof(stHwId), HI_TRUE);

    return HI_ERR_SUCCESS;
}


HI_PRV HI_U32 mrsDfxGetUserVer(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption)
{
    DIAG_CMD_MRS_GET_USER_VER_STRU *pstVer = HI_NULL;
    HI_MDM_SYS_INFO_S *pstInfo = (HI_MDM_SYS_INFO_S *)HI_MDM_GetSysInfo();

    if (((ID_DIAG_CMD_MRS_GET_USER_VER != usId) && (ID_DIAG_CMD_USER_VER_QRY != usId)) || (pstInfo == HI_NULL))
    {
        return HI_ERR_INVALID_ID;
    }

    MRS_NOT_USED(pCmdParam);
    MRS_NOT_USED(usCmdParamSize);

	pstVer = (DIAG_CMD_MRS_GET_USER_VER_STRU*)mrsToolsMalloc(sizeof(DIAG_CMD_MRS_GET_USER_VER_STRU));
	if (!pstVer)
	{
        return HI_ERR_MALLOC_FAILUE;
	}

    (hi_void)memset_s(pstVer, sizeof(DIAG_CMD_MRS_GET_USER_VER_STRU), 0, sizeof(DIAG_CMD_MRS_GET_USER_VER_STRU));
	mrsFwGetUserVer(pstVer->szUserVersion);
	pstVer->ucInterfaceVer = MRS_CMD_STRU_VER_R1;
	pstVer->ucStruSize = sizeof(DIAG_CMD_MRS_GET_USER_VER_STRU);
    pstVer->usSwVersion = pstInfo->usSwVer;
    (hi_void)memcpy_s(pstVer->szManuCode, sizeof(pstInfo->szManuCode), pstInfo->szManuCode, sizeof(pstInfo->szManuCode));
    (hi_void)memcpy_s(pstVer->szChipCode, sizeof(pstInfo->szChipCode), pstInfo->szChipCode, sizeof(pstInfo->szChipCode));
    mrsIntToBcd((HI_U32)pstInfo->usDay, &pstVer->aucDate[0], sizeof(HI_U8));
    mrsIntToBcd((HI_U32)pstInfo->usMonth, &pstVer->aucDate[1], sizeof(HI_U8));
    mrsIntToBcd((HI_U32)pstInfo->usYear, &pstVer->aucDate[2], sizeof(HI_U8));

    HI_MDM_ReportPacket(usId, (HI_U8)ulOption, (HI_PBYTE)pstVer, sizeof(DIAG_CMD_MRS_GET_USER_VER_STRU), HI_TRUE);

	mrsToolsFree(pstVer);

    return HI_ERR_SUCCESS;
}

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)

// 通过DIAG命令设置客户信息
HI_PRV HI_U32 mrsDfxCcoSetUserInfo(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    DIAG_CCO_DUT_SET_USER_INF_REQ_STRU * pstInfo = HI_NULL;
    DIAG_CCO_DUT_SET_USER_INF_IND_STRU stInd = {0};
    NV_MRS_USER_VER_S stVer;

    do
    {
        if(ID_DIAG_CMD_MRS_SET_USER_INFO != usId)
        {
            ulRet =  HI_ERR_INVALID_ID;
            HI_DIAG_LOG_MSG_E1(__LINE__, HI_DIAG_MT("【ERR】cmdid=%d"),usId);
            break;
        }

        if (pCmdParam == HI_NULL || usCmdParamSize == 0)
        {
            HI_DIAG_LOG_MSG_E1(__LINE__, HI_DIAG_MT("【ERR】param =%d"),usCmdParamSize);
            ulRet =  HI_ERR_INVALID_PARAMETER;
            break;
        }

        if (usCmdParamSize != sizeof(DIAG_CCO_DUT_SET_USER_INF_REQ_STRU))
        {
            HI_DIAG_LOG_MSG_E1(__LINE__, HI_DIAG_MT("【ERR】param size =%d"),usCmdParamSize);
            ulRet = HI_ERR_BAD_FRAME_SIZE;
            break;
        }

        pstInfo  = (DIAG_CCO_DUT_SET_USER_INF_REQ_STRU *)pCmdParam;

        ulRet = MRS_TimeBinIsValid(pstInfo->aucDate);
        if (HI_ERR_SUCCESS != ulRet)
        {
            ulRet = HI_ERR_BAD_DATA;
            HI_DIAG_LOG_MSG_E1(__LINE__, HI_DIAG_MT("【ERR】TIME INVALID"),ulRet);
            break;
        }

        //设置客户信息
        ulRet = mrsCcoSetProto((HI_U8)(pstInfo->usProtoVer));
        if (HI_ERR_SUCCESS != ulRet)
        {
            HI_DIAG_LOG_MSG_E1(__LINE__, HI_DIAG_MT("【ERR】Set proto =%d"),ulRet);
            break;
        }

        ulRet = mrsNVSetszId(pstInfo->aucManuCode);
        if (HI_ERR_SUCCESS != ulRet)
        {
            HI_DIAG_LOG_MSG_E1(__LINE__, HI_DIAG_MT("【ERR】Set szId =%d"),ulRet);
            break;
        }

        (hi_void)memset_s(&stVer, sizeof(stVer), 0, sizeof(stVer));
        (hi_void)memcpy_s(stVer.aucDate, sizeof(stVer.aucDate), pstInfo->aucDate, sizeof(pstInfo->aucDate));
        (hi_void)memcpy_s(stVer.aucUserVersion,sizeof(stVer.aucUserVersion), pstInfo->szUserVer, HI_USER_VER_LENGTH);
        (hi_void)memcpy_s(stVer.szChipCode, sizeof(stVer.szChipCode), pstInfo->szChipCode, sizeof(pstInfo->szChipCode));
        stVer.usSwVer = pstInfo->usSwVer;
        ulRet = mrsNVSetUserVerInfo(&stVer);
        if (HI_ERR_SUCCESS != ulRet)
        {
            HI_DIAG_LOG_MSG_E1(__LINE__, HI_DIAG_MT("【ERR】Set UserInfo =%d"),ulRet);
            break;
        }

    }while(HI_FALSE);

    stInd.ulRet = ulRet;

    HI_MDM_ReportPacket(usId, (HI_U8)ulOption, (HI_PBYTE)&stInd, sizeof(stInd), HI_TRUE);

    return ulRet;
}

//读取芯片ID
HI_PRV HI_U32 mrsDfxCcoReadMacChipID(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption)
{
	HI_U32 ulRet = HI_ERR_SUCCESS;
	EQUIP_CHIP_ID_R_STRU stInfo = {0};
	HI_U8 ucIPAddr[HI_IPV4_ADDR_LEN] = {0};

    (hi_void)memset_s(&stInfo, sizeof(EQUIP_CHIP_ID_R_STRU), 0, sizeof(EQUIP_CHIP_ID_R_STRU));
	do
	{
		if(ID_DIAG_CMD_MRS_READ_MAC_CHIP_ID != usId)
		{
			ulRet =  HI_ERR_INVALID_ID;
			stInfo.ret = HI_ERR_INVALID_ID;
			break;
		}

		MRS_NOT_USED(pCmdParam);
		MRS_NOT_USED(usCmdParamSize);

		//读取芯片ID
		ulRet = mrsReadIDProc(&stInfo);
		if(HI_ERR_SUCCESS != ulRet)
		{
			break;
		}
	}while(HI_FALSE);

    if(HI_ERR_SUCCESS != ulRet)
	{
	    (hi_void)memset_s(stInfo.chipId, MANAGEMENT_ID_SIZE, 0, MANAGEMENT_ID_SIZE);
	}

    ulRet = HI_MDM_NM_GetAddr(stInfo.mac, ucIPAddr);
	if(HI_ERR_SUCCESS != ulRet)
	{
		stInfo.ret |= HI_ERR_READ_CHIP_ID_MAC_FAIL;
	}
    HI_MDM_ReportPacket(usId, (HI_U8)ulOption, (HI_PBYTE)&stInfo, sizeof(EQUIP_CHIP_ID_R_STRU), HI_TRUE);

	return ulRet;
}

//写入ID
HI_PRV HI_U32 mrsDfxCcoWriteID(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption)
{
	HI_U32 ulRet = HI_ERR_SUCCESS;
	EQUIP_CHIP_ID_W_STRU stInfo = {0};
    (hi_void)memset_s(&stInfo, sizeof(EQUIP_CHIP_ID_W_STRU), 0, sizeof(EQUIP_CHIP_ID_W_STRU));
	do
	{
		if(ID_DIAG_CMD_MRS_WIRTE_ID != usId)
		{
			ulRet =  HI_ERR_INVALID_ID;
			break;
		}

		if (pCmdParam == HI_NULL || usCmdParamSize != MANAGEMENT_ID_SIZE)
		{
			ulRet =  HI_ERR_INVALID_PARAMETER;
			break;
		}

	    (hi_void)memcpy_s(stInfo.chipId, MANAGEMENT_ID_SIZE, pCmdParam, MANAGEMENT_ID_SIZE);

		//写入芯片ID
		ulRet = HI_MDM_SetMyManagementId(stInfo.chipId);
		if(HI_ERR_SUCCESS != ulRet)
		{
			stInfo.ret = HI_ERR_WRITE_CHIP_ID_NV_FAIL;
			break;
		}

		//读取芯片ID
		ulRet = HI_MDM_GetMyManagementId(stInfo.chipId);
		if(HI_ERR_SUCCESS != ulRet)
		{
			stInfo.ret = HI_ERR_READ_CHIP_ID_NV_FAIL;
			break;
		}
	}while(HI_FALSE);

    if(HI_ERR_SUCCESS != ulRet)
	{
	    (hi_void)memset_s(stInfo.chipId, MANAGEMENT_ID_SIZE, 0, MANAGEMENT_ID_SIZE);
	}

    HI_MDM_ReportPacket(usId, (HI_U8)ulOption, (HI_PBYTE)&stInfo, sizeof(EQUIP_CHIP_ID_W_STRU), HI_TRUE);
	return ulRet;
}

HI_PRV HI_U32 mrsDfxCcoRereshNvFile(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption)
{

    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_DIAG_CMD_MRS_REFRESH_NV_FILE_REQ_S *pstReq = HI_NULL;
    HI_DIAG_CMD_MRS_REFRESH_NV_FILE_IND_S stInd = {0};

    do
    {
        if(ID_DIAG_CMD_MRS_REFRESH_NV_FILE  != usId)
        {
            ulRet =  HI_ERR_INVALID_ID;
            HI_DIAG_LOG_MSG_E1(__LINE__, HI_DIAG_MT("【ERR】cmdid=%d"),usId);
            break;
        }

        if (pCmdParam == HI_NULL || usCmdParamSize == 0)
        {
            HI_DIAG_LOG_MSG_E1(__LINE__, HI_DIAG_MT("【ERR】param =%d"),usCmdParamSize);
            ulRet =  HI_ERR_INVALID_PARAMETER;
            break;
        }

        pstReq  = (HI_DIAG_CMD_MRS_REFRESH_NV_FILE_REQ_S *)pCmdParam;
        if (pstReq->ucStructSize != usCmdParamSize)
        {
            HI_DIAG_LOG_MSG_E2(__LINE__, HI_DIAG_MT("【ERR】size err =%d ucStructSize = %d"),usCmdParamSize,pstReq->ucStructSize);
            ulRet =  HI_ERR_INVALID_PARAMETER;
            break;
        }

        ulRet = HI_MDM_NV_RefreshDataToNvFile();

    }while(HI_FALSE);

    stInd.ulRc = ulRet;
    stInd.ucVer = MRS_CMD_STRU_VER_R1;
    stInd.ucStructSize = sizeof(HI_DIAG_CMD_MRS_REFRESH_NV_FILE_IND_S);

    HI_MDM_ReportPacket(usId, (HI_U8)ulOption, (HI_PBYTE)&stInd, stInd.ucStructSize, HI_TRUE);

    return ulRet;
}
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_NDM)

#define MR_CCO_NDM_INTERCONNECT_FRAME_HEAD_SIZE 3   //0x0040帧头长度

ndm_interconnect_info g_ndm_interconnect_info = {{0}, {0}, NDM_INTERCONNECT_CMD_ID_CONNECT, 0, {0}};
HI_U16 ndm_ctrl_sta_read_mrs_seq = 0;
ndm_interconnect_info* ndm_get_interconnect_info()
{
    return &g_ndm_interconnect_info;
}
HI_U32 mrsNdmConnectSearchResultReport(HI_U8 ucType,HI_MDM_NDM_CONNECT_SEARCH_REPORT_INFO_ST *pCmdData,HI_U32 ulDataSize)
{
    HI_DIAG_LOG_MSG_E0(__LINE__, HI_DIAG_MT("【ERR】mrsNdmConnectSearchResultReport"));

    if(ucType != NDM_CONNECT_REPORT_ACCESS_MAC && pCmdData->stei == 1)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    if(ulDataSize != sizeof(HI_MDM_NDM_CONNECT_SEARCH_REPORT_INFO_ST))
    {
        return HI_ERR_INVALID_PARAMETER;
    }
    if(!pCmdData)
	{
        return HI_ERR_INVALID_PARAMETER;
    }

    ndm_interconnect_info *pndm_intetconnect_info = ndm_get_interconnect_info();

    NDM_INTERCONNECT_CMD_ID_CONNECT_IND_STRU stConnectInd;
    (hi_void)memset_s(&stConnectInd, sizeof(NDM_INTERCONNECT_CMD_ID_CONNECT_IND_STRU), 0, sizeof(NDM_INTERCONNECT_CMD_ID_CONNECT_IND_STRU));

    stConnectInd.ret = pCmdData->is_search_success;
    if(stConnectInd.ret == HI_NDM_CONNECT_SUCCESS)
    {
        (hi_void)memcpy_s(pndm_intetconnect_info->success_mac, HI_PLC_MAC_ADDR_LEN, pCmdData->mac, HI_PLC_MAC_ADDR_LEN);
        (hi_void)memcpy_s(pndm_intetconnect_info->des_mac, HI_PLC_MAC_ADDR_LEN, pCmdData->mac, HI_PLC_MAC_ADDR_LEN);
        (hi_void)memcpy_s(stConnectInd.des_mac, HI_PLC_MAC_ADDR_LEN, pCmdData->mac, HI_PLC_MAC_ADDR_LEN);
    }
    else
    {
        (hi_void)memset_s(pndm_intetconnect_info,sizeof(ndm_interconnect_info), 0,sizeof(ndm_interconnect_info));
    }

    //未入网sta的stei和nid都为0
    if(pCmdData->stei == 0 && pCmdData->nid == 0)
    {
        pndm_intetconnect_info->isonline = FALSE;
    }
    else
    {
        pndm_intetconnect_info->isonline = TRUE;
    }

    HI_NDM_INTERCONNECT_CMD_STRU* pstInterConnectCmd = NULL;
    HI_U32 ulLen = sizeof(HI_NDM_INTERCONNECT_CMD_STRU)+sizeof(NDM_INTERCONNECT_CMD_ID_CONNECT_IND_STRU);
    pstInterConnectCmd = (HI_NDM_INTERCONNECT_CMD_STRU*)mrsToolsMalloc(ulLen);
    if(pstInterConnectCmd == NULL)
    {
        return HI_ERR_NOT_ENOUGH_MEMORY;
    }
    (hi_void)memset_s(pstInterConnectCmd,ulLen, 0,ulLen);

    pstInterConnectCmd->cmd_id = NDM_INTERCONNECT_CMD_ID_CONNECT;
    pstInterConnectCmd->ulDataSize = sizeof(NDM_INTERCONNECT_CMD_ID_CONNECT_IND_STRU);
    (hi_void)memcpy_s(pstInterConnectCmd->aucData,sizeof(NDM_INTERCONNECT_CMD_ID_CONNECT_IND_STRU), &stConnectInd, sizeof(NDM_INTERCONNECT_CMD_ID_CONNECT_IND_STRU));

    HI_U32 ret = HI_MDM_ReportPacket(HI_ND_NDM_CMD_PACKET, HI_DIAG_CMD_INSTANCE_DEFAULT, (HI_PBYTE)pstInterConnectCmd, (HI_U16)ulLen, HI_TRUE);
    mrsToolsFree(pstInterConnectCmd);
    return ret;
}
HI_U32 mrsInterConnectCmdConnectHandler(HI_PVOID pCmdParam, HI_U16 usCmdParamSize)
{
	if(usCmdParamSize != sizeof(NDM_INTERCONNECT_CMD_ID_CONNECT_REQ_STRU))
		return HI_ERR_INVALID_PARAMETER;

    ndm_interconnect_info *pndm_intetconnect_info = ndm_get_interconnect_info();
	NDM_INTERCONNECT_CMD_ID_CONNECT_REQ_STRU *pstConnectReq = (NDM_INTERCONNECT_CMD_ID_CONNECT_REQ_STRU*)pCmdParam;

    pndm_intetconnect_info->isonline = FALSE;
    (hi_void)memset_s(pndm_intetconnect_info->success_mac,HI_PLC_MAC_ADDR_LEN, 0,HI_PLC_MAC_ADDR_LEN);
    (hi_void)memcpy_s(pndm_intetconnect_info->des_mac, HI_PLC_MAC_ADDR_LEN, pstConnectReq->des_mac, HI_PLC_MAC_ADDR_LEN);

    HI_MDM_NdmConnectSearchResultReport(mrsNdmConnectSearchResultReport);
    HI_U32 ret = HI_MDM_NdmConnectStartSearchNet(pstConnectReq->des_mac);
    if(ret != HI_ERR_SUCCESS)
    {
        HI_NDM_INTERCONNECT_CMD_STRU* pstInterConnectCmd = NULL;
        HI_U32 ulLen = sizeof(HI_NDM_INTERCONNECT_CMD_STRU)+sizeof(NDM_INTERCONNECT_CMD_ID_CONNECT_IND_STRU);
        pstInterConnectCmd = (HI_NDM_INTERCONNECT_CMD_STRU*)mrsToolsMalloc(ulLen);
        if(pstInterConnectCmd == NULL)
        {
            return HI_ERR_NOT_ENOUGH_MEMORY;
        }
        (hi_void)memset_s(pstInterConnectCmd,ulLen, 0,ulLen);

        pstInterConnectCmd->cmd_id = NDM_INTERCONNECT_CMD_ID_CONNECT;
        pstInterConnectCmd->ulDataSize = sizeof(NDM_INTERCONNECT_CMD_ID_CONNECT_IND_STRU);
        NDM_INTERCONNECT_CMD_ID_CONNECT_IND_STRU stConnectInd;
        (hi_void)memset_s(&stConnectInd, sizeof(NDM_INTERCONNECT_CMD_ID_CONNECT_IND_STRU), 0, sizeof(NDM_INTERCONNECT_CMD_ID_CONNECT_IND_STRU));

        stConnectInd.ret = HI_NDM_CONNECT_FAILE;
        (hi_void)memcpy_s(pstInterConnectCmd->aucData,sizeof(NDM_INTERCONNECT_CMD_ID_CONNECT_IND_STRU), &stConnectInd, sizeof(NDM_INTERCONNECT_CMD_ID_CONNECT_IND_STRU));

        HI_MDM_ReportPacket(HI_ND_NDM_CMD_PACKET, HI_DIAG_CMD_INSTANCE_DEFAULT, (HI_PBYTE)pstInterConnectCmd, (HI_U16)ulLen, HI_TRUE);
        mrsToolsFree(pstInterConnectCmd);
    }
    return ret;
}

HI_U32 mrsInterConnectCmdForwadHandler(HI_PVOID pCmdParam, HI_U16 usCmdParamSize ,HI_BOOL bIsSendToSta)
{
    HI_DIAG_LOG_MSG_E0(__LINE__, HI_DIAG_MT("【ERR】mrsInterConnectCmdForwadHandler"));

    ndm_interconnect_info* pndm_interconnect_mac = ndm_get_interconnect_info();

    MRS_PLC_FRAME_DATA_S stFrameData = {0};
    stFrameData.is_bc_frame = HI_FALSE;

    (hi_void)memcpy_s(stFrameData.ucMac, HI_PLC_MAC_ADDR_LEN, pndm_interconnect_mac->success_mac, HI_PLC_MAC_ADDR_LEN);

    if(mrsToolsMemIsZero(stFrameData.ucMac,6))
    {
        return HI_ERR_INVALID_PARAMETER;
    }
    if(bIsSendToSta)
    {
        HI_U16 payload_len = sizeof(MRS_DATA_TRANSMIT_AFN13_DL_STRU) + usCmdParamSize;
        MRS_DATA_TRANSMIT_AFN13_DL_STRU * transmit_data = (MRS_DATA_TRANSMIT_AFN13_DL_STRU*)mrsToolsMalloc(payload_len);
        if(!transmit_data)
        {
            return HI_ERR_NOT_ENOUGH_MEMORY;
        }
        transmit_data->interface_ver = MRS_AFN13_DL_VER;
        transmit_data->stru_size     = sizeof(MRS_DATA_TRANSMIT_AFN13_DL_STRU);
        transmit_data->protocol      = 2;        // 645-07
        transmit_data->data_len      = usCmdParamSize;        // 获取待转发报文长度
        transmit_data->seq           = ndm_ctrl_sta_read_mrs_seq++;
        transmit_data->time_out      = 10; // 单位为百毫秒
        transmit_data->dir           = MRS_PLC_DL_FLG;  // 置下行帧标志
        (hi_void)memcpy_s(transmit_data->data, transmit_data->data_len, pCmdParam, usCmdParamSize); // 获取待转发的数据内容

        stFrameData.usId = ID_MRS_CMD_DATA_TRANSMIT_AFN13_PLC;
        stFrameData.pucPayload = (HI_PBYTE)transmit_data;
        stFrameData.usPayloadLen = payload_len;
    }
    else
    {
        /*0x0040-抄控器-CCO下行、上行报文格式说明*/
        /*域        字节号  大小(比特)           */
        /*协议类型  0       8                    */
        /*报文长度  1-2     16                   */
        /*报文内容  3-N                          */
        HI_U16 payload_len = MR_CCO_NDM_INTERCONNECT_FRAME_HEAD_SIZE + usCmdParamSize;
        HI_PBYTE pstPlcHead = mrsToolsMalloc(payload_len);
        if(!pstPlcHead)
        {
            return HI_ERR_NOT_ENOUGH_MEMORY;
        }

        *pstPlcHead = 0;
        *(pstPlcHead+1)= usCmdParamSize&0x00ff;
        *(pstPlcHead+2)= (usCmdParamSize>>8)&0x00ff;
        (hi_void)memcpy_s(pstPlcHead + MR_CCO_NDM_INTERCONNECT_FRAME_HEAD_SIZE ,usCmdParamSize,pCmdParam,usCmdParamSize);

        stFrameData.usId = ID_MRS_CMD_NDM_CTRL_CCO_PLC;
        stFrameData.pucPayload = pstPlcHead;
        stFrameData.usPayloadLen = payload_len;
    }

    HI_U32 ret = MRS_SendPlcFrame(&stFrameData);
    if(stFrameData.pucPayload)
    {
        mrsToolsFree(stFrameData.pucPayload);
    }
    return ret;
}

HI_U32 mrsReceiveInterConnectCommand(HI_U16 usId, HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_NDM_INTERCONNECT_CMD_STRU * pstInterConnectCmd = NULL;

    HI_UNREF_PARAM(usId);
    HI_UNREF_PARAM(ulOption);

    if(usCmdParamSize < sizeof(HI_NDM_INTERCONNECT_CMD_STRU))
        return HI_ERR_INVALID_PARAMETER;


    pstInterConnectCmd = (HI_NDM_INTERCONNECT_CMD_STRU*) pCmdParam;
    if(pstInterConnectCmd->ulDataSize + sizeof(HI_NDM_INTERCONNECT_CMD_STRU) > usCmdParamSize)
        return HI_ERR_INVALID_PARAMETER;


    ndm_interconnect_info *pndm_interconnect_mac = ndm_get_interconnect_info();
    pndm_interconnect_mac->cmd_id = pstInterConnectCmd->cmd_id;

    switch(pstInterConnectCmd->cmd_id)
    {
        case NDM_INTERCONNECT_CMD_ID_CONNECT ://连接
		{
			ret = mrsInterConnectCmdConnectHandler(pstInterConnectCmd->aucData,(HI_U16)pstInterConnectCmd->ulDataSize);
		}
		break;
        case NDM_INTERCONNECT_CMD_ID_TOPO ://TOPO
        case NDM_INTERCONNECT_CMD_ID_METER_ARCHIVE://表档案
        case NDM_INTERCONNECT_CMD_ID_ROUTE_CTRL:
        case NDM_INTERCONNECT_CMD_ID_CCO_READ_METER://CCO抄表
		{
			ret = mrsInterConnectCmdForwadHandler(pstInterConnectCmd->aucData,(HI_U16)pstInterConnectCmd->ulDataSize,FALSE);
		}
		break;
        case NDM_INTERCONNECT_CMD_ID_STA_READ_METER://sta抄表
		{
			ret = mrsInterConnectCmdForwadHandler(pstInterConnectCmd->aucData,(HI_U16)pstInterConnectCmd->ulDataSize,TRUE);
		}
		break;
        default:
        break;
    }

    return ret;

}
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

HI_PRV HI_CONST HI_DIAG_CMD_REG_OBJ_S g_stMrsCmdDefault[] =
{
    {ID_DIAG_CMD_CSIM_PACKET, ID_DIAG_CMD_SIMU_CTRL, SimRxInput, SimRxCtrl},

    {ID_DIAG_CMD_MRS_CHL_STATUS_QRY, ID_DIAG_CMD_MRS_CHL_STATUS_QRY, ChlStatusQry, HI_NULL},

#if defined(PRODUCT_CFG_PRODUCT_TYPE_NDM)
    {HI_ND_NDM_CMD_PACKET,HI_ND_NDM_CMD_PACKET,mrsReceiveInterConnectCommand, HI_NULL},
#endif
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    {ID_DIAG_CMD_MRS_CCO_QRY_QRY, ID_DIAG_CMD_MRS_CCO_GET_COLLECTOR_MAP, DfxCcoInf, HI_NULL},
    {ID_DIAG_CMD_MRS_GET_CLT_SEARCH_ABNORMAL, ID_DIAG_CMD_MRS_CCO_SEARCH_INFO_EXP, mrsDfxCcoSearchMeter, HI_NULL},
    {ID_DIAG_CMD_MRS_CCO_GET_MR_RETRY_CFG, ID_DIAG_CMD_MRS_CCO_MR_RETRY_CFG_DFX_INF, mrsDfxMrRetryDiagCmdNotify, HI_NULL},
    {ID_DIAG_CMD_MRS_CCO_GET_CLT_MACMAP, ID_DIAG_CMD_MRS_CCO_GET_CLT_MACMAP, DfxCcoInf, HI_NULL},
    {ID_DIAG_CMD_MRS_CCO_GET_PARAM, ID_DIAG_CMD_MRS_CCO_GET_PARAM, DfxCcoInf, HI_NULL},
	{ID_DIAG_CMD_MRS_HSO_SET_CCO_MODE, ID_DIAG_CMD_MRS_HSO_SET_CCO_MODE, mrsDfxCcoModeNotify, HI_NULL},
    {ID_DIAG_CMD_MRS_SET_USER_INFO, ID_DIAG_CMD_MRS_SET_USER_INFO, mrsDfxCcoSetUserInfo, HI_NULL},
    {ID_DIAG_CMD_MRS_REFRESH_NV_FILE,ID_DIAG_CMD_MRS_REFRESH_NV_FILE,mrsDfxCcoRereshNvFile, HI_NULL},
    {ID_DIAG_CMD_MRS_READ_MAC_CHIP_ID, ID_DIAG_CMD_MRS_READ_MAC_CHIP_ID, mrsDfxCcoReadMacChipID, HI_NULL},
	{ID_DIAG_CMD_MRS_WIRTE_ID, ID_DIAG_CMD_MRS_WIRTE_ID, mrsDfxCcoWriteID, HI_NULL},
	{ID_DIAG_CMD_MRS_CCO_GET_POWEROFF_INFO, ID_DIAG_CMD_MRS_CCO_GET_POWEROFFING_INFO, mrsDfxPoweroffCheckInfo, HI_NULL},
	{ID_DIAG_CMD_MRS_CCO_GET_NDMPLC_INFO, ID_DIAG_CMD_MRS_CCO_GET_NDMPLC_INFO, macCcoGetNdmPlcRevInfo, HI_NULL},
#endif
#if 0//optimized by weichao
#if defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG)
    {ID_DIAG_CMD_MRS_DAYFREEZE_REC, ID_DIAG_CMD_MRS_DAYFREEZE_REC, mrsLogSendDayfreezeRecToHso, HI_NULL},
#endif
#endif
    {ID_DIAG_CMD_MRS_GET_SYS_TIME, ID_DIAG_CMD_MRS_GET_SYS_TIME, mrsDfxGetSysTime, HI_NULL},
    {ID_DIAG_CMD_MRS_GET_USER_VER, ID_DIAG_CMD_MRS_GET_USER_VER, mrsDfxGetUserVer, HI_NULL},
    {ID_DIAG_CMD_USER_VER_QRY, ID_DIAG_CMD_USER_VER_QRY, mrsDfxGetUserVer, HI_NULL},
    {ID_DIAG_CMD_DRV_MEM_READ, ID_DIAG_CMD_DRV_MEM_READ, mrsDfxOsm, HI_NULL},
    {ID_DIAG_CMD_DRV_MEM_WRITE, ID_DIAG_CMD_DRV_MEM_WRITE, mrsDfxOssm, HI_NULL},
    {ID_DIAG_CMD_DRV_GET_HW_ID, ID_DIAG_CMD_DRV_GET_HW_ID, mrsDfxGetHwId, HI_NULL},

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
    {ID_DIAG_CMD_MRS_GET_CLT_SEARCH_RESULT, ID_DIAG_CMD_MRS_GET_CLT_SEARCH_RESULT, mrsDfxGetCltSearchResult, HI_NULL},
    {ID_DIAG_CMD_MRS_GET_CLT_SEARCH_STATS, ID_DIAG_CMD_MRS_GET_CLT_SEARCH_STATS, mrsDfxGetCltSearchStats, HI_NULL},
    {ID_DIAG_CMD_MRS_GET_CLT_SEARCH_LOCAL, ID_DIAG_CMD_MRS_GET_CLT_SEARCH_LOCAL, mrsDfxGetCltSearchLocal, HI_NULL},
    {ID_DIAG_CMD_MRS_GET_CLT_SEARCH_FRAME, ID_DIAG_CMD_MRS_GET_CLT_SEARCH_FRAME, mrsDfxGetCltSearchFrame, HI_NULL},
    {ID_DIAG_CMD_MRS_GET_CLT_SEARCH_LOCAL_EX, ID_DIAG_CMD_MRS_GET_CLT_SEARCH_LOCAL_EX, mrsDfxGetCltSearchLocalEx, HI_NULL},
    {ID_DIAG_CMD_MRS_GET_CLT_LOCAL_MAP, ID_DIAG_CMD_MRS_GET_CLT_LOCAL_MAP, mrsDfxGetCltLocalMap, HI_NULL},
    {ID_DIAG_CMD_MRS_GET_CLTII_EVENT_STATUS, ID_DIAG_CMD_MRS_GET_CLTII_EVENT_STATUS, mrsDfxGetCltiiEventStatus, HI_NULL},
    {ID_DIAG_CMD_MRS_GET_CLTII_EVENT_INF, ID_DIAG_CMD_MRS_GET_CLTII_EVENT_INF, mrsDfxGetCltiiEventInfo, HI_NULL},
    {ID_DIAG_CMD_MRS_GET_RS485_MULTI_CONNECT, ID_DIAG_CMD_MRS_GET_RS485_MULTI_CONNECT, mrsDfxGetRS485MultiConn, HI_NULL},
    {ID_DIAG_CMD_MRS_CLR_RS485_MULTI_CONNECT, ID_DIAG_CMD_MRS_CLR_RS485_MULTI_CONNECT, mrsDfxClrRS485MultiConn, HI_NULL},

    {HI_STA_TRANSMIT, HI_STA_TRANSMIT, mrsStaTransmitHsoCmd, HI_NULL},
    {HI_NDM_645_FRAME, HI_NDM_645_FRAME, mrsStaTransmitRemoteCmd, HI_NULL},
    {HI_CLTII_R485_TRANSMIT_DATA,HI_CLTII_R485_TRANSMIT_DATA, mrsRS485TranData, HI_NULL},
    {ID_DIAG_CMD_MRS_GET_STA_POWEROFF_INFO, ID_DIAG_CMD_MRS_GET_STA_POWEROFF_INFO, mrsDfxStaGetPoweroffInfo, HI_NULL},
#endif
};

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
HI_PRV HI_CONST HI_DIAG_CMD_REG_OBJ_S g_stMrsCmdIRemote[] =
{
    {ID_DIAG_CMD_MRS_CHL_STATUS_QRY, ID_DIAG_CMD_MRS_CHL_STATUS_QRY, ChlStatusQry, HI_NULL},
	{ID_DIAG_CMD_MRS_GET_USER_VER, ID_DIAG_CMD_MRS_GET_USER_VER, mrsDfxGetUserVer, HI_NULL},
    {ID_DIAG_CMD_USER_VER_QRY, ID_DIAG_CMD_USER_VER_QRY, mrsDfxGetUserVer, HI_NULL},
#if 0//optimized by weichao
#if defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG)
    {ID_DIAG_CMD_MRS_DAYFREEZE_REC, ID_DIAG_CMD_MRS_DAYFREEZE_REC, mrsLogSendDayfreezeRecToHso, HI_NULL},
#endif
#endif
    {ID_DIAG_CMD_MRS_GET_SYS_TIME, ID_DIAG_CMD_MRS_GET_SYS_TIME, mrsDfxGetSysTime, HI_NULL},
    {ID_DIAG_CMD_MRS_GET_CLT_SEARCH_RESULT, ID_DIAG_CMD_MRS_GET_CLT_SEARCH_RESULT, mrsDfxGetCltSearchResult, HI_NULL},
    {ID_DIAG_CMD_MRS_GET_CLT_SEARCH_STATS, ID_DIAG_CMD_MRS_GET_CLT_SEARCH_STATS, mrsDfxGetCltSearchStats, HI_NULL},
    {ID_DIAG_CMD_MRS_GET_CLT_SEARCH_LOCAL, ID_DIAG_CMD_MRS_GET_CLT_SEARCH_LOCAL, mrsDfxGetCltSearchLocal, HI_NULL},
    {ID_DIAG_CMD_MRS_GET_CLT_SEARCH_FRAME, ID_DIAG_CMD_MRS_GET_CLT_SEARCH_FRAME, mrsDfxGetCltSearchFrame, HI_NULL},
    {ID_DIAG_CMD_MRS_GET_CLT_SEARCH_LOCAL_EX, ID_DIAG_CMD_MRS_GET_CLT_SEARCH_LOCAL_EX, mrsDfxGetCltSearchLocalEx, HI_NULL},
    {ID_DIAG_CMD_MRS_GET_CLT_LOCAL_MAP, ID_DIAG_CMD_MRS_GET_CLT_LOCAL_MAP, mrsDfxGetCltLocalMap, HI_NULL},
    {ID_DIAG_CMD_MRS_GET_CLTII_EVENT_STATUS, ID_DIAG_CMD_MRS_GET_CLTII_EVENT_STATUS, mrsDfxGetCltiiEventStatus, HI_NULL},
    {ID_DIAG_CMD_MRS_GET_CLTII_EVENT_INF, ID_DIAG_CMD_MRS_GET_CLTII_EVENT_INF, mrsDfxGetCltiiEventInfo, HI_NULL},
    {ID_DIAG_CMD_MRS_GET_RS485_MULTI_CONNECT, ID_DIAG_CMD_MRS_GET_RS485_MULTI_CONNECT, mrsDfxGetRS485MultiConn, HI_NULL},
    {ID_DIAG_CMD_MRS_CLR_RS485_MULTI_CONNECT, ID_DIAG_CMD_MRS_CLR_RS485_MULTI_CONNECT, mrsDfxClrRS485MultiConn, HI_NULL},

    {HI_STA_TRANSMIT, HI_STA_TRANSMIT, mrsStaTransmitHsoCmd, HI_NULL},
    {HI_NDM_645_FRAME, HI_NDM_645_FRAME, mrsStaTransmitRemoteCmd, HI_NULL},
    {HI_CLTII_R485_TRANSMIT_DATA, HI_CLTII_R485_TRANSMIT_DATA, mrsRS485TranData, HI_NULL},

    {ID_DIAG_CMD_DRV_MEM_READ, ID_DIAG_CMD_DRV_MEM_READ, mrsDfxOsm, HI_NULL},
    {ID_DIAG_CMD_DRV_MEM_WRITE, ID_DIAG_CMD_DRV_MEM_WRITE, mrsDfxOssm, HI_NULL},
    {ID_DIAG_CMD_DRV_GET_HW_ID, ID_DIAG_CMD_DRV_GET_HW_ID, mrsDfxGetHwId, HI_NULL},
    {ID_DIAG_CMD_MRS_GET_STA_POWEROFF_INFO, ID_DIAG_CMD_MRS_GET_STA_POWEROFF_INFO, mrsDfxStaGetPoweroffInfo, HI_NULL},
};
#endif


HI_PRV HI_CONST HI_DIAG_CMD_REG_OBJ_S g_stMrsCmdDRemote[] =
{
    {ID_DIAG_CMD_MRS_BEGIN, ID_DIAG_CMD_MRS_END, HI_NULL, HI_NULL},
    {ID_DIAG_CMD_DRV_MEM_READ, ID_DIAG_CMD_DRV_MEM_WRITE, HI_NULL, HI_NULL},
    {ID_DIAG_CMD_DRV_GET_HW_ID,ID_DIAG_CMD_DRV_GET_HW_ID, HI_NULL, HI_NULL},
};


#if defined(PRODUCT_CFG_PRODUCT_TYPE_NDM)
HI_PRV HI_CONST HI_DIAG_CMD_REG_OBJ_S g_stMrsCmdDRemoteNdm[] =
{
    {HI_NDM_645_FRAME, HI_NDM_645_FRAME, mrsCcdRxTransmitCmd, HI_NULL},
};
#endif

HI_VOID mrsRegistCommandInit(HI_VOID)
{
    HI_MDM_RegisterCommand(g_stMrsCmdDefault, HI_ARRAY_COUNT(g_stMrsCmdDefault), HI_DIAG_CMD_INSTANCE_DEFAULT);

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
    HI_MDM_RegisterCommand(g_stMrsCmdIRemote, HI_ARRAY_COUNT(g_stMrsCmdIRemote), HI_DIAG_CMD_INSTANCE_IREMOTE);
#endif

    HI_MDM_RegisterCommand(g_stMrsCmdDRemote, HI_ARRAY_COUNT(g_stMrsCmdDRemote), HI_DIAG_CMD_INSTANCE_DREMOTE);

#if defined(PRODUCT_CFG_PRODUCT_TYPE_NDM)
    HI_MDM_RegisterCommand(g_stMrsCmdDRemoteNdm,HI_ARRAY_COUNT(g_stMrsCmdDRemoteNdm),HI_DIAG_CMD_INSTANCE_DREMOTE_NDM);
#endif
}

