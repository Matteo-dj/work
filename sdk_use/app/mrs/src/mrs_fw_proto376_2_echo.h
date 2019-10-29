//*****************************************************************************
//
//                  版权所有 (C), 1998-2011, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_fw_proto376_2_echo.h
//  版 本 号   : V1.0
//  作    者   : niesongsong/kf62735
//  生成日期   : 2011-12-14
//  功能描述   : TODO: ...
//
//  函数列表   : TODO: ...
//  修改历史   :
//  1.日    期 : 2011-12-14
//    作    者 : niesongsong/kf62735
//    修改内容 : 创建文件
//
//*****************************************************************************
#ifndef __MRS_FW_PROTO376_2_ECHO_H__
#define __MRS_FW_PROTO376_2_ECHO_H__

HI_START_HEADER

#define MRS_3762_F10_NODE_MAX_NUM   (32)
#define MRS_3762_ADD_METERS_MAX_NUM  (32)

/* 否认帧: 错误状态字 国网规范(376.2-2009版) */
#define MRS_3762_DENY_STATUS_COMMUNICATION_OVERTIME     (0)     // 通信超时
#define MRS_3762_DENY_STATUS_INVALID_DATA               (1)     // 无效数据单元
#define MRS_3762_DENY_STATUS_ERROR_LENGTH               (2)     // 长度错
#define MRS_3762_DENY_STATUS_ERROR_CHECK_SUM            (3)     // 校验错误
#define MRS_3762_DENY_STATUS_INEXISTENT_INFO            (4)     // 信息类不存在
#define MRS_3762_DENY_STATUS_ERROR_FORMAT               (5)     // 格式错误
#define MRS_3762_DENY_STATUS_REPETITION_METER           (6)     // 表号重复: 表号-电表通信地址
#define MRS_3762_DENY_STATUS_INEXISTENT_METER           (7)     // 表号不存在
#define MRS_3762_DENY_RM_BUFPOOL_FULL                   (9)     // 读表模块缓存池满
#define MRS_3762_DENY_RM_645ID_OVERRUN                  (10)    // 最大数据项目数超限(645帧数量)
#define MRS_3762_DENY_RM_METER_BUSY                     (11)    // 该表正在抄读中，自定义
#define MRS_3762_DENY_CCO_BUSY                          (12)    // CCO模块忙、不允许抄表

/* 否认帧: 错误状态字 国网规范(1376.2-2013版) */
#define MRS_13762_DENY_STATUS_COMMUNICATION_OVERTIME    (MRS_3762_DENY_STATUS_COMMUNICATION_OVERTIME)
#define MRS_13762_DENY_STATUS_INVALID_DATA              (MRS_3762_DENY_STATUS_INVALID_DATA)
#define MRS_13762_DENY_STATUS_ERROR_LENGTH              (MRS_3762_DENY_STATUS_ERROR_LENGTH)
#define MRS_13762_DENY_STATUS_ERROR_CHECK_SUM           (MRS_3762_DENY_STATUS_ERROR_CHECK_SUM)
#define MRS_13762_DENY_STATUS_INEXISTENT_INFO           (MRS_3762_DENY_STATUS_INEXISTENT_INFO)
#define MRS_13762_DENY_STATUS_ERROR_FORMAT              (MRS_3762_DENY_STATUS_ERROR_FORMAT)
#define MRS_13762_DENY_STATUS_REPETITION_METER          (MRS_3762_DENY_STATUS_REPETITION_METER)
#define MRS_13762_DENY_STATUS_INEXISTENT_METER          (MRS_3762_DENY_STATUS_INEXISTENT_METER)
#define MRS_13762_DENY_STATUS_METER_APP_NOT_REPLY       (8)     // 电表应用层无应答: 指通信结点正常通信或中继，但被抄的电表645协议无应答
#define MRS_13762_DENY_STATUS_CCO_BUSY                  (9)     // 主节点忙: 表示主节点正在执行上一条命令，暂时无法响应当前命令
#define MRS_13762_DENY_STATUS_CCO_NOT_SUPPORT           (10)    // 主节点不支持此命令: 主节点对当前命令无法支持
#define MRS_13762_DENY_STATUS_STA_NOT_REPLY             (11)    // 从节点无应答: 主节点未收到从节点的应答命令
#define MRS_13762_DENY_STATUS_STA_NOT_JOIN_NETWORK      (12)    // 从节点不在网内: 从节点尚未加入主节点的网络
#define MRS_13762_DENY_RM_BUFPOOL_FULL                  (109)   // 读表模块缓存池满
#define MRS_13762_DENY_RM_645ID_OVERRUN                 (110)   // 最大数据项目数超限(645帧数量)
#define MRS_13762_DENY_RM_METER_BUSY                    (111)   // 该表正在抄读中，自定义

