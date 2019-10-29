//****************************************************************************
//
//                  版权所有 (C), 2001-2011, 华为技术有限公司
//
//****************************************************************************
//  文 件 名   : icc.c
//  版 本 号   : V1.0
//  作    者   : 边海文/00208046
//  生成日期   : 2015/02/05
//  功能描述   : 用于板间通信
//  函数列表   : 
//  修改历史   : 
//****************************************************************************
#include <hi_types.h>
#include <hi_mdm_types.h>
#if defined (PORDUCT_CFG_SUPPORT_ICC)
#include <hi_icc.h>
#include <hi_errno.h>
#include <icc_vx.h>
#include <hi_socket.h>
#include <hi_stdlib.h>
#include <dfx_icc.h>
#include "icc_debug.h"
#include "icc.h"

//*****************************************************************************
// 函数名称: icc_receive
// 功能描述: 接收数据
// 参数列表:
//  socket:目标socket
//  buf:接收缓存
//  totalLen:缓存大小
//
//*****************************************************************************
HI_U32 icc_receive(HI_S32 s,HI_PBYTE buf,HI_S32 totalLen)
{
    int len = 0;
    int readLen = 0;
    int curReadLen = 0;
    HI_ICC_STAT_S *pIccErrCtx = ICC_GET_ERR_CTX();
    HI_ICC_PrintDebug("[IN][%s %u][socket=%d][totalLen=%u]\r\n"
        ,__func__,__LINE__,s,totalLen);
    
    if(s == HI_SYS_INVALID_SOCKET)
    {
        HI_ICC_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
        pIccErrCtx->s_rcv_fail++;//这增加错误统计合适吗
        return HI_ERR_INVALID_PARAMETER;
    }

    while(totalLen>0)
    {
        curReadLen = (totalLen >= 1460)?1460:totalLen;
        len = recv(s,buf+readLen,(size_t)curReadLen,0);
        if(len<=0||len > curReadLen)
        {
            HI_ICC_PrintErr("[ERR][%s %u][socket=%d][len==%d]\r\n"
                ,__func__,__LINE__,s,len);
            pIccErrCtx->s_rcv_fail++;
            return HI_ERR_ICC_SOCKET_RECV;
        }
        totalLen-=len;
        readLen+=len;
    }
    HI_ICC_PrintDebug("[OUT][%s %u][socket=%d][readLen=%u]\r\n",__func__,__LINE__,s,readLen);
    pIccErrCtx->suc_rcv_bytes += (HI_U32)readLen;
    return HI_ERR_SUCCESS;
}

//*****************************************************************************
// 函数名称: icc_send
// 功能描述: 发送数据
// 参数列表:
// 返回值:
//*****************************************************************************
HI_U32 icc_send(HI_S32 s,HI_PBYTE buf,HI_S32 totalLen)
{
    int len = 0;
    int sendLen = 0;
    int curSendLen = 0;
    HI_ICC_STAT_S *pIccErrCtx = ICC_GET_ERR_CTX();
    if(s == HI_SYS_INVALID_SOCKET)
    {   
        pIccErrCtx->s_send_fail++;//这增加错误统计合适吗
        return HI_ERR_INVALID_PARAMETER;
    }
    while(totalLen>0)
    {
        curSendLen = (totalLen >= 1460)?1460:totalLen;
        len= send(s,buf+sendLen,(size_t)curSendLen,0);
        if(len<0||len > curSendLen)
        {
            HI_ICC_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
            pIccErrCtx->s_send_fail++;
            return HI_ERR_ICC_SOCKET_SEND;
        }
        
        totalLen -= len;
        sendLen += len;
    }
    
    HI_ICC_PrintDebug("[OK][%s %u][sendLen=%u]\r\n",__func__,__LINE__,sendLen);
    pIccErrCtx->suc_snd_bytes += (HI_U32)sendLen;
    return HI_ERR_SUCCESS;
}

