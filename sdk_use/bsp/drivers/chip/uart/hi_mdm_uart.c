#include "uart.h"
#include "hi_mdm_uart.h"
#include "hi_mdm_types.h"
#include "uart_drv.h"
#define UART_FD_BASE 0
#define UART_NUMBER 3


typedef struct
{
    HI_U32 baudrate ;	/**< 波特率  */
    HI_U8 data_bits ;	/**< 数据位位数  */
    HI_U8 stop_bits;	/**< 停止位位数  */
    HI_U8 parity ;		/**< 奇偶校验标识  */
    HI_U8 reserved;
    
} HI_UART_ATTR_S;

extern uart_driver_data_t* udd_g[UART_NUM];

HI_U32 HI_MDM_UART_Getfd(HI_UART_PORT_E enPortId, HI_U32* fd)
{
    if(HI_NULL == fd || enPortId >= HI_UART_MAX)
    {
        return HI_ERR_FAILURE;
    }
    
    *fd = (HI_U32)(UART_FD_BASE + enPortId);
	return HI_ERR_SUCCESS;
}


HI_U32 HI_MDM_UART_Open(HI_UART_PORT_E enPortId, HI_UART_MODE_E enMode)
{
    uart_driver_data_t* udd;
    
    udd = uart_open(enPortId, (uart_mode_e)enMode);
    if(NULL == udd)
    {
        return HI_ERR_FAILURE;
    }

    return (HI_U32)(UART_FD_BASE + enPortId);

}
HI_U32 HI_MDM_UART_Read(HI_S32 fd, HI_U8* pData, HI_U32 ulDataSize)
{
    uart_driver_data_t* udd;

    if(fd - UART_FD_BASE > UART_NUMBER || fd - UART_FD_BASE < 0)
    {
        return HI_ERR_FAILURE;
    }    
    udd = udd_g[fd - UART_FD_BASE];

    return uart_read(udd, (HI_CHAR*)pData,ulDataSize);
}

HI_U32 HI_MDM_UART_Write(HI_S32 fd, HI_U8* pData, HI_U32 ulDataSize) 
{
    uart_driver_data_t* udd;

    if(fd - UART_FD_BASE > UART_NUMBER || fd - UART_FD_BASE < 0)
    {
        return HI_ERR_FAILURE;
    }    
    udd = udd_g[fd - UART_FD_BASE];

    return uart_write(udd, (HI_CHAR *)pData,ulDataSize);
}



HI_U32 HI_MDM_UART_Ioctl(HI_S32 fd, HI_U32 ulRequest, HI_U32 pAttr)
{
    uart_driver_data_t* udd;
	HI_U32 ret = HI_ERR_SUCCESS;
    if((fd - UART_FD_BASE > UART_NUMBER) ||((HI_PVOID)pAttr==HI_NULL))
    {
        return HI_ERR_FAILURE;
    }

    udd = udd_g[fd - UART_FD_BASE];

    if(UART_CFG_SET_ATTR==ulRequest)
    {	
        
        HI_UART_ATTR_S attr;
        HI_UART_INIT_TYPE_S* pstUartCfg = (HI_UART_INIT_TYPE_S*)pAttr;
    	attr.baudrate = pstUartCfg->baud_rate;
    	attr.data_bits = (HI_U8)pstUartCfg->data_bits;
        if(pstUartCfg->parity == 0)
        {
            attr.parity = HI_UART_PARITY_NONE;
        }
        else if(pstUartCfg->parity == 1)
        {
            attr.parity = HI_UART_PARITY_ODD;
        }
        else if(pstUartCfg->parity == 2)
        {
            attr.parity = HI_UART_PARITY_EVEN;
        }

        if(pstUartCfg->stop_bits== 1)
        {
            attr.stop_bits = HI_UART_STOP_BIT_1;
        }
        else if(pstUartCfg->stop_bits == 2)
        {
            attr.stop_bits = HI_UART_STOP_BIT_2;
        }
        return uart_ioctl(udd, (HI_S32)ulRequest, (unsigned long)&attr);    

    }
    else if(UART_CFG_GET_ATTR==ulRequest)
    {
        
        HI_UART_ATTR_S attr={0};
        HI_UART_INIT_TYPE_S* pstUartCfg = (HI_UART_INIT_TYPE_S*)pAttr;
        ret=uart_ioctl(udd, (HI_S32)ulRequest, (unsigned long)&attr);
        if(ret==HI_ERR_SUCCESS)
        {
            pstUartCfg->baud_rate=attr.baudrate;
            pstUartCfg->data_bits= (HI_U32)attr.data_bits ;
            if(attr.parity == HI_UART_PARITY_NONE)
            {
                pstUartCfg->parity = 0;
            }
            else if(attr.parity == HI_UART_PARITY_ODD)
            {
                pstUartCfg->parity = 1;
            }
            else if(attr.parity == HI_UART_PARITY_EVEN)
            {
                pstUartCfg->parity = 2;
            }
            
            if(attr.stop_bits == HI_UART_STOP_BIT_1)
            {
                pstUartCfg->stop_bits= 1;
            }
            else if(attr.stop_bits == HI_UART_STOP_BIT_2)
            {
                pstUartCfg->stop_bits = 2;
            }

        }
        return ret;     
            
    }
    else
    {
        return uart_ioctl(udd, (HI_S32)ulRequest, pAttr);    
    }
    
}

