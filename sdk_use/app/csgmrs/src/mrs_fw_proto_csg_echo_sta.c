//*****************************************************************************
//
//                  版权所有 (C), 2001-2014, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_fw_proto_csg_echo_sta.c
//  版 本 号   : V1.0 
//  作    者   : cuiate/c00233580
//  生成日期   : 2015-12-26
//  功能描述   : TODO: ...
//               
//  函数列表   : TODO: ...
//  修改历史   : 
//  1.日    期 : 2015-12-26
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
#include "mrs_srv_common.h"
#include "mrs_srv_sta_queue.h"
#include "mrs_srv_sta_baudrate.h"
#include "mrs_srv_parallel_sta.h"
#include "mrs_srv_sta_event.h"
#include "mrs_srv_sta.h"
#include "mrs_fw_proto_csg_echo_sta.h"
#include "mrs_srv_sta_searchmeter.h"
#include "mrs_srv_clti_searchmeter.h"
#include "mrs_srv_clti_searchmeter_csg.h"
#include "mrs_srv_collector.h"
#include "mrs_srv_csg_plc.h"
#include "mrs_srv_csg_plc_sta.h"
#include "mrs_srv_cco_queue.h"
#include "mrs_srv_clti_upg_csg.h"

HI_START_HEADER

#if (defined(PRODUCT_CFG_PRODUCT_TYPE_STA) && defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD))


HI_PRV MRS_CSG_AFN_FUNC_MAP g_astStaCsgAfnFuncMap[] =
{
    {0xEA010001, mrsCsg_AFN00_EA010001},    // 确认
    {0xEA010002, mrsCsg_AFN00_EA010002},    // 否认

    {0xEA062101, mrsCsg_AFN21_EA062101},    // 请求表地址个数
    {0xEA032102, mrsCsg_AFN21_EA032102},    // 返回表地址
    {0xEA062103, mrsCsg_AFN21_EA062103},    // 请求采集器地址
    {0xEA062105, mrsCsg_AFN21_EA062105},    // 请求探测状态

    {0xEA032201, mrsCsg_AFN22_EA032201},    // 透传上行数据到采集器应答

    {0xEA002301, mrsCsg_AFN23_EA002301},    // 查询厂商代码和版本信息

    {0xEA062403, mrsCsg_AFN24_EA062403},    // 请求文件信息
    {0xEA062404, mrsCsg_AFN24_EA062404},    // 请求文件处理进度
};


HI_U32 mrsStaProtoCsgEchoInit(HI_VOID)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    
    ulRet = mrsProtoCsgEchoInit();
    if (HI_ERR_SUCCESS == ulRet)
    {
        mrsProtoCsgEchoRegFuncMap(g_astStaCsgAfnFuncMap, MRS_TOOLS_ALEN(g_astStaCsgAfnFuncMap));
    }
    
    return ulRet;
}


HI_U32 mrsStaProtoCsgEchoDeinit(HI_VOID)
{
    return mrsProtoCsgEchoDeinit();
}


