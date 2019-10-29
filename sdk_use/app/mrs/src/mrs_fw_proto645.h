//*****************************************************************************
//
//                  ��Ȩ���� (C), 1998-2011, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mrs_fw_proto645.h
//  �� �� ��   : V1.0 
//  ��    ��   : shenhankun/00130424
//  ��������   : 2011-11-29
//  ��������   : TODO: ...
//               
//  �����б�   : TODO: ...
//  �޸���ʷ   : 
//  1.��    �� : 2011-11-29
//    ��    �� : shenhankun/00130424
//    �޸����� : �����ļ� 
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
    ID_MRS_UART_645BUF = 0,    // ҵ�񴮿�BUF ID
    ID_MRS_IRDA_645BUF,        // ���⴮��BUF ID
    ID_MRS_RF_645BUF,        // ���ߴ���BUF ID
    MRS_645BUF_MAX             // 645����BUF������
} EM_MRS_645BUF_ID;

#define ID_MRS_BBUDA_645BUF     (ID_MRS_IRDA_645BUF)

#define MRS_645_FRAME_CONTROL_DIR_COMMAND_BIT            0 //����λ-����
#define MRS_645_FRAME_CONTROL_DIR_RESPONSION_BIT         1 //����λ-Ӧ��
#define MRS_645_FRAME_CONTROL_RESPONSION_NOMAL_BIT       0 //Ӧ���־-����Ӧ��
#define MRS_645_FRAME_CONTROL_RESPONSION_ABNORMITY_BIT   1 //Ӧ���־-�쳣Ӧ��
#define MRS_645_FRAME_CONTROL_AFER_FRAME_NON_BIT         0 //����֡��־-��
#define MRS_645_FRAME_CONTROL_AFER_FRAME_EXIST_BIT       1 //����֡��־-��
#define MRS_645_FRAME_CONTROL_FUNC_BIT       0x11 //������-������

#define MRS_645_FRAME_DATA_DI_SIZE     4
#define MRS_645_FRAME_DATA_DI_SIZE_V97 2
#define MRS_645_FRAME_COMMAND_ID_SIZE  4
#define MRS_645_FRAME_STAT_ID_SIZE     4

#define MRS_645_FRAME_CONTROL_DIR_UP        (0x80)
#define MRS_645_METER_ADDR_WILDCARDS        (0xAA)
#define MRS_645_METER_FRAME_HEAD            (0xFE)

#define MRS_645_FRAME_CONTROL_READ_DATA97   0x01
#define MRS_645_FRAME_CONTROL_READ_FOLLOW97 0x02
#define MRS_645_FRAME_CONTROL_READ_DATA     0x11
#define MRS_645_FRAME_CONTROL_READ_FOLLOW   0x12
#define MRS_645_FRAME_CONTROL_READ_ADDR     0x13
#define MRS_645_FRAME_CONTROL_WRITE_DATA    0x14
#define MRS_645_FRAME_CONTROL_WRITE_MAX     0x1D
#define MRS_645_FRAME_CONTROL_BRAKE         0x1C
#define MRS_645_FRAME_CONTROL_SAFE          0x03

#define MRS_645_CONTROL_TIMEOUT           2000

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

#define  MRS_645_CTRL_OFFSET                (MRS_645_FRAME_CTRL_OFFSET)
#define MRS_PREFIX_FE_SIZE                  (4)

#define MRS_645_IS_CTRL_CMD(x)              ( (((x) >= MRS_645_FRAME_CONTROL_WRITE_DATA) && ((x) <= MRS_645_FRAME_CONTROL_WRITE_MAX)) \
                                                || ((x) == MRS_645_FRAME_CONTROL_SAFE) )


typedef struct  
{
    HI_U8* pucRcvBuffer;    // �ڴ��������Ľ���֡buffer
    HI_U16 usRcvBufferSize; // ����֡buffer��С
    HI_U16 usDagramSize;    
}MRS_645_PROTO_CTX_STRU;


