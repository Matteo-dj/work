#ifndef _NWK_H
#define	_NWK_H

#include "mac.h"
#include "fifo_API.h"

#define ONE_PAGE_NEIGHBOURS  16
#define WART_METER_PAGE_SIZE  5
#define TOTAL_WART_METER_SIZE 32 //CJJ-2018-5-15:���֧��32������ˮ������

//�ռ�����������Ӧ֡�����ʱ��
#define   waitJoinNetRespondMaxTime     ((u32)(64*aBaseSlotDuration))
//����ڵ�ȴ�������ʱʱ�䣨3Сʱ
#define   nwkNetworkFormationWaitTime   ((u32)3*60*60*1000)

//����ڵ��ڸ��ŵ�����������������֡�ļ����180s��
#define   nwkOrphanApplyInterval        ((u32)180*1000)

//�������ڵ���δ����������������κ�֡ʱ��������̬��ʱ�䣨36Сʱ����ͬʱҲ��Ϊ�ڵ�����ʱ��
#define   nwkOrphanInterval             ((u32)36*60*60*1000)

#pragma pack(1)

enum	NwkFrameType
{
  DataFrame = 0x00,
  CommdFrame
};

enum  NwkCommands
{
  JOIN_NETWORK_REQUEST = 0x01,
  JOIN_NETWORK_RESPOND = 0x02,
  ROUTE_ERROR = 0x03,
  RSSI_COLLECT_REQUEST = 0x10,
  RSSI_COLLECT_RESPOND = 0x11,
  CONFIG_SUB_NODE_REQUEST = 0x12,
  CONFIG_SUB_NODE_RESPOND = 0x13,
  FREE_NODE_READY = 0x16,
  COLLECT_WATER_DATA_REQUEST = 0X0A,
  COLLECT_WATER_DATA_RESPOND = 0X0B,
  LPM_RSSI_COLLECT_REQUEST = 0x0C, //LPM:LowPowerMeter
  LPM_RSSI_COLLECT_RESPOND = 0x0D 
};

typedef struct nwkFCD
{
  u8  frameType:2;
  u8  dstAddrMode:2;
  u8  srcAddrMode:2;
  u8  bit6:1;
  u8  routeInfo:1;
}sNwkFCD;

typedef struct Nwk_FrameNum_Radius
{
  u8  radius:4;
  u8  frameNum:4;
}sNwk_FrameNum_Radius;

typedef struct NwkRouteInfo
{
  u8  relayCount:5;       //�м̽ڵ���
  u8  relayIndexL3bit:3;  //�м�������3λ
  u8  relayIndexH2bit:2;  //�м�������2λ
  u8  relay1AddrMode:2;
  u8  relay2AddrMode:2;
  u8  relay3AddrMode:2;
  u8  relay4AddrMode:2;
  u8  relay5AddrMode:2;
  u8  relay6AddrMode:2;
  u8  RES:2;
}sNwkRouteInfo;

//���ڱ������֡��֡ͷ��Ϣ�����ڹ�����Ӧ֡
typedef struct NwkRecvFrmaeHeadInfo
{
  sNwkFCD  sFCD;
  u8  headLen;
  u8  dstAddrLen;
  u8  srcAddrLen;
  u8  dstAddr[6];
  u8  srcAddr[6];
  sNwk_FrameNum_Radius sFrameNum_Radius;
  sNwkRouteInfo sRouteInfo;
  u8  relayAddr[36];
}sNwkRecvFrmaeHeadInfo;

extern  sNwkRecvFrmaeHeadInfo sNwkRecvHeadInfo;


typedef struct ConfigSubNOdeCommdOptions
{
  u8 timeSlotNum:1;
  u8 levelNum:1;
  u8 channelGroupNum:1;
  u8 shortAddr:1;
  u8 panID:1;
  u8 relayList:1;
  u8 feelExpd:1;
  u8 inNet:1;
}sConfigSubNOdeCommdOptions;

//����������Ӧ֡���������غ�
typedef struct
{	
  u8 	CommType;           //���������ʶ
  u8 	CommOption;         //����ѡ��
  u8 	PanID[2];	    //���ڵ����������PanID
  u8 	CenterAddr[6];	    //���Ľڵ��ַ
  u8    TSNum[2];           //ʱ϶�Ų�κ�
  u8    RssiValue;          //��ǿֵ
  u8    RelayCnt;	    //�м���
}sNwkCommRespondPayload;    

