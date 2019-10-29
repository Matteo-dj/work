
#ifndef __HI_BATTERY_H__
#define __HI_BATTERY_H__
#include <hi_types.h>
#include <hi_mdm_types.h>
typedef enum
{
    HI_BAT_PERCENT_ERR_NONE,//没问题
    HI_BAT_PERCENT_ERR_INITING,//初始化中
    HI_BAT_PERCENT_ERR_VER_WRONG,//BQ芯片版本号不正确
    HI_BAT_PERCENT_ERR_NOT_EXIST_BATTERY,//电池不在位
    HI_BAT_PERCENT_ERR_PERCENT_WRONG,//电量不准确
}HI_BAT_PERCENT_ERR_E;


#if defined (PRODUCT_CFG_BQ_SUPPORT)
HI_U32 HI_BAT_Init(HI_VOID);
HI_U32 HI_BAT_UpgradeChip(HI_PVOID pFile, HI_U32 ulFileSize);
HI_U32 HI_BAT_ReadChipVer(HI_U16 *pMainVer,HI_U8 *pSubVer);
HI_U32 HI_BAT_ReportBatStat(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption);
HI_U32 HI_BAT_ReportBatteryPercent(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption);
/**************************************************************
名称:HI_BAT_GetBatPercent
功能:更新电池状态信息
参数:bUpdateInitStat是否更新初始化状态
    HI_TRUE:是
    HI_FALSE:否
返回值:
***************************************************************/
HI_U32 HI_BAT_GetBatPercent(HI_U8 *pBatPercent,HI_BAT_PERCENT_ERR_E * penErrCode);


//*****************************************************************************
// 函数名称: HI_BAT_GetCurrent
// 功能描述: 获取电池充放电流(mA)
// 参数说明: 无
// 返 回 值: 无
// 调用要求:无
// 调用举例: TODO: ...
// 作    者:
//*****************************************************************************
HI_S16 HI_BAT_GetCurrent(HI_VOID);
//*****************************************************************************
// 函数名称: HI_BAT_GetVoltValue
// 功能描述: 获取电池电压(mv),存储的实际值
// 参数说明: 无
// 返 回 值: 无
// 调用要求:无
// 调用举例: TODO: ...
// 作    者:
//*****************************************************************************
HI_U16 HI_BAT_GetVoltValue(HI_VOID);
//*****************************************************************************
// 函数名称: HI_BAT_GetTemperatureValue
// 功能描述: 获取电池温度(0.1开),存储的实际值
// 参数说明: 无
// 返 回 值: 无
// 调用要求:无
// 调用举例: TODO: ...
// 作    者:
//*****************************************************************************
HI_U16 HI_BAT_GetTemperatureValue(HI_VOID);

//*****************************************************************************
// 函数名称: HI_BAT_GetOriginalBatPercent
// 功能描述: 获取电池电量(%),存储的实际值
// 参数说明: 无
// 返 回 值: 无
// 调用要求:无
// 调用举例: TODO: ...
// 作    者:
//*****************************************************************************
HI_U8  HI_BAT_GetOriginalBatPercent(HI_VOID);
#endif
#endif

