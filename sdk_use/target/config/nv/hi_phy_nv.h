/******************************************************************************

                  版权所有 (C), 2001-2011, 华为技术有限公司

 ******************************************************************************
  文 件 名   : nv.h
  版 本 号   : 初稿
  作    者   : 宋树臣
  生成日期   : 2011年11月15日
  最近修改   :
  功能描述   : PHY_NV
  函数列表   :
  修改历史   :
  1.日    期   : 2011年11月15日
    作    者   : 宋树臣
    修改内容   : 创建文件
--------------------------------------------------------------------------------

******************************************************************************/
#ifndef PHY_NV_H
#define PHY_NV_H

/************************************************************
                     包含其它模块的头文件
************************************************************/
#include "hi_types.h"

#ifdef __cplusplus
extern "C"{
#endif

/************************************************************
                               宏定义
************************************************************/
//NV项变更的ID编号基值
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
//NV项变更的ID编号的最大值
#define    DIAG_CMD_MAX_NV_ID               DIAG_CMD_CONGREGATE_NV_ID


#define FREQ_TRY_NUM                      6                 //盲检频段个数
#define MAX_FREQ_NUM_V100                 (7)               // V100频段个数 (7)

// V100原NV
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
                             数据结构定义
************************************************************/

/***************************AGC****************************/
typedef struct
{
   HI_U8  Quick_Adj_Win;        //AAGC快调平滑统计点数{2};
   HI_U8  Quick_Sat_Win;        //快速检测饱和统计点数{0};
   HI_U8  Sat_Statics_Mode;     //饱和率统计方式{0};
   HI_U8  PowStaway;            //功率上报方式{0};

   HI_U8  AAGCMode;             //AAGC工作模式{15};3
   HI_U8  PowTarget;            //目标功率；{38};
   HI_U8  PowTarget_QuickAdj;   //快调阶段目标功率{38};
   HI_U8  ThrsSatPow;           //饱和功率门限{50};

   HI_U16 SatSubAAGC;          //信号饱和时AAGC增益负补偿值{320};
   HI_U8  ThrsSatPowNBI;        //NBI检测复位的饱和功率门限{48}
   HI_U8  Thrs_quick_adj;       //快调阶段AAGC调整门限；{5};

   HI_U8  Thrs_preamble_adj;    //preamble阶段AAGC调整门限；{0};
   HI_U8  Thrs_isp_adj;         //ISP检测AAGC调整门限；{4}
   HI_U8  Diff_Pow_Thresh;      //窄带干扰的AAGC补偿门限；{10};
   HI_U8  Compen_AAGC;          //窄带干扰的AAGC补偿值；{80};

   HI_U8  AGC_set_slot;         // AAGC调整稳定时间;{100};
   HI_U8  Rsd0;
   HI_U8  PowOffset[10];        //AD功率补偿{0,0,0,1,1,2,3,5,10,20};

   HI_U16 ADPeakValue;          //AD参考电压{750};
   HI_U16 MaxAAGC;              //AGC最大Gain值{768};
   HI_S16 MinAAGC;              //AGC最小Gain值{-192}
   HI_S16 Fix_AAGC;             //固定AAGC增益；{320};

   HI_S16 InitialAAGC;          //初始AAGC增益；{320};
   HI_U8  DAGC_offset;          //DAGC偏移因子{0};
   HI_U8  DAGCByPass;          //DAGC使能{0};

   HI_U16 Fix_DAGC_Gain;       //固定DAGC调整配置增益{0};
   HI_U8  Fix_DAGC_Enable;     //固定DAGC调整使能开关{0};
   HI_U8  Fix_AAGC_Enable;      //固定AAGC增益配置使能开关;{0};

   HI_U16 Max_DAGC;            //最大DAGC因子{480};
   HI_U8 PowTarget_ISP;      //ISP模式下目标功率{38}
   HI_U8 ISP_Adj_Win;        //ISP模式下统计功率窗口长度{2}

   HI_U8 HiAFEStep;       //AFE调整步长精度{2}:0代表步长1，code负增长;1代表步长2，code负增长；
                            //2代表步长1，code正增长；3代表步长2，code正增长；
                            //由AD确定，AD9866配2，自研配3，其他需按以上要求配
   HI_U8 ISP_QuickAdj_MaxLen;         //ISP模式下快调稳定时间{5}

   HI_U8 PowOffset_ISP[10];  //ISP模式下AD功率补偿{0,0,0,1,1,2,3,5,10,20};
}DIAG_CMD_AGC_PARA_NV_STRU;

/***************************TX****************************/
typedef struct
{
    HI_U8    TxTestEn;               //TX端数据导出（到逻辑分析仪）使能{1}
    HI_U8    SyncCpNum;              //SyncCP的个数（8->8,9->9,10->10,11->11,12->12,13->13,14->14,15->15,16->16）{10}
    HI_U16   SoundingSymbolNum;      //Sounding帧的符号个数{64}（该参数值配置限制为2的n次方）

    HI_U8    SofNonRoboCodeRate;     //Sof帧非ROBO模式的码率（0->1/2,1->16/21,2->16/18）{0}
    HI_U8    FcQam;                  //FC的调制方式（0：QPSK；1：BPSK）{0}
    HI_U8    NonRoboQamType;         //Sof帧 非ROBO模式调制方式选择（0->选用BIt分配的ToneMap表；1->固定调制方式）{0}
    HI_U8    NonRoboFixedQam;        //Sof帧 非ROBO模式的调制方式（0x1：BPSK, 0x2：QPSK, 0x3：8QAM,
                                     // 0x4：16QAM, 0x6：64QAM, 0x8：256QAM）{1}

    HI_U8    GiFcAndD1D2 ;           //FC和Payload前两个符号的GI值 （1->264;2->300;3->400;4->458;5->624）{4}
    HI_U8    GiSr;                   //SR的GI值（0->190;1->264;2->300;3->400;4->458;5->624）{1}
    HI_U8    GiMr;                   //MR的GI值（0->190;1->264;2->300;3->400;4->458;5->624）{1}
    HI_U8    GiHr;                   //HR的GI值（0->190;1->264;2->300;3->400;4->458;5->624）{1}

    HI_U8    Ri;                     //滚降间隔（0->0,1->124） {1}
    HI_S8    PowerStep[MAX_FREQ_NUM_V100];           //功率调整对应频段1~7{0 3 7 7 8}
    
    HI_U8    FreqTryOrder;           //测试频段号{0}
    HI_S8    PowerReduce[MAX_FREQ_NUM_V100];                //发射功率的衰减因子，根据硬件的不同，软件确定在设置发射功率时是否使用该因子    
}DIAG_CMD_TX_NV_STRU;

/***************************NBI****************************/
typedef struct
{
    HI_U8    NBIDetEn;        //窄带检测整个功能使能{1};
    HI_U8    NBIDetModeEn[4]; //窄带检测场景(A.B.C.D)功能使能
    HI_U8    NFEn;            //NF 整个功能使能{1};
    HI_U8    NBIFilterEn[3];  //Notch Filter使能{1,1,1};
    HI_U8    NBIToneNullEn[7];//ToneNulling使能{1,1,1,1,1,1,1};

    HI_U8    NF_NBINum;       //Notch Filter抑制NBI的最大数目  {3};
    HI_U8    AvgNumSym;       //用于NBI检测的符号数目，最大支持4个符号{2};
    HI_U8    NBIThreshold1;   //NBI检测门限1（主门限），线性值{10};
    HI_U8    NBIThreshold2;   //NBI检测门限2（辅门限），线性值{4};

    HI_U8    NBIStep;         //1024点FFT时相邻NBI的跨度{10}
    HI_U8    Const;           //求平均PSD时所乘以的常数因子{2}；
    HI_U8    Rsd1;
    HI_U8    NBI_B_T;         //NF支持处理最大NBI的带宽 {111}

    HI_U8    Scale;           //平均功率的比例因子{1};
    HI_U8    NBI_NF_Tn_En;    //经过NF滤波之后的NBI是否进行处理使能标志{1}
    HI_U16   PowTh;          //NBI检测功率门限10

    HI_U16   Dfs;             //NF滤波器的步长1{350};
    HI_U16   Msdf;            //NF滤波器的步长2{64860};

    HI_U16   Bsl;             //NF滤波器的步长3{65077};
    HI_U16   Msd;             //NF滤波器的步长4{65535};

    HI_U32   R;               //NF滤波器的步长5{65077};
    HI_U16   Nsubset;         //1024点FFT时用于统计功率子集的大小（8-512）{128};
    HI_U8   Rsd7;
    HI_U8    Rsd0;              //保留

    HI_U8    BackToneMaskEn;   //启用备用Tone Mask
    HI_U8    NBITNEn;         //ToneNull 整个功能使能{1}
    HI_U8    OmReportSwitch;    //可维可测的上报开关
    HI_U8    Rsd3;              //保留0
    HI_U8    Delat_CS_Th;       //State3与State4的次数比较阈值
    HI_U8    NBICS3_0_Th;       //NBI的state3~state0策略判断的阈值
    HI_U16   Rsd2;              //保留
    HI_U32   State3ToState0TimerPeriod; //state3~state0的状态变化监视定时器的周期
    HI_S16   Rsd4[4];
    HI_S16   Rsd5[2][2];   //配置NF系数的分目 {0,0,0,0,0,0};
    HI_U32   Rsd6;

    HI_U32   Timer_th;        //定时启动两次NBI检测的时间间隔门限，单位ms，缺省为300000ms
}DIAG_CMD_NBI_PARA_NV_STRU;

/***************************与HSO的NV接口结构体****************************/
typedef struct
{
    DIAG_CMD_AGC_PARA_NV_STRU    AgcPararNv;
    DIAG_CMD_TX_NV_STRU          TxNv;     
    DIAG_CMD_NBI_PARA_NV_STRU    NbiNv;
} PHY_NV_STRU;

/************************************************************
                             接口函数声明
 ************************************************************/

#ifdef __cplusplus
}
#endif


#endif


