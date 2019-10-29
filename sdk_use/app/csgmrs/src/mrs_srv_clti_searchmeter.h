//*****************************************************************************
//
//                  版权所有 (C), 1998-2014, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_srv_clti_searchmeter.h
//  版 本 号   : V1.0 
//  作    者   : liupeidong/00238134
//  生成日期   : 2014-03-18
//  功能描述   : I型采集器搜表相关函数及接口定义
//               
//  函数列表   : TODO: ...
//  修改历史   : 
//  1.日    期 : 2014-03-18
//    作    者 : liupeidong/00238134
//    修改内容 : 创建文件 
//
//*****************************************************************************
#ifndef __MRS_SRV_CLTI_SEARCHMETER_H__
#define __MRS_SRV_CLTI_SEARCHMETER_H__
HI_START_HEADER
#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)

#define MRS_SEC_TO_MSEC                       (1000)         // 秒->毫秒

#define MRS_645_FRAME_DI_LEN_V97              (2)
#define MRS_645_FRAME_DI_LEN_V07              (4)
#define MRS_CLTI_QUERY_RX_DATA_INFO_LEN       (7)            // I采搜表结果应答帧数据域中(DI+搜表状态+表总数+本帧表数)的长度
#define MRS_CLTI_QUERY_RX_ADDR_PROTO_LEN      (7)            // I采搜表结果应答帧数据域中每个表及其规约的长度

#define MRS_CLTI_REPORT_POS_STATUS            (4)            // 搜表结果上报帧数据域下标: 搜表状态
#define MRS_CLTI_REPORT_POS_TOTAL             (5)            // 搜表结果上报帧数据域下标: 搜表总数
#define MRS_CLTI_REPORT_POS_THIS              (6)            // 搜表结果上报帧数据域下标: 本帧上报数量
#define MRS_CLTI_REPORT_POS_LIST              (7)            // 搜表结果上报帧数据域下标: 上报电表和规约列表

#define MRS_DI_POSITIVE_A_V97                 (0x9010)       // VER97 读正向有功电能(+A)
#define MRS_DI_POSITIVE_A_V07                 (0x00010000)   // VER07 读正向有功电能(+A)
#define MRS_DI_CLTI_SM_START                  (0x0B0A0B0A)   // 开始搜表
#define MRS_DI_CLTI_SM_STOP                   (0x0B0C0B0C)   // 停止搜表
#define MRS_DI_CLTI_SM_QUERY                  (0x0B0D0B0D)   // 查询搜表结果

#define MRS_CLTI_SM_FINISH                    (0x00)         // I采搜表结束
#define MRS_CLTI_SM_IN_PROGRESS               (0x01)         // I采搜表进行中

#define MRS_CLTI_SM_REPORT_MAX                (16)           // I采搜表结果每帧上报的最大电表数量

#define MRS_CLTI_FRAME_TYPE_SM_START          (1)            // I采搜表帧类型: 开始搜表
#define MRS_CLTI_FRAME_TYPE_SM_STOP           (2)            // I采搜表帧类型: 停止搜表
#define MRS_CLTI_FRAME_TYPE_SM_QUERY_FIRST    (3)            // I采搜表帧类型: 查询搜表结果 第1帧
#define MRS_CLTI_FRAME_TYPE_SM_QUERY_FOLLOW   (4)            // I采搜表帧类型: 查询搜表结果 后续帧
#define MRS_CLTI_FRAME_TYPE_POSITIVE_A        (5)            // I采上电尝试上次入网地址: 读正向有功电能(+A)

#define MRS_CLTI_SEND_BYTE_TIME_2400          (5)            // 发送每个字节的延时
 
#define MRS_CLTI_SM_START_TRY_MAX             (2)            // 开始查询帧重试次数
#define MRS_CLTI_SM_STOP_TRY_MAX              (2)            // 停止查询帧重试次数
#define MRS_CLTI_SM_QUERY_FOLLOW_TRY_MAX      (3)            // 查询后续帧的重试次数，此时搜表已停止     

#define MRS_CLTI_SET_VER_ADDR_OFFSET          (MRS_PREFIX_FE_SIZE + MRS_645_FRAME_1ST_HEAD_LEN)
#define MRS_CLTI_SET_SOFTWARE_VER_OFFSET      (18)
#define MRS_CLTI_SET_BOOT_VER_OFFSET          (20)
#define MRS_CLTI_SET_VER_DI                   (0x0B0A0C00)
#define MRS_CLTI_VER_INFO_LEN                 (3)

MRS_CLTI_SM_QUERY_CTX* mrsCltiGetQueryCtx(HI_VOID);
HI_VOID mrsCltiSetQueryCtx(HI_U8, HI_U8, HI_U8);
HI_VOID mrsCltiResetSmRunning(HI_VOID);
HI_U32 mrsCltiRx1300Proc(HI_U8* pucAddr, MRS_645_CTRL_STRU *pstCtrl);
HI_VOID mrsStaSetCltiType(HI_BOOL);
HI_BOOL mrsIsRealClti(HI_VOID);
HI_VOID mrsCltiWriteCLTAddress2Nv(HI_U8*);
HI_BOOL mrsCltiGetValidMeterInfo(MRS_STA_SEARCH_CTX *pstCtx);
HI_U32 mrsCltiTryGetAddrForMAC(HI_VOID);
HI_U32 mrsCltiTryReadPositiveATx_V07(HI_U8*);
HI_U32 mrsCltiTryReadPositiveATx_V97(HI_U8*, HI_U16);
HI_VOID mrsCltiTryReadPositiveATx(HI_VOID);

HI_U32 mrsCltiFrameEnQueue(HI_U8*, HI_U16, HI_U8);
HI_VOID mrsCltiFrameSendPrepare(MRS_STA_ITEM * pstItem);
HI_U32 mrsCltiFrameTimeoutProc(HI_VOID);

HI_U32 mrsCltiSmStartTx(HI_VOID);
HI_U32 mrsCltiSmStopTx(HI_VOID);
HI_U32 mrsCltiSmQueryTx(HI_U8, HI_U8, HI_U8);
HI_U32 mrsCltiSmQueryTxRetry(HI_VOID);

HI_VOID mrsCltiTryReadPositiveARx(MRS_645_FRAME_STRU*);
HI_VOID mrsCltiSmFrameRx(MRS_645_FRAME_STRU*);
HI_U32 mrsCltiSmStartRx(MRS_645_FRAME_STRU*);
HI_U32 mrsCltiSmStopRx(MRS_645_FRAME_STRU*);
HI_U32 mrsCltiSmQueryRx(MRS_645_FRAME_STRU *pst645Hrd);
HI_BOOL mrsCheckCltiSmQueryRxData(HI_U8*, HI_U8);
HI_U32 mrsCltiSaveSmResult(HI_U8*);
HI_U32 mrsCltiSmStopCmd(MRS_STA_SEARCH_CTX *pstCtx);
HI_U32 mrsCltiSearchFinish(HI_U8* pucData);

HI_U32 mrsCltiSearchStart(MRS_STA_SEARCH_CTX *pstCtx, HI_BOOL bTryFirstFlag);

HI_U32 mrsCltiSearchStop(MRS_STA_SEARCH_CTX *pstCtx);

HI_U32 mrsCltiSearchMeterInsert(MRS_STA_SEARCH_CTX *pstCtx, HI_U8 *pucMeter, HI_U8 ucProtocol);


#endif // defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
HI_END_HEADER
#endif // __MRS_SRV_CLTI_SEARCHMETER_H__

