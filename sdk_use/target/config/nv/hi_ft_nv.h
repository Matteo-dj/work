//*****************************************************************************
//
//                  版权所有 (C), 1998-2011, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : hi_ft_nv.h
//  版 本 号   : V1.0
//  作    者   : shenhankun/00130424
//  生成日期   : 2011-10-26
//  功能描述   : 工厂模式下的NV项设置
//
//  函数列表   : TODO: ...
//  修改历史   :
//  1.日    期 : 2011-10-26
//    作    者 : shenhankun/00130424
//    修改内容 : 创建文件
//
//*****************************************************************************

//*****************************************************************************
// PROJECT   :
// SUBSYSTEM :
// MODULE    :
// OWNER     :
//*****************************************************************************

#ifndef __FT_NV_H__
#define __FT_NV_H__
#include <hi_types.h>
//*****************************************************************************


//*****************************************************************************
// NV ID 定义 [0xF00, 0xFFF]
//*****************************************************************************
#define  HI_NV_FTM_ID_START            0xF00
#define  HI_NV_FTM_ID_END              0xFFF

#define  HI_NV_FTM_MANUFACOTR_ID       0xF01 // HI_NV_FTM_MANUFACOTR_ID_S
#define  HI_NV_FTM_RESERVED1_ID        0xF02 // HI_NV_FTM_RESERVED1_ID_S
#define  HI_NV_FTM_PRODUCT_ID          0xF03 // HI_NV_FTM_PRODUCT_ID_S
#define  HI_NV_FTM_RESERVED2_ID        0xF04 // HI_NV_FTM_RESERVED2_ID_S
#define  HI_NV_FTM_STARTUP_CFG_ID      0xF05 // HI_NV_FTM_STARTUP_CFG_S
#define  HI_NV_FTM_UPG_START_CFG_ID    0xF06 // HI_NV_FTM_UPG_START_CFG_S
#define  HI_NV_FTM_UPG_CFG_ID          0xF07 // HI_NV_FTM_UPG_CFG_S

#define  ID_DIAG_CMD_CONFIG_NM_SERIAL_NUMBER        (0xF08)

#define HI_NV_FTM_RESERVED3_ID         0xF09 // HI_NV_FTM_RESERVED3_ID_S


///////////////////////////////////////////////////////////////////////////////
// Reserved for used.
#define HI_NV_FTM_RESERVED4_ID         0xFA0 // HI_NV_FTM_RESERVED4_ID_S
#define  HI_NV_FTM_NM_INIT_PARA         0xFA1 // HI_NV_FTM_RESERVED1_S

#define  HI_NV_FTM_UPG_SCHEDL_CFG_ID            0xFA2 // 升级全网策略的配置
#define  HI_NV_FTM_UPG_SCHEDL_ADVANCE_CFG_ID    0xFA3 // 升级全网策略的配置

#define  HI_NV_FTM_COLLECT_PARAM_FILE   0xFA4 // 采集器参数存储
/* BEGIN: Modified by b00208046, 2014/1/10   问题单号:DTS2014010400315*/
#define  HI_NV_FTM_BQ_UPG_ID            0xFA5 // HI_NV_FTM_CMN_CFG_ID_S
/* END:   Modified by b00208046, 2014/1/10 */
#define  HI_NV_FTM_CCO_PARAM_ID         0xFA6 // CCO
#define  HI_NV_FTM_PASSWORD_ID          0xFA7 // 存储HSO密码
#define  HI_NV_FTM_NDM_AUTO_CHECK_ID    0xFA7 // HI_NV_FTM_RESERVED7_S
#define  HI_NV_FTM_MANAGEMENT_ID        0xFA8 // 存储芯片管理ID 和设备ID
#define  HI_NV_FTM_RESERVED9_ID         0xFA9 // HI_NV_FTM_RESERVED9_S
//*****************************************************************************


//*****************************************************************************
#define HI_IS_NV_FTM(x)               ((x)>=HI_NV_FTM_ID_START) && ((x)<HI_NV_FTM_ID_END)
//*****************************************************************************


//*****************************************************************************
typedef struct
{
    HI_CHAR  szId[3];     // 厂商代码，默认为"HS"
    HI_CHAR  szChipId[5]; // 芯片代码，默认为"3911"
}HI_NV_FTM_MANUFACOTR_ID_S;

typedef enum
{
    HI_FTM_HW_PLATFORM_TYPE_UNKOWN = 0, // 默认为 ASIC
    HI_FTM_HW_PLATFORM_TYPE_FPGA, // FPGA
    HI_FTM_HW_PLATFORM_TYPE_ASIC  // ASIC
} HI_FTM_HW_PLATFORM_TYPE_E;


/**
 * @ingroup hct_mdm
 * 芯片类型
 */
typedef enum
{
    HI_FTM_CHIP_TYPE_UNKOWN = 0,/**< 未知芯片类型*/
    HI_FTM_CHIP_TYPE_3911C,     /**< C芯片类型*/
    HI_FTM_CHIP_TYPE_3911T,     /**< T芯片类型*/
    HI_FTM_CHIP_TYPE_MAX
} HI_FTM_CHIP_TYPE_E;

/* BEGIN: Added by b00208046, 2014/1/10   问题单号:DTS2014010400315*/
typedef struct
{
    HI_U32 ulBatChipUpgVer;                   //BQ芯片版本号
    HI_U32 ulBatChipUpgCheckSum;              //之前升级文件CRC
    HI_U16 usBatChipUpgStatus;                //之前的升级状态
    HI_U8 ucReserved1[6];                     //BQ升级预留
    HI_U8 ucDefaultGateway[HI_IPV4_ADDR_LEN]; //以太网默认网关
    HI_U8 ucReserved2[44];                    //保留字节
}HI_NV_FTM_CMN_CFG_ID_S;


