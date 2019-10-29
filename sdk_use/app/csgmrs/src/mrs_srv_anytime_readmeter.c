//*****************************************************************************
//
//                  版权所有 (C), 1998-2011, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_srv_anytime_readmeter.c
//  版 本 号   : V1.0
//  作    者   : liuxipeng/KF54842
//  生成日期   : 2012-06-04
//  功能描述   : 随抄读表模块函数实现
//
//  函数列表   :
//  修改历史   :
//  1.日    期 : 2012-06-04
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
#include "mrs_fw_proto645.h"
#include "mrs_fw_proto376_2.h"
#include "mrs_fw_proto376_2_echo.h"
#include "mrs_fw_proto376_2_frame.h"
#include "mrs_fw_proto_csg_echo.h"
#include "mrs_fw_tools.h"
#include "mrs_fw_log.h"
#include "mrs_srv_common.h"
#include "mrs_srv_cco_macmap.h"
#include "mrs_srv_list.h"
#include "mrs_srv_cco_event.h"
#include "mrs_srv_cco.h"
#include "mrs_srv_cco_readmeter.h"
#include "mrs_srv_anytime_readmeter.h"
#include "mrs_dfx.h"
#include "mrs_srv_mrslog.h"
#include "mrs_srv_cco_testmode.h"
#include "mrs_srv_cco_detect_rm.h"
#include "mrs_srv_readmeter_manage.h"
#include "mrs_srv_cco_mr_task.h"
HI_START_HEADER

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)

HI_VOID FreePlcData( MRS_CCO_SRV_CTX_STRU * pstCcoCtx );
HI_VOID mrsGetDestAddr(HI_OUT HI_U8 * pucDestAddr,
                       HI_IN HI_U8 * pstAddr, HI_IN HI_U8 ucProtocol, HI_IN HI_U8 * pDataBuff, HI_U16 usDataLen);
HI_U32 mrsCreateReadMeterFramePlc(HI_IN HI_U8 * mac_addr, HI_IN HI_U8 protocol,
                                  HI_IN HI_U8 * data_buff, HI_IN HI_U16 data_len);
HI_U32 mrsCreateReadMeterFrame376_2(HI_IN HI_U8 ucProtocol, HI_IN HI_U8 * pDataBuff, HI_IN HI_U16 usDataLen,
                                    HI_OUT HI_PBYTE * ppOutBuff, HI_OUT HI_U16 * usOutLen);
HI_U32 mrsSendReadMeterFramePlc(HI_VOID);
HI_U32 mrsAFN13F1ReportAck(HI_VOID);
HI_U32 mrsXrReportAck(HI_VOID);
HI_U32 mrsCsgCreateReadMeterFramePlc(HI_U8 *pucMacAddr, HI_U8 *pucSrcAddr, HI_U8 *pucDstAddr, HI_U8 *pucDataBuf, HI_U16 usDataLen);


HI_VOID mrsSetXRStatus(MRS_ANYTIME_RM_STATE_ENUM state)
{
    MRS_CCO_SRV_CTX_STRU *cco_ctx = mrsCcoGetContext();

    cco_ctx->stAnytimeRMModel.enState = state;
    mrsDfxXrStatus(state);
}


// 释放掉备份的PLC数据
HI_VOID FreePlcData( MRS_CCO_SRV_CTX_STRU * pstCcoCtx )
{
    if(pstCcoCtx->stAnytimeRMModel.pstPlcDataDl != HI_NULL)
    {
        if(pstCcoCtx->stAnytimeRMModel.pstPlcDataDl->pucPayload != HI_NULL)
        {
            mrsToolsFree(pstCcoCtx->stAnytimeRMModel.pstPlcDataDl->pucPayload);   // 释放转发数据块
            pstCcoCtx->stAnytimeRMModel.pstPlcDataDl->pucPayload = HI_NULL;
        }

        mrsToolsFree(pstCcoCtx->stAnytimeRMModel.pstPlcDataDl);   // 释放PLC帧备份
        pstCcoCtx->stAnytimeRMModel.pstPlcDataDl = HI_NULL;
    }
}

//*****************************************************************************
// 函数名称: mrsAnyTimeReadMeterInit
// 功能描述: 随抄读表初始化
//
// 参数说明:
//   无
//
// 返 回 值:
//   无
//
// 调用要求:
// 调用举例:
// 作    者: liuxipeng/KF54842 [2012-06-09]
//*****************************************************************************
HI_VOID mrsAnyTimeReadMeterInit(HI_VOID)
{
    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();
    PLC_OVERTIME_INF_STRU* TimeOutInf = mrsGetPlcOverTimeInfAfn13();

    MRS_StopTimer(EN_MRS_SRV_CCO_STA_TIMER);
    MRS_StopTimer(EN_MRS_SRV_CCO_SERVICE_END_TIMER);

    TimeOutInf->Try = 0;   // 与STA通讯超时次数清理
    TimeOutInf->MaxTry = TimeOutInf->MaxTryNvS;
    TimeOutInf->Loop = 0;

    mrsSetXRStatus(EN_MRS_ANYTIME_RM_IDLE);

    FreePlcData(pstCcoCtx);

    pstCcoCtx->stAnytimeRMModel.bIsBC = HI_FALSE;   // 置为非广播式读表

    return;
}


// 获取目标地址-优先使用645报文地址
HI_VOID mrsGetDestAddr(HI_OUT HI_U8 * pucDestAddr,
                       HI_IN HI_U8 * pstAddr, HI_IN HI_U8 ucProtocol, HI_IN HI_U8 * pDataBuff, HI_U16 usDataLen)
{
    HI_U16 pos = 0;
    HI_U16 frame_len = 0;

    (hi_void)memcpy_s(pucDestAddr, HI_METER_ADDR_LEN, pstAddr, HI_METER_ADDR_LEN);

    if (mrsCheck645Proto(ucProtocol)
        && (HI_ERR_SUCCESS == mrsFind645Frame(pDataBuff, (HI_S16)usDataLen, &pos, &frame_len)))
    {
        (hi_void)memcpy_s(pucDestAddr, HI_METER_ADDR_LEN, pDataBuff + pos + MRS_645_FRAME_METERADD_OFFSET, HI_METER_ADDR_LEN);
    }
}


