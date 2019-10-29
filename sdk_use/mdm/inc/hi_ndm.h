//****************************************************************************
//
//                  版权所有 (C), 2001-2011, 华为技术有限公司
//
//****************************************************************************
//  文 件 名   : hi_ndm.h
//  版 本 号   : V1.0
//  作    者   : 沈汉坤/00130424
//  生成日期   : 2012年06月10日
//  功能描述   : 抄控器接口定义, 仅供 MDM 和 HSO 使用
//  函数列表   :
//  修改历史   :
//  1.日    期 : 2012年06月10日
//    作    者 : 沈汉坤/00130424
//    修改内容 : 创建文件
//
//****************************************************************************
#ifndef __HI_NDM_H__
#define __HI_NDM_H__
#ifndef PRODUCT_CFG_HSO
#include <hi_ft_nv.h>
#endif

#if !defined(PRODUCT_CFG_CORE_TYPE_BOOTLOADER) || defined(PRODUCT_CFG_HSO)
#ifndef PRODUCT_CFG_HSO
#include <hi_mdm.h>
#else
#include "hi_types.h"
#endif
#include <hi_cmn_nv.h>
#include <hi_config.h>
//****************************************************************************

//****************************************************************************
//
// 操控包ID 定义
// 操控器包缺省范围 [0x3400, 0x3600) 即 HI_ND_NDM_START, HI_ND_NDM_END-1
//
#define HI_ND_NDM_START                     HI_ND_NDM_START_DEFAULT
#define HI_ND_NDM_END                       HI_ND_NDM_END_DEFAULT

#define HI_SYS_STATUS                       0x3400  // 查询当前的系统工作状态等信息 , ,  HI_SYS_STATUS_IND_S
#define HI_ND_ERR_IND                       0x3401
#define HI_ND_CONNECTED_INFO                0x3402  // 参数: , , HI_ND_CONNECTED_INFO_IND_S
#define HI_BIDB_EXPORT                      ID_DIAG_CMD_BIDB_EXPORT  // 参数: HI_BIDB_EXPORT_REQ_S,,HI_BIDB_EXPORT_IND_S
#define HI_BIDB_IMPORT                      ID_DIAG_CMD_BIDB_IMPORT  // 参数: HI_BIDB_IMPORT_REQ_S,,HI_BIDB_IMPORT_IND_S
#define HI_MAC_FIND_MODULE                  0x3407  // 参数: , , ,HI_MAC_FIND_MODULE_IND_S
/* BEGIN: Added by guopan, 2013/11/12   问题单号:DTS2013111201546*/
#if defined(PRODUCT_CFG_FEATURE_NDC_CHL)
#define HI_ND_NETWORK_SEARCH_CONN 0x3408   //用于HSO发起的PLC网络搜索和连接
#endif
/* END:   Added by guopan, 2013/11/12 */
/* BEGIN: Added by b00208046, 2013/11/18   PN:DTS2013111801139 */
#define HI_ND_DEVICE_INFO                   0x3409
/* END:   Added by b00208046, 2013/11/18 */
#define HSO_QRY_UE_API_VER                  0x340A

#define HI_UPG_STATUS                       0x3410  // 查询升级状态 , ,HI_UPG_STATUS_IND_S
#define HI_UPG_FINISH                       0x3411  // 参数: HI_UPG_FINISH_REQ_S, , HI_UPG_FINISH_IND_S
#define HI_UPG_START                        ID_DIAG_CMD_UPG_START  // 参数: HI_UPG_START_REQ_S, , HI_UPG_START_IND_S
#define HI_UPG_STOP                         0x3413  // HI_UPG_STOP_REQ_S
#define HI_UPG_VER_BACK                     0x3414  // HI_UPG_VER_BACK_REQ_S, , HI_UPG_VER_BACK_IND_S
#define HI_UPG_TRANSMIT_BLK                 0x3415  // 发送升级文件块 HI_UPG_TRANSMIT_BLK_REQ_S, , HI_UPG_TRANSMIT_BLK_IND_S
#define HI_ND_MODULE_DETECT                 0x3416  // , , HI_ND_MODULE_DETECT_IND_S
#define HI_UPG_STAT_INFO_IND                0x3417  // HI_UPG_STATUS, , HI_UPG_STAT_INFO_IND_S
#define HI_UPG_STATUS_REMOTE_QUERY          0x3418  // 远程 进度查询
#define HI_UPG_AFFIRM                       0x3419  //全网升级完成通知，用于通知各模块重启
#define HI_UPG_STA_LIST                     0x341A  // 发送STA列表
#define HI_UPG_EXTEND_INFO                  0x341B  // 升级扩展信息
#define HI_UPG_LIST_REPLY                   0x341C  // 列表升级应答HSO查询列表
#define HI_UPG_SET_PROTOCOL                 0x341D  // 南网版本设置升级协议
#define HI_DBG_MEM_STAT                     0x3420  // 内存统计信息上报请求, ,
#define HI_DBG_MEM_SUM_STAT_IND             0x3421  // 内存统计信息上报, HI_DBG_MEM_SUM_STAT_IND_S
#define HI_DBG_MEM_MOD_STAT_IND             0x3422  // 内存统计信息上报, HI_DBG_MEM_MOD_STAT_IND_S
#define HI_DBG_STAT_Q                       0x3430  // HI_DBG_STAT_Q_S,,HI_DBG_MEM_MOD_STAT_IND_S
#define HI_DBG_DUMP_DIAG_ITEMS              0x3431  // HI_DBG_DUMP_DIAG_ITEMS_REQ_S,,    : dumpmsg dump flash key message
#define HI_DBG_DFX_TASK_TEST                0x3432  // HI_DBG_DFX_TASK_TEST_S, ,

