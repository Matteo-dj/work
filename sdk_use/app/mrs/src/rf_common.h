#ifndef __COMMON_H__
#define __COMMON_H__

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "app_mrs.h"
#include "Mrs_fw_tools.h"
#include "Mrs_fw.h"
#include "Mrs_srv_res.h"
#include "Hi_mdm_timer.h"
#include "Mrs_cmd_msg.h"
#include "APP_res_cfg.h"
#include "Mrs_fw.h"
#include "Mrs_srv_rf.h"
#include "Hi_mdm_types.h"
#include "Hi_usr_NV.h"
#include "Mrs_fw_log.h"



//#include "Mrs_srv_sta.h"



#define     __I     volatile const   /*!< defines 'read only' permissions     
*/
#define     __O     volatile         /*!< defines 'write only' permissions    
*/
#define     __IO    volatile         /*!< defines 'read / write' permissions  
*/

/*!< Signed integer types  */
typedef   signed char     int8_t;
typedef   signed short    int16_t;
typedef   signed long     int32_T;

/*!< Unsigned integer types  */
typedef unsigned char     uint8_t;
typedef unsigned short    uint16_t;
typedef unsigned long     uint32_T;
typedef unsigned long long    uint64_t;

typedef int32_T  s32;
typedef int16_t s16;
typedef int8_t  s8;

typedef uint64_t  u64;
typedef uint32_T  u32;
typedef uint16_t u16;
typedef uint8_t  u8;

#define RF_DEBUG

#pragma pack(1)
#define   EEP_START_ADDR          0x00 

#define   CHANNEL_GROUP           0x00
#define   TX_POWER                0x01

#define   ADJUST_FREQUENCY_FLAG   0x02  //1�ֽ�
#define   CARRIER_FREQUENCY_HIGH  0x03  //1�ֽ�
#define   CARRIER_FREQUENCY_LOW   0x04  //1�ֽ�
#define   TX_POWER_FLAG           0x05  //1�ֽ�
#define   TX_POWER_VALUE          0x06  //1�ֽ� 
#define   TX_POWER_mCODE_FLAG     0x07  //1�ֽ�
#define   TX_POWER_mCODE_VALUE    0x08  //1�ֽ� 

#define   NWE_CHIP_FLAH_LOW       0x3E //1���ֽ�
#define   NWE_CHIP_FLAH_HIGH      0x3F //1���ֽ�
#define   EEP_END_ADDR            0x40         

#define   NODE_TYPE_VALUE         0x07
#define   VENDER_ID1              'G'
#define   VENDER_ID2              'D'
#define   HARDWARE_SUB_VER        0x00
#define   HARDWARE_MASTER_VER     0x02

#define   SOFTWARE_SUB_VER        0x00
#define   SOFTWARE_MASTER_VER     0x01
#define   BOOTLOADER_VER          0x01

#define   INTERNAL_SUB_VERSION    0x03
#define   INTERNAL_MASTER_VERSION 0x10
#define   SUPPLY_AREA             0x02


#define   RF_MAX_BUF_LEN          258
#define   LONG_ADDR_LEN           6
#define   SHORT_ADDR_LEN          2

//��ʱ϶ʱ��300ms
#define   aBaseSlotDuration             300

#define   FIFO_SIZE                     64

#define   MAX_NEIGHBOURS                70
#define   MAX_LP_NEIGHBOURS             48
#define   MAX_NET_SCALE                 512

#define   SCD_TMSLT_MAX_NUM            1//    3

#define   FEEL_TMSLD_INTERVAL       150
#define   FEEL_FRAME_SEND_TIME      138       //��֪�㲥֡����ʱ��Ϊ137.6ms
#define   FEEL_FRAME_DWN_LINK       0x01
#define   FEEL_FRAME_UP_LINK        0x02

#define   RF_TASK_TIMER             10

extern __IO uint32_t wv_ticks;

enum  StatusFlag
{
  STATUS_FALSE = 0x00,
  STATUS_TRUE
};

