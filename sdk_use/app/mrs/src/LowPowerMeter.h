#ifndef _LOW_POWER_METER_H
#define	_LOW_POWER_METER_H

#include "Rf_common.h"
#include "phy.h"
#include "mac.h"
#include "nwk.h"
#include "app.h"
#include "task.h"
#include "fifo_API.h"

#pragma pack(1)

typedef struct LowPowerPhyPayloadHead
{
  u8  FCD[2];
  u8  SN;
  u8  PanID[2];
  u8  dstAddr[7];
  u8  srcAddr[7];
}sLowPowerPhyPayloadHead;

extern sWaterMeterData waterMeterBuf[TOTAL_WART_METER_SIZE+1];
extern FIFO    waterMeteFifoCtr;

typedef struct  lowPowerNeighbour
{
  u8  addr[7];
  u8  inputPower;
  u32 timer;
}sLowPowerNeighbour ;
extern sLowPowerNeighbour gLowPowerNeighbour[MAX_LP_NEIGHBOURS+1];
extern FIFO    waterMeteNeiFifoCtr;

#pragma pack()
u8  SgdGetFreSteps( u8 channelGroupNum  );

void  SgdWakeUpLowPowerMeter( u16 shortAddr, u8 channelGroupNum  );

void  SgdSetRFTxParams( u8 channelGroupNum );

void  SgdSetRFRxParams(void);

void  SgdBroadLowPowerMeterRelayTSFrame( sMAC_BroadFramePayload *pPayload,u8 channelGroupNum );

void  SgdParseLowPowerMeterFrame( u8 *buf, u16 len );

void  SgdUpDateLPNeighbourList( u8 *addr );


void  SgdRelayLowPowerMeterCommand( u8 *buf, u16 len,u16 shortAddr,u8 channelGroupNum );

void  SgdNwkFreeLPNeighbourList(void);

void  SgdFormLowPowerMeterRespondFrame( u8 *pInData, u8 inLen );

void SgdParseLowPowerMeterUPdat( u8 *buf, u16 len );

void vResetPrmbAndSysConfig(void);


#endif
