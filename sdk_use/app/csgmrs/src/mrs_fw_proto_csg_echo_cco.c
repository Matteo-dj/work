//*****************************************************************************
//
//                  版权所有 (C), 2001-2014, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_fw_proto_csg_echo_cco.c
//  版 本 号   : V1.0
//  作    者   : cuiate/c00233580
//  生成日期   : 2015-12-25
//  功能描述   : TODO: ...
//
//  函数列表   : TODO: ...
//  修改历史   :
//  1.日    期 : 2015-12-25
//    作    者 : cuiate/c00233580
//    修改内容 : 创建文件
//
//*****************************************************************************

#include "mrs_common.h"
#include "mrs_fw_n.h"
#include "mrs_fw_proto_csg_echo.h"
#include "mrs_fw_proto_csg.h"
#include "mrs_fw_tools.h"
#include "mrs_fw_proto376_2_echo.h"
#include "mrs_fw_log.h"
#include "mrs_srv_res.h"
#include "mrs_srv_cco_queue.h"
#include "mrs_srv_cco_macmap.h"
#include "mrs_srv_cco_broadcast.h"
#include "mrs_srv_list.h"
#include "mrs_srv_cco_event.h"
#include "mrs_srv_cco_remote_upg.h"
#include "mrs_srv_cco.h"
#include "mrs_srv_cco_searchmeter.h"
#include "mrs_srv_cco_archives.h"
#include "mrs_srv_cco_remote_upg.h"
#include "mrs_dfx_cco_remote_upg.h"
#include "mrs_srv_cco_event.h"
#include "mrs_dfx_cco.h"
#include "mrs_time.h"
#include "mrs_srv_cco_event.h"
#include "mrs_srv_mrslog.h"
#include "mrs_srv_queue.h"
#include "mrs_srv_cco_mr_task.h"
#include "mrs_fw_proto_csg_echo_cco.h"
#include "mrs_srv_cco_detect_rm.h"


HI_START_HEADER

#if (defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) && defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD))
//*****************************************************************************
//*****************************************************************************

HI_PRV MRS_CSG_AFN_FUNC_MAP g_astCcoCsgAfnFuncMap[] =
{
    {0xE8010001, mrsCsg_AFN00_E8010001},    // 确认
    {0xE8010002, mrsCsg_AFN00_E8010002},    // 否认

    {0xE8020101, mrsCsg_AFN01_E8020101},    // 复位硬件
    {0xE8020102, mrsCsg_AFN01_E8020102},    // 初始化档案
    {0xE8020103, mrsCsg_AFN01_E8020103},    // 初始化任务

    {0xE8020201, mrsCsg_AFN02_E8020201},    // 添加任务
    {0xE8020202, mrsCsg_AFN02_E8020202},    // 删除任务
    {0xE8000203, mrsCsg_AFN02_E8000203},    // 查询未完成任务数
    {0xE8030204, mrsCsg_AFN02_E8030204},    // 查询未完成任务列表
    {0xE8030205, mrsCsg_AFN02_E8030205},    // 查询未完成任务详细信息
    {0xE8000206, mrsCsg_AFN02_E8000206},    // 查询剩余可分配任务数
    {0xE8020207, mrsCsg_AFN02_E8020207},    // 添加多播任务
    {0xE8020208, mrsCsg_AFN02_E8020208},    // 启动任务
    {0xE8020209, mrsCsg_AFN02_E8020209},    // 暂停任务

    {0xE8000301, mrsCsg_AFN03_E8000301},    // 查询厂家代码和版本信息
    {0xE8000302, mrsCsg_AFN03_E8000302},    // 查询本地通讯模块运行模式信息
    {0xE8000303, mrsCsg_AFN03_E8000303},    // 查询主节点地址
    {0xE8030304, mrsCsg_AFN03_E8030304},    // 查询通讯延迟时长
    {0xE8000305, mrsCsg_AFN03_E8000305},    // 查询从节点个数
    {0xE8030306, mrsCsg_AFN03_E8030306},    // 查询从节点信息
    {0xE8000307, mrsCsg_AFN03_E8000307},    // 查询从节点主动注册进度
    {0xE8030308, mrsCsg_AFN03_E8030308},    // 查询从节点的父节点

    {0xE8020401, mrsCsg_AFN04_E8020401},    // 设置主节点地址
    {0xE8020402, mrsCsg_AFN04_E8020402},    // 添加从节点
    {0xE8020403, mrsCsg_AFN04_E8020403},    // 删除从节点
    {0xE8020404, mrsCsg_AFN04_E8020404},    // 允许/禁止上报从节点事件
    {0xE8020405, mrsCsg_AFN04_E8020405},    // 激活从节点主动注册
    {0xE8020406, mrsCsg_AFN04_E8020406},    // 终止从节点主动注册

    {0xE8060601, mrsCsg_AFN06_E8060601},    // 请求集中器时间

    {0xE8020701, mrsCsg_AFN07_E8020701},    // 启动文件传输
    {0xE8020702, mrsCsg_AFN07_E8020702},    // 传输文件内容
    {0xE8000703, mrsCsg_AFN07_E8000703},    // 查询文件信息
    {0xE8000704, mrsCsg_AFN07_E8000704},    // 查询文件进度
    {0xE8030705, mrsCsg_AFN07_E8030705},    //查询文件传输失败节点

    {0xE803F001, mrsCsg_AFNF0_E803F001},    // PLC灵敏度测试
    {0xE803F002, mrsCsg_AFNF0_E803F002},    // 过零检测
};


HI_U32 mrsCcoProtoCsgEchoInit(HI_VOID)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;

    ulRet = mrsProtoCsgEchoInit();
    if (HI_ERR_SUCCESS == ulRet)
    {
        mrsProtoCsgEchoRegFuncMap(g_astCcoCsgAfnFuncMap, MRS_TOOLS_ALEN(g_astCcoCsgAfnFuncMap));
    }

    return ulRet;
}


HI_U32 mrsCcoProtoCsgEchoDeinit(HI_VOID)
{
    return mrsProtoCsgEchoDeinit();
}

HI_VOID mrsCsgQueueFree(HI_VOID * p)
{
    mrsToolsFree(p);
}


HI_VOID mrsCsgListFree(HI_VOID * p, HI_VOID *pParam)
{
    HI_UNREF_PARAM(pParam);
    mrsCsgQueueFree(p);
}


// AFN00 E8010001: 确认
HI_U32 mrsCsg_AFN00_E8010001(HI_INOUT HI_VOID * pParam)
{
    P_MRS_SRV_CCO_QUEUE pstCcoQueue = mrsGetCcoQueue();
    MRS_SRV_CCO_ITEM *pstCcoItem = HI_NULL;
    MRS_SRV_CCO_ITEM *pstCcoItemTemp = HI_NULL;

    MRS_NOT_USED(pParam);
    MRS_StopTimer(MRS_CCO_TIMER_MASTER_REPORT);

    if (MRS_CCO_QUEUE_WAIT_ACK_STATUS != mrsCcoGetQueueStatus())
    {
        return HI_ERR_SUCCESS;
    }

    pstCcoItem = mrsSrvQueueTop(&pstCcoQueue->stCcoQueue);
    if (!pstCcoItem)
    {
        return HI_ERR_FAILURE;
    }

    mrsDfxCcoQueueRxAckCnt();

    if (pstCcoItem->MrsCcoRxRespProc)
    {
        pstCcoItem->MrsCcoRxRespProc(pstCcoItem->pParam);
    }

    pstCcoItemTemp = mrsSrvQueueTop(&pstCcoQueue->stCcoQueue);
    if (pstCcoItem == pstCcoItemTemp)
    {
        mrsCcoQueueEnd(pstCcoQueue);
    }

    return HI_ERR_SUCCESS;
}


// AFN00 F2:否认
HI_U32 mrsCsg_AFN00_E8010002(HI_INOUT HI_VOID * pParam)
{
    P_MRS_SRV_CCO_QUEUE pstCcoQueue = mrsGetCcoQueue();
    MRS_SRV_CCO_ITEM *pstCcoItem = HI_NULL;
    MRS_SRV_CCO_ITEM *pstCcoItemTemp = HI_NULL;

    MRS_NOT_USED(pParam);
    MRS_StopTimer(MRS_CCO_TIMER_MASTER_REPORT);

    if (MRS_CCO_QUEUE_WAIT_ACK_STATUS != mrsCcoGetQueueStatus())
    {
        return HI_ERR_SUCCESS;
    }

    pstCcoItem = mrsSrvQueueTop(&pstCcoQueue->stCcoQueue);
    if (!pstCcoItem)
    {
        return HI_ERR_FAILURE;
    }

    mrsDfxCcoQueueRxDenyCnt();

    if (pstCcoItem->MrsCcoRxRespProc)
    {
        pstCcoItem->MrsCcoRxRespProc(pstCcoItem->pParam);
    }

    pstCcoItemTemp = mrsSrvQueueTop(&pstCcoQueue->stCcoQueue);
    if (pstCcoItem == pstCcoItemTemp)
    {
        mrsCcoQueueEnd(pstCcoQueue);
    }    

    return HI_ERR_SUCCESS;
}


// AFN01 F1:硬件初始化
HI_U32 mrsCsg_AFN01_E8020101(HI_INOUT HI_VOID * pParam)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    AFN_DI_UL_PARAM * pstAfnParam = (AFN_DI_UL_PARAM *)pParam;
    static HI_U8 ucFirst = 0;

    HI_U8 ucDeny = MRS_CSG_DENY_COMMUNICATION_OVERTIME;//否认帧中的错误状态字

    if (!mrsIsAllowCcoReset())
    {
        ucFirst = 0;
        ucDeny = MSG_CSG_DENY_CCO_BUSY;
        ulRet = mrsCsgProtoDenyFrame(&ucDeny, pstAfnParam);
        return ulRet;
    }

    if (ucFirst)
    {
        ulRet = mrsCsgProtoAckFrame(1,pstAfnParam);
        return ulRet;
    }

    ucFirst++;

    //组织确认帧
    ulRet = mrsCsgProtoAckFrame(1,pstAfnParam);

    if (ulRet == HI_ERR_SUCCESS)
    {
        MRS_StartTimer(EN_MRS_SRV_CCO_SYSTEM_RESET_TIMER, MRS_SRV_CCO_SYSTEM_RESET_TIMER_VALUE, HI_SYS_TIMER_ONESHOT);
    }
    else
    {
        ucFirst = 0;
    }

    return ulRet;
}


// AFN01 F2:初始化表档案
HI_U32 mrsCsg_AFN01_E8020102(HI_INOUT HI_VOID * pParam)
{
    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();
    AFN_DI_UL_PARAM * pstAfnParam = (AFN_DI_UL_PARAM *)pParam;

    mrsSrvArchivesClear();//清空表档案

    //回应确认帧
    mrsCsgProtoAckFrame(0,pstAfnParam);

    (hi_void)memset_s(&pstCcoCtx->stReadMeterModel.stCurrNode, sizeof(MRS_ARCHIVES_NODE_STRU), 0, sizeof(MRS_ARCHIVES_NODE_STRU));
    pstCcoCtx->stReadMeterModel.usCurrNodeSeq = 0;
    pstCcoCtx->stReadMeterModel.usSuccNum = 0;

    mrsCcoReadFailReset();

    MRS_StartTimer(EN_MRS_SRV_CCO_WRITE_FLASH_ACTIV_TIMER, pstCcoCtx->ucFlashSaveDelay *MS_PER_SECOND, HI_SYS_TIMER_ONESHOT);
    mrsCcoSmWhitelistSwitchTimerReset();

    mrsCcoDetectRmReset();
    mrsDfxCcoArchivesClearTime(HI_MDM_GetMilliSeconds());
    mrsDfxLrMeterNum(0);
    mrsSetArchivesStatus(MRS_CCO_ARCHIVES_STATUS_WAITING);

    return HI_ERR_SUCCESS;
}


// AFN01 F3:初始化任务
HI_U32 mrsCsg_AFN01_E8020103(HI_INOUT HI_VOID * pParam)
{
    AFN_DI_UL_PARAM *pstAfnParam = (AFN_DI_UL_PARAM *)pParam;
    mrsCcoMrTaskDeleteAll();
    return mrsCsgProtoAckFrame(0, pstAfnParam);
}


