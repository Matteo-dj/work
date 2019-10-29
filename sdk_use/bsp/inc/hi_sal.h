//****************************************************************************
//
//                  版权所有 (C), 2001-2011, 华为技术有限公司
//
//****************************************************************************
//  文 件 名   : hi_sal.h
//  版 本 号   : V1.0
//  作    者   : 沈汉坤/00130424
//  生成日期   : 2011年10月10日
//  功能描述   : SAL(系统功能应用层), 包括OS接口/DMS/NV/DIAG和系统初始化和OS资源分配管理
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

/* BEGIN: Added by b00208046, 2014/3/13   问题单号:DTS2014031300959 */
typedef enum
{
    HI_MDM_UFC_BLK_ID_UPG = 0, // 升级
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
    HI_SAL_MEM_POOL_ID_SYS,//系统内存池
    HI_SAL_MEM_POOL_ID_DYNAMIC,//动态内存池
    HI_SAL_MEM_POOL_ID_STATIC,//静态内存池
    HI_SAL_MEM_POOL_ID_MAX,
}HI_SAL_MEM_POOL_ID_E;
#if defined(PRODUCT_CFG_UFC_FLASH_MANAGE) // 仅CCO产品系列支持该特性
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
// 复位处理接口定义
//****************************************************************************
// 延迟重启(单位s):仅影响g_stProCtx.astRstNotify[x].bRstDelay为HI_TRUE的主动复位事件
HI_EAPI HI_U32 HI_SYS_ActiveRstDelay(HI_IN HI_U32 duration);
// 延迟重启使能
HI_EAPI HI_U32 HI_SYS_RstDelayEnable(HI_SYS_RST_DELAY_CON_E enDelayCon,HI_U32 ms);

// 延迟重启去使能
HI_EAPI HI_U32 HI_SYS_RstDelayDisable (HI_SYS_RST_DELAY_CON_E enDelayCon);

// 注册复位通知处理入口
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
// 获取系统状态 HI_SYS_STATUS_IND_S
//****************************************************************************
HI_EAPI HI_SYS_STATUS_IND_S* HI_SYS_GetSysStatus(HI_VOID);
#endif
//*****************************************************************************


//*****************************************************************************
// 函数名称: HI_MDM_Sleep
// 功能描述: 任务睡眠
//
// 参数说明:
//   ulMilliSecond [in] 睡眠时间, 单位为毫秒, 精度为10ms
//   入参小于10ms时，当做10ms处理。大于10ms时整除向下对齐
//
// 返 回 值:
//    HI_ERR_SUCCESS: 成功
//    其它错误码参见hi_errno.h
//
// 调用要求: 仅在任务中使用
// 调用举例: HI_MDM_Sleep(10)
// 作    者: shenhankun/00130424 [2011-11-10]
//*****************************************************************************
#define HI_SYS_TASK_WAIT_EXCEPTION_POLLING_WAIT()     HI_MDM_Sleep(HI_SYS_TASK_WAIT_EXCEPTION_TIME_OUT)


//*****************************************************************************
// 函数名称:  HI_SYS_GetMemPoolInfo
//
// 功能描述:  获取内存池信息
//
// 参数说明:
//   enPoolId[in]内存池ID
//   pstPoolInfo[out]内存池信息
//
// 返 回 值:
//    HI_ERR_SUCCESS : 获取内存池信息成功
//    其它值错误
// 调用要求:
//*****************************************************************************
HI_U32 HI_SYS_GetMemPoolInfo(HI_SAL_MEM_POOL_ID_E enPoolId,HI_MDM_MEM_POOL_INFO_S *pstInfo);

#if defined(PRODUCT_CFG_HAVE_STATIC_MEM_PROFILE)
#define HI_SYS_GETSTATIC_MEM(ulId, ulSize)  HI_MDM_GetStaticMem(ulId, ulSize); HI_DIAG_ReportSysInitEvt(id)
#else
#define HI_SYS_GETSTATIC_MEM(ulId, ulSize)  HI_MDM_GetStaticMem(ulId, ulSize);
#endif


