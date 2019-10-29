#ifndef __FASTBOOT_PLATFORM_H
#define __FASTBOOT_PLATFORM_H

#include <hi_product_config.h>
#include <boot.h>

#ifdef PRODUCT_CFG_BOARD_TYPE_ASIC
#define HI3911_ASIC
#endif

#define HI_XTAL_CLOCK_BASE   (25)    //æß’Ò ±÷”£¨timer∫Õled ±÷”
#ifdef HI3911_ASIC
#define CONFIG_PL011_CLOCK   66500000
#define CONFIG_TIMER_CLOCK   25000000
#define CONFIG_AHB_CLOCK     133000000
#else
#define CONFIG_PL011_CLOCK   37500000
#define CONFIG_TIMER_CLOCK   37500000
#define CONFIG_AHB_CLOCK     37500000
#endif

//sys reg base addr define
#define HI_LED_REG_BASE     (0x101E9000)
#define HI_WDG_REG_BASE     (0x101E1000)

/*----------------------------/
 * Serial Configuration
 *--------------------------*/
#define REG_BASE_UART0       0x101EC000
#define REG_BASE_UART1       0x101ED000
//#define CONFIG_BAUDRATE      115200
//#define CONFIG_SERIAL1       REG_BASE_UART1

/*-----------------------------/
  * crc configuration
  *--------------------------*/
#define REG_BASE_CRC     (0x101EA000)

/*-----------------------------/
  * spi flash configuration
  *--------------------------*/

#define MEM_BASE_SPI 0x30000000
#define REG_BASE_SFC 0x10130000
#define SFC_MEM_BASE MEM_BASE_SPI
#define SFC_REG_BASE REG_BASE_SFC

/*----------------------------/
 * sys register define
 *--------------------------*/
#define CFG_RAM_ADDR (0x70000000) 
#define CFG_LOAD_ADDR (0x70008000) 

#define REG_BASE_SCTL 0x101e0000
#define REG_SC_CTRL   0x00
#define REG_SC_SYSRES 0x04
#define REG_SC_WDG_RST 0x3C
//#define SC_PERI_CLKSEL  0x28
#define SFC_CLK_DIV_2   (2<<8)
#define SFC_CLK_SEL     (1<<4)

//#define SYS_BASE_SC_VERSION_ID_REG  (REG_BASE_SCTL + 0x50)
#define SYS_SC_VERSION_ID_BIT_TYPE_ID_CHIP_T (1 << 1)
#define CHIP_VERSION_MASK (0x0000FFF0)
#define CHIP_VERSION_V100 (0x100)
#define CHIP_VERSION_V200 (0x200)

typedef enum
{
    HI_CHIP_VERSION_UNKNOW  = 0,
    HI_CHIP_VERSION_V100,
    HI_CHIP_VERSION_V200,
} HI_CHIP_VERSION_E;

//#define CONFIG_SYS_MALLOC_LEN (0x40000 +128*1024)
//#define CONFIG_SYS_GBL_DATA_SIZE 128
/*--------------------------/
* timer configuration
*-------------------------*/
#define REG_BASE_TIMER 0x101e2000 
#define REG_BASE_SCTL  0x101e0000
#define REG_BASE_SF    0x10180000
#ifndef TURNOFF_NEWFUNCTION_BEFORE_B020
#define REG_BASE_REMAP		0x101e6000 
#define SD_DHAL_LSR			0x14        /*Line Status */
#define SD_DHAL_LSR_TX_EMPTY		0x00000040  /* THR empty  */
#undef  UART0
#define UART0 0
#undef  UART1
#define UART1 1
#undef  UART2
#define UART2 2
#undef  UART3
#define UART3 3
#define GPIO_SEL_UART 0
#define GPIO_SEL_GPIO 1
#endif

#define TIMER1_LOADCOUNT 0x00
#define TIMER1_CURRENTVALUE 0x04
#define TIMER1_CONTROLREG 0x08


#define CFG_TIMERBASE    REG_BASE_TIMER
/* TIMER BIT MASK */
#define TIMER_DISABLE       0
#define TIMER_ENABLE        1

#define TIMER_FREE_RUNNING  0
#define TIMER_PERIOD        (1<<1)

#define TIMER_INT_UNMASK    0
#define TIMER_INT_MASK      (1<<2)

#define TIMER_FREE_RUNNING_RUN \
    (TIMER_ENABLE | TIMER_FREE_RUNNING | TIMER_INT_UNMASK)

#define TIMER_FREE_RUNNING_STOP \
    (TIMER_DISABLE | TIMER_FREE_RUNNING | TIMER_INT_MASK)

#define CFG_HZ 		(CONFIG_TIMER_CLOCK)  //timer clock configure
#define READ_TIMER      (*(volatile unsigned long *)(CFG_TIMERBASE + TIMER1_CURRENTVALUE))

/* set user input wait timeout when fastboot startup, it's value from 0 to 9 */
#define CONFIG_FASTBOOT_DELAY          (0)
extern unsigned int g_uart_baseaddr;
/*****************************************************************************/

#endif /* __ASM_ARCH_PLATFORM_H */

