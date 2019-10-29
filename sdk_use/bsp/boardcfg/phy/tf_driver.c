/******************************************************************************

                  ��Ȩ���� (C), 2001-2016, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : drv_board.c
  �� �� ��   : ����
  ��    ��   : liujian 00280874
  ��������   : 2017��10��31��
  ����޸�   :
  ��������   : ̨��ʶ�����GPIO�ӿ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2017��10��31��
    ��    ��   :  liujian 00280874
    �޸�����   : �����ļ�

******************************************************************************/
#include "tf_driver.h"
#include "hi_mdm.h"
#include "phase_driver.h"
#include "hi_ioexp.h"
#include "hi_sal.h"
#include "led_driver.h"
#include <hi_lcd.h>
#include <hi_mdm_tf.h>
/*****************************************************************************
 �� �� ��  : HI_BOARD_TfGpioInit
 ��������  : ̨��ʶ�����GPIO��ʼ��
 �������  : HI_VOID
 �������  : ��
 �� �� ֵ  : HI_VOID
 ���������õĺ���  :
 ���ñ������ĺ���  :
 ���ʵ�ȫ�ֱ���  :
 �޸ĵ�ȫ�ֱ���  :
 �޸ĵ�BBP�Ĵ����б�  :
*****************************************************************************/
HI_VOID HI_BOARD_TfGpioInit(HI_VOID)
{

    #if defined(PRODUCT_CFG_PRODUCT_TYPE_NDM)
    //̨��ʶ��GPIO��ʼ��
    HI_BOARD_ZeroCrossInGpioInit();
    #endif

    #if defined(PRODUCT_CFG_PRODUCT_TYPE_NDM)
    if(HI_TRUE == HI_DMS_IsTfHardWare())
    {
        led_tf_transmitter_led_init();
        HI_BOARD_SetTfSendEdgeModeUpGpioInit();
        HI_BOARD_SetTfSendEdgeModeDownGpioInit();
        HI_BOARD_SetTfTxOutDataGpioInit();
        HI_BOARD_SetAnCheckClearGpioInit();
        HI_BOARD_SetAnCheckGpioInit();
    }
    HI_BOARD_SetBeepGpioInit();
    #endif
}

/*****************************************************************************
 HI_BOARD_ZeroCrossInGpioInit
 ��������  : ̨��ʶ��GPIO��ʼ��
 �������  : HI_VOID
 �������  : ��
 �� �� ֵ  : HI_VOID
 ���������õĺ���  :
 ���ñ������ĺ���  :
 ���ʵ�ȫ�ֱ���  :
 �޸ĵ�ȫ�ֱ���  :
 �޸ĵ�BBP�Ĵ����б�  :
*****************************************************************************/
HI_VOID HI_BOARD_ZeroCrossInGpioInit(HI_VOID)
{
    // ̨��ʶ���ǣ�STA�˹�������ʼ��
    //GPIOģʽ����
    HI_MDM_IO_SetDir(ZERO_CROSS_IN_GPIO_PIN, HI_GPIO_DIRECTION_IN);
}

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
*****************************************************************************/
HI_VOID HI_BOARD_SetTfTxOutDataGpioInit(HI_VOID)
{
    // GPIO 20����ΪGPIOģʽ
    HI_MDM_IO_SetMux(HI_HW_SPI_DO_IO20, (HI_U8)HI_GPIO20_SEL_GPIO20);
    // ��ʼ��Ϊ���
    HI_MDM_IO_SetDir(HI_GPIO_IDX_20, HI_GPIO_DIRECTION_OUT);
    //��ʼ��Ϊ�͵�ƽ
    HI_MDM_IO_SetOutputVal(HI_GPIO_IDX_20, HI_GPIO_VALUE0);
}

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
HI_VOID HI_BOARD_SetTfSendEdgeModeUpGpioInit(HI_VOID)
{
    HI_MDM_IO_SetOutputVal(HI_GPIO_IDX_9, HI_GPIO_VALUE0);
    HI_MDM_IO_SetDir(HI_GPIO_IDX_9, HI_GPIO_DIRECTION_OUT);
}

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
HI_VOID HI_BOARD_SetTfSendEdgeModeDownGpioInit(HI_VOID)
{
    HI_MDM_IO_SetOutputVal(HI_GPIO_IDX_10, HI_GPIO_VALUE0);
    HI_MDM_IO_SetDir(HI_GPIO_IDX_10, HI_GPIO_DIRECTION_OUT);
}

