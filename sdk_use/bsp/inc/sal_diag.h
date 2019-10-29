//*****************************************************************************
//
//                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : msp_diag.h
//  �� �� ��   : V1.0 
//  ��    ��   : ����/00130424
//  ��������   : 2008��07��07��
//  ��������   : DIAG use to implement the Protocol logging and the system 
//               testing for HiStudio at PC side. This file define all exported 
//               interfaces for other modules to use DIAG.
// 
//  �����б�   : DIAG_Printf
//               HI_DIAG_PrintfV
//               DIAG_PutTextA
//               DIAG_Dump
//               HI_DIAG_ReportLayerMessage
//               DIAG_ReportAirMessageLog
//               DIAG_IsAirMessageLogAvailable
//               DIAG_ReportEventLog
//               DIAG_ReportEventLogEx
//  �޸���ʷ   : 
//  1.��    �� : 2008��07��07��
//    ��    �� : ����/00130424
//    �޸����� : �����ļ�����������ĺ����ӿڡ�
//  2.��    �� : 2009��02��23��
//    ��    �� : ���ɸ�/00149569
//    �޸����� : 1> ����ע��Ϊͳһ��ʽ
//               2> �޸�DIAG_ReportEventLog֧��һ��������
//                  ���DIAG_ReportEventLogEx֧�ֶ���������¼���־��
//  3.��    �� : 2009��04��08��
//    ��    �� : ���ɸ�/00149569
//    �޸����� : 1> �տ���Ϣ�Ͳ����Ϣ�ṹ����ulSideId��ulDestMod���ֶΣ�
//                  �ֱ��ʾ��������UE���Ŀ��ģ��
//               2> ���DIAG_SIDE_UE��DIAG_SIDE_NET�����궨���ʾulSideId��ֵ
//               3> �����ϱ���־�ӿ����ERR_SAL_CFG_LOG_NOT_ALLOW�����룬
//                  ��ʾ���ò������ϱ�
//  4.��    �� : 2009��04��30��
//    ��    �� : ���ɸ�/00149569
//    �޸����� : 1> DIAG_TRACEV ϵ�к궨����� DIAG_TRACEV4
//               2> ���DIAG_SIDE_UE��DIAG_SIDE_NET�����궨���ʾulSideId��ֵ
//               3> �����ϱ���־�ӿ����ERR_SAL_CFG_LOG_NOT_ALLOW�����룬
//                  ��ʾ���ò������ϱ�
//  5.��    �� : 2009��05��04��
//    ��    �� : ���ɸ�/00149569
//    �޸����� : �޸Ĳ��������ڴ����ʧ�ܵķ���ֵ
//  6.��    �� : 2009��09��27��
//    ��    �� : ���ɸ�/00149569
//    �޸����� : �޸Ĳ���δʵ�ֽӿ�ע�ͣ���עδʵ��
//  7.��    �� : 2011��09��10��
//    ��    �� : ����/00130424
//    �޸����� : �������ֽӿڶ��壬�����䵽Hi3911��Ŀ�� 
//*****************************************************************************

#ifndef __SAL_DIAG_H__
#define __SAL_DIAG_H__
#include <hi_types.h>
#include "dfx_sal.h"
HI_START_HEADER
//****************************************************************************

//*****************************************************************************
#define DIAG_CTRL_DEFAULT            0x0
#define DIAG_CTRL_MULTI_UNIT_PACKET  0xFFFF 
#define DIAG_CTRL_AUTO_ACK_PACKET    0xFFFE 
#define DIAG_CTRL_MAX_NUM            0xFFF0
//*****************************************************************************


//*****************************************************************************
// DIAG ��������Ϣ
typedef struct
{
    HI_U32 ulCmdId;
    HI_U32 ulAuId;
    HI_U32 ulPacketSn;
    HI_U32 ulReqIdx; // ���������ţ���������Ƿ�֡.
    HI_U16 usPortNum;
    HI_U16 usSn; // �������к�
    HI_U16 usCtrl; // ��������֣����ڿ�����������ȼ�
    HI_U16 usPayloadSize;  // ��������ĳ���
    HI_PBYTE pucPayload; // ������������� 
} DIAG_PACKET_REQ_STRU;

