//*****************************************************************************

//
//                  版权所有 (C), 1998-2011, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_srv_cco_readmeter.c
//  版 本 号   : V1.0
//  作    者   : liuxipeng/KF54842
//  生成日期   : 2011-12-13
//  功能描述   : 读表模块函数实现
//
//  函数列表   :
//  修改历史   :
//  1.日    期 : 2011-12-13
//    作    者 : liuxipeng/KF54842
//    修改内容 : 创建文件
//
//*****************************************************************************
#include "mrs_common.h"
#include "mrs_cmd_msg.h"
#include "mrs_fw_proto645.h"
#include "mrs_fw_proto376_2.h"
#include "mrs_fw_proto376_2_echo.h"
#include "mrs_fw_proto376_2_frame.h"
#include "mrs_fw_tools.h"
#include "mrs_fw_log.h"
#include "mrs_srv_common.h"
#include "mrs_srv_cco_macmap.h"
#include "mrs_srv_parallel_cco.h"
#include "mrs_srv_list.h"
#include "mrs_srv_cco_event.h"
#include "mrs_srv_cco_remote_upg.h"
#include "mrs_srv_cco.h"
#include "mrs_srv_cco_readmeter.h"
#include "mrs_fw_n.h"
#include "mrs_dfx.h"
#include "mrs_srv_cco_searchmeter.h"
#include "mrs_srv_mrslog.h"
#include "mrs_srv_cco_queue.h"
#include "mrs_srv_cco_archives.h"
#include "mrs_srv_cco_testmode.h"
#include "mrs_srv_cco_detect_rm.h"

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)

HI_VOID mrsSetReadMeterState(MRS_MRV_CCO_READMETER_STATE_ENUM state)
{
    MRS_CCO_SRV_CTX_STRU *cco_ctx = mrsCcoGetContext();

    cco_ctx->stReadMeterModel.enCurrState = state;
    mrsDfxLrState(state);
}

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO_BROAD_BAND_UNIT)
HI_VOID mrsInitMrStateBbu(HI_VOID)
{
    HI_U32 status = mrsCcoGetState();

    status &= ~MRS_CCO_STATUS_NORMAL;
    mrsCcoSetState(status);
}
#endif

//*****************************************************************************
// 函数名称: mrsReadMeterFirstRunProcess
// 功能描述: 读表模块第一次运行处理(重构)
//
// 参数说明:
//   pstCcoCtx [in/out] 当前状态
//
// 返 回 值:
//    HI_ERR_SUCCESS 成功
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: liuxipeng/KF54842 [2011-12-22]
//*****************************************************************************
HI_U32 mrsReadMeterFirstRunProcess(MRS_CCO_SRV_CTX_STRU* pstCcoCtx)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U16 i = 0; //新添加的标识
    HI_U16 usMeterNum = mrsSrvArchivesMeterNum();

    if (pstCcoCtx == HI_NULL)
    {
        ret = HI_ERR_FAILURE;
        return ret;
    }

    pstCcoCtx->stReadMeterModel.enMeterInfState = EN_MRS_SRV_CCO_METERINF_STATE_NORMAL;

    mrsReadMeterStateInit();  // 读表模块状态初始化

    if (usMeterNum == 0)
    {        
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_001, HI_DIAG_MT("no meter"));
        return HI_ERR_NOT_FOUND;   // 表库中没有电表
    }

    //在此处找到当前电表信息中第一个有效的表信息（序号）
    for (i = 0; i < PRODUCT_CFG_MRS_MAX_NODE_NUM; i++)
    {
        MRS_ARCHIVES_NODE_STRU *pstNode = HI_NULL;

        pstNode = mrsSrvArchivesQuery(i);
        if (pstNode == HI_NULL)
        {
            break;
        }

        if (pstNode->bValid && !pstNode->ucRead) // 查找到一个有效的表信息
        {
            pstCcoCtx->stReadMeterModel.usCurrNodeSeq = i;
            (hi_void)memcpy_s(&pstCcoCtx->stReadMeterModel.stCurrNode, sizeof(pstCcoCtx->stReadMeterModel.stCurrNode),
                pstNode, sizeof(MRS_ARCHIVES_NODE_STRU));
            pstCcoCtx->stReadMeterModel.ulTimeOutCount = 0;   // 找到新节点初始化超时次数
            break;
        }
    }

    return HI_ERR_SUCCESS;
}


//*****************************************************************************
// 函数名称: mrsReadterModelCco
// 功能描述: 读表模块处理入口函数
//
// 参数说明:
//  This function has no arguments.
//
// 返 回 值:
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: liuxipeng/KF54842 [2011-12-15]
//*****************************************************************************
HI_VOID mrsReadterModelCco(HI_VOID * pAckParam)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    MRS_CCO_SRV_CTX_STRU* pstCcoCtx = mrsCcoGetContext(); // 定义指向全局上下文结构的指针
    MRS_ARCHIVES_NODE_STRU *pstNode = HI_NULL;
    HI_BOOL bLoopFlg = HI_TRUE;  // 循环
    HI_U16 usMeterNum = mrsSrvArchivesMeterNum();

    MRS_NOT_USED(pAckParam);

    MRS_StopTimer(EN_MRS_SRV_CCO_READMETER_ACTIV_TIMER);    // 程序已执行(将激活定时器停止)
#if 0//optimized by weichao
    if (HI_FALSE == mrsCcoClockStatusAllow())
    {
        return;
    }
#endif 
    if (pstCcoCtx->stReadMeterModel.enMeterInfState == EN_MRS_SRV_CCO_METERINF_STATE_FIRST)   // 首次上电
    {
        ret = mrsReadMeterFirstRunProcess(pstCcoCtx);
        if (ret != HI_ERR_SUCCESS)
        {
            // 延时MRS_SRV_CCO_TIMER_READMETER_ACTIV_VALUE时间后重新激活读表模块
            MRS_StartTimer(EN_MRS_SRV_CCO_READMETER_ACTIV_TIMER, MRS_SRV_CCO_TIMER_READMETER_ACTIV_VALUE,
                           HI_SYS_TIMER_ONESHOT);

            return;
        }
    }

    if (0 == usMeterNum)   //没有电表
    {
        (hi_void)memset_s(&pstCcoCtx->stReadMeterModel.stCurrNode, sizeof(MRS_ARCHIVES_NODE_STRU), 0, sizeof(MRS_ARCHIVES_NODE_STRU));
        pstCcoCtx->stReadMeterModel.usCurrNodeSeq = 0;  //(改为这样)
        mrsReadMeterStateInit();

        // 延时MRS_SRV_CCO_TIMER_READMETER_ACTIV_VALUE时间后重新激活读表模块
        MRS_StartTimer(EN_MRS_SRV_CCO_READMETER_ACTIV_TIMER, MRS_SRV_CCO_TIMER_READMETER_ACTIV_VALUE * 25,
                       HI_SYS_TIMER_ONESHOT);

        return;
    }

    if (0 == (pstCcoCtx->status & MRS_CCO_STATUS_NORMAL)) // 如果不是轮抄状态
    {
        return;   // 非轮抄状态(不执行抄表流程)
    }

    pstNode = mrsSrvArchivesQuery(pstCcoCtx->stReadMeterModel.usCurrNodeSeq);
    if (pstNode == HI_NULL || pstNode->ucRead)
    {
        pstCcoCtx->stReadMeterModel.ucCurrNodeInquireTries = 0;
        pstCcoCtx->stReadMeterModel.usCurrNodeSeq++;
        ret = mrsReadMeterFindValidNode(pstCcoCtx);
        mrsReadMeterStateInit();  // 读表模块状态初始化

        if (HI_ERR_SUCCESS != ret)
        {
            return;
        }
    }

    if (!mrsCurrNodeValid())
    {
        ret = mrsReadMeterFindValidNode(pstCcoCtx);

        mrsReadMeterStateInit();  // 读表模块状态初始化
        
        if (HI_ERR_SUCCESS != ret)
        {
            return;
        }
    }

    while (bLoopFlg)
    {
        bLoopFlg = mrsReadMeterStateProcess(pstCcoCtx);     // 抄表状态处理
        HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_022, HI_DIAG_MT("bLoopFlg-%d"),bLoopFlg);
    }

    return;
}

//*****************************************************************************
// 函数名称: mrsWaitAskReplyProcess
// 功能描述: 等待询问应答处理(重构)
//
// 参数说明:
//   pstCcoCtx [in/out] 当前状态
//   bLoopFlg [out] 是否循环标志
//
// 返 回 值:
//    无
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: liuxipeng/KF54842 [2011-12-22]
//*****************************************************************************
HI_VOID mrsWaitAskReplyProcess(MRS_CCO_SRV_CTX_STRU* pstCcoCtx, HI_BOOL *bLoopFlg)
{
    if (pstCcoCtx->stReadMeterModel.pstAfn14F1 != HI_NULL)
    {
        pstCcoCtx->stReadMeterModel.ucCurrNodeInquireTries = 0;
        mrsSetReadMeterState(EN_MRS_SRV_CCO_READMETER_SEND_PLC);    // 置为发送读表数据
        MRS_StopTimer(EN_MRS_SRV_CCO_MUSTER_TIMER);     // 正常收到数据-将与集中器通讯定时器停止
    }
    else
    {
        *bLoopFlg = HI_FALSE;      // 需要等待
    }

    return;
}

