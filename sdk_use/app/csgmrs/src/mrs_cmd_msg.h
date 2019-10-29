//*****************************************************************************
//
//                  版权所有 (C), 2001-2011, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_cmd_msg.h
//  版 本 号   : V1.0
//  作    者   : 沈汉坤/00130424
//  生成日期   : 2011-10-25
//  功能描述   : MRS 命令和消息定义
//  函数列表   : NA
//  修改历史   :
//  1.日    期 : 2011-10-25
//    作    者 : 沈汉坤/00130424
//    修改内容 : 创建文件
//*****************************************************************************

#ifndef __MRS_CMD_MSG_H__
#define __MRS_CMD_MSG_H__

#include <hi_app.h>

HI_START_HEADER


//*****************************************************************************
// 消息定义
// MRS相关的外部消息和内部事件如定时.
//
// 消息ID定义限制说明：
// [0x0000-0x0100]的消息ID预留给MRS框架内部使用, 消息ID必须小于0xFFFF.
//
// shenhankun/2011-10-25
//*****************************************************************************

#define ID_MRS_MSG_PLC_CHL_STATUS       0x200   // PLC通道状态
#define ID_MRS_MSG_MR_TIMER             0x201   // 定时通知消息, 使用MRS_StartTimer后产生的定时器超时通知消息
#define ID_MRS_MSG_MR_DATA              0x210   // 电表/集中器/采集器的数据通知
#define ID_MRS_MSG_MR_DATA_CSG          0x211   // 广东电网集中器数据通知

#define ID_MRS_MSG_HSO_FRAME            0x220   // HSO帧处理
#define ID_UPG_MSG_REPORT               0x230   //上报CCO端的STA进度
//*****************************************************************************

#define PROTO_VER_3762_2009     (0)
#define PROTO_VER_13762_2013    (1)

#define MRS_DEVICE_TYPE_COLLECTOR   DIAG_MRS_DEVICE_TYPE_COLLECTOR
#define MRS_DEVICE_TYPE_METER       DIAG_MRS_DEVICE_TYPE_METER
#define MRS_DEVICE_TYPE_BROAD_CARRIER_COMM_UNIT (2)


#define MRS_MAP_DEV_TYPE_METER      (0)
#define MRS_MAP_DEV_TYPE_CLT_I      (1)
#define MRS_MAP_DEV_TYPE_CLT_II     (2)

#define BCD2INT(x)      (HI_S32)((x)/16 * 10 + ((x) % 16))
#define UCHAR2BCD(x)    (HI_U8)((x)/10 * 16 + ((x) % 10))


typedef enum
{
    MRS_CMD_STRU_VER_R0 = 0,
    MRS_CMD_STRU_VER_R1,
    MRS_CMD_STRU_VER_R2,
    MRS_CMD_STRU_VER_R3,
} MRS_CMD_STRU_VER_E;


#define MRS_AFN13_DL_VER_R0             (MRS_CMD_STRU_VER_R0)
#define MRS_AFN13_DL_VER_R1             (MRS_CMD_STRU_VER_R1)
#define MRS_AFN13_UP_VER_R0             (MRS_CMD_STRU_VER_R0)
#define MRS_AFN13_UP_VER_R1             (MRS_CMD_STRU_VER_R1)
#define MRS_AFN13_DL_VER                (MRS_AFN13_DL_VER_R1)
#define MRS_AFN13_UP_VER                (MRS_AFN13_UP_VER_R1)

#define MRS_AFN14_DL_VER_R0             (MRS_CMD_STRU_VER_R0)
#define MRS_AFN14_DL_VER_R1             (MRS_CMD_STRU_VER_R1)
#define MRS_AFN14_UP_VER_R0             (MRS_CMD_STRU_VER_R0)
#define MRS_AFN14_UP_VER_R1             (MRS_CMD_STRU_VER_R1)
#define MRS_AFN14_DL_VER                (MRS_AFN14_DL_VER_R1)
#define MRS_AFN14_UP_VER                (MRS_AFN14_UP_VER_R1)

#define MRS_AFN_F1_DL_VER_R0            (MRS_CMD_STRU_VER_R0)
#define MRS_AFN_F1_DL_VER_R1            (MRS_CMD_STRU_VER_R1)
#define MRS_AFN_F1_UP_VER_R0            (MRS_CMD_STRU_VER_R0)
#define MRS_AFN_F1_UP_VER_R1            (MRS_CMD_STRU_VER_R1)
#define MRS_AFN_F1_DL_VER               (MRS_AFN_F1_DL_VER_R1)
#define MRS_AFN_F1_UP_VER               (MRS_AFN_F1_UP_VER_R1)

