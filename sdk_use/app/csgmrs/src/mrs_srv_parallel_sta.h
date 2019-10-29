 //*****************************************************************************
//
//                  版权所有 (C), 1998-2011, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_srv_parallel_sta.h
//  版 本 号   : V1.0 
//  作    者   : liuxipeng/KF54842
//  生成日期   : 2012-12-22
//  功能描述   : 并发读表模块函数及结构声明
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

#ifndef __MRS_SRV_PARALLEL_STA_H__
#define __MRS_SRV_PARALLEL_STA_H__

HI_START_HEADER


#define MRS_PARALLEL_GAP_TIMER_MAX 2000 // 帧间隔定时器最大值
#define MRS_PARALLEL_GAP_TIMER_MIN 10    // 帧间隔定时器最小值

#define MRS_PARALLEL_645_FRAME_MAX 32   // STA端并发支持的最大645帧个数

// 645帧上下文结构(用于保存收发帧的指针;长度;应答标志)
typedef struct
{
    HI_PBYTE tx_buf;    // 发送缓冲区指针(指向队列中数据缓冲区的具体帧头-无需malloc无需free)
    HI_PBYTE rx_buf;    // 接收缓冲区指针(收到电表应答帧后malloc;上报cco后free)
    HI_S16 tx_len;      // 待发送数据的长度
    HI_S16 rx_len;      // 接收缓冲区的长度
}MRS_645_FRAME_CTX;

// STA端并发读表上下文
typedef struct
{
    HI_U32 resp_flg;        // 应答标志(收到电表应答后置此标志--0~31bit分别代表MRS_645_FRAME_CTX中的元素)
    HI_U32 nak_flg;         // 否认帧标志(收到否认帧后置此标志--0~31bit分别代表MRS_645_FRAME_CTX中的元素)
    HI_U16 gap_flg : 1;     // 帧间隔定时器使用标志(TRUE 使用)
    HI_U16 gap_timer : 15;  // 帧间隔定时器 
    HI_U8 noresp_try_flag : 1;  // 未应答重试标志-由CCO给定-STA由此判定未应答后是否需要重试
    HI_U8 nak_try_flag : 1;     // 否认重试标志-由CCO给定-STA由此判定接收到否认帧后是否需要重试
    HI_U8 try_max : 2;      // 最大重试次数(由CCO侧给定)
    HI_U8 protocol;         // 接收帧的规约类型
    HI_U8 index;            // 当前处理的645帧序号(起始为0;最大值小于frame_num)
    HI_S8 trun_num;         // 抄读轮次数(初始化设置上下文时置为0)
    HI_U8 ok_num;           // 成功读到的帧个数(只在首轮统计)
    HI_U8 frame_num;        // 645帧个数
    MRS_645_FRAME_CTX frame_ctx[MRS_PARALLEL_645_FRAME_MAX];    // 645帧上下文-支持MRS_PARALLEL_645_FRAME_MAX个645帧
}MRS_PARALLEL_STA_CTX;

// 获取当前处理帧的缓冲区指针
#define MRS_PARALLEL_GET_TX_BUF(ctx, buf)            \
        do                                           \
        {                                            \
            buf = ctx->frame_ctx[ctx->index].tx_buf; \
        }while(0);

// 获取当前处理帧的缓冲区长度
#define MRS_PARALLEL_GET_TX_LEN(ctx, len)                    \
        do                                                   \
        {                                                    \
            len = (HI_U16)ctx->frame_ctx[ctx->index].tx_len; \
        }while(0);

// 设置STA端并发读表上下文
HI_PUBLIC HI_VOID mrsParallelSetStaCtx(MRS_STA_ITEM * queue_item, MRS_PARALLEL_STA_CTX * ctx);

// 处理CCO数据转发命令
HI_PUBLIC HI_U32 mrsParallelCmdTransmit(HI_IN HI_U16 usId, HI_PVOID pReqPacket, HI_U16 usReqPacketSize);

// 多个串行645帧发送接收处理函数
HI_PUBLIC HI_U32 mrsParallelMutliFrameProcess(MRS_STA_ITEM * queue_item, HI_U8 * data, HI_U16 len, HI_U8 protocol);

// 并发读表队列处理
HI_PUBLIC HI_U32 mrsParallelQueueProcess(MRS_STA_ITEM * queue_item, MRS_PARALLEL_STA_CTX * ctx);

// 刷新并发读表帧响应标志
HI_PUBLIC HI_VOID mrsParallelRefreshRespFlg(HI_BOOL flg);

// 刷新并发读表帧状态
HI_PUBLIC HI_VOID mrsParallelRefreshFrameStatus(HI_BOOL flg);


HI_END_HEADER

#endif //__MRS_SRV_PARALLEL_H__