//*****************************************************************************
// 函数名称: mrsWaitPlcReplyProcess
// 功能描述: 等待PLC应答处理(重构)
//
// 参数说明:
//   pstCcoCtx [in/out] 当前状态
//   bLoopFlg [out] 是否循环标志
//
// 返 回 值:
//    无
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: liuxipeng/KF54842 [2011-12-22]
//*****************************************************************************
HI_VOID mrsWaitPlcReplyProcess(MRS_CCO_SRV_CTX_STRU* pstCcoCtx, HI_BOOL *bLoopFlg)
{
    if (pstCcoCtx->stReadMeterModel.pstUpReadMeterData != HI_NULL)
    {
        if (pstCcoCtx->stReadMeterModel.pstAfn14F1 != HI_NULL)
        {
            pstCcoCtx->stReadMeterModel.pstAfn14F1->MRS_AFN14_FREE_FUN(pstCcoCtx->stReadMeterModel.pstAfn14F1);
            pstCcoCtx->stReadMeterModel.pstAfn14F1 = HI_NULL;
        }

        pstCcoCtx->stReadMeterModel.ulTimeOutCount = 0;   // 收到新的PLC数据初始化超时次数

        mrsSetReadMeterState(EN_MRS_SRV_CCO_READMETER_UPDATA);     // 置为上传读表数据
        MRS_StopTimer(EN_MRS_SRV_CCO_STA_TIMER);     // 正常收到数据-将与STA通讯定时器停止
    }
    else
    {
        *bLoopFlg = HI_FALSE;      // 需要等待   2015-08-18:这句执行不到
    }

    if(pstCcoCtx->stReadMeterModel.pstPlcDataDl != HI_NULL)
    {
        if(pstCcoCtx->stReadMeterModel.pstPlcDataDl->pucPayload != HI_NULL)
        {
            mrsToolsFree(pstCcoCtx->stReadMeterModel.pstPlcDataDl->pucPayload);   // 释放转发数据块   
            pstCcoCtx->stReadMeterModel.pstPlcDataDl->pucPayload = HI_NULL;
        }
        
        mrsToolsFree(pstCcoCtx->stReadMeterModel.pstPlcDataDl);   // 释放PLC帧备份    
        pstCcoCtx->stReadMeterModel.pstPlcDataDl = HI_NULL;
    }
    
    return;
}

//*****************************************************************************
// 函数名称: mrsWaitUpDataAckProcess
// 功能描述: 等待数据上传应答处理(重构)
//
// 参数说明:
//   pstCcoCtx [in/out] 当前状态
//   bLoopFlg [out] 是否循环标志
//
// 返 回 值:
//    无
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: liuxipeng/KF54842 [2011-12-22]
//*****************************************************************************
HI_VOID mrsWaitUpDataAckProcess(MRS_CCO_SRV_CTX_STRU* pstCcoCtx, HI_BOOL *bLoopFlg)
{
    PLC_OVERTIME_INF_STRU *pstAfn14 = mrsGetPlcOverTimeInfAfn14();
	
	HI_UNREF_PARAM(pstCcoCtx);
	
    mrsReadMeterStateInit();  // 读表模块状态初始化(重新开始询问)

    // 激活读表模块
    MRS_StartTimer(EN_MRS_SRV_CCO_READMETER_ACTIV_TIMER, 
                        (HI_U32)pstAfn14->MeterFrameInterval,
                        HI_SYS_TIMER_ONESHOT);
    mrsDfxLrOkCnt();    // 统计轮抄抄读成功次数

    *bLoopFlg = HI_FALSE;      // 需要等待

    return;
}

//*****************************************************************************
// 函数名称: mrsReadMeterStateProcess
// 功能描述: 读表状态处理(重构)
//
// 参数说明:
//   pstCcoCtx [IN/OUT] 读表状态信息
//
// 返 回 值:
//    HI_TRUE 持续执行
//    HI_FALSE 等待状态
//
// 调用要求:
// 调用举例:
// 作    者: liuxipeng/KF54842 [2011-12-20]
//*****************************************************************************
HI_BOOL mrsReadMeterStateProcess(HI_INOUT MRS_CCO_SRV_CTX_STRU* pstCcoCtx)
{
    HI_BOOL bLoopFlg = HI_TRUE;  // 循环标志(HI_FALSE:需要等待 HI_TRUE:持续执行)

    switch (pstCcoCtx->stReadMeterModel.enCurrState)
    {
    case EN_MRS_SRV_CCO_READMETER_IDLE:              // 空闲
        if(mrsGetNeedGetTimeFlg())
        {
            mrsSetNeedGetTimeFlg(HI_FALSE);

            MRS_StartTimer(EN_MRS_SRV_CCO_READMETER_ACTIV_TIMER, MRS_SRV_CCO_TIMER_READMETER_ACTIV_VALUE * 5,
               HI_SYS_TIMER_ONESHOT);

            return HI_FALSE;
        }
        else
        {
            mrsSetReadMeterState(EN_MRS_SRV_CCO_READMETER_ASK);     // 置为询问状态
        }
        break;

    case EN_MRS_SRV_CCO_READMETER_ASK:               // 询问
        // 获取CCO就绪状态(HI_TRUE : 已就绪可以读表; HI_FALSE : 未就绪)
        if (mrsIsCcoUpgStatus())
        {
            // 当前CCO未就绪-不允许询问读表
            mrsSetReadMeterState(EN_MRS_SRV_CCO_READMETER_IDLE);   // 空闲
            MRS_StartTimer(EN_MRS_SRV_CCO_READMETER_ACTIV_TIMER, 
                                MRS_SRV_CCO_TIMER_READMETER_REACTIV_VALUE,
                                HI_SYS_TIMER_ONESHOT);
        }
        else
        {
            mrsInquireReadMeter();  // 发送询问读表命令(向集中器发送)
        }
        bLoopFlg = HI_FALSE;      // 需要等待
        break;

    case EN_MRS_SRV_CCO_READMETER_WAIT_ASK_REPLY:    // 等待询问的应答
        mrsWaitAskReplyProcess(pstCcoCtx, &bLoopFlg);
        break;

    case EN_MRS_SRV_CCO_READMETER_WAIT_REVISE_RESP:    // 等待延时修正应答
        mrsWaitAskReplyProcess(pstCcoCtx, &bLoopFlg);
        break;
        
    case EN_MRS_SRV_CCO_READMETER_SEND_PLC:          // 发送读表数据
        mrsAssembleReadMeterData(); //组织14-01下行帧
        mrsTransmit14F1FramePlc(); //转发数据到STA
        bLoopFlg = HI_FALSE;      // 需要等待
        break;

    case EN_MRS_SRV_CCO_READMETER_WAIT_PLC_REPLY:    // 等待读表数据应答
        HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_032, HI_DIAG_MT("bLoopFlg-%d"),bLoopFlg);
        mrsWaitPlcReplyProcess(pstCcoCtx, &bLoopFlg);
        break;

    case EN_MRS_SRV_CCO_READMETER_REPEAT_PLC:        // 重发读表数据
        mrsDfxLrPlcTryCnt();    // 统计重发次数
        mrsTransmit14F1FramePlc();     // 转发数据到STA
        bLoopFlg = HI_FALSE;      // 需要等待
        break;

    case EN_MRS_SRV_CCO_READMETER_UPDATA:            // 上传读表数据(上传至集中器)
        mrsUpReterMetreData();  // 上传读表数据
        bLoopFlg = HI_FALSE;      // 需要等待
        break;

    case EN_MRS_SRV_CCO_READMETER_WAIT_UPDATA_ACK:   // 等待上传读表数据确认
        mrsWaitUpDataAckProcess(pstCcoCtx, &bLoopFlg);
        break;

    case EN_MRS_SRV_CCO_READMETER_REPEAT_UPDATA:     // 重发上传读表数据
        {
            P_MRS_SRV_CCO_QUEUE pstCcoQueue = mrsGetCcoQueue();
            MRS_SRV_CCO_ITEM *pstCcoItem = HI_NULL;
    
            pstCcoItem = mrsSrvQueueTop(&pstCcoQueue->stCcoQueue);
            if (pstCcoItem)
            {
                mrsDfxRmUartTx(pstCcoItem->pData, pstCcoItem->usDataLen, HI_TRUE);
            }
        }
    
        mrsDfxLrUpDataTryCnt();
        mrsDfxLrUpDataCnt();
    /* BEGIN: Modified by fengxiaomin/00209182, 2014/3/27   问题单号:DTS2014032906596 */
    /* END:   Modified by fengxiaomin/00209182, 2014/3/27 */
        mrsSetReadMeterState(EN_MRS_SRV_CCO_READMETER_WAIT_UPDATA_ACK);   // 等待集中器确认
        //mrsUpReterMetreData();  // 上传读表数据
        bLoopFlg = HI_FALSE;      // 需要等待
        break;

    default:
        mrsReadMeterStateInit();  // 读表模块状态初始化(重新开始询问)

        // 延时MRS_SRV_CCO_TIMER_READMETER_ACTIV_VALUE时间后重新激活读表模块
        MRS_StartTimer(EN_MRS_SRV_CCO_READMETER_ACTIV_TIMER,
                            1000 * 60, HI_SYS_TIMER_ONESHOT);

        bLoopFlg = HI_FALSE;      // 需要等待
        break;
    }

    return bLoopFlg;
}

