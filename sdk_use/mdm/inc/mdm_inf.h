//****************************************************************************
//
//                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾
//
//****************************************************************************
//  �� �� ��   : mdm_inf.h
//  �� �� ��   : V1.0
//  ��    ��   : ����/00130424
//  ��������   : 2011��10��10��
//  ��������   : modem�����õ�˽�нӿ�,
//
//  �����б�   :
//  �޸���ʷ   :
//  1.��    �� : 2011��10��10��
//    ��    �� : ����/00130424
//    �޸����� : �����ļ�
//
//****************************************************************************

#ifndef _MDM_INF_H__
#define _MDM_INF_H__

#if !defined(PRODUCT_CFG_CORE_TYPE_BOOTLOADER) || defined(PRODUCT_CFG_HSO)
#include <hi_mdm.h>
#endif
//****************************************************************************

/* BEGIN: PN: DTS2014012607684 MODIFY\ADD\DEL by cuiate/00233580 at 2014/2/17 */
HI_CPP_START
/* END:   PN: DTS2014012607684 MODIFY\ADD\DEL by cuiate/00233580 at 2014/2/17 */

//*****************************************************************************
#define HI_SYS_IO_MAC_RX_LED_ON                     (1<<0) // 20ms
#define HI_SYS_IO_MAC_TX_LED_ON                     (1<<1) // 20ms

#define HI_SYS_IO_APP_RX_LED_ON                     (1<<2) // 100ms
#define HI_SYS_IO_APP_TX_LED_ON                     (1<<3) // 100ms

#define HI_SYS_IO_APP_TX_PHASE_A_LED_ON             (1<<4) // 100ms
#define HI_SYS_IO_APP_TX_PHASE_B_LED_ON             (1<<5) // 100ms
#define HI_SYS_IO_APP_TX_PHASE_C_LED_ON             (1<<6) // 100ms

#define HI_SYS_IO_LED_ALL_OFF                       (1<<7)

#define HI_SYS_IO_SDM_ENG_TEST_FRAME_ON             (1<<8)
#define HI_SYS_IO_SDM_ENG_TEST_FRAME_OFF            (1<<9)
#define HI_SYS_IO_SDM_ENG_TEST_FRAME_TIMEOUT        (1<<10)

#define HI_SYS_IO_EVT_MAX                           (1<<10)

#define HI_SYS_IO_MAC_RX_LED_ON_TIME                 20 // 20ms
#define HI_SYS_IO_MAC_TX_LED_ON_TIME                 20 // 20ms

#define HI_SYS_IO_APP_RX_LED_ON_TIME                 100 // 100ms
#define HI_SYS_IO_APP_TX_LED_ON_TIME                 100 // 100ms

#define HI_SYS_IO_APP_TX_PHASE_A_LED_ON_TIME         100 // 100ms
#define HI_SYS_IO_APP_TX_PHASE_B_LED_ON_TIME         100 // 100ms
#define HI_SYS_IO_APP_TX_PHASE_C_LED_ON_TIME         100 // 100ms
//*****************************************************************************


HI_EAPI HI_VOID HI_SYS_RstResume(HI_SYS_RST_RESUME_CAUSE_E enCause);
//****************************************************************************


//*****************************************************************************
// UPG Service
// �ο� <<Hi3911V100 B017��������������.docx>> �еĽӿ�˵��
//*****************************************************************************
#if defined(PRODUCT_CFG_SUPPORT_UPG)
//���¶����ȡ�汾�Ľӿ�
typedef enum tagHI_DRV_OBJ_E
{
    HI_DRV_OBJ_NULL,

    HI_DRV_OBJ_CODE,
    HI_DRV_OBJ_NV, //���ܷ�����������ɾ���������������
    HI_DRV_OBJ_BOOT,

    // ��չ��������
    // TODO: CCO�ϴ洢��STA������FLASH����
    HI_DRV_OBJ_UPGFILE_BACKUP,

    // �豸�����ļ�
    HI_DRV_OBJ_DEVICE,

    // �������������ļ�
    HI_DRV_OBJ_OTHER,

    /* ��������-XоƬ�����ļ� */
    HI_DRV_OBJ_OTHER_X,

    /* ��������-3911оƬ�����ļ� */
    HI_DRV_OBJ_OTHER_3911,

    HI_DRV_OBJ_UNKNOWN
} HI_DRV_OBJ_E;

