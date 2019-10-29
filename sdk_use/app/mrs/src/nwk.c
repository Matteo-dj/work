#include "nwk.h"
#include "mac.h"
#include "phy.h"
#include "app.h"
#include "delay.h"
#include "Rf_common.h"
#include "task.h"
#include "fifo_API.h"
#include  "LowPowerMeter.h"

#define MAX_PATHS  3
#define MAX_RELAY_NUMS  6

void  (*pNwkFrame[]) ( u8 *buf, u16 len ) = 
{
  SgdNwkParseDataFrame,
  SgdNwkParseCommFrame
};

//MAC层接收帧帧头信息保存结构体
extern sMAC_FrameHead_6 sMACRxHead;

//网络层帧头信息保存结构体
sNwkRecvFrmaeHeadInfo sNwkRecvHeadInfo;

//网络层帧序号
u8   nwkFrameNumber;
/***************************************
*输入:无
*输出:无
*返回:无
*描述:初始化一些默认参数
*作者:张月保
***************************************/
void SgdNwkInit(void)
{
  memset( &sNwkRecvHeadInfo, 0x00, sizeof(sNwkRecvHeadInfo) );
  nwkFrameNumber = 0x01;
}

/***************************************
*输入:无
*输出:buf,len
*返回:无 
*描述:组织入网申请请求帧
*作者:张月保
***************************************/
void SgdNwkJoinNetWorkRequest(void)
{}

/***************************************
*输入:buf,len
*输出:无
*返回:无 
*描述:分析MAC返回数据帧
*作者:张月保
***************************************/
void SgdNwkParseFrame( u8 *buf, u16 len )
{ 
  if( ( buf == NULL ) || ( len == 0 ) ) return;
  u8 nwkBroadAddr[6] = {0};
  memset( nwkBroadAddr, 0xFF, sizeof(nwkBroadAddr) );
  u8 nwkLocalAddr[6] = {0};
  
  memset(&sNwkRecvHeadInfo, 0x00, sizeof(sNwkRecvHeadInfo));
  
  sNwkFCD *pFCD = (sNwkFCD *)buf;
  //如果网络层帧类型不正确，则返回
  if( pFCD->frameType > CommdFrame && PLC_BOARD_FRAME != buf[0]) return;

  if(PLC_BOARD_FRAME == buf[0])
  {
     uartRelayPlcBoardFrame(buf, len );
     return;
  }

  if( !SgdParseNwkFrameHead( buf, len ) ) return;
  
  if( sNwkRecvHeadInfo.dstAddrLen == LONG_ADDR_LEN)
    memcpy( nwkLocalAddr, sNode.longAddr, sNwkRecvHeadInfo.dstAddrLen );
  else
    memcpy( nwkLocalAddr, sNode.shortAddr, sNwkRecvHeadInfo.dstAddrLen );

  if( (memcmp( (char*)sNwkRecvHeadInfo.dstAddr, (char *)nwkLocalAddr, sNwkRecvHeadInfo.dstAddrLen ) == 0)
     ||(memcmp((char*)sNwkRecvHeadInfo.dstAddr, (char *)nwkBroadAddr, sNwkRecvHeadInfo.dstAddrLen ) == 0 ) ) 
  {
    //接收帧的目标地址是当前模块地址，或者是广播帧
    if( sNwkRecvHeadInfo.sFCD.routeInfo )
    {
      //有路由域
      if( sNwkRecvHeadInfo.sRouteInfo.relayCount > MAX_RELAY_NUMS ) return;
      SgdNwkReverseRelayAddr( sNwkRecvHeadInfo.relayAddr, sNwkRecvHeadInfo.sRouteInfo.relayCount, sNwkRecvHeadInfo.sRouteInfo.relay1AddrMode );
    }
    len -= sNwkRecvHeadInfo.headLen;
    {
      pNwkFrame[pFCD->frameType](buf + sNwkRecvHeadInfo.headLen, len);
    }
    return;
  }

  if( (sNwkRecvHeadInfo.sFrameNum_Radius.radius - 1) > 0  )
  {    
    u8 relayIndex = (sNwkRecvHeadInfo.sRouteInfo.relayIndexH2bit << 2) + sNwkRecvHeadInfo.sRouteInfo.relayIndexL3bit;
    if( sNwkRecvHeadInfo.sFCD.routeInfo && ( relayIndex > 0 ) )
    {
      //半径大于1，有路由，且中继索引大于0，则转发该帧
      len -= sNwkRecvHeadInfo.headLen;
      SgdNwkFormRelayFrame( buf+sNwkRecvHeadInfo.headLen, len );
    }      
  }
}

