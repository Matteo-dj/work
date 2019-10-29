//*****************************************************************************
//
//                  版权所有 (C), 1998-2011, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_srv_cco.h
//  版 本 号   : V1.0
//  作    者   : liuxipeng/KF54842
//  生成日期   : 2011-11-30
//  功能描述   : TODO: ...
//
//  函数列表   : TODO: ...
//  修改历史   :
//  1.日    期 : 2011-11-30
//    作    者 : liuxipeng/KF54842
//    修改内容 : 创建文件
//
//*****************************************************************************
#ifndef __MRS_SERVICE_CCO_H__
#define __MRS_SERVICE_CCO_H__

#include "hi_mdm.h"
#include "mrs_srv_list.h"

HI_START_HEADER


#define MRS_CCO_READ_METER_TASK_END     1 //抄表任务结束
#define MRS_CCO_SEARCH_METER_TASK_END   2 //搜表任务结束

#define MRS_PARALLEL_NUM_CHECK_MODE     (7)     // 送检模式下最大并发数
//#define MRS_PARALLEL_NUM_MAX_DEFAULT    (5)     // 默认最大并发数

#define MRS_CCO_ARCHIVES_STATUS_EFFECT     (0)
#define MRS_CCO_ARCHIVES_STATUS_WAITING    (1)

#define MRS_CCO_ARCHIVES_NUM_MINI          (7) //当表档案数量不大于该数据时,CCO收到添加任务
#define MRS_LOW_POWER_AFE_METER_NUM_MAX    (2) //当cco中表档案数量不大于2时，降低发射功率

typedef struct
{
    HI_BOOL bValid;         // 有效标志 HI_TRUE: 有效  HI_FALSE: 无效
    HI_U8   ucProtocol;     // 规约类型 00:保留(未知) 01:97-645 02:07-645 其它:未知
    HI_U16  usNodeSeq;      // 表档案中从节点(电表)序号
    HI_U8   aucNodeAddr[HI_METER_ADDR_LEN]; // 从节点(电表)地址
    HI_U8   ucSave : 1;     // 存盘标志 0:不需要存盘  1:需要存盘
    HI_U8   ucRead : 2;     // 抄读标识 0:未抄读  1:已抄读
    HI_U8   ucDetect : 1;    // 探测抄表当前抄读状态 0:不需要抄读  1:需要抄读
    HI_U8   ucRmTurn : 2;   // 探测抄表当前抄读轮数
    HI_U8   ucReserved : 2;
    HI_U8   ucCS;           // 校验和
} MRS_ARCHIVES_NODE_STRU;  // 表档案从节点(电表)信息结构

typedef struct
{
    HI_S8 Try;              // 当前重试次数
    HI_S8 MaxTry;           // 最大重试次数
    HI_S8 MaxTryNvS;        // 最大重试次数参数(单播)
    HI_S8 MaxTryNvB;        // 最大重试次数参数(广播)
    HI_S32 OverTime;        // 超时时间
    HI_S32 LayTimeOutNv;    // 层级超时时间参数
    HI_S32 MeterTimeOutNv;  // 电表超时时间参数
    HI_S32 RevisoryNv;      // CCO侧PLC超时修正时间
    HI_S32 CtrlCmdDelayNv;  // 补偿控制命令延时参数(电表侧)
    HI_S32 CltIDelayNv;     // I型采集器延时参数
    HI_U8 IsCtrlCmdFlg;     // 控制命令标志(如果是控制命令则取值为1)
    HI_U8 MaxTimeout;       // 最大超时时间 单位:秒
    HI_S8 Loop;             // 当前轮次
    HI_S8 MaxTryLoop;       // 最大重试轮次
/* BEGIN: Modified by liuxipeng/lKF54842,2014/5/30 问题单号:DTS2014052908512 */
    HI_U16 MeterFrameInterval;  // STA端645帧间隔时间
    HI_U8 reserved[2];
/* END  : Modified by liuxipeng/lKF54842,2014/5/30 问题单号:DTS2014052908512 */
}PLC_OVERTIME_INF_STRU;   // PLC通道超时相关信息

