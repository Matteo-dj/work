#ifndef _TF_DRIVER_H_
#define _TF_DRIVER_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include "hi_types.h"
#include "hi_mdm_io.h"

#define ZERO_CROSS_IN_GPIO_PIN          HI_GPIO_IDX_6                //�����ж�����GPIO

//̨��ʶ����ջ��ĵ�λ�л�ģʽ
#define TF_RCV_MODE_AUTO                    0x00    // �Զ��л�ģʽ
#define TF_RCV_MODE_240K                    0x01    // �̶�Ϊ240K����ģʽ
#define TF_RCV_MODE_1M                      0x02    // �̶�Ϊ1M����ģʽ

 //NDM������ģʽ
#define TF_SEND_EDGE_MODE_OFF                               0           //�����ؿ��عر�
#define TF_SEND_EDGE_MODE_UP                                1           //�����ط���ģʽ
#define TF_SEND_EDGE_MODE_DOWN                              2           //�½��ط���ģʽ

#define TF_TX_OUT_DATA_PIN                    HI_GPIO_IDX_20           //���Ͷ˵��������GPIO��GPIO20
#define TF_NDM_ZERO_DTC_RX_PIN        IO_EXP_IO_NUM_13     // ���ջ����չܽ�
#define TF_NDM_BEEP_PIN                           IO_EXP_IO_NUM_14    // ���ջ��������ܽ�
#define TF_NDM_PHASE_A_PIN                    IO_EXP_IO_NUM_00    // ���������A������ָʾ��
#define TF_NDM_PHASE_B_PIN                    IO_EXP_IO_NUM_01    // ���������B��λ����ָʾ��
#define TF_NDM_PHASE_C_PIN                    IO_EXP_IO_NUM_05    // ���������C��λ����ָʾ��
#define TF_NDM_AN_CHECK_PIN                IO_EXP_IO_NUM_06    // ������ϵ���AN��ѹ����չIO0_6�ܽ�
#define TF_NDM_AN_CHECK_READ_PIN   IO_EXP_IO_NUM_07   // ������ϵ���AN��ѹ����ȡ�ź���չIO0_7�ܽ�
#define TF_NDM_TX_ERR_LED_PIN            IO_EXP_IO_NUM_12    // ���������ָʾ�ƣ���չIO1_2�ܽ�


typedef struct
{
    HI_U32 gpio_direction;                     // GPIO�����룬�������
    HI_GPIO_VALUE_E gpio_value;                //GPIO �ĸߵ͵�ƽ
}DRV_GPIO_VAL_AND_DIREC_STRU;

//̨��ʶ��NDMģʽ
typedef enum
{
    TF_IDENTIFY_NORMAL_MODE = 0,
    TF_IDENTIFY_RCV_MODE,
    TF_IDENTIFY_SEND_MODE,
    TF_IDENTIFY_LCD_OFF_MODE,
}TF_IDENTIFY_MODE_E;
HI_VOID HI_BOARD_TfGpioInit(HI_VOID);
HI_VOID HI_BOARD_ZeroCrossInGpioInit(HI_VOID);
/*****************************************************************************
 �� �� ��  : HI_BOARD_SetTfTxOutDataGpioInit
 ��������  : GPIO20�ܽų�ʼ��������͵�ƽ
 �������  : HI_VOID
 �������  : ��
 �� �� ֵ  : HI_VOID
 ���������õĺ���  :
 ���ñ������ĺ���  :
 ���ʵ�ȫ�ֱ���  :
 �޸ĵ�ȫ�ֱ���  :
 �޸ĵ�BBP�Ĵ����б�  :

 �޸���ʷ      :
  1.��    ��   : 2016��10��17��
    ��    ��   : sunhaiyang/00238140
    �޸�����   : �����ɺ���

*****************************************************************************/
HI_EXTERN HI_VOID HI_BOARD_SetTfTxOutDataGpioInit(HI_VOID);
/*****************************************************************************
 �� �� ��  : HI_BOARD_SetTfSendEdgeModeUpGpioInit
 ��������  : ��GPIO9�ܽų�ʼ��Ϊ�͵�ƽ�����
 �������  : ��
 �������  : ��
 �� �� ֵ  : HI_VOID
 ���������õĺ���  :
 ���ñ������ĺ���  :
 ���ʵ�ȫ�ֱ���  :
 �޸ĵ�ȫ�ֱ���  :
 �޸ĵ�BBP�Ĵ����б�  :
*****************************************************************************/
HI_EXTERN HI_VOID HI_BOARD_SetTfSendEdgeModeUpGpioInit(HI_VOID);

