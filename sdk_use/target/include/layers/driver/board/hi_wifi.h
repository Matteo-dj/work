
#ifndef __HI_WIFI_H__
#define __HI_WIFI_H__
#include <hi_types.h>
#include <hi_mdm_types.h>
#include <icc_protocal_filemanager.h>

#ifdef PRODUCT_CFG_SUPPORT_WIFI
/*****************************************************************************
 函数名称: HI_WIFI_SetSsid
 功能描述: 设置WIFI名字
 参数说明: ucwifiname:要设置的WIFI名字，数组长度不大于19个字节
 返 回 值: 0表示成功
 调用要求: 在开关WIFI之后，需等待十秒才可调用HI_MDM_SetWifiSsid函数。
 调用举例: TODO: ...
 作    者:
*****************************************************************************/
HI_U32 HI_WIFI_SetSsid(HI_IN HI_CHAR ucwifiname[WIFI_SSID_LENGTH+1]);
/*****************************************************************************
 函数名称: HI_WIFI_GetSsid
 功能描述: 获取WIFI名字
 参数说明: ucwifiname:获取WIFI名字的首地址，数组长度等于19个字节
 返 回 值: 0成功
 调用要求: 在开关WIFI之后，需等待十秒才可调用HI_MDM_GetWifiSsid函数
 调用举例: TODO: ...
 作    者:
*****************************************************************************/
HI_U32 HI_WIFI_GetSsid(HI_OUT HI_CHAR ucwifiname[WIFI_SSID_LENGTH+1]);
HI_U32 HI_WIFI_On(HI_VOID);
HI_U32 HI_WIFI_Off(HI_VOID);

#endif

#endif

