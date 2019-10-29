#ifndef _MAC_H
#define	_MAC_H

#include <time.h>
#include "Rf_common.h"
#include "phy.h"
#include "fifo_API.h"

#define   MAC_FCD_LEN               2
#define   MAC_SEQUENCE_NUMBER_LEN   1
#define   MAC_PANID_LEN             2

#define     aMaxFrameRetries    3
#define     aMaxRankCount       8
#define     aMaxRoundCount      8
#define     aUnitBackoffPeriod  10
#define     macAckWaitDuration  300
#define     aTifs               30

#pragma pack(1)

extern u8   macSequenceNumber;

enum  MacFrameType
{
  MacBeaconFrame = 0x00,
  MacDataFrame,
  MacConfirmFrame,
  MacCommdFrame,
  MacBroadFrame,
  MACFeelNetBroadFrame
};

enum  MacCommandID
{
  NetworkMaintenanceRequest = 0x01,
  NetworkMaintenanceRespond = 0x02,
  AlarmBoxDataRequest = 0x10,
  AlarmBoxRespond = 0x11,
  AlarmBoxReported = 0x12,
  AlarmBoxReportedRespond = 0x13,
  BroadcastCall = 0xF0,
  WaterMeterUpData = 0x50, 
  WaterMeterUpRespond = 0x51,
  MACupgrade = 0xE0,
  MACupgradeVersionCheck=0xE1
};

typedef struct MacFCD
{
  u8  frameType:3;
  u8  secure:1;
  u8  frameHold:1;
  u8  confirm:1;
  u8  PanID:1;
  u8  bit7:1;
  u8  SN:1;
  u8  expandInfo:1;
  u8  dstAddrMode:2;
  u8  frameVer:2;
  u8  srcAddrMode:2;
}sMacFCD;

//MAC层寻址域为6字节长地址的帧头结构
typedef struct MAC_FrameHead_6
{
  sMacFCD sFCD;
  u8  SN;
  u8  PanID[2];
  u8  dstAddr[6];
  u8  srcAddr[6];
}sMAC_FrameHead_6;

//信标帧载荷
typedef struct macBeaconPayload
{
  u8  txDelay;            //发送随即延时
  u8  roundCnt;           //轮次次数
  u8  TS_LevelNum[2];     //时隙号以及层次号
  u8  beaconMark;         //信标标识
  u8  networkScale[2];    //网络规模
  u8  rssiThreshold;      //场强门限
  u8  centrePanID[2];     //中心节点PanID
  u8  centreAddr[6];      //中心节点地址
}sMacBeaconPayload;       

#define MAC_BEACON_PARAM_FIFO 3
typedef struct macBeaconParam
{
  u8  networkScale[2];    //网络规模
  u8  rssiThreshold;      //场强门限
  u8  centrePanID[2];     //中心节点PanID
  u8  centreAddr[6];      //中心节点地址
}sMacBeaconParam;

//感知扩展帧载荷
typedef struct macFeelExpdPayload
{
  u8  cmdId;             //命令标识
  u8  cmdNo;             //命令序号
  u8  broadId;           //广播标识
  u8 currTmSlotNo[2];    //当前时隙号
  u8 TmSlotNm[2];        //时隙数
// FlExpdTmSlotNmType  TmSlotNm;    //时隙数
  u8 netStsData[50];
}macFeelExpdPayloadType; 

typedef struct  MAC_CMDPayloadRoute
{
  u8  count:4;
  u8  index:4;  
}sMAC_CMDPayloadRoute;

extern sMAC_FrameHead_6 sMACRxHead;

typedef struct  MAC_BroadFramePayload
{
  u8  txDelay;
  u8  timeSlotNum[2];
  u8  broadMark;
  u8  meshScale[2];
  u8  waterMeterScale[2];
  u8  waterMeterRSSITH;
}sMAC_BroadFramePayload;

typedef struct  BroadcastCallPayload
{
  u8  commdMark;          //命令标识
  u8  txDelay;            //随机延时
  u8  broadMark;          //广播标识
  u8  relayIndex;         //转发索引
  u8  recvRssi;           //接收信号强度
  u8  workGroupNum;       //工作信道组号
  u8  ES_Ver[3];          //外部版本号
  u8  IS_Ver[2];          //内部版本号
  u8  timeSlotNum[2];     //当前转发时隙号
  u8  networkScale[2];    //转发网络规模
}sBroadcastCallPayload;

typedef struct RxPackageRecord
{
  u16 upGradeOverTimer;
  bool apsUpgrade;           //是否为APS层升级帧
  bool macUpgrade;           //是否为MAC升级帧 
  bool feelFlag;
  bool upgradeCheckFlag;
  u8  upGradeStatue;         //表示升级完成，1表示正在升级  
  u16 recvPackageNum;       //接收包总数
  u16 totalPackages;        //总包数
  u16 recvLen;              //接收长度
  u8  RxPackageRec[50];     //接收记录表
}sRxPackageRecord;

typedef struct feelUpgradeStatue_st
{
  u8 len;
  u8 ProducerName[2];
  u8 expandCmd;
  
  u8 upGradeType;
  u8 softWareVer[3];
  u16 count;
}feelUpgradeStatue;


