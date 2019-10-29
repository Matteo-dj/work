//*****************************************************************************
//
//				  版权所有 (C), 深圳市国电科技通信有限公司
//
//*****************************************************************************
	  //  文 件 名	 : mrs_srv_rf.c
	  //  版 本 号	 : V1.0 
	  //  作	者	 : luodk
	  //  生成日期	 : 2018-6-06
	  //  功能描述	 : TODO: ...
	  //			   
	  //  函数列表	 : TODO: ...
	  //  修改历史	 : 
	  //  1.日	  期 : 2018-06-06
	  //	作	  者 : luodk
	  //	修改内容 : 创建文件 
	  //
//*****************************************************************************
#include "mrs_common.h"
#include "app_config.h"

#if defined(PRODUCT_CFG_SUPPORT_MRS_DUOL_MODE_RF)
#include "mrs_fw_log.h"
#include "mrs_fw_tools.h"
#include "mrs_srv_common.h"

#include "mrs_srv_res.h"
#include "mrs_cmd_msg.h"
#include "hi_mdm_time.h"
#include "hi_mdm_mac.h"

#include "mrs_srv_sta_queue.h"
#include "mrs_srv_list.h"
#include "mrs_srv_res.h"
#include "mrs_srv_parallel_sta.h"
#include "mrs_cmd_msg.h"
#include "mrs_fw_proto645.h"

#include "mrs_srv_sta_meter.h"
#include "mrs_fw_log.h"
#include "mrs_fw_tools.h"
#include "mrs_time.h"
#include "mrs_srv_list.h"
#include "mrs_srv_queue.h"
#include "mrs_srv_common.h"
#include "mrs_srv_sta_event.h"
#include "mrs_srv_sta_queue.h"
#include "mrs_srv_sta.h"

#include "mrs_dfx.h"
#include "mrs_fw_proto376_2_frame.h"
#include "mrs_fw_proto376_2_echo.h"
#include "mrs_fw_log.h"
#include "mrs_srv_common.h"
#include "mrs_srv_cco_macmap.h"
#include "mrs_srv_parallel_cco.h"
#include "mrs_power_failure.h"
#include "mrs_fw_n.h"
#include "mrs_srv_sta_event.h"
#include "mrs_srv_cco.h"
#include "mrs_srv_cco_event.h"
#include "mrs_fw_tools.h"
#include "mrs_srv_rf.h"
#include "mrs_srv_dau.h"
#include "rf_main.h"


//CJJ-HPLC-20181204:水表组网状态设置与查询
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
WATER_METER_NET_STATUE gWaterMeterNetStatue;

HI_VOID setWaterMeterNetStatue(WATER_METER_NET_STATUE statue)
{
	gWaterMeterNetStatue = statue;
}
WATER_METER_NET_STATUE   getWaterMeterNetStatue(HI_VOID)
{
	return gWaterMeterNetStatue;
}
#endif
//CJJ-HPLC-20181204


#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)

#define ID_MRS_CMD_RF_CMD                   (0x0040)

#define MRS_UART_CMD_TIMEOUT 10
typedef struct
{
    HI_U8              ulMinCI;
    HI_U8              ulMaxCI;
    MRS_RF_645_CI_FUN   func;
} MRS_RF_CI_PAIR_STRU;


//*****************************************************************************
//全局变量
//*****************************************************************************
HI_U32 g_CcoSnid=0; 			//获取的SNID
HI_U16 g_usCcoTei=0; 		//获取的Tei
HI_U16 g_usMaxTei=0xFFFF; 		//获取的Tei

HI_U32 g_CcoReqTimesCnt=0xFF;

#define STA_POWER_STATE_POWERON (0x01<<0)
#define STA_POWER_STATE_POWEROFF (0x01<<1)
#define STA_POWER_STATE_RECVPOWEROFF (0x01<<2)

static HI_U32 g_StaPowerStateflag=0; // 停电 接收停电信息状态，复电状态发送,因为只需要发送一次所以记录

static HI_U32 g_nNeedRfTransmitPowerInfo=0x0; // 是否需要无线转发

static HI_U32 g_StaMaxTeiFlag=0; // 设置最大的Tei标志


MRS_RF_RUNNING mrsRfRunUart;
MRS_RF_ALL_WATER_METER_INFO mrsRfAllWaterMeterInfo;
MRS_RF_PLC_RUN_INFO_STRU mrsRfPlcRnInfo;
//*****************************************************************************

// 
//0x10	获取HPLC应用层标准停电报文
//0x11	转发HPLC应用层标准停电报文
//0x12	获取SNID、TEI以及本网络TEI最大值
//0x13	获取表地址
HI_U32 mrsRf645_1FH_CI10(HI_VOID *param, HI_VOID **ppOutParam, HI_U16 *pOutParamLen, HI_U8 ucPreSize);
HI_U32 mrsRf645_1FH_CI11(HI_VOID *param, HI_VOID **ppOutParam, HI_U16 *pOutParamLen, HI_U8 ucPreSize);
HI_U32 mrsRf645_1FH_CI12(HI_VOID *param, HI_VOID **ppOutParam, HI_U16 *pOutParamLen, HI_U8 ucPreSize);
HI_U32 mrsRf645_1FH_CI13(HI_VOID *param, HI_VOID **ppOutParam, HI_U16 *pOutParamLen, HI_U8 ucPreSize);
HI_U32 mrsRf645_1FH_CI14(HI_VOID *param, HI_VOID **ppOutParam, HI_U16 *pOutParamLen, HI_U8 ucPreSize);


HI_U32 mrsRFDau0x25DecodeFrame(HI_U8 *pPayload, HI_U8 ucPayloadLen);


#define CI_10     (0x10)
#define CI_11     (0x11)
#define CI_12     (0x12)
#define CI_13     (0x13)
#define CI_14     (0x14)

MRS_RF_CI_PAIR_STRU MRS_RF_CI_PAIR_MAP_0x1FH[] =
{
    {CI_10, CI_10, mrsRf645_1FH_CI10},//0x10	获取HPLC应用层标准停电报文
	{CI_11, CI_11, mrsRf645_1FH_CI11},//0x11	转发HPLC应用层标准停电报文
	{CI_12, CI_12, mrsRf645_1FH_CI12},//0x12	获取SNID、TEI以及本网络TEI最大值
	{CI_13, CI_13, mrsRf645_1FH_CI13},//0x13	获取表地址
    {CI_14, CI_14, mrsRf645_1FH_CI14},//0x14 主动上报
};

HI_U32 mrsRf645Dec(HI_U8* pucData, HI_U16 usDataSize, MRS_RF_645_FRAME_STRU *pst645Frame);
HI_U32 APPMsgRcv(HI_U32 ulMdmMsgId, HI_PVOID pMdmMsg, HI_PVOID pData, HI_U32 ulOption);


HI_PRV HI_CONST HI_MDM_APPOBJ_S g_stAppObj =
{
    HI_MDM_OBJ_ID_USR2,      // APP对象ID
    APPMsgRcv    // APP接收MDM消息的处理入口
};

HI_U32 APPMsgRcv(HI_U32 ulMdmMsgId, HI_PVOID pMdmMsg, HI_PVOID pData, HI_U32 ulOption)
{
	HI_MDM_MSG_APPRCV_S* pstMsgRcv = (HI_MDM_MSG_APPRCV_S*)pMdmMsg;
	
	if(ulMdmMsgId == HI_MDM_NM_FREQ_QRY_ID) 
    { 
        //HI_MDM_NM_FREQ_QRY_IND_S* pstInd = (HI_MDM_NM_FREQ_QRY_IND_S*)pstMsgRcv->pucPacket; 
		
    } 
	else if(ulMdmMsgId == HI_MDM_NM_SNID_QRY_ID)
	{
		
		HI_MDM_NM_SNID_QRY_IND_S *pstInd = (HI_MDM_NM_SNID_QRY_IND_S *)pstMsgRcv->pucPacket;
		g_CcoSnid = pstInd->ulSnid;
	}
	else if(ulMdmMsgId == HI_MDM_NM_NODE_ID)
    {
		HI_MDM_NM_NODE_IND_S* pstInd = (HI_MDM_NM_NODE_IND_S*)pstMsgRcv->pucPacket; 
		g_usCcoTei = pstInd->uTei;
    } 

	return HI_ERR_SUCCESS;
}
HI_U32 AppMDMQuerySn(HI_U32 usMsgId)
{
	HI_U32 ret = HI_ERR_SUCCESS;
	HI_MDM_MSG_CTRL_S stCtrl; 
	//HI_MDM_Memzero(&stCtrl, sizeof(HI_MDM_MSG_CTRL_S));
	(hi_void)memset_s(&stCtrl, sizeof(HI_MDM_MSG_CTRL_S),0,sizeof(HI_MDM_MSG_CTRL_S));
	g_CcoReqTimesCnt++;
	stCtrl.ulSn = g_CcoReqTimesCnt;
	stCtrl.usDULink = HI_FALSE; 
	stCtrl.usTransId = HI_MDM_OBJ_ID_USR2;
	
	ret = HI_MDM_MsgSend(usMsgId, HI_NULL, 0, &stCtrl);
	return ret;
}

MRS_RF_ALL_WATER_METER_INFO *mrsRfGetAllWaterMeterInfo(HI_VOID)
{
	return &mrsRfAllWaterMeterInfo;
}
HI_U16 mrsGetTei(HI_VOID)
{
	return g_usCcoTei;
}
MRS_RF_PLC_RUN_INFO_STRU *mrsGetRfPlcRunInfo(HI_VOID)
{
	return (&mrsRfPlcRnInfo);
}

HI_U32 mrsRfInit(HI_VOID)
{
	g_StaPowerStateflag=0;
	g_nNeedRfTransmitPowerInfo=0x0;
	HI_MDM_AppInit(&g_stAppObj);
	
	mrsToolsMemZero_s(&mrsRfAllWaterMeterInfo,sizeof(mrsRfAllWaterMeterInfo),sizeof(mrsRfAllWaterMeterInfo));
	mrsSrvInitList(&(mrsRfAllWaterMeterInfo.pStaReportDataList));
	
	mrsToolsMemZero_s(&mrsRfRunUart,sizeof(mrsRfRunUart),sizeof(mrsRfRunUart));
	mrsToolsMemZero_s(&mrsRfPlcRnInfo,sizeof(mrsRfPlcRnInfo),sizeof(mrsRfPlcRnInfo));
	
	mrsSrvInitQueue(&(mrsRfRunUart.RfQueue));
	
	MRS_StartTimer(MRS_STA_TIMER_RF_TIME,100, HI_SYS_TIMER_PERIODIC);
	return HI_ERR_SUCCESS;
}
struct Rf_que_param
{
    MRS_RF_UART_NODE * item;    //节点
    HI_U8 result;           //结果
    HI_U8 padding[3];
};

HI_BOOL mrsStaRfQueueReplace(HI_VOID * node,HI_VOID * param)
{
    struct Rf_que_param * p = (struct Rf_que_param *)param;
    MRS_RF_UART_NODE * item = (MRS_RF_UART_NODE *)node;

    //参数有误
    if (!p || !node)
    {
        return HI_TRUE;
    }
    p->result = HI_FALSE;
	//HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_057, HI_DIAG_MT("判断重复"),p->item->ulRfCrc,item->ulRfCrc);

    if (p->item->ulRfCrc == item->ulRfCrc)
    {
       p->result=HI_TRUE;
       return HI_TRUE;
    }

    return HI_FALSE;
}

HI_BOOL mrsStaRfTryEnQueue(MRS_SRV_QUEUE *queue,MRS_RF_UART_NODE * item)
{
    HI_BOOL ret = HI_TRUE;
    if (queue && item)
    {
        struct Rf_que_param extra = {0};


        extra.item = item;
        extra.result = HI_FALSE;

        //相同会话的是不可以入队列的
        mrsSrvTraverseQueue(queue, mrsStaRfQueueReplace,&extra);

        //如果找到该业务的其它数据包
        if (extra.result)
        {
//            HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_056, HI_DIAG_MT("queue is 重复的"));
            return HI_TRUE;
        }

        mrsSrvEnQueue(queue,(MRS_SRV_NODE *)item);
    }
    return ret;
}

HI_U32 mrsRfInsertUartDataItem(HI_U16 usRfTimeOut,HI_U8 *ucData, HI_U16 usLen,HI_U8 nCi,HI_U8 ucMaxCount)
{
	MRS_SRV_QUEUE *pRfQueue=&(mrsRfRunUart.RfQueue);
	HI_U16 usRfDataLen=sizeof(MRS_RF_UART_NODE)+usLen;
    MRS_RF_UART_NODE *pstNode = mrsToolsMalloc(usRfDataLen);

    if (!pstNode)
    {
        return HI_ERR_MALLOC_FAILUE;
    }
	
    mrsToolsMemZero_s(pstNode, usRfDataLen, usRfDataLen);
	pstNode->usRfTimeOut = usRfTimeOut;
    pstNode->usNodeDataLen = usLen;
	pstNode->ucRfNodeCi = nCi;
	pstNode->ucRfNodeSendMaxTime = ucMaxCount;
	
    mrsToolsMemcpy_s(pstNode->uaNodeData, pstNode->usNodeDataLen, ucData, pstNode->usNodeDataLen); //采集器地址
	pstNode->ulRfCrc = HI_MDM_Crc32(0, pstNode->uaNodeData, pstNode->usNodeDataLen);
    //(HI_VOID)mrsSrvEnQueue(pRfQueue, &pstNode->RfLink);
   	mrsStaRfTryEnQueue(pRfQueue,pstNode);
    return HI_ERR_SUCCESS;
}
HI_U32 mrsRfDelUartDataItem(HI_VOID)
{
	MRS_SRV_QUEUE *pRfQueue=&(mrsRfRunUart.RfQueue);
    MRS_RF_UART_NODE *pstNode = (MRS_RF_UART_NODE *)mrsSrvDeQueue(pRfQueue);

    if (pstNode)
    {
        mrsToolsFree(pstNode);
    }
    return HI_ERR_SUCCESS;
}
HI_U32 mrsRfUartTimerFun(HI_VOID)
{
	static HI_U32 ulTimeCount=0;
	static HI_U32 ulTimeTickCountStateflag=0;
	static HI_U32 ulTimeTickCountMaxTeiFlag=0;
	HI_U8 Data[4];
	if(g_StaPowerStateflag)
	{
		ulTimeTickCountStateflag++;
		if(ulTimeTickCountStateflag%280==0)
		{
			ulTimeTickCountStateflag=0;
			g_StaPowerStateflag=0;
		}
	}
	if(g_StaMaxTeiFlag)
	{
		ulTimeTickCountMaxTeiFlag++;
		if(ulTimeTickCountMaxTeiFlag%30==0)
		{
			ulTimeTickCountMaxTeiFlag=0;
			g_StaMaxTeiFlag=0;
			mrsRfSetSnidTei();
		}
	}
	if(mrsRfPlcRnInfo.usSend0x25TimeCount>0)
	{
		mrsRfPlcRnInfo.usSend0x25TimeCount--;
		if(mrsRfPlcRnInfo.usSend0x25TimeCount==0)
		{
			mrsRfReadRSSI25(0);
		}
	}
	if(mrsRfPlcRnInfo.usRfTx0x44TimeCount>0)
	{
		mrsRfPlcRnInfo.usRfTx0x44TimeCount--;
		if(mrsRfPlcRnInfo.usRfTx0x44TimeCount==0)
		{
			mrsStaRfOwnerShipDataTransmit2CCO(Data,0,ID_MRS_CMD_DATA_TRANSMIT_AFN13_PLC);
//			HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_045, HI_DIAG_MT("[RF]report 0x44"));
		}
	}
	MRS_SRV_QUEUE *pRfQueue=&(mrsRfRunUart.RfQueue);
	if(mrsSrvQueueIsEmpty(pRfQueue))
	{
		mrsRfRunUart.ucRfLock = HI_FALSE;
		return HI_ERR_SUCCESS;
	}
	MRS_RF_UART_NODE *pstNode = (MRS_RF_UART_NODE *)mrsSrvQueueTop(pRfQueue);
	if(HI_NULL==pstNode)
	{
		return HI_ERR_SUCCESS;
	}
	if(mrsRfRunUart.ucRfLock)
	{
		ulTimeCount++;
		if(ulTimeCount > pstNode->usRfTimeOut)
		{
			mrsRfRunUart.ucRfSendTry++;
			if(mrsRfRunUart.ucRfSendTry< mrsRfRunUart.ucRfSendMaxTime)
			{
				ulTimeCount=0;
			  //CJJ-HPLC:20180921
				#if defined(SIGNAL_MCU)
				rfReceiveHplcDat(pstNode->uaNodeData,pstNode->usNodeDataLen);
				#else
				MRS_SendMrData(pstNode->uaNodeData, pstNode->usNodeDataLen, HI_DMS_CHL_UART_PORT_DIAG);
				#endif
				//HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1009, HI_DIAG_MT("RF Data"), pstNode->uaNodeData, pstNode->usNodeDataLen);
				//CJJ-HPLC:20180921
			}
			else
			{
				mrsRfDelUartDataItem();
				mrsRfRunUart.ucRfLock=HI_FALSE;
			}
		}
	}
	else
	{
		mrsRfRunUart.ucRfLock = HI_TRUE;
		mrsRfRunUart.ucRfSendTry=0;
		mrsRfRunUart.ucRfCi= pstNode->ucRfNodeCi;
		mrsRfRunUart.ucRfSendMaxTime = pstNode->ucRfNodeSendMaxTime;
		ulTimeCount=0;
		//CJJ-HPLC:20180921
		#if defined(SIGNAL_MCU) 
		rfReceiveHplcDat(pstNode->uaNodeData,pstNode->usNodeDataLen);
		#else
		MRS_SendMrData(pstNode->uaNodeData, pstNode->usNodeDataLen, HI_DMS_CHL_UART_PORT_DIAG);
		#endif
		//HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1010, HI_DIAG_MT("RF Data"), pstNode->uaNodeData, pstNode->usNodeDataLen);
		//CJJ-HPLC:20180921
	}
	return HI_ERR_SUCCESS;
}

