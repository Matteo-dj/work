#include "rf_main.h"
#include "radio.h"
#include "Public.h"
#include "bsp.h"
#include  "LowPowerMeter.h"


static volatile u8 rfStatus;
static volatile u16 rfRxLen;
static volatile u16 rfTxLen;
static HI_TIMER_HANDLE_S scanRfHandel;
static HI_TIMER_HANDLE_S rfTaskTimerHandel;

/*------�������ݷ���-----------------*/
void  rfWLTx(u8 channel)
{
  rfStatus = 4;
  //CJJ-2018-9-3:ʹ�ö�ʱ���ж�ģʽ�����ö�ʱ����Ϣģʽ
  #ifdef MSG_NOT_INT
  MRS_StopTimer(MRS_STA_TIMER_RF_SCAN_CHANNLE);
  #else
  HI_MDM_TIMER_Stop(&scanRfHandel);
  #endif

  u32 txMaxTime = sWLTx.sBuff.len;
  if(txMaxTime <= RF_MAX_BUF_LEN)
  {
    txMaxTime = WAIT_TX_DAT_MAX_TIME;
  }
  else
  {
    txMaxTime = 100 + (txMaxTime * 8) / 10;
  }
  MRS_StartTimer(MRS_STA_TIMER_RF_OVER_TIME,txMaxTime, HI_SYS_TIMER_ONESHOT);

  if(sWLTx.sBuff.len <= 0)return;
  
  si446x_change_state(SI446X_CMD_CHANGE_STATE_ARG_NEXT_STATE1_NEW_STATE_ENUM_READY);

  si446x_fifo_info(SI446X_CMD_FIFO_INFO_ARG_FIFO_TX_BIT);
  
  si446x_get_int_status(0u, 0u, 0u);
  
  rfTxLen = ( sWLTx.sBuff.len > FIFO_SIZE ) ? FIFO_SIZE : sWLTx.sBuff.len;
  
  si446x_write_tx_fifo(rfTxLen, sWLTx.sBuff.data);

  if(sWLTx.sBuff.len < 6500)
  {
    si446x_start_tx(channel, 0x80,sWLTx.sBuff.len);
  }
  else
  {
    si446x_start_tx(channel, 0x80,0);
  }
}

/*------��ʱɨ������ͨ��---------------*/
void  scanRfChannle(void)
{
  rfRxLen = 0;
  rfStatus = 1;
  if( ++channelArrPos >= sizeof(channelIndexArr) )
  {
    channelArrPos = 0;
  }
  u8 chnnlIndx = channelIndexArr[channelArrPos];
  u8 chnnlNo = CaculateChannelNo(chnnlIndx&0x01,chnnlIndx>>1);
  vRadio_StartRX(chnnlNo, 258);
}

/*------ʹ�����߽���---------------*/
void  enableRfSendReceive(void)
{
  #ifdef MSG_NOT_INT
  MRS_StartTimer(MRS_STA_TIMER_RF_SCAN_CHANNLE, SINGLE_CHANNEL_LISTEN_TIME, HI_SYS_TIMER_PERIODIC);
  #else
  HI_MDM_TIMER_Start(&scanRfHandel, scanRfChannle, SINGLE_CHANNEL_LISTEN_TIME,HI_TIMER_TYPE_PERIOD, 0);
  #endif
}

/*------��ֹ���߽���---------------*/
void  disbaleRfSendReceive(void)
{
  #ifdef MSG_NOT_INT
  MRS_StopTimer(MRS_STA_TIMER_RF_SCAN_CHANNLE);
  #else
  HI_MDM_TIMER_Stop(&scanRfHandel);
  #endif
}

/*------����������ɣ�������Ϣ������---------------*/
void  sendRfDatApp(void)
{
  HI_SYS_QUEUE_MSG_S stMsg;
  u32 gpio_para = rfRecMsg;
  
  mrsToolsMemset_s(&stMsg, sizeof(stMsg), 0, sizeof(stMsg));
  stMsg.ulMsgId = ID_RF_WIRE_LESS_REPORT;
  stMsg.ulTimeStamp = HI_MDM_GetMilliSeconds();
  stMsg.ulParam[0] = gpio_para;
  (HI_VOID)MRS_SendMessage(EN_APP_QUEUE_MRS, &stMsg, 0);
}

