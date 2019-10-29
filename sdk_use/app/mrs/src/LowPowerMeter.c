#include  "LowPowerMeter.h"
#include "delay.h"
#include "radio.h"
#include "bsp.h"
#include "Public.h"
#include "fifo_API.h"
#include "nwk.h"
#include "rf_main.h"


extern u8  appRecvFrameNumber;

sWaterMeterData waterMeterBuf[TOTAL_WART_METER_SIZE+1];
FIFO    waterMeteFifoCtr;
sLowPowerNeighbour gLowPowerNeighbour[MAX_LP_NEIGHBOURS+1];
FIFO    waterMeteNeiFifoCtr;
/*
说明：设置前导码为0,1字节的同步码：0x55
*/
static void vLowPowerMtWakeupConfig(void)
{
    si446x_set_property(0x10,0x01,0x00,0x00);
    si446x_set_property(0x11,0x03,0x00,0x20,0x55,0x00);
}

/*
说明：设置前导码为80,2字节的同步码：0x98,0xF3
*/
void vResetPrmbAndSysConfig(void)
{
    si446x_set_property(0x10,0x01,0x00,0x50);
    si446x_set_property(0x11,0x03,0x00,0x21,0x98,0xF3);
}
/***************************************
*输入:shortAddr
*输出:无
*返回:无
*描述:唤醒低功耗表计
*作者:张月保
***************************************/
void  SgdWakeUpLowPowerMeter( u16 shortAddr, u8 channelGroupNum )
{ 
  gwarteBoard.channel   = channelGroupNum;
  gwarteBoard.shortAddr = shortAddr;
  gwarteBoard.ctl.wakeUpFlag = TRUE;
  gwarteBoard.ctl.wakeUpNum  = 50;
  gwarteBoard.ctl.wakeUpLen  = 124;
  gwarteBoard.ctl.TxLen      = FIFO_SIZE;
  //HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_080, HI_DIAG_MT("[SgdWakeUpLowPowerMeter]唤醒低功耗表计"));
  memset( &sWLTx, 0x00, sizeof(sWLTx) );
  memset( sWLTx.sBuff.data, 0x55, 124 );
  sWLTx.sBuff.data[119] = 0x0C; 
  sWLTx.sBuff.data[120] = gwarteBoard.ctl.wakeUpNum;
  sWLTx.sBuff.data[121] = shortAddr & 0xFF;
  sWLTx.sBuff.data[122] = shortAddr >> 8;
  sWLTx.sBuff.data[123] = SgdGetSum( &sWLTx.sBuff.data[120], 3);
  SgdPhyWhitingBuf( &sWLTx.sBuff.data[120], 4 );
  sWLTx.sBuff.len = 6375;  
  //设置通信速率 25K
  vSetRadioDataRate_25K();
  //调频到指定工作频点
  u8 steps = SgdGetFreSteps( channelGroupNum );
  
  vLowPowerMtWakeupConfig();
  HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_028, HI_DIAG_MT("[SgdWakeUpLowPowerMeter]25k"));
  rfWLTx(steps);
}

void  SgdSetRFTxParams( u8 channelGroupNum )
{
#if 0 //CJJ  
  u8 steps = 0;
  	
  //设置通信速率 25K
  SPI4432WriteRegister(TXDataRate1_6E, 0xCC);
  SPI4432WriteRegister(TXDataRate0_6F, 0xCD); 
  SPI4432WriteRegister(ModulationModeControl1_70, 0x24);

  //调频到指定工作频点
  steps = SgdGetFreSteps( channelGroupNum );
  SPI4432WriteRegister( FrequencyHoppingChannelSelect_79, steps );
#endif	
}

void  SgdSetRFRxParams(void)
{
#if 0 //CJJ    
  //TX Data Rate Settings(空中传输速率 10K)
  SPI4432WriteRegister(TXDataRate1_6E, 0x51);
  SPI4432WriteRegister(TXDataRate0_6F, 0xec); 
  SPI4432WriteRegister(ModulationModeControl1_70, 0x20);  
#endif  
  vSetRadioDataRate_10K();
  //HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_025, HI_DIAG_MT("[SgdSetRFRxParams]10k"));
}

u8  SgdGetFreSteps( u8 channelGroupNum  )
{
  u8 steps = 0;
  if( channelGroupNum == 0 )
  {
    steps = 91;
  }
  else if( (channelGroupNum > 0) && (channelGroupNum <= 16) )
  {
    steps = 74 + channelGroupNum;
  }
  else if( channelGroupNum == 17 )
  {
    steps = 92;
  }
  else
  {
    steps = 80 + channelGroupNum;
  }
  
  return steps;
}

