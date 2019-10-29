//****************************************************************************
//
//                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾
//
//****************************************************************************
//  �� �� ��   : hi_sal.h
//  �� �� ��   : V1.0
//  ��    ��   : ����/00130424
//  ��������   : 2011��10��10��
//  ��������   : SAL(ϵͳ����Ӧ�ò�), ����OS�ӿ�/DMS/NV/DIAG��ϵͳ��ʼ����OS��Դ�������
//  �����б�   :
//  �޸���ʷ   :
//  1.��    �� : 2011��10��10��
//    ��    �� : ����/00130424
//    �޸����� : �����ļ�
//  2.��    �� : 2011��10��27��
//    ��    �� : dubin/KF35509
//    �޸����� : ����Cipher��GPIO�ӿڶ���
//
//****************************************************************************

#ifndef __HI_SAL_H__
#define __HI_SAL_H__
#include <hi_types.h>
HI_START_HEADER
#include <hi_mdm_types.h>
#include <hi_ft_nv.h>
#include <hi_sal_prv.h>
#include <hi_mdm.h>
#if defined(PRODUCT_CFG_TASK_INT_SYS_ERR_INFO)&&defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
#include <drv_dfx.h>

typedef struct
{
    HI_U32 ulLisrTickIn;
    HI_U32 ulLisrTickOut;
    HI_U32 ulLisrCnt;
    HI_U32 ulHisrTickIn;
    HI_U32 ulHisrTickOut;
    HI_U32 ulHisrCnt;
}HI_ISR_INFO_S;

typedef struct
{
    HI_U32 ulLisrTickIn;
    HI_U32 ulLisrTickOut;
    HI_U32 ulLisrCnt;
    HI_U32 ulTxCnt;
    HI_U32 ulRxCnt;
}HI_UART_INFO_S;

typedef struct
{
    HI_U32 ulJobPool[MAX_ISR_JOBS];
    DRV_DFX_SPY_INFO stSpyInfo;
}HI_OS_INFO_S;

typedef struct
{
    HI_U32 ulMagic;
    HI_U32 ulInt_lr;
    HI_UART_INFO_S stUartTick[4];
    HI_ISR_INFO_S stPhyTick;
    HI_ISR_INFO_S stHrtimer;
    HI_ISR_INFO_S stEth;
    HI_ISR_INFO_S stGPIO;
    HI_OS_INFO_S stOsInfo;
}HI_SYS_ISR_STAT_S;

extern HI_SYS_ISR_STAT_S g_Isr_stat;

#endif //PRODUCT_CFG_TASK_INT_SYS_ERR_INFO

#define HI_SYS_POWER_CUT_RESTART   		1
#define HI_SYS_NON_POWER_CUT_RESTART   	0

#if defined(PRODUCT_CFG_BIG_ENDIAN)
#define HI_SWAP32(data)       (data)
#else
#define HI_SWAP32(data)       ((((unsigned int)(data) << 24) & (unsigned int)0xff000000) | \
                             (((unsigned int)(data) <<  8) & (unsigned int)0x00ff0000) | \
                             (((unsigned int)(data) >>  8) & (unsigned int)0x0000ff00) | \
                             (((unsigned int)(data) >> 24) & (unsigned int)0x000000ff))
#endif

/* BEGIN: Added by b00208046, 2014/3/13   ���ⵥ��:DTS2014031300959 */
typedef enum
{
    HI_MDM_UFC_BLK_ID_UPG = 0, // ����
    HI_MDM_UFC_BLK_ID_MR_LOG, // mr log
    HI_MDM_UFC_BLK_ID_FA,
    HI_MDM_UFC_BLK_ID_NMLV,
}HI_MDM_UFC_BLK_ID_E;
typedef enum
{
    HI_MDM_UFC_OPT_WRITE = 0x1,
    HI_MDM_UFC_OPT_READ  = 0x2,
    HI_MDM_UFC_LIMIT = 0x100,
}HI_MDM_UFC_OPT_E;
typedef enum
{
    HI_SAL_MEM_POOL_ID_SYS,//ϵͳ�ڴ��
    HI_SAL_MEM_POOL_ID_DYNAMIC,//��̬�ڴ��
    HI_SAL_MEM_POOL_ID_STATIC,//��̬�ڴ��
    HI_SAL_MEM_POOL_ID_MAX,
}HI_SAL_MEM_POOL_ID_E;
#if defined(PRODUCT_CFG_UFC_FLASH_MANAGE) // ��CCO��Ʒϵ��֧�ָ�����
HI_EAPI HI_U32 HI_MDM_UfcBlkAvail(HI_U8 ucBlkId);
HI_EAPI HI_U32 HI_MDM_UfcBlkObtain(HI_U8 ucBlkId, HI_U32 ulSize, HI_PVOID* ppAddr, HI_U8 ulOpt);
HI_EAPI HI_U32 HI_MDM_UfcBlkRelease(HI_U8 ucBlkId);
HI_EAPI HI_U32 HI_MDM_UfcBlkRead(HI_U8 ucBlkId, HI_CONST HI_U8* pAddress, HI_CONST HI_U32 ulSize, HI_U32 * pReadSize, HI_CHAR* pBuffer);
HI_EAPI HI_U32 HI_MDM_UfcBlkWrite(HI_U8 ucBlkId, HI_CONST HI_U8* pAddress, HI_U32 ulSize, HI_U32 * pWrittensize, HI_CONST HI_CHAR* pBuffer, HI_BOOL bAutoErase);
HI_EAPI HI_U32 HI_MDM_UfcBlkErase(HI_U8 ucBlkId, HI_CONST HI_U8* pAddress, HI_CONST HI_U32 ulSize, HI_U32 * pEraseSize);
HI_EAPI HI_U32 HI_MDM_UfcGetAddressAndSize(HI_U8 ucBlkId,HI_U32 *pFlashAddress,HI_U32 *pFlashSize);
HI_EAPI HI_VOID HI_MDM_UfcSetWriteAble(HI_BOOL writeAble);
#endif
/* END:   Added by b00208046, 2014/3/13 */


//*****************************************************************************
//****************************************************************************
// ��λ����ӿڶ���
//****************************************************************************
// �ӳ�����(��λs):��Ӱ��g_stProCtx.astRstNotify[x].bRstDelayΪHI_TRUE��������λ�¼�
HI_EAPI HI_U32 HI_SYS_ActiveRstDelay(HI_IN HI_U32 duration);
// �ӳ�����ʹ��
HI_EAPI HI_U32 HI_SYS_RstDelayEnable(HI_SYS_RST_DELAY_CON_E enDelayCon,HI_U32 ms);

// �ӳ�����ȥʹ��
HI_EAPI HI_U32 HI_SYS_RstDelayDisable (HI_SYS_RST_DELAY_CON_E enDelayCon);

// ע�Ḵλ֪ͨ�������
HI_EAPI HI_U32 HI_SYS_RstRegisterNotify (HI_CONST HI_SYS_RST_NOTIFY_F pfnRstNotify, HI_SYS_RST_TRIGGER_CON_E enTrigerCon);
HI_EAPI HI_BOOL HI_SYS_RstIsDelayStatus(HI_VOID);
HI_EAPI HI_U32 HI_SYS_RstSleep(HI_VOID);
//****************************************************************************

#if defined(PRODUCT_CFG_GET_UPG_FILE_BUILD_DATE_TIME)
HI_EAPI HI_U32 HI_SYS_GetUpgFileBuildDateTime(HI_IN HI_PVOID pUpgFileBuffer, HI_U16 usUpgFileBufferSize, HI_OUT HI_U8 aucUpgBuildDataTime[6]);
#endif
//****************************************************************************


#if !defined(PRODUCT_CFG_CORE_TYPE_BOOTLOADER)
//****************************************************************************
// ��ȡϵͳ״̬ HI_SYS_STATUS_IND_S
//****************************************************************************
HI_EAPI HI_SYS_STATUS_IND_S* HI_SYS_GetSysStatus(HI_VOID);
#endif
//*****************************************************************************


//*****************************************************************************
// ��������: HI_MDM_Sleep
// ��������: ����˯��
//
// ����˵��:
//   ulMilliSecond [in] ˯��ʱ��, ��λΪ����, ����Ϊ10ms
//   ���С��10msʱ������10ms��������10msʱ�������¶���
//
// �� �� ֵ:
//    HI_ERR_SUCCESS: �ɹ�
//    ����������μ�hi_errno.h
//
// ����Ҫ��: ����������ʹ��
// ���þ���: HI_MDM_Sleep(10)
// ��    ��: shenhankun/00130424 [2011-11-10]
//*****************************************************************************
#define HI_SYS_TASK_WAIT_EXCEPTION_POLLING_WAIT()     HI_MDM_Sleep(HI_SYS_TASK_WAIT_EXCEPTION_TIME_OUT)


//*****************************************************************************
// ��������:  HI_SYS_GetMemPoolInfo
//
// ��������:  ��ȡ�ڴ����Ϣ
//
// ����˵��:
//   enPoolId[in]�ڴ��ID
//   pstPoolInfo[out]�ڴ����Ϣ
//
// �� �� ֵ:
//    HI_ERR_SUCCESS : ��ȡ�ڴ����Ϣ�ɹ�
//    ����ֵ����
// ����Ҫ��:
//*****************************************************************************
HI_U32 HI_SYS_GetMemPoolInfo(HI_SAL_MEM_POOL_ID_E enPoolId,HI_MDM_MEM_POOL_INFO_S *pstInfo);

#if defined(PRODUCT_CFG_HAVE_STATIC_MEM_PROFILE)
#define HI_SYS_GETSTATIC_MEM(ulId, ulSize)  HI_MDM_GetStaticMem(ulId, ulSize); HI_DIAG_ReportSysInitEvt(id)
#else
#define HI_SYS_GETSTATIC_MEM(ulId, ulSize)  HI_MDM_GetStaticMem(ulId, ulSize);
#endif


//****************************************************************************
// ��������: HI_MDM_Malloc HI_MDM_Free
// ��������: ��̬���䡢�ͷ��ڴ�
//
// ����˵��:
//   ulModId [in] ģ��ID, 0��ʾ��׼ģ��,������ϵͳʹ��, �μ� hi_config.h ����, HI_MOD_ID_*,�û�����USER : [900, 999]
//   ulSize [in] ��̬�ڴ��С
//   pAddr [in] ��̬�ڴ�ָ��
//
// �� �� ֵ:
//    �����ʾ�ڴ��ַ�����ʾϵͳû�г�ʼ�������ڴ治��
//
// ����Ҫ��: 1) ϵͳ��ʼ���ɹ�
//           2) ʹ�÷�����malloc��free����
//           4) ulModId �����ڷ�����ͷŽӿ����������һ��.
// ���þ���: TODO: ...
// ��    ��: shenhankun/00130424 [2011-10-19]
//****************************************************************************

HI_VOID HI_SYS_CHECK_DYN_HEAP(HI_PVOID pHeap, HI_U32 ulSize, HI_U32 ulDbg);
#if defined(PRODUCT_CFG_DYN_MEM_CHECK) && !defined(PRODUCT_CFG_CORE_TYPE_BOOTLOADER) && defined(PRODUCT_CFG_DYN_MEM_CHECK_API)
HI_VOID HI_SYS_CheckDynHeap(HI_PVOID pHeap, HI_U32 ulSize, HI_U32 v3 , HI_U32 v4, HI_U32 ulLine, HI_PCSTR pszFunction);
#define HI_SYS_CHECK_DYN_HEAP(_pHeap, _ulSize, _v3)  HI_SYS_CheckDynHeap(_pHeap, _ulSize, _v3, HI_SYS_GET_LR(), (HI_U32)__LINE__, (HI_PCSTR)__FUNCTION__)
#else
#define HI_SYS_CHECK_DYN_HEAP(_pHeap, _ulSize, _v3) {}
#endif
//*****************************************************************************


//*****************************************************************************
// ��������: HI_SYS_ControlInterrupts
// ��������: �жϿ���
//
// ����˵��:
//   ulLevel [in] HI_SYS_ENABLE_INTERRUPTS  : �����ж�ʹ��
//                HI_SYS_DISABLE_INTERRUPTS : �����жϹر�
//
// �� �� ֵ:
//    HI_SYS_DISABLE_INTERRUPTS : �����жϹر�
//    HI_SYS_ENABLE_INTERRUPTS  : �����ж�ʹ��
//    HI_ERR_FAILURE        : �жϿ���ʧ��
//
// ����Ҫ��: TODO: ...
// ���þ���:
//           HI_VOID interrupt20(HI_VOID)
//           {
//              HI_U32 level = HI_SYS_ControlInterrupts(HI_SYS_DISABLE_INTERRUPTS)
//              // Code ...
//              HI_SYS_ControlInterrupts(level)
//           }
//           HI_VOID interrupt21(HI_VOID)
//           {
//              HI_SYS_DisableInterrupts()
//              // Code ...
//              HI_SYS_EnableInterrupts()
//           }
//
// ��    ��: shenhankun/00130424 [2011-12-01]
//*****************************************************************************
HI_EAPI HI_U32 HI_SYS_ControlInterrupts(HI_U32 ulLevel);
#if defined(PRODUCT_CFG_SAL_API_SUPPORT)
#define HI_SYS_DisableInterrupts()   HI_U32 ulSysIntLevel = HI_SYS_ControlInterrupts(HI_SYS_DISABLE_INTERRUPTS);
#define HI_SYS_EnableInterrupts()    HI_SYS_ControlInterrupts(ulSysIntLevel);

#define HI_SYS_INTS_LOCK()           { HI_S32 __intlevel__ = (HI_S32)HI_SYS_ControlInterrupts(HI_SYS_DISABLE_INTERRUPTS);
#define HI_SYS_INTS_UNLOCK()         HI_SYS_ControlInterrupts((HI_U32)__intlevel__); }
#endif
//*****************************************************************************

//*****************************************************************************
// ��������: HI_SYS_TSTK_ERR_ASSERT
// ��������: ����ջ���
//
// ����˵��:
//   ��
//
// �� �� ֵ:
//    ��
//
// ����Ҫ��: �����е���, �����������ж�
// ��    ��: shenhankun/00130424 [2012-01-29]
//*****************************************************************************
#if defined(PRODUCT_CFG_CHECK_TASK_STACK) && !defined(HAVE_PCLINT_CHECK)
#define HI_SYS_CHECK_TASK_STACK()   do { HI_SYS_TaskStackErrAssertPrv((HI_PCSTR )HI_NULL, (HI_U32)__LINE__, (HI_PCSTR )__FUNCTION__);} while(0)
#else
#define HI_SYS_CHECK_TASK_STACK()   do { } while(0)
#endif
#define HI_SYS_TSTK_ERR_ASSERT()    HI_SYS_CHECK_TASK_STACK()
//*****************************************************************************


