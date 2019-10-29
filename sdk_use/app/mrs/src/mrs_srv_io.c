//*****************************************************************************
//
//                  版权所有 (C), 2001-2012, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : sal_dev_io.c
//  版 本 号   : V2.0
//  作    者   : fengxiaomin/00209182
//  生成日期   : 2012年07月06日
//  功能描述   : IO控制实现.
//  函数列表   :
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

#include "mrs_common.h"
#include "mrs_cmd_msg.h"
#include "mrs_fw_n.h"
#include "mrs_fw_proto376_2.h"
#include "mrs_fw_proto645.h"
#include "mrs_fw_tools.h"
#include "mrs_fw_proto376_2_echo.h"
#include "mrs_fw_log.h"
#include "mrs_srv_cco_macmap.h"
/* BEGIN: Added by fengxiaomin/00209182, 2014/1/10   问题单号号:DTS2014012607684 */
#include "mrs_srv_cco_remote_upg.h"
/* END:   Added by fengxiaomin/00209182, 2014/1/10 */
#include "mrs_srv_res.h"
#include "mrs_srv_io.h"
#include "hi_mdm.h"
#include "mrs_dfx.h"
#include "mrs_srv_cco.h"
#include "mrs_srv_sta_queue.h"
#include "mrs_srv_parallel_sta.h"
#include "mrs_srv_sta.h"
#include "mrs_time.h"
#include "mrs_srv_capture.h"

#if (defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) || defined(PRODUCT_CFG_PRODUCT_TYPE_STA))

#define TX_LED_DURATION     100     // 发送指示灯亮的时间，单位:ms
#define ERR_LED_DURATION    30      // 故障指示灯亮的时间，单位:ms

#define MRS_IO_PIN_SET_RESULT_HIGH  (0)
#define MRS_IO_PIN_SET_RESULT_LOW   (1)
#define MRS_IO_PIN_SET_RESULT_FAIL  (2)

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
/* CCO端管脚初始化 */
HI_PRV HI_U32 mrsCcoIOParamInit(HI_VOID);
#endif

//***********************************
//***********************************

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
/* STA端管脚初始化 */
HI_PRV HI_U32 mrsStaIOParamInit(HI_VOID);
#endif

//***********************************
//***********************************

/* 获取IO上下文 */
HI_PRV MRS_IO_CTX_STRU *GetIOCtx(HI_VOID);

/* 不同产品形态下IO参数初始化 */
HI_PRV HI_U32 mrsIOParamInit(HI_VOID);

/* 初始化GPIO */
HI_PRV HI_U32 mrsGpioInit(HI_VOID);

HI_PRV HI_U32 mrsIORstPro(HI_U32 ulParam);

/* 复位中断 */
HI_U32 mrsResetInt(MRS_IO_PARAM_STRU *pstParam);

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
HI_PRV HI_U32 mrsRunEventOutCallBack(HI_BOOL bEvtOut);
HI_PRV HI_S32 mrsIOEventOutCallback(HI_U32 param);
HI_PRV HI_BOOL mrsIoIsValidLevel(HI_U8 ucExpectVal, HI_GPIO_VALUE_E enCurrVal);
HI_PRV MRS_IO_PARAM_STRU *mrsGetEvtIoParam(HI_VOID);
#endif

HI_PRV HI_U32 mrsRunResetCallback(HI_VOID);
HI_PRV HI_S32 mrsIOResetCallback(HI_U32 param);

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
HI_PRV HI_U8 mrsReadPinSet(HI_VOID);
#endif

#define EVENT_OUT_CALLBACK_MAX      (1)
#define RESET_CALLBACK_MAX          (1)

typedef struct
{
    HI_VOID  *fct;
    HI_U32   ulParam;
} CALLBACK_PARAM;

MRS_IO_CTX_STRU g_stIOCtx;

CALLBACK_PARAM  g_eventOutCallback[EVENT_OUT_CALLBACK_MAX] = {{HI_NULL, 0}};
CALLBACK_PARAM  g_resetCallback[RESET_CALLBACK_MAX] = {{HI_NULL, 0}};

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)

