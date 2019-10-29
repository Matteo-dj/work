//*****************************************************************************
//
//                  版权所有 (C), 1998-2011, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : dfx_app.h
//  版 本 号   : V1.0
//  作    者   : shenhankun/00130424
//  生成日期   : 2011-11-10
//  功能描述   : APP的HSO包ID和对应参数结构定义
//
//  函数列表   : 无
//  修改历史   :
//  1.日    期 : 2011-11-10
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


#ifndef __DFX_APP_H__
#define __DFX_APP_H__

#include <hi_ft_nv.h>
#include <app_nv.h>
#include <hi_mdm.h>
#include <dfx_sys.h>
#include <dfx_sal.h>
#ifndef PRODUCT_CFG_HSO
#include <hi_eqt_pc_ndm.h>
#endif

#include <hi_cmn_nv.h>


//////////////////////////////////////////////////////////////////////////
// 命令ID定义
//////////////////////////////////////////////////////////////////////////


#define ID_DIAG_CMD_BATTERY_INFO                            (0x3409)//与HI_ND_DEVICE_INFO相同
#define ID_DIAG_CMD_BATTERY_STAT                            (0x3552)//与HI_DIAG_DRV_BQ_STAT_MSG相同

#define ID_DIAG_CMD_NDM_SD_DC_QUERY_STAT                    (0x3553) // HSO命令行查询命令
#define ID_DIAG_CMD_NDM_SD_DC_SET_CONFIG                    (0x3554) // HSO->调试控制台->数据采集->存储采集: 设置 "sd_dc_set"
#define ID_DIAG_CMD_NDM_SD_DC_QUERY_CONFIG                  (0x3555) // HSO->调试控制台->数据采集->存储采集: 查询 "sd_dc_query"


// 模拟或是Stub的包ID定义
#define ID_DIAG_CMD_CSIM_PACKET                              (0x5700) // 集中器模拟包
#define ID_DIAG_CMD_MSIM_PACKET                              (0x5701) // 电表模拟包
#define ID_DIAG_CMD_SIMU_CTRL                                (0x5710) // 模拟包控制
#define ID_DIAG_CMD_SIMU_PACKET_ECHO                         (0x5720) // 模拟包回显


// 命令查询
#define ID_DIAG_CMD_MRS_BEGIN                                (0x5800)
#define ID_DIAG_CMD_MRS_CHL_STATUS_QRY                       (0x5800)
#define ID_DIAG_CMD_MRS_SET_USER_INFO                        (0x5810)
#define ID_DIAG_CMD_MRS_REFRESH_NV_FILE                      (0x5811)

#define ID_DIAG_CMD_MRS_READ_MAC_CHIP_ID					 (0x5812)//读取芯片ID
#define ID_DIAG_CMD_MRS_WIRTE_ID							 (0x5813)//写入芯片ID

// CCO相关状态查询
#define ID_DIAG_CMD_MRS_CCO_QRY_QRY                          (0x5820)
#define ID_DIAG_CMD_MRS_CCO_PLC_STATUS                       (0x5821)  // 获取PLC通道状态
#define ID_DIAG_CMD_MRS_CCO_GET_MACMAP                       (0x5822)  // 获取当前Mac地址映射表
#define ID_DIAG_CMD_MRS_CCO_GET_METER_INF                    (0x5823)  // 获取当前电表信息(表库)
#define ID_DIAG_CMD_MRS_CCO_GET_CURR_NODE                    (0x5824)  // 获取当前抄表处理的节点及相关信息
#define ID_DIAG_CMD_MRS_CCO_INFO_QRY                         (0x5825)  // 查询读表应用相关信息(映射表状态、暂停状态) DIAG_CMD_MRS_CCO_QRY_STRU
#define ID_DIAG_CMD_MRS_CCO_GET_COLLECTOR_MAP                (0x5826)  // 查询采集器映射信息
#define ID_DIAG_CMD_MRS_GET_SYS_TIME                         (0x5827)  // 查询系统时钟
/* BEGIN: PN:  MODIFY\ADD\DEL by cuichunyan/00276962 at 2015/11/11 */
#define ID_DIAG_CMD_MRS_CCO_GET_PARAM                        (0x5828)  //查询CCO表档案存储生效时间
/* END: PN:  MODIFY\ADD\DEL by cuichunyan/00276962 at 2015/11/11 */
#define ID_DIAG_CMD_MRS_GET_CLT_SEARCH_ABNORMAL              (0x584A)  // 查询采集器搜表异常信息

/* BEGIN: PN: DTS2015033100426 MODIFY\ADD\DEL by cuiate/00233580 at 2015/3/26 */
#define ID_DIAG_CMD_MRS_CCO_GET_MR_RETRY_CFG                 (0x582A)
#define ID_DIAG_CMD_MRS_CCO_SET_MR_RETRY_CFG                 (0x582B)
#define ID_DIAG_CMD_MRS_CCO_MR_RETRY_CFG_DFX_INF             (0x582C)
/* END:   PN: DTS2015033100426 MODIFY\ADD\DEL by cuiate/00233580 at 2015/3/26 */
#define ID_DIAG_CMD_MRS_HSO_SET_CCO_MODE                     (0x582D)  // 设置运行模式

#define ID_DIAG_CMD_MRS_GET_CCO_SEARCH_DFX                   (0x584B)  // 获取全网搜表维测信息
#define ID_DIAG_CMD_MRS_GET_CCO_SLAVENODE_INFO               (0x584C)  // 获取全网搜表结果(从节点信息)
#define ID_DIAG_CMD_MRS_CCO_SEARCH_INFO_EXP                  (0x584D)  // 获取全网搜表维测信息, 扩展

#define ID_DIAG_CMD_MRS_CCO_GET_POWEROFF_INFO               (0x5831)  // 获取停电检测维测信息
#define ID_DIAG_CMD_MRS_CCO_GET_STA_POWEROFF_INFO           (0x5832)  // 获取站点停电检测维测信息
#define ID_DIAG_CMD_MRS_CCO_GET_POWEROFF_HISTORY            (0x5833)  // 获取停电检测维测历史记录
#define ID_DIAG_CMD_MRS_CCO_GET_POWEROFFING_INFO            (0x5834)  // 获取正在停电流程中的站点信息
#define ID_DIAG_CMD_MRS_CCO_GET_NDMPLC_INFO                 (0x5836)  // 获取抄控器plc最近一次报文信息

// STA相关查询
#if defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG)
#define ID_DIAG_CMD_MRS_DAYFREEZE_REC                        (0x5830)  // 查询冻结统计量
#endif

#define ID_DIAG_CMD_MRS_GET_STA_POWEROFF_INFO               (0x5835)  // 获取站点停电信息

#define ID_DIAG_CMD_MRS_GET_CLT_SEARCH_RESULT                (0x5840)  // 查询采集器搜表结果
#define ID_DIAG_CMD_MRS_GET_CLT_SEARCH_STATS                 (0x5841)  // 查询采集器搜表统计信息
#define ID_DIAG_CMD_MRS_GET_CLT_SEARCH_LOCAL                 (0x5842)  // 查询采集器本地搜表信息
#define ID_DIAG_CMD_MRS_GET_CLT_SEARCH_FRAME                 (0x5843)  // 查询采集器搜表报文统计信息
/* BEGIN: PN: DTS2015051601564 MODIFY\ADD\DEL by cuiate/00233580 at 2015/5/11 */
#define ID_DIAG_CMD_MRS_GET_CLT_SEARCH_LOCAL_EX              (0x5844)  // 查询采集器本地搜表信息(扩展)
/* END:   PN: DTS2015051601564 MODIFY\ADD\DEL by cuiate/00233580 at 2015/5/11 */

#define ID_DIAG_CMD_MRS_GET_CLTII_EVENT_STATUS               (0x5845)
#define ID_DIAG_CMD_MRS_GET_CLTII_EVENT_INF                  (0x5846)
#define ID_DIAG_CMD_MRS_CCO_GET_CLT_MACMAP                   (0x5849)

#define ID_DIAG_CMD_MRS_GET_CLT_LOCAL_MAP                   (0x5848)  // 查询STA本地映射表
#define ID_DIAG_CMD_MRS_GET_USER_VER                        (0x5850)
/* BEGIN: Modified by cuichunyan/c00276962 2016/11/10  问题单号:cltii R485 MultiConnect */
#define ID_DIAG_CMD_MRS_GET_RS485_MULTI_CONNECT        		(0x5851)  //查询II采485混接
#define ID_DIAG_CMD_MRS_CLR_RS485_MULTI_CONNECT        		(0x5852)  //清除II采485混接
/* END  : Modified by cuichunyan/c00276962 2016/11/10  问题单号:cltii R485 MultiConnect */