/*****************************************************************************
 �� �� ��  : HI_BOARD_SetSwitchLevel
 ��������  : ����̨��ʶ��λ
 �������  : HI_VOID
 �������  : ��
 �� �� ֵ  : HI_VOID
 ���������õĺ���  :
 ���ñ������ĺ���  :
 ���ʵ�ȫ�ֱ���  :
 �޸ĵ�ȫ�ֱ���  :
 �޸ĵ�BBP�Ĵ����б�  :
*****************************************************************************/
HI_VOID HI_BOARD_SetSwitchLevel(HI_U8 level)
{
    if(TF_RCV_MODE_240K == level)
    {
        if(HI_ND_TYPE_DCM != HI_DMS_LoadDevType())
        {
            #ifdef PRODUCT_CFG_BOARD_TYPE_ASIC_V200
            if (HI_ND_TYPE_THREE_STA != HI_DMS_LoadDevType())
            {
                HI_MDM_IO_SetOutputVal(HI_GPIO_IDX_27,HI_GPIO_VALUE0);
            }
            #endif
        }
        else
        {
            HI_MDM_IO_SetOutputVal(HI_GPIO_IDX_24,HI_GPIO_VALUE0);
        }
    }
    else if(TF_RCV_MODE_1M == level)
    {
        if(HI_ND_TYPE_DCM != HI_DMS_LoadDevType())
        {
            #ifdef PRODUCT_CFG_BOARD_TYPE_ASIC_V200
            if (HI_ND_TYPE_THREE_STA != HI_DMS_LoadDevType())
            {
                HI_MDM_IO_SetOutputVal(HI_GPIO_IDX_27, HI_GPIO_VALUE1);
            }
            #endif
        }
        else
        {
            HI_MDM_IO_SetOutputVal(HI_GPIO_IDX_24, HI_GPIO_VALUE1);
        }
    }
}

//���÷�����ģʽ
HI_VOID HI_BOARD_SetSendEdgeMode(HI_U8 edge_mode)
{
    //�л�̨��ʶ�����еķ�����ģʽ��Ӳ��ʱ��Ҫ���Ƚ�����GPIO�õͣ��ٽ���Ӧ�ܽ��ø�
    switch(edge_mode)
    {
        case TF_SEND_EDGE_MODE_OFF:
            //�����ؿ���
            HI_MDM_IO_SetOutputVal(HI_GPIO_IDX_9, HI_GPIO_VALUE0);
            //�½��ؿ���
            HI_MDM_IO_SetOutputVal(HI_GPIO_IDX_10, HI_GPIO_VALUE0);
            break;

        case TF_SEND_EDGE_MODE_UP:
            HI_MDM_IO_SetOutputVal(HI_GPIO_IDX_10, HI_GPIO_VALUE0);
            HI_MDM_IO_SetOutputVal(HI_GPIO_IDX_9, HI_GPIO_VALUE1);
            break;

        case TF_SEND_EDGE_MODE_DOWN:
            HI_MDM_IO_SetOutputVal(HI_GPIO_IDX_9, HI_GPIO_VALUE0);
            HI_MDM_IO_SetOutputVal(HI_GPIO_IDX_10, HI_GPIO_VALUE1);
            break;

        default:
            HI_MDM_IO_SetOutputVal(HI_GPIO_IDX_9, HI_GPIO_VALUE0);
            HI_MDM_IO_SetOutputVal(HI_GPIO_IDX_10, HI_GPIO_VALUE0);
            break;
    }
}

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
HI_VOID HI_BOARD_SetPhaseGpioMode(HI_VOID)
{
    // GPIO2,����ΪGPIOģʽ
    HI_MDM_IO_SetMux(HI_HW_DN_RX_LED_IO2, (HI_U8)HI_GPIO2_SEL_GPIO2);

    // GPIO3,����ΪGPIOģʽ
    HI_MDM_IO_SetMux(HI_HW_DN_TX_LED_IO3, (HI_U8)HI_GPIO3_SEL_GPIO3);

    // GPIO4,����ΪGPIOģʽ
    HI_MDM_IO_SetMux(HI_HW_STATUS_LED_IO4, (HI_U8)HI_GPIO4_SEL_GPIO4);
}

