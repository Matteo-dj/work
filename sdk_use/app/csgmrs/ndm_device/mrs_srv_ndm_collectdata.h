/********************************************************************************
Copyright (C), Huawei Technologies Co., Ltd. 2012-2020. All rights reserved.
*********************************************************************************
File Name     : mrs_srv_ndm_collectdata.h
Version       : Initial draft
Author        : HCT
Created Date  : 2016-10-13
Last Modified by: c00233580
Description   : TODO: ...
Function List : TODO: ...
Change History: TODO: ...
********************************************************************************/

#ifndef _MRS_SRV_NDM_COLLECT_DATA_H_
#define _MRS_SRV_NDM_COLLECT_DATA_H_
#include <hi_types.h>
#include <hi_mdm_types.h>
#if defined(PRODUCT_CFG_PRODUCT_TYPE_NDM)

#include <hi_cmn_nv.h>
#include <hi_mdm_datacollect.h>
#include "dfx_app.h"

HI_START_HEADER

#define MS_PER_MINUTES                                  (60*1000)
#define SEC_PER_MINUTE                                  (60)

#define MRS_NDM_DC_HSO_MAX_TIME                         (6*60*1000) // Max len of time for hso collect data.

#define MRS_DC_SD_REBOOT_START_RETRY_TIMES              10          // 断电续采最多尝试10次

#define MRS_DC_SD_NORMAL_PERIOD_RETRY_TIME              (60*1000)   // 普通周期采发送消息失败重试延时时间
#define MRS_DC_SD_ZCROSS_RETRY_TIME                     (10*1000)   // 过零采发消息失败重试延时时间
#define MRS_DC_SD_WAIT_INTERRUPT_TIME                   (1*1000)    // 等待中断超时时间
#define MRS_DC_SD_WAIT_TS_OVER_TIME                     (10*1000)   // 等待时隙超时时间
#define MRS_DC_SD_TS_ENOUGHT_TIME                       (20*1000)   // 采数需要时隙时间

#define MRS_DC_SD_NEED_SD_CARD_MIN_SIZE                 (3*1024)    // 采数允许最小SD卡大小。单位KB
#define MRS_DC_SD_SD_CARD_ALERT_SIZE                    (200*1024)  // SD告警最小大小。单位KB

#define MRS_DC_SD_ZCROSS_FILE_COUNT                     10          // 过零采数文件个数

#define MRS_DC_SD_KEY_DOWN_ZCROSS_UP_MIN_TIME           (1000)      // ms
#define MRS_DC_SD_KEY_DOWN_ZCROSS_UP_MAX_TIME           (3000)      // ms
#define MRS_DC_SD_DEFAULT_NV_VALUE_RAM_SIZE             0x200000    //默认采数RAM大小
#define MRS_DC_SD_DEFAULT_BEFORE_POINT_LEN              0x60000     //向前采大小，定死
#define MRS_DC_SD_DEFAULT_AFTER_POINT_LEN               0x20000     //向后采大小,定死
#define MRS_DC_SD_SD_CARD_MAX_ERR_TIMES                 5           // SD卡错误大于该值，停止采数
#define MRS_DC_SD_UNFOLLOW_MIN_PERIOD_TIME              3           // 非跟随模式下，周期采最小周期。minutes
#define MRS_DC_SD_FOLLOW_MIN_PERIOD_TIME                3           // 跟随模式下，周期采最小周期。minutes
#define MRS_DC_SD_UNFOLLOW_MAX_TIMES_PER_PERIOD         10          // 非跟随模式下，最大每轮采集次数
#define MRS_DC_SD_FOLLOW_TIMES_PER_PERIOD               1           // 跟随模式下，周期采每轮采集次数
#define MRS_DC_SD_UNFOLLOW_MIN_TRIGER_TIME              2           // 非跟随触发采，最小触发存储时间。单位分钟
#define MRS_DC_SD_FOLLOW_MIN_TRIGER_TIME                2           // 跟随触发采，最小触发存储时间。单位分钟