//*****************************************************************************
// Socket API
//*****************************************************************************
#ifdef PRODUCT_CFG_FEATURE_SOCKET
//*****************************************************************************
// ��������: HI_SYS_SocketSend
// ��������: �����������ӵ����ݰ�����ʽ�׽ӿڷ������ݡ�
//           �������ݱ����׽ӿڣ�����ע�ⷢ�����ݳ��Ȳ�
//           Ӧ����ͨѶ������IP����󳤶�
//
// ����˵��:
//   s [in] : һ�����ڱ�ʶ�������׽ӿڵ�������
//   buf [in] : �������������ݵĻ�����
//   len [in] : �����������ݵĳ���
//
// �� �� ֵ:
//          ���޴�����,�������������ݵ�������
//          ����Ļ�������ERROR(-1)����
//  ˵��:
//       ��������������I/O�ĵ���ִ�з�ʽ
//*****************************************************************************
HI_EAPI HI_S32    HI_SYS_SocketSend(HI_SOCKET s, HI_PBYTE buf, HI_S32 len);
//*****************************************************************************
// ��������: HI_SYS_SocketRecv
// ��������: ���������������ӵ����ݱ�����ʽ�׽ӿڽ������ݵĽ��ա�
//
// ����˵��:
//   s [in] : һ�����ڱ�ʶ�������׽ӿڵ�������
//   buf [out] : ���ڽ������ݵĻ�����
//   len [in] : �������ĳ���
//
// �� �� ֵ:
//          ���޴�������HI_SYS_SocketRecv()���ض�����ֽ�����
//          �����������ֹ������0������������󣬷���-1
//  ˵��:
//       ��������������I/O�ĵ���ִ�з�ʽ
//*****************************************************************************
HI_EAPI HI_S32    HI_SYS_SocketRecv (HI_SOCKET s, HI_OUT HI_PBYTE buf, HI_S32 len);
//*****************************************************************************
// ��������: HI_SYS_SocketSendTo
// ��������: ������������ָ����socket�����Է�����
//
// ����˵��:
//   s [in] : һ�����ڱ�ʶ�������׽ӿڵ�������
//   buf [in] : ���ڷ������ݵĻ�����
//   len [in] : �������ĳ���
//   pucIp [in]: ָ��Ŀ���׽��ֵĵ�ַ
//   usPortNum [in] : �˿ں�
//
// �� �� ֵ:
//          �ɹ��򷵻�ʵ�ʴ��ͳ�ȥ���ַ�����ʧ�ܷ��أ�1
//  ˵��:
//         ��������������I/O�ĵ���ִ�з�ʽ
//*****************************************************************************
HI_EAPI HI_S32    HI_SYS_SocketSendTo(HI_SOCKET s, HI_PBYTE buf, HI_S32 len, HI_IN HI_PCSTR pucIp, HI_U16 usPortNum);
//*****************************************************************************
// ��������: HI_SYS_SocketRecvFrom
// ��������:��������Զ��������ָ����socket����������
//          �������ݴ����ɲ���bufָ����ڴ�ռ�
//
// ����˵��:
//   s [in] : һ�����ڱ�ʶ�������׽ӿڵ�������
//   buf [in] : ���ڽ������ݵĻ�����
//   len [in] : �������ĳ���
//
// �� �� ֵ:
//          �ɹ��򷵻ؽ��յ����ַ���,ʧ�ܷ���-1
//  ˵��:
//          ��������������I/O�ĵ���ִ�з�ʽ
//*****************************************************************************
HI_EAPI HI_S32    HI_SYS_SocketRecvFrom(HI_SOCKET s, HI_OUT HI_PBYTE buf, HI_S32 len);
//*****************************************************************************
// ��������: HI_SYS_SocketConnect
// ��������:���������ڴ�����ָ���ⲿ�˿ڵ�����
//
// ����˵��:
//   s [in] : һ�����ڱ�ʶ�������׽ӿڵ�������
//   pucIp [in] : Ŀ��IP��ַ
//   usPortNum [in] : �˿ں�
//
// �� �� ֵ:
//          �ɹ��򷵻�0��ʧ�ܷ���-1
//*****************************************************************************
HI_EAPI HI_S32    HI_SYS_SocketConnect(HI_SOCKET s, HI_PCSTR pucIp, HI_U16 usPortNum);
//*****************************************************************************
// ��������: HI_SYS_SocketConnect
// ��������:�����κ����͵��׽ӿڽ�ֹ���ա���ֹ���ͻ��ֹ�շ�
//
// ����˵��:
//   s [in] : һ�����ڱ�ʶ�������׽ӿڵ�������
//   how����־������������ֹ��Щ������
//              how�ķ�ʽ�����ֱַ���
//              SHUT_RD��0�����ر�sockfd�ϵĶ����ܣ���ѡ�������sockfd���ж�������
//              SHUT_WR��1�����ر�sockfd��д���ܣ���ѡ�������sockfd����д������
//              SHUT_RDWR��2�����ر�sockfd�Ķ�д���ܡ�
// �� �� ֵ:
//           �ɹ��򷵻�0��ʧ�ܷ���-1
//*****************************************************************************
HI_EAPI HI_S32    HI_SYS_SocketShutdown(HI_SOCKET s, HI_S32 how);
//*****************************************************************************
// ��������: HI_SYS_SocketClose
// ��������:�������ر�һ���׽ӿ�
//
// ����˵��:
//   s [in] : һ�����ڱ�ʶ�������׽ӿڵ�������
//
// �� �� ֵ:
//          �ɹ��򷵻�0��ʧ�ܷ���-1
//*****************************************************************************
HI_EAPI HI_S32    HI_SYS_SocketClose(HI_SOCKET s);
//*****************************************************************************
// ��������: HI_SYS_SocketAccept
// ��������:��һ���׽ӿڽ���һ������
//
// ����˵��:
//   s [in] : һ�����ڱ�ʶ�������׽ӿڵ�������
//
// �� �� ֵ:
//           �ɹ�����һ���µ��׽�����������ʧ�ܷ���-1��
//*****************************************************************************
HI_EAPI HI_SOCKET HI_SYS_SocketAccept (HI_SOCKET s);
//*****************************************************************************
// ��������: HI_SYS_SocketTcpCreate
// ��������: ����TCP socket��
//      Ĭ�ϻ��������¼�������,��ͨ��onebuiler���޸�����:
//      ����BUFFER: PRODUCT_CFG_TCP_TX_BUFFER
//      ����BUFFER: PRODUCT_CFG_TCP_RCV_BUFFER
//      KEEPALIVE����:
//          PRODUCT_CFG_TCP_KEEPINTVL  #���÷���keepalive�ļ��ʱ�䣨s��
//          PRODUCT_CFG_TCP_KEEPALIVE_R2 #���ò�ͨ����ʼ����keepalive��ʱ�䣨s��
//          PRODUCT_CFG_KEEPALIVE_WAIT  #����keepalive̽�����(��)
//    #���÷���keepalive�ļ��ʱ�䣨s��
// ����˵��:��
//
// �� �� ֵ: ��ȷ:SOCKET_ID.
//           ʧ��:0xFFFFFFFF
//
// ����Ҫ��: �����е���, �����������ж�
// ��    ��: shenhankun/00130424 [2012-01-29]
//*****************************************************************************
HI_EAPI HI_SOCKET HI_SYS_SocketTcpCreate(HI_VOID);
//*****************************************************************************
// ��������: SAL_SocketUdpCreate
// ��������: ����UDP socket��
//
// ����˵��:��
//
// �� �� ֵ: ��ȷ:SOCKET_ID.
//           ʧ��:0xFFFFFFFF
//
// ����Ҫ��: �����е���, �����������ж�
// ��    ��: shenhankun/00130424 [2012-01-29]
//*****************************************************************************
HI_EAPI HI_SOCKET HI_SYS_SocketUdpCreate(HI_VOID);
HI_EAPI HI_U32    HI_SYS_SocketSetLinger(HI_SOCKET s);
//*****************************************************************************
// ��������: HI_SYS_SocketConnect
// ��������:��һ���ص�ַ��һ�׽ӿ�����
//                       ������������δ���ӵ����ݱ��������׽ӿ�
//
// ����˵��:
//   s [in] : һ�����ڱ�ʶ�������׽ӿڵ�������
//   pucIp [in] : Ŀ��IP��ַ
//   usPortNum [in] : �˿ں�
//
// �� �� ֵ:
//                   �ɹ��򷵻�HI_ERR_SUCCESS��ʧ�ܷ���HI_ERR_SYSTEM_CALL_ERROR
//*****************************************************************************
HI_EAPI HI_S32    HI_SYS_SocketBind(HI_SOCKET s, HI_PCSTR pucIp, HI_U16 usPortNum);
//*****************************************************************************
// ��������: HI_SYS_SocketListen
// ��������:����һ���׽ӿڲ��������������.
//
// ����˵��:
//   s [in] : һ�����ڱ�ʶ�������׽ӿڵ�������
//   backlog [in] : �ȴ����Ӷ��е���󳤶�
//
// �� �� ֵ:
//                   �ɹ��򷵻�0��ʧ�ܷ���-1
//*****************************************************************************
HI_EAPI HI_S32    HI_SYS_SocketListen(HI_SOCKET s, HI_S16 backlog);
//*****************************************************************************
// ��������: HI_SYS_SocketSetFd
// ��������:����Socketλͼ�е�λ��
//
// ����˵��:
//   s [in] : һ�����ڱ�ʶ�������׽ӿڵ�������
//   fd [in] : socket λ������ָ��
//
// �� �� ֵ:
//                   ��
//*****************************************************************************
HI_EAPI HI_VOID   HI_SYS_SocketSetFd(HI_SOCKET s, HI_IN HI_SYS_SOCKET_FD* fd);
HI_EAPI HI_BOOL   HI_SYS_SocketIsSetFd(HI_SOCKET s, HI_IN HI_SYS_SOCKET_FD* fd);
//*****************************************************************************
// ��������: HI_SYS_SocketSelect
// ��������:ȷ��һ�������׽ӿڵ�״̬
//
// ����˵��:
//   nfds [in] : ��һ������ֵ����ָ�����������ļ��������ķ�Χ
//   readfds [in] : ָ��һ��ȴ��ɶ��Լ����׽ӿ�
//   writefds [in] : ָ��һ��ȴ���д�Լ����׽ӿ�
//   ulTimeOut [in] : ���ȴ�ʱ��
// �� �� ֵ:
//                   �ɹ�����HI_ERR_SUCCESS
//                   ��ʱ����HI_ERR_TIMEOUT
//                   ���󷵻�HI_ERR_SYSTEM_CALL_ERROR
//*****************************************************************************
HI_EAPI HI_U32    HI_SYS_SocketSelect(HI_S32 nfds, HI_OUT HI_SYS_SOCKET_FD* readfds, HI_OUT HI_SYS_SOCKET_FD*writefds, HI_U32 ulTimeOut);
HI_EAPI HI_S32    HI_SYS_SocketGetError(HI_VOID);
#if defined(PRODUCT_CFG_SUPPORT_SET_SOCKET_OPT_SO_X)
HI_EAPI HI_U32    HI_SYS_SocketSetOpt(HI_SOCKET s, HI_SYS_SOCKET_OPT_ID_E enOpt, HI_PVOID optval,  HI_U32 optlen);// optval:Pointer to the new value for the option; optlen: The size in bytes of the location pointed to by optval
#else
HI_EAPI HI_U32    HI_SYS_SocketSetOpt(HI_SOCKET s, HI_SYS_SOCKET_OPT_ID_E enOpt, HI_U32 ulVal);
#endif
HI_EAPI HI_U32    HI_SYS_SocketSetKeepAlive(HI_SOCKET s, HI_S32 slEnable, HI_S32 slTcpKeepIdle, HI_S32 slTcpKeepCnt);
#else
HI_PRVL HI_S32    HI_SYS_SocketSend(HI_SOCKET s, HI_PBYTE buf, HI_S32 len) {return HI_SYS_SOCKET_ERROR;}
HI_PRVL HI_S32    HI_SYS_SocketRecv (HI_SOCKET s, HI_OUT HI_PBYTE buf, HI_S32 len) {return HI_SYS_SOCKET_ERROR;}
HI_PRVL HI_S32    HI_SYS_SocketSendTo(HI_SOCKET s, HI_PBYTE buf, HI_S32 len, HI_IN HI_PCSTR pucIp, HI_U16 usPortNum) {return HI_SYS_SOCKET_ERROR;}
HI_PRVL HI_S32    HI_SYS_SocketRecvFrom(HI_SOCKET s, HI_OUT HI_PBYTE buf, HI_S32 len) {return HI_SYS_SOCKET_ERROR;}
HI_PRVL HI_S32    HI_SYS_SocketConnect(HI_SOCKET s, HI_PCSTR pucIp, HI_U16 usPortNum) {return HI_SYS_SOCKET_ERROR;}
HI_PRVL HI_S32    HI_SYS_SocketShutdown(HI_SOCKET s, HI_S32 how) {return HI_SYS_SOCKET_ERROR;}
HI_PRVL HI_S32    HI_SYS_SocketClose(HI_SOCKET s) {return HI_SYS_SOCKET_ERROR;}
HI_PRVL HI_SOCKET HI_SYS_SocketAccept (HI_SOCKET s) {return HI_SYS_SOCKET_ERROR;}
HI_PRVL HI_SOCKET HI_SYS_SocketTcpCreate(HI_VOID) {return HI_SYS_SOCKET_ERROR;}
HI_PRVL HI_SOCKET HI_SYS_SocketUdpCreate(HI_VOID) {return HI_SYS_SOCKET_ERROR;}
HI_PRVL HI_U32    HI_SYS_SocketSetLinger(HI_SOCKET s) {return HI_SYS_SOCKET_ERROR;}
HI_PRVL HI_S32    HI_SYS_SocketBind(HI_SOCKET s, HI_PCSTR pucIp, HI_U16 usPortNum) {return HI_SYS_SOCKET_ERROR;}
HI_PRVL HI_S32    HI_SYS_SocketListen(HI_SOCKET s, HI_S16 backlog) {return HI_SYS_SOCKET_ERROR;}
HI_PRVL HI_VOID   HI_SYS_SocketSetFd(HI_SOCKET s, HI_IN HI_SYS_SOCKET_FD* fd) {return ;}
HI_PRVL HI_BOOL   HI_SYS_SocketIsSetFd(HI_SOCKET s, HI_IN HI_SYS_SOCKET_FD* fd) {return HI_FALSE;}
HI_PRVL HI_U32    HI_SYS_SocketSelect(HI_S32 nfds, HI_OUT HI_SYS_SOCKET_FD* readfds, HI_OUT HI_SYS_SOCKET_FD*writefds, HI_U32 ulTimeOut) {return HI_SYS_SOCKET_ERROR;}
HI_PRVL HI_S32    HI_SYS_SocketGetError(HI_VOID) {return HI_ERR_NOT_SUPPORT;}
#if defined(PRODUCT_CFG_SUPPORT_SET_SOCKET_OPT_SO_X)
HI_PRVL HI_U32    HI_SYS_SocketSetOpt(HI_SOCKET s, HI_SYS_SOCKET_OPT_ID_E enOpt, HI_PVOID optval,  HI_U32 optlen) {return HI_SYS_SOCKET_ERROR;}
#else
HI_PRVL HI_U32    HI_SYS_SocketSetOpt(HI_SOCKET s, HI_SYS_SOCKET_OPT_ID_E enOpt, HI_U32 ulVal) {return HI_SYS_SOCKET_ERROR;}
#endif
HI_PRVL HI_U32    HI_SYS_SocketSetKeepAlive(HI_SOCKET s, HI_S32 slEnable, HI_S32 slTcpKeepIdle, HI_S32 slTcpKeepCnt) {return HI_SYS_SOCKET_ERROR;}
#endif
//****************************************************************************

