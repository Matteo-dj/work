/******************************************************************************

                  版权所有 (C), 2001-2011, 华为技术有限公司

 ******************************************************************************
  文 件 名   : phy_om_interface.h
  版 本 号   : 初稿
  作    者   : songshuchen 40236
  生成日期   : 2012年2月2日
  最近修改   :
  功能描述   : 可维可测信息上报接口
  函数列表   :
  修改历史   :
  1.日    期   : 2012年2月2日
    作    者   : songshuchen 40236
    修改内容   : 创建文件
--------------------------------------------------------------------------------

******************************************************************************/
#ifndef PHY_OM_INTERFACE_H
#define PHY_OM_INTERFACE_H

/************************************************************
                     包含其它模块的头文件
************************************************************/
#include "sal_inf.h"
//#include "c_struct_define.h"
#include "dfx_sys.h"


#ifdef __cplusplus
extern "C"{
#endif


/************************************************************
                               宏定义
************************************************************/
//HSO下发的OM信息的ID
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

//内部统计量查询ID
#define PHY_OM_REQ_ID_BASE            (0x01a0)
#define QUERY_AFE_CONFIG_INFO_REQ       (PHY_OM_REQ_ID_BASE + 0x00)
#define ID_DIAG_CMD_NTB_SOFT_INFO       (PHY_OM_REQ_ID_BASE + 0x04)
#define ID_DIAG_CMD_QUERY_NTB_STAT      (PHY_OM_REQ_ID_BASE + 0x05)
#define PHY_OM_REQ_ID_MAX             ID_DIAG_CMD_QUERY_NTB_STAT

//HSO收到的OM信息的ID
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


#define      OM_BLER_TIMER_ID           1       //OM的Bler定时器的ID
#define      OM_PREAMBLE_STATE_TIMER_ID 1       //OM的Preamble的定时器的ID

#define      OM_REPORT_ENABLE           1
#define      OM_REPORT_DISABLE          0

#define OM_FILTRATE_TEI_ENABLE          1       //Tei过滤使能
#define OM_FILTRATE_TEI_DISABLE         0       //Tei过滤禁止

#define      TONE_MAP_NUM               512

/************************************************************
                             数据结构定义
************************************************************/
typedef struct
{
    HI_U32  Ntb;                                     //当前的时间
    HI_S8  aagc_gain_rpt_c_aagc_gain_rpt;            //AAGC增益
    HI_U8  Rsd0;                                     //保留
    HI_U16 rssi_prev_db_rpt_c_regrssi1db;            //AD输出平均功率

    HI_U32 rssi_prev_afe_rpt_c_afepower;             //估计的AD直流值
    HI_U16 rssi_post_db_rpt_c_rssi_post_db_rpt;      //滤波后信号平均功率
    HI_U16 rssi_prev_sat_rpt_c_regstarate;           //上报饱和率

    HI_S16 dagc_gain_rpt_c_dagc_gain_rpt;            //DAGC增益
    HI_U8  aagc_fsm_curr_c_aagc_fsm_curr;            //AAGC内部状态的状态
    HI_U8  agc_gain_rpt_c_agc_gain_rpt;              //AGC调整增益

    HI_U16 rssi_prev_psiga_c_rssi_prev_psiga;        //快调阶段最后N点功率
    HI_U16 rssi_prev_psigb_c_rssi_prev_psigb;        //快调阶段最后2N到N点功率

    HI_U16 rssi_prev_psat_c_rssi_prev_psat;          //快调阶段饱和并行窗统计功率
    HI_U16 rssi_post_psig_c_rssi_post_psig;          //滤波后快速统计功率
} AGC_DCOC_STRUC;
//SNAP_STRUC  OM_SNAP1_REQ
typedef struct
{
    HI_U32  Ntb;                               //当前的时间

    HI_U8  snap_ctrl_c_snap_ctrl;              //快照控制信号
    HI_U8 Rsd0;
    HI_U16 Rsd1;

/* END:   Modified by jiangyuhan/00290021, 2015/5/8 */
} SNAP_STRUC;


// STATE_STRUC  OM_ STATE_REQ
typedef struct
{
    HI_U32  Ntb;                                                    //当前的时间

    HI_U16 state2_cnt_c_state2_cnt;                     //进入帧监听state2 的计数
    HI_U16 state3_cnt_c_state3_cnt;                     //进入帧监听state3 的计数

    HI_U16 state4_cnt_c_state4_cnt;                    //进入帧监听state4 的计数
    HI_U16 para_det_ok_cnt_c_para_det_ok_cnt; //并行检测成功的计数
/* END:   Modified by jiangyuhan/00290021, 2015/5/8 */
} STATE_STRUC;


//SNAP1_STRUC  OM_SNAP1_REQ
typedef struct
{
    HI_U32  Ntb;                                                       //当前的时间
    HI_U8   fft_num_c_fft_num;                                //preamble 中用多少1024块作FFT
    HI_U8   Rsd0;
    HI_U16 s2t4_num_c_s2t4;                                 //从state2 到state4 的25M 采样点数
    HI_U16 wait_fc_sample_c_wait_fc_sample;        //滤波后信号平均功率
    HI_U16 Rsd1;
/* END:   Modified by jiangyuhan/00290021, 2015/5/8 */
} SNAP1_STRUC;

//SNAP2_STRUC  OM_SNAP2_REQ
typedef struct
{
    HI_U32  Ntb;                                                        //当前的时间
    HI_U16  state4_rssi1_c_state4_rssi1;                    //统计到state4 截止的RSSI1功率
    HI_U16  state4_rssi2_c_state4_rssi2;                    //统计到state4 截止的RSSI2功率
    HI_S16  pream_aacg_gain_c_pream_aagc_gain;    //快调AAGC增益
    HI_S16  dagc_gain_c_dagc_gain;                          //state4 的DAGC增益
    HI_S16  aagc_gain_c_aagc_gain;                          //state4 的AAGC增益
    HI_S16  Rsd0;
    /* END:   Modified by jiangyuhan/00290021, 2015/5/8 */
} SNAP2_STRUC;


//SNAP3_STRUC  OM_SNAP3_REQ
typedef struct
{
    HI_U32  Ntb;                                                     //当前的时间
    HI_U8   freq_par1_c_freq_par1;                        //频段1 的PAR
    HI_U8   freq_par2_c_freq_par2;                        //频段2 的PAR
    HI_U8   freq_par3_c_freq_par3;                        //频段3 的PAR
    HI_U8   freq_par4_c_freq_par4;                        //频段4 的PAR
    HI_U8   freq_par5_c_freq_par5;                        //频段5 的PAR
    HI_U8   freq_par6_c_freq_par6;                        //频段6 的PAR
    HI_U8   det_freq_mod_c_det_freq_mod;           //并行检测得到的频段模式
    HI_U8   Rsd0;
/* END:   Modified by jiangyuhan/00290021, 2015/5/8 */
} SNAP3_STRUC;

//SNAP4_STRUC  OM_SNAP4_REQ
typedef struct
{
    HI_U32  Ntb;                                            //当前的时间
    HI_U8   nbi_det_mod_c_nbi_det_mod;      //窄带干扰检测是什么时候做的
    HI_U8   Rsd0;
    HI_U16  nbi_s1_c_nbi_s1;                         //窄带干扰起始子载波
    HI_U16  nbi_s2_c_nbi_s2;                         //窄带干扰起始子载波
    HI_U16  nbi_s3_c_nbi_s3;                         //窄带干扰起始子载波
    HI_U16  nbi_d1_c_nbi_d1;                        //窄带干扰截止子载波
    HI_U16  nbi_d2_c_nbi_d2;                        //窄带干扰截止子载波
    HI_U16  nbi_d3_c_nbi_d3;                        //窄带干扰截止子载波
    HI_U8    nbi_cnt_c_nbi_cnt;                      //窄带干扰数量
    HI_U8    Rsd1;
    HI_U32  nbi_pow1_c_nbi_pow1;               //窄带干扰功率
    HI_U32  nbi_pow2_c_nbi_pow2;               //窄带干扰功率
    HI_U32  nbi_pow3_c_nbi_pow3;               //窄带干扰功率
/* END:   Modified by jiangyuhan/00290021, 2015/5/8 */
} SNAP4_STRUC;

typedef struct
{
    HI_U32  Ntb;                                                    //当前的时间
    HI_U16  thresh_snr_c_thresh_snr;                    // 用于采样时间估计的eamble 的符号数
    HI_U8    sco_carrier_num_c_sco_carrier_num;  //用于采样时间估计的选子载波的SNR 门限
    HI_U8   Rsd0;
    HI_S16  pream_freq_err_c_pream_freq_err;    // preamble 鉴频得到的估计值
    HI_U8   sco_use_par_c_sco_use_par;              //  SCO估计对应的PAR
    HI_U8   sco_use_g2_sco_use_g2;                    //  SCO估计对应的积分支路增益
    HI_S16  clk2cubic_c_clk2cubic;                         // SCO输出的用于cubic 插值的频偏
    HI_U16  Rsd1;
/* END:   Modified by jiangyuhan/00290021, 2015/5/8 */
} SNAP5_STRUC;

typedef struct
{
    HI_U32  Ntb;                                                //当前的时间
    HI_U16  first_path_c_first_path;                   // 符号精同步首径距离主径的长度
    HI_U16  path_len_c_path_len;                     // 符号精同步首径多径的长度
/* END:   Modified by jiangyuhan/00290021, 2015/5/8 */
} SNAP6_STRUC;

typedef struct
{
    HI_U32  Ntb;                                                           //当前的时间
    
    HI_U8  fcok_turbo_cnt_c_fcok_turbo_cnt;               // FC译码成功时turbo 迭代次数
    HI_U8  is_insert_cp_c_is_insert_cp;                        // 根据FC译码结果判断在PL中是否插入了syncp
    HI_U8  ispl_in_frame_c_ispl_in_frame;                    // 判断当前帧是否包含payload
    HI_U8  ispl_drop_frame_c_ispl_drop_frame;           // 判断当前帧是否丢掉payload

    HI_U8  ispb1_decode_ok_c_ispb1_decode_ok;        // 是否PL1 译码成功
    HI_U8  ispb2_decode_ok_c_ispb2_decode_ok;        // 是否PL2 译码成功
    HI_U8  pb1ok_turbo_cnt_c_pb1ok_turbo_cnt;        // PL1 译码成功时turbo 迭代次数
    HI_U8  pb2ok_turbo_cnt_c_pb2ok_turbo_cnt;        // PL2 译码成功时turbo 迭代次数
/* END:   Modified by jiangyuhan/00290021, 2015/5/8 */
} SNAP7_STRUC;

//NBI_STRUC
//NBI的滤波器系数
typedef struct
{
    HI_U32 Ntb;

    HI_U8  rpt_nbi_status_c_rpt_nbi_det;        // NBI检测成功与否标志
    HI_U8  rpt_nbi_status_c_rpt_nf_count;       // 采用NF处理的NBI数目、
    HI_U8  rpt_nbi_status_c_rpt_nbi_count;      // 实际检测的NBI总数目
    HI_U8  Rsd0;

    HI_U16 rpt_nf0_coef0_c_rpt_nf0_a1;          // 上报Notch Filter0使能信号a1

    HI_U16 rpt_nf0_coef0_c_rpt_nf0_a2;          // 上报Notch Filter0使能信号a2
    HI_U16 rpt_nf0_coef1_c_rpt_nf0_b0;          // 上报Notch Filter0滤波系数b0
    HI_U16 rpt_nf0_coef1_c_rpt_nf0_b1;          // 上报Notch Filter0滤波系数b1

    HI_U16 rpt_nf0_coef2_c_rpt_nf0_b2;          // 上报Notch Filter0滤波系数b2
    HI_U16 rpt_nf1_coef0_c_rpt_nf1_a1;          // 上报Notch Filter1滤波系数a1
    HI_U16 rpt_nf1_coef0_c_rpt_nf1_a2;          // 上报Notch Filter1滤波系数a2
    HI_U16 rpt_nf1_coef1_c_rpt_nf1_b0;          // 上报Notch Filter1滤波系数b0

    HI_U16 rpt_nf1_coef1_c_rpt_nf1_b1;          // 上报Notch Filter1滤波系数b1
    HI_U16 rpt_nf1_coef2_c_rpt_nf1_b2;          // 上报Notch Filter1滤波系数b2
    HI_U16 rpt_nf2_coef0_c_rpt_nf2_a1;          // 上报Notch Filter2滤波系数a1
    HI_U16 rpt_nf2_coef0_c_rpt_nf2_a2;          // 上报Notch Filter2滤波系数a2

    HI_U16 rpt_nf2_coef1_c_rpt_nf2_b0;          // 上报Notch Filter2滤波系数b0
    HI_U16 rpt_nf2_coef1_c_rpt_nf2_b1;          // 上报Notch Filter2滤波系数b1
    HI_U16 rpt_nf2_coef2_c_rpt_nf2_b2;          // 上报Notch Filter2滤波系数b2
    HI_U8  rpt_nf0_coef0_c_rpt_nf0_en;          // 上报Notch Filter0使能信号
    HI_U8  rpt_nf1_coef0_c_rpt_nf1_en;          // 上报Notch Filter1使能信号

    HI_U8  rpt_nf2_coef0_c_rpt_nf2_en;          // 上报Notch Filter2使能信号
    HI_U8  rpt_nf0_coef0_c_rpt_nf_overflow;     // 上报超过NF处理带宽门限的NBI
    HI_U8  rpt_nbi_fsm_c_rpt_nbi_fsm;           // NBI检测状态机上报
    HI_U8  Rsd1;                                // 保留
}NBI_RPT_NF_STRU;

//NBI的检测状态:包括每个NBI的功率及起止位置
typedef struct
{
    HI_U8  rpt_nbi0_status0_c_rpt_nbi0_en;       //nbi0有效标志
    HI_U8  rpt_nbi1_status0_c_rpt_nbi1_en;       //nbi1有效标志
    HI_U8  rpt_nbi2_status0_c_rpt_nbi2_en;       //nbi2有效标志
    HI_U8  rpt_nbi3_status0_c_rpt_nbi3_en;       //nbi3有效标志

    HI_U8  rpt_nbi4_status0_c_rpt_nbi4_en;       //nbi4有效标志
    HI_U8  rpt_nbi5_status0_c_rpt_nbi5_en;       //nbi5有效标志
    HI_U8  rpt_nbi6_status0_c_rpt_nbi6_en;       //nbi6有效标志
    HI_U8  Rsd1;                                 //保留

    HI_U32 rpt_nbi0_status0_c_rpt_nbi0_pow;      //nbi0噪声功率
    HI_U16 rpt_nbi0_status1_c_rpt_nbi0_end;      //nbi0噪声的结束位置
    HI_U16 rpt_nbi0_status1_c_rpt_nbi0_start;    //nbi0噪声的起始位置

    HI_U32 rpt_nbi1_status0_c_rpt_nbi1_pow;      //nbi1噪声功率
    HI_U16 rpt_nbi1_status1_c_rpt_nbi1_end;      //nbi1噪声的结束位置
    HI_U16 rpt_nbi1_status1_c_rpt_nbi1_start;    //nbi1噪声的起始位置

    HI_U32 rpt_nbi2_status0_c_rpt_nbi2_pow;      //nbi2噪声功率
    HI_U16 rpt_nbi2_status1_c_rpt_nbi2_end;      //nbi2噪声的结束位置
    HI_U16 rpt_nbi2_status1_c_rpt_nbi2_start;    //nbi2噪声的起始位置

    HI_U32 rpt_nbi3_status0_c_rpt_nbi3_pow;      //nbi3噪声功率
    HI_U16 rpt_nbi3_status1_c_rpt_nbi3_end;      //nbi3噪声的结束位置
    HI_U16 rpt_nbi3_status1_c_rpt_nbi3_start;    //nbi3噪声的起始位置

    HI_U32 rpt_nbi4_status0_c_rpt_nbi4_pow;      //nbi4噪声功率
    HI_U16 rpt_nbi4_status1_c_rpt_nbi4_end;      //nbi4噪声的结束位置
    HI_U16 rpt_nbi4_status1_c_rpt_nbi4_start;    //nbi4噪声的起始位置

    HI_U32 rpt_nbi5_status0_c_rpt_nbi5_pow;      //nbi5噪声功率
    HI_U16 rpt_nbi5_status1_c_rpt_nbi5_end;      //nbi5噪声的结束位置
    HI_U16 rpt_nbi5_status1_c_rpt_nbi5_start;    //nbi5噪声的起始位置

    HI_U32 rpt_nbi6_status0_c_rpt_nbi6_pow;      //nbi6噪声功率
    HI_U16 rpt_nbi6_status1_c_rpt_nbi6_end;      //nbi6噪声的结束位置
    HI_U16 rpt_nbi6_status1_c_rpt_nbi6_start;    //nbi6噪声的起始位置
} NBI_RPT_STATUS_STRUC;

//OM_NBI_IND
typedef struct
{
    HI_U32  Ntb;                                     //当前的时间
    NBI_RPT_NF_STRU         NbiRptNf;   //NF滤波器系数组
    NBI_RPT_STATUS_STRUC    NbiRptStatus;
} NBI_STRUC ;

//SYMBOL_ALIGN_STRUC  OM_SYMBOL_ALIGN_IND
typedef struct
{
    HI_U32  Ntb;                                    // 当前的时间

    HI_U16 symbol_align_pathout_c_pathlen;          // 估计出的径长度
    HI_U16 symbol_align_pathout_c_firstpathlen;     // 估计出的首径距最大径的偏移长度

    HI_U8  symbol_align_pathout_c_pream_par;        // 检测出的频段对应的峰均比
    HI_U8  symbol_align_state_c_symbolalignstate;   // 符号同步状态
    HI_U16 Rsd0;                                    // 保留
} SYMBOL_ALIGN_STRUC;

//CLOCK_STRUC  OM_CLOCK_IND
typedef struct
{
    HI_U32 Ntb;                                      //当前的时间
    HI_U16 pream_carrier_cnt_c_pream_carrier_cnt;    //preamble选择的载波个数
    HI_U8  pll_fsm_state_curr_c_pll_fsm_state_curr;  //PLL当前状态 PLLState
    HI_U8  rsd1;                                     //保留

    HI_U16 g1_g2_curr_c_pll_coef_g1;                 //根据PLLState状态上报G1 G2
    HI_U16 g1_g2_curr_c_pll_coef_g2;                 //根据PLLState状态上报G1 G2

    HI_S16 pilot_freq_error;                         //Pilot的频偏值：单位为0.5ppm
    HI_S16 preamble_freq_error;                      //preamble的频偏值：单位为0.5ppm

    HI_S16 sound_freq_error;                         //sound的频偏值：单位为0.5ppm
    HI_S16 freq_error;                               //PLL输出的频偏值：单位为0.5ppm

    HI_S16 ma_freq_error;                            //滑动平均值MAFreqErr频偏值：单位为0.5ppm
    HI_S16 thresh_snr_c_thresh_snr;                  //载波集选择门限

    HI_S16 snr_pavg_c_psnr_avg;                      //平均信噪比
    HI_U8  singu_cnt_c_singu_cnt;                    //clk_est模块的连续异常次数
    HI_U8  rsd0;                                     //保留

    HI_S16 rx_cubic_err_c_rx_cubic_err;              //RX的Cubic频偏
    HI_U16 pream_carrier_cnt_c_pilot_carrier_cnt;    //pilot选择的载波个数
} CLOCK_STRUC ;

//TONEMAP_STRUC  OM_TX_TONE_MAP_IND
typedef struct
{
    HI_U32  Ntb;                                     //当前的时间
    HI_U32  ToneMap[64];                    //64个元素，即(TONE_MAP_NUM >> 3)
    HI_U16  TxPbSymbolNum;                          //发送的PB块符号数
    HI_U16  Rsd;                                    //保留
}TX_TONEMAP_STRUC;

//BLER_STRUC  OM_BLER_IND
typedef struct
{
    HI_U32     Ntb;               //当前的时间
    HI_U32     TotalPbNum;        //累计的PB块个数
    HI_U32     TotalPbRightNum;   //累计正确的PB块个数

    HI_U32     InstantPbNum;      //瞬间的PB块个数
    HI_U32     InstantPbRightNum; //瞬间正确的PB块个数

    HI_U32     TotalFcNum;        //累计的FC个数
    HI_U32     TotalFcRightNum;   //累计正确的FC个数

    HI_U32     InstantFcNum;      //周期的FC总数
    HI_U32     InstantFcRightNum; //周期的FC正确数

    HI_U32     TotalState2Cnt;    //累计的State2
    HI_U32     TotalState3Cnt;    //累计的State3
    HI_U32     TotalState4Cnt;    //累计的State4

    HI_U32     InstantState2Cnt;  //瞬间的State2
    HI_U32     InstantState3Cnt;  //瞬间的State3
    HI_U32     InstantState4Cnt;  //瞬间的State4
}OM_BLER_STRUC;

// pending
//BLER_STRUC  OM_BLER_IND
typedef struct
{
    HI_U32     pad;               //101a0850 值, 每次读取之后需要写1清(并行检测成功的次数)
    //HI_U32     Ntb;
    HI_U32     TotalPbNum;        //累计的PB块个数
    HI_U32     TotalPbRightNum;   //累计正确的PB块个数

    HI_U32     InstantPbNum;      //瞬间的PB块个数
    HI_U32     InstantPbRightNum; //瞬间正确的PB块个数

    HI_U32     TotalFcNum;        //累计的FC个数
    HI_U32     TotalFcRightNum;   //累计正确的FC个数

    HI_U32     InstantFcNum;      //周期的FC总数
    HI_U32     InstantFcRightNum; //周期的FC正确数

    HI_U32     TotalState2Cnt;    //累计的State2
    HI_U32     TotalState3Cnt;    //累计的State3
    HI_U32     TotalState4Cnt;    //累计的State4

    HI_U32     InstantState2Cnt;  //瞬间的State2
    HI_U32     InstantState3Cnt;  //瞬间的State3
    //HI_U32     InstantState4Cnt;  //瞬间的State4
    HI_U32     PcsInterruptCnt;  //Preamble中断次数, ulStatisticInfoRet.ulPcsInterruptCnt
}OM_BLER_STRUC_V200;

typedef struct
{
    HI_U32 Ntb;                                 //当前的时间

    HI_U32 State0ToState1Peak;                  //ST0向ST1跳转时CORR的峰值
    HI_U32 State1ToState2Peak;                  //ST1向ST2跳转时CORR的峰值
    HI_U32 State2ToState3Peak;                  //ST2向ST3跳转时CORR的峰值
    HI_U32 State3ToState4Peak;                  //ST3向ST4跳转时CORR的峰值

    HI_S16 snr_pavg_c_psnr_avg;                 //平均信噪比
    HI_U8  symbol_align_pathout_c_pream_par;    //峰均比
    HI_U8  FcCrcResult;                         //保留

    HI_S16 thresh_snr_c_thresh_snr;             //Snr阈值
    HI_S16 preamble_freq_error;                 //Preamble的频偏

    HI_S16 pll_freq_error;                      //pll的频偏
    HI_U16 pream_carrier_cnt_c_pream_carrier_cnt;   //preamble选择的有效载波个数

    HI_U16 pream_carrier_cnt_c_pilot_carrier_cnt;   //pilot选择的有效载波个数
    HI_U16 g1_g2_curr_c_pll_coef_g1;                //根据PLLState状态上报G1 G2

    HI_U16 g1_g2_curr_c_pll_coef_g2;                //根据PLLState状态上报G1 G2
    HI_S16 ma_freq_error;                           //滑动平均值MAFreqErr频偏值：单位为0.5ppm

    HI_U8  pll_fsm_state_curr_c_pll_fsm_state_curr; //PLL当前状态 PLLState
    HI_U8  singu_cnt_c_singu_cnt;                   //clk_est模块的连续异常次数
    HI_U16 rsd;                                     //保留
}OM_PREAMBLE_STATIC_STRUC;

//SOUNDING_SNR_STRUC  OM_SOUND_SNR_IND
typedef struct
{
    HI_U32  Ntb;                              //当前的时间
    HI_S16  SoundSnr[TONE_MAP_NUM];         //估计出的Sounding SNR
} SOUND_SNR_STRUC ;

//PREAMBLE_SNR_STRUC  OM_PREAMBLE_SNR_IND
typedef struct
{
    HI_U32  Ntb;                              //当前的时间
    HI_S16  PreambleSnr[TONE_MAP_NUM];       //估计出的preamble SNR
} PREAMBLE_SNR_STRUC ;

//IMP上报  OM_IMP_IND
typedef struct
{
    HI_U32 Ntb;                              //当前的时间
    HI_U16 td_report_c_imp_data_num;         //检测周期内收到脉冲噪声干扰的载波个数
    HI_S16 td_report_c_dc_estimate;          //估计的AD直流值
} IMP_STRUC ;

//Carrier Sense上报  OM_CARRIER_SENSE_IND
typedef struct
{
    HI_U32 Ntb;                                     //当前的时间
    HI_U8  carrier_sense_rpt_c_carriersensestate;    //监听状态
    HI_U8  carrier_sense_rpt_c_fc_start_flag;
    HI_U8  carrier_sense_rpt_c_pream_fft_num;        //可做FFT的符号个数
    HI_U8  Rsd;                                      //保留
} CARRIER_SENSE_STRUC;

//本地监听峰值个数超过K时的状态上报(中断)  OM_CARRIER_SENSE_PEAK_IND
typedef struct
{
    HI_U32 Ntb;                                     //当前的时间
    HI_U32 PeakOverCnt;
}CARRIER_SENSE_PEAK_ISR_STRUC;

//Ntb上报值
typedef struct
{
    HI_U32 Ntb;                                      //当前的时间

    HI_U16 delta_dbg_reg3_c_freq_err_singu_th;       //当前所用的异常判断门限
    HI_S16 delta_dbg_reg3_c_ntb_ma_freq_err_dly;     //上一次的滑动平均值

    HI_U8  delta_dbg_reg4_c_ntb_singu_cnt;           //当前状态下的异常值个数
    HI_U8  ntb_state_c_ntb_curr_state;               //NTB PLL当前状态 NTBState
    HI_S16 freq_err_reg_c_clk_err_out;               //clk与NTB的最终频偏计算结果

    HI_U16 k4_rpt_reg_c_k4w0;                        //当前iir滤波器中所用的k4w0
    HI_U16 k4_rpt_reg_c_k4wf;                        //当前iir滤波器中所用的k4wf
    HI_U32 ltmr_timer_reg_c_ltmr_n;                  //本地定时器，频率为905/3600，上报值为每次得到bts时本地定时器的值
    HI_U32 bts_out_reg_c_bts_out;                    //当前解出的bts值

    HI_U32 delta_dbg_reg0_c_delta_ltmr;              //两个LTmr时间差
    HI_S16 delta_dbg_reg1_c_delta_freq_err;          //NTB的前后两次频偏差
    HI_S16 delta_dbg_reg1_c_ntb_freq_err_est;        //用相邻两个BTS与LTmr计算获得的偏差

    HI_S16 delta_dbg_reg2_c_ntb_pll_coef_g1;         //根据NTBState状态上报NTB PLL中的G1 G2
    HI_S16 delta_dbg_reg2_c_ntb_pll_coef_g2;         //根据NTBState状态上报NTB PLL中的G1 G2

    HI_U32 delta_dbg_reg5_c_freq_offset;             //第n次BTS与LTM的时间差
    HI_U32 delta_bts_reg_c_delta_bts;                //两个BTS时间差
    HI_U32 ntb_freq_offset0_c_ntb_freq_offset;       //NTB的时间偏差
    HI_S32 ntb_freq_offset1_c_ntb_freq_offset;

    HI_S16 delta_ntbfreqerr_c_delta_ntb_freq_error;  //NTB的频偏差滤波后的频偏值：单位为1ppm
    HI_S16 ntb_freq_error_c_ntb_freq_err;            //NTB的频偏值：单位为1ppm
    HI_S16 freq_err_reg_c_ntb_freq_err_af_pll;      //NTB的PLL输出：单位为1ppm
    HI_S16 rsd;
}NTB_STRUC;

//bitloading的上报数据
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

//bitloading的上报值
typedef struct
{
    HI_U32 Ntb;
    HI_U32 BitSum;                          //比特分配的总和
    BITLOADINT_DATA_STRUC DataPacket;
}BITLOADINT_STRUC;

//ISP的上报值
typedef struct
{
    HI_U32 Ntb;
    HI_U32 rpt_isp_seq0_c_rpt_isp_seq0;      //每次判决时五种相位相关值序列中每种相位相关值序列的最大值
    HI_U32 rpt_isp_seq1_c_rpt_isp_seq1;      //每次判决时五种相位相关值序列中每种相位相关值序列的最大值
    HI_U32 rpt_isp_seq2_c_rpt_isp_seq2;      //每次判决时五种相位相关值序列中每种相位相关值序列的最大值
    HI_U32 rpt_isp_seq3_c_rpt_isp_seq3;      //每次判决时五种相位相关值序列中每种相位相关值序列的最大值
    HI_U32 rpt_isp_seq4_c_rpt_isp_seq4;      //每次判决时五种相位相关值序列中每种相位相关值序列的最大值
    HI_U32 rpt_isp_mout_c_rpt_isp_mout;      //含有最大值相位的相关值序列窗口外最大值
    HI_U32 rpt_isp_soutl_c_rpt_isp_soutl;    //含有最大值相位的相关值序列窗口外和值
    HI_U32 rpt_isp_south_c_rpt_isp_south;    //含有最大值相位的相关值序列窗口外和值
}ISP_STRUC;

//FC上报的内容
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

//TX完成时上报的内容
typedef struct
{
    HI_U32 Ntb;

    HI_U8  Dt;                                  //帧类型
    HI_U8  PbNum;                               //PB块的数目
    HI_U16 SymbolNum;                           //发送的符号数

    HI_U8  PilotFlag;                           //发送Pilot的标识
    HI_U8  ExtPayloadMode;                      //扩展TMI
    HI_U16 PayloadMode;                         //TMI标识

    HI_S16 TxCubicFreqErr;                      //TX端的Cubic频偏
    HI_U8  freq;                                //保留
    HI_U8  pad;

    HI_U32 BitloadingAddr;                      //发送时采用的ToneMap
}OM_TX_REPORT_STRU;

typedef struct
{
    HI_U16 Switch;                  //上报开关
    HI_U8  FiltrateTeiSiwtch;       //保留
    HI_U8  Rsd0;

    HI_U16 Stei;                    //需要上报的Tei
    HI_U16 Rsd1;
}OM_REPORT_SWITCH_STRU;

//HSO下发的BLER上报的开关
typedef struct
{
    OM_REPORT_SWITCH_STRU ReportSwitch;     //上报开关
    HI_U32 Period;                           //定时上报的周期，单位为ms
}OM_BLER_SWITCH_STRU;

//HSO下发的Preamble State上报的开关
typedef struct
{
    OM_REPORT_SWITCH_STRU ReportSwitch;     //上报开关
    HI_U32 Period;                           //定时上报的周期，单位为ms
}OM_PREAMBLE_STATE_SWITCH_STRU;

//周期上报的开关
typedef struct
{
    OM_REPORT_SWITCH_STRU ReportSwitch;     //上报开关
    HI_U32 Period;                           //定时上报的周期，单位为ms
}OM_PERIOD_SWITCH_STRU;


//Bitloading的内部的可维可测变量
typedef struct
{
    HI_U32 MaxBitError;
    HI_U32 BitloadingToneMapError;
} BITLOADING_INNER_ERROR_STRU;

//OM的内部错误计数
typedef struct
{
    HI_U32 ReqIdError;                  //HSO下发的查询ID错误
    HI_U32 ReportError;                 //可维可测信息上报错误
}OM_INNER_ERROR_STRU;

/* BEGIN: Added by sunhaiyang/00238140, 2015/6/23   PN:B4 NTB*/
// B4 测试上报HSO的结构体
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

//B4测试，上报当前状态
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
    HI_U32 bpc;                         //时隙所属的BPC周期
    HI_U32 starttime;                  // 时隙开始的相对时间，单位:1微妙
    HI_U32 duration;                   // 时隙持续时间，单位:1微妙

    HI_U16 tei;                        // 时隙对应的tei
    HI_U8  slot_type;                  // 时隙类型
    HI_U8  lid;                        // 时隙对应的lid

    HI_U8  phase;                      // 时隙相位(CSMA时隙存在)
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
    HI_U32 nbe;                   // 信标条目数
    HI_U16 beacon_fc_snid;        // 信标FC中SNID
    HI_U16 beacon_snid;           // 信标中SNID

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
    HI_U32 snid        : 4;     // 该位用于标记PB的snid
    HI_U32 rc          : 4;     // 站点重启计数
    HI_U32 hopcnt      : 8;
    HI_U32 send_type   : 3;     // 新增，记录发送策略
    HI_U32 send_count  : 5;     // 新增，记录重发次数
    HI_U32 packet_index : 16;
    HI_U8  oda[HI_PLC_MAC_ADDR_LEN];
    HI_U8  rsvd[10];
} DIAG_CMD_MAC_HEAD_LONG_STRU;

