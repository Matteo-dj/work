//*****************************************************************************
//
//                  版权所有 (C), 2001-2011, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_fw.h
//  版 本 号   : V1.0 
//  作    者   : 沈汉坤/00130424
//  生成日期   : 2011-10-25
//  功能描述   : MRS基础框架接口, 供MRS的Service功能实现引用 
//  函数列表   : NA
//  修改历史   : 
//  1.日    期 : 2011-10-25
//    作    者 : 沈汉坤/00130424
//    修改内容 : 创建文件 
//*****************************************************************************


#ifndef __MRS_FW_H__
#define __MRS_FW_H__
HI_START_HEADER

#include <app_mrs.h>
#include <app_nv.h>
#include <hi_usr_nv.h>
#include "mrs_cmd_msg.h"
#include "mrs_dfx.h"

#define MRS_PROTO645_DATAGRAM_DATA_LEN_MAX     200
#define MRS_PROTO645_DATAGRAM_HEADER_LEN_MAX   12
#define MRS_PROTO645_DATAGRAM_LEN_MAX   (MRS_PROTO645_DATAGRAM_DATA_LEN_MAX\
                                        +MRS_PROTO645_DATAGRAM_HEADER_LEN_MAX)

#define HI_SYS_TIMER_ONESHOT    HI_TIMER_TYPE_ONCE
#define HI_SYS_TIMER_PERIODIC   HI_TIMER_TYPE_PERIOD

//*****************************************************************************
// 函数名称: MRS_StartTimer MRS_StopTimer
// 功能描述: 启动/停止定时器
//           
// 参数说明:
//   ucTimeId [in] 取值为 [EN_APP_TASK_TIMER_MRS_1, EN_APP_TIMER_MAX)
//   ulPeriod [in] 定时周期,定时周期到后，会产生 消息 ID_MRS_MSG_MR_TIMER 通过注册的入口传递给Service.
//   ucOption [in] 取值为 HI_SYS_TIMER_PERIODIC 或是 HI_SYS_TIMER_ONESHOT 表示周期还是非周期
//
// 返 回 值: 
//    HI_ERR_SUCCESS : 成功 
//    其它为错误码 
//
// 调用要求: 仅在任务中的调用，不能在中断中调用 
//           ucOption取值为SAL_TIMER_ONESHOT，可以不需要调用MRS_StopTimer释放定时器
// 调用举例: TODO: ...
// 作    者: shenhankun/00130424 [2011-11-10]
//*****************************************************************************
HI_PUBLIC HI_U32 MRS_StartTimer(HI_U8 ucTimerId, HI_U32 ulPeriod, HI_U8 ucOption);
HI_PUBLIC HI_U32 MRS_StopTimer(HI_U8 ucTimerId);

HI_PUBLIC HI_U32 MRS_SendMessage(HI_U32 ulQueueId, HI_SYS_QUEUE_MSG_S *pstMsg, HI_U32 ulTimeOut);
HI_PUBLIC HI_U32 MRS_WaitMessage(HI_U32 ulQueueId, HI_SYS_QUEUE_MSG_S *pstMsg, HI_U32 ulTimeOut);
HI_PUBLIC HI_U32 MRS_GetQueueMsgNum(HI_U32 ulQueueId);

HI_PUBLIC HI_U32 MRS_WaitSem(HI_U32 ulSemId, HI_U32 ulTimeOut);
HI_PUBLIC HI_U32 MRS_SignalSem(HI_U32 ulSemId);


// 控制域结构体
typedef struct 
{
    // 通讯方式
    // ****************************
    // 0-保留
    // 1-集中式路由载波通信
    // 2-分布式路由载波通信
    // 3-电力线宽带载波通信
    // 4~9-备用
    // 10-微功率无线通信
    // 11~19-备用
    // 20-以太网通信
    // 21~63-备用
    // ****************************
    HI_U8 ucCommForm :6;

    HI_U8 ucPrm :1;   // 启动标志 0-从动站 1-启动站
    HI_U8 ucDir :1;   // 传输方向 0-下行 1-上行
    
    HI_U8 reserved[3];
}MRS_3762_CTRL_REALM_STRU; // 376.2-2009 控制域