/* 周期抄表模式 */
typedef enum
{
    PERIOD_METER_READ_MODE_BOTH = 0,
    PERIOD_METER_READ_MODE_MASTER,
    PERIOD_METER_READ_MODE_CCO,
} PERIOD_METER_READ_MODE_ENUM;

#define MRS_AFN_FRAME_DELAY_UNRELATED   (0x00)
#define MRS_AFN_FRAME_DELAY_RELATED     (0x01)

// 当前报文本地通信上行时长，固定值1秒
#define MRS_AFN_COMM_DELAY              (1)

#define MRS_AFN(AFN)                    (HI_U8)(AFN)
/* BEGIN: Added by fengxiaomin/00209182, 2014/1/10   问题单号号:DTS2014012607684 */
#define MRS_CCO_PROTOCOL_AFN_13        (0x13)
#define MRS_CCO_PROTOCOL_AFN_06        (0x06)
#define MRS_CCO_PROTOCOL_AFN_14        (0x14)

/* BEGIN: Modified by fengxiaomin/00209182, 2014/3/27   问题单号:DTS2014032708427 */
#define MRS_CCO_QUERY_NODE_NUM_MAX     (64)
#define MRS_CCO_10_F101_DATA_FIX_LEN   (3)
#define MRS_CCO_10_F101_VER_INFO_LEN   (11)
#define MRS_CHAR_TO_INT(p)             (HI_U8)((p) - '0')
#define MRS_CCO_STA_NODE_INFORMATION   (0x1000)

#define MRS_CCO_10_F101_REPORT_NUM_OFFSET  (2)
#define MRS_CCO_10_F101_ADDR_OFFSET       (3)
#define MRS_CCO_10_F101_NODER_INFO_OFFSET   (9)
#define MRS_CCO_10_F101_BOOT_VER_OFFSET   (13)
#define MRS_CCO_10_F101_SOFTWARE_VER_OFFSET   (11)

#define MRS_CCO_10_F104_NODE_QUERY_MAX  (15)
#define MRS_CCO_10_F104_NODE_SIZE       (15)
#define MRS_CCO_10_F104_HEAD_LEN        (3)

#define MRS_CCO_10_F21_DATA_FIX_LEN   (5)
#define MRS_CCO_10_F21_TOPO_INFO_LEN  (11)
#define MRS_CCO_START_SEQ_ONE         (1)

#define MRS_CCO_10_F31_DATA_FIX_LEN     (5)
#define MRS_CCO_10_F31_PHASE_INFO_LEN   (8)
#define MRS_CCO_10_F31_PN_RESULT_OFFSET (4)
#define MRS_CCO_10_F31_METER_TYPE_OFFSET (3)
#define MRS_CCO_10_F31_T_PHASE_TYPE_LEN  (3)
#define MRS_CCO_10_F31_T_PHASE_TYPE_OFFSET (5)


#define MRS_CCO_10_F32_DATA_FIX_LEN      (5)
#define MRS_CCO_10_F32_CHANNEL_INFO_LEN  (8)
#define MRS_CCO_10_F32_CCO_SUCC_RATE     (100)

#define MRS_CCO_10_F33_DATA_FIX_LEN      (5)
#define MRS_CCO_10_F33_ID_INFO_LEN       (30)

#define MRS_CCO_10_F111_DATA_FIX_LEN   (10)
#define MRS_CCO_10_F111_NID_INFO_LEN   (3)

#define MRS_CCO_STA_CHIP_ID_LEN        (24)

#define MRS_WORK_FREQ_0                (0)
#define MRS_WORK_FREQ_1                (1)

#define MRS_CCO_10_F112_DATA_FIX_LEN   (5)
#define MRS_CCO_10_F112_CHIP_INFO_LEN  (33)

#define MRS_ND_TOPO_ROLE_INVALID  (0)
#define MRS_ND_TOPO_ROLE_STA      (1)
#define MRS_ND_TOPO_ROLE_PROXY    (2)
#define MRS_ND_TOPO_ROLE_CCO      (4)
#define MRS_CCO_TOPO_ROLE_OFFSET  (4)
#define MRS_CCO_TOPO_START_SEQ_OFFSET   (2)
#define MRS_CCO_TOPO_REPORT_NUM_OFFSET  (4)

