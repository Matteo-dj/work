
//============================================================================
// Name        : Radio_hal.c
// Author      : 韩正玉
// Version     : v1.0
// Date 		: 16-9-27
// Copyright   : 深国电
// Description : radio driver
//============================================================================
#include "radio_hal.h"
#include "spi.h"


HI_S32 IRQ_config(HI_S32 (*gpio_callback)(HI_U32 gpio_para))
{
  HI_S32 gpio_para = 0,ret = 0;

  ret = HI_IO_IntInit();

  ret = HI_MDM_IO_IntConnect (HI_GPIO_IDX_31, HI_INT_EDGE, 
                              HI_EDGE_FALL_LEVEL_LOW, 
                              gpio_callback,gpio_para, HI_FALSE );

  return ret;
}

void RADIO_Config(void)
{
  HI_MDM_IO_SetMux(HI_HW_SFC_HOLDN_IO25,HI_GPIO25_SEL_GPIO25);//SDN 复位管脚
  HI_MDM_IO_SetDir(HI_GPIO_IDX_25, HI_GPIO_DIRECTION_OUT);
  HI_MDM_IO_SetOutputVal(HI_GPIO_IDX_25, HI_GPIO_VALUE1);//SDN初始值为

  HI_MDM_IO_SetMux(HI_HW_PERI_RSTN_IO31,HI_GPIO31_SEL_GPIO31);//IRQ
  HI_MDM_IO_SetDir(HI_GPIO_IDX_31, HI_GPIO_DIRECTION_IN);
}

void radio_hal_AssertShutdown(void)
{
  HI_MDM_IO_SetOutputVal(HI_GPIO_IDX_25, HI_GPIO_VALUE1);
}

void radio_hal_DeassertShutdown(void)
{
  HI_MDM_IO_SetOutputVal(HI_GPIO_IDX_25, HI_GPIO_VALUE0);
}

void radio_hal_ClearNsel(void)
{
  HI_MDM_IO_SetOutputVal(HI_GPIO_IDX_28, HI_GPIO_VALUE0);
}

void radio_hal_SetNsel(void)
{
  HI_MDM_IO_SetOutputVal(HI_GPIO_IDX_28, HI_GPIO_VALUE1);
}

u8 radio_hal_NirqLevel(void)
{
  HI_GPIO_VALUE_E ioVal = 0;
  HI_MDM_IO_GetInputVal (HI_GPIO_IDX_31, &ioVal);
  return ioVal;
}

void radio_hal_SpiWriteByte(u8 byteToWrite)
{
  SpiWriteOneByte(byteToWrite);
}

u8 radio_hal_SpiReadByte(void)
{
  return SpiReadOneByte();
}

void radio_hal_SpiWriteData(u8 byteCount, u8* pData)
{
  SpiWriteManyData(byteCount,pData);
}

void radio_hal_SpiReadData(u8 byteCount, u8* pData)
{
  SpiReadManyData(byteCount,pData);

}

