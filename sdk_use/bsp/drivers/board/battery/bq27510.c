/************************************************I2C操作函数***********************************************************/
#include <hi_types.h>
#include <hi_mdm_types.h>
#if defined (PRODUCT_CFG_BQ_SUPPORT)
#include <hi_driver.h>
#include <hi_mdm_sem.h>
#include "bq27510.h"
#include "battery_debug.h"

HI_U32 g_battert_sem;
/**************************************************************
函数名:bq27510_write_bytes
功能:对I2C设备进行，连续若干字节写操作
参数:clintAddress:设备地址左移1位
     param:连续写入的数据指针
     len:数据长度
返回值:对I2C设备操作错误
***************************************************************/
HI_U32 bq27510_write_bytes(HI_U8 clintAddress,HI_U8 *param,HI_U32 len)
{
    HI_U32 ret=HI_ERR_SUCCESS;
    HI_I2C_DATA_S dataStruct = {0};
    //TODO:如参判断
    (hi_void)memset_s((HI_VOID *)&dataStruct, sizeof(dataStruct), 0x00, sizeof(dataStruct));
    dataStruct.pSendBuf = param;
    dataStruct.ulSendLength = len;
    dataStruct.pReceiveBuf = HI_NULL;
    dataStruct.ulReceiveLength = 0;
    ret = HI_I2C_SendData(HI_I2C_IDX_0, clintAddress,&dataStruct);
    return ret;
}

/**************************************************************
函数名:bq27510_read_bytes
功能:从指定寄存器读取，指定长度数据
参数:

返回值:

***************************************************************/
HI_U32 bq27510_read_bytes(HI_U8 clintAddress,HI_U8 reg,HI_U8 *param,HI_U32 len)
{
    HI_U32 ret=HI_ERR_SUCCESS;
    HI_I2C_DATA_S dataStruct = {0};
    //TODO:如参判断
    (hi_void)memset_s((HI_VOID *)&dataStruct, sizeof(dataStruct), 0x00, sizeof(dataStruct));
    dataStruct.pSendBuf= (HI_U8 *)&reg;
    dataStruct.ulSendLength= 1;
    dataStruct.pReceiveBuf  = param;
    dataStruct.ulReceiveLength = len;
    ret = HI_I2C_SendRecvData(HI_I2C_IDX_0, clintAddress,&dataStruct);
    return ret;
}

/**************************************************************
函数名:bq27510_readVolt
功能:读取电池电压
参数:
返回值:

***************************************************************/
HI_U32 bq27510_readVolt(HI_U16 *pVolt)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    //写数据
    HI_U8 param[2]={0x00};//将0x00->0x00，0x00->0x01
    if(pVolt == HI_NULL)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    BQ_I2C_MUTEX_LOCK();
    ret = bq27510_read_bytes(BQ27510_NORMAL_ADDR, REG_VOLT_LOW,param,2);
    BQ_I2C_MUTEX_UNLOCK();

    if(ret != HI_ERR_SUCCESS)
    {
        //todo:输出错误
    }
    *pVolt = (param[0])|(param[1]<<8);
    //负电压当做0V处理
    /* BEGIN: Added by b00208046, 2014/2/18   问题单号:DTS2014021406532 */
    if(*pVolt >= 0x8000)
    {
        *pVolt = 0;
    }
    /* END:   Added by b00208046, 2014/2/18 */
    return ret;
}


/**************************************************************
函数名:bq27510_readTemperature
功能:读取电池温度
参数:
返回值:

***************************************************************/
HI_U32 bq27510_readTemperature(HI_U16 *pTemp)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    //写数据
    HI_U8 param[2]={0x00};//将0x00->0x00，0x00->0x01
    if(pTemp == HI_NULL)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    BQ_I2C_MUTEX_LOCK();
    ret = bq27510_read_bytes(BQ27510_NORMAL_ADDR, REG_TEMP_LOW,param,2);
    BQ_I2C_MUTEX_UNLOCK();

    if(ret != HI_ERR_SUCCESS)
    {
        //todo:输出错误
    }
    *pTemp = (param[0])|(param[1]<<8);
    return ret;
}

/**************************************************************
函数名:bq27510_readCurrent
功能:   读取电池电流
参数:
返回值:

***************************************************************/
HI_U32 bq27510_readCurrent(HI_S16 *pCurrent)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    //写数据
    HI_U8 param[2]={0x00};//将0x00->0x00，0x00->0x01
    if(pCurrent == HI_NULL)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    BQ_I2C_MUTEX_LOCK();
    ret=bq27510_read_bytes(BQ27510_NORMAL_ADDR, REG_CURRENT_LOW,param,2);
    BQ_I2C_MUTEX_UNLOCK();
    if(ret!=HI_ERR_SUCCESS)
    {
        //todo:输出错误
    }
    *pCurrent = (HI_S16)((param[1]<<8)|param[0]);
    return ret;
}

/**************************************************************
函数名:bq27510_readBatPercent
功能:读取电池温度
参数:
返回值:
***************************************************************/
HI_U32 bq27510_readBatPercent(HI_U8 *batpercent)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    //写数据
    HI_U8 param[2]={0x00};//将0x00->0x00，0x00->0x01

    if(batpercent == HI_NULL)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    BQ_I2C_MUTEX_LOCK();
    ret=bq27510_read_bytes(BQ27510_NORMAL_ADDR, REG_SOC_LOW,param,2);
    BQ_I2C_MUTEX_UNLOCK();

    if(ret!=HI_ERR_SUCCESS)
    {
        //todo:输出错误
    }
    *batpercent = param[0];

    return ret;
}

/**************************************************************
函数名:bq27510_readFirmWareMainVersion
功能:读取型号版本
参数:
返回值:

***************************************************************/
HI_U32 bq27510_readFirmWareMainVersion(HI_U16 *mainVer)
{
    //读取device type
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
函数名:bq27510_readFirmWareSubVersion
功能:读取子版本号
参数:
返回值:
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
函数名:bq27510_resetChip
功能:复位bq芯片,不带信号量保护
参数:
返回值:
***************************************************************/

HI_PRV HI_U32 bq27510_resetChip2(HI_VOID)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    //写数据
    HI_U8 cmd[3]={0x00,0x41,0x00};//将0x00->0x00，0x00->0x01
    ret = bq27510_write_bytes(BQ27510_NORMAL_ADDR,cmd, sizeof(cmd));
    if(ret!=HI_ERR_SUCCESS)
    {
        //todo:输出错误
    }
    HI_MDM_Sleep(BQ_UPG_SLEEP_AFTER_RESET);
    return ret;
}


/**************************************************************
函数名:bq27510_resetChip
功能:复位bq芯片
参数:
返回值:
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