#define MRS_ND_TOPO_NODE_TYPE_INVALID  (0xFF)
#define MRS_ND_TOPO_NODE_TYPE_CCO      (0)
#define MRS_ND_TOPO_NODE_TYPE_STA      (1)
#define MRS_ND_TOPO_NODE_TYPE_CLTI     (2)
#define MRS_ND_TOPO_NODE_TYPE_CLTII    (3)

#define MRS_CCO_QUERY_TOPO_10_F21      (0)
#define MRS_CCO_QUERY_TOPO_10_F228     (1)
#define MRS_CCO_QUERY_TOPO_10_F31      (2)
#define MRS_CCO_QUERY_TOPO_10_F32      (3)
#define MRS_CCO_QUERY_TOPO_10_F33      (4)

#define MRS_CCO_10_F228_DATA_FIX_LEN   (5)
#define MRS_CCO_10_F228_TOPO_INFO_LEN  (18)

#define MRS_CCO_QUERY_TOPO_INFO        (1)
#define MRS_CCO_QUERY_STA_ID_INFO      (2)
#define MRS_CCO_QUERY_TOPO_STA_ID_INFO (MRS_CCO_QUERY_TOPO_INFO | MRS_CCO_QUERY_STA_ID_INFO)
#define MRS_CCO_NETWORK_NID_LEN        (3)

/* END:   Modified by fengxiaomin/00209182, 2014/3/27 */
/* END:   Added by fengxiaomin/00209182, 2014/1/10 */
#define MRS_AFN_FN_LO(Fn) /*lint -save -e778*/  (HI_U8)(1 << (((Fn) - 1) % 8))  /*lint -restore*/
#define MRS_AFN_FN_HI(Fn) /*lint -save -e778*/  (HI_U8)(((Fn) - 1) / 8)         /*lint -restore*/

#define MRS_MAX_RELAY_LEVEL            (15)

#define MRS_PHASE_ALL                  (7)
#define MRS_PHASE_A                    (1)
#define MRS_PHASE_B                    (2)
#define MRS_PHASE_C                    (4)
#define MRS_PHASE_INVALID              (0)
#define MRS_PHASE_MASK                 (7)

#define MRS_MAIN_NODE_ADDR_LEN         (6)

//645报文最大长度上限
#define MRS_AFNF1_F1_645_MAX_LEN       (2000)

//AFN_FN处理函数
typedef HI_U32 (*MRS_3762_AFN_FUN)(HI_VOID * param);

typedef struct
{
    HI_U8 ucCmdState    :1;    // 命令状态 0-未处理 1-已处理
    HI_U8 ucCH_1State   :1;    // 信道1状态 0-忙 1-闲
    HI_U8 ucCH_2State   :1;    // 信道2状态 0-忙 1-闲
    HI_U8 ucCH_3State   :1;    // 信道3状态 0-忙 1-闲
    HI_U8 ucCH_4State   :1;    // 信道4状态 0-忙 1-闲
    HI_U8 ucCH_5State   :1;    // 信道5状态 0-忙 1-闲
    HI_U8 ucCH_6State   :1;    // 信道6状态 0-忙 1-闲
    HI_U8 ucCH_7State   :1;    // 信道7状态 0-忙 1-闲

    HI_U8 ucCH_8State   :1;    // 信道8状态 0-忙 1-闲
    HI_U8 ucCH_9State   :1;    // 信道9状态 0-忙 1-闲
    HI_U8 ucCH_10State  :1;    // 信道10状态 0-忙 1-闲
    HI_U8 ucCH_11State  :1;    // 信道11状态 0-忙 1-闲
    HI_U8 ucCH_12State  :1;    // 信道12状态 0-忙 1-闲
    HI_U8 ucCH_13State  :1;    // 信道13状态 0-忙 1-闲
    HI_U8 ucCH_14State  :1;    // 信道14状态 0-忙 1-闲
    HI_U8 ucCH_15State  :1;    // 信道15状态 0-忙 1-闲

    HI_U16 uSWitTime;          // 等待时间
}AFN_00_F1; // 确认