#define MRS_GET_DEVICE_INF_DL_VER_R0    (MRS_CMD_STRU_VER_R0)
#define MRS_GET_DEVICE_INF_DL_VER_R1    (MRS_CMD_STRU_VER_R1)
#define MRS_GET_DEVICE_INF_DL_VER_R2    (MRS_CMD_STRU_VER_R2)
#define MRS_GET_DEVICE_INF_UP_VER_R0    (MRS_CMD_STRU_VER_R0)
#define MRS_GET_DEVICE_INF_UP_VER_R1    (MRS_CMD_STRU_VER_R1)
#define MRS_GET_DEVICE_INF_UP_VER_R2    (MRS_CMD_STRU_VER_R2)
#define MRS_GET_DEVICE_INF_DL_VER       (MRS_GET_DEVICE_INF_DL_VER_R2)
#define MRS_GET_DEVICE_INF_UP_VER       (MRS_GET_DEVICE_INF_UP_VER_R2)

#define MRS_START_SEARCH_DL_VER_R0      (MRS_CMD_STRU_VER_R0)
#define MRS_START_SEARCH_DL_VER_R1      (MRS_CMD_STRU_VER_R1)
#define MRS_START_SEARCH_DL_VER         (MRS_START_SEARCH_DL_VER_R1)

#define MRS_GET_METERLIST_DL_VER_R0     (MRS_CMD_STRU_VER_R0)
#define MRS_GET_METERLIST_DL_VER_R1     (MRS_CMD_STRU_VER_R1)
#define MRS_GET_METERLIST_DL_VER_R2     (MRS_CMD_STRU_VER_R2)
#define MRS_GET_METERLIST_UP_VER_R0     (MRS_CMD_STRU_VER_R0)
#define MRS_GET_METERLIST_UP_VER_R1     (MRS_CMD_STRU_VER_R1)
#define MRS_GET_METERLIST_UP_VER_R2     (MRS_CMD_STRU_VER_R2)
#define MRS_GET_METERLIST_DL_VER        (MRS_GET_METERLIST_DL_VER_R2)
#define MRS_GET_METERLIST_UP_VER        (MRS_GET_METERLIST_UP_VER_R2)

#define MRS_STOP_SEARCH_DL_VER_R0       (MRS_CMD_STRU_VER_R0)
#define MRS_STOP_SEARCH_DL_VER_R1       (MRS_CMD_STRU_VER_R1)
#define MRS_STOP_SEARCH_DL_VER          (MRS_STOP_SEARCH_DL_VER_R1)

#define MRS_BROADCAST_DL_VER_R0         (MRS_CMD_STRU_VER_R0)
#define MRS_BROADCAST_DL_VER_R1         (MRS_CMD_STRU_VER_R1)
#define MRS_BROADCAST_DL_VER            (MRS_BROADCAST_DL_VER_R1)

#define MRS_EVENT_ENABLE_DL_VER_R1      (MRS_CMD_STRU_VER_R1)
#define MRS_EVENT_ENABLE_DL_VER         (MRS_EVENT_ENABLE_DL_VER_R1)

#define MRS_EVENT_INF_DL_VER_R0         (MRS_CMD_STRU_VER_R0)
#define MRS_EVENT_INF_DL_VER_R1         (MRS_CMD_STRU_VER_R1)
#define MRS_EVENT_INF_UP_VER_R0         (MRS_CMD_STRU_VER_R0)
#define MRS_EVENT_INF_UP_VER_R1         (MRS_CMD_STRU_VER_R1)
#define MRS_EVENT_INF_DL_VER            (MRS_EVENT_INF_DL_VER_R1)
#define MRS_EVENT_INF_UP_VER            (MRS_EVENT_INF_UP_VER_R1)

#define MRS_TEST_TRANSMIT_DL_VER_R0     (MRS_CMD_STRU_VER_R0)
#define MRS_TEST_TRANSMIT_DL_VER_R1     (MRS_CMD_STRU_VER_R1)
#define MRS_TEST_TRANSMIT_DL_VER        (MRS_TEST_TRANSMIT_DL_VER_R1)

