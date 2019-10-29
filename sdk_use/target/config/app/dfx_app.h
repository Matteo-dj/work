//*****************************************************************************
//
//                  ��Ȩ���� (C), 1998-2011, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : dfx_app.h
//  �� �� ��   : V1.0
//  ��    ��   : shenhankun/00130424
//  ��������   : 2011-11-10
//  ��������   : APP��HSO��ID�Ͷ�Ӧ�����ṹ����
//
//  �����б�   : ��
//  �޸���ʷ   :
//  1.��    �� : 2011-11-10
//    ��    �� : shenhankun/00130424
//    �޸����� : �����ļ�
//
//*****************************************************************************

//*****************************************************************************
// PROJECT   :
// SUBSYSTEM :
// MODULE    :
// OWNER     :
//*****************************************************************************


#ifndef __DFX_APP_H__
#define __DFX_APP_H__

#include <hi_ft_nv.h>
#include <app_nv.h>
#include <hi_mdm.h>
#include <dfx_sys.h>
#include <dfx_sal.h>
#ifndef PRODUCT_CFG_HSO
#include <hi_eqt_pc_ndm.h>
#endif

#include <hi_cmn_nv.h>


//////////////////////////////////////////////////////////////////////////
// ����ID����
//////////////////////////////////////////////////////////////////////////


#define ID_DIAG_CMD_BATTERY_INFO                            (0x3409)//��HI_ND_DEVICE_INFO��ͬ
#define ID_DIAG_CMD_BATTERY_STAT                            (0x3552)//��HI_DIAG_DRV_BQ_STAT_MSG��ͬ

#define ID_DIAG_CMD_NDM_SD_DC_QUERY_STAT                    (0x3553) // HSO�����в�ѯ����
#define ID_DIAG_CMD_NDM_SD_DC_SET_CONFIG                    (0x3554) // HSO->���Կ���̨->���ݲɼ�->�洢�ɼ�: ���� "sd_dc_set"
#define ID_DIAG_CMD_NDM_SD_DC_QUERY_CONFIG                  (0x3555) // HSO->���Կ���̨->���ݲɼ�->�洢�ɼ�: ��ѯ "sd_dc_query"


// ģ�����Stub�İ�ID����
#define ID_DIAG_CMD_CSIM_PACKET                              (0x5700) // ������ģ���
#define ID_DIAG_CMD_MSIM_PACKET                              (0x5701) // ���ģ���
#define ID_DIAG_CMD_SIMU_CTRL                                (0x5710) // ģ�������
#define ID_DIAG_CMD_SIMU_PACKET_ECHO                         (0x5720) // ģ�������


// �����ѯ
#define ID_DIAG_CMD_MRS_BEGIN                                (0x5800)
#define ID_DIAG_CMD_MRS_CHL_STATUS_QRY                       (0x5800)
#define ID_DIAG_CMD_MRS_SET_USER_INFO                        (0x5810)
#define ID_DIAG_CMD_MRS_REFRESH_NV_FILE                      (0x5811)

#define ID_DIAG_CMD_MRS_READ_MAC_CHIP_ID					 (0x5812)//��ȡоƬID
#define ID_DIAG_CMD_MRS_WIRTE_ID							 (0x5813)//д��оƬID

// CCO���״̬��ѯ
#define ID_DIAG_CMD_MRS_CCO_QRY_QRY                          (0x5820)
#define ID_DIAG_CMD_MRS_CCO_PLC_STATUS                       (0x5821)  // ��ȡPLCͨ��״̬
#define ID_DIAG_CMD_MRS_CCO_GET_MACMAP                       (0x5822)  // ��ȡ��ǰMac��ַӳ���
#define ID_DIAG_CMD_MRS_CCO_GET_METER_INF                    (0x5823)  // ��ȡ��ǰ�����Ϣ(���)
#define ID_DIAG_CMD_MRS_CCO_GET_CURR_NODE                    (0x5824)  // ��ȡ��ǰ������Ľڵ㼰�����Ϣ
#define ID_DIAG_CMD_MRS_CCO_INFO_QRY                         (0x5825)  // ��ѯ����Ӧ�������Ϣ(ӳ���״̬����ͣ״̬) DIAG_CMD_MRS_CCO_QRY_STRU
#define ID_DIAG_CMD_MRS_CCO_GET_COLLECTOR_MAP                (0x5826)  // ��ѯ�ɼ���ӳ����Ϣ
#define ID_DIAG_CMD_MRS_GET_SYS_TIME                         (0x5827)  // ��ѯϵͳʱ��
/* BEGIN: PN:  MODIFY\ADD\DEL by cuichunyan/00276962 at 2015/11/11 */
#define ID_DIAG_CMD_MRS_CCO_GET_PARAM                        (0x5828)  //��ѯCCO�����洢��Чʱ��
/* END: PN:  MODIFY\ADD\DEL by cuichunyan/00276962 at 2015/11/11 */
#define ID_DIAG_CMD_MRS_GET_CLT_SEARCH_ABNORMAL              (0x584A)  // ��ѯ�ɼ����ѱ��쳣��Ϣ

/* BEGIN: PN: DTS2015033100426 MODIFY\ADD\DEL by cuiate/00233580 at 2015/3/26 */
#define ID_DIAG_CMD_MRS_CCO_GET_MR_RETRY_CFG                 (0x582A)
#define ID_DIAG_CMD_MRS_CCO_SET_MR_RETRY_CFG                 (0x582B)
#define ID_DIAG_CMD_MRS_CCO_MR_RETRY_CFG_DFX_INF             (0x582C)
/* END:   PN: DTS2015033100426 MODIFY\ADD\DEL by cuiate/00233580 at 2015/3/26 */
#define ID_DIAG_CMD_MRS_HSO_SET_CCO_MODE                     (0x582D)  // ��������ģʽ

#define ID_DIAG_CMD_MRS_GET_CCO_SEARCH_DFX                   (0x584B)  // ��ȡȫ���ѱ�ά����Ϣ
#define ID_DIAG_CMD_MRS_GET_CCO_SLAVENODE_INFO               (0x584C)  // ��ȡȫ���ѱ���(�ӽڵ���Ϣ)
#define ID_DIAG_CMD_MRS_CCO_SEARCH_INFO_EXP                  (0x584D)  // ��ȡȫ���ѱ�ά����Ϣ, ��չ

#define ID_DIAG_CMD_MRS_CCO_GET_POWEROFF_INFO               (0x5831)  // ��ȡͣ����ά����Ϣ
#define ID_DIAG_CMD_MRS_CCO_GET_STA_POWEROFF_INFO           (0x5832)  // ��ȡվ��ͣ����ά����Ϣ
#define ID_DIAG_CMD_MRS_CCO_GET_POWEROFF_HISTORY            (0x5833)  // ��ȡͣ����ά����ʷ��¼
#define ID_DIAG_CMD_MRS_CCO_GET_POWEROFFING_INFO            (0x5834)  // ��ȡ����ͣ�������е�վ����Ϣ
#define ID_DIAG_CMD_MRS_CCO_GET_NDMPLC_INFO                 (0x5836)  // ��ȡ������plc���һ�α�����Ϣ

// STA��ز�ѯ
#if defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG)
#define ID_DIAG_CMD_MRS_DAYFREEZE_REC                        (0x5830)  // ��ѯ����ͳ����
#endif

#define ID_DIAG_CMD_MRS_GET_STA_POWEROFF_INFO               (0x5835)  // ��ȡվ��ͣ����Ϣ

#define ID_DIAG_CMD_MRS_GET_CLT_SEARCH_RESULT                (0x5840)  // ��ѯ�ɼ����ѱ���
#define ID_DIAG_CMD_MRS_GET_CLT_SEARCH_STATS                 (0x5841)  // ��ѯ�ɼ����ѱ�ͳ����Ϣ
#define ID_DIAG_CMD_MRS_GET_CLT_SEARCH_LOCAL                 (0x5842)  // ��ѯ�ɼ��������ѱ���Ϣ
#define ID_DIAG_CMD_MRS_GET_CLT_SEARCH_FRAME                 (0x5843)  // ��ѯ�ɼ����ѱ���ͳ����Ϣ
/* BEGIN: PN: DTS2015051601564 MODIFY\ADD\DEL by cuiate/00233580 at 2015/5/11 */
#define ID_DIAG_CMD_MRS_GET_CLT_SEARCH_LOCAL_EX              (0x5844)  // ��ѯ�ɼ��������ѱ���Ϣ(��չ)
/* END:   PN: DTS2015051601564 MODIFY\ADD\DEL by cuiate/00233580 at 2015/5/11 */

#define ID_DIAG_CMD_MRS_GET_CLTII_EVENT_STATUS               (0x5845)
#define ID_DIAG_CMD_MRS_GET_CLTII_EVENT_INF                  (0x5846)
#define ID_DIAG_CMD_MRS_CCO_GET_CLT_MACMAP                   (0x5849)

