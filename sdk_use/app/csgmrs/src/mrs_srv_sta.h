//*****************************************************************************
//
//                  版权所有 (C), 1998-2011, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_srv_sta.h
//  版 本 号   : V1.0
//  作    者   : shenhankun/00130424
//  生成日期   : 2011-11-08
//  功能描述   : TODO: ...
//
//  函数列表   : TODO: ...
//  修改历史   :
//  1.日    期 : 2011-11-08
//    作    者 : shenhankun/00130424
//    修改内容 : 创建文件
//
//*****************************************************************************
#ifndef __MRS_SERVICE_STA_H__
#define __MRS_SERVICE_STA_H__

#include "mrs_srv_sta_queue.h"
#include "mrs_srv_parallel_sta.h"

HI_START_HEADER
//STA端是主动上报还是响应上报
//*****************************************************************************
//STA_PRM_STA_CCO: STA主动上报，STA为启动方
//STA_PRM_CCO_STA: STA为被动响应上报，STA为从动方
//*****************************************************************************
#define STA_PRM_STA_CCO     1
#define STA_PRM_CCO_STA     0

#define  METER_CNF_ADDR     0x13

/* BEGIN: Added by fengxiaomin/00209182, 2014/2/19   问题单号:DTS2014012301705 */
#define STA_MAC_ADDRESS_END_BYTE  0xFE
/* END:   Added by fengxiaomin/00209182, 2014/2/19 */
#define MRS_CSG_PHYSICAL_TEST_MODE_LISTEN    (10)

//*****************************************************************************
//METER_CNF_ADDR   :  电表地址响应功能码
//*****************************************************************************
#define  MRS_STA_REPORT_METER_ADDR_RESPOND   0  //回应cco端下发的读表地址帧标志
#define  MRS_STA_REPORT_METER_ADDR_ACTIVE    1  //主动上报表地址和mac地址标志
#define  MRS_STA_REPORT_METER_ADDR_CHANGE    2  //当STA端表地址改变时主动上报表地址和mac地址标志
#define  MRS_STA_REPORT_METER_INFO_VALID     1  //STA端主动上报表地址生效
#define  MRS_STA_REPORT_METER_INFO_INVALID   0  //STA端主动上报表地址失效

#define  MRS_STA_LOCAL_MAP_ENTRY_TTL_MAX     2048   // STA本地映射电表列表每项TTL最大值

#define MRS_STA_RM_CFG_LOCAL    (0)     /* Local, STA/CltII <--> Meter, CltI Module <--> CltI  CNcomment: 本地: STA/II采与电表之间或I采模块与I采之间。CNend */
#define MRS_STA_RM_CFG_CLTI     (1)     /* CltI transfer, CltI Module <--> CltI <--> Meter  CNcomment: I采转发: I采模块通过I采转发抄读电表的默认参数。CNend */
#define MRS_STA_RM_CFG_AUTO     (0xFF)  /* Auto mode, choose 'Local' or 'CltI transfer' mode by product type  CNcomment: 自动，根据产品形态自动选择。CNend */

#define MRS_CHECK_LEAVE_TIME_LEN_TH_VALID(leave_time_len) ((leave_time_len) > 0)

//表地址列表项
/*********************************
  7   6   5   4   3   2   1   0
|<--有效标志--> |<--规约类型-->|
*********************************/
typedef struct
{
    HI_U8  ucValidFlag:4;                   // 有效标志
    HI_U8  ucProtocol:4;                    // 规约类型
    HI_U8  padding;
    HI_U16 usOption;                        // TTL/DI
    HI_U8  ucMeterAddr[HI_METER_ADDR_LEN];  // 表地址
    HI_U16 usBaudRate;                      // 电表通信速率
}MRS_METER_ITEM_S;