#define ONE_PACKAGE_MAX_LEN  168 //升级数据包长度（再计算）
typedef struct upGradDat
{
  u8  totalPackages[2];       //当前包序号（从1开始）
  u8  currentPackageNo[2];          //总包数（不包含启动帧）
  struct
  {
    u8 low4bit :4;              //升级类型低4位：1-升级boot,2-升级app，3-升级app以及boot,其他值保留。
    u8 high4bit:4;             //高4位b4-b7:0-列表升级，1-邻居节点升级,2-邻居节点指定列表升级，3-层次升级,4-列表全网升级;
  }UpdateType; 
  u8  nodeType;                //0-中心节点（13版），1-I采(13版)，2-II采(13版)，3-单相表模块(13版)。
                               //4-中心节点（09版）， 5-I采(09版)，6-II采(09版)，7-单相表模块(09版)。  8-三相表(09版)，9-三相表(13版)。	5 - 13二采 6-集中器模块
  u8  UpGradeChannelGroupIndex;  //升级信道组号
  u8  fileCRC[2];              //文件校验
  u8  len;
  u8  dat[ONE_PACKAGE_MAX_LEN];
}upGradDat_st;

#define ONE_PAGE_ADDR    168
enum  upGradeType
{
  listUpgrad = 0,
  neiUpgrad,
  neiListUpgrad,
  layerUpgrad,
  allUpgrad
};

typedef struct backUpgradPara
{
  struct
  {
    u8 low4bit :4;              //升级类型低4位：1-升级boot,2-升级app，3-升级app以及boot,其他值保留。
    u8 high4bit:4;             //高4位b4-b7:0-列表升级，1-邻居节点升级,2-邻居节点指定列表升级，3-层次升级,4-列表全网升级;
  }UpdateType;                
  u8  fileCRC[2];              //文件校验
  u8  bootSize[2];            //bootloader程序大小 (同时升级bootloader和APP时指定)
}backUpgradPara_st;

typedef struct upGradStart
{
  u8 upGradeOverTimer[2];             //升级时间
  u8  totalPackages[2];        //总包数（不包含启动帧）
  u8  currentPackageNo[2];     //当前包序号（固定为0）
  struct
  {
    u8 low4bit :4;              //升级类型低4位：1-升级boot,2-升级app，3-升级app以及boot,其他值保留。
    u8 high4bit:4;             //高4位b4-b7:0-列表升级，1-邻居节点升级,2-邻居节点指定列表升级，3-层次升级,4-列表全网升级;
  }UpdateType;                 
  u8  nodeType;                 //0-中心节点（13版），1-I采(13版)，2-II采(13版)，3-单相表模块(13版)。 4-中心节点（09版）， 
                                //5-I采(09版)，6-II采(09版)，7-单相表模块(09版)。  8-三相表(09版)，9-三相表(13版)。
  u8  UpGradeChannelGroupIndex;//升级信道组号
  u8  HardwareVersion[2];      //硬件版本号
  u8  BootloaderVersion;       //Bootloader 版本号
  u8  SoftwareVersion[2];      //软件版本
  u8  fileCRC[2];              //文件校验
  u8  layerCnt;                //升级类型为层次升级时，指定层次号，子节点在网，最大为7
  u8  bootSize[2];            //bootloader程序大小 (同时升级bootloader和APP时指定)
  //u8  addrListIndex[2];       //地址列表指示：[0]指示当前页数，[1]指示总页数。
  u8  intervalVer[2];         //内部版本不同也支持升级
  u8  addrListCnt;            //地址列表个数
  u8  addrList[ONE_PAGE_ADDR];//地址列表
}upGradStart_st;

#define MACexpandInfoLoadLen sizeof(upGradStart_st)
enum  MacExpandCmd
{
  UpgradStart = 0x00,
  UpgradData  = 0x01,
  getUpgradResult = 0x02
};

typedef struct MACexpandInfo
{
  u8 len;
  u8 ProducerName[2];
  u8 expandCmd;
  u8 MACexpandInfoLoad[MACexpandInfoLoadLen];

}MACexpandInfo_st;
extern MACexpandInfo_st *gMACexpandInfo;


#pragma pack()

void    SgdParseMacFrame( u8 *buf, u16 len );

bool 	SgdMacDoCSMA_CA( u8 rssiThreshold );

void	SgdParseMACBeaconFrame( u8 *buf, u16 len );

void	SgdParseMACDataFrame( u8 *buf, u16 len );

void	SgdParseMACConfirmFrame( u8 *buf, u16 len );

void    SgdParseMACCommandFrame( u8 *buf, u16 len );

void    SgdParseMACBroadFrame( u8 *buf, u16 len );

void	SgdFormMACNetworkMaintenanceRespond( u8 *pInbuf, u16 inLen );

void	SgdRelayNetworkMaintenance( u8 *buf, u16 len );

void 	SgdMacFormAndSendConfirm( u8 *buf, u16 len );

void    SgdMacFormDataFrame( sBuffer *pBuff );

void    SgdMacFormRelayBeaconFrame( u8 *buf, u16 len, u8 *ts_levelNum );

void    SgdMacUpdateDSN(void);

void    SgdEnableSysTimer( u32 time );

void    SgdDisableSysTimer(void);

void  SgdRelayMACAlarmBoxDataRequest( u8 *pInBuff, u16 inLen );

void  SgdRelayMACAlarmBoxReportedRespond( u8 *pInBuff, u16 inLen );

bool  SgdRelayMACBroadFrame( sMAC_BroadFramePayload *pPayload );
  
void  SgdParseMACFeelNetBroadFrame( u8 *buf, u16 len );

void  SgdRelayMACBroadcastCallCommd( u8 *pInBuff, u16 inLen,u8 command );

sMacBeaconParam * sMacSaveBeaconParam(u8 *panID);

void relayPlcPowerDownFrame(u8 *inDat,u8 inLen);

#endif

