/******************************************************************************

                  版权所有 (C), 2001-2011, 华为技术有限公司

 ******************************************************************************
  文 件 名   : mrs_power_failure.h
  版 本 号   : 初稿
  作    者   : jingyankai/00415756
  生成日期   : 2017年8月16日
  最近修改   :
  功能描述   : app停电检测模块
  函数列表   :
  修改历史   :
  1.日    期   : 2017年6月1日
    作    者   : likunhe/00194117
    修改内容   : 创建文件

******************************************************************************/
#ifndef MRS_POWER_FAILURE_H
#define MRS_POWER_FAILURE_H

HI_START_HEADER


/************************************************************
                     包含其它模块的头文件
************************************************************/
#include "hi_mdm.h"

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************
                               宏定义
************************************************************/
#define MAX_REPORT_LEN          255         //发送的最大数据长度
#define SAVE_POSITION                   (1) //保留位位置
#define REPORT_EVENT_TYPE               (2) //上报事件类型位置
#define DATA_START_TEI_POSITION         (1)
#define DATA_END_TEI_POSITION           (2)
#define DATA_FIX_SIZE                   (5)
#define MRS_US_TO_S_MASK                 1000
#define MRS_COMM_UNIT_TYPE_POWER_FAILURE  (1)
#define MIN_START_TEI                   (2)
#define MAX_REPORT_TEI_CNT              41
#define CCO_ERASE_BITMAP                (3)
#define BIT_WIZE                        (8)
#define SINGLE_AMMETER_POWER_FAILURE    (1)
#define CCO_REPORT_EVENT_DATA           (3)


/************************************************************
                             数据结构定义
************************************************************/
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) || defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
typedef struct
{
    HI_U8 power_failure_bitmap[POWER_FAILURE_BITMAP_LEN];           //汇集的bitmap

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    HI_U8 cco_diff_bitmap[POWER_FAILURE_BITMAP_LEN];                //需要新上报的bitmap
    HI_U8 cco_last_send_bitmap[POWER_FAILURE_BITMAP_LEN];           //cco上次上报集中器的bitmap内容

    HI_U8 cco_wait_erase;                                           //cco等待擦除数据次数
    HI_BOOL cco_erease_bitmap_flag;                                 //cco擦除bitmap标志位
    HI_U16 pad1;
#endif

    HI_BOOL isRedayToSend;                                          //进入发送状态
    HI_U8  sta_send_times;                                          //sta发送停电上报帧次数
    HI_U16 pad2;                                                     //保留位

}POWER_FAILURE_CTRL_S;

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)

typedef struct
{
    HI_U16 usMyTei;
    HI_BOOL devicePowerFailure;
    HI_BOOL devicePullReset;

}POWER_FAILURE_RCV_MAC_REPORT_S;

/************************************************************
                             接口函数声明
 ************************************************************/
HI_U32 mrsPowerFailureCallback(HI_U8 ucType, HI_MDM_POWER_FAILURE_REPORT_ST *pCmdData, HI_U32 ulDataSize);
HI_U32 mrsHandlePowerFailure(HI_IN HI_SYS_QUEUE_MSG_S* pstMsg);
HI_U32 mrsHandleLocalPowerFailure(HI_VOID);
#endif
HI_VOID mrsSetStaPowerFailureNvCtrl(NV_APP_MRS_EVT_CFG_STRU *nv);
HI_VOID mrsGetOthersPowerFailureFrameData(MRS_CSG_PLC_FRAME_HEAD * pstRcvEvtInfo);
#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
HI_U32 mrsCreatePowerFailureFrame(HI_VOID);
HI_U32 mrsHandleOthersPowerFailureFrame(HI_VOID);
HI_VOID mrsHandlePowerOn(HI_VOID);
POWER_FAILURE_RCV_MAC_REPORT_S *mrsGetPowerFailureMacInfo(HI_VOID);
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
HI_U32 mrsCcoEvtReport(HI_VOID);
HI_U32 mrsCcoCreateReportFrame(HI_PBYTE *ppBuf, HI_U16 *usBufLen, HI_U16 *startTeiPos);
HI_U32 mrsCcoRspPowerFailureEvt(HI_VOID);
HI_VOID mrsEraseBitmap(HI_VOID);
#endif
NV_APP_MRS_EVT_CFG_STRU *mrsGetPowerFailureNvCtrl(HI_VOID);
POWER_FAILURE_CTRL_S *mrsGetPowerFailureCtrlInfo(HI_VOID);

#endif

#ifdef __cplusplus
}
#endif

HI_END_HEADER

#endif


