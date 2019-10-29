/******************************************************************************

                  ��Ȩ���� (C), 2001-2016, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : drv_board.c
  �� �� ��   : ����
  ��    ��   : liujian 00280874
  ��������   : 2017��10��31��
  ����޸�   :
  ��������   : ��λ���GPIO�ӿ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2017��10��31��
    ��    ��   :  liujian 00280874
    �޸�����   : �����ļ�

******************************************************************************/
#include "phase_driver.h"
#include "hi_mdm_switch.h"
#include "hi_mdm_io.h"
#include "hi_mdm.h"
#include "hi_driver.h"


/*****************************************************************************
 �� �� ��  : HI_BOARD_PhaseInit
 ��������  : ����λ��ʼ��
 �������  : HI_VOID
 �������  : ��
 �� �� ֵ  : HI_VOID
 ���������õĺ���  :
 ���ñ������ĺ���  :
 ���ʵ�ȫ�ֱ���  :
 �޸ĵ�ȫ�ֱ���  :
 �޸ĵ�BBP�Ĵ����б�  :
*****************************************************************************/
HI_VOID HI_BOARD_PhaseInit(HI_VOID)
{
    //����λ��TXͨ·��ʼ��
    //�����źŵ�ƽ����

    HI_MDM_SWITCH_Off(HI_SWITCH_IDX_0);
    HI_MDM_SWITCH_Off(HI_SWITCH_IDX_1);
    HI_MDM_SWITCH_Off(HI_SWITCH_IDX_2);
    //����λ��RXͨ·��ʼ��
    //����GPIO������
    HI_MDM_IO_SetDir(RX_PHASE_A_GPIO_PIN, HI_GPIO_DIRECTION_OUT);
    HI_MDM_IO_SetDir(RX_PHASE_B_GPIO_PIN, HI_GPIO_DIRECTION_OUT);
    HI_MDM_IO_SetDir(RX_PHASE_C_GPIO_PIN, HI_GPIO_DIRECTION_OUT);
}

/*****************************************************************************
 �� �� ��  : HI_BOARD_SetTxSinglePhase
 ��������  : ����λ��TXͨ·����
 �������  : HI_U8 phase_name : ��λ��(PHASE_A��PHASE_B��PHASE_C)
             HI_U8 phase_mode : ��λ����ģʽ(PHASE_MODE_OFF��PHASE_MODE_ON)
 �������  : ��
 �� �� ֵ  : HI_VOID
 ���������õĺ���  :
 ���ñ������ĺ���  :
 ���ʵ�ȫ�ֱ���  :
 �޸ĵ�ȫ�ֱ���  :
 �޸ĵ�BBP�Ĵ����б�  :
*****************************************************************************/
HI_VOID HI_BOARD_SetTxSinglePhase(HI_U32 phase_name, HI_U8 phase_mode)
{
    if(phase_mode)
    {
        HI_MDM_SWITCH_On((HI_SWITCH_IDX_E)(phase_name-1));
    }
    else
    {
        HI_MDM_SWITCH_Off((HI_SWITCH_IDX_E)(phase_name-1));
    }
}

/*****************************************************************************
 �� �� ��  : HI_BOARD_SetRxSinglePhase
 ��������  : ����λ��RXͨ·����
 �������  : HI_U8 phase_name : ��λ��(PHASE_A��PHASE_B��PHASE_C)
             HI_U8 phase_mode : ��λ����ģʽ(PHASE_MODE_OFF��PHASE_MODE_ON)
 �������  : ��
 �� �� ֵ  : HI_VOID
 ���������õĺ���  :
 ���ñ������ĺ���  :
 ���ʵ�ȫ�ֱ���  :
 �޸ĵ�ȫ�ֱ���  :
 �޸ĵ�BBP�Ĵ����б�  :
*****************************************************************************/
HI_VOID HI_BOARD_SetRxSinglePhase(HI_U32 phase_name, HI_U8 phase_mode)
{
    HI_GPIO_VALUE_E gpio_val;

    gpio_val = (HI_GPIO_VALUE_E)((PHASE_MODE_ON == phase_mode) ?  HI_GPIO_VALUE1 : HI_GPIO_VALUE0);

    switch(phase_name)
    {
        case PHASE_A :
            HI_MDM_IO_SetOutputVal(RX_PHASE_A_GPIO_PIN, gpio_val);
            break;

        case PHASE_B :
            HI_MDM_IO_SetOutputVal(RX_PHASE_B_GPIO_PIN, gpio_val);
            break;

        case PHASE_C :
            HI_MDM_IO_SetOutputVal(RX_PHASE_C_GPIO_PIN, gpio_val);
            break;

        default :
            HI_MDM_IO_SetOutputVal(RX_PHASE_A_GPIO_PIN, gpio_val);
    }
}


