//*****************************************************************************
//
//                  ��Ȩ���� (C), 1998-2011, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mrs_fw_proto376_2_echo.h
//  �� �� ��   : V1.0
//  ��    ��   : niesongsong/kf62735
//  ��������   : 2011-12-14
//  ��������   : TODO: ...
//
//  �����б�   : TODO: ...
//  �޸���ʷ   :
//  1.��    �� : 2011-12-14
//    ��    �� : niesongsong/kf62735
//    �޸����� : �����ļ�
//
//*****************************************************************************
#ifndef __MRS_FW_PROTO376_2_ECHO_H__
#define __MRS_FW_PROTO376_2_ECHO_H__

HI_START_HEADER

#define MRS_3762_F10_NODE_MAX_NUM   (32)
#define MRS_3762_ADD_METERS_MAX_NUM  (32)

/* ����֡: ����״̬�� �����淶(376.2-2009��) */
#define MRS_3762_DENY_STATUS_COMMUNICATION_OVERTIME     (0)     // ͨ�ų�ʱ
#define MRS_3762_DENY_STATUS_INVALID_DATA               (1)     // ��Ч���ݵ�Ԫ
#define MRS_3762_DENY_STATUS_ERROR_LENGTH               (2)     // ���ȴ�
#define MRS_3762_DENY_STATUS_ERROR_CHECK_SUM            (3)     // У�����
#define MRS_3762_DENY_STATUS_INEXISTENT_INFO            (4)     // ��Ϣ�಻����
#define MRS_3762_DENY_STATUS_ERROR_FORMAT               (5)     // ��ʽ����
#define MRS_3762_DENY_STATUS_REPETITION_METER           (6)     // ����ظ�: ���-���ͨ�ŵ�ַ
#define MRS_3762_DENY_STATUS_INEXISTENT_METER           (7)     // ��Ų�����
#define MRS_3762_DENY_RM_BUFPOOL_FULL                   (9)     // ����ģ�黺�����
#define MRS_3762_DENY_RM_645ID_OVERRUN                  (10)    // ���������Ŀ������(645֡����)
#define MRS_3762_DENY_RM_METER_BUSY                     (11)    // �ñ����ڳ����У��Զ���
#define MRS_3762_DENY_CCO_BUSY                          (12)    // CCOģ��æ����������

/* ����֡: ����״̬�� �����淶(1376.2-2013��) */
#define MRS_13762_DENY_STATUS_COMMUNICATION_OVERTIME    (MRS_3762_DENY_STATUS_COMMUNICATION_OVERTIME)
#define MRS_13762_DENY_STATUS_INVALID_DATA              (MRS_3762_DENY_STATUS_INVALID_DATA)
#define MRS_13762_DENY_STATUS_ERROR_LENGTH              (MRS_3762_DENY_STATUS_ERROR_LENGTH)
#define MRS_13762_DENY_STATUS_ERROR_CHECK_SUM           (MRS_3762_DENY_STATUS_ERROR_CHECK_SUM)
#define MRS_13762_DENY_STATUS_INEXISTENT_INFO           (MRS_3762_DENY_STATUS_INEXISTENT_INFO)
#define MRS_13762_DENY_STATUS_ERROR_FORMAT              (MRS_3762_DENY_STATUS_ERROR_FORMAT)
#define MRS_13762_DENY_STATUS_REPETITION_METER          (MRS_3762_DENY_STATUS_REPETITION_METER)
#define MRS_13762_DENY_STATUS_INEXISTENT_METER          (MRS_3762_DENY_STATUS_INEXISTENT_METER)
#define MRS_13762_DENY_STATUS_METER_APP_NOT_REPLY       (8)     // ���Ӧ�ò���Ӧ��: ָͨ�Ž������ͨ�Ż��м̣��������ĵ��645Э����Ӧ��
#define MRS_13762_DENY_STATUS_CCO_BUSY                  (9)     // ���ڵ�æ: ��ʾ���ڵ�����ִ����һ�������ʱ�޷���Ӧ��ǰ����
#define MRS_13762_DENY_STATUS_CCO_NOT_SUPPORT           (10)    // ���ڵ㲻֧�ִ�����: ���ڵ�Ե�ǰ�����޷�֧��
#define MRS_13762_DENY_STATUS_STA_NOT_REPLY             (11)    // �ӽڵ���Ӧ��: ���ڵ�δ�յ��ӽڵ��Ӧ������
#define MRS_13762_DENY_STATUS_STA_NOT_JOIN_NETWORK      (12)    // �ӽڵ㲻������: �ӽڵ���δ�������ڵ������
#define MRS_13762_DENY_RM_BUFPOOL_FULL                  (109)   // ����ģ�黺�����
#define MRS_13762_DENY_RM_645ID_OVERRUN                 (110)   // ���������Ŀ������(645֡����)
#define MRS_13762_DENY_RM_METER_BUSY                    (111)   // �ñ����ڳ����У��Զ���

/* ���ڳ���ģʽ */
typedef enum
{
    PERIOD_METER_READ_MODE_BOTH = 0,
    PERIOD_METER_READ_MODE_MASTER,
    PERIOD_METER_READ_MODE_CCO,
} PERIOD_METER_READ_MODE_ENUM;

#define MRS_AFN_FRAME_DELAY_UNRELATED   (0x00)
#define MRS_AFN_FRAME_DELAY_RELATED     (0x01)

// ��ǰ���ı���ͨ������ʱ�����̶�ֵ1��
#define MRS_AFN_COMM_DELAY              (1)

#define MRS_AFN(AFN)                    (HI_U8)(AFN)
/* BEGIN: Added by fengxiaomin/00209182, 2014/1/10   ���ⵥ�ź�:DTS2014012607684 */
#define MRS_CCO_PROTOCOL_AFN_13        (0x13)
#define MRS_CCO_PROTOCOL_AFN_06        (0x06)
#define MRS_CCO_PROTOCOL_AFN_14        (0x14)

/* BEGIN: Modified by fengxiaomin/00209182, 2014/3/27   ���ⵥ��:DTS2014032708427 */
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

//645������󳤶�����
#define MRS_AFNF1_F1_645_MAX_LEN       (2000)

//AFN_FN������
typedef HI_U32 (*MRS_3762_AFN_FUN)(HI_VOID * param);

typedef struct
{
    HI_U8 ucCmdState    :1;    // ����״̬ 0-δ���� 1-�Ѵ���
    HI_U8 ucCH_1State   :1;    // �ŵ�1״̬ 0-æ 1-��
    HI_U8 ucCH_2State   :1;    // �ŵ�2״̬ 0-æ 1-��
    HI_U8 ucCH_3State   :1;    // �ŵ�3״̬ 0-æ 1-��
    HI_U8 ucCH_4State   :1;    // �ŵ�4״̬ 0-æ 1-��
    HI_U8 ucCH_5State   :1;    // �ŵ�5״̬ 0-æ 1-��
    HI_U8 ucCH_6State   :1;    // �ŵ�6״̬ 0-æ 1-��
    HI_U8 ucCH_7State   :1;    // �ŵ�7״̬ 0-æ 1-��

    HI_U8 ucCH_8State   :1;    // �ŵ�8״̬ 0-æ 1-��
    HI_U8 ucCH_9State   :1;    // �ŵ�9״̬ 0-æ 1-��
    HI_U8 ucCH_10State  :1;    // �ŵ�10״̬ 0-æ 1-��
    HI_U8 ucCH_11State  :1;    // �ŵ�11״̬ 0-æ 1-��
    HI_U8 ucCH_12State  :1;    // �ŵ�12״̬ 0-æ 1-��
    HI_U8 ucCH_13State  :1;    // �ŵ�13״̬ 0-æ 1-��
    HI_U8 ucCH_14State  :1;    // �ŵ�14״̬ 0-æ 1-��
    HI_U8 ucCH_15State  :1;    // �ŵ�15״̬ 0-æ 1-��

    HI_U16 uSWitTime;          // �ȴ�ʱ��
}AFN_00_F1; // ȷ��

typedef struct
{
    HI_U8 ucCmdState    :1;    // ����״̬ 0-δ���� 1-�Ѵ���
    HI_U8 ucCH_1State   :1;    // �ŵ�1״̬ 0-æ 1-��
    HI_U8 ucCH_2State   :1;    // �ŵ�2״̬ 0-æ 1-��
    HI_U8 ucCH_3State   :1;    // �ŵ�3״̬ 0-æ 1-��
    HI_U8 ucCH_4State   :1;    // �ŵ�4״̬ 0-æ 1-��
    HI_U8 ucCH_5State   :1;    // �ŵ�5״̬ 0-æ 1-��
    HI_U8 ucCH_6State   :1;    // �ŵ�6״̬ 0-æ 1-��
    HI_U8 ucCH_7State   :1;    // �ŵ�7״̬ 0-æ 1-��

    HI_U8 ucCH_8State   :1;    // �ŵ�8״̬ 0-æ 1-��
    HI_U8 ucCH_9State   :1;    // �ŵ�9״̬ 0-æ 1-��
    HI_U8 ucCH_10State  :1;    // �ŵ�10״̬ 0-æ 1-��
    HI_U8 ucCH_11State  :1;    // �ŵ�11״̬ 0-æ 1-��
    HI_U8 ucCH_12State  :1;    // �ŵ�12״̬ 0-æ 1-��
    HI_U8 ucCH_13State  :1;    // �ŵ�13״̬ 0-æ 1-��
    HI_U8 ucCH_14State  :1;    // �ŵ�14״̬ 0-æ 1-��
    HI_U8 ucCH_15State  :1;    // �ŵ�15״̬ 0-æ 1-��

    HI_U8 ucCH_16State  :1;    // �ŵ�16״̬ 0-æ 1-��
    HI_U8 ucCH_17State  :1;    // �ŵ�17״̬ 0-æ 1-��
    HI_U8 ucCH_18State  :1;    // �ŵ�18״̬ 0-æ 1-��
    HI_U8 ucCH_19State  :1;    // �ŵ�19״̬ 0-æ 1-��
    HI_U8 ucCH_20State  :1;    // �ŵ�20״̬ 0-æ 1-��
    HI_U8 ucCH_21State  :1;    // �ŵ�21״̬ 0-æ 1-��
    HI_U8 ucCH_22State  :1;    // �ŵ�22״̬ 0-æ 1-��
    HI_U8 ucCH_23State  :1;    // �ŵ�23״̬ 0-æ 1-��

    HI_U8 ucCH_24State  :1;    // �ŵ�24״̬ 0-æ 1-��
    HI_U8 ucCH_25State  :1;    // �ŵ�25״̬ 0-æ 1-��
    HI_U8 ucCH_26State  :1;    // �ŵ�26״̬ 0-æ 1-��
    HI_U8 ucCH_27State  :1;    // �ŵ�27״̬ 0-æ 1-��
    HI_U8 ucCH_28State  :1;    // �ŵ�28״̬ 0-æ 1-��
    HI_U8 ucCH_29State  :1;    // �ŵ�29״̬ 0-æ 1-��
    HI_U8 ucCH_30State  :1;    // �ŵ�30״̬ 0-æ 1-��
    HI_U8 ucCH_31State  :1;    // �ŵ�31״̬ 0-æ 1-��

    HI_U16 uSWitTime;          // �ȴ�ʱ��
}AFN_00_F1_NEW; // ȷ��

// ���ݵ�Ԫ��ϢAFN-00_F2
typedef HI_U8 AFN_00_F2;

typedef struct
{
    HI_U8 ucReadFlg;
    HI_U8 ucDataLen;         // ���ĳ���
    HI_U8 ucAttNodeNum;      // �ز��ӽڵ㸽���ڵ����
    HI_U8 reserved;

    HI_U8 *pucData;          // ��������
    HI_U8 *pucAttNodeAdd;    // �ز��ӽڵ㸽���ڵ��ַ
}AFN_14_F1_DL; // ·�����󳭶�����-����

typedef struct
{
    MRS_3762_FRAME_STRU  * pstFrame;
    HI_PBYTE             * ppucOutBuffer;// �������
    HI_U16               * pusBufferLen; // �������
    HI_PBYTE               pContent;     //3762֡�е�Ӧ��������
    HI_U16                 usContentLen; //3762֡�е�Ӧ�������򳤶�
    HI_U8                  ucAfn;        // AFN
    HI_U8                  ucFnIndex;    // FN
    HI_BOOL                ucNdmPlc;     // ������Ϣ��ndm��plc֡����
    HI_U8                  reserved[3];  // ����
}AFN_FN_UL_PARAM; // AFN_FN����ͨ�ô���֮�����Լ��

// ���ݵ�Ԫ��ϢAFN-03_F1_UP
typedef struct
{
    HI_S8    CoCode[2];      // ���̴���(ASCII)
    HI_S8    ChipCode[2];    // оƬ����(ASCII)
    HI_U8    VerDay;         // �汾���� - ��
    HI_U8    VerMonth;       // �汾���� - ��
    HI_U8    VerYear;        // �汾���� - ��
    HI_U8    reserved[1];    // ...
    HI_U16   Ver;            // �汾��
    HI_U8    reserved2[2];    // ...
}AFN_03_F1_UP; // ���̴���Ͱ汾��Ϣ-����

typedef struct
{
    AFN_14_F1_DL * pstAFN_14_F1_DL;     //����ָ��
    HI_VOID (*MRS_AFN14_FREE_FUN)(HI_VOID *);   //�ͷ��ڴ溯��
}AFN_14_F1_DL_PARAM; // AFN_14_F4���У�����Լ��

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
}afn_10_f40_data_param; // AFN_10_F40���յ������ݵ�Ԫ�ṹ

typedef afn_10_f40_data_param afn_10_f40_dl_data_param;

typedef struct
{
    afn_10_f40_data_param ul_param;
	HI_U8 id_len;
	HI_U8 id_info[MANAGEMENT_ID_SIZE]; // оƬIDΪ24�ֽڣ��豸IDΪ11�ֽڣ���оƬID���ȶ���
}afn_10_f40_ul_data_param; // AFN_10_F40�������ݵ�Ԫ�ṹ
#pragma pack()

//*****************************************************************************
// ��������: mrs3762ProtoAckFrame
// ��������: ����ȷ��֡
//
// ����˵��:
//   HI_PBYTE pucBuffer [OUT]
//
//                          ...
//
// �� �� ֵ:
//    TODO: ...
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: niesongsong/kf62735 [2011-12-13]
//*****************************************************************************
HI_U32 mrs3762ProtoAckFrame(HI_IN HI_U16 waitTime, HI_IN AFN_FN_UL_PARAM * pstDlFrameInf,HI_OUT HI_PBYTE * pucBuffer,HI_OUT HI_U16 * pusBufferLen);

//*****************************************************************************
// ��������: mrs3762ProtoDenyFrame
// ��������: ����ȷ��֡
//
// ����˵��:
//   HI_PBYTE pucBuffer [in] ...
//                          ...
//
// �� �� ֵ:
//    TODO: ...
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: niesongsong/kf62735 [2011-12-13]
//*****************************************************************************
HI_U32 mrs3762ProtoDenyFrame(HI_IN AFN_00_F2 * pstAckFrame,HI_IN AFN_FN_UL_PARAM * pstDlFrameInf,HI_OUT HI_PBYTE * pucBuffer,HI_OUT HI_U16 * pusBufferLen);
//*****************************************************************************
// ��������: mrs3762AfnDispatch
// ��������: ����AFNӦ�ò�����
//
// ����˵��:
//   ucAfn [in] ...   AFN��
//              ...
//   pstFrame [in/out] ֡
//                     ...
//
// �� �� ֵ:
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: niesongsong/kf62735 [2011-12-13]
//*****************************************************************************
HI_U32 mrs3762AfnDispatch(HI_IN MRS_3762_FRAME_STRU * pstFrame,HI_OUT HI_PBYTE * ppOutBuffer,
	HI_U16 * pusBufferLen,HI_BOOL ucNdmPlc);

//*****************************************************************************
// ��������: mrs3762FindAfnProc
// ��������: ����AFN��FN�ҵ���Ӧ��FN������
//
// ����˵��:
//   afn [in] AFN��Ӧ�ó�������
//
//   fnIndex [in] fn����
//
//
// �� �� ֵ:
//    �ɹ����ظô���ĺ���ָ��
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: niesongsong/kf62735 [2011-12-23]
//*****************************************************************************
MRS_3762_AFN_FUN mrs3762FindAfnProc(HI_U8 afn,HI_U8 fnIndex);

//AFN FN ϵ�д�����,Ŀǰ��ûʵ��
HI_U32 mrs3762_AFNXX_XX(HI_INOUT HI_VOID * param);

//AFN 00H
HI_U32 mrs3762_AFN00_F1(HI_INOUT HI_VOID * param);
HI_U32 mrs3762_AFN00_F2(HI_INOUT HI_VOID * param);

//AFN 01H
HI_U32 mrs3762_AFN01_F1(HI_INOUT HI_VOID * param);
HI_U32 mrs3762_AFN01_F2(HI_INOUT HI_VOID * param);
HI_U32 mrs3762_AFN01_F3(HI_INOUT HI_VOID * param);

/* ���ⵥ��: DTS2014010608635 BEGIN: Added by liuxipeng/lkf54842, 2014/01/07 */
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
/* ���ⵥ��: DTS2014010608635 END:   Added by liuxipeng/lkf54842, 2014/01/07 */
/* BEGIN: Modified by fengxiaomin/00209182, 2014/3/28   ���ⵥ��:DTS2014032708937 */
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