HI_EAPI HI_U32 HI_DRV_VersionUpdateStart(HI_IN HI_UPG_START_S* pstStart);
HI_EAPI HI_U32 HI_DRV_VersionUpdateStop(HI_VOID);
HI_EAPI HI_U32 HI_DRV_GetStartupStatus(HI_OUT HI_U32 *pstStatus);
HI_EAPI HI_U32 HI_DRV_VersionUpdateFinish(HI_U32 ulOption);
HI_EAPI HI_U32 HI_DRV_UpgGetFile(HI_OUT HI_U32* pulUpgFileAddr, HI_OUT HI_U32* pulUpgFileSize,HI_OUT HI_UPG_FILE_HEADER_S* pstFile); // CCO Only
HI_EAPI HI_U32 HI_DRV_GetFileVer(HI_IN HI_U8* pstFileBlk, HI_OUT HI_UPG_FILE_VER_S* pstCodeVer, HI_DRV_OBJ_E bRunUpgFile);
HI_EAPI HI_U32 HI_DRV_GetVersion(HI_IN HI_DRV_OBJ_E obj, HI_OUT HI_UPG_FILE_VER_S* pstCodeVer);
HI_EAPI HI_U32 HI_DRV_VersionCompare(HI_IN HI_DRV_OBJ_E obj, HI_IN HI_UPG_FILE_VER_S *old_ver, HI_IN HI_UPG_FILE_VER_S *new_ver, HI_OUT HI_S32 *result);
HI_EAPI hi_u32 HI_DRV_UpgGetMaxCacheSize(hi_void);


//*****************************************************************************
// ��������: HI_DRV_VerCmp
// ��������:  �����ļ��汾�Ƚϡ�
//
// ����˵��:
//   obj       [in]  HI_DRV_OBJ_E       : ����(code,nv,boot)
//   pstOldVer [in]  HI_UPG_FILE_VER_S  : �ɵ��ļ��汾��
//   pstNewVer [in]  HI_UPG_FILE_VER_S  : �µ��ļ��汾��
//   result    [out]
//              <0 : pstOldVer < pstNewVer
//              =0 : pstOldVer = pstNewVer
//              >0 : pstOldVer > pstNewVer
// �� �� ֵ:
//  HI_ERR_FAILURE : �ð汾������Ҫ��
//  HI_ERR_SUCCESS : �Ƚϳɹ�
//  HI_ERR_INVALID_PARAMETER : ��������
//*****************************************************************************
HI_EAPI HI_U32 HI_DRV_VerCmp(HI_IN HI_DRV_OBJ_E obj, HI_IN HI_UPG_FILE_VER_S * pstOldVer,HI_IN HI_UPG_FILE_VER_S * pstNewVer, HI_OUT HI_S32 *result);
HI_EAPI HI_U32 HI_DRV_VersionGetFileAddr (HI_OUT HI_U32* pulUpgFileAddr, HI_OUT HI_U32* pulUpgFileSize, HI_BOOL bRunAddr);
HI_EAPI HI_U32 HI_DRV_SoftInitComplete(HI_U32 ulOption);
HI_EAPI HI_U32 HI_DRV_UpgFileCheckCrc(HI_U8* buffer, HI_U32 len);
HI_EAPI HI_U32 HI_DRV_UpgFileGetInfo(HI_U8* buffer, HI_U32 len, HI_DRV_OBJ_E * obj,  HI_UPG_FILE_VER_S* pstCodeVer);
HI_EAPI HI_U32 HI_DRV_UpgCacheClear(HI_VOID);
HI_EAPI HI_U32 HI_DRV_UpgRunFileLength(HI_U32 *pulLength);
HI_EAPI HI_U32 HI_DRV_UpgRunFileCrc32(HI_U32 *pulCrc);
HI_EAPI HI_U32 HI_DRV_UpgCacheRead(HI_U32 ulOffset, HI_U8 * pBuf, HI_U32 ulBufLen);
HI_EAPI HI_U32 HI_DRV_UpgCacheWrite(HI_U32 ulOffset, HI_U8 * pBuf, HI_U32 ulBufLen);
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
HI_PUBLIC HI_U32 HI_DRV_UpgCcoCacheFlashAddr(HI_VOID);
HI_PUBLIC HI_U32 HI_DRV_UpgCcoCacheCrc(HI_U32 ulFileLength, HI_U32 *pulCrc);
HI_PUBLIC hi_u32 HI_DRV_UpgFileCheckType(hi_u8* buffer, hi_u32 len, hi_u8 chip_mode);
#endif

