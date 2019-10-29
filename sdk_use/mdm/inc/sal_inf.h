//****************************************************************************
//
//                  版权所有 (C), 2001-2011, 华为技术有限公司
//
//****************************************************************************
//  文 件 名   : sal_inf.h
//  版 本 号   : V1.0
//  作    者   : 沈汉坤/00130424
//  生成日期   : 2011年10月10日
//  功能描述   : SAL提供给Modem内部使用的接口,仅供 MAC/SAL/DRV/OS使用,APP禁止使用.
//  函数列表   :
//  修改历史   :
//  1.日    期 : 2011年10月10日
//    作    者 : 沈汉坤/00130424
//    修改内容 : 创建文件
//  2.日    期 : 2011年10月27日
//    作    者 : dubin/KF35509
//    修改内容 : 增加Cipher和GPIO接口定义
//
//****************************************************************************

#ifndef __SAL_INF_H__
#define __SAL_INF_H__
#include <hi_types.h>
HI_START_HEADER

#ifndef MAC_V200_LLT
#include <hi_sal.h>
#else
#include <hi_sal_llt.h>
#endif
#include <hi_ndm.h>
#include <hi_driver.h>

#if !defined(MAC_V200_LLT)
#include "sal_common.h"
#endif
//#include <sal_inf_m.h>
//****************************************************************************


//****************************************************************************
#if defined(PRODUCT_CFG_EQUIP_TEST_MODE)
#if defined (SAL_HAVE_OS_VX_VER)
#define  SAL_HAVE_RES_CFG_TYPES
#include <os_res_cfg_equip.h>
#endif
#else
#ifndef PRODUCT_CFG_FEATURE_HAVE_APPS
#define  SAL_HAVE_RES_CFG_TYPES
#include <os_res_cfg.h>
#undef  SAL_HAVE_RES_CFG_TYPES
#endif
#endif
//****************************************************************************


//****************************************************************************

//****************************************************************************

typedef enum
{
    HI_CHIP_MODE_NORMAL,
    HI_CHIP_MODE_USER_X,
} HI_CHIP_MODE_E;



//****************************************************************************
#if !defined(SAL_HAVE_FEATURE_UT)

HI_EAPI HI_U32 SAL_StartHRTimerPrv(HI_INOUT TIMER_HANDLE_T* phTimer, HI_PVOID pfnTimerRoutine, HI_U32 ulTimeOut);
HI_EAPI HI_U32 SAL_StopHRTimerPrv(HI_INOUT TIMER_HANDLE_T* phTimer);
#else

HI_PRVL HI_U32 SAL_StartHRTimerPrv(HI_INOUT TIMER_HANDLE_T* phTimer, HI_PVOID pfnTimerRoutine, HI_U32 ulTimeOut) { return HI_ERR_NOT_SUPPORT;}
HI_PRVL HI_U32 SAL_StopHRTimerPrv(HI_INOUT TIMER_HANDLE_T* phTimer) { return HI_ERR_NOT_SUPPORT;}
#endif



HI_EAPI HI_U32 SAL_StartUserHRTimer(HI_U16 usTimerId, HI_U32 ulTimeOut, HI_VOID_CALLBACK_F pfnTimerRoutine);


//****************************************************************************
#if defined(PRODUCT_CFG_OS_WIN)
// 上电后初始化, 仅供 UT 使用
HI_EAPI HI_VOID SAL_Init(HI_PVOID pFirstAvailableMemory);
#endif
HI_EAPI HI_VOID SAL_PowerOff(HI_U32 ulOption);
//****************************************************************************

//*****************************************************************************
// OS 定时器
//*****************************************************************************


//*****************************************************************************
// 函数名称: SAL_GetChipType
// 功能描述: 芯片类型定义
//
// 参数说明:
//  This function has no parameters.
//
// 返 回 值:
//    参见 HI_FTM_CHIP_TYPE_E 定义
//
// 调用要求: SAL模块初始化后（上电后SAL初始化完毕之后），才可以使用该接口
// 作    者: shenhankun/00130424 [2011-12-01]
//*****************************************************************************
HI_EAPI HI_FTM_CHIP_TYPE_E SAL_GetChipType(HI_VOID);

HI_EAPI HI_CHIP_MODE_E SAL_GetChipMode(HI_VOID);
HI_EAPI HI_VOID SAL_SetChipMode(HI_CHIP_MODE_E chip_mode);

//*****************************************************************************
// 函数名称: SAL_GetProductType
// 功能描述: 产品形态判断，获取当前的产品形态类型
//
// 参数说明:
//  This function has no parameters.
//
// 返 回 值:
//   参见 EN_SAL_PRODUCT_TYPE 定义
//
// 调用要求: SAL模块初始化后（上电后SAL初始化完毕之后），才可以使用该接口
// 作    者: shenhankun/00130424 [2011-12-01]
//*****************************************************************************
HI_EAPI HI_FTM_PRODUCT_TYPE_E SAL_GetProductType(HI_VOID);
//****************************************************************************


//****************************************************************************
HI_EAPI HI_CONST HI_UE_PRODUCT_VER_S*  SAL_GetProductVer(HI_VOID);
HI_EAPI HI_CONST HI_UE_SOFT_VERSION_S* SAL_GetSoftVer(HI_VOID);
HI_EAPI HI_CONST HI_PRODUCT_INFO_S* SAL_GetProductInfo(HI_VOID);
HI_EAPI HI_CONST HI_PRODUCT_INFO_S* SAL_GetSdkProductInfo(HI_VOID);
//****************************************************************************


//****************************************************************************
// GPIO Interface
//****************************************************************************

//****************************************************************************
// 函数名称: SAL_GPIOSetDir
// 功能描述: 设置GPIO输送方向
//
// 参数说明:
//   groupnr：GPIO组号，从0开始
//   Bit：gpio管脚，从0开始
//   Dir：0输入，1输出
//
// 返 回 值:
//    0表示成功，其他值失败
//
// 作    者: dkf35509
//****************************************************************************
HI_EAPI HI_U32  SAL_GpioSetDir(HI_U32  groupnr, HI_U32  bit,  HI_U32 dir);

//****************************************************************************
// 函数名称: SAL_GpioGetDir
// 功能描述: 获取Gpio输送方向
//
// 参数说明:
//   groupnr：Gpio组号，从0开始
//   Bit：gpio管脚，从0开始
//   Dir：输出参数，获取的Gpio传输方向值
//
// 返 回 值:
//    0表示成功，其他值失败
//
// 作    者: dkf35509
//****************************************************************************
HI_EAPI HI_U32  SAL_GpioGetDir(HI_U32 groupnr, HI_U32 bit, HI_U32* dir);

