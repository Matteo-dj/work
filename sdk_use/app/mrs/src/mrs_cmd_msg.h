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

#define ID_MRS_MSG_MR_DATA_645          0x210
#define ID_MRS_MSG_MR_DATA_698_45       0x211
#define ID_MRS_MSG_MR_DATA_TRANSPARENT  0x212
#define ID_MRS_MSG_MR_DATA_CJT_188  	0x213

#define ID_MRS_MSG_HSO_FRAME            0x220   // HSO帧处理
#define ID_UPG_MSG_REPORT               0x230   //上报CCO端的STA进度
//CJJ-HPLC:20180922
#define ID_RF_IQR_MSG_REPORT            0x240   //无线中断消息
#define ID_RF_WIRE_LESS_REPORT          0x250   //无线接收数据消息处理
//CJJ-HPLC:20180922

//*****************************************************************************

#define MRS_PORT_NUM_OFFSET_IN_APP_FRAME    0   // 在应用层报文中，端口号的偏移
#define MRS_PORT_NUM_LEN_IN_APP_FRAME       1   // 在应用层报文中，端口号的长度
#define MRS_CMD_ID_OFFSET_IN_APP_FRAME      1   // 在应用层报文中，报文ID的偏移
#define MRS_CMD_ID_LEN_IN_APP_FRAME         2   // 在应用层报文中，报文ID的长度
#define MRS_HEAD_LEN_IN_APP_FRAME           4   // 在应用层报文中，报文头长度

#define PROTO_VER_3762_2009     (0)
#define PROTO_VER_13762_2013    (1)

#define MRS_DEVICE_TYPE_COLLECTOR   DIAG_MRS_DEVICE_TYPE_COLLECTOR
#define MRS_DEVICE_TYPE_METER       DIAG_MRS_DEVICE_TYPE_METER
#define MRS_DEVICE_TYPE_BROAD_CARRIER_COMM_UNIT (5)       //Matteo
#define MRS_POWER_FAILURE_COMMUNICATE_TYPE      (4)


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

/* BEGIN: PN: DTS2016021906890 MODIFY\ADD\DEL by cuiate/00233580 at 2016/2/20 */
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
#define MRS_GET_METERLIST_DL_VER        (MRS_GET_METERLIST_DL_VER_R1)
#define MRS_GET_METERLIST_UP_VER        (MRS_GET_METERLIST_UP_VER_R1)
/* END:   PN: DTS2016021906890 MODIFY\ADD\DEL by cuiate/00233580 at 2016/2/20 */

#define MRS_STOP_SEARCH_DL_VER_R0       (MRS_CMD_STRU_VER_R0)
#define MRS_STOP_SEARCH_DL_VER_R1       (MRS_CMD_STRU_VER_R1)
#define MRS_STOP_SEARCH_DL_VER          (MRS_STOP_SEARCH_DL_VER_R1)

#define MRS_BROADCAST_DL_VER_R0         (MRS_CMD_STRU_VER_R0)
#define MRS_BROADCAST_DL_VER_R1         (MRS_CMD_STRU_VER_R1)
#define MRS_BROADCAST_DL_VER            (MRS_BROADCAST_DL_VER_R1)

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

#define MRS_CMD_TEST_NTB_DATA_VER       (MRS_CMD_STRU_VER_R1)


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

#define ID_MRS_CMD_GET_POWEROFF_INFO        (0x0015)    // 获取站点停电信息(PLC通道)

#define ID_MRS_CMD_IR_CMD                   (0x0020)
#define ID_MRS_CMD_NDM_CTRL_CCO_PLC         (0x0040)    // 运维抄控宝-CCO(PLC通道)
#define ID_MRS_CMD_RF_SET_TIME_SLOT         (0x0041)
#define ID_MRS_CMD_TF_PLC                   (0x00A1)    // 台区户变关系识别(PLC通道)
#define ID_MRS_CMD_GET_DEVICE_ID_PLC        (0x00A2)    // 获取子设备各类ID(芯片ID和设备ID)

#define ID_MRS_CMD_DATA_TRANSMIT_PLC        (0x0800)    // 数据转发命令

