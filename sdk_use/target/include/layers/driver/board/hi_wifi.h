
#ifndef __HI_WIFI_H__
#define __HI_WIFI_H__
#include <hi_types.h>
#include <hi_mdm_types.h>
#include <icc_protocal_filemanager.h>

#ifdef PRODUCT_CFG_SUPPORT_WIFI
/*****************************************************************************
 ��������: HI_WIFI_SetSsid
 ��������: ����WIFI����
 ����˵��: ucwifiname:Ҫ���õ�WIFI���֣����鳤�Ȳ�����19���ֽ�
 �� �� ֵ: 0��ʾ�ɹ�
 ����Ҫ��: �ڿ���WIFI֮����ȴ�ʮ��ſɵ���HI_MDM_SetWifiSsid������
 ���þ���: TODO: ...
 ��    ��:
*****************************************************************************/
HI_U32 HI_WIFI_SetSsid(HI_IN HI_CHAR ucwifiname[WIFI_SSID_LENGTH+1]);
/*****************************************************************************
 ��������: HI_WIFI_GetSsid
 ��������: ��ȡWIFI����
 ����˵��: ucwifiname:��ȡWIFI���ֵ��׵�ַ�����鳤�ȵ���19���ֽ�
 �� �� ֵ: 0�ɹ�
 ����Ҫ��: �ڿ���WIFI֮����ȴ�ʮ��ſɵ���HI_MDM_GetWifiSsid����
 ���þ���: TODO: ...
 ��    ��:
*****************************************************************************/
HI_U32 HI_WIFI_GetSsid(HI_OUT HI_CHAR ucwifiname[WIFI_SSID_LENGTH+1]);
HI_U32 HI_WIFI_On(HI_VOID);
HI_U32 HI_WIFI_Off(HI_VOID);

#endif

#endif

