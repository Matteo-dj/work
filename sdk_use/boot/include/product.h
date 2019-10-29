#ifndef _PRODUCT_H_
#define _PRODUCT_H_

#include <hi_product_config.h>
#include <types.h>

/* 晶振设置 */
#ifdef PRODUCT_CFG_BOARD_TYPE_ASIC
#define HI_XTAL_CLOCK_BASE (25)//晶振时钟，timer和led时钟
#else   //FAGA
#ifdef  PRODUCT_CFG_BOARD_FPGA_CLOCK_25M
#define HI_XTAL_CLOCK_BASE   (25)    //晶振时钟，timer和led时钟
#else
#define HI_XTAL_CLOCK_BASE   (37.5)    //晶振时钟，timer和led时钟
#endif
#endif

#define HI_XTAL_CLOCK_REAL 25000000

#define CONFIG_PL011_CLOCK   66500000
#define CONFIG_TIMER_CLOCK   25000000
#define CONFIG_AHB_CLOCK     133000000

/* 看门狗时间等配置 */
#define WATCHDOG_TIME (30000000)//30s
#define CONFIG_BAUDRATE      (115200)

/* 地址信息 */
#define CFG_LOAD_ADDR (0x70008000) 
#define BOOT_AREA_SIZE          PRODUCT_CFG_BOOT_FLASH_SIZE_U
#define CFG_UPG_DATA_TEMP_ADDR  PRODUCT_CFG_BOOT_DYNAMIC_ADDR_END_U
#define CFG_BOOT_TEXT_ADDR		PRODUCT_CFG_LD_BOOTLOADER_TEXT_BASE
#define CFG_DECOMPRESSED_SIZE	(CFG_BOOT_TEXT_ADDR - CFG_LOAD_ADDR)
#define CFG_FLASH_BLOCK_SIZE    PRODUCT_CFG_FLASH_BLOCK_SIZE
#define NV_AREA_SIZE            PRODUCT_CFG_NV_MAX_SIZE 

/* 版本相关 */
#define VERSION_VRCB            PRODUCT_CFG_FULL_VERSION_STR
#define VERSION_BOOT            PRODUCT_CFG_BOOTLOADER_VERSION_STR
#define BUILD_DATE              PRODUCT_CFG_BUILD_DATE
#define BUILD_TIME              PRODUCT_CFG_BUILD_TIME


/* 升级相关 */
#ifdef PRODUCT_CFG_SUPPORT_UPG
#define BOOT_SUPPORT_UPG
#endif

//其他产品形态
#if defined(PRODUCT_CFG_EQUIP_TEST_MODE)
#define SUPPORT_EQUIP_TEST      PRODUCT_CFG_EQUIP_TEST_MODE
#endif

#define HI_ND_SYS_STATUS_NORMAL      0 // 正常模式
#define HI_ND_SYS_STATUS_UPGRADE     1 // 升级模式

#define FILE_AREA_SIZE          PRODUCT_CFG_UPG_FILE_MAX_SIZE
#define NV_FILE_SIZE_MAX        PRODUCT_CFG_NV_FILE_SIZE_MAX //NV文件大小

#define CFG_UPG_NV_TEMP_FILE    (CFG_UPG_DATA_TEMP_ADDR + FILE_AREA_SIZE)
#define TIME_UPG_REBOOT         2
#define PRODUCT_CFG_NV_FILE_SIZE_MAX	PRODUCT_CFG_NV_FILE_SIZE_MAX_NEW

#define CFG_BOOT_VERSION				PRODUCT_CFG_BOOT_VERSION_ADDR_U  
#define PRODUCT_CFG_UPG_FILE_MAX_SIZE	PRODUCT_CFG_UPG_FILE_MAX_SIZE_NEW_U

/* set user input wait timeout when fastboot startup, it's value from 0 to 9 */
#define CONFIG_FASTBOOT_DELAY          (0)

typedef struct
{
    u8 ucVVerNo;     // V部分
    u8 ucRVerNo;     // R部分
    u16 usBVerNo;    // B部分
    u8 ucCVerNo;     // C部分
    u8 ucSpcNo;      // SPC部分
    u8 ucPlatform;   // 平台编号
    u8 ucProductNo;  // 产品编号
} HI_UPG_FILE_VER_S;

typedef struct
{
    u32 ulId;                 // Section ID值, 0表示不使用该Section
    u32 ulOffset;             // Section 相对 ucData 的偏移
    u32 ulSectionSize;        // Section长度, 单位字节
    HI_UPG_FILE_VER_S stVer;
} HI_UPG_SECTION_ID_S;

typedef struct
{
    HI_UPG_SECTION_ID_S astCfg[8]; // 同时能支持的NV配置文件
    //u32 ulFlashAddress;        // 该Section保存到Flash的地址, 目前版本不使用，该字段预留.
} HI_UPG_FILE_CFG_SECTION_S;

typedef struct
{
    HI_UPG_SECTION_ID_S astCfg[8]; // 同时能支持的CODE文件
    u32 ulRamAddress;          // 该Section加载到RAM的地址, 取值为 HI_UPG_FILE_CODE_RAM_ADDR_AUTO 表示不指定, 由模块自己确定RAM地址
} HI_UPG_FILE_CODE_SECTION_S;

typedef struct
{
    u32 ulSectorSize;
    u32 aulCodeAddr[2];
    u32 ulCodeAreaSize;
    u32 ulNvAddr;
    u32 ulNvAreaSize;
    u16 usNvItemSize;
    u16 reserved;
    u16 ausBoardType[2];
} HI_UPG_BOARD_CONFIG_S;

#define HI_UPG_FILE_HEADER_SIZE  396
typedef struct
{
    u32 ulMagic;                           // 通过 HI_UPG_FILE_SIGNATURE 组合该字段
    u32 ulChecksum;                        // Checksum
    char szDateTime[20];                   // 升级文件生成的日期和时间 如 2012-05-17 18:01:00（包括'\0'结尾字符）
    u8 ucMachine;                          // 目标机器, 取值为 HI_UPG_FILE_MACHINE_XXX定义
    u8 ucNumberOfCodeSections;             // 代码Section个数
    u8 ucNumberOfCfgSections;              // 配置Section个数
    u8 ucFmtVer;                           // 文件格式版本， 取值为 HI_UPGA_FILE_FMT_VER_1、其它值
    HI_UPG_FILE_CFG_SECTION_S stSecCfg;    // 配置Section 列表
    char szManuCode[2];                    // HS代表海思
    u16 reserved;                          // 保留
    HI_UPG_FILE_CODE_SECTION_S stSecCode;  // 代码Section列表
    u32 aulSHA256[8];                      // 升级文件数据SHA256校验.
    u32 ulDataSize;                        // 所有Section总长度, 单位字节
} HI_UPG_FILE_HEADER_S;

#endif /*_PRODUCT_H_*/ 
