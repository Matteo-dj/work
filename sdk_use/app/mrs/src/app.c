#include "app.h"
#include "nwk.h"
#include "delay.h"
#include "Rf_common.h"
#include  "LowPowerMeter.h"
#include "mac.h"
#include "task.h"
#include "radio.h"
#include "rf_main.h"


#define   MAX_PARITY_ID         0x02    //����У���ʶ���ֵ
#define   MAX_BAUDRATE_ID       0x05    //���ڲ����ʱ�ʶ���ֵ
#define   MAX_TX_POWER_ID       0x07    //���书�ʱ�ʶ���ֵ
#define   MAX_NUMS_GROUP        32      //�ŵ�������ֵ
#define   MAX_RSSI_THRESHOLD    120     //RSSI�������ֵ

//mac������֡֡ͷ��Ϣ����ṹ��
extern sMAC_FrameHead_6 sMACRxHead;
//�����֡ͷ��Ϣ����ṹ��
extern sNwkRecvFrmaeHeadInfo sNwkRecvHeadInfo;
//�����֡���
extern  u8  nwkFrameNumber;
//Ӧ�ò�֡���
static  u8  appFrameNumber;
//�յ���֡��֡���
u8  appRecvFrameNumber;

void (*pAppFun[])( u8 *buf, u16 len ) = 
{ 
  SgdAppParseConfirm_DenyFrame,
  SgdAppParseCommdFrame,
  SgdAppDataRelayFrame,
  SgdAppReportFrame
};


/***************************************
*����:buf,len
*���:��
*����:��
*����:APS���֡����
*����:���±�
***************************************/
void SgdAppParseFrame( u8 *buf, u16 len )
{
  if( (buf == NULL) || ( len == 0 ) ) return;
  u8 pos = 0;
  sAppFcd *pFcd = (sAppFcd *)buf;
  if( pFcd->frameType != COMMAND_FRAME && pFcd->frameType != DATA_RELAY_FRAME) return; //֡���Ͳ���
  pos = sizeof(sAppFcd);
  appRecvFrameNumber = buf[pos++];
  if( pFcd->OEI )
  {
    pos += (buf[pos]+1);
  }
  len -= pos;

  {
    pAppFun[pFcd->frameType]( &buf[pos], len );
  }
}

/***************************************
*����:buf,len
*���:��
*����:��
*����:APS��ȷ��/����֡��������
*����:���±�
***************************************/
void SgdAppParseConfirm_DenyFrame( u8 *buf, u16 len )
{}

/***************************************
*����:buf,len
*���:��
*����:��
*����:APS������֡��������
*����:���±�
***************************************/
void SgdAppParseCommdFrame( u8 *buf, u16 len )
{
  if( (buf == NULL) || (len == 0) ) return;
  u8 pos = 0;
  u8 status = 0;
  u8 CommdType = buf[pos++];
  len -= pos;
  switch( CommdType )
  {
  #if 0
  case  ConfigUart:
    status = SgdAppUartConfig( &buf[pos], len );
    SgdAppFormConfirm_DenyFrame( status );
    break;
  case  SetChannelGroupNum:
    status = SgdAppSetChannelGroup( &buf[pos], len );
    SgdAppFormConfirm_DenyFrame( status );
    break;
  case  SetRSSIThreshold:
    status = SgdAppSetRssiThreshold( &buf[pos], len );
    SgdAppFormConfirm_DenyFrame( status );
    break;
	#endif
  case  SetTxPowrer:
    status = SgdAppSetTxPower( &buf[pos], len );
    SgdAppFormConfirm_DenyFrame( status );
    break;
  case  GetNodeConfigInfo:
    SgdAppGetNodeConfigInfo();      
    break;
	#if 0
  case  RebootNode:
    status = SgdAppRebootDev( &buf[pos], len );
    SgdAppFormConfirm_DenyFrame( status );
    break;
  case  UpGradeNode:
    status = SgdAppUpGradeDev( &buf[pos], len );
    //CJJ-2017-6-20.���ظ����ϣ���ֹ�����ŵ��������·籩
    //SgdAppFormConfirm_DenyFrame( status );
    break;   
	#endif
  case  EXTEND_CI:
    SgdAppParseExtendCI( &buf[pos], len );
    break;
  default:   
    break;
  }
}