//用于存储本地挑战值计算参数
typedef struct
{
    HI_U32 verMagic:24;//0X50 0X57 0X44  'PWD'
    HI_U32 verNum:8;//默认写0x1
    HI_U8  aucSTK[32];//用来存储PWD的STK
    HI_U32 aulSTKCrc;//STK密码校验值
    HI_U8  aucSalt[16];		// 计算口令存储信息的盐值，旧版本此位置为0
    HI_U16 usCnt;		// 计算口令存储信息的PBKDF2迭代次数，旧版本此位置为0
    HI_U16 reserved[3];
} HI_NV_FTM_PASS_WORD_S;

typedef HSO_ENUM
{
    HI_FTM_STA_TYPE_UNKOWN = 0, // 默认为 ASIC
    HI_FTM_STA_TYPE_USTA, // USTA
    HI_FTM_STA_TYPE_MAX
} HI_FTM_STA_TYPE_E;

/* END:   Added by b00208046, 2014/1/10 */
#define HI_FTM_CHIP_SDRAM_2M   2
#define HI_FTM_CHIP_SDRAM_8M   8
#define HI_FTM_CHIP_SDRAM_16M  16

// STA产品类型中电表/I采的产品子类型定义
#define  HI_FTM_PRODUCT_TYPE_STATYPE_M 0 // 电表
#define  HI_FTM_PRODUCT_TYPE_STATYPE_I 1 // I采