/***************************************
*输入:buf,len
*输出:无
*返回:无 
*描述:解析NWK层帧头内容
*作者:张月保
***************************************/
bool  SgdParseNwkFrameHead( u8 *buf, u16 len )
{
  u8 pos = 0;
  sNwkFCD *pFCD =  (sNwkFCD *)buf;
  sNwkRecvHeadInfo.sFCD = *pFCD;
  pos = sizeof(sNwkFCD);
  if( sNwkRecvHeadInfo.sFCD.dstAddrMode == LONG_ADDR_MODE )
  {
    sNwkRecvHeadInfo.dstAddrLen = LONG_ADDR_LEN;
  }
  else if( sNwkRecvHeadInfo.sFCD.dstAddrMode == SHORT_ADDR_MODE )
  {
    sNwkRecvHeadInfo.dstAddrLen = SHORT_ADDR_LEN;    
  }
  else
    return FALSE;
  memcpy( sNwkRecvHeadInfo.dstAddr, &buf[pos], sNwkRecvHeadInfo.dstAddrLen );
  pos += sNwkRecvHeadInfo.dstAddrLen;

  if( sNwkRecvHeadInfo.sFCD.srcAddrMode == LONG_ADDR_MODE )
  {
    sNwkRecvHeadInfo.srcAddrLen = LONG_ADDR_LEN;
  }
  else if( sNwkRecvHeadInfo.sFCD.srcAddrMode == SHORT_ADDR_MODE )
  {
    sNwkRecvHeadInfo.srcAddrLen = SHORT_ADDR_LEN;    
  }
  else
    return FALSE;
  
  memcpy( sNwkRecvHeadInfo.srcAddr, &buf[pos], sNwkRecvHeadInfo.srcAddrLen );
  pos += sNwkRecvHeadInfo.srcAddrLen;
  
  sNwk_FrameNum_Radius *pSN_Radius = (sNwk_FrameNum_Radius *)&buf[pos];
  sNwkRecvHeadInfo.sFrameNum_Radius = *pSN_Radius;
  pos += sizeof(sNwk_FrameNum_Radius);
  if( sNwkRecvHeadInfo.sFCD.routeInfo )
  {
    //有路由域
    sNwkRouteInfo *pRoute = (sNwkRouteInfo *)&buf[pos];
    sNwkRecvHeadInfo.sRouteInfo = *pRoute;
    pos += sizeof(sNwkRouteInfo);
    if( sNwkRecvHeadInfo.sRouteInfo.relayCount > MAX_RELAY_NUMS ) return FALSE;
    if( sNwkRecvHeadInfo.sRouteInfo.relay1AddrMode == LONG_ADDR_MODE )
    {
      memcpy( sNwkRecvHeadInfo.relayAddr, &buf[pos], sNwkRecvHeadInfo.sRouteInfo.relayCount *  LONG_ADDR_LEN );
      pos += (sNwkRecvHeadInfo.sRouteInfo.relayCount *  LONG_ADDR_LEN);
    }
    else if( sNwkRecvHeadInfo.sRouteInfo.relay1AddrMode == SHORT_ADDR_MODE )
    {
      memcpy( sNwkRecvHeadInfo.relayAddr, &buf[pos], sNwkRecvHeadInfo.sRouteInfo.relayCount *  SHORT_ADDR_LEN );
      pos += (sNwkRecvHeadInfo.sRouteInfo.relayCount *  SHORT_ADDR_LEN);
    }
    else
      return FALSE;
  }
  sNwkRecvHeadInfo.headLen = pos;
  return TRUE;
}

/***************************************
*输入:buf,len
*输出:无
*返回:无 
*描述:解析网络层数据帧
*作者:张月保
***************************************/
void  SgdNwkParseDataFrame( u8 *buf, u16 len )
{
  SgdAppParseFrame( buf, len );
}