/***************************************
*����:buf,len
*���:��
*����:��
*����:��ȡ�ڵ��������Ϣ
*����:���±�
***************************************/
void SgdAppGetNodeConfigInfo(void)
{
  u8 pos = 0;
  memset( &sWLTx, 0x00, sizeof(sWLTx) );
  sAppFcd *pFcd = (sAppFcd *)sWLTx.sBuff.data;
  pFcd->frameType = COMMAND_FRAME;
  pos = sizeof(sAppFcd);
  sWLTx.sBuff.data[pos++] = appRecvFrameNumber;
  sWLTx.sBuff.data[pos++] = GetNodeConfigInfo;
  
  memcpy( &sWLTx.sBuff.data[pos], sNode.longAddr, LONG_ADDR_LEN );
  pos += LONG_ADDR_LEN;
  sWLTx.sBuff.data[pos++] = NODE_TYPE_VALUE;
  memcpy( &sWLTx.sBuff.data[pos], sNode.panID, MAC_PANID_LEN );
  pos += MAC_PANID_LEN;
  memcpy( &sWLTx.sBuff.data[pos], sNode.shortAddr, SHORT_ADDR_LEN );
  pos += SHORT_ADDR_LEN;

  sWLTx.sBuff.data[pos++] = VENDER_ID1;  
  sWLTx.sBuff.data[pos++] = VENDER_ID2;
  sWLTx.sBuff.data[pos++] = HARDWARE_SUB_VER; 
  sWLTx.sBuff.data[pos++] = HARDWARE_MASTER_VER;
  sWLTx.sBuff.data[pos++] = SOFTWARE_SUB_VER;
  sWLTx.sBuff.data[pos++] = SOFTWARE_MASTER_VER;
  sWLTx.sBuff.data[pos++] = BOOTLOADER_VER;
  sWLTx.sBuff.data[pos++] = Flash_Readbyte( TX_POWER );
  sWLTx.sBuff.data[pos++] = 0;
  sWLTx.sBuff.data[pos++] = sNode.channelGroupNum;
  sWLTx.sBuff.data[pos++] = powerDown.tei[0];
  sWLTx.sBuff.data[pos++] = powerDown.tei[1];
  sWLTx.sBuff.data[pos++] = 0;
  sWLTx.sBuff.data[pos++] = 0;
  sWLTx.sBuff.data[pos++] = 0;
  sWLTx.sBuff.data[pos] = 0;

  sWLTx.sBuff.len = pos;
  
  SgdNwkFormDataFrame( &sWLTx.sBuff );
}


/***************************************
*����:buf,len
*���:��
*����:��
*����:����APS��ȷ��/����֡
*����:���±�
***************************************/
void SgdAppFormConfirm_DenyFrame( u8 status )
{
  memset( &sWLTx, 0x00, sizeof(sWLTx) );
  u8 *p = sWLTx.sBuff.data;
  *( p + sWLTx.sBuff.len++ ) = 0x00;
  *( p + sWLTx.sBuff.len++ ) = appRecvFrameNumber;
  *( p + sWLTx.sBuff.len++ ) = status;
  
  SgdNwkFormDataFrame( &sWLTx.sBuff );
}

/***************************************
*����:buf,len
*���:��
*����:��
*����:APS�����ô��ں���
*����:���±�
***************************************/
u8  SgdAppUartConfig( u8 *buf, u8 len )
{
  return 0;
}

/***************************************
*����:buf,len
*���:��
*����:��
*����:APS�������ŵ����
*����:���±�
***************************************/
u8  SgdAppSetChannelGroup( u8 *buf, u8 len )
{
#if 0
  if( buf[0] > 32 ) return 0;
  sNode.channelGroupNum = buf[0];
  datFromFlashToRam();
  Flash_Writebyte( CHANNEL_GROUP, sNode.channelGroupNum,TRUE );
  SgdInitChannelIndexArr();
  sUpRssiThreshold.timer = RssiThresholdUpdateInterval - 5000;
  return 1;
#else
  return 0;
#endif
}

/***************************************
*����:buf,len
*���:��
*����:��
*����:APS������RSSI����ֵ
*����:���±�
***************************************/
u8  SgdAppSetRssiThreshold( u8 *buf, u8 len )
{
  return 0;
}


