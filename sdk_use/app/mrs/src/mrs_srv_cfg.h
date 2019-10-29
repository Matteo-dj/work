//*****************************************************************************
//
//                  版权所有 (C), 1998-2011, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_srv_cfg.h
//  版 本 号   : V1.0
//  作    者   : shenhankun/00130424
//  生成日期   : 2011-09-26
//  功能描述   : DIAG 通道类命令配置文件
//
//  函数列表   : NA
//  修改历史   :
//  1.日    期 : 2011-09-26
//    作    者 : shenhankun/00130424
//    修改内容 : 创建文件
//
//*****************************************************************************

#ifndef _MRS_SRV_CFG_H_
#define _MRS_SRV_CFG_H_


#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
HI_U32 mrsCmdCcoDataTransmit(HI_U16 usId, HI_IN HI_PVOID, HI_U16);
HI_U32 mrsCmdCcoGetMeterList(HI_U16 usId, HI_IN HI_PVOID, HI_U16);
HI_U32 mrsCmdCcoPlcTestTransmit(HI_U16 usId, HI_IN HI_PVOID, HI_U16);
HI_U32 mrsCmdCcoNdmPlcProc(HI_U16 usId, HI_IN HI_PVOID, HI_U16);
HI_U32 mrsCmdCcoPlcGetDevId(HI_IN HI_U16 id, HI_PVOID req_packet, HI_U16 packet_size);

HI_U32 mrsMsgCcoPlcChlStatusNotify(HI_U16 usId, HI_IN HI_PVOID);
HI_U32 mrsMsgCcoDeviceDataRx(HI_U16 usId, HI_IN HI_PVOID);
HI_U32 mrsMsgCcoTimer(HI_U16 usId, HI_IN HI_PVOID);
HI_U32 mrsCmdCcoGetPoweroffInfo(HI_U16 usId, HI_PVOID req_packet, HI_U16 packet_size);
HI_U32 mrsCmdCcoHandleTfCmd(HI_IN HI_U16 usId, HI_PVOID pReqPacket, HI_U16 usReqPacketSize);
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
HI_U32 mrsCmdStaDataTransmit(HI_U16 usId, HI_IN HI_PVOID, HI_U16);
HI_U32 mrsCmdStaSearchMeter(HI_U16 usId, HI_IN HI_PVOID, HI_U16);
HI_U32 mrsCmdStaPlcTestTransmit(HI_U16 usId, HI_IN HI_PVOID, HI_U16);

HI_U32 mrsMsgStaPlcChlStatusNotify(HI_U16 usId, HI_IN HI_PVOID);
HI_U32 mrsMsgStaTimer(HI_U16 usId, HI_IN HI_PVOID);

HI_U32 mrsMsgStaUartFrmRx_645(HI_U16 usMsgId, HI_PVOID pMsgParam);
HI_U32 mrsMsgStaUartFrmRx_698_45(HI_U16 usMsgId, HI_PVOID pMsgParam);
HI_U32 mrsMsgStaUartFrmRx_Trans(HI_U16 usMsgId, HI_PVOID pMsgParam);
HI_U32 mrsCmdStaGetPoweroffInfo(HI_IN HI_U16 usId, HI_PVOID pReqPacket, HI_U16 usReqPacketSize);
HI_U32 mrsCmdStaHandleTfCmd(HI_IN HI_U16 usId, HI_PVOID pReqPacket, HI_U16 usReqPacketSize);
HI_U32 mrsCmdStaPlcGetDevId(HI_IN HI_U16 id, HI_PVOID req_packet, HI_U16 packet_size);
#if defined(PRODUCT_CFG_SUPPORT_MRS_DUOL_MODE_RF_II)
HI_U32 mrsCmdStaSetTimeSlot(HI_IN HI_U16 usId, HI_PVOID pReqPacket, HI_U16 usReqPacketSize);
#endif
//HI_U32 mrsCmdStaStartFindOwnerShip(HI_IN HI_U16 usId, HI_PVOID pReqPacket, HI_U16 usReqPacketSize);
//HI_U32 mrsCmdStaGetAddressInfo(HI_IN HI_U16 usId, HI_PVOID pReqPacket, HI_U16 usReqPacketSize);
//HI_U32 mrsCmdStaSetMeterOwnerShip(HI_IN HI_U16 usId, HI_PVOID pReqPacket, HI_U16 usReqPacketSize);
//HI_U32 mrsCmdStaGetMeterDataInfo(HI_IN HI_U16 usId, HI_PVOID pReqPacket, HI_U16 usReqPacketSize);
#endif
#if defined(PRODUCT_CFG_PRODUCT_TYPE_NDM)
HI_U32 mrsNdmPlcProcess(HI_IN HI_U16 usId, HI_PVOID pReqPacket, HI_U16 usReqPacketSize);
hi_u32 mrs_msg_ndm_device_data_rx(hi_u16 id, hi_pvoid param);
hi_u32 mrs_ndm_handle_plc_detect_id(hi_u16 id, hi_pvoid packet, hi_u16 packet_size);
#endif