#define HI_ND_NDM_CMD_PACKET				(0x5853)  // 抄控宝-CCO控制命令
#define ID_DIAG_CMD_MRS_END                                  (0x5900)


#define ID_DIAG_CMD_UPG_DEBUG_QUERY_DETAIL                   (0x5A00)  // DIAG_CMD_UPG_DEBUG_QUERY_DETAIL_STRU
#define ID_DIAG_CMD_MDM_UPG_STATUS                           (0x5A01)
#define ID_DIAG_CMD_MDM_UPG_STATUS_STAT                      (0x5A03)  //全网升级统计值
#define ID_DIAG_CMD_MDM_UPG_STOP                             (0x5A02)

/* BEGIN:PN:主站升级  MODIFY\ADD  by chenli/00238131 at 2014/1/23 */
#define ID_DIAG_CMD_MDM_UPG_STATUS_BS                       (0x5A04)
#define ID_DIAG_CMD_OPEN_UPG_OPTION_FORCE                   (0x5A05)  //打开强制升级功能

//for test
#define ID_DIAG_CMD_MDM_UPG_FILE_CHECK                      (0x5A06)
#define ID_DIAG_CMD_GET_UPG_FORCE_FLAG                      (0x5A07)




typedef struct
{
    HI_U32 ulUpgId;
    HI_U32 ulBlkSize;
    HI_U32 ulFileSize;
    HI_U32 ulNum;
    HI_UPG_STATUS_IND_ENTRY_S entry[8];
}DIAG_CMD_UPG_DFX_STATUS_IND_STRU;

typedef struct
{
    HI_U8  blk_per_pkt;
    HI_U8  pad[2];
    HI_U8 mac[6];
    HI_U8 usUpgStatus;
    HI_U16 bitmap_cnt;
    HI_U8  bitmap[164]; /* HI_UPG_STATUS_BITMAP_LEN */
}DIAG_CMD_UPG_DEBUG_QUERY_DETAIL_STRU;

/* BEGIN:PN:主站升级  MODIFY\ADD  by chenli/00238131 at 2014/1/23 */
//0x5a03
typedef struct
{
    HI_U32 ulUpgId;         // 全网升级的标识
    HI_U16 usOkNum;         // 全网升级中通信模块升级成功的总个数，即升级文件传输结束，升级文件验证通过，且升级验证通过
    HI_U16 usCompleteNum;   // 全网升级中通信模块升级完成的总个数，即升级文件传输结束，升级文件验证通过。
    HI_U16 usFailNum;       // 全网升级中通信模块升级失败的总个数
    HI_U16 usProcessNum;    // 全网升级中通信模块正在升级的总个数，即升级文件正在传输过程中
    HI_U8  ucUpgStatus;     // 当前全网的升级状态
    HI_U8  pad[3];
}DIAG_CMD_UPG_DFX_STAT_STATUS_IND_STRU;

//0x5a04
typedef struct
{
    HI_U32 ulUpgId;         // 升级的标识
    HI_U8  ucUpgStatus;     // 当前升级状态
    HI_U8  ucUpgType;       // 当前升级类型
    HI_U8  aucReserved[2];
}DIAG_CMD_UPG_DFX_BS_STATUS_IND_STRU;
/* END:PN:主站升级  MODIFY\ADD  by chenli/00238131 at 2014/1/23 */

/* BEGIN:PN:DTS2014031206674 屏蔽强制升级功能 MODIFY\ADD  by chenli/00238131 at 2014/3/24 */
typedef struct
{
    HI_BOOL forceFlag;
    HI_U8 pad[3];
}DIAG_CMD_UPG_FORCE_FLAG;
/* END:PN:DTS2014031206674  屏蔽强制升级功能 MODIFY\ADD  by chenli/00238131 at 2014/3/24 */

//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

typedef struct
{
    HI_U32 ulDataSize;
    HI_U8  aucData[0];
} DIAG_CMD_CSIM_REQ_STRU;

typedef struct
{
    HI_U32 ulDataSize;
    HI_U8  aucData[0];
} DIAG_CMD_CSIM_ACK_STRU;


typedef struct
{
    HI_U32 ulDataSize;
    HI_U8  aucData[0];
} DIAG_CMD_MSIM_REQ_STRU;

typedef struct
{
    HI_U32 ulDataSize;
    HI_U8  aucData[0];
} DIAG_CMD_MSIM_ACK_STRU;


///////////////////////////////////////////////////////////////////////////////
// MRS 信息查询
///////////////////////////////////////////////////////////////////////////////

// MRS 通道相关信息查询
typedef struct
{
    HI_FTM_CHIP_TYPE_E enChipType;
    HI_FTM_PRODUCT_TYPE_E enProductType;
    HI_FTM_HW_PLATFORM_TYPE_E enHwPlatformType;
    HI_U32 ulPlcChlStatus;
    HI_SAL_DFX_HOST_CTRL_S stHostCtrlInfo;
    HI_NV_FTM_PRODUCT_ID_S stProductId;
    HI_U32 bEnable; // 对齐需要, 声明为 HI_U32, 如果需要增加变量，则可以调整该类型
} DIAG_CMD_MRS_CHL_QRY_STRU;

typedef enum
{
  EN_MRS_SYS_CALL_ERR_MALLOC_FAIL,
  EN_MRS_SYS_CALL_MAX
}MRS_SYSTEM_CALL_ERROR_ENUM;


typedef struct
{
    HI_U32 Index;           // 映射表中的序号
    HI_U8 NodeMac[6];       // 对应的Mac地址
    HI_U8 NodeMeter[6];     // 节点(电表)地址
    HI_U32 DevType;         // 设备类别 (电表模块; 采集器模块)
    HI_U32 usSendFrame;     // 当前节点(电表)发送645帧次数
    HI_U32 usRecvFrame;     // 当前节点(电表)接收645帧次数
} MRS_DFX_CCO_NODE_MAP_STRU;    // 获取当前映射表结构(单一)

#define MRS_DFX_MAC_MAP_MAX_PACK_NUM  14     // 单次传输Mac映射表最大个数
typedef struct
{
    HI_U16 TotalNum;
    HI_U16 CurrNum;
    MRS_DFX_CCO_NODE_MAP_STRU stNodeMap[MRS_DFX_MAC_MAP_MAX_PACK_NUM];
} MRS_DFX_CCO_NODE_MAP_INF_STRU;    // 获取当前映射表结构

typedef struct
{
    HI_U8  NodeMeter[6];    // 节点地址(表地址)
    HI_U16 Index;           // 序号
    HI_U16 usType;          // 规约类型
    HI_U16 usFlg;           // 抄读标志(低字节有效)
} MRS_DFX_CCO_NODE_METER_STRU;

#define MRS_DFX_METER_MAX_PACK_NUM  32 //单次传送表档案结构
#define MRS_DFX_CLT_METER_MAX       32 //采集器下挂电表数量上限
#define MRS_MAX_METERNUM            (MRS_DFX_CLT_METER_MAX)
typedef struct
{
    HI_U16 TotalNum;
    HI_U16 CurrNum;
    MRS_DFX_CCO_NODE_METER_STRU stNodeMap[MRS_DFX_METER_MAX_PACK_NUM];
} MRS_DFX_CCO_METERS_STRU;    // 表档案结构

// MRS 相关信息查询
typedef struct
{
    HI_U8 mrs_route_status;             // 读表路由控制状态(0:正常; 1:重启; 2:暂停; 3:恢复)
    HI_U8 map_status;                   // 映射表状态(0:未就绪; 1:已就绪)
    HI_U8 reserved[2];
    HI_U16 meter_num;                   // 表档案中节点数量
    HI_U16 map_num;                     // 当前映射表数量
} DIAG_CMD_MRS_CCO_QRY_STRU;

// II采映射关系查询
typedef struct
{
    HI_U32  sn;
    HI_U8   mac[HI_PLC_MAC_ADDR_LEN];
    HI_U16  reserved;
} DIAG_CMD_MRS_CCO_COLLECTOR_MAP_REQ_STRU;

typedef struct
{
    HI_U8   meter_num;
    HI_U8   reserved;
    HI_U8   mac[HI_PLC_MAC_ADDR_LEN];   // II采入网的MAC地址
    HI_U8   asset[HI_METER_ADDR_LEN];   // II采资产编号
    HI_U8   id[HI_PLC_MAC_ADDR_LEN];    // II采ID号，目前使用II采默认的MAC地址
    HI_U8   payload[0];
} MRS_DFX_CCO_COLLECTOR_MAP_ITEM_STRU;