// reserved: 0x3502--0x3509

#define HI_UPG_NV_W                         ID_DIAG_CMD_UPG_NV_W // 参数: HI_UPG_NV_W_REQ_S, , 
#define HI_UPG_NV_R                         0x3511       // 参数: HI_UPG_NV_R_REQ_S, , HI_UPG_NV_IND_S
#define HI_UPG_NV_BW                        HI_UPG_NV_W  // 参数: HI_UPG_NV_W_REQ_S, ,
#define HI_UPG_NV_BR                        HI_UPG_NV_R  // 参数: HI_UPG_NV_R_REQ_S, , HI_UPG_NV_IND_S
#define HI_UPG_NV_BE_IND                    0x3512       // 参数: , , HI_UPG_NV_BE_IND_S
/* BEGIN: Added by b00208046, 2013/11/30   PN:DTS2013111801139 */
#define HI_NDM_BAT_UPG_STATUS_MSG           0x3550          //查询升级状态
#define HI_DIAG_DRV_NDM_DEBUG              (0x3551)         //调试用，正式代码删除 II_NDM_TEST
#define HI_DIAG_DRV_BQ_STAT_MSG             0x3552          //调试命令 查看BQ模块全局变量
#define HI_DIAG_DRV_SD_DC_STAT              0x3553
#define HI_NDM_SD_DC_SET                    0x3554        //"sd_dc_set"
#define HI_NDM_SD_DC_QUERY                  0x3555        //"sd_dc_query"

/* END:   Added by b00208046, 2013/11/30 */
//****************************************************************************


//****************************************************************************

// 控制字定义(15bit)
//上行控制字
#define HI_ND_FRAME_UL_CTRL_STATUS_OK                      (0)      //命令请求处理完毕且成功
#define HI_ND_FRAME_UL_CTRL_STATUS_TRANS                   (1)      //命令请求正在处理中，且已经透传到指定中继
#define HI_ND_FRAME_UL_CTRL_STATUS_FAIL_INVALID_PARAM      (100)    // 参数错误
#define HI_ND_FRAME_UL_CTRL_DREMOTE_FRM                    (900)    //645帧处理

#define HI_ND_FRAME_STATUS_OK                      HI_ND_FRAME_UL_CTRL_STATUS_OK
#define HI_ND_FRAME_STATUS_TRANS                   HI_ND_FRAME_UL_CTRL_STATUS_TRANS
#define HI_ND_FRAME_STATUS_FAIL_INVALID_PARAM      HI_ND_FRAME_UL_CTRL_STATUS_FAIL_INVALID_PARAM

//下行控制字
#define HI_ND_FRAME_DL_CTRL_DEFAULT                      0          //MAC确定是否需要处理，如果不处理则透传给其它模块，并等待其它模块处理后再由MAC透传给HSO
#define HI_ND_FRAME_DL_CTRL_MAC                          1          //需要MAC处理，不需要透传给APP，默认都给MAC处理
#define HI_ND_FRAME_DL_CTRL_NEED_ACK                     2
#define HI_ND_FRAME_DL_CTRL_NOT_NEED_ACK                 3
/* END ADD by db 2012-06-30   */
#define HI_ND_FRAME_DL_CTRL_DREMOTE_FRM                   HI_ND_FRAME_UL_CTRL_DREMOTE_FRM

#define HI_ND_DL  0
#define HI_ND_UL  1

#define HI_UPG_STATUS_QRY_SYS_MODE   0 // 查询工作模式
#define HI_UPG_STATUS_QRY_UPGRADE    1 // 查询当前升级情况

