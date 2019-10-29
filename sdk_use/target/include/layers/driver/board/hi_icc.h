//****************************************************************************
//
//                  版权所有 (C), 2001-2011, 华为技术有限公司
//
//****************************************************************************
//  文 件 名   : icc.h
//  版 本 号   : V1.0
//  作    者   : 边海文/00208046
//  生成日期   : 2015/02/05
//  功能描述   : 用于板间通信
//  函数列表   :
//  修改历史   :
//****************************************************************************
#ifndef __HI_ICC_H__
#define __HI_ICC_H__
#include <hi_types.h>
#include <hi_mdm_types.h>


typedef enum
{
    ICC_CHANNEL_ID_20000,
    ICC_CHANNEL_ID_20001,
    ICC_CHANNEL_ID_MAX,
}ICC_CHANNEL_ID_E;//应用不要直接使用这些ID

#define ICC_USR_CHANNEL_ID_FMGR ICC_CHANNEL_ID_20000
#define ICC_USR_CHANNEL_ID_UPG  ICC_CHANNEL_ID_20001
#define ICC_USR_CHANNEL_ID_NET_SET  ICC_CHANNEL_ID_20001

typedef enum
{
    HI_ICC_CMD_UNSUPPORT = 0,
    HI_ICC_CMD_UPG_ST = 1,
    HI_ICC_CMD_UPG_GET_VER = HI_ICC_CMD_UPG_ST,
    HI_ICC_CMD_UPG_TRANS_FILE,

    HI_ICC_CMD_UPG_END,

    //FILE MANAGER命令范围[100,150)
    HI_ICC_CMD_FMGR_ST = 100,
    HI_ICC_CMD_FMGR_CREATE = HI_ICC_CMD_FMGR_ST,
    HI_ICC_CMD_FMGR_INIT,
	HI_ICC_CMD_FMGR_OPEN,
	HI_ICC_CMD_FMGR_WRITE,
	HI_ICC_CMD_FMGR_READ,
	HI_ICC_CMD_FMGR_CLOSE,
	HI_ICC_CMD_FMGR_DELETE,//通过system+access获取
	HI_ICC_CMD_FMGR_SEEK,
	HI_ICC_CMD_FMGR_GET_SIZE,//通过statfs获取
	HI_ICC_CMD_FMGR_IS_EXIST,//通过statfs获取
	HI_ICC_CMD_FMGR_STAT,
	HI_ICC_CMD_FMGR_STAT_FS,
	HI_ICC_CMD_FMGR_ACCESS,
	HI_ICC_CMD_FMGR_SYSOUT,
	HI_ICC_CMD_FMGR_SET_TIME,
	HI_ICC_CMD_SET_WIFI_SSID,//设置WIFI名字
	HI_ICC_CMD_GET_WIFI_SSID,//获取WIFI名字
	HI_ICC_CMD_FMGR_END,
    HI_ICC_CMD_XXX_ST = 200,
}HI_ICC_CMD_E;//ICC命令应答ID分配表格


typedef struct
{
    HI_U32 ulBufCount;
    HI_PBYTE pBuf[3];
    HI_U32 ulBufLen[3];
}HI_ICC_CMD_DATA_BUF_S;

typedef struct
{
    HI_U32 magicNum;//魔术字
    HI_U16 cmdId;//命令ID
    HI_U16 reserver;
    HI_U32 dataSize; //数据大小不含头信息
    HI_U8 data[0];//数据
}HI_ICC_CMD_PKT_S;//ICC命令报文头

typedef struct
{
    HI_U32 magicNum;//魔术字
    HI_U16 cmdId;//命令ID
    HI_U16 reserver;
    HI_U32 ret_sys;//通道|系统错误
    HI_U32 dataSize;//数据大小不含头信息
    HI_U32 data[0];//数据
}HI_ICC_IND_PKT_S;//ICC应答报文头

HI_U32 ICC_CHANNEL_LOCK(ICC_CHANNEL_ID_E chan_id,HI_U32 ulTimeOut);
HI_VOID ICC_CHANNEL_UNLOCK(ICC_CHANNEL_ID_E chan_id);