#define HI_FTM_PRODUCT_APPID_MRS    (0)//集抄系统
#define HI_FTM_PRODUCT_APPID_RMT    (1)//宽带远程载波终端
#define HI_FTM_PRODUCT_APPID_RSV    (4)//注意:4096的ID区间ID保留给boot，非产品使用
#define HI_FTM_PRODUCT_ID_RANGE_MRS  (500)
#define HI_FTM_PRODUCT_ID_RANGE_RMT  (200)
#ifdef PRODUCT_CFG_FEATURE_REMOTE_COMMUNICATION_TERMINAL
#define HI_FTM_PRODUCT_TYPE_BASE  (HI_FTM_PRODUCT_ID_RANGE_MRS) //宽带载波汇聚终端系统
#define HI_FTM_PRODUCT_TYPE_RANGE (HI_FTM_PRODUCT_ID_RANGE_RMT)
#else
#define HI_FTM_PRODUCT_TYPE_BASE 0    //集抄系统
#define HI_FTM_PRODUCT_TYPE_RANGE (HI_FTM_PRODUCT_ID_RANGE_MRS)
#endif
// 最大值为16bit
#ifndef MAC_V200_LLT
typedef HSO_ENUM
{
    HI_FTM_PRODUCT_TYPE_UNKOWN = 0,

    HI_FTM_PRODUCT_TYPE_C01 = (HI_FTM_PRODUCT_TYPE_BASE+1),// 集中器载波模块；版本1；单相耦合
    HI_FTM_PRODUCT_TYPE_C02,
    HI_FTM_PRODUCT_TYPE_C03,
    HI_FTM_PRODUCT_TYPE_C04,
    HI_FTM_PRODUCT_TYPE_C05,
    HI_FTM_PRODUCT_TYPE_C06,
    HI_FTM_PRODUCT_TYPE_C07,
    HI_FTM_PRODUCT_TYPE_C08,

    HI_FTM_PRODUCT_TYPE_C11 = (HI_FTM_PRODUCT_TYPE_BASE+11),// 集中器载波模块；版本1；三相耦合
    HI_FTM_PRODUCT_TYPE_C12,
    HI_FTM_PRODUCT_TYPE_C13,
    HI_FTM_PRODUCT_TYPE_C14,
    HI_FTM_PRODUCT_TYPE_C15,
    HI_FTM_PRODUCT_TYPE_C16,
    HI_FTM_PRODUCT_TYPE_C17,
    HI_FTM_PRODUCT_TYPE_C18,

    HI_FTM_PRODUCT_TYPE_R21 = (HI_FTM_PRODUCT_TYPE_BASE+21),// 中继器；版本1；单相耦合
    HI_FTM_PRODUCT_TYPE_R22,
    HI_FTM_PRODUCT_TYPE_R23,
    HI_FTM_PRODUCT_TYPE_R24,
    HI_FTM_PRODUCT_TYPE_R25,
    HI_FTM_PRODUCT_TYPE_R26,
    HI_FTM_PRODUCT_TYPE_R27,
    HI_FTM_PRODUCT_TYPE_R28,

    HI_FTM_PRODUCT_TYPE_T31 = (HI_FTM_PRODUCT_TYPE_BASE+31),// 三相表；版本1；三相耦合
    HI_FTM_PRODUCT_TYPE_T32,
    HI_FTM_PRODUCT_TYPE_T33,
    HI_FTM_PRODUCT_TYPE_T34,

    HI_FTM_PRODUCT_TYPE_R35,                    // 中继器；版本1；三相耦合
    HI_FTM_PRODUCT_TYPE_R36,
    HI_FTM_PRODUCT_TYPE_R37,
    HI_FTM_PRODUCT_TYPE_R38,

    HI_FTM_PRODUCT_TYPE_D41 = (HI_FTM_PRODUCT_TYPE_BASE+41),// 抄控器；版本1；单相耦合
    HI_FTM_PRODUCT_TYPE_D42,
    HI_FTM_PRODUCT_TYPE_D43,
    HI_FTM_PRODUCT_TYPE_D44,
    HI_FTM_PRODUCT_TYPE_D45,
    HI_FTM_PRODUCT_TYPE_D46,
    HI_FTM_PRODUCT_TYPE_D47,
    HI_FTM_PRODUCT_TYPE_D48,

    HI_FTM_PRODUCT_TYPE_D51 = (HI_FTM_PRODUCT_TYPE_BASE+51),// 抄控器；版本8；三相耦合
    HI_FTM_PRODUCT_TYPE_D52,
    HI_FTM_PRODUCT_TYPE_D53,
    HI_FTM_PRODUCT_TYPE_D54,
    HI_FTM_PRODUCT_TYPE_D55,
    HI_FTM_PRODUCT_TYPE_D56,
    HI_FTM_PRODUCT_TYPE_D57,
    HI_FTM_PRODUCT_TYPE_D58,

    HI_FTM_PRODUCT_TYPE_D61 = (HI_FTM_PRODUCT_TYPE_BASE+61),// 三代抄控器
    HI_FTM_PRODUCT_TYPE_D62,
    HI_FTM_PRODUCT_TYPE_D63,
    HI_FTM_PRODUCT_TYPE_D64,

    HI_FTM_PRODUCT_TYPE_M101 = (HI_FTM_PRODUCT_TYPE_BASE+101),// 电表/I型采集器 载波模块；版本1
    HI_FTM_PRODUCT_TYPE_M102,
    HI_FTM_PRODUCT_TYPE_M103,
    HI_FTM_PRODUCT_TYPE_M104,
    HI_FTM_PRODUCT_TYPE_M105,
    HI_FTM_PRODUCT_TYPE_M106,
    HI_FTM_PRODUCT_TYPE_M107,
    HI_FTM_PRODUCT_TYPE_M108,

    HI_FTM_PRODUCT_TYPE_M111 = (HI_FTM_PRODUCT_TYPE_BASE+111),// II型采集器版本1
    HI_FTM_PRODUCT_TYPE_M112,
    HI_FTM_PRODUCT_TYPE_M113,
    HI_FTM_PRODUCT_TYPE_M114,
    HI_FTM_PRODUCT_TYPE_M115,
    HI_FTM_PRODUCT_TYPE_M116,
    HI_FTM_PRODUCT_TYPE_M117,
    HI_FTM_PRODUCT_TYPE_M118,
    /* start- 以下ID号由于兼容性问题，仅用于硬件ID，不可用于软件ID -start */
    HI_FTM_PRODUCT_TYPE_M119,
    HI_FTM_PRODUCT_TYPE_M120,
    HI_FTM_PRODUCT_TYPE_M121,
    HI_FTM_PRODUCT_TYPE_M122,
    HI_FTM_PRODUCT_TYPE_M123,
    HI_FTM_PRODUCT_TYPE_M124,
    HI_FTM_PRODUCT_TYPE_M125,
    HI_FTM_PRODUCT_TYPE_M126,

    HI_FTM_PRODUCT_TYPE_M131 = (HI_FTM_PRODUCT_TYPE_BASE+131),// 电表/I型采集器 载波模块；版本9
    HI_FTM_PRODUCT_TYPE_M132,
    HI_FTM_PRODUCT_TYPE_M133,
    HI_FTM_PRODUCT_TYPE_M134,
    HI_FTM_PRODUCT_TYPE_M135,
    HI_FTM_PRODUCT_TYPE_M136,
    HI_FTM_PRODUCT_TYPE_M137,
    HI_FTM_PRODUCT_TYPE_M138,
    /* end --end */
    HI_FTM_PRODUCT_TYPE_ANY = (HI_FTM_PRODUCT_TYPE_BASE + HI_FTM_PRODUCT_TYPE_RANGE),

    HI_FTM_PRODUCT_TYPE_MAX
} HI_FTM_PRODUCT_TYPE_E;
#else

