//*****************************************************************************
//
//                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : app_mrs.h
//  �� �� ��   : V1.0
//  ��    ��   : ����/00130424
//  ��������   : 2011��09��06��
//  ��������   : MRS����ӿ�
//  �����б�   : ��
//  �޸���ʷ   :
//  1.��    �� : 2011��09��06��
//    ��    �� : ����/00130424
//    �޸����� : �����ļ�
//*****************************************************************************


#ifndef __APP_MRS_H__
#define __APP_MRS_H__
#include <app_common.h>
HI_START_HEADER

#define MRS_SRV_UART_PARITY_NONE        (0)
#define MRS_SRV_UART_PARITY_ODD         (1)
#define MRS_SRV_UART_PARITY_EVEN        (2)
#define MRS_SRV_BAUD_RATE_9600          (9600)
#define MRS_SRV_BAUD_RATE_115200        (115200)

typedef HI_U32 (*MRS_FW_MSG_PROC_PFN)(HI_U16 usId, HI_IN HI_PVOID);

typedef struct
{
    HI_U16 usId;

    // ����: MAC��ַ,����ʾĿ�ĵ�ַʱ:ȡֵȫΪ��, ���ʾ���͵�CCO, ȫΪFF���ʾ�㲥,����Ϊָ���ĵ�ַ;
    // ����: ��ʾԴMAC��ַ
    HI_U8 ucMac[HI_PLC_MAC_ADDR_LEN];
    HI_U8 is_bc_frame       : 1;
    HI_U8 is_local_bc_frame : 1;
    HI_U8 is_broadcast_send : 1;    // �Ƿ�ȫ���㲥���ͣ�Ŀ�ĵ�ַΪȫFFʱ��Ч��HI_TRUEȫ���㲥��HI_FALSE����㲥
    HI_U8 reserved          : 5;
    HI_U8 ucOption;
    HI_U16 usPayloadLen; // ֡����
    HI_PBYTE pucPayload; // ����֡

    // ���ղ���
    HI_U32 bIsMyNetFrame    : 1;
    HI_U32 ucRcvFreq        : 7;
    HI_U32 ulRcvNid         : 24;
}MRS_PLC_FRAME_DATA_S, MRS_PLC_FRAME_DATA_STRU;

//#define MRS_PLC_FRAME_DATA_STRU  MRS_PLC_FRAME_DATA_S

typedef HI_U32 (*MRS_FW_CMD_REQ_PFN)(HI_U16 usId, HI_IN HI_PVOID, HI_U16);

typedef struct
{
    HI_U16 usMinId;
    HI_U16 usMaxId;
    MRS_FW_CMD_REQ_PFN pfnReq;  // ������ĺ���ָ��
} MRS_FW_CMD_PROC_TBL_STRU;


typedef struct
{
    HI_U16 usMinId;
    HI_U16 usMaxId;
    MRS_FW_MSG_PROC_PFN pfnMsg;  // ��Ϣ����ĺ���ָ��
} MRS_FW_MSG_PROC_TBL_STRU;


//*****************************************************************************
// ��������: MRS_CmdRegister
// ��������: �����ע��
//
// ����˵��:
//   pstTbl [in] �������ڱ�
//   usNum [in] �������ڸ���
//
// �� �� ֵ:
//    TODO: ...
//
// ����Ҫ��: ��CCO/STA���յ��Է��������MRS��ܻ��Զ����ø�ע��Ĵ�����ڡ�
// ���þ���: TODO: ...
// ��    ��: shenhankun/00130424 [2011-11-04]
//*****************************************************************************
HI_EXTERN HI_U32 MRS_CmdRegister(HI_CONST MRS_FW_CMD_PROC_TBL_STRU* pstTbl, HI_U16 usNum);