//表地址列表
typedef struct
{
    HI_U8 ucMeterNum;               // 电表个数
    HI_U8 ucType;                   // 模块类型 (MRS_STA_TYPE_METER 电表类型; MRS_STA_TYPE_COLLECTOR 采集器类型)
    HI_U8 padding[2];
    MRS_METER_ITEM_S stNodeMeter;   // 结点表地址（也有可能没有）
    MRS_METER_ITEM_S stMeterItem[PRODUCT_CFG_MRS_MAX_METER_IN_MAC_NUM];
}MRS_METER_LIST_S;

#if defined(PRODUCT_CFG_SYS_RST_SOLUTION)
typedef struct
{
    HI_U32 ulRxUartTimes;   //STA接收到电表报文次数
    HI_U32 ulRxRMPlcTimes;     //STA/II采接收抄表PLC帧次数
}MRS_STA_RESET_PARAM_S;//STA主动复位相关参数信息
#endif


// 此处根据缓冲区中的报文做特殊处理:
// 13/14和事件查询报文: 在缓冲区中缓存表回复的报文
// 广播报文: 缓存CCO下发的报文
// 比较时根据id和seq比较，如果有相同的报文，则直接返回抄读结果(13/14和事件查询)或不处理(广播报文)

typedef struct
{
    HI_U32 id;          // PLC通道ID
    HI_U16 seq;         // 报文序号
    HI_U16 frame_len;   // 报文长度
    HI_U8 valid;        // 有效标志 0:无效 1:有效
    HI_U8 protocol;     // 规约类型
    HI_U16 padding;
    HI_U32 timestamp;   // 时间戳
    HI_U8 frame_buf[MRS_STA_FRAME_BUF_ITEM_BUFSIZE];
} MRS_STA_FRAME_BUF_ITEM_STRU;

typedef struct
{
    HI_U32 max_count;                   // 存储报文的最大数量
    HI_U32 current;                     // 当前索引位置
    HI_U32 timeout;                     // 报文有效期
    MRS_STA_FRAME_BUF_ITEM_STRU * item; // 存储报文的结构
} MRS_STA_FRAME_BUF_CTX;

typedef struct
{
    HI_BOOL bValid;
    HI_U8 aucAddr[HI_METER_ADDR_LEN];
    HI_U8 ucProtocol;
    HI_U16 us97DI;
    HI_U16 usId;
}MRS_STA_RM_METER_INFO;

typedef NV_MRS_STA_RM_CFG_STRU MRS_STA_RM_CFG_STRU;

