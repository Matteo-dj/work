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

#define   ADJUST_FREQUENCY_FLAG   0x02  //1字节
#define   CARRIER_FREQUENCY_HIGH  0x03  //1字节
#define   CARRIER_FREQUENCY_LOW   0x04  //1字节
#define   TX_POWER_FLAG           0x05  //1字节
#define   TX_POWER_VALUE          0x06  //1字节 
#define   TX_POWER_mCODE_FLAG     0x07  //1字节
#define   TX_POWER_mCODE_VALUE    0x08  //1字节 

#define   NWE_CHIP_FLAH_LOW       0x3E //1个字节
#define   NWE_CHIP_FLAH_HIGH      0x3F //1个字节
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

//单时隙时间300ms
#define   aBaseSlotDuration             300

#define   FIFO_SIZE                     64

#define   MAX_NEIGHBOURS                70
#define   MAX_LP_NEIGHBOURS             48
#define   MAX_NET_SCALE                 512

#define   SCD_TMSLT_MAX_NUM            1//    3

#define   FEEL_TMSLD_INTERVAL       150
#define   FEEL_FRAME_SEND_TIME      138       //感知广播帧传输时间为137.6ms
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
  u8 cmmdNo;	//命令序号add by hzy 16/3/11

  u8 tmSltNum;	//第二时隙数量
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
  u8  inNetworkFlag;        //在网标识，0：离网；1：在网
  u8  channelGroupNum;      //工作信道组号
  u8  inputPower;           //当前接收到的信号的功率值
  u8  rssiThershold;        //RSSI门限值
  u8  panID[2];             //网络标识
  u8  timeSlotLevelNum[2];  //时隙、层次号
  u8  shortAddr[2];         //节点短地址
  u8  longAddr[6];          //节点长地址 
  u8  masterDefaultAddr[6]; //中心节点默认地址：0xAAAAAAAAAAAA
  u8  masterConfigAddr[6];  //中心节点配置地址
  u8  waterChannelGroupNum; //水、气、热表工作信道组号
  
  u8 commdNo;	           //命令序号add by hzy 16/3/11
  u8 tmSlotNum;	           //第二时隙数量
  u16 scdTmSlotNo;
  CounterFlNetType flNetCuntr;
}sNodeParameters;

extern  sNodeParameters sNode;

typedef struct UartConfigParameters
{
  u8  parity;                 //串口校验=0,1,2(无，奇，偶)
  u8  wordLength;             //串口数据位=8，9（USART_WL_8B，USART_WL_9B） 
  u16 baudRate;               //串口波特率   
}sUartConfigParameters;

typedef struct BeaconStatusWord
{
  u8  relayedFlag:1;            //信标帧转发标识，为1表示已经转发过信标帧
  u8  relaySwitch:1;            //信标帧转发开关，为1表示开始转发信标帧
  u8  relayTimerSwitch:1;       //信标帧转发计时器开关
  u8  intervalTimerSwitch:1;    //信标周期计时器开关
  u8  RES:4;
}sBeaconStatusWord;

typedef struct BeaconRelayInfo
{
  sBeaconStatusWord status;
  u8  randomDelayTr;              //随机延迟时间
  u32 waitRelayTimeTi;            //转发当前信标帧所需等待的时间 
  u32 relayIntervalTe;            //两个合法的信标帧之间的时间间隔  
}sBeaconRelayInfo;
extern volatile sBeaconRelayInfo sBeacon;


#define SINGLE_CHANNEL_LISTEN_TIME  8     //单信道最大监听时间
#define WAIT_SYN_WORD_MAX_TIME      128   //等待同步字最大时间
#define WAIT_RF_DAT_MAX_TIME        220   //等待接收RF数据最大溢出时间
#define WAIT_TX_DAT_MAX_TIME        290
typedef struct ChannelListenStatus
{
  u8  listenSwitch:1;         //信道监听启动开关
  u8  listenTimerSwitch:1;    //信道监听计时器开关
  u8  synWordTimerSwitch:1;   //等待同步字计时器开关
  u8  recDatTimerSwitch:1;
  u8  listenTimeOutFlag:1;
  u8  synWordTimeOutFlag:1;
  u8  recDatTimeOutFlag:1;
  u8  RES:1;
}sChannelListenStatus;

typedef struct RFChannelListen
{
  sChannelListenStatus status;
  u8    listenTimer;            //信道监听计时器
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

//串口字节间的超时时间
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
  u8 startMark:1;		//控制域，启动标志位PRM,1个比特;0---服务器发起，1---客户机发起
  u8 transferMark:1;	//控制域，传输方向位DIR，1个比特;0---客户机发起，1---服务器发出
}sControlCode_698;

typedef struct AddrFlag_698
{
  u8 serverByte:3;
  u8 res:3;
  u8 serverType:2; //0表示单地址，1表示通配地址，2表示组地址，3表示广播地址
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
  bool  responseIdentifier;   //应答标识；TRUE表示需要响应；FALSE表示不需要响应
  sBuffer sBuff;
}sWireLessTx;

extern sWireLessTx sWLTx;

typedef struct WireLessRx
{
  sBuffer sBuff;
}sWireLessRx;

extern sWireLessRx sWLRx;

//入网申请响应帧应答计时器
typedef struct JoinNetRespond
{
  bool  respondTimerSwitch;    //应答计时器开关            
  u16   waitTimeForRespond;    //需等待的应答总时间
}sJoinNetRespond;

