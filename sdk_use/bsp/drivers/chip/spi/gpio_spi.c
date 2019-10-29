//*****************************************************************************
//
//                  ��Ȩ���� (C),  ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : gpio_spi.c
//  �� �� ��   : V1.0
//  ��    ��   : wanglei/00279578
//  ��������   : 2014-10-10
//  ��������   : GPIOģ��SPI����, Ϊ����ģ��IOЧ�ʣ����ļ�������δ�������ж�
//
//  �����б�   : TODO: ...
//  �޸���ʷ   :
//  1.��    �� : 2014-10-10
//    ��    �� : wanglei/00279578
//    �޸����� : �����ļ�
//  1.��    �� : 2018-11-03
//    ��    �� : yuchuang/00425297
//    �޸����� : ɾ����rf��ϵĴ��롣
//
//*****************************************************************************

#include "hi_types.h"
#include "hi_mdm_types.h"
#include "hi_mdm_io.h"
#include "gpio_spi.h"
#include "hi_mdm_time.h"

/*GPIO��ʼ��*/
HI_VOID HI_MDM_GpioSpiInit(HI_VOID)
{
    /*����CS/MOSI/SCLK��ʼֵ*/
    (HI_VOID)HI_MDM_IO_SetOutputVal(GPIO_SPI_MOSI, HI_GPIO_VALUE0);//SDO
    (HI_VOID)HI_MDM_IO_SetOutputVal(GPIO_SPI_SCLK, HI_GPIO_VALUE0);//CLK
    (HI_VOID)HI_MDM_IO_SetOutputVal(GPIO_SPI_NSEL, HI_GPIO_VALUE1);//CS
}

/*Ƭѡʹ��*/
HI_VOID HI_MDM_GpioSpiSelEnable(HI_VOID)
{
    (HI_VOID)HI_MDM_IO_SetOutputVal(GPIO_SPI_NSEL,HI_GPIO_VALUE0);
    (HI_VOID)HI_MDM_Udelay(1);
}

/*Ƭѡ�ر�*/
HI_VOID HI_MDM_GpioSpiSelDisable(HI_VOID)
{
    (HI_VOID)HI_MDM_IO_SetOutputVal(GPIO_SPI_NSEL,HI_GPIO_VALUE1);
    (HI_VOID)HI_MDM_Udelay(1);
}
/***********************************************************************************
 * ����:    ����һ���ֽ�
 * ���:    txData->Ҫ���͵�����;
 *
 *
 * ����ֵ:  ��
 *
***********************************************************************************/
HI_VOID HI_MDM_GpioSpiWriteByte(HI_U8 txData)
{
	HI_U8 i;

    (HI_VOID)HI_MDM_IO_SetOutputVal(GPIO_SPI_SCLK, HI_GPIO_VALUE0);
	for (i = 0;i < BIT_COUNT;i++)
	{
        if (txData&0x80)
		{
			(HI_VOID)HI_MDM_IO_SetOutputVal(GPIO_SPI_MOSI, HI_GPIO_VALUE1);
		}
		else
		{
			(HI_VOID)HI_MDM_IO_SetOutputVal(GPIO_SPI_MOSI, HI_GPIO_VALUE0);
		}
		(HI_VOID)HI_MDM_IO_SetOutputVal(GPIO_SPI_SCLK, HI_GPIO_VALUE1);
        txData <<= 1;
		(HI_VOID)HI_MDM_IO_SetOutputVal(GPIO_SPI_SCLK, HI_GPIO_VALUE0);
	}
    (HI_VOID)HI_MDM_IO_SetOutputVal(GPIO_SPI_MOSI, HI_GPIO_VALUE0);
    //�趨һ��DEFAULT״̬��ȷ��һ������״̬
}
/***********************************************************************************
 * ����:    ����һ���ֽ�
 * ���:    ��
 *
 *
 * ����ֵ:  rxData->���յ��ֽ�
***********************************************************************************/
HI_U8 HI_MDM_GpioSpiReadByte(HI_VOID)
{
    HI_U8 i;
    HI_U8 rxData = 0;
    HI_GPIO_VALUE_E tmp;

    (HI_VOID)HI_MDM_IO_SetOutputVal(GPIO_SPI_SCLK, HI_GPIO_VALUE0);

    for (i = 0;i < BIT_COUNT;i++)
    {
        rxData <<= 1;
        (HI_VOID)HI_MDM_IO_SetOutputVal(GPIO_SPI_SCLK, HI_GPIO_VALUE1);

        (HI_VOID)HI_MDM_IO_GetInputVal(GPIO_SPI_MISO, &tmp);
        if (tmp)
        {
            rxData |= 0x01;
        }
        else
        {
            rxData &= ~0x01;
        }
       (HI_VOID)HI_MDM_IO_SetOutputVal(GPIO_SPI_SCLK, HI_GPIO_VALUE0);
    }
    return rxData;
}
/***********************************************************************************
 * ����:    ���ֽ����ݷ���
 * ���:    num->�������ݵĸ���
 *          pvalue->�������ݵ�ָ��
 *
 * ����ֵ:  ��
 *
***********************************************************************************/
HI_VOID HI_MDM_GpioSpiWriteData(HI_U8 num, HI_U8 *pvalue)
{
	HI_U8 i;

	for (i=0; i<num; i++)
	{
        HI_MDM_GpioSpiWriteByte(pvalue[i]);
	}
}
/***********************************************************************************
 * ����:    ���ֽ����ݶ�ȡ
 * ���:    num->��ȡ���ݵĸ���
 *          pvalue->���ݴ�ŵ�ָ��
 *
 * ����ֵ:  ��
***********************************************************************************/
HI_VOID HI_MDM_GpioSpiReadData(HI_U8 num, HI_U8 *pvalue)
{
	HI_U8 i;

    for (i=0; i<num; i++)
    {
        pvalue[i]=HI_MDM_GpioSpiReadByte();
    }
}


