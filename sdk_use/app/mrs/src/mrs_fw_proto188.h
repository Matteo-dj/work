//*****************************************************************************
//
//                  版权所有 (C), 2017-2027, 深圳国电科技
//
//*****************************************************************************

//*****************************************************************************

#ifndef _MRS_FW_PROTO188_H_
#define _MRS_FW_PROTO188_H_
HI_CPP_START
#if defined(MRS_SRV_PROTOCOL_CJT188)
#define HI_188_ADDR_LEN (7)

#define HI_188_ADDR_POS (2)

#define MRS_PROTO188_DATAGRAM_DATA_LEN_MAX     200
#define MRS_PROTO188_DATAGRAM_HEADER_LEN_MAX   13
#define MRS_PROTO188_DATAGRAM_LEN_MAX   (MRS_PROTO188_DATAGRAM_DATA_LEN_MAX\
											+MRS_PROTO188_DATAGRAM_HEADER_LEN_MAX)

#define SGD_188_FRAME_DECAY     1080000 //3h
#define SGD_TIME_MIN_INTO_TICKS (6000)  // 1min
#define SGD_188_FRAME_OFFSET_DI   (11)
#define SGD_188_FRAME_OFFSET_SER  (13)


typedef struct 
{
    HI_U8 ucType; // 操作设备类型
    HI_U8 ucAddr[7];           // 表地址，固定7字节 
    HI_U8 ucCtrl; // 控制码
    HI_U8 ucDataRealmLen; // 数据域大小必须小于100字节, 0表示无数据域
    HI_U8 ucDataRealm[MRS_PROTO188_DATAGRAM_DATA_LEN_MAX]; // 数据域 
}MRS_188_FRAME_STRU;

HI_U32 mrsToolsCheck188Frame(HI_PBYTE ptr,HI_U16 len);
HI_U32 MRS_Proto188Dec( HI_IN HI_PBYTE pucDatagram, HI_U16 usDatagramSize, HI_OUT MRS_188_FRAME_STRU* pstFrame );
HI_U32 mrsFind188MeterAddr(HI_PBYTE pucBuffer, HI_U16 usLength, HI_U8 *pucMeterAddr);
HI_U32 mrsFind188Frame(HI_IN HI_PBYTE in_buf, HI_IN HI_S16 in_len, HI_OUT HI_U16 *start, HI_OUT HI_U16 *out_len);
HI_U8 mrsGetT188ProMeterType(HI_PBYTE pucBuffer, HI_U16 usLength);
HI_U8 *mrsGetT188ProMeterAddress(HI_PBYTE pucBuffer, HI_U16 usLength);
HI_U8 *mrsGetT188ProData(HI_PBYTE pucBuffer, HI_U16 usLength);
HI_U8 mrsGetT188ProDataLen(HI_PBYTE pucBuffer, HI_U16 usLength);

#endif


HI_CPP_END
#endif 
