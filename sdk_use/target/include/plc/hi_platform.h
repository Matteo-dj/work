/*
 * for HI3911V100 platform .
 *
 * History:
 *      2012-01-14 create by w00190206
 */

#ifndef __HI_PLATFORM_H__
#define __HI_PLATFORM_H__

#include <hi_product_config.h>

#ifdef PRODUCT_CFG_OS_VX
#include "int/esal_pr_int_defs.h"
#endif
#include <hi_reg_sys.h>

HI_EXTERN HI_U32 g_cfg_apb_clock;   
HI_EXTERN HI_U32 g_cfg_arm_clock;
HI_EXTERN HI_U32 g_cfg_uart_clock;

#define HI_XTAL_CLOCK_REAL 25000000
#ifdef PRODUCT_CFG_BOARD_TYPE_ASIC
#define HI_XTAL_CLOCK_BASE (25)//晶振时钟，timer和led时钟
#else   //FAGA
#ifdef  PRODUCT_CFG_BOARD_FPGA_CLOCK_25M
#define HI_XTAL_CLOCK_BASE   (25)    //晶振时钟，timer和led时钟
#else
#define HI_XTAL_CLOCK_BASE   (37.5)    //晶振时钟，timer和led时钟
#endif
#endif

#if  defined(ARMCACHE) && defined(ARMCACHE_926E) && (ARMCACHE == ARMCACHE_926E)
#define CACHE_LINE_SIZE (32)
#else //其他场景根据实际情况设置
#define CACHE_LINE_SIZE (64)
#endif //(ARMCACHE == ARMCACHE_926E)
#ifndef PAGE_SIZE
#define PAGE_SIZE   (4 << 10)
#endif
#define HI_INT_TABLE_BASE   (0x00000000)
#define HI_GPIO_SEL_BASE    (0x101e6000)

#define HI_TIMER1_BASE      (0x101e2000)
#define HI_TIMER2_BASE      (0x101e2014)
#define HI_SFC_REG_BASE     (0x10130000)
#define HI_VIC_BASE         (0x10140000)
#define HI_DMAC_BASE        (0x10150000)
#define HI_ETH_BASE         (0x10180000)
#define HI_CIPHER_REG_BASE  (0x10190000)
#define HI_PHY_BASE         (0x101A0000)
#define HI_PERIPHERAL_BASE  (0x101E0000)

//sys reg base addr define
#define HI_WDG_REG_BASE     (0x101E1000)
#define HI_LED_REG_BASE     (0x101E9000)
#define HI_SWITCH_REG_BASE  HI_LED_REG_BASE
#define HI_CRC_REG_BASE     (0x101EA000)
#define HI_UART0_REG_BASE   (0x101EC000)
#define HI_UART1_REG_BASE   (0x101ED000)
#define HI_UART2_REG_BASE   (0x101EE000)
#define HI_UART3_REG_BASE   (0x101EF000)

#define HI_INT_TABLE_END   (HI_INT_TABLE_BASE+(PAGE_SIZE<<4))
#define HI_SFC_REG_END     (HI_SFC_REG_BASE+(PAGE_SIZE<<4))
#define HI_VIC_END         (HI_VIC_BASE+(PAGE_SIZE<<4))
#define HI_DMAC_END        (HI_DMAC_BASE+(PAGE_SIZE<<4))
#define HI_ETH_END         (HI_ETH_BASE+(PAGE_SIZE<<4))
#define HI_CIPHER_REG_END  (HI_CIPHER_REG_BASE+(PAGE_SIZE<<4))
#define HI_PHY_END         (HI_PHY_BASE+(PAGE_SIZE<<4))
#define HI_PERIPHERAL_END  (HI_PERIPHERAL_BASE+(PAGE_SIZE<<5))


#define HI_IR_REG_BASE     HI_LED_REG_BASE
#define HI_SFC_MEM_BASE     (0x30000000)
#define HI_RAM_MEM_BASE     (0x70000000)
#define HI_SDRAM_SIZE       (0x01000000)//16M
#define HI_FLASH_MAX_ADDR   (PRODUCT_CFG_BASE_FLASH_ADDR + PRODUCT_CFG_CHIP_SUPPORT_FLASH_MAX_SIZE)
#define HI_RAM_MAX_ADDR     (0x70800000)

//sys reg offset


#define HI_TCM_BASE_ADDR (0x0)
#define HI_TCM_SIZE (0x1000)


#define HI_VIC_OFFSET_IRQ_INTEN      0x0
#define HI_VIC_OFFSET_IRQ_INTMASK    0x8
#define HI_VIC_OFFSET_IRQ_INTFORCE   0x10
#define HI_VIC_OFFSET_IRQ_RAWSTATUS  0x18
#define HI_VIC_OFFSET_IRQ_STATUS     0x20
#define HI_VIC_OFFSET_IRQ_MASKSTATUS  0x28
#define HI_VIC_OFFSET_IRQ_FINALSTATUS  0x30
#define HI_VIC_OFFSET_IRQ_PLEVEL       0xD8
#define HI_VIC_OFFSET_APB_ICTL_COMP_VERSION  0x3F8
#define HI_VIC_OFFSET_IRQ_PN                 0xE8





#ifdef PRODUCT_CFG_FEATURE_DEBUG_SHELL



/* enable shell and drv debug log */
#ifndef PRODUCT_CFG_EQUIP_TEST_MODE
#define TERMINAL_DISPLAY
#else
#endif
#endif

//drv debug
//#define DRV_LED_DEBUG

//通用寄存器位置信息
#define GENERAL_REG_FLASH_PROTECT_MAGIC (0x101e020C)
#define GENERAL_REG_V200_FLAG           (0x101e0210)
#define GENERAL_REG_BOOT_VER            (0x101e0214)
#define GENERAL_REG_ENV_ADDR1           (0x101e0218)
#define GENERAL_REG_ENV_ADDR2           (0x101e021C)



#endif  /* __HI_PLATFORM_H__*/

