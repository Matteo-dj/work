//*****************************************************************************
//
//                  版权所有 (C), 1998-2011, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_srv_common.h
//  版 本 号   : V1.0 
//  作    者   : shenhankun/00130424
//  生成日期   : 2011-11-08
//  功能描述   : TODO: ...
//               
//  函数列表   : TODO: ...
//  修改历史   : 
//  1.日    期 : 2011-11-08
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


#ifndef __MRS_SRV_COMMON_H__
#define __MRS_SRV_COMMON_H__

#include "mrs_srv_res.h"

HI_START_HEADER


//*****************************************************************************
// 功能模块上下文结构定义
//*****************************************************************************

// 规约类型定义
#define MRS_STA_PROTOCOL_UNKNOWN        0   // 未知的
#define MRS_STA_PROTOCOL_1997           1   // 645-1997
#define MRS_STA_PROTOCOL_2007           2   // 645-2007

// PLC通道接收/发送模式定义
#define MRS_PLC_RX_TX_MODE_NORMAL       0   // 普通模式(须等待组网路由优化完成后才进行PLC收发)
#define MRS_PLC_RX_TX_MODE_FORCE        1   // 强制模式(不等待Mac层的组网及优化过程强制进行收发)

// STA模块类型定义
#define MRS_STA_TYPE_METER              0   // 电表形态
#define MRS_STA_TYPE_COLLECTOR          1   // 采集器形态

// STA-METER之间超时时间默认值
#define MRS_RM_METER_OVERTIME_MAX       (1000 * 10)  // 电表应答超时(最大)
#define MRS_RM_METER_OVERTIME_DEFAULT   (1800)       // 电表应答超时(默认)
#define MRS_RM_METER_OVERTIME_MIN       (100)        // 电表应答超时(最小)
#define MRS_CHECK_METER_FAULT_FRAME_NUM (2)          // STA端检测电表故障所须的最小帧个数

#define MRS_100MS_TO_MS(x)              ((x) * 100)
#define MRS_10MS_TO_MS(x)               ((x) * 10)
#define MRS_MS_TO_100MS(x)              ((x) / 100)
#define MRS_MS_TO_10MS(x)               ((x) / 10)

// 搜表状态
#define MRS_SEARCH_STATUS_IDLE          (0)
#define MRS_SEARCH_STATUS_IN_PROGRESS   (1)
#define MRS_SEARCH_STATUS_UNKNOWN       (3)

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)

// 通道改变通知处理函数类型
typedef HI_U32 (* MRS_CHL_NOTIFY_PROC_F)(HI_BOOL, HI_U32, HI_BOOL *);


// 通道改变通知处理初始化
HI_VOID mrsSrvChlNotifyProcInit(HI_VOID);

// 通道改变通知处理函数注册
HI_U32 mrsSrvChlNotifyProcReg(MRS_CHL_NOTIFY_PROC_F, HI_U32);

// 通道改变通知处理函数清理
HI_U32 mrsSrvChlNotifyProcClear(MRS_CHL_NOTIFY_PROC_F);

// 通道改变通知处理函数调用
HI_U32 mrsSrvChlNotifyProc(HI_BOOL);

#endif // defined(PRODUCT_CFG_PRODUCT_TYPE_STA)


HI_U32 mrsSrvGetVerInfo(HI_U16 *pusVer, HI_U8 *pucBootVer);

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA) || defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
HI_VOID mrsPhyTestTimeoutProc(HI_VOID);
HI_VOID mrsEnterTestModeProc(HI_VOID);
#endif

HI_END_HEADER
#endif //__MRS_SRV_COMMON_H__