/*****************************************************************************
 �� �� ��  : HI_BOARD_SetPhaseGpioDirecInit
 ��������  : ���CCO����Ӳ�����ܣ�������λ��GPIOΪ���
 �������  : HI_IN HI_U8 phase_mode
 �������  : ��
 �� �� ֵ  : HI_VOID
 ���������õĺ���  :
 ���ñ������ĺ���  :
 ���ʵ�ȫ�ֱ���  :
 �޸ĵ�ȫ�ֱ���  :
 �޸ĵ�BBP�Ĵ����б�  :
*****************************************************************************/
HI_VOID HI_BOARD_SetPhaseGpioDirecInit(HI_VOID)
{
    HI_U32 index = 0;
    HI_U32 gpio_pin[PHASE_CNT] = {HI_GPIO_IDX_2,HI_GPIO_IDX_3,HI_GPIO_IDX_4};

    for(index = 0; index < PHASE_CNT; index++)
    {
        HI_MDM_IO_SetDir((HI_GPIO_IDX_E)(gpio_pin[index]), HI_GPIO_DIRECTION_OUT);
    }
}
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
HI_VOID HI_BOARD_SetPhaseGpioValue(HI_IN HI_U8 phase_mode)
{
    HI_U32 index = 0;
    HI_GPIO_VALUE_E gpio_val;
    HI_U32 gpio_pin[PHASE_CNT] = {HI_GPIO_IDX_2,HI_GPIO_IDX_3,HI_GPIO_IDX_4};

    gpio_val = ((PHASE_MODE_ON == phase_mode) ?  HI_GPIO_VALUE1 : HI_GPIO_VALUE0);
    for(index = 0; index < PHASE_CNT; index++)
    {
        // ����GPIO������
        HI_MDM_IO_SetOutputVal((HI_GPIO_IDX_E)(gpio_pin[index]), gpio_val);
    }
}
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
HI_VOID HI_BOARD_SetSinglePhaseGpioValue(HI_IN HI_GPIO_IDX_E gpio_pin,HI_IN HI_U8 phase_mode)
{
    HI_GPIO_VALUE_E gpio_val;

    gpio_val = ((PHASE_MODE_ON == phase_mode) ?  HI_GPIO_VALUE1 : HI_GPIO_VALUE0);

    HI_MDM_IO_SetOutputVal(gpio_pin, gpio_val);
}

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
HI_U32 HI_BOARD_SetBeepValue(HI_BOOL onoff)
{
#if (defined(PRODUCT_CFG_PRODUCT_TYPE_NDM) && defined(PRODUCT_CFG_SUPPORT_IO_EXP))
    HI_IO_EXP_OUT_VAL_E gpio_val = HI_IO_EXP_OUT_VAL_LOW;

    gpio_val = ((HI_TRUE == onoff) ?  HI_IO_EXP_OUT_VAL_LOW: HI_IO_EXP_OUT_VAL_HIGHT);

    return HI_IO_EXP_SetOutPutVal(NDM_IO_BEEP_ENABLE,gpio_val);
#else
    HI_UNREF_PARAM(onoff);
    return HI_ERR_NOT_SUPPORT;
#endif
}

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
HI_U32 HI_BOARD_SetBeepGpioInit(HI_VOID)
{
#if (defined(PRODUCT_CFG_PRODUCT_TYPE_NDM) && defined(PRODUCT_CFG_SUPPORT_IO_EXP))
    HI_U32 ret = HI_ERR_FAILURE;
    ret = HI_IO_EXP_SetDir(NDM_IO_BEEP_ENABLE, HI_IO_EXP_DIR_OUT);
    if(ret != HI_ERR_SUCCESS)
    {
        return ret;
    }

    return HI_IO_EXP_SetOutPutVal(NDM_IO_BEEP_ENABLE,HI_IO_EXP_OUT_VAL_HIGHT);
#else
    return HI_ERR_NOT_SUPPORT;
#endif
}

/*****************************************************************************
 �� �� ��  : HI_BOARD_SetNdmZeroDtcRxInit
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
HI_U32 HI_BOARD_SetNdmZeroDtcRxInit(HI_VOID)
{
#if (defined(PRODUCT_CFG_PRODUCT_TYPE_NDM) && defined(PRODUCT_CFG_SUPPORT_IO_EXP))
    HI_U32 ret = HI_ERR_FAILURE;

    ret = HI_IO_EXP_SetDir(IO_EXP_IO_NUM_13, HI_IO_EXP_DIR_OUT);
    if(HI_ERR_SUCCESS != ret)
    {
        return ret;
    }

    return HI_IO_EXP_SetOutPutVal(IO_EXP_IO_NUM_13,HI_IO_EXP_OUT_VAL_LOW);
#else
    return HI_ERR_NOT_SUPPORT;
#endif
}

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
HI_U32 HI_BOARD_SetNdmZeroDtcRxValue(HI_BOOL gpio_value)
{
#if (defined(PRODUCT_CFG_PRODUCT_TYPE_NDM) && defined(PRODUCT_CFG_SUPPORT_IO_EXP))
    HI_IO_EXP_OUT_VAL_E gpio_val = HI_IO_EXP_OUT_VAL_LOW;

    gpio_val = ((HI_TRUE == gpio_value) ?  HI_IO_EXP_OUT_VAL_HIGHT: HI_IO_EXP_OUT_VAL_LOW);

    return HI_IO_EXP_SetOutPutVal(IO_EXP_IO_NUM_13,gpio_val);
#else
    HI_UNREF_PARAM(gpio_value);
    return HI_ERR_NOT_SUPPORT;
#endif
}


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
HI_VOID HI_BOARD_SetAnCheckClearGpioInit(HI_VOID)
{
#if (defined(PRODUCT_CFG_PRODUCT_TYPE_NDM) && defined(PRODUCT_CFG_SUPPORT_IO_EXP))
    HI_IO_EXP_SetDir(IO_EXP_IO_NUM_06, HI_IO_EXP_DIR_OUT);
    HI_IO_EXP_SetOutPutVal(IO_EXP_IO_NUM_06,HI_IO_EXP_OUT_VAL_HIGHT);
#endif
}
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
*****************************************************************************/
HI_VOID HI_BOARD_SetAnCheckClearGpioValue(HI_BOOL gpio_value)
{
#if (defined(PRODUCT_CFG_PRODUCT_TYPE_NDM) && defined(PRODUCT_CFG_SUPPORT_IO_EXP))
     HI_IO_EXP_OUT_VAL_E gpio_val = HI_IO_EXP_OUT_VAL_LOW;

    gpio_val = ((HI_TRUE == gpio_value) ?  HI_IO_EXP_OUT_VAL_HIGHT : HI_IO_EXP_OUT_VAL_LOW);
    HI_IO_EXP_SetOutPutVal(IO_EXP_IO_NUM_06,gpio_val);
#else
    HI_UNREF_PARAM(gpio_value);
#endif
}
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
HI_VOID HI_BOARD_SetAnCheckGpioInit(HI_VOID)
{
#if (defined(PRODUCT_CFG_PRODUCT_TYPE_NDM) && defined(PRODUCT_CFG_SUPPORT_IO_EXP))
    HI_IO_EXP_SetDir(IO_EXP_IO_NUM_07, HI_IO_EXP_DIR_IN);
#endif
}
/*****************************************************************************
 �� �� ��  : HI_BOARD_GetAnCheckValue
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
HI_U8 HI_BOARD_GetAnCheckValue(HI_VOID)
{
#if (defined(PRODUCT_CFG_PRODUCT_TYPE_NDM) && defined(PRODUCT_CFG_SUPPORT_IO_EXP))
    HI_IO_EXP_OUT_VAL_E val = HI_IO_EXP_OUT_VAL_LOW;

    HI_IO_EXP_GetInputVal(IO_EXP_IO_NUM_07,&val);

    return (HI_U8)val;
#else
    return 0;
#endif
}

/*****************************************************************************
 �� �� ��  : HI_BOARD_SetOutSeqData
 ��������  : ����̨��ʶ�����еĶ�Ӧ����
 �������  : HI_U8 send_bit
 �������  : ��
 �� �� ֵ  : HI_U32
 ���������õĺ���  :
 ���ñ������ĺ���  :
 ���ʵ�ȫ�ֱ���  :
 �޸ĵ�ȫ�ֱ���  :
 �޸ĵ�BBP�Ĵ����б�  :
*****************************************************************************/
HI_U32 HI_BOARD_SetOutSeqData(HI_U8 send_bit)
{
    return HI_MDM_IO_SetOutputVal(TF_TX_OUT_DATA_PIN, (HI_GPIO_VALUE_E)send_bit);
}


