#include <math.h>
#include "phy.h"
#include "delay.h"
#include "Rf_common.h"
#include "Public.h"
#include "si446x_cmd.h"

#define RSSI_MAX			188
#define RSSI_MIN			40
#define RSSI_SAMPLES	                20
#define STANDAR_NUM 	        0x01

/***************************************
*输入:无
*输出:无
*返回:返回RSSI的门限值
*描述:读取RSSI门限值
*作者:张月保
***************************************/
u8 SgdPhyGetRSSIThreshold(void)
{
  u8 value = 0;
  
  return value;
}

/***************************************
*输入:无
*输出:无
*返回:返回当前RSSI的值
*描述:读取当前RSSI值
*作者:张月保
***************************************/
u8 SgdPhyGetRSSIValue(void)
{
  INT8U rssi_value = 0; 
  si446x_get_modem_status(0);
  rssi_value = Si446xCmd.GET_MODEM_STATUS.CURR_RSSI;
  return rssi_value;
}

/***************************************
*输入:无
*输出:无
*返回:RSSI平均值
*描述:空闲信道评估
*作者:张月保
***************************************/
u8 SgdPhyGetAverRSSI(void)
{
  INT8U i = 0;
  INT8U sample = 0;
  INT8U min = 255;
  INT8U max = 0;
  INT16U sum = 0;
  INT8U value = 0;
  
  for( i = 0; i < RSSI_SAMPLES; i++ )
  {
    sample = SgdPhyGetRSSIValue();
    if( sample <= RSSI_MIN )
    {
      sample = RSSI_MIN;
    }
    if( sample >= RSSI_MAX )
    {
      sample = RSSI_MAX;
    }
    sum += sample;
    if( sample > max )
    {
      max = sample;
    }
    if( sample < min )
    {
      min = sample;
    }
    usDelay(100);
  }

  sum -= max;
  sum -= min;
  
  sum /= ( RSSI_SAMPLES - 2 );
  value = (INT8U)sum;
  return value;
}


/***************************************
*输入:无
*输出:无
*返回:无
*描述:获取指定信道的空中信号强度
*作者:张月保
***************************************/
u8  SgdGetInputPower(void)
{
  INT8U  rssi = 0;
  rssi = SgdPhyGetAverRSSI();
  INT8U  PowerValue = 0x40 + 70 - (rssi>>1); 
  if(PowerValue < 40) PowerValue = 40;
  if(PowerValue > 120) PowerValue = 120;
  return PowerValue;
}

/***************************************
*输入:buf,len
*输出:buf
*返回:是否有效帧;返回0是无效帧；返回1是有效帧。
*描述:解析物理层数据帧
*作者:张月保
***************************************/
bool SgdPhyParseFrame( sBuffer *pBuff )
{	
  if( pBuff == NULL )  return FALSE;
  
  sPhyFrameHead *pHead = (sPhyFrameHead *)pBuff->data;
  if( pHead->ChannelIndex != channelIndexArr[channelArrPos] ) return FALSE;
  if( pHead->StandardNum != STANDAR_NUM ) return FALSE;
  
  if( pHead->FrameHeadCRC != (( pHead->FrameLen ^ pHead->ChannelIndex ) ^ pHead->StandardNum) ) return FALSE;
  
  u16 crc1 = (pBuff->data[pHead->FrameLen+2] << 8) + pBuff->data[pHead->FrameLen+1];
  u16 crc2 = SgdPhyCalCrc( pBuff->data, pHead->FrameLen+1 );
  if( crc1 != crc2 ) return FALSE;

  gRandNumber = (u16)(gRandNumber + crc1);
  pBuff->len -= (sizeof(sPhyFrameHead)+2); 
  
  return TRUE;
}