#define HI_UPG_STATUS_OK            HI_MDM_UPG_STATUS_OK      // 升级成功
#define HI_UPG_STATUS_END           HI_MDM_UPG_STATUS_END    // 升级完成
#define HI_UPG_STATUS_CONTINUE      2                        // 升级过程中断电后, 需要续传
#define HI_UPG_STATUS_NORMAL        4                        // 正常升级状态
#define HI_UPG_STATUS_UNKNOWN       HI_MDM_UPG_STATUS_UNKNOWN    // 未知

#define HI_UPG_STATUS_STOPPING      HI_MDM_UPG_STATUS_STOPPING

#define HI_UPG_STATUS_FAIL          HI_MDM_UPG_STATUS_FAIL      // 升级失败
#define HI_UPG_STATUS_PROCESS       HI_MDM_UPG_STATUS_PROCESS   // 正在升级
#define HI_UPG_STATUS_TRANSMIT      HI_MDM_UPG_STATUS_TRANSMIT  // 正在转发

#define HI_UPG_VER_UPDATE_OPTION_FORCE      0x80    // 强制升级
#define HI_UPG_VER_UPDATE_OPTION_CONTINUE   0x40    // 继续升级块
#define HI_UPG_VER_UPDATE_OPTION_MASK       ((HI_UPG_VER_UPDATE_OPTION_START)\
                                           | (HI_UPG_VER_UPDATE_OPTION_CONTINUE)\
                                           | (HI_UPG_VER_UPDATE_OPTION_FORCE))

//#define HI_PLC_RELAY_NUM_MAX             4 // 3, 5, 7, 9, 11, 13
#define HI_PLC_RELAY_NUM_MAX             3 //抄控器通道所支持的MAC列表中MAC地址的最大数

#define HI_COL_METER_NUM_MAX             33

#define HI_PLC_CRASH_SERIAL_RPT_CODE     "$PlCCrAsH^"

#define HI_DEID_OPTION_REFRESH          0xF0
//****************************************************************************

/* BEGIN: Added by guopan, 2013/11/12   问题单号:DTS2013111201546*/
#if defined(PRODUCT_CFG_FEATURE_NDC_CHL)
/* BEGIN: Deleted by likunhe/00194117, 2014/1/10   问题单号:DTS2014010707179*/
//#define MAC_NODE_NAME_LEN  6
/* END: Deleted by likunhe/00194117, 2014/1/10 */

/*定义NDM上报搜网结果的规格*/
#define MDL_REPORT_MAX        80        //根据单个报文的上报容量确定
#endif
/* END:   Added by guopan, 2013/11/12 */
//****************************************************************************
#ifndef PRODUCT_CFG_HSO
typedef struct
{
    HI_ND_REMOTE_FRAME_HEAD_S stFrmHrd;
    HI_U8 aucPayload[0];
} HI_MDM_REQ_MSG_S;
#endif
/* BEGIN: Added by guopan, 2013/11/12   问题单号:DTS2013111201546*/
#if defined(PRODUCT_CFG_FEATURE_NDC_CHL)

#define    HI_NDC_SEARCH_CONN_IND_TYPE_NEW           0  //表示NDC通知HSO更新/添加PLC搜网信息
#define    HI_NDC_SEARCH_CONN_IND_TYPE_ALL             1  //表示NDC上报所有PLC网络信息
#define    HI_NDC_SEARCH_CONN_IND_TYPE_STATUS     2  //表示NDC上报当前的搜网状态

#define HI_MDM_PLC_FREQ_AUTO            (0xFF)  //表示不指定频段，由NDC自动搜索
#define HI_MDM_PLC_SNID_AUTO             (0)  //表示不指定snid，由NDC自动搜索

/************************************************************
                             数据结构定义
************************************************************/

typedef struct
{
     HI_U32     ulMsgId;        //表示出现错误所在的请求消息ID
     HI_U32     ulErrCode;      //使用hi_errno.h中的错误码
} HI_ND_ERR_IND_S;
#endif
/* END:   Added by guopan, 2013/11/12 */
/* BEGIN: Added by b00208046, 2014/1/4   问题单号:DTS2014010400315*/
/***************************************电池电量查询*******************************************************************/
typedef enum
{
    HI_ND_DEVICE_INFO_BAT_STATUS_NORMAL             = 0x00,     //正常模式
    HI_ND_DEVICE_INFO_BAT_STATUS_POWER_SOURCE_USE   = 0x01,     //外接电源在位
    HI_ND_DEVICE_INFO_BAT_STATUS_IS_CHARGE          = 0x02,     //充电中
    HI_ND_DEVICE_INFO_BAT_STATUS_BAT_NO_EXISTT      = 0x03,     //电池不在位

    HI_ND_DEVICE_INFO_BAT_STATUS_BAT_INIT_ING       = 0x10,     //初始化未完成
    HI_ND_DEVICE_INFO_BAT_STATUS_BAT_NO_FIRMWARE    = 0x11,     //充电芯片没有软件
    HI_ND_DEVICE_INFO_BAT_STATUS_BAT_UPGRATE_ING    = 0x12,     //升级中
}HI_ND_DEVICE_INFO_BAT_STATUS_E;//枚举值 范围需小于0x100
typedef struct
{
    HI_U8 ucBatteryPercent;
    HI_U8 ucStatus;                     //HI_ND_DEVICE_INFO_BAT_STATUS_E
    HI_U8 reserved[2];
}HI_ND_DEVICE_INFO_IND_S;
/***************************************HSO查询API版本*******************************************************************/
//命令接口版本号  
#define HI_CMD_VER (0)

