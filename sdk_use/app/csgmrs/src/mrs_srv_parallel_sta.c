//*****************************************************************************
//
//                  版权所有 (C), 1998-2011, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_srv_parallel_sta.c
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
#include "mrs_srv_common.h"
#include "mrs_fw_proto645.h"
#include "mrs_fw_tools.h"
#include "mrs_fw_log.h"
#include "mrs_srv_sta_queue.h"
#include "mrs_srv_parallel_sta.h"
#include "mrs_srv_sta_event.h"
#include "mrs_srv_sta.h"
#include "mrs_srv_collector.h"
#include "mrs_dfx.h"
#include "mrs_srv_mrslog.h"
#include "mrs_srv_sta_searchmeter.h"
#include "mrs_srv_clti_searchmeter.h"
#include "mrs_srv_collector_irda.h"

#if (defined(PRODUCT_CFG_PRODUCT_TYPE_STA) && !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD))

// 并发重读处理
HI_U32 mrsParallelTryProcess(MRS_PARALLEL_STA_CTX * ctx);

// *****************************************

HI_VOID mrsParallelInitStaCtx(HI_VOID);
HI_U16 mrsParallelGetRxBufLen(MRS_PARALLEL_STA_CTX * ctx);
HI_U32 mrsParallelCopyRxDataToPlcBuf(MRS_PARALLEL_STA_CTX * ctx, HI_PBYTE buf, HI_U16 * out_len);
HI_U32 mrsParallelSendPlcToCco(MRS_STA_ITEM * queue_item, MRS_PARALLEL_STA_CTX * ctx);
HI_VOID mrsParallelIndexStepper(MRS_PARALLEL_STA_CTX * ctx);
HI_U32 mrsParallelReceiveData(HI_U8 * data, HI_U16 len, HI_U8 protocol);
HI_BOOL mrsParallelNeedTry(MRS_PARALLEL_STA_CTX * ctx);
HI_U32 mrsParallelRefreshNextTryIndex(MRS_PARALLEL_STA_CTX * ctx);
HI_U32 mrsParallelCompatibleDl(HI_IN MRS_DATA_TRANSMIT_AFNF1_DL_STRU * t_data,
                                            HI_IN HI_U16 t_len, HI_OUT HI_PBYTE * data);
HI_U32 mrsParallelReceivePackPlcDl(HI_IN MRS_DATA_TRANSMIT_AFNF1_DL_STRU * t_data,
                                            HI_IN HI_U16 t_len, HI_IN HI_BOOL bc_flg);
                                            
/* BEGIN: Modified by liuxipeng/lKF54842,2014/4/26 问题单号:DTS2014042407354 */
// 电表故障判断
HI_BOOL mrsIsMeterFault(MRS_PARALLEL_STA_CTX * ctx);
// 广播帧过滤                                   
HI_BOOL mrsParallelBroadCastFiltrate(MRS_STA_SRV_CTX_STRU *pstStaCtx, 
                                            HI_U8 *pData, HI_U16 usLen);
/* END  : Modified by liuxipeng/lKF54842,2014/4/26 问题单号:DTS2014042407354 */

HI_PRV HI_U32 mrsParallelIrdaPlcResp(MRS_PARALLEL_STA_CTX *pstCtx, HI_U16 usSeq, HI_U8 *pData, HI_U16 usDataLen);
HI_PRV HI_U32 mrsParallelCheckIrda(MRS_PARALLEL_STA_CTX *pstCtx);
HI_PRV HI_U32 mrsParallelIrdaPlcProc(HI_U16 usSeq, HI_U8 *pData, HI_U16 usDataLen);

// *****************************************

// 初始化STA端并发读表上下文
HI_VOID mrsParallelInitStaCtx(HI_VOID)
{
    MRS_PARALLEL_STA_CTX * ctx = mrsStaGetParallelCtx();
    HI_U16 i = 0;
    
    HI_ASSERT(ctx);

    for(i = 0; i < MRS_PARALLEL_645_FRAME_MAX; i++)
    {
        if(ctx->frame_ctx[i].rx_buf)
        {
            mrsToolsFree(ctx->frame_ctx[i].rx_buf);
        }
    }

    (hi_void)memset_s(ctx, sizeof(MRS_PARALLEL_STA_CTX), 0, sizeof(MRS_PARALLEL_STA_CTX));
}