typedef struct
{
    HI_U8 ucCheckNum;
    HI_U8 ucExpectLevel;    // 期望电平
    HI_U8 padding[2];
}MRS_EVTOUT_CTX;

#define MRS_EVT_MAX_CHECK_NUM  (3)

#define MRS_EVT_LEVEL_LOW      (0)
#define MRS_EVT_LEVEL_HIGH     (1)


HI_PRV MRS_EVTOUT_CTX g_EventOutCtx = {0, MRS_EVT_LEVEL_HIGH, {0, 0}};

HI_PRV MRS_EVTOUT_CTX *mrsGetEvtOutCtx(HI_VOID)
{
    return &g_EventOutCtx;
}

MRS_IO_PARAM_STRU *mrsGetEvtIoParam(HI_VOID)
{
    return &g_stIOCtx.eventOutIOInfo;
}

HI_U32 mrsEvtOutResetInt(HI_VOID)
{
    HI_U32 ret = HI_ERR_SUCCESS;

    if ((mrsToolsIsTChip()) && (mrsToolsIsSTA() || mrsToolsIsTSTA()))
    {
        MRS_IO_PARAM_STRU *pstEvt = mrsGetEvtIoParam();

        ret = mrsResetInt(pstEvt);
    }

    return ret;
}


//注册回调函数
HI_U32 mrsRegEventOutCallBack(mrsCallBack f, HI_U32 ulParam)
{
    HI_U32 i;

    for (i = 0; i < EVENT_OUT_CALLBACK_MAX; i++)
    {
        if (HI_NULL == g_eventOutCallback[i].fct)
        {
            g_eventOutCallback[i].fct = (HI_VOID *)f;
            g_eventOutCallback[i].ulParam = ulParam;
            return HI_ERR_SUCCESS;
        }
    }

    return HI_ERR_FAILURE;
}


//执行回调函数
HI_U32 mrsRunEventOutCallBack(HI_BOOL bEvtOut)
{
    HI_U32 i;

    for (i = 0; i < EVENT_OUT_CALLBACK_MAX; i++)
    {
        if (HI_NULL != g_eventOutCallback[i].fct)
        {
            ((mrsCallBack)g_eventOutCallback[i].fct)(g_eventOutCallback[i].ulParam, bEvtOut);
        }
    }

    return HI_ERR_SUCCESS;
}

#endif


//注册RESET pin回调函数
HI_U32 mrsRegResetCallBack(mrsResetCallBack f, HI_U32 ulParam)
{
    HI_U32 i;

    for (i = 0; i < RESET_CALLBACK_MAX; i++)
    {
        if (HI_NULL == g_resetCallback[i].fct)
        {
            g_resetCallback[i].fct = (HI_VOID *)f;
            g_resetCallback[i].ulParam = ulParam;
            return HI_ERR_SUCCESS;
        }
    }

    return HI_ERR_FAILURE;
}

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)

HI_BOOL mrsIoIsValidLevel(HI_U8 ucExpectVal, HI_GPIO_VALUE_E enCurrVal)
{
    if(ucExpectVal == MRS_EVT_LEVEL_HIGH)
    {
        if(enCurrVal != HI_GPIO_VALUE1)
        {
            return HI_FALSE;
        }
    }
    else
    {
        if(enCurrVal != HI_GPIO_VALUE0)
        {
            return HI_FALSE;
        }
    }

    return HI_TRUE;
}

HI_S32 mrsIOEventOutCallback(HI_U32 param)
{
    HI_S32 ret = (HI_S32)HI_ERR_SUCCESS;
    MRS_EVTOUT_CTX *pstEvtOutCtx = mrsGetEvtOutCtx();
    MRS_IO_PARAM_STRU *pstParam = mrsGetEvtIoParam();

    MRS_NOT_USED(param);

    (HI_VOID)HI_MDM_IO_GetInputVal((HI_GPIO_IDX_E)(pstParam->stData.enGpioIndex * MRS_IO_INDEX_OFFSET_SIZE + pstParam->stData.enGpioBit),
                      (HI_GPIO_VALUE_E *) (&pstParam->stData.stGpioValueDirec.enGpioValue));

    if(HI_FALSE == mrsIoIsValidLevel(pstEvtOutCtx->ucExpectLevel,
                                      pstParam->stData.stGpioValueDirec.enGpioValue))
    {
        mrsEvtOutResetInt();
        return HI_ERR_SUCCESS;
    }

    pstEvtOutCtx->ucCheckNum = 0;

    MRS_StartTimer(MRS_STA_TIMER_IO_EVENT_OUT, MRS_STA_TIME_IO_EVENTOUT_VALUE, HI_SYS_TIMER_ONESHOT);

    return ret;
}