typedef struct
{
    HI_U32  sn;
    HI_U16  num;
    HI_U16  total;
    HI_U8   payload[0];
} DIAG_CMD_MRS_CCO_COLLECTOR_MAP_IND_STRU;


/* BEGIN: PN:  MODIFY\ADD\DEL by cuichunyan/00276962 at 2015/11/11 */
typedef struct
{
    HI_U8 ucFlashSaveDelay;     // 存盘延迟时间，单位: 秒
    HI_U8 reserved[15];         // 默认值0
} DIAG_CMD_MRS_CCO_GET_PARAM_INF_STRU;
/* END: PN:  MODIFY\ADD\DEL by cuichunyan/00276962 at 2015/11/11 */

/* BEGIN: Added by xiangchaobin/00378718, 2017/06/26   PN: poweroff/on check report */
// 获取全网停电汇总维测信息
typedef struct
{
    HI_U32  ulRet;
    HI_U32  ulGetPoweroffInfoSeq;       // 获取停电信息报文序号

    HI_U16  usMacReportOffSum;          // 收到MAC层停电上报总次数
    HI_U16  usMacReportOnSum;           // 收到MAC层来电上报总次数
    HI_U16  usDetectSuccessSum;         // 探测成功总次数
    HI_U16  usInterceptSum;             // 探测失败被停电记录拦截总次数

    HI_U16  us3762ReportOffSum;         // 上报集中器离线次数
    HI_U16  us3762ReportOnSum;          // 上报集中器上线次数
    HI_U16  usRecheckFailSum;           // 复核失败总次数
    HI_U16  usMacReportCorrectSum;      // MAC正确上报总次数

    HI_U16  usMacReportWrongSum;        // MAC误报总次数
    HI_U16  usMacReportMissSum;         // MAC漏报总次数
    HI_U16  usDetectReportCorrectSum;   // 第三层过滤前，正确上报总次数
    HI_U16  usDetectReportWrongSum;     // 第三层过滤前，误报总次数

    HI_U16  usDetectReportMissSum;      // 第三层过滤前，漏报总次数
    HI_U16  us3762ReportCorrectSum;     // 给集中器，正确上报总次数
    HI_U16  us3762ReportWrongSum;       // 给集中器，误报总次数
    HI_U16  us3762ReportMissSum;        // 给集中器，漏报总次数

    HI_BOOL bDetectComplete;            // 是否探测完成
    HI_BOOL bRecheckComplete;           // 是否复核完成
    HI_U16  usDetectingNum;             // 正在探测的站点个数
    HI_U16  usRecheckingNum;            // 正在复核的站点个数
    HI_U16  usDetectIdx;                // 当前探测到哪一个节点

    HI_U16  usRecheckIdx;               // 当前复核到哪一个节点
    HI_U16  pad[7];
} DIAG_CMD_MRS_CCO_POWEROFF_INFO_IND_STRU;

// 获取站点停电维测信息，参数结构
typedef struct
{
    HI_U8  mac[HI_PLC_MAC_ADDR_LEN];
    HI_U16  pad;
} DIAG_CMD_MRS_STA_POWEROFF_INFO_REQ_STRU;

// 获取站点停电维测信息，应答结构
typedef struct
{
    HI_U32  ulRet;

    HI_U8   ucStatus;                   // 停电检测状态，参考MRS_POWEROFF_STATUS_E
    HI_U8   ucReCheckStatus;            // 停电复核状态，参考MRS_POWERON_RECHECK_STATUS_E
    HI_U8   ucDetectQueryCnt;           // 探测尝试次数
    HI_U8   ucRecheckQueryCnt;          // 复核尝试次数

    HI_U8   ucReportCheckScore;         // 上报情况复核积分
    HI_U8   ucRecheckFailCnt;           // 复核失败次数
    HI_U16  usMacReportOffCnt;          // 收到MAC层上报离线次数

    HI_U16  usMacReportOnCnt;           // 收到MAC层上报上线次数
    HI_BOOL bPassedDetect;              // 本周期是否有通过过停电探测
    HI_U8   ucMacReportWrongCnt;        // MAC上报的，误报次数

    HI_U8   ucMacReportMissCnt;         // MAC上报的，漏报次数
    HI_U8   ucMacReportCorrectCnt;      // MAC上报的，正确次数
    HI_U8   ucDetectReportWrongCnt;     // 探测后上报，误报次数
    HI_U8   ucDetectReportMissCnt;      // 探测后上报，漏报次数

    HI_U8   ucDetectReportCorrectCnt;   // 探测后上报，正确次数
    HI_U8   uc3762ReportWrongCnt;       // 上报集中器后的，误报次数
    HI_U8   uc3762ReportMissCnt;        // 上报集中器后的，漏报次数
    HI_U8   uc3762ReportCorrectCnt;     // 上报集中器后的，正确次数

    HI_U32  ulLastMacReportOffTime;     // 最近一次MAC层上报离线的时间
    HI_U32  ulLastMacReportOnTime;      // 最近一次MAC层上报上线的时间
    HI_U32  ulLastReportOffTime;        // 最近一次上报停电到集中器的时间
    HI_U32  ulLastReportOnTime;         // 最近一次上报来电到集中器的时间
    HI_U32  ulMeterLastPoweroffTime;    // 电表最近一次停电RTC时间
    HI_U32  ulMeterLastPoweronTime;     // 电表最近一次上电RTC时间

    HI_SYS_CALENDAR_TIME_S stLastMacReportOffTime;
    HI_SYS_CALENDAR_TIME_S stLastMacReportOnTime;
    HI_SYS_CALENDAR_TIME_S stLastReportOffTime;
    HI_SYS_CALENDAR_TIME_S stLastReportOnTime;
    HI_SYS_CALENDAR_TIME_S stMeterLastPoweroffTime;
    HI_SYS_CALENDAR_TIME_S stMeterLastPoweronTime;

    HI_U32  ulLastMacReportWrongTime;   // 上一次MAC层误报的时间，CCO上电时间
    HI_U32  pad[12];
} DIAG_CMD_MRS_STA_POWEROFF_INFO_IND_STRU;

#define DIAG_CMD_MRS_GET_POWEROFF_HISTORY_NUM   (10)    // 一次读取历史记录的个数

// 读取停电上报历史记录请求
typedef struct
{
    HI_U8 begin_index;
    HI_U8 pad[3];
} DIAG_CMD_MRS_POWEROFF_HISTORY_REQ_STRU;

// 历史记录节点结构 24B
typedef struct
{
    HI_U8  ucStatus; // 停电检测状态，参考MRS_POWEROFF_STATUS_E
    HI_U8  pad;
    HI_U8  aucMac[HI_PLC_MAC_ADDR_LEN];

    HI_U32 ulMacReportOffTime;      // MAC上报离线的时间
    HI_U32 ulMacReportOnTime;       // MAC上报上线的时间
    HI_U32 ulMeterLastPoweroffTime; // 电表最近一次停电时间
    HI_U32 ulMeterLastPoweronTime;  // 电表最近一次上电时间
} DIAG_CMD_MRS_POWEROFF_HISTORY_NODE_STRU;

// 读取停电上报历史记录回复
typedef struct
{
    HI_U32 ulRet;

    HI_U8  ucIndex;
    HI_U8  pad[3];

    DIAG_CMD_MRS_POWEROFF_HISTORY_NODE_STRU astRecords[DIAG_CMD_MRS_GET_POWEROFF_HISTORY_NUM];
} DIAG_CMD_MRS_POWEROFF_HISTORY_IND_STRU;

// 获取停电流程中信息请求
typedef struct
{
    HI_U8  ucDetectStatus;
    HI_U8  ucRecheckStatus;
    HI_U16 usBeginIndex;
} DIAG_CMD_MRS_POWEROFFING_INFO_REQ_STRU;

#define DIAG_POWEROFFING_QUERY_NUM  (20)
#define DOAG_POWEROFF_NODE_MAC_LEN  (8)

typedef struct
{
    HI_U8 aucMac[DOAG_POWEROFF_NODE_MAC_LEN];
} DIAG_POWEROFFING_NODE_STRU;

// 获取停电流程中信息回复
typedef struct
{
    HI_U32 ulRet;

    DIAG_POWEROFFING_NODE_STRU astMacList[DIAG_POWEROFFING_QUERY_NUM];
} DIAG_CMD_MRS_POWEROFFING_INFO_IND_STRU;

