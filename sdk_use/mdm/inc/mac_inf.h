//****************************************************************************
//
//                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾
//
//****************************************************************************
//  �� �� ��   : mac_inf.h
//  �� �� ��   : V1.0
//  ��    ��   : ����/00130424
//  ��������   : 2011��10��10��
//  ��������   : MAC�ӿڶ���,���� MAC/SAL/DRV/OSʹ��,APP��ֹʹ��.
//  �����б�   :
//  �޸���ʷ   :
//  1.��    �� : 2011��10��10��
//    ��    �� : ����/00130424
//    �޸����� : �����ļ�
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
    HI_U8   ucSrcMac[HI_PLC_MAC_ADDR_LEN];		// ԴMAC��ַ
    HI_U8   ucDestMac[HI_PLC_MAC_ADDR_LEN];		// Ŀ��MAC��ַ

    HI_U16  Mtype;							    // ����������
    HI_U8   MacFrameHeadVer;   					// MAC֡ͷ����
    HI_U8   MacHeadType;                        // MAC֡ͷ����

    HI_BOOL IsDefaultLid;                       // �Ƿ�ʹ��Ĭ�����ȼ�
    HI_U8   Lid;								// ���ȼ�
    HI_U8   Hopcnt;							    // �������
    HI_U8   BroadcastForwardMode;				// �㲥����ת������

    HI_U8   Phase;                              // ���ñ��ķ�����λ
    HI_BOOL is_fix_pk_index;                    // �Ƿ�Ϊ���ٷ��ͱ���
    HI_U8   Snid;                               // ���ñ��ķ��͵������
    HI_U8 ndm_special_ver;            // �Ƿ�Ϊ�������������

    HI_U32  Timeout;							// ���Ĺ���ʱ��

    HI_U16  ProtocolVer;						// Э������
    HI_U8   MaxResendCnt;						// ����ش�����
    HI_BOOL IsNeigHborCommunication;            // �Ƿ������ͨ��
}HI_MAC_FRAME_CTRL_STRU;

#define MAC_DEFAULT_BROADCAST_FORWARD_MODE			(0)		// ʹ��Ĭ�Ϲ㲥ת��ģʽ
#define MAC_LOCAL_BROADCAST_MODE                    (1)     // ���ع㲥ģʽ
#define MAC_BROADCAST_PROXY_FORWARD_MODE			(2)		// �㲥���Ĵ���ת��ģʽ
#define MAC_BROADCAST_ALL_STATION_FORWARD_MODE		(3)		// �㲥��������վ��ת��ģʽ

typedef struct
{
	HI_U8   DefalutLid;			// Ĭ�����ȼ�
	HI_BOOL IsLed;				// �Ƿ���
	HI_BOOL IsZeroCopy;         // �Ƿ��㿽��
	HI_BOOL multi_protocol;     // ��Э�����

	MAC_PACKET_INPUT_FUN InputFun;	// ���մ�����
}HI_MDM_CHANNEL_INFO_STRU;

HI_U32 HI_MDM_RegisterChannel(HI_IN HI_MDM_CHANNEL_INFO_STRU *pMacChannelInfo,HI_IN HI_U8 ChannelType);
HI_U32 HI_MDM_MacSendFrame(HI_IN HI_PVOID Frame, HI_U16 FrameSize,
                                            HI_IN HI_MAC_FRAME_CTRL_STRU *pFrameCtrl,HI_IN HI_PVOID Buf);

//*****************************************************************************
// ��������: DMS_SendFrame MAC_SendFrame
// ��������: MAC���DMS����̫��֡���ͺͽ��սӿ�
//           DMS_SendFrame : MAC --> DMS
//           MAC_SendFrame : DMS --> MAC
//
// ����˵��:
//   pFrame [in] : DMS��MAC��֡����
//   usFrameSize [in] : ASL��MAC��֡֡��С����λ�ֽ�
//
// �� �� ֵ:
//    HI_ERR_SUCCESS : ��ʾ�ɹ�
//
// ����Ҫ��: DMS_SendFrame�ӿڽ���MAC��ʹ�� MAC_SendFrame�ӿڽ���DMS��ʹ��
// ���þ���: TODO: ...
//*****************************************************************************
#if defined(PRODUCT_CFG_COMPATIBILITY_NEW_FEATURE)
HI_EAPI HI_U32 DMS_SendFrame(HI_PVOID pFrame, HI_U16 usFrameSize,HI_PLC_FRAME_CTRL_STRU* pFrameCtrl,HI_PVOID pBuf);
HI_EAPI HI_U32 MAC_SendFrame(HI_IN HI_PVOID pFrame, HI_U16 usFrameSize, HI_IN DMS_MAC_FRAME_CTRL_S* pstFrmCtrl);
#else
HI_EAPI HI_U32 DMS_SendFrame(HI_IN HI_PVOID pFrame, HI_U16 usFrameSize);
HI_EAPI HI_U32 MAC_SendFrame(HI_IN HI_PVOID pFrame, HI_U16 usFrameSize);
#endif

