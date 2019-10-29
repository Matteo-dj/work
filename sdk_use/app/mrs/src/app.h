#ifndef _APP_H
#define	_APP_H

#include "mac.h"
#include "nwk.h"

#pragma pack(1)

enum  AppFrameType
{
  CONFIRM_OR_DENY_FRAME = 0x00,
  COMMAND_FRAME = 0x01,
  DATA_RELAY_FRAME = 0x02,
  REPORTED_FRAME = 0x03
};

enum AppCI
{
  ConfigUart = 0x00,
  SetChannelGroupNum,
  SetRSSIThreshold,
  SetTxPowrer,
  GetNodeConfigInfo,
  RebootNode,
  UpGradeNode,
  BroadcastTimeCalibration,
  RelayAlarmBoxDataRequest = 0x10,
  RelayAlarmBoxRespond = 0x11,
  RelayAlarmBoxReported = 0x12,
  RelayAlarmBoxReportedRespond = 0x13,
  EXTEND_CI = 0xF1,
  RelayLowPowerMeterFrame = 0x0D
};

enum  Parity_id
{
  PARITY_NO = 0x00,
  PARITY_ODD,
  PARITY_EVEN
};

enum  Baudrate_id
{
  BAUDRATE_1200 = 0x01,
  BAUDRATE_2400,
  BAUDRATE_4800,
  BAUDRATE_9600,
  BAUDRATE_19200
};

enum  AppExtend_CI
{
  GET_INPUT_POWER_WL = 0x00,
  ACTIVE_JOIN_NET_CMD = 0x01,
  GET_RSSITHRESHOLD = 0x02,
  GET_SYSTEN_UP_TIME = 0x03,
  RECV_SENSITIVITY_TEST = 0x04,
  APS_GET_INTERNAL_VERSION = 0x05,
  APS_GET_BEACON_VALUE = 0x06,
  APS_GET_RSSI_VALUE = 0x07,
  APS_GET_BEACON_INDEX_VALUE = 0x08,
  APS_GET_WLTXBUFF_VALUE = 0x09,
  APS_GET_BEACON_PAY_LOAD = 0x10,
  GET_TX_POWER=0x11,
  APS_GET_FLASH_PARAM=0x20,

  APS_TEST_COMMAND=0x30
};

typedef struct AppFcd
{
  u8  frameType:3;
  u8  OEI:1;
  u8  RES:4;
}sAppFcd;

//自定义信息
typedef struct customInfo
{
  u8  ProducerName[3];      //厂家名称，本公司默认为"S","G","D"
  u8  UpGradeChannelGroup;  //升级信道组号
  u8  UpGradeChannelNum;    //升级信道号
  u8  appStartAddr[2];      //应用程序起始地址
  u8  bootver;              //硬件版本号
  u8  fileCRC[2];           //文件校验
}scustomInfo;

typedef struct UpgradeDataInfo
{
  u8  ProducerMark[2];      //厂家标识 本公司默认为"G","D"
  u8  nodeType;             //设备类型
  u8  totalPackages[2];     //总包数
  u8  currentPackageNo[2];  //当前包序号
  scustomInfo sCusInfo;
}sUpgradeDataInfo;

typedef struct AppDataRelay  
{
  struct
  {
    volatile u32 switchON               :1;
    volatile u32 waitMeterRpsSwitch     :1;
    volatile u32 waitMeterRpsOverTimer  :1;
    volatile u32 waitCACRpsSwitch       :1;
    volatile u32 waitCACRpsOverTimer    :1;
    volatile u32 waitCACRpsSuccess      :1;
    volatile u32 mulFrameRecSwitch      :1;
    volatile u32 mulFrameRecOverTimer   :1;
    volatile u32 timer                  :24;
    volatile u32 mulFrameTransSwitch    :1;
    volatile u32 sendRetryCnt           :3;
    volatile u32                        :4;
    volatile u32 maxTimer               :24;
  }timeCtl;

  u8 appRecvFrameNumber;
  u8 recvChannelArrPos;
  u8 apsDataRelayMaxLen;
  u8 DTI;
  sNwkRecvFrmaeHeadInfo sNwkRecvHeadInfo;
  sMAC_FrameHead_6      sMACRxHead;
}AppDataRelay_st;

#pragma pack()

void  SgdAppParseFrame( u8 *buf, u16 len );
void  SgdAppParseConfirm_DenyFrame( u8 *buf, u16 len );
void  SgdAppParseCommdFrame( u8 *buf, u16 len );
void  SgdAppDataRelayFrame( u8 *buf, u16 len );
void  SgdAppReportFrame( u8 *buf, u16 len );

void  SgdAppFormReportFrame( u8 *pInData, u8 inLen );
void  SgdAppFormDataRelayFrame( u8 *pInData, u8 inLen );
void  SgdAppRelayBroadcastFrame( u8 *buf, u16 len );

u8  SgdAppUartConfig( u8 *buf, u8 len );
u8  SgdAppSetChannelGroup( u8 *buf, u8 len );
u8  SgdAppSetRssiThreshold( u8 *buf, u8 len );
u8  SgdAppSetTxPower( u8 *buf, u8 len );
u8  SgdAppRebootDev( u8 *buf, u8 len );
u8  SgdAppUpGradeDev( u8 *buf, u8 len );
u8  SgdAppSetVenderAndVer( u8 *buf, u8 len );
u8  SgdAppSetLongAddr( u8 *buf, u8 len );

void  SgdAppGetNodeConfigInfo(void);
void  SgdAppBroadcastTime( u8 *buf, u16 len );

void  SgdAppFormConfirm_DenyFrame( u8 status  );

void  SgdAppUpdateSequenceNum(void);

void  SgdAppParseExtendCI( u8 *buf, u16 len );

u8    SgdAppGetInputPower( u8 *buf, u8 len );

void  SgdAppReplyInputPowerForWL(void);

void  SgdAppReplyRssiThreshold(void);

void  SgdAppReplySystemUpTime(void);

void  SgdRelayAPSAlarmBoxRespond( u8 *pInBuff, u16 inLen );

void  SgdRelayAPSAlarmBoxReported( u8 *pInBuff, u16 inLen );

void  SgdAppReplyBeaconTE(void);

u32 SgdAppGetInRechargeCnt( u8 *buff, u8 len );

u32 SgdAppGetMeterRechargeCnt(void);

u8  SgdAppFormRechargeRespond( u8 *buff );

void  SgdApsTestCommand(u8 *inDat,u16 inLen);
#endif