//*****************************************************************************
//
//                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mrs_cmd_msg.h
//  �� �� ��   : V1.0
//  ��    ��   : ����/00130424
//  ��������   : 2011-10-25
//  ��������   : MRS �������Ϣ����
//  �����б�   : NA
//  �޸���ʷ   :
//  1.��    �� : 2011-10-25
//    ��    �� : ����/00130424
//    �޸����� : �����ļ�
//*****************************************************************************

#ifndef __MRS_CMD_MSG_H__
#define __MRS_CMD_MSG_H__
HI_START_HEADER


//*****************************************************************************
// ��Ϣ����
// MRS��ص��ⲿ��Ϣ���ڲ��¼��綨ʱ.
//
// ��ϢID��������˵����
// [0x0000-0x0100]����ϢIDԤ����MRS����ڲ�ʹ��, ��ϢID����С��0xFFFF.
//
// shenhankun/2011-10-25
//*****************************************************************************

#define ID_MRS_MSG_PLC_CHL_STATUS       0x200   // PLCͨ��״̬
#define ID_MRS_MSG_MR_TIMER             0x201   // ��ʱ֪ͨ��Ϣ, ʹ��MRS_StartTimer������Ķ�ʱ����ʱ֪ͨ��Ϣ
#define ID_MRS_MSG_MR_DATA              0x210   // ���/������/�ɼ���������֪ͨ

#define ID_MRS_MSG_MR_DATA_645          0x210
#define ID_MRS_MSG_MR_DATA_698_45       0x211
#define ID_MRS_MSG_MR_DATA_TRANSPARENT  0x212
#define ID_MRS_MSG_MR_DATA_CJT_188  	0x213

#define ID_MRS_MSG_HSO_FRAME            0x220   // HSO֡����
#define ID_UPG_MSG_REPORT               0x230   //�ϱ�CCO�˵�STA����
//CJJ-HPLC:20180922
#define ID_RF_IQR_MSG_REPORT            0x240   //�����ж���Ϣ
#define ID_RF_WIRE_LESS_REPORT          0x250   //���߽���������Ϣ����
//CJJ-HPLC:20180922

//*****************************************************************************

#define MRS_PORT_NUM_OFFSET_IN_APP_FRAME    0   // ��Ӧ�ò㱨���У��˿ںŵ�ƫ��
#define MRS_PORT_NUM_LEN_IN_APP_FRAME       1   // ��Ӧ�ò㱨���У��˿ںŵĳ���
#define MRS_CMD_ID_OFFSET_IN_APP_FRAME      1   // ��Ӧ�ò㱨���У�����ID��ƫ��
#define MRS_CMD_ID_LEN_IN_APP_FRAME         2   // ��Ӧ�ò㱨���У�����ID�ĳ���
#define MRS_HEAD_LEN_IN_APP_FRAME           4   // ��Ӧ�ò㱨���У�����ͷ����

#define PROTO_VER_3762_2009     (0)
#define PROTO_VER_13762_2013    (1)

#define MRS_DEVICE_TYPE_COLLECTOR   DIAG_MRS_DEVICE_TYPE_COLLECTOR
#define MRS_DEVICE_TYPE_METER       DIAG_MRS_DEVICE_TYPE_METER
#define MRS_DEVICE_TYPE_BROAD_CARRIER_COMM_UNIT (5)       //Matteo
#define MRS_POWER_FAILURE_COMMUNICATE_TYPE      (4)


#define MRS_MAP_DEV_TYPE_METER      (0)
#define MRS_MAP_DEV_TYPE_CLT_I      (1)
#define MRS_MAP_DEV_TYPE_CLT_II     (2)

#define BCD2INT(x)      (HI_S32)((x)/16 * 10 + ((x) % 16))
#define UCHAR2BCD(x)    (HI_U8)((x)/10 * 16 + ((x) % 10))


typedef enum
{
    MRS_CMD_STRU_VER_R0 = 0,
    MRS_CMD_STRU_VER_R1,
    MRS_CMD_STRU_VER_R2,
    MRS_CMD_STRU_VER_R3,
} MRS_CMD_STRU_VER_E;


#define MRS_AFN13_DL_VER_R0             (MRS_CMD_STRU_VER_R0)
#define MRS_AFN13_DL_VER_R1             (MRS_CMD_STRU_VER_R1)
#define MRS_AFN13_UP_VER_R0             (MRS_CMD_STRU_VER_R0)
#define MRS_AFN13_UP_VER_R1             (MRS_CMD_STRU_VER_R1)
#define MRS_AFN13_DL_VER                (MRS_AFN13_DL_VER_R1)
#define MRS_AFN13_UP_VER                (MRS_AFN13_UP_VER_R1)

#define MRS_AFN14_DL_VER_R0             (MRS_CMD_STRU_VER_R0)
#define MRS_AFN14_DL_VER_R1             (MRS_CMD_STRU_VER_R1)
#define MRS_AFN14_UP_VER_R0             (MRS_CMD_STRU_VER_R0)
#define MRS_AFN14_UP_VER_R1             (MRS_CMD_STRU_VER_R1)
#define MRS_AFN14_DL_VER                (MRS_AFN14_DL_VER_R1)
#define MRS_AFN14_UP_VER                (MRS_AFN14_UP_VER_R1)

#define MRS_AFN_F1_DL_VER_R0            (MRS_CMD_STRU_VER_R0)
#define MRS_AFN_F1_DL_VER_R1            (MRS_CMD_STRU_VER_R1)
#define MRS_AFN_F1_UP_VER_R0            (MRS_CMD_STRU_VER_R0)
#define MRS_AFN_F1_UP_VER_R1            (MRS_CMD_STRU_VER_R1)
#define MRS_AFN_F1_DL_VER               (MRS_AFN_F1_DL_VER_R1)
#define MRS_AFN_F1_UP_VER               (MRS_AFN_F1_UP_VER_R1)

