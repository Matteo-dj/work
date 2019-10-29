/*! @file radio.h
 * @brief This file is contains the public radio interface functions.
 *
 * @b COPYRIGHT
 * @n Silicon Laboratories Confidential
 * @n Copyright 2012 Silicon Laboratories, Inc.
 * @n http://www.silabs.com
 */

#ifndef RADIO_H_
#define RADIO_H_

#include "radio_hal.h"
#include "compiler_defs.h"
/*****************************************************************************
 *  Global Macros & Definitions
 *****************************************************************************/
/*! Maximal packet length definition (FIFO size) */
#define RADIO_MAX_PACKET_LENGTH     64u
#pragma pack(1)

/*****************************************************************************
 *  Global Typedefs & Enums
 *****************************************************************************/
typedef struct
{
    U8   *Radio_ConfigurationArray;

    U8   Radio_ChannelNumber;
    U8   Radio_PacketLength;
    U8   Radio_State_After_Power_Up;

    U16  Radio_Delay_Cnt_After_Reset;

    U8   Radio_CustomPayload[RADIO_MAX_PACKET_LENGTH];
} tRadioConfiguration;

#pragma pack()
/*****************************************************************************
 *  Global Variable Declarations
 *****************************************************************************/
extern SEGMENT_VARIABLE_SEGMENT_POINTER(pRadioConfiguration, tRadioConfiguration, SEG_CODE, SEG_CODE);
extern SEGMENT_VARIABLE(customRadioPacket[RADIO_MAX_PACKET_LENGTH], U8, SEG_XDATA);

/*! Si446x configuration array */
extern SEGMENT_VARIABLE(Radio_Configuration_Data_Array[], U8, SEG_CODE);


/*****************************************************************************
 *  Global Function Declarations
 *****************************************************************************/
  void  vRadio_Init(void);
  void  vRadio_StartRX(U8,U16);
  U8 bRadio_Check_Ezconfig(U16);
  void vRadio_TxPN9_Code(U8 chnnelNo);
  void vRadio_Set_Txpower(U8 powerlvl);
  void Shutdown_TxPN9_Code(void);
  void vRadio_StartTx_Packet(U8 channel);
  void vRadio_Listening(void);
  void vRecycleListenChannel(void);
  void vSetRadioTxpower(U8 powerlvl);
  void RF_RxOn(bool interrupt_06, u8 channalInx);
  void RF_TxOn(bool interrupt_06, u8 channalInx);
  void vSetRadioDataRate_25K(void);
  void vSetRadioDataRate_10K(void);
  void vSetRadioDataRate_40K(void);
  void vAdjustFrequencyOffset(INT8U hbyte,INT8U lbyte);

#endif /* RADIO_H_ */

