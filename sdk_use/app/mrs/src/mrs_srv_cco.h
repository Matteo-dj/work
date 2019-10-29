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
#include "mrs_srv_cco_macmap.h"
#include "mrs_fw_proto376_2_frame.h"

HI_START_HEADER


#define MRS_CCO_READ_METER_TASK_END     1 //抄表任务结束
#define MRS_CCO_SEARCH_METER_TASK_END   2 //搜表任务结束


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
    HI_U8   ucRfReportFlag : 1;// 用于水表，已经上报的标志
	HI_U8   ucReserved : 1;
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
    HI_U8  aucMacAdd[HI_PLC_MAC_ADDR_LEN]; //存储plc接收的MAC地址
    HI_U16  ucRsv1;
}MR_READ_METER_STRU;    // 读表模块结构
typedef struct
{
    HI_U8 enCurrRfState;  
    MRS_ARCHIVES_NODE_STRU stRfCurrNode;   // 当前处理的节点(读表)
    HI_U16 usCurrRfNodeSeq; //读表模块中抄读的节点序号
    MRS_PLC_FRAME_DATA_STRU *pstRfPlcDataDl; // 转发的plc下行数据(CC0->STA)
    UP_READMETER_DATA * pstRfUpReadMeterData; // 上传读表数据(备份)
    HI_U16 usRfMeterNum;
    
    PLC_OVERTIME_INF_STRU stRfOverTimeInf;    // PLC通道超时时间相关信息
    HI_U32 ulTimeOutMaxNumNv;  // 节点最大连续超时次数(Nv项)
    HI_U32 ulTimeOutCount;  // 当前节点数据项连续超时次数
    HI_U32 ulTurnNumNv;     // 最大轮次数(Nv项)
    HI_U32 ulCurrTurn;      // 当前轮次数
    HI_U16 usPLCTimeOut;    
    HI_U16 usRfPlcSeq;  //帧序号
    HI_U16 us20F3UpSeq;  //帧序号
    HI_U16 usAfn20F3UpTimeOut;      //上行报文超时时间(Nv项，单位ms)
    HI_U8  ucAfn20F3UpRetryMax;     //上行报文超时重试次数(Nv项)
    HI_U8  ucCurrNodeInquireTries;  //本轮次当前节点问询次数
    HI_U8  aucMacAdd[HI_PLC_MAC_ADDR_LEN]; //存储plc接收的MAC地址
}MR_READ_WATER_METER_STRU; 

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
#if defined(MRS_SRV_PROTOCOL_CJT188) 
	PLC_OVERTIME_INF_STRU stOverTimeInfSaved; //用于存储PLC通道超时时间相关信息。
	HI_S8 ucAdditionTime188;	//188抄表补充时间
	HI_S8 ucPlcMaxReTryS188;	//188抄表单播重试次数
	HI_S8 ucPlcMaxReTryB188;	//188抄表广播重试次数
	HI_U8 reserved;
#endif
    HI_U16 seq;  //帧序号
    HI_BOOL bIsBC;                          // 是否为广播式读表
    HI_U8 ucProtocol;
    HI_U8 seq_1376_2;
    HI_U8 from;         // 点抄读表帧来源
                        // XR_FROM_AFN02_F1 (转发通讯协议数据帧)
                        // XR_FROM_AFN13_F1 (监控载波从节点)
    HI_U8 ucFn; // beijing,02F2 和02F1 一致的格式，就是有不一样的处理
    HI_U8 aucMeterAddr[HI_METER_ADDR_LEN];
}MR_ANYTIME_READMETER_STRU;    // 随抄读表模块

typedef struct
{
    HI_U8   bc_send_interval;
    HI_U8   bc_loop_max;
    HI_U8   topo_query_interval;
    HI_U8   topo_stable_times;

    HI_U8   query_max_way;
    HI_U8   query_interval;
    HI_U8   query_force_times;
    HI_U8   query_except_times;

    HI_U8   search_finish_reason;
    HI_U8   report_AA_2_master;
    HI_U16  usAfn06F4UpTimeOut;     // 搜表06F4上报超时时间，单位：ms

    HI_U8   ucAfn06F4UpRetryMax;    // 搜表06F4上报最大重试次数
    HI_U8   ucBindNetwork;          // 是否使能锁定功能
    HI_U16  usSMDurationDefault;    // 搜表默认时长，单位min

    HI_U8   ucRecoverWhiteSwitch;   // 搜表结束时，是否恢复白名单开关状态
    HI_U8   ucBindCmdSendInterval;  // 锁定命令每轮之间的发送间隔，单位s
    HI_U8   ucCheckMainNodeAddr;    // 检查主节点地址改变时，重新进行台区识别
    HI_U8   ucAllowSMInTf;          // 台区识别中是否允许搜表

    HI_U16  usCheckMainNodeTimeout; // 检查主节点地址超时时间, 单位s
    HI_U8   ucSmReportWaitLockMsg;  // 搜表上报是否等待锁定报文
    HI_U8   ucCloseWhiteWhenSM;     // 搜表过程是否关闭白名单

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
    MRS_SRV_LINK link;

    METER_NODE * pstMeterNode;

    HI_U8 query_times;
    HI_U8 pad[3];
} METER_LIST_NODE;