// 确认
HI_U32 mrsCsg_AFN00_EA010001(HI_INOUT HI_VOID *pParam)
{
    MRS_STA_SRV_CTX_STRU * pstStaCtx = mrsStaGetContext();
    MRS_STA_ITEM * pstItem = HI_NULL;

    HI_UNREF_PARAM(pParam);

    pstItem = mrsSrvQueueTop(&pstStaCtx->stQueModule.stMrQueue);
    if (!pstItem)
    {
        return HI_ERR_SUCCESS;
    }

    switch (pstItem->id)
    {
    case MRS_STA_ITEM_ID_CSG_CLTI_SEARCH_METER:
        {
            HI_U8 ucStatus = mrsCsgCltIGetStatus();
            MRS_CSG_CLTI_SM_CONTEXT *pstCltISmCtx = mrsCsgCltiCtx();

            if (MRS_CSG_CLTI_STATUS_QRY_DETECT_LIST != ucStatus)
            {
                break;
            }

            mrsStaStopTtlTimer(pstItem);
            mrsStaTryDeQueue(&pstStaCtx->stQueModule, mrsStaQueueFree);

            MRS_StopTimer(MRS_STA_TIMER_CSG_CLTI_QUEUE);
            MRS_StopTimer(MRS_STA_TIMER_CSG_CLTI_SM_QUERY);
            MRS_StopTimer(MRS_STA_TIMER_CSG_QRY_DETECT_STATUS);
            
            mrsToolsFree(pstCltISmCtx->pucFrame);
            pstCltISmCtx->usFrameLength = 0;
            pstCltISmCtx->ucTry = 0;
            pstCltISmCtx->ucStatus = MRS_CSG_CLTI_STATUS_IDLE;
            
            if (mrsCsgCltIIsDetectStatus())
            {
                MRS_StartTimer(MRS_STA_TIMER_CSG_QRY_DETECT_STATUS, MRS_STA_TIME_CSG_QRY_DETECT_STATUS, HI_SYS_TIMER_ONESHOT);
            }
            else
            {
                return mrsCsgCltIQueryMeterInf();
            }
        }
        break;

    case MRS_STA_ITEM_ID_CSG_CLTI_UPG_DEVICE:
        {
            HI_U8 ucProcess = mrsCsgCltIUpgGetProcess();

            if (MRS_CSG_CLTI_UPG_START== ucProcess)
            {
                MRS_CSG_CLTI_UPG_MODULE_STRU *pstCltIUpgCtx = mrsCsgCltIUpgGetCtx();
            
                mrsStaTryDeQueue(&pstStaCtx->stQueModule, mrsStaQueueFree);
                MRS_StopTimer(MRS_STA_TIMER_CSG_CLTI_UPG);
            
                mrsToolsFree(pstCltIUpgCtx->pucFrame);
                pstCltIUpgCtx->usFrameLength = 0;
                pstCltIUpgCtx->ucTry = 0;
            
                // 初始化文件传输信息
                mrsCsgCltIUpgSegInit();
            
                return mrsCsgCltIUpgTransmitProc(); // 启动文件传输
            }
            else if (MRS_CSG_CLTI_UPG_TRANSIMIT == ucProcess)
            {
                MRS_CSG_CLTI_UPG_MODULE_STRU *pstCltIUpgCtx = mrsCsgCltIUpgGetCtx();
            
                mrsStaTryDeQueue(&pstStaCtx->stQueModule, mrsStaQueueFree);
                MRS_StopTimer(MRS_STA_TIMER_CSG_CLTI_UPG);
                mrsToolsFree(pstCltIUpgCtx->pucFrame);
                pstCltIUpgCtx->usFrameLength = 0;
                pstCltIUpgCtx->ucTry = 0;
            
                if (pstCltIUpgCtx->stCurSeg.usCurrNo + 1 == pstCltIUpgCtx->stItem.usTotalSeg)
                {   
                    mrsCsgCltIUpgEndProc(HI_MDM_DEVICE_UPG_STATUS_SUCCESS);
                }
                else
                {
                    mrsCsgCltIUpgGetCurSeg();
                    return mrsCsgCltIUpgTransmitProc();     // 启动文件传输
                }
            }
        }
        break;

    default:
        break;
    }

    return HI_ERR_SUCCESS;
}


// 否认
HI_U32 mrsCsg_AFN00_EA010002(HI_INOUT HI_VOID *pParam)
{
    MRS_STA_SRV_CTX_STRU * pstStaCtx = mrsStaGetContext();
    MRS_STA_ITEM * pstItem = HI_NULL;

    HI_UNREF_PARAM(pParam);
    
    pstItem = mrsSrvQueueTop(&pstStaCtx->stQueModule.stMrQueue);
    if (!pstItem)
    {
        return HI_ERR_SUCCESS;
    }

    switch (pstItem->id)
    {
    case MRS_STA_ITEM_ID_CSG_CLTI_SEARCH_METER:
        {
            HI_U8 ucStatus = mrsCsgCltIGetStatus();
            MRS_CSG_CLTI_SM_CONTEXT *pstCltISmCtx = mrsCsgCltiCtx();
            
            if (MRS_CSG_CLTI_STATUS_QRY_DETECT_LIST != ucStatus)
            {
                break;
            }

            mrsStaStopTtlTimer(pstItem);
            mrsStaTryDeQueue(&pstStaCtx->stQueModule, mrsStaQueueFree);

            MRS_StopTimer(MRS_STA_TIMER_CSG_CLTI_QUEUE);
            MRS_StopTimer(MRS_STA_TIMER_CSG_CLTI_SM_QUERY);
            MRS_StopTimer(MRS_STA_TIMER_CSG_QRY_DETECT_STATUS);
            
            mrsToolsFree(pstCltISmCtx->pucFrame);
            pstCltISmCtx->usFrameLength = 0;
            pstCltISmCtx->ucTry = 0;
            pstCltISmCtx->ucStatus = MRS_CSG_CLTI_STATUS_IDLE;
            
            if (mrsCsgCltIIsDetectStatus())
            {
                mrsCsgCltIQueryDetectStatus();
            }
        }
        break;

    case MRS_STA_ITEM_ID_CSG_CLTI_UPG_DEVICE:
        mrsCsgCltIUpgEndProc(HI_MDM_DEVICE_UPG_STATUS_FAIL);
        mrsStaTryDeQueue(&pstStaCtx->stQueModule, mrsStaQueueFree);
        MRS_StopTimer(MRS_STA_TIMER_CSG_CLTI_UPG);
        break;

    default:
        break;
    }

    return HI_ERR_SUCCESS;
}


