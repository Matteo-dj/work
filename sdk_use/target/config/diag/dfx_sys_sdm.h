//*****************************************************************************
//
//                  版权所有 (C), 1998-2011, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : dfx_sys_sdm.h
//  版 本 号   : V1.0
//  作    者   : shenhankun/00130424
//  生成日期   : 2011-11-10
//  功能描述   : 系统级诊断接口定义
//
//  函数列表   : 无
//  修改历史   :
//  1.日    期 : 2012-08-20
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

#ifndef __DFX_SYS_SDM_H__
#define __DFX_SYS_SDM_H__

#include "hi_types.h"
#include "hi_mdm_types.h"
#include <hi_ft_nv.h>
#include <dfx_sys.h>
/* BEGIN: Added by likunhe/00194117, 2014/6/10   PN:DTS2014060603580*/
#include "hi_phy_nv.h"
#include "hi_phy_nv_v200.h"
/* END:   Added by likunhe/00194117, 2014/6/10 */

HI_START_HEADER
//*****************************************************************************


//*****************************************************************************
// SDM interface
//*****************************************************************************

#define HI_DFX_RD_2K_MEM_SIZE       2*1024
#define HI_DFX_RD_SMALL_MEM_SIZE    90
#define NM_STAT_SOFT_RESET_INFO_NUM     (5)        // 统计软重启的最大个数，超过范围循环使用

/* BEGIN: Added by likunhe/00194117, 2014/6/10   PN:DTS2014060603580*/
#define HI_SDM_FA_EVALUATE_RESULT_FREQ_NUM_MAX      3           //频段评估中参存储的频段个数最大值
/* END:   Added by likunhe/00194117, 2014/6/10 */

typedef struct
{
    HI_U32 ulAddr;
    HI_U32 ulSize; // *2KB
} HI_DFX_RD_MEM_S;

typedef struct
{
    HI_U32 ulAddr;
}HI_SDM_KLG_MACBUFFER_S;

typedef struct
{
    HI_U32  snid;                        // 重启前的snid

    HI_U8  soft_reset_reason;           // MAC软重启原因
    HI_U8  freq;                        // 重启前得频段
    HI_U8  pad[2];                         // 用于记录更详细的可维可测
    HI_U32 reset_sys_time;              // MAC软重启事件
}DIAG_CMD_SOFT_RESET_INFO_STRU;

typedef struct
{
    HI_U8  index;                       // 软重启计数
    HI_U8  pad[3];
    DIAG_CMD_SOFT_RESET_INFO_STRU soft_reset_info[NM_STAT_SOFT_RESET_INFO_NUM];
}DIAG_CMD_SOFT_RESET_STRU;

//*****************************************************************************

typedef struct
{
    HI_U32 last_fail_reason;
    HI_U16 call_total_times;
    HI_U16 call_fail_times;
}HI_DIAG_SYNC_STAT_S;
#if (defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) && !defined(PRODUCT_CFG_EQUIP_TEST_MODE))
HI_EXTERN HI_DIAG_SYNC_STAT_S g_diag_sysnc_stat;
#endif

//*****************************************************************************
// 系统级 保存到Flash的关键信息
//*****************************************************************************
typedef struct
{
    HI_U32 cnt[10];
    HI_U32 size[10];
}HI_SYS_DFX_NM_ERR_SIZE_LOG_S;

typedef struct
{
    HI_U32 ulMagic;
    HI_U32 ulBootCnt;
    HI_U32 hi_sys_upg_start;
    HI_U32 hi_sys_upg_stop;  //stop 的次数
    HI_U32 nv_init_Phase1;
    HI_U32 nv_init_Phase2;

    HI_U8 hi_sys_upg_update;
    HI_U8 hi_sys_upg_sta_update;//dai shuo
    HI_U8 hi_sys_upg_crc_ok;
    HI_U8 hi_sys_upg_finish;//重启前

    HI_U8 hi_sys_upg_ok;// 重启后
    HI_U8 application_init;
    HI_U8 mac_close_led;
    HI_U8 wdg_enable;

    HI_U8 init_end;
    HI_U8 MAC_RsbTaskInit_ok;
    HI_U8 MAC_TxTaskInit_ok;
    HI_U8 MAC_CsTaskInit_ok;

    HI_U8 MAC_NmMsgTaskInit_ok;
    HI_U8 MAC_NmEvtTaskInit_ok;
    HI_U8 MAC_DfxTaskInit_ok;
    HI_U8 temp;
}HI_SDM_KLG_INIT_S;

typedef enum
{
    HI_RUNSTATUS_10         = 10, // 10 收到PLC
    HI_RUNSTATUS_20         = 20, // 20 处理PLC消息
    HI_RUNSTATUS_30         = 30, // 30 进队列
    HI_RUNSTATUS_40         = 40, // 40 处理队列
    HI_RUNSTATUS_50         = 50, // 50 发串口消息
    HI_RUNSTATUS_60         = 60, // 60 处理串口发送消息
    HI_RUNSTATUS_70         = 70, // 70 接收到串口报文，并发送消
    HI_RUNSTATUS_80         = 80, // 80 处理串口接收消息
    HI_RUNSTATUS_90         = 90, // 90 发送PLC消息到队列
    HI_RUNSTATUS_100        = 100 // 100 响应消息，发送PLC
}HI_APP_RUNSTATUS;

typedef struct
{
    HI_U32 ulMagic;
    HI_U32 IsNetConnected; // 是否已联网
    HI_U32 runStatus;      // 当前运行状态
    HI_U32 runTimeStamp;   // 运行状态刷新时间

    //PLC通路
    HI_U32 plcMsgId;     // 最近一次接收到的报文类型、长度，时刻
    HI_U32 plcMsgLen;    // 最近一次接收到的报文长度
    HI_U32 plcMsgTimeStamp; // 最近一次接收到的报文时刻
    HI_U32 plcTotalRxBlk; // PLC接收到的帧计数


    //UART通路
    HI_U32 uartMsgLen;    // 最近一次接收到的报文长度
    HI_U32 uartMsgTimeStamp; // 最近一次接收到的报文时刻

    // 当前队列状态

    // 异常情况
    // 初始化失败、内存申请失败
    //HI_U32 InitFailed;
    //HI_U32 MallocFailedCnt;

    // 发送给PLC失败
    HI_U32 plcSendFailedCnt;
    HI_U32 plcSendMsgFailedCnt;
    // 发送串口失败
    HI_U32 uartSendFailedCnt;
    HI_U32 uartSendMsgFailedCnt;

    // 当前消息队列元素个数
    HI_U32 MsgQueenCnt;

} HI_SDM_KLG_APP_S;

typedef enum
{
    HI_SMID_BEACON_FC            = 0,
    HI_SMID_BEACON_PAYLOAD       ,
    HI_SMID_BEACON_FC_B          ,
    HI_SMID_BEACON_PAYLOAD_B     ,
    HI_SMID_BEACON_FC_C          ,
    HI_SMID_BEACON_PAYLOAD_C     ,
    HI_SMID_BEACON_SLOT_MGR_HEAD ,
    HI_SMID_NDM_FRG_RSB_LINK     ,
    HI_SMID_DIAG_BUFFER_PRINT    ,
    HI_SMID_DIAG_BUFFER_MSG      ,
    HI_SMID_DIAG_BUFFER_CMD      ,
    HI_SMID_DIAG_BUFFER_DATA     ,
    HI_SMID_DIAG_BUFFER_EVT      ,
    HI_SMID_DIAG_BUFFER_LOCAL    ,
    HI_SMID_DIAG_BUFFER_PROXY    ,
    HI_SMID_MAC_BUFFER_FREE_LIST    ,
    HI_SMID_MRS_FW_CTX,
    HI_SMID_MRS_IO_CTX,
    HI_SMID_MRS_376PROTO            ,
    HI_SMID_MRS_CCO_CTX            ,
    HI_SMID_MRS_CCO_SRV_METER_INFO            ,
    HI_SMID_MRS_CCO_SRV_METER_FLAG            ,
    HI_SMID_MRS_CCO_SRV_METER_RPT_INFO            ,
    HI_SMID_MRS_COLLECTOR_CTX            ,
    HI_SMID_MAC_PK_FILTER_V1            ,
    HI_SMID_MAC_PK_FILTER_V2            ,
    HI_SMID_MAC_CCO_IP_ENTRY            ,
    HI_SMID_MAC_STA_IP_ENTRY            ,
    HI_SMID_MAC_ROUTE_PATH_TABLE            ,
    HI_SMID_MAC_ROUTE_TABLE            ,
    HI_SMID_UPG_FW_CTX,
    HI_SMID_CCO_UPG_CTX            ,
    HI_SMID_STA_UPG_CTX            ,
    HI_SMID_PLC_MAC_SWITCH          ,

    HI_SMID_MAX = 36
}HI_SMID_E;

#if !defined(PRODUCT_CFG_CORE_TYPE_BOOTLOADER) || defined(PRODUCT_CFG_HSO)
#define HI_DFX_SYS_INFO_ERR_FLASH_ERR    1
typedef struct
{
    HI_U32 ulAdd;
    HI_U32 ulSize;
}HI_SDM_GMEM_INFO_S;

typedef struct
{
    HI_U8 *fc;
    HI_U8 *payload;
    HI_U16 pb_size;
    HI_U16 rsvd;
} HI_MAC_BCN_FRM_S;

#define HI_DFX_SAVE_ID_MAC_REJOIN      1
#define HI_DFX_SAVE_ID_NM_NET_FORMED   2
#define HI_DFX_SAVE_ID_TX_RX_PERIOD    3

typedef struct
{
    HI_U8 aucMTskId[HI_SYS_MTSK_MAX_NUM];
    HI_U8 aucMTskStkErr[HI_SYS_MTSK_MAX_NUM];
    HI_U8 aucATskId[HI_SYS_ATSK_MAX_NUM];
    HI_U8 aucATskStkErr[HI_SYS_ATSK_MAX_NUM];
}HI_SYS_TASK_STK_ERR_S;

#if defined(PRODUCT_CFG_SUPPORT_SDM_KLG) || defined(PRODUCT_CFG_SUPPORT_SDM_TM_KLG)
#define HI_SDM_KLG_SYS_MAGIC_CODE 0xA1B2C3D4

typedef struct
{
    HI_U32 ulMagic;
    HI_U32 ulBootCnt;   // boot counter
    HI_U16 bStartup;
    HI_U16 bCloseLed;
    HI_U32 ulRunTimeStamp; // sec

    HI_U32 ulBootTime0; // sec
    HI_U32 ulBootTime1; // sec
    HI_U32 ulRebootCnt; // reboot counter by HI_SYS_ReBoot().
    HI_U32 ulRebootId;
    HI_U32 ulRebootTime; // sec

    HI_U32 ulInt1Cnt[2]; // d/e
    HI_U32 ulInt2Cnt[2]; // d/e
    HI_U32 aulUpgCnt[2]; // ok/err
    HI_U32 ulErr; // flash read err

    HI_U32 ulOsScheduleCnt;
    HI_U32 ulOsTimerRunCnt;
    HI_U32 ulHrLisrTimerCnt;
    HI_U32 ulHrHisrTimerCnt;

    HI_SYS_STATUS_IND_S           stSysStatusInd;
    HI_U32 ulWdEnable;
    HI_U8 aucDbgSync[4];

#if defined(PRODUCT_CFG_SUPPORT_SDM_KLG)
    HI_SYS_TASK_STK_ERR_S stTskStkErr;

    HI_U32 tx_mac_led_on_cnt;
    HI_U32 tx_mac_led_off_cnt;
    HI_U32 tx_app_led_on_cnt;
    HI_U32 tx_app_led_off_cnt;

    HI_U32 rx_mac_led_on_cnt;
    HI_U32 rx_mac_led_off_cnt;
    HI_U32 rx_app_led_on_cnt;
    HI_U32 rx_app_led_off_cnt;

    HI_U32 mac_rejoin_cnt;
    HI_U8 save_id;

    HI_SYS_DFX_NM_ERR_SIZE_LOG_S  stErrSizeLog;
    HI_SDM_GMEM_INFO_S            stGmemInfo[HI_SMID_MAX];
    HI_MAC_BCN_FRM_S              bcn_frm[3];
    HI_U32 ulMacBufferHead;
#endif
} HI_SDM_KLG_SYS_S;
#endif
//*****************************************************************************

//*****************************************************************************
#define HI_SYS_DFX_nm_create_assoc_cnf_OFFSET       0
#define HI_SYS_DFX_nm_create_assoc_ind_OFFSET       1

#if defined(PRODUCT_CFG_SUPPORT_SDM_TM_KLG)
HI_EXTERN HI_SDM_KLG_SYS_S                  g_stSdmKlgSys;
#endif

