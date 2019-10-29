/******************************************************************************
*             Copyright (C) 2018 - 2018, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName: hi_mdm_types.h
* Description��SAL(ϵͳ����Ӧ�ò�)�������Ͷ���
*
* History:
* Version              Date         DefectNum           Description
* V200R001C00SPC010    2018-03-31   NA                  ��һ�ΰ汾������
* V200R001C00SPC012    2018-07-17   DTS2018061507315	�޸���HI_SYS_STATUS_INFO_S�ṹ�г�ԱucSysMode��������
                                    DTS2018071400465    ������PLC��׼Э��ͷǱ�׼Э��ĺ궨�塢HI_MAC_NETWORK_TOPO_ENTRY_S�����Ӳ���mac_type��
* V200R001C00SPC050B013    2019-01-09   DTS2019010900227    ���������ģ������2������3����λʶ������
******************************************************************************/
#ifndef __HI_MDM_TYPES_H__
#define __HI_MDM_TYPES_H__
#ifndef __HI_TYPES_H__
#error "Please include hi_types.h before using hi_mdm_types.h"
#endif
HI_START_HEADER
#ifndef PRODUCT_CFG_HSO
#include <hi_mdm_msg.h>
#endif
//*****************************************************************************

//*****************************************************************************
// �ֽڶ���
//*****************************************************************************
#define ALIGNTYPE_1BYTE 1
#define ALIGNTYPE_2BYTE 2
#define ALIGNTYPE_4BYTE 4
#define ALIGNTYPE_8BYTE 8
#define ALIGNTYPE_64BYTE 64
#define ALIGNTYPE_32BYTE 32
#define ALIGNTYPE_4K 4096
#define ALIGN_NEXT(val,a) ((((val) + ((a)-1)) & (~((a)-1))))
#define ALIGN_LENGTH(val, a) ALIGN_NEXT(val, a)
#define HI_ALL_F_32  0xFFFFFFFF
#define HI_ALL_F_16  0xFFFF



//*****************************************************************************
// �����������Ͷ���
//*****************************************************************************

typedef struct
{
    HI_U32 data[20];
}MEM_POOL_HANDLE_T;


typedef struct
{
    HI_U32 data[20];
}TIMER_HANDLE_T;

typedef struct
{
    HI_U16 ulFailTimes;
    HI_U16 usPeak;
    HI_U32 ulPeakFirstTick;
    HI_U32 ulPeakLastTick;
}HI_DIAG_BUF_STAT_S;

typedef struct
{
    HI_U32 ulDynRamUsedPeek;
    HI_U32 ulDynRamUsedPeekTime;
}HI_DYN_MEM_PEEK_S;



//*****************************************************************************


//*****************************************************************************
#ifdef PRODUCT_CFG_OS_WIN
#define HI_TIMER_DEFINE(TimerProc)   __stdcall TimerProc(HI_U32 ulId, HI_U32 uMsg, HI_U32 ulData, HI_U32 dw1, HI_U32 dw2)
#define HI_TASK_DEFINE(TaskProc)     __cdecl TaskProc(HI_VOID* pArgv)
#define HI_HITIMER_DEFINE(TimerProc) __stdcall TimerProc(HI_U32 uTimerID, HI_U32 uMsg, HI_U32 dwUser, HI_U32 dw1, HI_U32 dw2)
#endif

#ifdef PRODUCT_CFG_OS_NU
#define HI_TIMER_DEFINE(TimerProc)    TimerProc(HI_U32 ulData)
#define HI_TASK_DEFINE(TaskProc)      TaskProc(HI_U32 ulArgc, HI_VOID* pArgv)
#define HI_HITIMER_DEFINE(TimerProc)  TimerProc(HI_U32 ulParam)
#endif

#ifdef PRODUCT_CFG_OS_VX
#define HI_TIMER_DEFINE(TimerProc)    TimerProc(HI_U32 ulData)
#define HI_TASK_DEFINE(TaskProc)      TaskProc(HI_VOID* pArgv)
#define HI_HITIMER_DEFINE(TimerProc)  TimerProc(HI_U32 ulParam)
#endif


#if defined(PRODUCT_CFG_OS_WIN) && defined(PRODUCT_CFG_HAVE_WIN_STUB) && !defined(PRODUCT_CFG_HSO)
# include <hal_stub.h>
# pragma warning(disable:4055)
# pragma warning(disable:4054)
# define HI_HAL_LOAD_INTERFACE(pszFileName, pfn)   (HI_PVOID)HAL_GetProcAddress(pszFileName, # pfn)
# define HI_DLL_FUNC(name, entry)                 ((HAL_PROC)HI_HAL_LOAD_INTERFACE(HI_DLL_LIB_NAME(name), entry))
# define HI_DLL_INF(name, entry)                   HI_DLL_FUNC(name, entry)()
#endif
//*****************************************************************************
typedef enum
{
    HI_DMS_RX_TIME_OUT_UART_DIAG,
    HI_DMS_RX_TIME_OUT_MAX,
}HI_DMS_RX_TIME_OUT_E;


//*****************************************************************************
typedef struct
{
    HI_U32 ulLo;
    HI_U32 ulHi;
}HI_OBJ64_S;
#define HI_OBJ64_S_INIT_DEFINED {0,0}
#define OBJ64_COUNT_ADD(Obj) if(((HI_OBJ64_S*)Obj)->ulLo >= 0xFFFFFFFF) ((HI_OBJ64_S*)Obj)->ulHi++; else ((HI_OBJ64_S*)Obj)->ulLo++;
typedef struct
{
    HI_U32 ulMajorMinorVersion; // ���汾��.�ΰ汾��
    HI_U32 ulRevisionVersion;   // �����汾��
    HI_U32 ulBuildVersion;      // �ڲ��汾��
} HI_UE_SOFT_VERSION_S;


#define HI_BUILD_VER_DATE_LEN                (10)
#define HI_BUILD_VER_TIME_LEN                (8)
#define HI_BUILD_VER_PRODUCT_NAME_LEN_MAX    (28)
#define HI_BUILD_VER_PRODUCT_LEN_MAX         (HI_BUILD_VER_PRODUCT_NAME_LEN_MAX+HI_BUILD_VER_DATE_LEN+HI_BUILD_VER_TIME_LEN+6)

typedef struct
{
    HI_U16 usVVerNo;                         // V����
    HI_U16 usRVerNo;                         // R����
    HI_U16 usCVerNo;                         // C����
    HI_U16 usBVerNo;                         // B����
    HI_U16 usSpcNo;                          // SPC����
    HI_U16 usCustomVer;                      // �ͻ����Ʊ���
    HI_U32 ulProductNo;                      // ��Ʒ���ͱ�ţ�����ͬ������ϵ�Ӳ��ƽ̨
    HI_CHAR acBuildDate[HI_BUILD_VER_DATE_LEN]; // build����, ��ʽΪMon dd yyyy
    HI_CHAR acBuildTime[HI_BUILD_VER_TIME_LEN]; // buildʱ��, ��ʽΪhh:mm:ss
} HI_UE_BUILD_VER_INFO_S; // �ڲ��汾

typedef struct
{
    HI_U16 usVVerNo;           // V����
    HI_U16 usRVerNo;           // R����
    HI_U16 usCVerNo;           // C����
    HI_U16 usBVerNo;           // B����
    HI_U16 usSpcNo;            // SPC����
    HI_U16 usHardwareVerNo;    // Ӳ��PCB�ź�ӡ�ư�汾��
    HI_U32 ulProductNo;        // ��Ʒ���ͱ�ţ�����ͬ������ϵ�Ӳ��ƽ̨
} HI_UE_PRODUCT_VER_S;

typedef struct
{
    HI_CHAR* pszVer;         // "V100R001C00B00"
    HI_CHAR* pszProductVer;  // "Hi3911 V100R001C00B00"
    HI_CHAR* pszFileVer;     // Modem Version: like "1.999.111"
    HI_CHAR* pszChipName;    // "Hi3911"
    HI_CHAR* pszDate;        // �� 2017-08-01
    HI_CHAR* pszTime;        // �� 14:30:26
} HI_PRODUCT_INFO_S;

typedef struct
{
    HI_U32 ulId;
    HI_U32 ulCrc;
    HI_U32 ulBlkSize;
    HI_U32 ulBlkCnt;
    HI_U32 ulFileSize;
    HI_U32 ulAddr;
} HI_FILE_TRANSMIT_REQ_S;

typedef struct
{
    HI_U32 ulCheckSum;
    HI_U32 ulMagicNum;
    HI_U32 ulSn;
    HI_U32 ulRxLen;
    HI_U32 ulRet;
    HI_CHAR szBuffer[140];
} HI_FILE_TRANSMIT_BLK_IND_S;

typedef struct
{
    HI_U32 ulCheckSum;
    HI_U32 ulMagicNum;
    HI_U32 ulSn;
    HI_U32 ulBuffSize;
    HI_U8 ucBuffer[0];
} HI_FILE_TRANSMIT_BLK_REQ_S;

typedef struct
{
    HI_U32 ulLoadId;
    HI_U32 ulErr;
}HI_FILE_TRANSMIT_RESULT_S;
//*****************************************************************************

//*****************************************************************************
#define HI_INVALID_NV_DATA  0xFFFFFFFF
//*****************************************************************************

//*****************************************************************************

#define HI_SYS_WAIT_FOREVER           0xFFFFFFFF
#define HI_INVALID_QUEUE_ID           0xFFFFFFFF
#define HI_SYS_USER_MESSAGE_ID_MAX    0x00FFFFFF

#define HI_SYS_MSG_SYS_INIT_STATUS    (HI_SYS_USER_MESSAGE_ID_MAX+1)
#define HI_SYS_MSG_UPG_STATUS         (HI_SYS_MSG_SYS_INIT_STATUS)

#define HI_SYS_DISABLE_INTERRUPTS    1 // �����жϹر�
#define HI_SYS_ENABLE_INTERRUPTS     2 // �����ж�ʹ��

#define HI_DIAG_CMD_ID(x)            ID_DIAG_CMD_ ## x

#define HI_DIAG_SYS_STAT_OBJ_CNT     2048

typedef enum
{
    EN_SYS_DUMMY_TYPEO_MEM
} HI_SYS_DUMMY_TYPE;

typedef enum
{
    EN_SAL_ERROR_TYPE_INIT_INVALID_CHIP_TYPE,
    EN_SAL_ERROR_TYPE_INIT_NO_MEM,  // �����ʼ�������У�ϵͳ�ṩ���ڴ治�㣬�����޷���������(�����޸��Ĵ���)
    EN_SAL_ERROR_TYPE_OS_RES_LIMIT, // �����ʼ�������У�OS������Դ���㣬�����޷���������(�����޸��Ĵ���)
    EN_SAL_ERROR_TYPE_STOP,         // �������й����У������޷���������(�����޸��Ĵ���)
    EN_SAL_ERROR_TYPE_FATAL,
    EN_SAL_ERROR_TYPE_INIT_TASKS,
    EN_SAL_ERROR_TYPE_INIT
} HI_SYS_ERROR_TYPE_E;

typedef HI_U32 (*HI_RST_USER_REQ_HANDLE_F)(HI_U32);
#define HI_SYS_INVALID_SOCKET                  (HI_SOCKET)(~0)
#define HI_SYS_SOCKET_ERROR                    (-1)
#define HI_SYS_SOCKET_SHUTDOWN_BOTH            0x02

typedef struct
{
#if defined(PRODUCT_CFG_OS_WIN)
    HI_U32 fd_buffer[80];
#else
    HI_U32 fd_buffer[8];
#endif
}HI_SYS_SOCKET_FD;

#define SAL_SOCKET_ERROR     (-1)

typedef HI_U8 HI_SYS_UART_E;

// Specify the baud rate in SDA_UartOpen().
typedef enum
{
    EN_SAL_UART_BAUD_RATE_300     = 300,     // 300 bps
    EN_SAL_UART_BAUD_RATE_600     = 600,     // 600 bps
    EN_SAL_UART_BAUD_RATE_1200    = 1200,     // 1.2 Kbps
    EN_SAL_UART_BAUD_RATE_2400    = 2400,     // 2.4 Kbps
    EN_SAL_UART_BAUD_RATE_4800    = 4800,     // 4.8 Kbps
    EN_SAL_UART_BAUD_RATE_9600    = 9600,     // 9.6 Kbps
    EN_SAL_UART_BAUD_RATE_14400   = 14400,    // 14.4 Kbps
    EN_SAL_UART_BAUD_RATE_19200   = 19200,    // 19.2 Kbps
    EN_SAL_UART_BAUD_RATE_28800   = 28800,    // 28.8 Kbps
    EN_SAL_UART_BAUD_RATE_33600   = 33600,    // 33.6 Kbps
    EN_SAL_UART_BAUD_RATE_38400   = 38400,    // 38.4 Kbps
    EN_SAL_UART_BAUD_RATE_57600   = 57600,    // 57.6 Kbps
    EN_SAL_UART_BAUD_RATE_115200  = 115200,   // 115.2 Kbps
    EN_SAL_UART_BAUD_RATE_230400  = 230400,   // 230.4 Kbps
} HI_SYS_UART_BAUD_RATE_E;

// Define the UART configuration structure for SDA_UartOpen().
typedef struct
{
    HI_U8 ucDataBit;  //  Data format 7bit; 8bit
    HI_U8 ucStop;     // Number of stop bits, 1: 1 stop bit; 2: 2 stop bits
    HI_U8 ucParity;   // Parity check. 0: No parity check; 1: Parity check is odd; 2: Parity check is even;
    HI_U8 ucFlowCtrl; // Hardware Auto Flow control
    HI_SYS_UART_BAUD_RATE_E enRate; // Baud Rate which defined above.
} HI_SYS_UART_CFG_S;

/**
 * @ingroup hct_uart
 * UART ����
 */
typedef struct
{
    HI_U32  baud_rate;  /**<������*/
    HI_U32  data_bits;  /**<����λ��*/
    HI_U32  stop_bits;  /**<ֹͣλ 0:0��ֹͣλ 1:1.5��2��ֹͣλ*/
    HI_U32  parity;     /**<��żУ�� 0:��У�� 1:��У�� 2:żУ��*/
} HI_UART_INIT_TYPE_S;

#define HI_SYS_UART_CFG_DEFAULT_VAL  {(HI_SYS_UART_BAUD_RATE_E)HI_DMS_DIAG_UART_DEFAULT_BAUDRATE,8, 1, 0}

// ����λ
#define HI_UART_DATA5BIT             (5)
#define HI_UART_DATA6BIT             (6)
#define HI_UART_DATA7BIT             (7)
#define HI_UART_DATA8BIT             (8)