// 添加任务
HI_U32 mrsCsg_AFN02_E8020201(HI_VOID *pParam)
{
    AFN_DI_UL_PARAM *pstAfnParam = (AFN_DI_UL_PARAM *)pParam;
    MRS_CSG_FRAME_STRU *pstFrame = pstAfnParam->pstFrame;
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U8 ucDeny = MRS_CSG_DENY_COMMUNICATION_OVERTIME;

    mrsRemoteUpgRxFilePro();
    
    ulRet = mrsCcoMrTaskInsertUnicast(pstFrame, &ucDeny);
    mrsCcoDfxRmUartRx(pstFrame->aucAppData, pstFrame->usAppDataLen, HI_FALSE);
    if (HI_ERR_SUCCESS == ulRet)
    {
        ulRet = mrsCsgProtoAckFrame(0, pstAfnParam);
        if (HI_ERR_SUCCESS == ulRet)
        {
            mrsDfxRmUartTx((HI_U8 *)(*pstAfnParam->ppucOutBuffer), *pstAfnParam->pusBufferLen, HI_FALSE);
        }
    }
    else
    {
        ulRet = mrsCsgProtoDenyFrame(&ucDeny, pstAfnParam);
        if (HI_ERR_SUCCESS == ulRet)
        {
            mrsDfxRmUartDenyTx((HI_U8 *)(*pstAfnParam->ppucOutBuffer), *pstAfnParam->pusBufferLen);
        }
    }

    return ulRet;
}


// 删除任务
HI_U32 mrsCsg_AFN02_E8020202(HI_VOID *pParam)
{
    AFN_DI_UL_PARAM *pstAfnParam = (AFN_DI_UL_PARAM *)pParam;
    MRS_CSG_FRAME_STRU *pstFrame = pstAfnParam->pstFrame;
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U8 ucDeny = MRS_CSG_DENY_COMMUNICATION_OVERTIME;

    mrsCcoDfxRmUartRx(pstFrame->aucAppData, pstFrame->usAppDataLen, HI_FALSE);
    if (sizeof(HI_U16) == pstFrame->usAppDataLen)
    {
        HI_U16 usId = MRS_COMBIN16(pstFrame->aucAppData[0], pstFrame->aucAppData[1]);

        if (usId > MRS_CCO_MR_TASK_ID_MAX)
        {
            ucDeny = MSG_CSG_DENY_TASK_ID_NOT_EXIST;
            ulRet = mrsCsgProtoDenyFrame(&ucDeny, pstAfnParam);
            if (HI_ERR_SUCCESS == ulRet)
            {
                mrsDfxRmUartDenyTx((HI_U8 *)(*pstAfnParam->ppucOutBuffer), *pstAfnParam->pusBufferLen);
            }

            return ulRet;
         }

        ulRet = mrsCcoMrTaskDelete(usId);
        if (HI_ERR_SUCCESS == ulRet)
        {
            ulRet = mrsCsgProtoAckFrame(0, pstAfnParam);
            if (HI_ERR_SUCCESS == ulRet)
            {
                mrsDfxRmUartTx((HI_U8 *)(*pstAfnParam->ppucOutBuffer), *pstAfnParam->pusBufferLen, HI_FALSE);
            }

            return ulRet;
        }

        ucDeny = MSG_CSG_DENY_TASK_NOT_EXIST;
    }
    else
    {
        ucDeny = MRS_CSG_DENY_ERROR_LENGTH;
    }

    ulRet = mrsCsgProtoDenyFrame(&ucDeny, pstAfnParam);
    if (HI_ERR_SUCCESS == ulRet)
    {
        mrsDfxRmUartDenyTx((HI_U8 *)(*pstAfnParam->ppucOutBuffer), *pstAfnParam->pusBufferLen);
    }

    return ulRet;
}


// 查询未完成任务
HI_U32 mrsCsg_AFN02_E8000203(HI_VOID *pParam)
{
    AFN_DI_UL_PARAM *pstAfnParam = (AFN_DI_UL_PARAM *)pParam;
    MRS_CSG_FRAME_STRU *pstFrame = pstAfnParam->pstFrame;
    MRS_CSG_IND_FRAME_STRU stIndFrame = {0};
    HI_U16 usTaskRemain = mrsCcoMrTaskNum();
    HI_U32 ulRet = HI_ERR_SUCCESS;

    mrsCcoDfxRmUartRx(pstFrame->aucAppData, pstFrame->usAppDataLen, HI_FALSE);
    (hi_void)memset_s(&stIndFrame, sizeof(stIndFrame), 0, sizeof(stIndFrame));
    stIndFrame.ucAfn = pstFrame->ucAfn;
    stIndFrame.ulDI = pstFrame->ulDI;
    stIndFrame.ucSeq = pstFrame->ucSeq;
    stIndFrame.ucPrm = 0;
    stIndFrame.ucAddrFlag = 0;

    stIndFrame.pAppData = (HI_U8 *)&usTaskRemain;
    stIndFrame.usAppDataLen = sizeof(usTaskRemain);

    ulRet = mrsCreateCsgFrame(&stIndFrame, pstAfnParam->ppucOutBuffer, pstAfnParam->pusBufferLen);
    if (HI_ERR_SUCCESS == ulRet)
    {
        mrsDfxRmUartTx((HI_U8 *)(*pstAfnParam->ppucOutBuffer), *pstAfnParam->pusBufferLen, HI_FALSE);
    }

    return ulRet;
}


// 查询未完成任务列表
HI_U32 mrsCsg_AFN02_E8030204(HI_VOID *pParam)
{
    AFN_DI_UL_PARAM *pstAfnParam = (AFN_DI_UL_PARAM *)pParam;
    MRS_CSG_FRAME_STRU *pstFrame = pstAfnParam->pstFrame;
    MRS_CSG_IND_FRAME_STRU stIndFrame = {0};
    HI_U16 usStart = 0;
    HI_U32 ulRet = HI_ERR_SUCCESS;

    mrsCcoDfxRmUartRx(pstFrame->aucAppData, pstFrame->usAppDataLen, HI_FALSE);
    if (3 != pstFrame->usAppDataLen)
    {
        return HI_ERR_BAD_DATA;
    }

    usStart = MRS_COMBIN16(pstFrame->aucAppData[0], pstFrame->aucAppData[1]);

    if (pstFrame->aucAppData[2] == 0)
    {
        return HI_ERR_FAILURE;
    }

    (hi_void)memset_s(&stIndFrame, sizeof(stIndFrame), 0, sizeof(stIndFrame));
    stIndFrame.ucAfn = pstFrame->ucAfn;
    stIndFrame.ulDI = MRS_CSG_DI_IND_UNTASK_LIST;
    stIndFrame.ucSeq = pstFrame->ucSeq;
    stIndFrame.ucPrm = 0;
    stIndFrame.ucAddrFlag = 0;

    ulRet = mrsCcoMrTaskGetIdBuffer(usStart, pstFrame->aucAppData[2], &stIndFrame.pAppData, &stIndFrame.usAppDataLen);
    if (HI_ERR_SUCCESS != ulRet)
    {
        return HI_ERR_FAILURE;
    }

    ulRet = mrsCreateCsgFrame(&stIndFrame, pstAfnParam->ppucOutBuffer, pstAfnParam->pusBufferLen);
    if (HI_ERR_SUCCESS == ulRet)
    {
        mrsDfxRmUartTx((HI_U8 *)(*pstAfnParam->ppucOutBuffer), *pstAfnParam->pusBufferLen, HI_FALSE);
    }

    mrsToolsFree(stIndFrame.pAppData);

    return ulRet;
}


// 查询未完成任务详细信息
HI_U32 mrsCsg_AFN02_E8030205(HI_VOID *pParam)
{
    AFN_DI_UL_PARAM *pstAfnParam = (AFN_DI_UL_PARAM *)pParam;
    MRS_CSG_FRAME_STRU *pstFrame = pstAfnParam->pstFrame;
    MRS_CSG_IND_FRAME_STRU stIndFrame = {0};
    HI_U16 usId = 0;
    HI_U32 ulRet = HI_ERR_SUCCESS;

    mrsCcoDfxRmUartRx(pstFrame->aucAppData, pstFrame->usAppDataLen, HI_FALSE);
    if (sizeof(HI_U16) != pstFrame->usAppDataLen)
    {
        return HI_ERR_BAD_DATA;
    }

    (hi_void)memset_s(&stIndFrame, sizeof(stIndFrame), 0, sizeof(stIndFrame));
    stIndFrame.ucAfn = pstFrame->ucAfn;
    stIndFrame.ulDI = MRS_CSG_DI_IND_UNTASK_INFO;
    stIndFrame.ucSeq = pstFrame->ucSeq;
    stIndFrame.ucPrm = 0;
    stIndFrame.ucAddrFlag = 0;

    usId = MRS_COMBIN16(pstFrame->aucAppData[0], pstFrame->aucAppData[1]);
    ulRet = mrsCcoMrTaskGetTaskInfo(usId, &stIndFrame.pAppData, &stIndFrame.usAppDataLen);
    if (HI_ERR_SUCCESS != ulRet)
    {
        HI_U8 ucDeny = MSG_CSG_DENY_TASK_ID_NOT_EXIST;
        return mrsCsgProtoDenyFrame(&ucDeny, pstAfnParam);
    }

    ulRet = mrsCreateCsgFrame(&stIndFrame, pstAfnParam->ppucOutBuffer, pstAfnParam->pusBufferLen);
    if (HI_ERR_SUCCESS == ulRet)
    {
        mrsDfxRmUartTx((HI_U8 *)(*pstAfnParam->ppucOutBuffer), *pstAfnParam->pusBufferLen, HI_FALSE);
    }

    mrsToolsFree(stIndFrame.pAppData);

    return ulRet;
}


// 查询剩余可分配任务数
HI_U32 mrsCsg_AFN02_E8000206(HI_VOID *pParam)
{
    AFN_DI_UL_PARAM *pstAfnParam = (AFN_DI_UL_PARAM *)pParam;
    MRS_CSG_FRAME_STRU *pstFrame = pstAfnParam->pstFrame;
    MRS_CSG_IND_FRAME_STRU stIndFrame = {0};
    HI_U16 usTaskRemain = mrsCcoMrTaskRemain();
    HI_U32 ulRet = HI_ERR_SUCCESS;

    mrsCcoDfxRmUartRx(pstFrame->aucAppData, pstFrame->usAppDataLen, HI_FALSE);
    (hi_void)memset_s(&stIndFrame, sizeof(stIndFrame), 0, sizeof(stIndFrame));
    stIndFrame.ucAfn = pstFrame->ucAfn;
    stIndFrame.ulDI = pstFrame->ulDI;
    stIndFrame.ucSeq = pstFrame->ucSeq;
    stIndFrame.ucPrm = 0;
    stIndFrame.ucAddrFlag = 0;

    stIndFrame.pAppData = (HI_U8 *)&usTaskRemain;
    stIndFrame.usAppDataLen = sizeof(usTaskRemain);

    ulRet = mrsCreateCsgFrame(&stIndFrame, pstAfnParam->ppucOutBuffer, pstAfnParam->pusBufferLen);
    if (HI_ERR_SUCCESS == ulRet)
    {
        mrsDfxRmUartTx((HI_U8 *)(*pstAfnParam->ppucOutBuffer), *pstAfnParam->pusBufferLen, HI_FALSE);
    }

    return ulRet;
}


// 添加多播任务
HI_U32 mrsCsg_AFN02_E8020207(HI_VOID *pParam)
{
    AFN_DI_UL_PARAM *pstAfnParam = (AFN_DI_UL_PARAM *)pParam;
    MRS_CSG_FRAME_STRU *pstFrame = pstAfnParam->pstFrame;
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U8 ucDeny = MRS_CSG_DENY_COMMUNICATION_OVERTIME;

    mrsCcoDfxRmUartRx(pstFrame->aucAppData, pstFrame->usAppDataLen, HI_FALSE);
    ulRet = mrsCcoMrTaskInsertMulticast(pstFrame, &ucDeny);
    if (HI_ERR_SUCCESS == ulRet)
    {
        ulRet = mrsCsgProtoAckFrame(0, pstAfnParam);
        if (HI_ERR_SUCCESS == ulRet)
        {
            mrsDfxRmUartTx((HI_U8 *)(*pstAfnParam->ppucOutBuffer), *(pstAfnParam->pusBufferLen), HI_FALSE);
        }
    }
    else
    {
        ulRet = mrsCsgProtoDenyFrame(&ucDeny, pstAfnParam);
        if (HI_ERR_SUCCESS == ulRet)
        {
            mrsDfxRmUartDenyTx((HI_U8 *)(*pstAfnParam->ppucOutBuffer), *pstAfnParam->pusBufferLen);
        }
    }

    return ulRet;
}


