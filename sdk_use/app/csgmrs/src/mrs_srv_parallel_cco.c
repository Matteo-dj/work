//*****************************************************************************
//
//                  版权所有 (C), 1998-2011, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_srv_parallel_cco.c
//  版 本 号   : V1.0
//  作    者   : liuxipeng/KF54842
//  生成日期   : 2012-12-22
//  功能描述   : 并发读表模块函数实现
//
//  函数列表   :
//  修改历史   :
//  1.日    期 : 2012-12-22
//    作    者 : liuxipeng/KF54842
//    修改内容 : 创建文件
//
//*****************************************************************************

//*****************************************************************************
// PROJECT   :
// SUBSYSTEM :
// MODULE    :
// OWNER     :
//*****************************************************************************

#include "mrs_common.h"
#include "mrs_cmd_msg.h"
#include "mrs_fw_tools.h"
#include "mrs_fw_log.h"
#include "mrs_fw_proto376_2_echo.h"
#include "mrs_fw_proto376_2_frame.h"
#include "mrs_dfx.h"
#include "mrs_srv_mrslog.h"
#include "mrs_srv_readmeter_manage.h"
#include "mrs_srv_cco_macmap.h"
#include "mrs_srv_parallel_cco.h"
#include "mrs_srv_cco.h"
#include "mrs_srv_list.h"
#include "mrs_srv_cco_mr_task.h"
#include "mrs_fw_proto_csg_echo.h"
#include "mrs_srv_cco_archives.h"

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)

HI_PRV HI_U32 mrsParallelPlcRespProc(P_MRS_RM_REQ_DATA pstData);

// 并发读表初始化(必须在读表管理模块初始化后执行)
HI_U32 mrsParallelReadMeterInit(HI_VOID)
{
    HI_U32 ret = HI_ERR_SUCCESS;

    ret = mrsRmRegisterStrategy(MRS_RM_TYPE_AFNF1H_F1,
                                EN_MRS_RETRY_CFG_03,
                                EN_MRS_RM_RETRY_STRATEGY_02,
                                EN_MRS_PARALLEL_POOL_LOW,
                                mrsParallelPlcRespProc);

    return ret;
}


#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
HI_PRV HI_U32 mrsParallelPlcRespProc(P_MRS_RM_REQ_DATA pstData)
{
    MRS_CCO_MR_TASK_ITEM_STRU *pstItem = HI_NULL;
    HI_U32 ulRet = HI_ERR_SUCCESS;

    if (!pstData || !pstData->pvParam)
    {
        mrsCcoMrTaskNotify();
        return HI_ERR_FAILURE;
    }

    pstItem = (MRS_CCO_MR_TASK_ITEM_STRU *)pstData->pvParam;
    if (pstData->pucData)
    {
        ulRet = mrsCcoMrTaskReportFrameProc(pstItem, pstData->pucData, pstData->usDataLen);
    }
    else
    {
        ulRet = mrsCcoMrTaskReportItemNak(pstItem, pstData->ucAddr, MRS_CSG_TASK_STATUS_OTHER_ERR);
    }

    if (MRS_CCO_MR_TASK_TYPE_UNICAST == pstItem->ucType)
    {
        mrsCcoMrTaskRemoveNode(pstItem);
    }
    else
    {
        MRS_CCO_MR_TASK_PARAM_MULTICAST *pstMulti = (MRS_CCO_MR_TASK_PARAM_MULTICAST *)&pstItem->unParam.stMulticast;
        HI_U16 usIdx;

        for (usIdx = 0; usIdx < pstMulti->usSubNodeNum; usIdx++)
        {
            HI_U8 *pucAddr = HI_NULL;
            MRS_ARCHIVES_NODE_STRU *pstNode = HI_NULL;

            if (MRS_CCO_MR_TASK_MULTI_ALLMETER == pstMulti->ucType)
            {
                pstNode = mrsSrvArchivesQuery(usIdx);
                if ((pstNode == HI_NULL) || !pstNode->bValid)
                {
                    mrsSrvBitSet(pstMulti->pBitmap, usIdx);
                    continue;
                }

                pucAddr = pstNode->aucNodeAddr;
            }
            else
            {
                pucAddr = pstMulti->pSubNode + HI_METER_ADDR_LEN * usIdx;
            }

            if (mrsToolsMemEq(pucAddr, pstData->ucAddr, HI_METER_ADDR_LEN))
            {
                mrsSrvBitSet(pstMulti->pBitmap, usIdx);
                break;
            }
        }

        if (mrsSrvBitIsFull(pstMulti->pBitmap))
        {
            mrsCcoMrTaskReportItemNak(pstItem, HI_NULL, MRS_CSG_TASK_STATUS_SUCCESS);
            mrsRmRemoveItemFromPool((HI_VOID *)pstItem);
            mrsCcoMrTaskRemoveNode(pstItem);
        }
    }

    mrsCcoMrTaskNotify();

    return ulRet;
}