#define ID_DIAG_CMD_MRS_GET_CLT_LOCAL_MAP                   (0x5848)  // ��ѯSTA����ӳ���
#define ID_DIAG_CMD_MRS_GET_USER_VER                        (0x5850)
/* BEGIN: Modified by cuichunyan/c00276962 2016/11/10  ���ⵥ��:cltii R485 MultiConnect */
#define ID_DIAG_CMD_MRS_GET_RS485_MULTI_CONNECT        		(0x5851)  //��ѯII��485���
#define ID_DIAG_CMD_MRS_CLR_RS485_MULTI_CONNECT        		(0x5852)  //���II��485���
/* END  : Modified by cuichunyan/c00276962 2016/11/10  ���ⵥ��:cltii R485 MultiConnect */

#define HI_ND_NDM_CMD_PACKET				(0x5853)  // ���ر�-CCO��������
#define ID_DIAG_CMD_MRS_END                                  (0x5900)


#define ID_DIAG_CMD_UPG_DEBUG_QUERY_DETAIL                   (0x5A00)  // DIAG_CMD_UPG_DEBUG_QUERY_DETAIL_STRU
#define ID_DIAG_CMD_MDM_UPG_STATUS                           (0x5A01)
#define ID_DIAG_CMD_MDM_UPG_STATUS_STAT                      (0x5A03)  //ȫ������ͳ��ֵ
#define ID_DIAG_CMD_MDM_UPG_STOP                             (0x5A02)

/* BEGIN:PN:��վ����  MODIFY\ADD  by chenli/00238131 at 2014/1/23 */
#define ID_DIAG_CMD_MDM_UPG_STATUS_BS                       (0x5A04)
#define ID_DIAG_CMD_OPEN_UPG_OPTION_FORCE                   (0x5A05)  //��ǿ����������

//for test
#define ID_DIAG_CMD_MDM_UPG_FILE_CHECK                      (0x5A06)
#define ID_DIAG_CMD_GET_UPG_FORCE_FLAG                      (0x5A07)




typedef struct
{
    HI_U32 ulUpgId;
    HI_U32 ulBlkSize;
    HI_U32 ulFileSize;
    HI_U32 ulNum;
    HI_UPG_STATUS_IND_ENTRY_S entry[8];
}DIAG_CMD_UPG_DFX_STATUS_IND_STRU;

typedef struct
{
    HI_U8  blk_per_pkt;
    HI_U8  pad[2];
    HI_U8 mac[6];
    HI_U8 usUpgStatus;
    HI_U16 bitmap_cnt;
    HI_U8  bitmap[164]; /* HI_UPG_STATUS_BITMAP_LEN */
}DIAG_CMD_UPG_DEBUG_QUERY_DETAIL_STRU;

/* BEGIN:PN:��վ����  MODIFY\ADD  by chenli/00238131 at 2014/1/23 */
//0x5a03
typedef struct
{
    HI_U32 ulUpgId;         // ȫ�������ı�ʶ
    HI_U16 usOkNum;         // ȫ��������ͨ��ģ�������ɹ����ܸ������������ļ���������������ļ���֤ͨ������������֤ͨ��
    HI_U16 usCompleteNum;   // ȫ��������ͨ��ģ��������ɵ��ܸ������������ļ���������������ļ���֤ͨ����
    HI_U16 usFailNum;       // ȫ��������ͨ��ģ������ʧ�ܵ��ܸ���
    HI_U16 usProcessNum;    // ȫ��������ͨ��ģ�������������ܸ������������ļ����ڴ��������
    HI_U8  ucUpgStatus;     // ��ǰȫ��������״̬
    HI_U8  pad[3];
}DIAG_CMD_UPG_DFX_STAT_STATUS_IND_STRU;

//0x5a04
typedef struct
{
    HI_U32 ulUpgId;         // �����ı�ʶ
    HI_U8  ucUpgStatus;     // ��ǰ����״̬
    HI_U8  ucUpgType;       // ��ǰ��������
    HI_U8  aucReserved[2];
}DIAG_CMD_UPG_DFX_BS_STATUS_IND_STRU;
/* END:PN:��վ����  MODIFY\ADD  by chenli/00238131 at 2014/1/23 */

/* BEGIN:PN:DTS2014031206674 ����ǿ���������� MODIFY\ADD  by chenli/00238131 at 2014/3/24 */
typedef struct
{
    HI_BOOL forceFlag;
    HI_U8 pad[3];
}DIAG_CMD_UPG_FORCE_FLAG;
/* END:PN:DTS2014031206674  ����ǿ���������� MODIFY\ADD  by chenli/00238131 at 2014/3/24 */

//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

typedef struct
{
    HI_U32 ulDataSize;
    HI_U8  aucData[0];
} DIAG_CMD_CSIM_REQ_STRU;

typedef struct
{
    HI_U32 ulDataSize;
    HI_U8  aucData[0];
} DIAG_CMD_CSIM_ACK_STRU;


typedef struct
{
    HI_U32 ulDataSize;
    HI_U8  aucData[0];
} DIAG_CMD_MSIM_REQ_STRU;

typedef struct
{
    HI_U32 ulDataSize;
    HI_U8  aucData[0];
} DIAG_CMD_MSIM_ACK_STRU;


///////////////////////////////////////////////////////////////////////////////
// MRS ��Ϣ��ѯ
///////////////////////////////////////////////////////////////////////////////

// MRS ͨ�������Ϣ��ѯ
typedef struct
{
    HI_FTM_CHIP_TYPE_E enChipType;
    HI_FTM_PRODUCT_TYPE_E enProductType;
    HI_FTM_HW_PLATFORM_TYPE_E enHwPlatformType;
    HI_U32 ulPlcChlStatus;
    HI_SAL_DFX_HOST_CTRL_S stHostCtrlInfo;
    HI_NV_FTM_PRODUCT_ID_S stProductId;
    HI_U32 bEnable; // ������Ҫ, ����Ϊ HI_U32, �����Ҫ���ӱ���������Ե���������
} DIAG_CMD_MRS_CHL_QRY_STRU;

typedef enum
{
  EN_MRS_SYS_CALL_ERR_MALLOC_FAIL,
  EN_MRS_SYS_CALL_MAX
}MRS_SYSTEM_CALL_ERROR_ENUM;


typedef struct
{
    HI_U32 Index;           // ӳ����е����
    HI_U8 NodeMac[6];       // ��Ӧ��Mac��ַ
    HI_U8 NodeMeter[6];     // �ڵ�(���)��ַ
    HI_U32 DevType;         // �豸��� (���ģ��; �ɼ���ģ��)
    HI_U32 usSendFrame;     // ��ǰ�ڵ�(���)����645֡����
    HI_U32 usRecvFrame;     // ��ǰ�ڵ�(���)����645֡����
} MRS_DFX_CCO_NODE_MAP_STRU;    // ��ȡ��ǰӳ���ṹ(��һ)

#define MRS_DFX_MAC_MAP_MAX_PACK_NUM  14     // ���δ���Macӳ���������
typedef struct
{
    HI_U16 TotalNum;
    HI_U16 CurrNum;
    MRS_DFX_CCO_NODE_MAP_STRU stNodeMap[MRS_DFX_MAC_MAP_MAX_PACK_NUM];
} MRS_DFX_CCO_NODE_MAP_INF_STRU;    // ��ȡ��ǰӳ���ṹ

typedef struct
{
    HI_U8  NodeMeter[6];    // �ڵ��ַ(���ַ)
    HI_U16 Index;           // ���
    HI_U16 usType;          // ��Լ����
    HI_U16 usFlg;           // ������־(���ֽ���Ч)
} MRS_DFX_CCO_NODE_METER_STRU;

#define MRS_DFX_METER_MAX_PACK_NUM  32 //���δ��ͱ����ṹ
#define MRS_DFX_CLT_METER_MAX       32 //�ɼ����¹ҵ����������
#define MRS_MAX_METERNUM            (MRS_DFX_CLT_METER_MAX)
typedef struct
{
    HI_U16 TotalNum;
    HI_U16 CurrNum;
    MRS_DFX_CCO_NODE_METER_STRU stNodeMap[MRS_DFX_METER_MAX_PACK_NUM];
} MRS_DFX_CCO_METERS_STRU;    // �����ṹ

// MRS �����Ϣ��ѯ
typedef struct
{
    HI_U8 mrs_route_status;             // ����·�ɿ���״̬(0:����; 1:����; 2:��ͣ; 3:�ָ�)
    HI_U8 map_status;                   // ӳ���״̬(0:δ����; 1:�Ѿ���)
    HI_U8 reserved[2];
    HI_U16 meter_num;                   // �����нڵ�����
    HI_U16 map_num;                     // ��ǰӳ�������
} DIAG_CMD_MRS_CCO_QRY_STRU;

// II��ӳ���ϵ��ѯ
typedef struct
{
    HI_U32  sn;
    HI_U8   mac[HI_PLC_MAC_ADDR_LEN];
    HI_U16  reserved;
} DIAG_CMD_MRS_CCO_COLLECTOR_MAP_REQ_STRU;

typedef struct
{
    HI_U8   meter_num;
    HI_U8   reserved;
    HI_U8   mac[HI_PLC_MAC_ADDR_LEN];   // II��������MAC��ַ
    HI_U8   asset[HI_METER_ADDR_LEN];   // II���ʲ����
    HI_U8   id[HI_PLC_MAC_ADDR_LEN];    // II��ID�ţ�Ŀǰʹ��II��Ĭ�ϵ�MAC��ַ
    HI_U8   payload[0];
} MRS_DFX_CCO_COLLECTOR_MAP_ITEM_STRU;