typedef struct
{
    HI_U8 ucBufferSize;
    HI_U8 reservedd[3];
    HI_U8 aucBuffer[64];
} DIAG_PACKET_ACK_CACHE_STRU;

#define DIAG_MSG_P3_MASK(x)         (x)&0xFFFF00FF
#if defined(PRODUCT_CFG_DIAG_IND_SAVE_FLASH)
#define DIAG_MSG_P3_ADJUST(x)       (x) = DIAG_MSG_P3_MASK(x)
#else
#define DIAG_MSG_P3_ADJUST(x)       (x) = (x)
#endif

typedef enum
{ 
    EN_DIAG_PORT_TYPE_RP = 0, // report to remote device via device port(like eth/uart.)
    EN_DIAG_PORT_TYPE_LF = 1, // report to local flash device.
} DIAG_RPT_PORT_TYPE_E;

// BUFFER ID �������С�� 0xFFFF
typedef enum
{ 
    EN_DIAG_BUFFER_ID_PRINT = 0,   // ��ӡ���� 
    EN_DIAG_BUFER_ID_MSG,      // ��Ϣ���� 
    EN_DIAG_BUFER_ID_CMD,      // ����� 
    EN_DIAG_BUFER_ID_EVT,      // EVENT���� 
    EN_DIAG_BUFER_ID_DATA,     // �û������ݻ��� 
    EN_DIAG_BUFER_ID_LOCAL,    // �������ݣ������浽flash�����С  
    EN_DIAG_BUFER_ID_PROXY,    // ת���ն˵Ļ���, ����CCO��Ч
    EN_DIAG_BUFFER_ID_MAX = 7,
    EN_DIAG_BUFFER_ID_ACK     = 0xF000,
    EN_DIAG_BUFFER_ID_INVALID = 0xFFFF
} DIAG_BUFFER_ID_ENUM;

// Check the pre-defined value.
#if !defined(HI_HAVE_NATIVE_COMPILER_VC)
#define EN_DIAG_BUFFER_ID_MAX 7
#if (EN_DIAG_BUFFER_ID_MAX > SAL_DIAG_SDM_CAT_MAX)
#error "EN_DIAG_BUFFER_ID_MAX > SAL_DIAG_SDM_CAT_MAX"
#endif
#endif

#define DIAG_INIT_EVT_NO_MASK   0x10000
#define DIAG_ERROR_EVT_NO_MASK  0x20000
//*****************************************************************************

//*****************************************************************************
HI_BOOL diag_IsInit(HI_VOID);
HI_BOOL diag_IsLogicalChlConnect(HI_VOID); // �߼�ͨ���Ƿ����� 
HI_U32  diag_GetBufferingAvalibleBlocks(DIAG_BUFFER_ID_ENUM enBufId);
HI_BOOL diag_CheckQueueFull(HI_U32 ulQueueId);
HI_BOOL diag_GetDebugMode(HI_VOID);
HI_BOOL diag_SetDebugMode(HI_BOOL bDebugMode);

HI_U16 diag_GetDevId(HI_VOID);
HI_U8* diag_GetLocalMac(HI_VOID);
HI_U8* diag_GetCcoMac(HI_VOID);
HI_U32 diag_RefreshSysInitEvent(HI_BOOL bForce);
HI_U32 diag_ReportSysCriticalEvt(HI_U32 ulEvtId, HI_U32 ulError);
HI_U32 diag_ReportAllSysInitEvent(HI_U8 ucOption);
HI_BOOL diag_IsUserCmdBusyStatus(HI_VOID) ;
HI_VOID diag_SetUserCmdBusyStatus(HI_BOOL bBusy) ;
HI_VOID HI_DIAG_LogInd(HI_U32 ulId, HI_U8 ucInstanceId, HI_PBYTE pucPacket, HI_U16 usPacketSize, HI_BOOL bSync);
HI_VOID HI_DIAG_LogCmd (HI_U16 ulId, HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption);

//*****************************************************************************

//*****************************************************************************
// ��������: diag_CreateDataPacket
// ��������: ��ȡ���ݰ�Buffer
//           
// ����˵��:
//   enBufId [in] ���ݰ�ID
//   ulId [in] ��ID
//   usPacketSize [in] ����С 
//   bDynMem [in] �Ƿ�ʹ�ö�̬���䷽ʽ, TRUE_T��ʾʹ�ö�̬���䷽ʽ, FALSE_T��ʾʹ�þ�̬������Buffer��ʽ.
//
// �� �� ֵ: 
//    �ǿձ�ʾBuffer��ַ
//    �ձ�ʾ�޷���ȡ��Buffer.
//    
// ��    ��: shenhankun/00130424 [2012-01-04]
//*****************************************************************************