#define MRS_DC_SD_UN_FIX_AGCGAIN                        ((HI_S32)0xFFFFFFFF)

#define MRS_DC_SD_MAX_FILE_COUNT_PER_FOLDER             500         // 每个文件夹下最多允许有多少个文件
#define MRS_DC_SD_FILE_NAME_MAX_LEN                     30          // SD卡支持文件名长度
#define MRS_DC_SD_MAX_SEQUENCE_FOLDER_NUM               255         // 最大序号文件夹序号

#define MRS_DC_SD_MAX_SEQ_NUM                           1000        // 最大流水号
#define MRS_DC_SD_DC_TYPE_BYTE_LEN                      5           // 采数类型
#define MRS_DC_SD_MAX_NAME_ID                           1000        // 最大NameID.
#define MRS_DC_SD_MAX_TIME_SECOND                       1000000     // 最大时间。单位s
#define MRS_DC_SD_MAX_ERR_CODE                          0x10000     // 最大错误码

#define MRS_DC_SD_BASE_FOLDER_NAME                      "/sd_dc"
#define MRS_DC_SD_BASE_FOLDER_NAME_SLANT_LEN            7                           // "/sd_dc/"
#define MRS_DC_SD_USER_FOLDER_NAME_SLANT_LEN            (NDM_DC_FOLDER_NAME_SIZE+1) // "DC_XXX/"
#define MRS_DC_SD_SEQUENCE_FOLDER_NAME_SLANT_LEN        4                           // "255/"
// 文件路径长度，不包含文件名
#define MRS_DC_SD_TOTAL_FOLDER_SLANT_LEN \
    (MRS_DC_SD_BASE_FOLDER_NAME_SLANT_LEN + MRS_DC_SD_USER_FOLDER_NAME_SLANT_LEN + MRS_DC_SD_SEQUENCE_FOLDER_NAME_SLANT_LEN)
// 文件路径 + 文件名 最大长度
#define MRS_DC_SD_TOTAL_PATH_NAME_LEN  \
    (MRS_DC_SD_TOTAL_FOLDER_SLANT_LEN + MRS_DC_SD_FILE_NAME_MAX_LEN)

// Data collect msg id
enum
{
    // Hso Dc
    EN_NDM_MSG_DC_HSO_UPLOAD_DATA,
    EN_NDM_MSG_DC_HSO_UPLOAD_START,
    EN_NDM_MSG_DC_HSO_UPLOAD_STOP,

    // Sd Dc
    EN_NDM_MSG_DC_SD_START,                 // 开始采数主流程
    EN_NDM_MSG_DC_SD_NORMAL_PERIOD,         // 普通周期采
    EN_NDM_MSG_DC_SD_NORMAL_TRIGER_MONITOR, // 普通触发采轮询
    EN_NDM_MSG_DC_SD_ZCROSS,                // 过零采
    EN_NDM_MSG_DC_SD_FOLLOW_TRIGER_MONITOR, // 跟随触发采轮询
    EN_NDM_MSG_DC_SD_FOLLOW_PERIOD_MONITOR, // 跟随周期采轮询
    EN_NDM_MSG_DC_STOP,                     // 停止采数
    EN_NDM_MSG_DC_REBOOT_START,             // 断电续采
    EN_NDM_MSG_DC_CHECK_TIME,               // 校时(设置3518时间)
};

// Hso collect data control struct
typedef struct
{
    HI_BUF_S strDcInfo;
    HI_PVOID pDCAddr;
} MRS_NDM_DC_HSO_CTRL_STRU;

