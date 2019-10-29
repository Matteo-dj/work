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

//MAC��Ѱַ��Ϊ6�ֽڳ���ַ��֡ͷ�ṹ
typedef struct MAC_FrameHead_6
{
  sMacFCD sFCD;
  u8  SN;
  u8  PanID[2];
  u8  dstAddr[6];
  u8  srcAddr[6];
}sMAC_FrameHead_6;

//�ű�֡�غ�
typedef struct macBeaconPayload
{
  u8  txDelay;            //�����漴��ʱ
  u8  roundCnt;           //�ִδ���
  u8  TS_LevelNum[2];     //ʱ϶���Լ���κ�
  u8  beaconMark;         //�ű��ʶ
  u8  networkScale[2];    //�����ģ
  u8  rssiThreshold;      //��ǿ����
  u8  centrePanID[2];     //���Ľڵ�PanID
  u8  centreAddr[6];      //���Ľڵ��ַ
}sMacBeaconPayload;       

#define MAC_BEACON_PARAM_FIFO 3
typedef struct macBeaconParam
{
  u8  networkScale[2];    //�����ģ
  u8  rssiThreshold;      //��ǿ����
  u8  centrePanID[2];     //���Ľڵ�PanID
  u8  centreAddr[6];      //���Ľڵ��ַ
}sMacBeaconParam;

//��֪��չ֡�غ�
typedef struct macFeelExpdPayload
{
  u8  cmdId;             //�����ʶ
  u8  cmdNo;             //�������
  u8  broadId;           //�㲥��ʶ
  u8 currTmSlotNo[2];    //��ǰʱ϶��
  u8 TmSlotNm[2];        //ʱ϶��
// FlExpdTmSlotNmType  TmSlotNm;    //ʱ϶��
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
  u8  commdMark;          //�����ʶ
  u8  txDelay;            //�����ʱ
  u8  broadMark;          //�㲥��ʶ
  u8  relayIndex;         //ת������
  u8  recvRssi;           //�����ź�ǿ��
  u8  workGroupNum;       //�����ŵ����
  u8  ES_Ver[3];          //�ⲿ�汾��
  u8  IS_Ver[2];          //�ڲ��汾��
  u8  timeSlotNum[2];     //��ǰת��ʱ϶��
  u8  networkScale[2];    //ת�������ģ
}sBroadcastCallPayload;

typedef struct RxPackageRecord
{
  u16 upGradeOverTimer;
  bool apsUpgrade;           //�Ƿ�ΪAPS������֡
  bool macUpgrade;           //�Ƿ�ΪMAC����֡ 
  bool feelFlag;
  bool upgradeCheckFlag;
  u8  upGradeStatue;         //��ʾ������ɣ�1��ʾ��������  
  u16 recvPackageNum;       //���հ�����
  u16 totalPackages;        //�ܰ���
  u16 recvLen;              //���ճ���
  u8  RxPackageRec[50];     //���ռ�¼��
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


#define ONE_PACKAGE_MAX_LEN  168 //�������ݰ����ȣ��ټ��㣩
typedef struct upGradDat
{
  u8  totalPackages[2];       //��ǰ����ţ���1��ʼ��
  u8  currentPackageNo[2];          //�ܰ���������������֡��
  struct
  {
    u8 low4bit :4;              //�������͵�4λ��1-����boot,2-����app��3-����app�Լ�boot,����ֵ������
    u8 high4bit:4;             //��4λb4-b7:0-�б�������1-�ھӽڵ�����,2-�ھӽڵ�ָ���б�������3-�������,4-�б�ȫ������;
  }UpdateType; 
  u8  nodeType;                //0-���Ľڵ㣨13�棩��1-I��(13��)��2-II��(13��)��3-�����ģ��(13��)��
                               //4-���Ľڵ㣨09�棩�� 5-I��(09��)��6-II��(09��)��7-�����ģ��(09��)��  8-�����(09��)��9-�����(13��)��	5 - 13���� 6-������ģ��
  u8  UpGradeChannelGroupIndex;  //�����ŵ����
  u8  fileCRC[2];              //�ļ�У��
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
    u8 low4bit :4;              //�������͵�4λ��1-����boot,2-����app��3-����app�Լ�boot,����ֵ������
    u8 high4bit:4;             //��4λb4-b7:0-�б�������1-�ھӽڵ�����,2-�ھӽڵ�ָ���б�������3-�������,4-�б�ȫ������;
  }UpdateType;                
  u8  fileCRC[2];              //�ļ�У��
  u8  bootSize[2];            //bootloader�����С (ͬʱ����bootloader��APPʱָ��)
}backUpgradPara_st;

typedef struct upGradStart
{
  u8 upGradeOverTimer[2];             //����ʱ��
  u8  totalPackages[2];        //�ܰ���������������֡��
  u8  currentPackageNo[2];     //��ǰ����ţ��̶�Ϊ0��
  struct
  {
    u8 low4bit :4;              //�������͵�4λ��1-����boot,2-����app��3-����app�Լ�boot,����ֵ������
    u8 high4bit:4;             //��4λb4-b7:0-�б�������1-�ھӽڵ�����,2-�ھӽڵ�ָ���б�������3-�������,4-�б�ȫ������;
  }UpdateType;                 
  u8  nodeType;                 //0-���Ľڵ㣨13�棩��1-I��(13��)��2-II��(13��)��3-�����ģ��(13��)�� 4-���Ľڵ㣨09�棩�� 
                                //5-I��(09��)��6-II��(09��)��7-�����ģ��(09��)��  8-�����(09��)��9-�����(13��)��
  u8  UpGradeChannelGroupIndex;//�����ŵ����
  u8  HardwareVersion[2];      //Ӳ���汾��
  u8  BootloaderVersion;       //Bootloader �汾��
  u8  SoftwareVersion[2];      //����汾
  u8  fileCRC[2];              //�ļ�У��
  u8  layerCnt;                //��������Ϊ�������ʱ��ָ����κţ��ӽڵ����������Ϊ7
  u8  bootSize[2];            //bootloader�����С (ͬʱ����bootloader��APPʱָ��)
  //u8  addrListIndex[2];       //��ַ�б�ָʾ��[0]ָʾ��ǰҳ����[1]ָʾ��ҳ����
  u8  intervalVer[2];         //�ڲ��汾��ͬҲ֧������
  u8  addrListCnt;            //��ַ�б����
  u8  addrList[ONE_PAGE_ADDR];//��ַ�б�
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

