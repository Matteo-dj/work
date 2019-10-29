//*****************************************************************************
//
//                  版权所有 (C), 1998-2011, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_srv_cco_remote_upg.h
//  版 本 号   : V1.0 
//  作    者   : fengxiaomin/f00209182
//  生成日期   : 2014-1-9
//  功能描述   : 远程升级模块函数及接口定义
//               
//  函数列表   : TODO: ...
//  修改历史   : 
//  1.日    期 : 2014-1-9
//    作    者 : fengxiaomin/f00209182 
//    修改内容 : 创建文件 
//
//*****************************************************************************
#ifndef __MRS_SERVICE_CCO_REMOTE_UPG_H__
#define __MRS_SERVICE_CCO_REMOTE_UPG_H__
HI_START_HEADER


/* BEGIN: Added by fengxiaomin/00209182, 2014/1/9   问题单号号:DTS2014012607684 */
#define MRS_CCO_UPG_ERROR_SEG          (0x0000FFFF)  //错误段标识
#define MRS_CCO_SINGAL_UPG_DATA_LEN_MAX   (1024) //每包升级数据最大长度

// Fn(2)+文件标识(1)+文件属性(1)+文件指令(1)+总段数n(2)+第i段标识(4)+数据长度lf(2)+数据(lf)
#define MRS_CCO_UPG_FIX_DATA_OFFSET    (13)    // 远程升级报文中数据域内容偏移长度
#define MRS_CCO_UPG_DATA_LEN_OFFSET    (11)  //远程升级报文中数据长度偏移长度
#define MRS_CCO_UPG_CURRENT_SEG_OFFSET   (7) // 远程升级报文中数据域内容中段标识位置的偏移长度
#define MRS_CCO_UPG_TOTAL_SEG_OFFSET   (5)   //远程升级报文中总段数偏移长度
#define MRS_CCO_UPG_FILE_PROPERTY_OFFSET  (3) //远程升级报文中文件属性偏移长度
#define MRS_CCO_UPG_FILE_FLAG_OFFSET   (2)   //远程升级报文中文件标识偏移长度

#define MRS_CCO_REMOTE_UPG_ID          HI_MDM_OBJ_ID_MRS  //MRS模块升级ID
#define MRS_CCO_REMOTE_UPG_OBJ_ID 	   HI_MDM_OBJ_ID_MRS  //MRS主站升级对象

#define MRS_CCO_UPGRADE_IDLE_PERIOD         (0) //MRS升级空闲阶段
#define MRS_CCO_UPGRADE_LOCAL_LOAD_PERIOD   (1) // MRS升级过程中本地下装阶段
#define MRS_CCO_UPGRADE_LOCAL_UPG_PERIOD    (2) // MRS升级过程中本地升级阶段

//文件标识
#define MRS_CCO_CLEAR_UPG_LOAD_FILE   (0x00) //清除下装文件
#define MRS_CCO_LOCAL_CCO_UPGRADE     (0x03) //本地CCO单点升级
#define MRS_CCO_BOARDCAST_UPGRADE      (0x07) //全网升级
#define MRS_CCO_STA_UPGRADE           (0x08) //全网STA升级

#define MRS_CCO_UPG_END_FRAME         (0x01) //结束帧
//升级文件类型
#define MRS_CCO_UPGRADE_FILE_CODE     (1)
#define MRS_CCO_UPGRADE_FILE_NV       (2)
#define MRS_CCO_UPGADE_FILE_BOOT      (3)

//文件检查标志
#define MRS_CCO_UPG_FILE_CHECK_FINISH       (1)  //升级文件检查完成
#define MRS_CCO_UPG_FILE_NEED_CHECK         (0)  //升级文件需要检查
#define MRS_CCO_UPG_FILE_CHECK_RESPONSION_OR_TIMEOUT      (1)  //升级文件检查已响应或超时
#define MRS_CCO_UPG_FILE_CHECK_NEED_RESPONSION  (0)  //升级文件检查未响应

