#include <hi_driver.h>
#include <hi_mdm.h>
#include <hi_mdm_cpu_stat.h>
#include <hi_mdm_event.h>
#include <hi_mdm_isr.h>
#include <hi_mdm_mem.h>
#include <hi_mdm_msg.h>
#include <hi_mdm_sem.h>
#include <hi_mdm_sys.h>
#include <hi_mdm_task.h>
#include <hi_mdm_timer.h>
#include <hi_mem.h>
#include <hi_stdlib.h>


#include "hi_mdm_mac.h"
#include "fast_send_policy.h"

#include "demo_dfx.h"

#define PRODUCT_CFG_SUPPORT_OSM
#define PRODUCT_CFG_SUPPORT_OSSM

typedef struct
{
    HI_U32 ulArgc;          // 参数个数
    HI_CHAR *pchArgv[10];   // 参数列表
    HI_CHAR *pchOutBuf;     // 应答buf指针
} MRS_SHELL_CMD_PRE_STRU;

#define MRS_MEM_VALID_ADDR(x)   (HI_BOOL)((x) < 0x80000000)
#define MEM_READ_MAX_NUM    (0x32)
#define BUFFER_OUTLINE_SIZE (1024)

HI_CONST HI_CHAR MRS_STR_IND_NOT_ENOUGH_MEMORY[] = "not enough memory.\n";
HI_CONST HI_CHAR MRS_STR_IND_PARAM_WRONG[] = "para num wrong.\n";
HI_CONST HI_CHAR MRS_STR_IND_INVALID_ADDR[] = "invalid addr.\n";
HI_CONST HI_CHAR MRS_STR_IND_LEN_TOO_LARGE[] = "len can't > 0x32.\n";
HI_CONST HI_CHAR MRS_STR_IND_LEN_IS_ZERO[] = "len can't = 0.\n";
HI_CONST HI_CHAR MRS_STR_IND_SUCCESS[] = "success.\n";

HI_U32 mrsDfxStrToArg(HI_CHAR *pStr, HI_U16 usMaxLen, HI_U32 *pArgc, HI_CHAR *pArgv[], HI_U32 ulMaxArgc);

MEMO_DFX_CMD_CB_S g_demo_dfx_cb = {0};

HI_VOID HI_TIMER_DEFINE(DemoDfxFastSendTest)
{
    #if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    demo_fast_send_policy_test();
    #endif
}

HI_VOID DemoDfxFastSendPeriodPolicyStart(HI_VOID)
{
    demo_fast_send_cfg_sc();
    HI_MDM_TIMER_Stop(&(g_demo_dfx_cb.dfx_timer));
    HI_MDM_TIMER_Start(&(g_demo_dfx_cb.dfx_timer), (TIMER_PROC_FUNC)DemoDfxFastSendTest, 1000, HI_TIMER_TYPE_PERIOD, 0);
}

HI_VOID DemoDfxFastSendPeriodPolicySTOP(HI_VOID)
{
    HI_MDM_TIMER_Stop(&(g_demo_dfx_cb.dfx_timer));
}

HI_VOID DemoDfxFastSendSinglePolicy(HI_VOID)
{
    #if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    demo_fast_send_policy_test();
    #endif
}

#if (defined(PRODUCT_CFG_SUPPORT_OSM) || defined(PRODUCT_CFG_SUPPORT_OSSM))
MRS_SHELL_CMD_PRE_STRU *mrsDfxShellCmdPrepare(HI_PVOID pCmdParam, HI_U32 ulCmdParamSize, HI_U32 ulOutSize)
{
    MRS_SHELL_CMD_PRE_STRU *pstShellCmd = HI_NULL;
    HI_CHAR *pchOutBuf = HI_NULL;

    pstShellCmd = (MRS_SHELL_CMD_PRE_STRU *)HI_MDM_Malloc(1, sizeof(MRS_SHELL_CMD_PRE_STRU));
    if (HI_NULL == pstShellCmd)
    {
        return HI_NULL;
    }

    (hi_void)memset_s(pstShellCmd, sizeof(MRS_SHELL_CMD_PRE_STRU), 0, sizeof(MRS_SHELL_CMD_PRE_STRU));

    if (ulOutSize > 0)
    {
        pchOutBuf = (HI_CHAR *)HI_MDM_Malloc(1,ulOutSize);
        if (HI_NULL == pchOutBuf)
        {
            HI_MDM_Free(1,pstShellCmd);
            return HI_NULL;
        }

        (hi_void)memset_s(pchOutBuf, ulOutSize, 0,ulOutSize);
    }

    pstShellCmd->pchOutBuf = pchOutBuf;

    if (HI_ERR_SUCCESS != mrsDfxStrToArg((HI_CHAR *)pCmdParam, (HI_U16)ulCmdParamSize, (HI_U32 *)&pstShellCmd->ulArgc, pstShellCmd->pchArgv, 10))
    {
        HI_MDM_Free(1,pchOutBuf);
        HI_MDM_Free(1,pstShellCmd);
    }

    return pstShellCmd;
}