typedef struct _DIAG_CMD_HSO_QRY_UE_API_VER_IND_S_
{
    HI_U16 usCmdVer;   //命令接口版本号              
    HI_U16 usHAPIVer; //API高版本号
    HI_U16 usAPIVer;   //单板目前API版本              	
    HI_U16 usCVer;     //C版本			 	
    HI_U32 ulErrCode;  //错误码				
    HI_U32 ulPad1;     //保留位			 	
} DIAG_CMD_HSO_QRY_UE_API_VER_IND_S;






/* END:   Modified by b00208046, 2015/4/1 */
typedef struct
{
    HI_U8 ucMainPhase;//是否正在采数 HI_SD_DC_MAIN_PHASE_E
    HI_U8 startType;//SD_DC_START_TYPE_E
    HI_U8   reserver[2];
    /* END:   Modified by b00208046, 2015/1/29 */
}HI_SD_DC_MAIN_CTRL_CTX_S;
typedef struct
{
    HI_U8 isSupport;
    HI_U8 pad[3];
}HI_SD_DC_MODULE_CTRL_CTX_S;





/* add by yuming for ndm led err */
typedef enum
{
    NDM_APP_VER_ERR_ID,			//ID APP Version Err 
    NDM_MAC_TF_ERR_ID,			//ID TF Err 
    LER_ERR_ID_MAX,    			//ID number is limited to 32
}HI_LER_ERR_ID_E;

HI_EXTERN HI_U32 HI_NDM_LED_Handle_Err(HI_LER_ERR_ID_E enErrID, HI_BOOL bErrStat);
/* end */

//****************************************************************************


//****************************************************************************
// 查询直连模块的基本信息: HI_ND_CONNECT_MODULE_INFO
//****************************************************************************/

//****************************************************************************
// REQ: 无
// IND: HI_ND_CONNECTED_INFO_IND_S
typedef struct
{
    HI_U8 ucSysMode :7; // 设备状态
    HI_U8 bJoin     :1; // 是否组网完成
    HI_U8 ucDevType; // 设备产品形态
    HI_U8 ucMacAddr[HI_PLC_MAC_ADDR_LEN]; // 通信模块MAC
    HI_U8 ucIp[HI_IPV4_ADDR_LEN]; // 以太网IP
    HI_U32 ulSnid; // 通信网络SNID
    HI_U8 ucCcoAddr[HI_PLC_MAC_ADDR_LEN]; // CCO MAC
    HI_U8 pad[2];
    HI_CHAR szPositoin[HI_MODULE_POSITION_LEN_MAX]; // 位置信息

    HI_U32 usStartTime; // 设备启动时间
} HI_ND_CONNECTED_INFO_IND_S;
#define HI_ND_CONNECTED_INFO_ACK_S  HI_ND_CONNECTED_INFO_IND_S

//*****************************************************************************
typedef enum
{
    HI_SYS_KDBG_RC_ID_MAC = 1,

    HI_SYS_KDBG_RC_ID_MAX
}HI_SYS_KDBG_RC_ID_E;

typedef struct
{
    HI_SYS_KDBG_RC_ID_E id;
    HI_U32 data[4];
}HI_DMID_KDBG_RC_S;

typedef enum
{
    HI_SYS_KDBG_RPT_ID_MAC = 1,

    HI_SYS_KDBG_RPT_ID_MAX
}HI_SYS_KDBG_RPT_ID_E;

typedef struct
{
    HI_SYS_KDBG_RPT_ID_E id;
    HI_U32 data[4];
}HI_DMID_KDBG_RPT_S;
//*****************************************************************************



//****************************************************************************
// HI_ND_MAC_FIND_MODULE
//****************************************************************************

