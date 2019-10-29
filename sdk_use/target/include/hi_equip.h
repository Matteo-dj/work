/*
 * for HI3911V100 equip .
 *
 * History: 
 *      2012-08-15 create by f00209182
 */ 

#ifndef __HI_EQUIP_H__
#define __HI_EQUIP_H__
#include <hi_types.h>
#include <hi_errno.h>
#include <dfx_sys.h>//to check 循环嵌套头文件?


typedef struct HI_EQUIP_NV_DATA_S
{
    HI_U16 usId;
    HI_U8 ulDataBuffer[2];
}HI_EQUIP_NV_DATA;

//查询DUT工作模式
#define EQUIP_UE_NORMAL_MODE      (0)
#define EQUIP_UE_TEST_MODE        (2)


#define EQUIP_MODE_READ_CAPABILITY_DEFAULT  (0x00)
#define EQUIP_MODE_READ_CAPABILITY_ZERO_CROSS (0x01)
#define EQUIP_MODE_READ_CAPABILITY_HW_VER     (0x02)

typedef struct EQUIP_UE_MODE_READ_ACK_STRU_S
{
    HI_U8 ucMode;
    HI_U8 ucSdramRet;
	HI_U8 ucCapability;
	HI_U8 ucRsv;
	HI_U32 ulModeRet;
}EQUIP_UE_MODE_READ_ACK_STRU;

//修改DUT工作模式
typedef struct EQUIP_UE_MODE_REQ_STRU_S
{
    HI_U32 ulUeMode;
}EQUIP_UE_MODE_REQ_STRU;

typedef struct EQUIP_UE_MODE_ACK_STRU_S
{
    HI_U32 ret;
}EQUIP_UE_MODE_ACK_STRU;

#define EQUIP_NTB_NUM                    (10)
#define EQUIP_NTB_NUM_DUT                (EQUIP_NTB_NUM*2)
#define EQUIP_NTB_NUM_CCO                (20)

typedef struct _EQUIP_ZERO_CROSS_REQ_STRU_
{
    HI_U8 level;
    HI_U8 phase : 2; /* 相线：0-A相 1-B相 2-C相 3-保留 */
    HI_U8 edge  : 1; /* 沿信息：0-上升沿 1-下降沿 */
    HI_U8 rsv1  : 5;
    HI_U8 rsv2[2];
}EQUIP_ZERO_CROSS_REQ_STRU;

// 目前CCO和被检板的数据结构一样，CCO的与被检板复用一个
typedef struct _EQUIP_ZERO_CROSS_REQ_ACK_STRU_
{
    HI_U32 ret;
    HI_U8  level;
	HI_BOOL preciseFlag;//精准过零标志位，0:非精准过零；1:精准过零
    HI_U8  phase : 2; /* 相线：0-A相 1-B相 2-C相 3-保留 */
    HI_U8  edge  : 1; /* 沿信息：0-上升沿 1-下降沿 */
    HI_U8  rsv1  : 5;
    HI_U8  rsv2;
    HI_U32 ntb[EQUIP_NTB_NUM_DUT];
} EQUIP_ZERO_CROSS_REQ_ACK_STRU;

typedef struct _EQUIP_HW_VER_ID_REQ_ACK_STRU_
{
    HI_U8 ucHwVerId;
	HI_U8 ucRsv[3];
	HI_U32 ulRet;
    HI_U32 ulRsv1[2];
}EQUIP_HW_VER_ID_REQ_ACK_STRU;