/**
* @ingroup hct_sys
* @brief 产品形态
*/
typedef enum
{
	HI_FTM_PRODUCT_TYPE_UNKOWN = 0,

		HI_FTM_PRODUCT_TYPE_C01 = (HI_FTM_PRODUCT_TYPE_BASE+1),/**< 集中器载波模块；版本1；单相耦合 */
		HI_FTM_PRODUCT_TYPE_C02,
		HI_FTM_PRODUCT_TYPE_C03,
		HI_FTM_PRODUCT_TYPE_C04,
		HI_FTM_PRODUCT_TYPE_C05,
		HI_FTM_PRODUCT_TYPE_C06,
		HI_FTM_PRODUCT_TYPE_C07,
		HI_FTM_PRODUCT_TYPE_C08,

		HI_FTM_PRODUCT_TYPE_C11 = (HI_FTM_PRODUCT_TYPE_BASE+11),/**< 集中器载波模块；版本1；三相耦合*/
		HI_FTM_PRODUCT_TYPE_C12,
		HI_FTM_PRODUCT_TYPE_C13,
		HI_FTM_PRODUCT_TYPE_C14,
		HI_FTM_PRODUCT_TYPE_C15,
		HI_FTM_PRODUCT_TYPE_C16,
		HI_FTM_PRODUCT_TYPE_C17,
		HI_FTM_PRODUCT_TYPE_C18,

		HI_FTM_PRODUCT_TYPE_R21 = (HI_FTM_PRODUCT_TYPE_BASE+21),/**< 中继器；版本1；单相耦合*/
		HI_FTM_PRODUCT_TYPE_R22,
		HI_FTM_PRODUCT_TYPE_R23,
		HI_FTM_PRODUCT_TYPE_R24,
		HI_FTM_PRODUCT_TYPE_R25,
		HI_FTM_PRODUCT_TYPE_R26,
		HI_FTM_PRODUCT_TYPE_R27,
		HI_FTM_PRODUCT_TYPE_R28,

		HI_FTM_PRODUCT_TYPE_T31 = (HI_FTM_PRODUCT_TYPE_BASE+31),/**< 三相表；版本1；三相耦合*/
		HI_FTM_PRODUCT_TYPE_T32,
		HI_FTM_PRODUCT_TYPE_T33,
		HI_FTM_PRODUCT_TYPE_T34,

		HI_FTM_PRODUCT_TYPE_R35,                                /**< 中继器；版本1；三相耦合*/
		HI_FTM_PRODUCT_TYPE_R36,
		HI_FTM_PRODUCT_TYPE_R37,
		HI_FTM_PRODUCT_TYPE_R38,

		HI_FTM_PRODUCT_TYPE_D41 = (HI_FTM_PRODUCT_TYPE_BASE+41),/**< 抄控器；版本1；单相耦合*/
		HI_FTM_PRODUCT_TYPE_D42,
		HI_FTM_PRODUCT_TYPE_D43,
		HI_FTM_PRODUCT_TYPE_D44,
		HI_FTM_PRODUCT_TYPE_D45,
		HI_FTM_PRODUCT_TYPE_D46,
		HI_FTM_PRODUCT_TYPE_D47,
		HI_FTM_PRODUCT_TYPE_D48,

		HI_FTM_PRODUCT_TYPE_D51 = (HI_FTM_PRODUCT_TYPE_BASE+51),/**< 抄控器；版本8；三相耦合*/
		HI_FTM_PRODUCT_TYPE_D52,
		HI_FTM_PRODUCT_TYPE_D53,
		HI_FTM_PRODUCT_TYPE_D54,
		HI_FTM_PRODUCT_TYPE_D55,
		HI_FTM_PRODUCT_TYPE_D56,
		HI_FTM_PRODUCT_TYPE_D57,
		HI_FTM_PRODUCT_TYPE_D58,

		HI_FTM_PRODUCT_TYPE_D61 = (HI_FTM_PRODUCT_TYPE_BASE+61),/**< 三代抄控器*/
		HI_FTM_PRODUCT_TYPE_D62,
		HI_FTM_PRODUCT_TYPE_D63,
		HI_FTM_PRODUCT_TYPE_D64,

		HI_FTM_PRODUCT_TYPE_M101 = (HI_FTM_PRODUCT_TYPE_BASE+101),/**< 电表/I型采集器 载波模块；版本1*/
		HI_FTM_PRODUCT_TYPE_M102,
		HI_FTM_PRODUCT_TYPE_M103,
		HI_FTM_PRODUCT_TYPE_M104,
		HI_FTM_PRODUCT_TYPE_M105,
		HI_FTM_PRODUCT_TYPE_M106,
		HI_FTM_PRODUCT_TYPE_M107,
		HI_FTM_PRODUCT_TYPE_M108,

		HI_FTM_PRODUCT_TYPE_M111 = (HI_FTM_PRODUCT_TYPE_BASE+111),/**< II型采集器版本1*/
		HI_FTM_PRODUCT_TYPE_M112,
		HI_FTM_PRODUCT_TYPE_M113,
		HI_FTM_PRODUCT_TYPE_M114,
		HI_FTM_PRODUCT_TYPE_M115,
		HI_FTM_PRODUCT_TYPE_M116,
		HI_FTM_PRODUCT_TYPE_M117,
		HI_FTM_PRODUCT_TYPE_M118,
		/* start- 以下ID号由于兼容性问题，仅用于硬件ID，不可用于软件ID -start */
		HI_FTM_PRODUCT_TYPE_M119,
		HI_FTM_PRODUCT_TYPE_M120,
		HI_FTM_PRODUCT_TYPE_M121,
		HI_FTM_PRODUCT_TYPE_M122,
		HI_FTM_PRODUCT_TYPE_M123,
		HI_FTM_PRODUCT_TYPE_M124,
		HI_FTM_PRODUCT_TYPE_M125,
		HI_FTM_PRODUCT_TYPE_M126,

		HI_FTM_PRODUCT_TYPE_M131 = (HI_FTM_PRODUCT_TYPE_BASE+131),/**< 电表/I型采集器 载波模块；版本9*/
		HI_FTM_PRODUCT_TYPE_M132,
		HI_FTM_PRODUCT_TYPE_M133,
		HI_FTM_PRODUCT_TYPE_M134,
		HI_FTM_PRODUCT_TYPE_M135,
		HI_FTM_PRODUCT_TYPE_M136,
		HI_FTM_PRODUCT_TYPE_M137,
		HI_FTM_PRODUCT_TYPE_M138,
		/* end --end */
		HI_FTM_PRODUCT_TYPE_ANY = (HI_FTM_PRODUCT_TYPE_BASE + HI_FTM_PRODUCT_TYPE_RANGE),

		HI_FTM_PRODUCT_TYPE_MAX
} HI_FTM_PRODUCT_TYPE_E;