HI_U32 mrsRfSendDataQueue(HI_U8 ucSubId,HI_PBYTE pucDatagram,HI_U8 ucDataRealmLen,HI_U8 ucBaudRateIndex)
{ 
    MRS_STA_ITEM *item = HI_NULL;
    MRS_STA_SRV_CTX_STRU * sta = mrsStaGetContext();
    
    item = (MRS_STA_ITEM *)mrsToolsMalloc(sizeof(MRS_STA_ITEM) + ucDataRealmLen);
    if (!item)
    {
        return  HI_ERR_MALLOC_FAILUE;
    }
    
    mrsToolsMemZero_s(item, sizeof(MRS_STA_ITEM) + ucDataRealmLen, sizeof(MRS_STA_ITEM) + ucDataRealmLen);
    item->id = ID_MRS_CMD_RF_CMD;
    item->buf = (HI_U8 *)item + sizeof(MRS_STA_ITEM);
    item->from = MRS_STA_QUEUE_FROM_RF;
    item->ucbaudrate_index = ucBaudRateIndex;
    item->len = ucDataRealmLen;
    item->time_out = 20; // 2s
    item->sub_id = ucSubId;
    mrsToolsMemcpy_s(item->buf, ucDataRealmLen, pucDatagram, ucDataRealmLen);
    item->ucProtocol = METER_PROTO_645_2007;
    item->bAutoStrategy = HI_TRUE;
    item->ucFrameTimeout = (HI_U8)MRS_MS_TO_100MS(mrsGet645FrameInterval(MRS_STA_RM_CFG_LOCAL));

    if (!mrsStaTryEnQueue(&(sta->stQueModule), item))
    {
        mrsToolsFree(item);
    }
    
    mrsStaNotifyQueue();
    return HI_ERR_SUCCESS;
            
}

MRS_RF_645_CI_FUN mrsRfFrameProc(HI_U8 ucCtrl, HI_U8 ucCI)
{
    MRS_RF_CI_PAIR_STRU * pair = HI_NULL;
    HI_S32 size = 0;
    HI_S32 index;

    switch (ucCtrl)
    {
        case 0x1F:
            pair = MRS_RF_CI_PAIR_MAP_0x1FH;
            size = MRS_TOOLS_ALEN(MRS_RF_CI_PAIR_MAP_0x1FH);
            break;
			
        default:
            return HI_NULL;
    }

    for (index = 0; index < size; index++)
    {
        if ((ucCI >= pair[index].ulMinCI) && (ucCI <= pair[index].ulMaxCI))
        {
            return pair[index].func;
        }
    }

    return HI_NULL;
}
HI_U32 mrsRf645Dec(HI_U8* pucData, HI_U16 usDataSize, MRS_RF_645_FRAME_STRU *pst645Frame)
{
    HI_U8 *pucBuffer = pucData;

    if (pucData == HI_NULL || pst645Frame == HI_NULL)
    {
        return HI_ERR_FAILURE;
    }

    pucBuffer++;
    mrsToolsMemcpy_s(pst645Frame->ucAddr, HI_METER_ADDR_LEN, pucBuffer, HI_METER_ADDR_LEN); //采集器地址

    //HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1004, HI_DIAG_MT("地址"), pst645Frame->ucAddr, HI_METER_ADDR_LEN);

    pucBuffer += HI_METER_ADDR_LEN;
    pucBuffer++;
    pst645Frame->ucCtrl = *(pucBuffer++); //控制码
    pst645Frame->ucDataRealmLen = *(pucBuffer++);
	pst645Frame->ucCl = *(pucBuffer++);

    mrsToolsMemcpy_s(pst645Frame->ucDataRealm, sizeof(pst645Frame->ucDataRealm), pucBuffer-1, pst645Frame->ucDataRealmLen);

	return HI_ERR_SUCCESS;
}

//*****************************************************************************
// 函数名称: mrsRfAssembleFrame
// 功能描述: 组织645回应帧
//           
// 参数说明:
//    ppFrame     [OUT]组织成功的帧
//    pFrmLen     [OUT]组织成功的帧长度
//    pPayload    [IN] 负载
//    ucPayloadLen[IN] 负载数据长度
//    p645Frame   [IN] 接收到的报文
//                            
// 返 回 值: 
//    组帧成功返回  HI_ERR_SUCCESS
//    组帧失败返回  HI_ERR_FAILURE
//*****************************************************************************
HI_U32 mrsRfAssembleFrame(HI_VOID **ppFrame, HI_U16 *pFrmLen, HI_U8 *pPayload, HI_U8 ucPayloadLen, 
								 MRS_RF_645_FRAME_STRU *p645Frame, HI_U8 ucPreSize)
{
    HI_U16 usBufLength = MRS_RF_645_MIN_LENGTH + ucPreSize + ucPayloadLen;
    HI_U8 * pBuffer = HI_NULL;
    HI_U16 usLength = 0;
    HI_U8 i = 0;

    if (((ucPayloadLen > 0) && (!pPayload)) || (!p645Frame))
    {
        return HI_ERR_FAILURE;
    }

    pBuffer = mrsToolsMalloc(usBufLength);
    if (!pBuffer)
    {
        return HI_ERR_MALLOC_FAILUE;
    }

    mrsToolsMemZero_s(pBuffer, usBufLength, usBufLength);
    // 前导FE
    for (i = 0; i < ucPreSize; i++)
    {
        pBuffer[usLength++] = 0xFE;
    }

    // HEAD
    pBuffer[usLength++] = MRS_645_FRAME_START_FLG;

    // ADDR
    mrsToolsMemcpy_s(pBuffer + usLength, HI_METER_ADDR_LEN, p645Frame->ucAddr, HI_METER_ADDR_LEN);
    usLength += HI_METER_ADDR_LEN;

    // HEAD 2
    pBuffer[usLength++] = MRS_645_FRAME_START_FLG;

    // CTRL
    pBuffer[usLength++] = p645Frame->ucCtrl | MRS_645_FRAME_CONTROL_DIR_UP;

    // LENGTH
    pBuffer[usLength++] = ucPayloadLen;

    // DATA
    for (i = 0; i < ucPayloadLen; i++)
    {
        pBuffer[usLength++] = pPayload[i];
    }

    // CS
    pBuffer[usLength] = mrsToolsCalcCheckSum(pBuffer + ucPreSize, usLength - ucPreSize);
    usLength++;

    // END
    pBuffer[usLength++] = MRS_645_FRAME_END_FLG;

    *ppFrame = pBuffer;
    *pFrmLen = usLength;
    return HI_ERR_SUCCESS;
}


HI_U32 mrsRfAssembleDFrame(HI_VOID **ppFrame, HI_U16 *pFrmLen, HI_U8 ucErrCode,MRS_RF_645_FRAME_STRU *p645Frame, HI_U8 ucPreSize)
{
    if (!p645Frame)
    {
        return HI_ERR_FAILURE;
    }

    p645Frame->ucCtrl |= MRS_645_FRAME_CONTROL_DENY_MASK;
	
    return mrsRfAssembleFrame(ppFrame, pFrmLen, &ucErrCode, sizeof(ucErrCode), p645Frame, ucPreSize);
}

HI_VOID mrsOneSetRfSetAddress(HI_VOID)
{
	//static HI_U8 flag=1;
	//if(flag)
	{
		//flag=0;
		mrsRfSetAddress();
	}
}

//*****************************************************************************
// 函数名称: mrsStaRFRecvDealFun
// 功能描述: 获取从无线接收过来的完整帧
//           
// 参数说明:
//    pstRxData [IN]接收到的无线的数据
//                            
// 返 回 值: 
//    成功返回  HI_ERR_SUCCESS
//    失败返回  HI_ERR_FAILURE
//    继续接收数据 HI_ERR_CONTINUE
//*****************************************************************************
HI_U32 mrsStaRFRecvDealFun(HI_DMS_CHL_RX_S* pstRxData)
{
    HI_U32 ret=HI_ERR_SUCCESS;
	HI_U32 i=0;
    MRS_ONE_RAW_FRAME_STRU stFrame = {0};
    MRS_RF_645_FRAME_STRU *pst645Frame = HI_NULL;
    MRS_RF_645_CI_FUN pIrDIProc = HI_NULL;
	MRS_RF_DAU_FRAME_STRU *pDauFrame=HI_NULL;

    //HI_PBYTE pucDatagram = HI_NULL;
    //HI_U8 ucDataRealmLen = 0;
    HI_U8 ucBaudRateIndex = 0;
	HI_U8 ucSubId=MRS_RF_SUBID_TRANSMIT_TOOL;
	MRS_RF_ALL_WATER_METER_INFO *pWaterMeterInfo =mrsRfGetAllWaterMeterInfo();
    HI_U8 *pData = HI_NULL;
    HI_U16 usDataLen = 0;

    if (pstRxData == HI_NULL)
    {
        ret = HI_ERR_FAILURE;
        return ret;
    }
	mrs645ProtoStreamInputCheck((HI_BOOL)pstRxData->bCanDiscardOldData, ID_MRS_RF_645BUF);

    // 获取到完整的帧数据
    ret = mrs645ProtoStreamInput(pstRxData->pPayload, pstRxData->usPayloadLen, &stFrame, ID_MRS_RF_645BUF);
    if (HI_ERR_SUCCESS != ret)
    {
        return ret;
    }

    //HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_011, HI_DIAG_MT("[RF]收到RF帧"));
    HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1001, HI_DIAG_MT("[RF]收到数据内容"), stFrame.pucDatagram, stFrame.usDatagramSize);

    do
    {
        //解析645帧
        pst645Frame = (MRS_RF_645_FRAME_STRU *)mrsToolsMalloc(sizeof(MRS_RF_645_FRAME_STRU));
        if(pst645Frame == HI_NULL)
        {
            //HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_001, HI_DIAG_MT("[RF]no memory"));
            ret = HI_ERR_FAILURE;
            break;
        }

        mrsToolsMemZero_s(pst645Frame, sizeof(MRS_RF_645_FRAME_STRU), sizeof(MRS_RF_645_FRAME_STRU));

        //检查645帧内容是否正确
        ret = mrsRf645Dec(stFrame.pucDatagram, stFrame.usDatagramSize, pst645Frame);
        if ((ret == HI_ERR_SUCCESS)&&(pst645Frame->ucCtrl&0x1F==0x1F)) 
        {
        	//HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_022, HI_DIAG_MT("[RF]CI"),pst645Frame->ucCl);
			if((pst645Frame->ucCl>=0x20) &&(pst645Frame->ucCl<0x24))
			{
				mrsRfDelUartDataItem();
				mrsRfRunUart.ucRfLock=HI_FALSE;
				break;
			}
			if(mrsRfRunUart.ucRfLock)
			{
				if(pst645Frame->ucCl != mrsRfRunUart.ucRfCi)
				{
					//HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_043, HI_DIAG_MT("[RF]Cl,Ci"),pst645Frame->ucCl,mrsRfRunUart.ucRfCi);
					break;
				}
				switch(mrsRfRunUart.ucRfCi)
				{
				case 0x24:
					pDauFrame = mrsRFDauDecode(pst645Frame->ucDataRealm+2,pst645Frame->ucDataRealmLen-2);
					if(pDauFrame==HI_NULL)
					{
						break;
					}
					
					for(i=0;i<pWaterMeterInfo->ucWaterMeterCount;i++)
					{
						if(0==mrsToolsMemcmp(pWaterMeterInfo->saWaterMeterInfo[i].ucaWaterMeterAddr,pDauFrame->ucaDataSrcAddr,7))
						{
							pWaterMeterInfo->saWaterMeterInfo[i].ucFieldValue =pst645Frame->ucDataRealm[1];
							break;
						}
					}
					if(i >= pWaterMeterInfo->ucWaterMeterCount)
					{
						mrsToolsMemcpy_s(pWaterMeterInfo->saWaterMeterInfo[pWaterMeterInfo->ucWaterMeterCount].ucaWaterMeterAddr, 7, pDauFrame->ucaDataSrcAddr, 7);
						pWaterMeterInfo->saWaterMeterInfo[pWaterMeterInfo->ucWaterMeterCount].ucFieldValue =pst645Frame->ucDataRealm[1];
						pWaterMeterInfo->ucWaterMeterCount++;
					}
					if(pDauFrame->ucDataCmdId == 0x45)
					{
						/*
						if(mrsRfPlcRnInfo.pOwnerInfo==HI_NULL)
						{
							break;
						}
						for(i=0;i<mrsRfPlcRnInfo.ucOwnerInfoNum;i++)
						{
							if(0==mrsToolsMemcmp(mrsRfPlcRnInfo.pOwnerInfo+i*8,pDauFrame->ucaDataSrcAddr,7))
							{
								mrsRfPlcRnInfo.pOwnerInfo[i*8+7] =(pDauFrame->ucapDataData[0]+1);
								break;
							}
						}
						
						HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_095, HI_DIAG_MT("[RF]Len"),mrsSrvQueueLength(&(mrsRfRunUart.RfQueue)));
						if(mrsSrvQueueLength(&(mrsRfRunUart.RfQueue))==1)
						{
							mrsRfPlcRnInfo.usRfTx0x44TimeCount=0;
							mrsStaRfOwnerShipDataTransmit2CCO(pDauFrame->ucapDataData+3,pDauFrame->ucapDataData[2],ID_MRS_CMD_DATA_TRANSMIT_AFN13_PLC);
						}
						*/
						mrsStaRfDataTransmit2CCO(pst645Frame->ucDataRealm+2,pst645Frame->ucDataRealmLen-2,ID_MRS_CMD_DATA_TRANSMIT_AFN13_PLC);
					}
					else if(pDauFrame->ucDataCmdId == 0x41)
					{
						//mrsStaRfDataTransmit2CCO(pDauFrame->ucapDataData+1,pDauFrame->ucapDataData[0],ID_MRS_CMD_DATA_TRANSMIT_AFN13_PLC);
						mrsStaRfDataTransmit2CCO(pst645Frame->ucDataRealm+2,pst645Frame->ucDataRealmLen-2,ID_MRS_CMD_DATA_TRANSMIT_AFN13_PLC);
					}
					else
					{
						//HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_044, HI_DIAG_MT("[RF]pDauFrame->ucDataCmdId"),pDauFrame->ucDataCmdId);
					}
					break;
					
				case 0x25:
					ret = mrsRFDau0x25DecodeFrame(pst645Frame->ucDataRealm+1,pst645Frame->ucDataRealmLen-1);
					break;
				}
				mrsToolsFree(pDauFrame);
				if(ret==HI_ERR_SUCCESS)
				{
					mrsRfDelUartDataItem();
					mrsRfRunUart.ucRfLock=HI_FALSE;
				}
				break;
			}
        	
			
            pIrDIProc = mrsRfFrameProc(pst645Frame->ucCtrl&0x1F, pst645Frame->ucCl);
            if (pIrDIProc)
            {
                ret = pIrDIProc(pst645Frame, (HI_PVOID *)&pData, &usDataLen, MRS_PREFIX_FE_SIZE);
                if (ret != HI_ERR_SUCCESS)
                {
                    mrsRfAssembleDFrame((HI_VOID **)&pData, &usDataLen, ret, 
									  pst645Frame, MRS_PREFIX_FE_SIZE);
                }

                break;
            }
			if(pst645Frame->ucCl!=0x24)
			{
        		MRS_SendMrData(stFrame.pucDatagram, stFrame.usDatagramSize, HI_DMS_CHL_UART_PORT_APP);
			}
			break;
        }
		else if((ret == HI_ERR_SUCCESS)&&((pst645Frame->ucCtrl&0x1F)==0x4))
		{
			MRS_SendMrData(stFrame.pucDatagram, stFrame.usDatagramSize, HI_DMS_CHL_UART_PORT_APP);
			break;
		}
		ucBaudRateIndex = 0;
        ret = mrsRfSendDataQueue(ucSubId,stFrame.pucDatagram, stFrame.usDatagramSize,ucBaudRateIndex);
    } while (0);

    if ((usDataLen > 0) && pst645Frame)
    {
      //CJJ-HPLC:20180921  
      #if defined(SIGNAL_MCU)
			rfReceiveHplcDat(pData,usDataLen);
				#if 0
	      HI_SYS_QUEUE_MSG_S stMsg;
	      HI_U32 gpio_para = uartRecMsg;
	      
	      mrsToolsMemset_s(&stMsg, sizeof(stMsg), 0, sizeof(stMsg));
	      stMsg.ulMsgId = ID_RF_WIRE_LESS_REPORT;
	      stMsg.ulTimeStamp = HI_MDM_GetMilliSeconds();
	      stMsg.ulParam[0] = gpio_para;
	      stMsg.ulParam[1] = (HI_U32)(usDataLen);
	      stMsg.ulParam[2] = (HI_U32)(pData);
	      (HI_VOID)MRS_SendMessage(EN_APP_QUEUE_MRS, &stMsg, 0);
				#endif
      #else
      MRS_SendMrData(pData, usDataLen, HI_DMS_CHL_UART_PORT_DIAG);
      #endif
      //HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1003, HI_DIAG_MT("回复数据"), pData, usDataLen);
    }

    mrsToolsFree(stFrame.pucDatagram);
    mrsToolsFree(pData);
    mrsToolsFree(pst645Frame);

    return ret;
}