// STA读取停电信息回复
typedef struct
{
    HI_U32 ulRet;

    HI_U32 ulLastPoweroffTimeSec;
    HI_U32 ulLastPoweronTimeSec;

    HI_SYS_CALENDAR_TIME_S stLastPoweroffTime;
    HI_SYS_CALENDAR_TIME_S stLastPoweronTime;

    HI_U16 usRcvRecheckCmdCnt;      // 收到复核报文次数
    HI_U16 usRcvRecheckCmdForMeCnt; // 收到给我的复核报文次数
    HI_U16 usRcvDetectCmdCnt;       // 收到探测报文次数
    HI_U16 usRcvDetectCmdForMeCnt;  // 收到给我的探测报文次数

    HI_U16 ucSendCmdFailCnt;        // 报文发送失败次数
    HI_U16 pad;
    HI_U32 ulSendCmdErrCode;        // 报文发送返回码

    HI_U32 ulLastReadPowerInfoTime; // 上一次从电表读取停电信息时间
    HI_U32 ulStopWhere;

    HI_U32 pad1[9];
} DIAG_CMD_MRS_STA_GET_POWEROFF_INFO_IND_STRU;
/* END:   Added by xiangchaobin/00378718, 2017/06/26 */


#define DIAG_CMD_MRS_MODE_NORMAL				(0x00)
#define DIAG_CMD_MRS_MODE_ONEKEY_DIAGNOSTIC	    (0x01)

typedef struct
{
	HI_U8 ucMode;
	HI_U8 reserved[3];
} DIAG_CMD_MRS_HSO_SET_MODE_REQ_S;

typedef struct
{
    HI_U32 ulRet;
} DIAG_CMD_MRS_HSO_SET_MODE_IND_S;

#if defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG)
#define DAYFREEZE_MAX_PACK_NUM 8

typedef struct
{
    HI_U32 dayIndex;
} DIAG_CMD_MRS_DAYFREEZE_REC_REQ_STRU;

typedef struct
{
    HI_U32 sysTime;
    HI_U8 meter[3];
    HI_U8 causeSet;
} MRS_SRV_MRSLOG_DAYFREEZE_ITEM;

typedef struct
{
    HI_U16 currDayIndex;     // 当前日索引
    HI_U16 currIndex;        // 本次传输的索引
    HI_U16 arrayIndex;       // 数组索引
    HI_U8 meterCount;        // 本日电表数量
    HI_U8 Count;             // 本次电表数量
    MRS_SRV_MRSLOG_DAYFREEZE_ITEM item[DAYFREEZE_MAX_PACK_NUM];
} DIAG_CMD_MRS_DAYFREEZE_REC_IND_STRU;// 查询STA端日冻结统计量

#endif


typedef struct
{
    HI_S32 tmYear;     // year
    HI_S32 tmMon;      // month
    HI_S32 tmMday;     // day of the month
    HI_S32 tmHour;     // hours
    HI_S32 tmMin;      // minutes
    HI_S32 tmSec;      // seconds
    HI_S32 tmWday;     // day of the week
    HI_S32 tmYday;     // day of the year
} MRS_SYS_TIME_S;

typedef struct
{
    HI_U32 second;      // seconds
    HI_U32 timeStamp;   // millisecond
    MRS_SYS_TIME_S tmTime;
} DIAG_CMD_MRS_GET_SYS_TIME_IND_STRU;


// 采集器可维可测命令，搜表结果
#define DIAG_CMD_MRS_SEARCH_METER_NUM   (10)

typedef struct
{
    HI_U8 ucAddr[HI_METER_ADDR_LEN];    // 电表地址
    HI_U8 ucProtocol;                   // 规约类型
    HI_U8 ucBaudrate;                   // 波特率/100(12:1200 24:2400 48:4800 96:9600)
} MRS_METER_ITEM;

// 本地搜表停止的原因，正常结束、停止命令
#define MRS_SEARCH_END_REASON_DEFAULT   (0)
#define MRS_SEARCH_END_REASON_FINISH    (1)
#define MRS_SEARCH_END_REASON_STOP      (2)

typedef struct
{
    HI_U8 ucTotal;
    HI_U8 ucCurrent;
    HI_U8 ucSearchStatus;
    HI_U8 ucEndReason;
    MRS_METER_ITEM stMeter[DIAG_CMD_MRS_SEARCH_METER_NUM];
} DIAG_CMD_MRS_GET_CLT_SEARCH_RESULT_IND_STRU;


// 采集器可维可测命令，搜表统计信息
typedef struct
{
    HI_U32 ulSearchSN;
    HI_U8  ucTotalMeter;
    HI_U8  ucSearchStatus;
    HI_U16 usPlcStartRx;
    HI_U32 usPlcStartRxLTS;
    HI_U16 usPlcQueryRx;
    HI_U16 usPlcQueryTx;
    HI_U32 ulPlcQueryRxLTS;
    HI_U32 ulPlcQueryTxLTS;
    HI_U16 usComStartRx;
    HI_U16 usComStartTx;
    HI_U32 ulComStartRxLTS;
    HI_U32 ulComStartTxLTS;
    HI_U16 usComQueryRx;
    HI_U16 usComQueryTx;
    HI_U32 ulComQueryRxLTS;
    HI_U32 ulComQueryTxLTS;
    HI_U16 usBaudrate;
    HI_U8  ucComSetMacAddr[HI_PLC_MAC_ADDR_LEN];
    HI_U32 ulComFirstQueryLTS;
    HI_U32 ulComQueryResultLTS;
    HI_U32 ulComSetMacAddrLTS;
}DIAG_CMD_MRS_GET_CLT_SEARCH_STATS_IND_STRU;


// 采集器可维可测命令，搜表本地统计
typedef struct
{
    HI_U16 usSearch;
    HI_U16 usValid;
    HI_U16 usInvalidLen1;
    HI_U16 usInvalidLen2;
    HI_U16 usInvalidLen3;
    HI_U8  ucMaxDepth;
    HI_U8  ucAbnormal;
} MRS_CLT_SEARCH_LOCAL_STATS;

typedef struct
{
    HI_U32 ulStartTime;
    HI_U32 ulEndTime;
    HI_U8  ucSearchStatus;
    HI_U8  ucProtocol;
    HI_U16 usBaudrate;
    HI_U32 ulSearchDI;
    HI_U8  ucSearchAddr[HI_METER_ADDR_LEN];
    HI_U8  ucMeter2Mac[HI_METER_ADDR_LEN];
    HI_U32 ulMeter2MacLTS;
    MRS_CLT_SEARCH_LOCAL_STATS stStartup;
    MRS_CLT_SEARCH_LOCAL_STATS stProtocol07;
    MRS_CLT_SEARCH_LOCAL_STATS stProtocol97a;
    MRS_CLT_SEARCH_LOCAL_STATS stProtocol97b;
} DIAG_CMD_MRS_GET_CLT_SEARCH_LOCAL_IND_STRU;

/* BEGIN: PN: DTS2015051601564 MODIFY\ADD\DEL by cuiate/00233580 at 2015/5/11 */
typedef struct
{
    MRS_CLT_SEARCH_LOCAL_STATS stProtocol97c;
    MRS_CLT_SEARCH_LOCAL_STATS stProtocol698;
    HI_U8 aucReserved[40];
} DIAG_CMD_MRS_GET_CLT_SEARCH_LOCAL_EX_IND_STRU;
/* END:   PN: DTS2015051601564 MODIFY\ADD\DEL by cuiate/00233580 at 2015/5/11 */

typedef struct
{
    HI_U8 ucStatus;             // 监控状态：0：空闲；1：正在监控；2：正在上报；其它：无效
    HI_U8 ucLastMeterProtocol;  // 最近一次监控电表的规约类型
    // 最近一次监控电表的地址
    HI_U8 aucLastMeterAddr[HI_METER_ADDR_LEN];
    HI_U32 ulLastStartTS;       // 最近一次监控开始时间
    HI_U32 ulLastEndTS;         // 最近一次监控结束时间(如果正在监控，该值为当前时间)
    HI_U32 ulEventTotal;        // 上电以来发生事件的总次数
    HI_U8 ucLastEventCount;     // 最近一次监控，监控到的事件数量
    HI_U8 ucLastEventReported;  // 最近一次监控到的事件中已上报的事件数量
    HI_U8 ucLastEventCleared;  // 最近一次监控到的事件中已上报的事件数量
    HI_U8 aucReserved[41];
} DIAG_CMD_MRS_GET_CLTII_EVENT_STATUS_STRU;


#define DIAG_CMD_MRS_CLT_EVENT_REPORT_MAX	(10)

typedef struct
{
    HI_U8 aucMeterAddr[HI_METER_ADDR_LEN];  // 电表地址
    HI_U8 ucProtocol;   // 电表规约类型；1-97规约，2-07规约，其它-无效
    HI_U8 ucEventFlag;  // 事件标识
                        // D0: 事件1 0-无 1-有
                        // D1: 事件2 0-无 1-有
                        // D2: 事件3 0-无 1-有
                        // D3: 事件4 0-无 1-有
                        // D4~D7: 0000b
} DIAG_CMD_MRS_CLTII_EVENT_STRU;