HI_VOID mrsEventTimeOutProc(HI_VOID)
{
    MRS_EVTOUT_CTX *pstEvtOutCtx = mrsGetEvtOutCtx();
    MRS_IO_PARAM_STRU *pstParam = mrsGetEvtIoParam();

    (HI_VOID)HI_MDM_IO_GetInputVal((HI_GPIO_IDX_E)(pstParam->stData.enGpioIndex * MRS_IO_INDEX_OFFSET_SIZE + pstParam->stData.enGpioBit),
                      (HI_GPIO_VALUE_E *) (&pstParam->stData.stGpioValueDirec.enGpioValue));

    if(HI_FALSE == mrsIoIsValidLevel(pstEvtOutCtx->ucExpectLevel,
                                      pstParam->stData.stGpioValueDirec.enGpioValue))
    {
        mrsEvtOutResetInt();
        return;
    }

    pstEvtOutCtx->ucCheckNum++;

    if(pstEvtOutCtx->ucCheckNum >= MRS_EVT_MAX_CHECK_NUM)
    {
        mrsRunEventOutCallBack(pstEvtOutCtx->ucExpectLevel);
        if(pstEvtOutCtx->ucExpectLevel == MRS_EVT_LEVEL_HIGH)
        {
            pstEvtOutCtx->ucExpectLevel = MRS_EVT_LEVEL_LOW;
            pstParam->stInterrData.enRisingnFalling = HI_EDGE_FALL_LEVEL_LOW;
        }
        else
        {
            pstEvtOutCtx->ucExpectLevel = MRS_EVT_LEVEL_HIGH;
            pstParam->stInterrData.enRisingnFalling = HI_EDGE_RISE_LEVEL_HIGH;
        }

        pstEvtOutCtx->ucCheckNum = 0;

        mrsEvtOutResetInt();
    }
    else
    {
        MRS_StartTimer(MRS_STA_TIMER_IO_EVENT_OUT, MRS_STA_TIME_IO_EVENTOUT_VALUE, HI_SYS_TIMER_ONESHOT);
    }

    return;
}

#endif


HI_S32 mrsIOResetCallback(HI_U32 param)
{
    HI_S32 ret = (HI_S32)HI_ERR_SUCCESS;
    MRS_IO_CTX_STRU *pstCtx = GetIOCtx();
    MRS_IO_PARAM_STRU *pstParam = HI_NULL;
    HI_U8 timer = 0;
    HI_U32 period = 0;

    MRS_NOT_USED(param);

    pstParam = &(pstCtx->resetIOInfo);
    (HI_VOID)HI_MDM_IO_GetInputVal((HI_GPIO_IDX_E)(pstParam->stData.enGpioIndex * MRS_IO_INDEX_OFFSET_SIZE + pstParam->stData.enGpioBit),
                      (HI_GPIO_VALUE_E *)(&pstParam->stData.stGpioValueDirec.enGpioValue));

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    timer = MRS_CCO_TIMER_RESET_PIN;
    period = MRS_CCO_TIME_RESET_PIN;
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
    timer = MRS_STA_TIMER_RESET_PIN;
    period = MRS_STA_TIME_RESET_PIN;
#endif

    MRS_StopTimer(timer);

    if ((HI_GPIO_VALUE_E)HI_EDGE_FALL_LEVEL_LOW == pstParam->stData.stGpioValueDirec.enGpioValue)
    {
        pstParam->stInterrData.enRisingnFalling = HI_EDGE_RISE_LEVEL_HIGH;

        MRS_StartTimer(timer, period, HI_SYS_TIMER_ONESHOT);
    }
    else
    {
        pstParam->stInterrData.enRisingnFalling = HI_EDGE_FALL_LEVEL_LOW;
    }

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
    if ((mrsToolsIsTChip()) && (mrsToolsIsSTA() || mrsToolsIsTSTA()))
#endif
    {
        //HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_001, HI_DIAG_MT("reset gpio enable."));lisr中不允许调用阻塞接口
        mrsResetInt(pstParam);
    }

    return ret;
}


