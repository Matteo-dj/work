//*****************************************************************************
//
//                  版权所有 (C), 1998-2011, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_srv_readmeter_manage.h
//  作    者   : liuxipeng/KF54842
//  生成日期   : 2014-02-28
//  功能描述   : 读表管理模块外部函数声明及宏定义
//
//  函数列表   :
//  修改历史   :
//  1.日    期 : 2014-02-28
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

#ifndef __MRS_SRV_READMETER_MANAGE_H__
#define __MRS_SRV_READMETER_MANAGE_H__

HI_START_HEADER

// 业务读表类型定义
#define MRS_RM_TYPE_AFN02H_F1  (0x0201)    // "数据转发"业务读表
#define MRS_RM_TYPE_AFN13H_F1  (0x1301)    // "点抄"业务读表 (监控载波从节点)
#define MRS_RM_TYPE_AFN14H_F1  (0x1401)    // "轮抄"业务读表 (路由数据抄读)
#define MRS_RM_TYPE_AFNF1H_F1  (0xF101)    // "并发"业务读表 (自定义)
#define MRS_RM_TYPE_MAX_NUM    (4)         // 支持的读表类型最大值


// 业务读表重传策略
typedef enum
{
    EN_MRS_RM_RETRY_STRATEGY_01 = 0,  // 策略1-适用于点抄"AFN=13H-F1"
    EN_MRS_RM_RETRY_STRATEGY_02,      // 策略2-适用于轮抄、并发、数据转发
    MRS_RM_RETRY_TACTIC_MAX
} MRS_RM_RETRY_STRATEGY_ENUM;

// 重传配置数量
typedef enum
{
    EN_MRS_RETRY_CFG_01 = 0,        // "点抄"-"数据转发"推荐使用
    EN_MRS_RETRY_CFG_02,            // "轮抄"推荐使用
    EN_MRS_RETRY_CFG_03             // "并发"推荐使用
} MRS_RETRY_CFG_NUM_ENUM;

// 并发池数量
typedef enum
{
    EN_MRS_PARALLEL_POOL_HIGH = 0,  // "点抄"读表推荐使用
    EN_MRS_PARALLEL_POOL_LOW        // 其它读表请求推荐使用
} MRS_PARALLEL_POOL_ENUM;


#define MRS_RM_MAC_MAX_LAYER        (8)     // MAC支持的最大层级,需和Mac最大层级一致
#define MRS_RM_MAC_MIN_LAYER        (1)     // MAC支持的最小层级,需和Mac最小层级一致
#define MRS_RM_DEFAULT_LAYER_BACST  (3)     // 广播默认层级,用于计算广播超时


// 重传配置默认及边界值定义
#define MRS_RM_METER_FRAME_INTERVAL_MAX         (1000 * 10)     // STA端645帧最大间隔时间
#define MRS_RM_METER_FRAME_INTERVAL_DEFAULT     (50)            // STA端645帧默认间隔时间
#define MRS_RM_METER_FRAME_INTERVAL_MIN         (0)             // STA端645帧最小间隔时间

#define MRS_RM_PLC_LAYER_DELAY_MAX              (10000)
#define MRS_RM_PLC_LAYER_DELAY_DEFAULT          (500)
#define MRS_RM_PLC_LAYER_DELAY_MIN              (0)

#define MRS_RM_PLC_BC_OVERTIME_MAX              (30000)
#define MRS_RM_PLC_BC_OVERTIME_DEFAULT          (MRS_RM_PLC_LAYER_DELAY_DEFAULT * MRS_RM_DEFAULT_LAYER_BACST)
#define MRS_RM_PLC_BC_OVERTIME_MIN              (1000)

#define MRS_RM_PLC_REVISORY_MAX                 (1000 * 10)     // PLC通讯超时修正值(最大)
#define MRS_RM_PLC_REVISORY_DEFAULT             (0)             // PLC通讯超时修正值(默认)
#define MRS_RM_PLC_REVISORY_MIN                 (-1000 * 10)    // PLC通讯超时修正值(最小)

#define MRS_RM_CTRL_CMD_DELAY_MAX               (1000 * 10)     // 费控命令补充最大超时时间
#define MRS_RM_CTRL_CMD_DELAY_DEFAULT           (2000)          // 费控命令补充默认超时时间
#define MRS_RM_CTRL_CMD_DELAY_MIN               (0)             // 费控命令补充最小超时时间

#define MRS_RM_CLTI_DELAY_MAX                   (1000 * 10)     // I型采集器补充最大超时时间
#define MRS_RM_CLTI_DELAY_DEFAULT               (1800)          // I型采集器补充默认超时时间
#define MRS_RM_CLTI_DELAY_MIN                   (0)             // I型采集器补充最小超时时间

#define MRS_RM_PLC_RETRY_LOOP_MAX               (10)            // [点抄]PLC通道单播+广播重试轮数(最大)
#define MRS_RM_PLC_RETRY_LOOP_DEFAULT           (1)             // [点抄]PLC通道单播+广播重试轮数(默认)
#define MRS_RM_PLC_RETRY_LOOP_MIN               (1)             // [点抄]PLC通道单播+广播重试轮数(最小)

#define MRS_RM_PLC_RETRY_CNT_MAX                (10)            // PLC通道单播重试次数(最大)
#define MRS_RM_PLC_RETRY_CNT_DEFAULT            (2)             // PLC通道单播重试次数(默认)
#define MRS_RM_PLC_RETRY_CNT_MIN                (0)             // PLC通道单播重试次数(最小)