typedef enum
{
    EN_MRS_SRV_CCO_READMETER_IDLE,   // 空闲
    EN_MRS_SRV_CCO_READMETER_ASK,    // 询问
    EN_MRS_SRV_CCO_READMETER_WAIT_ASK_REPLY,    // 等待询问的应答
    EN_MRS_SRV_CCO_READMETER_WAIT_REVISE_RESP,  // 等待修正时延(14-F3)的应答
    EN_MRS_SRV_CCO_READMETER_SEND_PLC,          // 发送读表数据
    EN_MRS_SRV_CCO_READMETER_WAIT_PLC_REPLY,    // 等待读表数据应答
    EN_MRS_SRV_CCO_READMETER_REPEAT_PLC,        // 重发读表数据
    EN_MRS_SRV_CCO_READMETER_UPDATA,            // 上传读表数据(上传至集中器)
    EN_MRS_SRV_CCO_READMETER_WAIT_UPDATA_ACK,   // 等待上传读表数据确认
    EN_MRS_SRV_CCO_READMETER_REPEAT_UPDATA,     // 重发上传读表数据
    EN_MRS_SRV_CCO_READMETER_ERR,               // 错误状态
}MRS_MRV_CCO_READMETER_STATE_ENUM;  // 读表模块状态

typedef enum
{
    EN_MRS_SRV_CCO_METERINF_STATE_FIRST,     // 首次上电
    EN_MRS_SRV_CCO_METERINF_STATE_NORMAL,    // 正常
}MRS_MRV_CCO_METERINF_STATE_ENUM;  // 表信息状态

typedef struct
{
    HI_U16 usDataLen;
    HI_U8 ucData[0];
}UP_READMETER_DATA;

typedef struct
{
    MRS_MRV_CCO_READMETER_STATE_ENUM enCurrState;    // 抄表模块状态
    MRS_MRV_CCO_METERINF_STATE_ENUM enMeterInfState;  // 表信息状态
    MRS_ARCHIVES_NODE_STRU stCurrNode;   // 当前处理的节点(读表)
    MRS_PLC_FRAME_DATA_STRU *pstPlcDataDl; // 转发的plc下行数据(CC0->STA)
    AFN_14_F1_DL_PARAM * pstAfn14F1;    // 读表数据指针(备份)
    UP_READMETER_DATA * pstUpReadMeterData; // 上传读表数据(备份)
    HI_U16 usMeterNum;
    HI_U16 usCurrNodeSeq; //读表模块中抄读的节点序号
    PLC_OVERTIME_INF_STRU stOverTimeInf;    // PLC通道超时时间相关信息
    HI_U32 ulTimeOutMaxNumNv;  // 节点最大连续超时次数(Nv项)
    HI_U32 ulTimeOutCount;  // 当前节点数据项连续超时次数
    HI_U32 ulTurnNumNv;     // 最大轮次数(Nv项)
    HI_U32 ulCurrTurn;      // 当前轮次数
    HI_U16 seq;  //帧序号
    HI_U16 usSuccNum;    //置为抄读成功的节点数量
    HI_U16 usAfn06F2UpTimeOut;      //06F2上行报文超时时间(Nv项，单位ms)
    HI_U8  ucAfn06F2UpRetryMax;     //06F2上行报文超时重试次数(Nv项)
    HI_U8  ucAfn14F1UpRetryMax;     //14F1上行报文超时重试次数(Nv项)
    HI_U16 usAfn14F1UpTimeOut;      //14F1上行报文超时时间(Nv项，单位ms)
    HI_U8  ucCurrNodeInquireTries;  //本轮次当前节点问询次数
    HI_U8  ucRsv;
}MR_READ_METER_STRU;    // 读表模块结构

typedef enum
{
    EN_MRS_ANYTIME_RM_IDLE = 0,         // 空闲状态
    EN_MRS_ANYTIME_RM_WAIT_REVISE_RESP, // 等待延时修正应答状态
    EN_MRS_ANYTIME_RM_REVISE_RESP_PROC,// 延时修正应答处理状态
    EN_MRS_ANYTIME_RM_WAIT_CHLAVAIL,    // 等待通道可用状态
    EN_MRS_ANYTIME_RM_WAIT_PLC,         // 等待PLC应答状态
    EN_MRS_ANYTIME_RM_PRE_RESUME,       // 预备恢复状态(处于此状态后一定时间CCO状态恢复到EN_MRS_CCO_NORMAL)
}MRS_ANYTIME_RM_STATE_ENUM;     // 随抄状态

