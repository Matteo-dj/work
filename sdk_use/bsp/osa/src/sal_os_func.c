/* Sal_os_vx.c - vxworks interface functins for Hisilicon */

/*
 * Copyright (c) 1992,1993,1995-1998,2001,2004-2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history

*/

#ifndef __SAL_OS_H__
#define __SAL_OS_H__

#include <los_exc.h>
#include <los_task.h>
#include <los_swtmr.h>
#include <los_memory.h>
#include <los_base.h>
#include <los_queue.h>
#include <los_sem.h>

#include <hi_types.h>
#include <hi_sal.h>
#include "os_vx_sal_api.h"
#include <sal_common.h>//先包含sal_common.h在包含dfx_sal.h代码才能编译通过

#include "dfx_sal.h"
#include <hi_sal_nv.h>
#include <time.h>
#include <sockets.h>
#include <netif.h>
#include <sal_os_dfx.h>
#include <interrupt.h>
#include <hi_platform.h>
#include <hi_driver_internal.h>
#include <sal_common.h>
#include <hi_sal_cfg.h>
#include <hi_sal.h>
#include <hi_drv_com.h>
#include <hi_init.h>

#define VX_GIVE_HI_TOTAL_HEAP_SIZE PRODUCT_CFG_RAM_TOTAL_HEAP_SIZE
#define ARM_IRQ_FIQ_IN_CPSR (0xC0)
#define EXC_IN_ISR      (0x55555555)
#define EXC_IN_TASK     (0xFFFFFFFF)
#define EXC_HOOK_ERR    (0x66666666)

extern HI_U8 g_aucSysVal[];

#define VX_TIMER_USED 0x55534544    /*ASIC are 'USED'*/
#define VX_TIMER2_PARA 0x50415241    /*ASIC are 'PARA'*/

HI_SYS_EXCEPTION_INFO_S SAL_OSExcInfo;
static TSK_INFO_S s_stCrashTaskInfo;//死机时任务信息


void bsp_init_early(void)
{
    (*(volatile unsigned *)(HI_VIC_BASE+HI_VIC_OFFSET_IRQ_PN+4*0))=0x3; /*lint !e10 !e40 !e63*/

}
void board_config(void)
{
    bsp_init_early();
    (HI_VOID)APP_InitCfgEarly();    
    HI_CFG_SetPhaseEnd();
}




/* END:   Added by w00190206, 2013/8/19 */
extern HI_U32 __svc_stack_top;
extern HI_U32 __irq2_stack_top;


HI_VOID SAL_OSExcHook(HI_U32 vecNum, EXC_CONTEXT_S* pEsf)
{
    HI_U32 ret;
    HI_U32 tid;
    TSK_INFO_S *pTaskInfo = &s_stCrashTaskInfo;
    EXC_CONTEXT_S *pExcContext = pEsf;  
    HI_SYS_EXCEPTION_INFO_S *pExcInfo = &SAL_OSExcInfo;
    
    SAL_DISABLE_MMU();
    if(vecNum<=4)
    {
        pExcInfo->eid = (vecNum*4);
/*
            【屏蔽人】z00193780
            【审核人】w00190206
            【代码分析】vecNum从操作系统中的汇编函数中获取，可能执行该分支，需要保留，误报。
            【功能影响】不影响正常业务，屏蔽告警
 */
    }
    else
    {
        pExcInfo->eid = vecNum;
    }

    //test_crash_print_info_1(vecNum,pEsf);

    if (HI_NULL != pExcContext)
    {
        SAL_SdmSysErrSetSpInfo((pExcContext->uwR[13]),(pExcContext->uwR[15]));
        pExcInfo->esp = pExcContext->uwR[13];
        pExcInfo->epc = pExcContext->uwR[15];
        pExcInfo->cpsr   = pExcContext->uwCPSR;
        pExcInfo->pc     = pExcContext->uwR[15];
        pExcInfo->ttbase = 0x123;//Debug,liteos暂未用
        memcpy_s(pExcInfo->r, sizeof(pExcInfo->r), &(pExcContext->uwR[0]), sizeof(pExcInfo->r));
        if ((vecNum==0xF5&&g_vuwIntCount>1) ||(vecNum!=0xF5&&g_vuwIntCount>0))
        {
            pExcInfo->evn = EXC_IN_ISR;
            strncpy_s(pExcInfo->task_name, sizeof(pExcInfo->task_name), "isr", TASK_NAME_SIZE_MAX);
            pExcInfo->task_stk_size = (HI_U32)&__irq2_stack_top-(HI_U32)&__svc_stack_top;
            pExcInfo->task_status = 0;
            pExcInfo->atcstk[0] = (HI_U32)&__svc_stack_top;
            pExcInfo->atcstk[1] = (HI_U32)&__irq2_stack_top;
        }
        else
        {
            pExcInfo->evn = EXC_IN_TASK;
            tid = LOS_CurTaskIDGet();
            if (LOS_ERRNO_TSK_ID_INVALID != tid)
            {
                ret = LOS_TaskInfoGet(tid, pTaskInfo);
                if (HI_ERR_SUCCESS == ret)
                {
                    strncpy_s(pExcInfo->task_name, sizeof(pExcInfo->task_name), pTaskInfo->acName, TASK_NAME_SIZE_MAX);
                    pExcInfo->task_stk_size = pTaskInfo->uwStackSize;
                    pExcInfo->task_status = pTaskInfo->usTaskStatus;
                    pExcInfo->atcstk[0] = pTaskInfo->uwTopOfStack;
                    pExcInfo->atcstk[1] = pTaskInfo->uwBottomOfStack;                    
                }
            }
        }        
    }
    else
    {
        pExcInfo->evn = EXC_HOOK_ERR;
    }

    //test_crash_print_info_2(pExcInfo);
    HI_SYS_SetExceptionInfo(pExcInfo);

    return;
}

