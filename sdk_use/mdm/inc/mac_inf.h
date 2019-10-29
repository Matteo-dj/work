//****************************************************************************
//
//                  版权所有 (C), 2001-2011, 华为技术有限公司
//
//****************************************************************************
//  文 件 名   : mac_inf.h
//  版 本 号   : V1.0
//  作    者   : 沈汉坤/00130424
//  生成日期   : 2011年10月10日
//  功能描述   : MAC接口定义,仅供 MAC/SAL/DRV/OS使用,APP禁止使用.
//  函数列表   :
//  修改历史   :
//  1.日    期 : 2011年10月10日
//    作    者 : 沈汉坤/00130424
//    修改内容 : 创建文件
//
//****************************************************************************

#ifndef __MAC_INF_H__
#define __MAC_INF_H__

#include <hi_types.h>
#include "hi_mdm_mac.h"
HI_START_HEADER
//*****************************************************************************

typedef struct
{
    HI_U8 ucSrcMac[HI_PLC_MAC_ADDR_LEN];
    HI_U8 ucDestMac[HI_PLC_MAC_ADDR_LEN];
} DMS_MAC_FRAME2_S;



typedef struct
{
    HI_U8   ucSrcMac[HI_PLC_MAC_ADDR_LEN];		// 源MAC地址
    HI_U8   ucDestMac[HI_PLC_MAC_ADDR_LEN];		// 目的MAC地址

    HI_U16  Mtype;							    // 管理报文类型
    HI_U8   MacFrameHeadVer;   					// MAC帧头类型
    HI_U8   MacHeadType;                        // MAC帧头类型

    HI_BOOL IsDefaultLid;                       // 是否使用默认优先级
    HI_U8   Lid;								// 优先级
    HI_U8   Hopcnt;							    // 最大跳数
    HI_U8   BroadcastForwardMode;				// 广播报文转发规则

    HI_U8   Phase;                              // 配置报文发送相位
    HI_BOOL is_fix_pk_index;                    // 是否为快速发送报文
    HI_U8   Snid;                               // 配置报文发送的网络号
    HI_U8 ndm_special_ver;            // 是否为抄控器诊断数据

    HI_U32  Timeout;							// 报文过期时间

    HI_U16  ProtocolVer;						// 协议类型
    HI_U8   MaxResendCnt;						// 最大重传次数
    HI_BOOL IsNeigHborCommunication;            // 是否跨网络通信
}HI_MAC_FRAME_CTRL_STRU;

#define MAC_DEFAULT_BROADCAST_FORWARD_MODE			(0)		// 使用默认广播转发模式
#define MAC_LOCAL_BROADCAST_MODE                    (1)     // 本地广播模式
#define MAC_BROADCAST_PROXY_FORWARD_MODE			(2)		// 广播报文代理转发模式
#define MAC_BROADCAST_ALL_STATION_FORWARD_MODE		(3)		// 广播报文所有站点转发模式

typedef struct
{
	HI_U8   DefalutLid;			// 默认优先级
	HI_BOOL IsLed;				// 是否点灯
	HI_BOOL IsZeroCopy;         // 是否零拷贝
	HI_BOOL multi_protocol;     // 多协议接收

	MAC_PACKET_INPUT_FUN InputFun;	// 接收处理函数
}HI_MDM_CHANNEL_INFO_STRU;

HI_U32 HI_MDM_RegisterChannel(HI_IN HI_MDM_CHANNEL_INFO_STRU *pMacChannelInfo,HI_IN HI_U8 ChannelType);
HI_U32 HI_MDM_MacSendFrame(HI_IN HI_PVOID Frame, HI_U16 FrameSize,
                                            HI_IN HI_MAC_FRAME_CTRL_STRU *pFrameCtrl,HI_IN HI_PVOID Buf);

//*****************************************************************************
// 函数名称: DMS_SendFrame MAC_SendFrame
// 功能描述: MAC层和DMS的以太网帧发送和接收接口
//           DMS_SendFrame : MAC --> DMS
//           MAC_SendFrame : DMS --> MAC
//
// 参数说明:
//   pFrame [in] : DMS和MAC的帧定义
//   usFrameSize [in] : ASL和MAC的帧帧大小，单位字节
//
// 返 回 值:
//    HI_ERR_SUCCESS : 表示成功
//
// 调用要求: DMS_SendFrame接口仅供MAC层使用 MAC_SendFrame接口仅供DMS层使用
// 调用举例: TODO: ...
//*****************************************************************************
#if defined(PRODUCT_CFG_COMPATIBILITY_NEW_FEATURE)
HI_EAPI HI_U32 DMS_SendFrame(HI_PVOID pFrame, HI_U16 usFrameSize,HI_PLC_FRAME_CTRL_STRU* pFrameCtrl,HI_PVOID pBuf);
HI_EAPI HI_U32 MAC_SendFrame(HI_IN HI_PVOID pFrame, HI_U16 usFrameSize, HI_IN DMS_MAC_FRAME_CTRL_S* pstFrmCtrl);
#else
HI_EAPI HI_U32 DMS_SendFrame(HI_IN HI_PVOID pFrame, HI_U16 usFrameSize);
HI_EAPI HI_U32 MAC_SendFrame(HI_IN HI_PVOID pFrame, HI_U16 usFrameSize);
#endif

