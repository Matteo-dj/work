/*! @file radio.c
 * @brief This file contains functions to interface with the radio chip.
 *
 * @b COPYRIGHT
 * @n Silicon Laboratories Confidential
 * @n Copyright 2012 Silicon Laboratories, Inc.
 * @n http://www.silabs.com
 */

#include "bsp.h"
#include "Public.h"
#include "phy.h"
#include "radio.h"
#include "delay.h"
/*****************************************************************************
 *  Local Macros & Definitions
 *****************************************************************************/

/*****************************************************************************
 *  Global Variables
 *****************************************************************************/
SEGMENT_VARIABLE(Radio_Configuration_Data_Array[], U8, SEG_CODE) = \
              RADIO_CONFIGURATION_DATA_ARRAY;

SEGMENT_VARIABLE(RadioConfiguration, tRadioConfiguration, SEG_CODE) = \
                        RADIO_CONFIGURATION_DATA;

SEGMENT_VARIABLE_SEGMENT_POINTER(pRadioConfiguration, tRadioConfiguration, SEG_CODE, SEG_CODE) = \
                        &RadioConfiguration;


SEGMENT_VARIABLE(customRadioPacket[RADIO_MAX_PACKET_LENGTH], U8, SEG_XDATA);

/*****************************************************************************
 *  Local Function Declarations
 *****************************************************************************/
void vRadio_PowerUp(void);

/*!
 *  Power up the Radio.
 *
 *  @note
 *
 */
void vRadio_PowerUp(void)
{
  SEGMENT_VARIABLE(wDelay,  U16, SEG_XDATA) = 0u;

  /* Hardware reset the chip */
  si446x_reset();

  /* Wait until reset timeout or Reset IT signal */
  for (; wDelay < pRadioConfiguration->Radio_Delay_Cnt_After_Reset; wDelay++);
}

/*!
 *  Radio Initialization.
 *
 *  @author Sz. Papp
 *
 *  @note
 *
 */
void vRadio_Init(void)
{
  volatile U16 wDelay;
  //bool led_flag = FALSE;
  //U8 tryInitRadioTimes = 0;
  /* Power Up the radio chip */
  vRadio_PowerUp();

  /* Load radio configuration */
  while ((SI446X_SUCCESS != si446x_configuration_init(pRadioConfiguration->Radio_ConfigurationArray))) //&& (tryInitRadioTimes < 5))
  {
  	//tryInitRadioTimes++; 
	/*
    if(led_flag == FALSE)
    {
      led_flag = FALSE;
    }
    else
    {
      led_flag = TRUE;
    }
    */
    for (wDelay = 0x7FFF; wDelay--; ) ;
    /* Power Up the radio chip */
    vRadio_PowerUp();
  }

  // Read ITs, clear pending ones
  si446x_get_int_status(0u, 0u, 0u);
  si446x_set_property(0x20,1,0x4e,0x40);  //compen - 0x40
  
  //频率校准
  if(0x01 == Flash_Readbyte(ADJUST_FREQUENCY_FLAG))
  {
    vAdjustFrequencyOffset(Flash_Readbyte(CARRIER_FREQUENCY_HIGH),Flash_Readbyte(CARRIER_FREQUENCY_LOW));
  }

  //设置发射功率
  u8 txPower = Flash_Readbyte( TX_POWER );
  if( txPower > 0x07 )txPower = 0x00;
  vSetRadioTxpower(txPower);
  
//4438的ID信息获取  
//si446x_part_info(); 
  
//476.3M输出功率测试  
//  u8 chnnlIndx = 0;                  
//  u8 chnnlNo = CaculateChannelNo(chnnlIndx&0x01,chnnlIndx>>1);
//  vRadio_TxPN9_Code(chnnlNo);
  
//476.3灵敏度测试，需更改配置文件  
//vRadio_StartRX(24, 252);
//  while(1)
//  { 
//  //独立看门狗喂狗
//    resetWatchDog();
//  }   
}

/*!
 *  Set Radio to RX mode. .
 *
 *  @param channel Freq. Channel,  packetLength : 0 Packet handler fields are used , nonzero: only Field1 is used
 *
 *  @note
 *
 */