//****************************************************************************
// 搜索设备
// REQ: 无
// ACK:
typedef struct
{
    HI_U8   type   :4;   // HI_ND_TYPE_DCM:抄控器; HI_ND_TYPE_CCO: 头端; HI_ND_TYPE_STA:终端; HI_ND_TYPE_RELAY:中继
    HI_U8   online :1;   // 在线、离线
    HI_U8   status :1;   // 正常、异常
    HI_U8   phase_mode;  // 相位模式, 多相位还是单相位
    HI_U8   phase;       // 相位类型
    HI_U8   band;        // 频段
    HI_U16  tei;         // 站点tei(在线时)
    HI_U32  join_time;   // 加入网络时间(在线时)
    HI_U8   mac[HI_PLC_MAC_ADDR_LEN]; // MAC地址
} HI_MODULE_ITEM_S;

typedef struct
{
    HI_U16 usItemCnt; // 周边模块个数
    HI_MODULE_ITEM_S astItem[0];
} HI_MAC_FIND_MODULE_IND_S;


// 搜索节点状态（电表正常、电表异常）、节点类型（电表、采集器）、节点地址（电表地址），并上报给HSO
// REQ: 无
// ACK:
typedef struct
{
    HI_U8   type;     // HI_ND_CONNECTED_DEV_TYPE_METER: 电表; HI_ND_CONNECTED_DEV_TYPE_COLLECTOR: 采集器
    HI_U8   status;   // 0为正常，其它值为错误码，表示异常
    HI_U8   addr[HI_PLC_MAC_ADDR_LEN];  // 模块连接的设备（如电表）的地址
    HI_U8   mac[HI_PLC_MAC_ADDR_LEN]; // MAC地址
} HI_APP_MODULE_ITEM_S;

typedef struct
{
    HI_U16 usItemCnt; // 周边模块个数
    HI_APP_MODULE_ITEM_S astItem[0];
} HI_APP_FIND_MODULE_IND_S;
//****************************************************************************


//****************************************************************************

#if defined(PRODUCT_CFG_SUPPORT_UPG)
typedef struct
{
    HI_U32 ulUpgId;
} HI_UPG_FINISH_REQ_S;

typedef struct
{
    HI_U32 ulRc;     // 结果码, HI_ERR_SUCCESS表示成功, 其它为错误码
    HI_U32 ulUpgId;  // 升级ID
} HI_UPG_FINISH_IND_S;

typedef struct
{
    HI_U32 ulUpgId;  // 升级ID, ID号在升级过程中必须唯一.
    HI_U16 usOption;
    HI_U16 reserved;
} HI_UPG_VER_BACK_REQ_S;

typedef struct
{
    HI_U32 ulRc;     // 结果码, HI_ERR_SUCCESS表示成功, 其它为错误码
    HI_U32 ulUpgId;  // 升级ID, ID号在升级过程中必须唯一.
} HI_UPG_VER_BACK_IND_S;

typedef struct
{
    HI_U32 ulUpgId;  // 升级ID, ID号在升级过程中必须唯一.
    HI_U16 usResetInterval; //重启等待时间，单位sec
    HI_U16 usOption;
} HI_UPG_AFFIRMATIVE_REQ_S;

typedef struct
{
    HI_U32 ulRc;     // 结果码, HI_ERR_SUCCESS表示成功, 其它为错误码
    HI_U32 ulUpgId;  // 升级ID, ID号在升级过程中必须唯一.
} HI_UPG_AFFIRMATIVE_IND_S;

#define HI_UPG_LIST_STA_NUM  (30)
#define HI_UPG_LIST_NUM_MAX  (2047)

typedef enum 
{
    HI_UPG_LIST_VER0 = 0,
} HI_UPG_LIST_VER_E;

#define HI_UPG_LIST_VER     (HI_UPG_LIST_VER0)

typedef struct
{
    HI_U8 ucStruVer;      //version: this version is zero
    HI_U8 ucStruSize;     //structure size
    HI_U8 ucPacketTotal;  //total frames
    HI_U8 ucPacketSeq;    //current frame number,begin with one
    HI_U32 ulUpgId;       //upg ID
    HI_U16 usStaTotal;    //total number of sta
    HI_U16 usStaCurrent;  //current frame contains the number of sta
    HI_U8 aucStaList[HI_UPG_LIST_STA_NUM][HI_PLC_MAC_ADDR_LEN]; // MAC address list
}HI_UPG_STA_LIST_BLK_REQ_S;

typedef struct
{
    HI_U8 ucStruVer;  //version: this version is zero
    HI_U8 ucStruSize; //structure size
    HI_U8 ucPacketSeq;//current frame number
    HI_U8 pad;
    HI_U32 ulUpgId;   //upg ID
    HI_U32 ulRet;     //return code
}HI_UPG_STA_LIST_BLK_IND_S;

typedef enum 
{
    HI_UPG_EXTEND_INF_VER0 = 0,
} HI_UPG_EXTEND_INF_VER_E;