#endif

#define HI_FTM_PRODUCT_TYPE_C21   HI_FTM_PRODUCT_TYPE_R21
#define HI_FTM_PRODUCT_TYPE_C28   HI_FTM_PRODUCT_TYPE_R28
#define HI_FTM_PRODUCT_TYPE_C31   HI_FTM_PRODUCT_TYPE_T31
#define HI_FTM_PRODUCT_TYPE_C34   HI_FTM_PRODUCT_TYPE_T34
#define HI_FTM_PRODUCT_TYPE_C38   HI_FTM_PRODUCT_TYPE_R38
#define HI_FTM_PRODUCT_TYPE_C41   HI_FTM_PRODUCT_TYPE_D41
#define HI_FTM_PRODUCT_TYPE_C48   HI_FTM_PRODUCT_TYPE_D48
#define HI_FTM_PRODUCT_TYPE_C58   HI_FTM_PRODUCT_TYPE_D58
#define HI_FTM_PRODUCT_TYPE_S101  HI_FTM_PRODUCT_TYPE_M101
#define HI_FTM_PRODUCT_TYPE_S107  HI_FTM_PRODUCT_TYPE_M107
#define HI_FTM_PRODUCT_TYPE_S108  HI_FTM_PRODUCT_TYPE_M108
#define HI_FTM_PRODUCT_TYPE_S111  HI_FTM_PRODUCT_TYPE_M111
#define HI_FTM_PRODUCT_TYPE_S118  HI_FTM_PRODUCT_TYPE_M118
//*****************************************************************************

typedef struct
{
    HI_FTM_HW_PLATFORM_TYPE_E enHwPlatform; // 平台类型
    HI_FTM_CHIP_TYPE_E enChipType;          // 芯片类型
    HI_FTM_PRODUCT_TYPE_E enProductType;    // 产品形态
    HI_U8A ucPlcMac[HI_PLC_MAC_ADDR_LEN]; // 通信模块MAC地址
    HI_U8 ucEthMac[HI_PLC_MAC_ADDR_LEN]; // 网口的MAC地址
    HI_U8 ucEthIp[HI_IPV4_ADDR_LEN]; // 网口的IP地址, 默认V4  192.168.1.100
    HI_U8 ucEthMask[HI_IPV4_ADDR_LEN];  //网口的子网掩码
} HI_NV_FTM_PRODUCT_ID_S;



#define HI_SYS_TEST_MODE_EQM_NONE      0
#define HI_SYS_TEST_MODE_EQM_STA_M     1
#define HI_SYS_TEST_MODE_EQM_STA_IIC   2
#define HI_SYS_TEST_MODE_EQM_NDM       4
#define HI_SYS_TEST_MODE_EQM_NSTA      8

#ifndef PRODUCT_CFG_ALONE_NV_FILE_UPG
typedef struct
{
    HI_U32 ulStatus;         // HI_ND_SYS_STATUS_NORMAL HI_ND_SYS_STATUS_UPGRADE HI_ND_SYS_STATUS_TEST  HI_ND_SYS_STATUS_*
    HI_U32 ulLoadFlashAddr;  // 启动区地址
    HI_U32 ulLastBlkIdx;     // 最后一个连续有效升级文件块的索引号
    HI_U32 ulRcvFileSize;    // 接收的升级文件长度(指连续的有效块组成的大小)
    HI_U32 ulUpgId;
    HI_U32 ulUpgWindowTime;
    HI_U32 ulBackTimes;   //记录回退的次数
    HI_U32 ulRuntimes;    //boot中用来记录启动OS和应用的次数。
    HI_U32 reserved[2];
} HI_NV_FTM_STARTUP_CFG_S;
#else
//HI_NV_FTM_STARTUP_CFG_S中ucUpgTarget可配置值
#define DRV_UPG_CLEAR    0
#define DRV_UPG_NV_ALONE 1
#define DRV_UPG_CODE_NV  2
#define DRV_UPG_BOOT     3

typedef struct
{
    HI_U32 ulStatus;         // HI_ND_SYS_STATUS_NORMAL HI_ND_SYS_STATUS_UPGRADE HI_ND_SYS_STATUS_TEST  HI_ND_SYS_STATUS_*
    HI_U32 ulLoadFlashAddr;  // 启动区地址
    HI_U32 ulLastBlkIdx;     // 最后一个连续有效升级文件块的索引号
    HI_U32 ulRcvFileSize;    // 接收的升级文件长度(指连续的有效块组成的大小)
    HI_U32 ulUpgId;
    HI_U32 ulUpgWindowTime;
    HI_U32 ulBackTimes;   //记录回退的次数
    HI_U32 ulRuntimes; //boot中用来记录启动OS和应用的次数。
    HI_U8  ucUpgTarget;     //升级NV，还是CODE+NV
    HI_U8  ucTestMode; // HI_SYS_TEST_MODE_EQM_STA_M, HI_SYS_TEST_MODE_EQM_*
    HI_U8  ucDecompressNv;//当读取到的标志位0x55的时候解压nv
    HI_U8  reserved1[1];
    HI_U32 reserved2;
} HI_NV_FTM_STARTUP_CFG_S;
#endif

//升级开始信息,来自于Start命令发送来的
typedef struct
{
    HI_U32 ulOption;    //升级选项
    HI_U32 ulUpgId;     //升级ID
    HI_U32 ulFileLen;   //升级文件的长度
    HI_U32 ulBlkNum;    //块个512字节为一个单位
    HI_U32 ulUpgTimeWindow; //升级时间窗
    HI_U32 ulLastBlockId;   //上一个BLOCK的ID
    HI_U32 ulCrc;
}HI_NV_FTM_UPG_START_CFG_S;

