
#include <hi_types.h>
#include <hi_mdm_types.h>
#if defined (PORDUCT_CFG_SUPPORT_ICC)
#include <hi_icc.h>
#include "icc_debug.h"
#include "icc.h"
#include <icc_protocal_filemanager.h>

HI_PRV HI_U32 icc_check_connect_cmd(HI_OUT HI_U32 *ret_result,ICC_CHANNEL_ID_E channelId)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_S32 socket = HI_SYS_INVALID_SOCKET;
    HI_ICC_IND_PKT_S stIccInd = {0};
    HI_U32 ulUsrCmdLen = sizeof(HI_FMGR_INIT_CMD_S);
    HI_FMGR_INIT_CMD_S stUsrCmd = {0};
    HI_FMGR_INIT_IND_S stUsrInd = {0};
    HI_ICC_CMD_DATA_BUF_S stCmdBufs={0};
    
    HI_ICC_PrintDebug("[IN][%s][%u]\r\n",__func__,__LINE__);

    //  TODO:入参判断    
    if(ret_result==HI_NULL)
    {
        return HI_ERR_INVALID_PARAMETER;
    }
    if(HI_ERR_SUCCESS != ICC_CHANNEL_LOCK(channelId,HI_SYS_WAIT_FOREVER))
    {
        return HI_ERR_WAIT_SEM_FAIL;
    }
    socket = HI_ICC_SocketCreate(channelId,1,1);
    if(socket == HI_SYS_INVALID_SOCKET)
    {
        HI_ICC_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
        (HI_VOID)ICC_CHANNEL_UNLOCK(channelId);
        return HI_ERR_ICC_SOCKET_CONNECT;
    }

    //发送系统数据
    stCmdBufs.ulBufCount=1;
    stCmdBufs.pBuf[0]=(HI_PBYTE)&stUsrCmd;
    stCmdBufs.ulBufLen[0]=sizeof(stUsrCmd);
    ret = HI_ICC_SendCmdHeadData(socket,HI_ICC_CMD_FMGR_INIT,ulUsrCmdLen,&stCmdBufs);
    if(HI_ERR_SUCCESS != ret)
    {
        HI_ICC_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
        HI_ICC_SocketClose(socket);
        (HI_VOID)ICC_CHANNEL_UNLOCK(channelId);
        return ret;
    }

    //接收系统IND
    ret = HI_ICC_Receive(socket,(HI_PBYTE)&stIccInd,sizeof(stIccInd));
    if(HI_ERR_SUCCESS != ret)
    {
        HI_ICC_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
        HI_ICC_SocketClose(socket);
        (HI_VOID)ICC_CHANNEL_UNLOCK(channelId);
        return ret;
    }
    if(stIccInd.ret_sys != HI_ERR_SUCCESS)
    {
        HI_ICC_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
        HI_ICC_SocketClose(socket);
        (HI_VOID)ICC_CHANNEL_UNLOCK(channelId);
        return stIccInd.ret_sys;
    }

    //接收用户IND
    ret = HI_ICC_Receive(socket,(HI_PBYTE)&stUsrInd,sizeof(stUsrInd));
    if(HI_ERR_SUCCESS != ret)
    {
        HI_ICC_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
        HI_ICC_SocketClose(socket);
        (HI_VOID)ICC_CHANNEL_UNLOCK(channelId);
        return ret;
    }
    *ret_result = stUsrInd.ret_result;
    
//END:
    HI_ICC_SocketClose(socket);
    HI_ICC_PrintDebug("[OUT][INFO][%s %u][socket=%d][ret_result=%u]\r\n"
        ,__func__,__LINE__,socket,*ret_result);
    (HI_VOID)ICC_CHANNEL_UNLOCK(channelId);
    return HI_ERR_SUCCESS;
}

HI_VOID icc_check_connect_status(HI_VOID)
{    
    HI_U32 i = 0;
    HI_ICC_CTRL_S *pIccCtx = ICC_GET_CTRL_CTX();
    HI_U32 ulResult=HI_ERR_SUCCESS;
    HI_U32 ret=HI_ERR_SUCCESS;

    
    for(i=0;i<ICC_CHANNEL_ID_MAX;i++)
    {
        if(pIccCtx->channel[i].bClient==HI_TRUE)
        {
            ret = icc_check_connect_cmd(&ulResult,(ICC_CHANNEL_ID_E)i);
            
            if(ret==HI_ERR_SUCCESS && ulResult==HI_ERR_SUCCESS)
            {
                pIccCtx->channel[i].tryConnectFailTimes=0;
                HI_ICC_SetConnectStat((ICC_CHANNEL_ID_E)i,HI_TRUE);
            }
            else
            {
                pIccCtx->channel[i].tryConnectFailTimes++;
                if(pIccCtx->channel[i].tryConnectFailTimes >= ICC_MAX_TRY_CONNECT_FAIL_TIMES)
                {
                    HI_ICC_SetConnectStat((ICC_CHANNEL_ID_E)i,HI_FALSE);
                    pIccCtx->channel[i].tryConnectFailTimes=ICC_MAX_TRY_CONNECT_FAIL_TIMES;       
                }
            }
        }
    }
    
}
#endif
