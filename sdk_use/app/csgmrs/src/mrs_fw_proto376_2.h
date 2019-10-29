//*****************************************************************************
//
//                  ��Ȩ���� (C), 1998-2011, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mrs_fw_proto376_2.h
//  �� �� ��   : V1.0 
//  ��    ��   : shenhankun/00130424
//  ��������   : 2011-11-29
//  ��������   : TODO: ...
//               
//  �����б�   : mrs3762StreamInput
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

#ifndef __MRS_FW_PROTO376_2_H__
#define __MRS_FW_PROTO376_2_H__
HI_START_HEADER

#define MRS_CCO_FRAME_TYPE_3762         (0)
#define MRS_CCO_FRAME_TYPE_CSG          (1)

#define MRS_3762_FRAME_HEAD_SIZE        (1) // 376.2֡ͷ�ֽ���
#define MRS_3762_FRAME_LEN_SIZE         (2) // 376.2֡�����ֽ���
#define MRS_3762_FRAME_CTRL_SIZE        (1) // 376.2֡�������ֽ���
#define MRS_3762_FRAME_INFO_SIZE        (6) // 376.2֡��Ϣ���ֽ���
#define MRS_3762_FRAME_AFN_SIZE         (1) // 376.2֡AFN�ֽ���
#define MRS_3762_FRAME_FN_SIZE          (2) // 376.2֡FN�ֽ���
#define MRS_3762_FRAME_CS_SIZE          (1) // 376.2֡У���ֽ���
#define MRS_3762_FRAME_END_SIZE         (1) // 376.2֡β�ֽ���

#define MRS_3762_FRAME_MIN_SIZE         (MRS_3762_FRAME_HEAD_SIZE  \
                                        + MRS_3762_FRAME_LEN_SIZE  \
                                        + MRS_3762_FRAME_CTRL_SIZE \
                                        + MRS_3762_FRAME_INFO_SIZE \
                                        + MRS_3762_FRAME_AFN_SIZE  \
                                        + MRS_3762_FRAME_FN_SIZE   \
                                        + MRS_3762_FRAME_CS_SIZE   \
                                        + MRS_3762_FRAME_END_SIZE)   // 376.2֡�̶���С����

#define MRS_CSG_FRAME_HEAD_SIZE         (1) // ����֡ͷ�ֽ���
#define MRS_CSG_FRAME_LEN_SIZE          (2) // ����֡�����ֽ���
#define MRS_CSG_FRAME_CTRL_SIZE         (1) // ����֡�������ֽ���
#define MRS_CSG_FRAME_AFN_SIZE          (1) // ����֡AFN�ֽ���
#define MRS_CSG_FRAME_SEQ_SIZE          (1) // ����֡SEQ�ֽ���
#define MRS_CSG_FRAME_DI_SIZE           (4) // ����֡DI�ֽ���
#define MRS_CSG_FRAME_CS_SIZE           (1) // ����֡У���ֽ���
#define MRS_CSG_FRAME_END_SIZE          (1) // ����֡β�ֽ���
#define MRS_CSG_FRAME_MIN_SIZE          (MRS_CSG_FRAME_HEAD_SIZE    \
                                        + MRS_CSG_FRAME_LEN_SIZE    \
                                        + MRS_CSG_FRAME_CTRL_SIZE   \
                                        + MRS_CSG_FRAME_AFN_SIZE    \
                                        + MRS_CSG_FRAME_SEQ_SIZE    \
                                        + MRS_CSG_FRAME_DI_SIZE     \
                                        + MRS_CSG_FRAME_CS_SIZE     \
                                        + MRS_CSG_FRAME_END_SIZE)   // ����֡�̶���С����



typedef struct  
{
    HI_U8* pucRcvBuffer;    // �ڴ��������Ľ���֡buffer
    HI_U16 usRcvBufferSize; // ����֡ʵ��buffer��С
    HI_U16 usDagramSize;    // �������۴�С 
} MRS_3762_PROTO_CTX_STRU;

//*****************************************************************************
// ��������: mrs3762ProtoInit
// ��������: 376.2Э�鴦�������ĳ�ʼ��
//           
// ����˵��:
//   pstCtx [in/out] MRS_3762_PROTO_CTX_STRU�ṹ
//
// �� �� ֵ: 
//    HI_ERR_SUCCESS : �ɹ���ȡ������376.2֡ 
//    ����Ϊ������ 
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: shenhankun/00130424 [2011-11-29]
//*****************************************************************************
HI_U32 mrs3762ProtoInit(HI_IN HI_OUT MRS_3762_PROTO_CTX_STRU* pstCtx);

HI_VOID mrs3762DiscardOldData(HI_VOID);
HI_U32 mrs3762ProtoStreamInputCheck(HI_BOOL bDiscardOldStream);
HI_VOID mrs3762ProtoStream2Cache(HI_PBYTE pucStream, HI_U16 usStreamLen);
HI_U32 mrs3762ProtoBuffer(HI_OUT MRS_ONE_RAW_FRAME_STRU ** ppstFrame, HI_IN HI_U16 usFrameType);


HI_END_HEADER

#endif //__MRS_PROTO376_2_H__