/***************************************
*输入:pageIndex,页序号
*输出:无
*返回:无 
*描述:构造主动水表上报应答帧
*作者:CJJ
***************************************/
void SgdNwkFormWaterDataRespond( u8 pageIndex )
{
  u8 pos = 0;
  memset( &sWLTx, 0x00, sizeof(sWLTx) );
  sNwkFCD *pFCD = (sNwkFCD *)sWLTx.sBuff.data;
  *pFCD = sNwkRecvHeadInfo.sFCD;
  pos += sizeof(sNwkFCD);
  //接收帧的源地址作为响应帧的目标地址
  memcpy( &sWLTx.sBuff.data[pos], sNwkRecvHeadInfo.srcAddr, sNwkRecvHeadInfo.srcAddrLen );
  pos += sNwkRecvHeadInfo.srcAddrLen;
  //接收帧的目标地址作为响应帧的源地址
  memcpy( &sWLTx.sBuff.data[pos], sNwkRecvHeadInfo.dstAddr, sNwkRecvHeadInfo.dstAddrLen );
  pos += sNwkRecvHeadInfo.dstAddrLen;
  sNwk_FrameNum_Radius *pSN_Radius = (sNwk_FrameNum_Radius *)&sWLTx.sBuff.data[pos];
  *pSN_Radius = sNwkRecvHeadInfo.sFrameNum_Radius;
  pos += sizeof(sNwk_FrameNum_Radius);
  if( sNwkRecvHeadInfo.sFCD.routeInfo )
  {
    //有路由域
    if( sNwkRecvHeadInfo.sRouteInfo.relayCount > MAX_RELAY_NUMS ) return;
    pSN_Radius->radius = sNwkRecvHeadInfo.sRouteInfo.relayCount + 1;
    sNwkRouteInfo *pRoute = (sNwkRouteInfo *)&sWLTx.sBuff.data[pos];
    *pRoute = sNwkRecvHeadInfo.sRouteInfo;
    pRoute->relayIndexL3bit = pRoute->relayCount & 0x07;
    pRoute->relayIndexH2bit = pRoute->relayCount >> 3;
    pos += sizeof(sNwkRouteInfo);
    //拷贝中继地址列表
    memcpy( &sWLTx.sBuff.data[pos], sNwkRecvHeadInfo.relayAddr, pRoute->relayCount *LONG_ADDR_LEN );
    pos += (pRoute->relayCount *LONG_ADDR_LEN);
    //mac层目标地址
    memcpy( sMACRxHead.dstAddr, &sNwkRecvHeadInfo.relayAddr[(pRoute->relayCount-1)*LONG_ADDR_LEN], LONG_ADDR_LEN );
  }
  else
  {
    //没有路由域
    //半径
    pSN_Radius->radius = 0x01;
    //mac层目标地址
    memcpy( sMACRxHead.dstAddr, sNwkRecvHeadInfo.srcAddr, LONG_ADDR_LEN );
  }
  
  sWLTx.sBuff.data[pos++] = COLLECT_WATER_DATA_RESPOND; //命令标识
  u8 totalPages = 0;
  u8 nodeNum = 0;
  u8 WaterMeterDataNums = FIFO_COUNT(&waterMeteFifoCtr,TOTAL_WART_METER_SIZE+1);

  if( WaterMeterDataNums > 0 )
  {
    if( WaterMeterDataNums < WART_METER_PAGE_SIZE )
    {
      totalPages = 1;
    }
    else if( (WaterMeterDataNums % WART_METER_PAGE_SIZE) == 0 )
    {
      totalPages = (WaterMeterDataNums / WART_METER_PAGE_SIZE);
    }
    else
    {
      totalPages = (WaterMeterDataNums / WART_METER_PAGE_SIZE) + 1;
    }
    //如果页序号大于等于总页数，说明参数不正确，不响应
    if( pageIndex >= totalPages )
      pageIndex = 0;//return;
    nodeNum = ( pageIndex == (totalPages - 1) ) ? \
              WaterMeterDataNums -(pageIndex * WART_METER_PAGE_SIZE) : WART_METER_PAGE_SIZE;   
  }
  
  sWLTx.sBuff.data[pos++] = (totalPages << 4) + pageIndex;
  sWLTx.sBuff.data[pos++] = nodeNum;

  //获取上报式水表数据
  SgdNwkGetWaterMeterData( &sWLTx.sBuff.data[pos], &pos, nodeNum, pageIndex );
 
  sWLTx.sBuff.len = pos;
  
  sMACRxHead.sFCD.dstAddrMode = LONG_ADDR_MODE;
  sMACRxHead.sFCD.srcAddrMode = LONG_ADDR_MODE;
  //mac源地址
  memcpy( sMACRxHead.srcAddr, sNode.longAddr, LONG_ADDR_LEN );
  
  SgdMacFormDataFrame( &sWLTx.sBuff );  
}