// 信息域结构体(下行)
typedef struct
{
    HI_U8 ucRouteFlag      : 1;  // 路由标识 0-带路由 1-不带路由
    HI_U8 ucSubsidiaryFlag : 1;  // 附属节点标识 0-无附属节点 1-有附属节点
    HI_U8 ucModuleFlag     : 1;  // 通讯模块标识 0-集中器模块 1-子节点模块
    HI_U8 ucClashCheck     : 1;  // 冲突检测 0-无冲突检测 1-有冲突检测
    HI_U8 ucRelayLevel     : 4;  // 中继级别 0~15 0-表示无中继
    
    HI_U8 ucCHFlag         : 4;  // 信道标识 0~15 0-表示不区分信道
    HI_U8 ucCorrErrCode    : 4;  // 纠错编码标识 0~15 0-未编码 1-表示RS编码 2~15保留
    
    HI_U8 ucResByte; // 预计应答字节数
    HI_U8 reserved;
    
    HI_U16 usCommSpeed : 15; // 通讯速率
    HI_U16 usSpeedUnit : 1;  // 通讯速率单位 0-bps 1-kbps
    
    HI_U8 ucSeq;                // 报文序列号
    HI_U8 reserved1;
} MRS_3762_INFO_REALM_DL_STRU; // 信息域(下行)

// 信息域结构体(上行)
typedef struct
{
    HI_U8 ucRouteFlag   : 1; // 路由标识0-带路由 1-不带路由
    HI_U8 ucNone1       : 1; // 
    HI_U8 ucModuleFlag  : 1; // 通讯模块标识 0-集中器模块 1-子节点模块
    HI_U8 ucNone2       : 1; // 
    HI_U8 ucRelayLevel  : 4; // 中继级别 0~15 0-表示无中继
    
    HI_U8 ucCHFlag      : 4; // 信道标识 0~15 0-表示不区分信道 
    HI_U8 ucNone3       : 4; //  
    
    HI_U8 ucPhaseFlag   : 4; // 实测相线标志 0-不确定 1~3-代表1相2相3相
    HI_U8 ucMeterCH     : 4; // 电表通道特征
    
    HI_U8 ucCmdSignal   : 4; // 末级命令信号品质 0~15 0-无品质 1-最低品质
    HI_U8 ucResSignal   : 4; // 末级应答信号品质 0~15 0-无品质 1-最低品质
    
    HI_U8 ucEvtFlg      : 1; // 事件标志
    HI_U8 ucResrved     : 7; // 预留

    HI_U8 ucSeq;            // 报文序列号
    HI_U8 reserved[2];
} MRS_3762_INFO_REALM_UL_STRU;  // 信息域(上行)

// 地址域结构体
typedef struct 
{
    HI_U8 ucSrcAdd[HI_PLC_MAC_ADDR_LEN];        // 源地址,固定6字节
    HI_U8 ucRelayLevel;                      // 中继级别 0~15
    HI_U8 reserved1;
    HI_U8 ucRelayAdd[15][HI_PLC_MAC_ADDR_LEN];  // 中继地址 0~15
    HI_U8 ucDestAdd[HI_PLC_MAC_ADDR_LEN];       // 目标地址,固定6字节
    HI_U8 reserved[2];
}MRS_3762_ADDR_REALM_STRU; 

// 应用数据域
typedef struct
{
    HI_U8 ucAfn; // 应用功能码AFN
    HI_U8 reserved;
    HI_U16 usAppDataLen;         // 数据单元标识和数据单元的长度，如果该成员为0则表示无数据
    HI_U8 pucAppDataContent[0];  // 数据单元标识 和 数据单元 部分内容
} MRS_3762_APP_DATA_REALM_STRU;

// 用户数据区头部结构体
typedef struct 
{
    union
    {
       MRS_3762_INFO_REALM_DL_STRU  stInfRealmDL;          // 信息域下行
       MRS_3762_INFO_REALM_UL_STRU  stInfRealmUL;           // 信息域上行
    } uInfoRealm;
    MRS_3762_ADDR_REALM_STRU     stAddrRealm;            // 地址域
    MRS_3762_APP_DATA_REALM_STRU stAppDataRealm;         // 应用数据域
}MRS_3762_USER_DATA_HRD_STRU; 