//*****************************************************************************
// ��������: DMS_SendFrame MAC_SendMessage
// ��������: MAC���DMS����Ϣ���ͺͽ��սӿ�
//           DMS_SendMessage : MAC --> DMS
//           MAC_SendMessage : DMS --> MAC
//
// ����˵��:
//   ulMsgId [in] : ��ϢID,ȡֵΪ ID_MSG _XXX(HI_MSG_ND_REMOTE_PACKET/ID_MSG_ROUTER_STATUS_IND/ID_MSG_DIAG_CMD) ���� HI_CHL_MSG _XXX(HI_CHL_MSG_UNKNOWN)
//   pMsgParam [in] : ��Ϣ����, ��С����������ϢID��Ӧ�����ݽṹ, ������Ϊ��
//
// �� �� ֵ:
//    HI_ERR_SUCCESS : ��ʾ�ɹ�
//
// ����Ҫ��: DMS_SendMessage�ӿڽ���MACʹ�� MAC_SendMessage�ӿڽ���DMSʹ��
// ���þ���: TODO: ...
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
/* BEGIN: Modified by liujian 00280874, 2016/4/19   ���ⵥ��:PHY*/
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
    ID_MSG_DFX_CONNECT_PK,              //������ͨ�������ӱ���
    /* END:   Added by likunhe/00194117, 2013/9/8 */
    ID_MSG_RSB_REASSEMBLE_NOTIFY,
    ID_MSG_MAC_END = 0x400,

    /* BEGIN: Added by likunhe/00194117, 2013/9/11   PN:DTS2013092304717*/
    //NDA
    ID_MSG_NFC_BEGIN = 0x500,
    ID_MSG_NFC_REQ_APK,                 // ������ͨ��ת���������������ͨ��-->NDM
    ID_MSG_NFC_RCV_APK,                 // CCO/STA-->NDM
    ID_MSG_NFC_MGT,                     // �ڲ�������Ϣ
    ID_MSG_NFC_END = 0x600,
    /* END:   Added by likunhe/00194117, 2013/9/11 */
};


typedef struct
{
    HI_BOOL bRouterAvail;
    HI_BOOL bIsCco;
    HI_U16  usTEI;  // ����12bit��Ч
    HI_U8   ucMac[HI_PLC_MAC_ADDR_LEN];
    HI_U8   ucCcoMac[HI_PLC_MAC_ADDR_LEN];
} MSG_ROUTER_STATUS_IND_STRU;


typedef struct
{
    HI_U16 usId; // DIAG ��ID
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

    HI_U8 ucLayer;      // ��ǰ�ڵ����ڲ㼶��
    HI_U8 ucChildLayer;  // ��ǰ�ڵ���ӽڵ�Ĳ㼶��
    HI_U16 usChildNum;   // ��ǰ�ڵ���ӽڵ������0��ʾû���ӽڵ�
    HI_U16 usTei;        // ��ǰģ���TEI
} MAC_NET_PARAM_QRY_S;

typedef struct
{
    HI_DMS_PORT_CTRL_OPT_ID_E enCtrlId;
    HI_PVOID pInputParam;
    HI_PVOID pOutputParam;
}MSG_MAC_CTRL_S;

//̨��ʶ��NDM-NDM��Ϣ
typedef struct
{
    //HI_BOOL ndm_identify_mode_switch;                      //̨��ʶ��ndmģʽʹ�ܿ���
    HI_BOOL ndm_identify_rx_mode_switch;                     // ̨��ʶ����ջ�ʹ�ܿ���
    HI_BOOL ndm_identify_tx_mode_switch;                     // ̨��ʶ�����ʹ�ܿ���
    HI_BOOL ndm_identify_strength_switch;                    // ndm̨��ʶ��ǿ��ʹ�ܿ���
    HI_U8 transformer_index;
    HI_U8 phase_index;
    HI_U8 pad[3];

    HI_U32 start_time_stamp;                                       //ndm̨��ʶ��ʼʱ���
    HI_U32 stop_time_stamp;                                       //ndm̨��ʶ��ֹͣʱ���

    HI_U32 rsvd[5];
}tf_identify_ndm_info;


HI_U32 MAC_QueryNetParam(HI_OUT MAC_NET_PARAM_QRY_S* pstNetParam);

/* BEGIN: Added by likunhe/00194117, 2014/1/14   PN:DTS2013120902593*/

/*****************************************************************************
 �� �� ��  : HI_MAC_OpenAfeRxHpf
 ��������  : ��AFE�Ľ��ո�ͨ�˲���
 �������  : HI_VOID
 �������  : ��
 �� �� ֵ  : HI_VOID
 ���������õĺ���  :
 ���ñ������ĺ���  :
 ���ʵ�ȫ�ֱ���  :
 �޸ĵ�ȫ�ֱ���  :
 �޸ĵ�BBP�Ĵ����б�  :

 �޸���ʷ      :
  1.��    ��   : 2014��1��6��
    ��    ��   : likunhe/00194117
    �޸�����   : �����ɺ���

*****************************************************************************/
HI_VOID HI_MAC_OpenAfeRxHpf(HI_VOID);

/*****************************************************************************
 �� �� ��  : HI_MAC_CloseAfeRxHpf
 ��������  : �ر�AFE�Ľ��ո�ͨ�˲���
 �������  : HI_VOID
 �������  : ��
 �� �� ֵ  : HI_VOID
 ���������õĺ���  :
 ���ñ������ĺ���  :
 ���ʵ�ȫ�ֱ���  :
 �޸ĵ�ȫ�ֱ���  :
 �޸ĵ�BBP�Ĵ����б�  :

 �޸���ʷ      :
  1.��    ��   : 2014��1��6��
    ��    ��   : likunhe/00194117
    �޸�����   : �����ɺ���

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