/***************************************
*����:buf,len
*���:��
*����:��
*����:APS�����÷��书��
*����:���±�
***************************************/
u8  SgdAppSetTxPower( u8 *buf, u8 len )
{
  if( buf[0] > MAX_TX_POWER_ID ) return 0;

  datFromFlashToRam();
  Flash_Writebyte( TX_POWER, buf[0] ,TRUE);

  vSetRadioTxpower(buf[0]);
  
  return 1;
}

/***************************************
*����:buf,len
*���:��
*����:��
*����:APS���豸��������
*����:���±�
***************************************/
u8  SgdAppRebootDev( u8 *buf, u8 len )
{
  return 0;
}


/***************************************
*����:buf,len
*���:��
*����:��
*����:APS�������������
*����:���±�
***************************************/
u8  SgdAppUpGradeDev( u8 *buf, u8 len )
{
  return 0;
}

/***************************************
*����:��
*���:��
*����:��
*����:�����豸����ַ
*����:���±�
***************************************/
u8  SgdAppSetLongAddr( u8 *buf, u8 len )
{
  if( len != LONG_ADDR_LEN ) return 0;
  u8 i = 0;
  
  for( i = 0; i < (len-1); i++ )
  {
    sNode.longAddr[i] = buf[i];
  }
  sNode.longAddr[i] = buf[i];
  return 1;
}


/***************************************
*����:��
*���:��
*����:aps�غ�������ݳ���
*����:�����豸����ַ
*����:CJJ
***************************************/
u8 apsPaylodMaxLen(void)
{
  return 0;
}

/***************************************
*����:��
*���:��
*����:��
*����:�����豸����ַ
*����:���±�
***************************************/
void  SgdAppDataRelayFrame( u8 *buf, u16 len )
{
  u8 pos = 0;
  //u8 DTI = buf[pos++];
  pos++; //weichao
  
  memset( (char *)&sUsart2, 0x00, sizeof(sUsart2) );
  sUsart2.dataLen = len - pos;
  if( sUsart2.dataLen > MAX_DL_645_FRAME_LEN ) return;
  memcpy((char *) sUsart2.dataBuffer, &buf[pos], sUsart2.dataLen );
  MRS_SendMrData(sUsart2.dataBuffer, sUsart2.dataLen, HI_DMS_CHL_UART_PORT_APP);
}


/***************************************
*����:buf,len
*���:��
*����:��
*����:APS���ϱ�֡����
*����:���±�
***************************************/
void  SgdAppReportFrame( u8 *buf, u16 len )
{

}

/***************************************
*����:buf,len
*���:��
*����:��
*����:APS��㲥Уʱ����֡
*����:���±�
***************************************/
void  SgdAppBroadcastTime( u8 *buf, u16 len )
{}

/***************************************
*����:psDRParams
*���:��
*����:��
*����:�㲥УʱMAC��֡
*����:���±�
***************************************/
void sgdMacFormBroadcastFrame( sBuffer *pBuff )
{}


/***************************************
*����:buf,len
*���:��
*����:��
*����:�㲥УʱNWK����֡
*����:CJJ
***************************************/
void  sgdNwkFormBroadcastFrame( sBuffer *pBuffer )
{}

/***************************************
*����:buf,len
*���:��
*����:��
*����:ת���㲥Уʱ����֡
*����:���±�
***************************************/
void SgdAppRelayBroadcastFrame( u8 *buf, u16 len )
{}

/***************************************
*����:buf,len
*���:��
*����:��
*����:����APS���ϱ�֡
*����:���±�
***************************************/
void  SgdAppFormReportFrame( u8 *pInData, u8 inLen )
{}

/***************************************
*����:buf,len
*���:��
*����:��
*����:����APS������ת��֡
*����:���±�
***************************************/
void  SgdAppFormDataRelayFrame( u8 *pInData, u8 inLen )
{}