//*****************************************************************************
// 函数名称: mrsCreateReadMeterFramePlc
// 功能描述: 创建读表数据PLC转发帧
//
// 参数说明:
//   pucMacAddr 目的Mac地址
//   ucProtocol 规约类型
//   pDataBuff 待转发的数据帧
//   usDataLen 带转发的数据长度
//
// 返 回 值:
//   无
//
// 调用要求:
// 调用举例:
// 作    者: liuxipeng/KF54842 [2012-06-09]
//*****************************************************************************
HI_U32 mrsCreateReadMeterFramePlc(HI_IN HI_U8 * mac_addr, HI_IN HI_U8 protocol,
                                                  HI_IN HI_U8 * data_buff, HI_IN HI_U16 data_len)
{
    MRS_PLC_FRAME_DATA_STRU * plc_frame_data = HI_NULL;
    MRS_DATA_TRANSMIT_AFN13_DL_STRU * transmit_data = HI_NULL;
    MRS_CCO_SRV_CTX_STRU * cco_ctx = mrsCcoGetContext();
    HI_U8 * payload = HI_NULL;
    HI_U16 payload_len = 0;
    HI_U32 meter_timeout = 0;

    plc_frame_data = (MRS_PLC_FRAME_DATA_STRU *)mrsToolsMalloc(sizeof(MRS_PLC_FRAME_DATA_STRU));
    if(plc_frame_data != HI_NULL)
    {
        (hi_void)memset_s(plc_frame_data, sizeof(MRS_PLC_FRAME_DATA_STRU), 0, sizeof(MRS_PLC_FRAME_DATA_STRU));
    }
    else
    {
        return HI_ERR_NO_MORE_MEMORY;
    }

    payload_len = sizeof(MRS_DATA_TRANSMIT_AFN13_DL_STRU) + data_len;
    payload = mrsToolsMalloc(payload_len);

    if(payload != HI_NULL)
    {
        transmit_data = (MRS_DATA_TRANSMIT_AFN13_DL_STRU *)payload;
        (hi_void)memset_s(transmit_data, payload_len, 0, payload_len);
    }
    else
    {
        mrsToolsFree(plc_frame_data);
        return HI_ERR_NO_MORE_MEMORY;
    }

    // 帧序号自增
    cco_ctx->stAnytimeRMModel.seq++;

    // 费控命令处理
    mrsCtrlCmdProc(data_buff,data_len);

    // 初始化超时及重试次数
    mrsGetMaxTryAfn13(mac_addr);
    mrsGetPlcTimeOutAfn13(mac_addr, HI_FALSE, &meter_timeout);

    // 组织转发数据包
    transmit_data->interface_ver = MRS_AFN13_DL_VER;
    transmit_data->stru_size     = sizeof(MRS_DATA_TRANSMIT_AFN13_DL_STRU);
    transmit_data->protocol      = protocol;        // 获取规约类型
    transmit_data->data_len      = data_len;        // 获取待转发报文长度
    transmit_data->seq           = cco_ctx->stAnytimeRMModel.seq;
    transmit_data->time_out      = (HI_U8)MRS_MS_TO_100MS(meter_timeout); // 单位为百毫秒
    transmit_data->dir           = MRS_PLC_DL_FLG;  // 置下行帧标志
    (hi_void)memcpy_s(transmit_data->data, transmit_data->data_len, data_buff, data_len); // 获取待转发的数据内容

    // 组织PLC包
    plc_frame_data->usId         = ID_MRS_CMD_DATA_TRANSMIT_AFN13_PLC;    // 获取PLC帧命令ID
    (hi_void)memcpy_s(plc_frame_data->ucMac, sizeof(plc_frame_data->ucMac), mac_addr, HI_PLC_MAC_ADDR_LEN);  // 获取Mac地址
    plc_frame_data->usPayloadLen = payload_len;       // 获取转发数据包长度
    plc_frame_data->pucPayload   = (HI_U8 *)payload;  // 获取转发数据包指针

    // 备份数据转发PLC包(重发)
    cco_ctx->stAnytimeRMModel.pstPlcDataDl = plc_frame_data;

#if defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG)
/* BEGIN: liuxipeng/KF54842 2014/2/20   问题单号: DTS2014022003115 */
    mrsLogFrameInfStatTx(data_buff, data_len, (HI_U8)cco_ctx->stAnytimeRMModel.bIsBC);
/* END  : liuxipeng/KF54842 2014/2/20   问题单号: DTS2014022003115 */
#endif

    return HI_ERR_SUCCESS;
}

//*****************************************************************************
// 函数名称: mrsCreateReadMeterFrame376_2
// 功能描述: 创建读表数据上行帧(376.2 AFN-13 F1)
//
// 参数说明:
//   ucProtocol 规约类型
//   pDataBuff 抄读回的数据内容
//   usDataLen 抄读回的数据长度
//   ppOutBuff 组织完成的376.2帧
//   usOutLen  376.2帧长度
//
// 返 回 值:
//   无
//
// 调用要求:
// 调用举例:
// 作    者: liuxipeng/KF54842 [2012-06-09]
// 修    改: cuiate/c00233580  [2013-04-01]
//*****************************************************************************
HI_U32 mrsCreateReadMeterFrame376_2(HI_IN HI_U8 ucProtocol, HI_IN HI_U8 * pDataBuff, HI_IN HI_U16 usDataLen,
                                                  HI_OUT HI_PBYTE * ppOutBuff, HI_OUT HI_U16 * usOutLen)
{
    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();
    HI_U32 ret = HI_ERR_SUCCESS;

    do
    {
        PROTO376_2_FRAME_PRE_STRU stFrame;
        HI_U8 * data_buf = HI_NULL;
        HI_U16 data_len = 0;
        HI_U16 offset = 0;
        HI_U16 comm_delay_len = 0;

        data_len = usDataLen + 2;   // 规约(1) + 长度(1)
        if ((pstCcoCtx->ucProtoVer != PROTO_VER_3762_2009)
            && (pstCcoCtx->stAnytimeRMModel.from == XR_FROM_AFN13_F1))
        {
            // 当前报文本地通信上行时长
            comm_delay_len = 2;
        }

        data_len += comm_delay_len;
        data_buf = mrsToolsMalloc(data_len);
        if (!data_buf)
        {
            ret = HI_ERR_MALLOC_FAILUE;
            break;
        }

        (hi_void)memset_s(data_buf, data_len, 0, data_len);
        if (comm_delay_len > 0)
        {
            data_buf[offset++] = (MRS_AFN_COMM_DELAY % 255);
            data_buf[offset++] = (MRS_AFN_COMM_DELAY / 255);
        }

        data_buf[offset++] = ucProtocol;
        data_buf[offset++] = (HI_U8)usDataLen;
        if (usDataLen > 0)
        {
            (hi_void)memcpy_s(data_buf + offset, data_len - offset, pDataBuff, usDataLen);
        }

        (hi_void)memset_s(&stFrame, sizeof(stFrame), 0, sizeof(stFrame));
        stFrame.ucModuleFlag = 1;
        stFrame.ucIs645 = 0;
        stFrame.ucSeq = (HI_U8)pstCcoCtx->stAnytimeRMModel.seq_1376_2;

        if (mrsCheck645Proto(ucProtocol))
        {
            (hi_void)memcpy_s(stFrame.ucSrcAdd, HI_METER_ADDR_LEN, &pDataBuff[1], HI_METER_ADDR_LEN);
        }

        (hi_void)memcpy_s(stFrame.ucDstAdd, HI_METER_ADDR_LEN, pstCcoCtx->ucMainNodeAddr, HI_METER_ADDR_LEN);
        mrsHexInvert(stFrame.ucDstAdd, sizeof(stFrame.ucDstAdd));

        if(pstCcoCtx->stAnytimeRMModel.from == XR_FROM_AFN13_F1)
        {
            stFrame.stAfnData.ucAFN = MRS_AFN(0x13);
        }
        else
        {
            stFrame.stAfnData.ucAFN = MRS_AFN(0x02);
        }

        stFrame.stAfnData.ucDT[0] = MRS_AFN_FN_LO(1);
        stFrame.stAfnData.ucDT[1] = MRS_AFN_FN_HI(1);
        stFrame.stAfnData.pData = data_buf;
        stFrame.stAfnData.usDataLen = data_len;

        ret = mrsCreate3762Frame(&stFrame, ppOutBuff, usOutLen, stFrame.ucSrcAdd);

        mrsToolsFree(data_buf);
    } while(0);

    return ret;
}

