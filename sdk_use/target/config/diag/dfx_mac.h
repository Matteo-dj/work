/*************************************************
  Copyright:   Huawei Technologies Co., Ltd. 1998-2011
  File name: dfx_mac.h
  Author:  dkf35509
  Description:  可维可测接口
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
 *  定义相关条目的最大限值
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


#define PLC_NDM_IP_MAC_MAP_NUM_MAX          (4)     //以IP方式与抄控器连接时，站点端的IP与MAC的映射表的最大成员数

#define MAC_DIAG_CMD_QUERY_NETWORK_TOPO_ENTRY_NUM           (8)     //NDA通道中拓扑上报结构中包含的节点数
#define DFX_NTB_SYNC_STA_CNT_MAX         (8)
#define DFX_NTB_EXCEPTION_CNT_MAX        (8)
#define DFX_NTB_SINGU_FREQERR_CNT_MAX    (8)
#define DFX_PATH_TABLE_BITMAP_LEN        (128)                     //路径信息表bitmap大小
#define DFX_PATH_TABLE_BIT_SIZE          (8)                     //路径信息表比特位大小
#define PHS_PER_TO_NUM                  (100)

//精简拓扑中芯片的编号
typedef enum
{
    CHIP_VER_IDX_HI3911 = 0x01,
    CHIP_VER_IDX_MAX = 0x03,
    CHIP_VER_IDX_INVALID = 0xff
}CHIP_VER_IDX_ENUM;

#define MAX_NEIGHBOR_SUM            (15)          // 邻居网络最大个数,不包括自身网络
#define QUERY_MAX_NEIGHBOR_SUM      (30)         // 查询接口中涉及邻居网络最大个数,不包括自身网络


/**
 * 上报到HSO的类别
 **/
#define TYPE_VERSION unsigned char
#define TYPE_DEBUG_RELEASE      (0x02)     // 表示HSO的研发调测版本和现场维护版共存
#define TYPE_DEBUG              (0x00)     // 表示HSO的研发调测版本
#define TYPE_RELEASE            (0x01)     // 表示 HSO的现场维护版本

/************************************************************************/
/*      层间消息ID    （0x1000~0x5000）                                 */
/************************************************************************/

/********************************************************************/

/************************************************************************/
/*      通用业务查询类 命令ID    [0X1000 ~ 0X1BFF]                           */
/***********START*************************************************************/
typedef HI_U16 ID_DIAG_QUERY;
#define ID_DIAG_CMD_QUERY_ID_SEG_START                (0x1000)
#define ID_DIAG_CMD_QUERY_ID_SEG_END                  (0x1BFF)

#define ID_DIAG_CMD_QUERY_NETWORK_CONVERGENCE_TIME    (0x1000)       // 查询站点自组网耗时
#define ID_DIAG_CMD_QUERY_NETWORK_TOPO                (0x1001)       // 查询网络拓扑表
#define ID_DIAG_CMD_QUERY_ROUTER_TABLE                (0x1002)       // 查询路由表信息
#define ID_DIAG_CMD_QUERY_TX_STATICS                  (0X1003)       // 查询发送机统计量
#define ID_DIAG_CMD_QUERY_RX_STATICS                  (0X1004)       // 查询接收机统计量
#define ID_DIAG_CMD_QUERY_RSB_STATICS_PK              (0X1005)       // 查询重组模块统计量
#define ID_DIAG_CMD_TF_COLLECT_DATA_ZERO_CROSS_NTB    (0x1006)       // 抄控器采集过零NTB
#define ID_DIAG_CMD_QUERY_ALL_STA_INFO                (0X1007)       // 查询站点代理变更和离线次数
#define ID_DIAG_CMD_QUERY_DEVICE_FREQ_INFO            (0X1008)       // 查询站点频段信息
#define ID_DIAG_CMD_QUERY_NETWORK_STATE_INFO          (0X1009)       // 查询CCO端组网信息或者STA短关联信息
#define ID_DIAG_CMD_QUERY_NEIGHBOR_NETWORK            (0x100A)       // 查询邻居网络信息
#define ID_DIAG_CMD_QUERY_NETWORK_ATTRIBUTE           (0x100B)       // 查询站点网络属性
#define ID_DIAG_CMD_QUERY_REQUEST_JOIN_FAILED_COUNT   (0x100C)       // 查询站点关联失败次数
#define ID_DIAG_CMD_QUERY_JOIN_NETWORK_EXPIRED        (0x100D)       // 查询站点关联耗时
#define ID_DIAG_CMD_QUERY_ASSOCIATED_STAS             (0x100E)       // 查询站点关联对象列表
#define ID_DIAG_CMD_QUERY_DISCOVER_LIST               (0x100F)       // 查询站点发现列表
//#define ID_DIAG_CMD_QUERY_NTB_STAT                    (0x1010)       // 查询NTB模块统计量
#define ID_DIAG_CMD_QUERY_PHS_STAT                    (0x1011)       // 查询相位识别模块统计量
#define ID_DIAG_CMD_QUERY_NDM_CONNECT_STAT            (0x1012)       // 查询抄控器连接信息(已停用)
#define ID_DIAG_CMD_QUERY_VIOLENCE_STA                (0x1013)       // 查询CCO端拒绝列表(已不在使用)
#define ID_DIAG_CMD_QUERY_SEG_STATICS_PK              (0x1014)       // 查询分段模块统计量
#define ID_DIAG_CMD_QUERY_NDM_STATICS                 (0x1015)       // 查询抄控器模块统计量
#define ID_DIAG_CMD_QUERY_DISCOVER_LIST_SM            (0x1016)       // 精简的发现列表
#define ID_DIAG_CMD_QUERY_MAC_SYS_ERR                 (0x1017)       // MAC的系统错误

#define ID_DIAG_CMD_QUERY_FREQUENCY_STATUS            (0x101b)       // 是否接收到全网频段变更帧
#define ID_DIAG_CMD_QUERY_MULTI_NETWORK_RCV_INFO      (0x101d)       // 查询多网络下接收信标帧和SOF帧FC个数
#define ID_DIAG_CMD_CCO_MAC_MULTYNET_QUERY            (0x101e)       // 查询SNID(STA和CCO行为不同点)
#define ID_DIAG_CMD_CCO_MAC_MULTYNET_SET              (0x101f)       // 多网络设置命令
#define ID_DIAG_CMD_MAC_FREQ_SET                      (0x1020)       // 设置频段,切频时间,如果在切频状态STA和CCO不同
#define ID_DIAG_CMD_MAC_FREQ_QUERY                    (0x1021)       // 查询频段，是否正在切，切的进度还剩多长时间 STA和CCO不同
#define ID_DIAG_CMD_CCO_MAC_WHITE_STA_QUERY           (0x1022)       // 白名单是否开启的状态查询，是否把表档案做在白名单
#define ID_DIAG_CMD_CCO_MAC_WHITE_STA_SET             (0x1023)       // 白名单设置
#define ID_DIAG_CMD_CCO_MAC_CLR                       (0x1024)       // 清除拒绝列表
#define ID_DIAG_CMD_QUERY_CVG_STATICS_PK              (0x1025)       // 汇聚分发报文统计
#define ID_DIAG_CMD_QUERY_LEAVE_IND_STAT_INFO         (0x1026)       // 统计离线指示报文
#define ID_DIAG_CMD_SOFT_RESET                        (0x1027)       // 查询站点软重启信息
#define ID_DIAG_CMD_PROXY_NUM_QUERY                   (0x1028)       // 查询当前的代理数
#define ID_DIAG_CMD_CHILD_SUM_QUERY                   (0x1029)       // 查询某站点的子站点数
#define ID_DIAG_CMD_NM_PACKET_STAT_QUERY              (0x102a)       // 网管报文个数统计
#define ID_DIAG_CMD_NM_ASSOC_REQ_STAT_QUERY           (0x102b)       // 关联请求报文统计
#define ID_DIAG_CMD_NM_ASSOC_CNF_STAT_QUERY           (0x102c)       // 关联回复报文统计
#define ID_DIAG_CMD_STA_SNID_INFO                     (0x102d)       // STA选择SNID状态
#define ID_DIAG_CMD_STA_PROXY_CHANGE_INFO             (0x102e)       // STA获取代理变更可维可测信息
#define ID_DIAG_CMD_ASSOC_INFO                        (0x102f)       // 获取网络管理关联与代理请求处理的可维可测信息
#define ID_DIAG_CMD_NDM_CONNECT                       (0x1030)       // 设置抄控器连接参数
#define ID_DIAG_CMD_QUERY_FC_PB_FAULT_INFO            (0x1031)       // 获取fc和pb错位统计
#define ID_DIAG_CMD_NDC_TEST                          (0x1032)       // NDC通道测试启动命令
#if defined(PRODUCT_CFG_PRODUCT_TYPE_NDM) || defined(PRODUCT_CFG_HSO)
#define ID_DIAG_CMD_GET_NDM_CTRL_ATTENUATE            (0x1033)       // 查询抄控器衰减模式
#define ID_DIAG_CMD_SET_NDM_CTRL_ATTENUATE            (0x1034)       // 设置抄控器衰减模式
#endif
#define ID_DIAG_CMD_NDM_IP_MAC_MAP_INFO               (0x1035)       // 抄控器的IP和MAC映射表
#define ID_DIAG_CMD_NDM_ATTR_SET                      (0x1036)       // 设置抄控器的属性
#define ID_DIAG_CMD_LED_STATUS_INFO                   (0x1037)       // 查询LED指示灯状态
#define ID_DIAG_CMD_QUERY_RX_DIAGNOSTIC               (0x1038)       // 查询接收机诊断统计量
#define ID_DIAG_CMD_QUERY_TX_DIAGNOSTIC               (0x1039)       // 查询发送机诊断统计量
#define ID_DIAG_CMD_QUERY_PLC_DEV_STAT_INFO           (0x103a)       // 查询PLC IP模块统计量
#define ID_DIAG_CMD_MULTI_EXCEPTION_STATUS_INFO       (0x103b)       // 查询多网络异常状态信息
#define ID_DIAG_CMD_QUERY_COMM_RATE_INFO              (0x1040)       // 查询上报通信率信息
#define ID_DIAG_CMD_QUERY_CHOSEN_STA_DISLIST          (0x1041)       // 查询指定站点的发送列表
#define ID_DIAG_CMD_QUERY_CHOSEN_STA_TOPOINFO         (0x1042)       // 查询指定站点的拓扑信息
#define ID_DIAG_CMD_QUERY_VERSION_INFO                (0x1043)       // 查询站点的版本信息和灌包NV配置信息
#define ID_DIAG_CMD_QUERY_BACKUP_MAC                  (0x1045)       // 查询备份MAC地址
#define ID_DIAG_CMD_SET_BACKUP_MAC                    (0x1046)       // 设置备份MAC地址
#define ID_DIAG_CMD_NDM_FOLLOW_STAT_STRU              (0x1047)       // 查询抄控器巡检采数跟随信息
#define ID_DIAG_CMD_SET_NDM_FOLLOW                    (0x1048)       // 设置抄控器采数跟随功能
#define ID_DIAG_CMD_QUERY_MAX_VIOLENCE_STA            (0x1049)       // 设置查询拒绝列表个数为200的信息
#define ID_DIAG_CMD_MAC_GET_WHITELIST                 (0x104a)       // 查询白名单
#define ID_DIAG_CMD_MAC_REPORT_WHITELIST_CHANGED      (0x104b)       // 上报白名单变更
#define ID_DIAG_CMD_QUERY_TX_SEND_IP_TIMEOUT_STAT     (0x104d)       // 设置查询IP报文发送超时信息
#define ID_DIAG_CMD_QUERY_PLC_SWITCH_TRX_STAT         (0x104e)       // 查询交换模块收发统计量
#define ID_DIAG_CMD_QUERY_PLC_SWITCH_MAPPING_TABLE    (0x104f)       // 查询交换模块映射表信息
#define ID_DIAG_CMD_QUERY_PLC_SWITCH_FRAGMENT_QUEUE   (0x1050)       // 查询交换模块分片队列信息
#define ID_DIAG_CMD_QUERY_PLC_SWITCH_PROTOCOL_STAT    (0x1051)       // 查询交换模块不同协议报文统计量
#define ID_DIAG_CMD_SET_FAULT_LOCAT                   (0x1052)       // HSO下发启动、停止抄控器故障定位命令
#define ID_DIAG_CMD_QUERY_FAULT_LOCAT_STATUS          (0x1053)       // HSO下发查询抄控器故障定位状态命令
#define ID_DIAG_CMD_QUERY_FAULT_LOCAT_STAT            (0x1054)       // 查询故障定位统计信息
#define ID_DIAG_CMD_QUERY_RF_TX_DIAGNOSTIC            (0x1055)       // 查询RF设备TX模块诊断计数
#define ID_DIAG_CMD_QUERY_RF_RX_DIAGNOSTIC            (0x1056)       // 查询RF设备RX模块诊断计数
//#define ID_DIAG_CMD_QUERY_RF_SNID_CHANGE_INFO         (0x1057)       // 查询RF设备RX模块SNID冲突改变信息
#define ID_DIAG_CMD_QUERY_LOCK_NETWORK_STATUS         (0x1058)        // 查询锁定网络状态
#define ID_DIAG_CMD_NETWORK_MODE_SWITCH_REQ           (0x1059)       // 设置网络模式切换
#define ID_DIAG_CMD_UNLOCK_NETWORK_STATUS             (0x105a)       // 解锁网络锁定

#define ID_DIAG_CMD_QUERY_DUAL_MODULE_INFO            (0x1060)       // 查询网络中双模站点信息
#define ID_DIAG_CMD_QUERY_STA_INFO                    (0x1061)       // 查询站点信息(为101b查询命令的补充)
#define ID_DIAG_CMD_QUERY_CONNECTED_GRAPH_INFO        (0x1062)       // 查询站点连通信息
#define ID_DIAG_CMD_QUERY_RF_CURRENT_COMM_RATE        (0x1063)       // 查询RF设备查询当前通信率
#define ID_DIAG_CMD_QUERY_RF_MODE_CHANGE_COMM_RATE    (0x1064)       // 查询RF设备查询模式切换时通信率

#define ID_DIAG_CMD_TF_IDENTIFY_SET_REQ               (0x1065)       // HSO控制台区识别
#define ID_DIAG_CMD_TF_IDENTIFY_STA_CONFIG_REQ        (0x1066)       // HSO设置识别范围
#define ID_DIAG_CMD_TF_IDENTIFY_RESULT_REQ            (0x1067)       // HSO查询台区识别结果
#define ID_DIAG_CMD_BLACKLIST_REQ                     (0x1068)       // HSO查询台区识别黑名单
#define ID_DIAG_CMD_BLACKLIST_CLEAR_REQ               (0x1069)       // HSO清除台区识别黑名单
#define ID_DIAG_CMD_TF_CCO_STATIC                     (0x106a)       // 台区识别CCO端的统计量
#define ID_DIAG_CMD_TF_STA_STATIC                     (0x106b)       // 台区识别STA端的统计量
#define ID_DIAG_CMD_TF_STATIC                         (0x106c)       // 台区识别公共统计量
#define ID_DIAG_CMD_QUERY_TF_IDENTIFY_INFO            (0x106d)       // 查询台区识别统计信息
#define ID_DIAG_CMD_QUERY_TF_IDENTIFY_STA_RANGE       (0x106e)       // 查询台区识别范围
#define ID_DIAG_CMD_TF_LOCK_STA_OM                    (0x106f)       // 锁定或解锁STA端的可维可测信息
#define ID_DIAG_CMD_QUERY_HIS_RESULT_CACHE_INFO       (0x1070)       // 查询历史台区识别结果缓存
#define ID_DIAG_CMD_QUERY_CUR_RESULT_CACHE_INFO       (0x1071)       // 查询当前台区识别结果缓存
#define ID_DIAG_CMD_TF_CCO_FL_STATIC                  (0x1073)       // CCO端的统计量
#define ID_DIAG_CMD_QUERY_TF_NTB_STATIC               (0x1074)       // CCO端的统计量
#define ID_DIAG_CMD_QUERY_NDM2NDM_TF_STATUS_REQ       (0x1075)       // HSO查询ndm台区识别状态和结果
#define ID_DIAG_CMD_NDM2NDM_TF_SET_REQ                (0x1076)       // HSO设置ndm台区识别
#define ID_DIAG_CMD_QUERY_PHASE_IDENTIFY_RESULT_REQ   (0X1077)       // HSO查询相位识别结果
#define ID_DIAG_CMD_PI_CCO_STATIC                     (0x1078)       // 相位识别CCO端的统计量
#define ID_DIAG_CMD_PI_CCO_BASE_NTB                   (0x1079)       // 相位识别CCO端基准NTB数据上报
#define ID_DIAG_CMD_ASSOC_DFX_INFO                    (0x107A)       // 站点加入网络可维可测
#define ID_DIAG_CMD_MULTI_NETWORK_ASSOC_DFX_INFO      (0x107B)       // 多网络关联可维可测
#define ID_DIAG_CMD_SERIAL_NUM_CHANGE_DFX_INFO        (0x107C)       // 组网序列号重启可维可测
#define ID_DIAG_CMD_TF_IDENTIFY_STATE                 (0x107d)       //台区识别状态查询
#define ID_DIAG_CMD_QUERY_CONNECT_NDM_INFO            (0x107e)       // CCO连接NDM的统计信息
#define ID_DIAG_CMD_QUERY_WOBBLE_WITHOUT_SIGNAL       (0x107f)       // 无台区识别序列发送时的扰动统计信息
#define ID_DIAG_CMD_QUERY_WOBBLE_WITH_SIGNAL          (0x1080)       // 有台区识别序列发送时的扰动统计信息
#define ID_DIAG_CMD_TX_DUTY_STATIC                    (0x1081)       // 查询发送占空比信息
#define ID_DIAG_CMD_QUERY_FL_TOPO_REFERENCE           (0x1082)
#define ID_DIAG_CMD_QUERY_FL_TOPO_RECORD              (0x1083)
#define ID_DIAG_CMD_CLEAR_FL_INFO                     (0x1084)
#define ID_DIAG_CMD_QUERY_FL_STATICS                  (0X1085)       // 查询关键节点诊断统计量
#define ID_DIAG_CMD_QUERY_RF_ASSOC_INFO               (0x1086)       // 查询双模RF关联请求信息
#define ID_DIAG_CMD_TF_EXACT_FALSE_INFO               (0x1087)       // STA精同步过程虚警信息
#define ID_DIAG_CMD_SET_LISTEN_TIME_BEFORE_FIRST_BEA  (0x1088)       // CCO端设置多网络监听时间
#define ID_DIAG_CMD_RECORD_SWITCH_FREQ_INFO           (0x1089)       // STA，NDM端记录收到的切换频段通知信息
#define ID_DIAG_CMD_COORDINATE_CONFLICT_INFO          (0x1090)       // STA多网络冲突信息
#define ID_DIAG_CMD_COORDINATE_MULTI_CCO_INFO_PLC     (0x1091)       // STA同一网络号下多个CCO信息
//#define ID_DIAG_CMD_COORDINATE_MULTI_CCO_INFO_RF      (0x1092)       // RF链路上STA同一网络号下多个CCO信息
#define ID_DIAG_CMD_QUERY_TF_NOISE_CACHE_INFO         (0x1093)       // STA端统计台区识别噪声缓存信息
#define ID_DIAG_CMD_TF_CROSSTALK_RESULT_REQ           (0x1094)       // HSO查询台区识别结果中非本台区站点信息
#define ID_DIAG_CMD_PTPC_TEST                         (0x1096)
#define ID_DIAG_CMD_PTPC_STATE_INFO                   (0x1097)
#define ID_DIAG_CMD_PTPC_COORDINATE_INFO              (0x1098)
#define ID_DIAG_CMD_PTPC_STAT_INFO                    (0x1099)
#define ID_DIAG_CMD_PTPC_STATE_RECORD_INFO            (0x109a)
//#define ID_DIAG_CMD_NTB_SOFT_INFO                      (0x109b)

//#define ID_DIAG_CMD_TF_STATICS_STAT                   (0x109c)       // 查询台区识别流程统计量
#define ID_DIAG_CMD_TF_CHIP_STAT                      (0x109d)       // 查询芯片端台区识别统计量
#define ID_DIAG_CMD_QUERY_NTB_EXCEPTION               (0x109e)
#define ID_DIAG_CMD_NTB_NEIGHBOR_FREQERR              (0X10a2)

#ifdef PRODUCT_CFG_ONLINE_TEST_MODE
#define ID_DIAG_CMD_QUERY_GT_TX_STATISTIC_INFO        (0x10a3)
#define ID_DIAG_CMD_QUERY_GT_RX_STATISTIC_INFO        (0x10a4)
#define ID_DIAG_CMD_QUERY_GT_DFX_STATISTIC_INFO       (0x10a5)
#define ID_DIAG_CMD_QUERY_GT_NTB_STATISTIC_INFO       (0x10a6)
#endif

#define ID_DIAG_CMD_QUERY_GT_NTB_RESET_INFO           (0x10a7)
#define ID_DIAG_CMD_EMC_POWERLOADING_CONFIG           (0X10A8)       // EMC配置POWERLOADING
#define ID_DIAG_CMD_TX_SEND_QUEUE_INFO                (0x10aa)
#define ID_DIAG_CMD_SLOT_UPDATE_INFO                  (0x10ab)
#define ID_DIAG_CMD_QUERY_NTB_STAT_V200               (0x10ac)       // 查询NTB模块统计量
#define ID_DIAG_CMD_QUERY_NTB_FREQERR_STAT_V200       (0x10ad)       // 查询NTB 频偏统计

#define ID_DIAG_CMD_SEND_SC_FRAME_STUB                (0x10ae)       // 发送窄带帧测试命令
#define ID_DIAG_CMD_QUERY_SC_NM_STATISTIC             (0x10af)       // 单载波网管层统计量查询
#define ID_DIAG_CMD_QUERY_SC_TX_STATISTIC             (0x10b0)       // 单载波发送机统计量查询
#define ID_DIAG_CMD_QUERY_SC_RX_STATISTIC             (0x10b1)
#define ID_DIAG_CMD_TF_PASSIVE_STATIC                 (0x10b2)       // 无扰识别统计量
#define ID_DIAG_CMD_TFP_REPORT_NTB                    (0x10b4)       // 上报NTB到HSO界面.
#define ID_DIAG_CMD_QUERY_MF_UPPER_BRIDGING_STATISTIC (0x10b5)       //
#define ID_DIAG_CMD_QUERY_MF_DETECT_NETWORK_STATISTIC (0x10b6)       //
#define ID_DIAG_CMD_QUERY_MF_DETECT_STATION_STATISTIC (0x10b7)
#define ID_DIAG_CMD_QUERY_MULTI_FREQUENCY             (0x10b8)
#define ID_DIAG_CMD_QUERY_RT_REPAIR_STAT              (0x10b9)       // 实时路由修复模块统计量
#define ID_DIAG_CMD_QUERY_PTCL_DETECT_STAT            (0x10ba)       // 协议探测统计量
#define ID_DIAG_CMD_QUERY_NETWORK_ATTRIBUTE_V200      (0x10bb)       // 查询站点的网络属性(V200版本)
#define ID_DIAG_CMD_QUERY_TFP_NSR_NTB_FILTER_STATIC   (0x10bc)       // 统计无扰识别大噪声，ntb异常滤除统计量
#define ID_DIAG_CMD_TFP_IDENTIFY_RESULT_STATIC        (0x10bd)       // 统计无扰识别结果
#define ID_DIAG_CMD_MULTI_NETWORK_RCV_INFO_V200       (0x10be)       // V200多网络报文接收查询
#define ID_DIAG_CMD_QUERY_NTB_STATIC_V200             (0x10bf)       // v200 NTB统计量查询
#define ID_DIAG_CMD_SET_TRANSMIT_POWER_V200           (0x10C0)       // 设置发射功率
#define ID_DIAG_CMD_DELAYED_RESTART_V200              (0x10C1)       // 全网延迟重启
#define ID_DIAG_CMD_COMMON_TEMP_COMMAND               (0x10C2)       // 通用临时命令
#define ID_DIAG_CMD_REPORT_PB_STAT                    (0x10C3)       // PB块上报

#define ID_DIAG_CMD_GET_TOP_NEIGHBOR                  (0x10c4)      // 查询站点最好的邻居
#define ID_DIAG_CMD_STA_COLLECT_RESULT_DETAIL         (0x10c5)      // 查询SNR统计结果详细数据
#define ID_DIAG_CMD_STA_COLLECT_RESULT_SIMPLE         (0x10c6)      // 查询SNR统计结果简要数据
#define ID_DIAG_CMD_STA_LOCK_NETWORK                  (0x10c7)      // 设置STA锁定网络
#define ID_DIAG_CMD_STA_SNR_HISTORY_NODE              (0x10c8)      // 查询单次历史记录
#define ID_DIAG_CMD_STA_SNR_NET_HISTORY               (0x10c9)      // 查询单网络48次历史记录
#define ID_DIAG_CMD_CCO_SET_NODE_FIND_PARA            (0x10ca)      // CCO设置节点查找参数
#define ID_DIAG_CMD_STA_FAST_CHANGE_STAT              (0x10cb)      // 查询站点快速收敛记录
#define ID_DIAG_CMD_STA_STABLE_CHANGE_STAT            (0x10cc)      // 查询站点长期评估切换网络记录
#define ID_DIAG_CMD_STA_SNR_CTRL_STAT                 (0x10cd)      // 查询SNR策略控制状态
#define ID_DIAG_CMD_CCO_SNR_TF_STAT                   (0x10ce)      // 查询台区识别状态
#define ID_DIAG_CMD_QUERY_LOCK_INFO_EXP               (0x10cf)      // 查询锁定信息
#define ID_DIAG_CMD_QUERY_POWER_FAILURE_INFO          (0x10d0)      // 查询停电上报事件信息
#define ID_DIAG_CMD_REPORT_PB72_STAT                  (0x10D1)      // 72PB块上报
#define ID_DIAG_CMD_REPORT_PB136_STAT                 (0x10D2)      // 136PB块上报
#define ID_DIAG_CMD_REPORT_PB264_STAT                 (0x10D3)      // 264PB块上报
#define ID_DIAG_CMD_QUERY_NEAR_STA_DATA               (0x10d4)      // 查询站点采集的附近站点信息
#define ID_DIAG_CMD_QUERY_NEAR_NET_INFO               (0x10d5)      // 查询站点采集的附近网络信息
#define ID_DIAG_CMD_SET_BELONG_NETWORK                (0x10d6)      // 设置站点的归属网络
#define ID_DIAG_CMD_CLEAR_BELONG_NETWORK              (0x10d7)      // 清除站点的归属网络
#define ID_DIAG_CMD_SET_SWITCH_FREQ                   (0x10D8)      // 全网切换频段
#define ID_DIAG_CMD_POWER_CTRL_STAT                   (0x10D9)      // 查询功率控制统计量
#define ID_DIAG_CMD_GET_BELONG_CHANGE_REOCRDS         (0x10da)      // 获取归属变更记录
#define ID_DIAG_CMD_GET_LOCK_REOCRDS                  (0x10db)      // 获取锁定记录
#define ID_DIAG_CMD_SET_TRANSMIT_POWER_BY_FREQ        (0x10dc)      //延时设置各频段数字发射功率
#define ID_DIAG_CMD_QUERY_ROUTE_PATH_REFERENCE        (0x10dd)      //查询路径信息表使用情况
#define ID_DIAG_CMD_QUERY_PK_FILTER                   (0x10de)      //查询报文过滤情况
#define ID_DIAG_CMD_QUERY_DIRECT_SEND_INFO            (0x10df)      //查询直接发送信息
#define ID_DIAG_CMD_FAST_NOTICE_INFO                  (0x10e0)
#define ID_DIAG_CMD_QUERY_TFP_TRX_STAT                (0x10e1)      // 查询无扰收发统计信息
#define ID_DIAG_CMD_TFP_IDENTIFY_SET_REQ              (0x10e2)      // HSO下发启动无扰识别命令
#define ID_DIAG_CMD_TFP_IDENTIFY_QUERY_RESULT         (0x10e3)      // HSO下发查询无扰识别结果命令
#define ID_DIAG_CMD_QUERY_ENERGY_STATICS              (0x10e4)      // 能源应用维测信息命令
#define ID_DIAG_CMD_TFP_IDENTIFY_CLEAR_RESULT         (0x10e5)      // HSO下发清除无扰识别结果命令

//停电模块维测信息查看
#define ID_DIAG_CMD_CCO_GET_POWEROFF_INFO             (0x10e6)      // 获取停电检测上报信息
#define ID_DIAG_CMD_CCO_GET_STA_POWEROFF_INFO         (0x10e7)      // 获取指定站点停电检测上报信息
#define ID_DIAG_CMD_CCO_GET_POWEROFF_HISTORY          (0x10e8)      // 查询STA帮助邻居应答探测的当前信息
#define ID_DIAG_CMD_STA_GET_CUR_HELP_REPORT_INFO      (0x10e9)      // STA端STA帮助邻居应答探测的历史信息
#define ID_DIAG_CMD_STA_GET_HB_TRANS_DELAY_INFO       (0x10ea)      // 查询STA叶子节点心跳延迟转发信息
#define ID_DIAG_CMD_STA_GET_HIS_HELP_REPORT_INFO      (0x10eb)      // 获取站点停电上报过程历史记录
#define ID_DIAG_CMD_NDM_GET_CONNECT_DFX_INFO          (0x10ec)      // 获取ndm站点连接信息
#define ID_DIAG_CMD_NDM_GET_DTECTID_DFX_INFO          (0x10ed)      // 获取ndm站点探测信息


#define ID_DIAG_CMD_QUERY_ROUTE_PROXY_INFO            (0x10f0)      // 查询高频采集相关路由及代理变更信息

#define ID_DIAG_CMD_MAC_PHYSICAL_TEST_MODE            (0x1A01)

#define ID_DIAG_CMD_MAC_SET_PLC_PROTOCOL              (0x1A06)      // 设置PLC协议版本
#define ID_DIAG_CMD_MAC_QUERY_PLC_PROTOCOL            (0x1A07)      // 查询PLC协议版本

// 无扰NTB-本地识别组件维测 0x1a80 ~ 0x1a89
#define ID_DIAG_CMD_TF_ND_MIN                         (0x1a80)
#define ID_DIAG_CMD_TF_ND_STA_GET_CUR_INFO            (0x1a80)      // STA: 查询当前识别信息
#define ID_DIAG_CMD_TF_ND_STA_GET_HISTORY             (0x1a81)      // STA: 查询历史记录
#define ID_DIAG_CMD_TF_ND_STA_GET_STATISTIC           (0x1a82)      // STA: 查询状态记录
#define ID_DIAG_CMD_TF_ND_STA_GET_NET_HIS             (0x1a83)      // STA: 查询单网络的历史记录
#define ID_DIAG_CMD_TF_ND_STA_CLEAR_RESULT            (0x1a85)      // STA: 清除识别结果
#define ID_DIAG_CMD_TF_ND_MAX                         (0x1a89)

#if defined(PRODUCT_CFG_PRODUCT_TYPE_NDM) || defined(PRODUCT_CFG_HSO)
#define ID_DIAG_CMD_MAC_REPROT_NDM_CAPTURE            (0x1a90)      //NDM上报抓包消息
#define ID_DIAG_CMD_MAC_SET_NDM_CAPTURE_SWITCH        (0x1a91)      //HSO下发抓包过滤开关
#define ID_DIAG_CMD_MAC_SET_NDM_ACCESS_INFO           (0x1a92)      //HSO下发接入信息
#endif

// 无扰SNR组件新增维测 0x1a90 ~ 0x1a99
#define ID_DIAG_CMD_SNR_MIN                           (0x1b90)
#define ID_DIAG_CMD_SNR_SET_VALUE_CNT_PARAM           (0x1b90)      // 设置值域统计参数
#define ID_DIAG_CMD_SNR_QUERY_VALUE_CNT_DATA          (0x1b91)      // 查询值域统计信息
#define ID_DIAG_CMD_SNR_MAX                           (0x1b99)

/**************END**************************************************/

/**************END**************************************************/

/************************************************************************/
/*     抄控器业务 类ID  [0x1c00 ~ 0x1c80 )                               */
/***********START*************************************************************/
#define ID_DIAG_CMD_NDM_CTRL_BUSI_ID_SEG_START  (0x1c00)
#define ID_DIAG_CMD_NDM_CTRL_BUSI_ID_SEG_END    (0x1c80)

#define ID_DIAG_CMD_NDM_CTRL_ROUTE_LOCK     (0x1C00)  // 路由锁定
#define ID_DIAG_CMD_NDM_CTRL_ROUTE_UNLOCK   (0x1C01)  // 路由解锁
#define ID_DIAG_CMD_NDM_CTRL_ROUTE_CONFIG   (0x1C02)  // 静态路由配置
#define ID_DIAG_CMD_NDM_CTRL_TRACE          (0x1C03)  // trace 命令
#define ID_DIAG_CMD_NDM_CTRL_PING           (0x1C04)  // ping 命令
#define ID_DIAG_CMD_NDM_CTRL_SEARCH_DEV     (0x1C05)  // 搜索设备
/**************END**************************************************/

/*********************************************************/
/***    其他业务类  [0x1c81~0x1ca0)                    ***/
/********START*************************************************/
#define ID_DIAG_CMD_QUERY_DFX_NDM_NM_ATTR    (0x1c81)   //用于抄控器查询网络的基本信息
#define ID_DIAG_CMD_NDM_CTRL_PIPE_ACK        (0x1c82) //用于需要抄控器通道在每到达一个站点后回复ACK用.
#define ID_DIAG_CMD_QUERY_CMD_NOT_SUPPORT    (0x1c83)
#define ID_DIAG_CMD_MAC_QUERY_MAC_BUFFER     (0x1c84)
//#define ID_DIAG_CMD_MAC_CONFIG_SW            (0x1C85)
#define ID_DIAG_CMD_MAC_HANDLE_CONNECT_IND   (0x1C86)
#define ID_DIAG_CMD_MAC_HANDLE_CONNECT_CNF   (0x1C87)

/********END**************************************************/

/************************************************************/
/*** 保留 [0x1ca1 ~ 0x2000)        **************************/
/************************************************************/


///////////////////////////////////////////////////////
////////////////----以上是 查询命令----------/////////////////////
//////////////////////////////////////////////////////////


/***********************************************/
// 命令行开关 0x1C00 ~ 0x1c0F
/*************************************************/

//////////////////////////////////////////////////////////////////////////
// 抄控器相关DIAG 命令 (0x1C10 ~ 0X1CFF)
// added by db 2012-06-07
//////////////////////////////////////////////////////////////////////////


/************************************************************************/
/*    START  主动上报类 以及 存储类 ID  0x2900~0x3000                   */
/************************************************************************/
typedef HI_U16 ID_DIAG_REPORT_CMD;

#define ID_DIAG_CMD_NOTIFY_MAC_SPEED_LOW             (0x2900)
#define ID_DIAG_CMD_REPORT_TXRX_STATIC_SPEED         (0x2901)
#define ID_DIAG_CMD_REPORT_JOIN_FAILED               (0x2902)
#define ID_DIAG_CMD_REPORT_NETWORK_ATTRIBUTE         (0x2905)
#define ID_DIAG_CMD_RECORD_SPEED_ABNORMITY           (0x2922)

/*******END 主动上报类 以及 存储类 ID  **************************************/
/************************************************************************/

/************************************************************************/
/*      主动上报类ID  0x2900~0x3000                                                  */
/************************************************************************/


/************************************************************************/
/*   事件ID (0x1000,0x5000   )                                         */
/************************************************************************/
typedef HI_U16 ID_DIAG_EVENT;
#define ID_DIAG_EVENT_NO_SACK_BROADCAST (0x1001)
#define ID_DIAG_EVENT_NOTIFY_MAC_RX_SPEED_LOW   (0x1002)
#define ID_DIAG_EVENT_NOTIFY_MAC_TX_SPEED_LOW   (0x1003)


/************************************************************************/
/*        2    查询类                                                      */
/************************************************************************/
/**
 *
 * 2.1    头端：网络收敛时间
 *
 */
typedef struct _DIAG_CMD_QUERY_NETWORK_CONVERGENCE_TIME_STRU_
{
   HI_U32 convergence_time;             //组网完成时间，单位s
} DIAG_CMD_QUERY_NETWORK_CONVERGENCE_TIME_STRU;

/**
 *
 * 2.1.1    头端：频段状态
 *
 */