HI_U32 mrsRf645_1FH_CI10(HI_VOID *param, HI_VOID **ppOutParam, HI_U16 *pOutParamLen, HI_U8 ucPreSize)
{
	HI_U32 ret=HI_ERR_SUCCESS;
	//MRS_RF_645_FRAME_STRU* frame = (MRS_RF_645_FRAME_STRU*)param;
	HI_U16 usPayloadLen=0,i=0;
	HI_U8 *pcTemp=HI_NULL;
	HI_U16 usOffset=0;
	/*
	if(g_StaPowerStateflag&(STA_POWER_STATE_RECVPOWEROFF|STA_POWER_STATE_POWEROFF)==0)
	{
		return HI_ERR_NOT_FOUND;
	}
	*/
	HI_U8 *pucPayload= mrsRfGetPowerFailureInfo(&usPayloadLen,g_nNeedRfTransmitPowerInfo);
	if(HI_NULL == pucPayload)
	{
		return HI_ERR_MALLOC_FAILUE;
	}
	pcTemp=(HI_U8*)mrsToolsMalloc(usPayloadLen+2+4);
	if(HI_NULL == pcTemp)
	{
		mrsToolsFree(pucPayload);
		return HI_ERR_MALLOC_FAILUE;
	}
	pcTemp[usOffset++]= 0x10;
	 if(g_nNeedRfTransmitPowerInfo>0)
	 {
	 	pcTemp[usOffset++]= 0x1;
	 }
	 else
	 {
	 	pcTemp[usOffset++]= 0;
	 }
	 
	 pcTemp[usOffset++]=0x11;
	 pcTemp[usOffset++]=0x8;
	 pcTemp[usOffset++]=0x0;
	 pcTemp[usOffset++]=0x0;
	 for(i=0;i<usPayloadLen;i++)
	 {
	 	pcTemp[usOffset+i]=pucPayload[i];
	 }
	 //Matteo
	 HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_055, HI_DIAG_MT("[Matteo] wireless sta send"));
	ret =mrsRfAssembleFrame(ppOutParam, pOutParamLen, pcTemp, usPayloadLen+usOffset, 
								(MRS_RF_645_FRAME_STRU*)param, ucPreSize);

	g_nNeedRfTransmitPowerInfo=g_nNeedRfTransmitPowerInfo>0?(g_nNeedRfTransmitPowerInfo-1):0;
	mrsToolsFree(pucPayload);
	mrsToolsFree(pcTemp);
	return ret;
}

HI_U32 mrsRf645_1FH_CI11(HI_VOID *param, HI_VOID **ppOutParam, HI_U16 *pOutParamLen, HI_U8 ucPreSize)
{
	HI_U32 ret=HI_ERR_SUCCESS;
	HI_U8 payload=0x11;
	MRS_RF_645_FRAME_STRU* frame = (MRS_RF_645_FRAME_STRU*)param;
	if(frame->ucDataRealmLen<17)
	 {
	 	//HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_008, HI_DIAG_MT("[RF]frame->ucDataRealmLen"),frame->ucDataRealmLen);
	 	return HI_ERR_BAD_FRAME;
	 }
	if(frame->ucDataRealm[1]!=0x11 ||frame->ucDataRealm[2]!=0x8)
	{
		
	 	//HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_029, HI_DIAG_MT("[RF]参数错误"));
		return HI_ERR_INVALID_PARAMETER;
	}
	//HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_029, HI_DIAG_MT("[RF]参数错误"),&(frame->ucDataRealm[5]));
	ret =mrsRfSetPowerFailureInfo(&(frame->ucDataRealm[5]),&g_nNeedRfTransmitPowerInfo);
	if(ret!=HI_ERR_SUCCESS)
	{
		
	 	//HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_028, HI_DIAG_MT("[RF]ret"),ret);
		return ret;
	}
	ret =mrsRfAssembleFrame(ppOutParam, pOutParamLen, &payload, sizeof(payload), 
								(MRS_RF_645_FRAME_STRU*)param, ucPreSize);
	
	return ret;

}

HI_U32 mrsRf645_1FH_CI12(HI_VOID *param, HI_VOID **ppOutParam, HI_U16 *pOutParamLen, HI_U8 ucPreSize)
{
	HI_U32 ret=HI_ERR_SUCCESS;
	HI_U8 payload[16];
	HI_U32 noffset=0;
	AppMDMQuerySn(HI_MDM_NM_SNID_QRY_ID);
	AppMDMQuerySn(HI_MDM_NM_NODE_ID);
	/*
	12	CI扩展标识符
	SNID	3Byte
	TEI 2字节的TEI
	MaxTEI	2字节的最大TEI	
	*/
	payload[noffset++]=0x12;

	payload[noffset++]=g_CcoSnid&0xFF;
	payload[noffset++]=(g_CcoSnid>>8)&0xFF;
	payload[noffset++]=(g_CcoSnid>>16)&0xFF;

	payload[noffset++]=g_usCcoTei&0xFF;
	payload[noffset++]=(g_usCcoTei>>8)&0xFF;

	payload[noffset++]=g_usMaxTei&0xFF;
	payload[noffset++]=(g_usMaxTei>>8)&0xFF;
	ret =mrsRfAssembleFrame(ppOutParam, pOutParamLen, payload, noffset, (MRS_RF_645_FRAME_STRU*)param, ucPreSize);
	return ret;

}

HI_U32 mrsRf645_1FH_CI13(HI_VOID *param, HI_VOID **ppOutParam, HI_U16 *pOutParamLen, HI_U8 ucPreSize)
{
	HI_U32 ret=HI_ERR_SUCCESS;
	MRS_STA_SRV_CTX_STRU* pstaCtx = mrsStaGetContext();
	HI_U8 payload[8];
	//HI_U8 ucaMac[HI_METER_ADDR_LEN]={0};
	HI_U32 noffset=0;
	payload[noffset++]=0x13;
	
    //是否是有效的MAC地址
    /*
    if (mrsToolsInvalidMac(pstaCtx->ucMacAddr))
    {
    	HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_033, HI_DIAG_MT("[RF]没有读到地址"));
        return HI_ERR_NOT_CONNECT;
    }
	HI_MDM_GetProductId(ucaMac, HI_NULL, HI_NULL, HI_NULL);
	if(mrsToolsMemEq(pstaCtx->ucMacAddr, ucaMac, HI_PLC_MAC_ADDR_LEN))
	{
		HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_034, HI_DIAG_MT("[RF]没有读到表地址,MAC地址"));
		return HI_ERR_NOT_CONNECT;
	}
	*/
	
    mrsToolsMemcpy_s(payload + noffset, HI_PLC_MAC_ADDR_LEN, pstaCtx->ucMacAddr, HI_PLC_MAC_ADDR_LEN);
	mrsHexInvert(payload + noffset,HI_METER_ADDR_LEN);
	noffset += HI_METER_ADDR_LEN;
	ret =mrsRfAssembleFrame(ppOutParam, pOutParamLen, payload, noffset, (MRS_RF_645_FRAME_STRU*)param, ucPreSize);
	return ret;
}
HI_U32 mrsRf645_1FH_CI14(HI_VOID *param, HI_VOID **ppOutParam, HI_U16 *pOutParamLen, HI_U8 ucPreSize)
{
	MRS_RF_ALL_WATER_METER_INFO *pWaterMeterInfo =mrsRfGetAllWaterMeterInfo();
	MRS_SRV_LINK * it;
	MRS_SRV_LIST * list;
	MRS_RF_REPORT_DATA_NODE *pTempNode=HI_NULL;
	HI_U32 ulNowTime=HI_MDM_GetSeconds();

	list = &(pWaterMeterInfo->pStaReportDataList);
	HI_U32 ret=HI_ERR_SUCCESS;
	HI_U32 i=0;
	HI_U8 payload=0x14;
	//HI_U16 usCRC16=0;
	HI_U16 usDataLen=0;
	
	MRS_RF_645_FRAME_STRU* frame = (MRS_RF_645_FRAME_STRU*)param;
	
    //HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1006, HI_DIAG_MT("[RF]内容"), frame->ucDataRealm+2, frame->ucDataRealmLen-2);
	MRS_RF_DAU_FRAME_STRU *pDauFrame = mrsRFDauOnwWayDecode(frame->ucDataRealm+2,frame->ucDataRealmLen-2);
	if(pDauFrame==HI_NULL)
	{
		return HI_ERR_BAD_FRAME;
	}
	
    //HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1007, HI_DIAG_MT("[RF]内容2"), pDauFrame->ucapDataData, pDauFrame->ucDataLen);
	for(i=0;i<pWaterMeterInfo->ucWaterMeterCount;i++)
	{
		if(0==mrsToolsMemcmp(pWaterMeterInfo->saWaterMeterInfo[i].ucaWaterMeterAddr,pDauFrame->ucaDataSrcAddr,6))
		{
			pWaterMeterInfo->saWaterMeterInfo[i].ucFieldValue =frame->ucDataRealm[1];
			break;
		}
	}
	if((pWaterMeterInfo->ucWaterMeterCount<RF_MAX_WATER_METER_COUNT)&&(i >= pWaterMeterInfo->ucWaterMeterCount))
	{
		mrsToolsMemcpy_s(pWaterMeterInfo->saWaterMeterInfo[pWaterMeterInfo->ucWaterMeterCount].ucaWaterMeterAddr, 7, pDauFrame->ucaDataSrcAddr, 7);
		pWaterMeterInfo->saWaterMeterInfo[pWaterMeterInfo->ucWaterMeterCount].ucFieldValue =frame->ucDataRealm[1];
		pWaterMeterInfo->ucWaterMeterCount++;
	}
	//HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_040, HI_DIAG_MT("[RF]C14 RSSI"),frame->ucDataRealm[1]);
	usDataLen = sizeof(MRS_RF_REPORT_DATA_NODE)+pDauFrame->ucDataLen;
	
	//HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_074, HI_DIAG_MT("[RF]C14 usDataLen"),usDataLen,pDauFrame->ucDataLen);
	MRS_RF_REPORT_DATA_NODE *node = (MRS_RF_REPORT_DATA_NODE*)mrsToolsMalloc(usDataLen);
	if(!node)
	{
		//HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_075, HI_DIAG_MT("[RF]C14 mrsToolsMalloc Fail"));
		mrsToolsFree(pDauFrame);
		return HI_ERR_MALLOC_FAILUE;
	}
	mrsToolsMemZero_s(node,sizeof(MRS_RF_REPORT_DATA_NODE),sizeof(MRS_RF_REPORT_DATA_NODE));
	node->ucReportDataLen = pDauFrame->ucDataLen;
	node->ulStartTime = HI_MDM_GetSeconds();
	mrsToolsMemcpy_s(node->ucRecvWaterMeterAddr,6,pDauFrame->ucaDataSrcAddr,6);
	mrsToolsMemcpy_s(node->ucPStaReportData, pDauFrame->ucDataLen,pDauFrame->ucapDataData, pDauFrame->ucDataLen);
	
    //HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1008, HI_DIAG_MT("[RF]内容3"), node->ucPStaReportData, node->ucReportDataLen);
	if(pWaterMeterInfo->ucReportDataCount >64)
	{
		mrsSrvListRemoveHeadNode(&(pWaterMeterInfo->pStaReportDataList));
	}
	
	// 遍历链表
    for (it = list->next; it != list; it = it->next)
    {
        pTempNode = (MRS_RF_REPORT_DATA_NODE *)it;
		if((0==mrsToolsMemcmp(pTempNode->ucRecvWaterMeterAddr,node->ucRecvWaterMeterAddr,6))
			|| ((ulNowTime - pTempNode->ulStartTime)>259200))
		{
			// 地址相同，日期大于3天删除 更新数据
			pWaterMeterInfo->ucReportDataCount--;
			mrsSrvListRemove(list,(MRS_SRV_LINK *)pTempNode);
			mrsToolsFree(pTempNode);
			//HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_094, HI_DIAG_MT("[RF]Del one ucaStaReportData,Count"),pWaterMeterInfo->ucReportDataCount);			
			break;
		}
    }		

	mrsSrvListInsTail(&(pWaterMeterInfo->pStaReportDataList),&(node->link));
	pWaterMeterInfo->ucReportDataCount++;
	ret =mrsRfAssembleFrame(ppOutParam, pOutParamLen, &payload, sizeof(payload), 
								(MRS_RF_645_FRAME_STRU*)param, ucPreSize);
	mrsToolsFree(pDauFrame);
	
	return ret;
}

HI_U32 mrsRfGen645Frame(HI_IN MRS_RF_645_FRAME_STRU *pFrameInf, HI_OUT HI_U8 **ppFrame, HI_OUT HI_U16 *pFrameLen)
{
    HI_U16 usBufferLength = MRS_RF_645_MIN_LENGTH + pFrameInf->ucDataRealmLen;
    HI_U8 *pBuffer = HI_NULL;
    HI_U16 usOffset = 0;

    pBuffer = mrsToolsMalloc(usBufferLength);
    if (!pBuffer)
    {
        return HI_ERR_MALLOC_FAILUE;
    }

    mrsToolsMemZero_s(pBuffer, usBufferLength, usBufferLength);

    
    // HEAD
    pBuffer[usOffset++] = MRS_645_FRAME_START_FLG;

    // ADDR
    mrsToolsMemcpy_s(pBuffer + usOffset, usBufferLength - usOffset, pFrameInf->ucAddr, HI_METER_ADDR_LEN);
    usOffset += HI_METER_ADDR_LEN;

    // HEAD 2
    pBuffer[usOffset++] = MRS_645_FRAME_START_FLG;

    // CTRL
    pBuffer[usOffset++] = pFrameInf->ucCtrl| MRS_645_FRAME_CONTROL_DIR_UP;

    // LENGTH
    pBuffer[usOffset++] = pFrameInf->ucDataRealmLen;

 
    // DATA
    if (pFrameInf->ucDataRealmLen > 0)
    {
        mrsToolsMemcpy_s(pBuffer + usOffset, usBufferLength - usOffset, pFrameInf->ucDataRealm, pFrameInf->ucDataRealmLen);
        usOffset += pFrameInf->ucDataRealmLen;
    }

    // CS
    pBuffer[usOffset] = mrsToolsCalcCheckSum(pBuffer, usOffset);
    usOffset++;

    // END
    pBuffer[usOffset++] = MRS_645_FRAME_END_FLG;

    *ppFrame = pBuffer;
    *pFrameLen = usOffset;

    return HI_ERR_SUCCESS;
}


HI_U32 mrsRfCreateFrameTransmit(HI_PBYTE *ppFrame,HI_U16 *pusLength, HI_PBYTE pbuf, HI_U8 pbuflen)
{
    MRS_RF_645_FRAME_STRU stFrameInf;

    mrsToolsMemZero_s(&stFrameInf, sizeof(stFrameInf), sizeof(stFrameInf));
	mrsStaGetMacAddr(stFrameInf.ucAddr);
	mrsHexInvert(stFrameInf.ucAddr,HI_METER_ADDR_LEN);
    //采集器地址
    stFrameInf.ucCtrl = 0x1F;
    mrsToolsMemcpy_s(stFrameInf.ucDataRealm,RF_DATA_BUF_MAX,pbuf,pbuflen);
    stFrameInf.ucDataRealmLen = pbuflen;
       
    return mrsRfGen645Frame(&stFrameInf, ppFrame, pusLength);
    
}


HI_VOID mrsRfTransmit(HI_PBYTE pucData, HI_U16 usDataSize,HI_U16 usTimeout,HI_U8 nCi)
{
   
    HI_PBYTE pFrame = HI_NULL;
    HI_U16 usFrameLength = 0;
    
    if (mrsRfCreateFrameTransmit(&pFrame, &usFrameLength,pucData,(HI_U8)usDataSize) == HI_ERR_SUCCESS)
    {
		mrsRfInsertUartDataItem(usTimeout,pFrame,usFrameLength,nCi,2);
    }
    
    mrsToolsFree(pFrame);
}

HI_VOID mrsRfTransmitTimeAndCount(HI_PBYTE pucData, HI_U16 usDataSize,HI_U16 usTimeout,HI_U8 nCi,HI_U8 ucSendCount)
{
   
    HI_PBYTE pFrame = HI_NULL;
    HI_U16 usFrameLength = 0;
    
    if (mrsRfCreateFrameTransmit(&pFrame, &usFrameLength,pucData,(HI_U8)usDataSize) == HI_ERR_SUCCESS)
    {
		mrsRfInsertUartDataItem(usTimeout,pFrame,usFrameLength,nCi,ucSendCount);
    }
    
    mrsToolsFree(pFrame);
}

HI_VOID mrsRfDirectTransmit(HI_PBYTE pucData, HI_U16 usDataSize)
{
	//CJJ-HPLC:调试串口的数据通过消息发送给RF任务处理
#if defined(SIGNAL_MCU)
	rfReceiveHplcDat(pucData,usDataSize);
#else
	MRS_SendMrData(pucData, usDataSize, HI_DMS_CHL_UART_PORT_DIAG);
#endif
	//HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1011, HI_DIAG_MT("mrsRfDirectTransmit"), pucData, usDataSize); 
}