// SD卡采数主流程状态
typedef enum
{
    MRS_DC_SD_MAIN_PHASE_STOPPED,        // 采数已停止或初始状态
    MRS_DC_SD_MAIN_PHASE_STARTED,        // 采数已开始
    MRS_DC_SD_MAIN_PHASE_STOPPING,       // 采数停止过程中

    MRS_DC_SD_MAIN_PHASE_STARING_ID_ST,  // 开始状态起始ID
    MRS_DC_SD_MAIN_PHASE_STARING_NORMAL = MRS_DC_SD_MAIN_PHASE_STARING_ID_ST, //普通采数开始中
    MRS_DC_SD_MAIN_PHASE_STARTING_PROMPT_ZCROSS_UP,      //提示上升沿采数
    MRS_DC_SD_MAIN_PHASE_STARTING_ZCROSS_UP,             //上升沿采数开始中
    MRS_DC_SD_MAIN_PHASE_STARTING_PROMPT_ZCROSS_DOWN,    //提示下降沿采数
    MRS_DC_SD_MAIN_PHASE_STARTING_ZCROSS_DONW,           //下降沿采数开始中
    MRS_DC_SD_MAIN_PHASE_STARTING_REBOOT,                //断电续采开始流程中
    MRS_DC_SD_MAIN_PHASE_STARING_ID_END = MRS_DC_SD_MAIN_PHASE_STARTING_REBOOT,//开始状态结束ID
} MRS_DC_SD_MAIN_PHASE_E;

// SD卡采数主流程状态变化 触发原因
typedef enum
{
    MRS_DC_SD_CHANGE_PHASE_REASON_KEY_DOWN,     //按键按下
    MRS_DC_SD_CHANGE_PHASE_REASON_KEY_UP,       //按键抬起
    MRS_DC_SD_CHANGE_PHASE_REASON_REBOOT_START, //重启续采

    MRS_DC_SD_CHANGE_PHASE_REASON_SD_CADR_ERR,  //Sd卡错误
    MRS_DC_SD_CHANGE_PHASE_REASON_COMPLETE,     //采数完毕
    MRS_DC_SD_CHANGE_PHASE_PROCEES_NEED,        //其它触发原因,流程到这就应该这样变化
} MRS_DC_SD_CHANGE_PHASE_REASON_E;

//采数启动原因
typedef enum
{
    MRS_DC_SD_START_TYPE_KEY_NORNAL,
    MRS_DC_SD_START_TYPE_KEY_ZCROSS_UP,
    MRS_DC_SD_START_TYPE_KEY_ZCROSS_DOWN,
    MRS_DC_SD_START_TYPE_REBOOT,
} MRS_DC_SD_START_TYPE_E;

//采数无法开始错误原因
typedef enum
{
    MRS_DC_SD_NOT_START_REASON_NONE,

    MRS_DC_SD_NOT_START_REASON_MEM_GET_FAIL,            // 获取内存空间失败
    MRS_DC_SD_NOT_START_REASON_NV_READ_FAIL,            // 读取NV失败
    MRS_DC_SD_NOT_START_REASON_NV_WRITE_FAIL,           // 写NV失败
    MRS_DC_SD_NOT_START_REASON_NV_CONFIG_WRONG,         // NV配置错误
    MRS_DC_SD_NOT_START_REASON_PREPARE_FAIL,            // 采数准备失败
    MRS_DC_SD_NOT_START_REASON_SEND_CHECK_TIME_FAIL,    // 发送校时消息失败

    MRS_DC_SD_NOT_START_REASON_SD_NOT_EXIST = 0x10, // SD卡不在位
    MRS_DC_SD_NOT_START_REASON_SD_GET_SIZE_FAIL,
    MRS_DC_SD_NOT_START_REASON_SD_NO_SPACE,         // 空间不足 (不够使用)
    MRS_DC_SD_NOT_START_REASON_SD_API_CALL_FAIL,    // 调用API失败
    MRS_DC_SD_NOT_START_REASON_REGIST_MAC_FUNC_FAIL = 0x20,//MAC错误
    MRS_DC_SD_NOT_START_REASON_ENABLE_FOLLOW_FAIL,  // 空间即将不足 (达到告警值)

} MRS_DC_SD_NOT_START_REASON_E;