/***************************************
*输入:buf,len
*输出:无
*返回:无 
*描述:解析网络层命令帧
*作者:张月保
***************************************/
void SgdNwkParseCommFrame(  u8 *buf, u16 len )
{
  if( (buf == NULL) || (len == 0) ) return;
  u8 commandType = buf[0];
  u8 currPageIndex = 0;
  //sConfigSubNOdeCommdOptions *pConfigOptions = NULL;
  switch( commandType )
  {
    case  COLLECT_WATER_DATA_REQUEST:
      currPageIndex = buf[1] & 0x0F;
      SgdNwkFormWaterDataRespond( currPageIndex );
      break;
    case  LPM_RSSI_COLLECT_REQUEST:
      currPageIndex = buf[1] & 0x0F;
      SgdNwkFormLowPowerMeterRSSIRespond( currPageIndex );
      break;
    default:
      break;
  }
}


/***************************************
*输入:无
*输出:无
*返回:无 
*描述:构造网络层入网申请响应帧
*作者:张月保
***************************************/
void SgdNwkFormJoinNetRespondFrame(void)
{}
  
void joinNetResond(joinNetResondCtr_st *pTmp)
{}

/***************************************
*输入:buf, paths
*输出:buf, len 
*返回:无 
*描述:从所有路由信息中获取一条最优的路径
*作者:张月保
***************************************/
void  SgdNwkGetBestPath( u8 *buf, u8 *len )
{}


/***************************************
*输入:buf
*输出:无
*返回:无 
*描述:保存入网申请响应帧的相关信息
*作者:张月保
***************************************/
void SgdNwkCollectJoinNetRespondInfo( u8 *buf, u16 len )
{}

/***************************************
*输入:void
*输出:无
*返回:无 
*描述:构造游离节点就绪帧
*作者:张月保
***************************************/
void  SgdNwkFormReadyFrame(void)
{}


/***************************************
*输入:pInfo
*输出:无
*返回:无 
*描述:构造没有路由信息的游离节点就绪帧
*作者:张月保
***************************************/
void  SgdNwkFormReadyFrameNoRoute( sNwkJoinNetRespondInfo *pInfo )
{}

/***************************************
*输入:pInfo
*输出:无
*返回:无 
*描述:构造有路由信息的游离节点就绪帧
*作者:张月保
***************************************/
void SgdNwkFormReadyFrameRoute( sNwkJoinNetRespondInfo *pInfo  )
{}

/***************************************
*输入:buf,len
*输出:无
*返回:无 
*描述:构造网络层场强收集应答帧
*作者:张月保
***************************************/
void SgdNwkFormRSSIRespond( u8 pageIndex )
{}

/***************************************
*输入:无
*输出:无
*返回:无 
*描述:构造网络层配置子节点应答帧,兼容路由
*作者:张月保
***************************************/
void SgdNwkConfigNodeRespond(void)
{}