HI_VOID mrsRfSetSnidTei(HI_VOID)
{
	HI_U8 payload[16];
	HI_U32 noffset=0;
	AppMDMQuerySn(HI_MDM_NM_SNID_QRY_ID);
	AppMDMQuerySn(HI_MDM_NM_NODE_ID);
	/*
	12	CI扩展标识符
	SNID	3Byte
	TEI 2字节的TEI
	MaxTEI	2字节的最大TEI	
	*/
	payload[noffset++]=0x20;

	payload[noffset++]=g_CcoSnid&0xFF;
	payload[noffset++]=(g_CcoSnid>>8)&0xFF;
	payload[noffset++]=(g_CcoSnid>>16)&0xFF;

	payload[noffset++]=g_usCcoTei&0xFF;
	payload[noffset++]=(g_usCcoTei>>8)&0xFF;

	payload[noffset++]=g_usMaxTei&0xFF;
	payload[noffset++]=(g_usMaxTei>>8)&0xFF;
	mrsRfTransmit(payload,noffset,MRS_UART_CMD_TIMEOUT,0x20);
	
}
HI_VOID mrsRfSetTimeslot(HI_U16 usTimeslot)
{
  HI_U8 payload[32];
	HI_U32 noffset=0;
	payload[noffset++]=MRS_645_FRAME_START_FLG;
	mrsStaGetMacAddr(payload + noffset);
	mrsHexInvert(payload + noffset,HI_METER_ADDR_LEN);
	noffset += HI_METER_ADDR_LEN;
	

	payload[noffset++]=MRS_645_FRAME_START_FLG;

	payload[noffset++]=0x1F| MRS_645_FRAME_CONTROL_DIR_UP;
	payload[noffset++]=3;

	payload[noffset++]=0x21;
	payload[noffset++]=usTimeslot&0xFF;
	payload[noffset++]=(usTimeslot>>8)&0xFF;
	
	payload[noffset] = mrsToolsCalcCheckSum(payload, noffset);
	noffset++;
	
	payload[noffset++] = MRS_645_FRAME_END_FLG;

	mrsRfDirectTransmit(payload,noffset);
}
HI_VOID mrsRfSetAddress(HI_VOID)
{
	HI_U8 payload[8];
	HI_U32 noffset=0;
	payload[noffset++]=0x22;
	mrsStaGetMacAddr(payload + noffset);
	mrsHexInvert(payload + noffset,HI_METER_ADDR_LEN);
	noffset += HI_METER_ADDR_LEN;
	mrsRfTransmit(payload,noffset,MRS_UART_CMD_TIMEOUT,0x22);
}
HI_VOID mrsRfSetStateInfo(HI_U8 nFlag)
{
	HI_U8 payload[8];
	HI_U32 noffset=0;
	payload[noffset++]=0x23;
	payload[noffset++]=nFlag;
	/*
	if(nFlag==0)
	{
		g_StaPowerStateflag =(0x01<<nFlag);
	}
	else
	{
		if(g_StaPowerStateflag&(STA_POWER_STATE_RECVPOWEROFF|STA_POWER_STATE_POWEROFF))
		{
			HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_030, HI_DIAG_MT("[RF]g_StaPowerStateflag"),g_StaPowerStateflag);
			return ;
		}
		g_StaPowerStateflag |=(0x01<<nFlag);
	}
	*/
	
	mrsRfTransmit(payload,noffset,MRS_UART_CMD_TIMEOUT,0x23);
}
HI_VOID mrsRfSetDirectTransfer24(HI_U8 *pData,HI_U16 usDateLen)
{
	HI_U8 *payload=(HI_U8 *)mrsToolsMalloc(256);
	HI_U32 noffset=0;
	if(payload==HI_NULL)
	{
		//HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_079, HI_DIAG_MT("[RF]DirectTransfer24 Malloc Fail"));
		return ;
	}
	
	mrsToolsMemZero_s(payload, 256, 256);
	payload[noffset++]=0x24;
	mrsToolsMemcpy_s(payload+noffset,255,pData,usDateLen);
	noffset += usDateLen;

	mrsRfTransmit(payload,noffset,MRS_UART_CMD_TIMEOUT*5,0x24);
	mrsToolsFree(payload);
}
HI_VOID mrsRfReadRSSI25(HI_U8 ucPage)
{
	HI_U8 payload[2];
	HI_U32 noffset=0;
	payload[noffset++]=0x25;
	payload[noffset++]=ucPage;
	mrsRfTransmit(payload,noffset,MRS_UART_CMD_TIMEOUT*2,0x25);
}

HI_VOID mrsRfClearPowerState(HI_VOID)
{
	g_StaPowerStateflag=0;
}
HI_VOID mrsRfSetNeedRfTransmitPowerInfo(HI_U32 ulNeedSendCount)
{
	g_nNeedRfTransmitPowerInfo=ulNeedSendCount;
}
HI_VOID mrsRfSetMaxTei(HI_U16 usMaxTei)
{
	if(g_usMaxTei!=usMaxTei)
	{
		g_StaMaxTeiFlag=1;
	}
	g_usMaxTei=usMaxTei;
}
/*
HI_VOID mrsRfSetMeterOwnerShip(HI_U8 ucMeterCount,HI_U8 *ucaData,HI_U8 *nTime)
{
	MRS_STA_SRV_CTX_STRU* pstaCtx = mrsStaGetContext();
	HI_U32 i=0,noffent=0,k=0;
	HI_U8 ucData[32];
	HI_U8 ucMeterMac[7]={0};
	MRS_RF_ALL_WATER_METER_INFO *pWaterMeterInfo =mrsRfGetAllWaterMeterInfo();
	
	mrsRfPlcRnInfo.ucOwnerInfoNum=0;
	if(mrsRfPlcRnInfo.pOwnerInfo)
	{
		mrsToolsFree(mrsRfPlcRnInfo.pOwnerInfo);
	}
	mrsRfPlcRnInfo.pOwnerInfo = (HI_U8 *)mrsToolsMalloc(ucMeterCount*8);
	if(mrsRfPlcRnInfo.pOwnerInfo==HI_NULL)
	{
		HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_042, HI_DIAG_MT("[RF]SetMeterOwnerShip Malloc Fail"));
		return ;
	}
	mrsRfSetSnidTei();
	mrsRfPlcRnInfo.ucOwnerInfoNum=ucMeterCount;
	mrsToolsMemcpy_s(mrsRfPlcRnInfo.pOwnerInfo,ucMeterCount*8,ucaData,ucMeterCount*8);
	for(i=0;i<ucMeterCount;i++)
	{
		noffent=0;
		ucData[noffent++]=(g_CcoSnid%32+1);
		mrsToolsMemcpy_s(ucData+noffent,6,pstaCtx->ucMacAddr,6);
		mrsHexInvert(ucData+noffent,6);
		noffent+=6;
		ucData[noffent++]=(g_usCcoTei&0xFF);
		ucData[noffent++]=((g_usCcoTei>>8) &0xFF);

		ucData[noffent++]=1; // 时隙
		ucData[noffent++]=0;
		
		mrsToolsMemcpy_s(ucData+noffent,6,nTime,6);
		noffent+=6;
		if(ucData[noffent-1]>0x60)
		{
			ucData[noffent++]=0x19;
		}
		else
		{
			ucData[noffent++]=0x20;
		}
		
		ucData[noffent++]=1; // 工作模式
		
		ucData[noffent++]=0xFF; // 工作时段
		ucData[noffent++]=0xFF;
		ucData[noffent++]=0xFF;
		HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_076, HI_DIAG_MT("[RF]STA SetMeterOwnerShip "),i);
		mrsToolsMemcpy_s(ucMeterMac,7,mrsRfPlcRnInfo.pOwnerInfo+i*8,6);
		
		for(k=0;k<pWaterMeterInfo->ucWaterMeterCount;k++)
		{
			if(0==mrsToolsMemcmp(pWaterMeterInfo->saWaterMeterInfo[k].ucaWaterMeterAddr,ucMeterMac,6))
			{
				ucMeterMac[6]=pWaterMeterInfo->saWaterMeterInfo[k].ucaWaterMeterAddr[6];
				break;
			}
		}
		mrsRFDau0x4XEncodeFrame(ucData,noffent,ucMeterMac,MRS_UART_CMD_TIMEOUT*5,0x44);
	}
	mrsRfPlcRnInfo.usRfTx0x44TimeCount=(ucMeterCount+1)*16*MRS_UART_CMD_TIMEOUT;
}

HI_U32 mrsRFDau0x4XEncodeFrame(HI_U8 *pPayload, HI_U8 ucPayloadLen,HI_U8 ucAddr[7],HI_U16 nTime,HI_U8 ucCmdid)
{
	HI_U8 *pDauData=HI_NULL;
	HI_U8 *pData=HI_NULL;
	HI_U16 ucDataLen=0,ucDauDataLen=0;
	HI_U32 ret =HI_ERR_SUCCESS;
	HI_U8 ucChannelInfo=0;
	HI_U32 j=0;
	MRS_RF_ALL_WATER_METER_INFO *pStaWaterMeter =mrsRfGetAllWaterMeterInfo();
	for(j=0;j<pStaWaterMeter->ucWaterMeterCount;j++)
	{
		if(0==mrsToolsMemcmp(ucAddr,pStaWaterMeter->saWaterMeterInfo[j].ucaWaterMeterAddr,7))
		{
			ucChannelInfo = pStaWaterMeter->saWaterMeterInfo[j].ucChannelInfoNum*2;
			break;
		}
	}

	HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_089, HI_DIAG_MT("[RF]STA ucCmdid,ucChannelInfo"),ucCmdid,ucChannelInfo);

	ret=mrsRFDauEncodeFrame(&pDauData,&ucDauDataLen,pPayload,ucPayloadLen,ucAddr,ucCmdid,ucChannelInfo);
	if(ret !=HI_ERR_SUCCESS)
	{
		return ret;
	}
	pData = (HI_U8 *)mrsToolsMalloc(256);
	if(pData==HI_NULL)
	{
		return HI_ERR_MALLOC_FAILUE;
	}
	
	pData[0]=0x24;
	pData[1]=ucChannelInfo;
	pData[2]=ucCmdid;
	mrsToolsMemcpy_s(pData+3,ucDauDataLen,pDauData,ucDauDataLen);
	mrsToolsFree(pDauData);
	if(0x44 == ucCmdid)
	{
		mrsRfTransmitTimeAndCount(pData,ucDauDataLen+3,nTime,0x24,3);
	}
	else
	{
		mrsRfTransmit(pData,ucDauDataLen+3,nTime,0x24);
	}
	mrsToolsFree(pData);
	return ret;
}
*/

HI_U32 mrsRFDau0x42EncodeFrame(HI_U8 *pPayload, HI_U8 ucPayloadLen,HI_U16 usSend0x25Time)
{
	HI_U8 *pDauData=HI_NULL;
	HI_U8 *pData=HI_NULL;
	HI_U8 *pRfData=HI_NULL;
	HI_U16 ucDataLen=0;
	HI_U16 ucDauDataLen=0;
	HI_U32 ret =HI_ERR_SUCCESS;
	
	ret=mrsRFDauEncodeFrame(&pDauData,&ucDauDataLen,pPayload,ucPayloadLen,MRS_RF_BROADCAST_MAC,0x42,0);
	if(ret !=HI_ERR_SUCCESS)
	{
		return ret;
	}
	pRfData = (HI_U8 *)mrsToolsMalloc(256);
	if(pRfData==HI_NULL)
	{
		return HI_ERR_MALLOC_FAILUE;
	}
	pRfData[0]=0x24;
	pRfData[1]=0;//g_CcoSnid%32;
	pRfData[2]=0x42;
	mrsToolsMemcpy_s(pRfData+3,ucDauDataLen,pDauData,ucDauDataLen);
	mrsToolsFree(pDauData);
	
	ret =mrsRfCreateFrameTransmit(&pData, &ucDataLen,pRfData,ucDauDataLen+3);
	if(ret !=HI_ERR_SUCCESS)
	{
		mrsToolsFree(pRfData);
		return ret;
	}
	mrsRfDirectTransmit(pData,ucDataLen);
	mrsToolsFree(pRfData);
	mrsToolsFree(pData);
	mrsRfPlcRnInfo.usSend0x25TimeCount=usSend0x25Time;
  //HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1022, HI_DIAG_MT("[RF]STA收集RF水表场强时间"),&usSend0x25Time,sizeof(usSend0x25Time));
}


HI_U32 mrsRFDau0x25DecodeFrame(HI_U8 *pPayload, HI_U8 ucPayloadLen)
{
	HI_U8 ucAllPage=0;
	HI_U8 ucCurPage=0;
	HI_U8 ucSRRINum=0;
	HI_U8 ucOffent=0,i=0;
	MRS_RF_ALL_WATER_METER_INFO * pMeterInfo = mrsRfGetAllWaterMeterInfo();
	if(ucPayloadLen<2)
	{
		return HI_ERR_FAILURE;
	}
	ucAllPage= ((pPayload[ucOffent]>>4)&0x0F);
	ucCurPage= (pPayload[ucOffent]&0x0F);
	ucOffent++;
	ucSRRINum = pPayload[ucOffent++];
	if((ucSRRINum*8+2) !=ucPayloadLen)
	{
		//HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_048, HI_DIAG_MT("[RF]ucSRRINum,ucPayloadLen"),ucSRRINum,ucPayloadLen);
		return HI_ERR_FAILURE;
	}
	if(ucAllPage>(ucCurPage+1))
	{
		mrsRfReadRSSI25((ucCurPage+1));
	}
	if(ucCurPage==0)
	{
		pMeterInfo->ucWaterMeterCount=0;
		mrsToolsMemZero_s(pMeterInfo->saWaterMeterInfo, sizeof(MRS_RF_WATER_METER_SIGLE)*RF_MAX_WATER_METER_COUNT, sizeof(MRS_RF_WATER_METER_SIGLE)*RF_MAX_WATER_METER_COUNT);
	}
	for(i=0;i<ucSRRINum;i++)
	{
		mrsToolsMemcpy_s(pMeterInfo->saWaterMeterInfo[pMeterInfo->ucWaterMeterCount].ucaWaterMeterAddr,7,pPayload+ucOffent+i*8,7);
		pMeterInfo->saWaterMeterInfo[pMeterInfo->ucWaterMeterCount].ucFieldValue = pPayload[ucOffent+i*8+7];
		pMeterInfo->saWaterMeterInfo[pMeterInfo->ucWaterMeterCount].ucChannelInfoNum=0;
		pMeterInfo->ucWaterMeterCount++;
	}
	return HI_ERR_SUCCESS;
}
HI_U32 mrsRfStaCreateReadMeterPlc(HI_VOID *pFrame,HI_U8 *data,HI_U16 len,HI_U16 usId)
{
    HI_U8 ucVer = MRS_AFN13_UP_VER;
    HI_U16 usDataLen = 0;
    MRS_DATA_TRANSMIT_UP_STRU *pstData = HI_NULL;
    HI_U8 *pucPayload = HI_NULL;
	MRS_PLC_FRAME_DATA_STRU *frame_data = (MRS_PLC_FRAME_DATA_STRU *)pFrame;
	//HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_070, HI_DIAG_MT("[RF]mrsRfStaCreateReadMeterPlc"),usId);

#if defined(PRODUCT_CFG_SUPPORT_TEST_CONCENTRATOR)
    MRS_CMD_DATA_TRANSMIT_STRU *pstTrans = HI_NULL;

    usDataLen = sizeof(MRS_DATA_TRANSMIT_UP_STRU) + len + sizeof(MRS_CMD_DATA_TRANSMIT_STRU);
    pucPayload = (HI_U8 *)mrsToolsMalloc(usDataLen);
    pstTrans = (MRS_CMD_DATA_TRANSMIT_STRU *)pucPayload;
#else
    usDataLen = sizeof(MRS_DATA_TRANSMIT_UP_STRU) + len+1;
    pucPayload = (HI_U8 *)mrsToolsMalloc(usDataLen);
    pstData = (MRS_DATA_TRANSMIT_UP_STRU *)pucPayload;
#endif

    if (!pucPayload)
    {
        return HI_ERR_NO_MORE_MEMORY;
    }

    mrsToolsMemZero_s(frame_data, sizeof(MRS_PLC_FRAME_DATA_STRU), sizeof(MRS_PLC_FRAME_DATA_STRU));
    mrsToolsMemZero_s(pucPayload, usDataLen, usDataLen);

#if defined(PRODUCT_CFG_SUPPORT_TEST_CONCENTRATOR)
    pstData = (MRS_DATA_TRANSMIT_UP_STRU *)pstTrans->aucPayload;
#endif

    pstData->interface_ver = ucVer;
    pstData->stru_size     = sizeof(MRS_DATA_TRANSMIT_UP_STRU);
    pstData->protocol      = WIRELESS_TRANSMISSION_PROTOCOL;
    pstData->data_len      = len+1;
    pstData->seq           = mrsGetRfPlcRunInfo()->usPlcSn;
    pstData->dir           = MRS_PLC_UP_FLG;

	pstData->data[0]=mrsGetRfPlcRunInfo()->ucWaterMeterType;

    // 复制数据
    mrsToolsMemcpy_s(pstData->data+1, len, data, len);

#if defined(PRODUCT_CFG_SUPPORT_TEST_CONCENTRATOR)
    pstTrans->usCmdId = usId;
    pstTrans->usPayloadLen = sizeof(MRS_DATA_TRANSMIT_UP_STRU) + len;
    frame_data->usId = ID_MRS_CMD_DATA_TRANSMIT_PLC;
#else
    frame_data->usId         = usId;    // 获取业务ID
#endif
    frame_data->usPayloadLen = usDataLen;
    frame_data->pucPayload   = (HI_PBYTE)pucPayload;

    return HI_ERR_SUCCESS;
}
HI_U32 mrsStaCreateResponseSetShipPlc(HI_VOID *pFrame,MRS_RF_PLC_RUN_INFO_STRU *pPlcRunInfo,HI_U16 usId)
{
    HI_U8 ucVer = 0x01;
    HI_U16 usDataLen = 0;
    MRS_DATA_TRANSMIT_UP_STRU *pstData = HI_NULL;
    HI_U8 *pucPayload = HI_NULL;
	MRS_PLC_FRAME_DATA_STRU *frame_data = (MRS_PLC_FRAME_DATA_STRU *)pFrame;
	HI_U16 len=pPlcRunInfo->ucOwnerInfoNum*8;
	HI_U32 i=0,j=0;
	MRS_RF_ALL_WATER_METER_INFO *pStaWaterMeter=mrsRfGetAllWaterMeterInfo();
    usDataLen = sizeof(MRS_DATA_TRANSMIT_UP_STRU) + 1+len;
    pucPayload = (HI_U8 *)mrsToolsMalloc(usDataLen);
   
    if (!pucPayload)
    {
        return HI_ERR_NO_MORE_MEMORY;
    }
	if(pPlcRunInfo->pOwnerInfo==HI_NULL)
	{
		return HI_ERR_INVALID_PARAMETER;
	}
	pstData = (MRS_DATA_TRANSMIT_UP_STRU *)pucPayload;

    mrsToolsMemZero_s(frame_data, sizeof(MRS_PLC_FRAME_DATA_STRU), sizeof(MRS_PLC_FRAME_DATA_STRU));
    mrsToolsMemZero_s(pucPayload, usDataLen, usDataLen);
	for(i=0;i<pPlcRunInfo->ucOwnerInfoNum;i++)
	{
		if(pPlcRunInfo->pOwnerInfo[i*8+7]!=0)
		{
			//pstData->ucMeterSuccessCount++;//设置低功耗表计成功的数量
			for(j=0;j<pStaWaterMeter->ucWaterMeterCount;j++)
			{
				if(0==mrsToolsMemcmp(mrsRfPlcRnInfo.pOwnerInfo+i*8,pStaWaterMeter->saWaterMeterInfo[j].ucaWaterMeterAddr,7))
				{
					pStaWaterMeter->saWaterMeterInfo[j].ucChannelInfoNum = (pPlcRunInfo->pOwnerInfo[i*8+7]-1);
					break;
				}
			}
			pPlcRunInfo->pOwnerInfo[i*8+7]=1;
		}
	}
	pstData->interface_ver = ucVer;
    pstData->stru_size     = sizeof(MRS_DATA_TRANSMIT_UP_STRU);// 结构体长度
    pstData->protocol      = WIRELESS_TRANSMISSION_PROTOCOL;
    pstData->data_len      = len+1;
    pstData->seq           = mrsGetRfPlcRunInfo()->usPlcSn;// 序列号
    pstData->dir           = MRS_PLC_UP_FLG;
	pstData->data[0]		=WIRELESS_SET_OWNERSHIP; //增加扩展信息域，代表低功耗表类型含义
    // 复制数据
    if(len>0)
    {
    	mrsToolsMemcpy_s(pstData->data+1, len, pPlcRunInfo->pOwnerInfo, len);
    }
	frame_data->usId         = usId;    // 获取业务ID
    frame_data->usPayloadLen = usDataLen;
    frame_data->pucPayload   = (HI_PBYTE)pucPayload;

    return HI_ERR_SUCCESS;
}

