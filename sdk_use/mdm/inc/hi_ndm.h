//****************************************************************************
//
//                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾
//
//****************************************************************************
//  �� �� ��   : hi_ndm.h
//  �� �� ��   : V1.0
//  ��    ��   : ����/00130424
//  ��������   : 2012��06��10��
//  ��������   : �������ӿڶ���, ���� MDM �� HSO ʹ��
//  �����б�   :
//  �޸���ʷ   :
//  1.��    �� : 2012��06��10��
//    ��    �� : ����/00130424
//    �޸����� : �����ļ�
//
//****************************************************************************
#ifndef __HI_NDM_H__
#define __HI_NDM_H__
#ifndef PRODUCT_CFG_HSO
#include <hi_ft_nv.h>
#endif

#if !defined(PRODUCT_CFG_CORE_TYPE_BOOTLOADER) || defined(PRODUCT_CFG_HSO)
#ifndef PRODUCT_CFG_HSO
#include <hi_mdm.h>
#else
#include "hi_types.h"
#endif
#include <hi_cmn_nv.h>
#include <hi_config.h>
//****************************************************************************

//****************************************************************************
//
// �ٿذ�ID ����
// �ٿ�����ȱʡ��Χ [0x3400, 0x3600) �� HI_ND_NDM_START, HI_ND_NDM_END-1
//
#define HI_ND_NDM_START                     HI_ND_NDM_START_DEFAULT
#define HI_ND_NDM_END                       HI_ND_NDM_END_DEFAULT

#define HI_SYS_STATUS                       0x3400  // ��ѯ��ǰ��ϵͳ����״̬����Ϣ , ,  HI_SYS_STATUS_IND_S
#define HI_ND_ERR_IND                       0x3401
#define HI_ND_CONNECTED_INFO                0x3402  // ����: , , HI_ND_CONNECTED_INFO_IND_S
#define HI_BIDB_EXPORT                      ID_DIAG_CMD_BIDB_EXPORT  // ����: HI_BIDB_EXPORT_REQ_S,,HI_BIDB_EXPORT_IND_S
#define HI_BIDB_IMPORT                      ID_DIAG_CMD_BIDB_IMPORT  // ����: HI_BIDB_IMPORT_REQ_S,,HI_BIDB_IMPORT_IND_S
#define HI_MAC_FIND_MODULE                  0x3407  // ����: , , ,HI_MAC_FIND_MODULE_IND_S
/* BEGIN: Added by guopan, 2013/11/12   ���ⵥ��:DTS2013111201546*/
#if defined(PRODUCT_CFG_FEATURE_NDC_CHL)
#define HI_ND_NETWORK_SEARCH_CONN 0x3408   //����HSO�����PLC��������������
#endif
/* END:   Added by guopan, 2013/11/12 */
/* BEGIN: Added by b00208046, 2013/11/18   PN:DTS2013111801139 */
#define HI_ND_DEVICE_INFO                   0x3409
/* END:   Added by b00208046, 2013/11/18 */
#define HSO_QRY_UE_API_VER                  0x340A

#define HI_UPG_STATUS                       0x3410  // ��ѯ����״̬ , ,HI_UPG_STATUS_IND_S
#define HI_UPG_FINISH                       0x3411  // ����: HI_UPG_FINISH_REQ_S, , HI_UPG_FINISH_IND_S
#define HI_UPG_START                        ID_DIAG_CMD_UPG_START  // ����: HI_UPG_START_REQ_S, , HI_UPG_START_IND_S
#define HI_UPG_STOP                         0x3413  // HI_UPG_STOP_REQ_S
#define HI_UPG_VER_BACK                     0x3414  // HI_UPG_VER_BACK_REQ_S, , HI_UPG_VER_BACK_IND_S
#define HI_UPG_TRANSMIT_BLK                 0x3415  // ���������ļ��� HI_UPG_TRANSMIT_BLK_REQ_S, , HI_UPG_TRANSMIT_BLK_IND_S
#define HI_ND_MODULE_DETECT                 0x3416  // , , HI_ND_MODULE_DETECT_IND_S
#define HI_UPG_STAT_INFO_IND                0x3417  // HI_UPG_STATUS, , HI_UPG_STAT_INFO_IND_S
#define HI_UPG_STATUS_REMOTE_QUERY          0x3418  // Զ�� ���Ȳ�ѯ
#define HI_UPG_AFFIRM                       0x3419  //ȫ���������֪ͨ������֪ͨ��ģ������
#define HI_UPG_STA_LIST                     0x341A  // ����STA�б�
#define HI_UPG_EXTEND_INFO                  0x341B  // ������չ��Ϣ
#define HI_UPG_LIST_REPLY                   0x341C  // �б�����Ӧ��HSO��ѯ�б�
#define HI_UPG_SET_PROTOCOL                 0x341D  // �����汾��������Э��
#define HI_DBG_MEM_STAT                     0x3420  // �ڴ�ͳ����Ϣ�ϱ�����, ,
#define HI_DBG_MEM_SUM_STAT_IND             0x3421  // �ڴ�ͳ����Ϣ�ϱ�, HI_DBG_MEM_SUM_STAT_IND_S
#define HI_DBG_MEM_MOD_STAT_IND             0x3422  // �ڴ�ͳ����Ϣ�ϱ�, HI_DBG_MEM_MOD_STAT_IND_S
#define HI_DBG_STAT_Q                       0x3430  // HI_DBG_STAT_Q_S,,HI_DBG_MEM_MOD_STAT_IND_S
#define HI_DBG_DUMP_DIAG_ITEMS              0x3431  // HI_DBG_DUMP_DIAG_ITEMS_REQ_S,,    : dumpmsg dump flash key message
#define HI_DBG_DFX_TASK_TEST                0x3432  // HI_DBG_DFX_TASK_TEST_S, ,