/***************************************
*输入:shortAddr
*输出:无
*返回:无
*描述:DAU广播低功耗表计转发时隙命令（0x42）
*作者:张月保
***************************************/
void  SgdBroadLowPowerMeterRelayTSFrame( sMAC_BroadFramePayload *pPayload,u8 channelGroupNum )
{}

/***************************************
*输入:buf, len
*输出:无
*返回:无
*描述:解析主动式上报帧
*作者:CJJ
***************************************/
void SgdParseLowPowerMeterUPdat( u8 *buf, u16 len )
{
  u8 pos = 0;
  sWaterMeterLoadHead *p = (sWaterMeterLoadHead *)buf;
  sWaterMeterLoadEnd *p1 = (sWaterMeterLoadEnd *)&buf[sizeof(sWaterMeterLoadHead)+(p->len)];
  if(0x68 != p->start) return;
  if(0x84 != p->command) return;
  if(0x16 != p1->end) return;
  if(SgdGetSum(buf,sizeof(sWaterMeterLoadHead)+(p->len)) != p1->cs) return;
  pos += sizeof(sWaterMeterLoadHead);
  if(0xc6 != buf[pos]) return;
  if(SgdGetSum(&buf[pos],(p->len)-1) != buf[pos + (p->len)-1]) return;
  pos++;
  
  
  u8 WaterMeterDataNums = FIFO_COUNT(&waterMeteFifoCtr,TOTAL_WART_METER_SIZE+1);
  sWaterMeterData *pTmp = NULL;
  //检索已保存的上报式水表，如果地址有相同的，则更新数据
  for(u8 i = 0;i < WaterMeterDataNums;i++)
  {
    pTmp = &waterMeterBuf[FIFO_POP_IDX(&waterMeteFifoCtr,TOTAL_WART_METER_SIZE+1,i)];
    
    if(0 == memcmp(pTmp->addr,p->addr,sizeof(pTmp->addr)))
    {
      pTmp->inputPower = sNode.inputPower;
      memcpy(pTmp->time,buf+pos,sizeof(sWaterMeterData)-12);
      pTmp->timer = wv_ticks;
      return;
    }
  }
  
//CJJ-2018-6-11：信号强的替代信号弱的，不使用先入先出的方式  
//  //如果FIFO已满，则弹出栈底数据
//  if (FIFO_FULL(&waterMeteFifoCtr,TOTAL_WART_METER_SIZE+1))
//  {
//    u8 inVaild = FIFO_POP(&waterMeteFifoCtr,TOTAL_WART_METER_SIZE+1);
//  } 
//  //将数据写入栈顶位置
//  pTmp = &waterMeterBuf[FIFO_PUSH(&waterMeteFifoCtr,TOTAL_WART_METER_SIZE+1)];
//  pTmp->inputPower = sNode.inputPower;
//  memcpy(pTmp->addr,p->addr,7);
//  memcpy(pTmp->time,buf+pos,sizeof(sWaterMeterData)-12);
//  pTmp->timer = wv_ticks;
  
  //如果FIFO未满，直接存
  if (!FIFO_FULL(&waterMeteFifoCtr,TOTAL_WART_METER_SIZE+1))
  {
    pTmp = &waterMeterBuf[FIFO_PUSH(&waterMeteFifoCtr,TOTAL_WART_METER_SIZE+1)];
    pTmp->inputPower = sNode.inputPower;
    memcpy(pTmp->addr,p->addr,7);
    memcpy(pTmp->time,buf+pos,sizeof(sWaterMeterData)-12);
    pTmp->timer = wv_ticks;
  } 
  //替换信号最弱的
  else
  {
    u8 inputPower = 0;
    sWaterMeterData *pChange = NULL;
    
    for(u8 i = 0;i < WaterMeterDataNums;i++)
    {
       pTmp = &waterMeterBuf[FIFO_POP_IDX(&waterMeteFifoCtr,TOTAL_WART_METER_SIZE+1,i)];
       if((pTmp->inputPower) > inputPower)
       {
          inputPower = pTmp->inputPower;
          pChange = pTmp;
       }
    }
    if((NULL != pChange) && ((pChange->inputPower) > sNode.inputPower))
    {
      pChange->inputPower = sNode.inputPower;
      memcpy(pChange->addr,p->addr,7);
      memcpy(pChange->time,buf+pos,sizeof(sWaterMeterData)-12);
      pChange->timer = wv_ticks;
    }
  }
}

