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

//MAC�����֡֡ͷ��Ϣ����ṹ��
extern sMAC_FrameHead_6 sMACRxHead;

//�����֡ͷ��Ϣ����ṹ��
sNwkRecvFrmaeHeadInfo sNwkRecvHeadInfo;

//�����֡���
u8   nwkFrameNumber;
/***************************************
*����:��
*���:��
*����:��
*����:��ʼ��һЩĬ�ϲ���
*����:���±�
***************************************/
void SgdNwkInit(void)
{
  memset( &sNwkRecvHeadInfo, 0x00, sizeof(sNwkRecvHeadInfo) );
  nwkFrameNumber = 0x01;
}

/***************************************
*����:��
*���:buf,len
*����:�� 
*����:��֯������������֡
*����:���±�
***************************************/
void SgdNwkJoinNetWorkRequest(void)
{}

/***************************************
*����:buf,len
*���:��
*����:�� 
*����:����MAC��������֡
*����:���±�
***************************************/
void SgdNwkParseFrame( u8 *buf, u16 len )
{ 
  if( ( buf == NULL ) || ( len == 0 ) ) return;
  u8 nwkBroadAddr[6] = {0};
  memset( nwkBroadAddr, 0xFF, sizeof(nwkBroadAddr) );
  u8 nwkLocalAddr[6] = {0};
  
  memset(&sNwkRecvHeadInfo, 0x00, sizeof(sNwkRecvHeadInfo));
  
  sNwkFCD *pFCD = (sNwkFCD *)buf;
  //��������֡���Ͳ���ȷ���򷵻�
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
    //����֡��Ŀ���ַ�ǵ�ǰģ���ַ�������ǹ㲥֡
    if( sNwkRecvHeadInfo.sFCD.routeInfo )
    {
      //��·����
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
      //�뾶����1����·�ɣ����м���������0����ת����֡
      len -= sNwkRecvHeadInfo.headLen;
      SgdNwkFormRelayFrame( buf+sNwkRecvHeadInfo.headLen, len );
    }      
  }
}

/***************************************
*����:buf,len
*���:��
*����:�� 
*����:����NWK��֡ͷ����
*����:���±�
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
    //��·����
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
*����:buf,len
*���:��
*����:�� 
*����:�������������֡
*����:���±�
***************************************/
void  SgdNwkParseDataFrame( u8 *buf, u16 len )
{
  SgdAppParseFrame( buf, len );
}