/* BEGIN: PN: DTS2016021906890 MODIFY\ADD\DEL by cuiate/00233580 at 2016/2/20 */
#define MRS_GET_DEVICE_INF_DL_VER_R0    (MRS_CMD_STRU_VER_R0)
#define MRS_GET_DEVICE_INF_DL_VER_R1    (MRS_CMD_STRU_VER_R1)
#define MRS_GET_DEVICE_INF_DL_VER_R2    (MRS_CMD_STRU_VER_R2)
#define MRS_GET_DEVICE_INF_UP_VER_R0    (MRS_CMD_STRU_VER_R0)
#define MRS_GET_DEVICE_INF_UP_VER_R1    (MRS_CMD_STRU_VER_R1)
#define MRS_GET_DEVICE_INF_UP_VER_R2    (MRS_CMD_STRU_VER_R2)
#define MRS_GET_DEVICE_INF_DL_VER       (MRS_GET_DEVICE_INF_DL_VER_R2)
#define MRS_GET_DEVICE_INF_UP_VER       (MRS_GET_DEVICE_INF_UP_VER_R2)

#define MRS_START_SEARCH_DL_VER_R0      (MRS_CMD_STRU_VER_R0)
#define MRS_START_SEARCH_DL_VER_R1      (MRS_CMD_STRU_VER_R1)
#define MRS_START_SEARCH_DL_VER         (MRS_START_SEARCH_DL_VER_R1)

#define MRS_GET_METERLIST_DL_VER_R0     (MRS_CMD_STRU_VER_R0)
#define MRS_GET_METERLIST_DL_VER_R1     (MRS_CMD_STRU_VER_R1)
#define MRS_GET_METERLIST_DL_VER_R2     (MRS_CMD_STRU_VER_R2)
#define MRS_GET_METERLIST_UP_VER_R0     (MRS_CMD_STRU_VER_R0)
#define MRS_GET_METERLIST_UP_VER_R1     (MRS_CMD_STRU_VER_R1)
#define MRS_GET_METERLIST_UP_VER_R2     (MRS_CMD_STRU_VER_R2)
#define MRS_GET_METERLIST_DL_VER        (MRS_GET_METERLIST_DL_VER_R1)
#define MRS_GET_METERLIST_UP_VER        (MRS_GET_METERLIST_UP_VER_R1)
/* END:   PN: DTS2016021906890 MODIFY\ADD\DEL by cuiate/00233580 at 2016/2/20 */

#define MRS_STOP_SEARCH_DL_VER_R0       (MRS_CMD_STRU_VER_R0)
#define MRS_STOP_SEARCH_DL_VER_R1       (MRS_CMD_STRU_VER_R1)
#define MRS_STOP_SEARCH_DL_VER          (MRS_STOP_SEARCH_DL_VER_R1)

#define MRS_BROADCAST_DL_VER_R0         (MRS_CMD_STRU_VER_R0)
#define MRS_BROADCAST_DL_VER_R1         (MRS_CMD_STRU_VER_R1)
#define MRS_BROADCAST_DL_VER            (MRS_BROADCAST_DL_VER_R1)

#define MRS_EVENT_INF_DL_VER_R0         (MRS_CMD_STRU_VER_R0)
#define MRS_EVENT_INF_DL_VER_R1         (MRS_CMD_STRU_VER_R1)
#define MRS_EVENT_INF_UP_VER_R0         (MRS_CMD_STRU_VER_R0)
#define MRS_EVENT_INF_UP_VER_R1         (MRS_CMD_STRU_VER_R1)
#define MRS_EVENT_INF_DL_VER            (MRS_EVENT_INF_DL_VER_R1)
#define MRS_EVENT_INF_UP_VER            (MRS_EVENT_INF_UP_VER_R1)

#define MRS_TEST_TRANSMIT_DL_VER_R0     (MRS_CMD_STRU_VER_R0)
#define MRS_TEST_TRANSMIT_DL_VER_R1     (MRS_CMD_STRU_VER_R1)
#define MRS_TEST_TRANSMIT_DL_VER        (MRS_TEST_TRANSMIT_DL_VER_R1)

#define MRS_TEST_AFNF0_DL_VER_R0        (MRS_CMD_STRU_VER_R0)
#define MRS_TEST_AFNF0_DL_VER_R1        (MRS_CMD_STRU_VER_R1)
#define MRS_TEST_AFNF0_UP_VER_R0        (MRS_CMD_STRU_VER_R0)
#define MRS_TEST_AFNF0_UP_VER_R1        (MRS_CMD_STRU_VER_R1)
#define MRS_TEST_AFNF0_DL_VER           (MRS_TEST_AFNF0_DL_VER_R1)
#define MRS_TEST_AFNF0_UP_VER           (MRS_TEST_AFNF0_UP_VER_R1)

#define MRS_CMD_TEST_MODE_DL_VER_R1     (MRS_CMD_STRU_VER_R1)
#define MRS_CMD_TEST_MODE_DL_VER        (MRS_CMD_TEST_MODE_DL_VER_R1)

#define MRS_CMD_TEST_MODE_UP_VER_R1     (MRS_CMD_STRU_VER_R1)
#define MRS_CMD_TEST_MODE_UP_VER        (MRS_CMD_TEST_MODE_UP_VER_R1)

#define MRS_CMD_TEST_NTB_DATA_VER       (MRS_CMD_STRU_VER_R1)


typedef struct
{
    HI_BOOL bAvail; // HI_TRUE: PLCͨ������; HI_FALSE: PLCͨ��������
    HI_U8 reserved[3];
} MRS_MSG_PLC_CHL_STATUS_STRU;

typedef struct
{
    HI_U16 usDataSizse;
    HI_U16 reserved;
    HI_PBYTE pucData; // ����������֡
} MRS_DATA_BUFFER_STRU;

typedef MRS_DATA_BUFFER_STRU MRS_MSG_MR_3762_DATA_STRU;

//
typedef struct
{
    HI_U16   usDataSizse;
    HI_U8    ucProtocol;
    HI_U8    padding[1];
    HI_PBYTE pucData; // ����������֡
} MRS_MSG_MR_645_DATA_STRU;