#if defined(PRODUCT_CFG_SUPPORT_SDM_KLG)
HI_EXTERN HI_SDM_KLG_INIT_S                 g_stSdmKlgInit;
HI_EXTERN HI_SDM_KLG_APP_S                  g_stSdmKlgApp;
HI_EXTERN HI_SDM_KLG_MACBUFFER_S            g_stSdmMacBuffer[];

#define SET_g_stSdmSklgInfo_m(m,v)          g_stSdmKlgSys.m = v
#define SET_g_stSdmSklgInfo_m_inc(m)        g_stSdmKlgSys.m++
#define SET_g_stSdmSklgInit_m(m,v)          g_stSdmKlgInit.m = v
#define SET_g_stSdmSklgInit_m_inc(m)        g_stSdmKlgInit.m ++
#define SET_g_stSdmSklgApp_m(m,v)           g_stSdmKlgApp.m = v
#define SET_g_stSdmSklgApp_m_inc(m)         g_stSdmKlgApp.m++
#else
#define SET_g_stSdmSklgInfo_m(m,v)
#define SET_g_stSdmSklgInfo_m_inc(m)
#define SET_g_stSdmSklgInit_m(m,v)
#define SET_g_stSdmSklgInit_m_inc(m)
#define SET_g_stSdmSklgApp_m(m,v)
#define SET_g_stSdmSklgApp_m_inc(m)
#endif


//*****************************************************************************
// SDM命令定义
//*****************************************************************************
#define HI_DSID_SDM_BASE           0xB110    // HI_DSID_SDM_CHL_S

#define HI_DSID_SDM_BSP            0xB110    // ,,HI_DSID_SDM_BSP_S,
#define HI_DSID_SDM_CHL            0xB111    // ,,HI_DSID_SDM_CHL_S
#define HI_DSID_SDM_RX             0xB112    // ,,HI_DSID_SDM_RX_S
#define HI_DSID_SDM_TX             0xB113    // ,,HI_DSID_SDM_TX_S
#define HI_DSID_SDM_BCN            0xB114    // ,,HI_DSID_SDM_BCN_S
#define HI_DSID_SDM_NM             0xB115    // ,,HI_DSID_SDM_NM_S
#define HI_DSID_SDM_RT             0xB116    // ,,HI_DSID_SDM_RT_S
#define HI_DSID_SDM_MR             0xB117    // ,,HI_DSID_SDM_MR_S
#define HI_DSID_SDM_DUMP           0xB118    // HI_DSID_SDM_DUMP_S, ,HI_DSID_SDM_XX_S
#define HI_DSID_SDM_EVT            0xB119
#define HI_DSID_SDM_DATA_DUMP      0xB120    // HI_SDM_DATA_DUMP_REQ_S,,HI_SDM_DATA_DUMP_IND_S

#define HI_DSID_SDM_EVT_SEARCH     0xB121
#define HI_DSID_SDM_STORAGE_HEAD   0xB122

#define HI_DSID_SDM_RAM_ERASE      0xB123

/* BEGIN: Added by b00208046, 2013/12/16   PN:DTS2013122602557 */
//0xB130-0xB139 抄表可维可测使用
#define HI_DSID_SDM_MRS_BASE       0xB130
#define HI_DSID_SDM_MLG_LRPR       0xB130
#define HI_DSID_SDM_MLG_LOW_RATIO  0xB131
#define HI_DSID_SDM_MLG_XR         0xB132
#define HI_DSID_SDM_MLG_DAY_FREEZE 0xB133
#define HI_DSID_SDM_MRS_END        0xB133
/* END:   Added by b00208046, 2013/12/16 */
/* BEGIN: Added by b00208046, 2014/3/17   问题单号:DTS2014012702376 */
#define HI_DSID_SDM_FA             0xB135
#define HI_DSID_SDM_BAT            0xB136
/* END:   Added by b00208046, 2014/3/17 */
#define HI_DSID_SDM_KLG_DUMP       0xB150    // HI_SDM_KLG_DUMP_S
#define HI_DSID_SDM_KLG_SYS        0xB151    // HI_SDM_KLG_SYS_S
#define HI_DSID_SDM_KLG_INIT       0xB152    // HI_SDM_KLG_INIT_S
#define HI_DSID_SDM_KLG_APP        0xB153    // HI_SDM_KLG_APP_S
#define HI_DSID_SDM_KLG_OS         0xB154    // HI_SDM_KLG_OS_S
#define HI_DSID_SDM_KLG_SYSERR     0xB155
#define HI_DSID_SDM_KLG_ALL        0xB156
//*****************************************************************************
// sdm test 命令定义
//*****************************************************************************
#define HI_DSID_SDM_TEST_BASE      0xB160
#define HI_DSID_SDM_TEST_SEVT      0xB161
#define HI_DSID_SDM_TEST_REVT      0xB162
//....保留
#define HI_DSID_SDM_TEST_END       0xB169
//*****************************************************************************

//*****************************************************************************

typedef struct
{
    HI_U32 ret;
}HI_SDM_ERASE_RAM_IND_S;

//魔术字
#define MAGIC_NUM_PERIOD 0x5245504d//MPER
#define MAGIC_NUM_EVT    0x54564553//SEVT
#define MAGIC_NUM_MRS    0x53524d4d//MMRS
#define MAGIC_NUM_FA     0x3141464D//MFA1
#define MAGIC_NUM_BAT    0x5441424D//MBAT
#define MAGIC_NUM_NMLV   0x4E4d4c56//NMLV
#if ((!defined(PRODUCT_CFG_BOARD_TYPE_ASIC)) && (defined(PRODUCT_CFG_HW_CHECK)))
#define MAGIC_NUM_FPGA   0x41475046//FPGA
#endif
/* BEGIN: Modified by b00208046, 2014/3/17   问题单号:DTS2014012702376 */
typedef struct
{
    HI_U32  id;
    HI_U32  hex[6];
    HI_U32  decimal_0;
    HI_U32  decimal_1;
    HI_U32  decimal_2;
    HI_U32  decimal_3;
    HI_U32  decimal_4;
    HI_U32  decimal_5;
} HI_DSID_SDM_TEST_S;//TEST 命令
/* END:   Modified by b00208046, 2014/3/17 */
typedef struct
{
    HI_U32 idx;
    HI_U32 magic;
    HI_U32 sec:6;
    HI_U32 min:6;
    HI_U32 hour:5;
    HI_U32 day:5;
    HI_U32 month:4;
    HI_U32 year:6;
    HI_U32 macbpc;
} HI_SDM_ENG_STORAGE_S;

/* BEGIN: Modified by b00208046, 2014/3/17   问题单号:DTS2014012702376 */
typedef enum
{
    SDM_BSP = 0, // HI_DSID_SDM_BSP_S
    SDM_CHL,     // HI_DSID_SDM_CHL_S
    SDM_RX,      // HI_DSID_SDM_RX_S
    SDM_TX,      // HI_DSID_SDM_TX_S
    SDM_BCN,     // HI_DSID_SDM_BCN_S
    SDM_NM,      // HI_DSID_SDM_NM_S
    SDM_RT,      // HI_DSID_SDM_RT_S
    SDM_MR = 7,  // HI_DSID_SDM_MR_S
    SDM_EVT,     // HI_SDM_ENG_EVT_S

    /* BEGIN: Added by b00208046, 2013/12/16   PN:DTS2013122602557 */
    //抄表相关数据
    SDM_MRS_LRPR_STATS = 100,
    SDM_MRS_LOW_RATIO  ,
    SDM_MRS_XR_STATS   ,
    SDM_MRS_DAY_FREEZE_STATS,
    SDM_MRS_ALL,

    SDM_FA  = 120,
    SDM_BAT = 130,
    /* END:   Added by b00208046, 2013/12/16 */

}HI_SDM_CMD_ID_E;//大小限制在0-255
/* END:   Modified by b00208046, 2014/3/17 */

//*****************************************************************************
typedef enum
{
  HI_SDM_ENG_EVT_VT_ID_NORMAL = 0,
  HI_SDM_ENG_EVT_VT_ID_WIFI_OFF,
  HI_SDM_ENG_EVT_VT_ID_WIFI_ON,
  HI_SDM_ENG_EVT_VT_ID_SD_DC_START,

  HI_SDM_ENG_EVT_VT_ID_NAC_ETH = 32,
  HI_SDM_ENG_EVT_VT_ID_NAC_UART,
  HI_SDM_ENG_EVT_VT_ID_NAC_UART_RX,
  HI_SDM_ENG_EVT_VT_ID_NAC_BQ_UPG,
  HI_SDM_ENG_EVT_VT_ID_NAC_SD_CARD,
  HI_SDM_ENG_EVT_VT_ID_NAC_PLC_SENSITIVE,
  HI_SDM_ENG_EVT_VT_ID_NAC_KEY0, //采数按键
  HI_SDM_ENG_EVT_VT_ID_NAC_WRITE_BACK_NV,
#if ((!defined(PRODUCT_CFG_BOARD_TYPE_ASIC)) && (defined(PRODUCT_CFG_HW_CHECK)))
  HI_SDM_ENG_EVT_VT_ID_FPGA_CTRL,
#endif

  // TODO:
}HI_SDM_ENG_EVT_VT_ID_E;

/* BEGIN: Modified by b00208046, 2014/3/17   问题单号:DTS2014012702376 */
typedef struct
{
    HI_SDM_ENG_EVT_VT_ID_E id;
    HI_U32 data[3];//替代原有的ulTemperature,ulVoltage,reserve

} HI_SDM_ENG_EVT_VT_S;

#if ((!defined(PRODUCT_CFG_BOARD_TYPE_ASIC)) && (defined(PRODUCT_CFG_HW_CHECK)))
typedef struct
{
    HI_SDM_ENG_EVT_VT_ID_E id;
	HI_S32 temperature;
	HI_U32 temperatureAlarmTimes;
	HI_U8 fanAlarmTimes;
	HI_U8 activeRstTimes;// 单板长时间温度异常，任务无法启动，超过24小时，主动重启
    HI_U8 rsv[2];
} HI_SDM_ENG_EVT_FPGA_S;
#endif

typedef struct
{
    HI_SDM_ENG_EVT_VT_ID_E id;
    HI_U8  flag;
    HI_U8  err_code;
    HI_U16 reserved;
    HI_U32 data[2];
} HI_SDM_ENG_EVT_NAC_S;
typedef struct
{
    HI_U8   bq_ver_old;//BQ文件中版本号
    HI_U8   bq_ver_new;//升级后BQ芯片中读出版本号
    HI_U16  reserved;
    HI_U32  ret;//HI_ERR_XXX
} HI_SDM_ENG_EVT_NAC_BQUPG_S;//可维可测信息 BQ升级私有//default value:  0x0000a1a1
typedef struct
{
    HI_U8   rxDataNum;//UART校验成功次数
    HI_U8   reserved1;
    HI_U16  reserved2;
    HI_U32  ret;//UART测试错误码 HI_ERR_XXX
} HI_SDM_ENG_EVT_NAC_UART_S;//FLASH可维可测私有结构(串口) default value: 0x00000003
typedef struct
{
    HI_SDM_ENG_EVT_VT_ID_E id;
    HI_CHAR rxDada1[4];
    HI_CHAR rxDada2[4];
    HI_CHAR rxDada3[4];
} HI_SDM_ENG_EVT_NAC_UART_RX_S; //default value: 0x63614e23(#Nac) 0x63614e23 0x63614e23
typedef struct
{
    HI_CHAR rxData[4];
    HI_U32  ret;
} HI_SDM_ENG_EVT_NAC_SDCARD_S;//default value: 0x54534554
typedef struct
{
    HI_U32  rxBeaconNum;//收到的Beacon的个数(作为bIsBeacon的赋值依据)
    HI_U8   bIsBeacon;//MAC网络状态 HI_TRUE可以继续测试，HI_FALSE等待按键退出
    HI_U8   txFailNumber;//发送失败次数
    HI_U8   txNumber;//发送次数
    HI_U8   reserved;
} HI_SDM_ENG_EVT_NAC_PLC_S;//default value: 0x0000000n  0x00nn0001
typedef struct
{
    HI_SDM_ENG_EVT_VT_ID_E id;
    HI_U8  isKeyInterrupt;
    HI_U8  isResponceKey;
    HI_U8  isPressDown;
    HI_U8  isPressUp;
    HI_U32 data[2];
} HI_SDM_ENG_EVT_NAC_KEY0_S;//default value: 0x01000101
/* END:   Modified by b00208046, 2014/3/17 */
//*****************************************************************************


//*****************************************************************************