#define MRS_TEST_AFNF0_DL_VER_R0        (MRS_CMD_STRU_VER_R0)
#define MRS_TEST_AFNF0_DL_VER_R1        (MRS_CMD_STRU_VER_R1)
#define MRS_TEST_AFNF0_UP_VER_R0        (MRS_CMD_STRU_VER_R0)
#define MRS_TEST_AFNF0_UP_VER_R1        (MRS_CMD_STRU_VER_R1)
#define MRS_TEST_AFNF0_DL_VER           (MRS_TEST_AFNF0_DL_VER_R1)
#define MRS_TEST_AFNF0_UP_VER           (MRS_TEST_AFNF0_UP_VER_R1)

#define MRS_CMD_TEST_MODE_DL_VER_R1     (MRS_CMD_STRU_VER_R1)
#define MRS_CMD_TEST_MODE_DL_VER        (MRS_CMD_TEST_MODE_DL_VER_R1)

#define MRS_CMD_TEST_MODE_UP_VER_R1     (MRS_CMD_STRU_VER_R1)
#define MRS_CMD_TEST_MODE_UP_VER        (MRS_CMD_TEST_MODE_UP_VER_R1)


typedef struct
{
    HI_BOOL bAvail; // HI_TRUE: PLC通道可用; HI_FALSE: PLC通道不可用
    HI_U8 reserved[3];
} MRS_MSG_PLC_CHL_STATUS_STRU;

typedef struct
{
    HI_U16 usDataSizse;
    HI_U16 reserved;
    HI_PBYTE pucData; // 完整的数据帧
} MRS_DATA_BUFFER_STRU;

typedef MRS_DATA_BUFFER_STRU MRS_MSG_MR_3762_DATA_STRU;

//
typedef struct
{
    HI_U16   usDataSizse;
    HI_U8    ucProtocol;
    HI_U8    padding[1];
    HI_PBYTE pucData; // 完整的数据帧
} MRS_MSG_MR_645_DATA_STRU;

typedef struct
{
    HI_U8 ucTimerId; // Timer ID
    HI_U8 reserved[3];
} MRS_MSG_MR_TIMER_STRU;


//*****************************************************************************
// 命令定义
// 注意: APP内部定义的命令ID必须小于 HI_DFX_ID_BASE_MAC
//*****************************************************************************
#define ID_MRS_CMD_DATA_TRANSMIT_AFN13_PLC  (0x0001)    // 数据转发命令AFN-13(PLC通道)
#define ID_MRS_CMD_DATA_TRANSMIT_AFN14_PLC  (0x0002)    // 数据转发命令AFN-14(PLC通道)
#define ID_MRS_CMD_PARALLEL_DATA_PLC        (0x0003)    // 并发读表数据转发命令(PLC通道)
#define ID_MRS_CMD_BROADCAST_TRANSMIT_PLC   (0x0004)    // 广播数据转发命令(PLC通道)
#define ID_MRS_CMD_EVENT_TRANSMIT_PLC       (0x0005)    // 从节点事件上报命令(PLC通道)
#define ID_MRS_CMD_TEST_TRANSMIT_PLC        (0x0006)    // 测试命令(PLC通道)
#define ID_MRS_CMD_TEST_TRANSMIT_AFNF0_PLC  (0x0007)    // PLC通道测试(PLC通道)
#define ID_MRS_CMD_EVENT_INF_PLC            (0x0008)    // 从节点事件相关命令(PLC通道)

#define ID_MRS_CMD_GET_METERADDR_PLC        (0x0010)    // 获取电表地址(PLC通道)
#define ID_MRS_CMD_GET_METERLIST            (0x0011)    // 获取搜表结果(PLC通道)
#define ID_MRS_CMD_START_SEARCH_METER       (0x0012)    // 开始搜表命令(PLC通道)
#define ID_MRS_CMD_STOP_SEARCH_METER        (0x0013)    // 停止搜表命令(PLC通道)
#define ID_MRS_CMD_GET_DEVICE_INF_PLC       (0x0014)    // 获取设备信息(映射关系)

#define ID_MRS_CMD_IR_CMD                   (0x0020)

#define ID_MRS_CMD_CSG_DATA_TRANSMIT_PLC    (ID_APP_CMD_CSG_DATA_TRANSMIT_PLC)  // 0x0101

#define ID_MRS_CMD_DATA_TRANSMIT_PLC        (0x0800)    // 数据转发命令