//****************************************************************************
// 函数名称: SAL_Gpio_Write_bit
// 功能描述: 向指定Gpio管脚写入0或者1
//
// 参数说明:
//   groupnr：Gpio组号，从0开始
//   Bit：Gpio管脚，从0开始
//   Value：0、1.
//
// 返 回 值:
//    0表示成功，其他值失败
//
// 作    者: dkf35509
//****************************************************************************
HI_EAPI HI_U32  SAL_GpioWriteBit(HI_U32  groupnr, HI_U32  bit, HI_U32 ulValue);

//****************************************************************************
// 函数名称: SAL_GpioReadBit
// 功能描述: 从指定Gpio管脚读取数据
//
// 参数说明:
//   groupnr：Gpio组号，从0开始
//   Bit：Gpio管脚，从0开始
//   Vale：输出参数，从指定Gpio管脚读取的值
//
// 返 回 值:
//    0表示成功，其他值失败
//
// 作    者: dkf35509
//****************************************************************************
HI_EAPI HI_U32  SAL_GpioReadBit(HI_U32  groupnr, HI_U32  bit, HI_U32* valu);

//****************************************************************************
// 函数名称: SAL_GpioIinit
// 功能描述: 初始化Gpio驱动
//
// 参数说明:
//   无
//
// 返 回 值:
//    0表示成功，其他值失败
//
// 作    者: dkf35509
//****************************************************************************
HI_EAPI HI_U32 SAL_GpioIinit(HI_VOID);

//****************************************************************************
// 函数名称: SAL_GpioDestroy
// 功能描述: 销毁Gpio驱动
//
// 参数说明:
//   无
//
// 返 回 值:
//    无
//
// 作    者: dkf35509
//****************************************************************************
HI_EAPI HI_VOID SAL_GpioDestroy(HI_VOID);


//****************************************************************************
// Cipher Interface
//****************************************************************************
typedef enum
{
    EN_SAL_ECB_MODE = 0,
    EN_SAL_CBC_MODE,
    EN_SAL_CFB_MODE,
    EN_SAL_OFB_MODE,
    EN_SAL_MAX_MODE
}SAL_CIPHER_WORK_MODE_ENUM;

typedef enum
{
    EN_SAL_ALG_AES = 0,
    EN_SAL_ALG_DES,
    EN_SAL_ALG_3DES
}SAL_CIPHER_ALG_ENUM;

typedef enum
{
    EN_SAL_AES_KEY_128BIT = 0,
    EN_SAL_DES_KEY_64BIT,  // key valid on des ///
    EN_SAL_DES_KEY_128BIT  // 2key valid on 3des ///
}SAL_CIPHER_KEY_LENGTH_ENUM;

typedef enum
{
    EN_SAL_WIDTH_8BIT = 0,
    EN_SAL_WIDTH_64BIT,
    EN_SAL_WIDTH_128BIT
}SAL_CIPHER_BIT_WIDTH_ENUM;


#define     SAL_CIPHER_STATUS_SUCCESS           (1 << 0)
#define     SAL_CIPHER_STATUS_MEM_LEN_ERR       (1 << 1)
#define     SAL_CIPHER_STATUS_BUS_ERR           (1 << 2)


typedef HI_S32 (*SAL_FCIPHER_CALLBACK_PFN)(HI_PVOID, HI_S32);

typedef struct
{
    SAL_CIPHER_ALG_ENUM daes_alg;
    SAL_CIPHER_WORK_MODE_ENUM daes_work_mode;
    SAL_CIPHER_BIT_WIDTH_ENUM daes_bit_width;
    SAL_CIPHER_KEY_LENGTH_ENUM daes_key_length;
}SAL_CIPHER_CTRL_STRU;

typedef struct
{
    HI_U32 cipher_key1[2];
    HI_U32 cipher_key2[2];
    HI_U32 ivin[2];
}SAL_CIPHER_DES_KEYIVIN_STRU;

typedef struct
{
    HI_U32 cipher_key[4];
    HI_U32 ivin[4];
}SAL_CIPHER_AES_KEYIVIN_STRU;

#define SAL_CIPHER_DEV_BUSY     -0x1000
#define SAL_CIPHER_DEV_IDLE     -0x1001
#define SAL_CIPHER_DEV_CLOSE    -0x1002
#define SAL_CIPHER_INVAL        -0x1003
#define SAL_CIPHER_MEM_ALLOC    -0x1004

//
// Cipher usage:
// 1.   You should call HI_CIPHER_Open to init device.
// 2.   You should call HI_CIPHER_GetHandle to
//      alloc an task handle to process business
// 3.   You should call HI_CIPHER_GET_DEV to (try to) hold the device.
// 4.   Config device as you want by calling HI_CIPHER_ConfigAES
//      or HI_CIPHER_ConfigDES
// 5.   Do you job such as call HI_CIPHER_Encrypt or HI_CIPHER_Decrypt.
// 6.   After your job is done, you should release device
//      by calling HI_CIPHER_PUT_DEV.
// 7.   Then if you will not use cipher again, release handle and close device.
//      Just call HI_CIPHER_ReleaseHandle, and HI_CIPHER_Close.
//

// Open device.
HI_EAPI HI_U32 SAL_CipherOpen (HI_VOID);

// Close it
HI_EAPI HI_U32 SAL_CipherClose (HI_VOID);

// Get an task handle to do you business.
HI_EAPI HI_U32 SAL_CipherGetHandle (HI_VOID **handle);

// Release the task handle alloced for you before.
HI_EAPI HI_U32 SAL_CipherReleaseHandle (HI_VOID *handle);

// Hold the device.
// -    If @try set to !0, and device is already hold by other task,
//  this function will return immediately.
// -    If @try set to 0, this call will suspend until other task
//  release device.
//
HI_EAPI HI_U32 SAL_CipherGetDev(HI_PVOID handle, HI_S32 slTry);

// Release device.
HI_EAPI HI_U32 SAL_CipherPutDev(HI_PVOID handle);

// Config AES.
HI_EAPI HI_U32 SAL_CipherConfigAES(SAL_CIPHER_CTRL_STRU* daes_ctrl, SAL_CIPHER_AES_KEYIVIN_STRU * aes_key_ivin, HI_PVOID handle);

// Config DES.
HI_EAPI HI_U32 SAL_CipherConfigDES(SAL_CIPHER_CTRL_STRU* daes_ctrl, SAL_CIPHER_DES_KEYIVIN_STRU * des_key_ivin, HI_PVOID handle);

//
// MAX encrypt & decrypt data size 1M - 4  BYTE.
// Address and length are advised align at  4 byte bound.
// Cipher will not check this but user should insure it.
//
#define SAL_CIPHER_MAX_DATA_SIZE    (((1 << 20) -1) & (~0x3))

