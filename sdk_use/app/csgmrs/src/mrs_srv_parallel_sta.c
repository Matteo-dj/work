//*****************************************************************************
//
//                  ��Ȩ���� (C), 1998-2011, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mrs_srv_parallel_sta.c
//  �� �� ��   : V1.0 
//  ��    ��   : liuxipeng/KF54842
//  ��������   : 2012-12-22
//  ��������   : ��������ģ�麯��ʵ��
//               
//  �����б�   :
//  �޸���ʷ   : 
//  1.��    �� : 2012-12-22
//    ��    �� : liuxipeng/KF54842
//    �޸����� : �����ļ� 
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

// �����ض�����
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
                                            
/* BEGIN: Modified by liuxipeng/lKF54842,2014/4/26 ���ⵥ��:DTS2014042407354 */
// �������ж�
HI_BOOL mrsIsMeterFault(MRS_PARALLEL_STA_CTX * ctx);
// �㲥֡����                                   
HI_BOOL mrsParallelBroadCastFiltrate(MRS_STA_SRV_CTX_STRU *pstStaCtx, 
                                            HI_U8 *pData, HI_U16 usLen);
/* END  : Modified by liuxipeng/lKF54842,2014/4/26 ���ⵥ��:DTS2014042407354 */

HI_PRV HI_U32 mrsParallelIrdaPlcResp(MRS_PARALLEL_STA_CTX *pstCtx, HI_U16 usSeq, HI_U8 *pData, HI_U16 usDataLen);
HI_PRV HI_U32 mrsParallelCheckIrda(MRS_PARALLEL_STA_CTX *pstCtx);
HI_PRV HI_U32 mrsParallelIrdaPlcProc(HI_U16 usSeq, HI_U8 *pData, HI_U16 usDataLen);

// *****************************************

// ��ʼ��STA�˲�������������
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

// ����STA�˲�������������
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

    // ��ʼ������������(�ͷ��ڴ�-����)
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
    
    // ���ҵ���Ч֡�����η���֡��������
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

// ��ȡ�������ջ���������(��֡���Ⱥ�)
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

// ���ο������յ���645���ݵ�PLC��������ȥ
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

    // �����ʵ�������ݵĳ���
    *out_len = temp_len;

    return ret;
}

// �������ж������ӦPLC֡������
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
 
    //�ܳ��� = �Ự�ṹ + ת���ṹ + ����
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
    
    //��������
    ret = mrsParallelCopyRxDataToPlcBuf(ctx, t_data->data, &copy_len);
    if(ret != HI_ERR_SUCCESS)
    {
        mrsToolsFree(t_data);
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_005, HI_DIAG_MT("get buf err!"));
        return HI_ERR_FAILURE;
    }
    
    t_data->data_len = copy_len;
    
    //���CCO��ַ(ȫ0)
    (hi_void)memset_s(plc_data.ucMac, sizeof(plc_data.ucMac), 0, sizeof(plc_data.ucMac));

    plc_data.usId = queue_item->id;    // ��ȡҵ��ID 
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

/* BEGIN: Modified by fengxiaomin/00209182, 2014/3/27   ���ⵥ��:DTS2014032906596 */
    mrsDfxRmPlcUpAfn(ID_MRS_CMD_PARALLEL_DATA_PLC, plc_data.pucPayload, ret);
/* END:   Modified by fengxiaomin/00209182, 2014/3/27 */

    mrsToolsFree(t_data);

    return ret;
}

// ���������ִδ���
HI_VOID mrsParallelIndexStepper(MRS_PARALLEL_STA_CTX * ctx)
{
    HI_ASSERT(ctx);

    ctx->index++;
    
    if(ctx->index >= ctx->frame_num)
    {
        // �������
        ctx->index = 0;

        // �����ִ�����
        ctx->trun_num++;
    }

    return;
}

// ���յ�����ݴ���
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

    // �����Ӧ�����֡ʱ�ÿռ��Ѿ���ʹ��, �ض�ʱ��Ҫ�ͷŸÿռ�
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

