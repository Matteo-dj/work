//*****************************************************************************
//
//                  版权所有 (C), 2001-2011, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_srv_sta_queue.h
//  版 本 号   : V1.0 
//  作    者   : 聂松松/KF62735
//  生成日期   : 2012-07-11
//  功能描述   : 升级
//  函数列表   : NA
//  修改历史   : 
//  1.日    期 : 2012-07-11
//    作    者 : 聂松松/KF62735
//    修改内容 : 创建文件 
//*****************************************************************************
#ifndef __MRS_SRV_STA_QUEUE_H__
#define __MRS_SRV_STA_QUEUE_H__

//依赖于通用队列和会话模块
#include "mrs_srv_queue.h"
#include "mrs_srv_meter.h"
#include "mrs_fw_n.h"

HI_CPP_START

//通知队列
#define mrsStaNotifyQueue()                                             \
    do {                                                                \
            HI_SYS_QUEUE_MSG_S stMsg = { 0 };                           \
            stMsg.ulParam[0] = MRS_STA_TIMER_QUEUE;                     \
            stMsg.ulMsgId = ID_MRS_MSG_MR_TIMER;                        \
            mrsSendMessage2Queue(&stMsg);                               \
    }while(0)

#define MRS_QUEUE_MAX_TRY 2     // 读表队列最大重试次数

//4.1 STA抄表队列
typedef struct 
{
    MRS_SRV_QUEUE stMrQueue;  //队列
    HI_U8 retries;            //重试次数
    HI_U8 lock;               //锁  
    HI_U8 padding[2];
    MRS_MET_MODULE stMetModule;
}MRS_QUE_MODULE;

//抄表队列元素
typedef struct
{
    MRS_SRV_NODE link;  //必须放在第一位
    HI_U8   from;       //来源，本地，抄控器，HSO
    HI_U8   bcFlg;      //广播地址标志(源地址为广播地址-使用广播地址响应)
    HI_U8   time_out;   //STA端超时时间 -单位:百毫秒
    HI_U8   option;     // I采搜表相关帧类型: 1开始搜表 2停止搜表 3查询搜表结果前16个 4查询搜表结果后16个 5上电尝试上次入网地址
    HI_U16  id;         // 业务类型(plc id)
    HI_U16  len;        // 缓冲区长度
    HI_U8 * buf;        // 发送数据缓冲区
    HI_U8   ucbaudrate_index;      // 波特率索引 00-默认波特率   
   
    HI_U8   noresp_try_flag : 1;  // 未应答重试标志-由CCO给定-STA由此判定未应答后是否需要重试
    HI_U8   nak_try_flag    : 1;  // 否认重试标志-由CCO给定-STA由此判定接收到否认帧后是否需要重试
    HI_U8   try_max  : 2;   // 最大重试次数
    HI_U8   sub_id : 4;     //业务id下的分类,1-透抄id下的校时命令
    HI_U16  gap_timer;  // 帧间间隔时间
    HI_U16  seq;        // 记录下行帧的序号 组织上行帧时取此序号

    HI_U8   bAutoStrategy  : 1;     // Auto detect strategy switch, 0 - total frame timeout strategy (old strategy), 1 - frame head & interval timeout strategy.
    HI_U8   bTtlTimerFlg   : 1;     // TTL timer started flag, when TTL timer alarm, if rx buf isn't empty, start frame interval timer.
    HI_U8   bFrameTimerFlg : 1;     // Frame interval timer started flag
    HI_U8   bRcvByteFlg    : 1;     // Rx data flag, whether received data after TTL timer.
    HI_U8   ucFrameTimeout : 4;     // Frame interval, unit 100ms.
    HI_U8   ucProtocol;
	HI_U8  	dev_mac[HI_PLC_MAC_ADDR_LEN]; //抄表设备的mac
	HI_U8  	pad[2];
} MRS_STA_ITEM;

//STA发送队列的一些接口
HI_U32 mrsStaQueueInit(MRS_QUE_MODULE *);   //初始化队列
HI_U32 mrsStaQueueReset(MRS_QUE_MODULE *);  //重置队列
HI_BOOL mrsStaTryEnQueue(MRS_QUE_MODULE *,MRS_STA_ITEM *);//入队列
HI_BOOL mrsStaTryDeQueue(MRS_QUE_MODULE *,HI_VOID (*)(HI_VOID *));//尝试出队列
HI_VOID mrsStaQueueProc(MRS_QUE_MODULE *);  //队列处理函数 
HI_VOID mrsStaQueueFree(HI_VOID *);         //释放结点
HI_VOID * mrsStaQueueAlloc(HI_U32 size);    //申请内存
HI_BOOL mrsStaQueueUnLock(MRS_QUE_MODULE *);
HI_U32 mrsStaSendTestMeter(HI_U8 ucBcFlag, HI_PBYTE pucData, HI_U16 usDataSize);
HI_VOID mrsStaUartTxStat(HI_U16 usId, HI_U8* pData, HI_U16 pDataLen);

// Update STA queue status
HI_VOID mrsStaUpdateQueueStatus(HI_VOID);

// Start TTL timer
HI_VOID mrsStaStartTtlTimer(MRS_STA_ITEM *pstItem, HI_U32 ulTTL);

// Stop TTL timer
HI_VOID mrsStaStopTtlTimer(MRS_STA_ITEM *pstItem);

HI_VOID mrsStaQueueBlackListProc(HI_U8 *pucMeterAddr);

HI_CPP_END
#endif