/***************************************
*输入:buf, len
*输出:无
*返回:无
*描述:解析低功耗表计协议
*作者:张月保
***************************************/
void  SgdParseLowPowerMeterFrame( u8 *buf, u16 len )
{
  u8 pos = 0;
  u8 commandId = 0;
  sLowPowerPhyPayloadHead *pPayloadHead = (sLowPowerPhyPayloadHead *)buf;
  pos = sizeof(sLowPowerPhyPayloadHead);
  commandId = buf[pos++];
  HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_026, HI_DIAG_MT("[SgdParseLowPowerMeterFrame]commandId"),commandId);
  switch( commandId )
  {
  case  0x40:
  case  0x44:
  case  0x4A:
    break;
    #if 0
    {
      if( commandId == 0x44 )
        sNode.waterChannelGroupNum = buf[pos];
      shortAddr = SgdPhyCalCrc( pPayloadHead->dstAddr, 7 );
      if( shortAddr == 0x9999 )
        shortAddr = 0x999A;
      SgdWakeUpLowPowerMeter( shortAddr, channelIndexArr[recvChannelArrPos] >> 1 );
      SgdRelayLowPowerMeterCommand( buf, len,channelIndexArr[recvChannelArrPos] >> 1 );
    }
    break;
    #endif
  case  0x41:
  case  0x45:
  case  0x4B:
    //SgdFormLowPowerMeterRespondFrame( buf, len );
    //CJJ-2018-7-4:串口透传物理层报文至HPLC
    uartRelayPlcWaterDat(sWLRx.sBuff.data,sWLRx.sBuff.len+6,HPLC_SEND_LPW_DAT);
    break;    
  case  0x42:
    break;
  case  0x43:
    SgdUpDateLPNeighbourList( pPayloadHead->srcAddr );
    break;  
  case  0x46:
    break;
  case  0x47:
    break;
  case  0xAA:
    break;
    #if 0
      shortAddr = 0x9999;
      SgdWakeUpLowPowerMeter( shortAddr, channelIndexArr[recvChannelArrPos] >> 1 );      
      SgdRelayLowPowerMeterCommand( buf, len ,channelIndexArr[recvChannelArrPos] >> 1);
    break;
    #endif
  default:
    break;
  }    
}

/***************************************
*输入:addr
*输出:无
*返回:无
*描述:更新低功耗表计邻居表
*作者:张月保
***************************************/
void  SgdUpDateLPNeighbourList( u8 *addr )
{
  u8 WaterMeterNeiDataNums = FIFO_COUNT(&waterMeteNeiFifoCtr,MAX_LP_NEIGHBOURS+1);
  sLowPowerNeighbour *pTmp = NULL;
  //HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1002, HI_DIAG_MT("[WATER]addr"), addr, 6);
  //如果地址相同，则更新场强值
  for(u8 i = 0;i < WaterMeterNeiDataNums;i++)
  {
    pTmp = &gLowPowerNeighbour[FIFO_POP_IDX(&waterMeteNeiFifoCtr,MAX_LP_NEIGHBOURS+1,i)];
    
    if(0 == memcmp(pTmp->addr,addr,sizeof(pTmp->addr)))
    {
      pTmp->inputPower = sNode.inputPower;
      pTmp->timer      = wv_ticks;
      return;
    }
	//weichao
	//HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1001, HI_DIAG_MT("[WATER]gLowPowerNeighbour[i]"), &gLowPowerNeighbour[i], sizeof(sLowPowerNeighbour));
  }
  
  //如果FIFO未满，则直接进栈
  if (!FIFO_FULL(&waterMeteNeiFifoCtr,MAX_LP_NEIGHBOURS+1))
  {
    pTmp = &gLowPowerNeighbour[FIFO_PUSH(&waterMeteNeiFifoCtr,MAX_LP_NEIGHBOURS+1)];
    pTmp->inputPower = sNode.inputPower;
    pTmp->timer      = wv_ticks;
    memcpy(pTmp->addr,addr,7);
    LPNeighbourNums ++;
	//HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_022, HI_DIAG_MT("[WATER]push"));
  } 
  //替换信号最弱的
  else
  {
    u8 inputPower = 0;
    sLowPowerNeighbour *pChange = NULL;
    //HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_023, HI_DIAG_MT("[WATER]change"));
    for(u8 i = 0;i < WaterMeterNeiDataNums;i++)
    {
       pTmp = &gLowPowerNeighbour[FIFO_POP_IDX(&waterMeteNeiFifoCtr,MAX_LP_NEIGHBOURS+1,i)];
       if((pTmp->inputPower) > inputPower)
       {
          inputPower = pTmp->inputPower;
          pChange = pTmp;
       }
    }
    if((NULL != pChange) && ((pChange->inputPower) > sNode.inputPower))
    {
      pChange->inputPower = sNode.inputPower;
      pChange->timer      = wv_ticks;
      memcpy(pChange->addr,addr,7);
    }
  }
}

