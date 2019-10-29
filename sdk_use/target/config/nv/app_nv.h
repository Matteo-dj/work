//*****************************************************************************
//
//                  版权所有 (C), 1998-2011, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : app_nv.h
//  版 本 号   : V1.0
//  作    者   : shenhankun/00130424
//  生成日期   : 2011-10-26
//  功能描述   : TODO: ...
//
//  函数列表   : TODO: ...
//  修改历史   :
//  1.日    期 : 2011-10-26
//    作    者 : shenhankun/00130424
//    修改内容 : 创建文件
//
//*****************************************************************************

//*****************************************************************************
// PROJECT   :
// SUBSYSTEM :
// MODULE    :
// OWNER     :
//*****************************************************************************


#ifndef __APP_NV_H__
#define __APP_NV_H__


#define  ID_NV_APP_MR_CFG               0x0401  //
#define  ID_NV_MRS_CCO_ADDR             0x0402  // 载波主节点地址
#define  ID_NV_APP_SEARCHMETER_CFG      0x0403  // 搜表相关配置
#define  ID_NV_APP_BAUDRATE_CFG         0x0404  // 波特率相关配置
#define  ID_NV_APP_CTLII_CFG            0x0405  // II型采集器相关配置
#define  ID_NV_APP_EVENT_CFG            0x0406  // 事件相关配置
#define  ID_NV_APP_STATE_CHG_REPORT_CFG 0x0408  // 上报从节点在线状态变化参数配置
#define  ID_NV_APP_SM_RESULT1           0x040A  // 搜表结果前16个
#define  ID_NV_APP_SM_RESULT2           0x040B  // 搜表结果后16个

#define  ID_NV_APP_NVM_DATA_0           0x040C  // 数据存储0
#define  ID_NV_APP_NVM_DATA_1           0x040D  // 数据存储1
#define  ID_NV_APP_NVM_DATA_2           0x040E  // 数据存储2
#define  ID_NV_APP_NVM_DATA_3           0x040F  // 数据存储3

#define  ID_NV_APP_CLT_EVT_INFO1        (ID_NV_APP_NVM_DATA_0)  // 搜表结果中第1--8个表地址的事件信息
#define  ID_NV_APP_CLT_EVT_INFO2        (ID_NV_APP_NVM_DATA_1)  // 搜表结果中第9--16个表地址的事件信息
#define  ID_NV_APP_CLT_EVT_INFO3        (ID_NV_APP_NVM_DATA_2)  // 搜表结果中第17--24个表地址的事件信息
#define  ID_NV_APP_CLT_EVT_INFO4        (ID_NV_APP_NVM_DATA_3)  // 搜表结果中第25--32个表地址的事件信息
#define  ID_NV_MRS_STA_RESET_TIME_INF_0 (ID_NV_APP_NVM_DATA_0)  // STA模块上电时间信息0
#define  ID_NV_MRS_STA_RESET_TIME_INF_1 (ID_NV_APP_NVM_DATA_1)  // STA模块上电时间信息1

#define  ID_NV_APP_PLC_OVERTIME_INFO    0x0410  // 读表通讯超时参数
#define  ID_MV_APP_SIMU_METER_INFO      0x0412  //
#define  ID_MV_APP_MAX_PLC_TIMEOUT_NUM  0x0413  // 节点数据项PLC最大超时次数
#define  ID_NV_APP_MAC_FROM_METER       0x0414  // 使用表地址代替Mac地址功能使能开关
#define  ID_NV_APP_MAX_TURN_NUM         0x0415  // 最大轮次重试次数
#define  ID_NV_APP_BROADCAST_TX         0x0416  // 使用广播地址响应广播地址命令使能开关
#define  ID_NV_MRS_RM_CFG               0x0417  // 读表管理配置相关参数
#define  ID_NV_MRS_DUTY_CYCLE_CFG_INFO  0x0418  // 占空比阈值
#define  ID_NV_APP_MR_DUT               0x0421  // 工装执行广播标志
#define  ID_NV_APP_MAC_MAP_INFO         0x0422  // CCO端映射表维护信息
#define  ID_NV_APP_REPORT_METER_INFO    0x0423  // STA端主动上报表地址和MAC地址信息
#define  ID_NV_MRS_UPG_PARAM_INFO       0x0426  //远程升级中MRS模块参数信息
#define  ID_NV_APP_CLTI_CFG             0x0427  //I采配置
#define  ID_NV_MRS_STA_RM_CFG           0x0430  // STA抄表参数配置
#define  ID_NV_MRS_CCO_PARAM_ID         0x0431  // 预留
#define  ID_NV_APP_CCO_SM_CFG_EXP       0x0432  // CCO搜表相关配置扩展
#define  ID_NV_APP_STA_SM_CFG_EXP       0x0433  // STA搜表相关配置扩展
#define  ID_NV_MRS_DC_SD_INFO           0X0434  // SD卡采数配置信息
#define  MRS_RETRY_CFG_MAX_NUM          3       // 重传参数数量
#define  MRS_RM_POOL_MAX_NUM            2       // 读表缓存池数量
#define  MRS_BROADCAST_RM_97_DI_NUM     2       // 全网广播中抄读97表的DI个数