/*------���ߴ���ʱ-------------------*/
void waitRfOverTimer(void)
{
  if(3 == rfStatus && (rfRxLen > ALMOST_FULL_RX && rfRxLen < sWLRx.sBuff.len))
  {
    si446x_read_rx_fifo(RX_FIFO_ALMOST_FULL_BYTES,&sWLRx.sBuff.data[rfRxLen]);
    si446x_change_state(SI446X_CMD_CHANGE_STATE_ARG_NEXT_STATE1_NEW_STATE_ENUM_READY);
    si446x_fifo_info(SI446X_CMD_FIFO_INFO_ARG_FIFO_RX_BIT); 
    disbaleRfSendReceive();
    sendRfDatApp();
  }
  else
  {
    si446x_change_state(SI446X_CMD_CHANGE_STATE_ARG_NEXT_STATE1_NEW_STATE_ENUM_READY);
    si446x_fifo_info(SI446X_CMD_FIFO_INFO_ARG_FIFO_RX_BIT); 
    enableRfSendReceive();
    scanRfChannle();
  }
  
  MRS_StopTimer(MRS_STA_TIMER_RF_OVER_TIME);

  if(TRUE == gwarteBoard.ctl.datSendFlag || TRUE == gwarteBoard.ctl.wakeUpFlag)
  {
    memset((char *)&gwarteBoard,0x00,sizeof(gwarteBoard));
    vResetPrmbAndSysConfig();
    SgdSetRFRxParams();
  }

  else if(TRUE == gmCodeTest.mTestStartFlag)
  {
    memset((char *)&gmCodeTest,0x00,sizeof(gmCodeTest));
  }
}

/*-------����IRQ�ж���Ϣ����ӿں���---*/
void  rfProcessIrq(void)
{
  //�͹��ı�ƻ�������ת��
  if(TRUE == gwarteBoard.ctl.wakeUpFlag)
  {
    SgdRelayLowPowerMeterCommand(gwarteBoard.datBuf, gwarteBoard.datLen,\
                                 gwarteBoard.shortAddr,gwarteBoard.channel);
  }
  //M�������Ͳ���
  else if(TRUE == gmCodeTest.mTestStartFlag)
  {
    SgdRFTxTestSend();
  }
  //�����շ�
  else
  {
    si446x_get_int_status(0u, 0u, 0u);

    if(1 == rfStatus)
    {
      if(Si446xCmd.GET_INT_STATUS.MODEM_PEND & SI446X_CMD_GET_INT_STATUS_REP_MODEM_STATUS_PREAMBLE_DETECT_BIT)
      {  
        #ifdef MSG_NOT_INT
        MRS_StopTimer(MRS_STA_TIMER_RF_SCAN_CHANNLE);
        #else
        HI_MDM_TIMER_Stop(&scanRfHandel);
        #endif
        MRS_StartTimer(MRS_STA_TIMER_RF_OVER_TIME, WAIT_SYN_WORD_MAX_TIME, HI_SYS_TIMER_ONESHOT);
        rfStatus = 2;
      }
    }

    else if(2 == rfStatus)
    {
      if(Si446xCmd.GET_INT_STATUS.MODEM_PEND & SI446X_CMD_GET_INT_STATUS_REP_MODEM_STATUS_SYNC_DETECT_BIT)
      {
        MRS_StartTimer(MRS_STA_TIMER_RF_OVER_TIME, WAIT_RF_DAT_MAX_TIME, HI_SYS_TIMER_ONESHOT);
        sNode.inputPower = SgdGetInputPower();
        rfStatus = 3;
      }
    }

    else if(3 == rfStatus)
    {
      if(Si446xCmd.GET_INT_STATUS.PH_PEND & SI446X_CMD_GET_INT_STATUS_REP_PH_STATUS_RX_FIFO_ALMOST_FULL_BIT)
      {  
        si446x_read_rx_fifo(RX_FIFO_ALMOST_FULL_BYTES,&sWLRx.sBuff.data[rfRxLen]);

        if( rfRxLen == 0 )
        {         
          u8 DataLen = sWLRx.sBuff.data[0]; 
          SgdPhyWhitingData( &DataLen, 0x1FF );
          sWLRx.sBuff.len = DataLen + 3;
        }
        
        rfRxLen += RX_FIFO_ALMOST_FULL_BYTES;
        
        if( (sWLRx.sBuff.len <= RX_FIFO_ALMOST_FULL_BYTES) || ( rfRxLen >= sWLRx.sBuff.len ) ) 
        {    
          si446x_change_state(SI446X_CMD_CHANGE_STATE_ARG_NEXT_STATE1_NEW_STATE_ENUM_READY);
          si446x_fifo_info(SI446X_CMD_FIFO_INFO_ARG_FIFO_RX_BIT); 
          MRS_StopTimer(MRS_STA_TIMER_RF_OVER_TIME);
          disbaleRfSendReceive();
          sendRfDatApp();
        }      
      }    
    }

    else if(4 == rfStatus)
    {
      if (Si446xCmd.GET_INT_STATUS.PH_PEND & SI446X_CMD_GET_INT_STATUS_REP_PH_PEND_PACKET_SENT_PEND_BIT)
      {
        if(TRUE == gwarteBoard.ctl.datSendFlag)
        {
          memset((char *)&gwarteBoard,0x00,sizeof(gwarteBoard));
          vResetPrmbAndSysConfig();
          SgdSetRFRxParams();   
        }
        si446x_change_state(SI446X_CMD_CHANGE_STATE_ARG_NEXT_STATE1_NEW_STATE_ENUM_READY);
        si446x_fifo_info(SI446X_CMD_FIFO_INFO_ARG_FIFO_TX_BIT); 
        MRS_StopTimer(MRS_STA_TIMER_RF_OVER_TIME);
        scanRfChannle();	
		enableRfSendReceive();
      }

      if(Si446xCmd.GET_INT_STATUS.PH_PEND & SI446X_CMD_GET_INT_STATUS_REP_PH_PEND_TX_FIFO_ALMOST_EMPTY_PEND_BIT)
      {  
        if(rfTxLen < sWLTx.sBuff.len)
        {
          u8 wLen = 0;
          if ( (sWLTx.sBuff.len - rfTxLen) > ALMOST_FULL_RX )
          {
            wLen = ALMOST_FULL_RX;    
          }
          else
          {
            wLen = sWLTx.sBuff.len - rfTxLen;
          }
          si446x_write_tx_fifo( wLen, &sWLTx.sBuff.data[rfTxLen] );
          rfTxLen += wLen;
        }
      }
    }
  }
}

