#include <hi_types.h>
#include <hi_mdm_types.h>
#include <hi_eqt_pc_ndm.h>
#include <drv_dfx.h>
#include <dfx_app.h>
#include "app_ndm_diag.h"
#include "app_ndm_msg.h"
#include "app_ndm_tools.h"
#include <hi_battery.h>

HI_PRV HI_U32 appNdmDiagCmdProc(HI_U16 usId, HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption);
HI_PRV HI_U32 appNdmFindMsgIdByDiagId(HI_U16 usDiagId, HI_U16 *pusMsgId);

HI_EXTERN HI_U32 mrsNdmMsgModeQuery(HI_U16 usId, HI_PVOID pMsgParam);
HI_EXTERN HI_U32 mrsNdmMsgModeSet(HI_U16 usId, HI_PVOID pMsgParam);
HI_EXTERN HI_U32 mrsNdmMsgMacSet(HI_U16 usId, HI_PVOID pMsgParam);
HI_EXTERN HI_U32 mrsNdmMsgMacQuery(HI_U16 usId, HI_PVOID pMsgParam);
HI_EXTERN HI_U32 mrsNdmMsgZeroCross(HI_U16 usId, HI_PVOID pMsgParam);
HI_EXTERN HI_U32 mrsNdmMsgPlcTest(HI_U16 usId, HI_PVOID pMsgParam);
HI_EXTERN HI_U32 mrsNdmMsgSsidSet(HI_U16 usId, HI_PVOID pMsgParam);
HI_EXTERN HI_U32 mrsNdmMsgSsidQuery(HI_U16 usId, HI_PVOID pMsgParam);
HI_EXTERN HI_U32 mrsNdmMsgBqLoad(HI_U16 usId, HI_PVOID pMsgParam);
HI_EXTERN HI_U32 mrsNdmMsgBqLoadResultQuery(HI_U16 usId, HI_PVOID pMsgParam);
HI_EXTERN HI_U32 mrsNdmMsgKeyTest(HI_U16 usId, HI_PVOID pMsgParam);
HI_EXTERN HI_U32 mrsNdmMsgDisplayTest(HI_U16 usId, HI_PVOID pMsgParam);
HI_EXTERN HI_U32 mrsNdmMsgTestVerQuery(HI_U16 usId, HI_PVOID pMsgParam);
HI_EXTERN HI_U32 mrsNdmMsgTestInit(HI_U16 usId, HI_PVOID pMsgParam);
HI_EXTERN HI_U32 mrsNdmMsgNtbSyncSet(HI_U16 usId, HI_PVOID pMsgParam);
HI_EXTERN HI_U32 mrsNdmMsgNtbSyncQuery(HI_U16 usId, HI_PVOID pMsgParam);
HI_EXTERN HI_U32 mrsNdmMsgTfCardTest(HI_U16 usId, HI_PVOID pMsgParam);
HI_EXTERN HI_U32 mrsNdmMsgRtcTime(HI_U16 usId, HI_PVOID pMsgParam);
HI_EXTERN HI_U32 mrsNdmMsgDcSdReportStat(HI_U16 usId, HI_PVOID pMsgParam);
HI_EXTERN HI_U32 mrsNdmMsgDcSdSetConfig(HI_U16 usId, HI_PVOID pMsgParam);
HI_EXTERN HI_U32 mrsNdmMsgDcSdQueryConfig(HI_U16 usId, HI_PVOID pMsgParam);


typedef HI_U32 (*MRS_FW_MSG_PROC_PFN)(HI_U16 usId, HI_IN HI_PVOID);
typedef struct
{
    HI_U16 usMinId;
    HI_U16 usMaxId;
    MRS_FW_MSG_PROC_PFN pfnMsg;  // 消息处理的函数指针
} MRS_FW_MSG_PROC_TBL_STRU;

typedef struct
{
    HI_U16 usDiagId;
    HI_U16 usMsgId;
} MRS_NDM_DIAG_MSG_ID_TABLE_STRU;

