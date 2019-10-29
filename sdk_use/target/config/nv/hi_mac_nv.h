/*************************************************
  Copyright:   Huawei Technologies Co., Ltd. 1998-2011
  File name: mac_nv.h
  Author:  dkf35509
  Description:  NV相关ID和结构体定义
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

// 用于配置项中表明启用/禁用标志
#define  MAC_NV_CONFIG_ENABLE 1
#define  MAC_NV_CONFIG_DISABLE 0

/************************************************************************/
/*    NV 项 ID                                                          */
/************************************************************************/
typedef HI_U16 ID_DIAG_CONFIG;
#define ID_DIAG_CMD_CONFIG_IF                           (0x0202)    // 帧间隔参数
#define ID_DIAG_CMD_CONFIG_CLASSES                      (0x0203)    // 汇聚分类参数
#define ID_DIAG_CMD_CONFIG_TIMEOUT_ON_PRI               (0x0204)    // 报文过期时间参数
#define ID_DIAG_CMD_CONFIG_BUFFER_PERCENT_ON_PRI        (0x0205)    // 发送队列大小参数
#define ID_DIAG_CMD_CONFIG_LED_NV                       (0x0209)    // 灯控模块参数
#define ID_DIAG_CMD_NDM_PIPE_NV                         (0x020C)    // 抄控器通道参数
#define ID_DIAG_CMD_CONFIG_ASSOC_CONFIG                 (0x020D)    // 关联参数
//#define ID_DIAG_CMD_CONFIG_SOUND_STARTUP_THRESHOLD      (0x0213)    // SOUNDING启动参数
#define ID_DIAG_CMD_CONFIG_PHASE_ATTR                   (0x0214)    // 相位模块参数
#define ID_DIAG_CMD_CONFIG_TDMA_CONFIG                  (0x0217)    // TDMA调度参数
#define ID_DIAG_CMD_CONFIG_ROUTE_ATTRIB                 (0x0220)    // 路由评估参数
#define ID_DIAG_CMD_CONFIG_TX_CSMA_ALGORITHM            (0x0221)    // CSMA退避参数
#define ID_DIAG_CMD_CONFIG_TX_SOF_TMI_POLICY            (0x0222)    // 发送TMI选择策略参数
#define ID_DIAG_CMD_CONFIG_BEACON_TMI                   (0x0223)    // 信标帧TMI选择参数
#define ID_DIAG_CMD_CONFIG_DFX_CONFIG                   (0x0224)    // 可维可测开关参数
#define ID_DIAG_CMD_CONFIG_ROUTE                        (0x0225)    // IP路由配置参数
#define ID_DIAG_CMD_MULTI_NETWORK_ASSOC_CONFIG                       (0x0226)    // 多网络关联参数
#define ID_DIAG_CMD_BEACON_SLOT_CONFIG                  (0x0227)    // 信标时隙参数
#define ID_DIAG_CMD_FILTER_SWITCH_CONFIG                (0X0229)    // 报文过滤开关参数
#define ID_DIAG_CMD_CONFIG_STA_BLIND_CHECK              (0x022A)    // 频段盲检参数
#define ID_DIAG_CMD_CONFIG_NDM_CONNECT_PARA             (0x022D)    // 抄控器连接参数
#define ID_DIAG_CMD_CONFIG_MONITOR_CONFIG               (0x022E)    // 报文监控参数
#define ID_DIAG_CMD_SPEED_ABNORMITY_NV                  (0x022F)    // 速率异常检测参数
#define ID_DIAG_CMD_CONFIG_RT_CYCLE_SELF_ADAPT_PARA     (0x0230)    // 路由周期参数
#define ID_DIAG_CMD_STA_GPIO_NV                         (0x0231)    // STA的GPIO管脚配置参数以及关键节点定位参数
#define ID_DIAG_CMD_CONFIG_MAC_NDM_CHL                  (0x0232)    // 抄控器通道参数
#define ID_DIAG_CMD_CONFIG_MULTI_EXCEPTION_NV           (0x0233)    // 多网络异常检测参数
#define ID_DIAG_CMD_CFG_FA_NV                           (0x0234)    // 频段自适应参数
#define ID_DIAG_CMD_CONFIG_SPEED_CONFIG                 (0x0235)    // MAC速率参数
#define ID_DIAG_CMD_CONFIG_REPORT_COMM_RATE             (0x0236)    // 通信率上报参数
#define ID_DIAG_CMD_CONFIG_SPEED_TEST                   (0x0237)    // 性能测试配置参数
#define ID_DIAG_CMD_CONFIG_PLC_SWITCH_PARA_CONFIG       (0x0239)    // 交换模块参数(仅在远程载波通讯终端版本使用)
#define ID_DIAG_CMD_CFG_TF_NV                           (0x023A)    // 台区识别参数
#define ID_DIAG_CMD_CONFIG_ZERO_CROSS_DIFFERENCE_NV     (0x023B)    // 过零点检测参数
#define ID_DIAG_CMD_CFG_PI_NV                           (0x023C)    // 相位识别参数
#define ID_DIAG_CMD_CONFIG_PLC_SWITCH_FILTER_CONFIG     (0x0240)    // 交换模块报文过滤参数
#define ID_DIAG_CMD_CONFIG_FAULT_LOCAT_PARA_CONFIG      (0x0241)    // 关键节点定位参数
//#define ID_DIAG_CMD_CONFIG_DUAL_MODULE_PARA_CONFIG      (0x0242)    // 双模策略参数
#define ID_DIAG_CMD_CONFIG_DUTY_PARA_CONFIG             (0x0243)    // 占空比控制参数(测试使用)
#define ID_DIAG_CMD_CONFIG_TF_CROSSTALK_STA_PARA        (0x0244)    // 台区识别串扰策略参数(STA)
#define ID_DIAG_CMD_CONFIG_SNR_CONFIG                   (0x0245)    // 关联参数扩展NV项(作为0x20D的补充)
#define ID_DIAG_CMD_CONFIG_SNR_TF_CONFIG                (0x0246)    // SNR台区识别参数

#define ID_DIAG_CMD_CONFIG_PTPC_PARA                    (0x024A)    // UIU的参数
#define ID_DIAG_CMD_CONFIG_EMC_PARA                     (0x024B)    // EMC认证参数
#define ID_DIAG_CMD_NTB_PARA                            (0x024D)    // V200-NTB参数
//#define ID_DIAG_CMD_GT_TEST_TMI_INFO_PARAM              (0x024F)
//#define ID_DIAG_CMD_GT_TEST_NTB_PARAM                   (0x0250)
//#define ID_DIAG_CMD_SC_NETWORK_PARA_CONFIG              (0x0251)    // 单载波组网参数
#define ID_DIAG_CMD_TF_PASSIVE_NV                       (0x0252)    // 无扰识别控制参数
//#define ID_DIAG_CMD_MULTI_FREQ_PARAM_CONFIG             (0x0253)    // 多频组网参数
#define ID_DIAG_CMD_POWER_FAILURE_STATISTIC_INFO        (0x0254)     //停电事件相关信息存储info
#define ID_DIAG_CMD_CONFIG_COORDINATE_CONFIG            (0x0305)    // 多网络协调参数
#define ID_DIAG_CMD_CONFIG_POWEROFF_CHECK_CONFIG        (0x0306)    // 停电检测参数
#define HI_NV_FTM_ROUTE_PROXY_CONFIG                    (0x0310)    // HI_NV_FTM_RESERVED0_S
/************************************************************************/
/*             NV设置类                                                   */
/************************************************************************/

#define FA_TIMETABLE_NUM_MAX            9                   //自检定时时刻的最大个数

#define SNR_TRANS_CLASS_NUM                     (8)         // 传输SNR层级数目
#define SNR_COLLECT_CLASS_NUM                   (16)         // 统计SNR层级数目

#define SNR_FAST_ACCURATE_TH_NUM                (4)         // 快速收敛高精门限个数
#define SNR_FAST_ACCURATE_AVG_PECISION          (10)        // 快速收敛高精门限的SNR平均值精度

//*****************************************************************************
// 预留NV项定义, 最大 64字节
//*****************************************************************************
typedef struct
{
    HI_U8      static_proxy_mac[HI_PLC_MAC_ADDR_LEN];   // 指定静态中继MAC地址
    HI_BOOL    my_static_proxy_exist;                   // 是否设置指定静态中继
    HI_BOOL    to_be_static_repeater;                   // 本站点是否为静态中继

    HI_U8      freq_evaluate_mode;                      //频段评估模式。0：自检模式；1：完成模式  1   工厂区
    HI_U8      rsvd2;                                   //保留
    HI_U16     max_node_num;                            //历史评估最大站点数。完整评估后刷新，自检满足标准且个数增大时刷新    0   工厂区
}HI_NV_FTM_ROUTE_PROXY_CONFIG_S;

//定时参数定义
typedef struct
{
    HI_BOOL is_enable;          //定时有效的标识
    HI_U8  hour;                // hours
    HI_U8  min;                 // minutes
    HI_U8  sec;                 // seconds
}MAC_REALTIME_STRU;

/**
 * 1.2    帧间隔配置
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

   HI_U16 ack_min_redundancy;                  //MAC的应答帧的最小随机间隔，单位:us
   HI_U16 ack_max_redundancy;                  //MAC的应答帧的最大随机间隔，单位:us
} DIAG_CMD_CONFIG_IF_STRU;

/**
 *
 * 1.3    分类规则及与优先级对应关系配置
 *
 */