typedef struct
{
    HI_U8 ucCmdState    :1;    // 命令状态 0-未处理 1-已处理
    HI_U8 ucCH_1State   :1;    // 信道1状态 0-忙 1-闲
    HI_U8 ucCH_2State   :1;    // 信道2状态 0-忙 1-闲
    HI_U8 ucCH_3State   :1;    // 信道3状态 0-忙 1-闲
    HI_U8 ucCH_4State   :1;    // 信道4状态 0-忙 1-闲
    HI_U8 ucCH_5State   :1;    // 信道5状态 0-忙 1-闲
    HI_U8 ucCH_6State   :1;    // 信道6状态 0-忙 1-闲
    HI_U8 ucCH_7State   :1;    // 信道7状态 0-忙 1-闲

    HI_U8 ucCH_8State   :1;    // 信道8状态 0-忙 1-闲
    HI_U8 ucCH_9State   :1;    // 信道9状态 0-忙 1-闲
    HI_U8 ucCH_10State  :1;    // 信道10状态 0-忙 1-闲
    HI_U8 ucCH_11State  :1;    // 信道11状态 0-忙 1-闲
    HI_U8 ucCH_12State  :1;    // 信道12状态 0-忙 1-闲
    HI_U8 ucCH_13State  :1;    // 信道13状态 0-忙 1-闲
    HI_U8 ucCH_14State  :1;    // 信道14状态 0-忙 1-闲
    HI_U8 ucCH_15State  :1;    // 信道15状态 0-忙 1-闲

    HI_U8 ucCH_16State  :1;    // 信道16状态 0-忙 1-闲
    HI_U8 ucCH_17State  :1;    // 信道17状态 0-忙 1-闲
    HI_U8 ucCH_18State  :1;    // 信道18状态 0-忙 1-闲
    HI_U8 ucCH_19State  :1;    // 信道19状态 0-忙 1-闲
    HI_U8 ucCH_20State  :1;    // 信道20状态 0-忙 1-闲
    HI_U8 ucCH_21State  :1;    // 信道21状态 0-忙 1-闲
    HI_U8 ucCH_22State  :1;    // 信道22状态 0-忙 1-闲
    HI_U8 ucCH_23State  :1;    // 信道23状态 0-忙 1-闲

    HI_U8 ucCH_24State  :1;    // 信道24状态 0-忙 1-闲
    HI_U8 ucCH_25State  :1;    // 信道25状态 0-忙 1-闲
    HI_U8 ucCH_26State  :1;    // 信道26状态 0-忙 1-闲
    HI_U8 ucCH_27State  :1;    // 信道27状态 0-忙 1-闲
    HI_U8 ucCH_28State  :1;    // 信道28状态 0-忙 1-闲
    HI_U8 ucCH_29State  :1;    // 信道29状态 0-忙 1-闲
    HI_U8 ucCH_30State  :1;    // 信道30状态 0-忙 1-闲
    HI_U8 ucCH_31State  :1;    // 信道31状态 0-忙 1-闲

    HI_U16 uSWitTime;          // 等待时间
}AFN_00_F1_NEW; // 确认

// 数据单元信息AFN-00_F2
typedef HI_U8 AFN_00_F2;

typedef struct
{
    HI_U8 ucReadFlg;
    HI_U8 ucDataLen;         // 报文长度
    HI_U8 ucAttNodeNum;      // 载波从节点附属节点个数
    HI_U8 reserved;

    HI_U8 *pucData;          // 报文内容
    HI_U8 *pucAttNodeAdd;    // 载波从节点附属节点地址
}AFN_14_F1_DL; // 路由请求抄读内容-下行

typedef struct
{
    MRS_3762_FRAME_STRU  * pstFrame;
    HI_PBYTE             * ppucOutBuffer;// 输出缓存
    HI_U16               * pusBufferLen; // 输出长度
    HI_PBYTE               pContent;     //3762帧中的应用数据域
    HI_U16                 usContentLen; //3762帧中的应用数据域长度
    HI_U8                  ucAfn;        // AFN
    HI_U8                  ucFnIndex;    // FN
    HI_BOOL                ucNdmPlc;     // 所传信息是ndm的plc帧数据
    HI_U8                  reserved[3];  // 保留
}AFN_FN_UL_PARAM; // AFN_FN上行通用传数之间参数约定

// 数据单元信息AFN-03_F1_UP
typedef struct
{
    HI_S8    CoCode[2];      // 厂商代码(ASCII)
    HI_S8    ChipCode[2];    // 芯片代码(ASCII)
    HI_U8    VerDay;         // 版本日期 - 日
    HI_U8    VerMonth;       // 版本日期 - 月
    HI_U8    VerYear;        // 版本日期 - 年
    HI_U8    reserved[1];    // ...
    HI_U16   Ver;            // 版本号
    HI_U8    reserved2[2];    // ...
}AFN_03_F1_UP; // 厂商代码和版本信息-上行