// 启动任务
HI_U32 mrsCsg_AFN02_E8020208(HI_INOUT HI_VOID *pParam)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    AFN_DI_UL_PARAM *pstAfnParam = (AFN_DI_UL_PARAM *)pParam;
    MRS_CSG_FRAME_STRU *pstFrame = pstAfnParam->pstFrame;

    mrsCcoMrTaskSetStatus(MRS_CCO_MR_TASK_ENABLE);
    mrsCcoDfxRmUartRx(pstFrame->aucAppData, pstFrame->usAppDataLen, HI_FALSE);

    ulRet = mrsCsgProtoAckFrame(0, pstAfnParam);
    if (HI_ERR_SUCCESS == ulRet)
    {
        mrsDfxRmUartTx((HI_U8 *)(*pstAfnParam->ppucOutBuffer), *pstAfnParam->pusBufferLen, HI_FALSE);
    }

    return ulRet;
}


// 暂停任务
HI_U32 mrsCsg_AFN02_E8020209(HI_INOUT HI_VOID *pParam)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    AFN_DI_UL_PARAM *pstAfnParam = (AFN_DI_UL_PARAM *)pParam;
    MRS_CSG_FRAME_STRU *pstFrame = pstAfnParam->pstFrame;

    mrsCcoMrTaskSetStatus(MRS_CCO_MR_TASK_PAUSE);
    mrsCcoDfxRmUartRx(pstFrame->aucAppData, pstFrame->usAppDataLen, HI_FALSE);

    ulRet = mrsCsgProtoAckFrame(10, pstAfnParam);
    if (HI_ERR_SUCCESS == ulRet)
    {
        mrsDfxRmUartTx((HI_U8 *)(*pstAfnParam->ppucOutBuffer), *pstAfnParam->pusBufferLen, HI_FALSE);
    }

    return ulRet;
}


HI_U32 makeBuf_AFN03_E8000301(HI_OUT HI_U8 * pucBuf, HI_INOUT HI_U16 * pusBufLen)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    AFN_CSG_MANU_INFO_STRU stManuInf;

    if ((!pucBuf) || (!pusBufLen) || (*pusBufLen < sizeof(stManuInf)))
    {
        return HI_ERR_FAILURE;
    }

    (hi_void)memset_s(&stManuInf, sizeof(stManuInf), 0, sizeof(stManuInf));

    ulRet = mrsCsgProtoManuInfo(&stManuInf);
    if (HI_ERR_SUCCESS == ulRet)
    {
        (hi_void)memcpy_s(pucBuf, sizeof(stManuInf), &stManuInf, sizeof(stManuInf));
        *pusBufLen = sizeof(stManuInf);
    }

    return ulRet;
}


// AFN03 F3:查询厂家代码和版本号
HI_U32 mrsCsg_AFN03_E8000301(HI_INOUT HI_VOID * pParam)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U8 aucBuf[MRS_CSG_COCODE_VER_IND_LEN] = {0};
    HI_U16 usBufLen = MRS_CSG_COCODE_VER_IND_LEN;

    ulRet = makeBuf_AFN03_E8000301(aucBuf, &usBufLen);
    if (ulRet == HI_ERR_SUCCESS)
    {
        ulRet = MRS_ProtoCsgEncode(aucBuf, usBufLen, pParam);
    }

    return ulRet;
}


HI_U32 makeBuf_AFN03_E8000302(HI_OUT HI_U8 ** ppucOutBuf, HI_OUT HI_U16 * pusOutLen)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;

    do
    {
        HI_U8 * pucBuf = HI_NULL;
        HI_U16 usBufLen = MRS_CSG_COMMUNICATION_UP_DATA_LEN;
        HI_U16 usTemp = 0;
        HI_U16 usOffset = 0;
        HI_U16 usTmpLen = 0;

        MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();

        pucBuf = mrsToolsMalloc(usBufLen);
        if (pucBuf == HI_NULL)
        {
            ulRet = HI_ERR_MALLOC_FAILUE;
            break;
        }

        (hi_void)memset_s(pucBuf, usBufLen, 0, usBufLen);

        pucBuf[usOffset++] = 0x02;  // D0~D3 : 2  (宽带电力线载波通讯) 4表示”窄带+微功率无线”，5表示”宽带+微功率无线
                                    // D4~D7 : 0  预留

        // 最大支持的376.2报文长度：可以正确接收的376.2报文最大长度
        usTemp = MRS_MAX_FRAME_LEN;
        usOffset = MRS_CSG_FRAME_MAX_LENGTH_OFFSET;
        (hi_void)memcpy_s(pucBuf + usOffset, usBufLen - usOffset, &usTemp, sizeof(usTemp));

        // 文件传输支持的最大单个数据包长度：在文件传输协议"中支持的最大分包大小
        usTemp = MRS_MAX_FRAME_LEN;
        usOffset = MRS_CSG_MAX_PACKET_LENGTH_OFFSET;
        (hi_void)memcpy_s(pucBuf + usOffset, usBufLen - usOffset, &usTemp, sizeof(usTemp));

        // 升级操作等待时间：终端发送完最后一个升级数据包后，需要等待模块完成升级的时间长度。
        usOffset = MRS_CSG_UPG_WAIT_TIME_OFFSET;
        pucBuf[usOffset] = MRS_UPG_WAIT_TIME;

        // 主节点地址：本地通信模块的主节点地址。
        usOffset = MRS_CSG_CCO_ADDR_OFFSET;
        (hi_void)memcpy_s(pucBuf + usOffset, usBufLen - usOffset, pstCcoCtx->ucMainNodeAddr, sizeof(pstCcoCtx->ucMainNodeAddr));
        mrsHexInvert(pucBuf + usOffset, sizeof(pstCcoCtx->ucMainNodeAddr));

        // 支持的最大从节点数量：本模块（带路由管理）支持的最大从节点下装数量
        usTemp = PRODUCT_CFG_MRS_MAX_NODE_NUM;
        usOffset = MRS_CSG_STATION_MAX_NUM_OFFSET;
        (hi_void)memcpy_s(pucBuf + usOffset, usBufLen - usOffset, &usTemp, sizeof(usTemp));

        // 当前从节点数量：本模块（带路由管理）当前下装的从节点数量
        usTemp = mrsSrvArchivesMeterNum(); // 表档案节点数
        usOffset = MRS_CSG_STATION_NUM_OFFSET;
        (hi_void)memcpy_s(pucBuf + usOffset, usBufLen - usOffset, &usTemp, sizeof(usTemp));

        //支持单次读写从节点信息的最大数量
        usTemp = MRS_3762_F10_NODE_MAX_NUM;
        usOffset = MRS_CSG_NODE_MAX_NUM_OFFSET;
        (hi_void)memcpy_s(pucBuf + usOffset, usBufLen - usOffset, &usTemp, sizeof(usTemp));

        // 通信模块使用的376.2协议发布日期： BCD编码，YYMMDD日期格式
        usOffset = MRS_CSG_PROTO_RELEASE_OFFSET;
        (hi_void)memcpy_s(pucBuf + usOffset, usBufLen - usOffset, MRS_CSG_PROTO_RELEASE_DATE, 3);

        usOffset = MRS_CSG_DEVICE_INFO_OFFSET;
        usTmpLen = usBufLen - usOffset;
        ulRet = makeBuf_AFN03_E8000301(pucBuf + usOffset, &usTmpLen);//厂商代码和版本信息
        if (ulRet == HI_ERR_SUCCESS)
        {
            *ppucOutBuf = pucBuf;
            *pusOutLen = usBufLen;
        }
        else
        {
            mrsToolsFree(pucBuf);
        }
    } while (0);

    return ulRet;
}


// AFN03 DIE8000302:查询本地通信模块运行模式信息
HI_U32 mrsCsg_AFN03_E8000302(HI_INOUT HI_VOID * pParam)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U8 *pucBuf = HI_NULL;
    HI_U16 usBufLen = 0;

    ulRet = makeBuf_AFN03_E8000302(&pucBuf, &usBufLen);
    if (ulRet == HI_ERR_SUCCESS)
    {
        ulRet = MRS_ProtoCsgEncode(pucBuf, usBufLen, pParam);
        mrsToolsFree(pucBuf);
    }

    return ulRet;
}


// 主动上报“本地通信模块运行模式信息”
HI_U32 mrsCsg_AFN03_E8000302_UP(HI_VOID)
{
    MRS_CCO_SRV_CTX_STRU *pstCcoCtx = mrsCcoGetContext();
    MRS_CSG_IND_FRAME_STRU stIndFrame;
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U8 *pFrame = HI_NULL;
    HI_U16 usFrameLen = 0;
    MRS_SRV_CCO_ITEM *pstCcoItem = HI_NULL;

    do
    {
        (hi_void)memset_s(&stIndFrame, sizeof(stIndFrame), 0, sizeof(stIndFrame));
        stIndFrame.ucPrm = 1;
        stIndFrame.ucAddrFlag = 0;
        stIndFrame.ucAfn = MRS_CSG_AFN_REQUEST_03H;
        stIndFrame.ulDI = MRS_CSG_DI_IND_RUN_MODE_INF;
        stIndFrame.ucSeq = ++(pstCcoCtx->ucSeq);

        ulRet = makeBuf_AFN03_E8000302(&stIndFrame.pAppData, &stIndFrame.usAppDataLen);
        if (ulRet != HI_ERR_SUCCESS)
        {
            break;
        }

        ulRet = mrsCreateCsgFrame(&stIndFrame, &pFrame, &usFrameLen);
        if (HI_ERR_SUCCESS != ulRet)
        {
            break;
        }

        pstCcoItem = (MRS_SRV_CCO_ITEM *)mrsToolsMalloc(sizeof(MRS_SRV_CCO_ITEM) + usFrameLen);
        if (!pstCcoItem)
        {
            ulRet = HI_ERR_NOT_ENOUGH_MEMORY;
            break;
        }

        (hi_void)memset_s(pstCcoItem, sizeof(MRS_SRV_CCO_ITEM) + usFrameLen, 0, sizeof(MRS_SRV_CCO_ITEM) + usFrameLen);
        pstCcoItem->bValid = HI_TRUE;
        pstCcoItem->ucAfn = MRS_CSG_AFN_REQUEST_03H;
        pstCcoItem->usFn = (HI_U16)(MRS_CSG_DI_IND_RUN_MODE_INF & 0xFFFF);
        pstCcoItem->usTimeOut = 1500;
        pstCcoItem->ucMaxTry = 0;
        pstCcoItem->usDataLen = usFrameLen;
        (hi_void)memcpy_s(pstCcoItem->pData, usFrameLen, pFrame, usFrameLen);

        mrsCcoJoinQueue(pstCcoItem);

        mrsActiveCcoQueue();
    } while (0);

    mrsToolsFree(pFrame);
    mrsToolsFree(stIndFrame.pAppData);

    return ulRet;
}


//E8 00 03 03：查询主节点地址
HI_U32 mrsCsg_AFN03_E8000303(HI_INOUT HI_VOID * pParam)
{
    HI_U8 ucNodeAddress[HI_METER_ADDR_LEN] = {0};
    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();

    //查询载波主节点地址
    (hi_void)memcpy_s(ucNodeAddress, sizeof(ucNodeAddress), pstCcoCtx->ucMainNodeAddr, sizeof(pstCcoCtx->ucMainNodeAddr));
    mrsHexInvert(ucNodeAddress, sizeof(ucNodeAddress));

    return MRS_ProtoCsgEncode(ucNodeAddress, sizeof(ucNodeAddress), pParam);
}