/***************************************
*输入:buf
*输出:无
*返回:无 
*描述:转发命令帧
*作者:张月保
***************************************/
void SgdNwkFormRelayFrame( u8 *buf, u16 len )
{
  //转发命令帧，只需修改半径域以及中继索引
  u8 pos = 0;
  memset( &sWLTx, 0x00, sizeof(sWLTx) );
  memcpy( sWLTx.sBuff.data, &sNwkRecvHeadInfo.sFCD, sizeof(sNwkRecvHeadInfo.sFCD) );
  pos = sizeof(sNwkRecvHeadInfo.sFCD);
  memcpy( &sWLTx.sBuff.data[pos], sNwkRecvHeadInfo.dstAddr, sNwkRecvHeadInfo.dstAddrLen );
  pos += sNwkRecvHeadInfo.dstAddrLen;
  memcpy( &sWLTx.sBuff.data[pos], sNwkRecvHeadInfo.srcAddr, sNwkRecvHeadInfo.srcAddrLen );
  pos += sNwkRecvHeadInfo.srcAddrLen; 
  sNwkRecvHeadInfo.sFrameNum_Radius.radius -= 1;
  
//CJJ-2017-9-22：中继转时帧序号不变    
//  if( sNwkRecvHeadInfo.sFrameNum_Radius.frameNum == 0x0F )
//    sNwkRecvHeadInfo.sFrameNum_Radius.frameNum = 0x01;
//  else
//    sNwkRecvHeadInfo.sFrameNum_Radius.frameNum++;
  
  memcpy( &sWLTx.sBuff.data[pos], &sNwkRecvHeadInfo.sFrameNum_Radius, sizeof(sNwkRecvHeadInfo.sFrameNum_Radius) );
  pos += sizeof(sNwkRecvHeadInfo.sFrameNum_Radius);
  u8 relayIndex = (sNwkRecvHeadInfo.sRouteInfo.relayIndexH2bit << 2) + sNwkRecvHeadInfo.sRouteInfo.relayIndexL3bit;
  relayIndex -= 1;
  sNwkRecvHeadInfo.sRouteInfo.relayIndexH2bit = (relayIndex >> 3) & 0x03;
  sNwkRecvHeadInfo.sRouteInfo.relayIndexL3bit = relayIndex & 0x07;
  memcpy( &sWLTx.sBuff.data[pos], &sNwkRecvHeadInfo.sRouteInfo, sizeof(sNwkRecvHeadInfo.sRouteInfo) );
  pos += sizeof(sNwkRecvHeadInfo.sRouteInfo);
  u8 relayAddrLen = 0;
  if( sNwkRecvHeadInfo.sRouteInfo.relay1AddrMode == LONG_ADDR_MODE )
    relayAddrLen = LONG_ADDR_LEN;
  else
    relayAddrLen = SHORT_ADDR_LEN;
  if( sNwkRecvHeadInfo.sRouteInfo.relayCount > MAX_RELAY_NUMS ) return;
  memcpy( &sWLTx.sBuff.data[pos], sNwkRecvHeadInfo.relayAddr, sNwkRecvHeadInfo.sRouteInfo.relayCount*relayAddrLen );
  pos += sNwkRecvHeadInfo.sRouteInfo.relayCount*relayAddrLen; 
  memcpy( &sWLTx.sBuff.data[pos], buf, len );
  pos += len;
  sWLTx.sBuff.len = pos;
  
  //mac层目標地址
  if( relayIndex > 0 )
  {
    sMACRxHead.sFCD.dstAddrMode = sNwkRecvHeadInfo.sRouteInfo.relay1AddrMode;
    u8 macDstAddrPos = (relayIndex - 1) * relayAddrLen;
    memcpy( sMACRxHead.dstAddr, &sNwkRecvHeadInfo.relayAddr[macDstAddrPos], relayAddrLen );
  }
  else
  {
    sMACRxHead.sFCD.dstAddrMode = sNwkRecvHeadInfo.sFCD.dstAddrMode;
    memcpy( sMACRxHead.dstAddr, sNwkRecvHeadInfo.dstAddr, sNwkRecvHeadInfo.dstAddrLen );
  }
  //mac层源地址
  u8 macSrcAddrPos = relayIndex * relayAddrLen;
  sMACRxHead.sFCD.srcAddrMode = sNwkRecvHeadInfo.sRouteInfo.relay1AddrMode;
  memcpy( sMACRxHead.srcAddr, &sNwkRecvHeadInfo.relayAddr[macSrcAddrPos], relayAddrLen );  
  
  SgdMacFormDataFrame( &sWLTx.sBuff );  
}

/***************************************
*输入:buf
*输出:无
*返回:无 
*描述:保存子节点配置信息
*作者:张月保
***************************************/
void SgdNwkSaveNodeInfo( u8 *buf, u16 len )
{}


/***************************************
*输入:无
*输出:无
*返回:无 
*描述:删除收集入网响应帧信息的链表
*作者:张月保
***************************************/
void SgdNwkFreeInNetInfoLK(void)
{}

/***************************************
*输入:无
*输出:无
*返回:无 
*描述:删除组网过程中的邻居表
*作者:张月保
***************************************/
void SgdNwkFreeNeighbourList(void)
{}

