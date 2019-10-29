//*****************************************************************************
//
//				  版权所有 (C), 深圳市国电科技通信有限公司
//
//*****************************************************************************
	  //  文 件 名	 : mrs_srv_rf.h
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

#ifndef __MRS_SRV_RF_H__
#define __MRS_SRV_RF_H__

//CJJ-HPLC-20181204:水表组网状态设置与查询
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)

typedef enum
{
	WATER_METER_NET_TABLE = 1,
	WATER_METER_START_NET,
	WATER_METER_COMPLETE_NET

}WATER_METER_NET_STATUE;

HI_VOID setWaterMeterNetStatue(WATER_METER_NET_STATUE statue);
WATER_METER_NET_STATUE   getWaterMeterNetStatue(HI_VOID);
#endif
//CJJ-HPLC-20181204


#if defined(PRODUCT_CFG_SUPPORT_MRS_DUOL_MODE_RF)
#include "mrs_srv_list.h"
#include "mrs_srv_queue.h"
#define RF_MAX_WATER_METER_COUNT 48
#define  MRS_RF_BROADCAST_MAC              "\xff\xff\xff\xff\xff\xff\xff" 

typedef struct
{
	MRS_SRV_LINK link;   // 链接 MRS_RF_REPORT_DATA_NODE
	HI_U32 ulStartTime;
	HI_U8 ucRecvWaterMeterAddr[6];
	HI_U8 ucReportDataState;
	HI_U8 ucReportDataLen;
	HI_U8 ucPStaReportData[0];
}MRS_RF_REPORT_DATA_NODE;

typedef struct
{
	HI_U8 ucaWaterMeterAddr[7]; // 水表地址
	HI_U8 ucFieldValue; // 场强信息
	HI_U8 ucConfigInfo; // 配置信息
	HI_U8 ucChannelInfoNum; // 通道信息
	HI_U8 ucaRev[2];
} MRS_RF_WATER_METER_SIGLE;

typedef struct
{
	HI_U8 ucWaterMeterCount;
	MRS_RF_WATER_METER_SIGLE saWaterMeterInfo[RF_MAX_WATER_METER_COUNT];
	HI_U8 ucReportDataCount;
	MRS_SRV_LIST pStaReportDataList; // 指向MRS_RF_REPORT_DATA_NODE
} MRS_RF_ALL_WATER_METER_INFO;

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
#define MRS_RF_645_MIN_LENGTH              12 // 645帧最小长度
#define RF_DATA_BUF_MAX 256

#define MRS_RF_ERR_BUSY             (0x01)
#define MRS_RF_ERR_NOT_EXIST        (0x02)
#define MRS_RF_ERR_NOT_SUPPORT      (0x03)

#define MRS_RF_RF_FRAME_CTRL       0
#define MRS_RF_RF_FRAME_SN         2
#define MRS_RF_RF_FRAME_PinID      3
#define MRS_RF_RF_FRAME_DST_ADDR   5
#define MRS_RF_RF_FRAME_SRC_ADDR   12
#define MRS_RF_RF_FRAME_CMD_ID     19
#define MRS_RF_RF_FRAME_LOAD_DATA  20

typedef struct
{
    HI_U8   ucRfLock;
	HI_U8   ucRfSendTry;// 发送次数
	HI_U8   ucRfSendMaxTime;// 发送最大次数
	HI_U8   ucRfTimeCount;// 发送超时时间
	HI_U8   ucRfCi;
    HI_U8   aucRfTxAddr[HI_METER_ADDR_LEN];
    MRS_SRV_QUEUE RfQueue;
} MRS_RF_RUNNING;
typedef struct
{
    MRS_SRV_NODE RfLink;
	HI_U8  ucRfNodeCi;
	HI_U16 usRfTimeOut;
	HI_U32 ulRfCrc;
	HI_U8  ucRfNodeSendMaxTime;// 发送最大次数
	HI_U16 usNodeDataLen;
	HI_U8 uaNodeData[0];
} MRS_RF_UART_NODE;


