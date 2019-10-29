//============================================================================
// Name        : Radio_hal.h
// Author      : 韩正玉
// Version     : v1.0
// Date 		: 16-9-27
// Copyright   : 深国电
// Description : Spi driver
//============================================================================
#ifndef _RADIO_HAL_H_
#define _RADIO_HAL_H_
#include "spi.h"

#define RF_GPIO_0_PORT	        GPIOB
#define RF_GPIO_0_PIN           GPIO_PIN_9 

#define RF_GPIO_1_PORT	        GPIOB
#define RF_GPIO_1_PIN           GPIO_PIN_7 

#define RF_SDN_PORT	        GPIOB
#define RF_SDN_PIN              GPIO_PIN_8 

#define RF_NIRQ_PORT	        GPIOB
#define RF_NIRQ_PIN	        GPIO_PIN_6 

#define RF_NSEL_PORT	        GPIOA
#define RF_NSEL_PIN             GPIO_PIN_15 

#define RF_POWER_PORT           GPIOF
#define RF_POWER_PIN            GPIO_PIN_0

#define RF_RAMP_CTL_PORT        GPIOF
#define RF_RAMP_CTL_PIN         GPIO_PIN_0

#define nIRQ                    radio_hal_NirqLevel()

void RADIO_Config(void);
void radio_hal_AssertShutdown(void);
void radio_hal_DeassertShutdown(void);
void radio_hal_ClearNsel(void);
void radio_hal_SetNsel(void);
u8 radio_hal_NirqLevel(void);

void radio_hal_SpiWriteByte(u8 byteToWrite);
u8 radio_hal_SpiReadByte(void);

void radio_hal_SpiWriteData(u8 byteCount, u8* pData);
void radio_hal_SpiReadData(u8 byteCount, u8* pData);
HI_S32 IRQ_config(HI_S32 (*gpio_callback)(HI_U32 gpio_para));


#endif