//*****************************************************************************
// 函数名称: mrsInquireReadMeter
// 功能描述: 询问是否抄表
//
// 参数说明:
//  This function has no arguments.
//
// 返 回 值:
//    无
// 调用要求:
// 调用举例:
// 作    者: liuxipeng/KF54842 [2011-12-16]
// 修    改: cuiate/00233580   [2013-04-01]
//*****************************************************************************
HI_VOID mrsInquireReadMeter(HI_VOID)
{
    HI_U32 ret = HI_ERR_SUCCESS;

    MRS_CCO_SRV_CTX_STRU* pstCcoCtx = mrsCcoGetContext(); // 定义指向全局上下文结构的指针
    MRS_ARCHIVES_NODE_STRU *pstNode = HI_NULL;
    HI_U16 usLen = 0;
    HI_PBYTE pucOutBuf = HI_NULL;
    HI_U16 usCurrSeq = 0;

    if (pstCcoCtx->stReadMeterModel.stCurrNode.bValid == HI_FALSE)
    {
        // 延时MRS_SRV_CCO_TIMER_READMETER_ACTIV_VALUE时间后重新激活读表模块
        MRS_StartTimer(EN_MRS_SRV_CCO_READMETER_ACTIV_TIMER, 
                            MRS_SRV_CCO_TIMER_READMETER_ACTIV_VALUE, HI_SYS_TIMER_ONESHOT);
        return;
    } 

    pstNode = mrsSrvArchivesQuery(pstCcoCtx->stReadMeterModel.usCurrNodeSeq);
    if (pstNode == HI_NULL || pstNode->ucRead)
    {
        // 激活读表模块
        mrsAcivReadMeter();

        return;
    } 
    
    // 组织AFN14-F1上行帧
    do
    {
        PROTO376_2_FRAME_AFN_AND_DATA_STRU stAfnData;
        HI_U8 ucBuf[9] = { 0 };

        ucBuf[usLen++] = 0x00; // 未知相位
        (hi_void)memcpy_s(&ucBuf[usLen], sizeof(ucBuf) - usLen, pstCcoCtx->stReadMeterModel.stCurrNode.aucNodeAddr, HI_METER_ADDR_LEN);
        usLen += HI_METER_ADDR_LEN;
        usCurrSeq = pstCcoCtx->stReadMeterModel.usCurrNodeSeq;
        if (MRS_CCO_SEQ_START_FROM_1 == pstCcoCtx->ucSeqControl)
        {
            usCurrSeq++;
        }
        
        (hi_void)memcpy_s(&ucBuf[usLen], sizeof(ucBuf) - usLen, &usCurrSeq, sizeof(HI_U16));
        usLen += 2;

        (hi_void)memset_s(&stAfnData, sizeof(stAfnData), 0, sizeof(stAfnData));
        stAfnData.ucAFN = MRS_AFN(0x14);
        stAfnData.ucDT[0] = MRS_AFN_FN_LO(1);
        stAfnData.ucDT[1] = MRS_AFN_FN_HI(1);
        stAfnData.pData = ucBuf;
        stAfnData.usDataLen = usLen;

        usLen = 0;
        ret = mrsCreate3762UpFrame(&stAfnData, &pucOutBuf, &usLen, pstCcoCtx->stReadMeterModel.stCurrNode.aucNodeAddr, HI_NULL);
        if (ret != HI_ERR_SUCCESS)
        {
            break;
        }

        ret = MRS_SendMrData(pucOutBuf, usLen, HI_DMS_CHL_UART_PORT_APP); // 发送询问读表帧
    } while (0);

    mrsToolsFree(pucOutBuf);

    if (ret == HI_ERR_SUCCESS)
    {
        pstCcoCtx->stReadMeterModel.ucCurrNodeInquireTries++;
        MRS_StartTimer(EN_MRS_SRV_CCO_MUSTER_TIMER, 
                            mrsToolsGetTimeOut(pstCcoCtx->stReadMeterModel.usAfn14F1UpTimeOut), 
                            HI_SYS_TIMER_ONESHOT); // 启动定时器
        mrsSetReadMeterState(EN_MRS_SRV_CCO_READMETER_WAIT_ASK_REPLY);    // 置为等待询问应答数据
        mrsDfxLrReqCnt();
    }
    else
    {
        mrsReadMeterStateInit();  // 抄表模块错误处理

        MRS_StartTimer(EN_MRS_SRV_CCO_READMETER_ACTIV_TIMER, 
                            1000 * 60, HI_SYS_TIMER_ONESHOT);
    }

    return;
}

//*****************************************************************************
// 函数名称: mrsAssembleReadMeterData
// 功能描述:组织PLC读表数据帧
//           
// 参数说明:
//   无
// 返 回 值: 
//    无
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: liuxipeng/KF54842 [2011-12-16]
//*****************************************************************************
HI_VOID mrsAssembleReadMeterData(HI_VOID)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    MRS_CCO_SRV_CTX_STRU* cco_ctx = mrsCcoGetContext(); // 定义指向全局上下文结构的指针
    AFN_14_F1_DL_PARAM * dl_param = HI_NULL;
    AFN_14_F1_DL * dl_data = HI_NULL;
    MRS_PLC_FRAME_DATA_STRU *plc_frame_data = HI_NULL;
    MRS_DATA_TRANSMIT_AFN14_DL_STRU *transmit_data = HI_NULL;
    MRS_ARCHIVES_NODE_STRU *pstNode = HI_NULL;

    if (cco_ctx->stReadMeterModel.stCurrNode.bValid == HI_FALSE)
    {
        // 延时MRS_SRV_CCO_TIMER_READMETER_ACTIV_VALUE时间后重新激活读表模块
        MRS_StartTimer(EN_MRS_SRV_CCO_READMETER_ACTIV_TIMER,
                            MRS_SRV_CCO_TIMER_READMETER_ACTIV_VALUE,HI_SYS_TIMER_ONESHOT);
        return;
    }

    pstNode = mrsSrvArchivesQuery(cco_ctx->stReadMeterModel.usCurrNodeSeq);
    if (pstNode == HI_NULL || pstNode->ucRead)
    {
        // 延时MRS_SRV_CCO_TIMER_READMETER_ACTIV_VALUE时间后重新激活读表模块
        MRS_StartTimer(EN_MRS_SRV_CCO_READMETER_ACTIV_TIMER,
                            MRS_SRV_CCO_TIMER_READMETER_ACTIV_VALUE,HI_SYS_TIMER_ONESHOT);
        return;
    } 

    dl_param = cco_ctx->stReadMeterModel.pstAfn14F1;  // 获取AFN14-F1参数
    if(dl_param == HI_NULL)
    {
        mrsReadMeterStateInit();  // 抄表模块错误处理

        // 延时MRS_SRV_CCO_TIMER_READMETER_ACTIV_VALUE时间后重新激活读表模块
        MRS_StartTimer(EN_MRS_SRV_CCO_READMETER_ACTIV_TIMER,
                            1000 * 60, HI_SYS_TIMER_ONESHOT);
        return;
    }    

    dl_data = dl_param->pstAFN_14_F1_DL;    // 获取AFN01-F1数据内容

    if(dl_data->ucReadFlg == MRS_ALLOW_READ_CCO) // 可以抄读
    {
        HI_U16 transmit_len = 0;
        HI_U8 mac[HI_PLC_MAC_ADDR_LEN] = {0};
        HI_U8 aucMeter[HI_METER_ADDR_LEN] = {0};
        HI_U32 meter_time = 0;

        mrsMeterAddrPreProc(aucMeter, cco_ctx->stReadMeterModel.stCurrNode.aucNodeAddr, cco_ctx->stReadMeterModel.stCurrNode.ucProtocol);
        
        // 获取电表地址对应的节点MAC地址
        ret = mrsMapGetMAC(aucMeter, mac);
        if(ret == HI_ERR_SUCCESS)
        {
            HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1000, HI_DIAG_MT("MAP-MAC"), mac, HI_PLC_MAC_ADDR_LEN);
        }
        else
        {
            (hi_void)memset_s(mac, HI_PLC_MAC_ADDR_LEN, 0xFF, HI_PLC_MAC_ADDR_LEN);   // 没有找到对应的MAC地址(使用广播MAC)
        }

        // 初始化超时及重试次数    
        mrsGetMaxTryAfn14(mac);
        mrsGetPlcTimeOutAfn14(mac, HI_FALSE, &meter_time);
        
        // 分配内存
        transmit_len = sizeof(MRS_DATA_TRANSMIT_AFN14_DL_STRU) + dl_data->ucDataLen;        
        transmit_data = (MRS_DATA_TRANSMIT_AFN14_DL_STRU *)mrsToolsMalloc(transmit_len);
        if(!transmit_data)
        {
            return;
        }
        else
        {
            (hi_void)memset_s(transmit_data, transmit_len, 0, transmit_len);
        }

        // PLC帧序号自增
        cco_ctx->stReadMeterModel.seq++;
        
        // 组织数据转发包
        transmit_data->interface_ver = MRS_AFN14_DL_VER_R1;
        transmit_data->stru_size     = sizeof(MRS_DATA_TRANSMIT_AFN14_DL_STRU);
        transmit_data->protocol      = pstNode->ucProtocol;
        transmit_data->data_len      = dl_data->ucDataLen; // 获得转发数据的长度
        transmit_data->seq           = cco_ctx->stReadMeterModel.seq;
        transmit_data->time_out      = (HI_U8)MRS_MS_TO_100MS(meter_time);
        (hi_void)memcpy_s(transmit_data->data, transmit_data->data_len, dl_data->pucData, dl_data->ucDataLen); // 将要转发的内容拷贝到转发数据区

        plc_frame_data = mrsToolsMalloc(sizeof(MRS_PLC_FRAME_DATA_STRU));
        if(!plc_frame_data)
        {
            mrsToolsFree(transmit_data);
            return;
        }
        else
        {
            (hi_void)memset_s(plc_frame_data, sizeof(MRS_PLC_FRAME_DATA_STRU), 0, sizeof(MRS_PLC_FRAME_DATA_STRU));
        }
        
        plc_frame_data->usId         = ID_MRS_CMD_DATA_TRANSMIT_AFN14_PLC;
        (hi_void)memcpy_s(plc_frame_data->ucMac, HI_PLC_MAC_ADDR_LEN, mac, HI_PLC_MAC_ADDR_LEN);
        plc_frame_data->usPayloadLen = transmit_len;              // PLC帧数据长度
        plc_frame_data->pucPayload   = (HI_PBYTE)transmit_data; // PLC帧内容
        
        cco_ctx->stReadMeterModel.pstPlcDataDl = plc_frame_data; 

    #if defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG)
        //mrsLogFrameInfStatTx(dl_data->pucData, dl_data->ucDataLen, (HI_U8)(mrsToolsBroadCastMac(mac)));//optimized by weichao
    #endif
    }
    else    // 抄读失败或抄读成功
    {
        if(dl_data->ucReadFlg == MRS_READ_FAILED_CCO)
        {
            HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_014, HI_DIAG_MT("LR readmeter fail"));
        }
        else
        {
            pstNode->ucRead = MRS_READ_METER_FLAG_BIT;
            cco_ctx->stReadMeterModel.usSuccNum++;
        }

        cco_ctx->stReadMeterModel.usCurrNodeSeq++; // (改为此句) 移动到下一节点
        ret = mrsReadMeterFindValidNode(cco_ctx);  // 查找有效节点
        
        mrsReadMeterStateInit();  // 抄表模块初始化

        if (ret == HI_ERR_SUCCESS)
        {
            // 激活读表模块
            mrsAcivReadMeter();
        }
    }

    return;
}

