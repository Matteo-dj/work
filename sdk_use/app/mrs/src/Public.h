//============================================================================
// Name        : Public.h
// Author      : 韩正玉
// Version     : v1.0
// Date 			 : 16-9-10
// Copyright   : 深国电
// Description : Public API
//============================================================================

#ifndef PUBLIC_H
#define PUBLIC_H
#include  "rf_common.h"
#include "compiler_defs.h"
#include "si446x_api_lib.h"

typedef unsigned char INT8U;
typedef unsigned short INT16U;
typedef unsigned int INT32U;  
typedef enum {True = 1, False= 0} Bool;
typedef enum {OFF = 0,ON = 1}SwitchStatus;

#define CRC_POLY 		0x8408

#define RX_FIFO_ALMOST_FULL_BYTES 50
#define WLESS_PROTOCOL_GROUP_MAX  32
#define ALMOST_FULL_RX            30

INT8U CaculateChannelNo( INT8U CurrentChannel, INT8U Group );


#endif