/***************************************
*输入:buf,len
*输出:无
*返回:无
*描述:组织网络层数据帧
*作者:张月保
***************************************/
void  SgdNwkFormDataFrame( sBuffer *pBuffer )
{  
  if( pBuffer == NULL ) return;
  if( sNwkRecvHeadInfo.headLen == 0 ) return;
  if( (pBuffer->len + sNwkRecvHeadInfo.headLen) > RF_MAX_BUF_LEN ) return;
  SgdMemMoveBack( pBuffer, sNwkRecvHeadInfo.headLen );

  u8 pos = 0;
  sNwkFCD *pFCD = (sNwkFCD *)pBuffer->data;
  *pFCD = sNwkRecvHeadInfo.sFCD;
  pFCD->dstAddrMode = sNwkRecvHeadInfo.sFCD.srcAddrMode;
  pFCD->srcAddrMode = sNwkRecvHeadInfo.sFCD.dstAddrMode;
  
  pos += sizeof(sNwkFCD);
  //接收帧的源地址作为响应帧的目标地址
  memcpy( &sWLTx.sBuff.data[pos], sNwkRecvHeadInfo.srcAddr, sNwkRecvHeadInfo.srcAddrLen );
  pos += sNwkRecvHeadInfo.srcAddrLen;
  //接收帧的目标地址作为响应帧的源地址
  memcpy( &sWLTx.sBuff.data[pos], sNwkRecvHeadInfo.dstAddr, sNwkRecvHeadInfo.dstAddrLen );
  pos += sNwkRecvHeadInfo.dstAddrLen;
  //帧序号和半径
  sNwk_FrameNum_Radius *pSN_Radius = (sNwk_FrameNum_Radius *)&sWLTx.sBuff.data[pos];
  *pSN_Radius = sNwkRecvHeadInfo.sFrameNum_Radius;
  pos += sizeof(sNwk_FrameNum_Radius);
  if( sNwkRecvHeadInfo.sFCD.routeInfo )
  {
    //有路由域
    //半径
    u8  relayAddrLen = 0;
    u8  relayTotalLen = 0;
    if( sNwkRecvHeadInfo.sRouteInfo.relayCount > MAX_RELAY_NUMS ) return;
    pSN_Radius->radius = sNwkRecvHeadInfo.sRouteInfo.relayCount + 1;
    sNwkRouteInfo *pRoute = (sNwkRouteInfo *)&sWLTx.sBuff.data[pos];
    *pRoute = sNwkRecvHeadInfo.sRouteInfo;
    pRoute->relayIndexL3bit = pRoute->relayCount & 0x07;
    pRoute->relayIndexH2bit = pRoute->relayCount >> 3;
    pos += sizeof(sNwkRouteInfo);
    //拷贝中继地址列表
    if( sNwkRecvHeadInfo.sRouteInfo.relay1AddrMode == LONG_ADDR_MODE )
      relayAddrLen = LONG_ADDR_LEN;
    else
      relayAddrLen = SHORT_ADDR_LEN;
    relayTotalLen = pRoute->relayCount *relayAddrLen;
    
    memcpy( &sWLTx.sBuff.data[pos], sNwkRecvHeadInfo.relayAddr, relayTotalLen );
    pos += relayTotalLen;
    //mac层目标地址
    sMACRxHead.sFCD.dstAddrMode = sNwkRecvHeadInfo.sRouteInfo.relay1AddrMode;
    memcpy( sMACRxHead.dstAddr, &sNwkRecvHeadInfo.relayAddr[(pRoute->relayCount-1)*relayAddrLen], relayAddrLen );
  }
  else
  {
    //没有路由域
    //半径
    pSN_Radius->radius = 0x01;
    //mac层目标地址
    sMACRxHead.sFCD.dstAddrMode = sNwkRecvHeadInfo.sFCD.srcAddrMode;
    memcpy( sMACRxHead.dstAddr, sNwkRecvHeadInfo.srcAddr, sNwkRecvHeadInfo.srcAddrLen );
  }

  pBuffer->len += pos;
  
  sMACRxHead.sFCD.srcAddrMode = pFCD->srcAddrMode;
  memcpy( sMACRxHead.srcAddr, sNwkRecvHeadInfo.dstAddr, sNwkRecvHeadInfo.dstAddrLen );
  
  SgdMacFormDataFrame( pBuffer );

}

/***************************************
*输入:无
*输出:无
*返回:无
*描述:构造路由错误命令帧
*作者:张月保
***************************************/
void  SgdNwkFormRouteErrorFrame(void)
{ 
}

/***************************************
*输入:无
*输出:无
*返回:无
*描述:更新网络层帧序号
*作者:张月保
***************************************/
void  SgdNwkUpdateSequenceNum(void)
{
  if( nwkFrameNumber == 0x0F )
  {
    nwkFrameNumber = 1;
  }
  else
  {
    nwkFrameNumber++;
  } 
}

/***************************************
*输入:relayAddr,relayCnt,addrLen
*输出:relayAddr
*返回:无
*描述:将中继地址域的地址倒序
*作者:张月保
***************************************/
void  SgdNwkReverseRelayAddr( u8 *relayAddr, u8 relayCnt, u8 addrMode )
{
  s8 i = 0;
  u8 pos = 0;
  u8 addrList[36] = {0};
  u8 addrLen = 0;
  if( addrMode == LONG_ADDR_MODE )
    addrLen = LONG_ADDR_LEN;
  else
    addrLen = SHORT_ADDR_LEN;
  if( relayCnt > 6 ) return;

  for( i = relayCnt-1; i >= 0; i-- )
  {
    memcpy( &addrList[pos], &relayAddr[i*addrLen], addrLen );
    pos += addrLen;
  }
 
  memcpy( relayAddr, addrList, relayCnt * addrLen );

}

/***************************************
*输入:buf, len, nodeNum, pageIndex
*输出:buf, len
*返回:无
*描述:从邻居表中获取指定的邻居表地址以及RSSI值
*作者:张月保
***************************************/
void  SgdNwkGetNeighbours( u8 *buf, u8 *len, u8 nodeNum, u8 pageIndex )
{}