// 376.2 帧头信息结构体
typedef struct 
{
    MRS_3762_CTRL_REALM_STRU stCtrl;
    MRS_3762_USER_DATA_HRD_STRU strUsrDataHrd; // 用户数据区头部定义
}MRS_3762_FRAME_STRU;


// 南网(广东电网)帧头信息结构体
typedef struct
{
    // 控制域
    HI_U8 reserved1  : 5;
    HI_U8 ucAddrFlag : 1;
    HI_U8 ucPrm      : 1;
    HI_U8 ucDir      : 1;

    HI_U8 reserved2[3];

    // 地址域
    HI_U8 aucSrcAddr[HI_METER_ADDR_LEN];
    HI_U8 aucDstAddr[HI_METER_ADDR_LEN];

    // 应用数据域
    HI_U32 ulDI;
    HI_U8  ucAfn;
    HI_U8  ucSeq;
    HI_U16 usAppDataLen;
    HI_U8 aucAppData[0];
} MRS_CSG_FRAME_STRU;


typedef struct
{
	HI_U16 ret;		//读id返回值，0:成功
	HI_U16 flag;	//读取的id来源，0:读自nv, 1:读自efuse
	HI_U8  chipId[24];
	HI_U8  mac[6];
	HI_U8  rsv[2];
}EQUIP_CHIP_ID_R_STRU;


typedef struct
{
	HI_U32 ret;
	HI_U8  chipId[24];
}EQUIP_CHIP_ID_W_STRU;

//ID存储错误码
typedef enum
{
	HI_ERR_READ_CHIP_ID_EFUSE_FAIL = 1,//读efuse失败
	HI_ERR_READ_CHIP_ID_NV_FAIL = 2,	   //读nv失败
	HI_ERR_READ_CHIP_ID_MAC_FAIL = 4,	   //读mac失败
	HI_ERR_WRITE_CHIP_ID_NV_FAIL = 8,	   //写nv失败
}DUT_CHIP_ID_RW_ERRNO_ENUM;


//*****************************************************************************
// 函数名称: MRS_Proto3762Dec
// 功能描述: 解析376.2协议的报文头，数据单元标识 和 数据单元 部分内容不解析
//           
// 参数说明:
//   pucDatagram [in] 报文
//   usDatagramSize [in] 报文大小，单位字节
//   ppstFrame [out] 指向 MRS_3762_FRAME_STRU 结构, 用于保存解析后的帧结构信息. 
//                   该指针由该函数分配, 需要使用 HI_SYS_FREE(HI_MOD_ID_APP_PROTO, ppstFrame) 释放. 
//   
// 返 回 值: 
//    HI_ERR_SUCCESS : 成功
//    其它为错误码 
//
// 调用要求: 接收到 ID_MRS_MSG_MR_DATA 消息后，调用该接口获取 376.2 帧结构 
// 调用举例: TODO: ...
// 作    者: shenhankun/00130424 [2011-11-21]
//*****************************************************************************
HI_EXTERN HI_U32 MRS_Proto3762Dec(HI_IN HI_PBYTE pucDatagram, HI_U16 usDatagramSize, HI_OUT MRS_3762_FRAME_STRU** ppstFrame);
HI_EXTERN HI_U32 MRS_ProtoCsgDec(HI_IN HI_PBYTE pucDatagram, HI_U16 usDatagramSize, HI_OUT MRS_CSG_FRAME_STRU** ppstFrame);

