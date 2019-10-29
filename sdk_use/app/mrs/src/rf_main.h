#ifndef __MAIN_H__
#define __MAIN_H__

#include "Rf_common.h"
//Matteo
#include "../inc/app_config.h"


//去掉"x"采用消息模式处理RF收发
#define xMSG_NOT_INT

#pragma pack(1)
enum  rfTask_ID
{
  rfRecMsg = 0x01,
  uartRecMsg,
  rfTimerMsg
};


#pragma pack()

void assert_failed(u8* file, u32 line);

void rf_main(void);

void  scanRfChannle(void);

void  rfProcessIrq(void);

void waitRfOverTimer(void);

void  rfWLTx(u8 channel);

u16 testFunc(void);

void rf_main_task(u32 *pMsg);


void  enableRfSendReceive(void);

void  disbaleRfSendReceive(void);

#endif /* __MAIN_H__ */