// reserved: 0x3502--0x3509

#define HI_UPG_NV_W                         ID_DIAG_CMD_UPG_NV_W // ����: HI_UPG_NV_W_REQ_S, , 
#define HI_UPG_NV_R                         0x3511       // ����: HI_UPG_NV_R_REQ_S, , HI_UPG_NV_IND_S
#define HI_UPG_NV_BW                        HI_UPG_NV_W  // ����: HI_UPG_NV_W_REQ_S, ,
#define HI_UPG_NV_BR                        HI_UPG_NV_R  // ����: HI_UPG_NV_R_REQ_S, , HI_UPG_NV_IND_S
#define HI_UPG_NV_BE_IND                    0x3512       // ����: , , HI_UPG_NV_BE_IND_S
/* BEGIN: Added by b00208046, 2013/11/30   PN:DTS2013111801139 */
#define HI_NDM_BAT_UPG_STATUS_MSG           0x3550          //��ѯ����״̬
#define HI_DIAG_DRV_NDM_DEBUG              (0x3551)         //�����ã���ʽ����ɾ�� II_NDM_TEST
#define HI_DIAG_DRV_BQ_STAT_MSG             0x3552          //�������� �鿴BQģ��ȫ�ֱ���
#define HI_DIAG_DRV_SD_DC_STAT              0x3553
#define HI_NDM_SD_DC_SET                    0x3554        //"sd_dc_set"
#define HI_NDM_SD_DC_QUERY                  0x3555        //"sd_dc_query"

/* END:   Added by b00208046, 2013/11/30 */
//****************************************************************************


//****************************************************************************

// �����ֶ���(15bit)
//���п�����
#define HI_ND_FRAME_UL_CTRL_STATUS_OK                      (0)      //��������������ҳɹ�
#define HI_ND_FRAME_UL_CTRL_STATUS_TRANS                   (1)      //�����������ڴ����У����Ѿ�͸����ָ���м�
#define HI_ND_FRAME_UL_CTRL_STATUS_FAIL_INVALID_PARAM      (100)    // ��������
#define HI_ND_FRAME_UL_CTRL_DREMOTE_FRM                    (900)    //645֡����

#define HI_ND_FRAME_STATUS_OK                      HI_ND_FRAME_UL_CTRL_STATUS_OK
#define HI_ND_FRAME_STATUS_TRANS                   HI_ND_FRAME_UL_CTRL_STATUS_TRANS
#define HI_ND_FRAME_STATUS_FAIL_INVALID_PARAM      HI_ND_FRAME_UL_CTRL_STATUS_FAIL_INVALID_PARAM

//���п�����
#define HI_ND_FRAME_DL_CTRL_DEFAULT                      0          //MACȷ���Ƿ���Ҫ���������������͸��������ģ�飬���ȴ�����ģ�鴦�������MAC͸����HSO
#define HI_ND_FRAME_DL_CTRL_MAC                          1          //��ҪMAC��������Ҫ͸����APP��Ĭ�϶���MAC����
#define HI_ND_FRAME_DL_CTRL_NEED_ACK                     2
#define HI_ND_FRAME_DL_CTRL_NOT_NEED_ACK                 3
/* END ADD by db 2012-06-30   */
#define HI_ND_FRAME_DL_CTRL_DREMOTE_FRM                   HI_ND_FRAME_UL_CTRL_DREMOTE_FRM

#define HI_ND_DL  0
#define HI_ND_UL  1

#define HI_UPG_STATUS_QRY_SYS_MODE   0 // ��ѯ����ģʽ
#define HI_UPG_STATUS_QRY_UPGRADE    1 // ��ѯ��ǰ�������

#define HI_UPG_STATUS_OK            HI_MDM_UPG_STATUS_OK      // �����ɹ�
#define HI_UPG_STATUS_END           HI_MDM_UPG_STATUS_END    // �������
#define HI_UPG_STATUS_CONTINUE      2                        // ���������жϵ��, ��Ҫ����
#define HI_UPG_STATUS_NORMAL        4                        // ��������״̬
#define HI_UPG_STATUS_UNKNOWN       HI_MDM_UPG_STATUS_UNKNOWN    // δ֪

