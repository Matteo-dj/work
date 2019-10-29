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
#define     GET_METER_ADDR_ROUNDS           16   //��ȡ����ַ����Ӧ����
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
*����:��
*���:��
*����:��
*����:��ȡ����ַ
*����:���±�
***************************************/
bool  SgdGetMeterAddr(void)
{}

/***************************************
*����:��
*���:��
*����:��
*����:��ʼ�������
*����:���±�
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
*����:��
*���:��
*����:��
*����:���ü����ŵ�
*����:���±�
***************************************/
void  SgdSetListeningChannel(void)
{}

/***************************************
*����:��
*���:��
*����:��
*����:����Ƿ��е���¼�����
*����:���±�
***************************************/
void SgdEventReportedProcess(void)
{}

/***************************************
*����:��
*���:��
*����:��
*����:����ڵ������������̴���
*����:���±�
***************************************/
void  SgdActiveJoinNetProcess(void)
{}

/***************************************
*����:��
*���:��
*����:��
*����:������Ƶ��������
*����:���±�
***************************************/
void  SgdWLTx(u8 channel,bool reCalchannel)
{}

/***************************************
*����:��
*���:��
*����:TRUE=�ְ�������ɣ�FALSE=δ���
*����:698����ְ�����
*����:CJJ
***************************************/
bool appMulFrameRelay(bool formFrame)
{}

/*-----���߷�����Ϣ���ݸ�HPLC--------*/
void sendMsgToHplc(void)
{
  HI_DMS_CHL_RX_S pstRxData = {0};

  pstRxData.bCanDiscardOldData = 1;
  pstRxData.usPayloadLen = sUsart2.dataLen;
  pstRxData.pPayload = sUsart2.dataBuffer;
  mrsStaRFRecvDealFun(&pstRxData);
}

/***************************************
*����:��
*���:��
*����:��
*����:����������
*����:���±�
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
    //���ڽ������
    sUsart2.status.RxDone = STATUS_FALSE;    
    SgdParseUartData();
    memset( (char *)&sUsart2, 0x00, sizeof(sUsart2) );  
  } 
}

/*------��ӦHPLC��ѯˮ���ھӳ�ǿ����------*/
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
    //���ҳ��Ŵ��ڵ�����ҳ����˵����������ȷ������Ӧ
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
  //��ȡ�ھӽڵ��RSSIֵ
  pos = sUsart2.dataLen;
  SgdNwkGetLowPowerMeterNeighbours( (u8 *)&sUsart2.dataBuffer[pos], &pos, nodeNum, pageIndex );
  sUsart2.dataLen = pos;
  sProEnd_645 *pEnd = (sProEnd_645 *)&sUsart2.dataBuffer[sUsart2.dataLen];
  pEnd->sumCrc = SgdGetSum( (u8 *)sUsart2.dataBuffer, sUsart2.dataLen );
  pEnd->endCode = FRAME_END_CODE_645;
  sUsart2.dataLen += sizeof(sProEnd_645);

  sendMsgToHplc();

}

/*-------ת��HPLC���Э������������---------*/
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
        //CJJ-20181017:TEI2��Сʱ϶�Ľڵ��յ�����������
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

/*-----�Ӵ���ת��ˮ������㱨����HPLC---------*/
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


/*-----�Ӵ���ת�������յ��ĵ���㲥֡---------*/
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

