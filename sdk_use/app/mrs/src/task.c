#include "rf_main.h"
#include "task.h"
#include "Rf_common.h"
#include "phy.h"
#include "mac.h"
#include "nwk.h"
#include "app.h"
#include "delay.h"
#include "debug.h"
#include "radio.h"
#include "Public.h"
#include "bsp.h"
#include "LowPowerMeter.h"
#include "phy.h"
#include "Mrs_srv_rf.h"

#define     UART_MAX_WAIT_TIME_645          800
#define     UART_MAX_WAIT_TIME_698          1200
#define     GET_METER_ADDR_ROUNDS           16   //读取电表地址自适应次数
#define     METER_PROTOAL_TYPE_97           1
#define     METER_PROTOAL_TYPE_07           2
#define     METER_PROTOAL_TYPE_698          3

#define     FRAME_START_CODE_645           0x68
#define     FRAME_END_CODE_645             0x16

#if defined(SIGNAL_MCU)&&defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
//static backUpgradPara_st backUpgradHead;
static u8 meterProtoalType;

#if 0
void  SgdGetNodeParameters(void)
{
  u8  tmp[13] = {0x68 ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0x68 ,0x1F ,0x01 ,0x13 ,0xFF,0x16};

  memset( sNode.longAddr, 0x99, sizeof(sNode.longAddr) );
  sUsart2.dataLen = sizeof(tmp);
  memcpy( (char *)sUsart2.dataBuffer, tmp, sUsart2.dataLen ); 
  sendMsgToHplc();
}
#endif

/***************************************
*输入:无
*输出:无
*返回:无
*描述:获取电表地址
*作者:张月保
***************************************/
bool  SgdGetMeterAddr(void)
{}

/***************************************
*输入:无
*输出:无
*返回:无
*描述:初始化随机数
*作者:张月保
***************************************/
void  SgdGetRandNumber(void)
{
  RF_RxOn(FALSE, 0x00);
  for ( sWLRx.sBuff.len = 0; sWLRx.sBuff.len < 200; sWLRx.sBuff.len++ )
  {
    sWLRx.sBuff.data[sWLRx.sBuff.len] = SgdPhyGetRSSIValue();
    usDelay(100);
  }
 
  gRandNumber = SgdPhyCalCrc( sWLRx.sBuff.data, sWLRx.sBuff.len );
  memset( &sWLRx, 0x00, sizeof(sWLRx) );
}


/***************************************
*输入:无
*输出:无
*返回:无
*描述:设置监听信道
*作者:张月保
***************************************/
void  SgdSetListeningChannel(void)
{}

/***************************************
*输入:无
*输出:无
*返回:无
*描述:检测是否有电表事件产生
*作者:张月保
***************************************/
void SgdEventReportedProcess(void)
{}

/***************************************
*输入:无
*输出:无
*返回:无
*描述:游离节点主动入网进程处理
*作者:张月保
***************************************/
void  SgdActiveJoinNetProcess(void)
{}

/***************************************
*输入:无
*输出:无
*返回:无
*描述:无线射频开启发射
*作者:张月保
***************************************/
void  SgdWLTx(u8 channel,bool reCalchannel)
{}

/***************************************
*输入:无
*输出:无
*返回:TRUE=分包发送完成，FALSE=未完成
*描述:698抄表分包发送
*作者:CJJ
***************************************/
bool appMulFrameRelay(bool formFrame)
{}

/*-----无线发送消息数据给HPLC--------*/
void sendMsgToHplc(void)
{
  HI_DMS_CHL_RX_S pstRxData = {0};

  pstRxData.bCanDiscardOldData = 1;
  pstRxData.usPayloadLen = sUsart2.dataLen;
  pstRxData.pPayload = sUsart2.dataBuffer;
  mrsStaRFRecvDealFun(&pstRxData);
}

/***************************************
*输入:无
*输出:无
*返回:无
*描述:处理串口任务
*作者:张月保
***************************************/
void rfReceiveHplcDat(u8 * inDat,u16 inLen)
{
  sUsart2.dataLen = inLen;
  memcpy(sUsart2.dataBuffer,inDat,sUsart2.dataLen);
  sUsart2.status.RxDone = STATUS_TRUE;
  SgdUsartProcess();
}


void  SgdUsartProcess(void)
{
  if( sUsart2.status.RxDone )
  {
    //串口接收完成
    sUsart2.status.RxDone = STATUS_FALSE;    
    SgdParseUartData();
    memset( (char *)&sUsart2, 0x00, sizeof(sUsart2) );  
  } 
}

/*------响应HPLC查询水表邻居场强命令------*/
void hplcGegNodeRssi(u8 pageIndex)
{
  u8 totalPages = 0;
  u8 nodeNum = 0;
  u8 WaterMeterNeiDataNums = FIFO_COUNT(&waterMeteNeiFifoCtr,MAX_LP_NEIGHBOURS+1);
  u8 pos = 0;

  if( WaterMeterNeiDataNums > 0 )
  {
    if( WaterMeterNeiDataNums < ONE_PAGE_NEIGHBOURS )
    {
      totalPages = 1;
    }
    else if( (WaterMeterNeiDataNums % 16) == 0 )
    {
      totalPages = (WaterMeterNeiDataNums >> 4);
    }
    else
    {
      totalPages = (WaterMeterNeiDataNums >> 4) + 1;
    }
    //如果页序号大于等于总页数，说明参数不正确，不响应
    if( pageIndex >= totalPages )
      pageIndex = 0;//return;
    nodeNum = ( pageIndex == (totalPages - 1) ) ? WaterMeterNeiDataNums -(pageIndex << 4) : ONE_PAGE_NEIGHBOURS;   
  }
  
  memset( (char *)&sUsart2, 0x00, sizeof(sUsart2) );
  sProHead_645 *pHead = (sProHead_645 *)&sUsart2.dataBuffer[sUsart2.dataLen];
  pHead->startCode1 = FRAME_START_CODE_645;
  pHead->startCode2 = FRAME_START_CODE_645;
  memcpy( pHead->addr, sNode.longAddr, LONG_ADDR_LEN );
  pHead->sCtrCode.functionCode = ExtendFunctionCode;
  pHead->dataLen  = 3 + (8*nodeNum);
  sUsart2.dataLen = sizeof(sProHead_645);
  sUsart2.dataBuffer[sUsart2.dataLen++] = HPLC_GET_NODE_RSSI;
  sUsart2.dataBuffer[sUsart2.dataLen++] = (totalPages << 4) + pageIndex;
  sUsart2.dataBuffer[sUsart2.dataLen++] = nodeNum;
  //获取邻居节点和RSSI值
  pos = sUsart2.dataLen;
  SgdNwkGetLowPowerMeterNeighbours( (u8 *)&sUsart2.dataBuffer[pos], &pos, nodeNum, pageIndex );
  sUsart2.dataLen = pos;
  sProEnd_645 *pEnd = (sProEnd_645 *)&sUsart2.dataBuffer[sUsart2.dataLen];
  pEnd->sumCrc = SgdGetSum( (u8 *)sUsart2.dataBuffer, sUsart2.dataLen );
  pEnd->endCode = FRAME_END_CODE_645;
  sUsart2.dataLen += sizeof(sProEnd_645);

  sendMsgToHplc();

}