//*****************************************************************************
// ��������: diag_SendDataPacket
// ��������: �첽�������ݰ���HSO(ͨ��OS������)
//           
// ����˵��:
//   enBufId [in] buffer ID
//   ulId [in] ��ID
//   pucPacket [in] ���ݰ���buffer��ַ, ͨ�� diag_CreateDataPacket ��ȡ 
//   usPacketSize [in] ���ݰ���С
//   ulTimeOut [in] ��ʾ���ȴ�, ������ֵ��ʾ�ȴ���ʱʱ��, ��λ����.
//
// �� �� ֵ: 
//    HI_ERR_SUCCESS: �ɹ�
//    ����ֵ��ʾ������.
//
// ����Ҫ��: pucPacketָ�����ͨ�� diagCreateDataPacket��ȡ
// ���þ���: 
//  HI_U16 usPacketSize = 100
//  HI_PBYTE pucPacket = diag_CreateDataPacket(EN_DIAG_BUFFER_ID_PRINT, DIAG_CMD_LOG_PRINT_RAW_TXT_IND, (HI_U16) usPacketSize,HI_FALSE)
//  diag_SendDataPacket(EN_DIAG_BUFFER_ID_PRINT, pucPacket, usPacketSize, 0)
// ��    ��: shenhankun/00130424 [2012-01-04]
//*****************************************************************************
HI_U32 diag_SendDataPacket(DIAG_BUFFER_ID_ENUM enBufId, HI_U32 ulId, HI_PBYTE pucPacket, HI_U16 usPacketSize, HI_U32 ulTimeOut);

#if defined(PRODUCT_CFG_FEATURE_REG_PARALLEL_CMD)
HI_DIAG_CMD_F diag_FindUserInputCmd(HI_U8 ucInstanceId, HI_U32 ulId, HI_DIAG_CMD_PARALLEL_CTRL_F* pfnCmdCtrl);
#else
HI_DIAG_CMD_F diag_FindUserInputCmd(HI_U8 ucInstanceId, HI_U32 ulId);
#endif
//*****************************************************************************


//*****************************************************************************
// 0-0x50 ΪPLC�ڲ�ͨѶʹ�õ�ID��
#define ID_DIAG_CMD_PLC_CHL_CONNECT        0x01

// DIAG���ṹ����
typedef struct
{
    HI_U8 ucMacAddress[HI_PLC_MAC_ADDR_LEN]; // CCO��MAC��ַ
}DIAG_CMD_PLC_CHL_CONNECT_STRU;

#define DIAG_EVT_ACK_COMPLETE          (1<<0)

typedef struct  
{
    HI_U32 ulCmdId;
    HI_U16 usPortNum;
    HI_U16 usBufferSize;
    HI_PVOID pBuffer;
    HI_PVOID pAckInstance;
    HI_U32 ulTimeOut;
    HI_PVOID pCache;
    HI_U32 ulCacheSize;
}DIAG_SEND_PACKET_INFO_STRU;
//*****************************************************************************

//*****************************************************************************
HI_U32 diag_Init(HI_VOID);

//
// usCtrl ȡֵΪ: DIAG_CTRL_DEFAULT ���� DIAG_CTRL_AUTO_ACK_PACKET 
// bMgrAckPacket : HI_TRUE: Ϊͨ�����������ACK
//
HI_U32 diag_SendAckPacket(HI_IN DIAG_SEND_PACKET_INFO_STRU* pstSndPacket, HI_U16 usCtrl, HI_BOOL bMgrAckPacket, HI_BOOL bSync);

