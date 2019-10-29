//============================================================================
// Name        : Public.cpp
// Author      : 韩正玉
// Version     : v1.0
// Date 			 : 16-9-10
// Copyright   : 深国电
// Description : Public API
//============================================================================
#include "Public.h"
#include "si446x_cmd.h"
#include "delay.h"

/*************************************************************
*函数名称：INT8U CaculateChannelNo( INT8U CurrentChannel, INT8U Group )
*功能描述：计算信道编号
*输入参数：CurrentChannel,Group
*输出参数：无
*返 回 值：信道编号
*************************************************************/
INT8U CaculateChannelNo( INT8U CurrentChannel, INT8U Group )
{
  INT8U crtChannel = 0;
  if(Group >WLESS_PROTOCOL_GROUP_MAX || CurrentChannel >1)return 24;
  
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
  return crtChannel;
}
  
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

/***************************************
*输入:buf,len,interval
*输出:内存赋值的buf
*返回:无
*描述:将buf里的内存向后移interval个字节
*作者:张月保
***************************************/
void SgdMemMoveBack( sBuffer *pBuff, u8 interval )
{ 
  u16 len = pBuff->len;
  while( len-- != 0 )
  {
    pBuff->data[len+interval] = pBuff->data[len];
    pBuff->data[len] = 0x00;
  }
}