//�˽ṹ�����������յ�����������Ӧ֡ʱ�������Ϣ
typedef struct sNwkJoinNetRespondInfo
{
  u8    SrcAddr[6];        //����������Ӧ֡��Դ��ַ
  u8    PanID[2];          //�������PanID
  u8    CentreAddr[6];     //���Ľڵ��ַ
  u8    LevelNum;          //��κ�
  u8    RssiValue;         //��ǿֵ
  u8 	RelayCnt;          //�м̽ڵ����
  u8 	ReAddrList[12];    //�м̽ڵ��ַ�б�ÿ���м̽ڵ�ĵ�ַΪ�̵�ַ2�ֽڣ����6���м̽ڵ�
}sNwkJoinNetRespondInfo;    

#define IN_NET_REQUEST_NUM 3
//typedef struct
//{
//  sNwkJoinNetRespondInfo *front;   //������ָ��
//  sNwkJoinNetRespondInfo *rear;    //����βָ��
//}sNwkInNetInfoLK;

typedef struct nwkJionNetRequestFrame
{
  sNwkFCD sFCD;
  u8  dstAddr[6];
  u8  srcAddr[6];
  sNwk_FrameNum_Radius sSN_Radius;
  u8 	CommType;           //���������ʶ
  u8 	CommOption;         //����ѡ��  
}sNwkJionNetRequestFrame;

#define MAX_JOINT_RESPOND_NODE 5

extern FIFO    joinNetFifoCtr;

typedef struct joinNetResondCtr_type
{
  volatile bool relaySwitch;
  volatile bool realyFlag;
  volatile u32  delayTime;

  u8   recvChannelArrPos; 
  u8   inputPower;
  sNwk_FrameNum_Radius sFrameNum_Radius; 
  u8  srcAddr[6];
}joinNetResondCtr_st;

typedef struct 
{ 
  joinNetResondCtr_st     joinNetResondCtr[MAX_JOINT_RESPOND_NODE+1];
}joinNetResondSave_st;

#pragma pack()
void  SgdNwkInit(void);

void  SgdNwkParseFrame( u8 *buf, u16 len );

bool  SgdParseNwkFrameHead( u8 *buf, u16 len );

void  SgdNwkParseDataFrame( u8 *buf, u16 len );

void  SgdNwkParseCommFrame( u8 *buf, u16 len );

void  SgdNwkFormReadyFrame(void);

void  SgdNwkFormReadyFrameNoRoute( sNwkJoinNetRespondInfo *pInfo );

void  SgdNwkFormReadyFrameRoute( sNwkJoinNetRespondInfo *pInfo );

void  SgdNwkFormRSSIRespond( u8 pageIndex );

void SgdNwkFormRelayFrame( u8 *buf, u16 len );

void  SgdNwkConfigNodeRespond(void);

void  SgdNwkCollectJoinNetRespondInfo( u8 *buf, u16 len );

void  SgdNwkSaveNodeInfo( u8 *buf, u16 len );

void  SgdNwkFreeInNetInfoLK(void);

void  SgdNwkFreeNeighbourList(void);

void  SgdNwkFormDataFrame( sBuffer *pBuffer );

void  SgdNwkJoinNetWorkRequest(void);

void  SgdNwkFormJoinNetRespondFrame( void );

void  SgdNwkGetBestPath( u8 *buf, u8 *len );

void  SgdNwkFormRouteErrorFrame(void);

void  SgdNwkUpdateSequenceNum(void);

void  SgdNwkReverseRelayAddr( u8 *relayAddr, u8 relayCnt, u8 addrMode );

void  SgdNwkGetNeighbours( u8 *buf, u8 *len, u8 nodeNum, u8 pageIndex );

void  SgdNwkSetRelayAddrMode( sNwkRouteInfo *pRoute, u8 addrMode );

void  SgdNwkFormLowPowerMeterRSSIRespond( u8 pageIndex );

void  SgdNwkGetLowPowerMeterNeighbours( u8 *buf, u8 *len, u8 nodeNum, u8 pageIndex );

void  SgdNwkGetWaterMeterData( u8 *buf, u8 *len, u8 nodeNum, u8 pageIndex );

void joinNetResond(joinNetResondCtr_st *pTmp);

#endif