HI_VOID mrsResetTimeOutProc(HI_VOID)
{
    // TODO:  这里是否需要考虑其它模块的处理
    mrsRunResetCallback();
}

HI_U32 mrsRunResetCallback(HI_VOID)
{
    HI_U32 i;

    for (i = 0; i < RESET_CALLBACK_MAX; i++)
    {
        if (HI_NULL != g_resetCallback[i].fct)
        {
            ((mrsResetCallBack)g_resetCallback[i].fct)(g_resetCallback[i].ulParam);
        }
    }

    return HI_ERR_SUCCESS;
}


MRS_IO_CTX_STRU *GetIOCtx(HI_VOID)
{
    return &g_stIOCtx;
}


HI_U32 mrsInitIOCtx(HI_VOID)
{
    HI_U32 ret = HI_ERR_SUCCESS;

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
    if (mrsToolsIsIICollector())
    {
        return HI_ERR_NOT_SUPPORT;
    }
#endif

    (hi_void)memset_s(&g_stIOCtx, sizeof(g_stIOCtx), 0, sizeof(g_stIOCtx));

    mrsRegResetCallBack(mrsIORstPro, 0);

    ret = mrsIOParamInit();
    if (HI_ERR_SUCCESS != ret)
    {
        return HI_ERR_FAILURE;
    }

    ret = mrsGpioInit();
    if (HI_ERR_SUCCESS != ret)
    {
        return HI_ERR_FAILURE;
    }

    return ret;
}


/* 不同产品形态下IO参数初始化 */
HI_U32 mrsIOParamInit(HI_VOID)
{
    HI_U32 ret = HI_ERR_SUCCESS;

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    ret = mrsCcoIOParamInit();
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
    ret = mrsStaIOParamInit();
#endif


    return ret;
}