typedef struct
{
    MRS_ANYTIME_RM_STATE_ENUM enState;      // 随抄状态
    MRS_PLC_FRAME_DATA_STRU *pstPlcDataDl;  // 转发的plc下行数据(CC0->STA)
    HI_U32 ulTimeOut;                       // 随抄超时时间
    PLC_OVERTIME_INF_STRU stOverTimeInf;    // PLC通道超时时间相关信息
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
    PLC_OVERTIME_INF_STRU stCsgOverTimeInf;    // 广东PLC通道超时时间相关信息
#endif
    HI_U16 seq;  //帧序号
    HI_BOOL bIsBC;                          // 是否为广播式读表
    HI_U8 ucProtocol;
    HI_U8 seq_1376_2;
    HI_U8 from;         // 点抄读表帧来源
                        // XR_FROM_AFN02_F1 (转发通讯协议数据帧)
                        // XR_FROM_AFN13_F1 (监控载波从节点)
    HI_U8 aucMeterAddr[HI_METER_ADDR_LEN];
}MR_ANYTIME_READMETER_STRU;    // 随抄读表模块

typedef struct
{
    HI_U8  bc_send_interval;
    HI_U8  bc_loop_max;

    HI_U8  topo_query_interval;
    HI_U8  topo_stable_times;

    HI_U8  query_max_way;
    HI_U8  query_interval;
    HI_U8  query_force_times;
    HI_U8  query_except_times;

    HI_U8  search_finish_reason;
/* BEGIN: PN: DTS2015051601564 MODIFY\ADD\DEL by cuiate/00233580 at 2015/5/11 */
    HI_U8  report_AA_2_master;
    HI_U16 usAfn06F4UpTimeOut;       // 搜表06F4上报超时时间，单位：ms
    HI_U8  ucAfn06F4UpRetryMax;       // 搜表06F4上报最大重试次数
    HI_U8  ucBindCmdSendInterVal;   // 锁定报文发送间隔
    HI_U8  ucBindCmdSendMaxWay;     // 锁定报文一次最大发几帧
    HI_U8  ucBindCmdSendMaxTimes;   // 锁定报文最大发送次数

    HI_U8  ucRecoverWhiteSwitch;    // 搜表结束时，是否恢复搜表前白名单状态
    HI_U8  ucCloseWhiteWhenSM;      // 搜表过程是否关闭白名单
    HI_U16 usSMDurationDefault;     // 搜表默认时长，单位min

    HI_U32  seq;
} MRS_SEARCH_METER_CFG_S;

typedef struct
{
    MRS_PLC_FRAME_DATA_STRU *plc;

    HI_U8   send_retry;
    HI_U8   retry_max;
    HI_U16  reserved;
} MRS_SEARCH_METER_FRAME_S;


typedef struct
{
    MRS_SRV_NODE link;
    HI_U8   mac[HI_PLC_MAC_ADDR_LEN];
    HI_U8   asset[HI_METER_ADDR_LEN];
    HI_U8   id[HI_PLC_MAC_ADDR_LEN];
    HI_U8   type;
    HI_U8   num;
    HI_U8   item[0];
} METER_NODE;


typedef struct
{
    HI_U8   mac[HI_PLC_MAC_ADDR_LEN];
    HI_U8   query;
    HI_U8   type   : 4;
    HI_U8   result : 3;     // 参考MRS_CCO_SM_RESULT_E
    HI_U8   valid  : 1;
} MAC_ITEM;

typedef struct
{
    HI_U16 num;
    HI_U16 current;
    HI_U16 bind_current;
    HI_U16 pad;
    MAC_ITEM mac[PRODUCT_CFG_MRS_MAX_TOPO_NUM];
} MRS_SEARCH_METER_MAC_LIST;


// 全网搜表CCO可维可测
typedef struct
{
    HI_U16 usUserDefinedDuration;               // 用户设定搜表时长，单位: 分
    HI_U16 usTotal;                             // 当前CCO已经保存的从节点信息总条数
    HI_U16 usMeterEntryNum;                     // 当前CCO已经保存的载波电表从节点信息的总条数
    HI_U16 usCltEntryNum;                       // 当前CCO已经保存的采集器从节点信息的总条数
    HI_U32 ulStartTime;                         // 搜表开始的时间戳
    HI_U32 ulEndTime;                           // 搜表停止的时间戳
} MRS_CCO_SEARCH_DFX_STRU;

