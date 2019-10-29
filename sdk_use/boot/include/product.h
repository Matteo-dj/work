#ifndef _PRODUCT_H_
#define _PRODUCT_H_

#include <hi_product_config.h>
#include <types.h>

/* �������� */
#ifdef PRODUCT_CFG_BOARD_TYPE_ASIC
#define HI_XTAL_CLOCK_BASE (25)//����ʱ�ӣ�timer��ledʱ��
#else   //FAGA
#ifdef  PRODUCT_CFG_BOARD_FPGA_CLOCK_25M
#define HI_XTAL_CLOCK_BASE   (25)    //����ʱ�ӣ�timer��ledʱ��
#else
#define HI_XTAL_CLOCK_BASE   (37.5)    //����ʱ�ӣ�timer��ledʱ��
#endif
#endif

#define HI_XTAL_CLOCK_REAL 25000000

#define CONFIG_PL011_CLOCK   66500000
#define CONFIG_TIMER_CLOCK   25000000
#define CONFIG_AHB_CLOCK     133000000

/* ���Ź�ʱ������� */
#define WATCHDOG_TIME (30000000)//30s
#define CONFIG_BAUDRATE      (115200)

/* ��ַ��Ϣ */
#define CFG_LOAD_ADDR (0x70008000) 
#define BOOT_AREA_SIZE          PRODUCT_CFG_BOOT_FLASH_SIZE_U
#define CFG_UPG_DATA_TEMP_ADDR  PRODUCT_CFG_BOOT_DYNAMIC_ADDR_END_U
#define CFG_BOOT_TEXT_ADDR		PRODUCT_CFG_LD_BOOTLOADER_TEXT_BASE
#define CFG_DECOMPRESSED_SIZE	(CFG_BOOT_TEXT_ADDR - CFG_LOAD_ADDR)
#define CFG_FLASH_BLOCK_SIZE    PRODUCT_CFG_FLASH_BLOCK_SIZE
#define NV_AREA_SIZE            PRODUCT_CFG_NV_MAX_SIZE 

/* �汾��� */
#define VERSION_VRCB            PRODUCT_CFG_FULL_VERSION_STR
#define VERSION_BOOT            PRODUCT_CFG_BOOTLOADER_VERSION_STR
#define BUILD_DATE              PRODUCT_CFG_BUILD_DATE
#define BUILD_TIME              PRODUCT_CFG_BUILD_TIME


/* ������� */
#ifdef PRODUCT_CFG_SUPPORT_UPG
#define BOOT_SUPPORT_UPG
#endif

//������Ʒ��̬
#if defined(PRODUCT_CFG_EQUIP_TEST_MODE)
#define SUPPORT_EQUIP_TEST      PRODUCT_CFG_EQUIP_TEST_MODE
#endif

#define HI_ND_SYS_STATUS_NORMAL      0 // ����ģʽ
#define HI_ND_SYS_STATUS_UPGRADE     1 // ����ģʽ

#define FILE_AREA_SIZE          PRODUCT_CFG_UPG_FILE_MAX_SIZE
#define NV_FILE_SIZE_MAX        PRODUCT_CFG_NV_FILE_SIZE_MAX //NV�ļ���С

#define CFG_UPG_NV_TEMP_FILE    (CFG_UPG_DATA_TEMP_ADDR + FILE_AREA_SIZE)
#define TIME_UPG_REBOOT         2
#define PRODUCT_CFG_NV_FILE_SIZE_MAX	PRODUCT_CFG_NV_FILE_SIZE_MAX_NEW

#define CFG_BOOT_VERSION				PRODUCT_CFG_BOOT_VERSION_ADDR_U  
#define PRODUCT_CFG_UPG_FILE_MAX_SIZE	PRODUCT_CFG_UPG_FILE_MAX_SIZE_NEW_U

/* set user input wait timeout when fastboot startup, it's value from 0 to 9 */
#define CONFIG_FASTBOOT_DELAY          (0)

typedef struct
{
    u8 ucVVerNo;     // V����
    u8 ucRVerNo;     // R����
    u16 usBVerNo;    // B����
    u8 ucCVerNo;     // C����
    u8 ucSpcNo;      // SPC����
    u8 ucPlatform;   // ƽ̨���
    u8 ucProductNo;  // ��Ʒ���
} HI_UPG_FILE_VER_S;

typedef struct
{
    u32 ulId;                 // Section IDֵ, 0��ʾ��ʹ�ø�Section
    u32 ulOffset;             // Section ��� ucData ��ƫ��
    u32 ulSectionSize;        // Section����, ��λ�ֽ�
    HI_UPG_FILE_VER_S stVer;
} HI_UPG_SECTION_ID_S;

typedef struct
{
    HI_UPG_SECTION_ID_S astCfg[8]; // ͬʱ��֧�ֵ�NV�����ļ�
    //u32 ulFlashAddress;        // ��Section���浽Flash�ĵ�ַ, Ŀǰ�汾��ʹ�ã����ֶ�Ԥ��.
} HI_UPG_FILE_CFG_SECTION_S;

typedef struct
{
    HI_UPG_SECTION_ID_S astCfg[8]; // ͬʱ��֧�ֵ�CODE�ļ�
    u32 ulRamAddress;          // ��Section���ص�RAM�ĵ�ַ, ȡֵΪ HI_UPG_FILE_CODE_RAM_ADDR_AUTO ��ʾ��ָ��, ��ģ���Լ�ȷ��RAM��ַ
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
    u32 ulMagic;                           // ͨ�� HI_UPG_FILE_SIGNATURE ��ϸ��ֶ�
    u32 ulChecksum;                        // Checksum
    char szDateTime[20];                   // �����ļ����ɵ����ں�ʱ�� �� 2012-05-17 18:01:00������'\0'��β�ַ���
    u8 ucMachine;                          // Ŀ�����, ȡֵΪ HI_UPG_FILE_MACHINE_XXX����
    u8 ucNumberOfCodeSections;             // ����Section����
    u8 ucNumberOfCfgSections;              // ����Section����
    u8 ucFmtVer;                           // �ļ���ʽ�汾�� ȡֵΪ HI_UPGA_FILE_FMT_VER_1������ֵ
    HI_UPG_FILE_CFG_SECTION_S stSecCfg;    // ����Section �б�
    char szManuCode[2];                    // HS����˼
    u16 reserved;                          // ����
    HI_UPG_FILE_CODE_SECTION_S stSecCode;  // ����Section�б�
    u32 aulSHA256[8];                      // �����ļ�����SHA256У��.
    u32 ulDataSize;                        // ����Section�ܳ���, ��λ�ֽ�
} HI_UPG_FILE_HEADER_S;

#endif /*_PRODUCT_H_*/ 
