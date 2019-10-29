//*****************************************************************************
//
//                  版权所有 (C), 2001-2011, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : os_vx_sal_api.h
//  版 本 号   : V1.0 
//  作    者   : 沈汉坤/00130424
//  生成日期   : 2011年09月06日
//  功能描述   : NU OS 适配接口文件
//  函数列表   : 无
//  修改历史   : 
//  1.日    期 : 2011年09月06日
//    作    者 : 沈汉坤/00130424
//    修改内容 : 创建文件 
//*****************************************************************************

#ifndef __OS_VX_SAL_API_H__
#define __OS_VX_SAL_API_H__

#if defined(PRODUCT_CFG_OS_VX)
#include <hi_mdm_types.h>
#include <los_typedef.h>
//#include <hisoc/irq.h>
//#define size_t        unsigned int

/* BEGIN: Added by wuxianfeng 190206, 2013/2/20   PN:DTS2013022006448*/
// 
// 设置任务时间片，以tick为单位
// if ticks = 0 timeslice is unable ;
//
HI_U32 SAL_TASK_TIMESLICE(HI_S32 ticks);
/* END:   Added by wuxianfeng 190206, 2013/2/20 */




HI_U32 SAL_SYS_GET_SP(HI_VOID);
/* BEGIN: Added by chenghuanhuan , 2013/06/24   PN:DTS2013061704954*/
HI_VOID SAL_DISABLE_MMU (HI_VOID);
/* END: Added by chenghuanhuan , 2013/06/24   PN:DTS2013061704954*/
#define TASK_NAME_SIZE_MAX 15
/* END:   Added by b00208046, 2014/5/10 */
// 
// 死机信息 OS 回调
// eid              :异常号 (0, 0xFF)
// evn              :异常中断号 : 中断发生异常的中断号,无中断发生异常, 则填为 0xFFFFFFFF, 表示任务异常.
// esp              :发生异常当时所在的栈指针
// epc              :发生异常当时所在的PC
// ttfa             : 最后运行的任务栈回溯数组
// 死机 ttfa[0] <-- ttfa[1]  <-- ...  <-- ttfa[9]
// 
// 注意: 
// 该接口由OS调用，SAL实现 
//
typedef struct 
{
    HI_U32  eid;
    HI_U32  evn;
    HI_U32  esp;
    HI_U32  epc;
    HI_U32  task_status;
    HI_CHAR task_name[TASK_NAME_SIZE_MAX+1];
    HI_U32  task_stk_size;
    HI_U32  atcstk[2]; // task stack region : [atcstk[0], atcstk[1]]
    HI_U32  ttfa[HI_SYS_TT_FUNC_MAX_NUM];
    HI_U32  r[15];
    HI_U32  pc;
    HI_U32  cpsr;
    HI_U32  ttbase;
}HI_SYS_EXCEPTION_INFO_S;

HI_VOID HI_SYS_SetExceptionInfo(HI_SYS_EXCEPTION_INFO_S* pstSysExpInfo); // 上层提供， VX调用 
HI_SYS_EXCEPTION_INFO_S* HI_SYS_GetExceptionInfo(HI_PVOID pTask);

// 该接口由OS调用，SAL实现 
HI_VOID  SAL_ApplicationInitialize(HI_PVOID mem_pool, HI_PVOID uncached_mem_pool, HI_U32 ulSize);


// ETH - SOCKET API
HI_S32 SAL_SocketSendTo(HI_SOCKET s, HI_PBYTE buf, HI_S32 len, HI_PCSTR pucIp, HI_U16 usPortNum);
HI_S32 SAL_SocketRecvFrom(HI_SOCKET s, HI_OUT HI_PBYTE buf, HI_S32 len);
HI_S32 SAL_SocketConnect(HI_SOCKET s, HI_PCSTR pucIp, HI_U16 usPortNum);
HI_SOCKET SAL_SocketAccept(HI_SOCKET s);
HI_SOCKET SAL_SocketTcpCreate(HI_VOID);
HI_SOCKET SAL_SocketUdpCreate(HI_VOID);
HI_U32 SAL_SocketSetKeepAlive(HI_SOCKET s, HI_S32 slEnable, HI_S32 slTcpKeepIdle, HI_S32 slTcpKeepCnt);
HI_U32 SAL_SocketSetLinger(HI_SOCKET s);
HI_U32 SAL_SocketSetRcvTimeout(HI_SOCKET s,HI_U32 timeval_sec);
HI_S32 SAL_SocketBind(HI_SOCKET s, HI_PCSTR pucIp, HI_U16 usPortNum); // HI_ERR_SUCCESS 表示成功其它失败
HI_S32 SAL_SocketListen(HI_SOCKET s, HI_S16 backlog);
HI_S32 SAL_SocketSend(HI_SOCKET s, HI_PBYTE buf, HI_S32 len);
HI_S32 SAL_SocketShutdown(HI_SOCKET s, HI_S32 how);
HI_S32 SAL_SocketClose(HI_SOCKET s);
HI_S32 SAL_Recv(HI_SOCKET s, HI_OUT HI_PBYTE buf, HI_S32 len);
HI_VOID SAL_SocketSetFd(HI_SOCKET s, HI_SYS_SOCKET_FD* fd);
HI_BOOL SAL_SocketIsSetFd(HI_SOCKET s, HI_SYS_SOCKET_FD* fd);
HI_U32 SAL_SocketSelect(HI_S32 nfds, HI_SYS_SOCKET_FD* readfds, HI_SYS_SOCKET_FD * writefds, HI_U32 ulTimeOut);
/* BEGIN: Added by b00208046, 2015/1/31   问题单号:NDM3_ICC*/
HI_SOCKET SAL_SocketCreateOnly(HI_VOID);
HI_S32 SAL_SocketSendTimeOut(HI_SOCKET s, HI_PBYTE buf, HI_S32 len,HI_U16 sec);
HI_U32 HI_SYS_SocketConnectTimeOut(HI_SOCKET s, HI_PCSTR pucIp, HI_U16 usPortNum,HI_U32 sec);
/* END: Added by b00208046, 2015/1/31   问题单号:NDM3_ICC */

