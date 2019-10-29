//*****************************************************************************
//
//                  版权所有 (C), 1998-2013, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_srv_cltii_searchmeter.h
//  版 本 号   : V1.0 
//  作    者   : cuiate/00233580
//  生成日期   : 2014-08-13
//  功能描述   : II采搜表函数及接口声明
//               
//  函数列表   : TODO: ...
//  修改历史   : 
//  1.日    期 : 2014-08-13
//    作    者 : cuiate/00233580
//    修改内容 : 创建文件 
//
//*****************************************************************************

#ifndef _MRS_SRV_CLTII_SEARCHMETER_H_
#define _MRS_SRV_CLTII_SEARCHMETER_H_
HI_START_HEADER
#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)

#define MRS_SM_POS_H                (4)
#define MRS_SM_POS_A                (1)
#define MRS_SM_POS_DI               (10)

#define MRS_SM_MASK_BYTE_GDW             (0xAA)             // 国网07、97规约搜表掩码
#define MRS_SM_ZERO_BYTE                 (0x00)             // 搜表缩位从0x00开始进位去问
#define MRS_BCD_BYTE_DEC_MAX_VALUE       (99)               // 搜表地址单字节十六进制BCD码看做十进制的最大值

#define MRS_CLTII_SEARCH_STATE_IDLE                 (0)     // 非搜表状态
#define MRS_CLTII_SEARCH_STATE_TRYFISRT             (1)     // 上电尝试上次入网表地址
#define MRS_CLTII_SEARCH_STATE_DETECT               (2)     // 探测上次搜表结果
#define MRS_CLTII_SEARCH_STATE_LOOP                 (3)     // 通配符循环搜表
#define MRS_CLTII_SEARCH_STATE_VERIFY               (4)     // 搜到表后抄读验证
#define MRS_CLTII_SEARCH_STATE_EXPLORE              (5)     // 多搜N字节
#define MRS_CLTII_SEARCH_STATE_EXPLORE_VERIFY       (6)     // 多搜N字节过程中搜到表抄读验证
#define MRS_CLTII_SEARCH_STATE_DETECT_LIST          (7)     // 搜表列表探测


//*****************************************************************************
// 接口定义
//*****************************************************************************

HI_U32 mrsCltiiSearchStart(MRS_STA_SEARCH_CTX *pstCtx, HI_BOOL bTryFirstFlag);

HI_U32 mrsCltiiSearchStop(MRS_STA_SEARCH_CTX *pstCtx);

HI_VOID mrsCltiiSearchSetState(HI_U8 ucState);

HI_U8 mrsCltiiSearchGetState(HI_VOID);

HI_U32 mrsCltiiSend2Meter(HI_PBYTE pstBuf, HI_U16 usLen); 

HI_U32 mrsCltiiSearchRx(HI_DMS_CHL_RX_S* pstRx);

HI_U32 mrsCltiiSearchByteTimeoutProc(MRS_STA_SEARCH_CTX *pstCtx);

HI_U32 mrsCltiiSearchFrameTimeoutProc(MRS_STA_SEARCH_CTX *pstCtx);

HI_U32 mrsCltiiSearchVMTimeoutProc(HI_VOID);

HI_BOOL mrsCltiiSearchIsNvNodeDI97Valid(HI_U8 ucOption);

HI_U8* mrsCltiiSearchTxAddr(MRS_STA_SEARCH_CTX* pstCtx);

HI_VOID mrsCltiiSearchGetTxAddr(MRS_STA_SEARCH_CTX* pstCtx, HI_OUT HI_U8* pucAddr);

HI_U8 mrsCltiiSearchTxProtocol(MRS_STA_SEARCH_CTX *pstCtx);

HI_U32 mrsCltiiSearchTxDI(MRS_STA_SEARCH_CTX *pstCtx);

HI_U8 mrsCltiiSearchGetAddrDepth(HI_U8* pucAddr, HI_U8 ucProtocol);

HI_U32 mrsCltiiSearchDetectRxProc(HI_U8 *pData, HI_U16 usLen);

HI_BOOL mrsCltiiSearchIsEnable(HI_VOID);

HI_U32 mrsCltiiInitSearchFrame(MRS_STA_SEARCH_CTX * pstCtx, HI_U32 ulDI, HI_U8 ucProtocol);

HI_U32 mrsCltiiCreateSearchFrame(MRS_STA_SEARCH_CTX * pstCtx, HI_U8 *pucMeter, HI_U8 ucProtocol);

HI_VOID  mrsCltiiSearchResetRunning(MRS_STA_SEARCH_CTX* pstCtx);

HI_U8 mrsCltiiSearchGetNodeBaudRateIndex(HI_U8 ucParam);

HI_U32 mrsCltiiSearchGetNodeDI(MRS_STA_SEARCH_CTX* pstCtx, NV_APP_MRS_SM_NODE* pstNode);
//*****************************************************************************
//*****************************************************************************


#endif //defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
HI_END_HEADER
#endif //_MRS_SRV_CLTII_SEARCHMETER_H_