// 设置STA端并发读表上下文
HI_VOID mrsParallelSetStaCtx(MRS_STA_ITEM * queue_item, MRS_PARALLEL_STA_CTX * ctx)
{
    HI_PBYTE buf = HI_NULL;
    HI_S16 buf_len = 0;
    HI_U32 ret = HI_ERR_SUCCESS;

    HI_ASSERT(ctx);

    if(!queue_item)
    {
        return;
    }

    // 初始化并发上下文(释放内存-清零)
    mrsParallelInitStaCtx();

    buf = queue_item->buf;
    buf_len = (HI_S16)queue_item->len;

    ctx->gap_timer = queue_item->gap_timer;

#if 1
    ctx->noresp_try_flag = queue_item->noresp_try_flag;
    ctx->nak_try_flag = queue_item->nak_try_flag;
    ctx->try_max = queue_item->try_max;
#else
    ctx->noresp_try_flag = HI_TRUE;
    ctx->nak_try_flag = HI_TRUE;
    ctx->try_max = MRS_QUEUE_MAX_TRY - 1;
#endif
    
    // 查找到有效帧并依次放入帧上下文中
    while((buf) && (buf_len > 0))
    {
        HI_U16 pos = 0;
        HI_U16 frame_len = 0;
        
        ret = mrsFind645Frame(buf, buf_len, &pos, &frame_len);
        if(HI_ERR_SUCCESS == ret)
        {
            if(ctx->frame_num >= MRS_PARALLEL_645_FRAME_MAX)
            {
                break;
            }
            
            ctx->frame_ctx[ctx->frame_num].tx_buf = buf + pos;
            ctx->frame_ctx[ctx->frame_num].tx_len = (HI_S16)frame_len;
            ctx->frame_num++;
            
            buf += (pos + frame_len);
            buf_len -= (HI_S16)(pos + frame_len);            
        }
        else
        {
            break;
        }
    }

    return;
}

// 获取并发接收缓冲区长度(多帧长度和)
HI_U16 mrsParallelGetRxBufLen(MRS_PARALLEL_STA_CTX * ctx)
{
    HI_U16 len = 0;
    HI_U16 i = 0;

    HI_ASSERT(ctx);
    
    for(i = 0; i < ctx->frame_num; i++)
    {
        if(BIT32_GET(ctx->resp_flg,i) == HI_TRUE)
        {
            len += (HI_U16)ctx->frame_ctx[i].rx_len;
        }
    }

    return len;
}

// 依次拷贝接收到的645数据到PLC缓冲区中去
HI_U32 mrsParallelCopyRxDataToPlcBuf(MRS_PARALLEL_STA_CTX * ctx, HI_PBYTE buf, HI_U16 * out_len)
{
    HI_U32 ret = HI_ERR_FAILURE;
    HI_U16 i = 0;
    HI_U16 pos = 0;
    HI_U16 temp_len = 0;

    HI_ASSERT(ctx);

    if((!buf) || (!out_len))
    {
        return HI_ERR_FAILURE;
    }

    for(i = 0; i < ctx->frame_num; i++)
    {
        if(BIT32_GET(ctx->resp_flg,i) == HI_TRUE)
        {
            if(ctx->frame_ctx[i].rx_buf != HI_NULL)
            {
                (hi_void)memcpy_s(buf + pos, (HI_U16)(ctx->frame_ctx[i].rx_len), ctx->frame_ctx[i].rx_buf, (HI_U16)(ctx->frame_ctx[i].rx_len));
                pos += (HI_U16)ctx->frame_ctx[i].rx_len;
                temp_len += (HI_U16)ctx->frame_ctx[i].rx_len;
                ret = HI_ERR_SUCCESS;
            }
        }
    }

    // 输出真实拷贝数据的长度
    *out_len = temp_len;

    return ret;
}