typedef struct
{
    HI_U8  aucMac[HI_PLC_MAC_ADDR_LEN];
    HI_U16 pad;
    MRS_SRV_LINK * link;
} MRS_METER_LIST_NODE_GET_STRU;

typedef struct
{
    HI_U8   mac[HI_PLC_MAC_ADDR_LEN];
    HI_U8   query;
    HI_U8   type   : 4;
    HI_U8   result : 3;
    HI_U8   valid  : 1;
} MAC_ITEM;

typedef struct
{
    HI_U16 num;
    HI_U16 current;
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
    HI_U8  ucCurNodeReported;   // 当前节点已上报
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

    HI_BOOL bCheckingMainAddr;      // 是否正在检查主节点地址
    HI_BOOL bLockComplete;          // 是否锁定完成
#if defined(PRODUCT_CFG_HUNAN)
	HI_BOOL bNewMeterSearch:1;	//是否新增电表搜表流程
    HI_BOOL bNodeCheck:1;	//是否已检查档案信息
    HI_U8 ucNotInArchivesNum;
#endif
    HI_U8   pad[2];
    MRS_SRV_LIST stUnLockStaList;   // 未上报站点链表
    MRS_SRV_LINK *pstCurLockNode;   // 当前正在查询的链表节点
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

#define MRS_CCO_LEVEL_DISABLE                   (0)
#define MRS_CCO_LEVEL_ENABLE                    (1)


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
    HI_U32 reserved;
}MRS_CCO_BROADCAST_STRU;

typedef enum
{
    MRS_EVENT_REPORT_STATUS_IDLE,
    MRS_EVENT_REPORT_STATUS_QUERYING,
    MRS_EVENT_REPORT_STATUS_REPORTING,
} MRS_EVENT_REPORT_STATUS_E;
typedef HI_U32 (* MRS_SEND_CCO_PROC_F)(HI_VOID *);
typedef HI_VOID (* MRS_SEND_CCO_PROC_RX_F)(HI_VOID *);

// 查询一轮之间的间隔
#define MRS_EVENT_QUERYING_LOOP_INTERVAL    (6000)  // 6s

// 每次查询之间的间隔
#define MRS_EVENT_QUERYING_INTERVAL         (1000)  // 1s

// 读表时的拓扑场景类型
#define MRS_RM_SCENE_UNKNOWN                     (0)   // 读表时的拓扑场景: 未知场景
#define MRS_RM_SCENE_TOPO_HAS_CLTI               (1)   // 读表时的拓扑场景: 拓扑中含有I型采集器载波站点
#define MRS_RM_SCENE_TOPO_HAS_CLTII_NO_CLTI      (2)   // 读表时的拓扑场景: 拓扑中含有II型采集器,没有I型采集器载波站点
#define MRS_RM_SCENE_TOPO_HAS_NO_CLT             (3)   // 读表时的拓扑场景: 拓扑中不含采集器载波站点

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) && defined(PRODUCT_CFG_PRODUCT_TYPE_CCO_BROAD_BAND_UNIT)
#define MRS_CCO_FUNC_DEFAULT                                    (0)
#define MRS_CCO_FUNC_BBU                                        (0x1)

#define MRS_CCO_BBU_TIME_MULTI                                  (10)
#define MRS_CCO_BBU_MAX_BYTE_INTERVAL_TIME_ZERO                 (0)
#define MRS_CCO_BBU_MAX_BYTE_INTERVAL_TIME_DEFAULT              (500)

#define MRS_CCO_BBU_MASK             (0x01)
#endif

#if defined(PRODUCT_CFG_HUNAN)
typedef struct
{
    HI_U8  mac[HI_PLC_MAC_ADDR_LEN];          /**< MAC地址             */
    HI_U8  ucUpdateFlg;                       /**< 更新标志                  */
    HI_U8  aucModuleId[11];                   /**< 模块ID                  */
}HI_MODULE_ID_S;