HI_U32 mrsParallelReadMeterProcCsg(HI_U8 *pucFrame, HI_U16 usLength, HI_U8 *pucAddr, HI_VOID *pvParam)
{
    MRS_RM_REQ_DATA stData = {0};

    if (!pvParam)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    (hi_void)memset_s(&stData, sizeof(stData), 0, sizeof(stData));
    stData.ucAddr       = pucAddr;
    stData.usRmType     = MRS_RM_TYPE_AFNF1H_F1;
    stData.ucProtocol   = MRS_PROTO645_VER_2007;
    stData.ucMrSeq      = 0;
    stData.usDataLen    = usLength;
    stData.pucData      = pucFrame;
    stData.pvParam      = pvParam;

    return mrsRmReqJoinPool(&stData);
}

#else

// 并发读表PLC应答处理
HI_PRV HI_U32 mrsParallelPlcRespProc(P_MRS_RM_REQ_DATA pstData)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    PROTO376_2_FRAME_PRE_STRU stFrame;
    HI_U8 *pucOutBuf = HI_NULL;
    HI_U16 usOutLen = 0;

    if(!pstData)
    {
        return HI_ERR_FAILURE;
    }

    (hi_void)memset_s(&stFrame, sizeof(stFrame), 0, sizeof(stFrame));

    stFrame.ucModuleFlag = 1;
    stFrame.ucSeq = pstData->ucMrSeq;
    stFrame.ucIs645 = 1;
    stFrame.ucProtocol = pstData->ucProtocol;
    (hi_void)memcpy_s(stFrame.ucSrcAdd, HI_METER_ADDR_LEN, pstData->ucAddr, HI_METER_ADDR_LEN);

    stFrame.stAfnData.usDataLen = pstData->usDataLen;
    stFrame.stAfnData.pData = pstData->pucData;

    stFrame.stAfnData.ucAFN   = MRS_AFN((pstData->usRmType & 0xFF00) >> 8);     // AFN = F1H Fn=1
    stFrame.stAfnData.ucDT[0] = MRS_AFN_FN_LO(pstData->usRmType & 0x00FF);
    stFrame.stAfnData.ucDT[1] = MRS_AFN_FN_HI(pstData->usRmType & 0x00FF);

    ret = mrsCreate3762Frame(&stFrame, &pucOutBuf, &usOutLen, HI_NULL);
    if(ret == HI_ERR_SUCCESS)
    {
        ret = MRS_SendMrData(pucOutBuf, usOutLen, HI_DMS_CHL_UART_PORT_APP);
        mrsDfxPrUartTx();
    }

    mrsToolsFree(pucOutBuf);

    return ret;
}


// 集中器下行并发数据处理
HI_U32 mrsParallelReadMeterProc(HI_IN HI_U8 ucSeq, HI_IN HI_U8 ucProtocol, HI_IN HI_U8 * pucBuf, HI_IN HI_U16 usBufLen)
{
    MRS_RM_REQ_DATA stData = {0};

    (hi_void)memset_s(&stData, sizeof(stData), 0, sizeof(stData));

    stData.ucAddr     = HI_NULL;
    stData.usRmType   = MRS_RM_TYPE_AFNF1H_F1;
    stData.ucProtocol = ucProtocol;
    stData.ucMrSeq    = ucSeq;
    stData.usDataLen  = usBufLen;
    stData.pucData    = pucBuf;

    return mrsRmReqJoinPool(&stData);
}
#endif

#endif