//
// If callback is not set, you can call HI_CIPHER_Is_Busy
// to decide whether your job is done.
//
HI_EAPI HI_U32 SAL_CipherEncrypt(HI_U32 ulSrc, HI_U32 ulDest,
                                 HI_U32 ulByteLength, HI_PVOID pHandle, SAL_FCIPHER_CALLBACK_PFN pfnCallback,
                                 HI_PVOID pArg);

HI_EAPI HI_U32 SAL_CipherDecrypt(HI_U32 u32Src, HI_U32 u32Dest,
                                 HI_U32 u32ByteLength, HI_PVOID pHandle, SAL_FCIPHER_CALLBACK_PFN pfnCallback,
                                 HI_PVOID pArg);

//
// If return value != HI_ERR_CIPHER_DEV_BUSY,
// then last jobs's result is stored to @status.
//
HI_EAPI HI_U32 SAL_CipherIsBusy(HI_PVOID handle, HI_S32* status);

HI_EAPI HI_U32 SAL_InitSpiFpga(HI_VOID);

#ifdef PRODUCT_CFG_OS_NU
HI_EAPI HI_U32 SAL_SetEthCfg(HI_IN char * ucIfName,HI_IN HI_PVOID pCmdParam ,HI_U16 usCmdParamSize);
HI_EAPI HI_U32 SAL_GetEthCfg(HI_IN char * ucIfName,HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize);
#endif

typedef struct
{
    HI_U16 usPortNum; // 端口号
    HI_U8  bPhyConnect; // HI_TRUE: 通道连接，FALSE_T: 通道断开(不可用)
    HI_U8  reserved;
    HI_U32 pad1;   // 保留
    HI_U32 pad2; //保留
    HI_U32 ulFlashTotalSize;
} HI_DMS_DIAG_FLASH_PROT_INFO_S;

// Get DIAG Flash storage Info(address and size etc.)
HI_EAPI HI_U32 DMS_GetDiagFlashStorageInfo(HI_DMS_DIAG_FLASH_PROT_INFO_S* pstFlashPort, HI_U32 ulOption);


typedef struct
{
    /* BEGIN: Added by likunhe/00194117, 2014/1/3   PN:DTS2013122607967*/
    HI_U16 usTransId;
    HI_U16 usRsd;
    /* END:   Added by likunhe/00194117, 2014/1/3 */
    HI_U16 usUserData;
    HI_U16 usPayloadSize;
    HI_PVOID pPayload;
} HI_ND_REMOTE_PACKET_S;

typedef enum
{
    HI_MSG_ND_REMOTE_PACKET = 0x400,
/* BEGIN ADDED by db 2012-09-20  问题单号: DTS2012092008323 */
    HI_MSG_ND_REMOTE_FRAGMENT_PACKET = 0x401
/* END ADDED by db 2012-09-20   */

} HI_MSG_ND_E;
//****************************************************************************


//****************************************************************************
// Flash API
//****************************************************************************
/* BEGIN: Deleted by b00208046, 2013/7/3   PN:DTS2013062400945  */
/* END: Deleted by b00208046, 2013/7/3 */
//****************************************************************************


//****************************************************************************
// SAL DFX Interface.
//****************************************************************************
#include <dfx_sal.h>


//*****************************************************************************
// 函数名称: HI_DIAG_ReportDRemotePacket
// 功能描述: 上报直接远程HSO帧到HSO
//
// 参数说明:
//   ulId [in] 包ID
//   pucPacket [in] 数据包的buffer地址, 该函数不会释放该指针,如果不需要可以值为HI_NULL
//   usPacketSize [in] 数据包大小, 如果不需要可以值为0
//   pstRemoteFrm [in] 远程帧头, usDULink 和 usUserData 可不填写. 仅 ucInstanceId=HI_DIAG_CMD_INSTANCE_DREMOTE 有效
//   bSync [in] HI_TRUE:不通过OS队列发送包, 仅适用于HI_DIAG_RegiestInputCommand注册的处理接口中;
//              HI_FASE:通过OS队列发送包
//
// 返 回 值:
//    HI_ERR_SUCCESS: 成功
//    其它值表示错误码.
//
// 调用要求: 1. HSO通道可用; 2) 缓存足够;
//           3) 没有配置开关, 能一直能上报, 如果需要受开关限制,则需调用HI_DIAG_ReportCommand
//           4) HI_DIAG_SendUserPacket仅支持发送到OS队列
// 调用举例:
//  MY_PACKET_STRU stPacket = {0};
//  HI_DIAG_SendUserPacket(0x1000, &stPacket, sizeof(MY_PACKET_STRU));
// 作    者: shenhankun/00130424 [2012-01-04]
//*****************************************************************************
#if !defined(PRODUCT_CFG_CORE_TYPE_BOOTLOADER)
HI_EAPI HI_U32 SAL_ReportDRemotePacket(HI_U32 ulId, HI_PBYTE pucPacket, HI_U16 usPacketSize, HI_ND_REMOTE_FRAME_HEAD_S* pstRemoteFrm, HI_BOOL bSync);
#else
#define SAL_ReportDRemotePacket
#endif

HI_EAPI HI_U32 SAL_ReportLocalPacket(HI_U32 ulId, HI_PBYTE pucPacket, HI_U16 usPacketSize, HI_BOOL bSync);
#if !defined(PRODUCT_CFG_CORE_TYPE_BOOTLOADER)
HI_EAPI HI_U32 SAL_ReportDRemoteFrame(HI_U32 ulId, HI_ND_REMOTE_FRAME_HEAD_S* pstRemoteFrm);
#else
#define SAL_ReportDRemoteFrame
#endif

//*****************************************************************************
// 函数名称: HI_DIAG_GetDRemoteFrameHeader
// 功能描述: 获取直接远程命令的HSO帧头
//
// 参数说明:
//   pCmdParam [in] HSO包
//   usCmdParamSize [in] HSO包大小
//
// 返 回 值:
//    HI_ERR_SUCCESS : 处理成功, DIAG将返回给HSO,错误码为OK
//    其它为错误码
//
// 调用要求: 1. 不支持多任务调用.
//           2. 仅能在 HI_DIAG_RegiestCommand 注册的命令处理入口中调用, 其它地方调用可能导致系统异常.
//*****************************************************************************
#if !defined(PRODUCT_CFG_CORE_TYPE_BOOTLOADER)
HI_EAPI HI_ND_REMOTE_FRAME_HEAD_S* SAL_GetDRemoteFrameHeader(HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize);
#else
#define SAL_GetDRemoteFrameHeader
#endif
#define HI_DIAG_GetDRemoteFrameHeader SAL_GetDRemoteFrameHeader

#define  HI_SYS_SocketInit()   {}

#if !defined(PRODUCT_CFG_CORE_TYPE_BOOTLOADER) && defined(PRODUCT_CFG_NVM_EXT_SUPPORT)
HI_EAPI HI_U32 SAL_NVM_VersionUpdate(HI_IN HI_PVOID pNvFileData, HI_U32 ulNvFileDataSize);
HI_EAPI HI_U32 SAL_NVM_VersionBack(HI_VOID);
#endif