/*-------转发HPLC多表协议数据至无线---------*/
void relayLPWDatToWirless(u8 *inDat,u16 inLen)
{
  u8 pos = 0,channel = 0,cmdID = 0;
  u16  shortAddr = 0;
  sLowPowerPhyPayloadHead *pPayloadHead = NULL;
  
  if(NULL != inDat && 0 != inLen)
  {
    channel = inDat[pos++];
    cmdID = inDat[pos++];
    channel >>= 1;
    
    pPayloadHead = (sLowPowerPhyPayloadHead *)&inDat[pos+sizeof(sPhyFrameHead)];
    shortAddr = SgdPhyCalCrc( pPayloadHead->dstAddr, 7 );
    if( shortAddr == 0x9999 )
    {
      shortAddr = 0x999A;
    }

    if((inLen-pos) <= WARTE_BOARD_MAX_LEN)
    {
      memset((char *)&gwarteBoard,0,sizeof(gwarteBoard));
      memcpy(gwarteBoard.datBuf,&inDat[pos],inLen-pos);
      gwarteBoard.datLen = inLen-pos;
      gwarteBoard.channel = channel;
      gwarteBoard.shortAddr = shortAddr;

      if(0x42 == cmdID)
      {
        //CJJ-20181017:TEI2最小时隙的节点收到命令立马发送
        u16 tei = powerDown.tei[1] << 8 | powerDown.tei[0];
        if(tei >= 2)
        {
          gwarteBoard.ctl.timer = 2500 * (tei - 2);
          gwarteBoard.ctl.warteBoardSwtich = SWTICH_ON;
        }
        respondPlcConfirm(HPLC_SEND_LPW_DAT);
      }
      else
      {
        disbaleRfSendReceive();
        SgdWakeUpLowPowerMeter( shortAddr, channel);
        //SgdRelayLowPowerMeterCommand( &inDat[pos], inLen-pos,channel);
      }
    }
  }
}

/*-----从串口转发水表物理层报文至HPLC---------*/
void uartRelayPlcWaterDat(u8 *inDat, u16 inLen ,u8 extendCI)
{
  memset( (char *)&sUsart2, 0x00, sizeof(sUsart2) );
  
  sProHead_645 *pHead = (sProHead_645 *)&sUsart2.dataBuffer[sUsart2.dataLen];
  pHead->startCode1 = FRAME_START_CODE_645;
  pHead->startCode2 = FRAME_START_CODE_645;
  memcpy( pHead->addr, sNode.longAddr, LONG_ADDR_LEN );
  pHead->sCtrCode.functionCode = ExtendFunctionCode;
  pHead->dataLen  = 2 + inLen;
  sUsart2.dataLen = sizeof(sProHead_645);
  sUsart2.dataBuffer[sUsart2.dataLen++] = extendCI;
  sUsart2.dataBuffer[sUsart2.dataLen++] = sNode.inputPower;
  if(sUsart2.dataLen + inLen < MAX_DL_645_FRAME_LEN)
  {
    memcpy((char *)&sUsart2.dataBuffer[sUsart2.dataLen],inDat,inLen);
  }
  sUsart2.dataLen += inLen;
  
  sProEnd_645 *pEnd = (sProEnd_645 *)&sUsart2.dataBuffer[sUsart2.dataLen];
  pEnd->sumCrc = SgdGetSum( (u8 *)sUsart2.dataBuffer, sUsart2.dataLen );
  pEnd->endCode = FRAME_END_CODE_645;
  sUsart2.dataLen += sizeof(sProEnd_645);

  sendMsgToHplc();

}


/*-----从串口转发无线收到的掉电广播帧---------*/
void uartRelayPlcBoardFrame(u8 *inDat, u16 inLen )
{
  memset( (char *)&sUsart2, 0x00, sizeof(sUsart2) );
  
  sProHead_645 *pHead = (sProHead_645 *)&sUsart2.dataBuffer[sUsart2.dataLen];
  pHead->startCode1 = FRAME_START_CODE_645;
  pHead->startCode2 = FRAME_START_CODE_645;
  memcpy( pHead->addr, sNode.longAddr, LONG_ADDR_LEN );
  pHead->sCtrCode.functionCode = ExtendFunctionCode;
  pHead->dataLen  = 1 + inLen;
  sUsart2.dataLen = sizeof(sProHead_645);
  sUsart2.dataBuffer[sUsart2.dataLen++] = RELAY_PLC_POWER_DOWN_FRAME; 
  if(sUsart2.dataLen + inLen < MAX_DL_645_FRAME_LEN)
  {
    memcpy((char *)&sUsart2.dataBuffer[sUsart2.dataLen],inDat,inLen);
  }
  sUsart2.dataLen += inLen;
  
  sProEnd_645 *pEnd = (sProEnd_645 *)&sUsart2.dataBuffer[sUsart2.dataLen];
  pEnd->sumCrc = SgdGetSum( (u8 *)sUsart2.dataBuffer, sUsart2.dataLen );
  pEnd->endCode = FRAME_END_CODE_645;
  sUsart2.dataLen += sizeof(sProEnd_645);

  sendMsgToHplc();
}

/*-----回复PLC主动上报应答确认帧----------*/
void respondPlcConfirm(u8 extendCmd)
{
  memset( (char *)&sUsart2, 0x00, sizeof(sUsart2) );

  sProHead_645 *pHead = (sProHead_645 *)&sUsart2.dataBuffer[sUsart2.dataLen];
  pHead->startCode1 = FRAME_START_CODE_645;
  pHead->startCode2 = FRAME_START_CODE_645;
  memcpy( pHead->addr, sNode.longAddr, LONG_ADDR_LEN );
  pHead->sCtrCode.functionCode = ExtendFunctionCode;
  pHead->dataLen  = 1;
  sUsart2.dataLen = sizeof(sProHead_645);
  sUsart2.dataBuffer[sUsart2.dataLen++] = extendCmd;

  sProEnd_645 *pEnd = (sProEnd_645 *)&sUsart2.dataBuffer[sUsart2.dataLen];
  pEnd->sumCrc = SgdGetSum( (u8 *)sUsart2.dataBuffer, sUsart2.dataLen );
  pEnd->endCode = FRAME_END_CODE_645;
  sUsart2.dataLen += sizeof(sProEnd_645);
  
  sendMsgToHplc();
}