HI_VOID mrsDfxShellCmdRestore(MRS_SHELL_CMD_PRE_STRU **pstShellCmd)
{
    if (HI_NULL != *pstShellCmd)
    {
        HI_MDM_Free(1,(*pstShellCmd)->pchOutBuf);
        HI_MDM_Free(1,(*pstShellCmd));
    }
}

HI_U32 mrsDfxStrToArg(HI_CHAR *pStr, HI_U16 usMaxLen, HI_U32 *pArgc, HI_CHAR *pArgv[], HI_U32 ulMaxArgc)
{
    HI_U16 i = 0;
    HI_BOOL bPreBlank = HI_TRUE;
    HI_U32 ulArgc = 0;


    if (0 == ulMaxArgc)
    {
        *pArgc = 0;
        return HI_ERR_SUCCESS;
    }

    // 确保字符串有结束符
    pStr[usMaxLen - 1] = '\0';

    for (i = 0; i < usMaxLen; i++)
    {
        // 命令结束
        if (pStr[i] == '\r' || pStr[i] == '\n' || pStr[i] == '\0')
        {
            pStr[i] = '\0';
            break;
        }

        if (pStr[i] == ' ' || pStr[i] == '\t')
        {
            pStr[i] = '\0';
            bPreBlank = HI_TRUE;

            if (ulArgc >= ulMaxArgc)
            {
                break;
            }
        }
        else
        {
            // 词首
            if (bPreBlank)
            {
                if (ulArgc < ulMaxArgc)
                {
                    pArgv[ulArgc] = &(pStr[i]);
                    ulArgc++;
                }
            }

            bPreBlank = HI_FALSE;
        }
    }

    *pArgc = ulArgc;

    return HI_ERR_SUCCESS;
}
#endif


#if defined(PRODUCT_CFG_SUPPORT_OSM)
HI_U32 mrsDfxOsmPrv(HI_U16 usId, HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption)
{
    MRS_SHELL_CMD_PRE_STRU *pstShellCmd = HI_NULL;
    HI_CHAR *pchReportBuf = HI_NULL;
    HI_U32 ulReportBufSize = BUFFER_OUTLINE_SIZE;
    HI_CHAR chTempStr[20] = {0};

    HI_CHAR ** ppArgv = HI_NULL;
    HI_U32 ulArgc = 0;

    HI_U32 ulMemAddr = 0;
    HI_U32 ulMemSize = 0;

    pstShellCmd = mrsDfxShellCmdPrepare(pCmdParam, usCmdParamSize, ulReportBufSize);
    if (HI_NULL == pstShellCmd)
    {
        HI_MDM_ReportPacket(usId, (HI_U8)ulOption, (HI_PBYTE)MRS_STR_IND_NOT_ENOUGH_MEMORY, (HI_U16)strlen(MRS_STR_IND_NOT_ENOUGH_MEMORY)+1, HI_TRUE);
        return HI_ERR_SUCCESS;
    }

    ulArgc = pstShellCmd->ulArgc;
    ppArgv = pstShellCmd->pchArgv;
    pchReportBuf = pstShellCmd->pchOutBuf;

    do
    {
        if (ulArgc != 2)
        {
            HI_MDM_ReportPacket(usId, (HI_U8)ulOption, (HI_PBYTE)MRS_STR_IND_PARAM_WRONG, (HI_U16)strlen(MRS_STR_IND_PARAM_WRONG)+1, HI_TRUE);
            break;
        }

        ulMemAddr = strtoul(ppArgv[0], (HI_CHAR **)HI_NULL, 16);
        ulMemSize = strtoul(ppArgv[1], (HI_CHAR **)HI_NULL, 16);

        if (ulMemSize > MEM_READ_MAX_NUM)
        {
            HI_MDM_ReportPacket(usId, (HI_U8)ulOption, (HI_PBYTE)MRS_STR_IND_LEN_TOO_LARGE, (HI_U16)strlen(MRS_STR_IND_LEN_TOO_LARGE)+1, HI_TRUE);
            break;
        }
        else if (ulMemSize == 0)
        {
            HI_MDM_ReportPacket(usId, (HI_U8)ulOption, (HI_PBYTE)MRS_STR_IND_LEN_IS_ZERO, (HI_U16)strlen(MRS_STR_IND_LEN_IS_ZERO)+1, HI_TRUE);
            break;
        }

        if (0 != (ulMemAddr & 0x3))
        {
            HI_MDM_ReportPacket(usId, (HI_U8)ulOption, (HI_PBYTE)MRS_STR_IND_INVALID_ADDR, (HI_U16)strlen(MRS_STR_IND_INVALID_ADDR)+1, HI_TRUE);
            break;
        }

        (hi_void)memcpy_s(pchReportBuf, ulReportBufSize, "\r\n", sizeof("\r\n"));

        while (ulMemSize > 0)
        {
            HI_U32 ulLineLen = 0;
            HI_U32 i = 0;
            HI_U32 j = 0;
            HI_U32 ulValue = 0;

            (hi_void)sprintf_s(chTempStr, sizeof(chTempStr), "0x%08x  ", ulMemAddr);
            (hi_void)strcat_s(pchReportBuf, ulReportBufSize, chTempStr);

            ulLineLen = (ulMemSize >= 4) ? 4 : ulMemSize;

            for (i = 0; i < ulLineLen; i++)
            {
                if ((!MRS_MEM_VALID_ADDR(ulMemAddr)) || (HI_MDM_PHY_IsAccessPhyMem(ulMemAddr) == HI_FALSE))
                {
                    HI_MDM_ReportPacket(usId, (HI_U8)ulOption, (HI_PBYTE)MRS_STR_IND_INVALID_ADDR, (HI_U16)strlen(MRS_STR_IND_INVALID_ADDR)+1, HI_TRUE);
                    mrsDfxShellCmdRestore(&pstShellCmd);
                    return HI_ERR_SUCCESS;
                }

                (hi_void)strcat_s(pchReportBuf, ulReportBufSize, "0x");
                ulValue = *(HI_U32 *)ulMemAddr;

                j = 7;
                while (j > 0)
                {
                    if (ulValue & (0xF << (4 * j)))
                    {
                        break;
                    }

                    (hi_void)strcat_s(pchReportBuf, ulReportBufSize, "0");
                    j--;
                }

                (hi_void)sprintf_s(chTempStr, sizeof(chTempStr), "%x  ", ulValue);
                (hi_void)strcat_s(pchReportBuf, ulReportBufSize, chTempStr);

                ulMemAddr += 4;
            }

            ulMemSize -= ulLineLen;
            (hi_void)strcat_s(pchReportBuf, ulReportBufSize, "\r\n");
        }

        HI_MDM_ReportPacket(usId, (HI_U8)ulOption, (HI_PBYTE)pchReportBuf, (HI_U16)strlen(pchReportBuf), HI_TRUE);
    } while (0);

    mrsDfxShellCmdRestore(&pstShellCmd);

    return HI_ERR_SUCCESS;
}

