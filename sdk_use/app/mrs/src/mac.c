#include "mac.h"
#include "phy.h"
#include "nwk.h"
#include "app.h"
#include "delay.h"
#include "Rf_common.h"
#include "task.h"
#include "LowPowerMeter.h"

sMAC_FrameHead_6 sMACRxHead;

MACexpandInfo_st *gMACexpandInfo;

u8   macSequenceNumber;

void (*pMacFrame[])( u8 *buf, u16 len ) = 
{ 
  SgdParseMACBeaconFrame,
  SgdParseMACDataFrame,
  SgdParseMACConfirmFrame,
  SgdParseMACCommandFrame,
  SgdParseMACBroadFrame,
  SgdParseMACFeelNetBroadFrame
};

/***************************************
*����:buf, len
*���:��
*����:��
*����:��֯��֪�㲥֡�����ͻ���
*����:Just Hon
***************************************/
void MakeFeelBroadFrameToTxbuffer( u8 *buf, u16 len)
{}

/***************************************
*����:buf,len
*���:��
*����:��
*����:����MAC���֪�㲥֡
*����:Just Hon
***************************************/
void  SgdParseMACFeelNetBroadFrame( u8 *buf, u16 len )
{}

/**************************************
*����:buf,len 
*���:��
*����:��
*����:����MAC��֡
*����:���±�
***************************************/
void  SgdParseMacFrame( u8 *buf, u16 len )
{ 
  u8 pos = 0;
  u8 macBroadAddr[7] = {0};
  bool boardFrame = FALSE;
  
  memset( macBroadAddr, 0xFF, sizeof(macBroadAddr) );
  if( macSequenceNumber == 0 )
  {
    //CJJ-2017-9-11��MAC��֡��ų�ʼֵΪ�����
    macSequenceNumber = gRandNumber%0xff;
    if( macSequenceNumber == 0 )
    {
      macSequenceNumber++;
    }
  }
  //HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_025, HI_DIAG_MT("[SgdParseMacFrame]aa"));
  //������˫ģ
  #if (2 == SUPPLY_AREA)
  if( (buf[0] == 0x47) && (buf[1] == 0xCD) )
  {
    sLowPowerPhyPayloadHead *pPayloadHead = (sLowPowerPhyPayloadHead *)buf;
    if(  (memcmp( (char *)pPayloadHead->dstAddr, (char *)sNode.longAddr, LONG_ADDR_LEN ) != 0) 
       &&(memcmp( (char *)pPayloadHead->dstAddr, (char *)macBroadAddr, sizeof(macBroadAddr) ) != 0) ) 
    {
      //HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_026, HI_DIAG_MT("[SgdParseMacFrame]bb"));
      return;
    }
    SgdParseLowPowerMeterFrame( buf, len );
  }
  else  
  #endif
  {
    recvChannelArrPos = channelArrPos;
    memset( &sMACRxHead, 0x00, sizeof(sMACRxHead) );
    sMacFCD *pFCD = (sMacFCD *)buf;
    sMACRxHead.sFCD = *pFCD;
    //���MAC��֡���Ͳ���ȷ���򷵻�
    if( pFCD->frameType != MacDataFrame && pFCD->frameType != MacCommdFrame) return;
    //���û��PANID���򷵻�
    if( !pFCD->PanID )  return;
    
    pos =  MAC_FCD_LEN + MAC_SEQUENCE_NUMBER_LEN;
    memcpy( sMACRxHead.PanID, &buf[pos], MAC_PANID_LEN );
    pos += MAC_PANID_LEN;
    
    if( pFCD->dstAddrMode == LONG_ADDR_MODE )
    {
      memcpy( sMACRxHead.dstAddr, &buf[pos], LONG_ADDR_LEN );
      //�ж�Ŀ���ַ�Ƿ�ǰ�豸�ĳ���ַ�������Ƿ�㲥��ַ
      if(  (memcmp( (char *)sMACRxHead.dstAddr, (char *)sNode.longAddr, LONG_ADDR_LEN ) != 0) ) 
      {
        if((memcmp( (char *)sMACRxHead.dstAddr, (char *)macBroadAddr, LONG_ADDR_LEN ) != 0)) return;
        boardFrame = TRUE;
      }
      pos += LONG_ADDR_LEN;
    }
    else if( pFCD->dstAddrMode == SHORT_ADDR_MODE )
    {
      memcpy( sMACRxHead.dstAddr, &buf[pos], SHORT_ADDR_LEN );
      //�ж�Ŀ���ַ�Ƿ�ǰ�豸�ĳ���ַ�������Ƿ�㲥��ַ
      if(  (memcmp( (char *)sMACRxHead.dstAddr, (char *)sNode.shortAddr, SHORT_ADDR_LEN ) != 0) ) 
      {
        if(memcmp( (char *)sMACRxHead.dstAddr, (char *)macBroadAddr, SHORT_ADDR_LEN ) != 0) return;
        boardFrame = TRUE;
      }
      pos += SHORT_ADDR_LEN;
    }
    else
    {
      return;
    }
    
    if( pFCD->srcAddrMode == LONG_ADDR_MODE )
    {
      memcpy( sMACRxHead.srcAddr, &buf[pos], LONG_ADDR_LEN );
      pos += LONG_ADDR_LEN;
    }
    else if( pFCD->srcAddrMode == SHORT_ADDR_MODE )
    {
      memcpy( sMACRxHead.srcAddr, &buf[pos], SHORT_ADDR_LEN );
      pos += SHORT_ADDR_LEN;
    }
    else
      return;

    //���ȷ��������Ϊ�棬����Ӧȷ��֡
     //CJJ-2017-12-27:ֻ���Ƿǹ㲥֡�Żظ�ȷ��
    if( (pFCD->confirm) &&  (FALSE == boardFrame))
    { 
      SgdMacFormAndSendConfirm( buf, len );   
    }
    //����չ��Ϣ��
    if( pFCD->expandInfo )
    {
      gMACexpandInfo = (MACexpandInfo_st*)&buf[pos];
      pos += (buf[pos] + 1);
    }
    //CJJ-2017-12-27:������λ������
    if(len < pos) return;
    len -= pos;

    {
      pMacFrame[pFCD->frameType]( &buf[pos], len );
    }
    #ifdef SPI_FLASH
    if( pFCD->expandInfo )
    {
      snodeUpgrade();
      gMACexpandInfo = NULL;
    }
    #endif
  }
}