typedef struct
{
    HI_U8 ucTotal;      // 有事件电表总数
    HI_U8 ucCurrent;    // 本帧报文中包含的电表数
    HI_U8 aucPadding[2];
    DIAG_CMD_MRS_CLTII_EVENT_STRU astEvent[DIAG_CMD_MRS_CLT_EVENT_REPORT_MAX];
} DIAG_CMD_MRS_GET_CLTII_EVENT_INF_STRU;


// 采集器可维可测命令，搜表报文
#define MRS_DFX_SEARCH_FRAME_MAX_LEN            (56)
#define MRS_DFX_SEARCH_FRAME_CURRENT            (5)

// 结构体大小20+56=76 B
typedef struct
{
    HI_U32 ulDI;
    HI_U8  ucAddr[HI_METER_ADDR_LEN];
    HI_U8  ucBaudrate;
    HI_U8  ucTimeoutCnt;
    HI_U16 usFrameStats;
    HI_U16 usFrameHeadTS;
    HI_U16 usFrameTailTS;
    HI_U8  ucPad;
    HI_U8  ucLen;
    HI_U8  ucFrame[MRS_DFX_SEARCH_FRAME_MAX_LEN];
} MRS_DFX_SEARCH_FRAME_STRU;

// 结构体大小4+76*5=384 B
typedef struct
{
    HI_U8 ucReportTotal;         // 本次上报的总数，最大150
    HI_U8 ucCurrent;             // 本数据包包含的总数
    HI_U8 ucStartIndex;          // 本数据包第1条报文在ucReportTotal个里的下标
    HI_U8 ucTotal;               // II采搜表交互报文的总数，可能大于150
    MRS_DFX_SEARCH_FRAME_STRU stFrame[MRS_DFX_SEARCH_FRAME_CURRENT];
} DIAG_CMD_MRS_GET_CLT_SEARCH_FRAME_IND_STRU;


#define DIAG_CMD_MRS_CLT_LOCAL_MAP_NUM   (10)

typedef struct
{
    HI_U8 aucMeterAddr[HI_METER_ADDR_LEN];
    HI_U8 ucProto;
    HI_U8 ucPad;
} MRS_DFX_CLT_LOCAL_MAP_ENTRY;


typedef struct
{
    HI_U8 ucTotal;
    HI_U8 ucCurrent;
    HI_U8 ucStartIndex;
    HI_U8 ucPad;
    MRS_DFX_CLT_LOCAL_MAP_ENTRY astMapEntry[DIAG_CMD_MRS_CLT_LOCAL_MAP_NUM];
} DIAG_CMD_MRS_GET_CLT_LOCAL_MAP_IND_STRU;


// CCO端可维可测命令: II采异常信息
#define DIAG_CMD_SEARCH_ABNORMAL_CNT    (10)

typedef struct
{
    HI_U8 ucDevId[HI_PLC_MAC_ADDR_LEN];
    HI_U8 ucAbnormal;
    HI_U8 ucPad;
} MRS_DFX_ABNORMAL_CLTII_STRU;

typedef struct
{
    HI_U16 usTotal;
    HI_U16 usCurrent;
    HI_U16 usStartIndex;
    HI_U16 usPad;
    MRS_DFX_ABNORMAL_CLTII_STRU astAbnormal[DIAG_CMD_SEARCH_ABNORMAL_CNT];
} DIAG_CMD_MRS_GET_CLT_SEARCH_ABNORMAL_IND_STRU;


// 采集器可维可测，II采搜表交互报文存储
#define MRS_DFX_SEARCH_FRAME_MAX    (150)
#define MRS_DFX_SEARCH_BUF_SIZE     (40 * MRS_DFX_SEARCH_FRAME_MAX)

typedef struct
{
    HI_U32 ulDI;
    HI_U8  ucAddr[HI_METER_ADDR_LEN];
    HI_U8  ucBaudrate;
    HI_U8  ucFrameLen;
    HI_U16 usFrameOffset;
    HI_U16 usFrameStats;
    HI_U32 ulFrameHeadTS:12;
    HI_U32 ulFrameTailTS:12;
    HI_U32 ulByteIntervalTimeoutCnt:8;
} MRS_DFX_SEARCH_FRAME_HEAD;

typedef struct
{
    HI_U16 usFrameTotal;
    HI_U16 usBufTail;
    MRS_DFX_SEARCH_FRAME_HEAD stHead[MRS_DFX_SEARCH_FRAME_MAX];
    HI_U8 ucBuf[MRS_DFX_SEARCH_BUF_SIZE];
} MRS_DFX_SEARCH_FRAME_STATS_STRU;


// CCO端: 全网搜表可维可测信息上报结构
#define MRS_DFX_CCO_SEARCH_INFO_IND_VER0   0

#define MRS_CCO_STATUS_SEARCH_MODE_NORMAL       (0)
#define MRS_CCO_STATUS_SEARCH_MODE_TRANSFORMER  (1)

typedef struct
{
    HI_U8  ucInterfaceVer;                       // 接口的版本号
    HI_U8  ucStruSize;                           // 本结构的大小
    HI_U8  ucSearchStatus;                       // 搜表状态
/* BEGIN: PN: DTS2015061008265 MODIFY\ADD\DEL by cuiate/00233580 at 2015/6/23 */
    HI_U8  ucOption;                             // [0:3]:0-普通模式,1-台区识别模式,其它-保留; [4:7]:保留
/* END:   PN: DTS2015061008265 MODIFY\ADD\DEL by cuiate/00233580 at 2015/6/23 */
    HI_U32 ulSearchDuration;                     // 搜表耗时，单位: 毫秒
    HI_U16 usUserDefinedDuration;                // 用户设定的搜表时长，单位: 分
    HI_U16 usTotal;                              // CCO已保存的站点从节点信息的总条数
    HI_U16 usMeterEntryNum;                      // CCO已保存的载波电表站点从节点信息总条数
    HI_U16 usCltEntryNum;                        // CCO已保存的采集器站点从节点信息总条数
} DIAG_CMD_MRS_GET_CCO_SEARCH_INFO_IND_STRU;


// CCO端: 所有从节点信息(全网搜表结果)查询和上报结构
#define MRS_DFX_CCO_SLAVENODE_INFO_REQ_VER0    0
#define MRS_DFX_CCO_SLAVENODE_INFO_IND_VER0    0

#define DIAG_MRS_REQ_OPTION_ALL                (0)
#define DIAG_MRS_REQ_OPTION_METER              (1)
#define DIAG_MRS_REQ_OPTION_COLLECTOR          (2)

#define MRS_DFX_COUNT_REQ_ALL                  (HI_U16)(-1)

#define DIAG_MRS_DEVICE_TYPE_COLLECTOR         (0)
#define DIAG_MRS_DEVICE_TYPE_METER             (1)

typedef struct
{
    HI_U8  ucInterfaceVer;      // 接口的版本号
    HI_U8  ucStruSize;          // 本结构的大小
    HI_U8  ucSearchStatus;      // 搜表状态
    HI_U8  ucOption;            // [0:3]:0-普通模式,1-台区识别模式; [4:7]:保留

    HI_U16 usMacListNum;        // 在MAC列表中的节点数目
    HI_U16 usUnlockStaNum;      // 已查到搜表结果，待锁定节点数目
    HI_U16 usUnreportStaNum;    // 已锁定，待上报节点数目
    HI_U16 usReportedStaNum;    // 已上报节点数目

    HI_U8  bQueryComplete;      // 是否查询完毕
    HI_U8  bLockComplete;       // 是否锁定完毕
    HI_U16 pad;

    HI_U32 pad1[12];
} DIAG_CMD_CCO_SEARCH_INFO_EXP_IND;

typedef struct
{
    HI_U8  ucInterfaceVer;                       // 接口的版本号
    HI_U8  ucStruSize;                           // 本结构的大小
    HI_U8  ucSN;                                 // 本次查询的序号
    HI_U8  ucOption;                             // 查询选项: 0所有设备类型；1载波电表；2采集器
    HI_U16 usStart;                              // 查询的从节点信息起始编号(下标),从0开始
    HI_U16 usCount;                              // 查询的从节点信息条数
} DIAG_CMD_MRS_GET_CCO_SLAVENODE_INFO_REQ_STRU;

