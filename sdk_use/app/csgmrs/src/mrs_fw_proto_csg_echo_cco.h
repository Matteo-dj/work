//*****************************************************************************
//
//                  版权所有 (C), 2001-2014, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_fw_proto_csg_echo_cco.h
//  版 本 号   : V1.0
//  作    者   : cuiate/c00233580
//  生成日期   : 2015-12-25
//  功能描述   : TODO: ...
//
//  函数列表   : TODO: ...
//  修改历史   :
//  1.日    期 : 2015-12-25
//    作    者 : cuiate/c00233580
//    修改内容 : 创建文件
//
//*****************************************************************************

#ifndef _MRS_FW_PROTO_CSG_ECHO_CCO_H_
#define _MRS_FW_PROTO_CSG_ECHO_CCO_H_
HI_START_HEADER
#include "hi_equip.h"
#include "equip_dut_proc.h"
#include "mrs_fw_proto_csg_echo.h"

#if (defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) && defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD))

#define MRS_BC_DELAY_TIME                   (1)     // 1s
#define MRS_UPG_WAIT_TIME                   (60)    // 10s
#define MRS_MAX_FRAME_LEN                   (1024)  // 最大支持的报文长度
#define MRS_CSG_PROTO_RELEASE_DATE          "\x01\x02\x16"

#define MRS_CSG_FRAME_MAX_LENGTH_OFFSET     (1)     // 最大支持协议报文长度
#define MRS_CSG_MAX_PACKET_LENGTH_OFFSET    (3)     // 文件传输最大单包长度
#define MRS_CSG_UPG_WAIT_TIME_OFFSET        (5)     // 升级等待时间
#define MRS_CSG_CCO_ADDR_OFFSET             (6)     // 主节点地址
#define MRS_CSG_STATION_MAX_NUM_OFFSET      (12)    // 支持的最大从节点数量
#define MRS_CSG_STATION_NUM_OFFSET          (14)    // 当前从节点数量
#define MRS_CSG_NODE_MAX_NUM_OFFSET         (16)    // 支持单次读写从节点信息的最大数量
#define MRS_CSG_PROTO_RELEASE_OFFSET        (18)    // 发布时间
#define MRS_CSG_DEVICE_INFO_OFFSET          (21)    // 厂商代码和版本信息
#define MRS_CSG_COMMUNICATION_UP_DATA_LEN   (30)

#define MRS_CSG_FILES_INFOS_FIX_LEN         (18)
#define MRS_CSG_FILES_FAILED_FIX_LEN        (3)



HI_PUBLIC HI_U32 mrsCcoProtoCsgEchoInit(HI_VOID);

HI_PUBLIC HI_U32 mrsCcoProtoCsgEchoDeinit(HI_VOID);

HI_U32 makeBuf_AFN03_E8000301(HI_OUT HI_U8 * pucBuf, HI_INOUT HI_U16 * pusBufLen);
HI_U32 makeBuf_AFN03_E8000302(HI_OUT HI_U8 ** ppaucOutBuf, HI_OUT HI_U16 * pusOutLen);

HI_VOID mrsCsgQueueFree(HI_VOID *p);
HI_VOID mrsCsgListFree(HI_VOID *p, HI_VOID *pParam);

HI_U32 mrsCsgIsMeterRepeat(HI_U8 *pucContent, HI_U8 ucMeterIndex, HI_U8 ucMeterNum);
HI_U32 mrsCsgNeedInsertNode(HI_U8 *pucContent, HI_U8 ucMeterNum, HI_U8 *ucDeny);

HI_U32 mrsCsgGetFileBuffer(HI_U8 **ppBuffer, HI_U16 *pusLength);
HI_U32 mrsCsgGetFileProcess(HI_U8 **ppucBuffer, HI_U16 *pusLength);
HI_U32 mrsCsgGetFileFailedInfo(HI_U16 usStartNo, HI_U8 ucNodesCnt, HI_U8 **ppucBuffer, HI_U16 *pusLength);
HI_U32 mrsCsgPlcTestProc(HI_PVOID pParam, HI_U16 usSize);

// AFN00 F1 E8010001: 确认
HI_U32 mrsCsg_AFN00_E8010001(HI_INOUT HI_VOID *pParam);

// AFN00 F2: 否认
HI_U32 mrsCsg_AFN00_E8010002(HI_INOUT HI_VOID *pParam);


// AFN01 F1: 硬件初始化
HI_U32 mrsCsg_AFN01_E8020101(HI_INOUT HI_VOID *pParam);

// AFN01 F2: 初始化表档案
HI_U32 mrsCsg_AFN01_E8020102(HI_INOUT HI_VOID *pParam);

