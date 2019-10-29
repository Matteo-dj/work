#include <hi_types.h>
#include <hi_mdm_types.h>
#if defined (PRODUCT_CFG_BQ_SUPPORT)

#include <dfx_battery.h>
#include <hi_battery.h>
#include <hi_sal.h>//HI_DIAG_ReportPacket
#include "bq27510.h"
/*************************************************宏 调试宏************************************************************/
/*************************************************全局变量*************************************************************/
BQ27510_STATUS_S g_bq27510_status;
/**********************************************逻辑功能函数****************************************************************/
//*****************************************************************************
// 函数名称: battery_is_chip_ready
// 功能描述: 判断BQ芯片是否初始化完毕,只有当BQ芯片初始化完毕读取到的值才是正确的。除(BQ芯片损坏,i2c通信)等异常情况。
// 参数说明: NA
// 返 回 值: HI_TRUE:BQ芯片初始化完毕
//           HI_FALSE:BQ芯片初始化中
// 调用要求: 无
// 调用举例: TODO: ...
// 作    者:
//*****************************************************************************
HI_PRV HI_BOOL battery_is_chip_ready(HI_VOID)
{
    if(g_bq27510_status.isInit == HI_FALSE || g_bq27510_status.isResetting == HI_TRUE)
    {
        return HI_FALSE;
    }
    return HI_TRUE;
}

//*****************************************************************************
// 函数名称: HI_BAT_GetBatPercent
// 功能描述: 获取电池电量百分比(逻辑处理后的值)
// 参数说明: NA
// 返 回 值: NA
// 调用要求: 无
// 调用举例: TODO: ...
// 作    者:
//*****************************************************************************
HI_U32 HI_BAT_GetBatPercent(HI_U8 *pBatPercent,HI_BAT_PERCENT_ERR_E * penErrCode)
{
    if(pBatPercent==HI_NULL||penErrCode==HI_NULL)
    {
        return HI_ERR_BATTERY_INVALID_PARAM;
    }
    
    if(!g_bq27510_status.isInit)
    {
        return HI_ERR_BATTERY_NOT_INIT;
    }

    //正在初始化,放在第一个判断条件
    if(battery_is_chip_ready()==HI_FALSE)
    {
        *penErrCode = HI_BAT_PERCENT_ERR_INITING;
        return HI_ERR_SUCCESS;
    }

    //芯片版本不正确
    if((g_bq27510_status.mainVer != BQ27510_GOOD_VERSION_A))
    {
        *penErrCode = HI_BAT_PERCENT_ERR_VER_WRONG;
        return HI_ERR_SUCCESS;
    }

    //电池不在位
    if(g_bq27510_status.initIsBatExist == HI_FALSE)//使用上电时的电池在位状态进行判断
    {
        *penErrCode = HI_BAT_PERCENT_ERR_NOT_EXIST_BATTERY;
        return HI_ERR_SUCCESS;
    }

    //电池电量不准确
    if(g_bq27510_status.ucBatteryPercent>100)
    {
        *penErrCode = HI_BAT_PERCENT_ERR_PERCENT_WRONG;
        return HI_ERR_SUCCESS;
    }

    *penErrCode = HI_BAT_PERCENT_ERR_NONE;
    *pBatPercent = g_bq27510_status.ucBatteryPercent;
    if(*pBatPercent >= 95)//规避电池无法充满问题,显示假电量
    {
        *pBatPercent = 100;
    }
    return HI_ERR_SUCCESS;
}

HI_U32 HI_BAT_ReadChipVer(HI_U16 *pMainVer,HI_U8 *pSubVer)
{
    HI_U32 ret = HI_ERR_FAILURE;
    if(!g_bq27510_status.isInit)
    {
        return HI_ERR_BATTERY_NOT_INIT;
    }

    
    if((HI_NULL == pMainVer)||(HI_NULL == pSubVer))
    {
        return HI_ERR_BATTERY_INVALID_PARAM;
    }
    ret = bq27510_readFirmWareMainVersion(pMainVer);
    if(HI_ERR_SUCCESS != ret)
    {
        return ret;
    }
    ret = bq27510_readFirmWareSubVersion(pSubVer);
    if(HI_ERR_SUCCESS != ret)
    {
        return ret;
    }
    return HI_ERR_SUCCESS;
}


HI_U32 HI_BAT_UpgradeChip(HI_PVOID pFile, HI_U32 ulFileSize)
{
    HI_U32 ret = HI_ERR_FAILURE;
    if(!g_bq27510_status.isInit)
    {
        return HI_ERR_BATTERY_NOT_INIT;
    }

    ret = bq27510_CheckUpgFile(0, 0, pFile, ulFileSize);
    if(ret != HI_ERR_SUCCESS)
    {
        return ret;
    }

    return bq27510_Upgrade(0, 0, pFile, ulFileSize);
}

