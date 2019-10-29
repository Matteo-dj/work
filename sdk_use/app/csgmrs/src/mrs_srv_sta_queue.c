//*****************************************************************************
//
//                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mrs_srv_sta_queue.c
//  �� �� ��   : V1.0 
//  ��    ��   : ������/KF62735
//  ��������   : 2012-07-11
//  ��������   : STA����
//  �����б�   : NA
//  �޸���ʷ   : 
//  1.��    �� : 2012-07-11
//    ��    �� : ������/KF62735
//    �޸����� : �����ļ� 
//*****************************************************************************
#include "mrs_common.h"
#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
#include "mrs_fw_log.h"
#include "mrs_fw_tools.h"
#include "mrs_fw_proto645.h"
#include "mrs_fw_proto376_2_echo.h"
#include "mrs_srv_common.h"
#include "mrs_srv_sta_queue.h"
#include "mrs_srv_io.h"
#include "hi_equip.h"
#include "../equ/equip_dut_proc.h"
#include "mrs_srv_collector.h"
#include "mrs_srv_collector.h"
#include "mrs_srv_parallel_sta.h"
#include "mrs_srv_sta_event.h"
#include "mrs_srv_sta.h"
#include "mrs_dfx.h"
#include "mrs_srv_sta_searchmeter.h"
#include "mrs_srv_mrslog.h"
#include "mrs_srv_clti_searchmeter.h"
#include "mrs_srv_cltii_searchmeter.h"
#include "mrs_srv_cltii_event.h"
#include "mrs_srv_sta_dutycycle.h"
#include "mrs_srv_clti_searchmeter_csg.h"
#include "mrs_srv_clti_upg_csg.h"
#include "mrs_srv_baudrate_manage.h"


struct que_param
{
    MRS_STA_ITEM    * item;    //�ڵ�
    MRS_QUE_MODULE  * module;  //STA����
    HI_U8 result;           //���
    HI_U8 padding[3];
};

//Ԫ�رȽϺ���
HI_BOOL mrsStaQueueReplace(HI_VOID *,HI_VOID *);


//*****************************************************************************
// ��������: mrsStaQueueInit
// ��������: ��ʼ������ģ��   
// ����˵��:
//   MRS_QUE_MODULE *   [IN/OUT] ����ģ��
// �� �� ֵ: 
//   ���Ϊ�շ���HI_ERR_FAILURE,����������سɹ���
// ����Ҫ��:
// ���þ���:
// ��    ��: niesongsong/nkf62735 [2012-05-29]
// ��    ע: �������ģ�鲻Ϊ�ն��ε��ô˺����������գ����ܻ����ڴ�й©!!!
//*****************************************************************************
HI_U32 mrsStaQueueInit(MRS_QUE_MODULE * queue)
{
    if (!queue) 
    {
        return HI_ERR_FAILURE;
    }
    
    //���
    (hi_void)memset_s(queue,sizeof(MRS_QUE_MODULE), 0,sizeof(MRS_QUE_MODULE));
    
    //��ʼ����
    MRS_LOCK_INIT(queue->lock);
    MRS_StopTimer(MRS_STA_TIMER_RM_QUEUE_LOCK);

    //��ʼ������
    mrsSrvInitQueue(&queue->stMrQueue);

    //������Դ���
    queue->retries = 0;

    //��ʼ�������ģ��
    mrsSrvMeterInit(&queue->stMetModule,1,mrsStaQueueAlloc,mrsStaQueueFree);

    return HI_ERR_SUCCESS;
}

//*****************************************************************************
// ��������: mrsStaQueueReset
// ��������: ���ö���   
// ����˵��:
//   MRS_QUE_MODULE *   [IN/OUT] ����ģ��
// �� �� ֵ: 
//   ���Ϊ�շ���HI_ERR_FAILURE,����������سɹ���
// ����Ҫ��:
// ��    ��: niesongsong/nkf62735 [2012-05-29]
// ��    ע: �������ģ�鲻Ϊ�������ն��У���mrsStaQueueFree�ͷŽ�㣬
//           Ȼ���ٳ�ʼ�����У��ýӿ���mrsStaQueueInit�����Ǹýӿ�
//           ����ն��С�
//*****************************************************************************
HI_U32 mrsStaQueueReset(MRS_QUE_MODULE * queue)
{
    if (queue) 
    {
        mrsSrvEmptyQueue(&queue->stMrQueue,mrsStaQueueFree);
        mrsStaQueueInit(queue);
        return HI_ERR_SUCCESS;
    }
    return HI_ERR_FAILURE;
}