typedef struct
{
    HI_U16 usCurrNum;           // 当前映射表中处理的从节点序号
    HI_U16 usCurrSeq;           // 当前上报从节点信息队列中从节点序号
    HI_U8  ucReserved;          // 保留字段
    HI_U8  ucSendTimeCount;     // 在搜表中发送询问时钟的总次数计数
    HI_U16 usReportedNum;       // 在上报队列中已上报从节点个数

    HI_BOOL bQueryComplete;
    /* BEGIN: Added by l00238134, 2014/11/5   PN:DTS2014110801400*/
    HI_BOOL bWhiteListSwitch;
    HI_BOOL bWhiteListDelayTimer;
    HI_U8  ucWhiteListDelayTime;
    /* END:   Added by l00238134, 2014/11/5 */

    HI_U16 usStableTimes;
    HI_U16 usFrameLen;
    HI_U8 * pReportFrame;

    MR_NODE_METER_ADD_STRU * pReportedNodeInf;   //上报从节点信息队列

    MRS_SEARCH_METER_CFG_S stSearchCfg;
    MRS_SEARCH_METER_FRAME_S stSearchFrame;
    MRS_SEARCH_METER_MAC_LIST stMacList;
    MRS_SRV_QUEUE stMeterQueue;
    MRS_SRV_LIST stAbnormalCltii;
    MRS_SRV_LIST stSearchResultMeter;
    MRS_SRV_LIST stSearchResultClt;
    MRS_CCO_SEARCH_DFX_STRU stSearchDfx;
}MR_AUTO_SEARCH_STRU;  //自动搜表结构

typedef struct
{
    HI_U16 usReadFailNum;
    HI_U16 reserved;
    MR_NODE_METER_ADD_STRU * pReportedNodeInf;
} MR_READ_FAIL_STRU;

typedef struct
{
    HI_U32 ulFileFlag;      //文件标志 0x0101
    HI_U16 usVersion;       //版本信息  0x02
    HI_U16 usMaxNodeNum;    //最大支持节点数量
    //HI_U16 usRealNodeNum; //flash中真实存储的表信息条数
} MRS_FLASH_METER_INFO_FILE_HEAD_STRU; //flash中所存储的表档案文件头结构


#define MRS_CCO_STATUS_CREATE_MAP               (0x00000001)    // 映射表维护(创建映射表)
#define MRS_CCO_STATUS_NORMAL                   (0x00000002)    // 正常状态(允许询问式抄表)
#define MRS_CCO_STATUS_ANYTIME_READMETER        (0x00000004)    // 随抄状态(点抄式抄表)
#define MRS_CCO_STATUS_SEARCH_METER             (0x00000010)    // 搜索电表状态
/* BEGIN: PN: DTS2015061008265 MODIFY\ADD\DEL by cuiate/00233580 at 2015/6/23 */
#define MRS_CCO_STATUS_SEARCH_METER_TRANSFORMER (0x00000020)    // 台区识别搜表状态

#define MRS_CCO_STATUS_SEARCH_METER_MASK        (MRS_CCO_STATUS_SEARCH_METER | MRS_CCO_STATUS_SEARCH_METER_TRANSFORMER)
/* END:   PN: DTS2015061008265 MODIFY\ADD\DEL by cuiate/00233580 at 2015/6/23 */


#define MRS_CCO_NON_WORKING_STATUS              (0xFF)
#define MRS_CCO_WORKING_STATUS                  (1)

#define MRS_CCO_WORK_SWITCH_LOCAL_READ_METER    (0)
#define MRS_CCO_WORK_SWITCH_SEARCH_METER        (1)
#define MRS_CCO_WORK_SWITCH_REMOTE_UPGRADE      (2)
#define MRS_CCO_WORK_SWITCH_OTHER               (3)

#define MRS_CCO_LR_MODE_SINGLE                  (0)
#define MRS_CCO_LR_MODE_CONTINUOUS              (1)

#define MRS_CCO_SEQ_START_FROM_0                (0)
#define MRS_CCO_SEQ_START_FROM_1                (1)


