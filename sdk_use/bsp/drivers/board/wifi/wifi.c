//*****************************************************************************
//
//                  版权所有 (C), 1998-2011, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : drv_ndm_eqt_interfence.c
//  版 本 号   : V1.0
//  作    者   : wanglei/00279578
//  生成日期   : 2015-08-20
//  功能描述   : 提供WIFI名字设置与BQ升级接口
//
//  函数列表   : TODO: ...
//  修改历史   :
//  1.日    期 : 2015-08-20
//    作    者 : wanglei/00279578
//    修改内容 : 创建文件
//*****************************************************************************
#include <hi_types.h>
#include <hi_mdm_types.h>
#ifdef PRODUCT_CFG_SUPPORT_WIFI
#include <hi_filemanager.h>
#include <hi_icc.h>
#include <hi_ioexp.h>
#include <hi_stdlib.h>

/***************魔术字定义****************/
#define HI_SSID_DEFAULT_RCV_TIME_OUT_SEC        5
#define HI_SSID_DEFAULT_CON_TIME_OUT_SEC        5

HI_U32 HI_WIFI_SetSsid(HI_IN HI_CHAR ucwifiname[WIFI_SSID_LENGTH+1])
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_S32 socket = HI_SYS_INVALID_SOCKET;
    HI_ICC_IND_PKT_S stIccInd = {0};
    HI_SET_SSID_CMD_S stUsrCmd = {{0}};
    HI_SET_SSID_IND_S stUsrInd = {0};
    HI_U32 ulwifinamelen = 0;
    HI_ICC_CMD_DATA_BUF_S stCmdBufs={0};

    if(ucwifiname == HI_NULL)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    ulwifinamelen = strlen(ucwifiname);

    if((ulwifinamelen < 1)||(ulwifinamelen > WIFI_SSID_LENGTH))
    {
        return HI_ERR_INVALID_PARAMETER;
    }
    (hi_void)memcpy_s(stUsrCmd.ucwifiname, WIFI_SSID_LENGTH+1, ucwifiname, WIFI_SSID_LENGTH+1);

    //DEBUG:步骤1:选择20001通道，并锁定
    if(HI_ERR_SUCCESS != ICC_CHANNEL_LOCK(ICC_USR_CHANNEL_ID_NET_SET,HI_SYS_WAIT_FOREVER))
    {
        return HI_ERR_WAIT_SEM_FAIL;
    }
    //DEBUG:步骤2:创建一个SOCKET
    socket = HI_ICC_SocketCreate(ICC_USR_CHANNEL_ID_NET_SET,HI_SSID_DEFAULT_CON_TIME_OUT_SEC,HI_SSID_DEFAULT_RCV_TIME_OUT_SEC);
    if(socket == HI_SYS_INVALID_SOCKET)
    {
        (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_NET_SET);
        return HI_ERR_ICC_SOCKET_CONNECT;
    }
    //DEBUG:步骤3:发送命令,命令ID:HI_ICC_CMD_FMGR_SET_SSID
    stCmdBufs.ulBufCount=1;
    stCmdBufs.pBuf[0]=(HI_PBYTE)&stUsrCmd;
    stCmdBufs.ulBufLen[0]=sizeof(stUsrCmd);
    ret = HI_ICC_SendCmdHeadData(socket,HI_ICC_CMD_SET_WIFI_SSID,sizeof(HI_SET_SSID_CMD_S),&stCmdBufs);
    if(HI_ERR_SUCCESS != ret)
    {
        HI_ICC_SocketClose(socket);
        (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_NET_SET);
        return ret;
    }

    //DEBUG:步骤4:接收从3518返回的iccind报文头，判断是否有通道异常
    ret = HI_ICC_Receive(socket,(HI_PBYTE)&stIccInd,sizeof(stIccInd));
    if(HI_ERR_SUCCESS != ret)
    {
        HI_ICC_SocketClose(socket);
        (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_NET_SET);
        return ret;
    }
    if(stIccInd.ret_sys != HI_ERR_SUCCESS)
    {
        HI_ICC_SocketClose(socket);
        (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_NET_SET);
        return stIccInd.ret_sys;
    }
    //接收3518返回的执行结果
    ret = HI_ICC_Receive(socket,(HI_PBYTE)&stUsrInd,sizeof(stUsrInd));
    if(HI_ERR_SUCCESS != ret)
    {
        HI_ICC_SocketClose(socket);
        (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_NET_SET);
        return ret;
    }
    //END:
    HI_ICC_SocketClose(socket);
    (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_NET_SET);
    return HI_ERR_SUCCESS;
}

