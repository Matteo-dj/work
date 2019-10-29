#include <hi_types.h>
#include <hi_mdm_types.h>
#if defined (PRODUCT_CFG_HW_RTC_SUPPORT)
#include <hi_rtc.h>
#include <hi_mdm_i2c.h>
#include <hi_stdlib.h>
#define DS1672_DEV_ADDR 0xd0 //DS1672的器件地址
#define DS1672_COUNT_LENTH 4 //0x00-0x03为计数字节的地址
#define DS1672_TOTAL_LENTH 6 //0x04-0x05为控制字节的地址
#define DS1672_CONTROL_LENTH 2 //EOSC使能位，地址0x04
#define DS1672_TRICKLE_LENTH 2 //充电配置位，地址0x05

/*初始化DS1672，芯片使能，并采用二极管+250欧电阻的充电模式*/
HI_U32 HI_RTC_Init(HI_VOID)
{
    HI_I2C_DATA_S  stDs1672_add_point;
    HI_U32 status;
    HI_U8  ucSenddata[DS1672_CONTROL_LENTH] = {0x05,0xa9};
    HI_U8  ucAddr[DS1672_CONTROL_LENTH] = {0x04,0x0};
    HI_I2C_Init(HI_I2C_IDX_0);    
    
    /*把EOSC位写入0，使芯片使能，开始计数*/
    stDs1672_add_point.pSendBuf = ucAddr;
    stDs1672_add_point.ulSendLength = DS1672_CONTROL_LENTH;  
	status = HI_I2C_SendData(HI_I2C_IDX_0,DS1672_DEV_ADDR,&stDs1672_add_point);
    if (status != HI_ERR_SUCCESS)
    {
        return status;
    }
    

    /*把trickle位写1010 1001，使用二极管+250欧电阻*/
    stDs1672_add_point.pSendBuf = ucSenddata;
    stDs1672_add_point.ulSendLength = DS1672_TRICKLE_LENTH;  
    status = HI_I2C_SendData(HI_I2C_IDX_0,DS1672_DEV_ADDR,&stDs1672_add_point);
    if (status != HI_ERR_SUCCESS)
    {
        return status;
    }

    
    return HI_ERR_SUCCESS;
}

/*读取时间*/
/*传入一个变量地址，使得函数调用结束后，把得到的时间返回给这个变量*/
HI_U32 HI_RTC_ReadTime(HI_U32 *pulTime)
{
#ifndef PRODUCT_CFG_SUPPORT_CCT3911
    HI_I2C_DATA_S  stDs1672_add_point;
    HI_U32 status;
    HI_U8  ucReadBuf[DS1672_COUNT_LENTH] = {0};
    HI_U8  ucAddr = 0x0;

    if (pulTime == HI_NULL)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    stDs1672_add_point.pSendBuf = &ucAddr;
    stDs1672_add_point.ulSendLength = 1;  
	stDs1672_add_point.ulReceiveLength = DS1672_COUNT_LENTH;
    stDs1672_add_point.pReceiveBuf = ucReadBuf;

	status = HI_I2C_SendRecvData(HI_I2C_IDX_0,DS1672_DEV_ADDR,&stDs1672_add_point);
    if (status != HI_ERR_SUCCESS)
    {
        return status;
    }   
    /*把4个字节按小端模式通过移位或合并成一个。*/
    *pulTime = (ucReadBuf[3]<<24)
                |(ucReadBuf[2]<<16)
                |(ucReadBuf[1]<<8)
                |(ucReadBuf[0]);
#else
    HI_U32 ulRtcTime = (HI_MDM_GetRealTime() - 60 * 60 * 8);
    *pulTime = ulRtcTime;
#endif

    return HI_ERR_SUCCESS;
}

/*写入时间*/
/*传入一个32位的变量，使得RTC从这个值的基础上开始计数*/
HI_U32 HI_RTC_WriteTime(HI_U32 ulTime)
{
#ifndef PRODUCT_CFG_SUPPORT_CCT3911
    HI_I2C_DATA_S  stDs1672_add_point = {0};
    HI_U32 status;
    HI_U8  ucSendBuf[DS1672_COUNT_LENTH+1] = {0};
    HI_U8  ucAddr = 0x0;

    ucSendBuf[0] = ucAddr;
    
    /*小端数据直接拷贝，把传入的参数ultime由低到高拆开，
    分别存放在数组的第2,3,4个元素中，第三个参数为ultime的大小*/
    (hi_void)memcpy_s(&ucSendBuf[1],DS1672_COUNT_LENTH, &ulTime, sizeof(HI_U32));

    stDs1672_add_point.pSendBuf = ucSendBuf;
    stDs1672_add_point.ulSendLength = DS1672_COUNT_LENTH+1;  

	status = HI_I2C_SendData(HI_I2C_IDX_0, DS1672_DEV_ADDR, &stDs1672_add_point);
    if (status != HI_ERR_SUCCESS)
    {
        return status;
    }

    return HI_ERR_SUCCESS;
#else
    return HI_MDM_SetRealTime(ulTime + 60 * 60 * 8);
#endif
}
#endif
