//*****************************************************************************
//
//                  版权所有 (C), 2017-2027, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_fw_proto698_45.h
//  版 本 号   : V1.0 
//  作    者   : Cui Ate/c00233580
//  生成日期   : 2017-06-28
//  功能描述   : TODO: ...
//               
//  函数列表   : TODO: ...
//  修改历史   : 
//  1.日    期 : 2017-06-28
//    作    者 : Cui Ate/c00233580
//    修改内容 : 创建文件 
//
//*****************************************************************************

#ifndef _MRS_FW_PROTO698_45_H_
#define _MRS_FW_PROTO698_45_H_
HI_CPP_START


#define MRS_FRAME_HEAD          (0x68)
#define MRS_FRAME_TAIL          (0x16)
#define MRS_698_CRC_INIT        (0xFFFF)

/* length = start (1B) + len (2B) + ctrl (1B) + a (>= 3B) + HCS (2B) + FCS (2B) + tail (1B) */
#define MRS_698_FRAME_MIN_LEN   (12)
#define MRS_698_FIXED_HEAD_LEN  (5)     /* L + C + SA + CA */

#define MRS_698_FRAME_HEAD_OFFSET       (0)
#define MRS_698_FRAME_LENGTH_OFFSET     (1)
#define MRS_698_FRAME_CTRL_OFFSET       (3)
#define MRS_698_FRAME_SA_OFFSET         (4)
#define MRS_698_FRAME_ADDR_OFFSET       (5)

#define MRS_698_FRAME_DIR_MASK          (0x80)

#define MRS_698_FRAME_CONTROL_DIR_COMMAND_BIT            0 //方向位-命令
#define MRS_698_FRAME_CONTROL_DIR_RESPONSION_BIT         1 //方向位-应答

/*lint -e958*/
#pragma pack(1)
typedef struct
{
    HI_U8 ucHead;
    HI_U16 usLength;
    HI_U8 ucCtrl;
    HI_U8 ucSAInf;
    HI_U8 aucAddr[HI_METER_ADDR_LEN];
    HI_U8 ucCAInf;
    HI_U16 usHCS;
} MRS_698_HEAD_STRU;
#pragma pack()
/*lint +e958*/


HI_VOID mrs698Crc16Init(HI_VOID);

HI_U16 mrs698Crc16(HI_REG HI_U16 usInitCrc, HI_REG HI_U8 * pBuf, HI_REG HI_S32 sLength);


HI_U32 mrsCheck698Frame(HI_PBYTE pucFrame, HI_U16 usLength);

HI_U32 mrsFind698Frame(HI_PBYTE pucBuffer, HI_U16 usLength, HI_U16 *pusStart, HI_U16 *pusFrameLength);

HI_U32 mrsFind698MeterAddr(HI_PBYTE pucBuffer, HI_U16 usLength, HI_U8 *pucMeterAddr);

HI_U32 mrsFind698Dir(HI_IN HI_PBYTE buf, HI_IN HI_U16 len, HI_OUT HI_U8 *dir);

HI_CPP_END
#endif // _MRS_FW_PROTO698_45_H_