/***************************************
*����:pageIndex,ҳ���
*���:��
*����:�� 
*����:��������ˮ���ϱ�Ӧ��֡
*����:CJJ
***************************************/
void SgdNwkFormWaterDataRespond( u8 pageIndex )
{
  u8 pos = 0;
  memset( &sWLTx, 0x00, sizeof(sWLTx) );
  sNwkFCD *pFCD = (sNwkFCD *)sWLTx.sBuff.data;
  *pFCD = sNwkRecvHeadInfo.sFCD;
  pos += sizeof(sNwkFCD);
  //����֡��Դ��ַ��Ϊ��Ӧ֡��Ŀ���ַ
  memcpy( &sWLTx.sBuff.data[pos], sNwkRecvHeadInfo.srcAddr, sNwkRecvHeadInfo.srcAddrLen );
  pos += sNwkRecvHeadInfo.srcAddrLen;
  //����֡��Ŀ���ַ��Ϊ��Ӧ֡��Դ��ַ
  memcpy( &sWLTx.sBuff.data[pos], sNwkRecvHeadInfo.dstAddr, sNwkRecvHeadInfo.dstAddrLen );
  pos += sNwkRecvHeadInfo.dstAddrLen;
  sNwk_FrameNum_Radius *pSN_Radius = (sNwk_FrameNum_Radius *)&sWLTx.sBuff.data[pos];
  *pSN_Radius = sNwkRecvHeadInfo.sFrameNum_Radius;
  pos += sizeof(sNwk_FrameNum_Radius);
  if( sNwkRecvHeadInfo.sFCD.routeInfo )
  {
    //��·����
    if( sNwkRecvHeadInfo.sRouteInfo.relayCount > MAX_RELAY_NUMS ) return;
    pSN_Radius->radius = sNwkRecvHeadInfo.sRouteInfo.relayCount + 1;
    sNwkRouteInfo *pRoute = (sNwkRouteInfo *)&sWLTx.sBuff.data[pos];
    *pRoute = sNwkRecvHeadInfo.sRouteInfo;
    pRoute->relayIndexL3bit = pRoute->relayCount & 0x07;
    pRoute->relayIndexH2bit = pRoute->relayCount >> 3;
    pos += sizeof(sNwkRouteInfo);
    //�����м̵�ַ�б�
    memcpy( &sWLTx.sBuff.data[pos], sNwkRecvHeadInfo.relayAddr, pRoute->relayCount *LONG_ADDR_LEN );
    pos += (pRoute->relayCount *LONG_ADDR_LEN);
    //mac��Ŀ���ַ
    memcpy( sMACRxHead.dstAddr, &sNwkRecvHeadInfo.relayAddr[(pRoute->relayCount-1)*LONG_ADDR_LEN], LONG_ADDR_LEN );
  }
  else
  {
    //û��·����
    //�뾶
    pSN_Radius->radius = 0x01;
    //mac��Ŀ���ַ
    memcpy( sMACRxHead.dstAddr, sNwkRecvHeadInfo.srcAddr, LONG_ADDR_LEN );
  }
  
  sWLTx.sBuff.data[pos++] = COLLECT_WATER_DATA_RESPOND; //�����ʶ
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
    //���ҳ��Ŵ��ڵ�����ҳ����˵����������ȷ������Ӧ
    if( pageIndex >= totalPages )
      pageIndex = 0;//return;
    nodeNum = ( pageIndex == (totalPages - 1) ) ? \
              WaterMeterDataNums -(pageIndex * WART_METER_PAGE_SIZE) : WART_METER_PAGE_SIZE;   
  }
  
  sWLTx.sBuff.data[pos++] = (totalPages << 4) + pageIndex;
  sWLTx.sBuff.data[pos++] = nodeNum;

  //��ȡ�ϱ�ʽˮ������
  SgdNwkGetWaterMeterData( &sWLTx.sBuff.data[pos], &pos, nodeNum, pageIndex );
 
  sWLTx.sBuff.len = pos;
  
  sMACRxHead.sFCD.dstAddrMode = LONG_ADDR_MODE;
  sMACRxHead.sFCD.srcAddrMode = LONG_ADDR_MODE;
  //macԴ��ַ
  memcpy( sMACRxHead.srcAddr, sNode.longAddr, LONG_ADDR_LEN );
  
  SgdMacFormDataFrame( &sWLTx.sBuff );  
}


/***************************************
*����:buf,len
*���:��
*����:�� 
*����:�������������֡
*����:���±�
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
*����:��
*���:��
*����:�� 
*����:�������������������Ӧ֡
*����:���±�
***************************************/
void SgdNwkFormJoinNetRespondFrame(void)
{}
  
void joinNetResond(joinNetResondCtr_st *pTmp)
{}

/***************************************
*����:buf, paths
*���:buf, len 
*����:�� 
*����:������·����Ϣ�л�ȡһ�����ŵ�·��
*����:���±�
***************************************/
void  SgdNwkGetBestPath( u8 *buf, u8 *len )
{}


/***************************************
*����:buf
*���:��
*����:�� 
*����:��������������Ӧ֡�������Ϣ
*����:���±�
***************************************/
void SgdNwkCollectJoinNetRespondInfo( u8 *buf, u16 len )
{}

/***************************************
*����:void
*���:��
*����:�� 
*����:��������ڵ����֡
*����:���±�
***************************************/
void  SgdNwkFormReadyFrame(void)
{}


/***************************************
*����:pInfo
*���:��
*����:�� 
*����:����û��·����Ϣ������ڵ����֡
*����:���±�
***************************************/
void  SgdNwkFormReadyFrameNoRoute( sNwkJoinNetRespondInfo *pInfo )
{}

/***************************************
*����:pInfo
*���:��
*����:�� 
*����:������·����Ϣ������ڵ����֡
*����:���±�
***************************************/
void SgdNwkFormReadyFrameRoute( sNwkJoinNetRespondInfo *pInfo  )
{}

/***************************************
*����:buf,len
*���:��
*����:�� 
*����:��������㳡ǿ�ռ�Ӧ��֡
*����:���±�
***************************************/
void SgdNwkFormRSSIRespond( u8 pageIndex )
{}

/***************************************
*����:��
*���:��
*����:�� 
*����:��������������ӽڵ�Ӧ��֡,����·��
*����:���±�
***************************************/
void SgdNwkConfigNodeRespond(void)
{}


