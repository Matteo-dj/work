
#ifndef __GPIO_SPI_H__
#define __GPIO_SPI_H__
#include <hi_types.h>
#include <hi_mdm_io.h>



#define GPIO_SPI_SCLK   HI_GPIO_IDX_6
#define GPIO_SPI_NSEL   HI_GPIO_IDX_28
#define GPIO_SPI_MISO   HI_GPIO_IDX_1
#define GPIO_SPI_MOSI   HI_GPIO_IDX_0


#define BIT_COUNT 8


HI_VOID HI_MDM_GpioSpiInit(HI_VOID);
HI_VOID HI_MDM_GpioSpiSelEnable(HI_VOID);
HI_VOID HI_MDM_GpioSpiSelDisable(HI_VOID);
HI_VOID HI_MDM_GpioSpiWriteByte(HI_U8 txData);
HI_U8   HI_MDM_GpioSpiReadByte(HI_VOID);
HI_VOID HI_MDM_GpioSpiWriteData(HI_U8 num, HI_U8 *pvalue);
HI_VOID HI_MDM_GpioSpiReadData(HI_U8 num, HI_U8 *pvalue);
#endif
