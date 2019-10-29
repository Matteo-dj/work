/**
******************************************************************************
* @file    delay.h 
* @author  zouliang
* @version V1.0
* @date    2011-11-09
* @brief   
******************************************************************************
*/
#ifndef _DELAY_H
#define _DELAY_H

#include "Rf_common.h"

#define TICKS_PER_SECOND    1000    //1Khz

#pragma pack(1)

#pragma pack()

void Delay(__IO u32 nTime);
void TimingDelay_Decrement(void);
void CpuSpeed(u8 ticks);
void usDelay(u32 us);
void msDelay(u32 ms);

#endif