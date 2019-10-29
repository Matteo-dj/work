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
#ifndef PHY_NV_V200_H
#define PHY_NV_V200_H

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
//#define DIAG_CMD_FREQ_PARA_NV_1_ID_V200          0x0063 //DIAG_CMD_FREQ_PARA_NV的扩展
#define DIAG_CMD_FREQ_POWER_ID_V200             0x0067
#define DIAG_CMD_AFE_PCS_NV_ID_V200                  0x0068
#define DIAG_CMD_TONEMASK_NV_ID_V200             0x0069
#define DIAG_CMD_TF_REG_PARAM_NV_ID_V200            0x006a  // 台区识别寄存器参数配置
#define DIAG_CMD_TF_REG_IIR_PARAM_NV_ID_V200           0x006b  // 台区识别iir寄存器参数配置
//NV项变更的ID编号的最大值
#define    DIAG_CMD_MAX_NV_ID_V200               DIAG_CMD_TF_REG_IIR_PARAM_NV_ID_V200

#define FREQ_CH_NUM_MAX                     (13)                     //频段并行检测的通道最大个数
#define MAX_FREQ_NUM                        26                      //频段个数,此宏与FREQ_TRY_ORDER_MAX对应
#define HGT_POWER_LOADING_NUM_MAX           (512)

/************************************************************
                             数据结构定义
************************************************************/

/***************************AGC****************************/
typedef struct
{
    HI_U8 AAGCMode;// AAGC工作模式
    HI_U8 ThrsSatPow;// 饱和功率门限
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
    HI_U8 DAGC_offset;// DAGC偏移因子;
    HI_U8 DAGCByPass;// DAGC使能;
    HI_S8 Fix_DAGC_Gain;// 固定DAGC调整配置增益;

    HI_U8 Fix_DAGC_Enable;// 固定DAGC调整使能开关;
    HI_U8 Max_DAGC;// 最大DAGC因子;
    HI_U8 DagcSt0Target;
    HI_U8 AFEStep;// AFE调整步长精度:0代表步长1，code负增长;1代表步长2，code负增长；2代表步长1，code正增长；3代表步长2，code正增长；由AD确定，AD9866配2，自研配3，其他需按以上要求配

    HI_U8 st23useNum;
    HI_U8 st23WinSize;
    HI_U8 SetDagcPow;//手动设置Dagc目标功率使能 {0,1}
    HI_U8 DagcPowTarget;//手动设置Dagc目标功率

    HI_U8 st23UseNum_post;//滤波后状态2、3功率统计使用窗个数 {1~3} 实际用值为设置值2的方幂
    HI_U8 st23WinSize_post;//滤波后状态2、3功率统计使用窗长度 {5~10}    实际用值为设置值2的方幂
    HI_U8 TXPower;//设置信号发射功率
    HI_U8 ChanDecScale;//估计噪声比例因子

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
    HI_U16 PreamSatThres[3];                      //Preamble统计饱和门限1{100,60,20}

    HI_U8 SatAdjEn;                                     //统计功率饱和强制复位帧监听使能{0}
    HI_U8 AGCcompenrate[3];                       //AGC增益补偿率1{20,15,5}

    HI_U8 DagcSt0TargetDet;                       //dagc状态0给帧监听通道目标功率{43}
    HI_U8 Rsvd[3];
}DIAG_CMD_AGC_PARA_NV_STRU_V200;//64byte

/***************************TX****************************/
typedef struct
{
    HI_U32 freqmode_FC_COPY_Offset             :2;         //FC拷贝移位模式{2,0,2,1,1,1,1,1,1,1,1,1,1}
    HI_U32 FCPLPhaseShift                                 :2;         //FCPL数据相位旋转模式{2,0,2,1,1,1,1,1,1,1,1,1,1}
    HI_U32 PreambleMode                                 :2;         //前导采用的频域序列模式{1,0,1,1,1,1,1,1,1,1,1,1,1}
    HI_U32 GWV200TMIMode                            :1;         //国网和V200采用的TMI表模式{1,0,1,0,0,0,0,0,0,0,0,0,0}
    HI_U32 BroadCastMaskEn                            :1;          //BroadCastMask使能{1,1,1,1,1,1,1,1,1,1,1,1,1}
    HI_U32 rsvd                                                   :24;       //保留
}DIAG_CMD_TX_NV_FOR_FREQ_STRU_V200;

typedef struct
{
    HI_S8    PowerIndex[4];                                  // 频段1 ~13的发射端功率{9,14,17,20}

    HI_U8    FixAfeParamEn;                                //固定AFEHpf使能开关{0}
    HI_U8    txFreq;                                              // 发送频段编号,取值:0-25{1}
    HI_U8    RI;                                                      //滚降间隔（0->0,1->124） {1}
    HI_U8    toneMaskMode;                              // ToneMask模式，0:正常模式；1:欧洲模式{0}

    HI_U8    PowerloadingEn;                             //{1}
    HI_U8    UseB4AFEParam;                             // AFE滤波器模式配置开关{1}
    HI_U8    SyncCpNum;                                    // SyncCp 的个数{10}
    HI_U8    Preamble_Only_Ind;                       //是否只发送Preamble {0}

    HI_U8    TX_Test_En;                                     // 发射端测试模式使能{0}
    HI_U8    TX_Test_Mode;                               // 发射端测试模式{0}
    HI_U8    FixAfeParam;                                   //固定AFEHpf模式{0}
    HI_U8    PreamAddWin;                               //{0}

    HI_U16   GI_FC_D1_D2 ;                                //FC和Payload前两个符号的GI值 （1->264;2->300;3->400;4->458;5->624）{458}
    HI_U16   GI_Sounding;                                 //{264}

    HI_U16   GI_HR;                                            //{264}
    HI_U16   GI_MR;                                           //{264}

    HI_U16   GI_SR;                                             //{264}
    HI_U8   reducePARdB;                                 //{0}

    HI_U8   fix_freq_en                    : 1;            //接收固定频段开关(即并行检测是否关闭){0}
    HI_U8   tx_hpf_mode                 : 1;           //0
    HI_U8   tx_lpf_mode                  : 1;           //0
    HI_U8   tx_hpf_en                      : 1;           //0
    HI_U8   reducePAREn                : 1;           //0
    HI_U8   isFreqParaTableEn        : 1;           //频段参数配置表使能标识{1}
    HI_U8   Gain0p8dBBypass          :1;            //Preamble和FC提升0.8dB功率的功能Bypass{0}
    HI_U8   Rsvd1                             : 1;           //保留{0}

    HI_U16 V200_FC_GI;                                  //V200频段的FC的GI{458}
    HI_U16 V200_D1D2_GI;                            //V200频段的第1、2个PL符号的GI{458}

    HI_U16 GW_FC_GI;                                    //国网频段的FC的GI{458}
    HI_U16 GW_D1D2_GI;                              //国网频段的第1、2个PL符号的GI{458}

    HI_U8 V100ScrambleMode                    :1;        //V100频段的PL扰码模式{0}
    HI_U8 V200ScrambleMode                    :1;        //V200频段的PL扰码模式{1}
    HI_U8 V200MPDUScrambleEn               :1;        //V200对MPDU扰码使能{1}
    HI_U8 GWMPDUScrambleEn                 :1;        //国网对MPDU扰码使能{1}
    HI_U8 V200HalfByteShiftMode             :1;        //V200半字节移位模式{1}
    HI_U8 V200STableMode                        :1;        //V200Turbo码S表模式{1}
    HI_U8 V200ROBOInterleaverMode       :1;        //V200ROBO交织模式{1}
    HI_U8 GWROBOInterleaverMode         :1;        //国网ROBO交织模式{1}

    HI_U8 V200_1816PuncTureMode          :1;       //V200的16/18码率打孔模式{1}
    HI_U8 GW_1816PuncTureMode            :1;       //国网的16/18码率打孔模式{1}
    HI_U8 Rsvd2                                             :6;
    HI_U8 Par_Th;                                                   //判断虚警的峰均比门限{0}
    HI_U8 Rsvd3;

    DIAG_CMD_TX_NV_FOR_FREQ_STRU_V200 Tx_Param[FREQ_CH_NUM_MAX];    //对应符号同步频段相关C寄存器
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
    HI_U16 wf;                              // 一阶滤波参数
    HI_U16 freqerr_sliding_exception_th;     // 频偏异常门限

    HI_U16 freqerr_preamble_evalu_diff_th; // preamble频偏估计偏差允许范围
    HI_S16 freqerr_ntb_evalu_max_th;       // NTB估计单帧频偏允许范围 <= 180ppm

    HI_S16 freqerr_ntb_evalu_min_th;       // NTB估计单帧频偏允许范围 >= -180ppm
    HI_U8 freqerr_exception_cnt_th   ;    // 连续频偏异常数量门限
    HI_U8 sliding_filtering_wnd_size ;    // 滑动平均窗口大小

    HI_U32 rcv_bcn_interval_max_th;         // 信标帧间隔上限
    HI_U32 rcv_bcn_interval_min_th;         // 信标帧间隔下限
} DIAG_CMD_NTB_CALC_NV_STRU_V200;

