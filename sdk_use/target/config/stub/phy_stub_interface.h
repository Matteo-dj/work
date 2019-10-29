/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : phy_stub_interface.h
  �� �� ��   : ����
  ��    ��   : likunhe/00194117
  ��������   : 2011��11��29��
  ����޸�   :
  ��������   : ������Ĳ���׮�ӿ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2011��11��29��
    ��    ��   : likunhe/00194117
    �޸�����   : �����ļ�

******************************************************************************/
#ifndef DRIVER_STUB_INTERFACE_H
#define DRIVER_STUB_INTERFACE_H

/************************************************************
                     ��������ģ���ͷ�ļ�
************************************************************/
#include "sal_inf.h"
#include "phy_driver_interface.h"

#ifdef __cplusplus
    extern "C"{
#endif

/************************************************************
                               �궨��
************************************************************/
//GTR�·�����ϢID
#define STUB_REQ_BASE_ID             0x0000
#define STUB_BEACON_SG_ID            (STUB_REQ_BASE_ID + 0x0)
#define STUB_SOF_SG_ID            (STUB_REQ_BASE_ID + 0x1)
#define STUB_SACK_SG_ID            (STUB_REQ_BASE_ID + 0x2)
#define STUB_COORDINATE_SG_ID            (STUB_REQ_BASE_ID + 0x3)
#define STUB_SET_TONE_MASK_TABLE_ID   (STUB_REQ_BASE_ID + 0x4)
#define STUB_SET_POWER_LOADING_TABLE_ID   (STUB_REQ_BASE_ID + 0x5)


#define STUB_FC_BEACON_ID           (STUB_REQ_BASE_ID + 0x50)  // Beacon��FC����Ϣ
#define STUB_FC_SOF_ID              (STUB_REQ_BASE_ID + 0x51)  // Sof��FC����Ϣ
#define STUB_FC_SOUND_ID            (STUB_REQ_BASE_ID + 0x52)  // δ��
#define STUB_FC_SACK_ID             (STUB_REQ_BASE_ID + 0x53)  // Sack��FC����Ϣ
#define STUB_NTB_CONFIG_ID         (STUB_REQ_BASE_ID + 0x54)  // ��վ��Ķ�����ʶ����
#define STUB_TEI_CONFIG_ID          (STUB_REQ_BASE_ID + 0x55)  // ��վ����豸��ʶ����
#define STUB_ROLE_CONFIG_ID         (STUB_REQ_BASE_ID + 0x56)  // ��վ��Ľ�ɫ����
#define STUB_PROXY_CONFIG_ID        (STUB_REQ_BASE_ID + 0x57)  // ��վ��ĸ���������
#define STUB_TX_TONE_MAP_ID         (STUB_REQ_BASE_ID + 0x58)  // ����ʱ�õ�Tone Map
#define STUB_RX_TONE_MAP_ID         (STUB_REQ_BASE_ID + 0x59)  // ����ʱ�õ�Tone Map
#define STUB_CRC_SWITCH_ID          (STUB_REQ_BASE_ID + 0x5a)  // CRC����ϱ��Ŀ�������
#define STUB_BLER_REQUEST_ID        (STUB_REQ_BASE_ID + 0x5b)  // Bler�ϱ��Ŀ�������
#define STUB_PCS_SWITCH_ID          (STUB_REQ_BASE_ID + 0x5c)  // �ز������ϱ��Ŀ�������
#define STUB_TX_DONE_SWITCH_ID      (STUB_REQ_BASE_ID + 0x5d)  // ���ͽ���ϱ��Ŀ���
#define STUB_CHANNEL_INFO_SWITCH_ID (STUB_REQ_BASE_ID + 0x5e)  // �ŵ������ϱ��Ŀ���
#define STUB_FC_RTS_CTS_ID          (STUB_REQ_BASE_ID + 0x5f)  // RTS/CTS��FC����Ϣ
/* BEGIN: Added by likunhe/00194117, 2012/2/24   PN:Build3 Require*/
#define STUB_STATISTIC_INFO_REQUEST_ID (STUB_REQ_BASE_ID + 0x60)  // Preamble��FC����Ϣ��ѯ
/* BEGIN: Added by likunhe/00194117, 2012/4/28   ���ⵥ��:DTS2012050801863*/
//#define STUB_REQ_MAX_ID             STUB_STATISTIC_INFO_REQUEST_ID
#define STUB_ISP_SEND_ID            (STUB_REQ_BASE_ID + 0x61)   //ISP����ʹ��
#define STUB_ISP_RCV_ID             (STUB_REQ_BASE_ID + 0x62)   //ISP����ʹ��
#define STUB_STATISTIC_INTERRUTP_INFO_REQUEST_ID    (STUB_REQ_BASE_ID + 0x63)   //��ѯ�쳣�ж�
//#define STUB_REQ_MAX_ID             STUB_STATISTIC_INTERRUTP_INFO_REQUEST_ID
#define STUB_COORDINATE_ID         (STUB_REQ_BASE_ID + 0x64)
#define STUB_UNIVERSE_ID           (STUB_REQ_BASE_ID + 0x65)
#define STUB_RX_FILTER_ID          (STUB_REQ_BASE_ID + 0x66)
#define STUB_RD_REG_ID             (STUB_REQ_BASE_ID + 0x67)
#define STUB_WR_REG_ID             (STUB_REQ_BASE_ID + 0x68)
#define STUB_ACCESS_CONFIG_ID      (STUB_REQ_BASE_ID + 0x69)
#define STUB_CYC_SEND_ID           (STUB_REQ_BASE_ID + 0x6a)
#define STUB_RX_FILTER1_ID         (STUB_REQ_BASE_ID + 0x6b)
#define STUB_TX_PL_ID              (STUB_REQ_BASE_ID + 0x6c)
#define STUB_REQ_PL_ID             (STUB_REQ_BASE_ID + 0x6d)   //��ѯ�쳣�ж�

/* BEGIN: Added by liujian 00280874, 2016/4/26   ���ⵥ��:MAC_PHY_TEST*/
#define STUB_RX_FREQERR_ID          (STUB_REQ_BASE_ID + 0x6e)
#define STUB_QUERY_RCV_INFO_ID      (STUB_REQ_BASE_ID + 0x6F)
#define STUB_SET_TEST_INFO_ID       (STUB_REQ_BASE_ID + 0x70)
#define STUB_DEFC_FORM_INFO_ID      (STUB_REQ_BASE_ID + 0x71)
#define STUB_BUFF_FILTER_INFO_ID    (STUB_REQ_BASE_ID + 0x72)
#define STUB_SEND_TIMESTAMP_ID      (STUB_REQ_BASE_ID + 0x73)
#define STUB_TX_FREQERR_ID          (STUB_REQ_BASE_ID + 0x74)
#define STUB_BEACON_V200_ID         (STUB_REQ_BASE_ID + 0x75)
#define STUB_SOF_V200_ID            (STUB_REQ_BASE_ID + 0x76)
#define STUB_SACK_V200_ID           (STUB_REQ_BASE_ID + 0x77)
#define STUB_COORDINATE_V200_ID     (STUB_REQ_BASE_ID + 0x78)
#define STUB_SNID_FREQERR_FORM_INFO_ID          (STUB_REQ_BASE_ID + 0x79)
#define STUB_MAC_PHY_TEST_INFO_REQUEST_ID       (STUB_REQ_BASE_ID + 0x7b)
#define STUB_SC_FRAME_TX_ID         (STUB_REQ_BASE_ID + 0x7c)   //���ز�(խ��)���ķ���
#define STUB_SC_RX_STATIC_REQ_ID    (STUB_REQ_BASE_ID + 0x7d)   //���ز�(խ��)���Ľ���ͳ�Ʋ�ѯ
#define STUB_SET_TMI_TABLE_ID       (STUB_REQ_BASE_ID + 0x7e)
#define STUB_REQ_MAX_ID             (STUB_SET_TMI_TABLE_ID)


//�쳣����GTR�·�������ID
#define STUB_ABNORMITY_REQ_BASE_ID          0x00a0
#define STUB_ISP_RX_TX_SWITCH_REQ_ID        (STUB_ABNORMITY_REQ_BASE_ID + 0x00)//ISP���ա����л��쳣����
#define STUB_SOFT_RESET_REQ_ID              (STUB_ABNORMITY_REQ_BASE_ID + 0x01)//�����PHY��λ
/* BEGIN: Added by likunhe/00194117, 2012/5/11   ���ⵥ��:DTS2012052206629*/
#define STUB_BACK_TONE_MASK_REQ_ID          (STUB_ABNORMITY_REQ_BASE_ID + 0x02)//ʹ�ñ���ToneMask
#define STUB_NTB_ROLE_CONFIG_REQ_ID         (STUB_ABNORMITY_REQ_BASE_ID + 0x03)//NTB�Ľ�ɫ����
#define STUB_NTB_RESULT_REQ_ID              (STUB_ABNORMITY_REQ_BASE_ID + 0x04)//NTB�Ľ����ѯ
/* BEGIN: Modified by likunhe/00194117, 2012/5/22   ���ⵥ��:DTS2012052508462*/
#define STUB_POWER_SAVE_REQ_ID              (STUB_ABNORMITY_REQ_BASE_ID + 0x05)//�����͹��Ĳ���
#define STUB_RX_TX_SWITCH_REQ_ID            (STUB_ABNORMITY_REQ_BASE_ID + 0x06)//����rx/tx�л�����
/* BEGIN: Modified by likunhe/00194117, 2012/6/18   PN:DTS2012061806451*/
//#define STUB_ABNORMITY_REQ_MAX_ID           STUB_RX_TX_SWITCH_REQ_ID
#define STUB_GTR_CUT_LOG_REQ_ID             (STUB_ABNORMITY_REQ_BASE_ID + 0x07)//��ӡ������������log�ָ��
/* BEGIN: Modified by likunhe/00194117, 2012/6/20   ���ⵥ��:AddChannelConfigPara*/
#define STUB_CHANNEL_CONFIG_REQ_ID          (STUB_ABNORMITY_REQ_BASE_ID + 0x08)//�ŵ���������
#define STUB_SPEED_CONFIG_REQ_ID            (STUB_ABNORMITY_REQ_BASE_ID + 0x09)//���ʲ���
#define STUB_NOISE_CONFIG_REQ_ID            (STUB_ABNORMITY_REQ_BASE_ID + 0x0a)//������������
#define STUB_WR_REG_REQ_ID                  (STUB_ABNORMITY_REQ_BASE_ID + 0x0b)//д�Ĵ���
#define STUB_RD_REG_REQ_ID                  (STUB_ABNORMITY_REQ_BASE_ID + 0x0c)//���Ĵ���
#define STUB_TFPT_DOWNLOAD_REG_ID           (STUB_ABNORMITY_REQ_BASE_ID + 0x0d )//tftp����
#define STUB_ABNORMITY_REQ_MAX_ID           STUB_TFPT_DOWNLOAD_REG_ID


//GTR�·���NV������ID��Ż�ֵ
#define STUB_CMD_BASE_NV_ID                 0x0720
#define STUB_CMD_AGC_PARA_NV_ID             (STUB_CMD_BASE_NV_ID + 0x0000)
#define STUB_CMD_HREST_NV_ID                (STUB_CMD_BASE_NV_ID + 0x0001)
#define STUB_CMD_FEQ_NV_ID                  (STUB_CMD_BASE_NV_ID + 0x0002)
#define STUB_CMD_CARRIER_SENSE_NV_ID        (STUB_CMD_BASE_NV_ID + 0x0003)
#define STUB_CMD_TX_NV_ID                   (STUB_CMD_BASE_NV_ID + 0x0004)
#define STUB_CMD_TD_PROCESS_NV_ID           (STUB_CMD_BASE_NV_ID + 0x0006)
#define STUB_CMD_FEC_NV_ID                  (STUB_CMD_BASE_NV_ID + 0x0007)
#define STUB_CMD_SYMALIGN_NV_ID             (STUB_CMD_BASE_NV_ID + 0x000b)
#define STUB_CMD_NBI_PARA_NV_ID             (STUB_CMD_BASE_NV_ID + 0x000c)
#define STUB_CMD_IMPN_PARA_NV_ID            (STUB_CMD_BASE_NV_ID + 0x000d)
#define STUB_CMD_PLCSC_PARA_NV_ID           (STUB_CMD_BASE_NV_ID + 0x000e)
#define STUB_CMD_CUBIC_PARA_NV_ID           (STUB_CMD_BASE_NV_ID + 0x000f)
#define STUB_CMD_FREQ_NV_ID                 (STUB_CMD_BASE_NV_ID + 0x0010)
#define STUB_CMD_ISP_NV_ID                  (STUB_CMD_BASE_NV_ID + 0x0011)
#define STUB_CMD_CLOCK_NV_ID                (STUB_CMD_BASE_NV_ID + 0x0012)
//#define STUB_CMD_NTB_NV_ID                  (STUB_CMD_BASE_NV_ID + 0x0013)
#define STUB_CMD_PCS_NV_ID                  (STUB_CMD_BASE_NV_ID + 0x0014)
#define STUB_CMD_PHY_STUB_NV_ID             (STUB_CMD_BASE_NV_ID + 0x0015)
#define STUB_CMD_OVER_CURRENT_NV_ID         (STUB_CMD_BASE_NV_ID + 0x0016)
#define STUB_CMD_FREQ_PARA_NV_1_ID          (STUB_CMD_BASE_NV_ID + 0x0017)
#define STUB_CMD_CARRIER_SENSE_NV_1_ID      (STUB_CMD_BASE_NV_ID + 0x0018)
#define STUB_CMD_CARRIER_SENSE_NV_2_ID      (STUB_CMD_BASE_NV_ID + 0x0019)
#define STUB_CMD_MAX_NV_ID                  (STUB_CMD_CARRIER_SENSE_NV_2_ID)


//�ϱ�GTR��ID
#define STUB_RX_BASE_ID             0x0080
#define STUB_TX_DONE_ID             (STUB_RX_BASE_ID + 0x00)  // TX�����Ϣ
#define STUB_FC_CRC_RESULT_ID       (STUB_RX_BASE_ID + 0x01)  // FC��CRC�������Ϣ
#define STUB_PAYLOAD_CRC_RESULT_ID  (STUB_RX_BASE_ID + 0x02)  // Payload��CRC�������Ϣ
#define STUB_PCS_RESULT_ID          (STUB_RX_BASE_ID + 0x03)  // PCS�Ľ������Ϣ
#define STUB_CHANNEL_INFO_ID        (STUB_RX_BASE_ID + 0x04)  // �ŵ������ϱ�
#define STUB_STATISTIC_INFO_ID      (STUB_RX_BASE_ID + 0x05)  // �ŵ������ϱ�
#define STUB_ISP_RESULT_INFO_ID     (STUB_RX_BASE_ID + 0x06)  // ��ѯISP�Ľ��
#define STUB_STATISTIC_INTERRUPT_INFO_ID    (STUB_RX_BASE_ID + 0x07)  // ��ѯISP�Ľ��
#define STUB_NTB_RESULT_INFO_ID     (STUB_RX_BASE_ID + 0x08)  //�ϱ���NTB��ѯ���
#define STUB_PCS_IND                (STUB_RX_BASE_ID + 0x09)
#define STUB_FC_IND                 (STUB_RX_BASE_ID + 0x0a)
#define STUB_PB_IND                 (STUB_RX_BASE_ID + 0x0b)
#define STUB_WAKE_SLEEP_IND         (STUB_RX_BASE_ID + 0x0c)
#define STUB_GTR_CUT_LOG_IND        (STUB_RX_BASE_ID + 0x0d)  //GTR����Log�ķָ��
#define STUB_CHIP_SELECT_INFO_IND   (STUB_RX_BASE_ID + 0x0e)  //оƬɸѡ�Ľ��
#define STUB_RD_REG_INFO_ID         (STUB_RX_BASE_ID + 0x0f)  //��ȡ�ļĴ������
#define STUB_RCV_INFO_ID            (STUB_RX_BASE_ID + 0X10)    //������Ϣ����ϱ�
#define STUB_MAC_PHY_TEST_INFO_ID   (STUB_RX_BASE_ID + 0X11)    //mac_phy�ӿڲ�����Ϣ�ϱ�
#define STUB_SC_RX_STATIC_RSP_ID    (STUB_RX_BASE_ID + 0x12)   //���ز�(խ��)���Ľ���ͳ�Ʋ�ѯӦ��
#define STUB_SC_TX_FRAME_RPT_ID     (STUB_RX_BASE_ID + 0x13)   //���ز����ͱ���ά���ϱ�
#define STUB_SC_RX_FRAME_RPT_ID     (STUB_RX_BASE_ID + 0x14)   //���ز����ձ���ά���ϱ�
#define STUB_SC_FREAM_TIME_RPT_ID     (STUB_RX_BASE_ID + 0x15)   //���ز����ձ���ά����
#define STUB_SC_PCS_RPT_ID     (STUB_RX_BASE_ID + 0x16)   //���ز����ձ���ά���ϱ�


#define FC_BYTE_SIZE        16          //FC���ֽ���
#define TONE_MAP_SIZE       512         //Tone Map�Ĵ�С
/* BEGIN: Added by likunhe/00194117, 2012/6/20   PN:AddChannelConfigPara*/
#define CHANNEL_COEFF_NUM           1024        //�ŵ�ϵ���ĸ���
/* END:   Added by likunhe/00194117, 2012/6/20 */
/* BEGIN: Added by likunhe/00194117, 2012/7/19   PN:AddNoiseCtrl*/
#define NOISE_PACKET_DATA_NUM       512         //�������ݰ��Ĵ�С
/* END:   Added by likunhe/00194117, 2012/7/19 */
/* BEGIN: Added by likunhe/00194117, 2012/8/31   PN:AddNoiseCtrl*/
#define TFTP_DOWNLOAD_FILE_MAX_BYTE_SIZE (8 * 1024 * 1024)   //tftp�ļ�������ֽ���
/* END:   Added by likunhe/00194117, 2012/8/31 */

//���ز��Ĺ���ģʽ
#define STUB_SC_WORK_MODE_OFF                   0           //�ر�
#define STUB_SC_WORK_MODE_TX                    1           //����ģʽ,���������
#define STUB_SC_WORK_MODE_RX                    2           //����ģʽ
#define STUB_SC_WORK_MODE_TX_PKT_CHECK          3           //����ģʽ�������Զ�У��
#define STUB_SC_WORK_MODE_RX_PKT_CHECK          4           //����ģʽ�������Զ�У��
#define STUB_SC_WORK_MODE_TX_NTB                5           //����ģʽ����NTB
#define STUB_SC_WORK_MODE_RX_NTB                6           //����ģʽ����NTB
#define STUB_SC_WORK_MODE_PHY_TEST              7           //PHY�Ĳ���ģʽ

//��������
#define RTS_CTS_DT                  (3)
#define SOUND_DT                    (4)
#define SYNCH_DT                     (5)
#define UNIVERSE_DT                 (6)

/************************************************************
                             ���ݽṹ����
************************************************************/

//HSO�·���TX��Tone Map�ṹ
typedef struct _tx_tone_map_table_st_
{
    //HI_U32 dtei;
    bit_loading_st tone_map;
}tx_tone_map_table_st;

//HSO�·���RX��Tone Map�ṹ
typedef struct _rx_tone_map_table_st_
{
    /* BEGIN: Modified by likunhe/00194117, 2012/5/29   ���ⵥ��:SoundingBitloadingTest*/
    HI_U16 stei;
    HI_U16 tmi;
    /* END:   Modified by likunhe/00194117, 2012/5/29 */
    bit_loading_st tone_map_table_pb136;
    bit_loading_st tone_map_table_pb520;
}rx_tone_map_table_st;


//Bitloading��Tone Map���ϱ��ṹ
typedef struct _bitloading_result_st_
{
    bit_loading_st tone_map_table_pb136;
    bit_loading_st tone_map_table_pb520;
}bitloading_result_st;


//stub����ϱ����ƿ���
typedef struct _stub_reprot_switch_st_
{
    HI_U8  crc_switch;
    HI_U8  tx_done_switch;
    HI_U8  pcs_switch;
    HI_U8  channel_info_switch;
}stub_reprot_switch_st;

//����Beacon֡��FC����
typedef struct _beacon_fc_para_st_
{
    HI_U8  dt;         //�����
    HI_U8 access;
    HI_U8  snid;       //�������ʶ��
    HI_U8 insertCP_flag;
    HI_U32 bts;        //�ű�ʱ���������ʱ�䣩
    HI_U32 bpc;        //�ű����ڼ���
    HI_U16 stei;       //Դ�豸��ʶ��
    HI_U8  tmi;
	HI_U8 bts_ext;
	HI_U32 rsd;
}beacon_fc_para_st;


//����Sof֡��FC����
typedef struct _sof_fc_para_st_
{
    HI_U8  dt;        //�����
    HI_U8 access;
    HI_U8  snid;      //�������ʶ��
    HI_U8 insertCP_flag;

    HI_U16 stei;      //Դ�豸��ʶ��
    HI_U16 dtei;      //Ŀ���豸��ʶ��

    HI_U8  pb_num;    //PB����Ŀ
    HI_U8  tmi;       //ToneMap������
    HI_U8  bcf;       //�㲥��־λ
    HI_U8 next_tmi;

	HI_U8 ext_tmi;
	HI_U8 rsd1;
	HI_U16 reserve2;     //����
    /* BEGIN: Added by likunhe/00194117, 2012/3/3   ���ⵥ��:V130 Stub Edit*/
    //HI_U8  pb_size;   //PB��Ĵ�С��0��520��1��136
    // HI_U8  pilot;      //pilot��ʶ
    /* END:   Added by likunhe/00194117, 2012/3/3 */
}sof_fc_para_st;


//����Sound֡��FC����
typedef struct _sound_fc_para_st_
{
    HI_U8  dt;         //�����
    HI_U8  snid;       //�������ʶ��
    HI_U16 stei;       //Դ�豸��ʶ��
    HI_U16 dtei;       //Ŀ���豸��ʶ��
    HI_U8  saf;        //����Ӧ���־
    /* BEGIN: Added by likunhe/00194117, 2012/3/3   ���ⵥ��:V130 Stub Edit*/
    //HI_U8  tmi;        //
    HI_U8  ver;        //
    /* END:   Added by likunhe/00194117, 2012/3/3 */
}sound_fc_para_st;


//����Sack֡��FC�Ĳ���
typedef struct _sack_fc_para_st_
{
    HI_U8  dt;
    HI_U8  snid;
    HI_U8 access;//j00206949 2015.2.7
    HI_U8 reserve;
}sack_fc_para_st;

//����Rts/Cts֡��FC�Ĳ���
typedef struct _rts_cts_fc_para_st_
{
    HI_U8  dt;
    HI_U8  snid;
    HI_U8 access;//j00206949 2015.2.7
    HI_U8 ver;
}rts_cts_fc_para_st;

//j00206949 2015.2.7 start
//����coordinate֡��FC�Ĳ���
typedef struct _coordinate_fc_para_st_
{
    HI_U8  dt;
    HI_U8  snid;
    HI_U8 access;
    HI_U8 reserve;
}coordinate_fc_para_st;

//����ͨ��֡��FC�Ĳ���
typedef struct _universe_fc_para_st_
{
    HI_U8  dt;
    HI_U8  snid;
    HI_U8 access;//j00206949 2015.2.7
    /* BEGIN: Modified by liujian 00280874, 2016/5/30   ���ⵥ��:MAC_PHY_TEST*/
    //HI_U8 reserve;
    HI_U8 stei;
    /* END:   Modified by liujian 00280874, 2016/5/30 */

    HI_U8 mac[6];
    HI_U16 ver;
}universe_fc_para_st;

//���ɱ���֡��FC�Ĳ���
typedef struct _reserve_fc_para_st_
{
    HI_U8  dt;
    HI_U8  snid;
    HI_U8 access;//j00206949 2015.2.7
    HI_U8 reserve;
}reserve_fc_para_st;
//j00206949 2015.2.7 end

// ����SNID
typedef struct _stub_my_snid_st_
{
    HI_U8  my_snid;
    HI_U8  resreve1;
    HI_U16 reserve2;
}stub_my_snid_st;

// ��վ��TEI
typedef struct _stub_my_tei_st_
{
    HI_U16 my_tei;
    HI_U16 reserve;
}stub_my_tei_st;

typedef struct _stub_ntb_para_st_
{
    HI_U16 syn_freq;
    HI_U16 syn_snid;

    HI_U16 syn_tei;
    HI_U16 pad;
}stub_ntb_para_st;

typedef struct _stub_my_access_st_
{
    HI_U16 access;
    HI_U16 reserve;
}stub_my_access_st;

// ��վ���ɫ
typedef struct _stub_my_role_st_
{
    //HI_U8  my_role;
    //HI_U8  reserve1;
    //HI_U16 reserve2;
    HI_U32 my_role;//ʵ�������ж�����
}stub_my_role_st;

// ��վ�㸸����TEI
typedef struct _stub_my_proxy_st_
{
    /* BEGIN: Modified by likunhe/00194117, 2012/5/17   ���ⵥ��:DTS2012052206629*/
    //HI_U8  my_proxy;
    //HI_U8  reserve1;
    //HI_U16 reserve2;
    HI_U16 my_proxy;
    HI_U16 reserve1;
    /* END:   Modified by likunhe/00194117, 2012/5/17 */
}stub_my_proxy_st;

/* BEGIN: Added by likunhe/00194117, 2012/5/17   PN:DTS2012052206629*/
//NTB�Ľ�ɫ����
typedef struct _stub_ntb_role_st_
{
    HI_U8  synch_snid;
    HI_U8  ntb_stei_parse_en;   //ͬ���������ж�ʹ��
    HI_U16 rsd2;
}stub_ntb_role_st;
/* END:   Added by likunhe/00194117, 2012/5/17 */

// ǰ������ϱ���
typedef struct _stub_pcs_report_st_
{
    HI_U8  pcs_report;
    HI_U8  type;
    HI_U16 reserve;
}stub_pcs_report_st;

// CRC����ϱ��Ŀ���
typedef struct _stub_crc_switch_st_
{
    HI_U8  crc_switch;
    HI_U8  reserve1;
    HI_U16 reserve2;
}stub_crc_switch_st;

// PCS����ϱ��Ŀ���
typedef struct _stub_pcs_switch_st_
{
    HI_U8  pcs_switch;
    HI_U8  reserve1;
    HI_U16 reserve2;
}stub_pcs_switch_st;

// BLER����ϱ��Ŀ���
typedef struct _stub_bler_switch_st_
{
    HI_U8  bler_switch;
    HI_U8  reserve1;
    HI_U16 reserve2;
}stub_bler_switch_st;

// ���ͽ���ϱ��Ŀ���
typedef struct _stub_tx_done_switch_st_
{
    HI_U8  dt;
    HI_U8  reserve1;
    HI_U16 reserve2;
}stub_tx_done_switch_st;

// �ŵ������ϱ��Ŀ���
typedef struct _stub_channel_info_switch_st_
{
    HI_U8  channel_info_switch;
    HI_U8  reserve1;
    HI_U16 reserve2;
}stub_channel_info_switch_st;

/* BEGIN: Added by likunhe/00194117, 2012/3/4   ���ⵥ��:V130 Stub Edit*/
typedef struct
{
    HI_U8  statistic_info_switch;
    HI_U8  rsd1;
    HI_U16 period;           //��ʱ�ϱ������ڣ���λΪms
}stub_statistic_info_switch_st;
/* END:   Added by likunhe/00194117, 2012/3/4 */

/* BEGIN: Added by likunhe/00194117, 2012/4/28   ���ⵥ��:DTS2012050801863*/
//ISP���Ϳ���
typedef struct
{
    HI_U8  tx_en;
    HI_U8  tx_once_en;
    HI_U16 rsd2;
}stub_isp_tx_para_st;

//ISP���տ���
typedef struct
{
    HI_U8  isp_det_en;
    HI_U8  isp_det_mode;
    HI_U16 rsd2;
}stub_isp_rx_para_st;
/* END:   Added by likunhe/00194117, 2012/4/28 */

/* BEGIN: Added by likunhe/00194117, 2012/5/3   ���ⵥ��:DTS2012050801863*/
//ISP�ա����л���ʱ�����ò���
typedef struct
{
    HI_U16 timer_switch;
    HI_U16 rsd;
    HI_U32 timer_period;
}stub_isp_rx_tx_switch_para_st;
/* END:   Added by likunhe/00194117, 2012/5/3 */

/* BEGIN: Added by likunhe/00194117, 2012/5/5   ���ⵥ��:DTS2012050801863*/
//�����PHY��λ�Ķ�ʱ�����ò���
typedef struct
{
    HI_U16 timer_switch;
    HI_U16 rsd;
    HI_U32 timer_period;
}stub_soft_reset_para_st;
/* END:   Added by likunhe/00194117, 2012/5/5 */

//OMͳ����
typedef struct _stub_tx_om_st_
{
    HI_U32 stub_receive_cnt;
    HI_U32 nv_id_error;
    /* BEGIN: Added by likunhe/00194117, 2012/5/5   ���ⵥ��:DTS2012050801863*/
    HI_U32 abnormity_cmd_error;
    /* END:   Added by likunhe/00194117, 2012/5/5 */
}stub_tx_om_st;


// ���ͽ���ϱ��Ľṹ
typedef struct _tx_done_result_st_
{
    HI_U8  frame_type;     // ֡����
    HI_U8  result;         // ?
    HI_U16 rsd;
} tx_done_result_st;

// FC��CRC����ϱ��Ľṹ
typedef struct _fc_crc_result_st_
{
    HI_U8  frame_type;
    HI_U8  crc_result;
    HI_U16 rsd;
}fc_crc_result_st;

// Payload��CRC����ϱ��Ľṹ
typedef struct _payload_crc_result_st_
{
    HI_U8  frame_type;
    HI_U8  crc_result;
    HI_U8  pb_index;
    HI_U8  reserve;
}payload_crc_result_st;

//PCS�Ľ���ϱ��Ľṹ
typedef struct _pcs_resut_st_
{
    HI_U8  type;
    HI_U8  reserve1;
    HI_U16 reserve2;
}pcs_resut_st;

//�ŵ������ϱ��Ľṹ
typedef struct _channel_info_st_
{
    HI_U8  channel_info;
    HI_U8  reserve1;
    HI_U16 reserve2;
}channel_info_st;
/* BEGIN: Added by likunhe/00194117, 2012/3/4   ���ⵥ��:V130 Stub Edit*/
//PCS��FC��CRCͳ��
typedef struct _statistic_info_st_
{
    HI_U32 pcs_interrupt_cnt;      //Preamble�ж�
    HI_U32 fc_interrupt_cnt;       //FC���ж�
    HI_U32 pb_interrupt_cnt;       //PB���ж�
    HI_U32 rx_end_interrupt_cnt;   //��������ж�
    HI_U32 tx_end_interrupt_cnt;   //��������ж�

    HI_U32 fc_right_expect_cnt;
    HI_U32 fc_right_cnt;
    HI_U32 fc_total_cnt;
    HI_U32 stub_tx_cnt;            //Stub�ķ��ʹ���
}statistic_info_st;
/* END:   Added by likunhe/00194117, 2012/2/24 */

/* BEGIN: Added by likunhe/00194117, 2012/4/28   ���ⵥ��:DTS2012050801863*/
//�쳣�ж�ͳ��
typedef struct _statistic_interrupt_info_st_
{
    //HI_U32 isp_series_detect    ;
    HI_U32 nbi_pow_int_cnt;
    HI_U32 isp_win_detect       ;
    HI_U32 isp_send_end         ;
    HI_U32 zero_cross_detect    ;
    HI_U32 only_preamble_rcv    ;
    HI_U32 onlypreamble_send_end;
    HI_U32 tone_map_move_err    ;
    HI_U32 tei_query_err        ;
    HI_U32 beacon_tmi_err       ;
    HI_U32 sof_tmi_err          ;
 	HI_U32 burst_send_finish_cnt;
	HI_U32 burst_rcv_finish_cnt;
	HI_U32 snr_rpt_cnt;
	HI_U32 fc_err_cnt;
	HI_U32 fc_ok_cnt;
	HI_U32 pb4_cnt;
	HI_U32 pb3_cnt;
	HI_U32 pb2_cnt;
	HI_U32 pb1_cnt;
	HI_U32 rcv_overflow_cnt;
	HI_U32 st4_cnt;
	HI_U32 st3_cnt;
	HI_U32 st2_cnt;
	HI_U32 para_ok_cnt;
	HI_U32 frame_send_ok_cnt;
	HI_U32 frame_send_fail_cnt;
	HI_U32 one_frame_send_rlt_cnt;
	HI_U32 one_frame_send_start_cnt;
	HI_U32 one_frame_send_finish_cnt;
	HI_U32 tx_rx_collision_cnt;
	HI_U32 tx_tx_collision_cnt;
	HI_U32 wirte_collision_cnt;
	HI_U32 cancel_send_fail_cnt;
	//HI_U32 rsv;
	HI_U32 nbi_table_int_cnt;
}statistic_interrupt_info_st;

//ISP�����Ϣͳ��
typedef struct _isp_result_info_st_
{
    HI_U32 f1_ph1_cnt;
    HI_U32 f1_ph2_cnt;
    HI_U32 f1_ph3_cnt;
    HI_U32 f1_ph4_cnt;
    HI_U32 f1_ph5_cnt;
    HI_U32 f2_ph1_cnt;
    HI_U32 f2_ph2_cnt;
    HI_U32 f2_ph3_cnt;
    HI_U32 f2_ph4_cnt;
    HI_U32 f2_ph5_cnt;
}isp_result_info_st;
/* END:   Added by likunhe/00194117, 2012/4/28 */

/* BEGIN: Added by likunhe/00194117, 2012/5/17   PN:DTS2012052206629*/
//NTB�Ľ��
typedef struct _ntb_result_info_
{
    HI_U32 curr_bts;    //��ǰ�����btsֵ
}ntb_result_info;
/* END:   Added by likunhe/00194117, 2012/5/17 */
/* BEGIN: Added by likunhe/00194117, 2012/5/22   PN:DTS2012052508462*/
//�͹��Ĳ��ԵĿ���
typedef struct _power_save_switch_st_
{
    HI_U16 timer_switch;
    HI_U16 rsd;
    HI_U32 timer_period;
}power_save_switch_st;

//�͹��Ĳ��Խ���ϱ�
typedef struct
{
    HI_U32 TimerCount;
    HI_U32 CrcRsult;
}OM_POWER_SAVE_TRACE_IND_STRU;
/* END:   Added by likunhe/00194117, 2012/5/22 */

/* BEGIN: Added by likunhe/00194117, 2012/5/23   PN:DTS2012052508462*/
//rx/tx�л����Կ���
typedef struct _rx_tx_switch_st_
{
    HI_U16 timer_switch;
    HI_U16 rsd;
    HI_U32 timer_period;
}rx_tx_switch_st;
/* END:   Added by likunhe/00194117, 2012/5/23 */

#define PL_LEN_MAX (520)

//GTR�����������ָ��
typedef struct _gtr_mpdu_st_
{
    HI_U16 mpdu_num;
    HI_U16 mpdu_len;
    HI_U8 mpdu[PL_LEN_MAX];
}gtr_mpdu_st;
typedef struct _mpdu_st_
{
    HI_U16 mpdu_num;
    HI_U16 mpdu_len;
    HI_U8 mpdu[PL_LEN_MAX * PB_NUM_MAX];
}mpdu_st;
/* BEGIN: Added by likunhe/00194117, 2012/6/20   PN:AddChannelConfigPara*/
//�ŵ�����������
typedef struct _channel_config_st_
{
    HI_U32 index;       //���ݰ�������
    HI_S16 data[512];
}channel_config_st;
/* END:   Added by likunhe/00194117, 2012/6/20 */
/* BEGIN: Added by likunhe/00194117, 2012/6/30   PN:DTS2012062705423*/
//оƬɸѡ�Ľ��
typedef struct _chip_select_info_st_
{
   HI_U32 reslut;
}chip_select_info_st;
/* END:   Added by likunhe/00194117, 2012/6/30 */
/* BEGIN: Added by likunhe/00194117, 2012/7/19   PN:AddNoiseCtrl*/
//��������
typedef struct _noise_config_st_
{
    HI_U16 index;                       //���ݰ���������
    HI_U16 length;                      //���ݰ�����Ч����:�ֽ�
    HI_U32 addr;                        //Ҫд��ĵ�ַ
    HI_U8  data[NOISE_PACKET_DATA_NUM]; //��������
}noise_config_st;

//д�Ĵ����Ĳ���
typedef struct _wr_reg_para_st_
{
    HI_U32 addr;            //��ַ
    HI_U32 data;            //Ҫд�������
}wr_reg_para_st;

//���Ĵ����Ĳ���
typedef struct _rd_reg_para_st_
{
    HI_U32 addr;            //��ַ
}rd_reg_para_st;

//��ȡ�ļĴ���ֵ
typedef struct _rd_reg_info_st_
{
    HI_U32 data;            //����
}rd_reg_info_st;
/* END:   Added by likunhe/00194117, 2012/7/19 */
/* BEGIN: Added by likunhe/00194117, 2012/8/31   PN:AddNoiseCtrl*/
//tftp���صĲ���
typedef struct _tftp_download_para_st_
{
    HI_CHAR remote_ip[4];       //PC��IP��ַ
    HI_CHAR path[128];           //�ļ�·��
    HI_PBYTE loc_addr;          //����Ҫд������ڵ�ַ
    HI_U32  file_size;          //�����ļ��Ĵ�С����λ:�ֽ�
}tftp_download_para_st;
/* END:   Added by likunhe/00194117, 2012/8/31 */

// �˲������ھ��߼��汾(0728)
typedef struct _filter_cfg_para_st_
{
	HI_U8 filter_id;
	HI_U8 filter_dt;
	HI_U8 filter_access;
	HI_U8 filter_snid;
	HI_U16 filter_dtei;
	HI_U16 filter_action;
	HI_U8 filter_dt_vld;
	HI_U8 filter_access_vld;
	HI_U8 filter_snid_vld;
	HI_U8 filter_dtei_vld;
}filter_cfg_para_st;

// �˲����������߼��汾��Ŀǰb�汾ʹ��
typedef struct _filter_cfg_para_1_st_
{
	filter_cfg_para_st stFilterCfgPara;
	HI_U16 filter_stei;
	HI_U8 filter_stei_vld;
	HI_U8 rsv;
}filter_cfg_para_1_st;

typedef struct _stub_cyc_send_info_st_
{
    HI_U32 cyc_send_gap;    //ѭ�����͵�֡�������λus
    HI_U16 cyc_send_time;   //ѭ�����͵Ĵ���
    HI_U16 cyc_send_mode;   //ѭ������ʹ�ܱ�ʶ��1:���ز�ѭ������ʹ��,2:���ز�ѭ������ʹ��
}stub_cyc_send_info_st;

/* BEGIN: Added by liujian 00280874, 2016/4/29   ���ⵥ��:MAC_PHY_TEST*/
typedef struct
{
    HI_U8 is_5bit_tmi;
    HI_U8 is_not_clear_buff_status;
    HI_U8 wr_collision_fc_pb;
    HI_U8 read_buff_when_phy_read;

    HI_U8 pad;
    HI_U8 is_fill_payload_data_from_file;
    HI_U8 is_v200_mode;
    HI_U8 is_niverse_frame_head_stamp;
}stub_test_info_st;

typedef struct
{
    HI_U8 is_fill_time_stamp;
    HI_U8 is_fill_local_clock;
    HI_U16 time_stamp_offset;
}stub_send_time_stamp_st;

typedef struct
{
    HI_BOOL use_local_clock;
    HI_U8 pad;
    HI_S16 send_freqerr;
}stub_tx_freqerr_st;


typedef struct
{
    HI_BOOL is_fix_rx_freqerr;
    HI_U8  pad;
    HI_S16 rcv_freqerr;
}stub_rx_freqerr_st;

typedef struct
{
    HI_U8 mac[6];
    HI_U8 rsvd[2];
}stub_mac_addr_st;

typedef struct
{
    HI_U8 verid_enable[4];
    HI_U8 verid_value[4];
    HI_U8 verid_policy[4];

    HI_U8 dt_enable[9];
    HI_U8 dt_value[9];
    HI_U8 dt_policy[9];
    HI_U8 snid_enable[6];
    HI_U8 snid_value[6];
    HI_U8 snid_policy[6];
    HI_U8 odtei_enable[3];
    HI_U8 odtei_value[3];
    HI_U8 odtei_policy[3];
    HI_U8 dmac_enable[3];
    stub_mac_addr_st dmac_value[3];
    HI_U8 dmac_policy[3];
    HI_U8 bcf_enable[2];
    HI_U8 bcf_value[2];
    HI_U8 bcf_policy[2];
    HI_U8 access_enable[3];
    HI_U8 access_value[3];
    HI_U8 access_policy[3];

    HI_U8 Pad;
}stub_buff_filter_st;

typedef struct
{
    HI_U8 report_switch;
    HI_U8 rsd1;
    HI_U16 rsd2;
}stub_statistic_interrupt_info_st;


typedef struct
{
    HI_U8 ver_index;
    HI_BOOL ver_enable_flag;
    HI_U8 dt_index;
    HI_BOOL dt_enable_flag;

    HI_U8 data_tart_addr;
    HI_U8 data_end_addr;
    HI_BOOL data_enable_flag;
    HI_U8 pad;
}defc_form_set_info_st;

typedef struct
{
    HI_U8 data;
    HI_U8 filter_policy;
    HI_BOOL enable_flag;
    HI_U8 pad;
}buff_filter_set_info_st;

typedef struct
{
    HI_U8  dt;         //�����
    HI_U8 access;
    HI_U8  snid;       //�������ʶ��
    HI_U8 insertCP_flag;
    HI_U32 bts;        //�ű�ʱ���������ʱ�䣩
    HI_U32 bpc;        //�ű����ڼ���
    HI_U16 stei;       //Դ�豸��ʶ��
    HI_U8  tmi;
    HI_U8 pb_cnt;
    HI_U8 ver;
    HI_U8 rsd[3];
}v200_bcn_fc_para_st;

typedef struct
{
    HI_U8  dt;        //�����
    HI_U8 access;
    HI_U8  snid;      //�������ʶ��
    HI_U8 insertCP_flag;

    HI_U16 stei;      //Դ�豸��ʶ��
    HI_U16 dtei;      //Ŀ���豸��ʶ��

    HI_U8  pb_num;    //PB����Ŀ
    HI_U8  tmi;       //ToneMap������
    HI_U8  bcf;       //�㲥��־λ
    HI_U8 ext_tmi;

    HI_S16 freq_err;
    HI_U8 tx_local_clock;
    HI_U8 ver;
}v200_sof_fc_para_st;

typedef struct
{
    HI_U8  dt;
    HI_U8  snid;
    HI_U8 access;
    HI_U8 ver;
}v200_sack_fc_para_st;

typedef struct
{
    HI_U8  dt;
    HI_U8  snid;
    HI_U8 access;
    HI_U8 ver;
}v200_coordinate_fc_para_st;

typedef struct
{
    HI_U8  dt;        //�����
    HI_U8 access;
    HI_U8  pad;
    HI_U8  bcf;       //�㲥��־λ

    HI_U32  snid;      //�������ʶ��

    HI_U16 stei;      //Դ�豸��ʶ��
    HI_U16 dtei;      //Ŀ���豸��ʶ��

    HI_U8  pb_num;    //PB����Ŀ
    HI_U8  tmi;       //ToneMap������
    HI_U8 ext_tmi;
    HI_U8 ver;
}sg_sof_fc_para_st;

typedef struct
{
    HI_U8  dt;         //�����
    HI_U8 access;
    HI_U8 rsd[2];

    HI_U32  snid;      //�������ʶ��
    HI_U32 bpc;        //�ű����ڼ���

    HI_U16 stei;       //Դ�豸��ʶ��
    HI_U8  tmi;
    HI_U8 ver;
}sg_bcn_fc_para_st;

typedef struct
{
    HI_U8  dt;
    HI_U8  rsvd;
    HI_U8 access;
    HI_U8 ver;

    HI_U32  snid;      //�������ʶ��
}sg_sack_fc_para_st;

typedef struct
{
    HI_U8  dt;
    HI_U8  rsvd;
    HI_U8 access;
    HI_U8 ver;

    HI_U32  snid;      //�������ʶ��
}sg_coordinate_fc_para_st;

typedef struct
{
    HI_U32 snid;
    HI_S16 freq_err;
    HI_U16 index;
}snid_freqerr_form_set_info_st;

typedef struct
{
    HI_U32 send_enable_time_diff;
    HI_U32 buff_ret_err_cnt;
    HI_U32 read_rcv_info_time;
    HI_U32 rsvd[11];
}mac_phy_test_info_st;

typedef struct
{
    HI_U8 nCopys;
    HI_U8 roboMap;
    HI_U8 pbSize;
    HI_U8 gi;

    HI_U8 codeRate;
    HI_U8 is_tmi_mode;
    HI_U8 tmi_index;
    HI_U8 rsvd;
}tmi_table_info_st;

typedef struct
{
    HI_U32 data[16];
    HI_U8 rsvd[4];
}tone_mask_table_info_st;

typedef struct
{
    HI_U8 data[512];
    HI_U8 rsvd[4];
}power_loading_table_info_st;


//խ�����ķ�������
typedef struct
{
    HI_U8  sc_mode;                         //խ������ģʽ
    HI_U8  modulation_mode;            //���Ʒ�ʽ
    HI_U16 frame_len;                       //֡��

    HI_U8 code_rate;                        //����
    HI_U8 bts_offset;
    HI_U8 pad1[2];
}stub_sc_frame_tx_req_st;

//խ�����Ľ���ͳ�Ʋ�ѯ����
typedef struct
{
    HI_U8 is_query;                         //�Ƿ��ѯ��1:��ѯ��0�����
    HI_U8 pad[3];
}stub_sc_rx_static_req_st;

//խ�����Ľ���ͳ�Ʋ�ѯӦ��
typedef struct
{
    HI_U32 tx_end_cnt;                      //��������ж�ͳ��
    HI_U32 pcs_cnt;                         //�ز������ж�ͳ��
    HI_U32 rx_end_cnt;                      //��������ж�ͳ
    HI_U32 crc_err_cnt;             //��������ж�ͳ��

    HI_U32 rx_state_err_cnt;        //����״̬����
    HI_U32 rx_len_err_cnt;          //���ձ��ĳ��ȴ���
    HI_U32 rx_data_err_cnt;         //���ձ������ݴ���

    HI_U32 pad[3];
}stub_sc_rx_static_rsp_st;

//���ز�NTB ����
typedef struct
{
    HI_U8 is_ntb ;
    HI_U8 bts_offset;
    HI_U16 pkt_cnt ;

    HI_U8 data[60];
}stub_sc_ntb_pkt_st;

//���ز����ͱ���ά���ϱ�
typedef struct
{
    HI_U32 send_cnt;                        //���ͱ��ĵļ���
    HI_U32 cur_ntb_high;                         //����ʱ��NTB
    HI_U32 cur_ntb_low;                         //����ʱ��NTB
    HI_U32 pkt_ntb;                         //����������NTB
    HI_U32 cur_ltmr_high;                        //����ʱ��ltmr
    HI_U32 cur_ltmr_low;                        //����ʱ��ltmr

    HI_U32 pad[4];
}stub_sc_tx_frame_rpt_st;


//���ز����ձ���ά���ϱ�
typedef struct
{
    HI_U32 send_cnt_in_pkt;                 //���ձ����еķ��ͼ���
    HI_U32 rcv_cnt;                         //���ձ��ļ���
    HI_U32 cur_ntb_high;                         //����ʱ��NTB�ĸ�λ
    HI_U32 cur_ntb_low;                         //����ʱ��NTB�ĵ�λ

    HI_U32 cur_ltmr_high;                        //����ʱ��ltmr�ĸ�λ
    HI_U32 cur_ltmr_low;                        //����ʱ��ltmr�ĵ�λ
    HI_U32 lock_ntb_high;                        //������NTB�ĸ�λ
    HI_U32 lock_ntb_low;                        //������NTB�ĵ�λ

    HI_U32 lock_ltmr_high;                       //������ltmr�ĸ�λ
    HI_U32 lock_ltmr_low;                       //������ltmr�ĵ�λ
    HI_U32 pkt_ntb;                         //����������NTB
    HI_U32 fec_clk_delay;                   //����ģ�����ʱ

    HI_U16 phy_rcv_cnt;
    HI_S16 freq_err;
    HI_U32 pad[2];
}stub_sc_rx_frame_rpt_st;

typedef struct
{
       HI_U32 fream_time;
       HI_U32 send_time;
}stub_sc_frame_time_rpt_st;
typedef struct
{
       HI_U32 pcs_report_time;
       HI_U32 sys_ok_int_tim;
}stub_sc_pcs_rpt_st;
typedef struct
{
    HI_U8 is_rcv_filter_en;
    HI_U8 pad;
    HI_U16 pad2;

    HI_U16 expect_snid;
    HI_U16 expect_stei;
}stub_rx_rcv_filter_st;

/************************************************************
                             �ӿں�������
 ************************************************************/

extern HI_VOID stub_init(HI_VOID);
/* BEGIN: Added by likunhe/00194117, 2012/4/1   ���ⵥ��:DTS2012040100709*/
/*****************************************************************************
 �� �� ��  : GetToneMapFixed
 ��������  : ���ù̶�ToneMapʱ�Ĳ��Բ���
 �������  : HI_VOID
 �������  : ��
 �� �� ֵ  : HI_VOID
 ���������õĺ���  :
 ���ñ������ĺ���  :
 ���ʵ�ȫ�ֱ���  :
 �޸ĵ�ȫ�ֱ���  :
 �޸ĵ�BBP�Ĵ����б�  :

 �޸���ʷ      :
  1.��    ��   : 2012��3��31��
    ��    ��   : likunhe/00194117
    �޸�����   : �����ɺ���

*****************************************************************************/
extern HI_VOID GetToneMapFixed(HI_VOID);

/*****************************************************************************
 �� �� ��  : GetToneMapBitloading
 ��������  : �����Զ�ToneMapʱ�Ĳ��Բ���
 �������  : HI_VOID
 �������  : ��
 �� �� ֵ  : HI_VOID
 ���������õĺ���  :
 ���ñ������ĺ���  :
 ���ʵ�ȫ�ֱ���  :
 �޸ĵ�ȫ�ֱ���  :
 �޸ĵ�BBP�Ĵ����б�  :

 �޸���ʷ      :
  1.��    ��   : 2012��3��31��
    ��    ��   : likunhe/00194117
    �޸�����   : �����ɺ���

*****************************************************************************/
HI_EXTERN HI_VOID GetToneMapBitloading(HI_VOID);
/* END:   Added by likunhe/00194117, 2012/4/1 */

/* BEGIN: Added by likunhe/00194117, 2012/6/30   PN:DTS2012062705423*/
/*****************************************************************************
 �� �� ��  : stub_chip_select_init
 ��������  : оƬɸѡ���ܳ�ʼ��
 �������  : HI_VOID
 �������  : ��
 �� �� ֵ  : HI_VOID
 ���������õĺ���  :
 ���ñ������ĺ���  :
 ���ʵ�ȫ�ֱ���  :
 �޸ĵ�ȫ�ֱ���  :
 �޸ĵ�BBP�Ĵ����б�  :

 �޸���ʷ      :
  1.��    ��   : 2012��6��30��
    ��    ��   : likunhe/00194117
    �޸�����   : �����ɺ���

*****************************************************************************/
HI_EXTERN HI_VOID stub_chip_select_init(HI_VOID);
/* END:   Added by likunhe/00194117, 2012/6/30 */
/* BEGIN: Added by likunhe/00194117, 2012/7/10   PN:DTS2012062705407*/

/* BEGIN: Modified by liujian 00280874, 2016/5/3   ���ⵥ��:MAC_PHY_TEST*/
HI_EXTERN HI_VOID stub_statistic_interrupt_info_report(stub_statistic_interrupt_info_st* stub_statistic_interrupt_info);
/* END:   Modified by liujian 00280874, 2016/5/3 */

HI_EXTERN void stub_statistic_info_report(stub_statistic_info_switch_st *p_static_info_switch);
/* END:   Added by likunhe/00194117, 2012/7/10 */

/* BEGIN: Added by sunhaiyang/00238140, 2015/6/23   PN:B4 NTB*/
HI_EXTERN HI_VOID ntb_b4_clear_ntb_table(HI_VOID);
/* END:   Added by sunhaiyang/00238140, 2015/6/23 */
HI_VOID stub_rx_increase_isp_send_cnt(HI_VOID);

HI_VOID stub_rx_disable_isp_tx_en(HI_VOID);

#ifdef __cplusplus
}
#endif

#endif
