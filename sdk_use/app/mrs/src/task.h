#ifndef __TASK_H__
#define __TASK_H__

#include "Rf_common.h"

#define		__IO		volatile  
typedef	void (*pFunction)(void);

#pragma pack(1)

enum	TestCommdMode
{
  CMD_MODE_0,
  CMD_MODE_1,
  CMD_MODE_2,
  CMD_MODE_3,
  CMD_MODE_4,
  CMD_MODE_5
};

#pragma pack()
void  SgdGetNodeParameters(void);
bool  SgdGetMeterAddr(void);
#if 0
u8    SgdFormGetAddrFrame( u8 *pOutBuff );
#endif
//void  SgdSetRandomSeed(void);
void  SgdGetRandNumber(void);



void  SgdSetListeningChannel(void);

void  SgdEventReportedProcess(void);

void  SgdActiveJoinNetProcess(void);

void  SgdWLTx(u8 channel,bool reCalchannel);

void rfReceiveHplcDat(u8 * inDat,u16 inLen);

void  SgdUsartProcess(void);

u8  SgdParseUartData(void);

void  SgdTimerProcess(void);

void  SgdSaveConfigInfo( u8 *buf, u8 len );

void  SgdDeleteConfigInfo(void);

void  SgdLedProcess(void);

void  SgdScanChannelProcess(void);

void  SgdWLRecvProcess(void);

void  SgdWLReceiving(void);

void  SgdWLSendProcess(void);

void  SgdNodeOffNetWorkProcess(void);

void  SgdUpdateRssiThresholdProcess(void);

void  SgdNormalResponsePro_645( sControlCode_645 *pCtrCode, u8 extendCI );

void  SgdExceptionResponsePro_645( sControlCode_645 *pCtrCode, u8 extendCI );

bool  SgdGetEventReportStatusWord(void );

void snodeUpgrade(void);

void exeUpgrad(void);

void checkNeighbour(void);

void AppJumpBootProgram(u32 jumpAddr);

bool SgdIsProtocol_645(u8 *pInDat,u16 inLen,u16 *starCodeSqe);//Suxy_20180122---bool SgdIsProtocol_645(u8 *pInDat,u16 inLen);

bool SgdIsProtocol_698( u8 *pInDat,u16 inLen,u8 *pServerAddr, u8 *addrLength,u16 *starCodeSqe);//Suxy_20180122---bool SgdIsProtocol_698( u8 *pInDat,u16 inLen,u8 *pServerAddr, u8 *addrLength);

void snodeUpgrade(void);

void uartRelayPlcBoardFrame(u8 *inDat, u16 inLen );

void uartRelayPlcWaterDat(u8 *inDat, u16 inLen ,u8 extendCI);

void respondPlcConfirm(u8 extendCmd);
void sendMsgToHplc(void);
void relayLPWDatToWirless(u8 *inDat,u16 inLen);

#endif