// 过零采数触发种类
typedef enum
{
    MRS_DC_SD_ZCROSS_ISR_TYPE_FALL,
    MRS_DC_SD_ZCROSS_ISR_TYPE_RISE,
    MRS_DC_SD_ZCROSS_ISR_TYPE_UNKNOWN,
} MRS_DC_SD_ZCROSS_ISR_TYPE_E;

// SD卡采数停止原因
typedef enum
{
    MRS_DC_SD_STOP_TYPE_KEY,                // 按键停止采数
    MRS_DC_SD_STOP_TYPE_SD_CARD_ERR_TIMES,  // SD卡错误停止采数
    MRS_DC_SD_STOP_TYPE_COMPLETE,           // 采数完毕自动停止
} MRS_DC_SD_STOP_TYPE_E;

//显示ID
typedef enum
{
    MRS_DC_SD_DISPLAY_DC_NONE,              // 采数状态：空
    MRS_DC_SD_DISPLAY_DC_NORMAL,            // 采数状态：普通采数，跟随采数
    MRS_DC_SD_DISPLAY_DC_ZERO_CROSS_UP,     // 采数状态: 过零上升沿采数
    MRS_DC_SD_DISPLAY_DC_ZERO_CROSS_DOWN,   // 采数状态：过零下降沿采数

    MRS_DC_SD_DISPLAY_ALERT_FULL,           // SD卡空间不足
    MRS_DC_SD_DISPLAY_ALERT_SD_ERR,         // SD卡错误
    MRS_DC_SD_DISPLAY_ALERT_NONE,           // 无告警
} MRS_DC_SD_DISPLAY_E;

// SD卡告警状态
typedef enum
{
    MRS_DC_SD_ALERT_TYPE_NONE,          // 无告警
    MRS_DC_SD_ALERT_TYPE_SD_CARD_FULL,  // 空间不足
    MRS_DC_SD_ALERT_TYPE_SD_CARD_ERR,   // SD卡错误
    MRS_DC_SD_ALERT_TYPE_SD_CARD_MAX
} MRS_DC_SD_ALERT_TYPE_E;


#define MRS_DC_SD_DATA_COLLECT_TYPE_NOISE       (0x0001)
#define MRS_DC_SD_DATA_COLLECT_TYPE_FC          (0x0002)
#define MRS_DC_SD_DATA_COLLECT_TYPE_STAT4       (0x0004)
#define MRS_DC_SD_DATA_COLLECT_TYPE_DEFAULT     (0x0008)    /*已下与DRV无关 周期采数配置*/

//文件类型,根据文件类型不同存储不同文件名
typedef enum
{
    MRS_DC_SD_SAVE_FILE_TYPE_A,     //时隙触发时间足够
    MRS_DC_SD_SAVE_FILE_TYPE_P,     //时隙触发时间不足

    MRS_DC_SD_SAVE_FILE_TYPE_D,     //盲采
    MRS_DC_SD_SAVE_FILE_TYPE_F,     //FC
    MRS_DC_SD_SAVE_FILE_TYPE_N,     //噪声
    MRS_DC_SD_SAVE_FILE_TYPE_S,     //状态4
    MRS_DC_SD_SAVE_FILE_TYPE_ZCROSS_FALL,//过零下降沿
    MRS_DC_SD_SAVE_FILE_TYPE_ZCROSS_RISE,//过零上升沿
    MRS_DC_SD_SAVE_FILE_TYPE_MAX,
} MRS_DC_SD_SAVE_FILE_TYPE_E;

// 所有采数类型
#define MRS_DC_SD_DATA_COLLECT_TYPE_ALL             \
    (MRS_DC_SD_DATA_COLLECT_TYPE_NOISE | MRS_DC_SD_DATA_COLLECT_TYPE_FC | \
     MRS_DC_SD_DATA_COLLECT_TYPE_STAT4 | MRS_DC_SD_DATA_COLLECT_TYPE_DEFAULT)