typedef struct _DIAG_CMD_QUERY_FREQUENCY_STATUS_STRU_
{
    HI_U8 is_rcv_change_freq_mode_notify;//0: 频带不变     1:频带发生变化   2 :不是CCO
    HI_U8 pad[3];
} DIAG_CMD_QUERY_FREQUENCY_STATUS_STRU;

/**
 *
 * 2.2    头端：网络拓扑信息
 *
 */
#define MAC_APPOINTED_STATIC_PROXY_TYPE            (1)
#define MAC_DYNAMIC_CHOSE_PROXY_TYPE               (2)

typedef struct
{
    HI_U16 tei;                                     // 路径下一跳TEI地址
    HI_S16 snr;                                     // 路径平均信噪比
    HI_U8  channel_info;                            // 信道容量值
    HI_U8  par;                                     // 峰均比
    HI_U8  up_comm_rate;                            // 上行通信率
    HI_U8  down_comm_rate;                          // 下行通信率
    HI_U8  send_success_rate;                       // 实际发送数据成功率
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
    HI_U8 mac[6];                           // 站点MAC地址
    HI_U8 ucSpcVer;                         // code+nv SPC image.hupg
    HI_U8 ucSpcNvVer;                       // nv image.hupg: SPC Ver

    HI_U32 level                : 4;        // 站点层级
    HI_U32 ability              : 4;        // 站点角色
    HI_U32 tei                  : 12;       // 站点TEI地址
    HI_U32 proxy_tei            : 12;       // 站点父代理TEI地址

    HI_U32 join_time;                       // 站点加入网络时间

    HI_U8  product_type         : 3;        // 产品形态
    HI_U8  in_use               : 1;        // 站点是否在线
    HI_U8  pad                  : 4;

    HI_U8  sta_phase0           : 2;        // 站点相位信息
    HI_U8  sta_phase1           : 2;        // 站点相位信息
    HI_U8  sta_phase2           : 2;        // 站点相位信息
    HI_U8  proxy_type           : 2;        // 中继的类型

    HI_U8  ucNvCVer;                        // nv image.hupg: C Ver
    HI_U8  ucChipVerIdx;

    HI_U8  ucBootVerIdx;
    HI_U8  aucVer[3];                       // image.hupg

    HI_U32 ulYear               : 6;        // 客户版本号-年
    HI_U32 ulMonth              : 4;        // 客户版本号-月
    HI_U32 ulDay                : 5;        // 客户版本号-日
    HI_U32 ulHour               : 5;
    HI_U32 ulMinute             : 6;
    HI_U32 ulSecond             : 6;

    HI_U16 usBVer;                          // code+nv image.hupg
    HI_U16 usBNvVer;                        // nv image.hupg: B Ver

    HI_U16 leave_cnt;                       // 离线次数
    HI_U16 proxy_change_cnt;                // 代理变更次数
    HI_U32 leave_total_time;                // 离线时间
    HI_U16 hw_reset_cnt;                    // 记录设备硬复位累积次数
    HI_U16 mac_sw_reset_cnt;                // 记录MAC软复位累积次数
} NFC_DIAG_CMD_QUERY_NETWORK_TOPO_ENTRY_S_VER0;


//NDA通道拓扑上报结构体中每个站点对应的结构体，44Bytes
typedef struct
{
    HI_U8 mac[6];                           // 站点MAC地址
    HI_U8 ucSpcVer;                         // code+nv SPC image.hupg
//    HI_U8 ucSpcNvVer;                       // nv image.hupg: SPC Ver
    HI_U8 ucSwVerLo;                        // 客户版本号低字节

    HI_U32 level                : 4;        // 站点层级
    HI_U32 ability              : 4;        // 站点角色
    HI_U32 tei                  : 12;       // 站点TEI地址
    HI_U32 proxy_tei            : 12;       // 站点父代理TEI地址

    HI_U32 join_time;                       // 站点加入网络时间

    HI_U8  product_type         : 3;        // 产品形态
    HI_U8  in_use               : 1;        // 站点是否在线
//    HI_U8  pad                  : 4;
    HI_U8  stru_ver             : 4;        // 结构版本号，此次为1

    HI_U8  sta_phase0           : 2;        // 站点相位信息
    HI_U8  sta_phase1           : 2;        // 站点相位信息
    HI_U8  sta_phase2           : 2;        // 站点相位信息
    HI_U8  proxy_type           : 2;        // 中继的类型

//    HI_U8  ucNvCVer;                        // nv image.hupg: C Ver
    HI_U8  ucSwVerHi;                       // 客户版本号高字节
    HI_U8  ucChipVerIdx;

    HI_U8  ucBootVerIdx;
    HI_U8  aucVer[3];                       // image.hupg

    HI_U32 ulYear               : 6;        // 客户版本号-年
    HI_U32 ulMonth              : 4;        // 客户版本号-月
    HI_U32 ulDay                : 5;        // 客户版本号-日
    HI_U32 ulHour               : 5;
    HI_U32 ulMinute             : 6;
    HI_U32 ulSecond             : 6;

    HI_U16 usBVer;                          // code+nv image.hupg
//    HI_U16 usBNvVer;                        // nv image.hupg: B Ver
    HI_CHAR szManuCode[2];                  // 厂商代码

    HI_U16 leave_cnt;                       // 离线次数
    HI_U16 proxy_change_cnt;                // 代理变更次数
    HI_U32 leave_total_time;                // 离线时间
    HI_U16 hw_reset_cnt;                    // 记录设备硬复位累积次数
    HI_U16 mac_sw_reset_cnt;                // 记录MAC软复位累积次数
} NFC_DIAG_CMD_QUERY_NETWORK_TOPO_ENTRY_S;

//NDA通道拓扑上报结构体
typedef struct
{
    HI_U16 rsp_idx;                             //流控通道应答报文编号
    HI_U16 total_num            : 10;           //需要上报的站点总数
    HI_U16 is_last              : 1;            //本轮请求的最后一个应答报文标识
    HI_U16 num                  : 5;            //当前数据包所含的站点数

    HI_U32 cur_req_send_idx     : 8;            //流控通道当前请求命令的编号
    HI_U32 pad                  : 24;           //

    NFC_DIAG_CMD_QUERY_NETWORK_TOPO_ENTRY_S entry[MAC_DIAG_CMD_QUERY_NETWORK_TOPO_ENTRY_NUM];
} NFC_DIAG_QUERY_NETWORK_TOPO_IND_S;

//拓扑上报HSO的接口
typedef struct _DIAG_CMD_QUERY_NETWORK_TOPO_STRU_
{
    HI_U16 sn;              //每轮拓扑上报的序列号，对于一次查询命令，对应的多个上报数据包的该值相同；不同轮请求的上报，该值不同
    HI_U16 is_last  : 1;    //本轮上报的最后一个包的标识：最后一个包，该值为1，非最后一个包，该值为0
    HI_U16 num      : 15;   //本数据包中包含的有效站点数

    NFC_DIAG_CMD_QUERY_NETWORK_TOPO_ENTRY_S entry[DFX_NETWORK_TOPO_ENTRY_MAX];
} DIAG_CMD_QUERY_NETWORK_TOPO_STRU;

//NDA通道查询拓扑命令的参数结构
typedef struct
{
   HI_U8 aucTeiBitmap[256];             //TEI过滤表配置

   HI_U32 CurReqSendIdx : 8;            //流控通道当前请求命令的索引号
   HI_U32 pad           : 24;           //保留

   HI_U32 ulSn;                         //一次HSO请求的标识，不同的请求，该值不同
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
 * 2.3    头端：报告和自己协调的网络信息.
 *
 */
typedef struct
{
    HI_U32 snid;
    HI_U8 is_single_comm;     //是否单通
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
 * 2.4    终端：(发起关联的次数) 请求失败的次数
 *
 */
typedef struct _DIAG_CMD_QUERY_REQUEST_JOIN_FAILED_COUNT_STRU_
{
   HI_U32 request_join_failed_count;
} DIAG_CMD_QUERY_REQUEST_JOIN_FAILED_COUNT_STRU;

/**
 *
 * 2.5    终端：关联用时（从发起关联请求到成功关联）
 *
 */
typedef struct _DIAG_CMD_QUERY_JOIN_NETWORK_EXPIRED_STRU_
{
   HI_U32 expire;
} DIAG_CMD_QUERY_JOIN_NETWORK_EXPIRED_STRU;

/**
 *
 * 2.6    终端：发起关联的对象STA
 *
 */
typedef struct
{
    HI_U8 mac[6];
    HI_U16 tei;
} DIAG_CMD_QUERY_ASSOCIATED_STAS_ENTRY_STRU;

//站点的关联对象信息
typedef struct _DIAG_CMD_QUERY_ASSOCIATED_STAS_STRU_
{
    HI_U16 num;
    HI_U16 rsvd;
    DIAG_CMD_QUERY_ASSOCIATED_STAS_ENTRY_STRU entry[DFX_ASSOCIATED_STAS_ENTRY_MAX];
} DIAG_CMD_QUERY_ASSOCIATED_STAS_STRU;

typedef struct
{
    HI_U16 assoc_req_cnt;                   // 关联请求次数
    HI_U16 proxy_tei;                       // 最终选择的代理

    HI_U32  snid;                            // 网络SNID号

    HI_U8  freq;                            // 频段
    HI_U8  network_form_serial_number;      // 组网序列号
    HI_U8  pad[2];

    HI_U32 join_time;                       // 加入网络时间
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
    HI_U32  snid;            // 网络号

    HI_U8  freq;            // 频段号
    HI_U8  status;          // 状态
    HI_U8  req_cnt;         // 关联次数
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
    HI_U8  cco_mac[6];              // CCO的MAC地址
    HI_U8  pad;
    HI_U8  network_serial_num;      // 组网序列号
    HI_U32  snid;                    // 网络号

    HI_U32 change_time;             // 变更时间
}DIAG_CMD_SERIAL_NUM_CHANGE_ENTRY_STRU;
#define NM_SERIAL_NUM_CHANGE_INFO_NUM   (10)
typedef struct
{
    HI_U32 index;
    DIAG_CMD_SERIAL_NUM_CHANGE_ENTRY_STRU serial_num_change_entry[NM_SERIAL_NUM_CHANGE_INFO_NUM];
}DIAG_CMD_SERIAL_NUM_CHANGE_DFX_STRU;
/**
 *
 * 2.7    终端：发现列表
 *
 */

typedef struct
{
    HI_U32 channel_info;                        // 信道质量
    HI_U32 stat_cnt;                            // 接收个数
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
    HI_U8  mac[HI_PLC_MAC_ADDR_LEN];                 // 站点MAC地址
    HI_U8  role;                                     // 站点角色
    HI_U8  level;                                    // 站点层级

    HI_U8  phase;                                    // 站点相位
    HI_U8  tm_capacity;                              // 站点负载能力
    HI_U8  up_channel_update_flag;
    HI_U8  rsvd1;                                 // sounding结果是否可用

    rt_channel_stat_st2 last_down_channel_stat;      // 上次下行通信率统计
    rt_channel_stat_st2 last_up_channel_stat;        // 上次上行通信率统计
    rt_channel_stat_st2 cur_down_channel_stat;       // 当前下行通信率统计
    HI_U8 comm_rate;                                 // 上下行通信率
    HI_U8 down_comm_rate;                            // 下行通信成功率
    HI_U8 up_comm_rate;                              // 上行通信率
    HI_U8 proxy_proxy_comm_rate;                     // 与代理的通信率
    HI_U32 send_pk_cnt;                              // 上行发送数据个数

    HI_U32 send_fail_cnt;                           // 发送失败次数
    HI_U32 send_success_cnt;                        // 发送成功次数
    HI_U8  level_relation;                          // 层级关系
    HI_U8  rsvd;
    HI_U16 proxy_tei;                               // 父代理TEI
    rt_proxy_comm_st2 proxy_channel_stat;           // 父代理下行通信率信息

    HI_U32 rcv_beacon_cnt;                          // 接收该站点BEACON帧的个数
    HI_U8  channel_info;                            // 信道质量(beacon帧存在)
    HI_U8  par;                                     // 峰均比(SOF帧)
    HI_S16 snr;                                     // 平均信噪比(SOF帧)
}DIAG_CMD_QUERY_DISCOVER_LIST_ENTRY_STRU;

typedef struct
{
   HI_U16 sn;
   HI_U16 is_last;
   HI_U16 num;
   HI_U16 pad;
   DIAG_CMD_QUERY_DISCOVER_LIST_ENTRY_STRU entry[DFX_DISCOVER_LIST_ENTRY_MAX];
} DIAG_CMD_QUERY_DISCOVER_LIST_STRU;

//精简的发现列表实体
typedef struct
{
    HI_U32 tei                      : 12;
    HI_U32 proxy_tei                : 12;//父代理TEI
    HI_U32 role                     : 4;//站点角色  0~15
    HI_U32 level                    : 4;//站点层级  0~15

    //可以计算得出
    //HI_U32  comm_rate                : 7;//上下行通信率,comm_rate = down_comm_rate * up_comm_rate / 100,由HSO计算显示
    HI_U32 down_comm_rate           : 7;//下行通信成功率
    HI_U32 up_comm_rate             : 7;//上行通信率
    HI_U32 proxy_proxy_comm_rate    : 7;//与代理的通信率
    HI_U32 level_relation           : 2;//层级关系 0~3
    HI_U32 pad1                     : 9;

    HI_U32 send_fail_cnt;              //发送失败次数
    HI_U32 send_success_cnt;           //发送成功次数

    HI_U8  channel_info;                //信道质量(beacon帧存在)
    HI_U8  par;                         //峰均比(SOF帧)
    HI_S16 snr                      : 12;//平均信噪比(SOF帧)
    HI_S16 pad2                     : 4;
}DIAG_CMD_QUERY_DISCOVER_LIST_ENTRY_SM_STRU;

//精简的发现列表的上报HSO的实体
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
 * 2.8    站点路由表信息
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
    HI_U16 usExceptedReplyCount; // 发现故障中继个数
    HI_U16 usExceptedMeterCount; // 发现故障终端个数

    HI_U16 ulFirstFormedNetElapsed; // 第一次组网时长
    HI_U16 pad;

    HI_U32 ulLastFormedNetElapsed; // 最后一次组网时长
    HI_U32 ulReFormedNetCount; // 重新组网次数

}DIAG_CMD_QUERY_NETWORK_STATE_INFO_CCO_ENTRY_S;

typedef struct
{
    HI_U32 ulAssocedFailedCount; // 发起关联失败次数
    HI_U32 ulAssocedSuccedCount; // 发起关联成功次数
    HI_U32 ulAssocedLastSuccedExpire; // 最后一次成功关联用时
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
    HI_U32  MaxFreqNum;                                 //频段总数
    HI_U32  FreqChangeNum;                              //频段切换次数

    HI_CHAR CurrFreq[DIAG_CMD_QUERY_DEVICE_FREQ_LIST_SIZE];   //当前的频段
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
    HI_BOOL                 SetNdm;         // hi_true 设置NDM设备
    HI_U8                   ucRsvd;
    HI_U8                   DevId[6];
    DIAG_CMD_NDC_HSO_REQ_S  stHsoRequest;
}DIAG_CMD_NDC_TEST_STRU;

#if defined(PRODUCT_CFG_PRODUCT_TYPE_NDM) || defined(PRODUCT_CFG_HSO)
typedef struct
{
    HI_U8 attenuate_flag;       // 衰减标记
    HI_U8 pad[3];
}DIAG_CMD_NDM_CTRL_ATTENUATE_STRU;
typedef struct
{
    HI_U32 ulRc;               // 结果码 成功清除返回HI_ERR_SUCCESS，OTHER VALUE fail
}DIAG_CMD_NDM_CTRL_ATTENUATE_ACK_STRU;
#endif

typedef struct
{
    HI_U8  mac[HI_PLC_MAC_ADDR_LEN];                 // 站点MAC地址
    HI_U16 proxy_tei;                                // 父代理TEI

    HI_U32 role : 4;                                 // 站点角色
    HI_U32 level : 4;                                // 站点层级
    HI_U32 min_multiproxy_comm_rate : 7;             // 路由最小通信率
    HI_U32 proxy_comm_rate : 7;			             // 与代理站点的上行通信率
    HI_U32 proxy_down_comm_rate : 7;		         // 与代理站点的下行通信率
    HI_U32 network_mode : 2;                         // 网络模式:0.PLC模式，1.RF模式，2.SC模式
    HI_U32 is_dual_module : 1;                       // 是否为双模模块:0.单模模块，1.双模模块

    HI_U8  comm_rate;                                // 上下行通信率
    HI_U8  down_comm_rate;                           // 下行通信成功率
    HI_U8  up_comm_rate;			                 // 上行通信成功率
    HI_U8  proxy_proxy_comm_rate;                    // 路径通信率

    HI_U16 cur_down_rcv_cnt;		                 // 当前周期下行接收个数(组网前统计接收信标帧，可能会超过255，因此采用16位)
    HI_S16 snr;                                      // 平均信噪比(SOF帧)

    HI_U8  channel_info;                             // 信道质量(beacon帧存在)
    HI_U8  is_exist : 1;                             // 通过下行路由窗口统计数据，确定该站点是否为本站点发现站点
    HI_U8  is_multi_freq_avail : 1;					 // 多频频段是否可用
	HI_U8  multi_freq : 5;							 // 多频频段
    HI_U8  is_in_use : 1;                            // 该缓存位置是否被使用。

    HI_U8  proxy_channel_info;                       // 与代理站点的信道容量
    HI_U8  down_rcv_flag : 1;                        // 下行接收标志，用于快速路由变更
    HI_U8  up_channel_update_flag : 1;               // 上行通信率是否更新
    HI_U8  proxy_calc_ok : 1;			             // 是否计算出与代理站点的通信率
    HI_U8  level_relation : 2;                       // 层级关系
    HI_U8  phase : 2;                                // 站点相位
    HI_U8  associate_flag : 1;                       // 关联标志，0.未关联，1.已关联

    HI_U8  not_in_use_time;                          // 路径信息表老化策略
    HI_U8  snd_channel_info;                         // 获取站点发送数据时的信道容量(空闲)
    HI_U8  rssi;                                     // 下行信号强度:RF链路信号强度、PLC信道衰减、SC信号强度
    HI_U8  up_rssi;                                  // 上行信号强度:RF链路信号强度、PLC信道衰减、SC信号强度

    HI_U8  down_rcv_cnt[4];	     // 滑窗下行接收个数
    HI_U8  up_rcv_cnt[4];	         // 滑窗上行接收个数,该站点接收本站点发送的报文数
    HI_U8  down_send_cnt[4];	     // 滑窗下行发送个数,该站点发送的报文数

    HI_U32 send_fail_cnt;                            // 发送失败次数
    HI_U32 send_success_cnt;                         // 发送成功次数
    HI_U32 rcv_beacon_cnt;                           // 接收该站点BEACON帧的个数
} DIAG_CMD_QUERY_CHOOSE_DISLIST;

typedef struct
{
    HI_U16 actual_index;    //站点实际存储位置
    HI_U8 path_tablae_bitmap[DFX_PATH_TABLE_BITMAP_LEN]; //路径信息表bit图

    HI_U16 exist_node;                          //已经存在的站点数目
    HI_U16 pad;
} DIAG_CMD_QUERY_PATH_TABLE_REFERENCE;

typedef struct
{
    HI_BOOL cco_is_in_use;          //cco存储位置是否被使用。
    HI_U8 pad;
    HI_U16 pk_filter_count;

    HI_U32 pk_filter_pos;       //tei实际存储位置

    HI_U8 pk_filter_bitmap[DFX_PATH_TABLE_BITMAP_LEN];
}DIAG_CMD_QUERY_PK_FILTER;

typedef struct
{
    HI_U16  tei;                            // 站点TEI
    HI_U16  proxy_tei;                      // 代理站点的TEI
    HI_U16  childsum;                       // 子节点的个数
    HI_U8   mac[HI_PLC_MAC_ADDR_LEN];       // MAC地址
    HI_U8   level;                          // 层级
    HI_U8   ability;                        // 参考 IS_CCO 宏定义
    HI_U8   heartbeat_count;                // 心跳个数 , 对于 heartbeat_count 的判断,是在心跳检测周内,只要有一次心跳就认为没有离线
    HI_U8   sta_phase[3];
    HI_BOOL new_joiner;                     // 是否为本轮刚加入的站点,心跳检测时区别于老站点,不做心跳检测离线处理的判断用
    HI_U8   product_type;                   // 产品的类型,参考 HI_ND_TYPE_CCO , HI_ND_TYPE_RELAY
    HI_U8   not_rcv_heartbeat_continuous;   // 连续没有收到心跳报文的检测周期计数
    HI_BOOL in_use;                         // 站点是否在线的标志
    HI_BOOL is_relay_ability;               // 是否为中继
    HI_U8   d2d_proxy_comm_rate;            // 路径通信率
    HI_U8   proxy_type;                     // 代理类型
    HI_U8   freq_mode_present;              // 当前的频段序号
    HI_U8   nm_mmv;                         // 网络管理报文版本号
    HI_U8   down_comm_rate;                 // 下行通信成功率
    HI_U8   up_comm_rate;                   // 上行通信成功率
    HI_U8   is_support_ared_recognition : 1;// 是否支持台区识别功能
    HI_U8   is_dual_module : 1;             // 是否为双模模块
    HI_U8   network_mode : 1;               // 网络模式
    HI_U8   pad : 4;
    HI_U8   power_ctrl_flag :1;               //是否是功率控制缓存站点
    HI_U8   is_notify_leave;                // 是否通知离线
    HI_U8   notify_leave_cnt;               // 通知次数
    HI_SYS_STATUS_INFO_S sta_ver;           // 版本信息
    HI_U32  addtime;                        // 站点加入网络时间
    HI_U32  assoc_random;                   // 关联请求随机数,参考业务背景说明-2
    HI_U32  proxy_change_cnt;               // 代理变更次数
    HI_U32  leave_cnt;                      // 离线次数
    HI_U32  leave_time;                     // 离线时间
    HI_U32  leave_total_time;               // 离线总时长
    HI_U16  hw_reset_cnt;                   // 记录设备硬复位累积次数
    HI_U16  mac_sw_reset_cnt;               // 记录MAC软复位累积次数
    HI_U8   leave_reason;                   // 离线原因
    HI_U8   hw_reset_reason;                // 硬重启原因
    HI_U8   sw_reset_reason;                // 软重启原因
    HI_U8   pad1;
    HI_U32  last_heartbeat_time;            // 最后一次心跳时间
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
    HI_U8  pad[2];// 填充
} HI_MAC_STRU;
typedef struct
{
    HI_U8  index;                                       // 当前使用MAC索引
    HI_U8  total_cnt;                                   // MAC总数
    HI_U8  clear_cnt;                                   // 清除MAC地址次数
    HI_U8  set_cnt;                                     // 设置备份MAC次数
    HI_MAC_STRU mac_list[NM_MAX_BACKUP_MAC_CNT];
}DIAG_CMD_QUERY_BACKUP_MAC_STRU;
//*****************************************************************************
// MAC: 多网路相关设置 SNID，开关 STA和CCO行为不同点？
// CMD ID : ID_DIAG_CMD_CCO_MAC_MULTYNET_SET
// REQ STRU:DIAG_CMD_CCO_MAC_MULTYNET_SET_REQ_STRU
// ACk STRU:DIAG_CMD_CCO_MAC_MULTYNET_SET_ACK_STRU
//*****************************************************************************
// REQ
#define MAC_MUTYNET_OP_VA_SET_SNID_ONLY     (1) // 设置SNID号 ONLY
#define MAC_MUTYNET_OP_VA_SET_MUTY_SWT_ONLY (2) // 设置多网络开关 ONLY
#define MAC_MUTYNET_OP_VA_SET_NORMAL        (3) // 全设置

typedef struct
{
    HI_U32 ulSnid;   // V100 :合法值: [1,15]  非法值: 0xff，该值时不设置
    HI_U8 ucIsMultyOpen;        // TRUE：多网络打开 FALSE: 多网络关闭 0xFF: 无效，该值时不设置
    HI_U8 ucMutyOpVa;           // 操作码：1设置enSnid；2设置ucIsMultyOpen；3全设置
    HI_U8 ucRsv[2];
} DIAG_CMD_CCO_MAC_MULTYNET_SET_REQ_STRU;

// ACK
typedef struct
{
    HI_U32 ulRet;// OK HI_ERR_SUCCESS; OTHER FAIL;
} DIAG_CMD_CCO_MAC_MULTYNET_SET_ACK_STRU;

//*****************************************************************************
// MAC: 多网络相关查询，查询SNID，开关状态，STA和CCO行为不同点？
// CMD ID : ID_DIAG_CMD_CCO_MAC_MULTYNET_QUERY
// REQ STRU:NULL
// ACk STRU:DIAG_CMD_CCO_MAC_MULTYNET_QUERY_ACK_STRU
//*****************************************************************************

// REQ
// NULL

// ACK
typedef struct
{
    HI_U32 ulSnid;               // V100:合法值: [1,6]  非法值: 0xff
    HI_U8  ucIsMultyOpen;        // TRUE：多网络打开 FALSE: 多网络关闭 0xFF: 无效
    HI_U8  ucRsv[3];
} DIAG_CMD_CCO_MAC_MULTYNET_QUERY_ACK_STRU;

//*****************************************************************************
// MAC: 频段设置 STA: 不支持设置；CCO:支持
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
    // 全频 0;低频1;高频2 无效值:0xff
    EN_MAC_FREQ enFreq;
    HI_U32 ulFreqSwtTime; // 单位秒，频段切换时间(系统默认两分钟) 0为非法不设置
} DIAG_CMD_MAC_FREQ_SET_REQ_STRU;

// ACK STRU
// 同步立即返回
typedef struct
{
    HI_U32 ulRc;          // 结果码 成功设置返回 HI_ERR_SUCCESS OTHER VALUE fail
    EN_MAC_FREQ enFreq;   // 全频 0;低频1;高频2 无效值:0xff
    HI_U32 ulFreqSwtTime; // 单位：s秒 频段切换时间(系统默认两分钟,120s)
} DIAG_CMD_MAC_FREQ_SET_ACK_STRU;

//*****************************************************************************
// MAC: 频段查询 CCO AND STA:可以查询当前频段，即将切换的频段，切换剩余时间
// CMD ID : ID_DIAG_CMD_MAC_FREQ_QUERY
// REQ STRU:NULL
// ACk STRU:DIAG_CMD_MAC_FREQ_QUERY_ACK_STRU
//*****************************************************************************

// REQ STRU
// NULL

// ACK STRU
typedef struct
{
    EN_MAC_FREQ enCurFreq;    // 当前频段0xff非法
    EN_MAC_FREQ enSwt2Freq;   // 0xff非法，在切换中才会返回
    HI_U8  isSwtching;        // 是否正在切换。HI_TRUE正在切换，HI_FALSE未在切换
    HI_U8  blink_freq_seq;
    HI_U8  rsv[2];
    HI_U32 ulFreqSwtTime;     // 单位为秒最小单位秒，isSwtching如果为TRUE还需要多久切换完成
} DIAG_CMD_MAC_FREQ_QUERY_ACK_STRU;


//*****************************************************************************
// MAC: 白名单是否开启的设置，设置下去CCO会软重启（超过50%的站点掉线，CCO软重启）
//      是否把表档案做在白名单的设置
// CMD ID : ID_DIAG_CMD_CCO_MAC_MULTYNET_REJECT_LIST_CLR
// REQ STRU:NULL
// ACk STRU:DIAG_CMD_CCO_MAC_MULTYNET_REJECT_LIST_CLR_ACK_STRU
//*****************************************************************************

// REQ STRU

// 用BIT表达更好，但现在变量少，简单实现
#define MAC_DFX_WHITE_SET_NORMAL      (3) // isWhiteOpen和isMtFileInWhite均设置，HSO只使用该值
#define MAC_DFX_WHITE_SET_WHITE_ONLY  (1) // 只设置isWhiteOpen
#define MAC_DFX_WHITE_SET_MTFILE_ONLY (2) // 只设置isMtFileInWhite

typedef struct
{
    HI_U8 ucOperVa;            // 操作码：MAC_DFX_WHITE_SET_NOMAR etc,HSO当前只下发MAC_DFX_WHITE_SET_NORMAL,不使用该值
    HI_BOOL isWhiteOpen;       // TRUE 开启 FALSE未开启
    HI_U8 mode;                // 白名单的模式
    HI_U8 rsv;
} DIAG_CMD_CCO_MAC_WHITE_STA_SET_REQ_STRU;

// ACK STRU
typedef struct
{
    HI_U32 ulRc;               // 结果码 成功设置返回HI_ERR_SUCCESS，OTHER VALUE fail
} DIAG_CMD_CCO_MAC_WHITE_STA_SET_ACK_STRU;


//*****************************************************************************
// MAC: 黑白名单状态查询 白名单是否开启的状态查询，是否把表档案做在白名单
// CMD ID : ID_DIAG_CMD_CCO_MAC_WHITE_STA_QUERY
// REQ STRU:NULL
// ACk STRU:DIAG_CMD_CCO_MAC_WHITE_STA_QUERY_ACK_STRU
//*****************************************************************************

// REQ STRU
// NULL

// ACK STRU
typedef struct
{
    HI_U8 isWhiteOpen;         // NV项中是否打开白名单 HI_TRUE 是; HI_FALSE 不是;
    HI_U8 mode;                // 白名单模式
    HI_U8 rsv1;                // 实际运行中MAC认证是否打开 HI_TRUE 是; HI_FALSE 不是;HSO未使用
    HI_U8 rsv;
} DIAG_CMD_CCO_MAC_WHITE_STA_QUERY_ACK_STRU;

//*****************************************************************************
// MAC: 清除MAC相关的东东，比如CCO拒绝列表等
// CMD ID : ID_DIAG_CMD_CCO_MAC_CLR
// REQ STRU: DIAG_CMD_CCO_MAC_CLR_REQ_STRU
// ACk STRU: DIAG_CMD_CCO_MAC_CLR_ACK_STRU
//*****************************************************************************

// REQ STRU
typedef enum _EN_MAC_CLR_OPER_
{
    EN_MAC_CLR_CCO_REJECT_LIST = 1,// 清除CCO多网络拒绝列表
    EN_MAC_CLR_INVALID = 0xff
} EN_MAC_CLR_OPER;

typedef struct
{
    EN_MAC_CLR_OPER enClrOp;  // 如上
} DIAG_CMD_CCO_MAC_CLR_REQ_STRU;

// ACK STRU
typedef struct
{
    HI_U32 ulRc;               // 结果码 成功清除返回HI_ERR_SUCCESS，OTHER VALUE fail
} DIAG_CMD_CCO_MAC_CLR_REQ_STRU_ACK_STRU;


typedef struct
{
    HI_U16 proxy_num;               // 当前代理数目
    HI_U16 pad;
} DIAG_CMD_PROXY_NUM_REQ_STRU;

typedef struct
{
    HI_U16 child_sum;               // 某站点的子站点数
    HI_U16 pad;
} DIAG_CMD_CHILD_SUM_ACK_STRU;

typedef struct
{
    HI_U16 tei;                     // 查询的站点
    HI_U16 pad;
} DIAG_CMD_CHILD_SUM_REQ_STRU;


typedef struct
{
    HI_U32 create_cnt;              // 产生报文个数
    HI_U32 rcv_cnt;                 // 接收报文个数
    HI_U32 last_create_time;        // 上次生成报文时间
    HI_U32 last_rcv_time;           // 上次接收报文时间
}nm_packet_info;

// 记录报文信息的最大个数
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
    nm_packet_info assoc_req_info;          // 关联请求报文信息
    nm_packet_info assoc_cnf_info;          // 关联回复报文信息
    nm_packet_info assoc_ind_info;          // 关联确认报文信息
    nm_packet_info assoc_gather_ind_info;   // 关联汇总报文信息
    nm_packet_info change_proxy_req_info;   // 代理变更请求信息
    nm_packet_info change_proxy_cnf_info;   // 代理变更确认信息

    nm_rcv_assoc_packet_info rcv_assoc_req_info;    // 接收到关联请求报文的信息
    nm_rcv_assoc_packet_info rcv_assoc_cnf_info;    // 接收到关联回复报文的信息
    #endif

    #if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    HI_U32 rcv_heartbeat_pk_cnt;                    // 接收心跳报文个数
    HI_U32 heartbeat_cycle_cnt;                     // 心跳周期计数
    #endif

    #if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
    HI_U32 create_heartbeat_by_timer_cnt;           // 由于定时器触发创建心跳报文个数
    HI_U32 create_heartbeat_by_rcv_cnt;             // 由于接收心跳报文触发创建心跳报文个数
    #endif

    HI_U32 creat_discovery_list_cnt;                //创建发现列表报文数
    HI_U32 rcv_discovery_list_cnt;                  //接收发现列表报文数

    HI_U32 send_msdu_success_cnt;                   //发送报文到汇聚成功的次数
    HI_U32 send_msdu_fail_cnt;                      //发送报文到汇聚失败的次数

    HI_U32 rcv_msdu_cnt;                    //接收到的网管报文计数
    HI_U32 not_support_msdu_cnt;            //不支持处理的报文计数
}nm_packet_stat_info_st;    //对应HSO查询结构体DIAG_CMD_NM_PACKET_STAT_STRU

typedef struct
{
    nm_rcv_assoc_packet_info rcv_assoc_req_info;    // 接收到关联请求报文的信息
}DIAG_CMD_NM_ASSOC_REQ_STAT_STRU;

typedef struct
{
    nm_rcv_assoc_packet_info rcv_assoc_cnf_info;    // 接收到关联回复报文的信息
}DIAG_CMD_NM_ASSOC_CNF_STAT_STRU;


typedef struct
{
    nm_packet_info assoc_req_info;          // 关联请求报文信息
    nm_packet_info assoc_cnf_info;          // 关联回复报文信息
    nm_packet_info assoc_ind_info;          // 关联确认报文信息
    nm_packet_info change_proxy_req_info;   // 代理变更请求信息
    nm_packet_info change_proxy_cnf_info;   // 代理变更确认信息
    nm_packet_info assoc_gather_ind_info;   // 关联汇总报文信息

    HI_U32 creat_discovery_list_cnt;        // 创建发现列表报文数
    HI_U32 rcv_discovery_list_cnt;          // 接收发现列表报文数

    HI_U32 rcv_heartbeat_pk_cnt;            // 接收心跳报文个数
    HI_U32 heartbeat_cycle_cnt;             // 心跳周期计数
    HI_U32 create_heartbeat_by_timer_cnt;   // 由于定时器触发创建心跳报文个数
    HI_U32 create_heartbeat_by_rcv_cnt;     // 由于接收心跳报文触发创建心跳报文个数

    HI_U32 send_msdu_success_cnt;                   //发送报文到汇聚成功的次数
    HI_U32 send_msdu_fail_cnt;                      //发送报文到汇聚失败的次数

    HI_U32 rcv_msdu_cnt;                    // 接收到的网管报文计数
    HI_U32 not_support_msdu_cnt;            // 不支持处理的报文计数
} DIAG_CMD_NM_PACKET_STAT_STRU;


typedef struct
{
    HI_BOOL     is_tried;                   // 当前轮次是否尝试过此SNID
    HI_U8       state;                      // 0表示无行动
    HI_S16      SNR;
    HI_U32      reassoc_time;               // 下次可以关联时间
    HI_U32      rcv_fc_cnt;                 // 收到FC的个数
    HI_U32      refuse_duration;            // 拒绝时间
    HI_U32      chose_cnt : 28;             // 选择到此SNID的次数
    HI_U32      main_network_snid : 4;      // 主网络SNID号
}sta_snid_stat_info;

typedef struct
{
    sta_snid_stat_info sta_snid_info[15];   // sta关联网络信息
}DIAG_CMD_STA_SNID_INFO_STRU;

#define DIAG_SNR_LEVEL_NUM              (16)
#define DIAG_SNR_COLLECT_CLASS_NUM      (9)
#define DIAG_QUERY_NEIGHBOR_NUM         (32)

typedef struct
{
    HI_BOOL sort_by_att;        // 查询结果展示时，是否以衰减值排序(默认SNR排序)
    HI_U8   filter_by_level : 1;// 方式1: 以层级来筛选
    HI_U8   level           : 7;
    HI_U16  tei;                // 方式2: 以TEI筛选

    HI_U32  join_freq : 8;      // 方式3: 以加入网络筛选，频段SNID、MAC地址和主节点地址任意一个命中即可
    HI_U32  join_snid : 24;
    HI_U8   join_cco_mac[HI_PLC_MAC_ADDR_LEN];
    HI_U8   join_main_node_addr[HI_PLC_MAC_ADDR_LEN];

    HI_U32  belong_freq : 8;    // 方式4: 以归属网络筛选，频段SNID、MAC地址和主节点地址任意一个命中即可
    HI_U32  belong_snid : 24;
    HI_U8   belong_cco_mac[HI_PLC_MAC_ADDR_LEN];
    HI_U8   belong_main_node_addr[HI_PLC_MAC_ADDR_LEN];
} DIAG_CMD_GET_TOP_NEIGHBOR_REQ_STRU;