//*****************************************************************************
// ��������: mrsStaTryEnQueue
// ��������: ���Խ���������� 
// ����˵��:
//   MRS_QUE_MODULE *   [IN/OUT] ����ģ��
//   MRS_STA_ITEM   *   [IN]     ����Ԫ��
// �� �� ֵ: 
//   ���Ԫ�ؼ�����гɹ�������HI_TRUE�����򷵻�HI_FALSE��
// ����Ҫ��:
// ��    ��: niesongsong/nkf62735 [2012-05-29]
// ��    ע: �ýӿڻὫԪ�ؼ�����У�����Ҳ��ע�⣬�������ʧ�ܣ�������
//           �ͷ�Ԫ�����ݣ��ýӿڲ���ȥ�ͷ�֮��
//*****************************************************************************
HI_BOOL mrsStaTryEnQueue(MRS_QUE_MODULE *queue,MRS_STA_ITEM * item)
{
    HI_U8 qlen = 0;
    MRS_SRV_QUEUE *q;
    HI_BOOL ret = HI_FALSE;
    HI_U8 qlen_max = mrsGetStaQueueMax();

    if (queue && item) 
    {
        struct que_param extra = {0};

        if(mrsStaPlcRetryFiltrate(item))
        {
            // �����ظ�PLC֡��ֹ�������
            return HI_FALSE;
        }
        
        q = &queue->stMrQueue;     

        qlen = (HI_U8)mrsSrvQueueLength(q);
        
        mrsDfxRefreshRmQueueNum(qlen);    // ����������ͳ��

        extra.item = item;
        extra.result = HI_FALSE;
        extra.module = queue;
        
        //��ͬ�Ự���ǲ���������е�
        mrsSrvTraverseQueue(&queue->stMrQueue, mrsStaQueueReplace,&extra);

        //����ҵ���ҵ����������ݰ�
        if (extra.result) 
        {
            mrsDfxRefreshRmQueueTopStatus(q, queue->lock); // ˢ�¶�����ж���״̬
            //��������
            return HI_TRUE;
        }

        qlen = (HI_U8)mrsSrvQueueLength(q);
        if (qlen >= qlen_max)
        {
            mrsDfxRmQueueFullCnt(); // ���������ͳ��
            HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_004, HI_DIAG_MT("queue is full"));
        }
        else 
        {
            mrsSrvEnQueue(q,(MRS_SRV_NODE *)item);
            ret = HI_TRUE;
        }
        mrsDfxRefreshRmQueueNum(qlen);    // ����������ͳ��
            
        //�����ǰû��Ԫ��
        if (qlen == 0) 
        {
            //������д�����
            mrsStaNotifyQueue();
        }
        mrsDfxRefreshRmQueueTopStatus(q, queue->lock); // ˢ�¶�����ж���״̬
    }
    return ret;
}

//*****************************************************************************
// ��������: mrsStaQueueFree
// ��������: �ͷŶ��еĽ��
// ����˵��:
//   HI_VOID *   [IN] ����ģ��
// �� �� ֵ: 
//   �ͷŽ�㣬һ�����ڳ����к���ն����ͷ�Ԫ�ء�
// ��    ��: niesongsong/nkf62735 [2012-05-29]
// ��    ע: �ޡ�
//*****************************************************************************
HI_VOID mrsStaQueueFree(HI_VOID * p)
{
    mrsToolsFree(p);
}

//*****************************************************************************
// ��������: mrsStaQueueAlloc
// ��������: �����ڴ�
// ����˵��:
//   HI_VOID *   [IN] ����ģ��
// �� �� ֵ: 
//   �����ڴ�
// ��    ��: niesongsong/nkf62735 [2012-05-29]
// ��    ע: �ޡ�
//*****************************************************************************
HI_VOID * mrsStaQueueAlloc(HI_U32 size)
{
    return mrsToolsMalloc(size);
}

