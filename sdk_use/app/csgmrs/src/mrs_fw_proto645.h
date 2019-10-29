//*****************************************************************************
//
//                  版权所有 (C), 1998-2011, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_fw_proto645.h
//  版 本 号   : V1.0 
//  作    者   : shenhankun/00130424
//  生成日期   : 2011-11-29
//  功能描述   : TODO: ...
//               
//  函数列表   : TODO: ...
//  修改历史   : 
//  1.日    期 : 2011-11-29
//    作    者 : shenhankun/00130424
//    修改内容 : 创建文件 
//
//*****************************************************************************

//*****************************************************************************
// PROJECT   : 
// SUBSYSTEM :
// MODULE    :  
// OWNER     :  
//*****************************************************************************

#ifndef __MRS_FW_PROTO645_H__
#define __MRS_FW_PROTO645_H__
HI_START_HEADER

typedef enum
{
    ID_MRS_UART_645BUF = 0,    // 业务串口BUF ID
    ID_MRS_IRDA_645BUF,        // 红外串口BUF ID
    MRS_645BUF_MAX             // 645串口BUF总数量
} EM_MRS_645BUF_ID;

#define ID_MRS_BBUDA_645BUF     (ID_MRS_IRDA_645BUF)

#define MRS_645_FRAME_CONTROL_DIR_COMMAND_BIT            0 //方向位-命令
#define MRS_645_FRAME_CONTROL_DIR_RESPONSION_BIT         1 //方向位-应答
#define MRS_645_FRAME_CONTROL_RESPONSION_NOMAL_BIT       0 //应答标志-正常应答
#define MRS_645_FRAME_CONTROL_RESPONSION_ABNORMITY_BIT   1 //应答标志-异常应答
#define MRS_645_FRAME_CONTROL_AFER_FRAME_NON_BIT         0 //后续帧标志-无
#define MRS_645_FRAME_CONTROL_AFER_FRAME_EXIST_BIT       1 //后续帧标志-有
#define MRS_645_FRAME_CONTROL_FUNC_BIT       0x11 //功能码-读数据

#define MRS_645_FRAME_DATA_DI_SIZE     4
#define MRS_645_FRAME_DATA_DI_SIZE_V97 2
#define MRS_645_FRAME_COMMAND_ID_SIZE  4
#define MRS_645_FRAME_STAT_ID_SIZE     4

#define MRS_645_FRAME_CONTROL_DIR_UP        (0x80)
#define MRS_645_METER_ADDR_WILDCARDS        (0xAA)


#define MRS_645_FRAME_CONTROL_READ_DATA97   (0x01)
#define MRS_645_FRAME_CONTROL_READ_FOLLOW97 (0x02)
#define MRS_645_FRAME_CONTROL_READ_DATA     (0x11)
#define MRS_645_FRAME_CONTROL_READ_FOLLOW   (0x12)
#define MRS_645_FRAME_CONTROL_READ_ADDR     (0x13)
#define MRS_645_FRAME_CONTROL_WRITE_DATA    (0x14)
#define MRS_645_FRAME_CONTROL_WRITE_MAX     (0x1D)
#define MRS_645_FRAME_CONTROL_BRAKE         (0x1C)
#define MRS_645_FRAME_CONTROL_SAFE          (0x03)

#define MRS_645_CONTROL_TIMEOUT             2000

#define MRS_645_FRAME_CONTROL_MASK          (0x1F)
#define MRS_645_FRAME_CONTROL_ADJUST_TIME   (0x08)
#define MRS_645_FRAME_CONTROL_DENY_MASK     (0x40)

#define MRS_645_FRAME_LENGTH_MIN            (12)
#define MRS_645_FRAME_1ST_HEAD_LEN          (1)
#define MRS_645_FRAME_METERADD_LEN          (6)
#define MRS_645_FRAME_2ND_HEAD_LEN          (1)
#define MRS_645_FRAME_CTRL_LEN              (1)
#define MRS_645_FRAME_LENGTH_LEN            (1)