// �������645֡���ͽ��մ�����
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
        // ������Ų���-�ж϶����Ƿ�Ϊһ�ֲ������ִ��������
        mrsParallelIndexStepper(ctx);

        // �ض�����-�ж��Ƿ�����ض��ִβ������ض�����
        ret = mrsParallelTryProcess(ctx);
        if(ret == HI_ERR_CONTINUE)
        {
            MRS_DATA_BOUNDARY_CHECK(ctx->gap_timer, MRS_PARALLEL_GAP_TIMER_MAX, MRS_PARALLEL_GAP_TIMER_MIN);
            MRS_StartTimer(MRS_STA_TIMER_TTL, ctx->gap_timer, HI_SYS_TIMER_ONESHOT);
            ctx->gap_flg = HI_TRUE;
            
            return HI_ERR_CONTINUE;
        }        
    }

    // ��֯PLC���ϱ���CCO
    ret = mrsParallelSendPlcToCco(queue_item, ctx);
    mrsParallelInitStaCtx();
    
    return ret;
}

// ����Ƿ���Ҫ�ض�
HI_BOOL mrsParallelNeedTry(MRS_PARALLEL_STA_CTX * ctx)
{
    HI_BOOL ret = HI_TRUE;

    if(ctx->ok_num == ctx->frame_num)
    {
        // ����������ɹ�
        return HI_FALSE;        
    }

    if(ctx->trun_num - 1 >= ctx->try_max)    // ��ǰ�ִα������ִκ����1
    {
        // ������������ִ�
        return HI_FALSE;
    }

    return ret;
}