// 创建并行读表的响应PLC帧并发送
HI_U32 mrsParallelSendPlcToCco(MRS_STA_ITEM * queue_item, MRS_PARALLEL_STA_CTX * ctx)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U16 rx_len = 0;
    HI_U16 copy_len = 0;
    MRS_PLC_FRAME_DATA_STRU plc_data = {0};
    MRS_DATA_TRANSMIT_AFNF1_UP_STRU * t_data = HI_NULL;
    HI_U16 t_len = 0;

    HI_ASSERT(ctx);

    if(!queue_item)
    {
       return HI_ERR_FAILURE;
    }
   
    rx_len = mrsParallelGetRxBufLen(ctx);
    
    if(0 == rx_len)
    {
        return HI_ERR_NO_MORE_DATA;
    }
 
    //总长度 = 会话结构 + 转发结构 + 数据
    t_len = (HI_U16)(sizeof(MRS_DATA_TRANSMIT_AFNF1_UP_STRU) + rx_len);
    t_data = (MRS_DATA_TRANSMIT_AFNF1_UP_STRU *)mrsToolsMalloc(t_len);    
    if (!t_data) 
    {
        return HI_ERR_FAILURE;
    }

    (hi_void)memset_s(t_data, t_len, 0, t_len);

    t_data->interface_ver = MRS_AFN_F1_UP_VER;
    t_data->stru_size = sizeof(MRS_DATA_TRANSMIT_AFNF1_UP_STRU);
    t_data->sta_status = MRS_STA_STATUS_RESP_OK;
    t_data->protocol = ctx->protocol;
    t_data->seq = (HI_U16)queue_item->seq;
    t_data->resp_status = (HI_U16)ctx->resp_flg;
    
    //复制数据
    ret = mrsParallelCopyRxDataToPlcBuf(ctx, t_data->data, &copy_len);
    if(ret != HI_ERR_SUCCESS)
    {
        mrsToolsFree(t_data);
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_005, HI_DIAG_MT("get buf err!"));
        return HI_ERR_FAILURE;
    }
    
    t_data->data_len = copy_len;
    
    //填充CCO地址(全0)
    (hi_void)memset_s(plc_data.ucMac, sizeof(plc_data.ucMac), 0, sizeof(plc_data.ucMac));

    plc_data.usId = queue_item->id;    // 获取业务ID 
    plc_data.pucPayload = (HI_PBYTE)t_data;
    plc_data.usPayloadLen = t_len;

#if defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG) && defined(MRS_LOG_STA_SWITCH)
    mrsLogFrameInfStatRx(t_data->data, copy_len, queue_item->buf, queue_item->len, t_data->resp_status);
#endif

    ret = MRS_SendPlcFrame(&plc_data);    
    if (ret != HI_ERR_SUCCESS) 
    {
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_007, HI_DIAG_MT("tx plc fail!"));
    }
    else
    {
        mrsDfxPrPlcTx();
    }

/* BEGIN: Modified by fengxiaomin/00209182, 2014/3/27   问题单号:DTS2014032906596 */
    mrsDfxRmPlcUpAfn(ID_MRS_CMD_PARALLEL_DATA_PLC, plc_data.pucPayload, ret);
/* END:   Modified by fengxiaomin/00209182, 2014/3/27 */

    mrsToolsFree(t_data);

    return ret;
}

// 并发读表轮次处理
HI_VOID mrsParallelIndexStepper(MRS_PARALLEL_STA_CTX * ctx)
{
    HI_ASSERT(ctx);

    ctx->index++;
    
    if(ctx->index >= ctx->frame_num)
    {
        // 序号清零
        ctx->index = 0;

        // 读表轮次自增
        ctx->trun_num++;
    }

    return;
}

// 接收电表数据处理
HI_U32 mrsParallelReceiveData(HI_U8 * data, HI_U16 len, HI_U8 protocol)
{
    MRS_PARALLEL_STA_CTX * ctx = mrsStaGetParallelCtx();
    MRS_645_FRAME_CTX * farme_ctx = HI_NULL;

    HI_ASSERT(ctx);

    if(!data)
    {
        return HI_ERR_FAILURE;
    }

    if(ctx->index >= ctx->frame_num)
    {
        HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_009, HI_DIAG_MT("index err index[%d] num[%d]"), ctx->index, ctx->frame_num);
        return HI_ERR_FAILURE;
    }
    
    farme_ctx = &ctx->frame_ctx[ctx->index];

    // 当电表应答否认帧时该空间已经被使用, 重读时需要释放该空间
    if(farme_ctx->rx_buf)
    {
        mrsToolsFree(farme_ctx->rx_buf);
        farme_ctx->rx_len = 0;
    }

    farme_ctx->rx_buf = mrsToolsMalloc(len);
    if(farme_ctx->rx_buf == HI_NULL)
    {
        return HI_ERR_NO_MORE_MEMORY;
    }

    (hi_void)memcpy_s(farme_ctx->rx_buf, len, data, len);
    farme_ctx->rx_len = (HI_S16)len;

    ctx->protocol = protocol;

    return HI_ERR_SUCCESS;
}