#define HI_UPG_STATUS_STOPPING      HI_MDM_UPG_STATUS_STOPPING

#define HI_UPG_STATUS_FAIL          HI_MDM_UPG_STATUS_FAIL      // ����ʧ��
#define HI_UPG_STATUS_PROCESS       HI_MDM_UPG_STATUS_PROCESS   // ��������
#define HI_UPG_STATUS_TRANSMIT      HI_MDM_UPG_STATUS_TRANSMIT  // ����ת��

#define HI_UPG_VER_UPDATE_OPTION_FORCE      0x80    // ǿ������
#define HI_UPG_VER_UPDATE_OPTION_CONTINUE   0x40    // ����������
#define HI_UPG_VER_UPDATE_OPTION_MASK       ((HI_UPG_VER_UPDATE_OPTION_START)\
                                           | (HI_UPG_VER_UPDATE_OPTION_CONTINUE)\
                                           | (HI_UPG_VER_UPDATE_OPTION_FORCE))

//#define HI_PLC_RELAY_NUM_MAX             4 // 3, 5, 7, 9, 11, 13
#define HI_PLC_RELAY_NUM_MAX             3 //������ͨ����֧�ֵ�MAC�б���MAC��ַ�������

#define HI_COL_METER_NUM_MAX             33

#define HI_PLC_CRASH_SERIAL_RPT_CODE     "$PlCCrAsH^"

#define HI_DEID_OPTION_REFRESH          0xF0
//****************************************************************************

/* BEGIN: Added by guopan, 2013/11/12   ���ⵥ��:DTS2013111201546*/
#if defined(PRODUCT_CFG_FEATURE_NDC_CHL)
/* BEGIN: Deleted by likunhe/00194117, 2014/1/10   ���ⵥ��:DTS2014010707179*/
//#define MAC_NODE_NAME_LEN  6
/* END: Deleted by likunhe/00194117, 2014/1/10 */

/*����NDM�ϱ���������Ĺ��*/
#define MDL_REPORT_MAX        80        //���ݵ������ĵ��ϱ�����ȷ��
#endif
/* END:   Added by guopan, 2013/11/12 */
//****************************************************************************
#ifndef PRODUCT_CFG_HSO
typedef struct
{
    HI_ND_REMOTE_FRAME_HEAD_S stFrmHrd;
    HI_U8 aucPayload[0];
} HI_MDM_REQ_MSG_S;
#endif
/* BEGIN: Added by guopan, 2013/11/12   ���ⵥ��:DTS2013111201546*/
#if defined(PRODUCT_CFG_FEATURE_NDC_CHL)

#define    HI_NDC_SEARCH_CONN_IND_TYPE_NEW           0  //��ʾNDC֪ͨHSO����/���PLC������Ϣ
#define    HI_NDC_SEARCH_CONN_IND_TYPE_ALL             1  //��ʾNDC�ϱ�����PLC������Ϣ
#define    HI_NDC_SEARCH_CONN_IND_TYPE_STATUS     2  //��ʾNDC�ϱ���ǰ������״̬

#define HI_MDM_PLC_FREQ_AUTO            (0xFF)  //��ʾ��ָ��Ƶ�Σ���NDC�Զ�����
#define HI_MDM_PLC_SNID_AUTO             (0)  //��ʾ��ָ��snid����NDC�Զ�����

/************************************************************
                             ���ݽṹ����
************************************************************/

typedef struct
{
     HI_U32     ulMsgId;        //��ʾ���ִ������ڵ�������ϢID
     HI_U32     ulErrCode;      //ʹ��hi_errno.h�еĴ�����
} HI_ND_ERR_IND_S;
#endif
/* END:   Added by guopan, 2013/11/12 */
/* BEGIN: Added by b00208046, 2014/1/4   ���ⵥ��:DTS2014010400315*/
/***************************************��ص�����ѯ*******************************************************************/
typedef enum
{
    HI_ND_DEVICE_INFO_BAT_STATUS_NORMAL             = 0x00,     //����ģʽ
    HI_ND_DEVICE_INFO_BAT_STATUS_POWER_SOURCE_USE   = 0x01,     //��ӵ�Դ��λ
    HI_ND_DEVICE_INFO_BAT_STATUS_IS_CHARGE          = 0x02,     //�����
    HI_ND_DEVICE_INFO_BAT_STATUS_BAT_NO_EXISTT      = 0x03,     //��ز���λ

    HI_ND_DEVICE_INFO_BAT_STATUS_BAT_INIT_ING       = 0x10,     //��ʼ��δ���
    HI_ND_DEVICE_INFO_BAT_STATUS_BAT_NO_FIRMWARE    = 0x11,     //���оƬû�����
    HI_ND_DEVICE_INFO_BAT_STATUS_BAT_UPGRATE_ING    = 0x12,     //������
}HI_ND_DEVICE_INFO_BAT_STATUS_E;//ö��ֵ ��Χ��С��0x100
typedef struct
{
    HI_U8 ucBatteryPercent;
    HI_U8 ucStatus;                     //HI_ND_DEVICE_INFO_BAT_STATUS_E
    HI_U8 reserved[2];
}HI_ND_DEVICE_INFO_IND_S;
/***************************************HSO��ѯAPI�汾*******************************************************************/
//����ӿڰ汾��  
#define HI_CMD_VER (0)

