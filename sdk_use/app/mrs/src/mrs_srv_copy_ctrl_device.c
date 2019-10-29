//*****************************************************************************
//
//                  ��Ȩ���� (C), 1998-2011, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mrs_srv_copy_ctrl_device.c
//  �� �� ��   : V1.0 
//  ��    ��   : liuxipeng/KF54842
//  ��������   : 2011-11-29
//  ��������   : ������Ӧ�ò����ʵ��
//               
//  �����б�   : 
//  �޸���ʷ   : 
//  1.��    �� : 2012-08-29
//    ��    �� : liuxipeng/KF54842
//    �޸����� : �����ļ� 
//
//*****************************************************************************
#include "mrs_common.h"
#include "mrs_fw_log.h"
#include "mrs_fw_tools.h"
#include "mrs_srv_copy_ctrl_device.h"
#include "mrs_fw_nv.h"
#include "hi_mdm.h"

#if defined(PRODUCT_CFG_PRODUCT_TYPE_NDM)

HI_PRV HI_U32 mrsCtrlPingRx(HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption);

// ���������մ���ת������(645)
HI_U32 mrsCcdRxTransmitCmd(HI_U16 ulId, HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption)
{
    HI_NDM_645_FRAME_S * pstData = HI_NULL;
    HI_U32 ret = HI_ERR_SUCCESS;

    MRS_NOT_USED(ulOption);
    
    if(!pCmdParam)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    if(ulId != HI_NDM_645_FRAME)
    {
        return HI_ERR_INVALID_ID;
    }

    if(usCmdParamSize != sizeof(HI_NDM_645_FRAME_S))
    {
        return HI_ERR_BAD_DATA;
    }
    
    pstData = (HI_NDM_645_FRAME_S *)pCmdParam;

    ret = MRS_SendMrData(pstData->aucFrame,pstData->usFrameSize,HI_DMS_CHL_UART_PORT_APP);

    return ret;
}

HI_U32 mrsCtrlPingRx(HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption)
{
    MRS_NOT_USED(ulOption);
    
    if(!pCmdParam || !usCmdParamSize)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1010, HI_DIAG_MT("rx DATA"), pCmdParam, usCmdParamSize);
    HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_001, HI_DIAG_MT("NDM RX len=%d"), usCmdParamSize);
    HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_002, HI_DIAG_MT("NDM RX %d %d"), *((HI_U8*)pCmdParam+usCmdParamSize-1),*((HI_U8*)pCmdParam+usCmdParamSize-2));

    return MRS_SendMrData(pCmdParam,usCmdParamSize,HI_DMS_CHL_UART_PORT_APP);

}
// ��������Ʒ��̬��ʼ��
HI_VOID mrsCopyCtrlDeviceInit(HI_VOID)
{
    HI_MDM_NV_RegisterChangedNoitfyProc(ID_NV_MRS_USER_VER, ID_NV_MRS_USER_VER, mrsNvChangeMsgNotify);
    HI_MDM_RegisterNdmPingFunc(HI_MDM_NDM_PING_ID_0,mrsCtrlPingRx);
}
#endif


