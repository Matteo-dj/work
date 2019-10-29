/*----------------------------------------------------------------------------
 * Copyright (c) <2014-2015>, <Huawei Technologies Co., Ltd>
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

#ifndef __UART_H__
#define __UART_H__

#include "uart_drv.h"

#ifdef __cplusplus
extern "C" {
#endif

#define UART_NUM                4
#define UART0                   0
#define UART1                   1
#define UART2                   2
#define UART3                   3

#define UART0_BASE              0x101EC000
#define UART1_BASE              0x101ED000
#define UART2_BASE              0x101EE000
#define UART3_BASE              0x101EF000


#define CONFIG_RX_BUF_SIZE      0xC00  
#define CONFIG_TX_BUF_SIZE      0x800  


#define UART_CIRC_BUF_ENABLE
#define UART_ATTR_DEFAULT    {115200, 8, STOP_BIT_1, PARITY_NONE}
#define UART0_ATTR_DEFAULT   {115200,   8, STOP_BIT_1, PARITY_EVEN}
#define IR_ATTR_DEFAULT      {1200,   8, STOP_BIT_1, PARITY_EVEN}

/*485 gpio*/
#define GPIO_PHY_BASE      0x101E3000 
#define GPIO_SWPORT_DR     0x00

/*ir*/
#define GPIO_SEL_BASE      0x101E6000
//#define IR_REG_BASE        0x101E9000
#define IRCLK_POLAR_0   0x68 //uart3
#define IRCLK_POLAR_1   0x6c //uart2
#define IR_MODUAL_CTRL  0x70
#define IR_MODUAL_ENABLE (1)
#define IR_CLOCK_DIV    0x74





uart_driver_data_t* uart_open(int uart_index, uart_mode_e uart_mode);
unsigned int uart_read(uart_driver_data_t* udd, char* buf, size_t count);
unsigned int uart_write(uart_driver_data_t* udd, const char* buf, size_t count);
unsigned int uart_ioctl(uart_driver_data_t* udd, int cmd, unsigned long arg);
unsigned int uart_close(uart_driver_data_t* udd);


#ifdef __cplusplus
}
#endif
#endif