//*****************************************************************************
// ��������: HI_DRV_UpgFlashWrite
// ��������: �������ļ�����д��FLASH
// ����˵��:
//      obj                     [IN]        �����ļ�����(code,nv,boot)
//      pInputBuffer            [IN]        Ҫд�������ָ��
//      ulInputBufferLen        [IN]        Ҫд������ݳ���
// �� �� ֵ:
//      HI_ERR_SUCCESS  д��ɹ�
//      ����ֵ��ʾʧ��
// ����Ҫ��:
// ���þ���:
//*****************************************************************************
HI_EAPI HI_U32 HI_DRV_UpgFlashWrite(HI_IN HI_DRV_OBJ_E obj, HI_IN HI_U8 * pInputBuffer, HI_IN HI_U32 ulInputBufferLen);

//*****************************************************************************
// ��������: HI_DRV_UpgGetObjectByNv
// ��������: ����NV��(0xF05)����ȡ�����Ķ���
// ����˵��:
//      obj                     [OUT]       �����ļ�����(code,nv,boot)
// �� �� ֵ:
//      HI_ERR_SUCCESS  ��ȡ�ɹ�
//      ����ֵ��ʾʧ��
// ����Ҫ��:
// ���þ���:
//*****************************************************************************
HI_EAPI HI_U32 HI_DRV_UpgGetObjectByNv(HI_OUT HI_DRV_OBJ_E * obj);

//*****************************************************************************
// ��������: HI_DRV_DecompressUpgFile
// ��������: ��ѹ������
// ����˵��:
//      obj                     [IN]        �����ļ�����(code,nv,boot)
//      pInputBuffer            [IN]        ������������ָ��
//      ulInputBufferLen        [IN]        �����������ݳ���
//      pOutputBuffer           [INOUT]     ��ѹ�����ݴ�ŵ�Bufferָ�룬�ɵ����߷����ڴ棬�ú�����д��ѹ�������
//      ulOutputBufferLen       [IN]        ��ѹ�����ݴ�ŵ�Buffer����
//      pulDecompressedDataLen  [INOUT]     ��ѹ�����ݵ�ʵ�ʳ���
// �� �� ֵ:
//      HI_ERR_SUCCESS  ��ѹ�ɹ�
//      ����ֵ��ʾʧ��
// ����Ҫ��:
// ���þ���:
//*****************************************************************************
HI_EAPI HI_U32 HI_DRV_DecompressUpgFile(HI_IN    HI_DRV_OBJ_E obj,
                                        HI_IN    HI_U8  *pInputBuffer,
                                        HI_IN    HI_U32 ulInputBufferLen,
                                        HI_INOUT HI_U8  *pOutputBuffer,
                                        HI_IN    HI_U32 ulOutputBufferLen,
                                        HI_INOUT HI_U32 *pulDecompressedDataLen);
//*****************************************************************************
// ��������: hi_Find_Section_Num
// ��������: ����ulId�ҵ����Ҷ�Ӧ��code section���
// ����˵��:
//      pHead [IN]  ������ָ��
//      ulId [IN]  section����ID
//      pulSecNum [OUT]  ���ض�Ӧ��code section���
// �� �� ֵ:
//      HI_ERR_SUCCESS  ��ѹ�ɹ�
//      ����ֵ��ʾʧ��
// ����Ҫ��:
// ���þ���:
//*****************************************************************************
HI_U32 hi_Find_Section_Num(HI_IN HI_UPG_FILE_HEADER_S *pHead, HI_IN HI_U32 ulId,HI_OUT HI_U32 *pulSecNum);

//*****************************************************************************
// ��������: HI_LZMA_GetDecompressSize
// ��������: ��ȡѹ��ѹ���ļ���С
// ����˵��:
//      pInputBuffer            [IN]        ����ѹ���ݵ�Bufferָ��
//      ulInputBufferLen        [IN]        ����ѹ���ݵĳ���
//      pOutputBuffer           [INOUT]     ��ѹ�����ݴ�ŵ�Bufferָ�룬�ɵ����߷����ڴ棬�ú�����д��ѹ�������
//      ulOutputBufferLen       [IN]        ��ѹ�����ݴ�ŵ�Buffer����
//      pulDecompressedDataLen  [INOUT]     ��ѹ�����ݵ�ʵ�ʳ���
// �� �� ֵ: 
//      HI_ERR_SUCCESS  ��ѹ�ɹ�
//      ����ֵ��ʾʧ��
// ����Ҫ��:
// ���þ���: 
//*****************************************************************************
HI_U32 HI_LZMA_GetDecompressSize(HI_IN HI_U8 *pInputBuffer,HI_IN HI_U32 ulInputBufferLen,HI_INOUT HI_U32 *pulDecompressedDataLen);