//*****************************************************************************
// UART
//*****************************************************************************
HI_EAPI HI_U32 HI_SYS_UartAdaptInit(HI_SYS_UART_E enUart, HI_U32 ulMaxByteInterval, HI_BOOL bEnable);
//*****************************************************************************


//****************************************************************************
// ��ȡ[ulMin,ulMax]֮����������
//****************************************************************************
HI_EAPI HI_U32 HI_SYS_GetRandNum(HI_U32 ulMin, HI_U32 ulMax);
HI_U32 HI_SYS_GetRandNumWithSeed(HI_U32 ulMin, HI_U32 ulMax, HI_U32 seed);
HI_U32 HI_SYS_GetRandNum32WithSeed(HI_U32 ulMin, HI_U32 ulMax, HI_U32 seed);

HI_EAPI HI_U32 HI_SYS_GetRandNum32(HI_U32 ulMin, HI_U32 ulMax);
HI_EAPI HI_U16 HI_SYS_GetRandNum16(HI_U16 ulMin, HI_U16 ulMax);
HI_EAPI HI_U32 HI_SYS_GetRandNum32AddMac(HI_U32 ulMin, HI_U32 ulMax);

//****************************************************************************

//****************************************************************************
// ��������: HI_SYS_Itoa
// ��������: ������ת�����ַ���
//
// ����˵��:
//      str[out]:ת������ַ���
//      num:��ת������
//      opt:ת����ʽHI_SYS_ITOA_OPT(ucfillChar,ucMinLen,enRadix)����
//          ucfillChar:����ַ���:�൱��sprint��%03d�е�0
//          ucMinLen:ת�����ַ�����С����:�൱��sprint��%03d�е�3
//          enRadix:����ʲô��ʽת��
//          enRadix:ת������HI_SYS_ITOA_RADIX_D:%d  HI_SYS_ITOA_RADIX_U:%u HI_SYS_ITOA_RADIX_H:%x
// �� �� ֵ:
//      HI_ERR_SUCCESS ת���ɹ�
//      ����:ת��ʧ��
//
// ����Ҫ��: ��
// ���þ���: ��
//****************************************************************************
#define HI_SYS_ITOA_RADIX_D     0
#define HI_SYS_ITOA_RADIX_U    1
#define HI_SYS_ITOA_RADIX_H     2

#define HI_SYS_ITOA_OPT(ucfillChar,ucMinLen,enRadix) ((((HI_U8)(ucfillChar))&0xFF)|((((HI_U8)(ucMinLen))<<8)&0xFF00)|((((HI_U8)(enRadix))<<16)&0xFF0000))
HI_EAPI HI_U32 HI_SYS_Itoa(HI_CHAR *str,HI_S32 num,HI_U32 opt);

HI_EAPI HI_U32 HI_SYS_Reboot(HI_SYS_REBOOT_CAUSE_E enCause);
HI_EAPI HI_U32 HI_SYS_RebootSafe(HI_SYS_REBOOT_CAUSE_E enCause);

//*****************************************************************************
// �����豸��ؽӿ�
//*****************************************************************************

//*****************************************************************************


//*****************************************************************************
// NV Interface
//*****************************************************************************

//*****************************************************************************
// ��������: HI_NVM_Read
//
// ��������: ����NV��ID��NV������
//           ��ȡIDΪulId��NV�������pItemData�ϣ���������ΪulItemDataLen,
//           ����ʵ�ʳ��ȵ��浽pusItemDataLenָ����.
//
// ����˵��: ulId [in] NV��ID��ID�ľ��嶨���ɹ�������.
//           pItemData[out] ����NV�����ݵ�ָ�룬���ڸú�����дָ����NV��õ�ַ.
//           ulItemDataLen[in] pItemDataָ��buffer�ĳ���,Ŀǰ��󳤶�֧��1024���ֽ�
//           bIfFailUseDefault[in] HI_TRUE: �����ȡNV��ʧ��, ��ʹ��NV�ļ��ĳ�ʼ��ֵ���.
//
// �� �� ֵ: HI_ERR_SUCCESS                ��ʾ��ȡNV��ɹ�
//           HI_ERR_NV_ITEM_IS_EMPTY	    NV��Ϊ��
//           HI_ERR_NV_NOT_SUPPORT_ID	    NV��ID������
//           HI_ERR_NV_NOT_SUPPORT_LENTH	NV��ID���ȳ���Ԥ�����ʵ�ʳ���
//           ERR_SAL_FLASH_ALIGN_ERROR	    Flash�������
//           ERR_SAL_FLASH_PROTECT_ERROR	Flash����
//           HI_ERR_NOT_SUPPORT	        ��֧��
//
// ����Ҫ��: 1����������ʹ�ã��������ж���ʹ��, ֧�ֶ����������
//*****************************************************************************
#if defined(PRODUCT_CFG_FEATURE_NVM)
HI_EAPI HI_U32 HI_NVM_Read(HI_U16 usId, HI_OUT HI_PVOID pItemData, HI_U16 usItemDataLen);
HI_EAPI HI_U32 HI_NVM_ReadItem(HI_U16 usId, HI_OUT HI_PVOID pItemData, HI_U16 usItemDataLen, HI_BOOL bIfFailUseDefault);
HI_EAPI HI_U32 HI_NVM_GetItemLength(HI_U16 usId, HI_OUT HI_U16* pusItemLength);
#else
HI_PRVL HI_U32 HI_NVM_Read(HI_U16 usId, HI_OUT HI_PVOID pItemData, HI_U16 usItemDataLen) {return HI_ERR_NOT_SUPPORT;}
HI_PRVL HI_U32 HI_NVM_ReadItem(HI_U16 usId, HI_OUT HI_PVOID pItemData, HI_U16 usItemDataLen, HI_BOOL bIfFailUseDefault) {return HI_ERR_NOT_SUPPORT;}
HI_PRVL HI_U32 HI_NVM_GetItemLength(HI_U16 usId, HI_OUT HI_U16* pusItemLength) {return 0;}
#endif
//*****************************************************************************


//*****************************************************************************
// ��������: HI_NVM_Write HI_NVM_WriteFactory
//
// ��������: ����NV��IDдNV������
//
// ����˵��: ulId [in] NV��ID��ID�ľ��嶨���ɹ�������.
//           pItemData[in] NV�����ݵ�ָ�룬 ���ڸú�����ȡNV�д���ڲ���NV������.
//           ulItemDataLen[in] pItemDataָ��buffer�ĳ��ȣ�Ŀǰ��󳤶�֧��1024���ֽ�
//
// �� �� ֵ: HI_ERR_SUCCESS                ��ʾдNV��ɹ�
//           HI_ERR_NV_ITEM_IS_EMPTY	    NV��Ϊ��.
//           HI_ERR_NV_NOT_SUPPORT_ID    	NV��ID������
//           HI_ERR_NV_NOT_SUPPORT_LENTH	NV��ID���ȳ���Ԥ�����ʵ�ʳ���
//           HI_ERR_NOT_SUPPORT	            ��֧��
//           ����������
//
// ����Ҫ��:
//           1) ��������ʹ�ã��������ж���ʹ��, ֧�ֶ��������
//           2) HI_NVM_Write��֧�ֹ���д��NV��,HI_NVM_WriteFactory��֧��д�빤��������
//*****************************************************************************
HI_EAPI HI_U32 HI_NVM_Write(HI_U16 usId, HI_CONST HI_PVOID pItemData, HI_U16 ulItemDataLen);
HI_EAPI HI_U32 HI_NVM_WriteFactory(HI_U16 usId, HI_CONST HI_PVOID pData,HI_U16 usDataLen);
HI_EAPI HI_VOID HI_NVIM_Update_Index(HI_VOID);

//*****************************************************************************
// ��������: HI_NVM_RestoreFactorySetting
//
// ��������: �ָ�ָ��section�ĵ�ǰ�汾��NV��ʼֵ
//
// ����˵��: ulSectionId[in]: ָ����section ID, ���Ϊ0 ��Ĭ�ϻָ����е�NV��
//
//����ֵ     : HI_ERR_SUCCESS            �����ɹ�
//             HI_ERR_MALLOC_FAILUE      �����ڴ�ʧ��
//             HI_ERR_INVALID_PARAMETER  ��ʼ�ļ�����
//             HI_ERR_NV_ERROR_READ      ��flashʧ��
//             ����������
//
// ����Ҫ��:
//           1) ��������ʹ�ã��������ж���ʹ��, ֧�ֶ��������
//*****************************************************************************
#if !defined(PRODUCT_CFG_CORE_TYPE_BOOTLOADER) && defined(PRODUCT_CFG_NVM_EXT_SUPPORT)
HI_EAPI HI_U32 HI_NVM_RestoreFactorySetting(HI_U32 ulSectionId);
#endif

//*****************************************************************************
// ��������: HI_NVM_RegisterChangedNoitfyProc
// ��������: NV����֪ͨ
//
// ����˵��:
//   usMinId [in] NV��ID��Сֵ
//   usMaxId [in] NV��ID���ֵ
//   pfnChanged [in] NV��ı�Ĵ�����, ��NV������ NVģ����Զ����ø�ע��Ľӿ�.
//
// �� �� ֵ:
//   HI_ERR_SUCCESS  ��ʾע��ɹ�
//   HI_ERR_NOT_FREEE_SPACE NVM�ڲ��ռ䲻��, �޷�ע��
//   ����ֵΪ������
//
// ����Ҫ��: ��SAL��ʼ��������ſ��Ե��øýӿ�.
// ���þ���:
//          HI_U32 MyNvChangedProc(HI_U16 ulId) { .... }
//          HI_NVM_RegisterChangedNoitfyProc(0x50, 0x100, MyNvChangedProc)
// ��    ��: shenhankun/00130424 [2011-12-12]
//*****************************************************************************
#if !defined(PRODUCT_CFG_CORE_TYPE_BOOTLOADER)
HI_EAPI HI_U32 HI_NVM_RegisterChangedNoitfyProc(HI_U16 usMinId, HI_U16 usMaxId, HI_NVM_CHANGED_NOTIFY_CALLBACK_F pfnChanged);
#if defined(PRODUCT_CFG_SAL_API_SUPPORT)
#define HI_NVM_RegisterChangedNoitfyProc(usMinId, usMaxId, pfnChanged) HI_NVM_RegisterChangedNoitfyProcPrv(usMinId,usMaxId,pfnChanged)
#endif
#endif
//*****************************************************************************

//*****************************************************************************
// DMS Interface
//*****************************************************************************

//*****************************************************************************
// ��������: HI_DMS_GetProductId HI_DMS_GetLocalMacAddr  HI_DMS_GetChipType HI_DMS_GetProductType
// ��������: ��ȡͨ��ģ��Ĳ�ƷID, ����MAC��ַ��оƬ���͡���Ʒ��̬����Ϣ
//           HI_DMS_GetLocalMacAddr HI_DMS_GetChipType HI_DMS_GetProductType ���� DMS_GetProductId�ķ�װ�ӿڣ�����
//           HI_DMS_GetLocalMacAddr ���ڻ�ȡͨ��ģ���MAC��ַ��
//           DMS_GetChipType���ڻ�ȡͨ��ģ���оƬ���ͣ�
//           DMS_GetProductType���ڻ�ȡͨ��ģ��Ĳ�Ʒ��̬
//
// ����˵��:
//   ucPlcMac[HI_PLC_MAC_ADDR_LEN] [out] ͨ��ģ���MAC��ַ
//   penChipType [out] ͨ��ģ������оƬ����, ����Ҫ�ò�������HI_NULL
//   penProductType [out] ͨ��ģ��Ķ��ƵĲ�Ʒ��̬, ����Ҫ�ò�������HI_NULL
//   penHwPlatType [out] ͨ��ģ���Ӳ��ƽ̨���ͣ�����Ҫ�ò�������HI_NULL
//
// �� �� ֵ:
//    HI_ERR_SUCCESS : �ɹ�
//    ����ֵ����
//
// ����Ҫ��: DSM��ʼ���󣬼��ɵ���
// ��    ��: shenhankun/00130424 [2011-10-26]
//*****************************************************************************
HI_EAPI HI_U32 HI_DMS_GetProductId(HI_OUT HI_U8 ucPlcMac[HI_PLC_MAC_ADDR_LEN], HI_OUT HI_FTM_CHIP_TYPE_E* penChipType, HI_OUT HI_FTM_PRODUCT_TYPE_E* penProductType, HI_OUT HI_FTM_HW_PLATFORM_TYPE_E* penHwPlatType);
HI_EAPI HI_U32 HI_DMS_GetLocalMacAddr(HI_U8 ucMac[HI_PLC_MAC_ADDR_LEN]); // ��ȡ���ص�ETH MAC��ַ
HI_EAPI HI_FTM_CHIP_TYPE_E HI_DMS_GetChipType(HI_VOID); // ��ȡоƬ����
HI_EAPI HI_FTM_PRODUCT_TYPE_E HI_DMS_GetProductType(HI_VOID); // ��ȡ��Ʒ����
HI_EAPI HI_FTM_HW_PLATFORM_TYPE_E HI_DMS_GetHwType(HI_VOID); // ��ȡӲ��ƽ̨����
HI_EAPI HI_FTM_CHIP_TYPE_E HI_DMS_GetChipTypeEx(HI_VOID); // �����ȡ�����Ϸ���оƬ���ͣ���Ĭ��Ϊ STA
HI_EAPI HI_FTM_PRODUCT_TYPE_E HI_DMS_GetProductTypeEx(HI_VOID); // �����ȡ�����Ϸ��Ĳ�Ʒ��̬����Ĭ��Ϊ ���


// HI_DMS_GetProductId �ľ���汾�ӿ�
HI_EAPI HI_BOOL HI_DMS_IsProductCon(HI_FTM_PRODUCT_TYPE_E enProductType);     // �Ƿ�Ϊ�������ز�ģ��
HI_EAPI HI_BOOL HI_DMS_IsProductRelay(HI_FTM_PRODUCT_TYPE_E enProductType);   // �Ƿ�Ϊ�м���
HI_EAPI HI_BOOL HI_DMS_IsProductMeter(HI_FTM_PRODUCT_TYPE_E enProductType);  // �Ƿ�Ϊ���
HI_EAPI HI_BOOL HI_DMS_IsProductMeterI(HI_FTM_PRODUCT_TYPE_E enProductType);  // �Ƿ�ΪI�Ͳɼ���
HI_EAPI HI_BOOL HI_DMS_IsProductMeterII(HI_FTM_PRODUCT_TYPE_E enProductType); // �Ƿ�Ϊ II�Ͳɼ���
HI_EAPI HI_BOOL HI_DMS_IsProductNdm(HI_FTM_PRODUCT_TYPE_E enProductType);    // �Ƿ�Ϊ������
HI_EAPI HI_BOOL HI_DMS_IsProductTsta(HI_FTM_PRODUCT_TYPE_E enProductType);    // �Ƿ�Ϊ�����ģ��
HI_EAPI HI_BOOL HI_DMS_IsInvalidProductType(HI_FTM_PRODUCT_TYPE_E enProductType);
HI_EAPI HI_BOOL HI_DMS_IsChipCco(HI_FTM_CHIP_TYPE_E enChipType); // �Ƿ�Ϊͷ���ز�ģ��
HI_EAPI HI_BOOL HI_DMS_IsChipSta(HI_FTM_CHIP_TYPE_E enChipType);  // �Ƿ�Ϊ�ն��ز�ģ��
HI_EAPI HI_U8 HI_DMS_LoadDevType(HI_VOID);
HI_EAPI HI_BOOL SAL_IsMeterCheckProduct(HI_VOID);
HI_EAPI HI_FTM_CHIP_TYPE_E HI_COM_ReadProductTypeFromHw(HI_U32 asic_ver);
HI_EAPI HI_FTM_PRODUCT_TYPE_E HI_COM_ReadChipTypeFromHw(HI_U32 pro_ver,HI_FTM_CHIP_TYPE_E chiType);
HI_EAPI HI_BOOL SAL_IsUstaProduct(HI_VOID);

// correct the spelling error

#define HI_IS_PRODUCT_CON()      HI_DMS_IsProductCon(HI_DMS_GetProductType())      // �Ƿ�Ϊ������
#define HI_IS_PRODUCT_METER()    HI_DMS_IsProductMeter(HI_DMS_GetProductType())   // �Ƿ�Ϊ���
#define HI_IS_PRODUCT_METERI()   HI_DMS_IsProductMeterI(HI_DMS_GetProductType())   // �Ƿ�ΪI�Ͳɼ���
#define HI_IS_PRODUCT_METERII()  HI_DMS_IsProductMeterII(HI_DMS_GetProductType())  // �Ƿ�Ϊ II�Ͳɼ���
#define HI_IS_PRODUCT_NDM()      HI_DMS_IsProductNdm(HI_DMS_GetProductType())     // �Ƿ�Ϊ������
#define HI_IS_PRODUCT_RELAY()    HI_DMS_IsProductRelay(HI_DMS_GetProductType())    // �Ƿ�Ϊ�м���
#define HI_IS_PRODUCT_Tsta()     HI_DMS_IsProductTsta(HI_DMS_GetProductType())    // �Ƿ�Ϊ�����ģ��

#define HI_IS_CCO_CHIP()         HI_DMS_IsChipCco(HI_DMS_GetChipType()) // �Ƿ�Ϊ CCO оƬ
#define HI_IS_STA_CHIP()         HI_DMS_IsChipSta(HI_DMS_GetChipType()) // �Ƿ�Ϊ STA оƬ

HI_EAPI HI_U32 HI_DMS_GetNetCfg(HI_U8 ucPlcMac[HI_PLC_MAC_ADDR_LEN], HI_U8 ucEthMac[HI_PLC_MAC_ADDR_LEN], HI_U8 ucEthIp[HI_IPV4_ADDR_LEN],HI_U8 ucEthMask[HI_IPV4_ADDR_LEN]);
//*****************************************************************************

//*****************************************************************************
// ��������: HI_DMS_SetForwarding
// ��������: ���ö˿� ת�ݹ���
//
// ����˵��:
//   usPortNum[in] ȡֵΪ HI_DMS_CHL_UART_PORT_APP �� HI_DMS_CHL_UART_PORT_APP�˿� ��ת�ݵ� HI_DMS_CHL_UART_PORT_DIAG�˿�
//   bForwarding[in] HI_TRUE: ���� ת��; HI_FALSE: ȡ��
//
// �� �� ֵ:
//    HI_ERR_SUCCESS : �ɹ�
//    ����ֵ����
//
// ����Ҫ��: ��ʼ���󣬼��ɵ���.
//           HI_DMS_SetForwarding(HI_DMS_CHL_UART_PORT_APP, HI_TRUE); // UART_APP-->UART_DIAG ->enable
//           HI_DMS_SetForwarding(HI_DMS_CHL_UART_PORT_APP, HI_FAlSE); // UART_APP-->UART_DIAG ->disable
// ��    ��: shenhankun/00130424 [2011-10-26]
//*****************************************************************************
HI_EAPI HI_U32 HI_DMS_SetForwarding(HI_U16 usPortNum, HI_BOOL bForwarding);
#if defined(PRODUCT_CFG_SAL_API_SUPPORT)
#if defined(PRODUCT_CFG_FEATURE_DMS_FORWARDING)
#define HI_DMS_SetForwarding(usPortNum, bForwarding) HI_DMS_SetForwardingPrv(usPortNum, bForwarding&0x1)
#endif
#endif
HI_EAPI HI_BOOL HI_DMS_IsForwarding(HI_U16 usPortNum);
#if defined(PRODUCT_CFG_SAL_API_SUPPORT)
#if defined(PRODUCT_CFG_FEATURE_DMS_FORWARDING)
#define HI_DMS_IsForwarding(usPortNum) HI_DMS_IsForwardingPrv(usPortNum)
#endif
#endif
//*****************************************************************************


//*****************************************************************************
// ��������:  HI_DMS_ChlSetup
//
// ��������: ͨ��ע��
//
// ����˵��:
//   pstChlPort [in] ͨ���˿ڶ���, HI_NULL���ʾ�ر�ָ���˿�
//   usPortNum [in] ͨ���˿�
//               HI_DMS_CHL_MAC_PORT_APP     : ��ʾMACͨ��
//               HI_DMS_CHL_MAC_PORT_DIAG    : ��ʾMACͨ��
//               HI_DMS_CHL_UART_PORT_APP    : ��ʾUART��MRͨ��
//               HI_DMS_CHL_UART_PORT_IR     : ��ʾIRͨ��
//               HI_DMS_CHL_TCP_PORT_APP     : ��ʾTCP��MRͨ��
//               HI_DMS_CHL_TCP_PORT_DIAG    : ��ʾTCP��DIAGͨ��
//               HI_DMS_CHL_UART_PORT_DIAG   : ��ʾUART��DIAGͨ��
//   usInstanceId [in] ͨ���˿�ʵ��, ȡֵ��Χ[0,1], ȱʧΪ0.
//   pstNvDevCfg [in] ��Ϊ�ձ�ʾʹ���û�����, HI_NULL���ʾ��NV�ж�ȡ��������ͨ��
//
// �� �� ֵ:
//    HI_ERR_SUCCESS : �ɹ�
//    ����ֵ����
// ����Ҫ��:
//    ʹ��Ĭ������: HI_DMS_ChlSetup
//    ����Ĭ�����ò���: Change NV item(HI_NV_FTM_DEVICE_CFG_ID) --> HI_DMS_ChlRefresh --> HI_DMS_ChlSetup
// ��    ��: shenhankun/00130424 [2011-10-26]
//*****************************************************************************
HI_EAPI HI_U32 HI_DMS_ChlSetup(HI_CONST HI_DMS_CHL_PORT_OBJ_S* pstChlPort, HI_U16 usPortNum);
#if defined(PRODUCT_CFG_SAL_API_SUPPORT)
#define HI_DMS_ChlSetup(pstChlPort, usPortNum) HI_DMS_ChlSetupInstance(pstChlPort, usPortNum, 0)
#endif
HI_EAPI HI_U32 HI_DMS_ChlRefresh(HI_DMS_NV_FTM_DEVICE_CFG_S* pstDevCfg);
//*****************************************************************************
// ��������: HI_DMS_ChlUartPortRefresh
// ��������: ͨ���ýӿڿ����޸Ĵ��ڳ�ʼ������
//
// ����˵��:
//   pstDevCfg[in]: ��Ҫ���õ�ֵ
//   chipType[in]:  оƬ����
//   usportNum[in]: �˿ں�
//
// �� �� ֵ:
//    HI_ERR_SUCCESS : �ɹ�
//    ����ֵ����
//
// ����Ҫ��:
//
//*****************************************************************************
HI_U32 HI_DMS_ChlUartPortRefresh(HI_FTM_DEVICE_UART_CFG_S* pstDevCfg, HI_FTM_CHIP_TYPE_E chipType, HI_U16 usportNum);

//*****************************************************************************


//*****************************************************************************
// ��������: HI_DMS_ChlTxFrm HI_DMS_ChlTxMsg
// ��������: ͨ����������֡/��Ϣ��֧��PLCͨ����UART��TCPͨ��
//
// ����˵��:
//   usPortNum [in] ͨ���˿�
//               HI_DMS_CHL_MAC_PORT_APP    : ��ʾMACͨ��
//               HI_DMS_CHL_UART_PORT_IR    : ��ʾIRͨ��
//               HI_DMS_CHL_MAC_PORT_DIAG   : ��ʾMACͨ��
//               HI_DMS_CHL_UART_PORT_APP   : ��ʾUART��MRͨ��
//               HI_DMS_CHL_TCP_PORT_APP    : ��ʾTCP��MRͨ��
//               HI_DMS_CHL_TCP_PORT_DIAG   : ��ʾTCP��DIAGͨ��
//               HI_DMS_CHL_UART_PORT_DIAG  : ��ʾUART��DIAGͨ��
//
//   pFrameData [in] DMS��֡���ݽṹ
//   pFrameCtrl [in] DMS��֡������Ϣ, ȡֵΪHI_NULL��ʾ�������Ӵ�����ETH�����ڵȣ�
//   ulMsgId [in] ��ϢID, ������� HI_CHL_MSG_MIN
//
// �� �� ֵ:
//    HI_ERR_SUCCESS           : �ɹ�
//    HI_ERR_NOT_CONNECT       : ͨ��û������, ����ʧ��
//    HI_ERR_INVALID_PARAMETER : ������, ��������
//    ����ֵΪ������
//
// ����Ҫ��: ��ʼ��ʱ���ȵ���HI_DMS_ChlSetup ����ע�ᰲװ�������ʹ��HI_DMS_ChlTx��������
// ��    ��: shenhankun/00130424 [2011-10-25]
//*****************************************************************************
#ifndef HI_HAVE_CROSS_COMPILER_ARM_GCC
HI_EAPI HI_U32 HI_DMS_ChlTx(HI_U16 usPortNum, HI_IN HI_DMS_CHL_TX_DATA_S* pFrameData, HI_IN HI_DMS_CHL_TX_CTRL_S* pFrameCtrl);
#endif
//*****************************************************************************


//*****************************************************************************
// ��������: HI_DMS_ChlCtrl
// ��������: ͨ�����úͲ�ѯ
//
// ����˵��:
//   usPortNum[in] HI_DMS_CHL_MAC_PORT_UPG HI_DMS_CHL_MAC_PORT_APP HI_DMS_CHL_UART_PORT_APP
//   enOpt[in] HI_DMS_PORT_CTRL_OFFLINE_ONLINE_RX : ͨ�����ܽ�������(����PLC��������δ���)
//             HI_DMS_PORT_CTRL_ONLINE_RX         : ͨ������������ɺ��ܽ�������
//             HI_DMS_PORT_CTRL_NO_DELAY          : ͨ������ʱ����(��������TCP�˿�)
//             HI_DMS_PORT_CTRL_RX_FRM_TO         : timeout in seconds,  pInputParam = time out value; pOutputParam=0
//			   �Ⱦ���ο� HI_DMS_PORT_CTRL_OPT_ID_E
//   pInputParam[in] enOpt ָ�����������
//   pOutputParam[in] enOpt ָ�����������
//
// �� �� ֵ:
//    HI_ERR_SUCCESS : �ɹ�
//    ����ֵ����
//
// ����Ҫ��: ��ʼ���󣬼��ɵ���.
// ��    ��: shenhankun/00130424 [2011-10-26]
//*****************************************************************************
#ifndef HI_HAVE_CROSS_COMPILER_ARM_GCC
HI_EAPI HI_U32 HI_DMS_ChlCtrl(HI_U16 usPortNum, HI_DMS_PORT_CTRL_OPT_ID_E enOpt, HI_IN HI_PVOID pInputParam, HI_OUT HI_PVOID pOutputParam);
#endif
//*****************************************************************************


//*****************************************************************************
#if defined(PRODUCT_CFG_SAL_API_SUPPORT)
#define HI_DMS_ChlTx(usPortNum, pFrameData, pFrameCtrl) HI_DMS_ChlTxFrm(usPortNum, pFrameData, pFrameCtrl)
#define HI_DMS_ChlCtrl(usPortNum, enCtrlId, pInputParam, pOutputParam) HI_DMS_ChlSendMessage(usPortNum, enCtrlId, pInputParam, pOutputParam)
#endif
//*****************************************************************************


//*****************************************************************************
// DIAG Interface
//*****************************************************************************




//*****************************************************************************
// ��������: HI_DIAG_ReportEvent
// ��������: Report the event log to 'Event Viewer' on PC side.
// ����˵��:
//           ulEvtId [in] Specify the event identifier.
//           ucType [in] �ϱ���HSO�����ȡֵ����
//           	 HI_DIAG_TYPE_A(0) ��ʾHSO���з�����汾
// 	             HI_DIAG_TYPE_B(1) ��ʾHSO���ֳ�ά���汾
// �� �� ֵ:
//           HI_ERR_SUCCESS           ��ʾ�ɹ�
//           HI_ERR_QUEUE_FULL        ��ʾ������
//           HI_ERR_NO_INITILIZATION  ��ʾ����δ��ʼ��
//           HI_ERR_FAILURE           ��ʾ����δ֪����
//           HI_ERR_CFG_NOT_ALLOW ��־�������ϱ�
//           HI_ERR_INVALID_PARAMETER ���ηǷ�
//           HI_ERR_NOT_ENOUGH_MEMORY �ڴ����ʧ��
// ����Ҫ��:
// ���þ���: HI_DIAG_ReportEvent(LOG_EVT_SHUT_DOWN)
// ��    ��: ���ɸ�/00149569/20090210
//*****************************************************************************
HI_EAPI HI_U32 HI_DIAG_ReportEvent(HI_U16 usEvtId, HI_U8 ucType);