typedef struct _DIAG_CMD_HSO_QRY_UE_API_VER_IND_S_
{
    HI_U16 usCmdVer;   //����ӿڰ汾��              
    HI_U16 usHAPIVer; //API�߰汾��
    HI_U16 usAPIVer;   //����ĿǰAPI�汾              	
    HI_U16 usCVer;     //C�汾			 	
    HI_U32 ulErrCode;  //������				
    HI_U32 ulPad1;     //����λ			 	
} DIAG_CMD_HSO_QRY_UE_API_VER_IND_S;






/* END:   Modified by b00208046, 2015/4/1 */
typedef struct
{
    HI_U8 ucMainPhase;//�Ƿ����ڲ��� HI_SD_DC_MAIN_PHASE_E
    HI_U8 startType;//SD_DC_START_TYPE_E
    HI_U8   reserver[2];
    /* END:   Modified by b00208046, 2015/1/29 */
}HI_SD_DC_MAIN_CTRL_CTX_S;
typedef struct
{
    HI_U8 isSupport;
    HI_U8 pad[3];
}HI_SD_DC_MODULE_CTRL_CTX_S;





/* add by yuming for ndm led err */
typedef enum
{
    NDM_APP_VER_ERR_ID,			//ID APP Version Err 
    NDM_MAC_TF_ERR_ID,			//ID TF Err 
    LER_ERR_ID_MAX,    			//ID number is limited to 32
}HI_LER_ERR_ID_E;

HI_EXTERN HI_U32 HI_NDM_LED_Handle_Err(HI_LER_ERR_ID_E enErrID, HI_BOOL bErrStat);
/* end */

//****************************************************************************


//****************************************************************************
// ��ѯֱ��ģ��Ļ�����Ϣ: HI_ND_CONNECT_MODULE_INFO
//****************************************************************************/

//****************************************************************************
// REQ: ��
// IND: HI_ND_CONNECTED_INFO_IND_S
typedef struct
{
    HI_U8 ucSysMode :7; // �豸״̬
    HI_U8 bJoin     :1; // �Ƿ��������
    HI_U8 ucDevType; // �豸��Ʒ��̬
    HI_U8 ucMacAddr[HI_PLC_MAC_ADDR_LEN]; // ͨ��ģ��MAC
    HI_U8 ucIp[HI_IPV4_ADDR_LEN]; // ��̫��IP
    HI_U32 ulSnid; // ͨ������SNID
    HI_U8 ucCcoAddr[HI_PLC_MAC_ADDR_LEN]; // CCO MAC
    HI_U8 pad[2];
    HI_CHAR szPositoin[HI_MODULE_POSITION_LEN_MAX]; // λ����Ϣ

    HI_U32 usStartTime; // �豸����ʱ��
} HI_ND_CONNECTED_INFO_IND_S;
#define HI_ND_CONNECTED_INFO_ACK_S  HI_ND_CONNECTED_INFO_IND_S

//*****************************************************************************
typedef enum
{
    HI_SYS_KDBG_RC_ID_MAC = 1,

    HI_SYS_KDBG_RC_ID_MAX
}HI_SYS_KDBG_RC_ID_E;

typedef struct
{
    HI_SYS_KDBG_RC_ID_E id;
    HI_U32 data[4];
}HI_DMID_KDBG_RC_S;

typedef enum
{
    HI_SYS_KDBG_RPT_ID_MAC = 1,

    HI_SYS_KDBG_RPT_ID_MAX
}HI_SYS_KDBG_RPT_ID_E;

typedef struct
{
    HI_SYS_KDBG_RPT_ID_E id;
    HI_U32 data[4];
}HI_DMID_KDBG_RPT_S;
//*****************************************************************************



//****************************************************************************
// HI_ND_MAC_FIND_MODULE
//****************************************************************************