/***************************************
*输入:pRoute
*输出:pRoute
*返回:无
*描述:设置中继地址模式
*作者:张月保
***************************************/
void  SgdNwkSetRelayAddrMode( sNwkRouteInfo *pRoute, u8 addrMode )
{
  switch( pRoute->relayCount )
  {
  case 1:
    pRoute->relay1AddrMode = addrMode;
    break;
  case 2:
    pRoute->relay1AddrMode = addrMode;
    pRoute->relay2AddrMode = addrMode;
    break;
  case 3:
    pRoute->relay1AddrMode = addrMode;
    pRoute->relay2AddrMode = addrMode;
    pRoute->relay3AddrMode = addrMode;
    break;
  case 4:
    pRoute->relay1AddrMode = addrMode;
    pRoute->relay2AddrMode = addrMode;
    pRoute->relay3AddrMode = addrMode;
    pRoute->relay4AddrMode = addrMode;
    break;
  case 5:
    pRoute->relay1AddrMode = addrMode;
    pRoute->relay2AddrMode = addrMode;
    pRoute->relay3AddrMode = addrMode;
    pRoute->relay4AddrMode = addrMode;
    pRoute->relay5AddrMode = addrMode;
    break;
  case 6:
    pRoute->relay1AddrMode = addrMode;
    pRoute->relay2AddrMode = addrMode;
    pRoute->relay3AddrMode = addrMode;
    pRoute->relay4AddrMode = addrMode;
    pRoute->relay5AddrMode = addrMode;
    pRoute->relay6AddrMode = addrMode;
    break;
  default:
    break;
  }  
}

/***************************************
*输入:pageIndex
*输出:无
*返回:无 
*描述:构造网络层水表场强收集应答帧
*作者:张月保
***************************************/
void SgdNwkFormLowPowerMeterRSSIRespond( u8 pageIndex )
{ 
  //场强收集命令帧的目标地址和源地址必须为长地址
  if( (sNwkRecvHeadInfo.dstAddrLen != sNwkRecvHeadInfo.srcAddrLen) && (sNwkRecvHeadInfo.dstAddrLen != LONG_ADDR_LEN) ) return;
  u8 pos = 0;
  memset( &sWLTx, 0x00, sizeof(sWLTx) );
  sNwkFCD *pFCD = (sNwkFCD *)sWLTx.sBuff.data;
  *pFCD = sNwkRecvHeadInfo.sFCD;
  pos += sizeof(sNwkFCD);
  //接收帧的源地址作为响应帧的目标地址
  memcpy( &sWLTx.sBuff.data[pos], sNwkRecvHeadInfo.srcAddr, sNwkRecvHeadInfo.srcAddrLen );
  pos += sNwkRecvHeadInfo.srcAddrLen;
  //接收帧的目标地址作为响应帧的源地址
  memcpy( &sWLTx.sBuff.data[pos], sNwkRecvHeadInfo.dstAddr, sNwkRecvHeadInfo.dstAddrLen );
  pos += sNwkRecvHeadInfo.dstAddrLen;
  sNwk_FrameNum_Radius *pSN_Radius = (sNwk_FrameNum_Radius *)&sWLTx.sBuff.data[pos];
  *pSN_Radius = sNwkRecvHeadInfo.sFrameNum_Radius;
  pos += sizeof(sNwk_FrameNum_Radius);
  if( sNwkRecvHeadInfo.sFCD.routeInfo )
  {
    //有路由域
    //中继的地址模式也必须为长地址模式
    if( sNwkRecvHeadInfo.sRouteInfo.relay1AddrMode != LONG_ADDR_MODE ) return;
    //半径
    if( sNwkRecvHeadInfo.sRouteInfo.relayCount > MAX_RELAY_NUMS ) return;
    pSN_Radius->radius = sNwkRecvHeadInfo.sRouteInfo.relayCount + 1;
    sNwkRouteInfo *pRoute = (sNwkRouteInfo *)&sWLTx.sBuff.data[pos];
    *pRoute = sNwkRecvHeadInfo.sRouteInfo;
    pRoute->relayIndexL3bit = pRoute->relayCount & 0x07;
    pRoute->relayIndexH2bit = pRoute->relayCount >> 3;
    pos += sizeof(sNwkRouteInfo);
    //拷贝中继地址列表
    memcpy( &sWLTx.sBuff.data[pos], sNwkRecvHeadInfo.relayAddr, pRoute->relayCount *LONG_ADDR_LEN );
    pos += (pRoute->relayCount *LONG_ADDR_LEN);
    //mac层目标地址
    memcpy( sMACRxHead.dstAddr, &sNwkRecvHeadInfo.relayAddr[(pRoute->relayCount-1)*LONG_ADDR_LEN], LONG_ADDR_LEN );
  }
  else
  {
    //没有路由域
    //半径
    pSN_Radius->radius = 0x01;
    //mac层目标地址
    memcpy( sMACRxHead.dstAddr, sNwkRecvHeadInfo.srcAddr, LONG_ADDR_LEN );
  }
  
  sWLTx.sBuff.data[pos++] = LPM_RSSI_COLLECT_RESPOND; //命令标识
  u8 totalPages = 0;
  u8 nodeNum = 0;
  u8 WaterMeterNeiDataNums = FIFO_COUNT(&waterMeteNeiFifoCtr,MAX_LP_NEIGHBOURS+1);

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
  
  sWLTx.sBuff.data[pos++] = (totalPages << 4) + pageIndex;
  sWLTx.sBuff.data[pos++] = nodeNum;
  //获取邻居节点和RSSI值
  SgdNwkGetLowPowerMeterNeighbours( &sWLTx.sBuff.data[pos], &pos, nodeNum, pageIndex );
 
  sWLTx.sBuff.len = pos;
  
  sMACRxHead.sFCD.dstAddrMode = LONG_ADDR_MODE;
  sMACRxHead.sFCD.srcAddrMode = LONG_ADDR_MODE;
  //mac源地址
  memcpy( sMACRxHead.srcAddr, sNode.longAddr, LONG_ADDR_LEN );
  
  SgdMacFormDataFrame( &sWLTx.sBuff );

}


