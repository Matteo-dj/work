/******************************************************************************
*             Copyright (C) 2018 - 2018, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName: hi_mdm_types.h
* Description：SAL(系统功能应用层)数据类型定义
*
* History:
* Version              Date         DefectNum           Description
* V200R001C00SPC010    2018-03-31   NA                  第一次版本发布。
* V200R001C00SPC012    2018-07-17   DTS2018061507315	修改了HI_SYS_STATUS_INFO_S结构中成员ucSysMode的描述，
                                    DTS2018071400465    增加了PLC标准协议和非标准协议的宏定义、HI_MAC_NETWORK_TOPO_ENTRY_S中增加参数mac_type。
* V200R001C00SPC050B013    2019-01-09   DTS2019010900227    增加三相表模块相线2，相线3的相位识别结果。
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
// 字节对齐
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
// 公用数据类型定义
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
    HI_U32 ulMajorMinorVersion; // 主版本号.次版本号
    HI_U32 ulRevisionVersion;   // 修正版本号
    HI_U32 ulBuildVersion;      // 内部版本号
} HI_UE_SOFT_VERSION_S;


#define HI_BUILD_VER_DATE_LEN                (10)
#define HI_BUILD_VER_TIME_LEN                (8)
#define HI_BUILD_VER_PRODUCT_NAME_LEN_MAX    (28)
#define HI_BUILD_VER_PRODUCT_LEN_MAX         (HI_BUILD_VER_PRODUCT_NAME_LEN_MAX+HI_BUILD_VER_DATE_LEN+HI_BUILD_VER_TIME_LEN+6)

typedef struct
{
    HI_U16 usVVerNo;                         // V部分
    HI_U16 usRVerNo;                         // R部分
    HI_U16 usCVerNo;                         // C部分
    HI_U16 usBVerNo;                         // B部分
    HI_U16 usSpcNo;                          // SPC部分
    HI_U16 usCustomVer;                      // 客户定制编码
    HI_U32 ulProductNo;                      // 产品类型编号，即不同外设组合的硬件平台
    HI_CHAR acBuildDate[HI_BUILD_VER_DATE_LEN]; // build日期, 格式为Mon dd yyyy
    HI_CHAR acBuildTime[HI_BUILD_VER_TIME_LEN]; // build时间, 格式为hh:mm:ss
} HI_UE_BUILD_VER_INFO_S; // 内部版本

typedef struct
{
    HI_U16 usVVerNo;           // V部分
    HI_U16 usRVerNo;           // R部分
    HI_U16 usCVerNo;           // C部分
    HI_U16 usBVerNo;           // B部分
    HI_U16 usSpcNo;            // SPC部分
    HI_U16 usHardwareVerNo;    // 硬件PCB号和印制板版本号
    HI_U32 ulProductNo;        // 产品类型编号，即不同外设组合的硬件平台
} HI_UE_PRODUCT_VER_S;

typedef struct
{
    HI_CHAR* pszVer;         // "V100R001C00B00"
    HI_CHAR* pszProductVer;  // "Hi3911 V100R001C00B00"
    HI_CHAR* pszFileVer;     // Modem Version: like "1.999.111"
    HI_CHAR* pszChipName;    // "Hi3911"
    HI_CHAR* pszDate;        // 如 2017-08-01
    HI_CHAR* pszTime;        // 如 14:30:26
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

#define HI_SYS_DISABLE_INTERRUPTS    1 // 所有中断关闭
#define HI_SYS_ENABLE_INTERRUPTS     2 // 所有中断使能

#define HI_DIAG_CMD_ID(x)            ID_DIAG_CMD_ ## x

#define HI_DIAG_SYS_STAT_OBJ_CNT     2048

typedef enum
{
    EN_SYS_DUMMY_TYPEO_MEM
} HI_SYS_DUMMY_TYPE;

typedef enum
{
    EN_SAL_ERROR_TYPE_INIT_INVALID_CHIP_TYPE,
    EN_SAL_ERROR_TYPE_INIT_NO_MEM,  // 程序初始化过程中，系统提供的内存不足，程序无法继续运行(不能修复的错误)
    EN_SAL_ERROR_TYPE_OS_RES_LIMIT, // 程序初始化过程中，OS供的资源不足，程序无法继续运行(不能修复的错误)
    EN_SAL_ERROR_TYPE_STOP,         // 程序运行过程中，程序无法继续运行(不能修复的错误)
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
 * UART 配置
 */
typedef struct
{
    HI_U32  baud_rate;  /**<波特率*/
    HI_U32  data_bits;  /**<数据位宽*/
    HI_U32  stop_bits;  /**<停止位 0:0个停止位 1:1.5或2个停止位*/
    HI_U32  parity;     /**<奇偶校验 0:无校验 1:奇校验 2:偶校验*/
} HI_UART_INIT_TYPE_S;

#define HI_SYS_UART_CFG_DEFAULT_VAL  {(HI_SYS_UART_BAUD_RATE_E)HI_DMS_DIAG_UART_DEFAULT_BAUDRATE,8, 1, 0}

// 数据位
#define HI_UART_DATA5BIT             (5)
#define HI_UART_DATA6BIT             (6)
#define HI_UART_DATA7BIT             (7)
#define HI_UART_DATA8BIT             (8)

// 停止位
#define HI_UART_STOP1                (1)
#define HI_UART_STOP2                (2)

// 校验
#define HI_UART_NOCHK                (0)
#define HI_UART_ODD                  (1)
#define HI_UART_EVEN                 (2)

/**
 * @ingroup hct_time
 * 墙上时间
 */
typedef struct
{
    HI_U16 year;    /**< 年*/
    HI_U8 month;    /**< 月*/
    HI_U8 day;      /**< 日*/
    HI_U8 hour;     /**< 时*/
    HI_U8 min;      /**< 分*/
    HI_U8 sec;      /**< 秒*/
    HI_U8 reserved;
}HI_SYS_CALENDAR_TIME_S;