// 查询信息节点 12
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

// SNR策略: 查询统计器中详细结果REQ, 只要一个命中即可
typedef struct
{
    HI_U32 freq : 8;
    HI_U32 snid : 24;
    HI_U8  cco_mac[HI_PLC_MAC_ADDR_LEN];
    HI_U8  main_node_addr[HI_PLC_MAC_ADDR_LEN];
} DIAG_CMD_STA_SNR_DATA_DETAIL_REQ_STRU;

// SNR策略: 查询统计器中详细结果IND
typedef struct
{
    HI_U32 ret;

    HI_U32 last_reset_time;     // 上次复位统计器的时间，单位s
    HI_U16 collect_duration;    // 统计时长，单位sec
    HI_U8  collect_enable;      // 是否正在统计
    HI_U8  pad;

    HI_U32 freq : 7;            // 工作频段
    HI_U32 snid : 24;           // 工作SNID
    HI_U32 is_detect_net : 1;   // 工作网络是否是探测网络

    HI_U8  detect_freq;         // 探测频段
    HI_U8  detect_snid;         // 探测SNID
    HI_U8  index;               // 在网络统计信息中的下标，将用在SNR统计器和历史记录中作为索引
    HI_U8  pad1;

    HI_U32 last_rcv_time;       // 上一次收到该网络信标的时间

    HI_U8  cco_mac[HI_PLC_MAC_ADDR_LEN];            // CCO的MAC地址
    HI_U8  main_node_addr[HI_PLC_MAC_ADDR_LEN];   // 主节点地址

    HI_S16 snr_avg[DIAG_SNR_LEVEL_NUM];
    HI_U8  att_avg[DIAG_SNR_LEVEL_NUM];
    HI_U32 fc_cnt[DIAG_SNR_LEVEL_NUM];
} DIAG_CMD_STA_SNR_DATA_DETAIL_IND_STRU;

// 单网络SNR历史记录结构 12B
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

// SNR策略: 查询统计器中简要结果IND 396
typedef struct
{
    HI_U32 ret;

    HI_U32 last_reset_time;     // 上次复位统计器的时间，单位s
    HI_U16 collect_duration;    // 统计时长
    HI_U16 collect_enable : 1;  // 是否正在统计
    HI_U16 list_node_num  : 15;

    diag_snr_net_result_st net_results[DIAG_QUERY_NEIGHBOR_NUM];
} DIAG_CMD_STA_SNR_DATA_SIMPLE_IND_STRU;

// SNR策略: 查询单次历史记录REQ
typedef struct
{
    HI_U32 record_index;
} DIAG_CMD_STA_SNR_HISTORY_NODE_REQ_STRU;

// SNR策略: 查询单次历史记录IND
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

// SNR策略: 查询单网络48次历史记录REQ
typedef struct
{
    HI_U32 freq : 8;
    HI_U32 snid : 24;
    HI_U8  cco_mac[HI_PLC_MAC_ADDR_LEN];
    HI_U8  main_node_addr[HI_PLC_MAC_ADDR_LEN];
    HI_U32 begin_index;
} DIAG_CMD_STA_SNR_NET_HISTORY_REQ_STRU;

// SNR策略: 查询单网络32次历史记录IND
typedef struct
{
    HI_U32 ret;
    HI_U32 save_snr_times;

    diag_snr_net_result_st history_records[DIAG_QUERY_SNR_NET_HIS_NUM];
} DIAG_CMD_STA_SNR_NET_HISTORY_IND_STRU;

typedef struct
{
    HI_U32 to_freq  : 8;
    HI_U32 to_snid  : 24;   // 切换到网络SNID

    HI_U8  status;          // 切换状态
    HI_U8  my_class;        // 本网络SNR层级
    HI_U8  to_class;        // 切换到网络的SNR层级
    HI_U8  net_cnt;         // 达到门限网络个数

    HI_S16 my_avg;
    HI_S16 to_avg;
} diag_fast_change_record_st;

// 站点快速收敛记录IND
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

// 查询站点长期评估切换网络状态IND
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

// 查询站点归属变更记录
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
    HI_U32 freq : 7;            // 工作频段
    HI_U32 nid  : 24;           // 工作NID
    HI_U32 is_detect_net : 1;   // 工作网络是否是探测网络

    HI_U8  detect_freq;         // 探测频段
    HI_U8  detect_nid;          // 探测NID
    HI_U8  index;               // 在网络统计信息中的下标
    HI_U8  is_set_main_node_addr    : 1;    // 是否设置了主节点地址
    HI_U8  pad0                     : 7;

    HI_U16 pad1;
    HI_U16 phase_att[PHASE_CNT];

    HI_U8  cco_mac[HI_PLC_MAC_ADDR_LEN];            // CCO的MAC地址
    HI_U8  main_node_addr[HI_PLC_MAC_ADDR_LEN];     // 主节点地址

    HI_U32 last_rcv_time;                           // 最近一次收到的时间
} diag_net_info_st;

// 96
typedef struct
{
    HI_U8  my_snr_class;
    HI_U8  belong_net_idx;
    HI_U8  last_change_reason;
    HI_U8  pad;

    HI_U16 belong_stable_times;     // 应归属信息稳定周期数
    HI_U16 snr_class_stable_times;  // SNR层级稳定周期数
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

    HI_U16 begin_new_collect_cnt;   //启动新的采集周期计数
    HI_U16 pad1;

    HI_S16 preamble_snr;
    HI_U16 preamble_attenuation;
    HI_U32 pream_sta_num_th_cnt;    //超出饱和门限的snr计数
} DIAG_CMD_STA_SNR_CTRL_STAT_IND_STRU;

// 查询附近站点信息REQ
typedef struct
{
    HI_U32 freq : 8;
    HI_U32 snid : 24;
    HI_U16 tei;
    HI_U16 pad;
} DIAG_CMD_NEAR_STA_DATA_REQ_STRU;

// 查询附近站点信息IND
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

// 设置站点的归属网络REQ
typedef struct
{
    HI_U8   cco_mac[HI_PLC_MAC_ADDR_LEN];   // 归属的地址，默认为CCO的MAC地址，is_main_node_addr为1时表示主节点地址
    HI_BOOL is_main_node_addr;              // cco_mac字段，是否为主节点地址
    HI_U8   freq;                           // 指定网络的频段
    HI_U32  snid;                           // 指定网络的SNID
} DIAG_CMD_SET_BELONG_NETWORK_REQ_STRU;

// 设置站点的归属网络IND
typedef struct
{
    HI_U32 ret; // 0代表命令执行成功，其他代表失败
} DIAG_CMD_SET_BELONG_NETWORK_IND_STRU;

// 清除站点的归属网络IND
typedef struct
{
    HI_U32 ret; // 0代表命令执行成功，其他代表失败
} DIAG_CMD_CLEAR_BELONG_NETWORK_IND_STRU;

// 锁定网络参数
typedef struct
{
    HI_U8   cco_mac[HI_PLC_MAC_ADDR_LEN];
    HI_U8   freq;
    HI_U8   is_main_node_addr;

    HI_U32  snid;
    HI_U32  duration; // 锁定时长，单位s
} DIAG_CMD_LOCK_NETWORK_REQ_STRU;

// 锁定网络回复
typedef struct
{
    HI_U32 ret;
} DIAG_CMD_LOCK_NETWORK_IND_STRU;


//抄控器强制连接的请求命令参数
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

//抄控器的属性设置接口
typedef struct
{
    HI_U16 ndm_tei;
    HI_U16 pad;
}DIAG_CMD_NDM_ATTR_SET_STRU;

//抄控器的属性设置应答接器
typedef struct
{
    HI_U32 result;      //设置结果
}DIAG_CMD_NDM_ATTR_SET_ACK_STRU;

/************************************************************************/
/*    4        各模块主动上报类                                            */
/************************************************************************/
/**
 *
 * 4.1    主动上报重复请求关联的STA
 *
 */
typedef struct _DIAG_CMD_REPORT_REPEAT_REQUEST_JOIN_STRU_
{
   HI_U16 tei;
   HI_U16 pad;
} DIAG_CMD_REPORT_REPEAT_REQUEST_JOIN_STRU;

/**
 *
 * 4.2    当缓冲区溢出时做事件记录(各个模块自己负责上报HSO)
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
 * 4.5    头端：TDMA调度周期分配情况( 由网络管理模块主动上报CONSOLE)
 *
 */
typedef struct  id_diag_cmd_tdma_entry_head_
{
    HI_U8 beacon_slot_count;              // 信标时隙数
    HI_U8 bind_csma_slot_count;      // 绑定CSMA时隙数
    HI_U8 tdma_slot_count;     // TDMA时隙数
    HI_U8 beacon_duration;    // beacon时隙持续时间，单位:100微妙.注:每个信标时隙持续时间都是相同的
} id_diag_cmd_tdma_entry_head;

typedef struct  id_diag_cmd_beacon_slot_
{
    HI_U32 offset;                         // 信标时隙开始的相对时间，针对信标周期开始时间的偏移。单位:100微秒
    HI_U16 tei;                            // 时隙对应的TEI
    HI_U8  beacon_type;                    // 信标类型
    HI_U8  event;                          // 标识事件(关联汇总)
} id_diag_cmd_beacon_slot;

typedef struct id_diag_cmd_bind_csma_slot_
{
    HI_U32 offset;                         // 时隙开始的相对时间，针对信标周期开始时间的偏移，单位:100微妙
    HI_U16 duration;                  // 时隙持续时间，单位:100微妙
    HI_U16 tei;                           // 时隙对应的目的TEI
} id_diag_cmd_bind_csma_slot;

typedef struct id_diag_cmd_csma_slot_
{
    HI_U16 offset;                         // 时隙开始的相对时间，针对信标周期开始时间的偏移，单位:100微妙
    HI_U16 duration;                    // 时隙持续时间，单位:100微妙
} id_diag_cmd_csma_slot;

typedef struct  id_diag_cmd_tdma_slot_
{
    HI_U16 offset;                         // 时隙开始的相对时间
    HI_U16 duration;                       // 时隙持续时间
    HI_U16 tei;                            // 时隙对应的TEI
    HI_U8  lid;                            // 时隙连接标识符
    HI_U8  rsvd;                           // 保留
} id_diag_cmd_tdma_slot;

//区域时隙条目
typedef struct id_diag_cmd_region_slot_st_
{
    HI_U32 starttime;                      // 区域时隙开始绝对时间，单位为NTB
    HI_U32 duration;                       // 持续时间,单位为100微妙
    HI_U8  region_id;                      // 网络SNID号
    HI_U8  region_type;                    // 时隙类型(保留时隙或者公用时隙)
    HI_U16 rsvd;                           // 保留
} id_diag_cmd_region_slot_st;

//更改SNID条目
typedef struct id_diag_cmd_snid_entry_
{
    HI_U32 starttime;                      // 更改SNID的时间
    HI_U8  snid;                           // 更改后的SNID号
    HI_U8  rsvd1;                          // 保留
    HI_U16 rsvd2;                          // 保留
} id_diag_cmd_snid_entry;

//更改密钥条目
typedef struct id_diag_cmd_encryption_key_entry_
{
    HI_U32 starttime;                      // 更改密钥的时间
    HI_U8  key_index;                      // 启用密钥的索引号
    HI_U8  svd1;                           // 保留
    HI_U16 rsvd2;                          // 保留
} id_diag_cmd_encryption_key_entry;

// 时隙分配表规格
#define DFX_ID_DIAG_CMD_MAX_BEACON_SLOT 150     // 最多beacon时隙数
#define DFX_ID_DIAG_CMD_MAX_BIND_CSMA_SLOT 50      // 最多绑定csma时隙数
#define DFX_ID_DIAG_CMD_MAX_TDMA_SLOT 50      // 最多tdma时隙数
#define DFX_ID_DIAG_CMD_MAX_REGION_SLOT_COUNT 7       // 最大区域时隙条目数

#define DFX_CONVERT_MAC_ADDR_MODE_NO (1)  //不转换
#define DFX_CONVERT_MAC_ADDR_MODE_YES (2) //转换
/**
 *
 * 4.15    站点加入网络后，主动上报自己的网络属性
 *
 */
typedef struct
{
    HI_U16   my_tei;                        // 本站点tei
    HI_U16   my_proxy_tei;                  // 本站点父代理

    HI_U8    my_level;                      // 本站点层级
    HI_U8    my_ability;                    // 本站点能力，是CCO, 还是中继，还是普通站点
    HI_U8    my_mac[6];                     // 本站点MAC地址

    HI_U32   my_snid;                       // 本站点SNID

    HI_BOOL  network_formed;                // 自组网是否完成
    HI_BOOL  send_data_in_self_phase;       // 是否在本站点相位发送数据
    HI_U8    my_phase_mode;                 // 本站点相位模式， 多相位还是单相位
    HI_U8    my_phase;                      // 本站点相位类型。 A B C ALL

    HI_U8    form_serial_number;            // 组网系列号
    HI_U8    freq_mode_present;             // 站点的频段模式
    HI_U8    fast_evaluate_flag;            // 快速路由评估标志
    HI_U8    network_state;                 // 网络状态

    HI_U32   select_snid;                   // 优选snid

    HI_U8    proxy_flag_exist;              // 是否存在静态代理
    HI_U8    chl_state;                     // MAC通道状态状态
    HI_U8    convert_mac_addr_mode;    // 是否将00结尾的mac地址转化为FE，DFX_CONVERT_MAC_ADDR_MODE_NO
    HI_U8    pad;

    HI_U32  blind_check_count;              // 盲检次数
    HI_U32  rcv_freq_change_notify_count;   // 接收频段切换通知次数
} DIAG_CMD_REPORT_NETWORK_ATTRIBUTE_STRU;

//MAC的通道状态，其定义如下：
typedef enum
{
    HI_MAC_CHL_STATE_UNKNOWN = 0,          //通道状态未知
    HI_MAC_CHL_STATE_FORMING,              //正在组网；
    HI_MAC_CHL_STATE_FORMED,               //组网完成
    HI_MAC_CHL_STATE_NORMAL,               //正常状态，可以支持约定的APP的所有业务；
    HI_MAC_CHL_STATE_MAINTAIN,             //通道维护状态，不支持APP的业务。
}HI_MAC_CHL_STATE_ENUM;

/**
 *
 * 4.16    发起代理变更或关联请求对象

 *
 */
 #define DFX_CHOSE_PROXY_SUM DFX_PROXY_SUM
typedef struct
{
    HI_U8    start_change_proxy;
    HI_U8    start_assoc_req;
    HI_U16   old_proxy;
    HI_U16   chose_proxy_tei[DFX_CHOSE_PROXY_SUM];     //选出的代理站点
    HI_U16   rsvd;
}DIAG_CMD_REPORT_CHOSE_PROXY_TEI_STRU;

/**
 *
 * 4.17    站点判断本站点离线

 *
 */
enum
{
    RCV_BEACON_FAILED = 1,              //STA收不到信标重启(一个完整心跳周期)7
    CHOSE_PROXY_FAIL,                   //收不到代理站点的信息(4个路由周期)
    SERIAL_NUM_CHANGE,                  //组网系列号发生改变
    RCV_LEAVE_IND,                      //接收到离线指示报文
    CCO_MAC_CHANGE,                     //CCO的MAC发生改变

    SAME_NEIGHBOR_SNID,                 //CCO使用，邻居网络SNID号与本网络SNID相同
    SET_FREQ_MODE,                      //STA收到NV中直接变更频段指令(不推荐操作)
    SET_MAC_ADDR,                       //STA接收到变更MAC地址命令
    SET_WHITE_LIST,                     //CCO通过白名单踢掉140个站点时会主动重启
    PROXY_CHANGE_DISCOVERY_NODE,        //STA发现本站点的代理身份变为发现站点

    NOT_IN_WRITER,                      //通过白名单剔除站点
    HSO_SET_CCO_SNID,                   //通过hso命令更改CCO的SNID号
    PCO_MAX_LEVEL,                      //本站点层级超出最大限制
    INTERFACE_SET_CCO_SNID,             //二次开发接口设置CCO的SNID
    NOT_IN_BACKUP_MAC_LIST,             //本站点MAC地址不在备份MAC地址列表

    STA_SET_PRODUCT_TYPE,               //站点设置产品类型不同
    NDM_METER_ACCESS,                   //抄控器整表测试强制连接
    NDM_METER_CHECK_DISCONNECT,         //抄控器整表测试断开连接
    PLC_SWITCH_TO_RF,                   // PLC网络模式切换到RF模式
    RF_SWITCH_TO_PLC,                   // RF网络模式切换到PLC模式

    TF_IDENTIFY_RESET = 21,             //台区识别重启
    TF_IDENTIFY_CHECK_SUPPORT_RESET,    // STA检测支持台区识别，并且已经发起关联请求，则进行软重启
    NM_ENTER_NETWORK_TEST_MODE_RESET,   // MAC层进入网络测试模式
    NM_EXIT_NETWORK_TEST_MODE_RESET,    // MAC层退出网络测试模式
    NM_RCV_NETWORK_CONFLICT_PK,         // 接收到网络冲突报文

    STA_EXIT_NON_ASSOCIATE = 26,        // STA退出非组网状态
    BBPLC_SWITCH_TO_SCPLC,              // 宽带PLC网络模式切换到单载波PLC网络模式
    SCPLC_SWITCH_TO_BBPLC,              // 单载波PLC网络模式切换到宽带PLC网络模式
    ENTER_PHY_TEST_MODE,                // 国网测试系统进入物理层测试模式
    EXIT_PHY_TEST_MODE,                 // 国网测试系统推迟物理层测试模式

    NETWORK_MODE_SWITCH = 31,           // 网络模式切换
    NETWORK_PTCL_SWITCH,                // 网络协议切换
    REJOIN_CALL_FROM_APP_LAYER,         // APP层调用重新入网接口
    CHANGE_LOCK_NETWORK,                // 改变锁定的网络，短期收敛
    DEVICE_POWER_FAILURE_RECOVER,       // 停电恢复

    CHANGE_LOCK_NETWORK_LONG = 36,      // 改变锁定的网络，长期收敛
    CHANGE_LOCK_NETWORK_LONG_LONG,      // 改变锁定的网络，超长期收敛
    CHANGE_TO_BELONG_NETWORK,           // 离开当前网络，去关联正确归属网络
    CHANGE_NOT_JOIN_LOCK_NET,           // 发现加入的不是锁定的网络
    CHANGE_LOCK_NETWORK_HSO,            // HSO设置锁定网络

    CHANGE_LOCK_NETWORK_NTB_CRT = 41,   // 改变锁定的网络，NTB修正策略
    STA_NOT_RCV_LEAVE_IND,              // 能源模式下，STA站点没有收到离线报文，并且满载，没有被锁定，则离线

    TOPO_TYPE_CHANGE_RESET,             // 组网类型变化触发MAC层软复位

    MAC_REJOIN_REASON_INVALID = 0xff    // 无效的软重启原因
};

typedef struct
{
   HI_U32   bpc;
   HI_U8    reason_code;  //   原因
   HI_U8    rsvd[3];
}DIAG_CMD_REPORT_LEAVE_NETWORK_STRU;

#define     VIOLENCE_STA_CNT_PER_PK      50  //每包最大决绝列表个数
typedef struct
{
   HI_U8 mac[VIOLENCE_STA_CNT_PER_PK][HI_PLC_MAC_ADDR_LEN];     // 拒绝站点MAC地址
}DIAG_CMD_QUERY_VIOLENCE_STA_STRU;

#define    MAX_VIOLENCE_STA_COUNT      200      //最大拒绝列表数
#define MAC_PLC_QUERY_VIOLEDNCE_VER_R0  0 // 定义拒绝列表查询版本号
#define MAC_PLC_QUERY_VIOLEDNCE_VER_R1  1 // 定义拒绝列表查询版本号

typedef struct
{
   HI_U8 mac[HI_PLC_MAC_ADDR_LEN];
   HI_U8 pad[2];
}DIAG_CMD_QUERY_VIOLENCE_STA_MAC_STRU;

typedef struct
{
   HI_U32  interface_ver;           // 版本号
   HI_U16  sn;                      // 包序号
   HI_BOOL is_last;                 // 是否最后一个包
   HI_U8   num;                     // 本次上报的个数
   DIAG_CMD_QUERY_VIOLENCE_STA_MAC_STRU mac[VIOLENCE_STA_CNT_PER_PK];
}DIAG_CMD_QUERY_MAX_VIOLENCE_STA_STRU;

typedef struct
{
    HI_U16  whitelist_num;                       //白名单中的站点数
    HI_BOOL mac_authentication;                  //白名单开关状态
    HI_U8   reason;                              //白名单变化的原因
    HI_U32  pad2;
}DIAG_CMD_REPORT_WHITELIST_CHANGED;


/***
 * MAC层速率低于门限值时上报当前速率
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
    HI_U32 ndm_set_pk_cnt;          // ndm 下发报文总数
    HI_U32 ndm2nm_pk_cnt;           // ndm 下发报文到nm模块进行分发的个数

    HI_U32 nm2cvg_pk_cnt;           // ndm下发报文通过nm发送到汇聚的总数
    HI_U32 cvg2seg_pk_cnt;          // 汇聚接收到ndm报文的总数
    HI_U32 seg_in_queue_pk_cnt;     // 分段入队列ndm报文的总数
    HI_U32 seg_in_queue_fail_cnt;   // 报文入分段队列失败的总数

    HI_U32 tx_all_pk_cnt;           // 发送机收到ndm报文的总数
    HI_U32 tx_send_success_cnt;     // 发送机发送成功的ndm报文的总数
    HI_U32 tx_send_fail_cnt;        // 发送机发送失败的ndm报文的总数

    HI_U32 tx_send_timeout_cnt;     // 发送机发送超时的ndm报文的总数
    HI_U32 tx_resend_cnt;           // 发送机重发的ndm报文的总数

    HI_U32 rx_rcv_pk_cnt;           // 接收机接收到ndm报文总数
    HI_U32 rsb2cvg_pk_cnt;          // 重组分发的汇聚ndm报文的总数
    HI_U32 rsb_timeout_cnt;         // 重组超时报文数
    HI_U32 rsb_filter_pk_cnt;       // 重组中被过滤掉的ndm报文的个数
    HI_U32 cvg2nm_pk_cnt;           // 汇聚分发到nm模块的ndm报文的总数
    HI_U32 nm_filter_pk_cnt;        // 网管过滤掉的报文数
    HI_U32 nm2ndm_pk_cnt;           // nm模块消息处理分发到ndm的ndm报文总数
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
/*         5    统计类                                                  */
/************************************************************************/
//查询请求命令的默认参数
typedef struct
{
    HI_U32 clearSwitch;
}DIAG_CMD_QUERY_REQ_PARA_STRU;

/**
 *
 * 5.1    DFX周期查询后主动上报发送统计数据（HSO, console）
 *
 */
typedef struct
{
    HI_U32 rts_cts_count;                       // 发送rts帧个数
    HI_U32 sack_count;                          // 发送sack帧个数
    HI_U32 beacon_count;                        // 发送信标帧个数
    HI_U32 sounding_count;                      // 发送sound帧个数

    HI_U32 unicast_count;                       // 单播报文个数
    HI_U32 broadcast_count;                     // 广播报文个数

    HI_U32 short_packet_count;                  // 发送短包个数
    HI_U32 long_packet_count;                   // 发送长包个数
    HI_U32 pb72_packet_count;                   // 发送pb72个数
    HI_U32 pb264_packet_count;                  // 发送pb264个数

    HI_U32 collision_count;                     // 发送冲突次数
    HI_U32 tx_get_all_msdu_cnt;                 // 发送机成功获取msdu的总数
    HI_U32 tx_get_nm_cnt;                       // 获取网管报文数
    HI_U32 tx_get_ip_cnt;                       // 获取IP报文数
    HI_U32 tx_get_app_cnt;                      // 获取APP报文数
    HI_U32 tx_get_upg_cnt;                      // 获取升级报文数

    HI_U32 tx_send_msdu_success;                // 发送机成功发送msdu的总数
    HI_U32 tx_send_nm_success;                  // 发送网管报文成功次数
    HI_U32 tx_send_ip_success;                  // 发送IP报文成功次数
    HI_U32 tx_send_app_success;                 // 发送APP报文成功次数
    HI_U32 tx_send_upg_success;                 // 发送升级报文成功次数


    HI_U32 tx_send_msdu_timeout;                // 发送机发送超时msdu的总数
    HI_U32 tx_send_nm_timeout;                  // 发送网管报文超时次数
    HI_U32 tx_send_ip_timeout;                  // 发送IP报文超时次数
    HI_U32 tx_send_app_timeout;                 // 发送APP报文超时次数
    HI_U32 tx_send_upg_timeout;                 // 发送升级报文超时次数

    HI_U32 tx_send_msdu_fail;                   // 发送机发送失败msdu的总数
    HI_U32 tx_send_nm_fail;                     // 发送网管报文失败次数
    HI_U32 tx_send_ip_fail;                     // 发送IP报文失败次数
    HI_U32 tx_send_app_fail;                    // 发送APP报文失败次数
    HI_U32 tx_send_upg_fail;                    // 发送升级报文失败次数

    HI_U32 tx_send_msdu_resend;                 // 发送机重发msdu的次数
    HI_U32 tx_send_nm_resend;                   // 发送网管报文重传次数
    HI_U32 tx_send_ip_resend;                   // 发送IP报文重传次数
    HI_U32 tx_send_app_resend;                  // 发送APP报文重传次数
    HI_U32 tx_send_upg_resend;                  // 发送升级报文重传次数

    HI_U32 sof_wait_sack_send_success;          // 等待SACK SOF发送成功个数
    HI_U32 sof_not_wait_sack_send_success;      // 不需要等待SACK报文发送成功个数
    HI_U32 sof_resend_exclude_fix_resend;       // 报文重传个数，不包括固定重传

    HI_U32 no_upg_slot_send;                    // 无bcsma时隙发送升级报文的个数
    HI_U32 tx_send_wait_end_interrupt_timeout;  // 等发送完成中断超时次数
    HI_U32 tx_state_abnormal_cnt;               // 发送状态异常次数
}DIAG_CMD_QUERY_TX_STATICS_STRU;

/**
 *
 * 5.2    DFX周期查询后主动上报接收统计数据（HSO, console）
 *
 */

typedef struct
{
    HI_U32 fc_count;                        // FC中断次数           FC中断中统计
    HI_U32 fc_crc_success_count;            // FC校验正确次数     FC中断中统计
    HI_U32 fc_crc_err_count;                // FC校验错误次数     FC中断中统计 （fccs校验错误，不触发fc中断后，则该统计项为0）
    HI_U32 fc_local_count;                  // 本地FC总数         beacon：sof：sack：sound：rts_cts:

    HI_U32 false_frame_count;               // 虚捡个数
    HI_U32 not_expect_dt_count;             // 不是预期帧的个数（碰撞产生）

    HI_U32 beacon_count;                    // 本地beacon帧个数
    HI_U32 sof_count;                       // 本地sof帧个数
    HI_U32 sack_count;                      // 本地sack帧个数
    HI_U32 rts_count;                       // 本地rts帧个数
    HI_U32 cts_count;                       // 本地cts帧个数
    HI_U32 sound_count;                     // 本地sound帧个数
    HI_U32 sound_sack_count;                // 本地sound sack个数

    HI_U32 coordinate_frame_count;          // 多网络协调帧个数
    HI_U32 pcs_interrupt_cnt;               // PCS中断次数
    HI_U32 parallel_ok_cnt;                 // 并行检测中断OK个数
    HI_U32 beacon_pb_crc_err_count;         // beacon pb校验失败总数
    HI_U32 beacon_pb_crc_success_count;     // beacon pb校验成功总数

    HI_U32 broadcast_count;                 // 本地广播报文个数    sof:            bcf
    HI_U32 unicast_count;                   // 本地单播报文个数    sof:        bcf
    HI_U32 resend_count;                    // 本地重传报文个数     sof:判断重传标志 resend_packet
    HI_U32 pb_cnt[4];                       // 接收不同PB块报文个数

    HI_U32 short_pb_crc_err_count;          // 136pb块crc校验错误数
    HI_U32 long_pb_crc_err_count;           // 520pb块crc校验错误数
    HI_U32 pb72_crc_err_count;              // 72pb块crc校验错误数
    HI_U32 pb264_crc_err_count;             // 264pb块crc校验错误数

    HI_U32 short_pb_count;                  // 136pb块个数   pb块中断
    HI_U32 long_pb_count;                   // 520pb块个数
    HI_U32 pb72_count;                      // 72pb块个数   pb块中断
    HI_U32 pb264_count;                     // 264pb块个数   pb块中断

    HI_U32 discard_packet_count;            // 丢弃报文
    HI_U32 no_pb_packet_count;              // 接收未完成，fc扫频 无PB块的报文个数
    HI_U32 time_out_count;                  // 报文在接收队列超时计数
}DIAG_CMD_QUERY_RX_STATICS_STRU;
typedef struct
{
    HI_U32 ssn_err_count;                   // ssn号异常个数
    HI_U32 send_sof_to_rsb_count;           // 发送到重组器任务SOF帧个数
    HI_U32 rx_lisr_count;                   // 接收机中断产生次数
    HI_U32 rx_hisr_count;                   // 接收机高级中断响应次数
    HI_U32 sack_success_count;              // 产生SACK帧内容为接收成功的个数
    HI_U32 sack_fail_count;                 // 产生SACK帧内容为接收失败的个数
    HI_U32 discard_beacon_count;            // 丢弃信标帧的个数
    HI_U32 discard_sounding_count;          // 丢弃SOUNDING帧的个数
    HI_U32 discard_sounding_ack_count;      // 丢弃SOUNDING ACK帧的个数
    HI_U32 discard_sack_count;              // 丢弃SACK帧的个数
    HI_U32 tx_send_cnt;                     // 发送次数
    HI_U32 burst_send_end_cnt;              // 发送完成中断次数
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
    HI_U32 tmi6_send_cnt;                   // TMI6发送报文次数
    HI_U32 tmi4_send_cnt;                   // TMI4发送报文次数
    HI_U32 tmi1_send_cnt;                   // TMI1发送报文次数
    HI_U32 tmi0_send_cnt;                   // TMI0发送报文次数
    HI_U32 tmi9_send_cnt;                   // TMI9发送报文次数
    HI_U32 tmi11_send_cnt;                  // TMI11发送报文次数
    HI_U32 tmi12_send_cnt;                  // TMI12发送报文次数
    HI_U32 tmi13_send_cnt;                  // TMI13发送报文次数
    HI_U32 tmi14_send_cnt;                  // TMI14发送报文次数
    HI_U32 sounding_send_cnt;               // 通过SOUNDING发送的次数
    HI_U32 backoff_fail_cnt;                // 退避失败次数
    HI_U32 send_event_cnt;                  // 发送机接收事件次数
    HI_U32 handle_event_cnt;                // 发送机处理事件个数
    HI_U32 tdma_period_refreshed_cnt;       // 时隙更新时间个数
    HI_U32 mpdu_queue_in_cnt;               // 报文入队列事件个数
    HI_U32 tx_en_send_fail_cnt;             // 使能PHY发送失败次数
    HI_U32 channel_busy_cnt;                // 发送前信道变忙次数
    HI_U32 tmi6_send_fail_cnt;              // TMI6发送报文失败次数
    HI_U32 tmi4_send_fail_cnt;              // TMI4发送报文失败次数
    HI_U32 tmi1_send_fail_cnt;              // TMI1发送报文失败次数
    HI_U32 tmi0_send_fail_cnt;              // TMI0发送报文失败次数
    HI_U32 tmi9_send_fail_cnt;              // TMI9发送报文失败次数
    HI_U32 tmi11_send_fail_cnt;             // TMI11发送报文失败次数
    HI_U32 tmi12_send_fail_cnt;             // TMI12发送报文失败次数
    HI_U32 tmi13_send_fail_cnt;             // TMI13发送报文失败次数
    HI_U32 tmi14_send_fail_cnt;             // TMI14发送报文失败次数
    HI_U32 tx_send_bea_fail_cnt;            // 信标由于时隙不足发送失败次数，不包括信道忙
    HI_U32 tx_send_bea_fail_by_busy_cnt;    // 信道忙，导致发送信标失败计数
}DIAG_CMD_QUERY_TX_DIAGNOSTIC_STRU;
typedef struct
{
    HI_U32 inital_sounding_req_cnt;              // 申请发起静态SOUNDING次数
    HI_U32 initial_sounding_cnt;                 // 发起静态SOUNDING次数
    HI_U32 dynamic_sounding_cnt;                 // 动态SOUNDING次数
    HI_U8  phy_in;
    HI_U8  pad[3];
}DIAG_CMD_QUERY_SOUND_STATICS_STRU;
typedef struct _DIAG_CMD_PLC_DEV_STAT_STRU_
{
    HI_U32 output_msdu_size_err_cnt;                 // 发送IP报文时报文大小异常次数
    HI_U32 output_get_mac_buffer_fail_cnt;           // 发送IP报文时申请MAC层BUFFER失败次数
    HI_U32 output_cvg_sgm_fail_cnt;                  // 发送IP报文时汇聚分段处理失败次数
    HI_U32 output_not_ip_cnt;                        // 发送IP报文时不是IP报文的次数
    HI_U32 output_send_cnt;                          // 发送IP报文次数
    HI_U32 input_msdu_size_err_cnt;                  // 接收IP报文时报文大小异常次数
    HI_U32 input_get_ip_buffer_fail_cnt;             // 接收IP报文时获取IP层BUFFER失败次数
    HI_U32 input_ip_broadcast_cnt;                   // 接收IP报文中广播IP报文的个数
    HI_U32 input_broadcast_not_my_packet_cnt;        // 接收IP报文时广播报文中非本站点报文个数
    HI_U32 input_broadcast_pk_my_packet_cnt;         // 接收IP报文时广播报文中本站点报文个数
    HI_U32 input_unicast_my_packet_cnt;              // 接收IP报文时单播报文本站点报文个数
    HI_U32 input_unicast_not_my_packet_cnt;          // 接收IP报文时单播报文非本站点报文个数
} DIAG_CMD_PLC_DEV_STAT_STRU;
typedef struct
{
    HI_U32 tx_retranfer_packet_count;            // 汇聚器交给发送机的转发报文个数
    HI_U32 tx_ip_packet_count;                   // 汇聚器交给发送机ip报文个数
    HI_U32 tx_network_packet_count;              // 汇聚器交给发送机网管报文个数
    HI_U32 tx_app_packet_count;                  // 汇聚器交给发送机应用报文个数
    HI_U32 tx_ndc_packet_count;                  // 汇聚器交给发送机NDC报文个数
    HI_U32 tx_unknown_packet_count;              // 汇聚器收到要发送的未知报文

    HI_U32 rx_retranfer_packet_count;            // 汇聚器从接收机收到转发报文个数
    HI_U32 rx_network_packet_count;              // 汇聚器从接收机收到的网管报文个数
    HI_U32 rx_ip_packet_count;                   // 汇聚器从接收机收到的ip报文个数
    HI_U32 rx_app_packet_count;                  // 汇聚器从接收机收到的app报文个数
    HI_U32 rx_unknown_packet_count;              // 汇聚器从接收机收到的未知报文个数
    HI_U32 rx_upg_packet_count;                  // 汇聚器从接收机收到的升级报文个数
    HI_U32 rx_ndc_packet_count;                  // 汇聚器从接收机收到的NDC报文个数

    HI_U32 unicast_discard_count;                // 单播报文丢弃个数：非本站点报文
    HI_U32 unicast_retransfer_count;             // 单播报文转发个数
    HI_U32 broadcast_discard_count;              // 广播报文丢弃个数
    HI_U32 broadcast_retransfer_count;           // 广播报文转发个数
    HI_U32 broadcast_retransfer_get_buf_fail_count; // 广播报文转发报文申请buf失败个数

    HI_U32 query_route_fail_count;               // 路由查询失败
    HI_U32 get_tei_by_mac_fail_count;            // 通过MAC地址获取TEI失败次数

    HI_U32 app_dequeue_buf_fail_count;           // 应用层报文入队列失败个数
    HI_U16 app_pk_msdu_size_err_count;           // 应用层报文大小参数异常次数
    HI_U16 set_app_send_info_fail_count;         // 设置应用层报文发送信息失败次数
    HI_U16 app_vlantag_err_count;                // 应用层报文优先级异常个数
    HI_U16 send_pk_to_app_fail_count;            // 发送报文到应用层失败个数
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
    HI_U32 ntb_stat_snid;                       // 当前NTB统计周边站点接收情况SNID号

    HI_U8  ntb_state;                           // 当前NTB处于的状态
    HI_U8  ntb_lock_state;                      // 是否锁定同步对象
    HI_U8  rsv[2];

    HI_U32 ntb_synch_snid;                      // 锁定同步对象的网络号

    HI_U16 ntb_synch_tei;                       // 锁定同步对象的TEI
    HI_U16 ntb_stat_tei;                        // 当前NTB统计到最佳TEI

    HI_U32 ntb_current_sys_time;                // 当前系统时间
    HI_U32 ntb_soft_control_begin_time;         // 软件控制时钟同步开始时间
    HI_U32 ntb_last_synch_time;                 // NTB最后同步时间
    HI_U32 ntb_lock_synch_station_begin_time;   // 锁定同步对象开始时间
    HI_U32 ntb_chip_control_begin_time;         // 芯片控制时钟同步开始时间

    HI_U32 ntb_soft_control_len;                // 软件控制时长

    HI_S16 freqerr_current;                     // 当前频偏
    HI_S16 freqerr_front;                       // 工作周期统计的频偏
    HI_S16 freqerr_sdram;                       // 存储到FLASH中的频偏值
    HI_S16 pad;                                 // 当前频偏
}DIAG_CMD_QUERY_NTB_STATE_STRU;