typedef struct
{
    HI_U32  sn;
    HI_U16  num;
    HI_U16  total;
    HI_U8   payload[0];
} DIAG_CMD_MRS_CCO_COLLECTOR_MAP_IND_STRU;


/* BEGIN: PN:  MODIFY\ADD\DEL by cuichunyan/00276962 at 2015/11/11 */
typedef struct
{
    HI_U8 ucFlashSaveDelay;     // �����ӳ�ʱ�䣬��λ: ��
    HI_U8 reserved[15];         // Ĭ��ֵ0
} DIAG_CMD_MRS_CCO_GET_PARAM_INF_STRU;
/* END: PN:  MODIFY\ADD\DEL by cuichunyan/00276962 at 2015/11/11 */

/* BEGIN: Added by xiangchaobin/00378718, 2017/06/26   PN: poweroff/on check report */
// ��ȡȫ��ͣ�����ά����Ϣ
typedef struct
{
    HI_U32  ulRet;
    HI_U32  ulGetPoweroffInfoSeq;       // ��ȡͣ����Ϣ�������

    HI_U16  usMacReportOffSum;          // �յ�MAC��ͣ���ϱ��ܴ���
    HI_U16  usMacReportOnSum;           // �յ�MAC�������ϱ��ܴ���
    HI_U16  usDetectSuccessSum;         // ̽��ɹ��ܴ���
    HI_U16  usInterceptSum;             // ̽��ʧ�ܱ�ͣ���¼�����ܴ���

    HI_U16  us3762ReportOffSum;         // �ϱ����������ߴ���
    HI_U16  us3762ReportOnSum;          // �ϱ����������ߴ���
    HI_U16  usRecheckFailSum;           // ����ʧ���ܴ���
    HI_U16  usMacReportCorrectSum;      // MAC��ȷ�ϱ��ܴ���

    HI_U16  usMacReportWrongSum;        // MAC���ܴ���
    HI_U16  usMacReportMissSum;         // MAC©���ܴ���
    HI_U16  usDetectReportCorrectSum;   // ���������ǰ����ȷ�ϱ��ܴ���
    HI_U16  usDetectReportWrongSum;     // ���������ǰ�����ܴ���

    HI_U16  usDetectReportMissSum;      // ���������ǰ��©���ܴ���
    HI_U16  us3762ReportCorrectSum;     // ������������ȷ�ϱ��ܴ���
    HI_U16  us3762ReportWrongSum;       // �������������ܴ���
    HI_U16  us3762ReportMissSum;        // ����������©���ܴ���

    HI_BOOL bDetectComplete;            // �Ƿ�̽�����
    HI_BOOL bRecheckComplete;           // �Ƿ񸴺����
    HI_U16  usDetectingNum;             // ����̽���վ�����
    HI_U16  usRecheckingNum;            // ���ڸ��˵�վ�����
    HI_U16  usDetectIdx;                // ��ǰ̽�⵽��һ���ڵ�

    HI_U16  usRecheckIdx;               // ��ǰ���˵���һ���ڵ�
    HI_U16  pad[7];
} DIAG_CMD_MRS_CCO_POWEROFF_INFO_IND_STRU;

// ��ȡվ��ͣ��ά����Ϣ�������ṹ
typedef struct
{
    HI_U8  mac[HI_PLC_MAC_ADDR_LEN];
    HI_U16  pad;
} DIAG_CMD_MRS_STA_POWEROFF_INFO_REQ_STRU;

// ��ȡվ��ͣ��ά����Ϣ��Ӧ��ṹ
typedef struct
{
    HI_U32  ulRet;

    HI_U8   ucStatus;                   // ͣ����״̬���ο�MRS_POWEROFF_STATUS_E
    HI_U8   ucReCheckStatus;            // ͣ�縴��״̬���ο�MRS_POWERON_RECHECK_STATUS_E
    HI_U8   ucDetectQueryCnt;           // ̽�Ⳣ�Դ���
    HI_U8   ucRecheckQueryCnt;          // ���˳��Դ���

    HI_U8   ucReportCheckScore;         // �ϱ�������˻���
    HI_U8   ucRecheckFailCnt;           // ����ʧ�ܴ���
    HI_U16  usMacReportOffCnt;          // �յ�MAC���ϱ����ߴ���

    HI_U16  usMacReportOnCnt;           // �յ�MAC���ϱ����ߴ���
    HI_BOOL bPassedDetect;              // �������Ƿ���ͨ����ͣ��̽��
    HI_U8   ucMacReportWrongCnt;        // MAC�ϱ��ģ��󱨴���

    HI_U8   ucMacReportMissCnt;         // MAC�ϱ��ģ�©������
    HI_U8   ucMacReportCorrectCnt;      // MAC�ϱ��ģ���ȷ����
    HI_U8   ucDetectReportWrongCnt;     // ̽����ϱ����󱨴���
    HI_U8   ucDetectReportMissCnt;      // ̽����ϱ���©������

    HI_U8   ucDetectReportCorrectCnt;   // ̽����ϱ�����ȷ����
    HI_U8   uc3762ReportWrongCnt;       // �ϱ���������ģ��󱨴���
    HI_U8   uc3762ReportMissCnt;        // �ϱ���������ģ�©������
    HI_U8   uc3762ReportCorrectCnt;     // �ϱ���������ģ���ȷ����

    HI_U32  ulLastMacReportOffTime;     // ���һ��MAC���ϱ����ߵ�ʱ��
    HI_U32  ulLastMacReportOnTime;      // ���һ��MAC���ϱ����ߵ�ʱ��
    HI_U32  ulLastReportOffTime;        // ���һ���ϱ�ͣ�絽��������ʱ��
    HI_U32  ulLastReportOnTime;         // ���һ���ϱ����絽��������ʱ��
    HI_U32  ulMeterLastPoweroffTime;    // ������һ��ͣ��RTCʱ��
    HI_U32  ulMeterLastPoweronTime;     // ������һ���ϵ�RTCʱ��

    HI_SYS_CALENDAR_TIME_S stLastMacReportOffTime;
    HI_SYS_CALENDAR_TIME_S stLastMacReportOnTime;
    HI_SYS_CALENDAR_TIME_S stLastReportOffTime;
    HI_SYS_CALENDAR_TIME_S stLastReportOnTime;
    HI_SYS_CALENDAR_TIME_S stMeterLastPoweroffTime;
    HI_SYS_CALENDAR_TIME_S stMeterLastPoweronTime;

    HI_U32  ulLastMacReportWrongTime;   // ��һ��MAC���󱨵�ʱ�䣬CCO�ϵ�ʱ��
    HI_U32  pad[12];
} DIAG_CMD_MRS_STA_POWEROFF_INFO_IND_STRU;

#define DIAG_CMD_MRS_GET_POWEROFF_HISTORY_NUM   (10)    // һ�ζ�ȡ��ʷ��¼�ĸ���

// ��ȡͣ���ϱ���ʷ��¼����
typedef struct
{
    HI_U8 begin_index;
    HI_U8 pad[3];
} DIAG_CMD_MRS_POWEROFF_HISTORY_REQ_STRU;

// ��ʷ��¼�ڵ�ṹ 24B
typedef struct
{
    HI_U8  ucStatus; // ͣ����״̬���ο�MRS_POWEROFF_STATUS_E
    HI_U8  pad;
    HI_U8  aucMac[HI_PLC_MAC_ADDR_LEN];

    HI_U32 ulMacReportOffTime;      // MAC�ϱ����ߵ�ʱ��
    HI_U32 ulMacReportOnTime;       // MAC�ϱ����ߵ�ʱ��
    HI_U32 ulMeterLastPoweroffTime; // ������һ��ͣ��ʱ��
    HI_U32 ulMeterLastPoweronTime;  // ������һ���ϵ�ʱ��
} DIAG_CMD_MRS_POWEROFF_HISTORY_NODE_STRU;

// ��ȡͣ���ϱ���ʷ��¼�ظ�
typedef struct
{
    HI_U32 ulRet;

    HI_U8  ucIndex;
    HI_U8  pad[3];

    DIAG_CMD_MRS_POWEROFF_HISTORY_NODE_STRU astRecords[DIAG_CMD_MRS_GET_POWEROFF_HISTORY_NUM];
} DIAG_CMD_MRS_POWEROFF_HISTORY_IND_STRU;

// ��ȡͣ����������Ϣ����
typedef struct
{
    HI_U8  ucDetectStatus;
    HI_U8  ucRecheckStatus;
    HI_U16 usBeginIndex;
} DIAG_CMD_MRS_POWEROFFING_INFO_REQ_STRU;

#define DIAG_POWEROFFING_QUERY_NUM  (20)
#define DOAG_POWEROFF_NODE_MAC_LEN  (8)

typedef struct
{
    HI_U8 aucMac[DOAG_POWEROFF_NODE_MAC_LEN];
} DIAG_POWEROFFING_NODE_STRU;