/*****************************************************************************
 �� �� ��  : HI_BOARD_SetTfSendEdgeModeDownGpioInit
 ��������  :��GPIO10�ܽų�ʼ��Ϊ�͵�ƽ�����
 �������  : ��
 �������  : ��
 �� �� ֵ  : HI_VOID
 ���������õĺ���  :
 ���ñ������ĺ���  :
 ���ʵ�ȫ�ֱ���  :
 �޸ĵ�ȫ�ֱ���  :
 �޸ĵ�BBP�Ĵ����б�  :
*****************************************************************************/
HI_EXTERN HI_VOID HI_BOARD_SetTfSendEdgeModeDownGpioInit(HI_VOID);
HI_VOID HI_BOARD_SetSwitchLevel(HI_U8 level);
HI_VOID HI_BOARD_SetSendEdgeMode(HI_U8 edge_mode);
/*****************************************************************************
 �� �� ��  : HI_BOARD_SetPhaseGpioMode
 ��������  : ��ȡGPIO2,3,4��ģʽ
 �������  : HI_VOID
 �������  : ��
 �� �� ֵ  : HI_VOID
 ���������õĺ���  :
 ���ñ������ĺ���  :
 ���ʵ�ȫ�ֱ���  :
 �޸ĵ�ȫ�ֱ���  :
 �޸ĵ�BBP�Ĵ����б�  :
*****************************************************************************/
HI_EXTERN HI_VOID HI_BOARD_SetPhaseGpioMode(HI_VOID);
/*****************************************************************************
 �� �� ��  : HI_BOARD_SetPhaseGpioDirecInit
 ��������  : ����GPIO2,3,4Ϊ��ʼ��Ϊ���
 �������  : HI_VOID
 �������  : ��
 �� �� ֵ  : HI_VOID
 ���������õĺ���  :
 ���ñ������ĺ���  :
 ���ʵ�ȫ�ֱ���  :
 �޸ĵ�ȫ�ֱ���  :
 �޸ĵ�BBP�Ĵ����б�  :
*****************************************************************************/
HI_EXTERN HI_VOID HI_BOARD_SetPhaseGpioDirecInit(HI_VOID);
/*****************************************************************************
 �� �� ��  : HI_BOARD_SetPhaseGpioValue
 ��������  : ���CCO����Ӳ�����ܣ���������λ���أ��򿪻��߹ر�
 �������  : HI_IN HI_U8 phase_mode
 �������  : ��
 �� �� ֵ  : HI_VOID
 ���������õĺ���  :
 ���ñ������ĺ���  :
 ���ʵ�ȫ�ֱ���  :
 �޸ĵ�ȫ�ֱ���  :
 �޸ĵ�BBP�Ĵ����б�  :
*****************************************************************************/
HI_EXTERN HI_VOID HI_BOARD_SetPhaseGpioValue(HI_IN HI_U8 phase_mode);
/*****************************************************************************
 �� �� ��  : HI_BOARD_SetSinglePhaseGpioValue
 ��������  : ���õ�����λGPIO�ܽŵĸߵ͵�ƽ
 �������  : HI_IN HI_U32 gpio_pin,HI_IN HI_U8 phase_mode
 �������  : ��
 �� �� ֵ  : HI_VOID
 ���������õĺ���  :
 ���ñ������ĺ���  :
 ���ʵ�ȫ�ֱ���  :
 �޸ĵ�ȫ�ֱ���  :
 �޸ĵ�BBP�Ĵ����б�  :
*****************************************************************************/
HI_EXTERN HI_VOID HI_BOARD_SetSinglePhaseGpioValue(HI_IN HI_GPIO_IDX_E gpio_pin,HI_IN HI_U8 phase_mode);
/*****************************************************************************
 �� �� ��  : HI_BOARD_SetBeepValue
 ��������  :���ý��ջ���������ͣ
 �������  : HI_BOOL onoff
 �������  : ��
 �� �� ֵ  : HI_U32
 ���������õĺ���  :
 ���ñ������ĺ���  :
 ���ʵ�ȫ�ֱ���  :
 �޸ĵ�ȫ�ֱ���  :
 �޸ĵ�BBP�Ĵ����б�  :
*****************************************************************************/
HI_EXTERN HI_U32 HI_BOARD_SetBeepValue(HI_BOOL onoff);
/*****************************************************************************
 �� �� ��  : HI_BOARD_SetBeepGpioInit
 ��������  :��ʼ��������Ϊ������ߵ�ƽ������
 �������  : HI_VOID
 �������  : ��
 �� �� ֵ  : HI_U32
 ���������õĺ���  :
 ���ñ������ĺ���  :
 ���ʵ�ȫ�ֱ���  :
 �޸ĵ�ȫ�ֱ���  :
 �޸ĵ�BBP�Ĵ����б�  :
*****************************************************************************/
HI_EXTERN HI_U32 HI_BOARD_SetBeepGpioInit(HI_VOID);
/*****************************************************************************
 �� �� ��  : drv_set_ndm_zero_dtc_rx_init
 ��������  :��ʼ�����ջ����չܽ�Ϊ������͵�ƽ
 �������  : HI_VOID
 �������  : ��
 �� �� ֵ  : HI_U32
 ���������õĺ���  :
 ���ñ������ĺ���  :
 ���ʵ�ȫ�ֱ���  :
 �޸ĵ�ȫ�ֱ���  :
 �޸ĵ�BBP�Ĵ����б�  :
*****************************************************************************/
HI_EXTERN HI_U32 HI_BOARD_SetNdmZeroDtcRxInit(HI_VOID);
/*****************************************************************************
 �� �� ��  : HI_BOARD_SetNdmZeroDtcRxValue
 ��������  :���ý��ջ����չܽŸߵ͵�ƽ
 �������  : HI_BOOL gpio_value
 �������  : ��
 �� �� ֵ  : HI_U32
 ���������õĺ���  :
 ���ñ������ĺ���  :
 ���ʵ�ȫ�ֱ���  :
 �޸ĵ�ȫ�ֱ���  :
 �޸ĵ�BBP�Ĵ����б�  :
*****************************************************************************/
HI_EXTERN HI_U32 HI_BOARD_SetNdmZeroDtcRxValue(HI_BOOL gpio_value);
/*****************************************************************************
 �� �� ��  : HI_BOARD_SetAnCheckClearGpioInit
 ��������  :�ϵ����Ƿ��ѹ������ѹ��ʶǰ��ͨ�������������ʶ
 �������  : HI_VOID
 �������  : ��
 �� �� ֵ  : HI_VOID
 ���������õĺ���  :
 ���ñ������ĺ���  :
 ���ʵ�ȫ�ֱ���  :
 �޸ĵ�ȫ�ֱ���  :
 �޸ĵ�BBP�Ĵ����б�  :
*****************************************************************************/
HI_EXTERN HI_VOID HI_BOARD_SetAnCheckClearGpioInit(HI_VOID);
/*****************************************************************************
 �� �� ��  : HI_BOARD_SetAnCheckClearGpioValue
 ��������  :�ϵ����Ƿ��ѹ������ѹ��ʶǰ��ͨ�������������ʶ
 �������  : HI_BOOL gpio_value
 �������  : ��
 �� �� ֵ  : HI_VOID
 ���������õĺ���  :
 ���ñ������ĺ���  :
 ���ʵ�ȫ�ֱ���  :
 �޸ĵ�ȫ�ֱ���  :
 �޸ĵ�BBP�Ĵ����б�  :

 �޸���ʷ      :
  1.��    ��   : 2015��11��23��
    ��    ��   : sunhaiyang/00238140
    �޸�����   : �����ɺ���

*****************************************************************************/
HI_EXTERN HI_VOID HI_BOARD_SetAnCheckClearGpioValue(HI_BOOL gpio_value);
/*****************************************************************************
 �� �� ��  : HI_BOARD_SetAnCheckGpioInit
 ��������  :�ϵ����Ƿ��ѹ����ȡ��ѹ�źŵĹܽţ���ʼ��Ϊ����
 �������  : HI_VOID
 �������  : ��
 �� �� ֵ  : HI_VOID
 ���������õĺ���  :
 ���ñ������ĺ���  :
 ���ʵ�ȫ�ֱ���  :
 �޸ĵ�ȫ�ֱ���  :
 �޸ĵ�BBP�Ĵ����б�  :
*****************************************************************************/
HI_EXTERN HI_VOID HI_BOARD_SetAnCheckGpioInit(HI_VOID);
/*****************************************************************************
 �� �� ��  : drv_get_an_check_value
 ��������  :�ϵ����Ƿ��ѹ����ȡ��ѹ�źŵĹܽŵĵ�ƽ
 �������  : HI_VOID
 �������  : ��
 �� �� ֵ  : HI_U8
 ���������õĺ���  :
 ���ñ������ĺ���  :
 ���ʵ�ȫ�ֱ���  :
 �޸ĵ�ȫ�ֱ���  :
 �޸ĵ�BBP�Ĵ����б�  :
*****************************************************************************/
HI_EXTERN HI_U8 HI_BOARD_GetAnCheckValue(HI_VOID);
HI_U32 HI_BOARD_SetOutSeqData(HI_U8 send_bit);
HI_VOID HI_BOARD_SetPhaseSwitchCtrlMode(HI_VOID);

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
HI_VOID HI_BOARD_SetCcoZeroCrossEnable(HI_U8 power_edge_type, HI_U8 phase);
#endif

