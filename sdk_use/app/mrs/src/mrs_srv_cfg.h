//*****************************************************************************
//
//                  ��Ȩ���� (C), 1998-2011, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mrs_srv_cfg.h
//  �� �� ��   : V1.0
//  ��    ��   : shenhankun/00130424
//  ��������   : 2011-09-26
//  ��������   : DIAG ͨ�������������ļ�
//
//  �����б�   : NA
//  �޸���ʷ   :
//  1.��    �� : 2011-09-26
//    ��    �� : shenhankun/00130424
//    �޸����� : �����ļ�
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
    {ID_MRS_CMD_DATA_TRANSMIT_AFN13_PLC, ID_MRS_CMD_TEST_TRANSMIT_PLC, mrsCmdCcoDataTransmit},  // ��������
    {ID_MRS_CMD_GET_METERLIST, ID_MRS_CMD_GET_METERLIST, mrsCmdCcoGetMeterList}, // �ѱ�����
    {ID_MRS_CMD_TEST_TRANSMIT_AFNF0_PLC, ID_MRS_CMD_TEST_TRANSMIT_AFNF0_PLC, mrsCmdCcoPlcTestTransmit}, // ��������
    {ID_MRS_CMD_EVENT_INF_PLC, ID_MRS_CMD_EVENT_INF_PLC, mrsCmdCcoEvtTransmit},
    {ID_MRS_CMD_DATA_TRANSMIT_PLC, ID_MRS_CMD_DATA_TRANSMIT_PLC, mrsCmdCcoDataTransmit},  // ����ת��
    #if defined (PRODUCT_OPTIMIZE_MATTEO)
    {ID_MRS_CMD_GET_POWEROFF_INFO, ID_MRS_CMD_GET_POWEROFF_INFO, mrsCmdCcoGetPoweroffInfo}, // ��ȡͣ����Ϣ
    #endif
    {ID_MRS_CMD_GET_DEVICE_ID_PLC, ID_MRS_CMD_GET_DEVICE_ID_PLC, mrsCmdCcoPlcGetDevId},  // ��ȡ���豸����ID
    {ID_MRS_CMD_NDM_CTRL_CCO_PLC, ID_MRS_CMD_NDM_CTRL_CCO_PLC, mrsCmdCcoNdmPlcProc}, // NDM��PLC֡������
    {ID_MRS_CMD_TF_PLC, ID_MRS_CMD_TF_PLC, mrsCmdCcoHandleTfCmd}, // ̨��ʶ���Ĵ���
#endif
#if defined(PRODUCT_CFG_PRODUCT_TYPE_NDM)
    {ID_MRS_CMD_NDM_CTRL_CCO_PLC, ID_MRS_CMD_NDM_CTRL_CCO_PLC, mrsNdmPlcProcess}, // NDM��PLC֡������
    {ID_MRS_CMD_DATA_TRANSMIT_AFN13_PLC, ID_MRS_CMD_DATA_TRANSMIT_AFN13_PLC, mrsNdmPlcProcess}, // NDM��PLC֡������
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

    {ID_MRS_CMD_DATA_TRANSMIT_AFN13_PLC, ID_MRS_CMD_TEST_TRANSMIT_PLC, mrsCmdStaDataTransmit},  // ��������
    {ID_MRS_CMD_EVENT_INF_PLC, ID_MRS_CMD_EVENT_INF_PLC, mrsCmdStaEvtInfProc},  // ��������
    {ID_MRS_CMD_GET_METERADDR_PLC, ID_MRS_CMD_GET_DEVICE_INF_PLC, mrsCmdStaSearchMeter}, // �ѱ�����
    {ID_MRS_CMD_TEST_TRANSMIT_AFNF0_PLC, ID_MRS_CMD_TEST_TRANSMIT_AFNF0_PLC, mrsCmdStaPlcTestTransmit}, // ��������
    {ID_MRS_CMD_GET_POWEROFF_INFO, ID_MRS_CMD_GET_POWEROFF_INFO, mrsCmdStaGetPoweroffInfo}, // ��ȡͣ����Ϣ
    {ID_MRS_CMD_TF_PLC, ID_MRS_CMD_TF_PLC, mrsCmdStaHandleTfCmd}, // ̨��ʶ���Ĵ���
    {ID_MRS_CMD_GET_DEVICE_ID_PLC, ID_MRS_CMD_GET_DEVICE_ID_PLC, mrsCmdStaPlcGetDevId}, // ��ȡ�豸����ID
#endif
#endif
};

HI_PRV HI_CONST MRS_FW_MSG_PROC_TBL_STRU g_astMsgsFunTable[] =
{
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    {ID_MRS_MSG_PLC_CHL_STATUS, ID_MRS_MSG_PLC_CHL_STATUS, mrsMsgCcoPlcChlStatusNotify},    // PLCͨ������֪ͨ
    {ID_MRS_MSG_MR_DATA, ID_MRS_MSG_MR_DATA, mrsMsgCcoDeviceDataRx},    // �����豸���ݽ���
    {ID_MRS_MSG_MR_TIMER, ID_MRS_MSG_MR_TIMER, mrsMsgCcoTimer}, // ��ʱ��
#endif
#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
    {ID_MRS_MSG_PLC_CHL_STATUS,     ID_MRS_MSG_PLC_CHL_STATUS,      mrsMsgStaPlcChlStatusNotify},   // PLCͨ������֪ͨ
    {ID_MRS_MSG_MR_TIMER,           ID_MRS_MSG_MR_TIMER,            mrsMsgStaTimer},                // ��ʱ��
    {ID_MRS_MSG_MR_DATA_645,        ID_MRS_MSG_MR_DATA_645,         mrsMsgStaUartFrmRx_645},        // �����豸���ݽ���(645)
    {ID_MRS_MSG_MR_DATA_698_45,     ID_MRS_MSG_MR_DATA_698_45,      mrsMsgStaUartFrmRx_698_45},     // �����豸���ݽ���(698.45)
    {ID_MRS_MSG_MR_DATA_TRANSPARENT,ID_MRS_MSG_MR_DATA_TRANSPARENT, mrsMsgStaUartFrmRx_Trans},      // �����豸���ݽ���(͸������)
#endif
#if defined(PRODUCT_CFG_PRODUCT_TYPE_NDM)
    {ID_MRS_MSG_MR_DATA, ID_MRS_MSG_MR_DATA, mrs_msg_ndm_device_data_rx},    /* �����豸���ݽ��� */
#endif
};



#endif

