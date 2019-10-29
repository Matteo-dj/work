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
#ifndef PHY_NV_H
#define PHY_NV_H

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
#define DIAG_CMD_BASE_NV_ID                 0x0001
#define DIAG_CMD_AGC_PARA_NV_ID             0x0001
//#define DIAG_CMD_DCOC_PARA_NV_ID            0x0002
//#define DIAG_CMD_FEQ_NV_ID                  0x0003
//#define DIAG_CMD_FEQ_PILOT_NV_ID            0x0004
#define DIAG_CMD_TX_NV_ID                   0x0005
//#define DIAG_CMD_SNR_EST_NV_ID              0x0006
//#define DIAG_CMD_FEC_NV_ID                  0x0008
//#define DIAG_CMD_SOFT_DEMOD_NV_ID           0x0009
//#define DIAG_CMD_PREAMBLE_NV_ID             0x000a
//#define DIAG_CMD_SYMALIGN_NV_ID             0x000c
#define DIAG_CMD_NBI_PARA_NV_ID             0x000d
#define DIAG_CMD_CONGREGATE_NV_ID            0x000e
//#define DIAG_CMD_BITLOADING_NV_ID           0x0011
//#define DIAG_CMD_ISP_NV_ID                  0x0012
//#define DIAG_CMD_CLOCK_NV_ID                0x0013
//#define DIAG_CMD_NTB_NV_ID                  0x0014
//#define DIAG_CMD_PHY_STUB_NV_ID             0x0016
//NV������ID��ŵ����ֵ
#define    DIAG_CMD_MAX_NV_ID               DIAG_CMD_CONGREGATE_NV_ID


#define FREQ_TRY_NUM                      6                 //ä��Ƶ�θ���
#define MAX_FREQ_NUM_V100                 (7)               // V100Ƶ�θ��� (7)

// V100ԭNV
#define HI3911_V1_DCOC_DC_SHIFT_BIT_VAL                                         (13)
#define HI3911_V1_DCOC_DC_DCOC_EN_VAL                                           (0)
#define HI3911_V1_DCOC_DC_DCOC_RST_MODE_VAL                                     (0)

#define HI3911_V1_FEQ_MV_NOISE_SCALE_VAL                                        (64)
#define HI3911_V1_FEQ_MV_NOISE_EN_VAL                                           (1)
#define HI3911_V1_FEQ_MV_NOISE_MODE_VAL                                         (0)
#define HI3911_V1_FEQ_FEQ_THRESH_VAL                                            (1)
#define HI3911_V1_FEQ_MV_NOISE_WIN0_VAL                                         (300)
#define HI3911_V1_FEQ_MV_NOISE_WIN1_VAL                                         (200)
#define HI3911_V1_FEQ_PILOT_ALPHA_SCALE_VAL                                     (819)
#define HI3911_V1_FEQ_PILOT_BETA_SCALE_VAL                                      (819)

#define HI3911_V1_SNR_EST_CARRIERS_PER_G_VAL                                     (1)
#define HI3911_V1_SNR_EST_METHOD_SELECT_VAL                                      (0)
#define HI3911_V1_SNR_EST_RECP_A_VAL                                             (158)
#define HI3911_V1_SNR_EST_AB_VAL                                                 (3328)

#define HI3911_V1_FEC_ITER_COUNT_FC_VAL                                          (15)
#define HI3911_V1_FEC_EXINFO_SCALE_VAL                                           (96)
#define HI3911_V1_FEC_CRC_EARLY_STOP_SWITCH_FC_VAL                               (1)
#define HI3911_V1_FEC_EARLY_STOP_START_ITER_FC_VAL                               (4)
#define HI3911_V1_FEC_ITER_COUNT_PL_VAL                                          (15)
#define HI3911_V1_FEC_CRC_EARLY_STOP_SWITCH_PL_VAL                               (1)
#define HI3911_V1_FEC_EARLY_STOP_START_ITER_PL_VAL                               (4)