// storage access id
typedef enum
{
  SDM_KLG_FLASH_B0 = 0, // 'crash' SDM_KLG flash storage region.
  SDM_KLG_FLASH_B1,     // 'period' SDM_KLG flash storage region.
  SDM_KLG_FLASH_B2,     // 'initializtion' SDM_KLG flash storage region.
  SDM_KLG_FLASH_B3,     // 'reboot by user(NOT rst mgr)' SDM_KLG flash storage region.
  SDM_KLG_RAM     = 8   // SDM KLG in RAM.
}HI_SDM_KLG_SA_ID_E;

typedef enum
{
  SDM_KLG_SYS = 0,
  SDM_KLG_INIT,
  SDM_KLG_APP,
  SDM_KLG_OS,
  SDM_KLG_SYSERR
}HI_SDM_KLG_ID_E;

typedef struct
{
    HI_SDM_KLG_SA_ID_E said;
    HI_SDM_KLG_ID_E scid;
    HI_U32 ulEnableSdmKlg; // 当前SDM KLG功能使能,收的HI_DSID_SDM_KLG_DUMP命令后默认关闭SDM KLG
} HI_SDM_KLG_DUMP_S;

/* BEGIN: Added by b00208046, 2014/1/20   问题单号:DTS2014010400315 电池电量可维可测*/


//SDM任务内部使用结构体:放在这仅为HSO解析
/* BEGIN: Added by b00208046, 2014/3/17   问题单号:DTS2014012702376 */
typedef struct
{
    //HI_SDM_ENG_EVT_CAUSE_E enSysCause;
    HI_U32 aulData[HI_SYS_MSG_PARAM_NUM_MAX_AULDATA];
}HI_SDM_ENG_EVT_PRV_S;
/* END:   Added by b00208046, 2014/3/17 */

HI_EXTERN HI_U32 HI_SDM_SaveCycleEvent(HI_SDM_ENG_EVT_VT_S* pstEvt,HI_BOOL beSync);
#if ((!defined(PRODUCT_CFG_BOARD_TYPE_ASIC)) && (defined(PRODUCT_CFG_HW_CHECK)))
HI_EXTERN HI_U32 HI_SDM_SaveCycleEventFpga(HI_SDM_ENG_EVT_FPGA_S* pstEvt,HI_BOOL beSync);
#endif
/* END:   Added by b00208046, 2014/1/20 */
#if !defined(PRODUCT_CFG_CORE_TYPE_BOOTLOADER) || defined(PRODUCT_CFG_HSO)

// storage access id
typedef enum
{
  SDM_FLASH = 0,
  SDM_RAM
}HI_SDM_SA_ID_E;

typedef struct
{
    HI_SDM_SA_ID_E  said;  // storage access id
    HI_SDM_CMD_ID_E scid;  // sdm command id.
    HI_U32 offset;    // reserved
    HI_U32 num;
} HI_DSID_SDM_DUMP_S;

typedef struct
{
    HI_U32 ulId;
    HI_U32 cnt;
    HI_U32 local_time;
    HI_U32 net_time;
    HI_U32 status;
}HI_DSID_SDM_HRD_S;

typedef struct
{
    HI_U32          usSaveTime;                 //已废弃
    HI_U32          ulFlashSize;                //flash size
    HI_U32          ulFlashAddr;                //flash basic addr
    HI_U32          pSdmSaveBuffer;             //周期存储结构指针
    HI_U32          magicNum;                   //内容魔术字
    HI_U16          ucSysCause;                 //已废弃
    HI_U16          sdm_content_arrtibute;       //SDM_CONTENT_ATTRIBUTE_E
    HI_U16          ulSdmSaveBufferSize;        //周期存储结构大小
    HI_U16          ulPerBlockSize;             //ulSdmPeriodSaveBufferSize 或者 ulSdmPeriodSaveBufferSize+magic+logic

    HI_U16          ulCurrentWriteBlockIdx;     //记录将要写入的FLASH偏移块号。
    HI_U16          ulCurrentLogicBlockIdx;     //记录将要写入的INDEX号。
    HI_U16          ulMaxWriteBlockIdx;         //可用块
    HI_U16          writeFailTimes;

    HI_U8           sdm_content_type;            //SDM_CONTENT_TYPE_E 内容ID
    HI_U8           sdm_blk_id;                  //共享flashID,对共享FLASH生效
    HI_U8           bCreatedSem:1;               //为1表示信号量创建成功，对支持同步操作的分区有意义
    HI_U8           bInited:1;                   //为1表示初始化完成
    HI_U8           cur_stat_reserver:6;
    HI_U8           reserver_2;

    HI_U32    		semHandle;                   //信号量
    HI_U32          semWaitTime;                 //获取信号量超时时间ms
}SDM_ENG_CTX_S;
typedef struct
{
    HI_U32 ulWriteFail;
    HI_U32 ulReadFail;
    HI_U32 ulMallocFail;
    HI_U32 MsgSendFailTimes;
    HI_U32 MsgSendSucessTimes;
    HI_U32 MsgReceiveTimes;
}SDM_ENG_DSID_MSG_S;
typedef struct
{
    HI_U32 tx_send_sof_cnt;                       // 发送机发送Sof报文个数
    HI_U32 tx_send_sack_cnt;                      // 发送机发送Sack报文个数
    HI_U32 tx_send_beacon_cnt;                    // 发送机发送Beacon报文个数
    HI_U16 tx_send_sound_cnt;                     // 发送机发送Sound报文个数
    HI_U16 tx_send_sound_ack_cnt;                 // 发送机发送Sound ack报文个数
    HI_U16 tx_send_freq_cnt;                      // 发送机发送Freq报文个数
    HI_U16 tx_send_coordinate_cnt;                // 发送机发送Coordinate报文个数
    HI_U16 rx_receive_sound_cnt;            // 接收机接收sound总数
    HI_U16 rx_receive_sound_ack_cnt;        // 接收机接收sound sack总数
    HI_U16 rx_receive_freq_cnt;             // 接收机接收freq总数
    HI_U16 rx_receive_coordinate_cnt;       // 接收机接收coordinate总数
    HI_U32 rx_receive_beacon_cnt;              // 接收机接收Beacon总数
    HI_U32 rx_receive_sof_cnt;              // 接收机接收sof总数
    HI_U32 rx_receive_sack_cnt;             // 接收机接收sack总数
    HI_U32 rx_get_macbuffer_success_cnt;    // 接收机获取缓存成功总数
    HI_U32 rx_get_macbuffer_fail_cnt;       // 接收机获取缓存失败总数
    HI_U16 rx_rcv_not_finished_cnt;         // 接收机接收未完成释放缓存总数
    HI_U16 rx_discard_packet_cnt;           // 接收机丢弃报文总数
    HI_U32 rsb_receive_cnt;                 // 重组模块处理报文总数
    HI_U32 rsb_reassemble_timeout_cnt;      // 重组模块重组超时丢弃报文总数
    HI_U16 rsb_reassemble_fail_cnt;         // 重组模块重组失败丢弃报文总数
    HI_U16 rsb_crc_fail_cnt;                // 重组模块校验失败丢弃报文总数
    HI_U32 rsb_not_receive_cnt;             // 重组模块非本站点丢弃报文总数
    HI_U32 rsb_abnormity_cnt;               // 重组模块报文异常丢弃报文总数
    HI_U32 rsb_filter_cnt;                  // 重组模块过滤报文总数
    HI_U32 rsb_deliver_cnt;                 // 重组模块分发报文总数
    HI_U32 cvg_receive_delivery_cnt;        // 汇聚模块接收分发报文总数
    HI_U32 cvg_discard_unicast_cnt;         // 汇聚模块丢弃单播报文总数
    HI_U32 cvg_transmit_unicast_cnt;        // 汇聚模块转发单播报文总数
    HI_U32 cvg_discard_broadcast_cnt;       // 汇聚模块丢弃广播报文总数
    HI_U32 cvg_transmit_broadcast_cnt;      // 汇聚模块转发广播报文总数
    HI_U32 cvg_deliver_app_cnt;             // 汇聚模块分发APP报文总数
    HI_U32 cvg_deliver_app_fail_cnt;        // 汇聚模块分发APP报文失败总数
    HI_U32 cvg_deliver_ip_cnt;              // 汇聚模块分发IP报文总数
    HI_U32 cvg_deliver_ip_size_fail_cnt;    // 汇聚模块分发IP报文报文超长失败总数
    HI_U32 cvg_deliver_ip_buffer_fail_cnt;  // 汇聚模块分发IP报文申请缓存失败总数
    HI_U32 cvg_deliver_ip_discard_broadcast_cnt;  // 汇聚模块分发IP报文丢弃广播报文总数
    HI_U32 cvg_deliver_nm_cnt;                    // 汇聚模块分发NM报文总数
    HI_U32 cvg_deliver_nm_fail_cnt;               // 汇聚模块分发NM报文失败总数
    HI_U32 cvg_deliver_invalid_type_cnt;          // 汇聚模块分发未知类型报文总数
    HI_U32 cvg_receive_transmit_cnt;              // 汇聚模块接收转发报文总数
    HI_U32 cvg_receive_transmit_fail_cnt;         // 汇聚模块接收转发报文失败总数
    HI_U32 cvg_receive_nm_cnt;                    // 汇聚模块接收NM报文总数
    HI_U32 cvg_receive_app_cnt;                   // 汇聚模块接收APP报文总数
    HI_U16 cvg_receive_app_no_buffer_cnt;         // 汇聚模块接收APP报文因缓存不足处理失败总数
    HI_U16 cvg_receive_app_queue_overflow_cnt;    // 汇聚模块接收APP报文队列溢出失败总数
    HI_U32 cvg_receive_ip_cnt;                    // 汇聚模块接收IP报文总数
    HI_U32 cvg_receive_ip_no_buffer_cnt;          // 汇聚模块接收IP报文因缓存不足失败总数
    HI_U32 cvg_handle_cnt;                        // 汇聚模块处理报文总数
    HI_U32 cvg_handle_transmit_cnt;               // 汇聚模块处理转发报文总数
    HI_U32 cvg_handle_nm_cnt;                     // 汇聚模块处理NM报文总数
    HI_U32 cvg_handle_ip_cnt;                     // 汇聚模块处理IP报文总数
    HI_U16 cvg_handle_app_cnt;                    // 汇聚模块处理APP报文总数
    HI_U16 cvg_handle_invalid_cnt;                // 汇聚模块处理未知类型报文总数
    HI_U32 cvg_handle_fail_cnt;                   // 汇聚模块处理报文失败总数
    HI_U32 cvg_handle_transmit_route_fail_cnt;    // 汇聚模块处理转发报文设置路由失败总数
    HI_U32 cvg_handle_nm_route_fail_cnt;          // 汇聚模块NM报文设置路由失败总数
    HI_U32 cvg_handle_app_route_fail_cnt;         // 汇聚模块APP报文设置路由失败总数
    HI_U32 cvg_handle_ip_route_fail_cnt;          // 汇聚模块IP报文设置路由失败总数
    HI_U32 cvg_segment_fail_cnt;                  // 汇聚模块分段处理失败总数
    HI_U16 pad;                                   // 汇聚模块分段生成MAC帧失败总数
    HI_U16 cvg_segment_split_mac_fail_cnt;        // 汇聚模块分段分割MAC失败总数
    HI_U32 cvg_segment_set_fc_fail_cnt;           // 汇聚模块分段设置FC失败总数
    HI_U32 cvg_segment_queue_overflow_cnt;        // 汇聚模块分段队列满失败总数
    HI_U32 segment_sof_cnt;                       // 分段模块接收报文总数
    HI_U32 sgm_mpdu_send_success_cnt;             // 分段模块因发送成功丢弃报文总数
    HI_U32 sgm_mpdu_send_fail_cnt;                // 分段模块因发送失败丢弃报文总数
    HI_U32 sgm_mpdu_send_timeout_cnt;             // 分段模块因报文超时丢弃报文总数
    HI_U32 sgm_mpdu_reset_discard_cnt;            // 分段模块因系统软重启丢弃报文总数
} HI_MAC_INTERFACE_STATISTICS_S;