/*****************************************************************************
 �� �� ��  : HI_BOARD_SetThreeMeterStaGpioMode
 ��������  : ���������STAģ������������ܽ�ΪGPIOģʽ
 �������  : HI_VOID
 �������  : ��
 �� �� ֵ  : HI_VOID
 ���������õĺ���  :
 ���ñ������ĺ���  :
 ���ʵ�ȫ�ֱ���  :
 �޸ĵ�ȫ�ֱ���  :
 �޸ĵ�BBP�Ĵ����б�  :
*****************************************************************************/
HI_VOID HI_BOARD_SetThreeMeterStaGpioMode(HI_VOID)
{
    // GPIO4,����ΪGPIOģʽ
    HI_MDM_IO_SetMux(HI_HW_STATUS_LED_IO4, (HI_U8)HI_GPIO4_SEL_GPIO4);
}

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
HI_VOID HI_BOARD_SetThreeMeterStaGpioDirAndValueInit(HI_VOID)
{
    HI_MDM_IO_SetDir(HI_GPIO_IDX_4, HI_GPIO_DIRECTION_OUT);
    HI_MDM_IO_SetOutputVal(HI_GPIO_IDX_4, HI_GPIO_VALUE0);
}

/*****************************************************************************
 �� �� ��  : HI_BOARD_SetPhaseSwitchCtrlMode
 ��������  : ����CCO��������л����Ƴ�ʼ��
 �������  : HI_VOID
 �������  : ��
 �� �� ֵ  : HI_VOID
 ���������õĺ���  :
 ���ñ������ĺ���  :
 ���ʵ�ȫ�ֱ���  :
 �޸ĵ�ȫ�ֱ���  :
 �޸ĵ�BBP�Ĵ����б�  :
*****************************************************************************/
HI_VOID HI_BOARD_SetPhaseSwitchCtrlMode(HI_VOID)
{
    #ifdef PRODUCT_CFG_BOARD_TYPE_ASIC_V200
    HI_MDM_IO_SetMux(HI_HW_GPIO26_BACKUP, (HI_U8)HI_GPIO26_BACKUP_SEL_CURRENT_PHASE_I00);
    HI_MDM_IO_SetMux(HI_HW_UART2_TXD_IO12, (HI_U8)HI_GPIO12_SEL_CURRENT_PHASE_IO1);
    HI_MDM_IO_SetMux(HI_HW_UART2_RXD_IO11, (HI_U8)HI_GPIO11_SEL_CURRENT_PHASE_IO2);
    #endif
}

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
/*****************************************************************************
 �� �� ��  : HI_BOARD_SetCcoZeroCrossEnable
 ��������  : ����CCO���㿪��
 �������  : HI_U8 power_edge_type : ��Ƶ���ڱ��أ�POWER_EDGE_X
             HI_U8 phase : ��λ
 �������  : ��
 �� �� ֵ  : HI_VOID
 ���������õĺ���  :
 ���ñ������ĺ���  : ͨ���������GPIO������CCO����λ���㿪��ʱ�����ñ�����
 ���ʵ�ȫ�ֱ���  :
 �޸ĵ�ȫ�ֱ���  :
 �޸ĵ�BBP�Ĵ����б�  :
*****************************************************************************/
HI_VOID HI_BOARD_SetCcoZeroCrossEnable(HI_U8 power_edge_type, HI_U8 phase)
{
    HI_GPIO_VALUE_E gpio26_out_value = HI_GPIO_VALUE0;
    HI_GPIO_VALUE_E gpio12_out_value = HI_GPIO_VALUE0;
    HI_GPIO_VALUE_E gpio11_out_value = HI_GPIO_VALUE0;

    if(POWER_EDGE_RISE == power_edge_type)
    {
        if(PHASE_A == phase)
        {
            gpio26_out_value = HI_GPIO_VALUE1;
        }
        else if(PHASE_B == phase)
        {
            gpio12_out_value = HI_GPIO_VALUE1;
        }
        else if(PHASE_C == phase)
        {
            gpio11_out_value = HI_GPIO_VALUE1;
        }
        else
        {}
    }
    else if(POWER_EDGE_FALL == power_edge_type)
    {
        if(PHASE_A == phase)
        {
            gpio26_out_value = HI_GPIO_VALUE0;
            gpio12_out_value = HI_GPIO_VALUE1;
            gpio11_out_value = HI_GPIO_VALUE1;
        }
        else if(PHASE_B == phase)
        {
            gpio26_out_value = HI_GPIO_VALUE1;
            gpio12_out_value = HI_GPIO_VALUE1;
            gpio11_out_value = HI_GPIO_VALUE1;
        }
        else if(PHASE_C == phase)
        {
            gpio26_out_value = HI_GPIO_VALUE1;
            gpio12_out_value = HI_GPIO_VALUE0;
            gpio11_out_value = HI_GPIO_VALUE1;
        }
        else
        {}
    }
    else
    {}

    HI_MDM_IO_SetOutputVal(HI_GPIO_IDX_26, gpio26_out_value);
    HI_MDM_IO_SetOutputVal(HI_GPIO_IDX_12, gpio12_out_value);
    HI_MDM_IO_SetOutputVal(HI_GPIO_IDX_11, gpio11_out_value);
}
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_NDM)
/*****************************************************************************
 �� �� ��  : HI_BOARD_TF_DataDisPlay
 ��������  : ���ر���ʾ�Ŷ������LCD����
 �������  : HI_VOID
 �������  : ��
 �� �� ֵ  : HI_VOID
 ���������õĺ���  :
 ���ñ������ĺ���  :
 ���ʵ�ȫ�ֱ���  :
 �޸ĵ�ȫ�ֱ���  :
 �޸ĵ�BBP�Ĵ����б�  :
*****************************************************************************/
HI_U32 HI_BOARD_TF_DataDisPlay(HI_IN HI_U16 wobble_scope)
{
    HI_U32 ret = HI_ERR_FAILURE;
    HI_LCD_DISPLAY_INFO_S lcd_cfg = {{0}, 0};
    HI_U8 index = 0;
    HI_U8 valid_index = HI_LCD_POSITION_MAX;
    HI_U8 data[HI_LCD_POSITION_MAX] = {0};

    // �Ŷ�ֵ>= 1000us,����ʾ"OFL"��ʾ���
    if(wobble_scope >= TF_NDM_MAX_REAL_NOISE)
    {
        lcd_cfg.data[HI_LCD_POSITION_1] = 'O';
        lcd_cfg.data[HI_LCD_POSITION_2] = 'F';
        lcd_cfg.data[HI_LCD_POSITION_3] = 'L';
    }
    else
    {
        for(index = 0; index < HI_LCD_POSITION_MAX; index++)
        {
            data[HI_LCD_POSITION_MAX - 1- index] = (HI_U8)(wobble_scope % 10);
            wobble_scope = wobble_scope / 10;
        }

        for(index = 0; index < HI_LCD_POSITION_MAX; index++)
        {
            if(0 != data[index])
            {
                valid_index = index;
                break;
            }
        }

        for(index = valid_index; index < HI_LCD_POSITION_MAX; index++)
        {
            lcd_cfg.data[index] = (HI_CHAR) (data[index] + '0');
        }

        if(valid_index == HI_LCD_POSITION_MAX)
        {
            lcd_cfg.data[HI_LCD_POSITION_3] = (HI_CHAR)(0 + '0');
        }
    }

    lcd_cfg.blink = HI_FALSE;
    lcd_cfg.pri = HI_LCD_DISPLAY_PRI_2;
    lcd_cfg.tsec = HI_LCD_DISPLAY_FOREVER;
    ret = HI_DRV_LCD_DisplayPrint(HI_LCD_FUNC_ID_TF, &lcd_cfg);

    return ret;
}

