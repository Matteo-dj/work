//*****************************************************************************
//
//                  版权所有 (C), 2001-2011, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : app_mrs.h
//  版 本 号   : V1.0
//  作    者   : 沈汉坤/00130424
//  生成日期   : 2011年09月06日
//  功能描述   : MRS对外接口
//  函数列表   : 无
//  修改历史   :
//  1.日    期 : 2011年09月06日
//    作    者 : 沈汉坤/00130424
//    修改内容 : 创建文件
//*****************************************************************************


#ifndef __APP_MRS_H__
#define __APP_MRS_H__
#include <app_common.h>
HI_START_HEADER

#define MRS_SRV_UART_PARITY_NONE        (0)
#define MRS_SRV_UART_PARITY_ODD         (1)
#define MRS_SRV_UART_PARITY_EVEN        (2)
#define MRS_SRV_BAUD_RATE_9600          (9600)
#define MRS_SRV_BAUD_RATE_115200        (115200)

typedef HI_U32 (*MRS_FW_MSG_PROC_PFN)(HI_U16 usId, HI_IN HI_PVOID);

typedef struct
{
    HI_U16 usId;

    // 发送: MAC地址,当表示目的地址时:取值全为零, 则表示发送到CCO, 全为FF则表示广播,其它为指定的地址;
    // 接收: 表示源MAC地址
    HI_U8 ucMac[HI_PLC_MAC_ADDR_LEN];
    HI_U8 is_bc_frame : 1;
    HI_U8 reserved : 7;
    HI_U8 ucOption;
	HI_U16 usPayloadLen; // 帧长度
    HI_PBYTE pucPayload;// 完整帧
}MRS_PLC_FRAME_DATA_S, MRS_PLC_FRAME_DATA_STRU;

//#define MRS_PLC_FRAME_DATA_STRU  MRS_PLC_FRAME_DATA_S

typedef HI_U32 (*MRS_FW_CMD_REQ_PFN)(HI_U16 usId, HI_IN HI_PVOID, HI_U16);

typedef struct
{
    HI_U16 usMinId;
    HI_U16 usMaxId;
    MRS_FW_CMD_REQ_PFN pfnReq;  // 请求处理的函数指针
} MRS_FW_CMD_PROC_TBL_STRU;


typedef struct
{
    HI_U16 usMinId;
    HI_U16 usMaxId;
    MRS_FW_MSG_PROC_PFN pfnMsg;  // 消息处理的函数指针
} MRS_FW_MSG_PROC_TBL_STRU;


//*****************************************************************************
// 函数名称: MRS_CmdRegister
// 功能描述: 命令处理注册
//
// 参数说明:
//   pstTbl [in] 命令处理入口表
//   usNum [in] 命令处理入口个数
//
// 返 回 值:
//    TODO: ...
//
// 调用要求: 当CCO/STA接收到对方的命令后，MRS框架会自动调用该注册的处理入口。
// 调用举例: TODO: ...
// 作    者: shenhankun/00130424 [2011-11-04]
//*****************************************************************************
HI_EXTERN HI_U32 MRS_CmdRegister(HI_CONST MRS_FW_CMD_PROC_TBL_STRU* pstTbl, HI_U16 usNum);


//*****************************************************************************
// 函数名称: MRS_MsgRegister
// 功能描述: 消息处理注册
//
// 参数说明:
//   pstTbl [in] 消息处理入口表
//   usNum [in] 消息处理入口个数
//
// 返 回 值:
//    TODO: ...
//
// 调用要求: 用于CCO/STA内部框架和服务之间的消息交互
// 调用举例: TODO: ...
// 作    者: shenhankun/00130424 [2011-11-04]
//*****************************************************************************
HI_EXTERN HI_U32 MRS_MsgRegister(HI_CONST MRS_FW_MSG_PROC_TBL_STRU* pstTbl, HI_U16 usNum);


//*****************************************************************************
// 函数名称: MRS_SendMrData
// 功能描述: 发送抄表数据给电表/集中器
//
// 参数说明:
//   pucData [in] MR数据
//   usDataSize [in] MR数据大小,单位字节
//
// 返 回 值:
//    HI_ERR_SUCCESS: 成功写入DMS队列中, 由DMS写入MR设备中
//    其它错误码
//
// 调用要求: 不支持自动填充前导字节等功能,仅提供通道功能发送数据的功能
// 调用举例: TODO: ...
// 作    者: shenhankun/00130424 [2011-11-04]
//*****************************************************************************
HI_EXTERN HI_U32 MRS_SendMrData(HI_IN HI_PBYTE pucData, HI_U16 usDataSize, HI_U16 ucChlType);

//*****************************************************************************
// 函数名称: MRS_UartWrite
// 功能描述: 发送抄表数据给串口(无消息传递直接写串口)
//
// 参数说明:
//   pucData [in] MR数据
//   usDataSize [in] MR数据大小,单位字节
//
// 返 回 值:
//    HI_ERR_SUCCESS: 成功写入Uart中
//    其它错误码
//
// 调用要求: 不支持自动填充前导字节等功能,仅提供通道功能发送数据的功能
// 调用举例: TODO: ...
// 作    者: liuxipeng/lkf54842 [2014-01-15]
//*****************************************************************************
#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
HI_EXTERN HI_U32 MRS_UartWrite(HI_IN HI_PBYTE pucData, HI_U16 usDataSize, HI_U16 ucChlType);
#endif

//*****************************************************************************
// 函数名称: MRS_SendPlcFrame
// 功能描述: 发送PLC帧的数据给CCO/STA
//
// 参数说明:
//   pstFrameData [in] MR帧数据
//
// 返 回 值:
//    HI_ERR_SUCCESS: 成功写入系统队列中, 由底层模块负责发送到其它通信模块.
//    其它错误码
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: shenhankun/00130424 [2011-11-04]
//*****************************************************************************
HI_EXTERN HI_U32 MRS_SendPlcFrame(HI_IN MRS_PLC_FRAME_DATA_S* pstFrameData);

HI_EXTERN HI_U32 MRS_SendFrameProc(HI_IN MRS_PLC_FRAME_DATA_S* pstFrameData);

//*****************************************************************************
// 函数名称: MRS_ChangeBaudRate
// 功能描述: 更改波特率
//
// 参数说明:
//   ulRate 波特率
//
// 返 回 值:
//    HI_ERR_SUCCESS: 成功修改波特率
//    其它错误码
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: shenhankun/00130424 [2012-09-22]
//*****************************************************************************
HI_EXTERN HI_U32 MRS_ChangeBaudRate(HI_U32 ulRate, HI_U8 ucParity);
HI_EXTERN HI_U32 mrsGetCurBaudRate(HI_VOID);

HI_EXTERN HI_U8 mrsGetPlcChlStatus(HI_VOID);
HI_EXTERN HI_U8 mrsGetTopoStatus(HI_VOID);
HI_EXTERN HI_VOID mrsSetTopoStatus(HI_U8);

HI_EXTERN HI_VOID mrsFreePlcFrameDataStru(MRS_PLC_FRAME_DATA_STRU *pstPlcFrameDataStru);

HI_END_HEADER
#endif // __APP_MRS_H__