// ˢ����һ����Ҫ����֡�����-��������ڷ���HI_ERR_NOT_FOUND
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
                // δ��Ӧ�����ض�
                found = (HI_S16)i;
                break;
            }
        }
        else
        {
            // �Ѿ���Ӧ
            if(ctx->nak_try_flag == HI_TRUE)
            {
                // ����֡�ض�����
                if(BIT32_GET(ctx->nak_flg,i) == HI_TRUE)
                {
                    // �յ�����֡
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
        ctx->index = ctx->frame_num - 1; // û���ҵ�-�����ߵ�������
    }

    return ret;
}

/* BEGIN: Modified by liuxipeng/lKF54842,2014/4/26 ���ⵥ��:DTS2014042407354 */
// �������ж�
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
/* END  : Modified by liuxipeng/lKF54842,2014/4/26 ���ⵥ��:DTS2014042407354 */

// �����ض�����
HI_U32 mrsParallelTryProcess(MRS_PARALLEL_STA_CTX * ctx)
{
    HI_U32 ret = HI_ERR_CONTINUE;
    
    HI_ASSERT(ctx);

    if(ctx->trun_num > 0)
    {
        // �����ض��ִ�
        while(mrsParallelNeedTry(ctx))
        {
            // �����ض�
            ret = mrsParallelRefreshNextTryIndex(ctx);  // ˢ����һ��Ҫ�ض�֡�����
            if(ret == HI_ERR_CONTINUE)
            {
                return ret; // �ҵ���-������
            }
            else
            {
                mrsParallelIndexStepper(ctx);   // �л�����һ�ִμ����ж�
            }            
        }

        // ����Ҫ�ض�-�������������ϱ�CCO
        ret = HI_ERR_SUCCESS;
    }

    return ret;
}

/* BEGIN: Modified by liuxipeng/lKF54842,2014/4/26 ���ⵥ��:DTS2014042407354 */
// ����HI_TRUE:����;����HI_FALSE:������
HI_BOOL mrsParallelBroadCastFiltrate(MRS_STA_SRV_CTX_STRU *pstStaCtx, 
                                            HI_U8 *pData, HI_U16 usLen)
{
    MRS_645BUF_INF stInf = {0};

    if(mrsStaGetSupportVM() == NV_SIMU_METER_DISABLE)
    {
        // ģ����ڹر�״̬
        if (mrsIsRealClti() || mrsToolsIsIICollector())
        {
            // �ɼ�����Ʒ��̬-������
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
            // ���Ǳ�վ��㲥��������-����
            return HI_TRUE ;
        }
    }

    return HI_FALSE;
}
/* END  : Modified by liuxipeng/lKF54842,2014/4/26 ���ⵥ��:DTS2014042407354 */

// Э������Դ���
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

// ��������PLC���ݽ���-����(CCO->STA)
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

    // �����Լ��
    ret = mrsParallelCompatibleDl(t_data, t_len, &data);
    if(ret != HI_ERR_SUCCESS)
    {
        return ret;
    }

    data_len = t_data->data_len;

/* BEGIN: Modified by liuxipeng/lKF54842,2014/4/26 ���ⵥ��:DTS2014042407354 */
    // �㲥֡����
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
/* END  : Modified by liuxipeng/lKF54842,2014/4/26 ���ⵥ��:DTS2014042407354 */

    if (mrsToolsIsIICollector())
    {
		ret = mrsParallelIrdaPlcProc(t_data->seq, data, data_len);
		if (HI_ERR_CONTINUE != ret)
		{
			return ret;
		}
	}
	
    //�����ݷ��͵������Ժ���
    item = mrsToolsMalloc(sizeof(MRS_STA_ITEM) + data_len);
    if (item == HI_NULL) 
    {
        return HI_ERR_FAILURE;
    }
    (hi_void)memset_s(item,sizeof(MRS_STA_ITEM) + data_len, 0,sizeof(MRS_STA_ITEM) + data_len);

    item->bcFlg = bc_flg;  // ��ȡ�㲥��־
    item->from = MRS_STA_QUEUE_FROM_PLC;
    item->time_out = t_data->time_out; // ��ȡ��ʱʱ��(MRS_METER_TIMER_UNIT_BASE ms)
    item->id = ID_MRS_CMD_PARALLEL_DATA_PLC;
    item->len = data_len;                               // ��ȡ���ݳ���
    item->buf = (HI_U8 *)item + sizeof(MRS_STA_ITEM);   // ƫ�����ݻ�����ָ��
    item->noresp_try_flag = (HI_U8)t_data->noresp_try_flag;    // ��ȡ����Ӧ�����Ա�־
    item->nak_try_flag = (HI_U8)t_data->nak_try_flag;          // ��ȡ��Ӧ���Ϻ����Ա�־
    item->try_max = (HI_U8)t_data->try_max;                    // ��ȡ������Դ���
    item->gap_timer = mrsGet645MeterInterval(MRS_STA_RM_CFG_AUTO); // ��ȡ֡���ʱ��(MRS_GAP_TIMER_UNIT_BASE ms)
    item->seq = t_data->seq;                            // ��ȡ֡���
    (hi_void)memcpy_s(item->buf, data_len, data, data_len);          // ��������    

    item->time_out = (HI_U8)MRS_MS_TO_100MS(mrsGet645FrameHeadTimeout(MRS_STA_RM_CFG_AUTO));
    item->bAutoStrategy = HI_TRUE;
    item->ucFrameTimeout = (HI_U8)MRS_MS_TO_100MS(mrsGet645FrameInterval(MRS_STA_RM_CFG_AUTO));

    //���Լ������
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

    //֪ͨ����
    mrsStaNotifyQueue();

    return ret;
}