typedef struct
{
    HI_U32 num;			/**<数组元素个数*/
    HI_MODULE_ID_S entry[HI_MAC_NETWORK_TOPO_ENTRY_MAX];	/**<模块ID信息*/
}HI_MAC_NETWORK_MODULE_ID_S;
#endif

typedef struct
{
	MRS_SRV_LINK link;
    HI_U8  ucaStaMac[HI_PLC_MAC_ADDR_LEN];
	HI_U8 ucAddressType; // 0代表STA 类型，1代表为CCO
	HI_U8  ucMeterNum;
	HI_U8  ucSendIndex;
    HI_U8  pMeterData[0];
} MRS_SEARCH_WATER_METER_LIST;
typedef struct
{
    HI_U8  ucaWaterMac[7];
	HI_U8  ucRFChannelInfoNum;
	HI_U8  ucWaterMeterIndex; // 最高场强值的表地址
	HI_U8  ucWaterMaxTryCount; // 连续抄读次数失败切换地址的次数
	HI_U8  ucCcoWaterMeterFlag; // 接在CCO 上的水表模块
	HI_U8  ucaStaMac[2][6];
	HI_U8  ucaFieldValue[2]; // 场强信息
} MRS_WATER_METER_RESULT;

//此变量用于记录当ndm的plc点抄引起的忙返回处理时间
#define MRS_CCO_NDM_PLC_BUSY_PROC    (2000)
#define MRS_WATER_METER_MAX_NUM 64

typedef struct
{
    HI_U16 usCurrNum;           // 当前映射表中处理的从节点序号
    HI_U16 usCurrSeq;           // 当前上报从节点信息队列中从节点序号
    HI_U8  ucCurNodeReported;   // 当前节点已上报
    HI_U8  ucSendTimeCount;     // 在搜表中发送询问时钟的总次数计数
    HI_U16 usReportedNum;       // 在上报队列中已上报从节点个数

	HI_U16 usWaterSearchSeq;	// 序列号
	HI_U16 usSignleWaterAllNum; // 单个水表的总数量
	HI_U8 ucSignleReportNum;	// 已经上报的水表数量

	HI_U8 ucSetOwnerShipMeterNum;// 设置水表所属关系表的数量
	
	HI_U8 ucReadWaterMeterDataIndex;//抄读水表时，记录使用的是哪个地址 

    HI_BOOL bQueryComplete;
	HI_BOOL bStartReadMeterFlag; // 读取表信息
	HI_BOOL bStartSetMeterShipFlag; // 开始设置表信息
	HI_BOOL bSetMeterShipSendPLCFlag; // 开始设置表发送了PLC信息

    HI_U16 usStableTimes;
	HI_U16 usWaterMeterResultNum;
	MRS_WATER_METER_RESULT stWaterMeterResult[MRS_WATER_METER_MAX_NUM]; // 根据链接获取的数据
    MRS_SEARCH_METER_FRAME_S stSearchWaterFrame;
    MRS_SRV_LIST stGetWaterMeterList;   // 上报站点链表的数据链表MRS_SEARCH_WATER_METER_LIST
}MR_WATER_METER_SEARCH_STRU;

typedef struct
{
	HI_U8     ndm_addr[HI_PLC_MAC_ADDR_LEN];	    //抄控器地址
	HI_U8     src_addr[HI_PLC_MAC_ADDR_LEN];	    //抄控器下发plc帧里包含的3762帧的源地址
	HI_U8     old_from;                             //点抄读表来源
	HI_U8     pro_type;                             //协议类型 plc帧头数据
	HI_U16    frame_len;			                //plc包含数据长度 plc帧头数据
}MR_CCO_NDM_PLC_INFO_STRU;                       //抄控器下发到cco信息记录  
typedef struct
{
	HI_U8             busy_from;                            // 缓冲的点抄帧来源
	HI_U8             reserved[3];                          // 保留
	MRS_3762_FRAME_STRU* pst_frame;                         // 抄控器点抄出现忙返回时的参数记录
	MR_CCO_NDM_PLC_INFO_STRU ndm_plc_info;                  // ndm帧plc忙返回时的信息记录
}MR_CCO_NDM_PLC_BUSY_STRU;                              // 当集中器抄表与抄控器点抄afn13时出现忙返回的处理结构体


//Matteo
typedef struct
{
	HI_U16 num;
    HI_U16 current;
    MAC_ITEM IICltmac[500];
}II_ARCHVICE;