// ��ȡͣ����������Ϣ�ظ�
typedef struct
{
    HI_U32 ulRet;

    DIAG_POWEROFFING_NODE_STRU astMacList[DIAG_POWEROFFING_QUERY_NUM];
} DIAG_CMD_MRS_POWEROFFING_INFO_IND_STRU;

// STA��ȡͣ����Ϣ�ظ�
typedef struct
{
    HI_U32 ulRet;

    HI_U32 ulLastPoweroffTimeSec;
    HI_U32 ulLastPoweronTimeSec;

    HI_SYS_CALENDAR_TIME_S stLastPoweroffTime;
    HI_SYS_CALENDAR_TIME_S stLastPoweronTime;

    HI_U16 usRcvRecheckCmdCnt;      // �յ����˱��Ĵ���
    HI_U16 usRcvRecheckCmdForMeCnt; // �յ����ҵĸ��˱��Ĵ���
    HI_U16 usRcvDetectCmdCnt;       // �յ�̽�ⱨ�Ĵ���
    HI_U16 usRcvDetectCmdForMeCnt;  // �յ����ҵ�̽�ⱨ�Ĵ���

    HI_U16 ucSendCmdFailCnt;        // ���ķ���ʧ�ܴ���
    HI_U16 pad;
    HI_U32 ulSendCmdErrCode;        // ���ķ��ͷ�����

    HI_U32 ulLastReadPowerInfoTime; // ��һ�δӵ���ȡͣ����Ϣʱ��
    HI_U32 ulStopWhere;

    HI_U32 pad1[9];
} DIAG_CMD_MRS_STA_GET_POWEROFF_INFO_IND_STRU;
/* END:   Added by xiangchaobin/00378718, 2017/06/26 */


#define DIAG_CMD_MRS_MODE_NORMAL				(0x00)
#define DIAG_CMD_MRS_MODE_ONEKEY_DIAGNOSTIC	    (0x01)

typedef struct
{
	HI_U8 ucMode;
	HI_U8 reserved[3];
} DIAG_CMD_MRS_HSO_SET_MODE_REQ_S;

typedef struct
{
    HI_U32 ulRet;
} DIAG_CMD_MRS_HSO_SET_MODE_IND_S;

#if defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG)
#define DAYFREEZE_MAX_PACK_NUM 8

typedef struct
{
    HI_U32 dayIndex;
} DIAG_CMD_MRS_DAYFREEZE_REC_REQ_STRU;

typedef struct
{
    HI_U32 sysTime;
    HI_U8 meter[3];
    HI_U8 causeSet;
} MRS_SRV_MRSLOG_DAYFREEZE_ITEM;

typedef struct
{
    HI_U16 currDayIndex;     // ��ǰ������
    HI_U16 currIndex;        // ���δ��������
    HI_U16 arrayIndex;       // ��������
    HI_U8 meterCount;        // ���յ������
    HI_U8 Count;             // ���ε������
    MRS_SRV_MRSLOG_DAYFREEZE_ITEM item[DAYFREEZE_MAX_PACK_NUM];
} DIAG_CMD_MRS_DAYFREEZE_REC_IND_STRU;// ��ѯSTA���ն���ͳ����

#endif


typedef struct
{
    HI_S32 tmYear;     // year
    HI_S32 tmMon;      // month
    HI_S32 tmMday;     // day of the month
    HI_S32 tmHour;     // hours
    HI_S32 tmMin;      // minutes
    HI_S32 tmSec;      // seconds
    HI_S32 tmWday;     // day of the week
    HI_S32 tmYday;     // day of the year
} MRS_SYS_TIME_S;

typedef struct
{
    HI_U32 second;      // seconds
    HI_U32 timeStamp;   // millisecond
    MRS_SYS_TIME_S tmTime;
} DIAG_CMD_MRS_GET_SYS_TIME_IND_STRU;


// �ɼ�����ά�ɲ�����ѱ���
#define DIAG_CMD_MRS_SEARCH_METER_NUM   (10)

typedef struct
{
    HI_U8 ucAddr[HI_METER_ADDR_LEN];    // ����ַ
    HI_U8 ucProtocol;                   // ��Լ����
    HI_U8 ucBaudrate;                   // ������/100(12:1200 24:2400 48:4800 96:9600)
} MRS_METER_ITEM;

// �����ѱ�ֹͣ��ԭ������������ֹͣ����
#define MRS_SEARCH_END_REASON_DEFAULT   (0)
#define MRS_SEARCH_END_REASON_FINISH    (1)
#define MRS_SEARCH_END_REASON_STOP      (2)

typedef struct
{
    HI_U8 ucTotal;
    HI_U8 ucCurrent;
    HI_U8 ucSearchStatus;
    HI_U8 ucEndReason;
    MRS_METER_ITEM stMeter[DIAG_CMD_MRS_SEARCH_METER_NUM];
} DIAG_CMD_MRS_GET_CLT_SEARCH_RESULT_IND_STRU;


// �ɼ�����ά�ɲ�����ѱ�ͳ����Ϣ
typedef struct
{
    HI_U32 ulSearchSN;
    HI_U8  ucTotalMeter;
    HI_U8  ucSearchStatus;
    HI_U16 usPlcStartRx;
    HI_U32 usPlcStartRxLTS;
    HI_U16 usPlcQueryRx;
    HI_U16 usPlcQueryTx;
    HI_U32 ulPlcQueryRxLTS;
    HI_U32 ulPlcQueryTxLTS;
    HI_U16 usComStartRx;
    HI_U16 usComStartTx;
    HI_U32 ulComStartRxLTS;
    HI_U32 ulComStartTxLTS;
    HI_U16 usComQueryRx;
    HI_U16 usComQueryTx;
    HI_U32 ulComQueryRxLTS;
    HI_U32 ulComQueryTxLTS;
    HI_U16 usBaudrate;
    HI_U8  ucComSetMacAddr[HI_PLC_MAC_ADDR_LEN];
    HI_U32 ulComFirstQueryLTS;
    HI_U32 ulComQueryResultLTS;
    HI_U32 ulComSetMacAddrLTS;
}DIAG_CMD_MRS_GET_CLT_SEARCH_STATS_IND_STRU;


// �ɼ�����ά�ɲ�����ѱ���ͳ��
typedef struct
{
    HI_U16 usSearch;
    HI_U16 usValid;
    HI_U16 usInvalidLen1;
    HI_U16 usInvalidLen2;
    HI_U16 usInvalidLen3;
    HI_U8  ucMaxDepth;
    HI_U8  ucAbnormal;
} MRS_CLT_SEARCH_LOCAL_STATS;

typedef struct
{
    HI_U32 ulStartTime;
    HI_U32 ulEndTime;
    HI_U8  ucSearchStatus;
    HI_U8  ucProtocol;
    HI_U16 usBaudrate;
    HI_U32 ulSearchDI;
    HI_U8  ucSearchAddr[HI_METER_ADDR_LEN];
    HI_U8  ucMeter2Mac[HI_METER_ADDR_LEN];
    HI_U32 ulMeter2MacLTS;
    MRS_CLT_SEARCH_LOCAL_STATS stStartup;
    MRS_CLT_SEARCH_LOCAL_STATS stProtocol07;
    MRS_CLT_SEARCH_LOCAL_STATS stProtocol97a;
    MRS_CLT_SEARCH_LOCAL_STATS stProtocol97b;
} DIAG_CMD_MRS_GET_CLT_SEARCH_LOCAL_IND_STRU;

/* BEGIN: PN: DTS2015051601564 MODIFY\ADD\DEL by cuiate/00233580 at 2015/5/11 */
typedef struct
{
    MRS_CLT_SEARCH_LOCAL_STATS stProtocol97c;
    MRS_CLT_SEARCH_LOCAL_STATS stProtocol698;
    HI_U8 aucReserved[40];
} DIAG_CMD_MRS_GET_CLT_SEARCH_LOCAL_EX_IND_STRU;
/* END:   PN: DTS2015051601564 MODIFY\ADD\DEL by cuiate/00233580 at 2015/5/11 */

typedef struct
{
    HI_U8 ucStatus;             // ���״̬��0�����У�1�����ڼ�أ�2�������ϱ�����������Ч
    HI_U8 ucLastMeterProtocol;  // ���һ�μ�ص��Ĺ�Լ����
    // ���һ�μ�ص��ĵ�ַ
    HI_U8 aucLastMeterAddr[HI_METER_ADDR_LEN];
    HI_U32 ulLastStartTS;       // ���һ�μ�ؿ�ʼʱ��
    HI_U32 ulLastEndTS;         // ���һ�μ�ؽ���ʱ��(������ڼ�أ���ֵΪ��ǰʱ��)
    HI_U32 ulEventTotal;        // �ϵ����������¼����ܴ���
    HI_U8 ucLastEventCount;     // ���һ�μ�أ���ص����¼�����
    HI_U8 ucLastEventReported;  // ���һ�μ�ص����¼������ϱ����¼�����
    HI_U8 ucLastEventCleared;  // ���һ�μ�ص����¼������ϱ����¼�����
    HI_U8 aucReserved[41];
} DIAG_CMD_MRS_GET_CLTII_EVENT_STATUS_STRU;


#define DIAG_CMD_MRS_CLT_EVENT_REPORT_MAX	(10)