//*****************************************************************************
// ��������: mrsStaTryDeQueue
// ��������: ���Գ����� 
// ����˵��:
//   MRS_QUE_MODULE *   queue   [IN] ����ģ��
//   HI_VOID (*f)(HI_VOID*)     [IN] �ͷź���
// �� �� ֵ: 
//    ������гɹ�������HI_TRUE�����򷵻�HI_FALSE
// ��    ��: niesongsong/nkf62735 [2012-05-29]
// ��    ע: �ýӿ��ڳ����У��û�Ӧ��ָ���ͷŵķ�����������ܵ��½�������
//           ���ý��û�еõ��ͷţ�������֪����ô���ĺ����������ָ�������
//           �����Ԫ���ڴ��Ӧ���ڴ��ͷź�����
//*****************************************************************************
HI_BOOL mrsStaTryDeQueue(MRS_QUE_MODULE *queue,HI_VOID (*free_node)(HI_VOID *))
{
    HI_BOOL ret = HI_FALSE;
    if (queue) 
    {
        MRS_STA_ITEM * item = HI_NULL;
        MRS_SRV_QUEUE *q = &queue->stMrQueue;
        queue->retries = 0;
        ret = HI_TRUE;

        item = mrsSrvDeQueue(q);
        if (free_node) 
        {
            free_node(item);
        }
        
        //�ͷſ���Ȩ
        MRS_UNLOCK(queue->lock);
        MRS_StopTimer(MRS_STA_TIMER_RM_QUEUE_LOCK);

        //������д�����
        mrsStaNotifyQueue();
        mrsDfxRefreshRmQueueTopStatus(q, queue->lock); // ˢ�¶�����ж���״̬
        mrsDfxRefreshRmQueueNum((HI_U8)mrsSrvQueueLength(q));    // ����������ͳ��
    }
    return ret;
}


