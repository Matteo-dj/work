//*****************************************************************************
//
//                  版权所有 (C), 2001-2011, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : hi_app.h
//  版 本 号   : V1.0 
//  作    者   : 沈汉坤/00130424
//  生成日期   : 2011年09月06日
//  功能描述   : app 接口定义
//  函数列表   : 无
//  修改历史   : 
//  1.日    期 : 2011年09月06日
//    作    者 : 沈汉坤/00130424
//    修改内容 : 创建文件 
//*****************************************************************************

#ifndef __HI_APP_H__
#define __HI_APP_H__

#include <hi_mdm.h>


//*****************************************************************************


//*****************************************************************************
// 消息定义
// 公用消息为 0x0 -- 0x100 
// MRS [0x100, 0x400] FW: [0x100,0x150), SRV: [0x150, 0x400]
// UPG [0x400, 0x800] FW: [0x400,0x450), SRV: [0x450, 0x600]
//
// 消息ID定义限制说明：
// [0x0000-0x0100]的消息ID预留给MRS框架内部使用, 消息ID必须小于0xFFFF.
// 
// shenhankun/2011-10-25
//*****************************************************************************

// MRS 消息 ID定义  0x150 -- 0x300

// UPG消息ID定义 0x400 -- 0x500

//*****************************************************************************


//*****************************************************************************
// PLC命令定义
// 注意: APP内部定义的命令ID必须小于 HI_DFX_ID_BASE_MAC 
// 公用部分 [0x0,0x100)
// MRS 范围 [0x100, 0x500)
// UPG范围  [0x500, 0x800)
//*****************************************************************************

// 公用的PLC命令定义 
#define ID_APP_MSG_376_2_FRAME_INPUT   0x01
#define ID_APP_MSG_645_FRAME_INPUT     0x02
#define ID_APP_MSG_MAC_FRAME_INPUT     0x03
#define ID_APP_MSG_MAC_FRAME_OUTPUT    0x10
#define ID_APP_MSG_MR_FRAME_OUTPUT     0x11
//*****************************************************************************


//*****************************************************************************
typedef HI_U32 (*APP_FW_CMD_REQ_F)(HI_U32, HI_IN HI_PVOID, HI_U16);

typedef struct
{
    HI_U32 minId;
    HI_U32 maxId;
    APP_FW_CMD_REQ_F pfnReq;    // 请求处理的函数指针
} APP_FW_CMD_PROC_TBL_S;

typedef HI_U32 (*APP_FW_MSG_PROC_F)(HI_U32, HI_IN HI_PVOID);

typedef struct
{
    HI_U32 minId;
    HI_U32 maxId;
    APP_FW_MSG_PROC_F pfnMsg;   // 消息处理的函数指针 
} APP_FW_MSG_PROC_TBL_S;

typedef struct  
{
    HI_BOOL bAvail; // HI_TRUE: PLC通道可用; HI_FALSE: PLC通道不可用
    HI_U8 reserved[3];
} APP_MSG_PLC_CHL_STATUS_S;

typedef struct
{
    HI_U32 msgId;
    APP_FW_MSG_PROC_F pfnMsg;
} APP_MSG_NODE_S;

typedef struct  
{
    HI_U16 usId;
    
    // 发送: MAC地址,当表示目的地址时:取值全为零, 则表示发送到CCO, 全为FF则表示广播,其它为指定的地址;
    // 接收: 表示源MAC地址 
    HI_U8 ucMac[HI_PLC_MAC_ADDR_LEN];
    HI_U8 reserved[2]; // padding only
    HI_U16 usPayloadLen; // 帧长度
    HI_PBYTE pucPayload;// 完整帧
}APP_PLC_FRAME_DATA_S;


//数据转发结构体 
typedef struct  
{
    HI_U8 ucMac[HI_PLC_MAC_ADDR_LEN];  // 数据发送方的MAC地址, @@ 可以去掉 
    HI_U16 usDataSizse; // 转发数据长度
    HI_U8 pucData[0];  // 转发数据缓冲区指针
} APP_PLC_CMD_DATA_TRANSMIT_S;