enum  SwitchStatus
{
  SWTICH_OFF = 0x00,
  SWTICH_ON
};

enum  AddrMode
{
  SHORT_ADDR_MODE = 0x02,
  LONG_ADDR_MODE = 0x03
};

typedef struct Buffer
{
  u16 len;
  u8  data[310];
}sBuffer;

typedef struct TimeSlot_Level_Num
{
  u16 TSNum:10;
  u16 levelNum:4;
  u16 RES:2;
}sTimeSlot_Level_Num;

typedef struct FeelNetExpand
{
  u8 cmmdNo;	//�������add by hzy 16/3/11

  u8 tmSltNum;	//�ڶ�ʱ϶����
  u16 scdTmSltNo;
}FeelNetExpandType;

typedef struct CounterFlNet
{
  __IO u32 freeWaitTime:25;
  __IO u32             :7;
  __IO u32 sendWaitTime:25; 
  __IO u32 sendFlag:1;
  __IO u32 flag:1;
  __IO u32 counterSwch:1;
  __IO u32 delaySwch:1;
 __IO  u32 lockFlag:1;
 __IO  u32 upLinkFstFlag:1;
 __IO u32 downLinkFstFlag:1;

}CounterFlNetType;  

typedef struct NodeParameters
{
  u8  inNetworkFlag;        //������ʶ��0��������1������
  u8  channelGroupNum;      //�����ŵ����
  u8  inputPower;           //��ǰ���յ����źŵĹ���ֵ
  u8  rssiThershold;        //RSSI����ֵ
  u8  panID[2];             //�����ʶ
  u8  timeSlotLevelNum[2];  //ʱ϶����κ�
  u8  shortAddr[2];         //�ڵ�̵�ַ
  u8  longAddr[6];          //�ڵ㳤��ַ 
  u8  masterDefaultAddr[6]; //���Ľڵ�Ĭ�ϵ�ַ��0xAAAAAAAAAAAA
  u8  masterConfigAddr[6];  //���Ľڵ����õ�ַ
  u8  waterChannelGroupNum; //ˮ�������ȱ����ŵ����
  
  u8 commdNo;	           //�������add by hzy 16/3/11
  u8 tmSlotNum;	           //�ڶ�ʱ϶����
  u16 scdTmSlotNo;
  CounterFlNetType flNetCuntr;
}sNodeParameters;

extern  sNodeParameters sNode;

typedef struct UartConfigParameters
{
  u8  parity;                 //����У��=0,1,2(�ޣ��棬ż)
  u8  wordLength;             //��������λ=8��9��USART_WL_8B��USART_WL_9B�� 
  u16 baudRate;               //���ڲ�����   
}sUartConfigParameters;

typedef struct BeaconStatusWord
{
  u8  relayedFlag:1;            //�ű�֡ת����ʶ��Ϊ1��ʾ�Ѿ�ת�����ű�֡
  u8  relaySwitch:1;            //�ű�֡ת�����أ�Ϊ1��ʾ��ʼת���ű�֡
  u8  relayTimerSwitch:1;       //�ű�֡ת����ʱ������
  u8  intervalTimerSwitch:1;    //�ű����ڼ�ʱ������
  u8  RES:4;
}sBeaconStatusWord;

typedef struct BeaconRelayInfo
{
  sBeaconStatusWord status;
  u8  randomDelayTr;              //����ӳ�ʱ��
  u32 waitRelayTimeTi;            //ת����ǰ�ű�֡����ȴ���ʱ�� 
  u32 relayIntervalTe;            //�����Ϸ����ű�֮֡���ʱ����  
}sBeaconRelayInfo;
extern volatile sBeaconRelayInfo sBeacon;