//****************************************************************************
// �����豸
// REQ: ��
// ACK:
typedef struct
{
    HI_U8   type   :4;   // HI_ND_TYPE_DCM:������; HI_ND_TYPE_CCO: ͷ��; HI_ND_TYPE_STA:�ն�; HI_ND_TYPE_RELAY:�м�
    HI_U8   online :1;   // ���ߡ�����
    HI_U8   status :1;   // �������쳣
    HI_U8   phase_mode;  // ��λģʽ, ����λ���ǵ���λ
    HI_U8   phase;       // ��λ����
    HI_U8   band;        // Ƶ��
    HI_U16  tei;         // վ��tei(����ʱ)
    HI_U32  join_time;   // ��������ʱ��(����ʱ)
    HI_U8   mac[HI_PLC_MAC_ADDR_LEN]; // MAC��ַ
} HI_MODULE_ITEM_S;

typedef struct
{
    HI_U16 usItemCnt; // �ܱ�ģ�����
    HI_MODULE_ITEM_S astItem[0];
} HI_MAC_FIND_MODULE_IND_S;


// �����ڵ�״̬���������������쳣�����ڵ����ͣ�����ɼ��������ڵ��ַ������ַ�������ϱ���HSO
// REQ: ��
// ACK:
typedef struct
{
    HI_U8   type;     // HI_ND_CONNECTED_DEV_TYPE_METER: ���; HI_ND_CONNECTED_DEV_TYPE_COLLECTOR: �ɼ���
    HI_U8   status;   // 0Ϊ����������ֵΪ�����룬��ʾ�쳣
    HI_U8   addr[HI_PLC_MAC_ADDR_LEN];  // ģ�����ӵ��豸�������ĵ�ַ
    HI_U8   mac[HI_PLC_MAC_ADDR_LEN]; // MAC��ַ
} HI_APP_MODULE_ITEM_S;

typedef struct
{
    HI_U16 usItemCnt; // �ܱ�ģ�����
    HI_APP_MODULE_ITEM_S astItem[0];
} HI_APP_FIND_MODULE_IND_S;
//****************************************************************************


//****************************************************************************

#if defined(PRODUCT_CFG_SUPPORT_UPG)
typedef struct
{
    HI_U32 ulUpgId;
} HI_UPG_FINISH_REQ_S;

typedef struct
{
    HI_U32 ulRc;     // �����, HI_ERR_SUCCESS��ʾ�ɹ�, ����Ϊ������
    HI_U32 ulUpgId;  // ����ID
} HI_UPG_FINISH_IND_S;

typedef struct
{
    HI_U32 ulUpgId;  // ����ID, ID�������������б���Ψһ.
    HI_U16 usOption;
    HI_U16 reserved;
} HI_UPG_VER_BACK_REQ_S;

typedef struct
{
    HI_U32 ulRc;     // �����, HI_ERR_SUCCESS��ʾ�ɹ�, ����Ϊ������
    HI_U32 ulUpgId;  // ����ID, ID�������������б���Ψһ.
} HI_UPG_VER_BACK_IND_S;

typedef struct
{
    HI_U32 ulUpgId;  // ����ID, ID�������������б���Ψһ.
    HI_U16 usResetInterval; //�����ȴ�ʱ�䣬��λsec
    HI_U16 usOption;
} HI_UPG_AFFIRMATIVE_REQ_S;

typedef struct
{
    HI_U32 ulRc;     // �����, HI_ERR_SUCCESS��ʾ�ɹ�, ����Ϊ������
    HI_U32 ulUpgId;  // ����ID, ID�������������б���Ψһ.
} HI_UPG_AFFIRMATIVE_IND_S;

#define HI_UPG_LIST_STA_NUM  (30)
#define HI_UPG_LIST_NUM_MAX  (2047)

typedef enum 
{
    HI_UPG_LIST_VER0 = 0,
} HI_UPG_LIST_VER_E;

#define HI_UPG_LIST_VER     (HI_UPG_LIST_VER0)

typedef struct
{
    HI_U8 ucStruVer;      //version: this version is zero
    HI_U8 ucStruSize;     //structure size
    HI_U8 ucPacketTotal;  //total frames
    HI_U8 ucPacketSeq;    //current frame number,begin with one
    HI_U32 ulUpgId;       //upg ID
    HI_U16 usStaTotal;    //total number of sta
    HI_U16 usStaCurrent;  //current frame contains the number of sta
    HI_U8 aucStaList[HI_UPG_LIST_STA_NUM][HI_PLC_MAC_ADDR_LEN]; // MAC address list
}HI_UPG_STA_LIST_BLK_REQ_S;

typedef struct
{
    HI_U8 ucStruVer;  //version: this version is zero
    HI_U8 ucStruSize; //structure size
    HI_U8 ucPacketSeq;//current frame number
    HI_U8 pad;
    HI_U32 ulUpgId;   //upg ID
    HI_U32 ulRet;     //return code
}HI_UPG_STA_LIST_BLK_IND_S;

typedef enum 
{
    HI_UPG_EXTEND_INF_VER0 = 0,
} HI_UPG_EXTEND_INF_VER_E;

#define HI_UPG_EXTEND_INF_VER                (HI_UPG_EXTEND_INF_VER0)