HI_U32 HI_WIFI_GetSsid(HI_OUT HI_CHAR ucwifiname[WIFI_SSID_LENGTH+1])
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_S32 socket = HI_SYS_INVALID_SOCKET;
    HI_ICC_IND_PKT_S stIccInd = {0};
    HI_GET_SSID_CMD_S stUsrCmd = {0};
    HI_GET_SSID_IND_S stUsrInd = {{0}};
    HI_ICC_CMD_DATA_BUF_S stCmdBufs={0};

    if(ucwifiname == HI_NULL)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    //DEBUG:步骤1:选择20001通道，并锁定
    if(HI_ERR_SUCCESS != ICC_CHANNEL_LOCK(ICC_USR_CHANNEL_ID_NET_SET,HI_SYS_WAIT_FOREVER))
    {
        return HI_ERR_WAIT_SEM_FAIL;
    }
    //DEBUG:步骤2:创建一个SOCKET
    socket = HI_ICC_SocketCreate(ICC_USR_CHANNEL_ID_NET_SET,HI_SSID_DEFAULT_CON_TIME_OUT_SEC,HI_SSID_DEFAULT_RCV_TIME_OUT_SEC);
    if(socket == HI_SYS_INVALID_SOCKET)
    {
        (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_NET_SET);
        return HI_ERR_ICC_SOCKET_CONNECT;
    }
    //DEBUG:步骤3:发送命令,命令ID:HI_ICC_CMD_FMGR_SET_SSID
    stCmdBufs.ulBufCount=1;
    stCmdBufs.pBuf[0]=(HI_PBYTE)&stUsrCmd;
    stCmdBufs.ulBufLen[0]=sizeof(stUsrCmd);
    ret = HI_ICC_SendCmdHeadData(socket,HI_ICC_CMD_GET_WIFI_SSID,sizeof(HI_GET_SSID_CMD_S),&stCmdBufs);//发一个U32进去
    if(HI_ERR_SUCCESS != ret)
    {
        HI_ICC_SocketClose(socket);
        (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_NET_SET);
        return ret;
    }


    //DEBUG:步骤5:接收从3518返回的iccind报文头，判断是否有通道异常
    ret = HI_ICC_Receive(socket,(HI_PBYTE)&stIccInd,sizeof(stIccInd));
    if(HI_ERR_SUCCESS != ret)
    {
        HI_ICC_SocketClose(socket);
        (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_NET_SET);
        return ret;
    }
    if(stIccInd.ret_sys != HI_ERR_SUCCESS)
    {
        HI_ICC_SocketClose(socket);
        (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_NET_SET);
        return stIccInd.ret_sys;
    }
    //接收3518返回的执行结果
    ret = HI_ICC_Receive(socket,(HI_PBYTE)&stUsrInd,sizeof(stUsrInd));
    if(HI_ERR_SUCCESS != ret)
    {
        HI_ICC_SocketClose(socket);
        (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_NET_SET);
        return ret;
    }

    (hi_void)memcpy_s(ucwifiname, WIFI_SSID_LENGTH+1, stUsrInd.ucwifiname,WIFI_SSID_LENGTH+1);
    //END:
    HI_ICC_SocketClose(socket);
    (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_NET_SET);
    return HI_ERR_SUCCESS;
}

HI_U32 HI_WIFI_On(HI_VOID)
{
    return HI_IO_EXP_SetOutPutVal(IO_EXP_IO_NUM_11,HI_IO_EXP_OUT_VAL_HIGHT);
}

HI_U32 HI_WIFI_Off(HI_VOID)
{
    return HI_IO_EXP_SetOutPutVal(IO_EXP_IO_NUM_11,HI_IO_EXP_OUT_VAL_LOW);

}
#endif
