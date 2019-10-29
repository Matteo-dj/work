//*****************************************************************************
//
//                  ��Ȩ���� (C), 1998-2011, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mrs_srv_capture.h
//  �� �� ��   : V1.0
//  ��    ��   : fengxiaomin/f00209182
//  ��������   : 2016-09-26
//  ��������   : TODO: ...
//
//  �����б�   : TODO: ...
//  �޸���ʷ   :
//  1.��    �� : 2016-09-26
//    ��    �� : fengxiaomin/f00209182
//    �޸����� : �����ļ�
//
//*****************************************************************************

//*****************************************************************************
// PROJECT   :
// SUBSYSTEM :
// MODULE    :
// OWNER     :
//*****************************************************************************

#ifndef __MRS_SRV_CAPTURE_H__
#define __MRS_SRV_CAPTURE_H__
HI_START_HEADER

typedef struct
{
    HI_U32 ver                  : 6;        // Э��汾��
    HI_U32 mac_head_len         : 6;        // ����ͷ����
    HI_U32 test_mode            : 4;        // ����ģʽ
    HI_U32 ruel_mode            : 4;        // ��Լ����
    HI_U32 test_para            : 12;       // ���Բ�����ת�����ݳ��Ȼ���ģʽ����ʱ��
}mrs_app_test_fream_st;

#define HI_FC_SIZE                              16
#define HI_LONG_PB_SIZE                         520
#define MRS_PLC_APP_HEAD_LEN                    4

#define MRS_HEAD_PORT_OFFSET                    0       // app����ͷ�еĶ˿ں�ƫ��
#define MRS_HEAD_PKT_ID_OFFSET                  1       // app����ͷ�еı���IDƫ��
#define MRS_PBH_SIZE_PSG                        1       // ����Э��PBͷ��С
#define MRS_HEAD_MSDU_TYPE_OFFSET_PSG           7
#define MRS_MSDU_TYPE_APP_PSG                   48      // Ӧ�ò㱨��
#define MRS_SHORT_HEAD_SIZE_PSG                 16      // ����Э���֡ͷ����
#define MRS_LONG_HEAD_SIZE_PSG                  28      // ����Э�鳤֡ͷ����

#define MRS_PKT_ID_PLC_TEST                     0x0006  // ͨ�Ų��Ա���ID
#define MRS_APP_PORT_NUM                        0x11    // ����ҵ��Ķ˿ں�
#define MRS_STD_PHYSICAL_TEST_MODE_LISTEN       (35)    // �ɽ������Ա��ĵ�ʱ�䣬��λs


#define MRS_PLC_FRAME_COM_OFFSET  (0)
#define MRS_PLC_FRAME_ID_OFFSET   (1)
#define MRS_PLC_RESP_FLAG_OFFSET  (5)
#define MRS_PLC_WORK_FLAG_OFFSET  (6)
#define MRS_PLC_FRAME_SEQ_OFFSET  (8)
#define MRS_PLC_TEST_ID_OFFSET    (12)
#define MRS_PLC_TEST_LEN_OFFSET   (14)
#define MRS_PLC_TEST_FREQ_OFFSET  (16)

#define MRS_PLC_TEST_RESP_FLAG    (0x20)
#define MRS_PLC_FRAME_COM_VALUSE  (0x11)
#define MRS_PLC_FRAME_ID_VALUSE   (0x0101)

#define MAC_TYPE_LL_PK            (0)         // MAC HEAD(32byte) �� ETH HEAD(18byte)Ϊ��׼Э��ṹ
#define MAC_TYPE_SS_PK            (1)         // MAC HEAD(12byte) �� ETH HEAD(2byte)

#define MAC_HEAD_LONG_SIZE        (32)
#define MAC_HEAD_SHORT_SIZE       (12)

#define MSDU_HEAD_LONG_SIZE       (18)
#define MSDU_HEAD_SHORT_SIZE      (2)

#define MAC_HEAD_TYPE_MASK        (0x01)
#define MSDU_HEAD_APP_TYPE        (0x01)
#define MAC_HEAD_MSDU_LEN_OFFSET  (2)


HI_VOID mrsSrvCaptureInit(HI_VOID);
HI_U32 mrsSrvCaptureDeinit(HI_VOID);
HI_U32 mrsSrvParseAppData(HI_U8 *pstPbData, HI_U8 **ppstData, HI_U16 *usLen);

HI_BOOL mrs_get_test_mode_enable(HI_VOID);
HI_VOID mrs_set_test_mode_enable(HI_BOOL enable);

HI_END_HEADER
#endif