//*****************************************************************************
// ��������: HI_DIAG_ReportAirMessage
// ��������:
//           Report the protocol air message to 'Air Message Viewer' on PC side
// ����˵��:
//           pRptMessage [in] Pointer to DIAG_AIR_MSG_LOG_STRU structure which hold the air message.
//           ucType [in] �ϱ���HSO�����ȡֵ����
//           	 HI_DIAG_TYPE_A(0) ��ʾHSO���з�����汾
// 	             HI_DIAG_TYPE_B(1) ��ʾHSO���ֳ�ά���汾
// �� �� ֵ:
//           HI_ERR_SUCCESS           ��ʾ�ɹ�
//           HI_ERR_NO_INITILIZATION  �豸δ��ʼ��
//           HI_ERR_QUEUE_FULL        OS������������ʧ��
//           HI_ERR_FAILURE           δ֪����
//           HI_ERR_CFG_NOT_ALLOW ��־�������ϱ�
//           HI_ERR_INVALID_PARAMETER ���ηǷ�
//           HI_ERR_NOT_ENOUGH_MEMORY �ڴ����ʧ��
// ����Ҫ��:
// ���þ���:
//           1.��pData�ǵ����߶�̬ʹ���ڴ���������:
//               pRptMessage.ulDataSize
//           2.��pDataΪ�����߶�̬ʹ���ڴ���������:
//               MSP_SET_HEAP(pRptMessage.ulDataSize)
//           3. HI_DIAG_ReportAirMessagePrv Ϊ�ڲ��ӿ�,��SALģ�鲻��ֱ�ӵ���
// ��    ��: ���ɸ�/00149569/20090210
//*****************************************************************************
#if defined(PRODUCT_CFG_FEATURE_DIAG_AIRMSG)
HI_EAPI HI_U32 HI_DIAG_ReportAirMessage(HI_DIAG_AIR_MSG_S* pRptMessage, HI_U8 ucType);
#if defined(PRODUCT_CFG_SAL_API_SUPPORT)
#define HI_DIAG_ReportAirMessage(pRptMessage, ucType) HI_DIAG_ReportAirMessagePrv(pRptMessage, ucType)
#endif
#endif

//*****************************************************************************
// ��������: HI_DIAG_ReportLayerMessage
// ��������:
//           Report the layer message to 'Layer Message Viewer' on PC side.
//           The function to install by OS HOOK.
// ����˵��:
//           pRptMessage [in]
//             Pointer to HI_DIAG_LAYER_MSG_S structure which hold the layer message.
//           ucType [in]
//             �ϱ���HSO�����ȡֵ����
//           	 HI_DIAG_TYPE_A(0) ��ʾHSO���з�����汾
// 	             HI_DIAG_TYPE_B(1) ��ʾHSO���ֳ�ά���汾
// �� �� ֵ:
//           HI_ERR_SUCCESS           ��ʾ�ɹ�
//           HI_ERR_NO_INITILIZATION  �豸δ��ʼ��
//           HI_ERR_QUEUE_FULL        OS������������ʧ��
//           HI_ERR_FAILURE           δ֪����
//           HI_ERR_CFG_NOT_ALLOW ��־�������ϱ�
//           HI_ERR_INVALID_PARAMETER ���ηǷ�
//           HI_ERR_NOT_ENOUGH_MEMORY �ڴ����ʧ��
// ����Ҫ��: 1) ���pRptMessage.pDataΪʹ���߶�̬�����ָ��,��ú����˳���,��ʹ�����ͷŸ�ָ��
// ���þ���: TODO ...
// ��    ��: ���ɸ�/00149569/20090210
//*****************************************************************************
HI_EAPI HI_U32 HI_DIAG_ReportLayerMessage(HI_DIAG_LAYER_MSG_S* pLayerMessage, HI_U8 ucType);
HI_EAPI HI_U32 HI_DIAG_ReportLayerMsg(HI_U16 usId, HI_U16 usSrcModId, HI_U16 usDestModId, HI_PVOID pData, HI_U16 usDataSize);
#if defined(PRODUCT_CFG_SAL_API_SUPPORT)
#define HI_DIAG_ReportLayerMessage(pLayerMessage, ucType) HI_DIAG_IS_CONNECT_CHK HI_DIAG_ReportLayerMessagePrv(pLayerMessage,ucType&HI_DIAG_TYPE_LLN)
#define HI_DIAG_ReportLayerMsg(usId, usSrcModId, usDestModId, pData, usDataSize) HI_DIAG_IS_CONNECT_CHK HI_DIAG_ReportLayerMsgPrv(usId, usSrcModId, usDestModId, pData, usDataSize)
#endif
//*****************************************************************************


//*****************************************************************************
// HI_DIAG_IsLayerMessageEnable
// DIAG �Ĳ����Ϣͨ�����, ����:
// 1) DIAGͨ���Ƿ�����; 2) �����Ϣ�����ÿ����Ƿ�ʹ��;
//*****************************************************************************
HI_EAPI HI_BOOL HI_DIAG_IsLayerMsgEnable(HI_U16 usId, HI_U16 usSrcModId, HI_U16 usDestModId);
#if defined(PRODUCT_CFG_SAL_API_SUPPORT)
#define HI_DIAG_IsLayerMsgEnable(usId, usSrcModId, usDestModId)  HI_DIAG_IsLayerMessageEnablePrv(usId, usSrcModId, usDestModId, HI_DIAG_TYPE_A)
#endif

#if defined(PRODUCT_CFG_FEATURE_DIAG_USERPLANE)
HI_EAPI HI_BOOL HI_DIAG_IsLogMsgEnable(HI_U32 ulModId, HI_U32 ulId);

#if defined(PRODUCT_CFG_SAL_API_SUPPORT)
#define HI_DIAG_IsLogMsgEnable(ulModId, ulId) ((HI_BOOL)(!HI_DIAG_IsUserPlaneMessageEnablePrv((HI_U32)ulModId, ulId)))
#endif
#endif


//*****************************************************************************


//*****************************************************************************
// ��������: HI_DIAG_ReportCommand HI_DIAG_IsCommandReportEnalbe
// ��������:
//           �ϱ�DIAG����HSO��, ����������, ֧���ж�ʹ��.
// ����˵��:
//           ulID       [in] COMMAND ID
//           usDataSize [in] pData Size�����Ϊ512�ֽ�
//           pData      [in]  Pointer to statistic info structure
// �� �� ֵ:
//           HI_ERR_SUCCESS           ��ʾ�ɹ�
//           HI_ERR_NO_INITILIZATION  �豸δ��ʼ��
//           HI_ERR_NOT_CONNECT       ͨ��δ����
//           HI_ERR_QUEUE_FULL        OS������������ʧ��
//           HI_ERR_FAILURE           δ֪����
//           HI_ERR_CFG_NOT_ALLOW     ���ÿ��ز������ϱ�
//           HI_ERR_INVALID_PARAMETER ���ηǷ�
//           HI_ERR_TOO_SMALL_BUFFER  �ڲ����治�㣬�������������usDataSize������, ���� HI_DIAG_BUFFERING_CMD_MAX_LEN
// ����Ҫ��: 1) ���pData���ɵ����߶�̬�����ָ�룬���ɵ����߸����ͷŸ�ָ��.
//           2) HI_DIAG_ReportCommand �����ϱ�, �����Ҫ�ж��Ƿ�ر������ϱ�����ʹ�� HI_DIAG_IsCommandReportEnalbe
//           3) һ�����������첽Ӧ��ĳ���
// ���þ���:
// ��    ��: ���ɸ�/00149569/20100225
//*****************************************************************************
HI_EAPI HI_U32 HI_DIAG_ReportCommand(HI_U16 usId, HI_U16 usDataSize, HI_PVOID pData);
HI_EAPI HI_BOOL HI_DIAG_IsCommandReportEnalbe(HI_VOID);
#if defined(PRODUCT_CFG_SAL_API_SUPPORT)
#define HI_DIAG_ReportCommand(usId, usDataSize, pData) HI_DIAG_ReportCommandPrv(usId,usDataSize,pData);

#define HI_DIAG_IsCommandReportEnalbe()  HI_DIAG_IsCommandReportEnalbePrv()
#endif

//*****************************************************************************
// ��������: HI_DIAG_LocalReportCommand
// ��������:
//           �ϱ��������FLash��
// ����˵��:
//           ulID       [in] COMMAND ID
//           ulDataSize [in] pData Size
//           pData      [in] Pointer to statistic info structure
// �� �� ֵ:
//           HI_ERR_SUCCESS           ��ʾ�ɹ�
//           HI_ERR_QUEUE_FULL        OS������������ʧ��
//           HI_ERR_INVALID_PARAMETER ���ηǷ�
//           HI_ERR_CFG_NOT_ALLOW     ���ÿ���û�д�(Ĭ�����ÿ��ش򿪣������ϱ�)
//           HI_ERR_NOT_SUPPORT       �ð汾��֧��
//           ����������
// ����Ҫ��: 1) HSO��������ֱ���ϱ���HSO, ���򱣴浽Flash.
//           2) �����߶�̬������ڴ����ɵ������ͷ�
// ���þ���:
// ��    ��: ���ɸ�/00149569/20100225
//*****************************************************************************
HI_EAPI HI_U32 HI_DIAG_LocalReportCommand(HI_U16 usId, HI_U16 usDataSize, HI_PVOID pData);


//*****************************************************************************
// ��������: HI_DIAG_EnableCommandReport
// ��������:
//           �ϱ�����ʹ��
// ����˵��:
//           bEnable  [in] HI_TRUE: ǿ�������ϱ������; HI_FALSE: �ر��ϱ�
//           ��
// �� �� ֵ:
//           HI_ERR_SUCCESS           ��ʾ�ɹ�
// ����Ҫ��: 1) �������ж��е���,֧�������е���
// ���þ���:
// ��    ��: ���ɸ�/00149569/20100225
//*****************************************************************************
HI_EAPI HI_U32 HI_DIAG_EnableCommandReport(HI_BOOL bEnable);

//*****************************************************************************
// ��������: HI_DIAG_RegisterCommand
// ��������: ע���������
//
// ����˵��:
//   pstCmdTbl [in] �����,��������Ϊ�������鴫���ò���.
//   usCmdNum [in] �������, ��� usCmdNum Ϊ��, �� pstCmdTbl��Ϊ����ȡ��ע��.
//   ucInstanceId [in] ʵ��ID, ȡֵ����.
//                     HI_DIAG_CMD_INSTANCE_DEFAULT ��ʾע�᱾�ش��������, SALֱ�ӵ��ø�ע���HSO��������.
//                     HI_DIAG_CMD_INSTANCE_IREMOTE ��ʾע���Ӵ����Զ������, ����·��, �Ҿ���MAC͸�������Զ������,SAL�ٵ��ø�ע���HSO��������.
//                     HI_DIAG_CMD_INSTANCE_DREMOTE ��ʾע��ֱ�Ӵ����Զ������, ��·��,ͨ��·���ж��Ƿ���Ҫ͸��Զ������
//
// �� �� ֵ:
//    HI_ERR_SUCCESS : ����ɹ�, DIAG�����ظ�HSO,������ΪOK
//    ����Ϊ������
//
// ����Ҫ��: 1. �ڳ�ʼ���׶ε���, ��֧�ֶ��������.
//           2. �����ע��20����ͬ�������.
//           3. �����������ͬ��������, ���Ǵ��е�, ��Ҫ��ע����������������ʱ�価����, ���ܶ��������������, Ŀǰ���ô���ʱΪ5s.
//           4. ��ͬʵ��IDע������ID������ͬ, ͬһʵ��ע�������ID����ΪΨһ,����ע��ʧ��.
// ���þ���:
//         �ο� <<Hi3911V100 SAL�ӿ�˵����.docx>>
// ��    ��: shenhankun/00130424 [2011-11-14]
//*****************************************************************************
HI_PRVL HI_U32 HI_DIAG_RegisterCommand(HI_CONST HI_DIAG_CMD_REG_OBJ_S* pstCmdTbl, HI_U16 usCmdNum, HI_U8 ucInstanceId)
{
    return HI_DIAG_RegisterCommandPrv(pstCmdTbl, usCmdNum, ucInstanceId);
}
#define HI_DIAG_RegiestCommand(pstCmdTbl, usCmdNum, ucInstanceId)    HI_DIAG_RegisterCommand(pstCmdTbl, usCmdNum, ucInstanceId)
#define HI_DIAG_RegiestInputCommand(pstCmdTbl, usCmdNum)             HI_DIAG_RegisterCommand(pstCmdTbl, usCmdNum, HI_DIAG_CMD_INSTANCE_DEFAULT)
//*****************************************************************************


//*****************************************************************************
// ��������: HI_DIAG_RegisterParallelCmd
// ��������: ע�����в���������ID��Χ,
//           Ĭ�����������֧�ִ�����ǰһ������û��ִ�����(ACKû�л�)���Զ�����ǰһ����������.
//
// ����˵��:
//   usMinId [in] ����ID��Сֵ
//   usMaxId [in] ����ID���ֵ
//
// �� �� ֵ:
//    HI_ERR_SUCCESS           : �ɹ�
//    HI_ERR_NOT_FOUND         : �б�����,�޷�ע��,������ע��4��.
//    HI_ERR_NO_INITILIZATION  : DIAGδ��ʼ��
//
// ����Ҫ��: 1) DIAG��ʼ����������Ч; 2)�������ڳ�ʼ�������е���
// ���þ���:
//         HI_PVOID GetParallelCmdCache(HI_U16 usId, HI_U32 ulCacheSize, HI_U16 usPkSn) { return HI_MALLOC(0, ulCacheSize); }
//         HI_DIAG_RegisterParallelCmd(0x5700, 0x5701, GetParallelCmdCache)
//         ��HI_DIAG_RegisterCommand ע��Ŀ��ƽӿ����ͷŸ��ڴ�
//         HI_PRV HI_CONST HI_DIAG_CMD_REG_OBJ_S g_stSimuCmdTbl[] = {
//         {ID_DIAG_CMD_CSIM_PACKET, ID_DIAG_CMD_SIMU_CTRL, CmdInputProc, CmdCtrlProc},
//         HI_PRV HI_U32 CmdInputProc(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption) {DIAG_CMD_CSIM_REQ_STRU* pstReq = (DIAG_CMD_CSIM_REQ_STRU*)pCmdParam;}
//         HI_PRV HI_U32 CmdCtrlProc(HI_U16 ulId, HI_U16 usPkSn, HI_PVOID pCache, HI_U32 ulOption){ HI_MDM_Free(0, pCache);}
// ��    ��: s00130424/s00130424 [2013-01-25]
//*****************************************************************************
HI_PRVL HI_U32 HI_DIAG_RegisterParallelCmd(HI_U16 usMinId, HI_U16 usMaxId, HI_DIAG_GET_CMD_PK_CACHE_F pfnGetPkCache)
{
    HI_ASSERT(pfnGetPkCache);
#if defined(PRODUCT_CFG_FEATURE_REG_PARALLEL_CMD)
    return  HI_DIAG_RegisterParallelCmdPrv(usMinId, usMaxId, pfnGetPkCache);
#else
    return HI_ERR_NOT_SUPPORT;
#endif
}