HI_U32 mrsStaRfDataTransmit2CCO(HI_U8 * data, HI_U16 len,HI_U16 usID)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    MRS_PLC_FRAME_DATA_STRU frame_data = { 0 };

    ret = mrsRfStaCreateReadMeterPlc(&frame_data,data, len,usID);
    if (ret != HI_ERR_SUCCESS)
    {
        //HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_041, HI_DIAG_MT("create plc err -[%d]"),ret);
        return HI_ERR_FAILURE;
    }

    ret = MRS_SendPlcFrame(&frame_data);
    if (ret != HI_ERR_SUCCESS)
    {
        //HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_042, HI_DIAG_MT("tx plc fail"));
    }
    mrsToolsFree(frame_data.pucPayload);

    return ret;
}
HI_U32 mrsStaRfOwnerShipDataTransmit2CCO(HI_U8 * data, HI_U16 len,HI_U16 usID)
{
	 HI_U32 ret = HI_ERR_SUCCESS;
    MRS_PLC_FRAME_DATA_STRU frame_data = { 0 };
	
	ret = mrsStaCreateResponseSetShipPlc(&frame_data,&mrsRfPlcRnInfo,usID);
    if (ret != HI_ERR_SUCCESS)
    {
        //HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_041, HI_DIAG_MT("create plc err -[%d]"),ret);
        return HI_ERR_FAILURE;
    }

    ret = MRS_SendPlcFrame(&frame_data);
    if (ret != HI_ERR_SUCCESS)
    {
        //HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_042, HI_DIAG_MT("tx plc fail"));
    }
    mrsToolsFree(frame_data.pucPayload);

    return ret;
}

HI_U32 mrsRFDauEncodeFrame(HI_U8 **ppData,HI_U16 *ucpDataLen,HI_U8 *pPayload, HI_U8 ucPayloadLen,
	HI_U8 ucaDataDstAddr[7],HI_U8 ucCmd,HI_U8 ucChannelInfo)
{
	HI_U32 ret=HI_ERR_SUCCESS;
	MRS_STA_SRV_CTX_STRU* pstaCtx = mrsStaGetContext();
	MRS_RF_DAU_FRAME_STRU *pDauData = (MRS_RF_DAU_FRAME_STRU *)mrsToolsMalloc(sizeof(MRS_RF_DAU_FRAME_STRU));
	if(pDauData==HI_NULL)
	{
		return HI_ERR_MALLOC_FAILUE;
	}
	pDauData->ucFrameLen=23+ucPayloadLen;
	/*
	if((ucCmd==0x42) ||(ucCmd==0x44) )
	{
		pDauData->ucChannelIndex =0;
	}
	else
	{
		pDauData->ucChannelIndex =g_CcoSnid%32;
	}*/
	pDauData->ucChannelIndex =ucChannelInfo;
	pDauData->ucStandardIdNum= 0x01;
	pDauData->ucFrameHeader =mrsRFDAUFrameHeader(pDauData->ucFrameLen,pDauData->ucChannelIndex,pDauData->ucStandardIdNum);

	pDauData->ucsDataFrameCtrl =0x47cd;
	pDauData->ucDataFrameSn=mrsGetDauFrameSn();
	mrsAddDauFrameSn();
	pDauData->usDataPinId = 0xFFFF;

	mrsToolsMemcpy_s(pDauData->ucaDataDstAddr,7,ucaDataDstAddr,7);

	mrsToolsMemcpy_s(pDauData->ucaDataSrcAddr,7,pstaCtx->ucMacAddr,6);
	mrsHexInvert(pDauData->ucaDataSrcAddr,6);
	pDauData->ucaDataSrcAddr[6]=0;
	pDauData->ucDataCmdId=ucCmd;
	pDauData->ucDataLen = ucPayloadLen;
	mrsToolsMemcpy_s(pDauData->ucapDataData,ucPayloadLen,pPayload,ucPayloadLen);

	ret = mrsRFDauEncode(ppData,ucpDataLen,pDauData);
	mrsToolsFree(pDauData);
	return ret;
}

#endif


#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
#include "mrs_srv_cco_archives.h"
#include "mrs_srv_cco_archives_n.h"

MRS_RF_ALL_WATER_METER_INFO mrsRfCCOAllWaterMeterInfo; 
MRS_CCO_RF_UART_RUNNING mrsCcoRfUartRun;
extern HI_U32 mrsAFN13F1ReportAck(HI_U8 aucMacAdd[HI_PLC_MAC_ADDR_LEN]);
HI_U32 mrsRfCcoDelUartDataItem(HI_VOID);

//HI_U32 g_CcoNetSnid=0; 			//获取的SNID
HI_U32 g_CcoQueryReqTimesCnt=0;

HI_U32 mrsRf3762_F6_F101(HI_U8 *param, HI_U8 ucSize);
HI_U32 mrsRFDauF3762_F7_DecodeFrame(HI_U8 *pPayload, HI_U8 ucPayloadLen);
HI_U32 mrsCcoSendADU0x42Data(HI_VOID);

MRS_RF_ALL_WATER_METER_INFO *mrsRfGetCCOAllWaterMeterInfo(HI_VOID)
{
	return &mrsRfCCOAllWaterMeterInfo;
}

HI_U32 APPCCOMsgRcv(HI_U32 ulMdmMsgId, HI_PVOID pMdmMsg, HI_PVOID pData, HI_U32 ulOption);

HI_PRV HI_CONST HI_MDM_APPOBJ_S g_CCOAppObj =
{
    HI_MDM_OBJ_ID_USR2,      // APP对象ID
    APPCCOMsgRcv    // APP接收MDM消息的处理入口
};

HI_U32 mrsRF3762ProtoStreamInput(HI_U8 flag,HI_IN HI_PBYTE pucStream, HI_IN HI_U16 usStreamLen, HI_OUT MRS_ONE_RAW_FRAME_STRU** ppstFrame);
extern HI_U32 mrs3762ProtoBuffer(HI_IN MRS_3762_PROTO_CTX_STRU * pst3762Ctx, HI_OUT MRS_ONE_RAW_FRAME_STRU** ppstFrame);

HI_U32 mrsRF3762ProtoStreamInput(HI_U8 flag,HI_IN HI_PBYTE pucStream, HI_IN HI_U16 usStreamLen, HI_OUT MRS_ONE_RAW_FRAME_STRU** ppstFrame)
{
    MRS_3762_PROTO_CTX_STRU * pst3762Ctx =(MRS_3762_PROTO_CTX_STRU *) mrs645GetContext(ID_MRS_RF_645BUF);

    // 判断缓冲区是否已满
    if (flag || ((usStreamLen + pst3762Ctx->usRcvBufferSize) > PRODUCT_CFG_MRS_3762_BUFFER_BYTE_MAX))
    {
        pst3762Ctx->usDagramSize = 0;
    	pst3762Ctx->usRcvBufferSize = 0;
    }
    
    //将数据放入缓冲区
    mrsToolsMemcpy_s(pst3762Ctx->pucRcvBuffer + pst3762Ctx->usRcvBufferSize,
        PRODUCT_CFG_MRS_3762_BUFFER_BYTE_MAX - pst3762Ctx->usRcvBufferSize, pucStream, usStreamLen);
    pst3762Ctx->usRcvBufferSize = (HI_U16)(pst3762Ctx->usRcvBufferSize +usStreamLen);

    return mrs3762ProtoBuffer(pst3762Ctx, ppstFrame);
}
HI_U32 MRSRfProto3762Dec( HI_IN HI_PBYTE pucDatagram, HI_U16 usDatagramSize, 
                       HI_OUT MRS_3762_FRAME_STRU** ppstFrame )
{
    HI_U32 ret          = HI_ERR_SUCCESS;
    HI_PBYTE pInBuffer  = 0;
    HI_U16 usAppDataLen = 0;
    HI_U16 usAddrLen    = 0;
    HI_U8 ucDir;
    MRS_3762_FRAME_STRU st3762Frame;

    if (!pucDatagram) 
    {
        return HI_ERR_FAILURE;
    }
    
    //校验帧，此处可以保证参数正确，数据满足最小帧长
    ret = mrsToolsCheck3762Frame(pucDatagram,usDatagramSize);
    if (ret != HI_ERR_SUCCESS) 
    {
       //HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_004, HI_DIAG_MT("376.2 Chk Err "));
        return HI_ERR_BAD_DATA;
    }

    mrsToolsMemZero_s(&st3762Frame, sizeof(st3762Frame), sizeof(st3762Frame));

    //开始解析
    pInBuffer  = pucDatagram;
    // 控制域
    pInBuffer += MRS_3762_FRAME_HEAD_SIZE + MRS_3762_FRAME_LEN_SIZE;
    ucDir                         = (HI_U8) (((*pInBuffer) >> 7) & 1);
    st3762Frame.stCtrl.ucDir      = ucDir;
    st3762Frame.stCtrl.ucPrm      = (HI_U8) (((*pInBuffer) >> 6) & 1);
    st3762Frame.stCtrl.ucCommForm = (HI_U8) ((*pInBuffer) & 0x3F);
    pInBuffer++;

    MRS_3762_INFO_REALM_DL_STRU* pDl = &(st3762Frame.strUsrDataHrd.uInfoRealm.stInfRealmDL);
    
    mrs3762RealmDlDec(pInBuffer, pDl);
    pInBuffer += MRS_3762_FRAME_INFO_SIZE; /* sizeof(MRS_3762_INFO_REALM_DL_STRU) */   

    // 地址域A
    if ((st3762Frame.strUsrDataHrd.uInfoRealm.stInfRealmDL.ucModuleFlag == 1))
    {
        HI_U8 ucRelayLevel = 0;
        
        st3762Frame.strUsrDataHrd.stAddrRealm.ucRelayLevel = pDl->ucRelayLevel;
        
        ucRelayLevel = st3762Frame.strUsrDataHrd.stAddrRealm.ucRelayLevel;

        usAddrLen = (HI_U16) ((ucRelayLevel + 2) * 6);
        
        if ((HI_S16)(usDatagramSize - MRS_3762_FRAME_MIN_SIZE) < usAddrLen)
        {
            // 地址域长度不足
            //HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_005, HI_DIAG_MT("376.2 addr len too short"));
            return HI_ERR_BAD_DATA;
        }
             
        // 源地址
        mrsToolsMemcpy_s(st3762Frame.strUsrDataHrd.stAddrRealm.ucSrcAdd,
            sizeof(st3762Frame.strUsrDataHrd.stAddrRealm.ucSrcAdd),
            pInBuffer, sizeof(st3762Frame.strUsrDataHrd.stAddrRealm.ucSrcAdd));
        pInBuffer += sizeof(st3762Frame.strUsrDataHrd.stAddrRealm.ucSrcAdd);
        
        // 中继地址
        if (ucRelayLevel <= 15)
        {
            mrsToolsMemcpy_s(st3762Frame.strUsrDataHrd.stAddrRealm.ucRelayAdd[0], 
                sizeof(st3762Frame.strUsrDataHrd.stAddrRealm.ucRelayAdd), pInBuffer, HI_PLC_MAC_ADDR_LEN * ucRelayLevel);
        }
        
        pInBuffer += HI_PLC_MAC_ADDR_LEN * ucRelayLevel;
        
        // 目的地址
        mrsToolsMemcpy_s(st3762Frame.strUsrDataHrd.stAddrRealm.ucDestAdd,
            sizeof(st3762Frame.strUsrDataHrd.stAddrRealm.ucDestAdd),
            pInBuffer, sizeof(st3762Frame.strUsrDataHrd.stAddrRealm.ucDestAdd));
        pInBuffer += sizeof(st3762Frame.strUsrDataHrd.stAddrRealm.ucDestAdd);
    }

    //总长度 = 固定长度+信息域+地址域+应用数据域
    usAppDataLen = (usDatagramSize - MRS_3762_FRAME_MIN_SIZE - usAddrLen + MRS_3762_FRAME_FN_SIZE);

    if((HI_S16)usAppDataLen < MRS_3762_FRAME_FN_SIZE)
    {
        return HI_ERR_BAD_DATA;
    }

    st3762Frame.strUsrDataHrd.stAppDataRealm.ucAfn = *pInBuffer++;    
    st3762Frame.strUsrDataHrd.stAppDataRealm.usAppDataLen = usAppDataLen;

    *ppstFrame = mrsToolsMalloc(sizeof(MRS_3762_FRAME_STRU) + usAppDataLen);
    if (*ppstFrame == HI_NULL) 
    {
        //分配内存失败
        return HI_ERR_MALLOC_FAILUE;
    }
    mrsToolsMemcpy_s(*ppstFrame, sizeof(MRS_3762_FRAME_STRU) + usAppDataLen, &st3762Frame, sizeof(MRS_3762_FRAME_STRU));

    mrsToolsMemcpy_s( (*ppstFrame)->strUsrDataHrd.stAppDataRealm.pucAppDataContent, usAppDataLen,
                pInBuffer, usAppDataLen);

    return ret;
} 

//*****************************************************************************
// 函数名称: mrsCcoRFRecvDealFun
// 功能描述: 获取从无线接收过来的完整帧
//           
// 参数说明:
//    pstRxData [IN]接收到的无线的数据
//                            
// 返 回 值: 
//    成功返回  HI_ERR_SUCCESS
//    失败返回  HI_ERR_FAILURE
//    继续接收数据 HI_ERR_CONTINUE
//*****************************************************************************
HI_U32 mrsCcoRFRecvDealFun(HI_DMS_CHL_RX_S* pstRxData)