/***************************************
*输入:无
*输出:无
*返回:串口数据类型
*描述:解析串口数据
*作者:张月保
***************************************/
u8 SgdParseUartData(void)
{
  u8 retValue = 0;
  u16 i = 0;
  u16 pos = 0;
  u8 extendCI = 0;
  u8 tmp[10] = {0};
  sControlCode_645 mCtrCode = {0};
  sProHead_645 *pHead = NULL;
  for( i = 0; i < sUsart2.dataLen; i++ )
  {
    pHead = (sProHead_645 *)&sUsart2.dataBuffer[i];
    if( pHead->startCode1 != FRAME_START_CODE_645 ) continue;
    if( pHead->startCode2 != FRAME_START_CODE_645 ) continue;
    pos = sizeof(sProHead_645);
    sProEnd_645 *pEnd = (sProEnd_645 *)&sUsart2.dataBuffer[i+pos+pHead->dataLen];
    if( pEnd->sumCrc != SgdGetSum( (u8 *)&sUsart2.dataBuffer[i], pos+pHead->dataLen ) ) continue;
    if( pEnd->endCode != FRAME_END_CODE_645 ) continue;
    mCtrCode = pHead->sCtrCode;
    
    if( pHead->sCtrCode.direction == DOWN_DIRECTION )
    {
      switch( pHead->sCtrCode.functionCode )
      {
      case ExtendFunctionCode:
        extendCI = sUsart2.dataBuffer[i+pos++];
        switch( extendCI )
        {
        case RF_TX_PN9_CODE:
          if( pHead->dataLen != 0x03 )
          {
            SgdExceptionResponsePro_645( &mCtrCode, extendCI );
          }
          else
          {
            memcpy( tmp, (char *)&sUsart2.dataBuffer[i+pos], pHead->dataLen - 1 );
            SgdNormalResponsePro_645( &mCtrCode, extendCI );
            SgdRFTxPN9Code( tmp );
          }
          break;
        case FREQUENCY_CALIBRATED:
          if( pHead->dataLen != 0x05 )
          {
            SgdExceptionResponsePro_645( &mCtrCode, extendCI );
          }
          else
          {
            memcpy( tmp, (char *)&sUsart2.dataBuffer[i+pos], pHead->dataLen - 1 );
            datFromFlashToRam();
            Flash_Writebyte(ADJUST_FREQUENCY_FLAG,0x01,FALSE);
            Flash_Writebyte(CARRIER_FREQUENCY_HIGH,tmp[2],FALSE);
            Flash_Writebyte(CARRIER_FREQUENCY_LOW,tmp[1],TRUE);

            SgdNormalResponsePro_645( &mCtrCode, extendCI );
            SgdRFCalibrateFrequency( tmp );
          }
          break;
          
        case TXPOWER_CALIBRATED:
          if( pHead->dataLen != 0x04 )
          {
            SgdExceptionResponsePro_645( &mCtrCode, extendCI );
          }
          else
          {
            memcpy( tmp, (char *)&sUsart2.dataBuffer[i+pos], pHead->dataLen - 1 );
            datFromFlashToRam();
            Flash_Writebyte(TX_POWER_FLAG,0x01,FALSE);
            Flash_Writebyte(TX_POWER_VALUE,tmp[1],TRUE);
            SgdNormalResponsePro_645( &mCtrCode, extendCI );
            SgdRFCalibrateTXpower(tmp);
          }
          break;
        //CJJ-2018-3-8：增加流水线功率自校准  
        case mCODE_TXPOWER_CALIBRATED:
          if( pHead->dataLen != 0x04 )
          {
            SgdExceptionResponsePro_645( &mCtrCode, extendCI );
          }
          else
          {
            memcpy( tmp, (char *)&sUsart2.dataBuffer[i+pos], pHead->dataLen - 1 );
            datFromFlashToRam();
            Flash_Writebyte(TX_POWER_mCODE_FLAG,0x01,FALSE);
            Flash_Writebyte(TX_POWER_mCODE_VALUE,tmp[1],TRUE);
            SgdNormalResponsePro_645( &mCtrCode, extendCI );
            SgdRFCalibrateTXpower(tmp);
          }
          break;
          
        case FREQUENCY_TXPOWER_CALIBRATED:
            if( pHead->dataLen != 0x07 )
            {
              SgdExceptionResponsePro_645( &mCtrCode, extendCI );
            }
            else
            {
              memcpy( tmp, (char *)&sUsart2.dataBuffer[i+pos], pHead->dataLen - 1 );
              datFromFlashToRam();
              Flash_Writebyte(ADJUST_FREQUENCY_FLAG,0x01,FALSE);
              Flash_Writebyte(CARRIER_FREQUENCY_HIGH,tmp[2],FALSE);
              Flash_Writebyte(CARRIER_FREQUENCY_LOW,tmp[1],FALSE);
              
              Flash_Writebyte(TX_POWER_mCODE_FLAG,0x01,FALSE);
              Flash_Writebyte(TX_POWER_mCODE_VALUE,tmp[3],FALSE);
              Flash_Writebyte(TX_POWER_FLAG,0x01,FALSE);
              Flash_Writebyte(TX_POWER_VALUE,tmp[4],TRUE);

              SgdNormalResponsePro_645( &mCtrCode, extendCI );
              SgdRFCalibrateFrequencyTXpower(tmp);
            }
            break;

				#if 0
        case SET_LONG_ADDR:
          retValue = SgdAppSetLongAddr( (u8 *)&sUsart2.dataBuffer[i+pos], pHead->dataLen - 1  );
          if( retValue > 0 )
          {
            SgdNormalResponsePro_645( &mCtrCode, extendCI );
          }
          else
          {
            SgdExceptionResponsePro_645( &mCtrCode, extendCI );
          }
          break;
				#endif
				
        case SET_RF_TX_POWER:
          retValue = SgdAppSetTxPower( (u8 *)&sUsart2.dataBuffer[i+pos], pHead->dataLen - 1  );
          if( retValue > 0 )
          {
            SgdNormalResponsePro_645( &mCtrCode, extendCI );
          }
          else
          {
            SgdExceptionResponsePro_645( &mCtrCode, extendCI );
          }
          break;
        case GET_INPUT_POWER:
          retValue = SgdAppGetInputPower( (u8 *)&sUsart2.dataBuffer[i+pos], pHead->dataLen - 1  );
          if( retValue > 0 )
          {
            SgdNormalResponsePro_645( &mCtrCode, extendCI );
          }
          else
          {
            SgdExceptionResponsePro_645( &mCtrCode, extendCI );
          }
          break;
        case  GET_VERSION_INFO:
          SgdNormalResponsePro_645( &mCtrCode, extendCI );
          break;
        case  GET_INTERNAL_VERSION:
          SgdNormalResponsePro_645( &mCtrCode, extendCI );
          break;
        case GET_DUAL_MODE_SENSTIV:
          SgdNormalResponsePro_645( &mCtrCode, extendCI );
          break;
        
        default:
          break;            
        }       
        break;
      case 0x04:
        if( pHead->dataLen != 0x03 )
        {
          SgdExceptionResponsePro_645( &mCtrCode, extendCI );
        }
        else
        {
          memset( (char *)&sRFTest, 0x00, sizeof(sRFTest) );
          sRFTest.channelIndex = sUsart2.dataBuffer[i+pos];
          sRFTest.CmdMode = sUsart2.dataBuffer[i+pos+1];
          sRFTest.Timer = sUsart2.dataBuffer[i+pos+2] * 1000;
          SgdNormalResponsePro_645( &mCtrCode, extendCI );
          SgdRFTxTest();
        }
        break;
      default:       
        break;
      }
    }
    else
    {
      switch( pHead->sCtrCode.functionCode )
      {
        case ExtendFunctionCode:
          extendCI = sUsart2.dataBuffer[i+pos++];
          switch(extendCI)
          {
            case GET_PLC_POWER_DOWN_FRAME:
              if(pHead->dataLen < PLC_MAC_FRAME_LEN)
              {
                powerDown.plcFrameLen = pHead->dataLen - 1;
                memcpy((char *)powerDown.plcFrameBuf,(char *)&sUsart2.dataBuffer[i+pos],powerDown.plcFrameLen);
              }
            break;
            case RELAY_PLC_POWER_DOWN_FRAME:
            break;
            case GET_PLC_PARAMETERM:
              if(8 == pHead->dataLen)
              {
                memcpy(powerDown.snid,(char *)&sUsart2.dataBuffer[i+pos],sizeof(powerDown.snid));
                pos += sizeof(powerDown.snid);
                memcpy(powerDown.tei,(char *)&sUsart2.dataBuffer[i+pos],sizeof(powerDown.tei));
                pos += sizeof(powerDown.tei);
                powerDown.getedTeiFlag = TRUE;
                memcpy(powerDown.maxTei,(char *)&sUsart2.dataBuffer[i+pos],sizeof(powerDown.maxTei));
                pos += sizeof(powerDown.maxTei);
                u32 snid = powerDown.snid[2] << 16 | powerDown.snid[1] << 8 | powerDown.snid[0];
                sNode.channelGroupNum = (snid % 32) + 1;
                SgdInitChannelIndexArr();
                SgdCalcRssiThresholdValue();

                powerDown.ctlTdma.maxTimer = (powerDown.maxTei[1] << 8 | powerDown.maxTei[0]) * aBaseSlotDuration;
              }
            break;
            case GET_PLC_METER_ADDR:
              if(7 == pHead->dataLen)
              {
                memcpy(sNode.longAddr,(char *)&sUsart2.dataBuffer[i+pos],sizeof(sNode.longAddr));
              }
            break;
            case RELAY_PLC_UP_WATER_FRAME:
              break;
            
            case SET_PLC_PARAMETERM:
              if(8 == pHead->dataLen)
              {
                memcpy(powerDown.snid,(char *)&sUsart2.dataBuffer[i+pos],sizeof(powerDown.snid));
                pos += sizeof(powerDown.snid);
                memcpy(powerDown.tei,(char *)&sUsart2.dataBuffer[i+pos],sizeof(powerDown.tei));
                pos += sizeof(powerDown.tei);
                powerDown.getedTeiFlag = TRUE;
                memcpy(powerDown.maxTei,(char *)&sUsart2.dataBuffer[i+pos],sizeof(powerDown.maxTei));
                pos += sizeof(powerDown.maxTei);
                if((0xff == powerDown.maxTei[0] && 0xff == powerDown.maxTei[1]) || 
                   (0x00 == powerDown.maxTei[0] && 0x00 == powerDown.maxTei[1]))
                {
                  powerDown.rfPowerDowmMode = 0x00;
                }
                else
                {
                  powerDown.rfPowerDowmMode = 0x01;
                }
                u32 snid = powerDown.snid[2] << 16 | powerDown.snid[1] << 8 | powerDown.snid[0];
                sNode.channelGroupNum = (snid % 32) + 1;
                SgdInitChannelIndexArr();
                SgdCalcRssiThresholdValue();

                powerDown.ctlTdma.maxTimer = (powerDown.maxTei[1] << 8 | powerDown.maxTei[0]) * aBaseSlotDuration;
              }
              respondPlcConfirm(extendCI);   
            break;
            
            case SET_PLC_TI:
              if(3 == pHead->dataLen)
              {
                memcpy(powerDown.ti,(char *)&sUsart2.dataBuffer[i+pos],sizeof(powerDown.ti));
                
                if(0x01 == powerDown.rfPowerDowmMode)
                {
                  u16 ti     = powerDown.ti[1] << 8 | powerDown.ti[0];
                  u16 maxTei = powerDown.maxTei[1] << 8 | powerDown.maxTei[0];
                  if(maxTei >= ti && maxTei > 0)
                  {
                    powerDown.ctlTdma.maxTimer = maxTei * aBaseSlotDuration;
                    powerDown.ctlTdma.curTimer = ti * aBaseSlotDuration;
                    powerDown.ctlTdma.startTimerSwitch = SWTICH_ON;
                  }
                }
              }
              respondPlcConfirm(extendCI);
            break;
            
            case SET_PLC_METER_ADDR:
              if(7 == pHead->dataLen)
              {
                memcpy(sNode.longAddr,(char *)&sUsart2.dataBuffer[i+pos],sizeof(sNode.longAddr));
              }
              respondPlcConfirm(extendCI);
            break;

            case UPDATE_POWER_STATUE:
              if(2 == pHead->dataLen)
              {
                u8 powerFlag = sUsart2.dataBuffer[i+pos];

                //CSMA模式上报停电状态
                if(0x01 != powerDown.rfPowerDowmMode)
                {  
                  if(0x01 == powerFlag || 0x02 == powerFlag)
                  {
                    //判断上次发送流程是否结束
                    if(FALSE == powerDown.ctlCsma.powerDownFlag)
                    {       
                      memset((char *)&powerDown.ctlCsma,0x00,sizeof(powerDown.ctlCsma));
                      powerDown.ctlCsma.powerDownFlag = TRUE;
                      powerDown.ctlCsma.readPlcFlag   = TRUE;
                      respondPlcConfirm(extendCI);              
                    }
                    else
                    {
                      respondPlcConfirm(extendCI);
                    }
                  }
                  else
                  {
                    respondPlcConfirm(extendCI);
                  }
                }
                //TDMA模式上报停电状态
                else
                {
                  if(0x01 == powerFlag || 0x02 == powerFlag)
                  {
                    if(SWTICH_ON == powerDown.ctlTdma.startTimerSwitch)
                    {
                      //判断上次发送流程是否结束
                      if(SWTICH_OFF == powerDown.ctlTdma.sendSwitch)
                      {
                        u32 uartTrasTimer = 0;//9600波特率约等于1ms/byte
                        u32 maxTimer = powerDown.ctlTdma.maxTimer;
                        u32 selfTimer = (powerDown.tei[1] << 8 | powerDown.tei[0]) * aBaseSlotDuration;
                        if(selfTimer > (uartTrasTimer + powerDown.ctlTdma.curTimer))
                        {
                          powerDown.ctlTdma.sendTimer = selfTimer - (uartTrasTimer + powerDown.ctlTdma.curTimer);
                        }
                        else if((selfTimer + (maxTimer - powerDown.ctlTdma.curTimer)) > uartTrasTimer)
                        {
                          powerDown.ctlTdma.sendTimer = selfTimer + (maxTimer - powerDown.ctlTdma.curTimer) - uartTrasTimer;
                        }
                        powerDown.ctlTdma.sendSwitch = SWTICH_ON;
                        powerDown.ctlTdma.sendFlag   = FALSE;      
                      }
                    }
                  }
                  respondPlcConfirm(extendCI);
                }
              }
            break;

            //北京版双模
            #if (2 == SUPPLY_AREA)            
            case HPLC_SEND_LPW_DAT:
              relayLPWDatToWirless((u8 *)&sUsart2.dataBuffer[i+pos],pHead->dataLen - 1);
              break;
            case HPLC_GET_NODE_RSSI:
              {
                u8 pageNum = sUsart2.dataBuffer[i+pos];
                hplcGegNodeRssi(pageNum);
              }
              break;
            #endif              

            default:
            break;
          }
          break;
          
        default:
          break;
      }
    }   
  } 
  return extendCI;
}