// 多个串行645帧发送接收处理函数
HI_U32 mrsParallelMutliFrameProcess(MRS_STA_ITEM * queue_item, HI_U8 * data, 
                                        HI_U16 len, HI_U8 protocol)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    MRS_PARALLEL_STA_CTX * ctx = mrsStaGetParallelCtx();

    HI_ASSERT(ctx);

    if((!queue_item) || (!data))
    {
        return HI_ERR_FAILURE;
    }

    ret = mrsParallelReceiveData(data, len, protocol);
    if(HI_ERR_SUCCESS == ret)
    {
        // 读表序号步进-判断读表是否为一轮并更新轮次相关数据
        mrsParallelIndexStepper(ctx);

        // 重读处理-判断是否进入重读轮次并进行重读处理
        ret = mrsParallelTryProcess(ctx);
        if(ret == HI_ERR_CONTINUE)
        {
            MRS_DATA_BOUNDARY_CHECK(ctx->gap_timer, MRS_PARALLEL_GAP_TIMER_MAX, MRS_PARALLEL_GAP_TIMER_MIN);
            MRS_StartTimer(MRS_STA_TIMER_TTL, ctx->gap_timer, HI_SYS_TIMER_ONESHOT);
            ctx->gap_flg = HI_TRUE;
            
            return HI_ERR_CONTINUE;
        }        
    }

    // 组织PLC包上报到CCO
    ret = mrsParallelSendPlcToCco(queue_item, ctx);
    mrsParallelInitStaCtx();
    
    return ret;
}

// 检查是否需要重读
HI_BOOL mrsParallelNeedTry(MRS_PARALLEL_STA_CTX * ctx)
{
    HI_BOOL ret = HI_TRUE;

    if(ctx->ok_num == ctx->frame_num)
    {
        // 所有项都抄读成功
        return HI_FALSE;        
    }

    if(ctx->trun_num - 1 >= ctx->try_max)    // 当前轮次比重试轮次恒大于1
    {
        // 超过最大重试轮次
        return HI_FALSE;
    }

    return ret;
}


// 刷新下一个需要重试帧的序号-如果不存在返回HI_ERR_NOT_FOUND
HI_U32 mrsParallelRefreshNextTryIndex(MRS_PARALLEL_STA_CTX * ctx)
{
    HI_U32 ret = HI_ERR_NOT_FOUND;
    HI_S16 found = -1;
    HI_U16 i = 0;

    HI_ASSERT(ctx);
 
    i = ctx->index;

    while((i < ctx->frame_num)
            && (i < MRS_PARALLEL_645_FRAME_MAX))
    {
        if(BIT32_GET(ctx->resp_flg,i) == HI_FALSE)
        {
            if(ctx->noresp_try_flag == HI_TRUE)
            {
                // 未响应进行重读
                found = (HI_S16)i;
                break;
            }
        }
        else
        {
            // 已经响应
            if(ctx->nak_try_flag == HI_TRUE)
            {
                // 否认帧重读开启
                if(BIT32_GET(ctx->nak_flg,i) == HI_TRUE)
                {
                    // 收到否认帧
                    found = (HI_S16)i;
                    break;
                }
            }
        }

        i++;
    }

    if(found != -1)
    {
        ctx->index = (HI_U8)found;
        ret = HI_ERR_CONTINUE;
    }
    else
    {
        ctx->index = ctx->frame_num - 1; // 没有找到-输出最高的索引号
    }

    return ret;
}

/* BEGIN: Modified by liuxipeng/lKF54842,2014/4/26 问题单号:DTS2014042407354 */
// 电表故障判断
HI_BOOL mrsIsMeterFault(MRS_PARALLEL_STA_CTX * ctx)
{
    if(ctx->frame_num >= MRS_CHECK_METER_FAULT_FRAME_NUM)
    {
        if(ctx->index >= (MRS_CHECK_METER_FAULT_FRAME_NUM - 1))
        {
            if(ctx->resp_flg == 0)
            {
                return HI_TRUE;
            }
        }
    }
    
    return HI_FALSE;
}
/* END  : Modified by liuxipeng/lKF54842,2014/4/26 问题单号:DTS2014042407354 */

// 并发重读处理
HI_U32 mrsParallelTryProcess(MRS_PARALLEL_STA_CTX * ctx)
{
    HI_U32 ret = HI_ERR_CONTINUE;
    
    HI_ASSERT(ctx);

    if(ctx->trun_num > 0)
    {
        // 进入重读轮次
        while(mrsParallelNeedTry(ctx))
        {
            // 允许重读
            ret = mrsParallelRefreshNextTryIndex(ctx);  // 刷新下一个要重读帧的序号
            if(ret == HI_ERR_CONTINUE)
            {
                return ret; // 找到了-继续读
            }
            else
            {
                mrsParallelIndexStepper(ctx);   // 切换到下一轮次继续判断
            }            
        }

        // 不需要重读-将缓冲区数据上报CCO
        ret = HI_ERR_SUCCESS;
    }

    return ret;
}