//*****************************************************************************
// 函数名称: mrsTransmit14F1FramePlc
// 功能描述: 转发PLC读表数据
//           
// 参数说明:
//   无
// 返 回 值: 
//    无
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: liuxipeng/KF54842 [2011-12-16]
//*****************************************************************************
HI_VOID mrsTransmit14F1FramePlc(HI_VOID)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    MRS_CCO_SRV_CTX_STRU* pstCcoCtx = mrsCcoGetContext(); // 定义指向全局上下文结构的指针
    MRS_PLC_FRAME_DATA_STRU *pstPlcFrameData = pstCcoCtx->stReadMeterModel.pstPlcDataDl;
    PLC_OVERTIME_INF_STRU* TimeOutInf = mrsGetPlcOverTimeInfAfn14();

    if (pstPlcFrameData == HI_NULL)
    {
        return;
    }

    ret = MRS_SendPlcFrame(pstPlcFrameData);

    {//统计环路测试和正常抄表帧时需要用到PLC帧中的payload,当发送失败时,该内存会被释放,因此统计信息放在判断返回值之前。
        MRS_DATA_TRANSMIT_AFN14_DL_STRU *pstAfn14 = (MRS_DATA_TRANSMIT_AFN14_DL_STRU*)(pstPlcFrameData->pucPayload);
        mrsDfxRmPlcTx((HI_U8*)pstAfn14 + sizeof(MRS_DATA_TRANSMIT_AFN14_DL_STRU), pstAfn14->data_len, ret);
    }

    if(ret == HI_ERR_SUCCESS)
    {
        MRS_StartTimer(EN_MRS_SRV_CCO_STA_TIMER, (HI_U32)TimeOutInf->OverTime, HI_SYS_TIMER_ONESHOT); // 启动定时器
        mrsSetReadMeterState(EN_MRS_SRV_CCO_READMETER_WAIT_PLC_REPLY);  // 等待PLC应答状态

        mrsDfxLrPlcTx();
    }
    else
    {
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_017, HI_DIAG_MT("tx LR plc fail"));

        mrsReadMeterStateInit();  // 抄表模块错误处理

        // 延时MRS_SRV_CCO_TIMER_READMETER_ACTIV_VALUE时间后重新激活读表模块
        MRS_StartTimer(EN_MRS_SRV_CCO_READMETER_ACTIV_TIMER,
                            1000 * 60, HI_SYS_TIMER_ONESHOT);
    }

   return;
}

//*****************************************************************************
// 函数名称: mrsUpReterMetreData
// 功能描述: 上报读表数据
//
// 参数说明:
//    无
//
// 返 回 值:
//    无
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: liuxipeng/KF54842 [2011-12-17]
// 修    改: cuiate/00233580   [2013-04-01]
//*****************************************************************************
HI_VOID mrsUpReterMetreData(HI_VOID)
{
    HI_U32 ret = HI_ERR_SUCCESS;

    MRS_CCO_SRV_CTX_STRU* pstCcoCtx = mrsCcoGetContext(); // 定义指向全局上下文结构的指针
    UP_READMETER_DATA * pstUpReadMeter = HI_NULL;
    HI_U8 *pucBuf = HI_NULL;
    HI_U16 usDataLen = 0;

    PROTO376_2_FRAME_AFN_AND_DATA_STRU stAfnData;
    HI_U8 *pucOutBuf = HI_NULL;
    HI_U16 usBufLen = 0;
    HI_U16 offset = 0;
    HI_U16 usCurrSeq = 0;

    MRS_SRV_CCO_ITEM *pstCcoItem = HI_NULL;


    pstUpReadMeter = pstCcoCtx->stReadMeterModel.pstUpReadMeterData;  // 获取"上报抄读数据"
    if (pstUpReadMeter == HI_NULL)
    {
        mrsReadMeterStateInit();  // 抄表模块错误处理

        // 延时MRS_SRV_CCO_TIMER_READMETER_ACTIV_VALUE时间后重新激活读表模块
        MRS_StartTimer(EN_MRS_SRV_CCO_READMETER_ACTIV_TIMER,
                            1000 * 60, HI_SYS_TIMER_ONESHOT);

        return;
    }

    // 4 = 从节点序号2 + 规约类型1 + 数据长度1
    // 当前报文本地通信上行时长(2)
    usDataLen = pstUpReadMeter->usDataLen + 4;
    if (pstCcoCtx->ucProtoVer != PROTO_VER_3762_2009)
    {
        usDataLen += 2;
    }

    pucBuf = mrsToolsMalloc(usDataLen);
    if (!pucBuf)
    {
        return;
    }

    usCurrSeq = pstCcoCtx->stReadMeterModel.usCurrNodeSeq;
    if (MRS_CCO_SEQ_START_FROM_1 == pstCcoCtx->ucSeqControl)
    {
        usCurrSeq++;
    }
    
    // 从节点序号
    (hi_void)memcpy_s(pucBuf + offset, usDataLen - offset, &usCurrSeq, 2);
    offset += 2;

    // 规约类型
    pucBuf[offset++] = pstCcoCtx->stReadMeterModel.stCurrNode.ucProtocol;

    if (pstCcoCtx->ucProtoVer != PROTO_VER_3762_2009)
    {
        pucBuf[offset++] = (MRS_AFN_COMM_DELAY % 256);
        pucBuf[offset++] = (MRS_AFN_COMM_DELAY / 256);
    }

    // 报文长度
    pucBuf[offset++] = (HI_U8)((pstUpReadMeter->usDataLen > 255) ? 255 : pstUpReadMeter->usDataLen);
    (hi_void)memcpy_s(pucBuf + offset, usDataLen - offset, pstUpReadMeter->ucData, pstUpReadMeter->usDataLen);

    (hi_void)memset_s(&stAfnData, sizeof(stAfnData), 0, sizeof(stAfnData));
    stAfnData.ucAFN = MRS_AFN(0x06); // AFN = 06H 主动上报类
    stAfnData.ucDT[0] = MRS_AFN_FN_LO(2);
    stAfnData.ucDT[1] = MRS_AFN_FN_HI(2);
    stAfnData.pData = pucBuf;
    stAfnData.usDataLen = usDataLen;
    stAfnData.ucPhaseFlag = HI_TRUE;

    ret = mrsCreate3762UpFrame(&stAfnData, &pucOutBuf, &usBufLen, pstCcoCtx->stReadMeterModel.stCurrNode.aucNodeAddr, pstCcoCtx->stReadMeterModel.aucMacAdd);

    if (ret != HI_ERR_SUCCESS)
    {
        mrsToolsFree(pucBuf);
        mrsToolsFree(pucOutBuf);
        return;
    }

    pstCcoItem = (MRS_SRV_CCO_ITEM *)mrsToolsMalloc(sizeof(MRS_SRV_CCO_ITEM) + usBufLen);
    if (!pstCcoItem)
    {
        mrsToolsFree(pucBuf);
        mrsToolsFree(pucOutBuf);
        mrsDfxCcoQueueMallocFailCnt();
        return;
    }

    (hi_void)memset_s(pstCcoItem, sizeof(MRS_SRV_CCO_ITEM) + usBufLen, 0, sizeof(MRS_SRV_CCO_ITEM) + usBufLen);
    pstCcoItem->bValid = HI_TRUE;
    pstCcoItem->ucAfn = 0x06;
    pstCcoItem->usFn = 0x02;
    pstCcoItem->usTimeOut = (HI_U16)mrsToolsGetTimeOut(pstCcoCtx->stReadMeterModel.usAfn06F2UpTimeOut);//单位为ms
    pstCcoItem->ucMaxTry = pstCcoCtx->stReadMeterModel.ucAfn06F2UpRetryMax;
    pstCcoItem->usDataLen = usBufLen;
    (hi_void)memcpy_s(pstCcoItem->pData, pstCcoItem->usDataLen, pucOutBuf, usBufLen);
    pstCcoItem->MrsCcoRxRespProc = mrsReadterModelCco;
    pstCcoItem->MrsCcoRxTotalTimeOutProc = mrsReadMeterTotalMusterTimerProcess;
    pstCcoItem->MrsCcoRxEveryTimeOutProc = mrsReadMeterMusterTimerProcess;
    
    mrsCcoJoinQueue(pstCcoItem);

    mrsActiveCcoQueue();
    mrsDfxLrUpDataCnt();
    mrsDfxRmUartTx(pucOutBuf, usBufLen, HI_TRUE);
    mrsSetReadMeterState(EN_MRS_SRV_CCO_READMETER_WAIT_UPDATA_ACK);   // 等待集中器确认

    mrsToolsFree(pucBuf);
    mrsToolsFree(pucOutBuf);

    return;
}