#define ID_MRS_CMD_DATA_TRANSMIT_TEST_MODE  (0xFFFE)
#define ID_MRS_CMD_DATA_TRANSMIT_XR         (ID_MRS_CMD_DATA_TRANSMIT_AFN13_PLC)
#define ID_MRS_CMD_DATA_TRANSMIT_LR         (ID_MRS_CMD_DATA_TRANSMIT_AFN14_PLC)

//*****************************************************************************

//#define MRS_METER_TIMER_UNIT_BASE   100   // 抄表数据转发中STA端超时时间单位基数(百毫秒)
//#define MRS_GAP_TIMER_UNIT_BASE     10    // 抄表数据转发中STA端帧间隔时间单位基数(十毫秒)

#define MRS_STA_STATUS_RESP_OK      0     // 电表正常应答
#define MRS_STA_STATUS_NO_RESP      1     // 电表无应答
#define MRS_STA_STATUS_RESP_ERR     2     // 电表异常应答
#define MRS_STA_STATUS_QUEUE_FULL   3     // 队列满
#define MRS_STA_STATUS_NO_READY     4     // STA端未就绪

#define MRS_PLC_DL_FLG 0    // plc帧下行标志
#define MRS_PLC_UP_FLG 1    // plc帧上行标志

#define MRS_PLC_FROM_FOLLOW     0   // 启动位: 来自从动站
#define MRS_PLC_FROM_START      1   // 启动为: 来自启动站

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
    HI_U16 resp_status:4;     // 固定为0
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
    HI_U16 pad : 4;
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

typedef struct {
    hi_u16 interface_ver:6;        // 协议版本号
    hi_u16 stru_len : 6;
    hi_u16 reserved : 4;
    hi_u16 reserved2;
} mrs_get_device_inf_dl_ver1;