#define NDM_DC_FOLDER_NAME_SIZE 21
// SD卡采数参数配置
typedef struct
{
    HI_U32  ulRamSize;          // 盲采开始地址
    HI_S32  slAgcGain;          // 增益对盲采有效
    HI_U16  usTrigerType;       // 采数类型
    HI_U16  usPeriodTime;       // 盲采周期，单位分钟
    HI_U16  usTrigerTime;       // 多长时间收不到PB快进行触发采数，单位分钟
    HI_U16  usNameId;           // 每次按键启动采数 +1

    HI_U8   ucTimesPerPeriod;   // 盲采每个周期连续进行多少次采集。
    HI_U8   ucCPowerOnEable;    // 重启续采功能使能开关
    HI_U8   ucCollecting;       // 是否正在进行采数
    HI_U8   ucIsFollowMode;     // 是否跟随

    HI_CHAR scFolderName[NDM_DC_FOLDER_NAME_SIZE];
    HI_U8   reserved1[3];

    HI_U8   scCcoMac[HI_PLC_MAC_ADDR_LEN];  // 跟随的CCO的MAC地址
    HI_U8   ucFreq;                         // 跟随网络的freq
    HI_U8   ucIsFixFreq;                    // 是否固定频段

    HI_U8   ucSnid;                         // 跟随SNID
    HI_U8   reserved2[11];
}NV_APP_MRS_NDM_DC_SD_PARAM_STRU;

typedef struct
{
    HI_U8   ucBcmInterval;      // 单位:秒
    HI_U8   ucBcmRetryMax;      // 重试次数

    HI_U8   ucProtoVer;         // 规范版本 0-09版 1-13版 其它-保留

    // 轮抄抄表参数：超时时间与重试次数
    HI_U8   ucAfn14F1UpRetryMax;//14F1上行报文超时重试次数(Nv项)
    HI_U16  usAfn14F1UpTimeOut; //14F1上行报文超时时间(Nv项，单位ms)
    HI_U16  usAfn06F2UpTimeOut; //06F2上行报文超时时间(Nv项，单位ms)
    HI_U8   ucAfn06F2UpRetryMax;//06F2上行报文超时重试次数(Nv项)

    HI_U8   ucTestModeSnId_ucCsgParallelEnable;

    // 仅供STA使用 begin
    HI_U8   ucFrameBufItemCount;// 缓存帧的最大数量
    HI_U8   ucFrameBufTimeout;  // 缓存帧超时时间 单位:秒
    // 仅供STA使用 end

    HI_U16  usTestFrameInterval_usCrcInitValue;// [辅助STA] 发送测试报文时间间隔，单位毫秒
    HI_U8   ucWhitelistDelay;   // CCO搜表结束打开白名单定时器时长，单位:分钟，默认:0

    HI_U8   ucSendTimeCount;    // 搜表过程中发送询问集中器时钟总次数的计数

    HI_U8   plc_rxtx_mode;      // PLC通道收发模式 00:等待组网路由优化完成后才进行PLC通道的收发; 01:强制PLC通道收发(不等待PLC状态);
    HI_U8   get_addr_try_num;   // 获取电表地址尝试次数
    HI_U16  usLowRatio;         // 抄通率低电表抄通门限值百分比，9876表示98.76%
    HI_U32  get_addr_Fail_wait_time;    // 获取电表地址失败后等待时间 单位:S
    HI_U32  ulSimuChlTimeout;   // 模拟通道空闲超时时间 单位:毫秒
    HI_U8   ucCcoFreqEnable;    // CCO如果获取到时钟，使能开启频段定时的开关，0:不启用,1:启用,默认值为1

    HI_U8   ucBindNetwork;      // 是否绑定CCO Network，1表示绑定，0表示不绑定，其他值未用。
    HI_U16  usBindDuration;     // 绑定CCO Neteork的持续时间，单位：分钟，最大45.5天；取值0xFFFF时，表示永久锁定，实际锁定136年（U32表示的最大秒数）

    HI_U8   ucTestModeEnable;   // [CCO] 测试模式特性使能开关
    HI_U8   ucTestFrameTimeout; // [CCO] 测试模式命令报文接收超时，单位：分钟，默认值：1分钟

    HI_U8   ucFlashSaveDelay;   // 表档案变更存储flash时间 模式 20s
    HI_U8   ucSoftTfMode;       // 无扰台区识别功能模式: 0关闭，1打开(白名单常关，站点主动切换网络)，2打开(白名单常关，站点不主动切换网络)
}NV_APP_MR_CFG_STRU;