typedef struct
{
    // 通道相关类统计量
    HI_U32 first_notify_time;     // 组网完成后第一次通知时标
    HI_U32 last_true_time;        // 最近一次通道改变为可用消息时标
    HI_U32 last_false_time;       // 最近一次通道改变为不可用消息时标
    HI_U32 chl_status;            // 当前通道状态
    HI_U16 chl_change_true;       // 通道可用次数
    HI_U16 chl_change_false;      // 通道不可用次数
    HI_U32 uart_rx_total_len[2];  // 串口接收字节数(发送消息/处理消息)
    HI_U16 uart_rx_ok_num[2];     // 串口接收完整帧数(发送消息/处理消息)
    HI_U32 uart_rx_last_time[2];  // 串口最近一次接收时间(发送消息/处理消息)
    HI_U32 uart_tx_total_len[2];  // 串口发送字节数(发送消息/处理消息)
    HI_U16 uart_tx_ok_num[2];     // 串口发送成功帧数（发送消息/处理消息）
    HI_U32 uart_tx_last_time[2];  // 串口最近一次发送时间(发送消息/处理消息)
    HI_U16 uart_tx_fail_num[2];   // 串口发送失败次数(发送消息/处理消息)
    HI_U32 uart_tx_err_cause[2];  // 串口最近一次发送失败原因(发送消息/处理消息)
    HI_U16 plc_rx_total_num[2];   // PLC接收帧次数(发送消息/处理消息)
    HI_U32 plc_rx_last_time[2];   // PLC最近一次接收时间(发送消息/处理消息)
    HI_U16 plc_rx_last_id[2];     // PLC最近一次接收ID(发送消息/处理消息)
    HI_U32 plc_rx_last_len[2];    // PLC最近一次接收包长度(发送消息/处理消息)
    HI_U16 plc_tx_total_num[2];   // PLC发送帧次数(发送消息/处理消息)
    HI_U32 plc_tx_last_time[2];   // PLC最近一次发送时间(发送消息/处理消息)
    HI_U16 plc_tx_last_id[2];     // PLC最近一次发送ID(发送消息/处理消息)
    HI_U32 plc_tx_last_len[2];    // PLC最近一次发送包长度(发送消息/处理消息)
    HI_U16 plc_tx_fail_num[2];    // PLC发送失败次数(发送消息/处理消息)
    HI_U32 plc_tx_fail_len[2];    // PLC发送最近一次失败包长度(发送消息/处理消息)
    HI_U32 plc_tx_err_cause[2];   // PLC发送最近一次失败原因(发送消息/处理消息)
}HI_MRS_INTERFACE_STATISTICS_S;    // MRS 外部接口统计量

//
// 端到端 接口级别 包括 MRS-IP-MAC-PHY
//
typedef struct
{
    HI_DSID_SDM_HRD_S hrd;

    // ETH 收发计算
    // --> *收计数(中断), 发送(任务)
    // TCP接收计数, TCP发送给MAC计数
    // SOCKET连接个数

    // ETH 收发计算
    // --> ,
    HI_U32 ulEthRxCnt; //*收计数(中断)
    HI_U32 ulEthTxCnt;// 发送包(任务)
    HI_U32 ulTcpRxCnt; // TCP接收计数
    HI_U32 ulTcpTxCnt;// TCP发送计数

    HI_U32 ulTcpInErrCnt; //接收到的TCP包解析不正确
    HI_U32 ulIpFragFailsCnt; //ip分片错误。

    HI_U32 ulTcpEstabResetsCnt; //连接rese次数。
    HI_U32 ulTcpListenCnt;   //连接次数。
    HI_U32 ulTcpAcceptCnt;   //
    HI_U32 ulTcpLinkFailCnt;

    // MAC收发计数
    // *IP包接收个数, 分段计数, 发送给PHY计数
    HI_MAC_INTERFACE_STATISTICS_S stMacHLInfo; // MAC 一级统计信息,侧重通路
    HI_U32 eth_to_mac_cnt;
    HI_U32 eth_to_cvg_cnt;              //以太网给汇聚的IP包数
    HI_U32 tx_ip_packet_cnt;            //汇聚接收以太网的IP包数
    HI_U32 sem_to_tx_cnt;               //分段通知发送机的数
    HI_U32 short_packet_cnt;            //发送机发短包数
    HI_U32 long_packet_cnt;             //发送机发长包数
    HI_U32 tx_end_itrupt_cnt;           //发送完成中断数
    HI_U32 sof_sack_rcv_cnt;            //收到对端的sack数

    // *发送IP包个数, PHY接收个数(OK/ERR)
    HI_U32 fc_ok_cnt;                   //fc正确数
    HI_U32 fc_err_cnt;                  //fc错误数
    HI_U32 pb_ok_cnt;                   //pb正确数
    HI_U32 pb_err_cnt;                  //pb错误数
    HI_U32 rx_beacon_cnt;               //收到beacon数
    HI_U32 rx_end_itrupt_cnt;           //接收完成中断数
    HI_U32 rx_to_rsb_cnt;               //接收机通知重给数
    HI_U32 rx_ip_packet_cnt;            //汇聚发送给以太网的IP包数
    HI_U32 cvg_to_eth_cnt;              //汇聚通知以太网的事件数
    
    // MRS 外部接口统计量
    HI_MRS_INTERFACE_STATISTICS_S mrs_chl_info;
} HI_DSID_SDM_CHL_S;

typedef struct
{
    HI_DSID_SDM_HRD_S hrd;

    // TODO: 业务现场关键信息定义
    HI_U32 rx_fc_cnt;
    HI_U32 rx_fc_crc_success_cnt;
    HI_U32 rx_fc_crc_err_cnt;
    HI_U32 rx_false_frame;
    HI_U32 rx_beacon_fc_recv_cnt;
    HI_U32 rx_beacon_fc_discard_cnt;
    HI_U32 rx_beacon_pb_crc_err_cnt;
    HI_U32 rx_beacon_pb_crc_success_cnt;
    HI_U32 rx_sof_broadcast_cnt;
    HI_U32 rx_sof_unicast_cnt;
    HI_U32 rx_sof_resend_cnt;
    HI_U32 rx_sof_136_pb_crc_err_cnt;
    HI_U32 rx_sof_136_pb_crc_success_cnt;
    HI_U32 rx_sof_520_pb_crc_err_cnt;
    HI_U32 rx_sof_520_pb_crc_success_cnt;
    HI_U32 detect_nbi_cnt;
    HI_U16 nbi_start[3];
    HI_U16 nbi_end[3];
    HI_U16 imp_noise_cnt;
    HI_U16 rsvd;
} HI_DSID_SDM_RX_S;


typedef struct
{
    HI_DSID_SDM_HRD_S hrd;

    // TODO: 业务现场关键信息定义
    HI_U32 tx_sof_unicast_cnt;
    HI_U32 tx_sof_broadcast_cnt;
    HI_U32 tx_sof_resend_cnt;
    HI_U32 tx_sof_136_pb_cnt;
    HI_U32 tx_sof_520_pb_cnt;
    HI_U32 tx_tdma_refresh_evt_cnt;
    HI_U32 tx_mpdu_queued_in_evt_cnt;
    HI_U32 tx_period_timeout_evt_cnt;
    HI_U32 tx_sack_received_evt_cnt;
    HI_U32 tx_sound_ack_received_evt_cnt;
    HI_U32 tx_backoff_end_evt_cnt;
    HI_U32 tx_vcs_state_changed_evt_cnt;
    HI_U32 tx_wait_response_timeout_evt_cnt;
    HI_U32 tx_send_success_evt_cnt;
    HI_U32 tx_buffer_clear_evt_cnt;
    HI_U32 tx_multi_send_evt_cnt;
    HI_U32 tx_mem_malloc_fail_cnt;
    HI_U32 tx_get_slot_fail_cnt;
    HI_U32 tx_send_vcs_busy_cnt;
    HI_U32 tx_send_all_queue_null_cnt;
    HI_U32 tx_send_queue_null_cnt;
    HI_U32 tx_start_hr_timer_fail_cnt;
    HI_U32 tx_stop_hr_timer_fail_cnt;

    HI_U32 tx_get_all_msdu_cnt;                   // 发送机获取报文总数
    HI_U32 sof_wait_sack_send_success;            // 等待SACK SOF发送成功个数
    HI_U32 sof_not_wait_sack_send_success;        // 不需要等待SACK报文发送成功个数
    HI_U32 sof_resend_exclude_fix_resend;         // 报文重传个数，不包括固定重传

    /* BEGIN: Added by zhangguobin, 2015/6/25   问题单号:双模-可维可测 */
    HI_U32 beacon_enqueue_cnt;          // BEACON帧入队列个数
	HI_U32 sof_enqueue_cnt;             // SOF帧入队列个数
    HI_U32 sack_enqueue_cnt;            // SACK帧入队列个数
    HI_U32 link_test_cnt;               // 链路测试报文队列个数

	HI_U32 send_beacon_cnt;             // 发送BEACON帧个数
	HI_U32 send_sof_cnt;                // 发送SOF帧个数
	HI_U32 send_sack_cnt;               // 发送SACK帧个数
	HI_U32 send_short_beacon_cnt;       //发送短beacon报文个数
    HI_U32 send_link_test_cnt;          //发送链路测试报文个数

	HI_U32 send_beacon_fragment_cnt;	// 发送BEACON帧分片个数
    HI_U32 send_sof_fragment_cnt;		// 发送SOF帧分片个数

	HI_U32 backoff_success_cnt;			// 退避成功次数
	HI_U32 backoff_fail_cnt;			// 退避失败次数

    HI_U32 send_timeout_cnt;            // 发送超时次数
    HI_U16 over_queue_threshold_cnt;    // 超过队列水线报文丢弃总计数;
    HI_U16 packet_timeout_err_cnt;      // 报文超时错误

    HI_U32 send_command_cnt;            // 发送命令次数
    HI_U32 write_tx_buf_cnt;            // 写发送缓存次数
    HI_U32 sack_timout_cnt;             // SACK超时计数
    HI_U32 send_beacon_pb_fail_cnt;     //发送beacon pb块失败计数

    HI_U16 write_tx_buf_fail;           // 写BUFF失败计数
    HI_U8  rf_chip_reset_cnt;           // RF芯片重启计数
    HI_U8  rf_send_timeout_reset_cnt;   // 发送超时重启计数

    HI_U32 rcv_beacon_cnt;              // 接收BEACON帧个数
	HI_U32 rcv_sof_cnt;                 // 接收SOF帧个数
	HI_U32 rcv_short_beacon_cnt;        // 接收短beacon报文个数
	HI_U32 rcv_link_test_cnt;           // 接收短链路检测报文个数

	HI_U32 rcv_beacon_fragment_cnt;		// 接收BEACON帧分片个数
	HI_U32 rcv_sof_fragment_cnt;		// 接收SOF帧分片个数

    HI_U32 beacon_crc_fail_cnt;         // BEACON帧校验失败个数
    HI_U32 short_beacon_fail_cnt;       // 短BEACON帧失败个数
    HI_U32 link_test_fail_cnt;          // 短链路检测报文失败个数

	HI_U32 rcv_incomplete_cnt;          // 接收不完整报文个数
	HI_U32 rcv_beacon_incomplete_cnt;   // 接收信标帧不完整个数
	HI_U32 rcv_invalid_cnt;             // 接收异常报文

    HI_U32 interrupt_cnt;               // 中断个数
	HI_U32 preamble_interrupt_cnt;		// 前导中断个数
	HI_U32 send_complete_interrupt_cnt;	// 发送完成中断个数
	HI_U32 rcv_interrupt_cnt;			// 接收中断个数
    HI_U32 error_cmd_cnt;               // 错误命令次数
    HI_U32 fifo_flow_cnt;               // 收发缓存溢出次数
	HI_U32 rcv_sof_incomplete_cnt;      // 接收信标帧不完整个数
    HI_U32 rcv_crc_fail_cnt;            // 中断上报crc错误个数
    HI_U32 rcv_rebuild_fail;
    HI_U32 rcv_rebuild_incomplete;
    HI_U32 rcv_return_sack_packe;
    HI_U32 rcv_pkt_length_err;			 // 报文长度错误
    HI_U16 rcv_pkt_size_too_short_err;
    HI_U16 rcv_pkt_size_too_long_err;
    HI_U32 rcv_no_self_pkt_cnt;

	HI_U8  switch_to_rf_by_blink_check_cnt;			// 由于盲检切换到RF模式的次数
	HI_U8  switch_to_rf_by_low_comm_rate_cnt;		// 由于低通信率切换到RF模式的次数
	HI_U8  switch_to_plc_by_not_join_network_cnt;	// 由于RF无法加入网络切换到PLC模式次数
	HI_U8  switch_to_plc_by_not_rcv_beacon_cnt;		// 由于RF无法接收到信标帧切换到PLC模式次数

    HI_U8  switch_to_plc_by_rf_link_exception;      // 由于RF链路通信异常切换到PLC模式次数
    HI_U8  network_mode_switch_cmd_cnt;             // 网络模式切换命令切换次数
    HI_U16  rf_rout_evaluation_num;                 //RF上路由评估次数

    HI_U32 rcv_self_snid_short_beacon_cnt;          //接收到自己网络短信标个数

	HI_U16 sub_network_snid_conflict_cnt;			// 子网络号冲突次数
	HI_U16 neighbor_network_snid;					// 邻居网络号
    /* END:   Added by zhangguobin, 2015/6/25 */
    HI_U32 rf_mode_change_comm_rate[12];

	HI_U8  switch_mf_to_plc_by_high_comm_rate_cnt;
	HI_U8  pad[3];
} HI_DSID_SDM_TX_S;

