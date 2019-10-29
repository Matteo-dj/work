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

#include "los_event.h"
#include "hisoc/uart.h"
#include "los_hwi.h"
#include "string.h"


EVENT_CB_S gUartEvent;
#ifdef LOSCFG_PLATFORM_UART_WITHOUT_VFS
#ifdef LOSCFG_SHELL
#define UART_BUF    128
static int g_cnt_ii = 0;
int g_uwUart = 0;
static UINT8 g_uart_buf[UART_BUF];
#endif
#endif
UINT8 uart_getc(void)
{
    UINT8 ch = 0;
    UINT32 base = UART_REG_BASE;

    while (!(GET_UINT32(base + UART_USR) & 0x08)) /*lint !e40*/
    {
        LOS_Msleep(100);
    }
    /* receive one char */
    READ_UINT8(ch, base + UART_DR);
    return ch;
}

/* send */
char uart_putc (char c)
{
    UINT32 status = UART_REG_BASE;
    /* Wait until THRE is empyt */
    while (!(GET_UINT32(status + UART_USR) & 0x02)); /*lint !e40*/
    /* send one char */
    WRITE_UINT8(c, status + UART_DR);
    return c;
}

char uart_fputc(char c,void *f)
{
    if (c == '\n')
    {
        uart_putc('\r'); /*lint !e534*/
    }
    return (uart_putc(c));
}

#ifdef LOSCFG_PLATFORM_UART_WITHOUT_VFS
#ifdef LOSCFG_SHELL
static void uart_notice_adapt(void)
{
    LOS_EventWrite(&gUartEvent,0x112);
}

void uart_get_raw(void)
{
    UINT8 ch;
    if(g_cnt_ii == 0)
    {
        memset(g_uart_buf, 0, UART_BUF);
    }
    ch = uart_getc();
    g_uart_buf[g_cnt_ii]=ch;
    g_cnt_ii++;
    switch(g_cnt_ii)
    {
        case 1: //only one char
            if(ch != 27)  //   un special
            {
                uart_notice_adapt();
                g_cnt_ii =0;
            }
            break;
        case 2:
            if(ch != 91)
            {
                uart_notice_adapt();
                g_cnt_ii =0;
            }
            break;
        case 3:
            switch(ch)
            {
                default:
                case 'A':
                case 'B':
                case 'C':
                case 'D':
                    {
                        uart_notice_adapt();
                        g_cnt_ii =0;
                    }
                    break;
                case 51:
                case 49:
                case 52:
                    break;
            }
            break;
        case 4:
            {
                uart_notice_adapt();
                g_cnt_ii =0;
            }
            break;
        default:
            {
                uart_notice_adapt();
                g_cnt_ii =0;
            }
            break;
    }
}

static void uart_irqhandle(void)
{
    UINT8 ch;
    extern void shellCmdLineParse(CHAR c);
    shellCmdLineParse(ch);
}

int uart_hwiCreate(void)
{
    UINT32 uwRet = 0;
    if (uwRet != LOS_HwiCreate(NUM_HAL_INTERRUPT_UART, 0xa0, 0, (HWI_PROC_FUNC)uart_irqhandle, 0))
    {
        return uwRet;
    }
    uart_interrupt_unmask();
    return 0;
}

#endif
#endif
void uart_init()
{
    unsigned int temp;
    unsigned int divider;
    unsigned char dividerH,dividerL;
    UINT32 base = UART_REG_BASE;

    /* disable UART1 FIFO */
    WRITE_UINT32(0, base + UART_FCR); /*lint !e40*/

    /* reset and enable UART1 FIFO */
    WRITE_UINT32(0x7, base + UART_FCR); /*lint !e40*/

    /* disable UART1 interrupt */
    WRITE_UINT32(0, base+UART_IER); /*lint !e40*/

    // dw_wait_idle();

    /*enable DLL and DLH*/
    WRITE_UINT32(0x80, base+UART_LCR); /*lint !e40*/
    /*Caculate devide*/
    temp = 16 * CONFIG_UART_BAUDRATE;
    divider = CONFIG_UART_CLK_INPUT / temp;
    dividerH = ((divider) & 0xff00) >> 8;
    dividerL = ((divider) & 0x00ff);

    /*configure DLL and DLH*/
    WRITE_UINT32(dividerL, base + UART_DLL); /*lint !e40*/
    WRITE_UINT32(dividerH, base + UART_DLH); /*lint !e40*/

    // dw_wait_idle();

    /* disable DLL and DLH */
    WRITE_UINT32(0x0, base + UART_LCR); /*lint !e40*/

    //dw_wait_idle();

    /* 8bit data, 1bit stop,even parity */
    WRITE_UINT32(0x1b, base + UART_LCR); /*lint !e40*/

    /*enable UART1*/
    WRITE_UINT32(0x1, base+UART_IER); /*lint !e40*/

    (VOID)LOS_EventInit(&gUartEvent);
}

void uart_interrupt_unmask(void)
{
    hal_interrupt_unmask(NUM_HAL_INTERRUPT_UART);
}