#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
/* CCO端管脚初始化 */
HI_U32 mrsCcoIOParamInit(HI_VOID)
{
    MRS_IO_CTX_STRU *pstIOCtx = GetIOCtx();
    MRS_IO_PARAM_STRU *pstIOParamInfo = HI_NULL;

    /* RESET */
    pstIOParamInfo = &pstIOCtx->resetIOInfo;
    pstIOParamInfo->enIOType = EN_MRS_IO_RESET;
    pstIOParamInfo->stData.stGpioValueDirec.enGpioDirec = HI_GPIO_DIRECTION_IN;
    pstIOParamInfo->stData.enGpioIndex = MRS_CCO_IO_RESET_INDEX;
    pstIOParamInfo->stData.enGpioBit = MRS_CCO_IO_RESET_BIT;
    pstIOParamInfo->stInterrData.enGpioIndexInt = MRS_CCO_IO_RESET_INDEX;
    pstIOParamInfo->stInterrData.enGpioBitInt = MRS_CCO_IO_RESET_BIT;
    pstIOParamInfo->stInterrData.gpio_callback = mrsIOResetCallback;
    pstIOParamInfo->stInterrData.ulCallbackArgv = 0;
    pstIOParamInfo->stInterrData.enLevelnEdge = HI_INT_EDGE;
    pstIOParamInfo->stInterrData.enRisingnFalling = HI_EDGE_FALL_LEVEL_LOW;
    HI_MDM_IO_SetDir((HI_GPIO_IDX_E)(MRS_CCO_IO_RESET_INDEX * MRS_IO_INDEX_OFFSET_SIZE + MRS_CCO_IO_RESET_BIT),
                      (HI_GPIO_DIRECTION_E)pstIOParamInfo->stData.stGpioValueDirec.enGpioDirec);

    /* SET */
    pstIOParamInfo = &pstIOCtx->setIOInfo;
    pstIOParamInfo->enIOType = EN_MRS_IO_SET;
    pstIOParamInfo->stData.stGpioValueDirec.enGpioDirec = HI_GPIO_DIRECTION_IN;
    pstIOParamInfo->stData.enGpioIndex = MRS_CCO_IO_SET_INDEX;
    pstIOParamInfo->stData.enGpioBit = MRS_CCO_IO_SET_BIT;
    HI_MDM_IO_SetDir((HI_GPIO_IDX_E)(MRS_CCO_IO_SET_INDEX * MRS_IO_INDEX_OFFSET_SIZE + MRS_CCO_IO_SET_BIT),
                      (HI_GPIO_DIRECTION_E)pstIOParamInfo->stData.stGpioValueDirec.enGpioDirec);

    return HI_ERR_SUCCESS;
}
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
/* STA端管脚初始化 */
HI_U32 mrsStaIOParamInit(HI_VOID)
{
    MRS_IO_CTX_STRU *pstIOCtx = GetIOCtx();
    MRS_IO_PARAM_STRU *pstIOParamInfo = HI_NULL;

    /* EVENT OUT */
    pstIOParamInfo = &pstIOCtx->eventOutIOInfo;
    pstIOParamInfo->enIOType = EN_MRS_IO_EVENT_OUT;
    pstIOParamInfo->stData.stGpioValueDirec.enGpioDirec = HI_GPIO_DIRECTION_IN;
    pstIOParamInfo->stData.enGpioIndex = MRS_STA_IO_EVENT_OUT_INDEX;
    pstIOParamInfo->stData.enGpioBit = MRS_STA_IO_EVENT_OUT_BIT;
    pstIOParamInfo->stInterrData.enGpioIndexInt = MRS_STA_IO_EVENT_OUT_INDEX;
    pstIOParamInfo->stInterrData.enGpioBitInt = MRS_STA_IO_EVENT_OUT_BIT;
    pstIOParamInfo->stInterrData.gpio_callback = mrsIOEventOutCallback;
    pstIOParamInfo->stInterrData.ulCallbackArgv = 0;
    pstIOParamInfo->stInterrData.enLevelnEdge = HI_INT_LEVEL;
    pstIOParamInfo->stInterrData.enRisingnFalling = HI_EDGE_RISE_LEVEL_HIGH;
    HI_MDM_IO_SetDir((HI_GPIO_IDX_E)(MRS_STA_IO_EVENT_OUT_INDEX * MRS_IO_INDEX_OFFSET_SIZE + MRS_STA_IO_EVENT_OUT_BIT),
                      (HI_GPIO_DIRECTION_E)pstIOParamInfo->stData.stGpioValueDirec.enGpioDirec);

    /* RESET */
    pstIOParamInfo = &pstIOCtx->resetIOInfo;
    pstIOParamInfo->enIOType = EN_MRS_IO_RESET;
    pstIOParamInfo->stData.stGpioValueDirec.enGpioDirec = HI_GPIO_DIRECTION_IN;
    pstIOParamInfo->stData.enGpioIndex = MRS_STA_IO_RESET_INDEX;
    pstIOParamInfo->stData.enGpioBit = MRS_STA_IO_RESET_BIT;
    pstIOParamInfo->stInterrData.enGpioIndexInt = MRS_STA_IO_RESET_INDEX;
    pstIOParamInfo->stInterrData.enGpioBitInt = MRS_STA_IO_RESET_BIT;
    pstIOParamInfo->stInterrData.gpio_callback = mrsIOResetCallback;
    pstIOParamInfo->stInterrData.ulCallbackArgv = 0;
    pstIOParamInfo->stInterrData.enLevelnEdge = HI_INT_EDGE;
    pstIOParamInfo->stInterrData.enRisingnFalling = HI_EDGE_FALL_LEVEL_LOW;
    HI_MDM_IO_SetDir((HI_GPIO_IDX_E)(MRS_STA_IO_RESET_INDEX * MRS_IO_INDEX_OFFSET_SIZE + MRS_STA_IO_RESET_BIT),
                      (HI_GPIO_DIRECTION_E)pstIOParamInfo->stData.stGpioValueDirec.enGpioDirec);

    /* SET */
    pstIOParamInfo = &pstIOCtx->setIOInfo;
    pstIOParamInfo->enIOType = EN_MRS_IO_SET;

    pstIOParamInfo->stData.stGpioValueDirec.enGpioDirec = HI_GPIO_DIRECTION_IN;
    pstIOParamInfo->stData.enGpioIndex = MRS_STA_IO_SET_INDEX;
    pstIOParamInfo->stData.enGpioBit = MRS_STA_IO_SET_BIT;
    HI_MDM_IO_SetDir((HI_GPIO_IDX_E)(MRS_STA_IO_SET_INDEX * MRS_IO_INDEX_OFFSET_SIZE + MRS_STA_IO_SET_BIT),
                      (HI_GPIO_DIRECTION_E)pstIOParamInfo->stData.stGpioValueDirec.enGpioDirec);

    /* ON_SITE */
    pstIOParamInfo = &pstIOCtx->setIOInfo;
    pstIOParamInfo->enIOType = EN_MRS_IO_ON_SITE;

    pstIOParamInfo->stData.stGpioValueDirec.enGpioDirec = HI_GPIO_DIRECTION_IN;
    pstIOParamInfo->stData.enGpioIndex = MRS_STA_IO_ON_SITE_INDEX;
    pstIOParamInfo->stData.enGpioBit = MRS_STA_IO_ON_SITE_BIT;
    HI_MDM_IO_SetDir((HI_GPIO_IDX_E)(MRS_STA_IO_ON_SITE_INDEX * MRS_IO_INDEX_OFFSET_SIZE + MRS_STA_IO_ON_SITE_BIT),
                      (HI_GPIO_DIRECTION_E)pstIOParamInfo->stData.stGpioValueDirec.enGpioDirec);

    return HI_ERR_SUCCESS;
}
#endif