typedef enum
{
    EN_NDM_EQT_MSG_MODE_QUERY,
    EN_NDM_EQT_MSG_MODE_SET,
    EN_NDM_EQT_MSG_MAC_SET,
    EN_NDM_EQT_MSG_MAC_QUERY,
    EN_NDM_EQT_MSG_ZERO_CROSS,
    
    EN_NDM_EQT_MSG_PLC_TEST,        // 5
    EN_NDM_EQT_MSG_SSID_SET,
    EN_NDM_EQT_MSG_SSID_QUERY,
    EN_NDM_EQT_MSG_BQ_LOAD_FROM_SD,
    EN_NDM_EQT_MSG_BQ_LOAD_RESULT_QUERY,
    
    EN_NDM_EQT_MSG_KEY_TEST,        // 10
    EN_NDM_EQT_MSG_DISPLAY_TEST,
    EN_NDM_EQT_MSG_TEST_VER_QUERY,
    EN_NDM_EQT_MSG_TEST_INIT,
    EN_NDM_EQT_MSG_NTB_SYNC_SET,
    
    EN_NDM_EQT_MSG_NTB_SYNC_QUERY,  // 15
    EN_NDM_EQT_MSG_TF_CARD_TEST,
    EN_NDM_MSG_RTC_TIME,
    EN_NDM_MSG_DC_SD_HSO_QUERY_STAT,    // HSO2é?ˉSD?¨2éêy×′ì?
    EN_NDM_MSG_DC_SD_HSO_SET_CONFIG,    // HSOéè??SD?¨2éêy2?êy
    
    EN_NDM_MSG_DC_SD_HSO_QUERY_CONFIG,  // HSO2é?ˉSD?¨2éêy2?êy 20

}NDM_MSG_E;

HI_PRV HI_CONST HI_DIAG_CMD_REG_OBJ_S g_stNdmCmdDefault[] = 
{

#if defined(PRODUCT_CFG_PRODUCT_TYPE_NDM)
    {ID_DIAG_CMD_MDM_MODE_QUERY,        ID_DIAG_CMD_MDM_TF_CARD_TEST,       appNdmDiagCmdProc, HI_NULL},
    {ID_DIAG_CMD_NDM_RTC_TIME,          ID_DIAG_CMD_NDM_RTC_TIME,           appNdmDiagCmdProc, HI_NULL},
    {ID_DIAG_CMD_NDM_SD_DC_QUERY_STAT,  ID_DIAG_CMD_NDM_SD_DC_QUERY_CONFIG, appNdmDiagCmdProc, HI_NULL},
#endif
#if defined (PRODUCT_CFG_BQ_SUPPORT)
    {ID_DIAG_CMD_BATTERY_INFO, ID_DIAG_CMD_BATTERY_INFO, HI_BAT_ReportBatteryPercent, HI_NULL},
    {ID_DIAG_CMD_BATTERY_STAT, ID_DIAG_CMD_BATTERY_STAT, HI_BAT_ReportBatStat, HI_NULL},
#endif

};

HI_PRV MRS_NDM_DIAG_MSG_ID_TABLE_STRU g_stNdmDiagMsgIdTbl[] =
{
    {ID_DIAG_CMD_MDM_MODE_QUERY,            EN_NDM_EQT_MSG_MODE_QUERY},
    {ID_DIAG_CMD_MDM_MODE_SET,              EN_NDM_EQT_MSG_MODE_SET},
    {ID_DIAG_CMD_MDM_MAC_SET,               EN_NDM_EQT_MSG_MAC_SET},
    {ID_DIAG_CMD_MDM_MAC_QUERY,             EN_NDM_EQT_MSG_MAC_QUERY},
    {ID_DIAG_CMD_MDM_ZERO_CROSS_TEST,       EN_NDM_EQT_MSG_ZERO_CROSS},
    {ID_DIAG_CMD_MDM_PLC_TEST,              EN_NDM_EQT_MSG_PLC_TEST},
    {ID_DIAG_CMD_MDM_SSID_SET,              EN_NDM_EQT_MSG_SSID_SET},
    {ID_DIAG_CMD_MDM_SSID_QUERY,            EN_NDM_EQT_MSG_SSID_QUERY},
    {ID_DIAG_CMD_MDM_BQ_LOAD_FROM_SD,       EN_NDM_EQT_MSG_BQ_LOAD_FROM_SD},
    {ID_DIAG_CMD_MDM_BQ_LOAD_RESULT_QUERY,  EN_NDM_EQT_MSG_BQ_LOAD_RESULT_QUERY},
    {ID_DIAG_CMD_MDM_KEY_TEST,              EN_NDM_EQT_MSG_KEY_TEST},
    {ID_DIAG_CMD_MDM_DISPLAY,               EN_NDM_EQT_MSG_DISPLAY_TEST},
    {ID_DIAG_CMD_MDM_EQT_VERSION,           EN_NDM_EQT_MSG_TEST_VER_QUERY},
    {ID_DIAG_CMD_MDM_EQT_TEST_INIT,         EN_NDM_EQT_MSG_TEST_INIT},
    {ID_DIAG_CMD_MDM_NTB_SYNC_SET,          EN_NDM_EQT_MSG_NTB_SYNC_SET},
    {ID_DIAG_CMD_MDM_NTB_SYNC_QUERY,        EN_NDM_EQT_MSG_NTB_SYNC_QUERY},
    {ID_DIAG_CMD_MDM_TF_CARD_TEST,          EN_NDM_EQT_MSG_TF_CARD_TEST},
    {ID_DIAG_CMD_NDM_RTC_TIME,              EN_NDM_MSG_RTC_TIME},
    {ID_DIAG_CMD_NDM_SD_DC_QUERY_STAT,      EN_NDM_MSG_DC_SD_HSO_QUERY_STAT},
    {ID_DIAG_CMD_NDM_SD_DC_SET_CONFIG,      EN_NDM_MSG_DC_SD_HSO_SET_CONFIG},
    {ID_DIAG_CMD_NDM_SD_DC_QUERY_CONFIG,    EN_NDM_MSG_DC_SD_HSO_QUERY_CONFIG},
};