typedef struct
{
    HI_U8 ucTimerId; // Timer ID
    HI_U8 reserved[3];
} MRS_MSG_MR_TIMER_STRU;


//*****************************************************************************
// �����
// ע��: APP�ڲ����������ID����С�� HI_DFX_ID_BASE_MAC
//*****************************************************************************
#define ID_MRS_CMD_DATA_TRANSMIT_AFN13_PLC  (0x0001)    // ����ת������AFN-13(PLCͨ��)
#define ID_MRS_CMD_DATA_TRANSMIT_AFN14_PLC  (0x0002)    // ����ת������AFN-14(PLCͨ��)
#define ID_MRS_CMD_PARALLEL_DATA_PLC        (0x0003)    // ������������ת������(PLCͨ��)
#define ID_MRS_CMD_BROADCAST_TRANSMIT_PLC   (0x0004)    // �㲥����ת������(PLCͨ��)
#define ID_MRS_CMD_EVENT_TRANSMIT_PLC       (0x0005)    // �ӽڵ��¼��ϱ�����(PLCͨ��)
#define ID_MRS_CMD_TEST_TRANSMIT_PLC        (0x0006)    // ��������(PLCͨ��)
#define ID_MRS_CMD_TEST_TRANSMIT_AFNF0_PLC  (0x0007)    // PLCͨ������(PLCͨ��)
#define ID_MRS_CMD_EVENT_INF_PLC            (0x0008)    // �ӽڵ��¼��������(PLCͨ��)

#define ID_MRS_CMD_GET_METERADDR_PLC        (0x0010)    // ��ȡ����ַ(PLCͨ��)
#define ID_MRS_CMD_GET_METERLIST            (0x0011)    // ��ȡ�ѱ���(PLCͨ��)
#define ID_MRS_CMD_START_SEARCH_METER       (0x0012)    // ��ʼ�ѱ�����(PLCͨ��)
#define ID_MRS_CMD_STOP_SEARCH_METER        (0x0013)    // ֹͣ�ѱ�����(PLCͨ��)
#define ID_MRS_CMD_GET_DEVICE_INF_PLC       (0x0014)    // ��ȡ�豸��Ϣ(ӳ���ϵ)

#define ID_MRS_CMD_GET_POWEROFF_INFO        (0x0015)    // ��ȡվ��ͣ����Ϣ(PLCͨ��)

#define ID_MRS_CMD_IR_CMD                   (0x0020)
#define ID_MRS_CMD_NDM_CTRL_CCO_PLC         (0x0040)    // ��ά���ر�-CCO(PLCͨ��)
#define ID_MRS_CMD_RF_SET_TIME_SLOT         (0x0041)
#define ID_MRS_CMD_TF_PLC                   (0x00A1)    // ̨�������ϵʶ��(PLCͨ��)
#define ID_MRS_CMD_GET_DEVICE_ID_PLC        (0x00A2)    // ��ȡ���豸����ID(оƬID���豸ID)

#define ID_MRS_CMD_DATA_TRANSMIT_PLC        (0x0800)    // ����ת������

#define ID_MRS_CMD_DATA_TRANSMIT_TEST_MODE  (0xFFFE)
#define ID_MRS_CMD_DATA_TRANSMIT_XR         (ID_MRS_CMD_DATA_TRANSMIT_AFN13_PLC)
#define ID_MRS_CMD_DATA_TRANSMIT_LR         (ID_MRS_CMD_DATA_TRANSMIT_AFN14_PLC)

//*****************************************************************************

//#define MRS_METER_TIMER_UNIT_BASE   100   // ��������ת����STA�˳�ʱʱ�䵥λ����(�ٺ���)
//#define MRS_GAP_TIMER_UNIT_BASE     10    // ��������ת����STA��֡���ʱ�䵥λ����(ʮ����)

#define MRS_STA_STATUS_RESP_OK      0     // �������Ӧ��
#define MRS_STA_STATUS_NO_RESP      1     // �����Ӧ��
#define MRS_STA_STATUS_RESP_ERR     2     // ����쳣Ӧ��
#define MRS_STA_STATUS_QUEUE_FULL   3     // ������
#define MRS_STA_STATUS_NO_READY     4     // STA��δ����

#define MRS_PLC_DL_FLG 0    // plc֡���б�־
#define MRS_PLC_UP_FLG 1    // plc֡���б�־

#define MRS_PLC_FROM_FOLLOW     0   // ����λ: ���ԴӶ�վ
#define MRS_PLC_FROM_START      1   // ����Ϊ: ��������վ

typedef struct
{
    HI_U8 ucMeterAdd[HI_METER_ADDR_LEN];   // 6Byte-BCD
    HI_U8 relay    : 4;
    HI_U8 quality  : 4;
    HI_U8 phase    : 3;
    HI_U8 protocol : 3;
    HI_U8 bak      : 2;
}MR_NODE_METER_ADD_STRU;      // �ڵ����ַ�ṹ


typedef struct
{
    HI_U16 interface_ver : 6; // Э��汾��
    HI_U16 stru_size : 6;     // ��֧�ֲ�ͬ�汾�����ɻ�Ϊ ucOffsetSize; //ƫ�Ƶ�������ĳ���,�̶�λ��
    HI_U16 resp_status:4;     // �̶�Ϊ0
    HI_U16 protocol : 4;      // ת�����ݵĹ�Լ����(0 ~ 15)
    HI_U16 data_len : 12;     // ת�����ݳ���(0 ~ 4095)
    HI_U16 seq;               // ֡���
    HI_U8 reserved;
    HI_U8 dir : 1;            // ����λ ���� 0; ���� 1
    HI_U8 data[0];            // ת�����ݻ�����ָ��
} MRS_DATA_TRANSMIT_UP_STRU;  // ����ת�����нṹ