//*****************************************************************************
// 函数名称: mrsSendReadMeterFramePlc
// 功能描述: 发送PLC读表帧
//
// 参数说明:
//   无
//
// 返 回 值:
//   HI_ERR_SUCCESS 发送成功
//
// 调用要求:
// 调用举例:
// 作    者: liuxipeng/KF54842 [2012-06-09]
// 修    改: cuiate/c00233580  [2013-04-01]
//*****************************************************************************
HI_U32 mrsSendReadMeterFramePlc(HI_VOID)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();
    HI_U8 mac[HI_PLC_MAC_ADDR_LEN] = {0};

    if(pstCcoCtx->stAnytimeRMModel.pstPlcDataDl == HI_NULL)
    {
        return HI_ERR_BAD_DATA;
    }

    if (pstCcoCtx->stAnytimeRMModel.bIsBC)
    {
        (hi_void)memcpy_s(mac, sizeof(mac), pstCcoCtx->stAnytimeRMModel.pstPlcDataDl->ucMac, HI_PLC_MAC_ADDR_LEN);
        (hi_void)memset_s(pstCcoCtx->stAnytimeRMModel.pstPlcDataDl->ucMac, HI_PLC_MAC_ADDR_LEN, 0xff, HI_PLC_MAC_ADDR_LEN);
    }

    ret = MRS_SendPlcFrame(pstCcoCtx->stAnytimeRMModel.pstPlcDataDl);
    if(ret == HI_ERR_SUCCESS)
    {
#if defined (PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
        MRS_CSG_TRANS_XR_DL_STRU *pstTransData = HI_NULL;
#else
        MRS_DATA_TRANSMIT_AFN13_DL_STRU *pstAfn13 = HI_NULL;
#endif

        mrsDfxXrPlcTx();
        if(pstCcoCtx->stAnytimeRMModel.bIsBC == HI_TRUE)
        {
            mrsDfxBcRmCnt();
        }

/* BEGIN: Modified by fengxiaomin/00209182, 2014/3/27   问题单号:DTS2014032906596 */
#if defined (PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
        pstTransData = (MRS_CSG_TRANS_XR_DL_STRU *)(pstCcoCtx->stAnytimeRMModel.pstPlcDataDl->pucPayload);
        mrsDfxRmPlcTx((HI_U8*)pstTransData + sizeof(MRS_CSG_TRANS_XR_DL_STRU), pstTransData->usDataLen, ret);
#else
        pstAfn13 = (MRS_DATA_TRANSMIT_AFN13_DL_STRU *)(pstCcoCtx->stAnytimeRMModel.pstPlcDataDl->pucPayload);
        mrsDfxRmPlcTx((HI_U8*)pstAfn13 + sizeof(MRS_DATA_TRANSMIT_AFN13_DL_STRU), pstAfn13->data_len, ret);
#endif
    }
/* END:   Modified by fengxiaomin/00209182, 2014/3/27 */

    if (pstCcoCtx->stAnytimeRMModel.bIsBC)
    {
        (hi_void)memcpy_s(pstCcoCtx->stAnytimeRMModel.pstPlcDataDl->ucMac, HI_PLC_MAC_ADDR_LEN, mac, HI_PLC_MAC_ADDR_LEN);
    }

    return ret;
}

HI_U32 mrsAFN13F1CreateAckFrame(HI_U8 **ppBuf, HI_U16 *pLen)
{
    HI_U8 DataBuff[12] = {0};

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO_BROAD_BAND_UNIT)
    if (mrsToolIsBbu())
    {
        // 江苏版本，不回复，清buffer
        mrsClear645ProtoBufferBbu(ID_MRS_BBUDA_645BUF);

        if (!mrsIsSimuChlOpened())
        {
            return HI_ERR_SKIP;
        }
    }
#endif

    return mrsCreateReadMeterFrame376_2(0, DataBuff, 0, ppBuf, pLen);
}

// 组织上报AFN13-F1应答帧(空数据内容)
HI_U32 mrsAFN13F1ReportAck(HI_VOID)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U8 * pOutBuff = HI_NULL;
    HI_U16 usOutLen = 0;

    ret = mrsAFN13F1CreateAckFrame(&pOutBuff, &usOutLen);
    if((ret == HI_ERR_SUCCESS) && (pOutBuff != HI_NULL))
    {
        MRS_SendMrData(pOutBuff, usOutLen, HI_DMS_CHL_UART_PORT_APP);   // 发送读表的返回报文到集中器
/* BEGIN: Modified by fengxiaomin/00209182, 2014/3/27   问题单号:DTS2014032906596 */
        mrsDfxRmUartTx(pOutBuff, usOutLen, HI_FALSE);
/* END:   Modified by fengxiaomin/00209182, 2014/3/27 */
    }
    mrsToolsFree(pOutBuff);   // 释放读表上行帧

    return ret;
}


HI_U32 mrsXrReportAck(HI_VOID)
{
    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();

    if (XR_FROM_PROTO_CSG == pstCcoCtx->stAnytimeRMModel.from)
    {
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_006, HI_DIAG_MT("[CSG TASK] mrsXrReportAck()."));
        mrsCcoMrTaskReportNoResponse();
        return HI_ERR_SUCCESS;
    }
    else
    {
        return mrsAFN13F1ReportAck();
    }
}

