//*****************************************************************************
//
//                  ��Ȩ���� (C), 1998-2010, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mux_rae.h
//  �� �� ��   : V1.0 
//  ��    ��   : ���ķ�/00149739
//  ��������   : 2010-04-21
//  ��������   : MUX��RAE����ͷ�ļ�
//               
//  �����б�   : TODO: ...
//  �޸���ʷ   : 
//  1.��    �� : 2010-04-21
//    ��    �� : ���ķ�/00149739
//    �޸����� : �����ļ� 
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
// RAE��MUX�Ľӿڶ���˵��
// 
// MUX��RAE֮�����ݰ������ͷ���ݽṹ����.
// MUX��DIAG֮������ݽṹ��������:
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
    HI_U32 ulAuId;            // MSP��AUID.
    HI_U32 ulPacketSn;        // �����к�
    HI_U8  ucChannelId;       // ͨ��ID
    HI_U8  ucType;            // ͨ�� MSP_MUX_DIAGPACKET_XX ��д
    HI_U16 usPacketDataSize;  // �����ݵĴ�С, ��pucPackt���ݴ�С, ������sizeof(MSP_MUX_PACKET_HEAD_STRU). 
    HI_U8  pucPacketData[0];  // ������buffer
} MSP_MUX_PACKET_HEAD_STRU;


typedef struct
{
    HI_U32 ulAuId;              /* MSP��AUID */
    HI_U32 ulPacketSn;          /* �����к� */
    HI_U8  ucChipTypeMode;      /* default: 0, �Ժ����������SC/SGоƬ */
    HI_U8  reserved[3];         /* �̶���0 */
    HI_U8  ucChannelId;         /* ͨ��ID */
    HI_U8  ucType;              /* ͨ�� MSP_MUX_DIAGPACKET_XX ��д */
    HI_U16 usPacketDataSize;    /* �����ݵĴ�С, ��pucPackt���ݴ�С, ������sizeof(MSP_MUX_PACKET_HEAD_STRU). */
    HI_U8  pucPacketData[0];    /* ������buffer */
} MSP_MUX_PACKET_HEAD_USERX_STRU;


// ���ֵ���ܳ���0xFF
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