HI_EAPI HI_U32 diag_SysStatObjQuery(HI_U16 usId, HI_U32* HI_OUT pulObj, HI_U16* HI_OUT pusObjLen, HI_U16* HI_OUT pusObjCnt, HI_BOOL* HI_OUT pbCusom);

HI_EAPI HI_U32 DIAG_ReportStatObj(HI_U16 usObjId, HI_PBYTE pucObj, HI_U16 usObjSize, HI_U16 usObjCnt, HI_U8 ucInstanceId, HI_BOOL bCusom, HI_BOOL bSync);

#if defined(PRODUCT_CFG_NVM_FACTORY_REPLACE)
HI_EAPI HI_U32 HI_NVM_Replace(HI_U16 usId, HI_PVOID pItemData, HI_U16 usItemDataLen);
#endif

HI_EAPI HI_U32 HI_DIAG_DispatchCommand(HI_U16 usId, HI_PVOID pPayload, HI_U16 usPayloadLen, HI_U8 ucInstanceId);

HI_EAPI HI_U32 HI_SYS_DbgUartWrite(HI_IN HI_U8* pucBuffer, HI_U16 usNumberOfBytesToWrite);

HI_EAPI HI_U32 HI_DIAG_ProcessReqPacket(HI_U16 usPortNum, HI_PBYTE pucData, HI_U16 usDataLen);
HI_EAPI HI_U32 HI_SYS_InitWatchdogCtrl(HI_BOOL bForce); // 初始化过程中复位控制, DTS2012102504891, ADDED by s00130424: 复位控制接口归一化，方便定位问题
HI_EAPI HI_U32 HI_DIAG_GetDbgSyncInfo(HI_OUT HI_U8 aucDbgSync[HI_DIAG_DBG_SYNC_SIZE_MAX]);
HI_EAPI HI_U32 HI_DIAG_SetDbgSyncInfo(HI_U8 aucDbgSync[HI_DIAG_DBG_SYNC_SIZE_MAX]);
HI_EAPI HI_VOID SAL_SetSysCause(HI_U8 ucSysCause);

//*****************************************************************************


//*****************************************************************************
// Fatal Error ID:
// Called by exception interface inside only.
//*****************************************************************************
#define HI_SYS_EID_UFAIL          0xF0  // user fatal error
#define HI_SYS_EID_KFAIL          0xF1  // kernel fatal error
#define HI_SYS_EID_RST            0xF2  // reset management
#define HI_SYS_EID_REBOOT         0xF3  // reboot call
#define HI_SYS_EID_STACK          0xF4  // stack overflow/underflow
/* BEGIN: Added by chenghuanhuan , 2013/06/22   PN:DTS2013061803551*/
#define HI_SYS_EID_WDG            0xF5  // watchdog reboot
/* END: Added by chenghuanhuan , 2013/06/22   PN:DTS2013061803551*/
//*****************************************************************************

//*****************************************************************************
#if defined(PRODUCT_CFG_HAVE_FEATURE_SYS_ERR_INFO) && defined(SAL_SUPPORT_SYS_ERR_INFO_API)
HI_EXTERN  HI_U8 HI_VOLATILE g_aucSysErrVal[];
//*****************************************************************************

//*****************************************************************************
#if defined(PRODUCT_CFG_OS_NU)
HI_EXTERN  HI_VOID * HI_VOLATILE TCD_Current_Thread;
HI_EXTERN  signed int ESAL_GE_ISR_Executing;
HI_EXTERN  signed int ESAL_GE_ISR_Executing_Counter;
#define SYS_CUR_TSK()    TCD_Current_Thread
#define SYS_ISR_E()      ESAL_GE_ISR_Executing
#define SYS_ISR_E_C()    ESAL_GE_ISR_Executing_Counter
#elif defined(PRODUCT_CFG_OS_VX)
#define SYS_CUR_TSK()    1
#define SYS_ISR_E()      0
#define SYS_ISR_E_C()    0
#endif
//*****************************************************************************

//*****************************************************************************
HI_PRVL char *_strcpy(char *s1, const char *s2)
{
    register char *p = s1;
    while((s1 - p < 15) && ((*s1++ = *s2++) != 0));
    *s1 = 0;
    return(p);
}

HI_PRVL char *_strncpy(char *s1,const char *s2, HI_S32 n)
{
    char *p = s1;

    while (n > 0)
    {
        --n;

        *s1 = *s2;

        if(!(*s1))
            break;

        s1++;
        s2++;
    }

    while (n-- > 0)   *s1++ = 0;

    return(p);
}
//*****************************************************************************


//*****************************************************************************
#if defined(PRODUCT_CFG_AUTO_RESET_SYSTEM_TIMEOUT) && defined(PRODUCT_CFG_SYS_ERR_WD_RST)
#define SYS_RST_WD_V0()  HI_REG_WRITE32(0x101E003C, 0x0000000F)
#else
#define SYS_RST_WD_V0()
#endif

#define SYS_ERR_RST_PROC_PHASE0()    HI_REG_READ32(0x101E003C, SAL_SYS_ERR_PTR()->wdctrl); SYS_RST_WD_V0()
#define SAL_SAVE_EXT_INFO()          do { SYS_ERR_RST_PROC_PHASE0(); }while(0)
//*****************************************************************************


//*****************************************************************************
#define _SET_EXCEPTION_DBG(_offset, _v)         SAL_SYS_ERR_PTR()->dbg_data[_offset] = _v
#define _SAVE_SDM_HRD2FLASH()                   HI_SYS_SaveUserData(HI_USERDATA_ID_CRASH_HRD, 0, 1, SAL_SDM_VAL_SIZE(), (HI_PVOID)SAL_SYS_ERR_PTR())
#ifdef PRODUCT_CFG_OS_NU
#define _SAVE_SDM_STK()                         SAL_SYS_ERR_PTR()->tstksize = (HI_U32)_SAL_CUR_THD->tc_stack_size; SAL_SYS_ERR_PTR()->tstk[0] = (HI_U32)(_SAL_CUR_THD->tc_stack_start); SAL_SYS_ERR_PTR()->tstk[1] = (HI_U32)(_SAL_CUR_THD->tc_stack_end)
#else
#define _SAVE_SDM_STK()                         SAL_SYS_ERR_PTR()->tstksize = (HI_U32)pstSysExpInfo->task_stk_size; SAL_SYS_ERR_PTR()->tstk[0] = pstSysExpInfo->atcstk[0]; SAL_SYS_ERR_PTR()->tstk[1] = pstSysExpInfo->atcstk[1]
#endif
#define _SAVE_SDM_FATAL_INFO(_txt,_func,_file)   if(_txt) _strncpy((char *)SAL_SYS_ERR_PTR()->err_exp, (const char*)_txt, sizeof(SAL_SYS_ERR_PTR()->err_exp));\
                                                 if(_func) _strncpy((char *)SAL_SYS_ERR_PTR()->err_func, (const char*)_func, sizeof(SAL_SYS_ERR_PTR()->err_func));\
                                                 if(_file) _strncpy((char *)SAL_SYS_ERR_PTR()->err_file, (const char*)SAL_GetFileShortName((HI_PCSTR)_file), sizeof(SAL_SYS_ERR_PTR()->err_file))