//*****************************************************************************
// 函数名称: mrsAnyTimeReadMeterPlcTimeOutProcess
// 功能描述: 随抄读表Plc帧超时处理
//
// 参数说明:
//   pstCcoCtx 全局上下文
//
// 返 回 值:
//   无
//
// 调用要求:
// 调用举例:
// 作    者: liuxipeng/KF54842 [2012-06-09]
// 修    改: cuiate/c00233580  [2013-04-01]
// 修    改: cuiate/c00233580  [2013-07-22]
//*****************************************************************************
HI_VOID mrsAnyTimeReadMeterPlcTimeOutProcess(MRS_CCO_SRV_CTX_STRU * pstCcoCtx)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    PLC_OVERTIME_INF_STRU* TimeOutInf = mrsGetPlcOverTimeInfAfn13();
    MR_ANYTIME_READMETER_STRU *anytime_readmeter = &pstCcoCtx->stAnytimeRMModel;
    MRS_PLC_FRAME_DATA_STRU *plc_data_dl = anytime_readmeter->pstPlcDataDl;

    mrsDfxXrPlcTimeout();

    if (plc_data_dl == HI_NULL)
    {
        return;
    }

    mrsDfxRmPlcDlRx(ID_MRS_CMD_DATA_TRANSMIT_AFN13_PLC, plc_data_dl->pucPayload, HI_TRUE);

    while (TimeOutInf->Loop < TimeOutInf->MaxTryLoop)
    {
        if (anytime_readmeter->bIsBC == HI_FALSE)   // 如果不是广播报文
        {
            if (TimeOutInf->Try >= TimeOutInf->MaxTry)  // 进入广播模式
            {
                TimeOutInf->Try = 0;

                if (0 == TimeOutInf->MaxTryNvB)     // 广播重试次数为0，回到单播模式
                {
                    if (0 == TimeOutInf->MaxTryNvS)
                    {
                        TimeOutInf->Loop = 0;
                        break;
                    }

                    TimeOutInf->Loop++;
                    continue;
                }

                anytime_readmeter->bIsBC = HI_TRUE;
                mrsGetMaxTryAfn13(HI_NULL);
                mrsGetPlcTimeOutAfn13(plc_data_dl->ucMac, HI_TRUE, HI_NULL);

            }
        }
        else
        {
            mrsDfxBcRmFailCnt();    // 统计广播帧抄读失败次数
        }

        if (TimeOutInf->Try < TimeOutInf->MaxTry)
        {
            TimeOutInf->Try++;

            mrsDfxXrPlcTryCnt();    // 统计重发的PLC帧

            ret = mrsSendReadMeterFramePlc();
            if (HI_ERR_SUCCESS == ret)
            {
                mrsSetXRStatus(EN_MRS_ANYTIME_RM_WAIT_PLC);
                MRS_StartTimer(EN_MRS_SRV_CCO_STA_TIMER, (HI_U32)TimeOutInf->OverTime, HI_SYS_TIMER_ONESHOT);
                return;
            }

            TimeOutInf->Try = 0;
            TimeOutInf->Loop = 0;
            break;
        }

        if (anytime_readmeter->bIsBC != HI_FALSE)   // 广播报文
        {
            TimeOutInf->Try = 0;
            TimeOutInf->Loop++;

            if ((TimeOutInf->MaxTryNvS > 0) && (!mrsToolsBroadCastMac(plc_data_dl->ucMac)))  // 不是广播地址时，才切换到单播，否则不切换
            {
                anytime_readmeter->bIsBC = HI_FALSE;
                mrsGetMaxTryAfn13(plc_data_dl->ucMac);
                mrsGetPlcTimeOutAfn13(plc_data_dl->ucMac, HI_FALSE, HI_NULL);
            }
        }
    }

    // 随抄读表失败次数统计
    mrsDfxXrFailCnt();

#if defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG)
    if(plc_data_dl->pucPayload != HI_NULL)
    {
        MRS_DATA_TRANSMIT_AFN13_DL_STRU * plcDlData = HI_NULL;

        plcDlData = (MRS_DATA_TRANSMIT_AFN13_DL_STRU *)plc_data_dl->pucPayload;
        if(plcDlData->data_len + sizeof(MRS_DATA_TRANSMIT_AFN13_DL_STRU) == plc_data_dl->usPayloadLen){
            mrsLogPlcOverTimeStat(plcDlData->data, plcDlData->data_len, 0, HI_NULL);
        }
    }
#endif

    if (!mrsIsDetectRm())
    {
        // 组织上报应答帧(空数据内容)
        mrsXrReportAck();
        mrsDecreaseMrTTL(&(pstCcoCtx->stMapModule), anytime_readmeter->aucMeterAddr);
    }

    mrsCcoDetectRmTimeoutProc(anytime_readmeter->aucMeterAddr);
    mrsCheckDetectEndProc();

    mrsAnyTimeReadMeterFinishProc();

    return ;
}

//*****************************************************************************
// 函数名称: mrsAnyTimeReadMeterReceivePlcProcess
// 功能描述: 随抄读表Plc帧接收处理
//
// 参数说明:
//   pstData PLC数据转发帧结构
//
// 返 回 值:
//   无
//
// 调用要求:
// 调用举例:
// 作    者: liuxipeng/KF54842 [2012-06-09]
//*****************************************************************************
HI_U32 mrsAnyTimeReadMeterReceivePlcProcess(MRS_DATA_TRANSMIT_AFN13_UP_STRU * transmit_data)
{
    MRS_CCO_SRV_CTX_STRU * cco_ctx = mrsCcoGetContext();
    HI_U8 * data_buff = HI_NULL;
    HI_U16 data_len = 0;
#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
    HI_U8 protocol = 0;
    HI_U8 * out_buff = HI_NULL;
    HI_U16 out_len = 0;
    HI_U32 ret = HI_ERR_SUCCESS;
#endif

    if(transmit_data == HI_NULL)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    if(cco_ctx->stAnytimeRMModel.enState != EN_MRS_ANYTIME_RM_WAIT_PLC)
    {
        HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_008, HI_DIAG_MT("XR status err [%d]"),cco_ctx->stAnytimeRMModel.enState);
        mrsDfxXrPlcExceedCnt();
        mrsDfxRmPlcUpRx(ID_MRS_CMD_DATA_TRANSMIT_AFN13_PLC, (HI_U8*)transmit_data, HI_TRUE);
        return HI_ERR_FAILURE;
    }

    if (transmit_data->seq != cco_ctx->stAnytimeRMModel.seq)
    {
        HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_009, HI_DIAG_MT("XR seq err [%d][%d]"),
                        transmit_data->seq,cco_ctx->stAnytimeRMModel.seq);
        mrsDfxXrPlcSeqErr();    // 统计丢弃的PLC帧数(过期的)
        mrsDfxRmPlcUpRx(ID_MRS_CMD_DATA_TRANSMIT_AFN13_PLC, (HI_U8*)transmit_data, HI_TRUE);
        return HI_ERR_FAILURE;
    }

#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
    protocol = (HI_U8)transmit_data->protocol;   // 获取规约类型
#endif
    data_buff = (HI_U8*)transmit_data + transmit_data->stru_size;   // 获取转发数据
    data_len = transmit_data->data_len;   // 获取转发数据长度

    if (mrsCcoIsTestMode())
    {
        HI_U16 usPos = 0;
        HI_U16 usFrameLen = 0;

        if ((HI_ERR_SUCCESS != mrsFind645Frame(data_buff, (HI_S16)data_len, &usPos, &usFrameLen))
            || (!mrsMeterAddrMatch(cco_ctx->stAnytimeRMModel.aucMeterAddr, data_buff + usPos + MRS_645_FRAME_METERADD_OFFSET)))
        {
            HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_001, HI_DIAG_MT("[XR]测试模式，地址不匹配"));
            return HI_ERR_FAILURE;
        }
    }

