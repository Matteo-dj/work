//*****************************************************************************
// 
//                  版权所有 (C), 1998-2011, 华为技术有限公司
// 
//*****************************************************************************
//  文 件 名   : mrs_srv_collector.c
//  版 本 号   : V2.0 
//  作    者   : fengxiaomin/f00209182
//  生成日期   : 2012-08-28
//  功能描述   : II型采集器处理函数的实现
//               
//  函数列表   : TODO: ...
//  修改历史   : 
//  1.日    期 : 2012-08-28
//    作    者 : fengxiaomin/f00209182
//    修改内容 : 创建文件 
// 
//*****************************************************************************
#include "mrs_common.h"
#include "mrs_cmd_msg.h"
#include "mrs_fw_tools.h"
#include "mrs_fw_proto645.h"
#include "mrs_fw_proto698_45.h"
#include "mrs_fw_log.h"
#include "mrs_fw_nv.h"
#include "mrs_srv_common.h"
#include "mrs_srv_sta_queue.h"
#include "mrs_srv_sta_baudrate.h"
#include "mrs_srv_collector.h"
#include "app_mrs.h"
#include "hi_driver.h"
#include "mrs_srv_parallel_sta.h"
#include "mrs_srv_sta_event.h"
#include "mrs_srv_sta.h"
#include "mrs_srv_sta_searchmeter.h"
#include "hi_mdm.h"
#include "equip_dut_proc.h"
#include "mrs_srv_baudrate_manage.h"
#include "mrs_srv_cltii_event.h"
#include "mrs_srv_collector_irda.h"
#include "mrs_dfx_clt.h"

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)

HI_PRV MRS_COLLECTOR_CTX_STRU g_stCollectorCtx = {0};

HI_U32 g_run_led_period = MRS_STA_TIME_RUN_LED_0_50HZ;

HI_U16 g_usTimingBaudRateList[] = { MRS_SRV_BAUD_RATE_300,
                                    MRS_SRV_BAUD_RATE_600,
                                    MRS_SRV_BAUD_RATE_1200,
                                    MRS_SRV_BAUD_RATE_2400,
                                    MRS_SRV_BAUD_RATE_4800,
                                    MRS_SRV_BAUD_RATE_7200,
                                    MRS_SRV_BAUD_RATE_9600,
                                    MRS_SRV_BAUD_RATE_19200
                                  };

HI_PRV HI_VOID mrsCltiiSaveTimingProtocol(HI_U8 pucMeter[]);
HI_PRV HI_U32 mrsCltiiTransTimingProc(HI_U8 *pData, HI_U16 usDataLen, HI_U8 *pucRetry, HI_U32 ulTimeOut);
HI_PRV HI_U32 mrsCltiiProtoResp(MRS_COLLECTOR_645_FRAME_STRU *pst645Frame, MRS_IR_645_DI_FUN pIrDIProc, MRS_STA_ITEM* pstItem);
HI_PRV HI_U32 mrsCltiiCheckCltAddr(HI_U8 *pAddr);
HI_PRV HI_VOID mrsCltiiSetType(HI_VOID);
HI_PRV HI_U16 mrsCltiiGetMeterBaudRate(HI_U8 *pucMeter, HI_U8 ucProtocol);


HI_VOID mrsCollectorSetRunLedTime(MRS_COLLECTOR_RUNLED_FREQ_E led_freq)
{

    switch (led_freq)
    {
        case MRS_COLLECTOR_RUNLED_0_25HZ:
            g_run_led_period = MRS_STA_TIME_RUN_LED_0_25HZ;
            break;

        case MRS_COLLECTOR_RUNLED_5_00HZ:
            g_run_led_period = MRS_STA_TIME_RUN_LED_5_00HZ;
            break;

        case MRS_COLLECTOR_RUNLED_0_50HZ:
        default:
            g_run_led_period = MRS_STA_TIME_RUN_LED_0_50HZ;
            break;
    }
}

MRS_COLLECTOR_CTX_STRU *mrsGetCollectorCtx(HI_VOID)
{
    return &g_stCollectorCtx;
}

/* BEGIN: Modified by cuichunyan/c00276962 2016/11/11  问题单号:cltii R485 MultiConnect */    
HI_VOID mrsInitMultiConnCfg(HI_VOID)
{
    MRS_COLLECTOR_CTX_STRU * pstCtx = mrsGetCollectorCtx();

    (hi_void)memset_s(&pstCtx->stMultiConn, sizeof(MRS_CLTII_R485_MULTI_CONN_STRU), 0, sizeof(MRS_CLTII_R485_MULTI_CONN_STRU));
}
/* END  : Modified by cuichunyan/c00276962 2016/11/11  问题单号:cltii R485 MultiConnect */


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
HI_VOID mrsCollectorInit(HI_VOID)
{
    (hi_void)memset_s(&g_stCollectorCtx, sizeof(MRS_COLLECTOR_CTX_STRU), 0, sizeof(MRS_COLLECTOR_CTX_STRU));
    mrsCltiiEventModuleInit();
    
/* BEGIN: Modified by cuichunyan/c00276962 2016/11/11  问题单号:cltii R485 MultiConnect */    
    mrsInitMultiConnCfg();

/* END  : Modified by cuichunyan/c00276962 2016/11/11  问题单号:cltii R485 MultiConnect */

    mrsNvCltIIRefresh(ID_NV_APP_CTLII_CFG);
    mrsCltiiEventMonitorFlagInit();
    mrsCltiiSetType();

    if (!EQUIP_DUT_IS_EQUIP_MODE())
    {
        //处理运行灯状态-保持闪烁
        mrsRunLedPro(HI_TRUE);
    }

    return;
}