HI_PRV HI_CONST MRS_FW_CMD_PROC_TBL_STRU g_astCmdFunTable[] =
{
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    {ID_MRS_CMD_DATA_TRANSMIT_AFN13_PLC, ID_MRS_CMD_TEST_TRANSMIT_PLC, mrsCmdCcoDataTransmit},  // 抄表命令
    {ID_MRS_CMD_GET_METERLIST, ID_MRS_CMD_GET_METERLIST, mrsCmdCcoGetMeterList}, // 搜表命令
    {ID_MRS_CMD_TEST_TRANSMIT_AFNF0_PLC, ID_MRS_CMD_TEST_TRANSMIT_AFNF0_PLC, mrsCmdCcoPlcTestTransmit}, // 测试命令
    {ID_MRS_CMD_EVENT_INF_PLC, ID_MRS_CMD_EVENT_INF_PLC, mrsCmdCcoEvtTransmit},
    {ID_MRS_CMD_DATA_TRANSMIT_PLC, ID_MRS_CMD_DATA_TRANSMIT_PLC, mrsCmdCcoDataTransmit},  // 数据转发
    #if defined (PRODUCT_OPTIMIZE_MATTEO)
    {ID_MRS_CMD_GET_POWEROFF_INFO, ID_MRS_CMD_GET_POWEROFF_INFO, mrsCmdCcoGetPoweroffInfo}, // 读取停电信息
    #endif
    {ID_MRS_CMD_GET_DEVICE_ID_PLC, ID_MRS_CMD_GET_DEVICE_ID_PLC, mrsCmdCcoPlcGetDevId},  // 读取子设备各类ID
    {ID_MRS_CMD_NDM_CTRL_CCO_PLC, ID_MRS_CMD_NDM_CTRL_CCO_PLC, mrsCmdCcoNdmPlcProc}, // NDM的PLC帧处理函数
    {ID_MRS_CMD_TF_PLC, ID_MRS_CMD_TF_PLC, mrsCmdCcoHandleTfCmd}, // 台区识别报文处理
#endif
#if defined(PRODUCT_CFG_PRODUCT_TYPE_NDM)
    {ID_MRS_CMD_NDM_CTRL_CCO_PLC, ID_MRS_CMD_NDM_CTRL_CCO_PLC, mrsNdmPlcProcess}, // NDM的PLC帧处理函数
    {ID_MRS_CMD_DATA_TRANSMIT_AFN13_PLC, ID_MRS_CMD_DATA_TRANSMIT_AFN13_PLC, mrsNdmPlcProcess}, // NDM的PLC帧处理函数
    {ID_MRS_CMD_GET_DEVICE_ID_PLC, ID_MRS_CMD_GET_DEVICE_ID_PLC, mrs_ndm_handle_plc_detect_id},
#endif
#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
#if defined(PRODUCT_CFG_SUPPORT_TEST_CONCENTRATOR)
    {ID_MRS_CMD_DATA_TRANSMIT_PLC, ID_MRS_CMD_DATA_TRANSMIT_PLC, mrsCmdStaDataTransmit},
#else
#if defined(PRODUCT_CFG_SUPPORT_MRS_DUOL_MODE_RF_II)
		{ID_MRS_CMD_RF_SET_TIME_SLOT, ID_MRS_CMD_RF_SET_TIME_SLOT, mrsCmdStaSetTimeSlot},
#endif
		//{ID_MRS_CMD_RF_START_FIND_OWNER,ID_MRS_CMD_RF_START_FIND_OWNER,mrsCmdStaStartFindOwnerShip},
		//{ID_MRS_CMD_RF_GET_ADDRESS_INFO,ID_MRS_CMD_RF_GET_ADDRESS_INFO,mrsCmdStaGetAddressInfo},
		//{ID_MRS_CMD_RF_SET_METER_OWNERSHIP,ID_MRS_CMD_RF_SET_METER_OWNERSHIP,mrsCmdStaSetMeterOwnerShip},
		//{ID_MRS_CMD_RF_GET_METER_INFO,ID_MRS_CMD_RF_GET_METER_INFO,mrsCmdStaGetMeterDataInfo},

    {ID_MRS_CMD_DATA_TRANSMIT_AFN13_PLC, ID_MRS_CMD_TEST_TRANSMIT_PLC, mrsCmdStaDataTransmit},  // 抄表命令
    {ID_MRS_CMD_EVENT_INF_PLC, ID_MRS_CMD_EVENT_INF_PLC, mrsCmdStaEvtInfProc},  // 抄表命令
    {ID_MRS_CMD_GET_METERADDR_PLC, ID_MRS_CMD_GET_DEVICE_INF_PLC, mrsCmdStaSearchMeter}, // 搜表命令
    {ID_MRS_CMD_TEST_TRANSMIT_AFNF0_PLC, ID_MRS_CMD_TEST_TRANSMIT_AFNF0_PLC, mrsCmdStaPlcTestTransmit}, // 测试命令
    {ID_MRS_CMD_GET_POWEROFF_INFO, ID_MRS_CMD_GET_POWEROFF_INFO, mrsCmdStaGetPoweroffInfo}, // 读取停电信息
    {ID_MRS_CMD_TF_PLC, ID_MRS_CMD_TF_PLC, mrsCmdStaHandleTfCmd}, // 台区识别报文处理
    {ID_MRS_CMD_GET_DEVICE_ID_PLC, ID_MRS_CMD_GET_DEVICE_ID_PLC, mrsCmdStaPlcGetDevId}, // 获取设备各类ID
#endif
#endif
};

