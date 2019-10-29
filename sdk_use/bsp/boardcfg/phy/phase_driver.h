#ifndef _PHASE_DRIVER_H_
#define _PHASE_DRIVER_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include "hi_types.h"


//����λ����ģʽ
#define PHASE_MODE_OFF                  0                   //��λ�ر�
#define PHASE_MODE_ON                   1                   //��λ��

//����λ��RXͨ·����
//����λ���ض�Ӧ��GPIO�ܽ�
#define RX_PHASE_A_GPIO_PIN             HI_GPIO_IDX_17
#define RX_PHASE_B_GPIO_PIN             HI_GPIO_IDX_18
#define RX_PHASE_C_GPIO_PIN             HI_GPIO_IDX_19


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
HI_EXTERN HI_VOID HI_BOARD_PhaseInit(HI_VOID);

/*****************************************************************************
 �� �� ��  : drv_set_tx_single_phase
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
HI_EXTERN HI_VOID HI_BOARD_SetTxSinglePhase(HI_U32 phase_name, HI_U8 phase_mode);
/*****************************************************************************
 �� �� ��  : drv_set_rx_single_phase
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
HI_EXTERN HI_VOID HI_BOARD_SetRxSinglePhase(HI_U32 phase_name, HI_U8 phase_mode);

#ifdef __cplusplus
 }
#endif

#endif

