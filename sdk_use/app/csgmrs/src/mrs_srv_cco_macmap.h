//*****************************************************************************
//
//                  版权所有 (C), 1998-2011, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_srv_cco_macmap.h
//  版 本 号   : V1.0 
//  作    者   : liuxipeng/KF54842
//  生成日期   : 2011-12-13
//  功能描述   : TODO: ...
//               
//  函数列表   : TODO: ...
//  修改历史   : 
//  1.日    期 : 2011-12-13
//    作    者 : liuxipeng/KF54842
//    修改内容 : 创建文件 
//
//*****************************************************************************
#ifndef __MRS_SRV_CCO_MACMAP_H__
#define __MRS_SRV_CCO_MACMAP_H__

HI_START_HEADER

//内部维护一个映射表CCO/MacMeterMap
#define MRS_MAC_DEVICE_MAP_NUM_MAX              (2448) //映射关系最大容量
#define MRS_MAC_CLT_MAP_NUM_MAX                 (2400)

#define MRS_MAC_MAP_TTL_MIN                     (1)
#define MRS_MAC_MAP_TTL_DEFAULT                 (15)
#define MRS_MAC_MAP_TTL_MAX                     (50)
#define MRS_MAC_MAP_CHECK_TTL_TIME_MIN          (1)   //单位为s
#define MRS_MAC_MAP_CHECK_TTL_TIME_DEFAULT      (2 * 60)   //单位为s
#define MRS_MAC_MAP_CHECK_TTL_TIME_MAX          (2 * 60 * 60)   //单位为s
#define MRS_MAC_MAP_MRS_TTL_MIN                 (1)
#define MRS_MAC_MAP_MRS_TTL_DEFAULT             (15)
#define MRS_MAC_MAP_MRS_TTL_MAX                 (50)

#define MRS_MAC_MAP_SEND_PLC_TIME_DEFAULT       (2000) //CCO端发送读表地址帧默认时间间隔
#define MRS_MAC_MAP_SEND_PLC_TIME_MAX           (1000 * 60) //CCO端发送读表地址帧最大时间间隔
#define MRS_MAC_MAP_SEND_PLC_TIME_MIN           (50) //CCO端发送读表地址帧最小时间间隔
#define MRS_MAC_MAP_SEND_LAST_PLC_TIME_DEFAULT  (1000 * 3) //CCO端发送最后一帧读表地址帧默认时间间隔
#define MRS_MAC_MAP_SEND_LAST_PLC_TIME_MAX      (1000 * 60) //CCO端发送最后一帧读表地址帧最大时间间隔
#define MRS_MAC_MAP_SEND_LAST_PLC_TIME_MIN      (50) //CCO端发送最后一帧读表地址帧最小时间间隔
#define MRS_MAC_MAP_SEND_PLC_NUM_DEFAULT        (5)    //CCO端下发读表地址帧时，一次性下发的数量默认值
#define MRS_MAC_MAP_SEND_PLC_NUM_MAX            (100)  //CCO端下发读表地址帧时，一次性下发的数量最大值
#define MRS_MAC_MAP_SEND_PLC_NUM_MIN            (1)    //CCO端下发读表地址帧时，一次性下发的数量最小值

#define MRS_MAC_MAP_SEND_PLC_RETRY_DEFAULT      (5)    // 发送读取电表地址帧重试次数默认值
#define MRS_MAC_MAP_SEND_PLC_RETRY_MAX          (100)  // 发送读取电表地址帧重试次数最大值
#define MRS_MAC_MAP_SEND_PLC_RETRY_MIN          (0)    // 发送读取电表地址帧重试次数最小值

#define MRS_MAX_MAP_CREATE_USED_TIME            (1000 * 60 * 30)    // 映射表最大创建所需时长

#define MRS_MAC_MAP_REFRESH_TIME_DEFAULT        (60 * 5)  // CCO端重新刷新映射表间隔时间默认值(单位S)
#define MRS_MAC_MAP_REFRESH_TIME_MAX            (60 * 60) // CCO端重新刷新映射表间隔时间最大值(单位S)
#define MRS_MAC_MAP_REFRESH_TIME_MIN            (10)      // CCO端重新刷新映射表间隔时间最小值(单位S)

/* BEGIN: Modified by cuichunyan/00276962, 2016/7/28   问题单号:   DTS2016072603716  */ 
#define MRS_MAC_MAP_MAX_REFRESH_NUM_DEFAULT     (7)  // CCO端重新刷新映射表间隔时间默认值(单位S)
/* END:   Modified by cuichunyan/00276962, 2016/7/28  */