#if defined(PRODUCT_CFG_DIAG_SUPPORT_LOG_RECORD)
HI_EAPI HI_U32 HI_DIAG_RegistDiagCmdNotify(HI_U32 ulId, HI_DIAG_CMD_NOTIFY_F * func);
#define HI_DIAG_RegistDiagCmdNotify(ulId,func) HI_DIAG_RegistDiagCmdNotifyPrv(ulId,func)
#else
HI_PRVL HI_U32 HI_DIAG_RegistDiagCmdNotify(HI_U32 ulId, HI_DIAG_CMD_NOTIFY_F * func){return HI_ERR_NOT_SUPPORT;}
#endif
HI_EAPI HI_U32 HI_DIAG_RunCmd(HI_U32 cmdId,HI_U32 ulOption,HI_PVOID pData,HI_U16 ulDataSize);

//*****************************************************************************
#if defined(PRODUCT_CFG_FEATURE_LOAD_FILE)
// ulLoadId          : should be less than HI_LOAD_FILE_PROC_ID_MAX, you can combine exteral option(HI_DIAG_LD_FILE_OPT_TIMEOUT)
// pfnLoadFileNotify : the call-back process after load file done, input: id,file_buffer,buffer_size,error_code
// pFileBuffer       : File Buffer
// ulFileBufferSize  : File Buffer size in Bytes
HI_EAPI HI_U32 HI_DIAG_RegisterLoadFileNoitfyProc(HI_U32 ulLoadId, HI_LD_FILE_CALLBACK_F pfnLoadFileNotify, HI_PVOID pFileBuffer, HI_U32 ulFileBufferSize);
#endif
HI_EAPI HI_U32 HI_DIAG_SetupLoadFileNoitfyProc(HI_U32 ulLoadId, HI_LD_FILE_CALLBACK_F pfnLoadFileNotify, HI_PVOID pFileBuffer, HI_U32 ulFileBufferSize);
#define HI_DIAG_SetupLoadFileNoitfyProc HI_DIAG_RegisterLoadFileNoitfyProc
//*****************************************************************************

//*****************************************************************************
// ��������: HI_DIAG_SendAckPacket HI_DIAG_SendUserAckPacket
// ��������: ����Ӧ�����HSO
//
// ����˵��:
//   ulId [in] DIAG����ID
//   pBuffer [in] buffer��ַ
//   usBufferSize [in] buffer��С
//   bSync [in] TRUE_T��ʾͬ�����͸�HSO, ��������; FALSE_T��ʾ�첽(ͨ�������ڴ��, ��OS���л����ڷ��͸�HSO),����������
//   hAckInstance [in] ͨ�� HI_DIAG_CreateAckInstance �ӿ� ������ACK��ʵ��, �粻��Ҫ��ò���ΪHI_NULL.
//
// �� �� ֵ:
//    HI_ERR_SUCCESS : ���ͳɹ�
//    ����Ϊ������
//
// ����Ҫ��:
//         1. HI_DIAG_SendAckPacket ��֧���� HI_DIAG_RegiestCommand ע��� ��������������ʹ��
//           ���������������ʹ��(�����Զ����������)������Ҫ������������ȴ���ACK��ʵ��������� HI_DIAG_SendUserAckPacket �� HSO.
//         2. ��֧�ֱ��ط���Ӧ���.
// ���þ���: TODO: ...
// ��    ��: shenhankun/00130424 [2011-11-29]
//*****************************************************************************
HI_EAPI HI_U32 HI_DIAG_SendAckPacket(HI_U32 ulId, HI_IN HI_PVOID pBuffer, HI_U16 usBufferSize, HI_BOOL bSync);
#if defined(PRODUCT_CFG_SAL_API_SUPPORT)
#define HI_DIAG_SendAckPacket(ulId, pBuffer, usBufferSize, bSync) HI_DIAG_SendAckPacketPrv(ulId, pBuffer, usBufferSize, bSync, HI_NULL)
#endif
//*****************************************************************************

//*****************************************************************************
// ��������: HI_DIAG_CreateAckInstance
// ��������: ����ACK��ʵ��
//
// ����˵��:
//   ulId [in] DIAG��ID, �����HI_DIAG_RegiestInputCommandע�����������������Ӧ��IDһ��.
//
// �� �� ֵ:
//    �ǿ�: ACK��ʵ��, ���ڴ��� HI_DIAG_SendUserAckPacket �ӿڲ��� hAckInstance
//    ��  : �ڴ治��/��������Ƿ�/û����HI_DIAG_RegiestInputCommandע�����������������ʹ��
//
// ����Ҫ��: ���� HI_DIAG_RegiestInputCommand ע��� ��������������ʹ��, ���򷵻� HI_NULL.
// ���þ���: TODO: ...
// ��    ��: shenhankun/00130424 [2011-11-29]
//*****************************************************************************
HI_EAPI HI_HANDLE HI_DIAG_CreateAckInstance(HI_U32 ulId);

//*****************************************************************************
// ��������: HI_DIAG_ReportPacket HI_DIAG_SendUserPacket
// ��������: �������ݰ���HSO
//
// ����˵��:
//   ulId [in] ��ID
//   pucPacket [in] ���ݰ���buffer��ַ, �ú��������ͷŸ�ָ��,�������Ҫ����ֵΪHI_NULL
//   usPacketSize [in] ���ݰ���С, �������Ҫ����ֵΪ0
//   bSync [in] HI_TRUE:��ͨ��OS���з��Ͱ�, �������� HI_DIAG_RegiestInputCommand ע��Ĵ���ӿ���;
//              HI_FASE:ͨ��OS���з��Ͱ�, Ԥ����Ļ���(COMMAND Buffer����)ʱ�����Զ����ö�̬���䷽ʽ.
//   ucInstanceId [in] ʵ��ID,HI_DIAG_CMD_INSTANCE_DEFAULT ��ʾ��������,SALֱ�ӵ��ø�ע���HSO��������
//                     HI_DIAG_CMD_INSTANCE_IREMOTE ��ʾԶ������, ����MAC͸����,SAL�ٵ��ø�ע���HSO��������
//
// �� �� ֵ:
//    HI_ERR_SUCCESS: �ɹ�
//    ����ֵ��ʾ������.
//
// ����Ҫ��: 1. HSOͨ������; 2) �����㹻;
//           3) û�����ÿ���, ��һֱ���ϱ�, �����Ҫ�ܿ�������,�������HI_DIAG_ReportCommand
//           4) HI_DIAG_SendUserPacket��֧�ַ��͵�OS����
//           5) ͨ��������Ƽ�ʹ�� HI_DIAG_SendUserPacket �ӿ�.
// ���þ���:
//  MY_PACKET_STRU stPacket = {0}
//  HI_DIAG_ReportPacket(0x1000, HI_DIAG_CMD_INSTANCE_LOCAL, &stPacket, sizeof(MY_PACKET_STRU), HI_FALSE)
//  HI_DIAG_SendUserPacket(0x1000, &stPacket, sizeof(MY_PACKET_STRU))
// ��    ��: shenhankun/00130424 [2012-01-04]
//*****************************************************************************
HI_EAPI HI_U32 HI_DIAG_ReportPacket(HI_U32 ulId, HI_U8 ucInstanceId, HI_PBYTE pucPacket, HI_U16 usPacketSize, HI_BOOL bSync);
HI_EAPI HI_U32 HI_DIAG_SendUserPacket(HI_U32 ulId, HI_PBYTE pucPacket, HI_U16 usPacketSize);
#if defined(PRODUCT_CFG_SAL_API_SUPPORT)
#define HI_DIAG_ReportPacket(ulId, ucInstanceId, pucPacket, usPacketSize, bSync) HI_DIAG_ReportPacketPrv(ulId, ucInstanceId, pucPacket, usPacketSize, bSync)
#define HI_DIAG_SendUserPacket(ulId, pucPacket, usPacketSize) HI_DIAG_ReportPacket(ulId, HI_DIAG_CMD_INSTANCE_LOCAL, pucPacket, usPacketSize, HI_FALSE)
#else
HI_PRVL HI_U32 HI_DIAG_ReportPacket(HI_U32 ulId, HI_U8 ucInstanceId, HI_PBYTE pucPacket, HI_U16 usPacketSize, HI_BOOL bSync) {return HI_ERR_NOT_SUPPORT;}
HI_PRVL HI_U32 HI_DIAG_SendUserPacket(HI_U32 ulId, HI_PBYTE pucPacket, HI_U16 usPacketSize) {return HI_ERR_NOT_SUPPORT;}
#endif
#define HI_DIAG_REPORT_TEXT(sz)  HI_DIAG_ReportPacket(HI_DBG_DIAG_TEXT, HI_DIAG_CMD_INSTANCE_LOCAL, sz, sizeof(sz)-1, HI_TRUE);
//*****************************************************************************

#if defined(PRODUCT_CFG_FEATURE_NDM)&& !defined(PRODUCT_CFG_FEATURE_UT)
//*****************************************************************************
// ��������: HI_DIAG_ReportStatObj
// ��������: �ϱ�ͳ������������, ��̨ת����HSO
//
// ����˵��:
//   ulId [in] ͳ���� ID, ȡ֮��Χ [HI_DFX_STAT_ID_BASE_SYS, HI_DFX_STAT_ID_MAX_SYS)
//   pStatObj [in] ͳ��������,ͨ��Ϊȫ�ֱ���
//   usStatObjSize [in] ͳ�������󳤶�
//
// �� �� ֵ:
//    HI_ERR_SUCCESS: �ɹ�
//    ����ֵ��ʾ������.
//
// ����Ҫ��:
// ���þ���:
// ��    ��: shenhankun/00130424 [2012-01-04]
//*****************************************************************************
HI_EAPI HI_U32 HI_DIAG_ReportStatObj(HI_U16 usId, HI_PVOID pStatObj, HI_U16 usStatObjSize);
#if defined(PRODUCT_CFG_SAL_API_SUPPORT)
#define HI_DIAG_ReportStatObj(usId, pStatObj, usStatObjSize) HI_DIAG_ReportStatObjPrv(usId, pStatObj, usStatObjSize)
#else
HI_PRVL HI_U32 HI_DIAG_ReportStatObj(HI_U16 usId, HI_PVOID pStatObj, HI_U16 usStatObjSize) {return HI_ERR_NOT_SUPPORT;}
#endif
#endif

//*****************************************************************************
// ��������: HI_DIAG_ReportXxxEvent
// ��������: ϵͳ��ʼ���¼��ϱ�ϵͳ����/�澯/�ؼ��¼��ϱ�
//           HI_DIAG_ReportSysInitEvt       : �����ӽ��ϱ����˿�(������ʽ�ϱ�, �������ж�/����)
//           HI_DIAG_ReportSysInitEvtRc     : �����ӽ��ϱ����˿�(������ʽ�ϱ�, �������ж�/����)
//           HI_DIAG_ReportSysEvt           : �����ӽ��ϱ����˿�(������ʽ�ϱ�, �������ж�/����)
//           HI_DIAG_ReportSysCriticalEvent : �޻���ֱ���ϱ����˿�(����ʽ�ϱ�, ����������)
// ����˵��:
//           usInitEvtId [in] ��ʾ�¼�ID, ȡֵ�޶�Ϊ HI_DEID_SYS_INIT_E �Ķ���
//           usSysEvtId [in] ��ʾ�¼�ID, ȡֵ�޶�Ϊ HI_DEID_SYS_E �Ķ���
//           ucOption [in] HI_DIAG_SYS_INIT_SET ��ʾ�����¼�, HI_DIAG_SYS_INIT_RESET ��ʾ����¼�, Ĭ��ΪHI_DIAG_SYS_INIT_SET1.
//           usRc [in] ������,��NO������ʾ
//           ulData [in] �Զ�������,��NO������ʾ
// �� �� ֵ:
//           HI_ERR_SUCCESS           ��ʾ�ɹ�
//           HI_ERR_QUEUE_FULL        ��ʾ������
//           HI_ERR_NO_INITILIZATION  ��ʾ����δ��ʼ��
//           HI_ERR_FAILURE           ��ʾ����δ֪����
//           HI_ERR_CFG_NOT_ALLOW     ��־�������ϱ�
//           HI_ERR_INVALID_PARAMETER ���ηǷ�
//           HI_ERR_NOT_ENOUGH_MEMORY �ڴ����ʧ��
// ����Ҫ��:
// ���þ���: HI_DIAG_ReportSysInitEvent(LOG_EVT_SHUT_DOWN)
// ��    ��: ���ɸ�/00149569/20090210
//*****************************************************************************
HI_U32 HI_DIAG_ReportSysInitEvtRc(HI_U16 usInitEvtId, HI_U16 usRc);
HI_U32 HI_DIAG_ReportSysInitEvt(HI_U16 usInitEvtId);
HI_U32 HI_DIAG_ReportSysEvt(HI_U16 usSysEvtId);
HI_U32 HI_DIAG_ReportSysCriticalEvent(HI_U32 ulEvtId,HI_U32 ulData);
HI_U32 HI_DIAG_ReportSysEvent(HI_U16 usSysEvtId, HI_U8 ucOption);
HI_U32 HI_DIAG_ReportSysInitEvent(HI_U16 usInitEvtId, HI_U8 ucOption);
//*****************************************************************************

//*****************************************************************************
#if defined(PRODUCT_CFG_SAL_API_SUPPORT) && !defined(PRODUCT_CFG_CORE_TYPE_BOOTLOADER)
#define HI_DIAG_ReportSysInitEvtRc(usInitEvtId, usRc)      HI_DIAG_ReportInitEvtPrv(usInitEvtId, HI_DIAG_SYS_INIT_SET, usRc)
#define HI_DIAG_ReportSysInitEvt(usInitEvtId)              HI_DIAG_ReportInitEvtPrv(usInitEvtId, HI_DIAG_SYS_INIT_SET, HI_ERR_SUCCESS)
#define HI_DIAG_ReportSysEvt(usSysEvtId)                   HI_DIAG_ReportSysEventPrv(usSysEvtId, HI_TRUE)
#define HI_DIAG_ReportSysCriticalEvent(ulEvtId, ulData)    HI_DIAG_ReportSysCriticalEventPrv(ulEvtId, ulData)
#define HI_DIAG_ReportSysEvent(usSysEvtId, ucOption)       HI_DIAG_ReportSysEvt(usSysEvtId)
#define HI_DIAG_ReportSysInitEvent(usInitEvtId, ucOption)  HI_DIAG_ReportInitEvtPrv(usInitEvtId, ucOption, HI_ERR_SUCCESS)
#else
#define HI_DIAG_ReportSysInitEvtRc(usInitEvtId, usRc)
#define HI_DIAG_ReportSysInitEvt(usInitEvtId)
#define HI_DIAG_ReportSysEvt(usSysEvtId)
#define HI_DIAG_ReportSysCriticalEvent(ulEvtId, ulData)
#define HI_DIAG_ReportSysEvent(usSysEvtId, ucOption)
#define HI_DIAG_ReportSysInitEvent(usInitEvtId, ucOption)
#endif
//*****************************************************************************