/***************************************
*输入:buf,len
*输出:无
*返回:无
*描述:转发低功耗表计请求命令
*作者:张月保
***************************************/
void  SgdRelayLowPowerMeterCommand( u8 *buf, u16 len,u16 shortAddr,u8 channelGroupNum )
{
  if(((gwarteBoard.ctl.wakeUpNum >= 50) && (gwarteBoard.ctl.TxLen == 124)) || ( (gwarteBoard.ctl.TxLen == 125) && (gwarteBoard.ctl.wakeUpNum > 0) ))
  {
    gwarteBoard.ctl.TxLen = 0;
    sWLTx.sBuff.data[119] = 0x55; 
    sWLTx.sBuff.data[120] = 0x0C;
    sWLTx.sBuff.data[121] = --gwarteBoard.ctl.wakeUpNum;
    sWLTx.sBuff.data[122] = shortAddr & 0xFF;
    sWLTx.sBuff.data[123] = shortAddr >> 8;
    sWLTx.sBuff.data[124] = SgdGetSum( &sWLTx.sBuff.data[121], 3);
    gwarteBoard.ctl.wakeUpLen = 125;
    SgdPhyWhitingBuf( &sWLTx.sBuff.data[121], 4 );
  }

  si446x_get_int_status(0u, 0u, 0u);
  if (Si446xCmd.GET_INT_STATUS.PH_PEND & SI446X_CMD_GET_INT_STATUS_REP_PH_PEND_PACKET_SENT_PEND_BIT)
  {
    si446x_change_state(SI446X_CMD_CHANGE_STATE_ARG_NEXT_STATE1_NEW_STATE_ENUM_READY);
    si446x_fifo_info(SI446X_CMD_FIFO_INFO_ARG_FIFO_TX_BIT);
    vResetPrmbAndSysConfig();
    memset( &sWLTx, 0x00, sizeof(sWLTx) );
    
    if(NULL != buf && (len > 0 && len < RF_MAX_BUF_LEN))
    {
      sWLTx.sBuff.len = len;
      memcpy(sWLTx.sBuff.data,buf,sWLTx.sBuff.len);
    }

    gwarteBoard.ctl.wakeUpFlag = FALSE;
    gwarteBoard.ctl.datSendFlag = TRUE;
    SgdPhyWhitingBuf( sWLTx.sBuff.data, sWLTx.sBuff.len );
    vSetRadioDataRate_25K();
    u8 steps = SgdGetFreSteps( channelGroupNum );
    rfWLTx(steps);
  }
  
  if ( (gwarteBoard.ctl.TxLen < gwarteBoard.ctl.wakeUpLen) && (Si446xCmd.GET_INT_STATUS.PH_PEND & SI446X_CMD_GET_INT_STATUS_REP_PH_PEND_TX_FIFO_ALMOST_EMPTY_PEND_BIT))
  {
    u8  wLen = 0;
    if ( (gwarteBoard.ctl.wakeUpLen - gwarteBoard.ctl.TxLen) > 34 )
    {
      wLen = 34;    
    }
    else
    {
      wLen = gwarteBoard.ctl.wakeUpLen - gwarteBoard.ctl.TxLen;
    }
    si446x_write_tx_fifo( wLen, &sWLTx.sBuff.data[gwarteBoard.ctl.TxLen] );
    gwarteBoard.ctl.TxLen += wLen;
  }
}

/***************************************
*输入:无
*输出:无
*返回:无 
*描述:删除低功耗表计邻居表
*作者:张月保
***************************************/
#if 0
void SgdNwkFreeLPNeighbourList(void)
{
  sLowPowerNeighbour *pHead = NULL;
  pHead = sLPNeighbourList.front;
  while( pHead != NULL )
  {
    sLPNeighbourList.front = pHead->next;
    free( pHead );
    pHead = NULL;
    if( sLPNeighbourList.front == NULL )
    {
      sLPNeighbourList.rear = NULL;
    }
    pHead = sLPNeighbourList.front;
  }
  LPNeighbourNums = 0;
}
#endif

/***************************************
*输入:pInData,inLen
*输出:无
*返回:无
*描述:构造低功耗表计Aps层命令应答帧
*作者:张月保
***************************************/
void  SgdFormLowPowerMeterRespondFrame( u8 *pInData, u8 inLen )
{
  memset( &sWLTx, 0x00, sizeof(sWLTx) );
  sWLTx.sBuff.data[sWLTx.sBuff.len++] = 0x01; //帧控制域
  sWLTx.sBuff.data[sWLTx.sBuff.len++] = appRecvFrameNumber; //帧序号
  sWLTx.sBuff.data[sWLTx.sBuff.len++] = RelayLowPowerMeterFrame; //命令标识
  memcpy( &sWLTx.sBuff.data[sWLTx.sBuff.len], pInData, inLen ); 
  sWLTx.sBuff.len += inLen;

  SgdNwkFormDataFrame( &sWLTx.sBuff );
}