/***************************************
*����:��
*���:��
*����:����0����ʾ��ǰ�ŵ�æ������1����ʾ��ǰ�ŵ�����
*����:ִ��CSMA-CA�㷨���ж��ŵ��Ƿ����
*����:���±�
***************************************/
bool SgdMacDoCSMA_CA( u8 rssiThreshold )
{
  u8 macMinBE = 3;
  u8 NB = 0;
  u8 BE = macMinBE;
  u8 aMaxBE = 5;
  u8 macMaxCsmaBackoffs = 4;
  u8 inputPower = 0;
	
  while( 1 )
  {
    if( BE != 0 )
    {
      u16 DelayCnt = gRandNumber % ((1 << BE) - 1);
      //��ʱX���˱�����
      HI_MDM_Sleep(DelayCnt * aUnitBackoffPeriod);
    }

    inputPower = SgdGetInputPower();
    if( inputPower <= rssiThreshold )
    {
      NB++;
      BE++;
      if( BE >= aMaxBE )
      {
        BE = aMaxBE;
      }
      
      if( NB > macMaxCsmaBackoffs ) return TRUE; 
    }
    else
    {
      return TRUE;
    }  
  }

}

/***************************************
*����:psDRParams
*���:��
*����:��
*����:�ϲ��������ݴ�����,��֯MAC������֡
*����:���±�
***************************************/
void SgdMacFormDataFrame( sBuffer *pBuff )
{
  u8  backMoveLen = MAC_FCD_LEN + MAC_SEQUENCE_NUMBER_LEN + MAC_PANID_LEN;
  if( sMACRxHead.sFCD.dstAddrMode == LONG_ADDR_MODE )
    backMoveLen += LONG_ADDR_LEN;
  else
    backMoveLen += SHORT_ADDR_LEN;
  
  if( sMACRxHead.sFCD.srcAddrMode == LONG_ADDR_MODE )
    backMoveLen += LONG_ADDR_LEN;
  else
    backMoveLen += SHORT_ADDR_LEN;

  //����������еĵ�ǰ���ݳ��Ⱥ���backMoveLen���Լ��ں󣬳����˻���������󳤶�
  //��ֱ�ӷ��أ���������ִ��
  if( (pBuff->len + backMoveLen) > RF_MAX_BUF_LEN ) return;
  SgdMemMoveBack( pBuff, backMoveLen );
  u8 pos = 0;
  sMACRxHead.sFCD.expandInfo = 0x00;
  memcpy( pBuff->data, &sMACRxHead.sFCD, MAC_FCD_LEN );
  pos += MAC_FCD_LEN;
  pBuff->data[pos++] = macSequenceNumber;
  memcpy( &pBuff->data[pos], sMACRxHead.PanID, MAC_PANID_LEN );
  pos += MAC_PANID_LEN;
  if( sMACRxHead.sFCD.dstAddrMode == LONG_ADDR_MODE )
  {
    memcpy( &pBuff->data[pos], sMACRxHead.dstAddr, LONG_ADDR_LEN );
    pos += LONG_ADDR_LEN;
  }
  else
  {
    memcpy( &pBuff->data[pos], sMACRxHead.dstAddr, SHORT_ADDR_LEN );
    pos += SHORT_ADDR_LEN;
  }

  if( sMACRxHead.sFCD.srcAddrMode == LONG_ADDR_MODE )
  {
    memcpy( &pBuff->data[pos], sMACRxHead.srcAddr, LONG_ADDR_LEN );
    pos += LONG_ADDR_LEN;
  }
  else
  {
    memcpy( &pBuff->data[pos], sMACRxHead.srcAddr, SHORT_ADDR_LEN );
    pos += SHORT_ADDR_LEN;
  }
  pBuff->len += pos;
 
  SgdMacUpdateDSN();

  sWLTx.responseIdentifier = TRUE;
}