//CCO收到清除下装标志
#define MRS_CCO_UPG_CLEAR_FILE_COMMAND_RX          (1) //CCO接收到清除下装命令
#define MRS_CCO_UPG_CLEAR_FILE_COMMAND_NON_RX      (0) //CCO没有接收到清除下装命令
/* BEGIN: Modified by fengxiaomin/00209182, 2014/5/6   问题单DTS2014050504643 */
#define MRS_CCO_UPG_CLEAR_FILE_COMMAND_RESPONSION_OR_TIMEOUT  (1) //CCO接收到清除下装命令后已经收到回应
/* END:   Modified by fengxiaomin/00209182, 2014/5/6 */
#define MRS_CCO_UPG_CLEAR_FILE_COMMAND_NEED_RESPONSION  (0) //CCO接收到清除下装命令后未回应
#define MRS_CCO_RESP_ACK_FALG   (1)
#define MRS_CCO_RESP_DENY_FALG  (0)
//本地升级阶段启动升级标志
#define MRS_CCO_NEED_START_UPG_PLC_BAD         (1)
#define MRS_CCO_NEED_START_UPG_PLC_OK          (0)

/* BEGIN: Modified by fengxiaomin/00209182, 2014/3/27   问题单号:DTS2014031100268 */
//允许升级标志
#define MRS_CCO_ALLOW_STOP_UPG  (0)
#define MRS_CCO_DENY_STOP_UPG   (1)
/* END:   Modified by fengxiaomin/00209182, 2014/3/27 */

//消息相关宏
#define MRS_CCO_UPG_FILE_CHECK_MSG_MAX_TIMES_MAX       (5)  //升级文件消息超时最大发送次数最大值
#define MRS_CCO_UPG_FILE_CHECK_MSG_MAX_TIMES_DEFAULT   (2)  //升级文件消息超时最大发送次数默认值
#define MRS_CCO_UPG_FILE_CHECK_MSG_MAX_TIMES_MIN       (1)  //升级文件消息超时最大发送次数最小值

#define MRS_CCO_REMOTE_UPG_START_FIX_BLKNUM  (1)
#define MRS_CCO_REMOTE_UPG_START_FIX_INDEX   (1)
//#define MRS_CCO_UPG_CMD_FAIL_MSG_TINE_INTERVAL_MAX      (60) //升级命令重发时间间隔最大值，单位为秒
//#define MRS_CCO_UPG_CMD_FAIL_MSG_TINE_INTERVAL_DEFAULT  (10) //升级命令重发时间间隔默认值，单位为秒
//#define MRS_CCO_UPG_CMD_FAIL_MSG_TINE_INTERVAL_MIN      (1) //升级命令重发时间间隔最小值，单位为秒

#define MRS_CCO_UPG_CMD_MSG_MAX_TIMES_MAX         (50)  //升级命令最大发送次数最大值
#define MRS_CCO_UPG_CMD_MSG_MAX_TIMES_DEFAULT     (6)  //升级命令最大发送次数默认值
#define MRS_CCO_UPG_CMD_MSG_MAX_TIMES_MIN         (1)  //升级命令最大发送次数最小值
/* BEGIN: PN: DTS2015042907476 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/29 */
#define MRS_CCO_UPG_CMD_WAIT_PLC_AVAILABLE_TIME   (180)
/* END:   PN: DTS2015042907476 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/29 */

#define MRS_CCO_UPG_FILE_RECEIVE_TIME_OUT_MAX       (2 * 60 * 60)  //升级下装阶段升级文件包接收超时时间最大值，单位为秒
#define MRS_CCO_UPG_FILE_RECEIVE_TIME_OUT_DEFAULT   (600)  //升级下装阶段升级文件包接收超时时间默认值，单位为秒
#define MRS_CCO_UPG_FILE_RECEIVE_TIME_OUT_MIN       (60)  //升级下装阶段升级文件包接收超时时间最小值，单位为秒

#define MRS_CCO_UPG_STATUS_QUERY_TIME_MAX       (1 * 60 * 60)   //MRS询问升级状态时间间隔最大值，单位为秒
#define MRS_CCO_UPG_STATUS_QUERY_TIME_DEFAULT   (60)   //MRS询问升级状态时间间隔默认值，单位为秒
#define MRS_CCO_UPG_STATUS_QUERY_TIME_MIN       (5)   //MRS询问升级状态时间间隔最小值，单位为秒