#define HI_UPG_EXTEND_INF_VER                (HI_UPG_EXTEND_INF_VER0)

#define HI_UPG_EXTEND_SUPPORT_LIST_UPGRADE   (0x01) //support list upg
#define HI_UPG_EXTEND_SUPPORT_SET_PROTO      (0x02) 
#define HI_UPG_EXTEND_SUPPORT_FUN            (HI_UPG_EXTEND_SUPPORT_LIST_UPGRADE)
#define HI_UPG_EXTEND_SUPPORT_FUN_CSG        (HI_UPG_EXTEND_SUPPORT_LIST_UPGRADE | HI_UPG_EXTEND_SUPPORT_SET_PROTO)

typedef struct
{
    HI_U8 ucStruVer;  // 版本号，此版本为0
    HI_U8 ucStruSize; // 结构体大小
    HI_U16 pad;
}HI_UPG_EXTEND_INFO_REQ_S;

typedef struct
{
    HI_U8 ucStruVer;  // 版本号，此版本为0
    HI_U8 ucStruSize; // 结构体大小
    HI_U8 ucUpgOption; //CCO升级STA方式:0:不涉及；1:全网;2:列表
    HI_U8 ucFunBitmap; //支持的功能特性
    HI_U32 ulUpgId;   //升级ID
    HI_U32 pad2;
}HI_UPG_EXTEND_INFO_IND_S;

typedef struct
{
    HI_U8 stru_ver;  // 版本号，此版本为0
    HI_U8 stru_size; // 结构体大小
    HI_U8 upg_mode;
    HI_U8 pad;
}HI_UPG_SET_PROTOCOL_REQ_S;

typedef struct
{
    HI_U8 ucStruVer;  // 版本号，此版本为0
    HI_U8 ucStruSize; // 结构体大小
    HI_U16 pad;
    HI_U32 ret;
}HI_UPG_SET_PROTOCOL_IND_S;

typedef HI_UPG_STA_LIST_BLK_REQ_S HI_UPG_LIST_REPLY_IND_S;
typedef struct
{
    HI_U16 usReqSn;
    HI_U16 usIdx;
    HI_U32 ulRc;
} HI_UPG_BLK_ID_S;


typedef struct
{
    HI_U16 usChecksum;
    HI_U8  ucMac[HI_PLC_MAC_ADDR_LEN];
    HI_U32 ulUpgId;
    HI_U16 usRc;
    HI_U16 usIdNum;
    HI_UPG_BLK_ID_S aulId[0];
} HI_UPG_TRANSMIT_BLK_IND_S;

// 用于CCO查询站点升级情况

typedef enum
{
    HI_UPG_STATUS_IND_ENTIRE_VER0,
    HI_UPG_STATUS_IND_ENTIRE_VER1,
} HI_UPG_STATUS_IND_ENTIRE_VER_E;

#define HI_UPG_STATUS_IND_ENTIRE_VER    (HI_UPG_STATUS_IND_ENTIRE_VER1)

#define HI_UPG_STATUS_BITMAP_LEN (PRODUCT_CFG_UPG_BITMAP_LEN)

typedef struct
{
    HI_U32 ulUpgId;
    HI_U16 usLastBlkIdx;
    HI_U16 ulErrorCode;
    HI_U32 ulRcvFileSize;
    HI_U32 ulBlkSize;
    HI_U32 ulFileSize;
    HI_U8  usUpgStatus;
    HI_U8  aucVer[3];
    HI_U16 ausVer[2];
    HI_U8  aucBootVer1;
    HI_U8  aucBootVer2;
    HI_U8  aucBootVer3;
    HI_U8  ucNvCVer; // NV.hupg: C Ver
    HI_U16 ausNvVer[2];
    HI_U32 ulChipVer;
    HI_U16 usLeftTimeWindow;
    HI_U8  aucBuildDataTime[6];
    HI_U16 usUpgReqSn;
    HI_U8  ucMacAddr[HI_PLC_MAC_ADDR_LEN];
    HI_U32 ulUpgEndTime;//modified by ds, duration(Sec.) of indicate sta from rcv start frame to rcv all upg data
} HI_UPG_STATUS_IND_ENTIRE_S_VER0;