#if defined(PRODUCT_CFG_SYS_RST_SOLUTION)
typedef struct
{
    HI_U32 ulRMStatTimes;       //抄表次数统计
    HI_U32 ulCcoRxUartTimes;    //CCO接收集中器报文统计次数
}MRS_CCO_RESET_PARAM_S;//CCO主动复位相关参数信息
#endif

typedef enum
{
    MRS_CCO_BROADCAST_IDLE,
    MRS_CCO_BROADCAST_BUSY,
} MRS_CCO_BROADCAST_STATE;

typedef struct
{
    HI_U8 ucInterval;
    HI_U8 ucRetryMax;
    HI_U8 ucRetry;
    HI_U8 ucState;
} MRS_CCO_BROADCAST_PARAM;

typedef struct
{
    MRS_CCO_BROADCAST_PARAM stParam;
    MRS_PLC_FRAME_DATA_STRU *pstPlcDataDl;  // 转发的plc下行数据(CC0->STA)
    HI_U8 ucSeq;
    HI_U8 padding[3];
    HI_VOID (*pfnBroadcastFinishNotify)(HI_VOID*);
    HI_VOID *pUserParam;
}MRS_CCO_BROADCAST_STRU;

typedef enum
{
    MRS_EVENT_REPORT_STATUS_IDLE,
    MRS_EVENT_REPORT_STATUS_QUERYING,
    MRS_EVENT_REPORT_STATUS_REPORTING,
} MRS_EVENT_REPORT_STATUS_E;

// 查询一轮之间的间隔
#define MRS_EVENT_QUERYING_LOOP_INTERVAL    (6000)  // 6s

// 每次查询之间的间隔
#define MRS_EVENT_QUERYING_INTERVAL         (1000)  // 1s

// 读表时的拓扑场景类型
#define MRS_RM_SCENE_UNKNOWN                     (0)   // 读表时的拓扑场景: 未知场景
#define MRS_RM_SCENE_TOPO_HAS_CLTI               (1)   // 读表时的拓扑场景: 拓扑中含有I型采集器载波站点
#define MRS_RM_SCENE_TOPO_HAS_CLTII_NO_CLTI      (2)   // 读表时的拓扑场景: 拓扑中含有II型采集器,没有I型采集器载波站点
#define MRS_RM_SCENE_TOPO_HAS_NO_CLT             (3)   // 读表时的拓扑场景: 拓扑中不含采集器载波站点

#define MRS_CCO_FUNC_DEFAULT                    (0x00)
#define MRS_CCO_FUNC_BBU                        (0x01)
#define MRS_CCO_FUNC_CSG_GD                     (0x02)

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) && defined(PRODUCT_CFG_PRODUCT_TYPE_CCO_BROAD_BAND_UNIT)

#define MRS_CCO_BBU_TIME_MULTI                                  (10)
#define MRS_CCO_BBU_MAX_BYTE_INTERVAL_TIME_ZERO                 (0)
#define MRS_CCO_BBU_MAX_BYTE_INTERVAL_TIME_DEFAULT              (500)

#endif