typedef struct
{
    AFN_14_F1_DL * pstAFN_14_F1_DL;     //数据指针
    HI_VOID (*MRS_AFN14_FREE_FUN)(HI_VOID *);   //释放内存函数
}AFN_14_F1_DL_PARAM; // AFN_14_F4下行，传参约定

typedef struct
{
    HI_U8 ucNodeAddr[6];
    HI_U8 ucRelayLevel : 4;
    HI_U8 ucQuality    : 4;
    HI_U8 ucPhase      : 3;
    HI_U8 ucProtocol   : 3;
    HI_U8 ucBak        : 2;
}AFN_10_F2_NODE_INFO;

#pragma pack(1)
typedef struct
{
    HI_U8 dev_type;
    HI_U8 node_addr[HI_PLC_MAC_ADDR_LEN];
	HI_U8 id_type;
}afn_10_f40_data_param; // AFN_10_F40接收到的数据单元结构

typedef afn_10_f40_data_param afn_10_f40_dl_data_param;

typedef struct
{
    afn_10_f40_data_param ul_param;
	HI_U8 id_len;
	HI_U8 id_info[MANAGEMENT_ID_SIZE]; // 芯片ID为24字节，设备ID为11字节，以芯片ID长度定义
}afn_10_f40_ul_data_param; // AFN_10_F40上行数据单元结构
#pragma pack()

//*****************************************************************************
// 函数名称: mrs3762ProtoAckFrame
// 功能描述: 构造确认帧
//
// 参数说明:
//   HI_PBYTE pucBuffer [OUT]
//
//                          ...
//
// 返 回 值:
//    TODO: ...
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: niesongsong/kf62735 [2011-12-13]
//*****************************************************************************
HI_U32 mrs3762ProtoAckFrame(HI_IN HI_U16 waitTime, HI_IN AFN_FN_UL_PARAM * pstDlFrameInf,HI_OUT HI_PBYTE * pucBuffer,HI_OUT HI_U16 * pusBufferLen);

//*****************************************************************************
// 函数名称: mrs3762ProtoDenyFrame
// 功能描述: 构造确认帧
//
// 参数说明:
//   HI_PBYTE pucBuffer [in] ...
//                          ...
//
// 返 回 值:
//    TODO: ...
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: niesongsong/kf62735 [2011-12-13]
//*****************************************************************************
HI_U32 mrs3762ProtoDenyFrame(HI_IN AFN_00_F2 * pstAckFrame,HI_IN AFN_FN_UL_PARAM * pstDlFrameInf,HI_OUT HI_PBYTE * pucBuffer,HI_OUT HI_U16 * pusBufferLen);
//*****************************************************************************
// 函数名称: mrs3762AfnDispatch
// 功能描述: 处理AFN应用层数据
//
// 参数说明:
//   ucAfn [in] ...   AFN码
//              ...
//   pstFrame [in/out] 帧
//                     ...
//
// 返 回 值:
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: niesongsong/kf62735 [2011-12-13]
//*****************************************************************************
HI_U32 mrs3762AfnDispatch(HI_IN MRS_3762_FRAME_STRU * pstFrame,HI_OUT HI_PBYTE * ppOutBuffer,
	HI_U16 * pusBufferLen,HI_BOOL ucNdmPlc);

//*****************************************************************************
// 函数名称: mrs3762FindAfnProc
// 功能描述: 根据AFN和FN找到相应的FN处理函数
//
// 参数说明:
//   afn [in] AFN，应用程序功能码
//
//   fnIndex [in] fn索引
//
//
// 返 回 值:
//    成功返回该处理的函数指针
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: niesongsong/kf62735 [2011-12-23]
//*****************************************************************************
MRS_3762_AFN_FUN mrs3762FindAfnProc(HI_U8 afn,HI_U8 fnIndex);

//AFN FN 系列处理函数,目前还没实现
HI_U32 mrs3762_AFNXX_XX(HI_INOUT HI_VOID * param);

//AFN 00H
HI_U32 mrs3762_AFN00_F1(HI_INOUT HI_VOID * param);
HI_U32 mrs3762_AFN00_F2(HI_INOUT HI_VOID * param);

//AFN 01H
HI_U32 mrs3762_AFN01_F1(HI_INOUT HI_VOID * param);
HI_U32 mrs3762_AFN01_F2(HI_INOUT HI_VOID * param);
HI_U32 mrs3762_AFN01_F3(HI_INOUT HI_VOID * param);