typedef struct
{
    HI_U16 interface_ver : 6; // Э��汾��
    HI_U16 stru_size : 6;     // ��֧�ֲ�ͬ�汾�����ɻ�Ϊ ucOffsetSize; //ƫ�Ƶ�������ĳ���,�̶�λ��
    HI_U16 pad : 4;
    HI_U16 protocol : 4;      // ת�����ݵĹ�Լ����(0 ~ 15)
    HI_U16 data_len : 12;     // ת�����ݳ���(0 ~ 4095)
    HI_U16 seq;               // ֡���
    HI_U8 time_out;           // STA�˳�ʱʱ��(100ms)(0 ~ 25500ms)
    HI_U8 dir : 1;            // ����λ ���� 0; ���� 1
    HI_U8 data[0];            // ת�����ݻ�����ָ��
} MRS_DATA_TRANSMIT_DL_STRU;  // ����ת�����нṹ

typedef MRS_DATA_TRANSMIT_UP_STRU MRS_DATA_TRANSMIT_AFN13_UP_STRU;
typedef MRS_DATA_TRANSMIT_UP_STRU MRS_DATA_TRANSMIT_AFN14_UP_STRU;
typedef MRS_DATA_TRANSMIT_DL_STRU MRS_DATA_TRANSMIT_AFN13_DL_STRU;
typedef MRS_DATA_TRANSMIT_DL_STRU MRS_DATA_TRANSMIT_AFN14_DL_STRU;


//AFNF1-F1(��������)����ת�����нṹ�� ��8���ֽڣ�
typedef struct
{
    HI_U16 interface_ver : 6;   // Э��汾��
    HI_U16 stru_size     : 6;   // �ṹ���С-�ɷ��ͷ�����-�����˽ṹ��ĳ���-���ڽ��շ��ҵ�data��λ��
    HI_U16 noresp_try_flag : 1; // δӦ�����Ա�־-��CCO����-STA�ɴ��ж�δӦ����Ƿ���Ҫ����
    HI_U16 nak_try_flag    : 1; // �������Ա�־-��CCO����-STA�ɴ��ж����յ�����֡���Ƿ���Ҫ����
    HI_U16 try_max       : 2;   // ������Դ���
    HI_U16 protocol      : 4;   // ת�����ݵĹ�Լ����(0 ~ 15)
    HI_U16 data_len      : 12;  // ת�����ݳ���(0 ~ 4095)
    HI_U16 seq;                 // ֡���
    HI_U8 time_out;             // ���ʱʱ��(100ms)(0 ~ 25500ms)
    HI_U8 gap_timer;            // ֡����(10ms)(0 ~ 2550ms)
    HI_U8 data[0];              // ת�����ݻ�����ָ��
} MRS_DATA_TRANSMIT_AFNF1_DL_STRU;


//AFNF1-F1(��������)����ת�����нṹ�� ��8���ֽڣ�
typedef struct
{
    HI_U16 interface_ver : 6;   // Э��汾��
    HI_U16 stru_size     : 6;   // �ṹ���С-�ɷ��ͷ�����-�����˽ṹ��ĳ���-���ڽ��շ��ҵ�data��λ��
    HI_U16 sta_status    : 4;   // STAӦ��״̬(����Ӧ��Ϊ0)
    HI_U16 protocol      : 4;   // ת�����ݵĹ�Լ����(0 ~ 15)
    HI_U16 data_len      : 12;  // ת�����ݳ���(0 ~ 4095)
    HI_U16 seq;                 // ֡���
    HI_U16 resp_status;         // ֡Ӧ��״̬(ÿһλ��ʶ645��֡��Ӧ��״̬)
    HI_U8 data[0];              // ת�����ݻ�����ָ��
} MRS_DATA_TRANSMIT_AFNF1_UP_STRU;

typedef struct
{
    HI_U8  addr[HI_METER_ADDR_LEN]; // ����ַ
    HI_U8  protocol;                // ��Լ����
    HI_U8  type : 4;                // ģ������(���ģ��; �ɼ���ģ��)
    HI_U8  padd : 4;
    HI_U32 tx_num;                  // ����֡����
    HI_U32 rx_num;                  // ����֡����
} MRS_CMD_METER_INF;


typedef struct
{
    HI_U8 addr[HI_METER_ADDR_LEN];  // ����ַ
    HI_U8 protocol;                 // ��Լ����
    HI_U8 type : 4;                 // ģ������(���ģ��; �ɼ���ģ��)
    HI_U8 padd : 4;
} MRS_CMD_METERLIST_ITEM;

typedef struct {
    hi_u16 interface_ver:6;        // Э��汾��
    hi_u16 stru_len : 6;
    hi_u16 reserved : 4;
    hi_u16 reserved2;
} mrs_get_device_inf_dl_ver1;

typedef struct
{
    HI_U16 interface_ver:6;        // Э��汾��
    HI_U16 stru_len : 6;
    HI_U16 reserved : 4;
    HI_U16 reserved2;
    HI_U8  src[HI_PLC_MAC_ADDR_LEN];
    HI_U8  dst[HI_PLC_MAC_ADDR_LEN];
} MRS_GET_DEVICE_INF_DL_STRU;

typedef struct
{
    HI_U16 interface_ver : 6;   // Э��汾��
    HI_U16 stru_len : 6;
    HI_U16 reserved : 4;
    HI_U8 meter_num;            // �������
    HI_U8 reserved2;
    HI_U8 meter_list[0];        // ����ַ(MRS_CMD_METER_INF)
} MRS_GET_METETADDR_UP_STRU;

typedef struct
{
    HI_U8 aucAddr[HI_METER_ADDR_LEN];
    HI_U8 ucProtocol;
    HI_U8 ucType : 4;
	HI_U8 ucStatus : 2; //STA����ӳ������״̬��1-δ���������ڱ����ѱ�0-�Ѿ����������ѱ���ɣ� ����-����
    HI_U8 reserved: 2;
} MRS_DEVICE_INF_STRU;

typedef struct
{
    HI_U8 aucAddr[HI_METER_ADDR_LEN];
    HI_U8 ucProtocol;
    HI_U8 ucType : 4;
    HI_U8 reserved: 4;
} MRS_METER_INF_STRU;