#define SINGLE_CHANNEL_LISTEN_TIME  8     //���ŵ�������ʱ��
#define WAIT_SYN_WORD_MAX_TIME      128   //�ȴ�ͬ�������ʱ��
#define WAIT_RF_DAT_MAX_TIME        220   //�ȴ�����RF����������ʱ��
#define WAIT_TX_DAT_MAX_TIME        290
typedef struct ChannelListenStatus
{
  u8  listenSwitch:1;         //�ŵ�������������
  u8  listenTimerSwitch:1;    //�ŵ�������ʱ������
  u8  synWordTimerSwitch:1;   //�ȴ�ͬ���ּ�ʱ������
  u8  recDatTimerSwitch:1;
  u8  listenTimeOutFlag:1;
  u8  synWordTimeOutFlag:1;
  u8  recDatTimeOutFlag:1;
  u8  RES:1;
}sChannelListenStatus;

typedef struct RFChannelListen
{
  sChannelListenStatus status;
  u8    listenTimer;            //�ŵ�������ʱ��
}sRFChannelListen;

extern volatile sRFChannelListen sRFChannel;

typedef struct tagRFSendFlag
{
  u8  sendTimerSwch:1;         
  u8  sendTimeOutFlag:1;
  u8  res:6;
}RFSendFlagType;

typedef struct tagRFSendStatus
{
  RFSendFlagType flag;
  u32 timer;           
}RFSendStatusType;
extern __IO RFSendStatusType gRFSendStatus;

//�����ֽڼ�ĳ�ʱʱ��
#define   MAX_USART_RX_OVER_TIME   50  
typedef struct UsartStatus
{
  u32  RxTimerSwitch:1;
  u32  RxDone:1;
  u32  TxDone:1;
  u32  RES:5;
  u32  bitPack:16;
  u32  cntPack:4;
  u32  :4;
}sUsartStatus;

#define MAX_DL_645_FRAME_LEN    255
#define MAX_DL_PACK_COUNT       13
#define MAX_645_READ_METER_TIME 3000
#define MAX_698_READ_METER_TIME 12000
#define MAX_698_SIGNAL_LEN      160

typedef struct UsartRxTx
{
  sUsartStatus status;
  u8  RxOverTimeTimer;
  u16  TxBytes;
  u16  dataLen;       
  u8  dataBuffer[MAX_DL_645_FRAME_LEN];
}sUsartRxTx;

extern volatile sUsartRxTx sUsart2;

typedef struct ControlCode_698
{
  u8 functionCode:4;
  u8 res:2;
  u8 startMark:1;		//������������־λPRM,1������;0---����������1---�ͻ�������
  u8 transferMark:1;	//�����򣬴��䷽��λDIR��1������;0---�ͻ�������1---����������
}sControlCode_698;

typedef struct AddrFlag_698
{
  u8 serverByte:3;
  u8 res:3;
  u8 serverType:2; //0��ʾ����ַ��1��ʾͨ���ַ��2��ʾ���ַ��3��ʾ�㲥��ַ
}sAddrFlag_698;

typedef struct ProHead_698
{
  u8 startCode;
  u8 dataLen[2];
  sControlCode_698 sCtrCode;
  sAddrFlag_698 sAddrFlag;
}sProHead_698;

enum ServerAddrType
{
  SINGLE_ADDRESS = 0,
  WILDCARD_ADDRESS,
  GROUP_ADDRESSS,
  BROADCAST_ADDRESS
};

typedef struct ProHeadEnd_698
{
  u8 sClientAddr;
  u8 sHeadHcs[2];
}sProHeadEnd_698;

typedef struct ProEnd_698
{
  u8 frameFcs[2];
  u8 endCode;
}sProEnd_698;

#define RELAY_FEEL_NET_LEN 80
typedef struct  SaveFeelNetDat_t
{
  u8 dataLen;
  u8 dataBuffer[RELAY_FEEL_NET_LEN];
  
}SaveFeelNetDat;

#define     WL_TX_MAX_TIMES     0x03

typedef struct WireLessTx
{
  bool  responseIdentifier;   //Ӧ���ʶ��TRUE��ʾ��Ҫ��Ӧ��FALSE��ʾ����Ҫ��Ӧ
  sBuffer sBuff;
}sWireLessTx;

extern sWireLessTx sWLTx;

typedef struct WireLessRx
{
  sBuffer sBuff;
}sWireLessRx;