/*------4438�жϷ����¼���Ϣ---------------*/
HI_S32 IRQ_callback(HI_U32 gpio_para)
{
  if(0 == radio_hal_NirqLevel())
  {
    #ifdef MSG_NOT_INT
    u32 gpio_para = 0;
    HI_SYS_QUEUE_MSG_S stMsg;

    mrsToolsMemset_s(&stMsg, sizeof(stMsg), 0, sizeof(stMsg));
    stMsg.ulMsgId = ID_RF_IQR_MSG_REPORT;
    stMsg.ulTimeStamp = HI_MDM_GetMilliSeconds();
    stMsg.ulParam[0] = gpio_para;
    (HI_VOID)MRS_SendMessage(EN_APP_QUEUE_MRS, &stMsg, 0);
    #else
    rfProcessIrq();
    #endif
  }
}


#if defined(SIGNAL_MCU)&&defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
/*------���߶�ʱ������ص�����---------------*/
void rfTaskTimer_Callback(void)
{
  volatile u8 err = 0;

  if( sRFTest.Status & TESTING_TIMER_ON )
  {
    if( sRFTest.Timer < RF_TASK_TIMER)
    {
      sRFTest.Status &= (~TESTING_TIMER_ON);
      sRFTest.Status |= TESTING_TIME_OUT;
      err++;
    }
    else
    {
      sRFTest.Timer -= RF_TASK_TIMER;
    }
  }

  if(sRFboot.timer < RF_REBOOTInterval)
  {
    sRFboot.timer += RF_TASK_TIMER;
  }
  else
  {
    sRFboot.timer = 0;
    sRFboot.rebootFlag = TRUE;
    err++;
  }

  if(sUpRssiThreshold.timer < RssiThresholdUpdateInterval )
  {
    sUpRssiThreshold.timer += RF_TASK_TIMER;
  }
  else
  {
    sUpRssiThreshold.timer = 0;
    sUpRssiThreshold.updateFlag = TRUE;
    err++;
  }
  
  //������˫ģ
  //#if (3 == SUPPLY_AREA)
  //CSMAģʽ�ϱ�ͣ����Ϣ
  if(0x01 != powerDown.rfPowerDowmMode)
  {
    if(powerDown.ctlCsma.powerDownFlag)
    {
      if(powerDown.ctlCsma.powerDownTimer > POWER_DOWN_MAX_TIMER)
      {
        memset((char *)&powerDown.ctlCsma,0x00,sizeof(powerDown.ctlCsma));
      }
      else 
      {
        powerDown.ctlCsma.powerDownTimer += RF_TASK_TIMER;
        
        if(powerDown.ctlCsma.readPlcSwtich)
        {
           if(powerDown.ctlCsma.readPlcIntVal >= RF_TASK_TIMER)
           {
              powerDown.ctlCsma.readPlcIntVal -= RF_TASK_TIMER;
           }
           else
           {
              powerDown.ctlCsma.readPlcSwtich = SWTICH_OFF;
              powerDown.ctlCsma.readPlcFlag   = TRUE;
              err++;
           }
        }

        if(powerDown.ctlCsma.csmaTimerSwitch)
        {
          if(powerDown.ctlCsma.csmaOverTimer >= RF_TASK_TIMER)
          {
            powerDown.ctlCsma.csmaOverTimer -= RF_TASK_TIMER;
          }
          else
          {
            powerDown.ctlCsma.csmaTimerSwitch = SWTICH_OFF;
            powerDown.ctlCsma.csmaTimerFlag   = TRUE;
            err++;
          }
        }

        if(TRUE == powerDown.ctlCsma.readPlcFlag )
        {
          err++;
        }

        if(TRUE == powerDown.ctlCsma.wlSendFlag )
        {
          err++;
        }
      }
    }
  }
  else
  {
    if(powerDown.ctlTdma.startTimerSwitch)
    {
      u32 maxTimer = powerDown.ctlTdma.maxTimer;
      if(powerDown.ctlTdma.curTimer >= maxTimer)
      {
        powerDown.ctlTdma.curTimer = 0;
      }
      else
      {
        powerDown.ctlTdma.curTimer += RF_TASK_TIMER;
      }

      if(powerDown.ctlTdma.sendSwitch)
      {
        if(powerDown.ctlTdma.sendTimer >= RF_TASK_TIMER)
        {
          powerDown.ctlTdma.sendTimer -= RF_TASK_TIMER;
        }
        else
        {
          powerDown.ctlTdma.sendSwitch = SWTICH_OFF;
          powerDown.ctlTdma.sendFlag = TRUE;
          err++;
        }
      }
    }
  }
  //#endif  
  
  //�������ģ��
  #if (2 == SUPPLY_AREA)
  if(SWTICH_ON == gwarteBoard.ctl.warteBoardSwtich)
  {
    if(gwarteBoard.ctl.timer >= RF_TASK_TIMER)
    {
      gwarteBoard.ctl.timer -= RF_TASK_TIMER;
    }
    else
    {
      gwarteBoard.ctl.warteBoardSwtich = SWTICH_OFF;
      gwarteBoard.ctl.warteBoardflag = TRUE;
      err++;
    }
  }

  if(wv_ticks <= (0xFFFFFFFF - RF_TASK_TIMER))
  {
    wv_ticks += RF_TASK_TIMER;

    if(0 == (wv_ticks % 60000))
    {
      err++;
    }
  }
  else
  {
    wv_ticks = 0;
  }
  #endif 

  if(0 != err)
  {
    HI_SYS_QUEUE_MSG_S stMsg;
    u32 gpio_para = rfTimerMsg;
    mrsToolsMemset_s(&stMsg, sizeof(stMsg), 0, sizeof(stMsg));
    stMsg.ulMsgId = ID_RF_WIRE_LESS_REPORT;
    stMsg.ulTimeStamp = HI_MDM_GetMilliSeconds();
    stMsg.ulParam[0] = gpio_para;
    (HI_VOID)MRS_SendMessage(EN_APP_QUEUE_MRS, &stMsg, 0);
  }
}

