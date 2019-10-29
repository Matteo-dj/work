/*!
 * File:
 *  radio_comm.h
 *
 * Description:
 *  This file contains the RADIO communication layer.
 *
 * Silicon Laboratories Confidential
 * Copyright 2011 Silicon Laboratories, Inc.
 */
#ifndef _RADIO_COMM_H_
#define _RADIO_COMM_H_


                /* ======================================= *
                 *              I N C L U D E              *
                 * ======================================= */


                /* ======================================= *
                 *          D E F I N I T I O N S          *
                 * ======================================= */

//#define RADIO_CTS_TIMEOUT 255
#define RADIO_CTS_TIMEOUT 60000

                /* ======================================= *
                 *     G L O B A L   V A R I A B L E S     *
                 * ======================================= */

extern SEGMENT_VARIABLE(radioCmd[16u], U8, SEG_XDATA);


                /* ======================================= *
                 *  F U N C T I O N   P R O T O T Y P E S  *
                 * ======================================= */

#if (defined SILABS_RADIO_SI446X) || (defined SILABS_RADIO_SI4455)
  U8 radio_comm_GetResponce(U8 byteCount, U8* pData);
  void radio_common_SendCmd(U8 byteCount, U8* pData);
  void radio_common_ReadData(U8 cmd, BIT pollCts, U8 byteCount, U8* pData);
  void radio_common_WriteData(U8 cmd, BIT pollCts, U8 byteCount, U8* pData);
#elif (defined SILABS_RADIO_SI4012)
  U8 radio_comm_GetResponce(U8 byteCount, U8* pData);
  U8 radio_common_SendCmd(U8 byteCount, U8* pData);
#endif

U8 rf_comm_PollCTS(void);
U8 radio_common_SendCmdGetResp(U8 cmdByteCount, U8* pCmdData, \
                             U8 respByteCount, U8* pRespData);
void radio_comm_ClearCTS(void);

#endif //_RADIO_COMM_H_