// ����CCO����ת������
HI_U32 mrsParallelCmdTransmit(HI_IN HI_U16 usId, HI_PVOID pReqPacket, HI_U16 usReqPacketSize)
{
    HI_U32 ret = HI_ERR_FAILURE;
    MRS_PLC_FRAME_DATA_STRU * plc_farme;
    MRS_DATA_TRANSMIT_AFNF1_DL_STRU * t_data = 0;
    
    plc_farme = (MRS_PLC_FRAME_DATA_STRU *)pReqPacket;
    MRS_NOT_USED(usReqPacketSize);

    //������
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


// ����������д���
HI_U32 mrsParallelQueueProcess(MRS_STA_ITEM * queue_item, MRS_PARALLEL_STA_CTX * ctx)
{
    HI_U32 ret = HI_ERR_SUCCESS;

    HI_ASSERT(ctx);

    if(HI_TRUE == mrsIsOldFrame())     // �Ǿɵ�֡
    {
        if (HI_FALSE == ctx->gap_flg)  // ���Ǵ���֡�����ʱ��
        {
        /* BEGIN: Modified by liuxipeng/lKF54842,2014/4/26 ���ⵥ��:DTS2014042407354 */
            if(mrsIsMeterFault(ctx))
            {
                mrsParallelInitStaCtx();
                return HI_ERR_SUCCESS;
            }
        /* END  : Modified by liuxipeng/lKF54842,2014/4/26 ���ⵥ��:DTS2014042407354 */            
        
            // ������Ų���-�ж϶����Ƿ�Ϊһ�ֲ������ִ��������
            mrsParallelIndexStepper(ctx);
            
            // �ض�����-�ж��Ƿ�����ض��ִβ������ض�����
            ret = mrsParallelTryProcess(ctx);
            if(ret != HI_ERR_CONTINUE)
            {
                // ���е�֡�Ѿ��������
                (HI_VOID)mrsParallelSendPlcToCco(queue_item, ctx);
    
                mrsParallelInitStaCtx();
                
                return HI_ERR_SUCCESS;
            }
        }
    }
    
    // ׼��������һ֡
    ctx->gap_flg = HI_FALSE;
    
    return HI_ERR_CONTINUE;
}

// ˢ�²�������֡��Ӧ��־
HI_VOID mrsParallelRefreshRespFlg(HI_BOOL flg)
{
    MRS_PARALLEL_STA_CTX * ctx = mrsStaGetParallelCtx();

    HI_ASSERT(ctx);

    flg? BIT32_SET(ctx->resp_flg, ctx->index) : BIT32_CLR(ctx->resp_flg, ctx->index);

    return;
}

// ˢ�²�������֡״̬
HI_VOID mrsParallelRefreshFrameStatus(HI_BOOL flg)
{
    MRS_PARALLEL_STA_CTX * ctx = mrsStaGetParallelCtx();

    HI_ASSERT(ctx);

    if (HI_TRUE == flg)
    {
        // ����ص�645����֡��־��λ
        BIT32_SET(ctx->nak_flg, ctx->index);
    }
    else
    {
        // ����ص�645����֡��־��λ
        BIT32_CLR(ctx->nak_flg, ctx->index);

        // ������ȷ֡����
        ctx->ok_num++;
         
    }

    return;
}

//*****************************************************************************
// ��������: mrsParallelIrdaPlcProc
// ��������: II���յ���������PLC�����
//           
// ����˵��:
//   usSeq      ֡���
//   pData      645֡����
//   usDataLen  645֡���ĳ���
//
// �� �� ֵ: 
//    HI_ERR_SUCCESS: �ɹ�
//    ����������
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: fengxiaomin/00209182 [2015-11-16]
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
// ��������: mrsParallelCheckIrda
// ��������: II���ж��յ��������������Ƿ��Ǻ���PLC����
//           
// ����˵��:
//   pstCtx     STA��������������
//
// �� �� ֵ: 
//    HI_ERR_SUCCESS: ����PLC����
//    ����������
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: fengxiaomin/00209182 [2015-11-16]
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
// ��������: mrsParallelIrdaPlcResp
// ��������: II��Ӧ�𲢷�����PLC֡��CCO
//           
// ����˵��:
//   pstCtx     STA��������������
//   usSeq      ֡���
//   pData      645֡����
//   usDataLen  645֡���ĳ���
//
// �� �� ֵ: 
//    HI_ERR_SUCCESS: ����PLC����
//    ����������
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: fengxiaomin/00209182 [2015-11-16]
//*****************************************************************************
HI_U32 mrsParallelIrdaPlcResp(MRS_PARALLEL_STA_CTX *pstCtx, HI_U16 usSeq, HI_U8 *pData, HI_U16 usDataLen)
{
	HI_U32 ulRet = HI_ERR_SUCCESS;
	HI_U8 ucIndex = 0;
	MRS_STA_ITEM stItem;
    MRS_IR_645_DI_FUN pIrDIProc = HI_NULL;
	MRS_COLLECTOR_645_FRAME_STRU *pst645Frame = HI_NULL;

	//����645֡
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

