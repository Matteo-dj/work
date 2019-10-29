#include <hi_types.h>
#include <hi_mdm_types.h>
#if defined (PRODUCT_CFG_HW_RTC_SUPPORT)
#include <hi_rtc.h>
#include <hi_mdm_i2c.h>
#include <hi_stdlib.h>
#define DS1672_DEV_ADDR 0xd0 //DS1672��������ַ
#define DS1672_COUNT_LENTH 4 //0x00-0x03Ϊ�����ֽڵĵ�ַ
#define DS1672_TOTAL_LENTH 6 //0x04-0x05Ϊ�����ֽڵĵ�ַ
#define DS1672_CONTROL_LENTH 2 //EOSCʹ��λ����ַ0x04
#define DS1672_TRICKLE_LENTH 2 //�������λ����ַ0x05

/*��ʼ��DS1672��оƬʹ�ܣ������ö�����+250ŷ����ĳ��ģʽ*/
HI_U32 HI_RTC_Init(HI_VOID)
{
    HI_I2C_DATA_S  stDs1672_add_point;
    HI_U32 status;
    HI_U8  ucSenddata[DS1672_CONTROL_LENTH] = {0x05,0xa9};
    HI_U8  ucAddr[DS1672_CONTROL_LENTH] = {0x04,0x0};
    HI_I2C_Init(HI_I2C_IDX_0);    
    
    /*��EOSCλд��0��ʹоƬʹ�ܣ���ʼ����*/
    stDs1672_add_point.pSendBuf = ucAddr;
    stDs1672_add_point.ulSendLength = DS1672_CONTROL_LENTH;  
	status = HI_I2C_SendData(HI_I2C_IDX_0,DS1672_DEV_ADDR,&stDs1672_add_point);
    if (status != HI_ERR_SUCCESS)
    {
        return status;
    }
    

    /*��trickleλд1010 1001��ʹ�ö�����+250ŷ����*/
    stDs1672_add_point.pSendBuf = ucSenddata;
    stDs1672_add_point.ulSendLength = DS1672_TRICKLE_LENTH;  
    status = HI_I2C_SendData(HI_I2C_IDX_0,DS1672_DEV_ADDR,&stDs1672_add_point);
    if (status != HI_ERR_SUCCESS)
    {
        return status;
    }

    
    return HI_ERR_SUCCESS;
}

/*��ȡʱ��*/
/*����һ��������ַ��ʹ�ú������ý����󣬰ѵõ���ʱ�䷵�ظ��������*/
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
    /*��4���ֽڰ�С��ģʽͨ����λ��ϲ���һ����*/
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

/*д��ʱ��*/
/*����һ��32λ�ı�����ʹ��RTC�����ֵ�Ļ����Ͽ�ʼ����*/
HI_U32 HI_RTC_WriteTime(HI_U32 ulTime)
{
#ifndef PRODUCT_CFG_SUPPORT_CCT3911
    HI_I2C_DATA_S  stDs1672_add_point = {0};
    HI_U32 status;
    HI_U8  ucSendBuf[DS1672_COUNT_LENTH+1] = {0};
    HI_U8  ucAddr = 0x0;

    ucSendBuf[0] = ucAddr;
    
    /*С������ֱ�ӿ������Ѵ���Ĳ���ultime�ɵ͵��߲𿪣�
    �ֱ���������ĵ�2,3,4��Ԫ���У�����������Ϊultime�Ĵ�С*/
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