//*****************************************************************************
// 函数名称: DMS_SendFrame MAC_SendMessage
// 功能描述: MAC层和DMS的消息发送和接收接口
//           DMS_SendMessage : MAC --> DMS
//           MAC_SendMessage : DMS --> MAC
//
// 参数说明:
//   ulMsgId [in] : 消息ID,取值为 ID_MSG _XXX(HI_MSG_ND_REMOTE_PACKET/ID_MSG_ROUTER_STATUS_IND/ID_MSG_DIAG_CMD) 或是 HI_CHL_MSG _XXX(HI_CHL_MSG_UNKNOWN)
//   pMsgParam [in] : 消息参数, 大小由依赖于消息ID对应的数据结构, 不允许为空
//
// 返 回 值:
//    HI_ERR_SUCCESS : 表示成功
//
// 调用要求: DMS_SendMessage接口仅供MAC使用 MAC_SendMessage接口仅供DMS使用
// 调用举例: TODO: ...
//*****************************************************************************
HI_EAPI HI_U32 DMS_SendMessage(HI_U32 ulMsgId, HI_IN HI_PVOID pMsgParam);
HI_EAPI HI_U32 MAC_SendMessage(HI_U32 ulMsgId, HI_IN HI_PVOID pMsgParam);
//*****************************************************************************


//*****************************************************************************
#if defined(PRODUCT_CFG_OS_WIN) && defined(PRODUCT_CFG_FEATURE_STUB_NO_SRC_MAC) && !defined(PRODUCT_CFG_HAVE_WIN_STUB_API)
#define MAC_STUB_FUNC                        HI_DLL_FUNC("sal", DMS_GetMacAdpInterface)
#define MAC_STUB_INF                         ((DMS_MAC_ADP_STRU*)(HI_DLL_INF("sal", DMS_GetMacAdpInterface)))
#define MAC_STUB_INF_SENDFRAME               MAC_STUB_INF->pfnSendFrame
#define MAC_STUB_INF_SENDMESSAGE             MAC_STUB_INF->pfnSendMessage
#define MAC_STUB_INF_GETINFO                 MAC_STUB_INF->pfnGetConfigInfo
#define MAC_SENDFRAME(pFrame, usFrameSize)   MAC_STUB_FUNC?(MAC_STUB_INF?(MAC_STUB_INF_SENDFRAME?MAC_STUB_INF_SENDFRAME(pFrame, usFrameSize): HI_ERR_INITILIZATION):HI_ERR_INITILIZATION):HI_ERR_INITILIZATION
#define MAC_SENDMESSAGE(ulMsgId, pMsgParam)  MAC_STUB_FUNC?(MAC_STUB_INF?(MAC_STUB_INF_SENDMESSAGE?MAC_STUB_INF_SENDMESSAGE(ulMsgId, pMsgParam): HI_ERR_INITILIZATION):HI_ERR_INITILIZATION):HI_ERR_INITILIZATION
#else
#if defined(PRODUCT_CFG_FEATURE_MAC_TASK)
#if defined(PRODUCT_CFG_COMPATIBILITY_NEW_FEATURE)
#define MAC_SENDFRAME(pFrame, usFrameSize, pstFrmCtrl)   MAC_SendFrame(pFrame, usFrameSize, pstFrmCtrl)
#define MAC_SENDMESSAGE(ulMsgId, pMsgParam)              MAC_SendMessage(ulMsgId, pMsgParam)
#else
#define MAC_SENDFRAME(pFrame, usFrameSize)               MAC_SendFrame(pFrame, usFrameSize)
#define MAC_SENDMESSAGE(ulMsgId, pMsgParam)              MAC_SendMessage(ulMsgId, pMsgParam)
#endif
#else
/* BEGIN: Modified by liujian 00280874, 2016/4/19   问题单号:PHY*/
#if defined(PRODUCT_CFG_COMPATIBILITY_NEW_FEATURE)
#define MAC_SENDFRAME(pFrame, usFrameSize, pstFrmCtrl)         0
#define MAC_SENDMESSAGE(ulMsgId, pMsgParam)                        0
#else
#define MAC_SENDFRAME(pFrame, usFrameSize)               0
#define MAC_SENDMESSAGE(ulMsgId, pMsgParam)              0
#endif
/* END:   Modified by liujian 00280874, 2016/4/19 */
#endif
#endif

