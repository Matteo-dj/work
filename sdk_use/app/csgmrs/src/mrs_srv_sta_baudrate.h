//*****************************************************************************
//
//                  版权所有 (C), 2001-2011, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_srv_sta_baudrate.h
//  版 本 号   : V1.0 
//  作    者   : 聂松松/KF62735
//  生成日期   : 2012-07-11
//  功能描述   : 测试波特率
//  函数列表   : NA
//  修改历史   : 
//  1.日    期 : 2012-07-11
//    作    者 : 聂松松/KF62735
//    修改内容 : 创建文件 
//*****************************************************************************
#ifndef __MRS_SRV_STA_BAUDRATE_H__
#define __MRS_SRV_STA_BAUDRATE_H__
HI_START_HEADER

typedef struct
{
    HI_U16  usBaudRate;
    HI_U8   ucRetry;
    HI_U8   ucPad;
}MRS_STA_DETECT_STRU;

MRS_STA_DETECT_STRU* mrsStaDetectGetContext(HI_VOID);

HI_VOID mrsStaDetectBaudRate(HI_VOID);

HI_VOID mrsCltIDetectBaudRate(HI_VOID);

// 搜表
HI_PUBLIC HI_VOID mrsStaSearchMeter(HI_VOID);

HI_END_HEADER
#endif