//****************************************************************************
// 函数名称: HI_MDM_Malloc HI_MDM_Free
// 功能描述: 动态分配、释放内存
//
// 参数说明:
//   ulModId [in] 模块ID, 0表示标准模块,保留给系统使用, 参见 hi_config.h 定义, HI_MOD_ID_*,用户可用USER : [900, 999]
//   ulSize [in] 动态内存大小
//   pAddr [in] 动态内存指针
//
// 返 回 值:
//    非零表示内存地址，零表示系统没有初始化或是内存不足
//
// 调用要求: 1) 系统初始化成功
//           2) 使用方法和malloc和free类似
//           4) ulModId 参数在分配和释放接口中输入必须一致.
// 调用举例: TODO: ...
// 作    者: shenhankun/00130424 [2011-10-19]
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
// 函数名称: HI_SYS_ControlInterrupts
// 功能描述: 中断控制
//
// 参数说明:
//   ulLevel [in] HI_SYS_ENABLE_INTERRUPTS  : 所有中断使能
//                HI_SYS_DISABLE_INTERRUPTS : 所有中断关闭
//
// 返 回 值:
//    HI_SYS_DISABLE_INTERRUPTS : 所有中断关闭
//    HI_SYS_ENABLE_INTERRUPTS  : 所有中断使能
//    HI_ERR_FAILURE        : 中断控制失败
//
// 调用要求: TODO: ...
// 调用举例:
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
// 作    者: shenhankun/00130424 [2011-12-01]
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
// 函数名称: HI_SYS_TSTK_ERR_ASSERT
// 功能描述: 任务栈检查
//
// 参数说明:
//   无
//
// 返 回 值:
//    无
//
// 调用要求: 任务中调用, 不能适用于中断
// 作    者: shenhankun/00130424 [2012-01-29]
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
// 函数名称: HI_SYS_SocketSend
// 功能描述: 适用于已连接的数据包或流式套接口发送数据。
//           对于数据报类套接口，必需注意发送数据长度不
//           应超过通讯子网的IP包最大长度
//
// 参数说明:
//   s [in] : 一个用于标识已连接套接口的描述字
//   buf [in] : 包含待发送数据的缓冲区
//   len [in] : 缓冲区中数据的长度
//
// 返 回 值:
//          若无错误发生,返回所发送数据的总数。
//          否则的话，返回ERROR(-1)错误
//  说明:
//       本函数采用阻塞I/O的调用执行方式
//*****************************************************************************
HI_EAPI HI_S32    HI_SYS_SocketSend(HI_SOCKET s, HI_PBYTE buf, HI_S32 len);
//*****************************************************************************
// 函数名称: HI_SYS_SocketRecv
// 功能描述: 本函数用于已连接的数据报或流式套接口进行数据的接收。
//
// 参数说明:
//   s [in] : 一个用于标识已连接套接口的描述字
//   buf [out] : 用于接收数据的缓冲区
//   len [in] : 缓冲区的长度
//
// 返 回 值:
//          若无错误发生，HI_SYS_SocketRecv()返回读入的字节数。
//          如果连接已中止，返回0。如果发生错误，返回-1
//  说明:
//       本函数采用阻塞I/O的调用执行方式
//*****************************************************************************
HI_EAPI HI_S32    HI_SYS_SocketRecv (HI_SOCKET s, HI_OUT HI_PBYTE buf, HI_S32 len);
//*****************************************************************************
// 函数名称: HI_SYS_SocketSendTo
// 功能描述: 用来将数据由指定的socket传给对方主机
//
// 参数说明:
//   s [in] : 一个用于标识已连接套接口的描述字
//   buf [in] : 用于发送数据的缓冲区
//   len [in] : 缓冲区的长度
//   pucIp [in]: 指向目的套接字的地址
//   usPortNum [in] : 端口号
//
// 返 回 值:
//          成功则返回实际传送出去的字符数，失败返回－1
//  说明:
//         本函数采用阻塞I/O的调用执行方式
//*****************************************************************************
HI_EAPI HI_S32    HI_SYS_SocketSendTo(HI_SOCKET s, HI_PBYTE buf, HI_S32 len, HI_IN HI_PCSTR pucIp, HI_U16 usPortNum);
//*****************************************************************************
// 函数名称: HI_SYS_SocketRecvFrom
// 功能描述:用来接收远程主机经指定的socket传来的数据
//          并把数据传到由参数buf指向的内存空间
//
// 参数说明:
//   s [in] : 一个用于标识已连接套接口的描述字
//   buf [in] : 用于接收数据的缓冲区
//   len [in] : 缓冲区的长度
//
// 返 回 值:
//          成功则返回接收到的字符数,失败返回-1
//  说明:
//          本函数采用阻塞I/O的调用执行方式
//*****************************************************************************
HI_EAPI HI_S32    HI_SYS_SocketRecvFrom(HI_SOCKET s, HI_OUT HI_PBYTE buf, HI_S32 len);
//*****************************************************************************
// 函数名称: HI_SYS_SocketConnect
// 功能描述:本函数用于创建与指定外部端口的连接
//
// 参数说明:
//   s [in] : 一个用于标识已连接套接口的描述字
//   pucIp [in] : 目的IP地址
//   usPortNum [in] : 端口号
//
// 返 回 值:
//          成功则返回0，失败返回-1
//*****************************************************************************
HI_EAPI HI_S32    HI_SYS_SocketConnect(HI_SOCKET s, HI_PCSTR pucIp, HI_U16 usPortNum);
//*****************************************************************************
// 函数名称: HI_SYS_SocketConnect
// 功能描述:用于任何类型的套接口禁止接收、禁止发送或禁止收发
//
// 参数说明:
//   s [in] : 一个用于标识已连接套接口的描述字
//   how：标志，用于描述禁止哪些操作。
//              how的方式有三种分别是
//              SHUT_RD（0）：关闭sockfd上的读功能，此选项将不允许sockfd进行读操作。
//              SHUT_WR（1）：关闭sockfd的写功能，此选项将不允许sockfd进行写操作。
//              SHUT_RDWR（2）：关闭sockfd的读写功能。
// 返 回 值:
//           成功则返回0，失败返回-1
//*****************************************************************************
HI_EAPI HI_S32    HI_SYS_SocketShutdown(HI_SOCKET s, HI_S32 how);
//*****************************************************************************
// 函数名称: HI_SYS_SocketClose
// 功能描述:本函数关闭一个套接口
//
// 参数说明:
//   s [in] : 一个用于标识已连接套接口的描述字
//
// 返 回 值:
//          成功则返回0，失败返回-1
//*****************************************************************************
HI_EAPI HI_S32    HI_SYS_SocketClose(HI_SOCKET s);
//*****************************************************************************
// 函数名称: HI_SYS_SocketAccept
// 功能描述:在一个套接口接受一个连接
//
// 参数说明:
//   s [in] : 一个用于标识已连接套接口的描述字
//
// 返 回 值:
//           成功返回一个新的套接字描述符，失败返回-1。
//*****************************************************************************
HI_EAPI HI_SOCKET HI_SYS_SocketAccept (HI_SOCKET s);
//*****************************************************************************
// 函数名称: HI_SYS_SocketTcpCreate
// 功能描述: 创建TCP socket。
//      默认会设置以下几个参数,可通过onebuiler来修改配置:
//      发送BUFFER: PRODUCT_CFG_TCP_TX_BUFFER
//      接收BUFFER: PRODUCT_CFG_TCP_RCV_BUFFER
//      KEEPALIVE配置:
//          PRODUCT_CFG_TCP_KEEPINTVL  #设置发送keepalive的间隔时间（s）
//          PRODUCT_CFG_TCP_KEEPALIVE_R2 #设置不通信则开始启动keepalive的时间（s）
//          PRODUCT_CFG_KEEPALIVE_WAIT  #设置keepalive探测次数(次)
//    #设置发送keepalive的间隔时间（s）
// 参数说明:无
//
// 返 回 值: 正确:SOCKET_ID.
//           失败:0xFFFFFFFF
//
// 调用要求: 任务中调用, 不能适用于中断
// 作    者: shenhankun/00130424 [2012-01-29]
//*****************************************************************************
HI_EAPI HI_SOCKET HI_SYS_SocketTcpCreate(HI_VOID);
//*****************************************************************************
// 函数名称: SAL_SocketUdpCreate
// 功能描述: 创建UDP socket。
//
// 参数说明:无
//
// 返 回 值: 正确:SOCKET_ID.
//           失败:0xFFFFFFFF
//
// 调用要求: 任务中调用, 不能适用于中断
// 作    者: shenhankun/00130424 [2012-01-29]
//*****************************************************************************
HI_EAPI HI_SOCKET HI_SYS_SocketUdpCreate(HI_VOID);
HI_EAPI HI_U32    HI_SYS_SocketSetLinger(HI_SOCKET s);
//*****************************************************************************
// 函数名称: HI_SYS_SocketConnect
// 功能描述:将一本地地址与一套接口捆绑。
//                       本函数适用于未连接的数据报或流类套接口
//
// 参数说明:
//   s [in] : 一个用于标识已连接套接口的描述字
//   pucIp [in] : 目的IP地址
//   usPortNum [in] : 端口号
//
// 返 回 值:
//                   成功则返回HI_ERR_SUCCESS，失败返回HI_ERR_SYSTEM_CALL_ERROR
//*****************************************************************************
HI_EAPI HI_S32    HI_SYS_SocketBind(HI_SOCKET s, HI_PCSTR pucIp, HI_U16 usPortNum);
//*****************************************************************************
// 函数名称: HI_SYS_SocketListen
// 功能描述:创建一个套接口并监听申请的连接.
//
// 参数说明:
//   s [in] : 一个用于标识已连接套接口的描述字
//   backlog [in] : 等待连接队列的最大长度
//
// 返 回 值:
//                   成功则返回0，失败返回-1
//*****************************************************************************
HI_EAPI HI_S32    HI_SYS_SocketListen(HI_SOCKET s, HI_S16 backlog);
//*****************************************************************************
// 函数名称: HI_SYS_SocketSetFd
// 功能描述:设置Socket位图中的位域
//
// 参数说明:
//   s [in] : 一个用于标识已连接套接口的描述字
//   fd [in] : socket 位域内容指针
//
// 返 回 值:
//                   无
//*****************************************************************************
HI_EAPI HI_VOID   HI_SYS_SocketSetFd(HI_SOCKET s, HI_IN HI_SYS_SOCKET_FD* fd);
HI_EAPI HI_BOOL   HI_SYS_SocketIsSetFd(HI_SOCKET s, HI_IN HI_SYS_SOCKET_FD* fd);
//*****************************************************************************
// 函数名称: HI_SYS_SocketSelect
// 功能描述:确定一个或多个套接口的状态
//
// 参数说明:
//   nfds [in] : 是一个整数值，是指集合中所有文件描述符的范围
//   readfds [in] : 指向一组等待可读性检查的套接口
//   writefds [in] : 指向一组等待可写性检查的套接口
//   ulTimeOut [in] : 最多等待时间
// 返 回 值:
//                   成功返回HI_ERR_SUCCESS
//                   超时返回HI_ERR_TIMEOUT
//                   错误返回HI_ERR_SYSTEM_CALL_ERROR
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
// 获取[ulMin,ulMax]之间的随机整数
//****************************************************************************
HI_EAPI HI_U32 HI_SYS_GetRandNum(HI_U32 ulMin, HI_U32 ulMax);
HI_U32 HI_SYS_GetRandNumWithSeed(HI_U32 ulMin, HI_U32 ulMax, HI_U32 seed);
HI_U32 HI_SYS_GetRandNum32WithSeed(HI_U32 ulMin, HI_U32 ulMax, HI_U32 seed);

HI_EAPI HI_U32 HI_SYS_GetRandNum32(HI_U32 ulMin, HI_U32 ulMax);
HI_EAPI HI_U16 HI_SYS_GetRandNum16(HI_U16 ulMin, HI_U16 ulMax);
HI_EAPI HI_U32 HI_SYS_GetRandNum32AddMac(HI_U32 ulMin, HI_U32 ulMax);

//****************************************************************************

//****************************************************************************
// 函数名称: HI_SYS_Itoa
// 功能描述: 将数字转换成字符串
//
// 参数说明:
//      str[out]:转换后的字符串
//      num:被转换数字
//      opt:转换方式HI_SYS_ITOA_OPT(ucfillChar,ucMinLen,enRadix)生成
//          ucfillChar:填充字符串:相当于sprint中%03d中的0
//          ucMinLen:转换成字符串最小长度:相当于sprint中%03d中的3
//          enRadix:按照什么格式转换
//          enRadix:转换类型HI_SYS_ITOA_RADIX_D:%d  HI_SYS_ITOA_RADIX_U:%u HI_SYS_ITOA_RADIX_H:%x
// 返 回 值:
//      HI_ERR_SUCCESS 转换成功
//      其它:转换失败
//
// 调用要求: 无
// 调用举例: 无
//****************************************************************************
#define HI_SYS_ITOA_RADIX_D     0
#define HI_SYS_ITOA_RADIX_U    1
#define HI_SYS_ITOA_RADIX_H     2

#define HI_SYS_ITOA_OPT(ucfillChar,ucMinLen,enRadix) ((((HI_U8)(ucfillChar))&0xFF)|((((HI_U8)(ucMinLen))<<8)&0xFF00)|((((HI_U8)(enRadix))<<16)&0xFF0000))
HI_EAPI HI_U32 HI_SYS_Itoa(HI_CHAR *str,HI_S32 num,HI_U32 opt);

