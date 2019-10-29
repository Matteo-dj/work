//*****************************************************************************
//
//                  ��Ȩ���� (C), 1998-2011, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mrs_srv_baudrate_manage.h
//  �� �� ��   : V1.0 
//  ��    ��   : liuxipeng/KF54842
//  ��������   : 2014-06-14
//  ��������   : �����ʹ���
//               �����������궨��
//               
//  �����б�   : 
//  �޸���ʷ   : 
//  1.��    �� : 2014-06-14
//    ��    �� : liuxipeng/KF54842
//    �޸����� : �����ļ� 
//
//*****************************************************************************

#ifndef __MRS_BAUDRATE_MANAGE_H__
#define __MRS_BAUDRATE_MANAGE_H__
HI_START_HEADER

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)

#define MRS_SRV_RATE_MAX                (5)      // ÿһ������������֧�ֵ�������ʸ���

#define MRS_SRV_BAUD_RATE_300           (300)
#define MRS_SRV_BAUD_RATE_600           (600)
#define MRS_SRV_BAUD_RATE_1200          (1200)
#define MRS_SRV_BAUD_RATE_2400          (2400)
#define MRS_SRV_BAUD_RATE_4800          (4800)
#define MRS_SRV_BAUD_RATE_7200          (7200)
#define MRS_SRV_BAUD_RATE_19200         (19200)
#define MRS_SRV_BAUD_RATE_38400         (38400)

#define MRS_SRV_BAUD_RATE_DEFAULT       MRS_SRV_BAUD_RATE_2400
#define MRS_SRV_BAUD_RATE_IDX_DEFAULT   (3)

#define MRS_UART_BYTE_BIT                       (1 + 8 + 1 + 1) // 1��ʼλ + 8����λ + 1У��λ + 1ֹͣλ
#define MRS_GET_UART_SEND_TIME(BaudRate, Len)   ((MRS_UART_BYTE_BIT * 1000 * (Len) + (BaudRate) - 1) / (BaudRate))                 

typedef enum
{
    MRS_SRV_ID_BAUDRATE_SM_97 = 0,      // ����������ID-�ѱ�97��Լ
    MRS_SRV_ID_BAUDRATE_SM_07,          // ����������ID-�ѱ�07��Լ
    MRS_SRV_ID_BAUDRATE_SM_698,         // ����������ID-�ѱ�698.45��Լ
    MRS_SRV_ID_BAUDRATE_RM_97,          // ����������ID-����97��Լ
    MRS_SRV_ID_BAUDRATE_RM_07,          // ����������ID-����07��Լ
    MRS_SRV_ID_BAUDRATE_RM_698,        // ����������ID-����698.45��Լ
    MRS_SRV_ID_BAUDRATE_RM_RSV1,        // ����������ID-������1
    MRS_SRV_ID_BAUDRATE_ADJUST_TIME,    // ����������ID-Уʱ
    MRS_SRV_ID_BAUDRATE_DETECT,         // ����������ID-�ϵ粨����̽��
    MRS_SRV_ID_BAUDRATE_MAX             // ����������ID-������
} MRS_SRV_BAUDRATE_ID_E;

typedef struct
{
    HI_U16 usBaudRateList[MRS_SRV_RATE_MAX];
    HI_U8  ucValidNum;
    HI_U8  padding;
} MRS_SRV_BAUDRATE_CFG_S, *P_MRS_SRV_BAUDRATE_CFG_S;


// ����NV���ó�ʼ�����������ñ�
HI_PUBLIC HI_U32 mrsSrvInitBaudRateCfgTable(HI_U16);

// ��ȡָ��ID�Ĳ���������
// ���ؿ�ָ�벻��ʹ��
// ������ͨ������ֵ�����ж������Ƿ�Ϸ�
// ����������������ʸ���Ϊ0��˵����Ӧ��ID�Ĳ�����������Ч
HI_PUBLIC P_MRS_SRV_BAUDRATE_CFG_S mrsSrvGetBaudRateCfg(MRS_SRV_BAUDRATE_ID_E);

// �����ʺ������Ż�ת����
// �ɹ�����HI_ERR_SUCCESS ʧ�ܷ���HI_ERR_FAILURE
// �����Ͳ����ʶ�Ӧ��ϵ:
//  1: MRS_SRV_BAUD_RATE_600
//  2: MRS_SRV_BAUD_RATE_1200
//  3: MRS_SRV_BAUD_RATE_2400
//  4: MRS_SRV_BAUD_RATE_4800
//  5: MRS_SRV_BAUD_RATE_9600
//  6: MRS_SRV_BAUD_RATE_19200
//  7: MRS_SRV_BAUD_RATE_38400
HI_PUBLIC HI_U32 mrsSrvBaudRate2Index(HI_IN HI_U16 baudrate, HI_OUT HI_U8 *index);
HI_PUBLIC HI_U32 mrsSrvIndex2BaudRate(HI_IN HI_U8 index, HI_OUT HI_U16 *baudrate);

#endif

HI_END_HEADER
#endif