HI_U32 diag_UserReqProc(HI_IN HI_PVOID pstReqInfo, HI_DIAG_CMD_F pfnInputCmd, HI_IN HI_PVOID pMsg, HI_U32 ulOption);
HI_U32 diag_GetPlcSdmMac(HI_OUT HI_U8 aucSdmMac[HI_PLC_MAC_ADDR_LEN]);
HI_U32 diag_SetPlcSdmMac(HI_IN HI_U8 aucSdmMac[HI_PLC_MAC_ADDR_LEN], HI_BOOL bEnable);
HI_U32 diag_SimuDiagMsg(HI_U32 ulId, HI_PVOID pPayloadBuffer, HI_U16 usPayloadBufferSize);
HI_BOOL diag_IsPlcSdm(HI_VOID);

HI_U32 diag_LoadFileNoitfyDispatch(HI_U32, HI_U32 ulId, HI_PVOID pFile, HI_U32 ulFileSize, HI_U32 ulErrCode);
HI_U32 diag_GetLoadFileOpt(HI_U32 ulId, HI_U32 ulFileSize, HI_PVOID* pFileBuffer, HI_U16* pusLdOpt);
#if defined(PRODUCT_CFG_SYS_QRY_STAT)
HI_PVOID dfxGetSysInfo(HI_U32* pulSysInfoSize);
#endif

#if defined(PRODUCT_CFG_SYS_QRY_STAT) && defined(PRODUCT_CFG_SYS_QRY_HAVE_OS_TASK_INFO)
HI_PVOID dfxGetSysOsInfo(HI_U32* pulSysInfoOsSize, HI_U32* pulCnt);
#endif

//*****************************************************************************
#if defined(PRODUCT_CFG_SDM_DIAG_CHL_SUPPORT_CALL_STAT) && !defined(PRODUCT_CFG_CORE_TYPE_BOOTLOADER)
#define SET_DIAG_API_CALL_CNT(enBufId)  \
    do { SAL_SET_SAL_DIAG_SDM_U32_INC(aulCallApiCnt[enBufId]); } while(0)

#define SET_DIAG_API_CALL_FAIL_CNT_RC(enBufId,ret) \
    do { if(ret != HI_ERR_SUCCESS) { SAL_SET_SAL_DIAG_SDM_U32_FAIL_INC(aulCallApiFailCnt[enBufId],aulCallApiFailRc[enBufId], ret); } else {SAL_SET_SAL_DIAG_SDM_U32_INC(aulCallApiOkCnt[enBufId]);} } while(0) 

#define SET_DIAG_API_CALL_CNT_IND() \
    do { SAL_SET_SAL_DIAG_SDM_U32_INC(aulIndCallApiAOER[0]); } while(0) 

#define SET_DIAG_API_CALL_CNT_NDMPK() \
    do { SAL_SET_SAL_DIAG_SDM_U32_INC(aulNdmPkCallApiAOER[0]); } while(0) 

#define SET_DIAG_API_CALL_FAIL_CNT_RC_IND(ret) \
    do { if(ret != HI_ERR_SUCCESS) { SAL_SET_SAL_DIAG_SDM_U32_FAIL_INC(aulIndCallApiAOER[2],aulIndCallApiAOER[3], ret); } else {SAL_SET_SAL_DIAG_SDM_U32_INC(aulIndCallApiAOER[1]); } } while(0) 

#define SET_DIAG_API_CALL_CNT_RPT_SYS_MSG() \
    do { SAL_SET_SAL_DIAG_SDM_U32_INC(aulSysMsgRptCallApiAOER[0]);} while(0) 

#define SET_DIAG_API_CALL_FAIL_CNT_RC_RPT_SYS_MSG(ret) \
    do { if(ret != HI_ERR_SUCCESS) { SAL_SET_SAL_DIAG_SDM_U32_FAIL_INC(aulSysMsgRptCallApiAOER[1],aulSysMsgRptCallApiAOER[2], ret); } else {SAL_SET_SAL_DIAG_SDM_U32_INC(aulSysMsgRptCallApiAOER[1]); }} while(0) 

#define SET_DIAG_API_CALL_CNT_RPT_SYS_EVT() \
    do { SAL_SET_SAL_DIAG_SDM_U32_INC(aulSysEvtRptCallApiAOER[0]);} while(0) 

#define SET_DIAG_API_CALL_FAIL_CNT_RC_RPT_SYS_EVT(ret) \
    do { if(ret != HI_ERR_SUCCESS) { SAL_SET_SAL_DIAG_SDM_U32_FAIL_INC(aulSysEvtRptCallApiAOER[1],aulSysEvtRptCallApiAOER[2], ret); } else {SAL_SET_SAL_DIAG_SDM_U32_INC(aulSysEvtRptCallApiAOER[1]); } } while(0) 

