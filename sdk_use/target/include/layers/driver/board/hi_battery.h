
#ifndef __HI_BATTERY_H__
#define __HI_BATTERY_H__
#include <hi_types.h>
#include <hi_mdm_types.h>
typedef enum
{
    HI_BAT_PERCENT_ERR_NONE,//û����
    HI_BAT_PERCENT_ERR_INITING,//��ʼ����
    HI_BAT_PERCENT_ERR_VER_WRONG,//BQоƬ�汾�Ų���ȷ
    HI_BAT_PERCENT_ERR_NOT_EXIST_BATTERY,//��ز���λ
    HI_BAT_PERCENT_ERR_PERCENT_WRONG,//������׼ȷ
}HI_BAT_PERCENT_ERR_E;


#if defined (PRODUCT_CFG_BQ_SUPPORT)
HI_U32 HI_BAT_Init(HI_VOID);
HI_U32 HI_BAT_UpgradeChip(HI_PVOID pFile, HI_U32 ulFileSize);
HI_U32 HI_BAT_ReadChipVer(HI_U16 *pMainVer,HI_U8 *pSubVer);
HI_U32 HI_BAT_ReportBatStat(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption);
HI_U32 HI_BAT_ReportBatteryPercent(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption);
/**************************************************************
����:HI_BAT_GetBatPercent
����:���µ��״̬��Ϣ
����:bUpdateInitStat�Ƿ���³�ʼ��״̬
    HI_TRUE:��
    HI_FALSE:��
����ֵ:
***************************************************************/
HI_U32 HI_BAT_GetBatPercent(HI_U8 *pBatPercent,HI_BAT_PERCENT_ERR_E * penErrCode);


//*****************************************************************************
// ��������: HI_BAT_GetCurrent
// ��������: ��ȡ��س�ŵ���(mA)
// ����˵��: ��
// �� �� ֵ: ��
// ����Ҫ��:��
// ���þ���: TODO: ...
// ��    ��:
//*****************************************************************************
HI_S16 HI_BAT_GetCurrent(HI_VOID);
//*****************************************************************************
// ��������: HI_BAT_GetVoltValue
// ��������: ��ȡ��ص�ѹ(mv),�洢��ʵ��ֵ
// ����˵��: ��
// �� �� ֵ: ��
// ����Ҫ��:��
// ���þ���: TODO: ...
// ��    ��:
//*****************************************************************************
HI_U16 HI_BAT_GetVoltValue(HI_VOID);
//*****************************************************************************
// ��������: HI_BAT_GetTemperatureValue
// ��������: ��ȡ����¶�(0.1��),�洢��ʵ��ֵ
// ����˵��: ��
// �� �� ֵ: ��
// ����Ҫ��:��
// ���þ���: TODO: ...
// ��    ��:
//*****************************************************************************
HI_U16 HI_BAT_GetTemperatureValue(HI_VOID);

//*****************************************************************************
// ��������: HI_BAT_GetOriginalBatPercent
// ��������: ��ȡ��ص���(%),�洢��ʵ��ֵ
// ����˵��: ��
// �� �� ֵ: ��
// ����Ҫ��:��
// ���þ���: TODO: ...
// ��    ��:
//*****************************************************************************
HI_U8  HI_BAT_GetOriginalBatPercent(HI_VOID);
#endif
#endif

