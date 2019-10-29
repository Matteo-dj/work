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

#include "uart.h"
#include "los_event.h"
#include "string.h"
#include "hi_mdm_uart.h"
#include "los_hwi.h"

#define CONFIG_MAX_BAUDRATE    921600 /* max baud rate */
extern uart_ops_t uart_driver_uops;

static uart_driver_data_t uart_0 =
{
    .num = UART0,
    .phys_base = UART0_BASE,
    .irq_num = NUM_HAL_INTERRUPT_UART0,    
    .rx_transfer = HI_NULL,
    .tx_transfer = HI_NULL,
    .rx_recv = uart_write_circ_buf,
    .tx_send = uart_read_circ_buf,
    .flags = UART_FLG_RD_BLOCK | UART_FLG_WD_BLOCK,
    .count = 0,
    .state = UART_STATE_NOT_OPENED,
    .ops = &uart_driver_uops,
    .attr = UART0_ATTR_DEFAULT,
};

static uart_driver_data_t uart_1 =
{
    .num = UART1,
    .phys_base = UART1_BASE,
    .irq_num = NUM_HAL_INTERRUPT_UART1,
    .rx_transfer = HI_NULL,
    .tx_transfer = HI_NULL,
    .rx_recv = uart_write_circ_buf,
    .tx_send = uart_read_circ_buf,
    .flags = UART_FLG_RD_BLOCK | UART_FLG_WD_BLOCK,
    .count = 0,
    .state = UART_STATE_NOT_OPENED,
    .ops = &uart_driver_uops,
    .attr = UART_ATTR_DEFAULT,
};

static uart_driver_data_t uart_2 =
{
    .num = UART2,
    .phys_base = UART2_BASE,
    .irq_num = NUM_HAL_INTERRUPT_UART2,
    .rx_transfer = HI_NULL,
    .tx_transfer = HI_NULL,
    .rx_recv = uart_write_circ_buf,
    .tx_send = uart_read_circ_buf,
    .flags = UART_FLG_RD_BLOCK | UART_FLG_WD_BLOCK,
    .count = 0,
    .state = UART_STATE_NOT_OPENED,
    .ops = &uart_driver_uops,
    .attr = UART_ATTR_DEFAULT,
};

static uart_driver_data_t uart_3 =
{
    .num = UART3,
    .phys_base = UART3_BASE,
    .irq_num = NUM_HAL_INTERRUPT_UART3_IR,
    .rx_transfer = HI_NULL,
    .tx_transfer = HI_NULL,
    .rx_recv = uart_write_circ_buf,
    .tx_send = uart_read_circ_buf,
    .flags = UART_FLG_RD_BLOCK | UART_FLG_WD_BLOCK,
    .count = 0,
    .state = UART_STATE_NOT_OPENED,
    .ops = &uart_driver_uops,
    .attr = IR_ATTR_DEFAULT,
};

const uart_driver_data_t* udd_g[UART_NUM] = {&uart_0, &uart_1, &uart_2, &uart_3};

uart_driver_data_t* uart_open(int uart_index, uart_mode_e mode)
{
	unsigned int ret = HI_ERR_FAILURE;
	uart_driver_data_t* udd = HI_NULL;

	if (uart_index < 0 || uart_index >= UART_NUM || mode > UART_485)
	{
		return HI_NULL;
	}

	udd = (uart_driver_data_t*)udd_g[uart_index];
	if (mode == UART_IR)
	{
		if (uart_index != UART3)
		{
		   return HI_NULL;			 
		}
	}
	
	if (UART_STATE_NOT_OPENED == udd->state)
	{
		ret = uart_init_circ_buf(udd, CONFIG_RX_BUF_SIZE, CONFIG_TX_BUF_SIZE);
		if (ret != HI_ERR_SUCCESS)
		{
			UART_SET_ERRNO(UART_ERR_INIT_CIRC_FAILED);		  
			return HI_NULL;
		}
		ret = LOS_EventInit(&udd->uart_event);
		if (ret != HI_ERR_SUCCESS)
		{
			uart_deinit_circ_buf(udd);
			return HI_NULL;
		}
		udd->type = mode;
		
		if (!udd->ops->startup || udd->ops->startup(udd))
		{
			uart_deinit_circ_buf(udd);
			UART_SET_ERRNO(UART_ERR_START_FAILED);		 
			return HI_NULL;
		}
		udd->state = UART_STATE_USEABLE;
	}
	else
	{
		if(mode != udd->type)
		{
			if (HI_NULL != udd->ops->startup)
			{
				udd->type = mode; 
				udd->ops->startup(udd);
			}
		}
	}
	
	return udd;
}