{
    HI_U32 ret = HI_ERR_SUCCESS;
    MRS_ONE_RAW_FRAME_STRU* pstFrame = HI_NULL;
	MRS_RF_DAU_FRAME_STRU *pDauFrame=HI_NULL;
	MRS_RF_ALL_WATER_METER_INFO *pWaterMeterInfo =mrsRfGetCCOAllWaterMeterInfo();
	MRS_SRV_QUEUE *pRfQueue=&(mrsCcoRfUartRun.RfCcoQueue);
	MRS_RF_CCO_UART_NODE *pstNode = (MRS_RF_CCO_UART_NODE *)mrsSrvQueueTop(pRfQueue);
	//HI_U32 ulTemp;
	HI_U32 i=0;

    // 获取到完整的帧数据
    ret = mrsRF3762ProtoStreamInput(pstRxData->bCanDiscardOldData,pstRxData->pPayload, pstRxData->usPayloadLen, &pstFrame);
    if(HI_ERR_SUCCESS == ret)
    {
        //HI_SYS_QUEUE_MSG_S stMsg = { 0 };

        if (pstFrame)
        {
            HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1000, HI_DIAG_MT("[CCO-MR]-Diag_RX"), pstFrame->pucDatagram, pstFrame->usDatagramSize);
            mrsDfxChlUartOkFrameCnt(pstFrame->usDatagramSize, HI_FALSE);    // 串口接收完整帧统计(消息发送)
        }

        
        HI_PBYTE pucDatagram  = pstFrame->pucDatagram;
        HI_U16 usDatagramSize = pstFrame->usDatagramSize;

        MRS_3762_FRAME_STRU* pst3762Frame = HI_NULL;

        ret = MRSRfProto3762Dec(pucDatagram, usDatagramSize, &pst3762Frame);
        if ((ret == HI_ERR_SUCCESS) && (pst3762Frame != HI_NULL))
        {
        	if(pst3762Frame->strUsrDataHrd.stAppDataRealm.ucAfn == 0x06)
        	{
        		
				HI_PBYTE pOutBuffer   = HI_NULL;
				HI_U16 usBufferOutLen = 0;
				PROTO376_2_FRAME_PRE_STRU pstPreFrame;
				HI_U8 ucData[6]={0xFF,0xFF,0xFF,0xFF,0,0};
				mrsToolsMemZero_s(&pstPreFrame, sizeof(PROTO376_2_FRAME_PRE_STRU), sizeof(PROTO376_2_FRAME_PRE_STRU));
				
				if((pst3762Frame->strUsrDataHrd.stAppDataRealm.pucAppDataContent[0]==MRS_AFN_FN_LO(100))
				&& (pst3762Frame->strUsrDataHrd.stAppDataRealm.pucAppDataContent[1]==MRS_AFN_FN_HI(100))
				)
    			{
	    			ret =mrsCcoRfSetPowerFailureInfo((MRS_EVENT_INFO_STRU *)(pst3762Frame->strUsrDataHrd.stAppDataRealm.pucAppDataContent+6));
				}
				else if((pst3762Frame->strUsrDataHrd.stAppDataRealm.pucAppDataContent[0]==MRS_AFN_FN_LO(101))
				&& (pst3762Frame->strUsrDataHrd.stAppDataRealm.pucAppDataContent[1]==MRS_AFN_FN_HI(101))
				)
    			{
    				ret =mrsRf3762_F6_F101((pst3762Frame->strUsrDataHrd.stAppDataRealm.pucAppDataContent+2),pst3762Frame->strUsrDataHrd.stAppDataRealm.usAppDataLen-2);
				}
				
				pstPreFrame.ucSeq = pst3762Frame->strUsrDataHrd.uInfoRealm.stInfRealmDL.ucSeq;
				pstPreFrame.stAfnData.ucAFN = 0;
				if(ret==HI_ERR_SUCCESS)
				{
				    pstPreFrame.stAfnData.ucDT[0] = 1;
				    pstPreFrame.stAfnData.ucDT[1] = 0;
				    pstPreFrame.stAfnData.pData = ucData;
				    pstPreFrame.stAfnData.usDataLen = 6;
				}
				else
				{
					ucData[0]=ret;
					pstPreFrame.stAfnData.ucDT[0] = 2;
				    pstPreFrame.stAfnData.ucDT[1] = 0;
				    pstPreFrame.stAfnData.pData = ucData;
				    pstPreFrame.stAfnData.usDataLen = 1;
				}
				ret=mrsCreate3762FrameDown(&pstPreFrame,&pOutBuffer,&usBufferOutLen,HI_NULL);
				if((ret == HI_ERR_SUCCESS) && (pOutBuffer!=HI_NULL))
				{
					ret = MRS_SendMrData(pOutBuffer, usBufferOutLen, HI_DMS_CHL_UART_PORT_DIAG);
					//HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1001, HI_DIAG_MT("[CCO-MR]-SendDiag"), pOutBuffer, usBufferOutLen);
					mrsToolsFree(pOutBuffer);
				}
			}
			else if(pst3762Frame->strUsrDataHrd.stAppDataRealm.ucAfn == 0x04)
        	{
				if((pst3762Frame->strUsrDataHrd.stAppDataRealm.pucAppDataContent[0]==MRS_AFN_FN_LO(7))
				&& (pst3762Frame->strUsrDataHrd.stAppDataRealm.pucAppDataContent[1]==MRS_AFN_FN_HI(7))
				)
    			{
    				mrsRFDauF3762_F7_DecodeFrame((pst3762Frame->strUsrDataHrd.stAppDataRealm.pucAppDataContent+2),pst3762Frame->strUsrDataHrd.stAppDataRealm.usAppDataLen-2);
				}
				else if((pst3762Frame->strUsrDataHrd.stAppDataRealm.pucAppDataContent[0]==MRS_AFN_FN_LO(6))
				&& (pst3762Frame->strUsrDataHrd.stAppDataRealm.pucAppDataContent[1]==MRS_AFN_FN_HI(6))
				)
    			{
    				HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_046, HI_DIAG_MT("[RF]AFN 04 F6"));
					do{
					if(pst3762Frame->strUsrDataHrd.stAppDataRealm.usAppDataLen<4)
					{
						break;
					}
    				pDauFrame = mrsRFDauDecode((pst3762Frame->strUsrDataHrd.stAppDataRealm.pucAppDataContent+4),pst3762Frame->strUsrDataHrd.stAppDataRealm.usAppDataLen-4);
					if(pDauFrame==HI_NULL)
					{
						break;
					}
					for(i=0;i<pWaterMeterInfo->ucWaterMeterCount;i++)
					{
						if(0==mrsToolsMemcmp(pWaterMeterInfo->saWaterMeterInfo[i].ucaWaterMeterAddr,pDauFrame->ucaDataSrcAddr,7))
						{
							pWaterMeterInfo->saWaterMeterInfo[i].ucFieldValue =pst3762Frame->strUsrDataHrd.stAppDataRealm.pucAppDataContent[3];
							break;
						}
					}
					if(i >= pWaterMeterInfo->ucWaterMeterCount)
					{
						mrsToolsMemcpy_s(pWaterMeterInfo->saWaterMeterInfo[pWaterMeterInfo->ucWaterMeterCount].ucaWaterMeterAddr, 7, pDauFrame->ucaDataSrcAddr, 7);
						pWaterMeterInfo->saWaterMeterInfo[pWaterMeterInfo->ucWaterMeterCount].ucFieldValue =pst3762Frame->strUsrDataHrd.stAppDataRealm.pucAppDataContent[3];
						pWaterMeterInfo->ucWaterMeterCount++;
					}
					if(pDauFrame->ucDataCmdId == 0x45)
					{
						for(i=0;i<pWaterMeterInfo->ucWaterMeterCount;i++)
						{
							if(0==mrsToolsMemcmp(pWaterMeterInfo->saWaterMeterInfo[i].ucaWaterMeterAddr,pDauFrame->ucaDataSrcAddr,7))
							{
								pWaterMeterInfo->saWaterMeterInfo[i].ucChannelInfoNum = pDauFrame->ucapDataData[0];
								break;
							}
						}
					}
					//收到数据
					else if(pDauFrame->ucDataCmdId == 0x41)
					{
						HI_U8 * out_buff = HI_NULL; 
					    HI_U16 out_len = 0;
						if((pstNode!=HI_NULL) && (pstNode->ucRfNodeCi==0x40))
						{
							pstNode->ucRfNodeCi = 0;
							ret = mrsCreateReadMeterFrame376_2(0, pDauFrame->ucapDataData+1, pDauFrame->ucapDataData[0], &out_buff, &out_len, HI_NULL);
							//HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_045, HI_DIAG_MT("[RF]Recv 0x41,dataLen"),pDauFrame->ucapDataData[0]);
							if((ret == HI_ERR_SUCCESS) && (out_buff != HI_NULL))
						    {
						    	
						        ret = MRS_SendMrData(out_buff, out_len, HI_DMS_CHL_UART_PORT_APP); // 发送读表的返回报文到集中器
						    }

	    					mrsToolsFree(out_buff);   // 释放读表上行帧  
						}
						else
						{
							//HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_096, HI_DIAG_MT("[RF]Recv 0x41,没有抄表,dataLen"),pDauFrame->ucapDataData[0]);
						}
						
						MR_WATER_METER_SEARCH_STRU *pWaterMeter=&(mrsCcoGetContext()->stWaterMeterSearchModel);
						pWaterMeter->stWaterMeterResult[pWaterMeter->ucReadWaterMeterDataIndex].ucWaterMaxTryCount=0;
					}
					else
					{
						//HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_044, HI_DIAG_MT("[RF]pDauFrame->ucDataCmdId"),pDauFrame->ucDataCmdId);
					}
					}while(0);
					mrsToolsFree(pDauFrame);
				}
				else
				{
					// 业务模块收到数据转发到无线模块
					ret = MRS_SendMrData(pucDatagram, usDatagramSize, HI_DMS_CHL_UART_PORT_APP);
				}
			}
			else if(mrsCcoRfUartRun.ucRfCcoLock &&(pst3762Frame->strUsrDataHrd.stAppDataRealm.ucAfn == 0x0)) 
			{
				if((pst3762Frame->strUsrDataHrd.stAppDataRealm.pucAppDataContent[0]==0x01)
				&& (pst3762Frame->strUsrDataHrd.stAppDataRealm.pucAppDataContent[1]==0x0))
				{
					// 确认帧
				}
				else if((pst3762Frame->strUsrDataHrd.stAppDataRealm.pucAppDataContent[0]==0x02)
				&& (pst3762Frame->strUsrDataHrd.stAppDataRealm.pucAppDataContent[1]==0x0))
				{
					// 否认帧
				}
			}
			else if((pst3762Frame->strUsrDataHrd.stAppDataRealm.ucAfn == 0x03) 
				&& (pst3762Frame->strUsrDataHrd.stAppDataRealm.pucAppDataContent[0]==MRS_AFN_FN_LO(10))
				&& (pst3762Frame->strUsrDataHrd.stAppDataRealm.pucAppDataContent[1]==MRS_AFN_FN_HI(10))
				)
			{
				HI_PBYTE pOutBuffer   = HI_NULL;
            	HI_U16 usBufferOutLen = 0;
				PROTO376_2_FRAME_PRE_STRU pstPreFrame;
				HI_U8 ucData[6]={0xFF,0xFF,0xFF,0xFF,0,0};
				mrsToolsMemZero_s(&pstPreFrame, sizeof(PROTO376_2_FRAME_PRE_STRU), sizeof(PROTO376_2_FRAME_PRE_STRU));

				// pst3762Frame 解释的数据存放处，可以判断
				
				pstPreFrame.ucSeq = pst3762Frame->strUsrDataHrd.uInfoRealm.stInfRealmDL.ucSeq;
			    pstPreFrame.stAfnData.ucAFN = 0;
			    pstPreFrame.stAfnData.ucDT[0] = 1;
			    pstPreFrame.stAfnData.ucDT[1] = 0;
			    pstPreFrame.stAfnData.pData = ucData;
			    pstPreFrame.stAfnData.usDataLen = 6;
				
				ret=mrsCreate3762FrameDown(&pstPreFrame,&pOutBuffer,&usBufferOutLen,HI_NULL);
				if((ret == HI_ERR_SUCCESS) && (pOutBuffer!=HI_NULL))
				{
					ret = MRS_SendMrData(pOutBuffer, usBufferOutLen, HI_DMS_CHL_UART_PORT_DIAG);
					//HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1001, HI_DIAG_MT("[CCO-MR]-SendDiag"), pOutBuffer, usBufferOutLen);
					mrsToolsFree(pOutBuffer);
				}
			}
			else
			{
				// 业务模块收到数据转发到无线模块
				ret = MRS_SendMrData(pucDatagram, usDatagramSize, HI_DMS_CHL_UART_PORT_APP);
			}
			// 收到解析正确的数据,删除数据
			if(mrsCcoRfUartRun.ucRfCcoLock)
			{
				mrsRfCcoDelUartDataItem();
				mrsCcoRfUartRun.ucRfCcoLock=HI_FALSE;
			}
        }
		mrsToolsFree(pst3762Frame);
        mrsToolsFree(pstFrame);
		MRS_3762_PROTO_CTX_STRU * pst3762Ctx =(MRS_3762_PROTO_CTX_STRU *) mrs645GetContext(ID_MRS_RF_645BUF);
        pst3762Ctx->usDagramSize = 0;
    	pst3762Ctx->usRcvBufferSize = 0;
    }
    else if(HI_ERR_CONTINUE == ret)
    {
        // 所有数据汇聚后, 发现总和不足1帧数据, 需要继续等待数据
    }
    else
    {
        //mrsDfxChlUartErrRx(pstFrame->usPayloadLen, ret, HI_FALSE); // 串口接收错误处理(消息发送)
    }

    return ret;
}

HI_U32 mrs3762_RF_ReportAFN04_F5(HI_U32 ulSnid)
{
	HI_PBYTE pOutBuf=HI_NULL;
	HI_U16 usBufLen=0;
	HI_U32 ret =HI_ERR_SUCCESS;
    PROTO376_2_FRAME_AFN_AND_DATA_STRU stAfnData;
	MRS_CCO_SRV_CTX_STRU *pCtx = mrsCcoGetContext();
	HI_U8 ucaData[10]={};

	HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_025, HI_DIAG_MT("[RF]ulSnid"),ulSnid);
	if(ulSnid==0)
	{
	    ucaData[0]=pCtx->ulCcoNetSnid&0xFF;
		ucaData[1]=(pCtx->ulCcoNetSnid>>8)&0xFF;
		ucaData[2]=(pCtx->ulCcoNetSnid>>16)&0xFF;
	}
	else
	{
		ucaData[0]=ulSnid&0xFF;
		ucaData[1]=(ulSnid>>8)&0xFF;
		ucaData[2]=(ulSnid>>16)&0xFF;
	}
	mrsToolsMemcpy_s(ucaData+3,HI_METER_ADDR_LEN,pCtx->ucMainNodeAddr,HI_METER_ADDR_LEN);
	mrsHexInvert(ucaData+3,HI_METER_ADDR_LEN);
    mrsToolsMemZero_s(&stAfnData, sizeof(stAfnData), sizeof(stAfnData));
    stAfnData.ucAFN = MRS_AFN(0x04); // AFN=04H
    stAfnData.ucDT[0] = MRS_AFN_FN_LO(5);
    stAfnData.ucDT[1] = MRS_AFN_FN_HI(5);
    stAfnData.pData = ucaData;
    stAfnData.usDataLen = 9;

    ret = mrsCreate3762DownFrame(&stAfnData, &pOutBuf, &usBufLen, HI_NULL, HI_NULL);
	if(ret == HI_ERR_SUCCESS)
	{
		//ret = MRS_SendMrData(pOutBuf, usBufLen, HI_DMS_CHL_UART_PORT_DIAG);
		mrsCcoRfInsertUartDataItem(20,pOutBuf, usBufLen,5,1);
		//HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1002, HI_DIAG_MT("[CCO-MR]-SendDiag"), pOutBuf, usBufLen);
		mrsToolsFree(pOutBuf);
	}
	return ret;
}
HI_U32 mrs3762_RF_ReportAFN04_F6(HI_U8 *pData,HI_U16 usDatalen,HI_U16 usRfTimeOut,HI_U8 ucMaxTry,HI_U8 ucID)
{
	HI_PBYTE pOutBuf=HI_NULL;
	HI_U16 usBufLen=0;
	HI_U32 ret =HI_ERR_SUCCESS;
    PROTO376_2_FRAME_AFN_AND_DATA_STRU stAfnData;
	//MRS_CCO_SRV_CTX_STRU *pCtx = mrsCcoGetContext();

	HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_080, HI_DIAG_MT("[RF]ReportAFN04_F6"));
	
    mrsToolsMemZero_s(&stAfnData, sizeof(stAfnData), sizeof(stAfnData));
    stAfnData.ucAFN = MRS_AFN(0x04); // AFN=04H
    stAfnData.ucDT[0] = MRS_AFN_FN_LO(6);
    stAfnData.ucDT[1] = MRS_AFN_FN_HI(6);
    stAfnData.pData = pData;
    stAfnData.usDataLen = usDatalen;

    ret = mrsCreate3762DownFrame(&stAfnData, &pOutBuf, &usBufLen, HI_NULL, HI_NULL);
	if(ret == HI_ERR_SUCCESS)
	{
		//ret = MRS_SendMrData(pOutBuf, usBufLen, HI_DMS_CHL_UART_PORT_DIAG);
		
		mrsCcoRfInsertUartDataItem(usRfTimeOut,pOutBuf, usBufLen,ucID,ucMaxTry);
		//HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1003, HI_DIAG_MT("[CCO-MR]-SendDiag"), pOutBuf, usBufLen);
		mrsToolsFree(pOutBuf);
	}
	return ret;
}

