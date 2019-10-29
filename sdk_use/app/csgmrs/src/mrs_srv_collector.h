//*****************************************************************************
//
//                  版权所有 (C), 1998-2011, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_srv_collector.h
//  版 本 号   : V2.0
//  作    者   : fengxiaomin/f00209182
//  生成日期   : 2012-08-28
//  功能描述   : TODO: ...
//
//  函数列表   : TODO: ...
//  修改历史   :
//  1.日    期 : 2012-08-28
//    作    者 : fengxiaomin/f00209182
//    修改内容 : 创建文件
//
//*****************************************************************************
#ifndef __MRS_SRV_COLLECTOR_H__
#define __MRS_SRV_COLLECTOR_H__
HI_START_HEADER


#define MRS_COLLECTOR_SEND_BOARD_INTERVAL_TIME    200  //广播校时以2400波特率转发后再次以1200波特率转发的超时时间
#define MRS_COLLECTOR_SEND_BOARD_END_TIME         1000  //广播校时以1200波特率转发结束超时时间

#define LED_RUN_STATUS_TURN_ON_OFF_TIME    1000      //运行灯亮/灭的时间
#define LED_RTX_TURN_ON_OFF_TIME    100      //运行灯亮/灭的时间
//#define LED_UART_RTX            HI_LED_NUM4   //485数据正在通信
#define LED_RUN_STATUS          HI_LED_IDX_2   //运行灯,表示采集器正在运行
//#define LED_PLC_TX      HI_LED0
//#define LED_PLC_RX      HI_LED1
#define MRS_COLLECTOR_97OR07_CONTROL_FLAG    0x10  //在区分97规约还是07规约时控制字判断参数

#define MRS_CLTII_TIMING_645_CTRL              (0x33)
#define MRS_CLTII_TIMING_645_UP_CTRL_INCREAZE  (0x80)
#define MRS_CLTII_TIMING_645_CTRL_MASK         (0x07)
#define MRS_CLTII_TIMING_645_CTRL_LEN          (2)
#define MRS_CLTII_TIMING_645_TRANSFER_OFFSET   (12)


typedef enum
{
    MRS_COLLECTOR_RUNLED_0_25HZ,
    MRS_COLLECTOR_RUNLED_0_50HZ,
    MRS_COLLECTOR_RUNLED_5_00HZ
} MRS_COLLECTOR_RUNLED_FREQ_E;

#define MRS_COLLECTOR_RUN_LED_NORMAL        (MRS_COLLECTOR_RUNLED_0_50HZ)
#define MRS_COLLECTOR_RUN_LED_SEARCHMETER   (MRS_COLLECTOR_RUNLED_5_00HZ)
#define MRS_COLLECTOR_RUN_LED_NO_METER      (MRS_COLLECTOR_RUNLED_0_25HZ)


typedef struct
{
    HI_U8 ucProtoType;
    HI_U8 ucTimingCnt;    //广播校时不同波特率发送标识 0-应发送以2400波特率 1-应发送以1200波特率
    HI_U8 ucRunLedStatus; //运行灯状态 0-目前为灭，应点亮 1-目前为亮，应熄灭
    HI_U8 ucUartLedStatus; //串口收发灯状态 0-目前为灭，应点亮 1-目前为亮，应熄灭
    HI_U32 ulTimingSendInterval;    // 广播校时帧发送间隔 单位: 毫秒
    HI_U8 ucBaudRateIndex;
/* BEGIN: PN:  MODIFY\ADD\DEL by cuichunyan/00276962 at 2015/10/21 */
    HI_U8 ucIrTrasmit645;//645透抄开关
/* END: PN:  MODIFY\ADD\DEL by cuichunyan/00276962 at 2015/10/21 */
    HI_U8 aucMeter[HI_METER_ADDR_LEN];
    HI_U8 aucDevice[HI_METER_ADDR_LEN];
    HI_U8 ucPowerSearchEnable;
    HI_U8 reserved;
} MRS_COLLECTOR_CTX_STRU;


// 上一次广播读表电表信息（只记录读表失败时信息）
typedef struct
{
    HI_U8 bValid : 1;
    HI_U8 ucIndex : 7;
    HI_U8 ucProtocol : 4;
    HI_U8 ucCount : 4;
    HI_U8 aucMeter[HI_METER_ADDR_LEN];
} MRS_CLTII_LAST_BC_METER_INF;


MRS_COLLECTOR_CTX_STRU *mrsGetCollectorCtx(HI_VOID);

//*****************************************************************************
// 函数名称: mrsCollectorInit
// 功能描述: II型采集器模块初始化
//
// 参数说明:
//
// 返 回 值:
//
// 调用要求:
// 调用举例:
// 作    者: fengxiaomin/f00209182 [2012-08-28]
//*****************************************************************************
HI_VOID mrsCollectorInit(HI_VOID);