unsigned int uart_read(uart_driver_data_t* udd, char* buf, size_t count)
{
	int len = 0;

	if (HI_NULL == udd)
	{
		return HI_ERR_FAILURE;
	}
	if ((HI_NULL == buf)||(0 == count))
	{
		UART_SET_ERRNO(UART_ERR_PARA_INVALID);							
		return HI_ERR_FAILURE;
	}
  
	if (udd->state != UART_STATE_USEABLE)
	{
		UART_SET_ERRNO(UART_ERR_NOT_OPENED);							
		return HI_ERR_FAILURE;
	}

	do
	{
		len = uart_read_circ_buf(udd->rx_transfer, buf, count);
#ifdef UART_DEBUG_INFO
		udd->uart_stat_info.read_circ_cnt += (unsigned int)len;
#endif
		if (len == 0)
		{
			if (udd->flags & UART_FLG_RD_BLOCK)
			{				 
				(void)LOS_EventRead(&udd->uart_event, UART_RD_EVENT,
									LOS_WAITMODE_OR | LOS_WAITMODE_CLR, LOS_WAIT_FOREVER);
			}
			else
			{
				break;
			}
		}
	}while (len == 0);
	

	return (HI_U32)len;
}

unsigned int uart_write(struct uart_driver_data* udd, const char* buf, size_t count)
{
	int len = 0;
	int left;
	/* if uart is not in useable, report error*/
	if (HI_NULL == udd)
	{
		return HI_ERR_FAILURE;
	}

	if ((HI_NULL == buf)||(0 == count))
	{
		UART_SET_ERRNO(UART_ERR_PARA_INVALID);							  
		return HI_ERR_FAILURE;
	} 
	left = (int)count;
	if (udd->state != UART_STATE_USEABLE)
	{
		UART_SET_ERRNO(UART_ERR_NOT_OPENED);							
		return HI_ERR_FAILURE;
	}
	if (udd->type == UART_IR || udd->type == UART_485)
	{
		uart_tf_interrupt_disable(udd);
		uart_set_tx_mode(udd);
	}
   
#ifdef UART_CIRC_BUF_ENABLE
	do
	{
		len += uart_write_circ_buf(udd->tx_transfer, (char*)&buf[len], (unsigned int)left);
		if (len < (int)count)
		{
			left = (int)count - len;
			
			if (udd->flags & UART_FLG_WD_BLOCK)
			{
				uart_tx_interrupt_enable(udd);
				(void)LOS_EventRead(&udd->uart_event, UART_WD_EVENT,
									LOS_WAITMODE_OR | LOS_WAITMODE_CLR, LOS_WAIT_FOREVER);
			}
			else
			{
				break;
			}
		}
		else
		{
			break;
		}
	}
	while (left > 0);
	uart_tx_interrupt_enable(udd);
#else
	if (udd->ops->start_tx)
	{
		len = udd->ops->start_tx(udd, buf, count);
	}
	if (udd->type == UART_IR || udd->type == UART_485)
	{
		uart_set_rx_mode(udd);
	}
	
#endif

#ifdef UART_DEBUG_INFO
	udd->uart_stat_info.write_circ_cnt += (unsigned int)len;
#endif

	return (HI_U32)len;
}