/* BEGIN: Added by x00180816, 2013/10/30   问题单号:DTS2013110701913 */
HI_U32 SAL_SetMtu(HI_IN const char * ucIfName,HI_U32 ulMtuSize);
/* END:   Added by x00180816, 2013/10/30 */
HI_U32 SAL_SetEthCfg(HI_IN char * ucIfName,HI_IN HI_PVOID pCmdParam ,HI_U16 usCmdParamSize);
HI_U32 SAL_GetEthCfg(HI_IN char * ucIfName,HI_IN HI_PVOID pCmdParam ,HI_U16 usCmdParamSize);
HI_S32 SAL_SocketGetOtherSizeAddr(HI_SOCKET s,HI_U32 *pAddr);//IPV4
#if defined(PRODUCT_CFG_TASK_INT_SYS_ERR_INFO)&&defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
HI_U32 SAL_Copy_OsInfo(HI_VOID);
#endif
HI_U32 SAL_GetDeafultGateway(HI_PCSTR pszName, HI_U8 ucGateway[HI_IPV4_ADDR_LEN]);

HI_U32 SAL_SetDeafultGateway(HI_PCSTR pszName, HI_U8 ucGateway[HI_IPV4_ADDR_LEN]);

HI_BOOL SAL_OSGetTaskPointer(HI_OUT HI_PVOID* pTask, HI_U32 idx);

// 参数为HI_NULL表示不需要该参数返回 
HI_U32 SAL_OSGetTaskInfo(HI_PVOID pTask, HI_CHAR name[16], 
                                    HI_U32 *status, HI_U32 *scheduled_count,
                                    HI_U32 *priority, HI_U32 *tsp,
                                    HI_U32 *time_slice, HI_PVOID *stack_base,
                                    HI_U32 *stack_size, HI_U32 *minimum_stack);

HI_CONST HI_CHAR* SAL_OSGetTaskStatusText(HI_U32 status); // status value --> status text, max. 12 bytes.
/* BEGIN: Added by wuxianfeng 190206, 2012/11/14   PN:DTS2012110607506*/
/*add by zhanglei for vxworks*/
HI_U32 SAL_SetPlcCfg(HI_IN HI_U8 * pIpAddr, HI_IN HI_U8 * pNetMask, HI_IN HI_U8 * pMacAddr);
HI_VOID SAL_netPktFreeFuncReg(HI_PVOID func);
HI_VOID SAL_netPktCreate(HI_PVOID ipcomPkt, HI_PVOID data, HI_U32 pktSize, HI_PVOID bufFree);
HI_VOID SAL_netPktDataGet(HI_PVOID ipcomPkt, HI_U8 **data);
HI_VOID SAL_netPktInput(HI_PVOID netif, HI_PVOID ipcomPkt, HI_S32 dataStart, HI_S32 dataEnd);
HI_VOID SAL_netPktIpStartGet(HI_PVOID ipcomPkt, HI_S32 *ipStart);



#define PLC_IP_SIOCSIFLLADDR        (0xE001)
#define PLC_IP_SIOCGIPLLADDR        (0xE002)
HI_S32  SAL_ioctlCmdMap(HI_S32 cmd);
HI_U32 SAL_SetEthEnable(HI_BOOL bEnable);
HI_VOID SAL_routeInit(HI_VOID);

HI_U32 SAL_FindTargetAddr(HI_IN HI_U32  ipAddr);
/* END:   Added by wuxianfeng 190206, 2012/11/14 */

HI_S32 HI_MDM_DataCacheInvalidate(HI_PVOID pAddress, HI_S32 lSize);
HI_S32 HI_MDM_DataCacheFlush(HI_PVOID pAddress, HI_S32 lSize);
#ifdef PRODUCT_CFG_PRODUCT_TYPE_NDM
//****************************************************************************
// 函数名称: SAL_StartFastHrTimer
//           
// 功能描述: 启动hrtimer
//           
//
// 参数说明:
//  pTimeHandle:定时器TIMER Handle
//  pfnTimerRoutine:HISR回调函数
//  pfnLisrTimerRoutine:LISR回调函数
//  ulTimeOut:超时值us
//  usr_data:用户数据
//
// 返 回 值:
//  HI_ERR_SUCCESS: 成功
//  
// TODO: 该接口非通用接口,仅用于台区识别定时器。由于启动定时器时没有重置attribute,要求pTimeHandle只能用于一个定时功能,不可分时用在多个定时功能。
//****************************************************************************
HI_U32 SAL_StartFastHrTimer(TIMER_HANDLE_T* pTimeHandle, HI_PVOID pfnTimerRoutine,HI_PVOID pfnLisrTimerRoutine, HI_U32 ulTimeOut,HI_U32 usr_data);
#else
HI_PRVL HI_U32 SAL_StartFastHrTimer(TIMER_HANDLE_T* pTimeHandle, HI_PVOID pfnTimerRoutine,HI_PVOID pfnLisrTimerRoutine, HI_U32 ulTimeOut,HI_U32 usr_data){return HI_ERR_NOT_SUPPORT;}
#endif

#endif
#endif // __OS_VX_SAL_API_H__