/***************************************
*����:panID
*���:��
*����:��Ҫ�ݴ������λ�ã����ΪNULLû���ҵ�
*����:���ݸ�����PANid,����fifo�����ݣ��ҳ�����Ҫ�洢������λ��
*����:���±�
***************************************/
sMacBeaconParam * sMacSaveBeaconParam(u8 *panID)
{}

/***************************************
*����:buf, len
*���:��
*����:��
*����:����MAC���ű�֡�����ж��Ƿ���Ҫת��
*����:���±�
***************************************/
void SgdParseMACBeaconFrame( u8 *buf, u16 len )
{}

/***************************************
*����:buf, len
*���:��
*����:��
*����:����MAC������֡
*����:���±�
***************************************/
void SgdParseMACDataFrame( u8 *buf, u16 len )
{ 
  SgdNwkParseFrame( buf, len );
}

/***************************************
*����:buf,len
*���:��
*����:��
*����:����ȷ��֡��������
*����:���±�
***************************************/
void SgdMacFormAndSendConfirm( u8 *buf, u16 len )
{}


/***************************************
*����:buf,len
*���:��
*����:��
*����:����MAC��ȷ��֡
*����:���±�
***************************************/
void SgdParseMACConfirmFrame( u8 *buf, u16 len )
{
  
}

/***************************************
*����:��
*���:��
*����:��
*����:�ظ��ϱ�ʽˮƽȷ��֡
*����:cjj
***************************************/
void  sgdWaterMeterUpRespond(void)
{
  memset( &sWLTx, 0x00, sizeof(sWLTx) );
  sMAC_FrameHead_6 *pHead = (sMAC_FrameHead_6 *)&sWLTx.sBuff.data[sWLTx.sBuff.len];
  pHead->sFCD = sMACRxHead.sFCD;
  pHead->SN = macSequenceNumber;
  memcpy( pHead->PanID, sMACRxHead.PanID, MAC_PANID_LEN );
  memcpy( pHead->dstAddr, sMACRxHead.srcAddr, LONG_ADDR_LEN );
  memcpy( pHead->srcAddr, sNode.longAddr, LONG_ADDR_LEN );
  sWLTx.sBuff.len = sizeof(sMAC_FrameHead_6);
  sWLTx.sBuff.data[sWLTx.sBuff.len++] = WaterMeterUpRespond;
  SgdMacUpdateDSN();

  sWLTx.responseIdentifier = TRUE;
}

