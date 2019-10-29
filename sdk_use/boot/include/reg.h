#ifndef __REG_H__
#define __REG_H__

#define HI_INT_TABLE_BASE   (0x00000000)

#define HI_SYSCTRL_REG_BASE (0x101e0000)
#define HI_TIMER1_BASE      (0x101e2000)
#define HI_TIMER2_BASE      (0x101e2014)
#define HI_SFC_REG_BASE     (0x10130000)
#define HI_VIC_BASE         (0x10140000)
#define HI_DMAC_BASE        (0x10150000)
#define HI_GPIO_SEL_BASE    (0x101e6000)
#define HI_ETH_BASE         (0x10180000)
#define HI_CIPHER_REG_BASE  (0x10190000)
#define HI_PHY_BASE         (0x101A0000)
#define HI_PERIPHERAL_BASE  (0x101E0000)
#define HI_WDG_REG_BASE     (0x101E1000)
#define HI_LED_REG_BASE     (0x101E9000)
#define HI_SWITCH_REG_BASE  HI_LED_REG_BASE
#define HI_TIMECNT_REG_BASE     (0x101EA000)
#define HI_UART0_REG_BASE   (0x101EC000)
#define HI_UART1_REG_BASE   (0x101ED000)
#define HI_UART2_REG_BASE   (0x101EE000)
#define HI_UART3_REG_BASE   (0x101EF000)
#define HI_IR_REG_BASE     HI_LED_REG_BASE
#define GPIO_PHY_BASE           0x101E3000

//gpio reg
#define GPIO_SWPORT_DR          0x00
#define GPIO_SWPORT_DDR         0x04

//general reg
#define GENERAL_REG_1                   (0x101e0200)
#define GENERAL_REG_2                   (0x101e0204)
#define GENERAL_REG_3                   (0x101e0208)
#define GENERAL_REG_FLASH_PROTECT_MAGIC (0x101e020C)
#define GENERAL_REG_V200_FLAG           (0x101e0210)
#define GENERAL_REG_BOOT_VER            (0x101e0214)
#define GENERAL_REG_ENV_ADDR1           (0x101e0218)
#define GENERAL_REG_ENV_ADDR2           (0x101e021C)

//sys reg offset
#define SC_CTRL         (0x00)
#define SC_SYS_RES      (0x04)
#define SC_PLLCTRL      (0x14)
#define SC_PERI_CLKEN   (0x20)
#define SC_PERI_CLKSEL  (0x28)
#define SC_WDG_RST_CTRL (0x3c)
#define SC_SLEEP_CTRL   (0x48)
#define SC_VERSION_ID   (0x50)
#define SC_HW_ID        (0x54)
#define SC_PHY_WRAP_CLK_EN  (0x74)
#define ENET_SLEEP_CTRL  (0x90)
#define SFC_CLK_DIV_2   (2<<8)
#define SFC_CLK_SEL     (1<<4)

//WDG reg offset
#define WDT_CR   (0x00)
#define WDT_TORR   (0x04)
#define WDT_CRR   (0x0c)

//led reg offset
#define LED_PH_CTRL  (0x00)
#define LED_START   (0x64)

//time cnt
#define TIME_S                   (0x0)
#define TIME_MS                  (0x4)
#define TIME_US                  (0x8)
#define TIME_CNT_CTRL            (0xC)
#define TIME_RD_EN               (1 << 1)
#define TIME_CNT_EN              (1 << 0)
#define TIME_RD_EN_WAIT_MAX      (10000)

#define SYS_BASE_SC_VERSION_ID_REG  (HI_SYSCTRL_REG_BASE + SC_VERSION_ID)
#define SYS_SC_VERSION_ID_BIT_TYPE_ID_CHIP_T (1 << 1)

/* macros */ 
#define HI_REG_WRITE(addr, val) (*(volatile unsigned int *)(addr) = (val))
#define HI_REG_READ(addr, val) ((val) = *(volatile unsigned int *)(addr))

#endif//__REG_H__