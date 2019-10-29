//*****************************************************************************
//
//                  版权所有 (C), 1998-2012, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : equip_dut_proc.h
//  版 本 号   : V2.0 
//  作    者   : fengxiaomin/00209182
//  生成日期   : 2012-07-20
//  功能描述   : TODO: ...
//               
//  函数列表   : TODO: ...
//  修改历史   : 
//  1.日    期 : 
//    作    者 : 
//    修改内容 : 创建文件 
//
//*****************************************************************************

//*****************************************************************************
// PROJECT   : 
// SUBSYSTEM :
// MODULE    :  
// OWNER     :  
//*****************************************************************************
#include <hi_equip.h>

#ifndef __EQUIP_DUT_PROC_H__
#define __EQUIP_DUT_PROC_H__

HI_START_HEADER
/* BEGIN: Modified by fengxiaomin/00209182, 2014/6/19   问题单DTS2014071004301 */
#define MRS_DUT_645_WORK_MODE_FRAME     {0x68,0xDC,0xEE,0xEB,0xDC,0xBB,0xCE,0x68,0x9E,0x08,0x35,0x35,0x32,0x91,0x35,0x33,0x33,0x33,0x8B,0x16}
#define MRS_DUT_SDRAM_POSITION          (15)
#define MRS_DUT_SDRAM_FAIL_VALUE        (0x32)
#define MRS_DUT_645_TIMEOUT             (20)
#define MRS_DUT_645_JUDGE_MODE_DI       {0x0D,0x12,0xFF,0xFE}

#define MRS_DUT_PRODUCT_CHECK_RESULT_FAIL    (0x08)
#define MRS_DUT_PRODUCT_LINE_RESULT_FAIL     (0x02)
#define MRS_DUT_TEST_RESULT_SUCCESS     (1)

#define MRS_DUT_ASSERT_ADDRSS_DEFAULT   {0xBB,0xBB,0xBB,0xBB,0xBB,0xBB}
#define MRS_DUT_ASSERT_ADDRSS_LEN       (6)
#define MRS_DUT_NV_POWER_DATA_VALUE     (7)  //正常模式下0x5中的PowerStep的取值
#define MRS_DUT_NV_AEFPGA_GAIN          (10) //正常模式下0x18中的TxAfePgaGain的取值
#define MRS_DUT_IDENTIFY_RET_INIT       (0xFFFFFFFF)
#define MRS_DUT_PRODUCT_LINE_MODE       (1)
#define MRS_DUT_PRODUCT_CHECK_MODE      (2)

#define MRS_DUT_645_TEST_SDRAM_ERROR    (0x00000010)
#define MRS_DUT_645_POWERE_DOWN_ERROR   (0x00000001)
#define MRS_DUT_645_MAC_WRITE_NV_ERROR  (0x00000002)
#define MRS_DUT_645_SET_MAC_ERROR       (0x00000004)
#define MRS_DUT_645_ASSERT_WRITE_NV_ERROR   (0x00000008)

#define MRS_PROTO645_DATA_CS_END_LEN_MAX    (202)

#define MRS_DUT_STA_645_FRAME_ADDR          {0xCC,0xCC,0xCC,0xCC,0xCC,0xCC}
/* END:   Modified by fengxiaomin/00209182, 2014/6/19 */
#define MRS_DUT_645_FRAME_CTRL          (0x1E)
#define MRS_DUT_645_FRAME_ERROR_DATA    (0x01)

#define MRS_DUT_645_FRAME_FRM_FLG       (0x00)

#define MRS_DUT_645_FRAME_DIR_DOWN      (0x00)
#define MRS_DUT_645_FRAME_DIR_UP        (0x80)

#define MRS_DUT_645_FRAME_DENY_FLG      (0x40)
#define MRS_DUT_645_FRAME_ACK_FLG       (0x00)

#define MRS_DUT_645_FRAME_CTRL_UP       (MRS_DUT_645_FRAME_DIR_UP | MRS_DUT_645_FRAME_ACK_FLG | MRS_DUT_645_FRAME_FRM_FLG | MRS_DUT_645_FRAME_CTRL)
#define MRS_DUT_645_FRAME_CTRL_UP_DENY  (MRS_DUT_645_FRAME_DIR_UP | MRS_DUT_645_FRAME_DENY_FLG | MRS_DUT_645_FRAME_FRM_FLG | MRS_DUT_645_FRAME_CTRL)

