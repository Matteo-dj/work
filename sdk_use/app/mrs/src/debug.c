#include "debug.h"
#include "task.h"
#include "Rf_common.h"
#include "phy.h"
#include "radio.h"
#include "Public.h"
#include "si446x_cmd.h"
#include "rf_main.h"


/*--------停止RF测试功能---------*/
void rfTestTimer(void)
{
  if( sRFTest.Status &  TESTING_TIME_OUT )
  {
    Shutdown_TxPN9_Code();
    
    vRadio_Init();
    
    //清空射频测试缓冲区
    memset( (char *)&sRFTest, 0x00, sizeof(sRFTest) );
    enableRfSendReceive();
  }    
}
/***************************************
*输入:无
*输出:无
*返回:无
*描述:使模块发射无调制的PN9码
*作者:zyb 2014.03.20
***************************************/
void  SgdRFTxPN9Code( u8 *pIn )
{ 
  memset( (char *)&sRFTest, 0x00, sizeof(sRFTest) );
  sRFTest.channelIndex = pIn[0];
  sRFTest.Timer = pIn[1] * 1000;

  u8 chnnlIndx = sRFTest.channelIndex;
  u8 chnnlNo = CaculateChannelNo(chnnlIndx&0x01,chnnlIndx>>1);

  disbaleRfSendReceive();
  vRadio_TxPN9_Code(chnnlNo);
  
  sRFTest.Status |= TESTING_TIMER_ON;
}

/***************************************
*输入:无
*输出:无
*返回:无
*描述:校准射频频点
*作者:zyb 2014.03.24
***************************************/
void  SgdRFCalibrateFrequency( u8 *pIn )
{
  memset((char *) &sRFTest, 0x00, sizeof(sRFTest) );
  sRFTest.Timer = pIn[3] * 1000;
    
  vAdjustFrequencyOffset(pIn[2],pIn[1]); 
  u8 chnnlIndx = pIn[0];
  u8 chnnlNo = CaculateChannelNo(chnnlIndx&0x01,chnnlIndx>>1);
  disbaleRfSendReceive();
  vRadio_TxPN9_Code(chnnlNo);
  sRFTest.Status |= TESTING_TIMER_ON;
}

/***************************************
*输入:无
*输出:无
*返回:无
*描述:功率校正
*作者:CJJ 2017.02.3
***************************************/
void  SgdRFCalibrateTXpower( u8 *pIn )
{
  memset((char *) &sRFTest, 0x00, sizeof(sRFTest) );
  sRFTest.Timer = pIn[2] * 1000;
  si446x_set_property(0x22,0x01,0x01,pIn[1]);

  u8 chnnlIndx = pIn[0];
  u8 chnnlNo = CaculateChannelNo(chnnlIndx&0x01,chnnlIndx>>1);
  disbaleRfSendReceive();
  vRadio_TxPN9_Code(chnnlNo);
  sRFTest.Status |= TESTING_TIMER_ON;
}
/***************************************
*输入:无
*输出:无
*返回:无
*描述:功率频率校正
*作者:CJJ 2017.02.3
***************************************/
void SgdRFCalibrateFrequencyTXpower( u8 *pIn )
{
  memset((char *) &sRFTest, 0x00, sizeof(sRFTest) );
  sRFTest.Timer = pIn[5] * 1000;
  si446x_set_property(0x22,0x01,0x01,pIn[3]);
  vAdjustFrequencyOffset(pIn[2],pIn[1]);

  u8 chnnlIndx = pIn[0];
  u8 chnnlNo = CaculateChannelNo(chnnlIndx&0x01,chnnlIndx>>1);
  disbaleRfSendReceive();
  vRadio_TxPN9_Code(chnnlNo);
  sRFTest.Status |= TESTING_TIMER_ON;
}

void vReconfigRFconfInfo(void)
{
  si446x_set_property(0x10,0x05,0,0x00,0x14,0x00,0x0f,0x11);

  si446x_set_property(0x12,0x02,0x0d,0x1f,0xff);
  si446x_set_property(0x12,0x02,0x11,0x1f,0xff);
  si446x_set_property(0x12,0x02,0x15,0x1f,0xff);
  si446x_set_property(0x12,0x02,0x19,0x1f,0xff);
  si446x_set_property(0x12,0x02,0x1d,0x1f,0xff);
}

/***************************************
*输入:无
*输出:无
*返回:无
*描述:射频发射性能测试
*作者:zyb 2014.03.24
***************************************/
void  SgdRFTxTest(void)
{
  gmCodeTest.mTestStartFlag = TRUE;
  gmCodeTest.mTestLen = (sRFTest.Timer * 10) / 8 + 1;
  if(gmCodeTest.mTestLen > RF_TX_TEST_MAX_LEN)
  {
    gmCodeTest.mTestLen = RF_TX_TEST_MAX_LEN;
  }
  
  vReconfigRFconfInfo();
  memset((char *)&sWLTx,0x00,sizeof(sWLTx));
  switch( sRFTest.CmdMode )
  {
    case CMD_MODE_0:
      memset( sWLTx.sBuff.data, 0x00, 64 );
      break;
    case CMD_MODE_1:
      memset( sWLTx.sBuff.data, 0xFF, 64 );
      break;    
    case CMD_MODE_4:
      memset( sWLTx.sBuff.data, 0x55, 64 );
      si446x_set_property(0x20,0x07,0x00,0x02,0x00,0x07,0x01,0x86,0xA0,0x01);
      break;
    default:
      memset( sWLTx.sBuff.data, 0x55, 64 );
      break;   
  } 

  if(0x01 == Flash_Readbyte(TX_POWER_mCODE_FLAG))
  {
    si446x_set_property(0x22,0x01,0x01,Flash_Readbyte(TX_POWER_mCODE_VALUE));//16
  }
  else
  {
    si446x_set_property(0x22,0x01,0x01,34);//16
  }

  u8 channel = CaculateChannelNo(sRFTest.channelIndex&0x01,sRFTest.channelIndex>>1);  
  sWLTx.sBuff.len = gmCodeTest.mTestLen;
  gmCodeTest.mTestLen -= 64;
  disbaleRfSendReceive();
  rfWLTx(channel);
}


void  SgdRFTxTestSend(void)
{             
  si446x_get_int_status(0u, 0u, 0u);
  
  if (0 == gmCodeTest.mTestLen || Si446xCmd.GET_INT_STATUS.PH_PEND & SI446X_CMD_GET_INT_STATUS_REP_PH_PEND_PACKET_SENT_PEND_BIT)
  {
     si446x_change_state(SI446X_CMD_CHANGE_STATE_ARG_NEXT_STATE1_NEW_STATE_ENUM_READY);
     si446x_fifo_info(SI446X_CMD_FIFO_INFO_ARG_FIFO_TX_BIT);
     memset((char *)&gmCodeTest,0x00,sizeof(gmCodeTest));
     enableRfSendReceive();
     vRadio_Init(); 
  }
  
  if ( (gmCodeTest.mTestLen > 0) && (Si446xCmd.GET_INT_STATUS.PH_PEND & SI446X_CMD_GET_INT_STATUS_REP_PH_PEND_TX_FIFO_ALMOST_EMPTY_PEND_BIT))
  {
    u8 wLen = 0;
    if ( gmCodeTest.mTestLen  > ALMOST_FULL_RX )
    {
      wLen = ALMOST_FULL_RX;   
    }
    else
    {
      wLen = (u8)gmCodeTest.mTestLen;
    }
    gmCodeTest.mTestLen -= wLen;
    si446x_write_tx_fifo( wLen, sWLTx.sBuff.data);
  }
}