typedef struct
{
    HI_U8 aucMeterAddr[HI_METER_ADDR_LEN];  // ����ַ
    HI_U8 ucProtocol;   // ����Լ���ͣ�1-97��Լ��2-07��Լ������-��Ч
    HI_U8 ucEventFlag;  // �¼���ʶ
                        // D0: �¼�1 0-�� 1-��
                        // D1: �¼�2 0-�� 1-��
                        // D2: �¼�3 0-�� 1-��
                        // D3: �¼�4 0-�� 1-��
                        // D4~D7: 0000b
} DIAG_CMD_MRS_CLTII_EVENT_STRU;

typedef struct
{
    HI_U8 ucTotal;      // ���¼��������
    HI_U8 ucCurrent;    // ��֡�����а����ĵ����
    HI_U8 aucPadding[2];
    DIAG_CMD_MRS_CLTII_EVENT_STRU astEvent[DIAG_CMD_MRS_CLT_EVENT_REPORT_MAX];
} DIAG_CMD_MRS_GET_CLTII_EVENT_INF_STRU;


// �ɼ�����ά�ɲ�����ѱ���
#define MRS_DFX_SEARCH_FRAME_MAX_LEN            (56)
#define MRS_DFX_SEARCH_FRAME_CURRENT            (5)

// �ṹ���С20+56=76 B
typedef struct
{
    HI_U32 ulDI;
    HI_U8  ucAddr[HI_METER_ADDR_LEN];
    HI_U8  ucBaudrate;
    HI_U8  ucTimeoutCnt;
    HI_U16 usFrameStats;
    HI_U16 usFrameHeadTS;
    HI_U16 usFrameTailTS;
    HI_U8  ucPad;
    HI_U8  ucLen;
    HI_U8  ucFrame[MRS_DFX_SEARCH_FRAME_MAX_LEN];
} MRS_DFX_SEARCH_FRAME_STRU;

// �ṹ���С4+76*5=384 B
typedef struct
{
    HI_U8 ucReportTotal;         // �����ϱ������������150
    HI_U8 ucCurrent;             // �����ݰ�����������
    HI_U8 ucStartIndex;          // �����ݰ���1��������ucReportTotal������±�
    HI_U8 ucTotal;               // II���ѱ������ĵ����������ܴ���150
    MRS_DFX_SEARCH_FRAME_STRU stFrame[MRS_DFX_SEARCH_FRAME_CURRENT];
} DIAG_CMD_MRS_GET_CLT_SEARCH_FRAME_IND_STRU;


#define DIAG_CMD_MRS_CLT_LOCAL_MAP_NUM   (10)

typedef struct
{
    HI_U8 aucMeterAddr[HI_METER_ADDR_LEN];
    HI_U8 ucProto;
    HI_U8 ucPad;
} MRS_DFX_CLT_LOCAL_MAP_ENTRY;


typedef struct
{
    HI_U8 ucTotal;
    HI_U8 ucCurrent;
    HI_U8 ucStartIndex;
    HI_U8 ucPad;
    MRS_DFX_CLT_LOCAL_MAP_ENTRY astMapEntry[DIAG_CMD_MRS_CLT_LOCAL_MAP_NUM];
} DIAG_CMD_MRS_GET_CLT_LOCAL_MAP_IND_STRU;


// CCO�˿�ά�ɲ�����: II���쳣��Ϣ
#define DIAG_CMD_SEARCH_ABNORMAL_CNT    (10)

typedef struct
{
    HI_U8 ucDevId[HI_PLC_MAC_ADDR_LEN];
    HI_U8 ucAbnormal;
    HI_U8 ucPad;
} MRS_DFX_ABNORMAL_CLTII_STRU;

typedef struct
{
    HI_U16 usTotal;
    HI_U16 usCurrent;
    HI_U16 usStartIndex;
    HI_U16 usPad;
    MRS_DFX_ABNORMAL_CLTII_STRU astAbnormal[DIAG_CMD_SEARCH_ABNORMAL_CNT];
} DIAG_CMD_MRS_GET_CLT_SEARCH_ABNORMAL_IND_STRU;


// �ɼ�����ά�ɲ⣬II���ѱ������Ĵ洢
#define MRS_DFX_SEARCH_FRAME_MAX    (150)
#define MRS_DFX_SEARCH_BUF_SIZE     (40 * MRS_DFX_SEARCH_FRAME_MAX)

typedef struct
{
    HI_U32 ulDI;
    HI_U8  ucAddr[HI_METER_ADDR_LEN];
    HI_U8  ucBaudrate;
    HI_U8  ucFrameLen;
    HI_U16 usFrameOffset;
    HI_U16 usFrameStats;
    HI_U32 ulFrameHeadTS:12;
    HI_U32 ulFrameTailTS:12;
    HI_U32 ulByteIntervalTimeoutCnt:8;
} MRS_DFX_SEARCH_FRAME_HEAD;

typedef struct
{
    HI_U16 usFrameTotal;
    HI_U16 usBufTail;
    MRS_DFX_SEARCH_FRAME_HEAD stHead[MRS_DFX_SEARCH_FRAME_MAX];
    HI_U8 ucBuf[MRS_DFX_SEARCH_BUF_SIZE];
} MRS_DFX_SEARCH_FRAME_STATS_STRU;


// CCO��: ȫ���ѱ��ά�ɲ���Ϣ�ϱ��ṹ
#define MRS_DFX_CCO_SEARCH_INFO_IND_VER0   0

#define MRS_CCO_STATUS_SEARCH_MODE_NORMAL       (0)
#define MRS_CCO_STATUS_SEARCH_MODE_TRANSFORMER  (1)

typedef struct
{
    HI_U8  ucInterfaceVer;                       // �ӿڵİ汾��
    HI_U8  ucStruSize;                           // ���ṹ�Ĵ�С
    HI_U8  ucSearchStatus;                       // �ѱ�״̬
/* BEGIN: PN: DTS2015061008265 MODIFY\ADD\DEL by cuiate/00233580 at 2015/6/23 */
    HI_U8  ucOption;                             // [0:3]:0-��ͨģʽ,1-̨��ʶ��ģʽ,����-����; [4:7]:����
/* END:   PN: DTS2015061008265 MODIFY\ADD\DEL by cuiate/00233580 at 2015/6/23 */
    HI_U32 ulSearchDuration;                     // �ѱ��ʱ����λ: ����
    HI_U16 usUserDefinedDuration;                // �û��趨���ѱ�ʱ������λ: ��
    HI_U16 usTotal;                              // CCO�ѱ����վ��ӽڵ���Ϣ��������
    HI_U16 usMeterEntryNum;                      // CCO�ѱ�����ز����վ��ӽڵ���Ϣ������
    HI_U16 usCltEntryNum;                        // CCO�ѱ���Ĳɼ���վ��ӽڵ���Ϣ������
} DIAG_CMD_MRS_GET_CCO_SEARCH_INFO_IND_STRU;


// CCO��: ���дӽڵ���Ϣ(ȫ���ѱ���)��ѯ���ϱ��ṹ
#define MRS_DFX_CCO_SLAVENODE_INFO_REQ_VER0    0
#define MRS_DFX_CCO_SLAVENODE_INFO_IND_VER0    0

#define DIAG_MRS_REQ_OPTION_ALL                (0)
#define DIAG_MRS_REQ_OPTION_METER              (1)
#define DIAG_MRS_REQ_OPTION_COLLECTOR          (2)

#define MRS_DFX_COUNT_REQ_ALL                  (HI_U16)(-1)

#define DIAG_MRS_DEVICE_TYPE_COLLECTOR         (0)
#define DIAG_MRS_DEVICE_TYPE_METER             (1)

typedef struct
{
    HI_U8  ucInterfaceVer;      // �ӿڵİ汾��
    HI_U8  ucStruSize;          // ���ṹ�Ĵ�С
    HI_U8  ucSearchStatus;      // �ѱ�״̬
    HI_U8  ucOption;            // [0:3]:0-��ͨģʽ,1-̨��ʶ��ģʽ; [4:7]:����

    HI_U16 usMacListNum;        // ��MAC�б��еĽڵ���Ŀ
    HI_U16 usUnlockStaNum;      // �Ѳ鵽�ѱ������������ڵ���Ŀ
    HI_U16 usUnreportStaNum;    // �����������ϱ��ڵ���Ŀ
    HI_U16 usReportedStaNum;    // ���ϱ��ڵ���Ŀ

    HI_U8  bQueryComplete;      // �Ƿ��ѯ���
    HI_U8  bLockComplete;       // �Ƿ��������
    HI_U16 pad;

    HI_U32 pad1[12];
} DIAG_CMD_CCO_SEARCH_INFO_EXP_IND;

typedef struct
{
    HI_U8  ucInterfaceVer;                       // �ӿڵİ汾��
    HI_U8  ucStruSize;                           // ���ṹ�Ĵ�С
    HI_U8  ucSN;                                 // ���β�ѯ�����
    HI_U8  ucOption;                             // ��ѯѡ��: 0�����豸���ͣ�1�ز����2�ɼ���
    HI_U16 usStart;                              // ��ѯ�Ĵӽڵ���Ϣ��ʼ���(�±�),��0��ʼ
    HI_U16 usCount;                              // ��ѯ�Ĵӽڵ���Ϣ����
} DIAG_CMD_MRS_GET_CCO_SLAVENODE_INFO_REQ_STRU;