// 非跟随: 所有类型
#define MRS_DC_SD_DATA_COLLECT_TYPE_UNFOLLOW_ALL    \
    (MRS_DC_SD_DATA_COLLECT_TYPE_NOISE | MRS_DC_SD_DATA_COLLECT_TYPE_FC | \
     MRS_DC_SD_DATA_COLLECT_TYPE_STAT4 | MRS_DC_SD_DATA_COLLECT_TYPE_DEFAULT)
// 跟随: 所有类型
#define MRS_DC_SD_DATA_COLLECT_TYPE_FOLLOW_ALL      \
    (MRS_DC_SD_DATA_COLLECT_TYPE_NOISE | MRS_DC_SD_DATA_COLLECT_TYPE_DEFAULT)
// 噪声: 所有类型
#define MRS_DC_SD_DATA_COLLECT_TYPE_NOISE_ALL       \
    (MRS_DC_SD_DATA_COLLECT_TYPE_NOISE | MRS_DC_SD_DATA_COLLECT_TYPE_DEFAULT)
// 触发:所有类型
#define MRS_DC_SD_DATA_COLLECT_TYPE_TRI_ALL         \
    (MRS_DC_SD_DATA_COLLECT_TYPE_NOISE | MRS_DC_SD_DATA_COLLECT_TYPE_FC | MRS_DC_SD_DATA_COLLECT_TYPE_STAT4)

HI_EXTERN HI_NDM_DATA_COLLECT_STATUS_MSG_S g_mrsNdmDcSdCtrl;

#define MRS_NDM_DC_SD_GET_MAIN_CTRL_CTX()       (&g_mrsNdmDcSdCtrl.stMainCtrlCtx)
#define MRS_NDM_DC_SD_GET_ERR_CTX()             (&g_mrsNdmDcSdCtrl.stErrCtx)
#define MRS_NDM_DC_SD_GET_CONFIG_CTX()          (&g_mrsNdmDcSdCtrl.stConfigCtx)
#define MRS_NDM_DC_SD_GET_STAT_CTX()            (&g_mrsNdmDcSdCtrl.stStatCtx)

/**
 @brief NDM HSO collect data.
    HSO collect data steps:
        1. HSO send cmd ID_DIAG_CMD_DRV_UPLOAD_DATA_START to collect data.
        2. board begin collect data with params from cmd.
            if success, return HI_ERR_SUCCESS and data size to HSO, and then, upload 10k data with mrsNdmMsgDcUploadData function.
            if fail, return err code.
        3. when HSO has received first 10k data, if need more data, HSO send cmd ID_DIAG_CMD_DRV_UPLOAD_DATA for next 10k data.
        4. when board get ID_DIAG_CMD_DRV_UPLOAD_DATA_STOP cmd, stop collect data.
 @attention \n
N/A
 @see \n
N/A
 */
HI_U32 mrsNdmDcHsoDiagCmdProc(HI_U16 usId, HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption);

HI_U32 mrsNdmDcMsgProc(HI_U16 usId, HI_PVOID pMsgParam);

HI_U32 mrsNdmDcSendSubMsg(HI_U32 ulMsgId, HI_PVOID pData, HI_U32 ulDataSize, HI_U32 ulOption);

HI_U32 mrsNdmMsgDcUploadDataStart(HI_U16 usId, HI_PVOID pMsgParam);
HI_U32 mrsNdmMsgDcUploadData(HI_U16 usId, HI_PVOID pMsgParam);
HI_U32 mrsNdmMsgDcUploadDataStop(HI_U16 usId, HI_PVOID pMsgParam);

HI_VOID mrsNdmDcHsoTimeout(HI_VOID);

/**
 @brief KEY Collect Data.
 */

/**
 @brief     Handle collect data key up/down event.
 @attention \n
N/A
 @param[in] bIsKeyDown is key down.
 @param[in] uMs key press time, measure in millisecond.
 @see \n
N/A
 */