// ֹͣλ
#define HI_UART_STOP1                (1)
#define HI_UART_STOP2                (2)

// У��
#define HI_UART_NOCHK                (0)
#define HI_UART_ODD                  (1)
#define HI_UART_EVEN                 (2)

/**
 * @ingroup hct_time
 * ǽ��ʱ��
 */
typedef struct
{
    HI_U16 year;    /**< ��*/
    HI_U8 month;    /**< ��*/
    HI_U8 day;      /**< ��*/
    HI_U8 hour;     /**< ʱ*/
    HI_U8 min;      /**< ��*/
    HI_U8 sec;      /**< ��*/
    HI_U8 reserved;
}HI_SYS_CALENDAR_TIME_S;

typedef struct
{
    HI_PCSTR pszTaskName;
    HI_PVOID pStackAddr;
    HI_U32 ulStackSize;
    HI_U32 ucPriority; // Ϊ�˶���, ԭ����8bit��Ϊ32bit
    HI_VOID_CALLBACK_F pfnTaskInit;
    HI_VOID_CALLBACK_F pfnTaskBody;
    HI_PCSTR pszTaskInitName;
    HI_PCSTR pszTaskBodyName;
} HI_SYS_TASK_CTX_CFG_S;
#ifndef PRODUCT_CFG_HSO
typedef struct
{
    HI_SYS_QUEUE_MSG_S* pstQueue;
    HI_PCSTR pszName;
    HI_U32 ulCnt;
} HI_SYS_QUEUE_MSG_CFG_S;

typedef struct
{
    HI_SYS_TASK_CTX_CFG_S* pastTaskCfg;
    HI_U32 usTaskCount;
    HI_SYS_QUEUE_MSG_CFG_S* pastQCfg;
    HI_U32 usQCount;
    HI_PCSTR* paszSemName;
    HI_U32 ulSemCount;
    HI_PCSTR* paszTimerName;
    HI_U32 ulTimerCount;
    HI_U32 ulUpgTaskQueueId;
}HI_SYS_APP_RES_CFG_S;
#endif

/**
* @ingroup  hct_nv
* @brief NV����֪ͨ�ص�������
*
* @par ����:
* NV����֪ͨ�ص�������
* @attention
* @li �ص������о��������޸�NV�����ѭ����
*
* @param  ulId [IN] ���޸ĵ�NV ID��
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_nv.h: �ýӿ��������ڵ�ͷ�ļ���
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
typedef HI_U32(*HI_NVM_CHANGED_NOTIFY_CALLBACK_F) (HI_U16 ulId);

#define HI_NVM_CHANGED_NOTIFY_CALLBACK_PFN HI_NVM_CHANGED_NOTIFY_CALLBACK_F

typedef HI_U32(*HI_LD_FILE_CALLBACK_F) (HI_U32 ulOption, HI_U32 ulId, HI_PVOID pFile, HI_U32 ulFileSize, HI_U32 ulErrCode);

/**
* @ingroup  hct_diag
* @brief  DIAG����״̬֪ͨ������
*
* @par ����:
* ��DIAGͨ������״̬�����仯ʱ,ͨ���ú���֪ͨAPP��
* @attention �ޡ�
*
* @param  usPortNum [IN] ͨ���š�
* @param  bConnect [IN] ����״̬��HI_TRUE:DIAG���� HI_FALSE:DIAG�Ͽ����ӡ�
*
* @retval ��Ҫ�� �����û�Ĭ�Ϸ���HI_ERR_SUCCESS��
*
* @par Dependency:
* @li hi_mdm_diag.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
typedef HI_U32(*HI_DIAG_CONNECT_F) (HI_U16 usPortNum, HI_BOOL bConnect);

/**
 * @ingroup hct_dms
 * ͨ��ģʽ
 */
typedef enum
{
    HI_DMS_LCHL_MODE_LOCAL = 0, /**< ͨ��������ԴΪ���� */
    HI_DMS_LCHL_MODE_PLC,       /**< ͨ��������ԴΪPLC */
    HI_DMS_LCHL_MODE_MAX
} HI_DMS_LCHL_MODE_E;

/**
 * @ingroup hct_dms
 * ͨ�����ݽ��յĿ��ƽṹ
 */
typedef struct
{
    HI_U16 usId;                            /**< ����ID������ͨ��ģʽΪHI_DMS_LCHL_MODE_PLCʱ��Ч */
    HI_U8 ucMode;                           /**< ͨ��ģʽ��ȡֵ��HI_DMS_LCHL_MODE_E */
    HI_U8 ucCtrl;                           /**< ����ѡ���֣�����ͨ��ģʽΪHI_DMS_LCHL_MODE_PLCʱ��Ч */
    HI_U8 bCanDiscardOldData    : 1;        /**< ���������ݱ�ʶ��HI_TUREΪ������HI_FALSEΪ������*/
    HI_U8 bDataTimeout          : 1;        /**< ��δʹ�� */
    HI_U8 reserved1 : 6;                    /**< ����λ */
    HI_U8 reserved2;                        /**< ����λ */
    HI_U8 ucDestMac[HI_PLC_MAC_ADDR_LEN];   /**< Ŀ��MAC��ַ */
    HI_U8 ucSrcMac[HI_PLC_MAC_ADDR_LEN];    /**< ԴMAC��ַ */
    HI_U32 bIsMyNetFrame    : 1;            /**< �Ƿ����籨�� */
    HI_U32 ucRcvFreq        : 7;            /**< ����Ƶ�κ� */
    HI_U32 ulRcvNid         : 24;           /**< ������·�� */
    HI_U16 usPayloadLen;                    /**< �������ݳ��� */
    HI_PBYTE pPayload;                      /**< ��������ָ�� */
} HI_DMS_CHL_RX_S;

typedef struct
{
    HI_U16 usIdx;
    HI_U16 usArqCnt;
    HI_U32 ulErr;
} HI_DMS_CHL_TX_S;

typedef struct
{
    HI_DMS_CHL_TX_S stChlTx;
} HI_DMS_CHL_STATUS_IND_S;


#define HI_DMS_PLC_FRM_LEN1              (3*512)
#define HI_DMS_PLC_FRM_LEN2              (80)

#define HI_DMS_GET_FRM_IDX(pInd)      (((HI_DMS_CHL_TX_S*)(pInd))->usIdx)
#define HI_DMS_GET_FRM_ERR(pInd)      (((HI_DMS_CHL_TX_S*)(pInd))->ulErr)
#define HI_DMS_GET_FRM_ARQ_CND(pInd)  (((HI_DMS_CHL_TX_S*)(pInd))->usArqCnt)


/**
 * @ingroup hct_dms
 * ͨ���Ͽ�
 */
#define HI_DMS_CHL_DISCONNECT      0    /**< ͨ���Ͽ� */

/**
 * @ingroup hct_dms
 * ͨ������
 */
#define HI_DMS_CHL_CONNECT         1    /**< ͨ������ */

#define HI_DMS_CHL_MSG_MIN         0x1000
#define HI_DMS_CHL_MSG_MAX         0x4000

/**
 * @ingroup hct_dms
 * HI_DMS_SetPortIoControl�ӿڹ���ѡ��
 */
typedef enum
{
    HI_DMS_PORT_CTRL_ONLINE_RX         = 0x0,               /**< ͨ�����������Ż�����ܽ������ݡ���ʱ����:pInputParam = 0; pOutputParam=0*/
    HI_DMS_PORT_CTRL_OFFLINE_ONLINE_RX = 0x1,               /**< ͨ�����ܽ������ݣ�����PLC��������״̬���� ��ʱ����:pInputParam = 0; pOutputParam=0*/
    HI_DMS_PORT_CTRL_RX_NA             = 0xFF,              /**< �ù�����δʹ�� */
    HI_DMS_PORT_CTRL_NO_DELAY          = 0x100,             /**< ��ʾͨ������ʱ���գ�������TCP�˿ڣ� */
    HI_DMS_PORT_CTRL_RX_TO             = 0x101,             /**< �ù�����δʹ�� */
    HI_DMS_PORT_CTRL_RX_FRM_TO         = 0x102,             /**< ��ʾ��ʱʱ�䣬��λΪ�롣pInputParam=��ʱʱ�䣻pOutputParam=0*/
    HI_DMS_PORT_CTRL_UART_MODE_485     = 0x200,             /**< 485����ģʽ */
    HI_DMS_PORT_CTRL_UART_MODE_IR      = 0x201,             /**< ����ģʽ*/
    HI_DMS_PORT_CTRL_UART_MODE_UART    = 0x202,             /**< 232ģʽ */
    HI_DMS_PORT_CTRL_UART_CLOSE		   = 0x203,             /**< �ر�����DMSͨ���Ĵ��ڶ˿� */
    HI_DMS_PORT_PLC_MIN                = HI_DMS_CHL_MSG_MIN,/**< �ù�����δʹ�� */
    HI_DMS_PORT_PLC_TOPO_QRY,                               /**< �ù�����δʹ�� */
    HI_DMS_PORT_CTRL_MAX
} HI_DMS_PORT_CTRL_OPT_ID_E;
#define HI_DMS_PORT_CTRL_OPEN  HI_DMS_PORT_CTRL_NA



/**
* @ingroup  hct_dms
* @brief  ͨ�����ӽӿ�ʵ��ָ�롣
*
* @par ����:
* ͨ���ӿ�ʵ��ָ�룬������ͨ����ʼ��ʱע���û��ص�������
* @attention �ޡ�
*
* @param  ����1 [IN] ͨ���˿ںš�
* @param  ����2 [IN] ����ͨ�������ӻ�Ͽ�������ȡֵΪHI_DMS_CHL_CONNECT���Ͽ�ȡֵΪHI_DMS_CHL_DISCONNECT��
*
* @retval #HI_ERR_SUCCESS ���ӳɹ���
* @retval ����ֵ ����ʧ�ܣ�����ֵ������μ�hi.errno.h��
*
* @par Dependency:
* @li hi_mdm_types.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
typedef HI_U32 (*HI_DMS_CHL_CONNECT_F)(HI_U16, HI_U8);

/**
* @ingroup  hct_dms
* @brief  ͨ����Ϣ����ӿ�ʵ��ָ�롣
*
* @par ����:
* ͨ���ӿ�ʵ��ָ�룬������ͨ����ʼ��ʱע���û��ص�������
* @attention �ޡ�
*
* @param  ����1 [IN] ͨ���˿ںš�
* @param  ����2 [IN] ��ϢID��
* @param  ����3 [IN] ��Ϣ������
*
* @retval #HI_ERR_SUCCESS ��Ϣ����ɹ�
* @retval ����ֵ ��Ϣ����ʧ�ܣ�����ֵ������μ�hi.errno.h��
*
* @par Dependency:
* @li hi_mdm_types.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
typedef HI_U32 (*HI_DMS_CHL_MSG_IND_F)(HI_U16, HI_U32, HI_PVOID);

/**
* @ingroup  hct_dms
* @brief  ͨ�����ݽ��սӿ�ʵ��ָ�롣
*
* @par ����:
* ͨ���ӿ�ʵ��ָ�룬������ͨ����ʼ��ʱע���û��ص�������
* @attention �ޡ�
*
* @param  ����1 [IN] ͨ���˿ںš�
* @param  ����2 [IN] ͨ�����ݽ��յĿ��ƽṹ��
*
* @retval #HI_ERR_SUCCESS ���ݽ��ճɹ���
* @retval ����ֵ ��������ʧ�ܣ�����ֵ������μ�hi.errno.h��
*
* @par Dependency:
* @li hi_mdm_types.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
typedef HI_U32 (*HI_DMS_CHL_RX_F)(HI_U16, HI_DMS_CHL_RX_S*);

typedef HI_U32 (*HI_DMS_CHL_IREMOTE_PACKET_F)(HI_U32, HI_PVOID, HI_U16);


/**
 * @ingroup hct_dms
 * ͨ���ӿ�ʵ���ṹ
 */
typedef struct
{
    HI_DMS_CHL_CONNECT_F         pfnChlConnect;         /**< ͨ������ */
    HI_DMS_CHL_RX_F              pfnChlDataRx;          /**< �������� */
    HI_DMS_CHL_IREMOTE_PACKET_F  pfnChlIRemoteDataRx;   /**< ��δʹ�� */
    HI_DMS_CHL_MSG_IND_F         pfnChlMsgRx;           /**< ͨ����Ϣ���� */
} HI_DMS_CHL_PORT_OBJ_S;

#define HI_DMS_DEFAULT_DEV_CHL_IF_VALUE  { HI_NULL, HI_NULL, HI_NULL, HI_NULL}

#if defined(PRODUCT_CFG_COMPATIBILITY_NEW_FEATURE)
#define HI_DMS_FRM_INTER_INFO1_SIZE        (sizeof(HI_U8) + sizeof(HI_U8)) // mmt, vt
#define HI_DMS_FRM_INTER_INFO2_SIZE        (4) // ���ÿ����ֶγ���
#else
#define HI_DMS_FRM_INTER_INFO1_SIZE        (HI_PLC_MAC_ADDR_LEN * 2 + sizeof(HI_U32) + sizeof(HI_U16)) // mac, mmt, vt
#define HI_DMS_FRM_INTER_INFO2_SIZE        (8) // ���ÿ����ֶγ���
#endif
#define HI_DMS_INTER_INFO_SIZE             (HI_DMS_FRM_INTER_INFO1_SIZE + HI_DMS_FRM_INTER_INFO2_SIZE)
#define HI_DMS_FRM_INTER_MAX_PAYLOAD_SIZE_PV100 (1994)
#define HI_DMS_FRM_INTER_MAX_PAYLOAD_SIZE_PSG   (2014)
#define HI_DMS_FRM_MAX_PAYLOAD_SIZE_PV100       (HI_DMS_FRM_INTER_MAX_PAYLOAD_SIZE_PV100-HI_DMS_FRM_INTER_INFO2_SIZE)
#define HI_DMS_FRM_MAX_PAYLOAD_SIZE_PSG         (HI_DMS_FRM_INTER_MAX_PAYLOAD_SIZE_PSG-HI_DMS_FRM_INTER_INFO2_SIZE)
#define HI_DMS_FRM_INTER_MAX_SIZE_PV100         (HI_DMS_FRM_INTER_MAX_PAYLOAD_SIZE_PV100+HI_DMS_FRM_INTER_INFO1_SIZE)
#define HI_DMS_FRM_INTER_MAX_SIZE_PSG           (HI_DMS_FRM_INTER_MAX_PAYLOAD_SIZE_PSG+HI_DMS_FRM_INTER_INFO1_SIZE)
#define HI_DMS_FRM_MAX_PAYLOAD_SIZE             ((HI_MDM_PTCL_VER_100 == HI_MDM_GetProtocolVer()) ? (HI_DMS_FRM_MAX_PAYLOAD_SIZE_PV100):(HI_DMS_FRM_MAX_PAYLOAD_SIZE_PSG))
#define HI_DMS_FRM_INTER_MAX_SIZE               ((HI_MDM_PTCL_VER_100 == HI_MDM_GetProtocolVer()) ? (HI_DMS_FRM_INTER_MAX_SIZE_PV100):(HI_DMS_FRM_INTER_MAX_SIZE_PSG))