//*****************************************************************************
// ��������: mrsStaQueueProc
// ��������: ���д�����
// ����˵��:
//   MRS_QUE_MODULE *   queue   [IN] ����ģ��
// �� �� ֵ: 
//    ��
// ��    ��: niesongsong/nkf62735 [2012-05-29]
// ��    ע: ���б������Ĳ����������ýӿڻ����Ƿ��������������������
//           ����������ʲô�����������û����������ᴦ��ͷ��㣨���·��ͣ�
//           ���Դ�������ɾ����
//*****************************************************************************
HI_VOID mrsStaQueueProc(MRS_QUE_MODULE * queue)
{
    // ���Լ����ɹ�
    if (MRS_TRY_LOCK(queue->lock)) 
    {
        HI_U32 ret = HI_ERR_SUCCESS;
        MRS_STA_ITEM  *item;
        MRS_SRV_QUEUE *q;
        HI_U32 TTL = MRS_RM_METER_OVERTIME_DEFAULT;
        HI_U8 * t_buf = HI_NULL;
        HI_U16 t_len = 0;
        HI_U32 ulBaudRate = 0;

        MRS_StartTimer(MRS_STA_TIMER_RM_QUEUE_LOCK, MRS_STA_TIME_RM_QUEUE_LOCK, HI_SYS_TIMER_ONESHOT);

        q = &queue->stMrQueue;
        do 
        {
            // ��ȡ�׽��
            item = mrsSrvQueueTop(q);
            if (item == HI_NULL) 
            {
                MRS_UNLOCK(queue->lock);  // �ͷſ���Ȩ
                MRS_StopTimer(MRS_STA_TIMER_RM_QUEUE_LOCK);
                break;
            }

            t_buf = item->buf;
            t_len = item->len;

            if(item->id == MRS_STA_ITEM_ID_CSG_CLTI_SEARCH_METER)
            {

            }
            else if (item->id == MRS_STA_ITEM_ID_SEARCH_METER_CLT_II)
            {
                mrsCltiiSend2Meter(t_buf, t_len);
                return;
            }
            else if (item->id == MRS_STA_ITEM_ID_CSG_CLTI_UPG_DEVICE)
            {
                mrsCsgCltIUpgFrameSend(item);
                return;
            }
#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
            else if(item->id == MRS_STA_ITEM_ID_SEARCH_METER_CLT_I)
            {
                mrsCltiFrameSendPrepare(item);
            }
            else if(item->id == ID_MRS_CMD_PARALLEL_DATA_PLC)
            {
                MRS_PARALLEL_STA_CTX * ctx = mrsStaGetParallelCtx();

                if(HI_TRUE != mrsIsOldFrame())
                {
                    mrsParallelSetStaCtx(item, ctx);
                }
                
                ret = mrsParallelQueueProcess(item, ctx);
                if(HI_ERR_SUCCESS == ret)
                {
                    // ����֡�������-������
                    queue->retries = MRS_QUEUE_MAX_TRY; 
                }
                else
                {
                    MRS_PARALLEL_GET_TX_BUF(ctx, t_buf);
                    MRS_PARALLEL_GET_TX_LEN(ctx, t_len);
                    
                    // �����������֡
                    queue->retries = 0;
                }
            }
#endif

            if (queue->retries >= MRS_QUEUE_MAX_TRY) 
            {
                HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_007, HI_DIAG_MT("queue timeout--out of queue"));

                if(!item->bcFlg)    // ֻ��¼�ǹ㲥֡����ʧ���¼�
                {
                #if defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG) && defined(MRS_LOG_STA_SWITCH)
                    if((item->id >= ID_MRS_CMD_DATA_TRANSMIT_AFN13_PLC)
                            && (item->id <= ID_MRS_CMD_PARALLEL_DATA_PLC))
                    {
                        mrsLogPlcOverTimeStat(item->buf, item->len, 0, HI_NULL);
                    }
                #endif
                }

                switch (item->id)
                {
                case MRS_STA_ITEM_ID_DUTY_CYCLE:
                    mrsStaDutyCycleTimeout();
                    break;

#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
                case MRS_STA_ITEM_ID_CLTII_EVENT:
                    mrsCltiiEventFrameTimeout();
                    break;

                case MRS_STA_ITEM_ID_CLTI_EVENT:
                    mrsCltiEventFrameTimeout();
                    break;

                case MRS_STA_ITEM_ID_SET_VER_CLT_I:
                    mrsCltiSmStartTx();
                    break;
#else
                case MRS_STA_ITEM_ID_CSG_CLTI_SEARCH_METER:
                    mrsCsgCltIFrameTimeout();
                    return;
#endif

                default:
#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
                    mrsCltiiTimingTimeOutProc(item);
#endif
                    break;
                }

                mrsStaTryDeQueue(queue,mrsStaQueueFree);
                break;
            }

            if ((MRS_STA_QUEUE_FROM_REMOTE == item->from)//��������ʽ�����ط� ccy
                || ((MRS_STA_ITEM_ID_CSG_TRANS_XR == item->id) && (MRS_CSG_PLC_CTRL_RESP_BIT_N == item->option))
                || (MRS_STA_ITEM_ID_CSG_CLTI_SEARCH_METER == item->id))
            {
                queue->retries = MRS_QUEUE_MAX_TRY-1;//ֻ����һ��
            }
            
            queue->retries++;   // ����������

            TTL = MRS_100MS_TO_MS(item->time_out);

            mrsStaUartTxStat(item->id, t_buf, t_len);

            if (mrsIsTestDI(t_buf, t_len))
            {
                ret = mrsStaSendTestMeter(item->bcFlg, t_buf, t_len);
                break;
            }

            // ���֧�������
            if (mrsStaGetSupportVM()) 
            {
                ret = mrsStaSend2Meter(&queue->stMetModule,t_buf, t_len);

#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
                if ((ID_MRS_CMD_BROADCAST_TRANSMIT_PLC == item->id) || (ID_MRS_CMD_TEST_TRANSMIT_PLC == item->id))
                {
                    // ����SKIP, ������
                    ret = HI_ERR_SKIP;
                }
                else
#endif
                {
                    mrsStaStartTtlTimer(item, TTL);
                }
                break;
            }

            // ���STA
            if (!mrsToolsIsIICollector()) 
            {
                HI_U8 * buf = HI_NULL;
                HI_U16 len = 0;
                HI_U8 pos_h = 0;    // ��ͷ���

                if (((item->id >= ID_MRS_CMD_DATA_TRANSMIT_AFN13_PLC) && (item->id <= ID_MRS_CMD_TEST_TRANSMIT_PLC))
                    || (item->id == MRS_STA_ITEM_ID_CSG_TRANS_XR))
                {
                    pos_h = 4;    // 4Ϊ4��0xFE
                }

                len = pos_h + t_len;
                buf = mrsToolsMalloc(len);
                if(!buf)
                {                
                    mrsStaTryDeQueue(queue,mrsStaQueueFree);
                    break;
                }

                (hi_void)memset_s(buf, len, 0xFE, pos_h);                   // ��ͷ���    
                (hi_void)memcpy_s(buf + pos_h, len - pos_h, t_buf, t_len);          // ��������    
                
                ulBaudRate = mrsGetCurBaudRate();
                TTL += MRS_GET_UART_SEND_TIME(ulBaudRate, t_len);

                ret = MRS_SendMrData(buf, len, HI_DMS_CHL_UART_PORT_APP);

                switch (item->id)
                {
#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
                case ID_MRS_CMD_BROADCAST_TRANSMIT_PLC:
                    // Ϊ�˹㲥֡�����أ��ѷ��ͳ�ȥ�Ĺ㲥֡��¼�ڻ������У��ظ�֡��������
                    mrsStaFrameBufInsert(item->id, item->seq, item->len, item->buf, 0);
                    mrsStaTryDeQueue(queue,mrsStaQueueFree);
                    ret = HI_ERR_SUCCESS;
                    break;

                case ID_MRS_CMD_TEST_TRANSMIT_PLC:
                    mrsStaTryDeQueue(queue,mrsStaQueueFree);
                    ret = HI_ERR_SUCCESS;
                    break;

                case ID_MRS_CMD_DATA_TRANSMIT_AFN13_PLC:
                    mrsStaStartTtlTimer(item, TTL);
                    if (MRS_STA_TRANSFER_TIMING_NORMAL == item->sub_id)
                    {
                        mrsStaFrameBufInsert(item->id, item->seq, item->len, item->buf, 0);
                        queue->retries = MRS_QUEUE_MAX_TRY;
                    }
                    break;
#endif

                case MRS_STA_ITEM_ID_CSG_TRANS_XR:
                    mrsStaStartTtlTimer(item, TTL);
                    if (MRS_CSG_PLC_CTRL_RESP_BIT_N == item->option)
                    {
                        // ����Ӧ�𣬼��ڹ��˱���
                        mrsStaFrameBufInsert(item->id, item->seq, item->len, item->buf, 0);
                        queue->retries = MRS_QUEUE_MAX_TRY;
                    }
                    break;

                default:
                    mrsStaStartTtlTimer(item, TTL);
                    break;
                }

                mrsToolsFree(buf);

                break;
            }
            else
            {
                if ((MRS_STA_ITEM_ID_CSG_TRANS_XR == item->id) && (MRS_CSG_PLC_CTRL_RESP_BIT_N == item->option))
                {
                    HI_U8 * buf = HI_NULL;
                    HI_U16 len = 0;
                    HI_U8 pos_h = 0;

                    len = pos_h + t_len;
                    buf = mrsToolsMalloc(len);
                    if(!buf)
                    {                
                        mrsStaTryDeQueue(queue, mrsStaQueueFree);
                        break;
                    }

                    (hi_void)memset_s(buf, len, 0xFE, pos_h);
                    (hi_void)memcpy_s(buf + pos_h, len - pos_h, t_buf, t_len);
                    ret = mrsBoardTimingSendPro(buf, len, &queue->retries);
                    mrsToolsFree(buf);
                    if (HI_ERR_SUCCESS != ret)
                    {
                        break;
                    }

                    // ����Ӧ�𣬼��ڹ��˱���
                    mrsStaFrameBufInsert(item->id, item->seq, item->len, item->buf, 0);
                    break;
                }

                ret = mrsSendData2Meter(t_buf, t_len, &queue->retries, TTL);
                if (HI_ERR_SUCCESS != ret)
                {
                    break;
                }

#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
                switch (item->id)
                {
                case ID_MRS_CMD_BROADCAST_TRANSMIT_PLC:
                    mrsStaFrameBufInsert(item->id, item->seq, item->len, item->buf, 0);
                    ret = HI_ERR_SUCCESS;
                    break;

                case ID_MRS_CMD_TEST_TRANSMIT_PLC:
                    mrsStaTryDeQueue(queue, mrsStaQueueFree);
                    ret = HI_ERR_SUCCESS;
                    break;

                case ID_MRS_CMD_DATA_TRANSMIT_AFN13_PLC:
                    if (MRS_STA_TRANSFER_TIMING_NORMAL == item->sub_id)
                    {
                        mrsStaFrameBufInsert(item->id, item->seq, item->len, item->buf, 0);
                    }
                    break;

                default:
                    break;
                }    
#endif
                break;
            }
        } while (0);
        
        if (ret != HI_ERR_SUCCESS)
        {
            mrsStaTryDeQueue(queue,mrsStaQueueFree);
        }
    }

    return ;
}