HI_VOID mrsCltiiSetType(HI_VOID)
{
    mrsStaGetContext()->stMeterList.ucType = MRS_STA_TYPE_COLLECTOR;
}

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
HI_U32 mrsSendData2Meter(HI_U8 *pData, HI_U16 usLen, HI_U8 *pucRetry, HI_U32 over_time, HI_U16 usId)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U8 *pCollectorData = HI_NULL;
    HI_U16 usMrDataLen = usLen;
    HI_U8 pFrameHead[4] = {0xFE,0xFE,0xFE,0xFE};
    HI_U16 usPos = 0, usFrameLen = 0;

    if (pData == HI_NULL)
    {
        return HI_ERR_FAILURE;
    }
    
    if(usLen < 9)
    {
        return HI_ERR_BAD_DATA;
    }
    
    usMrDataLen += sizeof(pFrameHead);
    
    pCollectorData = mrsToolsMalloc(usMrDataLen);
    if (pCollectorData == HI_NULL)
    {
        return HI_ERR_FAILURE;
    }

    (hi_void)memcpy_s(pCollectorData, usMrDataLen, pFrameHead, sizeof(pFrameHead));
    (hi_void)memcpy_s(pCollectorData + sizeof(pFrameHead), usMrDataLen - sizeof(pFrameHead), pData, usLen);

    do
    {
        ret = mrsFind645Frame(pData, (HI_S16)usLen, &usPos, &usFrameLen);
        if (HI_ERR_SUCCESS == ret)
        {
            if(HI_TRUE == mrsIsCltiiTiming())
            {
                HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_050, HI_DIAG_MT("CltiiTiming"),usFrameLen,over_time);
                (hi_void)memcpy_s(pCollectorData + sizeof(pFrameHead), usMrDataLen - sizeof(pFrameHead), pData + usPos, usFrameLen);
                usMrDataLen = usFrameLen + sizeof(pFrameHead);
                ret = mrsCltiiTransTimingProc(pCollectorData, usMrDataLen, pucRetry, over_time);
                break;
            }
            else if (MRS_CLTII_TIMING_645_CTRL == *(pData + usPos + MRS_645_FRAME_CTRL_OFFSET))
            {
                ret = mrsCltiiTimingProc(pData + usPos, pCollectorData, pucRetry, over_time);
                break;
            }
            else if (MRS_COLLECTOR_BOARDCAST_TIMEING_CONTROL == *(pData + usPos + MRS_645_FRAME_CTRL_OFFSET)) // 如果是广播校时，则分别以两种波特率发送数据
            {
                ret = mrsBoardTimingSendPro(pCollectorData, usMrDataLen, pucRetry);
                break;
            }
        }
        else
        {
            // 广播对时，透传报文
            if (ID_MRS_CMD_BROADCAST_TRANSMIT_PLC == usId)
            {
                ret = mrsBoardTimingSendPro(pCollectorData, usMrDataLen, pucRetry);
                break;
            }
        }

        mrsCollectorSendMrPro(pData, usLen, usId);
        if (MRS_STA_ITEM_ID_RM_CLT_II == usId)
        {
            ret = mrsCollectorSendMrData(pCollectorData + sizeof(pFrameHead), usMrDataLen - sizeof(pFrameHead), over_time);
        }
        else
        {
            ret= mrsCollectorSendMrData(pCollectorData, usMrDataLen, over_time);
        }
    } while (0);

    mrsToolsFree(pCollectorData);
    
    return ret;
}

//*****************************************************************************
// 函数名称: mrsCltiiTimingProc
// 功能描述: II型采集器收到05-F3后转发给电表的处理
//           
// 参数说明:
//   HI_U8 *data   645数据帧
//                            
// 返 回 值: 
//    
//
// 调用要求: 
// 调用举例: 
// 作    者: fengxiaomin/f00209182 [2015-07-29]
//*****************************************************************************
HI_U32 mrsCltiiTimingProc(HI_U8 *pData, HI_U8 *pFrame, HI_U8 *pucRetry, HI_U32 ulTimeout)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U16 usCtrl = 0;
    HI_U16 usLen = 0;
    HI_U16 usPos = 0, usFrameLen = 0;
    HI_U8 ucAffixLen = 4; //4个FE的长度
    
    (hi_void)memcpy_s(&usCtrl, sizeof(usCtrl), pData + MRS_645_FRAME_DATA_OFFSET, sizeof(usCtrl));
    mrs645DataDecode((HI_U8*)&usCtrl, sizeof(usCtrl));
    usLen = *(pData + MRS_645_FRAME_LENGTH_OFFSET) - MRS_CLTII_TIMING_645_CTRL_LEN;
    mrsCltiiSetBaudRateIndex(usCtrl & MRS_CLTII_TIMING_645_CTRL_MASK);
    (hi_void)memcpy_s(pFrame + ucAffixLen, usLen, pData + MRS_CLTII_TIMING_645_TRANSFER_OFFSET, usLen);
    mrs645DataDecode(pFrame + ucAffixLen, usLen);

    ulRet = mrsFind645Frame(pFrame + ucAffixLen, (HI_S16)usLen, &usPos, &usFrameLen);
    if (HI_ERR_SUCCESS != ulRet)
    {
        return ulRet;
    }
    
    usLen = usFrameLen + ucAffixLen;

    (hi_void)memmove_s(pFrame + ucAffixLen, usFrameLen, pFrame + ucAffixLen + usPos, usFrameLen);

    return mrsCltiiTransTimingProc(pFrame, usLen, pucRetry, ulTimeout);
}
    