#define ID_MRS_CMD_DATA_TRANSMIT_TEST_MODE  (0xFFFE)
#define ID_MRS_CMD_DATA_TRANSMIT_XR         (ID_MRS_CMD_DATA_TRANSMIT_AFN13_PLC)
#define ID_MRS_CMD_DATA_TRANSMIT_LR         (ID_MRS_CMD_DATA_TRANSMIT_AFN14_PLC)

//*****************************************************************************

#define MRS_STA_STATUS_RESP_OK      0     // 电表正常应答
#define MRS_STA_STATUS_NO_RESP      1     // 电表无应答
#define MRS_STA_STATUS_RESP_ERR     2     // 电表异常应答
#define MRS_STA_STATUS_QUEUE_FULL   3     // 队列满
#define MRS_STA_STATUS_NO_READY     4     // STA端未就绪

#define MRS_PLC_DL_FLG 0    // plc帧下行标志
#define MRS_PLC_UP_FLG 1    // plc帧上行标志


typedef struct
{
    HI_U8 ucMeterAdd[HI_METER_ADDR_LEN];   // 6Byte-BCD
    HI_U8 relay    : 4;
    HI_U8 quality  : 4;
    HI_U8 phase    : 3;
    HI_U8 protocol : 3;
    HI_U8 bak      : 2;
}MR_NODE_METER_ADD_STRU;      // 节点电表地址结构


typedef struct
{
    HI_U16 interface_ver : 6; // 协议版本号
    HI_U16 stru_size : 6;     // 若支持不同版本抄表，可换为 ucOffsetSize; //偏移到数据域的长度,固定位置
    HI_U16 resp_status:4;     // 帧应答状态：电表正常0、电表无应答1、电表异常应答2、队列满3、STA读表状态未就绪4
    HI_U16 protocol : 4;      // 转发数据的规约类型(0 ~ 15)
    HI_U16 data_len : 12;     // 转发数据长度(0 ~ 4095)
    HI_U16 seq;               // 帧序号
    HI_U8 reserved;
    HI_U8 dir : 1;            // 方向位 下行 0; 上行 1
    HI_U8 data[0];            // 转发数据缓冲区指针
} MRS_DATA_TRANSMIT_UP_STRU;  // 数据转发上行结构


typedef struct
{
    HI_U16 interface_ver : 6; // 协议版本号
    HI_U16 stru_size : 6;     // 若支持不同版本抄表，可换为 ucOffsetSize; //偏移到数据域的长度,固定位置
    HI_U16 noresp_flag : 1;   // 未应答重试标志-由CCO给定-STA由此判定未应答后是否需要重试
    HI_U16 nak_flag    : 1;   // 否认重试标志-由CCO给定-STA由此判定接收到否认帧后是否需要重试
    HI_U16 try_num     : 2;   // 最大重试次数
    HI_U16 protocol : 4;      // 转发数据的规约类型(0 ~ 15)
    HI_U16 data_len : 12;     // 转发数据长度(0 ~ 4095)
    HI_U16 seq;               // 帧序号
    HI_U8 time_out;           // STA端超时时间(100ms)(0 ~ 25500ms)
    HI_U8 dir : 1;            // 方向位 下行 0; 上行 1
    HI_U8 data[0];            // 转发数据缓冲区指针
} MRS_DATA_TRANSMIT_DL_STRU;  // 数据转发下行结构

typedef MRS_DATA_TRANSMIT_UP_STRU MRS_DATA_TRANSMIT_AFN13_UP_STRU;
typedef MRS_DATA_TRANSMIT_UP_STRU MRS_DATA_TRANSMIT_AFN14_UP_STRU;
typedef MRS_DATA_TRANSMIT_DL_STRU MRS_DATA_TRANSMIT_AFN13_DL_STRU;
typedef MRS_DATA_TRANSMIT_DL_STRU MRS_DATA_TRANSMIT_AFN14_DL_STRU;