/***************************************
*����:buf
*���:��
*����:�� 
*����:ת������֡
*����:���±�
***************************************/
void SgdNwkFormRelayFrame( u8 *buf, u16 len )
{
  //ת������֡��ֻ���޸İ뾶���Լ��м�����
  u8 pos = 0;
  memset( &sWLTx, 0x00, sizeof(sWLTx) );
  memcpy( sWLTx.sBuff.data, &sNwkRecvHeadInfo.sFCD, sizeof(sNwkRecvHeadInfo.sFCD) );
  pos = sizeof(sNwkRecvHeadInfo.sFCD);
  memcpy( &sWLTx.sBuff.data[pos], sNwkRecvHeadInfo.dstAddr, sNwkRecvHeadInfo.dstAddrLen );
  pos += sNwkRecvHeadInfo.dstAddrLen;
  memcpy( &sWLTx.sBuff.data[pos], sNwkRecvHeadInfo.srcAddr, sNwkRecvHeadInfo.srcAddrLen );
  pos += sNwkRecvHeadInfo.srcAddrLen; 
  sNwkRecvHeadInfo.sFrameNum_Radius.radius -= 1;
  
//CJJ-2017-9-22���м�תʱ֡��Ų���    
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
  
  //mac��Ŀ�˵�ַ
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
  //mac��Դ��ַ
  u8 macSrcAddrPos = relayIndex * relayAddrLen;
  sMACRxHead.sFCD.srcAddrMode = sNwkRecvHeadInfo.sRouteInfo.relay1AddrMode;
  memcpy( sMACRxHead.srcAddr, &sNwkRecvHeadInfo.relayAddr[macSrcAddrPos], relayAddrLen );  
  
  SgdMacFormDataFrame( &sWLTx.sBuff );  
}

/***************************************
*����:buf
*���:��
*����:�� 
*����:�����ӽڵ�������Ϣ
*����:���±�
***************************************/
void SgdNwkSaveNodeInfo( u8 *buf, u16 len )
{}


/***************************************
*����:��
*���:��
*����:�� 
*����:ɾ���ռ�������Ӧ֡��Ϣ������
*����:���±�
***************************************/
void SgdNwkFreeInNetInfoLK(void)
{}

/***************************************
*����:��
*���:��
*����:�� 
*����:ɾ�����������е��ھӱ�
*����:���±�
***************************************/
void SgdNwkFreeNeighbourList(void)
{}

/***************************************
*����:buf,len
*���:��
*����:��
*����:��֯���������֡
*����:���±�
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
  //����֡��Դ��ַ��Ϊ��Ӧ֡��Ŀ���ַ
  memcpy( &sWLTx.sBuff.data[pos], sNwkRecvHeadInfo.srcAddr, sNwkRecvHeadInfo.srcAddrLen );
  pos += sNwkRecvHeadInfo.srcAddrLen;
  //����֡��Ŀ���ַ��Ϊ��Ӧ֡��Դ��ַ
  memcpy( &sWLTx.sBuff.data[pos], sNwkRecvHeadInfo.dstAddr, sNwkRecvHeadInfo.dstAddrLen );
  pos += sNwkRecvHeadInfo.dstAddrLen;
  //֡��źͰ뾶
  sNwk_FrameNum_Radius *pSN_Radius = (sNwk_FrameNum_Radius *)&sWLTx.sBuff.data[pos];
  *pSN_Radius = sNwkRecvHeadInfo.sFrameNum_Radius;
  pos += sizeof(sNwk_FrameNum_Radius);
  if( sNwkRecvHeadInfo.sFCD.routeInfo )
  {
    //��·����
    //�뾶
    u8  relayAddrLen = 0;
    u8  relayTotalLen = 0;
    if( sNwkRecvHeadInfo.sRouteInfo.relayCount > MAX_RELAY_NUMS ) return;
    pSN_Radius->radius = sNwkRecvHeadInfo.sRouteInfo.relayCount + 1;
    sNwkRouteInfo *pRoute = (sNwkRouteInfo *)&sWLTx.sBuff.data[pos];
    *pRoute = sNwkRecvHeadInfo.sRouteInfo;
    pRoute->relayIndexL3bit = pRoute->relayCount & 0x07;
    pRoute->relayIndexH2bit = pRoute->relayCount >> 3;
    pos += sizeof(sNwkRouteInfo);
    //�����м̵�ַ�б�
    if( sNwkRecvHeadInfo.sRouteInfo.relay1AddrMode == LONG_ADDR_MODE )
      relayAddrLen = LONG_ADDR_LEN;
    else
      relayAddrLen = SHORT_ADDR_LEN;
    relayTotalLen = pRoute->relayCount *relayAddrLen;
    
    memcpy( &sWLTx.sBuff.data[pos], sNwkRecvHeadInfo.relayAddr, relayTotalLen );
    pos += relayTotalLen;
    //mac��Ŀ���ַ
    sMACRxHead.sFCD.dstAddrMode = sNwkRecvHeadInfo.sRouteInfo.relay1AddrMode;
    memcpy( sMACRxHead.dstAddr, &sNwkRecvHeadInfo.relayAddr[(pRoute->relayCount-1)*relayAddrLen], relayAddrLen );
  }
  else
  {
    //û��·����
    //�뾶
    pSN_Radius->radius = 0x01;
    //mac��Ŀ���ַ
    sMACRxHead.sFCD.dstAddrMode = sNwkRecvHeadInfo.sFCD.srcAddrMode;
    memcpy( sMACRxHead.dstAddr, sNwkRecvHeadInfo.srcAddr, sNwkRecvHeadInfo.srcAddrLen );
  }

  pBuffer->len += pos;
  
  sMACRxHead.sFCD.srcAddrMode = pFCD->srcAddrMode;
  memcpy( sMACRxHead.srcAddr, sNwkRecvHeadInfo.dstAddr, sNwkRecvHeadInfo.dstAddrLen );
  
  SgdMacFormDataFrame( pBuffer );

}

/***************************************
*����:��
*���:��
*����:��
*����:����·�ɴ�������֡
*����:���±�
***************************************/
void  SgdNwkFormRouteErrorFrame(void)
{ 
}

