#include <hi_types.h>
#include <hi_mdm_types.h>
#if defined (PRODUCT_CFG_BQ_SUPPORT)

#include <dfx_battery.h>
#include <hi_battery.h>
#include <hi_sal.h>//HI_DIAG_ReportPacket
#include "bq27510.h"
/*************************************************�� ���Ժ�************************************************************/
/*************************************************ȫ�ֱ���*************************************************************/
BQ27510_STATUS_S g_bq27510_status;
/**********************************************�߼����ܺ���****************************************************************/
//*****************************************************************************
// ��������: battery_is_chip_ready
// ��������: �ж�BQоƬ�Ƿ��ʼ�����,ֻ�е�BQоƬ��ʼ����϶�ȡ����ֵ������ȷ�ġ���(BQоƬ��,i2cͨ��)���쳣�����
// ����˵��: NA
// �� �� ֵ: HI_TRUE:BQоƬ��ʼ�����
//           HI_FALSE:BQоƬ��ʼ����
// ����Ҫ��: ��
// ���þ���: TODO: ...
// ��    ��:
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
// ��������: HI_BAT_GetBatPercent
// ��������: ��ȡ��ص����ٷֱ�(�߼�������ֵ)
// ����˵��: NA
// �� �� ֵ: NA
// ����Ҫ��: ��
// ���þ���: TODO: ...
// ��    ��:
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

    //���ڳ�ʼ��,���ڵ�һ���ж�����
    if(battery_is_chip_ready()==HI_FALSE)
    {
        *penErrCode = HI_BAT_PERCENT_ERR_INITING;
        return HI_ERR_SUCCESS;
    }

    //оƬ�汾����ȷ
    if((g_bq27510_status.mainVer != BQ27510_GOOD_VERSION_A))
    {
        *penErrCode = HI_BAT_PERCENT_ERR_VER_WRONG;
        return HI_ERR_SUCCESS;
    }

    //��ز���λ
    if(g_bq27510_status.initIsBatExist == HI_FALSE)//ʹ���ϵ�ʱ�ĵ����λ״̬�����ж�
    {
        *penErrCode = HI_BAT_PERCENT_ERR_NOT_EXIST_BATTERY;
        return HI_ERR_SUCCESS;
    }

    //��ص�����׼ȷ
    if(g_bq27510_status.ucBatteryPercent>100)
    {
        *penErrCode = HI_BAT_PERCENT_ERR_PERCENT_WRONG;
        return HI_ERR_SUCCESS;
    }

    *penErrCode = HI_BAT_PERCENT_ERR_NONE;
    *pBatPercent = g_bq27510_status.ucBatteryPercent;
    if(*pBatPercent >= 95)//��ܵ���޷���������,��ʾ�ٵ���
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
// ��������: HI_BAT_UpdateVersion
// ��������: ����BQоƬ�汾��
// ����˵��: NA
// �� �� ֵ: NA
// ����Ҫ��: ��
// ���þ���: TODO: ...
// ��    ��:
//*****************************************************************************
HI_U32 HI_BAT_UpdateVersion(HI_VOID)
{
        //��ȡоƬ�汾�ţ������ȡA�汾����ȷ��B�汾��ֵ0
    HI_U32 ret = HI_ERR_SUCCESS;
    if(!g_bq27510_status.isInit)
    {
        return HI_ERR_BATTERY_NOT_INIT;
    }

        
    ret = bq27510_readFirmWareMainVersion((HI_U16 *)(&g_bq27510_status.mainVer));
    if((g_bq27510_status.mainVer == BQ27510_GOOD_VERSION_A) && (ret == HI_ERR_SUCCESS))
    {
        //��ȡоƬB�汾
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
// ��������: HI_BAT_UpdateChipValue
// ��������: ���µ��״̬
// ����˵��: bUpdateInitStat:�Ƿ���³�ʼֵ HI_TRUE:�� HI_FALSE:��
//              ͨ�����ϵ���̺ͳ�ʼ��������ѡ��HI_TRUE
//
// �� �� ֵ: NA
// ����Ҫ��: ��
// ���þ���: TODO: ...
// ��    ��:
//*****************************************************************************
HI_U32 battery_updata_chip_val(HI_BOOL bFirstUpdate)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U8 ucBatPercent = 0;
    HI_U16 temperature = 0;
    HI_U16 volt = 0;
    HI_S16 ssCurrent = 0;


    //��ȡ���µ���
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

    //��ȡ���µ���
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

    //��ȡ���µ�ѹ�͵���Ƿ���λ
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

    //��ȡ���º͵���¶�
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
        //BQоƬ������ʼ��
        g_bq27510_status.isInit = HI_TRUE;
        
        //BQоƬ�ϵ�ʱ�����ĵ�ѹֵΪ0,�ȴ�BQоƬ�е�ֵ�������ֵ���ٶ�ȡ��
        HI_MDM_Sleep(2000);

        
        //����BQоƬ�汾��
        (HI_VOID)HI_BAT_UpdateVersion();
        
        //���µ�ѹ������
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