HI_PRV MRS_FW_MSG_PROC_TBL_STRU g_stNdmDiagMsgFuncTbl[] =
{
    {EN_NDM_EQT_MSG_MODE_QUERY,             EN_NDM_EQT_MSG_MODE_QUERY,              mrsNdmMsgModeQuery},
    {EN_NDM_EQT_MSG_MODE_SET,               EN_NDM_EQT_MSG_MODE_SET,                mrsNdmMsgModeSet},
    {EN_NDM_EQT_MSG_MAC_SET,                EN_NDM_EQT_MSG_MAC_SET,                 mrsNdmMsgMacSet},
    {EN_NDM_EQT_MSG_MAC_QUERY,              EN_NDM_EQT_MSG_MAC_QUERY,               mrsNdmMsgMacQuery},
    {EN_NDM_EQT_MSG_ZERO_CROSS,             EN_NDM_EQT_MSG_ZERO_CROSS,              mrsNdmMsgZeroCross},
    {EN_NDM_EQT_MSG_PLC_TEST,               EN_NDM_EQT_MSG_PLC_TEST,                mrsNdmMsgPlcTest},
    {EN_NDM_EQT_MSG_SSID_SET,               EN_NDM_EQT_MSG_SSID_SET,                mrsNdmMsgSsidSet},
    {EN_NDM_EQT_MSG_SSID_QUERY,             EN_NDM_EQT_MSG_SSID_QUERY,              mrsNdmMsgSsidQuery},
    {EN_NDM_EQT_MSG_BQ_LOAD_FROM_SD,        EN_NDM_EQT_MSG_BQ_LOAD_FROM_SD,         mrsNdmMsgBqLoad},
    {EN_NDM_EQT_MSG_BQ_LOAD_RESULT_QUERY,   EN_NDM_EQT_MSG_BQ_LOAD_RESULT_QUERY,    mrsNdmMsgBqLoadResultQuery},
    {EN_NDM_EQT_MSG_KEY_TEST,               EN_NDM_EQT_MSG_KEY_TEST,                mrsNdmMsgKeyTest},
    {EN_NDM_EQT_MSG_DISPLAY_TEST,           EN_NDM_EQT_MSG_DISPLAY_TEST,            mrsNdmMsgDisplayTest},
    {EN_NDM_EQT_MSG_TEST_VER_QUERY,         EN_NDM_EQT_MSG_TEST_VER_QUERY,          mrsNdmMsgTestVerQuery},
    {EN_NDM_EQT_MSG_TEST_INIT,              EN_NDM_EQT_MSG_TEST_INIT,               mrsNdmMsgTestInit},
    {EN_NDM_EQT_MSG_NTB_SYNC_SET,           EN_NDM_EQT_MSG_NTB_SYNC_SET,            mrsNdmMsgNtbSyncSet},
    {EN_NDM_EQT_MSG_NTB_SYNC_QUERY,         EN_NDM_EQT_MSG_NTB_SYNC_QUERY,          mrsNdmMsgNtbSyncQuery},
    {EN_NDM_EQT_MSG_TF_CARD_TEST,           EN_NDM_EQT_MSG_TF_CARD_TEST,            mrsNdmMsgTfCardTest},
    {EN_NDM_MSG_RTC_TIME,                   EN_NDM_MSG_RTC_TIME,                    mrsNdmMsgRtcTime},//工装
    
    {EN_NDM_MSG_DC_SD_HSO_QUERY_STAT,       EN_NDM_MSG_DC_SD_HSO_QUERY_STAT,        mrsNdmMsgDcSdReportStat},
    {EN_NDM_MSG_DC_SD_HSO_SET_CONFIG,       EN_NDM_MSG_DC_SD_HSO_SET_CONFIG,        mrsNdmMsgDcSdSetConfig},
    {EN_NDM_MSG_DC_SD_HSO_QUERY_CONFIG,     EN_NDM_MSG_DC_SD_HSO_QUERY_CONFIG,      mrsNdmMsgDcSdQueryConfig},
};

