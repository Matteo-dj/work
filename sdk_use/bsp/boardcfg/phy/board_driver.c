/******************************************************************************

                  ��Ȩ���� (C), 2001-2016, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : drv_board.c
  �� �� ��   : ����
  ��    ��   : liujian 00280874
  ��������   : 2017��10��31��
  ����޸�   :
  ��������   : �����Χ��·���GPIO�ӿ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2017��10��31��
    ��    ��   :  liujian 00280874
    �޸�����   : �����ļ�

******************************************************************************/
#include "board_driver.h"
#include "hi_mdm_io.h"
#include "hi_mdm_types.h"
#include "hi_sal.h"
#include "hi_driver.h"

HI_MDM_BOARD_CTRL_INFO_S g_BoardCtrlInfo = {0};

HI_MDM_BOARD_CTRL_INFO_S* HI_MDM_GetBoardCtrlInfo(HI_VOID)
{
    return &g_BoardCtrlInfo;
}

HI_VOID HI_MDM_SetBoardCtrlInfoSysStatus(HI_U32 SysStatus)
{
    HI_MDM_BOARD_CTRL_INFO_S* p_BoardCtrlInfo = HI_MDM_GetBoardCtrlInfo();

    p_BoardCtrlInfo->SysStatus = SysStatus;
}

HI_VOID HI_MDM_GpioInit(HI_VOID)
{
    HI_GPIO_IDX_E sta_gpio = HI_GPIO_IDX_0;
    HI_MDM_BOARD_CTRL_INFO_S* p_BoardCtrlInfo = HI_MDM_GetBoardCtrlInfo();

    //�ڹ�װģʽ�£������������ܽŽ��г�ʼ��
    if(HI_ND_SYS_STATUS_TEST == p_BoardCtrlInfo->SysStatus)
    {
        return;
    }

    //��STA��GPIO7�ܽų�ʼ��Ϊ�͵�ƽ
    if(HI_TRUE == HI_MDM_IsNeedControlStaPin())
    {
        #ifdef PRODUCT_CFG_BOARD_TYPE_ASIC_V200
        sta_gpio = HI_GPIO_IDX_16;
        #else
        sta_gpio = HI_GPIO_IDX_7;
        #endif

        HI_MDM_IO_SetDir(sta_gpio, HI_GPIO_DIRECTION_OUT);
        HI_MDM_IO_SetOutputVal(sta_gpio, HI_GPIO_VALUE1);
    }
}

/*****************************************************************************
�� �� ��  : HI_MDM_ControlCircuitBeforeSend
��������  : ����ǰ����GPIO�ܽŵ�ƽ
�������  : SendLength ���ķ��ͳ���
�������  : ��
�� �� ֵ  : ������
���������õĺ���  :
���ñ������ĺ���  :
���ʵ�ȫ�ֱ���  :
�޸ĵ�ȫ�ֱ���  :
�޸ĵ�BBP�Ĵ����б�  :
*****************************************************************************/
HI_VOID HI_MDM_ControlCircuitBeforeSend(HI_IN HI_U32 SendLength)
{
    #ifdef PRODUCT_CFG_VERSION_RELEASE
    // ���ķ���ʱ�����ڵ���5ms,���ܽŵ�ƽ����
    if(FRAME_SEND_LENGTH_TH <= SendLength)
    {
        HI_MDM_GetBoardCtrlInfo()->StaPinFlag = HI_TRUE;
        HI_MDM_StaPinControl(PIN_HIGH);
    }
    #endif
}

/*****************************************************************************
�� �� ��  : HI_MDM_ControlCircuitAfterSend
��������  : ���ͺ�����GPIO�ܽŵ�ƽ
�������  : HI_VOID
�������  : ��
�� �� ֵ  : ������
���������õĺ���  :
���ñ������ĺ���  :
���ʵ�ȫ�ֱ���  :
�޸ĵ�ȫ�ֱ���  :
�޸ĵ�BBP�Ĵ����б�  :
*****************************************************************************/
HI_VOID HI_MDM_ControlCircuitAfterSend(HI_VOID)
{
    // û�з��ͳɹ�����STA�ܽ�GPIO7��Ϊ�͵�ƽ
    #ifdef PRODUCT_CFG_VERSION_RELEASE
    HI_MDM_BOARD_CTRL_INFO_S* p_BoardCtrlInfo = HI_MDM_GetBoardCtrlInfo();
    if(HI_TRUE == p_BoardCtrlInfo->StaPinFlag)
    {
        HI_MDM_StaPinControl(PIN_LOW);
        p_BoardCtrlInfo->StaPinFlag = HI_FALSE;
    }
    #endif
}

/*****************************************************************************
�� �� ��  : HI_MDM_StaPinControl
��������  : STAվ��ͨ��GPIO7���ƹܽŵĵ�ƽ�ߵ�
�������  : state ��ƽ״̬

�������  : ��
�� �� ֵ  : ������
���������õĺ���  :
���ñ������ĺ���  :
���ʵ�ȫ�ֱ���  :
�޸ĵ�ȫ�ֱ���  :
�޸ĵ�BBP�Ĵ����б�  :
*****************************************************************************/
HI_VOID HI_MDM_StaPinControl(en_pin_state State)
{
    HI_GPIO_IDX_E StaGpio = HI_GPIO_IDX_0;
    HI_MDM_BOARD_CTRL_INFO_S* p_BoardCtrlInfo = HI_MDM_GetBoardCtrlInfo();

    // �ڹ�װģʽ�£�������GPIO7
    if(HI_ND_SYS_STATUS_TEST == p_BoardCtrlInfo->SysStatus)
    {
        return;
    }

    if(HI_FALSE == HI_MDM_IsNeedControlStaPin())
    {
        return;
    }

    #ifdef PRODUCT_CFG_BOARD_TYPE_ASIC_V200
    StaGpio = HI_GPIO_IDX_16;
    #else
    StaGpio = HI_GPIO_IDX_7;
    #endif

    switch(State)
    {
        case PIN_HIGH :
            HI_MDM_IO_SetOutputVal(StaGpio,HI_GPIO_VALUE0);
            break;
        case PIN_LOW:
            HI_MDM_IO_SetOutputVal(StaGpio,HI_GPIO_VALUE1);
            break;
        default:
            break;
    }
}

/*****************************************************************************
�� �� ��  : HI_MDM_IsNeedControlStaPin
��������  : �ж��Ƿ���Ҫ���ƹܽŵ�ƽ
�������  : ��

�������  : ��
�� �� ֵ  : ������
���������õĺ���  :
���ñ������ĺ���  :
���ʵ�ȫ�ֱ���  :
�޸ĵ�ȫ�ֱ���  :
�޸ĵ�BBP�Ĵ����б�  :
*****************************************************************************/
HI_BOOL HI_MDM_IsNeedControlStaPin(HI_VOID)
{
    HI_U8 product_type = HI_ND_TYPE_STA;

    product_type = HI_DMS_LoadDevType();
    if((HI_ND_TYPE_STA != product_type) && (HI_ND_TYPE_THREE_STA != product_type))
    {
        return HI_FALSE;
    }

    return HI_TRUE;
}

