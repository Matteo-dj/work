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
#include <hi_mdm_sem.h>

//TODO:增加配置send_buf和rcv_buf的操作
HI_SOCKET HI_ICC_SocketCreate(ICC_CHANNEL_ID_E enChannelId,HI_U32 ulConnectSec,HI_U32 ulRecvSendTimeOutSec)//CLIENT创建socket
{
    //统计量++
    
    struct timeval tv; 
    HI_SOCKET s = HI_SYS_INVALID_SOCKET;
    HI_ICC_STAT_S *pIccErrCtx = ICC_GET_ERR_CTX();
    HI_ICC_CTRL_S *pIccCtx = ICC_GET_CTRL_CTX();
    HI_U16 usPortNum = 0;
    
    ICC_CHECK_NOT_INIT_RETURN(HI_SYS_INVALID_SOCKET);

        
    if(enChannelId>=ICC_CHANNEL_ID_MAX)
    {
        return HI_SYS_INVALID_SOCKET;
    }
    usPortNum = pIccCtx->channel[enChannelId].phyPortNum;
    
    s = socket(AF_INET, SOCK_STREAM, 0);
    pIccErrCtx->s_creat_times++;
    if(s == HI_SYS_INVALID_SOCKET)
    {
        HI_ICC_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
        pIccErrCtx->s_create_fail++;
        return HI_SYS_INVALID_SOCKET;
    }
    if(HI_ERR_SUCCESS != icc_connect(s,pIccCtx->ucTargetIp,usPortNum,ulConnectSec))
    {
        HI_ICC_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);        
        pIccErrCtx->s_connect_fail++;
        icc_close_socket(s);
        return HI_SYS_INVALID_SOCKET;
    }

    
    tv.tv_sec = (HI_S32)ulRecvSendTimeOutSec;
    tv.tv_usec = 0;
    
    if(0 != setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (void *) &tv, sizeof(tv)))
    {
        HI_ICC_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);        
        pIccErrCtx->s_set_config_fail++;
        icc_close_socket(s);
        return HI_SYS_INVALID_SOCKET;
    }

    tv.tv_sec = (HI_S32)ulRecvSendTimeOutSec;
    tv.tv_usec = 0;
    
    if(setsockopt(s,SOL_SOCKET,SO_SNDTIMEO,&tv,sizeof(tv))<0)
    {
        HI_ICC_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);        
        pIccErrCtx->s_set_config_fail++;
        icc_close_socket(s);
        return HI_SYS_INVALID_SOCKET;

    }

    
    HI_ICC_PrintDebug("[OUT][%s %u][socket=0x%x]\r\n"
        ,__func__,__LINE__,s);
    return s;
}

HI_U32 HI_ICC_SocketClose(HI_SOCKET s)
{
    ICC_CHECK_NOT_INIT_RETURN(HI_ERR_ICC_NOT_INIT);
    
    return icc_close_socket(s);
}


//*****************************************************************************
// 函数名称: HI_ICC_SendCmdHead
// 功能描述: 发送命令头报文,仅在client端支持
//    建议优先使用HI_ICC_SendCmdHeadData接口,当HI_ICC_SendCmdHeadData接口无法满足需求时使用HI_ICC_SendCmdHead和HI_ICC_Send
// 参数列表:
// 返回值:
//*****************************************************************************
HI_U32 HI_ICC_SendCmdHead(HI_S32 s,HI_U16 cmdId,HI_U32 ulTotalUsrDataSize)
{
    HI_ICC_STAT_S *pIccErrCtx = ICC_GET_ERR_CTX();
    HI_ICC_CMD_PKT_S stIccCmd = {0};

    ICC_CHECK_NOT_INIT_RETURN(HI_ERR_ICC_NOT_INIT);


    (hi_void)memset_s((HI_VOID *)&stIccCmd, sizeof(stIccCmd), 0, sizeof(stIccCmd));
    stIccCmd.cmdId = cmdId;
    stIccCmd.dataSize = ulTotalUsrDataSize;
    stIccCmd.magicNum = HI_ICC_MAGIC_NUM;
    pIccErrCtx->last_snd_cmd_id = cmdId;
    return icc_send(s,(HI_PBYTE)&stIccCmd,sizeof(stIccCmd));
}