typedef struct
{
    HI_S16 PlcLayerDelayNv;     // 层级超时时间参数           单位:ms
    HI_S16 MeterOverTimeNv;     // 电表超时时间参数           单位:ms
    HI_S16 PlcRevisoryNv;       // CCO侧PLC超时修正值;        取值为0时表示不修正 单位:ms
    HI_S16 CtrlCmdDelayNv;      // 电表端补充超时时间参数     单位:ms
    HI_S16 CltIDelayNv;         // I型采集器补充超时时间参数  单位:ms
    HI_S16 MeterFrameInterval;  // STA端645帧间隔时间
    HI_S8 PlcMaxRetryNvS;       // 最大重试次数参数(单播)
    HI_S8 PlcMaxRetryNvB;       // 最大重试次数参数(广播)
    HI_U8 MeterMaxRetryNv;      // 电表端超时重试次数
    HI_U8 MeterRetryCfgNv;      // 电表端重传配置 Bit0:无应答重传; Bit1:否认帧重传;
    HI_S8 PlcMaxRetryLoop;      // 最大重试轮数
    HI_U8 MaxServiceTime;       // 最大超时时间 单位:s
    HI_U8 reserved[6];
}NV_MRS_RETRY_CFG_S;

typedef struct
{
    NV_MRS_RETRY_CFG_S stPlcOverTimeParam[MRS_RETRY_CFG_MAX_NUM];
}NV_APP_PLC_OVERTIME_INF_STRU;  // ID_NV_APP_PLC_OVERTIME_INFO

#define NV_SIMU_METER_DISABLE   0
#define NV_SIMU_METER_ENABLE    1
#define NV_SIMU_METER_AUTO      2

typedef struct
{
    HI_U8 ucMeterFlag;
    HI_U8 ucMeterNum;
    HI_U8 ucEvtOutFlg;
    HI_U8 ucDelay;
}MV_APP_SIMU_METER_INFO;

typedef struct
{
    HI_U32 ulMaxNum;     // 节点数据项最大PLC超时次数
}MV_APP_MAX_PLC_TIMEOUT_NUM;    // ID_MV_APP_MAX_PLC_TIMEOUT_NUM

typedef struct
{
    HI_U8 ucEnable;  // 使用表地址代替Mac地址功能开关 0:不启用 1:启用
    HI_U8 reserved[3];
}NV_APP_MAC_FROM_METER_STRU;

typedef struct
{
    HI_U32 ulTurnNum;   // 定抄读表最大重试轮次
}NV_APP_MAX_TURN_NUM;   // ID_NV_APP_MAX_TURN_NUM

typedef struct
{
    HI_U8 ucEnable;  // 使用广播地址响应广播地址命令使能开关 0:不启用 1:启用
    HI_U8 reserved[3];
}NV_APP_BROADCAST_TX_STRU;

typedef struct
{
    HI_U8 ucDutFlag; //工装测试中点抄使用广播抄表标志 0:不启用  1:启用 ，默认为0
    HI_U8 ucRePowerTime; // CCO恢复发射功率所需时间，默认15分钟
    HI_S16 sNtbAjustBase;   // STA和II采工装NTB校准基数
}NV_APP_MR_DUT_STRU;

typedef struct
{
    HI_U16 usSendMacPlcTime;     //CCO端发送读表地址PLC帧时间间隔
    HI_U16 usSendLastMacPlcTime; //CCO端发送最后一个Mac读表地址PLC帧后的时间间隔
    HI_U8 ucMapEnable;       //CCO端映射表维护使能开关 0:不启用  1:启用 ， 默认为1
    HI_S8 max_parallel_num;
    HI_S8 max_retry_num;
    HI_U8 ucCltMapEnable; //CCO端采集器映射表维护开关  0:不启用 1:启用 目前在江苏版本启用
    HI_S16 max_ttl;   //默认为15
    HI_U16 check_ttl_time; //默认为120s，单位为s
    HI_U16 map_turn_time;
    HI_U8 max_turn_num;
    HI_U8 ucLevelFlag_ucMaxMrsTtl;   //抄表时中继级数开关
}NV_APP_MAC_MAP_INFO_STRU;

