/************************************************I2C��������***********************************************************/
#include <hi_types.h>
#include <hi_mdm_types.h>
#if defined (PRODUCT_CFG_BQ_SUPPORT)
#include <hi_driver.h>
#include <hi_mdm_sem.h>
#include "bq27510.h"
#include "battery_debug.h"

HI_U32 g_battert_sem;
/**************************************************************
������:bq27510_write_bytes
����:��I2C�豸���У����������ֽ�д����
����:clintAddress:�豸��ַ����1λ
     param:����д�������ָ��
     len:���ݳ���
����ֵ:��I2C�豸��������
***************************************************************/
HI_U32 bq27510_write_bytes(HI_U8 clintAddress,HI_U8 *param,HI_U32 len)
{
    HI_U32 ret=HI_ERR_SUCCESS;
    HI_I2C_DATA_S dataStruct = {0};
    //TODO:����ж�
    (hi_void)memset_s((HI_VOID *)&dataStruct, sizeof(dataStruct), 0x00, sizeof(dataStruct));
    dataStruct.pSendBuf = param;
    dataStruct.ulSendLength = len;
    dataStruct.pReceiveBuf = HI_NULL;
    dataStruct.ulReceiveLength = 0;
    ret = HI_I2C_SendData(HI_I2C_IDX_0, clintAddress,&dataStruct);
    return ret;
}

/**************************************************************
������:bq27510_read_bytes
����:��ָ���Ĵ�����ȡ��ָ����������
����:

����ֵ:

***************************************************************/
HI_U32 bq27510_read_bytes(HI_U8 clintAddress,HI_U8 reg,HI_U8 *param,HI_U32 len)
{
    HI_U32 ret=HI_ERR_SUCCESS;
    HI_I2C_DATA_S dataStruct = {0};
    //TODO:����ж�
    (hi_void)memset_s((HI_VOID *)&dataStruct, sizeof(dataStruct), 0x00, sizeof(dataStruct));
    dataStruct.pSendBuf= (HI_U8 *)&reg;
    dataStruct.ulSendLength= 1;
    dataStruct.pReceiveBuf  = param;
    dataStruct.ulReceiveLength = len;
    ret = HI_I2C_SendRecvData(HI_I2C_IDX_0, clintAddress,&dataStruct);
    return ret;
}

/**************************************************************
������:bq27510_readVolt
����:��ȡ��ص�ѹ
����:
����ֵ:

***************************************************************/
HI_U32 bq27510_readVolt(HI_U16 *pVolt)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    //д����
    HI_U8 param[2]={0x00};//��0x00->0x00��0x00->0x01
    if(pVolt == HI_NULL)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    BQ_I2C_MUTEX_LOCK();
    ret = bq27510_read_bytes(BQ27510_NORMAL_ADDR, REG_VOLT_LOW,param,2);
    BQ_I2C_MUTEX_UNLOCK();

    if(ret != HI_ERR_SUCCESS)
    {
        //todo:�������
    }
    *pVolt = (param[0])|(param[1]<<8);
    //����ѹ����0V����
    /* BEGIN: Added by b00208046, 2014/2/18   ���ⵥ��:DTS2014021406532 */
    if(*pVolt >= 0x8000)
    {
        *pVolt = 0;
    }
    /* END:   Added by b00208046, 2014/2/18 */
    return ret;
}


/**************************************************************
������:bq27510_readTemperature
����:��ȡ����¶�
����:
����ֵ:

***************************************************************/
HI_U32 bq27510_readTemperature(HI_U16 *pTemp)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    //д����
    HI_U8 param[2]={0x00};//��0x00->0x00��0x00->0x01
    if(pTemp == HI_NULL)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    BQ_I2C_MUTEX_LOCK();
    ret = bq27510_read_bytes(BQ27510_NORMAL_ADDR, REG_TEMP_LOW,param,2);
    BQ_I2C_MUTEX_UNLOCK();

    if(ret != HI_ERR_SUCCESS)
    {
        //todo:�������
    }
    *pTemp = (param[0])|(param[1]<<8);
    return ret;
}

/**************************************************************
������:bq27510_readCurrent
����:   ��ȡ��ص���
����:
����ֵ:

***************************************************************/
HI_U32 bq27510_readCurrent(HI_S16 *pCurrent)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    //д����
    HI_U8 param[2]={0x00};//��0x00->0x00��0x00->0x01
    if(pCurrent == HI_NULL)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    BQ_I2C_MUTEX_LOCK();
    ret=bq27510_read_bytes(BQ27510_NORMAL_ADDR, REG_CURRENT_LOW,param,2);
    BQ_I2C_MUTEX_UNLOCK();
    if(ret!=HI_ERR_SUCCESS)
    {
        //todo:�������
    }
    *pCurrent = (HI_S16)((param[1]<<8)|param[0]);
    return ret;
}

