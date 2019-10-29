//*****************************************************************************
//
//                  版权所有 (C), 1998-2009, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : sal_os_socket.c
//  版 本 号   : V1.0
//  作    者   : 沈汉坤/00130424
//  生成日期   : 2009-04-17
//  功能描述   : TODO: ...
//
//  函数列表   : SAL_StartTimer、SAL_StopTimer 
//  修改历史   :
//  1.日    期 : 2009-04-17
//    作    者 : 沈汉坤/00130424
//    修改内容 : 创建文件
//  2.日    期 : 2009-05-12
//    作    者 : 陈文峰/00149739
//    修改内容 : 增加函数DO_GetCurrentTaskQueueId
//               修改函数 ODA_IsMessageAvailable 用来查询队列是否有效
//  3.日    期 : 2009-07-04
//    作    者 : 陈文峰/00149739
//    修改内容 : 增加g_aulODAHighMsgTable
//               修改函数 ODA_IsHiPriMsg 来查询是否是高优先级消息
//               目前使用的并非是最优化方式
//  4.日    期 : 20011年10月14日
//    作    者 : 沈汉坤/00130424
//    修改内容 : 调整适配为Hi3911项目需求 
//
//*****************************************************************************

//*****************************************************************************
// PROJECT   :
// SUBSYSTEM : SAL 
// MODULE    : SAL 
// OWNER     : 
//*****************************************************************************

#include "sal_common.h"

#if defined(PRODUCT_CFG_OS_VX) && defined(PRODUCT_CFG_FEATURE_SOCKET)

//*****************************************************************************
HI_S32 HI_SYS_SocketSend(HI_SOCKET s, HI_PBYTE buf, HI_S32 len)
{
#if defined(PRODUCT_CFG_FEATURE_POSIX_SOCKET)    
#if defined(PRODUCT_CFG_OS_VX)
    return SAL_SocketSend(s, buf, len);
#else
    return send((int) (s), buf, (int) len, 0);
#endif
#endif 
}

HI_S32 HI_SYS_SocketShutdown(HI_SOCKET s, HI_S32 how)
{
#if defined(PRODUCT_CFG_FEATURE_POSIX_SOCKET)    
#if defined(PRODUCT_CFG_OS_VX)
    return SAL_SocketShutdown(s, how);
#else
    return shutdown(s, how);
#endif 
#endif 
}

HI_S32 HI_SYS_SocketClose(HI_SOCKET s)
{
#if defined(PRODUCT_CFG_FEATURE_POSIX_SOCKET)
#if defined(PRODUCT_CFG_OS_WIN)
    return closesocket(s);
#else
#if defined(PRODUCT_CFG_OS_VX)
    return SAL_SocketClose(s);
#else
    return close(s);
#endif
#endif 
#endif 
}

HI_S32 HI_SYS_SocketRecv(HI_SOCKET s, HI_OUT HI_PBYTE buf, HI_S32 len)
{
    #if defined(PRODUCT_CFG_FEATURE_POSIX_SOCKET)    
    #if defined(PRODUCT_CFG_OS_VX)
    return SAL_Recv(s, buf, len);
    #else
    return recv(s, buf, len, 0);
    #endif
    #endif 
}

HI_S32 HI_SYS_SocketSendTo(HI_SOCKET s, HI_PBYTE buf, HI_S32 len, HI_PCSTR pucIp, HI_U16 usPortNum)
{
    #if defined(PRODUCT_CFG_FEATURE_POSIX_SOCKET)
    #if defined(PRODUCT_CFG_OS_VX)
    return SAL_SocketSendTo(s, buf, len, pucIp, usPortNum);
    #else
    struct sockaddr_in  dest;    
    memset_s((HI_VOID *)&dest,sizeof(dest), 0,sizeof(dest));    
    dest.sin_addr.s_addr = inet_addr(pucIp);
    dest.sin_family = AF_INET; 
    dest.sin_port   = htons(usPortNum);    
    return sendto(s, buf, len, 0, (struct sockaddr*)&dest, sizeof(dest));
    #endif 
    #endif 
}

