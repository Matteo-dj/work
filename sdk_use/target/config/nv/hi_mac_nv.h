/*************************************************
  Copyright:   Huawei Technologies Co., Ltd. 1998-2011
  File name: mac_nv.h
  Author:  dkf35509
  Description:  NV���ID�ͽṹ�嶨��
  Others:
  History:
*************************************************/

#ifndef MAC_NV_H
#define MAC_NV_H

#include "hi_types.h"
#include "hi_phy_nv.h"

#ifdef __cplusplus
extern "C" {
#endif

// �����������б�������/���ñ�־
#define  MAC_NV_CONFIG_ENABLE 1
#define  MAC_NV_CONFIG_DISABLE 0

/************************************************************************/
/*    NV �� ID                                                          */
/************************************************************************/
typedef HI_U16 ID_DIAG_CONFIG;
#define ID_DIAG_CMD_CONFIG_IF                           (0x0202)    // ֡�������
#define ID_DIAG_CMD_CONFIG_CLASSES                      (0x0203)    // ��۷������
#define ID_DIAG_CMD_CONFIG_TIMEOUT_ON_PRI               (0x0204)    // ���Ĺ���ʱ�����
#define ID_DIAG_CMD_CONFIG_BUFFER_PERCENT_ON_PRI        (0x0205)    // ���Ͷ��д�С����
#define ID_DIAG_CMD_CONFIG_LED_NV                       (0x0209)    // �ƿ�ģ�����
#define ID_DIAG_CMD_NDM_PIPE_NV                         (0x020C)    // ������ͨ������
#define ID_DIAG_CMD_CONFIG_ASSOC_CONFIG                 (0x020D)    // ��������
//#define ID_DIAG_CMD_CONFIG_SOUND_STARTUP_THRESHOLD      (0x0213)    // SOUNDING��������
#define ID_DIAG_CMD_CONFIG_PHASE_ATTR                   (0x0214)    // ��λģ�����
#define ID_DIAG_CMD_CONFIG_TDMA_CONFIG                  (0x0217)    // TDMA���Ȳ���
#define ID_DIAG_CMD_CONFIG_ROUTE_ATTRIB                 (0x0220)    // ·����������
#define ID_DIAG_CMD_CONFIG_TX_CSMA_ALGORITHM            (0x0221)    // CSMA�˱ܲ���
#define ID_DIAG_CMD_CONFIG_TX_SOF_TMI_POLICY            (0x0222)    // ����TMIѡ����Բ���
#define ID_DIAG_CMD_CONFIG_BEACON_TMI                   (0x0223)    // �ű�֡TMIѡ�����
#define ID_DIAG_CMD_CONFIG_DFX_CONFIG                   (0x0224)    // ��ά�ɲ⿪�ز���
#define ID_DIAG_CMD_CONFIG_ROUTE                        (0x0225)    // IP·�����ò���
#define ID_DIAG_CMD_MULTI_NETWORK_ASSOC_CONFIG                       (0x0226)    // �������������
#define ID_DIAG_CMD_BEACON_SLOT_CONFIG                  (0x0227)    // �ű�ʱ϶����
#define ID_DIAG_CMD_FILTER_SWITCH_CONFIG                (0X0229)    // ���Ĺ��˿��ز���
#define ID_DIAG_CMD_CONFIG_STA_BLIND_CHECK              (0x022A)    // Ƶ��ä�����
#define ID_DIAG_CMD_CONFIG_NDM_CONNECT_PARA             (0x022D)    // ���������Ӳ���
#define ID_DIAG_CMD_CONFIG_MONITOR_CONFIG               (0x022E)    // ���ļ�ز���
#define ID_DIAG_CMD_SPEED_ABNORMITY_NV                  (0x022F)    // �����쳣������
#define ID_DIAG_CMD_CONFIG_RT_CYCLE_SELF_ADAPT_PARA     (0x0230)    // ·�����ڲ���
#define ID_DIAG_CMD_STA_GPIO_NV                         (0x0231)    // STA��GPIO�ܽ����ò����Լ��ؼ��ڵ㶨λ����
#define ID_DIAG_CMD_CONFIG_MAC_NDM_CHL                  (0x0232)    // ������ͨ������
#define ID_DIAG_CMD_CONFIG_MULTI_EXCEPTION_NV           (0x0233)    // �������쳣������
#define ID_DIAG_CMD_CFG_FA_NV                           (0x0234)    // Ƶ������Ӧ����
#define ID_DIAG_CMD_CONFIG_SPEED_CONFIG                 (0x0235)    // MAC���ʲ���
#define ID_DIAG_CMD_CONFIG_REPORT_COMM_RATE             (0x0236)    // ͨ�����ϱ�����
#define ID_DIAG_CMD_CONFIG_SPEED_TEST                   (0x0237)    // ���ܲ������ò���
#define ID_DIAG_CMD_CONFIG_PLC_SWITCH_PARA_CONFIG       (0x0239)    // ����ģ�����(����Զ���ز�ͨѶ�ն˰汾ʹ��)
#define ID_DIAG_CMD_CFG_TF_NV                           (0x023A)    // ̨��ʶ�����
#define ID_DIAG_CMD_CONFIG_ZERO_CROSS_DIFFERENCE_NV     (0x023B)    // ����������
#define ID_DIAG_CMD_CFG_PI_NV                           (0x023C)    // ��λʶ�����
#define ID_DIAG_CMD_CONFIG_PLC_SWITCH_FILTER_CONFIG     (0x0240)    // ����ģ�鱨�Ĺ��˲���
#define ID_DIAG_CMD_CONFIG_FAULT_LOCAT_PARA_CONFIG      (0x0241)    // �ؼ��ڵ㶨λ����
//#define ID_DIAG_CMD_CONFIG_DUAL_MODULE_PARA_CONFIG      (0x0242)    // ˫ģ���Բ���
#define ID_DIAG_CMD_CONFIG_DUTY_PARA_CONFIG             (0x0243)    // ռ�ձȿ��Ʋ���(����ʹ��)
#define ID_DIAG_CMD_CONFIG_TF_CROSSTALK_STA_PARA        (0x0244)    // ̨��ʶ���Ų��Բ���(STA)
#define ID_DIAG_CMD_CONFIG_SNR_CONFIG                   (0x0245)    // ����������չNV��(��Ϊ0x20D�Ĳ���)
#define ID_DIAG_CMD_CONFIG_SNR_TF_CONFIG                (0x0246)    // SNR̨��ʶ�����

#define ID_DIAG_CMD_CONFIG_PTPC_PARA                    (0x024A)    // UIU�Ĳ���
#define ID_DIAG_CMD_CONFIG_EMC_PARA                     (0x024B)    // EMC��֤����
#define ID_DIAG_CMD_NTB_PARA                            (0x024D)    // V200-NTB����
//#define ID_DIAG_CMD_GT_TEST_TMI_INFO_PARAM              (0x024F)
//#define ID_DIAG_CMD_GT_TEST_NTB_PARAM                   (0x0250)
//#define ID_DIAG_CMD_SC_NETWORK_PARA_CONFIG              (0x0251)    // ���ز���������
#define ID_DIAG_CMD_TF_PASSIVE_NV                       (0x0252)    // ����ʶ����Ʋ���
//#define ID_DIAG_CMD_MULTI_FREQ_PARAM_CONFIG             (0x0253)    // ��Ƶ��������
#define ID_DIAG_CMD_POWER_FAILURE_STATISTIC_INFO        (0x0254)     //ͣ���¼������Ϣ�洢info
#define ID_DIAG_CMD_CONFIG_COORDINATE_CONFIG            (0x0305)    // ������Э������
#define ID_DIAG_CMD_CONFIG_POWEROFF_CHECK_CONFIG        (0x0306)    // ͣ�������
#define HI_NV_FTM_ROUTE_PROXY_CONFIG                    (0x0310)    // HI_NV_FTM_RESERVED0_S
/************************************************************************/
/*             NV������                                                   */
/************************************************************************/

#define FA_TIMETABLE_NUM_MAX            9                   //�Լ춨ʱʱ�̵�������

#define SNR_TRANS_CLASS_NUM                     (8)         // ����SNR�㼶��Ŀ
#define SNR_COLLECT_CLASS_NUM                   (16)         // ͳ��SNR�㼶��Ŀ

#define SNR_FAST_ACCURATE_TH_NUM                (4)         // ���������߾����޸���
#define SNR_FAST_ACCURATE_AVG_PECISION          (10)        // ���������߾����޵�SNRƽ��ֵ����

//*****************************************************************************
// Ԥ��NV���, ��� 64�ֽ�
//*****************************************************************************
typedef struct
{
    HI_U8      static_proxy_mac[HI_PLC_MAC_ADDR_LEN];   // ָ����̬�м�MAC��ַ
    HI_BOOL    my_static_proxy_exist;                   // �Ƿ�����ָ����̬�м�
    HI_BOOL    to_be_static_repeater;                   // ��վ���Ƿ�Ϊ��̬�м�

    HI_U8      freq_evaluate_mode;                      //Ƶ������ģʽ��0���Լ�ģʽ��1�����ģʽ  1   ������
    HI_U8      rsvd2;                                   //����
    HI_U16     max_node_num;                            //��ʷ�������վ����������������ˢ�£��Լ������׼�Ҹ�������ʱˢ��    0   ������
}HI_NV_FTM_ROUTE_PROXY_CONFIG_S;

//��ʱ��������
typedef struct
{
    HI_BOOL is_enable;          //��ʱ��Ч�ı�ʶ
    HI_U8  hour;                // hours
    HI_U8  min;                 // minutes
    HI_U8  sec;                 // seconds
}MAC_REALTIME_STRU;

/**
 * 1.2    ֡�������
 */
typedef struct _DIAG_CMD_CONFIG_IF_STRU_
{
   HI_U16 pifs;
   HI_U16 rifs;
   HI_U16 cifs;
   HI_U16 bifs;

   HI_U16 eifs[FREQ_TRY_NUM];

   HI_U8  pad1;
   HI_U8  pad;
   HI_U16 time_redundancy;

   HI_U16 ack_min_redundancy;                  //MAC��Ӧ��֡����С����������λ:us
   HI_U16 ack_max_redundancy;                  //MAC��Ӧ��֡���������������λ:us
} DIAG_CMD_CONFIG_IF_STRU;

/**
 *
 * 1.3    ������������ȼ���Ӧ��ϵ����
 *
 */
typedef enum _m_type_e_
{
    DFX_MANAGEMENT_MESSAGE_TYPE     =0x88E1,    // ���ع�����������
    DFX_IP4_MESSAGE_TYPE         = 0x88E3,  // IP��������
}m_type_e;
typedef  struct _DIAG_CMD_CONFIG_CLASSES_ENTRY_STRU_
{
    HI_U16 type;        // ��������.88e1:���� .0x88E3, IP��������
    HI_U8  flag ;       // (vlantag��TOS�� MMTYPE)
    HI_U8  priorty;     // �������ȼ�
}DIAG_CMD_CONFIG_CLASSES_ENTRY_STRU;

#define ID_DIAG_CMD_CONFIG_CLASSES_ENTRY_COUNT 10
typedef  struct _DIAG_CMD_CONFIG_CLASSES_STRU_
{
   HI_U16 num;
   HI_U16 pad;
   DIAG_CMD_CONFIG_CLASSES_ENTRY_STRU entry[ID_DIAG_CMD_CONFIG_CLASSES_ENTRY_COUNT];
} DIAG_CMD_CONFIG_CLASSES_STRU;

/**
 *
 * 1.4    ��ͬ���ȼ����ĵĳ�ʱʱ������
 *
 */
typedef struct
{
    HI_U32 timeout : 24;  // ��Ӧ�ĳ�ʱ
    HI_U32 priorty : 8;   // ���ȼ�
} DIAG_CMD_CONFIG_TIMEOUT_ON_PRI_ENTRY_STRU;
#define ID_DIAG_CMD_CONFIG_TIMEOUT_ON_PRI_ENTRY_COUNT 10
typedef struct _DIAG_CMD_CONFIG_TIMEOUT_ON_PRI_STRU_
{
    HI_U8 num;   // ����������n
    HI_U8 pad[3];
    DIAG_CMD_CONFIG_TIMEOUT_ON_PRI_ENTRY_STRU entry[ID_DIAG_CMD_CONFIG_TIMEOUT_ON_PRI_ENTRY_COUNT];    // ������ʵ��
} DIAG_CMD_CONFIG_TIMEOUT_ON_PRI_STRU;


/**
 *
 * 1.5    ��ͬ���ȼ���BUFFER ����
 *
 */
typedef struct
{
    HI_U8  priorty;             // �������ȼ�
    HI_U8  cco_queue_size;      // CCO���б��ĸ�������
    HI_U8  sta_queue_size;      // STA���б��ĸ�������
    HI_U8  pad;
} DIAG_CMD_CONFIG_BUFFER_PERCENT_ON_PRI_ENTRY_STRU;

#define ID_DIAG_CMD_CONFIG_BUFFER_PERCENT_ON_PRI_ENTRY_COUNT 10
typedef struct _DIAG_CMD_CONFIG_BUFFER_PERCENT_ON_PRI_STRU_
{
   HI_U8 num;
   HI_U8 pad[3];
   DIAG_CMD_CONFIG_BUFFER_PERCENT_ON_PRI_ENTRY_STRU entry[ID_DIAG_CMD_CONFIG_BUFFER_PERCENT_ON_PRI_ENTRY_COUNT];
} DIAG_CMD_CONFIG_BUFFER_PERCENT_ON_PRI_STRU;


/**
 *
 * 1.6    ����/���� buffer��С
 *
 */
typedef struct _DIAG_CMD_CONFIG_TXRX_BUFFER_SIZE_STRU_
{
   HI_U32 tx_buffer_size;
   HI_U32 rx_buffer_size;
} DIAG_CMD_CONFIG_TXRX_BUFFER_SIZE_STRU;

/**
 *
 * 1.7    �����ش�����
 *
 */
typedef struct _DIAG_CMD_CONFIG_RESEND_COUNT_STRU_
{
   HI_U32 resend_time;
} DIAG_CMD_CONFIG_RESEND_COUNT_STRU;

/**
 *
 * 1.8    ������MAC��ַ�б�����(CCO, ����������)
 *
 */
#ifdef SAL_HAVE_FEATURE_UT
#define ID_DIAG_CMD_MAC_WHITE_ITEM_MAX 150
#else
#define ID_DIAG_CMD_MAC_WHITE_ITEM_MAX 2048         //HSOʹ�ø�ֵ����HI_MAC_VALID_MAC_LIST_MAX��HI_BIDB_ITEM_NUM_MAX����
#endif

#define RT_NV_SLIDE_WINDOWS_CNT              6
typedef struct _DIAG_CMD_CONFIG_ROUTE_ATTRIB_STRU_
{
    HI_U16 rt_evaluate_cycle_time_len;             // ��ǰ�Ѳ��ã���������Ӧ�������ڵĲ���
    HI_U8  rt_rcv_proxy_down_min_th;               // ���н��մ���վ����С����
    HI_U8  rt_down_comm_rate_basic_limit;          // ����ͨ���ʻ�������
    HI_U8  rt_up_comm_rate_basic_limit;            // ����ͨ���ʻ�������
    HI_U8  rt_choose_proxy_upper_limit;            // ����ѡ��ͨ��������
    HI_U8  rt_choose_proxy_lower_limit;            // ����ѡ��ͨ��������

    HI_U8  rt_continue_better_th;                  // ����ѡ���������ڱ������
    HI_U8  rt_high_diff;                           // ���ޱ������
    HI_U8  rt_middle_diff;                         // ����������֮��������
    HI_U8  rt_low_diff;                            // ���ޱ������

    HI_U8  rt_switch_group_one;                    // bit0-bit7����:��������,������ѡ,snrУ׼,����㼶Զ��ʶ���㷨,�Ƿ��ű�֡��Ϊͨ����ͳ�Ƶ�����,�㼶Զ��ʶ���㷨,��ѡ����Ͳ㼶����վ����Ϊ��ѡ����,�ŵ�������Ϊ����������������
    HI_U8  rt_switch_group_two;                    // bit0-bit7����:����������ͨ��������,ͨ���ʵĵ͵�վ�㲻ѡ��Ϊ��ѡ����,����ͨ���ʸ�������ʱ��ѡ�����ͨ����վ����Ϊ����,�����б����Ƿ�Я����ά�ɲ���Ϣ,·���������ڼ����·������,������·��,���з�����·��,ʹ�ö�·��
    HI_U8  rt_snr_diff;                            // ��·��ѡ��ʱsnr���ޱ������
    HI_U8  rt_comm_rate_diff;                      // ��·��ѡ��ʱͨ�������ޱ������
    HI_U8  rt_high_level_chg_comm_rate_diff;
    HI_U8  rt_same_level_chg_comm_rate_diff;
    HI_U8  rt_low_level_chg_down_up_comm_rate_diff;
    HI_U8  rt_low_level_chg_up_comm_rate_limit;
    HI_U8  pad;

    HI_U16 rt_child_station_less_th;               // ��վ��ƫ������
    HI_U8  rt_proxy_send_discovery_pk_len;         // �����ͷ����б�����ʱ��         ��λ:S
    HI_U8  rt_sta_send_discovery_pk_len;           // ��ͨվ�㷢�ͷ����б�����ʱ��     ��λ:S
    HI_U8  slide_window_max;                       // ��󴰿���
    HI_U8  slide_window_weight[RT_NV_SLIDE_WINDOWS_CNT];       // ����Ȩ��
    HI_U8  min_first_evaluate_period;

    HI_U8  rt_min_comm_rate_th;                    // ����ǰ����ͨ������Ϊ�������õ�ͨ���ʵ�����
    HI_U8  enforcing_proxy_change_enable;          // ǿ�ƴ�����ʹ�ܱ�־
    HI_U16 enforcing_proxy_change_cycle;           // ǿ�ƴ��������ڣ���λ����

    HI_U16 preamble_sat_limit;
    HI_U8  current_attenuation_weight;             // ��˥���˲��㷨�У���ǰ֡˥��ֵ��Ȩ��
    HI_U8  current_mmib_weight;                    // ��MMIB�˲������У���ǰ֡mmib���ݵ�Ȩ��

    HI_U8  coefficient_high_limt;                  // ͨ����ϵ��������
    HI_U8  coefficient_middle_limt;                // ͨ����ϵ��������
    HI_U8  coefficient_low_limt;                   // ͨ����ϵ��������
    HI_U8  comm_rate_fliter_th;

    HI_U8  algorithm_type;                         // ·���㷨�����Ͷ���.����RT_ALGORITHM_V1R3
    HI_U8  sta_coefficient;                        // ����վ�����ͨ����ʱ�˵�ϵ��
    HI_BOOL is_repair_enable;                      // ·���޸�ʹ�ܱ�ʶ
    HI_U8 route_request_timer_period;              // ·���޸�����ʱ���ޣ���λs
}DIAG_CMD_CONFIG_ROUTE_ATTRIB_STRU;

#define RT_CYCLE_PARA_CNT           (6)
typedef struct _DIAG_CMD_CONFIG_RT_CYCLE_SEIF_ADAPTIVE_ATTRIB_STR_
{
    HI_U16 station_cnt;                     // վ�����
    HI_U16 evaluate_cycle_len;              // �������ڳ��� ��λ:��

    HI_U8  proxy_send_dk_cycle_len;         // ����վ�㷢�ͷ����б���Ƶ�� ��λ:��
    HI_U8  sta_send_dk_cycle_len;           // ����վ�㷢�ͷ����б���Ƶ�� ��λ:��
    HI_U8  pad[2];
}DIAG_CMD_CONFIG_RT_CYCLE_SEIF_ADAPTIVE_ATTRIB_STR;
typedef struct _DIAG_CMD_CONFIG_ROUTE_ADAPTIVE_ATTRIB_STRU_
{
    HI_U16 network_station_num;             // �����ģ����0:��ʾ��������ʵ��վ������̬ѡȡ����
    HI_U16 adaptive_redundance_para;        // ��������Ӧ��������������ֹ���ڲ�������
    DIAG_CMD_CONFIG_RT_CYCLE_SEIF_ADAPTIVE_ATTRIB_STR cycle_self_adaptive[RT_CYCLE_PARA_CNT];
}DIAG_CMD_CONFIG_ROUTE_ADAPTIVE_ATTRIB_STRU;

//LED���õ�NV��
typedef struct
{
    HI_U8 tx_rx_led_mode;  // �շ�ָʾ��ģʽ��0:�շ�ָʾ�Ʋ���˸��1:MAC�㱨���շ�,2:APP�����շ�,3:IP�����շ�,4:APP,IP���������շ�
    HI_BOOL phase_tx_led_switch; // ������λָʾ�ƿ���,HI_TRUE:����,HI_FALSE:�ر�
    HI_U8 led_state_switch;    // �쳣״ָ̬ʾ�ƿ����ۿ�,1��ʾ�����ã�0��ʾ����
    HI_U8 pad;

    HI_U32 pad1;
}DIAG_CMD_CONFIG_LED_NV_STRU;

//������ͨ����������
typedef struct
{
    HI_U16 fragment_sleep_time;             //�ְ����ݻ�ȡMacBufferʧ��ʱ�ȴ���ʱ��
    HI_U8  fragment_resend_cnt;             //�ְ����ݻ�ȡMacBufferʧ��ʱ����ش�����
    HI_U8  fc_pkt_num_max_per_req;          //����ͨ���У�ÿ���������ظ��ı�����

    HI_S16 proxy_snr_threshold;             //������ʹ�õĴ���վ���ƽ�����������
    HI_U16 fc_rsp_wait_timer_period;        //Ӧ��ʱ�����ȴ�ʱ��

    HI_U32 fc_req_wait_timer_period;        //һ������������������ʱ��

    HI_U8  fc_send_times_per_req;           //ÿ�������ķ��͵Ĵ���
    HI_U8  fc_send_times_per_rsp;           //ÿ��Ӧ���ķ��͵Ĵ���
    HI_U8  ndm_remote_send_switch;          //�������˵ķ�IPͨʹ�ܿ��أ�1:��,0:�ر�
    HI_BOOL is_fill_mac_addr_to_chip_id;
}DIAG_CMD_CONFIG_NDM_PIPE_NV_STRU;

//Plc ip��NV����
typedef struct
{
    HI_U16 mtu_size;
    HI_U16 pad;
}DIAG_CMD_CONFIG_PLC_IP_NV_STRU;
/**
 *
 * 1.21    ��̬·������
 *
 */
typedef struct _DIAG_CMD_CONFIG_STATIC_ROUTE_STRU_
{
    HI_U8 enable;
    HI_U8 spec;
    HI_U8 oda[6];
    HI_U8 next_mac_0[6];
    HI_U8 next_mac_1[6];
    HI_U8 next_mac_2[6];
    HI_U16 pad;

}DIAG_CMD_CONFIG_STATIC_ROUTE_STRU;


/**
 *
 * 1.23    ����ģʽ����( ����CCO����CCO֪ͨĿ��վ�����ģʽ )
 *
 */
typedef struct _DIAG_CMD_CONFIG_ENABLE_WORK_MODE_STRU_
{
   HI_U8 mode;  //�ṩ��Ӧ��ö������
   HI_U8 pad[3];
}DIAG_CMD_CONFIG_ENABLE_WORK_MODE_STRU;

/**
 *
 * 1.24 TONE MAP �����(��ʵ������TONEMAP����)
 *
 */
typedef struct _DIAG_CMD_CONFIG_TONE_MAP_CAPACITY_STRU_
{
    HI_U32 capacity;
} DIAG_CMD_CONFIG_TONE_MAP_CAPACITY_STRU;

/**
 *
 * 1.27    �ŵ�����������ֵ
 *
 */
//sounding����������
typedef struct _DIAG_CMD_CONFIG_SOUND_STARTUP_THRESHOLD_STRU_
{
    HI_BOOL sounding_enable;
    HI_U8 pad;
    HI_U16 non_robo_fail_threshold;       //��Roboʧ�ܵ����ޣ�������ֵ������������ѵ��

    HI_U8  absolute_thresh[4];          //��������
    HI_U8  relative_thresh[4];          //�������

    HI_U8  pad1[4];
}DIAG_CMD_CONFIG_SOUND_STARTUP_THRESHOLD_STRU;

/**
 *
 * 1.28    ��������
 *
 */
typedef struct
{
    HI_U16 evaluate_phase_bpc_or_cnt_th;    // �������ڣ�BPC
    HI_U16 boot_evaluate_fail_th;           // ����δ����������λ���ű�֡����ʧ�ܵ�������

    HI_U16 evaluate_cmm_diff;               // ͨ�������ʱȽ����ֵ
    HI_U16 evaluate_channel_diff;           // ͨ���ŵ������Ƚ����ֵ

    HI_U8  evaluate_mode;                   // ����ģʽ:1.ͨ�������� 2.�ŵ���������
    HI_U8  evaluate_cycle_mode;             // ��������ģʽ:1.BPCΪ���ڣ�2.���ո���Ϊ����
    HI_U8  channel_mode;                    // �ŵ�����ֵͳ��ģʽ:1.һ��ͳ�������ŵ�����ƽ��ֵ��2.�ŵ������˲�ֵ
    HI_U8  stat_data_mode;                  // ͳ������ģʽ:1.������ͳ������ģʽ 2.��ʷͳ������ģʽ
} DIAG_CMD_CONFIG_PHASE_EVALUATE_ATTR_STRU;

typedef struct
{
    HI_U8  cco_phase_cnt;                   // CCO����λ����(���֧��3����λ)
    HI_U8  default_phase;                   // Ĭ����λ(CCO����λģʽ��ʹ��)
    HI_U8  rx_phase_mode;                   // ��λ����ģʽ:0.����λ�������գ�1.����λͬʱ����
    HI_U8  resend_cnt_mode;                 // 0:ÿ��·���Ϸ���resend_cnt����,
                                            // 1:����·���Ϸ����ܴ���Ϊresend_cnt�Σ�ÿ��·�����ʹ�����ͬ

    HI_U8  max_resend_cnt;                  // ����ش�����
    HI_U8  fixed_resend_cnt;                // �̶��ش�����
    HI_U8  business_parallel_max_cnt;       //ȱʡֵΪ0������ֵ��Ϊ��ʱ�����ӿڷ���max_business_parallel_cnt��ֵ,ֻ���APPҵ������Ч
    HI_U8  business_broadcast_wait_sack;    //ȱʡֵΪ0����ʾ���ȴ�sack������Ϊ1ʱ����ʾ�ȴ�sack,ֻ���APPҵ������Ч

    DIAG_CMD_CONFIG_PHASE_EVALUATE_ATTR_STRU phs_evaluate_para;
}DIAG_CMD_CONFIG_PHASE_ATTR_STRU;

/*
1.31 TDMA��
*/
typedef struct _DIAG_CMD_CONFIG_TDMA_CONFIG_STRU_
{
    HI_U32  max_beacon_period;                              // ����ű����ڳ���
    HI_U32  min_beacon_period ;                             // ��С�ű����ڳ���

    HI_U16  percent_beacon_period;                          // CSMA���ű�ʱ϶����
    HI_U16  alloc_period_athead_time;                       // ��ǰ�����ű�֡��ʱ��

    HI_U8   one_bea_duration;                               // �ű�ʱ϶����
    HI_U8   central_beacon_num;                             // �����ű�֡����
    HI_BOOL no_report_backup_proxy_chance;                  // ���к�ѡ�������Ӱ����ű�ʱ϶�Ĵ���
    HI_U8   assoc_beacon_num;                               // �ܼ������׶ΰ����ű�ʱ϶����

    HI_U8   comm_rate;                                      // �����׶�ͨ����Ԥֵ
    HI_U8   min_discover_beacon_sum;                        // ��С�����ű�ʱ϶����
    HI_U8   net_forming_discover_beacon_state_count;        // �ܼ������ű�֡����
    HI_U8   net_forming_no_report_sta_chance_count;         // �ܼ������׶ΰ����ű�ʱ϶����

    HI_U16  min_phase_duration;                             // ÿ����λCSMA��С����
    HI_BOOL to_be_upgrade_cco;                              // �Ƿ���һ������ģʽ
    HI_U8   alloc_dis_bea_cnt_for_rt_one_sta;               // �����Ż��������ű�ʱ϶����

    HI_U16  min_network_packet_cp;                          // ��ʱ϶����ʱ���������ʱ϶��Сֵ
    HI_U16  tdma_period_msdu_send_min;                      // TDMAʱ϶�ڷ��ͱ��ĸ���(TDMAʱ϶δʹ��)

    HI_U16  mac_receive_msdu_max_interval;                  // TDMAʱ϶��ز���(TDMAʱ϶δʹ��)
    HI_U8   tdma_period_valid_check_policy;                 // TDMAʱ϶��Ч�Լ�����(TDMAʱ϶δʹ��)

    HI_BOOL equip_mode_flag;                                // �Ƿ�Ϊ��װģʽ
    HI_U16  max_proxy_count;                                // ���������
    HI_BOOL tdma_alloc_dfx_switch;                          // TDMAʱ϶�����ά�ɲ⿪��
    HI_U8   tdma_join_count_per_periods;                    // �ܼ������׶ΰ���ÿ�ֿ��������վ��ĸ���

    HI_U8   tdma_pk_duration;                               // ����һ������ʱ��
    HI_U8   performance_test_switch;                        // �Ƿ���������ģʽ��TDMA����(����ģʽ�²�����ʱ϶���룬ʱ϶����δʹ��)
    HI_U16  min_csma_duration;                              // ��СCSMAʱ϶
} DIAG_CMD_CONFIG_TDMA_CONFIG_STRU;

typedef struct _DIAG_CMD_CONFIG_COORDINATE_CONFIG_STRU_
{
    HI_U16 max_backup_sys_time;                 // ��������˱�ʱ��
    HI_U16 max_same_snid_err_time;              // �����������ͬ�����ʱ��

    HI_U16 send_pk_len_for_notify;              // �ڷǱ�CCO ��beacon�����ڷ���Э�����ĵ�����
    HI_U16 send_pk_len_for_coordinate;          // �ڱ�CCO��beacon�����ڷ���Э�����ĵ�����

    HI_U16 check_cannot_comm_limit;             // �жϲ�ͨ�����ޣ���λΪϵͳʱ�䣬�೤ʱ����û�յ�Ϊ��ͨ
    HI_U16 listen_time_before_first_bea;        // �ϵ������ھ����������Ϣ��ʱ��

    HI_BOOL open_coordinate_network;            // ������Э������
    HI_U8  network_evaluation_enable;           // �Ƿ�������ѡSNID���ܣ�0������
    HI_U8  continue_try_assoc_th;               // �������Թ����ִ�(��������в��ԳƳ������������糢�Զ����ֺ��ӳٹ���ʱ�䵽���ֵ)
    HI_U8  max_assoc_time;                      // ������ʱ��(��������в��ԳƳ�������������������ʱ��)

    HI_BOOL cco_snid_coordinate_switch;         // CCO�������Э������
    HI_BOOL sta_snid_coordinate_switch;         // STA�������Э������
    HI_BOOL conflict_detect_switch;             // ��ͻ��⿪��
    HI_BOOL is_coordinate_with_pv100_in_sg;     //�ڹ���Э�����Ƿ���V100Э��Э��

    HI_U16  coordinate_backoff_time;            // �������ͻ�˱�ʱ��
    HI_U16  change_sind_notify_time;            // ����ű��֪ͨʱ��

    HI_U16 exception_by_bpc_th;                 // BPC�쳣����
    HI_U16 exception_by_cco_mac_th;             // CCO MAC��ַ�쳣����
    HI_U16 report_interval;                     // �ϱ����
    HI_U16 exception_interval;                  // �쳣���
    HI_U16 max_send_query_neighbor_info_num;
    HI_BOOL protocol_type;
    HI_U8  pad;
}DIAG_CMD_CONFIG_COORDINATE_CONFIG_STRU;

/*
2.34  ���ͻ�CSMA�˱��㷨
*/
#define TX_CSMA_BACKOFF_BC_TABLE_ITEMS 20
#define TX_CSMA_BACKOFF_BC_DEFAULT_SLOT_MULTI 5
#define TX_CSMA_BACKOFF_BC_CCO_SLOT_MULTI 2
typedef struct _DIAG_CMD_CONFIG_CMSA_BACKOFF_POL_
{
    HI_U8 default_node_count;               // Ĭ�� ����Χվ����� == 10
    HI_U8 default_slot;                     // ==100
                                            // (*TX_CSMA_BACKOFF_BC_DEFAULT_SLOT_MULTI)��
                                            // �������õ�ֵ����TX_CSMA_BACKOFF_BC_DEFAULT_SLOT_MULTI.
    HI_U8 fixed_bc;                         // ����������ȼ� �̶����˱��� =4
    HI_U8 min_node_count;                   // С��Χ��ͻ������վ���� =2
    HI_U8 sm_scope_slot;                    // ������Χ ��С��Χ�� վ��ʱ��slotֵ. =120
    HI_U8 sm_scope_bc;                      // ������Χֻ����С��Χ��  ʱ�� �˱� ֵ. =7
    HI_U8 bg_scope_slot;                    // ������Χ���� TX_CSMA_BACKOFF_BC_TABLE_ITEMS
                                            // ��ʱ��slot . = 100  * TX_CSMA_BACKOFF_BC_DEFAULT_SLOT_MULTI
    HI_U8 bg_scope_bc;                      // ������Χ���� TX_CSMA_BACKOFF_BC_TABLE_ITEMS ��ʱ��BC,
                                            // = 31
    HI_U8 bc_table[TX_CSMA_BACKOFF_BC_TABLE_ITEMS];
}DIAG_CMD_CONFIG_CMSA_BACKOFF_ALGORITHM3_STRU;

typedef struct _DIAG_CMD_CONFIG_TX_CSMA_ALGORITHM_
{
    HI_U16 cw_slot;
    HI_U16 cw_min;

    HI_U16 cw_max;
    HI_U16 cw_multi;

    HI_U16 collision_start;
    HI_U16 collision_end;

    HI_U8  collision_multi;
    HI_U8  resend_start;
    HI_U8  resend_end;
    HI_U8  resend_multi;

    HI_U8  priority_start;
    HI_U8  priority_end;
    HI_U8  priority_multi;
    HI_U8  ver_sw;              // 1, ���㷨��2, ���㷨, 3 �㷨3

    HI_U8  hi_range[16];        // ���㷨ʹ�õ��˱��������ֵ
    HI_U8  low_range[16];

    DIAG_CMD_CONFIG_CMSA_BACKOFF_ALGORITHM3_STRU alg3;
}DIAG_CMD_CONFIG_TX_CSMA_ALGORITHM_STRU;

typedef struct _DIAG_CMD_CONFIG_ASSOC_CONFIG_
{
    HI_U8       mac_auth_enable;                        // ����MAC��֤
    HI_U8       nmk_auth_enable;                        // ����NMK��֤
    HI_U8       req_send_bpc_interval;                  // ���͹�����������BPC
    HI_U8       ind_send_max_count;                     // ����ind����������

    HI_U16      ind_send_time_interval;                 // ����ind���ļ��ʱ��
    HI_U16      max_child_sum;                          // �����վ�����

    HI_U8       is_carry_dfx_switch;                    // ������������Ƿ�Я����ά�ɲ���Ϣ����
    HI_U8       assoc_gather_switch;                    // ����ָʾ���ܿ���
    HI_U16      min_change_proxy_req_resend_time;       // �ȴ��������ظ���Сʱ��

    HI_U8       assoc_req_try_max;                      // ������������������
    HI_U8       wait_gather_ind_time;                   // �ȴ����ɻ��ܱ���ʱ��,��λs
    HI_U8       change_proxy_req_try_max;               // ����������������
    HI_U8       ind_resend_max_count;                   // ind�����ش�����

    HI_U8       disable_optimal_network_comm_rate;      // �رն�������ѡ���ܵ�ͨ��������
    HI_U8       network_evaluation_time;                // ��������ʱ�䣬��λs
    HI_BOOL     is_chip_ver_pkt_bit_enable;             // ͨ��V100Э���й�����������ӵı����ж�оƬ�Ĺ����Ƿ�ʹ��
    HI_U8       ptcl_detect_no_sta_time_len;            // ��վ����뼴����Э��̽������ޣ���λmin

    HI_U32      wait_for_mac_calculation_timeout;       // ������ַʱ��
    HI_U16      detect_period_interval;                 // CCO����Э��̽�������,��λ:min
    HI_BOOL     is_ptcl_detect_enable;                  // Э��̽���Ƿ�ʹ��
    HI_U8       detect_time_len;                        // Э��̽�ⱨ�ķ��͵ĳ���ʱ�䣬��λ:min

    HI_U8       wait_assoc_ind_timeout;                 // �ȴ�����ָʾ����ʱ�䣬��λs
    HI_U8       max_assoc_req;                          // ������������
    HI_BOOL     random_assoc_enable;                    // ����������ܿ���
    HI_U8       slot_dfx_switch;

    HI_U16      assoc_not_in_whiter;                    // δ�ڰ���������ʧ�ܺ��ٴη����������ʱ��
    HI_U16      assoc_not_set_whiter;                   // ����δ���ð���������ʧ�ܺ��ٴη����������ʱ��

    HI_U16      assoc_too_much_sta;                     // ����վ��������ʧ�ܺ��ٴη����������ʱ��
    HI_U16      assoc_too_much_pco;                     // ����վ��������ʧ�ܺ��ٴη����������ʱ��

    HI_U16      assoc_too_much_child;                   // ��վ��������ʧ�ܺ��ٴη����������ʱ��
    HI_U16      assoc_too_much_level;                   // �㼶����������ƹ���ʧ�ܺ��ٴη����������ʱ��

    HI_U16      assoc_repeat_mac;                       // �ظ�MAC��ַ����ʧ�ܺ��ٴη����������ʱ��
    HI_U16      ptcl_recover_time_len;                  // �Ͱ汾Э��վ�����ߺ�ָ��߰汾Э���ʱ�䣬��λ������

    HI_U16      detect_pkt_send_interval;               // Э��̽�ⱨ�ĵķ�������(�����ķ��ͼ��)����λ:s
    HI_BOOL     end_sequence_switch;                    // �˵�����ż�⿪��
    HI_BOOL     path_sequence_switch;                   // ·����ż�⿪��

    HI_U16      white_leave_sta_th;                     // �����������һ������߳�վ���������
    HI_U16      white_table_notify_num;                 // �������߳�վ������֪ͨ����
    HI_U16      cco_send_leave_pk_time;                 // CCO�����ӳ�����ָʾ���ļ��
    HI_U16      delay_leave_time_len;                   // STA�ӳ�����ʱ��

    HI_U8       heartbeat_leave_cycle_th;               // ���������վ��������������
    HI_U8       heartbeat_report_cycle_th;              // ���������վ���ϱ���������
    HI_U16      leave_info_storage_cycle;               // ������Ϣ�洢���ڣ���λ:����

    HI_U8       leave_info_storage_switch;              // ������Ϣ�洢����
    HI_U8       leave_info_storage_th;                  // һ�����ڴ洢����
    HI_U8       route_cylce_per_heartbeat_cycle;        // �������ڰ���·�����ڸ���
    HI_U8       heartbeat_pk_cnt_per_heartbeat_cycle;   // ÿ���������ڷ��ͼ�����������
}DIAG_CMD_CONFIG_ASSOC_CONFIG_STRU;

typedef struct _DIAG_CMD_CONFIG_SNR_CONFIG_STRU_
{
    HI_U8   ntb_crt_enable          : 1;        // NTB���������Ƿ�ʹ��
    HI_U8   is_first_use_ntb_result : 1;        // �Ƿ�����ʹ��NTB���
    HI_U8   pad0                    : 6;
    HI_U8   snr_history_num;                    // ���ڼ�¼����
    HI_U8   snr_collect_period;                 // ���ݲɼ����ڣ���λmin
    HI_U8   save_snr_times_th;                  // ����������ģʽ�£���ʷ��¼�����ﵽ���޾��ó�������

    HI_U8   belong_stable_times_th;             // ������Ϣ�ȶ���������
    HI_U8   snr_class_stable_times_th;          // SNR�㼶�ȶ���������
    HI_U16  snr_collect_duration_th;            // ͳ��ʱ�����ޣ�δ�ﵽ�����������ݲ��洢����ʷ��¼����λs

    HI_U16  snr_unit_duration;                  // ��λʱ�䳤�ȣ���λs
    HI_S8   snr_avg_th;                         // SNRƽ��ֵ����
    HI_U8   snr_unit_fc_cnt_abs;                // ÿ��λʱ�����FC��������

    HI_U8   his_valid_duration_th;              // ��ʷ��¼�ڸ�ʱ���ڣ��ſ����ڸ���SNR�㼶����λmin
    HI_BOOL fast_convergence_enable;            // �����������ʹ��
    HI_U8   lock_belong_net_duration;           // ����������ģʽ�£����ߺ��������������ã���λmin
    HI_U8   fast_lock_duration;                 // �����������: ÿ���л�����������ʱ�䣬��λmin

    HI_U8   stable_change_enable;               // ���������л�����ʹ��
    HI_U8   ntb_crt_att_redundance_th;          // NTB�������Ե�˥����������
    HI_U8   stable_check_first_time;            // ������ٴκ�ʼ��һ�μ��
    HI_U8   stable_check_interval;              // ÿ�����μ��һ��

    HI_U8   stable_greater_rate_th;             // A,B��������Ƚ�SNR,A�����B����Ҫ�õĴ�������,Ĭ��80%
    HI_U8   stable_gt_or_eq_rate_th;            // A,B�����Ƚ�SNR,A������ڵ���B����Ĵ������ޣ�Ĭ��90%
    HI_U16  stable_lock_duration;               // ��ͨ����ʱ������λmin

    HI_U16  pream_sta_num_th;                   // preamble���ͷ��������ޣ����������޵�snrֱ�Ӷ���,Ĭ��:80
    HI_U8   alpha_numerator;                    // ƽ��snr���˲�������,Ĭ��:1
    HI_U8   alpha_denominator;                  // ƽ��snr���˲�����ĸ,Ĭ��:16

    HI_U8   ntb_crt_attenuation_th;             // NTB�������Ե�˥��ֵ����
    HI_U8   stable_snr_redundance_th;           // ��������snr��Ժ�ƽ��ֵ�ıȽ��������ޣ�ȱʡֵ10����λ0.1DB
    HI_U16  ntb_crt_lock_duration;              // NTB������������ʱ������λmin

    HI_U8   fast_accurate_rounds[SNR_FAST_ACCURATE_TH_NUM]; // ������������
    HI_U8   fast_accurate_avgs[SNR_FAST_ACCURATE_TH_NUM];   // SNRƽ��ֵ���ޣ���λ0.1DB

    HI_U16  attenuation_th;                     // ˥�����ޣ��ŵ�˥�����ڴ�����ʱ�����ж�preamble�������ޣ�Ĭ��:10
    HI_U8   is_snr_class;                       // �Ƿ�ʹ��SNR�㼶
    HI_U8   belong_rcv_time_th;                 // ֻҪ���һ��ʱ�����յ����������磬ʶ������Ϊ�������磬��λmin
} DIAG_CMD_CONFIG_SNR_CONFIG_STRU;

#define NM_SNR_TF_ADAPT_PARA_NUM    (5)

typedef struct
{
    HI_U16 station_cnt;         // ̨����ģ
    HI_U8  topo_stable_time;    // �����ȶ�ʱ������λmin
    HI_U8  pad;
} DIAG_CMD_CONFIG_SNR_TF_ADAPT_PARA_ST;

typedef struct
{
    HI_U16  snr_tf_time_max;    // SNR̨��ʶ��ʹ�����ʱ������λmin
    HI_U8   is_tf_complete;     // SNR̨��ʶ���Ƿ����
    HI_U8   is_snr_class;       // �Ƿ�ʹ��SNR�㼶

    DIAG_CMD_CONFIG_SNR_TF_ADAPT_PARA_ST adapt_para[NM_SNR_TF_ADAPT_PARA_NUM];  // SNR̨��ʶ������Ӧ����

    HI_U16 snr_tf_time_exceed;  // ��������ģʱʹ��ʱ������λmin
    HI_U16 pad1;

    HI_U32  pad2[9];
} DIAG_CMD_CONFIG_SNR_TF_CONFIG_STRU;

typedef struct _DIAG_CMD_CONFIG_TX_SOF_TMI_POLICY_
{
    HI_U32   default_72          :4;
    HI_U32   default_136        :4;                //PB136�ش�����TMI�ر�ʱʹ�õ�TMI
    HI_U32   default_264        :4;
    HI_U32   default_520        :4;                //PB136�ش�����TMI�ر�ʱʹ�õ�TMI

    HI_U32   resend_72_enable           :1;          //PB136�ش�����TMIʹ�ܿ���
    HI_U32   resend_136_enable         :1;          //PB136�ش�����TMIʹ�ܿ���
    HI_U32   resend_264_enable         :1;          //PB136�ش�����TMIʹ�ܿ���
    HI_U32   resend_520_enable         :1;          //PB520�ش�����TMIʹ�ܿ���
    HI_U32   pad                                   :4;

    HI_U32   tmi_72_cnt                       :4;
    HI_U32   tmi_264_cnt                     :4;

    HI_U8   tmi_136_cnt;
    HI_U8   tmi_520_cnt;
    HI_U8   resend_72[4];
    HI_U8   resend_136[6];

    HI_U8   resend_264[4];
    HI_U8   resend_520[6];
    HI_U8   tmi_auto_adjust_enable;     //sof���ش�tmi�Զ�����
    HI_U8   degrade_resend_cnt;

    HI_U32  recover_tmi_period;             //�ָ�Ĭ��tmi�Ķ�ʱ������

    HI_U32   broadcast_72                  :4;
    HI_U32   broadcast_136                :4;
    HI_U32   broadcast_264                :4;
    HI_U32   broadcast_520                :4;
    HI_U32   ext_tmi_72                    :4;                //PB72����չTMI
    HI_U32   ext_tmi_136                  :4;                //PB136����չTMI
    HI_U32   ext_tmi_264                  :4;                //PB264����չTMI
    HI_U32   ext_tmi_520                  :4;                //PB520����չTMI
}DIAG_CMD_CONFIG_TX_SOF_TMI_POLICY_STRU;

typedef struct _DIAG_CMD_CONFIG_BCN_BEACON_TMI_
{
    HI_U8 payload520_tmi     :4;
    HI_U8 payload136_tmi     :4;
    HI_U8 payload72_tmi       :4;
    HI_U8 payload264_tmi     :4;
    HI_U8 pad[2];
}DIAG_CMD_CONFIG_BCN_BEACON_TMI_STRU;


typedef struct _DIAG_CMD_RECODE_RX_STAT_CONFIG_STRU_
{
    HI_U8 dfx_rx_report_switch;
    HI_U8 dfx_tx_report_switch;
    HI_U16 pad;

    HI_U8 rcv_fc_switch;
	HI_U8 diff_flag;
    HI_U8 pad2[2];
}DIAG_CMD_RECODE_RX_STAT_CONFIG_STRU;

typedef struct
{
    HI_U8 mac_speed_report_sw; // 0: �رռ�¼��Ϣ�� ��0��
    HI_U8 mac_speed_report_peroid;// MAC�����ϱ�����
    HI_U8 mac_speed_exception_notify_sw;// MAC�����ʵ����������޸澯����
    HI_U8 pad;
    HI_U32 mac_rx_speed_therold; // �����������ֵ
    HI_U32 mac_tx_speed_therold; // �����������ֵ

}DIAG_CMD_CONFIG_MAC_SPEED_ENTRY;

//�����쳣���Ե�NV��
typedef struct
{
    HI_U8  detect_timer_switch;             //���ܿ���:0,�أ�1,��
    HI_U8  speed_report_switch;             //�����ϱ�����:0,�أ�1,��
    HI_U16 detect_timer_period;             //���Ӷ�ʱ��������,��λms

    HI_U16 eth_rx_ip_cnt_min;               //��̫������IP������С��ֵ
    HI_U16 eth_tx_ip_cnt_min;               //��̫������IP������С��ֵ
    HI_U16 mac_rx_ip_cnt_min;               //MAC����IP������С��ֵ
    HI_U16 mac_tx_ip_cnt_min;               //MAC����IP������С��ֵ

    HI_U8  autio_detect_switch;             //�Զ���⹦�ܿ���:0,�أ�1,��
    HI_U8  tx_trace_switch;                 //���ͱ��ĸ����ϱ�����
    HI_U8  rx_trace_switch;                 //���ձ��ĸ����ϱ�����
    HI_U8  tx_time_swith;                   //����ʱ���ϱ�����
}DIAG_CMD_CONFIG_SPEED_ABNORMITY_NV_STRU;

typedef struct
{
    DIAG_CMD_CONFIG_MAC_SPEED_ENTRY mac_speed_config;
}DIAG_CMD_CONFIG_SPEED_CONFIG_STRU;
typedef struct
{
    DIAG_CMD_RECODE_RX_STAT_CONFIG_STRU recode_rx_stat_policy;
}DIAG_CMD_CONFIG_DFX_CONFIG_STRU;


typedef struct _DIAG_CMD_IP_ROUTE_ST_
{
    HI_U8  flag;
    HI_U8  rsvd[3];
    HI_U8  ip_dest[4];
    HI_U8  net_mask[4];
    HI_U8  gate_way[4];
}DIAG_CMD_IP_ROUTE_ST;

typedef struct _DIAG_CMD_CONFIG_IP_ROUTE_
{
    HI_U8  plc_ip[4];                           // PLC IP��ַ����
    HI_U8  plc_net_mask[4];                     // PLC IP��ַ����

    HI_U8  auto_mapping;                        // �Ƿ�ͨ��MAC��ַӳ��ΪETH IP
    HI_U8  rsvd[3];
    HI_U8  eth_ip[4];                           // ETH IP��ַ����
    HI_U8  eth_net_mask[4];                     // ETH IP��ַ����

    DIAG_CMD_IP_ROUTE_ST ndm_route_sta;         // ������·���·��3
    DIAG_CMD_IP_ROUTE_ST ndm_route_ndm;         // ������·���·��3
    DIAG_CMD_IP_ROUTE_ST concentrator_route;    // ������·���CCO�˲����ø�NV�gate_way�������󣬶�̬��ȡ��·��2
    DIAG_CMD_IP_ROUTE_ST test_route;            // ���ܲ���·����
}DIAG_CMD_CONFIG_IP_ROUTE_STRU;
typedef struct _DIAG_CMD_CONFIG_SW_NTB_
{
    HI_U32  ntb_control_mode;                           // NTB����ģʽ(оƬ���ƻ����������)
    HI_U32  synch_time_reset_th;                        // NTBʱ��ͬ����ʱ����
    HI_U32  synch_time_detch_th;                        // NTBʱ��ͬ����ʱ��鶨ʱ��ʱ��

    HI_S16  freqerr_boot_set_flag;                      // �Ƿ��ϵ��ȡFLASH�д洢��Ƶƫ
    HI_S16  freqerr_boot_nv;                            // FLASH�д洢Ƶƫ(��ʱ����)

    HI_U8   detect_cycle_flag;                          // �������ڼ�⿪��
    HI_U8   exception_reset_to_initial_flag;            // �쳣��λ����ʼ������
    HI_U8   freqerr_nv_refresh_flag;                    // ��ȡ����Ƶƫ����
    HI_U8   synch_detect_flag;                          // ͬ����ʱ��⿪��

    HI_U32  ntb_detect_stat_cycle_len;                  // NTBͳ���ű�֡��������ʱ��
    HI_U16  ntb_rcv_beacon_th;                          // ͳ���ű�֡��ÿ��վ����ѡ��͸�������
    HI_U8   ntb_dectect_stat_flag;
    HI_U8   ntb_synch_send_beacon_switch;               // NTBֻ����ͬ��״̬��������վ�㷢���ű���Կ���
    HI_U32  detect_cycle;                               // �������ڳ���

    HI_U16  h_par_th;                                   // �����������
    HI_U16  fccs_fail_th;                               // FCУ��ʧ������
    HI_U16  fccs_success_th;                            // FCУ��ɹ�����

    HI_U16  pb_crc_fail_th;                             // PB��ʧ������
    HI_U16  pb_crc_success_th;                          // PB��ɹ�����
    HI_U16  pb_crc_success_nv_th;                       // �洢Ƶƫ��FLASH��PBУ��ɹ�����

    HI_U32  ntb_soft_control_rcv_beacon_len;            // �������NTB��δ���յ��ű�֡�ʱ������
    HI_U32  ntb_soft_control_len;                       // �������NTB�ʱ������

    HI_U8   ntb_chip_state1_exception_th;                // ʱ��ͬ��оƬ������״̬1�쳣��������
    HI_U8   rsvd[3];
}DIAG_CMD_CONFIG_SW_NTB_STRU;
typedef struct _DIAG_CMD_CONFIG_BEACON_NV_
{
    HI_U8 alloc_slot_oneself_flag;      // ��ʱ϶����ʱ���Ƿ����ʱ϶��������
    HI_U8 cco_update_beacon_slot_flag;  // CCO���ű�ʱ϶�Ƿ����վ�����λ�л���λ����
    HI_U8 ptcl_detect_bcn_send_cnt;     // ÿ��Э��̽���ű�ķ��ʹ���
    HI_U8 ptcl_detect_bcn_send_mode;    // Э��̽���ű�ķ���ģʽ

    HI_U32 alloc_slot_time_th;          // �೤ʱ���ղ����ű�󣬲���ʱ϶
    HI_U32 alloc_slot_len;              // ������ʱ϶ʱ��
    HI_U32 protect_cp_len;              // �ظ������ϸ�����ʱ϶�ı������
    HI_U32 csma_slot_slice_th;          // CSMAʱ���Ƭ����С��λ����λ:SLICE_SLOT_UNIT(10ms)
}DIAG_CMD_CONFIG_BEACON_NV_STRU;

typedef struct _DIAG_CMD_CONFIG_LOCAL_BROADCAST_DEFAULT_SEND_TIMES_NV_STRU_
{
    HI_U32 local_broadcast_default_send_times;  //���ع㲥����Ĭ�Ϸ��ʹ���
}DIAG_CMD_CONFIG_LOCAL_BROADCAST_DEFAULT_SEND_TIMES_NV_STRU;

typedef struct _DIAG_CMD_CONFIG_FILTER_SWITCH_NV_STRU_
{
    HI_U32 filter_switch;   //���Ĺ��˿���

    HI_BOOL is_freq_filter_enable;
    HI_U8  filter_timeout;
    HI_U8  pad[2];
}DIAG_CMD_CONFIG_FILTER_SWITCH_NV_STRU;

#define FREQ_CHANNEL_CNT  (13)

typedef struct _DIAG_CMD_CONFIG_BLIND_CHECK_NV_STRU_
{
    HI_BOOL blind_switch;
    HI_U8   chip200_v100_freq[6];              // V200оƬ��V100Ƶ������(������ȫƵ)
    HI_U8   chip200_v200_freq[10];             // V200оƬ��V200Ƶ������(������ȫƵ)
    HI_U8   chip200_full_freq[2];              // V200оƬ��ȫƵ����
    HI_U8   chip200_sg_freq[6];              // V200оƬ�Ϲ���Ƶ������
    HI_U8   blind_check_mode;                   // ä��ģʽ
    HI_BOOL sta_equip_mode_flag;                //���sta��װģʽ���
    HI_U8   rsvd;

    HI_U8   sg_standard_freq[4];               // SG��׼Э�飬����SG�л�Ƶ��ʹ�ã�Ĭ��10,12,255,255

    HI_U32  whole_network_change_freq_time;    // CCO֪ͨSTA��ú���Ƶ��
    HI_U32  blind_check_time;                  // ä������
}DIAG_CMD_CONFIG_BLIND_CHECK_NV_STRU;

//��������NV��
typedef struct _DIAG_CMD_CONFIG_NDM_CONNECT_PARA_STRU_
{
    HI_U8  bea_normal_duration;
    HI_BOOL fix_meter_ndm_tei_enable;              //���������ģʽ���ֶ����ó�������TEI����
    HI_U16 meter_check_frame_time;                 //�������֡����ʱ��,��λms

    HI_U32 query_nm_attr_cnt_max_per_tei;           //��ѯ������������Ե�ʱ�����������ѯ����
    HI_U32 notify_meter_check_beacon_time;          //�������֪ͨ�ű�����ms
    HI_U32 maintain_meter_check_beacon_time;        //�������ά���ű�����ms
    HI_U32 close_meter_check_time;                  //�ر������⹦�ܵ�ʱ�䣬��λ:s

    HI_BOOL sta_meter_connect_enable;               //STA�ϵ�������Կ��أ�1:ʹ�ܣ�0:�ر�
    HI_U8  pad3;
    HI_U16 ndm_resend_times;

    HI_U16 ndm_tei;
    HI_BOOL heartbeat_send_enable;                  //�������������ķ��Ϳ���
    HI_U8  work_mode;                               //�������Ĺ���ģʽ

    HI_U8  ndm_attr_br_autio_enable;                //�������Ƿ��Զ��㲥���Կ���
    HI_U8  ndm_attr_br_pk_max;                      //���������Թ㲥���������
    HI_U16 query_nm_attr_tei_cnt_max;               //��ѯ������������Ե�ʱ�����������ѯtei����

    HI_U32 ndm_attr_br_period;                      //���������Թ㲥����
    HI_U32 qurey_nm_attr_period;                    //��ѯ������������ԵĶ�ʱ��������
    HI_U32 refresh_disc_timer_period;               //�������ķ����б�ˢ�¶�ʱ��������
    HI_U32 connect_monitor_timer_period;            //������ǿ�����Ӽ�ⶨʱ��������
    HI_U32 heartbeat_timer_period;                  //��������������ʱ��������
}DIAG_CMD_CONFIG_NDM_CONNECT_PARA_STRU;

typedef struct _DIAG_CMD_CONFIG_STA_GPIO_SET_PARA_STRU_
{
    HI_U8 tx_app_switch;       //0��ʾ����MAC����ʱ�ø�GPIO�ܽţ�1��ʾ����APP����ʱ�ø�GPIO�ܽ�
    HI_BOOL send_one_bcn_in_bpc_enable;  // ��һ��bpc��ֻ������һ���ű�
    HI_U16 message_send_length; //STA�ܽ����ߵ�ʱ�����Ƶ�λus
    HI_BOOL energy_mode_enable;         // ��Դģʽʹ�ܿ���
    HI_U8 detect_bcn_send_interval;     // ��Դģʽ��V100̽���ű�֡���ͼ��,Ĭ��6s,��λs
    HI_BOOL energy_test_mode_enable;    // ��Դģʽ�£������Բ⿪��,Ĭ��HI_FALSE
    HI_U8 pad;
    HI_U32 pad1;
}DIAG_CMD_CONFIG_STA_GPIO_SET_PARA_STRU;

#define MAX_PHY_SLEEP_CYCLE_LISTEN_NUM      (5)
#define MAX_OPEN_PHY_TIME_RANGE_NUM         (3)

typedef struct
{
    HI_BOOL phy_low_power_switch;               // PHY�͹��Ŀ���
    HI_BOOL phy_sleep_in_power_on_switch;       // �ϵ�״̬���Ƿ�PHY�Ƿ�������˯��
    HI_BOOL low_power_dfx_switch;               // �͹��Ŀ�ά�ɲ⿪��
    HI_U8   close_phy_cnt_assoc;                // ����״̬�£��ر�PHY��������

    HI_U8   open_phy_cnt_assoc;                 // ����״̬�£�����PHY��������
    HI_U8   suspend_phy_sleep_num;              // ä������ֺ󣬹���PHY˯��1������
    HI_U16  assoc_to_listen_timeout;            // ����״̬�л�������״̬�ĳ�ʱʱ�䣬��λ��

    HI_U16  power_on_keep_work_time;            // �ϵ籣�ֹ�����ʱ�䣬��λ����
    HI_U16  init_power_on_timeout;              // �ϵ�״̬�л�������״̬��ʱ�䣬��λ��

    HI_U8   random_sleep_num;                   // ˯�߶���������һ��
    HI_U8   close_phy_cnt_listen;               // ����״̬�£��ر�PHY��������
    HI_U8   open_phy_cnt_listen;                // ����״̬�£�����PHY��������
    HI_U8   wait_assoc_cnf_time;                // �ȴ������ظ���ʱ�䣬��λ��

    HI_U8   continue_open_phy_choose_cnt;       // ���������ֺ󣬳�����PHY
    HI_U8   good_waking_sleep_rate_choose_cnt;  // ���������ֺ�ѡ��õ���˯��
    HI_U8   good_close_phy_cnt_assoc;           // �õ���˯�ȹر�PHY��������
    HI_U8   good_open_phy_cnt_assoc;            // �õ���˯�ȴ�PHY��������

    HI_U32  phy_sleep_cycle_power_on;           // �ϵ�״̬�£�PHY˯�ߵ����ڣ���λ΢��
    HI_U32  phy_sleep_cycle_listen[MAX_PHY_SLEEP_CYCLE_LISTEN_NUM];           // ����״̬�£�PHY˯�ߵ����ڣ���λ΢��
    HI_U32  phy_sleep_cycle_assoc;              // �����׶Σ�PHY˯�ߵ����ڣ���λ΢��
    HI_U16  open_phy_time_begin[MAX_OPEN_PHY_TIME_RANGE_NUM];                 // ����PHY�򿪵Ŀ�ʼʱ��
    HI_U16  open_phy_time_end[MAX_OPEN_PHY_TIME_RANGE_NUM];                   // ����PHY�򿪵Ľ���ʱ��
}DIAG_CMD_CONFIG_MAC_LOW_POWER_STRU;


typedef struct _DIAG_CMD_CONFIG_MONITER_CONFIG_STRU_
{
    HI_BOOL  print_discover_list;
    HI_BOOL  save_discover_list_to_flash;
    HI_U16   rsvd;

    DIAG_CMD_CONFIG_MAC_LOW_POWER_STRU mac_low_power;
} DIAG_CMD_CONFIG_MONITER_CONFIG_STRU;

typedef struct _DIAG_CMD_CONFIG_MAC_NDM_CHL_STRU_
{
    HI_U8 ucAccessMode;                     //����ģʽ��ȡֵHI_NDC_CHL_ACCES_MODE_E
    HI_U8 ucPreferenceFreq;                 //����������Ƶ��
    HI_U8 ucPreferenceSNID;                 //����������SNID
    HI_U8 aucPreferenceCcoMac[6];           //�����������������������ѡ���ĸ�CCO����
    HI_U8 ucFreqScanPeriodCco;              //CCOƵ�μ������ڣ���λ�룬Ĭ��120�롣
    HI_U8 ucFreqScanPeriodSta;              //STAƵ�μ������ڣ���λ�룬Ĭ��170�롣
    HI_U8 ucFreqShortPeriod;                //����Ƶ�μ��������ڣ���λ�룬Ĭ��60��

    HI_U8 ucWaitConnectTimeOut;             //�ȴ�������ɳ�ʱʱ�䣬Ĭ��4�롣
    HI_U8 ucIdleTimeout;                    //����״̬���ΪIDLE���������ʱ�䣬���ں�תSEARCH����λ�룬Ĭ��5��
    HI_U8 ucReportPeriod;                   //����������ϱ�����,��λ�룬ȱʡ3��
    HI_U8 ucMaxSearchCnt;                   //�Զ����������������һ��ָ����������ЧƵ�Σ�Ĭ��Ϊ3;

    HI_U8 aucFreqIdx[FREQ_TRY_NUM];         //NDMʹ�õ�Ƶ��������Χ.HI_MDM_PLC_FREQ_1,HI_MDM_PLC_FREQ_2,HI_MDM_PLC_FREQ_3
    HI_U8 ucFilterFreq;                     // ���V200�������˷ǵ�ǰƵ���µ�����Ƶ����Ϣ
    HI_U8 ucFixRxEn;                        // ���V200�������رղ��м�⹦��
    HI_BOOL isFilterZeroIp;                 // �Ƿ����V100Э����NDC��Ŀ��IP��0����

    HI_U8 ucMaxConnectRetry;                //����ʧ�ܵ����Դ�����Ĭ��3�Ρ�
    HI_U8 ucMinSacnFcCnt;                   //Ƶ�μ��������ڣ��������������FC������������������ڻ�����Ч��Ĭ��Ϊ3
    HI_U8 ucMaxShortScan;                   //ʹ�ö������Զ���������������Ĭ��Ϊ3

    HI_U8 ucReportPrintSwitch;              //NDC�ϱ���HSO����Ϣ���Ƿ��ӡ��message�Ŀ��ء�ȱʡ����ӡ��
    HI_U8 ucConnMonPeriod;                  //NDC���Ӽ��ˢ�µ����ڣ���λ�룬Ĭ��10�롣
    HI_U8 ucAutoSearchNetEnable;            //�Զ������ܿ���
    HI_U8 ucExpireCheckPeriod;              //���������ʱ����ϻ���ʱ��ʱ�䣬Ĭ��Ϊ60s

    HI_U8 ucNtbAtuoSynchEnable;              // ���������У���̬����NTBͬ�����ܿ��أ�Ĭ��Ϊ��
    HI_U8 ucRcvFcTh;                         // �����Ƿ����NTBͬ��������ǰ�����fc�ĸ�����Ĭ��Ϊ5��
    HI_U8 pad[2];
}DIAG_CMD_CONFIG_MAC_NDM_CHL_STRU;

typedef struct
{
    HI_U32 mutli_exception_by_diff_mac_time_th;        //SNID��ͬ��MAC��ͬ�Ķ������쳣��ʱ������ֵ,��λmin
    HI_U32 multi_exception_by_diff_mac_cnt_th;         //SNID��ͬ��MAC��ͬ�Ķ������쳣�Ĵ�������ֵ
    HI_U32 multi_exception_by_ntb_reset_time_th;       //NTB�쳣��λ����Ķ������쳣��ʱ������ֵ,��λmin
    HI_U32 multi_exception_by_ntb_reset_alarm_cnt_th;  //NTB�쳣��λ����Ķ������쳣�Ĵ�������ֵ
    HI_U32 multi_exception_timer_period;               //�������쳣��ʱ�����ڣ���λms
}DIAG_CMD_CONFIG_MAC_MULTI_EXCEPTION_CTRL_STRU;

//�Զ����ʿ���
typedef struct
{
    HI_U8   is_power_ctrl_enable    : 1;                                           //���ʿ���ʹ�ܿ���{1}
    HI_U8   is_recover_power_enable    : 1;                                   // �Ƿ�ָ�ԭ�����ַ��书�ʿ���{1}
    HI_U8   set_white_list_time_min_th    : 4;
    HI_U8   rsvd    : 2;
    HI_S8   digital_power_v100;
    HI_U16  set_v100_detect_digital_power_time_th;

    HI_S8   digital_power[4];                                                       //ȫ��֪ͨ�����ַ��书��{9,14,16,18}

    HI_U16  send_interval;                         //whole_net_set_digital_power_timer���ں󣬷���ȫ�����÷��书�ʵ�V100̽���ű��֡�������λs{15}
    HI_U16  send_cnt_max;                        //whole_net_set_digital_power_timer���ں󣬷���ȫ�����÷��书�ʵ�V100̽���ű��������{120}

    HI_U32  set_auth_time_th;                                                //���ð��������������ʿ��Ƶ�ʱ�����ޣ���λs{3600}1h
    HI_U32  whole_net_set_digital_power_timer_period;         //ȫ��֪ͨ���ַ��书�ʵ����ڣ�whole_net_set_digital_power_timer��ʱʱ�䣬��λs{3600}1h
    HI_U32  power_ctrl_form_timeout_period;                          //���ʿ��ƻ���STA������ʱʱ�䣬power_ctrl_form_timeout_timer��ʱʱ�䣬��λs{3600}1h

    HI_U8   set_v100_power_rtc_hour_min;
    HI_U8   set_v100_power_rtc_hour_max;
    HI_U8   recover_v100_power_delay_time;
    HI_U8   pad;
}DIAG_CMD_CONFIG_POWER_CTRL_STRU;

typedef struct
{
    HI_U16  select_assoc_timeout;
    HI_U16  fc_state_timeout;
    HI_U16  beacon_state_timeout;
    HI_U16  assoc_state_timeout;
}DIAG_CMD_CONFIG_MULTI_NETWORK_STRU;

//ͨ�����ϱ�����
typedef struct
{
    HI_U8 report_switch;  // �ϱ�����
    HI_U8 report_period;  // �ϱ�����(report_period�����������ϱ�һ��)
    HI_U8 report_num;
    HI_U8 pad;

    DIAG_CMD_CONFIG_POWER_CTRL_STRU power_ctrl_nv;
}DIAG_CMD_CONFIG_REPORT_COMM_RATE_STRU;

typedef struct
{
    HI_U8  sta_eth_enable;              // STA����ʹ��
    HI_U8  speed_test_enable_flag;      // ʹ�ܹ������ģʽ
    HI_U16 mtu_size;
	HI_U8  packet_flag;
	HI_U8  rsd[3];
}DIAG_CMD_CONFIG_SPEED_TEST_NV;

typedef struct
{
    HI_U32 aging_time_th;               // �˿�ӳ����ϻ�ʱ�䣬��λ:ms
    HI_U32 aging_timer_cycle;           // �˿�ӳ����ϻ���ⶨʱ�����ڳ��ȣ���λ:ms
    HI_U32 expired_time_th;             // ���Ĺ���ʱ�䣬��λ:ms
    HI_U32 expired_timer_cycle;         // ���Ĺ��ڶ�ʱ�����ڳ��ȣ���λ:ms

    HI_U8  broadcast_port_fail;         // ��ȡ�˿�ʧ�ܺ��Ƿ�㲥��������
    HI_U8  packet_lid;                  // �������ȼ�
    HI_U8  use_min_expired_fragment;    // ��Ƭ���治��ʱ���滻����ʱ����̵ķ�Ƭ
    HI_U8  sta_port_communicate;        // �Ƿ�֧��STA�˿ڼ�ͨ��

    HI_U8  capture_switch;              // ץ��MSG����
    HI_U8  ethernet_interface_mode;     // ��̫����ģʽ
    HI_U8  log_switch;                  // LOG��Ϣ��ӡ����
    HI_U8  rsvd;
}DIAG_CMD_CONFIG_PLC_SWITCH_NV;

#define MAX_FILTER_PROTOCOL_NUM     (20)
typedef struct
{
    HI_U8  filter_mode;                                 // Э�����ģʽ
    HI_U8  filter_gratuitous_arp_switch;                // �������ARP���Ŀ���
    HI_U8  rsvd[2];

    HI_U16 protocol_list[MAX_FILTER_PROTOCOL_NUM];      // Э���
}DIAG_CMD_CONFIG_PLC_SWITCH_FILTER_NV;

typedef struct
{
    HI_U16 plc_multi_network_monitor_cycle;			// PLC��·������������ڣ���λ:S
    HI_U16 evaluation_time_len;						// RF����������ʱ������λ:S
    HI_U16 try_assoc_time_len;						// RFģʽ�������ʱ�䣬��λ:S

    HI_U8  plc_blink_check_rounds_th;				// PLCģʽ��ä���ִ�
    HI_U8  plc_low_rate_continuous_cycle_th;		// PLCģʽ�����ͨ���ʳ����ִ�
    HI_U8  plc_low_rate_th;							// PLCģʽ�����ͨ����
    HI_U8  rf_try_join_network_rounds_th;			// RFģʽ�¹����ִ�

    HI_U16 plc_not_join_network_th;                 // PLCģʽ���δ��������ʱ�䣬��λ:S
    HI_U16 rf_not_join_network_th;					// RFģʽ��δ���������ʱ�䣬��λ:S
    HI_U16 network_mode_time_cycle;					// ����ģʽ��ʱ������ڣ���λ:S
    HI_U16 rf_not_rcv_complete_beacon_th;			// RF���δ���յ������ű�֡ʱ�䣬��λ:S

    HI_U8  rf_rssi_th;                              // �ź�ǿ������
    HI_U8  sack_enable;                             // �Ƿ�ظ�SACK
    HI_U8  continue_timeout_end_send;               // SACK������ʱ�Ƿ��������
    HI_U8  rf_link_rssi_th;                         // RF��·��ͨ�ŵ��ź�ǿ������

    HI_U16 rf_comm_link_break_judgment_time_len;    // RFͨ����·�Ͽ��ж�ʱ������λ:S
    HI_U16 link_test_stat_cycle_time_len;           // RF��·ͨ�Ų���ͳ�����ڳ��ȣ���λ:S

    HI_U8  min_rcv_link_test_frame_cnt;             // RF��·ͨ�Ų���ͳ��������·����֡��С���ո���
    HI_U8  rf_proxy_change_up_limit;                 //RF��·����������
    HI_U8  rf_proxy_change_down_limit;               //RF��·����������
    HI_U8  rf_proxy_change_max_offset;               //RF��·��������ƫ��

    HI_U16 proxy_change_check_cycle;                 //RF��·��������ʱ������ڣ���λ:S

    HI_U8 rf_proxy_change_middle_offset;
    HI_U8 rf_proxy_change_min_offset;                //RF��·������Сƫ��
	HI_U8 rf_used_window_num;
    HI_U8 rssi_down_limit;

    HI_U8 rf_window_weighting[4];

    HI_U8 rssi_priority_limit;
    HI_U8 rf_assoc_max_num;
    HI_U8 network_mode_list[4];
    HI_U8 mf_proxy_comm_rate_th;					// ��Ƶ�´���ͨ�����г�����
    HI_U8 mf_low_proxy_comm_rate_contiguous_th;		// ��Ƶ�³�������ͨ�����������޵�ʱ��
}DIAG_CMD_CONFIG_DUAL_MODULE_PARA_NV;

//̨��ʶ�����̿���NV��
typedef struct
{
    HI_U8 tf_calcu_mode;                                // ̨��ʶ����㷽ʽ�����V200оƬ,0:�߼�ʶ��1:���ʶ��
    HI_U8 tf_set_code_type;                             // ����ѡ��,0:�����ƣ�1:ѡ��13bit���֣�2:ѡ��127bit����
    HI_U8 tf_get_ntb_mode;                              // ��ȡNTB��Դ�����V200оƬ,0:AD,1:�ⲿ��·
    HI_U8 tf_networking_protocol;                       // ����Э�飬0:V100Э�飬1:V200Э��

    HI_U8 broadcast_notify_max_cnt;                     // �㲥̨֪ͨ��ʶ������ֹͣ����,Ĭ��20��
    HI_U8 broadcast_notify_period;                      // �㲥̨֪ͨ��ʶ�����ڣ�Ĭ��Ϊ2s����λs
    HI_U8 set_ad_collect_data_mode;                     // V200оƬ����ģʽ���ã�0:1M�����ʣ�1:500K������
    HI_U8 report_line_data;                             // V200оƬ�ϱ�32·�����0:���ϱ���1:�ϱ���Ч��2:�ϱ�����

    HI_U16 bcn_notify_send_timer_period;                //̨��ʶ��֪ͨ�ű�ķ���ʱ��,��λ:s��180
    HI_U16 ndm_chl_req_timeout;                         //NDCͨ������ʱ��ʱ�䣬��λ:ms,6000
    HI_U16 restart_identify_time;                       //�Զ���������̨��ʶ��ļ������λ:s,Ĭ��ֵ:300s
    HI_U16 parallel_query_timeout;                      //���в�ѯ�ĳ�ʱʱ�䣬��λ:ms, 12000

    HI_BOOL is_restart_identify;                        //վ���ѯ��ʱ������̨��ʶ����ƿ��أ�Ĭ�ϴ�HI_TRUE
    HI_U8  rcv_bcn_notify_disable_cnt_min;              //�����������������ʱ��Ҫ�����յ�̨֪ͨ��ʶ��������ű�ĸ���
    HI_U8  change_cco_info_min_time;                    //CCO��Ϣ�仯������ʱ�䣬��λs
    HI_U8  ndm_to_sta_send_num;                         /*��������STAģʽ��̨�����з���������Ĭ��5�֣�ע: ���Ϊ9��*/

    HI_U8  ndm_chl_req_send_cnt_max;                    //������ͨ��������������,50
    HI_U8  query_parallel_send_num;                     //̨��ʶ���ѯ���з��͵�վ������5
    HI_U8  allow_sta_identify_cnt_max;                  //ÿ��վ����������ʶ�����,2
    HI_U8  per_sta_query_cnt_max;                       //ÿ��STA��ѯ��������, 5

    HI_U8  tf_rcv_mode;                                 //̨��ʶ�����ģʽ
    HI_BOOL is_tf_enable;                               //̨��ʶ��ʹ�ܱ�ʶ
    HI_BOOL is_seq_cycle_mode_disable;                  //̨��ʶ���������ڷ���ģʽ����
    HI_U8  is_multi_cco_notify_enable;                  //��CCO֪ͨ����̨��ʶ�𿪹�

    HI_U16 clear_line_purse_diff_cnt;                     // V200оƬ����13bit����ʶ��ǰ��purse_cnt��ֵ������182,��14�����ڣ��������·
    HI_U16 wait_union_jude_time;                           // V200оƬ��13bit����ʶ��ȴ������о�ʱ�䣬��λms��Ĭ��Ϊ5720

    HI_U16 send_ntb_collect_notify_timer_period;        //�ɼ�NTB֪ͨ�ķ�������
    HI_U8  om_ntb_collect_mode;                         //NTB�ɼ�����ģʽ
    HI_U8  send_ntb_collect_num_max;                    //����NTB�ɼ�֪ͨ���ĵ�������

    HI_U8 check_ndm_connect_time_out;                   // CCO���NDM�Ƿ����ӵ�ʱ�䣬��λ:min,1
    HI_BOOL is_report_result_req;
    HI_BOOL is_report_result_rsp;
    HI_BOOL is_collect_ntb_by_bcn_notify;

    HI_U16 send_13bit_seq_delay_interrupt_max_cnt;      // 13bit������λ�л�����ж�����Ϊ400����Ƶ����
    HI_U16 send_127bit_seq_delay_interrupt_max_cnt;    // 127bit������λ�л�����ж�����Ϊ480����Ƶ����
    HI_U8 freq_to_seq_array_index[FREQ_TRY_NUM];        // ̨��ʶ�������Ƶ�ε�ӳ���ϵ
    HI_U8 collect_ntb_phase;                            // ʵʱ�ɼ�NTB����λ��1:A,2:B,3:C
    HI_BOOL is_correction_voltage_enable;               // �Ƿ����ù��㹤װУ����ѹ,Ĭ�ϲ�����
    HI_U8 send_seq_type_cnt_max;                      // �����������͸������ֵ��Ĭ��2
    HI_U8  ndm_tf_mode_check_timer_period;              //��������ֹ����ģʽ�ļ�ⶨʱ������λs��ȱʡֵ120
    HI_U16 send_seq_min_interval;                       //̨��ʶ�����еķ�����С�������λs��Ĭ��ֵ180

    HI_U32 ndm_send_delay_interrupt_cnt_max;            //��������λ�л���ʱ������ж�������λ:20ms,ȱʡֵ500
    HI_U8  sta_check_tf_period_num_max;                 //���STA�Ƿ�֧��̨��ʶ��ļ���ִΣ�ÿ������Ϊ200ms,ȱʡֵ100
    HI_U8  ndm_set_edge_mode;                           //����NDM��������ʱ�������صĿ��ƣ�0:�����ƣ�1:����Ϊ��0��2:����Ϊ��1
    HI_U8  rcv_mode_switch_phase_cnt;                   //����ģʽ�л������ڣ���λ:ÿ����λ�����з���ʱ��
    HI_BOOL is_zero_cross_forever;                      //�����ж��Ƿ���Զ��

    HI_U16 other_tf_noise_th;                           //������̨����������ֵ���ޣ���λus��ȱʡֵ8
    HI_U16 other_tf_signal_high_th;                     //������̨�����źž�ֵ���ޣ���λus��ȱʡֵ15
    HI_U16 other_tf_signal_low_th;                      //������̨�����źž�ֵ���ޣ���λus��ȱʡֵ13
    HI_U16 cur_result_valid_time;                       //��ǰ̨��ʶ������Ч��ʱ�䣬��λs��ȱʡֵ7200�����ֵԼ18��Сʱ
    HI_U32 his_result_valid_time;                       //��ʷ̨��ʶ��������Чʱ�䣬��λs��ȱʡֵ172800(2��)

    HI_U16 abnormal_noise_th;                          //�쳣���������ޣ���λus��ȱʡֵ20us
    HI_U16 collect_noise_th;                            // �ɼ�������������Ϊcollect_noise_th��2.86sʱ�������Ĭ��ֵΪ629����ʾ30���ӡ�

    HI_U8 clear_collect_noise_time;                     // �������ͳ��ʱ�䣬Ĭ��Ϊ18s
    HI_BOOL is_real_time_calc_noise_enable;            // �Ƿ�ʵʱ��������ֵ��Ĭ��Ϊ1��ʵʱ����
    HI_BOOL send_seq_type_enable;                       // ������˿��Ʒ�������ʹ�ܣ�Ĭ��HI_FALSE����ʹ��
    HI_U8 collect_noise_mean_th;                       // �ɼ���������,Ĭ��Ϊ20us

    HI_BOOL report_ntb;                               // V200оƬʶ���ϱ�ÿ�ν��NTB,power_cnt��Ĭ���ϱ�
    HI_BOOL report_noise;                             // V200оƬʶ���ϱ�����ͳ��
    HI_BOOL is_test_have_v100_sta;                 // �����д���V100оƬSTA  ����׮����
    HI_U8 enable_test_ndm_rx;                         // ����״̬�£����ó��ر��������ر�ģʽ

    HI_U16 send_127bit_inter_seq_delay_interrupt_max_cnt;  // 127bit����Ŷ�������λ�����4.6s,Ĭ��230����λ��Ƶ����(20ms)
    HI_U8 precise_zero_corss_judge_th;                // �жϾ�׼�������������ֵ��Ĭ��10����λms
    HI_BOOL disable_poweredge_type;                   // ������Ƿ����������жϱ��أ�Ĭ��0����������

    HI_U8 send_seq_type0;                             // �����ʹ�ܿ�������£����͵�1���������ͣ�Ĭ��0
    HI_U8 send_seq_type1;                             // �����ʹ�ܿ�������£����͵�2���������ͣ�Ĭ��1
    HI_U8 send_seq_type0_turn_cnt;                   // ��������0���ִ�,Ĭ��55
    HI_U8 send_seq_type1_turn_cnt;                   // ��������1���ִ�,Ĭ��5

    HI_U16 send_seq_type0_delay_max_cnt;             // ��������0����λ֮������Ĭ��400
    HI_U16 send_seq_type1_delay_max_cnt;             // ��������1����λ֮������Ĭ��480
}DIAG_CMD_CFG_TF_NV_STRU;

//̨��ʶ�������NV��
typedef struct _DIAG_CMD_CONFIG_ZERO_CROSS_NV_
{
    HI_U16 noise_limit[7];                      /*[TF_ZERO_CROSS_NOISE_LIMIT_CNT],��������,��λ��40ns,����Ӧ�ɴ���С����*/
    HI_U8  false_alarm[7];                      /*[TF_ZERO_CROSS_NOISE_LIMIT_CNT],�����龯����,ȡֵ��Χ1��100����������Ϊ1������ƽ��ֵ��0.01Ϊ���ޣ�����Ϊ10������ƽ��ֵ��0.1Ϊ����*/
    HI_BOOL is_jammer;                          //����������ڸ��ŷ���ģʽ
    HI_BOOL ndm_rx_gear_switch;                 /*�������Գ�����ģʽ�£����ն˽������е�λ���أ�0:�Զ���λ��1:�ֶ��л���λ��Ĭ��Ϊ0*/
    HI_U8  ndm_rx_gear;                         /*�������Գ�����ģʽ�£����ն˽������еĵ�λ��0:Ϊ240K�Ľ��յ�λ��1:Ϊ1M�Ľ��յ�λ��Ĭ��Ϊ1*/

    HI_U16 jammer_seq_a;                        //���������͵�����A
    HI_U8  novel_mode;                          //�Ƿ�ʹ�������з���ʱ���Ŷ�ͳ�ƹ��ܣ�Ĭ��ΪHI_FALSE
    HI_U8  jammer_a_send_cnt;                   //����������A���͵Ĵ���

    HI_U8  jammer_b_send_cnt;                   //����������B���͵Ĵ���
    HI_U8  ndm_to_ndm_send_num;                 /*�������Գ�����ģʽ��̨�����з�������,Ĭ��4�֣�ע: ���Ϊ7��*/
    HI_U8  pad;
    HI_U8  novel_noise_limit_index;             // 4,���ڴ�ֵ

    HI_U8  tf_serial_num;                       /*ѡ��Ҫ�������е�̨�����*/
    HI_U8  noise_limit_mode;                    //��������ģʽ������0Ϊ1��������0(40us)ʹ�ã�����6Ϊ1��������6(4us)ʹ�ã�ȱʡֵ0x48
    HI_U16 err_value_threshold;                 /*�쳣ֵ�ж�����*/

    HI_U16 jammer_seq_b;                        //���������͵�����B
    HI_U16 novel_noise_mean_max_th;             //566us

    HI_U8  jammer_c_send_cnt;                   //����������C���͵Ĵ���
    HI_BOOL report_receive_ntb_switch;          /*�ϱ�̨��ʶ�����NTB��������Ϣ����*/
    HI_BOOL report_detect_sequence_switch;      /*�ϱ�̨��ʶ�������п���*/
    HI_BOOL tf_enable_pos_opp_switch;           /*��������λ�����������л�*/

    HI_U8  tf_switch_phase_mode;                /*������ĳ����λ��������,0:ȫ����λ��1:A��λ��2:B��λ��3:C��λ*/
    HI_BOOL tf_single_phase_tx;                 /*��ĳһ��λ�������ͱ�־*/
    HI_BOOL tf_enable_phase_ctrl;               /*�������������λ�л����أ�0:��������1:����*/
    HI_BOOL is_random_jammer;                   //�����������

    HI_U16 v200_soft_th;                          // Ĭ��Ϊ73
    HI_U8  tx_period_cnt_max;                   //ndm-sta������������е�����������λ:�������������һ������з���ʱ�� Ĭ��Ϊ3
    HI_U8  ndm_send_period_cnt;                 //ndm-ndm���������� Ĭ��Ϊ1

    HI_U8  sign3_synch_suc_th;                  // ��ͬ������3������ײʶ��ɹ��������ޣ�Ĭ��Ϊ22
    HI_U8  big_err_follow_cnt_max_percent;                              //ͳ�Ƹ����쳣��������������ٷֱȣ�Ĭ��Ϊ25%
    HI_U8  novel_signal_mean_th;                //40us
    HI_U8  novel_noise_mean_th;                 //40us

    HI_U8  ndm_ntb_report_time_switch;          //Ĭ��Ϊ0 : ÿ���ϱ�һ�Σ�1 : ֻ���ж�ʶ��ɹ�ʱ�ϱ�; ͬʱ��������Ϣ�ϱ�
    HI_U8  big_err_diff_cnt_max_percent;                                //ͳ�������쳣��������������ٷֱȣ�Ĭ��Ϊ25%
    HI_U8  noise_statistic_max_percent;                                //ͳ�����������ٷֱȣ�Ĭ��Ϊ25%
    HI_BOOL is_between_window_ntb_filter_enable;//���ntb�쳣�����أ�Ĭ��Ϊ1:�򿪣�0:�رգ�

    HI_U16 identify_disable_time_len;           // STĄ��ʶ��ر�ʱ������,��λ:s,90
    HI_U16 big_err_diff_th;                     //�쳣���������ޣ�Ĭ��55v�Ŷ���ѹΪ566us��50v�Ŷ���ѹΪ514us

    HI_BOOL is_in_window_ntb_filter_enable;     //�����ڵ�NTB���˿��أ�Ĭ��Ϊ1:�򿪣�0:�رգ�
    HI_U8 ntb_thblock;                          //Ĭ��50ms  ��غ궨��TF_ZERO_CROSS_LOST_PERIOD_CNT
    HI_U8 ntb_thperiod;                         //Ĭ��15ms  ��غ궨��TF_ZERO_CROSS_LOST_PERIOD_CNT
    HI_U8 ntb_thms;                             //Ĭ��1ms

    HI_U8 ntb_thus;                             //Ĭ��30us
    HI_U8 sta_first_window_soft_th;             //Ĭ���� v100_soft_th ��ֵ��ȣ�
    HI_U16 v100_soft_th;                 // Ĭ��72

    HI_U16 novel_signal_mean_max_th;            //Ĭ��566,��λus
    HI_U16 sta_weight_th;                       //Ĭ��216

    HI_U16 sta_rough_weight_th;                 // ��ͬ�������о���ֵ���ޣ�Ĭ��Ϊ144us
    HI_U16 sta_exact_weight_th;                 // ��ͬ�������о���ֵ���ޣ�Ĭ��Ϊ144us

    HI_U16 static_wobble_limit[6];              //[TF_NOISE_TH_CNT],����Ӧ�ɴ���С����

    HI_U16 ndm_report_ntb_rcv_level_switch_time;//ndm�ɼ�NTB�ϱ���λ�л���ʱ��ʱ��, ��λs��Ĭ��160s
    HI_BOOL is_three_phase_together_enable;     // ������ϵ��Լ��Ƿ�A,B,C�������һ���ж�Ϊ���ϣ�Ĭ��Ϊ0:���ϣ�����������̨��ʶ��
    HI_U8 ndm_real_noise_avg_cnt;               // �����ʵʱˢ�µ������Ŷ�ֵ�ĸ�����Ĭ��Ϊ12��

    HI_U8 ndm_crosstalk_min_th;                 // ���ջ����δ�����С���ޣ�Ĭ��13����λus
    HI_U8 ndm_crosstalk_max_th;                 // ���ջ����δ���������ޣ�Ĭ��40����λus
    HI_U16 jammer_seq_c;                        //���������͵�����C

    HI_U8 expect_freq1;                         //Ԥ�ڵ�����Ƶ��1
    HI_U8 expect_snid1;
    HI_U8 expect_freq2;
    HI_U8 expect_snid2;
}DIAG_CMD_CONFIG_ZERO_CROSS_NV_S;


//��λʶ��NV��
typedef struct
{
    HI_BOOL is_pi_enable;                           //��λʶ��ʹ�ܿ��أ�HI_TRUE
    HI_U8  ntb_query_parallel_num;                  //����NTB��ѯ�Ĳ���վ������3
    HI_U8  ntb_query_turn_cnt_max;                  //����NTB��ѯ������ִ�,5
    HI_U8  identify_fail_cnt_max;                   //��λʶ����������ʧ�ܴ���,5

    HI_U16 parallel_query_timeout;                  //���в�ѯ��ʱʱ�䣬��λ:ms,12000
    HI_U16 check_over_timeout;                      //ȷ����λʶ������Ķ�ʱ������λ:s��120

    HI_U8  phase_cnt_th_min;                        //��λʶ��ͳ�Ƶ���С��ֵ��15
    HI_U8  phase_cnt_other_th_min;                  //���������ʱ���������С��ֵ��10
    HI_U8  phase_cnt_other_th;                      //���������ʱ�����������ֵ��2
    HI_U8  phase_cnt_no_other_th_min;               //������Ϊ0ʱ���������С��ֵ��6

    HI_U8  two_phase_cnt_th_min;                    //������λʶ��ͳ�Ƶ���С��ֵ��6
    HI_U8  two_phase_cnt_other_th_min;              //������λʶ������£����������ʱ���������С��ֵ��4
    HI_U8  two_phase_cnt_other_th;                  //������λʶ������£����������ʱ�����������ֵ��2
    HI_U8  two_phase_cnt_no_other_th_min;           //������λʶ������£�������Ϊ0ʱ���������С��ֵ��2

    HI_U8  three_phase_cnt_th_min;                  //������λʶ��ͳ�Ƶ���С��ֵ��4
    HI_U8  three_phase_cnt_other_th_min;            //������λʶ������£����������ʱ���������С��ֵ��3
    HI_U8  three_phase_cnt_other_th;                //������λʶ������£����������ʱ�����������ֵ��1
    HI_U8  three_phase_cnt_no_other_th_min;         //������λʶ������£�������Ϊ0ʱ���������С��ֵ��2

    HI_BOOL is_report_zero_cross_ntb_req;           //�ϱ������ѯ���ĵĿ��أ�HI_TRUE
    HI_BOOL is_report_zero_cross_ntb_rsp;           //�ϱ�����Ӧ���ĵĿ��أ�HI_TRUE
    HI_BOOL is_report_zero_cross_ntb_cco;           //�ϱ�CCO�Ļ�׼����NTB
    HI_BOOL is_slow_query_enable;                   //���ٲ�ѯģʽ

    HI_S32 sta_zero_cross_ntb_offset;               //STA����NTB�����CCO��ƫ��������λ:1/25MHz
    HI_U16 ntb_report_timer_period;                 //CCO�ϱ������NTB�����ڣ���λs
    HI_U16 cco_period_th;                           //CCO�����ж����ޣ���λms

    HI_U8 insert_th;  //�쳣����������ɸѡ����,Ĭ��ֵΪ5����λ:ms
    HI_U8 miss_th;    //�쳣����������ɸѡ���ޣ�Ĭ��ֵΪ15����λ:ms
    HI_BOOL is_min_flag;  //�Ƿ�ʹ���ϸ�����
    HI_U8 period_th;    //���ڸ������ޣ�Ĭ��ֵΪ9

    HI_U8 line_error_th;//����߽�����ȷ��������ڸ���������ڸ�������,Ĭ��ֵΪ8
    HI_BOOL is_check_mode;     // �Ƿ�Ϊ�ͼ�ģʽ,HI_TRUE��ʾΪ�ͼ�ģʽ��HI_FALSE��ʾ����ģʽ
    HI_U8 two_phase_period_th;  // ������λ����£����ڸ������ޣ�Ĭ��ֵΪ6
    HI_U8 two_phase_line_error_th; // ������λ����£���������ȷ��������>���������޸�����Ĭ��ֵΪ5

    HI_U8 three_phase_period_th;   // ������λ����£����ڸ������ޣ�Ĭ��ֵΪ4
    HI_U8 three_phase_line_error_th; //������λ����£���������ȷ��������>���������޸�����Ĭ��ֵΪ3

    HI_U8 broadcast_send_max_cnt;                  // CCO�㲥֪ͨ������,Ĭ��3��
    HI_U8 broadcast_send_durtion;                  // CCO�㲥֪ͨʱ������Ĭ��6s����λs��

    HI_U8 period_check_min_cnt;                    // ���ڼ����С������24�Σ����Ϊ2Сʱ
    HI_U8 period_check_max_cnt;                    // ���ڼ����������48�Σ����Ϊ4Сʱ
    HI_U8 pad[2];
}DIAG_CMD_CONFIG_PI_NV_S;

// ���϶�λNV����Ʋ���
typedef struct
{
    HI_U8 fl_enable_switch;                    //�������϶�λ����:Ĭ�Ͽ���
    HI_U8 pad;
    HI_U8 cco_mac[HI_PLC_MAC_ADDR_LEN];   //Ҫ��λ�����CCO��MAC

    HI_U8 freq;                                                   //Ҫ��λ�����freq
    HI_U8 snid;                                                   //Ҫ��λ�����snid
    HI_U8 fl_refresh_topo_period;                    //TOPOˢ�����ڣ���λ:min
    HI_U8 continus_not_rcv_topo_max_cnt;    //����ˢ������ʧ������
}DIAG_CMD_CFG_FL_NV_STRU;

typedef struct
{
    HI_BOOL mac_ctrl_enable;                  // mac����ռ�ձȲ��Կ��أ�����ʹ��
    HI_BOOL duty_enable;                      // ռ�ձȲ���ʹ�ܿ���
    HI_U8 duty_percent;                       // ռ�ձȴ�С
    HI_BOOL is_vs_enable;                     //�Ƿ����õ�ѹ���书��

    HI_U32 max_sendlength;                    // ��������ͱ���ʱ������λms

    HI_BOOL period1_duty_percent_enable;      // ʹ������1��ռ�ձ�ͳ��
    HI_BOOL period2_duty_percent_enable;      //ʹ������2��ռ�ձ�ͳ��
    HI_BOOL period3_duty_percent_enable;      //ʹ������3��ռ�ձ�ͳ��
    HI_U8 con_high_voltage_max_cnt;           //�������ߵ�ƽ������

    HI_U32 duty_period1;                  // ����1���ȣ���λms
    HI_U32 duty_period2;                  // ����2���ȣ���λms
    HI_U32 duty_period3;                  // ����3���ȣ���λms

    HI_U8 check_zero_cross_int_period;        // ��ѹ����������жϸ�����Ĭ��50ms
    HI_U8 check_voltage_level_period;         // ��ѹ��������ѹ�ָ�ʱ�䣬Ĭ��10ms
    HI_U8 check_tx_power_period;              // ��ѹ�����������ʣ��������ʱ�䣬Ĭ��30����,��λ����
    HI_U8 check_zero_cross_min_cnt;           // ��ѹ����󣬼����չ����жϸ�����Ĭ��2��

    HI_U8 check_high_voltage_max_cnt;         // ���ߵ�ƽ��������Ĭ��10�Σ�100ms
    HI_U8 low_tx_power;                       // ��С�ķ��͹��ʣ�Ĭ��2����4dB
    HI_U8 adjust_tx_power_step;               // ���͹��ʵ���������Ĭ��2����4dB
    HI_U8 adjust_tx_power_check_min_cnt;      // �������͹��ʺ��������û�г��ֵ�ѹ�������,Ĭ��20��
}DIAG_CMD_CONFIG_MAC_DUTY_CTRL_PARA_STRU;

//̨��ʶ���Ų���NV
typedef struct
{
    HI_U8  lock_network_switch;     // �������翪��
    HI_U8  pad;
    HI_U16 normal_lock_duration;    // ��ͨ����ʱ������λmin

    HI_U32 lock_freqsnid_duration;  // ����Ƶ�κ������ʱ������λS
    HI_U32 lock_ccomac_duration;    // ����CCO MAC��ַʱ������λS

    HI_U16 save_nv_lock_len_th;     // ����ʱ����������ʱ���Ž�����MAC���µ�NV����λmin
    HI_U16 leave_time_count_period; // ��������������ʱ��ͳ�����ڣ���λmin

    HI_U32 assoc_belong_lock_duration;  // ������������ʱ������ʱ������λs
    HI_U32 join_belong_lock_duration;   // �����������ʱ������ʱ������λs

    HI_U32 rst_max_delay_time;          // ��ͨ�����£�������λ����ӳ�ʱ�䣬������ʱ�䲻��delay

    HI_U16 safe_lock_duration;          // ��ȫ����ʱ������λmin
    HI_U16 pad1;
}DIAG_CMD_CONFIG_TF_CROSSTALK_STA_PARA_STRU;

//USTA��NV����
#define MAX_SWEEP_FREQERR_NUM           (5)
typedef struct
{
    HI_S16 sweep_step_freqerr[MAX_SWEEP_FREQERR_NUM];           // ɨƵ��Ƶ��
    HI_U16 send_search_frame_interval;                          // ��������֡�������λ:����

    HI_U8  sweep_freq_num;                                      // ɨƵʱƵ��ĸ���
    HI_U8  max_send_search_frame_num;                           // ��������֡������:ÿ��Ƶ���Ϸ�������֡�ĸ���
    HI_U8  max_search_num;                                      // �����������:����Ƶ�����������ִ�
    HI_U8  max_send_synch_frame_num;                            // ����ͬ��֡��������

    HI_U16 passive_synch_timeout;                               // ����ͬ����ʱʱ�䣬��λ:����
    HI_U16 send_synch_interval_not_synch;                       // δͬ��״̬�·���ͬ��֡�ļ������λ:����
    HI_U16 send_synch_interval_in_synch;                        // ��ͬ��״̬�·���ͬ��֡�ļ������λ:����
    HI_U16 data_transmission_timeout;                           // ���ݴ��䳬ʱ����λ:��
    HI_U16 listen_timeout;                                      // ������ʱʱ�䣬��λ:��
    HI_U16 heartbeat_cycle;                                     // �������ڣ���λ:���룬��Ҫ�������ݴ��䡢��������������״̬��ʱ������

    HI_U8  max_app_packet_send_fail_num;                        // Ӧ�ò㱨�������ʧ�ܴ���
    HI_BOOL is_loopback_enable;                                 // ���ز���ģʽʹ�ܱ�ʶ
    HI_U8  pad[2];
}DIAG_CMD_CONFIG_PTPC_STRU;

typedef struct
{
    HI_U16  nbi_start;
    HI_U16  nbi_end;
}DIAG_CMD_CONFIG_AVOID_FREQ_ENTRY;

typedef struct
{
    HI_BOOL emc_switch;                         // EMC��֤����
    HI_U8   report_static_cyc;
    HI_U8   avoid_freq_num;                     // ��̬���õ�Ƶ�θ���
    HI_U8   avoid_freq_mask_extend;             // ��̬Ƶ�ʱ����������ز�����

    HI_U16  freq_avoid_extend_time;                    //Ƶ�ʸ�����ʧ����Ҫ��չ����ʱ������λ:S
    HI_U16  power_control_pk_cycle;             // ���ʿ��Ʊ������ڣ���λ:S

    HI_U16  emc_periodic_detect_cycle;          // EMC��ʱ������ڣ���λ:MS
    HI_U8   send_fail_th_power_recovery;        // ���ʻָ��б��ķ���ʧ������
    HI_U8   power_adjust_step;                  // ���ʵ�������

    HI_U8   max_channel_dec_thresh;             // �ŵ�˥�����ֵ
    HI_U8   min_channel_dec_thresh;             // �ŵ�˥����Сֵ
    HI_U8   white_noise_detect_th;              // �����������ж�����
    HI_U8   min_channel_dec_thresh_15;       // Ƶ��15ʱ��̬���ʿ���ģʽ��ά�ֶ�̬ƽ����ŵ�˥������: 47

    HI_U8   max_channel_dec_thresh_15;      // Ƶ��15ʱ��̬���ʿ���ģʽ��ά�ֶ�̬ƽ����ŵ�˥������: 52
    HI_U8   power_control_nv_switch;            // �Ƿ����ù��ʿ��ƿ���
    HI_U8   send_fail_recovery_power_switch;    // ����ʧ�ܴ������ʿ��ƿ���
    HI_U8   NoisePowTarget; //����Ŀ�깦��

    HI_U8   powerloading_merge_th;              // ���ۺϲ�����
    HI_U8   lab_nbi_judge_th;                   //NBIʵ���ҳ�������ȷ���жϵĴ�������
    HI_U8   lab_nbi_max;                          //�ж�Ϊʵ���ҳ�����NBI����
    HI_U8   min_channel_dec_thresh_16;      // Ƶ��16ʱ��̬���ʿ���ģʽ��ά�ֶ�̬ƽ����ŵ�˥������: 53


    HI_U8   max_channel_dec_thresh_16;    // Ƶ��16ʱ��̬���ʿ���ģʽ��ά�ֶ�̬ƽ����ŵ�˥������: 58
    HI_U8   default_afe_power;                   //emcģʽ��Ĭ�ϵ�afe����:8
    HI_U8   pad2;
    HI_U8   pad3;

    HI_U32  test_pk_cycle;
    HI_U16  test_pk_size;
    HI_U8   test_pk_num;                        // ÿ�뷢�Ͳ��Ա��ĸ���
    HI_U8   avoid_detect_th;                //��̬Ƶ�ʱ��ü���������

    DIAG_CMD_CONFIG_AVOID_FREQ_ENTRY freq[8];
}DIAG_CMD_CONFIG_EMC_PARA_STRU;

typedef struct _DIAG_CMD_CONFIG_MAC_RSVD3_
{
    HI_U32 rsvd[1];
}DIAG_CMD_CONFIG_MAC_RSVD3;

typedef struct _DIAG_CMD_CONFIG_MAC_RSVD4_
{
    HI_U32 rsvd[1];
}DIAG_CMD_CONFIG_MAC_RSVD4;

typedef struct
{

    HI_U8 level_cnt_max[4];         // ���㼶ͬ���������
    HI_U8 snr_diff_th;              // ����Ȳ�������
    HI_U8 rcv_bcn_cnt_diff_th;      // �����ű�֡������������
    HI_U8 pad;                      //�������Ƶƫ�Ƿ�Ҫ�洢��������Ƶƫ����
    HI_U8 ntb_policy_enable;

    HI_U32 stat_cycle_interval;         // NTB ͬ������
}DIAG_CMD_SEL_SYNC_STA_POLICY_NV_STRU;

#ifdef PRODUCT_CFG_ONLINE_TEST_MODE
#define TMI_NUM             (16)
#define SEND_FREQ_NUM       (6)
#define SEND_TMI_NUM        (4)
#define SEND_EXT_TMI_NUM    (4)
#define INVALID_TMI         (0xff)
#define INVALID_FREQ_MODE   (0Xff)

// 0x242 TMI ��Ϣ��
typedef struct
{
    HI_U8   tmi;
    HI_U8   pb_num;
    HI_U16  pb_size;
}DIAG_CMD_GT_TEST_TMI_INFO_ENTRY_ST;

typedef struct
{
    DIAG_CMD_GT_TEST_TMI_INFO_ENTRY_ST tmi_info[TMI_NUM];
    DIAG_CMD_GT_TEST_TMI_INFO_ENTRY_ST ext_tmi_info[TMI_NUM];
}DIAG_CMD_GT_TEST_TMI_INFO_STRU;

// 0x243 ��������ʹ��TMI
typedef struct
{
    HI_U8 freq_mode;
    HI_BOOL is_robust_mode;
    HI_U8 pad[2];

    HI_U8 tmi[SEND_TMI_NUM];         //TMI 1 6 9
    HI_U8 ext_tmi[SEND_EXT_TMI_NUM]; //EXTTMI 11 0 15
}DIAG_CMD_GT_TEST_SEND_SOF_POLICY_ST;

typedef struct
{
    HI_U8   tmi;
    HI_U8   freq_mode;
    HI_BOOL is_robust_mode;
    HI_U8   pad;
}DIAG_CMD_GT_TEST_SEND_BCN_POLICY_ST;

typedef struct
{
    HI_U32 eif;

    HI_U32 send_interval_sof;    // ����֡��������
    HI_U32 send_interval_bcn;    // GT�ű귢������
    HI_U16 local_tei;
    HI_U16 is_proxy_mode;

    HI_U8 mac[HI_PLC_MAC_ADDR_LEN];
    HI_U8 snid;
    HI_BOOL enable_ol_test;

    HI_U8 test_freq_mode_each_channel[7];
    HI_U8 pad;

    DIAG_CMD_GT_TEST_SEND_BCN_POLICY_ST bcn_send_policy;
    DIAG_CMD_GT_TEST_SEND_SOF_POLICY_ST sof_send_policy[SEND_FREQ_NUM];
}DIAG_CMD_GT_TEST_PARAM_STRU;

//30,0,0,160,120,0,50000000,4000000000
typedef struct
{
    HI_U16 stat_interval;               // �󴰿�ͳ������
    HI_U16 proxy_beacon_cnt_thresh;
    HI_U16 sta_beacon_cnt_thresh;
    HI_U16 check_fc_crc_err_interval;    //�����೤ʱ��һֱû�н��fc��˵��Ƶƫ�Ѿ����ԣ���Ҫ��λNTB
    HI_U16 freq_err_preamble_th;
    HI_U16 freq_err_bcn_th;                  //NTB��Ƶƫ�����ڲ���׶���Ч������{1342,}
    HI_U32 min_beacon_period;            //��С�ű���,��λ:bts,Ĭ��Ϊ2 * 25(Mhz) * 1000 * 1000
    HI_U32 max_beacon_period;            //����ű���,��λ:bts,Ĭ��Ϊ160 * 25(Mhz) * 1000 * 1000
}DIAG_CMD_GT_NTB_NV_STRU;
#endif

// ����ʶ��NV����
#define TFP_LIMIT_SNID_NUM          6
#define TF_NTB_ADAPT_PARA_NUM       8
typedef struct
{
    HI_BOOL is_tf_passive_enable;                // �Ƿ���������ʶ��
    HI_U8 period_check_durtion;                  // CCO,STA���ڼ�鶨ʱ�����ڣ�Ĭ��10min,��λmin
    HI_U8 check_multi_net_period;                // ��ѯ��Χ�Ƿ���ڶ��������ڣ�Ĭ��120����λmin
    HI_U8 check_multi_max_cnt;                   // ��ѯ��Χ�Ƿ���ڶ��������ڸ�����Ĭ��Ϊ24�Σ�2��

    HI_U16 identify_timeout;                     // ʶ��ʱʱ�䣬24Сʱ����λmin,
    HI_U8 check_sta_join_period_cnt;             // CCO�����ڼ��STA������������ڸ�����Ĭ��12����120min��
    HI_U8 check_con_collect_cco_ntb_cnt;         // CCO�˼�������յ�CCO_NTB�ĸ�����Ĭ��5�Ρ�

    HI_U32 next_turn_interval : 12;              // ������һ·�����Ĭ��1Сʱ,��λmin, 60
    HI_U32 cco_notify_start_max_cnt :  6;        // cco�㲥����֪ͨ������Ĭ��Ϊ6��,
    HI_U32 cco_notify_period : 6;                // cco�㲥����֪ͨ�����Ĭ��5s,��λs,
    HI_U32 ntb_jitter_th : 8;                    // �˳�ë�̵ľ���ֵ���޴�С��Ĭ��Ϊ150

    HI_U8 notify_neighbor_start_max_cnt;         // �㲥֪ͨ��̨��������Ĭ��Ϊ10��
    HI_U8 notify_neighbor_period;                // �㲥֪ͨ��̨�������Ĭ��5s����λs
    HI_U8 sta_notify_start_max_cnt;              // sta�յ���̨������֪ͨ������֪ͨcco,Ĭ��2��
    HI_U8 sta_notify_period;                     // sta�յ���̨������֪ͨ�����Ĭ��2s,��λs

    HI_U8 notify_start_durtion;                  // cco֪ͨ�����ɼ�ʱ�䣬Ĭ��80s,��λs
    HI_U8 cco_start_query_sta_durtion;           //�������ɼ����6min��cco��ʼ��ѯȫ��sta��Ϣ,Ĭ��6����λmin
    HI_U8 query_parallel_send_num;               // ������ѯ����
    HI_U8 per_sta_query_cnt_max;

    HI_U8 period_check_bcn_durtion;              // ���ڼ���ű�ʱ�䣬Ĭ��10min,��λmin
    HI_U8 ntb_thms;                              // 1ms,
    HI_U8 ntb_thus;                              // 30us
    HI_U8 delay_lock_durtion;                    // sta�ӳ�����ʱ�䣬Ĭ��3���ӣ�����min

    HI_U16 parallel_query_timeout;               // ������ʱ��ѯʱ�䣬12000ms����λms
    HI_U8 set_tfp_mode;                          // ����ʹ�ã��ֶ�������ֹͣ����ʶ��.0:�����ã�1:������2:ֹͣ,3:����HSO���������������ʶ��
    HI_U8 identify_limit_mode;                   // ����ʶ��ģʽ��0:��ʾ�κ��������ܹ�����1:ֻ����staվ�㴥��

    HI_U16 synch_ntb_cnt;                        // һ������ͬ����ntb������Ĭ��10000��
    HI_U16 synch_ntb_min_cnt;                    // һ�����ڼ��������ͬ����ntb����,5000��

    HI_U16 ntb_synch_th;                         // ntb֮������ͬ��������,Ĭ��1.2ms����λus
    HI_U16 delay_rst_durtion;                    // �ӳ�����ʱ��,2880min,2�죬��λmin

    HI_U16 delay_start_durtion;                  // �ӳ���������ʶ��ʱ�䣬��λmin,Ĭ��0
    HI_U8 delay_collect_ntb_time_by_nsr;         // �ӳٲɼ���һ��ntbʱ�䣬��λmin,Ĭ��30
    HI_U8 delay_collect_ntb_time_by_calc_err;    // ���ڼ�����������²ɼ�NTBʱ�䣬��λmin,Ĭ��2

    HI_U32 report_identify_result_to_hso_enable : 1; // �����ϱ�ʶ������HSO
    HI_U32 report_var_to_hso_enable : 1;             // �����ϱ����������̵�HSO
    HI_U32 report_ntb_to_hso_enable : 1;             // �����ϱ�ntb���ݵ�HSO
    HI_U32 report_cmd_to_hso_enable : 1;             // �����ϱ��������HSO
    HI_U32 report_ntb_to_hso_message_enable : 1;     // �����ϱ�ntb���ݵ�hso��message����
    HI_U32 is_allow_multi : 1;                       // �Ƿ�������ж����������0:������1:����,Ĭ��Ϊ1
    HI_U32 is_allow_other_sta_join : 1;              // �Ƿ���������վ�����
    HI_U32 is_snid_limit : 1;                        // �Ƿ�����ֻ�����������籨��    ,Ĭ��1������
    HI_U32 is_check_support_tfp : 1;                 // �ϵ����Ƿ�֧������ʶ��
    HI_U32 is_single_phase_mode : 1;                 // ����ģʽ���Ƿ�Ϊ����λģʽ
    HI_U32 is_period_check_enable : 1;               // CCO���Ƿ��������ڼ�����
    HI_U32 cco_send_compress_ntb_durtion : 8;         // CCO���ⷢ��ѹ��NTB���ڣ�Ĭ��8����λs
    HI_U32 con_not_rcv_rsp_max_cnt : 8;               // CCO���ж�����10��û���յ��ھ�����CCO_NTB�Ļ�Ӧ���ģ������վ�㷢��������
    HI_U32 con_query_fail_cnt : 5;                   // ������ѯSTAվ��ʧ�ܴ�����Ĭ��3

    HI_U32 nsr_th : 14;                             // ���������޵�ƽ����Ĭ��Ϊ25
    HI_U32 diff_th1 : 5;                            // ntb�쳣�˳�����1��Ĭ��4
    HI_U32 diff_th2 : 5;                            // ntb�쳣�˳�����2��Ĭ��8
    HI_U32 noise_tf_coef : 8;                       // ��������ƫ��ʱ���ж�STA,CCO�Ŷ���ֵ�Ƿ�����ϵ��,100%,Ĭ��16����0.16

    HI_U32 big_zc_diff_groupnum : 5;             // ��ƫ������ж� ����жϼ��������Ĭ��10
    HI_U32 big_zc_diff_valid_groupnum : 4;       // ��ƫ������ж���Ч������Ĭ��2
    HI_U32 big_zc_diff_th1 : 12;                 // STA��CCO��ntb_diff��ֵ��Ĭ��500us
    HI_U32 big_zc_diff_th2 : 11;                 // STA,CCO����ƽ���Ŷ�ֵ��Ĭ��400us

    HI_U32 ntb_noise_th : 5;                     // STA_CCO��STD��CCO��ƽ���Ŷ���ֵ�Ĳ�ֵ�������ж�STA�Ƿ�Ϊ��������Ĭ��5,��λus
    HI_U32 ntb_stage_th : 5;                     // �쳣NTB�˳��ط����ж��Ƿ�Ϊ̨�����������,Ĭ��4,��λus
    HI_U32 valid_ntb_cnt_th : 8;                 // �˳�NTB��ʣ��ntb��Ч������Ĭ��67��
    HI_U32 stage_ntb_diff_th : 6;                // �ж��Ƿ�Ϊ̨�����䣬����ǰ��ntb_diffʹ�ø�����Ĭ��10
    HI_U32 filter_err_ntb_th : 6;                // ��ë�̴������˳�ntb_diff������Ĭ��5
    HI_U32 is_enable_max_noise : 1;                // �Ƿ�����nsr���ܣ�Ĭ��1
    HI_U32 pad0 : 1;

    HI_U32 snid_limit_rcv[TFP_LIMIT_SNID_NUM];   // ��������ֻ�������õ����籨��

    HI_U16 req_result_min_interval;             // ����ʶ������С�������λmin
    HI_U16 is_all_sta_transmit      : 1;        // �Ƿ�����STA��ת��NTB����
    HI_U16 cco_collect_ntb_test     : 1;        // CCO�˲ɼ�NTB����
    HI_U16 is_single_phase_send     : 1;        // �Ƿ��෢��: һ֡��������ֻ��һ����λ������
    HI_U16 is_send_fall_edge        : 1;        // �Ƿ����½���NTB
    HI_U16 no_sta_join_time_th      : 8;        // �ϵ�೤ʱ����վ����룬������NTB����λmin
    HI_U16 is_send_ntb_enable       : 1;        // �Ƿ���NTB����
    HI_U16 rise_fall_ratio          : 3;        // ���������½��صı���

    HI_U8  send_period_default;                 // �߾��ȹ���NTBĬ�Ϸ������ڣ���λmin
    HI_U8  send_period_slow;                    // �߾��ȹ���NTB���ٷ������ڣ���λmin
    HI_U8  send_interval;                       // ���ķ��ͼ������λs
    HI_U8  send_round_max;                      // ���������

    HI_U8  match_succ_cnt_th;                   // �쳣�����н����Ҫ����İ������ޣ��޸�Ϊ5
    HI_U8  send_ntb_diff_num;                   // ���͹���NTB��ֵ����
    HI_U16 result_net_active_time_th    : 10;   // ���ս����Ծʱ�����ޣ���λmin
    HI_U16 miss_ntb_pkt_num_th          : 6;    // ƥ��ʧ�ܶ��ٴΣ�����Ϊδ�յ�������NTB

    HI_U8  rcv_ntb_duration;                    // ��ʱ�����յ���NTB���ݣ�������ʶ�𣬵�λmin
    HI_U8  lock_duration;                       // �������������ʱ�䣬��λmin
    HI_U8  period_diff_th       : 4;            // ��Ƶ���ڲ������ޣ���λus
    HI_U8  valid_ntb_num_ratio  : 4;            // һ���У���ЧNTB������ֵ�ﵽ���ޣ�������Ч����λ0.1���޸�Ϊ6
    HI_U8  save_his_duration;                   // ������ʷ��¼�ļ������λmin

    HI_U16 sta_cache_ntb_cnt;                   // STA�˻���NTB����
    HI_U16 variance_ratio_th1;                   // �����ֵ1����ֵΪ1.5������100���㣬Ϊ150

    HI_U8  adapt_pkt_cnt[TF_NTB_ADAPT_PARA_NUM];// ����Ӧ����������NTB��������
    HI_U16 adapt_ratio[TF_NTB_ADAPT_PARA_NUM];  // ����Ӧ��������Ӧ�����µı�ֵ���ޣ���λ0.01

    HI_U32 para_idx_max             : 3;        // �о������У�SNR�ر�ʱ������Ӧ����ʹ������±�
    HI_U32 para_idx_max_snr         : 3;        // �о������У�SNR��ʱ������Ӧ����ʹ������±�
    HI_U32 override_para_idx        : 3;        // �Ʒ������У�SNR�ر�ʱ������Ӧ����ʹ������±�
    HI_U32 override_para_idx_snr    : 3;        // �Ʒ������У�SNR��ʱ������Ӧ����ʹ������±�
    HI_U32 cache_valid_ratio        : 4;        // �����¼����Ч�ĸ�������λ0.1
    HI_U32 big_noise_net_ratio      : 4;        // ����������ı������ޣ���λ0.1
    HI_U32 big_noise_cache_ratio    : 4;        // �����������¼�ı������ޣ���λ0.1
    HI_U32 restart_cycle_len        : 8;        // ���ν������ݳ��������ޣ����¿�ʼʶ������
}DIAG_CMD_CONFIG_TF_PASSIVE_NV_STRU;

#define MULTI_FREQ_NV_CNT_MAX 		(4)
typedef struct
{
    HI_U8 multi_freq[MULTI_FREQ_NV_CNT_MAX];           // ��Ƶ̽��˳��

    HI_U8 nw_detect_cnt_max;
	HI_U8 nw_detect_interval;
    HI_U8 sta_detect_cnt_max;

	HI_U8 sta_detect_interval;

}DIAG_CMD_MULTI_FREQ_PARA_NV_STRU;

//ͣ���¼�ͳ��nv
typedef struct
{
    HI_U8 plug_module;                       //��ι�ģ��
    HI_U8 plug_module_read_fail;            //��ȡgpio26�ܽ�ʧ��
    HI_U8 module_power_failure_cnt;         //ģ��ͣ�����
    HI_U8 power_failure_recover;            //ͣ��ָ�ԭ��

    HI_U8 power_failure_fail_report_cnt;    //ͣ���¼�δ���ϱ�����
    HI_BOOL offen_power_failure;            //��η���ͣ���¼�
    HI_BOOL is_first_power_failure;        //�ǵ�һ��ͣ��
    HI_BOOL is_first_power_failure_recover; //��һ��ͣ��ָ�

    HI_U8 first_power_failure_recover;      //����ͣ��ָ�ԭ��
    HI_U8 pad1;
    HI_U16 reset_offten_power_failure;       //���þ���ͣ���־λʱ�䣬��λmin

    HI_U32 first_poower_failure_sys_time;   //����ͣ��ʱ��

    HI_U32 first_power_failure_recover_time;//����ͣ��ָ�ʱ��

    HI_U32 last_power_failure_sys_time;     //���һ��ͣ��ʱ��

    HI_U32 last_power_failure_recover_time; //���һ��ͣ��ָ�ʱ��

}DIAG_CMD_POWER_FAILURE_STATISTICS_NV_STRU;

//�ǵ���ģ��ͣ��nv����
#define POWEROFF_ADAPT_PARA_CNT     (6) // ͣ��������Ӧ��������

// ͣ��������Ӧ������Ŀ�ṹ
typedef struct
{
    HI_U16 station_cnt;                 // վ�����
    HI_U16 poweroff_check_cycle_len;    // ͣ��������ʱ��
    HI_U16 heartbeat_duration_th;       // δ�յ�����ʱ������
    HI_U16 pad;
} DIAG_CMD_POWEROFF_CHECK_ADAPTIVE_PARA_STRU;

// ͣ�������NV��ṹ
typedef struct
{
    HI_U16 network_station_num;         // �����ģ����0:��ʾ��������ʵ��վ������̬ѡȡ����
    HI_U16 adaptive_redundance_para;    // ��������Ӧ��������������ֹ���ڲ�������

    HI_U16 arrange_cycle_len;           // �����¼�ͳ����ˢ������ʱ������λmin
    HI_U8  channel_leave_cnt_th;        // �ŵ�ԭ�����ߴ�������
    HI_U8  pad;

    DIAG_CMD_POWEROFF_CHECK_ADAPTIVE_PARA_STRU adaptive_para[POWEROFF_ADAPT_PARA_CNT]; // ͣ��������Ӧ����

    HI_U8  protect_round_num;       // վ�����ߺ�������ô����������ߣ�����Ϊ����
    HI_U8  pad2;
    HI_U16 sta_delay_max_time;      // վ�����Ӧ��ʱ���ӳ����ʱ�䣬��λs

    HI_U8  eq_heartbeat_cnt_th;     // վ���յ���ô�����Ч�����󣬷������ΰ���Ӧ��
    HI_U8  set_eq_heartbeat_cnt;    // �ӳ���������Ҫ�ϱ���TEIʱ�������յ���Ч������Ϊ��ֵ
    HI_U8  sta_transmit_enable;     // Ҷ�ӽڵ�ת������ʹ��
    HI_U8  sta_transmit_delay;      // Ҷ�ӽڵ�ת�������ӳ�ʱ������λs

    HI_BOOL is_power_off_enable;    //ͣ��mac��ز���ʹ�ܿ���
    HI_U8  pad1[3];
} DIAG_CMD_CONFIG_POWEROFF_CHECK_PARA_STRU;



#ifdef __cplusplus
}
#endif

#endif /*MAC_NV_H*/