//*****************************************************************************
// ��������: HI_LZMA_Decompress
// ��������: ��ѹLZMA��ʽ������
// ����˵��:
//      pInputBuffer            [IN]        ����ѹ���ݵ�Bufferָ��
//      ulInputBufferLen        [IN]        ����ѹ���ݵĳ���
//      pOutputBuffer           [INOUT]     ��ѹ�����ݴ�ŵ�Bufferָ�룬�ɵ����߷����ڴ棬�ú�����д��ѹ�������
//      ulOutputBufferLen       [IN]        ��ѹ�����ݴ�ŵ�Buffer����
//      pulDecompressedDataLen  [INOUT]     ��ѹ�����ݵ�ʵ�ʳ���
// �� �� ֵ: 
//      HI_ERR_SUCCESS  ��ѹ�ɹ�
//      ����ֵ��ʾʧ��
// ����Ҫ��:
// ���þ���: 
//*****************************************************************************
HI_U32 HI_LZMA_Decompress(HI_IN    HI_U8  *pInputBuffer, 
                          HI_IN    HI_U32 ulInputBufferLen, 
                          HI_INOUT HI_U8  *pOutputBuffer,
                          HI_IN    HI_U32 ulOutputBufferLen,
                          HI_INOUT HI_U32 *pulDecompressedDataLen);
#endif
//****************************************************************************


//*****************************************************************************
// DFX
//*****************************************************************************
#define HI_DIAG_SAVE_RC_TO_FLASH_ID_0  0
#define HI_DIAG_SAVE_RC_TO_FLASH_ID_1  1

typedef enum
{
    HI_DIAG_SAVE_RC_TO_FLASH_ID_4 = 0, // 4
    HI_DIAG_SAVE_RC_TO_FLASH_ID_5,     // 5
    HI_DIAG_SAVE_RC_TO_FLASH_ID_6,     // 6
    HI_DIAG_SAVE_RC_TO_FLASH_ID_7,     // 7
    HI_DIAG_SAVE_RC_TO_FLASH_ID_8,     // 8
    HI_DIAG_SAVE_RC_TO_FLASH_ID_9,     // 9
    HI_DIAG_SAVE_RC_TO_FLASH_ID_A,     // A
    HI_DIAG_SAVE_RC_TO_FLASH_ID_MAX
}HI_DIAG_SAVE_RC_TO_FLASH_ID_S;  //B��C��D��V2R1B017֮��汾�������洢�����б�

HI_EXTERN HI_U32 HI_DFX_SaveRecord2FlashPrv(HI_DIAG_SAVE_RC_TO_FLASH_ID_S id, HI_PBYTE pucBuffer, HI_U16 usBufferSize);
HI_EXTERN HI_U32 HI_DFX_ReadRecordFlashPrv(HI_DIAG_SAVE_RC_TO_FLASH_ID_S id, HI_OUT HI_PBYTE pucBuffer, HI_U16 usBufferSize);
HI_PRVL HI_U32 HI_SYS_DfxSendFrameTest(HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize) {return HI_ERR_NOT_SUPPORT;}
HI_EAPI HI_U32 HI_SYS_DfxSetBootVerInfo(HI_CONST HI_CHAR* szProductVer, HI_CONST HI_CHAR* szBootVer, HI_CONST HI_CHAR szBuiltDataTime[20]);