HI_EAPI HI_U32 HI_SYS_Reboot(HI_SYS_REBOOT_CAUSE_E enCause);
HI_EAPI HI_U32 HI_SYS_RebootSafe(HI_SYS_REBOOT_CAUSE_E enCause);

//*****************************************************************************
// 其它设备相关接口
//*****************************************************************************

//*****************************************************************************


//*****************************************************************************
// NV Interface
//*****************************************************************************

//*****************************************************************************
// 函数名称: HI_NVM_Read
//
// 功能描述: 根据NV项ID读NV项内容
//           读取ID为ulId的NV项并拷贝到pItemData上，拷贝长度为ulItemDataLen,
//           返回实际长度到存到pusItemDataLen指针中.
//
// 参数说明: ulId [in] NV项ID，ID的具体定义由工具生成.
//           pItemData[out] 保存NV项内容的指针，用于该函数回写指定的NV项到该地址.
//           ulItemDataLen[in] pItemData指向buffer的长度,目前最大长度支持1024个字节
//           bIfFailUseDefault[in] HI_TRUE: 如果读取NV项失败, 则使用NV文件的初始化值替代.
//
// 返 回 值: HI_ERR_SUCCESS                表示读取NV项成功
//           HI_ERR_NV_ITEM_IS_EMPTY	    NV项为空
//           HI_ERR_NV_NOT_SUPPORT_ID	    NV项ID不存在
//           HI_ERR_NV_NOT_SUPPORT_LENTH	NV项ID长度超出预定义的实际长度
//           ERR_SAL_FLASH_ALIGN_ERROR	    Flash对齐错误
//           ERR_SAL_FLASH_PROTECT_ERROR	Flash保护
//           HI_ERR_NOT_SUPPORT	        不支持
//
// 调用要求: 1、在任务中使用，不能在中断中使用, 支持多任务操作。
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
// 函数名称: HI_NVM_Write HI_NVM_WriteFactory
//
// 功能描述: 根据NV项ID写NV项内容
//
// 参数说明: ulId [in] NV项ID，ID的具体定义由工具生成.
//           pItemData[in] NV项内容的指针， 用于该函数读取NV项并写到内部的NV项区域.
//           ulItemDataLen[in] pItemData指向buffer的长度，目前最大长度支持1024个字节
//
// 返 回 值: HI_ERR_SUCCESS                表示写NV项成功
//           HI_ERR_NV_ITEM_IS_EMPTY	    NV项为空.
//           HI_ERR_NV_NOT_SUPPORT_ID    	NV项ID不存在
//           HI_ERR_NV_NOT_SUPPORT_LENTH	NV项ID长度超出预定义的实际长度
//           HI_ERR_NOT_SUPPORT	            不支持
//           其它错误码
//
// 调用要求:
//           1) 在任务中使用，不能在中断中使用, 支持多任务操作
//           2) HI_NVM_Write不支持工厂写入NV项,HI_NVM_WriteFactory仅支持写入工厂配置项
//*****************************************************************************
HI_EAPI HI_U32 HI_NVM_Write(HI_U16 usId, HI_CONST HI_PVOID pItemData, HI_U16 ulItemDataLen);
HI_EAPI HI_U32 HI_NVM_WriteFactory(HI_U16 usId, HI_CONST HI_PVOID pData,HI_U16 usDataLen);
HI_EAPI HI_VOID HI_NVIM_Update_Index(HI_VOID);

//*****************************************************************************
// 函数名称: HI_NVM_RestoreFactorySetting
//
// 功能描述: 恢复指定section的当前版本的NV初始值
//
// 参数说明: ulSectionId[in]: 指定的section ID, 如果为0 则默认恢复所有的NV项
//
//返回值     : HI_ERR_SUCCESS            操作成功
//             HI_ERR_MALLOC_FAILUE      申请内存失败
//             HI_ERR_INVALID_PARAMETER  初始文件错误
//             HI_ERR_NV_ERROR_READ      读flash失败
//             其它错误码
//
// 调用要求:
//           1) 在任务中使用，不能在中断中使用, 支持多任务操作
//*****************************************************************************
#if !defined(PRODUCT_CFG_CORE_TYPE_BOOTLOADER) && defined(PRODUCT_CFG_NVM_EXT_SUPPORT)
HI_EAPI HI_U32 HI_NVM_RestoreFactorySetting(HI_U32 ulSectionId);
#endif

//*****************************************************************************
// 函数名称: HI_NVM_RegisterChangedNoitfyProc
// 功能描述: NV项变更通知
//
// 参数说明:
//   usMinId [in] NV项ID最小值
//   usMaxId [in] NV项ID最大值
//   pfnChanged [in] NV项改变的处理函数, 即NV项变更后 NV模块会自动调用该注册的接口.
//
// 返 回 值:
//   HI_ERR_SUCCESS  表示注册成功
//   HI_ERR_NOT_FREEE_SPACE NVM内部空间不足, 无法注册
//   其它值为错误码
//
// 调用要求: 在SAL初始化结束后才可以调用该接口.
// 调用举例:
//          HI_U32 MyNvChangedProc(HI_U16 ulId) { .... }
//          HI_NVM_RegisterChangedNoitfyProc(0x50, 0x100, MyNvChangedProc)
// 作    者: shenhankun/00130424 [2011-12-12]
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
// 函数名称: HI_DMS_GetProductId HI_DMS_GetLocalMacAddr  HI_DMS_GetChipType HI_DMS_GetProductType
// 功能描述: 获取通信模块的产品ID, 包括MAC地址、芯片类型、产品形态等信息
//           HI_DMS_GetLocalMacAddr HI_DMS_GetChipType HI_DMS_GetProductType 都是 DMS_GetProductId的封装接口，其中
//           HI_DMS_GetLocalMacAddr 用于获取通信模块的MAC地址，
//           DMS_GetChipType用于获取通信模块的芯片类型，
//           DMS_GetProductType用于获取通信模块的产品形态
//
// 参数说明:
//   ucPlcMac[HI_PLC_MAC_ADDR_LEN] [out] 通信模块的MAC地址
//   penChipType [out] 通信模块中主芯片类型, 不需要该参数则传入HI_NULL
//   penProductType [out] 通信模块的定制的产品形态, 不需要该参数则传入HI_NULL
//   penHwPlatType [out] 通信模块的硬件平台类型，不需要该参数则传入HI_NULL
//
// 返 回 值:
//    HI_ERR_SUCCESS : 成功
//    其它值错误
//
// 调用要求: DSM初始化后，即可调用
// 作    者: shenhankun/00130424 [2011-10-26]
//*****************************************************************************
HI_EAPI HI_U32 HI_DMS_GetProductId(HI_OUT HI_U8 ucPlcMac[HI_PLC_MAC_ADDR_LEN], HI_OUT HI_FTM_CHIP_TYPE_E* penChipType, HI_OUT HI_FTM_PRODUCT_TYPE_E* penProductType, HI_OUT HI_FTM_HW_PLATFORM_TYPE_E* penHwPlatType);
HI_EAPI HI_U32 HI_DMS_GetLocalMacAddr(HI_U8 ucMac[HI_PLC_MAC_ADDR_LEN]); // 获取本地的ETH MAC地址
HI_EAPI HI_FTM_CHIP_TYPE_E HI_DMS_GetChipType(HI_VOID); // 获取芯片类型
HI_EAPI HI_FTM_PRODUCT_TYPE_E HI_DMS_GetProductType(HI_VOID); // 获取产品类型
HI_EAPI HI_FTM_HW_PLATFORM_TYPE_E HI_DMS_GetHwType(HI_VOID); // 获取硬件平台类型
HI_EAPI HI_FTM_CHIP_TYPE_E HI_DMS_GetChipTypeEx(HI_VOID); // 如果获取不到合法的芯片类型，则默认为 STA
HI_EAPI HI_FTM_PRODUCT_TYPE_E HI_DMS_GetProductTypeEx(HI_VOID); // 如果获取不到合法的产品形态，则默认为 电表