typedef struct
{
    HI_U16 interface_ver : 6;
    HI_U16 stru_len : 6;
    HI_U16 reserved : 4;
    HI_U8 meter_num;            // �������
    HI_U8 reserved2;
    MRS_DEVICE_INF_STRU stDeviceInf; //�豸��Ϣ
/* BEGIN: PN: DTS2016021906890 MODIFY\ADD\DEL by cuiate/00233580 at 2016/2/20 */
    HI_U8  src[HI_PLC_MAC_ADDR_LEN];
    HI_U8  dst[HI_PLC_MAC_ADDR_LEN];
/* END:   PN: DTS2016021906890 MODIFY\ADD\DEL by cuiate/00233580 at 2016/2/20 */
    HI_U8 meter_list[0];        // ����Ϣ(MRS_METER_INF_STRU)
} MRS_GET_DEVICE_INF_UP_STRU;

typedef enum
{
    MRS_SM_SEARCH_RESULT = 0,   // ��ѯ�ӽڵ�ע��������
    MRS_SM_START_SEARCH,        // �����ӽڵ�����ע������
    MRS_SM_BIND_NETWORK,        // ֪ͨ�ӽڵ�������������
	MRS_SM_WATER_CMD = 4,
    MRS_SM_OPTION_MAX,
} MRS_SM_OPTION_E;


/* BEGIN: PN: DTS2016021906890 MODIFY\ADD\DEL by cuiate/00233580 at 2016/2/20 */
typedef struct
{
    HI_U16 interface_ver : 6;   // Э��汾��
    HI_U16 stru_len : 6;        // �ṹ�峤��
    HI_U16 force_resp : 1;      // ǿ��Ӧ���־
    HI_U16 option : 3;          // �ѱ����(MRS_SM_OPTION_E): ��дMRS_SM_START_SEARCH
    HI_U16 reserved;
    HI_U32 seq;                 // �ѱ����к�:ÿ���ѱ����Ψһ��һ�����,�����ѱ����++
} MRS_PLC_START_SEARCH_METER_S;


typedef struct
{
    HI_U16 interface_ver : 6;   // Э��汾��
    HI_U16 stru_len : 6;        // �ṹ�峤��
    HI_U16 force_resp : 1;      // ǿ��Ӧ���־
    HI_U16 option : 3;          // �ѱ����(MRS_SM_OPTION_E): ��дMRS_SM_SEARCH_RESULT
    HI_U16 reserved;
    HI_U32 seq;                 // �ѱ����к�:ÿ���ѱ����Ψһ��һ�����,�����ѱ����++
    HI_U8  src[HI_PLC_MAC_ADDR_LEN];
    HI_U8  dst[HI_PLC_MAC_ADDR_LEN];
	HI_U8  waterMeter_Data[0]; 
} MRS_PLC_METERLIST_DL_S;
/* END:   PN: DTS2016021906890 MODIFY\ADD\DEL by cuiate/00233580 at 2016/2/20 */

// ��ȡ�ѱ�����ӳ���(����)
typedef struct
{
    HI_U16 interface_ver : 6;       // Э��汾��
    HI_U16 stru_len : 6;            // �ṹ�峤��
    HI_U16 status   : 1;            // ״̬�ֶΣ�ǿ��Ӧ��ʱʹ��
    HI_U16 option   : 3;            // ����ѡ��(MRS_SM_OPTION_E)
    HI_U8  meter_num;               // �������
    HI_U8  type;                    // 0:��� 1:�ɼ���

    HI_U8  asset[HI_METER_ADDR_LEN];// STA/�ɼ�����ַ
    HI_U8  id[HI_METER_ADDR_LEN];

    HI_U32 seq;                     // �ѱ����к�:ÿ���ѱ����Ψһ��һ�����,�������ѱ����Żش�CCO

    HI_U32  reserved;                // Ԥ��

/* BEGIN: PN: DTS2016021906890 MODIFY\ADD\DEL by cuiate/00233580 at 2016/2/20 */
    HI_U8  src[HI_PLC_MAC_ADDR_LEN];
    HI_U8  dst[HI_PLC_MAC_ADDR_LEN];
/* END:   PN: DTS2016021906890 MODIFY\ADD\DEL by cuiate/00233580 at 2016/2/20 */

    HI_U8  meter_list[0];           // ����ַ�������ݽṹ(MRS_CMD_METERLIST_ITEM)
} MRS_PLC_METERLIST_UP_S;

// ֹͣ�ѱ�(����)
typedef struct
{
    HI_U16 interface_ver : 6;       // Э��汾��
    HI_U16 stru_len : 6;            // �ṹ�峤��
    HI_U16 reserved1: 4;
    HI_U16 reserved2;
    HI_U32 seq;                     // �ѱ����к�:��д�����ѱ�����
} MRS_PLC_STOP_SEARCH_METER_S;

typedef struct
{
    HI_BOOL bDir;   // 0-��������(CCO to STA) 1-��������(STA to CCO)
    HI_BOOL bFlg;   // HI_TRUE:ȷ��; HI_FALSE:����
    HI_U16 usId;    // ��ȷ�ϻ���ϵ�ID��(0xFFFF��Ϊ������ID��)
} MRS_ID_ACK_DENY_STRU;


#define MRS_EVENT_DI_ARRAY  "\x34\x48\x33\x37"  //"\x01\x15\x00\x04"
#define MRS_EVENT_DI_LEN    (4)
#define MRS_EVENT_CTRL      (0x11)


typedef struct
{
    HI_U16 interface_ver : 6;   // Э��汾��
    HI_U16 stru_len      : 6;   // ƫ�Ƶ�������ĳ��ȣ��̶�λ��
    HI_U16 reserved      : 4;
    HI_U16 reserved1     : 4;
    HI_U16 data_len      : 12;  // ���ݳ���
    HI_U8 data[0];
} MRS_BROADCAST_STRU;

typedef enum
{
    MRS_EVENT_DIR_BIT_CCO2STA,  // CCO->STA
    MRS_EVENT_DIR_BIT_STA2CCO,  // STA->CCO
} MRS_EVENT_DIR_BIT_E;

