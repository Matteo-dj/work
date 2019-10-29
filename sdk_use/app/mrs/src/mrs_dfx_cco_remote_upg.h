//*****************************************************************************
//
//                  版权所有 (C), 1998-2011, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_dfx_cco_remote_upg.h
//  版 本 号   : V1.0 
//  作    者   : fengxiaomin/f00209182
//  生成日期   : 2014-1-15
//  功能描述   : 远程升级模块函数及接口定义
//               
//  函数列表   : TODO: ...
//  修改历史   : 
//  1.日    期 : 2014-1-15
//    作    者 : fengxiaomin/f00209182 
//    修改内容 : 创建文件 
//
//*****************************************************************************
/* BEGIN: Added by fengxiaomin/00209182, 2014/1/15   问题单号号:DTS2014012607684  */
#ifndef __MRS_DFX_CCO_REMOTE_UPG_H__
#define __MRS_DFX_CCO_REMOTE_UPG_H__
HI_START_HEADER

//CCO的工作状态
#define MRS_DFX_CCO_REMOTE_UPG_PREPARE        (1)
#define MRS_DFX_CCO_LOCAL_READ_METER_PERIOD   (2)
#define MRS_DFX_CCO_PLC_SEARCH_PERIOD         (3)
#define MRS_DFX_CCO_REMOTE_UPG_PERIOD         (4)

//升级状态
#define MRS_DFX_CCO_UPG_FILE_LOAD_STATUS         (1)
#define MRS_DFX_CCO_UPG_LOCAL_UPG_STATUS         (2)
#define MRS_DFX_CCO_UPG_NOMAL_FINISH_STATUS      (3)
#define MRS_DFX_CCO_UPG_TEN_MINUTE_EXIT_STATUS   (4)
#define MRS_DFX_CCO_UPG_START_UPG_FAIL_STATUS    (5)
#define MRS_DFX_CCO_UPG_RX_STOP_UPG_STATUS       (6)

//远程升级错误码
#define MRS_DFX_CCO_UPG_FILE_HEAD_ERR            (1)
#define MRS_DFX_CCO_UPG_TOTAL_SEG_DIFF_START_ERR   (2)
#define MRS_DFX_CCO_UPG_MIDDLE_FRAME_LEN_DIFF_START_ERR  (4)
#define MRS_DFX_CCO_UPG_FILE_FLAG_DIFF_START_ERR    (8)
#define MRS_DFX_CCO_UPG_END_FRAME_LEN_OVER_START_ERR     (16)
#define MRS_DFX_CCO_UPG_SINGLE_LEN_OVERRUN_ERR   (32)
#define MRS_DFX_CCO_UPG_FILE_LEN_ERR             (64)
#define MRS_DFX_CCO_UPG_FILE_FLAG_OVERRUN_ERR    (128)
#define MRS_DFX_CCO_UPG_FILE_PROPERTY_OVERRUN_ERR   (256)
#define MRS_DFX_CCO_UPG_CURR_SEG_OVER_TOTAL_SEG_ERR  (512)
#define MRS_DFX_CCO_UPG_FILE_CRC_ERR    (1024)

//内存获取失败
/* BEGIN: Modified by fengxiaomin/00209182, 2014/3/27   问题单号:DTS2014032809465 */
#define MRS_DFX_UPG_GET_RELEASE_MEMORY_FAIL   (1)
/* END:   Modified by fengxiaomin/00209182, 2014/3/27 */

//升级文件类型
#define MRS_DFX_CCO_UPG_FILE_CODE       (1)
#define MRS_DFX_CCO_UPG_FILE_BOOT       (3)
#define MRS_DFX_CCO_UPG_CCO_STA_BOARDCAST   (4)
#define MRS_DFX_CCO_UPG_LOCAL_CCO       (5)
#define MRS_DFX_CCO_UPG_ALL_STA         (6)

//升级文件类型或升级方式
#define MRS_DFX_CCO_UPG_FILE_FLAG      (0)
//#define MRS_DFX_CCO_UPG_FILE_TYPE      (1)

//升级启动或停止返回值
#define MRS_DFX_CCO_UPG_RET_SCCESS   (0)
#define MRS_DFX_CCO_UPG_RET_FAIL     (1)
#define MRS_DFX_CCO_UPG_CMD_START    (0x10)
#define MRS_DFX_CCO_UPG_CMD_STOP    (0x20)

//消息ID
#define MRS_DFX_CCO_UPG_START_MSG_ID   (1)
#define MRS_DFX_CCO_UPG_STOP_MSG_ID    (2)
#define MRS_DFX_CCO_UPG_FILE_CHECK_MSG_ID  (4)
#define MRS_DFX_CCO_UPG_STATUS_QUERY_MSG_ID  (8)
#define MRS_DFX_CCO_UPG_TEN_MINUTE_RX_TIMEOUT_ID  (16)
#define MRS_DFX_CCO_UPG_QUERY_STATUS_TIMER_ID   (32)
#define MRS_DFX_CCO_UPG_FILE_HEAD_CHECK_TIMEOUT_ID (64)
#define MRS_DFX_CCO_UPG_FILE_CRC_TIMEOUT_ID   (128)
#define MRS_DFX_CCO_UPG_STOP_TIMEOUT_ID     (256)
#define MRS_DFX_CCO_UPG_MSG_MDM_INPUT_ID   (512)
#define MRS_DFX_CCO_UPG_STATUS_TIMEOUT_ID  (1024)