/***************************************
*����:��
*���:��
*����:��
*����:����Ӧ�ò�֡���
*����:���±�
***************************************/
void  SgdAppUpdateSequenceNum(void)
{
  if( appFrameNumber == 0xFF )
  {
    appFrameNumber = 1;
  }
  else
  {
    appFrameNumber++;
  }   
}
/***************************************
*����:��
*���:��
*����:��
*����:��ȡ�ڲ��汾
*����:���±�
***************************************/
void  SgdAppGetInternalVersion(void)
{
  u8 pos = 0;
  memset( &sWLTx, 0x00, sizeof(sWLTx) );
  sAppFcd *pFcd = (sAppFcd *)sWLTx.sBuff.data;
  pFcd->frameType = COMMAND_FRAME;
  pos = sizeof(sAppFcd);
  sWLTx.sBuff.data[pos++] = appRecvFrameNumber;
  sWLTx.sBuff.data[pos++] = EXTEND_CI;
  sWLTx.sBuff.data[pos++] = APS_GET_INTERNAL_VERSION;
  sWLTx.sBuff.data[pos++] = INTERNAL_SUB_VERSION;
  sWLTx.sBuff.data[pos++] = INTERNAL_MASTER_VERSION;
  sWLTx.sBuff.len = pos;
  
  SgdNwkFormDataFrame( &sWLTx.sBuff );  
}


/***************************************
*����:pInBuff�� inLen
*���:��
*����:��
*����:������ת�����������Ȳ�������
*����:���±�
***************************************/
void  SgdAppRecvSensitivityTest( u8 *pInBuff, u16 inLen )
{
  SensitivityTestCNT++;

  memset((char *) sUsart2.dataBuffer, 0x00, sizeof(sUsart2.dataBuffer) );
  sUsart2.dataLen = inLen;
  memcpy((char *) sUsart2.dataBuffer, pInBuff, sUsart2.dataLen ); 

  //sendMsgToHplc();
  MRS_SendMrData(sUsart2.dataBuffer, sUsart2.dataLen, HI_DMS_CHL_UART_PORT_APP);
}

/***************************************
*����:��
*���:��
*����:��
*����:����BeaconValue
***************************************/
void  SgdAppGetsBeaconValue(void)
{}

/***************************************
*����:��
*���:��
*����:��
*����:����beaconChannelIndex
***************************************/
void  SgdAppGetBeaconChannelIndexValue(void)
{}

/***************************************
*����:��
*���:��
*����:��
*����:����APS_GET_WLTXBUFF_VALUE
***************************************/
void  SgdAppGetsWLTxsBuffValue(void)
{}

void  SgdAppGetBeaconPayload(void)
{}

/***************************************
*����:��
*���:��
*����:��
*����:����RssiValue
***************************************/
void  SgdAppGetRssiValue(u8 *buf)
{
  u8 pos = 0;
  u8 inputPower = 0;
  disbaleRfSendReceive();
  RF_RxOn( FALSE, buf[0] );
  usDelay(1000);  
  u16 sum = 0;
  for(u8 i = 0; i < 20; i++ )
  {
    u8 sample = SgdGetInputPower(); 
    sum += sample;
  }
  inputPower = sum / 20;

  inputPower = SgdGetInputPower();
  enableRfSendReceive();
  memset( &sWLTx, 0x00, sizeof(sWLTx) );
  sAppFcd *pFcd = (sAppFcd *)sWLTx.sBuff.data;
  pFcd->frameType = COMMAND_FRAME;
  pos = sizeof(sAppFcd);
  sWLTx.sBuff.data[pos++] = appRecvFrameNumber;
  sWLTx.sBuff.data[pos++] = EXTEND_CI;
  sWLTx.sBuff.data[pos++] = APS_GET_RSSI_VALUE;
  sWLTx.sBuff.data[pos++] = inputPower;
  sWLTx.sBuff.len = pos;
  
  SgdNwkFormDataFrame( &sWLTx.sBuff );  
}

/***************************************
*����:buf, len
*���:��
*����:��
*����:��ȡ�ڵ�flash����
*����:CJJ
***************************************/
void SgdAppGetFlashParam(void)
{
  u8 pos = 0;
  memset( &sWLTx, 0x00, sizeof(sWLTx) );
  sAppFcd *pFcd = (sAppFcd *)sWLTx.sBuff.data;
  pFcd->frameType = COMMAND_FRAME;
  pos = sizeof(sAppFcd);
  sWLTx.sBuff.data[pos++] = appRecvFrameNumber;
  sWLTx.sBuff.data[pos++] = EXTEND_CI;
  sWLTx.sBuff.data[pos++] = APS_GET_FLASH_PARAM;
  
  for(u8 i = EEP_START_ADDR;i < EEP_END_ADDR;i++)
  {
    sWLTx.sBuff.data[pos++] = Flash_Readbyte(i);
  }
  sWLTx.sBuff.len = pos;
  SgdNwkFormDataFrame( &sWLTx.sBuff ); 
}