/* 问题单号: DTS2014010608635 BEGIN: Added by liuxipeng/lkf54842, 2014/01/07 */
//AFN 02H
HI_U32 mrs3762_AFN02_F1(HI_INOUT HI_VOID * param);

//AFN 03H
HI_U32 mrs3762_AFN03_F1(HI_INOUT HI_VOID * param);
HI_U32 mrs3762_AFN03_F2(HI_INOUT HI_VOID * param);
HI_U32 mrs3762_AFN03_F3(HI_INOUT HI_VOID * param);
HI_U32 mrs3762_AFN03_F4(HI_INOUT HI_VOID * param);
HI_U32 mrs3762_AFN03_F5(HI_INOUT HI_VOID * param);
HI_U32 mrs3762_AFN03_F6(HI_INOUT HI_VOID * param);
HI_U32 mrs3762_AFN03_F7(HI_INOUT HI_VOID * param);
HI_U32 mrs3762_AFN03_F9(HI_INOUT HI_VOID * param);
HI_U32 mrs3762_AFN03_F10(HI_INOUT HI_VOID * param);
HI_U32 mrs3762_AFN03_F11(HI_INOUT HI_VOID * param);
HI_U32 mrs3762_AFN03_F16(HI_INOUT HI_VOID * param);

//AFN 04H
HI_U32 mrs3762_AFN04_F1(HI_INOUT HI_VOID * param);
HI_U32 mrs3762_AFN04_F2(HI_INOUT HI_VOID * param);
HI_U32 mrs3762_AFN04_F3(HI_INOUT HI_VOID * param);

HI_U32 mrs3762_AFN04_F4(HI_INOUT HI_VOID * param);
HI_U32 mrs3762_AFN04_F5(HI_INOUT HI_VOID * param);
//AFN 05H
HI_U32 mrs3762_AFN05_F1(HI_INOUT HI_VOID * param);
HI_U32 mrs3762_AFN05_F2(HI_INOUT HI_VOID * param);
HI_U32 mrs3762_AFN05_F3(HI_INOUT HI_VOID * param);
HI_U32 mrs3762_AFN05_F4(HI_INOUT HI_VOID * param);
HI_U32 mrs3762_AFN05_F5(HI_INOUT HI_VOID * param);
HI_U32 mrs3762_AFN05_F6(HI_INOUT HI_VOID * param);
HI_U32 mrs3762_AFN05_F16(HI_INOUT HI_VOID * param);
HI_U32 mrs3762_AFN05_F101(HI_INOUT HI_VOID * param);
HI_U32 mrs3762_AFN05_F130(HI_INOUT HI_VOID * param);

//AFN 10H
HI_U32 mrs3762_AFN10_F1(HI_INOUT HI_VOID * param);
HI_U32 mrs3762_AFN10_F2(HI_INOUT HI_VOID * param);
HI_U32 mrs3762_AFN10_F3(HI_INOUT HI_VOID * param);
HI_U32 mrs3762_AFN10_F4(HI_INOUT HI_VOID * param);
HI_U32 mrs3762_AFN10_F5(HI_INOUT HI_VOID * param);
HI_U32 mrs3762_AFN10_F6(HI_INOUT HI_VOID * param);
HI_U32 mrs3762_AFN10_F21(HI_INOUT HI_VOID * param);
HI_U32 mrsQueryTopoDownCheck(AFN_FN_UL_PARAM *pstAfnParam, HI_U16 *pusStartSeq, HI_U8 *pucQueryNum, HI_U8 ucFrom);
HI_U32 mrsSetTopoQueryBuf(HI_U8* pData, HI_U8 ucQueryNum, HI_U16 usStartSeq, HI_U8* pucReportNum, HI_U8 ucFrom);
HI_U8 mrsGetNodeRole(HI_U8 ucTopoRole);
HI_U8 mrsGetNodeType(HI_U8 ucTopoType);
HI_U32 mrs3762_AFN10_F31(HI_INOUT HI_VOID * param);
HI_U32 mrs3762_AFN10_F32(HI_INOUT HI_VOID * param);
HI_U32 mrs3762_AFN10_F33(HI_INOUT HI_VOID * param);
HI_U32 mrs3762_AFN10_F40(HI_INOUT HI_VOID * param);
HI_U32 mrs3762_AFN10_F228(HI_INOUT HI_VOID * param);
HI_U32 mrs3762_AFN10_F100(HI_INOUT HI_VOID * param);
hi_u32 mrs3762_afn10_f104(HI_INOUT hi_void *param);
HI_U32 mrs3762_AFN10_F111(HI_INOUT HI_VOID * param);
HI_U32 mrs3762_AFN10_F112(HI_INOUT HI_VOID * param);
HI_U8 mrsGetPhaseFromTopo(HI_U8 ucPhaseState, HI_U8 ucPhaseResult);
HI_U8 mrsGetPhaseResultFromPhase(HI_U8 phase);
HI_U32 mrsSetQueryInfoBuf(HI_U8* pData, HI_U8 ucQueryNum, HI_U16 usStartSeq, HI_U8* pucReportNum, HI_U8 ucFrom);
HI_U32 mrsSetQueryStaIdBuf(HI_U8* pData, HI_U8 ucQueryNum, HI_U16 usStartSeq, HI_U8* pucReportNum);
HI_U32 mrsSetChipQueryBuf(HI_U8* pData, HI_U8 ucQueryNum, HI_U16 usStartSeq, HI_U8* pucReportNum);
HI_U32 mrsSetNetInfoBuf(HI_U8 *pBuffer, HI_U16 usBufLen, HI_MDM_NEIGHBOR_NET_INFO_S *pstNeighborInfo);