HI_U32 HI_ICC_SendCmdHeadData(HI_S32 s,HI_U16 cmdId,HI_U32 ulTotalUsrDataSize,HI_ICC_CMD_DATA_BUF_S *pstBufs)
{
    HI_U32 i=0;
    HI_U32 ret=HI_ERR_SUCCESS;

    ICC_CHECK_NOT_INIT_RETURN(HI_ERR_ICC_NOT_INIT);

    
    ret=HI_ICC_SendCmdHead(s,cmdId,ulTotalUsrDataSize);
    if(ret!=HI_ERR_SUCCESS||pstBufs==HI_NULL)
    {
        return ret;
    }

    for(i=0;i<pstBufs->ulBufCount;i++)
    {
        ret=icc_send(s,pstBufs->pBuf[i],(HI_S32)pstBufs->ulBufLen[i]);
        if(ret!=HI_ERR_SUCCESS)
        {
            return ret;
        }
    }
    
    return ret;
}

HI_U32 HI_ICC_ReceiveIndPkt(HI_S32 s,HI_U32 *pCmdId,HI_U32 *pUsrDataSize)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_ICC_IND_PKT_S stIccInd = {0};
    
    ICC_CHECK_NOT_INIT_RETURN(HI_ERR_ICC_NOT_INIT);

    ret = icc_receive(s,(HI_PBYTE)&stIccInd,sizeof(stIccInd));
    if(HI_ERR_SUCCESS != ret)
    {        
        return ret;
    }
    if(stIccInd.ret_sys!=HI_ERR_SUCCESS)
    {
        return stIccInd.ret_sys;
    }
    return HI_ERR_SUCCESS;
}

//*****************************************************************************
// 函数名称: HI_ICC_GetConnectStat
// 功能描述: 获取通道通道连接状态
//           
// 参数说明:
//      enChannelId[IN] :通道ID
//
// 返 回 值: 
//      HI_TRUE:通道连接
//      HI_FALSE:通道未连接
//
// 调用要求:
//
// 调用举例: 
// 作    者: 边海文20150313
//*****************************************************************************
HI_BOOL HI_ICC_GetConnectStat(HI_IN ICC_CHANNEL_ID_E enChannelId)
{
    HI_ICC_CTRL_S *pIccCtx = ICC_GET_CTRL_CTX();

    ICC_CHECK_NOT_INIT_RETURN(HI_FALSE);

    
    return pIccCtx->channel[enChannelId].bConnect;
}

HI_U32 HI_ICC_SetConnectStat(HI_IN ICC_CHANNEL_ID_E enChannelId,HI_BOOL bConnect)
{
    HI_ICC_CTRL_S *pIccCtx = ICC_GET_CTRL_CTX();
    
    ICC_CHECK_NOT_INIT_RETURN(HI_ERR_ICC_NOT_INIT);

    pIccCtx->channel[enChannelId].bConnect=bConnect;
    return HI_ERR_SUCCESS;
}

HI_U32 ICC_CHANNEL_LOCK(ICC_CHANNEL_ID_E chan_id,HI_U32 ulTimeOut)
{
    
    HI_ICC_CTRL_S *pIccCtx = ICC_GET_CTRL_CTX();
    
    ICC_CHECK_NOT_INIT_RETURN(HI_ERR_ICC_NOT_INIT);
    
    return HI_MDM_SEM_Wait(pIccCtx->channel[chan_id].ulSemId,ulTimeOut);
}

HI_VOID ICC_CHANNEL_UNLOCK(ICC_CHANNEL_ID_E chan_id)
{
    HI_ICC_CTRL_S *pIccCtx = ICC_GET_CTRL_CTX();

    ICC_CHECK_NOT_INIT_RETURN_NOVAL();

    HI_MDM_SEM_Signal(pIccCtx->channel[chan_id].ulSemId);
    
}
#endif