#define HI_UPG_EXTEND_SUPPORT_LIST_UPGRADE   (0x01) //support list upg
#define HI_UPG_EXTEND_SUPPORT_SET_PROTO      (0x02) 
#define HI_UPG_EXTEND_SUPPORT_FUN            (HI_UPG_EXTEND_SUPPORT_LIST_UPGRADE)
#define HI_UPG_EXTEND_SUPPORT_FUN_CSG        (HI_UPG_EXTEND_SUPPORT_LIST_UPGRADE | HI_UPG_EXTEND_SUPPORT_SET_PROTO)

typedef struct
{
    HI_U8 ucStruVer;  // �汾�ţ��˰汾Ϊ0
    HI_U8 ucStruSize; // �ṹ���С
    HI_U16 pad;
}HI_UPG_EXTEND_INFO_REQ_S;

typedef struct
{
    HI_U8 ucStruVer;  // �汾�ţ��˰汾Ϊ0
    HI_U8 ucStruSize; // �ṹ���С
    HI_U8 ucUpgOption; //CCO����STA��ʽ:0:���漰��1:ȫ��;2:�б�
    HI_U8 ucFunBitmap; //֧�ֵĹ�������
    HI_U32 ulUpgId;   //����ID
    HI_U32 pad2;
}HI_UPG_EXTEND_INFO_IND_S;

typedef struct
{
    HI_U8 stru_ver;  // �汾�ţ��˰汾Ϊ0
    HI_U8 stru_size; // �ṹ���С
    HI_U8 upg_mode;
    HI_U8 pad;
}HI_UPG_SET_PROTOCOL_REQ_S;

typedef struct
{
    HI_U8 ucStruVer;  // �汾�ţ��˰汾Ϊ0
    HI_U8 ucStruSize; // �ṹ���С
    HI_U16 pad;
    HI_U32 ret;
}HI_UPG_SET_PROTOCOL_IND_S;

typedef HI_UPG_STA_LIST_BLK_REQ_S HI_UPG_LIST_REPLY_IND_S;
typedef struct
{
    HI_U16 usReqSn;
    HI_U16 usIdx;
    HI_U32 ulRc;
} HI_UPG_BLK_ID_S;


typedef struct
{
    HI_U16 usChecksum;
    HI_U8  ucMac[HI_PLC_MAC_ADDR_LEN];
    HI_U32 ulUpgId;
    HI_U16 usRc;
    HI_U16 usIdNum;
    HI_UPG_BLK_ID_S aulId[0];
} HI_UPG_TRANSMIT_BLK_IND_S;

// ����CCO��ѯվ���������

typedef enum
{
    HI_UPG_STATUS_IND_ENTIRE_VER0,
    HI_UPG_STATUS_IND_ENTIRE_VER1,
} HI_UPG_STATUS_IND_ENTIRE_VER_E;

#define HI_UPG_STATUS_IND_ENTIRE_VER    (HI_UPG_STATUS_IND_ENTIRE_VER1)

#define HI_UPG_STATUS_BITMAP_LEN (PRODUCT_CFG_UPG_BITMAP_LEN)

typedef struct
{
    HI_U32 ulUpgId;
    HI_U16 usLastBlkIdx;
    HI_U16 ulErrorCode;
    HI_U32 ulRcvFileSize;
    HI_U32 ulBlkSize;
    HI_U32 ulFileSize;
    HI_U8  usUpgStatus;
    HI_U8  aucVer[3];
    HI_U16 ausVer[2];
    HI_U8  aucBootVer1;
    HI_U8  aucBootVer2;
    HI_U8  aucBootVer3;
    HI_U8  ucNvCVer; // NV.hupg: C Ver
    HI_U16 ausNvVer[2];
    HI_U32 ulChipVer;
    HI_U16 usLeftTimeWindow;
    HI_U8  aucBuildDataTime[6];
    HI_U16 usUpgReqSn;
    HI_U8  ucMacAddr[HI_PLC_MAC_ADDR_LEN];
    HI_U32 ulUpgEndTime;//modified by ds, duration(Sec.) of indicate sta from rcv start frame to rcv all upg data
} HI_UPG_STATUS_IND_ENTIRE_S_VER0;

typedef struct
{
    HI_U32 ulUpgId;
    HI_U16 usLastBlkIdx;
    HI_U16 ulErrorCode;
    HI_U32 ulRcvFileSize;
    HI_U32 ulBlkSize;
    HI_U32 ulFileSize;
    HI_U8  usUpgStatus;
    HI_U8  aucVer[3];
    HI_U16 ausVer[2];
    HI_U16 usSwVer;
    HI_U8  ucBootVer3;
    HI_U8 reserved;
    HI_CHAR szManuCode[2];
    HI_CHAR szChipCode[2];
    HI_U32 ulChipVer;
    HI_U16 usLeftTimeWindow;
    HI_U8  aucBuildDataTime[6];
    HI_U16 usYear  : 7;
    HI_U16 usMonth : 4;
    HI_U16 usDay   : 5;
    HI_U8  ucMacAddr[HI_PLC_MAC_ADDR_LEN];
    HI_U32 ulUpgEndTime;//modified by ds, duration(Sec.) of indicate sta from rcv start frame to rcv all upg data
} HI_UPG_STATUS_IND_ENTIRE_S;