//AFNF1-F1(并发抄表)数据转发下行结构体 （8个字节）
typedef struct
{
    HI_U16 interface_ver : 6;   // 协议版本号
    HI_U16 stru_size     : 6;   // 结构体大小-由发送方给定-描述此结构体的长度-用于接收方找到data的位置
    HI_U16 noresp_try_flag : 1; // 未应答重试标志-由CCO给定-STA由此判定未应答后是否需要重试
    HI_U16 nak_try_flag    : 1; // 否认重试标志-由CCO给定-STA由此判定接收到否认帧后是否需要重试
    HI_U16 try_max       : 2;   // 最大重试次数
    HI_U16 protocol      : 4;   // 转发数据的规约类型(0 ~ 15)
    HI_U16 data_len      : 12;  // 转发数据长度(0 ~ 4095)
    HI_U16 seq;                 // 帧序号
    HI_U8 time_out;             // 电表超时时间(100ms)(0 ~ 25500ms)
    HI_U8 gap_timer;            // 帧间间隔(10ms)(0 ~ 2550ms)
    HI_U8 data[0];              // 转发数据缓冲区指针
} MRS_DATA_TRANSMIT_AFNF1_DL_STRU;


//AFNF1-F1(并发抄表)数据转发上行结构体 （8个字节）
typedef struct
{
    HI_U16 interface_ver : 6;   // 协议版本号
    HI_U16 stru_size     : 6;   // 结构体大小-由发送方给定-描述此结构体的长度-用于接收方找到data的位置
    HI_U16 sta_status    : 4;   // STA应答状态(正常应答为0)
    HI_U16 protocol      : 4;   // 转发数据的规约类型(0 ~ 15)
    HI_U16 data_len      : 12;  // 转发数据长度(0 ~ 4095)
    HI_U16 seq;                 // 帧序号
    HI_U16 resp_status;         // 帧应答状态(每一位标识645多帧的应答状态)
    HI_U8 data[0];              // 转发数据缓冲区指针
} MRS_DATA_TRANSMIT_AFNF1_UP_STRU;

typedef struct
{
    HI_U8  addr[HI_METER_ADDR_LEN]; // 电表地址
    HI_U8  protocol;                // 规约类型
    HI_U8  type : 4;                // 模块类型(电表模块; 采集器模块)
    HI_U8  padd : 4;
    HI_U32 tx_num;                  // 发送帧个数
    HI_U32 rx_num;                  // 接收帧个数
} MRS_CMD_METER_INF;


typedef struct
{
    HI_U8 addr[HI_METER_ADDR_LEN];  // 电表地址
    HI_U8 protocol;                 // 规约类型
    HI_U8 type : 4;                 // 模块类型(电表模块; 采集器模块)
    HI_U8 padd : 4;
} MRS_CMD_METERLIST_ITEM;


typedef struct
{
    HI_U16 interface_ver:6;        // 协议版本号
    HI_U16 stru_len : 6;
    HI_U16 reserved : 4;
    HI_U16 reserved2;

/* BEGIN: PN: DTS2016021906890 MODIFY\ADD\DEL by cuiate/00233580 at 2016/2/20 */
    HI_U8  src[HI_PLC_MAC_ADDR_LEN];
    HI_U8  dst[HI_PLC_MAC_ADDR_LEN];
/* END:   PN: DTS2016021906890 MODIFY\ADD\DEL by cuiate/00233580 at 2016/2/20 */
} MRS_GET_DEVICE_INF_DL_STRU;

typedef struct
{
    HI_U16 interface_ver : 6;   // 协议版本号
    HI_U16 stru_len : 6;
    HI_U16 reserved : 4;
    HI_U8 meter_num;            // 电表数量
    HI_U8 reserved2;
    HI_U8 meter_list[0];        // 电表地址(MRS_CMD_METER_INF)
} MRS_GET_METETADDR_UP_STRU;

typedef struct
{
    HI_U8 aucAddr[HI_METER_ADDR_LEN];
    HI_U8 ucProtocol;
    HI_U8 ucType : 4;
    HI_U8 ucStatus : 2; //STA本地映射表就绪状态：0-未就绪（正在本地搜表）1-已就绪（本地搜表完成） 其它-保留
    HI_U8 reserved: 2;
} MRS_DEVICE_INF_STRU;

typedef struct
{
    HI_U8 aucAddr[HI_METER_ADDR_LEN];
    HI_U8 ucProtocol;
    HI_U8 ucType : 4;
    HI_U8 reserved: 4;
} MRS_METER_INF_STRU;