// 并发控制NV项配置
typedef struct
{
    HI_U16 usMaxParallelNum;       // 最大并发数
    HI_U16 usMaxFrame;             // 最大多帧数
    HI_U16 usMaxTotalItem;         // 最大业务读表请求总数量
    HI_U8 ucCheckRepeatMode;       // 检查重复读表请求模式(0:使用CRC检查; 1:使用表地址检查)
/* BEGIN: Modified by liuxipeng/lKF54842,2014/4/29 问题单号:DTS2014042306517 */
    HI_U8 ucMeterSwitchInterval;   // 电表切换间隔,用于控制同一采集器下不同电表切换时的间隔时间,单位10ms
/* END  : Modified by liuxipeng/lKF54842,2014/4/29 问题单号:DTS2014042306517 */
}NV_MRS_PARALLEL_CFG_S;

// 读表管理模块NV项配置
typedef struct
{
    NV_MRS_RETRY_CFG_S stRetryCfg[MRS_RETRY_CFG_MAX_NUM];
    NV_MRS_PARALLEL_CFG_S stParallelCfg[MRS_RM_POOL_MAX_NUM];
}NV_MRS_RM_CFG_STRU;    // ID_NV_MRS_RM_CFG


typedef struct
{
    HI_U16 usFrameHead;         // 首字节超时时间，默认: STA/II采-800ms, I采-3600ms
    HI_U16 usFrameInterval;     // 帧间隔时间，默认: 200ms
    HI_U16 usCtrlCmdDelay;      // 费控命令补偿，默认: 2000ms
    HI_U16 usMeterInterval;     // 645帧间隔
    HI_U8  ucQueueMax;          // 队列深度，默认: 16
    HI_U8  ucRetryCfg;          // bit0-bit1:STA轮抄和透抄重传次数  bit2-bit3:电表应答否认重传次数
    HI_U8  pad[6];
} NV_MRS_STA_RM_CFG_INF;

#define MRS_STA_RM_CFG_MAX      (2)

typedef struct
{
    NV_MRS_STA_RM_CFG_INF astRmCfg[MRS_STA_RM_CFG_MAX];  // 0: STA/II采; 1: I采
} NV_MRS_STA_RM_CFG_STRU;


typedef struct
{
    HI_U8 ucStaRepFlag;         // STA端主动上报表地址来维护映射表开关 0:不启用  1:启用 ， 默认为0
    HI_U8 ucMeterChangeFlag;    // STA端表地址改变后,上报CCO开关       0:不启用  1:启用    默认为??????
    HI_U8 ucDutAdcFlag;         // 工装ADC补偿测试开关，默认打开
    HI_U8 reserved;
}NV_APP_REPORT_METER_INFO_STRU;  //STA端主动上报表地址和MAC地址信息

