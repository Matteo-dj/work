
#define BOOT_TEXT_BASE	(0x70155000)
#define CFG_RAM_ADDR	(0x70000000)
#define CFG_2M_ADDR	(0x70200000)
#define REG_BASE_SCTL	(0x101e0000)
#define REG_SC_CTRL	(0x00)
#define REG_SC_SYSRES	(0x04)
#define REG_SC_WDG_RST  (0x3C)

#define REG_MDDRC_DYNAMIC_CONFIG0     (0x10110100)
#define TEST_RAM_SIZE      (0x12345678)
#define CFG_8M_SIZE         (0x280)

#define SFC_MEM_BASE	(0x30000000)

#define  BOOTLOADER_STACK  (0x70008000 - 4)

#if !defined(BOOT_FILLSIZE)
#error Macro BOOT_FILLSIZE required.
#endif