//STA运行的上下文
typedef struct
{
    HI_U8   ucState;            // STA运行状态

    HI_U8   macState:1;         // mac的状态
    HI_U8   plcState:1;         // plc的状态
    HI_U8   acdstate:1;         // ACD状态
    HI_U8   IsOldFrame:1;       // 队列元素新旧标志
    HI_U8   staRepFlag:1;       // STA端主动上报表地址来维护映射表开关，默认为关闭
    HI_U8   meterChangeFlag:1;  // STA端表地址改变后,上报CCO开关  0:不启用  1:启用    默认为??????

    HI_U8   sync_mac_flg : 1;   // 同步mac地址标志(上电后只同步一次):标志为FALSE时允许同步,同步完成后置为TRUE
                                // 上电获取电表地址超时后将标志置为TRUE,后续不进行mac地址同步
    HI_U8   ucDutFlag : 1;
    HI_U8   reserved;

    HI_U8   ucSeq;

/* BEGIN: Added by fengxiaomin/00209182, 2015/10/23   问题单号号:DTS2015102710230 */
    HI_U8   aucMeter[HI_METER_ADDR_LEN];  // 电表地址
    HI_U8   ucProtocol;                   // 规约类型
/* END:   Added by fengxiaomin/00209182, 2015/10/23 */

    HI_U8   plc_rxtx_mode;      // PLC通道收发模式 00:等待组网路由优化完成后才进行PLC通道的收发; 01:强制PLC通道收发(不等待PLC状态);

    HI_U32  addr_Fail_wait_time;// 获取电表地址失败后等待时间 单位:S
    HI_U8   addr_try_num;       // 获取电表地址尝试次数
    HI_U8   sta_type;           // STA 类型: 0 非法，1 电表，2 I型采集器

    HI_U8   ucMacAddr[HI_PLC_MAC_ADDR_LEN];

    HI_U16  detect_timeout;                  // STA上电流程: 探测波特率超时时间 单位:毫秒
    HI_U16  detect_period;                   // STA上电流程: 一个探测循环结束后再次开始循环的等待时间 单位:秒
    HI_U8   detect_loop_max;                 // STA上电流程: 探测波特率轮次
    HI_U8   read_positive_A_times;           // I采上电流程: 抄读NV中电表正向有功电能次数
    HI_U8   read_positive_A_interval;        // I采上电流程: 查询I型采集器第1块搜表结果时间间隔 单位:秒
    HI_U8   query_sm_interval;               // I采搜表: 过程中查询搜表结果时间间隔 单位:秒
    HI_BOOL is_real_clti;                    // 是否为真正的I采: 无搜表功能和有搜表功能都是真正的I采；探测波特率失败不是真正I采

    HI_U8   ucSoftTfMode;       // 无扰台区识别功能总开关
    HI_U16  usBindDuration;     // 绑定CCO Neteork的持续时间，单位：分钟，最大45.5天；取值0xFFFF时，表示永久锁定，实际锁定136年（U32表示的最大秒数）

    HI_U16  usLeaveTimeLenTh;   // 离网时长门限，单位min
    HI_U8   ucStaRstPinEnable;  // SAT端RST管脚使能
    HI_U8   pad;

    HI_U32  ulRstShielTime;      // 屏蔽RST管脚时长，单位s

#if defined(PRODUCT_CFG_SYS_RST_SOLUTION)
    MRS_STA_RESET_PARAM_S stStaResetInfo;  //STA端复位相关信息
#endif

    MRS_METER_LIST_S  stMeterList;      // 电表地址
    MRS_QUE_MODULE    stQueModule;      // 抄表队列模块

    MRS_PARALLEL_STA_CTX stParallelCtx; // 并发读表上下文

    MRS_STA_FRAME_BUF_CTX stFrameBufCtx;// 帧缓冲池上下文(过滤机制用)
    MRS_STA_RM_METER_INFO stRmMeterInfo;

    MRS_STA_RM_CFG_STRU stRmCfg;    /* Parameters of queue and meter-reading. CNcomment: 队列和抄表参数. CNend */

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
    HI_U32 (*pfnPlcAckProc)(MRS_CSG_PLC_FRAME_HEAD*);
    HI_U32 (*pfnPlcDenyProc)(MRS_CSG_PLC_FRAME_HEAD*);

    HI_U16 usPlcSeq;
    HI_U16 reserved3;
#endif
} MRS_STA_SRV_CTX_STRU;

//获取设置上下文
HI_PUBLIC MRS_STA_SRV_CTX_STRU * mrsStaGetContext(HI_VOID);
HI_PUBLIC MRS_STA_SRV_CTX_STRU * mrsStaSaveContext(MRS_STA_SRV_CTX_STRU *x);
// 获取STA端并发读表全局上下文
HI_PUBLIC MRS_PARALLEL_STA_CTX * mrsStaGetParallelCtx(HI_VOID);
//获取MAC地址

HI_PUBLIC HI_U8 * mrsStaGetMacAddr(HI_U8 macAddr[6]);