HI_U32 HI_MDM_UART_Close(HI_S32 fd)
{
    uart_driver_data_t* udd;

    if(fd - UART_FD_BASE > UART_NUMBER || fd - UART_FD_BASE < 0)
    {
        return HI_ERR_FAILURE;
    }
    
    udd = udd_g[fd - UART_FD_BASE];

    return uart_close(udd);
}

HI_U32 HI_MDM_UART_DfxGetStat(HI_UART_PORT_E enPortId,HI_UART_INIT_TYPE_S *pstCfg,HI_UART_DXF_STAT_S *pstStat)
{
    uart_driver_data_t* udd=HI_NULL;
    uart_attr_t stUartAttr={0};
    HI_U32 ret=HI_ERR_SUCCESS;

    if(enPortId >= HI_UART_MAX)
    {
        return HI_ERR_INVALID_PARAMETER;
    }
    udd = udd_g[enPortId];


    //读取配置&&将配置转换成外部格式  --->读取配置ioctrl实现需要修改,读取到的最好直接是HI_UART_INIT_TYPE_S
    if(pstCfg)
    {
        ret= uart_ioctl(udd, UART_CFG_GET_ATTR, (unsigned long)(&stUartAttr));
        if(ret)
        {
            return ret;
        }
        
        pstCfg->baud_rate=stUartAttr.baudrate;
        pstCfg->data_bits=stUartAttr.data_bits;

        if(stUartAttr.parity == HI_UART_PARITY_NONE)
        {
            pstCfg->parity = 0;
        }
        else if(stUartAttr.parity == HI_UART_PARITY_ODD)
        {
            pstCfg->parity = 1;
        }
        else if(stUartAttr.parity == HI_UART_PARITY_EVEN)
        {
            pstCfg->parity = 2;
        }

        if(stUartAttr.stop_bits== HI_UART_STOP_BIT_1)
        {
            pstCfg->stop_bits = 1;
        }
        else if(stUartAttr.stop_bits == HI_UART_STOP_BIT_2)
        {
            pstCfg->stop_bits = 2;
        }
    }


    if(pstStat)
    {
        uart_drv_stat_info_t *pst_stat_info = &(udd_g[enPortId]->uart_stat_info);
        pstStat->ulRcvIrqCnt = pst_stat_info->recv_irq_cnt;
        pstStat->ulRcvDataCnt = pst_stat_info->recv_irq_data_cnt;
        pstStat->ulSendIrqCnt = pst_stat_info->send_irq_cnt;
        pstStat->ulSendDataCnt = pst_stat_info->send_irq_data_cnt;

        pstStat->ulErrOverRun = pst_stat_info->recv_irq_err_overrun;
        pstStat->ulErrParity = pst_stat_info->recv_irq_err_parity;
        pstStat->ulErrFrame = pst_stat_info->recv_irq_err_frame;
        pstStat->ulErrBreak = pst_stat_info->recv_irq_err_break;
        pstStat->ulErrBusy = pst_stat_info->recv_irq_err_busy;

        (hi_void)memcpy_s(pstStat->ucLastRcvData, HI_UART_LAST_RECORD_BYTE_COUNT, pst_stat_info->recv_last_context.ucData, HI_UART_LAST_RECORD_BYTE_COUNT);
        pstStat->ulLastRcvSn = pst_stat_info->recv_last_context.ulNum;

        (hi_void)memcpy_s(pstStat->ucLastSendData, HI_UART_LAST_RECORD_BYTE_COUNT, pst_stat_info->send_last_context.ucData, HI_UART_LAST_RECORD_BYTE_COUNT);
        pstStat->ulLastSendSn = pst_stat_info->send_last_context.ulNum;
    }
    
    return HI_ERR_SUCCESS;//当pstCfg或者pstStat为NULL也会返回成功
}


