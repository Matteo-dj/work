//*****************************************************************************
//
//                  ��Ȩ���� (C), 1998-2011, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : drv_ndm_eqt_interfence.c
//  �� �� ��   : V1.0
//  ��    ��   : wanglei/00279578
//  ��������   : 2015-08-20
//  ��������   : �ṩWIFI����������BQ�����ӿ�
//
//  �����б�   : TODO: ...
//  �޸���ʷ   :
//  1.��    �� : 2015-08-20
//    ��    �� : wanglei/00279578
//    �޸����� : �����ļ�
//*****************************************************************************
#include <hi_types.h>
#include <hi_mdm_types.h>
#ifdef PRODUCT_CFG_SUPPORT_WIFI
#include <hi_filemanager.h>
#include <hi_icc.h>
#include <hi_ioexp.h>
#include <hi_stdlib.h>

/***************ħ���ֶ���****************/
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

    //DEBUG:����1:ѡ��20001ͨ����������
    if(HI_ERR_SUCCESS != ICC_CHANNEL_LOCK(ICC_USR_CHANNEL_ID_NET_SET,HI_SYS_WAIT_FOREVER))
    {
        return HI_ERR_WAIT_SEM_FAIL;
    }
    //DEBUG:����2:����һ��SOCKET
    socket = HI_ICC_SocketCreate(ICC_USR_CHANNEL_ID_NET_SET,HI_SSID_DEFAULT_CON_TIME_OUT_SEC,HI_SSID_DEFAULT_RCV_TIME_OUT_SEC);
    if(socket == HI_SYS_INVALID_SOCKET)
    {
        (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_NET_SET);
        return HI_ERR_ICC_SOCKET_CONNECT;
    }
    //DEBUG:����3:��������,����ID:HI_ICC_CMD_FMGR_SET_SSID
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

    //DEBUG:����4:���մ�3518���ص�iccind����ͷ���ж��Ƿ���ͨ���쳣
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
    //����3518���ص�ִ�н��
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

    //DEBUG:����1:ѡ��20001ͨ����������
    if(HI_ERR_SUCCESS != ICC_CHANNEL_LOCK(ICC_USR_CHANNEL_ID_NET_SET,HI_SYS_WAIT_FOREVER))
    {
        return HI_ERR_WAIT_SEM_FAIL;
    }
    //DEBUG:����2:����һ��SOCKET
    socket = HI_ICC_SocketCreate(ICC_USR_CHANNEL_ID_NET_SET,HI_SSID_DEFAULT_CON_TIME_OUT_SEC,HI_SSID_DEFAULT_RCV_TIME_OUT_SEC);
    if(socket == HI_SYS_INVALID_SOCKET)
    {
        (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_NET_SET);
        return HI_ERR_ICC_SOCKET_CONNECT;
    }
    //DEBUG:����3:��������,����ID:HI_ICC_CMD_FMGR_SET_SSID
    stCmdBufs.ulBufCount=1;
    stCmdBufs.pBuf[0]=(HI_PBYTE)&stUsrCmd;
    stCmdBufs.ulBufLen[0]=sizeof(stUsrCmd);
    ret = HI_ICC_SendCmdHeadData(socket,HI_ICC_CMD_GET_WIFI_SSID,sizeof(HI_GET_SSID_CMD_S),&stCmdBufs);//��һ��U32��ȥ
    if(HI_ERR_SUCCESS != ret)
    {
        HI_ICC_SocketClose(socket);
        (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_NET_SET);
        return ret;
    }


    //DEBUG:����5:���մ�3518���ص�iccind����ͷ���ж��Ƿ���ͨ���쳣
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
    //����3518���ص�ִ�н��
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
