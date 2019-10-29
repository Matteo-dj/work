
#include <hi_types.h>
#include <hi_mdm_types.h>
#include <hi_mdm.h>
#include <hi_driver_internal.h>
#include "hi_mdm_uart.h"
#include "los_printf.h"

HI_PRVL HI_U32 PortChange(HI_U32 ulChnId,HI_UART_PORT_E *penUartPort)
{

#if (defined(PRODUCT_CFG_PRODUCT_TYPE_NDM) || defined(PRODUCT_CFG_FEATURE_STA_CONCENTRATOR_CHECK))
    if(ulChnId == HI_DMS_CHL_UART_PORT_APP)
    {
        *penUartPort = (HI_UART_PORT_E)HI_DMS_CHL_UART_PORT_DIAG;
    }
    else if(ulChnId == HI_DMS_CHL_UART_PORT_DIAG)
    {
        return HI_ERR_NOT_SUPPORT;
    }
    else
    {
        *penUartPort = (HI_UART_PORT_E)ulChnId;
    }
#else
    *penUartPort = (HI_UART_PORT_E)ulChnId;
#endif
    return HI_ERR_SUCCESS;
}

HI_U32 sal_UartOpen(HI_SYS_UART_E enUartPort, HI_UART_MODE_TYPE_E enMode, HI_IN HI_UART_INIT_TYPE_S* pstUartCfg)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_UART_PORT_E enUartIdx = HI_UART_0;

    ret = PortChange(enUartPort,&enUartIdx);
    if (ret != HI_ERR_SUCCESS)
    {
        return ret;
    }

    ret = HI_MDM_UART_Open(enUartIdx, (HI_UART_MODE_E)enMode);
    if(ret == HI_ERR_FAILURE)
    {
        return HI_ERR_SYSTEM_CALL_ERROR;
    }
    //printf("enUartPort=%d,enMode=%d,baud_rate=%d,data_bits=%d,stop_bits=%d,parity=%d\n",
    //        enUartPort,enMode,pstUartCfg->baud_rate,pstUartCfg->data_bits,pstUartCfg->stop_bits,pstUartCfg->parity);
    ret = HI_MDM_UART_Ioctl((HI_S32)enUartIdx, UART_CFG_SET_ATTR, (HI_U32)pstUartCfg);
    if(ret == HI_ERR_FAILURE)
    {
        return HI_ERR_SYSTEM_CALL_ERROR;
    }

    return HI_ERR_SUCCESS;
}

HI_U32 sal_UartClose(HI_SYS_UART_E enUartPort ,HI_UART_MODE_TYPE_E enMode)
{
    HI_U32 ret = HI_ERR_SUCCESS;

    HI_UART_PORT_E enUartIdx = HI_UART_0;

    ret = PortChange(enUartPort,&enUartIdx);
    if (ret != HI_ERR_SUCCESS)
    {
        return ret;
    }



    ret = HI_MDM_UART_Close(enUartIdx);
    if(ret != HI_ERR_SUCCESS)
    {
        return HI_ERR_SYSTEM_CALL_ERROR;
    }

    return ret;
}

HI_U32 sal_UartRead(HI_SYS_UART_E enUartPort, HI_UART_MODE_TYPE_E enMode,HI_IN HI_U8* pucBuffer, HI_U16 usNumberOfBytesToRead, HI_OUT HI_U16* pusNumberOfBytesRead)
{
    HI_U32 len = 0;
    HI_CHAR c = 0;
    HI_U32 ret =HI_ERR_SUCCESS;
    HI_UART_PORT_E enUartIdx = HI_UART_0;

    ret = PortChange(enUartPort,&enUartIdx);
    if (ret != HI_ERR_SUCCESS)
    {
        return ret;
    }


    len = HI_MDM_UART_Read(enUartIdx, (HI_U8 *)&c, 1);

    if (len == HI_ERR_FAILURE)
    {
        *pusNumberOfBytesRead = 0;
        return HI_ERR_SYSTEM_CALL_ERROR;
    }
    else if (len == 0)
    {
        *pusNumberOfBytesRead = 0;
        return HI_ERR_NO_MORE_DATA;
    }
    else
    {
        pucBuffer[0] = (HI_UCHAR)c;
        *pusNumberOfBytesRead = 1;
        return HI_ERR_SUCCESS;
    }
}

HI_U32 sal_UartWrite(HI_SYS_UART_E enUartPort, HI_UART_MODE_TYPE_E enMode, HI_IN HI_U8* pucBuffer, HI_U16 usNumberOfBytesToWrite, HI_OUT HI_U16* pusNumberOfBytesWritten)
{
    HI_S32 len;
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_UART_PORT_E enUartIdx = HI_UART_0;

    ret = PortChange(enUartPort,&enUartIdx);
    if (ret != HI_ERR_SUCCESS)
    {
        return ret;
    }


    len = (HI_S32)HI_MDM_UART_Write(enUartIdx, pucBuffer, usNumberOfBytesToWrite);
    if(len == -1)
    {
        *pusNumberOfBytesWritten = (HI_U16)(-1);
        return HI_ERR_SYSTEM_CALL_ERROR;
    }
    else if(len == 0)
    {
        *pusNumberOfBytesWritten = 0;
        return HI_ERR_NOT_FREEE_SPACE;
    }
    else
    {
        *pusNumberOfBytesWritten = (HI_U16)len;
        return HI_ERR_SUCCESS;
    }
}