/*****************************************************************************
 �� �� ��  : HI_BOARD_TF_ModeDisPlay
 ��������  : ���÷���������ر�ģʽ
 �������  : HI_VOID
 �������  : ��
 �� �� ֵ  : HI_VOID
 ���������õĺ���  :
 ���ñ������ĺ���  :
 ���ʵ�ȫ�ֱ���  :
 �޸ĵ�ȫ�ֱ���  :
 �޸ĵ�BBP�Ĵ����б�  :
*****************************************************************************/
HI_U32 HI_BOARD_TF_ModeDisPlay(HI_IN HI_U8 transformer_mode)
{
    HI_U32 ret = HI_ERR_FAILURE;
    HI_LCD_DISPLAY_INFO_S lcd_cfg = {{0}, 0};

    //�����������ʾ̨��ʶ��ģʽ
    if(TF_IDENTIFY_SEND_MODE == transformer_mode)
    {
        lcd_cfg.data[HI_LCD_POSITION_1] = 'T';
        lcd_cfg.data[HI_LCD_POSITION_2] = HI_LCD_NOT_FLUSH;
        lcd_cfg.data[HI_LCD_POSITION_3] = HI_LCD_NOT_FLUSH;
    }
    else if(TF_IDENTIFY_RCV_MODE == transformer_mode)
    {
        lcd_cfg.data[HI_LCD_POSITION_1] = 'R';
        lcd_cfg.data[HI_LCD_POSITION_2] = '-';
        lcd_cfg.data[HI_LCD_POSITION_3] = '-';
    }
    else if(TF_IDENTIFY_NORMAL_MODE == transformer_mode)
    {
        //��������ģʽ���߷�̨��ģʽ
        HI_DRV_LCD_DisplayDisable(HI_LCD_FUNC_ID_TF);
        return HI_ERR_SUCCESS;
    }
    else if(TF_IDENTIFY_LCD_OFF_MODE == transformer_mode)
    {
        lcd_cfg.data[0] = HI_LCD_DISP_NONE;
        lcd_cfg.data[1] = HI_LCD_DISP_NONE;
        lcd_cfg.data[2] = HI_LCD_DISP_NONE;
    }
    else
    {
        //���۲���ִ��
        lcd_cfg.data[0] = 'E';
        lcd_cfg.data[1] = 'R';
        lcd_cfg.data[2] = 'R';
    }

    lcd_cfg.blink = HI_TRUE;
    lcd_cfg.pri = HI_LCD_DISPLAY_PRI_2;
    lcd_cfg.tsec = HI_LCD_DISPLAY_FOREVER;
    ret = HI_DRV_LCD_DisplayPrint(HI_LCD_FUNC_ID_TF, &lcd_cfg);

    return ret;
}