//*****************************************************************************
//
// �汾������, ��ʽΪ x,y,z ��ʾvx.y.z, x/y/zΪ����,ÿ���������Ϊ999
// �ڳ�ʼ����� XXX_Init()������, ����
// HI_VOID APP_Init() { HI_SYS_CONFIG_APP_VER(1,0,2);} ���ʾ APP�İ汾Ϊ v1.0.2 ��HSO�п��Բ�ѯ
//
#define HI_SYS_CONFIG_BOOT_VER(p,b,d) HI_SYS_DfxSetBootVerInfo(p, b, d)
#define HI_SYS_CONFIG_OS_VER(x,y,z)   HI_SYS_DfxSetVerInfo(HI_DFX_OS_VER,  HI_SAL_SYS_DFX_VER_STR(x,y,z), HI_SAL_SYS_DFX_VER_BUILD_DATETIME())
#define HI_SYS_CONFIG_SAL_VER(x,y,z)  HI_SYS_DfxSetVerInfo(HI_DFX_SAL_VER, HI_SAL_SYS_DFX_VER_STR(x,y,z), HI_SAL_SYS_DFX_VER_BUILD_DATETIME())
#define HI_SYS_CONFIG_DRV_VER(x,y,z)  HI_SYS_DfxSetVerInfo(HI_DFX_DRV_VER, HI_SAL_SYS_DFX_VER_STR(x,y,z), HI_SAL_SYS_DFX_VER_BUILD_DATETIME())
#define HI_SYS_CONFIG_PHY_VER(x,y,z)  HI_SYS_DfxSetVerInfo(HI_DFX_PHY_VER, HI_SAL_SYS_DFX_VER_STR(x,y,z), HI_SAL_SYS_DFX_VER_BUILD_DATETIME())
#define HI_SYS_CONFIG_MAC_VER(x,y,z)  HI_SYS_DfxSetVerInfo(HI_DFX_MAC_VER, HI_SAL_SYS_DFX_VER_STR(x,y,z), HI_SAL_SYS_DFX_VER_BUILD_DATETIME())
//*****************************************************************************


//****************************************************************************
// ϵͳ ���õ� UPG �ӿ�
//****************************************************************************

//****************************************************************************
HI_EAPI HI_U16 HI_SYS_Crc16(HI_U16 initData, HI_CONST HI_U8 *buf, HI_S32 len);
HI_EAPI HI_U32 HI_SYS_Crc32(HI_U32 initData, HI_CONST HI_U8* buf, HI_U32 len, HI_BOOL bSoftCrc);
//****************************************************************************

//****************************************************************************
// ��������: HI_SYS_SendInitMessage
// ��������: ����ʼ����Ϣ֪ͨ�ϲ����, ����DRVʹ��.
//
// ����˵��:
//   pstMsg [in] ��Ϣ��ṹ, ulMsgIdΪ HI_SYS_MSG_SYS_INIT_STATUS ʱ
//               ulParam[0]ȡֵΪHI_ND_SYS_STATUS_UPGRADE��ʾ������ģʽ�£������Ѿ�����
//               ulParam[0]ȡֵΪHI_ND_SYS_STATUS_NORMAL��ʾ������ģʽ�£������Ѿ�����
//
// �� �� ֵ:
//    HI_ERR_SUCCESS: �ɹ�.
//    HI_ERR_NO_INITILIZATION : SALδ��ʼ�����.
//    ����������ΪHI_MDM_MSG_Send�ķ���ֵ.
//
//****************************************************************************
HI_EAPI HI_U32 HI_SYS_SendInitMessage(HI_SYS_QUEUE_MSG_S* pstMsg);

//*****************************************************************************
// ����STʹ�õĽӿ�
//*****************************************************************************
// ��������: HI_SYS_DfxStartTestTask
// ��������: Test Task start API
//
// ����˵��:
//  enTaskId : ��������ID
//  pfnTask : �����������
//  ucPriority : �����������ȼ�, ����С��  HI_SYS_TEST_TASK_MAX
//
// �� �� ֵ:
//    None
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: s00130424/s00130424 [2012-08-02]
//*****************************************************************************
#if defined(PRODUCT_CFG_FEATURE_ST)
#define HI_SYS_TEST_TASK_STACK_SIZE    4096
typedef enum {HI_SYS_TEST_TASK_1, HI_SYS_TEST_TASK_2, HI_SYS_TEST_TASK_3, HI_SYS_TEST_TASK_MAX}HI_SYS_TEST_TASK_ID_E;
HI_EAPI HI_U32 HI_SYS_DfxStartTestTask(HI_SYS_TEST_TASK_ID_E enTaskId, HI_VOID_CALLBACK_F pfnTask, HI_U8 ulPriority);
#endif
//*****************************************************************************


//****************************************************************************
// �Ƿ�Ϊ�����汾
//****************************************************************************
HI_EAPI HI_BOOL HI_SYS_IsBaseCmpVer(HI_VOID);


//****************************************************************************
// ��⵱ǰ�Ƿ���DIAGģʽ��
//****************************************************************************
HI_EAPI HI_BOOL HI_SYS_IsDiagMode(HI_VOID);


//****************************************************************************
// ��ȡ��ʼ��״̬
//****************************************************************************
HI_EAPI HI_SYS_INIT_STATUS_E HI_SYS_GetInitStatus(HI_VOID);