/**
 @brief     NDM diag command process.
 @attention \n
N/A
 @param[in] usId  Diag command ID.
 @param[in] pCmdParam  Diag command pointer.
 @param[in] usCmdParamSize  Diag command size.
 @param[in] ulOption  Diag command option.
 @retval ::HI_ERR_SUCCESS success
 @retval ::others failure
 @see \n
N/A
 */
HI_U32 appNdmDiagCmdProc(HI_U16 usId, HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U16 usMsgId = 0;
    HI_PVOID pData = HI_NULL;

    if (HI_ERR_SUCCESS != appNdmFindMsgIdByDiagId(usId, &usMsgId))
    {
        return HI_ERR_INVALID_ID;
    }

    if (pCmdParam && usCmdParamSize)
    {
        pData = (HI_PVOID)mrsToolsMalloc(usCmdParamSize);
        if (!pData)
        {
            return HI_ERR_NOT_ENOUGH_MEMORY;
        }

        (hi_void)memcpy_s(pData, usCmdParamSize,pCmdParam, usCmdParamSize);
    }

        
    ulRet = APP_NdmMessageSend(MRS_NDM_MSG_Q_0,EN_MRS_FW_MSG_NDM_DIAG, (HI_U32)usMsgId,
        (uintptr_t)pData, (HI_U32)ulOption, (HI_U32)usCmdParamSize);
    if (HI_ERR_SUCCESS != ulRet)
    {
        mrsToolsFree(pData);
        return HI_ERR_FAILURE;
    }

    return HI_ERR_SUCCESS;
}


HI_U32 appNdmFindMsgIdByDiagId(HI_U16 usDiagId, HI_U16 *pusMsgId)
{
    HI_U32 i = 0;

    if (!pusMsgId)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    for (i = 0; i < MRS_TOOLS_ALEN(g_stNdmDiagMsgIdTbl); i++)
    {
        if (usDiagId == g_stNdmDiagMsgIdTbl[i].usDiagId)
        {
            *pusMsgId = g_stNdmDiagMsgIdTbl[i].usMsgId;
            return HI_ERR_SUCCESS;
        }
    }

    return HI_ERR_NOT_FOUND;
}


/**
 @brief     NDM eqt message process.
 @attention \n
N/A
 @param[in] pstMsg  Eqt message pointer.
 @retval ::HI_ERR_SUCCESS success
 @retval ::others failure
 @see \n
N/A
 */
HI_U32 APP_NdmDiagMsgProc(HI_IN HI_SYS_QUEUE_MSG_S* pstMsg)
{
    HI_U32 ulRet = HI_ERR_NOT_FOUND;

    if (pstMsg)
    {
        HI_U32 i = 0;
        HI_U16 usMsgId = 0;

        if (!pstMsg->ulParam[1] && pstMsg->ulParam[3])
        {
            return HI_ERR_INVALID_PARAMETER;
        }

        usMsgId = (HI_U16)pstMsg->ulParam[0];
        for (i = 0; i < MRS_TOOLS_ALEN(g_stNdmDiagMsgFuncTbl); i++)
        {
            if ((usMsgId >= g_stNdmDiagMsgFuncTbl[i].usMinId) && (usMsgId <= g_stNdmDiagMsgFuncTbl[i].usMaxId))
            {
                ulRet = g_stNdmDiagMsgFuncTbl[i].pfnMsg((HI_U16)pstMsg->ulParam[0], (HI_PVOID)(&pstMsg->ulParam[1]));
                break;
            }
        }

        mrsToolsFree(pstMsg->ulParam[1]);
    }

    return ulRet;
}

HI_U32 APP_NdmDiagInit(HI_VOID)
{
    return HI_MDM_RegisterCommand(g_stNdmCmdDefault,HI_ARRAY_COUNT(g_stNdmCmdDefault),HI_DIAG_CMD_INSTANCE_DEFAULT);
}