HI_U32 SAL_OSExcHookAdd(HI_VOID)
{  
    return LOS_ExcRegHook(SAL_OSExcHook);
}

HI_BOOL SAL_IsEthEnable(HI_VOID)
{
    return HI_TRUE;
}


#if defined(PRODUCT_CFG_TEST_BSP)&&defined(PRODUCT_CFG_TEST_DRV_CRASH)
HI_EXTERN    HI_U32 s_test_crash_pmalloc ;
#endif

/*socket adapter*/
HI_S32 SAL_SocketSendTo(HI_SOCKET s, HI_PBYTE buf, HI_S32 len, HI_PCSTR pucIp, HI_U16 usPortNum)
{
    struct sockaddr_in  dest;
    if (!pucIp)
    {
        return HI_ERR_INVALID_PARAMETER;
    }
    
    if(s == HI_SYS_INVALID_SOCKET)
    {
        return HI_SYS_SOCKET_ERROR;
    }

    memset_s((HI_VOID*)&dest,sizeof(dest), 0,sizeof(dest));
    if(0 == inet_aton(pucIp,(struct in_addr *)(&dest.sin_addr.s_addr)))
    {
        return (HI_S32)HI_ERR_FAILURE;
    }

    dest.sin_family = AF_INET;
    dest.sin_port   = htons(usPortNum);

    return sendto(s, buf, (HI_U32)len, 0, (struct sockaddr*)&dest, sizeof(dest));
}

HI_S32 SAL_SocketRecvFrom(HI_SOCKET s, HI_OUT HI_PBYTE buf, HI_S32 len)
{
    struct sockaddr_in from = { 0 };
    int nFromlen     = sizeof(from);
    if(s == HI_SYS_INVALID_SOCKET)
    {
        return HI_SYS_SOCKET_ERROR;
    }
    return recvfrom(s, (HI_PVOID)buf, (HI_U32)len, 0, (struct sockaddr*)&from, (socklen_t*)&nFromlen);
}

HI_S32 SAL_SocketConnect(HI_SOCKET s, HI_PCSTR pucIp, HI_U16 usPortNum)
{
    struct sockaddr_in server;

    if (!pucIp)
    {
        return HI_ERR_INVALID_PARAMETER;
    }
    
    if (s == HI_SYS_INVALID_SOCKET)
    {
        return HI_SYS_SOCKET_ERROR;
    }

    memset_s((HI_VOID *)&server, sizeof(server), 0, sizeof(server));
    server.sin_family      = AF_INET;
    if(0 == inet_aton(pucIp,(struct in_addr *)(&server.sin_addr.s_addr)))
    {
        return (HI_S32)HI_ERR_FAILURE;
    }
    server.sin_port        = htons(usPortNum);
    return connect(s, (struct sockaddr*)&server, sizeof(server));
}