#define MRS_645_FRAME_1ST_HEAD_OFFSET       (0)
#define MRS_645_FRAME_METERADD_OFFSET       (MRS_645_FRAME_1ST_HEAD_OFFSET + MRS_645_FRAME_1ST_HEAD_LEN)
#define MRS_645_FRAME_2ND_HEAD_OFFSET       (MRS_645_FRAME_METERADD_OFFSET + MRS_645_FRAME_METERADD_LEN)
#define MRS_645_FRAME_CTRL_OFFSET           (MRS_645_FRAME_2ND_HEAD_OFFSET + MRS_645_FRAME_2ND_HEAD_LEN)
#define MRS_645_FRAME_LENGTH_OFFSET         (MRS_645_FRAME_CTRL_OFFSET + MRS_645_FRAME_CTRL_LEN)
#define MRS_645_FRAME_DATA_OFFSET           (MRS_645_FRAME_LENGTH_OFFSET + MRS_645_FRAME_LENGTH_LEN)

#define MRS_645_FRAME_ADJUST_TIME_LEN       (0x06)
#define MRS_645_FRAME_ADJUST_TIME_FRAMELEN  (0x12)

#define MRS_645_FRAME_START_FLG             (0x68)
#define MRS_645_FRAME_END_FLG               (0x16)
#define MRS_645_FRAME_HEX33                 (0x33)

#define MRS_645_CTRL_OFFSET                 (MRS_645_FRAME_CTRL_OFFSET)
#define MRS_PREFIX_FE_SIZE                  (4)

#define MRS_645_IS_CTRL_CMD(x)              ( (((x) >= MRS_645_FRAME_CONTROL_WRITE_DATA) && ((x) <= MRS_645_FRAME_CONTROL_WRITE_MAX)) \
                                                || ((x) == MRS_645_FRAME_CONTROL_SAFE) )


typedef struct  
{
    HI_U8* pucRcvBuffer;    // 内存分配出来的接收帧buffer
    HI_U16 usRcvBufferSize; // 接收帧buffer大小
    HI_U16 usDagramSize;    
}MRS_645_PROTO_CTX_STRU;


typedef struct  
{
    HI_U8 * pucHead;    // 缓冲区中第一个合法帧的帧头
    HI_U8 * pucAddr;    // 缓冲区中第一个合法帧的表地址
    HI_U32  ulCrc;      // 缓冲区数据CRC校验
    HI_U8   ucNum;      // 缓冲区中合法645帧个数
    HI_BOOL bCtrlCmd;   // 是否存在费控命令的645帧
    HI_U8   padding[2];
}MRS_645BUF_INF;


typedef struct
{
    HI_U8 aucAddr[HI_METER_ADDR_LEN];
    HI_U8 ucCtrl;
    HI_U8 ucDiSize;
    HI_U32 ulDi;
    HI_U8 aucReserved[3];
    HI_U8 ucDataLength;
    HI_U8 *pData;
} MRS_PROTO645_FRAME_INF;


HI_VOID mrs645SetContext(HI_U32 buf_645, HI_U8 buf_id);
MRS_645_PROTO_CTX_STRU * mrs645GetContext(HI_U8 buf_id);



//*****************************************************************************
// 函数名称: mrs645ProtoInit
// 功能描述: 645协议处理上下文初始化
//           
// 参数说明:
//   pstCtx [in/out] MRS_645_PROTO_CTX_STRU结构指针
//
// 返 回 值: 
//    HI_ERR_SUCCESS : 成功获取完整的645帧 
//    其它为错误码 
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: shenhankun/00130424 [2011-11-29]
//*****************************************************************************
HI_U32 mrs645ProtoInit(HI_IN HI_OUT MRS_645_PROTO_CTX_STRU* pstCtx);