/**
 *@ingroup hct_upg
 * 开始升级时用户传给UPG的参数配置。默认填0即可。
*/
typedef struct
{
    HI_U16 total_number_when_start;    /**<开始升级时，网络内站点总数。成功启动升级时，保存此值。*/
    HI_U16 extra_timer_max_count; /**<暂未使用*/
    HI_U16 extra_timer_period;    /**<暂未使用。*/
    HI_U16 upg_verify_new_policy; /**<暂未使用。*/
    HI_U16 total_number_threshold; /**<暂未使用。*/
    HI_U16 rcv_timeout_timer_period; /**<HSO与单板交互超时时间。*/
    HI_U16 blk_send_period; /**<暂未使用。*/
    HI_U8  reserved2;
    HI_U8  upg_type; /**<升级类型为boot时,该值为0xAB。升级类型为code时，该值为1。*/
    HI_U8  device_upg_status;   /**<设备升级状态,升级过程中如果CCO/STA重启,启动后便于识别到设备升级状态。取值含义如下：\n
								0表示空闲状态。\n
								1表示正在升级状态。\n
								2表示升级成功状态。\n
								3表示升级失败状态。*/
    HI_U8  transmit_blk_num;    /**<暂未使用。*/
    HI_U16 mac_tx_arq_period;   /**<暂未使用。*/
    HI_U16 start_tx_period_num; /**<暂未使用。*/
    HI_U16 transmit_blk_resend_num; /**<暂未使用。*/
    HI_U16 broadcast_stop_num;      /**<暂未使用。*/
    HI_U16 broadcast_stop_period;   /**<暂未使用。*/
    HI_U16 sta_dfx_report;          /**<暂未使用。*/
    HI_U16 cco_dfx_report;          /**<暂未使用。*/
    HI_U16 broadcast_finish_num;    /**<暂未使用。*/
    HI_U16 broadcast_finish_period; /**<暂未使用。*/
    HI_U16 reserved;                /**<暂未使用。*/
    HI_U16 whether_upg_test_mode;   /**<暂未使用。*/
}HI_NV_FTM_UPG_CFG_S;


typedef struct _DIAG_CMD_CONFIG_NM_SERIAL_NUMBER_
{
    HI_U32 form_serial_number;
    HI_U32 assoc_random_number;
}DIAG_CMD_CONFIG_NM_SERIAL_NUMBER_STRU;



// 网管的工厂区NV参数
typedef struct
{
    HI_U8   main_network_snid;      // 主网络网络号
    HI_U8   init_freq;              // 初始化频段
    HI_U8   sub_network_snid;       // 子网络网络号
    HI_U8   network_mode;           // 网络模式

    HI_U8   network_mode_policy;    // 网络模式策略
    HI_U8   pa_switch;              // PA开关,0开PA,1关PA
    HI_U8   rf_channel;             // RF信道
    HI_U8   rf_rate;                // RF速率

    HI_U8   is_dual_module;         // 是否为双模模块，仅用于可维可测
    HI_U8   tx_power;               // RF发射功率
    HI_U8   modem_type;             // RF调制类型2FSK:0,2GFSK:1
    HI_U8   white_list_mode;        // 白名单的模式

    HI_U8   sta_type;               // sta种类
    HI_U8   recover_sg_freq;        // 恢复为国网协议时的频段
    HI_S16  correction_voltage;     // 过零点校正电压

    HI_U32  main_network_nid;
    HI_U32  sub_network_nid;

    HI_U8   lock_cco_mac[HI_PLC_MAC_ADDR_LEN];      // 锁定CCO的MAC地址
    HI_U8   belong_cco_addr[HI_PLC_MAC_ADDR_LEN];   // 归属网络CCO的地址
    HI_U8   reserved[6];
    HI_BOOL is_belong_main_node_addr;               // 是否归属主节点地址
    HI_BOOL not_recover_sg_from_v100    :1;          // 不从V100协议恢复到高协议
    HI_U8   reserved1                  :7;

    HI_U32  reserved2[5];
}HI_NV_FTM_NM_INIT_PARA_S;

typedef struct
{
    HI_U8   succ_ratio;          // 传输完成站点半分比
    HI_U8   blk_per_pkt;          //发送的一个包中的块数， 每个块按 UPG_BLOCK_SIZE 计
    HI_U8   query_switch;         //首次是否查询开关
    HI_U8   query_count;        // 最大查询次数

    HI_U16  data_loop_max;       //数据发送最大轮次，
    HI_U16  data_send_interval;  //包和包之间发送间隔 ms
    HI_U16  query_send_interval;  //查询帧发送间隔 ms
    HI_U16  query_max_ways;       //最大并发查询数

    HI_U8   exception_max_count; // 容忍的最大EXCEPTION次数
/* BEGIN:PN:DTS2014092002573 全网升级状态查询统计量优化 MODIFY\ADD  by chenli/00238131 at 2014/9/14*/
    //增加STA存在FLASH中的统计量 19个字节
    HI_U8   dfx_sta_reply_status_report_cnt;//STA主动上报状态查询次数
    HI_U8   dfx_sta_rcv_status_cnt;         //STA接收到状态查询的次数
    HI_U8   dfx_sta_reply_status_suc_cnt;   //STA应答状态查询成功次数

    HI_U8   dfx_sta_reply_status_fail_cnt;  //STA应答状态查询失败次数
    HI_U8   dfx_sta_reply_status_full_cnt;  //STA应答状态查询满的次数
    HI_U16  dfx_sta_reply_status_LRC;       //STA最后一次应答状态查询的结果
    HI_U32  dfx_sta_reply_rcv_file_size_LT; //STA最后一次上报状态查询时接收文件大小

    HI_U16  dfx_sta_rcv_start_suc_LTS;  //STA最后一次成功处理START帧的时间戳
    HI_U16  dfx_sta_rcv_blk_LTS;          //STA最后一次接收到数据包的时间戳
    HI_U16  dfx_sta_reply_status_suc_LTS;//STA最后一次成功应答状态查询的时间戳
    HI_U16  dfx_sta_rcv_affirm_LTS;  //STA最后一次接收到全网完成通知的时间戳
/* END:PN:DTS2014092002573 全网升级状态查询统计量优化  MODIFY\ADD  by chenli/00238131 at 2014/9/14*/
}UPG_BROADCAST_CFG_STRU; // 32