/*****************************************************************************
 �� �� ��  : HI_BOARD_SetThreeMeterStaGpioMode
 ��������  : ���������STAģ�����GPIO�ܽ�ģʽ
 �������  : HI_VOID
 �������  : ��
 �� �� ֵ  : HI_VOID
 ���������õĺ���  :
 ���ñ������ĺ���  :
 ���ʵ�ȫ�ֱ���  :
 �޸ĵ�ȫ�ֱ���  :
 �޸ĵ�BBP�Ĵ����б�  :
*****************************************************************************/
HI_VOID HI_BOARD_SetThreeMeterStaGpioMode(HI_VOID);

/*****************************************************************************
 �� �� ��  : HI_BOARD_SetThreeMeterStaGpioDirAndValueInit
 ��������  : ���������STAģ����Ʒ���͸ߵ͵�ƽ��ʼ��
 �������  : HI_VOID
 �������  : ��
 �� �� ֵ  : HI_VOID
 ���������õĺ���  :
 ���ñ������ĺ���  :
 ���ʵ�ȫ�ֱ���  :
 �޸ĵ�ȫ�ֱ���  :
 �޸ĵ�BBP�Ĵ����б�  :
*****************************************************************************/
HI_VOID HI_BOARD_SetThreeMeterStaGpioDirAndValueInit(HI_VOID);
#ifdef __cplusplus
 }
#endif

#endif