/***************************************
*����:buf, len
*���:��
*����:��
*����:��ȡ���书��
*����:CJJ
***************************************/
void SgdAppGetTxPower(void)
{
  u8 pos = 0;
  memset( &sWLTx, 0x00, sizeof(sWLTx) );
  sAppFcd *pFcd = (sAppFcd *)sWLTx.sBuff.data;
  pFcd->frameType = COMMAND_FRAME;
  pos = sizeof(sAppFcd);
  sWLTx.sBuff.data[pos++] = appRecvFrameNumber;
  sWLTx.sBuff.data[pos++] = EXTEND_CI;
  sWLTx.sBuff.data[pos++] = GET_TX_POWER;
  
  u8 txPower = Flash_Readbyte( TX_POWER );
  if( txPower > 0x07 )txPower = 0x00;
  sWLTx.sBuff.data[pos++] = txPower;
  sWLTx.sBuff.len = pos;
  
  SgdNwkFormDataFrame( &sWLTx.sBuff );  
}

/***************************************
*����:buf, len
*���:��
*����:��
*����:����APP����չ����
*����:���±�
***************************************/
void  SgdAppParseExtendCI( u8 *buf, u16 len )
{ 
  u8 pos = 0;
  u8 extendCI = buf[pos++];
  switch( extendCI )
  {
  case GET_INPUT_POWER_WL:
    SgdAppGetInputPower( &buf[pos], 1 );
    SgdAppReplyInputPowerForWL();      
    break;
  case GET_RSSITHRESHOLD:
    SgdAppReplyRssiThreshold();
    break;
  case  RECV_SENSITIVITY_TEST:
    SgdAppRecvSensitivityTest( &buf[pos], len - pos );
    break;
  case APS_GET_INTERNAL_VERSION:
    SgdAppGetInternalVersion();
    break;

	#if 0	
  case APS_GET_RSSI_VALUE:
      SgdAppGetRssiValue(&buf[pos]);
    break;
  case APS_GET_BEACON_INDEX_VALUE:
      SgdAppGetBeaconChannelIndexValue();
    break;
  case APS_GET_WLTXBUFF_VALUE:
    SgdAppGetsWLTxsBuffValue();
    break;
  case APS_GET_BEACON_PAY_LOAD:
      SgdAppGetBeaconPayload();
      break;
  case APS_GET_FLASH_PARAM: 
    SgdAppGetFlashParam();
    break;      
	#endif
		
  case GET_TX_POWER:
    SgdAppGetTxPower();
    break;
    
  default:
    break;
  }
}

/***************************************
*����:buf, len
*���:��
*����:��
*����:��ȡ��ǰRSSI��ֵ
*����:���±�
***************************************/
u8  SgdAppGetInputPower( u8 *buf, u8 len  )
{
  if( len != 1 ) return 0;
  memset( (char *)&sRFTest, 0x00, sizeof(sRFTest) );
  sRFTest.channelIndex = buf[0];
  
  bool interrupt_06 = FALSE;
  disbaleRfSendReceive();
  RF_RxOn( interrupt_06, sRFTest.channelIndex );
  usDelay(1000);
  sNode.inputPower = SgdGetInputPower();
  enableRfSendReceive();
  return  1;  
}