/***************************************
*����:buf,len
*���:��
*����:��
*����:����MAC������ά������֡
*����:���±�
***************************************/
void  SgdParseMACCommandFrame( u8 *buf, u16 len )
{  

  u8  pos = 0;
  u8  commandID = buf[pos++];
  switch( commandID )
  {
  #if 0
  case NetworkMaintenanceRequest:
    {
      sMAC_CMDPayloadRoute *pRoute = (sMAC_CMDPayloadRoute *)&buf[pos];
      if( pRoute->index > 0  )
      {
        //��ǰģ�鲻�����յ�Ŀ���ַ��ת������ά����������֡
        SgdRelayNetworkMaintenance( buf, len );
      }
      else
      {
        //��ǰģ�������յ�Ŀ���ַ��������·������Ϣһ�£�����Ӧ����ά����Ӧ֡
        pos += sizeof(sMAC_CMDPayloadRoute);
        if( memcmp( (char *)&buf[pos], (char *)sNode.longAddr, LONG_ADDR_LEN ) == 0 )
        {
          SgdFormMACNetworkMaintenanceRespond( buf, len );
        }
      }    
    }
    break;
  case NetworkMaintenanceRespond:
    {
      sMAC_CMDPayloadRoute *pRoute = (sMAC_CMDPayloadRoute *)&buf[pos];
      if( pRoute->index > 0  )
      {
        //��ǰģ�鲻�����յ�Ŀ���ַ��ת������ά����Ӧ����֡
        SgdRelayNetworkMaintenance( buf, len );
      }
    }
    break;
		#endif

  //������˫ģ
#if (2 == SUPPLY_AREA)
  case WaterMeterUpData:
    SgdParseLowPowerMeterUPdat(&buf[pos],len );

    sgdWaterMeterUpRespond();
    //CJJ-2018-7-4:����͸������㱨����HPLC
    uartRelayPlcWaterDat(sWLRx.sBuff.data,sWLRx.sBuff.len+6,RELAY_PLC_UP_WATER_FRAME);
    
    break;
#endif

  default:
    break;
  }
}