typedef struct
{
    HI_DSID_SDM_HRD_S hrd;

    // TODO: 业务现场关键信息定义
    HI_U32 bcn_slot_table_update_cnt;
    HI_U32 bcn_last_update_slot_table_time;
    HI_U32 bcn_last_update_slot_table_bpc;
    HI_U32 bcn_last_rcv_beacon_time;
    HI_U32 bcn_last_rcv_beacon_bpc;
    HI_U32 bcn_discard_beacon_slot_cnt;
    HI_U32 bcn_discard_bind_csma_slot_cnt;
    HI_U32 bcn_discard_csma_slot_cnt;
    HI_U32 bcn_discard_tdma_slot_cnt;
    HI_U32 bcn_give_beacon_slot_cnt;
    HI_U32 bcn_give_bind_csma_slot_cnt;
    HI_U32 bcn_give_csma_slot_cnt;
    HI_U32 bcn_give_tdma_slot_cnt;
    HI_U32 bcn_get_repeat_slot_cnt;
    HI_U32 bcn_give_oneself_slot_cnt;
    HI_U32 tx_get_slot_cnt;
    HI_U32 tx_get_slot_fail_cnt;
    HI_U32 phs_evaluate_a_cnt;
    HI_U32 phs_evaluate_b_cnt;
    HI_U32 phs_evaluate_c_cnt;
    HI_U32 synch_timeout_reset_cnt;
    HI_U32 exception_reset_cnt;
    HI_U32 exception_jump_reset_cnt;
    HI_U32 cylce_exception_reset_cnt;
    HI_U32 freqerr_exception_cnt;
    HI_U32 save_freqerr_success_cnt;
    HI_U32 save_freqerr_fail_cnt;
    HI_U32 soft_reset_cnt;
    HI_S16 freqerr_current;
    HI_U16 synch_tei;
    HI_U32  synch_snid;
    HI_U32 chip_state1_cnt;
    HI_U32 chip_state2_cnt;
    HI_U32 chip_state3_cnt;
    HI_U32 freqerr_cnt[36];
} HI_DSID_SDM_BCN_S;

typedef struct _dfx_switch_freq_info_
{
    HI_U16 freq_total_cnt;
    HI_U16 reason;
    HI_U32 last_time;       //本频段持续时间
}dfx_switch_freq_info;

typedef struct _dfx_switch_snid_info_
{
    HI_U16 snid_total_cnt;
    HI_U16 status;
    HI_U32 fc_success_cnt;
    HI_U32 beacon_pb_success_cnt;
    //HI_U32 sof_pb_success_cnt;
}dfx_switch_snid_info;

typedef struct _dfx_rt_info_
{
    HI_U16 rt_discover_station_cnt;        // 发现站点个数
    HI_U16 rt_near_station_cnt;            // 离CCO更近发现站点个数
    HI_U16 rt_upper_comm_rate_station_cnt; // 通信率上限站点个数
    HI_U16 rt_middle_comm_rate_station_cnt;// 通信率上下限中间站点个数
    HI_U16 rt_lower_comm_rate_station_cnt; // 通信率下限站点个数
    HI_U16 rt_rcv_discover_pk_cnt;         // 接收发现列表个数（1个窗口）
    HI_U16 rt_rcv_bea_cnt;         // 接收信标个数（1个窗口）
    HI_U8   rt_proxy_up_comm_rate;           // 与代理站点上行通信率
    HI_U8   rt_proxy_down_comm_rate;         // 与代理站点下行通信率
    HI_U16 rt_rcv_proxy_beacon_cnt;        // 接收代理站点信标帧个数（1个窗口）
    HI_U16 rt_rcv_proxy_discover_pk_cnt;   // 接收代理站点发现列表个数（1个窗口）
    HI_U8   proxy_mac[6];
    HI_U8   chose_proxy_mac[6];
    HI_U16   change_proxy_reason;
    HI_BOOL proxy_is_static;
    HI_U8       rsvd;
}dfx_rt_info;


#define DFX_RT_INDEX_MAX 10
/* BEGIN: Deleted by likunhe/00194117, 2014/6/10   问题单号:DTS2014060603580*/
//#define FREQ_TRY_VALID_NUM          3           //实际可用频段数
/* END: Deleted by likunhe/00194117, 2014/6/10 */

//代理变更原因比特位
enum
{
    PROXY_RATE_IS_LOW = 1<<0,
    PROXY_RATE_IS_MIDDLE = 1<<1,
    PROXY_RATE_IS_UPPER = 1<<2,
    PROXY_IS_FAR_TO_CCO = 1<<3,
    MULTI_PROXY_COMMRATE_BAD = 1<<4,
    HALF_WINDOW_COMM_BAD  = 1<<5,
    NEW_PROXY_LEVEL_IS_SMALL  = 1<<6,
    NEW_PROXY_IS_STA = 1<<7,
    NEW_PROXY_RATE_IS_BIG = 1<<8,
    CHANGE_TO_STATIC_PROXY = 1<<9,
};


typedef struct
{
    HI_DSID_SDM_HRD_S hrd;

    HI_U32  switch_freq_last_systime;         // 最后一次切频段系统时间
    dfx_switch_freq_info  switch_freq_info[FREQ_TRY_NUM];// 频段切换记录

    HI_U8   freq_now;                         // 当前频段
    HI_U8   freq_last;                        // 上次频段
    HI_U16  last_change_reason;               // 上次切换频段原因
    HI_U32  switch_snid_last_systime;         // 最后一次切换SNID系统时间

    dfx_switch_snid_info  switch_snid_info[15];// SNID切换记录

    HI_U32  join_first_systime;               // 第一次关联请求系统时间
    HI_U32  join_last_systime;                // 入网系统时间

    HI_U16  not_bigger_enough_bpc_cnt;          // 无更大bpc来关联请求次数
    HI_U16  rcv_bea_period_count;               // 接收几个beacon周期的信标
    HI_U16  chose_proxy_fail_cnt;               // 选择代理失败次数
    HI_U16  assoc_req_cnt;                      // 关联请求次数
    HI_U16  cco_reject_cnt;                     // CCO拒绝的次数
    HI_U16  cco_reject_reason;                  // CCO拒绝的原因

    HI_U32   mac_reset_cnt;                     // MAC软重启次数

    HI_U16  join_not_white_cnt;            // 不在白名单请求次数
    HI_U16  join_level_overflow_cnt;       // 级数过多次数
    HI_U16  join_proxy_overflow_cnt;       // 代理过多次数
    HI_U16  join_sta_overflow_cnt;         // 子站点过多次数
    HI_U16  join_total_overflow_cnt;       // 总数过多次数
    HI_U16  join_repeat_mac_cnt;           // 重复mac次数
    HI_U16  join_illege_proxy_cnt;         // 代理不存在次数
    HI_U16  join_end_sequence_err;         // end_sequence出错次数

    HI_U32  cco_recv_req_cnt;              // CCO收到请求总次数

    DIAG_CMD_SOFT_RESET_STRU soft_reset;

    HI_U32 nm_handle_assoc_req_cnt;
    HI_U32 nm_handle_assoc_cnf_cnt;
    HI_U32 nm_create_assoc_ind_01_cnt;
    HI_U32 nm_create_assoc_ind_03_cnt;
    HI_U32 nm_create_change_proxy_cnf_cnt;
    HI_U32 nm_handle_change_proxy_cnf_cnt;
    HI_U32 dfx_ndm_handle_hso_cmd_callback_cnt;
    HI_U32 dfx_ndm_pipe_output_cnt;
    HI_U32 dfx_ndm_packet_frame_copy_payload_list_cnt;
    HI_U32 dfx_ndm_handle_packet_cnt;
    HI_U32 dfx_ndm_pipe_send_ack_back_cnt;
    HI_U32 dfx_ndm_pipe_fragment_output_cnt;
    HI_U32 dfx_ndm_fill_reply_payload_cnt;
    HI_U32 Ndc_DRemoteSend_cnt;
    HI_U32 nm_write_nv_fail;

    HI_U8  cco_mac[HI_PLC_MAC_ADDR_LEN];            // 锁定网络的CCO MAC地址
    HI_U8  lock_status;                             // 锁定状态
    HI_U8  lock_status_identify;                    // 台区识别功能下锁定状态

    HI_U32 lock_begin_time;                         // 锁定开始时间
    HI_U32 lock_duration;                           // 锁定时长
    HI_U32 reject_time;                             // 锁定网络拒绝时间

    HI_U16 lock_cnt;                                // 锁定次数
    HI_U16 reject_cnt;                              // 锁定网络拒绝次数
    HI_U16 lock_timeout_cnt;                        // 锁定超时此时
    HI_U16 lock_fail_by_not_join_cnt;               // 未加入网络锁定失败
    HI_U16 send_msg_fail_cnt;                       // 消息发送失败
    HI_U16 unlock_cnt_by_identify;                  // 台区识别解锁次数(维测)

    HI_U8  lock_freq_identify;                      // 锁定网络频段号
    HI_U8  lock_snid_identify;                      // 锁定网络网络号
    HI_U8  cco_mac_identify[HI_PLC_MAC_ADDR_LEN];   // 台区识别锁定网络的CCO MAC地址

    HI_U32 lock_begin_time_by_identify;             // 锁定开始时间，单位S
    HI_U32 lock_freqsnid_duration_by_identify;      // 锁定时长，单位S
    HI_U32 lock_ccomac_duration_by_identify;        // 锁定CCO MAC时长，单位S
    HI_U32 unlock_time_by_identify;                 // 解锁时间(维测)
    HI_U16 lock_freqsnid_cnt_by_identify;           // 台区识别锁定频段和网络号次数(维测)
    HI_U16 lock_ccomac_cnt_by_identify;             // 台区识别锁定CCO MAC次数(维测)

    HI_U32 lock_freq : 8;                           // 锁定网络的频段
    HI_U32 lock_snid : 24;                          // 锁定网络的SNID
} HI_DSID_SDM_NM_S;

typedef struct
{
    HI_DSID_SDM_HRD_S hrd;

    dfx_rt_info  rt_dfx_info[DFX_RT_INDEX_MAX];
    HI_U8    rt_dfx_index;
    HI_U8    rsvd2[2];
    HI_U8   my_static_proxy_exist;
    HI_U8   static_proxy_mac[HI_PLC_MAC_ADDR_LEN];
    HI_U16 rt_fast_proxy_change_cnt;       // 快速触发路由变更次数
    HI_U16 rt_window_proxy_change_cnt;     // 整窗口变更次数
    HI_U16  lock_static_proxy_cnt;
    HI_U16  unlock_static_proxy_cnt;
    HI_U16  choose_static_proxy_fail_cnt;
    HI_U16  rf_choose_proxy_fial_cnt;
    HI_U16  pad;
} HI_DSID_SDM_RT_S;