//*****************************************************************************
// 函数名称: mrsReadMeterStateInit
// 功能描述: 读表模块状态初始化
//
// 参数说明:
//    无
// 返 回 值:
//    无
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: liuxipeng/KF54842 [2011-12-16]
//*****************************************************************************
HI_VOID mrsReadMeterStateInit(HI_VOID)
{
    MRS_CCO_SRV_CTX_STRU* pstCcoCtx = mrsCcoGetContext(); // 定义指向全局上下文结构的指针
    PLC_OVERTIME_INF_STRU* TimeOutInf = mrsGetPlcOverTimeInfAfn14();

    // 停止与抄表相关的定时器
    MRS_StopTimer(EN_MRS_SRV_CCO_MUSTER_TIMER);
    MRS_StopTimer(EN_MRS_SRV_CCO_STA_TIMER);

    mrsSetReadMeterState(EN_MRS_SRV_CCO_READMETER_IDLE);  // 当前状态置为空闲

    if (pstCcoCtx->stReadMeterModel.pstAfn14F1 != HI_NULL)
    {
        pstCcoCtx->stReadMeterModel.pstAfn14F1->MRS_AFN14_FREE_FUN(pstCcoCtx->stReadMeterModel.pstAfn14F1);   // 释放内存
    }

    pstCcoCtx->stReadMeterModel.pstAfn14F1 = HI_NULL;

    if(pstCcoCtx->stReadMeterModel.pstPlcDataDl != HI_NULL)
    {
        if(pstCcoCtx->stReadMeterModel.pstPlcDataDl->pucPayload != HI_NULL)
        {
            mrsToolsFree(pstCcoCtx->stReadMeterModel.pstPlcDataDl->pucPayload);   // 释放转发数据块   
            pstCcoCtx->stReadMeterModel.pstPlcDataDl->pucPayload = HI_NULL;
        }
        
        mrsToolsFree(pstCcoCtx->stReadMeterModel.pstPlcDataDl);   // 释放PLC帧备份    
        pstCcoCtx->stReadMeterModel.pstPlcDataDl = HI_NULL;
    }
    if (pstCcoCtx->stReadMeterModel.pstUpReadMeterData != HI_NULL)
    {
        mrsToolsFree(pstCcoCtx->stReadMeterModel.pstUpReadMeterData);   // 释放内存
    }

    pstCcoCtx->stReadMeterModel.pstUpReadMeterData = HI_NULL;
    mrsCcoQueueSetItemInvalid(0x06,0x02);

    TimeOutInf->Try = 0;
    TimeOutInf->MaxTry = TimeOutInf->MaxTryNvS;

    return;
}

//*****************************************************************************
// 函数名称: mrsReadMeterStaTimerProcess
// 功能描述: 定抄读表状态下STA通讯定时器超时处理
//           
// 参数说明:
//   pstCcoCtx [in/out] 当前状态
//
// 返 回 值: 
//    无
//
// 调用要求: 
// 调用举例: 
// 作    者: liuxipeng/KF54842 [2012-06-08]
//*****************************************************************************
HI_VOID mrsReadMeterStaTimerProcess( MRS_CCO_SRV_CTX_STRU* pstCcoCtx ) 
{
    PLC_OVERTIME_INF_STRU* TimeOutInf = mrsGetPlcOverTimeInfAfn14();
    HI_U32 num = 0;
/* BEGIN: Modified by fengxiaomin/00209182, 2013/12/11   问题单号:DTS2013122609670 */
    HI_U32 ret = HI_ERR_SUCCESS;
/* END:   Modified by fengxiaomin/00209182, 2013/12/11 */
    
    if(pstCcoCtx == HI_NULL)
    {
        return;
    }

    HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_020, HI_DIAG_MT("LR- wait plc resp timeout"));

    mrsDfxLrPlcTimeout();
/* BEGIN: Modified by fengxiaomin/00209182, 2014/3/27   问题单号:DTS2014032906596 */
    if(pstCcoCtx->stReadMeterModel.pstPlcDataDl)
    {
        mrsDfxRmPlcDlRx(ID_MRS_CMD_DATA_TRANSMIT_AFN14_PLC, pstCcoCtx->stReadMeterModel.pstPlcDataDl->pucPayload, HI_TRUE);
    }
/* END:   Modified by fengxiaomin/00209182, 2014/3/27 */
    
    if(pstCcoCtx->stReadMeterModel.enCurrState == EN_MRS_SRV_CCO_READMETER_WAIT_PLC_REPLY)
    {
        HI_U8* mac = HI_NULL;

        if(HI_NULL == pstCcoCtx->stReadMeterModel.pstPlcDataDl)
        {
            HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_021, HI_DIAG_MT("LR - recv frm when timeout"));
            return;
        }
        
        mac = pstCcoCtx->stReadMeterModel.pstPlcDataDl->ucMac;
        
        if (!mrsToolsBroadCastMac(mac))
        {
            if((TimeOutInf->Try >= TimeOutInf->MaxTry) && (TimeOutInf->MaxTryNvB))
            {                
                TimeOutInf->Try = 0;
                
                mrsGetMaxTryAfn14(HI_NULL); // 将重试次数置为广播重试次数(由非广播转广播) 
                mrsGetPlcTimeOutAfn14(mac, HI_TRUE, HI_NULL);
                
                (hi_void)memset_s(mac, HI_PLC_MAC_ADDR_LEN, 0xFF, HI_PLC_MAC_ADDR_LEN);   // 将目的地址设置为广播地址
            }
        }
        
        if(TimeOutInf->Try < TimeOutInf->MaxTry)
        {
            TimeOutInf->Try++;
            
            mrsSetReadMeterState(EN_MRS_SRV_CCO_READMETER_REPEAT_PLC); // 重发读表数据
            
            mrsReadterModelCco(HI_NULL);   // 调用入口函数处理
            
            return;
        }

        pstCcoCtx->stReadMeterModel.ulTimeOutCount++;   // 超时次数自增
        mrsDfxLrFailCnt();
#if 0//optimized by weichao
    #if defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG)
        if(pstCcoCtx->stReadMeterModel.pstPlcDataDl->pucPayload != HI_NULL)
        {
            MRS_DATA_TRANSMIT_AFN14_DL_STRU * plcDlData = HI_NULL;
            
            plcDlData = (MRS_DATA_TRANSMIT_AFN14_DL_STRU *)pstCcoCtx->stReadMeterModel.pstPlcDataDl->pucPayload;
            mrsLogPlcOverTimeStat(plcDlData->data, plcDlData->data_len, 0, HI_NULL);
        }
    #endif
#endif
    }    

    {
        MRS_ARCHIVES_NODE_STRU *pstNode = HI_NULL;

        pstNode = mrsSrvArchivesQuery(pstCcoCtx->stReadMeterModel.usCurrNodeSeq);
        
        if (pstNode != HI_NULL)
        {
            mrsCcoReadFailInsert(pstNode->aucNodeAddr, pstNode->ucProtocol);
            mrsDetectLrRmTimeout(pstNode);
            mrsDecreaseMrTTL(&pstCcoCtx->stMapModule, pstNode->aucNodeAddr);
        }
    }
    
    num = mrsGetPlcTimeOutMaxNum();
    if(num < 0xFFFFFFFF)  // 当重试次数为最大值时不进行跳转从节点操作
    {
        if(pstCcoCtx->stReadMeterModel.ulTimeOutCount >= num)
        {
            pstCcoCtx->stReadMeterModel.usCurrNodeSeq++; //(改为此句) 移动到下一节点
            ret = mrsReadMeterFindValidNode(pstCcoCtx);  //查找有效节点
        }
    }

    mrsReadMeterStateInit();    // 重新开始读表流程

    if (ret == HI_ERR_SUCCESS)
    {
        // 激活读表模块
        mrsAcivReadMeter();
    }

    return;
}

//*****************************************************************************
// 函数名称: mrsReadMeterMusterTimerProcess
// 功能描述: 定抄读表状态下集中器通讯定时器超时处理
//           
// 参数说明:
//   pstCcoCtx [in/out] 当前状态
//
// 返 回 值: 
//    无
//
// 调用要求: 
// 调用举例: 
// 作    者: liuxipeng/KF54842 [2012-06-08]
//*****************************************************************************
HI_U32 mrsReadMeterMusterTimerProcess(HI_VOID *param) 
{
    MRS_CCO_SRV_CTX_STRU* pstCcoCtx = mrsCcoGetContext(); // 定义指向全局上下文结构的指针

    MRS_NOT_USED(param);
    
    if ((pstCcoCtx->status & MRS_CCO_STATUS_NORMAL) == 0)
    {
        return HI_ERR_FAILURE;
    }

    if(pstCcoCtx->stReadMeterModel.enCurrState == EN_MRS_SRV_CCO_READMETER_WAIT_UPDATA_ACK)        // 如果为等待上传数据确认状态
    {
        mrsSetReadMeterState(EN_MRS_SRV_CCO_READMETER_REPEAT_UPDATA); // 重发上传读表数据
        mrsReadterModelCco(HI_NULL);   // 调用入口函数处理

        return HI_ERR_CONTINUE;
    }
    else
    {
        mrsReadMeterTotalMusterTimerProcess(pstCcoCtx);
    }

    return HI_ERR_SUCCESS;
}