// 须保证DIAG数据包大小不超过384字节
typedef struct
{
    HI_U8  ucInterfaceVer;      // 接口的版本号
    HI_U8  ucStruSize;          // 接口结构的大小
    HI_U8  ucSN;                // 应答的查询序号
    HI_U8  ucDevType;           // 载波节点的类型, 00H为采集器；01H为电表
    HI_U16 usTotal;             // 上报的载波节点总数
    HI_U16 usStartIndex;        // 本帧上报的第1条从节点信息的是第几条
    HI_U8  ucCurrent;           // 本帧上报的载波节点数
    HI_U8  aucPad[3];           // 保留位
    HI_U8  aucPayload[0];       // 载波电表：DIAG_MRS_CARRIER_METER_STRU; 采集器: DIAG_MRS_CLT_STRU
} DIAG_CMD_MRS_GET_CCO_SLAVENODE_INFO_IND_STRU;

typedef struct
{
    HI_U8  aucMeterAddr[HI_METER_ADDR_LEN];  // 1376.2报文中从节点通信地址
    HI_U8  ucProtoType;                      // 1376.2报文中从节点通信协议类型
    HI_U8  ucPad;                            // 保留位
} DIAG_MRS_METER_STRU;

typedef struct
{
    HI_U16 usSeq;                            // 上报1376.2帧的从节点序号
    HI_U8  aucPad[2];                        // 保留位
    DIAG_MRS_METER_STRU stMeter;             // 电表信息
} DIAG_MRS_CARRIER_METER_STRU;

typedef struct
{
    HI_U16 usSeq;                            // 上报1376.2帧的从节点序号
    HI_U8  aucMac[HI_PLC_MAC_ADDR_LEN];      // 采集器入网MAC地址（实际）
    HI_U8  aucAsset[HI_METER_ADDR_LEN];      // 采集器资产号
    HI_U8  aucID[HI_PLC_MAC_ADDR_LEN];       // 采集器自身MAC
    HI_U8  ucTotal;                          // 该采集器下接的RS485电表总个数
    HI_U8  ucNum;                            // 本帧上报的该采集器下接RS485电表总个数
    HI_U8  aucPad[2];                        // 保留位
    HI_U8  aucPayload[0];                    // 存放RS485电表相关信息：DIAG_MRS_METER_STRU
} DIAG_MRS_CLT_STRU;


/* BEGIN: PN: DTS2015033100426 MODIFY\ADD\DEL by cuiate/00233580 at 2015/3/26 */
typedef enum
{
    DIAG_APP_MRS_TIMEOUT_MODE_DEFAULT = 0,
    DIAG_APP_MRS_TIMEOUT_MODE_FAST,
    DIAG_APP_MRS_TIMEOUT_MODE_USERDEFINE,
    DIAG_APP_MRS_TIMEOUT_MODE_MAX
} DIAG_APP_MRS_TIMEOUT_MODE_E;

// 请求下行结构
typedef struct
{
    HI_U32 ulSeq;
    HI_U32 ulReqMode;       // 要查询参数的模式
} DIAG_CMD_MRS_CCO_GET_MR_TIMEOUT_REQ_STRU;

// 请求上行结构
typedef struct
{
    HI_U32 ulSeq;
    HI_U32 ulReqMode;       // 要查询参数的模式
    HI_U32 ulCurMode;       // 当前抄表超时参数模式
    NV_MRS_RETRY_CFG_S astRetryCfg[MRS_RETRY_CFG_MAX_NUM];
} DIAG_CMD_MRS_CCO_GET_MR_TIMEOUT_IND_STRU;

// 设置下行结构
typedef struct
{
    HI_U32 ulSeq;
    HI_U32 ulMode;
    NV_MRS_RETRY_CFG_S astRetryCfg[MRS_RETRY_CFG_MAX_NUM];
} DIAG_CMD_MRS_CCO_SET_MR_TIMEOUT_REQ_STRU;

// 设置上行结构
typedef struct
{
    HI_U32 ulSeq;
    HI_U32 ulRet;
} DIAG_CMD_MRS_CCO_SET_MR_TIMEOUT_IND_STRU;


#define DIAG_CMD_MRS_CCO_MR_TIMEOUT_DFX_ITEM_MAX    (10)

typedef struct
{
    HI_U32 ulChangeTS;      // 变更时间戳
    HI_U8  ucReqMode;       // 切换到的模式
    HI_U8  ucCurMode;       // 当前模式
    HI_U8  ucRet;           // 切换结果
    HI_U8  ucSimuStatus;    // 模拟通道状态
} DIAG_CMD_MRS_CCO_MR_TIMEOUT_DFX_ITEM_STRU;

typedef struct
{
    HI_U32 ulChangeTimes;
    DIAG_CMD_MRS_CCO_MR_TIMEOUT_DFX_ITEM_STRU astItem[DIAG_CMD_MRS_CCO_MR_TIMEOUT_DFX_ITEM_MAX];
} DIAG_CMD_MRS_CCO_MR_TIMEOUT_DFX_IND_STRU;

/* END:   PN: DTS2015033100426 MODIFY\ADD\DEL by cuiate/00233580 at 2015/3/26 */

#define MRS_WHITELIST_CHANGE_ITEM_MAX (12)

typedef struct
{
    HI_U32 ulTimes;
    HI_U8 aucStatus[MRS_WHITELIST_CHANGE_ITEM_MAX]; // 0x55: 关闭白名单  0xAA: 启动恢复定时器  0xAB: 恢复白名单  0xAC: 定时器刷新
    HI_U32 aulChangeTS[MRS_WHITELIST_CHANGE_ITEM_MAX];
} DIAG_CMD_MRS_CCO_WHITELIST_CHANGE_IND_STRU;

typedef DIAG_CMD_USER_VER_IND_STRU DIAG_CMD_MRS_GET_USER_VER_STRU;

///////////////////////////////////////////////////////////////////////////////

typedef struct
{
    HI_U32 ulLen;    // 待转发数据的长度
    HI_U8 data[0];   // 带转发数据的内容
}DIAG_MRS_TRANSMIT_STRU;  // DIAG通道的数据透传结构(用于Hso透传数据到STA串口)

enum
{
  EN_ERR_ID_MRS_CHL_CTX_MEM_GET,
  EN_ERR_ID_MRS_SRV_CCO_CTX_MEM_GET,
  EN_ERR_ID_MRS_SRV_CCO_ADDRMAP_MEM_GET,
  EN_ERR_ID_MRS_SRV_CCO_MACADD_ADDLIST_MEM_GET,
  EN_ERR_ID_MRS_SRV_CCO_MACADD_DELLIST_MEM_GET,
};


typedef struct
{
    HI_U8 ucVer;			//version   the default value is R1
    HI_U8 ucStructSize;		//
    HI_U8 ucReserved[2]; 	// reserved
} DIAG_CMD_MRS_GET_RS485_MULTI_CONNECT_REQ_STRU;// the struct of multi-connect   request

//应答结构体
typedef struct
{
    HI_U32 ulOccurTime; //  occur time   unit :s
} MRS_RS485_MULTI_CONNECT_ITEM;// The struct of respond

#define MRS_RS485_MULTI_CONNECT_MAX_TIMES   (1)// the max capacity of record
typedef struct
{
    HI_U8 ucVer;	     	  // version   the default value is R1
    HI_U8 ucStructSize;		  //size of struct
    HI_U8 ucRecordTimes;	  // record time
    HI_U8 ucReserved; 		  // reserved

    HI_U32 ulTotalTimes;	  //totaltimes of occur
    HI_U32 ulUserQryTime;	  //
    MRS_RS485_MULTI_CONNECT_ITEM astRecord[0]; //item of record
} DIAG_CMD_MRS_GET_RS485_MULTI_CONNECT_IND_STRU;//  The struct of respond


//清除II采485混接命令
typedef struct
{
    HI_U8 ucVer;			//version
    HI_U8 ucStructSize;		//
    HI_U8 ucReserved[2]; 	// reserved
} DIAG_CMD_MRS_ClEAR_RS485_MULTI_CONNECT_REQ_STRU;//the struct of request

typedef struct
{
	HI_U8 ucVer;			//
	HI_U8 ucStructSize; 	//
	HI_U8 ucReserved[2]; 	// reserved

	HI_U32 ulErrorCode;     //ERR CODE   0：SUCCESS；1：FAILTER
} DIAG_CMD_MRS_ClEAR_RS485_MULTI_CONNECT_IND_STRU;// The struct of respond


typedef enum
{
    HI_MRS_ERR_CODE_SUCCESS = 0,
    HI_MRS_ERR_CODE_WRONG_VERSION,
    HI_MRS_ERR_CODE_INVALID_TYPE,
    HI_MRS_ERR_CODE_LENGTH,

    HI_MRS_ERR_CODE_OTHER = 0xFF,
} DIAG_MRS_ERR_CODE_E;