// 请求表地址个数
HI_U32 mrsCsg_AFN21_EA062101(HI_INOUT HI_VOID *pParam)
{
    HI_UNREF_PARAM(pParam);
    return HI_ERR_NOT_SUPPORT;
}


// 返回表地址
HI_U32 mrsCsg_AFN21_EA032102(HI_INOUT HI_VOID *pParam)
{
    AFN_DI_UL_PARAM * pstAfnParam = (AFN_DI_UL_PARAM *)pParam;
    MRS_CSG_FRAME_STRU *pstFrame = pstAfnParam->pstFrame;

    return mrsCsgCltIQueryMeterInfRx(pstFrame);
}


// 请求采集器地址
HI_U32 mrsCsg_AFN21_EA062103(HI_INOUT HI_VOID *pParam)
{
    static HI_BOOL bFirst = HI_TRUE;
    AFN_DI_UL_PARAM * pstAfnParam = (AFN_DI_UL_PARAM *)pParam;
    MRS_CSG_FRAME_STRU *pstFrame = pstAfnParam->pstFrame;
    HI_U8 aucDutAddr[] = DUT_NORMAL_MODE_ADDR;
    HI_U8 ucDevAddr[HI_METER_ADDR_LEN] = {0};

    mrsHexInvert(aucDutAddr, sizeof(aucDutAddr));
    if (!bFirst)
    {
        return HI_ERR_SUCCESS;
    }

    //判断接收的报文长度是否符合预期
    if (pstFrame->usAppDataLen != HI_METER_ADDR_LEN)
    {
        HI_DIAG_LOG_MSG_E1(0, HI_DIAG_MT("[Csg_AFN21_EA062103]app len error:"), pstFrame->usAppDataLen);        
        return HI_ERR_SUCCESS;
    }

    bFirst = HI_FALSE;

    mrsStaSetState(MRS_STA_STATE_MR);
    MRS_StopTimer(MRS_STA_TIMER_TEST);

    mrsStaSetCltiType(HI_TRUE);

    if ((pstFrame->ucAddrFlag == PROTOCSG_CTRL_ADD_BIT_Y)
        && (memcmp(pstFrame->aucSrcAddr, aucDutAddr, HI_METER_ADDR_LEN) == 0))//判断特殊地址
    {
        mrsStaGetContext()->ucDutFlag = HI_TRUE;
        (hi_void)memcpy_s(ucDevAddr, HI_METER_ADDR_LEN, pstFrame->aucAppData, HI_METER_ADDR_LEN);
        mrsHexInvert(ucDevAddr, HI_METER_ADDR_LEN);    // 逆序
        mrsStaSetMeterToMac(ucDevAddr, STA_TYPE_CLTI, HI_MDM_METER_MAC);
    }
    else
    {
        mrsSetCollectorAddr(pstFrame->aucAppData);
        mrsCltiWriteCLTAddress2Nv(pstFrame->aucAppData);

        mrsStaSearchStart(HI_TRUE);
    }

    return HI_ERR_SUCCESS;
}


// 请求探测状态
HI_U32 mrsCsg_AFN21_EA062105(HI_INOUT HI_VOID *pParam)
{
    AFN_DI_UL_PARAM * pstAfnParam = (AFN_DI_UL_PARAM *)pParam;
    MRS_CSG_FRAME_STRU *pstFrame = pstAfnParam->pstFrame;

    return mrsCsgCltIQueryDetectStatusRx(pstFrame);
}


