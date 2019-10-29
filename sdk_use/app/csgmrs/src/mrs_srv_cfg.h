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
HI_U32 mrsCmdCcoSearchMeter(HI_U16 usId, HI_IN HI_PVOID, HI_U16);
HI_U32 mrsCmdCcoPlcTestTransmit(HI_U16 usId, HI_IN HI_PVOID, HI_U16);

HI_U32 mrsMsgCcoPlcChlStatusNotify(HI_U16 usId, HI_IN HI_PVOID);
HI_U32 mrsMsgCcoDeviceDataRx(HI_U16 usId, HI_IN HI_PVOID);
HI_U32 mrsMsgCcoTimer(HI_U16 usId, HI_IN HI_PVOID);

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
HI_U32 mrsCmdCcoCsgDataTransmit(HI_U16 usId, HI_IN HI_PVOID, HI_U16);
HI_U32 mrsMsgCcoDeviceDataRxCsg(HI_U16 usId, HI_IN HI_PVOID);
#endif

#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
HI_U32 mrsCmdStaDataTransmit(HI_U16 usId, HI_IN HI_PVOID, HI_U16);
HI_U32 mrsCmdStaSearchMeter(HI_U16 usId, HI_IN HI_PVOID, HI_U16);
HI_U32 mrsCmdStaPlcTestTransmit(HI_U16 usId, HI_IN HI_PVOID, HI_U16);

HI_U32 mrsMsgStaPlcChlStatusNotify(HI_U16 usId, HI_IN HI_PVOID);
HI_U32 mrsMsgStaDeviceDataRx(HI_U16 usId, HI_IN HI_PVOID);
HI_U32 mrsMsgStaTimer(HI_U16 usId, HI_IN HI_PVOID);

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
HI_U32 mrsCmdStaCsgDataTransmit(HI_U16 usId, HI_IN HI_PVOID, HI_U16);
HI_U32 mrsMsgStaDeviceDataRxCsg(HI_U16 usId, HI_IN HI_PVOID);
#endif
#endif

#if (defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) || defined(PRODUCT_CFG_PRODUCT_TYPE_STA))

HI_PRV HI_CONST MRS_FW_CMD_PROC_TBL_STRU g_astCmdFunTable[] =
{
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
    {ID_MRS_CMD_DATA_TRANSMIT_AFN13_PLC, ID_MRS_CMD_TEST_TRANSMIT_PLC, mrsCmdCcoDataTransmit},  // 抄表命令
    {ID_MRS_CMD_GET_METERLIST, ID_MRS_CMD_GET_DEVICE_INF_PLC, mrsCmdCcoSearchMeter}, // 搜表命令
    {ID_MRS_CMD_TEST_TRANSMIT_AFNF0_PLC, ID_MRS_CMD_TEST_TRANSMIT_AFNF0_PLC, mrsCmdCcoPlcTestTransmit}, // 测试命令
    {ID_MRS_CMD_EVENT_INF_PLC, ID_MRS_CMD_EVENT_INF_PLC, mrsCmdCcoEvtTransmit},
#else
    {ID_MRS_CMD_CSG_DATA_TRANSMIT_PLC, ID_MRS_CMD_CSG_DATA_TRANSMIT_PLC, mrsCmdCcoCsgDataTransmit},
#endif
    {ID_MRS_CMD_DATA_TRANSMIT_PLC, ID_MRS_CMD_DATA_TRANSMIT_PLC, mrsCmdCcoDataTransmit},  // 数据转发
#endif
#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
    {ID_MRS_CMD_DATA_TRANSMIT_AFN13_PLC, ID_MRS_CMD_TEST_TRANSMIT_PLC, mrsCmdStaDataTransmit},  // 抄表命令
    {ID_MRS_CMD_EVENT_INF_PLC, ID_MRS_CMD_EVENT_INF_PLC, mrsCmdStaEvtInfProc},  // 抄表命令
    {ID_MRS_CMD_GET_METERADDR_PLC, ID_MRS_CMD_GET_DEVICE_INF_PLC, mrsCmdStaSearchMeter}, // 搜表命令
    {ID_MRS_CMD_TEST_TRANSMIT_AFNF0_PLC, ID_MRS_CMD_TEST_TRANSMIT_AFNF0_PLC, mrsCmdStaPlcTestTransmit}, // 测试命令
#else
    {ID_MRS_CMD_CSG_DATA_TRANSMIT_PLC, ID_MRS_CMD_CSG_DATA_TRANSMIT_PLC, mrsCmdStaCsgDataTransmit},
#endif
#endif
};

HI_PRV HI_CONST MRS_FW_MSG_PROC_TBL_STRU g_astMsgsFunTable[] =
{
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    {ID_MRS_MSG_PLC_CHL_STATUS, ID_MRS_MSG_PLC_CHL_STATUS, mrsMsgCcoPlcChlStatusNotify},    // PLC通道连接通知
    {ID_MRS_MSG_MR_TIMER, ID_MRS_MSG_MR_TIMER, mrsMsgCcoTimer}, // 定时器
#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
    {ID_MRS_MSG_MR_DATA, ID_MRS_MSG_MR_DATA, mrsMsgCcoDeviceDataRx},    // 抄表设备数据接收
#else
    {ID_MRS_MSG_MR_DATA_CSG, ID_MRS_MSG_MR_DATA_CSG, mrsMsgCcoDeviceDataRxCsg},
#endif
#endif
#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
    {ID_MRS_MSG_PLC_CHL_STATUS, ID_MRS_MSG_PLC_CHL_STATUS, mrsMsgStaPlcChlStatusNotify},    // PLC通道连接通知
    {ID_MRS_MSG_MR_DATA, ID_MRS_MSG_MR_DATA, mrsMsgStaDeviceDataRx},    // 抄表设备数据接收
    {ID_MRS_MSG_MR_TIMER, ID_MRS_MSG_MR_TIMER, mrsMsgStaTimer}, // 定时器
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
    {ID_MRS_MSG_MR_DATA_CSG, ID_MRS_MSG_MR_DATA_CSG, mrsMsgStaDeviceDataRxCsg},
#endif
#endif
};

#endif //(defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) || defined(PRODUCT_CFG_PRODUCT_TYPE_STA))


#endif

