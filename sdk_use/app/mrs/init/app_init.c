//*****************************************************************************
//
//                  版权所有 (C), 1998-2011, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : app_init.c
//  版 本 号   : V1.0
//  作    者   : shenhankun/00130424
//  生成日期   : 2011-12-24
//  功能描述   : APP
//
//  函数列表   : TODO: ...
//  修改历史   :
//  1.日    期 : 2011-12-24
//    作    者 : shenhankun/00130424
//    修改内容 : 创建文件
//
//*****************************************************************************

//*****************************************************************************
// PROJECT   :
// SUBSYSTEM :
// MODULE    :
// OWNER     :
//*****************************************************************************
#include "app_common.h"
#include "app_dfx.h"
#include <hi_usr_nv.h>
#include "app_ndm_init.h"
#include "mrs_fw_sgd_flash.h"
#define BIT_FIELD_MASK(n) ((1<<n)-1)
//////////////////////////////////////////////////////////////////////////////
// 在 app_res_cfg.h 文件中 增删任务/队列, 调整任务队列的配置(如名称、大小)
//////////////////////////////////////////////////////////////////////////////
HI_PRV HI_VOID Set_UserInfo(HI_VOID)
{
    HI_U32 ret;
    NV_MRS_USER_VER_S stUserVer;
#if defined(PRODUCT_CFG_PRODUCT_TYPE_NDM)
    HI_NV_FTM_MANUFACOTR_ID_S sFtmManuId;
#else
    HI_U8 aucCode[3] = {0};
#endif
    HI_MDM_USER_INFO_S stUserInfo;

    (hi_void)memset_s(&stUserInfo, sizeof(stUserInfo), 0, sizeof(stUserInfo));
    (HI_VOID)HI_MDM_GetUserInfo(&stUserInfo);
#if !defined(PRODUCT_CFG_PRODUCT_TYPE_NDM)
	FLASH_USR_CFG_INFO_S stCode;
#endif
    //读客户版本信息
    (hi_void)memset_s(&stUserVer, sizeof(stUserVer), 0, sizeof(stUserVer));
    ret = HI_MDM_NV_Read(ID_NV_MRS_USER_VER, &stUserVer, sizeof(NV_MRS_USER_VER_S));
    if(ret != HI_ERR_SUCCESS)
    {
        (hi_void)memset_s(&stUserVer, sizeof(NV_MRS_USER_VER_S), 0, sizeof(NV_MRS_USER_VER_S));
    }

    stUserInfo.usSwVer  = stUserVer.usSwVer;
    stUserInfo.usYear   = (stUserVer.aucDate[0])&BIT_FIELD_MASK(7);
    stUserInfo.usMonth  = (stUserVer.aucDate[1])&BIT_FIELD_MASK(4);
    stUserInfo.usDay    = (stUserVer.aucDate[2])&BIT_FIELD_MASK(5);
    stUserInfo.szChipCode[0] = stUserVer.szChipCode[1];
    stUserInfo.szChipCode[1] = stUserVer.szChipCode[0];

    //设置厂商信息
#if defined(PRODUCT_CFG_PRODUCT_TYPE_NDM)
    (hi_void)memset_s(&sFtmManuId, sizeof(sFtmManuId), 0, sizeof(sFtmManuId));
    ret = HI_MDM_NV_Read(HI_NV_FTM_MANUFACOTR_ID, &sFtmManuId, sizeof(HI_NV_FTM_MANUFACOTR_ID_S));
    if(ret != HI_ERR_SUCCESS)
    {
        (hi_void)memset_s(&sFtmManuId, sizeof(HI_NV_FTM_MANUFACOTR_ID_S), 0, sizeof(HI_NV_FTM_MANUFACOTR_ID_S));
    }
    stUserInfo.szManuCode[0] = sFtmManuId.szId[0];
    stUserInfo.szManuCode[1] = sFtmManuId.szId[1];
#else
     (hi_void)memset_s(&stCode, sizeof(stCode),0,sizeof(stCode));
    ret = mrsSgdFlashRead(ID_FLASH_USR_CFG, &stCode,sizeof(stCode));
    if(ret != HI_ERR_SUCCESS)
    {
        //HI_MDM_Memzero(&stNvCfg, sizeof(stNvCfg));
    }
	else
	{
		(hi_void)memcpy_s(aucCode,2,&stCode, 2);
		if((aucCode[0]== 0xff && aucCode[1]== 0xff) || (aucCode[0]== 0 && aucCode[1]== 0))
		{
			HI_NV_FTM_MANUFACOTR_ID_S sFtmManuId;
			(hi_void)memset_s(&sFtmManuId, sizeof(sFtmManuId), 0, sizeof(sFtmManuId));
			ret = HI_MDM_NV_Read(HI_NV_FTM_MANUFACOTR_ID, &sFtmManuId, sizeof(HI_NV_FTM_MANUFACOTR_ID_S));
			if(ret == HI_ERR_SUCCESS)
			{
				aucCode[0] = sFtmManuId.szId[0];
				aucCode[1] = sFtmManuId.szId[1];
				if (aucCode[0] == 'g' && aucCode[1] == 'd')
				{
					aucCode[0] = 'G';
					aucCode[1] = 'D';
				}
				if (aucCode[0] == 't' && aucCode[1] == 'x')
				{
					aucCode[0] = 'T';
					aucCode[1] = 'X';
				}
			}
			else
			{
				aucCode[0] = 'G';
				aucCode[1] = 'D';
			}
		}
	}
    stUserInfo.szManuCode[0] = aucCode[1];
    stUserInfo.szManuCode[1] = aucCode[0];
#endif

    (HI_VOID)HI_MDM_SetUserInfo(&stUserInfo);
}

HI_VOID APP_Init(HI_VOID)
{
    HI_FTM_PRODUCT_TYPE_E enProductType = HI_MDM_GetProductType();
#ifdef PRODUCT_CFG_PRODUCT_TYPE_NDM
    APP_NdmInit();
#endif
    
#ifdef PRODUCT_CFG_BOARD_TYPE_ASIC
    if (HI_FTM_CHIP_TYPE_3911C == HI_MDM_GetHwChipType())
#endif
    {
        HI_MDM_EthInit();
    }

    Set_UserInfo();
    HI_MDM_InitStdProtocol();
    if (HI_MDM_IsProductCon(enProductType) || HI_MDM_IsProductMeter(enProductType)||HI_MDM_IsProductMeterI(enProductType)
        || HI_MDM_IsProductMeterII(enProductType) || HI_MDM_IsProductNdm(enProductType)
        || HI_MDM_IsProductTSta(enProductType))
    {
        MRS_Init();

     #ifndef HAVE_PCLINT_CHECK
        //APP VERSION
        HI_MDM_CONFIG_APP_VER(VERSION_MAIN,VERSION_FEATURE,VERSION_BUG);
     #endif

        appDfxInit();
    }
}