//AFN 11H
HI_U32 mrs3762_AFN11_F1(HI_INOUT HI_VOID * param);
HI_U32 mrs3762_AFN11_F2(HI_INOUT HI_VOID * param);
HI_U32 mrs3762_AFN11_F3(HI_INOUT HI_VOID * param);
HI_U32 mrs3762_AFN11_F4(HI_INOUT HI_VOID * param);
HI_U32 mrs3762_AFN11_F5(HI_INOUT HI_VOID * param);
HI_U32 mrs3762_AFN11_F6(HI_INOUT HI_VOID * param);
/* 问题单号: DTS2014010608635 END:   Added by liuxipeng/lkf54842, 2014/01/07 */
/* BEGIN: Modified by fengxiaomin/00209182, 2014/3/28   问题单号:DTS2014032708937 */
HI_U32 mrs3762_AFN11_F101(HI_INOUT HI_VOID * param);
/* END:   Modified by fengxiaomin/00209182, 2014/3/28 */

//AFN 12H
HI_U32 mrs3762_AFN12_F1(HI_INOUT HI_VOID * param);
HI_U32 mrs3762_AFN12_F2(HI_INOUT HI_VOID * param);
HI_U32 mrs3762_AFN12_F3(HI_INOUT HI_VOID * param);

//AFN 13H
HI_U32 mrs3762_AFN13_F1(HI_INOUT HI_VOID * param);

//AFN 14H
HI_U32 mrs3762_AFN14_F1(HI_INOUT HI_VOID * param);
#if defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG)
HI_U32 mrs3762_AFN14_F2(HI_INOUT HI_VOID * param);
HI_U32 mrs3762_AFN14_F2_UP(HI_VOID);
#endif
HI_U32 mrs3762_AFN14_F3(HI_INOUT HI_VOID * param);
HI_U32 mrs3762_AFN14_F3_UP(HI_INOUT AFN_FN_UL_PARAM *param, HI_IN HI_U8 protocol, HI_IN HI_U8 frame_len, HI_IN HI_U8 *frame_data);

//AFN 15H
HI_U32 mrs3762_AFN15_F1(HI_INOUT HI_VOID * param);

HI_U32 mrs3762_AFN20_F1(HI_INOUT HI_VOID * param);
HI_U32 mrs3762_AFN20_F2(HI_INOUT HI_VOID * param);
HI_U32 mrs3762_AFN03_F10_UP(HI_VOID);

//简易的组帧函数
HI_U32 mrs3762_EasyEnc(HI_U8 * pucContent,HI_U16 usLen,HI_VOID * param);

//GetFnIndex
HI_U8 mrs3762GetDtFnIndex(HI_IN HI_U8 dt[2]);

//AFN14 F1下行帧释放
HI_VOID mrs3762AFN14DLFree(HI_VOID *);

// AFNF0 F1: PLC测试帧
HI_U32 mrs3762_AFNF0_F1(HI_INOUT HI_VOID * param);

// AFNF0 F2: 过零检测测试帧
HI_U32 mrs3762_AFNF0_F2(HI_INOUT HI_VOID * param);
HI_U32 mrs3762_AFNF0_F229(HI_INOUT HI_VOID * param);
HI_U32 mrs3762_AFNF0_F230(HI_INOUT HI_VOID * param);
#if defined(MRS_SGD_INNER_VER)
//查询内外版本
HI_U32 mrs3762_AFNF0_F231(HI_INOUT HI_VOID * pParam);
#endif
// AFNF1 F1: 并发读表下行帧处理
HI_U32 mrs3762_AFNF1_F1(HI_INOUT HI_VOID * param);