typedef struct
{
    HI_U32 cmdId;
    APP_FW_CMD_REQ_F pfnReq;  // 当前请求处理的函数指针，仅支持一个命令，为了简化处理，不支持多个命令并行支持 
    APP_PLC_FRAME_DATA_S stReq; // 当前的REQ数据
} APP_CMD_NODE_S;
//*****************************************************************************


//*****************************************************************************
// 广东电网宽带载波互联互通
//*****************************************************************************

//*****************************************************************************
// 控制域
//*****************************************************************************

// 帧类型域
typedef enum
{
    APP_CSG_PLC_CTRL_TYPE_ACK = 0x0,    // 确认/否认
    APP_CSG_PLC_CTRL_TYPE_TRANS,        // 数据转发
    APP_CSG_PLC_CTRL_TYPE_CMD,          // 命令
    APP_CSG_PLC_CTRL_TYPE_REPORT,       // 主动上报
    APP_CSG_PLC_CTRL_TYPE_TEST = 0xE,   // 测试命令
} APP_CSG_PLC_CTRL_TYPE_E;


// 业务扩展域标志位
typedef enum
{
    APP_CSG_PLC_CTRL_EXTEND_BIT_N = 0x0,// 无业务扩展域
    APP_CSG_PLC_CTRL_EXTEND_BIT_Y       // 有业务扩展域
} APP_CSG_PLC_CTRL_EXTEND_BIT_E;


// 响应标志位
typedef enum
{
    APP_CSG_PLC_CTRL_RESP_BIT_N = 0x0,  // 无需响应
    APP_CSG_PLC_CTRL_RESP_BIT_Y         // 需要响应
} APP_CSG_PLC_CTRL_RESP_BIT_E;


// 启动标志位
typedef enum
{
    APP_CSG_PLC_CTRL_PRM_SLAVE = 0x0,   // 从动站
    APP_CSG_PLC_CTRL_PRM_HOST           // 主动站
} APP_CSG_PLC_CTRL_PRM_E;


// 传输方向位
typedef enum
{
    APP_CSG_PLC_CTRL_DIR_DL = 0x0,      // CCO-->STA
    APP_CSG_PLC_CTRL_DIR_UL             // STA-->CCO
} APP_CSG_PLC_CTRL_DIR_E;


// 协议版本号
typedef enum
{
    APP_CSG_PLC_PROTO_VER1 = 0x1,
} APP_CSG_PLC_PROTO_VER_E;

#define APP_CSG_PLC_PROTO_VERSION   (APP_CSG_PLC_PROTO_VER1)


//*****************************************************************************
// 业务ID
//*****************************************************************************

// 确认/否认
typedef enum
{
    APP_CSG_PLC_SRV_ID_ACK_CONFIRM = 0x00,  // 确认
    APP_CSG_PLC_SRV_ID_ACK_DENY,            // 否认
} APP_CSG_PLC_SRV_ID_ACK_E;


// 数据转发
typedef enum
{
    APP_CSG_PLC_SRV_ID_TRANS_XR = 0x00,     // 数据透传
    APP_CSG_PLC_SRV_ID_PLC_TEST = 0xF0,     // PLC灵敏度测试
} APP_CSG_PLC_SRV_ID_TRANS_E;


// 命令
typedef enum
{
    APP_CSG_PLC_SRV_ID_CMD_SM_RESULT = 0x00,// 查询搜表结果
    APP_CSG_PLC_SRV_ID_CMD_DL_METERLIST,    // 下发搜表列表
    APP_CSG_PLC_SRV_ID_CMD_FILE,            // 文件传输
    APP_CSG_PLC_SRV_ID_CMD_EVENT_FLAG,      // 允许/禁止从节点主动上报
    APP_CSG_PLC_SRV_ID_CMD_REBOOT,          // 从节点重启
    APP_CSG_PLC_SRV_ID_CMD_UPG_INFO,        // 查询升级信息
    APP_CSG_PLC_SRV_ID_CMD_BIND = 0x72,     // 锁定命令
    APP_CSG_PLC_SRV_ID_CMD_TEST = 0xF0,     // 测试帧
} APP_CSG_PLC_SRV_ID_CMD_E;


// 主动上报
typedef enum
{
    APP_CSG_PLC_SRV_ID_REPORT_EVENT = 0x00, // 事件主动上报
} APP_CSG_PLC_SRV_ID_REPORT_E;


