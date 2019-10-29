/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : phy_om_interface.h
  �� �� ��   : ����
  ��    ��   : songshuchen 40236
  ��������   : 2012��2��2��
  ����޸�   :
  ��������   : ��ά�ɲ���Ϣ�ϱ��ӿ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2012��2��2��
    ��    ��   : songshuchen 40236
    �޸�����   : �����ļ�
--------------------------------------------------------------------------------

******************************************************************************/
#ifndef PHY_OM_INTERFACE_H
#define PHY_OM_INTERFACE_H

/************************************************************
                     ��������ģ���ͷ�ļ�
************************************************************/
#include "sal_inf.h"
//#include "c_struct_define.h"
#include "dfx_sys.h"


#ifdef __cplusplus
extern "C"{
#endif


/************************************************************
                               �궨��
************************************************************/
//HSO�·���OM��Ϣ��ID
#define      OM_REQ_ID_BASE           (0x0100)
#define      OM_AGC_DCOC_REQ          (OM_REQ_ID_BASE + 0x00)
#define      OM_NBI_REQ               (OM_REQ_ID_BASE + 0x01)
#define      OM_SYMBOL_ALIGN_REQ      (OM_REQ_ID_BASE + 0x02)
#define      OM_CLOCK_REQ             (OM_REQ_ID_BASE + 0x03)
#define      OM_TX_TONE_MAP_REQ       (OM_REQ_ID_BASE + 0x04)
#define      OM_SOUND_SNR_REQ         (OM_REQ_ID_BASE + 0x05)
#define      OM_PREAMBLE_SNR_REQ      (OM_REQ_ID_BASE + 0x06)
#define      OM_IMP_REQ               (OM_REQ_ID_BASE + 0x07)
#define      OM_CARRIER_SENSE_REQ     (OM_REQ_ID_BASE + 0x08)
#define      OM_BITLOADING_REQ        (OM_REQ_ID_BASE + 0x09)
#define      OM_BLER_REQ              (OM_REQ_ID_BASE + 0x0a)
#define      OM_PREAMBLE_STATE_REQ    (OM_REQ_ID_BASE + 0x0b)
#define      OM_NTB_REQ               (OM_REQ_ID_BASE + 0x0c)
#define      OM_ISP_REQ               (OM_REQ_ID_BASE + 0x0d)
#define      OM_FC_REQ                (OM_REQ_ID_BASE + 0x0e)
#define      OM_TX_REQ                (OM_REQ_ID_BASE + 0x0f)
#define      OM_SNAP1_REQ             (OM_REQ_ID_BASE + 0x010)
#define      OM_SNAP2_REQ             (OM_REQ_ID_BASE + 0x011)
#define      OM_SNAP3_REQ             (OM_REQ_ID_BASE + 0x012)
#define      OM_SNAP4_REQ             (OM_REQ_ID_BASE + 0x013)
#define      OM_SNAP5_REQ             (OM_REQ_ID_BASE + 0x014)
#define      OM_SNAP6_REQ             (OM_REQ_ID_BASE + 0x015)
#define      OM_SNAP7_REQ             (OM_REQ_ID_BASE + 0x016)
#define      OM_STATE_REQ             (OM_REQ_ID_BASE + 0x017)
#define      OM_SNAP_REQ              (OM_REQ_ID_BASE + 0x018)
#define      OM_REQ_ID_MAX            (OM_SNAP_REQ)

//�ڲ�ͳ������ѯID
#define PHY_OM_REQ_ID_BASE            (0x01a0)
#define QUERY_AFE_CONFIG_INFO_REQ       (PHY_OM_REQ_ID_BASE + 0x00)
#define ID_DIAG_CMD_NTB_SOFT_INFO       (PHY_OM_REQ_ID_BASE + 0x04)
#define ID_DIAG_CMD_QUERY_NTB_STAT      (PHY_OM_REQ_ID_BASE + 0x05)
#define PHY_OM_REQ_ID_MAX             ID_DIAG_CMD_QUERY_NTB_STAT

//HSO�յ���OM��Ϣ��ID
#define      OM_IND_ID_BASE           (0x0150)
#define      OM_AGC_DCOC_IND          (OM_IND_ID_BASE + 0x00)
#define      OM_NBI_NF_IND            (OM_IND_ID_BASE + 0x01)
#define      OM_SYMBOL_ALIGN_IND      (OM_IND_ID_BASE + 0x02)
#define      OM_CLOCK_IND             (OM_IND_ID_BASE + 0x03)
#define      OM_TX_TONE_MAP_IND       (OM_IND_ID_BASE + 0x04)
#define      OM_BLER_IND              (OM_IND_ID_BASE + 0x05)
#define      OM_PREAM_STATE_IND       (OM_IND_ID_BASE + 0x06)
#define      OM_SOUND_SNR_IND         (OM_IND_ID_BASE + 0x07)
#define      OM_PREAMBLE_SNR_IND      (OM_IND_ID_BASE + 0x08)
#define      OM_IMP_IND               (OM_IND_ID_BASE + 0x09)
#define      OM_CARRIER_SENSE_IND     (OM_IND_ID_BASE + 0x0a)
#define      OM_BITLOADING_PB136_IND  (OM_IND_ID_BASE + 0x0b)
#define      OM_BITLOADING_PB520_IND  (OM_IND_ID_BASE + 0x0c)
#define      OM_CARRIER_SENSE_PEAK_IND (OM_IND_ID_BASE + 0x0d)
#define      OM_NTB_IND               (OM_IND_ID_BASE + 0x0e)
#define      OM_ISP_IND               (OM_IND_ID_BASE + 0x0f)
#define      OM_FC_IND                (OM_IND_ID_BASE + 0x10)
#define      OM_TX_IND                (OM_IND_ID_BASE + 0x11)
#define      OM_NBI_STATUS_IND        (OM_IND_ID_BASE + 0x13)
#define      OM_SNAP1_IND             (OM_IND_ID_BASE + 0x14)
#define      OM_SNAP2_IND             (OM_IND_ID_BASE + 0x15)
#define      OM_SNAP3_IND             (OM_IND_ID_BASE + 0x16)
#define      OM_SNAP4_IND             (OM_IND_ID_BASE + 0x17)
#define      OM_SNAP5_IND             (OM_IND_ID_BASE + 0x18)
#define      OM_SNAP6_IND             (OM_IND_ID_BASE + 0x19)
#define      OM_SNAP7_IND             (OM_IND_ID_BASE + 0x1a)
#define      OM_STATE_IND             (OM_IND_ID_BASE + 0x1b)
/* BEGIN: Added by sunhaiyang/00238140, 2015/6/23   PN:B4 NTB*/
#define      OM_NTB_FREQERROR_PPM_1_IND     (OM_IND_ID_BASE + 0x1f)
#define      OM_NTB_FREQERROR_PPM_2_IND     (OM_IND_ID_BASE + 0x20)
#ifdef PRODUCT_CFG_ONLINE_TEST_MODE
#define      OM_SLOT_PARSE              (OM_IND_ID_BASE + 0x37)
#define      OM_TX_JUDGE_IDLE_2_BACKOFF (OM_IND_ID_BASE + 0x38)
//#define      OM_TX_SEND_PARAM           (OM_IND_ID_BASE + 0x40)
//#define      OM_TX_SET_MPDU_PALOAD      (OM_IND_ID_BASE + 0x42)
#define      OM_DFX_RX_TEST_FRAME       (OM_IND_ID_BASE + 0X43)
//#define      OM_DFX_RX_GT_TEST_FRAME    (OM_IND_ID_BASE + 0X44)
//#define      OM_DFX_TX_REAL_SEND_FREQ   (OM_IND_ID_BASE + 0X45)
//#define      OM_DFX_NTB_DETAIL_STAT_STA (OM_IND_ID_BASE + 0X46)
//#define      OM_DFX_NTB_COARSE_STAT_STA (OM_IND_ID_BASE + 0X47)
//#define      OM_DFX_NTB_SYNC_STA_FREQ_ERR (OM_IND_ID_BASE + 0X48)
//#define      OM_DFX_RCV_STRU            (OM_IND_ID_BASE + 0X49)
#endif
#define      OM_IND_ID_MAX            (OM_NTB_FREQERROR_PPM_2_IND)


#define      OM_BLER_TIMER_ID           1       //OM��Bler��ʱ����ID
#define      OM_PREAMBLE_STATE_TIMER_ID 1       //OM��Preamble�Ķ�ʱ����ID

#define      OM_REPORT_ENABLE           1
#define      OM_REPORT_DISABLE          0

#define OM_FILTRATE_TEI_ENABLE          1       //Tei����ʹ��
#define OM_FILTRATE_TEI_DISABLE         0       //Tei���˽�ֹ

#define      TONE_MAP_NUM               512

/************************************************************
                             ���ݽṹ����
************************************************************/
typedef struct
{
    HI_U32  Ntb;                                     //��ǰ��ʱ��
    HI_S8  aagc_gain_rpt_c_aagc_gain_rpt;            //AAGC����
    HI_U8  Rsd0;                                     //����
    HI_U16 rssi_prev_db_rpt_c_regrssi1db;            //AD���ƽ������

    HI_U32 rssi_prev_afe_rpt_c_afepower;             //���Ƶ�ADֱ��ֵ
    HI_U16 rssi_post_db_rpt_c_rssi_post_db_rpt;      //�˲����ź�ƽ������
    HI_U16 rssi_prev_sat_rpt_c_regstarate;           //�ϱ�������

    HI_S16 dagc_gain_rpt_c_dagc_gain_rpt;            //DAGC����
    HI_U8  aagc_fsm_curr_c_aagc_fsm_curr;            //AAGC�ڲ�״̬��״̬
    HI_U8  agc_gain_rpt_c_agc_gain_rpt;              //AGC��������

    HI_U16 rssi_prev_psiga_c_rssi_prev_psiga;        //����׶����N�㹦��
    HI_U16 rssi_prev_psigb_c_rssi_prev_psigb;        //����׶����2N��N�㹦��

    HI_U16 rssi_prev_psat_c_rssi_prev_psat;          //����׶α��Ͳ��д�ͳ�ƹ���
    HI_U16 rssi_post_psig_c_rssi_post_psig;          //�˲������ͳ�ƹ���
} AGC_DCOC_STRUC;
//SNAP_STRUC  OM_SNAP1_REQ
typedef struct
{
    HI_U32  Ntb;                               //��ǰ��ʱ��

    HI_U8  snap_ctrl_c_snap_ctrl;              //���տ����ź�
    HI_U8 Rsd0;
    HI_U16 Rsd1;

/* END:   Modified by jiangyuhan/00290021, 2015/5/8 */
} SNAP_STRUC;


// STATE_STRUC  OM_ STATE_REQ
typedef struct
{
    HI_U32  Ntb;                                                    //��ǰ��ʱ��

    HI_U16 state2_cnt_c_state2_cnt;                     //����֡����state2 �ļ���
    HI_U16 state3_cnt_c_state3_cnt;                     //����֡����state3 �ļ���

    HI_U16 state4_cnt_c_state4_cnt;                    //����֡����state4 �ļ���
    HI_U16 para_det_ok_cnt_c_para_det_ok_cnt; //���м��ɹ��ļ���
/* END:   Modified by jiangyuhan/00290021, 2015/5/8 */
} STATE_STRUC;


//SNAP1_STRUC  OM_SNAP1_REQ
typedef struct
{
    HI_U32  Ntb;                                                       //��ǰ��ʱ��
    HI_U8   fft_num_c_fft_num;                                //preamble ���ö���1024����FFT
    HI_U8   Rsd0;
    HI_U16 s2t4_num_c_s2t4;                                 //��state2 ��state4 ��25M ��������
    HI_U16 wait_fc_sample_c_wait_fc_sample;        //�˲����ź�ƽ������
    HI_U16 Rsd1;
/* END:   Modified by jiangyuhan/00290021, 2015/5/8 */
} SNAP1_STRUC;

//SNAP2_STRUC  OM_SNAP2_REQ
typedef struct
{
    HI_U32  Ntb;                                                        //��ǰ��ʱ��
    HI_U16  state4_rssi1_c_state4_rssi1;                    //ͳ�Ƶ�state4 ��ֹ��RSSI1����
    HI_U16  state4_rssi2_c_state4_rssi2;                    //ͳ�Ƶ�state4 ��ֹ��RSSI2����
    HI_S16  pream_aacg_gain_c_pream_aagc_gain;    //���AAGC����
    HI_S16  dagc_gain_c_dagc_gain;                          //state4 ��DAGC����
    HI_S16  aagc_gain_c_aagc_gain;                          //state4 ��AAGC����
    HI_S16  Rsd0;
    /* END:   Modified by jiangyuhan/00290021, 2015/5/8 */
} SNAP2_STRUC;


//SNAP3_STRUC  OM_SNAP3_REQ
typedef struct
{
    HI_U32  Ntb;                                                     //��ǰ��ʱ��
    HI_U8   freq_par1_c_freq_par1;                        //Ƶ��1 ��PAR
    HI_U8   freq_par2_c_freq_par2;                        //Ƶ��2 ��PAR
    HI_U8   freq_par3_c_freq_par3;                        //Ƶ��3 ��PAR
    HI_U8   freq_par4_c_freq_par4;                        //Ƶ��4 ��PAR
    HI_U8   freq_par5_c_freq_par5;                        //Ƶ��5 ��PAR
    HI_U8   freq_par6_c_freq_par6;                        //Ƶ��6 ��PAR
    HI_U8   det_freq_mod_c_det_freq_mod;           //���м��õ���Ƶ��ģʽ
    HI_U8   Rsd0;
/* END:   Modified by jiangyuhan/00290021, 2015/5/8 */
} SNAP3_STRUC;

//SNAP4_STRUC  OM_SNAP4_REQ
typedef struct
{
    HI_U32  Ntb;                                            //��ǰ��ʱ��
    HI_U8   nbi_det_mod_c_nbi_det_mod;      //խ�����ż����ʲôʱ������
    HI_U8   Rsd0;
    HI_U16  nbi_s1_c_nbi_s1;                         //խ��������ʼ���ز�
    HI_U16  nbi_s2_c_nbi_s2;                         //խ��������ʼ���ز�
    HI_U16  nbi_s3_c_nbi_s3;                         //խ��������ʼ���ز�
    HI_U16  nbi_d1_c_nbi_d1;                        //խ�����Ž�ֹ���ز�
    HI_U16  nbi_d2_c_nbi_d2;                        //խ�����Ž�ֹ���ز�
    HI_U16  nbi_d3_c_nbi_d3;                        //խ�����Ž�ֹ���ز�
    HI_U8    nbi_cnt_c_nbi_cnt;                      //խ����������
    HI_U8    Rsd1;
    HI_U32  nbi_pow1_c_nbi_pow1;               //խ�����Ź���
    HI_U32  nbi_pow2_c_nbi_pow2;               //խ�����Ź���
    HI_U32  nbi_pow3_c_nbi_pow3;               //խ�����Ź���
/* END:   Modified by jiangyuhan/00290021, 2015/5/8 */
} SNAP4_STRUC;

typedef struct
{
    HI_U32  Ntb;                                                    //��ǰ��ʱ��
    HI_U16  thresh_snr_c_thresh_snr;                    // ���ڲ���ʱ����Ƶ�eamble �ķ�����
    HI_U8    sco_carrier_num_c_sco_carrier_num;  //���ڲ���ʱ����Ƶ�ѡ���ز���SNR ����
    HI_U8   Rsd0;
    HI_S16  pream_freq_err_c_pream_freq_err;    // preamble ��Ƶ�õ��Ĺ���ֵ
    HI_U8   sco_use_par_c_sco_use_par;              //  SCO���ƶ�Ӧ��PAR
    HI_U8   sco_use_g2_sco_use_g2;                    //  SCO���ƶ�Ӧ�Ļ���֧·����
    HI_S16  clk2cubic_c_clk2cubic;                         // SCO���������cubic ��ֵ��Ƶƫ
    HI_U16  Rsd1;
/* END:   Modified by jiangyuhan/00290021, 2015/5/8 */
} SNAP5_STRUC;

typedef struct
{
    HI_U32  Ntb;                                                //��ǰ��ʱ��
    HI_U16  first_path_c_first_path;                   // ���ž�ͬ���׾����������ĳ���
    HI_U16  path_len_c_path_len;                     // ���ž�ͬ���׾��ྶ�ĳ���
/* END:   Modified by jiangyuhan/00290021, 2015/5/8 */
} SNAP6_STRUC;

typedef struct
{
    HI_U32  Ntb;                                                           //��ǰ��ʱ��
    
    HI_U8  fcok_turbo_cnt_c_fcok_turbo_cnt;               // FC����ɹ�ʱturbo ��������
    HI_U8  is_insert_cp_c_is_insert_cp;                        // ����FC�������ж���PL���Ƿ������syncp
    HI_U8  ispl_in_frame_c_ispl_in_frame;                    // �жϵ�ǰ֡�Ƿ����payload
    HI_U8  ispl_drop_frame_c_ispl_drop_frame;           // �жϵ�ǰ֡�Ƿ񶪵�payload

    HI_U8  ispb1_decode_ok_c_ispb1_decode_ok;        // �Ƿ�PL1 ����ɹ�
    HI_U8  ispb2_decode_ok_c_ispb2_decode_ok;        // �Ƿ�PL2 ����ɹ�
    HI_U8  pb1ok_turbo_cnt_c_pb1ok_turbo_cnt;        // PL1 ����ɹ�ʱturbo ��������
    HI_U8  pb2ok_turbo_cnt_c_pb2ok_turbo_cnt;        // PL2 ����ɹ�ʱturbo ��������
/* END:   Modified by jiangyuhan/00290021, 2015/5/8 */
} SNAP7_STRUC;

//NBI_STRUC
//NBI���˲���ϵ��
typedef struct
{
    HI_U32 Ntb;

    HI_U8  rpt_nbi_status_c_rpt_nbi_det;        // NBI���ɹ�����־
    HI_U8  rpt_nbi_status_c_rpt_nf_count;       // ����NF�����NBI��Ŀ��
    HI_U8  rpt_nbi_status_c_rpt_nbi_count;      // ʵ�ʼ���NBI����Ŀ
    HI_U8  Rsd0;

    HI_U16 rpt_nf0_coef0_c_rpt_nf0_a1;          // �ϱ�Notch Filter0ʹ���ź�a1

    HI_U16 rpt_nf0_coef0_c_rpt_nf0_a2;          // �ϱ�Notch Filter0ʹ���ź�a2
    HI_U16 rpt_nf0_coef1_c_rpt_nf0_b0;          // �ϱ�Notch Filter0�˲�ϵ��b0
    HI_U16 rpt_nf0_coef1_c_rpt_nf0_b1;          // �ϱ�Notch Filter0�˲�ϵ��b1

    HI_U16 rpt_nf0_coef2_c_rpt_nf0_b2;          // �ϱ�Notch Filter0�˲�ϵ��b2
    HI_U16 rpt_nf1_coef0_c_rpt_nf1_a1;          // �ϱ�Notch Filter1�˲�ϵ��a1
    HI_U16 rpt_nf1_coef0_c_rpt_nf1_a2;          // �ϱ�Notch Filter1�˲�ϵ��a2
    HI_U16 rpt_nf1_coef1_c_rpt_nf1_b0;          // �ϱ�Notch Filter1�˲�ϵ��b0

    HI_U16 rpt_nf1_coef1_c_rpt_nf1_b1;          // �ϱ�Notch Filter1�˲�ϵ��b1
    HI_U16 rpt_nf1_coef2_c_rpt_nf1_b2;          // �ϱ�Notch Filter1�˲�ϵ��b2
    HI_U16 rpt_nf2_coef0_c_rpt_nf2_a1;          // �ϱ�Notch Filter2�˲�ϵ��a1
    HI_U16 rpt_nf2_coef0_c_rpt_nf2_a2;          // �ϱ�Notch Filter2�˲�ϵ��a2

    HI_U16 rpt_nf2_coef1_c_rpt_nf2_b0;          // �ϱ�Notch Filter2�˲�ϵ��b0
    HI_U16 rpt_nf2_coef1_c_rpt_nf2_b1;          // �ϱ�Notch Filter2�˲�ϵ��b1
    HI_U16 rpt_nf2_coef2_c_rpt_nf2_b2;          // �ϱ�Notch Filter2�˲�ϵ��b2
    HI_U8  rpt_nf0_coef0_c_rpt_nf0_en;          // �ϱ�Notch Filter0ʹ���ź�
    HI_U8  rpt_nf1_coef0_c_rpt_nf1_en;          // �ϱ�Notch Filter1ʹ���ź�

    HI_U8  rpt_nf2_coef0_c_rpt_nf2_en;          // �ϱ�Notch Filter2ʹ���ź�
    HI_U8  rpt_nf0_coef0_c_rpt_nf_overflow;     // �ϱ�����NF����������޵�NBI
    HI_U8  rpt_nbi_fsm_c_rpt_nbi_fsm;           // NBI���״̬���ϱ�
    HI_U8  Rsd1;                                // ����
}NBI_RPT_NF_STRU;

//NBI�ļ��״̬:����ÿ��NBI�Ĺ��ʼ���ֹλ��
typedef struct
{
    HI_U8  rpt_nbi0_status0_c_rpt_nbi0_en;       //nbi0��Ч��־
    HI_U8  rpt_nbi1_status0_c_rpt_nbi1_en;       //nbi1��Ч��־
    HI_U8  rpt_nbi2_status0_c_rpt_nbi2_en;       //nbi2��Ч��־
    HI_U8  rpt_nbi3_status0_c_rpt_nbi3_en;       //nbi3��Ч��־

    HI_U8  rpt_nbi4_status0_c_rpt_nbi4_en;       //nbi4��Ч��־
    HI_U8  rpt_nbi5_status0_c_rpt_nbi5_en;       //nbi5��Ч��־
    HI_U8  rpt_nbi6_status0_c_rpt_nbi6_en;       //nbi6��Ч��־
    HI_U8  Rsd1;                                 //����

    HI_U32 rpt_nbi0_status0_c_rpt_nbi0_pow;      //nbi0��������
    HI_U16 rpt_nbi0_status1_c_rpt_nbi0_end;      //nbi0�����Ľ���λ��
    HI_U16 rpt_nbi0_status1_c_rpt_nbi0_start;    //nbi0��������ʼλ��

    HI_U32 rpt_nbi1_status0_c_rpt_nbi1_pow;      //nbi1��������
    HI_U16 rpt_nbi1_status1_c_rpt_nbi1_end;      //nbi1�����Ľ���λ��
    HI_U16 rpt_nbi1_status1_c_rpt_nbi1_start;    //nbi1��������ʼλ��

    HI_U32 rpt_nbi2_status0_c_rpt_nbi2_pow;      //nbi2��������
    HI_U16 rpt_nbi2_status1_c_rpt_nbi2_end;      //nbi2�����Ľ���λ��
    HI_U16 rpt_nbi2_status1_c_rpt_nbi2_start;    //nbi2��������ʼλ��

    HI_U32 rpt_nbi3_status0_c_rpt_nbi3_pow;      //nbi3��������
    HI_U16 rpt_nbi3_status1_c_rpt_nbi3_end;      //nbi3�����Ľ���λ��
    HI_U16 rpt_nbi3_status1_c_rpt_nbi3_start;    //nbi3��������ʼλ��

    HI_U32 rpt_nbi4_status0_c_rpt_nbi4_pow;      //nbi4��������
    HI_U16 rpt_nbi4_status1_c_rpt_nbi4_end;      //nbi4�����Ľ���λ��
    HI_U16 rpt_nbi4_status1_c_rpt_nbi4_start;    //nbi4��������ʼλ��

    HI_U32 rpt_nbi5_status0_c_rpt_nbi5_pow;      //nbi5��������
    HI_U16 rpt_nbi5_status1_c_rpt_nbi5_end;      //nbi5�����Ľ���λ��
    HI_U16 rpt_nbi5_status1_c_rpt_nbi5_start;    //nbi5��������ʼλ��

    HI_U32 rpt_nbi6_status0_c_rpt_nbi6_pow;      //nbi6��������
    HI_U16 rpt_nbi6_status1_c_rpt_nbi6_end;      //nbi6�����Ľ���λ��
    HI_U16 rpt_nbi6_status1_c_rpt_nbi6_start;    //nbi6��������ʼλ��
} NBI_RPT_STATUS_STRUC;

//OM_NBI_IND
typedef struct
{
    HI_U32  Ntb;                                     //��ǰ��ʱ��
    NBI_RPT_NF_STRU         NbiRptNf;   //NF�˲���ϵ����
    NBI_RPT_STATUS_STRUC    NbiRptStatus;
} NBI_STRUC ;

//SYMBOL_ALIGN_STRUC  OM_SYMBOL_ALIGN_IND
typedef struct
{
    HI_U32  Ntb;                                    // ��ǰ��ʱ��

    HI_U16 symbol_align_pathout_c_pathlen;          // ���Ƴ��ľ�����
    HI_U16 symbol_align_pathout_c_firstpathlen;     // ���Ƴ����׾�����󾶵�ƫ�Ƴ���

    HI_U8  symbol_align_pathout_c_pream_par;        // ������Ƶ�ζ�Ӧ�ķ����
    HI_U8  symbol_align_state_c_symbolalignstate;   // ����ͬ��״̬
    HI_U16 Rsd0;                                    // ����
} SYMBOL_ALIGN_STRUC;

//CLOCK_STRUC  OM_CLOCK_IND
typedef struct
{
    HI_U32 Ntb;                                      //��ǰ��ʱ��
    HI_U16 pream_carrier_cnt_c_pream_carrier_cnt;    //preambleѡ����ز�����
    HI_U8  pll_fsm_state_curr_c_pll_fsm_state_curr;  //PLL��ǰ״̬ PLLState
    HI_U8  rsd1;                                     //����

    HI_U16 g1_g2_curr_c_pll_coef_g1;                 //����PLLState״̬�ϱ�G1 G2
    HI_U16 g1_g2_curr_c_pll_coef_g2;                 //����PLLState״̬�ϱ�G1 G2

    HI_S16 pilot_freq_error;                         //Pilot��Ƶƫֵ����λΪ0.5ppm
    HI_S16 preamble_freq_error;                      //preamble��Ƶƫֵ����λΪ0.5ppm

    HI_S16 sound_freq_error;                         //sound��Ƶƫֵ����λΪ0.5ppm
    HI_S16 freq_error;                               //PLL�����Ƶƫֵ����λΪ0.5ppm

    HI_S16 ma_freq_error;                            //����ƽ��ֵMAFreqErrƵƫֵ����λΪ0.5ppm
    HI_S16 thresh_snr_c_thresh_snr;                  //�ز���ѡ������

    HI_S16 snr_pavg_c_psnr_avg;                      //ƽ�������
    HI_U8  singu_cnt_c_singu_cnt;                    //clk_estģ��������쳣����
    HI_U8  rsd0;                                     //����

    HI_S16 rx_cubic_err_c_rx_cubic_err;              //RX��CubicƵƫ
    HI_U16 pream_carrier_cnt_c_pilot_carrier_cnt;    //pilotѡ����ز�����
} CLOCK_STRUC ;

//TONEMAP_STRUC  OM_TX_TONE_MAP_IND
typedef struct
{
    HI_U32  Ntb;                                     //��ǰ��ʱ��
    HI_U32  ToneMap[64];                    //64��Ԫ�أ���(TONE_MAP_NUM >> 3)
    HI_U16  TxPbSymbolNum;                          //���͵�PB�������
    HI_U16  Rsd;                                    //����
}TX_TONEMAP_STRUC;

//BLER_STRUC  OM_BLER_IND
typedef struct
{
    HI_U32     Ntb;               //��ǰ��ʱ��
    HI_U32     TotalPbNum;        //�ۼƵ�PB�����
    HI_U32     TotalPbRightNum;   //�ۼ���ȷ��PB�����

    HI_U32     InstantPbNum;      //˲���PB�����
    HI_U32     InstantPbRightNum; //˲����ȷ��PB�����

    HI_U32     TotalFcNum;        //�ۼƵ�FC����
    HI_U32     TotalFcRightNum;   //�ۼ���ȷ��FC����

    HI_U32     InstantFcNum;      //���ڵ�FC����
    HI_U32     InstantFcRightNum; //���ڵ�FC��ȷ��

    HI_U32     TotalState2Cnt;    //�ۼƵ�State2
    HI_U32     TotalState3Cnt;    //�ۼƵ�State3
    HI_U32     TotalState4Cnt;    //�ۼƵ�State4

    HI_U32     InstantState2Cnt;  //˲���State2
    HI_U32     InstantState3Cnt;  //˲���State3
    HI_U32     InstantState4Cnt;  //˲���State4
}OM_BLER_STRUC;

// pending
//BLER_STRUC  OM_BLER_IND
typedef struct
{
    HI_U32     pad;               //101a0850 ֵ, ÿ�ζ�ȡ֮����Ҫд1��(���м��ɹ��Ĵ���)
    //HI_U32     Ntb;
    HI_U32     TotalPbNum;        //�ۼƵ�PB�����
    HI_U32     TotalPbRightNum;   //�ۼ���ȷ��PB�����

    HI_U32     InstantPbNum;      //˲���PB�����
    HI_U32     InstantPbRightNum; //˲����ȷ��PB�����

    HI_U32     TotalFcNum;        //�ۼƵ�FC����
    HI_U32     TotalFcRightNum;   //�ۼ���ȷ��FC����

    HI_U32     InstantFcNum;      //���ڵ�FC����
    HI_U32     InstantFcRightNum; //���ڵ�FC��ȷ��

    HI_U32     TotalState2Cnt;    //�ۼƵ�State2
    HI_U32     TotalState3Cnt;    //�ۼƵ�State3
    HI_U32     TotalState4Cnt;    //�ۼƵ�State4

    HI_U32     InstantState2Cnt;  //˲���State2
    HI_U32     InstantState3Cnt;  //˲���State3
    //HI_U32     InstantState4Cnt;  //˲���State4
    HI_U32     PcsInterruptCnt;  //Preamble�жϴ���, ulStatisticInfoRet.ulPcsInterruptCnt
}OM_BLER_STRUC_V200;

typedef struct
{
    HI_U32 Ntb;                                 //��ǰ��ʱ��

    HI_U32 State0ToState1Peak;                  //ST0��ST1��תʱCORR�ķ�ֵ
    HI_U32 State1ToState2Peak;                  //ST1��ST2��תʱCORR�ķ�ֵ
    HI_U32 State2ToState3Peak;                  //ST2��ST3��תʱCORR�ķ�ֵ
    HI_U32 State3ToState4Peak;                  //ST3��ST4��תʱCORR�ķ�ֵ

    HI_S16 snr_pavg_c_psnr_avg;                 //ƽ�������
    HI_U8  symbol_align_pathout_c_pream_par;    //�����
    HI_U8  FcCrcResult;                         //����

    HI_S16 thresh_snr_c_thresh_snr;             //Snr��ֵ
    HI_S16 preamble_freq_error;                 //Preamble��Ƶƫ

    HI_S16 pll_freq_error;                      //pll��Ƶƫ
    HI_U16 pream_carrier_cnt_c_pream_carrier_cnt;   //preambleѡ�����Ч�ز�����

    HI_U16 pream_carrier_cnt_c_pilot_carrier_cnt;   //pilotѡ�����Ч�ز�����
    HI_U16 g1_g2_curr_c_pll_coef_g1;                //����PLLState״̬�ϱ�G1 G2

    HI_U16 g1_g2_curr_c_pll_coef_g2;                //����PLLState״̬�ϱ�G1 G2
    HI_S16 ma_freq_error;                           //����ƽ��ֵMAFreqErrƵƫֵ����λΪ0.5ppm

    HI_U8  pll_fsm_state_curr_c_pll_fsm_state_curr; //PLL��ǰ״̬ PLLState
    HI_U8  singu_cnt_c_singu_cnt;                   //clk_estģ��������쳣����
    HI_U16 rsd;                                     //����
}OM_PREAMBLE_STATIC_STRUC;

//SOUNDING_SNR_STRUC  OM_SOUND_SNR_IND
typedef struct
{
    HI_U32  Ntb;                              //��ǰ��ʱ��
    HI_S16  SoundSnr[TONE_MAP_NUM];         //���Ƴ���Sounding SNR
} SOUND_SNR_STRUC ;

//PREAMBLE_SNR_STRUC  OM_PREAMBLE_SNR_IND
typedef struct
{
    HI_U32  Ntb;                              //��ǰ��ʱ��
    HI_S16  PreambleSnr[TONE_MAP_NUM];       //���Ƴ���preamble SNR
} PREAMBLE_SNR_STRUC ;

//IMP�ϱ�  OM_IMP_IND
typedef struct
{
    HI_U32 Ntb;                              //��ǰ��ʱ��
    HI_U16 td_report_c_imp_data_num;         //����������յ������������ŵ��ز�����
    HI_S16 td_report_c_dc_estimate;          //���Ƶ�ADֱ��ֵ
} IMP_STRUC ;

//Carrier Sense�ϱ�  OM_CARRIER_SENSE_IND
typedef struct
{
    HI_U32 Ntb;                                     //��ǰ��ʱ��
    HI_U8  carrier_sense_rpt_c_carriersensestate;    //����״̬
    HI_U8  carrier_sense_rpt_c_fc_start_flag;
    HI_U8  carrier_sense_rpt_c_pream_fft_num;        //����FFT�ķ��Ÿ���
    HI_U8  Rsd;                                      //����
} CARRIER_SENSE_STRUC;

//���ؼ�����ֵ��������Kʱ��״̬�ϱ�(�ж�)  OM_CARRIER_SENSE_PEAK_IND
typedef struct
{
    HI_U32 Ntb;                                     //��ǰ��ʱ��
    HI_U32 PeakOverCnt;
}CARRIER_SENSE_PEAK_ISR_STRUC;

//Ntb�ϱ�ֵ
typedef struct
{
    HI_U32 Ntb;                                      //��ǰ��ʱ��

    HI_U16 delta_dbg_reg3_c_freq_err_singu_th;       //��ǰ���õ��쳣�ж�����
    HI_S16 delta_dbg_reg3_c_ntb_ma_freq_err_dly;     //��һ�εĻ���ƽ��ֵ

    HI_U8  delta_dbg_reg4_c_ntb_singu_cnt;           //��ǰ״̬�µ��쳣ֵ����
    HI_U8  ntb_state_c_ntb_curr_state;               //NTB PLL��ǰ״̬ NTBState
    HI_S16 freq_err_reg_c_clk_err_out;               //clk��NTB������Ƶƫ������

    HI_U16 k4_rpt_reg_c_k4w0;                        //��ǰiir�˲��������õ�k4w0
    HI_U16 k4_rpt_reg_c_k4wf;                        //��ǰiir�˲��������õ�k4wf
    HI_U32 ltmr_timer_reg_c_ltmr_n;                  //���ض�ʱ����Ƶ��Ϊ905/3600���ϱ�ֵΪÿ�εõ�btsʱ���ض�ʱ����ֵ
    HI_U32 bts_out_reg_c_bts_out;                    //��ǰ�����btsֵ

    HI_U32 delta_dbg_reg0_c_delta_ltmr;              //����LTmrʱ���
    HI_S16 delta_dbg_reg1_c_delta_freq_err;          //NTB��ǰ������Ƶƫ��
    HI_S16 delta_dbg_reg1_c_ntb_freq_err_est;        //����������BTS��LTmr�����õ�ƫ��

    HI_S16 delta_dbg_reg2_c_ntb_pll_coef_g1;         //����NTBState״̬�ϱ�NTB PLL�е�G1 G2
    HI_S16 delta_dbg_reg2_c_ntb_pll_coef_g2;         //����NTBState״̬�ϱ�NTB PLL�е�G1 G2

    HI_U32 delta_dbg_reg5_c_freq_offset;             //��n��BTS��LTM��ʱ���
    HI_U32 delta_bts_reg_c_delta_bts;                //����BTSʱ���
    HI_U32 ntb_freq_offset0_c_ntb_freq_offset;       //NTB��ʱ��ƫ��
    HI_S32 ntb_freq_offset1_c_ntb_freq_offset;

    HI_S16 delta_ntbfreqerr_c_delta_ntb_freq_error;  //NTB��Ƶƫ���˲����Ƶƫֵ����λΪ1ppm
    HI_S16 ntb_freq_error_c_ntb_freq_err;            //NTB��Ƶƫֵ����λΪ1ppm
    HI_S16 freq_err_reg_c_ntb_freq_err_af_pll;      //NTB��PLL�������λΪ1ppm
    HI_S16 rsd;
}NTB_STRUC;

//bitloading���ϱ�����
typedef struct
{
    HI_U32 gi:16;
    HI_U32 rsvd1:16;

    HI_U32 tmp_factor_nopilot:16;
    HI_U32 tmp_shift_x_nopilot:16;

    HI_U32 tmp_factor_pilot:16;
    HI_U32 tmp_shift_x_pilot:16;

    HI_U32 rsvd2;
    HI_U8  data[256];
}BITLOADINT_DATA_STRUC;

//bitloading���ϱ�ֵ
typedef struct
{
    HI_U32 Ntb;
    HI_U32 BitSum;                          //���ط�����ܺ�
    BITLOADINT_DATA_STRUC DataPacket;
}BITLOADINT_STRUC;

//ISP���ϱ�ֵ
typedef struct
{
    HI_U32 Ntb;
    HI_U32 rpt_isp_seq0_c_rpt_isp_seq0;      //ÿ���о�ʱ������λ���ֵ������ÿ����λ���ֵ���е����ֵ
    HI_U32 rpt_isp_seq1_c_rpt_isp_seq1;      //ÿ���о�ʱ������λ���ֵ������ÿ����λ���ֵ���е����ֵ
    HI_U32 rpt_isp_seq2_c_rpt_isp_seq2;      //ÿ���о�ʱ������λ���ֵ������ÿ����λ���ֵ���е����ֵ
    HI_U32 rpt_isp_seq3_c_rpt_isp_seq3;      //ÿ���о�ʱ������λ���ֵ������ÿ����λ���ֵ���е����ֵ
    HI_U32 rpt_isp_seq4_c_rpt_isp_seq4;      //ÿ���о�ʱ������λ���ֵ������ÿ����λ���ֵ���е����ֵ
    HI_U32 rpt_isp_mout_c_rpt_isp_mout;      //�������ֵ��λ�����ֵ���д��������ֵ
    HI_U32 rpt_isp_soutl_c_rpt_isp_soutl;    //�������ֵ��λ�����ֵ���д������ֵ
    HI_U32 rpt_isp_south_c_rpt_isp_south;    //�������ֵ��λ�����ֵ���д������ֵ
}ISP_STRUC;

//FC�ϱ�������
typedef struct
{
    HI_U32 Ntb;
    HI_U16 SymbolNum;
    HI_U16 Rsd;
}OM_FC_REPORT_STRU;

typedef struct
{
    HI_U32 freqIndex;
    HI_U32 SymbolNum;
    HI_U32 pbNum;
    HI_U32 tmi;
    HI_U32 extTmi;
}OM_SYMBOL_NUM_ERR_STRU;

//TX���ʱ�ϱ�������
typedef struct
{
    HI_U32 Ntb;

    HI_U8  Dt;                                  //֡����
    HI_U8  PbNum;                               //PB�����Ŀ
    HI_U16 SymbolNum;                           //���͵ķ�����

    HI_U8  PilotFlag;                           //����Pilot�ı�ʶ
    HI_U8  ExtPayloadMode;                      //��չTMI
    HI_U16 PayloadMode;                         //TMI��ʶ

    HI_S16 TxCubicFreqErr;                      //TX�˵�CubicƵƫ
    HI_U8  freq;                                //����
    HI_U8  pad;

    HI_U32 BitloadingAddr;                      //����ʱ���õ�ToneMap
}OM_TX_REPORT_STRU;

typedef struct
{
    HI_U16 Switch;                  //�ϱ�����
    HI_U8  FiltrateTeiSiwtch;       //����
    HI_U8  Rsd0;

    HI_U16 Stei;                    //��Ҫ�ϱ���Tei
    HI_U16 Rsd1;
}OM_REPORT_SWITCH_STRU;

//HSO�·���BLER�ϱ��Ŀ���
typedef struct
{
    OM_REPORT_SWITCH_STRU ReportSwitch;     //�ϱ�����
    HI_U32 Period;                           //��ʱ�ϱ������ڣ���λΪms
}OM_BLER_SWITCH_STRU;

//HSO�·���Preamble State�ϱ��Ŀ���
typedef struct
{
    OM_REPORT_SWITCH_STRU ReportSwitch;     //�ϱ�����
    HI_U32 Period;                           //��ʱ�ϱ������ڣ���λΪms
}OM_PREAMBLE_STATE_SWITCH_STRU;

//�����ϱ��Ŀ���
typedef struct
{
    OM_REPORT_SWITCH_STRU ReportSwitch;     //�ϱ�����
    HI_U32 Period;                           //��ʱ�ϱ������ڣ���λΪms
}OM_PERIOD_SWITCH_STRU;


//Bitloading���ڲ��Ŀ�ά�ɲ����
typedef struct
{
    HI_U32 MaxBitError;
    HI_U32 BitloadingToneMapError;
} BITLOADING_INNER_ERROR_STRU;

//OM���ڲ��������
typedef struct
{
    HI_U32 ReqIdError;                  //HSO�·��Ĳ�ѯID����
    HI_U32 ReportError;                 //��ά�ɲ���Ϣ�ϱ�����
}OM_INNER_ERROR_STRU;

/* BEGIN: Added by sunhaiyang/00238140, 2015/6/23   PN:B4 NTB*/
// B4 �����ϱ�HSO�Ľṹ��
typedef struct
{
    HI_U8 snid;
    HI_U8 freq;
    HI_U16 tei;

    HI_S32 FreqError;
    HI_S32 FreqErrorave;

    HI_U32 CBTSn0;
    HI_U32 CBTSn1;
    HI_U32 LTmrn0;
    HI_U32 LTmrn1;
}DIAG_CMD_NTB_FreqError_STRU;

//B4���ԣ��ϱ���ǰ״̬
typedef struct
{
    HI_U8 ntb_status;
    HI_U8 pad[3];
}DIAG_CMD_NTB_STATUS_STRU;

typedef struct
{
    HI_U16 ntb_freqerror_ppm_interval_1[50];
}DIAG_CMD_NTB_FREQERROR_PPM_1_STRU;

typedef struct
{
    HI_U16 ntb_freqerror_ppm_interval_2[50];
}DIAG_CMD_NTB_FREQERROR_PPM_2_STRU;

typedef struct
{
    HI_U16 tei :12;
    HI_U16 snid : 4;
    HI_S16 freqerr;
}DIAG_CMD_NTB_NEIGHBOR_FREQERR_IND_STRU;

typedef struct
{
    HI_U32 ImpStatSymCnt;
    HI_U32 ImpStatInd1Cnt;
    HI_U32 ImpStatInd2Cnt;
    HI_U32 ImpStatInd3Cnt;
    HI_U32 ImpStatSet0WinInd1Cnt;
    HI_U32 ImpStatSet0WinInd2Cnt;
    HI_U32 ImpStatSet0WinInd3Cnt;
} OM_REPORT_IMP_FCPL_STAT_STRU;

#ifdef PRODUCT_CFG_ONLINE_TEST_MODE
typedef struct
{
    HI_U8 snid;
    HI_U8 freq;
    HI_U16 tei;

    HI_U32 CBTS;
    HI_U32 LTmrn;
}DIAG_CMD_NTB_RCV_BEACON_STRU;

typedef struct
{
    HI_U16 tei;
    HI_U8  freq;
    HI_U8  pad;

    HI_U32 old_cbts_32;
    HI_U32 old_ltmr_32;
    HI_U32 old_ltmr_8;
    HI_U32 cbts_32;
    HI_U32 ltmr_32;
    HI_U32 ltmr_8;

    HI_U32 freq_offset_32;
    HI_U32 freq_offset_8;
    HI_S16 freq_err;
    HI_U16 pad1;

    HI_U32 min_beacon_period;
    HI_U32 max_beacon_period;
} DIAG_CMD_CHECK_ENOUGH_BCN_STRU;


typedef struct
{
    HI_U8 fc[16];
}DIAG_CMD_RX_FC_STRU;

typedef struct
{
    HI_U32 ntb_reinit_cnt;
}DIAG_CMD_NTB_REINIT_STRU;

typedef struct
{
    HI_U8 freq_mode;
    HI_U8 is_robust_mode;
    HI_U8 tmi;
    HI_U8 ext_tmi;
}DIAG_CMD_TX_BCN_INFO_STRU;

typedef struct
{
    HI_U32 tdma_type_cnt;
    HI_U32 update_slot_table_cnt;
    HI_U32 notify_tx_cnt;
    HI_U32 start_time;
    HI_U16 beacon_slot_cnt;
    HI_U16 central_beacon_slot_cnt;
    HI_U32 csma_slot_offset;
    HI_U32 csma_slot_cnt;
    HI_U32 csma_slot_start_time ;
    HI_U32 csma_slot_start_index;
}DIAG_CMD_BCN_PARSE_STRU;

typedef struct
{
    HI_U32 tx_period_refreshed_cnt;
    HI_U32 tx_period_timeout_cnt;
    HI_U32 tx_mpdu_queued_in_cnt;
    HI_U32 tx_vcs_state_changed_cnt;
    HI_U32 tx_back_off_cnt;
    HI_U32 tx_send_finished;

    HI_U32 period_status;
    HI_U32 period_timeout;
}DIAG_CMD_TX_SLOT_STRU;

typedef struct
{
    HI_U32 bpc;                         //ʱ϶������BPC����
    HI_U32 starttime;                  // ʱ϶��ʼ�����ʱ�䣬��λ:1΢��
    HI_U32 duration;                   // ʱ϶����ʱ�䣬��λ:1΢��

    HI_U16 tei;                        // ʱ϶��Ӧ��tei
    HI_U8  slot_type;                  // ʱ϶����
    HI_U8  lid;                        // ʱ϶��Ӧ��lid

    HI_U8  phase;                      // ʱ϶��λ(CSMAʱ϶����)
    HI_U8  rsvd[3];
}DIAG_CMD_TX_SLOT_INFO_STRU;

typedef struct
{
    HI_U32 period_status;
    HI_U32 current_time;
    HI_U32 starttime;
    HI_U32 endtime;
    HI_U32 duration;
    HI_U16 use_index;
    HI_U16 slot_count;
    HI_U32 s_relative_starttime;

    // DIAG_CMD_TX_SLOT_INFO_STRU slot_info;
}DIAG_CMD_TX_SLOT_REFRESH;

typedef struct
{
    HI_U32 nbe;                   // �ű���Ŀ��
    HI_U16 beacon_fc_snid;        // �ű�FC��SNID
    HI_U16 beacon_snid;           // �ű���SNID

    HI_U32 beacon_crc_value;
    HI_U32 beacon_crc_calc_tmp;
    HI_U32 pb_size;

}DIAG_CMD_SLOT_PARSE;

typedef struct
{
    HI_U8 freq_mode;
    HI_U8 is_robust_mode;
    HI_U8 tmi;
    HI_U8 ext_tmi;
}DIAG_CMD_TX_SOF_INFO_STRU;

typedef struct
{
    HI_U8 fc[16];
}DIAG_CMD_TX_BUILT_BCN_FC_STRU;

typedef struct
{
    HI_U8 freq ;
    HI_U8 snid ;
    HI_U16 tei ;
    HI_U32 CBTSn_32;
    HI_U32 LTmrn_32;
    HI_U32 LTmrn_8;
    HI_U32 phy_snap_data1;
}DIAG_CMD_NTB_HANDLE_BCN_STRU;

typedef struct
{
    HI_U32 mac_type    : 3;
    HI_U32 ctei        : 12;
    HI_U32 ee_ef       : 1;
    HI_U32 msdu_length : 16;
    HI_U32 ats;
    HI_U32 odtei       : 12;
    HI_U32 ostei       : 12;
    HI_U32 snid        : 4;     // ��λ���ڱ��PB��snid
    HI_U32 rc          : 4;     // վ����������
    HI_U32 hopcnt      : 8;
    HI_U32 send_type   : 3;     // ��������¼���Ͳ���
    HI_U32 send_count  : 5;     // ��������¼�ط�����
    HI_U32 packet_index : 16;
    HI_U8  oda[HI_PLC_MAC_ADDR_LEN];
    HI_U8  rsvd[10];
} DIAG_CMD_MAC_HEAD_LONG_STRU;

#define DETAIL_STAT_STA_FREQ_MODE_NUM (6)

typedef  struct
{
    HI_U16  tei;            //ͬ�������TEI
    HI_U16  CntSingu4NTB;   //оƬ���ݻ���ƽ���жϵ�ͬ��������쳣����

    HI_U32  last_rcv_LTmr_8;
    HI_U32  last_rcv_LTmr_32;
    HI_U32  last_rcv_CBTS;
    HI_U32  last_rcv_timestamp;

    HI_U8   freqmode;       //ͬ�������Ƶ�α��
    HI_U8   pad[3];
    HI_S64  freqoffset;     //ͬ����������һ�μ���ĵ�ǰ�󴰿ڽ���λ��BTS�� LTmr����λƫ�ƣ���NTB_FreqOffset(n)��_next
}good_synch_sta;

typedef struct
{
    HI_U32  freq_mode_last_rcv_LTmr_8;
    HI_U32  freq_mode_last_rcv_LTmr_32;
    HI_U32  freq_mode_last_rcv_CBTS;
    HI_U32  freq_mode_continue_rcv_cnt;
    HI_U32  freq_mode_TimerR;
} detail_stat_sta_freq_mode;

typedef struct
{
    HI_U16  tei;
    HI_U8   best_freq_mode_index ; // ϸͳ����ѡͬ������ʱʹ��
    HI_U8   is_sorted_freq_mode ;

    detail_stat_sta_freq_mode freq_mode[DETAIL_STAT_STA_FREQ_MODE_NUM];
} detail_stat_sta;

#endif

#if 1
//V100
typedef struct
{
    HI_U32 bts_front;               // �ϴν���ͬ��֡������ʱ�ӻ�ֵ
    HI_U32 ltmr_front;              // �ϴν���ͬ��֡�ı���ʱ�Ӽ���
    HI_U32 bts_current;             // ��ǰ����ͬ��֡������ʱ�ӻ�ֵ
    HI_U32 ltmr_current;            // ��ǰ����ͬ��֡�ı���ʱ�Ӽ���

    HI_U32 synch_time;              // �ϴ�ͬ��ʱ��

    HI_U8  state;                   // ͬ��״̬
    HI_U8  exception_cnt;           // �쳣����
    HI_S16 freqerr;                 // �����Ƶƫֵ

    HI_U8  synch_identify_type;     // ͬ�������ʶ����
    HI_U8  synch_mac[6];            // ͬ�������MAC��ַ
    HI_U8  synch_snid;              // ͬ������������

    HI_U16 synch_tei;               // ͬ��������ն��豸��ʶ
    HI_BOOL is_synch;               // �Ƿ�ͬ��
    HI_U8  pad;

    HI_U32 total_exception_cnt;     // �ܵ��쳣����
    HI_U32 total_synch_cnt;         // �ܵ�ͬ���ɹ�����
    HI_U32 synch_timeout_cnt;       // ͬ����ʱ����

    HI_U32 init_total_exception_cnt;// ��ʼ״̬�ܵ��쳣����
}DIAG_QUERY_SOFT_NTB_STRU;

typedef struct
{
    HI_U32 synch_timeout_reset_cnt;           // ͬ����ʱ��λ����
    HI_U32 exception_reset_cnt;               // ͬ���쳣��λ����
    HI_U32 exception_jump_reset_cnt;          // NTB����Ƶƫ�����쳣��λ����
    HI_U32 cylce_exception_reset_cnt;         // �������ڼ�⵽�쳣��λ
    HI_U32 cylce_detect_no_exception_cnt;     // �������ڼ�����쳣
    //HI_U32 cycle_exception_reset_init_cnt;  // �������������쳣�����������޵Ĵ���
    HI_U32 freqerr_exception_cnt;             // �ӼĴ�����ȡƵƫ�ж��쳣�Ĵ���
    HI_U32 chip_state1_exception_reset_cnt;   // оƬ�����£�NTB��״̬1���쳣��λ����
    HI_U32 chip_state1_exception_cnt;         // оƬ�����£�NTB��״̬1���쳣����

    HI_U32 soft_to_chip_no_rcv_beacon_fc_cnt; // 180S�ڽ��ղ���BEACON FC������оƬ���ƴ���
    HI_U32 soft_to_chip_ntb_disable_cnt;      // 300S��NTBδ��Ч����оƬ���ƴ���
    HI_U32 synch_change_discover_to_proxy_cnt;// ͬ��վ���ɷ���վ���л�Ϊ����վ�����
    HI_U32 synch_change_multi_network_cnt;    // ���ڶ���������ͬ��վ����

    HI_U32 save_freqerr_fail_cnt;             // �������ڻ�ȡƵƫֵʧ�ܴ���
    HI_U32 save_freqerr_success_cnt;          // �������ڻ�ȡƵƫ�ɹ�����
    HI_U32 soft_reset_cnt;                    // ��λ����

    HI_U32 chip_state1_cnt;                   // оƬ����ʱ��ͬ����״̬1ͬ������
    HI_U32 chip_state2_cnt;                   // оƬ����ʱ��ͬ����״̬2ͬ������
    HI_U32 chip_state3_cnt;                   // оƬ����ʱ��ͬ����״̬3ͬ������
}DIAG_CMD_QUERY_NTB_STATICS_STRU;


#endif

typedef struct
{
    HI_U8 data[64];
}OM_PHY_AFE_CFG_PARA_IND_STRU;

typedef struct
{
    HI_U8 option;
    HI_U8 offset;
    HI_U8 len;
    HI_U8 data;
}OM_PHY_AFE_CFG_PARA_REQ_STRU;


/************************************************************
           ���ļ��ṩ������ģ���ȫ�ֱ���
************************************************************/


#ifdef __cplusplus
}
#endif

#endif

