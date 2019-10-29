#ifndef _DEBUG_H
#define	_DEBUG_H

#include "Rf_common.h"

#define RF_TX_TEST_MAX_LEN ((u32)40000)

#pragma pack(1)


#pragma pack()
void  SgdRFTxPN9Code( u8 *pIn );

void  SgdRFCalibrateFrequency( u8 *pIn );

void  SgdRFTxTest(void );

void  SgdRFCalibrateTXpower( u8 *pIn );

void SgdRFCalibrateFrequencyTXpower( u8 *pIn );

void rfTestTimer(void);

void  SgdRFTxTestSend(void);

#endif