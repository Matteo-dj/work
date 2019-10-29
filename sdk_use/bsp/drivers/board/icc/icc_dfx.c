#include <hi_types.h>
#if defined (PORDUCT_CFG_SUPPORT_ICC)
#include <hi_mdm_types.h>
#include <hi_sal.h>
#include <hi_icc.h>
#include <dfx_icc.h>
HI_U32 HI_ICC_ReportStat(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption)
{
    HI_DIAG_GET_ICC_STAT_CMD_S *pCmd =pCmdParam;
    HI_DIAG_GET_ICC_STAT_IND_S stInd = {{0}};

    if(pCmd->bRemote)//ÔÝ²»Ö§³Ö
    {
        HI_DIAG_ReportPacket(usId,(HI_U8)ulOption,(HI_PBYTE)&stInd,sizeof(HI_DIAG_GET_ICC_STAT_IND_S),HI_TRUE);
    }
    else
    {
        
        HI_ICC_GetDfxInfo(&(stInd.iccStat));
        HI_DIAG_ReportPacket(usId,(HI_U8)ulOption,(HI_PBYTE)&stInd,sizeof(HI_DIAG_GET_ICC_STAT_IND_S),HI_TRUE);
    }
    return HI_ERR_SUCCESS;
}

#endif