typedef struct
{
    HI_U8 noisecoeff;          //  {2}
    HI_U8 FCMaxIterCount;      // FC 最大迭代次数 {5}
    HI_U8 ExInfoScale;         //Turbo译码extrinsic information缩放系数{96}
    HI_U8 mmibgain;            //译码比特和llr符号不同时增益

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
    HI_U8 mvNoise_En;             //  去噪是否使能{1}
    HI_U8 mvNoise_Mode;           //  去噪模式{0}
    HI_U8 isSetFirstPathLen;      // 是否设置固定的首径位置  {1}
    HI_U8 SetFirstPathLen;        //  设置固定的首径位置{50}

    HI_U8 SearchPathMethod;       // 搜索径的模式{0}
    HI_U8 minFirstPath;           // 首径的最小位置 {40}
    HI_U8 maxFirstPath;           // 首径的最大位置 {100}
    HI_U8 minLastPath;            // 尾径的最小位置 {40}

    HI_U8 maxLastPath;            //   尾径的最大位置 {100}
    HI_U8 reciNoiseEn;            //  噪声区间长度的倒数{10}
    HI_U8 T2;                     // 信道估计在CIR 的前T3 点和后T2 点找首径和尾径，此时CIR 最大径移到位置0 {50}
    HI_U8 T3;                     //信道估计在CIR 的前T3 点和后T2 点找首径和尾径，此时CIR 最大径移到位置0  {50}

    HI_U8 T4;                     // 在基于平均噪声幅度求门限时, 门限为平均噪声幅度*T4/16 {64}
    HI_U8 T5;                     //  在基于最大径幅度求门限时, 门限为平均噪声幅度*T5/256 {64}
    HI_U8 T6;                     //  在计算去噪声门限时统计噪声使用的噪声区间起点为T6{50}
    HI_U8 T7;                     // 在计算去噪声门限时统计噪声使用的噪声区间终点为多径长度-T7-1 {50}

    HI_U8 T8;                     // 在信道估计得时域去噪中，去噪声门限为噪声区间的平均噪声幅度*T8/16 {64}
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


//载波侦听相关因子NV项
typedef struct
{
    HI_U8 Corr1PowScale;                                                 // 相关器1 的基本门限 {13}
    HI_U8 Large_Corr1PowScale;                                      //  相关器1 的大门限{35}
    HI_U8 Corr1State4PowScale;                                      //  相关器1 的statr4 基本门限{13}
    HI_U8 Large_Corr1State4PowScale;                           //  相关器1 的state4 的大门限{35}

    HI_U8 Large_Corr1;                                                       // 用于判断高SNR 状态的大门限{75}
    HI_U8 ValidNumScale2;                                                //每路宽带帧监听中子相关的点数大门限系数{70}
    HI_U8 SynCPNumThresh;                                             //从状态3进入状态4时出现负峰的某路宽带帧监听要求检测到的SynCP的最小个数{4}
    HI_U8 ValidNumScale2st3;                                          //50

    HI_U8 Small_Corr1State4PowScale;                           // 13
    HI_U8 Corr2PowScale;                                                 // 相关器2 的基本门限 {16}{32}
    HI_U8 Corr2State4PowScale;                                      //相关器2的State4的基本门限{18}{32}
    HI_U8 st3SynCPNumThresh                           :3;                      //判断进入状态4时要求从状态3开始的有效SynCP个数的门限{1}
    HI_U8 scale_1_2_3                                          :5;          //corr1_re_1_2_3与相关2或3比较时的scale{8}

    HI_U32 Corr1SumPowScale                            :7;          //用于产生Thres02_03_13正常门限值的比例系数{10}
    HI_U32 Large_Corr1SumPowScale                 :7;          //用于产生Thres02_03_13大门限值的比例系数{10}
    HI_U32 rsd2                                                      :18;

    HI_U32 scale_01_12_23                                   :5;          //corr1_re_01_12_23与相关2或3比较时的scale{8}
    HI_U32 scale_02_03_13                                   :5;          //corr1_re_02_03_13与相关2或3比较时的scale{8}
    HI_U32 N1cntThres                                          :6;          //5
    HI_U32 corr2_len_div2                                    :8;          // 104
    HI_U32 corr3_len_div2                                    :8;          // 178
}DIAG_CMD_CARRIER_SENSE_CORR_SCALE_STRU_V200;

typedef struct
{
    HI_U32 FilterOut2Corr1           :5;                     //滤波器组输出到相关1的映射关系{0}
    HI_U32 FilterOut2Corr2           :5;                     //滤波器组输出到相关2的映射关系{0}
    HI_U32 FilterOut2Corr3           :5;                     //滤波器组输出到相关3的映射关系{0}
    HI_U32 FilterOut2Corr4           :5;                     //滤波器组输出到相关4的映射关系{0}
    HI_U32 FilterOut2Corr5           :5;                     //滤波器组输出到相关5的映射关系{0}
    HI_U32 FilterOut2Corr6           :5;                     //滤波器组输出到相关6的映射关系{0}
    HI_U32 Rsvd                             :2;                     // 0
}DIAG_CMD_CARRIER_SENSE_FILTER_OUT_STRU_V200;

typedef struct
{
    HI_U32 corrflt1En                                     : 1;    //第一路帧监听通路的开关 {1}
    HI_U32 corrflt2En                                     : 1;   //第二路帧监听通路的开关 {1}
    HI_U32 corrflt3En                                     : 1;   //第三路帧监听通路的开关 {1}
    HI_U32 corrflt4En                                     : 1;   //第四路帧监听通路的开关 {1}
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


//载波侦听NV
typedef struct
{
    DIAG_CMD_CARRIER_SENSE_CORR_SCALE_STRU_V200 wbCorrScale;            //宽带参数,{13,35,13,35,75,70,4,50,11,16,18,1,13,35,0}
    DIAG_CMD_CARRIER_SENSE_CORR_SCALE_STRU_V200 nbCorrScale;            //小宽带参数,{13,35,13,35,75,70,4,50,11,32,32,1,13,35,0}
    DIAG_CMD_CARRIER_SENSE_FREQ_INDEX_1_3_STRU_V200 freqindex_1_3;        //通道下变系数1-3 {-34,-68,-102}
    DIAG_CMD_CARRIER_SENSE_FREQ_INDEX_4_6_STRU_V200 freqindex_4_6;        //通道下变系数4-6 {-136,-170,-204}
    DIAG_CMD_CARRIER_SENSE_CHANNEL_EN_STRU_V200 csChlEn;
    DIAG_CMD_CARRIER_SENSE_FREQ_FILTER_0_4_STRU_V200 freqmodefilterarray_0_4;      //载波侦听不同频段下的检测通道映射 {62,60,60,30,3}
    DIAG_CMD_CARRIER_SENSE_FREQ_FILTER_5_9_STRU_V200 freqmodefilterarray_5_9;      //载波侦听不同频段下的检测通道映射 {12,48,1,2,4}
    DIAG_CMD_CARRIER_SENSE_FREQ_FILTER_10_12_STRU_V200 freqmodefilterarray_10_12;   //载波侦听不同频段下的检测通道映射 {8,16,32}
    DIAG_CMD_CARRIER_SENSE_FILTER_OUT_STRU_V200 FilterOut2Corr;     ////滤波器组输出到相关n的映射关系

    HI_U8 above_large_cont_cnt_Thres;                     //  帧监听高SNR/低SNR 状态切换时，连续门限 {2}
    HI_U8 DetectFFTNum;                                            // 相关值超过设定门限的1024 点的块，至少连续出现DetectFFTNum 个，从状态2 进入状态3 {4}
    HI_U16 PowThres;                                                  // 功率门限 {1}

    HI_U16 AGCStableNum;                                        // 自相关等待AGC 稳定的点数 {100}
    HI_U8 St4corr2selthres;                                       //210
    HI_U8 State2_max_tol_thres;                             //6

    HI_U8 State2_acc_tol_thres;                              // 3
    HI_U8 State2_cont_tol_thres;                            // 2
    HI_S16 freqindex0;                                          //通道下变系数, [-10]

    HI_U8 HighSNRThresState1Thres            : 4;                           // 4
    HI_U8 SelfdImpEn                                     : 1;                          //帧监听中脉冲处理使能{0}
    HI_U8 corr2En                                           : 1;                          // 1
    HI_U8 corr3En                                           : 1;                          // 1
    HI_U8 SelBetterSeldetectEn                  : 1;                           // 1

    HI_U8 st2tost3freqgroupEn                    : 1;                          //  1
    HI_U8 St4TimeOutEn                               : 1;                         // 1
    HI_U8 HighSNRState3DelayParam         : 4;                        // 3
    HI_U8 State3DelayParam                        : 2;                       //直接配置状态3等状态4的额外增加的超时时间{1}

    HI_U8 ValdNumScale;                                                       //  帧监听0/1/2/3/4 状态切换时，点数的相对门限{102}
    HI_U8 Large_ValidNumScale;                                          // 帧监听高SNR/低SNR 状态切换时，点数的相对门限 {120}

    HI_U8 above_large_acc_cnt_Thres;                               //  帧监听高SNR/低SNR 状态切换时，累计门限 {3}
    HI_U8 isFixParallelDetect800KFreq;                  //是否固定频段资源8到13之一用于并行检测{0}
    HI_U8 FixParallelDetect800KFreq;                    //固定频段资源8到13之一用于并行检测{0}
    HI_U8 isFixParallelDetect1p6MFreq;                //是否固定频段资源5到7之一用于并行检测{0}

    HI_U8 FixParallelDetect1p6MFreq;                  //固定频段资源5到7之一用于并行检测{0}
    HI_U8 SelfdImpLog2ThrAvgNum;                    //帧监听中平均幅度统计点数{9}
    HI_U8 Rsvd2[2];                                                     // 保留0

    HI_U8 Sum_corr1_02_03_13_highThres         :4;                      //corr1_02_03_13求和的正数的饱和门限比例系数{15}
    HI_U8 Sum_corr1_02_03_13_lowThres          :4;                      //corr1_02_03_13求和的负数的饱和门限比例系数{15}
    HI_U8 Sum_corr1_02_03_13_satEn                : 1;                     //在对corr1_02_03_13求和时，是否作饱和处理{0}
    HI_U8 SelfdImpSet0WinLen                            : 5;                      //帧监听中脉冲拍0窗长{7}
    HI_U8 Rsvd8                                                      : 2;
    HI_U8 SelfdImpThrScaleH;                                                        //帧监听中高门限的调整参数{70}
    HI_U8 SelfdImpThrScaleM;                                                       //帧监听中中门限的调整参数{70}

    HI_U32 bypass2corr                                         :1;         //bypass使用2个相关判断合法SynCP的方法{0}
    HI_U32 SelfdImpCntSmallValThr                     :9;         //帧监听IMP消除的小值阈值{2}
    HI_U32 SelfdImpCntSmallRatThr                    :4;        //帧监听IMP消除的小值比例{4}
    HI_U32 Rsvd3                                                   :18;
}DIAG_CMD_CARRIER_SENSE_NV_STRU_V200;//100  byte

typedef struct
{
    HI_U8 LpfEn;                                // 低通滤波器使能 {1}
    HI_U8 HpfBypass;                            // 高通滤波器Bypass{0}

    HI_U8 AutoSelectBypass2                 :1;                    // 并行检测成功后HPF及LPF是否切换滤波器,0：切换；1：不切换{0}
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
    HI_U8 rsvd1                                          :4;                     //保留

    HI_U8 RXCubicEn;                            //RX端Cubic插值滤波器使能开关{1}
    HI_U8 TXCubicEn;                            //TX端Cubic插值滤波器使能开关{1}
    HI_U8 rsvd2[2];                            // 保留位{0}
}DIAG_CMD_TD_PROCESS_NV_STRU_V200;//8byte

/***************************符号同步****************************/
typedef struct
{
    HI_U32 datastart                                         :9;                  // 频段资源1  ~13的起始子载波编号{80,100,100,61,30,98,166,29,63,97,131,165,199}
    HI_U32 datastop                                          :9;                 // 频段资源1 ~13 的截止子载波编号{490,230,230,191,93,161,229,60,94,128,162,196,230}
    HI_U32 ValidCarrierNum                             :9;                 // 频段资源1~13 的有效子载波数[411,131,131,131,64,64,64,32,32,32,32,32,32]
    HI_U32 V200_FC_BPSK_En                          :1;                 //V200频段资源1  ~13的FC使用BPSK的使能{0}
    HI_U32 V200_PreambledB                          :3;                 //频段资源1  ~13的Preamble功率提升几dB{0}
    HI_U32 is256IFFT                                          :1;                 //频段资源1  ~13的Robust模式使能开关{0}

    HI_U32 LpfMode                                          :4;                // 低通滤波器选择{9},取值从1开始         {9,8,8,7,2,6,8,1,2,4,6,7,8}
    HI_U32 HpfMode                                         :4;                // 高通滤波器选择{1},有效取值从1开始{5,6,6,4,2,6,8,2,4,6,7,8,9}
    HI_U32 freqChlMode                                   :2;                // 频段并行检测通道1、2、3的频段模式选择，，0：V100；1：V200；2：国标。{2,0,2,1,1,1,1,1,1,1,1,1,1}
                                                                                              // 受PHY并行检测限制，freqChlMode[1]与freqChlMode[2]不可同时为0或同时为2
    HI_U32 seqshift                                            :9;               // 频段1~13 的Preamble 频域序列V200 的移位值{0,0,0,39,66,2,498,63,29,507,473,259,405}
    HI_U32 V200_FCdB                                       :3;               //频段资源1  ~13的FC功率提升几dB{0}
    HI_U32 FC_Num                                            :6;              // 频段资源1~13 的FC 符号数{4,8,12,12,16,16,16,32,32,32,32,32,32}
    HI_U32 Rsvd                                                  :4;
}DIAG_CMD_SYMALIGN_FOR_FREQ_STRU_V200;//8byte

typedef struct
{
    HI_U8   PARScale ;                                          // 最大径能量的比例因子，用于求PAR{102}
    HI_U8   PreamFFTIndataMode           :2;       // 提升Robust模式性能{2}
    HI_U8   PreamFFTInNumSel               :4;       //PreamFFT靠前符号个数{2}
    HI_U8   Rsvd1                                      :2;       //保留
    HI_U8    isFixFreqParallelDetbypass;             // 固定频段的模式下，并行检测Bypass 控制信号{0}
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
    HI_U8    LPFMode_Default;                           //低通滤波器选择缺省值{8}
    HI_U8    HPFMode_Default;                          //高通滤波器选择缺省值{1}

    DIAG_CMD_SYMALIGN_FOR_FREQ_STRU_V200 Symbol_Align_Param[FREQ_CH_NUM_MAX];
}DIAG_CMD_SYMALIGN_NV_STRU_V200;//124byte

/***************************NBI****************************/
typedef struct
{
    HI_U8    NbiV200En;                               //是否使能NBI {1};
    HI_U8    NBIDetModeEn[4];                  //窄带检测场景(A.B.C.D)功能使能
    HI_U8    NBIFilterEn[3];                         //Notch Filter使能{1,1,1};

    HI_U8    NbiAbsThres;                           //  NBI 检测绝对门限{10}
    HI_U8    NbiWinSize;                             //  NBI 检测中滑动窗大小{33}
    HI_U8    NbiSubSize;                             //  NBI 检测中窗内减掉载波点数{3}
    HI_U8    NF_NBINum;                           //Notch Filter抑制NBI的最大数目  {3};

    HI_U8    NbiStep;                                  // 相邻NBI算作一个宽NBI的步长{10}
    HI_U8    NbiFftNum;                             // 除st3 触发外，其余方式NBI 检测所需FFT 个数{2}
    HI_U8    NfV200En;                               // NV 使能{1}
    HI_U8    NfPowThres;                           // 需要NF 进行滤波数据功率门限{15}

    HI_U8    St3NbiV200En;                       // 状态3 检测是否使能{1}
    HI_U8    St3NbiDetectScale;               //  状态3 检测对应参数{15}
    HI_U8    St3NbiWinSize;                     //  状态3 检测对应参数{49}
    HI_U8    St3NbiSubSize;                      //  状态3 检测对应参数{3}

    HI_U8    St3NbiStep;                          //  状态3 检测对应参数{3}
    HI_U8    St3NfV200En;                       //  状态3 检测对应参数{1}
    HI_U8    St3NfPowThres;                   //  状态3 检测对应参数{20}
    HI_U8    NfMaxNum;                         //  NF支持处理最大NBI的带宽(载波个数) {111}

    HI_U8    NBI_NF_Tn_En;                   //经过NF滤波之后的NBI是否进行处理使能标志{1}
    HI_U8    St3NbiAbsThres;                 //{15}
    HI_U16   Dfs;                                     //NF滤波器的步长1{350};

    HI_U16   Msdf;                                  //NF滤波器的步长2{64860};
    HI_U16   Bsl;                                     //NF滤波器的步长3{65077};

    HI_U16   Msd;                                   //NF滤波器的步长4{65535};
    HI_U16   Rsd1;                                  //保留位{0}

    HI_U32   R;                                       //NF滤波器的步长5{65077};

    HI_U16   NbiMaskStart1;                // 屏蔽NBI 频段1 , 起始载波{0}
    HI_U16   NbiMaskStop1;                // 屏蔽NBI 频段1 , 截止载波{0}

    HI_U16   NbiMaskStart2;                // 屏蔽NBI 频段2 , 起始载波{0}
    HI_U16   NbiMaskStop2;                 // 屏蔽NBI 频段2 , 截止载波{0}

    HI_U8    NBITNEn;                           //ToneNull 整个功能使能{1}
    HI_U8    NbiDetectScale;                //检测中平均功率加上的因子，{10}
    HI_U16   st3to0_det_cnt;

    HI_U32   Timer_th;                         //定时启动两次NBI检测的时间间隔门限，单位ms，缺省为300000ms{10000}

    HI_U16   TD_FrameEndDelay;        // 1024
    HI_U16   AGCWaitNBITime;           // 4096

    HI_U8 NfInputShift;                        // 0
    HI_U8 NbiMaxPowScale;                //NBI计算载波最大值比例因子{8}
    HI_U8 NbiMaxPowScaleEMC;         //用于检测EMC的计算载波最大值比例因子{8}
    HI_U8 Rsvd3;

    HI_U16 NbiStart[3];                         //第0-2组滤波器起始频点{0,0,0}
    HI_U16 NbiStop[3];                          //第0-2组滤波器终止频点{0,0,0}

    HI_U16 NbiSubSizeEMC;                   //用于EMC检测的小窗长度{19}

    HI_U8 NbiFindMaxEn                :1;    //检测NBI时找最大载波位置使能{1}
    HI_U8 NbiAddCarrier                :2;    //在大于最大载波位置增加检测数量{2}
    HI_U8 SetNBIEn                         :1;    //手动设置NBI起止载波范围使能{0}
    HI_U8 Rsvd1                               :4;
    HI_U8 Rsvd2;

    HI_U32 NbiAbsThresMax;                   //5000
    HI_U32 NbiAbsThresMin;                    //10
}DIAG_CMD_NBI_PARA_NV_STRU_V200;

typedef struct
{
    HI_U8 ImpV200En;            // v200 新版本的去脉冲干扰使能
    HI_U8 ImpUsedWinNum;         //  v200 新版本的去脉冲干扰中用于计算门限的窗口数量
    HI_U16 ImpScaleMaxPream;

    HI_U8 ImpMethod;             // v200 新版本的去脉冲干扰中对时域脉冲干扰点的处理方式
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
    HI_U8 ImpSet0WinIndOptionPream;//Preamble阶段，IMP消除窗处理选项    {0,1,2}
    HI_U8 ImpSet0WinIndOptionFCPL;//FCPL阶段，IMP消除窗处理选项  {0,1,2}

    HI_U16 ImpSet0NumThrPream;//Preamble阶段，每1024点IMP可消除数量门限  {0~1025}
    HI_U16 ImpSet0NumThrFCPL;//FCPL阶段，每1024点IMP可消除数量门限   {0~1025}

    HI_U8 ImpWinInd1Set0HalfLenOldPream;//Preamble阶段，消除窗指示1的旧数据半径 {0~39}
    HI_U8 ImpWinInd1Set0HalfLenNewPream;//Preamble阶段，消除窗指示1的新数据半径 {0~39}
    HI_U8 ImpWinInd2Set0HalfLenOldPream;//Preamble阶段，消除窗指示2的旧数据半径 {0~39}
    HI_U8 ImpWinInd2Set0HalfLenNewPream;//Preamble阶段，消除窗指示2的新数据半径 {0~39}

    HI_U8 ImpWinInd3Set0HalfLenOldPream;//Preamble阶段，消除窗指示3的旧数据半径 {0~39}
    HI_U8 ImpWinInd3Set0HalfLenNewPream;//Preamble阶段，消除窗指示3的新数据半径 {0~39}
    HI_U8 ImpWinInd1Set0MethodPream;//Preamble阶段，消除窗指示1的消除方法 {0~7}
    HI_U8 ImpWinInd2Set0MethodPream;//Preamble阶段，消除窗指示2的消除方法 {0~7}

    HI_U8 ImpWinInd3Set0MethodPream;//Preamble阶段，消除窗指示3的消除方法 {0~7}
    HI_U8 ImpWinInd1Set0HalfLenOldFCPL;//FCPL阶段，消除窗指示1的旧数据半径  {0~39}
    HI_U8 ImpWinInd1Set0HalfLenNewFCPL;//FCPL阶段，消除窗指示1的新数据半径  {0~39}
    HI_U8 ImpWinInd2Set0HalfLenOldFCPL;//FCPL阶段，消除窗指示2的旧数据半径  {0~39}

    HI_U8 ImpWinInd2Set0HalfLenNewFCPL;//FCPL阶段，消除窗指示2的新数据半径  {0~39}
    HI_U8 ImpWinInd3Set0HalfLenOldFCPL;//FCPL阶段，消除窗指示3的旧数据半径  {0~39}
    HI_U8 ImpWinInd3Set0HalfLenNewFCPL;//FCPL阶段，消除窗指示3的新数据半径  {0~39}
    HI_U8 ImpWinInd1Set0MethodFCPL;//FCPL阶段，消除窗指示1的消除方法

    HI_U8 ImpWinInd2Set0MethodFCPL;//FCPL阶段，消除窗指示2的消除方法
    HI_U8 ImpWinInd3Set0MethodFCPL;//FCPL阶段，消除窗指示3的消除方法
    HI_U16 ImpScaleMinPream;//对于Preamble阶段，V200新版本的去脉冲干扰中计算去干扰门限的比例系数，最小取值

    HI_U8 ImpScaleShiftForNCopiesThr;// 识别拷贝次数少，切换ImpScale，避免信号误拍
    HI_U8 ClImpNCpThr;// 识别拷贝次数少和高速调制方式，关闭IMP，避免信号误拍
    HI_U8 ClImpPlQamMdThr;// 识别拷贝次数少和高速调制方式，关闭IMP，避免信号误拍
    HI_U8 ImpSet0Ind1LowRkForGasPream;

    HI_U8 ImpSet0Ind2LowRkForGasPream;
    HI_U8 ImpSet0Ind3LowRkForGasPream;
    HI_U8 ImpSet0Ind1LowRkForGasFCPL;
    HI_U8 ImpSet0Ind2LowRkForGasFCPL;

    HI_U8 ImpSet0Ind3LowRkForGasFCPL;
    HI_U8 GasRecogLowPowWinNumPream;
    HI_U16 ImpScaleMinFCPL;

    HI_U16 ImpScaleMaxFCPL;
    HI_U8 GasRecogLinearThrPream;         //IMP高斯识别的线性门限参数: 6
    HI_U8 GasRecogWinCntMaxPream;         //IMP高斯识别的线性门限参数: 3

    HI_U8 NonGasRecogWinCntMaxPream;
    HI_U8 GasRecogLowPowWinNumFCPL;
    HI_U8 GasRecogHiPowWinNumFCPL;
    HI_U8 GasRecogLinearThrFCPL;         //IMP高斯识别的线性门限参数: 6

    HI_U8 GasRecogWinCntMaxFCPL;         //IMP高斯识别的线性门限参数: 3

    HI_U8 NonGasRecogWinCntMaxFCPL;
    HI_U8 SatProcModePream;
    HI_U8 SatProcModeFCPL;

    HI_U8 GasRecogHiPowWinNumPream;
    HI_U8 Rsd;
    HI_U16 SatIndThres;                                                     //IMP模块用于计算饱和指示的门限{511}

    HI_U8 FreqPowerThresScaleEn;
    HI_U8 FreqPowerThresScale;
    HI_U16 ClipInd1Scale;

    HI_U16 ClipInd2Scale;
    HI_U16 ClipInd3Scale;
}DIAG_CMD_IMPN_PARA_NV_STRU_V200;// 100

typedef struct
{
    HI_U8 c_tx_fec_rate;//码率（偏移地址0x002C，[5:3]，位宽3）0x0
    HI_U8 c_tx_modu_mode;//调制模式（偏移地址0x002C，[8:6]，位宽3）0x0
    HI_U8 c_tx_mac_length;//MAC数据字节数（偏移地址0x002C，[15:9]，位宽8）0x40
    HI_U8 c_lmt_ratio_th;

    HI_U8 c_hot_rstn;// 热复位 （偏移地址0x0080，[0]，位宽1位）0x1
    HI_U8 c_cool_rstn;// 冷复位 （偏移地址0x0080，[1]，位宽1位）0x1
    HI_U16 c_fs_freq;// (偏移地址0x0000，[9:0]，位宽10位，默认值0x3C)

    HI_U16 c_fc_0;//（偏移地址0x000C，[13:0]，位宽14位， 默认值0x352）
    HI_U16 c_fc_1;// (偏移地址0x000C，[27:14]，位宽14位， 默认值0x379)

    HI_U16 c_fc_2;// (偏移地址0x0010，[13:0]，位宽14位， 默认值0x395)
    HI_U16 c_fc_3;// (偏移地址0x0010，[27:14]，位宽14位， 默认值0x3A9)

    HI_U16 c_fc_4;// (偏移地址0x0014，[13:0]，位宽14位， 默认值0x3C4)
    HI_U16 c_fc_5;// (偏移地址0x0014，[27:14]，位宽14位， 默认值0x3E7)

    HI_U16 c_fc_6;// (偏移地址0x0018，[13:0]，位宽14位， 默认值0x40B)
    HI_U16 c_fc_7;// (偏移地址0x0018，[27:14]，位宽14位， 默认值0x42A)

    HI_U16 c_fc_8;//（偏移地址0x001C，[13:0]，位宽14位，默认值0x44C）
    HI_U16 c_fc_9;//（偏移地址0x001C，[27:14]，位宽14位，默认值0x472）

    HI_U16 c_fc_10;//（偏移地址0x0020，[13:0]，位宽14位，默认值0x487）
    HI_U16 c_fc_11;//（偏移地址0x0020，[27:14]，位宽14位，默认值0x4A1）

    HI_U16 c_fc_12;//（偏移地址0x0024，[13:0]，位宽14位，默认值0x4B6）
    HI_U16 c_fc_13;//（偏移地址0x0024，[27:14]，位宽14位，默认值0x4CE）

    HI_U16 c_fc_14;//（偏移地址0x0028，[13:0]，位宽14位，默认值0x4F1）
    HI_U16 c_fc_15;//（偏移地址0x0028，[27:14]，位宽14位，默认值0x514）

    HI_U8 c_lmt_bpf_bwa;//（[23:20]，位宽4位）0x4
    HI_U8 c_lmt_lmt_1;//（[19:15]，位宽5位）0xe
    HI_U8 c_lmt_st0_1;//（[14:10]，位宽5位）0x18
    HI_U8 c_lmt_lmt_2;//（[9:5]，位宽5位）0x8

    HI_U8 c_lmt_st0_2;//（[4:0]，位宽5位） 0x14
    HI_U8 c_lmt_pre_lmt;//([17:13]，位宽5) 噪声限值相关寄存器2  （偏移地址0x0074）0xa
    HI_U8 c_lmt_pst_lmt;//（[12:9]，位宽4）0x3
    HI_U8 c_lmt_pre_st0;//（[8:4]，位宽5）0x14

    HI_U8 c_lmt_pst_st0;//（[3:0]，位宽4）0x6
    HI_U8 c_si_fir_len;//（[11:10]，位宽2）软信息调整相关寄存器 （偏移地址0x0050）0x1
    HI_U8 c_null_lmt_th;//（[9:7]，位宽3）0x5
    HI_U8 c_si_din_lmt;//（[6:4]，位宽3）0x3

    HI_U8 c_si_max;//（[3:0]，位宽4）0x8
    HI_U8 c_coef_bw;//（[10:8]，位宽3）均衡器相关寄存器（偏移地址0x0060）0x3
    HI_U8 c_hpow_th;//（[7:0]，位宽8）0x2
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
    HI_U8 c_agc_vld_disable;//  （偏移地址0x0080，[9]，位宽1位）0x1

    HI_U8 c_frame_end;                      //6
    HI_U8 c_lmt_hpf_bws;                    // 2
    HI_U16 c_fc_add;                         //载波的偏移量0x0
    HI_U32 c_clk_freq;

    HI_U8 c_pk_gain2;//（偏移地址0x0048，[3:0]，位宽4位，默认值0x0C）
    HI_U8 c_pk_gain1;//（偏移地址0x0048，[7:4]，位宽4位，默认值0x09）
    HI_U8 c_pk_gain0;//（偏移地址0x0048，[11:8]，位宽4位，默认值0x0D）
    HI_U8 c_amp_refer_gain;//（偏移地址0x0078，[3:0]，位宽4位，默认值0x08）

    HI_U8 c_tx_lim_th;//（偏移地址0x0078，[11:4]，位宽8位，默认值0xD4）
    HI_U8 c_mean_dlt;//（偏移地址0x0078，[15:12]，位宽4位，默认值0x8）
    HI_U8 c_lmt_st0_3;//（偏移地址0x0078，[20:16]，位宽4位，默认值0x10）
    HI_U8 c_lmt_lmt_3;//（偏移地址0x0078，[25:21]，位宽5位，默认值0x8）

    HI_U8 c_mean_max_th0;//（偏移地址0x007C，[7:0]，位宽8位，默认值0x00）
    HI_U8 c_mean_min_th0;//（偏移地址0x007C，[15:8]，位宽8位，默认值0x00）
    HI_U8 c_mean_max_th1;//（偏移地址0x007C，[23:16]，位宽8位，默认值0x28）
    HI_U8 c_mean_min_th1;//（偏移地址0x007C，[31:24]，位宽8位，默认值0x0A)

    HI_BOOL is_sc_clk_en;   //窄带时钟是否关闭
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
    HI_U8 isp_tx_cnt;   //B070增加连续发送次数
    HI_U8 is_isp_tx_en;
    HI_U8 is_isp_test_mode;
}DIAG_CMD_ISP_NV_STRU_V200;

/***************************Clock****************************/
typedef struct
{
    HI_S16 Carrier_Set_SNR_Offset;       //原始SNR门限偏移值，unit:1/16dB{0 }
    HI_S16 Carrier_Set_SNR_Th;           //配置的原始SNR门限 {-336 }

    HI_U8  Carrier_Set_SNR_Th_Type;      //原始SNR门限获取方式，1：计算门限值,0：使用配置值  {1 }
    HI_U8  Preamble_FreqErrEstEn;       //Preamble 阶段的频偏估计使能，0：不使能,1：使能 {1, }
    HI_U8  Preamble_FreqErrEn;           //Preamble阶段频偏估计结果使能，0：不使能,1：使能 { 1,}
    HI_U8  m;                            //cLK优化方案中求平均相差时采用的除法前分母取数位宽处理m，取值范围为0-9{6}

    HI_U8 Rsd;
    HI_U8  Rsd0;                         //0
    HI_U8  K4MAWin;                      //频偏滑动平均窗的大小，可配置为0,1,2,3等对应的窗的大小为2,4,8,16 {2 }
    HI_U8  SinguCntTh;                   //允许奇异值连续发生的次数门限 { 5,}

    //此处按块复制入寄存器，顺序不可变
    HI_U16 FreqErr_CapSinguTh[4];        //不同信道场景的捕获阶段，频偏为奇异值的判决门限   {2684,3355,4027,5369}
    HI_U16 FreqErr_TrackSinguTh[4];      //不同信道场景的跟踪阶段，频偏为奇异值的判决门限{1342,2013,2684,3355}
    HI_U8  h_Par_Th[3];                  // 用3个h_Par门限值区分不同信道场景，从而选择不同的滤波系数{60,30,14},

    HI_U8  Rsd10;                         //0
    //此处按块复制入寄存器，顺序不可变
    HI_S16 G1_Capture[4];                //不同信道场景的捕获阶段的G1系数值  {0,0,0,0},
    HI_S16 G2_Capture[4];                //不同信道场景的捕获阶段的G2系数值  {16384,6554,1311,0}
    HI_S16 G1_Track[4];                  //不同信道场景的跟踪阶段的G1系数值  {0,0,0,0},
    HI_S16 G2_Track[4];                  //不同信道场景的跟踪阶段的G2系数值  {0,0,0,0},
    HI_S16 AlphaG1[2];                   //捕获与跟踪阶段的G1系数更新因子值  {26214,13107},
    HI_S16 AlphaG2[2];                   //捕获与跟踪阶段的G2系数更新因子值  {26214,13107},
    HI_U16 FreqErr_Capture_Th[4];        //不同信道场景的捕获阶段的系数更新的频偏门限值 {0,0,0,0}
    HI_U16 FreqErr_Track_Th[4];         //不同信道场景的跟踪阶段的系数更新的频偏门限值 {1342,2013,2684,3355}
    HI_U16 FreqErr_C2T_Th[4] ;           //不同信道场景的捕获转换为跟踪阶的频偏门限值{2013,2684,3355,4027}

    HI_U32 PL_BLER_Th;                   //BLER的门限{100}
    HI_U32 S2WTimerTh;                   //睡眠时间门限，根据此门限选择由Idle跳转到哪个模式: 5s 单位：s {5 }
    HI_U16 Agc_Sat_Th;                   //Preamble符号阶段饱和判断  {512}   20150605修改为1023
    HI_S16 FreqErr_Pre;                  //NV刷新精度门限，当前需要上报频偏值与上次的差别小于此门限时，则不刷新寄存器{671 }

    HI_U8 AssumClkerrMode;
    HI_U8 Rsd1[3];

    HI_S32 AssumClkerrStep;
}DIAG_CMD_CLOCK_NV_STRU_V200;

/***************************Pcs****************************/
typedef struct
{
    HI_U8 BurstSendEndInterrupt                                    :1;        // burst发送完成中断{1}
    HI_U8 BurstRcvEndInterrupt                                       :1;        // burst接收完成中断{1}
    HI_U8 Pb4Interrupt                                                      :1;        // pb4中断{1}
    HI_U8 Pb3Interrupt                                                      :1;        // pb3中断{1}
    HI_U8 Pb2Interrupt                                                      :1;        // pb2中断{1}
    HI_U8 Pb1Interrupt                                                      :1;        // pb1中断{1}
    HI_U8 FcErrInterrupt                                                    :1;        //FC检出错误中断使能{1}
    HI_U8 FcOkInterrupt                                                    :1;        //FC检出正确中断使能{1}

    HI_U8 ParaOkInterrupt                                                :1;        //并行检测中断使能{1}
    HI_U8 RxOverFlowInterrupt                                        :1;        //接收溢出中断{1}
    HI_U8 IspSendEndInterrupt                                         :1;        //isp发送结束的上报中断标志{0}
    HI_U8 OnlyPreambleRcvInterrupt                              :1;        //tx只发送preamble时，rx端接收到preamble的中断标志{0}
    HI_U8 OnlyPreambleSendEndInterrupt                     :1;        //tx只发送preamble时，preamble的发送完成中断标志{0}
    HI_U8 FrameSingalSendStart                                      :1;        //单次发送开始中断使能{1}
    HI_U8 SendWrCollision                                                :1;        //发送写冲突中断标志使能{1}
    HI_U8 Rsd1                                                                    :1;        //保留

    HI_U8 PcsReportStage                                                 :3;        //载波侦听的上报阶段{4}
    HI_U8 ErrPbReport                                                      :1;        //Payload的CRC校验失败时，Payload是否上报的选择。0：不上送;1：上送。{1}
    HI_U8 AbandonedFcReprot                                        :1;        //FC的CRC校验成功但按照处理流程需要舍弃时，FC是否上报的选择。0：不上送；1：上送。{1}
    HI_U8 ErrFcReprot                                                       :1;        //FC的CRC校验失败时，FC是否上报的选择。0：不上送；1：上送。{1}
    HI_U8 Rsd2                                                                   :2;        //保留

    HI_U8 Rsd3;
}DIAG_CMD_PCS_NV_STRU_V200;

typedef struct
{
    DIAG_CMD_PCS_NV_STRU_V200     PcsInterrupt;
    HI_U8    TxAfePgaGain;           //发射的AFE的模拟功率因子:000: -20dB,0001: -18dB,. . .,1010: 0dB{10}
    HI_U8    Rsd[3];
}DIAG_CMD_AFE_PCS_NV_STRU_V200;


//PHY的stub的控制NV
typedef struct
{
    HI_U8  ChipSelectEn;                    //芯片自动筛选开关:0,关闭;1,打开
    HI_U8  ChipSelectTxSwitch;              //芯片自动筛选TX功能启动开关:0,RX;1,TX
    HI_U8  PhyStressEn;                     //phy的压力测试启动开关:0,关闭;1,打开
    HI_U8  PhyStressTxSwitch;               //phy的压力测试开关:0,RX;1,TX

    HI_U8  PhySpeedEn;                      //phy的速率测试启动开关:0,关闭;1,打开
    HI_U8  PhySpeedTxSwitch;                //phy的速率测试开关:0,RX;1,TX
    HI_U8  PowerSaveSwitch;                 //低功耗功能开关:0,关闭;1,打开
    HI_U8  NoiseEn;                         //噪声功能启动开关:0,关闭；1,打开

    HI_U8  Phase;                           //CCO使能开关:0,关闭；1,打开
    HI_U8  CollectDataSwitch;               //采数功能开关:0xa5:打开;其它值:关闭
    HI_U8  PhyDiagnoseSwitch;               //PHY的异常状态检测开关:0,关闭；1,打开
    HI_U8  rsd1;                            //0xa5: AdiAfe;其它:HiAfe

    HI_U8 snid;                                 //同步对象的snid
    HI_U8 freq;                                 // 同步对象的频段
    HI_U16 tei;                                 //同步对象的tei;

    HI_U8 SetSyncObjectEn;              //是否有同步对象:0表示不设定同步对象，1:表示设定同步对象
    HI_U8 FreqErrorReportEn;             //表示为B4或者监听模式，0:表示B4模式，1:表示监听模式
    HI_U8 isHiGreenTreeV100;            //监听的是否为B4，或者Hi3911网络,0:表示为HI3911网络,1:表示B4
    HI_U8 NTBAlpha;                          // NTB进行alpha滤波系数
    HI_U32 min_beacon_period;          //最小信标间隔,单位:ms,默认为2000
    HI_U32 max_beacon_period;         //最大信标间隔,单位:ms默认为160000

    HI_U32 ntb_report_ppm_period;      //上报频偏ppm的周期，默认为20s，单位:ms
    HI_U32 ntb_report_ppm_switch;      // 上报频偏ppm的开关，1:开启，0:关闭，默认开启

    HI_U32 Rsd[4];                          //保留
}DIAG_CMD_PHY_STUB_NV_STRU_V200;

typedef struct
{
    HI_S8 freq_power[26] ;          //根据频段设置数字功率
    HI_U8 Rsd[2];
}DIAG_CMD_FREQ_POWER_NV_STRU_V200;

// V200 的台区识别NV配置参数
#define DIAG_CMD_TF_SOS_CNT               (20)
#define DIAG_CMD_TF_LINE                  (8)

typedef struct
{
    HI_U32 v100_old_en : 1;                   // 回退老版本v100使能，0:不生效，1:生效，为1时软件从电路或者芯片中获取一个中断
    HI_U32 lvai_en : 1;                       // 台区识别功能使能，0:不使能，1:使能，默认为1
    HI_U32 v100_en : 1;                       // v100兼容模式码字运算使能，0:不使能，1:使能，默认为1
    HI_U32 v200_en : 1;                       // v200码字运算使能，0:不使能，1:使能，默认为1
    HI_U32 pos_en : 1;                        // 正沿运算使能，0:不使能，1:使能，默认为1
    HI_U32 neg_en : 1;                        // 反沿运算使能，0:不使能，1:使能，默认为1
    HI_U32 iir_en : 1;                        // iir滤波器使能，0:不使能，1:使能，默认为1
    HI_U32 iir_stable_cnt : 9;                // Iir初始化后稳定的时长，默认200
    HI_U32 del_type_us : 1;                   // 微妙级异常删除控制，0:中断异常时，删除前面数据，1:中断异常时，删除后面数据，默认为0
    HI_U32 del_type_ms : 1;                   // 毫秒级异常删除控制，0:中断异常时，删除前面数据，1:中断异常时，删除后面数据，默认为1
    HI_U32 th_del_us : 10;                    // 微妙级门限，默认为30us,单位us
    HI_U32 power_period_stat_st : 1;          //  工频周期统计控制，从0变1时芯片动作，芯片完成一次统计后不再继续统计，如需再次统计，需软件先将其置0，然后再置1。
    HI_U32 noise_detect_en : 1;               // 噪声运算部分功能的使能信号，默认打开,0 关闭,1 打开
    HI_U32 suspend_poweredge_type : 2;        // 中断产生的强电边沿,Sta默认0,Cco默认1,0：双沿,1：升沿,2：降沿

    HI_U16 th_del_ms;                         // 毫秒级异常门限，默认1000us，单位us
    HI_U16 th_alarm;                          // 异常时间门限，默认15000us，单位us

    HI_U16 sth_v100;                          // v100的软值有效门限，默认72
    HI_U16 sth_v200;                          // v200的软值有效门限，默认73

    HI_U32 v100_seq_len : 9;                  // v100的序列的码字长度，默认13bit
    HI_U32 v100_seq_rep : 9;                  // v100的序列重复次数，默认为11，总的码字长度为 11*13 = 143bit
    HI_U32 v100_seq_num : 14;                 // v100的序列数据，150

    HI_U16 v200_seq_len;                      // v200的序列的码字长度，默认127bit
    HI_U16 v200_seq_num;                      // v200的序列数据个数，128

    HI_U32 gain;                              // iir的增益系数，默认为12366

    HI_U32 len;                               // 缓存深度，默认153

    HI_U8 th_l;                               // 低去噪门限，4us
    HI_U8 th_h;                               // 高去噪门限，15us
    HI_U16 th_coef;                           // 门限系数，341,相当于0.67

    HI_U16 suspend_source_switch : 1;         // 切换中断获取源，0:ad触发，此时Suspend_ad_en需要置为1, 1:外部电路触发，默认0
    HI_U16 ntb_source_switch : 1;             // 切换ntb获取源，0:全网，1:本地，默认0
    HI_U16 suspend_edge_type : 2;             // 中断边沿，0:上升沿，1:下降沿,2:两沿均有效，默认为0
    HI_U16 single_path_en : 1;                // 单路模式,0:无效，1:有效，此时suspend_ad_en配置为1，单路模式有效时，只对th_v_relative，th_v_absolute的第1个值
                                              // 进行配置，输出结果只看第1路输出的4组结果。
    HI_U16 suspend_ad_en  : 1;                // 芯片通过ad数据，使用某个门限电压提供1路过零中断，0:无效，1:有效，默认0，在   single_path_en为1时，配置为1
    HI_U16 vpp_learn_rst : 1;                 // 为1是，重新计算v_min,v_max,默认为0
    HI_U16 vpp_win_len : 6;                   // 参与评估的数据个数，默认为10
    HI_U16 power_period_close : 1;            // 用于停止工频周期统计，从0到1芯片检测上升沿停止工作并复位内部计数。软件每一次停止配置1再置0即可。
    HI_U16 t_in_f_lock : 1;                   // 是否在下降沿计算锁存8路的t_in,默认为1，启动下降沿锁存
    HI_U16 suspend_source_switch_ndm_tx : 1;  // 发射机的外部中断获取源，默认为外部电路触发,值为1
    //HI_U16 pad : 1;
    HI_U16 vpp_learn_duration;                // vpp评估的总时长，以样点数量表示，默认为2000

    HI_U32 edge_r_th_id : 4;                  // 检测上升沿使用的th_v序号，默认为5
    HI_U32 edge_f_th_id : 4;                  // 检测下降沿使用的th_v序号，默认2
    HI_U32 edge_step : 8;                     // 评估升降沿间隔，默认为50，物理意义为5毫秒
    HI_U32 th_v_type : 1;                     //    0:相对门限，1:绝对门限，默认为0
    HI_U32 pad4 : 8;                          // 保留位,默认为0
    HI_U32 collect_ntb_v_mode : 2;            // 收集NTB数据时的兼容模式，0:v100,v200都兼容，1:v100使能，2:v200使能
    HI_U32 om_ntb_collect_mode : 3;           // 采集NTB数据的模式，0:不采集，1:存储成功，存储异常，2:存储成功，3:存储异常
    HI_U32 lvai_cal_clk_sw_en : 2;            // 启动动态功耗，默认配置00，启动台区识别低功耗，00,01：硬件控制时钟使能，
                                              // 10：软件禁止时钟，硬件控制无效,11：软件使能时钟，硬件控制无效
    HI_U32 power_period_duration;             //   统计工频周期数量的时长，单位秒,默认:10
    HI_U32 th_t_calc_len : 8;                 // 去噪门限计算长度，默认为143
    HI_U32 pad2 : 24;
    HI_U32 noise_win_num : 24;                // 噪声统计窗的数量，以Noise_win_len为单位,在一般情况下，默认1800,在无发射机识别阶段，默认1,STA端按照每3min上报，NDM按照每1s上报
    HI_U32 noise_win_len : 8;                 // 噪声计算的窗长度，以工频周期为单位,默认50（1秒）

    HI_U32 all_phase_zerocross_en : 1;        // 三相同时锁存过零ntb使能信号，默认为00：不使能；1：使能；使能时，需将Suspend_poweredge_type置1（升沿），suspend_ad_en置1（ad中断使能）在使能结束时，需等3秒后台区识别结果才有效
    HI_U32 phase_one_order : 2;               // 三相切换第一次切换相位
    HI_U32 phase_two_order : 2;               // 三相切换第二次切换相位
    HI_U32 phase_three_order : 2;             // 三相切换第三次切换相位
    HI_U32 v_buf_step : 10;                   // 光耦学习电压采样时间间隔，单位微秒。默认：100。
    HI_U32 th_t_num_th : 5;                   // 计算门限时，大于该参数时，会处理内部最大的两个扰动值后做门限计算。默认值10。
    HI_U32 pad3 : 10;

    HI_U16 all_phase_dummy_th;                // Nv 有中断后，继续监听过零点一段时间，保证后续有中断依然能够上报默认之前1000，改为2500，单位微秒
    HI_U16 all_phase_overtime_th;             // 有中断后，继续继续监听过零点时间的上限，超时后强制切相；默认之前2000，改为3000，单位微秒

    HI_U32 ntb_offset_th;                     // NTB同步时，芯片内部NTB的最大同步门限，默认值为25000，对应1ms。

    HI_U16 th_v_relative[DIAG_CMD_TF_LINE];   // 相对门限默认值，93,124,186,341,682,837,899,930,如果单路模式有效，且相对门限有效，第1个值为
                                              // 为生效的比较点，对应比例:0.091,0.121,0.181,0.333,0.667,0.818,0.879,0.909
    HI_S16 th_v_absolute[DIAG_CMD_TF_LINE];   // 绝对门限，默认值，如果单路模式有效，且绝对门限有效，第1个值为生效的比较点，
}DIAG_CMD_CFG_TF_REG_PARAM_STRU_V200;

typedef struct
{
    HI_S16 sos[DIAG_CMD_TF_SOS_CNT];          //  默认参数为 16384;-32048; 16384; -21035; 7027; 16384; -28326; 15259; -29258; 13368
                                                                                                           //16384; -14345; 12004; -29834; 13867 ;16384; 10978; 5776; -29824; 13867
}DIAG_CMD_CFG_TF_REG_IIR_PARAM_STRU_V200;

#ifdef __cplusplus
}
#endif


#endif