//*****************************************************************************
// 函数名称: mrsSendMrData
// 功能描述: 发送读表数据给电表
//
// 参数说明:
//   HI_U8 type    载波模块类型-STA/II型采集器
//   HI_U8 *data   645数据帧
//   HI_U16 len    645数据帧长度
//
// 返 回 值:
//    成功返回  HI_ERR_SUCCESS
//
// 调用要求:
// 调用举例:
// 作    者: fengxiaomin/f00209182 [2012-08-28]
//*****************************************************************************
HI_U32 mrsSendData2Meter(HI_U8 *pData, HI_U16 usLen, HI_U8 *ucRetry, HI_U32 over_time);


//*****************************************************************************
// 函数名称: mrsCollectorSendMrPro
// 功能描述: II型采集器发送读表数据前的处理函数
//
// 参数说明:
//   HI_U8 *data   645数据帧
//
// 返 回 值:
//    成功返回  HI_ERR_SUCCESS
//
// 调用要求:
// 调用举例:
// 作    者: fengxiaomin/f00209182 [2012-08-28]
//*****************************************************************************
HI_VOID mrsCollectorSendMrPro(HI_U8 *data, HI_U16 usDataLen);


//*****************************************************************************
// 函数名称: mrsBoardTimingSend
// 功能描述: 转发II型采集器接收的广播校时帧
//
// 参数说明:
//   HI_U8 *pTimingData    II型采集器接收的广播校时帧
//   HI_U16 usDataLen     II型采集器接收的广播校时帧长度
//   HI_U8 *ucRetry       转发重试次数
//
// 返 回 值:
//    成功返回  HI_ERR_SUCCESS
//
// 调用要求:
// 调用举例:
// 作    者: fengxiaomin/f00209182 [2012-08-28]
//*****************************************************************************
HI_U32 mrsBoardTimingSendPro(HI_U8 *pTimingData, HI_U16 usDataLen, HI_U8 *pucRetry);

//*****************************************************************************
// 函数名称: mrsRunLedPro
// 功能描述: 使运行灯LED保持闪烁
//
// 参数说明:
//    ucFirstFlag   区分首次上电后执行还是运行起来之后执行
// 返 回 值:
//    成功返回  HI_ERR_SUCCESS
//
// 调用要求:
// 调用举例:
// 作    者: fengxiaomin/f00209182 [2012-08-28]
//*****************************************************************************
HI_VOID mrsRunLedPro(HI_U8 ucFirstFlag);

//*****************************************************************************
// 函数名称: mrsUARTRTxLedPro
// 功能描述: 485数据正在通信时使得LED灯闪烁
//
// 参数说明:
//
// 返 回 值:
//    成功返回  HI_ERR_SUCCESS
//
// 调用要求:
// 调用举例:
// 作    者: fengxiaomin/f00209182 [2012-08-28]
//*****************************************************************************
HI_VOID mrsUARTRTxLedPro(HI_VOID);

//*****************************************************************************
// 函数名称: mrsCollectorSendMrData
// 功能描述: II采发送抄表数据给电表
//
// 参数说明:
//
// 返 回 值:
//    成功返回  HI_ERR_SUCCESS
//
// 调用要求:
// 调用举例:
// 作    者: fengxiaomin/f00209182 [2013-07-18]
//*****************************************************************************
HI_U32 mrsCollectorSendMrData(HI_U8 *pCollectorData, HI_U16 usMrDataLen, HI_U32 over_time);


HI_VOID mrsCollectorSetRunLedTime(MRS_COLLECTOR_RUNLED_FREQ_E led_freq);

HI_U32 mrsCltiiTimingPreProc(MRS_PLC_FRAME_DATA_STRU *pstFrame, HI_PVOID *pData, HI_U16 *pusLen, HI_BOOL *bFlag);

HI_VOID mrsCltiiSetBaudRateIndex(HI_U8 ucIndex);

HI_U8 mrsCltiiGetBaudRateIndex(HI_VOID);

HI_BOOL mrsIsCltiiTiming(HI_VOID);

HI_U32 mrsCreateTimingUpFrame(HI_U8 **ppData, HI_U16 *pusDataLen, MRS_MSG_MR_645_DATA_STRU* pstData);

HI_U32 mrsCltiiTimingTimeOutProc(MRS_STA_ITEM *pstItem);

HI_U32 mrsCltiiGetMeterProtocol(HI_U8 pucMeter[], HI_U8 *pucProtocol);

HI_U32 mrsCltiiTimingProc(HI_U8 *pData, HI_U8 *pFrame, HI_U8 *pucRetry, HI_U32 ulTimeout);

HI_VOID mrsSetCollectorAddr(HI_U8 *pucDeviceAddr);

HI_U8 *mrsGetCollectorAddr(HI_VOID);

HI_U32 mrsCltiiProtoProc(HI_U8 *pData, HI_U16 usDataLen, HI_U16 usId, HI_U16 usSeq);

HI_U32 mrsCltiiCheckAddr(HI_U8 *pAddr);

HI_U32 mrsCltiiCheckMeterAddr(HI_U8 *pAddr);


MRS_CLTII_LAST_BC_METER_INF *mrsCltiiGetLastBcMeterInf(HI_VOID);

HI_VOID mrsCltiiClearLastBcMeterInf(HI_VOID);


HI_END_HEADER
#endif //__MRS_SRV_COLLECTOR_H__