typedef enum _m_type_e_
{
    DFX_MANAGEMENT_MESSAGE_TYPE     =0x88E1,    // 本地管理数据类型
    DFX_IP4_MESSAGE_TYPE         = 0x88E3,  // IP数据类型
}m_type_e;
typedef  struct _DIAG_CMD_CONFIG_CLASSES_ENTRY_STRU_
{
    HI_U16 type;        // 报文类型.88e1:本地 .0x88E3, IP数据类型
    HI_U8  flag ;       // (vlantag、TOS、 MMTYPE)
    HI_U8  priorty;     // 报文优先级
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
 * 1.4    不同优先级报文的超时时间设置
 *
 */
typedef struct
{
    HI_U32 timeout : 24;  // 对应的超时
    HI_U32 priorty : 8;   // 优先级
} DIAG_CMD_CONFIG_TIMEOUT_ON_PRI_ENTRY_STRU;
#define ID_DIAG_CMD_CONFIG_TIMEOUT_ON_PRI_ENTRY_COUNT 10
typedef struct _DIAG_CMD_CONFIG_TIMEOUT_ON_PRI_STRU_
{
    HI_U8 num;   // 设置项条数n
    HI_U8 pad[3];
    DIAG_CMD_CONFIG_TIMEOUT_ON_PRI_ENTRY_STRU entry[ID_DIAG_CMD_CONFIG_TIMEOUT_ON_PRI_ENTRY_COUNT];    // 设置项实体
} DIAG_CMD_CONFIG_TIMEOUT_ON_PRI_STRU;


/**
 *
 * 1.5    不同优先级的BUFFER 比例
 *
 */
typedef struct
{
    HI_U8  priorty;             // 队列优先级
    HI_U8  cco_queue_size;      // CCO队列报文个数限制
    HI_U8  sta_queue_size;      // STA队列报文个数限制
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
 * 1.6    发送/接收 buffer大小
 *
 */
typedef struct _DIAG_CMD_CONFIG_TXRX_BUFFER_SIZE_STRU_
{
   HI_U32 tx_buffer_size;
   HI_U32 rx_buffer_size;
} DIAG_CMD_CONFIG_TXRX_BUFFER_SIZE_STRU;

/**
 *
 * 1.7    报文重传次数
 *
 */
typedef struct _DIAG_CMD_CONFIG_RESEND_COUNT_STRU_
{
   HI_U32 resend_time;
} DIAG_CMD_CONFIG_RESEND_COUNT_STRU;

/**
 *
 * 1.8    白名单MAC地址列表设置(CCO, 关联请求用)
 *
 */
#ifdef SAL_HAVE_FEATURE_UT
#define ID_DIAG_CMD_MAC_WHITE_ITEM_MAX 150
#else
#define ID_DIAG_CMD_MAC_WHITE_ITEM_MAX 2048         //HSO使用该值，与HI_MAC_VALID_MAC_LIST_MAX和HI_BIDB_ITEM_NUM_MAX关联
#endif

#define RT_NV_SLIDE_WINDOWS_CNT              6
typedef struct _DIAG_CMD_CONFIG_ROUTE_ATTRIB_STRU_
{
    HI_U16 rt_evaluate_cycle_time_len;             // 当前已不用，改用自适应评估周期的参数
    HI_U8  rt_rcv_proxy_down_min_th;               // 下行接收代理站点最小个数
    HI_U8  rt_down_comm_rate_basic_limit;          // 下行通信率基本门限
    HI_U8  rt_up_comm_rate_basic_limit;            // 上行通信率基本门限
    HI_U8  rt_choose_proxy_upper_limit;            // 代理选择通信率上限
    HI_U8  rt_choose_proxy_lower_limit;            // 代理选择通信率下限

    HI_U8  rt_continue_better_th;                  // 代理选择连续优于变更门限
    HI_U8  rt_high_diff;                           // 上限变更门限
    HI_U8  rt_middle_diff;                         // 上限与下限之间变更门限
    HI_U8  rt_low_diff;                            // 下限变更门限

    HI_U8  rt_switch_group_one;                    // bit0-bit7开关:代理收敛,代理优选,snr校准,代理层级远近识别算法,是否将信标帧作为通信率统计的依据,层级远近识别算法,可选择更低层级发现站点作为候选代理,信道容量作为代理评估入门条件
    HI_U8  rt_switch_group_two;                    // bit0-bit7开关:不处理上行通信率限制,通信率的低的站点不选择为候选代理,代理通信率高于上限时可选择更高通信率站点作为代理,发现列表中是否携带可维可测信息,路由评估窗口间快速路由评估,评估多路径,下行发布多路径,使用多路径
    HI_U8  rt_snr_diff;                            // 多路径选择时snr下限变更门限
    HI_U8  rt_comm_rate_diff;                      // 多路径选择时通信率下限变更门限
    HI_U8  rt_high_level_chg_comm_rate_diff;
    HI_U8  rt_same_level_chg_comm_rate_diff;
    HI_U8  rt_low_level_chg_down_up_comm_rate_diff;
    HI_U8  rt_low_level_chg_up_comm_rate_limit;
    HI_U8  pad;

    HI_U16 rt_child_station_less_th;               // 子站点偏少门限
    HI_U8  rt_proxy_send_discovery_pk_len;         // 代理发送发现列表周期时长         单位:S
    HI_U8  rt_sta_send_discovery_pk_len;           // 普通站点发送发现列表周期时长     单位:S
    HI_U8  slide_window_max;                       // 最大窗口数
    HI_U8  slide_window_weight[RT_NV_SLIDE_WINDOWS_CNT];       // 窗口权重
    HI_U8  min_first_evaluate_period;

    HI_U8  rt_min_comm_rate_th;                    // 将当前窗口通信率作为评估采用的通信率的门限
    HI_U8  enforcing_proxy_change_enable;          // 强制代理变更使能标志
    HI_U16 enforcing_proxy_change_cycle;           // 强制代理变更周期，单位分钟

    HI_U16 preamble_sat_limit;
    HI_U8  current_attenuation_weight;             // 在衰减滤波算法中，当前帧衰减值的权重
    HI_U8  current_mmib_weight;                    // 在MMIB滤波计算中，当前帧mmib数据的权重

    HI_U8  coefficient_high_limt;                  // 通信率系数高门限
    HI_U8  coefficient_middle_limt;                // 通信率系数中门限
    HI_U8  coefficient_low_limt;                   // 通信率系数低门限
    HI_U8  comm_rate_fliter_th;

    HI_U8  algorithm_type;                         // 路由算法的类型定义.例如RT_ALGORITHM_V1R3
    HI_U8  sta_coefficient;                        // 发现站点计算通信率时乘的系数
    HI_BOOL is_repair_enable;                      // 路由修复使能标识
    HI_U8 route_request_timer_period;              // 路由修复请求超时门限，单位s
}DIAG_CMD_CONFIG_ROUTE_ATTRIB_STRU;

#define RT_CYCLE_PARA_CNT           (6)
typedef struct _DIAG_CMD_CONFIG_RT_CYCLE_SEIF_ADAPTIVE_ATTRIB_STR_
{
    HI_U16 station_cnt;                     // 站点个数
    HI_U16 evaluate_cycle_len;              // 评估周期长度 单位:秒

    HI_U8  proxy_send_dk_cycle_len;         // 代理站点发送发现列表报文频率 单位:秒
    HI_U8  sta_send_dk_cycle_len;           // 发现站点发送发现列表报文频率 单位:秒
    HI_U8  pad[2];
}DIAG_CMD_CONFIG_RT_CYCLE_SEIF_ADAPTIVE_ATTRIB_STR;
typedef struct _DIAG_CMD_CONFIG_ROUTE_ADAPTIVE_ATTRIB_STRU_
{
    HI_U16 network_station_num;             // 网络规模数，0:表示根据网络实际站点数动态选取参数
    HI_U16 adaptive_redundance_para;        // 周期自适应参数冗余量，防止周期参数动荡
    DIAG_CMD_CONFIG_RT_CYCLE_SEIF_ADAPTIVE_ATTRIB_STR cycle_self_adaptive[RT_CYCLE_PARA_CNT];
}DIAG_CMD_CONFIG_ROUTE_ADAPTIVE_ATTRIB_STRU;

//LED配置的NV项
typedef struct
{
    HI_U8 tx_rx_led_mode;  // 收发指示灯模式，0:收发指示灯不闪烁，1:MAC层报文收发,2:APP报文收发,3:IP报文收发,4:APP,IP报文允许收发
    HI_BOOL phase_tx_led_switch; // 发送相位指示灯开关,HI_TRUE:开启,HI_FALSE:关闭
    HI_U8 led_state_switch;    // 异常状态指示灯开启观看,1表示不启用，0表示启用
    HI_U8 pad;

    HI_U32 pad1;
}DIAG_CMD_CONFIG_LED_NV_STRU;

//抄控器通道参数配置
typedef struct
{
    HI_U16 fragment_sleep_time;             //分包数据获取MacBuffer失败时等待的时间
    HI_U8  fragment_resend_cnt;             //分包数据获取MacBuffer失败时最大重传次数
    HI_U8  fc_pkt_num_max_per_req;          //流控通道中，每次请求最大回复的报文数

    HI_S16 proxy_snr_threshold;             //抄控器使用的代理站点的平均信噪比门限
    HI_U16 fc_rsp_wait_timer_period;        //应答超时的最大等待时间

    HI_U32 fc_req_wait_timer_period;        //一次请求所允许的最大处理时间

    HI_U8  fc_send_times_per_req;           //每个请求报文发送的次数
    HI_U8  fc_send_times_per_rsp;           //每个应答报文发送的次数
    HI_U8  ndm_remote_send_switch;          //抄控器端的非IP通使能开关，1:打开,0:关闭
    HI_BOOL is_fill_mac_addr_to_chip_id;
}DIAG_CMD_CONFIG_NDM_PIPE_NV_STRU;

//Plc ip的NV设置
typedef struct
{
    HI_U16 mtu_size;
    HI_U16 pad;
}DIAG_CMD_CONFIG_PLC_IP_NV_STRU;
/**
 *
 * 1.21    静态路由设置
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
 * 1.23    工作模式设置( 配置CCO，由CCO通知目的站点更新模式 )
 *
 */
typedef struct _DIAG_CMD_CONFIG_ENABLE_WORK_MODE_STRU_
{
   HI_U8 mode;  //提供对应的枚举类型
   HI_U8 pad[3];
}DIAG_CMD_CONFIG_ENABLE_WORK_MODE_STRU;

/**
 *
 * 1.24 TONE MAP 最大规格(其实他就是TONEMAP表缓存)
 *
 */
typedef struct _DIAG_CMD_CONFIG_TONE_MAP_CAPACITY_STRU_
{
    HI_U32 capacity;
} DIAG_CMD_CONFIG_TONE_MAP_CAPACITY_STRU;

/**
 *
 * 1.27    信道估计启动阀值
 *
 */
//sounding的启动条件
typedef struct _DIAG_CMD_CONFIG_SOUND_STARTUP_THRESHOLD_STRU_
{
    HI_BOOL sounding_enable;
    HI_U8 pad;
    HI_U16 non_robo_fail_threshold;       //非Robo失败的门限，超过此值，则重新启动训练

    HI_U8  absolute_thresh[4];          //绝对门限
    HI_U8  relative_thresh[4];          //相对门限

    HI_U8  pad1[4];
}DIAG_CMD_CONFIG_SOUND_STARTUP_THRESHOLD_STRU;

/**
 *
 * 1.28    网络属性
 *
 */
typedef struct
{
    HI_U16 evaluate_phase_bpc_or_cnt_th;    // 评估周期：BPC
    HI_U16 boot_evaluate_fail_th;           // 由于未接收三个相位的信标帧评估失败的最大次数

    HI_U16 evaluate_cmm_diff;               // 通过接收率比较误差值
    HI_U16 evaluate_channel_diff;           // 通过信道容量比较误差值

    HI_U8  evaluate_mode;                   // 评估模式:1.通信率优先 2.信道容量优先
    HI_U8  evaluate_cycle_mode;             // 评估周期模式:1.BPC为周期，2.接收个数为周期
    HI_U8  channel_mode;                    // 信道容量值统计模式:1.一个统计周期信道容量平均值，2.信道容量滤波值
    HI_U8  stat_data_mode;                  // 统计数据模式:1.周期性统计数据模式 2.历史统计数据模式
} DIAG_CMD_CONFIG_PHASE_EVALUATE_ATTR_STRU;

typedef struct
{
    HI_U8  cco_phase_cnt;                   // CCO的相位个数(最大支持3个相位)
    HI_U8  default_phase;                   // 默认相位(CCO单相位模式下使用)
    HI_U8  rx_phase_mode;                   // 相位接收模式:0.三相位单独接收；1.三相位同时接收
    HI_U8  resend_cnt_mode;                 // 0:每条路径上发送resend_cnt次数,
                                            // 1:所有路径上发送总次数为resend_cnt次，每条路径发送次数相同

    HI_U8  max_resend_cnt;                  // 最大重传次数
    HI_U8  fixed_resend_cnt;                // 固定重传次数
    HI_U8  business_parallel_max_cnt;       //缺省值为0，当该值不为零时，本接口返回max_business_parallel_cnt的值,只针对APP业务报文有效
    HI_U8  business_broadcast_wait_sack;    //缺省值为0，表示不等待sack，设置为1时，表示等待sack,只针对APP业务报文有效

    DIAG_CMD_CONFIG_PHASE_EVALUATE_ATTR_STRU phs_evaluate_para;
}DIAG_CMD_CONFIG_PHASE_ATTR_STRU;

/*
1.31 TDMA配
*/
typedef struct _DIAG_CMD_CONFIG_TDMA_CONFIG_STRU_
{
    HI_U32  max_beacon_period;                              // 最大信标周期长度
    HI_U32  min_beacon_period ;                             // 最小信标周期长度

    HI_U16  percent_beacon_period;                          // CSMA与信标时隙比例
    HI_U16  alloc_period_athead_time;                       // 提前创建信标帧的时间

    HI_U8   one_bea_duration;                               // 信标时隙长度
    HI_U8   central_beacon_num;                             // 中央信标帧个数
    HI_BOOL no_report_backup_proxy_chance;                  // 所有候选代理增加安排信标时隙的次数
    HI_U8   assoc_beacon_num;                               // 密集关联阶段安排信标时隙轮数

    HI_U8   comm_rate;                                      // 关联阶段通信率预值
    HI_U8   min_discover_beacon_sum;                        // 最小发现信标时隙个数
    HI_U8   net_forming_discover_beacon_state_count;        // 密集发送信标帧轮数
    HI_U8   net_forming_no_report_sta_chance_count;         // 密集关联阶段安排信标时隙轮数

    HI_U16  min_phase_duration;                             // 每个相位CSMA最小长度
    HI_BOOL to_be_upgrade_cco;                              // 是否开启一级升级模式
    HI_U8   alloc_dis_bea_cnt_for_rt_one_sta;               // 快速优化过程中信标时隙轮数

    HI_U16  min_network_packet_cp;                          // 绑定时隙存在时，网络管理时隙最小值
    HI_U16  tdma_period_msdu_send_min;                      // TDMA时隙内发送报文个数(TDMA时隙未使用)

    HI_U16  mac_receive_msdu_max_interval;                  // TDMA时隙相关参数(TDMA时隙未使用)
    HI_U8   tdma_period_valid_check_policy;                 // TDMA时隙有效性检查策略(TDMA时隙未使用)

    HI_BOOL equip_mode_flag;                                // 是否为工装模式
    HI_U16  max_proxy_count;                                // 最大代理个数
    HI_BOOL tdma_alloc_dfx_switch;                          // TDMA时隙分配可维可测开关
    HI_U8   tdma_join_count_per_periods;                    // 密集关联阶段安排每轮可允许加入站点的个数

    HI_U8   tdma_pk_duration;                               // 发送一个报文时长
    HI_U8   performance_test_switch;                        // 是否启动测试模式下TDMA调度(测试模式下不进行时隙分离，时隙分离未使用)
    HI_U16  min_csma_duration;                              // 最小CSMA时隙
} DIAG_CMD_CONFIG_TDMA_CONFIG_STRU;

typedef struct _DIAG_CMD_CONFIG_COORDINATE_CONFIG_STRU_
{
    HI_U16 max_backup_sys_time;                 // 允许最大退避时长
    HI_U16 max_same_snid_err_time;              // 允许网络号相同的最大时长

    HI_U16 send_pk_len_for_notify;              // 在非本CCO 的beacon周期内发送协调报文的周期
    HI_U16 send_pk_len_for_coordinate;          // 在本CCO的beacon周期内发送协调报文的周期

    HI_U16 check_cannot_comm_limit;             // 判断不通的门限，单位为系统时间，多长时间内没收到为不通
    HI_U16 listen_time_before_first_bea;        // 上电后监听邻居网络带宽信息的时长

    HI_BOOL open_coordinate_network;            // 多网络协调开关
    HI_U8  network_evaluation_enable;           // 是否启用优选SNID功能，0不启用
    HI_U8  continue_try_assoc_th;               // 连续尝试关联轮次(针对上下行不对称场景，单个网络尝试多少轮后，延迟关联时间到最大值)
    HI_U8  max_assoc_time;                      // 最大关联时长(针对上下行不对称场景，单个网络最大关联时长)

    HI_BOOL cco_snid_coordinate_switch;         // CCO端网络号协调开关
    HI_BOOL sta_snid_coordinate_switch;         // STA端网络号协调开关
    HI_BOOL conflict_detect_switch;             // 冲突检测开关
    HI_BOOL is_coordinate_with_pv100_in_sg;     //在国网协议下是否与V100协议协调

    HI_U16  coordinate_backoff_time;            // 多网络冲突退避时长
    HI_U16  change_sind_notify_time;            // 网络号变更通知时长

    HI_U16 exception_by_bpc_th;                 // BPC异常门限
    HI_U16 exception_by_cco_mac_th;             // CCO MAC地址异常门限
    HI_U16 report_interval;                     // 上报间隔
    HI_U16 exception_interval;                  // 异常间隔
    HI_U16 max_send_query_neighbor_info_num;
    HI_BOOL protocol_type;
    HI_U8  pad;
}DIAG_CMD_CONFIG_COORDINATE_CONFIG_STRU;

/*
2.34  发送机CSMA退避算法
*/
#define TX_CSMA_BACKOFF_BC_TABLE_ITEMS 20
#define TX_CSMA_BACKOFF_BC_DEFAULT_SLOT_MULTI 5
#define TX_CSMA_BACKOFF_BC_CCO_SLOT_MULTI 2
typedef struct _DIAG_CMD_CONFIG_CMSA_BACKOFF_POL_
{
    HI_U8 default_node_count;               // 默认 的周围站点个数 == 10
    HI_U8 default_slot;                     // ==100
                                            // (*TX_CSMA_BACKOFF_BC_DEFAULT_SLOT_MULTI)，
                                            // 这里设置的值乘以TX_CSMA_BACKOFF_BC_DEFAULT_SLOT_MULTI.
    HI_U8 fixed_bc;                         // 用于最高优先级 固定的退避数 =4
    HI_U8 min_node_count;                   // 小范围冲突域的最多站点数 =2
    HI_U8 sm_scope_slot;                    // 用于周围 在小范围内 站点时的slot值. =120
    HI_U8 sm_scope_bc;                      // 用于周围只有在小范围内  时的 退避 值. =7
    HI_U8 bg_scope_slot;                    // 用于周围超过 TX_CSMA_BACKOFF_BC_TABLE_ITEMS
                                            // 个时的slot . = 100  * TX_CSMA_BACKOFF_BC_DEFAULT_SLOT_MULTI
    HI_U8 bg_scope_bc;                      // 用于周围超过 TX_CSMA_BACKOFF_BC_TABLE_ITEMS 个时的BC,
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
    HI_U8  ver_sw;              // 1, 老算法，2, 新算法, 3 算法3

    HI_U8  hi_range[16];        // 新算法使用的退避随机窗口值
    HI_U8  low_range[16];

    DIAG_CMD_CONFIG_CMSA_BACKOFF_ALGORITHM3_STRU alg3;
}DIAG_CMD_CONFIG_TX_CSMA_ALGORITHM_STRU;

typedef struct _DIAG_CMD_CONFIG_ASSOC_CONFIG_
{
    HI_U8       mac_auth_enable;                        // 启动MAC认证
    HI_U8       nmk_auth_enable;                        // 启动NMK认证
    HI_U8       req_send_bpc_interval;                  // 发送关联请求间隔的BPC
    HI_U8       ind_send_max_count;                     // 发送ind报文最大次数

    HI_U16      ind_send_time_interval;                 // 发送ind报文间隔时间
    HI_U16      max_child_sum;                          // 最大子站点个数

    HI_U8       is_carry_dfx_switch;                    // 网络关联报文是否携带可维可测信息开关
    HI_U8       assoc_gather_switch;                    // 关联指示汇总开关
    HI_U16      min_change_proxy_req_resend_time;       // 等待代理变更回复最小时间

    HI_U8       assoc_req_try_max;                      // 关联请求最大请求次数
    HI_U8       wait_gather_ind_time;                   // 等待生成汇总报文时间,单位s
    HI_U8       change_proxy_req_try_max;               // 代理变更最大请求次数
    HI_U8       ind_resend_max_count;                   // ind报文重传次数

    HI_U8       disable_optimal_network_comm_rate;      // 关闭多网络优选功能的通信率门限
    HI_U8       network_evaluation_time;                // 评估网络时间，单位s
    HI_BOOL     is_chip_ver_pkt_bit_enable;             // 通过V100协议中关联请求报文外加的比特判断芯片的功能是否使能
    HI_U8       ptcl_detect_no_sta_time_len;            // 无站点加入即启动协议探测的门限，单位min

    HI_U32      wait_for_mac_calculation_timeout;       // 计算表地址时间
    HI_U16      detect_period_interval;                 // CCO启动协议探测的周期,单位:min
    HI_BOOL     is_ptcl_detect_enable;                  // 协议探测是否使能
    HI_U8       detect_time_len;                        // 协议探测报文发送的持续时间，单位:min

    HI_U8       wait_assoc_ind_timeout;                 // 等待关联指示报文时间，单位s
    HI_U8       max_assoc_req;                          // 最大发起请求次数
    HI_BOOL     random_assoc_enable;                    // 随机关联功能开关
    HI_U8       slot_dfx_switch;

    HI_U16      assoc_not_in_whiter;                    // 未在白名单关联失败后，再次发起关联请求时长
    HI_U16      assoc_not_set_whiter;                   // 网络未设置白名单关联失败后，再次发起关联请求时长

    HI_U16      assoc_too_much_sta;                     // 网络站点过多关联失败后，再次发起关联请求时长
    HI_U16      assoc_too_much_pco;                     // 代理站点过多关联失败后，再次发起关联请求时长

    HI_U16      assoc_too_much_child;                   // 子站点过多关联失败后，再次发起关联请求时长
    HI_U16      assoc_too_much_level;                   // 层级超过最大限制关联失败后，再次发起关联请求时长

    HI_U16      assoc_repeat_mac;                       // 重复MAC地址关联失败后，再次发起关联请求时长
    HI_U16      ptcl_recover_time_len;                  // 低版本协议站点离线后恢复高版本协议的时间，单位：分钟

    HI_U16      detect_pkt_send_interval;               // 协议探测报文的发送周期(即报文发送间隔)，单位:s
    HI_BOOL     end_sequence_switch;                    // 端到端序号检测开关
    HI_BOOL     path_sequence_switch;                   // 路径序号检测开关

    HI_U16      white_leave_sta_th;                     // 允许白名单踢一次最多踢出站点个数门限
    HI_U16      white_table_notify_num;                 // 白名单踢出站点离线通知次数
    HI_U16      cco_send_leave_pk_time;                 // CCO发送延迟离线指示报文间隔
    HI_U16      delay_leave_time_len;                   // STA延迟重启时间

    HI_U8       heartbeat_leave_cycle_th;               // 心跳检测中站点离线周期门限
    HI_U8       heartbeat_report_cycle_th;              // 心跳检测中站点上报周期门限
    HI_U16      leave_info_storage_cycle;               // 离线信息存储周期，单位:分钟

    HI_U8       leave_info_storage_switch;              // 离线信息存储开关
    HI_U8       leave_info_storage_th;                  // 一个周期存储上限
    HI_U8       route_cylce_per_heartbeat_cycle;        // 心跳周期包含路由周期个数
    HI_U8       heartbeat_pk_cnt_per_heartbeat_cycle;   // 每个心跳周期发送几个心跳报文
}DIAG_CMD_CONFIG_ASSOC_CONFIG_STRU;

typedef struct _DIAG_CMD_CONFIG_SNR_CONFIG_STRU_
{
    HI_U8   ntb_crt_enable          : 1;        // NTB修正策略是否使能
    HI_U8   is_first_use_ntb_result : 1;        // 是否优先使用NTB结果
    HI_U8   pad0                    : 6;
    HI_U8   snr_history_num;                    // 长期记录个数
    HI_U8   snr_collect_period;                 // 数据采集周期，单位min
    HI_U8   save_snr_times_th;                  // 白名单常开模式下，历史记录个数达到门限就用长期收敛

    HI_U8   belong_stable_times_th;             // 归属信息稳定次数门限
    HI_U8   snr_class_stable_times_th;          // SNR层级稳定次数门限
    HI_U16  snr_collect_duration_th;            // 统计时长门限，未达到门限周期数据不存储到历史记录，单位s

    HI_U16  snr_unit_duration;                  // 单位时间长度，单位s
    HI_S8   snr_avg_th;                         // SNR平均值门限
    HI_U8   snr_unit_fc_cnt_abs;                // 每单位时间接收FC个数门限

    HI_U8   his_valid_duration_th;              // 历史记录在该时限内，才可用于更新SNR层级，单位min
    HI_BOOL fast_convergence_enable;            // 网络快速收敛使能
    HI_U8   lock_belong_net_duration;           // 白名单常开模式下，被踢后，锁定归属网络多久，单位min
    HI_U8   fast_lock_duration;                 // 网络快速收敛: 每次切换网络后的锁定时间，单位min

    HI_U8   stable_change_enable;               // 长期评估切换网络使能
    HI_U8   ntb_crt_att_redundance_th;          // NTB修正策略的衰减冗余门限
    HI_U8   stable_check_first_time;            // 保存多少次后开始第一次检查
    HI_U8   stable_check_interval;              // 每隔几次检查一次

    HI_U8   stable_greater_rate_th;             // A,B两个网络比较SNR,A网络比B网络要好的次数门限,默认80%
    HI_U8   stable_gt_or_eq_rate_th;            // A,B两个比较SNR,A网络大于等于B网络的次数门限，默认90%
    HI_U16  stable_lock_duration;               // 普通锁定时长，单位min

    HI_U16  pream_sta_num_th;                   // preamble饱和符号数门限，超过此门限的snr直接丢弃,默认:80
    HI_U8   alpha_numerator;                    // 平均snr的滤波器分子,默认:1
    HI_U8   alpha_denominator;                  // 平均snr的滤波器分母,默认:16

    HI_U8   ntb_crt_attenuation_th;             // NTB修正策略的衰减值门限
    HI_U8   stable_snr_redundance_th;           // 长期收敛snr相对和平均值的比较冗余门限，缺省值10，单位0.1DB
    HI_U16  ntb_crt_lock_duration;              // NTB修正策略锁定时长，单位min

    HI_U8   fast_accurate_rounds[SNR_FAST_ACCURATE_TH_NUM]; // 周期轮数门限
    HI_U8   fast_accurate_avgs[SNR_FAST_ACCURATE_TH_NUM];   // SNR平均值门限，单位0.1DB

    HI_U16  attenuation_th;                     // 衰减门限，信道衰减低于此门限时，不判断preamble饱和门限，默认:10
    HI_U8   is_snr_class;                       // 是否使用SNR层级
    HI_U8   belong_rcv_time_th;                 // 只要最近一段时间内收到过归属网络，识别结果就为归属网络，单位min
} DIAG_CMD_CONFIG_SNR_CONFIG_STRU;

#define NM_SNR_TF_ADAPT_PARA_NUM    (5)

typedef struct
{
    HI_U16 station_cnt;         // 台区规模
    HI_U8  topo_stable_time;    // 拓扑稳定时长，单位min
    HI_U8  pad;
} DIAG_CMD_CONFIG_SNR_TF_ADAPT_PARA_ST;

typedef struct
{
    HI_U16  snr_tf_time_max;    // SNR台区识别使用最大时长，单位min
    HI_U8   is_tf_complete;     // SNR台区识别是否完成
    HI_U8   is_snr_class;       // 是否使用SNR层级

    DIAG_CMD_CONFIG_SNR_TF_ADAPT_PARA_ST adapt_para[NM_SNR_TF_ADAPT_PARA_NUM];  // SNR台区识别自适应参数

    HI_U16 snr_tf_time_exceed;  // 超出最大规模时使用时长，单位min
    HI_U16 pad1;

    HI_U32  pad2[9];
} DIAG_CMD_CONFIG_SNR_TF_CONFIG_STRU;

typedef struct _DIAG_CMD_CONFIG_TX_SOF_TMI_POLICY_
{
    HI_U32   default_72          :4;
    HI_U32   default_136        :4;                //PB136重传更换TMI关闭时使用的TMI
    HI_U32   default_264        :4;
    HI_U32   default_520        :4;                //PB136重传更换TMI关闭时使用的TMI

    HI_U32   resend_72_enable           :1;          //PB136重传更换TMI使能开关
    HI_U32   resend_136_enable         :1;          //PB136重传更换TMI使能开关
    HI_U32   resend_264_enable         :1;          //PB136重传更换TMI使能开关
    HI_U32   resend_520_enable         :1;          //PB520重传更换TMI使能开关
    HI_U32   pad                                   :4;

    HI_U32   tmi_72_cnt                       :4;
    HI_U32   tmi_264_cnt                     :4;

    HI_U8   tmi_136_cnt;
    HI_U8   tmi_520_cnt;
    HI_U8   resend_72[4];
    HI_U8   resend_136[6];

    HI_U8   resend_264[4];
    HI_U8   resend_520[6];
    HI_U8   tmi_auto_adjust_enable;     //sof的重传tmi自动调整
    HI_U8   degrade_resend_cnt;

    HI_U32  recover_tmi_period;             //恢复默认tmi的定时器周期

    HI_U32   broadcast_72                  :4;
    HI_U32   broadcast_136                :4;
    HI_U32   broadcast_264                :4;
    HI_U32   broadcast_520                :4;
    HI_U32   ext_tmi_72                    :4;                //PB72的扩展TMI
    HI_U32   ext_tmi_136                  :4;                //PB136的扩展TMI
    HI_U32   ext_tmi_264                  :4;                //PB264的扩展TMI
    HI_U32   ext_tmi_520                  :4;                //PB520的扩展TMI
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
    HI_U8 mac_speed_report_sw; // 0: 关闭记录信息， 非0打开
    HI_U8 mac_speed_report_peroid;// MAC速率上报周期
    HI_U8 mac_speed_exception_notify_sw;// MAC层速率低于设置门限告警开关
    HI_U8 pad;
    HI_U32 mac_rx_speed_therold; // 最低速率门限值
    HI_U32 mac_tx_speed_therold; // 最低速率门限值

}DIAG_CMD_CONFIG_MAC_SPEED_ENTRY;

//速率异常测试的NV项
typedef struct
{
    HI_U8  detect_timer_switch;             //功能开关:0,关；1,开
    HI_U8  speed_report_switch;             //速率上报开关:0,关；1,开
    HI_U16 detect_timer_period;             //检视定时器的周期,单位ms

    HI_U16 eth_rx_ip_cnt_min;               //以太网接收IP包的最小阈值
    HI_U16 eth_tx_ip_cnt_min;               //以太网发送IP包的最小阈值
    HI_U16 mac_rx_ip_cnt_min;               //MAC接收IP包的最小阈值
    HI_U16 mac_tx_ip_cnt_min;               //MAC发送IP包的最小阈值

    HI_U8  autio_detect_switch;             //自动检测功能开关:0,关；1,开
    HI_U8  tx_trace_switch;                 //发送报文跟踪上报开关
    HI_U8  rx_trace_switch;                 //接收报文跟踪上报开关
    HI_U8  tx_time_swith;                   //发送时间上报开关
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
    HI_U8  plc_ip[4];                           // PLC IP地址网段
    HI_U8  plc_net_mask[4];                     // PLC IP地址掩码

    HI_U8  auto_mapping;                        // 是否通过MAC地址映射为ETH IP
    HI_U8  rsvd[3];
    HI_U8  eth_ip[4];                           // ETH IP地址网段
    HI_U8  eth_net_mask[4];                     // ETH IP地址掩码

    DIAG_CMD_IP_ROUTE_ST ndm_route_sta;         // 抄控器路由项，路由3
    DIAG_CMD_IP_ROUTE_ST ndm_route_ndm;         // 抄控器路由项，路由3
    DIAG_CMD_IP_ROUTE_ST concentrator_route;    // 集中器路由项，CCO端不设置该NV项，gate_way在组网后，动态获取，路由2
    DIAG_CMD_IP_ROUTE_ST test_route;            // 性能测试路由项
}DIAG_CMD_CONFIG_IP_ROUTE_STRU;
typedef struct _DIAG_CMD_CONFIG_SW_NTB_
{
    HI_U32  ntb_control_mode;                           // NTB控制模式(芯片控制或者软件控制)
    HI_U32  synch_time_reset_th;                        // NTB时钟同步超时门限
    HI_U32  synch_time_detch_th;                        // NTB时钟同步定时检查定时器时长

    HI_S16  freqerr_boot_set_flag;                      // 是否上电读取FLASH中存储的频偏
    HI_S16  freqerr_boot_nv;                            // FLASH中存储频偏(暂时不用)

    HI_U8   detect_cycle_flag;                          // 工作周期检测开关
    HI_U8   exception_reset_to_initial_flag;            // 异常复位到初始化开关
    HI_U8   freqerr_nv_refresh_flag;                    // 获取工作频偏开关
    HI_U8   synch_detect_flag;                          // 同步超时检测开关

    HI_U32  ntb_detect_stat_cycle_len;                  // NTB统计信标帧个数周期时长
    HI_U16  ntb_rcv_beacon_th;                          // 统计信标帧中每个站点入选最低个数门限
    HI_U8   ntb_dectect_stat_flag;
    HI_U8   ntb_synch_send_beacon_switch;               // NTB只有在同步状态，才允许本站点发送信标策略开关
    HI_U32  detect_cycle;                               // 工作周期长度

    HI_U16  h_par_th;                                   // 虚检峰均比门限
    HI_U16  fccs_fail_th;                               // FC校验失败门限
    HI_U16  fccs_success_th;                            // FC校验成功门限

    HI_U16  pb_crc_fail_th;                             // PB块失败门限
    HI_U16  pb_crc_success_th;                          // PB块成功门限
    HI_U16  pb_crc_success_nv_th;                       // 存储频偏到FLASH中PB校验成功门限

    HI_U32  ntb_soft_control_rcv_beacon_len;            // 软件控制NTB，未接收到信标帧最长时长门限
    HI_U32  ntb_soft_control_len;                       // 软件控制NTB最长时长门限

    HI_U8   ntb_chip_state1_exception_th;                // 时钟同步芯片控制下状态1异常次数门限
    HI_U8   rsvd[3];
}DIAG_CMD_CONFIG_SW_NTB_STRU;
typedef struct _DIAG_CMD_CONFIG_BEACON_NV_
{
    HI_U8 alloc_slot_oneself_flag;      // 无时隙可用时，是否产生时隙发送数据
    HI_U8 cco_update_beacon_slot_flag;  // CCO端信标时隙是否根据站点的相位切换相位开关
    HI_U8 ptcl_detect_bcn_send_cnt;     // 每个协议探测信标的发送次数
    HI_U8 ptcl_detect_bcn_send_mode;    // 协议探测信标的发送模式

    HI_U32 alloc_slot_time_th;          // 多长时间收不到信标后，产生时隙
    HI_U32 alloc_slot_len;              // 产生的时隙时长
    HI_U32 protect_cp_len;              // 重复利用上个周期时隙的保护间隔
    HI_U32 csma_slot_slice_th;          // CSMA时间分片的最小单位，单位:SLICE_SLOT_UNIT(10ms)
}DIAG_CMD_CONFIG_BEACON_NV_STRU;

typedef struct _DIAG_CMD_CONFIG_LOCAL_BROADCAST_DEFAULT_SEND_TIMES_NV_STRU_
{
    HI_U32 local_broadcast_default_send_times;  //本地广播报文默认发送次数
}DIAG_CMD_CONFIG_LOCAL_BROADCAST_DEFAULT_SEND_TIMES_NV_STRU;

typedef struct _DIAG_CMD_CONFIG_FILTER_SWITCH_NV_STRU_
{
    HI_U32 filter_switch;   //报文过滤开关

    HI_BOOL is_freq_filter_enable;
    HI_U8  filter_timeout;
    HI_U8  pad[2];
}DIAG_CMD_CONFIG_FILTER_SWITCH_NV_STRU;

#define FREQ_CHANNEL_CNT  (13)

typedef struct _DIAG_CMD_CONFIG_BLIND_CHECK_NV_STRU_
{
    HI_BOOL blind_switch;
    HI_U8   chip200_v100_freq[6];              // V200芯片的V100频段序列(不包含全频)
    HI_U8   chip200_v200_freq[10];             // V200芯片上V200频段序列(不包含全频)
    HI_U8   chip200_full_freq[2];              // V200芯片上全频序列
    HI_U8   chip200_sg_freq[6];              // V200芯片上国网频段序列
    HI_U8   blind_check_mode;                   // 盲检模式
    HI_BOOL sta_equip_mode_flag;                //陪测sta工装模式标记
    HI_U8   rsvd;

    HI_U8   sg_standard_freq[4];               // SG标准协议，用于SG切换频段使用，默认10,12,255,255

    HI_U32  whole_network_change_freq_time;    // CCO通知STA多久后变更频段
    HI_U32  blind_check_time;                  // 盲检周期
}DIAG_CMD_CONFIG_BLIND_CHECK_NV_STRU;

//抄控器的NV项
typedef struct _DIAG_CMD_CONFIG_NDM_CONNECT_PARA_STRU_
{
    HI_U8  bea_normal_duration;
    HI_BOOL fix_meter_ndm_tei_enable;              //在整表测试模式下手动配置抄控器的TEI开关
    HI_U16 meter_check_frame_time;                 //整表测试帧发送时间,单位ms

    HI_U32 query_nm_attr_cnt_max_per_tei;           //查询本网络基本属性的时所允许的最大查询次数
    HI_U32 notify_meter_check_beacon_time;          //整表测试通知信标周期ms
    HI_U32 maintain_meter_check_beacon_time;        //整表测试维护信标周期ms
    HI_U32 close_meter_check_time;                  //关闭整表检测功能的时间，单位:s

    HI_BOOL sta_meter_connect_enable;               //STA上的整表测试开关，1:使能，0:关闭
    HI_U8  pad3;
    HI_U16 ndm_resend_times;

    HI_U16 ndm_tei;
    HI_BOOL heartbeat_send_enable;                  //抄控器心跳报文发送开关
    HI_U8  work_mode;                               //抄控器的工作模式

    HI_U8  ndm_attr_br_autio_enable;                //抄控器是否自动广播属性开关
    HI_U8  ndm_attr_br_pk_max;                      //抄控器属性广播的最大报文数
    HI_U16 query_nm_attr_tei_cnt_max;               //查询本网络基本属性的时所允许的最大查询tei次数

    HI_U32 ndm_attr_br_period;                      //抄控器属性广播周期
    HI_U32 qurey_nm_attr_period;                    //查询本网络基本属性的定时器的周期
    HI_U32 refresh_disc_timer_period;               //抄控器的发现列表刷新定时器的周期
    HI_U32 connect_monitor_timer_period;            //抄控器强制连接监测定时器的周期
    HI_U32 heartbeat_timer_period;                  //抄控器的心跳定时器的周期
}DIAG_CMD_CONFIG_NDM_CONNECT_PARA_STRU;

typedef struct _DIAG_CMD_CONFIG_STA_GPIO_SET_PARA_STRU_
{
    HI_U8 tx_app_switch;       //0表示发送MAC报文时置高GPIO管脚，1表示发送APP报文时置高GPIO管脚
    HI_BOOL send_one_bcn_in_bpc_enable;  // 在一个bpc内只允许发送一个信标
    HI_U16 message_send_length; //STA管脚拉高的时间限制单位us
    HI_BOOL energy_mode_enable;         // 能源模式使能开关
    HI_U8 detect_bcn_send_interval;     // 能源模式下V100探测信标帧发送间隔,默认6s,单位s
    HI_BOOL energy_test_mode_enable;    // 能源模式下，用于自测开关,默认HI_FALSE
    HI_U8 pad;
    HI_U32 pad1;
}DIAG_CMD_CONFIG_STA_GPIO_SET_PARA_STRU;

#define MAX_PHY_SLEEP_CYCLE_LISTEN_NUM      (5)
#define MAX_OPEN_PHY_TIME_RANGE_NUM         (3)

typedef struct
{
    HI_BOOL phy_low_power_switch;               // PHY低功耗开关
    HI_BOOL phy_sleep_in_power_on_switch;       // 上电状态下是否PHY是否周期性睡眠
    HI_BOOL low_power_dfx_switch;               // 低功耗可维可测开关
    HI_U8   close_phy_cnt_assoc;                // 关联状态下，关闭PHY的周期数

    HI_U8   open_phy_cnt_assoc;                 // 关联状态下，开启PHY的周期数
    HI_U8   suspend_phy_sleep_num;              // 盲检多少轮后，挂起PHY睡眠1个周期
    HI_U16  assoc_to_listen_timeout;            // 关联状态切换到监听状态的超时时间，单位秒

    HI_U16  power_on_keep_work_time;            // 上电保持工作的时间，单位毫秒
    HI_U16  init_power_on_timeout;              // 上电状态切换到监听状态的时间，单位秒

    HI_U8   random_sleep_num;                   // 睡眠多个周期随机一次
    HI_U8   close_phy_cnt_listen;               // 监听状态下，关闭PHY的周期数
    HI_U8   open_phy_cnt_listen;                // 监听状态下，开启PHY的周期数
    HI_U8   wait_assoc_cnf_time;                // 等待关联回复的时间，单位秒

    HI_U8   continue_open_phy_choose_cnt;       // 关联多少轮后，持续打开PHY
    HI_U8   good_waking_sleep_rate_choose_cnt;  // 关联多少轮后，选择好的醒睡比
    HI_U8   good_close_phy_cnt_assoc;           // 好的醒睡比关闭PHY的周期数
    HI_U8   good_open_phy_cnt_assoc;            // 好的醒睡比打开PHY的周期数

    HI_U32  phy_sleep_cycle_power_on;           // 上电状态下，PHY睡眠的周期，单位微秒
    HI_U32  phy_sleep_cycle_listen[MAX_PHY_SLEEP_CYCLE_LISTEN_NUM];           // 监听状态下，PHY睡眠的周期，单位微秒
    HI_U32  phy_sleep_cycle_assoc;              // 关联阶段，PHY睡眠的周期，单位微秒
    HI_U16  open_phy_time_begin[MAX_OPEN_PHY_TIME_RANGE_NUM];                 // 保持PHY打开的开始时间
    HI_U16  open_phy_time_end[MAX_OPEN_PHY_TIME_RANGE_NUM];                   // 保持PHY打开的结束时间
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
    HI_U8 ucAccessMode;                     //连接模式，取值HI_NDC_CHL_ACCES_MODE_E
    HI_U8 ucPreferenceFreq;                 //优先搜索的频段
    HI_U8 ucPreferenceSNID;                 //优先搜索的SNID
    HI_U8 aucPreferenceCcoMac[6];           //当遇到搜索到多网络后，优先选择哪个CCO连接
    HI_U8 ucFreqScanPeriodCco;              //CCO频段监听周期，单位秒，默认120秒。
    HI_U8 ucFreqScanPeriodSta;              //STA频段监听周期，单位秒，默认170秒。
    HI_U8 ucFreqShortPeriod;                //设置频段监听短周期，单位秒，默认60秒

    HI_U8 ucWaitConnectTimeOut;             //等待连接完成超时时间，默认4秒。
    HI_U8 ucIdleTimeout;                    //网络状态变更为IDLE后，最长持续的时间，到期后转SEARCH，单位秒，默认5秒
    HI_U8 ucReportPeriod;                   //搜网结果的上报周期,单位秒，缺省3秒
    HI_U8 ucMaxSearchCnt;                   //自动搜网的最大轮数，一轮指遍历所有有效频段，默认为3;

    HI_U8 aucFreqIdx[FREQ_TRY_NUM];         //NDM使用的频段搜索范围.HI_MDM_PLC_FREQ_1,HI_MDM_PLC_FREQ_2,HI_MDM_PLC_FREQ_3
    HI_U8 ucFilterFreq;                     // 针对V200搜网过滤非当前频段下的其他频段信息
    HI_U8 ucFixRxEn;                        // 针对V200抄控器关闭并行检测功能
    HI_BOOL isFilterZeroIp;                 // 是否过滤V100协议下NDC条目中IP是0的信

    HI_U8 ucMaxConnectRetry;                //连接失败的重试次数，默认3次。
    HI_U8 ucMinSacnFcCnt;                   //频段监听周期内，最少需监听到的FC个数，不满足则短周期机制生效，默认为3
    HI_U8 ucMaxShortScan;                   //使用短周期自动搜网的最大次数，默认为3

    HI_U8 ucReportPrintSwitch;              //NDC上报给HSO的消息，是否打印到message的开关。缺省不打印。
    HI_U8 ucConnMonPeriod;                  //NDC连接监控刷新的周期，单位秒，默认10秒。
    HI_U8 ucAutoSearchNetEnable;            //自动网功能开关
    HI_U8 ucExpireCheckPeriod;              //搜网结果定时检测老化定时器时间，默认为60s

    HI_U8 ucNtbAtuoSynchEnable;              // 搜网过程中，动态进行NTB同步功能开关，默认为开
    HI_U8 ucRcvFcTh;                         // 设置是否进行NTB同步条件，前提接收fc的个数，默认为5个
    HI_U8 pad[2];
}DIAG_CMD_CONFIG_MAC_NDM_CHL_STRU;

typedef struct
{
    HI_U32 mutli_exception_by_diff_mac_time_th;        //SNID相同，MAC不同的多网络异常的时间门限值,单位min
    HI_U32 multi_exception_by_diff_mac_cnt_th;         //SNID相同，MAC不同的多网络异常的次数门限值
    HI_U32 multi_exception_by_ntb_reset_time_th;       //NTB异常复位引起的多网络异常的时间门限值,单位min
    HI_U32 multi_exception_by_ntb_reset_alarm_cnt_th;  //NTB异常复位引起的多网络异常的次数门限值
    HI_U32 multi_exception_timer_period;               //多网络异常定时器周期，单位ms
}DIAG_CMD_CONFIG_MAC_MULTI_EXCEPTION_CTRL_STRU;

//自动功率控制
typedef struct
{
    HI_U8   is_power_ctrl_enable    : 1;                                           //功率控制使能开关{1}
    HI_U8   is_recover_power_enable    : 1;                                   // 是否恢复原有数字发射功率开关{1}
    HI_U8   set_white_list_time_min_th    : 4;
    HI_U8   rsvd    : 2;
    HI_S8   digital_power_v100;
    HI_U16  set_v100_detect_digital_power_time_th;

    HI_S8   digital_power[4];                                                       //全网通知的数字发射功率{9,14,16,18}

    HI_U16  send_interval;                         //whole_net_set_digital_power_timer到期后，发送全网设置发射功率的V100探测信标的帧间隔，单位s{15}
    HI_U16  send_cnt_max;                        //whole_net_set_digital_power_timer到期后，发送全网设置发射功率的V100探测信标的最大个数{120}

    HI_U32  set_auth_time_th;                                                //设置白名单后不启动功率控制的时间门限，单位s{3600}1h
    HI_U32  whole_net_set_digital_power_timer_period;         //全网通知数字发射功率的周期，whole_net_set_digital_power_timer超时时间，单位s{3600}1h
    HI_U32  power_ctrl_form_timeout_period;                          //功率控制缓存STA入网超时时间，power_ctrl_form_timeout_timer超时时间，单位s{3600}1h

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

//通信率上报设置
typedef struct
{
    HI_U8 report_switch;  // 上报开关
    HI_U8 report_period;  // 上报周期(report_period个评估周期上报一次)
    HI_U8 report_num;
    HI_U8 pad;

    DIAG_CMD_CONFIG_POWER_CTRL_STRU power_ctrl_nv;
}DIAG_CMD_CONFIG_REPORT_COMM_RATE_STRU;

typedef struct
{
    HI_U8  sta_eth_enable;              // STA网口使能
    HI_U8  speed_test_enable_flag;      // 使能灌包测试模式
    HI_U16 mtu_size;
	HI_U8  packet_flag;
	HI_U8  rsd[3];
}DIAG_CMD_CONFIG_SPEED_TEST_NV;

typedef struct
{
    HI_U32 aging_time_th;               // 端口映射表老化时间，单位:ms
    HI_U32 aging_timer_cycle;           // 端口映射表老化检测定时器周期长度，单位:ms
    HI_U32 expired_time_th;             // 报文过期时间，单位:ms
    HI_U32 expired_timer_cycle;         // 报文过期定时器周期长度，单位:ms

    HI_U8  broadcast_port_fail;         // 获取端口失败后，是否广播发送数据
    HI_U8  packet_lid;                  // 报文优先级
    HI_U8  use_min_expired_fragment;    // 分片缓存不足时，替换过期时间最短的分片
    HI_U8  sta_port_communicate;        // 是否支持STA端口间通信

    HI_U8  capture_switch;              // 抓包MSG开关
    HI_U8  ethernet_interface_mode;     // 以太网口模式
    HI_U8  log_switch;                  // LOG信息打印开关
    HI_U8  rsvd;
}DIAG_CMD_CONFIG_PLC_SWITCH_NV;

#define MAX_FILTER_PROTOCOL_NUM     (20)
typedef struct
{
    HI_U8  filter_mode;                                 // 协议过滤模式
    HI_U8  filter_gratuitous_arp_switch;                // 过滤免费ARP报文开关
    HI_U8  rsvd[2];

    HI_U16 protocol_list[MAX_FILTER_PROTOCOL_NUM];      // 协议表
}DIAG_CMD_CONFIG_PLC_SWITCH_FILTER_NV;

typedef struct
{
    HI_U16 plc_multi_network_monitor_cycle;			// PLC链路多网络监听周期，单位:S
    HI_U16 evaluation_time_len;						// RF多网络评估时长，单位:S
    HI_U16 try_assoc_time_len;						// RF模式下最长关联时间，单位:S

    HI_U8  plc_blink_check_rounds_th;				// PLC模式下盲检轮次
    HI_U8  plc_low_rate_continuous_cycle_th;		// PLC模式下最低通信率持续轮次
    HI_U8  plc_low_rate_th;							// PLC模式下最低通信率
    HI_U8  rf_try_join_network_rounds_th;			// RF模式下关联轮次

    HI_U16 plc_not_join_network_th;                 // PLC模式下最长未加入网络时间，单位:S
    HI_U16 rf_not_join_network_th;					// RF模式下未加入网络最长时间，单位:S
    HI_U16 network_mode_time_cycle;					// 网络模式定时检测周期，单位:S
    HI_U16 rf_not_rcv_complete_beacon_th;			// RF上最长未接收到完整信标帧时间，单位:S

    HI_U8  rf_rssi_th;                              // 信号强度门限
    HI_U8  sack_enable;                             // 是否回复SACK
    HI_U8  continue_timeout_end_send;               // SACK连续超时是否结束发送
    HI_U8  rf_link_rssi_th;                         // RF链路可通信的信号强度门限

    HI_U16 rf_comm_link_break_judgment_time_len;    // RF通信链路断开判断时长，单位:S
    HI_U16 link_test_stat_cycle_time_len;           // RF链路通信测试统计周期长度，单位:S

    HI_U8  min_rcv_link_test_frame_cnt;             // RF链路通信测试统计周期链路测试帧最小接收个数
    HI_U8  rf_proxy_change_up_limit;                 //RF链路代理变更上限
    HI_U8  rf_proxy_change_down_limit;               //RF链路代理变更下限
    HI_U8  rf_proxy_change_max_offset;               //RF链路代理变更大偏移

    HI_U16 proxy_change_check_cycle;                 //RF链路代理变更定时检测周期，单位:S

    HI_U8 rf_proxy_change_middle_offset;
    HI_U8 rf_proxy_change_min_offset;                //RF链路代理变更小偏移
	HI_U8 rf_used_window_num;
    HI_U8 rssi_down_limit;

    HI_U8 rf_window_weighting[4];

    HI_U8 rssi_priority_limit;
    HI_U8 rf_assoc_max_num;
    HI_U8 network_mode_list[4];
    HI_U8 mf_proxy_comm_rate_th;					// 多频下代理通信率切出门限
    HI_U8 mf_low_proxy_comm_rate_contiguous_th;		// 多频下持续代理通信率满足门限的时长
}DIAG_CMD_CONFIG_DUAL_MODULE_PARA_NV;

//台区识别流程控制NV项
typedef struct
{
    HI_U8 tf_calcu_mode;                                // 台区识别计算方式，针对V200芯片,0:逻辑识别，1:软件识别
    HI_U8 tf_set_code_type;                             // 码字选择,0:不控制，1:选择13bit码字，2:选择127bit码字
    HI_U8 tf_get_ntb_mode;                              // 获取NTB来源，针对V200芯片,0:AD,1:外部电路
    HI_U8 tf_networking_protocol;                       // 网络协议，0:V100协议，1:V200协议

    HI_U8 broadcast_notify_max_cnt;                     // 广播通知台区识别开启，停止次数,默认20次
    HI_U8 broadcast_notify_period;                      // 广播通知台区识别周期，默认为2s，单位s
    HI_U8 set_ad_collect_data_mode;                     // V200芯片采数模式配置，0:1M采样率，1:500K采样率
    HI_U8 report_line_data;                             // V200芯片上报32路结果，0:不上报，1:上报有效，2:上报所有

    HI_U16 bcn_notify_send_timer_period;                //台区识别通知信标的发送时间,单位:s，180
    HI_U16 ndm_chl_req_timeout;                         //NDC通道请求超时的时间，单位:ms,6000
    HI_U16 restart_identify_time;                       //自动重新启动台区识别的间隔，单位:s,默认值:300s
    HI_U16 parallel_query_timeout;                      //并行查询的超时时间，单位:ms, 12000

    HI_BOOL is_restart_identify;                        //站点查询超时，补充台区识别控制开关，默认打开HI_TRUE
    HI_U8  rcv_bcn_notify_disable_cnt_min;              //抄控器解除搜网限制时需要连续收到通知台区识别结束的信标的个数
    HI_U8  change_cco_info_min_time;                    //CCO信息变化的最智时间，单位s
    HI_U8  ndm_to_sta_send_num;                         /*抄控器对STA模式，台区序列发送轮数，默认5轮，注: 最大为9轮*/

    HI_U8  ndm_chl_req_send_cnt_max;                    //抄控器通道请求发送最大次数,50
    HI_U8  query_parallel_send_num;                     //台区识别查询并行发送的站点数，5
    HI_U8  allow_sta_identify_cnt_max;                  //每个站点允许的最大识别次数,2
    HI_U8  per_sta_query_cnt_max;                       //每个STA查询的最大次数, 5

    HI_U8  tf_rcv_mode;                                 //台区识别接收模式
    HI_BOOL is_tf_enable;                               //台区识别使能标识
    HI_BOOL is_seq_cycle_mode_disable;                  //台区识别序列周期发送模式开关
    HI_U8  is_multi_cco_notify_enable;                  //多CCO通知启动台区识别开关

    HI_U16 clear_line_purse_diff_cnt;                     // V200芯片处理13bit码字识别，前后purse_cnt差值，超过182,即14个窗口，清除该线路
    HI_U16 wait_union_jude_time;                           // V200芯片的13bit码字识别等待联合判决时间，单位ms，默认为5720

    HI_U16 send_ntb_collect_notify_timer_period;        //采集NTB通知的发送周期
    HI_U8  om_ntb_collect_mode;                         //NTB采集开关模式
    HI_U8  send_ntb_collect_num_max;                    //发送NTB采集通知报文的最大次数

    HI_U8 check_ndm_connect_time_out;                   // CCO检测NDM是否连接的时间，单位:min,1
    HI_BOOL is_report_result_req;
    HI_BOOL is_report_result_rsp;
    HI_BOOL is_collect_ntb_by_bcn_notify;

    HI_U16 send_13bit_seq_delay_interrupt_max_cnt;      // 13bit码字相位切换最大中断数，为400个工频周期
    HI_U16 send_127bit_seq_delay_interrupt_max_cnt;    // 127bit码字相位切换最大中断数，为480个工频周期
    HI_U8 freq_to_seq_array_index[FREQ_TRY_NUM];        // 台区识别序号与频段的映射关系
    HI_U8 collect_ntb_phase;                            // 实时采集NTB的相位，1:A,2:B,3:C
    HI_BOOL is_correction_voltage_enable;               // 是否启用过零工装校正电压,默认不启用
    HI_U8 send_seq_type_cnt_max;                      // 发送码字类型个数最大值，默认2
    HI_U8  ndm_tf_mode_check_timer_period;              //抄控器禁止搜网模式的检测定时器，单位s，缺省值120
    HI_U16 send_seq_min_interval;                       //台区识别序列的发送最小间隔，单位s，默认值180

    HI_U32 ndm_send_delay_interrupt_cnt_max;            //抄控器相位切换延时的最大中断数，单位:20ms,缺省值500
    HI_U8  sta_check_tf_period_num_max;                 //检测STA是否支持台区识别的检测轮次，每轮周期为200ms,缺省值100
    HI_U8  ndm_set_edge_mode;                           //设置NDM发送序列时对正负沿的控制，0:不控制，1:设置为沿0，2:设置为沿1
    HI_U8  rcv_mode_switch_phase_cnt;                   //接收模式切换的周期，单位:每个相位的序列发送时长
    HI_BOOL is_zero_cross_forever;                      //过零中断是否永远打开

    HI_U16 other_tf_noise_th;                           //疑似邻台区的噪声均值门限，单位us，缺省值8
    HI_U16 other_tf_signal_high_th;                     //疑似邻台区的信号均值门限，单位us，缺省值15
    HI_U16 other_tf_signal_low_th;                      //疑似邻台区的信号均值门限，单位us，缺省值13
    HI_U16 cur_result_valid_time;                       //当前台区识别结果有效的时间，单位s，缺省值7200，最大值约18个小时
    HI_U32 his_result_valid_time;                       //历史台区识别结果的有效时间，单位s，缺省值172800(2天)

    HI_U16 abnormal_noise_th;                          //异常大噪声门限，单位us，缺省值20us
    HI_U16 collect_noise_th;                            // 采集大噪声的周期为collect_noise_th个2.86s时间个数，默认值为629，表示30分钟。

    HI_U8 clear_collect_noise_time;                     // 清除噪声统计时间，默认为18s
    HI_BOOL is_real_time_calc_noise_enable;            // 是否实时计算噪声值，默认为1，实时计算
    HI_BOOL send_seq_type_enable;                       // 发射机端控制发送码字使能，默认HI_FALSE，不使能
    HI_U8 collect_noise_mean_th;                       // 采集噪声门限,默认为20us

    HI_BOOL report_ntb;                               // V200芯片识别上报每次结果NTB,power_cnt，默认上报
    HI_BOOL report_noise;                             // V200芯片识别上报噪声统计
    HI_BOOL is_test_have_v100_sta;                 // 网络中存在V100芯片STA  ，打桩测试
    HI_U8 enable_test_ndm_rx;                         // 测试状态下，设置抄控宝启动，关闭模式

    HI_U16 send_127bit_inter_seq_delay_interrupt_max_cnt;  // 127bit间隔扰动码字相位间隔，4.6s,默认230，单位工频周期(20ms)
    HI_U8 precise_zero_corss_judge_th;                // 判断精准过零正负半轴差值，默认10，单位ms
    HI_BOOL disable_poweredge_type;                   // 调试项，是否允许配置中断边沿，默认0，允许配置

    HI_U8 send_seq_type0;                             // 发射机使能控制情况下，发送第1组码字类型，默认0
    HI_U8 send_seq_type1;                             // 发射机使能控制情况下，发送第2组码字类型，默认1
    HI_U8 send_seq_type0_turn_cnt;                   // 发送码字0的轮次,默认55
    HI_U8 send_seq_type1_turn_cnt;                   // 发送码字1的轮次,默认5

    HI_U16 send_seq_type0_delay_max_cnt;             // 发送码字0的相位之间间隔，默认400
    HI_U16 send_seq_type1_delay_max_cnt;             // 发送码字1的相位之间间隔，默认480
}DIAG_CMD_CFG_TF_NV_STRU;

//台区识别过零检测NV项
typedef struct _DIAG_CMD_CONFIG_ZERO_CROSS_NV_
{
    HI_U16 noise_limit[7];                      /*[TF_ZERO_CROSS_NOISE_LIMIT_CNT],噪声门限,单位是40ns,门限应由大至小配置*/
    HI_U8  false_alarm[7];                      /*[TF_ZERO_CROSS_NOISE_LIMIT_CNT],设置虚警门限,取值范围1－100，例如设置为1，则以平均值的0.01为门限，设置为10，则以平均值的0.1为门限*/
    HI_BOOL is_jammer;                          //发射机工作在干扰发射模式
    HI_BOOL ndm_rx_gear_switch;                 /*抄控器对抄控器模式下，接收端接收序列档位开关，0:自动档位，1:手动切换档位，默认为0*/
    HI_U8  ndm_rx_gear;                         /*抄控器对抄控器模式下，接收端接收序列的档位，0:为240K的接收档位，1:为1M的接收档位，默认为1*/

    HI_U16 jammer_seq_a;                        //干扰器发送的序列A
    HI_U8  novel_mode;                          //是否使能有序列发送时的扰动统计功能，默认为HI_FALSE
    HI_U8  jammer_a_send_cnt;                   //干扰器序列A发送的次数

    HI_U8  jammer_b_send_cnt;                   //干扰器序列B发送的次数
    HI_U8  ndm_to_ndm_send_num;                 /*抄控器对抄控器模式，台区序列发送轮数,默认4轮，注: 最大为7轮*/
    HI_U8  pad;
    HI_U8  novel_noise_limit_index;             // 4,大于此值

    HI_U8  tf_serial_num;                       /*选择要发送序列的台区编号*/
    HI_U8  noise_limit_mode;                    //噪声门限模式，比特0为1代表门限0(40us)使用，比特6为1代表门限6(4us)使用，缺省值0x48
    HI_U16 err_value_threshold;                 /*异常值判断门限*/

    HI_U16 jammer_seq_b;                        //干扰器发送的序列B
    HI_U16 novel_noise_mean_max_th;             //566us

    HI_U8  jammer_c_send_cnt;                   //干扰器序列C发送的次数
    HI_BOOL report_receive_ntb_switch;          /*上报台区识别接收NTB，和软信息开关*/
    HI_BOOL report_detect_sequence_switch;      /*上报台区识别检出序列开关*/
    HI_BOOL tf_enable_pos_opp_switch;           /*启动单相位正反沿连续切换*/

    HI_U8  tf_switch_phase_mode;                /*设置在某个相位发送序列,0:全部相位，1:A相位，2:B相位，3:C相位*/
    HI_BOOL tf_single_phase_tx;                 /*在某一相位持续发送标志*/
    HI_BOOL tf_enable_phase_ctrl;               /*启动软件控制相位切换开关，0:不开启，1:开启*/
    HI_BOOL is_random_jammer;                   //发送随机序列

    HI_U16 v200_soft_th;                          // 默认为73
    HI_U8  tx_period_cnt_max;                   //ndm-sta发射机发送序列的周期数，单位:正反沿三相遍历一遍的序列发送时长 默认为3
    HI_U8  ndm_send_period_cnt;                 //ndm-ndm发送周期数 默认为1

    HI_U8  sign3_synch_suc_th;                  // 精同步符号3码字碰撞识别成功次数门限，默认为22
    HI_U8  big_err_follow_cnt_max_percent;                              //统计跟随异常大脉冲个数的最大百分比，默认为25%
    HI_U8  novel_signal_mean_th;                //40us
    HI_U8  novel_noise_mean_th;                 //40us

    HI_U8  ndm_ntb_report_time_switch;          //默认为0 : 每组上报一次；1 : 只在判断识别成功时上报; 同时控制软信息上报
    HI_U8  big_err_diff_cnt_max_percent;                                //统计跳变异常大脉冲个数的最大百分比，默认为25%
    HI_U8  noise_statistic_max_percent;                                //统计噪声的最大百分比，默认为25%
    HI_BOOL is_between_window_ntb_filter_enable;//组间ntb异常处理开关，默认为1:打开；0:关闭；

    HI_U16 identify_disable_time_len;           // STA台区识别关闭时长门限,单位:s,90
    HI_U16 big_err_diff_th;                     //异常大脉冲门限，默认55v扰动电压为566us，50v扰动电压为514us

    HI_BOOL is_in_window_ntb_filter_enable;     //窗口内的NTB过滤开关，默认为1:打开；0:关闭；
    HI_U8 ntb_thblock;                          //默认50ms  相关宏定义TF_ZERO_CROSS_LOST_PERIOD_CNT
    HI_U8 ntb_thperiod;                         //默认15ms  相关宏定义TF_ZERO_CROSS_LOST_PERIOD_CNT
    HI_U8 ntb_thms;                             //默认1ms

    HI_U8 ntb_thus;                             //默认30us
    HI_U8 sta_first_window_soft_th;             //默认与 v100_soft_th 的值相等，
    HI_U16 v100_soft_th;                 // 默认72

    HI_U16 novel_signal_mean_max_th;            //默认566,单位us
    HI_U16 sta_weight_th;                       //默认216

    HI_U16 sta_rough_weight_th;                 // 粗同步联合判决软值门限，默认为144us
    HI_U16 sta_exact_weight_th;                 // 精同步联合判决软值门限，默认为144us

    HI_U16 static_wobble_limit[6];              //[TF_NOISE_TH_CNT],门限应由大至小配置

    HI_U16 ndm_report_ntb_rcv_level_switch_time;//ndm采集NTB上报档位切换定时器时间, 单位s，默认160s
    HI_BOOL is_three_phase_together_enable;     // 发射机上电自检是否将A,B,C三相接入一起判断为故障，默认为0:故障，不允许启动台区识别
    HI_U8 ndm_real_noise_avg_cnt;               // 发射机实时刷新电力线扰动值的个数，默认为12个

    HI_U8 ndm_crosstalk_min_th;                 // 接收机屏蔽串扰最小门限，默认13，单位us
    HI_U8 ndm_crosstalk_max_th;                 // 接收机屏蔽串扰最大门限，默认40，单位us
    HI_U16 jammer_seq_c;                        //干扰器发送的序列C

    HI_U8 expect_freq1;                         //预期的序列频段1
    HI_U8 expect_snid1;
    HI_U8 expect_freq2;
    HI_U8 expect_snid2;
}DIAG_CMD_CONFIG_ZERO_CROSS_NV_S;


//相位识别NV项
typedef struct
{
    HI_BOOL is_pi_enable;                           //相位识别使能开关，HI_TRUE
    HI_U8  ntb_query_parallel_num;                  //过零NTB查询的并发站点数，3
    HI_U8  ntb_query_turn_cnt_max;                  //过零NTB查询的最大轮次,5
    HI_U8  identify_fail_cnt_max;                   //相位识别允许的最大失败次数,5

    HI_U16 parallel_query_timeout;                  //并行查询超时时间，单位:ms,12000
    HI_U16 check_over_timeout;                      //确认相位识别结束的定时器，单位:s，120

    HI_U8  phase_cnt_th_min;                        //相位识别统计的最小阈值，15
    HI_U8  phase_cnt_other_th_min;                  //其它相非零时，本相的最小阈值，10
    HI_U8  phase_cnt_other_th;                      //其它相非零时，其它相的阈值，2
    HI_U8  phase_cnt_no_other_th_min;               //其它相为0时，本相的最小阈值，6

    HI_U8  two_phase_cnt_th_min;                    //两个相位识别统计的最小阈值，6
    HI_U8  two_phase_cnt_other_th_min;              //两个相位识别情况下，其它相非零时，本相的最小阈值，4
    HI_U8  two_phase_cnt_other_th;                  //两个相位识别情况下，其它相非零时，其它相的阈值，2
    HI_U8  two_phase_cnt_no_other_th_min;           //两个相位识别情况下，其它相为0时，本相的最小阈值，2

    HI_U8  three_phase_cnt_th_min;                  //三个相位识别统计的最小阈值，4
    HI_U8  three_phase_cnt_other_th_min;            //三个相位识别情况下，其它相非零时，本相的最小阈值，3
    HI_U8  three_phase_cnt_other_th;                //三个相位识别情况下，其它相非零时，其它相的阈值，1
    HI_U8  three_phase_cnt_no_other_th_min;         //三个相位识别情况下，其它相为0时，本相的最小阈值，2

    HI_BOOL is_report_zero_cross_ntb_req;           //上报过零查询报文的开关，HI_TRUE
    HI_BOOL is_report_zero_cross_ntb_rsp;           //上报过零应答报文的开关，HI_TRUE
    HI_BOOL is_report_zero_cross_ntb_cco;           //上报CCO的基准过零NTB
    HI_BOOL is_slow_query_enable;                   //慢速查询模式

    HI_S32 sta_zero_cross_ntb_offset;               //STA过零NTB相对于CCO的偏移量，单位:1/25MHz
    HI_U16 ntb_report_timer_period;                 //CCO上报缓存的NTB的周期，单位s
    HI_U16 cco_period_th;                           //CCO周期判断门限，单位ms

    HI_U8 insert_th;  //异常半周期数据筛选下限,默认值为5，单位:ms
    HI_U8 miss_th;    //异常半周期数据筛选上限，默认值为15，单位:ms
    HI_BOOL is_min_flag;  //是否使用严格门限
    HI_U8 period_th;    //周期个数门限，默认值为9

    HI_U8 line_error_th;//零火线接线正确正半轴大于负半轴的周期个数门限,默认值为8
    HI_BOOL is_check_mode;     // 是否为送检模式,HI_TRUE表示为送检模式，HI_FALSE表示正常模式
    HI_U8 two_phase_period_th;  // 两个相位情况下，周期个数门限，默认值为6
    HI_U8 two_phase_line_error_th; // 两个相位情况下，零火接线正确，正半轴>负半轴门限个数，默认值为5

    HI_U8 three_phase_period_th;   // 三个相位情况下，周期个数门限，默认值为4
    HI_U8 three_phase_line_error_th; //三个相位情况下，零火接线正确，正半轴>负半轴门限个数，默认值为3

    HI_U8 broadcast_send_max_cnt;                  // CCO广播通知最大次数,默认3次
    HI_U8 broadcast_send_durtion;                  // CCO广播通知时间间隔，默认6s，单位s。

    HI_U8 period_check_min_cnt;                    // 周期检查最小次数，24次，大概为2小时
    HI_U8 period_check_max_cnt;                    // 周期检查最大次数，48次，大概为4小时
    HI_U8 pad[2];
}DIAG_CMD_CONFIG_PI_NV_S;

// 故障定位NV项控制参数
typedef struct
{
    HI_U8 fl_enable_switch;                    //开启故障定位开关:默认开启
    HI_U8 pad;
    HI_U8 cco_mac[HI_PLC_MAC_ADDR_LEN];   //要定位网络的CCO的MAC

    HI_U8 freq;                                                   //要定位网络的freq
    HI_U8 snid;                                                   //要定位网络的snid
    HI_U8 fl_refresh_topo_period;                    //TOPO刷新周期，单位:min
    HI_U8 continus_not_rcv_topo_max_cnt;    //连续刷新拓扑失败门限
}DIAG_CMD_CFG_FL_NV_STRU;

typedef struct
{
    HI_BOOL mac_ctrl_enable;                  // mac控制占空比策略开关，调试使用
    HI_BOOL duty_enable;                      // 占空比策略使能开关
    HI_U8 duty_percent;                       // 占空比大小
    HI_BOOL is_vs_enable;                     //是否启用电压跌落功能

    HI_U32 max_sendlength;                    // 最大允许发送报文时长，单位ms

    HI_BOOL period1_duty_percent_enable;      // 使能周期1内占空比统计
    HI_BOOL period2_duty_percent_enable;      //使能周期2内占空比统计
    HI_BOOL period3_duty_percent_enable;      //使能周期3内占空比统计
    HI_U8 con_high_voltage_max_cnt;           //连续检测高电平最大次数

    HI_U32 duty_period1;                  // 周期1长度，单位ms
    HI_U32 duty_period2;                  // 周期2长度，单位ms
    HI_U32 duty_period3;                  // 周期3长度，单位ms

    HI_U8 check_zero_cross_int_period;        // 电压跌落检测过零中断个数，默认50ms
    HI_U8 check_voltage_level_period;         // 电压跌落检检测电压恢复时间，默认10ms
    HI_U8 check_tx_power_period;              // 电压跌落后调整功率，检查周期时间，默认30分钟,单位分钟
    HI_U8 check_zero_cross_min_cnt;           // 电压跌落后，检查接收过零中断个数，默认2个

    HI_U8 check_high_voltage_max_cnt;         // 检查高电平最大次数，默认10次，100ms
    HI_U8 low_tx_power;                       // 最小的发送功率，默认2，即4dB
    HI_U8 adjust_tx_power_step;               // 发送功率调整步长，默认2，即4dB
    HI_U8 adjust_tx_power_check_min_cnt;      // 调整发送功率后，连续检查没有出现电压跌落次数,默认20次
}DIAG_CMD_CONFIG_MAC_DUTY_CTRL_PARA_STRU;

//台区识别串扰策略NV
typedef struct
{
    HI_U8  lock_network_switch;     // 锁定网络开关
    HI_U8  pad;
    HI_U16 normal_lock_duration;    // 普通锁定时长，单位min

    HI_U32 lock_freqsnid_duration;  // 锁定频段和网络号时长，单位S
    HI_U32 lock_ccomac_duration;    // 锁定CCO MAC地址时长，单位S

    HI_U16 save_nv_lock_len_th;     // 锁定时长大于门限时，才将锁定MAC更新到NV，单位min
    HI_U16 leave_time_count_period; // 在锁定网络离网时长统计周期，单位min

    HI_U32 assoc_belong_lock_duration;  // 关联归属网络时，锁定时长，单位s
    HI_U32 join_belong_lock_duration;   // 加入归属网络时，锁定时长，单位s

    HI_U32 rst_max_delay_time;          // 普通锁定下，主动复位最大延迟时间，超过该时间不再delay

    HI_U16 safe_lock_duration;          // 安全锁定时长，单位min
    HI_U16 pad1;
}DIAG_CMD_CONFIG_TF_CROSSTALK_STA_PARA_STRU;

//USTA的NV参数
#define MAX_SWEEP_FREQERR_NUM           (5)
typedef struct
{
    HI_S16 sweep_step_freqerr[MAX_SWEEP_FREQERR_NUM];           // 扫频的频点
    HI_U16 send_search_frame_interval;                          // 发送搜索帧间隔，单位:毫秒

    HI_U8  sweep_freq_num;                                      // 扫频时频点的个数
    HI_U8  max_send_search_frame_num;                           // 发送搜索帧最大次数:每个频点上发送搜索帧的个数
    HI_U8  max_search_num;                                      // 最大搜索次数:所有频段搜索遍历轮次
    HI_U8  max_send_synch_frame_num;                            // 发送同步帧的最大次数

    HI_U16 passive_synch_timeout;                               // 被动同步超时时间，单位:毫秒
    HI_U16 send_synch_interval_not_synch;                       // 未同步状态下发送同步帧的间隔，单位:毫秒
    HI_U16 send_synch_interval_in_synch;                        // 已同步状态下发送同步帧的间隔，单位:毫秒
    HI_U16 data_transmission_timeout;                           // 数据传输超时，单位:秒
    HI_U16 listen_timeout;                                      // 监听超时时间，单位:秒
    HI_U16 heartbeat_cycle;                                     // 心跳周期，单位:毫秒，主要用于数据传输、监听、被动搜索状态定时器周期

    HI_U8  max_app_packet_send_fail_num;                        // 应用层报文最大发送失败次数
    HI_BOOL is_loopback_enable;                                 // 环回测试模式使能标识
    HI_U8  pad[2];
}DIAG_CMD_CONFIG_PTPC_STRU;

typedef struct
{
    HI_U16  nbi_start;
    HI_U16  nbi_end;
}DIAG_CMD_CONFIG_AVOID_FREQ_ENTRY;

typedef struct
{
    HI_BOOL emc_switch;                         // EMC认证开关
    HI_U8   report_static_cyc;
    HI_U8   avoid_freq_num;                     // 动态避让的频段个数
    HI_U8   avoid_freq_mask_extend;             // 动态频率避让延伸子载波个数

    HI_U16  freq_avoid_extend_time;                    //频率干扰消失后，需要扩展避让时长，单位:S
    HI_U16  power_control_pk_cycle;             // 功率控制报文周期，单位:S

    HI_U16  emc_periodic_detect_cycle;          // EMC定时检测周期，单位:MS
    HI_U8   send_fail_th_power_recovery;        // 功率恢复中报文发送失败门限
    HI_U8   power_adjust_step;                  // 功率调整步长

    HI_U8   max_channel_dec_thresh;             // 信道衰减最大值
    HI_U8   min_channel_dec_thresh;             // 信道衰减最小值
    HI_U8   white_noise_detect_th;              // 白噪声场景判定门限
    HI_U8   min_channel_dec_thresh_15;       // 频段15时动态功率控制模式下维持动态平衡的信道衰减下限: 47

    HI_U8   max_channel_dec_thresh_15;      // 频段15时动态功率控制模式下维持动态平衡的信道衰减上限: 52
    HI_U8   power_control_nv_switch;            // 是否启用功率控制开关
    HI_U8   send_fail_recovery_power_switch;    // 发送失败触发功率控制开关
    HI_U8   NoisePowTarget; //噪声目标功率

    HI_U8   powerloading_merge_th;              // 开槽合并门限
    HI_U8   lab_nbi_judge_th;                   //NBI实验室场景连续确认判断的次数门限
    HI_U8   lab_nbi_max;                          //判断为实验室场景的NBI门限
    HI_U8   min_channel_dec_thresh_16;      // 频段16时动态功率控制模式下维持动态平衡的信道衰减下限: 53


    HI_U8   max_channel_dec_thresh_16;    // 频段16时动态功率控制模式下维持动态平衡的信道衰减下限: 58
    HI_U8   default_afe_power;                   //emc模式下默认的afe功率:8
    HI_U8   pad2;
    HI_U8   pad3;

    HI_U32  test_pk_cycle;
    HI_U16  test_pk_size;
    HI_U8   test_pk_num;                        // 每秒发送测试报文个数
    HI_U8   avoid_detect_th;                //动态频率避让检测次数门限

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

    HI_U8 level_cnt_max[4];         // 各层级同步对象个数
    HI_U8 snr_diff_th;              // 信噪比差异门限
    HI_U8 rcv_bcn_cnt_diff_th;      // 接收信标帧数量差异门限
    HI_U8 pad;                      //本网络的频偏是否要存储到多网络频偏表中
    HI_U8 ntb_policy_enable;

    HI_U32 stat_cycle_interval;         // NTB 同步周期
}DIAG_CMD_SEL_SYNC_STA_POLICY_NV_STRU;

#ifdef PRODUCT_CFG_ONLINE_TEST_MODE
#define TMI_NUM             (16)
#define SEND_FREQ_NUM       (6)
#define SEND_TMI_NUM        (4)
#define SEND_EXT_TMI_NUM    (4)
#define INVALID_TMI         (0xff)
#define INVALID_FREQ_MODE   (0Xff)

// 0x242 TMI 信息表
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

// 0x243 挂网测试使用TMI
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

    HI_U32 send_interval_sof;    // 测试帧发送周期
    HI_U32 send_interval_bcn;    // GT信标发送周期
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
    HI_U16 stat_interval;               // 大窗口统计周期
    HI_U16 proxy_beacon_cnt_thresh;
    HI_U16 sta_beacon_cnt_thresh;
    HI_U16 check_fc_crc_err_interval;    //持续多长时间一直没有解对fc，说明频偏已经不对，需要复位NTB
    HI_U16 freq_err_preamble_th;
    HI_U16 freq_err_bcn_th;                  //NTB的频偏估计在捕获阶段生效的门限{1342,}
    HI_U32 min_beacon_period;            //最小信标间隔,单位:bts,默认为2 * 25(Mhz) * 1000 * 1000
    HI_U32 max_beacon_period;            //最大信标间隔,单位:bts,默认为160 * 25(Mhz) * 1000 * 1000
}DIAG_CMD_GT_NTB_NV_STRU;
#endif

// 无扰识别NV参数
#define TFP_LIMIT_SNID_NUM          6
#define TF_NTB_ADAPT_PARA_NUM       8
typedef struct
{
    HI_BOOL is_tf_passive_enable;                // 是否启用无扰识别
    HI_U8 period_check_durtion;                  // CCO,STA周期检查定时器周期，默认10min,单位min
    HI_U8 check_multi_net_period;                // 查询周围是否存在多网络周期，默认120，单位min
    HI_U8 check_multi_max_cnt;                   // 查询周围是否存在多网络周期个数，默认为24次，2天

    HI_U16 identify_timeout;                     // 识别超时时间，24小时，单位min,
    HI_U8 check_sta_join_period_cnt;             // CCO端周期检查STA加入网络的周期个数，默认12个，120min。
    HI_U8 check_con_collect_cco_ntb_cnt;         // CCO端检查连续收到CCO_NTB的个数，默认5次。

    HI_U32 next_turn_interval : 12;              // 启动下一路间隔，默认1小时,单位min, 60
    HI_U32 cco_notify_start_max_cnt :  6;        // cco广播启动通知次数，默认为6次,
    HI_U32 cco_notify_period : 6;                // cco广播启动通知间隔，默认5s,单位s,
    HI_U32 ntb_jitter_th : 8;                    // 滤除毛刺的绝对值门限大小，默认为150

    HI_U8 notify_neighbor_start_max_cnt;         // 广播通知邻台区次数，默认为10次
    HI_U8 notify_neighbor_period;                // 广播通知邻台区间隔，默认5s，单位s
    HI_U8 sta_notify_start_max_cnt;              // sta收到邻台区启动通知，单播通知cco,默认2次
    HI_U8 sta_notify_period;                     // sta收到邻台区启动通知间隔，默认2s,单位s

    HI_U8 notify_start_durtion;                  // cco通知开启采集时间，默认80s,单位s
    HI_U8 cco_start_query_sta_durtion;           //在启动采集后的6min后，cco开始查询全网sta信息,默认6，单位min
    HI_U8 query_parallel_send_num;               // 并发查询次数
    HI_U8 per_sta_query_cnt_max;

    HI_U8 period_check_bcn_durtion;              // 周期检查信标时间，默认10min,单位min
    HI_U8 ntb_thms;                              // 1ms,
    HI_U8 ntb_thus;                              // 30us
    HI_U8 delay_lock_durtion;                    // sta延迟锁定时间，默认3分钟，但是min

    HI_U16 parallel_query_timeout;               // 并发超时查询时间，12000ms，单位ms
    HI_U8 set_tfp_mode;                          // 测试使用，手动启动，停止无扰识别.0:不启用，1:启动，2:停止,3:采用HSO界面进行配置无扰识别
    HI_U8 identify_limit_mode;                   // 无扰识别模式，0:表示任何条件均能够触发1:只允许sta站点触发

    HI_U16 synch_ntb_cnt;                        // 一轮至少同步的ntb个数，默认10000个
    HI_U16 synch_ntb_min_cnt;                    // 一轮用于计算的至少同步的ntb个数,5000个

    HI_U16 ntb_synch_th;                         // ntb之间满足同步的门限,默认1.2ms，单位us
    HI_U16 delay_rst_durtion;                    // 延迟重启时间,2880min,2天，单位min

    HI_U16 delay_start_durtion;                  // 延迟启动无扰识别时间，单位min,默认0
    HI_U8 delay_collect_ntb_time_by_nsr;         // 延迟采集下一组ntb时间，单位min,默认30
    HI_U8 delay_collect_ntb_time_by_calc_err;    // 由于计算错误导致重新采集NTB时间，单位min,默认2

    HI_U32 report_identify_result_to_hso_enable : 1; // 允许上报识别结果到HSO
    HI_U32 report_var_to_hso_enable : 1;             // 允许上报方差计算过程到HSO
    HI_U32 report_ntb_to_hso_enable : 1;             // 允许上报ntb数据到HSO
    HI_U32 report_cmd_to_hso_enable : 1;             // 允许上报各项命令到HSO
    HI_U32 report_ntb_to_hso_message_enable : 1;     // 允许上报ntb数据到hso的message界面
    HI_U32 is_allow_multi : 1;                       // 是否允许进行多网络关联，0:不允许，1:允许,默认为1
    HI_U32 is_allow_other_sta_join : 1;              // 是否允许其他站点加入
    HI_U32 is_snid_limit : 1;                        // 是否限制只接收配置网络报文    ,默认1，启用
    HI_U32 is_check_support_tfp : 1;                 // 上电检测是否支持无扰识别
    HI_U32 is_single_phase_mode : 1;                 // 测试模式，是否为单相位模式
    HI_U32 is_period_check_enable : 1;               // CCO端是否允许周期检查机制
    HI_U32 cco_send_compress_ntb_durtion : 8;         // CCO向外发送压缩NTB周期，默认8，单位s
    HI_U32 con_not_rcv_rsp_max_cnt : 8;               // CCO端判断连续10次没有收到邻居网络CCO_NTB的回应报文，则不向该站点发送请求报文
    HI_U32 con_query_fail_cnt : 5;                   // 连续查询STA站点失败次数，默认3

    HI_U32 nsr_th : 14;                             // 噪声比门限的平方，默认为25
    HI_U32 diff_th1 : 5;                            // ntb异常滤除门限1，默认4
    HI_U32 diff_th2 : 5;                            // ntb异常滤除门限2，默认8
    HI_U32 noise_tf_coef : 8;                       // 计算大过零偏差时，判断STA,CCO扰动均值是否满足系数,100%,默认16，即0.16

    HI_U32 big_zc_diff_groupnum : 5;             // 大偏差场景下判断 最大判断间隔次数，默认10
    HI_U32 big_zc_diff_valid_groupnum : 4;       // 大偏差场景下判断有效次数，默认2
    HI_U32 big_zc_diff_th1 : 12;                 // STA与CCO的ntb_diff的值，默认500us
    HI_U32 big_zc_diff_th2 : 11;                 // STA,CCO自身平均扰动值，默认400us

    HI_U32 ntb_noise_th : 5;                     // STA_CCO的STD与CCO的平均扰动均值的差值，用来判断STA是否为大噪声，默认5,单位us
    HI_U32 ntb_stage_th : 5;                     // 异常NTB滤除地方，判断是否为台阶跳变的门限,默认4,单位us
    HI_U32 valid_ntb_cnt_th : 8;                 // 滤除NTB后，剩余ntb有效个数，默认67个
    HI_U32 stage_ntb_diff_th : 6;                // 判断是否为台阶跳变，计算前后ntb_diff使用个数，默认10
    HI_U32 filter_err_ntb_th : 6;                // 在毛刺处进行滤除ntb_diff个数，默认5
    HI_U32 is_enable_max_noise : 1;                // 是否启用nsr功能，默认1
    HI_U32 pad0 : 1;

    HI_U32 snid_limit_rcv[TFP_LIMIT_SNID_NUM];   // 测试限制只接收配置的网络报文

    HI_U16 req_result_min_interval;             // 请求识别结果最小间隔，单位min
    HI_U16 is_all_sta_transmit      : 1;        // 是否所有STA都转发NTB数据
    HI_U16 cco_collect_ntb_test     : 1;        // CCO端采集NTB测试
    HI_U16 is_single_phase_send     : 1;        // 是否单相发送: 一帧报文里面只带一个相位的数据
    HI_U16 is_send_fall_edge        : 1;        // 是否发送下降沿NTB
    HI_U16 no_sta_join_time_th      : 8;        // 上电多长时间无站点加入，允许发送NTB，单位min
    HI_U16 is_send_ntb_enable       : 1;        // 是否发送NTB报文
    HI_U16 rise_fall_ratio          : 3;        // 发送上升下降沿的比例

    HI_U8  send_period_default;                 // 高精度过零NTB默认发送周期，单位min
    HI_U8  send_period_slow;                    // 高精度过零NTB低速发送周期，单位min
    HI_U8  send_interval;                       // 报文发送间隔，单位s
    HI_U8  send_round_max;                      // 最大发送轮数

    HI_U8  match_succ_cnt_th;                   // 异常场景判结果需要满足的包数门限，修改为5
    HI_U8  send_ntb_diff_num;                   // 发送过零NTB差值个数
    HI_U16 result_net_active_time_th    : 10;   // 最终结果活跃时间门限，单位min
    HI_U16 miss_ntb_pkt_num_th          : 6;    // 匹配失败多少次，可认为未收到该网络NTB

    HI_U8  rcv_ntb_duration;                    // 此时间内收到过NTB数据，算正在识别，单位min
    HI_U8  lock_duration;                       // 锁定归属网络的时间，单位min
    HI_U8  period_diff_th       : 4;            // 工频周期差异门限，单位us
    HI_U8  valid_ntb_num_ratio  : 4;            // 一包中，有效NTB个数比值达到门限，才算有效，单位0.1，修改为6
    HI_U8  save_his_duration;                   // 保存历史记录的间隔，单位min

    HI_U16 sta_cache_ntb_cnt;                   // STA端缓存NTB个数
    HI_U16 variance_ratio_th1;                   // 方差比值1，比值为1.5，按照100计算，为150

    HI_U8  adapt_pkt_cnt[TF_NTB_ADAPT_PARA_NUM];// 自适应参数，接收NTB包数门限
    HI_U16 adapt_ratio[TF_NTB_ADAPT_PARA_NUM];  // 自适应参数，对应包数下的比值门限，单位0.01

    HI_U32 para_idx_max             : 3;        // 判决门限中，SNR关闭时，自适应参数使用最大下标
    HI_U32 para_idx_max_snr         : 3;        // 判决门限中，SNR打开时，自适应参数使用最大下标
    HI_U32 override_para_idx        : 3;        // 推翻门限中，SNR关闭时，自适应参数使用最大下标
    HI_U32 override_para_idx_snr    : 3;        // 推翻门限中，SNR打开时，自适应参数使用最大下标
    HI_U32 cache_valid_ratio        : 4;        // 缓存记录中有效的个数，单位0.1
    HI_U32 big_noise_net_ratio      : 4;        // 大噪声网络的比例门限，单位0.1
    HI_U32 big_noise_cache_ratio    : 4;        // 大噪声缓存记录的比例门限，单位0.1
    HI_U32 restart_cycle_len        : 8;        // 两次接收数据超过此门限，重新开始识别流程
}DIAG_CMD_CONFIG_TF_PASSIVE_NV_STRU;

#define MULTI_FREQ_NV_CNT_MAX 		(4)
typedef struct
{
    HI_U8 multi_freq[MULTI_FREQ_NV_CNT_MAX];           // 多频探测顺序

    HI_U8 nw_detect_cnt_max;
	HI_U8 nw_detect_interval;
    HI_U8 sta_detect_cnt_max;

	HI_U8 sta_detect_interval;

}DIAG_CMD_MULTI_FREQ_PARA_NV_STRU;

//停电事件统计nv
typedef struct
{
    HI_U8 plug_module;                       //插拔过模块
    HI_U8 plug_module_read_fail;            //读取gpio26管脚失败
    HI_U8 module_power_failure_cnt;         //模块停电次数
    HI_U8 power_failure_recover;            //停电恢复原因

    HI_U8 power_failure_fail_report_cnt;    //停电事件未能上报次数
    HI_BOOL offen_power_failure;            //多次发生停电事件
    HI_BOOL is_first_power_failure;        //是第一次停电
    HI_BOOL is_first_power_failure_recover; //第一次停电恢复

    HI_U8 first_power_failure_recover;      //初次停电恢复原因
    HI_U8 pad1;
    HI_U16 reset_offten_power_failure;       //重置经常停电标志位时间，单位min

    HI_U32 first_poower_failure_sys_time;   //初次停电时间

    HI_U32 first_power_failure_recover_time;//初次停电恢复时间

    HI_U32 last_power_failure_sys_time;     //最后一次停电时间

    HI_U32 last_power_failure_recover_time; //最后一次停电恢复时间

}DIAG_CMD_POWER_FAILURE_STATISTICS_NV_STRU;

//非电容模块停电nv设置
#define POWEROFF_ADAPT_PARA_CNT     (6) // 停电检测自适应参数条数

// 停电检测自适应参数条目结构
typedef struct
{
    HI_U16 station_cnt;                 // 站点个数
    HI_U16 poweroff_check_cycle_len;    // 停电检测周期时长
    HI_U16 heartbeat_duration_th;       // 未收到心跳时长门限
    HI_U16 pad;
} DIAG_CMD_POWEROFF_CHECK_ADAPTIVE_PARA_STRU;

// 停电检测参数NV项结构
typedef struct
{
    HI_U16 network_station_num;         // 网络规模数，0:表示根据网络实际站点数动态选取参数
    HI_U16 adaptive_redundance_para;    // 周期自适应参数冗余量，防止周期参数动荡

    HI_U16 arrange_cycle_len;           // 离线事件统计量刷新周期时长，单位min
    HI_U8  channel_leave_cnt_th;        // 信道原因离线次数门限
    HI_U8  pad;

    DIAG_CMD_POWEROFF_CHECK_ADAPTIVE_PARA_STRU adaptive_para[POWEROFF_ADAPT_PARA_CNT]; // 停电检测自适应参数

    HI_U8  protect_round_num;       // 站点上线后，连续这么多个周期在线，才认为在线
    HI_U8  pad2;
    HI_U16 sta_delay_max_time;      // 站点帮助应答时，延迟最大时间，单位s

    HI_U8  eq_heartbeat_cnt_th;     // 站点收到这么多个等效心跳后，放弃本次帮助应答
    HI_U8  set_eq_heartbeat_cnt;    // 延迟中有新需要上报的TEI时，将已收到等效心跳设为此值
    HI_U8  sta_transmit_enable;     // 叶子节点转发心跳使能
    HI_U8  sta_transmit_delay;      // 叶子节点转发心跳延迟时长，单位s

    HI_BOOL is_power_off_enable;    //停电mac相关部分使能开关
    HI_U8  pad1[3];
} DIAG_CMD_CONFIG_POWEROFF_CHECK_PARA_STRU;



#ifdef __cplusplus
}
#endif

#endif /*MAC_NV_H*/