// 设置电表地址到Mac层
HI_U32 mrsStaUpdateTopAddrInf(HI_IN HI_U8 ucAddr[HI_METER_ADDR_LEN], HI_IN HI_U8 ucProtocol);
// 将电表地址转换后设置为以太网地址
HI_U32 mrsSetEthIp(HI_U8 maddr[6]);
// 设置STA的模块Mac地址
HI_U8 * mrsStaSetMacAddr(HI_U8 macAddr[6]);
/* BEGIN: PN: DTS2015051601564 MODIFY\ADD\DEL by cuiate/00233580 at 2015/5/15 */
HI_U32 mrsStaUpdateAddr(HI_U8 ucMeterAddr[HI_METER_ADDR_LEN], HI_U8 ucProtocol);
HI_U32 mrsStaUpdateAddrProc(HI_U8 ucMeterAddr[HI_METER_ADDR_LEN], HI_U8 ucProtocol, HI_U16 usDi97);
/* END:   PN: DTS2015051601564 MODIFY\ADD\DEL by cuiate/00233580 at 2015/5/15 */

/**
 @brief     Add meter to local meterlist.  CNcomment: 添加电表到本地列表。CNend
 @param[in] pstItem  meter information pointer.  CNcomment: 电表信息指针。CNend
 */
HI_VOID mrsStaAddMeter2LocalList(MRS_METER_ITEM_S *pstItem);

// 查找表地址列表-找到相同的地址返回HI_TRUE并输出对应的数组下标
HI_BOOL mrsFindMeterList(MRS_METER_LIST_S * pMeterList,
                            HI_U8 addr[HI_METER_ADDR_LEN], HI_U16 * index);

// 获取CCO帧新旧标志
HI_PUBLIC HI_BOOL mrsIsOldFrame(HI_VOID);

// 设置CCO帧新旧标志
HI_PUBLIC HI_VOID mrsSetFrameFlg(HI_BOOL flg);

//*****************************************************************************
// 函数名称: mrsGetRepFlagNv
// 功能描述: 获取主动上报表地址开关---NV项
//
// 参数说明:
//   HI_VOID
//
// 返 回 值:
//    HI_VOID
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: fengxiaomin/00209182 [2013-02-21]
//*****************************************************************************
HI_VOID mrsGetRepFlagNv(HI_VOID);


// 返 回 值: STA接收电表报文统计量
//
// 调用要求:
// 调用举例:
// 作    者: fengxiaomin/00209182 [2013-03-02]
//*****************************************************************************
#if defined(PRODUCT_CFG_SYS_RST_SOLUTION)
HI_U32 mrsStaRxUartStat(HI_VOID);
HI_U32 mrsRxRMPlcStat(HI_VOID);
#endif

HI_U32 mrsDutUartDataPro(MRS_645_FRAME_STRU *pst645Hrd);
HI_BOOL mrsStaPlcRetryFiltrate(MRS_STA_ITEM * item);
HI_U32 mrsStaFrameBufInsert(HI_U32 id, HI_U16 seq, HI_U16 frame_len, HI_U8 * frame_data, HI_U8 protocol);
HI_U32 mrsStaFrameBufFind(HI_U32 id, HI_U16 seq, HI_U16 *frame_len, HI_U8 ** frame_data, HI_U8 *protocol);
HI_U8* mrsGetSendAddrFrame(HI_U16 * len);

HI_BOOL mrsGetSyncMacFlg(HI_VOID);
HI_VOID mrsSetSyncMacFlg(HI_BOOL flg);
HI_VOID mrsStaPowerOnPro(HI_VOID);
HI_U32 mrsSetCollectDevAddr(HI_U16 usId);

// 刷新首节点信息
HI_VOID mrsStaRefreshTopNodeInf(HI_U8 *, HI_U8 *);

// 设置表地址到Mac层并更新产品类型
HI_U32 mrsStaSetMeterToMac(HI_U8 *, HI_U8, HI_U8);

// 同步Mac层入网地址
HI_U32 mrsStaSyncMacJoinNetAddr(HI_VOID);

// 在搜表结果列表中获取指定表地址信息
MRS_METER_ITEM_S *mrsStaGetMeterInfFromSmResult(HI_U8 *);