//E8 03 03 04：查询通讯延迟时间
HI_U32 mrsCsg_AFN03_E8030304(HI_INOUT HI_VOID * pParam)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U8 * pucContent = HI_NULL;
    HI_U16 usContentLen = 0;
    HI_U16 uslength = 0;
    HI_U8 * pucbuf = HI_NULL;
    AFN_DI_UL_PARAM * pstAfnParam = (AFN_DI_UL_PARAM *)pParam;
    MRS_CSG_FRAME_STRU  *pstFrame = pstAfnParam->pstFrame;
    MRS_CSG_IND_FRAME_STRU stIndFrame = {0};
    HI_U8 ucDenyData = MRS_CSG_DENY_COMMUNICATION_OVERTIME;//否认帧中的错误状态字

    pucContent = pstAfnParam->pstFrame->aucAppData;
    usContentLen = pstAfnParam->pstFrame->usAppDataLen;

    do
    {
        // usContentLen: dist(6) + len(1)
        if (usContentLen != MRS_CSG_COMMUNICATE_DELAYTIME_LEN)//接收长度与实际长度不符合
        {
            ulRet = HI_ERR_BAD_DATA;
            ucDenyData = MRS_CSG_DENY_ERROR_LENGTH;
            break;
        }

        uslength = 2 + usContentLen;
        pucbuf = mrsToolsMalloc(uslength);
        if (!pucbuf)
        {
            ulRet = HI_ERR_NOT_ENOUGH_MEMORY;
            break;
        }

        //目标地址     (6)
        (hi_void)memset_s(pucbuf, uslength, 0, uslength);
        (hi_void)memcpy_s(pucbuf , HI_METER_ADDR_LEN, pucContent, HI_METER_ADDR_LEN);

        //通讯延迟时长 (2)
        pucbuf[6] = (MRS_BC_DELAY_TIME % 256);
        pucbuf[7] = (MRS_BC_DELAY_TIME / 256);

        //数据长度     (1)
        pucbuf[8] = pucContent[6];

        (hi_void)memset_s(&stIndFrame, sizeof(stIndFrame), 0, sizeof(stIndFrame));
        stIndFrame.ucAfn = pstFrame->ucAfn;
        stIndFrame.ulDI = MRS_CSG_DI_IND_COMM_DELAYTIMES;
        stIndFrame.ucSeq = pstFrame->ucSeq;
        stIndFrame.ucPrm = 0;
        stIndFrame.ucAddrFlag = 0;

        stIndFrame.pAppData = pucbuf;
        stIndFrame.usAppDataLen = uslength;
    } while (0);

    if (ulRet == HI_ERR_SUCCESS)
    {
        ulRet = mrsCreateCsgFrame(&stIndFrame, pstAfnParam->ppucOutBuffer, pstAfnParam->pusBufferLen);
        mrsToolsFree(stIndFrame.pAppData);
        pucbuf = HI_NULL;
    }
    else
    {
        ulRet = mrsCsgProtoDenyFrame(&ucDenyData, pstAfnParam);//回复否认帧
    }

    return ulRet;
}

// E8 00 03 05：查询从节点数量
HI_U32 mrsCsg_AFN03_E8000305(HI_INOUT HI_VOID * pParam)
{
    HI_U16 usSlaveNum = mrsSrvArchivesMeterNum();
    return MRS_ProtoCsgEncode((HI_U8 *)&usSlaveNum, sizeof(usSlaveNum), pParam);
}


//AFN03 F3：查询从节点信息
HI_PRV HI_U8 mrsCsgAFN03DIE8030306Buffer[3+sizeof(AFN_03_DI06_NODE_INFO)*MRS_3762_F10_NODE_MAX_NUM] = {0};
HI_U32 mrsCsg_AFN03_E8030306(HI_INOUT HI_VOID * pParam)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U8 * pucContent;
    HI_U16 usContentLen = 0;
    AFN_DI_UL_PARAM * pstAfnParam = (AFN_DI_UL_PARAM *)pParam;
    MRS_CSG_FRAME_STRU  *pstFrame = pstAfnParam ->pstFrame;
    MRS_CSG_IND_FRAME_STRU stIndFrame = {0};

    HI_U16 usStartNo = 0;
    HI_U8  ucFind = 0;
    HI_U8 ucNodeNum = 0;

    HI_U8 *pBuffer = HI_NULL;
    HI_U16 usReplyLen = 0;
    HI_U16 usMeterNum = mrsSrvArchivesMeterNum();

    HI_U8 ucDeny = MRS_CSG_DENY_COMMUNICATION_OVERTIME;

    // 下行数据为从节点起始序号 2个字节,从节点数量1个字节
    pucContent = pstFrame->aucAppData;
    usContentLen = pstFrame->usAppDataLen;

    do
    {
        HI_U16 usIndex = 0;
        HI_U8 * pCurrentNode = 0;

        //载波从节点信息
        if (usContentLen != MRS_CSG_FILES_FAILED_FIX_LEN)
        {
            HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_013, HI_DIAG_MT("length err=%d"),usContentLen);
            ulRet = HI_ERR_BAD_DATA;
            ucDeny = MRS_CSG_DENY_ERROR_LENGTH;
            break;
        }

        //获取到了从节点信息
        (hi_void)memcpy_s(&usStartNo,sizeof(usStartNo),pucContent,sizeof(usStartNo));
        ucNodeNum = pucContent[2];

        if (ucNodeNum == 0)
        {
            ulRet = HI_ERR_BAD_DATA;
            ucDeny = MRS_CSG_DENY_INVALID_DATA;
            break;
        }

        if (ucNodeNum > MRS_3762_F10_NODE_MAX_NUM)
        {
            // 从节点数量超过最大限制，按照32处理
            ucNodeNum = MRS_3762_F10_NODE_MAX_NUM;
        }

        //分配大小(总数量2 + 当前数量1 + 节点信息6*N),全局变量
        pBuffer = (HI_U8 *)mrsCsgAFN03DIE8030306Buffer;
        (hi_void)memset_s(pBuffer, sizeof(mrsCsgAFN03DIE8030306Buffer), 0, sizeof(mrsCsgAFN03DIE8030306Buffer));

        pCurrentNode = pBuffer + 3;

        for (usIndex = usStartNo; (usIndex < PRODUCT_CFG_MRS_MAX_NODE_NUM)
            && (ucFind < ucNodeNum); usIndex++)
        {
            MRS_ARCHIVES_NODE_STRU *pstMeterNode = mrsSrvArchivesQuery(usIndex);
            if ((pstMeterNode != HI_NULL) && (pstMeterNode->bValid))
            {
                AFN_03_DI06_NODE_INFO * pNode = (AFN_03_DI06_NODE_INFO * )pCurrentNode;
                pCurrentNode += sizeof(AFN_03_DI06_NODE_INFO);

                (hi_void)memset_s(pNode, sizeof(AFN_03_DI06_NODE_INFO), 0, sizeof(AFN_03_DI06_NODE_INFO));
                (hi_void)memcpy_s(pNode->aucNodeAddr, HI_METER_ADDR_LEN, pstMeterNode->aucNodeAddr, HI_METER_ADDR_LEN);

                ucFind ++;
            }
        }

        usReplyLen = (HI_U16)(3 + ucFind * sizeof(AFN_03_DI06_NODE_INFO));

        // 总数和数量
        (hi_void)memcpy_s(pBuffer, sizeof(usMeterNum), &usMeterNum, 2);
        pBuffer[2] = ucFind;

        (hi_void)memset_s(&stIndFrame, sizeof(stIndFrame), 0, sizeof(stIndFrame));
        stIndFrame.ucAfn = pstFrame->ucAfn;
        stIndFrame.ulDI = MRS_CSG_DI_IND_NODE_INFS;
        stIndFrame.ucSeq = pstFrame->ucSeq;
        stIndFrame.ucPrm = 0;
        stIndFrame.ucAddrFlag = 0;
        stIndFrame.usAppDataLen = usReplyLen;
        stIndFrame.pAppData = pBuffer;
        ulRet = mrsCreateCsgFrame(&stIndFrame, pstAfnParam->ppucOutBuffer, pstAfnParam->pusBufferLen);

        mrsCcoSmWhitelistSwitchTimerReset();
    } while (0);

    if (ulRet != HI_ERR_SUCCESS)
    {
        ulRet = mrsCsgProtoDenyFrame(&ucDeny, pstAfnParam);
    }

    return ulRet;
}


// AFN03 F7：从节点主动注册进度
HI_U32 mrsCsg_AFN03_E8000307(HI_INOUT HI_VOID * pParam)
{
    MRS_CCO_SRV_CTX_STRU *pstCcoCtx = mrsCcoGetContext();
    HI_U8 ucState = 0;

    if (pstCcoCtx->status & MRS_CCO_STATUS_SEARCH_METER)
    {
        ucState = 1;
    }

    return MRS_ProtoCsgEncode(&ucState, sizeof(ucState), pParam);
}


// AFN03 F8：从节点的父节点
HI_U32 mrsCsg_AFN03_E8030308(HI_INOUT HI_VOID * pParam)
{
    AFN_DI_UL_PARAM *pstAfnParam = (AFN_DI_UL_PARAM *)pParam;
    MRS_CSG_FRAME_STRU *pstFrame = pstAfnParam->pstFrame;
    MRS_CCO_SRV_CTX_STRU *pstCcoCtx = mrsCcoGetContext();
    HI_U32 ulRet = HI_ERR_SUCCESS;

    HI_U8 aucSrcAddr[HI_METER_ADDR_LEN] = {0};
    HI_U8 aucMacAddr[HI_PLC_MAC_ADDR_LEN] = {0};

    HI_U8 aucParentAddr[HI_METER_ADDR_LEN] = {0};
    HI_U8 ucSignalQuality = 0;

    MRS_CSG_IND_FRAME_STRU stIndFrame = {0};

    (hi_void)memset_s(aucParentAddr, HI_METER_ADDR_LEN, 0xFF, HI_METER_ADDR_LEN);

    do
    {
        HI_MAC_NETWORK_TOPO_S *pstTopo = HI_NULL;
        HI_MAC_NETWORK_TOPO_ENTRY_S *pstEntry = HI_NULL;
        MRS_MAC_DEVICE_PAIR *pItem = HI_NULL;
        HI_U32 i;
        HI_U16 usIndex = 0;
        HI_U16 usProxyTei = 0xFFFF;

        if (pstFrame->usAppDataLen != HI_METER_ADDR_LEN)
        {
            ulRet = HI_ERR_BAD_DATA;
            break;
        }

        (hi_void)memcpy_s(aucSrcAddr, HI_METER_ADDR_LEN, pstFrame->aucAppData, HI_METER_ADDR_LEN);

        ulRet = mrsGetMacFromMap(&pstCcoCtx->stMapModule.stMacMeterMap, aucSrcAddr, aucMacAddr);
        if (HI_ERR_SUCCESS != ulRet)
        {
            (hi_void)memcpy_s(aucMacAddr, HI_METER_ADDR_LEN, aucSrcAddr, HI_METER_ADDR_LEN);
            mrsHexInvert(aucMacAddr, HI_METER_ADDR_LEN);
        }

        mrsRefreshTopoInfo(pstCcoCtx);
        pstTopo = &pstCcoCtx->stTopoInf;
        pstEntry = pstTopo->entry + 1;
        for (i = 1; i < pstTopo->num; i++, pstEntry++)
        {
            if (mrsToolsMemEq(pstEntry->mac, aucMacAddr, HI_PLC_MAC_ADDR_LEN))
            {
                usProxyTei = pstEntry->proxy_tei;
                break;
            }
        }

        if (0xFFFF == usProxyTei)
        {
            break;
        }

        if (usProxyTei == 1)
        {
            // CCO
            (hi_void)memcpy_s(aucParentAddr, HI_METER_ADDR_LEN, pstCcoCtx->ucMainNodeAddr, HI_METER_ADDR_LEN);
            mrsHexInvert(aucParentAddr, HI_METER_ADDR_LEN);
            ucSignalQuality = 31;
            break;
        }

        pstEntry = pstTopo->entry + 1;
        for (i = 1; i < pstTopo->num; i++, pstEntry++)
        {
            if (pstEntry->tei == usProxyTei)
            {
                break;
            }
        }

        if (i >= pstTopo->num)
        {
            break;
        }

        pItem = pstCcoCtx->stMapModule.stMacMeterMap.pstItem;
        for (usIndex = 0; usIndex < pstCcoCtx->stMapModule.stMacMeterMap.usMapSize; usIndex++, pItem++)
        {
            if (pItem->bValid && mrsToolsMemEq(pItem->aucMacAddr, pstEntry->mac, HI_PLC_MAC_ADDR_LEN))
            {
                (hi_void)memcpy_s(aucParentAddr, HI_METER_ADDR_LEN, pItem->stDeviceInf.aucAddr, HI_METER_ADDR_LEN);
                ucSignalQuality = 31;
                break;
            }
        }

        if (usIndex == pstCcoCtx->stMapModule.stMacMeterMap.usMapSize)
        {
            (hi_void)memcpy_s(aucParentAddr, HI_METER_ADDR_LEN, pstEntry->mac, HI_METER_ADDR_LEN);
            mrsHexInvert(aucParentAddr, HI_METER_ADDR_LEN);
            ucSignalQuality = 31;
        }
    } while (0);

    (hi_void)memset_s(&stIndFrame, sizeof(stIndFrame), 0, sizeof(stIndFrame));
    stIndFrame.ucAfn = pstFrame->ucAfn;
    stIndFrame.ucSeq = pstFrame->ucSeq;
    stIndFrame.ulDI = 0xE8040308;
    stIndFrame.ucPrm = 0;
    stIndFrame.ucAddrFlag = 0;

    stIndFrame.usAppDataLen = HI_METER_ADDR_LEN * 2 + sizeof(HI_U8);
    stIndFrame.pAppData = (HI_U8 *)mrsToolsMalloc(stIndFrame.usAppDataLen);
    if (!stIndFrame.pAppData)
    {
        return HI_ERR_NOT_ENOUGH_MEMORY;
    }

    (hi_void)memcpy_s(stIndFrame.pAppData, HI_METER_ADDR_LEN, pstFrame->aucAppData, HI_METER_ADDR_LEN);
    (hi_void)memcpy_s(stIndFrame.pAppData + HI_METER_ADDR_LEN, HI_METER_ADDR_LEN, aucParentAddr, HI_METER_ADDR_LEN);
    stIndFrame.pAppData[HI_METER_ADDR_LEN + HI_METER_ADDR_LEN] = ucSignalQuality;

    ulRet = mrsCreateCsgFrame(&stIndFrame, pstAfnParam->ppucOutBuffer, pstAfnParam->pusBufferLen);
    mrsToolsFree(stIndFrame.pAppData);

    return ulRet;
}