bool readPlcBitMap(void)
{
  bool err = FALSE;
  u8  addrPos = 1;
  u8  extendCmdPos = 10;
  u8  crcPos = 11;
  u8  extendCmd = 0;
  u8  tmp[13] = {0x68 ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA ,0x68 ,0x1F ,0x01 ,0x13 ,0xFF,0x16};
  u8 *pData    = NULL;
  u16 usDataLen = 0;
  u32 ret = 0;

  //获取掉电节点位图
  MRS_RF_645_FRAME_STRU *pst645Frame = (MRS_RF_645_FRAME_STRU *)mrsToolsMalloc(sizeof(MRS_RF_645_FRAME_STRU));
  if(NULL != pst645Frame)
  {
    mrsToolsMemZero_s(pst645Frame, sizeof(MRS_RF_645_FRAME_STRU), sizeof(MRS_RF_645_FRAME_STRU));
    memcpy((char *)&tmp[addrPos], sNode.longAddr, LONG_ADDR_LEN );
    tmp[extendCmdPos] = GET_PLC_POWER_DOWN_FRAME; 
    tmp[crcPos] = SgdGetSum( (u8 *)tmp, sizeof(tmp) - 2 );
    mrsRf645Dec(tmp, sizeof(tmp),pst645Frame);
    ret = mrsRf645_1FH_CI10(pst645Frame, (HI_PVOID *)&pData, &usDataLen, 4);
    
    if(HI_ERR_SUCCESS == ret)
    {
      if(NULL != pData)
      {
        memcpy((char *)&sUsart2.dataBuffer, pData, usDataLen);
        sUsart2.dataLen = usDataLen;      
        mrsToolsFree(pData);
        pData = NULL;
        powerDown.plcFrameLen = 0;
        memset((char *)powerDown.plcFrameBuf,0x00,sizeof(powerDown.plcFrameBuf));
        extendCmd = SgdParseUartData();
        memset((char *)&sUsart2,0x00,sizeof(sUsart2));
        if(GET_PLC_POWER_DOWN_FRAME == extendCmd)
        {
          if(powerDown.plcFrameLen > 0 && 0x01 == powerDown.plcFrameBuf[0])
          {
            err = TRUE;
          }
        }
      }
    }
  }


  //CSMA模式
  if(0x01 != powerDown.rfPowerDowmMode && FALSE == powerDown.getedTeiFlag && TRUE == err)
  {
    err = FALSE;
    tmp[extendCmdPos] = GET_PLC_PARAMETERM; 
    tmp[crcPos] = SgdGetSum( (u8 *)tmp, sizeof(tmp) - 2 );

    mrsToolsMemZero_s(pst645Frame, sizeof(MRS_RF_645_FRAME_STRU), sizeof(MRS_RF_645_FRAME_STRU));
    mrsRf645Dec(tmp, sizeof(tmp),pst645Frame);
    ret = mrsRf645_1FH_CI12(pst645Frame, (HI_PVOID *)&pData, &usDataLen, 4);

    if(HI_ERR_SUCCESS == ret)
    {
      if(NULL != pData)
      {
        memcpy((char *)&sUsart2.dataBuffer, pData, usDataLen);
        sUsart2.dataLen = usDataLen; 
        mrsToolsFree(pData);
        extendCmd = SgdParseUartData();
        memset((char *)&sUsart2,0x00,sizeof(sUsart2));
        if(GET_PLC_PARAMETERM == extendCmd)
        {
          powerDown.getedTeiFlag = TRUE;
          err = TRUE;
        }
      }
    }
  }

  if(NULL != pst645Frame)
  {
    mrsToolsFree(pst645Frame);
  }
  
  return err;
}