//*****************************************************************************
// 函数名称: mrsCollectorSendMrPro
// 功能描述: II型采集器发送读表数据前的处理函数
//           
// 参数说明:
//   HI_U8 *data   645数据帧
//                            
// 返 回 值: 
//    
//
// 调用要求: 
// 调用举例: 
// 作    者: fengxiaomin/f00209182 [2012-08-28]
//*****************************************************************************
HI_VOID mrsCollectorSendMrPro(HI_U8 *data, HI_U16 usDataLen, HI_U16 usId)
{
    HI_U32 ret = 0;
    HI_U16 usFrameLen = 0;
    HI_U16 pos = 0;
    HI_U16 usBaudrate = MRS_SRV_BAUD_RATE_DEFAULT;
    HI_U8 ucProtocol = METER_PROTO_TRANSPARENT;
    HI_U8 *pucMeter = HI_NULL;
    HI_U8 aucMeter[HI_METER_ADDR_LEN] = {0};

    if (MRS_STA_ITEM_ID_RM_CLT_II == usId)
    {
        MRS_STA_SEARCH_CTX *pstCtx = mrsStaSearchCtx();

        mrsSrvIndex2BaudRate(pstCtx->stNvMeterList.ucBaudrateIndex, &usBaudrate);
        MRS_ChangeBaudRate(usBaudrate, MRS_SRV_UART_PARITY_EVEN);        
        return;
    }

    ret = mrsFind645Frame(data, (HI_S16)usDataLen, &pos, &usFrameLen);
    if (ret != HI_ERR_SUCCESS) 
    {   
        ret = mrsFind698Frame(data, usDataLen, &pos, &usFrameLen);
        if (ret != HI_ERR_SUCCESS)
        {
            HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_002, HI_DIAG_MT("frame 645 err,still trans"));
            // 设置默认波特率
            MRS_ChangeBaudRate(usBaudrate, MRS_SRV_UART_PARITY_EVEN);        
            return;
        }
    }
    
    do
    {
        MRS_STA_ITEM  *item = HI_NULL;
        MRS_SRV_QUEUE *queue = HI_NULL;
        MRS_STA_SRV_CTX_STRU *sta = mrsStaGetContext();
        MRS_QUE_MODULE *module = HI_NULL;

        module = &sta->stQueModule;
        queue = &module->stMrQueue;
        // 获取首结点
        item = mrsSrvQueueTop(queue);
        if (item == HI_NULL)
        {
            break;
        }

        if ((item->id == ID_MRS_CMD_IR_CMD) &&(item->ucbaudrate_index != 0))
        {
            if ((HI_U8)(item->ucbaudrate_index - 1) < MRS_TOOLS_ARRAY_COUNT(g_usTimingBaudRateList))
            {
                usBaudrate = g_usTimingBaudRateList[item->ucbaudrate_index -1];
                break;
            }
        }
        else if (item->id == MRS_STA_ITEM_ID_TRAN_DATA_CLT_II)
        {
            usBaudrate = item->option * 100;
            break;
        }

        ret = mrsFind698MeterAddr(data, usDataLen, (HI_U8 *)aucMeter);
        if (ret == HI_ERR_SUCCESS)
        {
            pucMeter = (HI_U8 *)aucMeter;
            ucProtocol = METER_PROTO_698_45;
        }
        else
        {
            pucMeter = data + pos + MRS_645_FRAME_METERADD_OFFSET;
            ucProtocol = mrsToolsGet645Ver(*(data + pos + MRS_645_FRAME_CTRL_OFFSET));
        }
        
        usBaudrate = mrsCltiiGetMeterBaudRate(pucMeter, ucProtocol);
    } while(HI_FALSE);
    
    MRS_ChangeBaudRate((HI_U32)usBaudrate, MRS_SRV_UART_PARITY_EVEN);
}


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
HI_U32 mrsBoardTimingSendPro(HI_U8 *pTimingData, HI_U16 usDataLen, HI_U8 *pucRetry)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    MRS_COLLECTOR_CTX_STRU *pstCollectorCtx = mrsGetCollectorCtx();
    HI_U32 queue_ttl = 0;
    HI_U8 ucMaxCnt = 0;
    HI_U16 usBaudrate = 0;
    P_MRS_SRV_BAUDRATE_CFG_S pstBausRateCfg = HI_NULL;

    pstBausRateCfg = mrsSrvGetBaudRateCfg(MRS_SRV_ID_BAUDRATE_ADJUST_TIME);
    if (!pstBausRateCfg)
    {
        ucMaxCnt = 1;
        usBaudrate = MRS_SRV_BAUD_RATE_DEFAULT;
    }
    else
    {
        ucMaxCnt = pstBausRateCfg->ucValidNum;
        usBaudrate = pstBausRateCfg->usBaudRateList[pstCollectorCtx->ucTimingCnt];
    }

    pstCollectorCtx->ucTimingCnt++;
    if (pstCollectorCtx->ucTimingCnt >= ucMaxCnt)
    {
        *pucRetry = MRS_QUEUE_MAX_TRY;
        pstCollectorCtx->ucTimingCnt = 0;
        queue_ttl = MRS_COLLECTOR_SEND_BOARD_END_TIME;
    }
    else
    {
        *pucRetry = 0;
        queue_ttl = MRS_GET_UART_SEND_TIME(usBaudrate, usDataLen) + pstCollectorCtx->ulTimingSendInterval;
    }

    ret = MRS_ChangeBaudRate(usBaudrate, MRS_SRV_UART_PARITY_EVEN);
    if (ret != HI_ERR_SUCCESS)
    {
        return ret;
    }
    
    ret = MRS_UartWrite(pTimingData, usDataLen, HI_DMS_CHL_UART_PORT_APP);
    
    // 开启定时器,激活队列处理函数
    MRS_StartTimer(MRS_STA_TIMER_TTL, queue_ttl, HI_SYS_TIMER_ONESHOT);
    
    return ret;
}


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
HI_VOID mrsRunLedPro(HI_BOOL bFirst)
{
    MRS_COLLECTOR_CTX_STRU *pstCollectorCtx = mrsGetCollectorCtx();
    
    if (bFirst) //首次上电初始化执行
    {
        HI_MDM_LED_Off(LED_RUN_STATUS);        
    }
    
    if (pstCollectorCtx->ucRunLedStatus == 0)
    {
        HI_MDM_LED_On(LED_RUN_STATUS);
        pstCollectorCtx->ucRunLedStatus = 1;
    }
    else
    {
        HI_MDM_LED_Off(LED_RUN_STATUS);
        pstCollectorCtx->ucRunLedStatus = 0;
    }

    MRS_StartTimer(MRS_STA_TIMER_RUN_LED, g_run_led_period, HI_SYS_TIMER_ONESHOT);

    return;
}

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
HI_VOID mrsUARTRTxLedPro(HI_VOID)
{
    MRS_COLLECTOR_CTX_STRU *pstCltIICtx = mrsGetCollectorCtx();
    HI_U8 ucPeriod = MRS_STA_TIME_UART_LED_NORMAL;

    if (HI_NULL != pstCltIICtx)
    {
        if (MRS_COLLECTOR_RUN_LED_MODE_STANDARD == pstCltIICtx->ucRunLedMode)
        {
            ucPeriod = MRS_STA_TIME_UART_LED_TEST;
        }
    }
    
    HI_MDM_LED_Set485Led(ucPeriod);
}

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
HI_U32 mrsCollectorSendMrData(HI_U8 *pCollectorData, HI_U16 usMrDataLen, HI_U32 over_time)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    MRS_STA_SRV_CTX_STRU * pstStaCtx = mrsStaGetContext();
    MRS_STA_ITEM *pstItem = mrsSrvQueueTop(&pstStaCtx->stQueModule.stMrQueue);
    HI_U32 ulBaudRate = mrsGetCurBaudRate();
    HI_U32 ulTimeout = over_time + MRS_GET_UART_SEND_TIME(ulBaudRate, usMrDataLen);
   
    ret = MRS_SendMrData(pCollectorData, usMrDataLen, HI_DMS_CHL_UART_PORT_APP);

    // 开启定时器,激活队列处理函数
    mrsStaStartTtlTimer(pstItem, ulTimeout);

    return ret;
}