typedef struct
{
    HI_U8 ucAddr[6];           // 表地址，固定6字节 
    HI_U8 ucCtrl; // 控制码
    HI_U8 ucCl;//CI长度为一个Byte、用于表示扩展标识符
    HI_U8 ucDataRealmLen; // 数据域大小必须小于200字节, 0表示无数据域
    HI_U8 ucDataRealm[RF_DATA_BUF_MAX]; // 数据域 
}MRS_RF_645_FRAME_STRU;

//数据标识码处理函数
typedef HI_U32 (*MRS_RF_645_CI_FUN)(HI_VOID * param, HI_PVOID *ppOutParam, HI_U16 *pOutParamLen, HI_U8 ucPreSize);


typedef struct
{
    HI_U8 ucFrameLen;
    HI_U8 ucChannelIndex;
    HI_U8 ucStandardIdNum;
    HI_U8 ucFrameHeader;
    HI_U8 ucapData[0]; // 数据域 
}MRS_RF_RF_FRAME_STRU;


typedef struct
{
    HI_U16 usPlcSn;
	HI_U16 usSend0x25TimeCount;
	HI_U16 usRfTx0x44TimeCount;
	HI_U8 ucSrcMeter[6];
    HI_U8 ucDstMeter[6];
	HI_U8 ucOwnerInfoNum;
	HI_U8 ucWaterMeterType;
	HI_U8 *pOwnerInfo;
}MRS_RF_PLC_RUN_INFO_STRU;

//*****************************************************************************
// 功能描述: 接收过来的完整帧

//*****************************************************************************
HI_U32 mrsRfInit(HI_VOID);

HI_U32 mrsStaRFRecvDealFun(HI_DMS_CHL_RX_S* pstRxData);

HI_VOID mrsRfTransmit(HI_PBYTE pucData, HI_U16 usDataSize,HI_U16 usTimeout,HI_U8 nCi);
HI_VOID mrsRfDirectTransmit(HI_PBYTE pucData, HI_U16 usDataSize);
HI_VOID mrsRfTransmitTimeAndCount(HI_PBYTE pucData, HI_U16 usDataSize,HI_U16 usTimeout,HI_U8 nCi,HI_U8 ucSendCount);
HI_VOID mrsOneSetRfSetAddress(HI_VOID);

HI_U32 mrsRfUartTimerFun(HI_VOID);

HI_VOID mrsRfSetStateInfo(HI_U8 nFlag);
HI_VOID mrsRfSetTimeslot(HI_U16 usTimeslot);
HI_VOID mrsRfSetAddress(HI_VOID);
HI_VOID mrsRfSetSnidTei(HI_VOID);
HI_VOID mrsRfClearPowerState(HI_VOID);
HI_VOID mrsRfSetNeedRfTransmitPowerInfo(HI_U32 ulNeedSendCount);
HI_VOID mrsRfSetMaxTei(HI_U16 usMaxTei);
HI_VOID mrsRfReadRSSI25(HI_U8 ucPage);
HI_VOID mrsRfSetDirectTransfer24(HI_U8 *pData,HI_U16 usDateLen);
MRS_RF_PLC_RUN_INFO_STRU *mrsGetRfPlcRunInfo(HI_VOID);
HI_U16 mrsGetTei(HI_VOID);

//HI_U32 mrsRFDau0x4XEncodeFrame(HI_U8 *pPayload, HI_U8 ucPayloadLen,HI_U8 ucAddr[7],HI_U16 nTime,HI_U8 ucCmdid);
HI_U32 mrsRFDau0x42EncodeFrame(HI_U8 *pPayload, HI_U8 ucPayloadLen,HI_U16 usSend0x25Time);
//HI_VOID mrsRfSetMeterOwnerShip(HI_U8 ucMeterCount,HI_U8 *ucaData,HI_U8 *nTime);