extern sJoinNetRespond sJoinNetResp;

typedef struct ActivejoinNetStatus
{
  u8  joinNetworkTimerSwitch:1;           //入网计时器开关
  u8  offNetworkTimerSwitch:1;            //离网计时器开关
  u8  waitJoinNetRespondTimerSwitch:1;    //等待入网申请响应帧计时器开关
  u8  waitConfigCommdTimerSwitch:1;       //等待配置子节点命令计时器开关
  u8  joinNetworkReady:1;                 //入网就绪状态
  u8  joiningNetworkStatus:1;             //正在入网状态，为1表示正在执行游离节点入网
  u8  offNetworkStatus:1;                 //离网标识状态，为1表示执行主动离网
  u8  collectJoinNetRespondTimerSwitch:1; //收集入网申请响应帧计时器开关
  u8  collectJoinNetRespondDone:1;        //收集网申请响应帧完成
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
  u32 timer;               //计时器,入网、离网都用同一个计时器
}sActivejoinNetTimer;

extern  volatile sActivejoinNetTimer sjoinNet;

#define    BROAD_DELAY_TIMER_FLAG         0x01    //广播校时延时计时器启动标识
#define    BROAD_DELAY_TIME_OUT_FLAG      0x02    //广播校时延时计时器超时标识，即完成等待时间，进行校时
typedef struct BroadCastTime
{
  u8    BroadFlag;               
  u32   DelayTimer;               //广播校时延时计时器                
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

//#define MIN_IFS     40                  //最小帧间隔周期
typedef struct SystemTimer
{
  bool  timerStart;
  bool  timerEnd;
  u32 timeCnt;
}sSystemTimer;

//邻居总个数
extern  u8  NeighbourNums;
extern  u8 joinNetNeighbourNums;
//CJJ-2018-1-19:邻居表使用数组保存
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
//  sNeighbourList *front;   //链表首指针
//  sNeighbourList *rear;    //链表尾指针
//}sNeighbourListLK;
//
//extern sNeighbourListLK sNeighbListLK;
   
#define   BEACON_MAC_PAY_LOAD_POS     17
typedef struct macBeaconDatSave
{
  u8 datBuf[BEACON_MAC_PAY_LOAD_POS];     //暂存信标数据帧   
  u8 datLen;                          //暂存信标数据buf长度  
  u8 TS_LevelNum[2]; //暂存信标转发时隙
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
  sLowPowerNeighbour *front;   //链表首指针
  sLowPowerNeighbour *rear;    //链表尾指针
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


#define   LED_TIMER_SPACE                10          //运行周期（毫秒）

#if defined(DOUBLE_COLOR_LEDS)
#define   CHANNEL_LED_RUN_SPACE         600  //(60*10=600ms ) //信道灯运行间隔2s，即信道灯指示完一轮后，等待2s，然后继续指示
#endif

#if defined(SINGLE_COLOR_LEDS)
#define   LED_INNET_RUN_SPACE           50   //(5*10=50ms)    //在网灯闪烁50*LED_TIMER_SPACE毫秒
#define   LED_OUTNET_RUN_SPACE          100  //(10*10=100ms)  //运行灯闪烁100*LED_TIMER_SPACE毫秒
#endif

#define   CHANNEL_LED_TURN_ON_SPACE     200  //(20*10=200ms)  //信道灯闪烁周期（1s闪烁一个bit）
#define   CHANNEL_LED_TURN_OFF_SPACE    50   //(5*10=50ms)    //信道灯关闭周期

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
  u8    LedTimer;                 //LED执行周期计时器
  u16    LedStatus;                //LED状态字
  
  u16   ChannelLedSpaceCnt;       //信道灯执行计数器
  u16   ChannelLedMaxSpace;       //信道灯运行周期
  
  u8    ChannelLedTurnOffSpaceCnt;//信道灯关闭计时器
  
  u8    RxLedSpaceCnt;            //射频接收灯运行周期计数器
  u8    RxLedMaxSpace;            //射频接收灯运行周期
  u8    RxLedRunCnt;              //射频接收灯执行次数
  
  u8    TxLedSpaceCnt;            //射频发射灯运行周期计数器
  u8    TxLedMaxSpace;            //射频发射灯运行周期
  u8    TxLedRunCnt;              //射频发射灯执行次数
  
  u8    TxdLedSpaceCnt;          //串口收发发灯运行周期计数器
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

//门限值更新周期（8小时）
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

//射频芯片重启周期时间为24小时，当24小时之内没有收到任何无线数据，则认为射频芯片死机
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
//随机数
extern  u16 gRandNumber;

#define   BOARD_CTR_OVER_TIMER   ((u32)120*1000)

extern volatile bool noCheckUartPe;

#define POWER_DOWN_MAX_SEND 5                    //发送掉电广播最大次数
#define POWER_DOWN_RETRY_VAL (5*1000)            //重发最小间隔时间
#define POWER_DOWN_RETRY_OVER_TIMER  (10*1000)   //重发最大间隔时间

#define POWER_DOWN_MAX_TIMER (30 * 1000)         //发送广播最大竞争时间
#define PLC_MAC_FRAME_LEN 160
#define PLC_BOARD_FRAME 0x11                     //掉电广播帧命令类型  

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