//*****************************************************************************
// 函数名称: mrs645ProtoStreamInput
// 功能描述: 对输入的字符流镜像分段重组, 判断帧完整性并获得完整的645帧
//           
// 参数说明:
//   pucStream [in] 输入字符流
//   usStreamLen [in] 输入字符流大小
//   pstFrame [out] 输出若干个完整645帧, 使用者不能释放pstFrame.pucDatagram该指针.
//
// 返 回 值: 
//    HI_ERR_SUCCESS : 成功获取完整的645帧 
//    HI_ERR_CONTINUE : 未能找到任何的完整帧,需要继续等待数据
//    其它为错误码 
//
// 调用要求: 使用者不能释放pstFrame.pucDatagram该指针，调用mrs645ProtoCleanup清除缓存.
// 调用举例: TODO: ...
// 作    者: shenhankun/00130424 [2011-11-29]
//*****************************************************************************
HI_U32 mrs645ProtoStreamInput(HI_PBYTE pucStream, HI_U16 usStreamLen,
                                        HI_OUT MRS_ONE_RAW_FRAME_STRU* pstFrame, HI_U8 buf_id);

HI_U32 mrs645ProtoStreamInputCheck(HI_BOOL bDiscardOldStream, HI_U8 buf_id);

// 在缓冲区中查找有效的645帧并输出帧头偏移及帧长
HI_U32 mrsFind645Frame(HI_IN HI_PBYTE in_buf, HI_IN HI_S16 in_len, 
                            HI_OUT HI_U16 *start, HI_OUT HI_U16 *out_len);

// 统计缓冲区中645帧信息
HI_VOID mrsCountBuf645Inf(HI_IN HI_PBYTE buf, HI_IN HI_U16 len, HI_OUT MRS_645BUF_INF *inf);

//设置因电表对某些命令处理时间长所引起超时时间的标志
HI_VOID mrsCtrlCmdProc(HI_U8 *pDataBuff,HI_U16 usDataLen);

// 645帧解码
HI_VOID mrs645DataDecode(HI_U8 * buf, HI_U32 len);
HI_VOID mrs645DataEncode(HI_U8 * buf, HI_U32 len);

// 电表地址预处理: 上行帧97表地址高位字节AAH->00H
HI_VOID mrsMeterAddrULPreProc(HI_OUT HI_U8 *aucDstAddr, HI_IN HI_U8 *aucSrcAddr, HI_IN HI_U8 ucProtocol, HI_IN HI_U8 ucCtrl);

// 电表地址预处理: 97表地址高位字节AAH->00H
HI_VOID mrsMeterAddrPreProc(HI_OUT HI_U8 *aucDstAddr, HI_IN HI_U8 *aucSrcAddr, HI_IN HI_U8 ucProtocol);

// 地址pDstAddr是否与pSrcAddr匹配(通配符0xAA)
// 去掉pSrcAddr高位字节的AAH之后，用低位字节和pDstAddr比较(不关注pDstAddr高位字节的AAH)
HI_BOOL mrsMeterAddrMatch(HI_U8 *pSrcAddr, HI_U8 *pDstAddr);

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO_BROAD_BAND_UNIT)
HI_U32 mrsIsSameAsBufferBbu(HI_U16 usDatagramSize, HI_U8 *pucDatagram, HI_U8 buf_id);
HI_U32 mrsAddFrameToBufferBbu(HI_U16 usDatagramSize, HI_U8 *pucDatagram, HI_U8 buf_id);
HI_VOID mrsClear645ProtoBufferBbu(HI_U8 buf_id);
#endif

HI_U32 mrsGen645Frame(HI_IN MRS_PROTO645_FRAME_INF *pFrameInf, HI_OUT HI_U8 **ppFrame, HI_OUT HI_U16 *pFrameLen);

HI_END_HEADER 
#endif //__MRS_PROTO645_H__


