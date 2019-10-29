#include <hi_types.h>
#include "drv_dfx_cmn_inc.h"
#include "drv_dfx_sal.h"
#include <dfx_sal.h>//for DIAG_CMD_NEW_SYSERR_QRY_STU
#include <sal_diag.h>
#include <hi_3518.h>

// 从 DIAG 模块中移到 DRV中, 因为这些代码只有DRV使用 s00130424
HI_U32 new_diag_syserr_qry(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption)
{
    HI_U32 ret = HI_ERR_NOT_SUPPORT;

    DIAG_CMD_NEW_SYSERR_QRY_STU* pstAck = (DIAG_CMD_NEW_SYSERR_QRY_STU* )HI_MDM_Malloc(HI_MOD_ID_SAL_DFX, sizeof(DIAG_CMD_NEW_SYSERR_QRY_STU));
    if(HI_NULL == pstAck)
    {
        ret = HI_ERR_NOT_ENOUGH_MEMORY;
        return ret;
    }
    (hi_void)memset_s(pstAck, sizeof(DIAG_CMD_NEW_SYSERR_QRY_STU), 0, sizeof(DIAG_CMD_NEW_SYSERR_QRY_STU));

    diag_get_NewSysErrInfo(pstAck);

    ret = HI_DIAG_ReportPacket(usId, (HI_U8)ulOption, (HI_PBYTE)pstAck, (HI_U16)sizeof(DIAG_CMD_NEW_SYSERR_QRY_STU), HI_TRUE);

    HI_MDM_Free(HI_MOD_ID_SAL_DFX, pstAck);

    return ret;
}

HI_U32 newdiagSysObjQry(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption)
{
    HI_U32 ret = HI_ERR_SUCCESS;
#if defined(PRODUCT_CFG_SYS_QRY_STAT)
    HI_U32 ulObjSize = 0;
    HI_PVOID pObjInfo = dfxGetSysInfo(&ulObjSize);
    
    if(ulObjSize && pObjInfo)
    {
        ret = HI_DIAG_ReportPacket(usId, (HI_U8)ulOption, (HI_PBYTE)pObjInfo, (HI_U16)ulObjSize, HI_TRUE);
    }

#else
    ret = HI_ERR_NOT_SUPPORT;
#endif

    return ret;
}
HI_U32  dfx_system_rst(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption)
{

    /* BEGIN: Added by b00208046, 2013/6/6   PN:DTS2013053004702 */
    HI_DIAG_SendAckPacket(usId, "RESET NOW", (HI_U16)strlen("RESET NOW"),HI_FALSE);
    HI_MDM_Sleep(500);
    /* END:   Added by b00208046, 2013/6/6 */
    HI_SYS_Reboot(HI_SYS_REBOOT_CAUSE_D);
    return HI_ERR_SUCCESS;
}

#if defined (PRODUCT_CFG_SUPPORT_3518) 
HI_U32  dfx_3518_system_rst(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption)
{
    HI_U32 ret = HI_ERR_FAILURE;
    ret = HI_3518_Reboot();
	if(HI_ERR_SUCCESS != ret)
    	HI_DIAG_SendAckPacket(usId, "3518 RESET FAIL", (HI_U16)strlen("3518 RESET FAIL"),HI_FALSE);
	else
		HI_DIAG_SendAckPacket(usId, "3518 RESET NOW", (HI_U16)strlen("3518 RESET NOW"),HI_FALSE);
	return ret;
}

#endif