typedef struct
{
    HI_DSID_SDM_HRD_S hrd;

    // MRS模块内部相关统计量
    HI_U8 cco_status;                 // CCO运行状态(正常、随抄、搜表、映射表)
    HI_U8 sta_status;                 // STA运行状态(初始化、正常)
    HI_U8 route_ctrl;                 // 路由控制状态
    HI_U8 vm_flg;                     // 虚拟表状态
    HI_U32 vm_change_num;             // 虚拟表改变次数
    HI_U32 map_ready_time;            // 映射表维护完成时标
    HI_U32 heart_num;                 // 心跳次数
    HI_U32 heart_last_time;           // 上一次心跳时标
    HI_U32 heart_timeout_num;         // 心跳超时次数
    HI_U32 heart_timeout_time;        // 上一次心跳超时时标
    HI_U32 meter_num;                 // 表档案数量
    HI_U32 LR_req_cnt;                // 轮抄询问读表次数
    HI_U32 LR_ack_cnt;                // 轮抄应答读表次数
    HI_U32 LR_plc_tx;                 // 轮抄PLC发送次数
    HI_U32 LR_plc_rx;                 // 轮抄PLC接收次数
    HI_U32 LR_meter_tx;               // 轮抄电表发送次数
    HI_U32 LR_meter_rx;               // 轮抄电表接收次数
    HI_U32 LR_plc_timeout;            // 轮抄PLC超时次数
    HI_U32 LR_ok_cnt;                 // 轮抄成功次数
    HI_U32 LR_fail_cnt;               // 轮抄失败次数
    HI_U32 XR_uart_rx;                // 随抄串口接收次数(CCO为接收读表帧; STA为接收电表帧)
    HI_U32 XR_uart_tx;                // 随抄串口发送次数(CCO为发送读表帧; STA为发送电表帧)
    HI_U32 XR_plc_tx;                 // 随抄PLC发送次数
    HI_U32 XR_plc_rx;                 // 随抄PLC接收次数
    HI_U32 XR_nak_num;                // 随抄否认次数
    HI_U8 XR_nak[4];                  // 最近4次否认原因
    HI_U32 XR_plc_timeout;            // 随抄PLC超时次数
    HI_U32 XR_ok_cnt;                 // 随抄成功次数
    HI_U32 XR_fail_cnt;               // 随抄失败次数
    HI_U32 PR_uart_rx;                // 并发串口接收次数(CCO为接收读表帧; STA为接收电表帧)
    HI_U32 PR_uart_tx;                // 并发串口发送次数(CCO为发送读表帧; STA为发送电表帧)
    HI_U32 PR_plc_tx;                 // 并发PLC发送次数
    HI_U32 PR_plc_rx;                 // 并发PLC接收次数
    HI_U32 PR_pool_num;               // 并发池元素个数
    HI_U32 PR_nak_num;                // 并发否认次数
    HI_U8 PR_nak[4];                  // 最近4次否认原因
    HI_U32 PR_plc_timeout;            // 并发PLC超时次数
    HI_U32 PR_ok_cnt;                 // 并发成功次数
    HI_U32 PR_fail_cnt;               // 并发失败次数
    HI_U32 msg_curr_num;              // 当前消息队列深度
    HI_U32 msg_max_num;               // 最大消息队列深度
    HI_U32 msg_max_num_time;          // 最大消息队列深度发生时间
    HI_U32 msg_total_tx_cnt;          // 消息总发送次数
    HI_U32 msg_total_rx_cnt;          // 消息总接收次数
    HI_U32 msg_last_tx_time;          // 上一次消息发送时间
    HI_U32 msg_last_rx_time;          // 上一次消息接收时间
    HI_U32 msg_last_tx_id;            // 上一次消息发送ID
    HI_U32 msg_last_rx_id;            // 上一次消息接收ID
    HI_U32 msg_tx_fail_cnt;           // 消息发送失败次数
    HI_U32 msg_max_delay;             // 最大的消息处理间隔
    HI_U32 msg_max_delay_id;          // 发生最大消息处理间隔时的ID
    HI_U32 msg_max_delay_time;        // 发生最大消息处理间隔时标
    HI_U32 msg_max_delay_queue_num;   // 发生最大消息处理间隔时队列深度
    HI_U32 vchl_change_num;           // 模拟通道改变次数
    HI_U8 vchl_status[4];             // 模拟通道状态(含历史状态)
    HI_U32 vchl_last_change_time[4];  // 模拟通道状态改变时刻(历史)
    HI_U8 rm_queue_curr_num;          // 读表队列当前深度
    HI_U8 rm_queue_max_num;           // 读表队列最大深度
    HI_U16 rm_queue_first_id;         // 读表队列队首ID
    HI_U32 rm_queue_lock_time;        // 读表队列队首锁定时间
    HI_U32 rm_queue_max_lock_time;    // 读表队列队首最大锁定时间
    HI_U32 timer_fail_cnt;            // 定时器启动失败次数
    HI_U32 timer_fail_val[4];         // 最近4次定时器启动失败时的定时值
    HI_U32 timer_fail_id[4];          // 最近4次定时器启动失败时的定时器ID
    HI_U32 timer_val_zero_cnt;        // 定时器定时值为0的次数
    HI_U32 timer_val_zero_id[4];      // 最近4次定时器值为0的定时器ID
    HI_U32 timer_val_zero_time[4];    // 最近4次定时器值为0的发生时刻
    // TODO: 业务现场关键信息定义
} HI_DSID_SDM_MR_S;

typedef struct
{
    HI_DSID_SDM_HRD_S hrd;

    // TODO: 业务现场关键信息定义
} HI_DSID_SDM_BOOT_S;


typedef struct
{
    HI_DSID_SDM_HRD_S hrd;
    HI_UPG_FILE_VER_S st_code_ver;      //代码版本信息
    HI_UPG_FILE_VER_S st_nv_ver;        //NV版本信息
    HI_U32 ul_boot_ver[2];              //BOOT版本信息
    HI_DSID_DRV_UART_INFO_S st_uart[4];
    HI_U32 ul_ir_rx_cnt;                //IR接收字符个数
    HI_U32 ul_ir_tx_cnt;                //IR发送字符个数
    HI_U32 ul_ir_parity_err_cnt;        //IR接收数据奇偶校验错误计数
    HI_U32 ul_ir_frame_err_cnt;         //IR 接收数据帧格式错误计数
    HI_U32 ul_ir_overrun_err_cnt;       //IR 接收FIFO溢出错误计数
    HI_U32 ul_ir_buffer_full_cnt;    //IR 接收BUFFER满错误计数
    HI_U32 ul_eth_lisr_int_cnt;         //ETH中断计数
    HI_U32 ul_eth_lisr_int_fault;       //ETH中断错误计数
    HI_U32 ul_eth_rx_int_cnt;           //ETH 接收中断计数
    HI_U32 ul_eth_tx_int_cnt;           //ETH 发送中断计数
    HI_U32 ul_eth_pkt_feed_cnt;         //ETH 硬件buffer填充计数
    HI_U32 ul_eth_pkt_release_cnt;      //ETH 硬件buffer 释放计数
    HI_U32 ul_eth_pkt_alloc_fail_cnt;   //ETH 接收buffer 申请失败计数
    HI_U32 ul_eth_pkt_jobadd_err_cnt;   //ETH JOBADD处理接收包出错计数
    HI_U32 ul_eth_pkt_rx_len_err_cnt;   //ETH 接收帧长错误计数
    HI_U32 ul_eth_tx_not_ready_cnt;     //ETH 由于太忙而不能及时发送的次数
    HI_U32 ul_wdg_tick_cnt;             //看门狗踢狗计数
    HI_DSID_SAL_HSO_GRAB_CONNECT_INFO_S st_hso_grab_connect[HI_DSID_SAL_HSO_GRAB_CONNECT_INFO_COUNT];//HSO抢占：抢占双方的IP地址和时间
    HI_U32 ulHtcpTempAddr;//HSO抢占:中间态被抢占地址
    HI_U16 us_hso_grab_count;//HSO抢占总次数
    HI_U16 rst_data;
    HI_U32 ul_plc2eth_err_cnt;     //
    HI_U32 ul_eth2plc_err_cnt;     //
    HI_U16 us_cipher_int_cnt;       //cipher中断次数计数
    HI_U16 us_cipher_errbus_cnt;    //cipher总线错误计数
    HI_U16 us_cipher_errlen_cnt;    //cipher长度错误计数
    HI_U16 us_cipher_errcb_cnt;     //用户回调函数错误次数
} HI_DSID_SDM_BSP_S;

typedef struct
{
    HI_DSID_SDM_HRD_S hrd;

    // TODO: 业务现场关键信息定义
} HI_DSID_SDM_EVT_MAC_S;

typedef struct
{
    HI_DSID_SDM_HRD_S hrd;

    // TODO: 业务现场关键信息定义
} HI_DSID_SDM_EVT_MR_S;

// cmd: HI_DSID_SDM_DUMP REQ
typedef struct
{
    HI_U16 usCheckSum;
    HI_U16 usProdType;
    HI_U8 ucSdmId;
    HI_U8 ucOpt;
    HI_U8 ucMac[6];//远程导出时目标mac地址
    HI_U32 ulAddr;//导出内容的FLASH起始地址
    HI_U32 ulSize;//导出内容的大小,全FF表示清空对应区域
} HI_SDM_DATA_DUMP_REQ_S;

typedef struct
{
    HI_U16 usCheckSum;
    HI_U16 usRc;//错误码
    HI_U32 ulAddr;//应答包内容对应FLASH地址
    HI_U32 ulSize;//应打败内容对应FLASH大小
    HI_U32 bFinish:1;//导出完成
    HI_U32 reserved:31;
    HI_U8  aucData[0];//数据
} HI_SDM_DATA_DUMP_IND_S;

typedef struct
{
    HI_U32 timestamp;
} HI_SDM_KLG_OS_S;

typedef enum
{
    HI_SDM_TEST_FRME_SWT_NORMAL = 0,
    HI_SDM_TEST_FRME_SWT_ON = 0x01,
    HI_SDM_TEST_FRME_SWT_OFF = 0x02
} HI_SDM_TEST_FRME_SWT_E;

//需要数据包特征注册
typedef struct
{
    HI_U32 key_event;           //HI_SDM_ENG_EVT_CAUSE_E中，目前使用HI_SDM_ENG_EVT_CAUSE_MAC_DISCARD_MSDU
    HI_U32 feature_value[2];    //报文特征字段的值，对应offset可设置2个
    HI_U16 feature_offset[2];   //报文特征字段偏移，从msdu头起的offset；可设置2个
    HI_U8  feature_size[2];     //报文特征字段的大小，最大可设置为4
    HI_U16 save_offset[2];      //要保存的报文中的字段偏移，从msdu头起的offset；可设置2个
    HI_U8  save_size[2];        //要保存的报文中的字段的大小，对应offset可设置2个；两个值之和不超过6(字节)
}HI_SDM_RegFeature_S;
//*****************************************************************************

//*****************************************************************************


#define HI_SDM_ENG_MRS_FLASH_SIZE       (PRODUCT_CFG_CCO_MRLOG_SECTOR_NUM*PRODUCT_CFG_FLASH_BLOCK_SIZE)

#if ((defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) || defined(PRODUCT_CFG_PRODUCT_TYPE_STA)) && defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG) || defined(PRODUCT_CFG_HSO))


#define HI_SDM_ENG_LRPR_IN_DAY          (24)    // 轮抄并发抄抄读数据统计次数，每小时统计1次，每天共24次
#define HI_SDM_ENG_METER_ADDR_LEN       (3)     // 存储表地址的长度，只存储末3字节
#define HI_SDM_ENG_FRAME_LEN_MAX        (31)    // 抄读失败时存储报文的最大长度，32字节
#define HI_SDM_ENG_METER_REC_MAX        (50)    // 记录电表的最大个数，每个存储项限定不超过50个。
                                                // 日冻结抄读失败的电表上限50个，每时段抄读成功率低于门限的表上限50个
#define HI_SDM_ENG_LOW_RATIO_IN_DAY     (6)     // 低于抄读门限电表的次数，每4小时为1个时段，每天6次

#define MRS_DAY_FREEZE_FAIL_TIMEOUT     0x01   // 日冻结数据抄读超时
#define MRS_DAY_FREEZE_FAIL_ADDRERR     0x02   // 日冻结数据抄读地址错误
#define MRS_DAY_FREEZE_FAIL_DENY        0x04   // 日冻结数据抄读返回否认帧
#define MRS_DAY_FREEZE_FAIL_INVALID     0x08   // 日冻结数据抄读数据内容非法
#define MRS_DAY_FREEZE_FAIL_CLOCKERR    0x10   // 日冻结数据抄读时钟错误
/* BEGIN: liuxipeng/KF54842 2014/2/20   问题单号: DTS2014022003115 */
#define MRS_DAY_FREEZE_FAIL_NO_JOIN_NET 0x20   // 日冻结数据抄读超时-未入网
/* END  : liuxipeng/KF54842 2014/2/20   问题单号: DTS2014022003115 */

#if (defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) || defined(PRODUCT_CFG_HSO))
typedef enum _HI_SDM_ENDG_MRS_STATS_TYPE_E_
{
    SDM_MRS_MR_STATS_TYPE_XR,
    SDM_MRS_MR_STATS_TYPE_LR,
    SDM_MRS_MR_STATS_TYPE_PR
}HI_SDM_ENG_MRS_MR_STATS_TYPE_E;

typedef struct
{
    HI_U8  statsType;   // 统计的抄读类型：1-轮抄 2-并发抄
    HI_U8  valid;       // 有效性：1-有效，其它-无效
    HI_U16 meterFileCnt;// 表档案中电表数量
    HI_U16 comRx;       // 串口接收报文总次数
    HI_U16 comTx;       // 串口发送报文总次数
    HI_U16 plcRx;       // PLC接收抄表总次数
    HI_U16 plcTx;       // PLC发送抄表总次数
} HI_SDM_ENG_MRS_MR_STATS_S;