typedef struct
{
    HI_U32 status;                  // 当前CCO状态
    HI_BOOL bPauseFlag;
    HI_BOOL PlcChStatus;            // 当前PLC通道状态 HI_FALSE : 不可用 HI_TRUE : 可用
    HI_U8   ucTopoScene;            // 拓扑场景类型: 用于读表超时时间计算判断
    HI_BOOL bFirstRun;              // 首次运行标志
    HI_BOOL bNeedGetTime;           // 需要获取时间标志
    HI_U8   ucCcoRstPinEnable;      // 去使能CCO端RST管脚
    HI_U8 ucMainNodeAddr[6];        // 载波主节点地址
    MR_ANYTIME_READMETER_STRU stAnytimeRMModel;         // 随抄读表模块
    MR_READ_METER_STRU stReadMeterModel;                // 读表模块
    MR_AUTO_SEARCH_STRU stAutoSearchModel;              // 自动搜表模块
#if defined(PRODUCT_CFG_SYS_RST_SOLUTION)
    MRS_CCO_RESET_PARAM_S stResetStatInfo;     //复位相关统计信息
#endif
	MR_READ_WATER_METER_STRU stReadWaterMeterModel; // 读取水表模块
	MR_WATER_METER_SEARCH_STRU stWaterMeterSearchModel;              // 自动搜表模块
    MRS_MAP_MODULE stMapModule;     //映射表模块
    HI_U8 aucSrcMac[HI_PLC_MAC_ADDR_LEN];
    HI_S16 sDutNtbAjustBase;

    HI_U8 ucSeq;                    // 下行帧序号
    HI_U8 ucSeqControl      : 1;    // 0:序号统一从0开始，1:序号统一从1开始
    HI_U8 ucFreqEnable      : 1;    // CCO获取到时钟后启动频段定时的使能开关 0:不启用;1:启用
    HI_U8 ucDutBoardFlag    : 1;    // 工装测试中点抄使用广播抄表标志 0:不启用  1:启用
    HI_U8 ucLrModeFlag      : 1;    // 轮抄模式 0:单轮模式 1:连续模式
/* BEGIN: PN: DTS2015042907476 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/29 */
    HI_U8 ucSearchWaitFlag  : 1;    // 搜表等待组网标识
    HI_U8 ucLevelFlag       : 1;    // 中继级数标志:0:不启用 1:启用
    HI_U8 ucSetFreqFlag     : 1;
    HI_U8 ucPadding         : 1;
/* END:   PN: DTS2015042907476 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/29 */

    HI_U8 ucProtoVer;               // 规范版本 0-09版 1-13版 其它-保留
    HI_U8 plc_rxtx_mode;            // PLC通道收发模式 00:等待组网路由优化完成后才进行PLC通道的收发; 01:强制PLC通道收发(不等待PLC状态);

    HI_U8 ucFlashSaveDelay;         // 表档案存盘激活时间
    HI_U8 ucPhase           : 3;
    HI_U8 reserved          : 5;

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO_BROAD_BAND_UNIT)
    HI_U8 ucFuncCfg;
    HI_U8 ucMaxByteIntervalTimeBbu;
    HI_U8 ucRunLedStatus;
    HI_U8 reserved1;
#endif
	HI_U32 ulCcoNetSnid;
    HI_U16 usSearchDuration;        // 搜表持续时间 单位: 分钟
    HI_U8  ucFreq;
    HI_U8  ucSoftTfMode; // 无扰台区识别模式
	HI_U8  aucCode[2];
    HI_U8  area_code;
    HI_U8  pad[3];
	HI_U16  ucRfTimeSlot; // RF  设置时隙时间发送时间，分钟
	HI_U16	ucRfSGDTimeSlot; // 国网设置RF  设置时隙时间发送时间，分钟
#if defined(MRS_SRV_MODULE_ID)
	//HI_U8 aucModuleId[MRS_SRV_MODULE_ID_LEN_MAX];
#endif

    MRS_CCO_BROADCAST_STRU stBroadcast; //广播模块
    MR_READ_FAIL_STRU stReadFail;       //抄表失败记录

    HI_MAC_NETWORK_TOPO_S stTopoInf;       //拓扑信息
    HI_U8 *pTopoIntegerInfo;      //一份完整拓扑信息，目前用于AFN=10-F21\F31\F32\F228
#if defined(PRODUCT_CFG_HUNAN)
	HI_MAC_NETWORK_MODULE_ID_S* pstModuleIdInf;
	HI_MAC_NETWORK_MODULE_ID_S* pstTempModuleIdInf;//临时模块ID信息，目前用于维护pstModualIdInf