#define MRS_DUT_645_DATA_HEX33          (0x33)


#define EQUIP_DUT_645_EQUIPMODE         (0x01)   // 装备模式
#define EQUIP_DUT_645_FRAME_START_CODE  (0x68)   // 645规约帧起始符
#define EQUIP_DUT_645_FRAME_END_CODE    (0x16)   // 645规约帧结束符
#define EQUIP_DUT_645_MIN_LEN           (12)     // 645规约帧最小长度
#define EQUIP_DUT_LED_ON_OFF_TIME       (150)    // 毫秒
#define EQUIP_WRT_NV_MAX_TIMES          (3) 


#define MRS_DUT_ERR_SUCCESS             (0x00000000)
#define MRS_DUT_ERR_LOWER_POWER         (0x00000001)
#define MRS_DUT_ERR_MAC2NV              (0x00000002)
#define MRS_DUT_ERR_ID2MAC              (0x00000004)
#define MRS_DUT_ERR_ASSET2NV            (0x00000008)

#define EQUIP_READ_CHIP_ID_FROM_NV           ((HI_U16)(0))
#define EQUIP_READ_CHIP_ID_FROM_EFUSE        ((HI_U16)(1))

typedef struct _EQUIP_TEST_DUT_INFO_STRU_
{ 
    HI_U8 RltCode;                      // 错误码
    HI_U8 DutMac[HI_PLC_MAC_ADDR_LEN];  // 被测试DUT实际的的MAC地址
    HI_U8 usVender[2];                  // 厂商代码，ASCII
    HI_U8 usChip[2];                    // 芯片代码，ASCII
    HI_U8  ucVerDate;                   // 版本日期-日，BCD码
    HI_U8  ucVerMonth;                  // 版本日期-月，BCD码
    HI_U8  ucVerYear;                   // 版本日期-年，BCD码
    HI_U8 ucVersion[2];                 // 版本号，BCD码
} EQUIP_TEST_MAC_CHK_RLT_STRU;


/* BEGIN: Modified by fengxiaomin/00209182, 2014/6/19   问题单DTS2014071004301 */
typedef struct
{
    HI_U32 ulIsEquipMode;
    HI_U32 ulInitDutMode;
    HI_U32 ulRxDutMode;
    HI_U8 ucGetModeFlag;
    HI_U8 zero_cross_phase : 2;
    HI_U8 zero_cross_edge  : 1;
    HI_U8 rsv1 : 5;
    HI_U8 reserved[2];
} EQUIP_DUT_MAIN_INFO_STRU;


typedef struct 
{
    HI_U8 ucFrameStart; // 
    HI_U8 ucAddr[HI_METER_ADDR_LEN];           // 表地址，固定6字节 
    HI_U8 ucFrameEnd; // 
    HI_U8 stCtrl; // 控制码
    HI_U8 ucDataRealmLen; // 数据域大小必须小于等于200字节, 0表示无数据域
    HI_U8 ucData[MRS_PROTO645_DATA_CS_END_LEN_MAX]; // 数据域、CS、结束符0x16
} MRS_DUT_645_FRAME_STRU;
/* END:   Modified by fengxiaomin/00209182, 2014/6/19 */

//客户版本信息
typedef struct
{
    HI_CHAR szUserVer[32];	/* 用户版本号，ASCII格式 */
    HI_CHAR szChipCode[3];	/* 芯片代码，ASCII格式 */
    HI_U8 aucDate[3];		/* 版本日期，BIN格式，YYMMDD */
    HI_U16 usSwVer;			/* 软件版本号，BCD格式 */
    HI_CHAR aucManuCode[3]; /* 厂商代码，ASCII格式 */
    HI_U8 reserved[17];
} EQUIP_SET_USER_INF_REQ_STRU;

typedef struct
{
    HI_U32 ulRet;		/* 设置结果：HI_ERR_SUCCESS:成功; 其他:失败 */
} EQUIP_SET_USER_INF_IND_STRU;