typedef struct
{
    HI_U32 ulUpgId;
    HI_U16 usLastBlkIdx;
    HI_U16 ulErrorCode;
    HI_U32 ulRcvFileSize;
    HI_U32 ulBlkSize;
    HI_U32 ulFileSize;
    HI_U8  usUpgStatus;
    HI_U8  aucVer[3];
    HI_U16 ausVer[2];
    HI_U16 usSwVer;
    HI_U8  ucBootVer3;
    HI_U8 reserved;
    HI_CHAR szManuCode[2];
    HI_CHAR szChipCode[2];
    HI_U32 ulChipVer;
    HI_U16 usLeftTimeWindow;
    HI_U8  aucBuildDataTime[6];
    HI_U16 usYear  : 7;
    HI_U16 usMonth : 4;
    HI_U16 usDay   : 5;
    HI_U8  ucMacAddr[HI_PLC_MAC_ADDR_LEN];
    HI_U32 ulUpgEndTime;//modified by ds, duration(Sec.) of indicate sta from rcv start frame to rcv all upg data
} HI_UPG_STATUS_IND_ENTIRE_S;

typedef struct
{
   HI_UPG_STATUS_IND_ENTIRE_S status;

    HI_U8  blk_per_pkt;// 发送一个升级包中包含的块个数
    /* BEGIN:PN:DTS2014092002573 全网升级状态查询统计量优化 MODIFY\ADD  by chenli/00238131 at 2014/9/20*/
    HI_U8  from;//状态查询来源:站点主动上报(0)/应答CCO发送的查询(2)
    /* END:PN:DTS2014092002573 全网升级状态查询统计量优化  MODIFY\ADD  by chenli/00238131 at 2014/9/20*/
    HI_U16 bitmap_cnt;
    HI_U8  bitmap[0];
} HI_UPG_STATUS_DETAIL_IND_S;

typedef enum
{
    UPG_QRY_PRODUCT_TYPE_METER = 0, //电能表
    UPG_QRY_PRODUCT_TYPE_METERI,    //I型采集器
    UPG_QRY_PRODUCT_TYPE_METERII,   //II型采集器
}HI_UPG_QUERY_PRODUCT_TYPE_E;

typedef enum
{
    UPG_INFO_ID_CODE = 0,   // 厂商标号
    UPG_INFO_ID_SW_VER,     // 版本信息
    UPG_INFO_ID_BOOT_VER,   // Bootloader版本号
    UPG_INFO_ID_FILE_CRC,   // 升级文件CRC32
    UPG_INFO_ID_FILE_LEN,   // 升级文件长度
    UPG_INFO_ID_CHIP, // 产品类型
    UPG_INFO_ID_DATE,       // 版本日期
}HI_UPG_QUERY_INFO_ID_E;

//国网协议
#define UPG_INFO_ID_PRODUCT_TYPE UPG_INFO_ID_CHIP

typedef struct
{
    HI_U16 usStruVer  : 6;  //协议版本号
    HI_U16 usStruSize : 6;  //报文头长度
    HI_U16 reserved   : 4;
    HI_U8 reserved2;
    HI_U8 ucInfoItemCnt;  //信息列表元素个数
    HI_U8 aucInfoItem[0]; //信息ID列表
} HI_UPG_STA_INFO_REQ_S; //私有协议查询站点信息REQ

typedef struct
{
    HI_U16 usStruVer  : 6;  //协议版本号
    HI_U16 usStruSize : 6;  //报文头长度
    HI_U16 reserved1  : 4;
    HI_U8 reserved2;
    HI_U8 ucInfoItemCnt;   //信息列表元素个数
    HI_U8 aucInfoItem[0];  //信息列表数据字段列表，顺序放置1个或多个信息字段(元素ID(1字节)+元素数据长度(1字节)+元素数据(L))
} HI_UPG_STA_INFO_IND_S; //私有协议查询站点信息IND


//国网互联互通PLC报文格式
typedef struct
{
    HI_U16 usStruVer  : 6;  //协议版本号
    HI_U16 usStruSize : 6;  //报文头长度
    HI_U16 reserved1  : 4;
    HI_U16 reserved2;
    HI_U32 ulUpgId;         //升级ID
    HI_U16 usTimeWindow;    //升级时间窗
    HI_U16 usPacketSize;    //升级块大小
    HI_U32 ulFileSize;      //升级文件大小
    HI_U32 ulCrc;           //文件CRC校验
} HI_UPG_STD_START_REQ_S;

typedef struct
{
    HI_U16 usStruVer  : 6;  //协议版本号
    HI_U16 usStruSize : 6;  //报文头长度
    HI_U16 reserved1  : 4;
    HI_U8  reserved2;
    HI_U8  ucRc;            //开始升级结果码
    HI_U32 ulUpgId;         //升级ID
} HI_UPG_STD_START_IND_S;

typedef struct
{
    HI_U16 usStruVer  : 6;  //协议版本号
    HI_U16 usStruSize : 6;  //报文头长度
    HI_U16 reserved1  : 4;
    HI_U16 reserved2;
    HI_U32 ulUpgId;         //升级ID
} HI_UPG_STD_STOP_REQ_S;