//*****************************************************************************
// PLC帧头
//*****************************************************************************

#define ID_APP_CMD_CSG_DATA_TRANSMIT_PLC    (0x0101)

typedef struct
{
    // 控制域
    HI_U16 usFrameType : 4; // 帧类型域
    HI_U16 reserved    : 8;
    HI_U16 usExtendBit : 1; // 业务扩展域标识位 0-无业务扩展域 1-有业务扩展域
    HI_U16 usRespBit   : 1; // 响应标识位 0-不需要应答 1-需要应答
    HI_U16 usPrm       : 1; // 启动标志位 0-此帧报文来自从动站 1-此帧报文来自启动站
    HI_U16 usDir       : 1; // 传输方向位 0-下行方向 1-上行方向

    // 业务标识
    HI_U8  ucSrvId;

    // 应用版本号
    HI_U8  ucVersion;

    // 帧序号
    HI_U16 usSeq;

    // 帧长
    HI_U16 usDataLength;

    HI_U8 aucData[0];
} APP_CSG_PLC_FRAME_HEAD;


// 数据转发：数据透传下行帧
typedef struct
{
    HI_U8 aucSrcAddr[HI_METER_ADDR_LEN];    // 源地址
    HI_U8 aucDstAddr[HI_METER_ADDR_LEN];    // 目的地址
    HI_U8 ucTimeout;        // 设备超时时间
    HI_U8 reserved;
    HI_U16 usDataLen;       // 转发数据长度
    HI_U8 aucData[0];       // 转发数据内容
} APP_CSG_TRANS_XR_DL_STRU;

// PLC探测报文
typedef struct
{
    HI_U16 usDataLen;       // 转发数据长度
    HI_U16 usExpLen;       // 期望接收报文
    HI_U8 aucData[0];       // 转发数据内容
} APP_CSG_PLC_TEST_STRU;


// 数据转发：数据透传上行帧
typedef struct
{
    HI_U8 aucSrcAddr[HI_METER_ADDR_LEN];    // 源地址
    HI_U8 aucDstAddr[HI_METER_ADDR_LEN];    // 目的地址
    HI_U8 reserved[2];
    HI_U16 usDataLen;       // 转发数据长度
    HI_U8 aucData[0];       // 转发数据内容
} APP_CSG_TRANS_XR_UL_STRU;


// 命令业务: 查询搜表结果上行帧
typedef struct
{
    HI_U8 aucMeterAddr[HI_METER_ADDR_LEN];
    HI_U8 reserved[2];
} APP_CSG_METER_INFO;


typedef struct
{
    HI_U8 ucMeterNum;
    HI_U8 reserved[3];
    APP_CSG_METER_INFO astMeterInf[0];
} APP_CSG_CMD_SM_RESULT_UL_STRU;


// 命令: 下发搜表列表
#define APP_CSG_CMD_DL_METERLIST_MAX    (16)
typedef APP_CSG_CMD_SM_RESULT_UL_STRU   APP_CSG_CMD_DL_METERLIST_STRU;


typedef enum
{
    APP_CSG_ID_FILE_INFO = 0x00,    // 下发文件信息
    APP_CSG_ID_FILE_DATA,           // 下发文件数据
    APP_CSG_ID_FILE_QUERY,          // 查询文件数据包接收状态
    APP_CSG_ID_FILE_FINISH,         // 文件传输完成通知
    APP_CSG_ID_FILE_U2B,            // 文件传输报文单播转本地广播
} APP_CSG_ID_FILE_E;


typedef enum
{
    APP_CSG_PLC_COMM_TIMEOUT = 0x00,
    APP_CSG_PLC_NOT_SUPPORT_SRV_ID,
    APP_CSG_PLC_CCO_BUSY,
    APP_CSG_PLC_METER_NO_RESPONSE,
    APP_CSG_PLC_FORMAT_ERROR,
    APP_CSG_PLC_DENY_OTHER = 0xFF,
} APP_CSG_PLC_DENY_E;


#define APP_CSG_FILE_FLAG_CLEAR     (0x00)
#define APP_CSG_FILE_FLAG_STA       (0x02)
#define APP_CSG_FILE_FLAG_CLTI      (0x03)