//设置主节点地址 E8020401
HI_U32 mrsCsg_AFN04_E8020401(HI_INOUT HI_VOID * pParam)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U8 * pContent;
    HI_U16 usContentLen = 0;
    AFN_DI_UL_PARAM * pstAfnParam = (AFN_DI_UL_PARAM *)pParam;
    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();
    HI_U8 ucDeny = MRS_CSG_DENY_COMMUNICATION_OVERTIME;//否认帧中的错误状态字
    HI_NV_FTM_CCO_PARAM_ID_S stCcoParam;

    if (pParam == HI_NULL)
    {
        ulRet = HI_ERR_BAD_DATA;
        return ulRet;
    }

    //上行数据为6个字节的主节点地址
    pContent = pstAfnParam->pstFrame->aucAppData;
    usContentLen = pstAfnParam->pstFrame->usAppDataLen;

    do
    {
        if (usContentLen != (sizeof(pstCcoCtx->ucMainNodeAddr))) //设置载波主节点的应用长度为
        {
            ulRet = HI_ERR_BAD_DATA;
            ucDeny = MRS_CSG_DENY_ERROR_LENGTH;
            break;
        }

        //设置主节点地址
        (hi_void)memcpy_s(pstCcoCtx->ucMainNodeAddr, sizeof(pstCcoCtx->ucMainNodeAddr), pContent, sizeof(pstCcoCtx->ucMainNodeAddr));
        mrsHexInvert(pstCcoCtx->ucMainNodeAddr, sizeof(pstCcoCtx->ucMainNodeAddr));
        HI_MDM_DevIdConfig(pstCcoCtx->ucMainNodeAddr);

        //根据项ID写NV项内容
#if defined(PRODUCT_CFG_SYS_RST_SOLUTION)
        HI_MDM_RstDelayEnable(HI_SYS_RST_DELAY_CON_WR_NV, HI_SYS_RST_DELAY_CON_WR_NV_TO);
#endif

        (hi_void)memset_s(&stCcoParam, sizeof(stCcoParam), 0, sizeof(stCcoParam));
        ulRet = HI_MDM_NV_Read(HI_NV_FTM_CCO_PARAM_ID, &stCcoParam, sizeof(HI_NV_FTM_CCO_PARAM_ID_S));
        if (ulRet == HI_ERR_SUCCESS)
        {
            (hi_void)memcpy_s(stCcoParam.ucDevAddr, sizeof(stCcoParam.ucDevAddr), pstCcoCtx->ucMainNodeAddr, sizeof(pstCcoCtx->ucMainNodeAddr));
            ulRet = HI_MDM_NV_Write(HI_NV_FTM_CCO_PARAM_ID, &stCcoParam, sizeof(HI_NV_FTM_CCO_PARAM_ID_S));
        }

#if defined(PRODUCT_CFG_SYS_RST_SOLUTION)
        HI_MDM_RstDelayDisable(HI_SYS_RST_DELAY_CON_WR_NV);
#endif
    } while (0);

    if (ulRet != HI_ERR_SUCCESS)
    {
        return  mrsCsgProtoDenyFrame(&ucDeny,pstAfnParam);
    }
    else
    {
        return mrsCsgProtoAckFrame(0,pstAfnParam);
    }
}


HI_U32 mrsCsgIsMeterRepeat(HI_U8 *pucContent, HI_U8 ucMeterIndex, HI_U8 ucMeterNum)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U16 usIndex = 0;
    HI_U8 aucMeterAddr[HI_METER_ADDR_LEN] = {0};
    HI_U8 aucTempAddr[HI_METER_ADDR_LEN] = {0};
    HI_U8 ucProtocol = MRS_PROTO645_VER_2007;

    if (pucContent == HI_NULL)
    {
        return HI_ERR_BAD_DATA;
    }

    mrsMeterAddrPreProc((HI_U8 *)(aucMeterAddr), pucContent + ucMeterIndex * HI_METER_ADDR_LEN, ucProtocol);
    for (usIndex = ucMeterIndex + 1; usIndex < ucMeterNum; usIndex++)
    {
        mrsMeterAddrPreProc(aucTempAddr, pucContent + usIndex * HI_METER_ADDR_LEN, ucProtocol);

        if (memcmp(aucMeterAddr, aucTempAddr, HI_METER_ADDR_LEN) == 0)
        {
            ulRet = HI_ERR_FAILURE; //要添加的表中存在重复表号
            HI_DIAG_LOG_MSG_E0(__LINE__, HI_DIAG_MT("【CSG】[mrsCsgIsMeterRepeat] 【ERR】//要添加的表中存在重复表号"));
            break;
        }
    }

    return ulRet;
}


HI_U32 mrsCsgNeedInsertNode(HI_U8 * pucContent, HI_U8 ucMeterNum, HI_U8 *pucDeny)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U32 usIsBCD = HI_FALSE;
    HI_U8 ucMeterIndex = 0;
    HI_U8 ucMaxAddNum = MRS_3762_ADD_METERS_MAX_NUM;
    HI_U16 usMeterNum = mrsSrvArchivesMeterNum();

    *pucDeny = MRS_CSG_DENY_COMMUNICATION_OVERTIME;

    if (HI_NULL == pucContent) //入参判断
    {
        return HI_ERR_BAD_DATA;
    }

    if (ucMeterNum > ucMaxAddNum)
    {
        return HI_ERR_FAILURE;
    }

    while (ucMeterIndex < ucMeterNum)
    {
        HI_U8 *pucAddr = pucContent + ucMeterIndex * HI_METER_ADDR_LEN;

        //检查是否是BCD码
        usIsBCD = mrsToolsCheckBCD(pucAddr, HI_METER_ADDR_LEN);
        if (usIsBCD != HI_TRUE)
        {
            ulRet = HI_ERR_BAD_DATA;
            *pucDeny = MSG_CSG_DENY_ERROR_FORMAT; //格式错误
            return ulRet;
        }

        //检查是否是广播地址
        usIsBCD = mrsToolsBroadCastAddr(pucAddr);
        if (usIsBCD == HI_TRUE)
        {
            ulRet = HI_ERR_BAD_DATA;
            *pucDeny = MSG_CSG_DENY_ERROR_FORMAT; //格式错误

            return ulRet;
        }

        ucMeterIndex++;
    }

    if (usMeterNum + ucMeterNum > PRODUCT_CFG_MRS_MAX_NODE_NUM) //检查表档案是否已满
    {
        ulRet = HI_ERR_FULL;
        *pucDeny = MRS_CSG_DENY_COMMUNICATION_OVERTIME;

        return ulRet;
    }

    for (ucMeterIndex = 0; ucMeterIndex < ucMeterNum; ucMeterIndex++)
    {
        HI_U16 usIndex = 0;
        HI_U8 *pucMeterAddr = HI_NULL;

        ulRet = mrsCsgIsMeterRepeat(pucContent, ucMeterIndex, ucMeterNum);
        if (ulRet != HI_ERR_SUCCESS)
        {
            *pucDeny = MSG_CSG_DENY_METER_REPEAT;//要添加的表中存在重复的表号
            ulRet = HI_ERR_INITILIZATION_ALREADY;
            break;
        }

        pucMeterAddr = pucContent + ucMeterIndex * HI_METER_ADDR_LEN;

        // 只查询表档案中的有效表
        while (usIndex < usMeterNum)
        {
            MRS_ARCHIVES_NODE_STRU *pstNode = HI_NULL;

            pstNode = mrsSrvArchivesQuery(usIndex);
            if (pstNode == HI_NULL)
            {
                return HI_ERR_FAILURE;
            }

            if (pstNode->bValid)
            {
                if (memcmp(pucMeterAddr, pstNode->aucNodeAddr, HI_METER_ADDR_LEN) == 0)
                {
                    *pucDeny = MSG_CSG_DENY_METER_REPEAT;//表号重复
                    ulRet = HI_ERR_INITILIZATION_ALREADY;

                    return ulRet;
                }
            }

            usIndex++;
        }
    }

    return ulRet;
}


//添加从节点地址 E8020402
HI_U32 mrsCsg_AFN04_E8020402(HI_INOUT HI_VOID * pParam)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U8 ucMeterNum = 0;
    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();
    AFN_DI_UL_PARAM * pstAfnParam = (AFN_DI_UL_PARAM *)pParam;
    HI_U8 ucDeny = MRS_CSG_DENY_COMMUNICATION_OVERTIME;//否认帧中的错误状态字

    HI_U16 usTempNum = mrsSrvArchivesMeterNum();

    //下行的应用数据
    HI_U8 * pucContent = pstAfnParam->pstFrame->aucAppData;
    HI_U16 usContentLen = pstAfnParam->pstFrame->usAppDataLen;

    do
    {
        HI_U8 i = 0;
        if (usContentLen < 1)
        {
            ulRet = HI_ERR_BAD_DATA;
            ucDeny = MRS_CSG_DENY_ERROR_LENGTH;
            break;
        }

        //长度判断
        ucMeterNum = pucContent[0];
        if (usContentLen != (HI_METER_ADDR_LEN * ucMeterNum + 1))
        {
            ulRet = HI_ERR_BAD_DATA;
            ucDeny = MRS_CSG_DENY_ERROR_LENGTH;
            break;
        }

        if (ucMeterNum == 0)
        {
            ucDeny = MRS_CSG_DENY_INVALID_DATA;//无效的数据单元
            ulRet = HI_ERR_BAD_DATA;
            break;
        }

        pucContent += 1;

        ulRet = mrsCsgNeedInsertNode(pucContent, ucMeterNum, &ucDeny); //判断是否应添加从节点
        if (HI_ERR_SUCCESS != ulRet)
        {
            break;
        }

        for (i = 0; i < ucMeterNum; i++)
        {
            MRS_ARCHIVES_NODE_STRU stNode;
            (hi_void)memset_s(&stNode, sizeof(stNode), 0, sizeof(stNode));
            (hi_void)memcpy_s(stNode.aucNodeAddr, HI_METER_ADDR_LEN, pucContent, HI_METER_ADDR_LEN);
            stNode.bValid = HI_TRUE;
            stNode.ucSave = HI_TRUE;
            stNode.ucProtocol = MRS_PROTO645_VER_2007;
            if (HI_ERR_SUCCESS != mrsSrvArchivesInsert(&stNode, HI_TRUE))
            {
                break;
            }

            pucContent += HI_METER_ADDR_LEN;
        }
    } while (0);

    if (ulRet == HI_ERR_SUCCESS)
    {
        ulRet = mrsCsgProtoAckFrame(0,pstAfnParam);

        //新添加了一个定时器(激活写入flash定时器)（分配ID）：超时后将添加的表信息写入flash
        MRS_StartTimer(EN_MRS_SRV_CCO_WRITE_FLASH_ACTIV_TIMER, pstCcoCtx->ucFlashSaveDelay * MS_PER_SECOND, HI_SYS_TIMER_ONESHOT);
        mrsCcoSmWhitelistSwitchTimerReset();
        mrsSetArchivesStatus(MRS_CCO_ARCHIVES_STATUS_WAITING);

        if ((0 == usTempNum) && (mrsSrvArchivesMeterNum() > 0))
        {
            mrsCcoDetectEmptyArchivesAddProc();
        }
    }
    else  //这里还没有区分什么原因不能添加，区分表号重复的否认帧数据单元
    {
        ulRet = mrsCsgProtoDenyFrame(&ucDeny, pstAfnParam);
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_016, HI_DIAG_MT("mrsSrvArchivesInsert failed"));
    }

    mrsDfxLrMeterNum(mrsSrvArchivesMeterNum());

    return ulRet;
}