HI_SOCKET SAL_SocketAccept(HI_SOCKET s)
{
    HI_SOCKET sck     = (HI_SOCKET)HI_SYS_SOCKET_ERROR;
#if defined(PRODUCT_CFG_TCP_KEEPALIVE_R2) && defined(PRODUCT_CFG_KEEPALIVE_WAIT)
    int enable = 1;
    int tcp_keep_idle;       /* set TCP_KEEPIDLE */
    int tcp_keep_cnt;        /* set TCP_KEEPCNT */

    int tcp_keep_itvl;
#endif
    struct sockaddr_in from = { 0 };
    if(s == HI_SYS_INVALID_SOCKET)
    {
        return s;
    }

    HI_S32 len  = sizeof(struct sockaddr_in);
    sck = accept(s, (struct sockaddr*)&from, (socklen_t*)&len);


#if defined(PRODUCT_CFG_TCP_KEEPALIVE_R2) && defined(PRODUCT_CFG_KEEPALIVE_WAIT)
    if(sck>=0)
    {
        enable = 1;
        (void)setsockopt(sck, SOL_SOCKET, SO_KEEPALIVE, (void * )&enable, sizeof(enable));
        
        tcp_keep_idle = PRODUCT_CFG_TCP_KEEPALIVE_R2;  /* Send first keepalive probe when the connections been idle this time (in seconds)*/
        (void)setsockopt(sck, IPPROTO_TCP, TCP_KEEPIDLE, (char*)&(tcp_keep_idle), sizeof(tcp_keep_idle));
        
        
        tcp_keep_itvl = PRODUCT_CFG_TCP_KEEPINTVL;
        (void)setsockopt(sck, IPPROTO_TCP, TCP_KEEPINTVL, (char*)&(tcp_keep_itvl), sizeof(tcp_keep_itvl));
        
        
        tcp_keep_cnt = PRODUCT_CFG_KEEPALIVE_WAIT;  /* Maximum number of keepalives before dropping the connection */
        (void)setsockopt(sck, IPPROTO_TCP, TCP_KEEPCNT, (char*)&( tcp_keep_cnt), sizeof(tcp_keep_cnt));
        

    }

#endif
    return sck;    /*add by zhanglei for demo compiler*/
}
/* BEGIN: Added by b00208046, 2014/10/9   问题单号:DTS2014100900574  获取连接对方IP地址*/
HI_S32 SAL_SocketGetOtherSizeAddr(HI_SOCKET s,HI_U32 *pAddr)//IPV4
{
    HI_S32 ret = 0; 
    struct sockaddr_in from = { 0 };
    HI_S32 len  = sizeof(struct sockaddr_in);
    
    if(s == HI_SYS_INVALID_SOCKET)
    {
        return HI_SYS_SOCKET_ERROR;
    }
    
    if(pAddr==HI_NULL)
    {
        return HI_SYS_SOCKET_ERROR;
    }  
    
    ret = getpeername(s,(struct sockaddr*)&from,(socklen_t *)&len);
    if(ret!=HI_SYS_SOCKET_ERROR)
    {
        memcpy_s(pAddr, 4, &(from.sin_addr), 4);
    }
    
    return ret;
}


HI_SOCKET SAL_SocketCreateOnly(HI_VOID)
{
    return socket(AF_INET, SOCK_STREAM, 0);
}
//*****************************************************************************
// 函数名称: SAL_SocketSendTimeOut
// 功能描述: 超时发送数据
//           
// 参数说明:
//      s[in]:socket
//      buf[in]:超时时间(单位:秒)
//      len[in]:
//      ms[in]:
//
// 返 回 值: 
//     HI_ERR_SUCCESS:成功
//     其它:错误
//
// 调用要求: vxworks操作系统上支持
//
// 调用举例: 
// 作    者: 边海文20150205
//*****************************************************************************
//TODO :可以修改接口为setopt 的 SO_SNDTIMEO (chenghuanhuan)。
HI_S32 SAL_SocketSendTimeOut(HI_SOCKET s, HI_PBYTE buf, HI_S32 len,HI_U16 usSec)
{
    struct timeval tv;
    HI_S32 slSendLen = 0;
    tv.tv_sec = usSec;
    tv.tv_usec = 0;
    
    if(setsockopt(s,SOL_SOCKET,SO_SNDTIMEO,&tv,sizeof(tv))<0)
    {
        return (HI_S32)HI_ERR_FAILURE;
    }

    slSendLen = send((int) (s), buf, (HI_U32) len, 0);
    if(slSendLen < 0)
    {
        return (HI_S32)HI_ERR_FAILURE;
    }
    return slSendLen;

}