typedef enum
{
    MRS_EVENT_FRM_BIT_TO,   // �������ԴӶ�վ
    MRS_EVENT_FRM_BIT_FROM, // ������������վ
} MRS_EVENT_FRM_BIT_E;


#define MRS_EVENT_FUN_ACK                (1)  // �¼�������: ȷ��(����)
#define MRS_EVENT_FUN_REPORT_ENABLE      (2)  // �¼�������: �����¼������ϱ�(����)
#define MRS_EVENT_FUN_REPORT_DISABLE     (3)  // �¼�������: ��ֹ�¼������ϱ�(����)
#define MRS_EVENT_FUN_CCOBUF_FULL        (4)  // �¼�������: CCO�໺����(����)

#define MRS_EVENT_FUN_REPORT             (1)  // �¼�������: �����ϱ�(����)
#define MRS_EVENT_FUN_REPORT_MODUL       (2)  //�¼�������:  �����ϱ�(����)��ģ�鴥��
#define MRS_EVENT_FUN_REPORT_CLT         (3)  // �¼�������: �����ϱ�(����)���ɼ�������

typedef struct
{
    HI_U32 ulInterfaceVer   : 6;   // Э��汾��
    HI_U32 ulStructLen      : 6;   // ƫ�Ƶ�������ĳ��ȣ��̶�λ��
    HI_U32 ulDirBit         : 1;   // ����λ: 0:CCO����STA; 1:STA����CCO
    HI_U32 ulFrmBit         : 1;   // ����λ: 0:�������ԴӶ�վ; 1:������������վ
    HI_U32 ulFunCode        : 6;   // ������:
    HI_U32 ulDataLen        : 12;  // ת�����ݳ���
    HI_U16 usSeq;                  // ֡���
    HI_U8  ucMeter[6];             // ����ַ: ����:���޶���
                                   //           ����:�����¼��ĵ���ַ
    HI_U8  ucData[0];              // ת�����ݻ�����ָ��
} MRS_EVENT_INFO_STRU;        /* ��ӦPLC ID Ϊ ID_MRS_CMD_EVENT_INF_PLC */


typedef struct
{
    HI_U16 interface_ver : 6;
    HI_U16 stru_len      : 6;
    HI_U16 timeout       : 4;
    HI_U16 protocol      : 4;
    HI_U16 data_len      : 12;
    HI_U8 data[0];
} MRS_TEST_TRANSMIT_STRU;


typedef struct
{
    HI_U16 interface_ver : 6;
    HI_U16 stru_len      : 6;
    HI_U16 reserved1     : 4;
    HI_U16 data_len;
    HI_U16 expect_len;
    HI_U16 reserved2;
    HI_U8  data[0];
} MRS_TEST_TRANSMIT_AFNF0_STRU;


typedef struct
{
    HI_U16 usCmdId;
    HI_U16 usPayloadLen;
    HI_U8 aucPayload[0];
} MRS_CMD_DATA_TRANSMIT_STRU;


typedef struct
{
    HI_U16 usInterfaceVer : 6;
    HI_U16 usStruLen : 6;
    HI_U16 usPadding1 : 4;
    HI_U8 ucSnId;
    HI_U8 ucPadding2;
} MRS_CMD_TEST_MODE_STRU;


typedef struct
{
    HI_U16 usInterfaceVer : 6;
    HI_U16 usStruLen : 6;
    HI_U16 usPadding : 4;
    HI_U16 usNtbCnt;            /* NTB���ݸ�����Ŀǰ�̶�Ϊ20 */
    HI_S16 sNtbAjustBase;       /* NTBУ����׼��CCO���͸�STA  */
    HI_U16 usPadding1;
    HI_U32 aulNtb[0];
} MRS_CMD_TEST_NTB_DATA_STRU;

#define MRS_PLC_POWEROFF_INFO_DL_MAC_NUM    (10)    // ��ѯͣ����Ϣ����һ��Я��MAC����
// ��ѯ���ͣ����Ϣ�����У�
typedef struct
{
    HI_U16 interface_ver    : 6;    // Э��汾��
    HI_U16 stru_len         : 6;    // �ṹ�峤��
    HI_U16 detect_flag      : 1;    // ̽���ʶ
    HI_U16 reserved1        : 3;
    HI_U16 sta_num          : 6;    // ��ҪӦ���վ�����
    HI_U16 reserved2        : 10;
    HI_U32 seq;                     // ��ѯ���кţ�ÿ�β�ѯ����Ψһ���к�
    HI_U8 mac_list[0];
} MRS_PLC_POWEROFF_INFO_DL_S;

// ��ѯ���ͣ����Ϣ�����У�
typedef struct
{
    HI_U16 interface_ver    : 6;    // Э��汾��
    HI_U16 stru_len         : 6;    // �ṹ�峤��
    HI_U16 reserved1        : 4;
    HI_U16 reserved2;
    HI_U32 seq;                     // ��ѯ���кţ�ÿ�β�ѯ����Ψһ���к�
    HI_U32 last_poweroff_time;      // ���һ��ͣ�緢��ʱ��
    HI_U32 last_poweron_time;       // ���һ��ͣ�����ʱ��
} MRS_PLC_POWEROFF_INFO_UP_S;

//ͣ���ϱ�����bitmap�汾�ṹ��
// ��ѯ���ͣ����Ϣ�����У�,��δʵ��
/*
typedef struct
{
HI_U16 interface_ver    : 6;     // Э��汾��
HI_U16 stru_len         : 6;     // �ṹ�峤��
HI_U16 option           : 3;    // 0̽��֡��1��ѯ֡
HI_U16 map_index        : 1;    // 0:TEIΪ0-511, 1:TEIΪ512-1023
HI_U16 reserved;
HI_U32 seq;                     // ��ѯ���кţ�ÿ�β�ѯ����Ψһ���к�
HI_U16 min_leave_len;           // ��С����ʱ��
HI_U16 min_rest_len;            // ��Сʣ��ʱ�䣨�ϱ���������
HI_U8  tei_bit_map[0];          // ����ѯվ���TEIλͼ
} MRS_PLC_POWEROFF_BITMAP_INFO_DL_S;
*/