#define HI3911_V1_SOFT_DEMOD_QAM_SFTD_X0_VAL                                     (27)
#define HI3911_V1_SOFT_DEMOD_QAM_SFTD_X1_VAL                                     (26)
#define HI3911_V1_SOFT_DEMOD_QAM_SFTD_X2_VAL                                     (25)
#define HI3911_V1_SOFT_DEMOD_QAM_SFTD_X3_VAL                                     (25)
#define HI3911_V1_SOFT_DEMOD_QAM_SFTD_X4_VAL                                     (24)
#define HI3911_V1_SOFT_DEMOD_QAM_SFTD_X5_VAL                                     (23)
#define HI3911_V1_SOFT_DEMOD_FACTOR0_VAL                                         (39322)
#define HI3911_V1_SOFT_DEMOD_FACTOR1_VAL                                         (39322)
#define HI3911_V1_SOFT_DEMOD_FACTOR2_VAL                                         (39322)
#define HI3911_V1_SOFT_DEMOD_FACTOR3_VAL                                         (39322)
#define HI3911_V1_SOFT_DEMOD_FACTOR4_VAL                                         (39322)
#define HI3911_V1_SOFT_DEMOD_FACTOR5_VAL                                         (27307)

#define HI3911_V1_SYMALIGN_FINE_SYNC_MODE_VAL                                    (1)
#define HI3911_V1_SYMALIGN_FINE_SYNC_POW_BIN_NUM_VAL                             (5)
#define HI3911_V1_SYMALIGN_FINE_SYNC_WIN_T1_VAL                                  (40)
#define HI3911_V1_SYMALIGN_FINE_SYNC_WIN_T2_VAL                                  (300)
#define HI3911_V1_SYMALIGN_FINE_SYNC_SCALE0_VAL                                  (64)
#define HI3911_V1_SYMALIGN_FINE_SYNC_SCALE1_VAL                                  (32)
#define HI3911_V1_SYMALIGN_FIX_FIRST_PATH_OFFSET_EN_VAL                          (1)
#define HI3911_V1_SYMALIGN_FIRST_PATH_OFFSET_VAL                                 (50)
#define HI3911_V1_SYMALIGN_FIRST_PATH_AJUST_VAL                                  (5)
#define HI3911_V1_SYMALIGN_LAST_PATH_AJUST_VAL                                   (10)
#define HI3911_V1_SYMALIGN_FINE_SYNC_PATH_LEN_MIN_VAL                            (200)
#define HI3911_V1_SYMALIGN_FINE_SYNC_PATH_LEN_OFFSET_VAL                         (0)
#define HI3911_V1_SYMALIGN_FINE_SYNC_FIXED_GI_EN_VAL                             (0)
#define HI3911_V1_SYMALIGN_FINE_SYNC_FIXED_GI_VAL                                (250)
#define HI3911_V1_SYMALIGN_FINE_SYNC_FRAME_NUM_VAL                               (5)

#define HI3911_V1_BITLOADING_MARGIN_VAL                                          (80)
#define HI3911_V1_BITLOADING_MAX_FIXED_BIT_TYPE_VAL                              (5)

#define HI3911_V1_ISP_WEIGHT_FACTOR_VAL                                          (38)
#define HI3911_V1_ISP_L_VAL                                                      (8)
#define HI3911_V1_ISP_D_VAL                                                      (1)
#define HI3911_V1_ISP_T0_VAL                                                     (512)
#define HI3911_V1_ISP_T1_VAL                                                     (9)
#define HI3911_V1_ISP_T2_VAL                                                     (13)
#define HI3911_V1_ISP_T3_VAL                                                     (7)
#define HI3911_V1_ISP_W_VAL                                                      (32)