#endif


#if defined(PRODUCT_CFG_SUPPORT_OSSM)
HI_U32 mrsDfxOssmPrv(HI_U16 usId, HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption)
{
    MRS_SHELL_CMD_PRE_STRU *pstShellCmd = HI_NULL;

    HI_CHAR ** ppArgv = HI_NULL;
    HI_U32 ulArgc = 0;

    HI_U32 ulMemAddr = 0;
    HI_U32 ulValue = 0;

    pstShellCmd = mrsDfxShellCmdPrepare(pCmdParam, usCmdParamSize, 0);
    if (HI_NULL == pstShellCmd)
    {
        HI_MDM_ReportPacket(usId, (HI_U8)ulOption, (HI_PBYTE)MRS_STR_IND_NOT_ENOUGH_MEMORY, (HI_U16)strlen(MRS_STR_IND_NOT_ENOUGH_MEMORY)+1, HI_TRUE);
        return HI_ERR_SUCCESS;
    }

    ulArgc = pstShellCmd->ulArgc;
    ppArgv = pstShellCmd->pchArgv;

    do
    {
        if (ulArgc != 2)
        {
            HI_MDM_ReportPacket(usId, (HI_U8)ulOption, (HI_PBYTE)MRS_STR_IND_PARAM_WRONG, (HI_U16)strlen(MRS_STR_IND_PARAM_WRONG)+1, HI_TRUE);
            break;
        }

        ulMemAddr = strtoul(ppArgv[0], (HI_CHAR **)HI_NULL, 16);
        ulValue = strtoul(ppArgv[1], (HI_CHAR **)HI_NULL, 16);

        if ((0 != (ulMemAddr & 0x3)) || (!MRS_MEM_VALID_ADDR(ulMemAddr)) || (HI_MDM_PHY_IsAccessPhyMem(ulMemAddr) == HI_FALSE))
        {
            HI_MDM_ReportPacket(usId, (HI_U8)ulOption, (HI_PBYTE)MRS_STR_IND_INVALID_ADDR, (HI_U16)strlen(MRS_STR_IND_INVALID_ADDR)+1, HI_TRUE);
            break;
        }

        *(volatile HI_U32 *)ulMemAddr = ulValue;

        HI_MDM_ReportPacket(usId, (HI_U8)ulOption, (HI_PBYTE)MRS_STR_IND_SUCCESS, (HI_U16)strlen(MRS_STR_IND_SUCCESS) + 1, HI_TRUE);
    } while (0);

    mrsDfxShellCmdRestore(&pstShellCmd);

    return HI_ERR_SUCCESS;
}
#endif

HI_U32 mrsDfxOsm(HI_U16 usId, HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;

#if defined(PRODUCT_CFG_SUPPORT_OSM)
    ulRet = mrsDfxOsmPrv(usId, pCmdParam, usCmdParamSize, ulOption);
#endif

    return ulRet;
}

HI_U32 mrsDfxOssm(HI_U16 usId, HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;

#if defined(PRODUCT_CFG_SUPPORT_OSSM)
    ulRet = mrsDfxOssmPrv(usId, pCmdParam, usCmdParamSize, ulOption);
#endif

    return ulRet;
}
