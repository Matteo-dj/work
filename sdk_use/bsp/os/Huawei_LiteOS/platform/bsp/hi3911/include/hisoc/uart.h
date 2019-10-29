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

#ifndef __HISOC_UART_H__
#define __HISOC_UART_H__

#include "asm/platform.h"
#include "los_typedef.h"
#include "los_base.h"

//extern UINT32 g_cfg_apb_clock;   
//extern UINT32 g_cfg_arm_clock;


//#define CONFIG_UART_CLK_INPUT         g_cfg_apb_clock
//#define CONFIG_BAUDRATE         115200

#define UART_DR                 0x0   //Êý¾Ý¼Ä´æÆ÷
#define UART_RSR                0x04   //Êý¾Ý¼Ä´æÆ÷
#define UART_FR                 0x18  //±êÖ¾¼Ä´æÆ÷
#define UART_CLR                0x44  //ÖÐ¶ÏÇå³ý¼Ä´æÆ÷
#define UART_CR                 0x30  //¿ØÖÆ¼Ä´æÆ÷
#define UART_IBRD               0x24  //ÕûÊý²¨ÌØÂÊ¼Ä´æÆ÷
#define UART_FBRD               0x28  //Ð¡Êý²¨ÌØÂÊ¼Ä´æÆ÷
#define UART_LCR_H              0x2C  //Ïß¿Ø¼Ä´æÆ÷
#define UART_IFLS               0x34  //ÖÐ¶ÏFIFOãÐÖµÑ¡Ôñ¼Ä´æÆ÷
#define UART_IMSC               0x38  //ÖÐ¶ÏÆÁ±Î¼Ä´æÆ÷
#define UART_RIS                0x3C  //Ô­Ê¼ÖÐ¶Ï×´Ì¬¼Ä´æÆ÷
#define UART_MIS                0x40  //ÆÁ±ÎºóÖÐ¶Ï×´Ì¬¼Ä´æÆ÷
#define UART_DMACR              0x48  //DMA¿ØÖÆ¼Ä´æÆ÷

#define UART_RBR          0x00   /*  Receive buff. */
#define UART_THR          0x00   /*  Transmit Holding  */
#define UART_DLL           0x00  /*  Divisor Latch Low */

#define UART_DLH          0x04  /*  Divisor Latch High */
#define UART_IER           0x04  /*  int enable*/

#define UART_IIR           0x08  /*  interrupt indentification REG*/
#define UART_FCR          0x08  /*  FIFO control*/

#define UART_LCR          0x0C  /*  Line control */

#define UART_LSR          0x14  /*  Line statue  */

#define UART_USR          0x7C  /*  Uart statues */

#define UART_HTX          0xA4  /*  Halt Tx*/

#define DE_FIFO_DEPTH        0x10   /*16bytes FIFO*/



//
#define UART0                  0
#define UART1                  1
#define UART2                  2
//#define UART3                  3
#define UART_NUM               3

#define UART0_DMA_RX_PERI       4
#define UART1_DMA_RX_PERI       6
#define UART2_DMA_RX_PERI       8
#define UART3_DMA_RX_PERI       10
extern unsigned int g_cfg_uart_clock;

#define CONFIG_UART_CLK_INPUT         g_cfg_uart_clock

#define hi_uart_mux_cfg(uart_num) ({\
         if(UART0 == uart_num){}\
         else if(UART1 == uart_num){\
            WRITE_UINT16(0x03, IO_MUX_REG_BASE + 0x0C0);\
            WRITE_UINT16(0x03, IO_MUX_REG_BASE + 0x0C8);\
         }\
         else if(UART2 == uart_num){\
            WRITE_UINT16(0x03, IO_MUX_REG_BASE + 0x0CC);\
            WRITE_UINT16(0x03, IO_MUX_REG_BASE + 0x0D0);\
         }\
        })
#define  hi_uart_clk_cfg(uart_num, flag)  ({\
        unsigned int tmp = 0;\
        tmp = GET_UINT32(CRG_REG_BASE + 0x0E4);\
        if(flag)\
            tmp |= (1<<(uart_num+15));\
        else\
            tmp &= ~(1<<(uart_num+15));\
        WRITE_UINT32(tmp, CRG_REG_BASE + 0x0E4);\
        })

#define  hi_uart_get_dma_peri(uart_num) ({\
        unsigned int peri_num;\
        if(UART0 == uart_num)\
        peri_num = UART0_DMA_RX_PERI;\
        else if(UART1 == uart_num)\
        peri_num = UART1_DMA_RX_PERI;\
        else if(UART2 == uart_num)\
        peri_num = UART2_DMA_RX_PERI;\
        peri_num;\
        })

#define STDIN                               0
#define STDOUT                              1
#define STDERR                              2
#define TTYS0                               "/dev/ttyS0"
#define CONSOLE                             "/dev/console"
#define SHELL                               "/dev/shell"
#define TELNET                              "/dev/telnet"


#define CONFIG_UART                        UART1
#define CONFIG_UART_BAUDRATE               115200

#if (CONFIG_UART == UART0)
    #define TTY_DEVICE                "/dev/uartdev-0"
    #define UART_REG_BASE             UART0_REG_BASE
    #define NUM_HAL_INTERRUPT_UART    NUM_HAL_INTERRUPT_UART0
#elif (CONFIG_UART == UART1)
    #define TTY_DEVICE                "/dev/uartdev-1"
    #define UART_REG_BASE             UART1_REG_BASE
    #define NUM_HAL_INTERRUPT_UART    NUM_HAL_INTERRUPT_UART1
#elif (CONFIG_UART == UART2)
    #define TTY_DEVICE                "/dev/uartdev-2"
    #define UART_REG_BASE             UART2_REG_BASE
    #define NUM_HAL_INTERRUPT_UART    NUM_HAL_INTERRUPT_UART2
#endif

typedef struct {
    UINT32 base;
    INT32 msec_timeout;
    int isr_vector;
} channel_data_t;

extern void uart_init(void);
extern void uart_interrupt_unmask(void);
extern int uart_hwiCreate(void);
extern UINT8 uart_getc(void);
extern char uart_putc(char c);
extern char uart_fputc(char c, void *f);

#endif