/* BEGIN: Modified by liuxipeng/lKF54842,2014/4/26 问题单号:DTS2014042407354 */
// 返回HI_TRUE:过滤;返回HI_FALSE:不过滤
HI_BOOL mrsParallelBroadCastFiltrate(MRS_STA_SRV_CTX_STRU *pstStaCtx, 
                                            HI_U8 *pData, HI_U16 usLen)
{
    MRS_645BUF_INF stInf = {0};

    if(mrsStaGetSupportVM() == NV_SIMU_METER_DISABLE)
    {
        // 模拟表处于关闭状态
        if (mrsIsRealClti() || mrsToolsIsIICollector())
        {
            // 采集器产品形态-不过滤
            return HI_FALSE;
        }
    }
    
    mrsCountBuf645Inf(pData, usLen, &stInf);

    if(stInf.pucAddr)
    {
        HI_BOOL result = HI_FALSE;
        HI_U16 index = 0;
        
        result = mrsFindMeterList(&pstStaCtx->stMeterList, stInf.pucAddr, &index);
        if(result == HI_FALSE)
        {
            // 不是本站点广播读表命令-过滤
            return HI_TRUE ;
        }
    }

    return HI_FALSE;
}
/* END  : Modified by liuxipeng/lKF54842,2014/4/26 问题单号:DTS2014042407354 */

// 协议兼容性处理
HI_U32 mrsParallelCompatibleDl(HI_IN MRS_DATA_TRANSMIT_AFNF1_DL_STRU * t_data,
                                            HI_IN HI_U16 t_len, HI_OUT HI_PBYTE * data)
{
    HI_U16 total_len = 0;

    if((!t_data) || (!data))
    {
        return HI_ERR_FAILURE;
    }
    
    total_len = t_data->stru_size + t_data->data_len;

    if(total_len != t_len)
    {
        HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_015, HI_DIAG_MT("len err : [%d][%d]"), total_len, t_len);
        *data = HI_NULL;
        return HI_ERR_BAD_DATA;
    }
    
    switch(t_data->interface_ver)
    {
    case MRS_AFN_F1_DL_VER_R0:
    default:
        total_len = sizeof(MRS_DATA_TRANSMIT_AFNF1_DL_STRU) + t_data->data_len;
        if(t_len < total_len)
        {
            HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_017, HI_DIAG_MT("len too short : [%d][%d]"), total_len, t_len);
            return HI_ERR_BAD_DATA;
        }
        
        break;
    }
    
    *data = (HI_U8 *)t_data + t_data->stru_size;
    
    return HI_ERR_SUCCESS;
}

// 并发读表PLC数据接收-下行(CCO->STA)
HI_U32 mrsParallelReceivePackPlcDl(HI_IN MRS_DATA_TRANSMIT_AFNF1_DL_STRU * t_data,
                                            HI_IN HI_U16 t_len, HI_IN HI_BOOL bc_flg)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    MRS_STA_SRV_CTX_STRU *sta = mrsStaGetContext();
    MRS_STA_ITEM * item = HI_NULL;
    HI_PBYTE data = HI_NULL;
    HI_U16 data_len = 0;

    if(!t_data)
    {
        return HI_ERR_FAILURE;
    }

    // 兼容性检查
    ret = mrsParallelCompatibleDl(t_data, t_len, &data);
    if(ret != HI_ERR_SUCCESS)
    {
        return ret;
    }

    data_len = t_data->data_len;

/* BEGIN: Modified by liuxipeng/lKF54842,2014/4/26 问题单号:DTS2014042407354 */
    // 广播帧过滤
    if(bc_flg)
    {
        HI_BOOL result;
        
        result = mrsParallelBroadCastFiltrate(sta, data, data_len);
        if(result == HI_TRUE)
        {
            return HI_ERR_SUCCESS;
        }
    }

    mrsDfxPrPlcRx();
    mrsDfxRmPlcDlRx(ID_MRS_CMD_PARALLEL_DATA_PLC, (HI_U8 *)t_data, HI_FALSE);