typedef struct
{
    HI_U16 interface_ver : 6;
    HI_U16 stru_len : 6;
    HI_U16 reserved : 4;
    HI_U8 meter_num;            // 电表数量
    HI_U8 reserved2;
    MRS_DEVICE_INF_STRU stDeviceInf; //设备信息
/* BEGIN: PN: DTS2016021906890 MODIFY\ADD\DEL by cuiate/00233580 at 2016/2/20 */
    HI_U8  src[HI_PLC_MAC_ADDR_LEN];
    HI_U8  dst[HI_PLC_MAC_ADDR_LEN];
/* END:   PN: DTS2016021906890 MODIFY\ADD\DEL by cuiate/00233580 at 2016/2/20 */
    HI_U8 meter_list[0];        // 表信息(MRS_METER_INF_STRU)
} MRS_GET_DEVICE_INF_UP_STRU;

typedef enum
{
    MRS_SM_SEARCH_RESULT,
    MRS_SM_START_SEARCH,
} MRS_SM_OPTION_E;


/* BEGIN: PN: DTS2016021906890 MODIFY\ADD\DEL by cuiate/00233580 at 2016/2/20 */
typedef struct
{
    HI_U16 interface_ver : 6;   // 协议版本号
    HI_U16 stru_len : 6;        // 结构体长度
    HI_U16 force_resp : 1;      // 强制应答标志
    HI_U16 option : 3;          // 搜表参数(MRS_SM_OPTION_E): 填写MRS_SM_START_SEARCH
    HI_U16 reserved;
    HI_U32 seq;                 // 搜表序列号:每次搜表具有唯一的一个序号,重新搜表序号++
} MRS_PLC_START_SEARCH_METER_S;


typedef struct
{
    HI_U16 interface_ver : 6;   // 协议版本号
    HI_U16 stru_len : 6;        // 结构体长度
    HI_U16 force_resp : 1;      // 强制应答标志
    HI_U16 option : 3;          // 搜表参数(MRS_SM_OPTION_E): 填写MRS_SM_SEARCH_RESULT
    HI_U16 reserved;
    HI_U32 seq;                 // 搜表序列号:每次搜表具有唯一的一个序号,重新搜表序号++
    HI_U8  src[HI_PLC_MAC_ADDR_LEN];
    HI_U8  dst[HI_PLC_MAC_ADDR_LEN];
} MRS_PLC_METERLIST_DL_S;
/* END:   PN: DTS2016021906890 MODIFY\ADD\DEL by cuiate/00233580 at 2016/2/20 */

// 获取搜表结果或映射表(上行)
typedef struct
{
    HI_U16 interface_ver : 6;       // 协议版本号
    HI_U16 stru_len : 6;            // 结构体长度
    HI_U16 status   : 1;            // 状态字段，强制应答时使用
    HI_U16 option   : 3;            // 操作选项(MRS_SM_OPTION_E)
    HI_U8  meter_num;               // 电表数量
    HI_U8  type;                    // 0:电表 1:采集器

    HI_U8  asset[HI_METER_ADDR_LEN];// STA/采集器地址
    HI_U8  id[HI_METER_ADDR_LEN];

    HI_U32 seq;                     // 搜表序列号:每次搜表具有唯一的一个序号,将本次搜表的序号回传CCO

    HI_U8  abnormal;                // II采可维可测: 上报搜表异常类型 置位：0:正常 1:告警 2:预警 4:多II采连接
    HI_U8  pad[3];

/* BEGIN: PN: DTS2016021906890 MODIFY\ADD\DEL by cuiate/00233580 at 2016/2/20 */
    HI_U8  src[HI_PLC_MAC_ADDR_LEN];
    HI_U8  dst[HI_PLC_MAC_ADDR_LEN];
/* END:   PN: DTS2016021906890 MODIFY\ADD\DEL by cuiate/00233580 at 2016/2/20 */

    HI_U8  meter_list[0];           // 电表地址包含数据结构(MRS_CMD_METERLIST_ITEM)
} MRS_PLC_METERLIST_UP_S;


// 停止搜表(下行)
typedef struct
{
    HI_U16 interface_ver : 6;       // 协议版本号
    HI_U16 stru_len : 6;            // 结构体长度
    HI_U16 reserved1: 4;
    HI_U16 reserved2;
    HI_U32 seq;                     // 搜表序列号:填写本次搜表的序号
} MRS_PLC_STOP_SEARCH_METER_S;

typedef struct
{
    HI_BOOL bDir;   // 0-下行命令(CCO to STA) 1-上行命令(STA to CCO)
    HI_BOOL bFlg;   // HI_TRUE:确认; HI_FALSE:否认
    HI_U16 usId;    // 被确认或否认的ID号(0xFFFF则为不区分ID号)
} MRS_ID_ACK_DENY_STRU;