typedef struct
{
	HI_U8 ucVer;
    HI_U8 ucStructSize;		// the size of struct
    HI_U16 usLen;    		// the len of transmit data
    HI_U8 ucErrCode; 		//err code  --DIAG_MRS_ERR_CODE_E
	HI_U8 ucReserved[3];	//
    HI_U8 data[0];   		// transmit data
} DIAG_MRS_RS485_TRANSMIT_IND_STRU;

/* END  : Modified by cuichunyan/c00276962 2016/11/10  问题单号:cltii R485 MultiConnect */


/* BEGIN: Modified by cuichunyan/c00276962 2016/11/10  问题单号:cltii R485 MultiConnect */

typedef struct
{
    HI_U8 ucVersion;                        //Version
    HI_U8 ucStructSize;                     //the size of struction
    HI_U8 ucReserved[2];                    //reserved
    HI_FTM_DEVICE_UART_CFG_S stComCfg;      //config of com
    DIAG_MRS_TRANSMIT_STRU stTransData;     //data of transmit
} DIAG_MRS_RS485_TRANSMIT_REQ_STRU;  // the struct of transmit data


typedef struct
{
    HI_FTM_CHIP_TYPE_E enChipType;
    HI_FTM_PRODUCT_TYPE_E enProductType;
    HI_U32  ulRet;
    HI_U8   reserve[8];               //保留位
}HI_DFX_HW_PRODUCT_ID_QRY_REQ_S;


/* END  : Modified by cuichunyan/c00276962 2016/11/10  问题单号:cltii R485 MultiConnect */
// 设置客户信息(CCO 工装测试)
typedef struct
{
    HI_U16 usStruVer : 6;	/* 结构体版本号，固定为0 */
    HI_U16 usStruSize: 8;	/* 此结构体大小 */
    HI_U16 usProtoVer: 2;	/* 规约类型：0:376.2协议; 1:1376.2协议; */
    HI_U16 usSwVer;			/* 软件版本号，BCD格式 */
    HI_CHAR szUserVer[32];	/* 用户版本号，ASCII格式 */
    HI_CHAR szChipCode[3];	/* 芯片代码，ASCII格式 */
    HI_U8 aucDate[3];		/* 版本日期，BIN格式，YYMMDD */
    HI_CHAR aucManuCode[3]; 	/* 厂商代码，ASCII格式 */
    HI_U8 reserved[3];
} DIAG_CCO_DUT_SET_USER_INF_REQ_STRU;

typedef struct
{
    HI_U32 ulRet;		/* 设置结果：HI_ERR_SUCCESS:成功; 其他:失败 */
} DIAG_CCO_DUT_SET_USER_INF_IND_STRU;

typedef struct
{
   HI_U8 ucVer;
   HI_U8 ucStructSize;
   HI_U8 ucReserved[2];
   HI_U8 ulPad[32];
}HI_DIAG_CMD_MRS_REFRESH_NV_FILE_REQ_S;


typedef struct
{
    HI_U8 ucVer;
    HI_U8 ucStructSize;
    HI_U8 ucReserved[34];
    HI_U32 ulRc;     // 结果码, HI_ERR_SUCCESS表示成功, 其它为错误码
} HI_DIAG_CMD_MRS_REFRESH_NV_FILE_IND_S;

typedef enum
{
	NDM_INTERCONNECT_CMD_ID_CONNECT = 0,//连接/搜索
	NDM_INTERCONNECT_CMD_ID_TOPO,//查询拓扑
	NDM_INTERCONNECT_CMD_ID_METER_ARCHIVE,//查询表档案
	NDM_INTERCONNECT_CMD_ID_ROUTE_CTRL,//路由操作
	NDM_INTERCONNECT_CMD_ID_CCO_READ_METER,//CCO远程点抄STA
	NDM_INTERCONNECT_CMD_ID_STA_READ_METER,//本地点抄STA
	NDM_INTERCONNECT_CMD_ID_MAX,
}NDM_INTERCONNECT_CMD_ID;
typedef struct
{
    HI_U8 des_mac[HI_PLC_MAC_ADDR_LEN];
    HI_U8 success_mac[HI_PLC_MAC_ADDR_LEN];
    NDM_INTERCONNECT_CMD_ID  cmd_id;//命令ID
    HI_BOOL isonline;
    HI_U8 pad[3];
}ndm_interconnect_info;
typedef struct
{
    NDM_INTERCONNECT_CMD_ID  cmd_id;//命令ID
    HI_U32  ulDataSize; // 数据长度
    HI_U8  aucData[0];//数据内容
} HI_NDM_INTERCONNECT_CMD_STRU;
typedef struct
{
HI_U8  des_mac[6]; //目标MAC
HI_U8  pad[2];
} NDM_INTERCONNECT_CMD_ID_CONNECT_REQ_STRU;
#define HI_NDM_CONNECT_SUCCESS		(1) // 成功
#define HI_NDM_CONNECT_FAILE   	(0) // 失败
typedef struct
{
HI_U8  ret; // 连接结果
HI_U8  des_mac[6]; //目标MAC
HI_U8  pad[1];
} NDM_INTERCONNECT_CMD_ID_CONNECT_IND_STRU;


// 注意:不要更改枚举值顺序，HSO兼容性
typedef enum
{
    HI_SD_DC_SET_CONFIG_ERR_CODE_NONE,
    HI_SD_DC_SET_CONFIG_ERR_CODE_SYS_CALL,         //系统错误
    HI_SD_DC_SET_CONFIG_ERR_CODE_INVALID_PARAM,    //参数格式不匹配
    HI_SD_DC_SET_CONFIG_ERR_CODE_TRIGER_TYPE,      //触发类型
    HI_SD_DC_SET_CONFIG_ERR_CODE_AGGAIN,           //固定增益
    HI_SD_DC_SET_CONFIG_ERR_CODE_PERIOD_TIME,      //周期时间
    HI_SD_DC_SET_CONFIG_ERR_CODE_PER_TIMES,        //每轮采数次数不对
    HI_SD_DC_SET_CONFIG_ERR_CODE_TRI_TIME,         //触发采时间
    HI_SD_DC_SET_CONFIG_ERR_CODE_FOLDER_NAME,      //文件夹名字错误
    HI_SD_DC_SET_CONFIG_ERR_CODE_MAC_ADDR,         //MAC地址非法
    HI_SD_DC_SET_CONFIG_ERR_CODE_MAC_FREQ,         //频段非法
    HI_SD_DC_SET_CONFIG_ERR_CODE_MAC_SNID,	       //SNID错误
    HI_SD_DC_SET_CONFIG_ERR_CODE_GET_RTC_TIME_FAIL, //获取RTC时间失败
    HI_SD_DC_SET_CONFIG_ERR_CODE_NOT_SUPPORT_FOLLOW_MODE,//不支持跟随模式
}HI_SD_DC_SET_CONFIG_ERR_CODE_E;//注意:不要更改枚举值顺序，HSO兼容性
typedef enum
{
    HI_SD_DC_SET_CONFIG_Q_ATTRIBUTE_BIT_FOLLOW      = 0x0001,  // 1表示跟随模式
    HI_SD_DC_SET_CONFIG_Q_ATTRIBUTE_BIT_FIX_FREQ    = 0x0002,  // 1表示固定频段
    HI_SD_DC_SET_CONFIG_Q_ATTRIBUTE_BIT_CONTINU_DC  = 0x0004,  // 1表示支持断点续传
    HI_SD_DC_SET_CONFIG_Q_ATTRIBUTE_BIT_BECOLLECTING = 0x0008, // 是否正在采数，仅用于查询当前状态
    HI_SD_DC_SET_CONFIG_Q_ATTRIBUTE_BIT_MAX          = 0x10000,
}HI_SD_DC_SET_Q_CONFIG_ATTRIBUTE_BIT_E;//HI_U16

typedef enum
{
    SD_DC_DATA_COLLECT_TYPE_NOISE = 0x0001,
    SD_DC_DATA_COLLECT_TYPE_FC = 0x0002,
    SD_DC_DATA_COLLECT_TYPE_STAT4 = 0x0004,

    /*已下与DRV无关 周期采数配置*/
    SD_DC_DATA_COLLECT_TYPE_DEFAULT = 0x0008,
}SD_DC_DATA_COLLECT_TYPE_E;