typedef struct
{
    HI_U32 status;                  // 当前CCO状态
    HI_BOOL bPauseFlag;
    HI_BOOL PlcChStatus;            // 当前PLC通道状态 HI_FALSE : 不可用 HI_TRUE : 可用
    HI_U8   ucTopoScene;            // 拓扑场景类型: 用于读表超时时间计算判断
    HI_U8   ucCheckMode;            // 送检模式 0-非送检模式 1-送检模式

    HI_U16  usPlcSeq;               // PLC报文序号-广东电网

    HI_U8 ucMainNodeAddr[6];        // 载波主节点地址
    MR_ANYTIME_READMETER_STRU stAnytimeRMModel;         // 随抄读表模块
    MR_READ_METER_STRU stReadMeterModel;                // 读表模块
    MR_AUTO_SEARCH_STRU stAutoSearchModel;              // 自动搜表模块
#if defined(PRODUCT_CFG_SYS_RST_SOLUTION)
    MRS_CCO_RESET_PARAM_S stResetStatInfo;     //复位相关统计信息
#endif

    MRS_MAP_MODULE stMapModule;     //映射表模块

    HI_U8 ucSeq;                    // 下行帧序号
    HI_U8 ucSeqControl      : 1;    // 0:序号统一从0开始，1:序号统一从1开始
    HI_U8 ucFreqEnable      : 1;    // CCO获取到时钟后启动频段定时的使能开关 0:不启用;1:启用
    HI_U8 ucDutBoardFlag    : 1;    // 工装测试中点抄使用广播抄表标志 0:不启用  1:启用
    HI_U8 ucLrModeFlag      : 1;    // 轮抄模式 0:单轮模式 1:连续模式
    HI_U8 ucSearchWaitFlag  : 1;    // 搜表等待组网标识
    HI_U8 bFirstRun         : 1;    // 首次运行标志
    HI_U8 bNeedGetTime      : 1;    // 需要获取时间标志
    HI_U8 bCsgParallel      : 1;    // 广东电网并发使能标志

    HI_U8 ucProtoVer;               // 规范版本 0-09版 1-13版 其它-保留
    HI_U8 plc_rxtx_mode;            // PLC通道收发模式 00:等待组网路由优化完成后才进行PLC通道的收发; 01:强制PLC通道收发(不等待PLC状态);

    HI_U8 ucFlashSaveDelay;         // 表档案存盘激活时间
    HI_U8 ucArchiveStatus   : 2;    // 表档案生效标志 0-默认生效  1-等待生效  2-已生效
    HI_U8 ucSetPowerFlag    : 1;    // 设置发射功率标志
    HI_U8 reserved          : 5;
    HI_U8 ucFuncCfg;
    HI_U8 ucRecoverPowerTime;       // CCO恢复发射功率所需时间，默认15分钟
    HI_U8 ucSoftTfMode;             // 无扰台区识别功能总开关: 0关闭，1打开(白名单常关，站点自由切换网络)，2打开(白名单常关，站点不主动切换网络)
    HI_U8 ucCcoRstPinEnable;        // CCO端RST管脚使能

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO_BROAD_BAND_UNIT)
    HI_U8 ucMaxByteIntervalTimeBbu;
    HI_U8 ucRunLedStatus;
    HI_U8 reserved1[2];
#endif

    HI_U16 usSearchDuration;        // 搜表持续时间 单位: 分钟

    MRS_CCO_BROADCAST_STRU stBroadcast; //广播模块
    MR_READ_FAIL_STRU stReadFail;       //抄表失败记录

    HI_MAC_NETWORK_TOPO_S stTopoInf;       //拓扑信息
    HI_U8 *pTopoIntegerInfo;      //一份完整拓扑信息，目前用于AFN=10-F21
    HI_U8 LowPowerMeterLastNum;//Matteo
}MRS_CCO_SRV_CTX_STRU;  // 功能模块上下文结构 CCO


// 获取CCO端全局上下文的结构指针
HI_PUBLIC MRS_CCO_SRV_CTX_STRU* mrsCcoGetContext(HI_VOID);

// CCO在任意状态下(除创建映射表状态)恢复为普通状态
HI_PUBLIC HI_VOID mrsRecoverNormalState(MRS_CCO_SRV_CTX_STRU* pstCcoCtx);

//获取当前的维护的MAC
HI_PUBLIC HI_U32 mrsMapGetCurrentMac(MRS_MAP_MODULE *module,HI_U8 mac[]);

HI_PUBLIC HI_BOOL mrsGetNeedGetTimeFlg(HI_VOID);

HI_PUBLIC HI_VOID mrsSetNeedGetTimeFlg(HI_BOOL flg);

// 获取PLC超时信息结构AFN-13
HI_PUBLIC PLC_OVERTIME_INF_STRU *mrsGetPlcOverTimeInfAfn13(HI_VOID);

// 获取PLC超时信息结构AFN-14
HI_PUBLIC PLC_OVERTIME_INF_STRU *mrsGetPlcOverTimeInfAfn14(HI_VOID);

// 获取PLC通道超时时间-AFN13
HI_PUBLIC HI_U32 mrsGetPlcTimeOutAfn13(HI_U8 *mac, HI_BOOL force_bc, HI_U32 *m_time);

// 获取PLC通道超时时间-AFN14
HI_PUBLIC HI_U32 mrsGetPlcTimeOutAfn14(HI_U8 *mac, HI_BOOL force_bc, HI_U32 *m_time);

// 获取PLC通道最大重试次数-AFN13
HI_PUBLIC HI_U8 mrsGetMaxTryAfn13(HI_U8 *mac);