#if defined(PRODUCT_CFG_COMPATIBILITY_NEW_FEATURE)
#define SEND_DMSFRAME2MAC(pucFrm, usFrmSize)  MAC_SENDFRAME(pucFrm, usFrmSize, &stFrmCtrl)
#else
#define SEND_DMSFRAME2MAC(pucFrm, usFrmSize)  MAC_SENDFRAME(pucFrm, usFrmSize)
#endif

#if defined(PRODUCT_CFG_OS_WIN) && defined(PRODUCT_CFG_FEATURE_STUB_NO_SRC_MAC) && !defined(PRODUCT_CFG_HAVE_WIN_STUB_API)
#define MAC_INIT() if(HI_DLL_FUNC("sal", MAC_Init)) {HI_DLL_INF("sal", MAC_Init);} if(HI_DLL_FUNC("sal_st", MAC_Init)) {HI_DLL_INF("sal_st", MAC_Init);} if(HI_DLL_FUNC("app", MAC_Init)) {HI_DLL_INF("app", MAC_Init);}
#elif defined(PRODUCT_CFG_OS_WIN) && defined(PRODUCT_CFG_FEATURE_STUB_NO_SRC_MAC)
#define MAC_INIT() if(HI_DLL_FUNC("sal", MAC_Init)) {HI_DLL_INF("sal", MAC_Init);} if(HI_DLL_FUNC("sal_st", MAC_Init)) {HI_DLL_INF("sal_st", MAC_Init);} if(HI_DLL_FUNC("app", MAC_Init)) {HI_DLL_INF("app", MAC_Init);}
#else
#define MAC_INIT()
#endif
//*****************************************************************************


//*****************************************************************************
enum
{
    ID_MSG_MAC_BEGIN = 0x100,
    ID_MSG_POWERON_REQ,
    ID_MSG_POWERON_CNF,
    ID_MSG_POWEROFF_REQ,
    ID_MSG_POWEROFF_CNF,
    ID_MSG_ROUTER_STATUS_IND,
    ID_MSG_DIAG_CMD,
    ID_MSG_NV_REFRESH_NOTIFY,

    ID_MSG_DFX_CTRL_PK,
    ID_MSG_DFX_NDM_NOTIFY,
    ID_MSG_DFX_QUERY_NOTIFY,
#ifdef PRODUCT_CFG_ONLINE_TEST_MODE
	ID_MSG_DFX_GT_RX_NOTIFY,
    ID_MSG_DFX_GT_TIMEOUT_NOTIFY,
    ID_MSG_DFX_GT_RX_BEACON_NOTIFY,
    ID_MSG_DFX_GT_NTB_NOTIFY,
#endif
    ID_MSG_DFX_MGT_MSG,

    ID_MSG_TX_STATUS_IND,               // MSG_TX_STATUS_IND_S
    ID_MSG_ASSOC_SYS_INFO_IND,          // MSG_ASSOC_SYS_INFO_IND_S
    ID_MSG_TX_FRM_QRY_IND,              // MSG_TX_FRM_QRY_IND_S
    ID_MSG_BIDB_CHANGED,                // MSG_BIDB_CHANGED_S
    ID_MSG_MAC_CTRL,                    // MSG_MAC_CTRL_S
    /* BEGIN: Added by likunhe/00194117, 2013/9/8   PN:DTS2013070208139*/
    ID_MSG_DFX_CONNECT_PK,              //抄控器通道的连接报文
    /* END:   Added by likunhe/00194117, 2013/9/8 */
    ID_MSG_RSB_REASSEMBLE_NOTIFY,
    ID_MSG_MAC_END = 0x400,

    /* BEGIN: Added by likunhe/00194117, 2013/9/11   PN:DTS2013092304717*/
    //NDA
    ID_MSG_NFC_BEGIN = 0x500,
    ID_MSG_NFC_REQ_APK,                 // 抄控器通道转发请求给报文流控通道-->NDM
    ID_MSG_NFC_RCV_APK,                 // CCO/STA-->NDM
    ID_MSG_NFC_MGT,                     // 内部管理消息
    ID_MSG_NFC_END = 0x600,
    /* END:   Added by likunhe/00194117, 2013/9/11 */
};


typedef struct
{
    HI_BOOL bRouterAvail;
    HI_BOOL bIsCco;
    HI_U16  usTEI;  // 仅低12bit有效
    HI_U8   ucMac[HI_PLC_MAC_ADDR_LEN];
    HI_U8   ucCcoMac[HI_PLC_MAC_ADDR_LEN];
} MSG_ROUTER_STATUS_IND_STRU;


