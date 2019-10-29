//*****************************************************************************
//
//                  版权所有 (C), 1998-2010, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mux_rae.h
//  版 本 号   : V1.0 
//  作    者   : 陈文峰/00149739
//  生成日期   : 2010-04-21
//  功能描述   : MUX和RAE共用头文件
//               
//  函数列表   : TODO: ...
//  修改历史   : 
//  1.日    期 : 2010-04-21
//    作    者 : 陈文峰/00149739
//    修改内容 : 创建文件 
//
//*****************************************************************************

//*****************************************************************************
// PROJECT   : 
// SUBSYSTEM :
// MODULE    :  
// OWNER     :  
//*****************************************************************************




#ifndef __MUX_RAE_H__
#define __MUX_RAE_H__

//*****************************************************************************
// 
// RAE和MUX的接口定义说明
// 
// MUX和RAE之间数据包传输的头数据结构定义.
// MUX和DIAG之间的数据结构定义如下:
//*****************************************************************************

#define MSP_MUX_DIAGPACKET_IS_CNF(ulType)      ((ulType&0x02)?HI_TRUE:HI_FALSE)
#define MSP_MUX_DIAGPACKET_IS_IND(ulType)      ((ulType&0x04)?HI_TRUE:HI_FALSE)
#define MSP_MUX_DIAGPACKET_IS_SYS(ulType)      ((ulType&0x01)?HI_TRUE:HI_FALSE)
#define MSP_MUX_DIAGPACKET_IS_UNKNOWN(ulType)  (ulType==0)

#define MSP_MUX_DIAGPACKET_IS_MUX_CMD(ulType)  ((ulType&0x80)?HI_TRUE:HI_FALSE)

#define MSP_MUX_DIAGPACKET_SET_CNF(ulType)      (ulType|=0x02)
#define MSP_MUX_DIAGPACKET_SET_IND(ulType)      (ulType|=0x04)
#define MSP_MUX_DIAGPACKET_SET_SYS(ulType)      (ulType|=0x01)
#define MSP_MUX_DIAGPACKET_SET_UNKNOWN(ulType)  (ulType=0)

#define MSP_MUX_DIAGPACKET_SET_MUX_CMD(ulType)  (ulType=0x80|0x01)

typedef struct
{
    HI_U32 ulAuId;            // MSP的AUID.
    HI_U32 ulPacketSn;        // 包序列号
    HI_U8  ucChannelId;       // 通道ID
    HI_U8  ucType;            // 通过 MSP_MUX_DIAGPACKET_XX 读写
    HI_U16 usPacketDataSize;  // 包数据的大小, 即pucPackt数据大小, 不包括sizeof(MSP_MUX_PACKET_HEAD_STRU). 
    HI_U8  pucPacketData[0];  // 包数据buffer
} MSP_MUX_PACKET_HEAD_STRU;


typedef struct
{
    HI_U32 ulAuId;              /* MSP的AUID */
    HI_U32 ulPacketSn;          /* 包序列号 */
    HI_U8  ucChipTypeMode;      /* default: 0, 以后可用于区分SC/SG芯片 */
    HI_U8  reserved[3];         /* 固定填0 */
    HI_U8  ucChannelId;         /* 通道ID */
    HI_U8  ucType;              /* 通过 MSP_MUX_DIAGPACKET_XX 读写 */
    HI_U16 usPacketDataSize;    /* 包数据的大小, 即pucPackt数据大小, 不包括sizeof(MSP_MUX_PACKET_HEAD_STRU). */
    HI_U8  pucPacketData[0];    /* 包数据buffer */
} MSP_MUX_PACKET_HEAD_USERX_STRU;


// 最大值不能超过0xFF
typedef enum 
{
    EN_MUX_CMD_CONNECT = 0
} MUX_SPY_CMD_ENUM;

typedef struct  
{
    HI_U8 ucChannelId;
    HI_U8 ucBearerId; 
    HI_U8 ucCmdId; 
    HI_U8 ucReserved;
} MSP_MUX_SPY_CMD_STRU;

#define DIAG_CHL_HRD_SIZE           (sizeof(MSP_MUX_PACKET_HEAD_STRU))
#define DIAG_CHL_HRD_SIZE_X         (sizeof(MSP_MUX_PACKET_HEAD_USERX_STRU))
#define DIAG_DATA_HRD_SIZE          (sizeof(MSP_DIAG_HEAD_IND_STRU))
#define DIAG_ACK_HRD_SIZE           (sizeof(MSP_DIAG_HEAD_CNF_STRU))
#define DIAG_CHL_DATA_HRD_SIZE      (DIAG_CHL_HRD_SIZE + DIAG_DATA_HRD_SIZE)
#define DIAG_CHL_DATA_HRD_X_SIZE    (DIAG_CHL_HRD_SIZE_X + DIAG_DATA_HRD_SIZE)
#define DIAG_CHL_ACK_HRD_SIZE       (DIAG_CHL_HRD_SIZE + DIAG_ACK_HRD_SIZE)

#endif //__MUX_RAE_H__