/*-----�ظ�PLC�����ϱ�Ӧ��ȷ��֡----------*/
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
*����:��
*���:��
*����:������������
*����:������������
*����:���±�
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
        //CJJ-2018-3-8��������ˮ�߹�����У׼  
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

                //CSMAģʽ�ϱ�ͣ��״̬
                if(0x01 != powerDown.rfPowerDowmMode)
                {  
                  if(0x01 == powerFlag || 0x02 == powerFlag)
                  {
                    //�ж��ϴη��������Ƿ����
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
                //TDMAģʽ�ϱ�ͣ��״̬
                else
                {
                  if(0x01 == powerFlag || 0x02 == powerFlag)
                  {
                    if(SWTICH_ON == powerDown.ctlTdma.startTimerSwitch)
                    {
                      //�ж��ϴη��������Ƿ����
                      if(SWTICH_OFF == powerDown.ctlTdma.sendSwitch)
                      {
                        u32 uartTrasTimer = 0;//9600������Լ����1ms/byte
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

            //������˫ģ
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

  //��ȡ����ڵ�λͼ
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


  //CSMAģʽ
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
  //CSMAģʽ�ϱ�ͣ����Ϣ
  if(0x01 != powerDown.rfPowerDowmMode)
  {
    if(powerDown.ctlCsma.powerDownFlag && powerDown.ctlCsma.sendCnt < POWER_DOWN_MAX_SEND)
    {
      if(powerDown.ctlCsma.readPlcFlag)
      {
        //�������λͼ��Ϣ���������ط�ʱ�䣬������ֹ�����ϱ�
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
          //����㲥֡
          relayPlcPowerDownFrame(&powerDown.plcFrameBuf[1],powerDown.plcFrameLen-1);
          //���������֡
          SgdPhyFormFrame( channelIndexArr[recvChannelArrPos] );
          //�׻���������
          SgdPhyWhitingBuf( sWLTx.sBuff.data, sWLTx.sBuff.len );
          //�������߷���
          u8 channel = channelIndexArr[recvChannelArrPos];
          channel = CaculateChannelNo(channel&0x01,channel>>1);
          rfWLTx(channel);

          //�����ɹ������������÷��ͼ��ʱ��
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
        //�ط�����ʧ�ܣ������»�ȡλͼ���������þ�����ʱʱ��
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
  //TDMA�ϱ�ͣ����Ϣ
  else
  {
    if(powerDown.ctlTdma.sendFlag)
    {
      powerDown.ctlTdma.sendFlag = FALSE;
      if(TRUE == readPlcBitMap())
      {
        recvChannelArrPos = 0x02;
        memset( &sWLTx, 0x00, sizeof(sWLTx) );
        //����㲥֡
        relayPlcPowerDownFrame(&powerDown.plcFrameBuf[1],powerDown.plcFrameLen-1);
        //���������֡
        SgdPhyFormFrame( channelIndexArr[recvChannelArrPos] );
        //�׻���������
        SgdPhyWhitingBuf( sWLTx.sBuff.data, sWLTx.sBuff.len );
        //�������߷���   
        u8 channel = channelIndexArr[recvChannelArrPos];
        channel = CaculateChannelNo(channel&0x01,channel>>1);
        rfWLTx(channel);
      }
    }
  }

//�������ģ��
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
*����:��
*���:��
*����:��
*����:����ڵ�������Ϣ
*����:���±�
***************************************/
void  SgdSaveConfigInfo( u8 *buf, u8 len )
{}

/***************************************
*����:��
*���:��
*����:��
*����:ɾ���ڵ�������Ϣ
*����:���±�
***************************************/
void  SgdDeleteConfigInfo(void)
{}

/***************************************
*����:��
*���:��
*����:��
*����:��״̬�л���������
*����:zt
***************************************/
void  SgdLedProcess(void)
{}

/***************************************
*����:��
*���:��
*����:��
*����:���á����������ŵ�
*����:���±�
***************************************/
void  SgdScanChannelProcess(void)
{}

/***************************************
*����:��
*���:��
*����:��
*����:�������ݽ��չ��ܽ��̺���
*����:���±�
***************************************/
void  SgdWLRecvProcess(void)
{
  if( sWLRx.sBuff.len > 0 )
  {
    //���յ��������ݣ���ƵоƬ������ʱ����λ
    sRFboot.timer = 0;

    //���׻�����
    SgdPhyWhitingBuf( sWLRx.sBuff.data, sWLRx.sBuff.len );

    //����������Ƿ���ȷ
    if( SgdPhyParseFrame( &sWLRx.sBuff ) )
    {
       
      //����MAC
      u8 macFramePos = sizeof(sPhyFrameHead);
      SgdParseMacFrame( &sWLRx.sBuff.data[macFramePos], sWLRx.sBuff.len );
    }
    
    SgdWLSendProcess();  
    memset( &sWLRx, 0x00, sizeof(sWLRx) );   
  
  }
}
/***************************************
*����:��
*���:��
*����:��
*����:������������
*����:���±�
***************************************/
void SgdWLReceiving(void)
{}

/***************************************
*����:��
*���:��
*����:��
*����:�������ݷ��͹��ܺ���
*����:���±�
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
      //���������֡
      SgdPhyFormFrame( channelIndexArr[recvChannelArrPos] );

      //�׻��������͵���������
      SgdPhyWhitingBuf( sWLTx.sBuff.data, sWLTx.sBuff.len );       
    }
    //�������߷���
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
*����:��
*���:��
*����:��
*����:����RSSI����ֵ
*����:���±�
***************************************/
void  SgdUpdateRssiThresholdProcess(void)
{
  if( sUpRssiThreshold.updateFlag )
  {   
    //���¸��ŵ�RSSI����ֵ
    sUpRssiThreshold.updateFlag = FALSE;
    SgdCalcRssiThresholdValue();    
  }
}


/***************************************
*����:��
*���:��
*����:��
*����:��������Ӧ��֡��645��ʽ
*����:���±�
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
*����:��
*���:��
*����:��
*����:�����쳣Ӧ��֡��645��ʽ
*����:���±�
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
*����:��
*���:��
*����:��
*����:���첢���ͻ�ȡ����¼��ϱ�ģʽ��/״̬������
*����:���±�
***************************************/
bool  SgdGetEventReportStatusWord(void)
{}

/***************************************
*����:��
*���:��
*����:T��
*����:ȫ����֪֡����
*����:CJJ
***************************************/
void flNetSend(void)
{

}

/***************************************
*����:srcAddr:�������ĵ�ַ�б�ָ��
      dstAddr����������Ŀ���ַ
      mode��2�̵�ַ��6����ַ
*���:��
*����:TRUE ������
*����:���������б��ַ
*����:CJJ
***************************************/
bool checkAddrList(u8 *srcAddr,u8 *dstAddr,u8 mode)
{}


/***************************************
*���룺����ָ��
*����:TRUE�����ɹ�
*����:������������֡
*����:CJJ
***************************************/
void paresUpgradStart(upGradStart_st *p)
{}

/***************************************
*���룺����ָ��
*����:TRUE�����ɹ�
*����:�����������ݰ�
*����:CJJ
***************************************/
void paresUpgradDat(upGradDat_st *p)
{}

/***************************************
*���룺��
*����:TURE��ʾд�뱸��ʽ������ȷ
*����:�жϱ���ʽ�����Ƿ����
*����:CJJ
***************************************/
bool upGradeDone(void)
{}

/*************************************************
*�������ƣ�SgdEraseAppCodeFlash
*����������������bootloader����������
*����������� 
*����������� 
*�� �� ֵ����
*************************************************/
void  SgdEraseBootloaderFlash(void)
{}

/***************************************
*����:��
*���:��
*����:��
*����:����bootloader����
*����:CJJ
***************************************/
bool upGradeBoot(void)
{}

//check_sum������룬�̻������һpage��������ġ�
const u8 CRC_FLAHS_DAT[] = 
{};

/***************************************
*����:��
*���:��
*����:��
*����:����check_sum����
*����:CJJ
***************************************/
void updateCheckProgram(void)
{}

/***************************************
*����:��
*���:��
*����:��
*����:ִ������
*����:CJJ
***************************************/
void exeUpgrad(void)
{}

/***************************************
*����:��
*���:��
*����:��
*����:�ڵ���򱸷�ʽ����
*����:CJJ
***************************************/
void snodeUpgrade(void)
{}

/***************************************
*����:��
*���:��
*����:��
*����:����ھӽڵ��
*����:CJJ
***************************************/
void checkNeighbour(void)
{
  static u32 t0 = 0;
  
  //ÿ���Ӽ��һ���ھ��б�����ϻ�ʱ�䵽��(48Сʱ)�����޳���Ӧ�ڵ�
  if(wv_ticks > t0)
  {
    if(wv_ticks - t0 > (60 * 1000))
    {
      t0 = wv_ticks;
      
      //CJJ-2018-1-11��ˮ���ھ��ϻ����
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
      
      //CJJ-2018-1-11���ϱ�ʽˮ���ϻ����
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
  //���������
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