#define _SAVE_SDM_FATAL4_INFO(_txt,_func)        if(_txt) _strncpy((char *)SAL_SYS_ERR_PTR()->err_exp, (const char*)_txt, sizeof(SAL_SYS_ERR_PTR()->err_exp));\
                                                 if(_func) _strncpy((char *)SAL_SYS_ERR_PTR()->err_func, (const char*)_func, sizeof(SAL_SYS_ERR_PTR()->err_func));

#define _SAVE_USER_DAT(_u1,_u2,_u3,_u4)        SAL_SYS_ERR_PTR()->val[0] = (HI_U32)_u1;\
                                               SAL_SYS_ERR_PTR()->val[1] = (HI_U32)_u2;\
                                               SAL_SYS_ERR_PTR()->val[2] = (HI_U32)_u3;\
                                               SAL_SYS_ERR_PTR()->val[3] = (HI_U32)_u4;

#ifdef PRODUCT_CFG_OS_NU
#define _SAL_CUR_THD               ((TC_TCB*)SYS_CUR_TSK())

#define _SAL_CP_TASK_INFO()       SAL_SAVE_EXT_INFO(); \
                                  if(SYS_CUR_TSK())\
                                  {SAL_SYS_ERR_PTR()->tstatus = (HI_U32)_SAL_CUR_THD->tc_status;\
                                   SAL_SYS_ERR_PTR()->tid     = (HI_U32)_SAL_CUR_THD->tc_id;\
                                   SAL_SYS_ERR_PTR()->tcsc    = (HI_U32)_SAL_CUR_THD->tc_scheduled;\
                                   SAL_SYS_ERR_PTR()->tcsp    = (HI_U32)_SAL_CUR_THD->tc_stack_pointer;\
                                   _SET_EXCEPTION_DBG(HI_SYS_SET_EXCEPTION_ISR, HI_FALSE);\
                                   _strcpy((char *)SAL_SYS_ERR_PTR()->tname, (const char *)(_SAL_CUR_THD->tc_name));\
                                   _SAVE_SDM_STK();\
                                   }\
                                   else\
                                   {\
                                     _SET_EXCEPTION_DBG(HI_SYS_SET_EXCEPTION_ISR,     HI_TRUE);\
                                     _SET_EXCEPTION_DBG(HI_SYS_SET_EXCEPTION_ISR_E,   SYS_ISR_E());\
                                     _SET_EXCEPTION_DBG(HI_SYS_SET_EXCEPTION_ISR_CNT, SYS_ISR_E_C());\
                                   }\
                                   HI_SYS_SystemError();

#elif defined(PRODUCT_CFG_OS_VX)
#define   DO_TASK_INFO2()                  do{\
                                              HI_S32 mm = 0;\
                                              for(mm = 0; mm< HI_SYS_TT_FUNC_MAX_NUM; mm++)\
                                              {\
                                                 SAL_SYS_ERR_PTR()->ttfa[mm] = pstSysExpInfo->ttfa[mm];\
                                              }\
                                              for(mm = 0; mm< 15; mm++)\
                                              {\
                                                 SAL_SYS_ERR_PTR()->r[mm] = pstSysExpInfo->r[mm];\
                                              }\
                                           }while(0)

#define   DO_TASK_INFO()                  do{\
                                              HI_SYS_EXCEPTION_INFO_S* pstSysExpInfo = HI_NULL;\
                                              HI_PVOID pTask = 0;\
                                              SAL_OSGetCurrentTaskPointer(pTask);\
                                              if(pTask)\
                                              {\
                                                pstSysExpInfo = HI_SYS_GetExceptionInfo(pTask);\
                                                SAL_OSGetTaskInfo(pTask, (HI_CHAR *)SAL_SYS_ERR_PTR()->tname, &(SAL_SYS_ERR_PTR()->tstatus), 0, 0, 0, 0, 0, &(SAL_SYS_ERR_PTR()->tstksize), 0);\
                                              }\
                                              if(pstSysExpInfo)\
                                              {\
                                                  SAL_SYS_ERR_PTR()->tstatus = (HI_U32)pstSysExpInfo->task_status;\
                                                  SAL_SYS_ERR_PTR()->cpsr    = (HI_U32)pstSysExpInfo->cpsr;\
                                                  DO_TASK_INFO2();\
                                                  _SAVE_SDM_STK();\
                                              }\
                                          }while(0)

#define _SAL_CP_TASK_INFO()      SAL_SAVE_EXT_INFO(); \
                                  if(SYS_CUR_TSK())\
                                  {\
                                   SAL_SYS_ERR_PTR()->tid     = (HI_U32)0;\
                                   SAL_SYS_ERR_PTR()->tcsc    = (HI_U32)0;\
                                   SAL_SYS_ERR_PTR()->tcsp    = (HI_U32)0;\
                                   _SET_EXCEPTION_DBG(HI_SYS_SET_EXCEPTION_ISR, HI_FALSE);\
                                   DO_TASK_INFO();\
                                   }\
                                   else\
                                   {\
                                     _SET_EXCEPTION_DBG(HI_SYS_SET_EXCEPTION_ISR,     HI_TRUE);\
                                     _SET_EXCEPTION_DBG(HI_SYS_SET_EXCEPTION_ISR_E,   SYS_ISR_E());\
                                     _SET_EXCEPTION_DBG(HI_SYS_SET_EXCEPTION_ISR_CNT, SYS_ISR_E_C());\
                                   }\
                                   HI_SYS_SystemError();