HI_S32 HI_SYS_SocketRecvFrom(HI_SOCKET s, HI_OUT HI_PBYTE buf, HI_S32 len)
{
    #if defined(PRODUCT_CFG_FEATURE_POSIX_SOCKET)    
    #if defined(PRODUCT_CFG_OS_VX)
    return SAL_SocketRecvFrom(s, buf, len);
    #else
    struct sockaddr_in from = { 0 };
    int nFromlen     = sizeof(from);    
    return recvfrom(s, (HI_PVOID)buf, len, 0, (struct sockaddr*)&from, &nFromlen);
    #endif 
    #endif
}


HI_S32 HI_SYS_SocketConnect(HI_SOCKET s, HI_PCSTR pucIp, HI_U16 usPortNum)
{
    #if defined(PRODUCT_CFG_FEATURE_POSIX_SOCKET)    
    #if defined(PRODUCT_CFG_OS_VX)
    return SAL_SocketConnect(s, pucIp, usPortNum);
    #else
    struct sockaddr_in server;
    memset_s((HI_VOID *)&server, sizeof(server), 0, sizeof(server));
    server.sin_family      = AF_INET;
    server.sin_addr.s_addr = inet_addr(pucIp);
    server.sin_port        = htons(usPortNum);    
    return connect(s, (struct sockaddr*)&server, sizeof(server));
    #endif 
    #endif 
}


HI_SOCKET HI_SYS_SocketAccept(HI_SOCKET s)
{    
    #if defined(PRODUCT_CFG_FEATURE_POSIX_SOCKET)
    #if defined(PRODUCT_CFG_OS_VX)
    return SAL_SocketAccept(s);
    #else
    HI_SOCKET sck     = (HI_SOCKET)HI_SYS_SOCKET_ERROR;
    {
        struct sockaddr_in from = { 0 };
        HI_S32 len  = sizeof(struct sockaddr_in);
        sck = accept(s, (struct sockaddr*)&from, &len);
    }
    return sck;
    #endif 
    #endif
}

HI_SOCKET HI_SYS_SocketTcpCreate(HI_VOID)
{
    #if defined(PRODUCT_CFG_FEATURE_POSIX_SOCKET)
    #if defined(PRODUCT_CFG_OS_VX)
    return SAL_SocketTcpCreate();
    #else
    return socket(AF_INET, SOCK_STREAM, 0);
    #endif 
    #endif
}

HI_SOCKET HI_SYS_SocketUdpCreate(HI_VOID)
{
    #if defined(PRODUCT_CFG_FEATURE_POSIX_SOCKET)
    #if defined(PRODUCT_CFG_OS_VX)
    return SAL_SocketUdpCreate();
    #else
    return socket(AF_INET, SOCK_DGRAM, 0);
    #endif
    #endif
}

HI_U32 HI_SYS_SocketSetLinger(HI_SOCKET s)
{
    #if defined(PRODUCT_CFG_FEATURE_POSIX_SOCKET)
    #if defined(PRODUCT_CFG_OS_VX)
    return SAL_SocketSetLinger(s);
    #else
    #endif
    #endif
}

HI_U32 HI_SYS_SocketSetKeepAlive(HI_SOCKET s, HI_S32 slEnable, HI_S32 slTcpKeepIdle, HI_S32 slTcpKeepCnt)
{
    return SAL_SocketSetKeepAlive(s, slEnable, slTcpKeepIdle, slTcpKeepCnt);
}

HI_S32 HI_SYS_SocketGetError(HI_VOID)
{
#ifdef PRODUCT_CFG_OS_WIN
    return WSAGetLastError();
#else
    return 0;
#endif 
}