/* EBD: Added by b00208046, 2015/1/29   问题单号:NDM3_ICC*/
HI_SOCKET SAL_SocketTcpCreate(HI_VOID)//TODO:此函数修改的返回值
{
    HI_NV_SAL_CONFIG_S stSalConfigNv = {{0}, {0}, 0};
    int s = socket(AF_INET, SOCK_STREAM, 0);
#if defined(PRODUCT_CFG_TCP_KEEPALIVE_R2) && defined(PRODUCT_CFG_KEEPALIVE_WAIT)
    int enable = 1;
    int tcp_keep_idle;       /* set TCP_KEEPIDLE */
    int tcp_keep_cnt;        /* set TCP_KEEPCNT */

    int tcp_keep_itvl;
#endif
   
#ifdef PRODUCT_CFG_NDM_DEBUG_OPEN
    int rcv_bufsize = PRODUCT_CFG_TCP_RCV_BUFFER;
    if(s == HI_SYS_INVALID_SOCKET)
    {
        return s ;
    }
   
    if(HI_ERR_SUCCESS == HI_NVM_Read(HI_NV_SAL_CONFIG, &stSalConfigNv, sizeof(stSalConfigNv)))
    {
        rcv_bufsize = stSalConfigNv.tcpRecvBuf;//PRODUCT_CFG_TCP_RCV_BUFFER
    }
    (void)setsockopt(s, SOL_SOCKET, SO_RCVBUF, &rcv_bufsize, sizeof(rcv_bufsize));
#endif


#if defined(PRODUCT_CFG_TCP_KEEPALIVE_R2) && defined(PRODUCT_CFG_KEEPALIVE_WAIT)
    enable = 1;
    (void)setsockopt(s, SOL_SOCKET, SO_KEEPALIVE, (void * )&enable, sizeof(enable));

    tcp_keep_idle = PRODUCT_CFG_TCP_KEEPALIVE_R2;  /* Send first keepalive probe when the connections been idle this time (in seconds)*/
    (void)setsockopt(s, IPPROTO_TCP, TCP_KEEPIDLE, (char*)&(tcp_keep_idle), sizeof(tcp_keep_idle));


    tcp_keep_itvl = PRODUCT_CFG_TCP_KEEPINTVL;
    (void)setsockopt(s, IPPROTO_TCP, TCP_KEEPINTVL, (char*)&(tcp_keep_itvl), sizeof(tcp_keep_itvl));


    tcp_keep_cnt = PRODUCT_CFG_KEEPALIVE_WAIT;  /* Maximum number of keepalives before dropping the connection */
    (void)setsockopt(s, IPPROTO_TCP, TCP_KEEPCNT, (char*)&( tcp_keep_cnt), sizeof(tcp_keep_cnt));

#endif
    return (HI_SOCKET)s;
}

HI_SOCKET SAL_SocketUdpCreate(HI_VOID)
{
    return socket(AF_INET, SOCK_DGRAM, 0);
}

HI_U32 SAL_SocketSetKeepAlive(HI_SOCKET s, HI_S32 slEnable, HI_S32 slTcpKeepIdle, HI_S32 slTcpKeepCnt)
{
    HI_U32 ret = 0;
    if(s == HI_SYS_INVALID_SOCKET)
    {
        return (HI_U32)HI_SYS_SOCKET_ERROR;
    }

    ret |= (HI_U32)setsockopt(s, SOL_SOCKET, SO_KEEPALIVE, (void * )&slEnable, sizeof(slEnable));

    if (slEnable)
    {
        /* Send first keepalive probe when the connections been isdl this time (in seconds)*/
        ret |= (HI_U32)setsockopt(s, IPPROTO_TCP, TCP_KEEPIDLE, (void *)&(slTcpKeepIdle), sizeof(slTcpKeepIdle));

        /* Maximum number of keepalives before dropping the connection */
        ret |= (HI_U32)setsockopt(s, IPPROTO_TCP, TCP_KEEPCNT, (void *)&( slTcpKeepCnt), sizeof(slTcpKeepCnt));
    }

    return ret;
}