extern sWireLessRx sWLRx;

//����������Ӧ֡Ӧ���ʱ��
typedef struct JoinNetRespond
{
  bool  respondTimerSwitch;    //Ӧ���ʱ������            
  u16   waitTimeForRespond;    //��ȴ���Ӧ����ʱ��
}sJoinNetRespond;

extern sJoinNetRespond sJoinNetResp;

typedef struct ActivejoinNetStatus
{
  u8  joinNetworkTimerSwitch:1;           //������ʱ������
  u8  offNetworkTimerSwitch:1;            //������ʱ������
  u8  waitJoinNetRespondTimerSwitch:1;    //�ȴ�����������Ӧ֡��ʱ������
  u8  waitConfigCommdTimerSwitch:1;       //�ȴ������ӽڵ������ʱ������
  u8  joinNetworkReady:1;                 //��������״̬
  u8  joiningNetworkStatus:1;             //��������״̬��Ϊ1��ʾ����ִ������ڵ�����
  u8  offNetworkStatus:1;                 //������ʶ״̬��Ϊ1��ʾִ����������
  u8  collectJoinNetRespondTimerSwitch:1; //�ռ�����������Ӧ֡��ʱ������
  u8  collectJoinNetRespondDone:1;        //�ռ���������Ӧ֡���
  u8  RES:7;
}sActivejoinNetStatus;

typedef struct sActiveJoinNetRetry_st
{
  u8 channel;
  u8 retryCnt;
  u32 retryInterval;
  u32 timer;

}sActiveJoinNetRetry;

typedef struct  ActivejoinNetTimer
{
  sActivejoinNetStatus sStatus;
  sActiveJoinNetRetry  retry;
  u32 timer;               //��ʱ��,��������������ͬһ����ʱ��
}sActivejoinNetTimer;

extern  volatile sActivejoinNetTimer sjoinNet;

#define    BROAD_DELAY_TIMER_FLAG         0x01    //�㲥Уʱ��ʱ��ʱ��������ʶ
#define    BROAD_DELAY_TIME_OUT_FLAG      0x02    //�㲥Уʱ��ʱ��ʱ����ʱ��ʶ������ɵȴ�ʱ�䣬����Уʱ
typedef struct BroadCastTime
{
  u8    BroadFlag;               
  u32   DelayTimer;               //�㲥Уʱ��ʱ��ʱ��                
}sBroadCastTime;

extern volatile sBroadCastTime sBroadCastT;

#define   TESTING_TIMER_ON      0x01
#define   TESTING_TIME_OUT      0x02
typedef struct RFTestingParams
{
  u8  Status;
  u8  ControlCode;
  u8  channelIndex;
  u8  CmdMode;
  u32 Timer;
}sRFTesting;
extern volatile sRFTesting sRFTest;

typedef struct mCodeTest_ST
{
  bool mTestStartFlag;
  u32  mTestLen;
}mCodeTest_st;
extern volatile mCodeTest_st gmCodeTest;

//#define MIN_IFS     40                  //��С֡�������
typedef struct SystemTimer
{
  bool  timerStart;
  bool  timerEnd;
  u32 timeCnt;
}sSystemTimer;

//�ھ��ܸ���
extern  u8  NeighbourNums;
extern  u8 joinNetNeighbourNums;
//CJJ-2018-1-19:�ھӱ�ʹ�����鱣��
typedef struct sNeighbourList
{
  u8 	addr[6];
//  u8    centrePanID[2];
  u8	inputPower;
  u32 timer;
//  u8	beaconMark;
}sNeighbourList;

//typedef struct
//{
//  sNeighbourList *front;   //������ָ��
//  sNeighbourList *rear;    //����βָ��
//}sNeighbourListLK;
//
//extern sNeighbourListLK sNeighbListLK;
   
#define   BEACON_MAC_PAY_LOAD_POS     17
typedef struct macBeaconDatSave
{
  u8 datBuf[BEACON_MAC_PAY_LOAD_POS];     //�ݴ��ű�����֡   
  u8 datLen;                          //�ݴ��ű�����buf����  
  u8 TS_LevelNum[2]; //�ݴ��ű�ת��ʱ϶
}sBeaconDatSaveInfo;
extern  sBeaconDatSaveInfo sBeaconDatSvae;