//*****************************************************************************
// 函数名称: icc_close_socket
// 功能描述: 关闭socket
// 参数列表:
// 返回值:
//*****************************************************************************
HI_U32 icc_close_socket(HI_SOCKET s)
{
    //统计量--
    HI_ICC_STAT_S *pIccErrCtx = ICC_GET_ERR_CTX();
    pIccErrCtx->s_close_times++;
    
    if(s == HI_SYS_INVALID_SOCKET)
    {
        return HI_ERR_ICC_CLISE_VALID_SOCKET;
    }
    if(close(s)<0)
    {
        return HI_ERR_ICC_CLOSE_FAIL;
    }
    else
    {
        return HI_ERR_SUCCESS;
    }
}

//*****************************************************************************
// 函数名称: icc_connect
// 功能描述: 连接
// 参数列表:
// 返回值:
//*****************************************************************************
#define HI_NONE_BLOCK 1
#define HI_BLOCK 0
HI_U32 icc_connect(HI_SOCKET s, HI_PCSTR pucIp, HI_U16 usPortNum,HI_U32 ulTimeOutsec)
{
    HI_BOOL bConnect = HI_FALSE;
    struct sockaddr_in server;
    struct timeval tv; 
    fd_set set; 
    HI_S32 iOpt = HI_NONE_BLOCK;
    HI_S32 error_code = -1;
    HI_S32 len = 4;
    HI_S32 select_result = 0;

    if ((!pucIp) || (s == HI_SYS_INVALID_SOCKET))
    {
        return HI_ERR_INVALID_PARAMETER;
    }
    
    (void)memset_s((void*)(&set), sizeof(*(&set)),0, sizeof(*(&set))); 
    (hi_void)memset_s(&server,sizeof(struct sockaddr_in), 0,sizeof(struct sockaddr_in));
    server.sin_family      = AF_INET;
    if(0 == inet_aton(pucIp,(struct in_addr *)(&server.sin_addr.s_addr)))
    {
        return HI_ERR_FAILURE;
    }
    server.sin_port        = htons(usPortNum);
    
    if (0 != ioctl(s, FIONBIO, (void*)&iOpt))
    {
        return HI_ERR_FAILURE;
    }
    if(connect(s, (struct sockaddr*)&server, sizeof(server))==-1)
    {
        tv.tv_sec = (HI_S32)ulTimeOutsec; 
        tv.tv_usec = 0; 
        (void)memset_s((void*)(&set), sizeof(*(&set)),0, sizeof(*(&set))); 
        FD_SET(s, &set);
        select_result = select(s+1, NULL, &set, NULL, &tv);
        if( select_result > 0) 
        { 
            (HI_VOID)getsockopt(s, SOL_SOCKET, SO_ERROR, &error_code, (socklen_t *)&len); 
            if(error_code == 0) 
                bConnect = HI_TRUE; 
            else 
                bConnect = HI_FALSE; 
        }
        else 
        {
            bConnect = HI_FALSE; 
        }

    }
    else
    {
       bConnect = HI_TRUE; 
    }
    iOpt = HI_BLOCK;
    if (0 != ioctl(s, FIONBIO, (void *)&iOpt))
    {
        return HI_ERR_FAILURE;
    }
    if(bConnect)
    {
        return HI_ERR_SUCCESS;
    }
    else
    {
        return HI_ERR_FAILURE;
    }

}




HI_U32 HI_ICC_GetDfxInfo(HI_OUT HI_ICC_STAT_S * pIccStat)
{
    HI_ICC_STAT_S *pIccErrCtx = ICC_GET_ERR_CTX();
    if(pIccStat!=HI_NULL)
    {
        (hi_void)memcpy_s(pIccStat,sizeof(HI_ICC_STAT_S),pIccErrCtx,sizeof(HI_ICC_STAT_S));
    }
    return HI_ERR_SUCCESS;
}


HI_U32 HI_ICC_Receive(HI_IN HI_S32 s,HI_OUT HI_PBYTE buf,HI_IN HI_S32 totalLen)
{
    ICC_CHECK_NOT_INIT_RETURN(HI_ERR_ICC_NOT_INIT);
    
    return icc_receive(s,buf,totalLen);
}



HI_U32 HI_ICC_Send(HI_IN HI_S32 s,HI_IN HI_PBYTE buf,HI_IN HI_S32 totalLen)
{
    ICC_CHECK_NOT_INIT_RETURN(HI_ERR_ICC_NOT_INIT);
    
    return icc_send(s,buf,totalLen);
}



#endif//PORDUCT_CFG_SUPPORT_ICC