// 获取PLC通道最大重试次数-AFN14
HI_PUBLIC HI_U8 mrsGetMaxTryAfn14(HI_U8 *mac);

// 获取节点Plc超时最大次数(用于跳转节点)
HI_PUBLIC HI_U32 mrsGetPlcTimeOutMaxNum(HI_VOID);

HI_PUBLIC MRS_CCO_BROADCAST_STRU *mrsGetBroadcastCtx(HI_VOID);


// 函数名称: mrsSyncMeterInfToWhiteList
HI_PUBLIC HI_U32 mrsSyncMeterInfToWhiteList(HI_MAC_WHIST_LIST_CHANGE_REASON_ENUM enReason);

/* BEGIN: PN: DTS2015061008265 MODIFY\ADD\DEL by cuiate/00233580 at 2015/6/23 */
// 台区识别启动搜表
HI_PUBLIC HI_U32 mrsCcoStartSearchMeterTransformer(HI_U32 ulDuration);

// 台区识别停止搜表
HI_PUBLIC HI_U32 mrsCcoStopSearchMeterTransformer(HI_VOID);

// 停止普通搜表
HI_PUBLIC HI_U32 mrsCcoStopSearchMeterNormal(HI_U8 ucReason);
/* END:   PN: DTS2015061008265 MODIFY\ADD\DEL by cuiate/00233580 at 2015/6/23 */

// 获取读表最大轮次数量
HI_PUBLIC HI_U32 mrsGetMaxTurnNum(HI_VOID);
// 设置读表最大轮次数量
HI_PUBLIC HI_VOID mrsSetMaxTurnNum(HI_U32 x);

// 获取当前读表轮次数
HI_PUBLIC HI_U32 mrsGetCurrTurnNum(HI_VOID);
// 设置当前读表轮次数
HI_PUBLIC HI_VOID mrsSetCurrTurnNum(HI_U32 x);
// 读表当前轮次自增
HI_PUBLIC HI_VOID mrsCurrTurnGrow(HI_VOID);

// 强制切换为路由暂停状态
HI_PUBLIC HI_VOID mrsForceSwitchPause(HI_VOID);

HI_PUBLIC HI_U32 mrsInitPlcOverTimeInf(HI_VOID);

//*****************************************************************************
// 函数名称: mrsCcoRxAfn13DataPro
// 功能描述: 点抄中CCO收到PLC数据包后的处理
//
// 参数说明:
//   pPayload       [in]   AFN-13数据包
//   usPayloadLen   [in]   AFN-13数据包长度
//   ppData         [out]  转发数据域中的数据
//   pusDataLen     [out]  转发数据域中的数据长度
//
// 返 回 值:
//   HI_ERR_SUCCESS  成功
//   HI_ERR_FAILURE  失败
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: fengxiaomin/f00209182 [2013-02-04]
// 修    改: cuiate/00233580 [2015-09-16]
//*****************************************************************************
HI_PUBLIC HI_U32 mrsCcoRxAfn13DataPro(HI_PVOID pPayload, HI_U16 usPayloadLen, HI_PVOID *ppData, HI_U16 *pusDataLen);

//*****************************************************************************
// 函数名称: mrsCcoRxAfn14DataPro
// 功能描述: 轮抄中CCO收到PLC数据包后的处理
//
// 参数说明:
//   pPayload       [in]   AFN-14数据包
//   usPayloadLen   [in]   AFN-14数据包长度
//   ppData         [out]  转发数据域中的数据
//   pusDataLen     [out]  转发数据域中的数据长度
//
// 返 回 值:
//   HI_ERR_SUCCESS  成功
//   HI_ERR_FAILURE  失败
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: fengxiaomin/f00209182 [2013-02-04]
// 修    改: cuiate/00233580 [2015-09-16]
//*****************************************************************************
HI_PUBLIC HI_U32 mrsCcoRxAfn14DataPro(HI_PVOID pPayload, HI_U16 usPayloadLen, HI_PVOID *ppData, HI_U16 *pusDataLen);


/* BEGIN: PN: DTS2015042907476 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/29 */
HI_PUBLIC HI_BOOL mrsGetCcoPlcChlStatus(HI_VOID);
/* END:   PN: DTS2015042907476 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/29 */

