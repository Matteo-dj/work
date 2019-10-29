//*****************************************************************************
//
//                  版权所有 (C), 2001-2011, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_fw_n.h
//  版 本 号   : V1.0
//  作    者   : 沈汉坤/00130424
//  生成日期   : 2011-10-25
//  功能描述   : MRS基础框架内部接口, 仅供mrs_fw.c文件使用，其它文件不能使用
//  函数列表   : NA
//  修改历史   :
//  1.日    期 : 2011-10-25
//    作    者 : 沈汉坤/00130424
//    修改内容 : 创建文件
//*****************************************************************************

#ifndef __MRS_FW_N_H__
#define __MRS_FW_N_H__
HI_START_HEADER


#include "mrs_fw_proto645.h"
#include "mrs_fw_proto376_2.h"


// 0 -- 0x100 为内部消息
#define MRS_MSG_BASE  0x1   // 注意与ndm_eqt_common.h中的NDM_EQT_MSG_BASE号段不能重复


#define MRS_MSG_BASE_MAX_NUM    (PRODUCT_CFG_Q_APP_MRS_MSG_BASE_NUM)

enum
{
    EN_MRS_FW_MSG_376_2_FRAME_INPUT = MRS_MSG_BASE,
    EN_MRS_FW_MSG_645_FRAME_INPUT,
    EN_MRS_FW_MSG_CSG_FRAME_INPUT,

    EN_MRS_FW_MSG_MAC_FRAME_INPUT,
    EN_MRS_FW_MSG_MAC_FRAME_OUTPUT,
    EN_MRS_FW_MSG_MR_FRAME_OUTPUT,
    EN_MRS_FW_MSG_MDM_INPUT,
    EN_MRS_FW_MSG_RM_PLC_SEND_REQ,
    EN_MRS_FW_MSG_NV_CHANGE_NOTIFY,
    EN_MRS_FW_MSG_ZERO_CROSS_NOTIFY,
    EN_MRS_FW_MSG_MR_RETRY_DIAG_CMD,
    EN_MRS_FW_MSG_MAC_ND_STATE_CHG,
    EN_MRS_FW_MSG_MAC_MSG,
    EN_MRS_FW_MSG_CLTII_EVENT_MON,
    EN_MRS_FW_MSG_MAC_JOIN_WHITE_LIST,
	EN_MRS_FW_MSG_DIAG_CMD_MSG,
    EN_MRS_FW_MSG_PAUSE_RESUME_DETECT,
    EN_MRS_FW_MSG_CCO_MR_TASK,
    EN_MRS_FW_MSG_POWER_FAILURE,
    EN_MRS_FW_MSG_TOPO_CHANGE_NOTIFY,
};


typedef struct
{
    HI_U16 usCmdId;
    HI_U8 reserved[2];
    MRS_FW_CMD_REQ_PFN pfnReq;  // 当前请求处理的函数指针，仅支持一个命令，为了简化处理，不支持多个命令并行支持
} MRS_CMD_NODE_STRU;

typedef struct
{
    HI_U16 usMsgId;
    HI_U8 reserved[2];
    MRS_FW_MSG_PROC_PFN pfnMsg;
} MRS_MSG_NODE_STRU;


typedef struct
{
    // Task
    HI_U32 aulTaskId[EN_APP_TASK_MAX];

    // Queue
    HI_U32 aulQueueId[EN_APP_Q_MAX];

    // Timer
    HI_TIMER_HANDLE_S astTimerHandle[EN_APP_TIMER_MAX];

    // Semaphore
    HI_U32 aulSemId[EN_APP_SEM_MAX];
} MRS_OS_RES_CFG_STRU;


// MRS 上下文结构定义
typedef struct
{
    HI_CHAR aucUserVersion[HI_USER_VER_LENGTH];

    HI_CONST MRS_FW_CMD_PROC_TBL_STRU* pCmdList; // 默认的命令处理列表
    HI_CONST MRS_FW_MSG_PROC_TBL_STRU* pMsgList; // 默认的消息处理列表
    HI_U16 usCmdNum; // 通道类的命令个数
    HI_U16 usMsgNum; // 消息个数

    MRS_CMD_NODE_STRU stCmdNode; // 当前处理的命令节点
    MRS_MSG_NODE_STRU stMsgNode; // 当前处理的消息节点

#if (defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) || defined(PRODUCT_CFG_PRODUCT_TYPE_STA))
    MRS_3762_PROTO_CTX_STRU st3762Proto;
#endif

    MRS_645_PROTO_CTX_STRU st645Proto[MRS_645BUF_MAX];

    HI_U32 ulSimuChlTimeout;    // 模拟通道空闲超时时间 单位:毫秒
    HI_BOOL bSimuChlConnect;    // HI_TRUE: 连接 HI_FALSE: 断开
    HI_U8 ucPlcChlStatus;
	HI_U8 ucPlcTopoStatus;  //拓扑组网状态 0-组网中 1-组网完成  2-优化完成
    HI_U8 ucEnableUartDbg;

    MRS_OS_RES_CFG_STRU stResCfg;
} MRS_FW_CTX_STRU;


// 发送消息给 MRS 队列
HI_U32 mrsSendMessage2Queue(HI_IN HI_SYS_QUEUE_MSG_S* pstMsg);

//获取通道端口
HI_DMS_CHL_PORT_OBJ_S mrsGetChlPort(HI_VOID);

MRS_FW_CTX_STRU * mrsGetFwCtx(HI_VOID);


//*****************************************************************************
// 函数名称: mrsFwChlInit
// 功能描述: MRS框架通道初始化
//           完成各个通道的初始化安装、回调函数注册功能
//           当识别为UART端口并且为II型采集器时将UART模式设置为485
//           当识别为IR端口后将模式设置为IR
// 参数说明:
//          pstChlPort : 通道回调函数实例
//          usPortNum  : 通道端口号
// 返 回 值:
//    HI_ERR_SUCCESS : 成功
//    其它值为错误码
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: liuxipeng/lkf54842 [2014-01-15]
//*****************************************************************************
HI_U32 mrsFwChlInit(HI_CONST HI_DMS_CHL_PORT_OBJ_S * pstChlPort, HI_U16 usPortNum);


HI_VOID mrsSetPlcForceMode(HI_VOID);
HI_VOID mrsPowerOnPro(HI_VOID);

HI_VOID mrsFwGetUserVer(HI_CHAR* pucVer);
HI_VOID mrsFwSetUserVer(NV_MRS_USER_VER_S* pstVer);

HI_VOID mrsFwEnableUartDbg(HI_U8 ucEnable);
HI_BOOL mrsIsUartDbgEnabled(HI_VOID);


HI_END_HEADER
#endif // __MRS_FW_N_H__