#define MRS_EVENT_DI_ARRAY  "\x34\x48\x33\x37"  //"\x01\x15\x00\x04"
#define MRS_EVENT_DI_LEN    (4)
#define MRS_EVENT_CTRL      (0x11)


typedef struct
{
    HI_U16 interface_ver : 6;   // 协议版本号
    HI_U16 stru_len      : 6;   // 偏移到数据域的长度，固定位置
    HI_U16 reserved      : 4;
    HI_U16 seq           : 4;
    HI_U16 data_len      : 12;  // 数据长度
    HI_U8 data[0];
} MRS_BROADCAST_STRU;

typedef enum
{
    MRS_EVENT_DIR_BIT_CCO2STA,  // CCO->STA
    MRS_EVENT_DIR_BIT_STA2CCO,  // STA->CCO
} MRS_EVENT_DIR_BIT_E;

typedef enum
{
    MRS_EVENT_FRM_BIT_TO,   // 报文来自从动站
    MRS_EVENT_FRM_BIT_FROM, // 报文来自主动站
} MRS_EVENT_FRM_BIT_E;


#define MRS_EVENT_FUN_ACK               (1)  // 事件功能码: 确认(下行)
#define MRS_EVENT_FUN_REPORT_ENABLE     (2)  // 事件功能码: 允许事件主动上报(下行)
#define MRS_EVENT_FUN_REPORT_DISABLE    (3)  // 事件功能码: 禁止事件主动上报(下行)
#define MRS_EVENT_FUN_CCOBUF_FULL       (4)  // 事件功能码: CCO侧缓存满(下行)

#define MRS_EVENT_FUN_REPORT            (1)  // 事件功能码: 主动上报(上行)
#define MRS_EVENT_FUN_REPORT_MODUL      (30)    //事件功能码，主动上报(上行)，模块触发 和国网版本不同


typedef struct
{
    HI_U32 ulInterfaceVer   : 6;   // 协议版本号
    HI_U32 ulStructLen      : 6;   // 偏移到数据域的长度，固定位置
    HI_U32 ulDirBit         : 1;   // 方向位: 0:CCO发给STA; 1:STA发给CCO
    HI_U32 ulFrmBit         : 1;   // 启动位: 0:报文来自从动站; 1:报文来自启动站
    HI_U32 ulFunCode        : 6;   // 功能码:
    HI_U32 ulDataLen        : 12;  // 转发数据长度
    HI_U16 usSeq;                  // 帧序号
    HI_U8  ucMeter[6];             // 电表地址: 下行:暂无定义
                                   //           上行:发生事件的电表地址
    HI_U8  ucData[0];              // 转发数据缓冲区指针
} MRS_EVENT_INFO_STRU;        /* 对应PLC ID 为 ID_MRS_CMD_EVENT_INF_PLC */


typedef struct
{
    HI_U16 interface_ver : 6;
    HI_U16 stru_len      : 6;
    HI_U16 timeout       : 4;
    HI_U16 protocol      : 4;
    HI_U16 data_len      : 12;
    HI_U8 data[0];
} MRS_TEST_TRANSMIT_STRU;


typedef struct
{
    HI_U16 interface_ver : 6;
    HI_U16 stru_len      : 6;
    HI_U16 reserved1     : 4;
    HI_U16 data_len;
    HI_U16 expect_len;
    HI_U16 reserved2;
    HI_U8  data[0];
} MRS_TEST_TRANSMIT_AFNF0_STRU;


typedef struct
{
    HI_U16 usCmdId;
    HI_U16 usPayloadLen;
    HI_U8 aucPayload[0];
} MRS_CMD_DATA_TRANSMIT_STRU;


typedef struct
{
    HI_U16 usInterfaceVer : 6;
    HI_U16 usStruLen : 6;
    HI_U16 usPadding1 : 4;
    HI_U8 ucSnId;
    HI_U8 ucPadding2;
} MRS_CMD_TEST_MODE_STRU;


// 帧类型域
#define MRS_CSG_PLC_CTRL_TYPE_ACK           (APP_CSG_PLC_CTRL_TYPE_ACK)
#define MRS_CSG_PLC_CTRL_TYPE_TRANS         (APP_CSG_PLC_CTRL_TYPE_TRANS)
#define MRS_CSG_PLC_CTRL_TYPE_CMD           (APP_CSG_PLC_CTRL_TYPE_CMD)
#define MRS_CSG_PLC_CTRL_TYPE_REPROT        (APP_CSG_PLC_CTRL_TYPE_REPORT)
#define MRS_CSG_PLC_CTRL_TYPE_TEST          (APP_CSG_PLC_CTRL_TYPE_TEST)