#if 0
typedef struct  lowPowerNeighbour
{
  u8  addr[7];
  u8  inputPower;
  struct lowPowerNeighbour *next;
}sLowPowerNeighbour;

typedef struct LowPowerNeighbourList
{
  sLowPowerNeighbour *front;   //������ָ��
  sLowPowerNeighbour *rear;    //����βָ��
}sLowPowerNeighbourList;
extern  sLowPowerNeighbourList  sLPNeighbourList;
#endif

extern  u8  LPNeighbourNums;

typedef struct  WaterMeterData
{
  u8  inputPower;
  u8  addr[7];
  u8  time[6];
  u8  meterNode;
  u8  sumFlow[4];
  u8  recordDat;
  u8  lastMonthRecord[4];
  u8  statue;
  u8  offLineyear;
  u32 timer;
}sWaterMeterData;  

typedef struct  WaterMeterLoadHead
{
  u8  start;
  u8  node;
  u8  addr[7];
  u8  command;
  u8  len;
}sWaterMeterLoadHead; 

typedef struct  WaterMeterLoadEnd
{
  u8 cs;
  u8 end;
}sWaterMeterLoadEnd; 

#define   apscMaxFrameRetries           3
#define   apsEventReportInterval        30*1000
#define   apsEventReportDelay           ((u32)30*60*1000)
//typedef struct EventReport
//{
//  bool  newEventFlag;
//  bool  reportFlag;
//  u8    reportCnt;
//  bool  reportTimerSwitch;
//  u32   reportTimer;
//}sEventReport;
typedef struct EventReport
{
  u32  newEventFlag:1;
  u32  reportTimerSwitch:1;
  u32  reportFlag:1;
  u32  reportCnt:5;
  u32  reportTimer:24;
}sEventReport;

extern volatile sEventReport sEvent;


#define   LED_TIMER_SPACE                10          //�������ڣ����룩

#if defined(DOUBLE_COLOR_LEDS)
#define   CHANNEL_LED_RUN_SPACE         600  //(60*10=600ms ) //�ŵ������м��2s�����ŵ���ָʾ��һ�ֺ󣬵ȴ�2s��Ȼ�����ָʾ
#endif

#if defined(SINGLE_COLOR_LEDS)
#define   LED_INNET_RUN_SPACE           50   //(5*10=50ms)    //��������˸50*LED_TIMER_SPACE����
#define   LED_OUTNET_RUN_SPACE          100  //(10*10=100ms)  //���е���˸100*LED_TIMER_SPACE����
#endif

#define   CHANNEL_LED_TURN_ON_SPACE     200  //(20*10=200ms)  //�ŵ�����˸���ڣ�1s��˸һ��bit��
#define   CHANNEL_LED_TURN_OFF_SPACE    50   //(5*10=50ms)    //�ŵ��ƹر�����

#define   CHANNEL_LED_RUN_FLAG            0x01
#define   RF_RX_TIMER_ON_FLAG             0x02
#define   RF_RX_LED_RUN_FLAG              0x04
#define   RF_TX_TIMER_ON_FLAG             0x08
#define   RF_TX_LED_RUN_FLAG              0x10
#define   CHANNEL_LED_TURN_OFF_TIMER_ON   0x20
#define   CHANNEL_LED_TURN_OFF_TIME_OUT   0x40