#define _SAL_CP_TASK_INFO_VX()    SAL_SAVE_EXT_INFO(); \
                                  if(SYS_CUR_TSK())\
                                  {SAL_SYS_ERR_PTR()->tstatus = (HI_U32)pstSysExpInfo->task_status;\
                                   SAL_SYS_ERR_PTR()->cpsr    = (HI_U32)pstSysExpInfo->cpsr;\
                                   SAL_SYS_ERR_PTR()->tid     = (HI_U32)0;\
                                   SAL_SYS_ERR_PTR()->tcsc    = (HI_U32)0;\
                                   SAL_SYS_ERR_PTR()->tcsp    = (HI_U32)0;\
                                   _SET_EXCEPTION_DBG(HI_SYS_SET_EXCEPTION_ISR, HI_FALSE);\
                                   _strncpy((char *)SAL_SYS_ERR_PTR()->tname, (const char *)(pstSysExpInfo->task_name), 16);\
                                   _SAVE_SDM_STK();\
                                   DO_TASK_INFO2();\
                                   }\
                                   else\
                                   {\
                                     _SET_EXCEPTION_DBG(HI_SYS_SET_EXCEPTION_ISR,     HI_TRUE);\
                                     _SET_EXCEPTION_DBG(HI_SYS_SET_EXCEPTION_ISR_E,   SYS_ISR_E());\
                                     _SET_EXCEPTION_DBG(HI_SYS_SET_EXCEPTION_ISR_CNT, SYS_ISR_E_C());\
                                   }\
                                   HI_SYS_SystemError();
#else
HI_EXTERN HI_CHAR g_ucCrashData[2][2048];
#define _SAL_CP_TASK_INFO() SAL_SYS_ERR_PTR()->tstk[0] = g_ucCrashData[0]; SAL_SYS_ERR_PTR()->tstk[1] = g_ucCrashData[1];\
                           _strcpy((char *)SAL_SYS_ERR_PTR()->tname, (const char *)("CrashTestTask"));\
                            HI_SYS_SystemError();
#endif

#ifdef PRODUCT_CFG_PLATFORM_HI3911
#if defined(PRODUCT_CFG_OS_VX)
#define _SAL_SP()      (HI_U32)SAL_SYS_GET_SP()
#define _SAL_ESP(x)    x
#else
#define _SAL_SP()      (HI_U32)HI_RTE_SP_READ()
#define _SAL_ESP(x)    x
#endif
#else
HI_EXTERN HI_U8 g_ucCrashSp[2048];
HI_EXTERN HI_U8 g_ucCrashEsp[2048];
#define _SAL_SP()      (HI_U32)g_ucCrashSp
#define _SAL_ESP(x)    (HI_U32)g_ucCrashEsp
#endif


//*****************************************************************************


//*****************************************************************************
#if defined(PRODUCT_CFG_OS_NU) || defined(PRODUCT_CFG_OS_WIN)



#define SAL_FAIL_ASSERT(_mid, _eno, _txt, _file, _ln, _func, _u1, _u2) do {\
                                           SAL_SYS_ERR_PTR()->oerr[0]  = 0;\
                                           SAL_SYS_ERR_PTR()->eid      = HI_SYS_EID_UFAIL;\
                                           SAL_SYS_ERR_PTR()->fid[0]   = __return_address();\
                                           SAL_SYS_ERR_PTR()->fid[1]   = 0;\
                                           SAL_SYS_ERR_PTR()->evn      = 0;\
                                           SAL_SYS_ERR_PTR()->esp      = 0;\
                                           SAL_SYS_ERR_PTR()->vn       = 0;\
                                           SAL_SYS_ERR_PTR()->type     = (HI_S32)EN_SAL_ERROR_TYPE_FATAL;\
                                           SAL_SYS_ERR_PTR()->oerr[1]  = (HI_U32)_mid;\
                                           SAL_SYS_ERR_PTR()->oerr[2]  = (HI_U32)_eno;\
                                           _SAVE_SDM_FATAL_INFO(_txt, _func,_file);\
                                           SAL_SYS_ERR_PTR()->err_ln    = (HI_U32)_ln;\
                                           _SAVE_USER_DAT(_u1,_u2,0,0);\
                                           SAL_SYS_ERR_PTR()->sp        = _SAL_SP();\
                                           _SAL_CP_TASK_INFO();\
                                       }while(0)

#define SAL_SDM_SET_FAIL4(_mid, _eno, _txt, _ln, _func, _u1, _u2, _u3, _u4)  do {\
                                           SAL_SYS_ERR_PTR()->oerr[0] = 0;\
                                           SAL_SYS_ERR_PTR()->eid     = HI_SYS_EID_UFAIL;\
                                           SAL_SYS_ERR_PTR()->fid[0]  = __return_address();\
                                           SAL_SYS_ERR_PTR()->fid[1]  = 0;\
                                           SAL_SYS_ERR_PTR()->evn     = 0;\
                                           SAL_SYS_ERR_PTR()->esp     = 0;\
                                           SAL_SYS_ERR_PTR()->vn      = 0;\
                                           SAL_SYS_ERR_PTR()->type    = (HI_S32)EN_SAL_ERROR_TYPE_FATAL;\
                                           SAL_SYS_ERR_PTR()->oerr[1]  = (HI_U32)_mid;\
                                           SAL_SYS_ERR_PTR()->oerr[2]  = (HI_U32)_eno;\
                                           _SAVE_SDM_FATAL4_INFO(_txt,_func);\
                                           SAL_SYS_ERR_PTR()->err_ln = (HI_U32)_ln;\
                                           _SAVE_USER_DAT(_u1,_u2,_u3,_u4);\
                                           SAL_SYS_ERR_PTR()->sp   = _SAL_SP();\
                                           _SAL_CP_TASK_INFO();\
                                       }while(0)


#define SAL_SET_STACK_ASSERT(_type, _mid, _eno, _txt, _file, _ln, _func, _esp) do {\
                                           SAL_SYS_ERR_PTR()->oerr[0]  = 0;\
                                           SAL_SYS_ERR_PTR()->eid      = HI_SYS_EID_STACK;\
                                           SAL_SYS_ERR_PTR()->fid[0]   = 0;\
                                           SAL_SYS_ERR_PTR()->fid[1]   = 0;\
                                           SAL_SYS_ERR_PTR()->evn      = 0;\
                                           SAL_SYS_ERR_PTR()->esp      = (HI_U32)_esp;\
                                           SAL_SYS_ERR_PTR()->vn       = 0;\
                                           SAL_SYS_ERR_PTR()->type     = (HI_S32)_type;\
                                           SAL_SYS_ERR_PTR()->oerr[1]  = (HI_U32)_mid;\
                                           SAL_SYS_ERR_PTR()->oerr[2]  = (HI_U32)_eno;\
                                           _SAVE_SDM_FATAL_INFO(_txt, _func, _file);\
                                           SAL_SYS_ERR_PTR()->err_ln   = (HI_U32)_ln;\
                                           SAL_SYS_ERR_PTR()->sp       = _SAL_SP();\
                                           _SAL_CP_TASK_INFO();\
                                       }while(0)





#endif


