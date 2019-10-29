//*****************************************************************************
//
//                  版权所有 (C), 1998-2012, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_srv_io.h
//  版 本 号   : V2.0
//  作    者   : fengxiaomin/00209182
//  生成日期   : 2012-07-17
//  功能描述   : TODO: ...
//
//  函数列表   : TODO: ...
//  修改历史   :
//  1.日    期 :
//    作    者 :
//    修改内容 : 创建文件
//
//  2.日    期 : 2013年04月12日
//    作    者 : cuiate/00233580
//    修改内容 : 重构
//
//*****************************************************************************

//*****************************************************************************
// PROJECT   :
// SUBSYSTEM :
// MODULE    :
// OWNER     :
//*****************************************************************************

#ifndef __MRS_SRV_IO_H__
#define __MRS_SRV_IO_H__

HI_START_HEADER


#include <hi_driver.h>
#include <mrs_power_failure.h>


/* CCO RESET PIN : GPIO16 */
#define MRS_CCO_IO_RESET_INDEX      (HI_GPIO_2)
#define MRS_CCO_IO_RESET_BIT        (HI_GPIO_BIT0)

/* CCO SET PIN : GPIO8 */
#define MRS_CCO_IO_SET_INDEX        (HI_GPIO_1)
#define MRS_CCO_IO_SET_BIT          (HI_GPIO_BIT0)

/* STA EVENT OUT PIN : GPIO15 */ //GPIO10
#define MRS_STA_IO_EVENT_OUT_INDEX  (HI_GPIO_1)
#define MRS_STA_IO_EVENT_OUT_BIT    (HI_GPIO_BIT2)

/* STA SET PIN : GPI15 */
#define MRS_STA_IO_SET_INDEX        (HI_GPIO_1)
#define MRS_STA_IO_SET_BIT          (HI_GPIO_BIT7)

/* STA RESET PIN : GPIO9 */
#define MRS_STA_IO_RESET_INDEX      (HI_GPIO_1)
#define MRS_STA_IO_RESET_BIT        (HI_GPIO_BIT1)

/* STA ON SITE PIN : GPIO26 */
#define MRS_STA_IO_ON_SITE_INDEX      (HI_GPIO_3)
#define MRS_STA_IO_ON_SITE_BIT        (HI_GPIO_BIT2)

#define MRS_IO_PULL_RTS_MIN_TIME    (5)				// 最小拉管脚时间
#define MRS_IO_POWER_FAILURE_DELAY_PULL_RTS    (5000)				// 停电拉动rst管脚延迟时间5s
#define MRS_IO_INDEX_OFFSET_SIZE    (8)

typedef HI_U32 (*mrsCallBack)(HI_U32 ulParam, HI_BOOL bEvtOut);
typedef HI_U32 (*mrsResetCallBack)(HI_U32 ulParam);


// LED指示灯定义
typedef enum
{
    EN_MRS_LED_PLCRX = 0,   // 接收灯
    EN_MRS_LED_PLCTX        // 发送灯
}MRS_LED_TYPE_ENUM;


typedef enum
{
    EN_MRS_IO_SET = 0,
    EN_MRS_IO_EVENT_OUT,
    EN_MRS_IO_LED,  //工装LED
    EN_MRS_IO_STA,
    EN_MRS_IO_RESET,
    EN_MRS_IO_ON_SITE,
} MRS_IO_TYPE_ENUM;


typedef struct
{
    MRS_IO_TYPE_ENUM            enIOType;       /* IO管脚类型 */
    HI_GPIO_DATA_S              stData;         /* gpio数据 */
    HI_GPIO_INT_CTRL_S          stInterrData;   /* gpio中断数据 */
} MRS_IO_PARAM_STRU;

typedef struct
{
    MRS_IO_PARAM_STRU       setIOInfo;
    MRS_IO_PARAM_STRU       resetIOInfo;
#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
    MRS_IO_PARAM_STRU       eventOutIOInfo;
#endif
} MRS_IO_CTX_STRU;


/* IO初始化 */
HI_U32 mrsInitIOCtx(HI_VOID);

//注册回调函数
HI_U32 mrsRegEventOutCallBack(mrsCallBack f, HI_U32 ulParam);

//注册RESET pin回调函数
HI_U32 mrsRegResetCallBack(mrsResetCallBack f, HI_U32 ulParam);

// RESET定时器超时处理
HI_VOID mrsResetTimeOutProc(HI_VOID);

// EVENT_OUT定时器超时处理
HI_VOID mrsEventTimeOutProc(HI_VOID);

//SET管脚处理函数（集中器：此函数在05-F1函数中被调用）（电表端的应该被哪个函数调用,暂时忽略）
HI_U8 mrsReadFootPinSet(HI_VOID);

HI_U32 mrsEvtOutResetInt(HI_VOID);

HI_END_HEADER

#endif //__SAL_RES_H__