// HI_DMS_GetProductId 的精简版本接口
HI_EAPI HI_BOOL HI_DMS_IsProductCon(HI_FTM_PRODUCT_TYPE_E enProductType);     // 是否为集中器载波模块
HI_EAPI HI_BOOL HI_DMS_IsProductRelay(HI_FTM_PRODUCT_TYPE_E enProductType);   // 是否为中继器
HI_EAPI HI_BOOL HI_DMS_IsProductMeter(HI_FTM_PRODUCT_TYPE_E enProductType);  // 是否为电表
HI_EAPI HI_BOOL HI_DMS_IsProductMeterI(HI_FTM_PRODUCT_TYPE_E enProductType);  // 是否为I型采集器
HI_EAPI HI_BOOL HI_DMS_IsProductMeterII(HI_FTM_PRODUCT_TYPE_E enProductType); // 是否为 II型采集器
HI_EAPI HI_BOOL HI_DMS_IsProductNdm(HI_FTM_PRODUCT_TYPE_E enProductType);    // 是否为抄控器
HI_EAPI HI_BOOL HI_DMS_IsProductTsta(HI_FTM_PRODUCT_TYPE_E enProductType);    // 是否为三相表模块
HI_EAPI HI_BOOL HI_DMS_IsInvalidProductType(HI_FTM_PRODUCT_TYPE_E enProductType);
HI_EAPI HI_BOOL HI_DMS_IsChipCco(HI_FTM_CHIP_TYPE_E enChipType); // 是否为头端载波模块
HI_EAPI HI_BOOL HI_DMS_IsChipSta(HI_FTM_CHIP_TYPE_E enChipType);  // 是否为终端载波模块
HI_EAPI HI_U8 HI_DMS_LoadDevType(HI_VOID);
HI_EAPI HI_BOOL SAL_IsMeterCheckProduct(HI_VOID);
HI_EAPI HI_FTM_CHIP_TYPE_E HI_COM_ReadProductTypeFromHw(HI_U32 asic_ver);
HI_EAPI HI_FTM_PRODUCT_TYPE_E HI_COM_ReadChipTypeFromHw(HI_U32 pro_ver,HI_FTM_CHIP_TYPE_E chiType);
HI_EAPI HI_BOOL SAL_IsUstaProduct(HI_VOID);

// correct the spelling error

#define HI_IS_PRODUCT_CON()      HI_DMS_IsProductCon(HI_DMS_GetProductType())      // 是否为集中器
#define HI_IS_PRODUCT_METER()    HI_DMS_IsProductMeter(HI_DMS_GetProductType())   // 是否为电表
#define HI_IS_PRODUCT_METERI()   HI_DMS_IsProductMeterI(HI_DMS_GetProductType())   // 是否为I型采集器
#define HI_IS_PRODUCT_METERII()  HI_DMS_IsProductMeterII(HI_DMS_GetProductType())  // 是否为 II型采集器
#define HI_IS_PRODUCT_NDM()      HI_DMS_IsProductNdm(HI_DMS_GetProductType())     // 是否为抄控器
#define HI_IS_PRODUCT_RELAY()    HI_DMS_IsProductRelay(HI_DMS_GetProductType())    // 是否为中继器
#define HI_IS_PRODUCT_Tsta()     HI_DMS_IsProductTsta(HI_DMS_GetProductType())    // 是否为三相表模块

#define HI_IS_CCO_CHIP()         HI_DMS_IsChipCco(HI_DMS_GetChipType()) // 是否为 CCO 芯片
#define HI_IS_STA_CHIP()         HI_DMS_IsChipSta(HI_DMS_GetChipType()) // 是否为 STA 芯片

HI_EAPI HI_U32 HI_DMS_GetNetCfg(HI_U8 ucPlcMac[HI_PLC_MAC_ADDR_LEN], HI_U8 ucEthMac[HI_PLC_MAC_ADDR_LEN], HI_U8 ucEthIp[HI_IPV4_ADDR_LEN],HI_U8 ucEthMask[HI_IPV4_ADDR_LEN]);
//*****************************************************************************

//*****************************************************************************
// 函数名称: HI_DMS_SetForwarding
// 功能描述: 设置端口 转递功能
//
// 参数说明:
//   usPortNum[in] 取值为 HI_DMS_CHL_UART_PORT_APP 则 HI_DMS_CHL_UART_PORT_APP端口 将转递到 HI_DMS_CHL_UART_PORT_DIAG端口
//   bForwarding[in] HI_TRUE: 设置 转递; HI_FALSE: 取消
//
// 返 回 值:
//    HI_ERR_SUCCESS : 成功
//    其它值错误
//
// 调用要求: 初始化后，即可调用.
//           HI_DMS_SetForwarding(HI_DMS_CHL_UART_PORT_APP, HI_TRUE); // UART_APP-->UART_DIAG ->enable
//           HI_DMS_SetForwarding(HI_DMS_CHL_UART_PORT_APP, HI_FAlSE); // UART_APP-->UART_DIAG ->disable
// 作    者: shenhankun/00130424 [2011-10-26]
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
// 函数名称:  HI_DMS_ChlSetup
//
// 功能描述: 通道注册
//
// 参数说明:
//   pstChlPort [in] 通道端口对象, HI_NULL则表示关闭指定端口
//   usPortNum [in] 通道端口
//               HI_DMS_CHL_MAC_PORT_APP     : 表示MAC通道
//               HI_DMS_CHL_MAC_PORT_DIAG    : 表示MAC通道
//               HI_DMS_CHL_UART_PORT_APP    : 表示UART的MR通道
//               HI_DMS_CHL_UART_PORT_IR     : 表示IR通道
//               HI_DMS_CHL_TCP_PORT_APP     : 表示TCP的MR通道
//               HI_DMS_CHL_TCP_PORT_DIAG    : 表示TCP的DIAG通道
//               HI_DMS_CHL_UART_PORT_DIAG   : 表示UART的DIAG通道
//   usInstanceId [in] 通道端口实例, 取值范围[0,1], 缺失为0.
//   pstNvDevCfg [in] 不为空表示使用用户配置, HI_NULL则表示从NV中读取重新配置通道
//
// 返 回 值:
//    HI_ERR_SUCCESS : 成功
//    其它值错误
// 调用要求:
//    使用默认配置: HI_DMS_ChlSetup
//    更改默认配置步骤: Change NV item(HI_NV_FTM_DEVICE_CFG_ID) --> HI_DMS_ChlRefresh --> HI_DMS_ChlSetup
// 作    者: shenhankun/00130424 [2011-10-26]
//*****************************************************************************
HI_EAPI HI_U32 HI_DMS_ChlSetup(HI_CONST HI_DMS_CHL_PORT_OBJ_S* pstChlPort, HI_U16 usPortNum);
#if defined(PRODUCT_CFG_SAL_API_SUPPORT)
#define HI_DMS_ChlSetup(pstChlPort, usPortNum) HI_DMS_ChlSetupInstance(pstChlPort, usPortNum, 0)
#endif
HI_EAPI HI_U32 HI_DMS_ChlRefresh(HI_DMS_NV_FTM_DEVICE_CFG_S* pstDevCfg);
//*****************************************************************************
// 函数名称: HI_DMS_ChlUartPortRefresh
// 功能描述: 通过该接口可以修改串口初始化配置
//
// 参数说明:
//   pstDevCfg[in]: 需要配置的值
//   chipType[in]:  芯片类型
//   usportNum[in]: 端口号
//
// 返 回 值:
//    HI_ERR_SUCCESS : 成功
//    其它值错误
//
// 调用要求:
//
//*****************************************************************************
HI_U32 HI_DMS_ChlUartPortRefresh(HI_FTM_DEVICE_UART_CFG_S* pstDevCfg, HI_FTM_CHIP_TYPE_E chipType, HI_U16 usportNum);

//*****************************************************************************


//*****************************************************************************
// 函数名称: HI_DMS_ChlTxFrm HI_DMS_ChlTxMsg
// 功能描述: 通道发送数据帧/消息，支持PLC通道、UART、TCP通道
//
// 参数说明:
//   usPortNum [in] 通道端口
//               HI_DMS_CHL_MAC_PORT_APP    : 表示MAC通道
//               HI_DMS_CHL_UART_PORT_IR    : 表示IR通道
//               HI_DMS_CHL_MAC_PORT_DIAG   : 表示MAC通道
//               HI_DMS_CHL_UART_PORT_APP   : 表示UART的MR通道
//               HI_DMS_CHL_TCP_PORT_APP    : 表示TCP的MR通道
//               HI_DMS_CHL_TCP_PORT_DIAG   : 表示TCP的DIAG通道
//               HI_DMS_CHL_UART_PORT_DIAG  : 表示UART的DIAG通道
//
//   pFrameData [in] DMS的帧数据结构
//   pFrameCtrl [in] DMS的帧控制信息, 取值为HI_NULL表示本地连接处理（如ETH、串口等）
//   ulMsgId [in] 消息ID, 必须大于 HI_CHL_MSG_MIN
//
// 返 回 值:
//    HI_ERR_SUCCESS           : 成功
//    HI_ERR_NOT_CONNECT       : 通道没有连接, 发送失败
//    HI_ERR_INVALID_PARAMETER : 空数据, 不允许发送
//    其它值为错误码
//
// 调用要求: 初始化时候，先调用HI_DMS_ChlSetup 进行注册安装，后才能使用HI_DMS_ChlTx发送数据
// 作    者: shenhankun/00130424 [2011-10-25]
//*****************************************************************************
#ifndef HI_HAVE_CROSS_COMPILER_ARM_GCC
HI_EAPI HI_U32 HI_DMS_ChlTx(HI_U16 usPortNum, HI_IN HI_DMS_CHL_TX_DATA_S* pFrameData, HI_IN HI_DMS_CHL_TX_CTRL_S* pFrameCtrl);
#endif
//*****************************************************************************


