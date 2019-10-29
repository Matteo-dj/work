//============================================================================
// Name        : Spi.c
// Author      : 韩正玉
// Version     : v1.0
// Date 		: 16-9-27
// Copyright   : 深国电
// Description : Spi driver
//============================================================================
#include "spi.h"

static HI_U32 fd;

u8 SPI4438_Init(void)
{
  HI_U32 ret = HI_ERR_SUCCESS;
  HI_SPI_DEVICE_USR_CFG_S stUsrCfg = {0};

  HI_MDM_IO_SetMux(HI_HW_GPIO28_BACKUP,HI_GPIO28_BACKUP_SEL_GPIO28);
  HI_MDM_IO_SetDir(HI_GPIO_IDX_28, HI_GPIO_DIRECTION_OUT);
  HI_MDM_IO_SetOutputVal(HI_GPIO_IDX_28, HI_GPIO_VALUE1);//NSEL

  HI_MDM_IO_SetMux(HI_HW_UP_RX_LED_IO0, HI_GPIO0_SEL_SSP_DO);//SDO
  HI_MDM_IO_SetMux(HI_HW_UP_TX_LED_IO1, HI_GPIO1_SEL_SSP_DI);//SDI
  HI_MDM_IO_SetMux(HI_HW_CRSZ_DTC_IO6,HI_GPIO6_SEL_SSP_CK);//CLK

  HI_MDM_SPI_Init (HI_SPI_ID_0);
  ret = HI_MDM_SPI_DevOpen (HI_SPI_ID_0 ,&fd);
  if(ret!=HI_ERR_SUCCESS)
  {
    return 1;
  }

  stUsrCfg.dataWidth = HI_SPI_CFG_DATA_WIDTH_E_8BIT;
  stUsrCfg.framMode = HI_SPI_CFG_FRAM_MODE_MOTOROLA;
  stUsrCfg.spo = HI_SPI_CFG_CLOCK_CPOL_0;
  stUsrCfg.sph = HI_SPI_CFG_CLOCK_CPHA_0;
  stUsrCfg.endian = HI_SPI_CFG_ENDIAN_LITTLE;
  stUsrCfg.baudRate= 4*1000*1000;
  stUsrCfg.waitEn = 0;
  stUsrCfg.waitVal = 0;
  stUsrCfg.prepareFunc = HI_NULL;
  stUsrCfg.restoreFunc = HI_NULL;
  ret = HI_MDM_SPI_Config(fd,&stUsrCfg);
  
  if(ret!=HI_ERR_SUCCESS)
  {
    return 2;
  }

  return 0;
}   

void SpiWriteOneByte(u8 byteToWrite)
{
  HI_MDM_SPI_SendData(fd,&byteToWrite,1,0);
}

u8 SpiReadOneByte(void)
{
  u8 outDat = 0;
  
  HI_MDM_SPI_RecvData(fd,&outDat,1,0);

  return outDat;
}

void SpiWriteManyData(u8 byteCount, u8* pData)
{
  if(byteCount > 0 && NULL != pData)
  {
    HI_MDM_SPI_SendData(fd,pData,byteCount,0);
  }
}

void SpiReadManyData(u8 byteCount, u8* pData)
{
  if(byteCount > 0 && NULL != pData)
  {
    HI_MDM_SPI_RecvData(fd,pData,byteCount,0);
  }
}