typedef struct
{
    HI_U32  seq;                // 搜表序列号:每次搜表具有唯一的一个序号,重新搜表序号加1
    HI_U32  search_di_07;       // 搜表报文中的DI值(645-2007规约电能表), 默认值0x04000401
    HI_U16  search_di_97;       // 搜表报文中的DI值(645-1997规约电能表)条读, 默认值0xC032
    HI_U16  frame_timeout;      // 电表应答帧首字节超时时间(ms)
    HI_U8   byte_timeout;       // 电表应答帧字节间隔超时时间(ms)
    HI_U8   meter_num_max;      // 电表的最大数量，规格是32
    HI_U8   search_bc_enable;   // 广播搜索97表开关 0:关闭 其它:打开
    HI_U8   search_retry;       // 搜表时报文重试次数
    HI_U8   bc_loop_max;        // 启动/停止搜表命令广播轮次
    HI_U8   bc_send_interval;   // 启动/停止搜表命令包发送间隔(100ms)
    HI_U8   topo_query_interval;// 搜表时TOPO查询时间间隔(min)
    HI_U8   topo_stable_times;  // TOPO连续未变更次数
    HI_U8   query_max_way;      // 并发单播查询的站点最大数量
    HI_U8   query_interval;     // 并发单播查询时间间隔(s)
    HI_U8   query_force_times;  // 强制要求应答次数门限
    HI_U8   query_except_times; // 查询异常次数门限
    HI_U8   finish_reason;      // 搜表结束原因

    HI_U8   detect_loop_max;            // STA上电流程: 探测波特率轮次
    HI_U16  detect_timeout;             // STA上电流程: 每次探测波特率超时时间 单位:毫秒
    HI_U16  detect_period;              // STA上电流程: 一个探测循环结束后再次开始循环的等待时间 单位:秒
    HI_U8   query_sm_interval;          // I采搜表: 过程中查询搜表结果时间间隔 单位:秒
    HI_U8   read_positive_A_times;      // I采上电流程: 抄读NV中电表正向有功电能次数
    HI_U8   read_positive_A_interval;   // I采上电流程: 查询I型采集器第1块搜表结果时间间隔 单位:秒
    HI_U8   explore_byte_count_max;     // II采搜到表并且验证完后多缩字节数上限，默认值为0
    HI_U16  search_di_97c;              // 搜表报文中的DI值(645-1997规约电能表)块读, 默认值0x901f
    HI_U16  search_di_97b;              // 搜表报文中的DI值(645-1997规约电能表)块读, 默认值0x9010
    HI_U8   invalid_frm_len_07;         // 07规约搜表乱码帧长度最小值，默认值为1
    HI_U8   invalid_frm_len_97;         // 97规约搜表乱码帧长度最小值，默认值为2
    HI_U8   report_AA_2_master;         // 搜表结果上报时表地址高字节AAH上报标识: 0-不上报，1-上报，默认值0

    HI_U8   ucSearchEnableFlag_ucCltIPlcThreshold;        	// I采/II采搜表使能，bit0：I采搜表使能，bit1表示II采搜表使能: 1表示功能开启，0表示功能关闭
    HI_U16  search_period;              // 周期搜表定时时长

    HI_U16  usAfn06F4UpTimeOut;         // 搜表06F4上报超时时间
    HI_U8   ucAfn06F4UpRetryMax;        // 搜表06F4上报最大重试次数
    HI_U8   ucDetectTimeOut_ucCltIQueryThreshold;            // II采搜表中探测搜表结果超时时间,单位:百毫秒
} NV_APP_SEARCHMETER_CFG_STRU;

#define MRS_TF_REPORT_MODE_NUM      (4)

#define APP_AREA_CODE_DEFAULT       (0x00)      /**< 地区码: 默认值 */
#define APP_AREA_CODE_BEIJING       (0x01)      /**< 地区码: 北京   */

typedef struct
{
    HI_U16  usSMDurationDefault;        // 搜表默认时长，单位min
    HI_U8   ucRecoverWhiteSwitch;   // 搜表结束时，是否恢复搜表前白名单状态
    HI_U8   ucBindCmdSendInterVal;  // 锁定命令发送间隔，每发送完一轮间隔这么长时间，单位s

    HI_U8   ucCheckMainNodeAddr;    // 检查主节点地址改变时，重新进行台区识别
    HI_U8   ucAllowSMInTf;          // 台区识别中是否允许搜表
    HI_U16  usCheckMainNodeTimeout; // 检查主节点地址超时时间

    HI_U8   ucSmReportWaitLockMsg;  // 搜表上报是否等待锁定报文
    HI_U8   ucCloseWhiteWhenSM;     // 搜表过程是否关闭白名单
    HI_U8   ucCcoRstPinEnable;      // CCO端RST管脚使能，1使能，0不使能
    HI_BOOL pad0;

    HI_U8   ucBindCmdSendMaxWay;    // 锁定报文一次最大发几帧，目前南网使用
    HI_U8   ucBindCmdSendMaxTimes;  // 锁定报文最大发送次数，目前南网使用
    HI_U16  pad1;

    HI_U8   ucReportModeInit;       // 初始化上报模式，每个模式对应的上报类型由aucReportModeMask配置
    HI_BOOL bUseNotifyMode;         // 是否使用集中器通知的上报模式
    HI_BOOL bCloseWhiteTfBegin;     // 启动识别时是否关闭白名单
    HI_BOOL bRecoverWhiteTfEnd;     // 停止识别时是否恢复白名单

    HI_BOOL bCareReportSwitch;      // 使用06-F5上报时，是否受事件上报开关影响
    HI_BOOL bReportUnknownMainNode; // 对于未知主节点地址的站点，是否上报
    HI_U8   aucMainNodeAddrUnknown[HI_PLC_MAC_ADDR_LEN];    // 未知主节点地址的站点上报时，填写此主节点地址

    HI_U8   aucReportModeMask[MRS_TF_REPORT_MODE_NUM];      // 各个模式上报哪些类型

    HI_U8   area_code;              /**< 地区码: 0-默认，1-北京版本，其他-保留 */
    HI_U8   ucQueryInterval;        // 查询间隔，单位s
    HI_U8   ucQueryMaxTry;          // 查询最大尝试次数
    HI_U8   ucQueryMaxWay;          // 一次发几帧

    HI_U16  usQueryBeginDelayTIme;  // 查询开始延迟，单位s
    HI_U16  usQueryRoundInterval;   // 轮次间隔，单位s

    HI_U8   ucFlagInArcInBelong;    // 类型定义: 在档案、属于本台区
    HI_U8   ucFlagInArcNoBelong;    // 类型定义: 在档案、不属于本台区
    HI_U8   ucFlagNoArcInBelong;    // 类型定义: 不在档案、属于本台区
    HI_U8   ucFlagNoArcNoBelong;    // 类型定义: 不在档案、不属于本台区

    HI_U8   aucSetOffModeMask[MRS_TF_REPORT_MODE_NUM];
	HI_U16   usNvRfTimeSlot;				// 配置双模时隙时间，分钟
	HI_U16   usNvRfSGDTimeSlot;				// 配置国网双模时隙时间，分钟;

    HI_U32  pad3[3];
} NV_APP_CCO_SM_CFG_EXP_STRU;