HI_U32 SAL_SocketSetLinger(HI_SOCKET s)
{
    return HI_ERR_NOT_SUPPORT;
}

/* BEGIN: Added by c00132816, 2013/9/23   问题单号:DTS2013091704842*/
HI_U32 SAL_SocketSetRcvTimeout(HI_SOCKET s,HI_U32 timeval_sec)
{
    struct timeval tv; 
    HI_U32  ret = HI_ERR_FAILURE;
    
    tv.tv_sec = (HI_S32)timeval_sec;
    tv.tv_usec = 0;
    
    if(s == HI_SYS_INVALID_SOCKET)
    {
        return (HI_U32)HI_SYS_SOCKET_ERROR;
    }

    ret = (HI_U32)setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (void *) &tv, sizeof(tv));
    if (ret  != ERR_OK)
    {
        return (HI_U32)HI_ERR_SYSTEM_CALL_ERROR;
    }
    else
    {
       return (HI_U32)HI_ERR_SUCCESS; 
    }
}
/* END:   Added by c00132816, 2013/9/23   问题单号:DTS2013091704842*/

HI_S32 SAL_SocketBind(HI_SOCKET s, HI_PCSTR pucIp, HI_U16 usPortNum)
{
    struct sockaddr_in local = { 0 };
    if(s == HI_SYS_INVALID_SOCKET)
    {
        return HI_SYS_SOCKET_ERROR;
    }

    local.sin_family      = AF_INET;
    if(pucIp == HI_NULL)
    {
        local.sin_addr.s_addr = INADDR_ANY;
    }
    else
    {
        if(0 == inet_aton(pucIp, (struct in_addr *) (&local.sin_addr.s_addr)))
        {
            return (HI_S32)HI_ERR_FAILURE;
        }
    }
    local.sin_port = htons((HI_U16) (usPortNum));

   

    if (0 == bind(s, (struct sockaddr*)&local, sizeof(struct sockaddr_in)))
    {
        return HI_ERR_SUCCESS;
    }
    else
    {
        return HI_ERR_SYSTEM_CALL_ERROR;
    }
}

HI_S32 SAL_SocketListen(HI_SOCKET s, HI_S16 backlog)
{    
    if(s == HI_SYS_INVALID_SOCKET)
    {
        return HI_SYS_SOCKET_ERROR;
    }
    return listen(s, backlog);
}

HI_S32 SAL_SocketSend(HI_SOCKET s, HI_PBYTE buf, HI_S32 len)
{
    if(s == HI_SYS_INVALID_SOCKET)
    {
        return HI_SYS_SOCKET_ERROR;
    }
    return send((HI_S32) (s), buf, (HI_U32) len, 0);
}

// how: HI_SYS_SOCKET_SHUTDOWN_BOTH
HI_S32 SAL_SocketShutdown(HI_SOCKET s, HI_S32 how)
{
    if(s == HI_SYS_INVALID_SOCKET)
    {
        return HI_SYS_SOCKET_ERROR;
    }
    return shutdown(s, how);
}

HI_S32 SAL_SocketClose(HI_SOCKET s)
{
    if(s == HI_SYS_INVALID_SOCKET)
    {
        return HI_SYS_SOCKET_ERROR;
    }
    return close(s);
}

// len : max. bytes to receive and save to buf.
HI_S32 SAL_Recv(HI_SOCKET s, HI_OUT HI_PBYTE buf, HI_S32 len)
{
    if(s == HI_SYS_INVALID_SOCKET)
    {
        return HI_SYS_SOCKET_ERROR;
    }

    return recv(s, buf, (HI_U32)len, 0);
}