HI_S32 HI_SYS_SocketBind(HI_SOCKET s, HI_PCSTR pucIp, HI_U16 usPortNum)
{
 #if defined(PRODUCT_CFG_FEATURE_POSIX_SOCKET)
    #if defined(PRODUCT_CFG_OS_VX)
    return SAL_SocketBind(s, pucIp, usPortNum);
    #else
    struct sockaddr_in localaddr = { 0 };
    
    // initialize socket address
    localaddr.sin_family      = AF_INET;
    if(pucIp == HI_NULL)
    {
        localaddr.sin_addr.s_addr = INADDR_ANY;
    }
    else 
    {
        localaddr.sin_addr.s_addr = inet_addr(pucIp);
    }
    localaddr.sin_port = htons((HI_U16) (usPortNum));
    
    #ifndef PRODUCT_CFG_OS_WIN
    localaddr.sin_len = sizeof(struct sockaddr_in);
    #endif
    
    if (0 == bind(s, (struct sockaddr*)&localaddr, sizeof(struct sockaddr_in)))
    {
        return HI_ERR_SUCCESS;
    }
    else
    {
        return HI_ERR_SYSTEM_CALL_ERROR;
    }
    #endif
#endif 
}


HI_S32 HI_SYS_SocketListen(HI_SOCKET s, HI_S16 backlog)
{
 #if defined(PRODUCT_CFG_FEATURE_POSIX_SOCKET)
    #if defined(PRODUCT_CFG_OS_VX)
    return SAL_SocketListen(s, backlog);
    #else
    return listen(s, backlog);
    #endif 
 #endif 
}

HI_VOID HI_SYS_SocketSetFd(HI_SOCKET s, HI_SYS_SOCKET_FD* fd)
{
#if defined(PRODUCT_CFG_FEATURE_POSIX_SOCKET)       
#if defined(PRODUCT_CFG_OS_VX)
    SAL_SocketSetFd(s, fd);
#else
    fd_set* pFd = (fd_set*)fd;
#if defined(PRODUCT_CFG_OS_NU)
    FD_PSET(s, pFd);
#else
    FD_SET((HI_U32)s, pFd);
#endif 
#endif
#endif
}

HI_BOOL HI_SYS_SocketIsSetFd(HI_SOCKET s, HI_SYS_SOCKET_FD* fd)
{
    if(HI_SYS_INVALID_SOCKET == s)
    {
        return HI_FALSE;
    }
    
#if defined(PRODUCT_CFG_FEATURE_POSIX_SOCKET)        
#if defined(PRODUCT_CFG_OS_VX)
    return SAL_SocketIsSetFd(s, fd);
#else
    return (FD_ISSET(s, fd)?HI_TRUE:HI_FALSE);
#endif
#endif 
}


HI_U32 HI_SYS_SocketSelect(HI_S32 nfds, HI_SYS_SOCKET_FD* readfds, HI_SYS_SOCKET_FD * writefds, HI_U32 ulTimeOut)
{
#if defined(PRODUCT_CFG_FEATURE_POSIX_SOCKET)        
#if defined(PRODUCT_CFG_OS_VX)
    return SAL_SocketSelect(nfds, readfds, writefds, ulTimeOut);
#else
    HI_S32 ret = 0;
    struct timeval* ptm = HI_NULL;
    struct timeval tm   = { 0 };

    if (ulTimeOut != HI_SYS_WAIT_FOREVER)
    {
        tm.tv_sec  = ulTimeOut*1000/(1000*1000);
        tm.tv_usec = (ulTimeOut%1000)*1000;
        ptm         = &tm;
    }
    ret = select(nfds, (struct fd_set *)readfds, (struct fd_set *)writefds, HI_NULL, ptm);    
    if (SOCKET_ERROR == ret)
    {
        return HI_ERR_SYSTEM_CALL_ERROR;
    }
    else if(0 == ret)
    {
        return HI_ERR_TIMEOUT;
    }
    else
    {
        return HI_ERR_SUCCESS;
    }
#endif 
#endif
}


#endif // #ifdef PRODUCT_CFG_FEATURE_SOCKET