typedef struct
{
    HI_PCSTR pszTaskName;
    HI_PVOID pStackAddr;
    HI_U32 ulStackSize;
    HI_U32 ucPriority; // 为了对齐, 原来的8bit改为32bit
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
* @brief NV项变更通知回调函数。
*
* @par 描述:
* NV项变更通知回调函数。
* @attention
* @li 回调函数中尽量避免修改NV造成死循环。
*
* @param  ulId [IN] 被修改的NV ID。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_nv.h: 该接口声明所在的头文件。
* @see 无。
* @since HiMDL_V200R001C00
*/
typedef HI_U32(*HI_NVM_CHANGED_NOTIFY_CALLBACK_F) (HI_U16 ulId);

#define HI_NVM_CHANGED_NOTIFY_CALLBACK_PFN HI_NVM_CHANGED_NOTIFY_CALLBACK_F

typedef HI_U32(*HI_LD_FILE_CALLBACK_F) (HI_U32 ulOption, HI_U32 ulId, HI_PVOID pFile, HI_U32 ulFileSize, HI_U32 ulErrCode);

/**
* @ingroup  hct_diag
* @brief  DIAG连接状态通知函数。
*
* @par 描述:
* 当DIAG通道连接状态发生变化时,通过该函数通知APP。
* @attention 无。
*
* @param  usPortNum [IN] 通道号。
* @param  bConnect [IN] 连接状态，HI_TRUE:DIAG连接 HI_FALSE:DIAG断开连接。
*
* @retval 无要求 建议用户默认返回HI_ERR_SUCCESS。
*
* @par Dependency:
* @li hi_mdm_diag.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
typedef HI_U32(*HI_DIAG_CONNECT_F) (HI_U16 usPortNum, HI_BOOL bConnect);

/**
 * @ingroup hct_dms
 * 通道模式
 */
typedef enum
{
    HI_DMS_LCHL_MODE_LOCAL = 0, /**< 通道数据来源为本地 */
    HI_DMS_LCHL_MODE_PLC,       /**< 通道数据来源为PLC */
    HI_DMS_LCHL_MODE_MAX
} HI_DMS_LCHL_MODE_E;

/**
 * @ingroup hct_dms
 * 通道数据接收的控制结构
 */
typedef struct
{
    HI_U16 usId;                            /**< 报文ID，仅当通道模式为HI_DMS_LCHL_MODE_PLC时有效 */
    HI_U8 ucMode;                           /**< 通道模式，取值见HI_DMS_LCHL_MODE_E */
    HI_U8 ucCtrl;                           /**< 报文选项字，仅当通道模式为HI_DMS_LCHL_MODE_PLC时有效 */
    HI_U8 bCanDiscardOldData    : 1;        /**< 丢弃旧数据标识，HI_TURE为丢弃，HI_FALSE为不丢弃*/
    HI_U8 bDataTimeout          : 1;        /**< 暂未使用 */
    HI_U8 reserved1 : 6;                    /**< 保留位 */
    HI_U8 reserved2;                        /**< 保留位 */
    HI_U8 ucDestMac[HI_PLC_MAC_ADDR_LEN];   /**< 目的MAC地址 */
    HI_U8 ucSrcMac[HI_PLC_MAC_ADDR_LEN];    /**< 源MAC地址 */
    HI_U32 bIsMyNetFrame    : 1;            /**< 是否本网络报文 */
    HI_U32 ucRcvFreq        : 7;            /**< 接收频段号 */
    HI_U32 ulRcvNid         : 24;           /**< 接收网路号 */
    HI_U16 usPayloadLen;                    /**< 接收数据长度 */
    HI_PBYTE pPayload;                      /**< 接收数据指针 */
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
 * 通道断开
 */
#define HI_DMS_CHL_DISCONNECT      0    /**< 通道断开 */

/**
 * @ingroup hct_dms
 * 通道连接
 */
#define HI_DMS_CHL_CONNECT         1    /**< 通道连接 */

#define HI_DMS_CHL_MSG_MIN         0x1000
#define HI_DMS_CHL_MSG_MAX         0x4000

/**
 * @ingroup hct_dms
 * HI_DMS_SetPortIoControl接口功能选项
 */
typedef enum
{
    HI_DMS_PORT_CTRL_ONLINE_RX         = 0x0,               /**< 通道仅在组网优化完后能接收数据。此时设置:pInputParam = 0; pOutputParam=0*/
    HI_DMS_PORT_CTRL_OFFLINE_ONLINE_RX = 0x1,               /**< 通道总能接收数据（不管PLC网络组网状态）。 此时设置:pInputParam = 0; pOutputParam=0*/
    HI_DMS_PORT_CTRL_RX_NA             = 0xFF,              /**< 该功能暂未使用 */
    HI_DMS_PORT_CTRL_NO_DELAY          = 0x100,             /**< 表示通道无延时接收（仅用于TCP端口） */
    HI_DMS_PORT_CTRL_RX_TO             = 0x101,             /**< 该功能暂未使用 */
    HI_DMS_PORT_CTRL_RX_FRM_TO         = 0x102,             /**< 表示超时时间，单位为秒。pInputParam=超时时间；pOutputParam=0*/
    HI_DMS_PORT_CTRL_UART_MODE_485     = 0x200,             /**< 485串口模式 */
    HI_DMS_PORT_CTRL_UART_MODE_IR      = 0x201,             /**< 红外模式*/
    HI_DMS_PORT_CTRL_UART_MODE_UART    = 0x202,             /**< 232模式 */
    HI_DMS_PORT_CTRL_UART_CLOSE		   = 0x203,             /**< 关闭用于DMS通道的串口端口 */
    HI_DMS_PORT_PLC_MIN                = HI_DMS_CHL_MSG_MIN,/**< 该功能暂未使用 */
    HI_DMS_PORT_PLC_TOPO_QRY,                               /**< 该功能暂未使用 */
    HI_DMS_PORT_CTRL_MAX
} HI_DMS_PORT_CTRL_OPT_ID_E;
#define HI_DMS_PORT_CTRL_OPEN  HI_DMS_PORT_CTRL_NA



/**
* @ingroup  hct_dms
* @brief  通道连接接口实例指针。
*
* @par 描述:
* 通道接口实例指针，用于在通道初始化时注册用户回调函数。
* @attention 无。
*
* @param  参数1 [IN] 通道端口号。
* @param  参数2 [IN] 控制通道是连接或断开，连接取值为HI_DMS_CHL_CONNECT，断开取值为HI_DMS_CHL_DISCONNECT。
*
* @retval #HI_ERR_SUCCESS 连接成功。
* @retval 其他值 连接失败，返回值含义请参见hi.errno.h。
*
* @par Dependency:
* @li hi_mdm_types.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
typedef HI_U32 (*HI_DMS_CHL_CONNECT_F)(HI_U16, HI_U8);

/**
* @ingroup  hct_dms
* @brief  通道消息处理接口实例指针。
*
* @par 描述:
* 通道接口实例指针，用于在通道初始化时注册用户回调函数。
* @attention 无。
*
* @param  参数1 [IN] 通道端口号。
* @param  参数2 [IN] 消息ID。
* @param  参数3 [IN] 消息参数。
*
* @retval #HI_ERR_SUCCESS 消息处理成功
* @retval 其他值 消息处理失败，返回值含义请参见hi.errno.h。
*
* @par Dependency:
* @li hi_mdm_types.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
typedef HI_U32 (*HI_DMS_CHL_MSG_IND_F)(HI_U16, HI_U32, HI_PVOID);

/**
* @ingroup  hct_dms
* @brief  通道数据接收接口实例指针。
*
* @par 描述:
* 通道接口实例指针，用于在通道初始化时注册用户回调函数。
* @attention 无。
*
* @param  参数1 [IN] 通道端口号。
* @param  参数2 [IN] 通道数据接收的控制结构体
*
* @retval #HI_ERR_SUCCESS 数据接收成功。
* @retval 其他值 接收数据失败，返回值含义请参见hi.errno.h。
*
* @par Dependency:
* @li hi_mdm_types.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
typedef HI_U32 (*HI_DMS_CHL_RX_F)(HI_U16, HI_DMS_CHL_RX_S*);

typedef HI_U32 (*HI_DMS_CHL_IREMOTE_PACKET_F)(HI_U32, HI_PVOID, HI_U16);


/**
 * @ingroup hct_dms
 * 通道接口实例结构
 */
typedef struct
{
    HI_DMS_CHL_CONNECT_F         pfnChlConnect;         /**< 通道连接 */
    HI_DMS_CHL_RX_F              pfnChlDataRx;          /**< 接收数据 */
    HI_DMS_CHL_IREMOTE_PACKET_F  pfnChlIRemoteDataRx;   /**< 暂未使用 */
    HI_DMS_CHL_MSG_IND_F         pfnChlMsgRx;           /**< 通道消息处理 */
} HI_DMS_CHL_PORT_OBJ_S;

#define HI_DMS_DEFAULT_DEV_CHL_IF_VALUE  { HI_NULL, HI_NULL, HI_NULL, HI_NULL}

#if defined(PRODUCT_CFG_COMPATIBILITY_NEW_FEATURE)
#define HI_DMS_FRM_INTER_INFO1_SIZE        (sizeof(HI_U8) + sizeof(HI_U8)) // mmt, vt
#define HI_DMS_FRM_INTER_INFO2_SIZE        (4) // 公用控制字段长度
#else
#define HI_DMS_FRM_INTER_INFO1_SIZE        (HI_PLC_MAC_ADDR_LEN * 2 + sizeof(HI_U32) + sizeof(HI_U16)) // mac, mmt, vt
#define HI_DMS_FRM_INTER_INFO2_SIZE        (8) // 公用控制字段长度
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
 * 数据通信参数
 */
typedef struct
{
    HI_U16 usDataSize;                          /**< 数据大小，即ucData所占空间大小，单位字节 */
    HI_U8 ucInsideInfo[HI_DMS_INTER_INFO_SIZE]; /**< 内部使用 */
    HI_U8 ucData[0];                            /**< 数据 */
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
#define HI_DMS_CHL_TX_TYPE_LB          0x08 // 本地广播
#define HI_DMS_CHL_TX_TYPE_MAX         0x0F

#define HI_DMS_CHL_TX_CTRL_NA          0x00 // 缺省发送
#define HI_DMS_CHL_TX_CTRL_CIPHER      0x01 // 加密模式
#define HI_DMS_CHL_TX_CTRL_FORCE       0x08 // 强制发送,无论是否组网/通道是否可用
#define HI_DMS_CHL_TX_CTRL_MAX         0x0F

/**
 * @ingroup hct_dms
 * PLC通信控制结构，具体使用方法请参考低压电力线宽带载波通信技术规范。
 */
typedef struct
{
    HI_U16   usId;/**< 通用报文结构的报文ID */
    HI_U16   usSn;/**< 暂未使用*/
    HI_U8    ucCtrl;/**< 报文选项字*/
    HI_U8    ucType;/**< 帧类型，取值:HI_DMS_CHL_TX_TYPE_NA为不指定帧类型；HI_DMS_CHL_TX_TYPE_MR表示帧类型为抄表；
                                      HI_DMS_CHL_TX_TYPE_UPG表示帧类型为升级；HI_DMS_CHL_TX_TYPE_DIAG表示帧类型为诊断；0x04～0xFF预留*/
    HI_U8    ucPrio;/**< Vlan标签*/
    HI_U8    ucDuration;/**< 暂未使用 */
    HI_U8    ucMaxResendCount; /**< 设置MAC报文重传次数 */
    HI_U8    ucPhase;           /**< 设置发送的相位只对CCO且广播发送时有效,如果不指定或指定相位错误则按当前默认相位发送。*/
    HI_BOOL  bIsBroadCast; /**< 是否全网广播，当ucDestMac为全FF时有效，HI_TRUE为全网广播，HI_FALSE为代理广播 */
    HI_U8    ucPad;
    HI_U8    ucDestMac[HI_PLC_MAC_ADDR_LEN];  /**< 目标模块的MAC地址, 1)取值全为零, 表示发送到CCO; 2)全为FF表示广播 */
    HI_U16   usUserInfoSize;  /**< 用户自定义控制信息大小 */
    HI_PBYTE pucUserInfo;    /**< 用户自定义控制信息 */
} HI_DMS_CHL_TX_CTRL_S;

typedef HI_DMS_CHL_TX_CTRL_S HI_DMS_CHL_TX_ADDR_S;

#define HI_DIAG_PAYLOAD_MAX_SIZE   512

#define HI_DIAG_TYPE_A     0 // 表示HSO的研发调测版本
#define HI_DIAG_TYPE_B     1 // 表示HSO的现场维护版本
#define HI_DIAG_TYPE_LLN  0x0F // Layer Level N/A
#define HI_DIAG_TYPE_SLN  0xF0 // System Level N/A
#define HI_DIAG_TYPE_SL0  0xF1 // System Level 0

// 打印级别
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
* @brief  获取存储命令数据缓存空间。
*
* @par 描述:
* 当单板收到HSO命令并且判断为并发命令时，通过调用该函数获取存储命令数据的空间。
* @attention
* @li 除非必要，不建议用户使用该接口。
* @li DIAG命令执行完毕后，申请的空间需要通过用户回调函数进行释放，具体使用方法请参见《Hi3911V200 API 开发指南》。
*
* @param  usId [IN] 命令ID。
* @param  ulCacheSize [IN] 命令数据大小。
* @param  usPkSn [IN] 数据包序列号。
*
* @retval HI_NULL 获取空间失败。
* @retval 非HI_NULL 获取空间成功。
*
* @par Dependency:
* @li hi_mdm_diag.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
typedef HI_PVOID (*HI_DIAG_GET_CMD_PK_CACHE_F) (HI_U16 usId, HI_U32 ulCacheSize, HI_U16 usPkSn);

/**
* @ingroup  hct_diag
* @brief 用户命令处理函数。
*
* @par 描述:
* DIAG根据收到数据中的命令ID，调用对应的命令处理函数。
* @attention 无。
*
* @param  ulId[IN] 命令ID。
* @param  ulId[IN] 命令内容指针。
* @param  ulId[IN] 命令内容大小。
* @param  ulId[IN] ulOption命令选项，用户上报应答报文时将收到option传递给对应的上报内容接口。
*
* @retval 不允许返回HI_ERR_NOT_FOUND。当命令处理流程中有调用HI_MDM_SendAckPacket回复ACK时，需返回HI_ERR_CONSUMED。
*
* @par Dependency:
* @li hi_mdm_diag.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
typedef HI_U32(*HI_DIAG_CMD_F) (HI_U16 ulId, HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption);

/**
* @ingroup  hct_diag
* @brief 并发命令处理结束，回调函数。
*
* @par 描述:
* 并发命令处理结束，回调函数。
* @attention 无。
*
* @param  ulId[IN] 命令ID。
* @param  usPkSn[IN] 命令报文序号。
* @param  pCache[IN] 指向在接收到命令时由用户分配的空间。
* @param  ulOption[IN] ulOption命令选项。
*
* @retval 执行结果。
*
* @par Dependency:
* @li hi_mdm_diag.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
#if defined(PRODUCT_CFG_FEATURE_REG_PARALLEL_CMD)
typedef HI_U32(*HI_DIAG_CMD_PARALLEL_CTRL_F) (HI_U16 ulId, HI_U16 usPkSn, HI_PVOID pCache, HI_U32 ulOption);
#endif

#define HI_DIAG_INPUT_CMD_PROC_F HI_DIAG_CMD_F

/**
 * @ingroup hct_diag
 * 命令ID与其处理函数对应关系
 */
typedef struct
{
    HI_U32 ulMinId; /**< 起始命令ID*/
    HI_U32 ulMaxId; /**< 结束命令ID*/
    HI_DIAG_CMD_F pfnInputCmd; /**< 命令处理函数*/
    #if defined(PRODUCT_CFG_FEATURE_REG_PARALLEL_CMD)
    HI_DIAG_CMD_PARALLEL_CTRL_F pfnCmdCtrl;/**< 当命令为并发命令时，命令执行完毕后调用的回调函数*/
    #endif
} HI_DIAG_CMD_REG_OBJ_S;


#define HI_DIAG_INPUT_CMD_PROC_S HI_DIAG_CMD_REG_OBJ_S

#define HI_DIAG_CMD_INSTANCE_DEFAULT      ((HI_U8)0) // 本地实例  -->HSO
#define HI_DIAG_CMD_INSTANCE_LOCAL        HI_DIAG_CMD_INSTANCE_DEFAULT // -->HSO
#define HI_DIAG_CMD_INSTANCE_IREMOTE      1 // 注册间接处理的远程命令实例(处理MAC透传过来的命令), 即处理注册的经过MAC透传后处理的远程命令 -->...-->...
#define HI_DIAG_CMD_INSTANCE_DREMOTE      2 // 注册直接处理的远程命令实例(可能会透传给其他站点), 即处理注册的不经MAC透传就处理的远程命令 -->HSO
#define HI_DIAG_CMD_INSTANCE_DREMOTE_NDM  3 // 注册接收NDM远程命令实例
#define HI_DIAG_CMD_INSTANCE_PLC          4
#define HI_DIAG_CMD_INSTANCE_LOCAL_ANY    5 // 本地端口发送 不检测通道是否可用
#define HI_DIAG_CMD_INSTANCE_UNKNOWN      0xFF // unknown instance

#define HI_DIAG_OBJ_ID_MASK                     (0x07)      //用户对象ID的有效位宽
#define HI_DIAG_INSTANCE_ID_MASK                (0x0f)      //通道模式ID的有效位宽
#define HI_DIAG_OBJ_ID_OFFSET_IN_INSTANCE_ID    (4)         //用户对象ID在通道模式ID中的偏移
#define HI_DIAG_GET_OBJ_ID_FROM_INSTANCE_ID(instanceId) \
            ((instanceId >> HI_DIAG_OBJ_ID_OFFSET_IN_INSTANCE_ID) & HI_DIAG_OBJ_ID_MASK)
#define HI_DIAG_OBJ_ID_OFFSET_IN_DIAGCHLREMOTERX_ID     (28)        //用户对象ID在diagChlRemoteRx()的命令参数中的偏移，参数利用


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
 * 主动重启原因码
 */
typedef enum
{
    HI_SYS_REBOOT_CAUSE_UNKNOWN = 0,

    HI_SYS_REBOOT_CAUSE_D=0x1,
    HI_SYS_REBOOT_CAUSE_C,
    HI_SYS_REBOOT_CAUSE_UPG_C, // upg complete
    HI_SYS_REBOOT_CAUSE_UPG_B, // ver back
    HI_SYS_REBOOT_CAUSE_WD,
    HI_SYS_REBOOT_CAUSE_UPG_SOFT, // 升级过程中，MAC软复位，CCO主动复位

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
	HI_SYS_REBOOT_CAUSE_FPGA_ACTIVE_RST,// 单板长时间温度异常，任务无法启动，超过24小时，主动重启
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
// HI_SDM_VERIFY_EXPR  : 表达式监控
// HI_SDM_VERIFY_RA    : RAM地址监控
// HI_SDM_VERIFY_CA    : 代码地址监控
// HI_SDM_VERIFY_PTR   : 指针监控
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
    HI_U8 ucMrLoopBack;         // 0: 不使用loopback方式;  1: 表示使用loopback方式;
    HI_U8 ucMrRedirect2HsoPort; // 1: 使用DIAG的tcp端口替代uart端口发送MR帧; 2: 使用HSO的UART通道
    HI_U8 ucPlcFrameBypassMac;  // 1: 使用模拟方式替代MAC的SendFrame; 0: 不使用模拟方式
    HI_U8 reserved; // 预留
} HI_SAL_DFX_HOST_CTRL_S;

typedef enum
{
    HI_DFX_MAC_MSG_ROUTER = 0x200,
    HI_DFX_MAC_MSG_DIAG

    // 追加的其它定义值, 必须小于 0x400
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
#define HI_USERDATA_ID_BIDB          2 // 基础库信息 HI_SYS_SaveUserData HI_SYS_GetUserData 参数ucUserDataId取值
#define HI_USERDATA_ID_CRASH_HRD     3
#define HI_USERDATA_ID_CRASH_DATA    4
#define HI_USERDATA_ID_CRASH_DATA1   5
#define HI_USERDATA_ID_CRASH_DATA2   6
#define HI_USERDATA_ID_CRASH_DATA3   7
#define HI_USERDATA_ID_TF_LIST       8
#define HI_USERDATA_ID_BLACK_LIST    9

typedef struct
{
    HI_U32 ulUpgId;          // 升级文件ID
    HI_U32 ulOption;         // 取值为 HI_UPG_REQ_OPTION_NA , HI_REQ_OPTION_CCO, HI_UPG_REQ_OPTION_STA , HI_UPG_REQ_OPTION_BROADCAST, HI_UPG_REQ_OPTION_UNICAST
} HI_UPG_REQ_OPTION_S;

typedef struct
{
    HI_U16 usId;
    HI_U16 usPayloadSize;
    HI_U8 aucPayload[0];
} HI_DIAG_PACKET_S;


#define HI_ND_SYS_STATUS_NORMAL      0 // 正常模式
#define HI_ND_SYS_STATUS_UPGRADE     1 // 升级模式
#define HI_ND_SYS_STATUS_TEST        2 // 测试模式
#define HI_ND_SYS_STATUS_DIAG        4 // 诊断模式
#define HI_ND_SYS_STATUS_UNKNOWN     8 // 未知模式

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

// 站点版本信息
typedef struct
{
    HI_U8  ucSysMode;			/**<单板启动时，处于的工作模式，该变量非实时更新。正常模式:#HI_ND_SYS_STATUS_NORMAL; 升级模式:#HI_ND_SYS_STATUS_UPGRADE; 测试模式:#HI_ND_SYS_STATUS_TEST; 诊断模式:#HI_ND_SYS_STATUS_DIAG; 未知模式:HI_ND_SYS_STATUS_UNKNOWN;*/
    HI_U8  ucBootVerIdx; 		/**<初始化配置BOOT版本号*/
    HI_U8  ucSysCause;   		/**<软重启原因*/
    HI_U8  aucVer[3];    		/**<V，R，C版本号*/
    HI_U8  aucBuildDataTime[6]; /**<编译时间*/

    HI_U16 ausVer[2];    		/**<B及SPC版本号*/
    HI_U16 usChipVer;			/**<芯片版本号。Hi3911v200芯片:NM_CHIP_VER_HI3911_V200; Hi3911v100芯片:NM_CHIP_VER_HI3911_V100; 其他芯片:NM_CHIP_VER_OTHER*/

    HI_U8  ucBootCause;  		/**<正常重启:#HI_ND_SYS_BOOT_CAUSE_NORMAL; 异常重启:#HI_ND_SYS_BOOT_CAUSE_EXP; 看门狗重启:#HI_ND_SYS_BOOT_CAUSE_WD*/

    HI_U8  ucBootVer;       	/**<Bootloader版本*/
    HI_U16 usSwVer;         	/**<软件版本号*/
    HI_U16 usYear  : 7;			/**<版本日期-年*/
    HI_U16 usMonth : 4;     	/**<版本日期-月*/
    HI_U16 usDay   : 5;     	/**<版本日期-日*/
    HI_CHAR szManuCode[2];  	/**<厂商代码*/
    HI_CHAR szChipCode[2];  	/**<芯片代码*/
} HI_SYS_STATUS_INFO_S;

typedef struct
{
    HI_U16 ausNvVer[2];
    HI_U16 usNvVerId;
    HI_U8 ucNvVer[3];
    HI_U8 reserver[3];
}HI_SYS_NV_VER_INFO_S;//存放nv的VR版本号

//站点属性信息
typedef struct
{
    HI_U8 mac[6];						/**<站点MAC地址*/
    HI_U8 level;						/**<站点层级*/
    HI_U8 d2d_proxy_comm_rate;			/**<路径通信率*/

    HI_U16 tei;							/**<设备唯一标示符*/
    HI_U16 proxy_tei;					/**<设备代理*/

    HI_U8  role; 						/**<站点角色，CCO:#HI_ND_TYPE_CCO; 代理或中继站点:#HI_ND_TYPE_RELAY; STA:#HI_ND_TYPE_STA; 抄控宝:HI_ND_TYPE_DCM;*/
    HI_U8  product_type;				/**<站点产品类型，CCO:#HI_ND_TYPE_CCO; 中继:#HI_ND_TYPE_RELAY; STA:#HI_ND_TYPE_STA; 抄控宝:HI_ND_TYPE_DCM;*/
    HI_U8  transformer_result;          /**<台区识别结果，未识别 : #HI_MDM_TRANSFORMER_RESULT_UNKNOWN \n
                                                                   属于本台区 : #HI_MDM_TRANSFORMER_RESULT_BELONG \n
                                                                   不属于本台区 : #HI_MDM_TRANSFORMER_RESULT_NOT_BELONG \n
                                                                   不支持识别 : #HI_MDM_TRANSFORMER_RESULT_NOT_SUPPORT \n
                                                                   识别失败 : #HI_MDM_TRANSFORMER_RESULT_FAIL */
    HI_U8  phase_state  : 4;            /**<相位识别状态\n
                                                                 未识别状态 : #HI_MAC_PHASE_IDENTIFY_STATE_UNKNOWN \n
                                                                 不支持相位识别 : #HI_MAC_PHASE_IDENTIFY_STATE_NOT_SUPPORT \n
                                                                 正在识别 : #HI_MAC_PHASE_IDENTIFY_STATE_DOING \n
                                                                 识别结束 : #HI_MAC_PHASE_IDENTIFY_STATE_OVER */
    HI_U8  phase_result : 4;            /**<相线1的相位识别结果\n
                                                                 A相位 : #PHASE_A \n
                                                                 B相位 : #PHASE_B \n
                                                                 C相位 : #PHASE_C \n
                                                                 空相位 : #INVALID_PHASE */

    HI_U8 mac_type;					  /**<MAC地址类型*/
    HI_U8 phase_result_b : 4;         /**<相线2的相位识别结果\n
                                                                 A相位 : #PHASE_A \n
                                                                 B相位 : #PHASE_B \n
                                                                 C相位 : #PHASE_C \n
                                                                 空相位 : #INVALID_PHASE */
    HI_U8 phase_result_c : 4;         /**<相线3的相位识别结果\n
                                                                 A相位 : #PHASE_A \n
                                                                 B相位 : #PHASE_B \n
                                                                 C相位 : #PHASE_C \n
                                                                 空相位 : #INVALID_PHASE */
	HI_U8  not_rcv_heartbeat_cycle : 4;	/**<未接收到心跳的周期数*/
    HI_U8  is_alloc_beacon_slot    : 1; /**<是否分配信标时隙*/
    HI_U8  rsvd1                   : 3;
    HI_U8 pad2;

    HI_SYS_STATUS_INFO_S sta_ver;		/**<版本信息，参见#HI_SYS_STATUS_INFO_S*/

    HI_U8  up_comm_rate;				/**<上行通信成功率*/
    HI_U8  down_comm_rate;				/**<下行通信成功率*/
    HI_U8  pn_state : 2;   				/**<零火线识别状态\n
                                                                                未识别状态 : #HI_MAC_PN_IDENTIFY_STATE_UNKNOWN\n
                                                                                不支持识别 : #HI_MAC_PN_IDENTIFY_STATE_NOT_SUPPORT\n
                                                                                正在识别 : #HI_MAC_PN_IDENTIFY_STATE_DOING\n
                                                                                识别结束 : #HI_MAC_PN_IDENTIFY_STATE_OVER*/
    HI_U8  pn_result : 2;  				/**<相线1的零火线识别结果\n
                                                                                零火线接线正确 : #PN_CONNECTION_RIGHT\n
                                                                                零火线接线反接 : #PN_CONNECTION_ERROR\n
                                                                                零火线接线情况未知 : #INVALID_PN_CONNECTION*/
    HI_U8  pn_result_b : 2;             /**<相线2的零火线识别结果\n
                                                                                零火线接线正确 : #PN_CONNECTION_RIGHT\n
                                                                                零火线接线反接 : #PN_CONNECTION_ERROR\n
                                                                                零火线接线情况未知 : #INVALID_PN_CONNECTION*/
    HI_U8  pn_result_c : 2;             /**<相线3的零火线识别结果\n
                                                                                零火线接线正确 : #PN_CONNECTION_RIGHT\n
                                                                                零火线接线反接 : #PN_CONNECTION_ERROR\n
                                                                                零火线接线情况未知 : #INVALID_PN_CONNECTION*/
    HI_U32 proxy_change_cnt;            /**<代理变更次数*/
    HI_U32 leave_cnt;                   /**<离线次数*/
    HI_U32 leave_total_time;            /**<离线的总时长*/
} HI_MAC_NETWORK_TOPO_ENTRY_S;

typedef struct
{
    HI_U32 num;			/**<数组元素个数*/
    HI_MAC_NETWORK_TOPO_ENTRY_S entry[HI_MAC_NETWORK_TOPO_ENTRY_MAX];	/**<拓扑站点属性信息*/
} HI_MAC_NETWORK_TOPO_S;

typedef struct
{
    HI_U8 ucLevel; /**<站点层级*/
    HI_PVOID pTopo;       /**<指向 #HI_MAC_NETWORK_TOPO_S 结构构成的数组首地址*/
} HI_MAC_CONFIG_INFO_S;

#define HI_HNV_FILE_SIGNATURE                HI_MAKE_IDENTIFIER('H','N','V','$')

#define HI_UPG_FILE_SIGNATURE                HI_MAKE_IDENTIFIER('H','i','s','i')

/**
 *@ingroup hct_upg
 * 表示非全网升级文件格式。
 */
#define HI_UPGA_FILE_FMT_VER_1              0x01
/**
 *@ingroup hct_upg
 * 表示全网升级文件格式。
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
 * 表示目标机器未指定，升级文件被认为非法。
*/
#define HI_UPG_FILE_MACHINE_UNKNOWN         0x00
/**
 *@ingroup hct_upg
 * 表示目标机器为ARM926EJ-S。
*/
#define HI_UPG_FILE_MACHINE_ARM926EJ_S      0x01
/**
 *@ingroup hct_upg
 * 表示目标机器为X86仅用于模拟环境。
*/
#define HI_UPG_FILE_MACHINE_X86             0x02

#define HI_UPG_FILE_BLK_TRANSMIT_DELIMITER  0x9F9F // 块传输定界符
#define HI_UPG_TRANSMIT_BLK_SIZE            (300)     // 块固定大小
#define HI_UPG_ACK_BLK_NUM                  (10)      // 每满10块反馈HSO ACK
#define HI_UPG_TRANSMIT_BLK_SIZE_MAX        (1024*2)
#define HI_UPG_TRANSMIT_BLK_NUM_MAX         (1024)    // 块最大个数
#define HI_UPG_TRANSMIT_FILE_INVALID        (0xFFF0)
#define HI_UPG_TRANSMIT_FILE_OK             (0xFFFF)
#define HI_UPG_TRANSMIT_FILE_SN_UNKNOWN     (~0)
#define HI_UPG_FILE_BLK_SIZE                (HI_UPG_TRANSMIT_BLK_SIZE*HI_UPG_ACK_BLK_NUM)//6000Bytes//(PRODUCT_CFG_FLASH_BLOCK_SIZE) // 升级文件存储的块固定大小
#define HI_UPG_FILE_BLK_NUM_MAX             (PRODUCT_CFG_UPG_FILE_MAX_SIZE/PRODUCT_CFG_FLASH_BLOCK_SIZE) // 升级文件存储的块块最大个数
#define HI_UPG_FILE_FULL_SIZE               0x1FFFFFF0

#define HI_UPG_FILE_CODE_RAM_ADDR_AUTO      0xFFFFFFFF

#define HI_UPG_FILE_UPG_END                 0xFFFFFFFF

#define HI_UPG_VERSION_NEW      1
#define HI_UPG_VERSION_BACK     2
#define HI_UPG_VERSION_ANY      3

/**
 *@ingroup hct_upg
 *升级文件版本。
*/
typedef struct
{
    HI_U8 ucVVerNo;    /**< V版本号。   */
    HI_U8 ucRVerNo;    /**< R版本号。   */
    HI_U16 usBVerNo;    /**< B版本号。   */
    HI_U8 ucCVerNo;    /**< C版本号。   */
    HI_U8 ucSpcNo;    /**< SPC版本号。   */
    HI_U8 ucPlatform;    /**< 平台编号：3表示8M RAM平台，4表示2M RAM平台。   */
    HI_U8 ucProductNo;    /**< 产品编号：0xFF表示V100产品，0xFE表示V200产品。   */
} HI_UPG_FILE_VER_S;

/**
 *@ingroup hct_upg
 *Section列表。
*/
typedef struct
{
    HI_U32 ulId;    /**< Section ID值，0表示不使用该Section。   */
    HI_U32 ulOffset;    /**< Section相对ucData的偏移。   */
    HI_U32 ulSectionSize;    /**< Section长度，单位：byte。   */
    HI_UPG_FILE_VER_S stVer;    /**< Section版本。   */
} HI_UPG_SECTION_ID_S;

/**
 *@ingroup hct_upg
 * 配置Section同时能支持的NV配置文件列表。
*/
typedef struct
{
    HI_UPG_SECTION_ID_S astCfg[8];    /**< 同时能支持的NV配置文件。   */
    //HI_U32 ulFlashAddress;        /**< 该Section保存到Flash的地址，目前版本不使用，该字段预留。   */
} HI_UPG_FILE_CFG_SECTION_S;

/**
 *@ingroup hct_upg
 * 代码section列表，最多支持8个Section。
*/
typedef struct
{
    HI_UPG_SECTION_ID_S astCfg[8];    /**< 同时能支持的CODE文件。   */
    uintptr_t ulRamAddress;    /**< 该Section加载到RAM的地址。   */
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
 * 升级文件头结构定义。
*/
typedef struct
{
    HI_U32 ulMagic;    /**<魔术字。STA升级文件为Hisi，其它升级文件为HISI。*/
    HI_U32 ulChecksum;    /**<CRC检验，即升级文件头（ulChecksum成员之后，不包括ulChecksum成员）和升级数据（长度为ulDataSize）的CRC校验。*/
    HI_CHAR szDateTime[20];    /**<升级文件生成的日期和时间， 如20120517-18:01:00。*/
    HI_U8 ucMachine;    /**<目标机器。\n
								HI_UPG_FILE_MACHINE_ARM926EJ_S表示ARM926EJ-S。\n
								HI_UPG_FILE_MACHINE_X86 表示X86，仅用于模拟环境。\n
								其它值待扩展使用。*/
    HI_U8 ucNumberOfCodeSections;    /**<代码Section个数。*/
    HI_U8 ucNumberOfCfgSections;    /**<配置Section个数。每个section表示不同升级配置数据类型（通过Section ID区分）。*/
    HI_U8 ucFmtVer;    /**<文件格式版本。HI_UPGA_FILE_FMT_VER_BUPG表示全网升级，HI_UPGA_FILE_FMT_VER_1非全网升级文件。*/
    HI_UPG_FILE_CFG_SECTION_S stSecCfg;    /**<配置Section 列表。*/
    HI_CHAR szManuCode[2];    /**<厂商代码。HS代表海思。*/
    HI_U16 reserved;    /**<保留。*/
    HI_UPG_FILE_CODE_SECTION_S stSecCode;    /**<代码Section列表。*/
    HI_U32 aulSHA256[8];    /**<升级文件数据SHA256校验。*/
    HI_U32 ulDataSize;    /**<所有Section总长度, 单位：byte。*/
} HI_UPG_FILE_HEADER_S;

#define HI_UPG_FILE_HEADER_SIZE  396

typedef struct
{
    HI_U32 ulUpgId;
    HI_U32 ulErrorCode;
    HI_U32 ulBlkSn;         // 升级文件块发送的序号，从0开始.
    HI_U32 ulBlkIdx;        // 升级文件块传输的索引号，从1开始, 且索引号连续.
    HI_U32 ulSectionId;     // 升级的 Section ID, 0表示没有Section升级.
    HI_U32 ulNextSectionId; // 开始升级下一个Section, 0表示已经没有可用Section可升级.
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
    HI_UPG_FILE_HEADER_S stFileHeader; // 备份区升级文件信息
} HI_DRV_UPG_FALSH_BACKUP_INFO_S; // 备份区

typedef struct
{
    HI_U32 ulVersionType;
    HI_U32 ulCodeFlashAddr;
    HI_U32 ulCfgFlashAddr;
    HI_U32 ulUpgId;
    HI_UPG_FILE_HEADER_S stFileHeader; //  运行区 升级文件信息
} HI_DRV_UPG_FLASH_RUNING_INFO_S;  // 运行区

typedef struct
{
    HI_U32 ulSysStatus;
    HI_DRV_UPG_FLASH_RUNING_INFO_S stFlashRunInfo;    // 运行区信息
    HI_DRV_UPG_FALSH_BACKUP_INFO_S stFlashBackupInfo; // 备份区信息
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
    HI_U16 usBlkIdx;  // 块传输的索引号
    HI_U16 usBlkSize;
    HI_U8* pucFileBlk;
} HI_UPG_FILE_BLK_UPDATE_S;

typedef struct
{
    HI_U32 ulUpgId;
    HI_U16 usBlkReqSn;
    HI_U16 usBlkIdx;  // 块传输的索引号
    HI_U16 usBlkSize;
    HI_U8* pucFileBlk;
} HI_UPG_FILE_BLK_INFO_S;


//#endif // UPG ONLY
//*****************************************************************************

// 以下为重启条件
#define  SAL_ANY_MR_FAIL_TIME           (24 * 60 * 60)      // 默认值24小时*60分*60秒      对应 HI_SYS_RST_CON_ANY_MR_FAIL
#define  SAL_ANY_MR_FAIL_SCAL           (30)                // 默认值30块表                对应 HI_SYS_RST_CON_ANY_MR_FAIL
#define  SAL_NO_FRAME_TIME              (24 * 60 * 60)      // 默认值24小时*60分*60秒      对应 HI_SYS_RST_CON_NOT_GET_FARME
#define  SAL_STA_NO_UART_DATA_TIME      (12 * 60 * 60)      // 默认值12小时   对应 HI_SYS_RST_CON_NOT_GET_METER_UART_DATA
#define  SAL_MAX_PROXY                  (150)               // 默认值150个代理以上             对应 HI_SYS_RST_CON_CCO_TOO_MANY_AGENT
#define  SAL_MAX_PROXY_TIME             (12 * 60  *60)      // 默认值150个代理 持续时间            对应 HI_SYS_RST_CON_CCO_TOO_MANY_AGENT
#define  SAL_STA_NO_BEA_OR_SOF_TIME     (30 * 60)           // 默认值30分*60秒             对应 HI_SYS_RST_CON_STA_NOT_TX HI_SYS_RST_CON_STA_NOT_RX
#define  SAL_CCO_NO_BEA_OR_SOF_TIME     (24 * 60 * 60)      // 默认值24小时*60分*60秒      对应 HI_SYS_RST_CON_CCO_NOT_TX HI_SYS_RST_CON_CCO_NOT_RX

#define  SAL_STA_SECURE_RST_DEFUAT_TIME (135 * 24 * 60 * 60)// 默认值135天  对应值 HI_SYS_RST_CON_STA_SECURE_RESET
#define  SAL_CTL_II_NO_UART_DATA_TIME   (2 * 24 * 3600)     // 默认2天 对应值 HI_SYS_RST_CON_NOT_GET_METER_UART_DATA_CLT_II
#define  SAL_STA_NO_PLC_DATA_TIME       (2 * 24 * 3600)     // 默认2天 对应值 HI_SYS_RST_CON_NOT_GET_METER_PLC
#define  SAL_SYS_RESET_TIME_24_HOURS    (24 * 60 * 60)      // 操作系统级错误 对应  HI_SYS_RST_CON_TIMER_EXHAUST  -> HI_SYS_RST_CON_MEMPOOL_ERR
#define  SAL_SYS_MAC_EXCE_TIME_24_HOURS (24 * 60)           // MAC协议栈异常时间，单位分钟

typedef enum
{
    HI_SYS_RST_CON_UNKNOWN = 0,                     // 修改0为1，防止出现默认填写出现FLASH写入值为0，对查问题无任何帮助

    HI_SYS_RST_CON_ANY_MR_FAIL,                     // CCO 所有表无法抄通, （规模大于30，建议时间24小时）
    HI_SYS_RST_CON_NOT_GET_FARME,                   // CCO 连续很长时间接收不到集中器的抄表报文,（时间一天）。对实验室无抄表业务有影响。
    HI_SYS_RST_CON_NOT_GET_METER_UART_DATA_STA,     // STA(不含II采) 同电表串口长时间失败 12小时

    HI_SYS_RST_CON_CCO_TOO_MANY_AGENT,              // CCO 3.2M带宽代理站点超过xx个，且超过12小时
    HI_SYS_RST_CON_STA_NOT_TX,                      // STA 接收不到SOF或者Beacon 超过90分钟，底板方案不复位 TODO
    HI_SYS_RST_CON_STA_NOT_RCV_BEA_SOF,             // STA 接收不到SOF并且接收不到Beacon 超过30分钟，底板方案不复位 TODO
    HI_SYS_RST_CON_STA_NOT_RCV_BEA,                 // STA 接收不到Beacon 超过30分钟，底板方案不复位 TODO
    HI_SYS_RST_CON_STA_NOT_RCV_SOF,                 // STA 接收不到SOF 超过30分钟，底板方案不复位 TODO
    HI_SYS_RST_CON_CCO_NOT_TX,                      // CCO 接收不到SOF或者Beacon 超过8小时，底板方案不复位 TODO
    HI_SYS_RST_CON_CCO_NOT_RCV_BEA_SOF,             // CCO 接收不到SOF并且Beacon 超过8小时，底板方案不复位 TODO
    HI_SYS_RST_CON_CCO_NOT_RCV_BEA,                 // CCO 接收不到Beacon 超过8小时，底板方案不复位 TODO
    HI_SYS_RST_CON_CCO_NOT_RCV_SOF,                 // CCO 接收不到SOF 超过8小时，底板方案不复位 TODO
    HI_SYS_RST_CON_STA_SECURE_RESET,                // 随机重启(防止单板长期运行产生的不可恢复问题) 90天-180天
    HI_SYS_RST_CON_NOT_GET_METER_UART_DATA_CLT_II,  // II采同电表串口长时间失败 48小时
    HI_SYS_RST_CON_NOT_GET_METER_PLC,               // STA 48小时未收到CCO下发的抄表帧
    HI_SYS_RST_CON_TIMER_EXHAUST,                   // TIMER资源耗尽,24小时后重启
    HI_SYS_RST_CON_JOBADD_EXHAUST,                  // 中断后半段资源耗尽，24小时后重启
    HI_SYS_RST_CON_MEM_NOT_ENOUGH,                  // 内存不足,24小时后重启
    HI_SYS_RST_CON_MEMPOOL_ERR,                     // 动态内存池检测错误,24小时后重启
    HI_SYS_RST_CON_MAC_BUFFER_ERR_CCO,              // CCO MAC BUFFER低于水限,空闲BUFFER个数小于20个，持续24小时后重启
    HI_SYS_RST_CON_MAC_BUFFER_ERR_STA,              // STA MAC BUFFER低于水限，空闲BUFFER个数小于20个，持续24小时后重启
    HI_SYS_RST_CON_NOT_JOIN_NETWORK_ERR,            // STA 未加入网络，持续24小时后重启
    HI_SYS_RST_CON_ROUTE_EVALUATE_ERR,              // STA 未启动路由评估，持续24小时后重启
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
    HI_SYS_RST_DELAY_CON_WR_NV,// 写NV项过程中，在超时内，不强制重启
    HI_SYS_RST_DELAY_CON_WR_MR_DOC, // 写表档案过程中，在超时内，不强制重启
	HI_SYS_RST_DELAY_CON_USR_RESERVED_2,
	HI_SYS_RST_DELAY_CON_USR_RESERVED_3,
	HI_SYS_RST_DELAY_CON_USR_RESERVED_4,
	HI_SYS_RST_DELAY_CON_USR_RESERVED_5,
	HI_SYS_RST_DELAY_CON_USR_RESERVED_6,
	HI_SYS_RST_DELAY_CON_USR_RESERVED_7,

    HI_SYS_RST_DELAY_CON_UPG = HI_SYS_RST_DELAY_CON_USER_END,       // 升级过程中, 不强制重启。
	HI_SYS_RST_DELAY_CON_BOOT_UPG,
    HI_SYS_RST_DELAY_CON_NO_PB,     // 无法收到PB块，导致频偏设置长时间超时（1000分钟），不强制重启
    // 万剑新增部分
    // 无
    // 郭攀新增
    // CCO在写组网系列号的时候，不允许重启；
    HI_SYS_RST_DELAY_CON_SUIT_NET,
    // CCO/STA在写SNID的时候，不允许重启；
    HI_SYS_RST_DELAY_CON_WRT_SNID,
    // STA在写关联随机数的时候，不允许重启
    HI_SYS_RST_DELAY_CON_RELATING_RANDOM_NUM,
    // STA在抄控器对其做整表测试时，不允许重启
    HI_SYS_RST_DELAY_CON_NDM_METER_CHECK,
    // 台区识别时不允许重启
    HI_SYS_RST_DELAY_CON_TF_IDENTIFY,
    // STA上电11小时内，锁定网络状态下，不允许重启
    HI_SYS_RST_DELAY_CON_LOCK_NETWORK,

	// 进入测试模式后 24 小时内不允许重启
    HI_SYS_RST_DELAY_ENTER_PHY_TEST_MODE,

    HI_SYS_RST_DELAY_CON_MAX
}HI_SYS_RST_DELAY_CON_E;

 // 延迟定义 单位秒


#define HI_SYS_RST_DELAY_CON_WR_NV_TO 10000
#define HI_SYS_RST_DELAY_CON_WR_MR_DOC_TO 10000// ms
#define HI_SYS_RST_DELAY_CON_BOOT_UPG_TO (120*1000)//120s
// CCO在写组网系列号的时候，不允许重启；
#define HI_SYS_RST_DELAY_CON_SUIT_NET_TO (60*1000)
// STA在写关联随机数的时候，不允许重启
#define HI_SYS_RST_DELAY_CON_RELATING_RANDOM_NUM_TO (60*1000)
// STA在抄控器对其做整表测试时，延时24小时再重启
#define HI_SYS_RST_DELAY_CON_NDM_METER_CHECK_TO (24 * 60 * 60 * 1000)

// STA在进入测试模式时，延时24小时再重启
#define HI_SYS_RST_DELAY_ENTER_PHY_TEST_MODE_TO (24 * 60 * 60 * 1000)

// 各组件投票返回结果,默认为HI_SYS_ENUM_RST_NO_REQ无需求
typedef enum
{
    HI_SYS_ENUM_RST_NO_REQ    = 0,// 无重启要求；
    HI_SYS_ENUM_RST_DO_PLEASE,    // 需要立即重启
    HI_SYS_ENUM_RST_NOT_PLEASE,   // 此时不能重启
    HI_SYS_ENUM_RST_MAX
}HI_SYS_RST_COMPONENT_VOTE_E;

// 投票返回的数据结构
#define SAL_RST_TYPE_CCO             2 // 头端
#define SAL_RST_TYPE_STA_OR_RELAY    3 // 终端(普通站点)
#define SAL_RST_TYPE_INVLAID    0xdeaddead
typedef struct
{
    HI_U32 enRebootCondition;// 重启条件
    HI_U32 ulKickDogTimes;   // 喂狗次数
    HI_U32 ulUnormal;        // 记录异常
    HI_U32 ulRole;           // 什么角色 0xdeaddead为角色异常
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
// 断言, 仅供开发内部调试使用
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
    HI_SYS_INIT_STATUS_MEM_LOW  = 0x02,  // 内存不足, 进入DIAG模式
    HI_SYS_INIT_STATUS_MOD_FAIL = 0x04,
    HI_SYS_INIT_STATUS_MAX
} HI_SYS_INIT_STATUS_E;
//****************************************************************************


//****************************************************************************
//MAC帧控制信息
typedef struct
{
    HI_U8 ucSrcMac[HI_PLC_MAC_ADDR_LEN];
    HI_U8 ucDestMac[HI_PLC_MAC_ADDR_LEN];
    HI_BOOL is_ee_iv;           // 是否加密
    HI_U8 max_resend_count;     // 报文重传次数设置
    HI_U8 ucSnid;
    HI_U8 ucPhase;              // 配置报文发送相位

    HI_U8   ndm_special_ver;    // 是否为北京抄控器数据,后续扩展 为多个适配类型
    HI_BOOL is_broadcast;       // 当MAC地址为全0xFF时，HI_TRUE为全网广播，HI_FALSE为代理广播
    HI_U16  pad1;
} DMS_MAC_FRAME_CTRL_S;
//****************************************************************************

/**
 * @ingroup hct_ETH
 * 以太网口信息
 */
typedef struct
{
    HI_U8  ip_addr[HI_IPV4_ADDR_LEN];             /**< IP地址 */
    HI_U8  net_mask[HI_IPV4_ADDR_LEN];          /**< 子网掩码*/
    HI_U8  gateway[HI_IPV4_ADDR_LEN];           /**< 默认网关*/
    HI_U8  mac_addr[HI_PLC_MAC_ADDR_LEN];	  /**< mac地址*/
    HI_U8  reserved[2];                                    /**< 保留字节*/
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
#define HI_CHECK_DEFAULT_ID(id)           (id) // 为了方便使用，这里的宏定义不做PCLINT检查
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
// 系统诊断量定义
//*****************************************************************************

#define HI_DSID_TYPE_QRY           0x00
#define HI_DSID_TYPE_PERIOD        0x01
#define HI_DSID_TYPE_CUSTOM        0x02

#define HI_DSID_MAX_NUM    0xFFFF

/**
 * @ingroup hct_diag
 * 统计量列表结构
 */
typedef struct
{
    HI_U16 usId;                /**< 统计量命令ID，ID范围为[0xEE00, 0xF000)。 */
    HI_U8 ucOption;             /**< 统计量等级，填写默认值HI_SYS_DMID_BUSINESS_L2 */
    HI_U8 ucType;               /**< 统计量类型，HI_DSID_TYPE_QRY表示查询上报（默认支持），HI_DSID_TYPE_PERIOD表示周期性主动上报（周期默认5s）。*/
    HI_U16 usArrayCnt;          /**< 当统计量为a时取值为1，当统计量为数组a[n]时取值为n*/
    HI_U16 uStatPacketSize;     /**< 当统计量为a时取值为sizeof(a)，当统计量为数组a[n]时取值为sizeof(a[0])*/
    HI_PVOID pStatPacket;       /**< 统计量地址，必须为全局变量*/
} HI_DIAG_SYS_STAT_OBJ_S;

#define HI_NDM_645_FRAME                    0x3500  // 收发645帧 对应结构为 HI_NDM_645_FRAME_S
#define HI_STA_TRANSMIT                     0x3501  // 转发STA数据给电表
#define HI_CLTII_R485_TRANSMIT_DATA         0x3502

// HI_NDM_645_FRAME 命令对应结构
typedef struct
{
    HI_U16 usFrameSize;     // 帧大小, 即aucFrameSize的实际长度, 单位字节
    HI_U16 reserved;        // 预留字段
    HI_U8 aucFrame[256];
} HI_NDM_645_FRAME_S;
//****************************************************************************


//****************************************************************************
// NDM 的帧格式定义
//****************************************************************************
typedef struct
{
    HI_U32 ulSn;
    HI_U16 usTransId;      // 传输标识
    HI_U16 usControl  :15; // 控制字
    HI_U16 usDULink   :1;  // 上下行标识

    HI_U16 usUserData;     // 用户自定义数据, 当前版本表示 HSO包ID
    HI_U8 ucProdType;
    HI_U8 pad;             //保留

    HI_U8 aucMacAddr[HI_PLC_RELAY_NUM_MAX][HI_PLC_MAC_ADDR_LEN]; // 目标地址
    HI_U8 aucNdmMacAddr[HI_PLC_MAC_ADDR_LEN];                    //NDM自用MAC，保持为0
} HI_ND_REMOTE_FRAME_HEAD_S;
//****************************************************************************

//****************************************************************************
// 系统状态定义
//****************************************************************************/
#define HI_ND_NET_STATUS_FORM_FAIL       0
#define HI_ND_NET_STATUS_FORM_END        1
#define HI_ND_NET_STATUS_FORMING         2
#define HI_ND_NET_STATUS_FORM_CHANGED    3
#define HI_ND_NET_STATUS_FORM_UNKNOWN    0xFF

#define HI_ND_SYS_ERR_STATUS_UNKNOWN      0xFF
#define HI_ND_SYS_ERR_STATUS_OK           0x0
#define HI_ND_SYS_ERR_STATUS_CRASH_FOUND  0x1

#define HI_ND_SYS_BOOT_CAUSE_NORMAL         0x0  // 正常重启
#define HI_ND_SYS_BOOT_CAUSE_EXP            0x1  // 异常重启
#define HI_ND_SYS_BOOT_CAUSE_WD             0x2  // 看门狗重启
#define HI_ND_SYS_BOOT_CAUSE_UPG_VERIFY     0x3  // 升级验证重启
#define HI_ND_SYS_BOOT_CAUSE_UPG_FAIL       0x4  // 升级失败重启
#define HI_ND_SYS_BOOT_CAUSE_UPG_BACK_FAIL  0x5  // 升级回退失败重启

#define HI_ND_SYS_UPG_CAUSE_NORMAL          0x01  // 表示升级无异常
#define HI_ND_SYS_UPG_CAUSE_FAIL            0x02  // 表示升级错误

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
    HI_U16 ausVer[2];           // B,SPC版本 {012,1}对应B012SPC001
    HI_U8  aucVer[3];           // VRC版本{100,1,2}对应V100R001C002
    HI_U8  aucBuildDataTime[6]; // 升级文件构建的日期和时间{12,12,31,15,59,47} 对应2012-12-31 15:59:47
    HI_U8  ucBootVer;           // BOOT版本35对应x.x.35
    HI_U8  ucSysMode;           // 当前系统模式  HI_ND_SYS_STATUS_XXX
    HI_U8  ucTestMode;          // 当前系统测试子模式
    HI_U16 usSwVer;             // 客户软件版本号,2字节BCD
    HI_U16 usYear  : 7;         // 客户版本:年
    HI_U16 usMonth : 4;         // 客户版本:月
    HI_U16 usDay   : 5;         // 客户版本:日
    HI_CHAR szManuCode[2];      // 客户版本:厂商代码ASCII
    HI_CHAR szChipCode[2];      // 客户版本:芯片代码ASCII
    HI_U16 usChipVer;           // 芯片版本
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
    HI_CHL_MSG_SYNC_WHITE_LIST,		// 同步白名单消息ID
    HI_CHL_MSG_ASSOC_SYS_INFO,      // MAC-->UPG
    HI_CHL_MSG_BOARDCAST_CFG,        // UPG-->MAC
    HI_CHL_MSG_TF_START_SEARCH_METER,    //台区识别启动搜表,MAC-APP
    HI_CHL_MSG_TF_STOP_SEARCH_METER,     //台区识别停止搜表,MAC-APP

    HI_CHL_MSG_MAX = HI_DMS_CHL_MSG_MAX
}HI_CHL_MSG_E;

#define HI_ND_TYPE_DCM      1 // 调试控制模块(抄控器)
#define HI_ND_TYPE_CCO      2 // 头端
#define HI_ND_TYPE_STA      3 // 终端(普通站点)
#define HI_ND_TYPE_RELAY    4 // 中继
#define HI_ND_TYPE_CLTII    5 // II采
#define HI_ND_TYPE_CLTI     6 // I采
#define HI_ND_TYPE_THREE_STA 7 // 三相表模块

#define HI_ND_DEV_TYPE_METER       0x10 // 电表
#define HI_ND_DEV_TYPE_METER_COL   0x11 // 采集器
#define HI_ND_DEV_TYPE_CCO         0x12 // CCO
#define HI_ND_DEV_TYPE_UNKNOWN     0xFF // 无法识别的设备
//****************************************************************************

//****************************************************************************
// HI_DIAG_SetupLoadFileNoitfyProc Option.
#define HI_DIAG_LD_FILE_OPT_NA     0x00 // 未指定的选项
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
    HI_CFG_PERFORMANCE_HIGH,//高性能设备
    HI_CFG_PERFORMANCE_LOW,//低性能设备
    HI_CFG_PERFORMANCE_MAX,//枚举最大值
}HI_CFG_PERFORMANCE_E;//早期配置设备性能选项值

#define CHANLLENGE_COMMON_SIZE 32
#define CHANLLENGE_SALT_SIZE 16
/******************************DIAG操作日志 ST***********************************************/
/**
 * @ingroup hct_diag
 * diag日志类型为本地命令
 */
#define HI_DIAG_LOG_OPT_LOCAL_CMD  1

/**
 * @ingroup hct_diag
 * diag日志类型为本地应答
 */
#define HI_DIAG_LOG_OPT_LOCAL_IND  2

/**
 * @ingroup hct_diag
 * diag日志类型为本地ACK
 */
#define HI_DIAG_LOG_OPT_LOCAL_ACK  3

/**
 * @ingroup hct_diag
 * diag日志类型为远程命令
 */
#define HI_DIAG_LOG_OPT_REMOTE_CMD 4

/**
 * @ingroup hct_diag
 * diag日志类型为CCO 接收到的STA 命令回复
 */
#define HI_DIAG_LOG_OPT_DREMOTE_CMD 5

/**
 * @ingroup hct_diag
 * DIAG操作通知描述信息
 */
typedef struct
{
    HI_U32 opt;/**<操作类型，取值为HI_DIAG_LOG_OPT_XXX*/
    HI_U8 plcMac[6];/**< 操作类型为远程命令时，取值为目标单板MAC地址；否则取值为0*/
    HI_U8 reserver[2];
}HI_DIAG_LOG_CTRL_S;


/**
* @ingroup  hct_diag
* @brief DIAG操作通知函数。
*
* @par 描述:
* 当有DIAG数据交互时，通过该函数将交互信息传递给APP。
* @attention
* @li 用户回调函数中不可进行阻塞性操作 （建议直接发送超时时间为0的消息给其他任务进行处理），否则会影响DIAG交互性能，甚至彻底阻塞DIAG通道。
* @li 用户回调函数中不可对交互数据进行修改，否则会造成DIAG交互数据错误。
* @li HI_DIAG_LOG_CTRL_S结构后续可能扩展，用户需注意结构扩展可能造成的兼容性问题。
*
* @param  ulId[IN] 命令ID。
* @param  pUsrData[IN] 交互数据指针。
* @param  usUsrDataSize[IN] 交互数据大小。
* @param  pCtrl[IN] 其它描述信息。
*
* @par Dependency:
* @li hi_mdm_diag.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
typedef HI_VOID (*HI_DIAG_CMD_NOTIFY_F)(HI_U16 ulId,HI_PVOID pUsrData,HI_U16 usUsrDataSize,HI_DIAG_LOG_CTRL_S * pCtrl);

#define DIAG_CMD_HOST_CONNECT_FOR_LOG                             (0x5000)//DIAG通道连接
#define DIAG_CMD_HOST_DISCONNECT_FOR_LOG                          (0x5001)//DIAG通道断开连接
#define DIAG_CMD_LOG_IN_PWD_CHECK_RESULT_FOR_LOG                  (0x5057)//登入密码校验结果

#define HI_LOGIN_AUTH_RESULT_ID_ACCOUNT_LOCKED_FOR_LOG 1//账号锁定
#define HI_LOGIN_AUTH_RESULT_ID_PASS_WORD_WRONG_FOR_LOG 47//密码错误
#define HI_LOGIN_AUTH_RESULT_ID_SYSTEM_CALL_ERROR_FOR_LOG 2999//其它错误
typedef struct
{
    HI_U32 errId;// HI_LOGIN_AUTH_RESULT_ID_XXX
    //当errId为HI_LOGIN_AUTH_RESULT_ID_PASS_WORD_WRONG_FOR_LOG时data表示还有几次机会账号被锁定(0xFFFFFFFF表示不支持锁定)；当errId为HI_LOGIN_AUTH_RESULT_ID_ACCOUNT_LOCKED_FOR_LOG时data表示账号剩余锁定时间(单位s)，其它情况无意义
    HI_U32 data;
}HI_DIAG_LOG_IN_PWD_CHECK_RESULT_FOR_LOG_S;
/******************************DIAG操作日志 END***********************************************/


/******************************基础信息库相关 ST***********************************************/
#define ID_DIAG_CMD_BIDB_EXPORT      0x3403  // 参数: HI_BIDB_EXPORT_REQ_S,,HI_BIDB_EXPORT_IND_S
#define ID_DIAG_CMD_BIDB_IMPORT      0x3405  // 参数: HI_BIDB_IMPORT_REQ_S,,HI_BIDB_IMPORT_IND_S

#define HI_BIDB_ITEM_NUM_MAX         1024
#define HI_BIDB_ITEM_NUM             10

#define HI_BIDM_IMPORT_CTRL_ADD      0 // 新增，如果已经存在，则覆盖
#define HI_BIDM_IMPORT_CTRL_UPDATE   1 // 替换，如果已经存在，则不覆盖
#define HI_BIDM_IMPORT_CTRL_REMOVE   2 // 替换


// 基础信息数据库基本信息项定义
#define HI_MODULE_POSITION_LEN_MAX       64
typedef struct
{
    HI_U8 ucMacAddr[HI_PLC_MAC_ADDR_LEN];           // 通信模块的MAC地址
    HI_U8 ucConnectedDevAddr[HI_PLC_MAC_ADDR_LEN];  // 模块连接的设备（如电表）的地址
    HI_CHAR szPositoin[HI_MODULE_POSITION_LEN_MAX]; // 通信模块的地理位置
} HI_BIDB_ITEM_S;

typedef struct
{
    HI_U16 usSn;//本次导出的基础信息库条目起始ID
    HI_U8  ucItems;//本次实际导出条目数
    HI_U8  bFinish;//导出完成
    HI_U32 ulRc;// 结果码 : HI_ERR_SUCCESS表示成功; HI_ERR_INVALID_PARAMETER:表示命令错误;  HI_ERR_NO_MORE_DATA:表示无数据
    HI_BIDB_ITEM_S astItem[HI_BIDB_ITEM_NUM];
} HI_BIDB_EXPORT_IND_S;

typedef struct
{
    HI_U16 usSn;//本次导出的基础信息库条目起始ID,0表示导入开始。
    HI_U16 usItems;//本次导出的基础信息库条目个数
    HI_U32 ulRc;
} HI_BIDB_EXPORT_REQ_S;

typedef struct
{
    HI_U16 usSn;//本次导入的基础信息库条目起始ID，0表示导入开始.
    HI_U8  ucItems;//本次导入的基础信息库条目个数,usSn==0且ucItems==0xFFFF时表示清空
    HI_U8  bFinish;//是否是最后一次导入
    HI_U32 ulRc;//无意义
    HI_BIDB_ITEM_S astItem[HI_BIDB_ITEM_NUM];//基础信息库内容
}HI_BIDB_IMPORT_REQ_S;

typedef struct
{
    HI_U16 usSn;//与REQ一致
    HI_U16 usItems;//实际导入条目
    HI_U32 ulRc;// 结果码
} HI_BIDB_IMPORT_IND_S;
/******************************基础信息库相关 END***********************************************/

/************* 广播/单播升级 NV项, 使用说明参考  << Hi3911V100 抄控器软件接口设计说明书.docx >> ST***************/
#define ID_DIAG_CMD_UPG_START 0x3412  // 参数: HI_UPG_START_REQ_S, , HI_UPG_START_IND_S
#define ID_DIAG_CMD_UPG_NV_W  0x3510       // 参数: HI_UPG_NV_W_REQ_S, ,

typedef struct
{
    HI_U16 usNvId;//NV ID
    HI_U16 usCrc;//
    HI_U8  ucMac[6];//目标PLC MAC地址
    HI_U8  ucTimes;//广播次数
    HI_U8  reserved[7];
    HI_U16 usPayloadLen; // payload 有效长度
    HI_U8  ucPayload[0]; // payload最大存储空间
} HI_UPG_NV_BW_REQ_S;

typedef struct
{
    HI_U32 ulRc;     // 结果码, HI_ERR_SUCCESS表示成功, 其它为错误码
    HI_U16 usNvId;
    HI_U8  ucMac[6];
    HI_U8  reserved[4];
} HI_UPG_NV_BW_IND_S;

typedef struct
{
    HI_U16 usNvId;
    HI_U8  ucMac[6];
    HI_U8  ucTimes;      //广播次数
    HI_U8  reserved[7];
} HI_UPG_NV_BR_REQ_S;

typedef struct
{
    HI_U32 ulRc;     // 结果码, HI_ERR_SUCCESS表示成功, 其它为错误码
    HI_U16 usCrc;
    HI_U16 usNvId;
    HI_U8  ucMac[6];
    HI_U8  reserved[4];
    HI_U16 usPayloadLen; // payload 有效长度
    HI_U8  ucPayload[0]; // payload最大存储空间
} HI_UPG_NV_BR_IND_S;
/************* 广播/单播升级 NV项, 使用说明参考  << Hi3911V100 抄控器软件接口设计说明书.docx >> END***************/

/*************串口相关配置***************/

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
    TIMER_HANDLE_T  pRxTimerPeriod;     //定时器句柄
    HI_U32          latestBaud;         //前一次波特率大小
    HI_U32          byteSpace;          //两帧报文间隔最小时间
    HI_U32          baudAdapteFlag;     //波特率自适应检测开启标志
    HI_U32          uartWriteBurst;     //写状态标志位
    HI_BOOL         isEnbale;           //波特率自适应功能开启标志
    HI_U8           reserved[3];
} HI_UART_ADAPT_TYPE_S;

typedef enum
{
    HI_IR0,  //对应 UART3
    HI_IR1,  //对应 UART2
    HI_IR_MAX,
}HI_IR_ID_TYPE_E;

/**
 * @ingroup hct_diag
 * 标准协议
 */
#define HI_MDM_PLC_PROTOCOL_STANDARD                1

/**
 * @ingroup hct_diag
 * 非标准协议
 */
#define HI_MDM_PLC_PROTOCOL_NON_STANDARD            0

//****************************************************************************
HI_END_HEADER
#endif

