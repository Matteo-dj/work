//****************************************************************************
//
//                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾
//
//****************************************************************************
//  �� �� ��   : icc.h
//  �� �� ��   : V1.0
//  ��    ��   : �ߺ���/00208046
//  ��������   : 2015/02/05
//  ��������   : ���ڰ��ͨ��
//  �����б�   :
//  �޸���ʷ   :
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
}ICC_CHANNEL_ID_E;//Ӧ�ò�Ҫֱ��ʹ����ЩID

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

    //FILE MANAGER���Χ[100,150)
    HI_ICC_CMD_FMGR_ST = 100,
    HI_ICC_CMD_FMGR_CREATE = HI_ICC_CMD_FMGR_ST,
    HI_ICC_CMD_FMGR_INIT,
	HI_ICC_CMD_FMGR_OPEN,
	HI_ICC_CMD_FMGR_WRITE,
	HI_ICC_CMD_FMGR_READ,
	HI_ICC_CMD_FMGR_CLOSE,
	HI_ICC_CMD_FMGR_DELETE,//ͨ��system+access��ȡ
	HI_ICC_CMD_FMGR_SEEK,
	HI_ICC_CMD_FMGR_GET_SIZE,//ͨ��statfs��ȡ
	HI_ICC_CMD_FMGR_IS_EXIST,//ͨ��statfs��ȡ
	HI_ICC_CMD_FMGR_STAT,
	HI_ICC_CMD_FMGR_STAT_FS,
	HI_ICC_CMD_FMGR_ACCESS,
	HI_ICC_CMD_FMGR_SYSOUT,
	HI_ICC_CMD_FMGR_SET_TIME,
	HI_ICC_CMD_SET_WIFI_SSID,//����WIFI����
	HI_ICC_CMD_GET_WIFI_SSID,//��ȡWIFI����
	HI_ICC_CMD_FMGR_END,
    HI_ICC_CMD_XXX_ST = 200,
}HI_ICC_CMD_E;//ICC����Ӧ��ID������


typedef struct
{
    HI_U32 ulBufCount;
    HI_PBYTE pBuf[3];
    HI_U32 ulBufLen[3];
}HI_ICC_CMD_DATA_BUF_S;

typedef struct
{
    HI_U32 magicNum;//ħ����
    HI_U16 cmdId;//����ID
    HI_U16 reserver;
    HI_U32 dataSize; //���ݴ�С����ͷ��Ϣ
    HI_U8 data[0];//����
}HI_ICC_CMD_PKT_S;//ICC�����ͷ

typedef struct
{
    HI_U32 magicNum;//ħ����
    HI_U16 cmdId;//����ID
    HI_U16 reserver;
    HI_U32 ret_sys;//ͨ��|ϵͳ����
    HI_U32 dataSize;//���ݴ�С����ͷ��Ϣ
    HI_U32 data[0];//����
}HI_ICC_IND_PKT_S;//ICCӦ����ͷ

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