typedef struct
{
    HI_U16 usLeaveTimeLenTh;    // 离网时长门限，单位min
    HI_U8  ucStaRstPinEnable;   // STA端RST管脚使能，1使能，0不使能
    HI_U8  pad;
    HI_U32 ulRstShielTime;      // 屏蔽RST管脚时长，单位s

    HI_U32 pad1[2];
} NV_APP_STA_SM_CFG_EXP_STRU;

// 波特率配置结构体
// 结构体成员不得随意调整位置(若需调整必须与mrs_srv_baudrate_manage.h中的相关结构同步修改)
// 每组配置中D0~D15每3bit表示一个通讯速率
// 0表示无效
// 1表示600bps
// 2表示1200bps
// 3表示2400bps
// 4表示4800bps
// 5表示9600bps
// 6表示19200bps
// 7表示38400bps
// 每组配置最多支持5中通讯速率
// 例如07读表需配置波特率为2400,4800,1200,9600  则对应usRmOf07取值为0 000 101 010 100 011 = 0x0AA3
typedef struct
{
    HI_U16 usSmOf97;        // 97表搜表配置
    HI_U16 usSmOf07;        // 07表搜表配置
    HI_U16 usSmOf698;       // 698表搜表配置
    HI_U16 usRmOf97;        // 97表抄表配置
    HI_U16 usRmOf07;        // 07表抄表配置
    HI_U16 usRmOf698;       // 698表搜表配置
    HI_U16 usReserved3;     // 保留字段3
    HI_U16 usAdjustTime;    // 广播校时配置
    HI_U16 usDetect;        // 上电探测波特率配置
    HI_U8  padding[2];
}NV_APP_BAUDRATE_CFG_STRU;


typedef struct
{
    HI_U16 usTimingSendInterval;    // 广播校时发送间隔
    HI_U16 usEvtMonitorInterval;    // 本地事件监控抄读间隔
    HI_U8  ucEvtMonitorFlag;        // 本地事件监控数据项标识
    HI_U8  ucIrTrasmit645;          // 透抄645标识
    HI_U8  ucPowerSearchEnable;     // Ii采本地搜表使能开关
    HI_U8  ucRunLedMode;            // II采运行灯状态模式: 0表示正常模式(正常状态485灯闪100ms)；1表示国网送检模式(485灯闪20ms),默认为1
/* BEGIN: ADD/Modified by cuichunyan/c00276962 2016/11/10  问题单号:cltii R485 MultiConnect */
    HI_U8  ucR485Monitor;           //II采485口监控是否开启，0：关闭 1：开启 。默认打开
/* END  : ADD/Modified by cuichunyan/c00276962 2016/11/10  问题单号:cltii R485 MultiConnect */
    HI_U8  area_version;            // 区域版本  bit0:江苏版本标志,置1表示江苏版本,默认为0
    HI_U8  padding[2];
} NV_APP_CLTII_CFG_STRU;


typedef struct
{
    HI_U8 ucFileCheckMsgMaxTimes; //升级文件检查消息超时处理最大重试次数
    HI_U8 ucUpgCmdMsgMaxTimes;   //MRS模块向UPG模块发送的升级命令,如果执行失败，重发的最大次数
/* BEGIN: PN: DTS2015042907476 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/29 */
    HI_U8 ucWaitUpgStartTimeMax;    // 远程升级等待开始升级最大时间，默认值: 180, 单位: 分钟
/* END:   PN: DTS2015042907476 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/29 */
    HI_U8 ucPowerOnStatusMaxTimes; //MRS模块上电后查询升级状态查询超时最大次数
    HI_U16 usMrsUpgFileRxTimeOut; //下装过程中接收升级数据包超时时间,初始值为600，单位为秒
    HI_U16 usMrsUpgStatusQueryTime; //MRS模块向UPG模块发送查询升级状态周期,初始值为60，单位为秒
}NV_APP_MRS_UPG_PARAM_STRU;