//*****************************************************************************
// 函数名称: HI_DMS_ChlCtrl
// 功能描述: 通道设置和查询
//
// 参数说明:
//   usPortNum[in] HI_DMS_CHL_MAC_PORT_UPG HI_DMS_CHL_MAC_PORT_APP HI_DMS_CHL_UART_PORT_APP
//   enOpt[in] HI_DMS_PORT_CTRL_OFFLINE_ONLINE_RX : 通道总能接收数据(不管PLC网络组网未完成)
//             HI_DMS_PORT_CTRL_ONLINE_RX         : 通道仅在组网完成后能接收数据
//             HI_DMS_PORT_CTRL_NO_DELAY          : 通道无延时接收(仅适用于TCP端口)
//             HI_DMS_PORT_CTRL_RX_FRM_TO         : timeout in seconds,  pInputParam = time out value; pOutputParam=0
//			   等具体参考 HI_DMS_PORT_CTRL_OPT_ID_E
//   pInputParam[in] enOpt 指定的输入参数
//   pOutputParam[in] enOpt 指定的输出参数
//
// 返 回 值:
//    HI_ERR_SUCCESS : 成功
//    其它值错误
//
// 调用要求: 初始化后，即可调用.
// 作    者: shenhankun/00130424 [2011-10-26]
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
// 函数名称: HI_DIAG_ReportEvent
// 功能描述: Report the event log to 'Event Viewer' on PC side.
// 参数说明:
//           ulEvtId [in] Specify the event identifier.
//           ucType [in] 上报到HSO的类别，取值如下
//           	 HI_DIAG_TYPE_A(0) 表示HSO的研发调测版本
// 	             HI_DIAG_TYPE_B(1) 表示HSO的现场维护版本
// 返 回 值:
//           HI_ERR_SUCCESS           表示成功
//           HI_ERR_QUEUE_FULL        表示队列满
//           HI_ERR_NO_INITILIZATION  表示队列未初始化
//           HI_ERR_FAILURE           表示发生未知错误
//           HI_ERR_CFG_NOT_ALLOW 日志不允许上报
//           HI_ERR_INVALID_PARAMETER 传参非法
//           HI_ERR_NOT_ENOUGH_MEMORY 内存分配失败
// 调用要求:
// 调用举例: HI_DIAG_ReportEvent(LOG_EVT_SHUT_DOWN)
// 作    者: 郭成钢/00149569/20090210
//*****************************************************************************
HI_EAPI HI_U32 HI_DIAG_ReportEvent(HI_U16 usEvtId, HI_U8 ucType);


//*****************************************************************************
// 函数名称: HI_DIAG_ReportAirMessage
// 功能描述:
//           Report the protocol air message to 'Air Message Viewer' on PC side
// 参数说明:
//           pRptMessage [in] Pointer to DIAG_AIR_MSG_LOG_STRU structure which hold the air message.
//           ucType [in] 上报到HSO的类别，取值如下
//           	 HI_DIAG_TYPE_A(0) 表示HSO的研发调测版本
// 	             HI_DIAG_TYPE_B(1) 表示HSO的现场维护版本
// 返 回 值:
//           HI_ERR_SUCCESS           表示成功
//           HI_ERR_NO_INITILIZATION  设备未初始化
//           HI_ERR_QUEUE_FULL        OS队列满，分配失败
//           HI_ERR_FAILURE           未知错误
//           HI_ERR_CFG_NOT_ALLOW 日志不允许上报
//           HI_ERR_INVALID_PARAMETER 传参非法
//           HI_ERR_NOT_ENOUGH_MEMORY 内存分配失败
// 调用要求:
// 调用举例:
//           1.若pData非调用者动态使用内存分配的数据:
//               pRptMessage.ulDataSize
//           2.若pData为调用者动态使用内存分配的数据:
//               MSP_SET_HEAP(pRptMessage.ulDataSize)
//           3. HI_DIAG_ReportAirMessagePrv 为内部接口,非SAL模块不能直接调用
// 作    者: 郭成钢/00149569/20090210
//*****************************************************************************
#if defined(PRODUCT_CFG_FEATURE_DIAG_AIRMSG)
HI_EAPI HI_U32 HI_DIAG_ReportAirMessage(HI_DIAG_AIR_MSG_S* pRptMessage, HI_U8 ucType);
#if defined(PRODUCT_CFG_SAL_API_SUPPORT)
#define HI_DIAG_ReportAirMessage(pRptMessage, ucType) HI_DIAG_ReportAirMessagePrv(pRptMessage, ucType)
#endif
#endif

//*****************************************************************************
// 函数名称: HI_DIAG_ReportLayerMessage
// 功能描述:
//           Report the layer message to 'Layer Message Viewer' on PC side.
//           The function to install by OS HOOK.
// 参数说明:
//           pRptMessage [in]
//             Pointer to HI_DIAG_LAYER_MSG_S structure which hold the layer message.
//           ucType [in]
//             上报到HSO的类别，取值如下
//           	 HI_DIAG_TYPE_A(0) 表示HSO的研发调测版本
// 	             HI_DIAG_TYPE_B(1) 表示HSO的现场维护版本
// 返 回 值:
//           HI_ERR_SUCCESS           表示成功
//           HI_ERR_NO_INITILIZATION  设备未初始化
//           HI_ERR_QUEUE_FULL        OS队列满，分配失败
//           HI_ERR_FAILURE           未知错误
//           HI_ERR_CFG_NOT_ALLOW 日志不允许上报
//           HI_ERR_INVALID_PARAMETER 传参非法
//           HI_ERR_NOT_ENOUGH_MEMORY 内存分配失败
// 调用要求: 1) 如果pRptMessage.pData为使用者动态分配的指针,则该函数退出后,由使用者释放该指针
// 调用举例: TODO ...
// 作    者: 郭成钢/00149569/20090210
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
// DIAG 的层间消息通道检查, 包括:
// 1) DIAG通道是否连接; 2) 层间消息的配置开关是否使能;
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
// 函数名称: HI_DIAG_ReportCommand HI_DIAG_IsCommandReportEnalbe
// 功能描述:
//           上报DIAG包到HSO中, 仅用于任务, 支持中断使用.
// 参数说明:
//           ulID       [in] COMMAND ID
//           usDataSize [in] pData Size，最大为512字节
//           pData      [in]  Pointer to statistic info structure
// 返 回 值:
//           HI_ERR_SUCCESS           表示成功
//           HI_ERR_NO_INITILIZATION  设备未初始化
//           HI_ERR_NOT_CONNECT       通道未连接
//           HI_ERR_QUEUE_FULL        OS队列满，分配失败
//           HI_ERR_FAILURE           未知错误
//           HI_ERR_CFG_NOT_ALLOW     配置开关不允许上报
//           HI_ERR_INVALID_PARAMETER 传参非法
//           HI_ERR_TOO_SMALL_BUFFER  内部缓存不足，可能是输入参数usDataSize过大导致, 大于 HI_DIAG_BUFFERING_CMD_MAX_LEN
// 调用要求: 1) 如果pData是由调用者动态分配的指针，则由调用者负责释放该指针.
//           2) HI_DIAG_ReportCommand 总能上报, 如果需要判断是否关闭命令上报，可使用 HI_DIAG_IsCommandReportEnalbe
//           3) 一般用于命令异步应答的场景
// 调用举例:
// 作    者: 郭成钢/00149569/20100225
//*****************************************************************************
HI_EAPI HI_U32 HI_DIAG_ReportCommand(HI_U16 usId, HI_U16 usDataSize, HI_PVOID pData);
HI_EAPI HI_BOOL HI_DIAG_IsCommandReportEnalbe(HI_VOID);
#if defined(PRODUCT_CFG_SAL_API_SUPPORT)
#define HI_DIAG_ReportCommand(usId, usDataSize, pData) HI_DIAG_ReportCommandPrv(usId,usDataSize,pData);

#define HI_DIAG_IsCommandReportEnalbe()  HI_DIAG_IsCommandReportEnalbePrv()
#endif

//*****************************************************************************
// 函数名称: HI_DIAG_LocalReportCommand
// 功能描述:
//           上报命令到本地FLash中
// 参数说明:
//           ulID       [in] COMMAND ID
//           ulDataSize [in] pData Size
//           pData      [in] Pointer to statistic info structure
// 返 回 值:
//           HI_ERR_SUCCESS           表示成功
//           HI_ERR_QUEUE_FULL        OS队列满，分配失败
//           HI_ERR_INVALID_PARAMETER 传参非法
//           HI_ERR_CFG_NOT_ALLOW     配置开关没有打开(默认配置开关打开，允许上报)
//           HI_ERR_NOT_SUPPORT       该版本不支持
//           其它错误码
// 调用要求: 1) HSO连接上则直接上报到HSO, 否则保存到Flash.
//           2) 调用者动态分配的内存需由调用者释放
// 调用举例:
// 作    者: 郭成钢/00149569/20100225
//*****************************************************************************
HI_EAPI HI_U32 HI_DIAG_LocalReportCommand(HI_U16 usId, HI_U16 usDataSize, HI_PVOID pData);


