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
*输入:buf, len
*输出:无
*返回:无
*描述:组织感知广播帧到发送缓冲
*作者:Just Hon
***************************************/
void MakeFeelBroadFrameToTxbuffer( u8 *buf, u16 len)
{}

/***************************************
*输入:buf,len
*输出:无
*返回:无
*描述:解析MAC层感知广播帧
*作者:Just Hon
***************************************/
void  SgdParseMACFeelNetBroadFrame( u8 *buf, u16 len )
{}

/**************************************
*输入:buf,len 
*输出:无
*返回:无
*描述:解析MAC层帧
*作者:张月保
***************************************/
void  SgdParseMacFrame( u8 *buf, u16 len )
{ 
  u8 pos = 0;
  u8 macBroadAddr[7] = {0};
  bool boardFrame = FALSE;
  
  memset( macBroadAddr, 0xFF, sizeof(macBroadAddr) );
  if( macSequenceNumber == 0 )
  {
    //CJJ-2017-9-11：MAC层帧序号初始值为随机数
    macSequenceNumber = gRandNumber%0xff;
    if( macSequenceNumber == 0 )
    {
      macSequenceNumber++;
    }
  }
  //HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_025, HI_DIAG_MT("[SgdParseMacFrame]aa"));
  //北京版双模
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
    //如果MAC层帧类型不正确，则返回
    if( pFCD->frameType != MacDataFrame && pFCD->frameType != MacCommdFrame) return;
    //如果没有PANID域，则返回
    if( !pFCD->PanID )  return;
    
    pos =  MAC_FCD_LEN + MAC_SEQUENCE_NUMBER_LEN;
    memcpy( sMACRxHead.PanID, &buf[pos], MAC_PANID_LEN );
    pos += MAC_PANID_LEN;
    
    if( pFCD->dstAddrMode == LONG_ADDR_MODE )
    {
      memcpy( sMACRxHead.dstAddr, &buf[pos], LONG_ADDR_LEN );
      //判断目标地址是否当前设备的长地址，或者是否广播地址
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
      //判断目标地址是否当前设备的长地址，或者是否广播地址
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

    //如果确认请求域为真，则响应确认帧
     //CJJ-2017-12-27:只有是非广播帧才回复确认
    if( (pFCD->confirm) &&  (FALSE == boardFrame))
    { 
      SgdMacFormAndSendConfirm( buf, len );   
    }
    //有扩展信息域
    if( pFCD->expandInfo )
    {
      gMACexpandInfo = (MACexpandInfo_st*)&buf[pos];
      pos += (buf[pos] + 1);
    }
    //CJJ-2017-12-27:解析错位不处理
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
*输入:无
*输出:无
*返回:返回0，表示当前信道忙；返回1，表示当前信道空闲
*描述:执行CSMA-CA算法，判断信道是否空闲
*作者:张月保
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
      //延时X个退避周期
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
*输入:psDRParams
*输出:无
*返回:无
*描述:上层请求数据处理函数,组织MAC层数据帧
*作者:张月保
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

  //如果缓冲区中的当前数据长度后移backMoveLen个自己节后，超出了缓冲区的最大长度
  //则直接返回，不再往下执行
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
*输入:panID
*输出:无
*返回:将要暂存参数的位置，如果为NULL没有找到
*描述:根据给定的PANid,查找fifo里数据，找出即将要存储参数的位置
*作者:张月保
***************************************/
sMacBeaconParam * sMacSaveBeaconParam(u8 *panID)
{}

/***************************************
*输入:buf, len
*输出:无
*返回:无
*描述:解析MAC层信标帧，并判断是否需要转发
*作者:张月保
***************************************/
void SgdParseMACBeaconFrame( u8 *buf, u16 len )
{}

/***************************************
*输入:buf, len
*输出:无
*返回:无
*描述:解析MAC层数据帧
*作者:张月保
***************************************/
void SgdParseMACDataFrame( u8 *buf, u16 len )
{ 
  SgdNwkParseFrame( buf, len );
}

/***************************************
*输入:buf,len
*输出:无
*返回:无
*描述:构造确认帧，并发送
*作者:张月保
***************************************/
void SgdMacFormAndSendConfirm( u8 *buf, u16 len )
{}


/***************************************
*输入:buf,len
*输出:无
*返回:无
*描述:解析MAC层确认帧
*作者:张月保
***************************************/
void SgdParseMACConfirmFrame( u8 *buf, u16 len )
{
  
}

/***************************************
*输入:无
*输出:无
*返回:无
*描述:回复上报式水平确认帧
*作者:cjj
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
*输入:buf,len
*输出:无
*返回:无
*描述:解析MAC层网络维护命令帧
*作者:张月保
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
        //当前模块不是最终的目标地址，转发网络维护请求命令帧
        SgdRelayNetworkMaintenance( buf, len );
      }
      else
      {
        //当前模块是最终的目标地址，而且与路由域信息一致，则响应网络维护响应帧
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
        //当前模块不是最终的目标地址，转发网络维护响应命令帧
        SgdRelayNetworkMaintenance( buf, len );
      }
    }
    break;
		#endif

  //北京版双模
#if (2 == SUPPLY_AREA)
  case WaterMeterUpData:
    SgdParseLowPowerMeterUPdat(&buf[pos],len );

    sgdWaterMeterUpRespond();
    //CJJ-2018-7-4:串口透传物理层报文至HPLC
    uartRelayPlcWaterDat(sWLRx.sBuff.data,sWLRx.sBuff.len+6,RELAY_PLC_UP_WATER_FRAME);
    
    break;
#endif

  default:
    break;
  }
}

/***************************************
*输入:buf,len
*输出:无
*返回:无
*描述:构造网络维护应答命令帧
*作者:张月保
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
    //将网络维护请求命令帧中的路由域倒序
    inBuffRelayAddrPos = inBuffPos + (pOutRoute->count-1-i)*LONG_ADDR_LEN;
    memcpy( &sWLTx.sBuff.data[outBuffPos], &pInbuf[inBuffRelayAddrPos], LONG_ADDR_LEN );
    outBuffPos += LONG_ADDR_LEN;
  }
  
  inBuffPos += (pOutRoute->count * LONG_ADDR_LEN);
  memcpy( &sWLTx.sBuff.data[outBuffPos], &pInbuf[inBuffPos], pOutRoute->count - 1 ); //下行场强信息域的数据保持不变
  outBuffPos += (pOutRoute->count - 1);
 
  //获取当前场强，添加到下行场强域中
  sWLTx.sBuff.data[outBuffPos-1] = sNode.inputPower;   
  
  //添加上行场强信息域
  outBuffPos += (pOutRoute->count - 1); 
  sWLTx.sBuff.len = outBuffPos;
  
  SgdMacUpdateDSN();

  sWLTx.responseIdentifier = TRUE;
}

/***************************************
*输入:buf, len
*输出:无
*返回:无
*描述:转发网络维护命令帧
*作者:张月保
***************************************/
void SgdRelayNetworkMaintenance( u8 *buf, u16 len )
{
  //转发网络维护命令帧时，只需改变帧序号域、目标地址、源地址,路径索引,第n跳的场强值
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
  
  //命令标识
  commandID = sWLTx.sBuff.data[pos++];
  
  sMAC_CMDPayloadRoute *pRoute = (sMAC_CMDPayloadRoute *)&sWLTx.sBuff.data[pos];
  u8 CurrentRssiPos = pRoute->count - pRoute->index - 2;
  pRoute->index -= 1;
  pos += sizeof(sMAC_CMDPayloadRoute);
  
  u8 dstAddrPos = pos + pRoute->index * LONG_ADDR_LEN;
  //取下一跳的目标地址
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
  
  //添加当前场强到场强域中
  sWLTx.sBuff.data[CurrentRssiPos] = sNode.inputPower;
  
  SgdMacUpdateDSN();
  
  sWLTx.responseIdentifier = TRUE;
}
#endif
/***************************************
*输入:buf
*输出:无
*返回:无
*描述:构造转发信标帧
*作者:张月保
***************************************/
void  SgdMacFormRelayBeaconFrame( u8 *buf, u16 len, u8 *ts_levelNum )
{}

/***************************************
*输入:无
*输出:无
*返回:无
*描述:更新网络层帧序号
*作者:张月保
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
*输入:无
*输出:无
*返回:无
*描述:使能帧间隔计时器
*作者:张月保
***************************************/
void  SgdEnableSysTimer( u32 time )
{}

/***************************************
*输入:无
*输出:无
*返回:无
*描述:失能帧间隔计时器
*作者:张月保
***************************************/
void  SgdDisableSysTimer(void)
{}

/***************************************
*输入:pInBuff, inLen
*输出:无
*返回:无
*描述:转发主站对智能箱数据请求命令
*作者:张月保
***************************************/
void  SgdRelayMACAlarmBoxDataRequest( u8 *pInBuff, u16 inLen )
{}

/***************************************
*输入:pInBuff, inLen
*输出:无
*返回:无
*描述:转发主站对智能箱事件上报应答帧
*作者:张月保
***************************************/
void  SgdRelayMACAlarmBoxReportedRespond( u8 *pInBuff, u16 inLen )
{}

/***************************************
*输入:buf, len
*输出:无
*返回:无
*描述:解析MAC层广播帧
*作者:张月保
***************************************/
void    SgdParseMACBroadFrame( u8 *buf, u16 len )
{}

/***************************************
*输入:pPayload
*输出:无
*返回:无
*描述:转发MAC层广播帧
*作者:张月保
***************************************/
bool  SgdRelayMACBroadFrame( sMAC_BroadFramePayload *pPayload )
{}

/***************************************
*输入:pInBuff, inLen
*输出:无
*返回:无
*描述:转发广播点名命令
*作者:张月保
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
  //mac层源地址
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