HI_U32 mrsReadMeterTotalMusterTimerProcess(HI_VOID *param)
{
    MRS_CCO_SRV_CTX_STRU* pstCcoCtx = mrsCcoGetContext(); // 定义指向全局上下文结构的指针

    MRS_NOT_USED(param);
    
    if ((pstCcoCtx->status & MRS_CCO_STATUS_NORMAL) == 0)
    {
        return HI_ERR_FAILURE;
    }

    // 14F1询问后等待应答，超过最大重试次数后，需要切到下一个电表继续询问抄表；未超过最大次数，需要继续询问抄表
    if(pstCcoCtx->stReadMeterModel.enCurrState == EN_MRS_SRV_CCO_READMETER_WAIT_ASK_REPLY)
    {
        if(pstCcoCtx->stReadMeterModel.ucCurrNodeInquireTries < pstCcoCtx->stReadMeterModel.ucAfn14F1UpRetryMax + 1)
        {
            mrsReadMeterStateInit();    // 重新开始读表流程
            mrsReadterModelCco(HI_NULL);   // 调用入口函数处理
            return HI_ERR_SUCCESS;
        }
        pstCcoCtx->stReadMeterModel.usCurrNodeSeq++;
    }
    
    //重新开始整个抄表流程
    mrsReadMeterStateInit();    // 重新开始读表流程
        
    // 延时MRS_SRV_CCO_TIMER_READMETER_ACTIV_VALUE时间后重新激活读表模块
    MRS_StartTimer(EN_MRS_SRV_CCO_READMETER_ACTIV_TIMER, 
                        MRS_SRV_CCO_TIMER_READMETER_ACTIV_VALUE,HI_SYS_TIMER_ONESHOT);

    return HI_ERR_SUCCESS;
}

//*****************************************************************************
// 函数名称: mrsReadMeterActiveTimer
// 功能描述: 读表模块激活定时器处理
//           
// 参数说明:
//   pstCcoCtx [in/out] 当前状态
//
// 返 回 值: 
//    无
//
// 调用要求: 
// 调用举例: 
// 作    者: liuxipeng/KF54842 [2012-06-08]
//*****************************************************************************
HI_VOID mrsReadMeterActiveTimer( MRS_CCO_SRV_CTX_STRU* pstCcoCtx ) 
{
/* BEGIN: Added by fengxiaomin/00209182, 2015/9/7   问题单号号:DTS2015091503701 */
    if (0 == (pstCcoCtx->status & MRS_CCO_STATUS_NORMAL))
    {
        return;
    }
/* END:   Added by fengxiaomin/00209182, 2015/9/7 */

    if(pstCcoCtx->stReadMeterModel.enCurrState == EN_MRS_SRV_CCO_READMETER_IDLE)
    {
        pstCcoCtx->stReadMeterModel.ucCurrNodeInquireTries = 0;
        mrsReadterModelCco(HI_NULL);   // 调用入口函数处理
    }
    else
    {
        mrsReadMeterStateInit();    // 重新开始读表流程
        
        // 激活读表模块
        mrsAcivReadMeter();
    }
    
    return;
}

//*****************************************************************************
// 函数名称: mrsReadMeterFindValidNode
// 功能描述: 查找有效节点：如果查找成功，直接返回；否则将当前处理节点信息置为空
//
// 参数说明:
//   pstCcoCtx [in/out] 当前状态
//
// 返 回 值:
//   HI_ERR_SUCCESS 查找到有效节点
//   HI_ERR_NOT_FOUND 没有查找到有效节点
//   0xFFFFFFFF地址映射表错误或读取失败
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: fengxiaomin/f00209182 [2012-05-26]
//*****************************************************************************
HI_U32 mrsReadMeterFindValidNode(MRS_CCO_SRV_CTX_STRU * pstCcoCtx)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U16 CurrNodeNum = 0;

    if (pstCcoCtx == HI_NULL)
    {
        ret = HI_ERR_FAILURE;

        return ret;
    }

    CurrNodeNum = pstCcoCtx->stReadMeterModel.usCurrNodeSeq;
    if (CurrNodeNum == PRODUCT_CFG_MRS_MAX_NODE_NUM)
    {
        if(mrsGetMaxTurnNum() != 0)
        {
            mrsCurrTurnGrow();  // 当前轮次自增
        }
        pstCcoCtx->stReadMeterModel.usCurrNodeSeq = 0;
        CurrNodeNum = 0; //防止当前序号超出节点数量
    }

    do
    {
        MRS_ARCHIVES_NODE_STRU *pstNode = HI_NULL;
        
        if((mrsGetMaxTurnNum() != 0)
            && (mrsGetCurrTurnNum() >= mrsGetMaxTurnNum()))
        {
            break;
        }

        pstNode = mrsSrvArchivesQuery(CurrNodeNum);
        
        if (pstNode != HI_NULL && pstNode->bValid && !pstNode->ucRead)
        {
            pstCcoCtx->stReadMeterModel.usCurrNodeSeq = CurrNodeNum; //(改为此句)当前处理的从节点序号
            (hi_void)memcpy_s(&pstCcoCtx->stReadMeterModel.stCurrNode, sizeof(pstCcoCtx->stReadMeterModel.stCurrNode), 
                pstNode, sizeof(MRS_ARCHIVES_NODE_STRU)); //当前处理的从节点信息
            pstCcoCtx->stReadMeterModel.ulTimeOutCount = 0;   // 找到新节点初始化超时次数

            return ret;
        }

        CurrNodeNum++;

        if (CurrNodeNum == PRODUCT_CFG_MRS_MAX_NODE_NUM)
        {
            if(mrsGetMaxTurnNum() != 0)
            {
                mrsCurrTurnGrow();  // 当前轮次自增
            }
            CurrNodeNum = 0;
        }
    } while (CurrNodeNum != pstCcoCtx->stReadMeterModel.usCurrNodeSeq); //(改为此句)

    mrsSetCurrTurnNum(0);   // 当前轮次清零

    if (MRS_CCO_LR_MODE_CONTINUOUS == pstCcoCtx->ucLrModeFlag)
    {
        mrsSrvArchivesReadFlagClear();
        ret = HI_ERR_SUCCESS;
    }
    else
    {
        mrsForceSwitchPause();
        mrs3762_ReportAFN06_F3(MRS_CCO_READ_METER_TASK_END); //抄表任务结束
        ret = HI_ERR_NOT_FOUND;
    }

    (hi_void)memset_s(&pstCcoCtx->stReadMeterModel.stCurrNode, sizeof(MRS_ARCHIVES_NODE_STRU), 0, sizeof(MRS_ARCHIVES_NODE_STRU));
    pstCcoCtx->stReadMeterModel.usCurrNodeSeq = 0;

    return ret;
}

//*****************************************************************************
// 函数名称: mrsCurrNodeValid
// 功能描述: 判断抄表过程中当前节点是否有效
//
// 参数说明:
//
// 返 回 值:
//   HI_FALSE 当前节点无效
//   HI_TRUE 当前有效节点有效
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: fengxiaomin/f00209182 [2012-09-04]
//*****************************************************************************
HI_BOOL mrsCurrNodeValid(HI_VOID)
{
    MRS_CCO_SRV_CTX_STRU* pstCcoCtx = mrsCcoGetContext();
    MRS_ARCHIVES_NODE_STRU *pstNode = HI_NULL;
        
    pstNode = mrsSrvArchivesQuery(pstCcoCtx->stReadMeterModel.usCurrNodeSeq);
    if (pstNode == HI_NULL)
    {
        return HI_FALSE;
    }
    if(pstNode->ucRead)
	{
		return HI_FALSE;
	}
    if (!pstCcoCtx->stReadMeterModel.stCurrNode.bValid || !pstNode->bValid)
    {
        return HI_FALSE;
    }
    else if (!mrsToolsMemEq(pstCcoCtx->stReadMeterModel.stCurrNode.aucNodeAddr, pstNode->aucNodeAddr, HI_METER_ADDR_LEN))
    {
        return HI_FALSE;
    }

    return HI_TRUE;
}