void  SgdTimerProcess(void)
{
  //CSMA模式上报停电信息
  if(0x01 != powerDown.rfPowerDowmMode)
  {
    if(powerDown.ctlCsma.powerDownFlag && powerDown.ctlCsma.sendCnt < POWER_DOWN_MAX_SEND)
    {
      if(powerDown.ctlCsma.readPlcFlag)
      {
        //如果读到位图信息，则设置重发时间，否则终止本次上报
        if(TRUE == readPlcBitMap() && TRUE == powerDown.ctlCsma.powerDownFlag)
        {
          powerDown.ctlCsma.readPlcFlag   = FALSE;
          powerDown.ctlCsma.wlSendFlag = TRUE;
          
          if(powerDown.ctlCsma.sendCnt > 0)
          {
            if(1 == powerDown.ctlCsma.sendCnt)
            {
              powerDown.ctlCsma.csmaOverTimer = 1000;
            }
            else
            {
              powerDown.ctlCsma.csmaOverTimer = POWER_DOWN_RETRY_OVER_TIMER - POWER_DOWN_RETRY_VAL;
            }
            powerDown.ctlCsma.csmaTimerSwitch = SWTICH_ON;
            powerDown.ctlCsma.csmaTimerFlag = FALSE;
          }
        }
        else
        {
          memset((char *)&powerDown.ctlCsma,0x00,sizeof(powerDown.ctlCsma));
        }
      }
      
      if(powerDown.ctlCsma.wlSendFlag)
      {
        bool interrupt_06 = FALSE;
        recvChannelArrPos = 0x02;
        RF_RxOn( interrupt_06, channelIndexArr[recvChannelArrPos] );
        u8 inputPower = SgdGetInputPower();
        
        if( inputPower > channelRssiThresholdArr[recvChannelArrPos] )
        {
          memset( &sWLTx, 0x00, sizeof(sWLTx) );
          //构造广播帧
          relayPlcPowerDownFrame(&powerDown.plcFrameBuf[1],powerDown.plcFrameLen-1);
          //构造物理层帧
          SgdPhyFormFrame( channelIndexArr[recvChannelArrPos] );
          //白化无线数据
          SgdPhyWhitingBuf( sWLTx.sBuff.data, sWLTx.sBuff.len );
          //开启无线发射
          u8 channel = channelIndexArr[recvChannelArrPos];
          channel = CaculateChannelNo(channel&0x01,channel>>1);
          rfWLTx(channel);

          //竞争成功，则重新设置发送间隔时间
          if(0 == powerDown.ctlCsma.sendCnt)
          {
            u16 tei = powerDown.tei[1] << 8 | powerDown.tei[0];
            u8 nDelay = tei % 9;
            powerDown.ctlCsma.readPlcIntVal = 1000 * (nDelay+1);
          }
          else
          {
            powerDown.ctlCsma.readPlcIntVal = POWER_DOWN_RETRY_VAL;
          }
          powerDown.ctlCsma.readPlcSwtich = SWTICH_ON;
          powerDown.ctlCsma.readPlcFlag   = FALSE;
          powerDown.ctlCsma.wlSendFlag = FALSE;
          powerDown.ctlCsma.csmaTimerSwitch = SWTICH_OFF;
          powerDown.ctlCsma.csmaTimerFlag = FALSE;
          powerDown.ctlCsma.csmaOverTimer = 0;
          powerDown.ctlCsma.sendCnt++;
        }
        //重发竞争失败，则重新获取位图并重新设置竞争超时时间
        else if(powerDown.ctlCsma.csmaTimerFlag)
        {
          if(TRUE == readPlcBitMap())
          {
             powerDown.ctlCsma.csmaOverTimer = POWER_DOWN_RETRY_OVER_TIMER;
             powerDown.ctlCsma.csmaTimerSwitch = SWTICH_ON;
             powerDown.ctlCsma.csmaTimerFlag = FALSE;
          }
          else
          {
            memset((char *)&powerDown.ctlCsma,0x00,sizeof(powerDown.ctlCsma));
          }
        }
      }
    }
    else
    {
      memset((char *)&powerDown.ctlCsma,0x00,sizeof(powerDown.ctlCsma));
    }
  }
  //TDMA上报停电信息
  else
  {
    if(powerDown.ctlTdma.sendFlag)
    {
      powerDown.ctlTdma.sendFlag = FALSE;
      if(TRUE == readPlcBitMap())
      {
        recvChannelArrPos = 0x02;
        memset( &sWLTx, 0x00, sizeof(sWLTx) );
        //构造广播帧
        relayPlcPowerDownFrame(&powerDown.plcFrameBuf[1],powerDown.plcFrameLen-1);
        //构造物理层帧
        SgdPhyFormFrame( channelIndexArr[recvChannelArrPos] );
        //白化无线数据
        SgdPhyWhitingBuf( sWLTx.sBuff.data, sWLTx.sBuff.len );
        //开启无线发射   
        u8 channel = channelIndexArr[recvChannelArrPos];
        channel = CaculateChannelNo(channel&0x01,channel>>1);
        rfWLTx(channel);
      }
    }
  }

//北京版表模块
#if (2 == SUPPLY_AREA)
  if(TRUE == gwarteBoard.ctl.warteBoardflag)
    {
      u16 shortAddr = 0x9999;
      gwarteBoard.ctl.warteBoardflag = FALSE;
      disbaleRfSendReceive();
      SgdWakeUpLowPowerMeter( shortAddr, gwarteBoard.channel);
      //SgdRelayLowPowerMeterCommand(gwarteBoard.datBuf, gwarteBoard.datLen,gwarteBoard.channel);
    }
#endif      
}
/***************************************
*输入:无
*输出:无
*返回:无
*描述:保存节点配置信息
*作者:张月保
***************************************/
void  SgdSaveConfigInfo( u8 *buf, u8 len )
{}