#define DETAIL_STAT_STA_FREQ_MODE_NUM (6)

typedef  struct
{
    HI_U16  tei;            //同步对象的TEI
    HI_U16  CntSingu4NTB;   //芯片根据滑动平均判断的同步对象的异常计数

    HI_U32  last_rcv_LTmr_8;
    HI_U32  last_rcv_LTmr_32;
    HI_U32  last_rcv_CBTS;
    HI_U32  last_rcv_timestamp;

    HI_U8   freqmode;       //同步对象的频段编号
    HI_U8   pad[3];
    HI_S64  freqoffset;     //同步对象的最后一次计算的当前大窗口结束位置BTS与 LTmr的相位偏移，〖NTB_FreqOffset(n)〗_next
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
    HI_U8   best_freq_mode_index ; // 细统计推选同步集合时使用
    HI_U8   is_sorted_freq_mode ;

    detail_stat_sta_freq_mode freq_mode[DETAIL_STAT_STA_FREQ_MODE_NUM];
} detail_stat_sta;

#endif

#if 1
//V100
typedef struct
{
    HI_U32 bts_front;               // 上次接收同步帧的网络时钟基值
    HI_U32 ltmr_front;              // 上次接收同步帧的本地时钟计数
    HI_U32 bts_current;             // 当前接收同步帧的网络时钟基值
    HI_U32 ltmr_current;            // 当前接收同步帧的本地时钟计数

    HI_U32 synch_time;              // 上次同步时间

    HI_U8  state;                   // 同步状态
    HI_U8  exception_cnt;           // 异常次数
    HI_S16 freqerr;                 // 计算的频偏值

    HI_U8  synch_identify_type;     // 同步对象标识类型
    HI_U8  synch_mac[6];            // 同步对象的MAC地址
    HI_U8  synch_snid;              // 同步对象的网络号

    HI_U16 synch_tei;               // 同步对象的终端设备标识
    HI_BOOL is_synch;               // 是否同步
    HI_U8  pad;

    HI_U32 total_exception_cnt;     // 总的异常次数
    HI_U32 total_synch_cnt;         // 总的同步成功次数
    HI_U32 synch_timeout_cnt;       // 同步超时次数

    HI_U32 init_total_exception_cnt;// 初始状态总的异常次数
}DIAG_QUERY_SOFT_NTB_STRU;