#define MRS_CCO_UPG_STATUS_QUERY_TIME_OUT     (2)   //MRS询问升级状态超时时间,单位为秒
#define MRS_CCO_UPG_STATUS_QUERY_POWERON_TIMES_MAX       (10)   //MRS询问升级状态次数最大值
#define MRS_CCO_UPG_STATUS_QUERY_POWERON_TIMES_DEFAULT   (5)   //MRS询问升级状态次数默认值
#define MRS_CCO_UPG_STATUS_QUERY_POWERON_TIMES_MIN       (1)  //MRS询问升级状态次数最小值

typedef NV_APP_MRS_UPG_PARAM_STRU MRS_CCO_REMOTE_UPG_PARAM_S;

typedef struct
{
    MRS_CCO_REMOTE_UPG_PARAM_S stUpgParam;
    HI_U8 reserved;
    HI_U8 ucUpgPeriod;      // mrs升级所处的阶段
    HI_U8 ucUpgFileFlag;   // 文件标识
    HI_U8 ucFileHeadCheckFlag:2;  //CCO接收到的升级文件头检查标志或结果  0--未检查，1--已检查，2--检查结果为错误
    HI_U8 ucResered:2;

    HI_U8 ucStartUpgFlag:1; //CCO进入本地升级阶段后是否启动升级标志，若组网优化未完成，值为1，若组网优化完成，值为0
    HI_U8 ucClearFileRxFlag:1;//CCO在升级过程中接收到清除下装标志 0--未收到，1--收到
    HI_U8 ucClearFileRxResp:1;//CCO在升级过程中接收到清除下装后回应标志 0--未回应，1--已回应
/* BEGIN: Modified by fengxiaomin/00209182, 2014/3/27   问题单号:DTS2014031100268 */
    HI_U8 ucAllowStopUpgFlag:1;//CCO在升级过程中是否允许停止升级标志 0--允许，1--不允许
/* END:   Modified by fengxiaomin/00209182, 2014/3/27 */
    HI_U32 ulTotalSegNum; // 总段数
    HI_U16 usSingleSegLen;// 每段长度
    HI_U16 usEndSegLen; //升级文件包中最后一段中数据域中升级数据长度
    HI_U8 ucFrameSeq;
    HI_U8 ucCurrCheckMsgTimes;  //当前消息发送次数
    HI_U8 ucCurrStopUpgMsgTimes;  //当前消息发送次数
    HI_U8 ucCurrStatusMsgTimes; //当前状态查询消息发送次数
    MRS_3762_FRAME_STRU  *pstFrame;
	HI_U32 ulClearFileSeg; // 清除下装命令中的段号
    HI_U32 ulCurrSegNum;  // 当前接收到的段号
    HI_U32 ulUpgradeFileLen; // 升级文件长度
    HI_U8 ucUpgId;
    HI_U8 reserved1[3];
    HI_MDM_UPG_START_REQ_S *pstUpgFileInfo;//升级文件的缓冲区指针
}MRS_REMOTE_UPG_MODULE;