/***************************************
*输入:无
*输出:无
*返回:无
*描述:删除节点配置信息
*作者:张月保
***************************************/
void  SgdDeleteConfigInfo(void)
{}

/***************************************
*输入:无
*输出:无
*返回:无
*描述:灯状态切换控制任务
*作者:zt
***************************************/
void  SgdLedProcess(void)
{}

/***************************************
*输入:无
*输出:无
*返回:无
*描述:设置、监听无线信道
*作者:张月保
***************************************/
void  SgdScanChannelProcess(void)
{}

/***************************************
*输入:无
*输出:无
*返回:无
*描述:无线数据接收功能进程函数
*作者:张月保
***************************************/
void  SgdWLRecvProcess(void)
{
  if( sWLRx.sBuff.len > 0 )
  {
    //接收到无线数据，射频芯片重启计时器复位
    sRFboot.timer = 0;

    //反白化数据
    SgdPhyWhitingBuf( sWLRx.sBuff.data, sWLRx.sBuff.len );

    //解析物理层是否正确
    if( SgdPhyParseFrame( &sWLRx.sBuff ) )
    {
       
      //解析MAC
      u8 macFramePos = sizeof(sPhyFrameHead);
      SgdParseMacFrame( &sWLRx.sBuff.data[macFramePos], sWLRx.sBuff.len );
    }
    
    SgdWLSendProcess();  
    memset( &sWLRx, 0x00, sizeof(sWLRx) );   
  
  }
}
/***************************************
*输入:无
*输出:无
*返回:无
*描述:接收无线数据
*作者:张月保
***************************************/
void SgdWLReceiving(void)
{}

/***************************************
*输入:无
*输出:无
*返回:无
*描述:无线数据发送功能函数
*作者:张月保
***************************************/
void  SgdWLSendProcess(void)
{
  if(TRUE == sWLTx.responseIdentifier )
  {
    disbaleRfSendReceive();
    sWLTx.responseIdentifier = FALSE;
    HI_MDM_Sleep(30);
    
    RF_RxOn(FALSE,channelIndexArr[recvChannelArrPos]);
    if( SgdMacDoCSMA_CA( channelRssiThresholdArr[recvChannelArrPos] ) )     
    {              
      //构造物理层帧
      SgdPhyFormFrame( channelIndexArr[recvChannelArrPos] );

      //白化即将发送的无线数据
      SgdPhyWhitingBuf( sWLTx.sBuff.data, sWLTx.sBuff.len );       
    }
    //开启无线发射
    u8 channel = channelIndexArr[recvChannelArrPos];
    channel = CaculateChannelNo(channel&0x01,channel>>1);
    rfWLTx(channel);   
  }
  else
  {
    enableRfSendReceive();
  }
}

/***************************************
*输入:无
*输出:无
*返回:无
*描述:更新RSSI门限值
*作者:张月保
***************************************/
void  SgdUpdateRssiThresholdProcess(void)
{
  if( sUpRssiThreshold.updateFlag )
  {   
    //更新各信道RSSI门限值
    sUpRssiThreshold.updateFlag = FALSE;
    SgdCalcRssiThresholdValue();    
  }
}


/***************************************
*输入:无
*输出:无
*返回:无
*描述:返回正常应答帧，645格式
*作者:张月保
***************************************/
void  SgdNormalResponsePro_645( sControlCode_645 *pCtrCode, u8 extendCI )
{
  memset( (char *)&sUsart2, 0x00, sizeof(sUsart2) );
 // sUsart2.dataBuffer[sUsart2.dataLen++] = 0xfE;
 // sUsart2.dataBuffer[sUsart2.dataLen++] = 0xfE;
 // sUsart2.dataBuffer[sUsart2.dataLen++] = 0xfE;
  
  sProHead_645 *pHead = (sProHead_645 *)&sUsart2.dataBuffer[sUsart2.dataLen];
  pHead->startCode1 = FRAME_START_CODE_645;
  pHead->startCode2 = FRAME_START_CODE_645;
  memcpy( pHead->addr, sNode.longAddr, LONG_ADDR_LEN );
  pHead->sCtrCode = *pCtrCode;
  pHead->sCtrCode.direction = UP_DIRECTION;
  sUsart2.dataLen = sizeof(sProHead_645);
  if( pHead->sCtrCode.functionCode == ExtendFunctionCode )
  {
    if( extendCI == GET_INPUT_POWER )
    {
      pHead->dataLen = 0x03;
      sUsart2.dataBuffer[sUsart2.dataLen++] = extendCI;
      u8 temp = sRFTest.channelIndex;
      sUsart2.dataBuffer[sUsart2.dataLen++] = temp;
      sUsart2.dataBuffer[sUsart2.dataLen++] = sNode.inputPower;
    }
    else if( extendCI == GET_VERSION_INFO )
    {
      pHead->dataLen = 0x10;
      sUsart2.dataBuffer[sUsart2.dataLen++] = extendCI;
      sUsart2.dataBuffer[sUsart2.dataLen++] = HARDWARE_SUB_VER;
      sUsart2.dataBuffer[sUsart2.dataLen++] = HARDWARE_MASTER_VER;
      sUsart2.dataBuffer[sUsart2.dataLen++] = SOFTWARE_SUB_VER;
      sUsart2.dataBuffer[sUsart2.dataLen++] = SOFTWARE_MASTER_VER;
      sUsart2.dataBuffer[sUsart2.dataLen++] = BOOTLOADER_VER;
      sUsart2.dataBuffer[sUsart2.dataLen++] = INTERNAL_SUB_VERSION;
      sUsart2.dataBuffer[sUsart2.dataLen++] = INTERNAL_MASTER_VERSION;
      
      u8 frequencyFlag = Flash_Readbyte(ADJUST_FREQUENCY_FLAG);
      u8 frequencyHigh = Flash_Readbyte(CARRIER_FREQUENCY_HIGH);
      u8 frequencyLow  = Flash_Readbyte(CARRIER_FREQUENCY_LOW);   

      sUsart2.dataBuffer[sUsart2.dataLen++] = frequencyFlag;
      sUsart2.dataBuffer[sUsart2.dataLen++] = frequencyHigh;
      sUsart2.dataBuffer[sUsart2.dataLen++] = frequencyLow;

      sUsart2.dataBuffer[sUsart2.dataLen++] = Flash_Readbyte(TX_POWER_mCODE_FLAG);
      sUsart2.dataBuffer[sUsart2.dataLen++] = Flash_Readbyte(TX_POWER_mCODE_VALUE);

      sUsart2.dataBuffer[sUsart2.dataLen++] = Flash_Readbyte(TX_POWER_FLAG);
      sUsart2.dataBuffer[sUsart2.dataLen++] = Flash_Readbyte(TX_POWER_VALUE);
      sUsart2.dataBuffer[sUsart2.dataLen++] = 0x00;
    }
    else if( extendCI == GET_INTERNAL_VERSION )
    {
      pHead->dataLen = 0x03;
      sUsart2.dataBuffer[sUsart2.dataLen++] = extendCI;
      sUsart2.dataBuffer[sUsart2.dataLen++] = INTERNAL_SUB_VERSION;
      sUsart2.dataBuffer[sUsart2.dataLen++] = INTERNAL_MASTER_VERSION;
    }

    else if( extendCI == GET_DUAL_MODE_SENSTIV )
    {
      pHead->dataLen = 0x02;
      sUsart2.dataBuffer[sUsart2.dataLen++] = extendCI;
      sUsart2.dataBuffer[sUsart2.dataLen++] = SensitivityTestCNT;
      SensitivityTestCNT = 0;
    }
    
    else
    {
      pHead->dataLen = 0x01;
      sUsart2.dataBuffer[sUsart2.dataLen++] = extendCI;    
    }
  }
  else  if( pHead->sCtrCode.functionCode == 0x04 )
  {
    pHead->dataLen = 0x03;
    u8 temp = sRFTest.channelIndex;
    sUsart2.dataBuffer[sUsart2.dataLen++] = temp;
    temp = sRFTest.CmdMode;
    sUsart2.dataBuffer[sUsart2.dataLen++] = temp;
    u32 temp1 = sRFTest.Timer;
    sUsart2.dataBuffer[sUsart2.dataLen++] = temp1 / 1000;
  } 
  else
  {
    pHead->dataLen = 0x00;
  }
  
  sProEnd_645 *pEnd = (sProEnd_645 *)&sUsart2.dataBuffer[sUsart2.dataLen];
  pEnd->sumCrc = SgdGetSum( (u8 *)sUsart2.dataBuffer, sUsart2.dataLen );
  pEnd->endCode = FRAME_END_CODE_645;
  sUsart2.dataLen += sizeof(sProEnd_645);
  
  sendMsgToHplc();
}