#endif
    HI_MDM_NETWORK_STA_ID_S *pstStaIdInfo; //一份完整节点ID信息，目前用于AFN=10-F33
	MR_CCO_NDM_PLC_INFO_STRU stCcoNdmPlcInfo; //抄控器下发ccoplc时的报文信息
    MR_CCO_NDM_PLC_BUSY_STRU stCcoNdmPlcBusyInfo; //有抄控器下发plc帧点抄引起的忙返回信息记录
    HI_U8 LowPowerMeterLastNum;//Matteo
    II_ARCHVICE *usIIArchive;//Matteo
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
#if defined(MRS_SRV_PROTOCOL_CJT188)
PLC_OVERTIME_INF_STRU *mrsGetPlcOverTimeInfSavedAfn13(HI_VOID);
#endif

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
HI_PUBLIC HI_U32 mrsCcoRxAfn13DataPro(HI_PVOID pPayload, HI_U16 usPayloadLen, HI_PVOID *ppData, HI_U16 *pusDataLen, HI_U8 ucMacAdd[HI_PLC_MAC_ADDR_LEN]);

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
HI_PUBLIC HI_U32 mrsCcoRxAfn14DataPro(HI_PVOID pPayload, HI_U16 usPayloadLen, HI_PVOID *ppData, HI_U16 *pusDataLen, HI_U8 ucMacAdd[HI_PLC_MAC_ADDR_LEN]);


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
HI_U8 mrsCcoGetStaPhase(HI_U8 *pMac);
HI_U8* mrsCcoGetSrcMac(HI_VOID);
HI_VOID mrsCcoSetCtxPhase(HI_U8 ucPhase);

// 启动 主节点地址检查流程
HI_VOID mrsCcoCheckMainAddrStart(MRS_CCO_SRV_CTX_STRU * pstCcoCtx);
// 处理 主节点地址改变
HI_VOID mrsCcoHandleMainAddrChange(MRS_CCO_SRV_CTX_STRU * pstCcoCtx);
// 处理 主节点地址检查超时
HI_VOID mrsCcoHandleCheckMainAddrTimeout(MRS_CCO_SRV_CTX_STRU * pstCcoCtx);

HI_VOID mrsCcoTopoStatusChgProc(HI_SYS_QUEUE_MSG_S* pstMsg);

HI_BOOL mrsCheckStaIsInTopoInf(HI_U8 aucMac[HI_PLC_MAC_ADDR_LEN]);
HI_U16 mrsCcoGetTeiByMac(HI_U8 *pMac);
HI_U32 mrsCcoGetMacByTei(HI_U16 usTei, HI_U8 *pucMac);

HI_MDM_SOFT_TF_MODE_EN mrsCcoGetSoftTfMode(HI_VOID);

HI_U32 mrsMsgSendPlcToNdm(HI_PBYTE pout_buffer, HI_U16 buffer_outlen);
void RfFreePLCData(MRS_PLC_FRAME_DATA_STRU *pstPlcDataDl);
HI_VOID RfFreeWaterMeterListNode(HI_VOID * pstNode, HI_VOID * pstCb);
HI_U32 mrsRfGetWaterMeterMAC(HI_U8 *pucDeviceAddr, HI_U8 *pucMacAddr);
HI_U32 mrsReportCcoUartRecvWaterMeterData(HI_VOID);
HI_U32 mrs3762_ReportAFN20_F3(HI_U8 *pData,HI_U16 usDataLen,MRS_SEND_CCO_PROC_RX_F pRxRespProc,MRS_SEND_CCO_PROC_F pTotalTimeOutProc);
HI_U32 mrsCcoSearchMeterGetWaterMeterlist(MRS_CCO_SRV_CTX_STRU *cco,HI_U8 ucFlag);
HI_U32 mrsCcoNdmPlcBusyAdd(AFN_FN_UL_PARAM * pst_afn_param, HI_U8 anytime_from);
HI_VOID mrsCcoNdmPlcTimeout(HI_VOID);
HI_VOID mrsCcoNdmPlcBusyProc(HI_VOID);

hi_bool mrs_check_area_code(hi_u8 area_code);
//Matteo
HI_U32 mrsCcoSaveIICltDeviceType( MRS_CCO_SRV_CTX_STRU  * cco,HI_PBYTE pMeterAddr,HI_U8 Type);


HI_END_HEADER
#endif //__MRS_SERVICE_H__