// TODO:读取配置ioctrl实现需要修改,读取到的最好直接是HI_UART_INIT_TYPE_S
unsigned int uart_ioctl(uart_driver_data_t* udd, int cmd, unsigned long arg)
{
    unsigned int ret = HI_ERR_SUCCESS;
    uart_attr_t* attr;

    if (HI_NULL == udd)
    {
        return HI_ERR_INVALID_PARAMETER;
    }
    
    if (udd->state != UART_STATE_USEABLE)
    {
        UART_SET_ERRNO(UART_ERR_NOT_OPENED);                            
        return HI_ERR_FAILURE;
    }

    switch (cmd)
    {
        case UART_CFG_SET_ATTR:
        {
            attr = (uart_attr_t*)arg;
            
            if ((attr->baudrate <= CONFIG_MAX_BAUDRATE) && (attr->baudrate  > 0)
                && attr->data_bits <= 8 && attr->data_bits >= 5
                &&(attr->parity == PARITY_NONE || attr->parity == PARITY_ODD || attr->parity == PARITY_EVEN)
                &&(attr->stop_bits== STOP_BIT_1 || attr->stop_bits == STOP_BIT_1P5 || attr->stop_bits == STOP_BIT_2))
            {
                (hi_void)memcpy_s(&udd->attr, sizeof(uart_attr_t), attr, sizeof(uart_attr_t));
                if (udd->ops->ioctl && udd->ops->ioctl(udd))
                {
                    UART_SET_ERRNO(UART_ERR_IOCTL_FAILED);                        
                    ret = HI_ERR_FAILURE;
                }
            }
            else
            {
                UART_SET_ERRNO(UART_ERR_PARA_INVALID);                    
                ret = HI_ERR_FAILURE;
            }
            break;
        }
        case UART_CFG_RD_BLOCK:
        {
            if (UART_RD_BLOCK == *(HI_U32*)(uintptr_t)arg)
            {
                udd->flags |= UART_FLG_RD_BLOCK;
            }
            else if (UART_RD_NONBLOCK == *(HI_U32*)(uintptr_t)arg)
            {
                udd->flags &= ~UART_FLG_RD_BLOCK;
                (void)LOS_EventWrite(&udd->uart_event, UART_RD_EVENT);
            }
            break;
        }
        case UART_CFG_WD_BLOCK:
        {
            if (UART_WD_BLOCK == *(HI_U32*)(uintptr_t)arg)
            {
                udd->flags |= UART_FLG_WD_BLOCK;
            }
            else if (UART_WD_NONBLOCK == *(HI_U32*)(uintptr_t)arg)
            {
                udd->flags &= ~UART_FLG_WD_BLOCK;

                (void)LOS_EventWrite(&udd->uart_event, UART_WD_EVENT);
            }
            break;
        }
        case UART_CFG_GET_ATTR:
        {

            attr = (uart_attr_t*)arg;
            (hi_void)memcpy_s(attr,  sizeof(uart_attr_t), &udd->attr, sizeof(uart_attr_t));
            break;
        }
        default:
        {    
            UART_SET_ERRNO(UART_ERR_PARA_INVALID);                            
            ret = HI_ERR_FAILURE;
            break;
        }
    }
    
    return ret;
}

unsigned int uart_close(uart_driver_data_t* udd)
{
    if (HI_NULL == udd)
    {
        return HI_ERR_INVALID_PARAMETER;
    }
    
    if (udd->state != UART_STATE_USEABLE)
    {
        UART_SET_ERRNO(UART_ERR_NOT_OPENED);                       
        return HI_ERR_FAILURE;
    }
    
    uart_tf_interrupt_disable(udd); 
    uart_rx_interrupt_disable(udd);
    uart_tx_interrupt_disable(udd);
    
    return HI_ERR_SUCCESS;
}