#define SET_DIAG_API_CALL_CNT_RPT_SYS_STAT() \
    do { SAL_SET_SAL_DIAG_SDM_U32_INC(aulSysStatRptCallApiAOER[0]);} while(0) 

#define SET_DIAG_API_CALL_FAIL_CNT_RC_RPT_SYS_STAT(ret) \
    do { if(ret != HI_ERR_SUCCESS) { SAL_SET_SAL_DIAG_SDM_U32_FAIL_INC(aulSysStatRptCallApiAOER[1],aulSysStatRptCallApiAOER[2], ret); } else {SAL_SET_SAL_DIAG_SDM_U32_INC(aulSysStatRptCallApiAOER[1]); } } while(0) 

#define SET_DIAG_API_CALL_FAIL_CNT_RC_NDMPK_RPT_STAT(ret) \
    do { if(ret != HI_ERR_SUCCESS) { SAL_SET_SAL_DIAG_SDM_U32_FAIL_INC(aulNdmPkCallApiAOER[1],aulNdmPkCallApiAOER[2], ret); } else {SAL_SET_SAL_DIAG_SDM_U32_INC(aulNdmPkCallApiAOER[1]); } } while(0) 
#else
#define SET_DIAG_API_CALL_CNT(enBufId)                    do { } while(0)
#define SET_DIAG_API_CALL_FAIL_CNT_RC(enBufId,ret)        do { } while(0)
#define SET_DIAG_API_CALL_CNT_IND()                       do { } while(0)
#define SET_DIAG_API_CALL_CNT_NDMPK()                     do { } while(0)
#define SET_DIAG_API_CALL_FAIL_CNT_RC_IND(ret)            do { } while(0)
#define SET_DIAG_API_CALL_CNT_RPT_SYS_MSG()               do { } while(0)
#define SET_DIAG_API_CALL_FAIL_CNT_RC_RPT_SYS_MSG(ret)    do { } while(0)
#define SET_DIAG_API_CALL_CNT_RPT_SYS_EVT()               do { } while(0)
#define SET_DIAG_API_CALL_FAIL_CNT_RC_RPT_SYS_EVT(ret)    do { } while(0)
#define SET_DIAG_API_CALL_CNT_RPT_SYS_STAT()              do { } while(0)
#define SET_DIAG_API_CALL_FAIL_CNT_RC_RPT_SYS_STAT(ret)   do { } while(0)
#define SET_DIAG_API_CALL_FAIL_CNT_RC_NDMPK_RPT_STAT(ret) do { } while(0)
#endif
//*****************************************************************************

//*****************************************************************************
#if defined(PRODUCT_CFG_DFX_DIAG_CHL_RPT)
#define SEND_DEBUG_DIAG_PK(id, data, len) HI_DIAG_ReportPacket(id, HI_DIAG_CMD_INSTANCE_DEFAULT, (HI_PBYTE)data, (HI_U16)len, HI_TRUE)
#else
#define SEND_DEBUG_DIAG_PK(id, data, len)  
#endif

// only apply to debug mode.
#define DIAG_DBG_OBJ_PK(id, data, size) do {\
            if(diag_GetDebugMode() == HI_TRUE)\
            {\
                SEND_DEBUG_DIAG_PK((HI_U16)id, (HI_PVOID)(data), (HI_U16)(size));\
            }\
}while(0)
//*****************************************************************************

//*****************************************************************************

//*****************************************************************************
HI_U32 diag_SysStatObjReportPeriod2Queue(HI_BOOL bCheckCfg);
//*****************************************************************************

HI_U32 diag_GetVerInfo(HI_INOUT DIAG_CMD_SOFT_VER_STRU* pstAck);
//��ȡ����ṹ��syserr
HI_U32 diag_get_NewSysErrInfo(HI_INOUT DIAG_CMD_NEW_SYSERR_QRY_STU* pstAck);
//��ȡ����ṹ��ver
HI_U32 diag_get_NewVerInfo(HI_INOUT DIAG_CMD_SOFT_NEW_VER_STRU* pstAck);


//*****************************************************************************
HI_END_HEADER
#endif // __SAL_DIAG_H__