HI_VOID SAL_SocketSetFd(HI_SOCKET s, HI_SYS_SOCKET_FD* fd)
{
    fd_set* pFd = (fd_set*)fd;
    if(s == HI_SYS_INVALID_SOCKET)
    {
        return;
    }
    FD_SET((HI_U32)s, pFd);
}

HI_BOOL SAL_SocketIsSetFd(HI_SOCKET s, HI_SYS_SOCKET_FD* fd)//次函数的返回值
{
    fd_set* pFd = (fd_set*)fd;    /*add by zhanglei for demo compiler*/
    
    if(s == HI_SYS_INVALID_SOCKET)
    {
        return (HI_BOOL)HI_FALSE;
    }

    return (HI_BOOL)(FD_ISSET((HI_U32)s, pFd) ? HI_TRUE : HI_FALSE);  
}

HI_U32 SAL_SocketSelect(HI_S32 nfds, HI_SYS_SOCKET_FD* readfds, HI_SYS_SOCKET_FD * writefds, HI_U32 ulTimeOut)
{
    HI_S32 ret = 0;
    struct timeval* pstTimeout = HI_NULL;
    struct timeval stTimeout   = { 0 };

    if (ulTimeOut != HI_SYS_WAIT_FOREVER)
    {
        stTimeout.tv_sec  = (HI_S32)(ulTimeOut/1000);
        stTimeout.tv_usec = (ulTimeOut%1000)*1000;
        pstTimeout        = &stTimeout;
    }

    /*if ulTimeOut=HI_SYS_WAIT_FOREVER, then pstTimeout=HI_NULL*/
    ret = select(nfds, (fd_set *)readfds, (fd_set *)writefds, HI_NULL, pstTimeout);
    if (SAL_SOCKET_ERROR == ret)
    {
        printf("%s,error",__func__);
        return HI_ERR_SYSTEM_CALL_ERROR;
    }
    else if(0 == ret)
    {
        printf("%s, timeout",__func__);
        return HI_ERR_TIMEOUT;
    }
    else
    {
        return HI_ERR_SUCCESS;
    }
}

HI_U32 HI_SYS_SocketSetOpt(HI_SOCKET s, HI_SYS_SOCKET_OPT_ID_E ulOpt, HI_PVOID optval,  HI_U32 optlen)
{
    int opt_val = 0;
    if(s == HI_SYS_INVALID_SOCKET)
    {
        return (HI_U32)HI_SYS_SOCKET_ERROR;
    }

    if (ulOpt == HI_SYS_SOCKET_OPT_ID_NO_DELAY)
    {
        if(optval) opt_val = *((int*)optval);
        /* Enable / disable the Nagle algorithm */
        if (SAL_SOCKET_ERROR == setsockopt(s, SOL_SOCKET, TCP_NODELAY,
            (char*)&opt_val, sizeof(opt_val)))
        {
            return HI_ERR_SYSTEM_CALL_ERROR;
        }
    }
    else if (ulOpt == HI_SYS_SOCKET_OPT_SO_BROADCAST)
    {
        if (SAL_SOCKET_ERROR == setsockopt(s, SOL_SOCKET, SO_BROADCAST,
            (char*)optval, optlen))
        {
            return HI_ERR_SYSTEM_CALL_ERROR;
        }
    }
    else if (ulOpt == HI_SYS_SOCKET_OPT_SO_LINGER)
    {
        if (SAL_SOCKET_ERROR == setsockopt(s, SOL_SOCKET, SO_LINGER,
            (char*)optval, optlen))
        {
            return HI_ERR_SYSTEM_CALL_ERROR;
        }
    }
    else if (ulOpt == HI_SYS_SOCKET_OPT_SO_REUSEADDR)
    {
        if (SAL_SOCKET_ERROR == setsockopt(s, SOL_SOCKET, SO_REUSEADDR,
            (char*)optval, optlen))
        {
            return HI_ERR_SYSTEM_CALL_ERROR;
        }
    }
    else if (ulOpt == HI_SYS_SOCKET_OPT_SO_RCVBUF)
    {
        if (SAL_SOCKET_ERROR == setsockopt(s, SOL_SOCKET, SO_RCVBUF,
            (char*)optval, optlen))
        {
            return HI_ERR_SYSTEM_CALL_ERROR;
        }
    }
    else if (ulOpt == HI_SYS_SOCKET_OPT_BLOCK)
    {
        opt_val = 0;
        if (ioctl(s, FIONBIO, (void *)&opt_val) != 0)
        {
            return HI_ERR_SYSTEM_CALL_ERROR;
        }
    }
    else if (ulOpt == HI_SYS_SOCKET_OPT_NO_BLOCK)
    {
        opt_val = 1;
        if (ioctl(s, FIONBIO, (void *)&opt_val) != 0)
        {
            return HI_ERR_SYSTEM_CALL_ERROR;
        }
    }
    else
    {
        return HI_ERR_NOT_SUPPORT;
    }

    return HI_ERR_SUCCESS;

}