// 透传上行数据到采集器应答
HI_U32 mrsCsg_AFN22_EA032201(HI_INOUT HI_VOID *pParam)
{
    MRS_STA_SRV_CTX_STRU * pstStaCtx = mrsStaGetContext();
    MRS_STA_ITEM * pstItem = HI_NULL;
    AFN_DI_UL_PARAM * pstAfnParam = (AFN_DI_UL_PARAM *)pParam;
    MRS_CSG_FRAME_STRU *pstFrame = pstAfnParam->pstFrame;
    MRS_CSG_TRANSIMIT_UL_STRU *pUlFrame = HI_NULL;
    HI_U8 *pFrameData = HI_NULL;
    HI_U16 usDataLen = 0;
    HI_U32 ulRet = HI_ERR_SUCCESS;
    MRS_645_FRAME_STRU *pst645Hrd = HI_NULL;
    
    pstItem = mrsSrvQueueTop(&pstStaCtx->stQueModule.stMrQueue);

    // 队列空 or 非PLC抄表 非转发 非探测抄表
    if (!pstItem
        || ((MRS_STA_ITEM_ID_CSG_TRANS_XR != pstItem->id) && (MRS_STA_ITEM_ID_TRANSMIT != pstItem->id)
            && !((MRS_STA_ITEM_ID_CSG_CLTI_SEARCH_METER == pstItem->id) && ((MRS_CSG_CLTI_STATUS_FIRST_07 == pstItem->option) || (MRS_CSG_CLTI_STATUS_FIRST_97 == pstItem->option)))))
    {
        return HI_ERR_FAILURE;
    }

    pFrameData = pstFrame->aucAppData;
    usDataLen = MRS_COMBIN16(pFrameData[0], pFrameData[1]);
    pFrameData += 2;

    if(usDataLen + 2 != pstFrame->usAppDataLen){
        return HI_ERR_FAILURE;
    }
    if (MRS_STA_ITEM_ID_CSG_CLTI_SEARCH_METER == pstItem->id)
    {
        pUlFrame = (MRS_CSG_TRANSIMIT_UL_STRU *)(pstItem->buf + MRS_PREFIX_FE_SIZE);
    }
    else
    {
        pUlFrame = (MRS_CSG_TRANSIMIT_UL_STRU *)pstItem->buf;
    }

    if (pUlFrame->ucSeq != pstFrame->ucSeq)
    {
        return HI_ERR_BAD_DATA;
    }

    mrsStaStopTtlTimer(pstItem);

    switch (pstItem->from)
    {
    case MRS_STA_QUEUE_FROM_REMOTE:
        mrsStaDiag2HSO(pFrameData, usDataLen, HI_DIAG_CMD_INSTANCE_IREMOTE);
        break;

    case MRS_STA_QUEUE_FROM_REMOTE_HSO:
        HI_MDM_ReportPacket(HI_STA_TRANSMIT, HI_DIAG_CMD_INSTANCE_IREMOTE, pFrameData, usDataLen, HI_TRUE);
        break;

    case MRS_STA_QUEUE_FROM_LOCAL:
        HI_MDM_ReportPacket(HI_STA_TRANSMIT, HI_DIAG_CMD_INSTANCE_LOCAL, pFrameData, usDataLen, HI_TRUE);
        break;

    case MRS_STA_QUEUE_FROM_STA:
        // mrsCsgCltITryFirstRx中会入队列，所以这里要先出队列
        mrsStaTryDeQueue(&pstStaCtx->stQueModule, mrsStaQueueFree);
        // 这里使用采集器应答报文中的原始地址
        mrsCsgCltITryFirstRx(pstFrame->aucSrcAddr);
        return HI_ERR_SUCCESS;

    default:
        mrsDfxXrUartRx();
        mrsStaDfxRmUartRx(pstItem->id,  pFrameData, usDataLen);

        pst645Hrd = (MRS_645_FRAME_STRU *)mrsToolsMalloc(sizeof(MRS_645_FRAME_STRU));
        if (!pst645Hrd)
        {
            break;
        }

        (hi_void)memset_s(pst645Hrd, sizeof(MRS_645_FRAME_STRU), 0, sizeof(MRS_645_FRAME_STRU));
        ulRet = MRS_Proto645Dec(pFrameData, usDataLen, pst645Hrd);
        if (HI_ERR_SUCCESS == ulRet)
        {
            MRS_STA_SEARCH_CTX *pstSmCtx = mrsStaSearchCtx();

            mrsCltiSearchMeterInsert(pstSmCtx, pst645Hrd->ucAddr, MRS_PROTO645_VER_1997);
            mrsStaDfxRmRxDeny(pstItem->id, pst645Hrd->stCtrl.ucSlaveFlag);
            mrsStaDfxRmRxErrData(pstItem->id, pst645Hrd->ucDataRealm, pst645Hrd->ucDataRealmLen);
        }

        mrsToolsFree(pst645Hrd);

        mrsStaCsgTransmitXr2CCO(pFrameData, usDataLen, pstItem);
        mrsStaFrameBufInsert(pstItem->id, pstItem->seq, usDataLen, pFrameData, MRS_PROTO645_VER_2007);
        break;
    }

    mrsStaTryDeQueue(&pstStaCtx->stQueModule, mrsStaQueueFree);

    return HI_ERR_SUCCESS;
}