#define HI3911_V1_NTB_WF4_NTB0_VAL                                               (32768)
#define HI3911_V1_NTB_WF4_NTB1_VAL                                               (32768)
#define HI3911_V1_NTB_WF4_NTB2_VAL                                               (16384)
#define HI3911_V1_NTB_WO4_NTB0_VAL                                               (65535)
#define HI3911_V1_NTB_WO4_NTB1_VAL                                               (65535)
#define HI3911_V1_NTB_WO4_NTB2_VAL                                               (65535)
#define HI3911_V1_NTB_NTB_TIMER_TH_VAL                                           (4000000000)
#define HI3911_V1_NTB_G1_NTB0_VAL                                                (0)
#define HI3911_V1_NTB_G1_NTB1_VAL                                                (0)
#define HI3911_V1_NTB_G1_NTB2_VAL                                                (0)
#define HI3911_V1_NTB_G2_NTB0_VAL                                                (32767)
#define HI3911_V1_NTB_G2_NTB1_VAL                                                (32767)
#define HI3911_V1_NTB_G2_NTB2_VAL                                                (32767)
#define HI3911_V1_NTB_NTB_TRACK_SINGU_TH0_VAL                                    (2013)
#define HI3911_V1_NTB_NTB_TRACK_SINGU_TH1_VAL                                    (1611)
#define HI3911_V1_NTB_NTB_TRACK_SINGU_TH2_VAL                                    (1342)
#define HI3911_V1_NTB_FREQ_ERR_OUT_SL_EN_VAL                                     (2)
#define HI3911_V1_NTB_NTB_FREQ_OFF_SET_MODE_VAL                                  (0)
#define HI3911_V1_NTB_FREQ_ERR_TH4_NTB_T0_VAL                                    (2013)
#define HI3911_V1_NTB_FREQ_ERR_TH4_NTB_T1_VAL                                    (1611)
#define HI3911_V1_NTB_FREQ_ERR_TH4_NTB_T2_VAL                                    (1342)
#define HI3911_V1_NTB_FREQ_ERR_TH4_WF0_VAL                                       (2013)
#define HI3911_V1_NTB_FREQ_ERR_TH4_WF1_VAL                                       (1342)
#define HI3911_V1_NTB_FREQ_ERR_TH4_NTB_C_VAL                                     (6710)
#define HI3911_V1_NTB_NTB_TIMER_LOW_TH_VAL                                       (900000)
#define HI3911_V1_NTB_NTB_DELAY_VAL                                              (13353)

/************************************************************
                             ���ݽṹ����
************************************************************/