#define   UART_RX_TIMER_ON_FLAG             0x80
#define   UART_RX_LED_RUN_FLAG              0x100
#define   UART_TX_TIMER_ON_FLAG             0x200
#define   UART_TX_LED_RUN_FLAG              0x400
typedef struct Led_Param
{
  u8    LedTimer;                 //LEDִ�����ڼ�ʱ��
  u16    LedStatus;                //LED״̬��
  
  u16   ChannelLedSpaceCnt;       //�ŵ���ִ�м�����
  u16   ChannelLedMaxSpace;       //�ŵ�����������
  
  u8    ChannelLedTurnOffSpaceCnt;//�ŵ��ƹرռ�ʱ��
  
  u8    RxLedSpaceCnt;            //��Ƶ���յ��������ڼ�����
  u8    RxLedMaxSpace;            //��Ƶ���յ���������
  u8    RxLedRunCnt;              //��Ƶ���յ�ִ�д���
  
  u8    TxLedSpaceCnt;            //��Ƶ������������ڼ�����
  u8    TxLedMaxSpace;            //��Ƶ�������������
  u8    TxLedRunCnt;              //��Ƶ�����ִ�д���
  
  u8    TxdLedSpaceCnt;          //�����շ������������ڼ�����
  u8    TxdLedMaxSpace;           
  u8    TxdLedRunCnt;             
  
  u8    RxdLedSpaceCnt;            
  u8    RxdLedMaxSpace;           
  u8    RxdLedRunCnt;              
}sLedParam;

extern u8 SensitivityTestCNT;

#define  ExtendFunctionCode   0x1F
enum  ExtendCI_645
{
  RF_TX_PN9_CODE = 0x01,
  FREQUENCY_CALIBRATED = 0x02,
  SET_VERSION_INFO = 0x03,
  SET_LONG_ADDR = 0x04,
  SET_RF_TX_POWER = 0x05,
  GET_INPUT_POWER = 0x06,
  GET_VERSION_INFO = 0x07,
  GET_INTERNAL_VERSION = 0x08,
  TXPOWER_CALIBRATED = 0x09,
  FREQUENCY_TXPOWER_CALIBRATED=0x0A,
  mCODE_TXPOWER_CALIBRATED=0x0B,
  GET_DUAL_MODE_SENSTIV=0x1E,

  GET_PLC_POWER_DOWN_FRAME=0x10,
  RELAY_PLC_POWER_DOWN_FRAME=0x11,
  GET_PLC_PARAMETERM=0x12,
  GET_PLC_METER_ADDR=0x13,
  RELAY_PLC_UP_WATER_FRAME=0x14,

  SET_PLC_PARAMETERM=0x20,
  SET_PLC_TI=0x21,
  SET_PLC_METER_ADDR=0x22,
  UPDATE_POWER_STATUE=0x23,
  HPLC_SEND_LPW_DAT=0x24,
  HPLC_GET_NODE_RSSI=0x25
  //HPLC_SET_RF_MODE=0x26
};

enum  DIRECTION
{
  DOWN_DIRECTION = 0,
  UP_DIRECTION = 1
};

typedef struct ControlCode_645
{
  u8  functionCode:5;
  u8  followUpFlag:1;
  u8  slaveRespondFlag:1;
  u8  direction:1;  
}sControlCode_645;

typedef struct ProHead_645
{
  u8  startCode1;
  u8  addr[6];
  u8  startCode2;
  sControlCode_645  sCtrCode;
  u8  dataLen;
}sProHead_645;

typedef struct ProEnd_645
{
  u8  sumCrc;
  u8  endCode;
}sProEnd_645;

//typedef struct upGradeTimer
//{
//  bool flag;
//  u8  Switch;
//  u32 timer;
//}upGradeTimer_st;
typedef struct upGradeTimer
{
  u32 flag  :1;
  u32 Switch:1;
  u32 timer :30;
}upGradeTimer_st;
extern volatile upGradeTimer_st UpgradTimer;

//����ֵ�������ڣ�8Сʱ��
#define   RssiThresholdUpdateInterval   ((u32)8*60*60*1000)
//typedef struct UpdateRssiThreshold
//{
//  bool  updateFlag;
//  u32   timer;
//}sUpdateRssiThreshold;
typedef struct UpdateRssiThreshold
{
  u32   updateFlag:1;
  u32   timer     :31;
}sUpdateRssiThreshold;
extern volatile sUpdateRssiThreshold sUpRssiThreshold;

