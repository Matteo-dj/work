#ifndef _PHY_H
#define	_PHY_H

#include "Rf_common.h"

#pragma pack(1)

#define aMaxPHYPayloadSize 252

typedef struct PhyFrameHead
{
  u8 FrameLen;
  u8 ChannelIndex;
  u8 StandardNum;
  u8 FrameHeadCRC;	
}sPhyFrameHead;

#pragma pack()
u8 	SgdPhyGetRSSIThreshold(void);

u8 	SgdPhyGetRSSIValue(void);

u8	SgdPhyGetAverRSSI(void);

u8      SgdGetInputPower(void);

bool    SgdPhyParseFrame( sBuffer *pBuff );

void    SgdPhyFormFrame( u8 channelIndex );

u16 	SgdPhyCalCrc( u8 *buf, u16 len );

void  	SgdPhySetCurrentChannel( u8 CurrentChannel, u8 Group );

u16 	SgdPhyWhitingData( u8 *data, u16 WhitingPN9 );

void    SgdPhyWhitingBuf( u8 *buff, u16 len );

#endif

