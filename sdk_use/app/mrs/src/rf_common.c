#include  "rf_common.h"
#include "Public.h"
#include "radio.h"
#include "phy.h"



sNodeParameters sNode;

volatile sRFChannelListen sRFChannel;

volatile sUsartRxTx sUsart2;

sWireLessTx sWLTx;

sWireLessRx sWLRx;

volatile sRFTesting sRFTest;
volatile mCodeTest_st gmCodeTest;

//RSSI门限值定时更新计时
volatile sUpdateRssiThreshold sUpRssiThreshold;
u8  channelRssiThresholdArr[4];
volatile u8  channelIndexArr[4];
volatile u8  channelArrPos;
volatile u8  recvChannelArrPos;
volatile sRF_Reboot sRFboot;
u16 gRandNumber;
u8  LPNeighbourNums;
plcPowerDown_st powerDown;
warteBoard_st gwarteBoard;
u8 SensitivityTestCNT = 0;
static u8  eepDat[EEP_END_ADDR-EEP_START_ADDR];

/*----------flash参数初始化-----------*/
void FlashInit(void)
{
  //u16  i = 0;
  u16 crc1 = 0;
  u16 crc2 = 0;
  u16 crc = 0;

  u32 ulRet = HI_MDM_NV_Read(HI_NV_FTM_RESERVED3_ID, eepDat, sizeof(eepDat));
  
  if (HI_ERR_SUCCESS == ulRet)
  {
    crc1 = SgdPhyCalCrc( eepDat, sizeof(eepDat)-2);
    crc2 = (eepDat[NWE_CHIP_FLAH_HIGH] << 8) | eepDat[NWE_CHIP_FLAH_LOW];

    if(crc1 != crc2)
    {
      memset(eepDat,0x00,sizeof(eepDat));
      crc = SgdPhyCalCrc( eepDat, sizeof(eepDat)-2);
      eepDat[NWE_CHIP_FLAH_LOW] = crc & 0XFF;
      eepDat[NWE_CHIP_FLAH_HIGH] = crc >> 8;
      HI_MDM_NV_Write(HI_NV_FTM_RESERVED3_ID, eepDat, sizeof(eepDat));
    }
  }
}

/***************************************
*输入:无
*输出:无
*返回:无
*描述:初始化节点公共参数
*作者:张月保
***************************************/
void CommonParamsInit(void)
{
  memset( &sNode, 0x00, sizeof(sNode) );
  memset( (char *)&sUsart2, 0x00, sizeof(sUsart2) );
  memset( &sWLTx, 0x00, sizeof(sWLTx) );
  memset( &sWLRx, 0x00, sizeof(sWLRx) );
  memset( (char *)&sRFTest, 0x00, sizeof(sRFTest) );
  memset( (char *)&sUpRssiThreshold, 0x00, sizeof(sUpRssiThreshold) );
  memset( channelRssiThresholdArr, 0x00, sizeof(channelRssiThresholdArr) );
  channelArrPos = 4;
  recvChannelArrPos = 0;
  
  memset( (char *)&sRFboot, 0x00, sizeof(sRFboot) );
  gRandNumber = 0;
  LPNeighbourNums = 0; 

  memset((char *)&powerDown,0x00,sizeof(powerDown));
  FlashInit();
}

/***************************************
*输入:addr
*输出:无
*返回:addr地址所对应flash的偏移地址
*描述:读取flash的值，半字读取
*作者:曹加加
***************************************/
u8 Flash_Readbyte(u16 addr)
{
  u8  retDat = 0;
  u8  dat[EEP_END_ADDR-EEP_START_ADDR] = {0};
  u32 ulRet = HI_MDM_NV_Read(HI_NV_FTM_RESERVED3_ID, dat, sizeof(dat));
  
  if (HI_ERR_SUCCESS == ulRet)
  {
    u16 crc1 = SgdPhyCalCrc( dat, sizeof(dat)-2);
    u16 crc2 = (dat[NWE_CHIP_FLAH_HIGH] << 8) | dat[NWE_CHIP_FLAH_LOW];

    if(crc1 == crc2)
    {
      retDat = dat[addr];
    }
    else
    {
      retDat = eepDat[addr];
    }
  }
  else
  {
    retDat = eepDat[addr];
  }

  return retDat;
}

/***************************************
*输入:addr，value
*输出:无
*返回:无
*描述:写flash
*作者:曹加加
***************************************/
void Flash_Writebyte(u16 addr,u8 value,bool flag)
{
  eepDat[addr] = value;
  
  if(TRUE == flag)
  {
    u16 crc = SgdPhyCalCrc( eepDat, sizeof(eepDat)-2);
    
    eepDat[NWE_CHIP_FLAH_LOW] = crc & 0XFF;
    eepDat[NWE_CHIP_FLAH_HIGH] = crc >> 8;

    HI_MDM_NV_Write(HI_NV_FTM_RESERVED3_ID, eepDat, sizeof(eepDat));
  }
}

void datFromFlashToRam(void)
{
  u8  dat[EEP_END_ADDR-EEP_START_ADDR] = {0};
  u32 ulRet = HI_MDM_NV_Read(HI_NV_FTM_RESERVED3_ID, dat, sizeof(dat));
    
  if (HI_ERR_SUCCESS == ulRet)
  {
    u16 crc1 = SgdPhyCalCrc( dat, EEP_END_ADDR-EEP_START_ADDR-2);
    u16 crc2 = (dat[NWE_CHIP_FLAH_HIGH] << 8) | dat[NWE_CHIP_FLAH_LOW];

    if(crc1 == crc2)
    {
      memcpy(eepDat,dat,sizeof(dat));
    }
  }
}

/***************************************
*输入:无
*输出:无
*返回:无
*描述:计算当前环境的RSSI门限值
*作者:张月保
***************************************/
void SgdCalcRssiThresholdValue(void)
{
  u8 minPower = 0; 
  u8 i = 0;
  
  for( i = 0; i < sizeof(channelIndexArr); i++ )
  {
    RF_RxOn(FALSE, channelIndexArr[i]);
    
    minPower = SgdGetMinInputPower();
    channelRssiThresholdArr[i] = minPower - 10;
  }
}

/***************************************
*输入:无
*输出:无
*返回:无
*描述:计算当前环境的RSSI值
*作者:张月保
***************************************/
u8 SgdGetMinInputPower(void)
{
  u8 i, currInputPower,minInputPower;
  minInputPower = 0;
  for( i = 0; i < 10; i++ )
  {
    currInputPower = SgdGetInputPower();
    if( currInputPower < 90 ) currInputPower = 90;
    if( currInputPower >= minInputPower )
      minInputPower = currInputPower;
  }
  return minInputPower;
}

void  SgdInitChannelIndexArr(void)
{
  memset( channelIndexArr, 0x00, sizeof(channelIndexArr) ); 
  channelIndexArr[0] = 0x00;
  channelIndexArr[1] = 0x01; 
  channelIndexArr[2] = (sNode.channelGroupNum << 1) + 0x00;
  channelIndexArr[3] = (sNode.channelGroupNum << 1) + 0x01; 
}

/***************************************
*输入:buf, len
*输出:无
*返回:sum数据buf的累加和
*描述:计算累加和
*作者:张月保
***************************************/
u8 SgdGetSum( u8 *buf, u8 len )
{
  u16 sum = 0x00;
  u16 i;
  for( i = 0; i < len; i ++)
  {
    sum += *( buf + i );
  }
  return (sum & 0xFF);
}