//*****************************************************************************
// 函数名称: HI_DIAG_EnableCommandReport
// 功能描述:
//           上报命令使能
// 参数说明:
//           bEnable  [in] HI_TRUE: 强制设置上报命令包; HI_FALSE: 关闭上报
//           无
// 返 回 值:
//           HI_ERR_SUCCESS           表示成功
// 调用要求: 1) 可以在中断中调用,支持任务中调用
// 调用举例:
// 作    者: 郭成钢/00149569/20100225
//*****************************************************************************
HI_EAPI HI_U32 HI_DIAG_EnableCommandReport(HI_BOOL bEnable);

//*****************************************************************************
// 函数名称: HI_DIAG_RegisterCommand
// 功能描述: 注册命令处理函数
//
// 参数说明:
//   pstCmdTbl [in] 命令表,必须申明为常量数组传给该参数.
//   usCmdNum [in] 命令个数, 如果 usCmdNum 为零, 而 pstCmdTbl不为零则取消注册.
//   ucInstanceId [in] 实例ID, 取值如下.
//                     HI_DIAG_CMD_INSTANCE_DEFAULT 表示注册本地处理的命令, SAL直接调用给注册的HSO命令处理入口.
//                     HI_DIAG_CMD_INSTANCE_IREMOTE 表示注册间接处理的远程命令, 不带路径, 且经过MAC透传后处理的远程命令,SAL再调用给注册的HSO命令处理入口.
//                     HI_DIAG_CMD_INSTANCE_DREMOTE 表示注册直接处理的远程命令, 带路径,通过路径判断是否需要透传远程命令
//
// 返 回 值:
//    HI_ERR_SUCCESS : 处理成功, DIAG将返回给HSO,错误码为OK
//    其它为错误码
//
// 调用要求: 1. 在初始化阶段调用, 不支持多任务调用.
//           2. 最多能注册20个不同的命令表.
//           3. 由于命令处理是同步被调用, 且是串行的, 故要求注册后的命令处理入口运行时间尽量短, 不能堵塞其它命令接收, 目前设置处理超时为5s.
//           4. 不同实例ID注册命令ID可以相同, 同一实例注册的命令ID必须为唯一,否则注册失败.
// 调用举例:
//         参考 <<Hi3911V100 SAL接口说明书.docx>>
// 作    者: shenhankun/00130424 [2011-11-14]
//*****************************************************************************
HI_PRVL HI_U32 HI_DIAG_RegisterCommand(HI_CONST HI_DIAG_CMD_REG_OBJ_S* pstCmdTbl, HI_U16 usCmdNum, HI_U8 ucInstanceId)
{
    return HI_DIAG_RegisterCommandPrv(pstCmdTbl, usCmdNum, ucInstanceId);
}
#define HI_DIAG_RegiestCommand(pstCmdTbl, usCmdNum, ucInstanceId)    HI_DIAG_RegisterCommand(pstCmdTbl, usCmdNum, ucInstanceId)
#define HI_DIAG_RegiestInputCommand(pstCmdTbl, usCmdNum)             HI_DIAG_RegisterCommand(pstCmdTbl, usCmdNum, HI_DIAG_CMD_INSTANCE_DEFAULT)
//*****************************************************************************


//*****************************************************************************
// 函数名称: HI_DIAG_RegisterParallelCmd
// 功能描述: 注册运行并发的命令ID范围,
//           默认所有命令仅支持串行且前一个命令没有执行完毕(ACK没有回)则自动覆盖前一个命令数据.
//
// 参数说明:
//   usMinId [in] 命令ID最小值
//   usMaxId [in] 命令ID最大值
//
// 返 回 值:
//    HI_ERR_SUCCESS           : 成功
//    HI_ERR_NOT_FOUND         : 列表已满,无法注册,最多仅能注册4次.
//    HI_ERR_NO_INITILIZATION  : DIAG未初始化
//
// 调用要求: 1) DIAG初始化结束后生效; 2)仅限于在初始化过程中调用
// 调用举例:
//         HI_PVOID GetParallelCmdCache(HI_U16 usId, HI_U32 ulCacheSize, HI_U16 usPkSn) { return HI_MALLOC(0, ulCacheSize); }
//         HI_DIAG_RegisterParallelCmd(0x5700, 0x5701, GetParallelCmdCache)
//         在HI_DIAG_RegisterCommand 注册的控制接口中释放该内存
//         HI_PRV HI_CONST HI_DIAG_CMD_REG_OBJ_S g_stSimuCmdTbl[] = {
//         {ID_DIAG_CMD_CSIM_PACKET, ID_DIAG_CMD_SIMU_CTRL, CmdInputProc, CmdCtrlProc},
//         HI_PRV HI_U32 CmdInputProc(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption) {DIAG_CMD_CSIM_REQ_STRU* pstReq = (DIAG_CMD_CSIM_REQ_STRU*)pCmdParam;}
//         HI_PRV HI_U32 CmdCtrlProc(HI_U16 ulId, HI_U16 usPkSn, HI_PVOID pCache, HI_U32 ulOption){ HI_MDM_Free(0, pCache);}
// 作    者: s00130424/s00130424 [2013-01-25]
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
// 函数名称: HI_DIAG_SendAckPacket HI_DIAG_SendUserAckPacket
// 功能描述: 发送应答包给HSO
//
// 参数说明:
//   ulId [in] DIAG包括ID
//   pBuffer [in] buffer地址
//   usBufferSize [in] buffer大小
//   bSync [in] TRUE_T表示同步发送给HSO, 阻塞操作; FALSE_T表示异步(通过分配内存后, 由OS队列缓存在发送给HSO),非阻塞操作
//   hAckInstance [in] 通过 HI_DIAG_CreateAckInstance 接口 创建的ACK包实例, 如不需要则该参数为HI_NULL.
//
// 返 回 值:
//    HI_ERR_SUCCESS : 发送成功
//    其它为错误码
//
// 调用要求:
//         1. HI_DIAG_SendAckPacket 仅支持在 HI_DIAG_RegiestCommand 注册的 命令请求处理函数中使用
//           如果在其它函数中使用(比如自定义的任务中)，则需要在命令处理函数中先创建ACK包实例，后调用 HI_DIAG_SendUserAckPacket 给 HSO.
//         2. 仅支持本地发送应答包.
// 调用举例: TODO: ...
// 作    者: shenhankun/00130424 [2011-11-29]
//*****************************************************************************
HI_EAPI HI_U32 HI_DIAG_SendAckPacket(HI_U32 ulId, HI_IN HI_PVOID pBuffer, HI_U16 usBufferSize, HI_BOOL bSync);
#if defined(PRODUCT_CFG_SAL_API_SUPPORT)
#define HI_DIAG_SendAckPacket(ulId, pBuffer, usBufferSize, bSync) HI_DIAG_SendAckPacketPrv(ulId, pBuffer, usBufferSize, bSync, HI_NULL)
#endif
//*****************************************************************************

//*****************************************************************************
// 函数名称: HI_DIAG_CreateAckInstance
// 功能描述: 创建ACK包实例
//
// 参数说明:
//   ulId [in] DIAG包ID, 必须和HI_DIAG_RegiestInputCommand注册的命令请求处理函数对应的ID一致.
//
// 返 回 值:
//    非空: ACK包实例, 用于传入 HI_DIAG_SendUserAckPacket 接口参数 hAckInstance
//    空  : 内存不足/输入参数非法/没有在HI_DIAG_RegiestInputCommand注册的命令请求处理函数中使用
//
// 调用要求: 仅在 HI_DIAG_RegiestInputCommand 注册的 命令请求处理函数中使用, 否则返回 HI_NULL.
// 调用举例: TODO: ...
// 作    者: shenhankun/00130424 [2011-11-29]
//*****************************************************************************
HI_EAPI HI_HANDLE HI_DIAG_CreateAckInstance(HI_U32 ulId);