//*****************************************************************************
// 函数名称: mrsReadMeterReceivePlcProcess
// 功能描述: 接收读表PLC数据处理
//
// 参数说明:
//          pstData 转发的数据内容
//          pstCcoCtx 全局上下文
// 返 回 值:
//          参见错误列表
//
// 调用要求: 无
// 调用举例: 无
// 作    者: liuxipeng/lKF54842 [2012-09-17]
//*****************************************************************************
HI_U32 mrsReadMeterReceivePlcProcess(MRS_DATA_TRANSMIT_AFN14_UP_STRU * transmit_data,
                                                MRS_CCO_SRV_CTX_STRU * cco_ctx, HI_U8 ucMacAdd[HI_PLC_MAC_ADDR_LEN])
{
    UP_READMETER_DATA * upload_data = HI_NULL;  //上报的数据
    HI_U16 upload_size = 0;
    HI_U8 *buffer = HI_NULL;
    MRS_ARCHIVES_NODE_STRU *pstNode = HI_NULL;

    //当前应该是等待数据应答状态
    if(cco_ctx->stReadMeterModel.enCurrState != EN_MRS_SRV_CCO_READMETER_WAIT_PLC_REPLY)
    {   
        mrsDfxLrPlcExceedCnt();
        mrsDfxRmPlcUpRx(ID_MRS_CMD_DATA_TRANSMIT_AFN14_PLC, (HI_U8*)transmit_data, HI_TRUE);
        return HI_ERR_FAILURE;
    }

    if (transmit_data->seq != cco_ctx->stReadMeterModel.seq)
    {   
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_031, HI_DIAG_MT("recv plc seq err, abandon"));
        mrsDfxLrPlcSeqErr();    // 统计丢弃的PLC帧个数
        mrsDfxRmPlcUpRx(ID_MRS_CMD_DATA_TRANSMIT_AFN14_PLC, (HI_U8*)transmit_data, HI_TRUE);
        return HI_ERR_FAILURE;
    }
    
    //如果使用者没有释放上报数据内存，选择释放
    if(cco_ctx->stReadMeterModel.pstUpReadMeterData != HI_NULL)
    {
        upload_data = cco_ctx->stReadMeterModel.pstUpReadMeterData;
        mrsToolsFree(upload_data);
    }

    pstNode = mrsSrvArchivesQuery(cco_ctx->stReadMeterModel.usCurrNodeSeq);
    buffer = (HI_U8 *)transmit_data + transmit_data->stru_size;

    if (mrsCcoIsTestMode() && pstNode != HI_NULL)
    {
        HI_U16 usPos = 0;
        HI_U16 usFrameLen = 0;

        if ((HI_ERR_SUCCESS != mrsFind645Frame(buffer, (HI_S16)transmit_data->data_len, &usPos, &usFrameLen))
            || (!mrsMeterAddrMatch(pstNode->aucNodeAddr, buffer + usPos + MRS_645_FRAME_METERADD_OFFSET)))
        {
            HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_013, HI_DIAG_MT("[LR]测试模式，地址不匹配"));
            return HI_ERR_FAILURE;
        }
    }

    //将探测抄表中表档案中抄读成功表的探测标志清除
    if (HI_NULL != pstNode)
    {
        mrsDetectRmRespProc(pstNode->aucNodeAddr);
    }
    
    //准备上传数据
    upload_size = sizeof(UP_READMETER_DATA) + transmit_data->data_len;
    upload_data = (UP_READMETER_DATA *)mrsToolsMalloc(upload_size);
    cco_ctx->stReadMeterModel.pstUpReadMeterData = upload_data;

    if(upload_data != HI_NULL)
    {
        (hi_void)memset_s(upload_data, upload_size, 0, upload_size);
        upload_data->usDataLen = transmit_data->data_len;
        (hi_void)memcpy_s(upload_data->ucData, upload_data->usDataLen, buffer, transmit_data->data_len);
    }
    else
    {
        mrsReadMeterStateInit();
 
        MRS_StartTimer(EN_MRS_SRV_CCO_READMETER_ACTIV_TIMER,
                            1000 * 60,
                            HI_SYS_TIMER_ONESHOT);
        
        return HI_ERR_NOT_ENOUGH_MEMORY;
    } 

    if (HI_NULL != pstNode)
    {
        mrsCcoReadFailRemove(pstNode->aucNodeAddr, pstNode->ucProtocol);
    }
#if 0//optimized by weichao
#if defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG)
    if(cco_ctx->stReadMeterModel.pstPlcDataDl != HI_NULL)
    {
        MRS_DATA_TRANSMIT_AFN14_DL_STRU * plcDlData = HI_NULL;
        
        plcDlData = (MRS_DATA_TRANSMIT_AFN14_DL_STRU *)cco_ctx->stReadMeterModel.pstPlcDataDl->pucPayload;
        if(plcDlData != HI_NULL)
        {
            mrsLogFrameInfStatRx(buffer, transmit_data->data_len, plcDlData->data, plcDlData->data_len, 1);
        }
    }
#endif
#endif
    if (ucMacAdd == HI_NULL)
    {
        (hi_void)memset_s(cco_ctx->stReadMeterModel.aucMacAdd, HI_PLC_MAC_ADDR_LEN, 0, HI_PLC_MAC_ADDR_LEN);
    }
    else
    {
        (hi_void)memcpy_s(cco_ctx->stReadMeterModel.aucMacAdd, HI_PLC_MAC_ADDR_LEN,ucMacAdd, HI_PLC_MAC_ADDR_LEN);
    }
    
    mrsReadterModelCco(HI_NULL);

    return HI_ERR_SUCCESS;
}

//*****************************************************************************
// 函数名称: mrsReadMeterResume
// 功能描述: 恢复轮抄抄表
//
// 参数说明:
//          无
// 返 回 值:
//          无
//
// 调用要求: 无
// 调用举例: 无
// 作    者: fengxiaomin/00209182 [2015-09-07]
//*****************************************************************************
HI_VOID mrsReadMeterResume(HI_VOID)
{
    HI_U32 status = mrsCcoGetState();
    MRS_CCO_SRV_CTX_STRU *pstCcoCtx = mrsCcoGetContext();

    if (HI_TRUE == pstCcoCtx->bPauseFlag)
    {
        return;
    }
    
    status |= MRS_CCO_STATUS_NORMAL;
    mrsCcoSetState(status);
    
    mrsReadMeterStateInit();    // 定时抄表模块初始化
    // 延时MRS_SRV_CCO_TIMER_READMETER_ACTIV_VALUE时间后重新激活读表模块
    MRS_StartTimer(EN_MRS_SRV_CCO_READMETER_ACTIV_TIMER,
                        MRS_SRV_CCO_TIMER_READMETER_ACTIV_VALUE,HI_SYS_TIMER_ONESHOT);
}
HI_VOID mrsReadWaterMeterStateInit(HI_VOID)
{
    MRS_CCO_SRV_CTX_STRU* pstCcoCtx = mrsCcoGetContext(); // 定义指向全局上下文结构的指针
    //PLC_OVERTIME_INF_STRU* TimeOutInf = mrsGetPlcOverTimeInfAfn14();//weichao
	HI_U16 usMeterAllNum = mrsSrvArchivesMeterNum();
	MRS_ARCHIVES_NODE_STRU *pstNode = HI_NULL;
	HI_U32 i=0;
	
	pstCcoCtx->stReadWaterMeterModel.enCurrRfState =1;

    if(pstCcoCtx->stReadWaterMeterModel.pstRfPlcDataDl != HI_NULL)
    {
        if(pstCcoCtx->stReadWaterMeterModel.pstRfPlcDataDl->pucPayload != HI_NULL)
        {
            mrsToolsFree(pstCcoCtx->stReadWaterMeterModel.pstRfPlcDataDl->pucPayload);   // 释放转发数据块   
            pstCcoCtx->stReadWaterMeterModel.pstRfPlcDataDl->pucPayload = HI_NULL;
        }
        
        mrsToolsFree(pstCcoCtx->stReadWaterMeterModel.pstRfPlcDataDl);   // 释放PLC帧备份    
        pstCcoCtx->stReadWaterMeterModel.pstRfPlcDataDl = HI_NULL;
    }
    if (pstCcoCtx->stReadWaterMeterModel.pstRfUpReadMeterData != HI_NULL)
    {
        mrsToolsFree(pstCcoCtx->stReadWaterMeterModel.pstRfUpReadMeterData);   // 释放内存
    }

    pstCcoCtx->stReadWaterMeterModel.pstRfUpReadMeterData = HI_NULL;

    pstCcoCtx->stReadWaterMeterModel.ucCurrNodeInquireTries=0;
	 pstCcoCtx->stReadWaterMeterModel.usPLCTimeOut = 8000;

	pstCcoCtx->stReadWaterMeterModel.usAfn20F3UpTimeOut = 3000;
	pstCcoCtx->stReadWaterMeterModel.ucAfn20F3UpRetryMax =2;
	pstCcoCtx->stReadWaterMeterModel.ulTimeOutMaxNumNv =2;

   
	//在此处找到当前电表信息中第一个有效的表信息（序号）
    for (i = 0; i < usMeterAllNum; i++)
    {
        pstNode = mrsSrvArchivesQuery(i);
        if (pstNode == HI_NULL)
        {
            break;
        }

        if (pstNode->bValid && (pstNode->ucProtocol>=METER_PROTO_SINGLEWAY_WATERMETER)) // 查找到一个有效的表信息
        {
            pstNode->ucRfReportFlag=0;
        }
    }
    return;
}
HI_U32 mrsMapGetWaterMeterMAC(HI_U8 *pucDeviceAddr, HI_U8 *pucMacAddr)
{
    MRS_CCO_SRV_CTX_STRU *pCco = mrsCcoGetContext();
    HI_U32 ulRet = HI_ERR_SUCCESS;
	HI_U16 i=0;

	for(i=0;i<pCco->stWaterMeterSearchModel.usWaterMeterResultNum;i++)
	{
		if(0 ==mrsToolsMemcmp(pCco->stWaterMeterSearchModel.stWaterMeterResult[i].ucaWaterMac,pucDeviceAddr,6))
		{
			mrsToolsMemcpy_s(pucMacAddr, HI_PLC_MAC_ADDR_LEN, pCco->stWaterMeterSearchModel.stWaterMeterResult[i].ucaStaMac, HI_PLC_MAC_ADDR_LEN);
			return ulRet;
		}
	}
    return HI_ERR_NOT_FOUND;
}