typedef struct
{
    HI_U8   ucReportEnable;     // 事件上报标识
    HI_U8   ucCcoMaxTry;
    HI_U16  usCcoTimeOut;       // 单位:ms

    HI_U16  usCcoFilterAgedTIme;//过滤链表老化时间,单位:分钟
    HI_U16  usCcoStatBakTime;   //CCO端事件统计量备份时间,单位:分钟

    HI_U16  usCcoEvtBufSize;    //CCO端事件缓存区大小
    HI_U8   ucMeterEvtType;     //STA识别到的电表事件类型，0：未知，1：13规范非新事件表，默认值为0
	HI_U8   ucStaConsDenyTimes;

    HI_U8   ucStaEvtEnable;     // STA是否允许事件上报
    HI_U8   ucStaMaxTry;        // 事件上报CCO最大重试次数
    HI_U8   ucCltIIEnable;      // II采上报事件开关
    HI_U8   ucEvtTimeOutMax;    // STA发送事件帧超时时间范围最大值 单位:秒

    HI_U8   ucStaTimeOut;       // 单位:ms
    HI_U8   ucStaMonitorInterval;//STA本地监控间隔，单位：分钟
    HI_U16  usPlcSeq;

    HI_U8   ucCltIEnable;
    HI_U8   ucWorkMode;         //工作模式
    HI_U16  usRandomMin;        // 停电站点随机上报时间最小值，单位毫秒，默认0

    HI_U16  usRandomMax;        // 停电站点随机上报时间最大值，单位毫秒，默认200
    HI_U16  usSendInterval; // 停电站点发送间隔时间，单位毫秒，默认2000
    HI_U8   ucSendTimes;        // 停电站点发送停电报文次数，默认10
    HI_U8   ucStaConvergeTime; // 汇集等待时间，单位秒，默认30
    HI_U8   ucPowerFailureReportEnable; //是否检测停电
    HI_U8   ucPowerOffMeterTimeDelay; //考虑到采集器断电后电表有的会延迟点时间 单位:100ms 默认15

    HI_U8   ucCcoConvergeTime; // 汇集等待时间，单位秒，默认30
    HI_U8   pad;
    HI_U16  pad1;
}NV_APP_MRS_EVT_CFG_STRU;


// 大小: 64字节
typedef struct
{
    HI_U8   ucReportEnable;              // 上报从节点状态变化使能开关
    HI_U8   ucPowerOffReasonSwitch;      // 掉电原因上报开关，关闭时将该原因改为未知
    HI_U8   ucPlcChlReasonSwitch;        // 信道原因上报开关，关闭时将该原因改为未知
    HI_U8   ucCacheNumMax;               // 上报前最多缓存的信息条数

    HI_U16  usCacheTimeout;              // 最多等待多长时间开始上报,单位: 100毫秒
    HI_U16  usReportTimeout;             // 06H-F10单次上报的超时时间, 单位: 毫秒

    HI_U8   ucReportRetryMax;            // 06H-F10上报的最大重试次数
    HI_U8   ucDetectEnable; //全网抄表开关 0:不启用 1:启用,默认为1
    HI_U16  usWhiteAddMaxTime; //将以mac地址入网的采集器加入白名单后，如果ucWhilteAddMaxTime时间内没有加入白名单的采集器站点入网,删除添加的白名单,单位分钟

	HI_U16  aus97DI[MRS_BROADCAST_RM_97_DI_NUM];
	HI_U32  ul07DI;

    HI_U8   ucRmTurn;
	HI_U8   ucTopoWaitMaxTime; //如果CCO一直处于组网中且只有CCO一个站点,20分钟后收到拒绝入网通知后将拒绝采集器站点加入表档案白名单,前提是表档案非空,单位分钟
	HI_U8   ucJoinNetMaxTime;  //探测机制中缓存在追加白名单中的站点入网时间,超时后启动探测抄表,单位:分钟
    HI_U8   ucBlackValidTime;  //探测机制中黑名单有效时间，单位:分钟

    HI_U8   ucStaJoinWhiteTime; //被拒绝的站点加入到白名单的时间 单位:分钟
    HI_U8   ucSingleMeterTime;  //每块表预留探测时间 单位:秒
    HI_U8   ucTurnTime; //发送电表地址列表Plc两轮时间间隔 单位:分钟
    HI_U8   ucPoweroffDetectMaxTries;   // 停电探测最大次数 6

    HI_U8   ucPoweroffDetectInterval;   // 停电探测广播报文发送间隔 10
    HI_U8   ucPoweroffRecheckMaxTries;  // 停电复核最大次数 30
    HI_U8   ucPoweroffRecheckInterval;  // 停电复核广播报文发送间隔 120
    HI_U8   ucReviewScoreFull;          // 停电检测复核满分分数 7

    HI_U8   ucWrongReportReduceScore;   // 误报时，减分分数 3
    HI_U8   ucMissReportPlusScore;      // 漏报时，加分分数 1
    HI_U8   ucCorrectReportPlusScore;   // 正确上报时，加分分数 1
    HI_U8   ucNoPassDetectPlusScore;    // 本周期站点没有探测成功的记录时，加分分数 1

    HI_U16  usNoPassDetectPeriod;       // 站点是否有探测成功记录识别周期，单位min 1440
    HI_U16  usReportMaxDelayTime;       // 正确上报：上报时间-停电时间≤此时间，单位min 20

    HI_U16  usMacReportWrongPeriodTh;   // MAC误报周期门限，单位min  10080
    HI_U8   ucPoweroffDetectExpTries;   // 在MAC误报周期门限之内，若有MAC误报，则额外尝试这么多次 6
    HI_U8   ucMacReportCacheMaxTime;    // MAC上报队列缓存最大时长 5

    HI_U16  usMacReportCacheMaxNum;     // MAC上报队列缓存最大个数 1024
    HI_U8   ucScalePowerOffDetectInterval; //10个站点停电时，探测间隔  48
    HI_U8   pad;                            // 0

    HI_U8   aucPad2[16];
} NV_APP_MRS_STATECHG_REPORT_CFG_STRU;