// HSO命令行查询SD卡采数状态
HI_U32 mrsNdmMsgDcSdReportStat(HI_U16 usId, HI_PVOID pMsgParam);
// HSO->调试控制台->数据采集->存储采集界面: 设置SD卡采数参数
HI_U32 mrsNdmMsgDcSdSetConfig(HI_U16 usId, HI_PVOID pMsgParam);
// HSO->调试控制台->数据采集->存储采集界面: 查询SD卡采数参数
HI_U32 mrsNdmMsgDcSdQueryConfig(HI_U16 usId, HI_PVOID pMsgParam);

// 按键消息处理
HI_VOID mrsNdmDcKeyMsgProc(HI_BOOL bIsKeyDown, HI_U32 uMs);
// 发送采数流程开始消息
HI_U32 mrsNdmDcSdSendStartMsg(MRS_DC_SD_START_TYPE_E eStartType);
// 开始采数流程
HI_U32 mrsNdmDcSdStart(HI_VOID);
// 采数准备
HI_U32 mrsNdmDcSdPrepare(HI_VOID);
// 进行采数
HI_U32 mrsNdmDcSdCollect(HI_VOID);
// 采数恢复
HI_U32 mrsNdmDcSdRestore(HI_VOID);
// 启动停止采数流程
HI_U32 mrsNdmDcSdStop(MRS_DC_SD_STOP_TYPE_E eStopType);
// 停止采数流程第一阶段
HI_U32 mrsNdmDcSdStopPhase1(MRS_DC_SD_STOP_TYPE_E eStopType);
// 停止采数流程第二阶段
HI_U32 mrsNdmDcSdStopPhase2(MRS_DC_SD_STOP_TYPE_E eStopType);
// 处理停止采数消息
HI_U32 mrsNdmDcSdStopMsgHandle(HI_U16 usId, HI_PVOID pMsgParam);

// 普通周期采: 发送开始消息
HI_U32 mrsNdmDcSdSendPeriodCollectMsg(HI_VOID);
// 普通周期采: 采集数据
HI_U32 mrsNdmDcSdNromalPeriodCollect(HI_VOID);
// 普通触发采: 轮询轮询定时器处理函数
HI_U32 mrsNdmDcSdNormalTrigerTimeout(HI_VOID);
// 普通触发采: 监听收到PB块状态
HI_U32 mrsNdmDcSdNormalTrigerPbMonitor(HI_VOID);
// 普通触发采: 采集数据
HI_U32 mrsNdmDcSdTrigerCollect(HI_VOID);
// 普通采: 采集数据
HI_U32 mrsNdmDcSdNormalCollect(DC_TRIG_TYPE_ENUM eCollectType, MRS_DC_SD_SAVE_FILE_TYPE_E eSaveFileType);

// 跟随采: 轮询
HI_U32 mrsNdmDcSdFollowTimeout(HI_VOID);
// 跟随采: 是否保存文件
HI_BOOL mrsNdmDcSdFollowIsSaveFile(HI_U8 ucSceneType);
// 跟随触发采: 轮询: 监听收到PB块状态
HI_U32 mrsNdmDcSdFollowTrigerPbMonitor(HI_VOID);
// 跟随触发采: 采集数据
HI_U32 mrsNdmDcSdFollowTrigerCollect(DC_TRIG_TYPE_ENUM eCollectType);
// 跟随周期采: 轮询
HI_U32 mrsNdmDcSdFollowPeriodMonitor(HI_VOID);
// 跟随周期采: 采集数据
HI_U32 mrsNdmDcSdFollowPeriodCollect(DC_TRIG_TYPE_ENUM eCollectType);

// 过零采: 进行过零采
HI_U32 mrsNdmDcSdZcrossCollect(HI_VOID);
// 过零采: 通知进行下一次采集
HI_U32 mrsNdmDcSdNotifyDoNextZcrossCollect(HI_VOID);


/* 注意: 仅普通采和跟随采支持断电续采  */