// 获取CCO就绪状态(HI_TRUE : 已就绪可以读表; HI_FALSE : 未就绪)
HI_PUBLIC HI_BOOL mrsGetCcoReadyStatus(HI_U8 ucFlag);

// 返 回 值: PLC接收数据包统计量
//
// 调用要求:
// 调用举例:
// 作    者: fengxiaomin/00209182 [2013-03-02]
//*****************************************************************************
#if defined(PRODUCT_CFG_SYS_RST_SOLUTION)
HI_PUBLIC HI_U32 mrsCcoRMPlcStat(HI_VOID);
#endif

//*****************************************************************************
// 函数名称: mrsCcoRxUartStat
// 功能描述: CCO接收集中器数据包统计
// 参数说明: 无
//
// 返 回 值: CCO接收集中器报文统计量
//
// 调用要求:
// 调用举例:
// 作    者: fengxiaomin/00209182 [2013-03-02]
//*****************************************************************************
#if defined(PRODUCT_CFG_SYS_RST_SOLUTION)
HI_PUBLIC HI_U32 mrsCcoRxUartStat(HI_VOID);
#endif

// Map表维护处理
// option :
// MRS_MAP_MAINTAIN_MODE_ACTIVE          主动维护
// MRS_MAP_MAINTAIN_MODE_PASSIVE         映射表维护模式(被动)-Topo变更驱动
// MRS_MAP_MAINTAIN_MODE_REFRESH         映射表维护模式(补充刷新)-定时器驱动
HI_PUBLIC HI_U32 mrsMapMaintainProcess(HI_U8 option);


HI_PUBLIC HI_U32 mrsCcoReadFailInsert(HI_U8 MeterAddr[HI_METER_ADDR_LEN], HI_U8 protocol);
/* BEGIN: PN: DTS2015042907341 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/29 */
HI_PUBLIC HI_U32 mrsCcoReadFailRemove(HI_U8 MeterAddr[HI_METER_ADDR_LEN], HI_U8 protocol);
/* END:   PN: DTS2015042907341 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/29 */
HI_PUBLIC HI_U32 mrsSeqChangeNotify(HI_U16 usId);
HI_PUBLIC HI_U8 mrsGetCcoWorkFlag(HI_VOID);
HI_PUBLIC HI_VOID mrsCcoStatusInit(MRS_CCO_SRV_CTX_STRU *pstCcoCtx);
HI_PUBLIC HI_VOID mrsCcoReadFailReset(HI_VOID);
HI_PUBLIC HI_U32 mrsRefreshTopoInfo(MRS_CCO_SRV_CTX_STRU *pstCcoCtx);
HI_PUBLIC HI_VOID mrsCcoPowerOnPro(HI_VOID);
HI_PUBLIC HI_VOID mrsGetCcoMrParam(NV_APP_MR_CFG_STRU* cfg);
HI_PUBLIC HI_U32 mrsInitPlcTimeOutMaxNum(HI_VOID);

// 获取读表时的拓扑场景类型
HI_PUBLIC HI_U8 mrsGetRmTopoScene(HI_VOID);

// 获取广播读表超时时间是否加I采补偿的标识
HI_PUBLIC HI_BOOL mrsRmBcTimeoutCltIDelayFlag(HI_VOID);

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO_BROAD_BAND_UNIT)
HI_VOID mrsRunLedProBbu(HI_VOID);
#endif

HI_VOID mrsCcoTopoStatusChgEntryCallback(HI_MDM_TOPO_STATUS_CHG_ENTRY_S* pstEntry);
HI_U32 mrsDiagCcoModeNotify(HI_SYS_QUEUE_MSG_S* pstMsg);
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
PLC_OVERTIME_INF_STRU *mrsGetCsgPlcOverTimeInf(HI_VOID);
HI_VOID mrsSetCsgPlcOverTimeInf(HI_U8 ucPriority);
#endif

HI_U8 mrsGetCheckMode(HI_VOID);
HI_VOID mrsSetArchivesStatus(HI_U8 ucStatus);
HI_U8 mrsGetArchivesStatus(HI_VOID);
HI_U32 mrsCheckMeterNum(HI_VOID);

HI_VOID mrsCcoTopoStatusChgProc(HI_SYS_QUEUE_MSG_S* pstMsg);

HI_END_HEADER
#endif //__MRS_SERVICE_H__