typedef struct
{
    HI_U16 usId; // DIAG 包ID
    HI_U16 usPayloadSize;
    HI_BYTE pucPayload[0];
} MSG_DIAG_CMD_STRU;


#define NV_NOTIFY_NUM_MAX  4
typedef struct
{
    HI_U16 ausNvid[NV_NOTIFY_NUM_MAX];
} MSG_NV_REFRESH_NOTIFY_STRU;


typedef struct
{
    HI_U32 ulIdx;
    HI_U16 bPbSize;
    HI_U16 usArqCnt;
    HI_U32 ulErr;
    HI_U32 ulTimeStamp;
} MSG_TX_STATUS_IND_S;

typedef struct
{
    HI_U8 ucData[HI_DMS_PLC_FRM_LEN2-1];
    HI_U8 ucDataSize;
} MSG_TX_FRM_QRY_IND_S;

typedef HI_SYS_STATUS_INFO_S MSG_ASSOC_SYS_INFO_IND_S;


typedef struct
{
    HI_U32 ulRc;
    HI_U8 reserved[8];
} MSG_BIDB_CHANGED_S;

typedef struct
{

    HI_U8 ucLayer;      // 当前节点所在层级数
    HI_U8 ucChildLayer;  // 当前节点带子节点的层级数
    HI_U16 usChildNum;   // 当前节点的子节点个数，0表示没有子节点
    HI_U16 usTei;        // 当前模块的TEI
} MAC_NET_PARAM_QRY_S;

typedef struct
{
    HI_DMS_PORT_CTRL_OPT_ID_E enCtrlId;
    HI_PVOID pInputParam;
    HI_PVOID pOutputParam;
}MSG_MAC_CTRL_S;

//台区识别NDM-NDM信息
typedef struct
{
    //HI_BOOL ndm_identify_mode_switch;                      //台区识别ndm模式使能开关
    HI_BOOL ndm_identify_rx_mode_switch;                     // 台区识别接收机使能开关
    HI_BOOL ndm_identify_tx_mode_switch;                     // 台区识别发射机使能开关
    HI_BOOL ndm_identify_strength_switch;                    // ndm台区识别强度使能开关
    HI_U8 transformer_index;
    HI_U8 phase_index;
    HI_U8 pad[3];

    HI_U32 start_time_stamp;                                       //ndm台区识别开始时间戳
    HI_U32 stop_time_stamp;                                       //ndm台区识别停止时间戳

    HI_U32 rsvd[5];
}tf_identify_ndm_info;


HI_U32 MAC_QueryNetParam(HI_OUT MAC_NET_PARAM_QRY_S* pstNetParam);

/* BEGIN: Added by likunhe/00194117, 2014/1/14   PN:DTS2013120902593*/

/*****************************************************************************
 函 数 名  : HI_MAC_OpenAfeRxHpf
 功能描述  : 打开AFE的接收高通滤波器
 输入参数  : HI_VOID
 输出参数  : 无
 返 回 值  : HI_VOID
 本函数调用的函数  :
 调用本函数的函数  :
 访问的全局变量  :
 修改的全局变量  :
 修改的BBP寄存器列表  :

 修改历史      :
  1.日    期   : 2014年1月6日
    作    者   : likunhe/00194117
    修改内容   : 新生成函数

*****************************************************************************/
HI_VOID HI_MAC_OpenAfeRxHpf(HI_VOID);

/*****************************************************************************
 函 数 名  : HI_MAC_CloseAfeRxHpf
 功能描述  : 关闭AFE的接收高通滤波器
 输入参数  : HI_VOID
 输出参数  : 无
 返 回 值  : HI_VOID
 本函数调用的函数  :
 调用本函数的函数  :
 访问的全局变量  :
 修改的全局变量  :
 修改的BBP寄存器列表  :

 修改历史      :
  1.日    期   : 2014年1月6日
    作    者   : likunhe/00194117
    修改内容   : 新生成函数

*****************************************************************************/
HI_VOID HI_MAC_CloseAfeRxHpf(HI_VOID);
HI_U32 HI_MAC_GetRxBeaconCnt(HI_VOID);
//*****************************************************************************

#if defined(PRODUCT_CFG_PRODUCT_TYPE_NDM)
HI_EXTERN tf_identify_ndm_info g_tf_ndm_info;
HI_U32 tf_enable_rx_mode_switch(HI_IN HI_BOOL mode_switch);
HI_U32 tf_enable_tx_mode_switch(HI_IN HI_BOOL mode_switch,HI_IN HI_U8 stop_reason);
HI_U32 tf_ndm_switch_strength_mode(HI_IN HI_BOOL mode_switch);
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
HI_VOID HI_MAC_GetNtbCollectAddr(HI_U32 *pAddr,HI_U32 *pSize);
#endif

HI_END_HEADER
#endif // __MAC_INF_H__