/***************************AGC****************************/
typedef struct
{
   HI_U8  Quick_Adj_Win;        //AAGC���ƽ��ͳ�Ƶ���{2};
   HI_U8  Quick_Sat_Win;        //���ټ�ⱥ��ͳ�Ƶ���{0};
   HI_U8  Sat_Statics_Mode;     //������ͳ�Ʒ�ʽ{0};
   HI_U8  PowStaway;            //�����ϱ���ʽ{0};

   HI_U8  AAGCMode;             //AAGC����ģʽ{15};3
   HI_U8  PowTarget;            //Ŀ�깦�ʣ�{38};
   HI_U8  PowTarget_QuickAdj;   //����׶�Ŀ�깦��{38};
   HI_U8  ThrsSatPow;           //���͹�������{50};

   HI_U16 SatSubAAGC;          //�źű���ʱAAGC���渺����ֵ{320};
   HI_U8  ThrsSatPowNBI;        //NBI��⸴λ�ı��͹�������{48}
   HI_U8  Thrs_quick_adj;       //����׶�AAGC�������ޣ�{5};

   HI_U8  Thrs_preamble_adj;    //preamble�׶�AAGC�������ޣ�{0};
   HI_U8  Thrs_isp_adj;         //ISP���AAGC�������ޣ�{4}
   HI_U8  Diff_Pow_Thresh;      //խ�����ŵ�AAGC�������ޣ�{10};
   HI_U8  Compen_AAGC;          //խ�����ŵ�AAGC����ֵ��{80};

   HI_U8  AGC_set_slot;         // AAGC�����ȶ�ʱ��;{100};
   HI_U8  Rsd0;
   HI_U8  PowOffset[10];        //AD���ʲ���{0,0,0,1,1,2,3,5,10,20};

   HI_U16 ADPeakValue;          //AD�ο���ѹ{750};
   HI_U16 MaxAAGC;              //AGC���Gainֵ{768};
   HI_S16 MinAAGC;              //AGC��СGainֵ{-192}
   HI_S16 Fix_AAGC;             //�̶�AAGC���棻{320};

   HI_S16 InitialAAGC;          //��ʼAAGC���棻{320};
   HI_U8  DAGC_offset;          //DAGCƫ������{0};
   HI_U8  DAGCByPass;          //DAGCʹ��{0};

   HI_U16 Fix_DAGC_Gain;       //�̶�DAGC������������{0};
   HI_U8  Fix_DAGC_Enable;     //�̶�DAGC����ʹ�ܿ���{0};
   HI_U8  Fix_AAGC_Enable;      //�̶�AAGC��������ʹ�ܿ���;{0};

   HI_U16 Max_DAGC;            //���DAGC����{480};
   HI_U8 PowTarget_ISP;      //ISPģʽ��Ŀ�깦��{38}
   HI_U8 ISP_Adj_Win;        //ISPģʽ��ͳ�ƹ��ʴ��ڳ���{2}

   HI_U8 HiAFEStep;       //AFE������������{2}:0������1��code������;1������2��code��������
                            //2������1��code��������3������2��code��������
                            //��ADȷ����AD9866��2��������3�������谴����Ҫ����
   HI_U8 ISP_QuickAdj_MaxLen;         //ISPģʽ�¿���ȶ�ʱ��{5}

   HI_U8 PowOffset_ISP[10];  //ISPģʽ��AD���ʲ���{0,0,0,1,1,2,3,5,10,20};
}DIAG_CMD_AGC_PARA_NV_STRU;

/***************************TX****************************/
typedef struct
{
    HI_U8    TxTestEn;               //TX�����ݵ��������߼������ǣ�ʹ��{1}
    HI_U8    SyncCpNum;              //SyncCP�ĸ�����8->8,9->9,10->10,11->11,12->12,13->13,14->14,15->15,16->16��{10}
    HI_U16   SoundingSymbolNum;      //Sounding֡�ķ��Ÿ���{64}���ò���ֵ��������Ϊ2��n�η���

    HI_U8    SofNonRoboCodeRate;     //Sof֡��ROBOģʽ�����ʣ�0->1/2,1->16/21,2->16/18��{0}
    HI_U8    FcQam;                  //FC�ĵ��Ʒ�ʽ��0��QPSK��1��BPSK��{0}
    HI_U8    NonRoboQamType;         //Sof֡ ��ROBOģʽ���Ʒ�ʽѡ��0->ѡ��BIt�����ToneMap��1->�̶����Ʒ�ʽ��{0}
    HI_U8    NonRoboFixedQam;        //Sof֡ ��ROBOģʽ�ĵ��Ʒ�ʽ��0x1��BPSK, 0x2��QPSK, 0x3��8QAM,
                                     // 0x4��16QAM, 0x6��64QAM, 0x8��256QAM��{1}

    HI_U8    GiFcAndD1D2 ;           //FC��Payloadǰ�������ŵ�GIֵ ��1->264;2->300;3->400;4->458;5->624��{4}
    HI_U8    GiSr;                   //SR��GIֵ��0->190;1->264;2->300;3->400;4->458;5->624��{1}
    HI_U8    GiMr;                   //MR��GIֵ��0->190;1->264;2->300;3->400;4->458;5->624��{1}
    HI_U8    GiHr;                   //HR��GIֵ��0->190;1->264;2->300;3->400;4->458;5->624��{1}

    HI_U8    Ri;                     //���������0->0,1->124�� {1}
    HI_S8    PowerStep[MAX_FREQ_NUM_V100];           //���ʵ�����ӦƵ��1~7{0 3 7 7 8}
    
    HI_U8    FreqTryOrder;           //����Ƶ�κ�{0}
    HI_S8    PowerReduce[MAX_FREQ_NUM_V100];                //���书�ʵ�˥�����ӣ�����Ӳ���Ĳ�ͬ�����ȷ�������÷��书��ʱ�Ƿ�ʹ�ø�����    
}DIAG_CMD_TX_NV_STRU;