/***************************************
*����:��
*���:��
*����:��
*����:���������֡���
*����:���±�
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
*����:relayAddr,relayCnt,addrLen
*���:relayAddr
*����:��
*����:���м̵�ַ��ĵ�ַ����
*����:���±�
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
*����:buf, len, nodeNum, pageIndex
*���:buf, len
*����:��
*����:���ھӱ��л�ȡָ�����ھӱ��ַ�Լ�RSSIֵ
*����:���±�
***************************************/
void  SgdNwkGetNeighbours( u8 *buf, u8 *len, u8 nodeNum, u8 pageIndex )
{}

/***************************************
*����:pRoute
*���:pRoute
*����:��
*����:�����м̵�ַģʽ
*����:���±�
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
*����:pageIndex
*���:��
*����:�� 
*����:���������ˮ��ǿ�ռ�Ӧ��֡
*����:���±�
***************************************/
void SgdNwkFormLowPowerMeterRSSIRespond( u8 pageIndex )
{ 
  //��ǿ�ռ�����֡��Ŀ���ַ��Դ��ַ����Ϊ����ַ
  if( (sNwkRecvHeadInfo.dstAddrLen != sNwkRecvHeadInfo.srcAddrLen) && (sNwkRecvHeadInfo.dstAddrLen != LONG_ADDR_LEN) ) return;
  u8 pos = 0;
  memset( &sWLTx, 0x00, sizeof(sWLTx) );
  sNwkFCD *pFCD = (sNwkFCD *)sWLTx.sBuff.data;
  *pFCD = sNwkRecvHeadInfo.sFCD;
  pos += sizeof(sNwkFCD);
  //����֡��Դ��ַ��Ϊ��Ӧ֡��Ŀ���ַ
  memcpy( &sWLTx.sBuff.data[pos], sNwkRecvHeadInfo.srcAddr, sNwkRecvHeadInfo.srcAddrLen );
  pos += sNwkRecvHeadInfo.srcAddrLen;
  //����֡��Ŀ���ַ��Ϊ��Ӧ֡��Դ��ַ
  memcpy( &sWLTx.sBuff.data[pos], sNwkRecvHeadInfo.dstAddr, sNwkRecvHeadInfo.dstAddrLen );
  pos += sNwkRecvHeadInfo.dstAddrLen;
  sNwk_FrameNum_Radius *pSN_Radius = (sNwk_FrameNum_Radius *)&sWLTx.sBuff.data[pos];
  *pSN_Radius = sNwkRecvHeadInfo.sFrameNum_Radius;
  pos += sizeof(sNwk_FrameNum_Radius);
  if( sNwkRecvHeadInfo.sFCD.routeInfo )
  {
    //��·����
    //�м̵ĵ�ַģʽҲ����Ϊ����ַģʽ
    if( sNwkRecvHeadInfo.sRouteInfo.relay1AddrMode != LONG_ADDR_MODE ) return;
    //�뾶
    if( sNwkRecvHeadInfo.sRouteInfo.relayCount > MAX_RELAY_NUMS ) return;
    pSN_Radius->radius = sNwkRecvHeadInfo.sRouteInfo.relayCount + 1;
    sNwkRouteInfo *pRoute = (sNwkRouteInfo *)&sWLTx.sBuff.data[pos];
    *pRoute = sNwkRecvHeadInfo.sRouteInfo;
    pRoute->relayIndexL3bit = pRoute->relayCount & 0x07;
    pRoute->relayIndexH2bit = pRoute->relayCount >> 3;
    pos += sizeof(sNwkRouteInfo);
    //�����м̵�ַ�б�
    memcpy( &sWLTx.sBuff.data[pos], sNwkRecvHeadInfo.relayAddr, pRoute->relayCount *LONG_ADDR_LEN );
    pos += (pRoute->relayCount *LONG_ADDR_LEN);
    //mac��Ŀ���ַ
    memcpy( sMACRxHead.dstAddr, &sNwkRecvHeadInfo.relayAddr[(pRoute->relayCount-1)*LONG_ADDR_LEN], LONG_ADDR_LEN );
  }
  else
  {
    //û��·����
    //�뾶
    pSN_Radius->radius = 0x01;
    //mac��Ŀ���ַ
    memcpy( sMACRxHead.dstAddr, sNwkRecvHeadInfo.srcAddr, LONG_ADDR_LEN );
  }
  
  sWLTx.sBuff.data[pos++] = LPM_RSSI_COLLECT_RESPOND; //�����ʶ
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
    //���ҳ��Ŵ��ڵ�����ҳ����˵����������ȷ������Ӧ
    if( pageIndex >= totalPages )
      pageIndex = 0;//return;
    nodeNum = ( pageIndex == (totalPages - 1) ) ? WaterMeterNeiDataNums -(pageIndex << 4) : ONE_PAGE_NEIGHBOURS;   
  }
  
  sWLTx.sBuff.data[pos++] = (totalPages << 4) + pageIndex;
  sWLTx.sBuff.data[pos++] = nodeNum;
  //��ȡ�ھӽڵ��RSSIֵ
  SgdNwkGetLowPowerMeterNeighbours( &sWLTx.sBuff.data[pos], &pos, nodeNum, pageIndex );
 
  sWLTx.sBuff.len = pos;
  
  sMACRxHead.sFCD.dstAddrMode = LONG_ADDR_MODE;
  sMACRxHead.sFCD.srcAddrMode = LONG_ADDR_MODE;
  //macԴ��ַ
  memcpy( sMACRxHead.srcAddr, sNode.longAddr, LONG_ADDR_LEN );
  
  SgdMacFormDataFrame( &sWLTx.sBuff );

}