// OS error
#ifdef PRODUCT_CFG_OS_VX
#define SAL_FAIL_ASSERT(_mid, _eno, _txt, _file, _ln, _func, _u1, _u2) do {\
                                           SAL_SYS_ERR_PTR()->oerr[0]  = 0;\
                                           SAL_SYS_ERR_PTR()->eid      = HI_SYS_EID_UFAIL;\
                                           SAL_SYS_ERR_PTR()->fid[0]   = 0;\
                                           SAL_SYS_ERR_PTR()->fid[1]   = 0;\
                                           SAL_SYS_ERR_PTR()->evn      = 0;\
                                           SAL_SYS_ERR_PTR()->esp      = 0;\
                                           SAL_SYS_ERR_PTR()->vn       = 0;\
                                           SAL_SYS_ERR_PTR()->type     = (HI_S32)EN_SAL_ERROR_TYPE_FATAL;\
                                           SAL_SYS_ERR_PTR()->oerr[1]  = (HI_U32)_mid;\
                                           SAL_SYS_ERR_PTR()->oerr[2]  = (HI_U32)_eno;\
                                           _SAVE_SDM_FATAL_INFO(_txt, _func, _file);\
                                           SAL_SYS_ERR_PTR()->err_ln   = (HI_U32)_ln;\
                                           _SAVE_USER_DAT(_u1,_u2,0,0);\
                                           SAL_SYS_ERR_PTR()->sp        = _SAL_SP();\
                                           _SAL_CP_TASK_INFO();\
                                        }while(0)

#define SAL_SDM_SET_FAIL4(_mid, _eno, _txt, _ln, _func, _u1, _u2, _u3, _u4) do {\
                                           SAL_SYS_ERR_PTR()->oerr[0] = 0;\
                                           SAL_SYS_ERR_PTR()->eid     = HI_SYS_EID_UFAIL;\
                                           SAL_SYS_ERR_PTR()->fid[0]  = 0;\
                                           SAL_SYS_ERR_PTR()->fid[1]  = 0;\
                                           SAL_SYS_ERR_PTR()->evn     = 0;\
                                           SAL_SYS_ERR_PTR()->esp     = 0;\
                                           SAL_SYS_ERR_PTR()->vn      = 0;\
                                           SAL_SYS_ERR_PTR()->type    = (HI_S32)EN_SAL_ERROR_TYPE_FATAL;\
                                           SAL_SYS_ERR_PTR()->oerr[1] = (HI_U32)_mid;\
                                           SAL_SYS_ERR_PTR()->oerr[2] = (HI_U32)_eno;\
                                           _SAVE_SDM_FATAL4_INFO(_txt, _func);\
                                           _SAVE_USER_DAT(_u1,_u2,_u3,_u4);\
                                           _SAL_CP_TASK_INFO();\
                                       }while(0)




#endif

#define SAL_SET_EXCEPTION_CALL_INC(_offset)      do { SAL_SYS_ERR_PTR()->dbg_data[_offset] ++;   } while(0)
#define SAL_SET_EXCEPTION_CALL_DEC(_offset)      do { SAL_SYS_ERR_PTR()->dbg_data[_offset] --;   } while(0)
#define SAL_EXCEPTION_CALL_ZERO(_offset)         do { SAL_SYS_ERR_PTR()->dbg_data[_offset] = 0;  } while(0)
#define SAL_SET_EXCEPTION_DBG(_offset, _v)       do { SAL_SYS_ERR_PTR()->dbg_data[_offset] = _v; } while(0)
#define SAL_RESET_EXCEPTION_CALL(_offset)        do { SAL_SYS_ERR_PTR()->dbg_data[_offset] = 0;  } while(0)
#define SAL_SET_EXCEPTION_INC(_offset)           do { SAL_SYS_ERR_PTR()->dbg_data[_offset] ++;   } while(0)
#define SAL_SET_DBG_DATA(_offset, _v)            do { SAL_SYS_ERR_PTR()->dbg_data[_offset] = _v;   } while(0)
#else
#define SAL_FAIL_ASSERT(_mid, _eno, _txt, _file, _ln, _func, _usrdata1, _userdata2)
#define SAL_SET_STACK_ASSERT(_type, _mid, _eno, _txt, _file, _no, _func)              do {} while(0)
#define SAL_SET_EXCEPTION_CALL_INC(_offset)
#define SAL_RESET_EXCEPTION_CALL(_offset)
#define SAL_SET_EXCEPTION_DBG(_offset, _v)
#define SAL_SET_EXCEPTION_INC(_offset)
#define SAL_SDM_SET_FAIL4(_mid, _eno, _txt, _ln, _func, _u1, _u2, _u3, _u4)
#define SAL_EXCEPTION_CALL_ZERO(_offset)
#define SAL_SET_EXCEPTION_CALL_DEC(_offset)
#define SAL_SET_DBG_DATA(_offset, _v)
#endif

//*****************************************************************************


//*****************************************************************************
HI_PRVL HI_PCSTR SAL_GetFileShortName(HI_PCSTR pszFile)
{
    HI_PCSTR pszShortFile = "";

    if (HI_NULL == pszFile)
    {
        return pszShortFile;
    }

    pszShortFile = pszFile;

    while(*pszFile++ != '\0')
    {
        if(*pszFile == '/' || *pszFile == '\\')
        {
            pszShortFile = ++pszFile;
        }
    }

    return pszShortFile;
}
//*****************************************************************************

//*****************************************************************************
#define HI_SYS_SET_EXCEPTION_USER_DIAG_RX_ACK_PROC    0
#define HI_SYS_SET_EXCEPTION_USER_DIAG_RX_USER_PROC   1
#define HI_SYS_SET_EXCEPTION_DIAG_RX_PROC             2
#define HI_SYS_SET_EXCEPTION_USER_DIAG_ID             3

#define HI_SYS_SET_EXCEPTION_ISR                      4
#define HI_SYS_SET_EXCEPTION_ISR_E                    5
#define HI_SYS_SET_EXCEPTION_ISR_CNT                  6

#define HI_SYS_SET_EXCEPTION_DIAG_RX_WE               10
#define HI_SYS_SET_PRODUCT_ID_CHIP                    11
#define HI_SYS_SET_PRODUCT_ID_PROUDCT                 12

#define HI_SYS_SET_EXCEPTION_MAX                      20
//*****************************************************************************


//*****************************************************************************
#if !defined(PRODUCT_CFG_EQUIP_TEST_MODE)
#if !defined(SAL_EXCLUDE_RES_CFG_INF_HRD)
#include <os_res_cfg.h>
#endif
#endif
//*****************************************************************************



//*****************************************************************************
// 函数名称: HI_DIAG_RegisterDefaultNdmCommand
// 功能描述: 注册抄控器通道缺省处理命令处理入口
//
// 参数说明:
//   pfnProcess [in] 命令处理入口
//
// 返 回 值:
//    HI_ERR_SUCCESS : 处理成功, DIAG将返回给HSO,错误码为OK
//    其它为错误码
//
// 调用要求:
// 作    者: shenhankun/00130424 [2011-11-14]
//*****************************************************************************
HI_PRVL HI_U32 HI_DIAG_RegisterDefaultNdmCommand(HI_CONST HI_DIAG_CMD_F pfnProcess)
{
    if(!HI_IS_VALID_CODE_ADDR((uintptr_t)pfnProcess))
    {
        return HI_ERR_INVALID_PARAMETER;
    }
    else
	{
		#if !defined(MAC_V200_LLT)
        return HI_DIAG_RegisterDRemoteCommandPrv(pfnProcess);
		#elif defined(MAC_V200_LLT)
		return HI_ERR_SUCCESS;
		#endif
	}
}