void vRadio_StartRX(U8 channel, U16 packetLenght )
{
  // Read ITs, clear pending ones
  si446x_get_int_status(0u, 0u, 0u);

   // Reset the Rx Fifo
   si446x_fifo_info(SI446X_CMD_FIFO_INFO_ARG_FIFO_RX_BIT);

  /* Start Receiving packet, channel 0, START immediately, Packet length used or not according to packetLength */
  si446x_start_rx(channel, 0u, packetLenght,
                  SI446X_CMD_START_RX_ARG_NEXT_STATE1_RXTIMEOUT_STATE_ENUM_NOCHANGE,
                  SI446X_CMD_START_RX_ARG_NEXT_STATE2_RXVALID_STATE_ENUM_READY,
                  SI446X_CMD_START_RX_ARG_NEXT_STATE3_RXINVALID_STATE_ENUM_RX );
}

void vRadio_StartTX(U8 channel, U8 packetLenght )
{
  si446x_fifo_info(SI446X_CMD_FIFO_INFO_ARG_FIFO_TX_BIT);
  
  si446x_get_int_status(0u, 0u, 0u);

  si446x_start_tx(channel, 0x80, packetLenght);
}

/*!
 *  Set Radio to TX mode, variable packet length.
 *
 *  @param channel Freq. Channel, Packet to be sent length of of the packet sent to TXFIFO
 *
 *  @note
 *
 */
 void vRadio_TxPN9_Code(U8 chnnelNo)
{
  si446x_set_property(0x20,0x01,0x00,0x10);
  
  //GPIO_ResetBits(RF_RAMP_CTL_PORT, RF_RAMP_CTL_PIN);
  si446x_start_tx(chnnelNo, 0x80, 0x00);
}
void vRadio_Set_Txpower(U8 powerlvl)
{
  si446x_set_property(0x22,0x01,0x01,powerlvl);
}
 void Shutdown_TxPN9_Code(void)
{
  si446x_set_property(0x20,0x01,0x00,0x03);
  si446x_change_state(SI446X_CMD_CHANGE_STATE_ARG_NEXT_STATE1_NEW_STATE_ENUM_READY);
}

void vSetRadioTxpower(INT8U powerlvl)
{
  switch( powerlvl)
  {
  case 0x00:
    if(0x01 == Flash_Readbyte(TX_POWER_FLAG))
    {
      si446x_set_property(0x22,0x01,0x01,Flash_Readbyte(TX_POWER_VALUE));//17
    }
    else
    {
      si446x_set_property(0x22,0x01,0x01,62);//17
    }
    break;
  case 0x01:
    si446x_set_property(0x22,0x01,0x01,30);//11 
    break;
  case 0x02:
    si446x_set_property(0x22,0x01,0x01,16);//5  
    break;
  case 0x03:
    si446x_set_property(0x22,0x01,0x01,9); // -1 
    break;
  case 0x04:
    si446x_set_property(0x22,0x01,0x01,126);//20 
    break;
  case 0x05:
    si446x_set_property(0x22,0x01,0x01,40);//14  
    break;       
  case 0x06:
    si446x_set_property(0x22,0x01,0x01,22);//8  
    break;    
  case 0x07:
    si446x_set_property(0x22,0x01,0x01,14);// 2   
    break; 
  default:
    si446x_set_property(0x22,0x01,0x01,62);//17dbm 
    break;
  }
  return;
}

void RF_RxOn(bool interrupt_06, u8 channalInx)
{
  u8 chnnlNo = CaculateChannelNo(channalInx&0x01,channalInx>>1);
  vRadio_StartRX(chnnlNo, 0);
}

void RF_TxOn(bool interrupt_06, u8 channalInx)
{
  u8 chnnlNo = CaculateChannelNo(channalInx&0x01,channalInx>>1);
  vRadio_StartTX(chnnlNo, 0);
}

void vSetRadioDataRate_25K(void)
{
    si446x_set_property(0x20,0x04,0x03,0x07, 0xA1, 0x20, 0x09);
}

void vSetRadioDataRate_10K(void)
{
    si446x_set_property(0x20,0x04,0x03,0x06, 0x1A, 0x80, 0x05);
}

void vSetRadioDataRate_40K(void)
{
    si446x_set_property(0x20,0x04,0x03,0x0C, 0x35, 0x00, 0x09);
}

void vAdjustFrequencyOffset(INT8U hbyte,INT8U lbyte)
{
  si446x_change_state(SI446X_CMD_CHANGE_STATE_ARG_NEXT_STATE1_NEW_STATE_ENUM_READY);
  for (volatile u32 wDelay0 = 0x7fff; wDelay0 > 0; wDelay0--);
  si446x_set_property(0x20,0x02,0x0d,hbyte,lbyte);
}