//*****************************************************************************
// ��������: MRS_MsgRegister
// ��������: ��Ϣ����ע��
//
// ����˵��:
//   pstTbl [in] ��Ϣ������ڱ�
//   usNum [in] ��Ϣ������ڸ���
//
// �� �� ֵ:
//    TODO: ...
//
// ����Ҫ��: ����CCO/STA�ڲ���ܺͷ���֮�����Ϣ����
// ���þ���: TODO: ...
// ��    ��: shenhankun/00130424 [2011-11-04]
//*****************************************************************************
HI_EXTERN HI_U32 MRS_MsgRegister(HI_CONST MRS_FW_MSG_PROC_TBL_STRU* pstTbl, HI_U16 usNum);


//*****************************************************************************
// ��������: MRS_SendMrData
// ��������: ���ͳ������ݸ����/������
//
// ����˵��:
//   pucData [in] MR����
//   usDataSize [in] MR���ݴ�С,��λ�ֽ�
//
// �� �� ֵ:
//    HI_ERR_SUCCESS: �ɹ�д��DMS������, ��DMSд��MR�豸��
//    ����������
//
// ����Ҫ��: ��֧���Զ����ǰ���ֽڵȹ���,���ṩͨ�����ܷ������ݵĹ���
// ���þ���: TODO: ...
// ��    ��: shenhankun/00130424 [2011-11-04]
//*****************************************************************************
HI_EXTERN HI_U32 MRS_SendMrData(HI_IN HI_PBYTE pucData, HI_U16 usDataSize, HI_U16 ucChlType);

//*****************************************************************************
// ��������: MRS_UartWrite
// ��������: ���ͳ������ݸ�����(����Ϣ����ֱ��д����)
//
// ����˵��:
//   pucData [in] MR����
//   usDataSize [in] MR���ݴ�С,��λ�ֽ�
//
// �� �� ֵ:
//    HI_ERR_SUCCESS: �ɹ�д��Uart��
//    ����������
//
// ����Ҫ��: ��֧���Զ����ǰ���ֽڵȹ���,���ṩͨ�����ܷ������ݵĹ���
// ���þ���: TODO: ...
// ��    ��: liuxipeng/lkf54842 [2014-01-15]
//*****************************************************************************
#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
HI_EXTERN HI_U32 MRS_UartWrite(HI_IN HI_PBYTE pucData, HI_U16 usDataSize, HI_U16 ucChlType);
#endif

//*****************************************************************************
// ��������: MRS_SendPlcFrame
// ��������: ����PLC֡�����ݸ�CCO/STA
//
// ����˵��:
//   pstFrameData [in] MR֡����
//
// �� �� ֵ:
//    HI_ERR_SUCCESS: �ɹ�д��ϵͳ������, �ɵײ�ģ�鸺���͵�����ͨ��ģ��.
//    ����������
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: shenhankun/00130424 [2011-11-04]
//*****************************************************************************
HI_EXTERN HI_U32 MRS_SendPlcFrame(HI_IN MRS_PLC_FRAME_DATA_S* pstFrameData);

HI_EXTERN HI_U32 MRS_SendFrameProc(HI_IN MRS_PLC_FRAME_DATA_S* pstFrameData);

//*****************************************************************************
// ��������: MRS_ChangeBaudRate
// ��������: ���Ĳ�����
//
// ����˵��:
//   ulRate ������
//
// �� �� ֵ:
//    HI_ERR_SUCCESS: �ɹ��޸Ĳ�����
//    ����������
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: shenhankun/00130424 [2012-09-22]
//*****************************************************************************
HI_EXTERN HI_U32 MRS_ChangeBaudRate(HI_U32 ulRate, HI_U8 ucParity);
HI_EXTERN HI_U32 mrsGetCurBaudRate(HI_VOID);

HI_EXTERN HI_U8 mrsGetPlcChlStatus(HI_VOID);
HI_EXTERN HI_U8 mrsGetTopoStatus(HI_VOID);
HI_EXTERN HI_VOID mrsSetTopoStatus(HI_U8);

HI_EXTERN HI_VOID mrsFreePlcFrameDataStru(MRS_PLC_FRAME_DATA_STRU *pstPlcFrameDataStru);

HI_END_HEADER
#endif // __APP_MRS_H__