typedef struct  
{
    HI_U8 * pucHead;    // �������е�һ���Ϸ�֡��֡ͷ
    HI_U8 * pucAddr;    // �������е�һ���Ϸ�֡�ı��ַ
    HI_U32  ulCrc;      // ����������CRCУ��
    HI_U8   ucNum;      // �������кϷ�645֡����
    HI_BOOL bCtrlCmd;   // �Ƿ���ڷѿ������645֡
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
// ��������: mrs645ProtoInit
// ��������: 645Э�鴦�������ĳ�ʼ��
//           
// ����˵��:
//   pstCtx [in/out] MRS_645_PROTO_CTX_STRU�ṹָ��
//
// �� �� ֵ: 
//    HI_ERR_SUCCESS : �ɹ���ȡ������645֡ 
//    ����Ϊ������ 
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: shenhankun/00130424 [2011-11-29]
//*****************************************************************************
HI_U32 mrs645ProtoInit(HI_IN HI_OUT MRS_645_PROTO_CTX_STRU* pstCtx);

//*****************************************************************************
// ��������: mrs645ProtoStream2Buffer
// ��������: ��������ַ�������ֶ�����
//           
// ����˵��:
//   pucStream [in] �����ַ���
//   usStreamLen [in] �����ַ�����С
//   ucBufId [in] ������ID
//
// �� �� ֵ: 
//    HI_ERR_SUCCESS : �ɹ���ȡ������645֡ 
//    HI_ERR_CONTINUE : δ���ҵ��κε�����֡,��Ҫ�����ȴ�����
//    ����Ϊ������ 
//
// ���þ���: TODO: ...
// ��    ��: cuiate/00233580 [2017-07-06]
//*****************************************************************************
HI_U32 mrs645ProtoStream2Buffer(HI_PBYTE pucStream, HI_U16 usStreamLen, HI_U8 buf_id);

//*****************************************************************************
// ��������: mrs645ProtoStreamInput
// ��������: ��������ַ�������ֶ�����, �ж�֡�����Բ����������645֡
//           
// ����˵��:
//   pucStream [in] �����ַ���
//   usStreamLen [in] �����ַ�����С
//   pstFrame [out] ������ɸ�����645֡, ʹ���߲����ͷ�pstFrame.pucDatagram��ָ��.
//
// �� �� ֵ: 
//    HI_ERR_SUCCESS : �ɹ���ȡ������645֡ 
//    HI_ERR_CONTINUE : δ���ҵ��κε�����֡,��Ҫ�����ȴ�����
//    ����Ϊ������ 
//
// ����Ҫ��: ʹ���߲����ͷ�pstFrame.pucDatagram��ָ�룬����mrs645ProtoCleanup�������.
// ���þ���: TODO: ...
// ��    ��: shenhankun/00130424 [2011-11-29]
//*****************************************************************************
HI_U32 mrs645ProtoStreamInput(HI_PBYTE pucStream, HI_U16 usStreamLen,
                                        HI_OUT MRS_ONE_RAW_FRAME_STRU* pstFrame, HI_U8 buf_id);

HI_U32 mrs645ProtoStreamInputCheck(HI_BOOL bDiscardOldStream, HI_U8 buf_id);

// �ڻ������в�����Ч��645֡�����֡ͷƫ�Ƽ�֡��
HI_U32 mrsFind645Frame(HI_IN HI_PBYTE in_buf, HI_IN HI_S16 in_len, 
                            HI_OUT HI_U16 *start, HI_OUT HI_U16 *out_len);

// ͳ�ƻ�������645֡��Ϣ
HI_VOID mrsCountBuf645Inf(HI_IN HI_PBYTE buf, HI_IN HI_U16 len, HI_OUT MRS_645BUF_INF *inf);

//���������ĳЩ�����ʱ�䳤������ʱʱ��ı�־
HI_VOID mrsCtrlCmdProc(HI_U8 *pDataBuff,HI_U16 usDataLen);

// 645֡����
HI_VOID mrs645DataDecode(HI_U8 * buf, HI_U32 len);
HI_VOID mrs645DataEncode(HI_U8 * buf, HI_U32 len);

// ����ַԤ����: ����֡97���ַ��λ�ֽ�AAH->00H
HI_VOID mrsMeterAddrULPreProc(HI_OUT HI_U8 *aucDstAddr, HI_IN HI_U8 *aucSrcAddr, HI_IN HI_U8 ucProtocol, HI_IN HI_U8 ucCtrl);

// ����ַԤ����: 97���ַ��λ�ֽ�AAH->00H
HI_VOID mrsMeterAddrPreProc(HI_OUT HI_U8 *aucDstAddr, HI_IN HI_U8 *aucSrcAddr, HI_IN HI_U8 ucProtocol);

// ��ַpDstAddr�Ƿ���pSrcAddrƥ��(ͨ���0xAA)
// ȥ��pSrcAddr��λ�ֽڵ�AAH֮���õ�λ�ֽں�pDstAddr�Ƚ�(����עpDstAddr��λ�ֽڵ�AAH)
HI_BOOL mrsMeterAddrMatch(HI_U8 *pSrcAddr, HI_U8 *pDstAddr);

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO_BROAD_BAND_UNIT)
HI_U32 mrsIsSameAsBufferBbu(HI_U16 usDatagramSize, HI_U8 *pucDatagram, HI_U8 buf_id);
HI_U32 mrsAddFrameToBufferBbu(HI_U16 usDatagramSize, HI_U8 *pucDatagram, HI_U8 buf_id);
HI_VOID mrsClear645ProtoBufferBbu(HI_U8 buf_id);
#endif

HI_U32 mrsGen645Frame(HI_IN MRS_PROTO645_FRAME_INF *pFrameInf, HI_OUT HI_U8 **ppFrame, HI_OUT HI_U16 *pFrameLen);
HI_U32 mrsGetFrame645MeterAddr(HI_U8 *pucFrame, HI_U16 usFrameLength, HI_U8 *pucMeterAddr);

HI_END_HEADER 
#endif //__MRS_PROTO645_H__