//以上调试通过(ping通新ip)，以下暂未通过
HI_U32 SAL_SetPlcCfg(HI_IN HI_U8 * pIpAddr, HI_IN HI_U8 * pNetMask, HI_IN HI_U8 * pMacAddr)
{
    DIAG_CMD_ETH_CONFIG_STRU stIpCfg;

    memcpy_s(stIpCfg.ip_addr, sizeof(stIpCfg.ip_addr), pIpAddr, 4);
    memcpy_s(stIpCfg.net_mask, sizeof(stIpCfg.net_mask), pNetMask, 4);
    memcpy_s(stIpCfg.mac_addr, sizeof(stIpCfg.mac_addr), pMacAddr, 6);
    if (HI_ERR_SUCCESS == SAL_SetEthCfg("eth1", &stIpCfg, sizeof(DIAG_CMD_ETH_CONFIG_STRU)))
    {
        return HI_ERR_SUCCESS;
    }
    
    return HI_ERR_SYSTEM_CALL_ERROR;
}

HI_U32 (*SAL_netPktFreeFunc)(HI_PVOID pkt) = HI_NULL;
HI_VOID SAL_netPktFreeFuncReg(HI_PVOID func)
{
    SAL_netPktFreeFunc = func;
    return;
}

HI_VOID SAL_netPktFreeHook(HI_PVOID data, HI_PVOID buf)
{
    if (SAL_netPktFreeFunc)
        SAL_netPktFreeFunc(buf);
    return;
}

HI_VOID SAL_netPktCreate(HI_PVOID ipcomPkt, HI_PVOID data, HI_U32 pktSize, HI_PVOID bufFree)
{
    return ;
}

HI_VOID SAL_netPktDataGet(HI_PVOID ipcomPkt, HI_U8 **data)
{
    return ;
}

HI_VOID SAL_netPktInput(HI_PVOID netif, HI_PVOID ipcomPkt, HI_S32 dataStart, HI_S32 dataEnd)
{
    return ;
}

HI_VOID SAL_netPktIpStartGet(HI_PVOID ipcomPkt, HI_S32 *ipStart)
{
    return ;
}

HI_S32  SAL_ioctlCmdMap(HI_S32 cmd)
{
    return HI_ERR_NOT_SUPPORT;
}

HI_VOID SAL_routeInit(HI_VOID)
{
    return ;
}


HI_VOID SAL_plcPktShow(HI_U32 flag, HI_U8 * data, HI_U32 len)
{
    int i;
    int lenth = (HI_S32)((len > 100) ? 100 : len);

    if (flag)
    {
        if (flag == 1)
            printf("RX: ");
        else
            printf("TX: ");
    }
    if (flag)
    {
        printf("ptr = %8x, len = %d \r\n", (HI_U32)data, len);
        printf("packets: \n\r");
        for (i = 0; i < lenth; i += 8)
        {
            printf("%02x %02x %02x %02x %02x %02x %02x %02x\n\r",
                data[i], data[i + 1],
                data[i + 2], data[i + 3],
                data[i + 4], data[i + 5],
                data[i + 6], data[i + 7]);
        }
    }
}



HI_U32 SAL_OSGetCurrentTaskPointer(HI_OUT HI_PVOID* pTask)
{
    if (!pTask)
    {
        return HI_ERR_INVALID_PARAMETER;
    }
    
    *pTask = (HI_PVOID)LOS_CurTaskIDGet();
    return  HI_ERR_SUCCESS; 
}
HI_VOID SAL_DISABLE_MMU (HI_VOID)
{
    return ;
}
#endif // __SAL_OS_H__

