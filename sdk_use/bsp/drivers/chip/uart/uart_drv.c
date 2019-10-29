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

#include "uart_drv.h"
#include "uart.h"
#include "asm/io.h"
#include "linux/interrupt.h"
#include "hi_driver.h"
#include "dfx_sys.h"
#include <hi_config.h>
#include <hi_platform.h>
#include <hi_low_power.h>

#define UART_RBR          0x00   /*  Receive buff. */
#define UART_THR          0x00   /*  Transmit Holding  */
#define UART_DLL          0x00  /*  Divisor Latch Low */
#define UART_DLH          0x04  /*  Divisor Latch High */
#define UART_IER          0x04  /*  int enable*/
#define UART_IIR          0x08  /*  interrupt indentification REG*/
#define UART_FCR          0x08  /*  FIFO control*/
#define UART_LCR          0x0C  /*  Line control */
#define UART_MCR          0x10  /*  Line control */
#define UART_LSR          0x14  /*  Line statue  */
#define UART_USR          0x7C  /*  Uart statues */
#define UART_TFL          0x80
#define UART_HTX          0xA4  /*  Halt Tx*/

#define UART_FCR_RXFIFO_ONEBYTE 0
#define UART_FCR_RXFIFO_QUARTER (1<<6)
#define UART_FCR_RXFIFO_HALF    (2<<6)
#define UART_FCR_RXFIFO_2TOFULL (3<<6)
#define UART_FCR_TXFIFO_EMPTY   0
#define UART_FCR_TXFIFO_2BYTE   (1<<4)
#define UART_FCR_TXFIFO_QUARTER (2<<4)
#define UART_FCR_TXFIFO_HALF    (3<<4)
#define UART_FCR_RESET_TXFIFO   (1<<2)
#define UART_FCR_RESET_RXFIFO   (1<<1)
#define UART_FCR_ENABLE_FIFO    (1<<0)

#define FCR_CONFIG_VAL  (UART_FCR_RXFIFO_QUARTER|UART_FCR_TXFIFO_2BYTE|UART_FCR_RESET_TXFIFO\
    |UART_FCR_RESET_RXFIFO|UART_FCR_ENABLE_FIFO)

#define UART_FIFO_TX_SIZE    16
#define UART_FIFO_TX_WL      2
#define UART_FIFO_RX_WL      4 

#define UART_IER_TX          (0x01 << 1)
#define UART_IER_RX          (0x01 << 0)
#define UART_IER_TF          (0x01 << 8) /*Tx finish*/

#define UART_IIR_RX_TIMEOUT           (0x03 << 2)
#define UART_IIR_RX                   (0x01 << 2)
#define UART_IIR_TX                   (0x01 << 1)
#define UART_IIR_TF                   0xE /*Tx finish*/

#define UART_USR_BUSY                 (0x01 << 0)
#define UART_USR_TXF_NF               (0x01 << 1)
#define UART_USR_TXF_E                (0x01 << 2)
#define UART_USR_RXF_NE               (0X01 << 3)

#define UART_LSR_OE 0x02    /* overrun error */
#define UART_LSR_PE 0x04    /* parity error */
#define UART_LSR_FE 0x08    /* framing error */
#define UART_LSR_BI 0x10    /* break interrupt */
#define UART_LSR_DR 0x01    /* 接收fifo中至少有一个数据 */


#define UART_IR_PERMIT_LOW_POWER_SLEEP_TICK (30*100)//红外有数据收发30秒之内不允许进入低功耗

#define CONFIG_UART_CLK_INPUT g_cfg_uart_clock
#define CONFIG_IR_CLK_INPUT   g_cfg_apb_clock

int irq_send_count = 0;


#define HI_UART_BUF_LOCK(x) HI_MDM_INT_Lock()
#define HI_UART_BUF_UNLOCK(x,val) HI_MDM_INT_Restore(val)