// 断电续采: 检查是否需要断电续采，若需要，启动通道轮询定时器
HI_U32 mrsNdmDcSdCheckRebootStart(HI_VOID);
// 断电续采: 轮询定时器处理
HI_U32 mrsNdmDcSdRebootCheckTimeout(HI_VOID);
// 断电续采: 进行断电续采
HI_U32 mrsNdmDcSdRebootStart(HI_VOID);


// 生成采数输入参数
HI_U32 mrsNdmDcGenerateInputParam(HI_IN DC_TRIG_TYPE_ENUM eCollectType, HI_OUT DC_INPUT_PARA_STRU * pstDcInParam);
// 存储文件
HI_U32 mrsNdmDcSdSaveFile(HI_BUF_S *pstBuf, MRS_DC_SD_SAVE_FILE_TYPE_E eSaveFileType, HI_U32 ulErrCode);
// 检查创建文件夹
HI_U32 mrsNdmDcSdCheckCreatFolder(HI_VOID);
// 生成文件名
HI_U32 mrsNdmDcSdGenerateFileName(HI_IN MRS_DC_SD_SAVE_FILE_TYPE_E eSaveFileType, HI_IN HI_U32 ulErrCode,
                                  HI_OUT HI_CHAR *scFileName, HI_OUT HI_U32 *pulLen);
// 将文件存储到SD卡
HI_U32 mrsNdmDcSdSaveFileToSdCard(HI_BUF_S * pstBuf, HI_CHAR * scFileName, HI_U32 ulFileNameLen);


// 检查SD卡状态
HI_U32 mrsNdmDcSdCheckSdCardStat(HI_OUT MRS_DC_SD_ALERT_TYPE_E * eSdAlertType);
// 初始化采数配置
HI_U32 mrsNdmDcSdInitConfig(MRS_DC_SD_START_TYPE_E eStartType);
// 检查NV配置
HI_U32 mrsNdmDcSdCheckConfig(HI_VOID);
// 写配置到NV
HI_U32 mrsNdmDcSdWriteConfigToNv(HI_BOOL bIsColleting, HI_U16 usNameId);


// 更新主流程状态
HI_U32 mrsNdmDcSdChangeMainPhase(MRS_DC_SD_CHANGE_PHASE_REASON_E eTrigerReason, HI_U32 ulKeyDownUpTime);
// 显示LED
HI_VOID mrsNdmDcSdLedDisplay(MRS_DC_SD_DISPLAY_E displayId);
// 更新SD卡错误相关LED显示
HI_VOID mrsNdmDcSdUpdateSdCardLedAlert(MRS_DC_SD_ALERT_TYPE_E eSdAlertType);
// 3518校时 (与保存文件时间有关)
HI_U32 mrsNdmDcSdRtcSetTime(HI_VOID);


// 检查通用文件名
HI_U32 mrsNdmDcSdCheckFolderName(HI_CHAR scFolderName[NDM_DC_FOLDER_NAME_SIZE]);
// 检查采数类型
HI_U32 mrsNdmDcSdCheckTrigerType(HI_U16 usTrigerType, HI_BOOL bIsFollowMode);
// 检查RAM ADDR SIZE
HI_U32 mrsNdmDsSdCheckAddrAndLen(HI_U32 ulStartAddr, HI_U32 ulEndAddr, HI_U32 ulAfterPointlen);
// 检查噪声AGC
HI_U32 mrsNdmDcSdCheckFixAgcGain(HI_S32 slAgcGain);
// 检查周期时长
HI_U32 mrsNdmDcSdCheckPeriodTime(HI_U16 usMinute, HI_BOOL bFollowMode);
// 检查每轮支持采数次数
HI_U32 mrsNdmDcSdCheckTimesPerPeriod(HI_U8 ucTimes, HI_BOOL bFollowMode);
// 检查采数触发时长
HI_U32 mrsNdmDcSdCheckTrigerTime(HI_U16 usMinute, HI_BOOL bFollowMode);

HI_END_HEADER
#endif // defined(PRODUCT_CFG_PRODUCT_TYPE_NDM)

#endif // _MRS_SRV_NDM_COLLECT_DATA_H_