typedef struct
{
    HI_U8  statsType;   // 统计的抄读类型：0-透抄
    HI_U8  valid;       // 有效性：1-有效，其它-无效
    HI_U16 pad;         // 填充字段
    HI_U16 comRx;       // 串口接收报文总次数
    HI_U16 comTx;       // 串口发送报文总次数
    HI_U16 plcRx;       // PLC接收抄表总次数
    HI_U16 plcTx;       // PLC发送抄表总次数
} HI_SDM_ENG_MRS_XR_STATS_S;

typedef struct
{
    HI_SDM_ENG_MRS_MR_STATS_S stats[HI_SDM_ENG_LRPR_IN_DAY];
} HI_SDM_ENG_MRS_LRPR_STATS_S;


typedef struct
{
    HI_U8 meter[HI_SDM_ENG_METER_ADDR_LEN];     // 电表地址末3字节
    HI_U8 ratio;                                // 抄读成功率整数部分，98.76%记录为98
} HI_SDM_ENG_MRS_LOW_RATIO_ITEM_S;

typedef struct
{
    HI_U8 valid;            // 有效性：1-有效，其它-无效
    HI_U8 pad;              // 填充字段
    HI_U16 meterCount;      // 抄读成功率低于门限电表数量

    // 抄读成功率低于门限电表信息，不超过50个时，按实际情况记录；超过50个时，只记录50个电表信息
    HI_SDM_ENG_MRS_LOW_RATIO_ITEM_S item[HI_SDM_ENG_METER_REC_MAX];
} HI_SDM_ENG_MRS_LOW_RATIO_PART_S;

typedef struct
{
    // 按时段统计的抄读成功率低于门限电表信息，每天分6个时段，每时段4小时。
    HI_SDM_ENG_MRS_LOW_RATIO_PART_S part[HI_SDM_ENG_LOW_RATIO_IN_DAY];
} HI_SDM_ENG_MRS_LOW_RATIO_S;

typedef struct
{
    HI_U16 total_ratio;  // 总体抄读成功率百分比，9876表示成功率98.76%
    HI_U16 meter_ratio;  // 抄读成功电表的百分比，9876表示成功率98.76%
} HI_SDM_ENG_DAY_FREEZE_RATIO_S;

typedef struct
{
    HI_U32 sysTime;                         // 最后一次抄读失败UTC时间
    HI_U8 meter[HI_SDM_ENG_METER_ADDR_LEN]; // 失败表地址，末3个字节
    HI_U8 causeSet;                         // 抄读失败原因并集，多次失败原因的集合
    HI_U8 causeLatest;                      // 最后一次抄读失败原因
                                            // 0x01:超时
                                            // 0x02:表地址错误
                                            // 0x04:电表否认帧
                                            // 0x08:645应用数据内容非法
                                            // 0x10:时钟不对
    HI_U8 frame[HI_SDM_ENG_FRAME_LEN_MAX];  // 最后一次失败时对应的失败报文
                                            // 0x01:存下行645报文
                                            // 0x02:存上行表地址
                                            // 0x04:存否认帧
                                            // 0x08:存645帧，至少需要从DI号往后存
                                            // 0x10:存时间报文
} HI_SDM_ENG_DAY_FREEZE_FAIL_ITEM;

typedef struct
{
    HI_SDM_ENG_DAY_FREEZE_RATIO_S ratio;    // 日冻结抄读总体成功率统计
    HI_U32 sysTime;                         // 系统UTC时间
    HI_U16 failCount;                       // 日冻结抄读失败电表数量
    HI_U16 pad;                             // 填充字段

    // 日冻结抄读失败电表信息，不超过50个时，按实际情况记录；超过50个时，只记录50个电表信息
    HI_SDM_ENG_DAY_FREEZE_FAIL_ITEM item[HI_SDM_ENG_METER_REC_MAX];
} HI_SDM_ENG_DAY_FREEZE_STATS_S;


typedef struct
{
    HI_SDM_ENG_STORAGE_S stStorageHrd;          // 周期存储头结构
    HI_SDM_ENG_MRS_LRPR_STATS_S lrprStats;      // 轮抄/并发抄抄读数据统计
    HI_SDM_ENG_MRS_LOW_RATIO_S lowRatioStats;   // 低于抄通率门限电表统计
    HI_SDM_ENG_MRS_XR_STATS_S xrStats;          // 透抄抄读数据统计
    HI_SDM_ENG_DAY_FREEZE_STATS_S dayFreeze;    // 日冻结数据统计
} HI_SDM_ENG_MRS_LOG_S;
#endif

#endif

/******************************************************************************
频段自适应
*******************************************************************************/

/* BEGIN: Added by likunhe/00194117, 2014/3/18   PN:DTS2014032708937*/
//频段评估结果存储结构
typedef struct
{
    HI_U32 id                   : 4;					//保留
    HI_U32 start_reason         : 8;				    //启动的原因
    HI_U32 stop_reason          : 8;		            //停止的原因
    HI_U32 init_freq            : 3;			        //原工作频段
    HI_U32 obj_freq             : 3;			        //评估结束时期望的工作频段
    HI_U32 state                : 5;                    //评估状态
    HI_U32 pad                  : 1;					//保留

    /* BEGIN: Modified by likunhe/00194117, 2014/6/10   问题单号:DTS2014060603580*/
    //HI_MAC_FA_FREQ_RESULT_STRU freq_result[3];                   //频段评估结果
    HI_MAC_FA_FREQ_RESULT_STRU freq_result[HI_SDM_FA_EVALUATE_RESULT_FREQ_NUM_MAX];     //频段评估结果
    /* END:   Modified by likunhe/00194117, 2014/6/10 */
}HI_SDM_FA_EVALUATE_RESULT_STRU;
/* END:   Added by likunhe/00194117, 2014/3/18 */


//*****************************************************************************


//*****************************************************************************
#define HI_DSID_SDM_BSP_MAX_SIZE      (512)
#define HI_DSID_SDM_CHL_MAX_SIZE      (512)
#define HI_DSID_SDM_RX_MAX_SIZE       (512)
#define HI_DSID_SDM_TX_MAX_SIZE       (512)
#define HI_DSID_SDM_BCN_MAX_SIZE      (512)
#define HI_DSID_SDM_NM_MAX_SIZE       (512)
#define HI_DSID_SDM_RT_MAX_SIZE       (512)
#define HI_DSID_SDM_MR_MAX_SIZE       (496)
//#define HI_DSID_SDM_MR_MAX_SIZE       (512-sizeof(HI_SDM_ENG_STORAGE_S))
#define HI_DSID_SDM_ITEM_MAX_SIZE     (512)

#define HI_SDM_CHECK_PARAM()  do {\
    HI_SDM_VERIFY_EXPR((sizeof(HI_DSID_SDM_BSP_S) <= HI_DSID_SDM_BSP_MAX_SIZE), sizeof(HI_DSID_SDM_BSP_S), 0);\
    HI_SDM_VERIFY_EXPR((sizeof(HI_DSID_SDM_CHL_S) <= HI_DSID_SDM_CHL_MAX_SIZE), sizeof(HI_DSID_SDM_CHL_S), 1);\
    HI_SDM_VERIFY_EXPR((sizeof(HI_DSID_SDM_RX_S)  <= HI_DSID_SDM_RX_MAX_SIZE),  sizeof(HI_DSID_SDM_RX_S),  2);\
    HI_SDM_VERIFY_EXPR((sizeof(HI_DSID_SDM_TX_S)  <= HI_DSID_SDM_TX_MAX_SIZE),  sizeof(HI_DSID_SDM_TX_S),  3);\
    HI_SDM_VERIFY_EXPR((sizeof(HI_DSID_SDM_BCN_S) <= HI_DSID_SDM_BCN_MAX_SIZE), sizeof(HI_DSID_SDM_BCN_S), 4);\
    HI_SDM_VERIFY_EXPR((sizeof(HI_DSID_SDM_NM_S)  <= HI_DSID_SDM_NM_MAX_SIZE),  sizeof(HI_DSID_SDM_NM_S),  5);\
    HI_SDM_VERIFY_EXPR((sizeof(HI_DSID_SDM_RT_S)  <= HI_DSID_SDM_RT_MAX_SIZE),  sizeof(HI_DSID_SDM_RT_S),  6);\
    HI_SDM_VERIFY_EXPR((sizeof(HI_DSID_SDM_MR_S)  <= HI_DSID_SDM_MR_MAX_SIZE),  sizeof(HI_DSID_SDM_MR_S),  7);\
}while(0)


#define HI_LEAVE_PLC_MAC_ADDR_OFFSET    (2)
/* BEGIN: Added by zhangguobin, 2015/3/26   问题单号:CQ_MODIFY */
#define HI_LEAVE_PLC_MAC_ADDR_LEN	(HI_PLC_MAC_ADDR_LEN - 2)
typedef struct
{
	HI_U8  mac[HI_LEAVE_PLC_MAC_ADDR_LEN];	// 站点MAC地址(为了节省存储空间，MAC地址仅存储低4个字节)

	HI_U32 leave_start_time;				// 离线开始时间（13集中器为日历时间，09集中器为系统运行时间）
	HI_U32 leave_time : 16;					// 离线时长，单位：分钟（65535分钟，可表示45天，若站点离线时间超过45天，则存储溢出）
	HI_U32 leave_reason : 4;				// 离线原因
	HI_U32 sw_reset_reason : 6;			    // 软重启原因
	HI_U32 hw_reset_reason : 6;				// 硬重启原因
}HI_SDM_LEAVE_INFO_S;

#define HI_SDM_MAX_LEAVE_ENTRY_NUM  ((PRODUCT_CFG_FLASH_BLOCK_SIZE - sizeof(HI_SDM_ENG_STORAGE_S)) / sizeof(HI_SDM_LEAVE_INFO_S))
typedef struct
{
    HI_SDM_ENG_STORAGE_S stStorageHrd;
    HI_SDM_LEAVE_INFO_S leaveinfo[HI_SDM_MAX_LEAVE_ENTRY_NUM];
}HI_SDM_ENG_NMLV_LOG_S;

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) && defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG)
HI_EXTERN HI_SDM_ENG_NMLV_LOG_S g_stSdmEngNmlvLog;
HI_EXTERN HI_SDM_ENG_NMLV_LOG_S* HI_SDM_NMLV_LOG_GPTR;
#define HI_SDM_NMLV_LOG_GPTR     ((HI_SDM_ENG_NMLV_LOG_S*)&g_stSdmEngNmlvLog)
#endif
/* END:   Added by zhangguobin, 2015/3/26 */


//*****************************************************************************

//*****************************************************************************
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) && defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG)
HI_EXTERN HI_SDM_ENG_MRS_LOG_S  g_stSdmEngMrsLog;
HI_EXTERN HI_SDM_ENG_MRS_LOG_S* HI_SDM_MRS_LOG_GPTR;
#define HI_SDM_MRS_LOG_GPTR     ((HI_SDM_ENG_MRS_LOG_S*)&g_stSdmEngMrsLog)
#endif



typedef enum
{
    SD_DRV_DFX_API_GET_FIRMWARE,
    SD_DRV_DFX_API_CHECK_EXIST,
    SD_DRV_DFX_API_GET_DISK_SIZE,
    SD_DRV_DFX_API_DISK_FORMAT,
    SD_DRV_DFX_API_GET_DIR_FIRST,
    SD_DRV_DFX_API_GET_DIR_NEXT,
    SD_DRV_DFX_API_MAKE_DIR,
    SD_DRV_DFX_API_ENTER_DIR,
    SD_DRV_DFX_API_BACK_ROOT,
    SD_DRV_DFX_API_MAKE_FILE,
    SD_DRV_DFX_API_REMOVE_FILE,
    SD_DRV_DFX_API_FILE_OPEN,
    SD_DRV_DFX_API_FILE_CLOSE,
    SD_DRV_DFX_API_FILE_GET_LEN,
    SD_DRV_DFX_API_FILE_READ,
    SD_DRV_DFX_API_FILE_CLEAR,
    SD_DRV_DFX_API_FILE_APPEND,
    SD_DRV_DFX_API_NOTIFY_CUR_TIME,
    SD_DRV_DFX_API_NOTIFY_GET_DIR_NUM,
    SD_DRV_DFX_API_DISK_OTHER_MAX,// TODO:不用的接口ID暂时放在下面，不进行统计

    SD_DRV_DFX_API_REMOVE_DIR,
    SD_DRV_DFX_API_FILE_SEEK,
    SD_DRV_DFX_API_DIRECTORY_RENAME,
}SD_DRV_DFX_API_E;

typedef struct
{
    HI_U16 usIdleMaxWaitTime;
    HI_U16 usReadyMaxWaitTime;
    HI_U8 ucIdleTimeOutTimes;
    HI_U8 ucReadyTimeOutTimes;
    HI_U8 reserver[2];

}SD_DRV_API_DFX_S;