typedef struct
{
   HI_UPG_STATUS_IND_ENTIRE_S status;

    HI_U8  blk_per_pkt;// ����һ���������а����Ŀ����
    /* BEGIN:PN:DTS2014092002573 ȫ������״̬��ѯͳ�����Ż� MODIFY\ADD  by chenli/00238131 at 2014/9/20*/
    HI_U8  from;//״̬��ѯ��Դ:վ�������ϱ�(0)/Ӧ��CCO���͵Ĳ�ѯ(2)
    /* END:PN:DTS2014092002573 ȫ������״̬��ѯͳ�����Ż�  MODIFY\ADD  by chenli/00238131 at 2014/9/20*/
    HI_U16 bitmap_cnt;
    HI_U8  bitmap[0];
} HI_UPG_STATUS_DETAIL_IND_S;

typedef enum
{
    UPG_QRY_PRODUCT_TYPE_METER = 0, //���ܱ�
    UPG_QRY_PRODUCT_TYPE_METERI,    //I�Ͳɼ���
    UPG_QRY_PRODUCT_TYPE_METERII,   //II�Ͳɼ���
}HI_UPG_QUERY_PRODUCT_TYPE_E;

typedef enum
{
    UPG_INFO_ID_CODE = 0,   // ���̱��
    UPG_INFO_ID_SW_VER,     // �汾��Ϣ
    UPG_INFO_ID_BOOT_VER,   // Bootloader�汾��
    UPG_INFO_ID_FILE_CRC,   // �����ļ�CRC32
    UPG_INFO_ID_FILE_LEN,   // �����ļ�����
    UPG_INFO_ID_CHIP, // ��Ʒ����
    UPG_INFO_ID_DATE,       // �汾����
}HI_UPG_QUERY_INFO_ID_E;

//����Э��
#define UPG_INFO_ID_PRODUCT_TYPE UPG_INFO_ID_CHIP

typedef struct
{
    HI_U16 usStruVer  : 6;  //Э��汾��
    HI_U16 usStruSize : 6;  //����ͷ����
    HI_U16 reserved   : 4;
    HI_U8 reserved2;
    HI_U8 ucInfoItemCnt;  //��Ϣ�б�Ԫ�ظ���
    HI_U8 aucInfoItem[0]; //��ϢID�б�
} HI_UPG_STA_INFO_REQ_S; //˽��Э���ѯվ����ϢREQ

typedef struct
{
    HI_U16 usStruVer  : 6;  //Э��汾��
    HI_U16 usStruSize : 6;  //����ͷ����
    HI_U16 reserved1  : 4;
    HI_U8 reserved2;
    HI_U8 ucInfoItemCnt;   //��Ϣ�б�Ԫ�ظ���
    HI_U8 aucInfoItem[0];  //��Ϣ�б������ֶ��б�˳�����1��������Ϣ�ֶ�(Ԫ��ID(1�ֽ�)+Ԫ�����ݳ���(1�ֽ�)+Ԫ������(L))
} HI_UPG_STA_INFO_IND_S; //˽��Э���ѯվ����ϢIND


//����������ͨPLC���ĸ�ʽ
typedef struct
{
    HI_U16 usStruVer  : 6;  //Э��汾��
    HI_U16 usStruSize : 6;  //����ͷ����
    HI_U16 reserved1  : 4;
    HI_U16 reserved2;
    HI_U32 ulUpgId;         //����ID
    HI_U16 usTimeWindow;    //����ʱ�䴰
    HI_U16 usPacketSize;    //�������С
    HI_U32 ulFileSize;      //�����ļ���С
    HI_U32 ulCrc;           //�ļ�CRCУ��
} HI_UPG_STD_START_REQ_S;

typedef struct
{
    HI_U16 usStruVer  : 6;  //Э��汾��
    HI_U16 usStruSize : 6;  //����ͷ����
    HI_U16 reserved1  : 4;
    HI_U8  reserved2;
    HI_U8  ucRc;            //��ʼ���������
    HI_U32 ulUpgId;         //����ID
} HI_UPG_STD_START_IND_S;

typedef struct
{
    HI_U16 usStruVer  : 6;  //Э��汾��
    HI_U16 usStruSize : 6;  //����ͷ����
    HI_U16 reserved1  : 4;
    HI_U16 reserved2;
    HI_U32 ulUpgId;         //����ID
} HI_UPG_STD_STOP_REQ_S;