#if defined (PORDUCT_CFG_SUPPORT_ICC)
HI_U32 HI_ICC_Init(HI_U8 aucClintIp[4]);
HI_U32 HI_ICC_Send(HI_IN HI_S32 socket,HI_IN HI_PBYTE buf,HI_IN HI_S32 totalLen);
HI_U32 HI_ICC_Receive(HI_IN HI_S32 socket,HI_OUT HI_PBYTE buf,HI_IN HI_S32 totalLen);
HI_SOCKET HI_ICC_SocketCreate(ICC_CHANNEL_ID_E enChannelId,HI_U32 ulConnectTimeOutSec,HI_U32 ulRecvSendTimeOutSec);
HI_U32 HI_ICC_SocketClose(HI_SOCKET s);
HI_U32 HI_ICC_SendCmdHead(HI_S32 socket,HI_U16 cmdId,HI_U32 ulTotalUsrDataSize);
HI_U32 HI_ICC_SendCmdHeadData(HI_S32 socket,HI_U16 cmdId,HI_U32 ulTotalUsrDataSize,HI_ICC_CMD_DATA_BUF_S *pstBufs);
HI_U32 HI_ICC_ReceiveIndPkt(HI_S32 socket,HI_U32 *pCmdId,HI_U32 *pUsrDataSize);
HI_BOOL HI_ICC_GetConnectStat(HI_IN ICC_CHANNEL_ID_E enChannelId);
HI_U32 HI_ICC_SetConnectStat(HI_IN ICC_CHANNEL_ID_E enChannelId,HI_BOOL bConnect);


#else//defined (PRODUCT_CFG_ICC_FMGR_SUPPORT) && !defined (PRODUCT_CFG_EQUIP_TEST_MODE)
HI_PRVL HI_U32 HI_ICC_Init(HI_U8 aucClintIp[4]){return HI_ERR_NOT_SUPPORT;}
HI_PRVL HI_U32 HI_ICC_Send(HI_IN HI_S32 socket,HI_IN HI_PBYTE buf,HI_IN HI_S32 totalLen){return HI_ERR_NOT_SUPPORT;}
HI_PRVL HI_U32 HI_ICC_Receive(HI_IN HI_S32 socket,HI_OUT HI_PBYTE buf,HI_IN HI_S32 totalLen){return HI_ERR_NOT_SUPPORT;}
HI_PRVL HI_SOCKET HI_ICC_SocketCreate(HI_IN ICC_CHANNEL_ID_E enChannelId,HI_IN HI_U32 ulConnectTimeOutSec,HI_IN HI_U32 ulRecvSendTimeOutSec){return HI_SYS_INVALID_SOCKET;}
HI_PRVL HI_U32 HI_ICC_SocketClose(HI_IN HI_SOCKET s){return HI_SYS_SOCKET_ERROR;}
HI_PRVL HI_U32 HI_ICC_SendCmdHead(HI_S32 socket,HI_U16 cmdId,HI_U32 ulTotalUsrDataSize){return HI_ERR_NOT_SUPPORT;}
HI_PRVL HI_U32 HI_ICC_SendCmdHeadData(HI_S32 socket,HI_U16 cmdId,HI_U32 ulTotalUsrDataSize,HI_ICC_CMD_DATA_BUF_S *pstBufs){return HI_ERR_NOT_SUPPORT;}
HI_PRVL HI_U32 HI_ICC_ReceiveIndPkt(HI_S32 socket,HI_U32 *pCmdId,HI_U32 *pUsrDataSize){return HI_ERR_NOT_SUPPORT;}
HI_PRVL HI_BOOL HI_ICC_GetConnectStat(HI_IN ICC_CHANNEL_ID_E enChannelId){return HI_FALSE;}
HI_PRVL HI_U32 HI_ICC_SetConnectStat(HI_IN ICC_CHANNEL_ID_E enChannelId,HI_BOOL bConnect){return HI_ERR_NOT_SUPPORT;}
#endif//defined (PRODUCT_CFG_ICC_FMGR_SUPPORT) && !defined (PRODUCT_CFG_EQUIP_TEST_MODE)
#endif//__HI_ICC_H__