/***************************************
*����:buf,len
*���:��
*����:��
*����:��������ά��Ӧ������֡
*����:���±�
***************************************/
#if 0
void SgdFormMACNetworkMaintenanceRespond( u8 *pInbuf, u16 inLen )
{
  u8  i = 0;
  u8  outBuffPos = 0;
  u8  inBuffPos = 0;
  u8  inBuffRelayAddrPos = 0;
  memset( &sWLTx, 0x00, sizeof(sWLTx) );
  sMAC_FrameHead_6 *pHead = (sMAC_FrameHead_6 *)sWLTx.sBuff.data;
  pHead->sFCD.frameType = MacCommdFrame;
  pHead->sFCD.PanID = 0x01;
  pHead->sFCD.SN = 0x01;
  pHead->sFCD.dstAddrMode = LONG_ADDR_MODE;
  pHead->sFCD.srcAddrMode = LONG_ADDR_MODE;  
  pHead->SN = macSequenceNumber;
  memcpy( pHead->PanID, sMACRxHead.PanID,  MAC_PANID_LEN );
  memcpy( pHead->dstAddr, sMACRxHead.srcAddr, LONG_ADDR_LEN );
  memcpy( pHead->srcAddr, sNode.longAddr, LONG_ADDR_LEN );
  outBuffPos = sizeof(sMAC_FrameHead_6);
  
  sWLTx.sBuff.data[outBuffPos++] = NetworkMaintenanceRespond;
  sMAC_CMDPayloadRoute *pOutRoute = (sMAC_CMDPayloadRoute *)&sWLTx.sBuff.data[outBuffPos];
  
  inBuffPos ++;
  sMAC_CMDPayloadRoute *pInRoute = (sMAC_CMDPayloadRoute *)&pInbuf[inBuffPos];
  
  *pOutRoute = *pInRoute;
  pOutRoute->index = pOutRoute->count - 2;
  
  outBuffPos += sizeof(sMAC_CMDPayloadRoute);
  inBuffPos += sizeof(sMAC_CMDPayloadRoute);
  
  for( i = 0; i < pOutRoute->count; i++ )
  {
    //������ά����������֡�е�·������
    inBuffRelayAddrPos = inBuffPos + (pOutRoute->count-1-i)*LONG_ADDR_LEN;
    memcpy( &sWLTx.sBuff.data[outBuffPos], &pInbuf[inBuffRelayAddrPos], LONG_ADDR_LEN );
    outBuffPos += LONG_ADDR_LEN;
  }
  
  inBuffPos += (pOutRoute->count * LONG_ADDR_LEN);
  memcpy( &sWLTx.sBuff.data[outBuffPos], &pInbuf[inBuffPos], pOutRoute->count - 1 ); //���г�ǿ��Ϣ������ݱ��ֲ���
  outBuffPos += (pOutRoute->count - 1);
 
  //��ȡ��ǰ��ǿ����ӵ����г�ǿ����
  sWLTx.sBuff.data[outBuffPos-1] = sNode.inputPower;   
  
  //������г�ǿ��Ϣ��
  outBuffPos += (pOutRoute->count - 1); 
  sWLTx.sBuff.len = outBuffPos;
  
  SgdMacUpdateDSN();

  sWLTx.responseIdentifier = TRUE;
}

/***************************************
*����:buf, len
*���:��
*����:��
*����:ת������ά������֡
*����:���±�
***************************************/
void SgdRelayNetworkMaintenance( u8 *buf, u16 len )
{
  //ת������ά������֡ʱ��ֻ��ı�֡�����Ŀ���ַ��Դ��ַ,·������,��n���ĳ�ǿֵ
  u8  pos = 0;
  u8  commandID = 0;
  memset( &sWLTx, 0x00, sizeof(sWLTx) );
  sMAC_FrameHead_6 *pHead = (sMAC_FrameHead_6 *)sWLTx.sBuff.data;
  pHead->sFCD.frameType = MacCommdFrame;
  pHead->sFCD.PanID = 0x01;
  pHead->sFCD.SN = 0x01;
  pHead->sFCD.dstAddrMode = LONG_ADDR_MODE;
  pHead->sFCD.srcAddrMode = LONG_ADDR_MODE;  
  pHead->SN = macSequenceNumber;
  
  memcpy( pHead->PanID, sMACRxHead.PanID,  MAC_PANID_LEN );
  memcpy( pHead->srcAddr, sNode.longAddr, LONG_ADDR_LEN );
  pos = sizeof(sMAC_FrameHead_6);
  
  memcpy( &sWLTx.sBuff.data[pos], buf, len );
  sWLTx.sBuff.len = pos + len;
  
  //�����ʶ
  commandID = sWLTx.sBuff.data[pos++];
  
  sMAC_CMDPayloadRoute *pRoute = (sMAC_CMDPayloadRoute *)&sWLTx.sBuff.data[pos];
  u8 CurrentRssiPos = pRoute->count - pRoute->index - 2;
  pRoute->index -= 1;
  pos += sizeof(sMAC_CMDPayloadRoute);
  
  u8 dstAddrPos = pos + pRoute->index * LONG_ADDR_LEN;
  //ȡ��һ����Ŀ���ַ
  memcpy( pHead->dstAddr, &sWLTx.sBuff.data[dstAddrPos], LONG_ADDR_LEN );
  pos += (pRoute->count * LONG_ADDR_LEN);

  if( commandID == NetworkMaintenanceRequest )
  {
    CurrentRssiPos += pos;
  }
  else
  {
    pos += (pRoute->count - 1);
    CurrentRssiPos += pos;
  }
  
  //��ӵ�ǰ��ǿ����ǿ����
  sWLTx.sBuff.data[CurrentRssiPos] = sNode.inputPower;
  
  SgdMacUpdateDSN();
  
  sWLTx.responseIdentifier = TRUE;
}
#endif
/***************************************
*����:buf
*���:��
*����:��
*����:����ת���ű�֡
*����:���±�
***************************************/
void  SgdMacFormRelayBeaconFrame( u8 *buf, u16 len, u8 *ts_levelNum )
{}