// 存储入网表地址到Nv项
HI_VOID mrsStaSaveJoinMeterAddr(HI_U8 *, HI_U8, HI_U16);


HI_VOID mrsGetStaMrParam(NV_APP_MR_CFG_STRU* cfg);

HI_U32 mrsStaTransmitHsoCmd(HI_U16 , HI_PVOID , HI_U16 , HI_U32 );
HI_U32 mrsStaTransmitRemoteCmd(HI_U16 , HI_PVOID , HI_U16 , HI_U32 );
// 设置STA产品类型(普通电表模块:I型采集器模块)
HI_VOID mrsStaSetType(HI_U8);
// 获取STA产品类型
HI_U8 mrsStaGetType(HI_VOID);
// 获取当前PLC通道状态
HI_BOOL mrsStaGetPlcChlStatus(HI_VOID);

HI_U32 mrsStaSerialRespRMPLC(MRS_STA_ITEM *pstItem, MRS_MSG_MR_645_DATA_STRU* pstData);

//将645帧数据转给CCO
HI_U32 mrsStaTransmit2CCO(HI_U8 * data, HI_U16 len, HI_U8 protocol, MRS_STA_ITEM * item);

// 是否允许绑定网络
HI_BOOL mrsStaBindNetworkEnable(HI_VOID);

// 锁定网络
HI_U32 mrsStaBindNetwork(HI_VOID);

// 检查: 若在此锁定时段内，离网时间占比超过门限，离网解锁
HI_U32 mrsNetworkCheckTimeout(HI_VOID);
// 检查: 此锁定时段，离网时间是否达到门限
HI_BOOL mrsCheckReachLeaveThInLockNet(HI_MDM_LOCK_INFO_STRU * stLockInfo);
// 解锁成功后，重新入网
HI_U32 mrsRejoinNetworkAfterUnlock(HI_VOID);

//将645帧转发给HSO
HI_U32 mrsStaDiag2HSO(HI_U8 * data, HI_U16 len,HI_U8 histance);

// Return the specified meter information of the local meter list. CNcomment: 本地地址列表中获取指定表地址信息. CNend
// pucMeter - Meter address
MRS_METER_ITEM_S * mrsStaGetLocalMeterInf(HI_U8 *pucMeter);

// Return 645 frame head timeout by type.
// ucType : MRS_STA_RM_CFG_LOCAL/MRS_STA_RM_CFG_CLTI/MRS_STA_RM_CFG_AUTO
HI_U16 mrsGet645FrameHeadTimeout(HI_U8 ucType);

// Return 645 frame interval timeout by type.
// ucType : MRS_STA_RM_CFG_LOCAL/MRS_STA_RM_CFG_CLTI/MRS_STA_RM_CFG_AUTO
HI_U16 mrsGet645FrameInterval(HI_U8 ucType);

// Return parallel-reading 645 interval timeout by type.
// ucType : MRS_STA_RM_CFG_LOCAL/MRS_STA_RM_CFG_CLTI/MRS_STA_RM_CFG_AUTO
HI_U16 mrsGet645MeterInterval(HI_U8 ucType);

// Return 645 CTRL frame correction by type.
// ucType : MRS_STA_RM_CFG_LOCAL/MRS_STA_RM_CFG_CLTI/MRS_STA_RM_CFG_AUTO
HI_U16 mrsGet645CtrlCmdDelay(HI_U8 ucType);

// Return STA queue max length.
HI_U8 mrsGetStaQueueMax(HI_VOID);

// Compare Dl & Ul frame, check wheher Ul frame match DL frame or not.
HI_BOOL mrsStaIsMr645FrameMatch(HI_U8 * pDlFrame, HI_U16 usDlLength, HI_U8 * pUlFrame, HI_U16 usUlLength);



HI_END_HEADER
#endif // __MRS_SERVICE_STA_H__