#define MRS_MAC_MAP_MAX_REFRESH_NUM_MAX         (100) // CCO端重新刷新映射表间隔时间最大值(单位S)
#define MRS_MAC_MAP_MAX_REFRESH_NUM_MIN         (0)      // CCO端重新刷新映射表间隔时间最小值(单位S)

#define MRS_MAP_MAINTAIN_MODE_ACTIVE            (1)    // 映射表维护模式(主动)-搜表等驱动
#define MRS_MAP_MAINTAIN_MODE_PASSIVE           (2)    // 映射表维护模式(被动)-Topo变更驱动
#define MRS_MAP_MAINTAIN_MODE_REFRESH           (3)    // 映射表维护模式(补充刷新)-定时器驱动

#define MRS_MAP_REPORT_IDLE                     (0)
#define MRS_MAP_REPORT_SUCCESS                  (1)
#define MRS_MAP_REPORT_CONTINUE                 (2)

//电表信息
typedef struct _MRS_METER_INFO
{
    HI_U8   ucMeterAddr[HI_METER_ADDR_LEN];   //电表地址,6字节
    HI_U8   ucProtocol;    //规约类型   
    HI_U8   ucDeviceType;   //设备类型
    HI_U32  usSendFrame;	// 当前节点(电表)发送645帧次数
    HI_U32  usRecvFrame;    // 当前节点(电表)接收645帧次数
} MRS_METER_INFO;


//MAC地址信息
typedef struct _MRS_MAC_INFO
{
    HI_U8 ucMacAddr[HI_PLC_MAC_ADDR_LEN];   //MAC地址,6字节，放在最前面
    HI_U8 report;    //上报标志  0-未上报   1-已上报成功  2-仍需上报,是采集器在搜表状态的上报
    HI_U8 retries;        //重试次数
} MRS_MAC_INFO;

typedef struct
{
    HI_U8 aucAddr[HI_METER_ADDR_LEN];
    HI_U8 ucProtocol;
    HI_U8 ucType : 4;
	HI_U8 ucStatus : 2; //采集器当前搜表状态 0-搜表完成 1-搜表中 2-未知
    HI_U8 reserved: 2;
} MRS_CCO_DEVICE_INF_STRU;

// MAC-METER
typedef struct _MRS_MAC_DEVICE_PAIR
{
    HI_U16  bValid : 1;
    HI_U16  usTTL  : 6;     /* Mac-meter map TTL. CNcomment: 映射关系TTL. CNend */
    HI_U16  usMrTTL: 6;     /* Meter-reading TTL. CNcomment: 抄表TTL.     CNend */
    HI_U16  pad    : 3;
    HI_U8   aucMacAddr[HI_PLC_MAC_ADDR_LEN];    // MAC地址
    MRS_CCO_DEVICE_INF_STRU     stDeviceInf;        // 设备信息
} MRS_MAC_DEVICE_PAIR;


//映射表信息
typedef struct _MRS_MAC_DEVICE_MAP
{
    HI_U16 usNumber;              //数量
    HI_U16 usMapSize;
    MRS_MAC_DEVICE_PAIR * pstItem;  //具体元素
} MRS_MAC_DEVICE_MAP;


typedef struct _MRS_MAC_LIST
{
    HI_S16 number;
    HI_U16 current;     //当前维护的MAC
    MRS_MAC_INFO *mac;  //mac列表
} MRS_MAC_LIST;


//映射表模块
typedef struct _MRS_MAP_MODULE
{
    HI_BOOL bInit;                       //是否初始化
    HI_U8 ucState;                      //状态
    HI_U8 ucChanges;                    //当前频段TOPO改变次数
    HI_U8 ucMapEnable;                  //CCO端映射表维护使能开关,默认为开
    HI_U16 usSendMacPlcTime;            //CCO端发送读表地址帧时间间隔
    HI_U16 usSendLastMacPlcTime;        //CCO端发送最后一个Mac读表地址PLC帧后的时间间隔
    HI_U8 ucMaxParallelnum;             // 发送读取电表地址帧最大并发数
    HI_U8 ucMaxRetryNum;                // 发送读取电表地址帧最大重试次数
    HI_U16 usCheckTtlTime;              //刷新映射表TTL的时间间隔,单位为s
    HI_U32 first_notify;
    HI_U16 map_turn_time;
    HI_U8 max_turn_num;
    HI_U8 curr_turn;
    HI_U16 usMaxTtl;
    HI_U8 ucCltMapEnable;
    HI_U8 ucMaxMrsTtl;
    MRS_MAC_LIST stNewMacList;          // 新增的MAC列表

    MRS_MAC_DEVICE_MAP stMacMeterMap;   // 映射表
    MRS_MAC_DEVICE_MAP stMacCltMap;     // 采集器映射表,当ucCltMapEnable为0时,pstItem为空,用到时要判空
} MRS_MAP_MODULE;