typedef enum
{
    DUT_FILE_LOG_FLAG_001 = 1,
    DUT_FILE_LOG_FLAG_002,
    DUT_FILE_LOG_FLAG_003,
    DUT_FILE_LOG_FLAG_004,
    DUT_FILE_LOG_FLAG_005,
    DUT_FILE_LOG_FLAG_006,
    DUT_FILE_LOG_FLAG_007,
    DUT_FILE_LOG_FLAG_008,
    DUT_FILE_LOG_FLAG_009,
    DUT_FILE_LOG_FLAG_010,
    DUT_FILE_LOG_FLAG_011,
    DUT_FILE_LOG_FLAG_012,
    DUT_FILE_LOG_FLAG_013,
    DUT_FILE_LOG_FLAG_014,
    DUT_FILE_LOG_FLAG_015,
    DUT_FILE_LOG_FLAG_016,
    DUT_FILE_LOG_FLAG_017,
    DUT_FILE_LOG_FLAG_018,
    DUT_FILE_LOG_FLAG_019,
    DUT_FILE_LOG_FLAG_020,
    DUT_FILE_LOG_FLAG_021,
    DUT_FILE_LOG_FLAG_022,
    DUT_FILE_LOG_FLAG_023,
    DUT_FILE_LOG_FLAG_024,
    DUT_FILE_LOG_FLAG_025,
    DUT_FILE_LOG_FLAG_026,
    DUT_FILE_LOG_FLAG_027,
    DUT_FILE_LOG_FLAG_028,
    DUT_FILE_LOG_FLAG_029,
    DUT_FILE_LOG_FLAG_030,
    DUT_FILE_LOG_FLAG_031,
    DUT_FILE_LOG_FLAG_032,
    DUT_FILE_LOG_FLAG_033,
    DUT_FILE_LOG_FLAG_034,
    DUT_FILE_LOG_FLAG_035,
    DUT_FILE_LOG_FLAG_036,
    DUT_FILE_LOG_FLAG_037,
    DUT_FILE_LOG_FLAG_038,
    DUT_FILE_LOG_FLAG_039,
    DUT_FILE_LOG_FLAG_040,
    DUT_FILE_LOG_FLAG_041,
    DUT_FILE_LOG_FLAG_042,
    DUT_FILE_LOG_FLAG_043,
    DUT_FILE_LOG_FLAG_044,
    DUT_FILE_LOG_FLAG_045,
    DUT_FILE_LOG_FLAG_046,
    DUT_FILE_LOG_FLAG_047,
    DUT_FILE_LOG_FLAG_048,
    DUT_FILE_LOG_FLAG_049,
    DUT_FILE_LOG_FLAG_050,
    DUT_FILE_LOG_FLAG_051,
    DUT_FILE_LOG_FLAG_052,
    DUT_FILE_LOG_FLAG_053,
    DUT_FILE_LOG_FLAG_054,
    DUT_FILE_LOG_FLAG_055,
    DUT_FILE_LOG_FLAG_056,
    DUT_FILE_LOG_FLAG_057,
    DUT_FILE_LOG_FLAG_058,
    DUT_FILE_LOG_FLAG_059,
    DUT_FILE_LOG_FLAG_060,
    DUT_FILE_LOG_FLAG_061,
    DUT_FILE_LOG_FLAG_062,
    DUT_FILE_LOG_FLAG_063,
    DUT_FILE_LOG_FLAG_064,
    DUT_FILE_LOG_FLAG_065,
    DUT_FILE_LOG_FLAG_066,
    DUT_FILE_LOG_FLAG_067,
    DUT_FILE_LOG_FLAG_068,
    DUT_FILE_LOG_FLAG_069,
    DUT_FILE_LOG_FLAG_070,
    DUT_FILE_LOG_FLAG_071,
    DUT_FILE_LOG_FLAG_072,
    DUT_FILE_LOG_FLAG_073,
    DUT_FILE_LOG_FLAG_074,
    DUT_FILE_LOG_FLAG_075,
    DUT_FILE_LOG_FLAG_076,
    DUT_FILE_LOG_FLAG_077,
    DUT_FILE_LOG_FLAG_078,
    DUT_FILE_LOG_FLAG_079,
    DUT_FILE_LOG_FLAG_080,
    DUT_FILE_LOG_FLAG_081,
    DUT_FILE_LOG_FLAG_082,
    DUT_FILE_LOG_FLAG_083,
    DUT_FILE_LOG_FLAG_084,
    DUT_FILE_LOG_FLAG_085,
    DUT_FILE_LOG_FLAG_086,
    DUT_FILE_LOG_FLAG_087,
    DUT_FILE_LOG_FLAG_088,
    DUT_FILE_LOG_FLAG_089,
    DUT_FILE_LOG_FLAG_090,
    DUT_FILE_LOG_FLAG_091,
    DUT_FILE_LOG_FLAG_092,
    DUT_FILE_LOG_FLAG_093,
    DUT_FILE_LOG_FLAG_094,
    DUT_FILE_LOG_FLAG_095,
    DUT_FILE_LOG_FLAG_096,
}DUT_FILE_LOG_FLAG_ENUM;