/***************************************
*����:inputPower
*���:��
*����:��
*����:ͨ�����߷��ص�ǰ���еĽ��������ź�ֵ
*����:���±�
***************************************/
void  SgdAppReplyInputPowerForWL(void)
{
  u8 pos = 0;
  memset( &sWLTx, 0x00, sizeof(sWLTx) );
  sAppFcd *pFcd = (sAppFcd *)sWLTx.sBuff.data;
  pFcd->frameType = COMMAND_FRAME;
  pos = sizeof(sAppFcd);
  sWLTx.sBuff.data[pos++] = appRecvFrameNumber;
  sWLTx.sBuff.data[pos++] = EXTEND_CI;
  sWLTx.sBuff.data[pos++] = GET_INPUT_POWER_WL;
  sWLTx.sBuff.data[pos++] = sRFTest.channelIndex;
  sWLTx.sBuff.data[pos++] = sNode.inputPower;
  sWLTx.sBuff.len = pos;
  
  SgdNwkFormDataFrame( &sWLTx.sBuff );
}

/***************************************
*����:��
*���:��
*����:��
*����:���ظ����ŵ���RSSI����ֵ
*����:���±�
***************************************/
void  SgdAppReplyRssiThreshold(void)
{
  u8 pos = 0;
  memset( &sWLTx, 0x00, sizeof(sWLTx) );
  sAppFcd *pFcd = (sAppFcd *)sWLTx.sBuff.data;
  pFcd->frameType = COMMAND_FRAME;
  pos = sizeof(sAppFcd);
  sWLTx.sBuff.data[pos++] = appRecvFrameNumber;
  sWLTx.sBuff.data[pos++] = EXTEND_CI;
  sWLTx.sBuff.data[pos++] = GET_RSSITHRESHOLD;
  memcpy( &sWLTx.sBuff.data[pos], channelRssiThresholdArr, sizeof(channelRssiThresholdArr) );
  pos += sizeof(channelRssiThresholdArr);
  sWLTx.sBuff.len = pos;
  
  SgdNwkFormDataFrame( &sWLTx.sBuff );  
}

/*-----------APS���������-------------*/
void  SgdApsTestCommand(u8 *inDat,u16 inLen)
{

#if 0
  u8 pos = 0;
  memset( &sWLTx, 0x00, sizeof(sWLTx) );
  sAppFcd *pFcd = (sAppFcd *)sWLTx.sBuff.data;
  pFcd->frameType = COMMAND_FRAME;
  pos = sizeof(sAppFcd);
  sWLTx.sBuff.data[pos++] = appRecvFrameNumber;
  sWLTx.sBuff.data[pos++] = EXTEND_CI;
  sWLTx.sBuff.data[pos++] = APS_TEST_COMMAND;
  sWLTx.sBuff.data[pos++] = 0xff;
  sWLTx.sBuff.data[pos++] = 0xaa;
  sWLTx.sBuff.data[pos++] = 0xaa;
  sWLTx.sBuff.data[pos++] = 0xff;
  memcpy( &sWLTx.sBuff.data[pos], inDat, inLen);
  pos += inLen;
  sWLTx.sBuff.len = pos;
  
  SgdNwkFormDataFrame( &sWLTx.sBuff );  

  SgdWLSendProcess();

  HI_MDM_Sleep(100);
#endif

}


void  SgdAppReplySystemUpTime(void)
{}

/***************************************
*����:pInBuff�� inLen
*���:��
*����:��
*����:����վת��������Ӧ��֡
*����:���±�
***************************************/
void  SgdRelayAPSAlarmBoxRespond( u8 *pInBuff, u16 inLen )
{}

/***************************************
*����:pInBuff�� inLen
*���:��
*����:��
*����:����վת�������������ϱ�֡
*����:���±�
***************************************/
void  SgdRelayAPSAlarmBoxReported( u8 *pInBuff, u16 inLen )
{}

/***************************************
*����:��
*���:��
*����:��
*����:��ȡ�ű�֡����TE
*����:���±�
***************************************/
void  SgdAppReplyBeaconTE(void)
{}


/***************************************
*����:buff, len 
*���:��
*����:�������
*����:�ӹ����ֵ�����л�ȡ�������
*����:���±�
***************************************/
u32 SgdAppGetInRechargeCnt( u8 *buff, u8 len )
{}

/***************************************
*����:��
*���:��
*����:�������
*����:�ӵ���л�ȡ�������
*����:���±�
***************************************/
u32 SgdAppGetMeterRechargeCnt(void)
{}


/***************************************
*����:��
*���:buff
*����:len
*����:���칺���ֵ��Ӧ֡
*����:���±�
***************************************/
u8  SgdAppFormRechargeRespond( u8 *buff )
{}