//*****************************************************************************
// 函数名称: HI_DIAG_ReportPacket HI_DIAG_SendUserPacket
// 功能描述: 发送数据包到HSO
//
// 参数说明:
//   ulId [in] 包ID
//   pucPacket [in] 数据包的buffer地址, 该函数不会释放该指针,如果不需要可以值为HI_NULL
//   usPacketSize [in] 数据包大小, 如果不需要可以值为0
//   bSync [in] HI_TRUE:不通过OS队列发送包, 仅适用于 HI_DIAG_RegiestInputCommand 注册的处理接口中;
//              HI_FASE:通过OS队列发送包, 预定义的缓冲(COMMAND Buffer不足)时候则自动启用动态分配方式.
//   ucInstanceId [in] 实例ID,HI_DIAG_CMD_INSTANCE_DEFAULT 表示本地命令,SAL直接调用给注册的HSO命令处理入口
//                     HI_DIAG_CMD_INSTANCE_IREMOTE 表示远程命令, 经过MAC透传后,SAL再调用给注册的HSO命令处理入口
//
// 返 回 值:
//    HI_ERR_SUCCESS: 成功
//    其它值表示错误码.
//
// 调用要求: 1. HSO通道可用; 2) 缓存足够;
//           3) 没有配置开关, 能一直能上报, 如果需要受开关限制,则需调用HI_DIAG_ReportCommand
//           4) HI_DIAG_SendUserPacket仅支持发送到OS队列
//           5) 通常情况下推荐使用 HI_DIAG_SendUserPacket 接口.
// 调用举例:
//  MY_PACKET_STRU stPacket = {0}
//  HI_DIAG_ReportPacket(0x1000, HI_DIAG_CMD_INSTANCE_LOCAL, &stPacket, sizeof(MY_PACKET_STRU), HI_FALSE)
//  HI_DIAG_SendUserPacket(0x1000, &stPacket, sizeof(MY_PACKET_STRU))
// 作    者: shenhankun/00130424 [2012-01-04]
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
// 函数名称: HI_DIAG_ReportStatObj
// 功能描述: 上报统计量到队列中, 后台转发给HSO
//
// 参数说明:
//   ulId [in] 统计量 ID, 取之范围 [HI_DFX_STAT_ID_BASE_SYS, HI_DFX_STAT_ID_MAX_SYS)
//   pStatObj [in] 统计量对象,通常为全局变量
//   usStatObjSize [in] 统计量对象长度
//
// 返 回 值:
//    HI_ERR_SUCCESS: 成功
//    其它值表示错误码.
//
// 调用要求:
// 调用举例:
// 作    者: shenhankun/00130424 [2012-01-04]
//*****************************************************************************
HI_EAPI HI_U32 HI_DIAG_ReportStatObj(HI_U16 usId, HI_PVOID pStatObj, HI_U16 usStatObjSize);
#if defined(PRODUCT_CFG_SAL_API_SUPPORT)
#define HI_DIAG_ReportStatObj(usId, pStatObj, usStatObjSize) HI_DIAG_ReportStatObjPrv(usId, pStatObj, usStatObjSize)
#else
HI_PRVL HI_U32 HI_DIAG_ReportStatObj(HI_U16 usId, HI_PVOID pStatObj, HI_U16 usStatObjSize) {return HI_ERR_NOT_SUPPORT;}
#endif
#endif

//*****************************************************************************
// 函数名称: HI_DIAG_ReportXxxEvent
// 功能描述: 系统初始化事件上报系统错误/告警/关键事件上报
//           HI_DIAG_ReportSysInitEvt       : 缓存间接接上报到端口(非阻塞式上报, 可用于中断/任务)
//           HI_DIAG_ReportSysInitEvtRc     : 缓存间接接上报到端口(非阻塞式上报, 可用于中断/任务)
//           HI_DIAG_ReportSysEvt           : 缓存间接接上报到端口(非阻塞式上报, 可用于中断/任务)
//           HI_DIAG_ReportSysCriticalEvent : 无缓存直接上报到端口(阻塞式上报, 仅用于任务)
// 参数说明:
//           usInitEvtId [in] 表示事件ID, 取值限定为 HI_DEID_SYS_INIT_E 的定义
//           usSysEvtId [in] 表示事件ID, 取值限定为 HI_DEID_SYS_E 的定义
//           ucOption [in] HI_DIAG_SYS_INIT_SET 表示设置事件, HI_DIAG_SYS_INIT_RESET 表示清除事件, 默认为HI_DIAG_SYS_INIT_SET1.
//           usRc [in] 错误码,在NO列中显示
//           ulData [in] 自定义数据,在NO列中显示
// 返 回 值:
//           HI_ERR_SUCCESS           表示成功
//           HI_ERR_QUEUE_FULL        表示队列满
//           HI_ERR_NO_INITILIZATION  表示队列未初始化
//           HI_ERR_FAILURE           表示发生未知错误
//           HI_ERR_CFG_NOT_ALLOW     日志不允许上报
//           HI_ERR_INVALID_PARAMETER 传参非法
//           HI_ERR_NOT_ENOUGH_MEMORY 内存分配失败
// 调用要求:
// 调用举例: HI_DIAG_ReportSysInitEvent(LOG_EVT_SHUT_DOWN)
// 作    者: 郭成钢/00149569/20090210
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
// 函数名称: HI_DIAG_ReportSysMessage
// 功能描述: 系统诊断信息上报
//
// 参数说明:
//   usId [in] 诊断包ID.
//   usSrcModId [in] 源模块ID 0表示不指定模块ID
//   usDestModId [in] 目的模块ID, 0表示不指定模块ID
//   pPacket [in] 诊断包buffer大小.
//   usPacketSize [in] 诊断包buffer大小.
//   ulOption [in] 诊断选项, 取值为 以下任何一个值.
//                 a) HI_SYS_DMID_BUSINESS_L0, HI_SYS_DMID_BUSINESS_L1, HI_SYS_DMID_BUSINESS_L2
//                 b) HI_SYS_DMID_NET_L0, HI_SYS_DMID_NET_L1,HI_SYS_DMID_NET_L2
//                 c) HI_SYS_DSID_BUSINESS_L0, HI_SYS_DSID_BUSINESS_L1, HI_SYS_DSID_BUSINESS_L2
//
// 返 回 值:
//    HI_ERR_SUCCESS: 成功
//    其它值表示错误码.
//
// 调用要求:
// 调用举例:
// 作    者: shenhankun/00130424 [2012-01-04]
//*****************************************************************************
#if defined(PRODUCT_CFG_SAL_API_SUPPORT) && !defined(PRODUCT_CFG_CORE_TYPE_BOOTLOADER)
HI_EAPI HI_U32 HI_DIAG_ReportSysMessage(HI_U16 usId, HI_U16 usSrcModId, HI_U16 usDestModId, HI_PVOID pPacket, HI_U16 usPacketSize, HI_U32 ulOption);
#define HI_DIAG_ReportSysMessage(usId, usSrcModId, usDestModId, pPacket, usPacketSize, ulOption) HI_DIAG_ReportSysMessagePrv(usId, usSrcModId, usDestModId, pPacket, usPacketSize, ulOption)
#else
HI_PRVL HI_U32 HI_DIAG_ReportSysMessage(HI_U16 usId, HI_U16 usSrcModId, HI_U16 usDestModId, HI_PVOID pPacket, HI_U16 usPacketSize, HI_U32 ulOption) { return HI_ERR_NOT_SUPPORT;}
#endif

#if defined(PRODUCT_CFG_FEATURE_NDM) && !defined(PRODUCT_CFG_CORE_TYPE_BOOTLOADER) && !defined(SAL_HAVE_FEATURE_UT)
//*****************************************************************************
// 函数名称: HI_DIAG_RegisterSysStat
// 功能描述: 注册系统诊断统计量
//
// 参数说明:
//   pstSysStatObjTbl [in] 统计量对象列表, 包含对象个数最大为 2048 个, 详见文档<<Hi3911V100 软件系统级诊断方案设计说明书.docx>>说明
//   usModId [in] 统计量所在的模块ID.
//   ulOption [in] 诊断选项, 详见文档<<Hi3911V100 软件系统级诊断方案设计说明书.docx>>说明
//   pStatPacket [in] 统计量指针(必须是全局变量指针或是静态变量指针)
//
// 返 回 值:
//    HI_ERR_SUCCESS: 成功.
//    其它值表示错误码.
//
// 调用要求:
// 调用举例:
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
// 作    者: shenhankun/00130424 [2012-01-04]
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
// 函数名称: HI_MAC_QueryInfo
// 功能描述: 获取MAC层全局的相关信息
//
// 参数说明:
//   pstInfo [out] : HI_MAC_CONFIG_INFO_S 结构指针
//
// 返 回 值:
//    HI_ERR_SUCCESS : 表示成功
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
//*****************************************************************************
HI_EAPI HI_U32 HI_MAC_QueryInfo(HI_OUT HI_MAC_CONFIG_INFO_S* pstInfo);


//****************************************************************************
// 用户数据存储接口
//****************************************************************************