// �뱣֤DIAG���ݰ���С������384�ֽ�
typedef struct
{
    HI_U8  ucInterfaceVer;      // �ӿڵİ汾��
    HI_U8  ucStruSize;          // �ӿڽṹ�Ĵ�С
    HI_U8  ucSN;                // Ӧ��Ĳ�ѯ���
    HI_U8  ucDevType;           // �ز��ڵ������, 00HΪ�ɼ�����01HΪ���
    HI_U16 usTotal;             // �ϱ����ز��ڵ�����
    HI_U16 usStartIndex;        // ��֡�ϱ��ĵ�1���ӽڵ���Ϣ���ǵڼ���
    HI_U8  ucCurrent;           // ��֡�ϱ����ز��ڵ���
    HI_U8  aucPad[3];           // ����λ
    HI_U8  aucPayload[0];       // �ز����DIAG_MRS_CARRIER_METER_STRU; �ɼ���: DIAG_MRS_CLT_STRU
} DIAG_CMD_MRS_GET_CCO_SLAVENODE_INFO_IND_STRU;

typedef struct
{
    HI_U8  aucMeterAddr[HI_METER_ADDR_LEN];  // 1376.2�����дӽڵ�ͨ�ŵ�ַ
    HI_U8  ucProtoType;                      // 1376.2�����дӽڵ�ͨ��Э������
    HI_U8  ucPad;                            // ����λ
} DIAG_MRS_METER_STRU;

typedef struct
{
    HI_U16 usSeq;                            // �ϱ�1376.2֡�Ĵӽڵ����
    HI_U8  aucPad[2];                        // ����λ
    DIAG_MRS_METER_STRU stMeter;             // �����Ϣ
} DIAG_MRS_CARRIER_METER_STRU;

typedef struct
{
    HI_U16 usSeq;                            // �ϱ�1376.2֡�Ĵӽڵ����
    HI_U8  aucMac[HI_PLC_MAC_ADDR_LEN];      // �ɼ�������MAC��ַ��ʵ�ʣ�
    HI_U8  aucAsset[HI_METER_ADDR_LEN];      // �ɼ����ʲ���
    HI_U8  aucID[HI_PLC_MAC_ADDR_LEN];       // �ɼ�������MAC
    HI_U8  ucTotal;                          // �òɼ����½ӵ�RS485����ܸ���
    HI_U8  ucNum;                            // ��֡�ϱ��ĸòɼ����½�RS485����ܸ���
    HI_U8  aucPad[2];                        // ����λ
    HI_U8  aucPayload[0];                    // ���RS485��������Ϣ��DIAG_MRS_METER_STRU
} DIAG_MRS_CLT_STRU;


/* BEGIN: PN: DTS2015033100426 MODIFY\ADD\DEL by cuiate/00233580 at 2015/3/26 */
typedef enum
{
    DIAG_APP_MRS_TIMEOUT_MODE_DEFAULT = 0,
    DIAG_APP_MRS_TIMEOUT_MODE_FAST,
    DIAG_APP_MRS_TIMEOUT_MODE_USERDEFINE,
    DIAG_APP_MRS_TIMEOUT_MODE_MAX
} DIAG_APP_MRS_TIMEOUT_MODE_E;

// �������нṹ
typedef struct
{
    HI_U32 ulSeq;
    HI_U32 ulReqMode;       // Ҫ��ѯ������ģʽ
} DIAG_CMD_MRS_CCO_GET_MR_TIMEOUT_REQ_STRU;

// �������нṹ
typedef struct
{
    HI_U32 ulSeq;
    HI_U32 ulReqMode;       // Ҫ��ѯ������ģʽ
    HI_U32 ulCurMode;       // ��ǰ����ʱ����ģʽ
    NV_MRS_RETRY_CFG_S astRetryCfg[MRS_RETRY_CFG_MAX_NUM];
} DIAG_CMD_MRS_CCO_GET_MR_TIMEOUT_IND_STRU;

// �������нṹ
typedef struct
{
    HI_U32 ulSeq;
    HI_U32 ulMode;
    NV_MRS_RETRY_CFG_S astRetryCfg[MRS_RETRY_CFG_MAX_NUM];
} DIAG_CMD_MRS_CCO_SET_MR_TIMEOUT_REQ_STRU;

// �������нṹ
typedef struct
{
    HI_U32 ulSeq;
    HI_U32 ulRet;
} DIAG_CMD_MRS_CCO_SET_MR_TIMEOUT_IND_STRU;


#define DIAG_CMD_MRS_CCO_MR_TIMEOUT_DFX_ITEM_MAX    (10)

typedef struct
{
    HI_U32 ulChangeTS;      // ���ʱ���
    HI_U8  ucReqMode;       // �л�����ģʽ
    HI_U8  ucCurMode;       // ��ǰģʽ
    HI_U8  ucRet;           // �л����
    HI_U8  ucSimuStatus;    // ģ��ͨ��״̬
} DIAG_CMD_MRS_CCO_MR_TIMEOUT_DFX_ITEM_STRU;

typedef struct
{
    HI_U32 ulChangeTimes;
    DIAG_CMD_MRS_CCO_MR_TIMEOUT_DFX_ITEM_STRU astItem[DIAG_CMD_MRS_CCO_MR_TIMEOUT_DFX_ITEM_MAX];
} DIAG_CMD_MRS_CCO_MR_TIMEOUT_DFX_IND_STRU;

/* END:   PN: DTS2015033100426 MODIFY\ADD\DEL by cuiate/00233580 at 2015/3/26 */

#define MRS_WHITELIST_CHANGE_ITEM_MAX (12)

typedef struct
{
    HI_U32 ulTimes;
    HI_U8 aucStatus[MRS_WHITELIST_CHANGE_ITEM_MAX]; // 0x55: �رհ�����  0xAA: �����ָ���ʱ��  0xAB: �ָ�������  0xAC: ��ʱ��ˢ��
    HI_U32 aulChangeTS[MRS_WHITELIST_CHANGE_ITEM_MAX];
} DIAG_CMD_MRS_CCO_WHITELIST_CHANGE_IND_STRU;

typedef DIAG_CMD_USER_VER_IND_STRU DIAG_CMD_MRS_GET_USER_VER_STRU;

///////////////////////////////////////////////////////////////////////////////

typedef struct
{
    HI_U32 ulLen;    // ��ת�����ݵĳ���
    HI_U8 data[0];   // ��ת�����ݵ�����
}DIAG_MRS_TRANSMIT_STRU;  // DIAGͨ��������͸���ṹ(����Hso͸�����ݵ�STA����)

enum
{
  EN_ERR_ID_MRS_CHL_CTX_MEM_GET,
  EN_ERR_ID_MRS_SRV_CCO_CTX_MEM_GET,
  EN_ERR_ID_MRS_SRV_CCO_ADDRMAP_MEM_GET,
  EN_ERR_ID_MRS_SRV_CCO_MACADD_ADDLIST_MEM_GET,
  EN_ERR_ID_MRS_SRV_CCO_MACADD_DELLIST_MEM_GET,
};


typedef struct
{
    HI_U8 ucVer;			//version   the default value is R1
    HI_U8 ucStructSize;		//
    HI_U8 ucReserved[2]; 	// reserved
} DIAG_CMD_MRS_GET_RS485_MULTI_CONNECT_REQ_STRU;// the struct of multi-connect   request

//Ӧ��ṹ��
typedef struct
{
    HI_U32 ulOccurTime; //  occur time   unit :s
} MRS_RS485_MULTI_CONNECT_ITEM;// The struct of respond

#define MRS_RS485_MULTI_CONNECT_MAX_TIMES   (1)// the max capacity of record
typedef struct
{
    HI_U8 ucVer;	     	  // version   the default value is R1
    HI_U8 ucStructSize;		  //size of struct
    HI_U8 ucRecordTimes;	  // record time
    HI_U8 ucReserved; 		  // reserved

    HI_U32 ulTotalTimes;	  //totaltimes of occur
    HI_U32 ulUserQryTime;	  //
    MRS_RS485_MULTI_CONNECT_ITEM astRecord[0]; //item of record
} DIAG_CMD_MRS_GET_RS485_MULTI_CONNECT_IND_STRU;//  The struct of respond


//���II��485�������
typedef struct
{
    HI_U8 ucVer;			//version
    HI_U8 ucStructSize;		//
    HI_U8 ucReserved[2]; 	// reserved
} DIAG_CMD_MRS_ClEAR_RS485_MULTI_CONNECT_REQ_STRU;//the struct of request

typedef struct
{
	HI_U8 ucVer;			//
	HI_U8 ucStructSize; 	//
	HI_U8 ucReserved[2]; 	// reserved

	HI_U32 ulErrorCode;     //ERR CODE   0��SUCCESS��1��FAILTER
} DIAG_CMD_MRS_ClEAR_RS485_MULTI_CONNECT_IND_STRU;// The struct of respond