int uart_circ_buf_empty(uart_circ_buf_t* transfer)
{
    if (transfer->flags & BUF_CIRCLED)
    {
        return 0;
    }

    if (transfer->wp == transfer->rp)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

unsigned int uart_init_circ_buf(uart_driver_data_t* udd, unsigned int rx_buf_size, unsigned int tx_buf_size)
{
    if ((!rx_buf_size) || (!tx_buf_size))
    {
        return HI_ERR_INVALID_PARAMETER;
    }
    
    udd->rx_transfer =  (uart_circ_buf_t*)malloc(sizeof(uart_circ_buf_t));
    if (NULL == udd->rx_transfer)
    {
        goto end;
    }

    (hi_void)memset_s((HI_VOID *)udd->rx_transfer, sizeof(uart_circ_buf_t), 0, sizeof(uart_circ_buf_t));
    udd->rx_transfer->data = (char*)malloc(rx_buf_size);
    if (NULL == udd->rx_transfer->data)
    {
        goto free_rx_transfer;
    }
    
    udd->rx_transfer->size = rx_buf_size;
    udd->tx_transfer = (uart_circ_buf_t*)malloc(sizeof(uart_circ_buf_t));
    if (NULL == udd->tx_transfer)
    {
        goto free_rx_circ_buf;
    }

    (hi_void)memset_s((HI_VOID *)udd->tx_transfer, sizeof(uart_circ_buf_t), 0, sizeof(uart_circ_buf_t));
    udd->tx_transfer->data = (char*)malloc(tx_buf_size);
    if (NULL == udd->tx_transfer->data)
    {
        goto free_tx_circ_buf;
    }
    
    udd->tx_transfer->size = tx_buf_size;
    return HI_ERR_SUCCESS;

free_tx_circ_buf:
    free((VOID*)udd->tx_transfer);
    udd->tx_transfer = HI_NULL;

free_rx_circ_buf:
    free((VOID*)udd->rx_transfer->data);
    udd->rx_transfer->data = HI_NULL;
free_rx_transfer:    
    free((VOID*)udd->rx_transfer);
    udd->rx_transfer = HI_NULL;
end:
    return HI_ERR_FAILURE;
}

void uart_reset_circ_buf(uart_driver_data_t* udd)
{
    if (NULL != udd && NULL != udd->rx_transfer)
    {
        udd->rx_transfer->flags = 0;
        udd->rx_transfer->wp= 0;
        udd->rx_transfer->rp= 0;
        (hi_void)memset_s((HI_VOID *)udd->rx_transfer->data, udd->rx_transfer->size, 0, udd->rx_transfer->size);
    }

    if (NULL != udd && NULL != udd->tx_transfer)
    {
        udd->tx_transfer->flags = 0;
        udd->tx_transfer->wp= 0;
        udd->tx_transfer->rp= 0;
        (hi_void)memset_s((HI_VOID *)udd->tx_transfer->data, udd->tx_transfer->size, 0, udd->tx_transfer->size);
    }  
}

void uart_deinit_circ_buf(uart_driver_data_t* udd)
{
    if (HI_NULL != udd->rx_transfer)
    {
        if (HI_NULL != udd->rx_transfer->data)
        {
            free(udd->rx_transfer->data);
            udd->rx_transfer->data = HI_NULL;
        }
        
        free(udd->rx_transfer);
        udd->rx_transfer = HI_NULL;
    }

    if (HI_NULL != udd->tx_transfer)
    {
        if (HI_NULL != udd->tx_transfer->data)
        {
            free(udd->tx_transfer->data);
            udd->tx_transfer->data = HI_NULL;
        }
        
        free(udd->tx_transfer);
        udd->tx_transfer = HI_NULL;
    }
    
}

int uart_read_circ_buf(uart_circ_buf_t* transfer, char* buf, size_t count)
{
    unsigned int wp, rp;
    unsigned long data;
    unsigned int buf_size;
    unsigned int flags;
    HI_U32 ulLastIntVal;

    if (NULL == transfer)
    {
        return -EFAULT;
    }

    ulLastIntVal = HI_UART_BUF_LOCK(transfer);
    wp = transfer->wp;
    rp = transfer->rp;
    flags = transfer->flags;
    HI_UART_BUF_UNLOCK(transfer,ulLastIntVal);
    data = (unsigned long)transfer->data;
    buf_size = transfer->size;
    if (!(flags & BUF_CIRCLED))
    {
        if (count >= (wp - rp))
        {
            count = wp - rp;
        }

        (hi_void)memcpy_s(buf, count, (void*)(data + rp), count);
        transfer->rp += count;
        return (int)count;
    }
    else
    {
        if (count < (buf_size - rp))
        {
            (hi_void)memcpy_s(buf, count, (void*)(data + rp), count);
            transfer->rp += count;
            return (int)count;
        }
        else
        {
            unsigned int copy_size = buf_size - rp;
            unsigned int left_size = count - copy_size;
            (hi_void)memcpy_s(buf, copy_size, (void*)(data + rp), copy_size);
            rp = 0;
            if (left_size > wp)
            {
                left_size = wp;
            }

            (hi_void)memcpy_s((void*)(buf + copy_size), left_size, (void*)(data + rp), left_size);
            ulLastIntVal = HI_UART_BUF_LOCK(transfer);
            transfer->rp = left_size;
            transfer->flags &= ~BUF_CIRCLED;
            HI_UART_BUF_UNLOCK(transfer,ulLastIntVal);
            return (int)(copy_size + left_size);
        }
    }
}

int uart_write_circ_buf(uart_circ_buf_t* transfer, const char* buf, size_t count)
{
    unsigned int wp, rp;
    unsigned long data;
    unsigned int buf_size;
    unsigned int flags;
    HI_U32 ulLastIntVal;

    if (NULL == transfer)
    {
        return -EFAULT;
    }
    
    ulLastIntVal = HI_UART_BUF_LOCK(transfer);
    wp = transfer->wp;
    rp = transfer->rp;
    flags=transfer->flags;
    HI_UART_BUF_UNLOCK(transfer,ulLastIntVal);
    
    data = (unsigned long)transfer->data;
    buf_size = transfer->size;

    if (!(flags & BUF_CIRCLED))
    {
        if (count < (buf_size - wp))
        {
            (hi_void)memcpy_s((void*)(data + wp), count, buf, count);
            transfer->wp += count;
            return (int)count;
        }
        else
        {
            unsigned int copy_size = buf_size - wp;
            unsigned int left_size = count - copy_size;
            (hi_void)memcpy_s((void*)(data + wp), copy_size, buf, copy_size);
            wp = 0;
            if (left_size > rp)
            {
                /* overflowed. some new data will be discarded */
                left_size = rp;
            }

            (hi_void)memcpy_s((void*)(data + wp), left_size, (void*)(buf + copy_size), left_size);

            ulLastIntVal = HI_UART_BUF_LOCK(transfer);
            transfer->wp = left_size;
            transfer->flags |= BUF_CIRCLED;
            HI_UART_BUF_UNLOCK(transfer,ulLastIntVal);
            return (int)(copy_size + left_size);
        }
    }
    else
    {
        if (count > (rp - wp))
        {
            /* overflowed. some new data will be discarded */
            count = rp - wp;
        }

        (hi_void)memcpy_s((void*)(data + wp), count, buf, count);
        transfer->wp += count;
        return (int)count;
    }
}

void uart_tx_interrupt_disable(uart_driver_data_t* udd)
{
    unsigned int tx_status = readl(udd->phys_base + UART_IER);
    writel((~UART_IER_TX)&tx_status, udd->phys_base + UART_IER);
}

void uart_tx_interrupt_enable(uart_driver_data_t* udd)
{
    unsigned int tx_status = readl(udd->phys_base + UART_IER);
    writew((unsigned short)(UART_IER_TX | tx_status), udd->phys_base + UART_IER);
}

void uart_rx_interrupt_disable(uart_driver_data_t* udd)
{
    unsigned int tx_status = readl(udd->phys_base + UART_IER);
    writel(~(UART_IER_RX) & tx_status, udd->phys_base + UART_IER);
}

void uart_rx_interrupt_enable(uart_driver_data_t* udd)
{
    unsigned int tx_status = readl(udd->phys_base + UART_IER);
    writel(UART_IER_RX | tx_status, udd->phys_base + UART_IER);   
}

void uart_tf_interrupt_disable(uart_driver_data_t* udd)
{
    unsigned int tx_status = readl(udd->phys_base + UART_IER);
    writel(~(UART_IER_TF) & tx_status, udd->phys_base + UART_IER);

}
void uart_tf_interrupt_enable(uart_driver_data_t* udd)
{
    unsigned int tx_status = readl(udd->phys_base + UART_IER);
    writel(UART_IER_TF | tx_status, udd->phys_base + UART_IER);
}
void uart_set_tx_mode(uart_driver_data_t* udd)
{
	unsigned int uvIntSave;
	
	uvIntSave = HI_MDM_INT_Lock();
	
	if (udd->type == UART_485)
	{
	    HI_MDM_IO_SetOutputVal(HI_GPIO_IDX_7,HI_GPIO_VALUE1);
	}
	else if (udd->type == UART_IR && udd->num == UART3)
	{
		uart_rx_interrupt_disable(udd);
		HI_IO_SetMux(HI_HW_UART3_RXD_IO15, HI_GPIO15_SEL_GPIO15);
	}
	
	HI_MDM_INT_Restore(uvIntSave);	  
}

void uart_set_rx_mode(uart_driver_data_t* udd)
{
	unsigned int uvIntSave;
	
	uvIntSave = HI_MDM_INT_Lock();
	
	if (udd->type == UART_485)
	{
        HI_MDM_IO_SetOutputVal(HI_GPIO_IDX_7,HI_GPIO_VALUE0);

	}

	if (udd->type == UART_IR && udd->num == UART3)
	{
		
		HI_IO_SetMux(HI_HW_UART3_RXD_IO15, HI_GPIO15_SEL_UART3_RXD);
		uart_rx_interrupt_enable(udd);
	}
	
	HI_MDM_INT_Restore(uvIntSave);	   
}

unsigned int status_lsr = 0;
unsigned int status_IIR_in = 0;

void get_recv_irq_err(unsigned int int_status, uart_drv_stat_info_t *pst_uart_stat_info)
{


    status_lsr = int_status;

    
    if (int_status & UART_LSR_OE)
    {
        (pst_uart_stat_info->recv_irq_err_overrun)++;
    }

    if (int_status & UART_LSR_PE)
    {
        (pst_uart_stat_info->recv_irq_err_parity)++;
    }

    if (int_status & UART_LSR_FE)
    {
        (pst_uart_stat_info->recv_irq_err_frame)++;
    }

    if (int_status & UART_LSR_BI)
    {
        (pst_uart_stat_info->recv_irq_err_break)++;
    }
}

static void uart_drv_irq(void* data)
{
    unsigned int status = 0;
    unsigned int usr = 0;
    unsigned int lsr = 0;
    char buf[64] = {0};
    unsigned int count = 0;
    unsigned int tx_fifo_cnt;
    uart_driver_data_t* udd = (uart_driver_data_t*)data;
    uart_drv_stat_info_t * pst_uart_stat_info = HI_NULL;

    if (NULL == udd)
    {
        return;
    }

    if(udd->num == UART3)
    {
        HI_LOW_POWER_UnForbid(HI_LOW_POWER_FORBID_ID_IR,UART_IR_PERMIT_LOW_POWER_SLEEP_TICK);
    }

    pst_uart_stat_info = &(udd->uart_stat_info);
    status = readl(udd->phys_base + UART_IIR);
    status &= 0xf;
    if ((status == UART_IIR_RX_TIMEOUT) ||	(status == UART_IIR_RX))
    {  
        get_recv_irq_err(readl(udd->phys_base + UART_LSR), pst_uart_stat_info);  
        if (udd->num == UART3)
        {

          do
            {
                lsr = readl(udd->phys_base + UART_LSR);
                if (!(lsr & UART_LSR_DR)) //空才执行
                {
                    if (!count)
                    {
                        readl((HI_U32)(udd->phys_base + (HI_U32)UART_RBR));/*lint !e522*/
                        (udd->uart_stat_info.recv_irq_err_emptyfifo_cnt)++;
                    }
                    break;
                } 

                buf[count] = (char)readl(udd->phys_base + UART_RBR);
                udd->uart_stat_info.recv_last_context.ucData[udd->uart_stat_info.recv_last_context.ulNum] = (unsigned char)buf[count];
                udd->uart_stat_info.recv_last_context.ulNum++;
                udd->uart_stat_info.recv_last_context.ulNum &= (HI_UART_LAST_RECORD_BYTE_COUNT-1);
                count++;
            }while (count < 64);
        }
        else
        {
            do
            {
                usr = readl(udd->phys_base + UART_USR);
                /* if uart hardware rx fifo is empty, go out of circle */
                if (!(usr & UART_USR_RXF_NE)) //空才执行
                {
                    if (!count)
                    {
                        readl((HI_U32)(udd->phys_base + (HI_U32)UART_RBR));/*lint !e522*/
                        (udd->uart_stat_info.recv_irq_err_emptyfifo_cnt)++;
                    }
                    break;
                }
                buf[count] = (char)readl(udd->phys_base + UART_RBR);               
                udd->uart_stat_info.recv_last_context.ucData[udd->uart_stat_info.recv_last_context.ulNum] = (unsigned char)buf[count];
                udd->uart_stat_info.recv_last_context.ulNum++;
                udd->uart_stat_info.recv_last_context.ulNum &= (HI_UART_LAST_RECORD_BYTE_COUNT-1);
                count++;
            }while (count < 64);
        }

        if (count >= 1)
        {
            udd->rx_recv(udd->rx_transfer, buf, count);
            if (udd->flags & UART_FLG_RD_BLOCK)
            {
                LOS_EventWrite(&udd->uart_event, UART_RD_EVENT);
            }
        }
              
#ifdef UART_DEBUG_INFO
        udd->uart_stat_info.recv_irq_cnt++;
        udd->uart_stat_info.recv_irq_data_cnt += count;
#endif
    }

    /* FIXME: send irq */
    if (status == UART_IIR_TX)
    {
        /* disable uart tx fifo interrupt */

        if(udd->num == UART3)
        {
            tx_fifo_cnt = UART_FIFO_TX_SIZE - UART_FIFO_TX_WL;
        }
        else 
        {
            unsigned int tfl = readl(udd->phys_base + UART_TFL);
            tx_fifo_cnt = UART_FIFO_TX_SIZE - tfl;
        }

    
        count = (unsigned int)udd->tx_send(udd->tx_transfer, buf, tx_fifo_cnt);//uart_read_circ_buf
        if (udd->ops->start_tx)
        {
            (void)udd->ops->start_tx(udd, buf, count);//uart_drv_start_tx
        }
        
        if (uart_circ_buf_empty(udd->tx_transfer))
        {
            uart_tx_interrupt_disable(udd);
            uart_tf_interrupt_enable(udd);   
        }

        if (udd->flags & UART_FLG_WD_BLOCK)
        {
            (void)LOS_EventWrite(&udd->uart_event, UART_WD_EVENT);
        }
#ifdef UART_DEBUG_INFO
        udd->uart_stat_info.send_irq_cnt ++;
#endif
    }

    if (status == UART_IIR_TF)
    {
        uart_tf_interrupt_disable(udd);
        uart_set_rx_mode(udd);
    }

}

#define WRITE_LCR_TIMEOUT (1000000)
#define LCR_DLAB 0x80
static void writelcr(uart_driver_data_t* udd, unsigned int val)
{
    unsigned int usr = 0;
    unsigned int lcr = 0;
    unsigned int timeout_cnt = 0;
    unsigned int i = 0;

    if (udd->num != UART3)
    {           
        lcr = readl(udd->phys_base + UART_LCR); 
        while (lcr != val)
        {
            if (timeout_cnt ++ >= WRITE_LCR_TIMEOUT)
            {
                LOS_Panic("!write lcr: %u",110);
    			return;
            }
            usr = readl(udd->phys_base + UART_USR);
            while (usr & UART_USR_BUSY)
            {
                HI_MDM_Udelay(1);
                if (timeout_cnt ++ >= WRITE_LCR_TIMEOUT)
                {
                    LOS_Panic("!write lcr: %u",111);
					return;
                }
                lcr = readl(udd->phys_base + UART_LCR);
                if (!(lcr & LCR_DLAB))//没有使能DHL,DLL
                {
                    for (i = 0; i < UART_FIFO_TX_SIZE + 1; i++)
                    {
                        (void)readl(udd->phys_base + UART_RBR);
                    }
                }
                else
                {
                    /*reset fifo*/
                    writel(FCR_CONFIG_VAL, udd->phys_base + UART_FCR); /*lint !e40*/                 
                }

                usr = readl(udd->phys_base + UART_USR);
            }

            writel(val, udd->phys_base + UART_LCR); /*lint !e40*/	
            lcr = readl(udd->phys_base + UART_LCR);
        }
    }
    else
    {
        lcr = readl(udd->phys_base + UART_LCR); 
        while (lcr != val)
        {
            HI_MDM_Udelay(1);
            if (timeout_cnt ++ >= WRITE_LCR_TIMEOUT)
            {
                LOS_Panic("!write lcr: %u",112);
				return;
            }
            if (!(lcr & LCR_DLAB))
            {
                (void)readl(udd->phys_base + UART_RBR);
            }
            else
            {
                /*reset fifo*/
               writel(FCR_CONFIG_VAL, udd->phys_base + UART_FCR); /*lint !e40*/   
            }
            writel(val, udd->phys_base + UART_LCR); /*lint !e40*/	
            lcr = readl(udd->phys_base + UART_LCR);            
        }
    }
    
}

typedef struct
{
    HI_U32 ulBaudRate;//波特率
    HI_U32 ulclkRate;//时钟频率
    HI_U16 usDivVal;//分频寄存器值
    HI_U8  ucSupportUart;//该配置使用哪几个串口,每bit表示一个串口
    HI_U8  pad;    
}HI_UART_BAUDRATE_MATCH_ITEM_S;

HI_UART_BAUDRATE_MATCH_ITEM_S g_stUartBaudRateMatchTbl[]=
{
    {115200,HI_XTAL_CLOCK_REAL,13,(1<<HI_UART_0),0},
};

/*
功能：根据波特率时钟计算出合适的分频值
*/
HI_U16 uart_baudrate_2_divval(HI_U32 ulUartId,HI_U32 ulBaudRate,HI_U32 ulClkRate)
{
    HI_U32 i;
    
	//1.优先查表:由于硬件偏差一般算法并不能保证适合所有硬件设计,因此增加了查表步骤。
    HI_UART_BAUDRATE_MATCH_ITEM_S *pItem;
    for(i=0;i<sizeof(g_stUartBaudRateMatchTbl)/sizeof(g_stUartBaudRateMatchTbl[0]);i++)
    {
        pItem=&g_stUartBaudRateMatchTbl[i];
        
        if( (ulClkRate==pItem->ulclkRate) && (ulBaudRate==pItem->ulBaudRate) && ((1<<ulUartId) & pItem->ucSupportUart) )
        {
            return pItem->usDivVal;
        }
    }

	//2.一般算法
    return (HI_U16)(((ulClkRate * 10) / (ulBaudRate*16) + 5) / 10);
}


static void uart_drv_set_attr(uart_driver_data_t* udd)
{
    unsigned int lcr = 0;
    unsigned int mcr = 0;
	
    unsigned short divider = 0;
    unsigned char dividerH = 0;
    unsigned char dividerL = 0;
	
    unsigned int uvIntSave;

    /*Caculate devide*/
    if (udd->num != UART3)
    {
        divider=uart_baudrate_2_divval(udd->num,udd->attr.baudrate,CONFIG_UART_CLK_INPUT);
    }
    else
    {
        divider=uart_baudrate_2_divval(udd->num,udd->attr.baudrate,CONFIG_IR_CLK_INPUT);
    }
    dividerH = ((divider) & 0xff00) >> 8;
    dividerL = ((divider) & 0x00ff);
    

    if (udd->attr.data_bits == 5)
    {
        lcr |= (DATA_BIT_5);
    }
    else if (udd->attr.data_bits == 6)
    {
        lcr |= (DATA_BIT_6);
    }
    else if (udd->attr.data_bits == 7)
    {
        lcr |= (DATA_BIT_7);
    }
    else
    {
        lcr |= (DATA_BIT_8);
    }

    if (udd->attr.stop_bits == STOP_BIT_1)
    {
        lcr |= (STOP_BIT_1);
    }
    else if (udd->attr.stop_bits == STOP_BIT_1P5 || udd->attr.stop_bits == STOP_BIT_2)
    {
        lcr |= (STOP_BIT_2);
    }

    if (udd->attr.parity == PARITY_NONE )
    {
        lcr |= (PARITY_NONE);
    }
    else if (udd->attr.parity == PARITY_ODD)
    {
        lcr |= (PARITY_ODD);
    }
    else if (udd->attr.parity == PARITY_EVEN)
    {
        lcr |= (PARITY_EVEN);
    }   

    uvIntSave = HI_MDM_INT_Lock();

    /* loopback mode */
    mcr = readl(udd->phys_base + UART_MCR);
    writel(mcr|(1<<4), udd->phys_base + UART_MCR);
    
    writelcr(udd, 0x80);

    /*configure DLL and DLH*/
    writel(dividerL, udd->phys_base + UART_DLL); /*lint !e40*/
    writel(dividerH, udd->phys_base + UART_DLH); /*lint !e40*/

    writelcr(udd, lcr);
    
    mcr = readl(udd->phys_base + UART_MCR);
    writel(mcr&(~(1<<4)), udd->phys_base + UART_MCR);
 
    (void)HI_MDM_INT_Restore(uvIntSave);
    
}

static void uart2ir(uart_ir_pola_type_e ir_pola_type)
{
    unsigned int bit = 0;
    unsigned int ctrl = 0;
    
    writel(ir_pola_type, LED_REG_BASE + IRCLK_POLAR_0);
    HI_IO_SetMux(HI_HW_UART3_TXD_IO16, HI_GPIO16_SEL_UART3_TXD);
    ctrl = readl(LED_REG_BASE + IR_MODUAL_CTRL);
    ctrl |= IR_MODUAL_ENABLE << bit;
    writel(ctrl, LED_REG_BASE + IR_MODUAL_CTRL);
    
    /* set clock to 38k */
    writel(((CONFIG_IR_CLK_INPUT/38000)/2-1), LED_REG_BASE + IR_CLOCK_DIV);
}

/* uops */
static int uart_drv_startup(uart_driver_data_t* udd)
{
    unsigned int ret = 0;
    unsigned int fcr = 0;
    
    /* disable interrupt */
    writel(0, udd->phys_base + UART_IER); /*lint !e40*/

    /*set baudrate,data_bit,stop_bit,parity*/
    uart_drv_set_attr(udd);
	

    /* reset and enable FIFO */
    fcr = FCR_CONFIG_VAL;
    writel(fcr, udd->phys_base + UART_FCR); /*lint !e40*/

    
    if(udd->type == UART_IR)
    {
        uart2ir(IR_NEGATIVE_POLA);
    }
        
    /* creat interrupt function for uart */
    ret = LOS_HwiCreate(udd->irq_num, 0, 0, (HWI_PROC_FUNC)uart_drv_irq, (HWI_ARG_T)udd);
    if (ret == 0 || ret == OS_ERRNO_HWI_ALREADY_CREATED)
    {
        uart_set_rx_mode(udd);
        uart_rx_interrupt_enable(udd);
        uart_tf_interrupt_disable(udd);
        hal_interrupt_unmask(udd->irq_num);
    }

    return 0;
}

static void uart_drv_shutdown(struct uart_driver_data* udd)
{
    unsigned int ctrl = 0;
    
    uart_tx_interrupt_disable(udd);
    uart_rx_interrupt_disable(udd);
    hal_interrupt_mask(udd->irq_num);
    
    /*disable ir*/
    if(udd->type == UART_IR)
    {
        ctrl = readl(LED_REG_BASE +IR_MODUAL_CTRL);
        ctrl &= ~((HI_U32)(IR_MODUAL_ENABLE << ((udd->num == UART3)? 0:1)));
        writel(ctrl, (HI_U32)(LED_REG_BASE +IR_MODUAL_CTRL));

    }
    
    LOS_HwiDelete(udd->irq_num, (HWI_ARG_T)udd);
}

static int uart_drv_start_tx(struct uart_driver_data* udd, const char* buf, size_t count)
{
    unsigned int idx = 0;

    if (count > UART_FIFO_TX_SIZE)
    {
        return (int)HI_ERR_FAILURE;
    }
     
    /* if tx fifo is empty, write data to fifo */
    while (idx < count)
    {
        writel((unsigned)(HI_U8)buf[idx], udd->phys_base + UART_THR);
        
        udd->uart_stat_info.send_last_context.ucData[udd->uart_stat_info.send_last_context.ulNum] =  (unsigned char)buf[idx];
        udd->uart_stat_info.send_last_context.ulNum++;
        udd->uart_stat_info.send_last_context.ulNum &= (HI_UART_LAST_RECORD_BYTE_COUNT-1);
        
        idx++;
    }
#ifdef UART_DEBUG_INFO        
    udd->uart_stat_info.send_irq_data_cnt += count;
#endif    
    return (int)count;
}

static int uart_drv_ioctl(uart_driver_data_t* udd)
{
    unsigned int fcr = 0;
    /* wait for send finish */
    uart_drv_set_attr(udd);

    /* reset and enable FIFO */
    fcr = FCR_CONFIG_VAL;
    writel(fcr, udd->phys_base + UART_FCR); /*lint !e40*/
    
    uart_rx_interrupt_enable(udd);    
    return 0;
}

uart_ops_t uart_driver_uops =
{
    .startup        = uart_drv_startup,
    .shutdown       = uart_drv_shutdown,
    .start_tx       = uart_drv_start_tx,
    .ioctl          = uart_drv_ioctl,
};