#define NTB_MAX_FREQERR_CNT                     (36)
typedef struct
{
    HI_U32 freqerr[NTB_MAX_FREQERR_CNT];
}DIAG_CMD_QUERY_NTB_FREQERR_STAT_STRU;
#define NTB_SYNCH_STATION_STAT_NUM      (10)        // 统计同步对象站点信息的缓存为10组，循环使用缓存
typedef struct
{
    HI_U16 synch_tei;           // 同步站点
    HI_U8  synch_snid;          // 同步网络号
    HI_U8  chip_state;          // 芯片时钟同步状态
    HI_S16 max_freqerr;         // 同步频偏最大值;
    HI_S16 min_freqerr;         // 同步频偏最小值
    HI_U32 synch_systime;       // 同步系统时间

    HI_U8  reset_reason;        // 复位原因
    HI_U8  pad[3];
}DIAG_CMD_NTB_SYNCH_STATION_INFO_STRU;
typedef struct
{
    HI_U8  index;               // 当前同步站点存储的索引
    HI_U8  pad[3];
    DIAG_CMD_NTB_SYNCH_STATION_INFO_STRU synch_station_stat[NTB_SYNCH_STATION_STAT_NUM];
} DIAG_CMD_QUERY_NTB_SYNCH_STATION_STRU;

typedef struct
{
    HI_U16 tei                      : 12;
    HI_U16 pad                      : 4;
    HI_S16 freqerr;                             // 配置频偏

    HI_U32 snid;

    HI_U8  freq;
    HI_U8  pad1                     : 1;
    HI_U8  sync_status              : 3;        // 该同步对象的同步状态
    HI_U8  freqerr_exception_cnt    : 4;        // 频偏异常次数
    HI_U16 pad2;

    HI_U32 last_update_timestamp;
}DIAG_NTB_NEIGHBOR_FREQERR_STRU;

#define NTB_MULTI_NET_CNT (32)                              //NTB模块支持的多网络个数
typedef struct
{
    DIAG_NTB_NEIGHBOR_FREQERR_STRU neighbor[NTB_MULTI_NET_CNT];
}DIAG_NTB_NEIGHBOR_FREQERR_STAT_STRU;

#define RX_STAT_NUM     (16)
typedef struct
{
    HI_U32 beacon_fc[RX_STAT_NUM];              // 统计接收不同网络信标帧FC个数
    HI_U32 sof_fc[RX_STAT_NUM];                 // 统计接收不同网络数据帧FC的个数
}DIAG_CMD_QUERY_MULTI_NETWORK_RCV_STRU;

//V200多网络接收统计
typedef struct
{
    HI_U32 snid[RX_STAT_NUM];
    HI_U32 bcn_fc[RX_STAT_NUM];
    HI_U32 sof_fc[RX_STAT_NUM];
    HI_U32 last_rcv_time[RX_STAT_NUM];
    HI_U16 store_overflow_cnt;                      //存储溢出计数
    HI_U16 pad1;

    HI_U32 store_overflow_snid;                     //存储溢出的最后一次记录的Snid
}DIAG_CMD_MULTI_NETWORK_RCV_V200_STRU;

typedef struct
{
    HI_U32  phase_a_total_rcv_cnt;              // A相位上接收CCO BEACON帧总个数
    HI_U32  phase_b_total_rcv_cnt;              // B相位上接收CCO BEACON帧总个数
    HI_U32  phase_c_total_rcv_cnt;              // C相位上接收CCO BEACON帧总个数

    HI_U32  phase_a_rcv_cnt_last;               // A相位上前一个周期接收CCO BEACON帧总个数
    HI_U32  phase_b_rcv_cnt_last;               // B相位上前一个周期接收CCO BEACON帧总个数
    HI_U32  phase_c_rcv_cnt_last;               // C相位上前一个周期接收CCO BEACON帧总个数
    HI_U32  phase_a_channel_last;               // A相位上前一个周期信道容量值
    HI_U32  phase_b_channel_last;               // B相位上前一个周期信道容量值
    HI_U32  phase_c_channel_last;               // C相位上前一个周期信道容量值

    HI_U32  phase_a_rcv_cnt_current;            // A相位上当前周期接收CCO BEACON帧个数
    HI_U32  phase_b_rcv_cnt_current;            // B相位上当前周期接收CCO BEACON帧个数
    HI_U32  phase_c_rcv_cnt_current;            // C相位上当前周期接收CCO BEACON帧个数
    HI_U32  phase_a_channel_current;            // A相位上当前周期信道容量值
    HI_U32  phase_b_channel_current;            // B相位上当前周期信道容量值
    HI_U32  phase_c_channel_current;            // C相位上当前周期信道容量值

    HI_U32  evaluate_phase_a_cnt;               // 评估出A相位次数
    HI_U32  evaluate_phase_b_cnt;               // 评估出B相位次数
    HI_U32  evaluate_phase_c_cnt;               // 评估出C相位次数

    HI_U8   phase[3];                           // 当前评估出相位排序
    HI_U8   frist_evaluate_cycle_is_end;        // 第一个评估周期是否结束

    HI_S8   phase_a_snr;                        //a相位的snr
    HI_S8   phase_b_snr;                        //b相位的snr
    HI_S8   phase_c_snr;                        //c相位的snr
    HI_BOOL phs_collect_info_finish;

    HI_U32   phase_a_send_success_cnt;                        //a相位上发送成功的次数
    HI_U32   phase_b_send_success_cnt;                        //b相位上发送成功的次数
    HI_U32   phase_c_send_success_cnt;                        //c相位上发送成功的次数

    HI_U32   phase_a_send_fail_cnt;                        //a相位上发送失败的次数
    HI_U32   phase_b_send_fail_cnt;                        //b相位上发送失败的次数
    HI_U32   phase_c_send_fail_cnt;                        //c相位上发送失败的次数

    HI_U32   phase_a_last_send_success_cnt;                        //a相位上发送成功的次数
    HI_U32   phase_b_last_send_success_cnt;                        //b相位上发送成功的次数
    HI_U32   phase_c_last_send_success_cnt;                        //c相位上发送成功的次数

    HI_U32   phase_a_last_send_fail_cnt;                        //a相位上发送失败的次数
    HI_U32   phase_b_last_send_fail_cnt;                        //b相位上发送失败的次数
    HI_U32   phase_c_last_send_fail_cnt;                        //c相位上发送失败的次数

    HI_U8    phase_a_set_send_cnt;                      //a相位设定的发送次数
    HI_U8    phase_b_set_send_cnt;                      //b相位设定的发送次数
    HI_U8    phase_c_set_send_cnt;                      //c相位设定的发送次数
    HI_U8    pad1;
}DIAG_CMD_QUERY_PHASE_STATICS_STRU;

typedef struct
{
    HI_U32 nm_leave_sta_num;                                // CCO统计离线指示站点个数
    HI_U32 nm_send_delay_leave_pk_cnt;                      // CCO发送延迟离线指示报文个数
    HI_U32 nm_send_short_delay_leave_pk_fail_cnt;           // CCO发送延迟离线指示报文失败个数
    HI_U32 nm_send_leave_ind_white_cnt;                     // CCO发送白名单产生离线指示报文次数
    HI_U32 nm_send_leave_ind_cnt;                           // CCO发送离线指示报文个数
    HI_U32 nm_rcv_delay_leave_pk_cnt;                       // 站点接收延迟离线指示报文个数
    HI_U32 nm_rcv_last_delay_leave_pk_time;                 // 站点最后接收延迟离线指示报文系统时间
    HI_U32 nm_handle_last_delay_leave_pk_time;              // 站点处理延迟离线指示报文系统时间
    HI_U32 nm_delay_leave_cnt;                              // 站点延迟离线次数
    HI_U32 nm_delay_leave_time;                             // 站点延迟离线系统时间
    HI_U32 nm_handle_leave_ind_white_cnt;                   // 站点处理白名单踢出离线指示报文次数
    HI_U32 nm_handle_leave_ind_cnt;                         // 站点处理非白名单离线指示报文次数
}DIAG_CMD_QUERY_DELAY_LEAVE_IND_INFO_STRU;

typedef struct
{
    HI_U32 create_proxy_change_req_cnt;                     // 创建代理变更请求报文次数
    HI_U32 handle_proxy_change_cnf_cnt;                     // 处理代理变更回复报文次数
    HI_U32 handle_proxy_change_bitmap_cnf_cnt;              // 处理比特图代理变更回复报文次数
    HI_U32 proxy_change_timeout_cnt;                        // 代理变更请求超时次数
    HI_U32 proxy_change_cnf_sequence_err_cnt;               // 代理变更回复序号异常次数

    HI_U16 candinate_proxy_tei[DFX_CHOSE_PROXY_SUM];        // 当前候选代理
    HI_U16 proxy_tei;                                       // 当前代理站点

    HI_U16 no_return_cnt;                                   // 当前代理变更未返回次数
    HI_U8  proxy_changing;                                  // 当前代理变更状态
    HI_U8  pad;
}DIAG_CMD_QUERY_PROXY_CHANGE_PK_STAT_STRU;

typedef struct
{
    HI_U32 nm_rcv_assoc_req_cnt;                    // 接收关联请求个数
    HI_U32 nm_assoc_req_sequence_err_cnt;           // 关联请求序号错误个数
    HI_U32 nm_assoc_req_network_form_seq_err_cnt;   // 关联请求组网序号错误个数
    HI_U32 nm_send_assoc_cnf_cnt;                   // 发送关联回复个数
    HI_U32 nm_create_assoc_ind_cnt;                 // 发送关联指示报文个数
    HI_U32 nm_create_assoc_gather_ind_cnt;          // 汇总报文个数
    HI_U32 nm_resend_assoc_ind_cnt;                 // 重复发送关联请求报文个数
    HI_U32 nm_rcv_proxy_change_req;                 // 接收代理变更请求个数
    HI_U32 nm_send_proxy_change_cnf;                // 发送代理变更回复报文个数
    HI_U32 nm_send_proxy_change_bitmap_cnf;         // 比特图代理变更回复报文个数
    HI_U32 nm_proxy_change_sequence_err_cnt;        // 代理变更请求序号错误个数
    HI_U32 nm_handle_proxy_change_fail_cnt;         // 处理代理变更请求失败个数
    HI_U32 nm_get_mac_buffer_fail_cnt;              // 网络管理申请MAC BUFFER失败次数
    HI_U32 nm_send_msg_fail_cnt;                    // 网络管理模块发送消息失败个数
    HI_U32 nm_send_msg_enforcing_proxy_change_fail; // 发送强制代理变更消息失败计数
}DIAG_CMD_QUERY_ASSOC_REQ_PK_STAT_STRU;

typedef struct
{
    HI_U32  rsb_send_event_cnt;          // 接收机发送重组事件个数
    HI_U32  rsb_handle_event_cnt;        // 重组器处理重组事件个数
    HI_U32  rsb_handle_pk_cnt;           // 重组器处理报文个数
    HI_U32  rsb_reassemble_fail_cnt;     // 报文重组失败个数
    HI_U32  msdu_size_err_cnt;           // 报文大小非法计数
    HI_U32  rsb_icv_rcv_cnt;             // 报文MAC校验成功个数
    HI_U32  rsb_not_my_pk_cnt;           // 非本站点报文个数
    HI_U32  rsb_filter_repeat_pk_cnt;    // 报文过滤重复报文个数
    HI_U32  rsb_filter_rcv_pk_cnt;       // 未过滤的正常报文个数
    HI_U32  rsb_deliver_pk_fail_cnt;     // 重组器分发报文失败个数
    HI_U32  rsb_deliver_pk_cnt;          // 重组器分发报文个数
    HI_U32  rsb_list_pk_cnt;             // 重组器链表中报文个数
    HI_U32  rsb_list_pk_max_cnt;         // 重组器链表中报文峰值个数
}DIAG_CMD_QUERY_RSB_STATICS_PK_STRU;

typedef struct
{
    HI_U8  all_queue_pk_cnt;               // 分段队列报文总数
    HI_U8  queue0_pk_cnt;                  // 队列0报文个数
    HI_U8  queue1_pk_cnt;                  // 队列1报文个数
    HI_U8  queue2_pk_cnt;                  // 队列2报文个数

    HI_U8  queue3_pk_cnt;                  // 队列3报文个数
    HI_U8  upg_queue_cnt;                  // 升级队列报文个数
    HI_U16 msdu_len_err_cnt;               // msdu长度错误

    HI_U32 queue0_overflow_cnt;            // 队列0溢出次数
    HI_U32 queue1_overflow_cnt;            // 队列1溢出次数
    HI_U32 queue2_overflow_cnt;            // 队列2溢出次数
    HI_U32 queue3_overflow_cnt;            // 队列3溢出次数
    HI_U32 upg_queue_overflow_cnt;         // 升级报文队列溢出次数

    HI_U32 segment_sof_cnt;                // 对SOF帧分段次数
    HI_U32 beacon_enqueue_cnt;             // 信标帧入分段队列次数
    HI_U32 sounding_enqueue_cnt;           // SOUNDING帧入队列次数
    HI_U32 tx_set_is_sending_cnt;          // 发送机设置报文处于发送状态次数
    HI_U32 tx_set_no_send_cnt;             // 发送机设置报文处于非发送状态次数
    HI_U32 tx_set_discard_cnt;             // 发送机设置丢弃报文次数

    HI_U32 detect_beacon_enqueue_cnt;      // 探测信标入队列次数
    HI_U32 segment_split_mac_fail_cnt;     // 报文分片错误次数
    HI_U32 segment_set_fc_fail_cnt;        // 报文设置FC错误次数
    HI_U32 segment_queue_overflow_cnt;     // 报文入队列溢出次数

    HI_U32 pad;                            // 报文分段成功次数
    HI_U32 send_event_to_tx_cnt;           // 通知发送机报文入队列次数
}DIAG_CMD_QUERY_SEG_STATICS_PK_STRU;

typedef struct
{
    HI_U16 port;                                    // 端口号
    HI_U8  eth_mac[HI_ETH_MAC_ADDR_LEN];            // 以太网MAC地址
    HI_U32 aging_time;                              // 老化时间

    HI_U32 rcv_cnt;                                 // 接收报文个数
    HI_U32 snd_cnt;                                 // 发送报文个数
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
    HI_U16 port;                                    // 端口号
    HI_U16 sequence;                                // 报文序号

    HI_U8  fragment_num;                            // 分片数
    HI_U8  fragment_state;                          // 分片重组状态
    HI_U8  pad[2];

    HI_U32 expired_time;                            // 分片过期时间
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
    HI_U32 get_port_fail_cnt;                       // 获取端口失败个数
    HI_U32 snd_broadcast_cnt;                       // 发送广播报文个数
    HI_U32 snd_unicast_cnt;                         // 发送单播报文个数
    HI_U32 get_port_fail_broadcast_cnt;             // 映射未建立，广播报文个数
    HI_U32 fragment_cnt[4];                         // 分片个数
    HI_U32 get_mac_buffer_fail;                     // 获取MAC层缓存失败个数
    HI_U32 switch_not_start_pk_cnt;                 // 交换机未启动时，到达交换机的报文个数
    HI_U32 rcv_broadcast_cnt;                       // 接收广播报文个数
    HI_U32 rcv_unicast_cnt;                         // 接收单播报文个数
    HI_U32 fragment_rsb_success_cnt[3];             // 重组成功个数
    HI_U32 fragment_rsb_fail_cnt[3];                // 重组失败个数
    HI_U32 get_fragment_entry_fail_cnt;             // 获取分片缓存失败次数
    HI_U32 send_to_eth_success_cnt;                 // 发送到以太网口成功个数
    HI_U32 send_to_eth_fail_cnt;                    // 发送到以太网口失败个数
    HI_U32 eth_data_len_invalid_cnt;                // 以太网报文长度异常次数
    HI_U32 send_to_net_protocol_stack_cnt;          // 发送到本地网络协议栈的报文个数
    HI_U32 send_to_plc_link_layer_cnt;              // 转发到电力线链路上报文个数
    HI_U32 multicast_pk_cnt;                        // 组播报文个数
    HI_U32 tx_get_switch_cnt;                       // 发送机获取交换报文个数
    HI_U32 tx_send_switch_success;                  // 发送机发送交换报文成功个数
    HI_U32 tx_send_switch_timeout;                  // 发送机发送交换报文超时个数
    HI_U32 tx_send_switch_fail;                     // 发送机发送交换报文失败个数
    HI_U32 tx_send_switch_resend;                   // 发送机发送交换报文重传次数
    HI_U32 cvg_to_tx_eth_packet_count;              // 汇聚交给发送机的ETH报文个数
    HI_U32 rx_to_cvg_eth_packet_count;              // 接收机交给汇聚的ETH报文个数
}DIAG_CMD_QUERY_PLC_SWITCH_TRX_STAT_STRU;

typedef struct
{
    HI_U32 arp_request_cnt;             // ARP请求报文个数
    HI_U32 arp_response_cnt;            // ARP响应报文个数

    HI_U32 ip_cnt;                      // IP报文个数
    HI_U32 ohter_three_layer_cnt;       // 三层其他协议报文

    HI_U32 tcp_cnt;                     // TCP报文个数
    HI_U32 udp_cnt;                     // UDP报文个数
    HI_U32 icmp_cnt;                    // ICMP报文个数
    HI_U32 igmp_cnt;                    // IGMP报文个数
    HI_U32 other_four_layer_cnt;        // 四层其他协议报文
}DIAG_CMD_QUERY_ETH_PACKET_STAT_STRU;

typedef struct
{
    DIAG_CMD_QUERY_ETH_PACKET_STAT_STRU eth_link_layer;
    DIAG_CMD_QUERY_ETH_PACKET_STAT_STRU plc_link_layer;
}DIAG_CMD_QUERY_PLC_SWITCH_ETH_PACKET_STAT_STRU;

typedef struct
{
    HI_U32 proxy_send_pk_cnt;           // 代理发送报文个数
    HI_U32 cco_receive_pk_cnt;          // CCO接收报文个数
}DIAG_CMD_QUERY_COMM_RATE_STRU;

typedef struct
{
    HI_BOOL is_set_afe_power;           // 是否设置模拟功率
    HI_BOOL is_set_digital_power;       // 是否设置数字功率
    HI_BOOL pad;                // 是否写入NV项
    HI_U8   afe_pwoer;                  // 模拟功率

    HI_S8   digital_power[4];           // 数字功率
    HI_U32  delay_effective_time;       // 延迟生效时间，单位S
}DIAG_CMD_SET_DELAY_POWER_CHANGE_STRU;

typedef struct
{
    HI_BOOL is_set_digital_power_by_freq;       // 是否设置数字功率
    HI_BOOL pad;                // 是否写入NV项
    HI_S8   digital_power_by_freq[26];           // 数字功率

    HI_U32  delay_effective_time;       // 延迟生效时间，单位S
}DIAG_CMD_SET_DELAY_POWER_CHANGE_BY_FREQ_STRU;

typedef struct
{
    HI_U32 delayed_restart_time;        // 延迟重启时间，单位S
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
    HI_U8 switch_freq;                 // 切换到的频段
    HI_U8 pad[3];

    HI_U32 delay_switch_freq_time;     // 延迟切换频段时间，单位S
}DIAG_CMD_SET_SWITCH_FREQ_STRU;

typedef struct
{
    HI_U32 count[10];
}DIAG_CMD_COMMON_TEMP_COMMAND_STRU;

/**
 *  层间消息 ID_DIAG_LAYER_PS_NM_FORMING
 *
 */
#define DFX_BUSINESS_LOG_LEN_MAX 256
typedef struct
{
    HI_CHAR log[DFX_BUSINESS_LOG_LEN_MAX];
}DIAG_LAYER_PS_NM_FORMING_STRU;

/**
 *  层间消息 DIAG_LAYER_PS_NM_COORDINATE_STRU
 *
 */
typedef struct
{
    HI_CHAR log[DFX_BUSINESS_LOG_LEN_MAX];
}DIAG_LAYER_PS_NM_COORDINATE_STRU;

/**
 *  层间消息 ID_DIAG_LAYER_PS_ROUTE
 *
 */
typedef struct
{
    HI_CHAR log[DFX_BUSINESS_LOG_LEN_MAX];
}DIAG_LAYER_PS_ROUTE_STRU;

/**
 *  层间消息 ID_DIAG_LAYER_PS_NM_CHANGEPROXY
 *
 */
typedef struct
{
    HI_CHAR log[DFX_BUSINESS_LOG_LEN_MAX];
}DIAG_LAYER_PS_NM_CHANGEPROXY_STRU;

/**
 *  层间消息 ID_DIAG_LAYER_PS_ROUTE_AGING
 *
 */
typedef struct
{
    HI_CHAR log[DFX_BUSINESS_LOG_LEN_MAX];
}DIAG_LAYER_PS_ROUTE_AGING_STRU;

/**
 *  层间消息 ID_DIAG_LAYER_PS_ROUTE_FEEDBACK
 *
 */
typedef struct
{
    HI_CHAR log[DFX_BUSINESS_LOG_LEN_MAX];
}DIAG_LAYER_PS_ROUTE_FEEDBACK_STRU;

/**
 *  层间消息 ID_DIAG_LAYER_PS_ROUTE_DETECT
 *
 */
typedef struct
{
    HI_CHAR log[DFX_BUSINESS_LOG_LEN_MAX];
}DIAG_LAYER_PS_ROUTE_DETECT_STRU;

/**
 *  层间消息 ID_DIAG_LAYER_PS_ROUTE_EVALUATE
 *
 */
typedef struct
{
    HI_CHAR log[DFX_BUSINESS_LOG_LEN_MAX];
}DIAG_LAYER_PS_ROUTE_EVALUATE_STRU;

typedef struct
{
    HI_U32 tx; // 接收速率
    HI_U32 rx; // 发送速率
}DIAG_CMD_REPORT_TXRX_STATIC_SPEED_STRU;

//速率异常统计
typedef struct
{
    HI_U32 time_stamp;                  //时间戳
    HI_U32 ntb;                         //ntb时间戳

    HI_U16 mac_rx_ip_pre_cnt;           // MAC上个统计周期接收IP报文个数
    HI_U16 mac_tx_ip_pre_cnt;           // MAC上个统计周期发送IP报文个数
    HI_U16 mac_rx_ip_cur_cnt;           // MAC当前统计周期接收IP报文个数
    HI_U16 mac_tx_ip_cur_cnt;           // MAC当前统计周期发送IP报文个数

    HI_U16 eth_rx_ip_pre_cnt;           // 以太网上个统计周期接收IP报文个数
    HI_U16 eth_tx_ip_pre_cnt;           // 以太网上个统计周期发送IP报文个数
    HI_U16 eth_rx_ip_cur_cnt;           // 以太网当前统计周期接收IP报文个数
    HI_U16 eth_tx_ip_cur_cnt;           // 以太网当前统计周期发送IP报文个数

    HI_U32 eth_rx_ip_abnormity_cnt;     // 以太网接收IP报文异常个数
    HI_U32 eth_tx_ip_abnormity_cnt;     // 以太网发送IP报文异常个数
    HI_U32 mac_rx_ip_abnormity_cnt;     // MAC层接收IP报文异常个数
    HI_U32 mac_tx_ip_abnormity_cnt;     // MAC层发送IP报文异常个数

    HI_U16 eth_used_buff_cnt;           // 以太网已用的buffer值
    HI_U16 eth_tx_err_cnt;              // 以太网发送失败的次数

    HI_U16 itrupt_cnt;                  // 中断个数
    HI_U16 pcs_itrupt_cnt;              // 前导中断个数
    HI_U16 fc_itrupt_cnt;               // FC中断个数
    HI_U16 pb_itrupt_cnt;               // PB块中断个数

    HI_U16 rx_end_itrupt_cnt;           // 接收完成中断个数
    HI_U16 tx_end_itrupt_cnt;           // 发送完成中断个数

    HI_U16 enable_itrupt_cnt;           // 使能中断个数
    HI_U16 disable_itrupt_cnt;          // 关闭中断个数

    HI_U16 rx_fc_success_cnt;           // 接收FC成功个数
    HI_U16 rx_short_pb_success_cnt;     // 接收短PB块成功个数
    HI_U16 rx_long_pb_success_cnt;      // 接收长PC块成功个数
    HI_U16 tx_short_pb_cnt;             // 发送短PB块个数

    HI_U16 tx_long_pb_cnt;              // 发送长PB块个数
    HI_U16 tx_msdu_success_cnt;         // 发送MSDU成功个数
}DIAG_CMD_SPEED_ABNORMITY_STATISTIC_STRU;