/**************************************************************
������:bq27510_readBatPercent
����:��ȡ����¶�
����:
����ֵ:
***************************************************************/
HI_U32 bq27510_readBatPercent(HI_U8 *batpercent)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    //д����
    HI_U8 param[2]={0x00};//��0x00->0x00��0x00->0x01

    if(batpercent == HI_NULL)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    BQ_I2C_MUTEX_LOCK();
    ret=bq27510_read_bytes(BQ27510_NORMAL_ADDR, REG_SOC_LOW,param,2);
    BQ_I2C_MUTEX_UNLOCK();

    if(ret!=HI_ERR_SUCCESS)
    {
        //todo:�������
    }
    *batpercent = param[0];

    return ret;
}

/**************************************************************
������:bq27510_readFirmWareMainVersion
����:��ȡ�ͺŰ汾
����:
����ֵ:

***************************************************************/
HI_U32 bq27510_readFirmWareMainVersion(HI_U16 *mainVer)
{
    //��ȡdevice type
    HI_U8 cmd[3]={0x00,0x08,0x00};
    HI_U8 param[2]={0};
    HI_U8 readReg=0;
    HI_U32 ret = HI_ERR_SUCCESS;

    BQ_I2C_MUTEX_LOCK();
    ret = bq27510_write_bytes(BQ27510_NORMAL_ADDR,cmd, sizeof(cmd));

    if(ret!=0)
    {
        BQ_I2C_MUTEX_UNLOCK();
        DRV_NDM_DEBUG_UART_Print("test_version_2 write ERR_CODE=%u\r\n",ret);
        return ret;
    }

    ret = bq27510_read_bytes(BQ27510_NORMAL_ADDR, readReg,param, sizeof(param));
    if(ret!=0)
    {
        BQ_I2C_MUTEX_UNLOCK();
        DRV_NDM_DEBUG_UART_Print("test_version_2 read ERR_CODE=%u\r\n",ret);
        return ret;
    }
    BQ_I2C_MUTEX_UNLOCK();
    *mainVer = (param[1]<<8)|(param[0]);

    return ret;
}

/**************************************************************
������:bq27510_readFirmWareSubVersion
����:��ȡ�Ӱ汾��
����:
����ֵ:
***************************************************************/
HI_U32 bq27510_readFirmWareSubVersion(HI_U8 *firmWareB)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U8 cmd[3]={0x3E,0x39,0x00};
    HI_U8 param[1]={0};

    BQ_I2C_MUTEX_LOCK();
    ret = bq27510_write_bytes(BQ27510_NORMAL_ADDR,cmd, sizeof(cmd));
    if(ret!=HI_ERR_SUCCESS)
    {
        BQ_I2C_MUTEX_UNLOCK();
        return ret;
    }

    HI_MDM_Sleep(20);
    ret = bq27510_read_bytes(BQ27510_NORMAL_ADDR, 0x40,param,1);
    if(ret!=HI_ERR_SUCCESS)
    {
        BQ_I2C_MUTEX_UNLOCK();
        return ret;
    }
    BQ_I2C_MUTEX_UNLOCK();
    *firmWareB = param[0];

    return ret;
}

/**************************************************************
������:bq27510_resetChip
����:��λbqоƬ,�����ź�������
����:
����ֵ:
***************************************************************/

HI_PRV HI_U32 bq27510_resetChip2(HI_VOID)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    //д����
    HI_U8 cmd[3]={0x00,0x41,0x00};//��0x00->0x00��0x00->0x01
    ret = bq27510_write_bytes(BQ27510_NORMAL_ADDR,cmd, sizeof(cmd));
    if(ret!=HI_ERR_SUCCESS)
    {
        //todo:�������
    }
    HI_MDM_Sleep(BQ_UPG_SLEEP_AFTER_RESET);
    return ret;
}


/**************************************************************
������:bq27510_resetChip
����:��λbqоƬ
����:
����ֵ:
***************************************************************/
HI_U32 bq27510_resetChip(HI_VOID)
{
    HI_U32 ret=HI_ERR_SUCCESS;
    BQ_I2C_MUTEX_LOCK();
    ret=bq27510_resetChip2();
    BQ_I2C_MUTEX_UNLOCK();
    return ret;
}

HI_U32 bq27510_init(HI_VOID)
{
    return HI_MDM_SEM_BCreate(&g_battert_sem,"battery",HI_SEM_ONE);
}
#endif//PRODUCT_CFG_BQ_SUPPORT