//*****************************************************************************
HI_EXTERN HI_U32 SAL_GetNdmIdStart(HI_VOID);
HI_EXTERN HI_U32 SAL_GetNdmIdEnd(HI_VOID);
//*****************************************************************************


//*****************************************************************************
#define SAL_CHECK_MAC_INT_ENABLE() do{ \
    if(!HI_SYS_IsSysInitComplete() || HI_SYS_IsDiagMode())\
    {\
        return ;\
    }\
}while(0)
//*****************************************************************************


//*****************************************************************************
#if defined(PRODUCT_CFG_SYS_FULL_VER_INFO_QRY)
HI_U32 DIAG_GetVerInfo(HI_OUT DIAG_CMD_SOFT_VER_STRU* pstAck);
#else
#define DIAG_GetVerInfo(pstAck)
#endif

#if defined(PRODUCT_CFG_SYS_QRY_STAT)
HI_PVOID DIAG_GetSysInfo(HI_OUT HI_U32* pulSysInfoSize);
#else
#define DIAG_GetSysInfo(pulSysInfoSize)   0
#endif
//*****************************************************************************

#if !defined(PRODUCT_CFG_CORE_TYPE_BOOTLOADER)
// 上交HSO时的分包参数
typedef struct
{
    HI_U16 usCmdId;
    HI_U16 usPacketSize; // 分包数据大小

    HI_U16 usFragmentID;  // 分包SN号，每个分包依次从0递增
    /* BEGIN: Modified by liujian 00280874, 2014/7/24*/
    //HI_U16 usFragmentCount;        //总分包数
    HI_U16 usFragmentCount : 9;      //总分包数
    HI_U16 rsb_pkt_flag : 1;         //0:不需要组成一个包上报;1:需要组成一个完整的包上报。MAC原有分包应用场景下，该值为0
    HI_U16 pad : 6;                  //保留，默认值为0
    /* END:   Modified by liujian 00280874, 2014/7/24 */

    HI_U16 usFragmentTransId; // 一次分包使用同一个TransId.
                         // 该值通过  HI_DIAG_GetFragmentTransId获取
    /* BEGIN: Modified by likunhe/00194117, 2014/1/3   问题单号:DTS2013122607967*/
    //HI_U16 pad;
    HI_U16 usTransId;
    /* END:   Modified by likunhe/00194117, 2014/1/3 */

    HI_PBYTE pucPacket;  // 分包数据区
} HI_DIAG_FRAGMENT_PACKET_PARAM_STRU;

#if defined(PRODUCT_CFG_NDM_SUPPORT_FRAGMENT)
//*****************************************************************************
// 函数名称: HI_DIAG_GetFragmentTransId
// 功能描述: 获取分包的transId
//
// 参数说明:
//
// 返 回 值:
//      返回 TransID
//
// 调用要求: 1. 用于要分包传输时调用以获取TransID,
//                  用于标示一次完整的分包传输过程。
// 调用举例:
// 作    者: db. 2012.09.20
//*****************************************************************************
HI_EAPI HI_U16 HI_DIAG_GetFragmentTransId(HI_VOID);


//*****************************************************************************
// 函数名称: HI_DIAG_ReportFragmentPacket
// 功能描述: 发送分包数据包到HSO
//           //
//            发送分包数据， 仅仅在远程通道上有效，
//            当非远程通道使用时等同于HI_DIAG_ReportPacket。
//
// 参数说明:
//   ulId [in] 包ID
//   param :
//          pucPacket [in] 数据包的buffer地址, 该函数不会释放该指针,如果不需要可以值为HI_NULL
//          usPacketSize [in] 数据包大小, 如果不需要可以值为0
//          usSn  [in] 分包序号
//          usFragmentCount[in]  总分包数
//          usTransId [in]  一次分包使用同一个TransId.该值通过
//                               HI_DIAG_GetFragmentTransId获取
//
//  bSync [in] HI_TRUE:不通过OS队列发送包, 仅适用于HI_DIAG_RegiestInputCommand注册的处理接口中;
//              HI_FASE:通过OS队列发送包, 预定义的缓冲(COMMAND Buffer不足)时候则自动启用动态分配方式.
//   ucInstanceId [in] 实例ID,HI_DIAG_CMD_INSTANCE_DEFAULT 表示本地命令,SAL直接调用给注册的HSO命令处理入口
//                     HI_DIAG_CMD_INSTANCE_IREMOTE 表示远程命令, 经过MAC透传后,SAL再调用给注册的HSO命令处理入口
//
// 返 回 值:
//    HI_ERR_SUCCESS: 成功
//    其它值表示错误码.
//
// 调用要求: 1. 要分包传输时使用
// 调用举例:
// // Example:
// HI_U32 dfx_query_topo(HI_U32 ulID,
//                      HI_U8 ucInstanceId, ….)
// {
//  HI_U16 usTransId = 0;
//    HI_U16 usSN = 0;
//    HI_U16 usFragmentCount = 0;
//    HI_ DIAG _FRAGMENT_PARAM_STRU param;
//
//    HI_MDM_Memzero(&param, sizeof(param));
//
//   // 获取分包个数, 总的站点个数/每个分包可以放的站点数.
//   usFragmentCount = query_topo_node_count() /
//                         FRAGMENT_SIZE;
//   usTransId = HI_DIAG_GetFragmentTransId();
//
//    param.usFragmentCount = usFragmentCount;
//    param.usTransId = usTransId;
//
//   for (usSn = 0; usSn < usFragmentCount; usSn ++)
//   {
//       Param.usSn = usSn;
//       HI_DIAG_ReportFragmentPacket(ulId, ucInstanceId
//                     & param, HI_FALSE);
//
//     ....
// }
//     ....
// }
// 作    者: db.2012.09.20
//*****************************************************************************
HI_EAPI HI_U32 HI_DIAG_ReportFragmentPacket(HI_U8 ucInstanceId, HI_DIAG_FRAGMENT_PACKET_PARAM_STRU *param, HI_BOOL bSync);
#endif
/* END ADDED by db 2012-09-20   */

#endif //PRODUCT_CFG_CORE_TYPE_BOOTLOADER


//*****************************************************************************
#include <mdm_inf.h>
//*****************************************************************************





//*****************************************************************************
HI_END_HEADER
#endif // __SAL_INF_H__