//���׵���֡����
HI_U32 mrs3762_EasyEnc(HI_U8 * pucContent,HI_U16 usLen,HI_VOID * param);

//GetFnIndex
HI_U8 mrs3762GetDtFnIndex(HI_IN HI_U8 dt[2]);

//AFN14 F1����֡�ͷ�
HI_VOID mrs3762AFN14DLFree(HI_VOID *);

// AFNF0 F1: PLC����֡
HI_U32 mrs3762_AFNF0_F1(HI_INOUT HI_VOID * param);

// AFNF0 F2: ���������֡
HI_U32 mrs3762_AFNF0_F2(HI_INOUT HI_VOID * param);
HI_U32 mrs3762_AFNF0_F229(HI_INOUT HI_VOID * param);
HI_U32 mrs3762_AFNF0_F230(HI_INOUT HI_VOID * param);
#if defined(MRS_SGD_INNER_VER)
//��ѯ����汾
HI_U32 mrs3762_AFNF0_F231(HI_INOUT HI_VOID * pParam);
#endif
// AFNF1 F1: ������������֡����
HI_U32 mrs3762_AFNF1_F1(HI_INOUT HI_VOID * param);

//*****************************************************************************
// ��������: mrsWhetherAddNode
// ��������: �ж��Ƿ�Ӧ��Ӵӽڵ�
//
// ����˵��:
//   pucContent [in] AFN11-F5�����ݵ�Ԫ
//   ucMeterNum [in] Ҫ��ӵĵ������
//   stDenyData [in/out] ����֡�еĴ���״̬��
//
// �� �� ֵ:
//   HI_ERR_SUCCESS Ӧ��Ӵӽڵ�
//   HI_ERR_BAD_DATA ��ַӳ��������ȡʧ��
//   HI_ERR_FULL �����ռ䲻�㣬�����
//   HI_ERR_INITILIZATION_ALREADY ����ظ��������
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: fengxiaomin/f00209182 [2012-06-05]
//*****************************************************************************
HI_U32 mrsWhetherAddNode(HI_U8 * pucContent, HI_U8 ucMeterNum, AFN_00_F2 *stDenyData);

//*****************************************************************************
// ��������: mrsIsExistRepeatMeter
// ��������: �ж�Ҫ��ӵı����Ƿ�����ظ����
//
// ����˵��:
//   pucContent [in] AFN11-F1�����ݵ�Ԫ
//   ucMeterIndex [in] ���
//   ucMeterNum [in] Ҫ��ӵĵ������
//
// �� �� ֵ:
//   HI_ERR_SUCCESS �������ظ����
//   HI_ERR_FAILURE �����ظ����
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: fengxiaomin/f00209182 [2012-11-02]
//*****************************************************************************
HI_U32 mrsIsExistRepeatMeter(HI_U8 * pucContent, HI_U8 ucMeterIndex, HI_U8 ucMeterNum);

//*****************************************************************************
// ��������: mrsCheckProtocolValid
// ��������: ����Լ�����Ƿ���Ч
//
// ����˵��:
//   ucProto [in] ��Լ����
//
// �� �� ֵ:
//   HI_ERR_FAILURE  ��Լ������Ч
//   HI_ERR_SUCCESS  ��Լ������Ч
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: fengxiaomin/f00209182 [2012-07-03]
//*****************************************************************************
HI_U32 mrsCheckProtocolValid(HI_U8 ucProto);

//*****************************************************************************
// ��������: mrsGetRouteFlag
// ��������: ��ȡ·����ɱ�־
//
// ����˵��:
//   HI_VOID
//
// �� �� ֵ:
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: fengxiaomin/f00209182 [2013-02-21]
//*****************************************************************************
/* BEGIN: Added by fengxiaomin/00209182, 2014/1/10   ���ⵥ�ź�:DTS2014012607684 */
HI_BOOL mrsGetRouteFlag(HI_VOID);

HI_VOID mrsInitSustainAfn(HI_VOID);
HI_U16 mrsMappedCodeVer(HI_U16 usVVer, HI_U16 usRVer, HI_U16 usSpcVer);
/* BEGIN: Modified by fengxiaomin/00209182, 2014/3/27   ���ⵥ��:DTS2014032708427 */
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
HI_U32 mrs3762_ReportAFN20_F4(HI_VOID);
HI_END_HEADER
#endif //__MRS_PROTO376_2_ECHO_H__