/***************************NBI****************************/
typedef struct
{
    HI_U8    NBIDetEn;        //խ�������������ʹ��{1};
    HI_U8    NBIDetModeEn[4]; //խ����ⳡ��(A.B.C.D)����ʹ��
    HI_U8    NFEn;            //NF ��������ʹ��{1};
    HI_U8    NBIFilterEn[3];  //Notch Filterʹ��{1,1,1};
    HI_U8    NBIToneNullEn[7];//ToneNullingʹ��{1,1,1,1,1,1,1};

    HI_U8    NF_NBINum;       //Notch Filter����NBI�������Ŀ  {3};
    HI_U8    AvgNumSym;       //����NBI���ķ�����Ŀ�����֧��4������{2};
    HI_U8    NBIThreshold1;   //NBI�������1�������ޣ�������ֵ{10};
    HI_U8    NBIThreshold2;   //NBI�������2�������ޣ�������ֵ{4};

    HI_U8    NBIStep;         //1024��FFTʱ����NBI�Ŀ��{10}
    HI_U8    Const;           //��ƽ��PSDʱ�����Եĳ�������{2}��
    HI_U8    Rsd1;
    HI_U8    NBI_B_T;         //NF֧�ִ������NBI�Ĵ��� {111}

    HI_U8    Scale;           //ƽ�����ʵı�������{1};
    HI_U8    NBI_NF_Tn_En;    //����NF�˲�֮���NBI�Ƿ���д���ʹ�ܱ�־{1}
    HI_U16   PowTh;          //NBI��⹦������10

    HI_U16   Dfs;             //NF�˲����Ĳ���1{350};
    HI_U16   Msdf;            //NF�˲����Ĳ���2{64860};

    HI_U16   Bsl;             //NF�˲����Ĳ���3{65077};
    HI_U16   Msd;             //NF�˲����Ĳ���4{65535};

    HI_U32   R;               //NF�˲����Ĳ���5{65077};
    HI_U16   Nsubset;         //1024��FFTʱ����ͳ�ƹ����Ӽ��Ĵ�С��8-512��{128};
    HI_U8   Rsd7;
    HI_U8    Rsd0;              //����

    HI_U8    BackToneMaskEn;   //���ñ���Tone Mask
    HI_U8    NBITNEn;         //ToneNull ��������ʹ��{1}
    HI_U8    OmReportSwitch;    //��ά�ɲ���ϱ�����
    HI_U8    Rsd3;              //����0
    HI_U8    Delat_CS_Th;       //State3��State4�Ĵ����Ƚ���ֵ
    HI_U8    NBICS3_0_Th;       //NBI��state3~state0�����жϵ���ֵ
    HI_U16   Rsd2;              //����
    HI_U32   State3ToState0TimerPeriod; //state3~state0��״̬�仯���Ӷ�ʱ��������
    HI_S16   Rsd4[4];
    HI_S16   Rsd5[2][2];   //����NFϵ���ķ�Ŀ {0,0,0,0,0,0};
    HI_U32   Rsd6;

    HI_U32   Timer_th;        //��ʱ��������NBI����ʱ�������ޣ���λms��ȱʡΪ300000ms
}DIAG_CMD_NBI_PARA_NV_STRU;

/***************************��HSO��NV�ӿڽṹ��****************************/
typedef struct
{
    DIAG_CMD_AGC_PARA_NV_STRU    AgcPararNv;
    DIAG_CMD_TX_NV_STRU          TxNv;     
    DIAG_CMD_NBI_PARA_NV_STRU    NbiNv;
} PHY_NV_STRU;

/************************************************************
                             �ӿں�������
 ************************************************************/

#ifdef __cplusplus
}
#endif


#endif