HI_U32 mrs3762_RF_ReportAFN04_F7(HI_U8 ucPageNum)
{
	HI_PBYTE pOutBuf=HI_NULL;
	HI_U16 usBufLen=0;
	HI_U32 ret =HI_ERR_SUCCESS;
    PROTO376_2_FRAME_AFN_AND_DATA_STRU stAfnData;
	//MRS_CCO_SRV_CTX_STRU *pCtx = mrsCcoGetContext();
	HI_U8 ucaData;
	//HI_U16 ucOffent=0;
	MRS_RF_ALL_WATER_METER_INFO * pWaterMeterInfo = mrsRfGetCCOAllWaterMeterInfo();
	if(ucPageNum==0)
	{
		pWaterMeterInfo->ucWaterMeterCount=0;
	}

	HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_081, HI_DIAG_MT("[RF]ReportAFN04_F7"));
	ucaData=ucPageNum;
    mrsToolsMemZero_s(&stAfnData, sizeof(stAfnData), sizeof(stAfnData));
    stAfnData.ucAFN = MRS_AFN(0x04); // AFN=04H
    stAfnData.ucDT[0] = MRS_AFN_FN_LO(7);
    stAfnData.ucDT[1] = MRS_AFN_FN_HI(7);
    stAfnData.pData = (&ucaData);
    stAfnData.usDataLen = 1;

    ret = mrsCreate3762DownFrame(&stAfnData, &pOutBuf, &usBufLen, HI_NULL, HI_NULL);
	if(ret == HI_ERR_SUCCESS)
	{
		//ret = MRS_SendMrData(pOutBuf, usBufLen, HI_DMS_CHL_UART_PORT_DIAG);
		ret =mrsCcoRfInsertUartDataItem(10,pOutBuf, usBufLen,7,1);
		//HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1003, HI_DIAG_MT("[CCO-MR]-SendDiag"), pOutBuf, usBufLen);
		mrsToolsFree(pOutBuf);
	}
	return ret;
}

HI_U32 mrsCcoSGDCreateSetTimeSlot(HI_U8 ** ppTimeSlot,HI_U16 usSlot,HI_U16 usMaxTei)
{
	static HI_U32 ulSlotSeq=0xFFFF;
    MRS_PLC_FRAME_DATA_STRU *plc = HI_NULL;
	MRS_PLC_SET_TIME_SLOT_S *pSlot=HI_NULL;
    HI_U8 *payload = HI_NULL;
    HI_U16 payload_len = sizeof(MRS_PLC_SET_TIME_SLOT_S);

    payload = mrsToolsMalloc(payload_len);
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

    mrsToolsMemZero_s(payload, payload_len, payload_len);
    pSlot = (MRS_PLC_SET_TIME_SLOT_S *)payload;
    pSlot->interface_ver = MRS_START_SEARCH_DL_VER;
    pSlot->stru_len = sizeof(MRS_PLC_SET_TIME_SLOT_S);
    pSlot->seq = ulSlotSeq++;
	pSlot->usTimeSlot =usSlot%3;
	pSlot->usMaxTei = usMaxTei;
    HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_046, HI_DIAG_MT("[RF]mrsCcoCreateSetTimeSlot seq"), pSlot->seq);

    mrsToolsMemZero_s(plc, sizeof(MRS_PLC_FRAME_DATA_STRU), sizeof(MRS_PLC_FRAME_DATA_STRU));
    plc->usId = ID_MRS_CMD_RF_SET_TIME_SLOT;
    plc->pucPayload = payload;
    plc->usPayloadLen = payload_len;
    mrsToolsMemset_s(plc->ucMac, HI_PLC_MAC_ADDR_LEN, 0xFF, HI_PLC_MAC_ADDR_LEN);

    *ppTimeSlot = (HI_U8 *)plc;

    return HI_ERR_SUCCESS;
}

HI_U32 mrsCcoSGDTimeSlotFun(HI_VOID)
{
	HI_U32 ret=HI_ERR_SUCCESS;
	HI_U16 usMaxTei=0;
	HI_U32 i=0;
	static HI_U32 s_ulTimeSlot=0,static_ulSendRetry=0;
	HI_U8 *pData=HI_NULL;
	MRS_CCO_SRV_CTX_STRU *pCtx = mrsCcoGetContext();
	for(i=0;i<pCtx->stTopoInf.num;i++)
	{
		if(pCtx->stTopoInf.entry[i].tei >usMaxTei)
		{
			usMaxTei = pCtx->stTopoInf.entry[i].tei;
		}
	}
	
	ret = mrsCcoSGDCreateSetTimeSlot(&pData,s_ulTimeSlot,usMaxTei);
	if((ret!=HI_ERR_SUCCESS)|| (pData==HI_NULL))
	{
		//HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_047, HI_DIAG_MT("[RF]mrsCcoTimeSlotFun ret"), ret);
		return HI_ERR_SUCCESS;
	}
	s_ulTimeSlot++;

	MRS_PLC_FRAME_DATA_S* pPlc=(MRS_PLC_FRAME_DATA_S*)pData;
   (HI_VOID)MRS_SendPlcFrame(pPlc);
  	static_ulSendRetry++;
    
    if (static_ulSendRetry >2)
    {
    	static_ulSendRetry=0;
		if(pCtx->ucRfTimeSlot>0)
		{
        	MRS_StartTimer(MRS_CCO_TIMER_SGD_RF_TIME, pCtx->ucRfSGDTimeSlot*60 * 1000, HI_SYS_TIMER_ONESHOT);
		}
	}
	else
	{
        MRS_StartTimer(MRS_CCO_TIMER_SGD_RF_TIME, 300, HI_SYS_TIMER_ONESHOT);
	}
    mrsToolsFree(pPlc->pucPayload);
    mrsToolsFree(pPlc);
	return ret;
}

HI_U32 mrsCcoSendTimeSlotPlcFrame(HI_U8 * pPlcData,HI_U32 waterBoardTimer)
{

    HI_U32 ret = HI_ERR_SUCCESS;
	MRS_PLC_FRAME_DATA_S* pPlc=HI_NULL;
	static HI_U32 static_ulSendRetry=0;
	MRS_CCO_SRV_CTX_STRU *pCtx = mrsCcoGetContext();

    do
    {
        if (HI_NULL == pPlcData)
        {
            break;
        }
		pPlc=(MRS_PLC_FRAME_DATA_S*)pPlcData;
       
        (HI_VOID)MRS_SendPlcFrame(pPlc);
		mrsCcoSendADU0x42Data();

        static_ulSendRetry++;
        
        if (static_ulSendRetry >0)
        {
        	static_ulSendRetry=0;
					//CJJ-HPLC-20181128:做非0判断
					if(pCtx->ucRfTimeSlot>0)
					{
          	MRS_StartTimer(MRS_CCO_TIMER_RF_TIME, pCtx->ucRfTimeSlot*60 * 1000, HI_SYS_TIMER_ONESHOT);
					}
					MRS_StartTimer(MRS_CCO_TIMER_GET_WATER_METERLIST, waterBoardTimer, HI_SYS_TIMER_ONESHOT);
		      //HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1021, HI_DIAG_MT("[RF]等待收集水表场强时间"), (HI_U8 *)&waterBoardTimer, sizeof(waterBoardTimer));
					pCtx->stWaterMeterSearchModel.bStartReadMeterFlag=1;
					pCtx->stWaterMeterSearchModel.usCurrNum=0;
		}
		else
		{
            MRS_StartTimer(MRS_CCO_TIMER_RF_TIME, 300, HI_SYS_TIMER_ONESHOT);
		}

        mrsToolsFree(pPlc->pucPayload);
    } while (0);

    return ret;
}

HI_U32 APPCCOMsgRcv(HI_U32 ulMdmMsgId, HI_PVOID pMdmMsg, HI_PVOID pData, HI_U32 ulOption)
{
	HI_MDM_MSG_APPRCV_S* pstMsgRcv = (HI_MDM_MSG_APPRCV_S*)pMdmMsg;
	
	if(ulMdmMsgId == HI_MDM_NM_FREQ_QRY_ID) 
    { 
        //HI_MDM_NM_FREQ_QRY_IND_S* pstInd = (HI_MDM_NM_FREQ_QRY_IND_S*)pstMsgRcv->pucPacket; 
		
    } 
	else if(ulMdmMsgId == HI_MDM_NM_SNID_QRY_ID)
	{
		
		HI_MDM_NM_SNID_QRY_IND_S *pstInd = (HI_MDM_NM_SNID_QRY_IND_S *)pstMsgRcv->pucPacket;
		mrsCcoGetContext()->ulCcoNetSnid = pstInd->ulSnid;
	}

	return HI_ERR_SUCCESS;
}
HI_U32 AppCCOMDMQuerySn(HI_U32 usMsgId)
{
	HI_U32 ret = HI_ERR_SUCCESS;
	HI_MDM_MSG_CTRL_S stCtrl; 
	//HI_MDM_Memzero(&stCtrl, sizeof(HI_MDM_MSG_CTRL_S));
	(hi_void)memset_s(&stCtrl, sizeof(HI_MDM_MSG_CTRL_S),0,sizeof(HI_MDM_MSG_CTRL_S));
	g_CcoQueryReqTimesCnt++;
	stCtrl.ulSn = g_CcoQueryReqTimesCnt;
	stCtrl.usDULink = HI_FALSE; 
	stCtrl.usTransId = HI_MDM_OBJ_ID_USR2;
	
	ret = HI_MDM_MsgSend(usMsgId, HI_NULL, 0, &stCtrl);
	return ret;
}
/*
HI_BOOL MrsRegisterStaAuthCallbackFuntion(HI_U8 *pucMacAddr, HI_U8 ucProductType, HI_U8 ucMacType, HI_VOID *pParam)
{
	if(HI_MDM_MAC_NETWORK_EVALUATE_END == mrsGetTopoStatus())
	{
		
	}
	return HI_TRUE;
}
*/

HI_U32 mrsRfCCOInit(HI_VOID)
{
	HI_MDM_AppInit(&g_CCOAppObj);
	
	mrsToolsMemZero_s(&mrsRfCCOAllWaterMeterInfo,sizeof(mrsRfCCOAllWaterMeterInfo),sizeof(mrsRfCCOAllWaterMeterInfo));
	mrsSrvInitList(&(mrsRfCCOAllWaterMeterInfo.pStaReportDataList));
	
	MRS_StartTimer(MRS_CCO_TIMER_RF_UART_TIME,100, HI_SYS_TIMER_PERIODIC);
	mrsToolsMemZero_s(&mrsCcoRfUartRun,sizeof(mrsCcoRfUartRun),sizeof(mrsCcoRfUartRun));
	
	mrsSrvInitQueue(&(mrsCcoRfUartRun.RfCcoQueue));

	//HI_MDM_RegisterStaAuthCallback(MrsRegisterStaAuthCallbackFuntion,HI_NULL);
	
	HI_MDM_IO_SetMux(HI_HW_SPI_DO_IO20, HI_GPIO20_SEL_GPIO20);
  	HI_MDM_IO_SetDir(HI_GPIO_IDX_20,HI_GPIO_DIRECTION_OUT);//设置方向为
  	HI_MDM_IO_SetOutputVal(HI_GPIO_IDX_20, HI_GPIO_VALUE1);//设置电平为
	return HI_ERR_SUCCESS;
}
HI_U32 mrsRf3762_F6_F101(HI_U8 *param, HI_U8 ucSize)
{
	//HI_U32 ret=HI_ERR_SUCCESS;
	HI_U32 i=0;
	//HI_U8 payload=0x14;
	//HI_U16 usCRC16=0;
	HI_U16 usDataLen=0;
	MRS_SRV_LINK * it;
	MRS_SRV_LIST * list;
	MRS_RF_REPORT_DATA_NODE *pTempNode=HI_NULL;
	MRS_RF_ALL_WATER_METER_INFO * pCcoWaterMeterInfo = mrsRfGetCCOAllWaterMeterInfo();
	HI_U32 ulNowTime=HI_MDM_GetSeconds();
	
    //HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1006, HI_DIAG_MT("[RF][F6_F101]内容"), param, ucSize);
	MRS_RF_DAU_FRAME_STRU *pDauFrame = mrsRFDauOnwWayDecode(param+2,ucSize-2);
	if(pDauFrame==HI_NULL)
	{
		return HI_ERR_BAD_FRAME;
	}
	
    HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1007, HI_DIAG_MT("[RF][F4_F101]内容2"), pDauFrame->ucapDataData, pDauFrame->ucDataLen);
	for(i=0;i<pCcoWaterMeterInfo->ucWaterMeterCount;i++)
	{
		if(0==mrsToolsMemcmp(pCcoWaterMeterInfo->saWaterMeterInfo[i].ucaWaterMeterAddr,pDauFrame->ucaDataSrcAddr,7))
		{
			pCcoWaterMeterInfo->saWaterMeterInfo[i].ucFieldValue =param[1];
			break;
		}
	}
	if((pCcoWaterMeterInfo->ucWaterMeterCount<RF_MAX_WATER_METER_COUNT)&&(i >= pCcoWaterMeterInfo->ucWaterMeterCount))
	{
		mrsToolsMemcpy_s(pCcoWaterMeterInfo->saWaterMeterInfo[pCcoWaterMeterInfo->ucWaterMeterCount].ucaWaterMeterAddr, 7, pDauFrame->ucaDataSrcAddr, 7);
		pCcoWaterMeterInfo->saWaterMeterInfo[pCcoWaterMeterInfo->ucWaterMeterCount].ucFieldValue =param[1];
		pCcoWaterMeterInfo->ucWaterMeterCount++;
	}
	//HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_090, HI_DIAG_MT("[RF][F6_F101]RSSI"),param[1]);
	//CJJ-HPLC-20181127
	//usDataLen = sizeof(MRS_RF_REPORT_DATA_NODE)+pDauFrame->ucapDataData[0];
	usDataLen = sizeof(MRS_RF_REPORT_DATA_NODE)+pDauFrame->ucDataLen-2;
	
	//HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_074, HI_DIAG_MT("[RF][F6_F101]usDataLen"),usDataLen,pDauFrame->ucapDataData[0]);
	MRS_RF_REPORT_DATA_NODE *node = (MRS_RF_REPORT_DATA_NODE*)mrsToolsMalloc(usDataLen);
	if(!node)
	{
		//HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_075, HI_DIAG_MT("[RF][F6_F101]mrsToolsMalloc Fail"));
		mrsToolsFree(pDauFrame);
		return HI_ERR_MALLOC_FAILUE;
	}
	mrsToolsMemZero_s(node,sizeof(MRS_RF_REPORT_DATA_NODE),sizeof(MRS_RF_REPORT_DATA_NODE));
	//CJJ-HPLC:20181127
	//node->ucReportDataLen = pDauFrame->ucapDataData[0];
	//mrsToolsMemcpy_s(node->ucPStaReportData, pDauFrame->ucapDataData[0],pDauFrame->ucapDataData+1, pDauFrame->ucapDataData[0]);
	node->ucReportDataLen = pDauFrame->ucDataLen-2;
	mrsToolsMemcpy_s(node->ucPStaReportData, node->ucReportDataLen,pDauFrame->ucapDataData+2, node->ucReportDataLen);
	
	node->ulStartTime = HI_MDM_GetSeconds();
    //HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1008, HI_DIAG_MT("[RF][F6_F101]内容3"), node->ucPStaReportData, node->ucReportDataLen);
	if(pCcoWaterMeterInfo->ucReportDataCount >64)
	{
		mrsSrvListRemoveHeadNode(&(pCcoWaterMeterInfo->pStaReportDataList));
	}
	list=&(pCcoWaterMeterInfo->pStaReportDataList);
	// 遍历链表
    for (it = list->next; it != list; it = it->next)
    {
        pTempNode = (MRS_RF_REPORT_DATA_NODE *)it;
		if(pTempNode->ucReportDataLen<6)
		{
			break;
		}
		if((0==mrsToolsMemcmp(pTempNode->ucPStaReportData,node->ucPStaReportData,6))
			|| ((ulNowTime - pTempNode->ulStartTime)>259200))
		{
			pCcoWaterMeterInfo->ucReportDataCount--;
			mrsSrvListRemove(list,(MRS_SRV_LINK *)pTempNode);
			mrsToolsFree(pTempNode);
			//HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_094, HI_DIAG_MT("[RF]Del one ucaStaReportData,Count"),pCcoWaterMeterInfo->ucReportDataCount);			
			break;
		}
    }
	mrsSrvListInsTail(&(pCcoWaterMeterInfo->pStaReportDataList),&(node->link));
	pCcoWaterMeterInfo->ucReportDataCount++;
	mrsToolsFree(pDauFrame);
	return HI_ERR_SUCCESS;
}
HI_U32 mrsCcoSendADU0x42Data(HI_VOID)
{
	HI_U8 Payload[16];
	static HI_U8 s_ucBrocastSn=0;
	HI_U8 ucOffent=0;
	HI_U32 ret=HI_ERR_SUCCESS;
	HI_U16 usMaxTei=0;
	HI_U16 usMeterCount=0;
	HI_U32 i=0;
	HI_U8 *pDauData=HI_NULL;
	HI_U16 ucDauDataLen=0;
	HI_U8 *pData=HI_NULL;
	//HI_U16 ucDataLen=0;

	MRS_CCO_SRV_CTX_STRU *pCtx = mrsCcoGetContext();
	HI_U16 usMeterAllNum = mrsSrvArchivesMeterNum();
	for(i=0;i<pCtx->stTopoInf.num;i++)
	{
		if(pCtx->stTopoInf.entry[i].tei >usMaxTei)
		{
			usMaxTei = pCtx->stTopoInf.entry[i].tei;
		}
	}

    MRS_ARCHIVES_NODE_STRU *pstNode = HI_NULL;
	//在此处找到当前电表信息中第一个有效的表信息（序号）
    for (i = 0; i < usMeterAllNum; i++)
    {
        pstNode = mrsSrvArchivesQuery(i);
        if (pstNode == HI_NULL)
        {
            break;
        }

        if (pstNode->bValid && (pstNode->ucProtocol>=METER_PROTO_SINGLEWAY_WATERMETER)) // 查找到一个有效的表信息
        {
            usMeterCount++;
        }
    }
	Payload[ucOffent++]=s_ucBrocastSn;
	Payload[ucOffent++]=(usMaxTei &0xFF);
	Payload[ucOffent++]=((usMaxTei>>8) &0xFF);
	Payload[ucOffent++]=1;
	Payload[ucOffent++]=0;
	Payload[ucOffent++]=(usMeterCount&0xFF);
	Payload[ucOffent++]=((usMeterCount>>8)&0xFF);
	HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_040, HI_DIAG_MT("[RF]usMeterCount,usMaxTei"),usMeterCount,usMaxTei);
	ret=mrsCCORFDauEncodeFrame(&pDauData,&ucDauDataLen,Payload,ucOffent,pCtx->ucMainNodeAddr,MRS_RF_BROADCAST_MAC,0x42,0);
	if(ret !=HI_ERR_SUCCESS)
	{
		return ret;
	}
	pData = (HI_U8 *)mrsToolsMalloc(ucDauDataLen+1);
	if(pData ==HI_NULL)
	{
		mrsToolsFree(pDauData);
		return HI_ERR_MALLOC_FAILUE;
	}
	pData[0]=ucDauDataLen;
	mrsToolsMemcpy_s(pData+1,ucDauDataLen,pDauData,ucDauDataLen);
	mrs3762_RF_ReportAFN04_F6(pData,ucDauDataLen+1,2,1,0x42);
	mrsToolsFree(pDauData);
	mrsToolsFree(pData);
	return HI_ERR_SUCCESS;
}
HI_U32 mrsRFDauF3762_F7_DecodeFrame(HI_U8 *pPayload, HI_U8 ucPayloadLen)
{
	HI_U8 ucAllPage=0;
	HI_U8 ucCurPage=0;
	HI_U8 ucSRRINum=0;
	HI_U8 ucOffent=0,i=0;
	MRS_RF_ALL_WATER_METER_INFO * pMeterInfo = mrsRfGetCCOAllWaterMeterInfo();
	
	//HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_095, HI_DIAG_MT("[RF]3762_F7_DecodeFrame"),ucPayloadLen);
	if(ucPayloadLen<2)
	{
		return HI_ERR_FAILURE;
	}
	ucAllPage= ((pPayload[ucOffent]>>4)&0x0F);
	ucCurPage= (pPayload[ucOffent]&0x0F);
	ucOffent++;
	ucSRRINum = pPayload[ucOffent++];
	if((ucSRRINum*8+2) !=ucPayloadLen)
	{
		//HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_048, HI_DIAG_MT("[RF]ucSRRINum,ucPayloadLen"),ucSRRINum,ucPayloadLen);
		return HI_ERR_FAILURE;
	}
	if(ucAllPage>(ucCurPage+1))
	{
		mrs3762_RF_ReportAFN04_F7((ucCurPage+1));
	}
	if(ucCurPage==0)
	{
		pMeterInfo->ucWaterMeterCount=0;
		mrsToolsMemZero_s(pMeterInfo->saWaterMeterInfo, sizeof(MRS_RF_WATER_METER_SIGLE)*RF_MAX_WATER_METER_COUNT, sizeof(MRS_RF_WATER_METER_SIGLE)*RF_MAX_WATER_METER_COUNT);
	}
	for(i=0;i<ucSRRINum;i++)
	{
		mrsToolsMemcpy_s(pMeterInfo->saWaterMeterInfo[pMeterInfo->ucWaterMeterCount].ucaWaterMeterAddr,7,pPayload+ucOffent+i*8,7);
		pMeterInfo->saWaterMeterInfo[pMeterInfo->ucWaterMeterCount].ucFieldValue = pPayload[ucOffent+i*8+7];
		pMeterInfo->ucWaterMeterCount++;
	}
}