/***************************************
*输入:buf, len, nodeNum, pageIndex
*输出:buf, len
*返回:无
*描述:获取上报式水表数据
*作者:CJJ
***************************************/
void  SgdNwkGetWaterMeterData( u8 *buf, u8 *len, u8 nodeNum, u8 pageIndex )
{
  u8 pos = 0;
  u8 i = 0;
  u8 neighbours = pageIndex * WART_METER_PAGE_SIZE;
  u8 WaterMeterDataNums = FIFO_COUNT(&waterMeteFifoCtr,TOTAL_WART_METER_SIZE+1);
  sWaterMeterData *pTmp = NULL;
  u8 nodeLen = sizeof(sWaterMeterData)-4;
  
  //要取的节点数据大于总的阶段数据，错误返回
  if(neighbours > WaterMeterDataNums || nodeNum > WART_METER_PAGE_SIZE)return;
  //从被却节点处开始，取n个至发送缓冲区
  for(i = 0;i < nodeNum;i++)
  {
    pTmp = &waterMeterBuf[FIFO_POP_IDX(&waterMeteFifoCtr,TOTAL_WART_METER_SIZE+1,neighbours+i)];
    memcpy(buf+pos,(u8 *)pTmp,nodeLen);
    pos += nodeLen;
    
    if(neighbours+i > WaterMeterDataNums)
    {
      break;
    }
  }
  *len += pos;  
}


/***************************************
*输入:buf, len, nodeNum, pageIndex
*输出:buf, len
*返回:无
*描述:从邻居表中获取指定的邻居表地址以及RSSI值
*作者:张月保
***************************************/
void  SgdNwkGetLowPowerMeterNeighbours( u8 *buf, u8 *len, u8 nodeNum, u8 pageIndex )
{
  u8 pos = 0;
  u8 i = 0;
  u8 neighbours = pageIndex * ONE_PAGE_NEIGHBOURS;
  u8 WaterMeterNeiDataNums = FIFO_COUNT(&waterMeteNeiFifoCtr,MAX_LP_NEIGHBOURS+1);
  sLowPowerNeighbour *pTmp = NULL;
  u8 nodeLen = sizeof(sLowPowerNeighbour)-4;
  
  //要取的节点数据大于总的阶段数据，错误返回
  if(neighbours > WaterMeterNeiDataNums || nodeNum > ONE_PAGE_NEIGHBOURS)return;
  //从被却节点处开始，取n个至发送缓冲区
  for(i = 0;i < nodeNum;i++)
  {
    pTmp = &gLowPowerNeighbour[FIFO_POP_IDX(&waterMeteNeiFifoCtr,MAX_LP_NEIGHBOURS+1,neighbours+i)];
    memcpy(buf+pos,(u8 *)pTmp,nodeLen);
    pos += nodeLen;
    
    if(neighbours+i > WaterMeterNeiDataNums)
    {
      break;
    }
  }
  *len += pos;   
}