//****************************************************************************
// 函数名称: HI_SYS_SaveUserData
// 功能描述: 将用户数据保存到的用户数据区中,一次调用可以保存多项数据.
//           通信模块掉电重启后通过HI_SYS_GetUserData可获取存储的用户数据
//
// 参数说明:
//   ulUserDataId [in] 用户数据ID， 一个用户数据ID可以对应多个数据项，通过ulIdx标记，取值如下.
//                    1) HI_USERDATA_ID_APP  表示保存应用的数据项, 每项大小必须相等(固定大小)
// 	                  2) HI_USERDATA_ID_BIDB 表示保存模块的基础信息库，pData对应HI_MAC_ND_BIDB_ITEM_S结构格式的数据,每项大小必须相等(固定大小)
//   ulItemOffset [in] 用户数据项起始索引, 当ulUserDataId对应多项数据项时，使用该参数表示不同项, 这时该参数取值必须从0开始递增.
//   usItemSize [in] ulUserDataId对应的每个数据项的固定长度.
//   ulItemNum [in] 待保存的数据项条数
//   pItems [in] ulUserDataId对应数据起始地址
//
// 返 回 值:
//    HI_ERR_SUCCESS           : 成功.
//    HI_ERR_FULL              : 存储满,无法继续保存数据项
//    HI_ERR_NOT_SUPPORT       : ulUserDataId=HI_USERDATA_ID_BIDB 时候,不支持终端模块.
//    HI_ERR_INVALID_PARAMETER : pData参数或是usDataSize非法，如为空参数.
//    HI_ERR_NO_INITILIZATION  : SAL未初始化完成.
//    其它错误码为Flash驱动的返回值.
//
// 调用要求:
//    1) ulUserDataId取值为 HI_USERDATA_ID_BIDB或是HI_USERDATA_ID_APP 时, 该接口仅支持CCO,不支持STA;
//    2) 如果数据项（某个索引）已经存在，则直接覆盖;
//    3) 每次读写的Item大小必须是固定大小.
//    4) 不支持擦除均衡/掉电恢复 特性;
//****************************************************************************
HI_EAPI HI_U32 HI_SYS_SaveUserData(HI_U8 ulUserDataId, HI_U32 ulItemOffset, HI_U32 ulItemNum, HI_U16 usItemSize, HI_IN HI_PVOID pItems);

//****************************************************************************
// 函数名称: HI_SYS_GetUserData
// 功能描述: 获取用户数据
//
// 参数说明:
//   ucUserDataId [in] 用户数据ID， 一个用户数据ID可以对应多个数据项，通过ulIdx标记，取值如下.
//                    1) HI_USERDATA_ID_APP  表示保存应用的数据项, 每项大小必须相等(固定大小)
// 	                  2) HI_USERDATA_ID_BIDB 表示保存模块的基础信息库，pData对应HI_MAC_ND_BIDB_ITEM_S结构格式的数据,每项大小必须相等(固定大小)
//   ulItemOffset [in] 用户数据项起始索引, 当ulUserDataId对应多项数据项时，使用该参数表示不同项, 这时该参数取值必须从0开始递增.
//   usItemSize [in] ulUserDataId对应的每个数据项的固定长度.
//   ulItemNum [in] 待读取的数据项条数
//   pusItemsRead [in] 实际读取的数据项条数,小于等于 ulItemNum, 如果pItemsBuffer为HI_NULL则该参数返回用户数据项个数
//   pItemsBuffer [out] ulUserDataId对应数据buffer. pItemsBuffer=HI_NULL, 通过 pusItemsRead返回已经存储的总item个数.
//
// 返 回 值:
//    HI_ERR_SUCCESS: 成功.
//    HI_ERR_NOT_SUPPORT : 终端模块，不支持该特性.
//    HI_ERR_NOT_EXIST   : 表示指定的用户数据不存在.
//    HI_ERR_NOT_FOUND   : 输入参数 ulItemOffset 不存在，表示用户数据不存在指定的数据项.
//    HI_ERR_INVALID_PARAMETER : pData参数或是usDataSize非法，如为空参数.
//    HI_ERR_NO_INITILIZATION  : SAL未初始化完成.
//    其它错误码为Flash驱动的返回值.
//
// 调用要求:
//    1) ulUserDataId取值为 HI_USERDATA_ID_BIDB或是HI_USERDATA_ID_APP时, 该接口仅支持CCO,不支持STA;
//    2) 每次读写的Item大小必须是固定大小.
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
}DIAG_RESTART_TYPE;//TODO:这地方用宏可能更好些

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
// 函数名称: HI_SYS_GetProductVerInfo HI_SYS_GetUeVerInfo
// 功能描述: 获取产品版本信息包括 芯片名称, 版本号等. 字符串格式
//
// 参数说明:
//   无
//
// 返 回 值:
//    非空: 表示产品版本信息
//
// 调用要求:
//    1) 无
//****************************************************************************
HI_EAPI HI_CONST HI_PRODUCT_INFO_S* HI_SYS_GetProductVerInfo(HI_VOID);
HI_EAPI HI_CONST HI_UE_PRODUCT_VER_S* HI_SYS_GetUeVerInfo(HI_VOID);
//****************************************************************************


//****************************************************************************
#if defined(PRODUCT_CFG_FEATURE_SOFT_PRODUCT_ID)
typedef struct
{
    HI_NV_FTM_PRODUCT_ID_S stProductId;
    HI_U32 bEnable; // 对齐需要, 声明为HI_U32, 如果需要增加变量，则可以调整该类型
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
// 函数名称: HI_SYS_GetSysInfo
// 功能描述: 获取系统信息, 包括 系统状态、版本号等
//
// 参数说明:
//  无
//
// 返 回 值:
//    None
//
// 调用要求: 系统状态信息中部分内容不是静态的，需要实时调用该接口获取系统状态信息以便同步.
// 调用举例: TODO: ...
// 作    者: s00130424/s00130424 [2012-08-02]
//****************************************************************************
HI_EAPI HI_SYS_STATUS_INFO_S* HI_SYS_GetSysInfo(HI_VOID);
//*****************************************************************************
// 函数名称: HI_SYS_GetNvVerInfo
// 功能描述: 获取NV VR版本号函数(为了兼容性补充新增结构体)
//
// 参数说明:
//  无
//
// 返 回 值:
//    None
//
// 调用要求: .系统状态信息中部分内容不是静态的，需要实时调用该接口获取系统状态信息以便同步.
// 调用举例: TODO: ...
// 作    者: b00208046[2014-07-17]
//****************************************************************************

HI_EAPI HI_SYS_NV_VER_INFO_S* HI_SYS_GetNvVerInfo(HI_VOID);

HI_EAPI HI_VOID SAL_SetBootVerIdx(HI_U8 ucBootVerIdx);

//****************************************************************************


//****************************************************************************
// 设置/查询 MAC、IP、掩码、网关等
// pszName : 取值为 "eth0"或是"eth1", 不支持其它值
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
// 函数名称: HI_DIAG_SendNdmReqFrame
// 功能描述: 通过抄控器通道发送用户帧 unicast
//
// 参数说明:
//   uscFrameId [in] 帧ID, 可以为DIAG包ID.
//   pucFrame [in] 帧buffer, 该函数不会释放该指针.
//   uscFrameSize [in] 帧buffer大小.
//   aucMacAddr [in] 通信模块路径. 参见 <<Hi3911V100 抄控器软件接口设计说明书.docx>>文档说明.
//   ulSn [in] 帧SN,如不需要则值为0
//
// 返 回 值:
//    HI_ERR_SUCCESS: 成功
//    其它值表示错误码.
//
// 调用要求:
// 调用举例:
// 作    者: shenhankun/00130424 [2012-01-04]
//*****************************************************************************
#if !defined(PRODUCT_CFG_CORE_TYPE_BOOTLOADER)
HI_EAPI HI_U32 HI_DIAG_SendNdmReqFrame(HI_U16 uscFrameId, HI_PBYTE pucFrame, HI_U16 uscFrameSize, HI_U8 aucMacAddr[HI_PLC_RELAY_NUM_MAX][HI_PLC_MAC_ADDR_LEN], HI_U32 ulSn);
HI_EAPI HI_U32 HI_DIAG_SendNdmFrame(HI_U16 usPacketId, HI_PBYTE pucPacket, HI_U16 usPacketSize, HI_U8 aucMacAddr[HI_PLC_RELAY_NUM_MAX][HI_PLC_MAC_ADDR_LEN], HI_U32 ulSn);
#endif

#if !defined(PRODUCT_CFG_CORE_TYPE_BOOTLOADER)
//****************************************************************************
// 获取系统状态 HI_SYS_STATUS_IND_S
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
// 检测指定的DIAG端口是否连接
// 返回值: HI_TRUE表示DIAG通道连接; HI_FALSE表示无连接
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
HI_VOID HI_SYS_RegisterWdMonitorTaskHook(WD_MONITOR_TASK_HOOK func);//注册看门狗任务钩子函数
hi_u8 HI_SYS_GetChipMode(hi_void);

HI_END_HEADER
#endif // __HI_SAL_H__