#define NV_SM_RESULT_METER_NUM   (16)

typedef struct
{
    HI_U8 aucAddr[6];           // 搜表结果: 电表地址
    HI_U8 ucParam;              // 搜表结果: 该电表的信息 [0:3]: 电能表规约类型(0透明规约 1-97规约 2-07规约 其它-保留)
                                // [4:7]: 搜表时连续未搜到的次数，连续3次未搜到的表从列表中删除
    HI_U8 ucOption;             // 搜表结果: 该电表若为97规约表: 搜表的DI值 0x10-0x9010 0x1f-0x901f 其它情况:固定为0
}NV_APP_MRS_SM_NODE;


typedef struct
{
    NV_APP_MRS_SM_NODE astMeter[NV_SM_RESULT_METER_NUM];     // 存储的搜表结果
}NV_APP_MRS_SM_RESULT;


// ID: 0x418
typedef struct
{
    HI_U8 ucValid;              // 有效性标识
    HI_U8 ucDutyCycle;          // 占空比控制策略的目标上限百分比
    HI_U16 usMaxSendLength;     // 占空比策略最大可连续发送时长，单位：毫秒
} NV_APP_STA_DUTY_CYCLE_CFG_ITEM;

typedef struct
{
    HI_U8 ucEnable;             // 占空比调整策略开启标识，0：关闭；1：使能
    HI_U8 aucReserved0[3];      // 保留字段，3字节
    HI_U8 ucShortDuration;      // 持续较短时间，单位：分钟；默认值30
    HI_U8 ucShortTimes;         // 短时间重启次数阈值，默认：10（受限于存储空间，不得超过64）
    HI_U8 ucLongDuration;       // 持续较长时间，单位：小时；默认值72
    HI_U8 ucLongTimes;          // 长时间重启次数阈值，默认：30（受限于存储空间，不得超过64）
    NV_APP_STA_DUTY_CYCLE_CFG_ITEM astCfgItem[6]; // 占空比信息

    HI_U32 aulReserved1[4];
} NV_APP_STA_DUTY_CYCLE_CFG_INFO;

typedef struct
{
    HI_U32 ulUpgId;

    HI_U8  ucStatus;
    HI_U8  ucFileId;
    HI_U16 usReserved;
}NV_APP_CLTI_CSG_CFG_STRU;//I采配置参数相关NV 0x427 ID_NV_APP_CLTI_CFG

// ID: 0x40C ~ 0x40F
typedef struct
{
    HI_U8 aucData[128];
} NV_APP_NVM_DATA_STRU;

typedef struct
{
    HI_U8 aucData[64];
}NV_APP_RESERVED_STRU;


typedef struct
{
    HI_U32 reserved[4];//预留
}NV_APP_CCO_PARAM_S;//0x431


#endif //__APP_NV_H__