/***************************************
*����:buf, len, nodeNum, pageIndex
*���:buf, len
*����:��
*����:��ȡ�ϱ�ʽˮ������
*����:CJJ
***************************************/
void  SgdNwkGetWaterMeterData( u8 *buf, u8 *len, u8 nodeNum, u8 pageIndex )
{
  u8 pos = 0;
  u8 i = 0;
  u8 neighbours = pageIndex * WART_METER_PAGE_SIZE;
  u8 WaterMeterDataNums = FIFO_COUNT(&waterMeteFifoCtr,TOTAL_WART_METER_SIZE+1);
  sWaterMeterData *pTmp = NULL;
  u8 nodeLen = sizeof(sWaterMeterData)-4;
  
  //Ҫȡ�Ľڵ����ݴ����ܵĽ׶����ݣ����󷵻�
  if(neighbours > WaterMeterDataNums || nodeNum > WART_METER_PAGE_SIZE)return;
  //�ӱ�ȴ�ڵ㴦��ʼ��ȡn�������ͻ�����
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
*����:buf, len, nodeNum, pageIndex
*���:buf, len
*����:��
*����:���ھӱ��л�ȡָ�����ھӱ��ַ�Լ�RSSIֵ
*����:���±�
***************************************/
void  SgdNwkGetLowPowerMeterNeighbours( u8 *buf, u8 *len, u8 nodeNum, u8 pageIndex )
{
  u8 pos = 0;
  u8 i = 0;
  u8 neighbours = pageIndex * ONE_PAGE_NEIGHBOURS;
  u8 WaterMeterNeiDataNums = FIFO_COUNT(&waterMeteNeiFifoCtr,MAX_LP_NEIGHBOURS+1);
  sLowPowerNeighbour *pTmp = NULL;
  u8 nodeLen = sizeof(sLowPowerNeighbour)-4;
  
  //Ҫȡ�Ľڵ����ݴ����ܵĽ׶����ݣ����󷵻�
  if(neighbours > WaterMeterNeiDataNums || nodeNum > ONE_PAGE_NEIGHBOURS)return;
  //�ӱ�ȴ�ڵ㴦��ʼ��ȡn�������ͻ�����
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