#define SD_DC_DATA_COLLECT_TYPE_ALL         (SD_DC_DATA_COLLECT_TYPE_NOISE|SD_DC_DATA_COLLECT_TYPE_FC|SD_DC_DATA_COLLECT_TYPE_STAT4|SD_DC_DATA_COLLECT_TYPE_DEFAULT)//所有采数类型
#define SD_DC_DATA_COLLECT_TYPE_UNFOLLOW_ALL         (SD_DC_DATA_COLLECT_TYPE_NOISE|SD_DC_DATA_COLLECT_TYPE_FC|SD_DC_DATA_COLLECT_TYPE_STAT4|SD_DC_DATA_COLLECT_TYPE_DEFAULT)//非跟随 :所有类型
#define SD_DC_DATA_COLLECT_TYPE_FOLLOW_ALL  (SD_DC_DATA_COLLECT_TYPE_NOISE|SD_DC_DATA_COLLECT_TYPE_DEFAULT)//跟随:所有类型
#define SD_DC_DATA_COLLECT_TYPE_NOISE_ALL   (SD_DC_DATA_COLLECT_TYPE_NOISE|SD_DC_DATA_COLLECT_TYPE_DEFAULT)//噪声:所有类型
#define SD_DC_DACOLLECT_TYPE_TRI_ALL        (SD_DC_DATA_COLLECT_TYPE_NOISE|SD_DC_DATA_COLLECT_TYPE_FC|SD_DC_DATA_COLLECT_TYPE_STAT4)//触发:所有类型

typedef struct
{
    HI_U8   ccomac[HI_PLC_MAC_ADDR_LEN];    //跟随的CCO的MAC地址
    HI_U8   freq;                           //跟随的网络的SNID
    HI_U8   snid;                            //跟随网络的snid

    HI_U32  ramSize;                        //盲采开始地址 0x200000
    HI_S32  AgcGain; 		                //增益对盲采有效
    HI_U16  usTrigerType;                   //采数类型SD_DC_DATA_COLLECT_TYPE_E
    HI_U16  periodTime;                     //盲采周期，单位分钟
    HI_U16  trigerTime;                     //多长时间收不到PB快进行触发采数，单位分钟
    HI_U16  attribute;                      //属性HI_SD_DC_SET_Q_CONFIG_ATTRIBUTE_BIT_E

    HI_U8   TimesPerPeriod;                 //盲采每个周期连续进行多少次采集。
    HI_CHAR scFolderName[NDM_DC_FOLDER_NAME_SIZE];
    HI_U8   pad[2];
}HI_SD_DC_CONFIG_SET_CMD_S; //设置REQ

typedef struct
{
    HI_SD_DC_SET_CONFIG_ERR_CODE_E errCode;
    HI_U32 reserver;
}HI_SD_DC_CONFIG_SET_IND_S;//设置IND

typedef struct
{
    HI_U32 reserver;
}HI_SD_DC_CONFIG_QUERY_CMD_S;//查询REQ

typedef struct
{
    //错误吗
    HI_SD_DC_SET_CONFIG_ERR_CODE_E errCode;
    HI_U32 curTimeSec;

    //MAC
    HI_U8   ccomac[HI_PLC_MAC_ADDR_LEN];    //跟随的CCO的MAC地址
    HI_U8   freq;                           //跟随网络的freq
    HI_U8   snid;

    //AgcGain
    HI_U32  ramSize;
    HI_S32  AgcGain; 		                //增益对盲采有效
    HI_U16  usTrigerType;                   //采数类型
    HI_U16  periodTime;                     //盲采周期，单位分钟
    HI_U16  trigerTime;                     //多长时间收不到PB快进行触发采数，单位分钟
    HI_U16  attribute;                      //属性HI_SD_DC_SET_Q_CONFIG_ATTRIBUTE_BIT_E
    HI_U8   TimesPerPeriod;                 //盲采每个周期连续进行多少次采集。
    HI_CHAR scFolderName[NDM_DC_FOLDER_NAME_SIZE];
    HI_U8   pad1[2];

}HI_SD_DC_CONFIG_QUERY_IND_S;

typedef struct
{
    HI_U32 ulSdTotalSize;       // SD卡总大小
    HI_U32 ulSdFreeSize;        // SD卡剩余空间

    HI_U16 usTotalFileCnt;      // 各种文件个数统计量之和，产生创建文件夹错误时小于各个文件统计量之和
    HI_U16 usFileCnt_ZCross;    // 过零采数文件个数

    HI_U16 usFileCnt_D;         // 本次采集盲采文件个数
    HI_U16 usFileCnt_N;         // 本次采集触发噪声文件个数(非跟随模式直接采，跟随模式跟随成功|失败时采)

    HI_U16 usFileCnt_S;         // 本次采集状态4文件个数
    HI_U16 usFileCnt_F;         // 本次采集FC文件个数

    HI_U16 usFileCnt_A;         // 本次采集全噪声个数
    HI_U16 usFileCnt_P;         // 本次采集半噪声个数

    HI_U32 ulLastRcvPbTimeStamp;// 上一次收到PB块的时间戳，单位s
    HI_U32 ulCurPeriodStartTime;// 本周期开始时间戳，单位s

    HI_U8  ucCurFolderSequence; // 当前序号文件序号 MRS_DC_SD_MAX_SEQUENCE_FOLDER_NUM
    HI_U8  pad[3];
}MRS_DC_SD_STAT_CTX_STRU;

typedef struct
{
    HI_U8   scCcoMac[HI_PLC_MAC_ADDR_LEN];  // 跟随的CCO的MAC地址
    HI_U8   ucFreq;                         // 跟随网络的freq
    HI_U8   ucIsFixFreq;                    // 是否固定频段

    HI_U8   ucSnid;             // 跟随的snid
    HI_U8   ucIsFollowMode;     // 是否是跟随模式
    HI_U8   ucIsSetRebootStart; // 当采数开始时，是否将正在采数状态更新到NV中:仅当NV配置支持断电续采，且启动原因非过零采数时设置为HI_TRUE
    HI_U8   ucZCrossType;       // 过零采数目的类型:上升沿or下降沿 MRS_DC_SD_ZCROSS_ISR_TYPE_E

    HI_U32  ulStartAddr;        // 采数内存地址
    HI_U32  ulRamSize;          // 盲采开始地址
    HI_S32  slAgcGain;          // 增益对盲采有效

    HI_U16  usTrigerType;       // 采数类型 MRS_DC_SD_DATA_COLLECT_TYPE_E
    HI_U16  usPeriodTime;       // 盲采周期，单位分钟
    HI_U16  usTrigerTime;       // 多长时间收不到PB快进行触发采数，单位分钟
    HI_U16  usNameId;           // 每次按键启动采数 +1

    HI_CHAR scFolderName[NDM_DC_FOLDER_NAME_SIZE];  // "DC_XXX"
    HI_U8   ucTimesPerPeriod;   // 盲采每个周期连续进行多少次采集。
    HI_U8   ucCPowerOnEable;    // 重启续采功能使能开关
    HI_U8   pad;
}MRS_DC_SD_CONFIG_CTX_STRU;

typedef struct
{
    //统计量
    HI_U16 usMsgSendFailTimes;          // 消息发送失败次数
    HI_U8  ucSdCardErrTimes;            // 无法创建文件等
    HI_U8  ucSdCardOpenErrTimes;        // 写文件错误

    HI_U8  ucSdCardWriteErrTimes;       // 写文件错误
    HI_U8  ucSdCardCloseErrTimes;       // 关闭文件错误
    HI_U8  ucSdCardCreatFolderErrTimes; // 创建文件夹错误
    HI_U8  ucSdCardGetInfoErrTimes;     // 获取SD卡状态错误

    HI_U8  ucSdCardAccessErrTimes;
    HI_U8  ucStopReason;                // 采数停止原因MRS_DC_SD_STOP_TYPE_E
    HI_U8  ucNotStartReason;            // 记录采数无法开始原因 MRS_DC_SD_NOT_START_REASON_E
    HI_U8  ucSetRtcFailTimes;           // 设置RTC时间失败次数
} MRS_DC_SD_ERR_CTX_STRU;

typedef struct
{
    HI_U8 ucMainPhase;  // 采数主流程 MRS_DC_SD_MAIN_PHASE_E
    HI_U8 ucStartType;  // 采数启动原因 MRS_DC_SD_START_TYPE_E
    HI_U8 reserver[2];
} MRS_DC_SD_MAIN_CTRL_CTX_STRU;

typedef struct
{
    MRS_DC_SD_MAIN_CTRL_CTX_STRU stMainCtrlCtx;
    MRS_DC_SD_ERR_CTX_STRU stErrCtx;
    MRS_DC_SD_CONFIG_CTX_STRU stConfigCtx;
    MRS_DC_SD_STAT_CTX_STRU stStatCtx;
} HI_NDM_DATA_COLLECT_STATUS_MSG_S;

typedef HI_NDM_DATA_COLLECT_STATUS_MSG_S MRS_NDM_DC_SD_CTRL_STRU;
#endif //__DFX_APP_H__