/***************************************
*输入:buf,len
*输出:无
*返回:FALSE表示构造帧失败；TRUE表示构造帧成功
*描述:构造物理层数据帧
*作者:张月保
***************************************/
void SgdPhyFormFrame( u8 channelIndex )
{
  u8 pos = sizeof(sPhyFrameHead);
  if( (sWLTx.sBuff.len + pos) > RF_MAX_BUF_LEN )
  {
    memset( &sWLTx, 0x00, sizeof(sWLTx) );
    return;
  }
  
  SgdMemMoveBack( &sWLTx.sBuff, pos );

  sPhyFrameHead *pHead = (sPhyFrameHead *)sWLTx.sBuff.data;
  pHead->FrameLen = sWLTx.sBuff.len + 3;
  pHead->ChannelIndex = channelIndex;
  pHead->StandardNum = STANDAR_NUM;
  pHead->FrameHeadCRC = ( pHead->FrameLen ^ pHead->ChannelIndex ) ^ pHead->StandardNum; 
  sWLTx.sBuff.len += pos;
  gRandNumber = SgdPhyCalCrc( sWLTx.sBuff.data, sWLTx.sBuff.len );
  sWLTx.sBuff.data[sWLTx.sBuff.len++] = gRandNumber & 0xFF;
  sWLTx.sBuff.data[sWLTx.sBuff.len++] = (gRandNumber >> 8) & 0xFF; 

}

#if 0
/***************************************
*输入:buf,len
*输出:无
*返回:校验码crc
*描述:计算校验码CRC
*作者:张月保
***************************************/
u16 SgdPhyCalCrc( u8 *buf, u16 len )
{
  u16 crc = 0xffff;
  u8 i;
  while( len-- )
  {
    crc ^= *buf++;
    for( i = 0; i < 8; i++ )
    {
      if( crc & 0x0001 )
      {
        crc >>= 1;
        crc ^= CRC_POLY;	
      }
      else
      {
        crc >>= 1;
      }	
    }		
  }
  crc ^= 0xffff;
  return crc;
  
}
#endif

#if 0
/***************************************
*输入:CurrentChannel，Group
*输出:无
*返回:无
*描述:设置工作频点
*作者:张月保
***************************************/
void  SgdPhySetCurrentChannel( u8 CurrentChannel, u8 Group )
{
  u8 crtChannel = 0;
  
  if (0 == Group)
  {
    if (0 == CurrentChannel) crtChannel = 24;
    else crtChannel = 66;
  }
  else if (0 == CurrentChannel)
  {
    if (24 >= Group) crtChannel = Group - 1;
    else crtChannel = Group;
  }
  else
  {
    if (27 >= Group) crtChannel = Group + 38;
    else crtChannel = Group + 39;
  }
  
  SPI4432WriteRegister(FrequencyHoppingChannelSelect_79, crtChannel);
}
#endif

#if 0
/***************************************
*输入:data,WhitingPN9
*输出:白化后的data.
*返回:伪随机数发生器的寄存器的值
*描述:白化单字符
*作者:张月保
***************************************/
u16 SgdPhyWhitingData( u8 *data, u16 WhitingPN9 )
{
  u8 i, c;
  *data ^= ( WhitingPN9 & 0xFF );
  for(  i = 0 ; i < 8; i ++ )
  {
    c = WhitingPN9 & 0x21;
    WhitingPN9 >>= 1;
    if( ( c == 0x21 ) || ( c == 0x00 ) )
    {
      WhitingPN9 = WhitingPN9 & 0x0ff;
    }
    else
    {
      WhitingPN9 |= 0x100;	
    }
    
  }
  
  return WhitingPN9;
	
}
#endif


#if 0
/***************************************
*输入:buff,len
*输出:白化后的buf
*返回:无
*描述:白化字符串
*作者:张月保
***************************************/
void SgdPhyWhitingBuf( u8 *buff, u16 len  )
{
  u16 WhitingPN9 = 0x1FF;
  u16 i = 0;
  while( len-- )
  {
    WhitingPN9 = SgdPhyWhitingData( &buff[i++], WhitingPN9 );
  }
}
#endif
