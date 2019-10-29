#ifndef _HI_EQT_PC_IIC_H_
#define _HI_EQT_PC_IIC_H_

#include "hi_types.h"

// 过零及硬件版本是否测试
#define EQUIP_PC_STATUS_CAPABILITY_DEFAULT  (0x00)
#define EQUIP_PC_STATUS_CAPABILITY_ZERO_CROSS (0x01)       
#define EQUIP_PC_STATUS_CAPABILITY_HW_VER_ID     (0x02)

#define IIC_ASSET_NUM_COMPST_LENGTH_MAX     (12)

// 探测报文下行

typedef struct _EQUIP_II_STATUS_STRU_
{ 
    HI_U8 Id;                  // 工位ID
    HI_U8 ProductKind;         // 产品形态
    HI_U8 EquipMode;           // 工装类型
    HI_U8 ucRsv;
    HI_CHAR Version[48]; // "V100R001C00B020SPC001 2011-08-01 14:30:26"
    HI_U8 ucRsv1[12];
} EQUIP_II_STATUS_STRU;

// 出厂检验最终结果上行
typedef struct _EQUIP_II_PRODCHECK_TEST_REPORT_STRU_
{ 
    HI_U8 assetNumWrt;        // 资产编号写入
    HI_U8 IrTest;             // 红外
    volatile HI_U8 LedTest;   // LED
    HI_U8 PlcTestRet;         // 载波测试
    HI_U8 modePowerModify;    // NV修改，即模式、发射功率和搜表开关NV修改是否成功
    HI_U8 usVersionResult;    // 0，待测试；1，成功；2，失败
    HI_U8 usIIMACResult;      // 0，待测试；1，成功；2，失败
    HI_U8 ucHwVerRet;
    HI_U32 powerUA;
    HI_U8 cZeroCrossRet;
    HI_S8 cZeroCrossValue;
    HI_U8 ucRsv[18];
}EQUIP_II_PRODCHECK_TEST_REPORT_STRU;


#define EQUIP_IR_VERSION_LEN_MAX (32)

// 产线工装最终结果上行
typedef struct _EQUIP_II_TEST_REPORT_STRU_
{ 
    HI_U8 IrTest;            // 红外
    HI_U8 LedTest;          // LED
    HI_U8 PlcTestRet;         // 载波测试
    HI_U8 modePowerModify;    // NV修改，即模式、发射功率和搜表开关NV修改是否成功
    HI_U8 ucTestNum;            // 用例执行个数
    HI_U8 ucRsv[3];
    HI_U8 irVersion[EQUIP_IR_VERSION_LEN_MAX];       //红外上报版本信息    
    HI_U8 ucRsv1[8];
}EQUIP_II_TEST_REPORT_STRU; 

// 扫码规则下行

typedef struct _EQUIP_II_STATUS_ACK_STRU_
{ 
    HI_U8 CompensateHead[IIC_ASSET_NUM_COMPST_LENGTH_MAX];           //前缀
    HI_U8 CompensateTail[IIC_ASSET_NUM_COMPST_LENGTH_MAX];           //后缀
    HI_U8 UsedInHead;                                 //前缀多少位有效
    HI_U8 UsedInTail;                                 //后缀多少位有效
    HI_U8 ScanOrNot;                                  //是否扫描条码 1 扫 0 不扫
    HI_U8 StartPosition;
    HI_U8 EndPosition;
    HI_U8 totalLen;
    HI_U8 ret;
    HI_U8 ucCapability;
    HI_U8 ucHwVerId;
    HI_U8 ucRsv1;
    HI_U16 usMinThreshold240K;  
	HI_U16 usMaxThreshold240K;
	HI_U16 usMinThreshold1M; 
	HI_U16 usMaxThreshold1M; 
	HI_U16 usLevelThreshold;
    HI_U8  ucRsv2[20]; 

} EQUIP_II_STATUS_ACK_STRU;

#endif