HI_U32 mrsCcoRemoteUpgInit(HI_VOID);
MRS_REMOTE_UPG_MODULE* mrsGetRemoteUpgCtx(HI_VOID);
HI_U32 mrsRemoteUpgMsgRcv(HI_U32 ulId, HI_PBYTE pucPacket, HI_U16 usPacketSize);
HI_VOID mrsUpgStatusMsgRx(HI_MDM_UPG_STATUS_BS_IND_S *pstInd);
HI_VOID mrsUpgStopMsgRx(HI_MDM_UPG_STOP_IND_S *pstInd);
HI_VOID mrsUpgLocalPro(MRS_REMOTE_UPG_MODULE *pstMrsUpgCtx);
HI_U32 mrsInitRemoteUpgNvDefault(NV_APP_MRS_UPG_PARAM_STRU *pstNvCfg);
HI_VOID mrsAFN15F1ReportAck(HI_U32 ulSeg, HI_U8 ucFrameSeq);
HI_VOID mrsRemoteUpgReset(MRS_REMOTE_UPG_MODULE * pstMrsUpgCtx);
HI_U32 mrsRemoteUpgStart(HI_U16 ulId, HI_PVOID pData, HI_U32 ulFileSize);
HI_U32 mrsRemoteUpgStop(HI_U16 ulId, HI_PVOID pCmdParam, HI_U16 usCmdParamSize);
HI_U32 mrsRemoteUpgStatus(HI_VOID);
HI_BOOL mrsIsUpgFileFlagValid(HI_U8 ucFileFlag, HI_U8 ucFileProperty);
HI_BOOL mrsUpgFileLenValid(HI_U8 *pContent, HI_U8 ucDataLenOff, HI_U8 ucCurrSegOff);
HI_BOOL mrsUpgFileStartLenValid(HI_U32 ulTotalSegNum, HI_U16 usDataLen, HI_U8 ucFileFlag);
HI_BOOL mrsUpgFileEndLenValid(HI_U32 ulTotalSegNum, HI_U16 usDataLen, HI_U8 ucFileFlag);
HI_BOOL mrsIsCcoStatePermitUpg(HI_VOID);
HI_U32 mrsRemoteUpgClear(MRS_REMOTE_UPG_MODULE * pstMrsUpgCtx);
HI_U32 mrsRemoteUpgStartFramePro(HI_U8 * pContent);
HI_U32 mrsRemoteUpgMiddleFramePro(HI_U8 * pContent);
HI_U32 mrsRemoteUpgEndFramePro(HI_U8 * pContent);
HI_U32 mrsGetUpgType(HI_U8 ucFileFlag);
HI_U32 mrsUpgFileCheck(MRS_REMOTE_UPG_MODULE * pstMrsUpgCtx);
HI_U32 mrsUpgMiddleEndFrameValid(HI_U8 * pContent, HI_U8 ucDataLenOff, HI_U8 ucCurrSegOff);
HI_VOID mrsUpgStopTimeOutPro(HI_VOID);
HI_VOID mrsUpgStatusTimeOutPro(HI_VOID);
HI_VOID mrsRemoteUpgRxFilePro(HI_VOID);
HI_VOID mrsUpgRxFileTimeOutPro(HI_VOID);
HI_BOOL mrsIsAllowCcoReset(HI_VOID);
HI_BOOL mrsIsCcoUpgStatus(HI_VOID);
HI_VOID mrsStartQueryUpgStatus(HI_U8 ucPowerFlag, MRS_REMOTE_UPG_MODULE * pstMrsUpgCtx);
HI_BOOL mrsIsSegValid(HI_U8 * pContent, HI_U8 ucSegOffset, MRS_REMOTE_UPG_MODULE * pstMrsUpgCtx);
HI_VOID mrsPowerOnGetUpgStatus(HI_VOID);
HI_U32 mrsRemoteUpgMsgProc(HI_SYS_QUEUE_MSG_S* pstMsg);
HI_BOOL mrsIsMdmUpgIdValid(HI_U32 ulMdmMsgId);
HI_VOID mrsPowerOnStatusTimeOut(HI_VOID);
 HI_U32 mrsUpgRespClearFile(HI_U8 ucFlag, MRS_REMOTE_UPG_MODULE *pstMrsUpgCtx);
/* BEGIN: Modified by fengxiaomin/00209182, 2014/3/27   问题单号:DTS2014032809465*/
HI_U32 mrsUpgStatusEndPro(MRS_REMOTE_UPG_MODULE *pstMrsUpgCtx);
/* END:   Modified by fengxiaomin/00209182, 2014/3/27 */
HI_U8 mrsGetRemoteUpgStatus(HI_VOID);
HI_U32 mrsRemoteUpgFrameRxPro(HI_U8 *pContent, HI_U32 ulSeg, HI_U16 usTotalSegNum);
/* BEGIN: Modified by fengxiaomin/00209182, 2014/3/27   问题单号:DTS2014032809465 */
HI_U32 mrsRemoteUpgFree(MRS_REMOTE_UPG_MODULE * pstMrsUpgCtx);
/* END:   Modified by fengxiaomin/00209182, 2014/3/27 */

/* END:   Added by fengxiaomin/00209182, 2014/1/16 */

HI_END_HEADER
#endif //__MRS_SERVICE_H__