// AFN01 F3: 初始化任务
HI_U32 mrsCsg_AFN01_E8020103(HI_INOUT HI_VOID *pParam);


// 添加任务
HI_U32 mrsCsg_AFN02_E8020201(HI_INOUT HI_VOID *pParam);

// 删除任务
HI_U32 mrsCsg_AFN02_E8020202(HI_INOUT HI_VOID *pParam);

// 查询未完成任务
HI_U32 mrsCsg_AFN02_E8000203(HI_INOUT HI_VOID *pParam);

// 查询未完成任务列表
HI_U32 mrsCsg_AFN02_E8030204(HI_INOUT HI_VOID *pParam);

// 查询未完成任务详细信息
HI_U32 mrsCsg_AFN02_E8030205(HI_INOUT HI_VOID *pParam);

// 查询剩余可分配任务数
HI_U32 mrsCsg_AFN02_E8000206(HI_INOUT HI_VOID *pParam);

// 添加多播任务
HI_U32 mrsCsg_AFN02_E8020207(HI_INOUT HI_VOID *pParam);

// 启动任务
HI_U32 mrsCsg_AFN02_E8020208(HI_INOUT HI_VOID *pParam);

// 暂停任务
HI_U32 mrsCsg_AFN02_E8020209(HI_INOUT HI_VOID *pParam);


// AFN03 F1:查询厂家代码和版本号
HI_U32 mrsCsg_AFN03_E8000301(HI_INOUT HI_VOID *pParam);

// AFN03 DIE8000302:查询本地通信模块运行模式信息
HI_U32 mrsCsg_AFN03_E8000302(HI_INOUT HI_VOID *pParam);

// 主动上报“本地通信模块运行模式信息”
HI_U32 mrsCsg_AFN03_E8000302_UP(HI_VOID);

// E8 00 03 03：查询主节点地址
HI_U32 mrsCsg_AFN03_E8000303(HI_INOUT HI_VOID *pParam);

// E8 03 03 04：查询通讯延迟时间
HI_U32 mrsCsg_AFN03_E8030304(HI_INOUT HI_VOID *pParam);

// E8 00 03 05：查询从节点数量
HI_U32 mrsCsg_AFN03_E8000305(HI_INOUT HI_VOID *pParam);

// AFN03 F3：从节点信息
HI_U32 mrsCsg_AFN03_E8030306(HI_INOUT HI_VOID *pParam);

// AFN03 F7：从节点主动注册进度
HI_U32 mrsCsg_AFN03_E8000307(HI_INOUT HI_VOID *pParam);

// AFN03 F8：从节点的父节点
HI_U32 mrsCsg_AFN03_E8030308(HI_INOUT HI_VOID *pParam);


// 设置主节点地址 E8020401
HI_U32 mrsCsg_AFN04_E8020401(HI_INOUT HI_VOID *pParam);

// 添加从节点地址 E8020402
HI_U32 mrsCsg_AFN04_E8020402(HI_INOUT HI_VOID *pParam);

// 删除从节点地址 E8020403
HI_U32 mrsCsg_AFN04_E8020403(HI_INOUT HI_VOID *pParam);

// 允许/禁止从节点事件 E8020404   上电后 默认为允许，
HI_U32 mrsCsg_AFN04_E8020404(HI_INOUT HI_VOID *pParam);

// 激活从节点主动注册 E8020405
HI_U32 mrsCsg_AFN04_E8020405(HI_INOUT HI_VOID *pParam);

// 终止从节点主动注册 E8020406
HI_U32 mrsCsg_AFN04_E8020406(HI_INOUT HI_VOID *pParam);


//请求集中器时间 E8060601
HI_U32 mrsCsg_AFN06_E8060601(HI_INOUT HI_VOID *pParam);


// 启动文件传输 E8020701
HI_U32 mrsCsg_AFN07_E8020701(HI_INOUT HI_VOID *pParam);

// 传输文件内容 E8020702
HI_U32 mrsCsg_AFN07_E8020702(HI_INOUT HI_VOID *pParam);

// 查询文件信息 E8030703
HI_U32 mrsCsg_AFN07_E8000703(HI_INOUT HI_VOID *pParam);

// 查询文件处理进度 E8000704
HI_U32 mrsCsg_AFN07_E8000704(HI_INOUT HI_VOID *pParam);

//查询文件传输失败节点
HI_U32 mrsCsg_AFN07_E8030705(HI_INOUT HI_VOID * pParam);

//PLC灵敏度测试
HI_U32 mrsCsg_AFNF0_E803F001(HI_INOUT HI_VOID * pParam);

//过零检测
HI_U32 mrsCsg_AFNF0_E803F002(HI_INOUT HI_VOID * pParam);


#endif

HI_END_HEADER
#endif