/* 初始化GPIO */
HI_U32 mrsGpioInit(HI_VOID)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    MRS_IO_CTX_STRU *pstIOCtx = GetIOCtx();

    /* RESET */
#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
    if ((mrsToolsIsTChip()) && (mrsToolsIsSTA() || mrsToolsIsTSTA()))
#endif
    {
        ret = mrsResetInt(&(pstIOCtx->resetIOInfo));
    }

    if (HI_ERR_SUCCESS != ret)
    {
        return HI_ERR_FAILURE;
    }

    return ret;
}



/* 复位中断 */
HI_U32 mrsResetInt(MRS_IO_PARAM_STRU *pstParam)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U8 ucIoNum = 0;

    if (pstParam == HI_NULL)
    {
        return HI_ERR_FAILURE;
    }

    ucIoNum = (HI_U8)(pstParam->stInterrData.enGpioIndexInt* MRS_IO_INDEX_OFFSET_SIZE
              + pstParam->stInterrData.enGpioBitInt);

    ret = HI_MDM_IO_IntDisconnect((HI_GPIO_IDX_E)ucIoNum);
    if (ret != HI_ERR_SUCCESS)
    {
        return HI_ERR_FAILURE;
    }

    ret = HI_MDM_IO_IntConnect((HI_GPIO_IDX_E)ucIoNum,
                                       (HI_GPIO_INT_E)pstParam->stInterrData.enLevelnEdge,
                                       (HI_GPIO_INT_POLARITY_E)pstParam->stInterrData.enRisingnFalling,
                                       pstParam->stInterrData.gpio_callback,
                                       pstParam->stInterrData.ulCallbackArgv,
                                       HI_FALSE);
    if (ret != HI_ERR_SUCCESS)
    {
    }

    return ret;
}