//*****************************************************************************
// 函数名称: MRS_Proto3762Enc
// 功能描述: 376.2协议报文组装
//           
// 参数说明:
//   pucDatagram [out] 376.2报文，需要使用 HI_SYS_FREE(HI_MOD_ID_APP_PROTO, pucDatagram) 释放
//   pusDatagramSize [out] 376.2报文大小
//   pstFrame [in] 376.2帧结构
//
// 返 回 值: 
//    HI_ERR_SUCCESS : 成功
//    其它为错误码 
//
// 调用要求: 由CCO调用 MRS_SendMrData 发送报文给集中器 
// 调用举例: TODO: ...
// 作    者: shenhankun/00130424 [2011-11-21]
//*****************************************************************************
HI_EXTERN HI_U32 MRS_Proto3762Enc(HI_OUT HI_PBYTE* pucDatagram, HI_OUT HI_U16* pusDatagramSize, HI_IN MRS_3762_FRAME_STRU* pstFrame);


// 645控制域结构体
typedef struct 
{
    HI_U8 ucFn        :5; // 请求及应答功能码    
    HI_U8 ucFrameFlag :1; // 后续帧标志 0-无后续数据帧 1-有后续数据帧
    HI_U8 ucSlaveFlag :1; // 从站异常标志 0-从站正确应答 1-从站对异常信息的应答
    HI_U8 ucDir       :1; // 传送方向 0-由主站发出的命令帧 1-由从站发出的应答帧
}MRS_645_CTRL_STRU;


#define MRS_PROTO645_VER_UNKNOWN 0
#define MRS_PROTO645_VER_1997    1
#define MRS_PROTO645_VER_2007    2 

#define mrsCheck645Proto(x) ((x == MRS_PROTO645_VER_2007) || (x == MRS_PROTO645_VER_1997))
// 645 帧头信息结构体
typedef struct 
{
    HI_U8 ucVer; // 协议版本,取值为 MRS_PROTO645_VER_XXX 
    HI_U8 ucAddr[6];           // 表地址，固定6字节 
    MRS_645_CTRL_STRU stCtrl; // 控制码
    HI_U8 ucDataRealmLen; // 数据域大小必须小于200字节, 0表示无数据域
    HI_U8 ucDataRealm[MRS_PROTO645_DATAGRAM_DATA_LEN_MAX]; // 数据域 
}MRS_645_FRAME_STRU;


//*****************************************************************************
// 函数名称: MRS_Proto645Dec
// 功能描述: 解析645协议报文头，数据单元标识 和 数据单元 部分内容不解析
//           
// 参数说明:
//   pucDatagram [in] 报文
//   usDatagramSize [in] 报文大小，单位字节
//   pstFrame [out] 指向MRS_645_FRAME_STRU结构, 用于保存解析后的帧结构信息.
//   
// 返 回 值: 
//    HI_ERR_SUCCESS : 成功
//    其它为错误码 
//
// 调用要求: 接收到 ID_MRS_MSG_MR_DATA 消息后，调用该接口获取645帧结构 
// 调用举例: TODO: ...
// 作    者: shenhankun/00130424 [2011-11-21]
//*****************************************************************************
HI_EXTERN HI_U32 MRS_Proto645Dec(HI_IN HI_PBYTE pucDatagram, HI_U16 usDatagramSize, HI_OUT MRS_645_FRAME_STRU* pstFrame);


//*****************************************************************************
// 函数名称: MRS_Proto645Enc
// 功能描述: 645协议报文组装
//           
// 参数说明:
//   ucDatagram [out] 645报文最大的缓存
//   pusDatagramSize [out] 645报文实际大小
//   pstFrame [in] 645帧结构 MRS_645_FRAME_STRU 
//
// 返 回 值: 
//    HI_ERR_SUCCESS : 成功
//    其它为错误码 
//
// 调用要求: 由STA调用 MRS_SendMrData 发送报文给电表，发送后需要使用HI_SYS_FREE(HI_MOD_ID_APP_PROTO, pucDatagram)释放帧数据
// 调用举例: TODO: ...
// 作    者: shenhankun/00130424 [2011-11-21]
//*****************************************************************************
HI_EXTERN HI_U32 MRS_Proto645Enc(HI_OUT HI_U8 ucDatagram[MRS_PROTO645_DATAGRAM_LEN_MAX], HI_OUT HI_U16* pusDatagramSize, HI_IN MRS_645_FRAME_STRU* pstFrame);





HI_END_HEADER
#endif // __MRS_FW_H__