//收、发包周期计数
typedef struct
{
    HI_U32 mac_rx_ip_cnt;       //MAC收、发IP报文数统计
    HI_U32 mac_tx_ip_cnt;

    HI_U32 eth_rx_ip_cnt;       //以太网收、发IP报文数统计
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

//MAC系统异常统计
typedef struct
{
    HI_U32 malloc_err_cnt;              //内存分配错误
    HI_U32 mfree_err_cnt;               //内存释放错误
}DIAG_CMD_QUERY_MAC_SYS_ERR_STRU;
typedef struct
{
    HI_U32 bcn_soft_crc_err_cnt;        // 信标帧软件校验失败次数
    HI_U32 beacon_snid_err_cnt;         // 信标帧网络号异常次数
    HI_U32 sof_crc_err_cnt;             // SOF帧MAC校验失败次数
    HI_U32 sof_snid_err_cnt;            // SOF帧网络号异常次数
    HI_U32 beacon_my_snid_zero_cnt;     // 切频段后设置my snid=0 beacon统计
    HI_U32 sof_my_snid_zero_cnt;        // 切频段后设置my snid=0 sof统计
    HI_U16 parse_bcn_entry_err_cnt;     // 解析信标条目错误
    HI_U16 bcn_belen_err_cnt;           // 信标条目长度错误
    HI_U32 sof_detect_snid_err_cnt;     // 探测协议sof的snid过滤次数
    HI_U32 rcv_my_bcn_cnt;              // 接收本网络的信标计数
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
    HI_U16 odtei;                   // 报文目的站点
    HI_U16 ostei;                   // 报文源站点

    HI_U16 dtei;                    // 下一跳
    HI_U16 packet_index;            // 报文序号

    HI_U16 msdu_size;               // 报文大小
    HI_U16 packet_type;             // 报文类型：升级报文、应用层报文、网络管理报文、IP报文

    HI_U8  result;                  // 发送结果：发送成功、发送失败、报文过期、重传报文
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
    HI_U16 ostei;                   // 报文源地址
    HI_U16 odtei;                   // 报文目的地址

    HI_U16 dtei;                    // 报文下一跳地址
    HI_U16 stei;                    // 报文发送地址

    HI_U16 msdu_size;               // 报文大小
    HI_U16 packet_type;             // 报文类型

    HI_U16 packet_index;
    HI_U8  lid;
    HI_U8  broadcast_flag;

    HI_U16 ptcl_ver;
    HI_U16 pad;
}DIAG_CMD_RX_REPORT_STRU;

typedef struct
{
    HI_U32 ntb;                         //接收到该报文时的NTB

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
//UIU模块的消息打印
typedef struct
{
    HI_U32 para1;
    HI_U32 para2;
    HI_U32 para3;
    HI_U32 para4;
    HI_U32 para5;
}DIAG_CMD_PTPC_REPORT_STRU;

//状态切换的消息
typedef struct
{
    HI_U8  old_state;               //老状态
    HI_U8  new_state;               //新状态
    HI_U16 msg_id;                  //触发该消息的动作ID

    HI_U32 pad[4];
}DIAG_CMD_PTPC_STATE_CHANGE_STRU;
//从PHY接收到报文的消息
typedef struct
{
    HI_U8 oda[HI_PLC_MAC_ADDR_LEN];
    HI_U8 osa[HI_PLC_MAC_ADDR_LEN];

    HI_U16 pkt_size;                    //报文长度
    HI_U16 pad;
}DIAG_CMD_PTPC_APP_PKT_FROM_PHY_STRU;


//从APP接收到报文的消息
typedef struct
{
    HI_U8 oda[HI_PLC_MAC_ADDR_LEN];
    HI_U8 osa[HI_PLC_MAC_ADDR_LEN];

    HI_U16 pkt_size;                    //报文长度
    HI_U16 pad;
}DIAG_CMD_PTPC_APP_PKT_FROM_APP_STRU;

//发送搜索帧的消息
typedef struct
{
    HI_U8  send_search_frame_cnt;                               //发送搜索帧的计数
    HI_U8  sweep_freq_index;                                    // 当前扫频频点索引
    HI_U8  search_frequency_range_cnt;                          // 搜索频段次数
    HI_U8  active_search_state;                                 // 主动搜索状态

    HI_U8  cur_freq;                                            //当前的工作频段
    HI_U8  cur_snid;
    HI_U8  ntb_auto_sync_state;                                 //NTB自动同步的状态
    HI_U8  pad;
}DIAG_CMD_PTPC_SEND_SEARCH_FRAME_STRU;

//接收搜索帧的消息
typedef struct
{
    HI_BOOL is_response;                        // 是否需要回应
    HI_BOOL is_sweep;                           // 是否扫频:0.未扫频，处于同步状态，1.扫频，未同步
    HI_U8  mac[HI_PLC_MAC_ADDR_LEN];

    HI_U8  req_snid;
    HI_U8  rsp_snid;
    HI_U8  network_state;                       // 0:离网,1:在网
    HI_U8  ntb_synch_state;                     // 0:未同步,1:已同步

    HI_U16 stei;                                // 站点TEI
    HI_U8  pad[2];
}DIAG_CMD_PTPC_RCV_SEARCH_FRAME_STRU;

//发送同步信标帧的消息
typedef struct
{
    HI_U8  ntb_synch_state;                     //NTB同步状态
    HI_U8  sta_state;                           //STA同步状态

    HI_U8 synch_mac[HI_PLC_MAC_ADDR_LEN];       //同步信标帧中的同步MAC

    HI_BOOL is_synch_frame_sweep;               //是否进行扫频发送信标
    HI_U8  pad;
}DIAG_CMD_PTPC_SEND_SYNC_FRAME_STRU;

typedef struct
{
    HI_U32 config[8];
}DIAG_CMD_MAC_CONFIG_SW_STRU;

#define DFX_NDM_CMD_MAC_R_NUM 10
typedef struct
{
    HI_U8 scope; // 1: 全网, 2: 单点
    HI_U8  pad;
    HI_U8 lockedProxyAddr[6];
    HI_U32 code; // 用于反馈设置结果
}DIAG_CMD_NDM_CTRL_ROUTE_LOCK_STRU; // 路由锁定

typedef struct
{
    HI_U8 scope; // 1: 全网, 2: 单点
    HI_U8 pad[3];
    HI_U32 code; // 用于反馈设置结果
}DIAG_CMD_NDM_CTRL_ROUTE_UNLOCK_STRU; // 路由解锁

typedef struct
{
    HI_U8 oda[HI_PLC_MAC_ADDR_LEN];
    HI_U8 next[HI_PLC_MAC_ADDR_LEN];

}DIAG_CMD_NDM_CTRL_ROUTE_CONFIG_STRU;// 静态路由配置

typedef struct
{
    HI_U8 echo[256];
}DIAG_CMD_NDM_CTRL_TRACE_STRU;//  trace 命令

//在一个PB块的情况下，抄控器通道所能承载的最大报文长度
//(LONG_PBB_SIZE - MAC_FRAME_HEADER_SIZE - MAC_ICV_SIZE - MM_HEAD_SIZE - sizeof(dfx_ndm_packet_head_st))
#define DIAG_CMD_NDM_CTRL_PING_PK_MAX_LEN (388)//限制为1个PB的可用大小

typedef struct
{
    HI_U32 ulLen; // 表示echo段的长度， 在外层保证4字节对齐
    HI_U8 echo[0];
}DIAG_CMD_NDM_CTRL_PING_STRU; //  ping 命令 ， 该命令不支持命令行操作.

typedef struct
{
    HI_U32 nuk;
}DIAG_CMD_NDM_CTRL_SEARCH_DEV_STRU; //  搜索设备

typedef struct
{
    HI_U8 echo1;
    HI_U8 echo2;
    HI_U16 ack;
}DIAG_CMD_NDM_CTRL_PIPE_ACK_STRU;

typedef struct
{
    HI_U8    network_status;                                        // 网络状态：未入网状态和已入网状态
    HI_U8    multi_network_status;                                  // 多网络状态：异常状态和正常状态
    HI_U8    search_meter_status;                                   // 搜表状态：搜表中和搜表结束
    HI_U8    read_meter_addr_status;                                // 读表地址状态：读取成功和读取失败

    HI_U8    sys_test_status;                                       // 是否是在工装模式
    HI_U8    pad[3];

    HI_U32   set_not_join_network_read_meteradd_fail_led_cnt;
    HI_U32   set_read_meter_fail_led_cnt;
    HI_U32   set_not_join_network_led_cnt;
    HI_U32   set_multi_network_exception_led_cnt;
    HI_U32   set_join_network_led_cnt;
    HI_U32   led_set_485_led_cnt;
    HI_U16  led_not_join_and_read_meter_fail_timer_start_fail_cnt;  // 未入网并且读表地址失败timer启动失败
    HI_U16  led_read_meter_fail_timer_start_fail_cnt;               // 读取电表地址timer启动失败

    HI_U8   led_rx_sys_test_timer_start_fail_cnt;                   // 工装模式下，启动STA点灯定时器失败次数
    HI_U8   led_set_sta_test_status_cnt;                            // 工装模式下，启动STA点灯次数
    HI_U8   led_set_CLTII_test_status_cnt;                          // 工装模式下，设置II采点灯次数
    HI_U8   led_set_ndm_test_status_cnt;                            // 工装模式下，设置抄控器点灯
}DIAG_CMD_LED_STATUS_STRU;

//HSO下发命令启动频段评估的接口
typedef struct
{
   HI_U8 mode;
   HI_U8 pad[3];
}DIAG_CMD_EVALUATE_FREQ_REQ_STRU;

//HSO下发评估评估的启动结果
typedef struct
{
   HI_U32 ret_code;         //返回码
   HI_U32 total_time;       //当前模式运行需要占用的时间，单位s
}DIAG_CMD_EVALUATE_FREQ_RSP_STRU;

//HSO清除频段评估的结果
typedef struct
{
   HI_U32 ret_code;         //返回码
}DIAG_CMD_CLEAR_EVALUATE_FREQ_RESULT_RSP_STRU;

//HSO查询当前频段评估的状态
typedef struct
{
    HI_U8  evaluate_mode;                   //频段评估的模式
    HI_U8  evaluate_reason;                 //当前评估启动的原因
    HI_U16  ret_code;

    HI_U32 run_time;                        //该模式已运行的时间,单位:s
    HI_U32 total_evaluate_time;             //当前评估需要的总时间,单位:s
}DIAG_CMD_QUERY_EVALUATE_FREQ_RSP_STRU;

//频段自适应模块的统计量
typedef struct
{
    HI_U32 pad;
    HI_U32 hso_set_cnt;

    HI_U32 realtime_start_cnt;                  //定时触发评估的次数
    HI_U32 full_start_success_cnt;
    HI_U32 full_start_consume_cnt;
    HI_U32 full_start_after_consume_cnt;

    HI_U32 full_start_white_list_change_cnt;
    HI_U32 auto_start_success_cnt;
    HI_U32 auto_start_fail_cnt;
    HI_U32 stop_success_evaluate_cnt;

    HI_U32 stop_fail_evaluate_inner_cnt;
    HI_U32 stop_fail_evaluate_outer_cnt;

    HI_U32 affirm_network_state_fail_cnt;       //确认组网状态失败
    HI_U32 auto_to_full_cnt;                    //自检不满足触发完成评估的次数
    HI_U32 realtime_start_fail_cnt;
}FA_STATISTIC_INFO_STRU;

//频段自适应模块的统计量
typedef struct
{
    FA_STATISTIC_INFO_STRU stat_info;

    HI_U8  cur_evaluate_mode;                                   //当前的评估模式
    HI_U8  cur_freq;                                            //当前频段, 该值在一定阶段可能为将要评估的频段
    HI_U8  init_freq;                                           //评估启动前的工作频段
    HI_U8  freq_evaluated_cnt;                                  //已评估的频段数

    HI_U8  evaluate_state;                                      //当前的评估状态
    HI_U8  start_reason;                                        //本次启动频段评估的原因
    HI_U8  stop_reason;                                         //本次停止频段评估的原因
    HI_U8  affirm_network_state_cnt;                            //确认组网完成状态的次数

    HI_U32 start_time_stamp;                                    //启动时间，单位:ms
    HI_U32 toatl_run_time;                                      //评估模式需要的总运行时间，单位:s
    HI_U32 stop_time_stamp;                                     //实际结束时间，单位:ms
    HI_U32 sta_join_cnt;                                        //加入网络的站点数统计

    HI_U16 node_num;                                            //除CCO外的全网站点数
    HI_U16 ping_send_node_cnt;                                  //本轮已发送测试报文的站点数

    HI_U16 ping_next_tei;                                       //下一次测试的起始站点TEI索引
    HI_U8  ping_turn_cnt;                                       //ping包测试的轮次计数
    HI_U8  auto_fail_cnt;                                       //自检评估不满足指标的次数

    //HI_MAC_FA_FREQ_RESULT_STRU freq_result[3];                //频段评估的结果
    HI_MAC_FA_FREQ_RESULT_STRU freq_result[FREQ_TRY_NUM];       //频段评估的结果
}DIAG_CMD_FA_STATISTIC_INFO_STRU;

// 台区识别命令，定义如下:
typedef enum
{
    HI_MAC_TF_IDENTIFY_CMD_UNKNOWN = 0,
    HI_MAC_TF_IDENTIFY_CMD_CCO_ALL_MODE_START,          //启动CCO全网识别模式
    HI_MAC_TF_IDENTIFY_CMD_CCO_PART_MODE_START,         //启动CCO指定站点识别模式

    HI_MAC_TF_IDENTIFY_CMD_STOP = 5,                    //停止台区识别模式
    HI_MAC_TF_IDENTIFY_CMD_CLEAR,                       //清空台区识别结果
    HI_MAC_TF_IDENTIFY_CMD_STOP_SEQ,                    //停止台区识别序列发送
    HI_MAC_TF_IDENTIFY_CMD_MAX
}HI_MAC_TF_IDENTIFY_CMD_E;

// 台区识别设置请求结构定义
typedef struct _DIAG_CMD_TF_IDENTIFY_SET_REQ_STRU_
{
    HI_MAC_TF_IDENTIFY_CMD_E cmd;           //台区识别命令

    HI_U16 identifyTime;                        //台区识别持续时间，单位min，该时间大于sendSeqTime
    HI_U16 sendSeqTime;                         //发射机工作时间，单位min

    HI_U32 pad[4];
}DIAG_CMD_TF_IDENTIFY_SET_REQ_STRU;

// 台区识别错误码定义如下:
typedef enum
{
    HI_MAC_TF_IDENTIFY_ERR_NULL = 0,
    HI_MAC_TF_IDENTIFY_ERR_SUCCESS,                    // 启动成功
    HI_MAC_TF_IDENTIFY_ERR_CCO_NOT_SUPPORT,            //CCO不支持台区识别
    HI_MAC_TF_IDENTIFY_ERR_NDM_NOT_SUPPORT,            //NMD不支持台区识别
    HI_MAC_TF_IDENTIFY_ERR_FAIL,                       //启动失败
    HI_MAC_TF_IDENTIFY_ERR_IDENTIFYING,                // 台区识别中
    HI_MAC_TF_IDENTIFY_ERR_BUSINESS_BUSY,              // 升级中app业务忙
    HI_MAC_TF_IDENTIFY_ERR_SEARCH_METER,               //搜表中
    HI_MAC_TF_IDENTIFY_ERR_BITMAP_INVALID,             //指定的位图不可用
    HI_MAC_TF_IDENTIFY_ERR_SWITCHING_FREQ,             // CCO处于全网切换频段中，不启动台区识别
    HI_MAC_TF_IDENTIFY_ERR_MAX
}HI_MAC_TF_IDENTIFY_ERR_E;

//应答DIAG_CMD_TF_IDENTIFY_RSP_STRU的定义如下：
//台区识别设置应答结构定义
typedef struct _DIAG_CMD_TF_IDENTIFY_RSP_STRU_
{
    HI_MAC_TF_IDENTIFY_ERR_E ret;                 //返回值
    HI_U32 pad;
}DIAG_CMD_TF_IDENTIFY_RSP_STRU;

// 台区识别模式，定义如下
typedef enum
{
    HI_MAC_TF_IDENTIFY_TYPE_NULL         = 0,
    HI_MAC_TF_IDENTIFY_TYPE_TEI             ,  //使用TEI进行台区识别
    HI_MAC_TF_IDENTIFY_TYPE_MAC             ,//使用MAC进行台区识别
    HI_MAC_TF_IDENTIFY_TYPE_TEI_MAC         ,//同时使用TEI和MAC进行台区识别

    HI_MAC_TF_IDENTIFY_TYPE_MAX
}HI_MAC_TF_IDENTIFY_TYPE_E;

#define HI_MAC_TF_TEI_BITMAP_LEN      32      // 下发台区识别范围长度
#define HI_MAC_TF_TEI_BITMAP_WIDTH    64      // 下发台区识别范围的宽度

//台区识别的识别范围设置
typedef struct _DIAG_CMD_TF_IDENTIFY_STA_CONFIG_STRU_
{
    HI_U16      sn;                                                     //序列号，对于一次命令，对应的多个数据包的该值相同；不同轮请求的上报，该值不同，所有sn号从1开始，其它命令也是
    HI_U16      is_last:1;                                           //本轮下发的最后一个包的标识：最后一个包，该值为1，非最后一个包，该值为0
    HI_U16      pkt_idx:15;                                        //本包的索引号，从1开始
    HI_MAC_TF_IDENTIFY_TYPE_E       ident_type;   //0:使用TEI进行识别，1:使用mac进行识别，2:同时使用TEI和mac进行识别

    HI_U32      tei_bitmap[HI_MAC_TF_TEI_BITMAP_LEN];   //待识别站点的TEI位图，以后可以改为mac地址

    HI_U8       pad[4];                                              //保留，以后可以改为mac个数
}DIAG_CMD_TF_IDENTIFY_STA_CONFIG_STRU;

// 应答DIAG_CMD_TF_IDENTIFY_STA_CONFIG_IND_STRU的定义如下：
// 台区识别设置应答结构定义
typedef struct _DIAG_CMD_TF_IDENTIFY_STA_CONFIG_IND_STRU_
{
    HI_MAC_TF_IDENTIFY_ERR_E ret;                 //返回值
    HI_U32 pad;
}DIAG_CMD_TF_IDENTIFY_STA_CONFIG_IND_STRU;
// HSO 查询部分台区识别设置范围
typedef struct _DIAG_CMD_TF_IDENTIFY_QRY_STA_CONFIG_REQ_STRU_
{
    HI_U32 tei_bitmap[HI_MAC_TF_TEI_BITMAP_LEN];   //待识别站点的TEI位图
}DIAG_CMD_TF_IDENTIFY_QRY_STA_CONFIG_REQ_STRU;

typedef struct _DIAG_CMD_TF_IDENTIFY_STA_QRY_RANGE_STRU_
{
    HI_U8       ident_type;    //全网还是部分识别，0：全网识别，1：部分识别，取值为HI_MAC_TF_IDENTIFY_QRY_TYPE_E类型
    HI_U8       pad[3];          //保留

    HI_U32     tei_bitmap[HI_MAC_TF_TEI_BITMAP_LEN];   //待识别站点的TEI位图，以后可以改为mac地址
}DIAG_CMD_TF_IDENTIFY_STA_QRY_RANGE_STRU;
// 台区识别查询识别结果下发参数
typedef enum
{
    HI_MAC_TF_IDENTIFY_QRY_TYPE_WHOLE_NET,
    HI_MAC_TF_IDENTIFY_QRY_TYPE_PART
}HI_MAC_TF_IDENTIFY_QRY_TYPE_E;

// 台区识别的识别结果定义如下:
typedef enum
{
    HI_MAC_TF_IDENTIFY_QRY_RESULT_UNKNOWN = 0,
    HI_MAC_TF_IDENTIFY_QRY_RESULT_THIS_POWER_DISTRI,      //为本台区站点
    HI_MAC_TF_IDENTIFY_QRY_RESULT_NON_THIS_POWER_DISTRI, //不为本台去站点
    HI_MAC_TF_IDENTIFY_QRY_RESULT_NOT_SUPPORT, //不支持台区识别
    HI_MAC_TF_IDENTIFY_QRY_RESULT_FAIL,        //识别结果查询失败
    HI_MAC_TF_IDENTIFY_QRY_RESULT_MAYBE_OTHER,  //可能是邻台区的站点
    HI_MAC_TF_IDENTIFY_QRY_RESULT_CODE_CRASH     // 码字碰撞
}HI_MAC_TF_IDENTIFY_QRY_RESULT_E;

//接收档位
#define DIAG_CMD_TF_RCV_LEVEL_UNKNOWN                               0       //接收档位未知
#define DIAG_CMD_TF_RCV_LEVEL_240K                                  1       //240K接收档位
#define DIAG_CMD_TF_RCV_LEVEL_1M                                    2       //1M接收档位

// 码字类型
#define DIAG_CMD_TF_13BIT_SEQ_TYPE                                  0       // 13bit码字类型
#define DIAG_CMD_TF_127BIT_SEQ_TYPE                                 1       // 127bit码字类型
#define DIAG_CMD_TF_127BIT_INTER_SEQ_TYPE                           2       // 127bit间隔扰动码字类型

//DIAG_CMD_TF_IDENTIFY_RESULT_ENTRY_STRU定义如下：
//台区识别结果中成员的定义，16字节
typedef struct _DIAG_CMD_TF_IDENTIFY_RESULT_ENTRY_STRU_
{
    HI_U16 tei : 12;
    HI_U16 best_line : 4;                   // 8个电压点，从1-8
    HI_U8  is_online            : 1;        //0:不在线，1:在线
    HI_U8  is_exist_diff_seq    : 1;        //台区识别结果是否存在不同门限检测出的序列不同的情况
    HI_U8  alg_mode             : 2;        //识别成功时的算法
    HI_U8  result               : 4;        //台区识别结果:见结果定义HI_MAC_TF_IDENTIFY_QRY_RESULT_E
    HI_U8  noise_limit          : 4;        //识别成功时的去噪门限
    HI_U8  seq_type             : 1;        // 码字类型，参见DIAG_CMD_TF_13BIT_SEQ_TYPE
    HI_U8  rcv_level            : 3;        //识别成功时的档位,取值参见DIAG_CMD_TF_RCV_LEVEL_240K的定义

    HI_U16 soft_value;                      //识别成功时的软值
    HI_U8  mac[HI_PLC_MAC_ADDR_LEN];

    HI_U16 signal_mean;                     //信号的均值
    HI_U16 noise_mean;                      //噪声的均值
}DIAG_CMD_TF_IDENTIFY_RESULT_ENTRY_STRU;

//台区识别状态
typedef enum
{
    HI_MAC_TF_IDENTIFY_STATUS_DOING                 = 0,        //台区扰动阶段
    HI_MAC_TF_IDENTIFY_STATUS_DONE                  = 1,        //台区识别全阶段结束
    HI_MAC_TF_IDENTIFY_STATUS_REVISE_MODE           = 2,        //识别结果校正阶段
}HI_MAC_TF_IDENTIFY_STATUS_E;

// 台区识别结果查询应答结构定义
typedef struct _DIAG_CMD_TF_IDENTIFY_RESULT_RSP_STRU_
{
    HI_U16 sn;                  //每台区识别结果上报的序列号，对于一次查询命令，对应的多个上报数据包的该值相同；不同轮请求的上报，该值不同
    HI_U16 pkt_idx;             //本包的索引号

    HI_U16 num_total;           //总个数
    HI_U16 entry_num;           //本数据包中包含的有效站点数

    HI_U32 tf_identify_time;    //已识别时间,单位:s

    HI_BOOL is_last;            //本轮上报的最后一个包的标识：最后一个包，该值为1，非最后一个包，该值为0
    HI_U8  status;              //0：识别中；1：识别结束
    HI_U8  pad[2];              //保留

    DIAG_CMD_TF_IDENTIFY_RESULT_ENTRY_STRU entry[0];
}DIAG_CMD_TF_IDENTIFY_RESULT_RSP_STRU;

#define DIAG_CMD_TF_STOP_REASOM_ZERO_INTERRUPT_ERR  1           //过零中断异常
//异常停止台区识别的原因
#define TF_STOP_ABNORMITY_IDENTIFY_REASON_UNKNOWN                       0       //未知
#define TF_STOP_ABNORMITY_IDENTIFY_REASON_NDM_ZERO_ERR                  1      //发射机过零中断异常
#define TF_STOP_ABNORMITY_IDENTIFY_REASON_WAIT_SEND_SEQ_END_TIMEOUT     2       //台区序列发送超时
#define TF_STOP_ABNORMITY_IDENTIFY_REASON_HSO                           3       //HSO停止台区识别的所有阶段
#define TF_STOP_ABNORMITY_IDENTIFY_REASON_SEND_REQ_TIMEOUT              4       //CCO通知NDM发送序列超时
#define TF_STOP_ABNORMITY_IDENTIFY_REASON_SEND_SEQ_REQ_TO_NDM           5       //向抄控器发送序列失败
#define TF_STOP_ABNORMITY_IDENTIFY_REASON_NDC_DISCONNECT                6       //CCO与NDM连接断开
#define TF_STOP_ABNORMITY_IDENTIFY_REASON_REINIT                        7       //软重启
#define TF_STOP_ABNORMITY_IDENTIFY_REASON_HSO_START                     8      //HSO启动台区识别时停止台区识别
#define TF_STOP_ABNORMITY_IDENTIFY_REASON_MODE_OVERTIME                 9      //台区识别模式到期停止
#define TF_STOP_ABNORMITY_IDENTIFY_REASON_NDM_IN_STA_MODE               10      //抄控器处于STA连接模式下停止台区识别
#define TF_STOP_ABNORMITY_IDENTIFY_REASON_NDM_NOT_TX                    11      // ndm不是发射机
#define TF_STOP_ABNORMITY_IDENTIFY_REASON_NDM_AUTO_CHECK_FAIL           12      //发射机上电自检失败
#define TF_STOP_ABNORMITY_IDENTIFY_REASON_UPG                           13       //升级而异常停止
#define TF_STOP_ABNORMITY_IDENTIFY_REASON_NDM_TO_NDM                    14       // 发射机处于ndm-ndm模式

//台区识别的状态查询的应答结构定义
typedef struct
{
    HI_U32 pad0;                //设置的序列发送模式持续时间，单位s
    HI_U32 run_time;            //当前已运行的时间,单位s

    HI_U8  identify_state;      //识别状态,取值HI_MAC_TF_IDENTIFY_STATUS_E
    HI_U8  identify_mode;       //识别模式,定义见TF_NOT_USE_IDENTIFY_MODE
    HI_BOOL is_searched_meter;  //是否做过搜表
    HI_U8  stop_reason;         //台区识别停止原因

    HI_U16 identify_seq;        //台区识别使用的序列
    HI_U8 seq_type;             // 码字类型
    HI_U8 pad2;

    HI_U32 set_identify_time;   //设置的识别模式持续时间，单位s
    HI_U32 pad[4];              //保留
}DIAG_CMD_TF_IDENTIFY_STATE_RSP_STRU;

#define HI_MAC_PHASE_IDENTIFY_RESULT_ENTRY_NUM_MAX      46      //相位识别结果每次上报的最大站点个数

//单个站点的相位识别结果
typedef struct
{
    HI_U16 tei;                 //本站点的TEI
    HI_U8  result   : 4;        //相线1的相位识别结果:PHASE_ALL、PHASE_A、PHASE_B、PHASE_C、INVALID_PHASE，参见hi_mdm.h
    HI_U8  state    : 4;        //相位识别状态:0,未识别;1:不支持;2:识别中;3:识别结束
    HI_U8  pn_identify_result : 2;  //相线1零火线反接识别结果:0,接线正确;1,反接；2,未知,参见hi_mdm.h
    HI_U8  pn_identify_state : 2;   //零火线反接识别状态:0,未识别;1:不支持;2:识别中;3:识别结束
    HI_U8  pn_identify_result_b : 2;  //相线2零火线反接识别结果:0,接线正确;1,反接；2,未知,参见hi_mdm.h
    HI_U8  pn_identify_result_c : 2;  //相线3零火线反接识别结果:0,接线正确;1,反接；2,未知,参见hi_mdm.h

    HI_U32 result_b : 4;        //相线2的相位识别结果:PHASE_ALL、PHASE_A、PHASE_B、PHASE_C、INVALID_PHASE，参见hi_mdm.h
    HI_U32 result_c : 4;        //相线3的相位识别结果:PHASE_ALL、PHASE_A、PHASE_B、PHASE_C、INVALID_PHASE，参见hi_mdm.h
    HI_U32 product_type : 5;    // 产品类型，参见HI_ND_TYPE_CCO,HI_ND_TYPE_STA,HI_ND_TYPE_RELAY,HI_ND_TYPE_THREE_STA
    HI_U32 pad3     : 19;       //保留
}DIAG_CMD_PHASE_IDENTIFY_RESULT_ENTRY_STRU;


//相位识别结果应答结构定义:
typedef struct
{
    HI_U16 sn;                  //每台区识别结果上报的序列号，对于一次查询命令，对应的多个上报数据包的该值相同；不同轮请求的上报，该值不同
    HI_U16 pkt_idx;             //本包的索引号，从0开始

    HI_U16 entry_num;           //本数据包中包含的有效站点数
    HI_BOOL is_last;            //本轮上报的最后一个包的标识：最后一个包，该值为1，非最后一个包，该值为0
    HI_U8  pad;                 //保留

    HI_U32 pad2[2];             //保留

    DIAG_CMD_PHASE_IDENTIFY_RESULT_ENTRY_STRU entry[0];
}DIAG_CMD_PHASE_IDENTIFY_RESULT_STRU;

// 台区识别黑名单产生类型如下:
typedef enum
{
    DIAG_CMD_BLACKLIST_TYPE_USER = 0,                       //由用户导入的黑名单
    DIAG_CMD_BLACKLIST_TYPE_AUTO                            //自动生成的黑名单
}DIAG_CMD_TF_IDENTIFY_BLACKLIST_TYPE_E;

// 台区识别黑名单错误类型
typedef enum
{
    HI_MAC_BLACKLIST_ERR_UNKNOWN = 0,              // 错误原因未知
    HI_MAC_BLACKLIST_ERR_BIG_NOISE,                // 大噪声
    HI_MAC_BLACKLIST_ERR_CODE_CRASH,               // 码字碰撞
    HI_MAC_BLACKLIST_ERR_BIG_NOISE_AND_CODE_CRASH, // 既有大噪声又有码字碰撞
}HI_MAC_TF_IDENTIFY_BLACKLIST_ERR_TYPE_E;
// 台区识别黑名单
typedef struct _DIAG_CMD_TF_IDENTIFY_BLACKLIST_ENTRY_S_
{
    HI_U8 mac[HI_PLC_MAC_ADDR_LEN];
    HI_U16 scope_avg;

    HI_U8 type : 2;             //黑名单的类型，取值为DIAG_CMD_TF_IDENTIFY_BLACKLIST_TYPE_E
    HI_U8 rcv_level : 3;     //接收档位
    HI_U8 pad1 : 3;
    HI_U8 err_type;         // 错误码类型，取值为HI_MAC_TF_IDENTIFY_BLACKLIST_ERR_TYPE_E
    HI_U8 pad2[2];
}DIAG_CMD_TF_IDENTIFY_BLACKLIST_ENTRY_S;

// 台区识别黑名单上报查询结构体定义如下:
typedef struct  _DIAG_CMD_BLACKLIST_REQ_S_
{
    HI_U16 sn;                      //结果上报的序列号，对于一次查询命令，对应的多个上报数据包的该值相同；不同轮请求的上报，该值不同
    HI_U16 pkt_idx;                 //本包的索引号

    HI_U16 num_total;               //总个数
    HI_U16 is_last : 1;             //本轮上报的最后一个包的标识：最后一个包，该值为1，非最后一个包，该值为0
    HI_U16 mac_num : 15;            //本数据包中包含的有效mac地址数

    HI_U8  pad[4];

    DIAG_CMD_TF_IDENTIFY_BLACKLIST_ENTRY_S entry[0];
}DIAG_CMD_BLACKLIST_REQ_S;

// 台区识别清空黑名单
typedef struct _DIAG_CMD_BLACKLIST_CLEAR_IND_STRU_
{
    HI_MAC_TF_IDENTIFY_ERR_E ret;                 //返回值
    HI_U32 pad;
}DIAG_CMD_BLACKLIST_CLEAR_IND_STRU;

typedef struct _DIAG_CMD_TF_CROSSTALK_RESULT_ENTRY_STRU_
{
   HI_U16 other_signal_mean;                                           // 对应最大扰动值
   HI_U16 result : 4;                                           // 是否为疑似临台区,参见HI_MAC_TF_IDENTIFY_QRY_RESULT_E
   HI_U16 pad : 12;

   HI_U8 my_mac[HI_PLC_MAC_ADDR_LEN];                 // 本站点MAC
   HI_U8 cco_mac[HI_PLC_MAC_ADDR_LEN];                // 对应最大扰动的CCO的MAC
}DIAG_CMD_TF_CROSSTALK_RESULT_ENTRY_STRU;

typedef struct _DIAG_CMD_TF_CROSSTALK_RESULT_RSP_STRU_
{
   HI_U16 sn;
   HI_U16 pkt_idx;

   HI_U16 num_total;           //总个数
   HI_U16 entry_num;           //本数据包中包含的有效站点数

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

//台区识别CCO端的统计量
typedef struct
{
    HI_U32 send_seq_req_to_ndm_cnt;         //发送序列请求给NDM的次数
    HI_U32 identify_tx_start_success_cnt;   //台区识别成功启动的次数
    HI_U32 send_seq_end_to_cco_cnt;         //发送序列完成通知的计数

    HI_U16 send_ndm_chl_busy_cnt;           //发送时NDC通道忙计数
    HI_U16 rcv_ndm_chl_free_cnt;            //接收到命令时，发送端空闲的计数(非预期的命令)

    HI_U32 resend_ndm_chl_req_cnt;          //NDC通道重传计数
    HI_U16 send_ndm_chl_req_fail_cnt;       //NDC通道请求发送失败计数
    HI_U16 resend_send_ndm_chl_req_fail_cnt;//NDC通道请求重发失败计数

    HI_U32 rcv_seq_req_to_ndm_cnt;          //接收序列请求给NDM的次数
    HI_U32 send_seq_cycle_stop_to_ndm_cnt;  //通知抄控器结束序列循环发送模式的请求的次数
    HI_U32 rcv_seq_end_to_cco_cnt;          //接收序列完成通知的计数
    HI_U32 rcv_repeat_ndm_chl_req_cnt;      //收到重复的NDC通道请求

    HI_U16 rcv_unexpect_ndm_chl_req_cnt;    //收到非预期的NDC通道请求
    HI_U16 rcv_unexpect_ndm_chl_rsp_cnt;    //收到非预期的NDC通道应答计数

    HI_U32 rcv_seq_req_to_ndm_rsp_cnt;      //发送序列请求给NDM的次数
    HI_U16 send_seq_cycle_stop_to_ndm_fail_cnt;     //发送失败次数
    HI_U16 ndm_send_seq_fail_by_req_cnt;    //抄控器在收到请求时发送失败的次数
    HI_U32 rcv_seq_end_to_cco_rsp_cnt;      //发送序列完成通知的计数

    HI_U32 send_result_req_cnt;             //CCO发送识别结果查询的次数

    HI_U16 send_result_req_alloc_msdu_buff_err_cnt;     //获取buffer失败
    HI_U16 send_result_req_msdu_to_tx_err_cnt;          //发送报文失败

    HI_U32 rcv_result_rsp_cnt;              //CCO收到识别结果应答的次数

    HI_U16 rcv_result_rsp_unexpect_cnt;     //CCO收到非预期的应答次数
    HI_U16 rcv_result_rsp_err_cnt;          //CCO收到应答报文内容错误次数

    HI_U32 identify_result_belong_cnt;      //识别结果为本台区站点的计数
    HI_U32 query_timeout_fail_cnt;          //查询超时而识别失败的站点计数
    HI_U32 identify_result_not_belong_cnt;  //识别结果不是本台区站点的计数

    HI_U32 query_one_turn_over_cnt;         //一轮查询结束计数
    HI_U32 stop_identify_cnt;               //停止台区识别的计数

    HI_U32 stop_abnormity_identify_cnt;     //异常停止台区识别计数
    HI_U8  stop_abnormity_identify_last_reason;         //最近一次异常停止的原因
    HI_BOOL ndm_tf_mode;                    //抄控器台区识别模式下禁止搜网的标识
    HI_U16 ndm_not_support_identify_cnt;    //CCO通知时，获知抄控器不支持台区识别计数

    HI_U32 send_seq_bit_cnt;                // 发送机发送台区识别序列次数

    HI_U16 identify_tx_start_fail_disable_cnt;          //因不允许启动台区识别而造成的启动失败次数
    HI_U16 identify_tx_start_fail_doing_cnt;            //因正在识别而造成的启动失败次数
    HI_U16 query_mode_continuance_timer_cnt;            //查询模式定时到期次数
    HI_U8  tf_identify_control_mode;                    //台区识别控制模式
    HI_BOOL is_identifying;

    HI_U32 query_timeout_sta_cnt;                       //查询超时的站点计数

    HI_U16 send_seq_end_to_cco_fail_cnt;                //向CCO发送序列完成通知失败
    HI_U16 rcv_result_unexpect_identify_state_cnt;      //CCO收到的STA的状态不符合预期

    HI_U32 rcv_not_support_ndm_chl_req_cnt;             //收到无法支持的NDC通道请求
    HI_U32 rcv_send_seq_cycle_stop_rsp_cnt;             //收到应答
    HI_U32 rcv_seq_cycle_stop_cnt;                      //接收到请求计数

    HI_U8  cco_seq_type;                                // CCO端码字类型
    HI_U8  ndm_seq_type;                                // ndm端码字类型
    HI_U16 pad;
}DIAG_CMD_TF_CCO_STATIC_STRU;

//CCO端流程统计量
typedef struct
{
    HI_U32 send_ntb_collect_req_cnt;                //发送NTB采集请求报文计数
    HI_U32 send_broadcast_notify_cnt;               // 发送广播通知台区识别启动，停止计数

    HI_U32 hso_cco_all_mode_start_cnt;              //HSO启动全网识别计数
    HI_U32 hso_cco_part_mode_start_cnt;             //HSO启动指定站点识别计数
    HI_U32 result_query_timer_cnt;                  //结果查询定时器回调计数
    HI_U16 send_seq_req_to_ndm_fail_cnt;            //向抄控器发送序列发送请求失败
    HI_U16 wait_send_seq_end_timeout_cnt;           //等序列发送完成通知超时

    HI_U32 new_sta_to_identify_cnt;                 //新加入的待识别站点计数

    HI_U16 rcv_seq_end_to_cco_unexpect_cnt;         //收到非预期的台区识别序列发送完成通知
    HI_U16 rcv_zero_cross_ntb_rsp_err_cnt;          //收到错误的相位识别过零NTB采集应答报文计数

    HI_U8 ndm_to_ndm_mode;                          // 发射机处于ndm-ndm的识别模式
    HI_U8 pad1;
    HI_U8  bcn_tf_notify;                           //是否通知STA启动台区识别
    HI_BOOL is_searched_meter;                       //是否做过搜表

    HI_U32 start_search_meter_cnt;                  //启动搜表次数
    HI_U32 stop_search_meter_cnt;                   //停止搜表次数
    HI_U16 start_search_meter_err_cnt;              //启动搜表失败计数
    HI_U16 stop_search_meter_err_cnt;               //停止搜表失败计数

    HI_U8 ndm_in_sta_mode_cnt;                         // CCO通知时，获知抄控器处于STA连接模式下，CCO不启动台区识别
    HI_U8 ndm_ndc_access_mode;                          // CCO通知时，获知抄控器所处的连接模式
    HI_U8 ndm_not_tx_cnt;                               // ndm不是发射机，CCO不进行台区识别
    HI_U8 ndm_auto_check_fail_cnt;                      //发射机上电自检失败

    HI_U32 last_identify_success_sta_cnt;           //本次查询未识别成功但上次识别成功的次数

    HI_U32 pad[3];
}DIAG_CMD_TF_CCO_FL_STATIC_STRU;

//台区识别STA端的统计量
typedef struct
{
    HI_U32 bcn_notify_enable_cnt;                   //beacon开启台区识别接收
    HI_U32 bcn_notify_disable_cnt;                  //beacon关闭台区识别接收

    HI_U32 rcv_result_req_cnt;                      //收到台区识别结果请求的次数
    HI_U32 send_result_rsp_cnt;                     //发送台区识别结果应答的次数

    HI_U16 phase_ntb_collect_reinit;                //相位识别NTB重新采集的次数
    HI_U16 identify_result_bpc_err_cnt;             //识别结果的BPC不符合预期的次数

    HI_U16 identify_freq_or_snid_err_cnt;           //识别出的频段或SNID与查询时不一致的次数
    HI_U8  last_query_tf_result;                    //最后一次查询时的台区识别结果
    HI_BOOL pad;

    HI_U32 rcv_ntb_collect_req_cnt;                 //收到NTB数据采集通知的报文计数
    HI_U16 sta_ntb_collect_store_index;             //NTB数据存储索引
    HI_U16 listen_network_snid;                     //接收到启动台区识别的网络SNID

    HI_U8  current_rcv_level;                       //当前接收档位,0为240k，1为1M
    HI_U8  rcv_mode;                                //接收档位切换模式
    HI_U16 om_ntb_collect_not_rcv_notify_cnt;       //NTB数据采集未收到通知

    HI_U16 rcv_level_switch_timer_cnt;              //接收档位切换定时器回回调计数
    HI_BOOL is_bcn_tf_start;                        //信标通知启动台区识别
    HI_BOOL is_seq_tf_start;                        //台区识别序列启动台区识别

    HI_U32 seq_tf_start_cnt;                        //序列启动台区识别的次数
    HI_U32 seq_tf_stop_cnt;                         //序列停止台区识别的次数
    HI_U32 rcv_zero_cross_ntb_req_cnt;              //接收到过零NTB请求报文次数
    HI_U32 send_zero_cross_ntb_rsp_cnt;             //发送过零NTB应答报文次数

    HI_U16 his_result_invalid_cnt;                  //历史结果失效次数
    HI_U16 cco_info_change_by_mac_cnt;              //因MAC不同而CCO的信息变化的次数

    HI_U16 cco_info_change_by_freqsnid_cnt;         //因freq和snid不同而CCO的信息变化的次数
    HI_U16 pad1;

    HI_U32 old_freq : 8;                            //CCO信息变化前的freq
    HI_U32 old_snid : 24;                           //CCO信息变化前的snid
    HI_U32 new_freq : 8;                            //CCO信息变化后的freq
    HI_U32 new_snid : 24;                           //CCO信息变化后的snid

    HI_U32 rcv_seq_when_before_invalid_cnt;         //前一次序列无效后收到序列的个数

    HI_U8  old_cco_mac[6];                          //CCO信息变化前的MAC
    HI_U8  new_cco_mac[6];                          //CCO信息变化后的MAC

    HI_U32 his_max_to_small_replaced_cnt;           //历史最大值变小后被替换的计数

    HI_U32 rcv_broadcast_notify_req_cnt;            //收到广播通知启动次数
}DIAG_CMD_TF_STA_STATIC_STRU;

//台区识别公共统计量
typedef struct
{
    HI_BOOL is_support_tf_identify;                 //是否支持过零中断
    HI_U8  sta_check_tf_cnt;                        //STA 检测是否支持台区识别的周期计数
    HI_U8  rcv_ntb_cnt;                             //接收到的过零中断计数
    HI_U8  rcv_a_phase_ntb_cnt;                     //接收到的A相位过零中断计数

    HI_U8  rcv_b_phase_ntb_cnt;                     //接收到的B相位过零中断计数
    HI_U8  rcv_c_phase_ntb_cnt;                     //接收到的C相位过零中断计数
    HI_U16 malloc_err_cnt;                          //动态内在申请失败计数

    HI_U32 switch_phase_by_timeout_cnt;             //因超时主动切相位次数
    HI_U32 zero_cross_interrupt_cnt;                //过零中断计数

    HI_U32 identify_tx_start_by_query_join_sta;     //查询时由新站点加入触发的识别流程

    HI_U16 send_msg_err_cnt;                        //发送消息失败计数
    HI_U16 start_timer_err_cnt;                     //定时器启动失败计数

    HI_U16 alloc_msdu_buff_err_cnt;                 //获取MacBuffer失败次数
    HI_U16 msdu_to_tx_err_cnt;                      //发送报文失败次数
    HI_U8 tf_check_set_sta_reset_cnt;               //STA检测台区识别导致STA软重启的次数
    HI_U8 current_edge;                             //NDM当前NTB沿
    HI_U16 report_to_hso_err_cnt;                   //上报HSO失败的计数

    HI_BOOL is_three_phase_check_ok;                //发射机三相检测OK
    HI_BOOL is_single_phase;                        //发射机是否为单相，
    HI_BOOL is_three_phase_together;                //发射机三相接在一起
    HI_U8 ndm_check_an_ok_cnt;                      //发射机上电自检AN正常次数

    HI_U8 ndm_check_an_err_cnt;                     //发射机上电自检AN过压故障次数
    HI_U8 phase_b_ok_c_err_cnt;                     //B相位有信号，C相位无信号次数
    HI_U8 phase_b_err_c_ok_cnt;                     //B相位无信号，C相位有信号次数
    HI_U8 check_phase_fail_turn_cnt;

    HI_U32 phase_ab_diff_us;                        //AB相位相位间隔
    HI_U32 edge_change_err_cnt;                     //NDM NTB沿切换错误次数

    HI_U8 phase_a_b_c_err_cnt;                      //A,B,C均没有过零
    HI_U8 phase_a_b_err_c_ok_cnt;                   //A,B相没有过零，C相有过零
    HI_U8 phase_a_c_err_b_ok_cnt;                   //A,C相没有过零，B相有过零
    HI_U8 phase_a_err_b_c_ok_cnt;                   //A相没有过零，B,C相有过零

    HI_U8 disable_ndm_to_ndm_mode;                  //不允许启动ndm-ndm模式
    HI_U8 cco_phase_check_result;                   // CCO相位检测结果,1:为ABC,2:ACB:3:失败
    HI_U8 cco_phase_collect_ntb_cnt;                // CCO相位检测收集NTB个数
    HI_U8 is_precise_zero_cross;                    // 是否为精准过零电路

    HI_U32 phase_bc_diff_us;                        //BC相位相位间隔
    HI_U32 phase_ac_diff_us;                        //AC相位相位间隔

    HI_U32 chip_zero_cross_interrupt_cnt;                          // 过零中断次数
    HI_U32 chip13bit_power_period_result_interrupt_cnt;     // V100码字工频周期中断上报次数
    HI_U32 chip127bit_power_period_result_interrupt_cnt;        // V200码字工频周期中断上报次数
    HI_U32 power_period_num_interrupt_cnt;                   // 统计power_period_num个数上报
    HI_U32 noise_detect_interrupt_cnt;                       // 噪声统计中断上报次数
    HI_U32 phase_b_interrupt_cnt;                              // 相位B中断次数
    HI_U32 phase_c_interrupt_cnt;                              // 相位C中断次数
    HI_U32 sta_13Bit_chip_refresh_cnt;              // sta的V100的刷新次数
    HI_U32 sta_127Bit_chip_refresh_cnt;             // sta的V200的刷新次数
    HI_U32 ndm_chip_refresh_cnt;             // ndm的V200的刷新次数

    HI_U8 t_sta_auto_check_phase_result[3];            // 三相位STA上电自检结果
    HI_U8 support_t_sta_three_zero_mode;                 // 三相模块是否支持三相过零检测功能。
    HI_U8 t_sta_first_valid_phase;                     // 三相模块第一个有效的相位。
    HI_U8 cco_phase_check_result_by_power_fail;        // 通过停电上报检测到相位结果
    HI_U16 t_sta_auto_check_phase_diff_us[3];    // 三相位STA上电自检相位间隔，单位us
}DIAG_CMD_TF_STATIC_STRU;

//台区识别序列提取统计量
typedef struct
{
    HI_U32 identify_success_bpc;                    //识别成功时的bpc
    HI_U32 ntb_cache_cnt;                           //有效缓存的NTB计数
    HI_U32 parse_receive_sequence_invalid_cnt;      //在某检测算法和门限下提取序列失败计数
    HI_U32 seq_bit_sum_invalid_cnt;                 //在某检测算法和门限下序列的累加和异常次数

    HI_U16 seq_diff_in_noise_limit_cnt;             //序列检测成功时不同去噪门限检测出的序列不同的计数
    HI_U8  identify_state;                          //STA的识别状态
    HI_U8  success_rcv_level;                       //识别成功时的接收档位

    HI_U32 seq_detect_window_invalid_cnt;           //窗口内台区识别序列检测无效的次数
    HI_U32 identify_success_stamp;                  //检测成功时间戳 单位:s
    HI_U32 identify_diff_seq_stamp;                 //检测到其他序列时间戳 单位:s

    HI_U16 novel_seq_cnt;                           //检测结果满足异常条件的次数
    HI_U8  identify_freq;                           //STA识别出序列对应的频段
    HI_U8 pad0;
    HI_U32 identify_snid;                           //STA识别出序列对应的SNID

    HI_U8 identify_cco_mac[6];                      // STA的127Bit识别出CCO_MAC
    HI_U8 seq_type;                                 // STA识别出的码字类型
    HI_U8 pad;

    HI_U32 detect_diff_snid_cnt;                    //从台区序列中提取的不同的SNID的次数
    HI_U32 last_detect_diff_snid;                   //最近一次检测到的异常SNID
    HI_U8 pad1;
    HI_U8  last_detect_diff_freq;                   //最后一次检测到异常SNID时所处的频段
    HI_U16 novel_seq_store_cnt;                     //异常序列存储次数计数

    HI_U32 identify_success_cnt;                    //台区识别成功次数

    HI_U32 bigger_than_50ms_cnt;                    //窗口内两个NTB间隔大于50ms计数
    HI_U32 smaller_than_1ms_cnt;                    //窗口内两个NTB间隔小于1ms计数
    HI_U32 lost_ntb_cnt;                            //窗口内丢失的过零NTB计数
    HI_U32 between_window_than_50ms_cnt;            //窗口间两个NTB间隔大于50ms计数
    HI_U32 between_window_smaller_than_1ms_cnt;
    HI_U32 between_window_lost_ntb_cnt;             //窗口间丢失的过零NTB计数
    HI_U32 detect_diff_freq_cnt;                    //保留

    HI_U32 smaller_than_30us_cnt;                   //窗口内两个NTB间隔小于30us计数
    HI_U32 smaller_than_5ms_cnt;                    //窗口内两个NTB间隔小于5ms计数

    HI_U32 sign1_synch_success_cnt;                 //第一个A同步成功计数
    HI_U32 sign2_synch_success_cnt;                 //第二个A同步成功计数
    HI_U32 sign2_synch_fail_cnt;                    //第二个A同步失败计数
    HI_U32 exact3_synch_success_cnt;                //精同步B识别成功次数

    HI_U32 exact3_synch_timeout_cnt;                //精同步B识别超时次数
    HI_U32 exact4_synch_success_cnt;                //精同步C识别成功次数
    HI_U32 exact4_synch_timeout_cnt;                //精同步C识别超时次数
    HI_U32 sta_single_win_fail_cnt;                 //单窗窗口达不到门限次数

    HI_U16 noise_avg[2];                            //单窗噪声均值
    HI_U16 start_ntb_index;                         //进行识别的NTB起始位置
    HI_U16 end_ntb_index;                           // 收集的NTB的结束位置
    HI_U16 remain_ntb_cnt;                          // 可用的NTB个数
    HI_U16 code_crash_cnt;                          // 码字碰撞次数

    HI_U32 sign2_synch_11_success_cnt;              // 粗同步码字A，在第11次识别为A的次数
    HI_U32 exact3_synch_fail_cnt;                   // 精同步符号3识别失败次数
    HI_U32 exact4_synch_fail_cnt;                   // 精同步符号4识别失败次数
    HI_U32 union_judge_cnt;                         // 联合判决次数
}DIAG_CMD_QUERY_TF_IDENTIFY_INFO_STRU;

//相位识别统计量
typedef struct
{
    HI_U32 identify_start_success_cnt;                  //相位识别成功启动次数
    HI_U32 stop_identify_cnt;                           //停止相位识别次数

    HI_U16 identify_start_fail_by_doing_cnt;            //识别过程中触发识别的次数
    HI_U16 identify_start_fail_by_not_support_cnt;      //因不支持识别而启动失败的次数
    HI_U16 identify_start_fail_by_tf_cnt;               //台区识别过程中启动相位识别失败次数
    HI_U16 rcv_zero_cross_ntb_rsp_err_cnt;              //接收到的报文错误次数

    HI_U32 send_zero_cross_ntb_req_cnt;                 //发送相位过零NTB查询报文的次数
    HI_U32 rcv_zero_cross_ntb_rsp_cnt;                  //收到相位过零NTB应答报文的次数

    HI_U32 parallel_query_timer_msg_cnt;                //并行查询超时检测定时器回调次数
    HI_U32 parallel_query_timeout_cnt;                  //并行查询超时次数

    HI_U32 query_one_turn_over_cnt;                     //查询一轮结束的次数
    HI_U32 check_over_timer_msg_cnt;                    //确认相位识别结束定时器的回调次数

    HI_U32 ntb_diff_smaller_than_1ms_cnt;               //过零NTB间隔小于1ms

    HI_U16 discard_zero_cross_rsp_cnt;
    HI_U16 get_phase_by_sta_ntb_rsp_err_cnt;            //计算相位时，STA的应答报文错误

    HI_U32 phase_success_sta_cnt;                       //相位识别成功次数
    HI_U32 phase_fail_cnt;                              //相位识别失败次数
    HI_U32 phase_fail_sta_cnt;                          //相位识别失败次数

    HI_U16 rcv_zero_cross_ntb_rsp_timeout_cnt;          //收到超时的过零NTB应答报文
    HI_U16 get_phase_by_sta_ntb_ref_err_cnt;            //计算相位时，CCO端的参数NTB参数错误

    HI_U32 pn_filter_abnormal_period_cnt;               //零火线识别过滤异常周期次数

    HI_U16 send_query_sta_ntb_by_tf_pkt_cnt;            // 通过台区识别报文查询次数
    HI_U16 rcv_sta_ntb_by_tf_pkt_cnt;                   // 通过台区识别报文接收到NTB回应次数

    HI_U16 rcv_sta_ntb_callback_cnt;                    // 接收到CCO通知的STA_NTB回调次数
    HI_U16 send_notify_collect_cnt;                     // 通知STA进行采集次数

    HI_U8 identify_mode;                                // 当前所处的识别模式，0老模式,1新模式
    HI_U8 notify_state;                                 // 新模式下通知状态
    HI_U8 tf_pkt_identify_cnt;                          // 新模式下启动次数
    HI_U8 cur_period_invalid_cnt;                       // cco_period为0的次数
    HI_U8 tf_lvai_srst_cnt;                            // 相位识别过程中，进行逻辑复位次数
    HI_U8 pad1[3];
    HI_U32 pad[4];
}DIAG_CMD_PI_CCO_STATIC_STRU;

#define DIAG_CMD_PI_CCO_BASE_NTB_NUM_MAX            92          //每次上报的最大NTB个数
//相位识别CCO端的基准NTB上报
typedef struct
{
    HI_U8  index;                                   //报文编号
    HI_U8  pad1;
    HI_U16 next_ntb_store_index;                    //下一个缓存的NTB的索引

    HI_U32 ntb_store_cnt;                           //已缓存的NTB个数

    HI_U32 pad[3];

    HI_U32 ntb[DIAG_CMD_PI_CCO_BASE_NTB_NUM_MAX];
}DIAG_CMD_PI_CCO_BASE_NTB_STRU;

//NTB异常统计
typedef struct
{
    HI_U32 lisr_hisr_err_cnt;                                   //低级中断到高级中断调用时间异常次数
    HI_U32 lisr_hisr_diff_cur;
    HI_U32 lisr_hisr_diff_max;
    HI_U32 lisr_hisr_diff_err;                                  //出现异常时的低级中断与高级中断的时间差，单位us

    HI_U32 lisr_hisr_diff_err_isp;                              //出现异常时的低级中断与高级中断的时间差，ISP计数器,单位us

    HI_U16 revised_zero_period;                                 //校正后的过零周期，单位us
    HI_U16 revise_zero_period_fail_cnt[3];                      //在三相校正过零周期失败计数

    HI_U16 revised_zero_period_invalid_in_send_cnt;             //发送时校正的过零周期非法
    HI_U16 tx_lost_consume_cnt;                                 //被冗错的"过零中断丢失"次数

    HI_U16 stop_in_send_cnt[3];                                 //发送过程中停止的计数
    HI_U16 clear_seq_data_timer_cnt;                            //定时器清除序列数据输出的次数

    HI_U32 tx_lost_times;                                       //发射端丢失中断发生的次数，一次可能丢多个中断
    HI_U32 tx_lost_pre_ntb;                                     //有中断丢失时的前一个NTB
    HI_U32 tx_lost_cur_ntb;                                     //有中断丢失时的后一个NTB
    HI_U32 tx_lost_isp;                                         //丢失中断时的ISP计数差值，单位us

    HI_U32 smaller_than_3ms_cnt;                                //小于3ms的发送中断间隔统计
    HI_U32 smaller_than_300us_cnt;                              //小于300us的发送中断间隔统计
    HI_U32 smaller_than_one_period_cnt;                         //比一个周期小的统计

    HI_U16 set_send_seq_data_lisr_err_cnt;                      //输出码字1的lisr回调异常计数
    HI_U16 set_send_seq_data_hisr_err_cnt;                      //输出码字1的hisr回调异常计数

    HI_U32 set_send_seq_data_cnt;                               //设置输出码字1的次数
    HI_U32 set_send_seq_data_hisr_cnt;                          //设置输出码字1的定时器hisr回调次数

    HI_U16 zero_period_small_cnt;                               //校正后的过零周期小于最小值的次数，53Hz
    HI_U16 zero_period_big_cnt;                                 //校正后的过零周期超过最大值的次数，45Hz

    HI_U32 pad[2];
}DIAG_CMD_QUERY_TF_NTB_STATIC_STRU;

//STA在不同门限检测到不同序列的统计
typedef struct
{
    HI_U16 tei;
    HI_U8  freq;
    HI_U8  snid;

    HI_U16 wobble_scope;                                        //扰动幅度均值
    HI_U16 noise_scope;                                         //噪声幅度均值

    HI_U32 time_stamp;                                          //时间戳，单位:s
    HI_U32 bpc;

    HI_U16 soft_val[TF_ZERO_CROSS_NOISE_LIMIT_CNT];             //不同门限检测出的软值
    HI_U8  seq_index[TF_ZERO_CROSS_NOISE_LIMIT_CNT];            //不同门限检测出的序列

    HI_U8  noise_limit_index;                                   //检测成功时的去噪声门限
    HI_U8  alg_index;                                           //检测成功时的检测算法
    HI_U8  identify_freq;                                       //识别出的频段
    HI_U32 identify_snid;                                       //识别出的snid

    HI_U16 pad0[42];                                            // 3*7*2
    HI_U8  pad1[56];                                            // 4*7*2
    HI_U8  pad2[56];                                            // 4*7*2
}DIAG_CMD_QUERY_TF_STA_SEQ_SUCCESS_INFO_STRU;

//锁定STA的可维可测
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

//STA台区识别接收采数存储结构:可维可测,该结构体大小不能超过TF_OM_NTB_COLLECT_BLOCK_SIZE(1400)
typedef struct
{
    HI_U16 ntb_collect_store_cnt;                                       //NTB当前存储次数计数，该值一直累加直到翻转
    HI_U16 ntb_collect_sn;                                              //NTB采集通知轮次编号

    DIAG_CMD_QUERY_TF_STA_SEQ_SUCCESS_INFO_STRU success_info;           //检测到序列的软值
    HI_U32 ntb[286];                                                    //ntb数据,空间大小(TF_ZERO_CROSS_SEQUENCE_CNT - 1) * TF_ZERO_CROSS_SEQUENCE_SIZE) * 2

    //前面的结构体为1388Bytes
    DIAG_CMD_TF_STA_OM_CMN_STRU cmn;
}DIAG_CMD_TF_STA_OM_STORE_STRU;

//NDM-NDM模式HSO查询ndm台区识别状态和结果
typedef enum
{
    HI_MAC_TF_IDENTIFY_CMD_NDM_TX_MODE_START = 0,
    HI_MAC_TF_IDENTIFY_CMD_NDM_RX_MODE_START,
    HI_MAC_TF_IDENTIFY_CMD_NDM_STOP,
} HI_MAC_NDM2NDM_TF_SET_CMD_REQ_E;

typedef HI_MAC_NDM2NDM_TF_SET_CMD_REQ_E HI_MAC_NDM2NDM_TF_QUERY_STATUS_REQ_E;

typedef enum
{
    HI_MAC_TF_IDENTIFY_DOING,                      // 识别中
    HI_MAC_TF_IDENTIFY_SUCCESS,                   // 识别成功
    HI_MAC_TF_IDENTIFY_FAIL,                       // 识别失败
}HI_MAC_NDM2NDM_TF_IDENTIFY_RESULT_ENUM;
typedef struct
{
    HI_MAC_NDM2NDM_TF_QUERY_STATUS_REQ_E eNdmMode;
    HI_U32 ulTfIdentTime;

    HI_U8  ucStatus;
    HI_U8  ucTfResult;                  // 识别结果，0:识别中，1:识别成功，2:识别失败
    HI_U8  ucSerialNum;
    HI_U8  ucPhase;
    HI_U8  ucLevel;                     // 接收机档位，1M,240K
    HI_U8  stop_reason;             // 发射机异常停止，0:正常，1:电力线谐波异常停止
    HI_U8  ucPad1[2];

    HI_U16 wobble_scope;            // 扰动均值,单位us
    HI_U16 noise_scope;             // 实时噪声均值,单位us
    HI_U32 ulPad[3];
} DIAG_CMD_NDM2NDM_TF_QUERY_STATUS_IND_STRU;

//NDM-NDM模式HSO设置ndm台区识别
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
    HI_U8 synch_state;       // 出现虚警时的同步状态
    HI_U8 limit_index;        // 噪声门限
    HI_U8 alg_index;         // 上下沿
    HI_U8 seq_index;        // 序列
    HI_U8 phase_index;       // 相位
    HI_U8 pad;
    HI_U16 soft_val;            // 软值
}DIAG_CMD_TF_EXACT_FALSE_ENTRY_ST;

typedef struct
{
    HI_U32 exact_false_cnt;          // 虚警出现次数
    DIAG_CMD_TF_EXACT_FALSE_ENTRY_ST exact_false_entry[DIAG_CMD_TF_EXACT_FALSE_CNT];
}DIAG_CMD_TF_EXACT_FALSE_INFO_STRU;

// 芯片端计算统计量
#define DIAG_CMD_TF_VOLTAGE_DETECT_LINE_CNT             (8)         // v200台区识别芯片上报线路个数

typedef struct
{
    HI_U16 v_min;                                            // 光耦最低电压
    HI_U16 v_max;                                            // 光耦最高电压

    HI_U32 power_period_cnt;                                 // 工频周期计数

    HI_U32 power_period_num;                                 // 统计的工频周期计数
    HI_U32 power_period_num_r;                               // 统计的工频周期计数的余数，单位us

    HI_U32 ntb_zerocrossr_local;                             // 上升沿 + 本地时钟的NTB
    HI_U32 ntb_zerocrossf_local;                             // 下降沿 + 本地时钟的NTB
    HI_U32 ntb_zerocrossr_net;                               // 上升沿+ 全网时钟的NTB
    HI_U32 ntb_zerocrossf_net;                               // 下降沿+ 全网时钟的NTB

    HI_U32 time_error1_cnt;                                  // timer_err = 1时，错误计数一次
    HI_U32 time_error2_cnt;                                  // timer_err = 2时，错误计数一次
    HI_U32 time_error3_cnt;                                  //timer_err = 3时，错误计数一次

    HI_U32 photocoupler_error_cnt;                           // 升降沿判断如果丢失了边沿，表示光耦输入异常出现一次，芯片计一次数。接口保留，功能待定

    HI_U8 o_valid;
    HI_U8 gear_switch;
    HI_U8 t_in_valid_cnt;                                    // 每次运算，8个门限中，数据有效的路数，每工频周期刷新一次
    HI_BOOL is_power_period_stat_err;                        // 工频周期统计是否异常

    HI_U32 power_period_err_offset;                          // 工频周期统计异常时数值

    HI_U32 ntb_result;                                       // 跟随数据上报的NTB

    HI_U16 line_noise[DIAG_CMD_TF_VOLTAGE_DETECT_LINE_CNT];  // 芯片端上报噪声
}DIAG_CMD_TF_CHIP_STATICS_STRU;

// 关键节点诊断2.0 可维可测信息
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
#define FL_TOPO_REF_ENTRY_MAX  (2000)   //2000版本修改
#define HI_SHORT_PLC_MAC_ADDR_LEN  (HI_PLC_MAC_ADDR_LEN - 2)
// 预留给基准拓扑的block数
#define HI_SDM_FL_TOPO_REF_BLOCK_NUM    (2)

// 存储块头部信息
#define BLOCK_REC_TYPE_TOPO_NOT_USED    (0)
#define BLOCK_REC_TYPE_TOPO_REF         (1)
#define BLOCK_REC_TYPE_TOPO_CHANGE      (2)

#define FL_TOPO_CHANGE_STA_ADD              (0) //站点入网
#define FL_TOPO_CHANGE_STA_DELETE           (1) //站点删除
#define FL_TOPO_CHANGE_STA_OFFLINE          (2) //站点离线
#define FL_TOPO_CHANGE_STA_CHANGE_PROXY     (3) //站点代理变更

typedef struct _fl_topo_record_parser_st_
{
    HI_U16 recode_type  : 4;
    HI_U16 pad          : 12;
    HI_U16 pad1;
}fl_topo_record_parser_st;

// 站点入网记录
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
// 站点删除记录
typedef struct _fl_topo_delete_record_st_  // 8 bytes
{
    HI_U16 recode_type  : 4;
    HI_U16 tei          : 12;
    HI_U8  reason;
    HI_U8  pad;
    HI_U32  time_stamp;
} fl_topo_delete_record_st;

// 站点离线记录
typedef struct _fl_topo_offline_record_st_  // 8 bytes
{
    HI_U16 recode_type  : 4;
    HI_U16 tei          : 12;
    HI_U8  pad[2];
    HI_U32  time_stamp;
} fl_topo_offline_record_st;

// 站点代理变更记录
typedef struct _fl_topo_change_proxy_record_st_  // 8 bytes
{
    HI_U16 recode_type  : 4;
    HI_U16 tei          : 12;
    HI_U16 proxy_tei    : 12;
    HI_U16 pad          : 4;
    HI_U32 time_stamp;

} fl_topo_change_proxy_record_st;

// 关键节点信息基准拓扑与HSO接口

#define MAC_PLC_QUERY_FL_TOPO_REF_VER_RO   (0)
#define MAC_PLC_QUERY_FL_TOPO_REF_VER_R1   (1)

#define MAC_PLC_QUERY_FL_TOPO_REC_VER_RO   (0)
#define MAC_PLC_QUERY_FL_TOPO_REC_VER_R1   (1)

#define DIAG_CMD_QUERY_FL_TOPO_REF_ENTRY_MAX   (45)


// 关键节点诊断基准拓扑
typedef struct _fl_topo_ref_entry_   // 8 bytes
{
    HI_U8  mac[HI_SHORT_PLC_MAC_ADDR_LEN];                  // 站点MAC地址

    HI_U32 tei          : 12;       // 站点TEI地址
    HI_U32 proxy_tei    : 12;       // 站点父代理TEI地址
    HI_U32 in_use       : 1;        // 站点是否在线
    HI_U32 pad          : 7;
}fl_topo_ref_entry;

typedef fl_topo_ref_entry DIAG_CMD_QUERY_FL_TOPO_REF_ENTRY_STRU  ;

typedef struct _DIAG_CMD_QUERY_FL_TOPO_REF_IND_STRU_   // 376 bytes
{
    HI_U32 fl_ver;                      //关键节点诊断特性版本号
    HI_U32 time_stamp;                  //基准拓扑生成时间戳

    HI_U16 sn;
    HI_U16 total_cnt        : 10;       //需要上报的站点总数
    HI_U16 is_last          : 1;        //本轮请求的最后一个应答报文标识
    HI_U16 pad              : 5;

    HI_U32 num              : 6;        //当前数据包所含的站点数
    HI_U32 pad1             : 26;

    DIAG_CMD_QUERY_FL_TOPO_REF_ENTRY_STRU entry[DIAG_CMD_QUERY_FL_TOPO_REF_ENTRY_MAX];

} DIAG_CMD_QUERY_FL_TOPO_REF_IND_STRU;


// 关键节点信息拓扑变化与HSO接口
typedef struct _DIAG_CMD_QUERY_FL_TOPO_RECORD_HEAD_S_   // 20 bytes
{
    HI_U32 fl_ver;                      //关键节点诊断特性版本号
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

// 清空关键节点信息HSO接口
typedef struct _DIAG_CMD_CLEAR_TOPO_RECORD_IND_STRU_  // 8 bytes
{
    HI_U32 ret;
    HI_U32 pad;
} DIAG_CMD_CLEAR_TOPO_RECORD_IND_STRU;

// 获取停电检测维测信息回复
typedef struct
{
    HI_U32 ret;

    HI_U32 cur_cycle_len;           // 当前检测周期时长，单位s
    HI_U32 cur_cycle_pass_len;      // 本周期已经过时长，单位s
    HI_U32 total_report_on_num;     // 总上报来电次数
    HI_U32 total_found_off_num;     // 总发现停电次数
    HI_U32 total_report_off_num;    // 总上报停电次数

    HI_U16 leave_info_num;          // 离线统计记录个数
    HI_U8  is_init;
    HI_U8  pad;
    HI_U32 last_arrange_time;       // 上一次整理离线统计记录的时间

    HI_U32 heartbeat_duration_th;   // 未收到心跳时长门限，单位s
    HI_U32 unknown_num;             // 未知站点数目
    HI_U32 protect_num;             // 保护站点数目
    HI_U32 on_num;                  // 在线站点数目
    HI_U32 found_off_num;
    HI_U32 report_off_num;          // 上报离线数目
    HI_U32 pad1[2];
} DIAG_CMD_CCO_GET_POWEROFF_INFO_IND_STRU;

// 获取指定站点停电检测维测信息请求
typedef struct
{
    HI_U32 tei;
} DIAG_CMD_GET_STA_POWEROFF_INFO_REQ_STRU;

// 获取指定站点停电检测维测信息回复
typedef struct
{
    HI_U32 ret;

    HI_U8  status; // 停电状态，参考nm_poweroff_status_e
    HI_U8  protect_online_round;
    HI_U16 report_on_cnt;   // 发现并上报来电次数
    HI_U16 found_off_cnt;   // 发现停电次数
    HI_U16 report_off_cnt;  // 上报停电次数(通过离线检测)
    HI_U32 last_off_time;   // 最近发现停电时间
    HI_U32 last_on_time;    // 最近发现上电时间

    HI_U8  lllast_channel_leave_cnt : 4;    // 上上上周期因信道原因离线次数
    HI_U8  llast_channel_leave_cnt  : 4;    // 上上周期因信道原因离线次数
    HI_U8  last_channel_leave_cnt   : 4;    // 上周期因信道离线次数
    HI_U8  cur_channel_leave_cnt    : 4;    // 本周期因信道离线次数

    HI_U16 pad1[21];
} DIAG_CMD_GET_STA_POWEROFF_INFO_IND_STRU;

// 获取站点停电过程记录请求
typedef struct
{
    HI_U32 tei;
} DIAG_CMD_CCO_GET_POWEROFF_HISTORY_REQ_STRU;

typedef struct
{
    HI_U16  proxy_tei;              // 代理站点的TEI
    HI_U16  childsum;               // 子节点的个数
    HI_U8   level;                  // 层级
    HI_U8   sta_phase[3];           // 站点相位
    HI_U8   ability;                // 参考 IS_CCO 宏定义
    HI_BOOL new_joiner;             // 是否为本轮刚加入的站点,心跳检测时区别于老站点,不做心跳检测离线处理的判断用
    HI_BOOL in_use;                 // 站点是否在线的标志
    HI_BOOL is_relay_ability;       // 是否为中继
    HI_U8   d2d_proxy_comm_rate;    // 路径通信率
    HI_U8   proxy_type;             // 代理类型:动态代理、指定静态代理
    HI_U8   topo_down_comm_rate;    // 下行通信成功率
    HI_U8   topo_up_comm_rate;      // 上行通信成功率
    HI_U32  addtime;                // 站点加入时间
    HI_U32  proxy_change_cnt;       // 代理变更次数
    HI_U32  leave_cnt;              // 离线次数
    HI_U32  leave_time;             // 离线时间点，单位:秒
    HI_U32  leave_total_time;       // 离线总时长，单位:秒
    HI_U16  hw_reset_cnt;           // 记录设备硬复位累积次数
    HI_U16  mac_sw_reset_cnt;       // 记录MAC软复位累积次数
    HI_U8   leave_reason;           // 离线原因
    HI_U8   hw_reset_reason;        // 硬重启原因
    HI_U8   sw_reset_reason;        // 软重启原因
    HI_U8   pn_identify_fail_cnt;   // 零火线反接识别失败次数
    HI_U32  last_heartbeat_time;    // 最后一次心跳时间,单位s

    HI_U8  min_multiproxy_comm_rate;        // 路由最小通信率
    HI_U8  proxy_comm_rate;                 // 与代理站点的上行通信率
    HI_U8  proxy_down_comm_rate;            // 与代理站点的下行通信率
    HI_U8  comm_rate;                       // 上下行通信率
    HI_U8  down_comm_rate;                  // 下行通信成功率
    HI_U8  up_comm_rate;                    // 上行通信成功率
    HI_U8  proxy_proxy_comm_rate;           // 路径通信率
    HI_U8  channel_info;                    // 信道质量(beacon帧存在)
    HI_S16 snr;                             // 平均信噪比(SOF帧)
    HI_U8  is_exist : 1;                    // 通过下行路由窗口统计数据，确定该站点是否为本站点发现站点
    HI_U8  network_mode : 1;                // 网络模式:0.PLC模式，1.RF模式
    HI_U8  is_dual_module : 1;              // 是否为双模模块:0.单模模块，1.双模模块
    HI_U8  rsvd1 : 5;
    HI_U8  proxy_channel_info;              // 与代理站点的信道容量
    HI_U32 send_fail_cnt;                   // 发送失败次数
    HI_U32 send_success_cnt;                // 发送成功次数
    HI_U32 rcv_beacon_cnt;                  // 接收该站点BEACON帧的个数
    HI_U32 rcv_cnt;                         // 接收到该站点发给本站点的单播报文(DTEI == my_tei)
} diag_poweroff_his_node_part_st;


// 获取站点停电过程记录回复
typedef struct
{
    HI_U32 ret;

    HI_U32  his_begin_addr;

    HI_U8   mac[HI_PLC_MAC_ADDR_LEN];       // MAC地址
    HI_U16  report_on_cnt;              // 发现并上报来电次数
    HI_U16  found_off_cnt;              // 发现停电次数
    HI_U16  report_off_cnt;             // 上报停电次数(通过离线检测)
    HI_U32  last_off_time;              // 最近发现停电时间
    HI_U32  last_on_time;               // 最近发现上电时间
    HI_U32  last_finish_protect_time;   // 最近保护结束时间

    diag_poweroff_his_node_part_st off_record;
    diag_poweroff_his_node_part_st on_record;
    diag_poweroff_his_node_part_st finish_protect_record;
} DIAG_CMD_CCO_GET_POWEROFF_HISTORY_IND_STRU;

#endif

#define NM_POWEROFF_SAVE_HIS_NUM            (49)
#define NM_DIAG_DISCOVER_TEI_BIT_MAP        (256)

// 查询站点帮助应答停电探测当前信息 280B
typedef struct
{
    HI_U32  ret;

    HI_U8   is_in_delay;
    HI_U8   pad;
    HI_U16  rcv_eq_report_cnt;
    HI_U32  cur_delay_time;     // 当前延迟时间，单位s

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

    HI_U8   need_report_tei_map[NM_DIAG_DISCOVER_TEI_BIT_MAP];  // 本次delay中，必须要应答的站点
} DIAG_CMD_STA_CUR_HELP_REPORT_IND_STRU;

// 查询站点帮助应答停电探测历史信息 280B
typedef struct
{
    HI_U32  ret;

    HI_U8   last_broadcast_map[NM_DIAG_DISCOVER_TEI_BIT_MAP];   // 上次广播位图
    HI_U8   his_report_tei_map[NM_DIAG_DISCOVER_TEI_BIT_MAP];   // 历史帮助应答的记录

} DIAG_CMD_STA_HIS_HELP_REPORT_IND_STRU;

//维测信息结构体
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

// 探测id维测命令
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

// 心跳报文结构
typedef struct
{
    HI_U16 ostei;
    HI_U16 discover_count_tei;
    HI_U32 discover_count;
    HI_U8  discover_tei[NM_DIAG_DISCOVER_TEI_BIT_MAP];
    HI_U8  rsvd[2];
} diag_heartbeat_check_st;


// 查询叶子节点心跳延迟转发信息 200B
typedef struct
{
    HI_U32 ret;

    HI_U8   is_in_trans_delay;  // 是否在转发延迟中
    HI_U8   pad1;
    HI_U16  start_trans_timer_cnt;
    HI_U16  direct_trans_cnt;
    HI_U16  merge_another_hb_cnt;
    HI_U16  trans_handle_timeout_fail_cnt;
    HI_U16  trans_heartbeat_succ_cnt;
    HI_U16  trans_heartbeat_fail_cnt;
    HI_U16  trans_heartbeat_ret_code;

    diag_heartbeat_check_st trans_heart_beat;  // 待转发的心跳报文

    HI_U32  pad[10];
} DIAG_CMD_STA_HB_TRANS_DELAY_IND_STRU;


#endif

/*--------------层间消息 START--------------------------------------------*/
/************************************************************************/
/*   MAC协议栈   层间消息ID    （0x1000~0x5000）                        */
/************************************************************************/
// 范围 [0x1000~0x5000)
// 1， 组网业务消息  [0x1000~0x1400) // 1024
// 2,  路由          [0x1400~0x1800) // 1024
// 3,  发送机        [0x1800~0x1C00) // 1024
// 4,  分段器        [0x1C00~0x1E00) // 512
// 5,  重组器        [0x1E00~0x2000) // 512
// 6,  beacon        [0x2000~0x2200) // 512
// 7,  conv          [0x2200~0x2400) // 512
// 8,  RX            [0x2400~0x2600) // 512
// 9,  NTB           [0x2600~0x2700) // 256
// 10, SOUNDING      [0x2700~0x2800) // 256
// 11, 抄控器        [0x2800~0x2900) // 256
// 12, DFX           [0X2900~0X2A00) // 256
// 13, FA            [0x2A00~0x2B00)
// 14, 保留          [0x2B00~0x5000)
/************************************************************************/
//
//  ID号的格式为: HI_DMID_PS_XXX_XXX
//  对应的结构体:    DMID_PS_XXX_XXX_S
//

/***********************************************************************
 13， 频段自适应消息  [0x2A00~0x2B00)
************************************************************************/
#define HI_DMID_MAC_FA_BASE_ID                      0x2A00
#define HI_DMID_MAC_FA_STA_RESULT                   (0x2A00 + 0x00)  //站点的评估结果

//频段评估中每个站点的测试结果消息定义
typedef struct
{
    HI_U8  evaluate_mode;                                        //当前的评估模式
    HI_U8  cur_freq;                                             //当前频段
    HI_U16 tei;                                                 //本站点的tei

    HI_U16 rcv_cnt;                                             //收到的测试报文数
    HI_U16 pad;

    HI_U32 total_time;                                          //测试时长延时，单位:ms
}HI_DMID_MAC_FA_STA_RESULT_S;

/************************************************************************/
/* 1， 组网业务消息  [0x1000~0x1400) // 1024 */
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
// 2,  路由          [0x1400~0x1800) // 1024
/************************************************************************/
#define ID_DIAG_LAYER_PS_ROUTE                       (0x1400)
#define ID_DIAG_LAYER_PS_ROUTE_AGING                 (0x1401)
#define ID_DIAG_LAYER_PS_ROUTE_FEEDBACK              (0x1402)
#define ID_DIAG_LAYER_PS_ROUTE_DETECT                (0x1403)
#define ID_DIAG_LAYER_PS_ROUTE_EVALUATE              (0x1404)


/************************************************************************/
// 3,  发送机        [0x1800~0x1C00) // 1024
/************************************************************************/
//发送机模块消息定义
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
//发送机模块消息定义
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
    HI_U8             discard_flag;              //MPDU报文的丢弃标志;
    HI_U8             pb_count;                  //PB块的个数;

    HI_U8             phase;                     //相位信息;
    HI_U8             bcf;                       //报文的广播标志位;
    HI_U16            mpdu_index;                //MPDU的索引值;

    HI_U16            pb_size;                   //PB块的大小;
    HI_U16            ctei;                      //交叉站点的终端设备标识符;

    HI_U16            fc_size;                   //报文的帧头大小;
    HI_U16            odtei;                     //目的终端设备标识符;

    HI_U32            expire_time;               //报文的过期时间;

    HI_U8             lid;
    HI_U8             encrypt_flag;
    HI_U8             rsvd;
    HI_U8             mpdu_queue_phase;          //单播报文在多相位模式下，所入队列的相位信息;

    HI_U8             cur_resend ;
    HI_U8             total_resend;
}HI_DMID_MAC_TX_MPDU_INFO_S;

/************************************************************************/
// 4,  分段器        [0x1C00~0x1E00) // 512
/************************************************************************/


/************************************************************************/
// 5,  重组器        [0x1E00~0x2000) // 512
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
// 11, 抄控器        [0x2800~0x2900) // 256
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

// 抄控器通道接收来自HSO的远程命令
typedef struct
{
    HI_U8  ucRole;  // CCO ,STA
    HI_U8  ucMacAddr[4][6];
    HI_U8  ucchannelType;
    HI_U16 usControl  :15;          // 控制字
    HI_U16 usDULink   :1;           // 上下行标识

    HI_U16 usCommandId;
    HI_U16 usTransId;
    HI_U16 payload_size;
    HI_U16 pad;

    HI_U32 sn;
}HI_DMID_MAC_DCM_RCV_REMOTE_CMD_INFO_S;

//抄控器通道转发报文信息
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
    HI_U8  pkt_idx;             //抄控器通道报文索引
    HI_U16 usTransId;

    HI_U16 odtei;
    //HI_U16 usControl;
    HI_U16 usControl  :15;              // 控制字
    HI_U16 usDULink   :1;               // 上下行标识

    HI_U16 usCommandId;
    HI_U16 usFragmentID;
    HI_U16 usFragmentCount;             // 总分包数
    HI_U16 usFragmentTransId; // 一次分包使用同一个TransId.该值通过HI_DIAG_GetFragmentTransId获取
    //HI_U16 pad2;

    HI_U32 sn;

    HI_U16 payload_size;
    HI_U16 pad2;
}HI_DMID_MAC_DCM_PIPE_TRANS_PK_INFO_S;

//抄控器通道回复ACK消息
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
    HI_U16 usControl  :15;          // 控制字
    HI_U16 usDULink   :1;           // 上下行标识

    HI_U16 usCommandId;
    HI_U16 usFragmentID;

    HI_U16 usFragmentCount;// 总分包数
    HI_U16 usFragmentTransId; // 一次分包使用同一个TransId.该值通过HI_DIAG_GetFragmentTransId获取
    //HI_U16 pad2;
}HI_DMID_MAC_DCM_PIPE_REPLY_ACK_INFO_S;