/**
 * @ingroup hct_dms
 * ����ͨ�Ų���
 */
typedef struct
{
    HI_U16 usDataSize;                          /**< ���ݴ�С����ucData��ռ�ռ��С����λ�ֽ� */
    HI_U8 ucInsideInfo[HI_DMS_INTER_INFO_SIZE]; /**< �ڲ�ʹ�� */
    HI_U8 ucData[0];                            /**< ���� */
} HI_DMS_CHL_TX_DATA_S;

#if defined(PRODUCT_CFG_FEATURE_DIAG_PROXY)
#define HI_DMS_CHL_FRAME_HRD_SIZE    (sizeof(HI_DMS_CHL_TX_DATA_S))
#else
#define HI_DMS_CHL_FRAME_HRD_SIZE    0
#endif

#define HI_DMS_CHL_TX_TYPE_NA          0x00
#define HI_DMS_CHL_TX_TYPE_MR          0x01
#define HI_DMS_CHL_TX_TYPE_UPG         0x02
#define HI_DMS_CHL_TX_TYPE_DIAG        0x04
#define HI_DMS_CHL_TX_TYPE_LB          0x08 // ���ع㲥
#define HI_DMS_CHL_TX_TYPE_MAX         0x0F

#define HI_DMS_CHL_TX_CTRL_NA          0x00 // ȱʡ����
#define HI_DMS_CHL_TX_CTRL_CIPHER      0x01 // ����ģʽ
#define HI_DMS_CHL_TX_CTRL_FORCE       0x08 // ǿ�Ʒ���,�����Ƿ�����/ͨ���Ƿ����
#define HI_DMS_CHL_TX_CTRL_MAX         0x0F

/**
 * @ingroup hct_dms
 * PLCͨ�ſ��ƽṹ������ʹ�÷�����ο���ѹ�����߿���ز�ͨ�ż����淶��
 */
typedef struct
{
    HI_U16   usId;/**< ͨ�ñ��Ľṹ�ı���ID */
    HI_U16   usSn;/**< ��δʹ��*/
    HI_U8    ucCtrl;/**< ����ѡ����*/
    HI_U8    ucType;/**< ֡���ͣ�ȡֵ:HI_DMS_CHL_TX_TYPE_NAΪ��ָ��֡���ͣ�HI_DMS_CHL_TX_TYPE_MR��ʾ֡����Ϊ����
                                      HI_DMS_CHL_TX_TYPE_UPG��ʾ֡����Ϊ������HI_DMS_CHL_TX_TYPE_DIAG��ʾ֡����Ϊ��ϣ�0x04��0xFFԤ��*/
    HI_U8    ucPrio;/**< Vlan��ǩ*/
    HI_U8    ucDuration;/**< ��δʹ�� */
    HI_U8    ucMaxResendCount; /**< ����MAC�����ش����� */
    HI_U8    ucPhase;           /**< ���÷��͵���λֻ��CCO�ҹ㲥����ʱ��Ч,�����ָ����ָ����λ�����򰴵�ǰĬ����λ���͡�*/
    HI_BOOL  bIsBroadCast; /**< �Ƿ�ȫ���㲥����ucDestMacΪȫFFʱ��Ч��HI_TRUEΪȫ���㲥��HI_FALSEΪ����㲥 */
    HI_U8    ucPad;
    HI_U8    ucDestMac[HI_PLC_MAC_ADDR_LEN];  /**< Ŀ��ģ���MAC��ַ, 1)ȡֵȫΪ��, ��ʾ���͵�CCO; 2)ȫΪFF��ʾ�㲥 */
    HI_U16   usUserInfoSize;  /**< �û��Զ��������Ϣ��С */
    HI_PBYTE pucUserInfo;    /**< �û��Զ��������Ϣ */
} HI_DMS_CHL_TX_CTRL_S;

typedef HI_DMS_CHL_TX_CTRL_S HI_DMS_CHL_TX_ADDR_S;

#define HI_DIAG_PAYLOAD_MAX_SIZE   512

#define HI_DIAG_TYPE_A     0 // ��ʾHSO���з�����汾
#define HI_DIAG_TYPE_B     1 // ��ʾHSO���ֳ�ά���汾
#define HI_DIAG_TYPE_LLN  0x0F // Layer Level N/A
#define HI_DIAG_TYPE_SLN  0xF0 // System Level N/A
#define HI_DIAG_TYPE_SL0  0xF1 // System Level 0

// ��ӡ����
#define HI_DIAG_PRINT_LEVEL_NONE          (0x80000000)
#define HI_DIAG_PRINT_LEVEL_ERROR         (0x40000000)
#define HI_DIAG_PRINT_LEVEL_WARNING       (0x20000000)
#define HI_DIAG_PRINT_LEVEL_INFO          (0x10000000)
#define HI_DIAG_PRINT_LEVEL_NORMAL        (0x08000000)
#define HI_DIAG_PRINT_LEVEL_2             (0x04000000)
#define HI_DIAG_PRINT_LEVEL_3             (0x02000000)
#define HI_DIAG_PRINT_LEVEL_4             (0x01000000)

#define HI_DIAG_MODID(module_id, type)         ((module_id&0x00FFFFFF) | type)
#define HI_DIAG_ID(module_id, type)            ((module_id&0x00FFFFFF) | type)

#ifdef HI_HAVE_DIAG_TXT_ID
#ifndef HI_DIAG_FILE_ID
#error 'DIAG_FILE_ID is not defined! Please define at the top of your file __FILE__'.
#endif
#define HI_DIAG_TXT(str)   (HI_PCSTR)(HI_DIAG_FILE_ID+__LINE__)
#else
#define HI_DIAG_TXT(str)   str
#endif

typedef struct
{
    HI_U16 usId;         // Specify the message id.
    HI_U16 usSrcTei;     // the source device TEI
    HI_U16 usDestTei;    // the destination device TEI
    HI_U16 usDataSize;   // the data size in bytes.
    HI_PVOID pData;      // Pointer to the data buffer.
} HI_DIAG_AIR_MSG_S;

typedef struct
{
    HI_U16 usId;    // Specify the message id.
    HI_U16 usSrcModId;
    HI_U16 usDestModId;
    HI_U16 usDataSize;     // the data size in bytes.
    HI_PVOID pData;        // Pointer to the data buffer.
} HI_DIAG_LAYER_MSG_S;

#define HI_DIAG_USERPLANE_MSG_DT_LBUF   0  // Local data buffer
#define HI_DIAG_USERPLANE_MSG_DT_GBUF   1  // Global data buffer
#define HI_DIAG_USERPLANE_MSG_DT_DBUF   2  // alloc data buffer
#define HI_DIAG_USERPLANE_MSG_DT_D64    3  // 8bytes parameters

typedef struct
{
    HI_U32   ulId;         // Specify the message id.
    HI_U16   usDataType;   // Data type HI_DIAG_USERPLANE_MSG_DT_XXX(like HI_DIAG_USERPLANE_MSG_DT_LBUF)
    HI_U16   usDataSize;   // the data size in bytes.
    HI_PVOID pData;        // Pointer to the data buffer.
} HI_DIAG_USERPLANE_MSG_S;


/**
* @ingroup  hct_diag
* @brief  ��ȡ�洢�������ݻ���ռ䡣
*
* @par ����:
* �������յ�HSO������ж�Ϊ��������ʱ��ͨ�����øú�����ȡ�洢�������ݵĿռ䡣
* @attention
* @li ���Ǳ�Ҫ���������û�ʹ�øýӿڡ�
* @li DIAG����ִ����Ϻ�����Ŀռ���Ҫͨ���û��ص����������ͷţ�����ʹ�÷�����μ���Hi3911V200 API ����ָ�ϡ���
*
* @param  usId [IN] ����ID��
* @param  ulCacheSize [IN] �������ݴ�С��
* @param  usPkSn [IN] ���ݰ����кš�
*
* @retval HI_NULL ��ȡ�ռ�ʧ�ܡ�
* @retval ��HI_NULL ��ȡ�ռ�ɹ���
*
* @par Dependency:
* @li hi_mdm_diag.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
typedef HI_PVOID (*HI_DIAG_GET_CMD_PK_CACHE_F) (HI_U16 usId, HI_U32 ulCacheSize, HI_U16 usPkSn);

/**
* @ingroup  hct_diag
* @brief �û����������
*
* @par ����:
* DIAG�����յ������е�����ID�����ö�Ӧ�����������
* @attention �ޡ�
*
* @param  ulId[IN] ����ID��
* @param  ulId[IN] ��������ָ�롣
* @param  ulId[IN] �������ݴ�С��
* @param  ulId[IN] ulOption����ѡ��û��ϱ�Ӧ����ʱ���յ�option���ݸ���Ӧ���ϱ����ݽӿڡ�
*
* @retval ��������HI_ERR_NOT_FOUND����������������е���HI_MDM_SendAckPacket�ظ�ACKʱ���践��HI_ERR_CONSUMED��
*
* @par Dependency:
* @li hi_mdm_diag.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
typedef HI_U32(*HI_DIAG_CMD_F) (HI_U16 ulId, HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption);

/**
* @ingroup  hct_diag
* @brief ���������������ص�������
*
* @par ����:
* ���������������ص�������
* @attention �ޡ�
*
* @param  ulId[IN] ����ID��
* @param  usPkSn[IN] �������š�
* @param  pCache[IN] ָ���ڽ��յ�����ʱ���û�����Ŀռ䡣
* @param  ulOption[IN] ulOption����ѡ�
*
* @retval ִ�н����
*
* @par Dependency:
* @li hi_mdm_diag.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
#if defined(PRODUCT_CFG_FEATURE_REG_PARALLEL_CMD)
typedef HI_U32(*HI_DIAG_CMD_PARALLEL_CTRL_F) (HI_U16 ulId, HI_U16 usPkSn, HI_PVOID pCache, HI_U32 ulOption);
#endif

#define HI_DIAG_INPUT_CMD_PROC_F HI_DIAG_CMD_F

/**
 * @ingroup hct_diag
 * ����ID���䴦������Ӧ��ϵ
 */
typedef struct
{
    HI_U32 ulMinId; /**< ��ʼ����ID*/
    HI_U32 ulMaxId; /**< ��������ID*/
    HI_DIAG_CMD_F pfnInputCmd; /**< �������*/
    #if defined(PRODUCT_CFG_FEATURE_REG_PARALLEL_CMD)
    HI_DIAG_CMD_PARALLEL_CTRL_F pfnCmdCtrl;/**< ������Ϊ��������ʱ������ִ����Ϻ���õĻص�����*/
    #endif
} HI_DIAG_CMD_REG_OBJ_S;


#define HI_DIAG_INPUT_CMD_PROC_S HI_DIAG_CMD_REG_OBJ_S

#define HI_DIAG_CMD_INSTANCE_DEFAULT      ((HI_U8)0) // ����ʵ��  -->HSO
#define HI_DIAG_CMD_INSTANCE_LOCAL        HI_DIAG_CMD_INSTANCE_DEFAULT // -->HSO
#define HI_DIAG_CMD_INSTANCE_IREMOTE      1 // ע���Ӵ����Զ������ʵ��(����MAC͸������������), ������ע��ľ���MAC͸�������Զ������ -->...-->...
#define HI_DIAG_CMD_INSTANCE_DREMOTE      2 // ע��ֱ�Ӵ����Զ������ʵ��(���ܻ�͸��������վ��), ������ע��Ĳ���MAC͸���ʹ����Զ������ -->HSO
#define HI_DIAG_CMD_INSTANCE_DREMOTE_NDM  3 // ע�����NDMԶ������ʵ��
#define HI_DIAG_CMD_INSTANCE_PLC          4
#define HI_DIAG_CMD_INSTANCE_LOCAL_ANY    5 // ���ض˿ڷ��� �����ͨ���Ƿ����
#define HI_DIAG_CMD_INSTANCE_UNKNOWN      0xFF // unknown instance

#define HI_DIAG_OBJ_ID_MASK                     (0x07)      //�û�����ID����Чλ��
#define HI_DIAG_INSTANCE_ID_MASK                (0x0f)      //ͨ��ģʽID����Чλ��
#define HI_DIAG_OBJ_ID_OFFSET_IN_INSTANCE_ID    (4)         //�û�����ID��ͨ��ģʽID�е�ƫ��
#define HI_DIAG_GET_OBJ_ID_FROM_INSTANCE_ID(instanceId) \
            ((instanceId >> HI_DIAG_OBJ_ID_OFFSET_IN_INSTANCE_ID) & HI_DIAG_OBJ_ID_MASK)
#define HI_DIAG_OBJ_ID_OFFSET_IN_DIAGCHLREMOTERX_ID     (28)        //�û�����ID��diagChlRemoteRx()����������е�ƫ�ƣ���������


#define HI_DIAG_REG_CMD_TBL(tbl)              tbl, HI_ARRAY_COUNT(tbl)
#define HI_DIAG_IS_DREMOTE_PACKET(x)          ((((HI_U8)x) & HI_DIAG_INSTANCE_ID_MASK) == ((HI_U8)HI_DIAG_CMD_INSTANCE_DREMOTE))
#define HI_DIAG_IS_IREMOTE_PACKET(x)          ((((HI_U8)x) & HI_DIAG_INSTANCE_ID_MASK) == ((HI_U8)HI_DIAG_CMD_INSTANCE_IREMOTE))
#define HI_DIAG_IS_LOCAL_PACKET(x)            ((((HI_U8)x & HI_DIAG_INSTANCE_ID_MASK)) == ((HI_U8)HI_DIAG_CMD_INSTANCE_LOCAL))
#define HI_DIAG_IS_VALID_PACKET_OPTION(x)     (HI_DIAG_IS_DREMOTE_PACKET(x) || HI_DIAG_IS_DREMOTE_PACKET(x) || HI_DIAG_IS_DREMOTE_PACKET(x))