typedef struct
{
    HI_U32 synch_timeout_reset_cnt;           // 同步超时复位次数
    HI_U32 exception_reset_cnt;               // 同步异常复位次数
    HI_U32 exception_jump_reset_cnt;          // NTB发生频偏跳表异常复位次数
    HI_U32 cylce_exception_reset_cnt;         // 工作周期检测到异常复位
    HI_U32 cylce_detect_no_exception_cnt;     // 工作周期检测无异常
    //HI_U32 cycle_exception_reset_init_cnt;  // 工作周期连续异常次数超过门限的次数
    HI_U32 freqerr_exception_cnt;             // 从寄存器获取频偏判断异常的次数
    HI_U32 chip_state1_exception_reset_cnt;   // 芯片控制下，NTB在状态1的异常复位次数
    HI_U32 chip_state1_exception_cnt;         // 芯片控制下，NTB在状态1的异常次数

    HI_U32 soft_to_chip_no_rcv_beacon_fc_cnt; // 180S内接收不到BEACON FC，交给芯片控制次数
    HI_U32 soft_to_chip_ntb_disable_cnt;      // 300S内NTB未生效交给芯片控制次数
    HI_U32 synch_change_discover_to_proxy_cnt;// 同步站点由发现站点切换为代理站点次数
    HI_U32 synch_change_multi_network_cnt;    // 由于多网络引发同步站点变更

    HI_U32 save_freqerr_fail_cnt;             // 工作周期获取频偏值失败次数
    HI_U32 save_freqerr_success_cnt;          // 工作周期获取频偏成功次数
    HI_U32 soft_reset_cnt;                    // 软复位次数

    HI_U32 chip_state1_cnt;                   // 芯片控制时钟同步在状态1同步次数
    HI_U32 chip_state2_cnt;                   // 芯片控制时钟同步在状态2同步次数
    HI_U32 chip_state3_cnt;                   // 芯片控制时钟同步在状态3同步次数
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
           本文件提供给其他模块的全局变量
************************************************************/


#ifdef __cplusplus
}
#endif

#endif