#if defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG)
    if(cco_ctx->stAnytimeRMModel.pstPlcDataDl != HI_NULL)
    {
        MRS_DATA_TRANSMIT_AFN13_DL_STRU * plcDlData = HI_NULL;

        plcDlData = (MRS_DATA_TRANSMIT_AFN13_DL_STRU *)cco_ctx->stAnytimeRMModel.pstPlcDataDl->pucPayload;
        if(plcDlData != HI_NULL && plcDlData->data_len + sizeof(MRS_DATA_TRANSMIT_AFN13_DL_STRU) == cco_ctx->stAnytimeRMModel.pstPlcDataDl->usPayloadLen)
        {
            mrsLogFrameInfStatRx(data_buff, data_len, plcDlData->data, plcDlData->data_len, 1);
        }
    }
#endif

    MRS_StopTimer(EN_MRS_SRV_CCO_STA_TIMER);    // 停止Plc帧超时定时器
    FreePlcData(cco_ctx);     // 释放备份的Plc下行帧数据

    mrsDfxXrOkCnt();

    if (mrsIsDetectRm())
    {
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_060, HI_DIAG_MT("detect rm rx resp"));

        mrsAnyTimeReadMeterFinishProc();
        //将探测抄表中表档案中抄读成功表的探测标志清除
        mrsDetectRmRespProc(mrsCcoGetDetectRmCtx()->stRunning.stNode.aucNodeAddr);

        return HI_ERR_SUCCESS;
    }
    else
    {
        //将探测抄表中表档案中抄读成功表的探测标志清除
        mrsDetectRmRespProc(cco_ctx->stAnytimeRMModel.aucMeterAddr);
    }

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO_BROAD_BAND_UNIT)
    if (mrsToolIsBbu())
    {
        // 清buffer
        mrsClear645ProtoBufferBbu(ID_MRS_BBUDA_645BUF);
    }

    // 如果NV项支持江苏版本波特率自适应，且为业务串口
    if (mrsToolIsBbu()
        && (!mrsIsSimuChlOpened()))
    {
        // 江苏版本波特率自适应 && 业务串口
        out_buff = data_buff;
        out_len = data_len;

        ret = HI_ERR_SUCCESS;
    }
    else
#endif
    {
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
        mrsCcoMrTaskReportXrFrame(data_buff, data_len);
#else
        // 模拟通道 || 非江苏版本波特率自适应
        ret = mrsCreateReadMeterFrame376_2(protocol, data_buff, data_len, &out_buff, &out_len);
#endif
    }

#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
    if((ret == HI_ERR_SUCCESS) && (out_buff != HI_NULL))
    {
        ret = MRS_SendMrData(out_buff, out_len, HI_DMS_CHL_UART_PORT_APP); // 发送读表的返回报文到集中器
        if(ret == HI_ERR_SUCCESS)
        {
            mrsDfxXrUartTx();
            mrsDfxRmUartTx(out_buff, out_len, HI_FALSE);
        }
    }

    mrsToolsFree(out_buff);   // 释放读表上行帧
#endif

    mrsAnyTimeReadMeterFinishProc();

    return HI_ERR_SUCCESS;
}

//*****************************************************************************
// 函数名称: mrsAnyTimeReadMeterPlcChlStatusProcess
// 功能描述: 随抄读表通道改变消息处理
//
// 参数说明:
//   pstCcoCtx 全局上下文
//
// 返 回 值:
//   无
//
// 调用要求:
// 调用举例:
// 作    者: liuxipeng/KF54842 [2012-06-09]
//*****************************************************************************
HI_U32 mrsAnyTimeReadMeterPlcChlStatusProcess(MRS_CCO_SRV_CTX_STRU * pstCcoCtx)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    PLC_OVERTIME_INF_STRU* TimeOutInf = mrsGetPlcOverTimeInfAfn13();

    if(pstCcoCtx == HI_NULL)
    {
        return HI_ERR_FAILURE;
    }

    if(pstCcoCtx->stAnytimeRMModel.enState != EN_MRS_ANYTIME_RM_WAIT_CHLAVAIL)
    {
        return HI_ERR_FAILURE;
    }

    if(pstCcoCtx->stAnytimeRMModel.pstPlcDataDl == HI_NULL)
    {
        return HI_ERR_FAILURE;
    }

    TimeOutInf->Try = 0;
    TimeOutInf->Loop = 0;

    // 调用发送Plc函数
    ret = mrsSendReadMeterFramePlc();
    if(ret == HI_ERR_SUCCESS)
    {
        mrsSetXRStatus(EN_MRS_ANYTIME_RM_WAIT_PLC);
        MRS_StartTimer(EN_MRS_SRV_CCO_STA_TIMER,
                            (HI_U32)TimeOutInf->OverTime, HI_SYS_TIMER_ONESHOT); // 启动定时器
    }

    return ret;
}

//*****************************************************************************
// 函数名称: mrsAnyTimeReadMeterProcess
// 功能描述: 随抄读表处理入口
//
// 参数说明:
//   pstAddr 目标地址
//   ucProtocol 规约类型
//   pDataBuff 待抄读的数据
//   usDataLen 待抄读数据的长度
//
// 返 回 值:
//   无
//
// 调用要求:
// 调用举例:
// 作    者: liuxipeng/KF54842 [2012-06-09]
//*****************************************************************************
HI_U32 mrsAnyTimeReadMeterProcess(HI_IN HI_U8 *pDstAddr, HI_IN HI_U8 ucProtocol, HI_IN HI_U8 * pDataBuff, HI_IN HI_U16 usDataLen)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U8 ucDestAddr[HI_PLC_MAC_ADDR_LEN] = {0};    // 目标表地址
    HI_U8 nodeMac[6] = {0};
    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();
    PLC_OVERTIME_INF_STRU * TimeOutInf = mrsGetPlcOverTimeInfAfn13();

    // 获取目标电表地址
    mrsGetDestAddr(pstCcoCtx->stAnytimeRMModel.aucMeterAddr, pDstAddr, ucProtocol, pDataBuff, usDataLen);
    mrsMeterAddrPreProc(ucDestAddr, pstCcoCtx->stAnytimeRMModel.aucMeterAddr, ucProtocol);

    // 查找映射关系
    ret = mrsMapGetMAC(ucDestAddr, nodeMac);
    if(ret == HI_ERR_SUCCESS)
    {
        pstCcoCtx->stAnytimeRMModel.bIsBC = HI_FALSE;   // 置为非广播式读表
    }
    else
    {
        (hi_void)memset_s(nodeMac, HI_PLC_MAC_ADDR_LEN, 0xFF, HI_PLC_MAC_ADDR_LEN);   // 没有找到对应的MAC地址(使用广播MAC)
        pstCcoCtx->stAnytimeRMModel.bIsBC = HI_TRUE;   // 置为广播式读表
    }

    if ((pstCcoCtx->ucDutBoardFlag)
        || (pstCcoCtx->stAnytimeRMModel.stOverTimeInf.MaxTryNvB == -1))
    {
        (hi_void)memset_s(nodeMac, HI_PLC_MAC_ADDR_LEN, 0xFF, HI_PLC_MAC_ADDR_LEN);   // 没有找到对应的MAC地址(使用广播MAC)
        pstCcoCtx->stAnytimeRMModel.bIsBC = HI_TRUE;   // 置为广播式读表
    }

    do
    {
        // 创建PLC数据转发包
        ret = mrsCreateReadMeterFramePlc(nodeMac, ucProtocol, pDataBuff, usDataLen);
        if(ret != HI_ERR_SUCCESS)
        {
            break;
        }

        // 发送Plc数据包
        ret = mrsSendReadMeterFramePlc();
        if(ret == HI_ERR_SUCCESS)
        {
            mrsSetXRStatus(EN_MRS_ANYTIME_RM_WAIT_PLC);
            MRS_StartTimer(EN_MRS_SRV_CCO_STA_TIMER,
                                (HI_U32)TimeOutInf->OverTime, HI_SYS_TIMER_ONESHOT); // 启动定时器
        }

    } while(0);

    if(ret == HI_ERR_SUCCESS)
    {
        HI_U32 status = mrsCcoGetState();

        status &= ~MRS_CCO_STATUS_NORMAL;
        status |= MRS_CCO_STATUS_ANYTIME_READMETER;
        mrsCcoSetState(status);

        // 启动服务结束定时器(到时间后会将随抄状态置为预恢复状态)
        MRS_StartTimer(EN_MRS_SRV_CCO_SERVICE_END_TIMER, (pstCcoCtx->stAnytimeRMModel.stOverTimeInf.MaxTimeout - MRS_RM_MAX_SERVICE_TIME_SUB) * 1000, HI_SYS_TIMER_ONESHOT);
    }
    else
    {
        mrsAnyTimeReadMeterFinishProc();
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_052, HI_DIAG_MT("any read fail"));
    }

    return ret;
}


