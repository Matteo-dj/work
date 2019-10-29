#include <hi_types.h>


#if defined (PRODUCT_CFG_HW_RTC_SUPPORT)
#include "drv_dfx_cmn_inc.h"
#include "drv_dfx_rtc.h"
#include <hi_rtc.h>
#define DRV_DFX_RTC_TIME_TOLERATE  (3)//Æ«ÒÆÖµ

HI_U32 dfx_Set_Ds1672_Time(HI_U32 ulWriteTime)
{
    HI_U32 ulReadTime;
    HI_U32 ret;

    ret = HI_RTC_WriteTime(ulWriteTime);
    if (ret != HI_ERR_SUCCESS)
    {
        return HI_ERR_FAILURE;
    }

    ret = HI_RTC_ReadTime(&ulReadTime);
    if (ret != HI_ERR_SUCCESS)
    {
        return HI_ERR_FAILURE;
    }
    if((ulReadTime >= (ulWriteTime+DRV_DFX_RTC_TIME_TOLERATE))||
        (ulReadTime < ulWriteTime))
    {
        return HI_ERR_FAILURE;
    }

    return HI_ERR_SUCCESS;

}

HI_U32  dfx_rtc_time(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption)
{
    HI_U32 ulRtcTime = 0;
    HI_U32 ret;
    DRV_DFX_NDM_RTC stRtcTime;

    (hi_void)memcpy_s(&stRtcTime, sizeof(DRV_DFX_NDM_RTC), pCmdParam, sizeof(DRV_DFX_NDM_RTC));
    if(HI_TRUE == SAL_IsMeterCheckProduct())
    {
        stRtcTime.ulRet = HI_ERR_RTC_ERR_INIT;
        HI_DIAG_ReportPacket(usId, (HI_U8)ulOption,(HI_PBYTE)&stRtcTime,
             (HI_U16)sizeof(DRV_DFX_NDM_RTC), HI_TRUE);
        return HI_ERR_SUCCESS;

    }

    if(HI_ERR_SUCCESS != HI_RTC_ReadTime(&ulRtcTime))
    {
        stRtcTime.ulRet = HI_ERR_RTC_ERR_READ;
        HI_DIAG_ReportPacket(usId, (HI_U8)ulOption,
            (HI_PBYTE)&stRtcTime, (HI_U16)sizeof(DRV_DFX_NDM_RTC), HI_TRUE);
        return HI_ERR_FAILURE;
    }

    /* ÅÐ¶ÏÊ±¼ä£¬ÔÊÐí3ÃëÎó²î*/
    if(((stRtcTime.ulRtcSecNum - DRV_DFX_RTC_TIME_TOLERATE) < ulRtcTime)
        &&((stRtcTime.ulRtcSecNum + DRV_DFX_RTC_TIME_TOLERATE) > ulRtcTime))
    {
        stRtcTime.ulRet = HI_ERR_SUCCESS;
        HI_DIAG_ReportPacket(usId, (HI_U8)ulOption,(HI_PBYTE)&stRtcTime,
            (HI_U16)sizeof(DRV_DFX_NDM_RTC), HI_TRUE);
        return HI_ERR_SUCCESS;
    }

    ret = dfx_Set_Ds1672_Time(stRtcTime.ulRtcSecNum);
    if(ret != HI_ERR_SUCCESS)
    {
        stRtcTime.ulRet = HI_ERR_RTC_ERR_WRITE;
    }
    else
    {
        stRtcTime.ulRet = HI_ERR_SUCCESS;
#ifndef PRODUCT_CFG_SUPPORT_CCT3911
#if ((!defined(PRODUCT_CFG_BOARD_TYPE_ASIC)) && (defined(PRODUCT_CFG_HW_CHECK)))		
		HI_MDM_SetRealTime(stRtcTime.ulRtcSecNum + 60 * 60 * 8);
#endif
#else
        HI_MDM_SetRealTime(stRtcTime.ulRtcSecNum + 60 * 60 * 8);
#endif
    }
    HI_DIAG_ReportPacket(usId, (HI_U8)ulOption,(HI_PBYTE)&stRtcTime,
        (HI_U16)sizeof(DRV_DFX_NDM_RTC), HI_TRUE);

    return HI_ERR_SUCCESS;
}
#endif