/* BEGIN: Modified by fengxiaomin/00209182, 2014/3/27   ���ⵥ��:DTS2014032906596 */
HI_VOID mrsStaUartTxStat(HI_U16 usId, HI_U8* pData, HI_U16 pDataLen)
{
#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
    if((usId < ID_MRS_CMD_DATA_TRANSMIT_AFN13_PLC) || (usId > ID_MRS_CMD_PARALLEL_DATA_PLC))
    {
        return;
    }

    if(usId == ID_MRS_CMD_PARALLEL_DATA_PLC)
    {
        mrsDfxPrUartTx();
    }
    else if(usId == ID_MRS_CMD_DATA_TRANSMIT_AFN13_PLC)
    {
        mrsDfxXrUartTx();
    }
    else if(usId == ID_MRS_CMD_DATA_TRANSMIT_AFN14_PLC)
    {
        mrsDfxLrMeterTx();
    }
#else
    HI_UNREF_PARAM(usId);

	mrsDfxXrUartTx();
#endif

    mrsDfxRmUartTx(pData, pDataLen, HI_FALSE);
}


HI_U32 mrsStaSendTestMeter(HI_U8 ucBcFlag, HI_PBYTE pucData, HI_U16 usDataSize)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    MRS_ONE_RAW_FRAME_STRU stFrame = {0};
    HI_DSID_APP_RM_RX_TX_INF_S *pstRmDfx = mrsDfxGetRmTxRxInf();
    MRS_645_FRAME_STRU st645Frame = {0};
    HI_U16 usBufferLen = 0;
    HI_U8 ucBuffer[MRS_PROTO645_DATAGRAM_LEN_MAX] = {0};
    HI_U16 usPos = 0;
    HI_U16 usFrameLen = 0;

    ret = mrsFind645Frame(pucData, (HI_S16)usDataSize, &usPos, &usFrameLen);
    if (ret != HI_ERR_SUCCESS)
    {
        return ret;
    }
    
    if (ucBcFlag)
    {
        HI_BOOL bFlag = HI_TRUE;
        MRS_STA_SRV_CTX_STRU *sta = mrsStaGetContext();
        HI_U8 ucMeterAddr[HI_METER_ADDR_LEN] = {0};
        HI_U16 usIndex = 0;

        (hi_void)memcpy_s(ucMeterAddr, HI_METER_ADDR_LEN, pucData + usPos + MRS_645_FRAME_METERADD_OFFSET, sizeof(ucMeterAddr));

        bFlag = mrsFindMeterList(&sta->stMeterList, ucMeterAddr, &usIndex);
        if(!bFlag)
        {
            mrsDfxRmPlcBcRx();
            pstRmDfx->ulUartTxNum--;
            pstRmDfx->ulTestUartTxNum--;
            return HI_ERR_FAILURE;
        }
    }
    
    st645Frame.stCtrl.ucDir = MRS_645_FRAME_CONTROL_DIR_RESPONSION_BIT;
    st645Frame.stCtrl.ucFn = MRS_645_FRAME_CONTROL_FUNC_BIT;
    st645Frame.stCtrl.ucFrameFlag = MRS_645_FRAME_CONTROL_AFER_FRAME_NON_BIT;
    st645Frame.stCtrl.ucSlaveFlag = MRS_645_FRAME_CONTROL_RESPONSION_NOMAL_BIT;
    
    pstRmDfx->ulTestUartRxNum++;
    pstRmDfx->ulUartRxNum++;
    pstRmDfx->ulPlcTxNum++;
    pstRmDfx->ulTestPlcTxNum++;
    (hi_void)memcpy_s(st645Frame.ucAddr, sizeof(st645Frame.ucAddr), pucData + usPos + MRS_645_FRAME_METERADD_OFFSET, sizeof(st645Frame.ucAddr));
    st645Frame.ucVer = MRS_PROTO645_VER_2007;
    (hi_void)memcpy_s(st645Frame.ucDataRealm, sizeof(st645Frame.ucDataRealm), pucData + usPos + MRS_645_FRAME_DATA_OFFSET, MRS_645_FRAME_DATA_DI_SIZE);
    st645Frame.ucDataRealmLen += MRS_645_FRAME_DATA_DI_SIZE;
    (hi_void)memcpy_s(st645Frame.ucDataRealm + st645Frame.ucDataRealmLen, sizeof(st645Frame.ucDataRealm) - st645Frame.ucDataRealmLen,
            pucData + usPos + MRS_645_FRAME_DATA_OFFSET + st645Frame.ucDataRealmLen, MRS_645_FRAME_COMMAND_ID_SIZE);
    st645Frame.ucDataRealmLen += MRS_645_FRAME_COMMAND_ID_SIZE;
    (hi_void)memcpy_s(st645Frame.ucDataRealm + st645Frame.ucDataRealmLen, MRS_645_FRAME_STAT_ID_SIZE,
            pucData + usPos + MRS_645_FRAME_DATA_OFFSET + st645Frame.ucDataRealmLen, MRS_645_FRAME_STAT_ID_SIZE);
    st645Frame.ucDataRealmLen += MRS_645_FRAME_STAT_ID_SIZE;
    (hi_void)memcpy_s(st645Frame.ucDataRealm + st645Frame.ucDataRealmLen, sizeof(HI_DSID_APP_RM_RX_TX_INF_S),
        pstRmDfx, sizeof(HI_DSID_APP_RM_RX_TX_INF_S));
    mrs645DataEncode(st645Frame.ucDataRealm + st645Frame.ucDataRealmLen, sizeof(HI_DSID_APP_RM_RX_TX_INF_S));
    st645Frame.ucDataRealmLen += sizeof(HI_DSID_APP_RM_RX_TX_INF_S);
    pstRmDfx->ulTestUartRxNum--;
    pstRmDfx->ulUartRxNum--;
    pstRmDfx->ulPlcTxNum--;
    pstRmDfx->ulTestPlcTxNum--;

    ret = MRS_Proto645Enc(ucBuffer, &usBufferLen, &st645Frame);
    if (ret != HI_ERR_SUCCESS)
    {
        return ret;
    }
    
    // ��ȡ��������֡����
    ret = mrs645ProtoStreamInput(ucBuffer, usBufferLen, &stFrame, ID_MRS_UART_645BUF);
    if(HI_ERR_SUCCESS == ret)
    {
        HI_SYS_QUEUE_MSG_S stMsg = {0}; 
        MRS_ONE_RAW_FRAME_STRU* pstFrame = (MRS_ONE_RAW_FRAME_STRU*)&stMsg.ulParam[0];
        
        (hi_void)memcpy_s(pstFrame, sizeof(MRS_ONE_RAW_FRAME_STRU), &stFrame, sizeof(MRS_ONE_RAW_FRAME_STRU));
        stMsg.ulMsgId = EN_MRS_FW_MSG_645_FRAME_INPUT;

        // ������Ϣ�� MRS����, ������Ϣ�ַ����� 
        ret = mrsSendMessage2Queue(&stMsg);
        if(ret != HI_ERR_SUCCESS)
        {
            mrsToolsFree(stFrame.pucDatagram);
        }
    }

    return ret;
}
/* END:   Modified by fengxiaomin/00209182, 2014/3/27 */