/* END  : Modified by liuxipeng/lKF54842,2014/4/26 问题单号:DTS2014042407354 */

    if (mrsToolsIsIICollector())
    {
		ret = mrsParallelIrdaPlcProc(t_data->seq, data, data_len);
		if (HI_ERR_CONTINUE != ret)
		{
			return ret;
		}
	}
	
    //将数据发送到队列以后处理
    item = mrsToolsMalloc(sizeof(MRS_STA_ITEM) + data_len);
    if (item == HI_NULL) 
    {
        return HI_ERR_FAILURE;
    }
    (hi_void)memset_s(item,sizeof(MRS_STA_ITEM) + data_len, 0,sizeof(MRS_STA_ITEM) + data_len);

    item->bcFlg = bc_flg;  // 获取广播标志
    item->from = MRS_STA_QUEUE_FROM_PLC;
    item->time_out = t_data->time_out; // 获取超时时间(MRS_METER_TIMER_UNIT_BASE ms)
    item->id = ID_MRS_CMD_PARALLEL_DATA_PLC;
    item->len = data_len;                               // 获取数据长度
    item->buf = (HI_U8 *)item + sizeof(MRS_STA_ITEM);   // 偏移数据缓冲区指针
    item->noresp_try_flag = (HI_U8)t_data->noresp_try_flag;    // 获取无响应后重试标志
    item->nak_try_flag = (HI_U8)t_data->nak_try_flag;          // 获取响应否认后重试标志
    item->try_max = (HI_U8)t_data->try_max;                    // 获取最大重试次数
    item->gap_timer = mrsGet645MeterInterval(MRS_STA_RM_CFG_AUTO); // 获取帧间隔时间(MRS_GAP_TIMER_UNIT_BASE ms)
    item->seq = t_data->seq;                            // 获取帧序号
    (hi_void)memcpy_s(item->buf, data_len, data, data_len);          // 报文内容    

    item->time_out = (HI_U8)MRS_MS_TO_100MS(mrsGet645FrameHeadTimeout(MRS_STA_RM_CFG_AUTO));
    item->bAutoStrategy = HI_TRUE;
    item->ucFrameTimeout = (HI_U8)MRS_MS_TO_100MS(mrsGet645FrameInterval(MRS_STA_RM_CFG_AUTO));

    //尝试加入队列
    if(!mrsStaTryEnQueue(&sta->stQueModule,item))
    {
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_023, HI_DIAG_MT("Enter queue fail"));
        mrsToolsFree(item);
    }
    #if defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG) && defined(MRS_LOG_STA_SWITCH)
    else
    {
        mrsLogFrameInfStatTx(item->buf, item->len, HI_FALSE);
    }
    #endif

    //通知队列
    mrsStaNotifyQueue();

    return ret;
}

// 处理CCO数据转发命令
HI_U32 mrsParallelCmdTransmit(HI_IN HI_U16 usId, HI_PVOID pReqPacket, HI_U16 usReqPacketSize)
{
    HI_U32 ret = HI_ERR_FAILURE;
    MRS_PLC_FRAME_DATA_STRU * plc_farme;
    MRS_DATA_TRANSMIT_AFNF1_DL_STRU * t_data = 0;
    
    plc_farme = (MRS_PLC_FRAME_DATA_STRU *)pReqPacket;
    MRS_NOT_USED(usReqPacketSize);

    //检查参数
	if (!plc_farme || !plc_farme->pucPayload 
		|| (usId != ID_MRS_CMD_PARALLEL_DATA_PLC))
	{
		HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_024, HI_DIAG_MT("param null"));
		return HI_ERR_FAILURE;
	}

#if defined(PRODUCT_CFG_SYS_RST_SOLUTION)
	{
	    MRS_STA_SRV_CTX_STRU *sta = HI_NULL;

	    sta = mrsStaGetContext();
		sta->stStaResetInfo.ulRxRMPlcTimes++;
	}
#endif

    t_data = (MRS_DATA_TRANSMIT_AFNF1_DL_STRU *)plc_farme->pucPayload;    

    ret = mrsParallelReceivePackPlcDl(t_data, plc_farme->usPayloadLen, plc_farme->is_bc_frame);

    return ret;
}