//抄控器通道报文到达目的站点信息
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
    HI_U16 usControl  :15;          // 控制字
    HI_U16 usDULink   :1;           // 上下行标识

    HI_U16 usCommandId;
    HI_U16 usFragmentID;

    //HI_U16 usFragmentCount;            // 总分包数
    HI_U16 usFragmentCount       : 9;    // 总分包数
    HI_U16 rsb_pkt_flag          : 1;    //0:不需要组成一个包上报;1:需要组成一个完整的包上报
    HI_U16 pad                   : 6;    //保留，默认值为0
    HI_U16 usFragmentTransId; // 一次分包使用同一个TransId.该值通过HI_DIAG_GetFragmentTransId获取
    //HI_U16 pad2;
}HI_DMID_MAC_DCM_PIPE_PK_ARRIVED_DEST_STA_INFO_S;

//抄控器通道接收来自PLC报文信息
typedef struct
{
    HI_U8  ucOSAMac[6];
    HI_U8  ucProxyMac[6];
    HI_U8  ucMacAddr[4][6];

    HI_U8  ucchannelType;
    HI_U8  type;
    HI_U16 usTransId;

    //HI_U16 usControl;
    HI_U16 usControl  :15;          // 控制字
    HI_U16 usDULink   :1;           // 上下行标识
    HI_U16 usCommandId;

    HI_U16 usFragmentID;

    //HI_U16 usFragmentCount;       // 总分包数
    HI_U16 usFragmentCount : 9;     // 总分包数
    HI_U16 rsb_pkt_flag : 1;        //0:不需要组成一个包上报;1:需要组成一个完整的包上报
    HI_U16 pad1 : 6;                //保留，默认值为0

    HI_U16 usFragmentTransId;       // 一次分包使用同一个TransId.该值通过HI_DIAG_GetFragmentTransId获取
    //HI_U16 pad;
    HI_U8  pkt_idx;
    HI_U8  pad;

    HI_U32 sn;

    HI_U16 payload_size;
    HI_U16 pad2;
}HI_DMID_MAC_DCM_PIPE_RECV_PK_FROM_PLC_INFO_S;