HI_PRV HI_CONST MRS_FW_MSG_PROC_TBL_STRU g_astMsgsFunTable[] =
{
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    {ID_MRS_MSG_PLC_CHL_STATUS, ID_MRS_MSG_PLC_CHL_STATUS, mrsMsgCcoPlcChlStatusNotify},    // PLC通道连接通知
    {ID_MRS_MSG_MR_DATA, ID_MRS_MSG_MR_DATA, mrsMsgCcoDeviceDataRx},    // 抄表设备数据接收
    {ID_MRS_MSG_MR_TIMER, ID_MRS_MSG_MR_TIMER, mrsMsgCcoTimer}, // 定时器
#endif
#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
    {ID_MRS_MSG_PLC_CHL_STATUS,     ID_MRS_MSG_PLC_CHL_STATUS,      mrsMsgStaPlcChlStatusNotify},   // PLC通道连接通知
    {ID_MRS_MSG_MR_TIMER,           ID_MRS_MSG_MR_TIMER,            mrsMsgStaTimer},                // 定时器
    {ID_MRS_MSG_MR_DATA_645,        ID_MRS_MSG_MR_DATA_645,         mrsMsgStaUartFrmRx_645},        // 抄表设备数据接收(645)
    {ID_MRS_MSG_MR_DATA_698_45,     ID_MRS_MSG_MR_DATA_698_45,      mrsMsgStaUartFrmRx_698_45},     // 抄表设备数据接收(698.45)
    {ID_MRS_MSG_MR_DATA_TRANSPARENT,ID_MRS_MSG_MR_DATA_TRANSPARENT, mrsMsgStaUartFrmRx_Trans},      // 抄表设备数据接收(透传报文)
#endif
#if defined(PRODUCT_CFG_PRODUCT_TYPE_NDM)
    {ID_MRS_MSG_MR_DATA, ID_MRS_MSG_MR_DATA, mrs_msg_ndm_device_data_rx},    /* 抄表设备数据接收 */
#endif
};



#endif