// 并发读表队列处理
HI_U32 mrsParallelQueueProcess(MRS_STA_ITEM * queue_item, MRS_PARALLEL_STA_CTX * ctx)
{
    HI_U32 ret = HI_ERR_SUCCESS;

    HI_ASSERT(ctx);

    if(HI_TRUE == mrsIsOldFrame())     // 是旧的帧
    {
        if (HI_FALSE == ctx->gap_flg)  // 不是处理帧间隔定时器
        {
        /* BEGIN: Modified by liuxipeng/lKF54842,2014/4/26 问题单号:DTS2014042407354 */
            if(mrsIsMeterFault(ctx))
            {
                mrsParallelInitStaCtx();
                return HI_ERR_SUCCESS;
            }
        /* END  : Modified by liuxipeng/lKF54842,2014/4/26 问题单号:DTS2014042407354 */            
        
            // 读表序号步进-判断读表是否为一轮并更新轮次相关数据
            mrsParallelIndexStepper(ctx);
            
            // 重读处理-判断是否进入重读轮次并进行重读处理
            ret = mrsParallelTryProcess(ctx);
            if(ret != HI_ERR_CONTINUE)
            {
                // 所有的帧已经处理完毕
                (HI_VOID)mrsParallelSendPlcToCco(queue_item, ctx);
    
                mrsParallelInitStaCtx();
                
                return HI_ERR_SUCCESS;
            }
        }
    }
    
    // 准备发送下一帧
    ctx->gap_flg = HI_FALSE;
    
    return HI_ERR_CONTINUE;
}

// 刷新并发读表帧响应标志
HI_VOID mrsParallelRefreshRespFlg(HI_BOOL flg)
{
    MRS_PARALLEL_STA_CTX * ctx = mrsStaGetParallelCtx();

    HI_ASSERT(ctx);

    flg? BIT32_SET(ctx->resp_flg, ctx->index) : BIT32_CLR(ctx->resp_flg, ctx->index);

    return;
}

// 刷新并发读表帧状态
HI_VOID mrsParallelRefreshFrameStatus(HI_BOOL flg)
{
    MRS_PARALLEL_STA_CTX * ctx = mrsStaGetParallelCtx();

    HI_ASSERT(ctx);

    if (HI_TRUE == flg)
    {
        // 电表返回的645否认帧标志置位
        BIT32_SET(ctx->nak_flg, ctx->index);
    }
    else
    {
        // 电表返回的645否认帧标志复位
        BIT32_CLR(ctx->nak_flg, ctx->index);

        // 更新正确帧计数
        ctx->ok_num++;
         
    }

    return;
}

//*****************************************************************************
// 函数名称: mrsParallelIrdaPlcProc
// 功能描述: II采收到并发红外PLC命令处理
//           
// 参数说明:
//   usSeq      帧序号
//   pData      645帧报文
//   usDataLen  645帧报文长度
//
// 返 回 值: 
//    HI_ERR_SUCCESS: 成功
//    其它错误码
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: fengxiaomin/00209182 [2015-11-16]
//*****************************************************************************
HI_U32 mrsParallelIrdaPlcProc(HI_U16 usSeq, HI_U8 *pData, HI_U16 usDataLen)
{
	HI_U32 ulRet = HI_ERR_SUCCESS;
	MRS_STA_ITEM stItem;
	MRS_PARALLEL_STA_CTX *pstCtx = mrsStaGetParallelCtx();
	
    (hi_void)memset_s(&stItem, sizeof(stItem), 0, sizeof(MRS_STA_ITEM));
	stItem.buf = pData;
	stItem.len = usDataLen;
	mrsParallelSetStaCtx(&stItem, pstCtx);

	if (!pstCtx->frame_num)
	{
		return HI_ERR_CONTINUE;
	}

	ulRet = mrsParallelCheckIrda(pstCtx);
	if (HI_ERR_SUCCESS != ulRet)
	{
		return HI_ERR_CONTINUE;
	}
	
	mrsParallelIrdaPlcResp(pstCtx, usSeq, pData, usDataLen);
	
	return HI_ERR_SUCCESS;
}