/*****************************************************************************
 �� �� ��  : HI_BOARD_TF_TxNumDisPlay
 ��������  : ���÷���̨�����
 �������  : HI_VOID
 �������  : ��
 �� �� ֵ  : HI_VOID
 ���������õĺ���  :
 ���ñ������ĺ���  :
 ���ʵ�ȫ�ֱ���  :
 �޸ĵ�ȫ�ֱ���  :
 �޸ĵ�BBP�Ĵ����б�  :
*****************************************************************************/
HI_U32 HI_BOARD_TF_TxNumDisPlay(HI_IN HI_U8 transformer_index)
{
    HI_U32 ret = HI_ERR_FAILURE;
    HI_LCD_DISPLAY_INFO_S lcd_cfg = {{0}, 0};

    //�����������ʾ̨�����
    lcd_cfg.data[HI_LCD_POSITION_1] = HI_LCD_NOT_FLUSH;
    lcd_cfg.data[HI_LCD_POSITION_2] = (HI_CHAR)('1' + transformer_index);
    lcd_cfg.data[HI_LCD_POSITION_3] = HI_LCD_NOT_FLUSH;
    lcd_cfg.blink = HI_TRUE;
    lcd_cfg.pri = HI_LCD_DISPLAY_PRI_2;
    lcd_cfg.tsec = HI_LCD_DISPLAY_FOREVER;
    ret = HI_DRV_LCD_DisplayPrint(HI_LCD_FUNC_ID_TF, &lcd_cfg);

    return ret;
}

/*****************************************************************************
 �� �� ��  : HI_BOARD_TF_TxPhaseDisPlay
 ��������  : ���÷������ж�Ӧ����λ
 �������  : HI_VOID
 �������  : ��
 �� �� ֵ  : HI_VOID
 ���������õĺ���  :
 ���ñ������ĺ���  :
 ���ʵ�ȫ�ֱ���  :
 �޸ĵ�ȫ�ֱ���  :
 �޸ĵ�BBP�Ĵ����б�  :
*****************************************************************************/
HI_U32 HI_BOARD_TF_TxPhaseDisPlay(HI_IN HI_U8 phase_index)
{
    HI_U32 ret = HI_ERR_FAILURE;
    HI_LCD_DISPLAY_INFO_S lcd_cfg = {{0}, 0};

    //�����������ʾ̨�����
    lcd_cfg.data[HI_LCD_POSITION_1] = HI_LCD_NOT_FLUSH;
    lcd_cfg.data[HI_LCD_POSITION_2] = HI_LCD_NOT_FLUSH;
    lcd_cfg.data[HI_LCD_POSITION_3] = (HI_CHAR)('A' + phase_index);
    lcd_cfg.blink = HI_TRUE;
    lcd_cfg.pri = HI_LCD_DISPLAY_PRI_2;
    lcd_cfg.tsec = HI_LCD_DISPLAY_FOREVER;
    ret = HI_DRV_LCD_DisplayPrint(HI_LCD_FUNC_ID_TF, &lcd_cfg);

    return ret;
}