//*****************************************************************************
// 函数名称: mrsWhetherAddNode
// 功能描述: 判断是否应添加从节点
//
// 参数说明:
//   pucContent [in] AFN11-F5的数据单元
//   ucMeterNum [in] 要添加的电表数量
//   stDenyData [in/out] 否认帧中的错误状态字
//
// 返 回 值:
//   HI_ERR_SUCCESS 应添加从节点
//   HI_ERR_BAD_DATA 地址映射表错误或读取失败
//   HI_ERR_FULL 表档案空间不足，不添加
//   HI_ERR_INITILIZATION_ALREADY 表号重复，不添加
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: fengxiaomin/f00209182 [2012-06-05]
//*****************************************************************************
HI_U32 mrsWhetherAddNode(HI_U8 * pucContent, HI_U8 ucMeterNum, AFN_00_F2 *stDenyData);

//*****************************************************************************
// 函数名称: mrsIsExistRepeatMeter
// 功能描述: 判断要添加的表中是否存在重复表号
//
// 参数说明:
//   pucContent [in] AFN11-F1的数据单元
//   ucMeterIndex [in] 序号
//   ucMeterNum [in] 要添加的电表数量
//
// 返 回 值:
//   HI_ERR_SUCCESS 不存在重复表号
//   HI_ERR_FAILURE 存在重复表号
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: fengxiaomin/f00209182 [2012-11-02]
//*****************************************************************************
HI_U32 mrsIsExistRepeatMeter(HI_U8 * pucContent, HI_U8 ucMeterIndex, HI_U8 ucMeterNum);

//*****************************************************************************
// 函数名称: mrsCheckProtocolValid
// 功能描述: 检查规约类型是否有效
//
// 参数说明:
//   ucProto [in] 规约类型
//
// 返 回 值:
//   HI_ERR_FAILURE  规约类型无效
//   HI_ERR_SUCCESS  规约类型有效
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: fengxiaomin/f00209182 [2012-07-03]
//*****************************************************************************
HI_U32 mrsCheckProtocolValid(HI_U8 ucProto);

//*****************************************************************************
// 函数名称: mrsGetRouteFlag
// 功能描述: 获取路由完成标志
//
// 参数说明:
//   HI_VOID
//
// 返 回 值:
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: fengxiaomin/f00209182 [2013-02-21]
//*****************************************************************************
/* BEGIN: Added by fengxiaomin/00209182, 2014/1/10   问题单号号:DTS2014012607684 */
HI_BOOL mrsGetRouteFlag(HI_VOID);

HI_VOID mrsInitSustainAfn(HI_VOID);
HI_U16 mrsMappedCodeVer(HI_U16 usVVer, HI_U16 usRVer, HI_U16 usSpcVer);
/* BEGIN: Modified by fengxiaomin/00209182, 2014/3/27   问题单号:DTS2014032708427 */
HI_U32 mrs3762_AFN10_F101(HI_INOUT HI_VOID * param);
HI_U32 mrsAfn10101Buf(HI_U8* pBuffer, HI_U8* pucReportNum, HI_U8 ucQueryNum, HI_U16 usStartSeq);
HI_U16 mrsGetCcoCodeVer(HI_CHAR *pVer);
HI_U16 mrsGetVRSpcVer(HI_CHAR *pVer, HI_CHAR* pStr);
HI_VOID mrsGetCco10F101Data(HI_U8 *pBuffer, HI_U8 ucReportNum);
HI_U32 mrsGetFrameMeter(HI_U8 ucProtocol, HI_U16 usDatalen, HI_U8 *pData, HI_U8* pMeter);
HI_U32 mrsCheckRmMeter(HI_U8 ucProtocol, HI_U8 *pMeter, HI_U16 usDatalen, HI_U8 *pData);
hi_void mrs_plc_detect_id_timeout(hi_void);

uintptr_t mrs_get_read_id_frame(hi_void);
hi_void mrs_free_read_id_frame(hi_void);
HI_VOID mrsMaintainTopoInfo(HI_VOID);
HI_U32 mrs3762_ReportAFN20_F4(HI_VOID);
HI_END_HEADER
#endif //__MRS_PROTO376_2_ECHO_H__