//抄控器报文入分段队列失败
typedef struct
{
    HI_U8 pkd_idx;          //报文编号
    HI_U8 pad[3];
}HI_DMID_MAC_DCM_SEG_QUEUE_IN_FAIL_INFO_S;

//抄控器报文发送成功
typedef struct
{
    HI_U8 pkd_idx;          //报文编号
    HI_U8 pad[3];
}HI_DMID_MAC_DCM_TX_SUCCESS_INFO_S;

//抄控器报文发送失败
typedef struct
{
    HI_U8 pkd_idx;          //报文编号
    HI_U8 pad[3];
}HI_DMID_MAC_DCM_TX_FAIL_INFO_S;

//抄控器报文发送超时
typedef struct
{
    HI_U8 pkd_idx;          //报文编号
    HI_U8 pad[3];
}HI_DMID_MAC_DCM_TX_TIMEOUT_INFO_S;


//抄控器报文重组超时
typedef struct
{
    HI_U8 pkd_idx;          //报文编号
    HI_U8 pad[3];
}HI_DMID_MAC_DCM_RSB_TIMEOUT_INFO_S;

#define DFX_BUFF_DISP_SIZE_MAX          64
typedef struct
{
    HI_U16 idx;
    HI_U16 size;

    HI_U32 buff[DFX_BUFF_DISP_SIZE_MAX >> 2];
}HI_DMID_MAC_BUFF_DISP_INFO_S;
//抄控器采数跟随策略统计量
typedef struct
{
    HI_U8  follow_ccomac[HI_PLC_MAC_ADDR_LEN];          //配置跟踪CCO的MAC地址
    HI_U8  cur_freq;                                    //当前工作频段
    HI_U8  snid;
    HI_U8  ndm_cur_status;                              //抄控器当前状态:搜网，跟随
    HI_U8  follow_status;                               //当前跟随状态:跟随中，跟随上，跟随失败
    HI_U8  set_follow_snid;                             //下发的跟随SNID
    HI_U8  set_follow_mode;                             //下发的跟随模式
    HI_U8  set_follow_cmd_cnt;                          //下发命令次数
    HI_U8  start_follow_cnt;                            //下发的启动采数跟随次数
    HI_U8  stop_follow_cnt;                             //下发的停止采数跟随次数
    HI_U8  start_follow_success_cnt;                    //下发启动采数跟随成功次数
    HI_U8  stop_follow_success_cnt;                     //下发停止采数跟随成功次数
    HI_U8  start_follow_fail_by_timer_fail_cnt;         //下发启动失败次数
    HI_U8  start_follow_fail_by_invalid_para_cnt;       //由于参数错误引起的启动失败次数
    HI_U8  start_follow_fail_by_cur_is_follow_cnt;      //由于当前已经采数，再次下发命令，不执行次数
    HI_U8  switch_freq_in_full_mode_cnt;                //一次整体遍历中，频段遍历次数
    HI_U8  notify_switch_freq_cnt;                      //全网通知切换频段次数
    HI_U8  continue_follow_fail_cnt;                    //连续跟随失败次数
    HI_U8  resume_cnt;                                  //停止采数，恢复搜网次数
    HI_U8  pad[2];
    HI_U16 rcv_valid_follow_snid_cnt;                   //本周期内收到配置SNID的信标个数，针对V1R1版本
    HI_U32 rcv_pb_cnt;                                  //跟随模式下，收到pB数
    HI_U16 rcv_valid_cco_mac_cnt;                       //本周期内收到配置CCO的个数
    HI_U16 tei;                                         //抄控器获取可用的空闲时隙对应的TEI
    HI_U32 starttime;                                   //抄控器获取可用的空闲时隙的起始时间
    HI_U32 duration;                                    //抄控器获取可用的空闲时隙长度:单位微妙
    HI_U32 rcv_valid_slot_cnt;                          //解析到可用空闲时隙次数
    HI_U32 follow_switch_freq_cnt;                      //跟随中，切换频段次数
    HI_U32 follow_timer_callback_cnt;                   //周期检测定时器回调次数
    HI_U32 start_timer_fail_cnt;                        //启动Timer失败次数
} DIAG_CMD_MAC_NDM_FOLLOW_STAT_STRU;

typedef struct
{
    HI_U8  ccomac[HI_PLC_MAC_ADDR_LEN];    //跟随的CCO的MAC地址
    HI_U8  freq;                           //跟随网络的freq
    HI_U8  snid;
    HI_U32 mode;                           //启动跟随模式，固定跟随，切换频段跟随
    HI_U32 cmd;                            //启动跟随，停止跟随的命令
}DIAG_CMD_MAC_NDM_SET_FOLLOW_REQ_STRU;
#define FL_FOLDER_AND_FILE_NAME_LEN   (25)
#define FL_MAX_TEI                                         (1016)

//HSO下发，启动，停止，查询故障定位命令
typedef enum
{
    HI_MAC_NOTIFY_START_FL_CMD = 1,            //通知启动故障定位功能
    HI_MAC_NOTIFY_STOP_FL_CMD,                    //通知停止故障定位功能
    HI_MAC_NOTIFY_QUERY_FL_STATUS_CMD,   //查询当前故障定位运行状态
}DIAG_CMD_SET_FAULT_LOCAT_ENUM;

//故障定位当前运行状态
typedef enum
{
    HI_MAC_FAULT_LOCAT_STOP,   //当前为定位结束状态
    HI_MAC_FAULT_LOCAT_ON,       //当前为定位中
}DIAG_CMD_FAULT_LOCAT_STATUS_ENUM;

//故障定位版本号
typedef enum
{
    FAULT_LOCAT_ERROR = 0,
    FAULT_LOCAT_VER1,
    FAULT_LOCAT_VER2,
    FAULT_LOCAT_VER3,
}DIAG_MAC_FAULT_LOCAT_VER_ENUM;

// 启动故障定位错误码
typedef enum
{
    HI_MAC_FAULT_LOCAT_ERR_SUCCESS = HI_ERR_SUCCESS,
    HI_MAC_FAULT_LOCAT_ERR_DISABLE = 1,           //当前抄控器不允许启动故障定位
    HI_MAC_FAULT_LOCAT_ERR_NOT_SUPPORT,   //当前抄控器模式下，不支持故障定位
    HI_MAC_FAULT_LOCAT_ERR_ALREADY_START,//当前已经处于故障定位状态
    HI_MAC_FAULT_LOCAT_ERR_ALREADY_STOP, //当前已经停止定位
    HI_MAC_FAULT_LOCAT_ERR_MAC,                    //下发的MAC参数非法
    HI_MAC_FAULT_LOCAT_ERR_FREQ,                  //下发的Freq参数非法
    HI_MAC_FAULT_LOCAT_ERR_SNID,                  //下发的SNID参数非法
    HI_MAC_FAULT_LOCAT_ERR_FOLDER_NAME, //下发的文件名非法
    HI_MAC_FAULT_LOCAT_ERR_SD_NO_EXIST,  // SD卡不存在
    HI_MAC_FAULT_LOCAT_ERR_SD_NO_MORE_SPACE, //SD卡剩余空间不足
    HI_MAC_FAULT_LOCAT_ERR_SD_SAME_NAME,           //SD卡中文件名重复
}DIAG_MAC_FAULT_LOCAT_ERR_ENUM;

//写入SD卡的控制信息
typedef struct
{
    HI_U16 write_sd_cnt;                //写入SD卡次数
    HI_U16 cco_hw_reset_cnt;          //CCO硬重启次数

    HI_U8 stop_reason;                 //停止拓扑录制原因
    HI_U8 pad[3];

    HI_U32 cco_join_time;               //CCO加入网络时间
    HI_U32 run_durtion;                    //故障定位时间
    HI_U32 sys_time;                      //记录该拓扑的系统时间

    HI_U8 freq;                                 //启动定位时网络freq
    HI_U8 snid;                                //启动定位时网络SNID
    HI_U8 change_freq;                  //全网通知NDM切换的freq
    HI_U8 change_freq_info[FREQ_TRY_NUM];
    HI_U8 rsvd[3];
}DIAG_MAC_FAULT_LOCAT_RECODE_CTRL_STRU;

//写入SD卡的拓扑信息
typedef struct
{
    DIAG_MAC_FAULT_LOCAT_RECODE_CTRL_STRU fl_ctrl_info;
    NFC_DIAG_CMD_QUERY_NETWORK_TOPO_ENTRY_S network_topo_entry[FL_MAX_TEI];
}DIAG_MAC_FAULT_LOCAT_NETWORK_TOPO_INFO_STRU;

//停止定位原因
typedef enum
{
    NDM_POWER_FAIL = 0,                                      //抄控器掉电
    STOP_BY_CCO_RESET,                                       //CCO重启
    STOP_BY_CONTINUS_NOT_RCV_TOPO,           //连续刷新拓扑失败，停止
    STOP_BY_HSO,                                                  //HSO下发停止
    STOP_REASON_MAX,
}DIAG_MAC_FAULT_LOCAT_STOP_REASON_ENUM;

//HSO下发命令，启动、停止抄控器故障定位
typedef struct
{
    HI_U8 mode;
    HI_U8 freq;
    HI_U8 snid;
    HI_U8 mac[HI_PLC_MAC_ADDR_LEN];
    HI_U8 pad[2];
    HI_CHAR  folder_name[FL_FOLDER_AND_FILE_NAME_LEN];   //文件夹名称
}DIAG_CMD_FAULT_LOCAT_REQ_STRU;

//HSO下发命令的回应报文
typedef struct
{
    HI_U32 ret_code;  //返回的错误码
    HI_U32 mode; //下发命令
}DIAG_CMD_FAULT_LOCAT_RSP_STRU;

//HSO 下发查询故障定位，回应报文
typedef struct
{
    HI_U8   fault_locat_status;                         //抄控器故障定位当前状态
    HI_U8   freq;
    HI_U8   snid;
    HI_U8   mac[HI_PLC_MAC_ADDR_LEN];
    HI_CHAR folder_name[FL_FOLDER_AND_FILE_NAME_LEN];   //文件夹名称
    HI_U8   pad[2];
    HI_U32  run_durtion;                                //定位运行时间
}DIAG_CMD_QUERY_FAULT_LOCAT_RSP_STRU;

typedef struct
{
    HI_U8  mac[HI_PLC_MAC_ADDR_LEN];                // MAC地址
    HI_U8  freq;                                    // 频段
    HI_U8  snid;                                    // 网络号
    HI_U8  curr_status;                             // 当前状态
    HI_U8  hso_start_cnt;                           // 启动诊断次数
    HI_U8  hso_stop_cnt;                            // 停止诊断次数
    HI_U8  hso_query_cnt;                           // 查询诊断结果次数
    HI_U8  stop_by_not_rcv_topo_cnt;                // 由于接收到拓扑信息停止次数
    HI_U8  stop_by_cco_reset_cnt;                   // 由于CCO复位停止次数
    HI_U16 cco_reset_cnt;                           // CCO复位次数
    HI_U32 cco_join_time;                           // CCO入网时间
    HI_U8  sd_check_exist_fail_cnt;                 // SD卡检测不存在计数
    HI_U8  sd_space_not_enough_cnt;                 // SD卡空间不存在计数
    HI_U8  sd_set_time_fail_cnt;                    // SD卡设置时间失败计数
    HI_U8  sd_folder_same_name_fail_cnt;            // SD卡文件名重复计数
    HI_U8  write_topo_to_sd_fail_cnt;               // 拓扑写入SD卡失败计数
    HI_U8  malloc_fail_cnt;                         // 申请空间失败
    HI_U8  change_freq;                             // 频段切换
    HI_U8  change_freq_info[FREQ_TRY_NUM];          // 频段切换信息
    HI_U8  rsvd[3];
    HI_U8  invalid_parameter_cnt;                   // 参数非法计数
    HI_U8  create_folder_fail_cnt;                  // 创建文件夹失败计数
    HI_U8  continus_not_rcv_topo_cnt;               // 持续接收不到拓扑计数
    HI_U8  hso_stop_write_sd_cnt;                   // 停止写SD卡计数
    HI_U16 send_refresh_topo_cnt;                   // 发送拓扑刷新计数
    HI_U16 topo_write_sd_cnt;                       // 拓扑写SD卡计数
    HI_U32 send_remote_cnt;                         // 发送远程命令计数
    HI_U32 send_remote_null_para_cnt;               // 发送远程命令空参数计数
    HI_U16 send_remote_malloc_fail_cnt;             // 发送远程命令申请空间失败计数
    HI_U16 send_remote_fail_cnt;                    // 发送远程命令失败计数
    HI_U32 send_msg_fail_cnt;                       // 发送消息失败计数
    HI_U32 run_time;                                // 运行时间
    HI_U8  get_share_mem_fail_cnt;                  // 获取共享内存失败计数
    HI_U8  pad[2];
    HI_CHAR folder_name[FL_FOLDER_AND_FILE_NAME_LEN]; // 文件夹名称
}DIAG_CMD_QUERY_FAULT_LOCAT_STAT_STRU;

typedef struct
{
    HI_U8  cco_mac[HI_PLC_MAC_ADDR_LEN];             // 锁定网络的CCO MAC地址
    HI_U8  lock_status;                              // 锁定状态
    HI_U8  lock_status_identify;                     // 台区识别功能下锁定状态

    HI_U32 lock_begin_time;                          // 锁定开始时间
    HI_U32 lock_duration;                            // 锁定时长
    HI_U32 reject_time;                              // 锁定网络拒绝时间

    HI_U32 lock_cnt;                                 // 锁定次数
    HI_U32 reject_cnt;                               // 锁定网络拒绝次数
    HI_U32 lock_timeout_cnt;                         // 锁定超时此时
    HI_U16 lock_fail_by_not_join_cnt;                // 未加入网络锁定失败
    HI_U16 send_msg_fail_cnt;                        // 消息发送失败
    HI_U8  lock_freq_identify;                       // 锁定网络频段号
    HI_U8  rsv;
    HI_U8  cco_mac_identify[HI_PLC_MAC_ADDR_LEN];    // 台区识别锁定网络的CCO MAC地址
    HI_U32 lock_snid_identify;                      //  锁定网络网络号
    HI_U32 lock_begin_time_by_identify;              // 锁定开始时间，单位S
    HI_U32 lock_freqsnid_duration_by_identify;       // 锁定时长，单位S
    HI_U32 lock_ccomac_duration_by_identify;         // 锁定CCO MAC时长，单位S
    HI_U32 unlock_time_by_identify;                  // 解锁时间(维测)
    HI_U32 lock_freqsnid_cnt_by_identify;            // 台区识别锁定频段和网络号次数(维测)
    HI_U32 lock_ccomac_cnt_by_identify;              // 台区识别锁定CCO MAC次数(维测)
    HI_U32 unlock_cnt_by_identify;                   // 台区识别解锁次数(维测)
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
    HI_U8  data_size;                       //数据长度
    HI_U8  comm_meteraddr[HI_PLC_MAC_ADDR_LEN];
}DIAG_CMD_PTPC_TEST_REQ_STRU;
typedef struct
{
    HI_U32 ret;
}DIAG_CMD_PTPC_TEST_INFO_STRU;

#ifdef PRODUCT_CFG_ONLINE_TEST_MODE
// GT 挂网测试SD存储测试结果
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

// GT挂网测试报文存储结构
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
    HI_BOOL curr_freq_tmi_all_sent;  // 当前发送频段中，所有tmi都已经尝试发送过了
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


/*--------------层间消息 END--------------------------------------------*/
/*无线TX模块记数器*/
typedef struct
{
    HI_U32 beacon_enqueue_cnt;          // BEACON帧入队列个数
    HI_U32 sof_enqueue_cnt;             // SOF帧入队列个数
    HI_U32 sack_enqueue_cnt;            // sack帧入队列个数
    HI_U32 link_test_cnt;               // 链路测试报文队列个数
    HI_U32 send_beacon_cnt;             // 发送BEACON帧个数
    HI_U32 send_sof_cnt;                // 发送SOF帧个数
    HI_U32 send_sack_cnt;               // 发送sack帧个数
    HI_U32 send_short_beacon_cnt;       // 发送短beacon报文个数
    HI_U32 send_link_test_cnt;          // 发送链路测试报文个数
    HI_U32 send_beacon_fragment_cnt;    // 发送BEACON帧分片个数
    HI_U32 send_sof_fragment_cnt;       // 发送SOF帧分片个数
    HI_U32 send_sack_fragment_cnt;      // 发送sack帧分片个数
    HI_U32 backoff_success_cnt;         // 退避成功次数
    HI_U32 backoff_fail_cnt;            // 退避失败次数
    HI_U32 send_timeout_cnt;            // 发送超时次数
    HI_U32 creat_backoff_timer_fail_cnt;// 创建退避定时器失败次数
    HI_U32 creat_send_timer_fail_cnt;   // 创建发送超时定时器失败次数
    HI_U32 creat_sack_timer_fail_cnt;   // 创建等待sack超时定时器失败次数
    HI_U32 over_queue_threshold_cnt;    // 超过队列水线报文丢弃总计数;
    HI_U32 packet_timeout_err_cnt;      // 报文超时错误
    HI_U8  current_pk_cnt;              // 当前队列中报文个数
    HI_U8  pad[3];
    HI_U32 send_command_cnt;            // 发送命令次数
    HI_U32 write_tx_buf_cnt;            // 写发送缓存次数
    HI_U32 sack_timout_cnt;             // sack超时计数
    HI_U32 write_tx_buf_fail;           // 写buff失败计数
    HI_U32 rf_chip_reset_cnt;           // rf芯片重启计数
    HI_U32 rf_send_timeout_reset_cnt;   // 发送超时重启计数
    HI_U32 send_beacon_pb_fail_cnt;     // 发送beacon pb块失败计数

}DIAG_CMD_QUERY_RF_TX_STAT_STRU;

/*无线RX模块记数器*/
typedef struct
{
    HI_U32 rcv_beacon_cnt;                     // 接收BEACON帧个数
    HI_U32 rcv_sof_cnt;                        // 接收SOF帧个数
    HI_U32 rcv_short_beacon_cnt;               // 接收短beacon报文个数
    HI_U32 rcv_link_test_cnt;                  // 接收短链路检测报文个数
    HI_U32 rcv_beacon_fragment_cnt;            // 接收BEACON帧分片个数
    HI_U32 rcv_sof_fragment_cnt;               // 接收SOF帧分片个数
    HI_U32 beacon_crc_fail_cnt;                // BEACON帧校验失败个数
    HI_U32 short_beacon_fail_cnt;              // 短BEACON帧失败个数
    HI_U32 link_test_fail_cnt;                 // 短链路检测报文失败个数
    HI_U32 rcv_incomplete_cnt;                 // 接收不完整报文个数
    HI_U32 rcv_beacon_incomplete_cnt;          // 接收信标帧不完整个数
    HI_U32 rcv_invalid_cnt;                    // 接收异常报文
    HI_U32 interrupt_cnt;                      // 中断个数
    HI_U32 preamble_interrupt_cnt;             // 前导中断个数
    HI_U32 send_complete_interrupt_cnt;        // 发送完成中断个数
    HI_U32 rcv_interrupt_cnt;                  // 接收中断个数
    HI_U32 tx_buf_empty_cnt;                   // 发送缓存空次数
    HI_U32 rx_buf_full_cnt;                    // 接收缓存满次数
    HI_U32 error_cmd_cnt;                      // 错误命令次数
    HI_U32 fifo_flow_cnt;                      // 收发缓存溢出次数
    HI_U32 rcv_sof_incomplete_cnt;             // 接收信标帧不完整个数
    HI_U32 rcv_crc_fail_cnt;                   // 中断上报crc错误个数
    HI_U32 rcv_rebuild_fail;                   // 缓冲重组失败
    HI_U32 rcv_rebuild_incomplete;             // 缓冲重组不完整
    HI_U32 rcv_return_sack_packe;              // 接收SACK个数
    HI_U32 rcv_return_sack_pb;                 // 接收PB个数
    HI_U32 rcv_pb_err_cnt;                     // 接收PB crc错误个数
    HI_U32 rcv_no_self_pkt_cnt;                // 接收到非本站点报文数
    HI_U32 rcv_list_err;                       // 接收队列链表错，
    HI_U32 rcv_pkt_leng_err;                   // 接收报文长度错误
    HI_U16 rcv_pkt_size_too_short_err;         // 接收报文长度过短错误
    HI_U16 rcv_pkt_size_too_long_err;          // 接收报文长度过长错误
    HI_U32 recode_path_info_err;               // 记录RF路径信息失败
    HI_U32 rcv_difference_bpc_beacon_cnt;      // 接收到的信标周期不同的信标个数
    HI_U32 rcv_self_snid_short_beacon_cnt;     // 接收到自己网络短信标个数
    HI_U32 rcv_beacon_max_difference;          // 两个信标之间的最大时间差
    HI_U32 occurence_time_max_difference;      // 最大时间差发生的时间
    HI_U32 rcv_beacon_min_difference;          // 两个信标之间的最小时间差
    HI_U32 rcv_last_beacon_max_difference;     // 上次两个信标之间的最大时间差
    HI_U32 last_occurence_time_max_difference; // 上次最大时间差发生的时间
    HI_U32 rcv_last_beacon_min_difference;     // 上次两个信标之间的最小时间差
    HI_U32 beacon_differenc_big_period_cnt;    // 两信标之前时间差大于信标周期+30ms的次数
    HI_U32 beacon_differenc_little_period_cnt; // 两信标之前时间差小于信标周期-30ms的次数

}DIAG_CMD_QUERY_RF_RX_STAT_STRU;
typedef struct
{
    HI_U8    is_dual_module;          // 是否为双模模块
    HI_U8    network_mode;            // 网络模式
    HI_BOOL  rf_communication;        // RF通信情况
    HI_U8    plc_communication;       // PLC通信情况(暂时未使用，待以后扩展)
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
    HI_U8  cco_mac[6];                  // 加入网络CCO的MAC地址
    HI_U8  mac_type;                    // MAC地址类型
    HI_U8  is_dual_module;              // 是否为双模模块

    HI_U8  main_network_snid;           // 主网络号(信标帧中payload中携带的主网络号)
    HI_U8  rf_network_snid;             // RF站点主网络号
    HI_U8  sub_network_snid;            // RF子网络号
    HI_U8  network_mode;                // 网络模式

    HI_U8  network_mode_policy;         // 网络模式策略
    HI_U8  pad[3];
}DIAG_CMD_QUERY_STA_INFO_STRU;

typedef struct
{
    HI_U16 tei;
    HI_U16 proxy_tei;               // 代理站点
    HI_U16 counnected_sta_num;      // 可连通站点个数
    HI_U16 pad;

    HI_U32 connected_graph[64];     // 连通图
}DIAG_CMD_QUERY_CONNECTED_GRAPH_IND_STRU;

typedef struct
{
    HI_U8 comm_rate_th;                     // 通信率门限
    HI_U8 comm_rate_is_enable;              // 是否启用通信率门限
    HI_S8 snr_th;                           // 平均信噪比门限
    HI_U8 snr_is_enable;                    // 是否启用平均信噪比门限

    HI_U8 down_comm_rate_th;                // 下行通信率门限
    HI_U8 down_comm_rate_is_enable;         // 是否启用下行通信率门限
    HI_U8 up_comm_rate_th;                  // 上行通信率门限
    HI_U8 up_comm_rate_is_enable;           // 是否启用上行通信率门限
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

/* 增加CCO连接NDM的记录*/
#define DFX_STAT_CONNECT_NDM_MAX   20   // 统计CCO连接NDM的次数
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

//抄控器采集过零NTB的请求
typedef struct
{
    HI_U16 cmd;
    HI_U16 time;                                                        //数据采集时长，单位s

    HI_U8 rcv_level;
    HI_U8 pad1[3];

    HI_U32 pad[3];
}DIAG_CMD_TF_COLLECT_DATA_ZERO_CROSS_NTB_REQ_STRU;

#define DIAG_CMD_TF_COLLECT_DATA_ZERO_CROSS_NTB_NUM_MAX             95  //上报报文中包含的最大NTB个数
//抄控器采集过零NTB的请求
typedef struct
{
    HI_U16 index;                                                       //报文索引，编号从0开始
    HI_U16 num;                                                         //报文中的数据个数

    HI_U32 pad2;

    HI_U32 data[DIAG_CMD_TF_COLLECT_DATA_ZERO_CROSS_NTB_NUM_MAX];       //NTB数据
}DIAG_CMD_TF_COLLECT_DATA_ZERO_CROSS_NTB_RSP_STRU;

#define TF_NOISE_TH_CNT                     6
//台区识别扰动的统计信息
typedef struct
{
    HI_U8 static_wobble_cnt[TF_NOISE_TH_CNT];
    HI_U8 static_wobble_avg[TF_NOISE_TH_CNT];
}DIAG_CMD_TF_WOBBLE_INFO_STRU;

//无台区识别序列发送时扰动时统计窗的最大个数
#define DIAG_CMD_WOBBLE_WITHOUT_SIGNAL_STORE_WINDOW_CNT_MAX     10

//无台区识别序列发送时的扰动统计
typedef struct
{
    HI_U32 bpc;
    HI_U32 ntb;

    DIAG_CMD_TF_WOBBLE_INFO_STRU sta_noise_statistics[DIAG_CMD_WOBBLE_WITHOUT_SIGNAL_STORE_WINDOW_CNT_MAX]; //10个窗
}DIAG_CMD_TF_WOBBLE_WITHOUT_SIGNAL_STRU;

//有台区识别序列发送时的扰动统计
typedef struct
{
    HI_U8 seq_index_ascend;                                                         //识别出的台区序列编号
    HI_U8 seq_index_descend;
    HI_U8 pad[2];

    HI_U32 bpc_ascend;
    HI_U32 ntb_ascend;
    DIAG_CMD_TF_WOBBLE_INFO_STRU signal_ascend[TF_STA_WINDOW_MAX_CNT];     //上升沿4个窗
    DIAG_CMD_TF_WOBBLE_INFO_STRU noise_ascend;                                      //上升沿1个窗

    HI_U32 bpc_descend;
    HI_U32 ntb_descend;
    DIAG_CMD_TF_WOBBLE_INFO_STRU signal_descend[TF_STA_WINDOW_MAX_CNT];    //下降沿4个窗
    DIAG_CMD_TF_WOBBLE_INFO_STRU noise_descend;                                     //下降沿1个窗
}DIAG_CMD_TF_WOBBLE_WITH_SIGNAL_STRU;


/*STA占空比发送可维可测信息*/
typedef struct
{
    HI_U8 duty_start_cnt;
    HI_U8 duty_stop_cnt;
    HI_U8 start_fail_cnt;
    HI_BOOL is_duty_on;

    HI_U32 max_sendlength;                // 下发的占空比最大发送时长，单位us

    HI_U8 duty_percent;                   // 下发的占空比，
    HI_U8 original_tx_power;              // 原始发送功率
    HI_U8 adjust_tx_power;                // 调整后发送功率
    HI_U8 check_zero_cross_int_cnt;       // 接收到过零中断个数

    HI_U32 remain_time_not_enough_cnt;
    HI_U32 sack_duty_cnt;
    HI_U32 sound_sack_duty_cnt;
    HI_U32 csma_duty_cnt;
    HI_U32 fresh_record_cnt;
    HI_U32 max_period1_duty_percent;      //周期1内最大占空比
    HI_U32 cur_period1_duty_percent;      // 周期1内当前占空比
    HI_U32 max_period2_duty_percent;      // 周期2内最大占空比
    HI_U32 cur_period2_duty_percent;      // 周期2内当前占空比
    HI_U32 max_period3_duty_percent;      // 周期3内最大占空比
    HI_U32 cur_period3_duty_percent;      // 周期3内当前占空比

    HI_U32 voltage_interrupt_fall_cnt;    // 电压跌落触发中断次数
    HI_U32 start_check_zero_cross_cnt;    // 开始进行检查过零中断次数
    HI_U32 voltage_recovery_cnt;          // 电压恢复次数
    HI_U32 low_voltage_cnt;               // 检测到低电平次数
    HI_U32 enable_adjust_tf_policy_cnt;   //电压跌落策略启用次数

    HI_U32 false_voltage_fall_cnt;        // 检查到假的电压跌落次数，没有过零
    HI_U32 decrease_tx_power_cnt;         // 减小发送功率次数
    HI_U32 increase_tx_power_cnt;         // 提升发送功率次数
}DIAG_CMD_TX_DUTY_STATICS_STRU;

#define DFX_MAX_IDENTIFY_RESULT_ENTRY_NUM   (15)
typedef struct
{
    HI_U32 freq : 7;                        // 频段号
    HI_U32 snid : 24;                       // 网络号
    HI_U32 is_valid : 1;                    // 数据是否有效

    HI_U8 pad0[2];
    HI_U8  cco_mac[HI_PLC_MAC_ADDR_LEN];    // CCO MAC地址

    HI_U16 signal_mean;                     // 信号均值
    HI_U16 noise_mean;                      //噪声均值

    HI_U16 soft_val;                        //软值
    HI_U8 seq_type;                         // 码字类型
    HI_U8 pad;                              //保留

    HI_U32 last_refresh_time;               //上一次刷新的时间，单位s
}DIAG_CMD_HIS_RESULT_ENTRY_STRU;

typedef struct
{
    DIAG_CMD_HIS_RESULT_ENTRY_STRU max;     //历史最大扰动
    DIAG_CMD_HIS_RESULT_ENTRY_STRU entry[DFX_MAX_IDENTIFY_RESULT_ENTRY_NUM];
}DIAG_CMD_HIS_RESULT_CACHE_STRU;

//当前识别结果
typedef struct
{
    HI_U32 freq : 7;                        // 频段号
    HI_U32 snid : 24;                       // 网络号
    HI_U32 is_valid : 1;                    // 数据是否有效

    HI_U8 pad0[2];
    HI_U8  cco_mac[HI_PLC_MAC_ADDR_LEN];    //CCO_MAC

    HI_U16 signal_mean;                     //信号均值
    HI_U16 noise_mean;                      //噪声均值

    HI_U16 soft_val;                        //软值
    HI_U8 seq_type;                         // 码字类型
    HI_U8 pad;                              //保留

    HI_U32 seq_refresh_time;                //序列上一次刷新的时间，单位s
    HI_U32 bcn_refresh_time;                //信标上一次刷新的时间，单位s
}DIAG_CMD_CUR_RESULT_ENTRY_STRU;

typedef struct
{
    HI_U32 last_seq_time;                   //最后一次收到扰动的识别，单位s
    DIAG_CMD_CUR_RESULT_ENTRY_STRU entry[DFX_MAX_IDENTIFY_RESULT_ENTRY_NUM];
}DIAG_CMD_CUR_RESULT_CACHE_STRU;


//采集台区识别大噪声数据
// 采集每个数据
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

    HI_U32  change_snid;                        // 未使用
    HI_U32  last_report_time;                   // 上次上报的时间
    HI_U32  report_cnt;                         // 上报次数
    HI_U32  bpc_exception_cnt;                  // 信标周期计数异常总次数
    HI_U32  cco_mac_exception_cnt;              // CCO MAC地址异常总次数
    HI_U32  change_snid_time;                   // 变更SNID的时间
    HI_U8   conflict_cco_mac[6];                // 冲突网络CCO MAC地址
    HI_U16  exception_by_bpc_cnt;               // 信标周期计数异常次数

    HI_U32  bpc_exception_detect_cnt;           // 信标周期计数判定冲突次数
    HI_U32  cco_mac_exception_detect_cnt;       // CCO MAC地址判定冲突次数

    HI_U32  rcv_query_req_cnt;                  // 接收查询请求次数
    HI_U32  send_query_response_cnt;            // 发送查询响应次数
    HI_U32  query_sn;                           // 查询序列号
    HI_U32  query_time;                         // 查询时间
    HI_U32  random_backoff_time;                // 随机退避时间
    HI_U32  send_query_result_time;             // 发送查询结果报文时间

    HI_U32  tatol_send_query_req_cnt;           // 发送查询请求次数
    HI_U32  rcv_conflict_report_cnt;            // 接收冲突上报次数
    HI_U32  rcv_query_response_cnt;             // 接收查询响应次数
}DIAG_CMD_COORDINATE_CONFLICT_STAT_STRU;

typedef struct
{
    HI_U8  cco_mac[6];                          // CCO MAC地址
    HI_U8  form_serial_number;                  // 组网序列号
    HI_U8  state;                               // 状态
}DIAG_CMD_COORDINATE_CCO_ENTRY_STRU;

typedef struct
{
    DIAG_CMD_COORDINATE_CCO_ENTRY_STRU cco_entry[6];
}DIAG_CMD_COORDINATE_MULTI_CCO_INFO_STRU;
// 设置多网络监听时间
typedef struct
{
    HI_U32 listen_time;
}DIAG_CMD_LISTEN_TIME_BEFORE_FIRST_BEA_REQ_STRU;

typedef struct
{
    HI_U32 ret_code;  //返回的错误码
}DIAG_CMD_LISTEN_TIME_BEFORE_FIRST_BEA_RSP_STRU;

// 记录STA,NDM收到的全网切换频段信息
typedef struct
{
    HI_U8 mac[HI_PLC_MAC_ADDR_LEN];
    HI_U8 freq;
    HI_U8 rcv_freq_bit_map;   // 接收到的所有频段的bitmap，第1bit表示频段1，第2bit表示频段2，第3bit表示频段3，第4bit表示频段4，第5bit表示频段5
    HI_U32 rcv_sys_time;
    HI_U32 switch_freq_cnt;
}DIAG_CMD_MAC_SWITCH_FREQ_ENTRY_ST;