typedef enum
{
    HI_MRS_ERR_CODE_SUCCESS = 0,
    HI_MRS_ERR_CODE_WRONG_VERSION,
    HI_MRS_ERR_CODE_INVALID_TYPE,
    HI_MRS_ERR_CODE_LENGTH,

    HI_MRS_ERR_CODE_OTHER = 0xFF,
} DIAG_MRS_ERR_CODE_E;

typedef struct
{
	HI_U8 ucVer;
    HI_U8 ucStructSize;		// the size of struct
    HI_U16 usLen;    		// the len of transmit data
    HI_U8 ucErrCode; 		//err code  --DIAG_MRS_ERR_CODE_E
	HI_U8 ucReserved[3];	//
    HI_U8 data[0];   		// transmit data
} DIAG_MRS_RS485_TRANSMIT_IND_STRU;

/* END  : Modified by cuichunyan/c00276962 2016/11/10  ���ⵥ��:cltii R485 MultiConnect */


/* BEGIN: Modified by cuichunyan/c00276962 2016/11/10  ���ⵥ��:cltii R485 MultiConnect */

typedef struct
{
    HI_U8 ucVersion;                        //Version
    HI_U8 ucStructSize;                     //the size of struction
    HI_U8 ucReserved[2];                    //reserved
    HI_FTM_DEVICE_UART_CFG_S stComCfg;      //config of com
    DIAG_MRS_TRANSMIT_STRU stTransData;     //data of transmit
} DIAG_MRS_RS485_TRANSMIT_REQ_STRU;  // the struct of transmit data


typedef struct
{
    HI_FTM_CHIP_TYPE_E enChipType;
    HI_FTM_PRODUCT_TYPE_E enProductType;
    HI_U32  ulRet;
    HI_U8   reserve[8];               //����λ
}HI_DFX_HW_PRODUCT_ID_QRY_REQ_S;


/* END  : Modified by cuichunyan/c00276962 2016/11/10  ���ⵥ��:cltii R485 MultiConnect */
// ���ÿͻ���Ϣ(CCO ��װ����)
typedef struct
{
    HI_U16 usStruVer : 6;	/* �ṹ��汾�ţ��̶�Ϊ0 */
    HI_U16 usStruSize: 8;	/* �˽ṹ���С */
    HI_U16 usProtoVer: 2;	/* ��Լ���ͣ�0:376.2Э��; 1:1376.2Э��; */
    HI_U16 usSwVer;			/* ����汾�ţ�BCD��ʽ */
    HI_CHAR szUserVer[32];	/* �û��汾�ţ�ASCII��ʽ */
    HI_CHAR szChipCode[3];	/* оƬ���룬ASCII��ʽ */
    HI_U8 aucDate[3];		/* �汾���ڣ�BIN��ʽ��YYMMDD */
    HI_CHAR aucManuCode[3]; 	/* ���̴��룬ASCII��ʽ */
    HI_U8 reserved[3];
} DIAG_CCO_DUT_SET_USER_INF_REQ_STRU;

typedef struct
{
    HI_U32 ulRet;		/* ���ý����HI_ERR_SUCCESS:�ɹ�; ����:ʧ�� */
} DIAG_CCO_DUT_SET_USER_INF_IND_STRU;

typedef struct
{
   HI_U8 ucVer;
   HI_U8 ucStructSize;
   HI_U8 ucReserved[2];
   HI_U8 ulPad[32];
}HI_DIAG_CMD_MRS_REFRESH_NV_FILE_REQ_S;


typedef struct
{
    HI_U8 ucVer;
    HI_U8 ucStructSize;
    HI_U8 ucReserved[34];
    HI_U32 ulRc;     // �����, HI_ERR_SUCCESS��ʾ�ɹ�, ����Ϊ������
} HI_DIAG_CMD_MRS_REFRESH_NV_FILE_IND_S;

typedef enum
{
	NDM_INTERCONNECT_CMD_ID_CONNECT = 0,//����/����
	NDM_INTERCONNECT_CMD_ID_TOPO,//��ѯ����
	NDM_INTERCONNECT_CMD_ID_METER_ARCHIVE,//��ѯ����
	NDM_INTERCONNECT_CMD_ID_ROUTE_CTRL,//·�ɲ���
	NDM_INTERCONNECT_CMD_ID_CCO_READ_METER,//CCOԶ�̵㳭STA
	NDM_INTERCONNECT_CMD_ID_STA_READ_METER,//���ص㳭STA
	NDM_INTERCONNECT_CMD_ID_MAX,
}NDM_INTERCONNECT_CMD_ID;
typedef struct
{
    HI_U8 des_mac[HI_PLC_MAC_ADDR_LEN];
    HI_U8 success_mac[HI_PLC_MAC_ADDR_LEN];
    NDM_INTERCONNECT_CMD_ID  cmd_id;//����ID
    HI_BOOL isonline;
    HI_U8 pad[3];
}ndm_interconnect_info;
typedef struct
{
    NDM_INTERCONNECT_CMD_ID  cmd_id;//����ID
    HI_U32  ulDataSize; // ���ݳ���
    HI_U8  aucData[0];//��������
} HI_NDM_INTERCONNECT_CMD_STRU;
typedef struct
{
HI_U8  des_mac[6]; //Ŀ��MAC
HI_U8  pad[2];
} NDM_INTERCONNECT_CMD_ID_CONNECT_REQ_STRU;
#define HI_NDM_CONNECT_SUCCESS		(1) // �ɹ�
#define HI_NDM_CONNECT_FAILE   	(0) // ʧ��
typedef struct
{
HI_U8  ret; // ���ӽ��
HI_U8  des_mac[6]; //Ŀ��MAC
HI_U8  pad[1];
} NDM_INTERCONNECT_CMD_ID_CONNECT_IND_STRU;


// ע��:��Ҫ����ö��ֵ˳��HSO������
typedef enum
{
    HI_SD_DC_SET_CONFIG_ERR_CODE_NONE,
    HI_SD_DC_SET_CONFIG_ERR_CODE_SYS_CALL,         //ϵͳ����
    HI_SD_DC_SET_CONFIG_ERR_CODE_INVALID_PARAM,    //������ʽ��ƥ��
    HI_SD_DC_SET_CONFIG_ERR_CODE_TRIGER_TYPE,      //��������
    HI_SD_DC_SET_CONFIG_ERR_CODE_AGGAIN,           //�̶�����
    HI_SD_DC_SET_CONFIG_ERR_CODE_PERIOD_TIME,      //����ʱ��
    HI_SD_DC_SET_CONFIG_ERR_CODE_PER_TIMES,        //ÿ�ֲ�����������
    HI_SD_DC_SET_CONFIG_ERR_CODE_TRI_TIME,         //������ʱ��
    HI_SD_DC_SET_CONFIG_ERR_CODE_FOLDER_NAME,      //�ļ������ִ���
    HI_SD_DC_SET_CONFIG_ERR_CODE_MAC_ADDR,         //MAC��ַ�Ƿ�
    HI_SD_DC_SET_CONFIG_ERR_CODE_MAC_FREQ,         //Ƶ�ηǷ�
    HI_SD_DC_SET_CONFIG_ERR_CODE_MAC_SNID,	       //SNID����
    HI_SD_DC_SET_CONFIG_ERR_CODE_GET_RTC_TIME_FAIL, //��ȡRTCʱ��ʧ��
    HI_SD_DC_SET_CONFIG_ERR_CODE_NOT_SUPPORT_FOLLOW_MODE,//��֧�ָ���ģʽ
}HI_SD_DC_SET_CONFIG_ERR_CODE_E;//ע��:��Ҫ����ö��ֵ˳��HSO������
typedef enum
{
    HI_SD_DC_SET_CONFIG_Q_ATTRIBUTE_BIT_FOLLOW      = 0x0001,  // 1��ʾ����ģʽ
    HI_SD_DC_SET_CONFIG_Q_ATTRIBUTE_BIT_FIX_FREQ    = 0x0002,  // 1��ʾ�̶�Ƶ��
    HI_SD_DC_SET_CONFIG_Q_ATTRIBUTE_BIT_CONTINU_DC  = 0x0004,  // 1��ʾ֧�ֶϵ�����
    HI_SD_DC_SET_CONFIG_Q_ATTRIBUTE_BIT_BECOLLECTING = 0x0008, // �Ƿ����ڲ����������ڲ�ѯ��ǰ״̬
    HI_SD_DC_SET_CONFIG_Q_ATTRIBUTE_BIT_MAX          = 0x10000,
}HI_SD_DC_SET_Q_CONFIG_ATTRIBUTE_BIT_E;//HI_U16

typedef enum
{
    SD_DC_DATA_COLLECT_TYPE_NOISE = 0x0001,
    SD_DC_DATA_COLLECT_TYPE_FC = 0x0002,
    SD_DC_DATA_COLLECT_TYPE_STAT4 = 0x0004,

    /*������DRV�޹� ���ڲ�������*/
    SD_DC_DATA_COLLECT_TYPE_DEFAULT = 0x0008,
}SD_DC_DATA_COLLECT_TYPE_E;