//*****************************************************************************
// ��������: mrsStaQueueUnLock
// ��������: ǿ�ƽ���
// ����˵��:
//   MRS_QUE_MODULE *   queue   [IN] ����ģ��
// �� �� ֵ: 
//    ��
// ��    ��: niesongsong/nkf62735 [2012-05-29]
// ��    ע: ���Ը����м���
//*****************************************************************************
HI_BOOL mrsStaQueueUnLock(MRS_QUE_MODULE * q)
{
    HI_BOOL r = HI_FALSE;

    if (!q) 
    {
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_013, HI_DIAG_MT("param null"));
        return r;
    }

    r = MRS_UNLOCK(q->lock);
    MRS_StopTimer(MRS_STA_TIMER_RM_QUEUE_LOCK);
    
    return r;
}

//*****************************************************************************
// ��������: mrsStaQueueReplace
// ��������: �滻��ͬ�ĻỰֵ��
// ����˵��:
//   MRS_QUE_MODULE *   queue   [IN] ����ģ��
// �� �� ֵ: 
//    ��
// ��    ��: niesongsong/nkf62735 [2012-05-29]
// ��    ע: ���Ը����м���
//*****************************************************************************
HI_BOOL mrsStaQueueReplace(HI_VOID * node,HI_VOID * param)
{
    struct que_param * p = (struct que_param *)param;
    MRS_STA_ITEM * item = (MRS_STA_ITEM *)node;

    //��������
    if (!p || !node) 
    {
        return HI_TRUE;
    }
    
    p->result = HI_FALSE;
    if ((p->item->id == item->id) && (p->item->seq == item->seq))
    {
        if ((node == mrsSrvQueueTop(&p->module->stMrQueue)) && (MRS_LOCK_STATUS(p->module->lock)))
        {
            p->result = HI_FALSE;
        }
        else 
        {
            mrsSrvQueueReplace(&(p->module->stMrQueue),(MRS_SRV_NODE *)item, (MRS_SRV_NODE *)p->item); 
            mrsStaQueueFree(item);
            p->result = HI_TRUE;

            return HI_TRUE;
        }
    }

    return HI_FALSE;
}