//删除从节点地址 E8020403
HI_U32 mrsCsg_AFN04_E8020403(HI_INOUT HI_VOID * pParam)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U8 ucUnitConstLen = 6;//报文中数据单元内容内容最小单元(字节数):表地址(6个字节)
    AFN_DI_UL_PARAM * pstAfnParam = (AFN_DI_UL_PARAM *)pParam;
    HI_U8 ucDeny = MRS_CSG_DENY_COMMUNICATION_OVERTIME;//否认帧中的错误状态字

    HI_U16 usTempNum = mrsSrvArchivesMeterNum();

    //下行的应用数据
    HI_U8 * pucContent = pstAfnParam->pstFrame->aucAppData;
    HI_U16 usContentLen = pstAfnParam->pstFrame->usAppDataLen;
    do
    {
        HI_U8 ucMeterNum = 0;
        HI_U8 ucMeterIndex = 0;
        HI_BOOL bIsBCD = HI_TRUE;
        HI_U8 ucDelFlag = 0;

        if (usContentLen < 1)
        {
            HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_017, HI_DIAG_MT("len<1:%d"),usContentLen);
            ulRet = HI_ERR_BAD_DATA;
            ucDeny = MRS_CSG_DENY_ERROR_LENGTH;
            break;
        }

        //长度判断
        ucMeterNum = pucContent[0];
        if (usContentLen != (ucUnitConstLen * ucMeterNum + 1))
        {
            HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_018, HI_DIAG_MT("len err:%d"),usContentLen);
            ulRet = HI_ERR_BAD_DATA;
            ucDeny = MRS_CSG_DENY_ERROR_LENGTH;
            break;
        }

        if (ucMeterNum == 0)
        {
            ucDeny = MRS_CSG_DENY_INVALID_DATA;//无效的数据单元
            ulRet = HI_ERR_BAD_DATA;
            break;
        }

        pucContent += 1;
        while (ucMeterIndex < ucMeterNum)
        {
            //检查是否是广播地址
            bIsBCD = !(mrsToolsBroadCastAddr(pucContent + ucMeterIndex * ucUnitConstLen));
            if (bIsBCD != HI_TRUE)
            {
                ulRet = HI_ERR_BAD_DATA;
                ucDeny = MSG_CSG_DENY_ERROR_FORMAT; //格式错误
                break;
            }

            ucMeterIndex++;
        }

        if (bIsBCD == HI_FALSE)
        {
            break;
        }

        while (ucMeterNum-- > 0)
        {
            if (HI_ERR_SUCCESS == mrsSrvArchivesDelete(pucContent))
            {
                ucDelFlag++;
            }

            pucContent += HI_METER_ADDR_LEN;
        }

        if (ucDelFlag== 0)
        {
            ucDeny = MSG_CSG_DENY_METER_NOT_EXIST;//表号不存在
            ulRet = HI_ERR_BAD_DATA;
            break;
        }
    } while (0);

    if (ulRet == HI_ERR_SUCCESS)
    {
        //组织确认帧
        ulRet = mrsCsgProtoAckFrame(0,pstAfnParam);

        mrsSrvArchivesRealign();

        //(激活写入flash定时器)（与添加从节点时的定时器相同）：超时后将删除且重新排列的表信息写入flash
        MRS_StartTimer(EN_MRS_SRV_CCO_WRITE_FLASH_ACTIV_TIMER, mrsCcoGetContext()->ucFlashSaveDelay * MS_PER_SECOND, HI_SYS_TIMER_ONESHOT);
        mrsCcoSmWhitelistSwitchTimerReset();
        mrsSetArchivesStatus(MRS_CCO_ARCHIVES_STATUS_WAITING);

        if ((0 != usTempNum) && (mrsSrvArchivesMeterNum() == 0))
        {
            mrsCcoDetectRmReset();
            mrsDfxCcoArchivesClearTime(HI_MDM_GetMilliSeconds());
        }
    }
    else
    {
        //组织否认帧
        ulRet = mrsCsgProtoDenyFrame(&ucDeny,pstAfnParam);
    }

    mrsDfxLrMeterNum(mrsSrvArchivesMeterNum());

    return ulRet;
}


// 允许/禁止从节点事件
HI_U32 mrsCsg_AFN04_E8020404(HI_INOUT HI_VOID * pParam)
{
    MRS_CCO_EVT_CTX_STRU *pstEvtCtx = mrsGetCcoEvtCtx();
    AFN_DI_UL_PARAM * pstAfnParam = (AFN_DI_UL_PARAM *)pParam;
    MRS_CSG_FRAME_STRU  *pstFrame = pstAfnParam->pstFrame;
    HI_U8 ucFunCode = 0;
    HI_U8 ucDeny = MRS_CSG_DENY_COMMUNICATION_OVERTIME;
    HI_U16 usWaitTime = 10; // 预计执行10秒
    HI_U32 ulRet = HI_ERR_SUCCESS;

    if (pstFrame->usAppDataLen != 1)
    {
        ucDeny = MRS_CSG_DENY_ERROR_LENGTH;
        return mrsCsgProtoDenyFrame(&ucDeny, pstAfnParam);
    }

    ucFunCode = pstFrame->aucAppData[0];

    // 置事件上报标志
    if (ucFunCode == MRS_EVENT_REPORT_DISABLE)
    {
        if (pstEvtCtx->ucReportEnable)
        {
            mrsSrvEmptyQueue(&(pstEvtCtx->stEvtQueue.stQueue), mrsCsgQueueFree);
            mrsSrvListClear(&(pstEvtCtx->stEvtReportedList.stEvtFilterList), mrsCsgListFree, HI_NULL);
        }

        pstEvtCtx->ucReportEnable = MRS_EVENT_REPORT_DISABLE;
    }
    else
    {
        pstEvtCtx->ucReportEnable = MRS_EVENT_REPORT_ENABLE;
    }

    ulRet = mrsCcoEvtFlagNotice(pstEvtCtx->ucReportEnable);

    if (HI_ERR_SUCCESS == ulRet)
    {
        return mrsCsgProtoAckFrame(usWaitTime, pstAfnParam);
    }
    else
    {
        return mrsCsgProtoDenyFrame(&ucDeny, pstAfnParam);
    }
}


// 激活从节点主动注册
HI_U32 mrsCsg_AFN04_E8020405(HI_INOUT HI_VOID * pParam)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    MRS_CCO_SRV_CTX_STRU *pstCcoCtx = mrsCcoGetContext();
    AFN_DI_UL_PARAM *pstAfnParam = (AFN_DI_UL_PARAM *)pParam;
    HI_U8 ucDeny = MRS_CSG_DENY_COMMUNICATION_OVERTIME;

    do
    {
        if (pstCcoCtx->status & MRS_CCO_STATUS_SEARCH_METER)
        {
            ulRet = HI_ERR_SUCCESS;
            break;
        }
        else if (pstCcoCtx->status & MRS_CCO_STATUS_SEARCH_METER_TRANSFORMER)
        {
            ulRet = HI_ERR_BUSY;
            ucDeny = MSG_CSG_DENY_CCO_BUSY;
            break;
        }

        // 记录搜表参数: 持续时间
        pstCcoCtx->usSearchDuration = 0;
        mrsCcoDfxSearchSetDuration((MRS_CCO_SEARCH_DFX_STRU* )&(pstCcoCtx->stAutoSearchModel.stSearchDfx), pstCcoCtx->usSearchDuration);

        mrsSearchMeterPrepare(pstCcoCtx);
        if (mrsGetCcoPlcChlStatus())
        {
            pstCcoCtx->ucSearchWaitFlag = HI_FALSE;
            ulRet = mrsSearchMeterProc(pstCcoCtx);
        }
        else
        {
            pstCcoCtx->ucSearchWaitFlag = HI_TRUE;
        }

        mrsCcoDfxSearchStartTime(&(pstCcoCtx->stAutoSearchModel.stSearchDfx));
        HI_DIAG_LOG_MSG_E0(__LINE__, HI_DIAG_MT("开始搜表"));
    } while (0);

    if (ulRet == HI_ERR_SUCCESS)
    {
        return mrsCsgProtoAckFrame(0, pstAfnParam);
    }
    else
    {
        return mrsCsgProtoDenyFrame(&ucDeny, pstAfnParam);
    }
}


//终止从节点主动注册 E8020406
HI_U32 mrsCsg_AFN04_E8020406(HI_INOUT HI_VOID * pParam)
{
    AFN_DI_UL_PARAM * pstAfnParam = (AFN_DI_UL_PARAM *)pParam;

    mrsCcoStopSearchMeterNormal((HI_U8)MRS_CCO_SEARCH_FINISH_STOP);
    return mrsCsgProtoAckFrame(0,pstAfnParam);
}


//请求集中器时间 E8060601
HI_U32 mrsCsg_AFN06_E8060601(HI_INOUT HI_VOID * pParam)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    AFN_DI_UL_PARAM * pstAfnParam = (AFN_DI_UL_PARAM *)pParam;
    //下行的应用数据
    HI_U8 *  pucContent = pstAfnParam->pstFrame->aucAppData;
    HI_U16 usContentLen = pstAfnParam->pstFrame->usAppDataLen;

    do
    {
        // + 时间(年月日时分秒)
        if (usContentLen != 6)
        {
            ulRet = HI_ERR_BAD_DATA;
            break;
        }

        ulRet = MRS_TimeIsValid(&pucContent[0]);
        if (ulRet != HI_ERR_SUCCESS)
        {
            ulRet = HI_ERR_BAD_DATA;
            break;
        }

        ulRet = mrsCcoSetClock(&pucContent[0]);
        if (ulRet != HI_ERR_SUCCESS)
        {
            break;
        }
    } while (0);

    return ulRet;
}

//启动文件传输 E8020701
HI_U32 mrsCsg_AFN07_E8020701(HI_INOUT HI_VOID * pParam)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    AFN_DI_UL_PARAM * pstAfnParam = (AFN_DI_UL_PARAM *)pParam;
    HI_U8 * pContent = HI_NULL;
    HI_U16 usContentLen = 0;
    HI_U8 ucDeny = MRS_CSG_DENY_COMMUNICATION_OVERTIME;//否认帧中的错误状态字
    MRS_REMOTE_UPG_MODULE * pstMrsUpgCtx = mrsGetRemoteUpgCtx();
    HI_U8 ucFileFlag = 0;

    pContent = pstAfnParam->pstFrame->aucAppData;
    usContentLen = pstAfnParam->pstFrame->usAppDataLen;

    do
    {
        //长度 = 文件性质(1)+ 文件ID(1)+ 目的地址(6)+ 文件总段数(2)+文件大小(4)+ 文件总校验(2)+文件传输时间(1)
        if (usContentLen != MRS_CSG_UPG_ACK_TOTAL_LEN)
        {
            ucDeny = MRS_CSG_DENY_ERROR_LENGTH;
            ulRet = HI_ERR_FAILURE;
            break;
        }

        ucFileFlag = *(pContent + MRS_CSG_UPG_FILE_FLAG_OFFSET);
        if (mrsCsgUpgFlgValid(ucFileFlag) == HI_FALSE)
        {
            ucDeny = MSG_CSG_DENY_OTHER;
            ulRet = HI_ERR_FAILURE;
            break;
        }
        
        pstMrsUpgCtx->ucUpgFileFlag = ucFileFlag;
        mrsDfxRefreshClearUpg(ucFileFlag);
        
        if (MRS_CSG_CLEAR_UPG_LOAD_FILE == ucFileFlag)//清除下装程序
        {
            mrsRemoteCsgClearFailedInfos();
            ulRet = mrsRemoteUpgClear(pstMrsUpgCtx);
            if (HI_ERR_CONTINUE != ulRet)
            {
                if (HI_ERR_SUCCESS == ulRet)
                {
                    pstMrsUpgCtx->ucUpgStatus = MRS_CSG_UPG_DONE;
                }
                
                break;
            }

            pstMrsUpgCtx->pstCsgFrame = mrsToolsMalloc(sizeof(MRS_CSG_FRAME_STRU));
            if (HI_NULL == pstMrsUpgCtx->pstCsgFrame)
            {
                ulRet = HI_ERR_NOT_ENOUGH_MEMORY;
                break;
            }

            (hi_void)memset_s(pstMrsUpgCtx->pstCsgFrame, sizeof(MRS_CSG_FRAME_STRU), 0, sizeof(MRS_CSG_FRAME_STRU));
            (hi_void)memcpy_s(pstMrsUpgCtx->pstCsgFrame, sizeof(MRS_CSG_FRAME_STRU), pstAfnParam->pstFrame, sizeof(MRS_CSG_FRAME_STRU));
            return ulRet;
        }
        
        pstMrsUpgCtx->ucUpgStatus = MRS_CSG_UPG_INPROGRESS;
        pstMrsUpgCtx->ucFileId = pContent[MRS_CSG_UPG_FILE_ID_OFFSET];
        mrsCsgSetUpgFileIdNv();
        
        (hi_void)memcpy_s(pstMrsUpgCtx->aucDestAddr, HI_METER_ADDR_LEN, pContent + MRS_CSG_UPG_DEST_ADDRS_OFFSET, HI_METER_ADDR_LEN);

        (hi_void)memcpy_s(&pstMrsUpgCtx->ulTotalSegNum, sizeof(pstMrsUpgCtx->ulTotalSegNum), pContent + MRS_CSG_UPG_TOTAL_SEG_OFFSET, sizeof(HI_U16));
        (hi_void)memcpy_s(&pstMrsUpgCtx->ulFileSize, sizeof(pstMrsUpgCtx->ulFileSize), pContent + MRS_CSG_UPG_TOTAL_DATA_LEN_OFFSET, sizeof(HI_U32));

        (hi_void)memcpy_s(&pstMrsUpgCtx->usFileCs, sizeof(pstMrsUpgCtx->usFileCs), pContent + MRS_CSG_UPG_TOTAL_CS_OFFSET, sizeof(HI_U16));
        pstMrsUpgCtx->ulUpgTimeWindow = pContent[MRS_CSG_UPG_OFFSET_TRANSIMIT_TIME];

    } while (HI_FALSE);

    if (ulRet == HI_ERR_SUCCESS)
    {
         return  mrsCsgProtoAckFrame(0, pstAfnParam);
    }
    else
    {
        return  mrsCsgProtoDenyFrame(&ucDeny, pstAfnParam);
    }
}