//*****************************************************************************
// 函数名称: mrsCltiiTimingPreProc
// 功能描述: II采收到透抄帧后的校时帧预处理
//           
// 参数说明:
//    pstFrame    透抄PLC内容
//    pData       645帧
//    pusLen      645帧长度
//    bFlag       透抄校时帧标志
//                            
// 返 回 值: 
//    成功返回  HI_ERR_SUCCESS
//
// 调用要求: 
// 调用举例: 
// 作    者: fengxiaomin/f00209182 [2015-07-18]
//*****************************************************************************
HI_U32 mrsCltiiTimingPreProc(MRS_PLC_FRAME_DATA_STRU *pstFrame, HI_PVOID *ppData, HI_U16 *pusLen, HI_BOOL *bFlag)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U16 usPos = 0, usFrameLen = 0;
    HI_U8 aucMeter[HI_METER_ADDR_LEN] = {0};
    HI_U8 *pFrame = HI_NULL;
    HI_U16 usLen = 0;
    
    if (!pstFrame || !ppData || !pusLen)
    {
        HI_DIAG_LOG_MSG_E3(MRS_FILE_LOG_FLAG_060, HI_DIAG_MT("timing param err"), pstFrame, ppData, pusLen);
        return HI_ERR_INVALID_PARAMETER;
    }

    do
    {
        HI_U16 usCtrl = 0;
        MRS_DATA_TRANSMIT_AFN13_DL_STRU *transmit = HI_NULL;

        transmit = (MRS_DATA_TRANSMIT_AFN13_DL_STRU *)pstFrame->pucPayload;        
        pFrame = (HI_U8*)pstFrame->pucPayload + transmit->stru_size;
        usLen = transmit->data_len;
        
        ulRet = mrsFind645Frame(pFrame, (HI_S16)usLen, &usPos, &usFrameLen);
        if (HI_ERR_SUCCESS != ulRet)
        {
            HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_059, HI_DIAG_MT("timing not find 645"), ulRet);
            ulRet = HI_ERR_SUCCESS;
            break;
        }

        if (MRS_CLTII_TIMING_645_CTRL != *(pFrame + usPos + MRS_645_FRAME_CTRL_OFFSET))
        {
            HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_058, HI_DIAG_MT("645 ctrl=%d"), *(pFrame + usPos + MRS_645_FRAME_CTRL_OFFSET));
            ulRet = HI_ERR_SUCCESS;
            break;
        }

        (hi_void)memcpy_s(aucMeter, HI_METER_ADDR_LEN, pFrame + usPos + MRS_645_FRAME_METERADD_OFFSET, HI_METER_ADDR_LEN);

        ulRet = mrsCltiiCheckMeterAddr(aucMeter);
        if (HI_ERR_SUCCESS != ulRet)
        {
            HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_057, HI_DIAG_MT("not meter"));
            ulRet = HI_ERR_NOT_EXIST;
            break;
        }

        if (*(pFrame + usPos + MRS_645_FRAME_LENGTH_OFFSET) <= MRS_CLTII_TIMING_645_CTRL_LEN)
        {
            ulRet = HI_ERR_FAILURE;
            break;
        }
        
        (hi_void)memcpy_s(&usCtrl, sizeof(usCtrl), pFrame + usPos + MRS_645_FRAME_DATA_OFFSET, sizeof(usCtrl));
        mrs645DataDecode((HI_U8*)&usCtrl, sizeof(usCtrl));
        mrsCltiiSetBaudRateIndex(usCtrl & MRS_CLTII_TIMING_645_CTRL_MASK);
        (hi_void)memcpy_s(mrsGetCollectorCtx()->aucMeter, HI_METER_ADDR_LEN, aucMeter, HI_METER_ADDR_LEN);
        mrsCltiiSaveTimingProtocol(aucMeter);
        
        *ppData = pFrame + usPos + MRS_CLTII_TIMING_645_TRANSFER_OFFSET;
        *pusLen = *(pFrame + usPos + MRS_645_FRAME_LENGTH_OFFSET) - MRS_CLTII_TIMING_645_CTRL_LEN;
        mrs645DataDecode(*ppData, *pusLen);
        *bFlag = HI_TRUE;
        HI_DIAG_LOG_MSG_E3(MRS_FILE_LOG_FLAG_056, HI_DIAG_MT("meter"),aucMeter[0],aucMeter[1],aucMeter[2]);
        HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_055, HI_DIAG_MT("pusLen=%d"),*pusLen);
        HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1000, HI_DIAG_MT("PLC_RX timing"), *ppData, *pusLen);
    } while (0);

    return ulRet;
}