//****************************************************************************
HI_EAPI HI_BOOL HI_SYS_IsSysInitComplete(HI_VOID);
HI_EAPI HI_U32 HI_DMS_ChlSpaceAvail(HI_U16 usPortNum, HI_U16 usDataToSendSize);
//*****************************************************************************


//*****************************************************************************
// ����ӿ�
//
#define SAL_PRINT0(fmt)
#define SAL_PRINT1(fmt, p1)
#define SAL_PRINT2(fmt, p1, p2)
#define SAL_PRINT3(fmt, p1, p2, p3)
#define SAL_PRINT4(fmt, p1,p2,p3,p4)
#define SAL_PRINT5(fmt, p1,p2,p3,p4,p5)
#define SAL_PRINTW0(fmt)
#define SAL_PRINTW1(fmt, p1)
#define SAL_PRINTW2(fmt, p1,p2)
#define SAL_PRINTW3(fmt, p1,p2,p3)
#define SAL_PRINTE0(fmt)
#define SAL_PRINTE1(fmt, p1)
#define SAL_PRINTE2(fmt, p1,p2)
#define SAL_PRINTE3(fmt, p1,p2,p3)
//*****************************************************************************

#if !defined(PRODUCT_CFG_CORE_TYPE_BOOTLOADER)


//****************************************************************************
#if !defined(PRODUCT_CFG_CORE_TYPE_BOOTLOADER) || defined(PRODUCT_CFG_HSO)
HI_EAPI HI_U32 DIAG_PostPkMsg(HI_U16 usPortNum, HI_DIAG_CMD_PARALLEL_CTRL_F pfnPkComplete, HI_MDM_DIAG_PK_REQ_S* pstPk);
HI_EAPI HI_U32 DIAG_ProcessReqPacket(HI_U16 usPortNum, HI_PBYTE pucData, HI_U16 usDataLen);
#endif
//****************************************************************************

//****************************************************************************
//#if defined(PRODUCT_CFG_FEATURE_MDM_MSG_MULTI_APP)
HI_EXTERN HI_U32 MAC_NdcRxProc(HI_U16 ulId, HI_U8 ucInstanceId, HI_ND_REMOTE_FRAME_HEAD_S* pstNdcFrm, HI_PBYTE pucPacket, HI_U16 usPacketSize);
//#endif
//****************************************************************************

//****************************************************************************
HI_EXTERN HI_U32 DMS_SendMessage2MacLayer(HI_U32 ulMsgId, HI_IN HI_PVOID pMsgParam);
#if !defined(PRODUCT_CFG_CORE_TYPE_BOOTLOADER) && defined(PRODUCT_CFG_FEATURE_EXPORT_NV_FILE)
HI_EXTERN HI_U32 NVM_GetExportFileInfo(HI_IN HI_OUT NVM_EXPORT_FILE_INFO_STRU *pExportFileInfo, HI_BOOL bWork);
#endif
HI_CONST HI_NV_FTM_PRODUCT_ID_S* SAL_GetProductId(HI_VOID);
//****************************************************************************

/* BEGIN: Added by sunhaiyang/00238140, 2014/2/19   PN:DTS2014012301705*/
#if defined(PRODUCT_CFG_FEATURE_NDC_CHL)
HI_EXTERN HI_U32 MAC_DevIdConfig(HI_U8 acuId[HI_MDM_DEV_ID_LEN]);
#endif // #if defined(PRODUCT_CFG_FEATURE_NDC_CHL)

HI_EXTERN HI_VOID CMN_DevIdBcd2Bin(HI_IN HI_U8 aucIdBcd[6], HI_OUT HI_U8 aucIdBin[5]);
HI_EXTERN HI_U32 CMN_DevIdBin2Bcd(HI_IN HI_U8 aucBcdBin[5], HI_OUT HI_U8 aucDevId[6]);
/* END:   Added by sunhaiyang/00238140, 2014/2/19 */
#endif // #if !defined(PRODUCT_CFG_CORE_TYPE_BOOTLOADER)
//****************************************************************************
HI_U32 UPG_SendMessage(HI_SYS_QUEUE_MSG_S* pstMsg);
/* BEGIN: PN: DTS2014012607684 MODIFY\ADD\DEL by cuiate/00233580 at 2014/2/17 */
HI_CPP_END
/* END:   PN: DTS2014012607684 MODIFY\ADD\DEL by cuiate/00233580 at 2014/2/17 */

//****************************************************************************
#endif // _MDM_INF_H__