HI_BOOL mrsIOIsRstPinEnable(HI_VOID)
{
    if (mrs_get_test_mode_enable() == HI_TRUE) {
        return HI_TRUE;
    }

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    MRS_CCO_SRV_CTX_STRU *pstCcoCtx = mrsCcoGetContext();

    if (pstCcoCtx && HI_FALSE == pstCcoCtx->ucCcoRstPinEnable)
    {
        return HI_FALSE;
    }
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
    MRS_STA_SRV_CTX_STRU *pstStaCtx = mrsStaGetContext();

    if (pstStaCtx && (HI_FALSE == pstStaCtx->ucStaRstPinEnable || HI_MDM_GetSeconds() < pstStaCtx->ulRstShielTime))
    {
        return HI_FALSE;
    }
#endif

    return HI_TRUE;
}

/* 当复位管脚有中断到来时，执行该处理函数 */
HI_U32 mrsIORstPro(HI_U32 ulParam)
{
    MRS_NOT_USED(ulParam);

	#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
	hi_u32 cur_sys_time = HI_MDM_GetSeconds();
	hi_u32 power_on_time = mrs_power_failure_get_power_on_time();	
	hi_bool device_not_pull_reset = HI_MDM_GetDevicePullFlag();
	#endif
    do
    {
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
        if (!mrsIsAllowCcoReset())
        {
            break;
        }
#endif

		/*
        if (HI_FALSE == mrsIOIsRstPinEnable())
        {
            break;
        }
		*/
		
		#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
		// 正常停电上电后禁止拉动rst管脚
        if(HI_MDM_IsPowerCut() == HI_TRUE && cur_sys_time <= MRS_IO_PULL_RTS_MIN_TIME){
			return HI_ERR_FAILURE;
		}

		// 模块上电时间记录5s内禁止拉动rst管脚
        if(mrsTimeSubDuration(cur_sys_time, power_on_time) <= MRS_IO_PULL_RTS_MIN_TIME){
			return HI_ERR_FAILURE;
		}
		
		//此处添加停电事件处理,如果处于停电状态，启动5s定时器，到期后判断是否可以拉动管脚
        if(device_not_pull_reset == HI_TRUE)
		{	
			MRS_StartTimer(MRS_STA_PULL_RST_DELAY_TIMER, MRS_IO_POWER_FAILURE_DELAY_PULL_RTS, HI_SYS_TIMER_ONESHOT);
			return HI_ERR_FAILURE;
		}
		#endif

        HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_061, HI_DIAG_MT("MRS_IORstPro"), ulParam);
			
		HI_SYS_RstDelayDisable(HI_SYS_RST_DELAY_ENTER_PHY_TEST_MODE);

        //直接调用复位函数复位
        HI_MDM_Reboot(HI_SYS_REBOOT_CAUSE_MRS10);

    } while (0);

    return HI_ERR_SUCCESS;
}


#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
HI_U8 mrsReadPinSet(HI_VOID)
{
    HI_U32 ret;
    HI_GPIO_DATA_S *data = HI_NULL;
    MRS_IO_CTX_STRU *pstIOCtx = GetIOCtx();

    data = &pstIOCtx->setIOInfo.stData;

    //读SET管脚
    ret = HI_MDM_IO_GetInputVal((HI_GPIO_IDX_E)(data->enGpioIndex * MRS_IO_INDEX_OFFSET_SIZE + data->enGpioBit),
                            (HI_GPIO_VALUE_E *)&data->stGpioValueDirec.enGpioValue);
    if (ret != HI_ERR_SUCCESS)
    {
        return 2;
    }

    if (data->stGpioValueDirec.enGpioValue == (HI_GPIO_VALUE_E)HI_EDGE_FALL_LEVEL_LOW) //如果SET管脚是低电平
    {
        return MRS_IO_PIN_SET_RESULT_LOW;//SET管脚是低电平
    }

    return MRS_IO_PIN_SET_RESULT_HIGH;//SET管脚是高电平
}


//SET管脚处理函数（集中器：此函数在05-F1函数中被调用）（电表端的应该被哪个函数调用,暂时忽略）
HI_U8 mrsReadFootPinSet(HI_VOID)
{
    return mrsReadPinSet();
}
#endif

#endif