#ifndef PRODUCT_CFG_BUILD_DATE
#define PRODUCT_CFG_BUILD_DATE "2017-10-01"
#endif
#ifndef PRODUCT_CFG_BUILD_TIME
#define PRODUCT_CFG_BUILD_TIME "10:00:01"
#endif

#define HI_SAL_SYS_DFX_VER_BUILD_DATETIME()   (HI_PCSTR)(PRODUCT_CFG_BUILD_DATE " " PRODUCT_CFG_BUILD_TIME)
#define HI_SAL_SYS_DFX_VER_STR(x,y,z)         (HI_PCSTR)(#x"."#y"."#z)

#define HI_SAL_DFX_HOST_CTRL_MR_SIMU_DISABLE   0
#define HI_SAL_DFX_HOST_CTRL_MR_SIMU_ENABLE    1
#define HI_SAL_DFX_HOST_CTRL_PLC_SIMU_ENABLE   1
//****************************************************************************

#define HI_SYS_REBOOT_CAUSE_MRS_START (0x8000)
#define HI_SYS_REBOOT_CAUSE_USR_END	  (0xA000)

/**
* @ingroup hct_sys
 * ��������ԭ����
 */
typedef enum
{
    HI_SYS_REBOOT_CAUSE_UNKNOWN = 0,

    HI_SYS_REBOOT_CAUSE_D=0x1,
    HI_SYS_REBOOT_CAUSE_C,
    HI_SYS_REBOOT_CAUSE_UPG_C, // upg complete
    HI_SYS_REBOOT_CAUSE_UPG_B, // ver back
    HI_SYS_REBOOT_CAUSE_WD,
    HI_SYS_REBOOT_CAUSE_UPG_SOFT, // ���������У�MAC��λ��CCO������λ

    HI_SYS_REBOOT_CAUSE_MAC_EXIT_NETWORK_TEST_MODE=0x100,
    HI_SYS_REBOOT_CAUSE_MAC1,
    HI_SYS_REBOOT_CAUSE_MAC2,
    HI_SYS_REBOOT_CAUSE_MAC3,
    HI_SYS_REBOOT_CAUSE_MAC4,
    HI_SYS_REBOOT_CAUSE_MAC5,
    HI_SYS_REBOOT_CAUSE_MAC6,

    HI_SYS_REBOOT_CAUSE_HRTIMER0=0x200,
    HI_SYS_REBOOT_CAUSE_HRTIMER1,
    HI_SYS_REBOOT_CAUSE_HRTIMER2,
    HI_SYS_REBOOT_CAUSE_HRTIMER3,
    HI_SYS_REBOOT_CAUSE_HRTIMER4,
    HI_SYS_REBOOT_CAUSE_HRTIMER5,
    HI_SYS_REBOOT_CAUSE_HRTIMER6,

#if ((!defined(PRODUCT_CFG_BOARD_TYPE_ASIC)) && (defined(PRODUCT_CFG_HW_CHECK)))
	HI_SYS_REBOOT_CAUSE_FPGA_TEMPERATURE_ALARM_RST = 0x300,
	HI_SYS_REBOOT_CAUSE_FPGA_ACTIVE_RST,// ���峤ʱ���¶��쳣�������޷�����������24Сʱ����������
#endif
    HI_SYS_REBOOT_CAUSE_RST_MIN,
    HI_SYS_REBOOT_CAUSE_RST0 = 0x1000, // RST MGR
    HI_SYS_REBOOT_CAUSE_RST1,
    HI_SYS_REBOOT_CAUSE_RST2,
    HI_SYS_REBOOT_CAUSE_RST_MAX = 0x1100,

    HI_SYS_REBOOT_CAUSE_MRS0 = HI_SYS_REBOOT_CAUSE_MRS_START,
    HI_SYS_REBOOT_CAUSE_MRS1,
    HI_SYS_REBOOT_CAUSE_MRS2,
    HI_SYS_REBOOT_CAUSE_MRS10,
    HI_SYS_REBOOT_CAUSE_MRS11,    
    HI_SYS_REBOOT_CAUSE_MRS15,


    HI_SYS_REBOOT_CAUSE_USR0 = 0x9000,
    HI_SYS_REBOOT_CAUSE_USR1,
    HI_SYS_REBOOT_CAUSE_USR2,

    HI_SYS_REBOOT_CAUSE_MAX = 0xF000
}HI_SYS_REBOOT_CAUSE_E;