//MRS远程升级主动结束原因
#define MRS_DFX_CCO_UPG_NOMAL_END     (0)
#define MRS_DFX_CCO_UPG_TEN_MINUTE_NON_RX_END  (1)
#define MRS_DFX_CCO_UPG_MODULE_START_FAIL_END  (2)


typedef struct
{
    HI_U32 ulTime;
    HI_U8 pPayload[4];
}MRS_CCO_UPG_EVENT_INFO_S;

typedef struct
{
    HI_U16 usId;
    HI_U16 usRet;
}MRS_CCO_UPG_MSG_ID_INFO_S;

typedef struct
{
    HI_DSID_APP_REMOTE_UPG_INF_S stRemoteUpgInfo;
}MRS_CCO_REMOTE_UPG_DFX_S;


MRS_CCO_REMOTE_UPG_DFX_S* mrsDfxGetRemoteUpg(HI_VOID);
HI_U32 mrsDfxRemoteUpgInit(HI_VOID);
HI_VOID mrsDfxRemoteUpgReset(HI_VOID);
HI_U8 mrsDfxGetUpgStatus(HI_VOID);
HI_VOID mrsDfxRefreshClearUpg(HI_U8 ucFileFlag);
HI_VOID mrsDfxMrsStartUpgFileTime(HI_VOID);
HI_VOID mrsDfxMrsEndUpgFileTime(HI_VOID);
HI_VOID mrsDfxUpgStartTime(HI_VOID);
HI_VOID mrsDfxUpgEndTime(HI_VOID);
HI_VOID mrsDfxRefreshUpgStatus(HI_U8 ucUpgStatus);
HI_VOID mrsDfxRefreshFrameErrFlag(HI_U16 usUpgStatus);
/* BEGIN: Modified by fengxiaomin/00209182, 2014/3/27   问题单号:DTS2014032809465*/
HI_VOID mrsDfxGetUpgMemoryFail(HI_VOID);
/* END:   Modified by fengxiaomin/00209182, 2014/3/27 */
HI_VOID mrsDfxGetUpgFileFlag(HI_U8 ucUpgFileFlag);
HI_VOID mrsDfxUpgCmdRet(HI_U8 ucCmd, HI_U32 ulRet, HI_U32 ulId);
HI_VOID mrsDfxUpgFileErrLen(HI_U32 ulFileLen);
HI_VOID mrsDfxRxUpgFrameNum(HI_VOID);
HI_VOID mrsDfxTxUpgFrameNum(HI_VOID);
HI_VOID mrsDfxRxUpgMsgNum(HI_VOID);
HI_VOID mrsDfxRxUpgIdErrMsgNum(HI_VOID);
HI_VOID mrsDfxTxUpgMsgNum(HI_VOID);
HI_VOID mrsDfxRxDrvMsgNum(HI_VOID);
HI_VOID mrsDfxTxDrvMsgNum(HI_VOID);
HI_VOID mrsDfxTxMsgFailInfo(HI_U16 usId, HI_U32 ulRet);
HI_U16 mrsDfxGetFrameErrFlag(HI_VOID);
HI_VOID mrsDfxRxMsgTotalNum(HI_VOID);
HI_VOID mrsDfxTxMrsMsgNum(HI_VOID);
HI_VOID mrsDfxRxMsgIdErrNum(HI_VOID);
HI_VOID mrsDfxSendMsgFail(HI_U16 usId, HI_U16 usRet);
HI_VOID mrsDfxUpgStatusRxErr(HI_U8 ucStatus);
/* BEGIN: Modified by fengxiaomin/00209182, 2014/3/27   问题单号:DTS2014032809465*/
HI_VOID mrsDfxReleaseMemoryFail(HI_VOID);
/* END:   Modified by fengxiaomin/00209182, 2014/3/27 */
/* BEGIN: Modified by fengxiaomin/00209182, 2014/3/27   问题单号:DTS2014031100268 */
HI_VOID mrsDfxStartUpgRxStop(HI_VOID);
/* END:   Modified by fengxiaomin/00209182, 2014/3/27 */
HI_VOID mrsDfxRxUpgMsgStart(HI_U32 ulRet);
HI_VOID mrsDfxRxUpgIdInd(HI_U32 ulStatus, HI_U32 ulIndUpgId, HI_U32 ulUpgId, HI_U8 ucPeriod);

HI_END_HEADER
#endif
/* END:   Added by fengxiaomin/00209182, 2014/1/16 */