// 业务扩展域标识位
#define MRS_CSG_PLC_CTRL_EXTEND_BIT_N       (APP_CSG_PLC_CTRL_EXTEND_BIT_N)
#define MRS_CSG_PLC_CTRL_EXTEND_BIT_Y       (APP_CSG_PLC_CTRL_EXTEND_BIT_Y)

// 响应标识位
#define MRS_CSG_PLC_CTRL_RESP_BIT_N         (APP_CSG_PLC_CTRL_RESP_BIT_N)
#define MRS_CSG_PLC_CTRL_RESP_BIT_Y         (APP_CSG_PLC_CTRL_RESP_BIT_Y)

// 启动标志位
#define MRS_CSG_PLC_CTRL_PRM_SLAVE          (APP_CSG_PLC_CTRL_PRM_SLAVE)
#define MRS_CSG_PLC_CTRL_PRM_HOST           (APP_CSG_PLC_CTRL_PRM_HOST)

// 传输方向位
#define MRS_CSG_PLC_CTRL_DIR_DL             (APP_CSG_PLC_CTRL_DIR_DL)
#define MRS_CSG_PLC_CTRL_DIR_UL             (APP_CSG_PLC_CTRL_DIR_UL)

#define MRS_CSG_PLC_PROTO_VERSION           (APP_CSG_PLC_PROTO_VERSION)


// 确认/否认
#define MRS_CSG_PLC_SRV_ID_ACK_CONFIRM      (APP_CSG_PLC_SRV_ID_ACK_CONFIRM)        // 确认
#define MRS_CSG_PLC_SRV_ID_ACK_DENY         (APP_CSG_PLC_SRV_ID_ACK_DENY)           // 否认

// 数据转发
#define MRS_CSG_PLC_SRV_ID_TRANS_XR         (APP_CSG_PLC_SRV_ID_TRANS_XR)           // 数据透传

// PLC灵敏度测试
#define MRS_CSG_PLC_SRV_ID_PLC_TEST         (APP_CSG_PLC_SRV_ID_PLC_TEST)           //PLC灵敏度测试

// 命令
#define MRS_CSG_PLC_SRV_ID_CMD_SM_RESULT    (APP_CSG_PLC_SRV_ID_CMD_SM_RESULT)      // 查询搜表结果
#define MRS_CSG_PLC_SRV_ID_CMD_DL_METERLIST (APP_CSG_PLC_SRV_ID_CMD_DL_METERLIST)   // 下发搜表列表
#define MRS_CSG_PLC_SRV_ID_CMD_FILE         (APP_CSG_PLC_SRV_ID_CMD_FILE)           // 文件传输
#define MRS_CSG_PLC_SRV_ID_CMD_EVENT_FLAG   (APP_CSG_PLC_SRV_ID_CMD_EVENT_FLAG)     // 允许/禁止从节点事件
#define MRS_CSG_PLC_SRV_ID_CMD_REBOOT       (APP_CSG_PLC_SRV_ID_CMD_REBOOT)         // 从节点重启
#define MRS_CSG_PLC_SRV_ID_CMD_UPG_INFO     (APP_CSG_PLC_SRV_ID_CMD_UPG_INFO)       // 查询升级信息
#define MRS_CSG_PLC_SRV_ID_CMD_BIND         (APP_CSG_PLC_SRV_ID_CMD_BIND)           // 锁定命令
#define MRS_CSG_PLC_SRV_ID_CMD_TEST         (APP_CSG_PLC_SRV_ID_CMD_TEST)           // 测试帧

// 主动上报
#define MRS_CSG_PLC_SRV_ID_REPORT_EVENT     (APP_CSG_PLC_SRV_ID_REPORT_EVENT)       // 事件主动上报


typedef APP_CSG_PLC_FRAME_HEAD      MRS_CSG_PLC_FRAME_HEAD;
typedef APP_CSG_TRANS_XR_DL_STRU    MRS_CSG_TRANS_XR_DL_STRU;
typedef APP_CSG_TRANS_XR_UL_STRU    MRS_CSG_TRANS_XR_UL_STRU;


HI_END_HEADER
#endif // __MRS_CMD_MSG_H__