#define SD_DC_DATA_COLLECT_TYPE_ALL         (SD_DC_DATA_COLLECT_TYPE_NOISE|SD_DC_DATA_COLLECT_TYPE_FC|SD_DC_DATA_COLLECT_TYPE_STAT4|SD_DC_DATA_COLLECT_TYPE_DEFAULT)//���в�������
#define SD_DC_DATA_COLLECT_TYPE_UNFOLLOW_ALL         (SD_DC_DATA_COLLECT_TYPE_NOISE|SD_DC_DATA_COLLECT_TYPE_FC|SD_DC_DATA_COLLECT_TYPE_STAT4|SD_DC_DATA_COLLECT_TYPE_DEFAULT)//�Ǹ��� :��������
#define SD_DC_DATA_COLLECT_TYPE_FOLLOW_ALL  (SD_DC_DATA_COLLECT_TYPE_NOISE|SD_DC_DATA_COLLECT_TYPE_DEFAULT)//����:��������
#define SD_DC_DATA_COLLECT_TYPE_NOISE_ALL   (SD_DC_DATA_COLLECT_TYPE_NOISE|SD_DC_DATA_COLLECT_TYPE_DEFAULT)//����:��������
#define SD_DC_DACOLLECT_TYPE_TRI_ALL        (SD_DC_DATA_COLLECT_TYPE_NOISE|SD_DC_DATA_COLLECT_TYPE_FC|SD_DC_DATA_COLLECT_TYPE_STAT4)//����:��������

typedef struct
{
    HI_U8   ccomac[HI_PLC_MAC_ADDR_LEN];    //�����CCO��MAC��ַ
    HI_U8   freq;                           //����������SNID
    HI_U8   snid;                            //���������snid

    HI_U32  ramSize;                        //ä�ɿ�ʼ��ַ 0x200000
    HI_S32  AgcGain; 		                //�����ä����Ч
    HI_U16  usTrigerType;                   //��������SD_DC_DATA_COLLECT_TYPE_E
    HI_U16  periodTime;                     //ä�����ڣ���λ����
    HI_U16  trigerTime;                     //�೤ʱ���ղ���PB����д�����������λ����
    HI_U16  attribute;                      //����HI_SD_DC_SET_Q_CONFIG_ATTRIBUTE_BIT_E

    HI_U8   TimesPerPeriod;                 //ä��ÿ�������������ж��ٴβɼ���
    HI_CHAR scFolderName[NDM_DC_FOLDER_NAME_SIZE];
    HI_U8   pad[2];
}HI_SD_DC_CONFIG_SET_CMD_S; //����REQ

typedef struct
{
    HI_SD_DC_SET_CONFIG_ERR_CODE_E errCode;
    HI_U32 reserver;
}HI_SD_DC_CONFIG_SET_IND_S;//����IND

typedef struct
{
    HI_U32 reserver;
}HI_SD_DC_CONFIG_QUERY_CMD_S;//��ѯREQ

typedef struct
{
    //������
    HI_SD_DC_SET_CONFIG_ERR_CODE_E errCode;
    HI_U32 curTimeSec;

    //MAC
    HI_U8   ccomac[HI_PLC_MAC_ADDR_LEN];    //�����CCO��MAC��ַ
    HI_U8   freq;                           //���������freq
    HI_U8   snid;

    //AgcGain
    HI_U32  ramSize;
    HI_S32  AgcGain; 		                //�����ä����Ч
    HI_U16  usTrigerType;                   //��������
    HI_U16  periodTime;                     //ä�����ڣ���λ����
    HI_U16  trigerTime;                     //�೤ʱ���ղ���PB����д�����������λ����
    HI_U16  attribute;                      //����HI_SD_DC_SET_Q_CONFIG_ATTRIBUTE_BIT_E
    HI_U8   TimesPerPeriod;                 //ä��ÿ�������������ж��ٴβɼ���
    HI_CHAR scFolderName[NDM_DC_FOLDER_NAME_SIZE];
    HI_U8   pad1[2];

}HI_SD_DC_CONFIG_QUERY_IND_S;

typedef struct
{
    HI_U32 ulSdTotalSize;       // SD���ܴ�С
    HI_U32 ulSdFreeSize;        // SD��ʣ��ռ�

    HI_U16 usTotalFileCnt;      // �����ļ�����ͳ����֮�ͣ����������ļ��д���ʱС�ڸ����ļ�ͳ����֮��
    HI_U16 usFileCnt_ZCross;    // ��������ļ�����

    HI_U16 usFileCnt_D;         // ���βɼ�ä���ļ�����
    HI_U16 usFileCnt_N;         // ���βɼ����������ļ�����(�Ǹ���ģʽֱ�Ӳɣ�����ģʽ����ɹ�|ʧ��ʱ��)

    HI_U16 usFileCnt_S;         // ���βɼ�״̬4�ļ�����
    HI_U16 usFileCnt_F;         // ���βɼ�FC�ļ�����

    HI_U16 usFileCnt_A;         // ���βɼ�ȫ��������
    HI_U16 usFileCnt_P;         // ���βɼ�����������

    HI_U32 ulLastRcvPbTimeStamp;// ��һ���յ�PB���ʱ�������λs
    HI_U32 ulCurPeriodStartTime;// �����ڿ�ʼʱ�������λs

    HI_U8  ucCurFolderSequence; // ��ǰ����ļ���� MRS_DC_SD_MAX_SEQUENCE_FOLDER_NUM
    HI_U8  pad[3];
}MRS_DC_SD_STAT_CTX_STRU;

typedef struct
{
    HI_U8   scCcoMac[HI_PLC_MAC_ADDR_LEN];  // �����CCO��MAC��ַ
    HI_U8   ucFreq;                         // ���������freq
    HI_U8   ucIsFixFreq;                    // �Ƿ�̶�Ƶ��

    HI_U8   ucSnid;             // �����snid
    HI_U8   ucIsFollowMode;     // �Ƿ��Ǹ���ģʽ
    HI_U8   ucIsSetRebootStart; // ��������ʼʱ���Ƿ����ڲ���״̬���µ�NV��:����NV����֧�ֶϵ����ɣ�������ԭ��ǹ������ʱ����ΪHI_TRUE
    HI_U8   ucZCrossType;       // �������Ŀ������:������or�½��� MRS_DC_SD_ZCROSS_ISR_TYPE_E

    HI_U32  ulStartAddr;        // �����ڴ��ַ
    HI_U32  ulRamSize;          // ä�ɿ�ʼ��ַ
    HI_S32  slAgcGain;          // �����ä����Ч

    HI_U16  usTrigerType;       // �������� MRS_DC_SD_DATA_COLLECT_TYPE_E
    HI_U16  usPeriodTime;       // ä�����ڣ���λ����
    HI_U16  usTrigerTime;       // �೤ʱ���ղ���PB����д�����������λ����
    HI_U16  usNameId;           // ÿ�ΰ����������� +1

    HI_CHAR scFolderName[NDM_DC_FOLDER_NAME_SIZE];  // "DC_XXX"
    HI_U8   ucTimesPerPeriod;   // ä��ÿ�������������ж��ٴβɼ���
    HI_U8   ucCPowerOnEable;    // �������ɹ���ʹ�ܿ���
    HI_U8   pad;
}MRS_DC_SD_CONFIG_CTX_STRU;

typedef struct
{
    //ͳ����
    HI_U16 usMsgSendFailTimes;          // ��Ϣ����ʧ�ܴ���
    HI_U8  ucSdCardErrTimes;            // �޷������ļ���
    HI_U8  ucSdCardOpenErrTimes;        // д�ļ�����

    HI_U8  ucSdCardWriteErrTimes;       // д�ļ�����
    HI_U8  ucSdCardCloseErrTimes;       // �ر��ļ�����
    HI_U8  ucSdCardCreatFolderErrTimes; // �����ļ��д���
    HI_U8  ucSdCardGetInfoErrTimes;     // ��ȡSD��״̬����

    HI_U8  ucSdCardAccessErrTimes;
    HI_U8  ucStopReason;                // ����ֹͣԭ��MRS_DC_SD_STOP_TYPE_E
    HI_U8  ucNotStartReason;            // ��¼�����޷���ʼԭ�� MRS_DC_SD_NOT_START_REASON_E
    HI_U8  ucSetRtcFailTimes;           // ����RTCʱ��ʧ�ܴ���
} MRS_DC_SD_ERR_CTX_STRU;

typedef struct
{
    HI_U8 ucMainPhase;  // ���������� MRS_DC_SD_MAIN_PHASE_E
    HI_U8 ucStartType;  // ��������ԭ�� MRS_DC_SD_START_TYPE_E
    HI_U8 reserver[2];
} MRS_DC_SD_MAIN_CTRL_CTX_STRU;

typedef struct
{
    MRS_DC_SD_MAIN_CTRL_CTX_STRU stMainCtrlCtx;
    MRS_DC_SD_ERR_CTX_STRU stErrCtx;
    MRS_DC_SD_CONFIG_CTX_STRU stConfigCtx;
    MRS_DC_SD_STAT_CTX_STRU stStatCtx;
} HI_NDM_DATA_COLLECT_STATUS_MSG_S;

typedef HI_NDM_DATA_COLLECT_STATUS_MSG_S MRS_NDM_DC_SD_CTRL_STRU;
#endif //__DFX_APP_H__