HI_U32 mrsRfStaCreateReadMeterPlc(HI_VOID *pFrame,HI_U8 *data,HI_U16 len,HI_U16 usId);
HI_U32 mrsStaRfDataTransmit2CCO(HI_U8 * data, HI_U16 len,HI_U16 usID);
HI_U32 mrsStaCreateResponseSetShipPlc(HI_VOID *pFrame,MRS_RF_PLC_RUN_INFO_STRU *pPlcRunInfo,HI_U16 usId);
HI_U32 mrsStaRfOwnerShipDataTransmit2CCO(HI_U8 * data, HI_U16 len,HI_U16 usID);




MRS_RF_ALL_WATER_METER_INFO *mrsRfGetAllWaterMeterInfo(HI_VOID);
HI_U32 mrsRFDauEncodeFrame(HI_U8 **ppData,HI_U16 *ucpDataLen,HI_U8 *pPayload, HI_U8 ucPayloadLen,
	HI_U8 ucaDataDstAddr[7],HI_U8 ucCmd,HI_U8 ucChannelInfo);


#endif


#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
HI_U32 mrsCCORFDauEncodeFrame(HI_U8 **ppData,HI_U16 *ucpDataLen,HI_U8 *pPayload, HI_U8 ucPayloadLen,
	HI_U8 ucaDataSrcAddr[6],HI_U8 ucaDataDstAddr[7],HI_U8 ucCmd,HI_U8 ucChannelInfo);

typedef struct
{
    HI_U8   ucRfCcoLock;
	HI_U8   ucRfSendTry;// 发送次数
	HI_U8   ucRfSendMaxTry;// 发送次数
	HI_U8   ucRfTimeCount;// 发送超时时间
	HI_U8   ucRfCi;
    HI_U8   aucRfTxAddr[HI_METER_ADDR_LEN];
    MRS_SRV_QUEUE RfCcoQueue;
} MRS_CCO_RF_UART_RUNNING;

typedef struct
{
    MRS_SRV_NODE RfLink;
	HI_U8   ucRfNodeCi;
	HI_U8 ucRfSendMaxTry;
	HI_U16 usRfTimeOut;
	
	HI_U32 ulRfCrc;
	HI_U16 usNodeDataLen;
	HI_U8 uaNodeData[0];
} MRS_RF_CCO_UART_NODE;



MRS_RF_ALL_WATER_METER_INFO *mrsRfGetCCOAllWaterMeterInfo(HI_VOID);

HI_U32 mrsCcoRFRecvDealFun(HI_DMS_CHL_RX_S* pstRxData);
HI_U32 mrs3762_RF_ReportAFN04_F5(HI_U32 ulSnid);

HI_U32 mrs3762_RF_ReportAFN04_F6(HI_U8 *pData,HI_U16 usDatalen,HI_U16 usRfTimeOut,HI_U8 ucMaxTry,HI_U8 ucID);
HI_U32 mrs3762_RF_ReportAFN04_F7(HI_U8 ucPageNum);

HI_U32 AppCCOMDMQuerySn(HI_U32 usMsgId);
HI_U32 mrsRfCCOInit(HI_VOID);
HI_U32 mrsCcoSGDTimeSlotFun(HI_VOID);

HI_U32 mrsCcoSendTimeSlotPlcFrame(HI_U8 * pPlcData,HI_U32 waterBoardTimer);
HI_VOID mrsCcoRFDau0x44EncodeFrame(HI_VOID);

HI_U32 mrsCcoRFDau0x4XEncodeFrame(HI_U8 *pPayload, HI_U8 ucPayloadLen,HI_U8 ucAddr[7],HI_U16 nTime,HI_U8 ucCmdid);
HI_U32 mrsCcoRfUartTimerFun(HI_VOID);
HI_U32 mrsCcoRfInsertUartDataItem(HI_U16 usRfTimeOut,HI_U8 *ucData, HI_U16 usLen,HI_U8 nCi,HI_U8 ucMaxTry);
#endif

#endif

#endif
