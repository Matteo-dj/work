#include "app.h"
#include "nwk.h"
#include "delay.h"
#include "Rf_common.h"
#include  "LowPowerMeter.h"
#include "mac.h"
#include "task.h"
#include "radio.h"
#include "rf_main.h"


#define   MAX_PARITY_ID         0x02    //串口校验标识最大值
#define   MAX_BAUDRATE_ID       0x05    //串口波特率标识最大值
#define   MAX_TX_POWER_ID       0x07    //发射功率标识最大值
#define   MAX_NUMS_GROUP        32      //信道组号最大值
#define   MAX_RSSI_THRESHOLD    120     //RSSI门限最大值

//mac层数据帧帧头信息保存结构体
extern sMAC_FrameHead_6 sMACRxHead;
//网络层帧头信息保存结构体
extern sNwkRecvFrmaeHeadInfo sNwkRecvHeadInfo;
//网络层帧序号
extern  u8  nwkFrameNumber;
//应用层帧序号
static  u8  appFrameNumber;
//收到的帧的帧序号
u8  appRecvFrameNumber;

void (*pAppFun[])( u8 *buf, u16 len ) = 
{ 
  SgdAppParseConfirm_DenyFrame,
  SgdAppParseCommdFrame,
  SgdAppDataRelayFrame,
  SgdAppReportFrame
};


