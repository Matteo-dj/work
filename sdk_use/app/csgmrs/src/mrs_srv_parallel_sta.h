 //*****************************************************************************
//
//                  ��Ȩ���� (C), 1998-2011, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mrs_srv_parallel_sta.h
//  �� �� ��   : V1.0 
//  ��    ��   : liuxipeng/KF54842
//  ��������   : 2012-12-22
//  ��������   : ��������ģ�麯�����ṹ����
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

#ifndef __MRS_SRV_PARALLEL_STA_H__
#define __MRS_SRV_PARALLEL_STA_H__

HI_START_HEADER


#define MRS_PARALLEL_GAP_TIMER_MAX 2000 // ֡�����ʱ�����ֵ
#define MRS_PARALLEL_GAP_TIMER_MIN 10    // ֡�����ʱ����Сֵ

#define MRS_PARALLEL_645_FRAME_MAX 32   // STA�˲���֧�ֵ����645֡����

// 645֡�����Ľṹ(���ڱ����շ�֡��ָ��;����;Ӧ���־)
typedef struct
{
    HI_PBYTE tx_buf;    // ���ͻ�����ָ��(ָ����������ݻ������ľ���֡ͷ-����malloc����free)
    HI_PBYTE rx_buf;    // ���ջ�����ָ��(�յ����Ӧ��֡��malloc;�ϱ�cco��free)
    HI_S16 tx_len;      // ���������ݵĳ���
    HI_S16 rx_len;      // ���ջ������ĳ���
}MRS_645_FRAME_CTX;

// STA�˲�������������
typedef struct
{
    HI_U32 resp_flg;        // Ӧ���־(�յ����Ӧ����ô˱�־--0~31bit�ֱ����MRS_645_FRAME_CTX�е�Ԫ��)
    HI_U32 nak_flg;         // ����֡��־(�յ�����֡���ô˱�־--0~31bit�ֱ����MRS_645_FRAME_CTX�е�Ԫ��)
    HI_U16 gap_flg : 1;     // ֡�����ʱ��ʹ�ñ�־(TRUE ʹ��)
    HI_U16 gap_timer : 15;  // ֡�����ʱ�� 
    HI_U8 noresp_try_flag : 1;  // δӦ�����Ա�־-��CCO����-STA�ɴ��ж�δӦ����Ƿ���Ҫ����
    HI_U8 nak_try_flag : 1;     // �������Ա�־-��CCO����-STA�ɴ��ж����յ�����֡���Ƿ���Ҫ����
    HI_U8 try_max : 2;      // ������Դ���(��CCO�����)
    HI_U8 protocol;         // ����֡�Ĺ�Լ����
    HI_U8 index;            // ��ǰ�����645֡���(��ʼΪ0;���ֵС��frame_num)
    HI_S8 trun_num;         // �����ִ���(��ʼ������������ʱ��Ϊ0)
    HI_U8 ok_num;           // �ɹ�������֡����(ֻ������ͳ��)
    HI_U8 frame_num;        // 645֡����
    MRS_645_FRAME_CTX frame_ctx[MRS_PARALLEL_645_FRAME_MAX];    // 645֡������-֧��MRS_PARALLEL_645_FRAME_MAX��645֡
}MRS_PARALLEL_STA_CTX;

// ��ȡ��ǰ����֡�Ļ�����ָ��
#define MRS_PARALLEL_GET_TX_BUF(ctx, buf)            \
        do                                           \
        {                                            \
            buf = ctx->frame_ctx[ctx->index].tx_buf; \
        }while(0);

// ��ȡ��ǰ����֡�Ļ���������
#define MRS_PARALLEL_GET_TX_LEN(ctx, len)                    \
        do                                                   \
        {                                                    \
            len = (HI_U16)ctx->frame_ctx[ctx->index].tx_len; \
        }while(0);

// ����STA�˲�������������
HI_PUBLIC HI_VOID mrsParallelSetStaCtx(MRS_STA_ITEM * queue_item, MRS_PARALLEL_STA_CTX * ctx);

// ����CCO����ת������
HI_PUBLIC HI_U32 mrsParallelCmdTransmit(HI_IN HI_U16 usId, HI_PVOID pReqPacket, HI_U16 usReqPacketSize);

// �������645֡���ͽ��մ�����
HI_PUBLIC HI_U32 mrsParallelMutliFrameProcess(MRS_STA_ITEM * queue_item, HI_U8 * data, HI_U16 len, HI_U8 protocol);

// ����������д���
HI_PUBLIC HI_U32 mrsParallelQueueProcess(MRS_STA_ITEM * queue_item, MRS_PARALLEL_STA_CTX * ctx);

// ˢ�²�������֡��Ӧ��־
HI_PUBLIC HI_VOID mrsParallelRefreshRespFlg(HI_BOOL flg);

// ˢ�²�������֡״̬
HI_PUBLIC HI_VOID mrsParallelRefreshFrameStatus(HI_BOOL flg);


HI_END_HEADER

#endif //__MRS_SRV_PARALLEL_H__






