//*****************************************************************************
//
//                  版权所有 (C),  华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : gpio_spi.c
//  版 本 号   : V1.0
//  作    者   : wanglei/00279578
//  生成日期   : 2014-10-10
//  功能描述   : GPIO模拟SPI总线, 为提升模拟IO效率，本文件函数均未作参数判断
//
//  函数列表   : TODO: ...
//  修改历史   :
//  1.日    期 : 2014-10-10
//    作    者 : wanglei/00279578
//    修改内容 : 创建文件
//  1.日    期 : 2018-11-03
//    作    者 : yuchuang/00425297
//    修改内容 : 删除与rf耦合的代码。
//
//*****************************************************************************

#include "hi_types.h"
#include "hi_mdm_types.h"
#include "hi_mdm_io.h"
#include "gpio_spi.h"
#include "hi_mdm_time.h"

/*GPIO初始化*/
HI_VOID HI_MDM_GpioSpiInit(HI_VOID)
{
    /*设置CS/MOSI/SCLK初始值*/
    (HI_VOID)HI_MDM_IO_SetOutputVal(GPIO_SPI_MOSI, HI_GPIO_VALUE0);//SDO
    (HI_VOID)HI_MDM_IO_SetOutputVal(GPIO_SPI_SCLK, HI_GPIO_VALUE0);//CLK
    (HI_VOID)HI_MDM_IO_SetOutputVal(GPIO_SPI_NSEL, HI_GPIO_VALUE1);//CS
}

/*片选使能*/
HI_VOID HI_MDM_GpioSpiSelEnable(HI_VOID)
{
    (HI_VOID)HI_MDM_IO_SetOutputVal(GPIO_SPI_NSEL,HI_GPIO_VALUE0);
    (HI_VOID)HI_MDM_Udelay(1);
}

/*片选关闭*/
HI_VOID HI_MDM_GpioSpiSelDisable(HI_VOID)
{
    (HI_VOID)HI_MDM_IO_SetOutputVal(GPIO_SPI_NSEL,HI_GPIO_VALUE1);
    (HI_VOID)HI_MDM_Udelay(1);
}
/***********************************************************************************
 * 功能:    发送一个字节
 * 入参:    txData->要发送的数据;
 *
 *
 * 返回值:  无
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
    //设定一个DEFAULT状态，确定一个空闲状态
}
/***********************************************************************************
 * 功能:    接收一个字节
 * 入参:    无
 *
 *
 * 返回值:  rxData->接收的字节
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
 * 功能:    多字节数据发送
 * 入参:    num->发送数据的个数
 *          pvalue->发送数据的指针
 *
 * 返回值:  无
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
 * 功能:    多字节数据读取
 * 入参:    num->读取数据的个数
 *          pvalue->数据存放的指针
 *
 * 返回值:  无
***********************************************************************************/
HI_VOID HI_MDM_GpioSpiReadData(HI_U8 num, HI_U8 *pvalue)
{
	HI_U8 i;

    for (i=0; i<num; i++)
    {
        pvalue[i]=HI_MDM_GpioSpiReadByte();
    }
}