/*****************************************************************************
 �� �� ��  : HI_BOARD_TF_RxNumDisPlay
 ��������  : ���ó��ر�ʶ��Ľ����̨�������ʾ
 �������  : HI_VOID
 �������  : ��
 �� �� ֵ  : HI_VOID
 ���������õĺ���  :
 ���ñ������ĺ���  :
 ���ʵ�ȫ�ֱ���  :
 �޸ĵ�ȫ�ֱ���  :
 �޸ĵ�BBP�Ĵ����б�  :
*****************************************************************************/
HI_U32 HI_BOARD_TF_RxNumDisPlay(HI_IN HI_U8 transformer_index)
{
    HI_U32 ret = HI_ERR_FAILURE;
    HI_LCD_DISPLAY_INFO_S lcd_cfg = {{0}, 0};

    //�����������ʾ̨�����
    lcd_cfg.data[HI_LCD_POSITION_1] = (HI_CHAR)('1' + transformer_index);
    lcd_cfg.data[HI_LCD_POSITION_2] = HI_LCD_NOT_FLUSH;
    lcd_cfg.data[HI_LCD_POSITION_3] = HI_LCD_NOT_FLUSH;
    lcd_cfg.pri = HI_LCD_DISPLAY_PRI_2;
    lcd_cfg.tsec = HI_LCD_DISPLAY_FOREVER;
    ret = HI_DRV_LCD_DisplayPrint(HI_LCD_FUNC_ID_TF, &lcd_cfg);

    return ret;
}
/*****************************************************************************
 �� �� ��  : HI_BOARD_TF_RxPhaseDisPlay
 ��������  : ���ó��ر�ʶ��Ľ������λ��ʾ
 �������  : HI_VOID
 �������  : ��
 �� �� ֵ  : HI_VOID
 ���������õĺ���  :
 ���ñ������ĺ���  :
 ���ʵ�ȫ�ֱ���  :
 �޸ĵ�ȫ�ֱ���  :
 �޸ĵ�BBP�Ĵ����б�  :
*****************************************************************************/
HI_U32 HI_BOARD_TF_RxPhaseDisPlay(HI_IN HI_U8 phase_index)
{
    HI_U32 ret = HI_ERR_FAILURE;
    HI_LCD_DISPLAY_INFO_S lcd_cfg = {{0}, 0};

    //�����������ʾ̨�����
    lcd_cfg.data[HI_LCD_POSITION_1] = HI_LCD_NOT_FLUSH;
    lcd_cfg.data[HI_LCD_POSITION_2] = (HI_CHAR)('A' + phase_index);
    lcd_cfg.data[HI_LCD_POSITION_3] = HI_LCD_NOT_FLUSH;
    lcd_cfg.pri = HI_LCD_DISPLAY_PRI_2;
    lcd_cfg.tsec = HI_LCD_DISPLAY_FOREVER;
    ret = HI_DRV_LCD_DisplayPrint(HI_LCD_FUNC_ID_TF, &lcd_cfg);

    return ret;
}
/*****************************************************************************
 �� �� ��  : HI_BOARD_TF_LevelDisPlay
 ��������  : ���ó��ر����ճɹ�ʱ��λ��ʾ��K,M
 �������  : HI_VOID
 �������  : ��
 �� �� ֵ  : HI_VOID
 ���������õĺ���  :
 ���ñ������ĺ���  :
 ���ʵ�ȫ�ֱ���  :
 �޸ĵ�ȫ�ֱ���  :
 �޸ĵ�BBP�Ĵ����б�  :
*****************************************************************************/
HI_U32 HI_BOARD_TF_LevelDisPlay(HI_IN HI_U8 gear)
{
    HI_U32 ret = HI_ERR_FAILURE;
    HI_LCD_DISPLAY_INFO_S lcd_cfg = {{0}, 0};

    //�����������ʾ̨��ʶ����յ�λ
    lcd_cfg.data[HI_LCD_POSITION_1] = HI_LCD_NOT_FLUSH;
    lcd_cfg.data[HI_LCD_POSITION_2] = HI_LCD_NOT_FLUSH;
    if(TF_RCV_MODE_1M == gear)
    {
        lcd_cfg.data[HI_LCD_POSITION_3] = 'M';
    }
    else if(TF_RCV_MODE_240K == gear)
    {
        lcd_cfg.data[HI_LCD_POSITION_3] = 'K';
    }

    lcd_cfg.pri = HI_LCD_DISPLAY_PRI_2;
    lcd_cfg.tsec = HI_LCD_DISPLAY_FOREVER;
    ret = HI_DRV_LCD_DisplayPrint(HI_LCD_FUNC_ID_TF, &lcd_cfg);

    return ret;
}
/*****************************************************************************
 �� �� ��  : HI_BOARD_TF_RxNokDisPlay
 ��������  : ���ó��ر�ʶ��ʧ����ʾNOK
 �������  : HI_VOID
 �������  : ��
 �� �� ֵ  : HI_VOID
 ���������õĺ���  :
 ���ñ������ĺ���  :
 ���ʵ�ȫ�ֱ���  :
 �޸ĵ�ȫ�ֱ���  :
 �޸ĵ�BBP�Ĵ����б�  :
*****************************************************************************/
HI_U32 HI_BOARD_TF_RxNokDisPlay(HI_VOID)
{
    HI_U32 ret = HI_ERR_FAILURE;
    HI_LCD_DISPLAY_INFO_S lcd_cfg = {{0}, 0};

    //�����������ʾ'NOK'
    lcd_cfg.data[HI_LCD_POSITION_1] = 'N';
    lcd_cfg.data[HI_LCD_POSITION_2] = 'O';
    lcd_cfg.data[HI_LCD_POSITION_3] = 'K';
    lcd_cfg.blink = HI_FALSE;
    lcd_cfg.pri = HI_LCD_DISPLAY_PRI_2;
    lcd_cfg.tsec = HI_LCD_DISPLAY_FOREVER;
    ret = HI_DRV_LCD_DisplayPrint(HI_LCD_FUNC_ID_TF, &lcd_cfg);

    return ret;
}
/*****************************************************************************
 �� �� ��  : HI_BOARD_TF_DisablePhaseDisPlay
 ��������  : ����������Ŷ�ʱ����Ӧ��λû���Ŷ�����ʾ"-"
 �������  : HI_VOID
 �������  : ��
 �� �� ֵ  : HI_VOID
 ���������õĺ���  :
 ���ñ������ĺ���  :
 ���ʵ�ȫ�ֱ���  :
 �޸ĵ�ȫ�ֱ���  :
 �޸ĵ�BBP�Ĵ����б�  :
*****************************************************************************/
HI_U32 HI_BOARD_TF_DisablePhaseDisPlay(HI_VOID)
{
    HI_U32 ret = HI_ERR_FAILURE;
    HI_LCD_DISPLAY_INFO_S lcd_cfg = {{0}, 0};

    //�����������ʾ��λ
    lcd_cfg.data[HI_LCD_POSITION_1] = HI_LCD_NOT_FLUSH;
    lcd_cfg.data[HI_LCD_POSITION_2] = HI_LCD_NOT_FLUSH;
    lcd_cfg.data[HI_LCD_POSITION_3] = (HI_CHAR)('-');
    lcd_cfg.blink = HI_TRUE;
    lcd_cfg.pri = HI_LCD_DISPLAY_PRI_2;
    lcd_cfg.tsec = HI_LCD_DISPLAY_FOREVER;
    ret = HI_DRV_LCD_DisplayPrint(HI_LCD_FUNC_ID_TF, &lcd_cfg);

    return ret;
}
/*****************************************************************************
 �� �� ��  : HI_BOARD_TF_ErrDisPlay
 ��������  : ����������쳣ʱ��LCD������ʾ������
 �������  : HI_VOID
 �������  : ��
 �� �� ֵ  : HI_VOID
 ���������õĺ���  :
 ���ñ������ĺ���  :
 ���ʵ�ȫ�ֱ���  :
 �޸ĵ�ȫ�ֱ���  :
 �޸ĵ�BBP�Ĵ����б�  :
*****************************************************************************/
HI_U32 HI_BOARD_TF_ErrDisPlay(HI_U8 err_code)
{
    HI_U32 ret = HI_ERR_FAILURE;
    HI_LCD_DISPLAY_INFO_S lcd_cfg = {{0}, 0};

    lcd_cfg.data[HI_LCD_POSITION_1] = 'E';
    lcd_cfg.data[HI_LCD_POSITION_2] = (HI_CHAR)('0' + (err_code /10));
    lcd_cfg.data[HI_LCD_POSITION_3] = (HI_CHAR)('0' + (err_code % 10));

    lcd_cfg.blink = HI_FALSE;
    lcd_cfg.pri = HI_LCD_DISPLAY_PRI_2;
    lcd_cfg.tsec = HI_LCD_DISPLAY_FOREVER;
    ret = HI_DRV_LCD_DisplayPrint(HI_LCD_FUNC_ID_TF, &lcd_cfg);

    return ret;
}

/*****************************************************************************
 �� �� ��  : HI_BOARD_TF_DisPlayInit
 ��������  : ����������ر���������ģʽ�����̨��ģʽ
 �������  : HI_VOID
 �������  : ��
 �� �� ֵ  : HI_VOID
 ���������õĺ���  :
 ���ñ������ĺ���  :
 ���ʵ�ȫ�ֱ���  :
 �޸ĵ�ȫ�ֱ���  :
 �޸ĵ�BBP�Ĵ����б�  :
*****************************************************************************/
HI_VOID HI_BOARD_TF_DisPlayInit(HI_VOID)
{
    HI_DRV_LCD_DisplayDisable(HI_LCD_FUNC_ID_TF);
}
#endif