//*****************************************************************************
// 函数名称: mrsCltiiSaveTimingProtocol
// 功能描述: 设置II采透明校时时的规约类型
//           
// 参数说明:
//    pucMeter      表地址
//    pucProtocol   表地址的规约类型
//                            
// 返 回 值: 
//    成功返回  HI_ERR_SUCCESS
//
// 调用要求: 
// 调用举例: 
// 作    者: fengxiaomin/f00209182 [2015-07-20]
//*****************************************************************************
HI_VOID mrsCltiiSaveTimingProtocol(HI_U8 pucMeter[])
{
    HI_U8 ucProtocol = 0;
    HI_U32 ulRet = HI_ERR_SUCCESS;
    
    ulRet = mrsCltiiGetMeterProtocol(pucMeter, &ucProtocol);
    if (HI_ERR_SUCCESS != ulRet)
    {
        return;
    }

    mrsGetCollectorCtx()->ucProtoType = ucProtocol;
}

//*****************************************************************************
// 函数名称: mrsCltiiGetMeterProtocol
// 功能描述: 获取II采表地址的规约类型
//           
// 参数说明:
//    pucMeter      表地址
//    pucProtocol   表地址的规约类型
//                            
// 返 回 值: 
//    成功返回  HI_ERR_SUCCESS
//
// 调用要求: 
// 调用举例: 
// 作    者: fengxiaomin/f00209182 [2015-07-20]
//*****************************************************************************
HI_U32 mrsCltiiGetMeterProtocol(HI_U8 pucMeter[], HI_U8 *pucProtocol)
{
    MRS_STA_SRV_CTX_STRU *pstSta = HI_NULL;
    HI_U16 usIndex = 0;

    if (!pucProtocol)
    {
        return HI_ERR_INVALID_PARAMETER;
    }
    
    pstSta = mrsStaGetContext();
    if (HI_TRUE == mrsFindMeterList(&pstSta->stMeterList, pucMeter, &usIndex))
    {
        *pucProtocol = pstSta->stMeterList.stMeterItem[usIndex].ucProtocol;
    }
    else if (HI_TRUE == mrsStaFindMeterOfSearchResult(pucMeter, 0, HI_FALSE, 0))
    {
        mrsFindMeterIndexOfSearchResult(pucMeter, &usIndex);
        *pucProtocol = mrsStaSearchCtx()->stMeterList.astMeter[usIndex].ucProtocol;
    }
    else
    {
        return HI_ERR_FAILURE;
    }

    return HI_ERR_SUCCESS;
}

//*****************************************************************************
// 函数名称: mrsIsCltiiTiming
// 功能描述: II采是否在透传校时
//           
// 参数说明:
//    无
//                            
// 返 回 值: 
//    成功返回  HI_TRUE
//
// 调用要求: 
// 调用举例: 
// 作    者: fengxiaomin/f00209182 [2015-07-18]
//*****************************************************************************
HI_BOOL mrsIsCltiiTiming(HI_VOID)
{
    MRS_STA_ITEM  *pstItem;
    MRS_SRV_QUEUE *pstQueue;
    MRS_STA_SRV_CTX_STRU * pstSta = mrsStaGetContext();

    pstQueue = &pstSta->stQueModule.stMrQueue;
    // 获取首结点
    pstItem = mrsSrvQueueTop(pstQueue);
    if (pstItem == HI_NULL) 
    {
        MRS_UNLOCK(pstSta->stQueModule.lock);  // 释放控制权
        MRS_StopTimer(MRS_STA_TIMER_RM_QUEUE_LOCK);
        return HI_FALSE;
    }

    if ((ID_MRS_CMD_DATA_TRANSMIT_AFN13_PLC == pstItem->id) 
        && (MRS_STA_TRANSFER_TIMING_CLTII_EX == pstItem->sub_id))
    {
        return HI_TRUE;
    }
    
    return HI_FALSE;
}

HI_VOID mrsCltiiSetBaudRateIndex(HI_U8 ucIndex)
{
    mrsGetCollectorCtx()->ucBaudRateIndex = ucIndex;
}

HI_U8 mrsCltiiGetBaudRateIndex(HI_VOID)
{
    return mrsGetCollectorCtx()->ucBaudRateIndex;
}



//*****************************************************************************
// 函数名称: mrsCltiiTransTimingProc
// 功能描述: II采转发校时帧处理
//           
// 参数说明:
//    pData      645帧
//    usDataLen  645帧长度
//                            
// 返 回 值: 
//    成功返回  HI_ERR_SUCCESS
//
// 调用要求: 
// 调用举例: 
// 作    者: fengxiaomin/f00209182 [2015-07-18]
//*****************************************************************************
HI_U32 mrsCltiiTransTimingProc(HI_U8 *pData, HI_U16 usDataLen, HI_U8 *pucRetry, HI_U32 ulTimeOut)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U8 ucIndex = 0;
    HI_U32 ulBaudRate = 0;

    if (!pData)
    {
        return HI_ERR_INVALID_PARAMETER;
    }
    
    ucIndex = mrsCltiiGetBaudRateIndex();
    if (ucIndex >= MRS_TOOLS_ARRAY_COUNT(g_usTimingBaudRateList))
    {
        return HI_ERR_FAILURE;
    }
    
    HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_051, HI_DIAG_MT("CltiiTransTiming baudrate"),g_usTimingBaudRateList[ucIndex]);
    ulRet = MRS_ChangeBaudRate(g_usTimingBaudRateList[ucIndex], MRS_SRV_UART_PARITY_EVEN);
    if (ulRet != HI_ERR_SUCCESS)
    {
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_010, HI_DIAG_MT("CltiiTransTiming set baudrate fail"));
        return ulRet;
    }
    
    ulRet = MRS_SendMrData(pData, usDataLen, HI_DMS_CHL_UART_PORT_APP);
    
    // 开启定时器,激活队列处理函数
    ulBaudRate = mrsGetCurBaudRate();
    ulTimeOut += MRS_GET_UART_SEND_TIME(ulBaudRate, usDataLen);
    MRS_StartTimer(MRS_STA_TIMER_TTL, ulTimeOut, HI_SYS_TIMER_ONESHOT);
    (*pucRetry)++;
    
    return ulRet;
}