HI_U32 mrsCCORFDauEncodeFrame(HI_U8 **ppData,HI_U16 *ucpDataLen,HI_U8 *pPayload, HI_U8 ucPayloadLen,
	HI_U8 ucaDataSrcAddr[6],HI_U8 ucaDataDstAddr[7],HI_U8 ucCmd,HI_U8 ucChannelInfo)
{
	HI_U32 ret=HI_ERR_SUCCESS;
	//MRS_CCO_SRV_CTX_STRU* pCcoCtx = mrsCcoGetContext();
	MRS_RF_DAU_FRAME_STRU *pDauData = (MRS_RF_DAU_FRAME_STRU *)mrsToolsMalloc(sizeof(MRS_RF_DAU_FRAME_STRU));
	if(pDauData==HI_NULL)
	{
		return HI_ERR_MALLOC_FAILUE;
	}
	pDauData->ucFrameLen=23+ucPayloadLen;
	/*
	if((ucCmd==0x42) ||(ucCmd==0x44) )
	{
		pDauData->ucChannelIndex =0;
		
	}
	else
	{
		pDauData->ucChannelIndex =g_CcoNetSnid%32;
	}*/
	pDauData->ucChannelIndex =ucChannelInfo;
	pDauData->ucStandardIdNum= 0x01;
	pDauData->ucFrameHeader =mrsRFDAUFrameHeader(pDauData->ucFrameLen,pDauData->ucChannelIndex,pDauData->ucStandardIdNum);

	pDauData->ucsDataFrameCtrl =0x47cd;
	pDauData->ucDataFrameSn=mrsGetDauFrameSn();
	mrsAddDauFrameSn();
	pDauData->usDataPinId = 0xFFFF;

	mrsToolsMemcpy_s(pDauData->ucaDataDstAddr,7,ucaDataDstAddr,7);

	//mrsToolsMemcpy_s(pDauData->ucaDataSrcAddr,7,pCcoCtx->ucMainNodeAddr,6);
	mrsToolsMemcpy_s(pDauData->ucaDataSrcAddr,7,ucaDataSrcAddr,6);
	mrsHexInvert(pDauData->ucaDataSrcAddr,6);
	if(mrsToolsBroadCastMac(pDauData->ucaDataSrcAddr))
	{
		pDauData->ucaDataSrcAddr[6]=0xFF;
	}
	else
	{
		pDauData->ucaDataSrcAddr[6]=0;
	}
	pDauData->ucDataCmdId=ucCmd;
	pDauData->ucDataLen = ucPayloadLen;
	mrsToolsMemcpy_s(pDauData->ucapDataData,ucPayloadLen,pPayload,ucPayloadLen);

	ret = mrsRFDauEncode(ppData,ucpDataLen,pDauData);
	mrsToolsFree(pDauData);
	return ret;
}
HI_U32 mrsRfCcoDelUartDataItem(HI_VOID)
{
	MRS_SRV_QUEUE *pRfQueue=&(mrsCcoRfUartRun.RfCcoQueue);
    MRS_RF_CCO_UART_NODE *pstNode = (MRS_RF_CCO_UART_NODE *)mrsSrvDeQueue(pRfQueue);

    if (pstNode)
    {
        mrsToolsFree(pstNode);
    }
    return HI_ERR_SUCCESS;
}
struct Rf_CCO_que_param
{
    MRS_RF_CCO_UART_NODE * item;    //节点
    HI_U8 result;           //结果
    HI_U8 padding[3];
};

HI_BOOL mrsCcoRfQueueReplace(HI_VOID * node,HI_VOID * param)
{
    struct Rf_CCO_que_param * p = (struct Rf_CCO_que_param *)param;
    MRS_RF_CCO_UART_NODE * item = (MRS_RF_CCO_UART_NODE *)node;

    //参数有误
    if (!p || !node)
    {
        return HI_TRUE;
    }
    p->result = HI_FALSE;

    if (p->item->ulRfCrc == item->ulRfCrc)
    {
		//HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_057, HI_DIAG_MT("重复"),p->item->ulRfCrc,item->ulRfCrc);
       p->result=HI_TRUE;
       return HI_TRUE;
    }

    return HI_FALSE;
}

HI_BOOL mrsCcoRfTryEnQueue(MRS_SRV_QUEUE *queue,MRS_RF_CCO_UART_NODE * item)
{
    HI_BOOL ret = HI_TRUE;
    if (queue && item)
    {
        struct Rf_CCO_que_param extra = {0};


        extra.item = item;
        extra.result = HI_FALSE;

        //相同会话的是不可以入队列的
        mrsSrvTraverseQueue(queue, mrsCcoRfQueueReplace,&extra);

        //如果找到该业务的其它数据包
        if (extra.result)
        {
            //HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_056, HI_DIAG_MT("queue is 重复的"));
            return HI_TRUE;
        }

        mrsSrvEnQueue(queue,(MRS_SRV_NODE *)item);
    }
    return ret;
}

HI_U32 mrsCcoRfInsertUartDataItem(HI_U16 usRfTimeOut,HI_U8 *ucData, HI_U16 usLen,HI_U8 nCi,HI_U8 ucMaxTry)
{
	MRS_SRV_QUEUE *pRfQueue=&(mrsCcoRfUartRun.RfCcoQueue);
	HI_U16 usRfDataLen=sizeof(MRS_RF_CCO_UART_NODE)+usLen;
    MRS_RF_CCO_UART_NODE *pstNode = mrsToolsMalloc(usRfDataLen);

    if (!pstNode)
    {
        return HI_ERR_MALLOC_FAILUE;
    }
	
    mrsToolsMemZero_s(pstNode, usRfDataLen, usRfDataLen);
	pstNode->usRfTimeOut = usRfTimeOut;
    pstNode->usNodeDataLen = usLen;
	pstNode->ucRfNodeCi = nCi;
	pstNode->ucRfSendMaxTry =(ucMaxTry>10?(10):ucMaxTry);
	
    mrsToolsMemcpy_s(pstNode->uaNodeData, pstNode->usNodeDataLen, ucData, pstNode->usNodeDataLen); //采集器地址
	pstNode->ulRfCrc = HI_MDM_Crc32(0, pstNode->uaNodeData, pstNode->usNodeDataLen);
    //(HI_VOID)mrsSrvEnQueue(pRfQueue, &pstNode->RfLink);
   	mrsCcoRfTryEnQueue(pRfQueue,pstNode);
    return HI_ERR_SUCCESS;
}

HI_U32 mrsCcoRfUartTimerFun(HI_VOID)
{
	static HI_U32 ulTimeCount=0;
	MRS_SRV_QUEUE *pRfQueue=&(mrsCcoRfUartRun.RfCcoQueue);
	if(mrsSrvQueueIsEmpty(pRfQueue))
	{
		mrsCcoRfUartRun.ucRfCcoLock = HI_FALSE;
		return HI_ERR_SUCCESS;
	}
	MRS_RF_CCO_UART_NODE *pstNode = (MRS_RF_CCO_UART_NODE *)mrsSrvQueueTop(pRfQueue);
	if(HI_NULL==pstNode)
	{
		return HI_ERR_SUCCESS;
	}
	if(mrsCcoRfUartRun.ucRfCcoLock)
	{
		ulTimeCount++;
		if(ulTimeCount > pstNode->usRfTimeOut)
		{
			mrsCcoRfUartRun.ucRfSendTry++;
			if(mrsCcoRfUartRun.ucRfSendTry<mrsCcoRfUartRun.ucRfSendMaxTry)
			{
				ulTimeCount=0;
				MRS_SendMrData(pstNode->uaNodeData, pstNode->usNodeDataLen, HI_DMS_CHL_UART_PORT_DIAG);
			}
			else
			{
				if(pstNode->ucRfNodeCi==0x40)
				{
					mrsAFN13F1ReportAck(HI_NULL);
				}
				mrsRfCcoDelUartDataItem();
				mrsCcoRfUartRun.ucRfCcoLock=HI_FALSE;
			}
		}
	}
	else
	{
		mrsCcoRfUartRun.ucRfCcoLock = HI_TRUE;
		mrsCcoRfUartRun.ucRfSendTry=0;
		mrsCcoRfUartRun.ucRfSendMaxTry=pstNode->ucRfSendMaxTry;
		ulTimeCount=0;
		MRS_SendMrData(pstNode->uaNodeData, pstNode->usNodeDataLen, HI_DMS_CHL_UART_PORT_DIAG);
	}
	return HI_ERR_SUCCESS;
}
HI_U32 mrsCcoRFDau0x4XEncodeFrame(HI_U8 *pPayload, HI_U8 ucPayloadLen,HI_U8 ucAddr[7],HI_U16 nTime,HI_U8 ucCmdid)
{
	HI_U8 *pDauData=HI_NULL;
	HI_U8 *pData=HI_NULL;
	HI_U16 ucDauDataLen=0;
	HI_U32 ret =HI_ERR_SUCCESS;
	MRS_CCO_SRV_CTX_STRU* pCcoCtx = mrsCcoGetContext();
	
	HI_U8 ucChannelInfo=0;
	HI_U32 j=0;
	MRS_RF_ALL_WATER_METER_INFO *pCcoWaterMeter =mrsRfGetCCOAllWaterMeterInfo();
	for(j=0;j<pCcoWaterMeter->ucWaterMeterCount;j++)
	{
		if(0==mrsToolsMemcmp(ucAddr,pCcoWaterMeter->saWaterMeterInfo[j].ucaWaterMeterAddr,7))
		{
			ucChannelInfo = pCcoWaterMeter->saWaterMeterInfo[j].ucChannelInfoNum*2;
			break;
		}
	}
	
	//HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_089, HI_DIAG_MT("[RF]CCO ucCmdid,ucChannelInfo"),ucCmdid,ucChannelInfo);
	ret=mrsCCORFDauEncodeFrame(&pDauData,&ucDauDataLen,pPayload,ucPayloadLen,pCcoCtx->ucMainNodeAddr,ucAddr,ucCmdid,ucChannelInfo);
	if(ret !=HI_ERR_SUCCESS)
	{
		return ret;
	}
	pData = (HI_U8 *)mrsToolsMalloc(256);
	if(pData==HI_NULL)
	{
		return HI_ERR_MALLOC_FAILUE;
	}
	pData[0]=ucDauDataLen;
	mrsToolsMemcpy_s(pData+1,ucDauDataLen,pDauData,ucDauDataLen);
	mrsToolsFree(pDauData);
	mrs3762_RF_ReportAFN04_F6(pData,ucDauDataLen+1,nTime,2,ucCmdid);
	mrsToolsFree(pData);
	return ret;
}
HI_VOID mrsCcoRFDau0x44EncodeFrame(HI_VOID)
{
	MRS_CCO_SRV_CTX_STRU* pCcoCtx = mrsCcoGetContext();
	MR_WATER_METER_SEARCH_STRU *pWaterMeter=&(pCcoCtx->stWaterMeterSearchModel);
	HI_U32 i=0,noffent=0;
	HI_U8 ucData[32];
	MRS_SRV_LINK * it;
    MRS_SRV_LIST * list;
	MRS_SEARCH_WATER_METER_LIST *node=HI_NULL;
	HI_SYS_CALENDAR_TIME_S stime;
	
	list = &(pWaterMeter->stGetWaterMeterList);
	for (it = list->next; it != list; it = it->next)
	{
		 node = (MRS_SEARCH_WATER_METER_LIST *)it;
		 if(node->ucAddressType)
		 {
		 	for(i=0;i<node->ucMeterNum;i++)
			{
				noffent=0;
				ucData[noffent++]=(pCcoCtx->ulCcoNetSnid%32+1);
				mrsToolsMemcpy_s(ucData+noffent,6,pCcoCtx->ucMainNodeAddr,6);
				mrsHexInvert(ucData+noffent,6);
				noffent+=6;
				ucData[noffent++]=1;//(g_CcoNetSnid&0xFF);
				ucData[noffent++]=0;//((g_CcoNetSnid>>8) &0xFF);

				ucData[noffent++]=1; // 时隙
				ucData[noffent++]=0;

				 HI_MDM_GetCalendarTime(&stime);
	
    			mrsIntToBcd((HI_U32)stime.sec, ucData+noffent, sizeof(HI_U8));
				noffent++;
				mrsIntToBcd((HI_U32)stime.min, ucData+noffent, sizeof(HI_U8));
				noffent++;
				mrsIntToBcd((HI_U32)stime.hour, ucData+noffent, sizeof(HI_U8));
				noffent++;
				mrsIntToBcd((HI_U32)stime.day, ucData+noffent, sizeof(HI_U8));
				noffent++;
				mrsIntToBcd((HI_U32)stime.month,ucData+noffent, sizeof(HI_U8));
				noffent++;
				mrsIntToBcd((HI_U32)stime.year,ucData+noffent, sizeof(HI_U16));
				noffent+=2;
				
				ucData[noffent++]=1; // 工作模式
				
				ucData[noffent++]=0xFF; // 工作时段
				ucData[noffent++]=0xFF;
				ucData[noffent++]=0xFF;
				HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_076, HI_DIAG_MT("[RF]CCO SetMeterOwnerShip "),i);

				mrsCcoRFDau0x4XEncodeFrame(ucData,noffent,(node->pMeterData+i*8),100,0x44);
			}
			mrsSrvListRemove(list,&(node->link));
			mrsToolsFree(node);
			break;
		 }
	}
}

#endif

#endif