//传输文件内容 E8020702
HI_U32 mrsCsg_AFN07_E8020702(HI_INOUT HI_VOID * pParam)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U32 ulSeg = 0;
    HI_U8 * pContent = HI_NULL;
    HI_U16 usContentLen = 0;
    AFN_DI_UL_PARAM * pstAfnParam = (AFN_DI_UL_PARAM *)pParam;
    MRS_CSG_FRAME_STRU  *pstFrame = pstAfnParam->pstFrame;
    HI_U8 ucDeny = MRS_CSG_DENY_COMMUNICATION_OVERTIME;//否认帧中的错误状态字
    HI_U16 usDataLen = 0;
    HI_U16 usCrc16 = 0;
    HI_U16 usTemp = 0;  

    MRS_REMOTE_UPG_MODULE * pstMrsUpgCtx = mrsGetRemoteUpgCtx();

    do
    {
        mrsRemoteUpgRxFilePro();
        usContentLen = pstFrame->usAppDataLen;
        pContent = pstFrame->aucAppData;

        (hi_void)memcpy_s(&usDataLen, sizeof(usDataLen), pContent + MRS_CSG_UPG_DATA_LEN_OFFSET, sizeof(usDataLen));
        if (usContentLen != (MRS_CSG_UPG_DATA_OFFSET + usDataLen + MRS_CSG_UPG_CS_LEN))
        {
            ucDeny = MRS_CSG_DENY_ERROR_LENGTH;
            ulRet = HI_ERR_FAILURE;
            HI_DIAG_LOG_MSG_E0(__LINE__, HI_DIAG_MT("【CSG】ERROR_LENGTH;"));
            break;
        }

        mrsDfxRxUpgFrameNum();


        (hi_void)memcpy_s(&ulSeg, sizeof(ulSeg), pContent + MRS_CSG_UPG_CURRENT_SEG_OFFSET, sizeof(HI_U16));
 
        if (ulSeg >= pstMrsUpgCtx->ulTotalSegNum)
        {
            mrsDfxRefreshFrameErrFlag(MRS_DFX_CCO_UPG_CURR_SEG_OVER_TOTAL_SEG_ERR);
            ucDeny = MSG_CSG_DENY_OTHER;
            HI_DIAG_LOG_MSG_E0(__LINE__, HI_DIAG_MT("【CSG】UPG_ERROR_SEG ;"));
            ulRet = HI_ERR_FAILURE;
            break;
        }

        (hi_void)memcpy_s(&usTemp, MRS_CSG_UPG_CS_LEN, pContent + MRS_CSG_UPG_DATA_OFFSET + usDataLen, MRS_CSG_UPG_CS_LEN);
        usCrc16 = HI_MDM_Crc16(pstMrsUpgCtx->usCrcValue, (HI_CONST HI_U8 *)(pContent+ MRS_CSG_UPG_DATA_OFFSET), usDataLen);
        if (usCrc16 != usTemp)
        {
            mrsDfxRefreshFrameErrFlag(MRS_DFX_CCO_UPG_FILE_CRC_ERR);
            ucDeny = MSG_CSG_DENY_OTHER;
            ulRet = HI_ERR_FAILURE;
            HI_DIAG_LOG_MSG_E2(__LINE__, HI_DIAG_MT("upgfileCrc= %#X CalCrc16= %#X"), usTemp,usCrc16);
            break;
        }

        ulRet = mrsRemoteUpgFrameRxPro(pContent, ulSeg, (HI_U16)pstMrsUpgCtx->ulTotalSegNum);
        pstMrsUpgCtx->ucFrameSeq = pstFrame->ucSeq;

        if (HI_ERR_SUCCESS != ulRet)
        {
            ucDeny = MSG_CSG_DENY_OTHER;
            HI_DIAG_LOG_MSG_E0(__LINE__, HI_DIAG_MT("【CSG】【mrsCsg_AFN07_E8020702】【ERR】if (HI_ERR_SUCCESS != ulRet)"));
            break;
        }

        if ((ulSeg + 1) == pstMrsUpgCtx->ulTotalSegNum)//最后一段
        {
            mrsUpgLocalPro(pstMrsUpgCtx);
        }

    }while (HI_FALSE);

    mrsDfxTxUpgFrameNum();
    if (HI_ERR_SUCCESS != ulRet)
    {
        return  mrsCsgProtoDenyFrame(&ucDeny, pstAfnParam);
    }
    else
    {
        return mrsCsgProtoAckFrame(0, pstAfnParam);
    }
}


HI_U32 mrsCsgGetFileBuffer(HI_U8 **ppBuffer, HI_U16 *pusLength)
{
    HI_U8 *pData = HI_NULL;
    HI_U16 usDataLen =  MRS_CSG_FILES_INFOS_FIX_LEN;
    MRS_REMOTE_UPG_MODULE * pstMrsUpgCtx = mrsGetRemoteUpgCtx();
    HI_U16 usCurrSeg = 0;
    pData = mrsToolsMalloc(usDataLen);
    if (!pData)
    {
        return HI_ERR_NOT_ENOUGH_MEMORY;
    }

    (hi_void)memset_s(pData, usDataLen, 0, usDataLen);
    if (pstMrsUpgCtx->ucUpgPeriod != MRS_CCO_UPGRADE_IDLE_PERIOD)//!!!
    {
        usCurrSeg = (HI_U16)(pstMrsUpgCtx->ulCurrSegNum + 1);
    }

    *ppBuffer = pData;
    *pusLength = usDataLen;

    *pData++ = pstMrsUpgCtx->ucUpgFileFlag;
    *pData++ = pstMrsUpgCtx->ucFileId;
    (hi_void)memcpy_s(pData, HI_METER_ADDR_LEN, pstMrsUpgCtx->aucDestAddr, HI_METER_ADDR_LEN);
    pData += HI_METER_ADDR_LEN;

    (hi_void)memcpy_s(pData, sizeof(pstMrsUpgCtx->ulTotalSegNum), &pstMrsUpgCtx->ulTotalSegNum, sizeof(HI_U16));
    pData += sizeof(HI_U16);

    (hi_void)memcpy_s(pData, sizeof(pstMrsUpgCtx->ulFileSize), &pstMrsUpgCtx->ulFileSize, sizeof(pstMrsUpgCtx->ulFileSize));
    pData += sizeof(pstMrsUpgCtx->ulFileSize);

    (hi_void)memcpy_s(pData, sizeof(pstMrsUpgCtx->usFileCs), &pstMrsUpgCtx->usFileCs, sizeof(pstMrsUpgCtx->usFileCs));
    pData += sizeof(pstMrsUpgCtx->usFileCs);

    (hi_void)memcpy_s(pData, sizeof(usCurrSeg), &usCurrSeg, sizeof(HI_U16));

    return HI_ERR_SUCCESS;
}


//查询文件信息 E8030703
HI_U32 mrsCsg_AFN07_E8000703(HI_INOUT HI_VOID * pParam)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    MRS_CSG_IND_FRAME_STRU stIndFrame = {0};
    AFN_DI_UL_PARAM  *pstAfnParam = (AFN_DI_UL_PARAM *)pParam;
    MRS_CSG_FRAME_STRU *pstFrame = pstAfnParam->pstFrame;

    HI_DIAG_LOG_MSG_E0(__LINE__, HI_DIAG_MT("【CSG】【mrsCsg_AFN07_E8000703】//查询文件信息 E8030703 start"));

    (hi_void)memset_s(&stIndFrame, sizeof(stIndFrame), 0, sizeof(stIndFrame));

    stIndFrame.ucAfn = pstFrame->ucAfn;
    stIndFrame.ulDI = pstFrame->ulDI;
    stIndFrame.ucSeq = pstFrame->ucSeq;
    stIndFrame.ucPrm = 0;
    stIndFrame.ucAddrFlag = 0;

    ulRet = mrsCsgGetFileBuffer(&stIndFrame.pAppData, &stIndFrame.usAppDataLen);
    if (HI_ERR_SUCCESS != ulRet)
    {
        return HI_ERR_FAILURE;
    }

    ulRet = mrsCreateCsgFrame(&stIndFrame, pstAfnParam->ppucOutBuffer, pstAfnParam->pusBufferLen);

    mrsToolsFree(stIndFrame.pAppData);

    return ulRet;
}


// TODO: 添加(2byte)失败节点数量
HI_U32 mrsCsgGetFileProcess(HI_U8 **ppucBuffer, HI_U16 *pusLength)
{
    HI_U8 *pData = HI_NULL;
    HI_U16 usFaildedCnt = mrsGetRemoteUpgCtx()->stUpgNumInfo.usFailNum;
    MRS_REMOTE_UPG_MODULE* pstMrsUpgCtx = mrsGetRemoteUpgCtx();
    HI_U16 usDataLen = MRS_CSG_FILE_PROCESS_FIX_LEN;
    
    pData = mrsToolsMalloc(usDataLen);
    if (!pData)
    {
        return HI_ERR_NOT_ENOUGH_MEMORY;
    }

    (hi_void)memset_s(pData, usDataLen, 0, usDataLen);

    *ppucBuffer = pData;
    *pusLength = usDataLen;

    *pData++ = pstMrsUpgCtx->ucUpgStatus;
    *pData++ = pstMrsUpgCtx->ucFileId;

    (hi_void)memcpy_s(pData, sizeof(usFaildedCnt), &usFaildedCnt, sizeof(usFaildedCnt));

    return HI_ERR_SUCCESS;
}


// 查询文件处理进度 E8000704
HI_U32 mrsCsg_AFN07_E8000704(HI_INOUT HI_VOID * pParam)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    MRS_CSG_IND_FRAME_STRU stIndFrame = {0};
    AFN_DI_UL_PARAM  *pstAfnParam = (AFN_DI_UL_PARAM *)pParam;
    MRS_CSG_FRAME_STRU *pstFrame = pstAfnParam->pstFrame;

    HI_DIAG_LOG_MSG_E0(__LINE__, HI_DIAG_MT("【CSG】【mrsCsg_AFN07_E8000704】//查询文件处理进度 E8000704 start"));

    (hi_void)memset_s(&stIndFrame, sizeof(stIndFrame), 0, sizeof(stIndFrame));

    stIndFrame.ucAfn = pstFrame->ucAfn;
    stIndFrame.ulDI = pstFrame->ulDI;
    stIndFrame.ucSeq = pstFrame->ucSeq;
    stIndFrame.ucPrm = 0;
    stIndFrame.ucAddrFlag = 0;

    ulRet = mrsCsgGetFileProcess(&stIndFrame.pAppData, &stIndFrame.usAppDataLen);
    if (HI_ERR_SUCCESS != ulRet)
    {
        return HI_ERR_FAILURE;
    }

    ulRet = mrsCreateCsgFrame(&stIndFrame, pstAfnParam->ppucOutBuffer, pstAfnParam->pusBufferLen);

    mrsToolsFree(stIndFrame.pAppData);

    return ulRet;
}

