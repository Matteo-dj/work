//*****************************************************************************
// 
//                  版权所有 (C), 1998-2011, 华为技术有限公司
// 
//*****************************************************************************
//  文 件 名   : mrs_srv_meter.c
//  版 本 号   : V1.0 
//  作    者   : niesongsong
//  生成日期   : 2012-08-16
//  功能描述   : 电表模块
//               
//  函数列表   : 
//  修改历史   : 
//  1.日    期 : 2012-08-16
//    作    者 : niesongsong/kf62735
//    修改内容 : 创建文件 
// 
//*****************************************************************************
#ifndef __MRS_SIMU_METER_H__
#define __MRS_SIMU_METER_H__

//依赖链表
#include "mrs_srv_list.h"

HI_START_HEADER

#define METER_NUM_MAX   PRODUCT_CFG_MRS_MAX_METER_IN_MAC_NUM

#define MRS_STA_SIM_F1_RATE_MODULUS  (124000 / 62)
#define MRS_STA_SIM_F10_RATE_MODULUS  (124000 / 12)
typedef HI_VOID * (*simu_alloc_fun)(HI_U32 sz);
typedef HI_VOID (*simu_free_fun)(HI_VOID *p);

typedef struct _MRS_SIMU_METER 
{
    HI_U8 ucVer;            /*版本,1997*/
    HI_U8 ucMeterAddr[6];   /*地址*/
    HI_U8 padding[1];

    HI_U32 uiRate[4];       /*4个费率*/
    HI_U32 uiKWH;           /*正向有功总电能*/     
    HI_U32 uiTime;          /*冻结时间格式*/
   
    HI_U16 usWord1;         /*状态字1*/
    HI_U16 usWord3;         /*状态字3*/
    HI_U32 ulOpenTimes : 24;     /*开盖次数*/
    HI_U32 ulPowerOffTimes : 24; /*掉电次数*/
    HI_U32 ulNeutralWire : 24;  /*零线电流*/
    HI_U32 ulRunTimes : 8;
    HI_U32 ulLiveWire : 24;   /*火线电流*/
}MRS_SIMU_METER,*PMRS_SIMU_METER;

//模拟电表模块
typedef struct 
{
    HI_U16 usNum;    /*电表个数*/
    HI_U8  padding[2];
    MRS_SIMU_METER *pstMList[METER_NUM_MAX]; /*电表列表，最多64个电表*/
    simu_alloc_fun pfnMalloc;
    simu_free_fun  pfnFree;
}MRS_MET_MODULE;

//输入输出的缓存
typedef struct 
{
    HI_U8 * ucBuf;        
    HI_U32  uiLen;
}MRS_MEM_BUF;

typedef struct 
{
    MRS_SRV_LINK link;
    MRS_MEM_BUF buf;
}MRS_MEM_LINK;

//初始始化电表模块
HI_PUBLIC HI_U32 mrsSrvMeterInit(MRS_MET_MODULE*,HI_U16,simu_alloc_fun,simu_free_fun);

//添加电表
HI_PUBLIC HI_U32 mrsSrvMeterAdd(MRS_MET_MODULE*,HI_U8 * maddr);

//电表回复
HI_PUBLIC HI_U32 mrsSrvMetersReply(MRS_MET_MODULE*,MRS_MEM_BUF *,MRS_SRV_LIST *);

// 同步虚拟表地址到Mac层
HI_PUBLIC HI_VOID mrsSyncVMAddrToMac(HI_VOID);

HI_U32 mrsStaSend2Meter(MRS_MET_MODULE *,HI_PBYTE, HI_U16);
HI_U32 mrsStaGetSupportVM(HI_VOID);
HI_U32 mrsStaSetSupportVM(HI_U32 flag);
HI_VOID mrsStaSetVMActiveDelay(HI_U32 ulTime);
HI_U32 mrsStaGetVMActiveDelay(HI_VOID);

HI_END_HEADER

#endif
