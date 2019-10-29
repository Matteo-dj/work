#include <hi_types.h>
#include <hi_mdm_types.h>

#ifdef PRODUCT_CFG_SUPPORT_3518
#include <hi_icc.h>
#include <icc_protocal_upg.h>
#include <hi_algorithm.h>
#include "3518_debug.h"
#define HI_3518_UPG_DEFAULT_RCV_TIME_OUT_SEC 5
#define HI_3518_UPG_DEFAULT_CON_TIME_OUT_SEC 5
HI_U32 HI_3518_UPG_TransFile(HI_U8 *pFile,HI_U32 ulFileSize,HI_U32 ulVer)
{
    HI_U32 ulCrc=0;
    HI_S32 socket = HI_SYS_INVALID_SOCKET;
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_3518_UPG_HEAD_S stUpgHead = {0};
    HI_ICC_IND_PKT_S stIccInd = {0};
    HI_3518_UPG_IND_S stUsrInd = {0};
    HI_ICC_CMD_DATA_BUF_S stCmdBufs={0};
    HI_U32 ulUsrCmdLen=0;
    
    ulCrc = HI_MDM_Crc32(0,pFile,ulFileSize);

    //填充UPG HEAD
    stUpgHead.magic = HI_3518_UPG_MAGIC;
    stUpgHead.ver = ulVer;
    stUpgHead.dataSize = ulFileSize;
    stUpgHead.crc = ulCrc;

    //锁通道
    if(HI_ERR_SUCCESS != ICC_CHANNEL_LOCK(ICC_USR_CHANNEL_ID_UPG,HI_SYS_WAIT_FOREVER))
    {
        return HI_ERR_WAIT_SEM_FAIL;
    }


    socket = HI_ICC_SocketCreate(ICC_USR_CHANNEL_ID_UPG,HI_3518_UPG_DEFAULT_CON_TIME_OUT_SEC,HI_3518_UPG_DEFAULT_RCV_TIME_OUT_SEC);
    if(socket == HI_SYS_INVALID_SOCKET)
    {
        ret = HI_ERR_ICC_SOCKET_CONNECT;
        HI_3518_UPG_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
        goto resume2;
    }


    //发送系统数据
    ulUsrCmdLen = sizeof(stUpgHead)+stUpgHead.dataSize;

    stCmdBufs.ulBufCount=2;
    stCmdBufs.pBuf[0]=(HI_PBYTE)&stUpgHead;
    stCmdBufs.ulBufLen[0]=sizeof(stUpgHead);
    stCmdBufs.pBuf[1]=pFile;
    stCmdBufs.ulBufLen[1]=stUpgHead.dataSize;
    ret = HI_ICC_SendCmdHeadData(socket,HI_ICC_CMD_UPG_TRANS_FILE,ulUsrCmdLen,&stCmdBufs);
    if(HI_ERR_SUCCESS != ret)
    {
        HI_3518_UPG_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
        goto resume3;
    }


    //接收系统IND
    ret = HI_ICC_Receive(socket,(HI_PBYTE)&stIccInd,sizeof(stIccInd));
    if(HI_ERR_SUCCESS != ret)
    {
        HI_3518_UPG_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
        goto resume3;
    }
    if(stIccInd.ret_sys!=HI_ERR_SUCCESS)
    {
        ret  = stIccInd.ret_sys;
        HI_3518_UPG_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
        goto resume3;
    }

    //接收用户IND
    ret = HI_ICC_Receive(socket,(HI_PBYTE)&stUsrInd,sizeof(stUsrInd));
    if(HI_ERR_SUCCESS != ret)
    {
        HI_3518_UPG_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
        goto resume3;

    }
    ret =HI_ERR_SUCCESS;
resume3:
    HI_ICC_SocketClose(socket);
resume2:
    (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_UPG);
    return ret;
}
#endif//PRODUCT_CFG_SUPPORT_3518