//****************************************************************************
//
// HI_SDM_VERIFY_EXPR  : ���ʽ���
// HI_SDM_VERIFY_RA    : RAM��ַ���
// HI_SDM_VERIFY_CA    : �����ַ���
// HI_SDM_VERIFY_PTR   : ָ����
//
//****************************************************************************
HI_EXTERN HI_VOID  HI_SYS_FatalVerifyPrv(HI_PCSTR szExp, HI_U32 ulUsrData1, HI_U32 ulUsrData2, HI_PCSTR pszFile, HI_U32 ulLn, HI_PCSTR pszFunc);
#define HI_SDM_VERIFY_EXPR(_expr, _usrdata1, _usrdata2) \
do{\
    if (!(_expr)) { HI_SYS_FatalVerifyPrv((HI_PCSTR)#_expr, (HI_U32)_usrdata1, (HI_U32)_usrdata2, (HI_PCSTR)0, (HI_U32)__LINE__, (HI_PCSTR)__FUNCTION__); } \
}while(0)

#define HI_SDM_VERIFY_CA(_addr) \
do{\
  if (((HI_U32)(_addr)) <= HI_ADDR_CODE_RAM)\
  { HI_SYS_FatalVerifyPrv((HI_PCSTR)#_addr, (HI_U32)_addr, (HI_U32)HI_SYS_GET_LR(), (HI_PCSTR)0, (HI_U32)__LINE__, (HI_PCSTR)__FUNCTION__); }\
}while(0)

HI_EXTERN HI_VOID HI_SYS_PtrFatalVerifyPrv(HI_PCSTR szExp, HI_U32 ulUsrData1, HI_U32 ulUsrData2, HI_U32 ulUsrData3, HI_U32 ulUsrData4, HI_U32 ulLn, HI_PCSTR pszFunc);
#define HI_SDM_VERIFY_PTR(exp, _ptr, _v1, _v2) \
do{\
    HI_SYS_PtrFatalVerifyPrv(exp, _ptr, _v1, _v2, (HI_U32)HI_SYS_GET_LR(), (HI_U32)__LINE__, (HI_PCSTR)__FUNCTION__);\
}while(0)

#define HI_SDM_VERIFY_RA(_addr, _v3, _v4) \
do{\
  if((((HI_U32)(_addr)) < HI_SYS_ADDR_USER_RAM) || (((HI_U32)(_addr)) >= HI_SYS_RAM_MAX_ADDR))\
  { HI_SYS_PtrFatalVerifyPrv((HI_PCSTR)#_addr, (HI_U32)_addr, (HI_U32)HI_SYS_GET_LR(), (HI_U32)_v3, (HI_U32)_v4, (HI_U32)__LINE__, (HI_PCSTR)__FUNCTION__); }\
}while(0)

#if defined(PRODUCT_CFG_SDM_CHECK_MORE_ADDR)
#define HI_SDM_IS_INVALID_RA(_addr) ((((HI_U32)(_addr)) < HI_SYS_ADDR_USER_RAM) || (((HI_U32)(_addr)) >= HI_SYS_RAM_MAX_ADDR))
#define HI_SDM_IS_INVALID_CA(_addr) (((HI_U32)(_addr)) <= HI_ADDR_CODE_RAM)
#else
#define HI_SDM_IS_INVALID_RA(_addr) ((((HI_U32)(_addr)) < HI_ADDR_CODE_RAM))
#define HI_SDM_IS_INVALID_CA(_addr) (((HI_U32)(_addr)) <= HI_ADDR_CODE_RAM)
#endif
//****************************************************************************


//****************************************************************************
typedef struct
{
    HI_U8 ucMrLoopBack;         // 0: ��ʹ��loopback��ʽ;  1: ��ʾʹ��loopback��ʽ;
    HI_U8 ucMrRedirect2HsoPort; // 1: ʹ��DIAG��tcp�˿����uart�˿ڷ���MR֡; 2: ʹ��HSO��UARTͨ��
    HI_U8 ucPlcFrameBypassMac;  // 1: ʹ��ģ�ⷽʽ���MAC��SendFrame; 0: ��ʹ��ģ�ⷽʽ
    HI_U8 reserved; // Ԥ��
} HI_SAL_DFX_HOST_CTRL_S;

typedef enum
{
    HI_DFX_MAC_MSG_ROUTER = 0x200,
    HI_DFX_MAC_MSG_DIAG

    // ׷�ӵ���������ֵ, ����С�� 0x400
} HI_SAL_DFX_MAC_MSG_E;

typedef enum
{
    HI_DFX_OS_VER,
    HI_DFX_SAL_VER,
    HI_DFX_DRV_VER,
    HI_DFX_PHY_VER,
    HI_DFX_MAC_VER,
    HI_DFX_APP_VER
} HI_SYS_DFX_VER_E;

#define HI_USERDATA_ID_USR           0
#define HI_USERDATA_ID_APP           1
#define HI_USERDATA_ID_BIDB          2 // ��������Ϣ HI_SYS_SaveUserData HI_SYS_GetUserData ����ucUserDataIdȡֵ
#define HI_USERDATA_ID_CRASH_HRD     3
#define HI_USERDATA_ID_CRASH_DATA    4
#define HI_USERDATA_ID_CRASH_DATA1   5
#define HI_USERDATA_ID_CRASH_DATA2   6
#define HI_USERDATA_ID_CRASH_DATA3   7
#define HI_USERDATA_ID_TF_LIST       8
#define HI_USERDATA_ID_BLACK_LIST    9

typedef struct
{
    HI_U32 ulUpgId;          // �����ļ�ID
    HI_U32 ulOption;         // ȡֵΪ HI_UPG_REQ_OPTION_NA , HI_REQ_OPTION_CCO, HI_UPG_REQ_OPTION_STA , HI_UPG_REQ_OPTION_BROADCAST, HI_UPG_REQ_OPTION_UNICAST
} HI_UPG_REQ_OPTION_S;

typedef struct
{
    HI_U16 usId;
    HI_U16 usPayloadSize;
    HI_U8 aucPayload[0];
} HI_DIAG_PACKET_S;


#define HI_ND_SYS_STATUS_NORMAL      0 // ����ģʽ
#define HI_ND_SYS_STATUS_UPGRADE     1 // ����ģʽ
#define HI_ND_SYS_STATUS_TEST        2 // ����ģʽ
#define HI_ND_SYS_STATUS_DIAG        4 // ���ģʽ
#define HI_ND_SYS_STATUS_UNKNOWN     8 // δ֪ģʽ

#define  HI_MAC_NETWORK_TOPO_ENTRY_MAX       2046

typedef struct
{
    HI_U8  ucSysMode;
    HI_U8  ucBootVerIdx; // the boot version index which configured by user when startup.
    HI_U8  ucSysCause;   // HI_SDM_ENG_EVT_CAUSE_E
    HI_U8  aucVer[3];    // image.hupg
    HI_U8  aucBuildDataTime[6]; // code+nv image.hupg

    HI_U16 ausVer[2];    // code+nv image.hupg

    HI_U16 usChipVer;
    HI_U8  ucBootCause;  // HI_ND_SYS_BOOT_CAUSE_XX
    HI_U8  ucNvCVer;     // nv image.hupg: C Ver

    HI_U16 ausNvVer[2];  // nv image.hupg: B/SPC Ver

    HI_U16 usNvVerId;    // build version id for each version.
    HI_U16 usUpgOption;  // upg option configured by user when startup
} HI_SYS_STATUS_INFO_S_VER0;

// վ��汾��Ϣ
typedef struct
{
    HI_U8  ucSysMode;			/**<��������ʱ�����ڵĹ���ģʽ���ñ�����ʵʱ���¡�����ģʽ:#HI_ND_SYS_STATUS_NORMAL; ����ģʽ:#HI_ND_SYS_STATUS_UPGRADE; ����ģʽ:#HI_ND_SYS_STATUS_TEST; ���ģʽ:#HI_ND_SYS_STATUS_DIAG; δ֪ģʽ:HI_ND_SYS_STATUS_UNKNOWN;*/
    HI_U8  ucBootVerIdx; 		/**<��ʼ������BOOT�汾��*/
    HI_U8  ucSysCause;   		/**<������ԭ��*/
    HI_U8  aucVer[3];    		/**<V��R��C�汾��*/
    HI_U8  aucBuildDataTime[6]; /**<����ʱ��*/

    HI_U16 ausVer[2];    		/**<B��SPC�汾��*/
    HI_U16 usChipVer;			/**<оƬ�汾�š�Hi3911v200оƬ:NM_CHIP_VER_HI3911_V200; Hi3911v100оƬ:NM_CHIP_VER_HI3911_V100; ����оƬ:NM_CHIP_VER_OTHER*/

    HI_U8  ucBootCause;  		/**<��������:#HI_ND_SYS_BOOT_CAUSE_NORMAL; �쳣����:#HI_ND_SYS_BOOT_CAUSE_EXP; ���Ź�����:#HI_ND_SYS_BOOT_CAUSE_WD*/

    HI_U8  ucBootVer;       	/**<Bootloader�汾*/
    HI_U16 usSwVer;         	/**<����汾��*/
    HI_U16 usYear  : 7;			/**<�汾����-��*/
    HI_U16 usMonth : 4;     	/**<�汾����-��*/
    HI_U16 usDay   : 5;     	/**<�汾����-��*/
    HI_CHAR szManuCode[2];  	/**<���̴���*/
    HI_CHAR szChipCode[2];  	/**<оƬ����*/
} HI_SYS_STATUS_INFO_S;

typedef struct
{
    HI_U16 ausNvVer[2];
    HI_U16 usNvVerId;
    HI_U8 ucNvVer[3];
    HI_U8 reserver[3];
}HI_SYS_NV_VER_INFO_S;//���nv��VR�汾��

//վ��������Ϣ
typedef struct
{
    HI_U8 mac[6];						/**<վ��MAC��ַ*/
    HI_U8 level;						/**<վ��㼶*/
    HI_U8 d2d_proxy_comm_rate;			/**<·��ͨ����*/

    HI_U16 tei;							/**<�豸Ψһ��ʾ��*/
    HI_U16 proxy_tei;					/**<�豸����*/

    HI_U8  role; 						/**<վ���ɫ��CCO:#HI_ND_TYPE_CCO; ������м�վ��:#HI_ND_TYPE_RELAY; STA:#HI_ND_TYPE_STA; ���ر�:HI_ND_TYPE_DCM;*/
    HI_U8  product_type;				/**<վ���Ʒ���ͣ�CCO:#HI_ND_TYPE_CCO; �м�:#HI_ND_TYPE_RELAY; STA:#HI_ND_TYPE_STA; ���ر�:HI_ND_TYPE_DCM;*/
    HI_U8  transformer_result;          /**<̨��ʶ������δʶ�� : #HI_MDM_TRANSFORMER_RESULT_UNKNOWN \n
                                                                   ���ڱ�̨�� : #HI_MDM_TRANSFORMER_RESULT_BELONG \n
                                                                   �����ڱ�̨�� : #HI_MDM_TRANSFORMER_RESULT_NOT_BELONG \n
                                                                   ��֧��ʶ�� : #HI_MDM_TRANSFORMER_RESULT_NOT_SUPPORT \n
                                                                   ʶ��ʧ�� : #HI_MDM_TRANSFORMER_RESULT_FAIL */
    HI_U8  phase_state  : 4;            /**<��λʶ��״̬\n
                                                                 δʶ��״̬ : #HI_MAC_PHASE_IDENTIFY_STATE_UNKNOWN \n
                                                                 ��֧����λʶ�� : #HI_MAC_PHASE_IDENTIFY_STATE_NOT_SUPPORT \n
                                                                 ����ʶ�� : #HI_MAC_PHASE_IDENTIFY_STATE_DOING \n
                                                                 ʶ����� : #HI_MAC_PHASE_IDENTIFY_STATE_OVER */
    HI_U8  phase_result : 4;            /**<����1����λʶ����\n
                                                                 A��λ : #PHASE_A \n
                                                                 B��λ : #PHASE_B \n
                                                                 C��λ : #PHASE_C \n
                                                                 ����λ : #INVALID_PHASE */

    HI_U8 mac_type;					  /**<MAC��ַ����*/
    HI_U8 phase_result_b : 4;         /**<����2����λʶ����\n
                                                                 A��λ : #PHASE_A \n
                                                                 B��λ : #PHASE_B \n
                                                                 C��λ : #PHASE_C \n
                                                                 ����λ : #INVALID_PHASE */
    HI_U8 phase_result_c : 4;         /**<����3����λʶ����\n
                                                                 A��λ : #PHASE_A \n
                                                                 B��λ : #PHASE_B \n
                                                                 C��λ : #PHASE_C \n
                                                                 ����λ : #INVALID_PHASE */
	HI_U8  not_rcv_heartbeat_cycle : 4;	/**<δ���յ�������������*/
    HI_U8  is_alloc_beacon_slot    : 1; /**<�Ƿ�����ű�ʱ϶*/
    HI_U8  rsvd1                   : 3;
    HI_U8 pad2;

    HI_SYS_STATUS_INFO_S sta_ver;		/**<�汾��Ϣ���μ�#HI_SYS_STATUS_INFO_S*/

    HI_U8  up_comm_rate;				/**<����ͨ�ųɹ���*/
    HI_U8  down_comm_rate;				/**<����ͨ�ųɹ���*/
    HI_U8  pn_state : 2;   				/**<�����ʶ��״̬\n
                                                                                δʶ��״̬ : #HI_MAC_PN_IDENTIFY_STATE_UNKNOWN\n
                                                                                ��֧��ʶ�� : #HI_MAC_PN_IDENTIFY_STATE_NOT_SUPPORT\n
                                                                                ����ʶ�� : #HI_MAC_PN_IDENTIFY_STATE_DOING\n
                                                                                ʶ����� : #HI_MAC_PN_IDENTIFY_STATE_OVER*/
    HI_U8  pn_result : 2;  				/**<����1�������ʶ����\n
                                                                                ����߽�����ȷ : #PN_CONNECTION_RIGHT\n
                                                                                ����߽��߷��� : #PN_CONNECTION_ERROR\n
                                                                                ����߽������δ֪ : #INVALID_PN_CONNECTION*/
    HI_U8  pn_result_b : 2;             /**<����2�������ʶ����\n
                                                                                ����߽�����ȷ : #PN_CONNECTION_RIGHT\n
                                                                                ����߽��߷��� : #PN_CONNECTION_ERROR\n
                                                                                ����߽������δ֪ : #INVALID_PN_CONNECTION*/
    HI_U8  pn_result_c : 2;             /**<����3�������ʶ����\n
                                                                                ����߽�����ȷ : #PN_CONNECTION_RIGHT\n
                                                                                ����߽��߷��� : #PN_CONNECTION_ERROR\n
                                                                                ����߽������δ֪ : #INVALID_PN_CONNECTION*/
    HI_U32 proxy_change_cnt;            /**<����������*/
    HI_U32 leave_cnt;                   /**<���ߴ���*/
    HI_U32 leave_total_time;            /**<���ߵ���ʱ��*/
} HI_MAC_NETWORK_TOPO_ENTRY_S;

typedef struct
{
    HI_U32 num;			/**<����Ԫ�ظ���*/
    HI_MAC_NETWORK_TOPO_ENTRY_S entry[HI_MAC_NETWORK_TOPO_ENTRY_MAX];	/**<����վ��������Ϣ*/
} HI_MAC_NETWORK_TOPO_S;

typedef struct
{
    HI_U8 ucLevel; /**<վ��㼶*/
    HI_PVOID pTopo;       /**<ָ�� #HI_MAC_NETWORK_TOPO_S �ṹ���ɵ������׵�ַ*/
} HI_MAC_CONFIG_INFO_S;

#define HI_HNV_FILE_SIGNATURE                HI_MAKE_IDENTIFIER('H','N','V','$')

#define HI_UPG_FILE_SIGNATURE                HI_MAKE_IDENTIFIER('H','i','s','i')

/**
 *@ingroup hct_upg
 * ��ʾ��ȫ�������ļ���ʽ��
 */
#define HI_UPGA_FILE_FMT_VER_1              0x01
/**
 *@ingroup hct_upg
 * ��ʾȫ�������ļ���ʽ��
 */
#define HI_UPGA_FILE_FMT_VER_BUPG           0xA1

#define HI_UPG_FILE_SECTION_ID_UNKNOWN      0x0000
#define HI_UPG_FILE_SECTION_ID_CODE_CCO     0xF001
#define HI_UPG_FILE_SECTION_ID_CODE_STA     0xF002
#ifdef PRODUCT_CFG_FEATURE_REMOTE_COMMUNICATION_TERMINAL
#define HI_FTM_PRODUCT_BOOTID_BASE HI_FTM_PRODUCT_APPID_RMT
#else
#define HI_FTM_PRODUCT_BOOTID_BASE HI_FTM_PRODUCT_APPID_MRS
#endif
#define HI_UPG_FILE_SECTION_ID_BOOT_ANY     (0x1001 + HI_FTM_PRODUCT_BOOTID_BASE_VALUE)

/**
 *@ingroup hct_upg
 * ��ʾĿ�����δָ���������ļ�����Ϊ�Ƿ���
*/
#define HI_UPG_FILE_MACHINE_UNKNOWN         0x00
/**
 *@ingroup hct_upg
 * ��ʾĿ�����ΪARM926EJ-S��
*/
#define HI_UPG_FILE_MACHINE_ARM926EJ_S      0x01
/**
 *@ingroup hct_upg
 * ��ʾĿ�����ΪX86������ģ�⻷����
*/
#define HI_UPG_FILE_MACHINE_X86             0x02

#define HI_UPG_FILE_BLK_TRANSMIT_DELIMITER  0x9F9F // �鴫�䶨���
#define HI_UPG_TRANSMIT_BLK_SIZE            (300)     // ��̶���С
#define HI_UPG_ACK_BLK_NUM                  (10)      // ÿ��10�鷴��HSO ACK
#define HI_UPG_TRANSMIT_BLK_SIZE_MAX        (1024*2)
#define HI_UPG_TRANSMIT_BLK_NUM_MAX         (1024)    // ��������
#define HI_UPG_TRANSMIT_FILE_INVALID        (0xFFF0)
#define HI_UPG_TRANSMIT_FILE_OK             (0xFFFF)
#define HI_UPG_TRANSMIT_FILE_SN_UNKNOWN     (~0)
#define HI_UPG_FILE_BLK_SIZE                (HI_UPG_TRANSMIT_BLK_SIZE*HI_UPG_ACK_BLK_NUM)//6000Bytes//(PRODUCT_CFG_FLASH_BLOCK_SIZE) // �����ļ��洢�Ŀ�̶���С
#define HI_UPG_FILE_BLK_NUM_MAX             (PRODUCT_CFG_UPG_FILE_MAX_SIZE/PRODUCT_CFG_FLASH_BLOCK_SIZE) // �����ļ��洢�Ŀ��������
#define HI_UPG_FILE_FULL_SIZE               0x1FFFFFF0

#define HI_UPG_FILE_CODE_RAM_ADDR_AUTO      0xFFFFFFFF

#define HI_UPG_FILE_UPG_END                 0xFFFFFFFF

#define HI_UPG_VERSION_NEW      1
#define HI_UPG_VERSION_BACK     2
#define HI_UPG_VERSION_ANY      3

/**
 *@ingroup hct_upg
 *�����ļ��汾��
*/
typedef struct
{
    HI_U8 ucVVerNo;    /**< V�汾�š�   */
    HI_U8 ucRVerNo;    /**< R�汾�š�   */
    HI_U16 usBVerNo;    /**< B�汾�š�   */
    HI_U8 ucCVerNo;    /**< C�汾�š�   */
    HI_U8 ucSpcNo;    /**< SPC�汾�š�   */
    HI_U8 ucPlatform;    /**< ƽ̨��ţ�3��ʾ8M RAMƽ̨��4��ʾ2M RAMƽ̨��   */
    HI_U8 ucProductNo;    /**< ��Ʒ��ţ�0xFF��ʾV100��Ʒ��0xFE��ʾV200��Ʒ��   */
} HI_UPG_FILE_VER_S;

/**
 *@ingroup hct_upg
 *Section�б�
*/
typedef struct
{
    HI_U32 ulId;    /**< Section IDֵ��0��ʾ��ʹ�ø�Section��   */
    HI_U32 ulOffset;    /**< Section���ucData��ƫ�ơ�   */
    HI_U32 ulSectionSize;    /**< Section���ȣ���λ��byte��   */
    HI_UPG_FILE_VER_S stVer;    /**< Section�汾��   */
} HI_UPG_SECTION_ID_S;

/**
 *@ingroup hct_upg
 * ����Sectionͬʱ��֧�ֵ�NV�����ļ��б�
*/
typedef struct
{
    HI_UPG_SECTION_ID_S astCfg[8];    /**< ͬʱ��֧�ֵ�NV�����ļ���   */
    //HI_U32 ulFlashAddress;        /**< ��Section���浽Flash�ĵ�ַ��Ŀǰ�汾��ʹ�ã����ֶ�Ԥ����   */
} HI_UPG_FILE_CFG_SECTION_S;

/**
 *@ingroup hct_upg
 * ����section�б����֧��8��Section��
*/
typedef struct
{
    HI_UPG_SECTION_ID_S astCfg[8];    /**< ͬʱ��֧�ֵ�CODE�ļ���   */
    uintptr_t ulRamAddress;    /**< ��Section���ص�RAM�ĵ�ַ��   */
} HI_UPG_FILE_CODE_SECTION_S;


typedef struct
{
    HI_U32 ulSectorSize;
    HI_U32 aulCodeAddr[2];
    HI_U32 ulCodeAreaSize;
    HI_U32 ulNvAddr;
    HI_U32 ulNvAreaSize;
    HI_U16 usNvItemSize;
    HI_U16 reserved;
    HI_U16 ausBoardType[2];
} HI_UPG_BOARD_CONFIG_S;

/**
 *@ingroup hct_upg
 * �����ļ�ͷ�ṹ���塣
*/
typedef struct
{
    HI_U32 ulMagic;    /**<ħ���֡�STA�����ļ�ΪHisi�����������ļ�ΪHISI��*/
    HI_U32 ulChecksum;    /**<CRC���飬�������ļ�ͷ��ulChecksum��Ա֮�󣬲�����ulChecksum��Ա�����������ݣ�����ΪulDataSize����CRCУ�顣*/
    HI_CHAR szDateTime[20];    /**<�����ļ����ɵ����ں�ʱ�䣬 ��20120517-18:01:00��*/
    HI_U8 ucMachine;    /**<Ŀ�������\n
								HI_UPG_FILE_MACHINE_ARM926EJ_S��ʾARM926EJ-S��\n
								HI_UPG_FILE_MACHINE_X86 ��ʾX86��������ģ�⻷����\n
								����ֵ����չʹ�á�*/
    HI_U8 ucNumberOfCodeSections;    /**<����Section������*/
    HI_U8 ucNumberOfCfgSections;    /**<����Section������ÿ��section��ʾ��ͬ���������������ͣ�ͨ��Section ID���֣���*/
    HI_U8 ucFmtVer;    /**<�ļ���ʽ�汾��HI_UPGA_FILE_FMT_VER_BUPG��ʾȫ��������HI_UPGA_FILE_FMT_VER_1��ȫ�������ļ���*/
    HI_UPG_FILE_CFG_SECTION_S stSecCfg;    /**<����Section �б�*/
    HI_CHAR szManuCode[2];    /**<���̴��롣HS����˼��*/
    HI_U16 reserved;    /**<������*/
    HI_UPG_FILE_CODE_SECTION_S stSecCode;    /**<����Section�б�*/
    HI_U32 aulSHA256[8];    /**<�����ļ�����SHA256У�顣*/
    HI_U32 ulDataSize;    /**<����Section�ܳ���, ��λ��byte��*/
} HI_UPG_FILE_HEADER_S;

#define HI_UPG_FILE_HEADER_SIZE  396

typedef struct
{
    HI_U32 ulUpgId;
    HI_U32 ulErrorCode;
    HI_U32 ulBlkSn;         // �����ļ��鷢�͵���ţ���0��ʼ.
    HI_U32 ulBlkIdx;        // �����ļ��鴫��������ţ���1��ʼ, ������������.
    HI_U32 ulSectionId;     // ������ Section ID, 0��ʾû��Section����.
    HI_U32 ulNextSectionId; // ��ʼ������һ��Section, 0��ʾ�Ѿ�û�п���Section������.
    HI_U32 ulOption;
} HI_DRV_UPG_NOTIFY_S;

typedef HI_U32 (*HI_UPDATE_NOTIFY_PROC_F)(HI_DRV_UPG_NOTIFY_S*);

typedef struct
{
    HI_U32 ulVersionType;
    HI_U32 ulCodeFlashAddr;
    HI_U32 ulCfgFlashAddr;
    HI_U32 ulStatus;
    HI_U32 ulLastBlkIdx;
    HI_U32 ulRcvFileSize;
    HI_U32 ulUpgId;
    HI_UPG_FILE_HEADER_S stFileHeader; // �����������ļ���Ϣ
} HI_DRV_UPG_FALSH_BACKUP_INFO_S; // ������

typedef struct
{
    HI_U32 ulVersionType;
    HI_U32 ulCodeFlashAddr;
    HI_U32 ulCfgFlashAddr;
    HI_U32 ulUpgId;
    HI_UPG_FILE_HEADER_S stFileHeader; //  ������ �����ļ���Ϣ
} HI_DRV_UPG_FLASH_RUNING_INFO_S;  // ������

typedef struct
{
    HI_U32 ulSysStatus;
    HI_DRV_UPG_FLASH_RUNING_INFO_S stFlashRunInfo;    // ��������Ϣ
    HI_DRV_UPG_FALSH_BACKUP_INFO_S stFlashBackupInfo; // ��������Ϣ
} HI_DRV_STARTUP_INFO_S;

typedef struct
{
    HI_U32 ulOption;
    HI_U32 ulUpgId;
    HI_U32 ulFileLen;
    HI_U32 ulBlkNum;
    HI_U32 ulUpgTimeWindow;
} HI_UPG_START_S;

typedef struct
{
    HI_U32 ulUpgId;
    HI_U16 usBlkReqSn;
    HI_U16 usBlkIdx;  // �鴫���������
    HI_U16 usBlkSize;
    HI_U8* pucFileBlk;
} HI_UPG_FILE_BLK_UPDATE_S;

typedef struct
{
    HI_U32 ulUpgId;
    HI_U16 usBlkReqSn;
    HI_U16 usBlkIdx;  // �鴫���������
    HI_U16 usBlkSize;
    HI_U8* pucFileBlk;
} HI_UPG_FILE_BLK_INFO_S;


//#endif // UPG ONLY
//*****************************************************************************

// ����Ϊ��������
#define  SAL_ANY_MR_FAIL_TIME           (24 * 60 * 60)      // Ĭ��ֵ24Сʱ*60��*60��      ��Ӧ HI_SYS_RST_CON_ANY_MR_FAIL
#define  SAL_ANY_MR_FAIL_SCAL           (30)                // Ĭ��ֵ30���                ��Ӧ HI_SYS_RST_CON_ANY_MR_FAIL
#define  SAL_NO_FRAME_TIME              (24 * 60 * 60)      // Ĭ��ֵ24Сʱ*60��*60��      ��Ӧ HI_SYS_RST_CON_NOT_GET_FARME
#define  SAL_STA_NO_UART_DATA_TIME      (12 * 60 * 60)      // Ĭ��ֵ12Сʱ   ��Ӧ HI_SYS_RST_CON_NOT_GET_METER_UART_DATA
#define  SAL_MAX_PROXY                  (150)               // Ĭ��ֵ150����������             ��Ӧ HI_SYS_RST_CON_CCO_TOO_MANY_AGENT
#define  SAL_MAX_PROXY_TIME             (12 * 60  *60)      // Ĭ��ֵ150������ ����ʱ��            ��Ӧ HI_SYS_RST_CON_CCO_TOO_MANY_AGENT
#define  SAL_STA_NO_BEA_OR_SOF_TIME     (30 * 60)           // Ĭ��ֵ30��*60��             ��Ӧ HI_SYS_RST_CON_STA_NOT_TX HI_SYS_RST_CON_STA_NOT_RX
#define  SAL_CCO_NO_BEA_OR_SOF_TIME     (24 * 60 * 60)      // Ĭ��ֵ24Сʱ*60��*60��      ��Ӧ HI_SYS_RST_CON_CCO_NOT_TX HI_SYS_RST_CON_CCO_NOT_RX

#define  SAL_STA_SECURE_RST_DEFUAT_TIME (135 * 24 * 60 * 60)// Ĭ��ֵ135��  ��Ӧֵ HI_SYS_RST_CON_STA_SECURE_RESET
#define  SAL_CTL_II_NO_UART_DATA_TIME   (2 * 24 * 3600)     // Ĭ��2�� ��Ӧֵ HI_SYS_RST_CON_NOT_GET_METER_UART_DATA_CLT_II
#define  SAL_STA_NO_PLC_DATA_TIME       (2 * 24 * 3600)     // Ĭ��2�� ��Ӧֵ HI_SYS_RST_CON_NOT_GET_METER_PLC
#define  SAL_SYS_RESET_TIME_24_HOURS    (24 * 60 * 60)      // ����ϵͳ������ ��Ӧ  HI_SYS_RST_CON_TIMER_EXHAUST  -> HI_SYS_RST_CON_MEMPOOL_ERR
#define  SAL_SYS_MAC_EXCE_TIME_24_HOURS (24 * 60)           // MACЭ��ջ�쳣ʱ�䣬��λ����

typedef enum
{
    HI_SYS_RST_CON_UNKNOWN = 0,                     // �޸�0Ϊ1����ֹ����Ĭ����д����FLASHд��ֵΪ0���Բ��������κΰ���

    HI_SYS_RST_CON_ANY_MR_FAIL,                     // CCO ���б��޷���ͨ, ����ģ����30������ʱ��24Сʱ��
    HI_SYS_RST_CON_NOT_GET_FARME,                   // CCO �����ܳ�ʱ����ղ����������ĳ�����,��ʱ��һ�죩����ʵ�����޳���ҵ����Ӱ�졣
    HI_SYS_RST_CON_NOT_GET_METER_UART_DATA_STA,     // STA(����II��) ͬ����ڳ�ʱ��ʧ�� 12Сʱ

    HI_SYS_RST_CON_CCO_TOO_MANY_AGENT,              // CCO 3.2M�������վ�㳬��xx�����ҳ���12Сʱ
    HI_SYS_RST_CON_STA_NOT_TX,                      // STA ���ղ���SOF����Beacon ����90���ӣ��װ巽������λ TODO
    HI_SYS_RST_CON_STA_NOT_RCV_BEA_SOF,             // STA ���ղ���SOF���ҽ��ղ���Beacon ����30���ӣ��װ巽������λ TODO
    HI_SYS_RST_CON_STA_NOT_RCV_BEA,                 // STA ���ղ���Beacon ����30���ӣ��װ巽������λ TODO
    HI_SYS_RST_CON_STA_NOT_RCV_SOF,                 // STA ���ղ���SOF ����30���ӣ��װ巽������λ TODO
    HI_SYS_RST_CON_CCO_NOT_TX,                      // CCO ���ղ���SOF����Beacon ����8Сʱ���װ巽������λ TODO
    HI_SYS_RST_CON_CCO_NOT_RCV_BEA_SOF,             // CCO ���ղ���SOF����Beacon ����8Сʱ���װ巽������λ TODO
    HI_SYS_RST_CON_CCO_NOT_RCV_BEA,                 // CCO ���ղ���Beacon ����8Сʱ���װ巽������λ TODO
    HI_SYS_RST_CON_CCO_NOT_RCV_SOF,                 // CCO ���ղ���SOF ����8Сʱ���װ巽������λ TODO
    HI_SYS_RST_CON_STA_SECURE_RESET,                // �������(��ֹ���峤�����в����Ĳ��ɻָ�����) 90��-180��
    HI_SYS_RST_CON_NOT_GET_METER_UART_DATA_CLT_II,  // II��ͬ����ڳ�ʱ��ʧ�� 48Сʱ
    HI_SYS_RST_CON_NOT_GET_METER_PLC,               // STA 48Сʱδ�յ�CCO�·��ĳ���֡
    HI_SYS_RST_CON_TIMER_EXHAUST,                   // TIMER��Դ�ľ�,24Сʱ������
    HI_SYS_RST_CON_JOBADD_EXHAUST,                  // �жϺ�����Դ�ľ���24Сʱ������
    HI_SYS_RST_CON_MEM_NOT_ENOUGH,                  // �ڴ治��,24Сʱ������
    HI_SYS_RST_CON_MEMPOOL_ERR,                     // ��̬�ڴ�ؼ�����,24Сʱ������
    HI_SYS_RST_CON_MAC_BUFFER_ERR_CCO,              // CCO MAC BUFFER����ˮ��,����BUFFER����С��20��������24Сʱ������
    HI_SYS_RST_CON_MAC_BUFFER_ERR_STA,              // STA MAC BUFFER����ˮ�ޣ�����BUFFER����С��20��������24Сʱ������
    HI_SYS_RST_CON_NOT_JOIN_NETWORK_ERR,            // STA δ�������磬����24Сʱ������
    HI_SYS_RST_CON_ROUTE_EVALUATE_ERR,              // STA δ����·������������24Сʱ������
    HI_SYS_RST_CON_MAX
} HI_SYS_RST_TRIGGER_CON_E;

typedef enum
{
    HI_SYS_RST_CAT_UNKNOWN = 0,
    HI_SYS_RST_CAT_SYS,
    HI_SYS_RST_CAT_PLC,
    HI_SYS_RST_CAT_LOCAL,
    HI_SYS_RST_CAT_MAX
}HI_SYS_RST_TRIGGER_CAT_E;

#define HI_SYS_RST_DELAY_CON_USER_ST 0
#define HI_SYS_RST_DELAY_CON_USER_END 8
typedef enum
{
    HI_SYS_RST_DELAY_CON_WR_NV,// дNV������У��ڳ�ʱ�ڣ���ǿ������
    HI_SYS_RST_DELAY_CON_WR_MR_DOC, // д���������У��ڳ�ʱ�ڣ���ǿ������
	HI_SYS_RST_DELAY_CON_USR_RESERVED_2,
	HI_SYS_RST_DELAY_CON_USR_RESERVED_3,
	HI_SYS_RST_DELAY_CON_USR_RESERVED_4,
	HI_SYS_RST_DELAY_CON_USR_RESERVED_5,
	HI_SYS_RST_DELAY_CON_USR_RESERVED_6,
	HI_SYS_RST_DELAY_CON_USR_RESERVED_7,

    HI_SYS_RST_DELAY_CON_UPG = HI_SYS_RST_DELAY_CON_USER_END,       // ����������, ��ǿ��������
	HI_SYS_RST_DELAY_CON_BOOT_UPG,
    HI_SYS_RST_DELAY_CON_NO_PB,     // �޷��յ�PB�飬����Ƶƫ���ó�ʱ�䳬ʱ��1000���ӣ�����ǿ������
    // ����������
    // ��
    // ��������
    // CCO��д����ϵ�кŵ�ʱ�򣬲�����������
    HI_SYS_RST_DELAY_CON_SUIT_NET,
    // CCO/STA��дSNID��ʱ�򣬲�����������
    HI_SYS_RST_DELAY_CON_WRT_SNID,
    // STA��д�����������ʱ�򣬲���������
    HI_SYS_RST_DELAY_CON_RELATING_RANDOM_NUM,
    // STA�ڳ������������������ʱ������������
    HI_SYS_RST_DELAY_CON_NDM_METER_CHECK,
    // ̨��ʶ��ʱ����������
    HI_SYS_RST_DELAY_CON_TF_IDENTIFY,
    // STA�ϵ�11Сʱ�ڣ���������״̬�£�����������
    HI_SYS_RST_DELAY_CON_LOCK_NETWORK,

	// �������ģʽ�� 24 Сʱ�ڲ���������
    HI_SYS_RST_DELAY_ENTER_PHY_TEST_MODE,

    HI_SYS_RST_DELAY_CON_MAX
}HI_SYS_RST_DELAY_CON_E;

 // �ӳٶ��� ��λ��


#define HI_SYS_RST_DELAY_CON_WR_NV_TO 10000
#define HI_SYS_RST_DELAY_CON_WR_MR_DOC_TO 10000// ms
#define HI_SYS_RST_DELAY_CON_BOOT_UPG_TO (120*1000)//120s
// CCO��д����ϵ�кŵ�ʱ�򣬲�����������
#define HI_SYS_RST_DELAY_CON_SUIT_NET_TO (60*1000)
// STA��д�����������ʱ�򣬲���������
#define HI_SYS_RST_DELAY_CON_RELATING_RANDOM_NUM_TO (60*1000)
// STA�ڳ������������������ʱ����ʱ24Сʱ������
#define HI_SYS_RST_DELAY_CON_NDM_METER_CHECK_TO (24 * 60 * 60 * 1000)

// STA�ڽ������ģʽʱ����ʱ24Сʱ������
#define HI_SYS_RST_DELAY_ENTER_PHY_TEST_MODE_TO (24 * 60 * 60 * 1000)

// �����ͶƱ���ؽ��,Ĭ��ΪHI_SYS_ENUM_RST_NO_REQ������
typedef enum
{
    HI_SYS_ENUM_RST_NO_REQ    = 0,// ������Ҫ��
    HI_SYS_ENUM_RST_DO_PLEASE,    // ��Ҫ��������
    HI_SYS_ENUM_RST_NOT_PLEASE,   // ��ʱ��������
    HI_SYS_ENUM_RST_MAX
}HI_SYS_RST_COMPONENT_VOTE_E;

// ͶƱ���ص����ݽṹ
#define SAL_RST_TYPE_CCO             2 // ͷ��
#define SAL_RST_TYPE_STA_OR_RELAY    3 // �ն�(��ͨվ��)
#define SAL_RST_TYPE_INVLAID    0xdeaddead
typedef struct
{
    HI_U32 enRebootCondition;// ��������
    HI_U32 ulKickDogTimes;   // ι������
    HI_U32 ulUnormal;        // ��¼�쳣
    HI_U32 ulRole;           // ʲô��ɫ 0xdeaddeadΪ��ɫ�쳣
    HI_U8  rsv[44];
} HI_SYS_RST_COMPONENT_VOTE_S;

typedef HI_SYS_RST_COMPONENT_VOTE_E(*HI_SYS_RST_COMPONENT_VOTE_F) (HI_OUT HI_SYS_RST_COMPONENT_VOTE_S * pstCondition);

typedef HI_U32(*HI_SYS_RST_NOTIFY_F) (HI_VOID);
typedef HI_BOOL(*HI_SYS_RST_TIMER_NOTIFY_F) (HI_VOID);


typedef enum
{
    HI_SYS_PLC_CHL_STATUS_UNKNOWN = 0     ,
    HI_SYS_PLC_CHL_STATUS_FC_CRC_SUCCESS  ,
    HI_SYS_PLC_CHL_STATUS_MAX = 0
}HI_SYS_PLC_CHL_STATUS_E;


typedef enum
{
    HI_SYS_REBOOT_POLICY_UNKNOWN =0xFF,
    HI_SYS_REBOOT_POLICY_DELAY5  =5,
    HI_SYS_REBOOT_POLICY_DELAY0  =0
}HI_SYS_REBOOT_POLICY_E;

#define HI_DRV_VERSION_UPDATE_FINISH_OPTION_APP   0
#define HI_DRV_VERSION_UPDATE_FINISH_OPTION_DRV   0xFFFF
//****************************************************************************


//****************************************************************************
// ����, ���������ڲ�����ʹ��
//
#if defined(HI_SYS_DEBUG) || defined(PRODUCT_CFG_SUPPORT_ASSERT)
#define HI_ASSERT(expr)\
    {\
        if (!(expr)) { HI_SYS_FatalVerifyPrv((HI_PCSTR)#expr, (HI_U32)0, (HI_U32)HI_SYS_GET_LR(), (HI_PCSTR)0, (HI_U32)__LINE__, (HI_PCSTR)__FUNCTION__); } \
    }
#else
#define HI_ASSERT(expr)  ((HI_VOID)0)
#endif
//****************************************************************************


//****************************************************************************
typedef enum
{
    HI_SYS_RST_RESUME_DEFAULT = 0,
    HI_SYS_RST_RESUME_HW_IO,
    HI_SYS_RST_RESUME_INIT,
    HI_SYS_RST_RESUME_WDM,
    HI_SYS_RST_RESUME_DIAG,
    HI_SYS_RST_RESUME_HISR_RX,
    HI_SYS_RST_RESUME_TX,
    HI_SYS_RST_RESUME_DMS,
    HI_SYS_RST_RESUME_RSTM, // rest monitor manager
    HI_SYS_RST_RESUME_NV,
    HI_SYS_RST_RESUME_TASK,
    HI_SYS_RST_RESUME_CAUSE_MAX
}HI_SYS_RST_RESUME_CAUSE_E;
//****************************************************************************


//****************************************************************************
// Command ID for HI_SYS_SocketSetOpt
typedef enum
{
    HI_SYS_SOCKET_OPT_ID_NO_DELAY = 0,
    HI_SYS_SOCKET_OPT_BLOCK         ,
    HI_SYS_SOCKET_OPT_NO_BLOCK      ,
    HI_SYS_SOCKET_OPT_SO_BROADCAST  ,
    HI_SYS_SOCKET_OPT_SO_LINGER     ,
    HI_SYS_SOCKET_OPT_SO_REUSEADDR  ,
    HI_SYS_SOCKET_OPT_SO_RCVBUF     ,
    HI_SYS_SOCKET_OPT_MAX
} HI_SYS_SOCKET_OPT_ID_E;
//****************************************************************************


//****************************************************************************
typedef enum
{
    HI_SYS_INIT_STATUS_OK       = 0,
    HI_SYS_INIT_STATUS_NV_FAIL  = 0x01,
    HI_SYS_INIT_STATUS_MEM_LOW  = 0x02,  // �ڴ治��, ����DIAGģʽ
    HI_SYS_INIT_STATUS_MOD_FAIL = 0x04,
    HI_SYS_INIT_STATUS_MAX
} HI_SYS_INIT_STATUS_E;
//****************************************************************************


//****************************************************************************
//MAC֡������Ϣ
typedef struct
{
    HI_U8 ucSrcMac[HI_PLC_MAC_ADDR_LEN];
    HI_U8 ucDestMac[HI_PLC_MAC_ADDR_LEN];
    HI_BOOL is_ee_iv;           // �Ƿ����
    HI_U8 max_resend_count;     // �����ش���������
    HI_U8 ucSnid;
    HI_U8 ucPhase;              // ���ñ��ķ�����λ

    HI_U8   ndm_special_ver;    // �Ƿ�Ϊ��������������,������չ Ϊ�����������
    HI_BOOL is_broadcast;       // ��MAC��ַΪȫ0xFFʱ��HI_TRUEΪȫ���㲥��HI_FALSEΪ����㲥
    HI_U16  pad1;
} DMS_MAC_FRAME_CTRL_S;
//****************************************************************************

/**
 * @ingroup hct_ETH
 * ��̫������Ϣ
 */
typedef struct
{
    HI_U8  ip_addr[HI_IPV4_ADDR_LEN];             /**< IP��ַ */
    HI_U8  net_mask[HI_IPV4_ADDR_LEN];          /**< ��������*/
    HI_U8  gateway[HI_IPV4_ADDR_LEN];           /**< Ĭ������*/
    HI_U8  mac_addr[HI_PLC_MAC_ADDR_LEN];	  /**< mac��ַ*/
    HI_U8  reserved[2];                                    /**< �����ֽ�*/
} HI_SYS_ETH_CONFIG_S;
//****************************************************************************


//****************************************************************************
#define _SAL_1M_NUM       (1000*1000)

#define _SAL_CPU_DELAY(cnt) \
{\
    HI_U32 __i = 0;\
    for (; __i < (cnt); __i++) { HI_NOP }\
}
#define HI_SYS_1M_NUM           _SAL_1M_NUM
#define HI_SYS_CPU_DELAY(xm)   _SAL_CPU_DELAY((xm)*HI_SYS_1M_NUM)

#if defined(PRODUCT_CFG_PLATFORM_HI3911)
#define HI_SYS_NEVER_RETURN()\
    for (;;)\
    {\
        _SAL_CPU_DELAY(50*_SAL_1M_NUM);\
    }
#else
#define HI_SYS_NEVER_RETURN()
#endif
//****************************************************************************


//****************************************************************************
#define __INC_BOUND_U32(x)                               if(((HI_U32)(x)) == (HI_U32)0xFFFFFFFF) (x) = 0
#define __INC_BOUND_U16(x)                               if(((HI_U16)(x)) == (HI_U16)0xFFFF) (x) = 0
#define HI_SAL_INC_U32(x)                                do { __INC_BOUND_U32(x); (x)++; } while(0)
#define HI_SAL_INC_U16(x)                                do { __INC_BOUND_U16(x); (x)++; } while(0)

#define HI_SYS_BCDHEX2DEC(b)            (((b&0xF0)>>4)*10+(b&0x0F))
#define HI_SYS_DEC2BCDHEX(b)            ((((b)/10)*0x10) + ((b)%10))

//****************************************************************************


//****************************************************************************
#if defined(HAVE_PCLINT_CHECK)
#define HI_CHECK_DEFAULT_ID(id)           (id) // Ϊ�˷���ʹ�ã�����ĺ궨�岻��PCLINT���
#else
#define HI_CHECK_DEFAULT_ID(id)           (((id)==0)?__LINE__:(id))
#endif
#define HI_DIAG_LOG_MSG_MK_ID_E(id)       HI_MAKEU32(((((HI_U16)(HI_CHECK_DEFAULT_ID(id)))<<4)+0), HI_DIAG_LOG_MSG_FILE_ID)
#define HI_DIAG_LOG_MSG_MK_ID_W(id)       HI_MAKEU32(((((HI_U16)(HI_CHECK_DEFAULT_ID(id)))<<4)+1), HI_DIAG_LOG_MSG_FILE_ID)
#define HI_DIAG_LOG_MSG_MK_ID_I(id)       HI_MAKEU32(((((HI_U16)(HI_CHECK_DEFAULT_ID(id)))<<4)+2), HI_DIAG_LOG_MSG_FILE_ID)
#define HI_DIAG_LOG_MSG_MK_MOD_ID(id)     HI_MAKEU32(HI_DIAG_LOG_MSG_MOD_ID, HI_CHECK_DEFAULT_ID(id))
//****************************************************************************


//****************************************************************************
#define HI_DIAG_SYS_INIT_RESET      0
#define HI_DIAG_SYS_INIT_SET        1

#define HI_NV_CFG_DISABLE_U16_SWT   0xFFFE
#define HI_NV_CFG_DISABLE_U32_SWT   0xFFFFFFFE
//****************************************************************************

//*****************************************************************************
// ϵͳ���������
//*****************************************************************************

#define HI_DSID_TYPE_QRY           0x00
#define HI_DSID_TYPE_PERIOD        0x01
#define HI_DSID_TYPE_CUSTOM        0x02

#define HI_DSID_MAX_NUM    0xFFFF

/**
 * @ingroup hct_diag
 * ͳ�����б�ṹ
 */
typedef struct
{
    HI_U16 usId;                /**< ͳ��������ID��ID��ΧΪ[0xEE00, 0xF000)�� */
    HI_U8 ucOption;             /**< ͳ�����ȼ�����дĬ��ֵHI_SYS_DMID_BUSINESS_L2 */
    HI_U8 ucType;               /**< ͳ�������ͣ�HI_DSID_TYPE_QRY��ʾ��ѯ�ϱ���Ĭ��֧�֣���HI_DSID_TYPE_PERIOD��ʾ�����������ϱ�������Ĭ��5s����*/
    HI_U16 usArrayCnt;          /**< ��ͳ����ΪaʱȡֵΪ1����ͳ����Ϊ����a[n]ʱȡֵΪn*/
    HI_U16 uStatPacketSize;     /**< ��ͳ����ΪaʱȡֵΪsizeof(a)����ͳ����Ϊ����a[n]ʱȡֵΪsizeof(a[0])*/
    HI_PVOID pStatPacket;       /**< ͳ������ַ������Ϊȫ�ֱ���*/
} HI_DIAG_SYS_STAT_OBJ_S;

#define HI_NDM_645_FRAME                    0x3500  // �շ�645֡ ��Ӧ�ṹΪ HI_NDM_645_FRAME_S
#define HI_STA_TRANSMIT                     0x3501  // ת��STA���ݸ����
#define HI_CLTII_R485_TRANSMIT_DATA         0x3502

// HI_NDM_645_FRAME �����Ӧ�ṹ
typedef struct
{
    HI_U16 usFrameSize;     // ֡��С, ��aucFrameSize��ʵ�ʳ���, ��λ�ֽ�
    HI_U16 reserved;        // Ԥ���ֶ�
    HI_U8 aucFrame[256];
} HI_NDM_645_FRAME_S;
//****************************************************************************


//****************************************************************************
// NDM ��֡��ʽ����
//****************************************************************************
typedef struct
{
    HI_U32 ulSn;
    HI_U16 usTransId;      // �����ʶ
    HI_U16 usControl  :15; // ������
    HI_U16 usDULink   :1;  // �����б�ʶ

    HI_U16 usUserData;     // �û��Զ�������, ��ǰ�汾��ʾ HSO��ID
    HI_U8 ucProdType;
    HI_U8 pad;             //����

    HI_U8 aucMacAddr[HI_PLC_RELAY_NUM_MAX][HI_PLC_MAC_ADDR_LEN]; // Ŀ���ַ
    HI_U8 aucNdmMacAddr[HI_PLC_MAC_ADDR_LEN];                    //NDM����MAC������Ϊ0
} HI_ND_REMOTE_FRAME_HEAD_S;
//****************************************************************************

//****************************************************************************
// ϵͳ״̬����
//****************************************************************************/
#define HI_ND_NET_STATUS_FORM_FAIL       0
#define HI_ND_NET_STATUS_FORM_END        1
#define HI_ND_NET_STATUS_FORMING         2
#define HI_ND_NET_STATUS_FORM_CHANGED    3
#define HI_ND_NET_STATUS_FORM_UNKNOWN    0xFF

#define HI_ND_SYS_ERR_STATUS_UNKNOWN      0xFF
#define HI_ND_SYS_ERR_STATUS_OK           0x0
#define HI_ND_SYS_ERR_STATUS_CRASH_FOUND  0x1

#define HI_ND_SYS_BOOT_CAUSE_NORMAL         0x0  // ��������
#define HI_ND_SYS_BOOT_CAUSE_EXP            0x1  // �쳣����
#define HI_ND_SYS_BOOT_CAUSE_WD             0x2  // ���Ź�����
#define HI_ND_SYS_BOOT_CAUSE_UPG_VERIFY     0x3  // ������֤����
#define HI_ND_SYS_BOOT_CAUSE_UPG_FAIL       0x4  // ����ʧ������
#define HI_ND_SYS_BOOT_CAUSE_UPG_BACK_FAIL  0x5  // ��������ʧ������

#define HI_ND_SYS_UPG_CAUSE_NORMAL          0x01  // ��ʾ�������쳣
#define HI_ND_SYS_UPG_CAUSE_FAIL            0x02  // ��ʾ��������

typedef struct
{
    HI_U16 usMode;
    HI_U8  ucNetStatus;
    HI_U8  ucSysErrStatus;
    HI_U8  ucBootCause; // HI_ND_SYS_BOOT_CAUSE_XXX
    HI_U8  ucUpgCause;
    HI_U16 usDevId16;
    HI_U16 ausVer[2];
    HI_U8  aucVer[3];
    HI_U8  aucBuildDataTime[6];
	HI_U8  ucNvCVer; // NV.hupg: C Ver
    HI_U16 ausNvVer[2]; // NV.hupg: B/SPC Ver
    HI_U16 usChipVer;
    HI_U8  ucBootVerIdx;
    HI_U8  ucTestMode;
    HI_U16 usTimeStampMin;
} HI_SYS_STATUS_IND_S;

typedef struct
{
    HI_U16 ausVer[2];           // B,SPC�汾 {012,1}��ӦB012SPC001
    HI_U8  aucVer[3];           // VRC�汾{100,1,2}��ӦV100R001C002
    HI_U8  aucBuildDataTime[6]; // �����ļ����������ں�ʱ��{12,12,31,15,59,47} ��Ӧ2012-12-31 15:59:47
    HI_U8  ucBootVer;           // BOOT�汾35��Ӧx.x.35
    HI_U8  ucSysMode;           // ��ǰϵͳģʽ  HI_ND_SYS_STATUS_XXX
    HI_U8  ucTestMode;          // ��ǰϵͳ������ģʽ
    HI_U16 usSwVer;             // �ͻ�����汾��,2�ֽ�BCD
    HI_U16 usYear  : 7;         // �ͻ��汾:��
    HI_U16 usMonth : 4;         // �ͻ��汾:��
    HI_U16 usDay   : 5;         // �ͻ��汾:��
    HI_CHAR szManuCode[2];      // �ͻ��汾:���̴���ASCII
    HI_CHAR szChipCode[2];      // �ͻ��汾:оƬ����ASCII
    HI_U16 usChipVer;           // оƬ�汾
    HI_U16 reserved;
} HI_MDM_SYS_INFO_S;

//****************************************************************************


//****************************************************************************
#define ID_DIAG_CMD_SEND_FRAME  (0x5721)
//****************************************************************************

//****************************************************************************
typedef enum
{
    HI_CHL_MSG_MIN = HI_DMS_CHL_MSG_MIN,
    HI_CHL_MSG_SYNC_WHITE_LIST,		// ͬ����������ϢID
    HI_CHL_MSG_ASSOC_SYS_INFO,      // MAC-->UPG
    HI_CHL_MSG_BOARDCAST_CFG,        // UPG-->MAC
    HI_CHL_MSG_TF_START_SEARCH_METER,    //̨��ʶ�������ѱ�,MAC-APP
    HI_CHL_MSG_TF_STOP_SEARCH_METER,     //̨��ʶ��ֹͣ�ѱ�,MAC-APP

    HI_CHL_MSG_MAX = HI_DMS_CHL_MSG_MAX
}HI_CHL_MSG_E;

#define HI_ND_TYPE_DCM      1 // ���Կ���ģ��(������)
#define HI_ND_TYPE_CCO      2 // ͷ��
#define HI_ND_TYPE_STA      3 // �ն�(��ͨվ��)
#define HI_ND_TYPE_RELAY    4 // �м�
#define HI_ND_TYPE_CLTII    5 // II��
#define HI_ND_TYPE_CLTI     6 // I��
#define HI_ND_TYPE_THREE_STA 7 // �����ģ��

#define HI_ND_DEV_TYPE_METER       0x10 // ���
#define HI_ND_DEV_TYPE_METER_COL   0x11 // �ɼ���
#define HI_ND_DEV_TYPE_CCO         0x12 // CCO
#define HI_ND_DEV_TYPE_UNKNOWN     0xFF // �޷�ʶ����豸
//****************************************************************************

//****************************************************************************
// HI_DIAG_SetupLoadFileNoitfyProc Option.
#define HI_DIAG_LD_FILE_OPT_NA     0x00 // δָ����ѡ��
#define HI_DIAG_LD_FILE_OPT_START  0x01 // HI_LOU16(ulOption)
#define HI_DIAG_LD_FILE_OPT_END    0x02 // HI_LOU16(ulOption)
#define HI_DIAG_LD_FILE_OPT_BLK    0x03 // HI_LOU16(ulOption)
//****************************************************************************

//****************************************************************************
// HI_DIAG_SetupLoadFileNoitfyProc Option.
#define HI_DIAG_LD_FILE_OPT_TIMEOUT  0xFF
#define HI_LOAD_HUPG_FILE_DRV_PROC_ID    0
#define HI_LOAD_HUPG_FILE_SAL_PROC_ID    1
#define HI_LOAD_FILE_BQ_PROC_ID          2
#define HI_LOAD_HUPG_FILE_MDM_PROC_ID    3
#define HI_LOAD_FILE_PROC_ID_MAX         0xFFFE
#define HI_LOAD_NV_FILE_PROC_ID          0xFFFE // the Process ID to burn NV File to UE
#define HI_LOAD_INVALID_FILE_PROC_ID     0xFFFF
//****************************************************************************

//****************************************************************************
// HI_DIAG_SetupLoadFileNoitfyProc BLOCK SIZE
#define HI_MDM_LD_FILE_BLOCK_SIZE        (1024)
//****************************************************************************
typedef enum
{
    HI_CFG_PERFORMANCE_HIGH,//�������豸
    HI_CFG_PERFORMANCE_LOW,//�������豸
    HI_CFG_PERFORMANCE_MAX,//ö�����ֵ
}HI_CFG_PERFORMANCE_E;//���������豸����ѡ��ֵ

#define CHANLLENGE_COMMON_SIZE 32
#define CHANLLENGE_SALT_SIZE 16
/******************************DIAG������־ ST***********************************************/
/**
 * @ingroup hct_diag
 * diag��־����Ϊ��������
 */
#define HI_DIAG_LOG_OPT_LOCAL_CMD  1

/**
 * @ingroup hct_diag
 * diag��־����Ϊ����Ӧ��
 */
#define HI_DIAG_LOG_OPT_LOCAL_IND  2

/**
 * @ingroup hct_diag
 * diag��־����Ϊ����ACK
 */
#define HI_DIAG_LOG_OPT_LOCAL_ACK  3

/**
 * @ingroup hct_diag
 * diag��־����ΪԶ������
 */
#define HI_DIAG_LOG_OPT_REMOTE_CMD 4

/**
 * @ingroup hct_diag
 * diag��־����ΪCCO ���յ���STA ����ظ�
 */
#define HI_DIAG_LOG_OPT_DREMOTE_CMD 5

/**
 * @ingroup hct_diag
 * DIAG����֪ͨ������Ϣ
 */
typedef struct
{
    HI_U32 opt;/**<�������ͣ�ȡֵΪHI_DIAG_LOG_OPT_XXX*/
    HI_U8 plcMac[6];/**< ��������ΪԶ������ʱ��ȡֵΪĿ�굥��MAC��ַ������ȡֵΪ0*/
    HI_U8 reserver[2];
}HI_DIAG_LOG_CTRL_S;


/**
* @ingroup  hct_diag
* @brief DIAG����֪ͨ������
*
* @par ����:
* ����DIAG���ݽ���ʱ��ͨ���ú�����������Ϣ���ݸ�APP��
* @attention
* @li �û��ص������в��ɽ��������Բ��� ������ֱ�ӷ��ͳ�ʱʱ��Ϊ0����Ϣ������������д����������Ӱ��DIAG�������ܣ�������������DIAGͨ����
* @li �û��ص������в��ɶԽ������ݽ����޸ģ���������DIAG�������ݴ���
* @li HI_DIAG_LOG_CTRL_S�ṹ����������չ���û���ע��ṹ��չ������ɵļ��������⡣
*
* @param  ulId[IN] ����ID��
* @param  pUsrData[IN] ��������ָ�롣
* @param  usUsrDataSize[IN] �������ݴ�С��
* @param  pCtrl[IN] ����������Ϣ��
*
* @par Dependency:
* @li hi_mdm_diag.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
typedef HI_VOID (*HI_DIAG_CMD_NOTIFY_F)(HI_U16 ulId,HI_PVOID pUsrData,HI_U16 usUsrDataSize,HI_DIAG_LOG_CTRL_S * pCtrl);

#define DIAG_CMD_HOST_CONNECT_FOR_LOG                             (0x5000)//DIAGͨ������
#define DIAG_CMD_HOST_DISCONNECT_FOR_LOG                          (0x5001)//DIAGͨ���Ͽ�����
#define DIAG_CMD_LOG_IN_PWD_CHECK_RESULT_FOR_LOG                  (0x5057)//��������У����

#define HI_LOGIN_AUTH_RESULT_ID_ACCOUNT_LOCKED_FOR_LOG 1//�˺�����
#define HI_LOGIN_AUTH_RESULT_ID_PASS_WORD_WRONG_FOR_LOG 47//�������
#define HI_LOGIN_AUTH_RESULT_ID_SYSTEM_CALL_ERROR_FOR_LOG 2999//��������
typedef struct
{
    HI_U32 errId;// HI_LOGIN_AUTH_RESULT_ID_XXX
    //��errIdΪHI_LOGIN_AUTH_RESULT_ID_PASS_WORD_WRONG_FOR_LOGʱdata��ʾ���м��λ����˺ű�����(0xFFFFFFFF��ʾ��֧������)����errIdΪHI_LOGIN_AUTH_RESULT_ID_ACCOUNT_LOCKED_FOR_LOGʱdata��ʾ�˺�ʣ������ʱ��(��λs)���������������
    HI_U32 data;
}HI_DIAG_LOG_IN_PWD_CHECK_RESULT_FOR_LOG_S;
/******************************DIAG������־ END***********************************************/


/******************************������Ϣ����� ST***********************************************/
#define ID_DIAG_CMD_BIDB_EXPORT      0x3403  // ����: HI_BIDB_EXPORT_REQ_S,,HI_BIDB_EXPORT_IND_S
#define ID_DIAG_CMD_BIDB_IMPORT      0x3405  // ����: HI_BIDB_IMPORT_REQ_S,,HI_BIDB_IMPORT_IND_S

#define HI_BIDB_ITEM_NUM_MAX         1024
#define HI_BIDB_ITEM_NUM             10

#define HI_BIDM_IMPORT_CTRL_ADD      0 // ����������Ѿ����ڣ��򸲸�
#define HI_BIDM_IMPORT_CTRL_UPDATE   1 // �滻������Ѿ����ڣ��򲻸���
#define HI_BIDM_IMPORT_CTRL_REMOVE   2 // �滻


// ������Ϣ���ݿ������Ϣ���
#define HI_MODULE_POSITION_LEN_MAX       64
typedef struct
{
    HI_U8 ucMacAddr[HI_PLC_MAC_ADDR_LEN];           // ͨ��ģ���MAC��ַ
    HI_U8 ucConnectedDevAddr[HI_PLC_MAC_ADDR_LEN];  // ģ�����ӵ��豸�������ĵ�ַ
    HI_CHAR szPositoin[HI_MODULE_POSITION_LEN_MAX]; // ͨ��ģ��ĵ���λ��
} HI_BIDB_ITEM_S;

typedef struct
{
    HI_U16 usSn;//���ε����Ļ�����Ϣ����Ŀ��ʼID
    HI_U8  ucItems;//����ʵ�ʵ�����Ŀ��
    HI_U8  bFinish;//�������
    HI_U32 ulRc;// ����� : HI_ERR_SUCCESS��ʾ�ɹ�; HI_ERR_INVALID_PARAMETER:��ʾ�������;  HI_ERR_NO_MORE_DATA:��ʾ������
    HI_BIDB_ITEM_S astItem[HI_BIDB_ITEM_NUM];
} HI_BIDB_EXPORT_IND_S;

typedef struct
{
    HI_U16 usSn;//���ε����Ļ�����Ϣ����Ŀ��ʼID,0��ʾ���뿪ʼ��
    HI_U16 usItems;//���ε����Ļ�����Ϣ����Ŀ����
    HI_U32 ulRc;
} HI_BIDB_EXPORT_REQ_S;

typedef struct
{
    HI_U16 usSn;//���ε���Ļ�����Ϣ����Ŀ��ʼID��0��ʾ���뿪ʼ.
    HI_U8  ucItems;//���ε���Ļ�����Ϣ����Ŀ����,usSn==0��ucItems==0xFFFFʱ��ʾ���
    HI_U8  bFinish;//�Ƿ������һ�ε���
    HI_U32 ulRc;//������
    HI_BIDB_ITEM_S astItem[HI_BIDB_ITEM_NUM];//������Ϣ������
}HI_BIDB_IMPORT_REQ_S;

typedef struct
{
    HI_U16 usSn;//��REQһ��
    HI_U16 usItems;//ʵ�ʵ�����Ŀ
    HI_U32 ulRc;// �����
} HI_BIDB_IMPORT_IND_S;
/******************************������Ϣ����� END***********************************************/

/************* �㲥/�������� NV��, ʹ��˵���ο�  << Hi3911V100 ����������ӿ����˵����.docx >> ST***************/
#define ID_DIAG_CMD_UPG_START 0x3412  // ����: HI_UPG_START_REQ_S, , HI_UPG_START_IND_S
#define ID_DIAG_CMD_UPG_NV_W  0x3510       // ����: HI_UPG_NV_W_REQ_S, ,

typedef struct
{
    HI_U16 usNvId;//NV ID
    HI_U16 usCrc;//
    HI_U8  ucMac[6];//Ŀ��PLC MAC��ַ
    HI_U8  ucTimes;//�㲥����
    HI_U8  reserved[7];
    HI_U16 usPayloadLen; // payload ��Ч����
    HI_U8  ucPayload[0]; // payload���洢�ռ�
} HI_UPG_NV_BW_REQ_S;

typedef struct
{
    HI_U32 ulRc;     // �����, HI_ERR_SUCCESS��ʾ�ɹ�, ����Ϊ������
    HI_U16 usNvId;
    HI_U8  ucMac[6];
    HI_U8  reserved[4];
} HI_UPG_NV_BW_IND_S;

typedef struct
{
    HI_U16 usNvId;
    HI_U8  ucMac[6];
    HI_U8  ucTimes;      //�㲥����
    HI_U8  reserved[7];
} HI_UPG_NV_BR_REQ_S;

typedef struct
{
    HI_U32 ulRc;     // �����, HI_ERR_SUCCESS��ʾ�ɹ�, ����Ϊ������
    HI_U16 usCrc;
    HI_U16 usNvId;
    HI_U8  ucMac[6];
    HI_U8  reserved[4];
    HI_U16 usPayloadLen; // payload ��Ч����
    HI_U8  ucPayload[0]; // payload���洢�ռ�
} HI_UPG_NV_BR_IND_S;
/************* �㲥/�������� NV��, ʹ��˵���ο�  << Hi3911V100 ����������ӿ����˵����.docx >> END***************/

/*************�����������***************/

#define HI_485_BURST_BEGIN (1)
#define HI_485_BURST_END (0)

#define HI_UART_BURST_BEGIN (1)
#define HI_UART_BURST_END (0)
typedef enum
{
    HI_EN485_RECE = 0,
    HI_EN485_SEND,
}HI_EN485_TYPE_E;

typedef enum
{
    HI_UARTMODE_UART=0,
    HI_UARTMODE_IR,
    HI_UARTMODE_485,
    HI_UARTMODE_MAX,
}HI_UART_MODE_TYPE_E;

typedef struct
{
    TIMER_HANDLE_T  pRxTimerPeriod;     //��ʱ�����
    HI_U32          latestBaud;         //ǰһ�β����ʴ�С
    HI_U32          byteSpace;          //��֡���ļ����Сʱ��
    HI_U32          baudAdapteFlag;     //����������Ӧ��⿪����־
    HI_U32          uartWriteBurst;     //д״̬��־λ
    HI_BOOL         isEnbale;           //����������Ӧ���ܿ�����־
    HI_U8           reserved[3];
} HI_UART_ADAPT_TYPE_S;

typedef enum
{
    HI_IR0,  //��Ӧ UART3
    HI_IR1,  //��Ӧ UART2
    HI_IR_MAX,
}HI_IR_ID_TYPE_E;

/**
 * @ingroup hct_diag
 * ��׼Э��
 */
#define HI_MDM_PLC_PROTOCOL_STANDARD                1

/**
 * @ingroup hct_diag
 * �Ǳ�׼Э��
 */
#define HI_MDM_PLC_PROTOCOL_NON_STANDARD            0

//****************************************************************************
HI_END_HEADER
#endif