/***************************************
*输入:buf,len
*输出:无
*返回:无
*描述:APS层解帧函数
*作者:张月保
***************************************/
void SgdAppParseFrame( u8 *buf, u16 len )
{
  if( (buf == NULL) || ( len == 0 ) ) return;
  u8 pos = 0;
  sAppFcd *pFcd = (sAppFcd *)buf;
  if( pFcd->frameType != COMMAND_FRAME && pFcd->frameType != DATA_RELAY_FRAME) return; //帧类型不对
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
*输入:buf,len
*输出:无
*返回:无
*描述:APS层确认/否认帧解析函数
*作者:张月保
***************************************/
void SgdAppParseConfirm_DenyFrame( u8 *buf, u16 len )
{}

/***************************************
*输入:buf,len
*输出:无
*返回:无
*描述:APS层命令帧解析函数
*作者:张月保
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
    //CJJ-2017-6-20.不回复否认，防止公共信道升级导致风暴
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
*输入:buf,len
*输出:无
*返回:无
*描述:读取节点的配置信息
*作者:张月保
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
*输入:buf,len
*输出:无
*返回:无
*描述:构造APS层确认/否认帧
*作者:张月保
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
*输入:buf,len
*输出:无
*返回:无
*描述:APS层配置串口函数
*作者:张月保
***************************************/
u8  SgdAppUartConfig( u8 *buf, u8 len )
{
  return 0;
}

/***************************************
*输入:buf,len
*输出:无
*返回:无
*描述:APS层配置信道组号
*作者:张月保
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
*输入:buf,len
*输出:无
*返回:无
*描述:APS层设置RSSI门限值
*作者:张月保
***************************************/
u8  SgdAppSetRssiThreshold( u8 *buf, u8 len )
{
  return 0;
}


/***************************************
*输入:buf,len
*输出:无
*返回:无
*描述:APS层设置发射功率
*作者:张月保
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
*输入:buf,len
*输出:无
*返回:无
*描述:APS层设备重启命令
*作者:张月保
***************************************/
u8  SgdAppRebootDev( u8 *buf, u8 len )
{
  return 0;
}


/***************************************
*输入:buf,len
*输出:无
*返回:无
*描述:APS层软件升级命令
*作者:张月保
***************************************/
u8  SgdAppUpGradeDev( u8 *buf, u8 len )
{
  return 0;
}

/***************************************
*输入:无
*输出:无
*返回:无
*描述:设置设备长地址
*作者:张月保
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
*输入:无
*输出:无
*返回:aps载荷最大数据长度
*描述:设置设备长地址
*作者:CJJ
***************************************/
u8 apsPaylodMaxLen(void)
{
  return 0;
}

/***************************************
*输入:无
*输出:无
*返回:无
*描述:设置设备长地址
*作者:张月保
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
*输入:buf,len
*输出:无
*返回:无
*描述:APS层上报帧函数
*作者:张月保
***************************************/
void  SgdAppReportFrame( u8 *buf, u16 len )
{

}

/***************************************
*输入:buf,len
*输出:无
*返回:无
*描述:APS层广播校时命令帧
*作者:张月保
***************************************/
void  SgdAppBroadcastTime( u8 *buf, u16 len )
{}

/***************************************
*输入:psDRParams
*输出:无
*返回:无
*描述:广播校时MAC组帧
*作者:张月保
***************************************/
void sgdMacFormBroadcastFrame( sBuffer *pBuff )
{}


/***************************************
*输入:buf,len
*输出:无
*返回:无
*描述:广播校时NWK层组帧
*作者:CJJ
***************************************/
void  sgdNwkFormBroadcastFrame( sBuffer *pBuffer )
{}

/***************************************
*输入:buf,len
*输出:无
*返回:无
*描述:转发广播校时命令帧
*作者:张月保
***************************************/
void SgdAppRelayBroadcastFrame( u8 *buf, u16 len )
{}

/***************************************
*输入:buf,len
*输出:无
*返回:无
*描述:构造APS层上报帧
*作者:张月保
***************************************/
void  SgdAppFormReportFrame( u8 *pInData, u8 inLen )
{}

/***************************************
*输入:buf,len
*输出:无
*返回:无
*描述:构造APS层数据转发帧
*作者:张月保
***************************************/
void  SgdAppFormDataRelayFrame( u8 *pInData, u8 inLen )
{}

/***************************************
*输入:无
*输出:无
*返回:无
*描述:更新应用层帧序号
*作者:张月保
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
*输入:无
*输出:无
*返回:无
*描述:获取内部版本
*作者:张月保
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
*输入:pInBuff， inLen
*输出:无
*返回:无
*描述:往串口转发接收灵敏度测试命令
*作者:张月保
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
*输入:无
*输出:无
*返回:无
*描述:返回BeaconValue
***************************************/
void  SgdAppGetsBeaconValue(void)
{}

/***************************************
*输入:无
*输出:无
*返回:无
*描述:返回beaconChannelIndex
***************************************/
void  SgdAppGetBeaconChannelIndexValue(void)
{}

/***************************************
*输入:无
*输出:无
*返回:无
*描述:返回APS_GET_WLTXBUFF_VALUE
***************************************/
void  SgdAppGetsWLTxsBuffValue(void)
{}

void  SgdAppGetBeaconPayload(void)
{}

/***************************************
*输入:无
*输出:无
*返回:无
*描述:返回RssiValue
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
*输入:buf, len
*输出:无
*返回:无
*描述:获取节点flash参数
*作者:CJJ
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
*输入:buf, len
*输出:无
*返回:无
*描述:获取发射功率
*作者:CJJ
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
*输入:buf, len
*输出:无
*返回:无
*描述:解析APP层扩展命令
*作者:张月保
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
*输入:buf, len
*输出:无
*返回:无
*描述:获取当前RSSI的值
*作者:张月保
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
*输入:inputPower
*输出:无
*返回:无
*描述:通过无线返回当前空中的接收能量信号值
*作者:张月保
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
*输入:无
*输出:无
*返回:无
*描述:返回各个信道的RSSI门限值
*作者:张月保
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

/*-----------APS层测试命令-------------*/
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
*输入:pInBuff， inLen
*输出:无
*返回:无
*描述:给主站转发报警箱应答帧
*作者:张月保
***************************************/
void  SgdRelayAPSAlarmBoxRespond( u8 *pInBuff, u16 inLen )
{}

/***************************************
*输入:pInBuff， inLen
*输出:无
*返回:无
*描述:给主站转发报警箱主动上报帧
*作者:张月保
***************************************/
void  SgdRelayAPSAlarmBoxReported( u8 *pInBuff, u16 inLen )
{}

/***************************************
*输入:无
*输出:无
*返回:无
*描述:获取信标帧周期TE
*作者:张月保
***************************************/
void  SgdAppReplyBeaconTE(void)
{}


/***************************************
*输入:buff, len 
*输出:无
*返回:购电次数
*描述:从购电充值命令中获取购电次数
*作者:张月保
***************************************/
u32 SgdAppGetInRechargeCnt( u8 *buff, u8 len )
{}

/***************************************
*输入:无
*输出:无
*返回:购电次数
*描述:从电表中获取购电次数
*作者:张月保
***************************************/
u32 SgdAppGetMeterRechargeCnt(void)
{}


/***************************************
*输入:无
*输出:buff
*返回:len
*描述:构造购电充值响应帧
*作者:张月保
***************************************/
u8  SgdAppFormRechargeRespond( u8 *buff )
{}