//*****************************************************************************
// 函数名称: mrsCreateTimingUpFrame
// 功能描述: 创建II采转发校时上行645帧
//           
// 参数说明:
//    ppData      校时上行645帧
//    pusDataLen  校时上行645帧长度
//    pstData     电表应答的645帧信息
//                            
// 返 回 值: 
//    成功返回  HI_ERR_SUCCESS
//
// 调用要求: 
// 调用举例: 
// 作    者: fengxiaomin/f00209182 [2015-07-20]
//*****************************************************************************
HI_U32 mrsCreateTimingUpFrame(HI_U8 **ppData, HI_U16 *pusDataLen, MRS_MSG_MR_645_DATA_STRU* pstData)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    MRS_645_FRAME_STRU* pstFrame = HI_NULL;
    HI_U8 ucCtrl = MRS_CLTII_TIMING_645_CTRL + MRS_CLTII_TIMING_645_UP_CTRL_INCREAZE;

    if (!ppData || !pusDataLen)
    {
        return HI_ERR_INVALID_PARAMETER;
    }
    
    *pusDataLen = MRS_645_FRAME_LENGTH_MIN;
    if (pstData)
    {
        *pusDataLen += pstData->usDataSizse;
    }

    *ppData = (HI_U8*)mrsToolsMalloc(MRS_PROTO645_DATAGRAM_LEN_MAX);
    if (!*ppData)
    {
        return HI_ERR_MALLOC_FAILUE;
    }

    (hi_void)memset_s(*ppData, MRS_PROTO645_DATAGRAM_LEN_MAX, 0, MRS_PROTO645_DATAGRAM_LEN_MAX);
   
    pstFrame = (MRS_645_FRAME_STRU*)mrsToolsMalloc(sizeof(MRS_645_FRAME_STRU));
    if (!pstFrame)
    {
        mrsToolsFree(*ppData);
        return HI_ERR_MALLOC_FAILUE;
    }
    
    (hi_void)memset_s(pstFrame, sizeof(MRS_645_FRAME_STRU), 0, sizeof(MRS_645_FRAME_STRU));
    (hi_void)memcpy_s(&pstFrame->stCtrl, sizeof(MRS_645_CTRL_STRU), &ucCtrl, sizeof(ucCtrl));
    (hi_void)memcpy_s(pstFrame->ucAddr, HI_METER_ADDR_LEN, mrsGetCollectorCtx()->aucMeter, HI_METER_ADDR_LEN);
    
    if (pstData)
    {
        pstFrame->ucDataRealmLen = (HI_U8)pstData->usDataSizse;
        if (pstFrame->ucDataRealmLen)
        {
            if(sizeof(pstFrame->ucDataRealm) >= pstData->usDataSizse){
				(hi_void)memcpy_s(pstFrame->ucDataRealm, sizeof(pstFrame->ucDataRealm), pstData->pucData, pstData->usDataSizse);	
			} 
            mrs645DataEncode(pstFrame->ucDataRealm, pstFrame->ucDataRealmLen);
        }
    }
    
    ulRet = MRS_Proto645Enc(*ppData, pusDataLen, pstFrame);
    
    mrsToolsFree(pstFrame);

    return ulRet;
}

//*****************************************************************************
// 函数名称: mrsCltiiTimingTimeOutProc
// 功能描述: II采转发校时超时处理
//           
// 参数说明:
//    pstItem      队列首节点信息
//                            
// 返 回 值: 
//    成功返回  HI_ERR_SUCCESS
//
// 调用要求: 
// 调用举例: 
// 作    者: fengxiaomin/f00209182 [2015-07-20]
//*****************************************************************************
HI_U32 mrsCltiiTimingTimeOutProc(MRS_STA_ITEM *pstItem)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    
    if (!pstItem || !mrsToolsIsIICollector())
    {
        return HI_ERR_FAILURE;
    }

    if ((ID_MRS_CMD_DATA_TRANSMIT_AFN13_PLC == pstItem->id) 
        && (MRS_STA_TRANSFER_TIMING_CLTII_EX == pstItem->sub_id))
    {
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_052, HI_DIAG_MT("timingTimeOutProc"));
        ulRet = mrsStaSerialRespRMPLC(pstItem, HI_NULL);
    }

    return ulRet;
}



HI_VOID mrsSetCollectorAddr(HI_U8 *pucDeviceAddr)
{
    MRS_COLLECTOR_CTX_STRU *pstCltCtx = mrsGetCollectorCtx();
    (hi_void)memcpy_s(pstCltCtx->aucDevice, HI_METER_ADDR_LEN, pucDeviceAddr, HI_METER_ADDR_LEN);
}

HI_U8 *mrsGetCollectorAddr(HI_VOID)
{
    MRS_COLLECTOR_CTX_STRU *pstCltCtx = mrsGetCollectorCtx();
    return pstCltCtx->aucDevice;
}