//*****************************************************************************
// ��������: HI_DIAG_ReportSysMessage
// ��������: ϵͳ�����Ϣ�ϱ�
//
// ����˵��:
//   usId [in] ��ϰ�ID.
//   usSrcModId [in] Դģ��ID 0��ʾ��ָ��ģ��ID
//   usDestModId [in] Ŀ��ģ��ID, 0��ʾ��ָ��ģ��ID
//   pPacket [in] ��ϰ�buffer��С.
//   usPacketSize [in] ��ϰ�buffer��С.
//   ulOption [in] ���ѡ��, ȡֵΪ �����κ�һ��ֵ.
//                 a) HI_SYS_DMID_BUSINESS_L0, HI_SYS_DMID_BUSINESS_L1, HI_SYS_DMID_BUSINESS_L2
//                 b) HI_SYS_DMID_NET_L0, HI_SYS_DMID_NET_L1,HI_SYS_DMID_NET_L2
//                 c) HI_SYS_DSID_BUSINESS_L0, HI_SYS_DSID_BUSINESS_L1, HI_SYS_DSID_BUSINESS_L2
//
// �� �� ֵ:
//    HI_ERR_SUCCESS: �ɹ�
//    ����ֵ��ʾ������.
//
// ����Ҫ��:
// ���þ���:
// ��    ��: shenhankun/00130424 [2012-01-04]
//*****************************************************************************
#if defined(PRODUCT_CFG_SAL_API_SUPPORT) && !defined(PRODUCT_CFG_CORE_TYPE_BOOTLOADER)
HI_EAPI HI_U32 HI_DIAG_ReportSysMessage(HI_U16 usId, HI_U16 usSrcModId, HI_U16 usDestModId, HI_PVOID pPacket, HI_U16 usPacketSize, HI_U32 ulOption);
#define HI_DIAG_ReportSysMessage(usId, usSrcModId, usDestModId, pPacket, usPacketSize, ulOption) HI_DIAG_ReportSysMessagePrv(usId, usSrcModId, usDestModId, pPacket, usPacketSize, ulOption)
#else
HI_PRVL HI_U32 HI_DIAG_ReportSysMessage(HI_U16 usId, HI_U16 usSrcModId, HI_U16 usDestModId, HI_PVOID pPacket, HI_U16 usPacketSize, HI_U32 ulOption) { return HI_ERR_NOT_SUPPORT;}
#endif

#if defined(PRODUCT_CFG_FEATURE_NDM) && !defined(PRODUCT_CFG_CORE_TYPE_BOOTLOADER) && !defined(SAL_HAVE_FEATURE_UT)
//*****************************************************************************
// ��������: HI_DIAG_RegisterSysStat
// ��������: ע��ϵͳ���ͳ����
//
// ����˵��:
//   pstSysStatObjTbl [in] ͳ���������б�, ��������������Ϊ 2048 ��, ����ĵ�<<Hi3911V100 ���ϵͳ����Ϸ������˵����.docx>>˵��
//   usModId [in] ͳ�������ڵ�ģ��ID.
//   ulOption [in] ���ѡ��, ����ĵ�<<Hi3911V100 ���ϵͳ����Ϸ������˵����.docx>>˵��
//   pStatPacket [in] ͳ����ָ��(������ȫ�ֱ���ָ����Ǿ�̬����ָ��)
//
// �� �� ֵ:
//    HI_ERR_SUCCESS: �ɹ�.
//    ����ֵ��ʾ������.
//
// ����Ҫ��:
// ���þ���:
//
// HI_DSID_SAL_DUMP_KEY_MSG_S  g_stMyStatDumpKeyMsg = {HI_OBJ64_S_INIT_DEFINED, HI_DSID_SAL_DUMP_KEY_MSG_S_INIT_DEFINED1, {0,0,}}
// HI_DSID_SAL_DEVIO_S         g_stMyStatDevIo      = {HI_OBJ64_S_INIT_DEFINED, HI_DSID_SAL_DEVIO_S_INIT_DEFINED1}
//
// HI_CONST HI_DIAG_SYS_STAT_OBJ_S g_stMyStatObjTbl[] = {
// {HI_DSID_SAL_DUMP_KEY_MSG1,    HI_SYS_DMID_BUSINESS_L0,   HI_DSID_TYPE_QRY,      1,                     sizeof(g_stMyStatDumpKeyMsg),        &g_stMyStatDumpKeyMsg},
// {HI_DSID_SAL_DEVIO1,           HI_SYS_DMID_BUSINESS_L0,   HI_DSID_TYPE_QRY,      1,                     sizeof(g_stMyStatDevIo),             &g_stMyStatDevIo},
// {HI_DSID_SYS_MOD_INIT1,        HI_SYS_DMID_BUSINESS_L0,   HI_DSID_TYPE_CUSTOM,   SYS_DFX_MOD_INIT_MAX,  sizeof(HI_DSID_SYS_MOD_INIT_S),      g_stStatSysModInit},
// TODO...
//}
//  HI_DIAG_RegisterSysStat(g_stMyStatObjTbl, HI_ARRAY_COUNT(g_stMyStatObjTbl))
//
// ��    ��: shenhankun/00130424 [2012-01-04]
//*****************************************************************************
HI_EAPI HI_U32 HI_DIAG_RegisterSysStat(HI_CONST HI_DIAG_SYS_STAT_OBJ_S* pstSysStatObjTbl, HI_U16 usObjNum);
#define HI_DIAG_RegisterSysStat(pstSysStatObjTbl, usObjNum) HI_DIAG_RegisterSysStatPrv(pstSysStatObjTbl, usObjNum)

HI_EAPI HI_U32 HI_DIAG_UpdateSysStatCounter(HI_U32* pulSysStatObj);
#define HI_UPG_OBJ_COUNTER(Obj) if(((HI_OBJ64_S*)Obj)->ulLo >= 0xFFFFFFFF) ((HI_OBJ64_S*)Obj)->ulHi++; else ((HI_OBJ64_S*)Obj)->ulLo++;
#define HI_DIAG_UpdateSysStatCounter(pulStatObj) do { HI_UPG_OBJ_COUNTER(pulStatObj); }while(0)

#endif // #if defined(PRODUCT_CFG_FEATURE_NDM)
//*****************************************************************************

//*****************************************************************************
// MAC
//*****************************************************************************

//*****************************************************************************
// ��������: HI_MAC_QueryInfo
// ��������: ��ȡMAC��ȫ�ֵ������Ϣ
//
// ����˵��:
//   pstInfo [out] : HI_MAC_CONFIG_INFO_S �ṹָ��
//
// �� �� ֵ:
//    HI_ERR_SUCCESS : ��ʾ�ɹ�
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
//*****************************************************************************
HI_EAPI HI_U32 HI_MAC_QueryInfo(HI_OUT HI_MAC_CONFIG_INFO_S* pstInfo);


//****************************************************************************
// �û����ݴ洢�ӿ�
//****************************************************************************

//****************************************************************************
// ��������: HI_SYS_SaveUserData
// ��������: ���û����ݱ��浽���û���������,һ�ε��ÿ��Ա����������.
//           ͨ��ģ�����������ͨ��HI_SYS_GetUserData�ɻ�ȡ�洢���û�����
//
// ����˵��:
//   ulUserDataId [in] �û�����ID�� һ���û�����ID���Զ�Ӧ��������ͨ��ulIdx��ǣ�ȡֵ����.
//                    1) HI_USERDATA_ID_APP  ��ʾ����Ӧ�õ�������, ÿ���С�������(�̶���С)
// 	                  2) HI_USERDATA_ID_BIDB ��ʾ����ģ��Ļ�����Ϣ�⣬pData��ӦHI_MAC_ND_BIDB_ITEM_S�ṹ��ʽ������,ÿ���С�������(�̶���С)
//   ulItemOffset [in] �û���������ʼ����, ��ulUserDataId��Ӧ����������ʱ��ʹ�øò�����ʾ��ͬ��, ��ʱ�ò���ȡֵ�����0��ʼ����.
//   usItemSize [in] ulUserDataId��Ӧ��ÿ��������Ĺ̶�����.
//   ulItemNum [in] �����������������
//   pItems [in] ulUserDataId��Ӧ������ʼ��ַ
//
// �� �� ֵ:
//    HI_ERR_SUCCESS           : �ɹ�.
//    HI_ERR_FULL              : �洢��,�޷���������������
//    HI_ERR_NOT_SUPPORT       : ulUserDataId=HI_USERDATA_ID_BIDB ʱ��,��֧���ն�ģ��.
//    HI_ERR_INVALID_PARAMETER : pData��������usDataSize�Ƿ�����Ϊ�ղ���.
//    HI_ERR_NO_INITILIZATION  : SALδ��ʼ�����.
//    ����������ΪFlash�����ķ���ֵ.
//
// ����Ҫ��:
//    1) ulUserDataIdȡֵΪ HI_USERDATA_ID_BIDB����HI_USERDATA_ID_APP ʱ, �ýӿڽ�֧��CCO,��֧��STA;
//    2) ��������ĳ���������Ѿ����ڣ���ֱ�Ӹ���;
//    3) ÿ�ζ�д��Item��С�����ǹ̶���С.
//    4) ��֧�ֲ�������/����ָ� ����;
//****************************************************************************
HI_EAPI HI_U32 HI_SYS_SaveUserData(HI_U8 ulUserDataId, HI_U32 ulItemOffset, HI_U32 ulItemNum, HI_U16 usItemSize, HI_IN HI_PVOID pItems);

//****************************************************************************
// ��������: HI_SYS_GetUserData
// ��������: ��ȡ�û�����
//
// ����˵��:
//   ucUserDataId [in] �û�����ID�� һ���û�����ID���Զ�Ӧ��������ͨ��ulIdx��ǣ�ȡֵ����.
//                    1) HI_USERDATA_ID_APP  ��ʾ����Ӧ�õ�������, ÿ���С�������(�̶���С)
// 	                  2) HI_USERDATA_ID_BIDB ��ʾ����ģ��Ļ�����Ϣ�⣬pData��ӦHI_MAC_ND_BIDB_ITEM_S�ṹ��ʽ������,ÿ���С�������(�̶���С)
//   ulItemOffset [in] �û���������ʼ����, ��ulUserDataId��Ӧ����������ʱ��ʹ�øò�����ʾ��ͬ��, ��ʱ�ò���ȡֵ�����0��ʼ����.
//   usItemSize [in] ulUserDataId��Ӧ��ÿ��������Ĺ̶�����.
//   ulItemNum [in] ����ȡ������������
//   pusItemsRead [in] ʵ�ʶ�ȡ������������,С�ڵ��� ulItemNum, ���pItemsBufferΪHI_NULL��ò��������û����������
//   pItemsBuffer [out] ulUserDataId��Ӧ����buffer. pItemsBuffer=HI_NULL, ͨ�� pusItemsRead�����Ѿ��洢����item����.
//
// �� �� ֵ:
//    HI_ERR_SUCCESS: �ɹ�.
//    HI_ERR_NOT_SUPPORT : �ն�ģ�飬��֧�ָ�����.
//    HI_ERR_NOT_EXIST   : ��ʾָ�����û����ݲ�����.
//    HI_ERR_NOT_FOUND   : ������� ulItemOffset �����ڣ���ʾ�û����ݲ�����ָ����������.
//    HI_ERR_INVALID_PARAMETER : pData��������usDataSize�Ƿ�����Ϊ�ղ���.
//    HI_ERR_NO_INITILIZATION  : SALδ��ʼ�����.
//    ����������ΪFlash�����ķ���ֵ.
//
// ����Ҫ��:
//    1) ulUserDataIdȡֵΪ HI_USERDATA_ID_BIDB����HI_USERDATA_ID_APPʱ, �ýӿڽ�֧��CCO,��֧��STA;
//    2) ÿ�ζ�д��Item��С�����ǹ̶���С.
//****************************************************************************
HI_EAPI HI_U32 HI_SYS_GetUserData(HI_U8 ucUserDataId, HI_U32 ulItemOffset, HI_U32 ulItemNum, HI_U16 usItemSize, HI_OUT HI_U16* pusItemsRead, HI_OUT HI_PVOID pItemsBuffer);



typedef enum
{
    RESTART_TYPE_UNKNOWN = 0                 ,
    RESTART_TYPE_POWER_CUT = 1               ,
    RESTART_TYPE_WD_RST     = 2              ,
    RESTART_TYPE_PC_ADDR_ZERO                ,
    RESTART_TYPE_UNDEFINED                   ,
    RESTART_TYPE_SWI                         ,

    RESTART_TYPE_PREFETCHW                   ,
    RESTART_TYPE_DATA_ABORT                  ,
    RESTART_TYPE_OS_RST                      ,
    RESTART_TYPE_UPG_COMPLETE                ,
    RESTART_TYPE_UPG_VER_BAK                 ,

    RESTART_TYPE_UPG_MAC_RST                 ,
    RESTART_TYPE_HRTIMER_RST0                ,
    RESTART_TYPE_HRTIMER_RST1                ,
    RESTART_TYPE_HRTIMER_RST2                ,
    RESTART_TYPE_HRTIMER_RST3                ,
    RESTART_TYPE_HRTIMER_RST4                ,
    RESTART_TYPE_HRTIMER_RST5                ,
    RESTART_TYPE_HRTIMER_RST6                ,
    RESTART_TYPE_ACTIVE_RST                  ,
    RESTART_TYPE_CONCENTEROR_HW_RST          ,
    RESTART_TYPE_IR_STA_HW_RST               ,
    RESTART_TYPE_RESET_LOW_RST               ,
    RESTART_TYPE_UPG_INIT_NULL               ,
    RESTART_TYPE_RST_CON_ANY_MR_FAIL         ,
    RESTART_TYPE_CON_NOT_GET_FARME           ,
    RESTART_TYPE_CON_NOT_GET_METER_UART_DATA ,
    RESTART_TYPE_CON_CCO_TOO_MANY_AGENT      ,
    RESTART_TYPE_CON_STA_NOT_TX              ,
    RESTART_TYPE_CON_STA_NOT_RCV_BEA_SOF     ,
    RESTART_TYPE_CON_STA_NOT_RCV_BEA         ,
    RESTART_TYPE_CON_STA_NOT_RCV_SOF         ,
    RESTART_TYPE_CON_CCO_NOT_TX              ,
    RESTART_TYPE_CON_CCO_NOT_RCV_BEA_SOF     ,
    RESTART_TYPE_CON_CCO_NOT_RCV_BEA         ,
    RESTART_TYPE_CON_CCO_NOT_RCV_SOF         ,
    RESTART_TYPE_CON_SECURE_REBOOT           ,
    RESTART_TYPE_CON_CLCII_UART_NOT_RCV_DATA ,
    RESTART_TYPE_CON_STA_PLC_NOT_RCV_DATA    ,
    RESTART_TYPE_CON_STA_CAPACITANCE_POWER_CUT  ,
    RESTART_TYPE_MAX
}DIAG_RESTART_TYPE;//TODO:��ط��ú���ܸ���Щ