//*****************************************************************************


//*****************************************************************************
#define DEAD_CHECK

typedef enum    _DFX_PK_SIZE_EXCEPTION_POS_E_
{
    nm_handle_assoc_req_ERR_FUNC = 0 ,
    nm_handle_assoc_cnf_ERR_FUNC ,
    nm_create_assoc_ind_01_ERR_FUNC ,
    nm_create_assoc_ind_02_ERR_FUNC ,
    nm_create_assoc_ind_03_ERR_FUNC ,
    nm_create_change_proxy_cnf_ERR_FUNC ,
    nm_handle_change_proxy_cnf_ERR_FUNC ,
    nm_create_change_key_ind_ERR_FUNC ,
    //plc_mac_output_NU_ERR_FUNC ,
    dfx_ndm_handle_hso_cmd_callback_ERR_FUNC ,
    dfx_ndm_packet_frame_ERR_FUNC ,
    dfx_ndm_packet_frame_copy_payload_list_ERR_FUNC ,
    dfx_ndm_handle_packet_ERR_FUNC ,
    dfx_ndm_pipe_send_ack_back_ERR_FUNC ,
    dfx_ndm_pipe_fragment_output_ERR_FUNC ,
    dfx_ndm_fill_reply_payload_ERR_FUNC ,
    //Ndc_DRemoteSend_ERR_FUNC
}DFX_PK_SIZE_EXCEPTION_POS_E;

#define MAX_PK_EXCEPTION_CNT 15//(17-2)

typedef struct
{
    HI_U16  type;                    // 0:INDICATION , 1: CNF, 2: REQ, 3:RSP
    HI_U16  channelType;             //抄控器通道的类型
    HI_U16  usCmdId;                 // 命令ID
    HI_U16  pad;

    HI_U32 payload_size;            //报文大小
}DFX_NDM_PK_EXCEPTION_INFO_S;
typedef struct
{
    HI_U16  pk_mmtype;              // 报文mtype
    HI_U16  ostei;                  // 报文mmtype
    HI_U16  odtei;                  // 报文类型
    HI_U16  pad;

    HI_U32  payload_size;           //报文大小
} DFX_NM_PK_EXCEPTION_INFO_S;

typedef union
{
    DFX_NDM_PK_EXCEPTION_INFO_S ndm_pk_excpetion_info;
    DFX_NM_PK_EXCEPTION_INFO_S nm_pk_excpetion_info;
}DFX_PK_EXCEPTION_INFO_U;
typedef struct _dfx_mac_check_pk_err_status_
{
    DFX_PK_SIZE_EXCEPTION_POS_E err_pos; // 异常函数位置
    DFX_PK_EXCEPTION_INFO_U err_pk_info; // 异常报文信息
}DFX_MAC_CHECK_PK_ERR_STATUS_S;

#define flash_block_size 4096
#define SDM_PK_INFO_SIZE sizeof(DFX_MAC_CHECK_PK_ERR_STATUS_S)
#define MAX_SDM_PK_NUM (flash_block_size/SDM_PK_INFO_SIZE)


typedef struct
{
    HI_U8 aucPkBuffer[MAX_SDM_PK_NUM][SDM_PK_INFO_SIZE];
} HI_SDM_MAC_ERR_PK_CACHE_S;


HI_EXTERN HI_SDM_MAC_ERR_PK_CACHE_S g_stMac_check_pk_info;
HI_EXTERN HI_U32 g_sdm_index;


//*****************************************************************************
// 函数接口定义
//*****************************************************************************
HI_EXTERN HI_VOID HI_SDM_KlgPkinfoSave(DFX_MAC_CHECK_PK_ERR_STATUS_S *pk_exception_info);

//*****************************************************************************

HI_EXTERN HI_VOID HI_SDM_KlgCrashSave(HI_VOID);

#if defined(PRODUCT_CFG_SUPPORT_SDM_TM_KLG)
HI_EXTERN HI_VOID HI_SDM_KlgRebootSave(HI_U32 ulId);
#endif

#if defined(PRODUCT_CFG_SUPPORT_SDM_KLG)
HI_EXTERN HI_VOID HI_SDM_KlgCtxInit(HI_VOID);
HI_EXTERN HI_VOID HI_SDM_KlgInitPhase1(HI_VOID);
HI_EXTERN HI_VOID HI_SDM_KlgInitSave(HI_BOOL bReboot);
HI_EXTERN HI_VOID HI_SDM_KlgChkErrSave(HI_U32 id);
HI_EXTERN HI_VOID HI_SDM_KlgRebootSave(HI_U32 ulId);
HI_EXTERN HI_VOID HI_SDM_KlgRebootInit(HI_VOID);
HI_EXTERN HI_VOID HI_SDM_KlgOsSave(HI_PVOID pBuffer, HI_U32 ulSize);
HI_EXTERN HI_VOID HI_SDM_KlgTskPeriodSave(HI_VOID);
HI_EXTERN HI_VOID HI_SDM_KlgChkMacBufferValid(HI_PVOID pAnyMacBuf, HI_U32 ulUsedSize, HI_U32 ulLn, HI_PCSTR pszFunc);
HI_EXTERN HI_VOID HI_SDM_KlgCheckMacBufferInit(HI_VOID);
HI_EXTERN HI_BOOL HI_SDM_KlgIsEnable(HI_VOID);
HI_EXTERN HI_VOID HI_SDM_KlgSetEnable(HI_BOOL bEnable);
#define HI_SDM_KLG_CHK_MAC_BUF_PTR(pAnyMacBuf, ulUsedSize)      HI_SDM_KlgChkMacBufferValid(pAnyMacBuf, ulUsedSize, __LINE__, __FUNCTION__)
#define HI_SDM_KLG_NM_KEY_INFO(offset, v)                       g_stSdmKlgSys.stErrSizeLog.size[offset] = v
#define HI_SDM_KLG_NM_KEY_INFO_INC(offset,v)                    HI_SAL_INC_U32(g_stSdmKlgSys.stErrSizeLog.cnt[offset]); HI_SDM_KLG_NM_KEY_INFO(offset, v)
#define HI_SDM_KLG_INFO_INC(offset,m)                           HI_SAL_INC_U32(g_stSdmKlgSys.m[offset])
#else
#define HI_SDM_KlgCtxInit()                 do {} while(0)
#define HI_SDM_KlgInitPhase1()              do {} while(0)
#define HI_SDM_KlgInitSave(bReboot)         do {} while(0)
//#define HI_SDM_KlgBufListSave()             do {} while(0)
#define HI_SDM_KlgCrashSave()               do {} while(0)
#define HI_SDM_KlgChkErrSave(id)            do {} while(0)
#if !defined(PRODUCT_CFG_SUPPORT_SDM_TM_KLG)||defined(PRODUCT_CFG_FEATURE_ST_PHY)
#define HI_SDM_KlgRebootSave(ulId)          do {} while(0)
#endif
#define HI_SDM_KlgRebootInit()              do {} while(0)
#define HI_SDM_KlgOsSave(pBuffer, ulSize)   do {} while(0)
#define HI_SDM_KlgTskPeriodSave()           do {} while(0)
#define HI_SDM_KlgCheckMacBufferInit()      do {} while(0)
#define HI_SDM_KlgIsEnable()                HI_FALSE
#define HI_SDM_KlgSetEnable(bEnable)        do {} while(0)
#define HI_SDM_KLG_CHK_MAC_BUF_PTR(pAnyMacBuf, ulUsedSize)                 do {} while(0)
#define HI_SDM_KLG_NM_KEY_INFO(offset, v)                                  do {} while(0)
#define HI_SDM_KLG_NM_KEY_INFO_INC(offset,v)                               do {} while(0)
#define HI_SDM_KLG_INFO_INC(offset,m)                                      do {} while(0)
#endif




#if defined(PRODUCT_CFG_SUPPORT_SDM_ENG)
HI_EXTERN HI_U32 HI_SDM_RegisterMsduSaveFeature(HI_SDM_RegFeature_S *pstFeature);
#else
HI_PRVL HI_U32 HI_SDM_RegisterMsduSaveFeature(HI_SDM_RegFeature_S *pstFeature)  { return HI_ERR_NOT_SUPPORT; }
#endif
//*****************************************************************************


//*****************************************************************************
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)&& defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG)
HI_EXTERN HI_U32 HI_SDM_WriteMrsLog(HI_BOOL isIdxAdd);
#else
HI_PRVL HI_U32 HI_SDM_WriteMrsLog(HI_BOOL isIdxAdd)  { HI_UNREF_PARAM(isIdxAdd); return HI_ERR_NOT_SUPPORT; }
#endif
//*****************************************************************************
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
HI_EXTERN HI_U32 HI_SDM_WriteNmlvLog(HI_BOOL isIdxAdd);
HI_EXTERN HI_VOID HI_SDM_NmlvInit(HI_U32 address,HI_U32 size);
#else
HI_PRVL HI_U32 HI_SDM_WriteNmlvLog(HI_BOOL isIdxAdd)  { HI_UNREF_PARAM(isIdxAdd); return HI_ERR_NOT_SUPPORT; }
HI_PRVL HI_VOID HI_SDM_NmlvInit(HI_U32 address,HI_U32 size){ HI_UNREF_PARAM(address); HI_UNREF_PARAM(size); return; }
#endif


#if defined(PRODUCT_CFG_CHECK_TASK_STACK_ERR)
HI_EXTERN HI_VOID HI_SDM_TskStakChkErr(HI_SYS_TASK_STK_ERR_S* pstStkErr);
#else
HI_PRVL HI_VOID HI_SDM_TskStakChkErr(HI_SYS_TASK_STK_ERR_S* pstStkErr) {HI_UNREF_PARAM(pstStkErr);}
#endif


#if defined(PRODUCT_CFG_CHECK_TASK_STACK_ERR_V2)
HI_EXTERN HI_VOID HI_SDM_CheckAllStack(HI_VOID);
#else
HI_PRVL HI_VOID HI_SDM_CheckAllStack(HI_VOID) {}
#endif

#if defined(PRODUCT_CFG_HAVE_OS_ALL_TASK_INFO)
HI_EXTERN HI_PVOID HI_SDM_GetOsKrlInfo(HI_VOID);
HI_EXTERN HI_U32 HI_SDM_GetOsKrlInfoSize(HI_VOID);
#else
HI_PRVL HI_PVOID HI_SDM_GetOsKrlInfo(HI_VOID)         { return HI_NULL; }
HI_PRVL HI_U32 HI_SDM_GetOsKrlInfoSize(HI_VOID)      { return HI_ERR_NOT_SUPPORT; }
#endif
HI_VOID HI_SDM_QueryOsKrlBasicInfo(HI_VOID);
HI_VOID HI_SDM_SetBootVer(HI_CHAR* pszBootVer);
#endif // PRODUCT_CFG_CORE_TYPE_BOOTLOADER
//*****************************************************************************


//*****************************************************************************
// HI_SDM_CheckTestFrameSwt
//*****************************************************************************
HI_SDM_TEST_FRME_SWT_E HI_SDM_CheckTestFrameSwt (HI_VOID);

//*****************************************************************************
//
// ulSdmId 取值为
//
// HI_DSID_SDM_CHL
// HI_DSID_SDM_RX
// HI_DSID_SDM_TX
// HI_DSID_SDM_BCN
// HI_DSID_SDM_NM
// HI_DSID_SDM_MR
// HI_DSID_SDM_KLG_ALL
// HI_DSID_SDM_KLG_SYSERR
//
// 模块中到处关键信息
HI_U32 HI_SDM_ExportData(HI_U32 ulSdmId, HI_PVOID pData, HI_U32 ulDataSize);
HI_U32 HI_SDM_NdmImportData(HI_U8 aucMac[HI_PLC_MAC_ADDR_LEN], HI_U32 ulSdmId, HI_PVOID pData, HI_U32 ulDataSize);
//*****************************************************************************

typedef struct
{
    HI_U32 ulTxOkNum;
    HI_U32 ulTxErrNum;
    HI_U32 ulRxOkNum;
    HI_U32 ulRxErrNum;
} HI_SDM_TEST_FRAME_INFO_S;


HI_U32 HI_SDM_SendTestFrame(HI_PVOID pFrame, HI_U32 ulFrameSize, HI_U8 ucFrameFlag, HI_U8 ucOption);
HI_U32 HI_SDM_GetTestFrameInfo (HI_SDM_TEST_FRAME_INFO_S* pstTestFrameInfo);

#endif // PRODUCT_CFG_CORE_TYPE_BOOTLOADER
//*****************************************************************************




//*****************************************************************************
HI_END_HEADER
#endif //__DFX_SYS_H__