typedef struct
{
    HI_U16 usStruVer  : 6;  //协议版本号
    HI_U16 usStruSize : 6;  //报文头长度
    HI_U16 reserved   : 4;
    HI_U16 usPacketSize;    //数据块大小
    HI_U32 ulUpgId;         //升级ID
    HI_U32 ulPacketNum;     //数据块编号,从0开始
    HI_U8 aucPacket[0];     //数据块
} HI_UPG_STD_TRANSMIT_REQ_S;

typedef struct
{
    HI_U16 usStruVer   : 6;  //协议版本号
    HI_U16 usStruSize  : 6;  //报文头长度
    HI_U16 reserved    : 4;
    HI_U16 usPacketNum;      //连续查询的块数
    HI_U32 ulPacketStart;    //起始块号,从0开始
    HI_U32 ulUpgId;          //升级ID
} HI_UPG_STD_STATUS_REQ_S;

typedef struct
{
    HI_U16 usStruVer   : 6;  //协议版本号
    HI_U16 usStruSize  : 6;  //报文头长度
    HI_U16 usUpgStatus : 4;  //升级状态
    HI_U16 usPacketNum;      //有效块数
    HI_U32 ulPacketStart;    //起始块号
    HI_U32 ulUpgId;          //升级ID
    HI_U8 aucBitmap[0];
} HI_UPG_STD_STATUS_IND_S;

typedef struct
{
    HI_U16 usStruVer   : 6;  //协议版本号
    HI_U16 usStruSize  : 6;  //报文头长度
    HI_U16 reserved    : 4;
    HI_U16 usResetTime;      //等待复位时间
    HI_U32 ulUpgId;          //升级ID
    HI_U32 ulTryRunningTime; //试运行时间
} HI_UPG_STD_AFFIRM_REQ_S;

typedef HI_UPG_STA_INFO_REQ_S HI_UPG_STD_STA_INFO_REQ_S;

typedef struct
{
    HI_U16 usStruVer  : 6;  //协议版本号
    HI_U16 usStruSize : 6;  //报文头长度
    HI_U16 reserved1  : 4;
    HI_U8 reserved2;
    HI_U8 ucInfoItemCnt;   //信息列表元素个数
    HI_U32 ulUpgId;        //升级ID
    HI_U8 aucInfoItem[0];  //信息列表数据字段列表，顺序放置1个或多个信息字段(元素ID(1字节)+元素数据长度(1字节)+元素数据(L))
} HI_UPG_STD_STA_INFO_IND_S;
#else
typedef struct
{
    HI_U32 ulUpgId;
    HI_U16 usLastBlkIdx;
    HI_U16 ulErrorCode;
    HI_U32 ulRcvFileSize;
    HI_U32 ulBlkSize;
    HI_U32 ulFileSize;
    HI_U8  usUpgStatus;
    HI_U8  aucVer[3];
    HI_U16 ausVer[2];
    HI_U32 ulChipVer;
    HI_U16 usLeftTimeWindow;
    HI_U8  aucBuildDataTime[6];
    HI_U16 usUpgReqSn;
    HI_U8  ucMacAddr[HI_PLC_MAC_ADDR_LEN];
} HI_UPG_STATUS_IND_S;
#endif


typedef struct
{
    HI_U8 ucDevType;
    HI_U8 usDevNum;
    HI_U8 ucDevAddr[HI_COL_METER_NUM_MAX][HI_PLC_MAC_ADDR_LEN];
} HI_ND_MODULE_DETECT_IND_S;
//****************************************************************************
#define HI_LOGIN_AUTH_RESULT_ID_SUCCESS 0
#define HI_LOGIN_AUTH_RESULT_ID_ACCOUNT_LOCKED 1
#define HI_LOGIN_AUTH_RESULT_ID_BUSY 20
#define HI_LOGIN_AUTH_RESULT_ID_PASS_WORD_WRONG 47
#define HI_LOGIN_AUTH_RESULT_ID_TIMEOUT 51
#define HI_LOGIN_AUTH_RESULT_ID_SYSTEM_CALL_ERROR 2999
typedef struct
{
    HI_U32 errCode;//HI_LOGIN_AUTH_RESULT_ID_XXX
    HI_U16 mainVer;//交互命令主版本
    HI_U16 subVer;//交互命令子版本
    HI_U32 magicNum;//魔术字
    HI_U32 id;//命令ID
    HI_U32 reserve;
    HI_U32 dataSize;//数据大小
    HI_U8 data[0];//数据内容
} HI_LOGIN_AUTH_RESULT_S;//密码验证结构体

#endif // defined(PRODUCT_CFG_CORE_TYPE_BOOTLOADER) || defined(PRODUCT_CFG_HSO)
//*****************************************************************************



//*****************************************************************************
#endif // __HI_NDM_H__