typedef struct
{
    HI_U16 usStruVer  : 6;  //Э��汾��
    HI_U16 usStruSize : 6;  //����ͷ����
    HI_U16 reserved   : 4;
    HI_U16 usPacketSize;    //���ݿ��С
    HI_U32 ulUpgId;         //����ID
    HI_U32 ulPacketNum;     //���ݿ���,��0��ʼ
    HI_U8 aucPacket[0];     //���ݿ�
} HI_UPG_STD_TRANSMIT_REQ_S;

typedef struct
{
    HI_U16 usStruVer   : 6;  //Э��汾��
    HI_U16 usStruSize  : 6;  //����ͷ����
    HI_U16 reserved    : 4;
    HI_U16 usPacketNum;      //������ѯ�Ŀ���
    HI_U32 ulPacketStart;    //��ʼ���,��0��ʼ
    HI_U32 ulUpgId;          //����ID
} HI_UPG_STD_STATUS_REQ_S;

typedef struct
{
    HI_U16 usStruVer   : 6;  //Э��汾��
    HI_U16 usStruSize  : 6;  //����ͷ����
    HI_U16 usUpgStatus : 4;  //����״̬
    HI_U16 usPacketNum;      //��Ч����
    HI_U32 ulPacketStart;    //��ʼ���
    HI_U32 ulUpgId;          //����ID
    HI_U8 aucBitmap[0];
} HI_UPG_STD_STATUS_IND_S;

typedef struct
{
    HI_U16 usStruVer   : 6;  //Э��汾��
    HI_U16 usStruSize  : 6;  //����ͷ����
    HI_U16 reserved    : 4;
    HI_U16 usResetTime;      //�ȴ���λʱ��
    HI_U32 ulUpgId;          //����ID
    HI_U32 ulTryRunningTime; //������ʱ��
} HI_UPG_STD_AFFIRM_REQ_S;

typedef HI_UPG_STA_INFO_REQ_S HI_UPG_STD_STA_INFO_REQ_S;

typedef struct
{
    HI_U16 usStruVer  : 6;  //Э��汾��
    HI_U16 usStruSize : 6;  //����ͷ����
    HI_U16 reserved1  : 4;
    HI_U8 reserved2;
    HI_U8 ucInfoItemCnt;   //��Ϣ�б�Ԫ�ظ���
    HI_U32 ulUpgId;        //����ID
    HI_U8 aucInfoItem[0];  //��Ϣ�б������ֶ��б�˳�����1��������Ϣ�ֶ�(Ԫ��ID(1�ֽ�)+Ԫ�����ݳ���(1�ֽ�)+Ԫ������(L))
} HI_UPG_STD_STA_INFO_IND_S;
#else
typedef struct
{
    HI_U32 ulUpgId;
    HI_U16 usLastBlkIdx;
    HI_U16 ulErrorCode;
    HI_U32 ulRcvFileSize;
    HI_U32 ulBlkSize;
    HI_U32 ulFileSize;
    HI_U8  usUpgStatus;
    HI_U8  aucVer[3];
    HI_U16 ausVer[2];
    HI_U32 ulChipVer;
    HI_U16 usLeftTimeWindow;
    HI_U8  aucBuildDataTime[6];
    HI_U16 usUpgReqSn;
    HI_U8  ucMacAddr[HI_PLC_MAC_ADDR_LEN];
} HI_UPG_STATUS_IND_S;
#endif


typedef struct
{
    HI_U8 ucDevType;
    HI_U8 usDevNum;
    HI_U8 ucDevAddr[HI_COL_METER_NUM_MAX][HI_PLC_MAC_ADDR_LEN];
} HI_ND_MODULE_DETECT_IND_S;
//****************************************************************************
#define HI_LOGIN_AUTH_RESULT_ID_SUCCESS 0
#define HI_LOGIN_AUTH_RESULT_ID_ACCOUNT_LOCKED 1
#define HI_LOGIN_AUTH_RESULT_ID_BUSY 20
#define HI_LOGIN_AUTH_RESULT_ID_PASS_WORD_WRONG 47
#define HI_LOGIN_AUTH_RESULT_ID_TIMEOUT 51
#define HI_LOGIN_AUTH_RESULT_ID_SYSTEM_CALL_ERROR 2999
typedef struct
{
    HI_U32 errCode;//HI_LOGIN_AUTH_RESULT_ID_XXX
    HI_U16 mainVer;//�����������汾
    HI_U16 subVer;//���������Ӱ汾
    HI_U32 magicNum;//ħ����
    HI_U32 id;//����ID
    HI_U32 reserve;
    HI_U32 dataSize;//���ݴ�С
    HI_U8 data[0];//��������
} HI_LOGIN_AUTH_RESULT_S;//������֤�ṹ��

#endif // defined(PRODUCT_CFG_CORE_TYPE_BOOTLOADER) || defined(PRODUCT_CFG_HSO)
//*****************************************************************************



//*****************************************************************************
#endif // __HI_NDM_H__