//��ƵоƬ��������ʱ��Ϊ24Сʱ����24Сʱ֮��û���յ��κ��������ݣ�����Ϊ��ƵоƬ����
#define RF_REBOOTInterval                ((u32)5*24*60*60*1000)
//typedef struct  RF_Reboot
//{
//  bool  rebootFlag;
//  u32 timer;
//}sRF_Reboot;
typedef struct  RF_Reboot
{
  u32  rebootFlag:1;
  u32  timer     :31;
}sRF_Reboot;
extern  volatile sRF_Reboot sRFboot;

extern  u8  channelRssiThresholdArr[4];
extern  volatile u8  channelIndexArr[4];
extern  volatile u8  channelArrPos;
extern  volatile u8  recvChannelArrPos;
//�����
extern  u16 gRandNumber;

#define   BOARD_CTR_OVER_TIMER   ((u32)120*1000)

extern volatile bool noCheckUartPe;

#define POWER_DOWN_MAX_SEND 5                    //���͵���㲥������
#define POWER_DOWN_RETRY_VAL (5*1000)            //�ط���С���ʱ��
#define POWER_DOWN_RETRY_OVER_TIMER  (10*1000)   //�ط������ʱ��

#define POWER_DOWN_MAX_TIMER (30 * 1000)         //���͹㲥�����ʱ��
#define PLC_MAC_FRAME_LEN 160
#define PLC_BOARD_FRAME 0x11                     //����㲥֡��������  

typedef struct plcPowerDown
{
  struct
  {
    volatile u32 maxTimer :32;
    volatile u32 curTimer :32;
    volatile u32 sendTimer:32;
    
    volatile u8 startTimerSwitch :1;
    volatile u8 sendSwitch:1;
    volatile u8 sendFlag:1;
    volatile u8         :5;
   }ctlTdma;
  
  struct
  {
    volatile u32 powerDownFlag : 1;
    volatile u32 readPlcSwtich : 1;
    volatile u32 readPlcFlag   : 1;
    volatile u32 csmaTimerSwitch:1;
    volatile u32 csmaTimerFlag  :1;
    volatile u32 wlSendFlag     :1;
    volatile u32               : 2;
    volatile u32 sendCnt       : 8;
    volatile u32 csmaOverTimer : 16;
    
    volatile u32 readPlcIntVal : 16;
    volatile u32 powerDownTimer: 16;
  }ctlCsma;

  
  u8 ti[2];
  u8 snid[3];
  u8 tei[2];
  u8 maxTei[2];
  u8 plcFrameLen;
  u8 plcFrameBuf[PLC_MAC_FRAME_LEN];
  u8 rfPowerDowmMode;
  bool getedTeiFlag;
  
}plcPowerDown_st;
extern plcPowerDown_st powerDown;


#define WARTE_BOARD_MAX_LEN 256
typedef struct warteBoard_ST
{
  struct
  {
    volatile u32 warteBoardSwtich :1;
    volatile u32 warteBoardflag   :1;
    volatile u32 timer:30;

    volatile u32 wakeUpFlag       :1;
    volatile u32 datSendFlag      :1;
    volatile u32 res              :6;
    volatile u32 wakeUpNum        :8;
    volatile u32 wakeUpLen        :8;
    volatile u32 TxLen            :8;
  }ctl;

  u16 shortAddr;
  u8 channel;
  u8 datLen;
  u8 datBuf[WARTE_BOARD_MAX_LEN];
}warteBoard_st;
extern warteBoard_st gwarteBoard;


#pragma pack()
//extern  u32 systemTimer;


void CommonParamsInit(void);

u8 Flash_Readbyte(u16 addr);

void Flash_Writebyte(u16 addr,u8 value,bool flag);

void SgdMemMoveBack( sBuffer *pBuff, u8 interval );

void SgdCalcRssiThresholdValue(void);

u8 SgdGetMinInputPower(void);

void  SgdInitChannelIndexArr(void);

u8    SgdGetSum( u8 *buf, u8 len );

void datFromFlashToRam(void);


#endif //__COMMON_H__

