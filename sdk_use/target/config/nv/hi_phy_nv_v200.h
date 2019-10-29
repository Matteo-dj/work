/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : nv.h
  �� �� ��   : ����
  ��    ��   : ������
  ��������   : 2011��11��15��
  ����޸�   :
  ��������   : PHY_NV
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2011��11��15��
    ��    ��   : ������
    �޸�����   : �����ļ�
--------------------------------------------------------------------------------

******************************************************************************/
#ifndef PHY_NV_V200_H
#define PHY_NV_V200_H

/************************************************************
                     ��������ģ���ͷ�ļ�
************************************************************/
#include "hi_types.h"

#ifdef __cplusplus
extern "C"{
#endif

/************************************************************
                               �궨��
************************************************************/
//NV������ID��Ż�ֵ
#define DIAG_CMD_BASE_NV_ID_V200                 0x0051
#define DIAG_CMD_AGC_PARA_NV_ID_V200             0x0051
#define DIAG_CMD_HREST_NV_ID_V200                0x0052
//#define DIAG_CMD_FEQ_NV_ID_V200                  0x0053
#define DIAG_CMD_CARRIER_SENSE_NV_ID_V200        0x0054
#define DIAG_CMD_TX_NV_ID_V200                   0x0055
#define DIAG_CMD_CLOCK_NV_ID_V200                0x0056
#define DIAG_CMD_TD_PROCESS_NV_ID_V200           0x0057
#define DIAG_CMD_FEC_NV_ID_V200                  0x0058
#define DIAG_CMD_NBI_PARA_NV_ID_V200             0x0059
#define DIAG_CMD_NTB_CALC_NV_ID_V200             0x005a
#define DIAG_CMD_SYMALIGN_NV_ID_V200             0x005c
#define DIAG_CMD_ISP_NV_ID_V200                  0x005d
#define DIAG_CMD_IMPN_PARA_NV_ID_V200            0x005e
#define DIAG_CMD_PLCSC_PARA_NV_ID_V200           0x005f
#define DIAG_CMD_POWER_LOADING_PARA_0_NV_ID_V200           0x0060
#define DIAG_CMD_POWER_LOADING_PARA_1_NV_ID_V200           0x0061
#define DIAG_CMD_PHY_STUB_NV_ID_V200             0x0062
//#define DIAG_CMD_FREQ_PARA_NV_1_ID_V200          0x0063 //DIAG_CMD_FREQ_PARA_NV����չ
#define DIAG_CMD_FREQ_POWER_ID_V200             0x0067
#define DIAG_CMD_AFE_PCS_NV_ID_V200                  0x0068
#define DIAG_CMD_TONEMASK_NV_ID_V200             0x0069
#define DIAG_CMD_TF_REG_PARAM_NV_ID_V200            0x006a  // ̨��ʶ��Ĵ�����������
#define DIAG_CMD_TF_REG_IIR_PARAM_NV_ID_V200           0x006b  // ̨��ʶ��iir�Ĵ�����������
//NV������ID��ŵ����ֵ
#define    DIAG_CMD_MAX_NV_ID_V200               DIAG_CMD_TF_REG_IIR_PARAM_NV_ID_V200

#define FREQ_CH_NUM_MAX                     (13)                     //Ƶ�β��м���ͨ��������
#define MAX_FREQ_NUM                        26                      //Ƶ�θ���,�˺���FREQ_TRY_ORDER_MAX��Ӧ
#define HGT_POWER_LOADING_NUM_MAX           (512)

/************************************************************
                             ���ݽṹ����
************************************************************/

/***************************AGC****************************/
typedef struct
{
    HI_U8 AAGCMode;// AAGC����ģʽ
    HI_U8 ThrsSatPow;// ���͹�������
    HI_U8 PowTarget_Quickv200;
    HI_S8 Fix_AAGC_v200;

    HI_S8 ThresSmall;
    HI_S8 ThresLarge;
    HI_S8 SatSubAAGC_v200;
    HI_S8 InitialAAGC_v200;

    HI_S8 MaxAAGC_v200;
    HI_S8 MinAAGC_v200;
    HI_U8 AagcQuickWinNum;
    HI_U8 AagcUsedWinNum;

    HI_U8 AagcQuicwinSize;
    HI_U8 Diff_Pow_Thresh_v200;
    HI_U16 AGC_set_slot_v200;

    HI_U8 Thrs_preamble_adj_v200;
    HI_U8 DAGC_offset;// DAGCƫ������;
    HI_U8 DAGCByPass;// DAGCʹ��;
    HI_S8 Fix_DAGC_Gain;// �̶�DAGC������������;

    HI_U8 Fix_DAGC_Enable;// �̶�DAGC����ʹ�ܿ���;
    HI_U8 Max_DAGC;// ���DAGC����;
    HI_U8 DagcSt0Target;
    HI_U8 AFEStep;// AFE������������:0������1��code������;1������2��code��������2������1��code��������3������2��code����������ADȷ����AD9866��2��������3�������谴����Ҫ����

    HI_U8 st23useNum;
    HI_U8 st23WinSize;
    HI_U8 SetDagcPow;//�ֶ�����DagcĿ�깦��ʹ�� {0,1}
    HI_U8 DagcPowTarget;//�ֶ�����DagcĿ�깦��

    HI_U8 st23UseNum_post;//�˲���״̬2��3����ͳ��ʹ�ô����� {1~3} ʵ����ֵΪ����ֵ2�ķ���
    HI_U8 st23WinSize_post;//�˲���״̬2��3����ͳ��ʹ�ô����� {5~10}    ʵ����ֵΪ����ֵ2�ķ���
    HI_U8 TXPower;//�����źŷ��书��
    HI_U8 ChanDecScale;//����������������

    HI_U8 FixDagcSt0En;
    HI_U8 FixDagcSt0Gain;
    HI_U8 DagcSt0Thresh;
    HI_U8 DagcSt0WinSize;

    HI_U8 DagcSt0Mode;
    HI_U8 DagcSt0Mode1Thresh;
    HI_S8 DagcSt0Mode1Gain;
    HI_S8 Min_DAGC;

    HI_U8 DagcSt0FlagThresh;
    HI_U8 AagcWaitFlagThres;
    HI_U8 AagcSt1alpha;
    HI_U8 Fix_AAGC_Enable;

    HI_U8 AagcWaitFlagThres_down;
    HI_U8 DagcSt0FlagThresh_down;
    HI_U8 DagcSt0Thresh_down;
    HI_U8 rssi_time_offset;

    HI_U8 Max_DAGC_Det;
    HI_U8 Max_DAGC_St0;
    HI_U16 PreamSatThres[3];                      //Preambleͳ�Ʊ�������1{100,60,20}

    HI_U8 SatAdjEn;                                     //ͳ�ƹ��ʱ���ǿ�Ƹ�λ֡����ʹ��{0}
    HI_U8 AGCcompenrate[3];                       //AGC���油����1{20,15,5}

    HI_U8 DagcSt0TargetDet;                       //dagc״̬0��֡����ͨ��Ŀ�깦��{43}
    HI_U8 Rsvd[3];
}DIAG_CMD_AGC_PARA_NV_STRU_V200;//64byte

/***************************TX****************************/
typedef struct
{
    HI_U32 freqmode_FC_COPY_Offset             :2;         //FC������λģʽ{2,0,2,1,1,1,1,1,1,1,1,1,1}
    HI_U32 FCPLPhaseShift                                 :2;         //FCPL������λ��תģʽ{2,0,2,1,1,1,1,1,1,1,1,1,1}
    HI_U32 PreambleMode                                 :2;         //ǰ�����õ�Ƶ������ģʽ{1,0,1,1,1,1,1,1,1,1,1,1,1}
    HI_U32 GWV200TMIMode                            :1;         //������V200���õ�TMI��ģʽ{1,0,1,0,0,0,0,0,0,0,0,0,0}
    HI_U32 BroadCastMaskEn                            :1;          //BroadCastMaskʹ��{1,1,1,1,1,1,1,1,1,1,1,1,1}
    HI_U32 rsvd                                                   :24;       //����
}DIAG_CMD_TX_NV_FOR_FREQ_STRU_V200;

typedef struct
{
    HI_S8    PowerIndex[4];                                  // Ƶ��1 ~13�ķ���˹���{9,14,17,20}

    HI_U8    FixAfeParamEn;                                //�̶�AFEHpfʹ�ܿ���{0}
    HI_U8    txFreq;                                              // ����Ƶ�α��,ȡֵ:0-25{1}
    HI_U8    RI;                                                      //���������0->0,1->124�� {1}
    HI_U8    toneMaskMode;                              // ToneMaskģʽ��0:����ģʽ��1:ŷ��ģʽ{0}

    HI_U8    PowerloadingEn;                             //{1}
    HI_U8    UseB4AFEParam;                             // AFE�˲���ģʽ���ÿ���{1}
    HI_U8    SyncCpNum;                                    // SyncCp �ĸ���{10}
    HI_U8    Preamble_Only_Ind;                       //�Ƿ�ֻ����Preamble {0}

    HI_U8    TX_Test_En;                                     // ����˲���ģʽʹ��{0}
    HI_U8    TX_Test_Mode;                               // ����˲���ģʽ{0}
    HI_U8    FixAfeParam;                                   //�̶�AFEHpfģʽ{0}
    HI_U8    PreamAddWin;                               //{0}

    HI_U16   GI_FC_D1_D2 ;                                //FC��Payloadǰ�������ŵ�GIֵ ��1->264;2->300;3->400;4->458;5->624��{458}
    HI_U16   GI_Sounding;                                 //{264}

    HI_U16   GI_HR;                                            //{264}
    HI_U16   GI_MR;                                           //{264}

    HI_U16   GI_SR;                                             //{264}
    HI_U8   reducePARdB;                                 //{0}

    HI_U8   fix_freq_en                    : 1;            //���չ̶�Ƶ�ο���(�����м���Ƿ�ر�){0}
    HI_U8   tx_hpf_mode                 : 1;           //0
    HI_U8   tx_lpf_mode                  : 1;           //0
    HI_U8   tx_hpf_en                      : 1;           //0
    HI_U8   reducePAREn                : 1;           //0
    HI_U8   isFreqParaTableEn        : 1;           //Ƶ�β������ñ�ʹ�ܱ�ʶ{1}
    HI_U8   Gain0p8dBBypass          :1;            //Preamble��FC����0.8dB���ʵĹ���Bypass{0}
    HI_U8   Rsvd1                             : 1;           //����{0}

    HI_U16 V200_FC_GI;                                  //V200Ƶ�ε�FC��GI{458}
    HI_U16 V200_D1D2_GI;                            //V200Ƶ�εĵ�1��2��PL���ŵ�GI{458}

    HI_U16 GW_FC_GI;                                    //����Ƶ�ε�FC��GI{458}
    HI_U16 GW_D1D2_GI;                              //����Ƶ�εĵ�1��2��PL���ŵ�GI{458}

    HI_U8 V100ScrambleMode                    :1;        //V100Ƶ�ε�PL����ģʽ{0}
    HI_U8 V200ScrambleMode                    :1;        //V200Ƶ�ε�PL����ģʽ{1}
    HI_U8 V200MPDUScrambleEn               :1;        //V200��MPDU����ʹ��{1}
    HI_U8 GWMPDUScrambleEn                 :1;        //������MPDU����ʹ��{1}
    HI_U8 V200HalfByteShiftMode             :1;        //V200���ֽ���λģʽ{1}
    HI_U8 V200STableMode                        :1;        //V200Turbo��S��ģʽ{1}
    HI_U8 V200ROBOInterleaverMode       :1;        //V200ROBO��֯ģʽ{1}
    HI_U8 GWROBOInterleaverMode         :1;        //����ROBO��֯ģʽ{1}

    HI_U8 V200_1816PuncTureMode          :1;       //V200��16/18���ʴ��ģʽ{1}
    HI_U8 GW_1816PuncTureMode            :1;       //������16/18���ʴ��ģʽ{1}
    HI_U8 Rsvd2                                             :6;
    HI_U8 Par_Th;                                                   //�ж��龯�ķ��������{0}
    HI_U8 Rsvd3;

    DIAG_CMD_TX_NV_FOR_FREQ_STRU_V200 Tx_Param[FREQ_CH_NUM_MAX];    //��Ӧ����ͬ��Ƶ�����C�Ĵ���
}DIAG_CMD_TX_NV_STRU_V200;//84byte

typedef struct
{
    HI_U32 ToneMaskPara[16];
}DIAG_CMD_TONEMASK_NV_STRU_V200;

typedef struct
{
    HI_U32 PowerLoadingTable[32];
}DIAG_CMD_POWER_LOADING_TABLE_NV_STRU_V200;

typedef struct
{
    HI_U16 wf;                              // һ���˲�����
    HI_U16 freqerr_sliding_exception_th;     // Ƶƫ�쳣����

    HI_U16 freqerr_preamble_evalu_diff_th; // preambleƵƫ����ƫ������Χ
    HI_S16 freqerr_ntb_evalu_max_th;       // NTB���Ƶ�֡Ƶƫ����Χ <= 180ppm

    HI_S16 freqerr_ntb_evalu_min_th;       // NTB���Ƶ�֡Ƶƫ����Χ >= -180ppm
    HI_U8 freqerr_exception_cnt_th   ;    // ����Ƶƫ�쳣��������
    HI_U8 sliding_filtering_wnd_size ;    // ����ƽ�����ڴ�С

    HI_U32 rcv_bcn_interval_max_th;         // �ű�֡�������
    HI_U32 rcv_bcn_interval_min_th;         // �ű�֡�������
} DIAG_CMD_NTB_CALC_NV_STRU_V200;

typedef struct
{
    HI_U8 noisecoeff;          //  {2}
    HI_U8 FCMaxIterCount;      // FC ���������� {5}
    HI_U8 ExInfoScale;         //Turbo����extrinsic information����ϵ��{96}
    HI_U8 mmibgain;            //������غ�llr���Ų�ͬʱ����

    HI_U8 PB520SymbolNumThres;
    HI_U8 PB256SymbolNumThres;
    HI_U8 PB136SymbolNumThres;
    HI_U8 PB72SymbolNumThres;

    HI_U8 PLMaxIterCountA;
    HI_U8 PLMaxIterCountB;
    HI_U8 extTMIEnTMI;
    HI_U8 demodmode;

    HI_S16 DefcClkerr_Cubic_offset;
    HI_U16 QAM16_Coef;
}DIAG_CMD_FEC_NV_STRU_V200;// 16

typedef struct
{
    HI_U8 mvNoise_En;             //  ȥ���Ƿ�ʹ��{1}
    HI_U8 mvNoise_Mode;           //  ȥ��ģʽ{0}
    HI_U8 isSetFirstPathLen;      // �Ƿ����ù̶����׾�λ��  {1}
    HI_U8 SetFirstPathLen;        //  ���ù̶����׾�λ��{50}

    HI_U8 SearchPathMethod;       // ��������ģʽ{0}
    HI_U8 minFirstPath;           // �׾�����Сλ�� {40}
    HI_U8 maxFirstPath;           // �׾������λ�� {100}
    HI_U8 minLastPath;            // β������Сλ�� {40}

    HI_U8 maxLastPath;            //   β�������λ�� {100}
    HI_U8 reciNoiseEn;            //  �������䳤�ȵĵ���{10}
    HI_U8 T2;                     // �ŵ�������CIR ��ǰT3 ��ͺ�T2 �����׾���β������ʱCIR ����Ƶ�λ��0 {50}
    HI_U8 T3;                     //�ŵ�������CIR ��ǰT3 ��ͺ�T2 �����׾���β������ʱCIR ����Ƶ�λ��0  {50}

    HI_U8 T4;                     // �ڻ���ƽ����������������ʱ, ����Ϊƽ����������*T4/16 {64}
    HI_U8 T5;                     //  �ڻ�����󾶷���������ʱ, ����Ϊƽ����������*T5/256 {64}
    HI_U8 T6;                     //  �ڼ���ȥ��������ʱͳ������ʹ�õ������������ΪT6{50}
    HI_U8 T7;                     // �ڼ���ȥ��������ʱͳ������ʹ�õ����������յ�Ϊ�ྶ����-T7-1 {50}

    HI_U8 T8;                     // ���ŵ����Ƶ�ʱ��ȥ���У�ȥ��������Ϊ���������ƽ����������*T8/16 {64}
    HI_U8 NullToneNoiseEstEn;
    HI_U8 Rsd2[2];

    HI_U16 FCPLNoisePowEst_alpha;
    HI_U8 NullToneWeight0;
    HI_U8 NullToneWeight1;

    HI_U8 NullToneWeight2;
    HI_U8 NullToneWeight3;
    HI_U8 detLNoiseEn;
    HI_U8 largenoisescale;

    HI_U8 isErasureEn;
    HI_U8 RobustClipscale;
    HI_U8 Rsd[2];
}DIAG_CMD_HREST_NV_STRU_V200;// 40


//�ز������������NV��
typedef struct
{
    HI_U8 Corr1PowScale;                                                 // �����1 �Ļ������� {13}
    HI_U8 Large_Corr1PowScale;                                      //  �����1 �Ĵ�����{35}
    HI_U8 Corr1State4PowScale;                                      //  �����1 ��statr4 ��������{13}
    HI_U8 Large_Corr1State4PowScale;                           //  �����1 ��state4 �Ĵ�����{35}

    HI_U8 Large_Corr1;                                                       // �����жϸ�SNR ״̬�Ĵ�����{75}
    HI_U8 ValidNumScale2;                                                //ÿ·���֡����������صĵ���������ϵ��{70}
    HI_U8 SynCPNumThresh;                                             //��״̬3����״̬4ʱ���ָ����ĳ·���֡����Ҫ���⵽��SynCP����С����{4}
    HI_U8 ValidNumScale2st3;                                          //50

    HI_U8 Small_Corr1State4PowScale;                           // 13
    HI_U8 Corr2PowScale;                                                 // �����2 �Ļ������� {16}{32}
    HI_U8 Corr2State4PowScale;                                      //�����2��State4�Ļ�������{18}{32}
    HI_U8 st3SynCPNumThresh                           :3;                      //�жϽ���״̬4ʱҪ���״̬3��ʼ����ЧSynCP����������{1}
    HI_U8 scale_1_2_3                                          :5;          //corr1_re_1_2_3�����2��3�Ƚ�ʱ��scale{8}

    HI_U32 Corr1SumPowScale                            :7;          //���ڲ���Thres02_03_13��������ֵ�ı���ϵ��{10}
    HI_U32 Large_Corr1SumPowScale                 :7;          //���ڲ���Thres02_03_13������ֵ�ı���ϵ��{10}
    HI_U32 rsd2                                                      :18;

    HI_U32 scale_01_12_23                                   :5;          //corr1_re_01_12_23�����2��3�Ƚ�ʱ��scale{8}
    HI_U32 scale_02_03_13                                   :5;          //corr1_re_02_03_13�����2��3�Ƚ�ʱ��scale{8}
    HI_U32 N1cntThres                                          :6;          //5
    HI_U32 corr2_len_div2                                    :8;          // 104
    HI_U32 corr3_len_div2                                    :8;          // 178
}DIAG_CMD_CARRIER_SENSE_CORR_SCALE_STRU_V200;

typedef struct
{
    HI_U32 FilterOut2Corr1           :5;                     //�˲�������������1��ӳ���ϵ{0}
    HI_U32 FilterOut2Corr2           :5;                     //�˲�������������2��ӳ���ϵ{0}
    HI_U32 FilterOut2Corr3           :5;                     //�˲�������������3��ӳ���ϵ{0}
    HI_U32 FilterOut2Corr4           :5;                     //�˲�������������4��ӳ���ϵ{0}
    HI_U32 FilterOut2Corr5           :5;                     //�˲�������������5��ӳ���ϵ{0}
    HI_U32 FilterOut2Corr6           :5;                     //�˲�������������6��ӳ���ϵ{0}
    HI_U32 Rsvd                             :2;                     // 0
}DIAG_CMD_CARRIER_SENSE_FILTER_OUT_STRU_V200;

typedef struct
{
    HI_U32 corrflt1En                                     : 1;    //��һ·֡����ͨ·�Ŀ��� {1}
    HI_U32 corrflt2En                                     : 1;   //�ڶ�·֡����ͨ·�Ŀ��� {1}
    HI_U32 corrflt3En                                     : 1;   //����·֡����ͨ·�Ŀ��� {1}
    HI_U32 corrflt4En                                     : 1;   //����·֡����ͨ·�Ŀ��� {1}
    HI_U32 corrflt5En                                     : 1;   // 1
    HI_U32 corrflt6En                                     : 1;   // 1

    HI_U32 corrflt1NBparamEn                     :1;   // 1
    HI_U32 corrflt2NBparamEn                     :1;   // 1
    HI_U32 corrflt3NBparamEn                     :1;   // 1
    HI_U32 corrflt4NBparamEn                     :1;   // 1
    HI_U32 corrflt5NBparamEn                     :1;   // 1
    HI_U32 corrflt6NBparamEn                     :1;   // 1

    HI_U32 rsvd                                               :20;
}DIAG_CMD_CARRIER_SENSE_CHANNEL_EN_STRU_V200;

typedef struct
{
    HI_S32 freqindex1                :10;        //-34
    HI_S32 freqindex2                :10;        //-68
    HI_S32 freqindex3                :10;        //-102
    HI_S32 Rsvd                          :2;
}DIAG_CMD_CARRIER_SENSE_FREQ_INDEX_1_3_STRU_V200;

typedef struct
{
    HI_S32 freqindex4                :10;        //-136
    HI_S32 freqindex5                :10;        //-170
    HI_S32 freqindex6                :10;        //-204
    HI_S32 Rsvd                          :2;
}DIAG_CMD_CARRIER_SENSE_FREQ_INDEX_4_6_STRU_V200;

typedef struct
{
    HI_U32 freqmode0filterarray        :6;              // 62
    HI_U32 freqmode1filterarray        :6;              // 60
    HI_U32 freqmode2filterarray        :6;              // 60
    HI_U32 freqmode3filterarray        :6;              // 30
    HI_U32 freqmode4filterarray        :6;              // 3
    HI_U32 Rsvd                                    :2;              // 0
}DIAG_CMD_CARRIER_SENSE_FREQ_FILTER_0_4_STRU_V200;

typedef struct
{
    HI_U32 freqmode5filterarray        :6;              // 12
    HI_U32 freqmode6filterarray        :6;              // 48
    HI_U32 freqmode7filterarray        :6;              // 1
    HI_U32 freqmode8filterarray        :6;              // 2
    HI_U32 freqmode9filterarray        :6;              // 4
    HI_U32 Rsvd                                    :2;              // 0
}DIAG_CMD_CARRIER_SENSE_FREQ_FILTER_5_9_STRU_V200;

typedef struct
{
    HI_U32 freqmode10filterarray        :6;              // 8
    HI_U32 freqmode11filterarray        :6;              // 16
    HI_U32 freqmode12filterarray        :6;              // 32
    HI_U32 Rsvd                                    :14;             // 0
}DIAG_CMD_CARRIER_SENSE_FREQ_FILTER_10_12_STRU_V200;


//�ز�����NV
typedef struct
{
    DIAG_CMD_CARRIER_SENSE_CORR_SCALE_STRU_V200 wbCorrScale;            //�������,{13,35,13,35,75,70,4,50,11,16,18,1,13,35,0}
    DIAG_CMD_CARRIER_SENSE_CORR_SCALE_STRU_V200 nbCorrScale;            //С�������,{13,35,13,35,75,70,4,50,11,32,32,1,13,35,0}
    DIAG_CMD_CARRIER_SENSE_FREQ_INDEX_1_3_STRU_V200 freqindex_1_3;        //ͨ���±�ϵ��1-3 {-34,-68,-102}
    DIAG_CMD_CARRIER_SENSE_FREQ_INDEX_4_6_STRU_V200 freqindex_4_6;        //ͨ���±�ϵ��4-6 {-136,-170,-204}
    DIAG_CMD_CARRIER_SENSE_CHANNEL_EN_STRU_V200 csChlEn;
    DIAG_CMD_CARRIER_SENSE_FREQ_FILTER_0_4_STRU_V200 freqmodefilterarray_0_4;      //�ز�������ͬƵ���µļ��ͨ��ӳ�� {62,60,60,30,3}
    DIAG_CMD_CARRIER_SENSE_FREQ_FILTER_5_9_STRU_V200 freqmodefilterarray_5_9;      //�ز�������ͬƵ���µļ��ͨ��ӳ�� {12,48,1,2,4}
    DIAG_CMD_CARRIER_SENSE_FREQ_FILTER_10_12_STRU_V200 freqmodefilterarray_10_12;   //�ز�������ͬƵ���µļ��ͨ��ӳ�� {8,16,32}
    DIAG_CMD_CARRIER_SENSE_FILTER_OUT_STRU_V200 FilterOut2Corr;     ////�˲�������������n��ӳ���ϵ

    HI_U8 above_large_cont_cnt_Thres;                     //  ֡������SNR/��SNR ״̬�л�ʱ���������� {2}
    HI_U8 DetectFFTNum;                                            // ���ֵ�����趨���޵�1024 ��Ŀ飬������������DetectFFTNum ������״̬2 ����״̬3 {4}
    HI_U16 PowThres;                                                  // �������� {1}

    HI_U16 AGCStableNum;                                        // ����صȴ�AGC �ȶ��ĵ��� {100}
    HI_U8 St4corr2selthres;                                       //210
    HI_U8 State2_max_tol_thres;                             //6

    HI_U8 State2_acc_tol_thres;                              // 3
    HI_U8 State2_cont_tol_thres;                            // 2
    HI_S16 freqindex0;                                          //ͨ���±�ϵ��, [-10]

    HI_U8 HighSNRThresState1Thres            : 4;                           // 4
    HI_U8 SelfdImpEn                                     : 1;                          //֡���������崦��ʹ��{0}
    HI_U8 corr2En                                           : 1;                          // 1
    HI_U8 corr3En                                           : 1;                          // 1
    HI_U8 SelBetterSeldetectEn                  : 1;                           // 1

    HI_U8 st2tost3freqgroupEn                    : 1;                          //  1
    HI_U8 St4TimeOutEn                               : 1;                         // 1
    HI_U8 HighSNRState3DelayParam         : 4;                        // 3
    HI_U8 State3DelayParam                        : 2;                       //ֱ������״̬3��״̬4�Ķ������ӵĳ�ʱʱ��{1}

    HI_U8 ValdNumScale;                                                       //  ֡����0/1/2/3/4 ״̬�л�ʱ���������������{102}
    HI_U8 Large_ValidNumScale;                                          // ֡������SNR/��SNR ״̬�л�ʱ��������������� {120}

    HI_U8 above_large_acc_cnt_Thres;                               //  ֡������SNR/��SNR ״̬�л�ʱ���ۼ����� {3}
    HI_U8 isFixParallelDetect800KFreq;                  //�Ƿ�̶�Ƶ����Դ8��13֮һ���ڲ��м��{0}
    HI_U8 FixParallelDetect800KFreq;                    //�̶�Ƶ����Դ8��13֮һ���ڲ��м��{0}
    HI_U8 isFixParallelDetect1p6MFreq;                //�Ƿ�̶�Ƶ����Դ5��7֮һ���ڲ��м��{0}

    HI_U8 FixParallelDetect1p6MFreq;                  //�̶�Ƶ����Դ5��7֮һ���ڲ��м��{0}
    HI_U8 SelfdImpLog2ThrAvgNum;                    //֡������ƽ������ͳ�Ƶ���{9}
    HI_U8 Rsvd2[2];                                                     // ����0

    HI_U8 Sum_corr1_02_03_13_highThres         :4;                      //corr1_02_03_13��͵������ı������ޱ���ϵ��{15}
    HI_U8 Sum_corr1_02_03_13_lowThres          :4;                      //corr1_02_03_13��͵ĸ����ı������ޱ���ϵ��{15}
    HI_U8 Sum_corr1_02_03_13_satEn                : 1;                     //�ڶ�corr1_02_03_13���ʱ���Ƿ������ʹ���{0}
    HI_U8 SelfdImpSet0WinLen                            : 5;                      //֡������������0����{7}
    HI_U8 Rsvd8                                                      : 2;
    HI_U8 SelfdImpThrScaleH;                                                        //֡�����и����޵ĵ�������{70}
    HI_U8 SelfdImpThrScaleM;                                                       //֡�����������޵ĵ�������{70}

    HI_U32 bypass2corr                                         :1;         //bypassʹ��2������жϺϷ�SynCP�ķ���{0}
    HI_U32 SelfdImpCntSmallValThr                     :9;         //֡����IMP������Сֵ��ֵ{2}
    HI_U32 SelfdImpCntSmallRatThr                    :4;        //֡����IMP������Сֵ����{4}
    HI_U32 Rsvd3                                                   :18;
}DIAG_CMD_CARRIER_SENSE_NV_STRU_V200;//100  byte

typedef struct
{
    HI_U8 LpfEn;                                // ��ͨ�˲���ʹ�� {1}
    HI_U8 HpfBypass;                            // ��ͨ�˲���Bypass{0}

    HI_U8 AutoSelectBypass2                 :1;                    // ���м��ɹ���HPF��LPF�Ƿ��л��˲���,0���л���1�����л�{0}
    HI_U8 AutoSelectBypass3                 :1;                    // 0
    HI_U8 AutoSelectBypass4                 :1;                    // 0
    HI_U8 AutoSelectBypass5                 :1;                    // 0
    HI_U8 AutoSelectBypass6                 :1;                     //0
    HI_U8 AutoSelectBypass7                 :1;                     //0
    HI_U8 AutoSelectBypass8                 :1;                     //0
    HI_U8 AutoSelectBypass9                 :1;                     //0

    HI_U8 AutoSelectBypass10               :1;                     //0
    HI_U8 AutoSelectBypass11               :1;                     //0
    HI_U8 AutoSelectBypass12               :1;                     //0
    HI_U8 AutoSelectBypass13               :1;                     //0
    HI_U8 rsvd1                                          :4;                     //����

    HI_U8 RXCubicEn;                            //RX��Cubic��ֵ�˲���ʹ�ܿ���{1}
    HI_U8 TXCubicEn;                            //TX��Cubic��ֵ�˲���ʹ�ܿ���{1}
    HI_U8 rsvd2[2];                            // ����λ{0}
}DIAG_CMD_TD_PROCESS_NV_STRU_V200;//8byte

/***************************����ͬ��****************************/
typedef struct
{
    HI_U32 datastart                                         :9;                  // Ƶ����Դ1  ~13����ʼ���ز����{80,100,100,61,30,98,166,29,63,97,131,165,199}
    HI_U32 datastop                                          :9;                 // Ƶ����Դ1 ~13 �Ľ�ֹ���ز����{490,230,230,191,93,161,229,60,94,128,162,196,230}
    HI_U32 ValidCarrierNum                             :9;                 // Ƶ����Դ1~13 ����Ч���ز���[411,131,131,131,64,64,64,32,32,32,32,32,32]
    HI_U32 V200_FC_BPSK_En                          :1;                 //V200Ƶ����Դ1  ~13��FCʹ��BPSK��ʹ��{0}
    HI_U32 V200_PreambledB                          :3;                 //Ƶ����Դ1  ~13��Preamble����������dB{0}
    HI_U32 is256IFFT                                          :1;                 //Ƶ����Դ1  ~13��Robustģʽʹ�ܿ���{0}

    HI_U32 LpfMode                                          :4;                // ��ͨ�˲���ѡ��{9},ȡֵ��1��ʼ         {9,8,8,7,2,6,8,1,2,4,6,7,8}
    HI_U32 HpfMode                                         :4;                // ��ͨ�˲���ѡ��{1},��Чȡֵ��1��ʼ{5,6,6,4,2,6,8,2,4,6,7,8,9}
    HI_U32 freqChlMode                                   :2;                // Ƶ�β��м��ͨ��1��2��3��Ƶ��ģʽѡ�񣬣�0��V100��1��V200��2�����ꡣ{2,0,2,1,1,1,1,1,1,1,1,1,1}
                                                                                              // ��PHY���м�����ƣ�freqChlMode[1]��freqChlMode[2]����ͬʱΪ0��ͬʱΪ2
    HI_U32 seqshift                                            :9;               // Ƶ��1~13 ��Preamble Ƶ������V200 ����λֵ{0,0,0,39,66,2,498,63,29,507,473,259,405}
    HI_U32 V200_FCdB                                       :3;               //Ƶ����Դ1  ~13��FC����������dB{0}
    HI_U32 FC_Num                                            :6;              // Ƶ����Դ1~13 ��FC ������{4,8,12,12,16,16,16,32,32,32,32,32,32}
    HI_U32 Rsvd                                                  :4;
}DIAG_CMD_SYMALIGN_FOR_FREQ_STRU_V200;//8byte

typedef struct
{
    HI_U8   PARScale ;                                          // ��������ı������ӣ�������PAR{102}
    HI_U8   PreamFFTIndataMode           :2;       // ����Robustģʽ����{2}
    HI_U8   PreamFFTInNumSel               :4;       //PreamFFT��ǰ���Ÿ���{2}
    HI_U8   Rsvd1                                      :2;       //����
    HI_U8    isFixFreqParallelDetbypass;             // �̶�Ƶ�ε�ģʽ�£����м��Bypass �����ź�{0}
    HI_U8    MaxDetectThresh;                           // 10

    HI_U8    MinDetectThresh;                            // 10
    HI_U8    FCPLTimeDomainCombineEn;         // 1
    HI_U8    PreambleCombineEn;                      // 1
    HI_U8    FreqNBIEn;                                        // 1

    HI_U16  SynCM2ndThres;                               // 190
    HI_U8    FreqNBIThres1;                                 // 8
    HI_U8    FreqNBIThres2;                                 // 5

    HI_U8    FreqNBIThres3;                                 // 5
    HI_U8    FreqNBIThres4;                                 // 10
    HI_U8    FreqNBIThres5;                                 // 5
    HI_U8    FreqNBIThres6;                                 // 8

    HI_U8    BPSK_NoisePowEstEn;                     // 1
    HI_U8    BPSK_NoisePowEst_alpha;              // 16
    HI_U8    LPFMode_Default;                           //��ͨ�˲���ѡ��ȱʡֵ{8}
    HI_U8    HPFMode_Default;                          //��ͨ�˲���ѡ��ȱʡֵ{1}

    DIAG_CMD_SYMALIGN_FOR_FREQ_STRU_V200 Symbol_Align_Param[FREQ_CH_NUM_MAX];
}DIAG_CMD_SYMALIGN_NV_STRU_V200;//124byte

/***************************NBI****************************/
typedef struct
{
    HI_U8    NbiV200En;                               //�Ƿ�ʹ��NBI {1};
    HI_U8    NBIDetModeEn[4];                  //խ����ⳡ��(A.B.C.D)����ʹ��
    HI_U8    NBIFilterEn[3];                         //Notch Filterʹ��{1,1,1};

    HI_U8    NbiAbsThres;                           //  NBI ����������{10}
    HI_U8    NbiWinSize;                             //  NBI ����л�������С{33}
    HI_U8    NbiSubSize;                             //  NBI ����д��ڼ����ز�����{3}
    HI_U8    NF_NBINum;                           //Notch Filter����NBI�������Ŀ  {3};

    HI_U8    NbiStep;                                  // ����NBI����һ����NBI�Ĳ���{10}
    HI_U8    NbiFftNum;                             // ��st3 �����⣬���෽ʽNBI �������FFT ����{2}
    HI_U8    NfV200En;                               // NV ʹ��{1}
    HI_U8    NfPowThres;                           // ��ҪNF �����˲����ݹ�������{15}

    HI_U8    St3NbiV200En;                       // ״̬3 ����Ƿ�ʹ��{1}
    HI_U8    St3NbiDetectScale;               //  ״̬3 ����Ӧ����{15}
    HI_U8    St3NbiWinSize;                     //  ״̬3 ����Ӧ����{49}
    HI_U8    St3NbiSubSize;                      //  ״̬3 ����Ӧ����{3}

    HI_U8    St3NbiStep;                          //  ״̬3 ����Ӧ����{3}
    HI_U8    St3NfV200En;                       //  ״̬3 ����Ӧ����{1}
    HI_U8    St3NfPowThres;                   //  ״̬3 ����Ӧ����{20}
    HI_U8    NfMaxNum;                         //  NF֧�ִ������NBI�Ĵ���(�ز�����) {111}

    HI_U8    NBI_NF_Tn_En;                   //����NF�˲�֮���NBI�Ƿ���д���ʹ�ܱ�־{1}
    HI_U8    St3NbiAbsThres;                 //{15}
    HI_U16   Dfs;                                     //NF�˲����Ĳ���1{350};

    HI_U16   Msdf;                                  //NF�˲����Ĳ���2{64860};
    HI_U16   Bsl;                                     //NF�˲����Ĳ���3{65077};

    HI_U16   Msd;                                   //NF�˲����Ĳ���4{65535};
    HI_U16   Rsd1;                                  //����λ{0}

    HI_U32   R;                                       //NF�˲����Ĳ���5{65077};

    HI_U16   NbiMaskStart1;                // ����NBI Ƶ��1 , ��ʼ�ز�{0}
    HI_U16   NbiMaskStop1;                // ����NBI Ƶ��1 , ��ֹ�ز�{0}

    HI_U16   NbiMaskStart2;                // ����NBI Ƶ��2 , ��ʼ�ز�{0}
    HI_U16   NbiMaskStop2;                 // ����NBI Ƶ��2 , ��ֹ�ز�{0}

    HI_U8    NBITNEn;                           //ToneNull ��������ʹ��{1}
    HI_U8    NbiDetectScale;                //�����ƽ�����ʼ��ϵ����ӣ�{10}
    HI_U16   st3to0_det_cnt;

    HI_U32   Timer_th;                         //��ʱ��������NBI����ʱ�������ޣ���λms��ȱʡΪ300000ms{10000}

    HI_U16   TD_FrameEndDelay;        // 1024
    HI_U16   AGCWaitNBITime;           // 4096

    HI_U8 NfInputShift;                        // 0
    HI_U8 NbiMaxPowScale;                //NBI�����ز����ֵ��������{8}
    HI_U8 NbiMaxPowScaleEMC;         //���ڼ��EMC�ļ����ز����ֵ��������{8}
    HI_U8 Rsvd3;

    HI_U16 NbiStart[3];                         //��0-2���˲�����ʼƵ��{0,0,0}
    HI_U16 NbiStop[3];                          //��0-2���˲�����ֹƵ��{0,0,0}

    HI_U16 NbiSubSizeEMC;                   //����EMC����С������{19}

    HI_U8 NbiFindMaxEn                :1;    //���NBIʱ������ز�λ��ʹ��{1}
    HI_U8 NbiAddCarrier                :2;    //�ڴ�������ز�λ�����Ӽ������{2}
    HI_U8 SetNBIEn                         :1;    //�ֶ�����NBI��ֹ�ز���Χʹ��{0}
    HI_U8 Rsvd1                               :4;
    HI_U8 Rsvd2;

    HI_U32 NbiAbsThresMax;                   //5000
    HI_U32 NbiAbsThresMin;                    //10
}DIAG_CMD_NBI_PARA_NV_STRU_V200;

typedef struct
{
    HI_U8 ImpV200En;            // v200 �°汾��ȥ�������ʹ��
    HI_U8 ImpUsedWinNum;         //  v200 �°汾��ȥ������������ڼ������޵Ĵ�������
    HI_U16 ImpScaleMaxPream;

    HI_U8 ImpMethod;             // v200 �°汾��ȥ��������ж�ʱ��������ŵ�Ĵ���ʽ
    HI_U8 ImpWinSize;
    HI_U8 PrefBuffUsedLen;
    HI_U8 ImpPreamFFTCheckLenOld;

    HI_U8 ImpPreamFFTCheckLenNew;
    HI_U8 ImpMethod2;
    HI_U8 ImpMethod3;
    HI_U8 ImpIndForSymbolEn;

    HI_U16 OneSymbolMaxCnt;
    HI_U16 ImpIndForSymbolThr1;

    HI_U16 ImpIndForSymbolThr2;
    HI_U16 ImpIndForSymbolThr3;

    HI_U8 AddWeightForSymbolNonRobustEn;
    HI_U8 AddWeightForSymbolRobustEn;
    HI_U16 AddWeight1;

    HI_U16 AddWeight2;
    HI_U16 AddWeight3;

    HI_U16 AddWeight4;
    HI_U8 ImpEqlDataSetIndAmpEn;
    HI_U8 ImpEqlDataNotForPowEn;

    HI_U8 ImpEqlDataRecogEn;
    HI_U8 ImpInDataEqlContThr;
    HI_U8 ImpSet0WinIndOptionPream;//Preamble�׶Σ�IMP����������ѡ��    {0,1,2}
    HI_U8 ImpSet0WinIndOptionFCPL;//FCPL�׶Σ�IMP����������ѡ��  {0,1,2}

    HI_U16 ImpSet0NumThrPream;//Preamble�׶Σ�ÿ1024��IMP��������������  {0~1025}
    HI_U16 ImpSet0NumThrFCPL;//FCPL�׶Σ�ÿ1024��IMP��������������   {0~1025}

    HI_U8 ImpWinInd1Set0HalfLenOldPream;//Preamble�׶Σ�������ָʾ1�ľ����ݰ뾶 {0~39}
    HI_U8 ImpWinInd1Set0HalfLenNewPream;//Preamble�׶Σ�������ָʾ1�������ݰ뾶 {0~39}
    HI_U8 ImpWinInd2Set0HalfLenOldPream;//Preamble�׶Σ�������ָʾ2�ľ����ݰ뾶 {0~39}
    HI_U8 ImpWinInd2Set0HalfLenNewPream;//Preamble�׶Σ�������ָʾ2�������ݰ뾶 {0~39}

    HI_U8 ImpWinInd3Set0HalfLenOldPream;//Preamble�׶Σ�������ָʾ3�ľ����ݰ뾶 {0~39}
    HI_U8 ImpWinInd3Set0HalfLenNewPream;//Preamble�׶Σ�������ָʾ3�������ݰ뾶 {0~39}
    HI_U8 ImpWinInd1Set0MethodPream;//Preamble�׶Σ�������ָʾ1���������� {0~7}
    HI_U8 ImpWinInd2Set0MethodPream;//Preamble�׶Σ�������ָʾ2���������� {0~7}

    HI_U8 ImpWinInd3Set0MethodPream;//Preamble�׶Σ�������ָʾ3���������� {0~7}
    HI_U8 ImpWinInd1Set0HalfLenOldFCPL;//FCPL�׶Σ�������ָʾ1�ľ����ݰ뾶  {0~39}
    HI_U8 ImpWinInd1Set0HalfLenNewFCPL;//FCPL�׶Σ�������ָʾ1�������ݰ뾶  {0~39}
    HI_U8 ImpWinInd2Set0HalfLenOldFCPL;//FCPL�׶Σ�������ָʾ2�ľ����ݰ뾶  {0~39}

    HI_U8 ImpWinInd2Set0HalfLenNewFCPL;//FCPL�׶Σ�������ָʾ2�������ݰ뾶  {0~39}
    HI_U8 ImpWinInd3Set0HalfLenOldFCPL;//FCPL�׶Σ�������ָʾ3�ľ����ݰ뾶  {0~39}
    HI_U8 ImpWinInd3Set0HalfLenNewFCPL;//FCPL�׶Σ�������ָʾ3�������ݰ뾶  {0~39}
    HI_U8 ImpWinInd1Set0MethodFCPL;//FCPL�׶Σ�������ָʾ1����������

    HI_U8 ImpWinInd2Set0MethodFCPL;//FCPL�׶Σ�������ָʾ2����������
    HI_U8 ImpWinInd3Set0MethodFCPL;//FCPL�׶Σ�������ָʾ3����������
    HI_U16 ImpScaleMinPream;//����Preamble�׶Σ�V200�°汾��ȥ��������м���ȥ�������޵ı���ϵ������Сȡֵ

    HI_U8 ImpScaleShiftForNCopiesThr;// ʶ�𿽱������٣��л�ImpScale�������ź�����
    HI_U8 ClImpNCpThr;// ʶ�𿽱������ٺ͸��ٵ��Ʒ�ʽ���ر�IMP�������ź�����
    HI_U8 ClImpPlQamMdThr;// ʶ�𿽱������ٺ͸��ٵ��Ʒ�ʽ���ر�IMP�������ź�����
    HI_U8 ImpSet0Ind1LowRkForGasPream;

    HI_U8 ImpSet0Ind2LowRkForGasPream;
    HI_U8 ImpSet0Ind3LowRkForGasPream;
    HI_U8 ImpSet0Ind1LowRkForGasFCPL;
    HI_U8 ImpSet0Ind2LowRkForGasFCPL;

    HI_U8 ImpSet0Ind3LowRkForGasFCPL;
    HI_U8 GasRecogLowPowWinNumPream;
    HI_U16 ImpScaleMinFCPL;

    HI_U16 ImpScaleMaxFCPL;
    HI_U8 GasRecogLinearThrPream;         //IMP��˹ʶ����������޲���: 6
    HI_U8 GasRecogWinCntMaxPream;         //IMP��˹ʶ����������޲���: 3

    HI_U8 NonGasRecogWinCntMaxPream;
    HI_U8 GasRecogLowPowWinNumFCPL;
    HI_U8 GasRecogHiPowWinNumFCPL;
    HI_U8 GasRecogLinearThrFCPL;         //IMP��˹ʶ����������޲���: 6

    HI_U8 GasRecogWinCntMaxFCPL;         //IMP��˹ʶ����������޲���: 3

    HI_U8 NonGasRecogWinCntMaxFCPL;
    HI_U8 SatProcModePream;
    HI_U8 SatProcModeFCPL;

    HI_U8 GasRecogHiPowWinNumPream;
    HI_U8 Rsd;
    HI_U16 SatIndThres;                                                     //IMPģ�����ڼ��㱥��ָʾ������{511}

    HI_U8 FreqPowerThresScaleEn;
    HI_U8 FreqPowerThresScale;
    HI_U16 ClipInd1Scale;

    HI_U16 ClipInd2Scale;
    HI_U16 ClipInd3Scale;
}DIAG_CMD_IMPN_PARA_NV_STRU_V200;// 100

typedef struct
{
    HI_U8 c_tx_fec_rate;//���ʣ�ƫ�Ƶ�ַ0x002C��[5:3]��λ��3��0x0
    HI_U8 c_tx_modu_mode;//����ģʽ��ƫ�Ƶ�ַ0x002C��[8:6]��λ��3��0x0
    HI_U8 c_tx_mac_length;//MAC�����ֽ�����ƫ�Ƶ�ַ0x002C��[15:9]��λ��8��0x40
    HI_U8 c_lmt_ratio_th;

    HI_U8 c_hot_rstn;// �ȸ�λ ��ƫ�Ƶ�ַ0x0080��[0]��λ��1λ��0x1
    HI_U8 c_cool_rstn;// �临λ ��ƫ�Ƶ�ַ0x0080��[1]��λ��1λ��0x1
    HI_U16 c_fs_freq;// (ƫ�Ƶ�ַ0x0000��[9:0]��λ��10λ��Ĭ��ֵ0x3C)

    HI_U16 c_fc_0;//��ƫ�Ƶ�ַ0x000C��[13:0]��λ��14λ�� Ĭ��ֵ0x352��
    HI_U16 c_fc_1;// (ƫ�Ƶ�ַ0x000C��[27:14]��λ��14λ�� Ĭ��ֵ0x379)

    HI_U16 c_fc_2;// (ƫ�Ƶ�ַ0x0010��[13:0]��λ��14λ�� Ĭ��ֵ0x395)
    HI_U16 c_fc_3;// (ƫ�Ƶ�ַ0x0010��[27:14]��λ��14λ�� Ĭ��ֵ0x3A9)

    HI_U16 c_fc_4;// (ƫ�Ƶ�ַ0x0014��[13:0]��λ��14λ�� Ĭ��ֵ0x3C4)
    HI_U16 c_fc_5;// (ƫ�Ƶ�ַ0x0014��[27:14]��λ��14λ�� Ĭ��ֵ0x3E7)

    HI_U16 c_fc_6;// (ƫ�Ƶ�ַ0x0018��[13:0]��λ��14λ�� Ĭ��ֵ0x40B)
    HI_U16 c_fc_7;// (ƫ�Ƶ�ַ0x0018��[27:14]��λ��14λ�� Ĭ��ֵ0x42A)

    HI_U16 c_fc_8;//��ƫ�Ƶ�ַ0x001C��[13:0]��λ��14λ��Ĭ��ֵ0x44C��
    HI_U16 c_fc_9;//��ƫ�Ƶ�ַ0x001C��[27:14]��λ��14λ��Ĭ��ֵ0x472��

    HI_U16 c_fc_10;//��ƫ�Ƶ�ַ0x0020��[13:0]��λ��14λ��Ĭ��ֵ0x487��
    HI_U16 c_fc_11;//��ƫ�Ƶ�ַ0x0020��[27:14]��λ��14λ��Ĭ��ֵ0x4A1��

    HI_U16 c_fc_12;//��ƫ�Ƶ�ַ0x0024��[13:0]��λ��14λ��Ĭ��ֵ0x4B6��
    HI_U16 c_fc_13;//��ƫ�Ƶ�ַ0x0024��[27:14]��λ��14λ��Ĭ��ֵ0x4CE��

    HI_U16 c_fc_14;//��ƫ�Ƶ�ַ0x0028��[13:0]��λ��14λ��Ĭ��ֵ0x4F1��
    HI_U16 c_fc_15;//��ƫ�Ƶ�ַ0x0028��[27:14]��λ��14λ��Ĭ��ֵ0x514��

    HI_U8 c_lmt_bpf_bwa;//��[23:20]��λ��4λ��0x4
    HI_U8 c_lmt_lmt_1;//��[19:15]��λ��5λ��0xe
    HI_U8 c_lmt_st0_1;//��[14:10]��λ��5λ��0x18
    HI_U8 c_lmt_lmt_2;//��[9:5]��λ��5λ��0x8

    HI_U8 c_lmt_st0_2;//��[4:0]��λ��5λ�� 0x14
    HI_U8 c_lmt_pre_lmt;//([17:13]��λ��5) ������ֵ��ؼĴ���2  ��ƫ�Ƶ�ַ0x0074��0xa
    HI_U8 c_lmt_pst_lmt;//��[12:9]��λ��4��0x3
    HI_U8 c_lmt_pre_st0;//��[8:4]��λ��5��0x14

    HI_U8 c_lmt_pst_st0;//��[3:0]��λ��4��0x6
    HI_U8 c_si_fir_len;//��[11:10]��λ��2������Ϣ������ؼĴ��� ��ƫ�Ƶ�ַ0x0050��0x1
    HI_U8 c_null_lmt_th;//��[9:7]��λ��3��0x5
    HI_U8 c_si_din_lmt;//��[6:4]��λ��3��0x3

    HI_U8 c_si_max;//��[3:0]��λ��4��0x8
    HI_U8 c_coef_bw;//��[10:8]��λ��3����������ؼĴ�����ƫ�Ƶ�ַ0x0060��0x3
    HI_U8 c_hpow_th;//��[7:0]��λ��8��0x2
    HI_U8 c_tx_gain;

    HI_U8 c_sync_th;
    HI_U8 c_xo_scan_step;
    HI_U8 c_xo_scan_stop;
    HI_U8 c_frame_fix;

    HI_U8 c_frame_begin;
    HI_U8 c_modu_fix;
    HI_U8 c_modu_mode;
    HI_U8 c_fec_rate;

    HI_U8 c_sync_divisor_add;
    HI_U8 c_sync_th_rmv;
    HI_U8 c_sync_th_am6;
    HI_U8 c_sync_th_dtn;

    HI_U8 c_merge_ena;
    HI_U8 c_equ_dat_max;
    HI_U8 c_agc_gain_dly;
    HI_U8 c_agc_vld_disable;//  ��ƫ�Ƶ�ַ0x0080��[9]��λ��1λ��0x1

    HI_U8 c_frame_end;                      //6
    HI_U8 c_lmt_hpf_bws;                    // 2
    HI_U16 c_fc_add;                         //�ز���ƫ����0x0
    HI_U32 c_clk_freq;

    HI_U8 c_pk_gain2;//��ƫ�Ƶ�ַ0x0048��[3:0]��λ��4λ��Ĭ��ֵ0x0C��
    HI_U8 c_pk_gain1;//��ƫ�Ƶ�ַ0x0048��[7:4]��λ��4λ��Ĭ��ֵ0x09��
    HI_U8 c_pk_gain0;//��ƫ�Ƶ�ַ0x0048��[11:8]��λ��4λ��Ĭ��ֵ0x0D��
    HI_U8 c_amp_refer_gain;//��ƫ�Ƶ�ַ0x0078��[3:0]��λ��4λ��Ĭ��ֵ0x08��

    HI_U8 c_tx_lim_th;//��ƫ�Ƶ�ַ0x0078��[11:4]��λ��8λ��Ĭ��ֵ0xD4��
    HI_U8 c_mean_dlt;//��ƫ�Ƶ�ַ0x0078��[15:12]��λ��4λ��Ĭ��ֵ0x8��
    HI_U8 c_lmt_st0_3;//��ƫ�Ƶ�ַ0x0078��[20:16]��λ��4λ��Ĭ��ֵ0x10��
    HI_U8 c_lmt_lmt_3;//��ƫ�Ƶ�ַ0x0078��[25:21]��λ��5λ��Ĭ��ֵ0x8��

    HI_U8 c_mean_max_th0;//��ƫ�Ƶ�ַ0x007C��[7:0]��λ��8λ��Ĭ��ֵ0x00��
    HI_U8 c_mean_min_th0;//��ƫ�Ƶ�ַ0x007C��[15:8]��λ��8λ��Ĭ��ֵ0x00��
    HI_U8 c_mean_max_th1;//��ƫ�Ƶ�ַ0x007C��[23:16]��λ��8λ��Ĭ��ֵ0x28��
    HI_U8 c_mean_min_th1;//��ƫ�Ƶ�ַ0x007C��[31:24]��λ��8λ��Ĭ��ֵ0x0A)

    HI_BOOL is_sc_clk_en;   //խ��ʱ���Ƿ�ر�
    HI_U8 pad;
    HI_U16 pad1;
}DIAG_CMD_PLCSC_PARA_NV_STRU_V200;

/***************************ISP****************************/
typedef struct
{
    HI_U8 f1_en;
    HI_U8 f1_sig_phase_idx;
    HI_U8 f2_en;
    HI_U8 f2_sig_phase_idx;

    HI_U16 isp_power_scale;
    HI_U16 isp_margin_len;

    HI_U32 isp_offset_len;

    HI_U8 isp_symbol_num;
    HI_U8 isp_tx_cnt;   //B070�����������ʹ���
    HI_U8 is_isp_tx_en;
    HI_U8 is_isp_test_mode;
}DIAG_CMD_ISP_NV_STRU_V200;

/***************************Clock****************************/
typedef struct
{
    HI_S16 Carrier_Set_SNR_Offset;       //ԭʼSNR����ƫ��ֵ��unit:1/16dB{0 }
    HI_S16 Carrier_Set_SNR_Th;           //���õ�ԭʼSNR���� {-336 }

    HI_U8  Carrier_Set_SNR_Th_Type;      //ԭʼSNR���޻�ȡ��ʽ��1����������ֵ,0��ʹ������ֵ  {1 }
    HI_U8  Preamble_FreqErrEstEn;       //Preamble �׶ε�Ƶƫ����ʹ�ܣ�0����ʹ��,1��ʹ�� {1, }
    HI_U8  Preamble_FreqErrEn;           //Preamble�׶�Ƶƫ���ƽ��ʹ�ܣ�0����ʹ��,1��ʹ�� { 1,}
    HI_U8  m;                            //cLK�Ż���������ƽ�����ʱ���õĳ���ǰ��ĸȡ��λ����m��ȡֵ��ΧΪ0-9{6}

    HI_U8 Rsd;
    HI_U8  Rsd0;                         //0
    HI_U8  K4MAWin;                      //Ƶƫ����ƽ�����Ĵ�С��������Ϊ0,1,2,3�ȶ�Ӧ�Ĵ��Ĵ�СΪ2,4,8,16 {2 }
    HI_U8  SinguCntTh;                   //��������ֵ���������Ĵ������� { 5,}

    //�˴����鸴����Ĵ�����˳�򲻿ɱ�
    HI_U16 FreqErr_CapSinguTh[4];        //��ͬ�ŵ������Ĳ���׶Σ�ƵƫΪ����ֵ���о�����   {2684,3355,4027,5369}
    HI_U16 FreqErr_TrackSinguTh[4];      //��ͬ�ŵ������ĸ��ٽ׶Σ�ƵƫΪ����ֵ���о�����{1342,2013,2684,3355}
    HI_U8  h_Par_Th[3];                  // ��3��h_Par����ֵ���ֲ�ͬ�ŵ��������Ӷ�ѡ��ͬ���˲�ϵ��{60,30,14},

    HI_U8  Rsd10;                         //0
    //�˴����鸴����Ĵ�����˳�򲻿ɱ�
    HI_S16 G1_Capture[4];                //��ͬ�ŵ������Ĳ���׶ε�G1ϵ��ֵ  {0,0,0,0},
    HI_S16 G2_Capture[4];                //��ͬ�ŵ������Ĳ���׶ε�G2ϵ��ֵ  {16384,6554,1311,0}
    HI_S16 G1_Track[4];                  //��ͬ�ŵ������ĸ��ٽ׶ε�G1ϵ��ֵ  {0,0,0,0},
    HI_S16 G2_Track[4];                  //��ͬ�ŵ������ĸ��ٽ׶ε�G2ϵ��ֵ  {0,0,0,0},
    HI_S16 AlphaG1[2];                   //��������ٽ׶ε�G1ϵ����������ֵ  {26214,13107},
    HI_S16 AlphaG2[2];                   //��������ٽ׶ε�G2ϵ����������ֵ  {26214,13107},
    HI_U16 FreqErr_Capture_Th[4];        //��ͬ�ŵ������Ĳ���׶ε�ϵ�����µ�Ƶƫ����ֵ {0,0,0,0}
    HI_U16 FreqErr_Track_Th[4];         //��ͬ�ŵ������ĸ��ٽ׶ε�ϵ�����µ�Ƶƫ����ֵ {1342,2013,2684,3355}
    HI_U16 FreqErr_C2T_Th[4] ;           //��ͬ�ŵ������Ĳ���ת��Ϊ���ٽ׵�Ƶƫ����ֵ{2013,2684,3355,4027}

    HI_U32 PL_BLER_Th;                   //BLER������{100}
    HI_U32 S2WTimerTh;                   //˯��ʱ�����ޣ����ݴ�����ѡ����Idle��ת���ĸ�ģʽ: 5s ��λ��s {5 }
    HI_U16 Agc_Sat_Th;                   //Preamble���Ž׶α����ж�  {512}   20150605�޸�Ϊ1023
    HI_S16 FreqErr_Pre;                  //NVˢ�¾������ޣ���ǰ��Ҫ�ϱ�Ƶƫֵ���ϴεĲ��С�ڴ�����ʱ����ˢ�¼Ĵ���{671 }

    HI_U8 AssumClkerrMode;
    HI_U8 Rsd1[3];

    HI_S32 AssumClkerrStep;
}DIAG_CMD_CLOCK_NV_STRU_V200;

/***************************Pcs****************************/
typedef struct
{
    HI_U8 BurstSendEndInterrupt                                    :1;        // burst��������ж�{1}
    HI_U8 BurstRcvEndInterrupt                                       :1;        // burst��������ж�{1}
    HI_U8 Pb4Interrupt                                                      :1;        // pb4�ж�{1}
    HI_U8 Pb3Interrupt                                                      :1;        // pb3�ж�{1}
    HI_U8 Pb2Interrupt                                                      :1;        // pb2�ж�{1}
    HI_U8 Pb1Interrupt                                                      :1;        // pb1�ж�{1}
    HI_U8 FcErrInterrupt                                                    :1;        //FC��������ж�ʹ��{1}
    HI_U8 FcOkInterrupt                                                    :1;        //FC�����ȷ�ж�ʹ��{1}

    HI_U8 ParaOkInterrupt                                                :1;        //���м���ж�ʹ��{1}
    HI_U8 RxOverFlowInterrupt                                        :1;        //��������ж�{1}
    HI_U8 IspSendEndInterrupt                                         :1;        //isp���ͽ������ϱ��жϱ�־{0}
    HI_U8 OnlyPreambleRcvInterrupt                              :1;        //txֻ����preambleʱ��rx�˽��յ�preamble���жϱ�־{0}
    HI_U8 OnlyPreambleSendEndInterrupt                     :1;        //txֻ����preambleʱ��preamble�ķ�������жϱ�־{0}
    HI_U8 FrameSingalSendStart                                      :1;        //���η��Ϳ�ʼ�ж�ʹ��{1}
    HI_U8 SendWrCollision                                                :1;        //����д��ͻ�жϱ�־ʹ��{1}
    HI_U8 Rsd1                                                                    :1;        //����

    HI_U8 PcsReportStage                                                 :3;        //�ز��������ϱ��׶�{4}
    HI_U8 ErrPbReport                                                      :1;        //Payload��CRCУ��ʧ��ʱ��Payload�Ƿ��ϱ���ѡ��0��������;1�����͡�{1}
    HI_U8 AbandonedFcReprot                                        :1;        //FC��CRCУ��ɹ������մ���������Ҫ����ʱ��FC�Ƿ��ϱ���ѡ��0�������ͣ�1�����͡�{1}
    HI_U8 ErrFcReprot                                                       :1;        //FC��CRCУ��ʧ��ʱ��FC�Ƿ��ϱ���ѡ��0�������ͣ�1�����͡�{1}
    HI_U8 Rsd2                                                                   :2;        //����

    HI_U8 Rsd3;
}DIAG_CMD_PCS_NV_STRU_V200;

typedef struct
{
    DIAG_CMD_PCS_NV_STRU_V200     PcsInterrupt;
    HI_U8    TxAfePgaGain;           //�����AFE��ģ�⹦������:000: -20dB,0001: -18dB,. . .,1010: 0dB{10}
    HI_U8    Rsd[3];
}DIAG_CMD_AFE_PCS_NV_STRU_V200;


//PHY��stub�Ŀ���NV
typedef struct
{
    HI_U8  ChipSelectEn;                    //оƬ�Զ�ɸѡ����:0,�ر�;1,��
    HI_U8  ChipSelectTxSwitch;              //оƬ�Զ�ɸѡTX������������:0,RX;1,TX
    HI_U8  PhyStressEn;                     //phy��ѹ��������������:0,�ر�;1,��
    HI_U8  PhyStressTxSwitch;               //phy��ѹ�����Կ���:0,RX;1,TX

    HI_U8  PhySpeedEn;                      //phy�����ʲ�����������:0,�ر�;1,��
    HI_U8  PhySpeedTxSwitch;                //phy�����ʲ��Կ���:0,RX;1,TX
    HI_U8  PowerSaveSwitch;                 //�͹��Ĺ��ܿ���:0,�ر�;1,��
    HI_U8  NoiseEn;                         //����������������:0,�رգ�1,��

    HI_U8  Phase;                           //CCOʹ�ܿ���:0,�رգ�1,��
    HI_U8  CollectDataSwitch;               //�������ܿ���:0xa5:��;����ֵ:�ر�
    HI_U8  PhyDiagnoseSwitch;               //PHY���쳣״̬��⿪��:0,�رգ�1,��
    HI_U8  rsd1;                            //0xa5: AdiAfe;����:HiAfe

    HI_U8 snid;                                 //ͬ�������snid
    HI_U8 freq;                                 // ͬ�������Ƶ��
    HI_U16 tei;                                 //ͬ�������tei;

    HI_U8 SetSyncObjectEn;              //�Ƿ���ͬ������:0��ʾ���趨ͬ������1:��ʾ�趨ͬ������
    HI_U8 FreqErrorReportEn;             //��ʾΪB4���߼���ģʽ��0:��ʾB4ģʽ��1:��ʾ����ģʽ
    HI_U8 isHiGreenTreeV100;            //�������Ƿ�ΪB4������Hi3911����,0:��ʾΪHI3911����,1:��ʾB4
    HI_U8 NTBAlpha;                          // NTB����alpha�˲�ϵ��
    HI_U32 min_beacon_period;          //��С�ű���,��λ:ms,Ĭ��Ϊ2000
    HI_U32 max_beacon_period;         //����ű���,��λ:msĬ��Ϊ160000

    HI_U32 ntb_report_ppm_period;      //�ϱ�Ƶƫppm�����ڣ�Ĭ��Ϊ20s����λ:ms
    HI_U32 ntb_report_ppm_switch;      // �ϱ�Ƶƫppm�Ŀ��أ�1:������0:�رգ�Ĭ�Ͽ���

    HI_U32 Rsd[4];                          //����
}DIAG_CMD_PHY_STUB_NV_STRU_V200;

typedef struct
{
    HI_S8 freq_power[26] ;          //����Ƶ���������ֹ���
    HI_U8 Rsd[2];
}DIAG_CMD_FREQ_POWER_NV_STRU_V200;

// V200 ��̨��ʶ��NV���ò���
#define DIAG_CMD_TF_SOS_CNT               (20)
#define DIAG_CMD_TF_LINE                  (8)

typedef struct
{
    HI_U32 v100_old_en : 1;                   // �����ϰ汾v100ʹ�ܣ�0:����Ч��1:��Ч��Ϊ1ʱ����ӵ�·����оƬ�л�ȡһ���ж�
    HI_U32 lvai_en : 1;                       // ̨��ʶ����ʹ�ܣ�0:��ʹ�ܣ�1:ʹ�ܣ�Ĭ��Ϊ1
    HI_U32 v100_en : 1;                       // v100����ģʽ��������ʹ�ܣ�0:��ʹ�ܣ�1:ʹ�ܣ�Ĭ��Ϊ1
    HI_U32 v200_en : 1;                       // v200��������ʹ�ܣ�0:��ʹ�ܣ�1:ʹ�ܣ�Ĭ��Ϊ1
    HI_U32 pos_en : 1;                        // ��������ʹ�ܣ�0:��ʹ�ܣ�1:ʹ�ܣ�Ĭ��Ϊ1
    HI_U32 neg_en : 1;                        // ��������ʹ�ܣ�0:��ʹ�ܣ�1:ʹ�ܣ�Ĭ��Ϊ1
    HI_U32 iir_en : 1;                        // iir�˲���ʹ�ܣ�0:��ʹ�ܣ�1:ʹ�ܣ�Ĭ��Ϊ1
    HI_U32 iir_stable_cnt : 9;                // Iir��ʼ�����ȶ���ʱ����Ĭ��200
    HI_U32 del_type_us : 1;                   // ΢��쳣ɾ�����ƣ�0:�ж��쳣ʱ��ɾ��ǰ�����ݣ�1:�ж��쳣ʱ��ɾ���������ݣ�Ĭ��Ϊ0
    HI_U32 del_type_ms : 1;                   // ���뼶�쳣ɾ�����ƣ�0:�ж��쳣ʱ��ɾ��ǰ�����ݣ�1:�ж��쳣ʱ��ɾ���������ݣ�Ĭ��Ϊ1
    HI_U32 th_del_us : 10;                    // ΢����ޣ�Ĭ��Ϊ30us,��λus
    HI_U32 power_period_stat_st : 1;          //  ��Ƶ����ͳ�ƿ��ƣ���0��1ʱоƬ������оƬ���һ��ͳ�ƺ��ټ���ͳ�ƣ������ٴ�ͳ�ƣ�������Ƚ�����0��Ȼ������1��
    HI_U32 noise_detect_en : 1;               // �������㲿�ֹ��ܵ�ʹ���źţ�Ĭ�ϴ�,0 �ر�,1 ��
    HI_U32 suspend_poweredge_type : 2;        // �жϲ�����ǿ�����,StaĬ��0,CcoĬ��1,0��˫��,1������,2������

    HI_U16 th_del_ms;                         // ���뼶�쳣���ޣ�Ĭ��1000us����λus
    HI_U16 th_alarm;                          // �쳣ʱ�����ޣ�Ĭ��15000us����λus

    HI_U16 sth_v100;                          // v100����ֵ��Ч���ޣ�Ĭ��72
    HI_U16 sth_v200;                          // v200����ֵ��Ч���ޣ�Ĭ��73

    HI_U32 v100_seq_len : 9;                  // v100�����е����ֳ��ȣ�Ĭ��13bit
    HI_U32 v100_seq_rep : 9;                  // v100�������ظ�������Ĭ��Ϊ11���ܵ����ֳ���Ϊ 11*13 = 143bit
    HI_U32 v100_seq_num : 14;                 // v100���������ݣ�150

    HI_U16 v200_seq_len;                      // v200�����е����ֳ��ȣ�Ĭ��127bit
    HI_U16 v200_seq_num;                      // v200���������ݸ�����128

    HI_U32 gain;                              // iir������ϵ����Ĭ��Ϊ12366

    HI_U32 len;                               // ������ȣ�Ĭ��153

    HI_U8 th_l;                               // ��ȥ�����ޣ�4us
    HI_U8 th_h;                               // ��ȥ�����ޣ�15us
    HI_U16 th_coef;                           // ����ϵ����341,�൱��0.67

    HI_U16 suspend_source_switch : 1;         // �л��жϻ�ȡԴ��0:ad��������ʱSuspend_ad_en��Ҫ��Ϊ1, 1:�ⲿ��·������Ĭ��0
    HI_U16 ntb_source_switch : 1;             // �л�ntb��ȡԴ��0:ȫ����1:���أ�Ĭ��0
    HI_U16 suspend_edge_type : 2;             // �жϱ��أ�0:�����أ�1:�½���,2:���ؾ���Ч��Ĭ��Ϊ0
    HI_U16 single_path_en : 1;                // ��·ģʽ,0:��Ч��1:��Ч����ʱsuspend_ad_en����Ϊ1����·ģʽ��Чʱ��ֻ��th_v_relative��th_v_absolute�ĵ�1��ֵ
                                              // �������ã�������ֻ����1·�����4������
    HI_U16 suspend_ad_en  : 1;                // оƬͨ��ad���ݣ�ʹ��ĳ�����޵�ѹ�ṩ1·�����жϣ�0:��Ч��1:��Ч��Ĭ��0����   single_path_enΪ1ʱ������Ϊ1
    HI_U16 vpp_learn_rst : 1;                 // Ϊ1�ǣ����¼���v_min,v_max,Ĭ��Ϊ0
    HI_U16 vpp_win_len : 6;                   // �������������ݸ�����Ĭ��Ϊ10
    HI_U16 power_period_close : 1;            // ����ֹͣ��Ƶ����ͳ�ƣ���0��1оƬ���������ֹͣ��������λ�ڲ����������ÿһ��ֹͣ����1����0���ɡ�
    HI_U16 t_in_f_lock : 1;                   // �Ƿ����½��ؼ�������8·��t_in,Ĭ��Ϊ1�������½�������
    HI_U16 suspend_source_switch_ndm_tx : 1;  // ��������ⲿ�жϻ�ȡԴ��Ĭ��Ϊ�ⲿ��·����,ֵΪ1
    //HI_U16 pad : 1;
    HI_U16 vpp_learn_duration;                // vpp��������ʱ����������������ʾ��Ĭ��Ϊ2000

    HI_U32 edge_r_th_id : 4;                  // ���������ʹ�õ�th_v��ţ�Ĭ��Ϊ5
    HI_U32 edge_f_th_id : 4;                  // ����½���ʹ�õ�th_v��ţ�Ĭ��2
    HI_U32 edge_step : 8;                     // ���������ؼ����Ĭ��Ϊ50����������Ϊ5����
    HI_U32 th_v_type : 1;                     //    0:������ޣ�1:�������ޣ�Ĭ��Ϊ0
    HI_U32 pad4 : 8;                          // ����λ,Ĭ��Ϊ0
    HI_U32 collect_ntb_v_mode : 2;            // �ռ�NTB����ʱ�ļ���ģʽ��0:v100,v200�����ݣ�1:v100ʹ�ܣ�2:v200ʹ��
    HI_U32 om_ntb_collect_mode : 3;           // �ɼ�NTB���ݵ�ģʽ��0:���ɼ���1:�洢�ɹ����洢�쳣��2:�洢�ɹ���3:�洢�쳣
    HI_U32 lvai_cal_clk_sw_en : 2;            // ������̬���ģ�Ĭ������00������̨��ʶ��͹��ģ�00,01��Ӳ������ʱ��ʹ�ܣ�
                                              // 10�������ֹʱ�ӣ�Ӳ��������Ч,11�����ʹ��ʱ�ӣ�Ӳ��������Ч
    HI_U32 power_period_duration;             //   ͳ�ƹ�Ƶ����������ʱ������λ��,Ĭ��:10
    HI_U32 th_t_calc_len : 8;                 // ȥ�����޼��㳤�ȣ�Ĭ��Ϊ143
    HI_U32 pad2 : 24;
    HI_U32 noise_win_num : 24;                // ����ͳ�ƴ�����������Noise_win_lenΪ��λ,��һ������£�Ĭ��1800,���޷����ʶ��׶Σ�Ĭ��1,STA�˰���ÿ3min�ϱ���NDM����ÿ1s�ϱ�
    HI_U32 noise_win_len : 8;                 // ��������Ĵ����ȣ��Թ�Ƶ����Ϊ��λ,Ĭ��50��1�룩

    HI_U32 all_phase_zerocross_en : 1;        // ����ͬʱ�������ntbʹ���źţ�Ĭ��Ϊ00����ʹ�ܣ�1��ʹ�ܣ�ʹ��ʱ���轫Suspend_poweredge_type��1�����أ���suspend_ad_en��1��ad�ж�ʹ�ܣ���ʹ�ܽ���ʱ�����3���̨��ʶ��������Ч
    HI_U32 phase_one_order : 2;               // �����л���һ���л���λ
    HI_U32 phase_two_order : 2;               // �����л��ڶ����л���λ
    HI_U32 phase_three_order : 2;             // �����л��������л���λ
    HI_U32 v_buf_step : 10;                   // ����ѧϰ��ѹ����ʱ��������λ΢�롣Ĭ�ϣ�100��
    HI_U32 th_t_num_th : 5;                   // ��������ʱ�����ڸò���ʱ���ᴦ���ڲ����������Ŷ�ֵ�������޼��㡣Ĭ��ֵ10��
    HI_U32 pad3 : 10;

    HI_U16 all_phase_dummy_th;                // Nv ���жϺ󣬼������������һ��ʱ�䣬��֤�������ж���Ȼ�ܹ��ϱ�Ĭ��֮ǰ1000����Ϊ2500����λ΢��
    HI_U16 all_phase_overtime_th;             // ���жϺ󣬼����������������ʱ������ޣ���ʱ��ǿ�����ࣻĬ��֮ǰ2000����Ϊ3000����λ΢��

    HI_U32 ntb_offset_th;                     // NTBͬ��ʱ��оƬ�ڲ�NTB�����ͬ�����ޣ�Ĭ��ֵΪ25000����Ӧ1ms��

    HI_U16 th_v_relative[DIAG_CMD_TF_LINE];   // �������Ĭ��ֵ��93,124,186,341,682,837,899,930,�����·ģʽ��Ч�������������Ч����1��ֵΪ
                                              // Ϊ��Ч�ıȽϵ㣬��Ӧ����:0.091,0.121,0.181,0.333,0.667,0.818,0.879,0.909
    HI_S16 th_v_absolute[DIAG_CMD_TF_LINE];   // �������ޣ�Ĭ��ֵ�������·ģʽ��Ч���Ҿ���������Ч����1��ֵΪ��Ч�ıȽϵ㣬
}DIAG_CMD_CFG_TF_REG_PARAM_STRU_V200;

typedef struct
{
    HI_S16 sos[DIAG_CMD_TF_SOS_CNT];          //  Ĭ�ϲ���Ϊ 16384;-32048; 16384; -21035; 7027; 16384; -28326; 15259; -29258; 13368
                                                                                                           //16384; -14345; 12004; -29834; 13867 ;16384; 10978; 5776; -29824; 13867
}DIAG_CMD_CFG_TF_REG_IIR_PARAM_STRU_V200;

#ifdef __cplusplus
}
#endif


#endif