typedef struct
{
    HI_U8  succ_ratio;              // 传输完成站点半分比
    HI_U8  blk_per_pkt;             //发送的一个包中的块数， 每个块按 UPG_BLOCK_SIZE 计
    HI_U8  query_switch;            //首次是否查询开关
    HI_U8  query_count;            // 最大查询次数

    HI_U16 data_loop_max;          //数据发送最大轮次，
    HI_U16 data_max_ways;           //同时并发的最大路数
    HI_U16 data_send_internal;      //包和包之间发送间隔 ms
    HI_U16 query_send_interval;     //查询帧发送间隔 ms
    HI_U16 query_max_ways;          //最大并发查询数
    HI_U16 dfx_cco_snd_affirm_cnt;  //CCO发送全网完成通知的次数

    HI_U8   exception_max_count; // 容忍的最大EXCEPTION次数
    HI_U8  dfx_cco_set_query_exception_sta_mac[3];  //最后一个被置为升级状态异常站点的MAC地址末3位

    HI_U16 dfx_cco_set_query_exception_sta_cnt;     //被置为升级状态异常站点总个数
    HI_U16 dfx_cco_first_rcv_start_time_stamp;     //CCO第一次成功调用START函数的时间戳

    HI_U16  dfx_cco_entry_bc_time_stamp;            //CCO进入广播策略的时间戳
    HI_U16  dfx_cco_entry_u2b_time_stamp;           //CCO进入U2B策略的时间戳
    HI_U16  dfx_cco_entry_uc_time_stamp;            //CCO进入单播策略的时间戳
    HI_U16  dfx_cco_snd_affim_time_stamp;           //CCO开始发送全网完成通知的时间戳
}UPG_U2B_CFG_STRU; // 32 不能超过32字节

typedef struct
{
    HI_U8   succ_ratio;          // 传输完成站点半分比
    HI_U8   blk_per_pkt;          //发送的一个包中的块数， 每个块按 UPG_BLOCK_SIZE 计
    HI_U8   query_switch;         //首次是否查询开关
    HI_U8   query_count;         // 最大查询次数， 配合 exception_max_count使用

    HI_U16  data_loop_max;     // 数据发送最大轮次，当达到该数时,exceptio加1，
                                // 当exception达到exception_max_count
                                // 后将该站点置为exception.
    HI_U16  data_max_ways;       //同时并发的最大路数
    HI_U16  data_send_interval;  //包和包之间发送间隔 ms
    HI_U16  query_send_interval;  //查询帧发送间隔 ms
    HI_U16  query_max_ways;       //最大并发查询数
    HI_U16  dfx_cco_rcv_status_report_cnt;//CCO接收到STA主动上报的状态查询总次数

    HI_U8   exception_max_count; // 容忍的最大EXCEPTION次数
/* BEGIN:PN:DTS2014092002573 全网升级状态查询统计量优化 MODIFY\ADD  by chenli/00238131 at 2014/9/14*/
    HI_U8   dfx_cco_snd_query_suc_sta_mac[3];   //CCO最后一次发送状态查询站点的MAC地址末3位

    HI_U16  dfx_cco_snd_status_suc_cnt;         //CCO发送状态查询成功次数
    HI_U16  dfx_cco_snd_status_fail_cnt;        //CCO发送状态查询失败次数

    HI_U16  dfx_cco_rcv_status_reply_cnt;        //CCO接收到状态查询应答总次数
    HI_U16  dfx_cco_rcv_status_reply_discard_cnt;//CCO接收到状态查询丢弃总次数
    HI_U32  dfx_cco_upg_id;                      //当前的升级ID
/* END:PN:DTS2014092002573 全网升级状态查询统计量优化  MODIFY\ADD  by chenli/00238131 at 2014/9/14*/
}UPG_UNICAST_CFG_STRU; // 32  不能超过32字节

typedef struct
{
    HI_U8 delayed_for_send_restart; // 策略执行完成后等待多久发送重启命令. 单位(m)
    HI_U8 delayed_for_recv_to_restart; // sta 收到重启命令后 延时多久重启 . 单位(s)
    HI_U8 send_count;          // 发送几轮命令
    HI_U8 dfx_sta_rcv_affirm_cnt; //STA接收到全网完成通知的次数
    HI_U16 send_interval;      //包和包之间发送间隔 ms
    HI_U16 dfx_sta_rcv_start_FRC; //第一次调用开始升级函数的返回值
}UPG_AFFIRM_CFG_STRU; // 8

