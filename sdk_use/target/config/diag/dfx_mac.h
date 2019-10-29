/*************************************************
  Copyright:   Huawei Technologies Co., Ltd. 1998-2011
  File name: dfx_mac.h
  Author:  dkf35509
  Description:  ��ά�ɲ�ӿ�
  Others:
  History:
*************************************************/

#ifndef _DFX_MAC_H
#define _DFX_MAC_H

#include "dfx_sys.h"
#include "dfx_sys_sdm.h"
#ifdef PRODUCT_CFG_ONLINE_TEST_MODE
#include "hi_mac_nv.h"
#endif
#include "hi_mdm_sys.h"
#include "hi_mdm_mac.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 *
 *  ���������Ŀ�������ֵ
 */
//#define  DFX_NETWORK_TOPO_ENTRY_MAX       10
#define  DFX_NETWORK_TOPO_ENTRY_MAX       8

#define  DFX_RX_STATICS_ENTRY_MAX         16
#define  DFX_TDMA_SCHEDULE_ENTRY_MAX      10
#define  DFX_OFFLINE_STA_ENTRY_MAX        10

#define  DFX_ROUTER_TABLE_ENTRY_MAX       31 //
#define  DFX_DISCOVER_LIST_ENTRY_MAX      5  //
#define  DFX_DISCOVER_LIST_ENTRY_SM_MAX   19
#define  DFX_ASSOCIATED_STAS_ENTRY_MAX    48


#define PLC_NDM_IP_MAC_MAP_NUM_MAX          (4)     //��IP��ʽ�볭��������ʱ��վ��˵�IP��MAC��ӳ��������Ա��

#define MAC_DIAG_CMD_QUERY_NETWORK_TOPO_ENTRY_NUM           (8)     //NDAͨ���������ϱ��ṹ�а����Ľڵ���
#define DFX_NTB_SYNC_STA_CNT_MAX         (8)
#define DFX_NTB_EXCEPTION_CNT_MAX        (8)
#define DFX_NTB_SINGU_FREQERR_CNT_MAX    (8)
#define DFX_PATH_TABLE_BITMAP_LEN        (128)                     //·����Ϣ��bitmap��С
#define DFX_PATH_TABLE_BIT_SIZE          (8)                     //·����Ϣ�����λ��С
#define PHS_PER_TO_NUM                  (100)

//����������оƬ�ı��
typedef enum
{
    CHIP_VER_IDX_HI3911 = 0x01,
    CHIP_VER_IDX_MAX = 0x03,
    CHIP_VER_IDX_INVALID = 0xff
}CHIP_VER_IDX_ENUM;

#define MAX_NEIGHBOR_SUM            (15)          // �ھ�����������,��������������
#define QUERY_MAX_NEIGHBOR_SUM      (30)         // ��ѯ�ӿ����漰�ھ�����������,��������������


/**
 * �ϱ���HSO�����
 **/
#define TYPE_VERSION unsigned char
#define TYPE_DEBUG_RELEASE      (0x02)     // ��ʾHSO���з�����汾���ֳ�ά���湲��
#define TYPE_DEBUG              (0x00)     // ��ʾHSO���з�����汾
#define TYPE_RELEASE            (0x01)     // ��ʾ HSO���ֳ�ά���汾

/************************************************************************/
/*      �����ϢID    ��0x1000~0x5000��                                 */
/************************************************************************/

/********************************************************************/

/************************************************************************/
/*      ͨ��ҵ���ѯ�� ����ID    [0X1000 ~ 0X1BFF]                           */
/***********START*************************************************************/
typedef HI_U16 ID_DIAG_QUERY;
#define ID_DIAG_CMD_QUERY_ID_SEG_START                (0x1000)
#define ID_DIAG_CMD_QUERY_ID_SEG_END                  (0x1BFF)

#define ID_DIAG_CMD_QUERY_NETWORK_CONVERGENCE_TIME    (0x1000)       // ��ѯվ����������ʱ
#define ID_DIAG_CMD_QUERY_NETWORK_TOPO                (0x1001)       // ��ѯ�������˱�
#define ID_DIAG_CMD_QUERY_ROUTER_TABLE                (0x1002)       // ��ѯ·�ɱ���Ϣ
#define ID_DIAG_CMD_QUERY_TX_STATICS                  (0X1003)       // ��ѯ���ͻ�ͳ����
#define ID_DIAG_CMD_QUERY_RX_STATICS                  (0X1004)       // ��ѯ���ջ�ͳ����
#define ID_DIAG_CMD_QUERY_RSB_STATICS_PK              (0X1005)       // ��ѯ����ģ��ͳ����
#define ID_DIAG_CMD_TF_COLLECT_DATA_ZERO_CROSS_NTB    (0x1006)       // �������ɼ�����NTB
#define ID_DIAG_CMD_QUERY_ALL_STA_INFO                (0X1007)       // ��ѯվ������������ߴ���
#define ID_DIAG_CMD_QUERY_DEVICE_FREQ_INFO            (0X1008)       // ��ѯվ��Ƶ����Ϣ
#define ID_DIAG_CMD_QUERY_NETWORK_STATE_INFO          (0X1009)       // ��ѯCCO��������Ϣ����STA�̹�����Ϣ
#define ID_DIAG_CMD_QUERY_NEIGHBOR_NETWORK            (0x100A)       // ��ѯ�ھ�������Ϣ
#define ID_DIAG_CMD_QUERY_NETWORK_ATTRIBUTE           (0x100B)       // ��ѯվ����������
#define ID_DIAG_CMD_QUERY_REQUEST_JOIN_FAILED_COUNT   (0x100C)       // ��ѯվ�����ʧ�ܴ���
#define ID_DIAG_CMD_QUERY_JOIN_NETWORK_EXPIRED        (0x100D)       // ��ѯվ�������ʱ
#define ID_DIAG_CMD_QUERY_ASSOCIATED_STAS             (0x100E)       // ��ѯվ����������б�
#define ID_DIAG_CMD_QUERY_DISCOVER_LIST               (0x100F)       // ��ѯվ�㷢���б�
//#define ID_DIAG_CMD_QUERY_NTB_STAT                    (0x1010)       // ��ѯNTBģ��ͳ����
#define ID_DIAG_CMD_QUERY_PHS_STAT                    (0x1011)       // ��ѯ��λʶ��ģ��ͳ����
#define ID_DIAG_CMD_QUERY_NDM_CONNECT_STAT            (0x1012)       // ��ѯ������������Ϣ(��ͣ��)
#define ID_DIAG_CMD_QUERY_VIOLENCE_STA                (0x1013)       // ��ѯCCO�˾ܾ��б�(�Ѳ���ʹ��)
#define ID_DIAG_CMD_QUERY_SEG_STATICS_PK              (0x1014)       // ��ѯ�ֶ�ģ��ͳ����
#define ID_DIAG_CMD_QUERY_NDM_STATICS                 (0x1015)       // ��ѯ������ģ��ͳ����
#define ID_DIAG_CMD_QUERY_DISCOVER_LIST_SM            (0x1016)       // ����ķ����б�
#define ID_DIAG_CMD_QUERY_MAC_SYS_ERR                 (0x1017)       // MAC��ϵͳ����

#define ID_DIAG_CMD_QUERY_FREQUENCY_STATUS            (0x101b)       // �Ƿ���յ�ȫ��Ƶ�α��֡
#define ID_DIAG_CMD_QUERY_MULTI_NETWORK_RCV_INFO      (0x101d)       // ��ѯ�������½����ű�֡��SOF֡FC����
#define ID_DIAG_CMD_CCO_MAC_MULTYNET_QUERY            (0x101e)       // ��ѯSNID(STA��CCO��Ϊ��ͬ��)
#define ID_DIAG_CMD_CCO_MAC_MULTYNET_SET              (0x101f)       // ��������������
#define ID_DIAG_CMD_MAC_FREQ_SET                      (0x1020)       // ����Ƶ��,��Ƶʱ��,�������Ƶ״̬STA��CCO��ͬ
#define ID_DIAG_CMD_MAC_FREQ_QUERY                    (0x1021)       // ��ѯƵ�Σ��Ƿ������У��еĽ��Ȼ�ʣ�೤ʱ�� STA��CCO��ͬ
#define ID_DIAG_CMD_CCO_MAC_WHITE_STA_QUERY           (0x1022)       // �������Ƿ�����״̬��ѯ���Ƿ�ѱ������ڰ�����
#define ID_DIAG_CMD_CCO_MAC_WHITE_STA_SET             (0x1023)       // ����������
#define ID_DIAG_CMD_CCO_MAC_CLR                       (0x1024)       // ����ܾ��б�
#define ID_DIAG_CMD_QUERY_CVG_STATICS_PK              (0x1025)       // ��۷ַ�����ͳ��
#define ID_DIAG_CMD_QUERY_LEAVE_IND_STAT_INFO         (0x1026)       // ͳ������ָʾ����
#define ID_DIAG_CMD_SOFT_RESET                        (0x1027)       // ��ѯվ����������Ϣ
#define ID_DIAG_CMD_PROXY_NUM_QUERY                   (0x1028)       // ��ѯ��ǰ�Ĵ�����
#define ID_DIAG_CMD_CHILD_SUM_QUERY                   (0x1029)       // ��ѯĳվ�����վ����
#define ID_DIAG_CMD_NM_PACKET_STAT_QUERY              (0x102a)       // ���ܱ��ĸ���ͳ��
#define ID_DIAG_CMD_NM_ASSOC_REQ_STAT_QUERY           (0x102b)       // ����������ͳ��
#define ID_DIAG_CMD_NM_ASSOC_CNF_STAT_QUERY           (0x102c)       // �����ظ�����ͳ��
#define ID_DIAG_CMD_STA_SNID_INFO                     (0x102d)       // STAѡ��SNID״̬
#define ID_DIAG_CMD_STA_PROXY_CHANGE_INFO             (0x102e)       // STA��ȡ��������ά�ɲ���Ϣ
#define ID_DIAG_CMD_ASSOC_INFO                        (0x102f)       // ��ȡ���������������������Ŀ�ά�ɲ���Ϣ
#define ID_DIAG_CMD_NDM_CONNECT                       (0x1030)       // ���ó��������Ӳ���
#define ID_DIAG_CMD_QUERY_FC_PB_FAULT_INFO            (0x1031)       // ��ȡfc��pb��λͳ��
#define ID_DIAG_CMD_NDC_TEST                          (0x1032)       // NDCͨ��������������
#if defined(PRODUCT_CFG_PRODUCT_TYPE_NDM) || defined(PRODUCT_CFG_HSO)
#define ID_DIAG_CMD_GET_NDM_CTRL_ATTENUATE            (0x1033)       // ��ѯ������˥��ģʽ
#define ID_DIAG_CMD_SET_NDM_CTRL_ATTENUATE            (0x1034)       // ���ó�����˥��ģʽ
#endif
#define ID_DIAG_CMD_NDM_IP_MAC_MAP_INFO               (0x1035)       // ��������IP��MACӳ���
#define ID_DIAG_CMD_NDM_ATTR_SET                      (0x1036)       // ���ó�����������
#define ID_DIAG_CMD_LED_STATUS_INFO                   (0x1037)       // ��ѯLEDָʾ��״̬
#define ID_DIAG_CMD_QUERY_RX_DIAGNOSTIC               (0x1038)       // ��ѯ���ջ����ͳ����
#define ID_DIAG_CMD_QUERY_TX_DIAGNOSTIC               (0x1039)       // ��ѯ���ͻ����ͳ����
#define ID_DIAG_CMD_QUERY_PLC_DEV_STAT_INFO           (0x103a)       // ��ѯPLC IPģ��ͳ����
#define ID_DIAG_CMD_MULTI_EXCEPTION_STATUS_INFO       (0x103b)       // ��ѯ�������쳣״̬��Ϣ
#define ID_DIAG_CMD_QUERY_COMM_RATE_INFO              (0x1040)       // ��ѯ�ϱ�ͨ������Ϣ
#define ID_DIAG_CMD_QUERY_CHOSEN_STA_DISLIST          (0x1041)       // ��ѯָ��վ��ķ����б�
#define ID_DIAG_CMD_QUERY_CHOSEN_STA_TOPOINFO         (0x1042)       // ��ѯָ��վ���������Ϣ
#define ID_DIAG_CMD_QUERY_VERSION_INFO                (0x1043)       // ��ѯվ��İ汾��Ϣ�͹��NV������Ϣ
#define ID_DIAG_CMD_QUERY_BACKUP_MAC                  (0x1045)       // ��ѯ����MAC��ַ
#define ID_DIAG_CMD_SET_BACKUP_MAC                    (0x1046)       // ���ñ���MAC��ַ
#define ID_DIAG_CMD_NDM_FOLLOW_STAT_STRU              (0x1047)       // ��ѯ������Ѳ�����������Ϣ
#define ID_DIAG_CMD_SET_NDM_FOLLOW                    (0x1048)       // ���ó������������湦��
#define ID_DIAG_CMD_QUERY_MAX_VIOLENCE_STA            (0x1049)       // ���ò�ѯ�ܾ��б����Ϊ200����Ϣ
#define ID_DIAG_CMD_MAC_GET_WHITELIST                 (0x104a)       // ��ѯ������
#define ID_DIAG_CMD_MAC_REPORT_WHITELIST_CHANGED      (0x104b)       // �ϱ����������
#define ID_DIAG_CMD_QUERY_TX_SEND_IP_TIMEOUT_STAT     (0x104d)       // ���ò�ѯIP���ķ��ͳ�ʱ��Ϣ
#define ID_DIAG_CMD_QUERY_PLC_SWITCH_TRX_STAT         (0x104e)       // ��ѯ����ģ���շ�ͳ����
#define ID_DIAG_CMD_QUERY_PLC_SWITCH_MAPPING_TABLE    (0x104f)       // ��ѯ����ģ��ӳ�����Ϣ
#define ID_DIAG_CMD_QUERY_PLC_SWITCH_FRAGMENT_QUEUE   (0x1050)       // ��ѯ����ģ���Ƭ������Ϣ
#define ID_DIAG_CMD_QUERY_PLC_SWITCH_PROTOCOL_STAT    (0x1051)       // ��ѯ����ģ�鲻ͬЭ�鱨��ͳ����
#define ID_DIAG_CMD_SET_FAULT_LOCAT                   (0x1052)       // HSO�·�������ֹͣ���������϶�λ����
#define ID_DIAG_CMD_QUERY_FAULT_LOCAT_STATUS          (0x1053)       // HSO�·���ѯ���������϶�λ״̬����
#define ID_DIAG_CMD_QUERY_FAULT_LOCAT_STAT            (0x1054)       // ��ѯ���϶�λͳ����Ϣ
#define ID_DIAG_CMD_QUERY_RF_TX_DIAGNOSTIC            (0x1055)       // ��ѯRF�豸TXģ����ϼ���
#define ID_DIAG_CMD_QUERY_RF_RX_DIAGNOSTIC            (0x1056)       // ��ѯRF�豸RXģ����ϼ���
//#define ID_DIAG_CMD_QUERY_RF_SNID_CHANGE_INFO         (0x1057)       // ��ѯRF�豸RXģ��SNID��ͻ�ı���Ϣ
#define ID_DIAG_CMD_QUERY_LOCK_NETWORK_STATUS         (0x1058)        // ��ѯ��������״̬
#define ID_DIAG_CMD_NETWORK_MODE_SWITCH_REQ           (0x1059)       // ��������ģʽ�л�
#define ID_DIAG_CMD_UNLOCK_NETWORK_STATUS             (0x105a)       // ������������

#define ID_DIAG_CMD_QUERY_DUAL_MODULE_INFO            (0x1060)       // ��ѯ������˫ģվ����Ϣ
#define ID_DIAG_CMD_QUERY_STA_INFO                    (0x1061)       // ��ѯվ����Ϣ(Ϊ101b��ѯ����Ĳ���)
#define ID_DIAG_CMD_QUERY_CONNECTED_GRAPH_INFO        (0x1062)       // ��ѯվ����ͨ��Ϣ
#define ID_DIAG_CMD_QUERY_RF_CURRENT_COMM_RATE        (0x1063)       // ��ѯRF�豸��ѯ��ǰͨ����
#define ID_DIAG_CMD_QUERY_RF_MODE_CHANGE_COMM_RATE    (0x1064)       // ��ѯRF�豸��ѯģʽ�л�ʱͨ����

#define ID_DIAG_CMD_TF_IDENTIFY_SET_REQ               (0x1065)       // HSO����̨��ʶ��
#define ID_DIAG_CMD_TF_IDENTIFY_STA_CONFIG_REQ        (0x1066)       // HSO����ʶ��Χ
#define ID_DIAG_CMD_TF_IDENTIFY_RESULT_REQ            (0x1067)       // HSO��ѯ̨��ʶ����
#define ID_DIAG_CMD_BLACKLIST_REQ                     (0x1068)       // HSO��ѯ̨��ʶ�������
#define ID_DIAG_CMD_BLACKLIST_CLEAR_REQ               (0x1069)       // HSO���̨��ʶ�������
#define ID_DIAG_CMD_TF_CCO_STATIC                     (0x106a)       // ̨��ʶ��CCO�˵�ͳ����
#define ID_DIAG_CMD_TF_STA_STATIC                     (0x106b)       // ̨��ʶ��STA�˵�ͳ����
#define ID_DIAG_CMD_TF_STATIC                         (0x106c)       // ̨��ʶ�𹫹�ͳ����
#define ID_DIAG_CMD_QUERY_TF_IDENTIFY_INFO            (0x106d)       // ��ѯ̨��ʶ��ͳ����Ϣ
#define ID_DIAG_CMD_QUERY_TF_IDENTIFY_STA_RANGE       (0x106e)       // ��ѯ̨��ʶ��Χ
#define ID_DIAG_CMD_TF_LOCK_STA_OM                    (0x106f)       // ���������STA�˵Ŀ�ά�ɲ���Ϣ
#define ID_DIAG_CMD_QUERY_HIS_RESULT_CACHE_INFO       (0x1070)       // ��ѯ��ʷ̨��ʶ��������
#define ID_DIAG_CMD_QUERY_CUR_RESULT_CACHE_INFO       (0x1071)       // ��ѯ��ǰ̨��ʶ��������
#define ID_DIAG_CMD_TF_CCO_FL_STATIC                  (0x1073)       // CCO�˵�ͳ����
#define ID_DIAG_CMD_QUERY_TF_NTB_STATIC               (0x1074)       // CCO�˵�ͳ����
#define ID_DIAG_CMD_QUERY_NDM2NDM_TF_STATUS_REQ       (0x1075)       // HSO��ѯndm̨��ʶ��״̬�ͽ��
#define ID_DIAG_CMD_NDM2NDM_TF_SET_REQ                (0x1076)       // HSO����ndm̨��ʶ��
#define ID_DIAG_CMD_QUERY_PHASE_IDENTIFY_RESULT_REQ   (0X1077)       // HSO��ѯ��λʶ����
#define ID_DIAG_CMD_PI_CCO_STATIC                     (0x1078)       // ��λʶ��CCO�˵�ͳ����
#define ID_DIAG_CMD_PI_CCO_BASE_NTB                   (0x1079)       // ��λʶ��CCO�˻�׼NTB�����ϱ�
#define ID_DIAG_CMD_ASSOC_DFX_INFO                    (0x107A)       // վ����������ά�ɲ�
#define ID_DIAG_CMD_MULTI_NETWORK_ASSOC_DFX_INFO      (0x107B)       // �����������ά�ɲ�
#define ID_DIAG_CMD_SERIAL_NUM_CHANGE_DFX_INFO        (0x107C)       // �������к�������ά�ɲ�
#define ID_DIAG_CMD_TF_IDENTIFY_STATE                 (0x107d)       //̨��ʶ��״̬��ѯ
#define ID_DIAG_CMD_QUERY_CONNECT_NDM_INFO            (0x107e)       // CCO����NDM��ͳ����Ϣ
#define ID_DIAG_CMD_QUERY_WOBBLE_WITHOUT_SIGNAL       (0x107f)       // ��̨��ʶ�����з���ʱ���Ŷ�ͳ����Ϣ
#define ID_DIAG_CMD_QUERY_WOBBLE_WITH_SIGNAL          (0x1080)       // ��̨��ʶ�����з���ʱ���Ŷ�ͳ����Ϣ
#define ID_DIAG_CMD_TX_DUTY_STATIC                    (0x1081)       // ��ѯ����ռ�ձ���Ϣ
#define ID_DIAG_CMD_QUERY_FL_TOPO_REFERENCE           (0x1082)
#define ID_DIAG_CMD_QUERY_FL_TOPO_RECORD              (0x1083)
#define ID_DIAG_CMD_CLEAR_FL_INFO                     (0x1084)
#define ID_DIAG_CMD_QUERY_FL_STATICS                  (0X1085)       // ��ѯ�ؼ��ڵ����ͳ����
#define ID_DIAG_CMD_QUERY_RF_ASSOC_INFO               (0x1086)       // ��ѯ˫ģRF����������Ϣ
#define ID_DIAG_CMD_TF_EXACT_FALSE_INFO               (0x1087)       // STA��ͬ�������龯��Ϣ
#define ID_DIAG_CMD_SET_LISTEN_TIME_BEFORE_FIRST_BEA  (0x1088)       // CCO�����ö��������ʱ��
#define ID_DIAG_CMD_RECORD_SWITCH_FREQ_INFO           (0x1089)       // STA��NDM�˼�¼�յ����л�Ƶ��֪ͨ��Ϣ
#define ID_DIAG_CMD_COORDINATE_CONFLICT_INFO          (0x1090)       // STA�������ͻ��Ϣ
#define ID_DIAG_CMD_COORDINATE_MULTI_CCO_INFO_PLC     (0x1091)       // STAͬһ������¶��CCO��Ϣ
//#define ID_DIAG_CMD_COORDINATE_MULTI_CCO_INFO_RF      (0x1092)       // RF��·��STAͬһ������¶��CCO��Ϣ
#define ID_DIAG_CMD_QUERY_TF_NOISE_CACHE_INFO         (0x1093)       // STA��ͳ��̨��ʶ������������Ϣ
#define ID_DIAG_CMD_TF_CROSSTALK_RESULT_REQ           (0x1094)       // HSO��ѯ̨��ʶ�����зǱ�̨��վ����Ϣ
#define ID_DIAG_CMD_PTPC_TEST                         (0x1096)
#define ID_DIAG_CMD_PTPC_STATE_INFO                   (0x1097)
#define ID_DIAG_CMD_PTPC_COORDINATE_INFO              (0x1098)
#define ID_DIAG_CMD_PTPC_STAT_INFO                    (0x1099)
#define ID_DIAG_CMD_PTPC_STATE_RECORD_INFO            (0x109a)
//#define ID_DIAG_CMD_NTB_SOFT_INFO                      (0x109b)

//#define ID_DIAG_CMD_TF_STATICS_STAT                   (0x109c)       // ��ѯ̨��ʶ������ͳ����
#define ID_DIAG_CMD_TF_CHIP_STAT                      (0x109d)       // ��ѯоƬ��̨��ʶ��ͳ����
#define ID_DIAG_CMD_QUERY_NTB_EXCEPTION               (0x109e)
#define ID_DIAG_CMD_NTB_NEIGHBOR_FREQERR              (0X10a2)

#ifdef PRODUCT_CFG_ONLINE_TEST_MODE
#define ID_DIAG_CMD_QUERY_GT_TX_STATISTIC_INFO        (0x10a3)
#define ID_DIAG_CMD_QUERY_GT_RX_STATISTIC_INFO        (0x10a4)
#define ID_DIAG_CMD_QUERY_GT_DFX_STATISTIC_INFO       (0x10a5)
#define ID_DIAG_CMD_QUERY_GT_NTB_STATISTIC_INFO       (0x10a6)
#endif

#define ID_DIAG_CMD_QUERY_GT_NTB_RESET_INFO           (0x10a7)
#define ID_DIAG_CMD_EMC_POWERLOADING_CONFIG           (0X10A8)       // EMC����POWERLOADING
#define ID_DIAG_CMD_TX_SEND_QUEUE_INFO                (0x10aa)
#define ID_DIAG_CMD_SLOT_UPDATE_INFO                  (0x10ab)
#define ID_DIAG_CMD_QUERY_NTB_STAT_V200               (0x10ac)       // ��ѯNTBģ��ͳ����
#define ID_DIAG_CMD_QUERY_NTB_FREQERR_STAT_V200       (0x10ad)       // ��ѯNTB Ƶƫͳ��

#define ID_DIAG_CMD_SEND_SC_FRAME_STUB                (0x10ae)       // ����խ��֡��������
#define ID_DIAG_CMD_QUERY_SC_NM_STATISTIC             (0x10af)       // ���ز����ܲ�ͳ������ѯ
#define ID_DIAG_CMD_QUERY_SC_TX_STATISTIC             (0x10b0)       // ���ز����ͻ�ͳ������ѯ
#define ID_DIAG_CMD_QUERY_SC_RX_STATISTIC             (0x10b1)
#define ID_DIAG_CMD_TF_PASSIVE_STATIC                 (0x10b2)       // ����ʶ��ͳ����
#define ID_DIAG_CMD_TFP_REPORT_NTB                    (0x10b4)       // �ϱ�NTB��HSO����.
#define ID_DIAG_CMD_QUERY_MF_UPPER_BRIDGING_STATISTIC (0x10b5)       //
#define ID_DIAG_CMD_QUERY_MF_DETECT_NETWORK_STATISTIC (0x10b6)       //
#define ID_DIAG_CMD_QUERY_MF_DETECT_STATION_STATISTIC (0x10b7)
#define ID_DIAG_CMD_QUERY_MULTI_FREQUENCY             (0x10b8)
#define ID_DIAG_CMD_QUERY_RT_REPAIR_STAT              (0x10b9)       // ʵʱ·���޸�ģ��ͳ����
#define ID_DIAG_CMD_QUERY_PTCL_DETECT_STAT            (0x10ba)       // Э��̽��ͳ����
#define ID_DIAG_CMD_QUERY_NETWORK_ATTRIBUTE_V200      (0x10bb)       // ��ѯվ�����������(V200�汾)
#define ID_DIAG_CMD_QUERY_TFP_NSR_NTB_FILTER_STATIC   (0x10bc)       // ͳ������ʶ���������ntb�쳣�˳�ͳ����
#define ID_DIAG_CMD_TFP_IDENTIFY_RESULT_STATIC        (0x10bd)       // ͳ������ʶ����
#define ID_DIAG_CMD_MULTI_NETWORK_RCV_INFO_V200       (0x10be)       // V200�����籨�Ľ��ղ�ѯ
#define ID_DIAG_CMD_QUERY_NTB_STATIC_V200             (0x10bf)       // v200 NTBͳ������ѯ
#define ID_DIAG_CMD_SET_TRANSMIT_POWER_V200           (0x10C0)       // ���÷��书��
#define ID_DIAG_CMD_DELAYED_RESTART_V200              (0x10C1)       // ȫ���ӳ�����
#define ID_DIAG_CMD_COMMON_TEMP_COMMAND               (0x10C2)       // ͨ����ʱ����
#define ID_DIAG_CMD_REPORT_PB_STAT                    (0x10C3)       // PB���ϱ�

#define ID_DIAG_CMD_GET_TOP_NEIGHBOR                  (0x10c4)      // ��ѯվ����õ��ھ�
#define ID_DIAG_CMD_STA_COLLECT_RESULT_DETAIL         (0x10c5)      // ��ѯSNRͳ�ƽ����ϸ����
#define ID_DIAG_CMD_STA_COLLECT_RESULT_SIMPLE         (0x10c6)      // ��ѯSNRͳ�ƽ����Ҫ����
#define ID_DIAG_CMD_STA_LOCK_NETWORK                  (0x10c7)      // ����STA��������
#define ID_DIAG_CMD_STA_SNR_HISTORY_NODE              (0x10c8)      // ��ѯ������ʷ��¼
#define ID_DIAG_CMD_STA_SNR_NET_HISTORY               (0x10c9)      // ��ѯ������48����ʷ��¼
#define ID_DIAG_CMD_CCO_SET_NODE_FIND_PARA            (0x10ca)      // CCO���ýڵ���Ҳ���
#define ID_DIAG_CMD_STA_FAST_CHANGE_STAT              (0x10cb)      // ��ѯվ�����������¼
#define ID_DIAG_CMD_STA_STABLE_CHANGE_STAT            (0x10cc)      // ��ѯվ�㳤�������л������¼
#define ID_DIAG_CMD_STA_SNR_CTRL_STAT                 (0x10cd)      // ��ѯSNR���Կ���״̬
#define ID_DIAG_CMD_CCO_SNR_TF_STAT                   (0x10ce)      // ��ѯ̨��ʶ��״̬
#define ID_DIAG_CMD_QUERY_LOCK_INFO_EXP               (0x10cf)      // ��ѯ������Ϣ
#define ID_DIAG_CMD_QUERY_POWER_FAILURE_INFO          (0x10d0)      // ��ѯͣ���ϱ��¼���Ϣ
#define ID_DIAG_CMD_REPORT_PB72_STAT                  (0x10D1)      // 72PB���ϱ�
#define ID_DIAG_CMD_REPORT_PB136_STAT                 (0x10D2)      // 136PB���ϱ�
#define ID_DIAG_CMD_REPORT_PB264_STAT                 (0x10D3)      // 264PB���ϱ�
#define ID_DIAG_CMD_QUERY_NEAR_STA_DATA               (0x10d4)      // ��ѯվ��ɼ��ĸ���վ����Ϣ
#define ID_DIAG_CMD_QUERY_NEAR_NET_INFO               (0x10d5)      // ��ѯվ��ɼ��ĸ���������Ϣ
#define ID_DIAG_CMD_SET_BELONG_NETWORK                (0x10d6)      // ����վ��Ĺ�������
#define ID_DIAG_CMD_CLEAR_BELONG_NETWORK              (0x10d7)      // ���վ��Ĺ�������
#define ID_DIAG_CMD_SET_SWITCH_FREQ                   (0x10D8)      // ȫ���л�Ƶ��
#define ID_DIAG_CMD_POWER_CTRL_STAT                   (0x10D9)      // ��ѯ���ʿ���ͳ����
#define ID_DIAG_CMD_GET_BELONG_CHANGE_REOCRDS         (0x10da)      // ��ȡ���������¼
#define ID_DIAG_CMD_GET_LOCK_REOCRDS                  (0x10db)      // ��ȡ������¼
#define ID_DIAG_CMD_SET_TRANSMIT_POWER_BY_FREQ        (0x10dc)      //��ʱ���ø�Ƶ�����ַ��书��
#define ID_DIAG_CMD_QUERY_ROUTE_PATH_REFERENCE        (0x10dd)      //��ѯ·����Ϣ��ʹ�����
#define ID_DIAG_CMD_QUERY_PK_FILTER                   (0x10de)      //��ѯ���Ĺ������
#define ID_DIAG_CMD_QUERY_DIRECT_SEND_INFO            (0x10df)      //��ѯֱ�ӷ�����Ϣ
#define ID_DIAG_CMD_FAST_NOTICE_INFO                  (0x10e0)
#define ID_DIAG_CMD_QUERY_TFP_TRX_STAT                (0x10e1)      // ��ѯ�����շ�ͳ����Ϣ
#define ID_DIAG_CMD_TFP_IDENTIFY_SET_REQ              (0x10e2)      // HSO�·���������ʶ������
#define ID_DIAG_CMD_TFP_IDENTIFY_QUERY_RESULT         (0x10e3)      // HSO�·���ѯ����ʶ��������
#define ID_DIAG_CMD_QUERY_ENERGY_STATICS              (0x10e4)      // ��ԴӦ��ά����Ϣ����
#define ID_DIAG_CMD_TFP_IDENTIFY_CLEAR_RESULT         (0x10e5)      // HSO�·��������ʶ��������

//ͣ��ģ��ά����Ϣ�鿴
#define ID_DIAG_CMD_CCO_GET_POWEROFF_INFO             (0x10e6)      // ��ȡͣ�����ϱ���Ϣ
#define ID_DIAG_CMD_CCO_GET_STA_POWEROFF_INFO         (0x10e7)      // ��ȡָ��վ��ͣ�����ϱ���Ϣ
#define ID_DIAG_CMD_CCO_GET_POWEROFF_HISTORY          (0x10e8)      // ��ѯSTA�����ھ�Ӧ��̽��ĵ�ǰ��Ϣ
#define ID_DIAG_CMD_STA_GET_CUR_HELP_REPORT_INFO      (0x10e9)      // STA��STA�����ھ�Ӧ��̽�����ʷ��Ϣ
#define ID_DIAG_CMD_STA_GET_HB_TRANS_DELAY_INFO       (0x10ea)      // ��ѯSTAҶ�ӽڵ������ӳ�ת����Ϣ
#define ID_DIAG_CMD_STA_GET_HIS_HELP_REPORT_INFO      (0x10eb)      // ��ȡվ��ͣ���ϱ�������ʷ��¼
#define ID_DIAG_CMD_NDM_GET_CONNECT_DFX_INFO          (0x10ec)      // ��ȡndmվ��������Ϣ
#define ID_DIAG_CMD_NDM_GET_DTECTID_DFX_INFO          (0x10ed)      // ��ȡndmվ��̽����Ϣ


#define ID_DIAG_CMD_QUERY_ROUTE_PROXY_INFO            (0x10f0)      // ��ѯ��Ƶ�ɼ����·�ɼ���������Ϣ

#define ID_DIAG_CMD_MAC_PHYSICAL_TEST_MODE            (0x1A01)

#define ID_DIAG_CMD_MAC_SET_PLC_PROTOCOL              (0x1A06)      // ����PLCЭ��汾
#define ID_DIAG_CMD_MAC_QUERY_PLC_PROTOCOL            (0x1A07)      // ��ѯPLCЭ��汾

// ����NTB-����ʶ�����ά�� 0x1a80 ~ 0x1a89
#define ID_DIAG_CMD_TF_ND_MIN                         (0x1a80)
#define ID_DIAG_CMD_TF_ND_STA_GET_CUR_INFO            (0x1a80)      // STA: ��ѯ��ǰʶ����Ϣ
#define ID_DIAG_CMD_TF_ND_STA_GET_HISTORY             (0x1a81)      // STA: ��ѯ��ʷ��¼
#define ID_DIAG_CMD_TF_ND_STA_GET_STATISTIC           (0x1a82)      // STA: ��ѯ״̬��¼
#define ID_DIAG_CMD_TF_ND_STA_GET_NET_HIS             (0x1a83)      // STA: ��ѯ���������ʷ��¼
#define ID_DIAG_CMD_TF_ND_STA_CLEAR_RESULT            (0x1a85)      // STA: ���ʶ����
#define ID_DIAG_CMD_TF_ND_MAX                         (0x1a89)

#if defined(PRODUCT_CFG_PRODUCT_TYPE_NDM) || defined(PRODUCT_CFG_HSO)
#define ID_DIAG_CMD_MAC_REPROT_NDM_CAPTURE            (0x1a90)      //NDM�ϱ�ץ����Ϣ
#define ID_DIAG_CMD_MAC_SET_NDM_CAPTURE_SWITCH        (0x1a91)      //HSO�·�ץ�����˿���
#define ID_DIAG_CMD_MAC_SET_NDM_ACCESS_INFO           (0x1a92)      //HSO�·�������Ϣ
#endif

// ����SNR�������ά�� 0x1a90 ~ 0x1a99
#define ID_DIAG_CMD_SNR_MIN                           (0x1b90)
#define ID_DIAG_CMD_SNR_SET_VALUE_CNT_PARAM           (0x1b90)      // ����ֵ��ͳ�Ʋ���
#define ID_DIAG_CMD_SNR_QUERY_VALUE_CNT_DATA          (0x1b91)      // ��ѯֵ��ͳ����Ϣ
#define ID_DIAG_CMD_SNR_MAX                           (0x1b99)

/**************END**************************************************/

/**************END**************************************************/

/************************************************************************/
/*     ������ҵ�� ��ID  [0x1c00 ~ 0x1c80 )                               */
/***********START*************************************************************/
#define ID_DIAG_CMD_NDM_CTRL_BUSI_ID_SEG_START  (0x1c00)
#define ID_DIAG_CMD_NDM_CTRL_BUSI_ID_SEG_END    (0x1c80)

#define ID_DIAG_CMD_NDM_CTRL_ROUTE_LOCK     (0x1C00)  // ·������
#define ID_DIAG_CMD_NDM_CTRL_ROUTE_UNLOCK   (0x1C01)  // ·�ɽ���
#define ID_DIAG_CMD_NDM_CTRL_ROUTE_CONFIG   (0x1C02)  // ��̬·������
#define ID_DIAG_CMD_NDM_CTRL_TRACE          (0x1C03)  // trace ����
#define ID_DIAG_CMD_NDM_CTRL_PING           (0x1C04)  // ping ����
#define ID_DIAG_CMD_NDM_CTRL_SEARCH_DEV     (0x1C05)  // �����豸
/**************END**************************************************/

/*********************************************************/
/***    ����ҵ����  [0x1c81~0x1ca0)                    ***/
/********START*************************************************/
#define ID_DIAG_CMD_QUERY_DFX_NDM_NM_ATTR    (0x1c81)   //���ڳ�������ѯ����Ļ�����Ϣ
#define ID_DIAG_CMD_NDM_CTRL_PIPE_ACK        (0x1c82) //������Ҫ������ͨ����ÿ����һ��վ���ظ�ACK��.
#define ID_DIAG_CMD_QUERY_CMD_NOT_SUPPORT    (0x1c83)
#define ID_DIAG_CMD_MAC_QUERY_MAC_BUFFER     (0x1c84)
//#define ID_DIAG_CMD_MAC_CONFIG_SW            (0x1C85)
#define ID_DIAG_CMD_MAC_HANDLE_CONNECT_IND   (0x1C86)
#define ID_DIAG_CMD_MAC_HANDLE_CONNECT_CNF   (0x1C87)

/********END**************************************************/

/************************************************************/
/*** ���� [0x1ca1 ~ 0x2000)        **************************/
/************************************************************/


///////////////////////////////////////////////////////
////////////////----������ ��ѯ����----------/////////////////////
//////////////////////////////////////////////////////////


/***********************************************/
// �����п��� 0x1C00 ~ 0x1c0F
/*************************************************/

//////////////////////////////////////////////////////////////////////////
// ���������DIAG ���� (0x1C10 ~ 0X1CFF)
// added by db 2012-06-07
//////////////////////////////////////////////////////////////////////////


/************************************************************************/
/*    START  �����ϱ��� �Լ� �洢�� ID  0x2900~0x3000                   */
/************************************************************************/
typedef HI_U16 ID_DIAG_REPORT_CMD;

#define ID_DIAG_CMD_NOTIFY_MAC_SPEED_LOW             (0x2900)
#define ID_DIAG_CMD_REPORT_TXRX_STATIC_SPEED         (0x2901)
#define ID_DIAG_CMD_REPORT_JOIN_FAILED               (0x2902)
#define ID_DIAG_CMD_REPORT_NETWORK_ATTRIBUTE         (0x2905)
#define ID_DIAG_CMD_RECORD_SPEED_ABNORMITY           (0x2922)

/*******END �����ϱ��� �Լ� �洢�� ID  **************************************/
/************************************************************************/

/************************************************************************/
/*      �����ϱ���ID  0x2900~0x3000                                                  */
/************************************************************************/


/************************************************************************/
/*   �¼�ID (0x1000,0x5000   )                                         */
/************************************************************************/
typedef HI_U16 ID_DIAG_EVENT;
#define ID_DIAG_EVENT_NO_SACK_BROADCAST (0x1001)
#define ID_DIAG_EVENT_NOTIFY_MAC_RX_SPEED_LOW   (0x1002)
#define ID_DIAG_EVENT_NOTIFY_MAC_TX_SPEED_LOW   (0x1003)


/************************************************************************/
/*        2    ��ѯ��                                                      */
/************************************************************************/
/**
 *
 * 2.1    ͷ�ˣ���������ʱ��
 *
 */
typedef struct _DIAG_CMD_QUERY_NETWORK_CONVERGENCE_TIME_STRU_
{
   HI_U32 convergence_time;             //�������ʱ�䣬��λs
} DIAG_CMD_QUERY_NETWORK_CONVERGENCE_TIME_STRU;

/**
 *
 * 2.1.1    ͷ�ˣ�Ƶ��״̬
 *
 */
typedef struct _DIAG_CMD_QUERY_FREQUENCY_STATUS_STRU_
{
    HI_U8 is_rcv_change_freq_mode_notify;//0: Ƶ������     1:Ƶ�������仯   2 :����CCO
    HI_U8 pad[3];
} DIAG_CMD_QUERY_FREQUENCY_STATUS_STRU;

/**
 *
 * 2.2    ͷ�ˣ�����������Ϣ
 *
 */
#define MAC_APPOINTED_STATIC_PROXY_TYPE            (1)
#define MAC_DYNAMIC_CHOSE_PROXY_TYPE               (2)

typedef struct
{
    HI_U16 tei;                                     // ·����һ��TEI��ַ
    HI_S16 snr;                                     // ·��ƽ�������
    HI_U8  channel_info;                            // �ŵ�����ֵ
    HI_U8  par;                                     // �����
    HI_U8  up_comm_rate;                            // ����ͨ����
    HI_U8  down_comm_rate;                          // ����ͨ����
    HI_U8  send_success_rate;                       // ʵ�ʷ������ݳɹ���
    HI_U8  rsvd[3];
}rt_path_node_comm_info_dfx;

typedef struct
{
    rt_path_node_comm_info_dfx proxy_path;
    rt_path_node_comm_info_dfx up_up_level_path;
    rt_path_node_comm_info_dfx up_level_path;
    rt_path_node_comm_info_dfx same_level_path;
}rt_path_comm_info_dfx;

typedef enum
{
    NFC_DIAG_CMD_TOPO_ENTRY_VER0 = 0,
    NFC_DIAG_CMD_TOPO_ENTRY_VER1,
} NFC_DIAG_CMD_TOPO_ENTRY_VER_E;

#define NFC_DIAG_CMD_TOPO_ENTRY_VER     (NFC_DIAG_CMD_TOPO_ENTRY_VER1)

typedef struct
{
    HI_U8 mac[6];                           // վ��MAC��ַ
    HI_U8 ucSpcVer;                         // code+nv SPC image.hupg
    HI_U8 ucSpcNvVer;                       // nv image.hupg: SPC Ver

    HI_U32 level                : 4;        // վ��㼶
    HI_U32 ability              : 4;        // վ���ɫ
    HI_U32 tei                  : 12;       // վ��TEI��ַ
    HI_U32 proxy_tei            : 12;       // վ�㸸����TEI��ַ

    HI_U32 join_time;                       // վ���������ʱ��

    HI_U8  product_type         : 3;        // ��Ʒ��̬
    HI_U8  in_use               : 1;        // վ���Ƿ�����
    HI_U8  pad                  : 4;

    HI_U8  sta_phase0           : 2;        // վ����λ��Ϣ
    HI_U8  sta_phase1           : 2;        // վ����λ��Ϣ
    HI_U8  sta_phase2           : 2;        // վ����λ��Ϣ
    HI_U8  proxy_type           : 2;        // �м̵�����

    HI_U8  ucNvCVer;                        // nv image.hupg: C Ver
    HI_U8  ucChipVerIdx;

    HI_U8  ucBootVerIdx;
    HI_U8  aucVer[3];                       // image.hupg

    HI_U32 ulYear               : 6;        // �ͻ��汾��-��
    HI_U32 ulMonth              : 4;        // �ͻ��汾��-��
    HI_U32 ulDay                : 5;        // �ͻ��汾��-��
    HI_U32 ulHour               : 5;
    HI_U32 ulMinute             : 6;
    HI_U32 ulSecond             : 6;

    HI_U16 usBVer;                          // code+nv image.hupg
    HI_U16 usBNvVer;                        // nv image.hupg: B Ver

    HI_U16 leave_cnt;                       // ���ߴ���
    HI_U16 proxy_change_cnt;                // ����������
    HI_U32 leave_total_time;                // ����ʱ��
    HI_U16 hw_reset_cnt;                    // ��¼�豸Ӳ��λ�ۻ�����
    HI_U16 mac_sw_reset_cnt;                // ��¼MAC��λ�ۻ�����
} NFC_DIAG_CMD_QUERY_NETWORK_TOPO_ENTRY_S_VER0;


//NDAͨ�������ϱ��ṹ����ÿ��վ���Ӧ�Ľṹ�壬44Bytes
typedef struct
{
    HI_U8 mac[6];                           // վ��MAC��ַ
    HI_U8 ucSpcVer;                         // code+nv SPC image.hupg
//    HI_U8 ucSpcNvVer;                       // nv image.hupg: SPC Ver
    HI_U8 ucSwVerLo;                        // �ͻ��汾�ŵ��ֽ�

    HI_U32 level                : 4;        // վ��㼶
    HI_U32 ability              : 4;        // վ���ɫ
    HI_U32 tei                  : 12;       // վ��TEI��ַ
    HI_U32 proxy_tei            : 12;       // վ�㸸����TEI��ַ

    HI_U32 join_time;                       // վ���������ʱ��

    HI_U8  product_type         : 3;        // ��Ʒ��̬
    HI_U8  in_use               : 1;        // վ���Ƿ�����
//    HI_U8  pad                  : 4;
    HI_U8  stru_ver             : 4;        // �ṹ�汾�ţ��˴�Ϊ1

    HI_U8  sta_phase0           : 2;        // վ����λ��Ϣ
    HI_U8  sta_phase1           : 2;        // վ����λ��Ϣ
    HI_U8  sta_phase2           : 2;        // վ����λ��Ϣ
    HI_U8  proxy_type           : 2;        // �м̵�����

//    HI_U8  ucNvCVer;                        // nv image.hupg: C Ver
    HI_U8  ucSwVerHi;                       // �ͻ��汾�Ÿ��ֽ�
    HI_U8  ucChipVerIdx;

    HI_U8  ucBootVerIdx;
    HI_U8  aucVer[3];                       // image.hupg

    HI_U32 ulYear               : 6;        // �ͻ��汾��-��
    HI_U32 ulMonth              : 4;        // �ͻ��汾��-��
    HI_U32 ulDay                : 5;        // �ͻ��汾��-��
    HI_U32 ulHour               : 5;
    HI_U32 ulMinute             : 6;
    HI_U32 ulSecond             : 6;

    HI_U16 usBVer;                          // code+nv image.hupg
//    HI_U16 usBNvVer;                        // nv image.hupg: B Ver
    HI_CHAR szManuCode[2];                  // ���̴���

    HI_U16 leave_cnt;                       // ���ߴ���
    HI_U16 proxy_change_cnt;                // ����������
    HI_U32 leave_total_time;                // ����ʱ��
    HI_U16 hw_reset_cnt;                    // ��¼�豸Ӳ��λ�ۻ�����
    HI_U16 mac_sw_reset_cnt;                // ��¼MAC��λ�ۻ�����
} NFC_DIAG_CMD_QUERY_NETWORK_TOPO_ENTRY_S;

//NDAͨ�������ϱ��ṹ��
typedef struct
{
    HI_U16 rsp_idx;                             //����ͨ��Ӧ���ı��
    HI_U16 total_num            : 10;           //��Ҫ�ϱ���վ������
    HI_U16 is_last              : 1;            //������������һ��Ӧ���ı�ʶ
    HI_U16 num                  : 5;            //��ǰ���ݰ�������վ����

    HI_U32 cur_req_send_idx     : 8;            //����ͨ����ǰ��������ı��
    HI_U32 pad                  : 24;           //

    NFC_DIAG_CMD_QUERY_NETWORK_TOPO_ENTRY_S entry[MAC_DIAG_CMD_QUERY_NETWORK_TOPO_ENTRY_NUM];
} NFC_DIAG_QUERY_NETWORK_TOPO_IND_S;

//�����ϱ�HSO�Ľӿ�
typedef struct _DIAG_CMD_QUERY_NETWORK_TOPO_STRU_
{
    HI_U16 sn;              //ÿ�������ϱ������кţ�����һ�β�ѯ�����Ӧ�Ķ���ϱ����ݰ��ĸ�ֵ��ͬ����ͬ��������ϱ�����ֵ��ͬ
    HI_U16 is_last  : 1;    //�����ϱ������һ�����ı�ʶ�����һ��������ֵΪ1�������һ��������ֵΪ0
    HI_U16 num      : 15;   //�����ݰ��а�������Чվ����

    NFC_DIAG_CMD_QUERY_NETWORK_TOPO_ENTRY_S entry[DFX_NETWORK_TOPO_ENTRY_MAX];
} DIAG_CMD_QUERY_NETWORK_TOPO_STRU;

//NDAͨ����ѯ��������Ĳ����ṹ
typedef struct
{
   HI_U8 aucTeiBitmap[256];             //TEI���˱�����

   HI_U32 CurReqSendIdx : 8;            //����ͨ����ǰ���������������
   HI_U32 pad           : 24;           //����

   HI_U32 ulSn;                         //һ��HSO����ı�ʶ����ͬ�����󣬸�ֵ��ͬ
} NFC_DIAG_QUERY_NETWORK_TOPO_REQ_S;

typedef struct
{
    HI_U16 usCurrentNum;
    HI_U16 reserved;
    HI_U16 sn;

    HI_U16 usTotalNum   : 10;
    HI_U16 is_last      : 1;
    HI_U16 num          : 5;

    NFC_DIAG_CMD_QUERY_NETWORK_TOPO_ENTRY_S astEntry[0];
}MAC_NDC_PKFC_NETWORK_TOPO_IND_S;


typedef struct
{
    HI_U8 mac[6];
    HI_U16 pad;
    HI_U32 proxy_change_count;
    HI_U32 offline_count;
} DIAG_CMD_QUERY_ALL_STA_INFO_ENTRY_STRU;

typedef struct _DIAG_CMD_QUERY_ALL_STA_INFO_STRU_
{
    HI_U16 sn;
    HI_U16 is_last;
    HI_U32 num;
    DIAG_CMD_QUERY_ALL_STA_INFO_ENTRY_STRU entry[DFX_NETWORK_TOPO_ENTRY_MAX];
} DIAG_CMD_QUERY_ALL_STA_INFO_STRU;


/**
 *
 * 2.3    ͷ�ˣ�������Լ�Э����������Ϣ.
 *
 */
typedef struct
{
    HI_U32 snid;
    HI_U8 is_single_comm;     //�Ƿ�ͨ
    HI_U8 rsv[3];

    HI_U32 bandwidth;
} DIAG_CMD_QUERY_NEIGHBOR_NETWORK_ENTRY_STRU;

typedef struct _DIAG_CMD_QUERY_NEIGHBOR_NETWORK_STRU_
{
   HI_U16 num;
   HI_U16 pad;
   DIAG_CMD_QUERY_NEIGHBOR_NETWORK_ENTRY_STRU entry[MAX_NEIGHBOR_SUM];
} DIAG_CMD_QUERY_NEIGHBOR_NETWORK_STRU;

/**
 *
 * 2.4    �նˣ�(��������Ĵ���) ����ʧ�ܵĴ���
 *
 */
typedef struct _DIAG_CMD_QUERY_REQUEST_JOIN_FAILED_COUNT_STRU_
{
   HI_U32 request_join_failed_count;
} DIAG_CMD_QUERY_REQUEST_JOIN_FAILED_COUNT_STRU;

/**
 *
 * 2.5    �նˣ�������ʱ���ӷ���������󵽳ɹ�������
 *
 */
typedef struct _DIAG_CMD_QUERY_JOIN_NETWORK_EXPIRED_STRU_
{
   HI_U32 expire;
} DIAG_CMD_QUERY_JOIN_NETWORK_EXPIRED_STRU;

/**
 *
 * 2.6    �նˣ���������Ķ���STA
 *
 */
typedef struct
{
    HI_U8 mac[6];
    HI_U16 tei;
} DIAG_CMD_QUERY_ASSOCIATED_STAS_ENTRY_STRU;

//վ��Ĺ���������Ϣ
typedef struct _DIAG_CMD_QUERY_ASSOCIATED_STAS_STRU_
{
    HI_U16 num;
    HI_U16 rsvd;
    DIAG_CMD_QUERY_ASSOCIATED_STAS_ENTRY_STRU entry[DFX_ASSOCIATED_STAS_ENTRY_MAX];
} DIAG_CMD_QUERY_ASSOCIATED_STAS_STRU;

typedef struct
{
    HI_U16 assoc_req_cnt;                   // �����������
    HI_U16 proxy_tei;                       // ����ѡ��Ĵ���

    HI_U32  snid;                            // ����SNID��

    HI_U8  freq;                            // Ƶ��
    HI_U8  network_form_serial_number;      // �������к�
    HI_U8  pad[2];

    HI_U32 join_time;                       // ��������ʱ��
}DIAG_CMD_ASSOC_ENTRY_INFO_STRU;

#define NM_ASSOC_DFX_INFO_NUM       (10)
typedef struct
{
    HI_U32 index;
    DIAG_CMD_ASSOC_ENTRY_INFO_STRU asso_info[NM_ASSOC_DFX_INFO_NUM];
}DIAG_CMD_ASSOC_INFO_STRU;

#define NM_ASSOC_DFX_RCV_BEACON  (1)
#define NM_ASSOC_DFX_RCV_REFUSE  (2)
typedef struct
{
    HI_U32  snid;            // �����

    HI_U8  freq;            // Ƶ�κ�
    HI_U8  status;          // ״̬
    HI_U8  req_cnt;         // ��������
    HI_U8  pad;

    HI_U32 starttime;
}DIAG_CMD_MULTI_NETWORK_ASSOC_INFO_STRU;

#define NM_MULTI_NETWORK_ASSOC_INFO_NUM     (18)
typedef struct
{
    HI_U32 index;
    DIAG_CMD_MULTI_NETWORK_ASSOC_INFO_STRU multi_network_assoc_entry[NM_MULTI_NETWORK_ASSOC_INFO_NUM];
}DIAG_CMD_MULTI_NETWORK_ASSOC_DFX_STRU;

typedef struct
{
    HI_U8  cco_mac[6];              // CCO��MAC��ַ
    HI_U8  pad;
    HI_U8  network_serial_num;      // �������к�
    HI_U32  snid;                    // �����

    HI_U32 change_time;             // ���ʱ��
}DIAG_CMD_SERIAL_NUM_CHANGE_ENTRY_STRU;
#define NM_SERIAL_NUM_CHANGE_INFO_NUM   (10)
typedef struct
{
    HI_U32 index;
    DIAG_CMD_SERIAL_NUM_CHANGE_ENTRY_STRU serial_num_change_entry[NM_SERIAL_NUM_CHANGE_INFO_NUM];
}DIAG_CMD_SERIAL_NUM_CHANGE_DFX_STRU;
/**
 *
 * 2.7    �նˣ������б�
 *
 */

typedef struct
{
    HI_U32 channel_info;                        // �ŵ�����
    HI_U32 stat_cnt;                            // ���ո���
}rt_channel_stat_st2;

typedef struct
{
    HI_U32 comm_rate;
    HI_U32 channel_info;
    HI_U32 down_comm_rate;
}rt_proxy_comm_st2;

typedef struct
{
    HI_U16 tei;
    HI_U16 rsvd2;
    HI_U8  mac[HI_PLC_MAC_ADDR_LEN];                 // վ��MAC��ַ
    HI_U8  role;                                     // վ���ɫ
    HI_U8  level;                                    // վ��㼶

    HI_U8  phase;                                    // վ����λ
    HI_U8  tm_capacity;                              // վ�㸺������
    HI_U8  up_channel_update_flag;
    HI_U8  rsvd1;                                 // sounding����Ƿ����

    rt_channel_stat_st2 last_down_channel_stat;      // �ϴ�����ͨ����ͳ��
    rt_channel_stat_st2 last_up_channel_stat;        // �ϴ�����ͨ����ͳ��
    rt_channel_stat_st2 cur_down_channel_stat;       // ��ǰ����ͨ����ͳ��
    HI_U8 comm_rate;                                 // ������ͨ����
    HI_U8 down_comm_rate;                            // ����ͨ�ųɹ���
    HI_U8 up_comm_rate;                              // ����ͨ����
    HI_U8 proxy_proxy_comm_rate;                     // ������ͨ����
    HI_U32 send_pk_cnt;                              // ���з������ݸ���

    HI_U32 send_fail_cnt;                           // ����ʧ�ܴ���
    HI_U32 send_success_cnt;                        // ���ͳɹ�����
    HI_U8  level_relation;                          // �㼶��ϵ
    HI_U8  rsvd;
    HI_U16 proxy_tei;                               // ������TEI
    rt_proxy_comm_st2 proxy_channel_stat;           // ����������ͨ������Ϣ

    HI_U32 rcv_beacon_cnt;                          // ���ո�վ��BEACON֡�ĸ���
    HI_U8  channel_info;                            // �ŵ�����(beacon֡����)
    HI_U8  par;                                     // �����(SOF֡)
    HI_S16 snr;                                     // ƽ�������(SOF֡)
}DIAG_CMD_QUERY_DISCOVER_LIST_ENTRY_STRU;

typedef struct
{
   HI_U16 sn;
   HI_U16 is_last;
   HI_U16 num;
   HI_U16 pad;
   DIAG_CMD_QUERY_DISCOVER_LIST_ENTRY_STRU entry[DFX_DISCOVER_LIST_ENTRY_MAX];
} DIAG_CMD_QUERY_DISCOVER_LIST_STRU;

//����ķ����б�ʵ��
typedef struct
{
    HI_U32 tei                      : 12;
    HI_U32 proxy_tei                : 12;//������TEI
    HI_U32 role                     : 4;//վ���ɫ  0~15
    HI_U32 level                    : 4;//վ��㼶  0~15

    //���Լ���ó�
    //HI_U32  comm_rate                : 7;//������ͨ����,comm_rate = down_comm_rate * up_comm_rate / 100,��HSO������ʾ
    HI_U32 down_comm_rate           : 7;//����ͨ�ųɹ���
    HI_U32 up_comm_rate             : 7;//����ͨ����
    HI_U32 proxy_proxy_comm_rate    : 7;//������ͨ����
    HI_U32 level_relation           : 2;//�㼶��ϵ 0~3
    HI_U32 pad1                     : 9;

    HI_U32 send_fail_cnt;              //����ʧ�ܴ���
    HI_U32 send_success_cnt;           //���ͳɹ�����

    HI_U8  channel_info;                //�ŵ�����(beacon֡����)
    HI_U8  par;                         //�����(SOF֡)
    HI_S16 snr                      : 12;//ƽ�������(SOF֡)
    HI_S16 pad2                     : 4;
}DIAG_CMD_QUERY_DISCOVER_LIST_ENTRY_SM_STRU;

//����ķ����б���ϱ�HSO��ʵ��
typedef struct
{
    HI_U16 sn;
    HI_U8  is_last      : 1;
    HI_U8  pad          : 7;
    HI_U8  num;
    DIAG_CMD_QUERY_DISCOVER_LIST_ENTRY_SM_STRU entry[DFX_DISCOVER_LIST_ENTRY_SM_MAX];
}DIAG_CMD_QUERY_DISCOVER_LIST_SM_STRU;


/**
 *
 * 2.8    վ��·�ɱ���Ϣ
 *
 */
typedef struct
{
   HI_U16 odtei;
   HI_U16 next_hop0;
   HI_U16 next_hop1;
   HI_U16 next_hop2;
   HI_U16 next_hop3;
   HI_U16 rsvd;
} DIAG_CMD_QUERY_ROUTER_TABLE_ENTRY_STRU;

typedef struct
{
    HI_U16 sn;
    HI_U16 is_last;
    HI_U16 num;
    HI_U16 pad;
    DIAG_CMD_QUERY_ROUTER_TABLE_ENTRY_STRU entry[DFX_ROUTER_TABLE_ENTRY_MAX];
} DIAG_CMD_QUERY_ROUTER_TABLE_STRU;

typedef struct
{
    HI_U8 bJoin;
    HI_U8 snid;
    HI_U8 ccoMacAddr[6];
}DIAG_CMD_QUERY_DFX_NDM_NM_ATTR_STRU;

typedef struct
{
    HI_U16 usExceptedReplyCount; // ���ֹ����м̸���
    HI_U16 usExceptedMeterCount; // ���ֹ����ն˸���

    HI_U16 ulFirstFormedNetElapsed; // ��һ������ʱ��
    HI_U16 pad;

    HI_U32 ulLastFormedNetElapsed; // ���һ������ʱ��
    HI_U32 ulReFormedNetCount; // ������������

}DIAG_CMD_QUERY_NETWORK_STATE_INFO_CCO_ENTRY_S;

typedef struct
{
    HI_U32 ulAssocedFailedCount; // �������ʧ�ܴ���
    HI_U32 ulAssocedSuccedCount; // ��������ɹ�����
    HI_U32 ulAssocedLastSuccedExpire; // ���һ�γɹ�������ʱ
}DIAG_CMD_QUERY_NETWORK_STATE_INFO_STA_ENTRY_S;

typedef struct
{
    DIAG_CMD_QUERY_NETWORK_STATE_INFO_CCO_ENTRY_S ccoEntry;
    DIAG_CMD_QUERY_NETWORK_STATE_INFO_STA_ENTRY_S staEntry;
}DIAG_CMD_QUERY_NETWORK_STATE_INFO_STRU;

#define DIAG_CMD_QUERY_DEVICE_FREQ_LIST_SIZE 16
#define DIAG_CMD_QUERY_DEVICE_FREQ_INFO_LIST_SIZE   (6)

typedef struct
{
    HI_CHAR list[DIAG_CMD_QUERY_DEVICE_FREQ_LIST_SIZE];
}DIAG_CMD_QUERY_DEVICE_FREQ_INFO_LIST_ENTRY;

typedef struct
{
    HI_U32  MaxFreqNum;                                 //Ƶ������
    HI_U32  FreqChangeNum;                              //Ƶ���л�����

    HI_CHAR CurrFreq[DIAG_CMD_QUERY_DEVICE_FREQ_LIST_SIZE];   //��ǰ��Ƶ��
    DIAG_CMD_QUERY_DEVICE_FREQ_INFO_LIST_ENTRY FreqList[DIAG_CMD_QUERY_DEVICE_FREQ_INFO_LIST_SIZE];
    DIAG_CMD_QUERY_DEVICE_FREQ_INFO_LIST_ENTRY FreqTotalList[DIAG_CMD_QUERY_DEVICE_FREQ_INFO_LIST_SIZE];
}DIAG_CMD_QUERY_DEVICE_FREQ_INFO_STRU;

typedef struct
{
    HI_U32    enAccessMode;

    HI_U8       ucFreq;
    HI_U8       ucSnid;
    HI_U8       ucMac[HI_PLC_MAC_ADDR_LEN];

    HI_U8       ucAction;
    HI_U8       reserved[3];
} DIAG_CMD_NDC_HSO_REQ_S;

typedef struct
{
    HI_BOOL                 SetNdm;         // hi_true ����NDM�豸
    HI_U8                   ucRsvd;
    HI_U8                   DevId[6];
    DIAG_CMD_NDC_HSO_REQ_S  stHsoRequest;
}DIAG_CMD_NDC_TEST_STRU;

#if defined(PRODUCT_CFG_PRODUCT_TYPE_NDM) || defined(PRODUCT_CFG_HSO)
typedef struct
{
    HI_U8 attenuate_flag;       // ˥�����
    HI_U8 pad[3];
}DIAG_CMD_NDM_CTRL_ATTENUATE_STRU;
typedef struct
{
    HI_U32 ulRc;               // ����� �ɹ��������HI_ERR_SUCCESS��OTHER VALUE fail
}DIAG_CMD_NDM_CTRL_ATTENUATE_ACK_STRU;
#endif

typedef struct
{
    HI_U8  mac[HI_PLC_MAC_ADDR_LEN];                 // վ��MAC��ַ
    HI_U16 proxy_tei;                                // ������TEI

    HI_U32 role : 4;                                 // վ���ɫ
    HI_U32 level : 4;                                // վ��㼶
    HI_U32 min_multiproxy_comm_rate : 7;             // ·����Сͨ����
    HI_U32 proxy_comm_rate : 7;			             // �����վ�������ͨ����
    HI_U32 proxy_down_comm_rate : 7;		         // �����վ�������ͨ����
    HI_U32 network_mode : 2;                         // ����ģʽ:0.PLCģʽ��1.RFģʽ��2.SCģʽ
    HI_U32 is_dual_module : 1;                       // �Ƿ�Ϊ˫ģģ��:0.��ģģ�飬1.˫ģģ��

    HI_U8  comm_rate;                                // ������ͨ����
    HI_U8  down_comm_rate;                           // ����ͨ�ųɹ���
    HI_U8  up_comm_rate;			                 // ����ͨ�ųɹ���
    HI_U8  proxy_proxy_comm_rate;                    // ·��ͨ����

    HI_U16 cur_down_rcv_cnt;		                 // ��ǰ�������н��ո���(����ǰͳ�ƽ����ű�֡�����ܻᳬ��255����˲���16λ)
    HI_S16 snr;                                      // ƽ�������(SOF֡)

    HI_U8  channel_info;                             // �ŵ�����(beacon֡����)
    HI_U8  is_exist : 1;                             // ͨ������·�ɴ���ͳ�����ݣ�ȷ����վ���Ƿ�Ϊ��վ�㷢��վ��
    HI_U8  is_multi_freq_avail : 1;					 // ��ƵƵ���Ƿ����
	HI_U8  multi_freq : 5;							 // ��ƵƵ��
    HI_U8  is_in_use : 1;                            // �û���λ���Ƿ�ʹ�á�

    HI_U8  proxy_channel_info;                       // �����վ����ŵ�����
    HI_U8  down_rcv_flag : 1;                        // ���н��ձ�־�����ڿ���·�ɱ��
    HI_U8  up_channel_update_flag : 1;               // ����ͨ�����Ƿ����
    HI_U8  proxy_calc_ok : 1;			             // �Ƿ����������վ���ͨ����
    HI_U8  level_relation : 2;                       // �㼶��ϵ
    HI_U8  phase : 2;                                // վ����λ
    HI_U8  associate_flag : 1;                       // ������־��0.δ������1.�ѹ���

    HI_U8  not_in_use_time;                          // ·����Ϣ���ϻ�����
    HI_U8  snd_channel_info;                         // ��ȡվ�㷢������ʱ���ŵ�����(����)
    HI_U8  rssi;                                     // �����ź�ǿ��:RF��·�ź�ǿ�ȡ�PLC�ŵ�˥����SC�ź�ǿ��
    HI_U8  up_rssi;                                  // �����ź�ǿ��:RF��·�ź�ǿ�ȡ�PLC�ŵ�˥����SC�ź�ǿ��

    HI_U8  down_rcv_cnt[4];	     // �������н��ո���
    HI_U8  up_rcv_cnt[4];	         // �������н��ո���,��վ����ձ�վ�㷢�͵ı�����
    HI_U8  down_send_cnt[4];	     // �������з��͸���,��վ�㷢�͵ı�����

    HI_U32 send_fail_cnt;                            // ����ʧ�ܴ���
    HI_U32 send_success_cnt;                         // ���ͳɹ�����
    HI_U32 rcv_beacon_cnt;                           // ���ո�վ��BEACON֡�ĸ���
} DIAG_CMD_QUERY_CHOOSE_DISLIST;

typedef struct
{
    HI_U16 actual_index;    //վ��ʵ�ʴ洢λ��
    HI_U8 path_tablae_bitmap[DFX_PATH_TABLE_BITMAP_LEN]; //·����Ϣ��bitͼ

    HI_U16 exist_node;                          //�Ѿ����ڵ�վ����Ŀ
    HI_U16 pad;
} DIAG_CMD_QUERY_PATH_TABLE_REFERENCE;

typedef struct
{
    HI_BOOL cco_is_in_use;          //cco�洢λ���Ƿ�ʹ�á�
    HI_U8 pad;
    HI_U16 pk_filter_count;

    HI_U32 pk_filter_pos;       //teiʵ�ʴ洢λ��

    HI_U8 pk_filter_bitmap[DFX_PATH_TABLE_BITMAP_LEN];
}DIAG_CMD_QUERY_PK_FILTER;

typedef struct
{
    HI_U16  tei;                            // վ��TEI
    HI_U16  proxy_tei;                      // ����վ���TEI
    HI_U16  childsum;                       // �ӽڵ�ĸ���
    HI_U8   mac[HI_PLC_MAC_ADDR_LEN];       // MAC��ַ
    HI_U8   level;                          // �㼶
    HI_U8   ability;                        // �ο� IS_CCO �궨��
    HI_U8   heartbeat_count;                // �������� , ���� heartbeat_count ���ж�,���������������,ֻҪ��һ����������Ϊû������
    HI_U8   sta_phase[3];
    HI_BOOL new_joiner;                     // �Ƿ�Ϊ���ָռ����վ��,�������ʱ��������վ��,��������������ߴ�����ж���
    HI_U8   product_type;                   // ��Ʒ������,�ο� HI_ND_TYPE_CCO , HI_ND_TYPE_RELAY
    HI_U8   not_rcv_heartbeat_continuous;   // ����û���յ��������ĵļ�����ڼ���
    HI_BOOL in_use;                         // վ���Ƿ����ߵı�־
    HI_BOOL is_relay_ability;               // �Ƿ�Ϊ�м�
    HI_U8   d2d_proxy_comm_rate;            // ·��ͨ����
    HI_U8   proxy_type;                     // ��������
    HI_U8   freq_mode_present;              // ��ǰ��Ƶ�����
    HI_U8   nm_mmv;                         // ��������İ汾��
    HI_U8   down_comm_rate;                 // ����ͨ�ųɹ���
    HI_U8   up_comm_rate;                   // ����ͨ�ųɹ���
    HI_U8   is_support_ared_recognition : 1;// �Ƿ�֧��̨��ʶ����
    HI_U8   is_dual_module : 1;             // �Ƿ�Ϊ˫ģģ��
    HI_U8   network_mode : 1;               // ����ģʽ
    HI_U8   pad : 4;
    HI_U8   power_ctrl_flag :1;               //�Ƿ��ǹ��ʿ��ƻ���վ��
    HI_U8   is_notify_leave;                // �Ƿ�֪ͨ����
    HI_U8   notify_leave_cnt;               // ֪ͨ����
    HI_SYS_STATUS_INFO_S sta_ver;           // �汾��Ϣ
    HI_U32  addtime;                        // վ���������ʱ��
    HI_U32  assoc_random;                   // �������������,�ο�ҵ�񱳾�˵��-2
    HI_U32  proxy_change_cnt;               // ����������
    HI_U32  leave_cnt;                      // ���ߴ���
    HI_U32  leave_time;                     // ����ʱ��
    HI_U32  leave_total_time;               // ������ʱ��
    HI_U16  hw_reset_cnt;                   // ��¼�豸Ӳ��λ�ۻ�����
    HI_U16  mac_sw_reset_cnt;               // ��¼MAC��λ�ۻ�����
    HI_U8   leave_reason;                   // ����ԭ��
    HI_U8   hw_reset_reason;                // Ӳ����ԭ��
    HI_U8   sw_reset_reason;                // ������ԭ��
    HI_U8   pad1;
    HI_U32  last_heartbeat_time;            // ���һ������ʱ��
    HI_U8    management_id[24];
}DIAG_CMD_QUERY_CHOOSE_TOPOINFO;
typedef struct
{
    HI_U32 tei;
}DIAG_CMD_QUERY_CHOOSE_REQ_PARA_STRU;
#define NM_MAX_BACKUP_MAC_CNT       (32)
typedef struct
{
    HI_U8  mac[HI_PLC_MAC_ADDR_LEN];
    HI_U8  pad[2];// ���
} HI_MAC_STRU;
typedef struct
{
    HI_U8  index;                                       // ��ǰʹ��MAC����
    HI_U8  total_cnt;                                   // MAC����
    HI_U8  clear_cnt;                                   // ���MAC��ַ����
    HI_U8  set_cnt;                                     // ���ñ���MAC����
    HI_MAC_STRU mac_list[NM_MAX_BACKUP_MAC_CNT];
}DIAG_CMD_QUERY_BACKUP_MAC_STRU;
//*****************************************************************************
// MAC: ����·������� SNID������ STA��CCO��Ϊ��ͬ�㣿
// CMD ID : ID_DIAG_CMD_CCO_MAC_MULTYNET_SET
// REQ STRU:DIAG_CMD_CCO_MAC_MULTYNET_SET_REQ_STRU
// ACk STRU:DIAG_CMD_CCO_MAC_MULTYNET_SET_ACK_STRU
//*****************************************************************************
// REQ
#define MAC_MUTYNET_OP_VA_SET_SNID_ONLY     (1) // ����SNID�� ONLY
#define MAC_MUTYNET_OP_VA_SET_MUTY_SWT_ONLY (2) // ���ö����翪�� ONLY
#define MAC_MUTYNET_OP_VA_SET_NORMAL        (3) // ȫ����

typedef struct
{
    HI_U32 ulSnid;   // V100 :�Ϸ�ֵ: [1,15]  �Ƿ�ֵ: 0xff����ֵʱ������
    HI_U8 ucIsMultyOpen;        // TRUE��������� FALSE: ������ر� 0xFF: ��Ч����ֵʱ������
    HI_U8 ucMutyOpVa;           // �����룺1����enSnid��2����ucIsMultyOpen��3ȫ����
    HI_U8 ucRsv[2];
} DIAG_CMD_CCO_MAC_MULTYNET_SET_REQ_STRU;

// ACK
typedef struct
{
    HI_U32 ulRet;// OK HI_ERR_SUCCESS; OTHER FAIL;
} DIAG_CMD_CCO_MAC_MULTYNET_SET_ACK_STRU;

//*****************************************************************************
// MAC: ��������ز�ѯ����ѯSNID������״̬��STA��CCO��Ϊ��ͬ�㣿
// CMD ID : ID_DIAG_CMD_CCO_MAC_MULTYNET_QUERY
// REQ STRU:NULL
// ACk STRU:DIAG_CMD_CCO_MAC_MULTYNET_QUERY_ACK_STRU
//*****************************************************************************

// REQ
// NULL

// ACK
typedef struct
{
    HI_U32 ulSnid;               // V100:�Ϸ�ֵ: [1,6]  �Ƿ�ֵ: 0xff
    HI_U8  ucIsMultyOpen;        // TRUE��������� FALSE: ������ر� 0xFF: ��Ч
    HI_U8  ucRsv[3];
} DIAG_CMD_CCO_MAC_MULTYNET_QUERY_ACK_STRU;

//*****************************************************************************
// MAC: Ƶ������ STA: ��֧�����ã�CCO:֧��
// CMD ID : ID_DIAG_CMD_MAC_FREQ_SET
// REQ STRU:DIAG_CMD_MAC_FREQ_SET_REQ_STRU
// ACk STRU:DIAG_CMD_MAC_FREQ_SET_ACK_STRU
//*****************************************************************************

// REQ STRU
typedef enum _EN_MAC_FREQ_
{
    EN_MAC_FREQ_0 = 0,
    EN_MAC_FREQ_1,
    EN_MAC_FREQ_2,
    EN_MAC_FREQ_3,
    EN_MAC_FREQ_4,
    EN_MAC_FREQ_5,
    EN_MAC_FREQ_MAX,
    EN_MAC_FREQ_INVALID = 0xff
} EN_MAC_FREQ;

typedef struct
{
    // ȫƵ 0;��Ƶ1;��Ƶ2 ��Чֵ:0xff
    EN_MAC_FREQ enFreq;
    HI_U32 ulFreqSwtTime; // ��λ�룬Ƶ���л�ʱ��(ϵͳĬ��������) 0Ϊ�Ƿ�������
} DIAG_CMD_MAC_FREQ_SET_REQ_STRU;

// ACK STRU
// ͬ����������
typedef struct
{
    HI_U32 ulRc;          // ����� �ɹ����÷��� HI_ERR_SUCCESS OTHER VALUE fail
    EN_MAC_FREQ enFreq;   // ȫƵ 0;��Ƶ1;��Ƶ2 ��Чֵ:0xff
    HI_U32 ulFreqSwtTime; // ��λ��s�� Ƶ���л�ʱ��(ϵͳĬ��������,120s)
} DIAG_CMD_MAC_FREQ_SET_ACK_STRU;

//*****************************************************************************
// MAC: Ƶ�β�ѯ CCO AND STA:���Բ�ѯ��ǰƵ�Σ������л���Ƶ�Σ��л�ʣ��ʱ��
// CMD ID : ID_DIAG_CMD_MAC_FREQ_QUERY
// REQ STRU:NULL
// ACk STRU:DIAG_CMD_MAC_FREQ_QUERY_ACK_STRU
//*****************************************************************************

// REQ STRU
// NULL

// ACK STRU
typedef struct
{
    EN_MAC_FREQ enCurFreq;    // ��ǰƵ��0xff�Ƿ�
    EN_MAC_FREQ enSwt2Freq;   // 0xff�Ƿ������л��вŻ᷵��
    HI_U8  isSwtching;        // �Ƿ������л���HI_TRUE�����л���HI_FALSEδ���л�
    HI_U8  blink_freq_seq;
    HI_U8  rsv[2];
    HI_U32 ulFreqSwtTime;     // ��λΪ����С��λ�룬isSwtching���ΪTRUE����Ҫ����л����
} DIAG_CMD_MAC_FREQ_QUERY_ACK_STRU;


//*****************************************************************************
// MAC: �������Ƿ��������ã�������ȥCCO��������������50%��վ����ߣ�CCO��������
//      �Ƿ�ѱ������ڰ�����������
// CMD ID : ID_DIAG_CMD_CCO_MAC_MULTYNET_REJECT_LIST_CLR
// REQ STRU:NULL
// ACk STRU:DIAG_CMD_CCO_MAC_MULTYNET_REJECT_LIST_CLR_ACK_STRU
//*****************************************************************************

// REQ STRU

// ��BIT�����ã������ڱ����٣���ʵ��
#define MAC_DFX_WHITE_SET_NORMAL      (3) // isWhiteOpen��isMtFileInWhite�����ã�HSOֻʹ�ø�ֵ
#define MAC_DFX_WHITE_SET_WHITE_ONLY  (1) // ֻ����isWhiteOpen
#define MAC_DFX_WHITE_SET_MTFILE_ONLY (2) // ֻ����isMtFileInWhite

typedef struct
{
    HI_U8 ucOperVa;            // �����룺MAC_DFX_WHITE_SET_NOMAR etc,HSO��ǰֻ�·�MAC_DFX_WHITE_SET_NORMAL,��ʹ�ø�ֵ
    HI_BOOL isWhiteOpen;       // TRUE ���� FALSEδ����
    HI_U8 mode;                // ��������ģʽ
    HI_U8 rsv;
} DIAG_CMD_CCO_MAC_WHITE_STA_SET_REQ_STRU;

// ACK STRU
typedef struct
{
    HI_U32 ulRc;               // ����� �ɹ����÷���HI_ERR_SUCCESS��OTHER VALUE fail
} DIAG_CMD_CCO_MAC_WHITE_STA_SET_ACK_STRU;


//*****************************************************************************
// MAC: �ڰ�����״̬��ѯ �������Ƿ�����״̬��ѯ���Ƿ�ѱ������ڰ�����
// CMD ID : ID_DIAG_CMD_CCO_MAC_WHITE_STA_QUERY
// REQ STRU:NULL
// ACk STRU:DIAG_CMD_CCO_MAC_WHITE_STA_QUERY_ACK_STRU
//*****************************************************************************

// REQ STRU
// NULL

// ACK STRU
typedef struct
{
    HI_U8 isWhiteOpen;         // NV�����Ƿ�򿪰����� HI_TRUE ��; HI_FALSE ����;
    HI_U8 mode;                // ������ģʽ
    HI_U8 rsv1;                // ʵ��������MAC��֤�Ƿ�� HI_TRUE ��; HI_FALSE ����;HSOδʹ��
    HI_U8 rsv;
} DIAG_CMD_CCO_MAC_WHITE_STA_QUERY_ACK_STRU;

//*****************************************************************************
// MAC: ���MAC��صĶ���������CCO�ܾ��б��
// CMD ID : ID_DIAG_CMD_CCO_MAC_CLR
// REQ STRU: DIAG_CMD_CCO_MAC_CLR_REQ_STRU
// ACk STRU: DIAG_CMD_CCO_MAC_CLR_ACK_STRU
//*****************************************************************************

// REQ STRU
typedef enum _EN_MAC_CLR_OPER_
{
    EN_MAC_CLR_CCO_REJECT_LIST = 1,// ���CCO������ܾ��б�
    EN_MAC_CLR_INVALID = 0xff
} EN_MAC_CLR_OPER;

typedef struct
{
    EN_MAC_CLR_OPER enClrOp;  // ����
} DIAG_CMD_CCO_MAC_CLR_REQ_STRU;

// ACK STRU
typedef struct
{
    HI_U32 ulRc;               // ����� �ɹ��������HI_ERR_SUCCESS��OTHER VALUE fail
} DIAG_CMD_CCO_MAC_CLR_REQ_STRU_ACK_STRU;


typedef struct
{
    HI_U16 proxy_num;               // ��ǰ������Ŀ
    HI_U16 pad;
} DIAG_CMD_PROXY_NUM_REQ_STRU;

typedef struct
{
    HI_U16 child_sum;               // ĳվ�����վ����
    HI_U16 pad;
} DIAG_CMD_CHILD_SUM_ACK_STRU;

typedef struct
{
    HI_U16 tei;                     // ��ѯ��վ��
    HI_U16 pad;
} DIAG_CMD_CHILD_SUM_REQ_STRU;


typedef struct
{
    HI_U32 create_cnt;              // �������ĸ���
    HI_U32 rcv_cnt;                 // ���ձ��ĸ���
    HI_U32 last_create_time;        // �ϴ����ɱ���ʱ��
    HI_U32 last_rcv_time;           // �ϴν��ձ���ʱ��
}nm_packet_info;

// ��¼������Ϣ��������
#define MAX_RCV_PACKET_INFO  20

typedef struct
{
    HI_U8 req_mac[HI_PLC_MAC_ADDR_LEN];
    HI_U16 pad;
    HI_U32 rcv_time;
}nm_rcv_assoc_object_info;

typedef struct
{
    HI_U8 index;
    HI_U8 pad[3];
    nm_rcv_assoc_object_info pk_info[MAX_RCV_PACKET_INFO];
}nm_rcv_assoc_packet_info;

typedef struct
{
    #if defined(PRODUCT_CFG_PRODUCT_TYPE_STA) || defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    nm_packet_info assoc_req_info;          // ������������Ϣ
    nm_packet_info assoc_cnf_info;          // �����ظ�������Ϣ
    nm_packet_info assoc_ind_info;          // ����ȷ�ϱ�����Ϣ
    nm_packet_info assoc_gather_ind_info;   // �������ܱ�����Ϣ
    nm_packet_info change_proxy_req_info;   // ������������Ϣ
    nm_packet_info change_proxy_cnf_info;   // ������ȷ����Ϣ

    nm_rcv_assoc_packet_info rcv_assoc_req_info;    // ���յ����������ĵ���Ϣ
    nm_rcv_assoc_packet_info rcv_assoc_cnf_info;    // ���յ������ظ����ĵ���Ϣ
    #endif

    #if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    HI_U32 rcv_heartbeat_pk_cnt;                    // �����������ĸ���
    HI_U32 heartbeat_cycle_cnt;                     // �������ڼ���
    #endif

    #if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
    HI_U32 create_heartbeat_by_timer_cnt;           // ���ڶ�ʱ�����������������ĸ���
    HI_U32 create_heartbeat_by_rcv_cnt;             // ���ڽ����������Ĵ��������������ĸ���
    #endif

    HI_U32 creat_discovery_list_cnt;                //���������б�����
    HI_U32 rcv_discovery_list_cnt;                  //���շ����б�����

    HI_U32 send_msdu_success_cnt;                   //���ͱ��ĵ���۳ɹ��Ĵ���
    HI_U32 send_msdu_fail_cnt;                      //���ͱ��ĵ����ʧ�ܵĴ���

    HI_U32 rcv_msdu_cnt;                    //���յ������ܱ��ļ���
    HI_U32 not_support_msdu_cnt;            //��֧�ִ���ı��ļ���
}nm_packet_stat_info_st;    //��ӦHSO��ѯ�ṹ��DIAG_CMD_NM_PACKET_STAT_STRU

typedef struct
{
    nm_rcv_assoc_packet_info rcv_assoc_req_info;    // ���յ����������ĵ���Ϣ
}DIAG_CMD_NM_ASSOC_REQ_STAT_STRU;

typedef struct
{
    nm_rcv_assoc_packet_info rcv_assoc_cnf_info;    // ���յ������ظ����ĵ���Ϣ
}DIAG_CMD_NM_ASSOC_CNF_STAT_STRU;


typedef struct
{
    nm_packet_info assoc_req_info;          // ������������Ϣ
    nm_packet_info assoc_cnf_info;          // �����ظ�������Ϣ
    nm_packet_info assoc_ind_info;          // ����ȷ�ϱ�����Ϣ
    nm_packet_info change_proxy_req_info;   // ������������Ϣ
    nm_packet_info change_proxy_cnf_info;   // ������ȷ����Ϣ
    nm_packet_info assoc_gather_ind_info;   // �������ܱ�����Ϣ

    HI_U32 creat_discovery_list_cnt;        // ���������б�����
    HI_U32 rcv_discovery_list_cnt;          // ���շ����б�����

    HI_U32 rcv_heartbeat_pk_cnt;            // �����������ĸ���
    HI_U32 heartbeat_cycle_cnt;             // �������ڼ���
    HI_U32 create_heartbeat_by_timer_cnt;   // ���ڶ�ʱ�����������������ĸ���
    HI_U32 create_heartbeat_by_rcv_cnt;     // ���ڽ����������Ĵ��������������ĸ���

    HI_U32 send_msdu_success_cnt;                   //���ͱ��ĵ���۳ɹ��Ĵ���
    HI_U32 send_msdu_fail_cnt;                      //���ͱ��ĵ����ʧ�ܵĴ���

    HI_U32 rcv_msdu_cnt;                    // ���յ������ܱ��ļ���
    HI_U32 not_support_msdu_cnt;            // ��֧�ִ���ı��ļ���
} DIAG_CMD_NM_PACKET_STAT_STRU;


typedef struct
{
    HI_BOOL     is_tried;                   // ��ǰ�ִ��Ƿ��Թ���SNID
    HI_U8       state;                      // 0��ʾ���ж�
    HI_S16      SNR;
    HI_U32      reassoc_time;               // �´ο��Թ���ʱ��
    HI_U32      rcv_fc_cnt;                 // �յ�FC�ĸ���
    HI_U32      refuse_duration;            // �ܾ�ʱ��
    HI_U32      chose_cnt : 28;             // ѡ�񵽴�SNID�Ĵ���
    HI_U32      main_network_snid : 4;      // ������SNID��
}sta_snid_stat_info;

typedef struct
{
    sta_snid_stat_info sta_snid_info[15];   // sta����������Ϣ
}DIAG_CMD_STA_SNID_INFO_STRU;

#define DIAG_SNR_LEVEL_NUM              (16)
#define DIAG_SNR_COLLECT_CLASS_NUM      (9)
#define DIAG_QUERY_NEIGHBOR_NUM         (32)

typedef struct
{
    HI_BOOL sort_by_att;        // ��ѯ���չʾʱ���Ƿ���˥��ֵ����(Ĭ��SNR����)
    HI_U8   filter_by_level : 1;// ��ʽ1: �Բ㼶��ɸѡ
    HI_U8   level           : 7;
    HI_U16  tei;                // ��ʽ2: ��TEIɸѡ

    HI_U32  join_freq : 8;      // ��ʽ3: �Լ�������ɸѡ��Ƶ��SNID��MAC��ַ�����ڵ��ַ����һ�����м���
    HI_U32  join_snid : 24;
    HI_U8   join_cco_mac[HI_PLC_MAC_ADDR_LEN];
    HI_U8   join_main_node_addr[HI_PLC_MAC_ADDR_LEN];

    HI_U32  belong_freq : 8;    // ��ʽ4: �Թ�������ɸѡ��Ƶ��SNID��MAC��ַ�����ڵ��ַ����һ�����м���
    HI_U32  belong_snid : 24;
    HI_U8   belong_cco_mac[HI_PLC_MAC_ADDR_LEN];
    HI_U8   belong_main_node_addr[HI_PLC_MAC_ADDR_LEN];
} DIAG_CMD_GET_TOP_NEIGHBOR_REQ_STRU;

// ��ѯ��Ϣ�ڵ� 12
typedef struct
{
    HI_U32 join_freq : 8;
    HI_U32 join_snid : 24;

    HI_U16 tei;
    HI_U8  level;
    HI_U8  min_att;

    HI_U16 att_avg;
    HI_S16 snr_avg;

    HI_U32 fc_cnt   : 22;
    HI_U32 pad      : 2;
    HI_U32 pad1     : 8;
} diag_neighbor_info_s;

#define DIAG_TOP_NEIGHBOR_QUERY_NUM     (32)

typedef struct
{
    HI_U32 ret;
    diag_neighbor_info_s results[DIAG_TOP_NEIGHBOR_QUERY_NUM];
} DIAG_CMD_GET_TOP_NEIGHBOR_IND_STRU;

// SNR����: ��ѯͳ��������ϸ���REQ, ֻҪһ�����м���
typedef struct
{
    HI_U32 freq : 8;
    HI_U32 snid : 24;
    HI_U8  cco_mac[HI_PLC_MAC_ADDR_LEN];
    HI_U8  main_node_addr[HI_PLC_MAC_ADDR_LEN];
} DIAG_CMD_STA_SNR_DATA_DETAIL_REQ_STRU;

// SNR����: ��ѯͳ��������ϸ���IND
typedef struct
{
    HI_U32 ret;

    HI_U32 last_reset_time;     // �ϴθ�λͳ������ʱ�䣬��λs
    HI_U16 collect_duration;    // ͳ��ʱ������λsec
    HI_U8  collect_enable;      // �Ƿ�����ͳ��
    HI_U8  pad;

    HI_U32 freq : 7;            // ����Ƶ��
    HI_U32 snid : 24;           // ����SNID
    HI_U32 is_detect_net : 1;   // ���������Ƿ���̽������

    HI_U8  detect_freq;         // ̽��Ƶ��
    HI_U8  detect_snid;         // ̽��SNID
    HI_U8  index;               // ������ͳ����Ϣ�е��±꣬������SNRͳ��������ʷ��¼����Ϊ����
    HI_U8  pad1;

    HI_U32 last_rcv_time;       // ��һ���յ��������ű��ʱ��

    HI_U8  cco_mac[HI_PLC_MAC_ADDR_LEN];            // CCO��MAC��ַ
    HI_U8  main_node_addr[HI_PLC_MAC_ADDR_LEN];   // ���ڵ��ַ

    HI_S16 snr_avg[DIAG_SNR_LEVEL_NUM];
    HI_U8  att_avg[DIAG_SNR_LEVEL_NUM];
    HI_U32 fc_cnt[DIAG_SNR_LEVEL_NUM];
} DIAG_CMD_STA_SNR_DATA_DETAIL_IND_STRU;

// ������SNR��ʷ��¼�ṹ 12B
typedef struct
{
    HI_U32 freq : 7;
    HI_U32 snid : 24;
    HI_U32 is_detect_net : 1;

    HI_U8  net_idx;
    HI_U8  snr_class;
    HI_S16 snr_avg;

    HI_U32 attenuation  : 8;
    HI_U32 fc_cnt       : 22;
    HI_U32 pad        : 2;
} diag_snr_net_result_st;

// SNR����: ��ѯͳ�����м�Ҫ���IND 396
typedef struct
{
    HI_U32 ret;

    HI_U32 last_reset_time;     // �ϴθ�λͳ������ʱ�䣬��λs
    HI_U16 collect_duration;    // ͳ��ʱ��
    HI_U16 collect_enable : 1;  // �Ƿ�����ͳ��
    HI_U16 list_node_num  : 15;

    diag_snr_net_result_st net_results[DIAG_QUERY_NEIGHBOR_NUM];
} DIAG_CMD_STA_SNR_DATA_SIMPLE_IND_STRU;

// SNR����: ��ѯ������ʷ��¼REQ
typedef struct
{
    HI_U32 record_index;
} DIAG_CMD_STA_SNR_HISTORY_NODE_REQ_STRU;

// SNR����: ��ѯ������ʷ��¼IND
typedef struct
{
    HI_U32 ret;
    HI_U32 save_snr_times;
    HI_U32 create_time;
    diag_snr_net_result_st net_results[DIAG_QUERY_NEIGHBOR_NUM];
} DIAG_CMD_STA_SNR_HISTORY_NODE_IND_STRU;


#define MAX_SNR_CHECK_STAT_NUM              (48)
#define MAX_SNR_CHECK_STAT_QUERY_NUM        (24)
#define MAX_SNR_BELONG_CHANGE_RECORD_NUM    (10)
#define DIAG_QUERY_SNR_NET_HIS_NUM          (32)

// SNR����: ��ѯ������48����ʷ��¼REQ
typedef struct
{
    HI_U32 freq : 8;
    HI_U32 snid : 24;
    HI_U8  cco_mac[HI_PLC_MAC_ADDR_LEN];
    HI_U8  main_node_addr[HI_PLC_MAC_ADDR_LEN];
    HI_U32 begin_index;
} DIAG_CMD_STA_SNR_NET_HISTORY_REQ_STRU;

// SNR����: ��ѯ������32����ʷ��¼IND
typedef struct
{
    HI_U32 ret;
    HI_U32 save_snr_times;

    diag_snr_net_result_st history_records[DIAG_QUERY_SNR_NET_HIS_NUM];
} DIAG_CMD_STA_SNR_NET_HISTORY_IND_STRU;

typedef struct
{
    HI_U32 to_freq  : 8;
    HI_U32 to_snid  : 24;   // �л�������SNID

    HI_U8  status;          // �л�״̬
    HI_U8  my_class;        // ������SNR�㼶
    HI_U8  to_class;        // �л��������SNR�㼶
    HI_U8  net_cnt;         // �ﵽ�����������

    HI_S16 my_avg;
    HI_S16 to_avg;
} diag_fast_change_record_st;

// վ�����������¼IND
typedef struct
{
    HI_U32  ret;

    HI_U8   fast_change_index;
    HI_U8   pad;
    HI_U16  fast_change_success_times;

    diag_fast_change_record_st records[MAX_SNR_CHECK_STAT_QUERY_NUM];
} DIAG_CMD_STA_FAST_CHANGE_STAT_IND_STRU;

typedef struct
{
    HI_U32 group_idx;
} DIAG_CMD_STA_FAST_CHANGE_STAT_REQ_STRU;

typedef struct
{
    HI_U32 my_freq          : 8;
    HI_U32 my_snid          : 24;

    HI_U32 to_freq          : 8;
    HI_U32 to_snid          : 24;

    HI_U32 status           : 4;
    HI_U32 pad              : 4;
    HI_U32 to_gt_rate       : 7;
    HI_U32 to_ge_rate       : 7;
    HI_U32 save_times       : 10;
} diag_stable_change_record_st;

// ��ѯվ�㳤�������л�����״̬IND
typedef struct
{
    HI_U32 ret;

    HI_U8   stable_change_index;
    HI_U8   pad;
    HI_U16  stable_change_success_times;

    diag_stable_change_record_st records[MAX_SNR_CHECK_STAT_QUERY_NUM];
} DIAG_CMD_STA_STABLE_CHANGE_STAT_IND_STRU;

// 40
typedef struct
{
    HI_U8 change_reason;
    HI_U8 old_net_idx;
    HI_U8 new_net_idx;
    HI_U8 old_snr_class : 4;
    HI_U8 new_snr_class : 4;

    HI_U32 old_net_freq : 8;
    HI_U32 old_net_snid : 24;
    HI_U8 old_net_main_node_addr[HI_PLC_MAC_ADDR_LEN];
    HI_U8 old_main_node_addr[HI_PLC_MAC_ADDR_LEN];

    HI_U32 new_net_freq : 8;
    HI_U32 new_net_snid : 24;
    HI_U8 new_net_main_node_addr[HI_PLC_MAC_ADDR_LEN];
    HI_U8 new_main_node_addr[HI_PLC_MAC_ADDR_LEN];

    HI_U32 change_time;
} diag_snr_belong_change_record_s;

// ��ѯվ����������¼
typedef struct
{
    HI_U32 ret;
    HI_U8 record_index;
    HI_U8 pad[3];

    diag_snr_belong_change_record_s records[MAX_SNR_BELONG_CHANGE_RECORD_NUM];
} DIAG_CMD_GET_BELONG_CHANGE_REOCRDS_IND_STRU;

typedef struct
{
    HI_U32 group_idx;
} DIAG_CMD_STA_STABLE_CHANGE_STAT_REQ_STRU;

typedef struct
{
    HI_U32 freq : 7;            // ����Ƶ��
    HI_U32 nid  : 24;           // ����NID
    HI_U32 is_detect_net : 1;   // ���������Ƿ���̽������

    HI_U8  detect_freq;         // ̽��Ƶ��
    HI_U8  detect_nid;          // ̽��NID
    HI_U8  index;               // ������ͳ����Ϣ�е��±�
    HI_U8  is_set_main_node_addr    : 1;    // �Ƿ����������ڵ��ַ
    HI_U8  pad0                     : 7;

    HI_U16 pad1;
    HI_U16 phase_att[PHASE_CNT];

    HI_U8  cco_mac[HI_PLC_MAC_ADDR_LEN];            // CCO��MAC��ַ
    HI_U8  main_node_addr[HI_PLC_MAC_ADDR_LEN];     // ���ڵ��ַ

    HI_U32 last_rcv_time;                           // ���һ���յ���ʱ��
} diag_net_info_st;

// 96
typedef struct
{
    HI_U8  my_snr_class;
    HI_U8  belong_net_idx;
    HI_U8  last_change_reason;
    HI_U8  pad;

    HI_U16 belong_stable_times;     // Ӧ������Ϣ�ȶ�������
    HI_U16 snr_class_stable_times;  // SNR�㼶�ȶ�������
    HI_U32 pad2;

    diag_net_info_st join_net_info;
    diag_net_info_st belong_net_info;

    HI_U8  find_join_net_info_fail;
    HI_U8  collector_init_fail_cnt;
    HI_U8  history_init_fail_cnt;
    HI_U8  cal_net_result_fail;

    HI_U8  trans_net_result_fail;
    HI_U8  cal_his_node_fail_cnt;
    HI_U8  add_snr_net_info_fail;
    HI_U8  link_node_malloc_fail;

    HI_U8  save_history_fail;
    HI_U8  cal_his_node_fail;
    HI_U8  fast_change_index;
    HI_U8  stable_change_index;

    HI_U16 fast_change_success_times;
    HI_U16 stable_change_success_times;

    HI_U16 begin_new_collect_cnt;   //�����µĲɼ����ڼ���
    HI_U16 pad1;

    HI_S16 preamble_snr;
    HI_U16 preamble_attenuation;
    HI_U32 pream_sta_num_th_cnt;    //�����������޵�snr����
} DIAG_CMD_STA_SNR_CTRL_STAT_IND_STRU;

// ��ѯ����վ����ϢREQ
typedef struct
{
    HI_U32 freq : 8;
    HI_U32 snid : 24;
    HI_U16 tei;
    HI_U16 pad;
} DIAG_CMD_NEAR_STA_DATA_REQ_STRU;

// ��ѯ����վ����ϢIND
typedef struct
{
    HI_U32 ret;

    HI_S16 snr_avg;
    HI_U16 att_avg;

    HI_U8  snr_class;
    HI_U8  min_att;
    HI_U16 pad;

    HI_S16 last_snr;
    HI_U16 last_att;

    HI_S32 snr_sum;
    HI_U32 att_sum;
    HI_U32 fc_cnt;

    diag_net_info_st join_net_info;
    diag_net_info_st belong_net_info;
} DIAG_CMD_NEAR_STA_DATA_IND_STRU;

typedef struct
{
    HI_U32 group_id;
}DIAG_CMD_NEAR_NET_INFO_REQ_STRU;

#define DIAG_QUERY_NET_INFO_NUM         (8)

typedef struct
{
    HI_U32 ret;

    diag_net_info_st net_infos[DIAG_QUERY_NET_INFO_NUM];
}DIAG_CMD_NEAR_NET_INFO_IND_STRU;

// ����վ��Ĺ�������REQ
typedef struct
{
    HI_U8   cco_mac[HI_PLC_MAC_ADDR_LEN];   // �����ĵ�ַ��Ĭ��ΪCCO��MAC��ַ��is_main_node_addrΪ1ʱ��ʾ���ڵ��ַ
    HI_BOOL is_main_node_addr;              // cco_mac�ֶΣ��Ƿ�Ϊ���ڵ��ַ
    HI_U8   freq;                           // ָ�������Ƶ��
    HI_U32  snid;                           // ָ�������SNID
} DIAG_CMD_SET_BELONG_NETWORK_REQ_STRU;

// ����վ��Ĺ�������IND
typedef struct
{
    HI_U32 ret; // 0��������ִ�гɹ�����������ʧ��
} DIAG_CMD_SET_BELONG_NETWORK_IND_STRU;

// ���վ��Ĺ�������IND
typedef struct
{
    HI_U32 ret; // 0��������ִ�гɹ�����������ʧ��
} DIAG_CMD_CLEAR_BELONG_NETWORK_IND_STRU;

// �����������
typedef struct
{
    HI_U8   cco_mac[HI_PLC_MAC_ADDR_LEN];
    HI_U8   freq;
    HI_U8   is_main_node_addr;

    HI_U32  snid;
    HI_U32  duration; // ����ʱ������λs
} DIAG_CMD_LOCK_NETWORK_REQ_STRU;

// ��������ظ�
typedef struct
{
    HI_U32 ret;
} DIAG_CMD_LOCK_NETWORK_IND_STRU;


//������ǿ�����ӵ������������
typedef struct
{
    HI_U8  mac[6];
    HI_U8  pad[2];
}DIAG_CMD_NDM_CONNECT_REQ_STRU;

typedef struct
{
    HI_U32 ip;

    HI_U8  mac[HI_PLC_MAC_ADDR_LEN];
    HI_U8  pad[2];
}PLC_NDM_IP_MAC_MAP_STRU;

typedef struct
{
    HI_U32 table_clear_cnt;
    HI_U32 set_success_cnt;
    HI_U32 unexpect_set_cnt;
    HI_U32 unexpect_mac_cnt;
    HI_U32 get_success_cnt;

    HI_U16 ndm_ip_sect_err_cnt;
    HI_U16 pad;
}DIAG_CMD_NDM_IP_MAC_MAP_STAT_STRU;

typedef struct
{
    PLC_NDM_IP_MAC_MAP_STRU           map[PLC_NDM_IP_MAC_MAP_NUM_MAX];
    DIAG_CMD_NDM_IP_MAC_MAP_STAT_STRU stat;
}DIAG_CMD_NDM_IP_MAC_MAP_INFO_STRU;

//���������������ýӿ�
typedef struct
{
    HI_U16 ndm_tei;
    HI_U16 pad;
}DIAG_CMD_NDM_ATTR_SET_STRU;

//����������������Ӧ�����
typedef struct
{
    HI_U32 result;      //���ý��
}DIAG_CMD_NDM_ATTR_SET_ACK_STRU;

/************************************************************************/
/*    4        ��ģ�������ϱ���                                            */
/************************************************************************/
/**
 *
 * 4.1    �����ϱ��ظ����������STA
 *
 */
typedef struct _DIAG_CMD_REPORT_REPEAT_REQUEST_JOIN_STRU_
{
   HI_U16 tei;
   HI_U16 pad;
} DIAG_CMD_REPORT_REPEAT_REQUEST_JOIN_STRU;

/**
 *
 * 4.2    �����������ʱ���¼���¼(����ģ���Լ������ϱ�HSO)
 *
 */
typedef struct _DIAG_CMD_REPORT_BUFFER_OVERFLOW_STRU_
{
   HI_U8 module_id;
   HI_U8 buffer_id;
   HI_U8 over_flow_size;
   HI_U8 pad;
} DIAG_CMD_REPORT_BUFFER_OVERFLOW_STRU;

/**
 *
 * 4.5    ͷ�ˣ�TDMA�������ڷ������( ���������ģ�������ϱ�CONSOLE)
 *
 */
typedef struct  id_diag_cmd_tdma_entry_head_
{
    HI_U8 beacon_slot_count;              // �ű�ʱ϶��
    HI_U8 bind_csma_slot_count;      // ��CSMAʱ϶��
    HI_U8 tdma_slot_count;     // TDMAʱ϶��
    HI_U8 beacon_duration;    // beaconʱ϶����ʱ�䣬��λ:100΢��.ע:ÿ���ű�ʱ϶����ʱ�䶼����ͬ��
} id_diag_cmd_tdma_entry_head;

typedef struct  id_diag_cmd_beacon_slot_
{
    HI_U32 offset;                         // �ű�ʱ϶��ʼ�����ʱ�䣬����ű����ڿ�ʼʱ���ƫ�ơ���λ:100΢��
    HI_U16 tei;                            // ʱ϶��Ӧ��TEI
    HI_U8  beacon_type;                    // �ű�����
    HI_U8  event;                          // ��ʶ�¼�(��������)
} id_diag_cmd_beacon_slot;

typedef struct id_diag_cmd_bind_csma_slot_
{
    HI_U32 offset;                         // ʱ϶��ʼ�����ʱ�䣬����ű����ڿ�ʼʱ���ƫ�ƣ���λ:100΢��
    HI_U16 duration;                  // ʱ϶����ʱ�䣬��λ:100΢��
    HI_U16 tei;                           // ʱ϶��Ӧ��Ŀ��TEI
} id_diag_cmd_bind_csma_slot;

typedef struct id_diag_cmd_csma_slot_
{
    HI_U16 offset;                         // ʱ϶��ʼ�����ʱ�䣬����ű����ڿ�ʼʱ���ƫ�ƣ���λ:100΢��
    HI_U16 duration;                    // ʱ϶����ʱ�䣬��λ:100΢��
} id_diag_cmd_csma_slot;

typedef struct  id_diag_cmd_tdma_slot_
{
    HI_U16 offset;                         // ʱ϶��ʼ�����ʱ��
    HI_U16 duration;                       // ʱ϶����ʱ��
    HI_U16 tei;                            // ʱ϶��Ӧ��TEI
    HI_U8  lid;                            // ʱ϶���ӱ�ʶ��
    HI_U8  rsvd;                           // ����
} id_diag_cmd_tdma_slot;

//����ʱ϶��Ŀ
typedef struct id_diag_cmd_region_slot_st_
{
    HI_U32 starttime;                      // ����ʱ϶��ʼ����ʱ�䣬��λΪNTB
    HI_U32 duration;                       // ����ʱ��,��λΪ100΢��
    HI_U8  region_id;                      // ����SNID��
    HI_U8  region_type;                    // ʱ϶����(����ʱ϶���߹���ʱ϶)
    HI_U16 rsvd;                           // ����
} id_diag_cmd_region_slot_st;

//����SNID��Ŀ
typedef struct id_diag_cmd_snid_entry_
{
    HI_U32 starttime;                      // ����SNID��ʱ��
    HI_U8  snid;                           // ���ĺ��SNID��
    HI_U8  rsvd1;                          // ����
    HI_U16 rsvd2;                          // ����
} id_diag_cmd_snid_entry;

//������Կ��Ŀ
typedef struct id_diag_cmd_encryption_key_entry_
{
    HI_U32 starttime;                      // ������Կ��ʱ��
    HI_U8  key_index;                      // ������Կ��������
    HI_U8  svd1;                           // ����
    HI_U16 rsvd2;                          // ����
} id_diag_cmd_encryption_key_entry;

// ʱ϶�������
#define DFX_ID_DIAG_CMD_MAX_BEACON_SLOT 150     // ���beaconʱ϶��
#define DFX_ID_DIAG_CMD_MAX_BIND_CSMA_SLOT 50      // ����csmaʱ϶��
#define DFX_ID_DIAG_CMD_MAX_TDMA_SLOT 50      // ���tdmaʱ϶��
#define DFX_ID_DIAG_CMD_MAX_REGION_SLOT_COUNT 7       // �������ʱ϶��Ŀ��

#define DFX_CONVERT_MAC_ADDR_MODE_NO (1)  //��ת��
#define DFX_CONVERT_MAC_ADDR_MODE_YES (2) //ת��
/**
 *
 * 4.15    վ���������������ϱ��Լ�����������
 *
 */
typedef struct
{
    HI_U16   my_tei;                        // ��վ��tei
    HI_U16   my_proxy_tei;                  // ��վ�㸸����

    HI_U8    my_level;                      // ��վ��㼶
    HI_U8    my_ability;                    // ��վ����������CCO, �����м̣�������ͨվ��
    HI_U8    my_mac[6];                     // ��վ��MAC��ַ

    HI_U32   my_snid;                       // ��վ��SNID

    HI_BOOL  network_formed;                // �������Ƿ����
    HI_BOOL  send_data_in_self_phase;       // �Ƿ��ڱ�վ����λ��������
    HI_U8    my_phase_mode;                 // ��վ����λģʽ�� ����λ���ǵ���λ
    HI_U8    my_phase;                      // ��վ����λ���͡� A B C ALL

    HI_U8    form_serial_number;            // ����ϵ�к�
    HI_U8    freq_mode_present;             // վ���Ƶ��ģʽ
    HI_U8    fast_evaluate_flag;            // ����·��������־
    HI_U8    network_state;                 // ����״̬

    HI_U32   select_snid;                   // ��ѡsnid

    HI_U8    proxy_flag_exist;              // �Ƿ���ھ�̬����
    HI_U8    chl_state;                     // MACͨ��״̬״̬
    HI_U8    convert_mac_addr_mode;    // �Ƿ�00��β��mac��ַת��ΪFE��DFX_CONVERT_MAC_ADDR_MODE_NO
    HI_U8    pad;

    HI_U32  blind_check_count;              // ä�����
    HI_U32  rcv_freq_change_notify_count;   // ����Ƶ���л�֪ͨ����
} DIAG_CMD_REPORT_NETWORK_ATTRIBUTE_STRU;

//MAC��ͨ��״̬���䶨�����£�
typedef enum
{
    HI_MAC_CHL_STATE_UNKNOWN = 0,          //ͨ��״̬δ֪
    HI_MAC_CHL_STATE_FORMING,              //����������
    HI_MAC_CHL_STATE_FORMED,               //�������
    HI_MAC_CHL_STATE_NORMAL,               //����״̬������֧��Լ����APP������ҵ��
    HI_MAC_CHL_STATE_MAINTAIN,             //ͨ��ά��״̬����֧��APP��ҵ��
}HI_MAC_CHL_STATE_ENUM;

/**
 *
 * 4.16    ���������������������

 *
 */
 #define DFX_CHOSE_PROXY_SUM DFX_PROXY_SUM
typedef struct
{
    HI_U8    start_change_proxy;
    HI_U8    start_assoc_req;
    HI_U16   old_proxy;
    HI_U16   chose_proxy_tei[DFX_CHOSE_PROXY_SUM];     //ѡ���Ĵ���վ��
    HI_U16   rsvd;
}DIAG_CMD_REPORT_CHOSE_PROXY_TEI_STRU;

/**
 *
 * 4.17    վ���жϱ�վ������

 *
 */
enum
{
    RCV_BEACON_FAILED = 1,              //STA�ղ����ű�����(һ��������������)7
    CHOSE_PROXY_FAIL,                   //�ղ�������վ�����Ϣ(4��·������)
    SERIAL_NUM_CHANGE,                  //����ϵ�кŷ����ı�
    RCV_LEAVE_IND,                      //���յ�����ָʾ����
    CCO_MAC_CHANGE,                     //CCO��MAC�����ı�

    SAME_NEIGHBOR_SNID,                 //CCOʹ�ã��ھ�����SNID���뱾����SNID��ͬ
    SET_FREQ_MODE,                      //STA�յ�NV��ֱ�ӱ��Ƶ��ָ��(���Ƽ�����)
    SET_MAC_ADDR,                       //STA���յ����MAC��ַ����
    SET_WHITE_LIST,                     //CCOͨ���������ߵ�140��վ��ʱ����������
    PROXY_CHANGE_DISCOVERY_NODE,        //STA���ֱ�վ��Ĵ�����ݱ�Ϊ����վ��

    NOT_IN_WRITER,                      //ͨ���������޳�վ��
    HSO_SET_CCO_SNID,                   //ͨ��hso�������CCO��SNID��
    PCO_MAX_LEVEL,                      //��վ��㼶�����������
    INTERFACE_SET_CCO_SNID,             //���ο����ӿ�����CCO��SNID
    NOT_IN_BACKUP_MAC_LIST,             //��վ��MAC��ַ���ڱ���MAC��ַ�б�

    STA_SET_PRODUCT_TYPE,               //վ�����ò�Ʒ���Ͳ�ͬ
    NDM_METER_ACCESS,                   //�������������ǿ������
    NDM_METER_CHECK_DISCONNECT,         //������������ԶϿ�����
    PLC_SWITCH_TO_RF,                   // PLC����ģʽ�л���RFģʽ
    RF_SWITCH_TO_PLC,                   // RF����ģʽ�л���PLCģʽ

    TF_IDENTIFY_RESET = 21,             //̨��ʶ������
    TF_IDENTIFY_CHECK_SUPPORT_RESET,    // STA���֧��̨��ʶ�𣬲����Ѿ�����������������������
    NM_ENTER_NETWORK_TEST_MODE_RESET,   // MAC������������ģʽ
    NM_EXIT_NETWORK_TEST_MODE_RESET,    // MAC���˳��������ģʽ
    NM_RCV_NETWORK_CONFLICT_PK,         // ���յ������ͻ����

    STA_EXIT_NON_ASSOCIATE = 26,        // STA�˳�������״̬
    BBPLC_SWITCH_TO_SCPLC,              // ���PLC����ģʽ�л������ز�PLC����ģʽ
    SCPLC_SWITCH_TO_BBPLC,              // ���ز�PLC����ģʽ�л������PLC����ģʽ
    ENTER_PHY_TEST_MODE,                // ��������ϵͳ������������ģʽ
    EXIT_PHY_TEST_MODE,                 // ��������ϵͳ�Ƴ���������ģʽ

    NETWORK_MODE_SWITCH = 31,           // ����ģʽ�л�
    NETWORK_PTCL_SWITCH,                // ����Э���л�
    REJOIN_CALL_FROM_APP_LAYER,         // APP��������������ӿ�
    CHANGE_LOCK_NETWORK,                // �ı����������磬��������
    DEVICE_POWER_FAILURE_RECOVER,       // ͣ��ָ�

    CHANGE_LOCK_NETWORK_LONG = 36,      // �ı����������磬��������
    CHANGE_LOCK_NETWORK_LONG_LONG,      // �ı����������磬����������
    CHANGE_TO_BELONG_NETWORK,           // �뿪��ǰ���磬ȥ������ȷ��������
    CHANGE_NOT_JOIN_LOCK_NET,           // ���ּ���Ĳ�������������
    CHANGE_LOCK_NETWORK_HSO,            // HSO������������

    CHANGE_LOCK_NETWORK_NTB_CRT = 41,   // �ı����������磬NTB��������
    STA_NOT_RCV_LEAVE_IND,              // ��Դģʽ�£�STAվ��û���յ����߱��ģ��������أ�û�б�������������

    TOPO_TYPE_CHANGE_RESET,             // �������ͱ仯����MAC����λ

    MAC_REJOIN_REASON_INVALID = 0xff    // ��Ч��������ԭ��
};

typedef struct
{
   HI_U32   bpc;
   HI_U8    reason_code;  //   ԭ��
   HI_U8    rsvd[3];
}DIAG_CMD_REPORT_LEAVE_NETWORK_STRU;

#define     VIOLENCE_STA_CNT_PER_PK      50  //ÿ���������б����
typedef struct
{
   HI_U8 mac[VIOLENCE_STA_CNT_PER_PK][HI_PLC_MAC_ADDR_LEN];     // �ܾ�վ��MAC��ַ
}DIAG_CMD_QUERY_VIOLENCE_STA_STRU;

#define    MAX_VIOLENCE_STA_COUNT      200      //���ܾ��б���
#define MAC_PLC_QUERY_VIOLEDNCE_VER_R0  0 // ����ܾ��б��ѯ�汾��
#define MAC_PLC_QUERY_VIOLEDNCE_VER_R1  1 // ����ܾ��б��ѯ�汾��

typedef struct
{
   HI_U8 mac[HI_PLC_MAC_ADDR_LEN];
   HI_U8 pad[2];
}DIAG_CMD_QUERY_VIOLENCE_STA_MAC_STRU;

typedef struct
{
   HI_U32  interface_ver;           // �汾��
   HI_U16  sn;                      // �����
   HI_BOOL is_last;                 // �Ƿ����һ����
   HI_U8   num;                     // �����ϱ��ĸ���
   DIAG_CMD_QUERY_VIOLENCE_STA_MAC_STRU mac[VIOLENCE_STA_CNT_PER_PK];
}DIAG_CMD_QUERY_MAX_VIOLENCE_STA_STRU;

typedef struct
{
    HI_U16  whitelist_num;                       //�������е�վ����
    HI_BOOL mac_authentication;                  //����������״̬
    HI_U8   reason;                              //�������仯��ԭ��
    HI_U32  pad2;
}DIAG_CMD_REPORT_WHITELIST_CHANGED;


/***
 * MAC�����ʵ�������ֵʱ�ϱ���ǰ����
 *
 */
typedef struct
{
    HI_U32 timestamp;
    HI_U32 ntb;
    HI_U32 bpc;
    HI_U32 rx_speed;
    HI_U32 tx_speed;
}DIAG_CMD_NOTIFY_MAC_SPEED_LOW_STRU;

typedef struct
{
    HI_U32 ndm_set_pk_cnt;          // ndm �·���������
    HI_U32 ndm2nm_pk_cnt;           // ndm �·����ĵ�nmģ����зַ��ĸ���

    HI_U32 nm2cvg_pk_cnt;           // ndm�·�����ͨ��nm���͵���۵�����
    HI_U32 cvg2seg_pk_cnt;          // ��۽��յ�ndm���ĵ�����
    HI_U32 seg_in_queue_pk_cnt;     // �ֶ������ndm���ĵ�����
    HI_U32 seg_in_queue_fail_cnt;   // ������ֶζ���ʧ�ܵ�����

    HI_U32 tx_all_pk_cnt;           // ���ͻ��յ�ndm���ĵ�����
    HI_U32 tx_send_success_cnt;     // ���ͻ����ͳɹ���ndm���ĵ�����
    HI_U32 tx_send_fail_cnt;        // ���ͻ�����ʧ�ܵ�ndm���ĵ�����

    HI_U32 tx_send_timeout_cnt;     // ���ͻ����ͳ�ʱ��ndm���ĵ�����
    HI_U32 tx_resend_cnt;           // ���ͻ��ط���ndm���ĵ�����

    HI_U32 rx_rcv_pk_cnt;           // ���ջ����յ�ndm��������
    HI_U32 rsb2cvg_pk_cnt;          // ����ַ��Ļ��ndm���ĵ�����
    HI_U32 rsb_timeout_cnt;         // ���鳬ʱ������
    HI_U32 rsb_filter_pk_cnt;       // �����б����˵���ndm���ĵĸ���
    HI_U32 cvg2nm_pk_cnt;           // ��۷ַ���nmģ���ndm���ĵ�����
    HI_U32 nm_filter_pk_cnt;        // ���ܹ��˵��ı�����
    HI_U32 nm2ndm_pk_cnt;           // nmģ����Ϣ����ַ���ndm��ndm��������
}DIAG_CMD_QUERY_NDM_PK_INFO_STRU;

typedef enum  _NDM_STATICS_CONTEXT
{
    NDM_SET = 0 ,
    NDM_NDM2NM ,
    NDM_NM2CVG ,
    NDM_CVG2SEG,
    NDM_SEG_IN_QUEUE,
    NDM_SEG_IN_QUEUE_FAIL,
    NDM_TX_ALL,
    NDM_TX_SUCCESS,
    NDM_TX_FAIL,
    NDM_TX_TIMEOUT,
    NDM_TX_RESEND,
    NDM_RX_RCV,
    NDM_RSB2CVG,
    NDM_RSB_TIMEOUT,
    NDM_RSB_FILTER,
    NDM_CVG2NM,
    NDM_NM_FILTER,
    NDM_NM2NDM
}NDM_STATICS_CONTEXT_ENUM ;
typedef enum  _tx_status_
{
    PK_GET = 0,
    TX_SUCCESS ,
    TX_TIMEOUT ,
    TX_FAIL ,
    TX_RESEND
}TX_STATUS_E ;

/************************************************************************/
/*         5    ͳ����                                                  */
/************************************************************************/
//��ѯ���������Ĭ�ϲ���
typedef struct
{
    HI_U32 clearSwitch;
}DIAG_CMD_QUERY_REQ_PARA_STRU;

/**
 *
 * 5.1    DFX���ڲ�ѯ�������ϱ�����ͳ�����ݣ�HSO, console��
 *
 */
typedef struct
{
    HI_U32 rts_cts_count;                       // ����rts֡����
    HI_U32 sack_count;                          // ����sack֡����
    HI_U32 beacon_count;                        // �����ű�֡����
    HI_U32 sounding_count;                      // ����sound֡����

    HI_U32 unicast_count;                       // �������ĸ���
    HI_U32 broadcast_count;                     // �㲥���ĸ���

    HI_U32 short_packet_count;                  // ���Ͷ̰�����
    HI_U32 long_packet_count;                   // ���ͳ�������
    HI_U32 pb72_packet_count;                   // ����pb72����
    HI_U32 pb264_packet_count;                  // ����pb264����

    HI_U32 collision_count;                     // ���ͳ�ͻ����
    HI_U32 tx_get_all_msdu_cnt;                 // ���ͻ��ɹ���ȡmsdu������
    HI_U32 tx_get_nm_cnt;                       // ��ȡ���ܱ�����
    HI_U32 tx_get_ip_cnt;                       // ��ȡIP������
    HI_U32 tx_get_app_cnt;                      // ��ȡAPP������
    HI_U32 tx_get_upg_cnt;                      // ��ȡ����������

    HI_U32 tx_send_msdu_success;                // ���ͻ��ɹ�����msdu������
    HI_U32 tx_send_nm_success;                  // �������ܱ��ĳɹ�����
    HI_U32 tx_send_ip_success;                  // ����IP���ĳɹ�����
    HI_U32 tx_send_app_success;                 // ����APP���ĳɹ�����
    HI_U32 tx_send_upg_success;                 // �����������ĳɹ�����


    HI_U32 tx_send_msdu_timeout;                // ���ͻ����ͳ�ʱmsdu������
    HI_U32 tx_send_nm_timeout;                  // �������ܱ��ĳ�ʱ����
    HI_U32 tx_send_ip_timeout;                  // ����IP���ĳ�ʱ����
    HI_U32 tx_send_app_timeout;                 // ����APP���ĳ�ʱ����
    HI_U32 tx_send_upg_timeout;                 // �����������ĳ�ʱ����

    HI_U32 tx_send_msdu_fail;                   // ���ͻ�����ʧ��msdu������
    HI_U32 tx_send_nm_fail;                     // �������ܱ���ʧ�ܴ���
    HI_U32 tx_send_ip_fail;                     // ����IP����ʧ�ܴ���
    HI_U32 tx_send_app_fail;                    // ����APP����ʧ�ܴ���
    HI_U32 tx_send_upg_fail;                    // ������������ʧ�ܴ���

    HI_U32 tx_send_msdu_resend;                 // ���ͻ��ط�msdu�Ĵ���
    HI_U32 tx_send_nm_resend;                   // �������ܱ����ش�����
    HI_U32 tx_send_ip_resend;                   // ����IP�����ش�����
    HI_U32 tx_send_app_resend;                  // ����APP�����ش�����
    HI_U32 tx_send_upg_resend;                  // �������������ش�����

    HI_U32 sof_wait_sack_send_success;          // �ȴ�SACK SOF���ͳɹ�����
    HI_U32 sof_not_wait_sack_send_success;      // ����Ҫ�ȴ�SACK���ķ��ͳɹ�����
    HI_U32 sof_resend_exclude_fix_resend;       // �����ش��������������̶��ش�

    HI_U32 no_upg_slot_send;                    // ��bcsmaʱ϶�����������ĵĸ���
    HI_U32 tx_send_wait_end_interrupt_timeout;  // �ȷ�������жϳ�ʱ����
    HI_U32 tx_state_abnormal_cnt;               // ����״̬�쳣����
}DIAG_CMD_QUERY_TX_STATICS_STRU;

/**
 *
 * 5.2    DFX���ڲ�ѯ�������ϱ�����ͳ�����ݣ�HSO, console��
 *
 */

typedef struct
{
    HI_U32 fc_count;                        // FC�жϴ���           FC�ж���ͳ��
    HI_U32 fc_crc_success_count;            // FCУ����ȷ����     FC�ж���ͳ��
    HI_U32 fc_crc_err_count;                // FCУ��������     FC�ж���ͳ�� ��fccsУ����󣬲�����fc�жϺ����ͳ����Ϊ0��
    HI_U32 fc_local_count;                  // ����FC����         beacon��sof��sack��sound��rts_cts:

    HI_U32 false_frame_count;               // ������
    HI_U32 not_expect_dt_count;             // ����Ԥ��֡�ĸ�������ײ������

    HI_U32 beacon_count;                    // ����beacon֡����
    HI_U32 sof_count;                       // ����sof֡����
    HI_U32 sack_count;                      // ����sack֡����
    HI_U32 rts_count;                       // ����rts֡����
    HI_U32 cts_count;                       // ����cts֡����
    HI_U32 sound_count;                     // ����sound֡����
    HI_U32 sound_sack_count;                // ����sound sack����

    HI_U32 coordinate_frame_count;          // ������Э��֡����
    HI_U32 pcs_interrupt_cnt;               // PCS�жϴ���
    HI_U32 parallel_ok_cnt;                 // ���м���ж�OK����
    HI_U32 beacon_pb_crc_err_count;         // beacon pbУ��ʧ������
    HI_U32 beacon_pb_crc_success_count;     // beacon pbУ��ɹ�����

    HI_U32 broadcast_count;                 // ���ع㲥���ĸ���    sof:            bcf
    HI_U32 unicast_count;                   // ���ص������ĸ���    sof:        bcf
    HI_U32 resend_count;                    // �����ش����ĸ���     sof:�ж��ش���־ resend_packet
    HI_U32 pb_cnt[4];                       // ���ղ�ͬPB�鱨�ĸ���

    HI_U32 short_pb_crc_err_count;          // 136pb��crcУ�������
    HI_U32 long_pb_crc_err_count;           // 520pb��crcУ�������
    HI_U32 pb72_crc_err_count;              // 72pb��crcУ�������
    HI_U32 pb264_crc_err_count;             // 264pb��crcУ�������

    HI_U32 short_pb_count;                  // 136pb�����   pb���ж�
    HI_U32 long_pb_count;                   // 520pb�����
    HI_U32 pb72_count;                      // 72pb�����   pb���ж�
    HI_U32 pb264_count;                     // 264pb�����   pb���ж�

    HI_U32 discard_packet_count;            // ��������
    HI_U32 no_pb_packet_count;              // ����δ��ɣ�fcɨƵ ��PB��ı��ĸ���
    HI_U32 time_out_count;                  // �����ڽ��ն��г�ʱ����
}DIAG_CMD_QUERY_RX_STATICS_STRU;
typedef struct
{
    HI_U32 ssn_err_count;                   // ssn���쳣����
    HI_U32 send_sof_to_rsb_count;           // ���͵�����������SOF֡����
    HI_U32 rx_lisr_count;                   // ���ջ��жϲ�������
    HI_U32 rx_hisr_count;                   // ���ջ��߼��ж���Ӧ����
    HI_U32 sack_success_count;              // ����SACK֡����Ϊ���ճɹ��ĸ���
    HI_U32 sack_fail_count;                 // ����SACK֡����Ϊ����ʧ�ܵĸ���
    HI_U32 discard_beacon_count;            // �����ű�֡�ĸ���
    HI_U32 discard_sounding_count;          // ����SOUNDING֡�ĸ���
    HI_U32 discard_sounding_ack_count;      // ����SOUNDING ACK֡�ĸ���
    HI_U32 discard_sack_count;              // ����SACK֡�ĸ���
    HI_U32 tx_send_cnt;                     // ���ʹ���
    HI_U32 burst_send_end_cnt;              // ��������жϴ���
}DIAG_CMD_QUERY_RX_DIAGNOSTIC_STRU;
typedef struct
{
    HI_U32 beacon_in_que_cnt;
    HI_U32 sof_in_que_cnt;
    HI_U32 fc_in_que_cnt;
    HI_U32 coordinate_in_que_cnt;
	HI_U32 direct_send_in_que_cnt;
    HI_U32 beacon_out_que_cnt;
    HI_U32 sof_out_que_cnt;
    HI_U32 fc_out_que_cnt;
    HI_U32 coordinate_out_que_cnt;
	HI_U32 direct_send_out_que_cnt;
    HI_U32 beacon_timeout_cnt;
    HI_U32 sof_timeout_cnt;
    HI_U32 fc_timeout_cnt;
    HI_U32 coordinate_timeout_cnt;
	HI_U32 direct_send_timout_cnt;
    HI_U32 get_beacon_cnt;
    HI_U32 get_sof_cnt;
    HI_U32 get_fc_cnt;
    HI_U32 get_coordinate_cnt;
    HI_U32 sof_overflow_cnt;
    HI_U32 fc_overflow_cnt;
    HI_U32 beacon_overflow_cnt;
    HI_U32 coordinate_overflow_cnt;
}DIAG_CMD_QUERY_TX_QUEUE_DIAGNOSTIC_STRU;

typedef struct
{
    HI_U32 get_slot_cnt;
    HI_U32 sta_slot_update_cnt;
    HI_U32 cco_slot_update_cnt;
    HI_U32 updata_event_cnt;
    HI_U32 no_slot_cnt;
}DIAG_CMD_QUERY_SLOT_DIAGNOSTIC_STRU;


typedef struct
{
    HI_U32 tmi6_send_cnt;                   // TMI6���ͱ��Ĵ���
    HI_U32 tmi4_send_cnt;                   // TMI4���ͱ��Ĵ���
    HI_U32 tmi1_send_cnt;                   // TMI1���ͱ��Ĵ���
    HI_U32 tmi0_send_cnt;                   // TMI0���ͱ��Ĵ���
    HI_U32 tmi9_send_cnt;                   // TMI9���ͱ��Ĵ���
    HI_U32 tmi11_send_cnt;                  // TMI11���ͱ��Ĵ���
    HI_U32 tmi12_send_cnt;                  // TMI12���ͱ��Ĵ���
    HI_U32 tmi13_send_cnt;                  // TMI13���ͱ��Ĵ���
    HI_U32 tmi14_send_cnt;                  // TMI14���ͱ��Ĵ���
    HI_U32 sounding_send_cnt;               // ͨ��SOUNDING���͵Ĵ���
    HI_U32 backoff_fail_cnt;                // �˱�ʧ�ܴ���
    HI_U32 send_event_cnt;                  // ���ͻ������¼�����
    HI_U32 handle_event_cnt;                // ���ͻ������¼�����
    HI_U32 tdma_period_refreshed_cnt;       // ʱ϶����ʱ�����
    HI_U32 mpdu_queue_in_cnt;               // ����������¼�����
    HI_U32 tx_en_send_fail_cnt;             // ʹ��PHY����ʧ�ܴ���
    HI_U32 channel_busy_cnt;                // ����ǰ�ŵ���æ����
    HI_U32 tmi6_send_fail_cnt;              // TMI6���ͱ���ʧ�ܴ���
    HI_U32 tmi4_send_fail_cnt;              // TMI4���ͱ���ʧ�ܴ���
    HI_U32 tmi1_send_fail_cnt;              // TMI1���ͱ���ʧ�ܴ���
    HI_U32 tmi0_send_fail_cnt;              // TMI0���ͱ���ʧ�ܴ���
    HI_U32 tmi9_send_fail_cnt;              // TMI9���ͱ���ʧ�ܴ���
    HI_U32 tmi11_send_fail_cnt;             // TMI11���ͱ���ʧ�ܴ���
    HI_U32 tmi12_send_fail_cnt;             // TMI12���ͱ���ʧ�ܴ���
    HI_U32 tmi13_send_fail_cnt;             // TMI13���ͱ���ʧ�ܴ���
    HI_U32 tmi14_send_fail_cnt;             // TMI14���ͱ���ʧ�ܴ���
    HI_U32 tx_send_bea_fail_cnt;            // �ű�����ʱ϶���㷢��ʧ�ܴ������������ŵ�æ
    HI_U32 tx_send_bea_fail_by_busy_cnt;    // �ŵ�æ�����·����ű�ʧ�ܼ���
}DIAG_CMD_QUERY_TX_DIAGNOSTIC_STRU;
typedef struct
{
    HI_U32 inital_sounding_req_cnt;              // ���뷢��̬SOUNDING����
    HI_U32 initial_sounding_cnt;                 // ����̬SOUNDING����
    HI_U32 dynamic_sounding_cnt;                 // ��̬SOUNDING����
    HI_U8  phy_in;
    HI_U8  pad[3];
}DIAG_CMD_QUERY_SOUND_STATICS_STRU;
typedef struct _DIAG_CMD_PLC_DEV_STAT_STRU_
{
    HI_U32 output_msdu_size_err_cnt;                 // ����IP����ʱ���Ĵ�С�쳣����
    HI_U32 output_get_mac_buffer_fail_cnt;           // ����IP����ʱ����MAC��BUFFERʧ�ܴ���
    HI_U32 output_cvg_sgm_fail_cnt;                  // ����IP����ʱ��۷ֶδ���ʧ�ܴ���
    HI_U32 output_not_ip_cnt;                        // ����IP����ʱ����IP���ĵĴ���
    HI_U32 output_send_cnt;                          // ����IP���Ĵ���
    HI_U32 input_msdu_size_err_cnt;                  // ����IP����ʱ���Ĵ�С�쳣����
    HI_U32 input_get_ip_buffer_fail_cnt;             // ����IP����ʱ��ȡIP��BUFFERʧ�ܴ���
    HI_U32 input_ip_broadcast_cnt;                   // ����IP�����й㲥IP���ĵĸ���
    HI_U32 input_broadcast_not_my_packet_cnt;        // ����IP����ʱ�㲥�����зǱ�վ�㱨�ĸ���
    HI_U32 input_broadcast_pk_my_packet_cnt;         // ����IP����ʱ�㲥�����б�վ�㱨�ĸ���
    HI_U32 input_unicast_my_packet_cnt;              // ����IP����ʱ�������ı�վ�㱨�ĸ���
    HI_U32 input_unicast_not_my_packet_cnt;          // ����IP����ʱ�������ķǱ�վ�㱨�ĸ���
} DIAG_CMD_PLC_DEV_STAT_STRU;
typedef struct
{
    HI_U32 tx_retranfer_packet_count;            // ������������ͻ���ת�����ĸ���
    HI_U32 tx_ip_packet_count;                   // ������������ͻ�ip���ĸ���
    HI_U32 tx_network_packet_count;              // ������������ͻ����ܱ��ĸ���
    HI_U32 tx_app_packet_count;                  // ������������ͻ�Ӧ�ñ��ĸ���
    HI_U32 tx_ndc_packet_count;                  // ������������ͻ�NDC���ĸ���
    HI_U32 tx_unknown_packet_count;              // ������յ�Ҫ���͵�δ֪����

    HI_U32 rx_retranfer_packet_count;            // ������ӽ��ջ��յ�ת�����ĸ���
    HI_U32 rx_network_packet_count;              // ������ӽ��ջ��յ������ܱ��ĸ���
    HI_U32 rx_ip_packet_count;                   // ������ӽ��ջ��յ���ip���ĸ���
    HI_U32 rx_app_packet_count;                  // ������ӽ��ջ��յ���app���ĸ���
    HI_U32 rx_unknown_packet_count;              // ������ӽ��ջ��յ���δ֪���ĸ���
    HI_U32 rx_upg_packet_count;                  // ������ӽ��ջ��յ����������ĸ���
    HI_U32 rx_ndc_packet_count;                  // ������ӽ��ջ��յ���NDC���ĸ���

    HI_U32 unicast_discard_count;                // �������Ķ����������Ǳ�վ�㱨��
    HI_U32 unicast_retransfer_count;             // ��������ת������
    HI_U32 broadcast_discard_count;              // �㲥���Ķ�������
    HI_U32 broadcast_retransfer_count;           // �㲥����ת������
    HI_U32 broadcast_retransfer_get_buf_fail_count; // �㲥����ת����������bufʧ�ܸ���

    HI_U32 query_route_fail_count;               // ·�ɲ�ѯʧ��
    HI_U32 get_tei_by_mac_fail_count;            // ͨ��MAC��ַ��ȡTEIʧ�ܴ���

    HI_U32 app_dequeue_buf_fail_count;           // Ӧ�ò㱨�������ʧ�ܸ���
    HI_U16 app_pk_msdu_size_err_count;           // Ӧ�ò㱨�Ĵ�С�����쳣����
    HI_U16 set_app_send_info_fail_count;         // ����Ӧ�ò㱨�ķ�����Ϣʧ�ܴ���
    HI_U16 app_vlantag_err_count;                // Ӧ�ò㱨�����ȼ��쳣����
    HI_U16 send_pk_to_app_fail_count;            // ���ͱ��ĵ�Ӧ�ò�ʧ�ܸ���
}DIAG_CMD_QUERY_CVG_STATICS_PK_STRU;

typedef struct
{
    HI_U16 tei;
    HI_U8  level;
    HI_U8  bt    :4;
    HI_U8  rsv  :4;

    HI_U32 snid;

    HI_U8  freqerr_exception_cnt;
    HI_U8  rcv_bcn_cnt;
    HI_S16  freqerr_output;
}DIAG_CMD_QUERY_NTB_CANDI_STA_STRU;

typedef struct
{
    HI_U32 ntb_state;
    HI_U32 sync_snid;
    HI_U32 cycle_cnt;
    HI_U32 curr_sync_sta_idx;
    HI_U16 curr_sync_tei;
    HI_S16 freqerr;
    HI_U32 sync_sta_freqerr_exception_cnt;
    HI_U32 sync_sta_change_cnt;
    HI_U32 candidate_sta_cnt;
    DIAG_CMD_QUERY_NTB_CANDI_STA_STRU candidate_sta[DFX_NTB_SYNC_STA_CNT_MAX];
}DIAG_CMD_QUERY_NTB_STATISTIC_STRU;

typedef struct
{
    HI_U16 nid_same_in_diff_freq_cnt;
    HI_U16 overflow_snid_cnt;

    HI_U32 overflow_snid;
    HI_U8  overflow_freq;
    HI_U8  pad1[3];
}DIAG_CMD_QUERY_NTB_STATIC_V200_STRU;

#define NTB_RESET_REASON_CNT_MAX (10)
typedef struct
{
    HI_U32 reset_timestamp;
    HI_U32 reset_reason;
}DIAG_CMD_QUERY_NTB_RESET_REASON_ENTRY_STRU;

typedef struct
{
    HI_U32 timestamp;
    HI_U32 cycle_cnt;
}DIAG_CMD_QUERY_NTB_CYLCE_STRU;

typedef struct
{
    HI_U32 reset_cnt;
    DIAG_CMD_QUERY_NTB_RESET_REASON_ENTRY_STRU entry[NTB_RESET_REASON_CNT_MAX];
}DIAG_CMD_QUERY_NTB_RESET_REASON_STRU;

#define DFX_NTB_SLIDING_FILTERING_WND_SIZE (8)
typedef struct
{
    HI_U16 tei;
    HI_U16 level;

    HI_S16 freqerr_normal;
    HI_U16 is_sync_sta;

    HI_U32 CBTSn_front;
    HI_U32 LTmrn_front_08;
    HI_U32 LTmrn_front_32;
    HI_U32 CBTSn;
    HI_U32 LTmrn_08;
    HI_U32 LTmrn_32;
    HI_U32 timestamp;

    HI_S16 sliding_filtering_wnd[DFX_NTB_SLIDING_FILTERING_WND_SIZE];
}DIAG_CMD_QUERY_NTB_EXCEPTION_ENTRY_STRU;

typedef struct
{
    HI_U32 avail_pos;
    DIAG_CMD_QUERY_NTB_EXCEPTION_ENTRY_STRU entry[DFX_NTB_EXCEPTION_CNT_MAX];
}DIAG_CMD_QUERY_NTB_EXCEPTION_STRU;

typedef struct
{
    HI_U32 ret;
}DIAG_CMD_CLEAR_NTB_EXCEPTION_STRU;

typedef struct
{
    HI_U32 ntb_stat_snid;                       // ��ǰNTBͳ���ܱ�վ��������SNID��

    HI_U8  ntb_state;                           // ��ǰNTB���ڵ�״̬
    HI_U8  ntb_lock_state;                      // �Ƿ�����ͬ������
    HI_U8  rsv[2];

    HI_U32 ntb_synch_snid;                      // ����ͬ������������

    HI_U16 ntb_synch_tei;                       // ����ͬ�������TEI
    HI_U16 ntb_stat_tei;                        // ��ǰNTBͳ�Ƶ����TEI

    HI_U32 ntb_current_sys_time;                // ��ǰϵͳʱ��
    HI_U32 ntb_soft_control_begin_time;         // �������ʱ��ͬ����ʼʱ��
    HI_U32 ntb_last_synch_time;                 // NTB���ͬ��ʱ��
    HI_U32 ntb_lock_synch_station_begin_time;   // ����ͬ������ʼʱ��
    HI_U32 ntb_chip_control_begin_time;         // оƬ����ʱ��ͬ����ʼʱ��

    HI_U32 ntb_soft_control_len;                // �������ʱ��

    HI_S16 freqerr_current;                     // ��ǰƵƫ
    HI_S16 freqerr_front;                       // ��������ͳ�Ƶ�Ƶƫ
    HI_S16 freqerr_sdram;                       // �洢��FLASH�е�Ƶƫֵ
    HI_S16 pad;                                 // ��ǰƵƫ
}DIAG_CMD_QUERY_NTB_STATE_STRU;

#define NTB_MAX_FREQERR_CNT                     (36)
typedef struct
{
    HI_U32 freqerr[NTB_MAX_FREQERR_CNT];
}DIAG_CMD_QUERY_NTB_FREQERR_STAT_STRU;
#define NTB_SYNCH_STATION_STAT_NUM      (10)        // ͳ��ͬ������վ����Ϣ�Ļ���Ϊ10�飬ѭ��ʹ�û���
typedef struct
{
    HI_U16 synch_tei;           // ͬ��վ��
    HI_U8  synch_snid;          // ͬ�������
    HI_U8  chip_state;          // оƬʱ��ͬ��״̬
    HI_S16 max_freqerr;         // ͬ��Ƶƫ���ֵ;
    HI_S16 min_freqerr;         // ͬ��Ƶƫ��Сֵ
    HI_U32 synch_systime;       // ͬ��ϵͳʱ��

    HI_U8  reset_reason;        // ��λԭ��
    HI_U8  pad[3];
}DIAG_CMD_NTB_SYNCH_STATION_INFO_STRU;
typedef struct
{
    HI_U8  index;               // ��ǰͬ��վ��洢������
    HI_U8  pad[3];
    DIAG_CMD_NTB_SYNCH_STATION_INFO_STRU synch_station_stat[NTB_SYNCH_STATION_STAT_NUM];
} DIAG_CMD_QUERY_NTB_SYNCH_STATION_STRU;

typedef struct
{
    HI_U16 tei                      : 12;
    HI_U16 pad                      : 4;
    HI_S16 freqerr;                             // ����Ƶƫ

    HI_U32 snid;

    HI_U8  freq;
    HI_U8  pad1                     : 1;
    HI_U8  sync_status              : 3;        // ��ͬ�������ͬ��״̬
    HI_U8  freqerr_exception_cnt    : 4;        // Ƶƫ�쳣����
    HI_U16 pad2;

    HI_U32 last_update_timestamp;
}DIAG_NTB_NEIGHBOR_FREQERR_STRU;

#define NTB_MULTI_NET_CNT (32)                              //NTBģ��֧�ֵĶ��������
typedef struct
{
    DIAG_NTB_NEIGHBOR_FREQERR_STRU neighbor[NTB_MULTI_NET_CNT];
}DIAG_NTB_NEIGHBOR_FREQERR_STAT_STRU;

#define RX_STAT_NUM     (16)
typedef struct
{
    HI_U32 beacon_fc[RX_STAT_NUM];              // ͳ�ƽ��ղ�ͬ�����ű�֡FC����
    HI_U32 sof_fc[RX_STAT_NUM];                 // ͳ�ƽ��ղ�ͬ��������֡FC�ĸ���
}DIAG_CMD_QUERY_MULTI_NETWORK_RCV_STRU;

//V200���������ͳ��
typedef struct
{
    HI_U32 snid[RX_STAT_NUM];
    HI_U32 bcn_fc[RX_STAT_NUM];
    HI_U32 sof_fc[RX_STAT_NUM];
    HI_U32 last_rcv_time[RX_STAT_NUM];
    HI_U16 store_overflow_cnt;                      //�洢�������
    HI_U16 pad1;

    HI_U32 store_overflow_snid;                     //�洢��������һ�μ�¼��Snid
}DIAG_CMD_MULTI_NETWORK_RCV_V200_STRU;

typedef struct
{
    HI_U32  phase_a_total_rcv_cnt;              // A��λ�Ͻ���CCO BEACON֡�ܸ���
    HI_U32  phase_b_total_rcv_cnt;              // B��λ�Ͻ���CCO BEACON֡�ܸ���
    HI_U32  phase_c_total_rcv_cnt;              // C��λ�Ͻ���CCO BEACON֡�ܸ���

    HI_U32  phase_a_rcv_cnt_last;               // A��λ��ǰһ�����ڽ���CCO BEACON֡�ܸ���
    HI_U32  phase_b_rcv_cnt_last;               // B��λ��ǰһ�����ڽ���CCO BEACON֡�ܸ���
    HI_U32  phase_c_rcv_cnt_last;               // C��λ��ǰһ�����ڽ���CCO BEACON֡�ܸ���
    HI_U32  phase_a_channel_last;               // A��λ��ǰһ�������ŵ�����ֵ
    HI_U32  phase_b_channel_last;               // B��λ��ǰһ�������ŵ�����ֵ
    HI_U32  phase_c_channel_last;               // C��λ��ǰһ�������ŵ�����ֵ

    HI_U32  phase_a_rcv_cnt_current;            // A��λ�ϵ�ǰ���ڽ���CCO BEACON֡����
    HI_U32  phase_b_rcv_cnt_current;            // B��λ�ϵ�ǰ���ڽ���CCO BEACON֡����
    HI_U32  phase_c_rcv_cnt_current;            // C��λ�ϵ�ǰ���ڽ���CCO BEACON֡����
    HI_U32  phase_a_channel_current;            // A��λ�ϵ�ǰ�����ŵ�����ֵ
    HI_U32  phase_b_channel_current;            // B��λ�ϵ�ǰ�����ŵ�����ֵ
    HI_U32  phase_c_channel_current;            // C��λ�ϵ�ǰ�����ŵ�����ֵ

    HI_U32  evaluate_phase_a_cnt;               // ������A��λ����
    HI_U32  evaluate_phase_b_cnt;               // ������B��λ����
    HI_U32  evaluate_phase_c_cnt;               // ������C��λ����

    HI_U8   phase[3];                           // ��ǰ��������λ����
    HI_U8   frist_evaluate_cycle_is_end;        // ��һ�����������Ƿ����

    HI_S8   phase_a_snr;                        //a��λ��snr
    HI_S8   phase_b_snr;                        //b��λ��snr
    HI_S8   phase_c_snr;                        //c��λ��snr
    HI_BOOL phs_collect_info_finish;

    HI_U32   phase_a_send_success_cnt;                        //a��λ�Ϸ��ͳɹ��Ĵ���
    HI_U32   phase_b_send_success_cnt;                        //b��λ�Ϸ��ͳɹ��Ĵ���
    HI_U32   phase_c_send_success_cnt;                        //c��λ�Ϸ��ͳɹ��Ĵ���

    HI_U32   phase_a_send_fail_cnt;                        //a��λ�Ϸ���ʧ�ܵĴ���
    HI_U32   phase_b_send_fail_cnt;                        //b��λ�Ϸ���ʧ�ܵĴ���
    HI_U32   phase_c_send_fail_cnt;                        //c��λ�Ϸ���ʧ�ܵĴ���

    HI_U32   phase_a_last_send_success_cnt;                        //a��λ�Ϸ��ͳɹ��Ĵ���
    HI_U32   phase_b_last_send_success_cnt;                        //b��λ�Ϸ��ͳɹ��Ĵ���
    HI_U32   phase_c_last_send_success_cnt;                        //c��λ�Ϸ��ͳɹ��Ĵ���

    HI_U32   phase_a_last_send_fail_cnt;                        //a��λ�Ϸ���ʧ�ܵĴ���
    HI_U32   phase_b_last_send_fail_cnt;                        //b��λ�Ϸ���ʧ�ܵĴ���
    HI_U32   phase_c_last_send_fail_cnt;                        //c��λ�Ϸ���ʧ�ܵĴ���

    HI_U8    phase_a_set_send_cnt;                      //a��λ�趨�ķ��ʹ���
    HI_U8    phase_b_set_send_cnt;                      //b��λ�趨�ķ��ʹ���
    HI_U8    phase_c_set_send_cnt;                      //c��λ�趨�ķ��ʹ���
    HI_U8    pad1;
}DIAG_CMD_QUERY_PHASE_STATICS_STRU;

typedef struct
{
    HI_U32 nm_leave_sta_num;                                // CCOͳ������ָʾվ�����
    HI_U32 nm_send_delay_leave_pk_cnt;                      // CCO�����ӳ�����ָʾ���ĸ���
    HI_U32 nm_send_short_delay_leave_pk_fail_cnt;           // CCO�����ӳ�����ָʾ����ʧ�ܸ���
    HI_U32 nm_send_leave_ind_white_cnt;                     // CCO���Ͱ�������������ָʾ���Ĵ���
    HI_U32 nm_send_leave_ind_cnt;                           // CCO��������ָʾ���ĸ���
    HI_U32 nm_rcv_delay_leave_pk_cnt;                       // վ������ӳ�����ָʾ���ĸ���
    HI_U32 nm_rcv_last_delay_leave_pk_time;                 // վ���������ӳ�����ָʾ����ϵͳʱ��
    HI_U32 nm_handle_last_delay_leave_pk_time;              // վ�㴦���ӳ�����ָʾ����ϵͳʱ��
    HI_U32 nm_delay_leave_cnt;                              // վ���ӳ����ߴ���
    HI_U32 nm_delay_leave_time;                             // վ���ӳ�����ϵͳʱ��
    HI_U32 nm_handle_leave_ind_white_cnt;                   // վ�㴦��������߳�����ָʾ���Ĵ���
    HI_U32 nm_handle_leave_ind_cnt;                         // վ�㴦��ǰ���������ָʾ���Ĵ���
}DIAG_CMD_QUERY_DELAY_LEAVE_IND_INFO_STRU;

typedef struct
{
    HI_U32 create_proxy_change_req_cnt;                     // ���������������Ĵ���
    HI_U32 handle_proxy_change_cnf_cnt;                     // ����������ظ����Ĵ���
    HI_U32 handle_proxy_change_bitmap_cnf_cnt;              // �������ͼ�������ظ����Ĵ���
    HI_U32 proxy_change_timeout_cnt;                        // ����������ʱ����
    HI_U32 proxy_change_cnf_sequence_err_cnt;               // �������ظ�����쳣����

    HI_U16 candinate_proxy_tei[DFX_CHOSE_PROXY_SUM];        // ��ǰ��ѡ����
    HI_U16 proxy_tei;                                       // ��ǰ����վ��

    HI_U16 no_return_cnt;                                   // ��ǰ������δ���ش���
    HI_U8  proxy_changing;                                  // ��ǰ������״̬
    HI_U8  pad;
}DIAG_CMD_QUERY_PROXY_CHANGE_PK_STAT_STRU;

typedef struct
{
    HI_U32 nm_rcv_assoc_req_cnt;                    // ���չ����������
    HI_U32 nm_assoc_req_sequence_err_cnt;           // ����������Ŵ������
    HI_U32 nm_assoc_req_network_form_seq_err_cnt;   // ��������������Ŵ������
    HI_U32 nm_send_assoc_cnf_cnt;                   // ���͹����ظ�����
    HI_U32 nm_create_assoc_ind_cnt;                 // ���͹���ָʾ���ĸ���
    HI_U32 nm_create_assoc_gather_ind_cnt;          // ���ܱ��ĸ���
    HI_U32 nm_resend_assoc_ind_cnt;                 // �ظ����͹��������ĸ���
    HI_U32 nm_rcv_proxy_change_req;                 // ���մ������������
    HI_U32 nm_send_proxy_change_cnf;                // ���ʹ������ظ����ĸ���
    HI_U32 nm_send_proxy_change_bitmap_cnf;         // ����ͼ�������ظ����ĸ���
    HI_U32 nm_proxy_change_sequence_err_cnt;        // ������������Ŵ������
    HI_U32 nm_handle_proxy_change_fail_cnt;         // �������������ʧ�ܸ���
    HI_U32 nm_get_mac_buffer_fail_cnt;              // �����������MAC BUFFERʧ�ܴ���
    HI_U32 nm_send_msg_fail_cnt;                    // �������ģ�鷢����Ϣʧ�ܸ���
    HI_U32 nm_send_msg_enforcing_proxy_change_fail; // ����ǿ�ƴ�������Ϣʧ�ܼ���
}DIAG_CMD_QUERY_ASSOC_REQ_PK_STAT_STRU;

typedef struct
{
    HI_U32  rsb_send_event_cnt;          // ���ջ����������¼�����
    HI_U32  rsb_handle_event_cnt;        // ���������������¼�����
    HI_U32  rsb_handle_pk_cnt;           // �����������ĸ���
    HI_U32  rsb_reassemble_fail_cnt;     // ��������ʧ�ܸ���
    HI_U32  msdu_size_err_cnt;           // ���Ĵ�С�Ƿ�����
    HI_U32  rsb_icv_rcv_cnt;             // ����MACУ��ɹ�����
    HI_U32  rsb_not_my_pk_cnt;           // �Ǳ�վ�㱨�ĸ���
    HI_U32  rsb_filter_repeat_pk_cnt;    // ���Ĺ����ظ����ĸ���
    HI_U32  rsb_filter_rcv_pk_cnt;       // δ���˵��������ĸ���
    HI_U32  rsb_deliver_pk_fail_cnt;     // �������ַ�����ʧ�ܸ���
    HI_U32  rsb_deliver_pk_cnt;          // �������ַ����ĸ���
    HI_U32  rsb_list_pk_cnt;             // �����������б��ĸ���
    HI_U32  rsb_list_pk_max_cnt;         // �����������б��ķ�ֵ����
}DIAG_CMD_QUERY_RSB_STATICS_PK_STRU;

typedef struct
{
    HI_U8  all_queue_pk_cnt;               // �ֶζ��б�������
    HI_U8  queue0_pk_cnt;                  // ����0���ĸ���
    HI_U8  queue1_pk_cnt;                  // ����1���ĸ���
    HI_U8  queue2_pk_cnt;                  // ����2���ĸ���

    HI_U8  queue3_pk_cnt;                  // ����3���ĸ���
    HI_U8  upg_queue_cnt;                  // �������б��ĸ���
    HI_U16 msdu_len_err_cnt;               // msdu���ȴ���

    HI_U32 queue0_overflow_cnt;            // ����0�������
    HI_U32 queue1_overflow_cnt;            // ����1�������
    HI_U32 queue2_overflow_cnt;            // ����2�������
    HI_U32 queue3_overflow_cnt;            // ����3�������
    HI_U32 upg_queue_overflow_cnt;         // �������Ķ����������

    HI_U32 segment_sof_cnt;                // ��SOF֡�ֶδ���
    HI_U32 beacon_enqueue_cnt;             // �ű�֡��ֶζ��д���
    HI_U32 sounding_enqueue_cnt;           // SOUNDING֡����д���
    HI_U32 tx_set_is_sending_cnt;          // ���ͻ����ñ��Ĵ��ڷ���״̬����
    HI_U32 tx_set_no_send_cnt;             // ���ͻ����ñ��Ĵ��ڷǷ���״̬����
    HI_U32 tx_set_discard_cnt;             // ���ͻ����ö������Ĵ���

    HI_U32 detect_beacon_enqueue_cnt;      // ̽���ű�����д���
    HI_U32 segment_split_mac_fail_cnt;     // ���ķ�Ƭ�������
    HI_U32 segment_set_fc_fail_cnt;        // ��������FC�������
    HI_U32 segment_queue_overflow_cnt;     // ����������������

    HI_U32 pad;                            // ���ķֶγɹ�����
    HI_U32 send_event_to_tx_cnt;           // ֪ͨ���ͻ���������д���
}DIAG_CMD_QUERY_SEG_STATICS_PK_STRU;

typedef struct
{
    HI_U16 port;                                    // �˿ں�
    HI_U8  eth_mac[HI_ETH_MAC_ADDR_LEN];            // ��̫��MAC��ַ
    HI_U32 aging_time;                              // �ϻ�ʱ��

    HI_U32 rcv_cnt;                                 // ���ձ��ĸ���
    HI_U32 snd_cnt;                                 // ���ͱ��ĸ���
}DIAG_CMD_QUERY_PLC_SWITCH_MAPPING_STRU;
#define DFX_PLC_SWITCH_MAPPING_TABLE_ENTRY_MAX      (16)
typedef struct
{
   HI_U16 start_index;
   HI_U16 num;
   DIAG_CMD_QUERY_PLC_SWITCH_MAPPING_STRU entry[DFX_PLC_SWITCH_MAPPING_TABLE_ENTRY_MAX];
}DIAG_CMD_QUERY_PLC_SWITCH_MAPPING_TABLE_STRU;
typedef struct
{
    HI_U16 port;                                    // �˿ں�
    HI_U16 sequence;                                // �������

    HI_U8  fragment_num;                            // ��Ƭ��
    HI_U8  fragment_state;                          // ��Ƭ����״̬
    HI_U8  pad[2];

    HI_U32 expired_time;                            // ��Ƭ����ʱ��
}DIAG_CMD_QUERY_PLC_SWITCH_FRAGMENT_STRU;
#define DFX_PLC_SWITCH_FRAGMENT_QUEUE_ENTRY_MAC     (16)
typedef struct
{
   HI_U16 start_index;
   HI_U16 num;

   DIAG_CMD_QUERY_PLC_SWITCH_FRAGMENT_STRU entry[DFX_PLC_SWITCH_FRAGMENT_QUEUE_ENTRY_MAC];
} DIAG_CMD_QUERY_PLC_SWITCH_FRAGMENT_QUEUE_STRU;

typedef struct
{
    HI_U32 get_port_fail_cnt;                       // ��ȡ�˿�ʧ�ܸ���
    HI_U32 snd_broadcast_cnt;                       // ���͹㲥���ĸ���
    HI_U32 snd_unicast_cnt;                         // ���͵������ĸ���
    HI_U32 get_port_fail_broadcast_cnt;             // ӳ��δ�������㲥���ĸ���
    HI_U32 fragment_cnt[4];                         // ��Ƭ����
    HI_U32 get_mac_buffer_fail;                     // ��ȡMAC�㻺��ʧ�ܸ���
    HI_U32 switch_not_start_pk_cnt;                 // ������δ����ʱ�����ｻ�����ı��ĸ���
    HI_U32 rcv_broadcast_cnt;                       // ���չ㲥���ĸ���
    HI_U32 rcv_unicast_cnt;                         // ���յ������ĸ���
    HI_U32 fragment_rsb_success_cnt[3];             // ����ɹ�����
    HI_U32 fragment_rsb_fail_cnt[3];                // ����ʧ�ܸ���
    HI_U32 get_fragment_entry_fail_cnt;             // ��ȡ��Ƭ����ʧ�ܴ���
    HI_U32 send_to_eth_success_cnt;                 // ���͵���̫���ڳɹ�����
    HI_U32 send_to_eth_fail_cnt;                    // ���͵���̫����ʧ�ܸ���
    HI_U32 eth_data_len_invalid_cnt;                // ��̫�����ĳ����쳣����
    HI_U32 send_to_net_protocol_stack_cnt;          // ���͵���������Э��ջ�ı��ĸ���
    HI_U32 send_to_plc_link_layer_cnt;              // ת������������·�ϱ��ĸ���
    HI_U32 multicast_pk_cnt;                        // �鲥���ĸ���
    HI_U32 tx_get_switch_cnt;                       // ���ͻ���ȡ�������ĸ���
    HI_U32 tx_send_switch_success;                  // ���ͻ����ͽ������ĳɹ�����
    HI_U32 tx_send_switch_timeout;                  // ���ͻ����ͽ������ĳ�ʱ����
    HI_U32 tx_send_switch_fail;                     // ���ͻ����ͽ�������ʧ�ܸ���
    HI_U32 tx_send_switch_resend;                   // ���ͻ����ͽ��������ش�����
    HI_U32 cvg_to_tx_eth_packet_count;              // ��۽������ͻ���ETH���ĸ���
    HI_U32 rx_to_cvg_eth_packet_count;              // ���ջ�������۵�ETH���ĸ���
}DIAG_CMD_QUERY_PLC_SWITCH_TRX_STAT_STRU;

typedef struct
{
    HI_U32 arp_request_cnt;             // ARP�����ĸ���
    HI_U32 arp_response_cnt;            // ARP��Ӧ���ĸ���

    HI_U32 ip_cnt;                      // IP���ĸ���
    HI_U32 ohter_three_layer_cnt;       // ��������Э�鱨��

    HI_U32 tcp_cnt;                     // TCP���ĸ���
    HI_U32 udp_cnt;                     // UDP���ĸ���
    HI_U32 icmp_cnt;                    // ICMP���ĸ���
    HI_U32 igmp_cnt;                    // IGMP���ĸ���
    HI_U32 other_four_layer_cnt;        // �Ĳ�����Э�鱨��
}DIAG_CMD_QUERY_ETH_PACKET_STAT_STRU;

typedef struct
{
    DIAG_CMD_QUERY_ETH_PACKET_STAT_STRU eth_link_layer;
    DIAG_CMD_QUERY_ETH_PACKET_STAT_STRU plc_link_layer;
}DIAG_CMD_QUERY_PLC_SWITCH_ETH_PACKET_STAT_STRU;

typedef struct
{
    HI_U32 proxy_send_pk_cnt;           // �����ͱ��ĸ���
    HI_U32 cco_receive_pk_cnt;          // CCO���ձ��ĸ���
}DIAG_CMD_QUERY_COMM_RATE_STRU;

typedef struct
{
    HI_BOOL is_set_afe_power;           // �Ƿ�����ģ�⹦��
    HI_BOOL is_set_digital_power;       // �Ƿ��������ֹ���
    HI_BOOL pad;                // �Ƿ�д��NV��
    HI_U8   afe_pwoer;                  // ģ�⹦��

    HI_S8   digital_power[4];           // ���ֹ���
    HI_U32  delay_effective_time;       // �ӳ���Чʱ�䣬��λS
}DIAG_CMD_SET_DELAY_POWER_CHANGE_STRU;

typedef struct
{
    HI_BOOL is_set_digital_power_by_freq;       // �Ƿ��������ֹ���
    HI_BOOL pad;                // �Ƿ�д��NV��
    HI_S8   digital_power_by_freq[26];           // ���ֹ���

    HI_U32  delay_effective_time;       // �ӳ���Чʱ�䣬��λS
}DIAG_CMD_SET_DELAY_POWER_CHANGE_BY_FREQ_STRU;

typedef struct
{
    HI_U32 delayed_restart_time;        // �ӳ�����ʱ�䣬��λS
}DIAG_CMD_DELAYED_RESTART_STRU;

typedef struct
{
    HI_U32 delay_time;
}DIAG_CMD_DELAY_SET_IND_STRU;

typedef struct
{
    HI_U32 ret;
    HI_U32 delay_time;
}DIAG_CMD_SET_SWITCH_FREQ_IND_STRU;

typedef struct
{
    HI_U8 switch_freq;                 // �л�����Ƶ��
    HI_U8 pad[3];

    HI_U32 delay_switch_freq_time;     // �ӳ��л�Ƶ��ʱ�䣬��λS
}DIAG_CMD_SET_SWITCH_FREQ_STRU;

typedef struct
{
    HI_U32 count[10];
}DIAG_CMD_COMMON_TEMP_COMMAND_STRU;

/**
 *  �����Ϣ ID_DIAG_LAYER_PS_NM_FORMING
 *
 */
#define DFX_BUSINESS_LOG_LEN_MAX 256
typedef struct
{
    HI_CHAR log[DFX_BUSINESS_LOG_LEN_MAX];
}DIAG_LAYER_PS_NM_FORMING_STRU;

/**
 *  �����Ϣ DIAG_LAYER_PS_NM_COORDINATE_STRU
 *
 */
typedef struct
{
    HI_CHAR log[DFX_BUSINESS_LOG_LEN_MAX];
}DIAG_LAYER_PS_NM_COORDINATE_STRU;

/**
 *  �����Ϣ ID_DIAG_LAYER_PS_ROUTE
 *
 */
typedef struct
{
    HI_CHAR log[DFX_BUSINESS_LOG_LEN_MAX];
}DIAG_LAYER_PS_ROUTE_STRU;

/**
 *  �����Ϣ ID_DIAG_LAYER_PS_NM_CHANGEPROXY
 *
 */
typedef struct
{
    HI_CHAR log[DFX_BUSINESS_LOG_LEN_MAX];
}DIAG_LAYER_PS_NM_CHANGEPROXY_STRU;

/**
 *  �����Ϣ ID_DIAG_LAYER_PS_ROUTE_AGING
 *
 */
typedef struct
{
    HI_CHAR log[DFX_BUSINESS_LOG_LEN_MAX];
}DIAG_LAYER_PS_ROUTE_AGING_STRU;

/**
 *  �����Ϣ ID_DIAG_LAYER_PS_ROUTE_FEEDBACK
 *
 */
typedef struct
{
    HI_CHAR log[DFX_BUSINESS_LOG_LEN_MAX];
}DIAG_LAYER_PS_ROUTE_FEEDBACK_STRU;

/**
 *  �����Ϣ ID_DIAG_LAYER_PS_ROUTE_DETECT
 *
 */
typedef struct
{
    HI_CHAR log[DFX_BUSINESS_LOG_LEN_MAX];
}DIAG_LAYER_PS_ROUTE_DETECT_STRU;

/**
 *  �����Ϣ ID_DIAG_LAYER_PS_ROUTE_EVALUATE
 *
 */
typedef struct
{
    HI_CHAR log[DFX_BUSINESS_LOG_LEN_MAX];
}DIAG_LAYER_PS_ROUTE_EVALUATE_STRU;

typedef struct
{
    HI_U32 tx; // ��������
    HI_U32 rx; // ��������
}DIAG_CMD_REPORT_TXRX_STATIC_SPEED_STRU;

//�����쳣ͳ��
typedef struct
{
    HI_U32 time_stamp;                  //ʱ���
    HI_U32 ntb;                         //ntbʱ���

    HI_U16 mac_rx_ip_pre_cnt;           // MAC�ϸ�ͳ�����ڽ���IP���ĸ���
    HI_U16 mac_tx_ip_pre_cnt;           // MAC�ϸ�ͳ�����ڷ���IP���ĸ���
    HI_U16 mac_rx_ip_cur_cnt;           // MAC��ǰͳ�����ڽ���IP���ĸ���
    HI_U16 mac_tx_ip_cur_cnt;           // MAC��ǰͳ�����ڷ���IP���ĸ���

    HI_U16 eth_rx_ip_pre_cnt;           // ��̫���ϸ�ͳ�����ڽ���IP���ĸ���
    HI_U16 eth_tx_ip_pre_cnt;           // ��̫���ϸ�ͳ�����ڷ���IP���ĸ���
    HI_U16 eth_rx_ip_cur_cnt;           // ��̫����ǰͳ�����ڽ���IP���ĸ���
    HI_U16 eth_tx_ip_cur_cnt;           // ��̫����ǰͳ�����ڷ���IP���ĸ���

    HI_U32 eth_rx_ip_abnormity_cnt;     // ��̫������IP�����쳣����
    HI_U32 eth_tx_ip_abnormity_cnt;     // ��̫������IP�����쳣����
    HI_U32 mac_rx_ip_abnormity_cnt;     // MAC�����IP�����쳣����
    HI_U32 mac_tx_ip_abnormity_cnt;     // MAC�㷢��IP�����쳣����

    HI_U16 eth_used_buff_cnt;           // ��̫�����õ�bufferֵ
    HI_U16 eth_tx_err_cnt;              // ��̫������ʧ�ܵĴ���

    HI_U16 itrupt_cnt;                  // �жϸ���
    HI_U16 pcs_itrupt_cnt;              // ǰ���жϸ���
    HI_U16 fc_itrupt_cnt;               // FC�жϸ���
    HI_U16 pb_itrupt_cnt;               // PB���жϸ���

    HI_U16 rx_end_itrupt_cnt;           // ��������жϸ���
    HI_U16 tx_end_itrupt_cnt;           // ��������жϸ���

    HI_U16 enable_itrupt_cnt;           // ʹ���жϸ���
    HI_U16 disable_itrupt_cnt;          // �ر��жϸ���

    HI_U16 rx_fc_success_cnt;           // ����FC�ɹ�����
    HI_U16 rx_short_pb_success_cnt;     // ���ն�PB��ɹ�����
    HI_U16 rx_long_pb_success_cnt;      // ���ճ�PC��ɹ�����
    HI_U16 tx_short_pb_cnt;             // ���Ͷ�PB�����

    HI_U16 tx_long_pb_cnt;              // ���ͳ�PB�����
    HI_U16 tx_msdu_success_cnt;         // ����MSDU�ɹ�����
}DIAG_CMD_SPEED_ABNORMITY_STATISTIC_STRU;

//�ա��������ڼ���
typedef struct
{
    HI_U32 mac_rx_ip_cnt;       //MAC�ա���IP������ͳ��
    HI_U32 mac_tx_ip_cnt;

    HI_U32 eth_rx_ip_cnt;       //��̫���ա���IP������ͳ��
    HI_U32 eth_tx_ip_cnt;

    HI_U32 itrupt_cnt;
    HI_U32 pcs_itrupt_cnt;
    HI_U32 fc_itrupt_cnt;
    HI_U32 pb_itrupt_cnt;

    HI_U32 rx_end_itrupt_cnt;
    HI_U32 tx_end_itrupt_cnt;

    HI_U32 enable_itrupt_cnt;
    HI_U32 disable_itrupt_cnt;

    HI_U32 rx_fc_success_cnt;
    HI_U32 rx_short_pb_success_cnt;
    HI_U32 rx_long_pb_success_cnt;
    HI_U32 tx_short_pb_cnt;

    HI_U32 tx_long_pb_cnt;
    HI_U32 tx_msdu_success_cnt;
}DFX_IP_PACKETS_STATISTIC_STRU;

//MACϵͳ�쳣ͳ��
typedef struct
{
    HI_U32 malloc_err_cnt;              //�ڴ�������
    HI_U32 mfree_err_cnt;               //�ڴ��ͷŴ���
}DIAG_CMD_QUERY_MAC_SYS_ERR_STRU;
typedef struct
{
    HI_U32 bcn_soft_crc_err_cnt;        // �ű�֡���У��ʧ�ܴ���
    HI_U32 beacon_snid_err_cnt;         // �ű�֡������쳣����
    HI_U32 sof_crc_err_cnt;             // SOF֡MACУ��ʧ�ܴ���
    HI_U32 sof_snid_err_cnt;            // SOF֡������쳣����
    HI_U32 beacon_my_snid_zero_cnt;     // ��Ƶ�κ�����my snid=0 beaconͳ��
    HI_U32 sof_my_snid_zero_cnt;        // ��Ƶ�κ�����my snid=0 sofͳ��
    HI_U16 parse_bcn_entry_err_cnt;     // �����ű���Ŀ����
    HI_U16 bcn_belen_err_cnt;           // �ű���Ŀ���ȴ���
    HI_U32 sof_detect_snid_err_cnt;     // ̽��Э��sof��snid���˴���
    HI_U32 rcv_my_bcn_cnt;              // ���ձ�������ű����
} DIAG_CMD_QUERY_FC_PB_FAULT_STRU;

#define RECODE_TX_NOT_WAIT_RESPONSE                             (1)
#define RECODE_TX_TIMEOUT                                       (2)
#define RECODE_TX_SACK_FAIL                                     (3)
#define RECODE_TX_SACK_SUCCESS                                  (4)

#define RECODE_TX_FINAL_RESULT                                  (1)
#define RECODE_TX_REAL_TIME_RESULT                              (2)

#define RECODE_TX_STAT_BPC(tx_recode,bpc_value)                 tx_recode.bpc = bpc_value
#define RECODE_TX_STAT_DTEI(tx_recode,dtei_value)               tx_recode.dtei = dtei_value
#define RECODE_TX_STAT_DT(tx_recode,dt_value)                   tx_recode.dt = dt_value
#define RECODE_TX_STAT_NTB(tx_recode,ntb_value)                 tx_recode.ntb = ntb_value
#define RECODE_TX_STAT_PBCNT(tx_recode,pb_cnt_value)            tx_recode.pb_cnt = pb_cnt_value
#define RECODE_TX_STAT_SEND_RESULT(tx_recode,send_result_value) tx_recode.send_result = send_result_value
#define RECODE_TX_STAT_FINAL_FLAG(tx_recode,final_flag_value)   tx_recode.final_flag = final_flag_value
#define RECODE_TX_STAT_TMI(tx_recode,tmi_value)                 tx_recode.tmi = tmi_value
#define RECODE_TX_STAT_RESEND_CNT(tx_recode,resend_cnt_value)   tx_recode.resend_cnt = resend_cnt_value
#define RECODE_TX_STAT_MTYPE(tx_recode,mtype_value)             tx_recode.mtype = mtype_value
#define RECODE_TX_STAT_MMTYPE(tx_recode,mmtype_value)           tx_recode.mmtype = mmtype_value


#define RECODE_RX_STAT_DISABLE                               (0)
#define RECODE_RX_STAT_ENABLE                                (1)

#define RECODE_RX_STAT_VALID_FLAG                            (1)
#define RECODE_RX_STAT_INVALID_FLAG                          (2)

#define RECORD_RX_STAT_BPC(rx_record,bpc_value)               rx_record.bpc = bpc_value
#define RECORD_RX_STAT_NTB(rx_record,ntb_value)               rx_record.ntb = ntb_value
#define RECORD_RX_STAT_SNR(rx_record,snr_value)               rx_record.snr = snr_value
#define RECORD_RX_STAT_PAR(rx_record,par_value)               rx_record.par = par_value
#define RECORD_RX_STAT_CUBIC(rx_record,cubic_value)           rx_record.cubic = cubic_value
#define RECORD_RX_STAT_DT(rx_record,dt_value)                 rx_record.dt  =  dt_value
#define RECORD_RX_STAT_SNID(rx_record,snid_value)             rx_record.snid = snid_value
#define RECORD_RX_STAT_CHANNEL(rx_record,channel_value)       rx_record.channel = channel_value
#define RECORD_RX_STAT_STEI(rx_record,stei_value)             rx_record.stei = stei_value
#define RECORD_RX_STAT_DTEI(rx_record,dtei_value)             rx_record.dtei = dtei_value
#define RECORD_RX_STAT_PB_CNT(rx_record,pb_cnt_value)         rx_record.pb_cnt = pb_cnt_value
#define RECORD_RX_STAT_PB_SIZE(rx_record,pb_size_value)       rx_record.pb_size = pb_size_value
#define RECORD_RX_STAT_PB_CRC_FAIL(rx_record)                 rx_record.pb_crc_fail_cnt++
#define RECORD_RX_STAT_PB_CRC_SUCCESS(rx_record)              rx_record.pb_crc_success_cnt++
#define RECODE_RX_STAT_VALID(rx_record)                       rx_record.flag = RECODE_RX_STAT_VALID_FLAG
#define RECODE_RX_STAT_INVALID(rx_record)                     rx_record.flag = RECODE_RX_STAT_INVALID_FLAG
#define RECODE_RX_STAT_MTYPE(rx_record,mtype_value)           rx_record.mtype = mtype_value
#define RECODE_RX_STAT_MMTYPE(rx_record,mmtype_value)         rx_record.mmtype = mmtype_value
#define RECODE_RX_STAT_OSTEI(rx_record,ostei_value)           rx_record.ostei = ostei_value
#define RECODE_RX_STAT_ODTEI(rx_record,odtei_value)           rx_record.odtei = odtei_value

typedef struct
{
    HI_U32 snd_ntb;
    HI_U16 odtei;                   // ����Ŀ��վ��
    HI_U16 ostei;                   // ����Դվ��

    HI_U16 dtei;                    // ��һ��
    HI_U16 packet_index;            // �������

    HI_U16 msdu_size;               // ���Ĵ�С
    HI_U16 packet_type;             // �������ͣ��������ġ�Ӧ�ò㱨�ġ���������ġ�IP����

    HI_U8  result;                  // ���ͽ�������ͳɹ�������ʧ�ܡ����Ĺ��ڡ��ش�����
    HI_U8  lid;
    HI_U8  pad1;
    HI_U8  broadcast_flag;

    HI_U16 ptcl_ver;
    HI_U16 pad;
}DIAG_CMD_TX_REPORT_STRU;

typedef struct
{
    HI_U32 para[12];
}DIAG_CMD_PLC_SWITCH_STRU;

typedef struct
{
    HI_U32 rcv_ntb;
    HI_U16 ostei;                   // ����Դ��ַ
    HI_U16 odtei;                   // ����Ŀ�ĵ�ַ

    HI_U16 dtei;                    // ������һ����ַ
    HI_U16 stei;                    // ���ķ��͵�ַ

    HI_U16 msdu_size;               // ���Ĵ�С
    HI_U16 packet_type;             // ��������

    HI_U16 packet_index;
    HI_U8  lid;
    HI_U8  broadcast_flag;

    HI_U16 ptcl_ver;
    HI_U16 pad;
}DIAG_CMD_RX_REPORT_STRU;

typedef struct
{
    HI_U32 ntb;                         //���յ��ñ���ʱ��NTB

    HI_U32  snid;

    HI_U8  dt;
    HI_U8  pad;
    HI_U16 stei;

    HI_U8  freq;
    HI_U8  attenuation;
    HI_U8  tmi;
    HI_U8  fc_crc;
    HI_U8   pb_cnt;
    HI_U8   phy_detect_ver;
    HI_U16 preamle_sat_num;

    HI_U8  pb_crc[4];

    HI_U8  fc_channel_quality;
    HI_S8  snr;
    HI_U8  pb_channel_quality[4];
    HI_U16 fc_ber;
    HI_U16 pb_ber[4];
    HI_U32 fc[4];
    HI_U32 bpc;
}DIAG_CMD_RCV_INFO_REPORT_STRU;

typedef struct
{
    HI_U16 pb_index;
    HI_U16 pb_size;

    HI_U32 curr_ntb;

    HI_U32 pb_addr[130];
}DIAG_CMD_RCV_PB_REPORT_STRU;

typedef struct
{
    HI_U16 pb_index;
    HI_U16 pb_size;

    HI_U32 pb_addr[18];
}DIAG_CMD_RCV_PB72_REPORT_STRU;
typedef struct
{
    HI_U16 pb_index;
    HI_U16 pb_size;

    HI_U32 pb_addr[34];
}DIAG_CMD_RCV_PB136_REPORT_STRU;
typedef struct
{
    HI_U16 pb_index;
    HI_U16 pb_size;

    HI_U32 pb_addr[66];
}DIAG_CMD_RCV_PB264_REPORT_STRU;


typedef struct
{
    HI_U8  dt;
    HI_U8  tmi;
    HI_U16 dtei;
    HI_U32 snd_ntb;
    HI_U32 bpc;
}DIAG_CMD_SND_INFO_REPORT_STRU;
//UIUģ�����Ϣ��ӡ
typedef struct
{
    HI_U32 para1;
    HI_U32 para2;
    HI_U32 para3;
    HI_U32 para4;
    HI_U32 para5;
}DIAG_CMD_PTPC_REPORT_STRU;

//״̬�л�����Ϣ
typedef struct
{
    HI_U8  old_state;               //��״̬
    HI_U8  new_state;               //��״̬
    HI_U16 msg_id;                  //��������Ϣ�Ķ���ID

    HI_U32 pad[4];
}DIAG_CMD_PTPC_STATE_CHANGE_STRU;
//��PHY���յ����ĵ���Ϣ
typedef struct
{
    HI_U8 oda[HI_PLC_MAC_ADDR_LEN];
    HI_U8 osa[HI_PLC_MAC_ADDR_LEN];

    HI_U16 pkt_size;                    //���ĳ���
    HI_U16 pad;
}DIAG_CMD_PTPC_APP_PKT_FROM_PHY_STRU;


//��APP���յ����ĵ���Ϣ
typedef struct
{
    HI_U8 oda[HI_PLC_MAC_ADDR_LEN];
    HI_U8 osa[HI_PLC_MAC_ADDR_LEN];

    HI_U16 pkt_size;                    //���ĳ���
    HI_U16 pad;
}DIAG_CMD_PTPC_APP_PKT_FROM_APP_STRU;

//��������֡����Ϣ
typedef struct
{
    HI_U8  send_search_frame_cnt;                               //��������֡�ļ���
    HI_U8  sweep_freq_index;                                    // ��ǰɨƵƵ������
    HI_U8  search_frequency_range_cnt;                          // ����Ƶ�δ���
    HI_U8  active_search_state;                                 // ��������״̬

    HI_U8  cur_freq;                                            //��ǰ�Ĺ���Ƶ��
    HI_U8  cur_snid;
    HI_U8  ntb_auto_sync_state;                                 //NTB�Զ�ͬ����״̬
    HI_U8  pad;
}DIAG_CMD_PTPC_SEND_SEARCH_FRAME_STRU;

//��������֡����Ϣ
typedef struct
{
    HI_BOOL is_response;                        // �Ƿ���Ҫ��Ӧ
    HI_BOOL is_sweep;                           // �Ƿ�ɨƵ:0.δɨƵ������ͬ��״̬��1.ɨƵ��δͬ��
    HI_U8  mac[HI_PLC_MAC_ADDR_LEN];

    HI_U8  req_snid;
    HI_U8  rsp_snid;
    HI_U8  network_state;                       // 0:����,1:����
    HI_U8  ntb_synch_state;                     // 0:δͬ��,1:��ͬ��

    HI_U16 stei;                                // վ��TEI
    HI_U8  pad[2];
}DIAG_CMD_PTPC_RCV_SEARCH_FRAME_STRU;

//����ͬ���ű�֡����Ϣ
typedef struct
{
    HI_U8  ntb_synch_state;                     //NTBͬ��״̬
    HI_U8  sta_state;                           //STAͬ��״̬

    HI_U8 synch_mac[HI_PLC_MAC_ADDR_LEN];       //ͬ���ű�֡�е�ͬ��MAC

    HI_BOOL is_synch_frame_sweep;               //�Ƿ����ɨƵ�����ű�
    HI_U8  pad;
}DIAG_CMD_PTPC_SEND_SYNC_FRAME_STRU;

typedef struct
{
    HI_U32 config[8];
}DIAG_CMD_MAC_CONFIG_SW_STRU;

#define DFX_NDM_CMD_MAC_R_NUM 10
typedef struct
{
    HI_U8 scope; // 1: ȫ��, 2: ����
    HI_U8  pad;
    HI_U8 lockedProxyAddr[6];
    HI_U32 code; // ���ڷ������ý��
}DIAG_CMD_NDM_CTRL_ROUTE_LOCK_STRU; // ·������

typedef struct
{
    HI_U8 scope; // 1: ȫ��, 2: ����
    HI_U8 pad[3];
    HI_U32 code; // ���ڷ������ý��
}DIAG_CMD_NDM_CTRL_ROUTE_UNLOCK_STRU; // ·�ɽ���

typedef struct
{
    HI_U8 oda[HI_PLC_MAC_ADDR_LEN];
    HI_U8 next[HI_PLC_MAC_ADDR_LEN];

}DIAG_CMD_NDM_CTRL_ROUTE_CONFIG_STRU;// ��̬·������

typedef struct
{
    HI_U8 echo[256];
}DIAG_CMD_NDM_CTRL_TRACE_STRU;//  trace ����

//��һ��PB�������£�������ͨ�����ܳ��ص�����ĳ���
//(LONG_PBB_SIZE - MAC_FRAME_HEADER_SIZE - MAC_ICV_SIZE - MM_HEAD_SIZE - sizeof(dfx_ndm_packet_head_st))
#define DIAG_CMD_NDM_CTRL_PING_PK_MAX_LEN (388)//����Ϊ1��PB�Ŀ��ô�С

typedef struct
{
    HI_U32 ulLen; // ��ʾecho�εĳ��ȣ� ����㱣֤4�ֽڶ���
    HI_U8 echo[0];
}DIAG_CMD_NDM_CTRL_PING_STRU; //  ping ���� �� �����֧�������в���.

typedef struct
{
    HI_U32 nuk;
}DIAG_CMD_NDM_CTRL_SEARCH_DEV_STRU; //  �����豸

typedef struct
{
    HI_U8 echo1;
    HI_U8 echo2;
    HI_U16 ack;
}DIAG_CMD_NDM_CTRL_PIPE_ACK_STRU;

typedef struct
{
    HI_U8    network_status;                                        // ����״̬��δ����״̬��������״̬
    HI_U8    multi_network_status;                                  // ������״̬���쳣״̬������״̬
    HI_U8    search_meter_status;                                   // �ѱ�״̬���ѱ��к��ѱ����
    HI_U8    read_meter_addr_status;                                // �����ַ״̬����ȡ�ɹ��Ͷ�ȡʧ��

    HI_U8    sys_test_status;                                       // �Ƿ����ڹ�װģʽ
    HI_U8    pad[3];

    HI_U32   set_not_join_network_read_meteradd_fail_led_cnt;
    HI_U32   set_read_meter_fail_led_cnt;
    HI_U32   set_not_join_network_led_cnt;
    HI_U32   set_multi_network_exception_led_cnt;
    HI_U32   set_join_network_led_cnt;
    HI_U32   led_set_485_led_cnt;
    HI_U16  led_not_join_and_read_meter_fail_timer_start_fail_cnt;  // δ�������Ҷ����ַʧ��timer����ʧ��
    HI_U16  led_read_meter_fail_timer_start_fail_cnt;               // ��ȡ����ַtimer����ʧ��

    HI_U8   led_rx_sys_test_timer_start_fail_cnt;                   // ��װģʽ�£�����STA��ƶ�ʱ��ʧ�ܴ���
    HI_U8   led_set_sta_test_status_cnt;                            // ��װģʽ�£�����STA��ƴ���
    HI_U8   led_set_CLTII_test_status_cnt;                          // ��װģʽ�£�����II�ɵ�ƴ���
    HI_U8   led_set_ndm_test_status_cnt;                            // ��װģʽ�£����ó��������
}DIAG_CMD_LED_STATUS_STRU;

//HSO�·���������Ƶ�������Ľӿ�
typedef struct
{
   HI_U8 mode;
   HI_U8 pad[3];
}DIAG_CMD_EVALUATE_FREQ_REQ_STRU;

//HSO�·������������������
typedef struct
{
   HI_U32 ret_code;         //������
   HI_U32 total_time;       //��ǰģʽ������Ҫռ�õ�ʱ�䣬��λs
}DIAG_CMD_EVALUATE_FREQ_RSP_STRU;

//HSO���Ƶ�������Ľ��
typedef struct
{
   HI_U32 ret_code;         //������
}DIAG_CMD_CLEAR_EVALUATE_FREQ_RESULT_RSP_STRU;

//HSO��ѯ��ǰƵ��������״̬
typedef struct
{
    HI_U8  evaluate_mode;                   //Ƶ��������ģʽ
    HI_U8  evaluate_reason;                 //��ǰ����������ԭ��
    HI_U16  ret_code;

    HI_U32 run_time;                        //��ģʽ�����е�ʱ��,��λ:s
    HI_U32 total_evaluate_time;             //��ǰ������Ҫ����ʱ��,��λ:s
}DIAG_CMD_QUERY_EVALUATE_FREQ_RSP_STRU;

//Ƶ������Ӧģ���ͳ����
typedef struct
{
    HI_U32 pad;
    HI_U32 hso_set_cnt;

    HI_U32 realtime_start_cnt;                  //��ʱ���������Ĵ���
    HI_U32 full_start_success_cnt;
    HI_U32 full_start_consume_cnt;
    HI_U32 full_start_after_consume_cnt;

    HI_U32 full_start_white_list_change_cnt;
    HI_U32 auto_start_success_cnt;
    HI_U32 auto_start_fail_cnt;
    HI_U32 stop_success_evaluate_cnt;

    HI_U32 stop_fail_evaluate_inner_cnt;
    HI_U32 stop_fail_evaluate_outer_cnt;

    HI_U32 affirm_network_state_fail_cnt;       //ȷ������״̬ʧ��
    HI_U32 auto_to_full_cnt;                    //�Լ첻���㴥����������Ĵ���
    HI_U32 realtime_start_fail_cnt;
}FA_STATISTIC_INFO_STRU;

//Ƶ������Ӧģ���ͳ����
typedef struct
{
    FA_STATISTIC_INFO_STRU stat_info;

    HI_U8  cur_evaluate_mode;                                   //��ǰ������ģʽ
    HI_U8  cur_freq;                                            //��ǰƵ��, ��ֵ��һ���׶ο���Ϊ��Ҫ������Ƶ��
    HI_U8  init_freq;                                           //��������ǰ�Ĺ���Ƶ��
    HI_U8  freq_evaluated_cnt;                                  //��������Ƶ����

    HI_U8  evaluate_state;                                      //��ǰ������״̬
    HI_U8  start_reason;                                        //��������Ƶ��������ԭ��
    HI_U8  stop_reason;                                         //����ֹͣƵ��������ԭ��
    HI_U8  affirm_network_state_cnt;                            //ȷ���������״̬�Ĵ���

    HI_U32 start_time_stamp;                                    //����ʱ�䣬��λ:ms
    HI_U32 toatl_run_time;                                      //����ģʽ��Ҫ��������ʱ�䣬��λ:s
    HI_U32 stop_time_stamp;                                     //ʵ�ʽ���ʱ�䣬��λ:ms
    HI_U32 sta_join_cnt;                                        //���������վ����ͳ��

    HI_U16 node_num;                                            //��CCO���ȫ��վ����
    HI_U16 ping_send_node_cnt;                                  //�����ѷ��Ͳ��Ա��ĵ�վ����

    HI_U16 ping_next_tei;                                       //��һ�β��Ե���ʼվ��TEI����
    HI_U8  ping_turn_cnt;                                       //ping�����Ե��ִμ���
    HI_U8  auto_fail_cnt;                                       //�Լ�����������ָ��Ĵ���

    //HI_MAC_FA_FREQ_RESULT_STRU freq_result[3];                //Ƶ�������Ľ��
    HI_MAC_FA_FREQ_RESULT_STRU freq_result[FREQ_TRY_NUM];       //Ƶ�������Ľ��
}DIAG_CMD_FA_STATISTIC_INFO_STRU;

// ̨��ʶ�������������:
typedef enum
{
    HI_MAC_TF_IDENTIFY_CMD_UNKNOWN = 0,
    HI_MAC_TF_IDENTIFY_CMD_CCO_ALL_MODE_START,          //����CCOȫ��ʶ��ģʽ
    HI_MAC_TF_IDENTIFY_CMD_CCO_PART_MODE_START,         //����CCOָ��վ��ʶ��ģʽ

    HI_MAC_TF_IDENTIFY_CMD_STOP = 5,                    //ֹ̨ͣ��ʶ��ģʽ
    HI_MAC_TF_IDENTIFY_CMD_CLEAR,                       //���̨��ʶ����
    HI_MAC_TF_IDENTIFY_CMD_STOP_SEQ,                    //ֹ̨ͣ��ʶ�����з���
    HI_MAC_TF_IDENTIFY_CMD_MAX
}HI_MAC_TF_IDENTIFY_CMD_E;

// ̨��ʶ����������ṹ����
typedef struct _DIAG_CMD_TF_IDENTIFY_SET_REQ_STRU_
{
    HI_MAC_TF_IDENTIFY_CMD_E cmd;           //̨��ʶ������

    HI_U16 identifyTime;                        //̨��ʶ�����ʱ�䣬��λmin����ʱ�����sendSeqTime
    HI_U16 sendSeqTime;                         //���������ʱ�䣬��λmin

    HI_U32 pad[4];
}DIAG_CMD_TF_IDENTIFY_SET_REQ_STRU;

// ̨��ʶ������붨������:
typedef enum
{
    HI_MAC_TF_IDENTIFY_ERR_NULL = 0,
    HI_MAC_TF_IDENTIFY_ERR_SUCCESS,                    // �����ɹ�
    HI_MAC_TF_IDENTIFY_ERR_CCO_NOT_SUPPORT,            //CCO��֧��̨��ʶ��
    HI_MAC_TF_IDENTIFY_ERR_NDM_NOT_SUPPORT,            //NMD��֧��̨��ʶ��
    HI_MAC_TF_IDENTIFY_ERR_FAIL,                       //����ʧ��
    HI_MAC_TF_IDENTIFY_ERR_IDENTIFYING,                // ̨��ʶ����
    HI_MAC_TF_IDENTIFY_ERR_BUSINESS_BUSY,              // ������appҵ��æ
    HI_MAC_TF_IDENTIFY_ERR_SEARCH_METER,               //�ѱ���
    HI_MAC_TF_IDENTIFY_ERR_BITMAP_INVALID,             //ָ����λͼ������
    HI_MAC_TF_IDENTIFY_ERR_SWITCHING_FREQ,             // CCO����ȫ���л�Ƶ���У�������̨��ʶ��
    HI_MAC_TF_IDENTIFY_ERR_MAX
}HI_MAC_TF_IDENTIFY_ERR_E;

//Ӧ��DIAG_CMD_TF_IDENTIFY_RSP_STRU�Ķ������£�
//̨��ʶ������Ӧ��ṹ����
typedef struct _DIAG_CMD_TF_IDENTIFY_RSP_STRU_
{
    HI_MAC_TF_IDENTIFY_ERR_E ret;                 //����ֵ
    HI_U32 pad;
}DIAG_CMD_TF_IDENTIFY_RSP_STRU;

// ̨��ʶ��ģʽ����������
typedef enum
{
    HI_MAC_TF_IDENTIFY_TYPE_NULL         = 0,
    HI_MAC_TF_IDENTIFY_TYPE_TEI             ,  //ʹ��TEI����̨��ʶ��
    HI_MAC_TF_IDENTIFY_TYPE_MAC             ,//ʹ��MAC����̨��ʶ��
    HI_MAC_TF_IDENTIFY_TYPE_TEI_MAC         ,//ͬʱʹ��TEI��MAC����̨��ʶ��

    HI_MAC_TF_IDENTIFY_TYPE_MAX
}HI_MAC_TF_IDENTIFY_TYPE_E;

#define HI_MAC_TF_TEI_BITMAP_LEN      32      // �·�̨��ʶ��Χ����
#define HI_MAC_TF_TEI_BITMAP_WIDTH    64      // �·�̨��ʶ��Χ�Ŀ��

//̨��ʶ���ʶ��Χ����
typedef struct _DIAG_CMD_TF_IDENTIFY_STA_CONFIG_STRU_
{
    HI_U16      sn;                                                     //���кţ�����һ�������Ӧ�Ķ�����ݰ��ĸ�ֵ��ͬ����ͬ��������ϱ�����ֵ��ͬ������sn�Ŵ�1��ʼ����������Ҳ��
    HI_U16      is_last:1;                                           //�����·������һ�����ı�ʶ�����һ��������ֵΪ1�������һ��������ֵΪ0
    HI_U16      pkt_idx:15;                                        //�����������ţ���1��ʼ
    HI_MAC_TF_IDENTIFY_TYPE_E       ident_type;   //0:ʹ��TEI����ʶ��1:ʹ��mac����ʶ��2:ͬʱʹ��TEI��mac����ʶ��

    HI_U32      tei_bitmap[HI_MAC_TF_TEI_BITMAP_LEN];   //��ʶ��վ���TEIλͼ���Ժ���Ը�Ϊmac��ַ

    HI_U8       pad[4];                                              //�������Ժ���Ը�Ϊmac����
}DIAG_CMD_TF_IDENTIFY_STA_CONFIG_STRU;

// Ӧ��DIAG_CMD_TF_IDENTIFY_STA_CONFIG_IND_STRU�Ķ������£�
// ̨��ʶ������Ӧ��ṹ����
typedef struct _DIAG_CMD_TF_IDENTIFY_STA_CONFIG_IND_STRU_
{
    HI_MAC_TF_IDENTIFY_ERR_E ret;                 //����ֵ
    HI_U32 pad;
}DIAG_CMD_TF_IDENTIFY_STA_CONFIG_IND_STRU;
// HSO ��ѯ����̨��ʶ�����÷�Χ
typedef struct _DIAG_CMD_TF_IDENTIFY_QRY_STA_CONFIG_REQ_STRU_
{
    HI_U32 tei_bitmap[HI_MAC_TF_TEI_BITMAP_LEN];   //��ʶ��վ���TEIλͼ
}DIAG_CMD_TF_IDENTIFY_QRY_STA_CONFIG_REQ_STRU;

typedef struct _DIAG_CMD_TF_IDENTIFY_STA_QRY_RANGE_STRU_
{
    HI_U8       ident_type;    //ȫ�����ǲ���ʶ��0��ȫ��ʶ��1������ʶ��ȡֵΪHI_MAC_TF_IDENTIFY_QRY_TYPE_E����
    HI_U8       pad[3];          //����

    HI_U32     tei_bitmap[HI_MAC_TF_TEI_BITMAP_LEN];   //��ʶ��վ���TEIλͼ���Ժ���Ը�Ϊmac��ַ
}DIAG_CMD_TF_IDENTIFY_STA_QRY_RANGE_STRU;
// ̨��ʶ���ѯʶ�����·�����
typedef enum
{
    HI_MAC_TF_IDENTIFY_QRY_TYPE_WHOLE_NET,
    HI_MAC_TF_IDENTIFY_QRY_TYPE_PART
}HI_MAC_TF_IDENTIFY_QRY_TYPE_E;

// ̨��ʶ���ʶ������������:
typedef enum
{
    HI_MAC_TF_IDENTIFY_QRY_RESULT_UNKNOWN = 0,
    HI_MAC_TF_IDENTIFY_QRY_RESULT_THIS_POWER_DISTRI,      //Ϊ��̨��վ��
    HI_MAC_TF_IDENTIFY_QRY_RESULT_NON_THIS_POWER_DISTRI, //��Ϊ��̨ȥվ��
    HI_MAC_TF_IDENTIFY_QRY_RESULT_NOT_SUPPORT, //��֧��̨��ʶ��
    HI_MAC_TF_IDENTIFY_QRY_RESULT_FAIL,        //ʶ������ѯʧ��
    HI_MAC_TF_IDENTIFY_QRY_RESULT_MAYBE_OTHER,  //��������̨����վ��
    HI_MAC_TF_IDENTIFY_QRY_RESULT_CODE_CRASH     // ������ײ
}HI_MAC_TF_IDENTIFY_QRY_RESULT_E;

//���յ�λ
#define DIAG_CMD_TF_RCV_LEVEL_UNKNOWN                               0       //���յ�λδ֪
#define DIAG_CMD_TF_RCV_LEVEL_240K                                  1       //240K���յ�λ
#define DIAG_CMD_TF_RCV_LEVEL_1M                                    2       //1M���յ�λ

// ��������
#define DIAG_CMD_TF_13BIT_SEQ_TYPE                                  0       // 13bit��������
#define DIAG_CMD_TF_127BIT_SEQ_TYPE                                 1       // 127bit��������
#define DIAG_CMD_TF_127BIT_INTER_SEQ_TYPE                           2       // 127bit����Ŷ���������

//DIAG_CMD_TF_IDENTIFY_RESULT_ENTRY_STRU�������£�
//̨��ʶ�����г�Ա�Ķ��壬16�ֽ�
typedef struct _DIAG_CMD_TF_IDENTIFY_RESULT_ENTRY_STRU_
{
    HI_U16 tei : 12;
    HI_U16 best_line : 4;                   // 8����ѹ�㣬��1-8
    HI_U8  is_online            : 1;        //0:�����ߣ�1:����
    HI_U8  is_exist_diff_seq    : 1;        //̨��ʶ�����Ƿ���ڲ�ͬ���޼��������в�ͬ�����
    HI_U8  alg_mode             : 2;        //ʶ��ɹ�ʱ���㷨
    HI_U8  result               : 4;        //̨��ʶ����:���������HI_MAC_TF_IDENTIFY_QRY_RESULT_E
    HI_U8  noise_limit          : 4;        //ʶ��ɹ�ʱ��ȥ������
    HI_U8  seq_type             : 1;        // �������ͣ��μ�DIAG_CMD_TF_13BIT_SEQ_TYPE
    HI_U8  rcv_level            : 3;        //ʶ��ɹ�ʱ�ĵ�λ,ȡֵ�μ�DIAG_CMD_TF_RCV_LEVEL_240K�Ķ���

    HI_U16 soft_value;                      //ʶ��ɹ�ʱ����ֵ
    HI_U8  mac[HI_PLC_MAC_ADDR_LEN];

    HI_U16 signal_mean;                     //�źŵľ�ֵ
    HI_U16 noise_mean;                      //�����ľ�ֵ
}DIAG_CMD_TF_IDENTIFY_RESULT_ENTRY_STRU;

//̨��ʶ��״̬
typedef enum
{
    HI_MAC_TF_IDENTIFY_STATUS_DOING                 = 0,        //̨���Ŷ��׶�
    HI_MAC_TF_IDENTIFY_STATUS_DONE                  = 1,        //̨��ʶ��ȫ�׶ν���
    HI_MAC_TF_IDENTIFY_STATUS_REVISE_MODE           = 2,        //ʶ����У���׶�
}HI_MAC_TF_IDENTIFY_STATUS_E;

// ̨��ʶ������ѯӦ��ṹ����
typedef struct _DIAG_CMD_TF_IDENTIFY_RESULT_RSP_STRU_
{
    HI_U16 sn;                  //ÿ̨��ʶ�����ϱ������кţ�����һ�β�ѯ�����Ӧ�Ķ���ϱ����ݰ��ĸ�ֵ��ͬ����ͬ��������ϱ�����ֵ��ͬ
    HI_U16 pkt_idx;             //������������

    HI_U16 num_total;           //�ܸ���
    HI_U16 entry_num;           //�����ݰ��а�������Чվ����

    HI_U32 tf_identify_time;    //��ʶ��ʱ��,��λ:s

    HI_BOOL is_last;            //�����ϱ������һ�����ı�ʶ�����һ��������ֵΪ1�������һ��������ֵΪ0
    HI_U8  status;              //0��ʶ���У�1��ʶ�����
    HI_U8  pad[2];              //����

    DIAG_CMD_TF_IDENTIFY_RESULT_ENTRY_STRU entry[0];
}DIAG_CMD_TF_IDENTIFY_RESULT_RSP_STRU;

#define DIAG_CMD_TF_STOP_REASOM_ZERO_INTERRUPT_ERR  1           //�����ж��쳣
//�쳣ֹ̨ͣ��ʶ���ԭ��
#define TF_STOP_ABNORMITY_IDENTIFY_REASON_UNKNOWN                       0       //δ֪
#define TF_STOP_ABNORMITY_IDENTIFY_REASON_NDM_ZERO_ERR                  1      //����������ж��쳣
#define TF_STOP_ABNORMITY_IDENTIFY_REASON_WAIT_SEND_SEQ_END_TIMEOUT     2       //̨�����з��ͳ�ʱ
#define TF_STOP_ABNORMITY_IDENTIFY_REASON_HSO                           3       //HSǪֹͣ��ʶ������н׶�
#define TF_STOP_ABNORMITY_IDENTIFY_REASON_SEND_REQ_TIMEOUT              4       //CCO֪ͨNDM�������г�ʱ
#define TF_STOP_ABNORMITY_IDENTIFY_REASON_SEND_SEQ_REQ_TO_NDM           5       //�򳭿�����������ʧ��
#define TF_STOP_ABNORMITY_IDENTIFY_REASON_NDC_DISCONNECT                6       //CCO��NDM���ӶϿ�
#define TF_STOP_ABNORMITY_IDENTIFY_REASON_REINIT                        7       //������
#define TF_STOP_ABNORMITY_IDENTIFY_REASON_HSO_START                     8      //HSO����̨��ʶ��ʱֹ̨ͣ��ʶ��
#define TF_STOP_ABNORMITY_IDENTIFY_REASON_MODE_OVERTIME                 9      //̨��ʶ��ģʽ����ֹͣ
#define TF_STOP_ABNORMITY_IDENTIFY_REASON_NDM_IN_STA_MODE               10      //����������STA����ģʽ��ֹ̨ͣ��ʶ��
#define TF_STOP_ABNORMITY_IDENTIFY_REASON_NDM_NOT_TX                    11      // ndm���Ƿ����
#define TF_STOP_ABNORMITY_IDENTIFY_REASON_NDM_AUTO_CHECK_FAIL           12      //������ϵ��Լ�ʧ��
#define TF_STOP_ABNORMITY_IDENTIFY_REASON_UPG                           13       //�������쳣ֹͣ
#define TF_STOP_ABNORMITY_IDENTIFY_REASON_NDM_TO_NDM                    14       // ���������ndm-ndmģʽ

//̨��ʶ���״̬��ѯ��Ӧ��ṹ����
typedef struct
{
    HI_U32 pad0;                //���õ����з���ģʽ����ʱ�䣬��λs
    HI_U32 run_time;            //��ǰ�����е�ʱ��,��λs

    HI_U8  identify_state;      //ʶ��״̬,ȡֵHI_MAC_TF_IDENTIFY_STATUS_E
    HI_U8  identify_mode;       //ʶ��ģʽ,�����TF_NOT_USE_IDENTIFY_MODE
    HI_BOOL is_searched_meter;  //�Ƿ������ѱ�
    HI_U8  stop_reason;         //̨��ʶ��ֹͣԭ��

    HI_U16 identify_seq;        //̨��ʶ��ʹ�õ�����
    HI_U8 seq_type;             // ��������
    HI_U8 pad2;

    HI_U32 set_identify_time;   //���õ�ʶ��ģʽ����ʱ�䣬��λs
    HI_U32 pad[4];              //����
}DIAG_CMD_TF_IDENTIFY_STATE_RSP_STRU;

#define HI_MAC_PHASE_IDENTIFY_RESULT_ENTRY_NUM_MAX      46      //��λʶ����ÿ���ϱ������վ�����

//����վ�����λʶ����
typedef struct
{
    HI_U16 tei;                 //��վ���TEI
    HI_U8  result   : 4;        //����1����λʶ����:PHASE_ALL��PHASE_A��PHASE_B��PHASE_C��INVALID_PHASE���μ�hi_mdm.h
    HI_U8  state    : 4;        //��λʶ��״̬:0,δʶ��;1:��֧��;2:ʶ����;3:ʶ�����
    HI_U8  pn_identify_result : 2;  //����1����߷���ʶ����:0,������ȷ;1,���ӣ�2,δ֪,�μ�hi_mdm.h
    HI_U8  pn_identify_state : 2;   //����߷���ʶ��״̬:0,δʶ��;1:��֧��;2:ʶ����;3:ʶ�����
    HI_U8  pn_identify_result_b : 2;  //����2����߷���ʶ����:0,������ȷ;1,���ӣ�2,δ֪,�μ�hi_mdm.h
    HI_U8  pn_identify_result_c : 2;  //����3����߷���ʶ����:0,������ȷ;1,���ӣ�2,δ֪,�μ�hi_mdm.h

    HI_U32 result_b : 4;        //����2����λʶ����:PHASE_ALL��PHASE_A��PHASE_B��PHASE_C��INVALID_PHASE���μ�hi_mdm.h
    HI_U32 result_c : 4;        //����3����λʶ����:PHASE_ALL��PHASE_A��PHASE_B��PHASE_C��INVALID_PHASE���μ�hi_mdm.h
    HI_U32 product_type : 5;    // ��Ʒ���ͣ��μ�HI_ND_TYPE_CCO,HI_ND_TYPE_STA,HI_ND_TYPE_RELAY,HI_ND_TYPE_THREE_STA
    HI_U32 pad3     : 19;       //����
}DIAG_CMD_PHASE_IDENTIFY_RESULT_ENTRY_STRU;


//��λʶ����Ӧ��ṹ����:
typedef struct
{
    HI_U16 sn;                  //ÿ̨��ʶ�����ϱ������кţ�����һ�β�ѯ�����Ӧ�Ķ���ϱ����ݰ��ĸ�ֵ��ͬ����ͬ��������ϱ�����ֵ��ͬ
    HI_U16 pkt_idx;             //�����������ţ���0��ʼ

    HI_U16 entry_num;           //�����ݰ��а�������Чվ����
    HI_BOOL is_last;            //�����ϱ������һ�����ı�ʶ�����һ��������ֵΪ1�������һ��������ֵΪ0
    HI_U8  pad;                 //����

    HI_U32 pad2[2];             //����

    DIAG_CMD_PHASE_IDENTIFY_RESULT_ENTRY_STRU entry[0];
}DIAG_CMD_PHASE_IDENTIFY_RESULT_STRU;

// ̨��ʶ�������������������:
typedef enum
{
    DIAG_CMD_BLACKLIST_TYPE_USER = 0,                       //���û�����ĺ�����
    DIAG_CMD_BLACKLIST_TYPE_AUTO                            //�Զ����ɵĺ�����
}DIAG_CMD_TF_IDENTIFY_BLACKLIST_TYPE_E;

// ̨��ʶ���������������
typedef enum
{
    HI_MAC_BLACKLIST_ERR_UNKNOWN = 0,              // ����ԭ��δ֪
    HI_MAC_BLACKLIST_ERR_BIG_NOISE,                // ������
    HI_MAC_BLACKLIST_ERR_CODE_CRASH,               // ������ײ
    HI_MAC_BLACKLIST_ERR_BIG_NOISE_AND_CODE_CRASH, // ���д���������������ײ
}HI_MAC_TF_IDENTIFY_BLACKLIST_ERR_TYPE_E;
// ̨��ʶ�������
typedef struct _DIAG_CMD_TF_IDENTIFY_BLACKLIST_ENTRY_S_
{
    HI_U8 mac[HI_PLC_MAC_ADDR_LEN];
    HI_U16 scope_avg;

    HI_U8 type : 2;             //�����������ͣ�ȡֵΪDIAG_CMD_TF_IDENTIFY_BLACKLIST_TYPE_E
    HI_U8 rcv_level : 3;     //���յ�λ
    HI_U8 pad1 : 3;
    HI_U8 err_type;         // ���������ͣ�ȡֵΪHI_MAC_TF_IDENTIFY_BLACKLIST_ERR_TYPE_E
    HI_U8 pad2[2];
}DIAG_CMD_TF_IDENTIFY_BLACKLIST_ENTRY_S;

// ̨��ʶ��������ϱ���ѯ�ṹ�嶨������:
typedef struct  _DIAG_CMD_BLACKLIST_REQ_S_
{
    HI_U16 sn;                      //����ϱ������кţ�����һ�β�ѯ�����Ӧ�Ķ���ϱ����ݰ��ĸ�ֵ��ͬ����ͬ��������ϱ�����ֵ��ͬ
    HI_U16 pkt_idx;                 //������������

    HI_U16 num_total;               //�ܸ���
    HI_U16 is_last : 1;             //�����ϱ������һ�����ı�ʶ�����һ��������ֵΪ1�������һ��������ֵΪ0
    HI_U16 mac_num : 15;            //�����ݰ��а�������Чmac��ַ��

    HI_U8  pad[4];

    DIAG_CMD_TF_IDENTIFY_BLACKLIST_ENTRY_S entry[0];
}DIAG_CMD_BLACKLIST_REQ_S;

// ̨��ʶ����պ�����
typedef struct _DIAG_CMD_BLACKLIST_CLEAR_IND_STRU_
{
    HI_MAC_TF_IDENTIFY_ERR_E ret;                 //����ֵ
    HI_U32 pad;
}DIAG_CMD_BLACKLIST_CLEAR_IND_STRU;

typedef struct _DIAG_CMD_TF_CROSSTALK_RESULT_ENTRY_STRU_
{
   HI_U16 other_signal_mean;                                           // ��Ӧ����Ŷ�ֵ
   HI_U16 result : 4;                                           // �Ƿ�Ϊ������̨��,�μ�HI_MAC_TF_IDENTIFY_QRY_RESULT_E
   HI_U16 pad : 12;

   HI_U8 my_mac[HI_PLC_MAC_ADDR_LEN];                 // ��վ��MAC
   HI_U8 cco_mac[HI_PLC_MAC_ADDR_LEN];                // ��Ӧ����Ŷ���CCO��MAC
}DIAG_CMD_TF_CROSSTALK_RESULT_ENTRY_STRU;

typedef struct _DIAG_CMD_TF_CROSSTALK_RESULT_RSP_STRU_
{
   HI_U16 sn;
   HI_U16 pkt_idx;

   HI_U16 num_total;           //�ܸ���
   HI_U16 entry_num;           //�����ݰ��а�������Чվ����

   HI_BOOL is_last;
   HI_U8 pad[3];

   DIAG_CMD_TF_CROSSTALK_RESULT_ENTRY_STRU entry[0];
}DIAG_CMD_TF_CROSSTALK_RESULT_RSP_STRU;

typedef struct
{
    HI_U16 his_begin_idx;
    HI_U16 pad;
} DIAG_CMD_CCO_TF_STAT_REQ_STRU;

typedef struct
{
    HI_U16 topo_num;
    HI_U16 online_num;
} diag_tf_his_node_st;

#define DIAG_TF_HIS_QUERY_NUM   (50)

typedef struct
{
    HI_U32 ret;

    HI_U8  snr_tf_complete;
    HI_U8  is_cco_snr_enable;
    HI_U16 complete_topo_num;
    HI_U32 complete_time;
    HI_U32 last_sta_join_off_time;

    HI_U16 his_idx;
    HI_U16 pad1;
    diag_tf_his_node_st histories[DIAG_TF_HIS_QUERY_NUM];
} DIAG_CMD_CCO_TF_STAT_IND_STRU;

//̨��ʶ��CCO�˵�ͳ����
typedef struct
{
    HI_U32 send_seq_req_to_ndm_cnt;         //�������������NDM�Ĵ���
    HI_U32 identify_tx_start_success_cnt;   //̨��ʶ��ɹ������Ĵ���
    HI_U32 send_seq_end_to_cco_cnt;         //�����������֪ͨ�ļ���

    HI_U16 send_ndm_chl_busy_cnt;           //����ʱNDCͨ��æ����
    HI_U16 rcv_ndm_chl_free_cnt;            //���յ�����ʱ�����Ͷ˿��еļ���(��Ԥ�ڵ�����)

    HI_U32 resend_ndm_chl_req_cnt;          //NDCͨ���ش�����
    HI_U16 send_ndm_chl_req_fail_cnt;       //NDCͨ��������ʧ�ܼ���
    HI_U16 resend_send_ndm_chl_req_fail_cnt;//NDCͨ�������ط�ʧ�ܼ���

    HI_U32 rcv_seq_req_to_ndm_cnt;          //�������������NDM�Ĵ���
    HI_U32 send_seq_cycle_stop_to_ndm_cnt;  //֪ͨ��������������ѭ������ģʽ������Ĵ���
    HI_U32 rcv_seq_end_to_cco_cnt;          //�����������֪ͨ�ļ���
    HI_U32 rcv_repeat_ndm_chl_req_cnt;      //�յ��ظ���NDCͨ������

    HI_U16 rcv_unexpect_ndm_chl_req_cnt;    //�յ���Ԥ�ڵ�NDCͨ������
    HI_U16 rcv_unexpect_ndm_chl_rsp_cnt;    //�յ���Ԥ�ڵ�NDCͨ��Ӧ�����

    HI_U32 rcv_seq_req_to_ndm_rsp_cnt;      //�������������NDM�Ĵ���
    HI_U16 send_seq_cycle_stop_to_ndm_fail_cnt;     //����ʧ�ܴ���
    HI_U16 ndm_send_seq_fail_by_req_cnt;    //���������յ�����ʱ����ʧ�ܵĴ���
    HI_U32 rcv_seq_end_to_cco_rsp_cnt;      //�����������֪ͨ�ļ���

    HI_U32 send_result_req_cnt;             //CCO����ʶ������ѯ�Ĵ���

    HI_U16 send_result_req_alloc_msdu_buff_err_cnt;     //��ȡbufferʧ��
    HI_U16 send_result_req_msdu_to_tx_err_cnt;          //���ͱ���ʧ��

    HI_U32 rcv_result_rsp_cnt;              //CCO�յ�ʶ����Ӧ��Ĵ���

    HI_U16 rcv_result_rsp_unexpect_cnt;     //CCO�յ���Ԥ�ڵ�Ӧ�����
    HI_U16 rcv_result_rsp_err_cnt;          //CCO�յ�Ӧ�������ݴ������

    HI_U32 identify_result_belong_cnt;      //ʶ����Ϊ��̨��վ��ļ���
    HI_U32 query_timeout_fail_cnt;          //��ѯ��ʱ��ʶ��ʧ�ܵ�վ�����
    HI_U32 identify_result_not_belong_cnt;  //ʶ�������Ǳ�̨��վ��ļ���

    HI_U32 query_one_turn_over_cnt;         //һ�ֲ�ѯ��������
    HI_U32 stop_identify_cnt;               //ֹ̨ͣ��ʶ��ļ���

    HI_U32 stop_abnormity_identify_cnt;     //�쳣ֹ̨ͣ��ʶ�����
    HI_U8  stop_abnormity_identify_last_reason;         //���һ���쳣ֹͣ��ԭ��
    HI_BOOL ndm_tf_mode;                    //������̨��ʶ��ģʽ�½�ֹ�����ı�ʶ
    HI_U16 ndm_not_support_identify_cnt;    //CCO֪ͨʱ����֪��������֧��̨��ʶ�����

    HI_U32 send_seq_bit_cnt;                // ���ͻ�����̨��ʶ�����д���

    HI_U16 identify_tx_start_fail_disable_cnt;          //����������̨��ʶ�����ɵ�����ʧ�ܴ���
    HI_U16 identify_tx_start_fail_doing_cnt;            //������ʶ�����ɵ�����ʧ�ܴ���
    HI_U16 query_mode_continuance_timer_cnt;            //��ѯģʽ��ʱ���ڴ���
    HI_U8  tf_identify_control_mode;                    //̨��ʶ�����ģʽ
    HI_BOOL is_identifying;

    HI_U32 query_timeout_sta_cnt;                       //��ѯ��ʱ��վ�����

    HI_U16 send_seq_end_to_cco_fail_cnt;                //��CCO�����������֪ͨʧ��
    HI_U16 rcv_result_unexpect_identify_state_cnt;      //CCO�յ���STA��״̬������Ԥ��

    HI_U32 rcv_not_support_ndm_chl_req_cnt;             //�յ��޷�֧�ֵ�NDCͨ������
    HI_U32 rcv_send_seq_cycle_stop_rsp_cnt;             //�յ�Ӧ��
    HI_U32 rcv_seq_cycle_stop_cnt;                      //���յ��������

    HI_U8  cco_seq_type;                                // CCO����������
    HI_U8  ndm_seq_type;                                // ndm����������
    HI_U16 pad;
}DIAG_CMD_TF_CCO_STATIC_STRU;

//CCO������ͳ����
typedef struct
{
    HI_U32 send_ntb_collect_req_cnt;                //����NTB�ɼ������ļ���
    HI_U32 send_broadcast_notify_cnt;               // ���͹㲥̨֪ͨ��ʶ��������ֹͣ����

    HI_U32 hso_cco_all_mode_start_cnt;              //HSO����ȫ��ʶ�����
    HI_U32 hso_cco_part_mode_start_cnt;             //HSO����ָ��վ��ʶ�����
    HI_U32 result_query_timer_cnt;                  //�����ѯ��ʱ���ص�����
    HI_U16 send_seq_req_to_ndm_fail_cnt;            //�򳭿����������з�������ʧ��
    HI_U16 wait_send_seq_end_timeout_cnt;           //�����з������֪ͨ��ʱ

    HI_U32 new_sta_to_identify_cnt;                 //�¼���Ĵ�ʶ��վ�����

    HI_U16 rcv_seq_end_to_cco_unexpect_cnt;         //�յ���Ԥ�ڵ�̨��ʶ�����з������֪ͨ
    HI_U16 rcv_zero_cross_ntb_rsp_err_cnt;          //�յ��������λʶ�����NTB�ɼ�Ӧ���ļ���

    HI_U8 ndm_to_ndm_mode;                          // ���������ndm-ndm��ʶ��ģʽ
    HI_U8 pad1;
    HI_U8  bcn_tf_notify;                           //�Ƿ�֪ͨSTA����̨��ʶ��
    HI_BOOL is_searched_meter;                       //�Ƿ������ѱ�

    HI_U32 start_search_meter_cnt;                  //�����ѱ����
    HI_U32 stop_search_meter_cnt;                   //ֹͣ�ѱ����
    HI_U16 start_search_meter_err_cnt;              //�����ѱ�ʧ�ܼ���
    HI_U16 stop_search_meter_err_cnt;               //ֹͣ�ѱ�ʧ�ܼ���

    HI_U8 ndm_in_sta_mode_cnt;                         // CCO֪ͨʱ����֪����������STA����ģʽ�£�CCO������̨��ʶ��
    HI_U8 ndm_ndc_access_mode;                          // CCO֪ͨʱ����֪����������������ģʽ
    HI_U8 ndm_not_tx_cnt;                               // ndm���Ƿ������CCO������̨��ʶ��
    HI_U8 ndm_auto_check_fail_cnt;                      //������ϵ��Լ�ʧ��

    HI_U32 last_identify_success_sta_cnt;           //���β�ѯδʶ��ɹ����ϴ�ʶ��ɹ��Ĵ���

    HI_U32 pad[3];
}DIAG_CMD_TF_CCO_FL_STATIC_STRU;

//̨��ʶ��STA�˵�ͳ����
typedef struct
{
    HI_U32 bcn_notify_enable_cnt;                   //beacon����̨��ʶ�����
    HI_U32 bcn_notify_disable_cnt;                  //beacon�ر�̨��ʶ�����

    HI_U32 rcv_result_req_cnt;                      //�յ�̨��ʶ��������Ĵ���
    HI_U32 send_result_rsp_cnt;                     //����̨��ʶ����Ӧ��Ĵ���

    HI_U16 phase_ntb_collect_reinit;                //��λʶ��NTB���²ɼ��Ĵ���
    HI_U16 identify_result_bpc_err_cnt;             //ʶ������BPC������Ԥ�ڵĴ���

    HI_U16 identify_freq_or_snid_err_cnt;           //ʶ�����Ƶ�λ�SNID���ѯʱ��һ�µĴ���
    HI_U8  last_query_tf_result;                    //���һ�β�ѯʱ��̨��ʶ����
    HI_BOOL pad;

    HI_U32 rcv_ntb_collect_req_cnt;                 //�յ�NTB���ݲɼ�֪ͨ�ı��ļ���
    HI_U16 sta_ntb_collect_store_index;             //NTB���ݴ洢����
    HI_U16 listen_network_snid;                     //���յ�����̨��ʶ�������SNID

    HI_U8  current_rcv_level;                       //��ǰ���յ�λ,0Ϊ240k��1Ϊ1M
    HI_U8  rcv_mode;                                //���յ�λ�л�ģʽ
    HI_U16 om_ntb_collect_not_rcv_notify_cnt;       //NTB���ݲɼ�δ�յ�֪ͨ

    HI_U16 rcv_level_switch_timer_cnt;              //���յ�λ�л���ʱ���ػص�����
    HI_BOOL is_bcn_tf_start;                        //�ű�֪ͨ����̨��ʶ��
    HI_BOOL is_seq_tf_start;                        //̨��ʶ����������̨��ʶ��

    HI_U32 seq_tf_start_cnt;                        //��������̨��ʶ��Ĵ���
    HI_U32 seq_tf_stop_cnt;                         //����ֹ̨ͣ��ʶ��Ĵ���
    HI_U32 rcv_zero_cross_ntb_req_cnt;              //���յ�����NTB�����Ĵ���
    HI_U32 send_zero_cross_ntb_rsp_cnt;             //���͹���NTBӦ���Ĵ���

    HI_U16 his_result_invalid_cnt;                  //��ʷ���ʧЧ����
    HI_U16 cco_info_change_by_mac_cnt;              //��MAC��ͬ��CCO����Ϣ�仯�Ĵ���

    HI_U16 cco_info_change_by_freqsnid_cnt;         //��freq��snid��ͬ��CCO����Ϣ�仯�Ĵ���
    HI_U16 pad1;

    HI_U32 old_freq : 8;                            //CCO��Ϣ�仯ǰ��freq
    HI_U32 old_snid : 24;                           //CCO��Ϣ�仯ǰ��snid
    HI_U32 new_freq : 8;                            //CCO��Ϣ�仯���freq
    HI_U32 new_snid : 24;                           //CCO��Ϣ�仯���snid

    HI_U32 rcv_seq_when_before_invalid_cnt;         //ǰһ��������Ч���յ����еĸ���

    HI_U8  old_cco_mac[6];                          //CCO��Ϣ�仯ǰ��MAC
    HI_U8  new_cco_mac[6];                          //CCO��Ϣ�仯���MAC

    HI_U32 his_max_to_small_replaced_cnt;           //��ʷ���ֵ��С���滻�ļ���

    HI_U32 rcv_broadcast_notify_req_cnt;            //�յ��㲥֪ͨ��������
}DIAG_CMD_TF_STA_STATIC_STRU;

//̨��ʶ�𹫹�ͳ����
typedef struct
{
    HI_BOOL is_support_tf_identify;                 //�Ƿ�֧�ֹ����ж�
    HI_U8  sta_check_tf_cnt;                        //STA ����Ƿ�֧��̨��ʶ������ڼ���
    HI_U8  rcv_ntb_cnt;                             //���յ��Ĺ����жϼ���
    HI_U8  rcv_a_phase_ntb_cnt;                     //���յ���A��λ�����жϼ���

    HI_U8  rcv_b_phase_ntb_cnt;                     //���յ���B��λ�����жϼ���
    HI_U8  rcv_c_phase_ntb_cnt;                     //���յ���C��λ�����жϼ���
    HI_U16 malloc_err_cnt;                          //��̬��������ʧ�ܼ���

    HI_U32 switch_phase_by_timeout_cnt;             //��ʱ��������λ����
    HI_U32 zero_cross_interrupt_cnt;                //�����жϼ���

    HI_U32 identify_tx_start_by_query_join_sta;     //��ѯʱ����վ����봥����ʶ������

    HI_U16 send_msg_err_cnt;                        //������Ϣʧ�ܼ���
    HI_U16 start_timer_err_cnt;                     //��ʱ������ʧ�ܼ���

    HI_U16 alloc_msdu_buff_err_cnt;                 //��ȡMacBufferʧ�ܴ���
    HI_U16 msdu_to_tx_err_cnt;                      //���ͱ���ʧ�ܴ���
    HI_U8 tf_check_set_sta_reset_cnt;               //STA���̨��ʶ����STA�������Ĵ���
    HI_U8 current_edge;                             //NDM��ǰNTB��
    HI_U16 report_to_hso_err_cnt;                   //�ϱ�HSOʧ�ܵļ���

    HI_BOOL is_three_phase_check_ok;                //�����������OK
    HI_BOOL is_single_phase;                        //������Ƿ�Ϊ���࣬
    HI_BOOL is_three_phase_together;                //������������һ��
    HI_U8 ndm_check_an_ok_cnt;                      //������ϵ��Լ�AN��������

    HI_U8 ndm_check_an_err_cnt;                     //������ϵ��Լ�AN��ѹ���ϴ���
    HI_U8 phase_b_ok_c_err_cnt;                     //B��λ���źţ�C��λ���źŴ���
    HI_U8 phase_b_err_c_ok_cnt;                     //B��λ���źţ�C��λ���źŴ���
    HI_U8 check_phase_fail_turn_cnt;

    HI_U32 phase_ab_diff_us;                        //AB��λ��λ���
    HI_U32 edge_change_err_cnt;                     //NDM NTB���л��������

    HI_U8 phase_a_b_c_err_cnt;                      //A,B,C��û�й���
    HI_U8 phase_a_b_err_c_ok_cnt;                   //A,B��û�й��㣬C���й���
    HI_U8 phase_a_c_err_b_ok_cnt;                   //A,C��û�й��㣬B���й���
    HI_U8 phase_a_err_b_c_ok_cnt;                   //A��û�й��㣬B,C���й���

    HI_U8 disable_ndm_to_ndm_mode;                  //����������ndm-ndmģʽ
    HI_U8 cco_phase_check_result;                   // CCO��λ�����,1:ΪABC,2:ACB:3:ʧ��
    HI_U8 cco_phase_collect_ntb_cnt;                // CCO��λ����ռ�NTB����
    HI_U8 is_precise_zero_cross;                    // �Ƿ�Ϊ��׼�����·

    HI_U32 phase_bc_diff_us;                        //BC��λ��λ���
    HI_U32 phase_ac_diff_us;                        //AC��λ��λ���

    HI_U32 chip_zero_cross_interrupt_cnt;                          // �����жϴ���
    HI_U32 chip13bit_power_period_result_interrupt_cnt;     // V100���ֹ�Ƶ�����ж��ϱ�����
    HI_U32 chip127bit_power_period_result_interrupt_cnt;        // V200���ֹ�Ƶ�����ж��ϱ�����
    HI_U32 power_period_num_interrupt_cnt;                   // ͳ��power_period_num�����ϱ�
    HI_U32 noise_detect_interrupt_cnt;                       // ����ͳ���ж��ϱ�����
    HI_U32 phase_b_interrupt_cnt;                              // ��λB�жϴ���
    HI_U32 phase_c_interrupt_cnt;                              // ��λC�жϴ���
    HI_U32 sta_13Bit_chip_refresh_cnt;              // sta��V100��ˢ�´���
    HI_U32 sta_127Bit_chip_refresh_cnt;             // sta��V200��ˢ�´���
    HI_U32 ndm_chip_refresh_cnt;             // ndm��V200��ˢ�´���

    HI_U8 t_sta_auto_check_phase_result[3];            // ����λSTA�ϵ��Լ���
    HI_U8 support_t_sta_three_zero_mode;                 // ����ģ���Ƿ�֧����������⹦�ܡ�
    HI_U8 t_sta_first_valid_phase;                     // ����ģ���һ����Ч����λ��
    HI_U8 cco_phase_check_result_by_power_fail;        // ͨ��ͣ���ϱ���⵽��λ���
    HI_U16 t_sta_auto_check_phase_diff_us[3];    // ����λSTA�ϵ��Լ���λ�������λus
}DIAG_CMD_TF_STATIC_STRU;

//̨��ʶ��������ȡͳ����
typedef struct
{
    HI_U32 identify_success_bpc;                    //ʶ��ɹ�ʱ��bpc
    HI_U32 ntb_cache_cnt;                           //��Ч�����NTB����
    HI_U32 parse_receive_sequence_invalid_cnt;      //��ĳ����㷨����������ȡ����ʧ�ܼ���
    HI_U32 seq_bit_sum_invalid_cnt;                 //��ĳ����㷨�����������е��ۼӺ��쳣����

    HI_U16 seq_diff_in_noise_limit_cnt;             //���м��ɹ�ʱ��ͬȥ�����޼��������в�ͬ�ļ���
    HI_U8  identify_state;                          //STA��ʶ��״̬
    HI_U8  success_rcv_level;                       //ʶ��ɹ�ʱ�Ľ��յ�λ

    HI_U32 seq_detect_window_invalid_cnt;           //������̨��ʶ�����м����Ч�Ĵ���
    HI_U32 identify_success_stamp;                  //���ɹ�ʱ��� ��λ:s
    HI_U32 identify_diff_seq_stamp;                 //��⵽��������ʱ��� ��λ:s

    HI_U16 novel_seq_cnt;                           //����������쳣�����Ĵ���
    HI_U8  identify_freq;                           //STAʶ������ж�Ӧ��Ƶ��
    HI_U8 pad0;
    HI_U32 identify_snid;                           //STAʶ������ж�Ӧ��SNID

    HI_U8 identify_cco_mac[6];                      // STA��127Bitʶ���CCO_MAC
    HI_U8 seq_type;                                 // STAʶ�������������
    HI_U8 pad;

    HI_U32 detect_diff_snid_cnt;                    //��̨����������ȡ�Ĳ�ͬ��SNID�Ĵ���
    HI_U32 last_detect_diff_snid;                   //���һ�μ�⵽���쳣SNID
    HI_U8 pad1;
    HI_U8  last_detect_diff_freq;                   //���һ�μ�⵽�쳣SNIDʱ������Ƶ��
    HI_U16 novel_seq_store_cnt;                     //�쳣���д洢��������

    HI_U32 identify_success_cnt;                    //̨��ʶ��ɹ�����

    HI_U32 bigger_than_50ms_cnt;                    //����������NTB�������50ms����
    HI_U32 smaller_than_1ms_cnt;                    //����������NTB���С��1ms����
    HI_U32 lost_ntb_cnt;                            //�����ڶ�ʧ�Ĺ���NTB����
    HI_U32 between_window_than_50ms_cnt;            //���ڼ�����NTB�������50ms����
    HI_U32 between_window_smaller_than_1ms_cnt;
    HI_U32 between_window_lost_ntb_cnt;             //���ڼ䶪ʧ�Ĺ���NTB����
    HI_U32 detect_diff_freq_cnt;                    //����

    HI_U32 smaller_than_30us_cnt;                   //����������NTB���С��30us����
    HI_U32 smaller_than_5ms_cnt;                    //����������NTB���С��5ms����

    HI_U32 sign1_synch_success_cnt;                 //��һ��Aͬ���ɹ�����
    HI_U32 sign2_synch_success_cnt;                 //�ڶ���Aͬ���ɹ�����
    HI_U32 sign2_synch_fail_cnt;                    //�ڶ���Aͬ��ʧ�ܼ���
    HI_U32 exact3_synch_success_cnt;                //��ͬ��Bʶ��ɹ�����

    HI_U32 exact3_synch_timeout_cnt;                //��ͬ��Bʶ��ʱ����
    HI_U32 exact4_synch_success_cnt;                //��ͬ��Cʶ��ɹ�����
    HI_U32 exact4_synch_timeout_cnt;                //��ͬ��Cʶ��ʱ����
    HI_U32 sta_single_win_fail_cnt;                 //�������ڴﲻ�����޴���

    HI_U16 noise_avg[2];                            //����������ֵ
    HI_U16 start_ntb_index;                         //����ʶ���NTB��ʼλ��
    HI_U16 end_ntb_index;                           // �ռ���NTB�Ľ���λ��
    HI_U16 remain_ntb_cnt;                          // ���õ�NTB����
    HI_U16 code_crash_cnt;                          // ������ײ����

    HI_U32 sign2_synch_11_success_cnt;              // ��ͬ������A���ڵ�11��ʶ��ΪA�Ĵ���
    HI_U32 exact3_synch_fail_cnt;                   // ��ͬ������3ʶ��ʧ�ܴ���
    HI_U32 exact4_synch_fail_cnt;                   // ��ͬ������4ʶ��ʧ�ܴ���
    HI_U32 union_judge_cnt;                         // �����о�����
}DIAG_CMD_QUERY_TF_IDENTIFY_INFO_STRU;

//��λʶ��ͳ����
typedef struct
{
    HI_U32 identify_start_success_cnt;                  //��λʶ��ɹ���������
    HI_U32 stop_identify_cnt;                           //ֹͣ��λʶ�����

    HI_U16 identify_start_fail_by_doing_cnt;            //ʶ������д���ʶ��Ĵ���
    HI_U16 identify_start_fail_by_not_support_cnt;      //��֧��ʶ�������ʧ�ܵĴ���
    HI_U16 identify_start_fail_by_tf_cnt;               //̨��ʶ�������������λʶ��ʧ�ܴ���
    HI_U16 rcv_zero_cross_ntb_rsp_err_cnt;              //���յ��ı��Ĵ������

    HI_U32 send_zero_cross_ntb_req_cnt;                 //������λ����NTB��ѯ���ĵĴ���
    HI_U32 rcv_zero_cross_ntb_rsp_cnt;                  //�յ���λ����NTBӦ���ĵĴ���

    HI_U32 parallel_query_timer_msg_cnt;                //���в�ѯ��ʱ��ⶨʱ���ص�����
    HI_U32 parallel_query_timeout_cnt;                  //���в�ѯ��ʱ����

    HI_U32 query_one_turn_over_cnt;                     //��ѯһ�ֽ����Ĵ���
    HI_U32 check_over_timer_msg_cnt;                    //ȷ����λʶ�������ʱ���Ļص�����

    HI_U32 ntb_diff_smaller_than_1ms_cnt;               //����NTB���С��1ms

    HI_U16 discard_zero_cross_rsp_cnt;
    HI_U16 get_phase_by_sta_ntb_rsp_err_cnt;            //������λʱ��STA��Ӧ���Ĵ���

    HI_U32 phase_success_sta_cnt;                       //��λʶ��ɹ�����
    HI_U32 phase_fail_cnt;                              //��λʶ��ʧ�ܴ���
    HI_U32 phase_fail_sta_cnt;                          //��λʶ��ʧ�ܴ���

    HI_U16 rcv_zero_cross_ntb_rsp_timeout_cnt;          //�յ���ʱ�Ĺ���NTBӦ����
    HI_U16 get_phase_by_sta_ntb_ref_err_cnt;            //������λʱ��CCO�˵Ĳ���NTB��������

    HI_U32 pn_filter_abnormal_period_cnt;               //�����ʶ������쳣���ڴ���

    HI_U16 send_query_sta_ntb_by_tf_pkt_cnt;            // ͨ��̨��ʶ���Ĳ�ѯ����
    HI_U16 rcv_sta_ntb_by_tf_pkt_cnt;                   // ͨ��̨��ʶ���Ľ��յ�NTB��Ӧ����

    HI_U16 rcv_sta_ntb_callback_cnt;                    // ���յ�CCO֪ͨ��STA_NTB�ص�����
    HI_U16 send_notify_collect_cnt;                     // ֪ͨSTA���вɼ�����

    HI_U8 identify_mode;                                // ��ǰ������ʶ��ģʽ��0��ģʽ,1��ģʽ
    HI_U8 notify_state;                                 // ��ģʽ��֪ͨ״̬
    HI_U8 tf_pkt_identify_cnt;                          // ��ģʽ����������
    HI_U8 cur_period_invalid_cnt;                       // cco_periodΪ0�Ĵ���
    HI_U8 tf_lvai_srst_cnt;                            // ��λʶ������У������߼���λ����
    HI_U8 pad1[3];
    HI_U32 pad[4];
}DIAG_CMD_PI_CCO_STATIC_STRU;

#define DIAG_CMD_PI_CCO_BASE_NTB_NUM_MAX            92          //ÿ���ϱ������NTB����
//��λʶ��CCO�˵Ļ�׼NTB�ϱ�
typedef struct
{
    HI_U8  index;                                   //���ı��
    HI_U8  pad1;
    HI_U16 next_ntb_store_index;                    //��һ�������NTB������

    HI_U32 ntb_store_cnt;                           //�ѻ����NTB����

    HI_U32 pad[3];

    HI_U32 ntb[DIAG_CMD_PI_CCO_BASE_NTB_NUM_MAX];
}DIAG_CMD_PI_CCO_BASE_NTB_STRU;

//NTB�쳣ͳ��
typedef struct
{
    HI_U32 lisr_hisr_err_cnt;                                   //�ͼ��жϵ��߼��жϵ���ʱ���쳣����
    HI_U32 lisr_hisr_diff_cur;
    HI_U32 lisr_hisr_diff_max;
    HI_U32 lisr_hisr_diff_err;                                  //�����쳣ʱ�ĵͼ��ж���߼��жϵ�ʱ����λus

    HI_U32 lisr_hisr_diff_err_isp;                              //�����쳣ʱ�ĵͼ��ж���߼��жϵ�ʱ��ISP������,��λus

    HI_U16 revised_zero_period;                                 //У����Ĺ������ڣ���λus
    HI_U16 revise_zero_period_fail_cnt[3];                      //������У����������ʧ�ܼ���

    HI_U16 revised_zero_period_invalid_in_send_cnt;             //����ʱУ���Ĺ������ڷǷ�
    HI_U16 tx_lost_consume_cnt;                                 //���ߴ��"�����ж϶�ʧ"����

    HI_U16 stop_in_send_cnt[3];                                 //���͹�����ֹͣ�ļ���
    HI_U16 clear_seq_data_timer_cnt;                            //��ʱ�����������������Ĵ���

    HI_U32 tx_lost_times;                                       //����˶�ʧ�жϷ����Ĵ�����һ�ο��ܶ�����ж�
    HI_U32 tx_lost_pre_ntb;                                     //���ж϶�ʧʱ��ǰһ��NTB
    HI_U32 tx_lost_cur_ntb;                                     //���ж϶�ʧʱ�ĺ�һ��NTB
    HI_U32 tx_lost_isp;                                         //��ʧ�ж�ʱ��ISP������ֵ����λus

    HI_U32 smaller_than_3ms_cnt;                                //С��3ms�ķ����жϼ��ͳ��
    HI_U32 smaller_than_300us_cnt;                              //С��300us�ķ����жϼ��ͳ��
    HI_U32 smaller_than_one_period_cnt;                         //��һ������С��ͳ��

    HI_U16 set_send_seq_data_lisr_err_cnt;                      //�������1��lisr�ص��쳣����
    HI_U16 set_send_seq_data_hisr_err_cnt;                      //�������1��hisr�ص��쳣����

    HI_U32 set_send_seq_data_cnt;                               //�����������1�Ĵ���
    HI_U32 set_send_seq_data_hisr_cnt;                          //�����������1�Ķ�ʱ��hisr�ص�����

    HI_U16 zero_period_small_cnt;                               //У����Ĺ�������С����Сֵ�Ĵ�����53Hz
    HI_U16 zero_period_big_cnt;                                 //У����Ĺ������ڳ������ֵ�Ĵ�����45Hz

    HI_U32 pad[2];
}DIAG_CMD_QUERY_TF_NTB_STATIC_STRU;

//STA�ڲ�ͬ���޼�⵽��ͬ���е�ͳ��
typedef struct
{
    HI_U16 tei;
    HI_U8  freq;
    HI_U8  snid;

    HI_U16 wobble_scope;                                        //�Ŷ����Ⱦ�ֵ
    HI_U16 noise_scope;                                         //�������Ⱦ�ֵ

    HI_U32 time_stamp;                                          //ʱ�������λ:s
    HI_U32 bpc;

    HI_U16 soft_val[TF_ZERO_CROSS_NOISE_LIMIT_CNT];             //��ͬ���޼�������ֵ
    HI_U8  seq_index[TF_ZERO_CROSS_NOISE_LIMIT_CNT];            //��ͬ���޼���������

    HI_U8  noise_limit_index;                                   //���ɹ�ʱ��ȥ��������
    HI_U8  alg_index;                                           //���ɹ�ʱ�ļ���㷨
    HI_U8  identify_freq;                                       //ʶ�����Ƶ��
    HI_U32 identify_snid;                                       //ʶ�����snid

    HI_U16 pad0[42];                                            // 3*7*2
    HI_U8  pad1[56];                                            // 4*7*2
    HI_U8  pad2[56];                                            // 4*7*2
}DIAG_CMD_QUERY_TF_STA_SEQ_SUCCESS_INFO_STRU;

//����STA�Ŀ�ά�ɲ�
typedef struct
{
    HI_U8  lock_cmd;
    HI_U8  pad;
    HI_U16 tei;
}DIAG_CMD_TF_LOCK_STA_OM_STRU;

typedef struct
{
    HI_U8  rcv_level;
    HI_U8  novel_index;
    HI_U8  pad1[2];

    HI_U32 pad2[2];
}DIAG_CMD_TF_STA_OM_CMN_STRU;

//STĄ��ʶ����ղ����洢�ṹ:��ά�ɲ�,�ýṹ���С���ܳ���TF_OM_NTB_COLLECT_BLOCK_SIZE(1400)
typedef struct
{
    HI_U16 ntb_collect_store_cnt;                                       //NTB��ǰ�洢������������ֵһֱ�ۼ�ֱ����ת
    HI_U16 ntb_collect_sn;                                              //NTB�ɼ�֪ͨ�ִα��

    DIAG_CMD_QUERY_TF_STA_SEQ_SUCCESS_INFO_STRU success_info;           //��⵽���е���ֵ
    HI_U32 ntb[286];                                                    //ntb����,�ռ��С(TF_ZERO_CROSS_SEQUENCE_CNT - 1) * TF_ZERO_CROSS_SEQUENCE_SIZE) * 2

    //ǰ��Ľṹ��Ϊ1388Bytes
    DIAG_CMD_TF_STA_OM_CMN_STRU cmn;
}DIAG_CMD_TF_STA_OM_STORE_STRU;

//NDM-NDMģʽHSO��ѯndm̨��ʶ��״̬�ͽ��
typedef enum
{
    HI_MAC_TF_IDENTIFY_CMD_NDM_TX_MODE_START = 0,
    HI_MAC_TF_IDENTIFY_CMD_NDM_RX_MODE_START,
    HI_MAC_TF_IDENTIFY_CMD_NDM_STOP,
} HI_MAC_NDM2NDM_TF_SET_CMD_REQ_E;

typedef HI_MAC_NDM2NDM_TF_SET_CMD_REQ_E HI_MAC_NDM2NDM_TF_QUERY_STATUS_REQ_E;

typedef enum
{
    HI_MAC_TF_IDENTIFY_DOING,                      // ʶ����
    HI_MAC_TF_IDENTIFY_SUCCESS,                   // ʶ��ɹ�
    HI_MAC_TF_IDENTIFY_FAIL,                       // ʶ��ʧ��
}HI_MAC_NDM2NDM_TF_IDENTIFY_RESULT_ENUM;
typedef struct
{
    HI_MAC_NDM2NDM_TF_QUERY_STATUS_REQ_E eNdmMode;
    HI_U32 ulTfIdentTime;

    HI_U8  ucStatus;
    HI_U8  ucTfResult;                  // ʶ������0:ʶ���У�1:ʶ��ɹ���2:ʶ��ʧ��
    HI_U8  ucSerialNum;
    HI_U8  ucPhase;
    HI_U8  ucLevel;                     // ���ջ���λ��1M,240K
    HI_U8  stop_reason;             // ������쳣ֹͣ��0:������1:������г���쳣ֹͣ
    HI_U8  ucPad1[2];

    HI_U16 wobble_scope;            // �Ŷ���ֵ,��λus
    HI_U16 noise_scope;             // ʵʱ������ֵ,��λus
    HI_U32 ulPad[3];
} DIAG_CMD_NDM2NDM_TF_QUERY_STATUS_IND_STRU;

//NDM-NDMģʽHSO����ndm̨��ʶ��
typedef struct
{
    HI_MAC_NDM2NDM_TF_SET_CMD_REQ_E eNdmMode;
    HI_U8  ucSerialNum;
    HI_U8  ucPad[3];

    HI_U32 ulPad[5];
} DIAG_CMD_NDM2NDM_TF_SET_REQ_STRU;
typedef enum
{
    HI_MAC_NDM2NDM_TF_SET_CMD_SUCCESS = 0,
    HI_MAC_NDM2NDM_TF_SET_CMD_INDENTIFYING,
    HI_MAC_NDM2NDM_TF_SET_CMD_NOT_SUPPORT,
    HI_MAC_NDM2NDM_TF_SET_CMD_NDM_TO_STA_MODE,
}HI_MAC_NDM2NDM_TF_SET_CMD_IND_E;

typedef struct
{
    HI_MAC_NDM2NDM_TF_SET_CMD_IND_E eRet;
    HI_MAC_NDM2NDM_TF_QUERY_STATUS_REQ_E eSetInfo;

    HI_U32 ulPad[2];
} DIAG_CMD_NDM2NDM_TF_SET_IND_STRU;

#define DIAG_CMD_TF_EXACT_FALSE_CNT     (5)
typedef struct
{
    HI_U8 synch_state;       // �����龯ʱ��ͬ��״̬
    HI_U8 limit_index;        // ��������
    HI_U8 alg_index;         // ������
    HI_U8 seq_index;        // ����
    HI_U8 phase_index;       // ��λ
    HI_U8 pad;
    HI_U16 soft_val;            // ��ֵ
}DIAG_CMD_TF_EXACT_FALSE_ENTRY_ST;

typedef struct
{
    HI_U32 exact_false_cnt;          // �龯���ִ���
    DIAG_CMD_TF_EXACT_FALSE_ENTRY_ST exact_false_entry[DIAG_CMD_TF_EXACT_FALSE_CNT];
}DIAG_CMD_TF_EXACT_FALSE_INFO_STRU;

// оƬ�˼���ͳ����
#define DIAG_CMD_TF_VOLTAGE_DETECT_LINE_CNT             (8)         // v200̨��ʶ��оƬ�ϱ���·����

typedef struct
{
    HI_U16 v_min;                                            // ������͵�ѹ
    HI_U16 v_max;                                            // ������ߵ�ѹ

    HI_U32 power_period_cnt;                                 // ��Ƶ���ڼ���

    HI_U32 power_period_num;                                 // ͳ�ƵĹ�Ƶ���ڼ���
    HI_U32 power_period_num_r;                               // ͳ�ƵĹ�Ƶ���ڼ�������������λus

    HI_U32 ntb_zerocrossr_local;                             // ������ + ����ʱ�ӵ�NTB
    HI_U32 ntb_zerocrossf_local;                             // �½��� + ����ʱ�ӵ�NTB
    HI_U32 ntb_zerocrossr_net;                               // ������+ ȫ��ʱ�ӵ�NTB
    HI_U32 ntb_zerocrossf_net;                               // �½���+ ȫ��ʱ�ӵ�NTB

    HI_U32 time_error1_cnt;                                  // timer_err = 1ʱ���������һ��
    HI_U32 time_error2_cnt;                                  // timer_err = 2ʱ���������һ��
    HI_U32 time_error3_cnt;                                  //timer_err = 3ʱ���������һ��

    HI_U32 photocoupler_error_cnt;                           // �������ж������ʧ�˱��أ���ʾ���������쳣����һ�Σ�оƬ��һ�������ӿڱ��������ܴ���

    HI_U8 o_valid;
    HI_U8 gear_switch;
    HI_U8 t_in_valid_cnt;                                    // ÿ�����㣬8�������У�������Ч��·����ÿ��Ƶ����ˢ��һ��
    HI_BOOL is_power_period_stat_err;                        // ��Ƶ����ͳ���Ƿ��쳣

    HI_U32 power_period_err_offset;                          // ��Ƶ����ͳ���쳣ʱ��ֵ

    HI_U32 ntb_result;                                       // ���������ϱ���NTB

    HI_U16 line_noise[DIAG_CMD_TF_VOLTAGE_DETECT_LINE_CNT];  // оƬ���ϱ�����
}DIAG_CMD_TF_CHIP_STATICS_STRU;

// �ؼ��ڵ����2.0 ��ά�ɲ���Ϣ
typedef struct _DIAG_CMD_QUERY_FL_INFO_STRU_
{
    HI_U16 curr_block_idx;
    HI_U16 curr_rec_nums_in_block;

    HI_U32 curr_block_offset;
    HI_U32 total_add_nums;
    HI_U32 total_offline_nums;
    HI_U32 total_delete_nums;
    HI_U32 total_change_proxy_nums;
    HI_U32 recover_block_times;
    HI_U32 recover_start_block_index;
    HI_U32 recover_block_nums;
    HI_U32 dfx_report_lock_times;
    HI_U32 dfx_report_unlock_times;
    HI_U32 block_size;

    HI_U16 block_grp_nums;
    HI_U16 block_nums_in_grp;

    HI_U8  is_recording;
    HI_U8  pad[3];

}DIAG_CMD_QUERY_FL_INFO_STRU;

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
#define FL_TOPO_REF_ENTRY_MAX  (2000)   //2000�汾�޸�
#define HI_SHORT_PLC_MAC_ADDR_LEN  (HI_PLC_MAC_ADDR_LEN - 2)
// Ԥ������׼���˵�block��
#define HI_SDM_FL_TOPO_REF_BLOCK_NUM    (2)

// �洢��ͷ����Ϣ
#define BLOCK_REC_TYPE_TOPO_NOT_USED    (0)
#define BLOCK_REC_TYPE_TOPO_REF         (1)
#define BLOCK_REC_TYPE_TOPO_CHANGE      (2)

#define FL_TOPO_CHANGE_STA_ADD              (0) //վ������
#define FL_TOPO_CHANGE_STA_DELETE           (1) //վ��ɾ��
#define FL_TOPO_CHANGE_STA_OFFLINE          (2) //վ������
#define FL_TOPO_CHANGE_STA_CHANGE_PROXY     (3) //վ�������

typedef struct _fl_topo_record_parser_st_
{
    HI_U16 recode_type  : 4;
    HI_U16 pad          : 12;
    HI_U16 pad1;
}fl_topo_record_parser_st;

// վ��������¼
typedef struct _fl_topo_add_record_st_  // 12 bytes
{
    HI_U16 recode_type  : 4;
    HI_U16 tei          : 12;
    HI_U16 proxy_tei    : 12;
    HI_U16 pad          : 4;
    HI_U32 time_stamp;
    HI_U8  mac[HI_SHORT_PLC_MAC_ADDR_LEN];

} fl_topo_add_record_st;


#define FL_TOPO_DELETE_REASON_LEAVE         (0)
#define FL_TOPO_DELETE_REASON_WHITE_LIST    (1)
// վ��ɾ����¼
typedef struct _fl_topo_delete_record_st_  // 8 bytes
{
    HI_U16 recode_type  : 4;
    HI_U16 tei          : 12;
    HI_U8  reason;
    HI_U8  pad;
    HI_U32  time_stamp;
} fl_topo_delete_record_st;

// վ�����߼�¼
typedef struct _fl_topo_offline_record_st_  // 8 bytes
{
    HI_U16 recode_type  : 4;
    HI_U16 tei          : 12;
    HI_U8  pad[2];
    HI_U32  time_stamp;
} fl_topo_offline_record_st;

// վ���������¼
typedef struct _fl_topo_change_proxy_record_st_  // 8 bytes
{
    HI_U16 recode_type  : 4;
    HI_U16 tei          : 12;
    HI_U16 proxy_tei    : 12;
    HI_U16 pad          : 4;
    HI_U32 time_stamp;

} fl_topo_change_proxy_record_st;

// �ؼ��ڵ���Ϣ��׼������HSO�ӿ�

#define MAC_PLC_QUERY_FL_TOPO_REF_VER_RO   (0)
#define MAC_PLC_QUERY_FL_TOPO_REF_VER_R1   (1)

#define MAC_PLC_QUERY_FL_TOPO_REC_VER_RO   (0)
#define MAC_PLC_QUERY_FL_TOPO_REC_VER_R1   (1)

#define DIAG_CMD_QUERY_FL_TOPO_REF_ENTRY_MAX   (45)


// �ؼ��ڵ���ϻ�׼����
typedef struct _fl_topo_ref_entry_   // 8 bytes
{
    HI_U8  mac[HI_SHORT_PLC_MAC_ADDR_LEN];                  // վ��MAC��ַ

    HI_U32 tei          : 12;       // վ��TEI��ַ
    HI_U32 proxy_tei    : 12;       // վ�㸸����TEI��ַ
    HI_U32 in_use       : 1;        // վ���Ƿ�����
    HI_U32 pad          : 7;
}fl_topo_ref_entry;

typedef fl_topo_ref_entry DIAG_CMD_QUERY_FL_TOPO_REF_ENTRY_STRU  ;

typedef struct _DIAG_CMD_QUERY_FL_TOPO_REF_IND_STRU_   // 376 bytes
{
    HI_U32 fl_ver;                      //�ؼ��ڵ�������԰汾��
    HI_U32 time_stamp;                  //��׼��������ʱ���

    HI_U16 sn;
    HI_U16 total_cnt        : 10;       //��Ҫ�ϱ���վ������
    HI_U16 is_last          : 1;        //������������һ��Ӧ���ı�ʶ
    HI_U16 pad              : 5;

    HI_U32 num              : 6;        //��ǰ���ݰ�������վ����
    HI_U32 pad1             : 26;

    DIAG_CMD_QUERY_FL_TOPO_REF_ENTRY_STRU entry[DIAG_CMD_QUERY_FL_TOPO_REF_ENTRY_MAX];

} DIAG_CMD_QUERY_FL_TOPO_REF_IND_STRU;


// �ؼ��ڵ���Ϣ���˱仯��HSO�ӿ�
typedef struct _DIAG_CMD_QUERY_FL_TOPO_RECORD_HEAD_S_   // 20 bytes
{
    HI_U32 fl_ver;                      //�ؼ��ڵ�������԰汾��
    HI_U32 total_packet_cnt;
    HI_U32 packet_index;
    HI_U32 buffer_size;
    HI_U16 sn;
    HI_U16 is_last      :1;
    HI_U16 pad          :15;

} DIAG_CMD_QUERY_FL_TOPO_RECORD_HEAD_S;

// MAX_NDM_FRAGMENT_SIZE  388
#define FL_TOPO_RECORD_BUF_SIZE   (388 - sizeof(DIAG_CMD_QUERY_FL_TOPO_RECORD_HEAD_S))

typedef struct _DIAG_CMD_QUERY_FL_TOPO_RECORD_IND_STRU_  // MAX_NDM_FRAGMENT_SIZE bytes
{
    DIAG_CMD_QUERY_FL_TOPO_RECORD_HEAD_S head;
    HI_U8 buffer[FL_TOPO_RECORD_BUF_SIZE];

} DIAG_CMD_QUERY_FL_TOPO_RECORD_IND_STRU;

// ��չؼ��ڵ���ϢHSO�ӿ�
typedef struct _DIAG_CMD_CLEAR_TOPO_RECORD_IND_STRU_  // 8 bytes
{
    HI_U32 ret;
    HI_U32 pad;
} DIAG_CMD_CLEAR_TOPO_RECORD_IND_STRU;

// ��ȡͣ����ά����Ϣ�ظ�
typedef struct
{
    HI_U32 ret;

    HI_U32 cur_cycle_len;           // ��ǰ�������ʱ������λs
    HI_U32 cur_cycle_pass_len;      // �������Ѿ���ʱ������λs
    HI_U32 total_report_on_num;     // ���ϱ��������
    HI_U32 total_found_off_num;     // �ܷ���ͣ�����
    HI_U32 total_report_off_num;    // ���ϱ�ͣ�����

    HI_U16 leave_info_num;          // ����ͳ�Ƽ�¼����
    HI_U8  is_init;
    HI_U8  pad;
    HI_U32 last_arrange_time;       // ��һ����������ͳ�Ƽ�¼��ʱ��

    HI_U32 heartbeat_duration_th;   // δ�յ�����ʱ�����ޣ���λs
    HI_U32 unknown_num;             // δ֪վ����Ŀ
    HI_U32 protect_num;             // ����վ����Ŀ
    HI_U32 on_num;                  // ����վ����Ŀ
    HI_U32 found_off_num;
    HI_U32 report_off_num;          // �ϱ�������Ŀ
    HI_U32 pad1[2];
} DIAG_CMD_CCO_GET_POWEROFF_INFO_IND_STRU;

// ��ȡָ��վ��ͣ����ά����Ϣ����
typedef struct
{
    HI_U32 tei;
} DIAG_CMD_GET_STA_POWEROFF_INFO_REQ_STRU;

// ��ȡָ��վ��ͣ����ά����Ϣ�ظ�
typedef struct
{
    HI_U32 ret;

    HI_U8  status; // ͣ��״̬���ο�nm_poweroff_status_e
    HI_U8  protect_online_round;
    HI_U16 report_on_cnt;   // ���ֲ��ϱ��������
    HI_U16 found_off_cnt;   // ����ͣ�����
    HI_U16 report_off_cnt;  // �ϱ�ͣ�����(ͨ�����߼��)
    HI_U32 last_off_time;   // �������ͣ��ʱ��
    HI_U32 last_on_time;    // ��������ϵ�ʱ��

    HI_U8  lllast_channel_leave_cnt : 4;    // �������������ŵ�ԭ�����ߴ���
    HI_U8  llast_channel_leave_cnt  : 4;    // �����������ŵ�ԭ�����ߴ���
    HI_U8  last_channel_leave_cnt   : 4;    // ���������ŵ����ߴ���
    HI_U8  cur_channel_leave_cnt    : 4;    // ���������ŵ����ߴ���

    HI_U16 pad1[21];
} DIAG_CMD_GET_STA_POWEROFF_INFO_IND_STRU;

// ��ȡվ��ͣ����̼�¼����
typedef struct
{
    HI_U32 tei;
} DIAG_CMD_CCO_GET_POWEROFF_HISTORY_REQ_STRU;

typedef struct
{
    HI_U16  proxy_tei;              // ����վ���TEI
    HI_U16  childsum;               // �ӽڵ�ĸ���
    HI_U8   level;                  // �㼶
    HI_U8   sta_phase[3];           // վ����λ
    HI_U8   ability;                // �ο� IS_CCO �궨��
    HI_BOOL new_joiner;             // �Ƿ�Ϊ���ָռ����վ��,�������ʱ��������վ��,��������������ߴ�����ж���
    HI_BOOL in_use;                 // վ���Ƿ����ߵı�־
    HI_BOOL is_relay_ability;       // �Ƿ�Ϊ�м�
    HI_U8   d2d_proxy_comm_rate;    // ·��ͨ����
    HI_U8   proxy_type;             // ��������:��̬����ָ����̬����
    HI_U8   topo_down_comm_rate;    // ����ͨ�ųɹ���
    HI_U8   topo_up_comm_rate;      // ����ͨ�ųɹ���
    HI_U32  addtime;                // վ�����ʱ��
    HI_U32  proxy_change_cnt;       // ����������
    HI_U32  leave_cnt;              // ���ߴ���
    HI_U32  leave_time;             // ����ʱ��㣬��λ:��
    HI_U32  leave_total_time;       // ������ʱ������λ:��
    HI_U16  hw_reset_cnt;           // ��¼�豸Ӳ��λ�ۻ�����
    HI_U16  mac_sw_reset_cnt;       // ��¼MAC��λ�ۻ�����
    HI_U8   leave_reason;           // ����ԭ��
    HI_U8   hw_reset_reason;        // Ӳ����ԭ��
    HI_U8   sw_reset_reason;        // ������ԭ��
    HI_U8   pn_identify_fail_cnt;   // ����߷���ʶ��ʧ�ܴ���
    HI_U32  last_heartbeat_time;    // ���һ������ʱ��,��λs

    HI_U8  min_multiproxy_comm_rate;        // ·����Сͨ����
    HI_U8  proxy_comm_rate;                 // �����վ�������ͨ����
    HI_U8  proxy_down_comm_rate;            // �����վ�������ͨ����
    HI_U8  comm_rate;                       // ������ͨ����
    HI_U8  down_comm_rate;                  // ����ͨ�ųɹ���
    HI_U8  up_comm_rate;                    // ����ͨ�ųɹ���
    HI_U8  proxy_proxy_comm_rate;           // ·��ͨ����
    HI_U8  channel_info;                    // �ŵ�����(beacon֡����)
    HI_S16 snr;                             // ƽ�������(SOF֡)
    HI_U8  is_exist : 1;                    // ͨ������·�ɴ���ͳ�����ݣ�ȷ����վ���Ƿ�Ϊ��վ�㷢��վ��
    HI_U8  network_mode : 1;                // ����ģʽ:0.PLCģʽ��1.RFģʽ
    HI_U8  is_dual_module : 1;              // �Ƿ�Ϊ˫ģģ��:0.��ģģ�飬1.˫ģģ��
    HI_U8  rsvd1 : 5;
    HI_U8  proxy_channel_info;              // �����վ����ŵ�����
    HI_U32 send_fail_cnt;                   // ����ʧ�ܴ���
    HI_U32 send_success_cnt;                // ���ͳɹ�����
    HI_U32 rcv_beacon_cnt;                  // ���ո�վ��BEACON֡�ĸ���
    HI_U32 rcv_cnt;                         // ���յ���վ�㷢����վ��ĵ�������(DTEI == my_tei)
} diag_poweroff_his_node_part_st;


// ��ȡվ��ͣ����̼�¼�ظ�
typedef struct
{
    HI_U32 ret;

    HI_U32  his_begin_addr;

    HI_U8   mac[HI_PLC_MAC_ADDR_LEN];       // MAC��ַ
    HI_U16  report_on_cnt;              // ���ֲ��ϱ��������
    HI_U16  found_off_cnt;              // ����ͣ�����
    HI_U16  report_off_cnt;             // �ϱ�ͣ�����(ͨ�����߼��)
    HI_U32  last_off_time;              // �������ͣ��ʱ��
    HI_U32  last_on_time;               // ��������ϵ�ʱ��
    HI_U32  last_finish_protect_time;   // �����������ʱ��

    diag_poweroff_his_node_part_st off_record;
    diag_poweroff_his_node_part_st on_record;
    diag_poweroff_his_node_part_st finish_protect_record;
} DIAG_CMD_CCO_GET_POWEROFF_HISTORY_IND_STRU;

#endif

#define NM_POWEROFF_SAVE_HIS_NUM            (49)
#define NM_DIAG_DISCOVER_TEI_BIT_MAP        (256)

// ��ѯվ�����Ӧ��ͣ��̽�⵱ǰ��Ϣ 280B
typedef struct
{
    HI_U32  ret;

    HI_U8   is_in_delay;
    HI_U8   pad;
    HI_U16  rcv_eq_report_cnt;
    HI_U32  cur_delay_time;     // ��ǰ�ӳ�ʱ�䣬��λs

    HI_U16  begin_delay_cnt;
    HI_U16  continue_delay_cnt;
    HI_U16  broadcast_hb_succ_cnt;
    HI_U16  broadcast_hb_fail_cnt;
    HI_U32  boradcast_hb_ret_code;
    HI_U16  mac_list_param_null_cnt;
    HI_U16  helpler_null_or_not_sta_cnt;
    HI_U16  no_new_tei_cnt;
    HI_U16  reach_eq_hb_th_cnt;
    HI_U16  rcv_eq_hb_total_cnt;
    HI_U16  help_delay_timeout_fail_cnt;
    HI_U16  get_tei_by_mac_fail_cnt;
    HI_U16  need_report_map_memset_cnt;

    HI_U16  sta_rcv_heartbeat_cnt;
    HI_U16  pad1;

    HI_U8   need_report_tei_map[NM_DIAG_DISCOVER_TEI_BIT_MAP];  // ����delay�У�����ҪӦ���վ��
} DIAG_CMD_STA_CUR_HELP_REPORT_IND_STRU;

// ��ѯվ�����Ӧ��ͣ��̽����ʷ��Ϣ 280B
typedef struct
{
    HI_U32  ret;

    HI_U8   last_broadcast_map[NM_DIAG_DISCOVER_TEI_BIT_MAP];   // �ϴι㲥λͼ
    HI_U8   his_report_tei_map[NM_DIAG_DISCOVER_TEI_BIT_MAP];   // ��ʷ����Ӧ��ļ�¼

} DIAG_CMD_STA_HIS_HELP_REPORT_IND_STRU;

//ά����Ϣ�ṹ��
typedef struct
{
    HI_U32 ndm_connect_create_search_cnt;
    HI_U32 ndm_connect_rcv_search_cnt;
    HI_U32 ndm_connect_create_sync_cnt;
    HI_U32 ndm_connect_rcv_sync_cnt;
    HI_U32 ndm_connect_send_diag_data_cnt;
    HI_U32 ndm_connect_rcv_diag_data_cnt;

    HI_U8 cur_freq;
    HI_U8 state;
    HI_U16 wait_diag_data_timeout_cnt;

    HI_U8 access_mac[HI_PLC_MAC_ADDR_LEN];
    HI_U16 access_tei;
    HI_U32 access_snid;

    HI_U16 last_access_tei;
    HI_U16 pad;

}DIAG_CMD_NDM_CONNECT_DFX_INFO_STRU;

// ̽��idά������
typedef struct
{
    HI_U32 ndm_detectid_create_bcn_cnt;
    HI_U32 ndm_detectid_rcv_diag_data_cnt;
    HI_U32 ndm_detectid_rcv_bcn_cnt;

    HI_U8 cur_freq;
    HI_U8 state;
    HI_U8 last_report_state;
    HI_BOOL is_rcv_detect_response;

    HI_U8 access_mac[HI_PLC_MAC_ADDR_LEN];
    HI_U16 wait_diag_data_timeout_cnt;

}DIAG_CMD_NDM_DETECTID_DFX_INFO_STRU;

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)

// �������Ľṹ
typedef struct
{
    HI_U16 ostei;
    HI_U16 discover_count_tei;
    HI_U32 discover_count;
    HI_U8  discover_tei[NM_DIAG_DISCOVER_TEI_BIT_MAP];
    HI_U8  rsvd[2];
} diag_heartbeat_check_st;


// ��ѯҶ�ӽڵ������ӳ�ת����Ϣ 200B
typedef struct
{
    HI_U32 ret;

    HI_U8   is_in_trans_delay;  // �Ƿ���ת���ӳ���
    HI_U8   pad1;
    HI_U16  start_trans_timer_cnt;
    HI_U16  direct_trans_cnt;
    HI_U16  merge_another_hb_cnt;
    HI_U16  trans_handle_timeout_fail_cnt;
    HI_U16  trans_heartbeat_succ_cnt;
    HI_U16  trans_heartbeat_fail_cnt;
    HI_U16  trans_heartbeat_ret_code;

    diag_heartbeat_check_st trans_heart_beat;  // ��ת������������

    HI_U32  pad[10];
} DIAG_CMD_STA_HB_TRANS_DELAY_IND_STRU;


#endif

/*--------------�����Ϣ START--------------------------------------------*/
/************************************************************************/
/*   MACЭ��ջ   �����ϢID    ��0x1000~0x5000��                        */
/************************************************************************/
// ��Χ [0x1000~0x5000)
// 1�� ����ҵ����Ϣ  [0x1000~0x1400) // 1024
// 2,  ·��          [0x1400~0x1800) // 1024
// 3,  ���ͻ�        [0x1800~0x1C00) // 1024
// 4,  �ֶ���        [0x1C00~0x1E00) // 512
// 5,  ������        [0x1E00~0x2000) // 512
// 6,  beacon        [0x2000~0x2200) // 512
// 7,  conv          [0x2200~0x2400) // 512
// 8,  RX            [0x2400~0x2600) // 512
// 9,  NTB           [0x2600~0x2700) // 256
// 10, SOUNDING      [0x2700~0x2800) // 256
// 11, ������        [0x2800~0x2900) // 256
// 12, DFX           [0X2900~0X2A00) // 256
// 13, FA            [0x2A00~0x2B00)
// 14, ����          [0x2B00~0x5000)
/************************************************************************/
//
//  ID�ŵĸ�ʽΪ: HI_DMID_PS_XXX_XXX
//  ��Ӧ�Ľṹ��:    DMID_PS_XXX_XXX_S
//

/***********************************************************************
 13�� Ƶ������Ӧ��Ϣ  [0x2A00~0x2B00)
************************************************************************/
#define HI_DMID_MAC_FA_BASE_ID                      0x2A00
#define HI_DMID_MAC_FA_STA_RESULT                   (0x2A00 + 0x00)  //վ����������

//Ƶ��������ÿ��վ��Ĳ��Խ����Ϣ����
typedef struct
{
    HI_U8  evaluate_mode;                                        //��ǰ������ģʽ
    HI_U8  cur_freq;                                             //��ǰƵ��
    HI_U16 tei;                                                 //��վ���tei

    HI_U16 rcv_cnt;                                             //�յ��Ĳ��Ա�����
    HI_U16 pad;

    HI_U32 total_time;                                          //����ʱ����ʱ����λ:ms
}HI_DMID_MAC_FA_STA_RESULT_S;

/************************************************************************/
/* 1�� ����ҵ����Ϣ  [0x1000~0x1400) // 1024 */
/************************************************************************/
#define ID_DIAG_LAYER_PS_NM_IGNORE                   (0x1000)
#define ID_DIAG_LAYER_PS_NM_FORMING                  (0x1001)
#define ID_DIAG_LAYER_PS_NM_COORDINATE               (0x1002)
#define ID_DIAG_LAYER_PS_NM_CHANGEPROXY              (0x1003)
#define HI_DMID_MAC_STA_REJOIN_TIME_NO_OK            (0x1004)
#define HI_DMID_MAC_GET_MAC_BY_TEI                         (0x1005)
#define HI_DMID_MAC_GET_LEVEL_BY_TEI                       (0x1006)
#define HI_DMID_MAC_GET_NEXT_HOP                             (0x1007)
#define HI_DMID_MAC_ADD_ROUTE_ITEM                         (0x1008)
#define HI_DMID_MAC_ADD_ROUTE_ITEM_BY_PROXY        (0x1009)

#define HI_DMID_MAC_STA_SEND_HEART_BEAT                 (0x100B)


typedef struct
{
    HI_U16 tei;
    HI_U16 proxy_tei;
    HI_U32 bpc;
}HI_DMID_MAC_STA_SEND_HEART_BEAT_S;

typedef struct
{
    HI_U16 tei;
    HI_U16 proxy_tei;
    HI_U32 bpc;
}HI_DMID_MAC_CCO_RCV_HEART_BEAT_S;

typedef struct
{
    HI_U32 assoc_last_bpc;
    HI_U8 req_send_bpc_interval;
    HI_U8 rsvd[3];
    HI_U32 curent_bpc;
}HI_DMID_MAC_STA_REJOIN_TIME_NO_OK_S;

typedef struct
{
    HI_U8 mac[6];
    HI_U16 tei;
}HI_DMID_MAC_GET_MAC_BY_TEI_S;

typedef struct
{
    HI_U8 level;
    HI_U8 rsvd;
    HI_U16 tei;
}HI_DMID_MAC_GET_LEVEL_BY_TEI_S;

typedef struct
{
    HI_U16 next_hop;
    HI_U16 odtei;
    HI_U16 my_tei;
    HI_U8 rsvd;
    HI_BOOL result;
}HI_DMID_MAC_GET_NEXT_HOP_S;

typedef struct
{
    HI_U16 odtei;
    HI_U16 next_hop;
    HI_U16 my_tei;
    HI_U8 rsvd;
    HI_U8 result;
}HI_DMID_MAC_ADD_ROUTE_ITEM_S;

typedef struct
{
    HI_U16 tei;
    HI_U16 tei_proxy;
    HI_U16 my_tei;
    HI_U8 rsvd;
    HI_U8 result;
}HI_DMID_MAC_ADD_ROUTE_ITEM_BY_PROXY_S;


/************************************************************************/
// 2,  ·��          [0x1400~0x1800) // 1024
/************************************************************************/
#define ID_DIAG_LAYER_PS_ROUTE                       (0x1400)
#define ID_DIAG_LAYER_PS_ROUTE_AGING                 (0x1401)
#define ID_DIAG_LAYER_PS_ROUTE_FEEDBACK              (0x1402)
#define ID_DIAG_LAYER_PS_ROUTE_DETECT                (0x1403)
#define ID_DIAG_LAYER_PS_ROUTE_EVALUATE              (0x1404)


/************************************************************************/
// 3,  ���ͻ�        [0x1800~0x1C00) // 1024
/************************************************************************/
//���ͻ�ģ����Ϣ����
#define HI_DMID_MAC_TX_RECEIVE_EVENT                            (0x1800)
#define HI_DMID_MAC_TX_RCTS_PACKET_GENERATE                     (0x1801)
#define HI_DMID_MAC_TX_BEACON_MPDU_INFO                         (0x1802)
#define HI_DMID_MAC_TX_MPDU_INFO                                (0x1803)
#define HI_DMID_MAC_TX_TDMA_PERIODINFO                          (0x1804)
#define HI_DMID_MAC_TX_TIMER_SET_INFO                           (0x1805)
#define HI_DMID_MAC_TX_WAIT_FRAME_SET_INFO                      (0x1806)
#define HI_DMID_MAC_TX_SEND_ENABLE_INFO                         (0x1807)
#define HI_DMID_MAC_TX_MPDU_SYMBOL_NUM_INFO                     (0x1808)
#define HI_DMID_MAC_TX_TONEMAP_QUERY_INFO                       (0x1809)
#define HI_DMID_MAC_TX_MPDU_TMI_INFO                            (0x1810)
#define HI_DMID_MAC_TX_PHASE_SWITCH_INFO                        (0x1811)
#define HI_DMID_MAC_TX_GET_MPDU_FL_INFO                         (0x1812)
//���ͻ�ģ����Ϣ����
typedef struct
{
    HI_U32 cur_ntb;
    HI_U32 src_mod;
    HI_U32 event_group;
    HI_U32 cur_event;
}HI_DMID_MAC_TX_RECEIVE_EVENT_S;


typedef struct
{
    HI_U32 cur_ntb;
    HI_U16 dtei;
    HI_U16 rsvd;
    HI_U32 duration;
    HI_U32 result;
}HI_DMID_MAC_TX_RCTS_PACKET_GENERATE_S;

typedef struct
{
    HI_U32 cur_ntb;
    HI_U8   mpdu_type;
    HI_U8   discard_flag;
    HI_U8   phase;
    HI_U8   exist_flag;
}HI_DMID_MAC_TX_BEACON_MPDU_INFO_S;

typedef struct
{
    HI_U8             mpdu_type;
    HI_U8             send_flag;
    HI_U8             discard_flag;              //MPDU���ĵĶ�����־;
    HI_U8             pb_count;                  //PB��ĸ���;

    HI_U8             phase;                     //��λ��Ϣ;
    HI_U8             bcf;                       //���ĵĹ㲥��־λ;
    HI_U16            mpdu_index;                //MPDU������ֵ;

    HI_U16            pb_size;                   //PB��Ĵ�С;
    HI_U16            ctei;                      //����վ����ն��豸��ʶ��;

    HI_U16            fc_size;                   //���ĵ�֡ͷ��С;
    HI_U16            odtei;                     //Ŀ���ն��豸��ʶ��;

    HI_U32            expire_time;               //���ĵĹ���ʱ��;

    HI_U8             lid;
    HI_U8             encrypt_flag;
    HI_U8             rsvd;
    HI_U8             mpdu_queue_phase;          //���������ڶ���λģʽ�£�������е���λ��Ϣ;

    HI_U8             cur_resend ;
    HI_U8             total_resend;
}HI_DMID_MAC_TX_MPDU_INFO_S;

/************************************************************************/
// 4,  �ֶ���        [0x1C00~0x1E00) // 512
/************************************************************************/


/************************************************************************/
// 5,  ������        [0x1E00~0x2000) // 512
/************************************************************************/

/************************************************************************/
// 6,  beacon        [0x2000~0x2200) // 512
/************************************************************************/


/************************************************************************/
// 7,  conv          [0x2200~0x2400) // 512
/************************************************************************/
#define HI_DMID_MAC_SEND_MSDU_TO_NM           (0x2200)
#define HI_DMID_MAC_ROUTE_LEARNIG             (0x2201)
#define HI_DMID_MAC_GET_TEI_FROM_TOPO         (0x2202)
#define HI_DMID_MAC_GET_TEI_FROM_PATH_TABLE   (0x2203)

typedef struct
{
    HI_U32 bpc;
    HI_U32 ntb;
    HI_U32 msdu_size;
}HI_DMID_MAC_SEND_MSDU_TO_NM_S;

typedef struct
{
    HI_U16 ostei;
    HI_U16 stei;
}HI_DMID_MAC_ROUTE_LEARNIG_S;

typedef struct
{
    HI_U8 mac[6];
    HI_U16 tei;
}HI_DMID_MAC_GET_TEI_FROM_TOPO_S;

typedef struct
{
    HI_U8 mac[6];
    HI_U16 tei;
}HI_DMID_MAC_GET_TEI_FROM_PATH_TABLE_S;



/************************************************************************/
// 8,  RX            [0x2400~0x2600) // 512
/************************************************************************/
#define HI_DMID_MAC_RX_RCV_FC                    (0x2400)
#define HI_DMID_MAC_RX_BEACON_STATUS             (0x2401)
#define HI_DMID_MAC_RX_SOF_STATUS                (0x2402)


typedef struct
{
    HI_U32  ntb;
    HI_S32  avg_snr;
    HI_U8   dt;
    HI_U8   expect_dt;
    HI_U16  stei;

    HI_U16  dtei;
    HI_U8   snid;
    HI_U8   freq;
}HI_DMID_MAC_RX_RCV_FC_INFO_S;
typedef struct
{
    HI_BOOL pb_decode_result;
    HI_BOOL pb_crc_result;
    HI_U8   snid;
    HI_U8   pad;
    HI_U16  stei;
    HI_U16  pad2;
    HI_U32  bpc;
}HI_DMID_MAC_RX_BEACON_STATUS_S;
typedef struct
{
    HI_BOOL pb_decode_result;
    HI_BOOL pb_crc_result;
    HI_U8   snid;
    HI_U8   pad;
    HI_U16  stei;
    HI_U16  pad2;
    HI_U32  bpc;
}HI_DMID_MAC_RX_SOF_STATUS_S;
/************************************************************************/
// 9, NTB            [0x2600~0x2700) // 256
/************************************************************************/

/************************************************************************/
// 10, SOUNDING      [0x2700~0x2800) // 256
/************************************************************************/

/************************************************************************/
// 11, ������        [0x2800~0x2900) // 256
/************************************************************************/
#define HI_DMID_MAC_DCM_RCV_REMOTE_CMD_INFO                     (0x2800)
#define HI_DMID_MAC_DCM_PIPE_TRANS_PK_INFO                      (0x2801)
#define HI_DMID_MAC_DCM_PIPE_REPLY_ACK_INFO                     (0x2802)
#define HI_DMID_MAC_DCM_PIPE_PK_ARRIVED_DEST_STA_INFO           (0x2803)
#define HI_DMID_MAC_DCM_PIPE_RECV_PK_FROM_PLC_INFO              (0x2804)
#define HI_DMID_MAC_DCM_SEG_QUEUE_IN_FAIL_INFO                  (0x2805)
#define HI_DMID_MAC_DCM_TX_SUCCESS_INFO                         (0x2806)
#define HI_DMID_MAC_DCM_TX_FAIL_INFO                            (0x2807)
#define HI_DMID_MAC_DCM_TX_TIMEOUT_INFO                         (0x2808)
#define HI_DMID_MAC_DCM_RSB_TIMEOUT_INFO                        (0x2809)
#define HI_DMID_MAC_BUFF_DISP_INFO                              (0x280a)

// ������ͨ����������HSO��Զ������
typedef struct
{
    HI_U8  ucRole;  // CCO ,STA
    HI_U8  ucMacAddr[4][6];
    HI_U8  ucchannelType;
    HI_U16 usControl  :15;          // ������
    HI_U16 usDULink   :1;           // �����б�ʶ

    HI_U16 usCommandId;
    HI_U16 usTransId;
    HI_U16 payload_size;
    HI_U16 pad;

    HI_U32 sn;
}HI_DMID_MAC_DCM_RCV_REMOTE_CMD_INFO_S;

//������ͨ��ת��������Ϣ
typedef struct
{
    HI_U8  ucRole;  // CCO ,STA
    HI_U8  ucOSAMac[6];
    HI_U8  ucProxyMac[6];
    HI_U8  ucMacAddr[4][6];
    HI_U8  oda[6];
    HI_U8  ucchannelType;

    HI_U8  type;
    //HI_U8  pad;
    HI_U8  pkt_idx;             //������ͨ����������
    HI_U16 usTransId;

    HI_U16 odtei;
    //HI_U16 usControl;
    HI_U16 usControl  :15;              // ������
    HI_U16 usDULink   :1;               // �����б�ʶ

    HI_U16 usCommandId;
    HI_U16 usFragmentID;
    HI_U16 usFragmentCount;             // �ְܷ���
    HI_U16 usFragmentTransId; // һ�ηְ�ʹ��ͬһ��TransId.��ֵͨ��HI_DIAG_GetFragmentTransId��ȡ
    //HI_U16 pad2;

    HI_U32 sn;

    HI_U16 payload_size;
    HI_U16 pad2;
}HI_DMID_MAC_DCM_PIPE_TRANS_PK_INFO_S;

//������ͨ���ظ�ACK��Ϣ
typedef struct
{
    HI_U8  ucRole;  // CCO ,STA
    HI_U8  ucOSAMac[6];
    HI_U8  ucProxyMac[6];
    HI_U8  ucMacAddr[4][6];
    HI_U8  oda[6];
    HI_U8  ucchannelType;

    HI_U8  type;
    HI_U8  pad;
    HI_U16 usTransId;

    HI_U16 odtei;
    //HI_U16 usControl;
    HI_U16 usControl  :15;          // ������
    HI_U16 usDULink   :1;           // �����б�ʶ

    HI_U16 usCommandId;
    HI_U16 usFragmentID;

    HI_U16 usFragmentCount;// �ְܷ���
    HI_U16 usFragmentTransId; // һ�ηְ�ʹ��ͬһ��TransId.��ֵͨ��HI_DIAG_GetFragmentTransId��ȡ
    //HI_U16 pad2;
}HI_DMID_MAC_DCM_PIPE_REPLY_ACK_INFO_S;

//������ͨ�����ĵ���Ŀ��վ����Ϣ
typedef struct
{
    HI_U8  ucRole;  // CCO ,STA
    HI_U8  ucOSAMac[6];
    HI_U8  ucProxyMac[6];
    HI_U8  ucMacAddr[4][6];
    HI_U8  ucchannelType;
    HI_U8  type;
    //HI_U8  pad;
    HI_U8  pkt_idx;

    HI_U16 usTransId;
    //HI_U16 usControl;
    HI_U16 usControl  :15;          // ������
    HI_U16 usDULink   :1;           // �����б�ʶ

    HI_U16 usCommandId;
    HI_U16 usFragmentID;

    //HI_U16 usFragmentCount;            // �ְܷ���
    HI_U16 usFragmentCount       : 9;    // �ְܷ���
    HI_U16 rsb_pkt_flag          : 1;    //0:����Ҫ���һ�����ϱ�;1:��Ҫ���һ�������İ��ϱ�
    HI_U16 pad                   : 6;    //������Ĭ��ֵΪ0
    HI_U16 usFragmentTransId; // һ�ηְ�ʹ��ͬһ��TransId.��ֵͨ��HI_DIAG_GetFragmentTransId��ȡ
    //HI_U16 pad2;
}HI_DMID_MAC_DCM_PIPE_PK_ARRIVED_DEST_STA_INFO_S;

//������ͨ����������PLC������Ϣ
typedef struct
{
    HI_U8  ucOSAMac[6];
    HI_U8  ucProxyMac[6];
    HI_U8  ucMacAddr[4][6];

    HI_U8  ucchannelType;
    HI_U8  type;
    HI_U16 usTransId;

    //HI_U16 usControl;
    HI_U16 usControl  :15;          // ������
    HI_U16 usDULink   :1;           // �����б�ʶ
    HI_U16 usCommandId;

    HI_U16 usFragmentID;

    //HI_U16 usFragmentCount;       // �ְܷ���
    HI_U16 usFragmentCount : 9;     // �ְܷ���
    HI_U16 rsb_pkt_flag : 1;        //0:����Ҫ���һ�����ϱ�;1:��Ҫ���һ�������İ��ϱ�
    HI_U16 pad1 : 6;                //������Ĭ��ֵΪ0

    HI_U16 usFragmentTransId;       // һ�ηְ�ʹ��ͬһ��TransId.��ֵͨ��HI_DIAG_GetFragmentTransId��ȡ
    //HI_U16 pad;
    HI_U8  pkt_idx;
    HI_U8  pad;

    HI_U32 sn;

    HI_U16 payload_size;
    HI_U16 pad2;
}HI_DMID_MAC_DCM_PIPE_RECV_PK_FROM_PLC_INFO_S;

//������������ֶζ���ʧ��
typedef struct
{
    HI_U8 pkd_idx;          //���ı��
    HI_U8 pad[3];
}HI_DMID_MAC_DCM_SEG_QUEUE_IN_FAIL_INFO_S;

//���������ķ��ͳɹ�
typedef struct
{
    HI_U8 pkd_idx;          //���ı��
    HI_U8 pad[3];
}HI_DMID_MAC_DCM_TX_SUCCESS_INFO_S;

//���������ķ���ʧ��
typedef struct
{
    HI_U8 pkd_idx;          //���ı��
    HI_U8 pad[3];
}HI_DMID_MAC_DCM_TX_FAIL_INFO_S;

//���������ķ��ͳ�ʱ
typedef struct
{
    HI_U8 pkd_idx;          //���ı��
    HI_U8 pad[3];
}HI_DMID_MAC_DCM_TX_TIMEOUT_INFO_S;


//�������������鳬ʱ
typedef struct
{
    HI_U8 pkd_idx;          //���ı��
    HI_U8 pad[3];
}HI_DMID_MAC_DCM_RSB_TIMEOUT_INFO_S;

#define DFX_BUFF_DISP_SIZE_MAX          64
typedef struct
{
    HI_U16 idx;
    HI_U16 size;

    HI_U32 buff[DFX_BUFF_DISP_SIZE_MAX >> 2];
}HI_DMID_MAC_BUFF_DISP_INFO_S;
//�����������������ͳ����
typedef struct
{
    HI_U8  follow_ccomac[HI_PLC_MAC_ADDR_LEN];          //���ø���CCO��MAC��ַ
    HI_U8  cur_freq;                                    //��ǰ����Ƶ��
    HI_U8  snid;
    HI_U8  ndm_cur_status;                              //��������ǰ״̬:����������
    HI_U8  follow_status;                               //��ǰ����״̬:�����У������ϣ�����ʧ��
    HI_U8  set_follow_snid;                             //�·��ĸ���SNID
    HI_U8  set_follow_mode;                             //�·��ĸ���ģʽ
    HI_U8  set_follow_cmd_cnt;                          //�·��������
    HI_U8  start_follow_cnt;                            //�·������������������
    HI_U8  stop_follow_cnt;                             //�·���ֹͣ�����������
    HI_U8  start_follow_success_cnt;                    //�·�������������ɹ�����
    HI_U8  stop_follow_success_cnt;                     //�·�ֹͣ��������ɹ�����
    HI_U8  start_follow_fail_by_timer_fail_cnt;         //�·�����ʧ�ܴ���
    HI_U8  start_follow_fail_by_invalid_para_cnt;       //���ڲ����������������ʧ�ܴ���
    HI_U8  start_follow_fail_by_cur_is_follow_cnt;      //���ڵ�ǰ�Ѿ��������ٴ��·������ִ�д���
    HI_U8  switch_freq_in_full_mode_cnt;                //һ����������У�Ƶ�α�������
    HI_U8  notify_switch_freq_cnt;                      //ȫ��֪ͨ�л�Ƶ�δ���
    HI_U8  continue_follow_fail_cnt;                    //��������ʧ�ܴ���
    HI_U8  resume_cnt;                                  //ֹͣ�������ָ���������
    HI_U8  pad[2];
    HI_U16 rcv_valid_follow_snid_cnt;                   //���������յ�����SNID���ű���������V1R1�汾
    HI_U32 rcv_pb_cnt;                                  //����ģʽ�£��յ�pB��
    HI_U16 rcv_valid_cco_mac_cnt;                       //���������յ�����CCO�ĸ���
    HI_U16 tei;                                         //��������ȡ���õĿ���ʱ϶��Ӧ��TEI
    HI_U32 starttime;                                   //��������ȡ���õĿ���ʱ϶����ʼʱ��
    HI_U32 duration;                                    //��������ȡ���õĿ���ʱ϶����:��λ΢��
    HI_U32 rcv_valid_slot_cnt;                          //���������ÿ���ʱ϶����
    HI_U32 follow_switch_freq_cnt;                      //�����У��л�Ƶ�δ���
    HI_U32 follow_timer_callback_cnt;                   //���ڼ�ⶨʱ���ص�����
    HI_U32 start_timer_fail_cnt;                        //����Timerʧ�ܴ���
} DIAG_CMD_MAC_NDM_FOLLOW_STAT_STRU;

typedef struct
{
    HI_U8  ccomac[HI_PLC_MAC_ADDR_LEN];    //�����CCO��MAC��ַ
    HI_U8  freq;                           //���������freq
    HI_U8  snid;
    HI_U32 mode;                           //��������ģʽ���̶����棬�л�Ƶ�θ���
    HI_U32 cmd;                            //�������棬ֹͣ���������
}DIAG_CMD_MAC_NDM_SET_FOLLOW_REQ_STRU;
#define FL_FOLDER_AND_FILE_NAME_LEN   (25)
#define FL_MAX_TEI                                         (1016)

//HSO�·���������ֹͣ����ѯ���϶�λ����
typedef enum
{
    HI_MAC_NOTIFY_START_FL_CMD = 1,            //֪ͨ�������϶�λ����
    HI_MAC_NOTIFY_STOP_FL_CMD,                    //ֹ֪ͨͣ���϶�λ����
    HI_MAC_NOTIFY_QUERY_FL_STATUS_CMD,   //��ѯ��ǰ���϶�λ����״̬
}DIAG_CMD_SET_FAULT_LOCAT_ENUM;

//���϶�λ��ǰ����״̬
typedef enum
{
    HI_MAC_FAULT_LOCAT_STOP,   //��ǰΪ��λ����״̬
    HI_MAC_FAULT_LOCAT_ON,       //��ǰΪ��λ��
}DIAG_CMD_FAULT_LOCAT_STATUS_ENUM;

//���϶�λ�汾��
typedef enum
{
    FAULT_LOCAT_ERROR = 0,
    FAULT_LOCAT_VER1,
    FAULT_LOCAT_VER2,
    FAULT_LOCAT_VER3,
}DIAG_MAC_FAULT_LOCAT_VER_ENUM;

// �������϶�λ������
typedef enum
{
    HI_MAC_FAULT_LOCAT_ERR_SUCCESS = HI_ERR_SUCCESS,
    HI_MAC_FAULT_LOCAT_ERR_DISABLE = 1,           //��ǰ�������������������϶�λ
    HI_MAC_FAULT_LOCAT_ERR_NOT_SUPPORT,   //��ǰ������ģʽ�£���֧�ֹ��϶�λ
    HI_MAC_FAULT_LOCAT_ERR_ALREADY_START,//��ǰ�Ѿ����ڹ��϶�λ״̬
    HI_MAC_FAULT_LOCAT_ERR_ALREADY_STOP, //��ǰ�Ѿ�ֹͣ��λ
    HI_MAC_FAULT_LOCAT_ERR_MAC,                    //�·���MAC�����Ƿ�
    HI_MAC_FAULT_LOCAT_ERR_FREQ,                  //�·���Freq�����Ƿ�
    HI_MAC_FAULT_LOCAT_ERR_SNID,                  //�·���SNID�����Ƿ�
    HI_MAC_FAULT_LOCAT_ERR_FOLDER_NAME, //�·����ļ����Ƿ�
    HI_MAC_FAULT_LOCAT_ERR_SD_NO_EXIST,  // SD��������
    HI_MAC_FAULT_LOCAT_ERR_SD_NO_MORE_SPACE, //SD��ʣ��ռ䲻��
    HI_MAC_FAULT_LOCAT_ERR_SD_SAME_NAME,           //SD�����ļ����ظ�
}DIAG_MAC_FAULT_LOCAT_ERR_ENUM;

//д��SD���Ŀ�����Ϣ
typedef struct
{
    HI_U16 write_sd_cnt;                //д��SD������
    HI_U16 cco_hw_reset_cnt;          //CCOӲ��������

    HI_U8 stop_reason;                 //ֹͣ����¼��ԭ��
    HI_U8 pad[3];

    HI_U32 cco_join_time;               //CCO��������ʱ��
    HI_U32 run_durtion;                    //���϶�λʱ��
    HI_U32 sys_time;                      //��¼�����˵�ϵͳʱ��

    HI_U8 freq;                                 //������λʱ����freq
    HI_U8 snid;                                //������λʱ����SNID
    HI_U8 change_freq;                  //ȫ��֪ͨNDM�л���freq
    HI_U8 change_freq_info[FREQ_TRY_NUM];
    HI_U8 rsvd[3];
}DIAG_MAC_FAULT_LOCAT_RECODE_CTRL_STRU;

//д��SD����������Ϣ
typedef struct
{
    DIAG_MAC_FAULT_LOCAT_RECODE_CTRL_STRU fl_ctrl_info;
    NFC_DIAG_CMD_QUERY_NETWORK_TOPO_ENTRY_S network_topo_entry[FL_MAX_TEI];
}DIAG_MAC_FAULT_LOCAT_NETWORK_TOPO_INFO_STRU;

//ֹͣ��λԭ��
typedef enum
{
    NDM_POWER_FAIL = 0,                                      //����������
    STOP_BY_CCO_RESET,                                       //CCO����
    STOP_BY_CONTINUS_NOT_RCV_TOPO,           //����ˢ������ʧ�ܣ�ֹͣ
    STOP_BY_HSO,                                                  //HSO�·�ֹͣ
    STOP_REASON_MAX,
}DIAG_MAC_FAULT_LOCAT_STOP_REASON_ENUM;

//HSO�·����������ֹͣ���������϶�λ
typedef struct
{
    HI_U8 mode;
    HI_U8 freq;
    HI_U8 snid;
    HI_U8 mac[HI_PLC_MAC_ADDR_LEN];
    HI_U8 pad[2];
    HI_CHAR  folder_name[FL_FOLDER_AND_FILE_NAME_LEN];   //�ļ�������
}DIAG_CMD_FAULT_LOCAT_REQ_STRU;

//HSO�·�����Ļ�Ӧ����
typedef struct
{
    HI_U32 ret_code;  //���صĴ�����
    HI_U32 mode; //�·�����
}DIAG_CMD_FAULT_LOCAT_RSP_STRU;

//HSO �·���ѯ���϶�λ����Ӧ����
typedef struct
{
    HI_U8   fault_locat_status;                         //���������϶�λ��ǰ״̬
    HI_U8   freq;
    HI_U8   snid;
    HI_U8   mac[HI_PLC_MAC_ADDR_LEN];
    HI_CHAR folder_name[FL_FOLDER_AND_FILE_NAME_LEN];   //�ļ�������
    HI_U8   pad[2];
    HI_U32  run_durtion;                                //��λ����ʱ��
}DIAG_CMD_QUERY_FAULT_LOCAT_RSP_STRU;

typedef struct
{
    HI_U8  mac[HI_PLC_MAC_ADDR_LEN];                // MAC��ַ
    HI_U8  freq;                                    // Ƶ��
    HI_U8  snid;                                    // �����
    HI_U8  curr_status;                             // ��ǰ״̬
    HI_U8  hso_start_cnt;                           // ������ϴ���
    HI_U8  hso_stop_cnt;                            // ֹͣ��ϴ���
    HI_U8  hso_query_cnt;                           // ��ѯ��Ͻ������
    HI_U8  stop_by_not_rcv_topo_cnt;                // ���ڽ��յ�������Ϣֹͣ����
    HI_U8  stop_by_cco_reset_cnt;                   // ����CCO��λֹͣ����
    HI_U16 cco_reset_cnt;                           // CCO��λ����
    HI_U32 cco_join_time;                           // CCO����ʱ��
    HI_U8  sd_check_exist_fail_cnt;                 // SD����ⲻ���ڼ���
    HI_U8  sd_space_not_enough_cnt;                 // SD���ռ䲻���ڼ���
    HI_U8  sd_set_time_fail_cnt;                    // SD������ʱ��ʧ�ܼ���
    HI_U8  sd_folder_same_name_fail_cnt;            // SD���ļ����ظ�����
    HI_U8  write_topo_to_sd_fail_cnt;               // ����д��SD��ʧ�ܼ���
    HI_U8  malloc_fail_cnt;                         // ����ռ�ʧ��
    HI_U8  change_freq;                             // Ƶ���л�
    HI_U8  change_freq_info[FREQ_TRY_NUM];          // Ƶ���л���Ϣ
    HI_U8  rsvd[3];
    HI_U8  invalid_parameter_cnt;                   // �����Ƿ�����
    HI_U8  create_folder_fail_cnt;                  // �����ļ���ʧ�ܼ���
    HI_U8  continus_not_rcv_topo_cnt;               // �������ղ������˼���
    HI_U8  hso_stop_write_sd_cnt;                   // ֹͣдSD������
    HI_U16 send_refresh_topo_cnt;                   // ��������ˢ�¼���
    HI_U16 topo_write_sd_cnt;                       // ����дSD������
    HI_U32 send_remote_cnt;                         // ����Զ���������
    HI_U32 send_remote_null_para_cnt;               // ����Զ������ղ�������
    HI_U16 send_remote_malloc_fail_cnt;             // ����Զ����������ռ�ʧ�ܼ���
    HI_U16 send_remote_fail_cnt;                    // ����Զ������ʧ�ܼ���
    HI_U32 send_msg_fail_cnt;                       // ������Ϣʧ�ܼ���
    HI_U32 run_time;                                // ����ʱ��
    HI_U8  get_share_mem_fail_cnt;                  // ��ȡ�����ڴ�ʧ�ܼ���
    HI_U8  pad[2];
    HI_CHAR folder_name[FL_FOLDER_AND_FILE_NAME_LEN]; // �ļ�������
}DIAG_CMD_QUERY_FAULT_LOCAT_STAT_STRU;

typedef struct
{
    HI_U8  cco_mac[HI_PLC_MAC_ADDR_LEN];             // ���������CCO MAC��ַ
    HI_U8  lock_status;                              // ����״̬
    HI_U8  lock_status_identify;                     // ̨��ʶ����������״̬

    HI_U32 lock_begin_time;                          // ������ʼʱ��
    HI_U32 lock_duration;                            // ����ʱ��
    HI_U32 reject_time;                              // ��������ܾ�ʱ��

    HI_U32 lock_cnt;                                 // ��������
    HI_U32 reject_cnt;                               // ��������ܾ�����
    HI_U32 lock_timeout_cnt;                         // ������ʱ��ʱ
    HI_U16 lock_fail_by_not_join_cnt;                // δ������������ʧ��
    HI_U16 send_msg_fail_cnt;                        // ��Ϣ����ʧ��
    HI_U8  lock_freq_identify;                       // ��������Ƶ�κ�
    HI_U8  rsv;
    HI_U8  cco_mac_identify[HI_PLC_MAC_ADDR_LEN];    // ̨��ʶ�����������CCO MAC��ַ
    HI_U32 lock_snid_identify;                      //  �������������
    HI_U32 lock_begin_time_by_identify;              // ������ʼʱ�䣬��λS
    HI_U32 lock_freqsnid_duration_by_identify;       // ����ʱ������λS
    HI_U32 lock_ccomac_duration_by_identify;         // ����CCO MACʱ������λS
    HI_U32 unlock_time_by_identify;                  // ����ʱ��(ά��)
    HI_U32 lock_freqsnid_cnt_by_identify;            // ̨��ʶ������Ƶ�κ�����Ŵ���(ά��)
    HI_U32 lock_ccomac_cnt_by_identify;              // ̨��ʶ������CCO MAC����(ά��)
    HI_U32 unlock_cnt_by_identify;                   // ̨��ʶ���������(ά��)
}DIAG_CMD_QUERY_LOCK_NETWORK_STAT_STRU;

typedef struct
{
    HI_U32 ulRc; //return code:0:success, other value fail
}DIAG_CMD_UNLOCK_NETWORK_STATUS_ACK_STRU;


typedef struct
{
    HI_U8  cmd;
    HI_U8  synch_snid;
    HI_U16 synch_tei;
    HI_U8  synch_mac[6];
    HI_U8  pad[2];
}DIAG_CMD_SOFT_CALC_NTB_TEST_REQ_STRU;
typedef struct
{
    HI_U32 ret;
}DIAG_CMD_QUERY_CALC_NTB_TEST_IND_STRU;

#define DFX_PTPC_SET_METERADDR          (1)
#define DFX_PTPC_SET_STRONG_POWER       (2)
#define DFX_PTPC_SET_BATTERY_POWER      (3)
#define DFX_PTPC_SEND_PACKEET           (4)
typedef struct
{
    HI_U8  cmd;
    HI_U8  data_size;                       //���ݳ���
    HI_U8  comm_meteraddr[HI_PLC_MAC_ADDR_LEN];
}DIAG_CMD_PTPC_TEST_REQ_STRU;
typedef struct
{
    HI_U32 ret;
}DIAG_CMD_PTPC_TEST_INFO_STRU;

#ifdef PRODUCT_CFG_ONLINE_TEST_MODE
// GT ��������SD�洢���Խ��
#define GT_TEST_FILE_SIZE_MAX        (0x1400000)   //20M
#define GT_TEST_FILE_IDENTIFIER_LEN  (16)
#define GT_TEST_FILE_IDENTIFIER      "GT_TEST_RESULT"

typedef struct
{
    HI_U32 time_stamp;

    HI_U32  tei         :12;
    HI_U32  tmi         :4;
    HI_U32  is_ext_tmi  :1;
    HI_U32  freq        :5;
    HI_U32  pad         :10;
}DIAG_CMD_QUERY_GT_TEST_RESULT_ENTRY_STRU;

typedef struct
{
    HI_CHAR identifier[GT_TEST_FILE_IDENTIFIER_LEN];
    HI_U32 gt_text_ver;
    HI_U32 file_create_time;
    HI_U32 file_last_update_time;
    HI_U32 entry_num;
    DIAG_CMD_GT_TEST_PARAM_STRU curr_nv_cfg;
    DIAG_CMD_QUERY_GT_TEST_RESULT_ENTRY_STRU result_entry[0];
}GT_TEXT_SD_FILE_TYPE_STRU;

// GT�������Ա��Ĵ洢�ṹ
#define GT_TEST_RESULT_ENTRY_NUM (2000)
typedef struct
{
    HI_U32 avail_cnt;
    DIAG_CMD_QUERY_GT_TEST_RESULT_ENTRY_STRU result_entry[GT_TEST_RESULT_ENTRY_NUM];
}DIAG_CMD_QUERY_GT_TEST_RESULT_STRU;

typedef struct _DIAG_CMD_GT_TEST_CTL_STRU_
{
    HI_U32 eif;
    HI_U32 send_interval_non_robust;
    HI_U32 send_interval_bcn;
    HI_U16 wait_3911_bcn_interval;
    HI_BOOL is_wait_3911_timeout;
    HI_U8  pad;

    HI_U32 tx_enqueue_sof_cnt;
    HI_U32 tx_enqueue_bcn_cnt;
    HI_U32 tx_sent_success_cnt;
    HI_U32 tx_sent_fail_cnt;

    HI_U32 rcv_not_gt_msg_cnt;
    HI_U32 rcv_gt_msg_cnt;
    HI_U8 sending_freq_index;
    HI_U8 sending_tmi_index;
    HI_U8 sending_ext_tmi_index;
    HI_BOOL curr_freq_tmi_all_sent;  // ��ǰ����Ƶ���У�����tmi���Ѿ����Է��͹���
    HI_U32 rcv_test_frame_crc_success_cnt;

    HI_U32 rcv_test_frame_crc_fail_cnt;
    HI_U32 rcv_test_frame_info_success_cnt;
    HI_U32 rcv_test_frame_info_fail_cnt;
    HI_U32 rcv_test_frame_rec_cnt;

    HI_U32 send_cur_times;
    HI_U8  snid;
    HI_BOOL is_proxy_mode;
    HI_BOOL is_sending_test_frame;
    HI_BOOL pad1;
    HI_U16 make_folder_fail_cnt;
    HI_U16 create_file_fail_cnt;
    HI_U16 detect_channel_3518_cnt;
    HI_U16 channel_3518_fail_cnt;
}DIAG_CMD_GT_TEST_CTL_STRU;

typedef struct
{
    HI_U32 tx_status;
    HI_U32 backoff_event_cnt;
    HI_U32 backoff_fail_cnt;

    HI_U32 avail_node_num;

    HI_U32 invoke_tx_en_cnt;
    HI_U32 send_success_cnt;
    HI_U32 send_fail_cnt;

    HI_U32 send_bcn_cnt;
    HI_U32 send_sof_cnt;

    HI_U32 last_msdu_size ;
    HI_U32 msdu_size_illegal_cnt;
    HI_U32 tx_end_interrupt_cnt;

    HI_U32 tx_enqueue_sof_cnt;
    HI_U32 tx_enqueue_bcn_cnt;

    HI_U32 discard_bcn_cnt;
}gt_tx_statistic_info_stru;

typedef struct
{
    HI_U32 rcv_pcs_interrupt_cnt;
    HI_U32 st4_cnt;
    HI_U32 st3_cnt;
    HI_U32 st2_cnt;

    HI_U32 rcv_fc_interrupt_cnt;
    HI_U32 rcv_pb_interrupt_cnt;
    HI_U32 pb1_cnt;
    HI_U32 pb2_cnt;
    HI_U32 rx_end_interrupt_cnt;
    HI_U32 write_colli_interrupt_cnt;
    HI_U32 rx_overflow_cnt;

    HI_U32 fc_ok_cnt ;
    HI_U32 fc_err_cnt;

    HI_U32 fc_decode_fail_cnt;
    HI_U32 fccs_result_fail_cnt;

    HI_U32 rcv_bcn_fc_cnt;
    HI_U32 bcn_pb_handle_cnt;
    HI_U32 bcn_pb_decode_result_fail_cnt;
    HI_U32 bcn_pb_crc_result_fail_cnt;
    HI_U32 rcv_sof_fc_cnt;
    HI_U32 rcv_sack_fc_cnt;
    HI_U32 rcv_sack_pb_cnt;
    HI_U32 sof_pb_handle_cnt;
    HI_U32 sof_pb_decode_result_fail_cnt;
    HI_U32 sof_pb_crc_result_fail_cnt;
    HI_U32 sof_pb_rcv_finished_handle_cnt;

    HI_U32 other_pb_handle_cnt;

    HI_U32 in_proxy_mode_rcv_gt_bcn_cnt;
    HI_U32 normal_mode_rcv_3911_bcn_cnt;
    HI_U32 rcv_neighbor_bcn_cnt;
    HI_U32 rcv_expected_bcn_cnt;

    HI_U32 rcv_expected_sof_cnt;
    HI_U32 rcv_unxpected_sof_cnt;
}gt_rx_statistic_info_stru;

typedef struct
{
    HI_U16 my_tei;
    HI_U16 sync_tei;

    HI_U16 stat0_4_cnt;
    HI_U16 stat1_2_cnt;

    HI_U16 stat3_5_cnt;
    HI_U16 stat6_cnt;

    HI_U16 detail_sta_ok_cnt;
    HI_U16 detail_sta_null_cnt;

    HI_U16 good_sync_sta_ok_cnt;
    HI_U16 good_sync_sta_null_cnt;

    HI_U32 ntb_reinit_cnt;
    HI_U32 ntb_switch_preamble_cnt;
    HI_U32 ntb_stat_window_num;

    HI_U32 good_sync_delta_bts_error_cnt;
    HI_U32 detail_stat_delta_bts_error_cnt;

    HI_U32 freqerr_exception_cnt;
}gt_ntb_statistic_st;
#endif
/************************************************************************/
// 12, DFX           [0X2900~0X2A00) // 256
/************************************************************************/


/*--------------�����Ϣ END--------------------------------------------*/
/*����TXģ�������*/
typedef struct
{
    HI_U32 beacon_enqueue_cnt;          // BEACON֡����и���
    HI_U32 sof_enqueue_cnt;             // SOF֡����и���
    HI_U32 sack_enqueue_cnt;            // sack֡����и���
    HI_U32 link_test_cnt;               // ��·���Ա��Ķ��и���
    HI_U32 send_beacon_cnt;             // ����BEACON֡����
    HI_U32 send_sof_cnt;                // ����SOF֡����
    HI_U32 send_sack_cnt;               // ����sack֡����
    HI_U32 send_short_beacon_cnt;       // ���Ͷ�beacon���ĸ���
    HI_U32 send_link_test_cnt;          // ������·���Ա��ĸ���
    HI_U32 send_beacon_fragment_cnt;    // ����BEACON֡��Ƭ����
    HI_U32 send_sof_fragment_cnt;       // ����SOF֡��Ƭ����
    HI_U32 send_sack_fragment_cnt;      // ����sack֡��Ƭ����
    HI_U32 backoff_success_cnt;         // �˱ܳɹ�����
    HI_U32 backoff_fail_cnt;            // �˱�ʧ�ܴ���
    HI_U32 send_timeout_cnt;            // ���ͳ�ʱ����
    HI_U32 creat_backoff_timer_fail_cnt;// �����˱ܶ�ʱ��ʧ�ܴ���
    HI_U32 creat_send_timer_fail_cnt;   // �������ͳ�ʱ��ʱ��ʧ�ܴ���
    HI_U32 creat_sack_timer_fail_cnt;   // �����ȴ�sack��ʱ��ʱ��ʧ�ܴ���
    HI_U32 over_queue_threshold_cnt;    // ��������ˮ�߱��Ķ����ܼ���;
    HI_U32 packet_timeout_err_cnt;      // ���ĳ�ʱ����
    HI_U8  current_pk_cnt;              // ��ǰ�����б��ĸ���
    HI_U8  pad[3];
    HI_U32 send_command_cnt;            // �����������
    HI_U32 write_tx_buf_cnt;            // д���ͻ������
    HI_U32 sack_timout_cnt;             // sack��ʱ����
    HI_U32 write_tx_buf_fail;           // дbuffʧ�ܼ���
    HI_U32 rf_chip_reset_cnt;           // rfоƬ��������
    HI_U32 rf_send_timeout_reset_cnt;   // ���ͳ�ʱ��������
    HI_U32 send_beacon_pb_fail_cnt;     // ����beacon pb��ʧ�ܼ���

}DIAG_CMD_QUERY_RF_TX_STAT_STRU;

/*����RXģ�������*/
typedef struct
{
    HI_U32 rcv_beacon_cnt;                     // ����BEACON֡����
    HI_U32 rcv_sof_cnt;                        // ����SOF֡����
    HI_U32 rcv_short_beacon_cnt;               // ���ն�beacon���ĸ���
    HI_U32 rcv_link_test_cnt;                  // ���ն���·��ⱨ�ĸ���
    HI_U32 rcv_beacon_fragment_cnt;            // ����BEACON֡��Ƭ����
    HI_U32 rcv_sof_fragment_cnt;               // ����SOF֡��Ƭ����
    HI_U32 beacon_crc_fail_cnt;                // BEACON֡У��ʧ�ܸ���
    HI_U32 short_beacon_fail_cnt;              // ��BEACON֡ʧ�ܸ���
    HI_U32 link_test_fail_cnt;                 // ����·��ⱨ��ʧ�ܸ���
    HI_U32 rcv_incomplete_cnt;                 // ���ղ��������ĸ���
    HI_U32 rcv_beacon_incomplete_cnt;          // �����ű�֡����������
    HI_U32 rcv_invalid_cnt;                    // �����쳣����
    HI_U32 interrupt_cnt;                      // �жϸ���
    HI_U32 preamble_interrupt_cnt;             // ǰ���жϸ���
    HI_U32 send_complete_interrupt_cnt;        // ��������жϸ���
    HI_U32 rcv_interrupt_cnt;                  // �����жϸ���
    HI_U32 tx_buf_empty_cnt;                   // ���ͻ���մ���
    HI_U32 rx_buf_full_cnt;                    // ���ջ���������
    HI_U32 error_cmd_cnt;                      // �����������
    HI_U32 fifo_flow_cnt;                      // �շ������������
    HI_U32 rcv_sof_incomplete_cnt;             // �����ű�֡����������
    HI_U32 rcv_crc_fail_cnt;                   // �ж��ϱ�crc�������
    HI_U32 rcv_rebuild_fail;                   // ��������ʧ��
    HI_U32 rcv_rebuild_incomplete;             // �������鲻����
    HI_U32 rcv_return_sack_packe;              // ����SACK����
    HI_U32 rcv_return_sack_pb;                 // ����PB����
    HI_U32 rcv_pb_err_cnt;                     // ����PB crc�������
    HI_U32 rcv_no_self_pkt_cnt;                // ���յ��Ǳ�վ�㱨����
    HI_U32 rcv_list_err;                       // ���ն��������
    HI_U32 rcv_pkt_leng_err;                   // ���ձ��ĳ��ȴ���
    HI_U16 rcv_pkt_size_too_short_err;         // ���ձ��ĳ��ȹ��̴���
    HI_U16 rcv_pkt_size_too_long_err;          // ���ձ��ĳ��ȹ�������
    HI_U32 recode_path_info_err;               // ��¼RF·����Ϣʧ��
    HI_U32 rcv_difference_bpc_beacon_cnt;      // ���յ����ű����ڲ�ͬ���ű����
    HI_U32 rcv_self_snid_short_beacon_cnt;     // ���յ��Լ�������ű����
    HI_U32 rcv_beacon_max_difference;          // �����ű�֮������ʱ���
    HI_U32 occurence_time_max_difference;      // ���ʱ������ʱ��
    HI_U32 rcv_beacon_min_difference;          // �����ű�֮�����Сʱ���
    HI_U32 rcv_last_beacon_max_difference;     // �ϴ������ű�֮������ʱ���
    HI_U32 last_occurence_time_max_difference; // �ϴ����ʱ������ʱ��
    HI_U32 rcv_last_beacon_min_difference;     // �ϴ������ű�֮�����Сʱ���
    HI_U32 beacon_differenc_big_period_cnt;    // ���ű�֮ǰʱ�������ű�����+30ms�Ĵ���
    HI_U32 beacon_differenc_little_period_cnt; // ���ű�֮ǰʱ���С���ű�����-30ms�Ĵ���

}DIAG_CMD_QUERY_RF_RX_STAT_STRU;
typedef struct
{
    HI_U8    is_dual_module;          // �Ƿ�Ϊ˫ģģ��
    HI_U8    network_mode;            // ����ģʽ
    HI_BOOL  rf_communication;        // RFͨ�����
    HI_U8    plc_communication;       // PLCͨ�����(��ʱδʹ�ã����Ժ���չ)
}DIAG_CMD_NETWORK_MODE_SWITCH_IND_PARA_STRU;

typedef struct
{
    HI_U16 tei : 12;
    HI_U16 network_mode : 1;
    HI_U16 rsvd : 3;

    HI_U8  mac[HI_PLC_MAC_ADDR_LEN];
}DIAG_CMD_QUERY_DUAL_MODULE_ENTRY_S;

#define DFX_DULA_MODULE_ENTRY_MAX   (40)
typedef struct
{
    HI_U16 sn;
    HI_U16 is_last  : 1;
    HI_U16 num      : 15;

    DIAG_CMD_QUERY_DUAL_MODULE_ENTRY_S entry[DFX_DULA_MODULE_ENTRY_MAX];
}DIAG_CMD_QUERY_DUAL_MODULE_STRU;

typedef struct
{
    HI_U8  cco_mac[6];                  // ��������CCO��MAC��ַ
    HI_U8  mac_type;                    // MAC��ַ����
    HI_U8  is_dual_module;              // �Ƿ�Ϊ˫ģģ��

    HI_U8  main_network_snid;           // �������(�ű�֡��payload��Я�����������)
    HI_U8  rf_network_snid;             // RFվ���������
    HI_U8  sub_network_snid;            // RF�������
    HI_U8  network_mode;                // ����ģʽ

    HI_U8  network_mode_policy;         // ����ģʽ����
    HI_U8  pad[3];
}DIAG_CMD_QUERY_STA_INFO_STRU;

typedef struct
{
    HI_U16 tei;
    HI_U16 proxy_tei;               // ����վ��
    HI_U16 counnected_sta_num;      // ����ͨվ�����
    HI_U16 pad;

    HI_U32 connected_graph[64];     // ��ͨͼ
}DIAG_CMD_QUERY_CONNECTED_GRAPH_IND_STRU;

typedef struct
{
    HI_U8 comm_rate_th;                     // ͨ��������
    HI_U8 comm_rate_is_enable;              // �Ƿ�����ͨ��������
    HI_S8 snr_th;                           // ƽ�����������
    HI_U8 snr_is_enable;                    // �Ƿ�����ƽ�����������

    HI_U8 down_comm_rate_th;                // ����ͨ��������
    HI_U8 down_comm_rate_is_enable;         // �Ƿ���������ͨ��������
    HI_U8 up_comm_rate_th;                  // ����ͨ��������
    HI_U8 up_comm_rate_is_enable;           // �Ƿ���������ͨ��������
}DIAG_CMD_QUERY_CONNECTED_GRAPH_REQ_STRU;

#define DFX_RF_SNID_COLLISION_MAX_NUM     (10)
typedef struct
{
    HI_U8 mac[HI_PLC_MAC_ADDR_LEN];
    HI_U16 collision_tei:12;
    HI_U16 sub_snid:4;

    HI_U32 time_stamp;

}DIAG_CMD_RF_SNID_CHANGE_RECORD_STRU;


typedef struct
{
    HI_U32 snid_change_num;
    DIAG_CMD_RF_SNID_CHANGE_RECORD_STRU rf_snid_chang_record[DFX_RF_SNID_COLLISION_MAX_NUM];
}DIAG_CMD_RF_SIND_CHANGE_INFO_STRU;

#define DFX_RF_COMM_RATE_MAX_NUM        (12)
#define DFX_RF_MODE_CHANGE_MAX_NUM      (5)
#define DFX_RF_ASSOC_REQ_MAX_NUM        (20)
typedef struct
{
    HI_U16 tei;
    HI_U8  path_comm_rate;
    HI_U8  avg_rssi;
}DIAG_CMD_RF_RT_PATH_COMM_RATE_RECORD_STRU;

typedef struct
{
    HI_U32 sum_num;
    HI_U32 porxy_comm_rate_lower_down_limit_num;
    HI_U32 porxy_comm_rate_between_20_to_30_num;
    HI_U32 porxy_comm_rate_high_30_num;
    HI_U32 porxy_rssi_lower_down_limit_num;
    HI_U8  my_rf_level;
    HI_U8  pad[3];

}DIAG_CMD_RF_COMM_RATE_STATE_STRU;

typedef struct
{
    DIAG_CMD_RF_COMM_RATE_STATE_STRU          rf_comm_rate_state;
    DIAG_CMD_RF_RT_PATH_COMM_RATE_RECORD_STRU rf_comm_rate_record[DFX_RF_COMM_RATE_MAX_NUM];
}DIAG_CMD_RF_CURRENT_COMM_RATE_STRU;


typedef struct
{
    HI_U32 time_stamp;
    DIAG_CMD_RF_RT_PATH_COMM_RATE_RECORD_STRU rf_comm_rate_record[DFX_RF_COMM_RATE_MAX_NUM];
}DIAG_CMD_RF_MODE_CHANGE_COMM_RATE_ITEM_STRU;

typedef struct
{
    DIAG_CMD_RF_MODE_CHANGE_COMM_RATE_ITEM_STRU rf_mode_change_record[DFX_RF_MODE_CHANGE_MAX_NUM];
}DIAG_CMD_RF_MODE_CHANGE_COMM_RATE_STRU;


typedef struct
{
    HI_U32 starttime;
    HI_U16 tei;
    HI_U8 rssi;
    HI_U8 pad;
}DIAG_CMD_RF_ASSOC_REQ_ITEM_STRU;

typedef struct
{
    HI_U32 record_num;
    DIAG_CMD_RF_ASSOC_REQ_ITEM_STRU rf_assoc_req_item[DFX_RF_ASSOC_REQ_MAX_NUM];
}DIAG_CMD_RF_ASSOC_REQ_INFO_RECORD_STRU;

/* ����CCO����NDM�ļ�¼*/
#define DFX_STAT_CONNECT_NDM_MAX   20   // ͳ��CCO����NDM�Ĵ���
typedef struct
{
    HI_U16 ndm_tei;
    HI_U8 ndm_mac[HI_PLC_MAC_ADDR_LEN];
    HI_U32 cur_time;
}DIAG_CMD_CONNECT_NDM_STRU;

typedef struct
{
    HI_U32 connect_cnt;

    DIAG_CMD_CONNECT_NDM_STRU ndm_connect_info[DFX_STAT_CONNECT_NDM_MAX];
}DIAG_CMD_CONNECT_NDM_STAT_STRU;

typedef struct
{
    HI_U32 info[5];
}DIAG_CMD_RF_DFX_REPORT_STRU;

#define DIAG_CMD_TF_COLLECT_DATA_ZERO_CROSS_NTB_START       0x0000
#define DIAG_CMD_TF_COLLECT_DATA_ZERO_CROSS_NTB_STOP        0x0001

//�������ɼ�����NTB������
typedef struct
{
    HI_U16 cmd;
    HI_U16 time;                                                        //���ݲɼ�ʱ������λs

    HI_U8 rcv_level;
    HI_U8 pad1[3];

    HI_U32 pad[3];
}DIAG_CMD_TF_COLLECT_DATA_ZERO_CROSS_NTB_REQ_STRU;

#define DIAG_CMD_TF_COLLECT_DATA_ZERO_CROSS_NTB_NUM_MAX             95  //�ϱ������а��������NTB����
//�������ɼ�����NTB������
typedef struct
{
    HI_U16 index;                                                       //������������Ŵ�0��ʼ
    HI_U16 num;                                                         //�����е����ݸ���

    HI_U32 pad2;

    HI_U32 data[DIAG_CMD_TF_COLLECT_DATA_ZERO_CROSS_NTB_NUM_MAX];       //NTB����
}DIAG_CMD_TF_COLLECT_DATA_ZERO_CROSS_NTB_RSP_STRU;

#define TF_NOISE_TH_CNT                     6
//̨��ʶ���Ŷ���ͳ����Ϣ
typedef struct
{
    HI_U8 static_wobble_cnt[TF_NOISE_TH_CNT];
    HI_U8 static_wobble_avg[TF_NOISE_TH_CNT];
}DIAG_CMD_TF_WOBBLE_INFO_STRU;

//��̨��ʶ�����з���ʱ�Ŷ�ʱͳ�ƴ���������
#define DIAG_CMD_WOBBLE_WITHOUT_SIGNAL_STORE_WINDOW_CNT_MAX     10

//��̨��ʶ�����з���ʱ���Ŷ�ͳ��
typedef struct
{
    HI_U32 bpc;
    HI_U32 ntb;

    DIAG_CMD_TF_WOBBLE_INFO_STRU sta_noise_statistics[DIAG_CMD_WOBBLE_WITHOUT_SIGNAL_STORE_WINDOW_CNT_MAX]; //10����
}DIAG_CMD_TF_WOBBLE_WITHOUT_SIGNAL_STRU;

//��̨��ʶ�����з���ʱ���Ŷ�ͳ��
typedef struct
{
    HI_U8 seq_index_ascend;                                                         //ʶ�����̨�����б��
    HI_U8 seq_index_descend;
    HI_U8 pad[2];

    HI_U32 bpc_ascend;
    HI_U32 ntb_ascend;
    DIAG_CMD_TF_WOBBLE_INFO_STRU signal_ascend[TF_STA_WINDOW_MAX_CNT];     //������4����
    DIAG_CMD_TF_WOBBLE_INFO_STRU noise_ascend;                                      //������1����

    HI_U32 bpc_descend;
    HI_U32 ntb_descend;
    DIAG_CMD_TF_WOBBLE_INFO_STRU signal_descend[TF_STA_WINDOW_MAX_CNT];    //�½���4����
    DIAG_CMD_TF_WOBBLE_INFO_STRU noise_descend;                                     //�½���1����
}DIAG_CMD_TF_WOBBLE_WITH_SIGNAL_STRU;


/*STAռ�ձȷ��Ϳ�ά�ɲ���Ϣ*/
typedef struct
{
    HI_U8 duty_start_cnt;
    HI_U8 duty_stop_cnt;
    HI_U8 start_fail_cnt;
    HI_BOOL is_duty_on;

    HI_U32 max_sendlength;                // �·���ռ�ձ������ʱ������λus

    HI_U8 duty_percent;                   // �·���ռ�ձȣ�
    HI_U8 original_tx_power;              // ԭʼ���͹���
    HI_U8 adjust_tx_power;                // �������͹���
    HI_U8 check_zero_cross_int_cnt;       // ���յ������жϸ���

    HI_U32 remain_time_not_enough_cnt;
    HI_U32 sack_duty_cnt;
    HI_U32 sound_sack_duty_cnt;
    HI_U32 csma_duty_cnt;
    HI_U32 fresh_record_cnt;
    HI_U32 max_period1_duty_percent;      //����1�����ռ�ձ�
    HI_U32 cur_period1_duty_percent;      // ����1�ڵ�ǰռ�ձ�
    HI_U32 max_period2_duty_percent;      // ����2�����ռ�ձ�
    HI_U32 cur_period2_duty_percent;      // ����2�ڵ�ǰռ�ձ�
    HI_U32 max_period3_duty_percent;      // ����3�����ռ�ձ�
    HI_U32 cur_period3_duty_percent;      // ����3�ڵ�ǰռ�ձ�

    HI_U32 voltage_interrupt_fall_cnt;    // ��ѹ���䴥���жϴ���
    HI_U32 start_check_zero_cross_cnt;    // ��ʼ���м������жϴ���
    HI_U32 voltage_recovery_cnt;          // ��ѹ�ָ�����
    HI_U32 low_voltage_cnt;               // ��⵽�͵�ƽ����
    HI_U32 enable_adjust_tf_policy_cnt;   //��ѹ����������ô���

    HI_U32 false_voltage_fall_cnt;        // ��鵽�ٵĵ�ѹ���������û�й���
    HI_U32 decrease_tx_power_cnt;         // ��С���͹��ʴ���
    HI_U32 increase_tx_power_cnt;         // �������͹��ʴ���
}DIAG_CMD_TX_DUTY_STATICS_STRU;

#define DFX_MAX_IDENTIFY_RESULT_ENTRY_NUM   (15)
typedef struct
{
    HI_U32 freq : 7;                        // Ƶ�κ�
    HI_U32 snid : 24;                       // �����
    HI_U32 is_valid : 1;                    // �����Ƿ���Ч

    HI_U8 pad0[2];
    HI_U8  cco_mac[HI_PLC_MAC_ADDR_LEN];    // CCO MAC��ַ

    HI_U16 signal_mean;                     // �źž�ֵ
    HI_U16 noise_mean;                      //������ֵ

    HI_U16 soft_val;                        //��ֵ
    HI_U8 seq_type;                         // ��������
    HI_U8 pad;                              //����

    HI_U32 last_refresh_time;               //��һ��ˢ�µ�ʱ�䣬��λs
}DIAG_CMD_HIS_RESULT_ENTRY_STRU;

typedef struct
{
    DIAG_CMD_HIS_RESULT_ENTRY_STRU max;     //��ʷ����Ŷ�
    DIAG_CMD_HIS_RESULT_ENTRY_STRU entry[DFX_MAX_IDENTIFY_RESULT_ENTRY_NUM];
}DIAG_CMD_HIS_RESULT_CACHE_STRU;

//��ǰʶ����
typedef struct
{
    HI_U32 freq : 7;                        // Ƶ�κ�
    HI_U32 snid : 24;                       // �����
    HI_U32 is_valid : 1;                    // �����Ƿ���Ч

    HI_U8 pad0[2];
    HI_U8  cco_mac[HI_PLC_MAC_ADDR_LEN];    //CCO_MAC

    HI_U16 signal_mean;                     //�źž�ֵ
    HI_U16 noise_mean;                      //������ֵ

    HI_U16 soft_val;                        //��ֵ
    HI_U8 seq_type;                         // ��������
    HI_U8 pad;                              //����

    HI_U32 seq_refresh_time;                //������һ��ˢ�µ�ʱ�䣬��λs
    HI_U32 bcn_refresh_time;                //�ű���һ��ˢ�µ�ʱ�䣬��λs
}DIAG_CMD_CUR_RESULT_ENTRY_STRU;

typedef struct
{
    HI_U32 last_seq_time;                   //���һ���յ��Ŷ���ʶ�𣬵�λs
    DIAG_CMD_CUR_RESULT_ENTRY_STRU entry[DFX_MAX_IDENTIFY_RESULT_ENTRY_NUM];
}DIAG_CMD_CUR_RESULT_CACHE_STRU;


//�ɼ�̨��ʶ�����������
// �ɼ�ÿ������
typedef struct
{
    HI_U16 noise_mean;
    HI_U16 rcv_level : 1;
    HI_U16 in_use : 1;
    HI_U16 pad : 14;

    HI_U32 collect_time;
}DIAG_CMD_COLLECT_NOISE_ENTRY_STRU;

#define DFX_MAX_COLLECT_NOISE_ENTRY_NUM   (48)
typedef struct
{
    HI_U32 cur_report_time;
    DIAG_CMD_COLLECT_NOISE_ENTRY_STRU entry[DFX_MAX_COLLECT_NOISE_ENTRY_NUM];
}DIAG_CMD_COLLECT_NOISE_CACHE_STRU;

typedef struct
{
    HI_U16 ptcl_ver;
    HI_U16 pad;
}DIAG_CMD_COORDINATE_CONFLICT_STAT_REQ_STRU;


typedef struct
{
    HI_BOOL is_reported;
    HI_U8   conflict_type;
    HI_U16  exception_by_cco_mac_cnt;

    HI_U32  change_snid;                        // δʹ��
    HI_U32  last_report_time;                   // �ϴ��ϱ���ʱ��
    HI_U32  report_cnt;                         // �ϱ�����
    HI_U32  bpc_exception_cnt;                  // �ű����ڼ����쳣�ܴ���
    HI_U32  cco_mac_exception_cnt;              // CCO MAC��ַ�쳣�ܴ���
    HI_U32  change_snid_time;                   // ���SNID��ʱ��
    HI_U8   conflict_cco_mac[6];                // ��ͻ����CCO MAC��ַ
    HI_U16  exception_by_bpc_cnt;               // �ű����ڼ����쳣����

    HI_U32  bpc_exception_detect_cnt;           // �ű����ڼ����ж���ͻ����
    HI_U32  cco_mac_exception_detect_cnt;       // CCO MAC��ַ�ж���ͻ����

    HI_U32  rcv_query_req_cnt;                  // ���ղ�ѯ�������
    HI_U32  send_query_response_cnt;            // ���Ͳ�ѯ��Ӧ����
    HI_U32  query_sn;                           // ��ѯ���к�
    HI_U32  query_time;                         // ��ѯʱ��
    HI_U32  random_backoff_time;                // ����˱�ʱ��
    HI_U32  send_query_result_time;             // ���Ͳ�ѯ�������ʱ��

    HI_U32  tatol_send_query_req_cnt;           // ���Ͳ�ѯ�������
    HI_U32  rcv_conflict_report_cnt;            // ���ճ�ͻ�ϱ�����
    HI_U32  rcv_query_response_cnt;             // ���ղ�ѯ��Ӧ����
}DIAG_CMD_COORDINATE_CONFLICT_STAT_STRU;

typedef struct
{
    HI_U8  cco_mac[6];                          // CCO MAC��ַ
    HI_U8  form_serial_number;                  // �������к�
    HI_U8  state;                               // ״̬
}DIAG_CMD_COORDINATE_CCO_ENTRY_STRU;

typedef struct
{
    DIAG_CMD_COORDINATE_CCO_ENTRY_STRU cco_entry[6];
}DIAG_CMD_COORDINATE_MULTI_CCO_INFO_STRU;
// ���ö��������ʱ��
typedef struct
{
    HI_U32 listen_time;
}DIAG_CMD_LISTEN_TIME_BEFORE_FIRST_BEA_REQ_STRU;

typedef struct
{
    HI_U32 ret_code;  //���صĴ�����
}DIAG_CMD_LISTEN_TIME_BEFORE_FIRST_BEA_RSP_STRU;

// ��¼STA,NDM�յ���ȫ���л�Ƶ����Ϣ
typedef struct
{
    HI_U8 mac[HI_PLC_MAC_ADDR_LEN];
    HI_U8 freq;
    HI_U8 rcv_freq_bit_map;   // ���յ�������Ƶ�ε�bitmap����1bit��ʾƵ��1����2bit��ʾƵ��2����3bit��ʾƵ��3����4bit��ʾƵ��4����5bit��ʾƵ��5
    HI_U32 rcv_sys_time;
    HI_U32 switch_freq_cnt;
}DIAG_CMD_MAC_SWITCH_FREQ_ENTRY_ST;

typedef struct
{
    HI_U8  usta_state;                          // ��ǰ״̬
    HI_U8  ntb_state;                           // NTBͬ��״̬
    HI_U8  power_supply_mode;                   // ��������
    HI_U8  target_sta_snid;                     // Ŀ��վ�������
    HI_U16 target_sta_tei;                      // Ŀ��վ��TEI��ַ
    HI_U8  meter_addr[6];                       // Ŀ��վ���MAC��ַ

    HI_U8  active_search_state;                 // ��������״̬
    HI_U8  sweep_freq_index;                    // ��ǰʹ��ɨƵ����
    HI_U8  search_frequency_range_cnt;          // �����ִ�
    HI_U8  sta_state;                           // STA״̬

    HI_U32 passive_search_begin_time;           // ����������ʼʱ�䣬��λS
    HI_U32 passive_search_network_time;         // ��ǰ��������ʱ�䣬��λS
    HI_U32 passive_search_freq_time;            // ��ǰƵ������ʱ�䣬��λS
    HI_U32 passive_lock_synch_time;             // ����ͬ������ʱ��
    HI_U32 last_data_transmission_time;         // ������ݴ���ʱ�䣬��λS
    HI_U32 last_listen_time;                    // ������ʱ�䣬��λS
}DIAG_CMD_QUERY_PTPC_STATE_STRU;

typedef struct
{
    HI_BOOL is_search;
    HI_U8  coordinate_listen_state;
    HI_S16 snr;
}DIAG_CMD_PTPC_COORDINATE_ENTRY_STRU;

typedef struct
{
    DIAG_CMD_PTPC_COORDINATE_ENTRY_STRU entry[16];
}DIAG_CMD_QUERY_PTPC_COORDINATE_STRU;

typedef struct
{
    HI_U32 rcv_packet_cnt_from_phy;             // ��PHY���յ����ĸ���
    HI_U32 last_rcv_packet_from_phy_time;       // ����PHY���յ�����ʱ��
    HI_U32 rcv_packet_cnt_from_app;             // ��APP���յ����ĸ���
    HI_U32 last_rcv_packet_from_app_time;       // ����APP���յ�����ʱ��

    HI_U32 discard_packet_cnt;                  // �����ı���
    HI_U32 send_packet_to_app_cnt;              // ���ͱ��ĵ�APP�ı��ĸ���
    HI_U32 send_search_cnt;                     // ��������֡����
    HI_U32 last_send_search_time;               // ���������֡ʱ��

    HI_U16 sta_enter_normal_state_err_cnt;      // STA��������״̬ʱ�쳣����
    HI_U16 pad3;                                // ��������֡����
    HI_U32 pad2;                                // ����������֡ʱ��
    HI_U32 send_search_response_cnt;            // ����������Ӧ֡����
    HI_U32 last_send_search_response_time;      // �����������Ӧ֡ʱ��

    HI_U32 rcv_send_search_response_cnt;        // ����������Ӧ֡����
    HI_U32 last_rcv_search_response_time;       // ������������Ӧ֡ʱ��
    HI_U32 send_synch_frame_cnt;                // ����ͬ��֡����
    HI_U32 last_send_synch_frame_time;          // �����ͬ��֡ʱ��

    HI_U16 rcv_packet_from_app_fail;            // ��APP���ձ���ʧ�ܸ���
    HI_U16 send_packet_to_app_fail_cnt;         // ���ͱ��ĵ�APP�ı���ʧ�ܸ���
    HI_U32 passive_search_freq_cnt;             // ��������Ƶ���л�����
    HI_U32 send_packet_fail_cnt;                // ���ͱ���ʧ�ܵĴ���
    HI_U32 send_fail_to_active_search_cnt;      // ���ͱ���ʧ�ܶ��л�����������״̬�Ĵ���

    HI_U16 heartbeat_timer_cnt;                 //������ʱ������
    HI_U16 send_msg_err_cnt;                    //������Ϣʧ�ܼ���
    HI_U16 start_timer_err_cnt;                 //������ʱ��ʧ�ܼ���
    HI_U16 duplicate_pkt_cnt;                   //�����˵����ظ����ļ���

    HI_U32 pad[6];
}DIAG_CMD_QUERY_PTPC_STAT_STRU;

#define MAX_RECORD_PTPC_STATE_NUM       (10)
typedef struct
{
    HI_U16 index;
    HI_U8  state[MAX_RECORD_PTPC_STATE_NUM];
    HI_U32 switch_time[MAX_RECORD_PTPC_STATE_NUM];
}DIAG_CMD_QUERY_PTPC_STATE_RECORD_STRU;

typedef struct
{
    HI_BOOL is_detecting;                       //�Ƿ�����̽��
    HI_U8  business_state;                      //ҵ��״̬
    HI_U8  pad[2];

    HI_U32 start_cnt;                           //��������
    HI_U32 stop_cnt;                            //ֹͣ����

    HI_U32 cod_send_cnt;                        //Э��֡���ʹ���
    HI_U32 detect_bcn_send_cnt;                 //̽���ű귢�ʹ���
    HI_U32 detect_bcn_rcv_cnt;                  //̽���ű���մ���
    HI_U32 ptcl_sg_to_v100_cnt;                 //Э��汾�ɹ�����ΪV100�Ĵ���
    HI_U32 ptcl_v100_to_sg_cnt;                 //Э��汾��V100��Ϊ�����Ĵ���

    HI_U16 send_msg_err_cnt;                    //������Ϣʧ�ܼ���
    HI_U16 set_business_state_cnt;              //APP����ҵ��״̬�ļ���

    HI_U32 pad1[6];
}DIAG_CMD_QUERY_PTCL_DETECT_STAT_STRU;

typedef struct
{
    HI_U16 ptcl_ver;                            //Э��汾��
    HI_U16 pad1;

    HI_U32 nid;                                 //����nid
    HI_U32 select_nid;                          //���ȵ�nid
    HI_U32 ptcl_detect_nid;                     //Э��̽���nid

    HI_U32 pad2[8];
}DIAG_CMD_REPORT_NETWORK_ATTRIBUTE_V200_STRU;

typedef struct
{
    HI_U16 nbi_start;
    HI_U16 nbi_end;
    HI_U32 power_value;
}DIAG_CMD_EMC_POWERLOADING_CONFIG_REQ_STRU;

typedef struct
{
    HI_U32 ret;
}DIAG_CMD_EMC_POWERLOADING_CONFIG_IND_STRU;


typedef struct
{
    HI_U32 trigger;
}DIAG_CMD_SC_FRAME_STUB_STRU;

typedef struct
{
    HI_U32 sc_frame_in_queue_cnt;
    HI_U32 sc_frame_in_queue_fail_cnt;
    HI_U32 rcv_cnt3;
    HI_U32 rcv_cnt4;
}DIAG_CMD_QUERY_SC_NM_STATISTIC_STRU;

typedef struct
{
    HI_U32 sof_frame_cnt;
    HI_U32 sack_frame_cnt;
    HI_U32 nm_frame_cnt;
	HI_U32 direct_frame_cnt;
}DIAG_CMD_QUERY_SC_TX_STATISTIC_STRU;

typedef struct
{
    HI_U32 sc_pcs_cnt;
    HI_U32 sc_rcv_end_cnt;
    HI_U32 sc_snd_end_cnt;
    HI_U32 sc_crc_err_cnt;
    HI_U32 sc_rcv_not_my_fragment_cnt;
    HI_U32 sc_rcv_my_fragment_cnt;
    HI_U32 sc_rcv_bcn_cnt;
    HI_U32 sc_rcv_sof_cnt;
    HI_U32 sc_rcv_sack_cnt;
    HI_U32 sc_rcv_nm_cnt;
    HI_U32 sc_get_mac_buf_fail_cnt;
    HI_U32 sc_response_sack_cnt;
    HI_U32 sc_rx_report_rsb_cnt;
	HI_U32 sc_rcv_direct_send_pkt_cnt;
}DIAG_CMD_QUERY_SC_RX_STATISTIC_STRU;

typedef struct
{
    HI_U32 rcv_ntb;
    HI_U16 signal_amplitude;
    HI_U8  noise_amplitude;
    HI_U8  rcv_len;

    HI_U8  data[104];
}DIAG_CMD_RX_SC_RCV_INFO_REPORT_STRU;

typedef struct
{
    HI_U32 snd_ntb;
    HI_U8  frame_type;
    HI_U8  snd_len;
    HI_U8  frame_index;
    HI_U8  pad;

    HI_U16 pk_index;
    HI_U16 dtei;
    HI_U16 ostei;
    HI_U16 odtei;

    HI_U8  data[104];
}DIAG_CMD_TX_SC_SND_INFO_REPORT_STRU;

typedef struct
{
    HI_U32 info[5];
}DIAG_CMD_SC_NM_SWITCH_STATUS_STRU;

typedef struct
{
    HI_U32 curr_state;
    HI_U32 next_state;
    HI_U32 alloc_state;
    HI_U32 curr_ntb;
    HI_S32 next_scheduling_time;
    HI_U32 beacon_start_time;
    HI_U32 beacon_period_len;
    HI_U32 next_beacon_period_start_time;
    HI_U32 beacon_slot_count;
    HI_U32 csma_slot_count;
}DIAG_CMD_SC_NM_TDMA_STRU;


typedef struct
{
    HI_U32 pad;
}DIAG_CMD_SC_NM_PASSIVE_ASSOC_STRU;

typedef struct
{
    HI_U8  detect_station_mac[6];
    HI_U16 pad;
}DIAG_CMD_SC_NM_DIRECTIONAL_ASSOC_STRU;

typedef struct
{
    HI_U32 pad;
}DIAG_CMD_SC_NM_ACTIVE_ASSOC_STRU;

typedef struct
{
    HI_U8  search_network_cnt;
    HI_U8  pad[3];
}DIAG_CMD_SC_NM_NETWORK_SEARCH_STRU;

typedef struct
{
    HI_U32 search_network_snid;
}DIAG_CMD_SC_NM_STATION_SEARCH_STRU;

typedef struct
{
    HI_U16 detect_station_tei;
    HI_U16 pad;
}DIAG_CMD_SC_NM_STATION_DETECT_STRU;
typedef struct
{
    HI_U32 snid;
    HI_U16 tei;
    HI_U8  mac[6];
}DIAG_CMD_SC_NM_ASSOCING_STRU;


typedef struct
{
	HI_U32 broadband_send_cnt;         //������ͼ���
	HI_U32 broadband_rcv_cnt;          //������ռ���
	HI_U32 sc_send_cnt;                //���ز����ͼ���
	HI_U32 sc_rcv_cnt;                 // ���ز����ռ���

	HI_U32 sc_report_pkt_cnt;         // ���ز������ϱ�����
	HI_U32 broadband_report_pkt_cnt;  // ��������ϱ�����
	HI_U32 sc_last_send_pkt_ntb;         // ���ز����һ�η���NTBʱ��
	HI_U32 bb_last_send_pkt_ntb;         // ������һ�η���NTBʱ��
	HI_U32 last_report_pkt_ntb;       // ���һ�ν����ϱ�NTBʱ��
	HI_U32 report_fail_cnt;           // �ϱ�ʧ�ܼ���
	HI_U32 sack_report_pkt_cnt;       //��չsack�����ϱ�����

	HI_U32 bb_sof_send_cnt;
	HI_U32 bb_sack_send_cnt;
	HI_U32 app_rcv_cnt;
}DIAG_CMD_FAST_SEND_STRU;


//����ʶ��ͳ����
typedef struct
{
    HI_U8 is_identifying;                              // ����ʶ����
    HI_BOOL is_support_tfp;                            // �Ƿ�֧������ʶ��
    HI_BOOL is_allow_tfp;                              // �Ƿ������������
    HI_U8 query_multi_cnt;                             // ��ѯ�Ƿ���ڶ��������

    HI_U16 identify_start_success_cnt;                 // cco����������ʶ��ɴ���
    HI_U16 identify_start_fail_disable_cnt;            // ��������������

    HI_U16 identify_start_fail_doing_cnt;              // ��������ʶ���У��ٴ�����ʧ��
    HI_U16 identify_start_packet_success_cnt;          // ����ÿ����ʶ�����

    HI_U16 malloc_err_cnt;                             // ����ռ�ʧ��
    HI_U16 send_query_neighbor_info_req_cnt;           // ���Ͳ�ѯ�Ƿ�Ϊ�����籨��

    HI_U16 rcv_query_neighbor_info_req_cnt;            // �յ���ѯ�Ƿ�Ϊ�����籨��
    HI_U16 send_query_sta_ntb_req_cnt;                 // ���Ͳ�ѯ������STA��NTB����

    HI_U16 rcv_query_sta_ntb_req_cnt;                  // �յ���ѯ��վ��STA��NTB����
    HI_U16 rcv_query_sta_ntb_rsp_cnt;

    HI_U16 send_notify_sta_belong_cnt;                 // ����֪ͨSTĄ����������
    HI_U16 rcv_notify_sta_belong_cnt;                  // �յ�֪ͨ��������

    HI_U16 identify_turn_cnt;                          // �����ִ�
    HI_U16 send_query_req_alloc_msdu_buff_err_cnt;     // ���ͱ���ʱ������bufferʧ�ܴ���

    HI_U16 send_msdu_to_tx_err_cnt;                    // ���Ͳ�����ѯ���ģ�����ʧ�ܴ���
    HI_U16 send_msg_err_cnt;                           // ������Ϣʧ�ܴ���

    HI_U16 rcv_query_rsp_err_cnt;                      // ���ջ�Ӧ����ʧ�ܴ���
    HI_U16 rcv_query_rsp_unexpect_cnt;

    HI_U16 query_timer_timeout_cnt;                    // ��ѯ��ʱ���ص�����
    HI_U16 query_timeout_sta_cnt;

    HI_U16 query_one_turn_over_cnt;                    // ��ѯһ��ʶ����ɴ���
    HI_U16 cco_send_broadcast_notify_cnt;              //  CCO�㲥֪ͨ����������ʶ�����

    HI_U16 broadcast_send_neighbor_notify_cnt;         // CCO,STA�㲥֪ͨ��̨����������ʶ�����
    HI_U16 sta_send_unicast_notify_to_cco_cnt;         // STA����֪ͨ��CCO��������ʶ�����

    HI_U16 cco_rcv_unicast_notify_cnt;                 // CCO �յ�STA����֪ͨ�Ĵ���
    HI_U16 start_timer_err_cnt;                        // ����timerʧ�ܴ���

    HI_U16 rcv_notify_start_cnt;                       //  STA�յ�����֪ͨ����
    HI_U16 rcv_neighbor_notify_cnt;                    // �㲥�յ���̨����������

    HI_U32 identify_run_time;                          // ʶ��ʱ��

    HI_U8 one_turn_identify_state;                     // һ��ʶ��״̬
    HI_U8 neighbor_cnt;                                // ��Χ�������
    HI_BOOL is_select_sta;                             // �Ƿ�ָ��������վ��
    HI_BOOL is_have_multi;                             // �Ƿ���ڶ�����

    HI_U8 identify_mode;                               // ����ʶ��ģʽ���μ�TFP_IDENTIFY_MODE_E
    HI_U8 sta_send_notify_proxy_cnt;                   // STAվ�㴴��������ʶ�𴥷����Ĵ���
    HI_U8 rcv_notify_proxy_cnt;                        // CCO,STAվ���յ�����ʶ�𴥷����Ĵ���
    HI_BOOL is_send_notify_start;                      // ����STAվ���Ƿ�������з��ʹ�������

    HI_U16 rcv_cco_ntb_cnt;                            // CCO�յ��ھ�����CCO_NTB����
    HI_U8 not_rcv_cco_ntb_cnt;                         // CCOû���յ��ھ�����CCO_NTB����
    HI_U8 con_not_rcv_cco_ntb_cnt;                     // CCO����10��û���յ�����´���

    HI_U8 phase_identify_pn_err_cnt;                   // ��λʶ��Ϊ��𷴽Ӵ���
    HI_U8 phase_identify_big_zc_diff_cnt;              // ��λʶ��Ϊ��ƫ���
    HI_U8 phase_identify_err_cnt;                      // ʶ����λ����
    HI_U8 phase_identify_fail_cnt;                     // ��λʶ��������

    HI_U32 last_req_result_time;                       // ��һ������NTBʶ����ʱ��

    HI_U16 write_mac_cnt;                              // ����NV��ʶ��������
    HI_U16 pad;
}DIAG_CMD_TF_PASSIVE_STATIC_STRU;

// ����ר�ż�¼����ʶ����ͳ����
typedef struct
{
    HI_BOOL is_identify_suc;                           // �Ƿ�ʶ��ɹ�
    HI_U8 identify_state;                              // ʶ��״̬���μ�
    HI_U8 identify_result;                             // ʶ����,     �μ�HI_MAC_TFP_IDENTIFY_RESULT_E
    HI_U8 belong_freq;                                 // ����freq

    HI_U32 belong_snid;                                // ����SNID

    HI_U8 belong_cco_mac[6];                           // ����cco_mac
    HI_U8 identify_cco_mac[6];                         // ʶ��ʱ��������cco_mac

    HI_U32 identify_in_snid;                           // ʶ��ʱ��������snid

    HI_U8 identify_in_freq;                            // ʶ��ʱ����freq
    HI_U8 pad[3];
}DIAG_CMD_TFP_IDENTIFY_RESULT_STATIC_STRU;

#define DIAG_TFP_STORE_LAST_NSR_NUM                 3  // ����洢��3������������

typedef struct
{
    HI_BOOL is_cur_max_nsr;                            // ���������Ƿ�Ϊ������
    HI_U8 nsr_store_cnt;                               // �����ȴ洢����
    HI_BOOL is_beforehand_valid;                       // �����Ƿ��Ƿ�Ԥ��Ч��������н������
    HI_BOOL is_cur_ntb_err_filter;                     // �����Ƿ����NTB�쳣

    HI_U32 nsr_store_total_cnt;                        // �ܹ������ȴ洢����
    HI_U32 max_nsr_cnt;                                // �������ȴ���
    HI_U32 calc_max_nsr_err_cnt;                       // ����������������
    HI_U32 not_max_nsr_cnt;                       // ���㲻�Ǵ���������

    HI_U32 nsr[DIAG_TFP_STORE_LAST_NSR_NUM];           // ����洢����������������

    HI_U32 ntb_err_filter_cnt;                         // NTB�쳣�˳�����
}DIAG_CMD_TFP_PASSIVE_NSR_NTB_ERR_STATIC_STRU;

#define DIAG_CMD_TFP_REPORT_NTB_NUM          90

typedef struct
{
    HI_U8 mode;
    HI_U8 pad;
    HI_U16 tei;

    HI_U8 phase;
    HI_U8 index;
    HI_U16 total_ntb_num;

    HI_U32 nid;

    HI_U32 ntb[DIAG_CMD_TFP_REPORT_NTB_NUM];
}DIAG_CMD_TFP_REPORT_NTB_STRU;

// ����ʶ���շ��������ͳ��
#define DIAG_TFP_NEIGHBOR_ENTRY_NUM 12

typedef struct
{
    HI_U32 nid : 24;                                  // �ھ�����snid
    HI_U32 freq : 8;                                  // �ھ�����freq

    // CCOָ������������Щվ������ھ��������ͨ��
    HI_U32 select_cur_net_sta : 12;                   // ָ��������վ������ھ��������ͨ��
    HI_U32 select_cur_net_backup_sta : 12;            // ָ��������վ������ھ��������ͨ�ŵı���վ��
    HI_U32 rcv_notify_conut : 8;                      // �յ����ھ�����֪ͨ����

    // ָ�����ھ�����ͨ��վ����Ϣ
    HI_U32 is_in_use : 1;
    HI_U32 is_select_sta : 1;                         // �Ƿ�ָ���������ͨ��
    HI_U32 select_cnt : 3;                            // ָ���������ͨ�Ŵ���
    HI_U32 select_sta : 12;                           // ָ���������ͨ��վ��
    HI_U32 select_backup_sta : 12;                    // ָ��������籸��ͨ��վ��
    HI_U32 is_calc_ntb_diff : 1;
    HI_U32 is_freqerr_valid : 1;
    HI_U32 is_sync_freqoffset_valid : 1;

    // CCO �����ⷢ��CCO_NTB��Ӧ����
    HI_U32 send_phase_a_ntb_rsp_cnt : 11;             // ����A��λ����
    HI_U32 send_phase_b_ntb_rsp_cnt : 11;             // ����B��λ����
    HI_U32 send_phase_c_ntb_rsp_cnt : 10;             // ����C��λ����

    // CCO,STA�յ��ھ�����֪ͨCCO_NTB��Ӧ����
    HI_U32 rcv_neighbor_phase_a_ntb_rsp_cnt : 11;     // �յ��ھ�����A��λ�Ļ�Ӧ��NTB����
    HI_U32 rcv_neighbor_phase_b_ntb_rsp_cnt : 11;     // �յ��ھ�����B��λ�Ļ�Ӧ��NTB����
    HI_U32 rcv_neighbor_phase_c_ntb_rsp_cnt : 10;     // �յ��ھ�����C��λ�Ļ�Ӧ��NTB����

    // �յ��ھ�����㲥������ֹͣ����ʶ��֪ͨ
    HI_U32 rcv_neighbor_notify_cmd_cnt : 11;          // �յ��ھ�����㲥֪ͨ����
    HI_U32 rcv_neighbor_notify_cmd_cnt_by_sta : 11;   // �յ��ھ�����ͨ��STAת����֪ͨ��CCO���յ�
    HI_U32 send_neighbor_notify_cmd_to_cur_cco_cnt : 10;// STA���յ��ھ�����֪ͨ�������͸�CCO

    HI_U32 sta_send_neighbor_phase_a_ntb_to_cur_cco_cnt : 11;// STA�յ�CCO�·�CCO_NTB��Ҫ��STA����ת��
    HI_U32 sta_send_neighbor_phase_b_ntb_to_cur_cco_cnt : 11;// STA�յ�CCO�·�CCO_NTB��Ҫ��STA����ת��
    HI_U32 sta_send_neighbor_phase_c_ntb_to_cur_cco_cnt : 10;// STA�յ�CCO�·�CCO_NTB��Ҫ��STA����ת��

    HI_U32 rcv_neighbor_cco_ntb_cnt : 10;             // ���յ����ھ�����CCO_NTB����
    HI_U32 pad : 22;
}DIAG_CMD_TFP_TRX_STATICS_ENTRY_STRU;

typedef struct
{
    HI_U32 rcv_cur_net_notify_start_cmd_cnt : 10;     // �յ���������������
    HI_U32 rcv_cur_net_notify_stop_cmd_cnt : 5;       // �յ�������ֹͣ����
    HI_U32 is_selected_sta : 1;                       // �Ƿ�ѡ���վ��Ϊ������ͨ��վ��
    HI_U32 select_sta_cnt : 8;                        // ��վ�㱻ָ��Ϊ������ͨ��վ�����
    HI_U32 notify_count : 8;                          // ֪ͨ����

    DIAG_CMD_TFP_TRX_STATICS_ENTRY_STRU entry[DIAG_TFP_NEIGHBOR_ENTRY_NUM];  // �ھ�������Ϣ
}DIAG_CMD_TFP_TRX_STATICS_STRU;

// ����ʶ��HSO���潻���ṹ�壬����
// 1��HSO�����·�����ö��ֵ
typedef enum
{
    HI_MAC_TFP_IDENTIFY_CMD_UNKOWN = 0,
    HI_MAC_TFP_IDENTIFY_CMD_START,        // ��������ʶ������
    HI_MAC_TFP_IDENTIFY_CMD_STOP,         // �·�ֹͣ����ʶ������
    HI_MAC_TFP_IDENTIFY_CMD_CLEAR,        // �·��������ʶ����
    HI_MAC_TFP_IDENTIFY_CMD_MAX,
}HI_MAC_TFP_IDENTIFY_CMD_E;

// 2��CCO�˻�ӦHSO�����ö��ֵ
typedef enum
{
    HI_MAC_TFP_IDENTIFY_ERR_NULL = 0,
    HI_MAC_TFP_IDENTIFY_ERR_SUCCESS,         // ����ʶ��ɹ�
    HI_MAC_TFP_IDENTIFY_ERR_PARAM,           // ��������
    HI_MAC_TFP_IDENTIFY_ERR_FAIL,            // ����ʶ������ʧ��
    HI_MAC_TFP_IDENTIFY_ERR_NOT_SUPPORT,     // CCO��֧������ʶ��
    HI_MAC_TFP_IDENTIFY_ERR_IDENTIFYING,     // ��������ʶ����
    HI_MAC_TFP_IDENTIFY_ERR_TF_DOING,        // ��������ʶ����
    HI_MAC_TFP_IDENTIFY_ERR_BUSINESS_BUSY,   // ����������
    HI_MAC_TFP_IDENTIFY_ERR_SEARCH_METER,    // �����ѱ���
    HI_MAC_TFP_IDENTIFY_ERR_MAX,
}HI_MAC_TFP_IDENTIFY_ERR_E;

// 3��HSO�����·�����ṹ
typedef struct
{
   HI_MAC_TFP_IDENTIFY_CMD_E cmd;          // �·�����ʶ������

   HI_U16 identify_durtion;                // �·���������ʶ�����ʱ�䣬��λmin
   HI_U16 pad;
   HI_U32 pad1[4];
}DIAG_CMD_TFP_IDENTIFY_SET_REQ_STRU;

// 4��CCO�˻�ӦHSO��������ṹ��
typedef struct
{
    HI_MAC_TFP_IDENTIFY_ERR_E ret;        // ��Ӧ�������
    HI_U32 pad;
}DIAG_CMD_TFP_IDENTIFY_RSP_STRU;

// 5��CCO�˻�ӦHSO��������Ž����ѯ�ṹ��
typedef struct
{
    HI_U32 tei : 12;                           // վ��tei
    HI_U32 is_online : 1;                      // վ���Ƿ�����
    HI_U32 result : 4;                         // վ��ʶ����,�μ�HI_MAC_TFP_IDENTIFY_RESULT_E
    HI_U32 pad : 15;

    HI_U8 mac[HI_PLC_MAC_ADDR_LEN];           // վ�� mac
    HI_U8 belong_mac[HI_PLC_MAC_ADDR_LEN];    // ����̨����cco_mac

    HI_U16 collect_ntb_cnt;                   // ��sta�ռ���ntb����
    HI_U16 var_rate;                          // �����ֵ
    HI_U16 rate_of_process;                   // ʶ�����
    HI_U16 pad1;
}DIAG_CMD_TFP_IDENTIFY_RESULT_ENTRY_STRU;

// 6��STAվ������ʶ����ö��ֵ
typedef enum
{
    HI_MAC_TFP_IDENTIFY_RESULT_UNKOWN  = 0,
    HI_MAC_TFP_IDENTIFY_RESULT_THIS_POWER_DISTRI,             // Ϊ��̨��վ��
    HI_MAC_TFP_IDENTIFY_RESULT_NON_THIS_POWER_DISTRI,         // ��Ϊ��̨��վ��
    HI_MAC_TFP_IDENTIFY_RESULT_NOT_SUPPORT,                   // ��֧������ʶ��
    HI_MAC_TFP_IDENTIFY_RESULT_PHASE_ERR,                     // վ����λʶ�����
    HI_MAC_TFP_IDENTIFY_RESULT_FAIL,                          // ʶ��ʧ��
}HI_MAC_TFP_IDENTIFY_RESULT_E;

// 7��CCO���ϱ���������ʶ�����ṹ��
typedef struct
{
    HI_U16 sn;                                        // ���β�ѯ�ı���sn�ţ���֤��Ϊ�����ϱ������
    HI_U16 pkt_idx;                                   // �����ϱ��У�ÿ��������

    HI_U16 num_total;                                 // �����ϱ����ܵİ���
    HI_U16 entry_num;                                 // �����ϱ��У��ð��к��е�վ�����

    HI_U32 identify_time;                             // ����ʶ��ʱ�䣬��λs
    HI_U32 identify_time_one_turn;                    // һ������ʶ������ʱ�䣬��λs

    HI_BOOL is_last;                                  // �����Ƿ�Ϊ�������һ���ϱ�
    HI_U8 identify_status;                            // ����ʶ��״̬ʶ���У�ʶ�����
    HI_U8 pad[2];

    DIAG_CMD_TFP_IDENTIFY_RESULT_ENTRY_STRU entry[0]; // �ϱ���վ����Ϣ
}DIAG_CMD_TFP_IDENTIFY_RESULT_RSP_STRU;

// 8������ʶ��״̬
typedef enum
{
    HI_MAC_TFP_IDENTIFY_STATE_DOING,                  // ����ʶ����
    HI_MAC_TFP_IDENTIFY_STATE_OVER,                   // ����ʶ�����
}HI_MAC_TFP_IDENTIFY_STATE_E;

typedef struct
{
    HI_U32 ret;
} DIAG_CMD_TFP_CLEAR_RESULT_IND_STRU;

#define MULTI_FREQ_CNT_MAX  (4)
typedef struct
{
    HI_U8 freq[MULTI_FREQ_CNT_MAX];

    HI_U32 rcv_link_cfm_req_cnt;
    HI_U32 send_link_cfm_res_succ_cnt;
    HI_U32 send_link_cfm_res_fail_cnt;

    HI_U32 rcv_multi_freq_notif_cnt;
    HI_U32 send_multi_freq_notif_succ_cnt;
    HI_U32 send_multi_freq_notif_fail_cnt;

    HI_U32 send_multi_freq_notif_cnf_succ_cnt;
    HI_U32 send_multi_freq_notif_cnf_fail_cnt;
}DIAG_CMD_MF_UPPER_BRIDGING_STRU;

typedef struct
{
    HI_BOOL is_switching;
    HI_U8   detect_status;
    HI_U8   reinit_cnt;
    HI_U8   freq_with_proxy;

    HI_U32  detect_succ_cnt;
    HI_U32  detect_fail_cnt;
    HI_U32  switch_freq_succ_cnt;
    HI_U32  switch_freq_fail_cnt;
}DIAG_CMD_MF_DOWN_BRIDGING_STRU;

typedef struct
{
    HI_BOOL is_detecting;
    HI_U8   detect_status;
    HI_U8   reinit_cnt;
    HI_U8   pad;

    HI_U32 target_freq  :8;
    HI_U32 target_nid   :24;

    HI_U32 detect_sta_succ_cnt;
    HI_U32 detect_sta_fail_cnt;

    HI_U32 start_sta_detect_cnt;

    HI_U32 send_switch_freq_succ_cnt;
    HI_U32 send_switch_freq_fail_cnt;

    HI_U32 send_mf_beacon_succ_cnt;
    HI_U32 send_mf_beacon_fail_cnt;
}DIAG_CMD_MF_NETWORKING_STRU;

typedef struct
{
    HI_BOOL is_detecting;
    HI_U8   detect_status;
    HI_U8   pad[2];

    HI_U32 detect_succ_cnt;
    HI_U32 detect_fail_cnt;
    HI_U32 detect_finish_notify_cnt;

    HI_U32 detect_response_succ_cnt;
    HI_U32 detect_response_fail_cnt;

    HI_U32 rcv_detect_cnt;
    HI_U32 rcv_detect_response_cnt;

}DIAG_CMD_MF_ACTIVE_DETECT_NETWORK_STRU;

typedef struct
{
    HI_U32    dt                        : 3;        //���������;
    HI_U32    access                    : 5;        //��������
    HI_U32    snid                      : 24;       //�������ʶ��;

    HI_U32    result                    : 4;        //��ʾPB����ս����0x0:���ճɹ�;0x1:CRCУ��ʧ��;����:����
    HI_U32    state                     : 4;        //bit0��ʾPB0��CRC�����bit1��ʾPB1��CRC�����...
    HI_U32    stei                      : 12;       //Դ�豸��ʶ��
    HI_U32    dtei                      : 12;       //Ŀ���豸��ʶ��

    HI_U32    pb_num                    : 4;        //���յ���PB������
    HI_U32    rsvd1                     : 4;        //����
    HI_U32    snr                       : 8;        //�ŵ�����
    HI_U32    load                      : 8;        //վ�㸺��
    HI_U32    rsvd2                     : 8;

    HI_U32    rsvd3                     : 4;       //����
    HI_U32    ver                       : 4;        //��׼�汾��
    HI_U32    fccs                      : 24;       //֡У������;
}DIAG_CMD_MF_DETECT_FRAME_STRU;

typedef struct
{
    HI_U16 tei;
    HI_U8  upperlink_freq;
    HI_U8  downlink_freq;
}DIAG_CMD_MF_SWITCH_FREQ_FRAME_STRU;

#define DIAG_CMD_MF_UPPER_LINK_FREQ_CNT_MAX     (4)

typedef struct
{
    HI_U16 down_link_tei;
    HI_U16 tei;

    HI_U8  freq_cnt;
    HI_U8  pad[3];

    HI_U8  freq[DIAG_CMD_MF_UPPER_LINK_FREQ_CNT_MAX];
}DIAG_CMD_MF_MULTI_FREQ_STRU;

typedef struct
{
    HI_U32 snid;
    HI_U8 freq;
    HI_U8 pad;
    HI_U16 tei;
}DIAG_CMD_MF_DETECT_PARAM;

typedef struct
{
    HI_U16 down_link_tei;
    HI_U8  nw_freq;
    HI_U8  pad;
}DIAG_CMD_MULTI_FREQ_CNF_STRU;

typedef struct
{
    HI_U32 tei;
}DIAG_CMD_QUERY_MULTI_FREQUENCY_PARAM_STRU;

typedef struct
{
    HI_U8 freq;
    HI_U8 pad[3];
}DIAG_CMD_QUERY_MULTI_FREQUENCY_STRU;

//ʵʱ·���޸�ģ��ͳ����
typedef struct
{
    HI_U32 create_route_request_cnt;                //����·�������ĵĴ���
    HI_U32 create_route_reply_cnt;                  //����·�ɻظ����ĵĴ���
    HI_U32 create_route_ack_cnt;                    //����·��Ӧ���ĵĴ���
    HI_U32 create_route_error_cnt;                  //����·�ɴ����ĵĴ���

    HI_U32 create_link_confirm_request_cnt;         //������·ȷ������Ĵ���
    HI_U32 create_link_confirm_response_cnt;        //������·Ӧ��Ĵ���
    HI_U32 rcv_link_confirm_request_cnt;
    HI_U32 rcv_link_confirm_response_cnt;

    HI_U32 rcv_route_request_cnt;
    HI_U32 rcv_route_reply_cnt;
    HI_U32 rcv_route_ack_cnt;
    HI_U32 rcv_route_error_cnt;

    HI_U32 repair_src_cnt;                          //��Ϊԭʼվ�㴴���޸��Ĵ���
    HI_U32 repair_dst_cnt;                          //��ΪĿ��վ�㱻�޸��Ĵ���
    HI_U32 repair_start_fail_cnt;                   //·���޸�����ʧ�ܵĴ���
}DIAG_CMD_QUERY_RT_REPAIR_STAT_STRU;

// ������Ϣ 68B
typedef struct
{
    HI_U8  lock_status;                     // ����״̬
    HI_U8  lock_status_identify;            // ̨��ʶ����������״̬
    HI_U8  cco_mac[HI_PLC_MAC_ADDR_LEN];    // ���������CCO MAC��ַ

    HI_U32 lock_freq : 8;                   // ���������Ƶ��
    HI_U32 lock_snid : 24;                  // ���������SNID

    HI_U8  is_leave_from_lock_net;          // ��һ���Ƿ��Ǵ��������������
    HI_U8  pad;
    HI_U16 write_nv_cnt;                    // дNV�Ĵ���

    HI_U32 lock_begin_time;                 // ������ʼʱ�䣬��λs
    HI_U32 lock_duration;                   // ����ʱ������λs
    HI_U32 reject_time;                     // ��������ܾ�ʱ��
    HI_U32 leave_time_len;                  // ����ʱ��(��lock_begin_time����)����λs
    HI_U32 last_leave_time;                 // ��һ������������������ʱ�������λs

    HI_U32 lock_mac_cnt;                    // ����MAC����
    HI_U32 lock_snid_cnt;                   // ����SNID����
    HI_U32 snid_to_mac_cnt;                 // ������SNIDתΪ����MAC�Ĵ���
    HI_U32 reject_cnt;                      // ��������ܾ�����(ά��)
    HI_U32 lock_timeout_cnt;                // ������ʱ����(ά��)
    HI_U16 lock_fail_by_not_join_cnt;       // δ������������ʧ��(ά��)
    HI_U16 send_msg_fail_cnt;               // ��Ϣ����ʧ��(ά��)

    HI_U8  belong_status;                           // ����״̬
    HI_U8  belong_clear_reason;                     // �������ԭ��(���һ��)
    HI_U8  belong_cco_mac[HI_PLC_MAC_ADDR_LEN];     // ����CCO��MAC��ַ
    HI_U32 belong_freq : 8;                         // ���������Ƶ��
    HI_U32 belong_snid : 24;                        // ���������SNID
} DIAG_CMD_QUERY_LOCK_INFO_STRU;

#define NM_LOCK_RECORD_NUM     (8)

typedef struct
{
    HI_U8  lock_reason;                     // ����ԭ��
    HI_U8  lock_status;                     // ����״̬
    HI_U8  cco_addr[HI_PLC_MAC_ADDR_LEN];   // ���������CCO��MAC�����ڵ��ַ
    HI_U32 lock_freq : 8;                   // ���������Ƶ��
    HI_U32 lock_snid : 24;                  // ���������SNID

    HI_U32 lock_begin_time;                 // ������ʼʱ��
    HI_U32 lock_duration;                   // ����ʱ��
    HI_U32 lock_duration_safe;

    HI_U8  unlock_reason;                   // ����ԭ��
    HI_U8  pad[3];
    HI_U32 unlock_time;                     // ����ʱ��
} diag_lock_record_s;

typedef struct
{
    HI_U8 index;
    HI_U8 pad[3];

    HI_U32 period_time;
    HI_U32 rst_delay_time;

    diag_lock_record_s lock_records[NM_LOCK_RECORD_NUM];
} DIAG_CMD_GET_LOCK_REOCRDS_IND_STRU;

#if defined(PRODUCT_CFG_TTCN)
typedef struct
{
    HI_U32 ulRxCnt;            //
    HI_U32 uLRxTS;             //

    HI_U32 ulRxOkCnt;         //����ץ������ͳ��
    HI_U32 ulRxOkTS;          //

    HI_U32 ulParseErrCnt;
    HI_U32 ulParseErrTS;      //

    HI_U32 ulPbNothingCnt;
    HI_U32 ulPbNothingTS;

    HI_U32 ulPbtypeErrCnt;
    HI_U32 ulPbtypeErrTS;      //

    HI_U32 ulTypeErrCnt;
    HI_U8 ucType;
    HI_U8 pad[3];
    HI_U32 ulTypeErrTS;      //

    HI_U32 ulPhy_com_cnt;   // enter the sender to com
    HI_U32 ulLastTs;        //the latest of time span

    HI_U16  uspb_size;     // the length of PB  block
    HI_U8   ucsurplus_pbcnt; // ʣ���pb�����
    HI_U8   ucPbCnt;

}DIAG_CMD_MAC_PHYSICAL_TEST_STRU;
#endif


#if defined(PRODUCT_CFG_PRODUCT_TYPE_NDM) || defined(PRODUCT_CFG_HSO)
#define NDM_FC_MAX_SIZE                                (16)
#define NDM_PB_MAX_SIZE                                (520)
typedef struct
{
    HI_U16 sn;                 //�����
    HI_U16 pktIdx;            //����index
    HI_U16 totalPktCnt;          //�ܰ���
    HI_U16 pbLen;
    HI_U16 totalpbLen;
    HI_U16 pad;
    HI_U32 timeStamp;
    HI_MDM_CAPTURE_PKT_PBB_TYPE_EN ePayloadType;//payload ����
    HI_U8  aucFc[NDM_FC_MAX_SIZE];
    HI_U8  aucPb[NDM_PB_MAX_SIZE];
}DIAG_CMD_MAC_NDM_CAPTURE_STRU;

#define NDM_MODE_CAPTURE_DATA    1
#define NDM_MODE_NORMAL    0
typedef struct
{
	HI_U32  capture_switch;       // ������λ��ʾ�ϱ�mpdu�������࿪��
	HI_U32  mode_switch;             // �л�ģʽ 1��ץ��ģʽ��0����ģʽ
	HI_U32  pad;
}DIAG_CMD_MAC_NDM_CAPTURE_SWITCH_STRU;

typedef struct
{
	HI_U32  ret;
}DIAG_CMD_MAC_NDM_CAPTURE_SWITCH_IND_STRU;

typedef struct
{
    HI_U8 freq;
    HI_U8 pad1;
    HI_U16 ptcl_ver;
    HI_U32 snid;
}DIAG_CMD_MAC_NDM_CAPTURE_SET_ACCESS_INFO_STRU;

typedef struct
{
    HI_U32 ret;
}DIAG_CMD_MAC_NDM_CAPTURE_SET_ACCESS_INFO_IND_STRU;
#endif
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)

//CCO��ͣ���¼���ѯ��Ϣ
typedef struct
{
    HI_U8 power_failure_dfx_bitmap[POWER_FAILURE_BITMAP_LEN];                   // ͣ��վ��bitmap

    HI_U16 pad;
    HI_U16 rcv_power_failure_tei_cnt;                                         // �յ�ͣ���ϱ�sta����Ŀ

    HI_U32 rcv_sys_time;                                                        //�յ�ͣ���ϱ�֡��ʱ��

}DIAG_CMD_QUERY_POWER_FAILURE_INFO_STRU;
#endif

//���ʿ���ͳ����
typedef struct
{
    HI_BOOL    is_power_ctrl_enable;                            // ���ʿ���ʹ�ܿ���
    HI_BOOL    is_recover_last_digital_power;
    HI_BOOL    is_whole_net_set_digital_power;                  // �Ƿ��Ѿ�ȫ�������ַ��书��
    HI_BOOL    is_set_v100_detect_digital_power;

    HI_S8      v100_detect_digital_power;
    HI_BOOL    is_power_ctrl_starting;
    HI_U8      pad[2];

    HI_S8      digital_power[4];                                // ȫ��֪ͨ�����ַ��书��
    HI_S8      last_digital_power[4];                           // ȫ��֪ͨ���ַ��书��֮ǰ�����ַ��书��

    HI_BOOL    is_whole_net_set_digital_power_timer_start;      // whole_net_set_digital_power_timer��ʱ���Ƿ�����(��ʱ��N)
    HI_U8      power_ctrl_stop_reason;
    HI_U16     sta_num;                                         // ���ʿ��ƻ����sta����

    HI_U16     total_send_cnt;                                  // ����ȫ�����÷��书�ʵ�V100̽���ű���ܸ�������
    HI_BOOL    is_power_ctrl_form_timeout_timer_start;          // (��ʱ��X)
    HI_BOOL    is_stop_record_power_ctrl_sta;

    HI_U32     whole_net_set_digital_power_timer_start_timestamp;   //��λs
    HI_U32     power_ctrl_form_timeout_timer_start_timestamp;        //��λs

    HI_U32     sta_set_power_cnt;
    HI_U32     set_v100_power_cnt;
    HI_U32     cur_power_ctrl_sta_cnt;
    HI_U32     power_ctrl_stop_cnt;
    HI_U32     start_timer_err_cnt;
    HI_U32     send_msg_err_cnt;
    HI_U32     send_power_ctrl_bcn_err_cnt;
}DIAG_CMD_QUERY_POWER_CTRL_INFO_STRU;
#define DIAG_FAST_NOTICE_TX_MAX_LOOP        (5)
#define DIAG_FAST_NOTICE_ITEM_COUNT         (2)

typedef struct
{
    HI_U32 ntb_tx_enqueue;          // ���������ntbʱ��
    HI_U32 ntb_tx_real;             // ʵ�ʷ���ntbʱ��
    HI_U32 ntb_rx;                  // ����ntbʱ��
} DIAG_FAST_NOTICE_RX_TX_NTB_ST;

typedef struct
{
    DIAG_FAST_NOTICE_RX_TX_NTB_ST  inf[DIAG_FAST_NOTICE_TX_MAX_LOOP];
} DIAG_FAST_NOTICE_RX_TX_NODE;

typedef struct
{
    HI_U32 time_ntb;                // ntbʱ��
    HI_U16 total;                   // վ������
    HI_U16 current;                 // ��ǰ����
    HI_U16 start_tei;               // ��ʼteis
    HI_U16 pad;
    DIAG_FAST_NOTICE_RX_TX_NODE node[DIAG_FAST_NOTICE_ITEM_COUNT];
} DIAG_FAST_NOTICE_RX_TX_ST;

typedef struct
{
    HI_U16 high_level_chg_cnt;
    HI_U16 same_level_proxy_chg_cnt;
    HI_U16 same_level_sta_chg_cnt;
    HI_U16 low_level_proxy_chg_cnt;
    HI_U16 low_level_sta_chg_cnt;
    HI_U16 low_comm_rate_chg_cnt;
}DIAG_QUERY_PROXY_CHG_INFO_STRU;

typedef struct
{
    DIAG_QUERY_PROXY_CHG_INFO_STRU proxy_chg_info;
    HI_U16 back_up_tei;
	HI_U16 old_proxy_tei;
    HI_U32 chg_time;
    HI_U8  pad[24];
}DIAG_CMD_QUERY_ROUTE_PROXY_INFO_STRU;

// ����PLCЭ��汾�����нṹ
typedef struct
{
    HI_U8 protocol_type;
    HI_U8 pad[3];
}DIAG_CMD_SET_PLC_PROTO_STRU;

typedef struct
{
    HI_U32 ret;
}DIAG_CMD_SET_IND_PLC_PROTO_STRU;

//��ѯPLCЭ��汾�����нṹ
typedef struct
{
    HI_U8 protocol_type;
    HI_U8 pad[3];
}DIAG_CMD_QUERY_IND_PLC_PROTO_STRU;

#define NODE_FIND_MAC_LIST_NUM      (8)
#define NODE_FIND_MAC_LIST_SIZE     (48)

//�ڵ����REQ�ṹ
typedef struct
{
    HI_U8 mode;             // ģʽ
    HI_U8 duration;         // ����ʱ������λmin
    HI_U8 mac_num;          // �б�ģʽ�£���ѯվ�����
    HI_U8 lock_duration;    // ����ʱ������λmin
    HI_U8 mac_list[NODE_FIND_MAC_LIST_SIZE];
} DIAG_CMD_NODE_FIND_REQ_STRU;

//�ڵ����IND�ṹ
typedef struct
{
    HI_U32 ret;
} DIAG_CMD_NODE_FIND_IND_STRU;

// ��ԴӦ��ά����Ϣ
typedef struct
{
    HI_BOOL is_energy_mode;
    HI_BOOL is_opening;
    HI_BOOL is_cco_no_send_bcn;
    HI_BOOL is_chip_detecting;

    HI_BOOL is_full;
    HI_BOOL is_white_list_empty;
    HI_BOOL is_unlock_rejoin_enable;           // �Ƿ��������ط��������߹���
    HI_U8 sta_rejoin_by_unlock_cnt;          // STAվ���������ص���û�б���������λ����

    HI_U16 open_cnt;
    HI_U16 stop_cnt;

    HI_U16 detect_chip_ver;
    HI_U16 clear_detect_chip_ver_cnt;

    HI_U32 network_form_time;
    HI_U32 network_form_max_time;

    HI_U16 hi3911V100_chip_ver_cnt;
    HI_U16 hi3911V200_chip_ver_cnt;

    HI_U16 hi3921V100_chip_ver_cnt;
    HI_U16 unkown_chip_ver_cnt;

    HI_U32 rcv_cco_packet_cnt;

    HI_U32 reject_by_no_register_cnt;        // CCO�˾ܾ�վ��������������Դ�汾��CCO��Ȩû����Ч
}DIAG_CMD_QUERY_ENERGY_STATICS_STRU;

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
// ����NTB-�ֲ�ʽ: ��ѯ��ǰʶ����Ϣ �������
typedef struct
{
    HI_U16  index;
    HI_BOOL is_use_idx; // �Ƿ�ʹ���±�����ѯ��������ʹ�������freq nid����ѯ
    HI_U8   freq;
    HI_U32  nid;
} DIAG_CMD_TF_GET_NET_IDENTIFY_INFO_REQ_STRU;

typedef struct
{
    HI_U32 cal_diff_fail    : 1;
    HI_U32 is_err_ntb       : 1;
    HI_U32 pad1             : 2;
    HI_U32 valid_ntb_cnt    : 8;    // ��ЧNTB����
    HI_U32 cco_disturb      : 10;   // CCO���Ŷ�����λus
    HI_U32 sta_disturb      : 10;   // STA���Ŷ�����λus

    HI_U16 zc_diff;                 // �����ƫ��
    HI_U16 pad2;

    HI_U32 variance;                // ����
} diag_tf_nd_cache_info_st;

#define DIAG_TF_ND_SAVE_CACHE_NUM   5
#define DIAG_TF_ND_SEQ_NUM          5

// ��ѯ��ǰ����ĳ����ʶ����Ϣ 84
typedef struct
{
    HI_U8  is_valid                 : 1;        // ��¼�Ƿ���Ч
    HI_U8  match_phase              : 3;        // ��վ���ڸ����������
    HI_U8  match_edge               : 2;        // ƥ��Ĳɼ���ʽ: �ο�POWER_EDGE_XXX
    HI_U8  pad1                     : 2;
    HI_U8  net_index;                           // ��������������������Ϣ�Ӷ�������Ϣ�л�ȡ
    HI_U8  collect_seqs[DIAG_TF_ND_SEQ_NUM];    // �ɼ����
    HI_U8  phase_masks[DIAG_TF_ND_SEQ_NUM];     // ͬһ�ɼ����к��£���λ���ݽ�������

    HI_U16 rcv_pkt_cnt;                         // ����NTB���ݰ���
    HI_U16 seq_eq_pkt_cnt;                      // ���������ͬ�İ���

    HI_U8  match_succ_cnt;                      // ƥ�䱾��NTB�ɹ�����
    HI_U8  big_period_diff_cnt;
    HI_U8  match_phase_fail_cnt;
    HI_U8  zc_diff_cnt;                         // ͳ�Ƶ�������ֵ���ݰ���

    HI_U8  cal_diff_fail_cnt;
    HI_U8  err_ntb_pkt_cnt;
    HI_U8  cal_var_fail_cnt;
    HI_U8  valid_pkt_cnt;                       // �����㹻��ЧNTB�İ��� (�ﵽ���㷽���)

    HI_U8  variance_cnt;
    HI_U8  cache_idx;
    HI_U16 pad;

    HI_U32 last_rcv_time;

    diag_tf_nd_cache_info_st caches[DIAG_TF_ND_SAVE_CACHE_NUM];

    HI_U16 period_diff_avg;                     // ��Ƶ����ƫ���ֵ����λNTB
    HI_U16 zc_diff_avg;                         // �����ƫ���ֵ

    HI_U32 variance_avg;                        // �����ֵ
} DIAG_CMD_TF_GET_NET_IDENTIFY_INFO_IND_STRU;

typedef struct
{
    HI_U32 freq : 8;
    HI_U32 nid  : 24;
} DIAG_CMD_TF_ND_GET_NET_RESULT_REQ_STRU;

typedef struct
{
    HI_U8  period_diff_valid    : 1;
    HI_U8  zc_diff_valid        : 1;
    HI_U8  pad1                 : 6;
    HI_U8  pad2;
    HI_U8  valid_pkt_cnt;
    HI_U8  variance_cnt;

    HI_U16 period_diff;
    HI_U16 zc_diff;

    HI_U32 variance;
} diag_tf_nd_his_info_st;

#define DIAG_TF_ND_HIS_NUM      25

typedef struct
{
    HI_U16 is_valid             : 1;
    HI_U16 is_big_period_diff   : 1;
    HI_U16 is_big_zc_diff       : 1;
    HI_U16 net_idx              : 6;
    HI_U16 pad1                 : 7;
    HI_U16 match_succ_cnt;

    HI_U16 big_period_diff_cnt;
    HI_U16 valid_pkt_cnt;

    HI_U16 period_diff_avg;
    HI_U16 zc_diff_avg;

    HI_U16 min_ratio;
    HI_U16 min_net_pkt_cnt;

    diag_tf_nd_his_info_st history[DIAG_TF_ND_HIS_NUM];
    HI_U32 round_begin_time[DIAG_TF_ND_HIS_NUM];
} DIAG_CMD_TF_ND_GET_NET_RESULT_IND_STRU;

typedef struct
{
    HI_U32 begin_index;
} DIAG_CMD_TF_ND_CUR_RESULT_REQ_STRU;

// 16
typedef struct
{
    HI_U16 is_valid             : 1;
    HI_U16 is_big_period_diff   : 1;
    HI_U16 is_big_zc_diff       : 1;
    HI_U16 net_idx              : 6;
    HI_U16 period_diff_valid    : 1;
    HI_U16 zc_diff_valid        : 1;
    HI_U16 pad1                 : 5;
    HI_U16 pad2;

    HI_U8  match_succ_cnt;
    HI_U8  big_period_diff_cnt;
    HI_U8  valid_pkt_cnt;
    HI_U8  variance_cnt;

    HI_U16 period_diff;
    HI_U16 zc_diff;

    HI_U32 variance;
} diag_tf_nd_net_result_st;

#define DIAG_TF_ND_RESULT_QUERY_NUM     16

typedef struct
{
    HI_U16 is_init_success      : 1;    // �Ƿ��ʼ���ɹ�
    HI_U16 is_caching           : 1;    // �Ƿ����ڻ�������
    HI_U16 is_cache_full        : 1;    // NTB����: �Ƿ�BUF������
    HI_U16 is_identifying       : 1;    // ����ʶ��: �Ƿ�����̨��ʶ����
    HI_U16 is_start_cache_fail  : 1;    // �Ƿ������ɼ�ʧ�ܣ���������һ��result_judge_timer��ʱ������������
    HI_U16 result_net_index     : 6;    // ����ʶ��������IDX
    HI_U16 result_type          : 5;    // �������ս��������
    HI_U16 next_cache_index;                       // NTB����: ��һ��Ҫ�洢��λ��

    HI_U32 last_rcv_time;               // ������������յ����ݳ���30min�����¿�ʼʶ��
    HI_U32 cur_round_begin_time;

    diag_tf_nd_net_result_st results[DIAG_TF_ND_RESULT_QUERY_NUM];
} DIAG_CMD_TF_ND_CUR_RESULT_IND_STRU;

#define TF_ND_IDENTIFY_STAT_NUM     8
#define TF_ND_JUDGE_RECOTD_NUM      25

enum
{
    TF_ND_BREAK_POINT_NET_LACK_VAR = 1, // ������δ�������
    TF_ND_BREAK_POINT_NO_RCV_JOIN,      // û���յ�����̨��NTB
    TF_ND_BREAK_POINT_VARIANCE_CMP,     // ����Ƚ�
    TF_ND_BREAK_POINT_ERR_SCENE_FAIL,   // �쳣������������������
    TF_ND_BREAK_POINT_ERR_SCENE_SUCC,   // �쳣�����������ﵽ����
};

typedef struct
{
    HI_U32 break_point  : 3;    // ���̽�����λ��
    HI_U32 is_override  : 1;    // �Ƿ�ﵽ�Ʒ�����
    HI_U32 net_idx      : 5;    // ����ֵ��Ӧ������
    HI_U32 max_ratio    : 16;
    HI_U32 pkt_cnt      : 7;
} diag_tfnd_result_judge_record_s;

typedef struct
{
    HI_U16  rcv_app_pkt_num;
    HI_U16  try_handle_app_pkt_num;
    HI_U16  seq_eq_app_pkt_num;
    HI_U16  handle_app_pkt_num;

    HI_U16  rcv_bj_pkt_num;
    HI_U16  try_handle_bj_pkt_num;
    HI_U16  seq_eq_bj_pkt_num;
    HI_U16  handle_bj_pkt_num;

    HI_U16  rcv_mgt_pkt_num;
    HI_U16  try_handle_mgt_pkt_num;
    HI_U16  seq_eq_mgt_pkt_num;
    HI_U16  handle_mgt_pkt_num;

    HI_U16  filter_err_ntb_cnt;
    HI_U16  filter_ntb_in_end_cnt;

    HI_U16  filter_ntb_stage_cnt;
    HI_U16  filter_ntb_jitter_fail;

    HI_U16  local_big_noise_cnt;
    HI_U16  restart_cache_cnt;

    HI_U8   identify_stat_num;
    HI_U8   cache_stat_num;
    HI_U8   result_num;
    HI_U8   judge_idx;

    HI_U8   identify_stat_records[TF_ND_IDENTIFY_STAT_NUM];
    HI_U8   cache_stat_records[TF_ND_IDENTIFY_STAT_NUM];
    HI_U8   result_net_idx[TF_ND_IDENTIFY_STAT_NUM];
    HI_U8   result_type[TF_ND_IDENTIFY_STAT_NUM];
    HI_U32  result_change_time[TF_ND_IDENTIFY_STAT_NUM];
    diag_tfnd_result_judge_record_s judge_records[TF_ND_JUDGE_RECOTD_NUM];
} diag_tf_nd_sta_stat_st;

// NTBʶ��״̬
typedef struct
{
    diag_net_info_st belong_net_info;
    diag_tf_nd_sta_stat_st stat;
} DIAG_CMD_TF_ND_STA_GET_STAT_IND_STRU;

// ���NTBʶ��������
typedef struct
{
    HI_BOOL is_clear_history;
    HI_BOOL is_restart_identify_round;
    HI_U16  pad;
} DIAG_CMD_TF_ND_STA_CLEAR_RESULT_REQ_STRU;

typedef struct
{
    HI_U32 ret;
} DIAG_CMD_TF_ND_STA_CLEAR_RESULT_IND_STRU;

typedef struct
{
    HI_U32 freq         : 7;
    HI_U32 nid          : 24;
    HI_U32 is_filter    : 1;    // �Ƿ��˲�
    HI_U16 tei;
    HI_U16 collect_duration;    // �ɼ�ʱ��
    HI_U16 hold_duration_min;   // �ڴ汣��ʱ�䣬������ʱ��Ͱ��ڴ��ͷ��ˣ���λmin
    HI_U16 pad;
} DIAG_CMD_SNR_SET_VALUE_CNT_PARAM_REQ_ST;

typedef struct
{
    HI_U32 ret;
} DIAG_CMD_SNR_SET_VALUE_CNT_PARAM_IND_ST;

typedef struct
{
    HI_U8  is_show_att_detail;
    HI_U8  ratio;
    HI_U16 pad;
} DIAG_CMD_SNR_QUERY_VALUE_CNT_INFO_REQ_ST;

#define DIAG_QUERY_VALUE_DETAIL_NUM     (120)

typedef struct
{
    HI_U32 ret;

    HI_U32 snr_total_cnt;
    HI_S16 snr_avg;
    HI_S16 ratio_snr_avg;

    HI_U32 att_total_cnt;
    HI_U16 att_avg;
    HI_U16 ratio_att_avg;

    HI_U16 detail_cnt[DIAG_QUERY_VALUE_DETAIL_NUM];
} DIAG_CMD_SNR_QUERY_VALUE_CNT_INFO_IND_ST;

#endif

#ifdef __cplusplus
}
#endif

#endif /*DFX_H*/