/***************************************
*����:��
*���:��
*����:��
*����:���������֡���
*����:���±�
***************************************/
void  SgdMacUpdateDSN(void)
{
  if( macSequenceNumber == 0xFF )
  {
    macSequenceNumber = 0x01;
  }
  else
  {
    macSequenceNumber++;
  }
}

/***************************************
*����:��
*���:��
*����:��
*����:ʹ��֡�����ʱ��
*����:���±�
***************************************/
void  SgdEnableSysTimer( u32 time )
{}

/***************************************
*����:��
*���:��
*����:��
*����:ʧ��֡�����ʱ��
*����:���±�
***************************************/
void  SgdDisableSysTimer(void)
{}

/***************************************
*����:pInBuff, inLen
*���:��
*����:��
*����:ת����վ��������������������
*����:���±�
***************************************/
void  SgdRelayMACAlarmBoxDataRequest( u8 *pInBuff, u16 inLen )
{}

/***************************************
*����:pInBuff, inLen
*���:��
*����:��
*����:ת����վ���������¼��ϱ�Ӧ��֡
*����:���±�
***************************************/
void  SgdRelayMACAlarmBoxReportedRespond( u8 *pInBuff, u16 inLen )
{}

/***************************************
*����:buf, len
*���:��
*����:��
*����:����MAC��㲥֡
*����:���±�
***************************************/
void    SgdParseMACBroadFrame( u8 *buf, u16 len )
{}

/***************************************
*����:pPayload
*���:��
*����:��
*����:ת��MAC��㲥֡
*����:���±�
***************************************/
bool  SgdRelayMACBroadFrame( sMAC_BroadFramePayload *pPayload )
{}

/***************************************
*����:pInBuff, inLen
*���:��
*����:��
*����:ת���㲥��������
*����:���±�
***************************************/
void  SgdRelayMACBroadcastCallCommd( u8 *pInBuff, u16 inLen,u8 command )
{}


void relayPlcPowerDownFrame(u8 *inDat,u8 inLen)
{
  u8 pos = 0;
  u8 panID[2] = {0xff,0xff};
  u8 dstAddr[6] = {0xff,0xff,0xff,0xff,0xff,0xff};

  memset( &sWLTx, 0x00, sizeof(sWLTx) );
  sMacFCD *pFCD = (sMacFCD *)sWLTx.sBuff.data;

  pFCD->frameType = MacDataFrame;
  pFCD->PanID = 0x01;
  pFCD->SN = 0x01;
  pFCD->dstAddrMode = LONG_ADDR_MODE;
  pFCD->srcAddrMode = LONG_ADDR_MODE;
  pos = sizeof(sMacFCD);
  
  sWLTx.sBuff.data[pos++] = macSequenceNumber;
  memcpy( &sWLTx.sBuff.data[pos], &panID, MAC_PANID_LEN );
  pos += MAC_PANID_LEN;
  memcpy( &sWLTx.sBuff.data[pos], &dstAddr, LONG_ADDR_LEN);
  pos += LONG_ADDR_LEN;
  //mac��Դ��ַ
  memcpy( &sWLTx.sBuff.data[pos], sNode.longAddr, LONG_ADDR_LEN );
  pos += LONG_ADDR_LEN;

  sWLTx.sBuff.len = sizeof(sMAC_FrameHead_6);

  if((inLen + sWLTx.sBuff.len) < RF_MAX_BUF_LEN)
  {
    memcpy((char *)&sWLTx.sBuff.data[sWLTx.sBuff.len],inDat,inLen);
  }

  sWLTx.sBuff.len += inLen;
  SgdMacUpdateDSN();
}