// ��ѯ���ͣ����Ϣ�����У�
/*
typedef struct
{
HI_U16 interface_ver    : 6;     // Э��汾��
HI_U16 stru_len         : 6;     // �ṹ�峤��
HI_U16 reserved1        : 4;
HI_U16 reserved2;
HI_U32 seq;                     // ��ѯ���кţ�ÿ�β�ѯ����Ψһ���к�
HI_U32 last_poweroff_time;      // ���һ��ͣ�緢��ʱ��
HI_U32 last_poweron_time;       // ���һ��ͣ�����ʱ��
} MRS_PLC_POWEROFF_BITMAP_INFO_UP_S;
*/

#define MRS_CMD_TF_PORT_NUM                 0x11    // ̨�������ϵʶ���ĵĶ˿ں�
#define MRS_CMD_TF_NTB_COMPRESS_TIMES       8       // NTB����ѹ������
#define MRS_CMD_TF_MAX_DIFF_NUM_1EDGE       223     // ������Ϣ��֪�����У�����ʱ����֪���������ֵ
#define MRS_CMD_TF_MAX_DIFF_NUM_2EDGE       219     // ������Ϣ��֪�����У�˫��ʱ����֪���������ֵ
#define MRS_CMD_TF_DATA_NOTIFY_FIXED_LEN    4       // ������Ϣ��֪���������򣬹̶����ֳ���
#define MRS_CMD_TF_DATA_SEQ_FIXED_LEN       8       // ������Ϣ�����У��̶����ֳ���
#define MRS_CMD_TF_RESULT_IND_DATA_SIZE     10      // ̨���б�����Ϣ���������򳤶�

// ̨�������ϵʶ���ĸ�ʽ
typedef struct
{
    HI_U16 interface_ver    : 6;    // Э��汾��
    HI_U16 stru_len         : 6;    // ֡ͷ����
    HI_U16 dir              : 1;    // ����λ
    HI_U16 from             : 1;    // ����λ: 0�Ӷ���1����
    HI_U16 phase            : 2;    // ��λ
    HI_U16 seq;                     // �������

    HI_U8 mac_addr[HI_PLC_MAC_ADDR_LEN];    // MAC��ַ�������ǵ���ַ�������ڵ��ַ
    HI_U8 data_type;                // �������ͣ��μ�MRS_PLC_TF_DATA_TYPE_E
    HI_U8 option;                   // �ɼ�����(��������)���μ�MRS_PLC_TF_OPTION_E

    HI_U8 data[0];                  // ������
} MRS_PLC_TF_CMD_S;

typedef enum
{
    MRS_PLC_TF_DATA_TYPE_VOLTAGE = 1,   // ��Ƶ��ѹ
    MRS_PLC_TF_DATA_TYPE_FREQUENCY,     // ��ƵƵ��
    MRS_PLC_TF_DATA_TYPE_PERIOD,        // ��Ƶ����
} MRS_PLC_TF_DATA_TYPE_E;

typedef enum
{
    MRS_PLC_TF_OPTION_COLLECT_START = 1,    // ̨�������ɼ�������������MRS_PLC_TF_COLLECT_START_S
    MRS_PLC_TF_OPTION_DATA_COLLECT,         // ̨��������Ϣ�ռ���������Ϊ��
    MRS_PLC_TF_OPTION_DATA_NOTIFY,          // ̨��������Ϣ��֪��������MRS_CMD_TF_DATA_NOTIFY_S/MRS_CMD_TF_DATA_NOTIFY_SING_EDGE_S
    MRS_PLC_TF_OPTION_RESULT_REQ,           // ̨���б�����ѯ��������Ϊ��
    MRS_PLC_TF_OPTION_RESULT_IND,           // ̨���б�����Ϣ��������MRS_CMD_TF_RESULT_IND_S
} MRS_PLC_TF_OPTION_E;

// ̨�������ɼ����������������ʽ
typedef struct
{
    HI_U32 start_ntb;       // ��ʼNTB
    HI_U8  collect_period;  // �ɼ����ڣ���λs (�ɼ���Ƶ����������������ֶ�)
    HI_U8  collect_num;     // �ɼ�����
    HI_U8  collect_seq;     // �ɼ����кţ������ڼ��βɼ���ȡֵ��Χ0-255
    HI_U8  pad;
} MRS_PLC_TF_COLLECT_START_S;

typedef enum
{
    MRS_CMD_TF_EDGE_UNKNOEN,    // ����
    MRS_CMD_TF_EDGE_FALL,       // �½���
    MRS_CMD_TF_EDGE_RISE,       // ������
    MRS_CMD_TF_EDGE_DOUBLE,     // ˫��
} MRS_CMD_TF_COLLECT_EDGE_E;

// ��Ƶ��ѹ/Ƶ��/�����������ж���
typedef struct
{
    HI_U32 base_ntb;                // ��ʼ�ɼ�NTB

    HI_U8  pad;
    HI_U8  report_num[PHASE_CNT];   // �����߱�������
    HI_U16 value[0];                // ����ֵ
} MRS_CMD_TF_DATA_SEQ_S;

// ̨��������Ϣ��֪�����������ʽ
typedef struct
{
    HI_U16 tei          : 12;
    HI_U16 collect_edge : 2;    // �ɼ���ʽ(����)���μ�MRS_CMD_TF_COLLECT_EDGE_E�����ڹ�Ƶ����ʱ��Ч
    HI_U16 pad          : 2;
    HI_U8  collect_seq;         // �ɼ����к�
    HI_U8  total_num;           // ��֪������

    MRS_CMD_TF_DATA_SEQ_S *data_seq1;   // ������Ϣ1
    MRS_CMD_TF_DATA_SEQ_S *data_seq2;   // ������Ϣ2������Ƶ��������ʱ���ܴ���
} MRS_CMD_TF_DATA_NOTIFY_S;