/* BEGIN: PN: DTS2015040900760 MODIFY\ADD\DEL by cuiate/00233580 at 2015/5/12 */
HI_U32 mrsAnyTimeReadMeterProcessDelay(HI_IN HI_U8 * pDataBuff, HI_IN HI_U16 usDataLen)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U8 nodeMac[HI_PLC_MAC_ADDR_LEN] = {0};
    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();
    PLC_OVERTIME_INF_STRU * TimeOutInf = mrsGetPlcOverTimeInfAfn13();

    // 获取目标电表地址
    HI_U16 pos = 0;
    HI_U16 len = usDataLen;

    (hi_void)memset_s(nodeMac, HI_PLC_MAC_ADDR_LEN, 0xFF, HI_PLC_MAC_ADDR_LEN);
    (hi_void)memset_s(pstCcoCtx->stAnytimeRMModel.aucMeterAddr, HI_METER_ADDR_LEN, 0, HI_METER_ADDR_LEN);
    pstCcoCtx->stAnytimeRMModel.bIsBC = HI_TRUE;

    ret = mrsFind645Frame(pDataBuff, (HI_S16)usDataLen, &pos, &len);
    if (HI_ERR_SUCCESS == ret)
    {
        HI_U8 ucProtocol = mrsToolsGet645Ver(pDataBuff[pos + MRS_645_FRAME_CTRL_OFFSET] & MRS_645_FRAME_CONTROL_MASK);
        HI_U8 aucAddr[HI_METER_ADDR_LEN];

        (hi_void)memcpy_s(pstCcoCtx->stAnytimeRMModel.aucMeterAddr, HI_METER_ADDR_LEN,
            pDataBuff + pos + MRS_645_FRAME_METERADD_OFFSET, HI_METER_ADDR_LEN);
        mrsMeterAddrPreProc(aucAddr, pDataBuff + pos + MRS_645_FRAME_METERADD_OFFSET, ucProtocol);

        // 查找映射关系
        ret = mrsMapGetMAC(aucAddr, nodeMac);
        if (HI_ERR_SUCCESS == ret)
        {
            pstCcoCtx->stAnytimeRMModel.bIsBC = HI_FALSE;   // 置为非广播式读表
        }
    }

    do
    {
        // 创建PLC数据转发包
        ret = mrsCreateReadMeterFramePlc(nodeMac, pstCcoCtx->stAnytimeRMModel.ucProtocol, pDataBuff, usDataLen);
        if(HI_ERR_SUCCESS != ret)
        {
            break;
        }

        // 发送Plc数据包
        ret = mrsSendReadMeterFramePlc();
        if(HI_ERR_SUCCESS == ret)
        {
            mrsSetXRStatus(EN_MRS_ANYTIME_RM_WAIT_PLC);
            MRS_StartTimer(EN_MRS_SRV_CCO_STA_TIMER,
                                (HI_U32)TimeOutInf->OverTime, HI_SYS_TIMER_ONESHOT); // 启动定时器
        }

    }while(0);

    if(ret != HI_ERR_SUCCESS)    // 出现错误
    {
        FreePlcData(pstCcoCtx);  // 清空备份的数据区

        mrsAnyTimeReadMeterFailEndProc(pstCcoCtx);
    }

    return ret;
}
/* END:   PN: DTS2015040900760 MODIFY\ADD\DEL by cuiate/00233580 at 2015/5/12 */

//*****************************************************************************
// 函数名称: mrsAnyTimeReadMeterFinishProc
// 功能描述: 随抄读表结束处理
//
// 参数说明:
//   无
//
// 返 回 值:
//   无
//
// 调用要求:
// 调用举例:
// 作    者: fengxiaomin/00209182 [2016-03-18]
//*****************************************************************************
HI_VOID mrsAnyTimeReadMeterFinishProc(HI_VOID)
{
    HI_U32 status = mrsCcoGetState();

    status &= ~MRS_CCO_STATUS_ANYTIME_READMETER;
    mrsCcoSetState(status);

    mrsAnyTimeReadMeterInit();  // 随抄模块初始化(初始化状态-释放内存-停止相应定时器)

    HI_MDM_SetTfNtbSendProcessCtrl(HI_MDM_NTB_SEND_PROCESS_RESUME, HI_MDM_NTB_SEND_CTRL_REASON_READ_METER);

#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
    mrsReadMeterResume();
    if (mrsIsDetectRm())
    {
        mrsSetDetectRmFlag(HI_FALSE);
        mrsResumeDetectRmSendMsg();
    }
    else
    {
        //集中器透抄结束后恢复探测抄表,启动3秒定时器
        MRS_StartTimer(MRS_CCO_TIMER_RESUME_DETECT_RM, MRS_CCO_TIME_RESUME_DETECT_RM_DEFAULT, HI_SYS_TIMER_ONESHOT);
    }
#endif
}