// 文件传输信息头结构
typedef struct
{
    HI_U8 ucInfoID;         // 文件传输信息ID
    HI_U8 reserved[3];
    HI_U8 aucInfo[0];       // 文件传输信息
} APP_CSG_CMD_FILE_HEAD;

// 文件传输
typedef struct
{
    HI_U8 ucFileFlag;       // 文件性质
    HI_U8 reserved;
    HI_U8 aucDstAddr[HI_METER_ADDR_LEN];
    HI_U32 ulFileCrc;       // 文件总校验CRC32
    HI_U32 ulFileLength;    // 文件大小
    HI_U16 usTotalSegment;  // 文件总段数n
    HI_U16 usTimeWindow;    // 文件传输时间窗
    HI_U32 ulFileID;        // 文件传输ID
} APP_CSG_CMD_FILE_INFO_DL_STRU;

typedef struct
{
    HI_U8 ucLength;         // 扩展域数据区长度: 用于标识厂家编码和扩展数据区长度
    HI_CHAR szCode[2];      // 厂家编码部分占2字节，格式为ASCII码
    HI_U8 ucUpgOption;      // 升级选项: [7:7]1-强制升级
    HI_U16 usForceBsn;      // 强制升级序列号
    HI_U16 reserved;
} APP_CSG_CMD_FILE_INFO_EXTEND_STRU;

typedef struct
{
    HI_U32 ulFileID;        // 文件传输ID
    HI_U16 usRet;           // 结果码
    HI_U16 usErrCode;       // 错误代码
} APP_CSG_CMD_FILE_INFO_UL_STRU;

#pragma pack(2)
// 文件数据(下行)
typedef struct
{
    HI_U16 usSegmentSn;     // 文件段号
    HI_U16 usTotalSegment;  // 文件总段数n
    HI_U32 ulFileID;        // 文件传输ID
    HI_U16 usDataLength;    // 文件段长度L
    HI_U8 aucData[0];       // 文件段内容
} APP_CSG_CMD_FILE_DATA_DL_STRU;
#pragma pack()

// 文件数据(上行)
typedef struct
{
    HI_U32 ulFileID;        // 文件传输ID
    HI_U32 ulRet;           // 结果码
} APP_CSG_CMD_FILE_DATA_UL_STRU;

// 文件包接收状态(下行)
typedef struct
{
    HI_U32 ulFileID;
    HI_U16 usSegmentStart;
    HI_U16 usSegmentCount;
} APP_CSG_CMD_FILE_STATUS_DL_STRU;

// 文件包接收状态(上行)
typedef struct
{
    HI_U32 ulFileID;
    HI_U16 usSegmentStart;
    HI_U8 ucFileRcvStatus;
    HI_U8 reserved;
    HI_U8 aucBitmap[0];
} APP_CSG_CMD_FILE_STATUS_UL_STRU;

// 文件传输完成通知(下行)
#pragma pack(2)
typedef struct
{
    HI_U32 ulFileID;
    HI_U16 usDelayResetTime;
} APP_CSG_CMD_FILE_FINISH_DL_STRU;
#pragma pack()

// 文件传输完成通知(上行)
typedef APP_CSG_CMD_FILE_DATA_UL_STRU   APP_CSG_CMD_FILE_FINISH_UL_STRU;
typedef enum
{
    APP_CSG_MODE_PHYSICAL_LOOPBACK = 0,     //回环测试
    APP_CSG_MODE_PHYSICAL_TRANSIMIT,        //透明转发测试
    APP_CSG_MODE_PHYSICAL_MAX,
} APP_CSG_PHYSICAL_TEST_MODE;        // 物理层测试模式
#define APP_CSG_PLC_TEST_MODE               (0)
typedef struct
{
    HI_U8 ucId; //测试ID  0-进入回环测试模式
    HI_U8 ucReserved; //预留
    HI_U16 usPayloadLen;    //数据区长度
    HI_U8 aucPayload[0]; //数据区
} APP_CSG_CMD_TEST_MODE_STRU;//宽带互联互通 测试帧结构
//*****************************************************************************
//*****************************************************************************



//*****************************************************************************
#endif // __APP_COMMON_H__