// 查询厂商代码和版本信息
HI_U32 mrsCsg_AFN23_EA002301(HI_INOUT HI_VOID *pParam)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    AFN_CSG_MANU_INFO_STRU stManuInf;

    (hi_void)memset_s(&stManuInf, sizeof(stManuInf), 0, sizeof(stManuInf));

    ulRet = mrsCsgProtoManuInfo(&stManuInf);
    if (HI_ERR_SUCCESS != ulRet)
    {
        return ulRet;
    }

    return MRS_ProtoCsgEncode((HI_U8 *)&stManuInf, sizeof(stManuInf), pParam);
}


// 请求文件信息
HI_U32 mrsCsg_AFN24_EA062403(HI_INOUT HI_VOID *pParam)
{
    MRS_STA_SRV_CTX_STRU * pstStaCtx = mrsStaGetContext();
    AFN_DI_UL_PARAM * pstAfnParam = (AFN_DI_UL_PARAM *)pParam;
    MRS_CSG_FRAME_STRU *pstFrame = pstAfnParam->pstFrame;
    MRS_STA_ITEM * pstItem = HI_NULL;
    HI_U16 usPayLen = pstFrame->usAppDataLen;
    HI_U8 * pucContent = pstFrame->aucAppData;
    MRS_CSG_UPG_ITEM_STRU stItem = {0};
    HI_U16 usCurSeg = 0;

    pstItem = mrsSrvQueueTop(&pstStaCtx->stQueModule.stMrQueue);
    if (!pstItem || pstItem->id != MRS_STA_ITEM_ID_CSG_CLTI_UPG_DEVICE)
    {
        return HI_ERR_FAILURE;
    }

    if (usPayLen != MRS_CSG_UPG_IND_LEN_FILE_INFO)
    {
        return HI_ERR_BAD_DATA;
    }

    MRS_StopTimer(MRS_STA_TIMER_CSG_CLTI_UPG);

    (hi_void)memset_s((HI_U8 *)&stItem, sizeof(MRS_CSG_UPG_ITEM_STRU), 0, sizeof(MRS_CSG_UPG_ITEM_STRU));
    stItem.ucFileId = pucContent[0];
    pucContent++;

    (hi_void)memcpy_s(stItem.aucDestAddr, HI_METER_ADDR_LEN, pucContent, HI_METER_ADDR_LEN);
    pucContent += HI_METER_ADDR_LEN;

    (hi_void)memcpy_s(&stItem.usTotalSeg, sizeof(stItem.usTotalSeg), pucContent, sizeof(stItem.usTotalSeg));
    pucContent += sizeof(stItem.usTotalSeg);

    (hi_void)memcpy_s(&stItem.ulFileLength, sizeof(stItem.ulFileLength), pucContent, sizeof(stItem.ulFileLength));
    pucContent += sizeof(stItem.ulFileLength);

    (hi_void)memcpy_s(&stItem.usTotalCs, sizeof(stItem.usTotalCs), pucContent, sizeof(stItem.usTotalCs));
    pucContent += sizeof(stItem.usTotalCs);

    (hi_void)memcpy_s(&usCurSeg,sizeof(usCurSeg), pucContent,sizeof(usCurSeg));

    mrsStaTryDeQueue(&pstStaCtx->stQueModule, mrsStaQueueFree);

    // 判断是否要从头开始或者继续传输
    if (HI_ERR_SUCCESS == mrsCsgCltIUpgJudgeRestart(&stItem, usCurSeg))
    {
        // 传输完成
        mrsCsgCltIUpgEndProc(HI_MDM_DEVICE_UPG_STATUS_SUCCESS);
    }

    return HI_ERR_SUCCESS;
}


// 请求文件处理进度
HI_U32 mrsCsg_AFN24_EA062404(HI_INOUT HI_VOID *pParam)
{
    AFN_DI_UL_PARAM * pstAfnParam = (AFN_DI_UL_PARAM *)pParam;
    MRS_CSG_FRAME_STRU *pstFrame = pstAfnParam->pstFrame;

    HI_U16 usPayLen = pstFrame->usAppDataLen;
    HI_U8 *pFrameData = pstFrame->aucAppData;

    if (usPayLen != MRS_CSG_UPG_IND_LEN_PROCESS)
    {
        return HI_ERR_BAD_DATA;
    }

    mrsCsgCltIUpgFileStatusProc(pFrameData[0]);

    return HI_ERR_SUCCESS;
}


#endif
HI_END_HEADER