//*****************************************************************************
// 函数名称: mrsAnyTimeReadMeterServiceTimeout
// 功能描述: 随抄读表最长时间超时处理
//
// 参数说明:
//   pstCcoCtx 全局上下文
//
// 返 回 值:
//   无
//
// 调用要求:
// 调用举例:
// 作    者: fengxiaomin/00209182 [2016-03-18]
//*****************************************************************************
HI_VOID mrsAnyTimeReadMeterServiceTimeout(MRS_CCO_SRV_CTX_STRU * pstCcoCtx)
{
    if (pstCcoCtx->status & MRS_CCO_STATUS_ANYTIME_READMETER)
    {
        mrsDecreaseMrTTL(&pstCcoCtx->stMapModule, pstCcoCtx->stAnytimeRMModel.aucMeterAddr);
        mrsAnyTimeReadMeterFailEndProc(pstCcoCtx);
    }
}

//*****************************************************************************
// 函数名称: mrsAnyTimeReadMeterFailEndProc
// 功能描述: 随抄读表失败结束处理
//
// 参数说明:
//   pstCcoCtx 全局上下文
//
// 返 回 值:
//   无
//
// 调用要求:
// 调用举例:
// 作    者: fengxiaomin/00209182 [2016-03-18]
//*****************************************************************************
HI_VOID mrsAnyTimeReadMeterFailEndProc(MRS_CCO_SRV_CTX_STRU * pstCcoCtx)
{
#if defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG)
    MRS_DATA_TRANSMIT_AFN13_DL_STRU * plcDlData = HI_NULL;

    if(pstCcoCtx->stAnytimeRMModel.pstPlcDataDl != HI_NULL)
    {
        plcDlData = (MRS_DATA_TRANSMIT_AFN13_DL_STRU *)pstCcoCtx->stAnytimeRMModel.pstPlcDataDl->pucPayload;
        if(plcDlData != HI_NULL && plcDlData->data_len + sizeof(MRS_DATA_TRANSMIT_AFN13_DL_STRU) == pstCcoCtx->stAnytimeRMModel.pstPlcDataDl->usPayloadLen)
        {
            mrsLogPlcOverTimeStat(plcDlData->data, plcDlData->data_len, 0, HI_NULL);
        }
    }
#endif

    if(EN_MRS_ANYTIME_RM_WAIT_REVISE_RESP != pstCcoCtx->stAnytimeRMModel.enState)
    {
        mrsXrReportAck();
    }

#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
    mrsReadMeterStateInit();    // 定时抄表模块初始化
#endif

    mrsAnyTimeReadMeterFinishProc();

    return;
}


#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)

HI_U32 mrsCsgCreateReadMeterFramePlc(HI_U8 *pucMacAddr, HI_U8 *pucSrcAddr, HI_U8 *pucDstAddr, HI_U8 *pucDataBuf, HI_U16 usDataLen)
{
    MRS_CCO_SRV_CTX_STRU *pstCcoCtx = mrsCcoGetContext();
    MRS_PLC_FRAME_DATA_STRU *pstPlcFrame = HI_NULL;
    MRS_CSG_PLC_FRAME_HEAD *pstFrameHead = HI_NULL;
    MRS_CSG_TRANS_XR_DL_STRU *pstTransData = HI_NULL;
    HI_U8 *pucPayload = HI_NULL;
    HI_U16 usPayloadLen = 0;
    HI_U32 ulMeterTimeout = 0;

    pstPlcFrame = (MRS_PLC_FRAME_DATA_STRU *)mrsToolsMalloc(sizeof(MRS_PLC_FRAME_DATA_STRU));
    if (!pstPlcFrame)
    {
        return HI_ERR_NO_MORE_MEMORY;
    }

    (hi_void)memset_s(pstPlcFrame, sizeof(MRS_PLC_FRAME_DATA_STRU), 0, sizeof(MRS_PLC_FRAME_DATA_STRU));

    usPayloadLen = (HI_U16)(sizeof(MRS_CSG_PLC_FRAME_HEAD) + sizeof(MRS_CSG_TRANS_XR_DL_STRU) + usDataLen);
    pucPayload = (HI_U8 *)mrsToolsMalloc(usPayloadLen);
    if (!pucPayload)
    {
        mrsToolsFree(pstPlcFrame);
        return HI_ERR_NO_MORE_MEMORY;
    }

    (hi_void)memset_s(pucPayload, usPayloadLen, 0, usPayloadLen);
    mrsCtrlCmdProc(pucDataBuf, usDataLen);     // 费控命令处理

    // 初始化超时及重试次数
    mrsGetMaxTryAfn13(pucMacAddr);
    mrsGetPlcTimeOutAfn13(pucMacAddr, HI_FALSE, &ulMeterTimeout);

    pstCcoCtx->stAnytimeRMModel.seq  = pstCcoCtx->usPlcSeq++;
    // 组织转发数据包
    pstFrameHead = (MRS_CSG_PLC_FRAME_HEAD *)pucPayload;

    pstFrameHead->usFrameType = MRS_CSG_PLC_CTRL_TYPE_TRANS;
    pstFrameHead->usExtendBit = MRS_CSG_PLC_CTRL_EXTEND_BIT_N;
    pstFrameHead->usRespBit = MRS_CSG_PLC_CTRL_RESP_BIT_Y;
    pstFrameHead->usPrm = MRS_CSG_PLC_CTRL_PRM_HOST;
    pstFrameHead->usDir = MRS_CSG_PLC_CTRL_DIR_DL;
    pstFrameHead->ucSrvId = MRS_CSG_PLC_SRV_ID_TRANS_XR;
    pstFrameHead->ucVersion = MRS_CSG_PLC_PROTO_VERSION;
    pstFrameHead->usSeq = pstCcoCtx->stAnytimeRMModel.seq;

    pstFrameHead->usDataLength = sizeof(MRS_CSG_TRANS_XR_DL_STRU) + usDataLen;

    pstTransData = (MRS_CSG_TRANS_XR_DL_STRU *)pstFrameHead->aucData;
    (hi_void)memcpy_s(pstTransData->aucSrcAddr, HI_METER_ADDR_LEN, pucSrcAddr, HI_METER_ADDR_LEN);
    (hi_void)memcpy_s(pstTransData->aucDstAddr, HI_METER_ADDR_LEN, pucDstAddr, HI_METER_ADDR_LEN);

    pstTransData->ucTimeout = (HI_U8)MRS_MS_TO_100MS(ulMeterTimeout);
    pstTransData->usDataLen = usDataLen;
    (hi_void)memcpy_s(pstTransData->aucData, pstTransData->usDataLen, pucDataBuf, usDataLen);

    pstPlcFrame->usId = ID_MRS_CMD_CSG_DATA_TRANSMIT_PLC;
    (hi_void)memcpy_s(pstPlcFrame->ucMac, HI_PLC_MAC_ADDR_LEN, pucMacAddr, HI_PLC_MAC_ADDR_LEN);
    pstPlcFrame->usPayloadLen = usPayloadLen;
    pstPlcFrame->pucPayload = pucPayload;

    // 备份数据转发PLC包(重发)
    pstCcoCtx->stAnytimeRMModel.pstPlcDataDl = pstPlcFrame;

#if defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG)
    mrsLogFrameInfStatTx(pucDataBuf, usDataLen, (HI_U8)pstCcoCtx->stAnytimeRMModel.bIsBC);
#endif

    return HI_ERR_SUCCESS;
}