//*****************************************************************************
// 函数名称: mrsParallelCheckIrda
// 功能描述: II采判断收到并发抄表命令是否是红外PLC命令
//           
// 参数说明:
//   pstCtx     STA并发抄表上下文
//
// 返 回 值: 
//    HI_ERR_SUCCESS: 红外PLC命令
//    其它错误码
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: fengxiaomin/00209182 [2015-11-16]
//*****************************************************************************
HI_U32 mrsParallelCheckIrda(MRS_PARALLEL_STA_CTX *pstCtx)
{
	HI_U32 ulRet = HI_ERR_SUCCESS;
	HI_U8 ucIndex = 0;
	HI_U8 aucDI[MRS_645_FRAME_DATA_DI_SIZE] = {0};
    MRS_IR_645_DI_FUN pIrDIProc = HI_NULL;
	
	ulRet = mrsIrDACheckAddr(pstCtx->frame_ctx[0].tx_buf + MRS_645_FRAME_METERADD_OFFSET, MRS_CLTII_TRANSFER_FROM);
	if (HI_ERR_SUCCESS != ulRet)
	{
		return ulRet;
	}

	for (ucIndex = 0; ucIndex < pstCtx->frame_num; ucIndex++)
	{
		HI_U8 *pData = pstCtx->frame_ctx[ucIndex].tx_buf;

	    (hi_void)memcpy_s(aucDI, sizeof(aucDI), pData + MRS_645_FRAME_DATA_OFFSET, MRS_645_FRAME_DATA_DI_SIZE);
		mrs645DataDecode(aucDI, MRS_645_FRAME_DATA_DI_SIZE);
		ulRet = mrsIrDAGetDIFunc(pData[MRS_645_FRAME_CTRL_OFFSET], aucDI, (HI_U32*)&pIrDIProc);
		if (HI_ERR_SUCCESS == ulRet)
		{
			return ulRet;
		}
	}

	return HI_ERR_FAILURE;
}

//*****************************************************************************
// 函数名称: mrsParallelIrdaPlcResp
// 功能描述: II采应答并发抄表PLC帧给CCO
//           
// 参数说明:
//   pstCtx     STA并发抄表上下文
//   usSeq      帧序号
//   pData      645帧报文
//   usDataLen  645帧报文长度
//
// 返 回 值: 
//    HI_ERR_SUCCESS: 红外PLC命令
//    其它错误码
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: fengxiaomin/00209182 [2015-11-16]
//*****************************************************************************
HI_U32 mrsParallelIrdaPlcResp(MRS_PARALLEL_STA_CTX *pstCtx, HI_U16 usSeq, HI_U8 *pData, HI_U16 usDataLen)
{
	HI_U32 ulRet = HI_ERR_SUCCESS;
	HI_U8 ucIndex = 0;
	MRS_STA_ITEM stItem;
    MRS_IR_645_DI_FUN pIrDIProc = HI_NULL;
	MRS_COLLECTOR_645_FRAME_STRU *pst645Frame = HI_NULL;

	//解析645帧
	pst645Frame = (MRS_COLLECTOR_645_FRAME_STRU *)mrsToolsMalloc(sizeof(MRS_COLLECTOR_645_FRAME_STRU));
	if(pst645Frame == HI_NULL)
	{
		return HI_ERR_MALLOC_FAILUE;
	}

    (hi_void)memset_s(pst645Frame, sizeof(MRS_COLLECTOR_645_FRAME_STRU), 0, sizeof(MRS_COLLECTOR_645_FRAME_STRU));

	for (ucIndex = 0; ucIndex < pstCtx->frame_num; ucIndex++)
	{
		ulRet = HI_ERR_FAILURE;
		mrsIrDA645Dec(pstCtx->frame_ctx[ucIndex].tx_buf, (HI_U16)pstCtx->frame_ctx[ucIndex].tx_len, 
					  pst645Frame, MRS_CLTII_TRANSFER_FROM);
		
		pIrDIProc = mrsIrFrameProc(pst645Frame->ucCtrl, pst645Frame->ucDataRealm);
		if (pIrDIProc)
		{
			ulRet = pIrDIProc(pst645Frame, (HI_PVOID *)&pData, &usDataLen, 0);
		}

		if (ulRet != HI_ERR_SUCCESS)
		{
			mrsAssembleDFrame((HI_VOID **)&pData, &usDataLen, MRS_IR_ERR_NOT_EXIST, pst645Frame, 0);
		}
		
		pstCtx->frame_ctx[ucIndex].rx_buf = pData;
		pstCtx->frame_ctx[ucIndex].rx_len = (HI_S16)usDataLen;
		
		BIT32_SET(pstCtx->resp_flg, pstCtx->index);
		pstCtx->index++;
	} 

    (hi_void)memset_s(&stItem, sizeof(stItem), 0, sizeof(stItem));
	stItem.seq = usSeq;
	stItem.id = ID_MRS_CMD_PARALLEL_DATA_PLC;
	pstCtx->protocol = MRS_PROTO645_VER_2007;
	
	mrsParallelSendPlcToCco(&stItem, pstCtx);
    mrsParallelInitStaCtx();
	mrsToolsFree(pst645Frame);

	return HI_ERR_SUCCESS;
}

#endif