/***************************************
*输入:无
*输出:无
*返回:无
*描述:返回异常应答帧，645格式
*作者:张月保
***************************************/
void  SgdExceptionResponsePro_645( sControlCode_645 *pCtrCode, u8 extendCI )
{
  memset( (char *)&sUsart2, 0x00, sizeof(sUsart2) );
  
  sProHead_645 *pHead = (sProHead_645 *)&sUsart2.dataBuffer[sUsart2.dataLen];
  pHead->startCode1 = FRAME_START_CODE_645;
  pHead->startCode2 = FRAME_START_CODE_645;
  memcpy( pHead->addr, sNode.longAddr, LONG_ADDR_LEN );
  pCtrCode->direction = UP_DIRECTION;
  pCtrCode->slaveRespondFlag = 0x01;
  pHead->sCtrCode = *pCtrCode;
  sUsart2.dataLen = sizeof(sProHead_645);
  if( pHead->sCtrCode.functionCode == ExtendFunctionCode )
  {
    pHead->dataLen = 0x01;
    sUsart2.dataBuffer[sUsart2.dataLen++] = extendCI;    
  } 
  else
  {
    pHead->dataLen = 0x00;
  }
  
  sProEnd_645 *pEnd = (sProEnd_645 *)&sUsart2.dataBuffer[sUsart2.dataLen];
  pEnd->sumCrc = SgdGetSum( (u8 *)sUsart2.dataBuffer, sUsart2.dataLen );
  pEnd->endCode = FRAME_END_CODE_645;
  sUsart2.dataLen += sizeof(sProEnd_645);

  sendMsgToHplc();

}

/***************************************
*输入:无
*输出:无
*返回:无
*描述:构造并发送获取电表事件上报模式字/状态字命令
*作者:张月保
***************************************/
bool  SgdGetEventReportStatusWord(void)
{}

/***************************************
*输入:无
*输出:无
*返回:T无
*描述:全网感知帧发送
*作者:CJJ
***************************************/
void flNetSend(void)
{

}

/***************************************
*输入:srcAddr:被检索的地址列表指针
      dstAddr：被检索的目标地址
      mode：2短地址，6长地址
*输出:无
*返回:TRUE 检索到
*描述:检索升级列表地址
*作者:CJJ
***************************************/
bool checkAddrList(u8 *srcAddr,u8 *dstAddr,u8 mode)
{}


/***************************************
*输入：数据指针
*返回:TRUE解析成功
*描述:解析升级启动帧
*作者:CJJ
***************************************/
void paresUpgradStart(upGradStart_st *p)
{}

/***************************************
*输入：数据指针
*返回:TRUE解析成功
*描述:解析升级数据包
*作者:CJJ
***************************************/
void paresUpgradDat(upGradDat_st *p)
{}

/***************************************
*输入：无
*返回:TURE表示写入备份式升级正确
*描述:判断备份式升级是否完成
*作者:CJJ
***************************************/
bool upGradeDone(void)
{}

/*************************************************
*函数名称：SgdEraseAppCodeFlash
*功能描述：擦擦除bootloader程序区代码
*输入参数：无 
*输出参数：无 
*返 回 值：无
*************************************************/
void  SgdEraseBootloaderFlash(void)
{}

/***************************************
*输入:无
*输出:无
*返回:无
*描述:升级bootloader程序
*作者:CJJ
***************************************/
bool upGradeBoot(void)
{}

//check_sum程序代码，固化在最后一page，请勿更改。
const u8 CRC_FLAHS_DAT[] = 
{};

/***************************************
*输入:无
*输出:无
*返回:无
*描述:更新check_sum程序
*作者:CJJ
***************************************/
void updateCheckProgram(void)
{}

/***************************************
*输入:无
*输出:无
*返回:无
*描述:执行升级
*作者:CJJ
***************************************/
void exeUpgrad(void)
{}

/***************************************
*输入:无
*输出:无
*返回:无
*描述:节点程序备份式升级
*作者:CJJ
***************************************/
void snodeUpgrade(void)
{}

