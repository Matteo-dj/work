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
    {ID_MRS_CMD_DATA_TRANSMIT_AFN13_PLC, ID_MRS_CMD_TEST_TRANSMIT_PLC, mrsCmdCcoDataTransmit},  // ��������
    {ID_MRS_CMD_GET_METERLIST, ID_MRS_CMD_GET_DEVICE_INF_PLC, mrsCmdCcoSearchMeter}, // �ѱ�����
    {ID_MRS_CMD_TEST_TRANSMIT_AFNF0_PLC, ID_MRS_CMD_TEST_TRANSMIT_AFNF0_PLC, mrsCmdCcoPlcTestTransmit}, // ��������
    {ID_MRS_CMD_EVENT_INF_PLC, ID_MRS_CMD_EVENT_INF_PLC, mrsCmdCcoEvtTransmit},
#else
    {ID_MRS_CMD_CSG_DATA_TRANSMIT_PLC, ID_MRS_CMD_CSG_DATA_TRANSMIT_PLC, mrsCmdCcoCsgDataTransmit},
#endif
    {ID_MRS_CMD_DATA_TRANSMIT_PLC, ID_MRS_CMD_DATA_TRANSMIT_PLC, mrsCmdCcoDataTransmit},  // ����ת��
#endif
#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
    {ID_MRS_CMD_DATA_TRANSMIT_AFN13_PLC, ID_MRS_CMD_TEST_TRANSMIT_PLC, mrsCmdStaDataTransmit},  // ��������
    {ID_MRS_CMD_EVENT_INF_PLC, ID_MRS_CMD_EVENT_INF_PLC, mrsCmdStaEvtInfProc},  // ��������
    {ID_MRS_CMD_GET_METERADDR_PLC, ID_MRS_CMD_GET_DEVICE_INF_PLC, mrsCmdStaSearchMeter}, // �ѱ�����
    {ID_MRS_CMD_TEST_TRANSMIT_AFNF0_PLC, ID_MRS_CMD_TEST_TRANSMIT_AFNF0_PLC, mrsCmdStaPlcTestTransmit}, // ��������
#else
    {ID_MRS_CMD_CSG_DATA_TRANSMIT_PLC, ID_MRS_CMD_CSG_DATA_TRANSMIT_PLC, mrsCmdStaCsgDataTransmit},
#endif
#endif
};

HI_PRV HI_CONST MRS_FW_MSG_PROC_TBL_STRU g_astMsgsFunTable[] =
{
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    {ID_MRS_MSG_PLC_CHL_STATUS, ID_MRS_MSG_PLC_CHL_STATUS, mrsMsgCcoPlcChlStatusNotify},    // PLCͨ������֪ͨ
    {ID_MRS_MSG_MR_TIMER, ID_MRS_MSG_MR_TIMER, mrsMsgCcoTimer}, // ��ʱ��
#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
    {ID_MRS_MSG_MR_DATA, ID_MRS_MSG_MR_DATA, mrsMsgCcoDeviceDataRx},    // �����豸���ݽ���
#else
    {ID_MRS_MSG_MR_DATA_CSG, ID_MRS_MSG_MR_DATA_CSG, mrsMsgCcoDeviceDataRxCsg},
#endif
#endif
#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
    {ID_MRS_MSG_PLC_CHL_STATUS, ID_MRS_MSG_PLC_CHL_STATUS, mrsMsgStaPlcChlStatusNotify},    // PLCͨ������֪ͨ
    {ID_MRS_MSG_MR_DATA, ID_MRS_MSG_MR_DATA, mrsMsgStaDeviceDataRx},    // �����豸���ݽ���
    {ID_MRS_MSG_MR_TIMER, ID_MRS_MSG_MR_TIMER, mrsMsgStaTimer}, // ��ʱ��
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
    {ID_MRS_MSG_MR_DATA_CSG, ID_MRS_MSG_MR_DATA_CSG, mrsMsgStaDeviceDataRxCsg},
#endif
#endif
};

#endif //(defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) || defined(PRODUCT_CFG_PRODUCT_TYPE_STA))


#endif