typedef struct
{
    HI_U8 delayed; //发送start 帧等待多久 发送数据. 单位s
    HI_U8 send_count; // 发送几轮命令
    HI_U16 send_interval; //包和包之间发送间隔 ms
    HI_U32 send_ways; // 单播并发时 并发路数。 //
}UPG_NOTIFY2START_CFG_STRU;// 8

typedef struct
{
    HI_U8 broadcast_switch; //是否采用全网广播的开关
    HI_U8 u2b_switch;       //是否采用单播转本地广播的开关
    HI_U8 unicast_switch;   //是否采用并发单播的开关
    HI_U8 slot_type;   // 0: 纯 B-CSMA， 1: 纯 TDMA, 2: B-CSMA 和 TDMA 混合

    UPG_BROADCAST_CFG_STRU bc_cfg; //全网广播配置
    UPG_AFFIRM_CFG_STRU affirm_cfg; // 重启命令
    UPG_NOTIFY2START_CFG_STRU notify2start_cfg; // start 配置在

    HI_U8 query_loop_interval; // 发送一组查询报文后等待时间 单位s.
    HI_U8 data_loop_interval; // 发送一轮 数据完成后等待的时间 s
    HI_U8 switch_schedule_delay; // 策略调度切换时间 单位 100ms
    HI_U8 upg_mode;
/* BEGIN:PN:DTS2014092002573 全网升级状态查询统计量优化 MODIFY\ADD  by chenli/00238131 at 2014/9/15*/
    HI_U32 dfx_sta_upg_id;       //当前的升级ID
    HI_U32 dfx_sta_upg_file_size;//当前升级文件大小
/* END:PN:DTS2014092002573 全网升级状态查询统计量优化  MODIFY\ADD  by chenli/00238131 at 2014/9/15*/
}HI_NV_FTM_UPG_SCHEDL_DEFAULT_CFG_S;

typedef struct
{
    UPG_U2B_CFG_STRU u2b_cfg;      //单播转本地广播配置
    UPG_UNICAST_CFG_STRU uc_cfg;   //并发单播配置
}HI_NV_FTM_UPG_SCHEDL_ADVANCE_CFG_S;

typedef struct
{
    HI_U8   CCTAddress[2];  // 注册集中器逻辑地址，默认值0xFFFF
    HI_U8   CLTAddress[6];  // 采集器资产号(地址)，默认值0xBBBBBBBBBBBB
    HI_U8   meter[6];       // 搜到的首块电能表地址，该地址将作为II型采集器的MAC地址入网
    HI_U8   protocol;       // 电表规约类型
    HI_U8   CCT_register;   // 是否注册标识，默认值0
    HI_U16  reset_times;    // 终端复位次数
    HI_U8   reserved1;      // v1r1版本中扩展已占用，兼容性（工装factory_mode）。
    HI_U8   eqt_result;     // 工装检验结果；第0、1位表示产线工装，第2、3位表示出厂工装，按位赋值:
                            // 00:默认值
                            // 01:工装测试合格
                            // 10:工装测试不合格
                            // 11:无效值
    HI_U16  DI_v97;         // 首块表地址对应的搜表DI值(97表有效)
    HI_S8   duty_cycle_index;//占空比阈值索引，大于NV配置的有效数量时，为默认占空比策略。默认值-1
    HI_U8   baudrate_index; // 首块表波特率索引
    HI_U8  isTsta;
    HI_U8   reserved[7];    // 保留字段，默认为0
} HI_NV_FTM_COLLECT_PARAM_FILE_S;


typedef struct
{
    HI_U32 reserved[16];
} HI_NV_FTM_RESERVED1_ID_S;

typedef struct
{
    HI_U32 reserved[16];
} HI_NV_FTM_RESERVED2_ID_S;

typedef struct
{
    HI_U32 reserved[16];
} HI_NV_FTM_RESERVED3_ID_S;

typedef struct
{
    HI_U32 reserved[16];
} HI_NV_FTM_RESERVED4_ID_S;

typedef struct
{
    HI_U8 ucDevAddr[6];     // CCO设备地址
    HI_U8 ucLrMode;         // 轮抄模式: 0-单轮模式(默认) 1-连续模式
    HI_U8 ucSeqCtrl;        // 从节点序列号
    HI_U8 ucFuncCfg;        //地方版本规约。0:通用版本;1:江苏波特率自适应版本;其他预留
    HI_U8 ucMaxByteIntervalTimeBbu;     // 时间= 数值 * 10ms
    HI_U8 ucFileFlg;        //文件标识 广东版本使用
    HI_U8 ucUpgId;         // 升级文件ID 与广东版本复用
    HI_U8 ucFileId;     // 升级文件ID 广东版本集中器下发的升级ID
    HI_U8 pad1;
    HI_U16 pad2;
    HI_U32 reserved[12];
} HI_NV_FTM_CCO_PARAM_ID_S;

typedef struct
{
    HI_U8 management_id[24];      // 芯片id
    HI_U8 dev_id[11];             // 设备id  MRS_DEV_ID_SIZE
    HI_U8 pad;
    HI_U32 reserved[7];
}HI_NV_FTM_MANAGEMENT_ID_S;

typedef struct
{
    HI_U32 reserved[16];
}HI_NV_FTM_RESERVED9_S;
//*****************************************************************************


//*****************************************************************************
#endif //__FT_NV_H__