//*****************************************************************************
// 函数名称: mrsCltiiProtoProc
// 功能描述: II采收到透传红外协议处理
//           
// 参数说明:
//    无
//                            
// 返 回 值: 
//    成功返回  HI_ERR_SUCCESS
//
// 调用要求: 
// 调用举例: 
// 作    者: fengxiaomin/f00209182 [2015-10-31]
//*****************************************************************************
HI_U32 mrsCltiiProtoProc(HI_U8 *pData, HI_U16 usDataLen, MRS_STA_ITEM* pstItem)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U16 usPos = 0, usFrameLen = 0;
    MRS_IR_645_DI_FUN pIrDIProc = HI_NULL;
    MRS_COLLECTOR_645_FRAME_STRU *pst645Frame = HI_NULL;

    ulRet = mrsFind645Frame(pData, (HI_S16)usDataLen, &usPos, &usFrameLen);
    if (HI_ERR_SUCCESS != ulRet)
    {
        return HI_ERR_SKIP;
    }
    
    //解析645帧
    pst645Frame = (MRS_COLLECTOR_645_FRAME_STRU *)mrsToolsMalloc(sizeof(MRS_COLLECTOR_645_FRAME_STRU));
    if(pst645Frame == HI_NULL)
    {
        return HI_ERR_SKIP;
    }
    
    (hi_void)memset_s(pst645Frame, sizeof(MRS_COLLECTOR_645_FRAME_STRU), 0, sizeof(MRS_COLLECTOR_645_FRAME_STRU));

    do
    {
        //检查645帧内容是否正确
        ulRet = mrsIrDA645Dec(pData + usPos, usFrameLen, pst645Frame, MRS_CLTII_TRANSFER_FROM);
        if (ulRet == HI_ERR_SUCCESS)
        {
            pIrDIProc = mrsIrFrameProc(pst645Frame->ucCtrl, pst645Frame->ucDataRealm);
            if (pIrDIProc)
            {
                mrsCltiiProtoResp(pst645Frame, pIrDIProc, pstItem);
                mrsToolsFree(pst645Frame);
                
                return HI_ERR_SUCCESS;
            }
        }
    } while (0);

    mrsToolsFree(pst645Frame);
    
    return HI_ERR_SKIP;
}

//*****************************************************************************
// 函数名称: mrsCltiiProtoResp
// 功能描述: II采发送应答PLC透抄帧给CCO
//           
// 参数说明:
//    无
//                            
// 返 回 值: 
//    成功返回  HI_ERR_SUCCESS
//
// 调用要求: 
// 调用举例: 
// 作    者: fengxiaomin/f00209182 [2015-10-31]
//*****************************************************************************
HI_U32 mrsCltiiProtoResp(MRS_COLLECTOR_645_FRAME_STRU *pst645Frame, MRS_IR_645_DI_FUN pIrDIProc, MRS_STA_ITEM* pstItem)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    MRS_STA_ITEM stItem;
    
    HI_U8 *pData = HI_NULL;
    HI_U16 usDataLen = 0;
    
    ulRet = pIrDIProc(pst645Frame, (HI_PVOID *)&pData, &usDataLen, 0);
    if (ulRet != HI_ERR_SUCCESS)
    {
        mrsAssembleDFrame((HI_VOID **)&pData, &usDataLen, MRS_IR_ERR_NOT_EXIST, pst645Frame, 0);
    }

    (hi_void)memset_s(&stItem, sizeof(stItem), 0, sizeof(stItem));
    stItem.id = pstItem->id;
    stItem.seq = pstItem->seq;
    (hi_void)memcpy_s(stItem.dev_mac, HI_PLC_MAC_ADDR_LEN, pstItem->dev_mac, HI_PLC_MAC_ADDR_LEN);
	
    //注:II采应答时,会将4个0xFE加在帧前,而报给CCO的都不带0xFE
    ulRet = mrsStaTransmit2CCO(pData, usDataLen, METER_PROTO_645_2007, &stItem);
    HI_DIAG_LOG_MSG_E3(MRS_FILE_LOG_FLAG_045, HI_DIAG_MT("send irda data PLC"),pData,usDataLen,ulRet);

    mrsToolsFree(pData);
    
    return HI_ERR_SUCCESS;
}

HI_U32 mrsCltiiCheckAddr(HI_U8 *pAddr)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;

    ulRet = mrsCltiiCheckCltAddr(pAddr);
    if (HI_ERR_SUCCESS == ulRet)
    {
        return ulRet;
    }

    return mrsCltiiCheckMeterAddr(pAddr);
}

HI_U32 mrsCltiiCheckCltAddr(HI_U8 *pAddr)
{
    HI_U8 *pCltAddr = HI_NULL;

    pCltAddr = mrsGetCollectorAddr();
    if (mrsToolsCollectorAddr(pAddr) || !mrsMeterAddrMatch(pAddr, pCltAddr))
    {
        return HI_ERR_FAILURE;
    }
    
    return HI_ERR_SUCCESS;
}

HI_U32 mrsCltiiCheckMeterAddr(HI_U8 *pAddr)
{
    HI_U16 usIndex = 0;
    MRS_STA_SRV_CTX_STRU *pstSta = HI_NULL;

    pstSta = mrsStaGetContext();
    
    if ((HI_FALSE == mrsFindMeterList(&pstSta->stMeterList, pAddr, &usIndex))
        && (HI_FALSE == mrsStaFindMeterOfSearchResult(pAddr, 0, HI_FALSE, 0)))
    {
        return HI_ERR_NOT_EXIST;
    }
    
    return HI_ERR_SUCCESS;
}
/* BEGIN: Modified by cuichunyan/c00276962 2016/11/10  问题单号:cltii R485 MultiConnect */

HI_U32 mrsRefreshRS485MultiConn(HI_VOID)
{
    MRS_COLLECTOR_CTX_STRU * pstCtx = mrsGetCollectorCtx();
    MRS_CLTII_R485_MULTI_CONN_STRU * pstInf = &pstCtx->stMultiConn;
    MRS_RS485_MULTI_CONNECT_ITEM stItem = {0};
    stItem.ulOccurTime = HI_MDM_GetSeconds(); 
    pstInf->ulTotalTimes++;
    
    #if MRS_RS485_MULTI_CONNECT_MAX_TIMES > 1
    (hi_void)memmove_s(&pstInf->stRecord[0], sizeof(MRS_RS485_MULTI_CONNECT_ITEM) * MRS_RS485_MULTI_CONNECT_MAX_TIMES, &pstInf->stRecord[1], sizeof(MRS_RS485_MULTI_CONNECT_ITEM) * (MRS_RS485_MULTI_CONNECT_MAX_TIMES - 1));
    #endif

    (hi_void)memcpy_s(&pstInf->stRecord[0], sizeof(MRS_RS485_MULTI_CONNECT_ITEM), (HI_PBYTE)&stItem, sizeof(MRS_RS485_MULTI_CONNECT_ITEM));    

    if (pstInf->ucRecordTimes < MRS_RS485_MULTI_CONNECT_MAX_TIMES)
    {
        pstInf->ucRecordTimes++;
    }


    return HI_ERR_SUCCESS;
}