//初始化映射表模块
HI_U32 mrsCcoMapInit(MRS_MAP_MODULE *pstModule);

//逆初始化映射表模块
HI_U32 mrsMapUnInitModule(MRS_MAP_MODULE *pstModule);

//清空映射表包括New Mac List
HI_U32 mrsMapEmptyModule(MRS_MAP_MODULE *pstModule);

//根据MAC地址删除表项
HI_U32 mrsMapDeleteMac(MRS_MAP_MODULE *pstModule, HI_U8 *pucMacAddr);

//标记New Mac List里MAC已经上报上来了
HI_U32 mrsMapMarkMac(MRS_MAP_MODULE *pstModule, HI_U8 *pucMacAddr, HI_U8 ucStatus);

//获取电表对应MAC
HI_U32 mrsMapGetMAC(HI_U8 *pucDeviceAddr, HI_U8 *pucMacAddr);

//添加MAC-METER映射(或者说是更新)
HI_U32 mrsMapAddMeter(MRS_MAP_MODULE *pstModule, HI_U8 *pucMacAddr, MRS_DEVICE_INF_STRU *pstMeterInf);
HI_U32 mrsMapAddCollector(MRS_MAP_MODULE *pstModule, HI_U8 *pucMacAddr, MRS_DEVICE_INF_STRU *pstCltInf);

//TOPO改变，重新检查映射表
HI_U32 mrsMapRecheckMacMeter(MRS_MAP_MODULE *pstModule, HI_MAC_NETWORK_TOPO_ENTRY_S * pstEntry, HI_U16 usNum);

//MAP维护函数
HI_U32 mrsMapMaintain(MRS_MAP_MODULE *pstModule);

//MAP超时处理函数
HI_U32 mrsMapTimeout(MRS_MAP_MODULE *pstModule);

//获取映射表维护使能开关
HI_VOID mrsGetNvMacMapInfo(MRS_MAP_MODULE *pstModule);

//CCO状态从建立映射表状态到正常抄表状态过程中的公共处理
HI_VOID mrsCcoMapToNormalPro(HI_VOID);

//映射表检查TTL超时处理函数
HI_VOID mrsMapTtlTimeoutFunc(MRS_MAP_MODULE *pstModule);

// 映射表生命值清0
HI_VOID mrsMapTtlZero(MRS_MAP_MODULE *pstModule);

// 映射表老化暂停
HI_VOID mrsMapAgingPause(HI_VOID);

// 映射表老化恢复
HI_VOID mrsMapAgingResume(HI_VOID);

HI_U32 mrsGetDeviceAddr(HI_U8 pucMacAddr[], HI_U8 ucType, HI_U8 pucDeviceAddr[]);

HI_VOID mrsMeterSetDevInf(MRS_DEVICE_INF_STRU *pstDevInf, MRS_METER_INF_STRU *pstMeterInf);

HI_BOOL mrsMapMacIsCollector(HI_U8 ucType);

HI_U32 mrsRmMapProc(MRS_645_FRAME_STRU *pstFrame, MRS_MAP_MODULE *pstModule, HI_U8 *pucMac);

HI_U32 mrsGetMacFromMap(MRS_MAC_DEVICE_MAP *pstMacMap, HI_U8 *pucDeviceAddr, HI_U8 *pucMacAddr);

/**
 @brief  Decrease meter-reading TTL when meter-reading is faield. TTL=0, delete the meter-mac pair.
 @brief  CNcomment: 抄表失败时TTL递减，TTL=0时，从映射表中给删除对应的映射关系。CNend
 @param[in] pstModule      Meter-Mac pair map header pointer.  CNcomment: 映射关系头指针。CNend
 @param[in] pucDeviceAddr  Device address pointer.             CNcomment: 设备地址指针。  CNend
 */
HI_VOID mrsDecreaseMrTTL(MRS_MAP_MODULE *pstModule, HI_U8 *pucDeviceAddr);


HI_END_HEADER

#endif //__MRS_SRV_CCO_MACMAP_H__