typedef struct
{
    HI_U16 tei          : 12;
    HI_U16 collect_edge : 2;    // �ɼ���ʽ(����)���μ�MRS_CMD_TF_COLLECT_EDGE_E�����ڹ�Ƶ����ʱ��Ч
    HI_U16 pad1         : 2;
    HI_U8  collect_seq;         // �ɼ����к�
    HI_U8  total_num;           // ��֪������

    HI_U32 base_ntb;            // ��ʼ�ɼ�NTB

    HI_U8  pad2;
    HI_U8  notify_num[PHASE_CNT];   // �����߱�������
    HI_U16 value[0];                // ����ֵ
} MRS_CMD_TF_DATA_NOTIFY_SING_EDGE_S;

typedef struct
{
    HI_U16 tei;
    HI_U8  complete_flag;   // ̨���б���̽�����־��0δ������1����
    HI_U8  result;          // ̨��ʶ�������μ�MRS_TF_RESULT_TYPE_E

    HI_U8  belong_addr[HI_PLC_MAC_ADDR_LEN];    // ����CCO�ĵ�ַ
    HI_U16 pad;                                 // ��Э���ֶ�
} MRS_CMD_TF_RESULT_IND_S;

typedef enum
{
    MRS_TF_RESULT_TYPE_UNKNOEN = 0, // δ֪
    MRS_TF_RESULT_TYPE_CUR_NET,     // ���Ϊ��̨��
    MRS_TF_RESULT_TYPE_OTHER_NET,   // ���Ϊ�Ǳ�̨��
} MRS_TF_RESULT_TYPE_E;


#define MRS_DEV_ID_SIZE                   (11) // �豸ID��Ϣ�ĳ���

#define MRS_STA_645_READ_ID_INFO_LEN      (1)  // sta��645֡��ȡ�豸����ID��Ϣ,���ش�����Ϣ�ĳ���

#define MRS_CCO_STA_PLC_READ_DEV_ID_TIME  (5000) // ��ȡsta\I��\II���豸����ID��ʱʱ��

//Matteo
#define MRS_CCO_CHECK_ARCHIVEE_CHANGE_TIME (1000*60)//���ڼ�⵵���仯

typedef enum
{
    MRS_DEV_TYPE_NDM = 1,          // ������
    MRS_DEV_TYPE_CCO,              // CCO
    MRS_DEV_TYPE_STA,              /* �����ģ�� */
    MRS_DEV_TYPE_REP,              // �м��� Repeater
    MRS_DEV_TYPE_CLT_II,           // II�Ͳɼ���
    MRS_DEV_TYPE_CLT_I,            // I�Ͳɼ���
    MRS_DEV_TYPE_THREE_STA,        /* �����ģ�� */
    MRS_DEV_TYPE_MAX,
} mrs_dev_type_e;                // �豸����

typedef enum
{
    MRS_DEV_ID_DEFAULT,            // 0x0Ҳ��ʾģ��IDּ�ں���ʷ��չЭ�����
    MRS_DEV_ID_CHIP,               // оƬID
    MRS_DEV_ID_DEV,                // �豸ID
    MRS_DEV_ID_MAX,
} mrs_dev_id_type_e;             // �豸��ID����

typedef enum
{
    MRS_STA_READ_ID_TYPE_ERR = 1,          // ��ȡ��id���Ͳ�����
    MRS_STA_READ_ID_ADDR_ERR,              // ��ȡ�ĵ�ַ����
    MRS_STA_READ_ID_FAIL_ERR,              // ��ȡID��Ϣʧ��
    MRS_STA_READ_ID_CREATE_FREAM_ERR,      // ����Ӧ��֡ʧ��
    MRS_STA_READ_ID_SEND_FREAM_ERR,        // ����Ӧ��֡ʧ��
} mrs_sta_645_read_id_errno_e;         // sta��645֡��ȡ�豸����ID��Ϣ������

#pragma pack(1)
/* PLC��ѯ�ӽڵ����ID��Ϣ�����У� */
typedef struct {
    hi_u16 stru_ver : 6;    /**< Э��汾��,���Ǽ����ԣ����汾ȡֵ�̶�Ϊ1 */
    hi_u16 stru_len : 6;    /**< �ṹ�峤�� */
    hi_u16 dir_flag : 1;    /**< ����λ: 0-���� 1-���� */
    hi_u16 id_type  : 3;    /**< ID����: 1-оƬID 2-ģ��ID 0-ģ��ID(����) */
    hi_u16 seq;             /**< ��ѯ���кţ�ÿ�β�ѯ����Ψһ���к� */
} mrs_plc_dev_id_info_dl;

/* PLC��ѯ�ӽڵ����ID��Ϣ�����У� */
typedef struct {
    hi_u16 stru_ver : 6;    /**< Э��汾��,���Ǽ����ԣ����汾ȡֵ�̶�Ϊ1 */
    hi_u16 stru_len : 6;    /**< �ṹ�峤�� */
    hi_u16 dir_flag : 1;    /**< ����λ: 0-���� 1-���� */
    hi_u16 id_type  : 3;    /**< ID����: 1-оƬID 2-ģ��ID 0-ģ��ID(����) */
    hi_u16 seq;             /**< ��ѯ���кţ�ÿ�β�ѯ����Ψһ���к� */
	hi_u8  id_len;          /**< ID���� */
	hi_u8  id_info[MANAGEMENT_ID_SIZE+1];   /**< оƬIDΪ24�ֽڣ��豸IDΪ11�ֽڣ���оƬID���ȶ���, + 1�ֽ��豸���� */
} mrs_plc_dev_id_info_up;
#pragma pack()
typedef struct
{
    HI_U16 interface_ver : 6;   // Э��汾��
    HI_U16 stru_len : 6;        // �ṹ�峤��
    HI_U16 option : 4;          // ����
    HI_U32 seq;                 // ���к�
    HI_U16 usDataLen;				//���ݳ���
    HI_U16 usTimeSlot;				//ʱ϶��
    HI_U16 usMaxTei;				//���TEI
} MRS_PLC_SET_TIME_SLOT_S;

HI_END_HEADER
#endif // __MRS_CMD_MSG_H__