HI_VOID mrsStaUpdateQueueStatus(HI_VOID)
{
    MRS_STA_SRV_CTX_STRU * pstStaCtx = mrsStaGetContext();
    MRS_STA_ITEM *pstItem = mrsSrvQueueTop(&pstStaCtx->stQueModule.stMrQueue);

    MRS_StopTimer(MRS_STA_TIMER_RM_FRAME_INTERVAL);

    if (pstItem && pstItem->bAutoStrategy)
    {
        if (pstItem->bTtlTimerFlg)
        {
            pstItem->bRcvByteFlg = HI_TRUE;
        }
        else if (pstItem->bFrameTimerFlg)
        {
            MRS_StartTimer(MRS_STA_TIMER_RM_FRAME_INTERVAL, MRS_100MS_TO_MS(pstItem->ucFrameTimeout), HI_SYS_TIMER_ONESHOT);
        }
    }    
}


HI_VOID mrsStaStartTtlTimer(MRS_STA_ITEM *pstItem, HI_U32 ulTTL)
{
    MRS_StopTimer(MRS_STA_TIMER_RM_FRAME_INTERVAL);
    MRS_StartTimer(MRS_STA_TIMER_TTL, ulTTL, HI_SYS_TIMER_ONESHOT);

    if (pstItem && pstItem->bAutoStrategy)
    {
        pstItem->bTtlTimerFlg = HI_TRUE;
        pstItem->bRcvByteFlg = HI_FALSE;
        pstItem->bFrameTimerFlg = HI_FALSE;
    }
}


HI_VOID mrsStaStopTtlTimer(MRS_STA_ITEM *pstItem)
{
    MRS_StopTimer(MRS_STA_TIMER_TTL);
    MRS_StopTimer(MRS_STA_TIMER_RM_FRAME_INTERVAL);

    if (pstItem)
    {
        pstItem->bTtlTimerFlg = HI_FALSE;
        pstItem->bRcvByteFlg = HI_FALSE;
        pstItem->bFrameTimerFlg = HI_FALSE;
    }
}


#endif // STA