HI_U32 mrsCsgGetFileFailedInfo(HI_U16 usStartNo, HI_U8 ucNodesCnt, HI_U8 **ppucBuffer, HI_U16 *pusLength)
{
    HI_U8 *pData = HI_NULL;
    HI_U16 usFailedNum = mrsGetRemoteUpgCtx()->stUpgNumInfo.usFailNum;
    HI_U8 ucAckNum = 0;
    HI_U16 usDataLen = MRS_CSG_FILE_FAILED_INFO_FIX_LEN;

    if (((usFailedNum == 0) && (!mrsGetRemoteUpgCtx()->pucFailedInfo)) || (usStartNo > usFailedNum))
    {
        ucAckNum = 0;
    }
    else
    {
        if ((usStartNo + ucNodesCnt) > usFailedNum)
        {
            ucAckNum = (HI_U8)(usFailedNum - usStartNo);
        }
        else
        {
            ucAckNum = ucNodesCnt;
        }
    }

    usDataLen += ucAckNum * HI_METER_ADDR_LEN;
    pData = mrsToolsMalloc(usDataLen);
    if (!pData)
    {
        return HI_ERR_NOT_ENOUGH_MEMORY;
    }

    (hi_void)memset_s(pData, usDataLen, 0, usDataLen);

    *ppucBuffer = pData;
    *pusLength = usDataLen;

    (hi_void)memcpy_s(pData, sizeof(usFailedNum), &usFailedNum, sizeof(usFailedNum));
    pData += sizeof(usFailedNum);
    *pData = ucAckNum;
    pData ++;

    if (mrsGetRemoteUpgCtx()->pucFailedInfo)
    {
        (hi_void)memcpy_s(pData, ucAckNum * HI_METER_ADDR_LEN, mrsGetRemoteUpgCtx()->pucFailedInfo + usStartNo * HI_METER_ADDR_LEN, ucAckNum * HI_METER_ADDR_LEN);
    }

    return HI_ERR_SUCCESS;
}

//查询文件传输失败节点
HI_U32 mrsCsg_AFN07_E8030705(HI_INOUT HI_VOID * pParam)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    MRS_CSG_IND_FRAME_STRU stIndFrame = {0};
    AFN_DI_UL_PARAM  *pstAfnParam = (AFN_DI_UL_PARAM *)pParam;
    MRS_CSG_FRAME_STRU *pstFrame = pstAfnParam->pstFrame;
    HI_U8 * pucContent;
    HI_U16 usContentLen = 0;

    HI_U16 usStartNo = 0;
    HI_U8 ucNodesCnt = 0;

    HI_DIAG_LOG_MSG_E0(__LINE__, HI_DIAG_MT("【CSG】【mrsCsg_AFN07_E8000704】//查询文件处理进度 E8000704 start"));

    (hi_void)memset_s(&stIndFrame, sizeof(stIndFrame), 0, sizeof(stIndFrame));

    pucContent = pstFrame->aucAppData;
    usContentLen = pstFrame->usAppDataLen;

    do
    {
        //载波从节点信息
        if (usContentLen != MRS_CSG_FILES_FAILED_FIX_LEN)
        {
            ulRet = HI_ERR_BAD_DATA;
            break;
        }

        (hi_void)memcpy_s(&usStartNo, sizeof(usStartNo), pucContent, sizeof(usStartNo));
        ucNodesCnt = pucContent[ sizeof(usStartNo)];

        if (ucNodesCnt > MRS_3762_F10_NODE_MAX_NUM)
        {
            // 从节点数量超过最大限制，按照32处理
            ucNodesCnt = MRS_3762_F10_NODE_MAX_NUM;
        }

    }while(HI_FALSE);

    stIndFrame.ucAfn = pstFrame->ucAfn;
    stIndFrame.ulDI = MRS_CSG_DI_IND_UPG_FAILED_PROC;
    stIndFrame.ucSeq = pstFrame->ucSeq;
    stIndFrame.ucPrm = MRS_CSG_PLC_CTRL_PRM_SLAVE;
    stIndFrame.ucAddrFlag = PROTOCSG_CTRL_ADD_BIT_N;

    ulRet = mrsCsgGetFileFailedInfo(usStartNo, ucNodesCnt, &stIndFrame.pAppData, &stIndFrame.usAppDataLen);
    if (HI_ERR_SUCCESS != ulRet)
    {
        return HI_ERR_FAILURE;
    }

    ulRet = mrsCreateCsgFrame(&stIndFrame, pstAfnParam->ppucOutBuffer, pstAfnParam->pusBufferLen);

    mrsToolsFree(stIndFrame.pAppData);

    return ulRet;
}

//PLC灵敏度测试
HI_U32 mrsCsg_AFNF0_E803F001(HI_INOUT HI_VOID * pParam)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    AFN_DI_UL_PARAM  *pstAfnParam = (AFN_DI_UL_PARAM *)pParam;
    MRS_CSG_FRAME_STRU *pstFrame = pstAfnParam->pstFrame;
    APP_CSG_PLC_TEST_STRU * pstRecvData = (APP_CSG_PLC_TEST_STRU * )pstFrame->aucAppData;
    HI_U16 usDataLen = 0;
    HI_PBYTE pucPayload = HI_NULL;
    HI_U16 usPayloadLen = 0;
    MRS_PLC_FRAME_DATA_STRU stPlcData = {0};
    MRS_CSG_PLC_FRAME_HEAD *pstFrameHead = HI_NULL;
    HI_U8 ucDeny = MRS_CSG_DENY_COMMUNICATION_OVERTIME;

    do
    {
        if (pstFrame->ucAddrFlag == PROTOCSG_CTRL_ADD_BIT_N)
        {
            ulRet = HI_ERR_BAD_DATA;
            ucDeny = MRS_CSG_DENY_ERROR_LENGTH;
            break;
        }

        usDataLen = pstRecvData->usDataLen;
        usPayloadLen = sizeof(MRS_CSG_PLC_FRAME_HEAD) +sizeof(APP_CSG_PLC_TEST_STRU) + usDataLen;
        pucPayload = (HI_U8 *)mrsToolsMalloc(usPayloadLen);
        if (!pucPayload)
        {
            ulRet = HI_ERR_MALLOC_FAILUE;
            break;
        }

        (hi_void)memset_s(pucPayload, usPayloadLen, 0, usPayloadLen);
        (hi_void)memset_s(&stPlcData, sizeof(stPlcData), 0, sizeof(stPlcData));

        pstFrameHead = (MRS_CSG_PLC_FRAME_HEAD *)pucPayload;
        pstFrameHead->usFrameType = MRS_CSG_PLC_CTRL_TYPE_TRANS;
        pstFrameHead->usExtendBit = MRS_CSG_PLC_CTRL_EXTEND_BIT_N;
        pstFrameHead->usRespBit = MRS_CSG_PLC_CTRL_RESP_BIT_Y;
        pstFrameHead->usPrm = MRS_CSG_PLC_CTRL_PRM_HOST;
        pstFrameHead->usDir = MRS_CSG_PLC_CTRL_DIR_DL;
        pstFrameHead->ucSrvId = MRS_CSG_PLC_SRV_ID_PLC_TEST;
        pstFrameHead->ucVersion = MRS_CSG_PLC_PROTO_VERSION;
        pstFrameHead->usSeq = mrsCcoGetContext()->usPlcSeq++;
        pstFrameHead->usDataLength = sizeof(APP_CSG_PLC_TEST_STRU) +usDataLen;

        (hi_void)memcpy_s(pstFrameHead->aucData, pstFrameHead->usDataLength, pstRecvData, pstFrameHead->usDataLength);
        stPlcData.usId = ID_MRS_CMD_CSG_DATA_TRANSMIT_PLC;
        stPlcData.pucPayload = pucPayload;
        stPlcData.usPayloadLen = usPayloadLen;
        mrsToolsConvertMeter2Mac(pstFrame->aucDstAddr, MRS_PROTO645_VER_1997, stPlcData.ucMac);

        MRS_SendPlcFrame(&stPlcData);

        mrsToolsFree(pucPayload);
        pucPayload = HI_NULL;
    } while(HI_FALSE);

    if (HI_ERR_SUCCESS != ulRet)
    {
        mrsCsgProtoDenyFrame(&ucDeny, pstAfnParam);
    }

    return ulRet;
}

//过零检测
HI_U32 mrsCsg_AFNF0_E803F002(HI_INOUT HI_VOID * pParam)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    AFN_DI_UL_PARAM  *pstAfnParam = (AFN_DI_UL_PARAM *)pParam;
    MRS_CSG_FRAME_STRU *pstFrame = pstAfnParam->pstFrame;
    HI_U8 * pucContent = pstFrame->aucAppData;
    HI_U16 usContentLen = pstFrame->usAppDataLen;

    do
    {
        EQUIP_ZERO_CROSS_REQ_STRU req;

        if (usContentLen != sizeof(EQUIP_ZERO_CROSS_REQ_STRU))
        {
            ulRet = HI_ERR_BAD_DATA;
            break;
        }

        (hi_void)memcpy_s(&req, sizeof(req), pucContent, sizeof(req));
        if ((req.level > 1) || (req.phase > 2)) {
            ulRet = HI_ERR_INVALID_PARAMETER;
            break;
        }

        mrs_zero_cross_set_info(req.phase | (req.edge << 2));

        ulRet = HI_MDM_CollectNtb(HI_TRUE, req.level, req.phase + PHASE_A, req.edge, mrsZeroCrossCheckNotify);
        if (HI_ERR_SUCCESS != ulRet)
        {
            ulRet = HI_ERR_FAILURE;
            break;
        }

        MRS_StartTimer(MRS_CCO_TIMER_ZERO_CROSS, MRS_STA_TIME_ZERO_CROSS, HI_SYS_TIMER_ONESHOT);
        return HI_ERR_SUCCESS;
    } while(HI_FALSE);

    mrsDutCsgReplyZeroCross(ulRet, HI_NULL);
    return ulRet;
}

HI_U32 mrsCsgPlcTestProc(HI_PVOID pParam, HI_U16 usSize)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U16 usPayloadLen = 0;
    HI_U8 * pucPayload = HI_NULL;
    HI_U8 *pucAppData = HI_NULL;

    MRS_PLC_FRAME_DATA_STRU *pstPlcFrame = (MRS_PLC_FRAME_DATA_STRU *)pParam;
    MRS_CSG_PLC_FRAME_HEAD *pstFrameHead = (MRS_CSG_PLC_FRAME_HEAD *)pstPlcFrame->pucPayload;

    APP_CSG_PLC_TEST_STRU * pstPlcData = (APP_CSG_PLC_TEST_STRU *)pstFrameHead->aucData;
    MRS_CSG_IND_FRAME_STRU stIndFrame = {0};

    if(usSize != pstPlcData->usExpLen + sizeof(APP_CSG_PLC_TEST_STRU) + sizeof(MRS_CSG_PLC_FRAME_HEAD) + sizeof(MRS_PLC_FRAME_DATA_STRU)){
        return HI_ERR_INVALID_PARAMETER;
    }
    HI_UNREF_PARAM(usSize);

    (hi_void)memset_s(&stIndFrame, sizeof(stIndFrame), 0, sizeof(stIndFrame));
    stIndFrame.ucAfn = MRS_CSG_AFN_DEBUG_F0H;
    stIndFrame.ulDI = MRS_CSG_DI_IND_PLC_TEST;
    stIndFrame.ucSeq = mrsCcoGetContext()->ucSeq;
    stIndFrame.ucPrm = PROTOCSG_CTRL_PRM_BIT_IND;
    stIndFrame.ucAddrFlag = PROTOCSG_CTRL_ADD_BIT_Y;
    stIndFrame.usAppDataLen = sizeof(pstPlcData->usExpLen)+ pstPlcData->usExpLen;

    stIndFrame.pAppData = mrsToolsMalloc(stIndFrame.usAppDataLen);
    if (!stIndFrame.pAppData)
    {
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_022, HI_DIAG_MT("Malloc failed."));
        return HI_ERR_NOT_ENOUGH_MEMORY;
    }

    (hi_void)memset_s(stIndFrame.pAppData, stIndFrame.usAppDataLen, 0, stIndFrame.usAppDataLen);

    pucAppData = stIndFrame.pAppData;
    (hi_void)memcpy_s(pucAppData, sizeof(pstPlcData->usExpLen), (HI_U8 *)&pstPlcData->usExpLen, sizeof(pstPlcData->usExpLen));
    (hi_void)memcpy_s(pucAppData + sizeof(pstPlcData->usExpLen), pstPlcData->usExpLen, pstPlcData->aucData, pstPlcData->usExpLen);

    ulRet = mrsCreateCsgFrame(&stIndFrame, &pucPayload, &usPayloadLen);

    if (HI_ERR_SUCCESS == ulRet)
    {
        ulRet = MRS_SendMrData(pucPayload, usPayloadLen, HI_DMS_CHL_UART_PORT_APP);
    }

    mrsToolsFree(stIndFrame.pAppData);
    mrsToolsFree(pucPayload);

    return ulRet;
}

//*****************************************************************************
//*****************************************************************************
#endif

HI_END_HEADER

