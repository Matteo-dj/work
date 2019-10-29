/*----------------------------------------------------------------------------
 * Copyright (c) <2013-2015>, <Huawei Technologies Co., Ltd>
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific prior written
 * permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *---------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
 * Notice of Export Control Law
 * ===============================================
 * Huawei LiteOS may be subject to applicable export control laws and regulations, which might
 * include those applicable to Huawei LiteOS of U.S. and the country in which you are located.
 * Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance with such
 * applicable export control laws and regulations.
 *---------------------------------------------------------------------------*/

#ifndef __ASM_PLATFORM_H__
#define __ASM_PLATFORM_H__

#include "asm/hal_platform_ints.h"

#define DDR_MEM_BASE            0x70000000

#define IO_MUX_REG_BASE     0x200F0000

#define GPIO15_REG_BASE     0x20260000
#define GPIO14_REG_BASE     0x20220000
#define GPIO13_REG_BASE     0x20210000
#define GPIO12_REG_BASE     0x20200000
#define GPIO11_REG_BASE     0x201F0000
#define GPIO10_REG_BASE     0x201E0000
#define GPIO9_REG_BASE      0x201D0000
#define GPIO8_REG_BASE      0x201C0000
#define GPIO7_REG_BASE      0x201B0000
#define GPIO6_REG_BASE      0x201A0000
#define GPIO5_REG_BASE     0x20190000
#define GPIO4_REG_BASE     0x20180000
#define GPIO3_REG_BASE     0x20170000
#define GPIO2_REG_BASE     0x20160000
#define GPIO1_REG_BASE     0x20150000
#define GPIO0_REG_BASE     0x20140000

#define MISC_REG_BASE     0x20120000
#define SPI0_REG_BASE       0x200C0000
#define PMC_REG_BASE        0x20270000  //
#define PWM47_REG_BASE     0x20131000
#define PWM03_REG_BASE     0x20130000
#define SPI_NANDC_REG_BASE     0x10040000
#define SPI1_REG_BASE     0x200E0000
#define NANDC_REG_BASE     0x10000000
#define IO_CTL_REG_BASE     0x200F0800

#define SDIO1_REG_BASE     0x206F0000  //
#define SDIO0_REG_BASE     0x206E0000  //
#define RTC_REG_BASE     0x20060000
#define GSF_REG_BASE     0x10090000

#define GMAC_USE_MEM_SIZE     0x00200000     //2MB
#define MMC_USE_MEM_SIZE     0x0//0x00003000    /12KB
#define SFC_USE_MEM_SIZE     0x0//0x00002000    /8KB
#define NFC_USE_MEM_SIZE     0x0//0x00001000    /4KB
#define DMAC_USE_MEM_SIZE     0x00001000     //4KB
#define NANDC_MEM_BASE     0x50000000  //

#define GMAC_USE_MEM_START     ((0x88000000 + 0x00001000)&(~0xfff)) //4KB align
#define MMC_USE_MEM_START     (GMAC_USE_MEM_START + GMAC_USE_MEM_SIZE)
#define SFC_USE_MEM_START     (MMC_USE_MEM_START + MMC_USE_MEM_SIZE)
#define NFC_USE_MEM_START     (SFC_USE_MEM_START + SFC_USE_MEM_SIZE)
#define DMAC_USE_MEM_START     (NFC_USE_MEM_START + NFC_USE_MEM_SIZE)
#define CRG_REG_BASE     0x20030000
#define FMC_REG_BASE     0x10010000
#define FMC_MEM_BASE     0x58000000

//only one GPIO
#define GPIO_REG_BASE       0x101E3000
#define DHD_REGON_GPIO_BASE       GPIO_REG_BASE
#define DHD_OOB_GPIO_BASE         GPIO_REG_BASE
//fixme
#define DHD_REGON_GPIO_NUM        0
#define DHD_OOB_GPIO_NUM          5


//only one i2c
#define I2C0_REG_BASE     0x101E7000
#define I2C1_REG_BASE     0x101E7000
#define I2C2_REG_BASE     0x101E7000

//sdd
#define UART0_REG_BASE          0x101EC000
#define UART1_REG_BASE          0x101ED000
#define UART2_REG_BASE          0x101EE000
#define UART3_REG_BASE          0x101EF000

#define BIT(n)  (1U << (n))


#define TIMER_ENABLE          BIT(0)
#define TIMER_MODE_PERIODIC   BIT(1)

#define TIMER3_REG_BASE     0x101E203c
#define TIMER2_REG_BASE     0x101E2028
#define TIMER1_REG_BASE     0x101E2014
#define TIMER0_REG_BASE     0x101E2000
#define TIMER_TICK_REG_BASE       TIMER1_REG_BASE   /* timer for tick */

#define SYS_TIMER_NUM            1
#define HR_TIMER_NUM             3

//#define SFC_MEM_BASE                0x00000000
#define MDDRC_REG_BASE           0X10110000
#define SFC_REG_BASE                 0X10130000
#define VIC_BASE                         0x10140000
#define DMAC_REG_BASE              0X10150000
#define ETH_REG_BASE                0X10180000
#define CIPHER_REG_BASE           0X10190000
#define PHY_REG_BASE                0X101A0000
#define PHY_BUFFER_BASE           0X101A5000
#define SYS_CTRL_REG_BASE       0x101E0000

#define WDG_REG_BASE            0x101E1000
#define GPIO_REG_BASE            0x101E3000
#define SPI_MASTER_BASE         0x101E5000
#define IO_CONFIG_BASE           0x101E6000
#define I2C_REG_BASE               0x101E7000
#define SPI_SLAVE_BASE            0x101E8000
#define LED_REG_BASE              0x101E9000
#define CRC_REG_BASE              0x101EA000
#define IR_REG_BASE                 0x101EB000
#define AFE_REG_BASE               0x101F0000
#define SDRAMTST_REG_BASE     0x101F1000
#define STC_REG_BASE                0x101F2000

#define SFC_MEM_BASE                0x30000000
#define SDRAM_BASE                     0x70000000
#define CACHE_ALIGNED_SIZE        64
#endif