HI_U32 mrsReadWaterMeterEncode(HI_VOID)
{
    //HI_U32 ret = HI_ERR_SUCCESS;
    MRS_CCO_SRV_CTX_STRU* pstCcoCtx = mrsCcoGetContext(); // 定义指向全局上下文结构的指针
	MRS_PLC_FRAME_DATA_STRU *plc=HI_NULL;
	MRS_PLC_METERLIST_DL_S * meterlist = HI_NULL;
	HI_U8 * payload = HI_NULL;
	HI_U16 payload_len = sizeof(MRS_PLC_METERLIST_DL_S)+4;
	HI_U8 aucIp[HI_IPV4_ADDR_LEN];
	HI_U16 usOffent=0;

	if(pstCcoCtx->stTopoInf.entry[pstCcoCtx->stReadWaterMeterModel.usCurrRfNodeSeq].tei ==1)
	{
		pstCcoCtx->stReadWaterMeterModel.usCurrRfNodeSeq++;
	}
	
	HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_025, HI_DIAG_MT("ReadWaterMeterEncode usCurrRfNodeSeq,All"),pstCcoCtx->stReadWaterMeterModel.usCurrRfNodeSeq,pstCcoCtx->stTopoInf.num);
    if (pstCcoCtx->stReadWaterMeterModel.usCurrRfNodeSeq >= pstCcoCtx->stTopoInf.num)   //没有电表
    {
        pstCcoCtx->stReadWaterMeterModel.usCurrRfNodeSeq = 0;  //(改为这样)
        pstCcoCtx->stReadWaterMeterModel.ulTimeOutCount = 0;   // 找到新节点初始化超时次数
        mrs3762_ReportAFN20_F4();
		RfFreePLCData(pstCcoCtx->stReadWaterMeterModel.pstRfPlcDataDl);
        return HI_ERR_NOT_FOUND;
    } 
   
    // 获取电表地址对应的节点MAC地址
    // 初始化超时及重试次数    
    // 分配内存
    payload = (HI_U8 *)mrsToolsMalloc(payload_len);
    if (HI_NULL == payload)
    {
        return HI_ERR_MALLOC_FAILUE;
    }
	plc = (MRS_PLC_FRAME_DATA_STRU *)mrsToolsMalloc(sizeof(MRS_PLC_FRAME_DATA_STRU));
	if (HI_NULL == plc)
    {
    	mrsToolsFree(payload);
        return HI_ERR_MALLOC_FAILUE;
    }

    // PLC帧序号自增
    pstCcoCtx->stReadWaterMeterModel.usRfPlcSeq++;
	
    mrsToolsMemZero_s(payload, payload_len, payload_len);
    meterlist = (MRS_PLC_METERLIST_DL_S *)payload;
    meterlist->interface_ver = MRS_GET_METERLIST_DL_VER;
    meterlist->stru_len = sizeof(MRS_PLC_METERLIST_DL_S);
    meterlist->option = MRS_SM_WATER_CMD;
    meterlist->seq = pstCcoCtx->stReadWaterMeterModel.usRfPlcSeq;

   	meterlist->waterMeter_Data[usOffent++]=3;
	meterlist->waterMeter_Data[usOffent++]=0;
	meterlist->waterMeter_Data[usOffent++]=0;
	meterlist->waterMeter_Data[usOffent++]=48;
    (HI_VOID)HI_MDM_NM_GetAddr(meterlist->src, aucIp);
    mrsToolsMemcpy_s(meterlist->dst, HI_PLC_MAC_ADDR_LEN, pstCcoCtx->stTopoInf.entry[pstCcoCtx->stReadWaterMeterModel.usCurrRfNodeSeq].mac, HI_PLC_MAC_ADDR_LEN);

    mrsToolsMemZero_s(plc, sizeof(plc), sizeof(plc));
    plc->usId = ID_MRS_CMD_GET_METERLIST;
    plc->pucPayload = payload;
    plc->usPayloadLen = payload_len;
    mrsToolsMemcpy_s(plc->ucMac, HI_PLC_MAC_ADDR_LEN, meterlist->dst, HI_PLC_MAC_ADDR_LEN);
	
    // 组织数据转发包
    if(pstCcoCtx->stReadWaterMeterModel.pstRfPlcDataDl != HI_NULL)
    {
        if(pstCcoCtx->stReadWaterMeterModel.pstRfPlcDataDl->pucPayload != HI_NULL)
        {
            mrsToolsFree(pstCcoCtx->stReadWaterMeterModel.pstRfPlcDataDl->pucPayload);   // 释放转发数据块   
            pstCcoCtx->stReadWaterMeterModel.pstRfPlcDataDl->pucPayload = HI_NULL;
        }
        
        mrsToolsFree(pstCcoCtx->stReadWaterMeterModel.pstRfPlcDataDl);   // 释放PLC帧备份    
        pstCcoCtx->stReadWaterMeterModel.pstRfPlcDataDl = HI_NULL;
    }
    pstCcoCtx->stReadWaterMeterModel.pstRfPlcDataDl = plc;
    HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_027, HI_DIAG_MT("ReadWaterMeterEncode end"));
    return HI_ERR_SUCCESS;

}
HI_VOID mrsRfTransmitFramePlcData(HI_VOID)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    MRS_CCO_SRV_CTX_STRU* pstCcoCtx = mrsCcoGetContext(); // 定义指向全局上下文结构的指针
    MRS_PLC_FRAME_DATA_STRU *pstPlcFrameData = pstCcoCtx->stReadWaterMeterModel.pstRfPlcDataDl;
    //PLC_OVERTIME_INF_STRU* TimeOutInf = mrsGetPlcOverTimeInfAfn14();//weichao

    if (pstPlcFrameData == HI_NULL)
    {
        return;
    }

    ret = MRS_SendPlcFrame(pstPlcFrameData);
    if(ret == HI_ERR_SUCCESS)
    {
        MRS_StartTimer(MRS_CCO_TIMER_RF_READ_WATERMETER, pstCcoCtx->stReadWaterMeterModel.usPLCTimeOut, HI_SYS_TIMER_ONESHOT); // 启动定时器
        pstCcoCtx->stReadWaterMeterModel.enCurrRfState=2; // 等待PLC应答状态
    }
    else
    {
        mrsReadWaterMeterStateInit();  // 抄表模块错误处理

        // 延时MRS_SRV_CCO_TIMER_READMETER_ACTIV_VALUE时间后重新激活读表模块
        MRS_StartTimer(MRS_CCO_TIMER_RF_READ_WATERMETER,1000 * 60, HI_SYS_TIMER_ONESHOT);
    }
	HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_028, HI_DIAG_MT("mrsRfTransmitFramePlcData"),ret);

   return;
}

HI_VOID mrsReadWaterMeterActiveTimer( MRS_CCO_SRV_CTX_STRU* pstCcoCtx ) 
{
	HI_U32 ret=HI_ERR_SUCCESS;
    if (0 == (pstCcoCtx->status & MRS_CCO_STATUS_NORMAL))
    {
		//HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_090, HI_DIAG_MT("ReadWaterMeterActiveTimer NOT MRS_CCO_STATUS_NORMAL"),pstCcoCtx->status);
        //return;
    }
	HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_023, HI_DIAG_MT("ReadWaterMeterActiveTimer enCurrRfState"),pstCcoCtx->stReadWaterMeterModel.enCurrRfState);

	switch(pstCcoCtx->stReadWaterMeterModel.enCurrRfState)
	{
		case 1:
			ret = mrsReadWaterMeterEncode();
			if(ret==HI_ERR_SUCCESS)
			{
				mrsRfTransmitFramePlcData();
			}
			break;
		case 2:
			pstCcoCtx->stReadWaterMeterModel.ulTimeOutCount++;
			if(pstCcoCtx->stReadWaterMeterModel.ulTimeOutCount> pstCcoCtx->stReadWaterMeterModel.ulTimeOutMaxNumNv)
			{
				pstCcoCtx->stReadWaterMeterModel.ulTimeOutCount=0;
				pstCcoCtx->stReadWaterMeterModel.enCurrRfState =1;
				pstCcoCtx->stReadWaterMeterModel.usCurrRfNodeSeq++;
				
				HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_024, HI_DIAG_MT("ReadWaterMeterActiveTimer 超时下一块"));
				MRS_StartTimer(MRS_CCO_TIMER_RF_READ_WATERMETER, 100, HI_SYS_TIMER_ONESHOT); // 启动定时器
				break;
			}
			mrsRfTransmitFramePlcData();
			break;
		case 3:
			pstCcoCtx->stReadWaterMeterModel.usCurrRfNodeSeq++;
			pstCcoCtx->stReadWaterMeterModel.ulTimeOutCount=0;
			pstCcoCtx->stReadWaterMeterModel.enCurrRfState =1;
			MRS_StartTimer(MRS_CCO_TIMER_RF_READ_WATERMETER, 100, HI_SYS_TIMER_ONESHOT); // 启动定时器
			break;
		case 0:
			mrsReadWaterMeterStateInit();    // 重新开始读表流程
        	pstCcoCtx->stReadWaterMeterModel.enCurrRfState =1;
			break;
	}
	
    return;
}

HI_VOID mrsWaterMeterModelRxRespProc(HI_VOID * pAckParam)
{
	MRS_CCO_SRV_CTX_STRU* pstCcoCtx = mrsCcoGetContext();
	
	HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_042, HI_DIAG_MT("MeterModelRxRespProc"));
    mrsReadWaterMeterActiveTimer(pstCcoCtx);
}
HI_U32 mrsWaterMeterModelAllTimeOutProc(HI_VOID * pAckParam)
{
	HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_043, HI_DIAG_MT("ModelAllTimeOutProc"));
    return mrs3762_ReportAFN20_F4();
}
#endif