/***************************************
*输入:无
*输出:无
*返回:无
*描述:检查邻居节点表
*作者:CJJ
***************************************/
void checkNeighbour(void)
{
  static u32 t0 = 0;
  
  //每分钟检查一次邻居列表，如果老化时间到了(48小时)，则剔除相应节点
  if(wv_ticks > t0)
  {
    if(wv_ticks - t0 > (60 * 1000))
    {
      t0 = wv_ticks;
      
      //CJJ-2018-1-11：水表邻居老化检测
      u8 WaterMeterNeiDataNums = FIFO_COUNT(&waterMeteNeiFifoCtr,MAX_LP_NEIGHBOURS+1);
      sLowPowerNeighbour *pTmpNei = NULL;
      sLowPowerNeighbour *pPopNei = NULL;
      sLowPowerNeighbour *pPushNei = NULL;
      
      for(u8 i = 0;i < WaterMeterNeiDataNums;i++)
      {
        pTmpNei = &gLowPowerNeighbour[FIFO_POP_IDX(&waterMeteNeiFifoCtr,MAX_LP_NEIGHBOURS+1,i)];
        if(wv_ticks > (pTmpNei->timer))
        {
          if((wv_ticks - (pTmpNei->timer)) > (48*60 *60 *1000))
          {
            for(u8 j = 0; j < i;j++)
            {
              pPopNei  = &gLowPowerNeighbour[FIFO_POP(&waterMeteNeiFifoCtr,MAX_LP_NEIGHBOURS+1)];
              pPushNei = &gLowPowerNeighbour[FIFO_PUSH(&waterMeteNeiFifoCtr,MAX_LP_NEIGHBOURS+1)];
              memcpy((char *)pPushNei,(char *)pPopNei,sizeof(sLowPowerNeighbour));
            }
            u16 inVaild = FIFO_POP(&waterMeteNeiFifoCtr,MAX_LP_NEIGHBOURS+1);
            if(LPNeighbourNums > 0)LPNeighbourNums--;
            break;
          }
        }
        else
        {
          pTmpNei->timer = wv_ticks;
        }
      }
      
      //CJJ-2018-1-11：上报式水表老化检测
      u8 WaterMeterDataNums = FIFO_COUNT(&waterMeteFifoCtr,TOTAL_WART_METER_SIZE+1);
      sWaterMeterData *pTmp = NULL;
      sWaterMeterData *pPop = NULL;
      sWaterMeterData *pPush = NULL;
      
      for(u8 i = 0;i < WaterMeterDataNums;i++)
      {
        pTmp = &waterMeterBuf[FIFO_POP_IDX(&waterMeteFifoCtr,TOTAL_WART_METER_SIZE+1,i)];
        if(wv_ticks > (pTmp->timer))
        {
          if((wv_ticks - (pTmp->timer)) > (72*60 *60 *1000))
          {
            for(u8 j = 0; j < i;j++)
            {
              pPop  = &waterMeterBuf[FIFO_POP(&waterMeteFifoCtr,TOTAL_WART_METER_SIZE+1)];
              pPush = &waterMeterBuf[FIFO_PUSH(&waterMeteFifoCtr,TOTAL_WART_METER_SIZE+1)];
              memcpy((char *)pPush,(char *)pPop,sizeof(sWaterMeterData));
            }
            u16 inVaild = FIFO_POP(&waterMeteFifoCtr,TOTAL_WART_METER_SIZE+1);
            break;
          }
        }
        else
        {
          pTmp->timer = wv_ticks;
        }
      }
    }
  }
  //计数器溢出
  else
  {
    t0 = wv_ticks;
  }
}

#if 0
//Suxy_20180122---bool SgdIsProtocol_645(u8 *pInDat,u16 inLen)
bool SgdIsProtocol_645(u8 *pInDat,u16 inLen,u16 *starCodeSqe)
{
  u16 i,pos;
  if( pInDat == NULL || 0 == inLen || inLen > MAX_DL_645_FRAME_LEN) return FALSE;
  for( i = 0; i < inLen; i++ )
  {
    pos = 0;
    sProHead_645 *pHead = (sProHead_645 *)&pInDat[i];
    if( (0x68 != pHead->startCode2) || ( pHead->startCode1 != 0x68 )) continue;
    pos = sizeof(sProHead_645);  
    pos += pHead->dataLen;
    
    if(i+pos > MAX_DL_645_FRAME_LEN)continue;
    sProEnd_645 *pEnd = (sProEnd_645 *)&pInDat[i+pos];
    if( pEnd->sumCrc != SgdGetSum( &pInDat[i], pos ) )continue;
    if( pEnd->endCode != 0x16 ) continue;
    (*starCodeSqe) = i;//Suxy_20180122+++

    return TRUE;
  }
	
  return FALSE;
}

//Suxy_20180122---bool SgdIsProtocol_698( u8 *pInDat,u16 inLen,u8 *pServerAddr, u8 *addrLength)
bool SgdIsProtocol_698( u8 *pInDat,u16 inLen,u8 *pServerAddr, u8 *addrLength,u16 *starCodeSqe)
{
  u16 i = 0, pos = 0, frameLen = 0;
  
  if( pInDat == NULL || 0 == inLen) return FALSE;
  
  for( i = 0; i < inLen; i++ )
  {
    pos = 0;
    
    sProHead_698 *pHead = (sProHead_698 *)&pInDat[i];
    if(pHead->startCode != 0x68) continue;
    frameLen = ((pHead->dataLen[1]<<8)&0xFF00) + (pHead->dataLen[0]&0xFF);
    if(frameLen > (inLen - i)) continue;
    if(pHead->sCtrCode.startMark != 1)
    {
      #ifdef __PRINTF_DEBUG
      Print("SgdIsProtocol_698 startMark = %d is not 0x01, transferMark = %d!\n", pHead->sCtrCode.startMark, pHead->sCtrCode.transferMark);
      #endif
    }
    if(pHead->sCtrCode.functionCode != 3)
    {
      #ifdef __PRINTF_DEBUG
      Print("SgdIsProtocol_698 functionCode = %d is not 0x03!\n", pHead->sCtrCode.functionCode);
      #endif
    }
    if(pHead->sAddrFlag.serverType != SINGLE_ADDRESS)

    {
      #ifdef __PRINTF_DEBUG
      Print("SgdIsProtocol_698 serverType = %d is not SINGLE_ADDRESS!\n", pHead->sAddrFlag.serverType);
      #endif
    }
    
    pos = sizeof(sProHead_698);
    if(pHead->sAddrFlag.serverType == BROADCAST_ADDRESS)
    {
      *addrLength = 1;
      pServerAddr[0] = 0xAA;
    }
    else
    {
      *addrLength = pHead->sAddrFlag.serverByte + 1;
      memcpy(pServerAddr, &pInDat[i+pos], *addrLength);
    }
    pos += (pHead->sAddrFlag.serverByte + 1);

    sProHeadEnd_698 *pHeadEnd = (sProHeadEnd_698 *)&pInDat[i+pos];
    if(pHeadEnd->sClientAddr != 0)
    {
      #ifdef __PRINTF_DEBUG
      Print("SgdIsProtocol_698 sClientAddr = %d is not 0x00!\n", pHeadEnd->sClientAddr);
      #endif
    }
    u16 headCrc1 = (pHeadEnd->sHeadHcs[1]<<8) + pHeadEnd->sHeadHcs[0];
    pos += sizeof(sProHeadEnd_698);
    u16 headCrc2 = SgdPhyCalCrc(&pInDat[i+1], (pos-1-2));
    if(headCrc1 != headCrc2)
    {
      #ifdef __PRINTF_DEBUG
      Print("SgdIsProtocol_698 sHeadHcs is error! headCrc1 = 0x%x, headCrc2 = 0x%x!\n", headCrc1, headCrc2);
      #endif
      
      continue;
    }

    pos = frameLen + 1 - 2;
    sProEnd_698 *pEnd = (sProEnd_698 *)&pInDat[i+pos];
    u16 endCrc1 = (pEnd->frameFcs[1]<<8) + pEnd->frameFcs[0];
    u16 endCrc2 = SgdPhyCalCrc(&pInDat[i+1], (frameLen-2));
    if(endCrc1 != endCrc2)
    {
      #ifdef __PRINTF_DEBUG
      Print("SgdIsProtocol_698 frameFcs is error! endCrc1 = 0x%x, endCrc2 = 0x%x!\n", endCrc1, endCrc2);
      #endif
      
      continue;
    }
    if( pEnd->endCode != 0x16 ) continue;
    (*starCodeSqe) = i;//Suxy_20180122+++
    
    return TRUE;
  }
	
  return FALSE;
}
#endif

#endif