/*-------------������Ƶ----------------------*/
void  SgdNodeOffNetWorkProcess(void)
{
  //��ƵоƬ����
  if( sRFboot.rebootFlag )
  {
    //������Ƶ
    sRFboot.rebootFlag = FALSE; 
    memset( (char *)&sRFTest, 0x00, sizeof(sRFTest) );
    //���³�ʼ����Ƶ
    vRadio_Init();  

    #ifdef MSG_NOT_INT
    MRS_StartTimer(MRS_STA_TIMER_RF_SCAN_CHANNLE, SINGLE_CHANNEL_LISTEN_TIME, HI_SYS_TIMER_PERIODIC);
    #else
    HI_MDM_TIMER_Start(&scanRfHandel, scanRfChannle, SINGLE_CHANNEL_LISTEN_TIME,HI_TIMER_TYPE_PERIOD, 0);
    #endif
  }
}

/*-------��ʼ������ģ������豸-------*/
void rf_main_init(void)
{
  HI_U32 ulret = 0;

  //��Ƶ��ʼ��
  if(0 == SPI4438_Init())
  {
    RADIO_Config();
    vRadio_Init();  
  }
#if 0
//�����Ȳ��Դ�;���������ļ�radio.h��ע��scanRfChannleɨƵ/�жϹܽ�
  //vRadio_StartRX(24, 252);
  //vRadio_TxPN9_Code(0);  
  HI_MDM_Sleep(2000);  
  //vRadio_Init(); 
  vRadio_StartRX(24, 252);
  return;
 //�����Ȳ��Դ�
#endif
  
  //����RF�жϹܽ�
  ulret = IRQ_config(IRQ_callback);
  //�����ŵ�ɨ�趨ʱ��
  #ifdef MSG_NOT_INT
  ulret = MRS_StartTimer(MRS_STA_TIMER_RF_SCAN_CHANNLE, SINGLE_CHANNEL_LISTEN_TIME, HI_SYS_TIMER_PERIODIC);
  #else
  ulret = HI_MDM_TIMER_Start(&scanRfHandel, scanRfChannle, SINGLE_CHANNEL_LISTEN_TIME,HI_TIMER_TYPE_PERIOD, 0);
  #endif

  #if 1
  ulret = HI_MDM_TIMER_Start(&rfTaskTimerHandel, rfTaskTimer_Callback, RF_TASK_TIMER,HI_TIMER_TYPE_PERIOD, 0);
  #else
  HI_HRTIMER_TYPE_S timerTest = {0};
  timerTest.expires =1000 * RF_TASK_TIMER;
  timerTest.func = rfTaskTimer_Callback;
  HI_MDM_HRTIMER_Start(&timerTest); 
  #endif
  
  //����������ʼ��
  CommonParamsInit();
  
  //��ʼ�����������
  SgdGetRandNumber();

  //��ʼ�������Ĳ���
  SgdNwkInit();
  
  //��ȡ�ڵ����
  //SgdGetNodeParameters();
  
  //��ʼ�������ŵ����ŵ�����
  SgdInitChannelIndexArr();
   
  //������ŵ�RSSI����ֵ
  SgdCalcRssiThresholdValue();
}

/*-------����������-------*/
void rf_main_task(u32 *pMsg)
{
  u32 taskMsg = pMsg[0];

  switch(taskMsg)
  {
    case rfRecMsg:
    {
      SgdWLRecvProcess();
    }
    break;

//CJJ-20181031:��������ֱ�ӻ�ȡ����ʹ����Ϣ
#if 0
    case uartRecMsg:
    {
      if(pMsg[1] < MAX_DL_645_FRAME_LEN)
      {
        sUsart2.dataLen = pMsg[1];
        memcpy(sUsart2.dataBuffer,(char *)pMsg[2],sUsart2.dataLen);
        sUsart2.status.RxDone = STATUS_TRUE;
        mrsToolsFree((HI_U8 *)pMsg[2]);
        SgdUsartProcess();
      }
    }
    break;
#endif    

    case rfTimerMsg:
    {
      rfTestTimer();
      SgdTimerProcess();
      SgdNodeOffNetWorkProcess();
      SgdUpdateRssiThresholdProcess();
      #if (2 == SUPPLY_AREA)
      checkNeighbour();
      #endif
    }
    break;
  }
}

#endif