#if defined(MRS_SGD_INNER_VER)
typedef struct _EQUIP_SW_VER_ID_REQ_ACK_STRU_
{
    HI_U8 aucSwInnerVer[8];
	HI_U16 aucSwExtVer;
	HI_U8 rltCode;
   	HI_U8 rsv;
}EQUIP_SW_VER_ID_REQ_ACK_STRU;
#endif
//上报PC是否开始测试
#define EQUIP_UE_TEST_START       (1)//开始测试
#define EQUIP_UE_TEST_STOP        (2)//停止测试
#define EQUIP_UE_TEST_REPEAT      (3)//重新测试
#define EQUIP_UE_TEST_NOTALLOW    (4)//非测试模式（目前STA工装使用）
#define EQUIP_ERR_UE_USER_MODE    (5)//用户模式
#define EQUIP_ERR_UE_TEST_MODE    (6)//测试模式
#define EQUIP_ERR_UART_ERR        (8)//串口异常
#define EQUIP_ERR_II_PRODCHECK_MODE  (7)//II采出厂检验模式
// 测试阶段标志
#define EQUIP_UE_BEFORE_AGING   (1)
#define EQUIP_UE_AFTER_AGING    (2)

typedef struct EQUIP_UE_START_REQ_STRU_S
{
    HI_U32 ulUeReq;       // 测试请求标志
    HI_U32 ucTestFhase;   // 测试阶段标志

}EQUIP_UE_START_REQ_STRU;


#define EQUIP_PRODLINE_TEST_645_DATA   0x1
#define EQUIP_PRODCHECK_TEST_645_DATA  0x2
#define EQUIP_TEST_645_DATA_SIZE       14
#define EQUIP_TEST_645_CS_SIZE         22
#define EQUIP_TEST_645_CS              0x94
#define EQUIP_TEST_645_DATA_ADD        0x33


typedef struct _EQUIP_II_START_REQ_STRU_
{
    HI_U32 ulUeReq; // 测试请求标志
}EQUIP_II_START_REQ_STRU;

typedef struct _EQUIP_II_START_REQ_ACK_STRU_
{
    HI_U32 ulRet;
}EQUIP_II_START_REQ_ACK_STRU;

//下发DUT写MAC命令

typedef struct EQUIP_MAC_STRU_S
{
    HI_U8 ucPlcMac[HI_PLC_MAC_ADDR_LEN]; 
    HI_U8 ucRsv[2];
}EQUIP_MAC_STRU;

//上传PC DUT写入MAC结果
typedef struct EQUIP_MAC_WRT_ACK_STRU_S
{
    HI_U32 ulRet;
}EQUIP_MAC_WRT_ACK_STRU;

//上报PC测试结果
#define EQUIP_NOT_TEST               (0)
#define EQUIP_TEST_OK                (1)
#define EQUIP_TEST_FAIL              (2)
#define EQUIP_NOT_SUPPORT            (6)

//新增
#define EQUIP_PMU_UNNORMAL           (1)
#define EQUIP_PMU_OK                 (0)
#if 1
typedef struct
{
    HI_U16 usVender;      //厂商代码
    HI_U16 usChip;        //芯片代码
    HI_U8  ucVerDate;     //版本日期-日
    HI_U8  ucVerMonth;    //版本日期-月
    HI_U8  ucVerYear;     //版本日期-年
    HI_U16 usVersion;     //版本
    HI_U8 ucRsv[3];
}NDM_PROTO_3762_AFN03_F1_UL_STRU;
#endif
//频偏下行
typedef struct EQUIP_FREQ_WRT_REQ_STRU_S
{
    HI_S16 sFreqValue; // 频偏值
    HI_S16 usRsv;
}EQUIP_FREQ_WRT_REQ_STRU;

typedef struct EQUIP_FREQ_WRT_ACK_STRU_S
{
    HI_U32 ret;
}EQUIP_FREQ_WRT_ACK_STRU;

// DUT主动上报的就需状态
// 该数据结构只在测试模式下上报，正常模式不上报
typedef struct _EQUIP_DUT_RUNSTA_IND_STRU_
{
    // 携带工作模式，按照目前机制该结构上报必然是测试模式
    HI_U8 ucStatus;
    HI_U8 ucSdramRet;
    HI_U8 ucRsv[2];  
}EQUIP_DUT_RUNSTA_IND_STRU;

#define EQUIP_OK 0x10000001

//读取三相表标识上行数据结构
typedef struct _Equip_ReadPhaseFlag_Up_S_
{
	hi_u8 type;
	hi_u8 ret;
}Equip_ReadPhaseFlag_Up_S;

#endif 