HI_U32 HI_SYS_GetTypeOfRestart(HI_U32 *RestartType);

//*****************************************************************************
// Helper routine
#define HI_SYS_TASK_EXCEPTION_POLLING_WAIT()    HI_MDM_Sleep(HI_DMS_CHL_EXCEPTION_POLLING_WAIT)
#define HI_SYS_TASK_WAIT_FOREVER()              HI_MDM_Sleep(0xFFFFFFFF)
//*****************************************************************************


//****************************************************************************
#define HI_SYS_CHECK_CODE_ADDR_U32(_addr)\
    do { \
        if(!HI_IS_VALID_CODE_ADDR(_addr))\
        {\
            return HI_ERR_ADDR_ACCESS_FAILURE;\
        }\
    }while(0)

//****************************************************************************
// ��������: HI_SYS_GetProductVerInfo HI_SYS_GetUeVerInfo
// ��������: ��ȡ��Ʒ�汾��Ϣ���� оƬ����, �汾�ŵ�. �ַ�����ʽ
//
// ����˵��:
//   ��
//
// �� �� ֵ:
//    �ǿ�: ��ʾ��Ʒ�汾��Ϣ
//
// ����Ҫ��:
//    1) ��
//****************************************************************************
HI_EAPI HI_CONST HI_PRODUCT_INFO_S* HI_SYS_GetProductVerInfo(HI_VOID);
HI_EAPI HI_CONST HI_UE_PRODUCT_VER_S* HI_SYS_GetUeVerInfo(HI_VOID);
//****************************************************************************


//****************************************************************************
#if defined(PRODUCT_CFG_FEATURE_SOFT_PRODUCT_ID)
typedef struct
{
    HI_NV_FTM_PRODUCT_ID_S stProductId;
    HI_U32 bEnable; // ������Ҫ, ����ΪHI_U32, �����Ҫ���ӱ���������Ե���������
}DMS_PRODUCT_ID_SOFT_CFG_STRU;
#define DMS_PRODUCT_ID_SOFT_CFG_DEFAULT_VALUE() {{HI_FTM_HW_PLATFORM_TYPE_UNKOWN, HI_FTM_CHIP_TYPE_UNKOWN, HI_FTM_PRODUCT_TYPE_VALUE_UNKOWN, {0,0,}, {0,0,}, {0,0,}, {0,0,}}, 0}

HI_EAPI HI_U32 HI_SYS_SetProductId(HI_IN DMS_PRODUCT_ID_SOFT_CFG_STRU* pstCfg);
#define HI_DMS_SetProductId(pstCfg)   HI_SYS_SetProductId(pstCfg)
#else
#define HI_DMS_SetProductId(pstCfg)
#endif

#if defined(PRODUCT_CFG_FEATURE_SOFT_PRODUCT_ID)
HI_EAPI HI_BOOL HI_SYS_IsUseSoftProductId(HI_VOID);
#else
HI_PRVL HI_BOOL HI_SYS_IsUseSoftProductId(HI_VOID) {return HI_FALSE;}
#endif
#define HI_DMS_IsUseSoftProductId()  HI_SYS_IsUseSoftProductId()
//****************************************************************************


//*****************************************************************************
// DFX
//*****************************************************************************
HI_EAPI HI_U32 HI_SYS_DfxSetHostCtrl(HI_SAL_DFX_HOST_CTRL_S* pstCtrl, HI_U32 pfnSendMrPacketByPass, HI_U32 pfSendMessageByPass);
HI_EAPI HI_SAL_DFX_HOST_CTRL_S* HI_SYS_DfxGetHostCtrl(HI_VOID);
HI_EAPI HI_U32 HI_SYS_DfxChlUartRx(HI_U16 usPortNum, HI_IN HI_PBYTE pDataBuffer, HI_U16 usDataBufferLen);
HI_EAPI HI_U32 HI_SYS_DfxSendFrame(HI_PVOID pPayload, HI_U16 usPayloadLen);
HI_EAPI HI_U32 HI_SYS_DfxSendMacMessage(HI_IN HI_PVOID pPacket, HI_U16 usPacketSize, HI_SAL_DFX_MAC_MSG_E enMsg);
HI_EAPI HI_U32 HI_SYS_DfxSetVerInfo(HI_SYS_DFX_VER_E enVerType, HI_CONST HI_CHAR szVer[12], HI_CONST HI_CHAR szBuiltDataTime[20]); // 1.0.0, PRODUCT_CFG_BUILD_DATE " " PRODUCT_CFG_BUILD_TIME
#define HI_SYS_CONFIG_APP_VER(x,y,z)  HI_SYS_DfxSetVerInfo(HI_DFX_APP_VER, HI_SAL_SYS_DFX_VER_STR(x,y,z), HI_SAL_SYS_DFX_VER_BUILD_DATETIME())
//*****************************************************************************


//*****************************************************************************
// ��������: HI_SYS_GetSysInfo
// ��������: ��ȡϵͳ��Ϣ, ���� ϵͳ״̬���汾�ŵ�
//
// ����˵��:
//  ��
//
// �� �� ֵ:
//    None
//
// ����Ҫ��: ϵͳ״̬��Ϣ�в������ݲ��Ǿ�̬�ģ���Ҫʵʱ���øýӿڻ�ȡϵͳ״̬��Ϣ�Ա�ͬ��.
// ���þ���: TODO: ...
// ��    ��: s00130424/s00130424 [2012-08-02]
//****************************************************************************
HI_EAPI HI_SYS_STATUS_INFO_S* HI_SYS_GetSysInfo(HI_VOID);
//*****************************************************************************
// ��������: HI_SYS_GetNvVerInfo
// ��������: ��ȡNV VR�汾�ź���(Ϊ�˼����Բ��������ṹ��)
//
// ����˵��:
//  ��
//
// �� �� ֵ:
//    None
//
// ����Ҫ��: .ϵͳ״̬��Ϣ�в������ݲ��Ǿ�̬�ģ���Ҫʵʱ���øýӿڻ�ȡϵͳ״̬��Ϣ�Ա�ͬ��.
// ���þ���: TODO: ...
// ��    ��: b00208046[2014-07-17]
//****************************************************************************

HI_EAPI HI_SYS_NV_VER_INFO_S* HI_SYS_GetNvVerInfo(HI_VOID);

HI_EAPI HI_VOID SAL_SetBootVerIdx(HI_U8 ucBootVerIdx);

//****************************************************************************


//****************************************************************************
// ����/��ѯ MAC��IP�����롢���ص�
// pszName : ȡֵΪ "eth0"����"eth1", ��֧������ֵ
//****************************************************************************
#define HI_SYS_ETH_NAME "eth0"
#define HI_SYS_PLC_NAME "eth1"
HI_EAPI HI_U32 HI_SYS_SetEthConfig(HI_PCSTR pszName, HI_IN HI_SYS_ETH_CONFIG_S* pstEthCfgInfo);
HI_EAPI HI_U32 HI_SYS_GetEthConfig(HI_PCSTR pszName, HI_OUT HI_SYS_ETH_CONFIG_S* pstEthCfgInfo);
HI_EAPI HI_U32 HI_SYS_AddRoute(HI_PCSTR pszName, HI_U8 aucDstAddr[HI_IPV4_ADDR_LEN], HI_U8 aucNetMask[HI_IPV4_ADDR_LEN],
        HI_U8 aucGateway[HI_IPV4_ADDR_LEN]);
HI_EAPI HI_U32 HI_SYS_DelRoute(HI_U8 aucDstAddr[HI_IPV4_ADDR_LEN]);
//****************************************************************************

HI_EAPI HI_U32 HI_SYS_SetRebootDelay(HI_U32 ulWdgTimeCfg);

//****************************************************************************
HI_U32 HI_DIAG_ChlSetup(HI_U16 usPortNum);
//****************************************************************************

HI_U32 HI_SYS_SendMessage(HI_U32 ulQueueId, HI_BOOL bAppQueue, HI_IN HI_SYS_QUEUE_MSG_S* pstMsg, HI_U32 ulTimeOut);


//****************************************************************************
#if !defined(PRODUCT_CFG_HSO)
#define HI_DIAG_PrintfV(ulId, pszFileName, ulLineNum, pszFmt, ...)
#endif
#define HI_DIAG_Print0(id,sz)
#define HI_DIAG_Print1(id,sz, p1)
#define HI_DIAG_Print2(id,sz, p1, p2)
#define HI_DIAG_Print3(id,sz, p1, p2, p3)
#define HI_DIAG_Print4(id,sz, p1, p2, p3, p4)
//****************************************************************************


//*****************************************************************************
// DIAG via NDM channle API
//*****************************************************************************
//*****************************************************************************
// ��������: HI_DIAG_SendNdmReqFrame
// ��������: ͨ��������ͨ�������û�֡ unicast
//
// ����˵��:
//   uscFrameId [in] ֡ID, ����ΪDIAG��ID.
//   pucFrame [in] ֡buffer, �ú��������ͷŸ�ָ��.
//   uscFrameSize [in] ֡buffer��С.
//   aucMacAddr [in] ͨ��ģ��·��. �μ� <<Hi3911V100 ����������ӿ����˵����.docx>>�ĵ�˵��.
//   ulSn [in] ֡SN,�粻��Ҫ��ֵΪ0
//
// �� �� ֵ:
//    HI_ERR_SUCCESS: �ɹ�
//    ����ֵ��ʾ������.
//
// ����Ҫ��:
// ���þ���:
// ��    ��: shenhankun/00130424 [2012-01-04]
//*****************************************************************************
#if !defined(PRODUCT_CFG_CORE_TYPE_BOOTLOADER)
HI_EAPI HI_U32 HI_DIAG_SendNdmReqFrame(HI_U16 uscFrameId, HI_PBYTE pucFrame, HI_U16 uscFrameSize, HI_U8 aucMacAddr[HI_PLC_RELAY_NUM_MAX][HI_PLC_MAC_ADDR_LEN], HI_U32 ulSn);
HI_EAPI HI_U32 HI_DIAG_SendNdmFrame(HI_U16 usPacketId, HI_PBYTE pucPacket, HI_U16 usPacketSize, HI_U8 aucMacAddr[HI_PLC_RELAY_NUM_MAX][HI_PLC_MAC_ADDR_LEN], HI_U32 ulSn);
#endif

#if !defined(PRODUCT_CFG_CORE_TYPE_BOOTLOADER)
//****************************************************************************
// ��ȡϵͳ״̬ HI_SYS_STATUS_IND_S
//****************************************************************************
HI_EAPI HI_SYS_STATUS_IND_S* HI_SYS_GetSysStatus(HI_VOID);
#endif

HI_U32 HI_SYS_SetEvt(HI_U32 ulEvtId, HI_BOOL bAppLayer, HI_U32 ulEventBit);
HI_U32 HI_SYS_WaitEvt(HI_U32 ulEvtId, HI_BOOL bAppLayer, HI_OUT HI_U32* pulEvent, HI_U32 ulTimeOut);
HI_U32 SAL_StopHRTimer(HI_INOUT TIMER_HANDLE_T* phTimer);
HI_U32 sal_StartHRTimer(HI_OUT TIMER_HANDLE_T* phTimer, HI_PVOID pfnTimerRoutine, HI_U32 ulTimeOut, HI_U32 ulTimerId);
HI_U32 HI_SYS_WaitMessage(HI_U32 ulQueueId, HI_BOOL bAppQueue, HI_OUT HI_SYS_QUEUE_MSG_S* pstMsg, HI_U32 ulTimeOut);
HI_U32 SAL_StartHRTimer(HI_OUT TIMER_HANDLE_T* phTimer, HI_PVOID pfnTimerRoutine, HI_U32 ulTimeOut);
HI_U32 HI_SYS_SignalSem(HI_U32 ulSemId, HI_BOOL bApp);
HI_U32 HI_SYS_WaitSem(HI_U32 ulSemId, HI_U32 ulTimeOut, HI_BOOL bApp);
#if defined (PRODUCT_CFG_ICC_FMGR_SUPPORT)
HI_U32 HI_3518_UPG_Transfer(HI_UPG_FILE_HEADER_S *pHead,HI_U8 * pFile);
#else
//HI_PRVL HI_U32 HI_3518_UPG_Transfer(HI_UPG_FILE_HEADER_S *pHead,HI_U8 * pFile){return HI_ERR_NOT_SUPPORT;}
#endif


HI_U32 HI_SYS_SetUserInfo(HI_MDM_USER_INFO_S *pstUserInfo);
HI_U32 HI_SYS_GetUserInfo(HI_MDM_USER_INFO_S *pstUserInfo);
//****************************************************************************
HI_U32 HI_SYS_route_table_show(HI_CHAR *buf, HI_U32 buf_len);

#if !defined(PRODUCT_CFG_BUILD_HSO_CODE)
//*****************************************************************************
// ���ָ����DIAG�˿��Ƿ�����
// ����ֵ: HI_TRUE��ʾDIAGͨ������; HI_FALSE��ʾ������
//*****************************************************************************
HI_EAPI HI_BOOL HI_DIAG_IsConnect(HI_VOID);
#define HI_DIAG_IsConnect()    HI_DIAG_IsConnectPrv()

#if defined(PRODUCT_CFG_DIAG_API_CHK_CHL_CONNECT)
#define HI_DIAG_IS_CONNECT_CHK   if(HI_DIAG_IsConnect())
#else
#define HI_DIAG_IS_CONNECT_CHK
#endif

#endif

typedef HI_U32 (*WD_MONITOR_TASK_HOOK)(HI_VOID);
HI_VOID HI_SYS_RegisterWdMonitorTaskHook(WD_MONITOR_TASK_HOOK func);//ע�ῴ�Ź������Ӻ���
hi_u8 HI_SYS_GetChipMode(hi_void);

HI_END_HEADER
#endif // __HI_SAL_H__