//*****************************************************************************
// 函数名称: HI_BAT_UpdateVersion
// 功能描述: 更新BQ芯片版本号
// 参数说明: NA
// 返 回 值: NA
// 调用要求: 无
// 调用举例: TODO: ...
// 作    者:
//*****************************************************************************
HI_U32 HI_BAT_UpdateVersion(HI_VOID)
{
        //读取芯片版本号，如果读取A版本不正确，B版本赋值0
    HI_U32 ret = HI_ERR_SUCCESS;
    if(!g_bq27510_status.isInit)
    {
        return HI_ERR_BATTERY_NOT_INIT;
    }

        
    ret = bq27510_readFirmWareMainVersion((HI_U16 *)(&g_bq27510_status.mainVer));
    if((g_bq27510_status.mainVer == BQ27510_GOOD_VERSION_A) && (ret == HI_ERR_SUCCESS))
    {
        //读取芯片B版本
        ret = bq27510_readFirmWareSubVersion((HI_U8 *)(&g_bq27510_status.subVer));
        if(ret != HI_ERR_SUCCESS)
        {
            g_bq27510_status.readFirmWareVersionFail++;
            g_bq27510_status.subVer = 0;
        }
    }
    else
    {
        g_bq27510_status.readFirmWareVersionFail++;
        g_bq27510_status.mainVer = 0;
        g_bq27510_status.subVer = 0;
    }
    return ret;
}


//*****************************************************************************
// 函数名称: HI_BAT_UpdateChipValue
// 功能描述: 更新电池状态
// 参数说明: bUpdateInitStat:是否更新初始值 HI_TRUE:是 HI_FALSE:否
//              通常在上电过程和初始化过程中选择HI_TRUE
//
// 返 回 值: NA
// 调用要求: 无
// 调用举例: TODO: ...
// 作    者:
//*****************************************************************************
HI_U32 battery_updata_chip_val(HI_BOOL bFirstUpdate)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U8 ucBatPercent = 0;
    HI_U16 temperature = 0;
    HI_U16 volt = 0;
    HI_S16 ssCurrent = 0;


    //读取更新电流
    ret = bq27510_readCurrent(&ssCurrent);
    if(ret == HI_ERR_SUCCESS)
    {
        g_bq27510_status.current = ssCurrent;
        if(bFirstUpdate)
        {
            g_bq27510_status.initCurrent = ssCurrent;
        }
    }
    else
    {
        g_bq27510_status.readCurrentFailTimes++;
    }

    //读取更新电量
    ret = bq27510_readBatPercent(&ucBatPercent);
    if(ret == HI_ERR_SUCCESS && ucBatPercent <= 100)
    {
        g_bq27510_status.ucBatteryPercent = ucBatPercent;
        if(bFirstUpdate)
        {
            g_bq27510_status.initBatPercent = ucBatPercent;
        }
    }
    else
    {
        g_bq27510_status.readBatPercentFailTimes++;

    }

    //读取更新电压和电池是否在位
    ret = bq27510_readVolt(&volt);
    if(ret == HI_ERR_SUCCESS)
    {
        g_bq27510_status.voltValue = volt;
        if(volt<2000)
        {
            g_bq27510_status.isBatExist = HI_FALSE;
        }
        else
        {
            g_bq27510_status.isBatExist = HI_TRUE;
        }

        if(bFirstUpdate)
        {
            g_bq27510_status.initVoltValue = g_bq27510_status.voltValue;
            g_bq27510_status.initIsBatExist = g_bq27510_status.isBatExist;
        }

    }
    else
    {
        g_bq27510_status.readVoltValFailTimes++;

    }

    //读取更新和电池温度
    ret = bq27510_readTemperature(&temperature);
    if(ret == HI_ERR_SUCCESS)
    {
        g_bq27510_status.temperature = temperature;
        if(bFirstUpdate)
        {
            g_bq27510_status.initTemperature = temperature;
        }
    }
    else
    {
        g_bq27510_status.readTemperatureFailTimes++;

    }
    return ret;
}



HI_U32 battery_init(HI_VOID)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    if(!g_bq27510_status.isInit)
    {

        ret=bq27510_init();
        if(ret!=HI_ERR_SUCCESS)
        {
            return ret;
        }
        //BQ芯片升级初始化
        g_bq27510_status.isInit = HI_TRUE;
        
        //BQ芯片上电时读到的电压值为0,等待BQ芯片中的值变成正常值后再读取。
        HI_MDM_Sleep(2000);

        
        //更新BQ芯片版本号
        (HI_VOID)HI_BAT_UpdateVersion();
        
        //更新电压等数据
        battery_updata_chip_val(HI_TRUE);

    }
    
    return HI_ERR_SUCCESS;
}

HI_U32 HI_BAT_ReportBatStat(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption)
{
    HI_DIAG_ReportPacket(usId, (HI_U8)ulOption, (HI_PBYTE)(&g_bq27510_status), sizeof(g_bq27510_status), HI_TRUE);
    return HI_ERR_SUCCESS;
}
#endif//PRODUCT_CFG_BQ_SUPPORT