HI_U32 mrsCsgAnyTimeReadMeterProcess(HI_U8 *pucSrcAddr, HI_U8 *pucDstAddr, HI_U8 *pucDataBuf, HI_U16 usDataLen)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U8 aucDstAddr[HI_METER_ADDR_LEN] = {0};
    HI_U8 aucMacAddr[HI_PLC_MAC_ADDR_LEN] = {0};
    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();
    PLC_OVERTIME_INF_STRU * pTimeOutInf = mrsGetPlcOverTimeInfAfn13();

    (hi_void)memcpy_s(pstCcoCtx->stAnytimeRMModel.aucMeterAddr, HI_METER_ADDR_LEN, pucDstAddr, HI_METER_ADDR_LEN);
    mrsMeterAddrPreProc(aucDstAddr, pucDstAddr, MRS_PROTO645_VER_1997);

    pstCcoCtx->stAnytimeRMModel.bIsBC = HI_FALSE;
    ulRet = mrsMapGetMAC(aucDstAddr, aucMacAddr);
    if (HI_ERR_SUCCESS != ulRet)
    {
        //没有映射关系，测试模式或组网中表数量不大于10时，从拓扑中获取mac地址抄表，测试系统
        if (HI_MDM_CHECK_MODE == mrsGetCheckMode()
            || (!mrsGetCcoPlcChlStatus() && (HI_ERR_SUCCESS == mrsCheckMeterNum())))
        {
            //抄表时查找所抄读的表地址对应的mac地址,如果不在映射表中,查找拓扑中对应的mac地址
            ulRet = mrsGetMacFromTopo(aucDstAddr, aucMacAddr);
            if (HI_ERR_SUCCESS != ulRet)
            {
                pstCcoCtx->stAnytimeRMModel.bIsBC = HI_TRUE;
                (hi_void)memcpy_s(aucMacAddr, HI_PLC_MAC_ADDR_LEN, MRS_BROADCAST_MAC, HI_PLC_MAC_ADDR_LEN);
            }
        }
        else
        {
            pstCcoCtx->stAnytimeRMModel.bIsBC = HI_TRUE;
            (hi_void)memcpy_s(aucMacAddr, HI_PLC_MAC_ADDR_LEN, MRS_BROADCAST_MAC, HI_PLC_MAC_ADDR_LEN);
        }
    }

    if ((pstCcoCtx->ucDutBoardFlag) || (pstCcoCtx->stAnytimeRMModel.stOverTimeInf.MaxTryNvB == -1))
    {
        (hi_void)memcpy_s(aucMacAddr, HI_PLC_MAC_ADDR_LEN, MRS_BROADCAST_MAC, HI_PLC_MAC_ADDR_LEN);
        pstCcoCtx->stAnytimeRMModel.bIsBC = HI_TRUE;
    }

    do
    {
        if (mrsCcoIsTestMode())
        {
            ulRet = mrsCreateReadMeterFramePlc(aucMacAddr, MRS_PROTO645_VER_2007, pucDataBuf, usDataLen);
        }
        else
        {
            ulRet = mrsCsgCreateReadMeterFramePlc(aucMacAddr, pucSrcAddr, pucDstAddr, pucDataBuf, usDataLen);
        }

        if (ulRet != HI_ERR_SUCCESS)
        {
            break;
        }

        // 发送Plc数据包
        ulRet = mrsSendReadMeterFramePlc();
        if (ulRet == HI_ERR_SUCCESS)
        {
            mrsSetXRStatus(EN_MRS_ANYTIME_RM_WAIT_PLC);
            MRS_StartTimer(EN_MRS_SRV_CCO_STA_TIMER, (HI_U32)pTimeOutInf->OverTime, HI_SYS_TIMER_ONESHOT);
        }

    } while(0);

    if(HI_ERR_SUCCESS != ulRet)    // 出现错误
    {
        FreePlcData(pstCcoCtx);  // 清空备份的数据区
    }

    return ulRet;
}


HI_VOID mrsCsgAnyTimeReadMeterReceivePlcProcess(HI_U8 *pucPayload, HI_U16 usPayloadLen)
{
    MRS_CCO_SRV_CTX_STRU *pstCcoCtx = mrsCcoGetContext();
    MRS_CSG_PLC_FRAME_HEAD *pstFrameHead = (MRS_CSG_PLC_FRAME_HEAD *)pucPayload;
    MRS_CSG_TRANS_XR_UL_STRU *pstXrUlFrame = (MRS_CSG_TRANS_XR_UL_STRU *)pstFrameHead->aucData;

    HI_UNREF_PARAM(usPayloadLen);

    if (EN_MRS_ANYTIME_RM_WAIT_PLC != pstCcoCtx->stAnytimeRMModel.enState)
    {
        mrsDfxXrPlcExceedCnt();
        mrsDfxRmPlcUpRx(ID_MRS_CMD_CSG_DATA_TRANSMIT_PLC, (HI_U8*)pstXrUlFrame, HI_TRUE);
        return;
    }

    if (pstFrameHead->usSeq != pstCcoCtx->stAnytimeRMModel.seq)
    {
        mrsDfxXrPlcSeqErr();
        mrsDfxRmPlcUpRx(ID_MRS_CMD_CSG_DATA_TRANSMIT_PLC, (HI_U8*)pstXrUlFrame, HI_TRUE);
        return;
    }

#if defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG)
    if (pstCcoCtx->stAnytimeRMModel.pstPlcDataDl)
    {
        MRS_CSG_PLC_FRAME_HEAD *pstPlcDlData = (MRS_CSG_PLC_FRAME_HEAD *)pstCcoCtx->stAnytimeRMModel.pstPlcDataDl->pucPayload;
        MRS_CSG_TRANS_XR_DL_STRU *pstXrDlFrame = (MRS_CSG_TRANS_XR_DL_STRU *)pstPlcDlData->aucData;

        if(pstCcoCtx->stAnytimeRMModel.pstPlcDataDl->usPayloadLen == pstPlcDlData->usDataLength + sizeof(MRS_CSG_PLC_FRAME_HEAD) && 
            pstPlcDlData->usDataLength == pstXrDlFrame->usDataLen + sizeof(MRS_CSG_TRANS_XR_DL_STRU)){
            mrsLogFrameInfStatRx(pstXrUlFrame->aucData, pstXrUlFrame->usDataLen, pstXrDlFrame->aucData, pstXrDlFrame->usDataLen, 1);
        }
    }
#endif

    MRS_StopTimer(EN_MRS_SRV_CCO_STA_TIMER);
    FreePlcData(pstCcoCtx);
    mrsDfxXrOkCnt();

    mrsCcoMrTaskReportXrFrame(pstXrUlFrame->aucData, pstXrUlFrame->usDataLen);
    mrsAnyTimeReadMeterFinishProc();
}
#endif


#endif
HI_END_HEADER