/* END  : Modified by cuichunyan/c00276962 2016/11/10  问题单号:cltii R485 MultiConnect */


HI_U32  mrsRS485TranDataInd(HI_IN HI_PBYTE pucDatagram, HI_U16 usDatagramSize, HI_U8 ucCmdId)
{
    HI_U32 ulRet = HI_ERR_FAILURE;
    DIAG_MRS_RS485_TRANSMIT_IND_STRU * pstInd = HI_NULL;
    HI_U16 usDataSize = 0;
    
    if ((pucDatagram == HI_NULL) || (usDatagramSize == 0))
    {
        HI_DIAG_LOG_MSG_E0(__LINE__, HI_DIAG_MT("parma is null"));
        return ulRet;
    }

    usDataSize = sizeof(DIAG_MRS_RS485_TRANSMIT_IND_STRU) + usDatagramSize;
    pstInd = mrsToolsMalloc(usDataSize);
    if (pstInd == HI_NULL)
    {
        HI_DIAG_LOG_MSG_E0(__LINE__, HI_DIAG_MT("malloc  failue"));
        return HI_ERR_MALLOC_FAILUE;
    }

    (hi_void)memset_s(pstInd, usDataSize, 0, usDataSize);
    pstInd->ucVer = MRS_CMD_STRU_VER_R1;
    pstInd->usLen = usDatagramSize;
    pstInd->ucStructSize = sizeof(DIAG_MRS_RS485_TRANSMIT_IND_STRU);
    (hi_void)memcpy_s(pstInd->data, usDatagramSize, pucDatagram, usDatagramSize);
    
    ulRet = HI_MDM_ReportPacket(HI_CLTII_R485_TRANSMIT_DATA,
                                 ucCmdId,
                                 (HI_PBYTE)pstInd,
                                 usDataSize,
                                 HI_TRUE);
                                 
    mrsToolsFree(pstInd);
    
    return ulRet;
}

HI_BOOL mrsCheckRS485Monitor(HI_VOID)
{
    return ((mrsGetCollectorCtx()->ucR485Monitor == HI_TRUE));
}

/* END  : Modified by cuichunyan/c00276962 2016/11/11  问题单号:cltii R485 test by BaudRate */


HI_U16 mrsCltiiGetMeterBaudRate(HI_U8 *pucMeter, HI_U8 ucProtocol)
{
    MRS_CLTII_LAST_BC_METER_INF *pstLastInf = mrsCltiiGetLastBcMeterInf();
    MRS_SRV_BAUDRATE_ID_E eId = MRS_SRV_ID_BAUDRATE_RM_07;
    MRS_SRV_BAUDRATE_CFG_S *pstCfg = HI_NULL;
    MRS_METER_ITEM_S *pstMeter = mrsStaGetLocalMeterInf(pucMeter);

    if (METER_PROTO_645_1997 == ucProtocol)
    {
        eId = MRS_SRV_ID_BAUDRATE_RM_97;
    }
    else if (METER_PROTO_698_45 == ucProtocol)
    {
        eId = MRS_SRV_ID_BAUDRATE_RM_698;
    }

    pstCfg = mrsSrvGetBaudRateCfg(eId);
    if (!pstCfg)
    {
    	if (pstMeter)
    	{
        	return pstMeter->usBaudRate;
    	}
        return MRS_SRV_BAUD_RATE_DEFAULT;
    }

    if ((pstLastInf->bValid) && (mrsToolsMemEq(pstLastInf->aucMeter, pucMeter, HI_METER_ADDR_LEN)))
    {
        if (pstLastInf->ucCount >= MRS_QUEUE_MAX_TRY)
        {
            pstLastInf->ucCount = 0;
            pstLastInf->ucIndex = ((pstLastInf->ucIndex + 1) % pstCfg->ucValidNum);
        }
    }
    else
    {
        mrsCltiiClearLastBcMeterInf();

	if (pstMeter)
        {
            HI_U8 i = 0;
            for (i = 0; i < pstCfg->ucValidNum; i++)
            {
                if (pstMeter->usBaudRate == pstCfg->usBaudRateList[i])
                {
                    pstLastInf->ucIndex = i;
                    break;
                }
            }
        }

        (hi_void)memcpy_s(pstLastInf->aucMeter, HI_METER_ADDR_LEN, pucMeter, HI_METER_ADDR_LEN);
        pstLastInf->ucProtocol = ucProtocol;
        pstLastInf->bValid = HI_TRUE;
    }

    pstLastInf->ucCount++;

    return pstCfg->usBaudRateList[pstLastInf->ucIndex];
}


HI_PRV MRS_CLTII_LAST_BC_METER_INF g_stLastBcMeterInf = {0};

// Return the meter information for the last broadcast-reading failure.
MRS_CLTII_LAST_BC_METER_INF *mrsCltiiGetLastBcMeterInf(HI_VOID)
{
    return &g_stLastBcMeterInf;
}

// Clear the meter information for the last broadcast-reading failure.
HI_VOID mrsCltiiClearLastBcMeterInf(HI_VOID)
{
    (hi_void)memset_s(mrsCltiiGetLastBcMeterInf(), sizeof(MRS_CLTII_LAST_BC_METER_INF), 0, sizeof(MRS_CLTII_LAST_BC_METER_INF));
}


#endif