#define MRS_RM_PLC_BC_RETRY_CNT_MAX             (10)            // PLC通道广播重试次数(最大)
#define MRS_RM_PLC_BC_RETRY_CNT_DEFAULT         (1)             // PLC通道广播重试次数(默认)
#define MRS_RM_PLC_BC_RETRY_CNT_MIN             (-10)           // PLC通道广播重试次数(最小)

#define MRS_RM_METER_RETRY_CNT_DEFAULT          (1)             // 电表端超时重传次数
#define MRS_RM_METER_RETRY_CFG_DEFAULT          (3)             // 电表端超时重传配置默认值(Bit0~1; Bit1~1)
#define MRS_RM_MAX_SERVICE_TIME_DEFAULT         (60)            // [点抄]单次抄表服务最大时间默认值 单位:S
#define MRS_RM_MAX_SERVICE_TIME_MIN             (3)             // [点抄]单次抄表服务最大时间(最小) 单位:S
#define MRS_RM_MAX_SERVICE_TIME_SUB             (2)             // [点抄]单次抄表服务最大时间使用时应减去此时间 单位:S

#define MRS_RM_CSG_MAX_SERVICE_TIME_DEFAULT     (20)            // 低优先级单次抄表服务最大时间默认值 单位:S

// 读表请求(应答)结构
typedef struct
{
    HI_U8 * ucAddr;     // 目的电表地址
    HI_U16  usRmType;   // 业务读表类型
    HI_U8   ucProtocol; // 数据转发帧规约类型
    HI_U8   ucMrSeq;    // 业务读表376.2帧序列号
    HI_U16  usDataLen;  // 业务读表数据转发长度
    HI_U8   reserved[2];
    HI_U8 * pucData;    // 业务读表数据转发内容

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
    HI_VOID* pvParam;
#endif
} MRS_RM_REQ_DATA, *P_MRS_RM_REQ_DATA, MRS_RM_RESP_DATA, *P_MRS_RM_RESP_DATA;

/* BEGIN: PN: DTS2015033100426 MODIFY\ADD\DEL by cuiate/00233580 at 2015/3/27 */
typedef NV_MRS_RETRY_CFG_S      MRS_RETRY_CFG_S, *P_MRS_RETRY_CFG_S;
typedef NV_MRS_PARALLEL_CFG_S   MRS_PARALLEL_CFG_S, *P_MRS_PARALLEL_CFG_S, *P_NV_MRS_PARALLEL_CFG_S;
/* END:   PN: DTS2015033100426 MODIFY\ADD\DEL by cuiate/00233580 at 2015/3/27 */


// 读表请求应答处理回调函数类型
typedef HI_U32 (* MRS_RM_DATA_RESP_PROC_F)(P_MRS_RM_RESP_DATA);


// 读表管理模块初始化
HI_PUBLIC HI_U32 mrsRmModuleInit(HI_VOID);


// 业务读表策略注册
HI_PUBLIC HI_U32 mrsRmRegisterStrategy(HI_U16, MRS_RETRY_CFG_NUM_ENUM, MRS_RM_RETRY_STRATEGY_ENUM,
                                            MRS_PARALLEL_POOL_ENUM, MRS_RM_DATA_RESP_PROC_F);

// 读表请求加入缓存池
HI_PUBLIC HI_U32 mrsRmReqJoinPool(P_MRS_RM_REQ_DATA);

// PLC发送请求处理
HI_PUBLIC HI_U32 mrsRmPlcSendReqProc(HI_SYS_QUEUE_MSG_S *);

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
HI_PUBLIC HI_U32 mrsRmPlcPackageReceiveCsg(HI_U8 *pucPayload, HI_U16 usPayloadLen);
HI_PUBLIC HI_VOID mrsRmRemoveItemFromPool(HI_VOID *pParam);
#else
// PLC包接收
HI_PUBLIC HI_U32 mrsRmPlcPackageReceive(MRS_DATA_TRANSMIT_AFNF1_UP_STRU *, HI_U16);
#endif

// 读表模块超时检查定时器处理
HI_PUBLIC HI_VOID mrsRmCheckTimerProc(HI_VOID);
HI_PUBLIC HI_U16 mrsGetNvParallelNum(HI_VOID);

// 复位读表管理模块(集中器下发AFN=12-F1路由重启时调用)
HI_PUBLIC HI_VOID mrsRmModuleReset(HI_VOID);

/* BEGIN: PN: DTS2015033100426 MODIFY\ADD\DEL by cuiate/00233580 at 2015/3/26 */
HI_PUBLIC HI_VOID mrsRmCfgInit(NV_MRS_RETRY_CFG_S *);
HI_PUBLIC HI_VOID mrsRmCfgParallelInit(NV_MRS_PARALLEL_CFG_S *);
HI_PUBLIC HI_VOID mrsRmRetryCfgInit(P_MRS_RETRY_CFG_S, NV_MRS_RETRY_CFG_S *);
/* END:   PN: DTS2015033100426 MODIFY\ADD\DEL by cuiate/00233580 at 2015/3/26 */
HI_PUBLIC HI_U32 mrsGetMacFromTopo(HI_U8 *pucMeterAddr, HI_U8 *pucMacAddr);
HI_PUBLIC HI_VOID mrsSetParallelNum(HI_U16 usNum);
HI_PUBLIC HI_U16 mrsGetParallelNum(HI_VOID);

HI_END_HEADER

#endif // __MRS_SRV_READMETER_MANAGE_H__