typedef struct
{
    HI_U8  usta_state;                          // 当前状态
    HI_U8  ntb_state;                           // NTB同步状态
    HI_U8  power_supply_mode;                   // 供电类型
    HI_U8  target_sta_snid;                     // 目标站点网络号
    HI_U16 target_sta_tei;                      // 目标站点TEI地址
    HI_U8  meter_addr[6];                       // 目标站点的MAC地址

    HI_U8  active_search_state;                 // 主动搜索状态
    HI_U8  sweep_freq_index;                    // 当前使用扫频索引
    HI_U8  search_frequency_range_cnt;          // 搜索轮次
    HI_U8  sta_state;                           // STA状态

    HI_U32 passive_search_begin_time;           // 被动搜索开始时间，单位S
    HI_U32 passive_search_network_time;         // 当前网络搜索时间，单位S
    HI_U32 passive_search_freq_time;            // 当前频段搜索时间，单位S
    HI_U32 passive_lock_synch_time;             // 被动同步锁定时间
    HI_U32 last_data_transmission_time;         // 最后数据传输时间，单位S
    HI_U32 last_listen_time;                    // 最后监听时间，单位S
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
    HI_U32 rcv_packet_cnt_from_phy;             // 从PHY接收到报文个数
    HI_U32 last_rcv_packet_from_phy_time;       // 最后从PHY接收到报文时间
    HI_U32 rcv_packet_cnt_from_app;             // 从APP接收到报文个数
    HI_U32 last_rcv_packet_from_app_time;       // 最后从APP接收到报文时间

    HI_U32 discard_packet_cnt;                  // 丢弃的报文
    HI_U32 send_packet_to_app_cnt;              // 发送报文到APP的报文个数
    HI_U32 send_search_cnt;                     // 发送搜索帧个数
    HI_U32 last_send_search_time;               // 最后发送搜索帧时间

    HI_U16 sta_enter_normal_state_err_cnt;      // STA进入正常状态时异常计数
    HI_U16 pad3;                                // 接收搜索帧个数
    HI_U32 pad2;                                // 最后接收搜索帧时间
    HI_U32 send_search_response_cnt;            // 发送搜索响应帧个数
    HI_U32 last_send_search_response_time;      // 最后发送搜索响应帧时间

    HI_U32 rcv_send_search_response_cnt;        // 接收搜索响应帧个数
    HI_U32 last_rcv_search_response_time;       // 最后接收搜索响应帧时间
    HI_U32 send_synch_frame_cnt;                // 发送同步帧个数
    HI_U32 last_send_synch_frame_time;          // 最后发送同步帧时间

    HI_U16 rcv_packet_from_app_fail;            // 从APP接收报文失败个数
    HI_U16 send_packet_to_app_fail_cnt;         // 发送报文到APP的报文失败个数
    HI_U32 passive_search_freq_cnt;             // 被动搜索频段切换次数
    HI_U32 send_packet_fail_cnt;                // 发送报文失败的次数
    HI_U32 send_fail_to_active_search_cnt;      // 发送报文失败而切换到主动搜索状态的次数

    HI_U16 heartbeat_timer_cnt;                 //心跳定时器计数
    HI_U16 send_msg_err_cnt;                    //发送消息失败计数
    HI_U16 start_timer_err_cnt;                 //启动定时器失败计数
    HI_U16 duplicate_pkt_cnt;                   //被过滤掉的重复报文计数

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
    HI_BOOL is_detecting;                       //是否正在探测
    HI_U8  business_state;                      //业务状态
    HI_U8  pad[2];

    HI_U32 start_cnt;                           //启动次数
    HI_U32 stop_cnt;                            //停止次数

    HI_U32 cod_send_cnt;                        //协调帧发送次数
    HI_U32 detect_bcn_send_cnt;                 //探测信标发送次数
    HI_U32 detect_bcn_rcv_cnt;                  //探测信标接收次数
    HI_U32 ptcl_sg_to_v100_cnt;                 //协议版本由国网变为V100的次数
    HI_U32 ptcl_v100_to_sg_cnt;                 //协议版本由V100变为国网的次数

    HI_U16 send_msg_err_cnt;                    //发送消息失败计数
    HI_U16 set_business_state_cnt;              //APP设置业务状态的计数

    HI_U32 pad1[6];
}DIAG_CMD_QUERY_PTCL_DETECT_STAT_STRU;

typedef struct
{
    HI_U16 ptcl_ver;                            //协议版本号
    HI_U16 pad1;

    HI_U32 nid;                                 //网络nid
    HI_U32 select_nid;                          //优先的nid
    HI_U32 ptcl_detect_nid;                     //协议探测的nid

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
	HI_U32 broadband_send_cnt;         //宽带发送计数
	HI_U32 broadband_rcv_cnt;          //宽带接收计数
	HI_U32 sc_send_cnt;                //单载波发送计数
	HI_U32 sc_rcv_cnt;                 // 单载波接收计数

	HI_U32 sc_report_pkt_cnt;         // 单载波接收上报计数
	HI_U32 broadband_report_pkt_cnt;  // 宽带接收上报计数
	HI_U32 sc_last_send_pkt_ntb;         // 单载波最后一次发送NTB时间
	HI_U32 bb_last_send_pkt_ntb;         // 宽带最后一次发送NTB时间
	HI_U32 last_report_pkt_ntb;       // 最后一次接收上报NTB时间
	HI_U32 report_fail_cnt;           // 上报失败计数
	HI_U32 sack_report_pkt_cnt;       //扩展sack报文上报计数

	HI_U32 bb_sof_send_cnt;
	HI_U32 bb_sack_send_cnt;
	HI_U32 app_rcv_cnt;
}DIAG_CMD_FAST_SEND_STRU;


//无扰识别统计量
typedef struct
{
    HI_U8 is_identifying;                              // 正在识别中
    HI_BOOL is_support_tfp;                            // 是否支持无扰识别
    HI_BOOL is_allow_tfp;                              // 是否允许进行无扰
    HI_U8 query_multi_cnt;                             // 查询是否存在多网络次数

    HI_U16 identify_start_success_cnt;                 // cco端启动无扰识别成次数
    HI_U16 identify_start_fail_disable_cnt;            // 不允许启动次数

    HI_U16 identify_start_fail_doing_cnt;              // 现在正在识别中，再次启动失败
    HI_U16 identify_start_packet_success_cnt;          // 启动每包的识别次数

    HI_U16 malloc_err_cnt;                             // 申请空间失败
    HI_U16 send_query_neighbor_info_req_cnt;           // 发送查询是否为多网络报文

    HI_U16 rcv_query_neighbor_info_req_cnt;            // 收到查询是否为多网络报文
    HI_U16 send_query_sta_ntb_req_cnt;                 // 发送查询本网络STA的NTB报文

    HI_U16 rcv_query_sta_ntb_req_cnt;                  // 收到查询本站点STA的NTB报文
    HI_U16 rcv_query_sta_ntb_rsp_cnt;

    HI_U16 send_notify_sta_belong_cnt;                 // 发送通知STA台区归属报文
    HI_U16 rcv_notify_sta_belong_cnt;                  // 收到通知归属报文

    HI_U16 identify_turn_cnt;                          // 启动轮次
    HI_U16 send_query_req_alloc_msdu_buff_err_cnt;     // 发送报文时，申请buffer失败次数

    HI_U16 send_msdu_to_tx_err_cnt;                    // 发送并发查询报文，发送失败次数
    HI_U16 send_msg_err_cnt;                           // 发送消息失败次数

    HI_U16 rcv_query_rsp_err_cnt;                      // 接收回应报文失败次数
    HI_U16 rcv_query_rsp_unexpect_cnt;

    HI_U16 query_timer_timeout_cnt;                    // 查询定时器回调次数
    HI_U16 query_timeout_sta_cnt;

    HI_U16 query_one_turn_over_cnt;                    // 查询一轮识别完成次数
    HI_U16 cco_send_broadcast_notify_cnt;              //  CCO广播通知本网络启动识别次数

    HI_U16 broadcast_send_neighbor_notify_cnt;         // CCO,STA广播通知邻台区启动无视识别次数
    HI_U16 sta_send_unicast_notify_to_cco_cnt;         // STA单播通知给CCO启动无扰识别次数

    HI_U16 cco_rcv_unicast_notify_cnt;                 // CCO 收到STA单播通知的次数
    HI_U16 start_timer_err_cnt;                        // 启动timer失败次数

    HI_U16 rcv_notify_start_cnt;                       //  STA收到启动通知次数
    HI_U16 rcv_neighbor_notify_cnt;                    // 广播收到邻台区启动次数

    HI_U32 identify_run_time;                          // 识别时间

    HI_U8 one_turn_identify_state;                     // 一轮识别状态
    HI_U8 neighbor_cnt;                                // 周围网络个数
    HI_BOOL is_select_sta;                             // 是否指定跨网络站点
    HI_BOOL is_have_multi;                             // 是否存在多网络

    HI_U8 identify_mode;                               // 触发识别模式，参见TFP_IDENTIFY_MODE_E
    HI_U8 sta_send_notify_proxy_cnt;                   // STA站点创建的无扰识别触发报文次数
    HI_U8 rcv_notify_proxy_cnt;                        // CCO,STA站点收到无扰识别触发报文次数
    HI_BOOL is_send_notify_start;                      // 代理STA站点是否允许进行发送触发报文

    HI_U16 rcv_cco_ntb_cnt;                            // CCO收到邻居网络CCO_NTB个数
    HI_U8 not_rcv_cco_ntb_cnt;                         // CCO没有收到邻居网络CCO_NTB次数
    HI_U8 con_not_rcv_cco_ntb_cnt;                     // CCO连续10次没有收到情况下次数

    HI_U8 phase_identify_pn_err_cnt;                   // 相位识别为零火反接次数
    HI_U8 phase_identify_big_zc_diff_cnt;              // 相位识别为大偏差场景
    HI_U8 phase_identify_err_cnt;                      // 识别相位错误
    HI_U8 phase_identify_fail_cnt;                     // 相位识别错误次数

    HI_U32 last_req_result_time;                       // 上一次请求NTB识别结果时间

    HI_U16 write_mac_cnt;                              // 更新NV中识别结果次数
    HI_U16 pad;
}DIAG_CMD_TF_PASSIVE_STATIC_STRU;

// 增加专门记录无扰识别结果统计量
typedef struct
{
    HI_BOOL is_identify_suc;                           // 是否识别成功
    HI_U8 identify_state;                              // 识别状态，参见
    HI_U8 identify_result;                             // 识别结果,     参见HI_MAC_TFP_IDENTIFY_RESULT_E
    HI_U8 belong_freq;                                 // 归属freq

    HI_U32 belong_snid;                                // 归属SNID

    HI_U8 belong_cco_mac[6];                           // 归属cco_mac
    HI_U8 identify_cco_mac[6];                         // 识别时所在网络cco_mac

    HI_U32 identify_in_snid;                           // 识别时所在网络snid

    HI_U8 identify_in_freq;                            // 识别时所在freq
    HI_U8 pad[3];
}DIAG_CMD_TFP_IDENTIFY_RESULT_STATIC_STRU;

#define DIAG_TFP_STORE_LAST_NSR_NUM                 3  // 最近存储的3组噪声比数据

typedef struct
{
    HI_BOOL is_cur_max_nsr;                            // 本组数据是否为大噪声
    HI_U8 nsr_store_cnt;                               // 噪声比存储次数
    HI_BOOL is_beforehand_valid;                       // 本组是否是否预有效，允许进行结果计算
    HI_BOOL is_cur_ntb_err_filter;                     // 本组是否存在NTB异常

    HI_U32 nsr_store_total_cnt;                        // 总共噪声比存储次数
    HI_U32 max_nsr_cnt;                                // 大噪声比次数
    HI_U32 calc_max_nsr_err_cnt;                       // 计算大噪声错误次数
    HI_U32 not_max_nsr_cnt;                       // 计算不是大噪声次数

    HI_U32 nsr[DIAG_TFP_STORE_LAST_NSR_NUM];           // 最近存储的三组噪声比数据

    HI_U32 ntb_err_filter_cnt;                         // NTB异常滤除次数
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

// 无扰识别收发报文情况统计
#define DIAG_TFP_NEIGHBOR_ENTRY_NUM 12

typedef struct
{
    HI_U32 nid : 24;                                  // 邻居网络snid
    HI_U32 freq : 8;                                  // 邻居网络freq

    // CCO指定本网络中哪些站点与该邻居网络进行通信
    HI_U32 select_cur_net_sta : 12;                   // 指定本网络站点与该邻居网络进行通信
    HI_U32 select_cur_net_backup_sta : 12;            // 指定本网络站点与该邻居网络进行通信的备份站点
    HI_U32 rcv_notify_conut : 8;                      // 收到该邻居网络通知次数

    // 指定与邻居网络通信站点信息
    HI_U32 is_in_use : 1;
    HI_U32 is_select_sta : 1;                         // 是否指定与该网络通信
    HI_U32 select_cnt : 3;                            // 指定与该网络通信次数
    HI_U32 select_sta : 12;                           // 指定与该网络通信站点
    HI_U32 select_backup_sta : 12;                    // 指定与该网络备份通信站点
    HI_U32 is_calc_ntb_diff : 1;
    HI_U32 is_freqerr_valid : 1;
    HI_U32 is_sync_freqoffset_valid : 1;

    // CCO 端向外发送CCO_NTB回应次数
    HI_U32 send_phase_a_ntb_rsp_cnt : 11;             // 发送A相位数据
    HI_U32 send_phase_b_ntb_rsp_cnt : 11;             // 发送B相位数据
    HI_U32 send_phase_c_ntb_rsp_cnt : 10;             // 发送C相位数据

    // CCO,STA收到邻居网络通知CCO_NTB回应报文
    HI_U32 rcv_neighbor_phase_a_ntb_rsp_cnt : 11;     // 收到邻居网络A相位的回应的NTB数据
    HI_U32 rcv_neighbor_phase_b_ntb_rsp_cnt : 11;     // 收到邻居网络B相位的回应的NTB数据
    HI_U32 rcv_neighbor_phase_c_ntb_rsp_cnt : 10;     // 收到邻居网络C相位的回应的NTB数据

    // 收到邻居网络广播启动，停止无扰识别通知
    HI_U32 rcv_neighbor_notify_cmd_cnt : 11;          // 收到邻居网络广播通知次数
    HI_U32 rcv_neighbor_notify_cmd_cnt_by_sta : 11;   // 收到邻居网络通过STA转发的通知，CCO端收到
    HI_U32 send_neighbor_notify_cmd_to_cur_cco_cnt : 10;// STA将收到邻居网络通知单播发送给CCO

    HI_U32 sta_send_neighbor_phase_a_ntb_to_cur_cco_cnt : 11;// STA收到CCO下发CCO_NTB需要该STA进行转发
    HI_U32 sta_send_neighbor_phase_b_ntb_to_cur_cco_cnt : 11;// STA收到CCO下发CCO_NTB需要该STA进行转发
    HI_U32 sta_send_neighbor_phase_c_ntb_to_cur_cco_cnt : 10;// STA收到CCO下发CCO_NTB需要该STA进行转发

    HI_U32 rcv_neighbor_cco_ntb_cnt : 10;             // 接收到该邻居网络CCO_NTB次数
    HI_U32 pad : 22;
}DIAG_CMD_TFP_TRX_STATICS_ENTRY_STRU;

typedef struct
{
    HI_U32 rcv_cur_net_notify_start_cmd_cnt : 10;     // 收到本网络启动次数
    HI_U32 rcv_cur_net_notify_stop_cmd_cnt : 5;       // 收到本网络停止次数
    HI_U32 is_selected_sta : 1;                       // 是否选择该站点为跨网络通信站点
    HI_U32 select_sta_cnt : 8;                        // 该站点被指定为跨网络通信站点次数
    HI_U32 notify_count : 8;                          // 通知次数

    DIAG_CMD_TFP_TRX_STATICS_ENTRY_STRU entry[DIAG_TFP_NEIGHBOR_ENTRY_NUM];  // 邻居网络信息
}DIAG_CMD_TFP_TRX_STATICS_STRU;

// 无扰识别HSO界面交互结构体，命令
// 1、HSO界面下发命令枚举值
typedef enum
{
    HI_MAC_TFP_IDENTIFY_CMD_UNKOWN = 0,
    HI_MAC_TFP_IDENTIFY_CMD_START,        // 启动无扰识别命令
    HI_MAC_TFP_IDENTIFY_CMD_STOP,         // 下发停止无扰识别命令
    HI_MAC_TFP_IDENTIFY_CMD_CLEAR,        // 下发清除无扰识别结果
    HI_MAC_TFP_IDENTIFY_CMD_MAX,
}HI_MAC_TFP_IDENTIFY_CMD_E;

// 2、CCO端回应HSO界面的枚举值
typedef enum
{
    HI_MAC_TFP_IDENTIFY_ERR_NULL = 0,
    HI_MAC_TFP_IDENTIFY_ERR_SUCCESS,         // 无扰识别成功
    HI_MAC_TFP_IDENTIFY_ERR_PARAM,           // 参数错误
    HI_MAC_TFP_IDENTIFY_ERR_FAIL,            // 无扰识别启动失败
    HI_MAC_TFP_IDENTIFY_ERR_NOT_SUPPORT,     // CCO不支持无扰识别
    HI_MAC_TFP_IDENTIFY_ERR_IDENTIFYING,     // 处于无扰识别中
    HI_MAC_TFP_IDENTIFY_ERR_TF_DOING,        // 处于有扰识别中
    HI_MAC_TFP_IDENTIFY_ERR_BUSINESS_BUSY,   // 处于升级中
    HI_MAC_TFP_IDENTIFY_ERR_SEARCH_METER,    // 处于搜表中
    HI_MAC_TFP_IDENTIFY_ERR_MAX,
}HI_MAC_TFP_IDENTIFY_ERR_E;

// 3、HSO界面下发命令结构
typedef struct
{
   HI_MAC_TFP_IDENTIFY_CMD_E cmd;          // 下发无扰识别命令

   HI_U16 identify_durtion;                // 下发启动无扰识别最大时间，单位min
   HI_U16 pad;
   HI_U32 pad1[4];
}DIAG_CMD_TFP_IDENTIFY_SET_REQ_STRU;

// 4、CCO端回应HSO界面命令结构体
typedef struct
{
    HI_MAC_TFP_IDENTIFY_ERR_E ret;        // 回应启动结果
    HI_U32 pad;
}DIAG_CMD_TFP_IDENTIFY_RSP_STRU;

// 5、CCO端回应HSO界面的无扰结果查询结构体
typedef struct
{
    HI_U32 tei : 12;                           // 站点tei
    HI_U32 is_online : 1;                      // 站点是否在线
    HI_U32 result : 4;                         // 站点识别结果,参见HI_MAC_TFP_IDENTIFY_RESULT_E
    HI_U32 pad : 15;

    HI_U8 mac[HI_PLC_MAC_ADDR_LEN];           // 站点 mac
    HI_U8 belong_mac[HI_PLC_MAC_ADDR_LEN];    // 归属台区的cco_mac

    HI_U16 collect_ntb_cnt;                   // 该sta收集的ntb个数
    HI_U16 var_rate;                          // 方差比值
    HI_U16 rate_of_process;                   // 识别进度
    HI_U16 pad1;
}DIAG_CMD_TFP_IDENTIFY_RESULT_ENTRY_STRU;

// 6、STA站点无扰识别结果枚举值
typedef enum
{
    HI_MAC_TFP_IDENTIFY_RESULT_UNKOWN  = 0,
    HI_MAC_TFP_IDENTIFY_RESULT_THIS_POWER_DISTRI,             // 为本台区站点
    HI_MAC_TFP_IDENTIFY_RESULT_NON_THIS_POWER_DISTRI,         // 不为本台区站点
    HI_MAC_TFP_IDENTIFY_RESULT_NOT_SUPPORT,                   // 不支持无扰识别
    HI_MAC_TFP_IDENTIFY_RESULT_PHASE_ERR,                     // 站点相位识别错误
    HI_MAC_TFP_IDENTIFY_RESULT_FAIL,                          // 识别失败
}HI_MAC_TFP_IDENTIFY_RESULT_E;

// 7、CCO端上报整体无扰识别结果结构体
typedef struct
{
    HI_U16 sn;                                        // 本次查询的报文sn号，保证均为本次上报的序号
    HI_U16 pkt_idx;                                   // 本次上报中，每包索引号

    HI_U16 num_total;                                 // 本次上报，总的包数
    HI_U16 entry_num;                                 // 本次上报中，该包中含有的站点个数

    HI_U32 identify_time;                             // 无扰识别时间，单位s
    HI_U32 identify_time_one_turn;                    // 一轮无扰识别运行时间，单位s

    HI_BOOL is_last;                                  // 本包是否为本次最后一包上报
    HI_U8 identify_status;                            // 无扰识别状态识别中，识别结束
    HI_U8 pad[2];

    DIAG_CMD_TFP_IDENTIFY_RESULT_ENTRY_STRU entry[0]; // 上报的站点信息
}DIAG_CMD_TFP_IDENTIFY_RESULT_RSP_STRU;

// 8、无扰识别状态
typedef enum
{
    HI_MAC_TFP_IDENTIFY_STATE_DOING,                  // 无扰识别中
    HI_MAC_TFP_IDENTIFY_STATE_OVER,                   // 无扰识别结束
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
    HI_U32    dt                        : 3;        //定界符类型;
    HI_U32    access                    : 5;        //网络类型
    HI_U32    snid                      : 24;       //短网络标识符;

    HI_U32    result                    : 4;        //表示PB块接收结果：0x0:接收成功;0x1:CRC校验失败;其它:保留
    HI_U32    state                     : 4;        //bit0表示PB0的CRC结果，bit1表示PB1的CRC结果，...
    HI_U32    stei                      : 12;       //源设备标识符
    HI_U32    dtei                      : 12;       //目标设备标识符

    HI_U32    pb_num                    : 4;        //接收到的PB块总数
    HI_U32    rsvd1                     : 4;        //保留
    HI_U32    snr                       : 8;        //信道质量
    HI_U32    load                      : 8;        //站点负载
    HI_U32    rsvd2                     : 8;

    HI_U32    rsvd3                     : 4;       //保留
    HI_U32    ver                       : 4;        //标准版本号
    HI_U32    fccs                      : 24;       //帧校验序列;
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

//实时路由修复模块统计量
typedef struct
{
    HI_U32 create_route_request_cnt;                //创建路由请求报文的次数
    HI_U32 create_route_reply_cnt;                  //创建路由回复报文的次数
    HI_U32 create_route_ack_cnt;                    //创建路由应答报文的次数
    HI_U32 create_route_error_cnt;                  //创建路由错误报文的次数

    HI_U32 create_link_confirm_request_cnt;         //创建链路确认请求的次数
    HI_U32 create_link_confirm_response_cnt;        //创建链路应答的次数
    HI_U32 rcv_link_confirm_request_cnt;
    HI_U32 rcv_link_confirm_response_cnt;

    HI_U32 rcv_route_request_cnt;
    HI_U32 rcv_route_reply_cnt;
    HI_U32 rcv_route_ack_cnt;
    HI_U32 rcv_route_error_cnt;

    HI_U32 repair_src_cnt;                          //作为原始站点创建修复的次数
    HI_U32 repair_dst_cnt;                          //作为目标站点被修复的次数
    HI_U32 repair_start_fail_cnt;                   //路由修复创建失败的次数
}DIAG_CMD_QUERY_RT_REPAIR_STAT_STRU;

// 锁定信息 68B
typedef struct
{
    HI_U8  lock_status;                     // 锁定状态
    HI_U8  lock_status_identify;            // 台区识别功能下锁定状态
    HI_U8  cco_mac[HI_PLC_MAC_ADDR_LEN];    // 锁定网络的CCO MAC地址

    HI_U32 lock_freq : 8;                   // 锁定网络的频段
    HI_U32 lock_snid : 24;                  // 锁定网络的SNID

    HI_U8  is_leave_from_lock_net;          // 上一次是否是从锁定网络离的网
    HI_U8  pad;
    HI_U16 write_nv_cnt;                    // 写NV的次数

    HI_U32 lock_begin_time;                 // 锁定开始时间，单位s
    HI_U32 lock_duration;                   // 锁定时长，单位s
    HI_U32 reject_time;                     // 锁定网络拒绝时间
    HI_U32 leave_time_len;                  // 离线时长(从lock_begin_time计算)，单位s
    HI_U32 last_leave_time;                 // 上一次在锁定网络离网的时间戳，单位s

    HI_U32 lock_mac_cnt;                    // 锁定MAC次数
    HI_U32 lock_snid_cnt;                   // 锁定SNID次数
    HI_U32 snid_to_mac_cnt;                 // 从锁定SNID转为锁定MAC的次数
    HI_U32 reject_cnt;                      // 锁定网络拒绝次数(维测)
    HI_U32 lock_timeout_cnt;                // 锁定超时次数(维测)
    HI_U16 lock_fail_by_not_join_cnt;       // 未加入网络锁定失败(维测)
    HI_U16 send_msg_fail_cnt;               // 消息发送失败(维测)

    HI_U8  belong_status;                           // 归属状态
    HI_U8  belong_clear_reason;                     // 归属清除原因(最近一次)
    HI_U8  belong_cco_mac[HI_PLC_MAC_ADDR_LEN];     // 归属CCO的MAC地址
    HI_U32 belong_freq : 8;                         // 归属网络的频段
    HI_U32 belong_snid : 24;                        // 归属网络的SNID
} DIAG_CMD_QUERY_LOCK_INFO_STRU;

#define NM_LOCK_RECORD_NUM     (8)

typedef struct
{
    HI_U8  lock_reason;                     // 锁定原因
    HI_U8  lock_status;                     // 锁定状态
    HI_U8  cco_addr[HI_PLC_MAC_ADDR_LEN];   // 锁定网络的CCO的MAC或主节点地址
    HI_U32 lock_freq : 8;                   // 锁定网络的频段
    HI_U32 lock_snid : 24;                  // 锁定网络的SNID

    HI_U32 lock_begin_time;                 // 锁定开始时间
    HI_U32 lock_duration;                   // 锁定时长
    HI_U32 lock_duration_safe;

    HI_U8  unlock_reason;                   // 解锁原因
    HI_U8  pad[3];
    HI_U32 unlock_time;                     // 解锁时间
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

    HI_U32 ulRxOkCnt;         //进入抓包函数统计
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
    HI_U8   ucsurplus_pbcnt; // 剩余的pb块个数
    HI_U8   ucPbCnt;

}DIAG_CMD_MAC_PHYSICAL_TEST_STRU;
#endif


#if defined(PRODUCT_CFG_PRODUCT_TYPE_NDM) || defined(PRODUCT_CFG_HSO)
#define NDM_FC_MAX_SIZE                                (16)
#define NDM_PB_MAX_SIZE                                (520)
typedef struct
{
    HI_U16 sn;                 //包序号
    HI_U16 pktIdx;            //本包index
    HI_U16 totalPktCnt;          //总包数
    HI_U16 pbLen;
    HI_U16 totalpbLen;
    HI_U16 pad;
    HI_U32 timeStamp;
    HI_MDM_CAPTURE_PKT_PBB_TYPE_EN ePayloadType;//payload 类型
    HI_U8  aucFc[NDM_FC_MAX_SIZE];
    HI_U8  aucPb[NDM_PB_MAX_SIZE];
}DIAG_CMD_MAC_NDM_CAPTURE_STRU;

#define NDM_MODE_CAPTURE_DATA    1
#define NDM_MODE_NORMAL    0
typedef struct
{
	HI_U32  capture_switch;       // 按比特位表示上报mpdu报文种类开关
	HI_U32  mode_switch;             // 切换模式 1，抓包模式；0正常模式
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

//CCO的停电事件查询信息
typedef struct
{
    HI_U8 power_failure_dfx_bitmap[POWER_FAILURE_BITMAP_LEN];                   // 停电站点bitmap

    HI_U16 pad;
    HI_U16 rcv_power_failure_tei_cnt;                                         // 收到停电上报sta的数目

    HI_U32 rcv_sys_time;                                                        //收到停电上报帧的时间

}DIAG_CMD_QUERY_POWER_FAILURE_INFO_STRU;
#endif

//功率控制统计量
typedef struct
{
    HI_BOOL    is_power_ctrl_enable;                            // 功率控制使能开关
    HI_BOOL    is_recover_last_digital_power;
    HI_BOOL    is_whole_net_set_digital_power;                  // 是否已经全网设置字发射功率
    HI_BOOL    is_set_v100_detect_digital_power;

    HI_S8      v100_detect_digital_power;
    HI_BOOL    is_power_ctrl_starting;
    HI_U8      pad[2];

    HI_S8      digital_power[4];                                // 全网通知的数字发射功率
    HI_S8      last_digital_power[4];                           // 全网通知数字发射功率之前的数字发射功率

    HI_BOOL    is_whole_net_set_digital_power_timer_start;      // whole_net_set_digital_power_timer定时器是否启动(定时器N)
    HI_U8      power_ctrl_stop_reason;
    HI_U16     sta_num;                                         // 功率控制缓存的sta个数

    HI_U16     total_send_cnt;                                  // 发送全网设置发射功率的V100探测信标的总个数计数
    HI_BOOL    is_power_ctrl_form_timeout_timer_start;          // (定时器X)
    HI_BOOL    is_stop_record_power_ctrl_sta;

    HI_U32     whole_net_set_digital_power_timer_start_timestamp;   //单位s
    HI_U32     power_ctrl_form_timeout_timer_start_timestamp;        //单位s

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
    HI_U32 ntb_tx_enqueue;          // 发送入队列ntb时刻
    HI_U32 ntb_tx_real;             // 实际发送ntb时刻
    HI_U32 ntb_rx;                  // 接收ntb时刻
} DIAG_FAST_NOTICE_RX_TX_NTB_ST;

typedef struct
{
    DIAG_FAST_NOTICE_RX_TX_NTB_ST  inf[DIAG_FAST_NOTICE_TX_MAX_LOOP];
} DIAG_FAST_NOTICE_RX_TX_NODE;

typedef struct
{
    HI_U32 time_ntb;                // ntb时刻
    HI_U16 total;                   // 站点总数
    HI_U16 current;                 // 当前个数
    HI_U16 start_tei;               // 起始teis
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

// 设置PLC协议版本的下行结构
typedef struct
{
    HI_U8 protocol_type;
    HI_U8 pad[3];
}DIAG_CMD_SET_PLC_PROTO_STRU;

typedef struct
{
    HI_U32 ret;
}DIAG_CMD_SET_IND_PLC_PROTO_STRU;

//查询PLC协议版本的上行结构
typedef struct
{
    HI_U8 protocol_type;
    HI_U8 pad[3];
}DIAG_CMD_QUERY_IND_PLC_PROTO_STRU;

#define NODE_FIND_MAC_LIST_NUM      (8)
#define NODE_FIND_MAC_LIST_SIZE     (48)

//节点查找REQ结构
typedef struct
{
    HI_U8 mode;             // 模式
    HI_U8 duration;         // 持续时长，单位min
    HI_U8 mac_num;          // 列表模式下，查询站点个数
    HI_U8 lock_duration;    // 锁定时长，单位min
    HI_U8 mac_list[NODE_FIND_MAC_LIST_SIZE];
} DIAG_CMD_NODE_FIND_REQ_STRU;

//节点查找IND结构
typedef struct
{
    HI_U32 ret;
} DIAG_CMD_NODE_FIND_IND_STRU;

// 能源应用维测信息
typedef struct
{
    HI_BOOL is_energy_mode;
    HI_BOOL is_opening;
    HI_BOOL is_cco_no_send_bcn;
    HI_BOOL is_chip_detecting;

    HI_BOOL is_full;
    HI_BOOL is_white_list_empty;
    HI_BOOL is_unlock_rejoin_enable;           // 是否启用满载非锁定离线功能
    HI_U8 sta_rejoin_by_unlock_cnt;          // STA站点由于满载但是没有被锁定，软复位次数

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

    HI_U32 reject_by_no_register_cnt;        // CCO端拒绝站点入网，由于能源版本下CCO鉴权没有生效
}DIAG_CMD_QUERY_ENERGY_STATICS_STRU;

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
// 无扰NTB-分布式: 查询当前识别信息 请求参数
typedef struct
{
    HI_U16  index;
    HI_BOOL is_use_idx; // 是否使用下标来查询，不是则使用下面的freq nid来查询
    HI_U8   freq;
    HI_U32  nid;
} DIAG_CMD_TF_GET_NET_IDENTIFY_INFO_REQ_STRU;

typedef struct
{
    HI_U32 cal_diff_fail    : 1;
    HI_U32 is_err_ntb       : 1;
    HI_U32 pad1             : 2;
    HI_U32 valid_ntb_cnt    : 8;    // 有效NTB个数
    HI_U32 cco_disturb      : 10;   // CCO的扰动，单位us
    HI_U32 sta_disturb      : 10;   // STA的扰动，单位us

    HI_U16 zc_diff;                 // 过零点偏差
    HI_U16 pad2;

    HI_U32 variance;                // 方差
} diag_tf_nd_cache_info_st;

#define DIAG_TF_ND_SAVE_CACHE_NUM   5
#define DIAG_TF_ND_SEQ_NUM          5

// 查询当前周期某网络识别信息 84
typedef struct
{
    HI_U8  is_valid                 : 1;        // 记录是否有效
    HI_U8  match_phase              : 3;        // 本站点在该网络的相线
    HI_U8  match_edge               : 2;        // 匹配的采集方式: 参考POWER_EDGE_XXX
    HI_U8  pad1                     : 2;
    HI_U8  net_index;                           // 网络索引，具体网络信息从多网络信息中获取
    HI_U8  collect_seqs[DIAG_TF_ND_SEQ_NUM];    // 采集序号
    HI_U8  phase_masks[DIAG_TF_ND_SEQ_NUM];     // 同一采集序列号下，相位数据接收掩码

    HI_U16 rcv_pkt_cnt;                         // 处理NTB数据包数
    HI_U16 seq_eq_pkt_cnt;                      // 报文序号相同的包数

    HI_U8  match_succ_cnt;                      // 匹配本地NTB成功包数
    HI_U8  big_period_diff_cnt;
    HI_U8  match_phase_fail_cnt;
    HI_U8  zc_diff_cnt;                         // 统计到过零点差值数据包数

    HI_U8  cal_diff_fail_cnt;
    HI_U8  err_ntb_pkt_cnt;
    HI_U8  cal_var_fail_cnt;
    HI_U8  valid_pkt_cnt;                       // 包含足够有效NTB的包数 (达到计算方差的)

    HI_U8  variance_cnt;
    HI_U8  cache_idx;
    HI_U16 pad;

    HI_U32 last_rcv_time;

    diag_tf_nd_cache_info_st caches[DIAG_TF_ND_SAVE_CACHE_NUM];

    HI_U16 period_diff_avg;                     // 工频周期偏差均值，单位NTB
    HI_U16 zc_diff_avg;                         // 过零点偏差均值

    HI_U32 variance_avg;                        // 方差均值
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
    HI_U16 is_init_success      : 1;    // 是否初始化成功
    HI_U16 is_caching           : 1;    // 是否正在缓存数据
    HI_U16 is_cache_full        : 1;    // NTB缓存: 是否将BUF缓存满
    HI_U16 is_identifying       : 1;    // 周期识别: 是否正在台区识别中
    HI_U16 is_start_cache_fail  : 1;    // 是否启动采集失败，将会在下一次result_judge_timer响时尝试重新启动
    HI_U16 result_net_index     : 6;    // 最终识别结果网络IDX
    HI_U16 result_type          : 5;    // 生成最终结果的类型
    HI_U16 next_cache_index;                       // NTB缓存: 下一个要存储的位置

    HI_U32 last_rcv_time;               // 如果连续两次收到数据超过30min，重新开始识别
    HI_U32 cur_round_begin_time;

    diag_tf_nd_net_result_st results[DIAG_TF_ND_RESULT_QUERY_NUM];
} DIAG_CMD_TF_ND_CUR_RESULT_IND_STRU;

#define TF_ND_IDENTIFY_STAT_NUM     8
#define TF_ND_JUDGE_RECOTD_NUM      25

enum
{
    TF_ND_BREAK_POINT_NET_LACK_VAR = 1, // 有网络未算出方差
    TF_ND_BREAK_POINT_NO_RCV_JOIN,      // 没有收到加入台区NTB
    TF_ND_BREAK_POINT_VARIANCE_CMP,     // 方差比较
    TF_ND_BREAK_POINT_ERR_SCENE_FAIL,   // 异常场景，不够包数门限
    TF_ND_BREAK_POINT_ERR_SCENE_SUCC,   // 异常场景，包数达到门限
};

typedef struct
{
    HI_U32 break_point  : 3;    // 流程结束的位置
    HI_U32 is_override  : 1;    // 是否达到推翻门限
    HI_U32 net_idx      : 5;    // 最大比值对应的网络
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

// NTB识别状态
typedef struct
{
    diag_net_info_st belong_net_info;
    diag_tf_nd_sta_stat_st stat;
} DIAG_CMD_TF_ND_STA_GET_STAT_IND_STRU;

// 清除NTB识别结果参数
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
    HI_U32 is_filter    : 1;    // 是否滤波
    HI_U16 tei;
    HI_U16 collect_duration;    // 采集时长
    HI_U16 hold_duration_min;   // 内存保持时间，过了这时间就把内存释放了，单位min
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