//数据标识码处理函数
typedef HI_U32 (*MRS_DUT_645_DI_FUN)(HI_VOID * param);

HI_EXTERN EQUIP_DUT_MAIN_INFO_STRU * do_GetDutInfo(HI_VOID);

#define EQUIP_DUT_IS_EQUIP_MODE() (do_GetDutInfo()->ulIsEquipMode == HI_ND_SYS_STATUS_TEST)

#define EQUIP_DUT_METER_MODULE() (do_GetDutInfo()->stMetermodule)

// 测试模式时，调用工装初始化函数
HI_VOID mrsDutCtxInit(HI_VOID);

// DUT接收帧处理
HI_U32 mrsDutRxProcess(MRS_ONE_RAW_FRAME_STRU *pstFrame);

// 对每一个接收的完整帧进行相应的处理
MRS_DUT_645_DI_FUN mrsDutFramePro(MRS_DUT_645_FRAME_STRU *pst645Frame);
/* BEGIN: Modified by fengxiaomin/00209182, 2014/6/19   问题单DTS2014071004301 */
HI_U32 mrsJudgeEqiupMode(HI_U8 *pucData, HI_U16 usDataLen);
HI_U32 mrsSetDutTestMode(HI_U32 ulCurrWorkMode);
//产线工装收到写mac处理
HI_U32 mrsDutProdLinePro(HI_U8 *pucData);
HI_U32 mrsDutProdCheckPro(HI_U8 *pucData);
HI_U32 mrsWriteCltAddress(HI_U8* CltAddress);
HI_U32 mrsDutResultReset(HI_U32 ulEquipMode);
HI_VOID mrsDutCltIISendStatus(HI_VOID);
HI_VOID mrsDutFrmDeQueue(HI_VOID);
HI_VOID mrsDutRunLedOn(HI_VOID);
HI_U32 mrsDutSetResultOk(HI_U32 ulEquipMode);
HI_BOOL mrsDutIsInitResult(HI_U8 ucResult, HI_U32 ulEquipMode);
HI_VOID mrsDutSetRxMode(HI_U32 ulMode);
/* END:   Modified by fengxiaomin/00209182, 2014/6/19 */

HI_VOID mrsZeroCrossCheckProc(HI_U32 result, EQUIP_ZERO_CROSS_REQ_ACK_STRU *pstResult);
HI_VOID mrsDut3762Reply_AFNF002(HI_U32 result, EQUIP_ZERO_CROSS_REQ_ACK_STRU *pstResult);
HI_VOID mrsZeroCrossCheckNotify(HI_U32 ulRet, HI_U8 ucLevel, HI_U32* ntb);
HI_U32 mrsEfuseReadID(HI_U8 efuse_data[EFUSE_AREA_SIZE]);

hi_void mrs_zero_cross_set_info(hi_u8 phase);
hi_u8 mrs_zero_cross_get_phase(hi_void);
hi_u8 mrs_zero_cross_get_edge(hi_void);
HI_U32 mrsWriteModuleId(HI_U8* moduleId);
HI_U32 mrsGetModule(HI_U8* moduleId);

HI_END_HEADER

#endif 