typedef struct
{
    HI_U16 interface_ver:6;        // 协议版本号
    HI_U16 stru_len : 6;
    HI_U16 reserved : 4;
    HI_U16 reserved2;
    HI_U8  src[HI_PLC_MAC_ADDR_LEN];
    HI_U8  dst[HI_PLC_MAC_ADDR_LEN];
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
	HI_U8 ucStatus : 2; //STA本地映射表就绪状态：1-未就绪（正在本地搜表）0-已就绪（本地搜表完成） 其它-保留
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
    MRS_SM_SEARCH_RESULT = 0,   // 查询从节点注册结果命令
    MRS_SM_START_SEARCH,        // 启动从节点主动注册命令
    MRS_SM_BIND_NETWORK,        // 通知从节点锁定网络命令
	MRS_SM_WATER_CMD = 4,
    MRS_SM_OPTION_MAX,
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
	HI_U8  waterMeter_Data[0]; 
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

    HI_U32  reserved;                // 预留

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
    HI_U16 reserved1     : 4;
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


#define MRS_EVENT_FUN_ACK                (1)  // 事件功能码: 确认(下行)
#define MRS_EVENT_FUN_REPORT_ENABLE      (2)  // 事件功能码: 允许事件主动上报(下行)
#define MRS_EVENT_FUN_REPORT_DISABLE     (3)  // 事件功能码: 禁止事件主动上报(下行)
#define MRS_EVENT_FUN_CCOBUF_FULL        (4)  // 事件功能码: CCO侧缓存满(下行)

#define MRS_EVENT_FUN_REPORT             (1)  // 事件功能码: 主动上报(上行)
#define MRS_EVENT_FUN_REPORT_MODUL       (2)  //事件功能码:  主动上报(上行)，模块触发
#define MRS_EVENT_FUN_REPORT_CLT         (3)  // 事件功能码: 主动上报(上行)，采集器触发

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


typedef struct
{
    HI_U16 usInterfaceVer : 6;
    HI_U16 usStruLen : 6;
    HI_U16 usPadding : 4;
    HI_U16 usNtbCnt;            /* NTB数据个数，目前固定为20 */
    HI_S16 sNtbAjustBase;       /* NTB校正基准，CCO发送给STA  */
    HI_U16 usPadding1;
    HI_U32 aulNtb[0];
} MRS_CMD_TEST_NTB_DATA_STRU;

#define MRS_PLC_POWEROFF_INFO_DL_MAC_NUM    (10)    // 查询停电信息下行一次携带MAC个数
// 查询最近停电信息（下行）
typedef struct
{
    HI_U16 interface_ver    : 6;    // 协议版本号
    HI_U16 stru_len         : 6;    // 结构体长度
    HI_U16 detect_flag      : 1;    // 探测标识
    HI_U16 reserved1        : 3;
    HI_U16 sta_num          : 6;    // 需要应答的站点个数
    HI_U16 reserved2        : 10;
    HI_U32 seq;                     // 查询序列号：每次查询具有唯一序列号
    HI_U8 mac_list[0];
} MRS_PLC_POWEROFF_INFO_DL_S;

// 查询最近停电信息（上行）
typedef struct
{
    HI_U16 interface_ver    : 6;    // 协议版本号
    HI_U16 stru_len         : 6;    // 结构体长度
    HI_U16 reserved1        : 4;
    HI_U16 reserved2;
    HI_U32 seq;                     // 查询序列号：每次查询具有唯一序列号
    HI_U32 last_poweroff_time;      // 最近一次停电发生时间
    HI_U32 last_poweron_time;       // 最近一次停电结束时间
} MRS_PLC_POWEROFF_INFO_UP_S;

//停电上报报文bitmap版本结构体
// 查询最近停电信息（下行）,暂未实现
/*
typedef struct
{
HI_U16 interface_ver    : 6;     // 协议版本号
HI_U16 stru_len         : 6;     // 结构体长度
HI_U16 option           : 3;    // 0探测帧，1查询帧
HI_U16 map_index        : 1;    // 0:TEI为0-511, 1:TEI为512-1023
HI_U16 reserved;
HI_U32 seq;                     // 查询序列号：每次查询具有唯一序列号
HI_U16 min_leave_len;           // 最小离线时长
HI_U16 min_rest_len;            // 最小剩余时间（上报集中器）
HI_U8  tei_bit_map[0];          // 待查询站点的TEI位图
} MRS_PLC_POWEROFF_BITMAP_INFO_DL_S;
*/

// 查询最近停电信息（上行）
/*
typedef struct
{
HI_U16 interface_ver    : 6;     // 协议版本号
HI_U16 stru_len         : 6;     // 结构体长度
HI_U16 reserved1        : 4;
HI_U16 reserved2;
HI_U32 seq;                     // 查询序列号：每次查询具有唯一序列号
HI_U32 last_poweroff_time;      // 最近一次停电发生时间
HI_U32 last_poweron_time;       // 最近一次停电结束时间
} MRS_PLC_POWEROFF_BITMAP_INFO_UP_S;
*/

#define MRS_CMD_TF_PORT_NUM                 0x11    // 台区户变关系识别报文的端口号
#define MRS_CMD_TF_NTB_COMPRESS_TIMES       8       // NTB数据压缩倍数
#define MRS_CMD_TF_MAX_DIFF_NUM_1EDGE       223     // 特征信息告知报文中，单沿时，告知总数量最大值
#define MRS_CMD_TF_MAX_DIFF_NUM_2EDGE       219     // 特征信息告知报文中，双沿时，告知总数量最大值
#define MRS_CMD_TF_DATA_NOTIFY_FIXED_LEN    4       // 特征信息告知报文数据域，固定部分长度
#define MRS_CMD_TF_DATA_SEQ_FIXED_LEN       8       // 特征信息序列中，固定部分长度
#define MRS_CMD_TF_RESULT_IND_DATA_SIZE     10      // 台区判别结果信息报文数据域长度

// 台区户变关系识别报文格式
typedef struct
{
    HI_U16 interface_ver    : 6;    // 协议版本号
    HI_U16 stru_len         : 6;    // 帧头长度
    HI_U16 dir              : 1;    // 方向位
    HI_U16 from             : 1;    // 启动位: 0从动，1启动
    HI_U16 phase            : 2;    // 相位
    HI_U16 seq;                     // 报文序号

    HI_U8 mac_addr[HI_PLC_MAC_ADDR_LEN];    // MAC地址，可以是电表地址或者主节点地址
    HI_U8 data_type;                // 特征类型，参见MRS_PLC_TF_DATA_TYPE_E
    HI_U8 option;                   // 采集类型(报文类型)，参见MRS_PLC_TF_OPTION_E

    HI_U8 data[0];                  // 数据域
} MRS_PLC_TF_CMD_S;

typedef enum
{
    MRS_PLC_TF_DATA_TYPE_VOLTAGE = 1,   // 工频电压
    MRS_PLC_TF_DATA_TYPE_FREQUENCY,     // 工频频率
    MRS_PLC_TF_DATA_TYPE_PERIOD,        // 工频周期
} MRS_PLC_TF_DATA_TYPE_E;

typedef enum
{
    MRS_PLC_TF_OPTION_COLLECT_START = 1,    // 台区特征采集启动，数据域MRS_PLC_TF_COLLECT_START_S
    MRS_PLC_TF_OPTION_DATA_COLLECT,         // 台区特征信息收集，数据域为空
    MRS_PLC_TF_OPTION_DATA_NOTIFY,          // 台区特征信息告知，数据域MRS_CMD_TF_DATA_NOTIFY_S/MRS_CMD_TF_DATA_NOTIFY_SING_EDGE_S
    MRS_PLC_TF_OPTION_RESULT_REQ,           // 台区判别结果查询，数据域为空
    MRS_PLC_TF_OPTION_RESULT_IND,           // 台区判别结果信息，数据域MRS_CMD_TF_RESULT_IND_S
} MRS_PLC_TF_OPTION_E;

// 台区特征采集启动报文数据域格式
typedef struct
{
    HI_U32 start_ntb;       // 起始NTB
    HI_U8  collect_period;  // 采集周期，单位s (采集工频周期特征不用填此字段)
    HI_U8  collect_num;     // 采集数量
    HI_U8  collect_seq;     // 采集序列号，整网第几次采集，取值范围0-255
    HI_U8  pad;
} MRS_PLC_TF_COLLECT_START_S;

typedef enum
{
    MRS_CMD_TF_EDGE_UNKNOEN,    // 保留
    MRS_CMD_TF_EDGE_FALL,       // 下降沿
    MRS_CMD_TF_EDGE_RISE,       // 上升沿
    MRS_CMD_TF_EDGE_DOUBLE,     // 双沿
} MRS_CMD_TF_COLLECT_EDGE_E;

// 工频电压/频率/周期特征序列定义
typedef struct
{
    HI_U32 base_ntb;                // 起始采集NTB

    HI_U8  pad;
    HI_U8  report_num[PHASE_CNT];   // 各相线报告数量
    HI_U16 value[0];                // 特征值
} MRS_CMD_TF_DATA_SEQ_S;

// 台区特征信息告知报文数据域格式
typedef struct
{
    HI_U16 tei          : 12;
    HI_U16 collect_edge : 2;    // 采集方式(边沿)，参见MRS_CMD_TF_COLLECT_EDGE_E，仅在工频周期时有效
    HI_U16 pad          : 2;
    HI_U8  collect_seq;         // 采集序列号
    HI_U8  total_num;           // 告知总数量

    MRS_CMD_TF_DATA_SEQ_S *data_seq1;   // 特征信息1
    MRS_CMD_TF_DATA_SEQ_S *data_seq2;   // 特征信息2，仅工频周期特征时可能存在
} MRS_CMD_TF_DATA_NOTIFY_S;

typedef struct
{
    HI_U16 tei          : 12;
    HI_U16 collect_edge : 2;    // 采集方式(边沿)，参见MRS_CMD_TF_COLLECT_EDGE_E，仅在工频周期时有效
    HI_U16 pad1         : 2;
    HI_U8  collect_seq;         // 采集序列号
    HI_U8  total_num;           // 告知总数量

    HI_U32 base_ntb;            // 起始采集NTB

    HI_U8  pad2;
    HI_U8  notify_num[PHASE_CNT];   // 各相线报告数量
    HI_U16 value[0];                // 特征值
} MRS_CMD_TF_DATA_NOTIFY_SING_EDGE_S;

typedef struct
{
    HI_U16 tei;
    HI_U8  complete_flag;   // 台区判别过程结束标志，0未结束，1结束
    HI_U8  result;          // 台区识别结果，参见MRS_TF_RESULT_TYPE_E

    HI_U8  belong_addr[HI_PLC_MAC_ADDR_LEN];    // 归属CCO的地址
    HI_U16 pad;                                 // 非协议字段
} MRS_CMD_TF_RESULT_IND_S;

typedef enum
{
    MRS_TF_RESULT_TYPE_UNKNOEN = 0, // 未知
    MRS_TF_RESULT_TYPE_CUR_NET,     // 结果为本台区
    MRS_TF_RESULT_TYPE_OTHER_NET,   // 结果为非本台区
} MRS_TF_RESULT_TYPE_E;


#define MRS_DEV_ID_SIZE                   (11) // 设备ID信息的长度

#define MRS_STA_645_READ_ID_INFO_LEN      (1)  // sta的645帧读取设备各类ID信息,返回错误信息的长度

#define MRS_CCO_STA_PLC_READ_DEV_ID_TIME  (5000) // 读取sta\I采\II采设备各类ID超时时间

//Matteo
#define MRS_CCO_CHECK_ARCHIVEE_CHANGE_TIME (1000*60)//周期检测档案变化

typedef enum
{
    MRS_DEV_TYPE_NDM = 1,          // 抄控器
    MRS_DEV_TYPE_CCO,              // CCO
    MRS_DEV_TYPE_STA,              /* 单相表模块 */
    MRS_DEV_TYPE_REP,              // 中继器 Repeater
    MRS_DEV_TYPE_CLT_II,           // II型采集器
    MRS_DEV_TYPE_CLT_I,            // I型采集器
    MRS_DEV_TYPE_THREE_STA,        /* 三相表模块 */
    MRS_DEV_TYPE_MAX,
} mrs_dev_type_e;                // 设备类型

typedef enum
{
    MRS_DEV_ID_DEFAULT,            // 0x0也表示模块ID旨在和历史扩展协议兼容
    MRS_DEV_ID_CHIP,               // 芯片ID
    MRS_DEV_ID_DEV,                // 设备ID
    MRS_DEV_ID_MAX,
} mrs_dev_id_type_e;             // 设备的ID类型

typedef enum
{
    MRS_STA_READ_ID_TYPE_ERR = 1,          // 读取的id类型不存在
    MRS_STA_READ_ID_ADDR_ERR,              // 读取的地址错误
    MRS_STA_READ_ID_FAIL_ERR,              // 读取ID信息失败
    MRS_STA_READ_ID_CREATE_FREAM_ERR,      // 创建应答帧失败
    MRS_STA_READ_ID_SEND_FREAM_ERR,        // 发送应答帧失败
} mrs_sta_645_read_id_errno_e;         // sta的645帧读取设备各类ID信息错误码

#pragma pack(1)
/* PLC查询从节点各类ID信息（下行） */
typedef struct {
    hi_u16 stru_ver : 6;    /**< 协议版本号,考虑兼容性，本版本取值固定为1 */
    hi_u16 stru_len : 6;    /**< 结构体长度 */
    hi_u16 dir_flag : 1;    /**< 方向位: 0-下行 1-上行 */
    hi_u16 id_type  : 3;    /**< ID类型: 1-芯片ID 2-模块ID 0-模块ID(兼容) */
    hi_u16 seq;             /**< 查询序列号：每次查询具有唯一序列号 */
} mrs_plc_dev_id_info_dl;

/* PLC查询从节点各类ID信息（上行） */
typedef struct {
    hi_u16 stru_ver : 6;    /**< 协议版本号,考虑兼容性，本版本取值固定为1 */
    hi_u16 stru_len : 6;    /**< 结构体长度 */
    hi_u16 dir_flag : 1;    /**< 方向位: 0-下行 1-上行 */
    hi_u16 id_type  : 3;    /**< ID类型: 1-芯片ID 2-模块ID 0-模块ID(兼容) */
    hi_u16 seq;             /**< 查询序列号：每次查询具有唯一序列号 */
	hi_u8  id_len;          /**< ID长度 */
	hi_u8  id_info[MANAGEMENT_ID_SIZE+1];   /**< 芯片ID为24字节，设备ID为11字节，以芯片ID长度定义, + 1字节设备类型 */
} mrs_plc_dev_id_info_up;
#pragma pack()
typedef struct
{
    HI_U16 interface_ver : 6;   // 协议版本号
    HI_U16 stru_len : 6;        // 结构体长度
    HI_U16 option : 4;          // 保留
    HI_U32 seq;                 // 序列号
    HI_U16 usDataLen;				//数据长度
    HI_U16 usTimeSlot;				//时隙号
    HI_U16 usMaxTei;				//最大TEI
} MRS_PLC_SET_TIME_SLOT_S;

HI_END_HEADER
#endif // __MRS_CMD_MSG_H__

