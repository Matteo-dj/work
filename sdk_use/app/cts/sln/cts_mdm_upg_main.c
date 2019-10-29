#include "app_common.h"
#include "dfx_app.h"
#include "cts_mdm_upg_main.h"
//*****************************************************************************
#define CTS_LOAD_FILE_BLOCK_SIZE        HI_MDM_LD_FILE_BLOCK_SIZE
#define CTS_LOAD_FILE_UPG_BUF_SIZE      (4 * 1024)
#define CTS_LOAD_FILE_UPG_PROC_ID       HI_LOAD_HUPG_FILE_SAL_PROC_ID

//释放内存
#define upgCtsFreeMem(p)\
            do{\
                if(p!=HI_NULL)\
                {\
                    HI_MDM_Free(HI_MOD_ID_APP_UPG,p);\
                    p=HI_NULL;\
                    CTS_PRINT0("upgCtsFreeMem pFile.");\
                }\
            }while(0)  
         
#if defined(PRODUCT_CFG_FEATURE_LOAD_FILE)
HI_U32 LoadUpgFile(HI_U32 ulOption, HI_U32 ulId, HI_PVOID pFile, HI_U32 ulFileSize, HI_U32 ulErrCode);
#endif
//HI_VOID upgCtsSetLdFileFinishFlag(HI_BOOL flag);
/* BEGIN:PN:SDK UPG测试框架  MODIFY\ADD  by chenli/00238131 at 2014/6/3 */
//drv HI_DIAG_RegisterLoadFileNoitfyProc接口对外开发后，需删除
HI_EAPI HI_U32 HI_DIAG_RegisterLoadFileNoitfyProc(HI_U32 ulLoadId, HI_LD_FILE_CALLBACK_F pfnLoadFileNotify, HI_PVOID pFileBuffer, HI_U32 ulFileBufferSize);
/* END:PN:SDK UPG测试框架  MODIFY\ADD  by chenli/00238131 at 2014/6/3*/

HI_PRV HI_BOOL g_loadFileFinishFlag             = HI_FALSE;
HI_PRV CTS_UPG_OBTAIN_MEM_CTX g_ctsObtainMemCtx = {0,0,0,HI_NULL};

HI_PRV HI_U32 APP_MdmUpgMsgRcv(HI_U32 ulMdmMsgId, HI_PVOID pMdmMsg,HI_PVOID pNotUsed, HI_U32 usOption)
{

    HI_MDM_MSG_APPRCV_S* pstMsgRcv = (HI_MDM_MSG_APPRCV_S*)pMdmMsg;
    HI_UNREF_PARAM(pNotUsed);

    if ((pstMsgRcv == HI_NULL) || (pstMsgRcv->pucPacket == HI_NULL))
    {
        CTS_PRINT0("APP_MdmUpgMsgRcv err parameter.");
        return HI_ERR_INVALID_PARAMETER;
    }

    switch (ulMdmMsgId)
    {
        case HI_MDM_UPG_STATUS:
            {
                HI_MDM_UPG_STATUS_IND_S *pstInd        = (HI_MDM_UPG_STATUS_IND_S*)pstMsgRcv->pucPacket;
                DIAG_CMD_UPG_DFX_STATUS_IND_STRU stInd = {0};

                HI_MDM_Memcpy(&stInd, pstInd, 
                    (HI_S32)(sizeof(HI_MDM_UPG_STATUS_IND_S) + pstInd->stUpgStatus.ulNum * sizeof(HI_UPG_STATUS_IND_ENTRY_S)));
                HI_MDM_ReportPacket(ID_DIAG_CMD_MDM_UPG_STATUS, 
                    (HI_U8)HI_DIAG_CMD_INSTANCE_DEFAULT, (HI_PBYTE)&stInd, sizeof(DIAG_CMD_UPG_DFX_STATUS_IND_STRU), HI_TRUE);
                CTS_PRINT2("APP_MdmUpgMsgRcv:ulMdmMsgId %x, ulNum %d", ulMdmMsgId, pstInd->stUpgStatus.ulNum);
                break;
            }
        case HI_MDM_UPG_STAT_INFO_IND:
            {
                HI_MDM_UPG_STAT_INFO_IND_S *pstInd = (HI_MDM_UPG_STAT_INFO_IND_S*)pstMsgRcv->pucPacket;
                                
                HI_MDM_ReportPacket(ID_DIAG_CMD_MDM_UPG_STATUS_STAT, (HI_U8)HI_DIAG_CMD_INSTANCE_DEFAULT,
                    (HI_PBYTE)pstInd, sizeof(HI_MDM_UPG_STAT_INFO_IND_S), HI_TRUE);
                CTS_PRINT1("APP_MdmUpgMsgRcv:ulMdmMsgId %x", ulMdmMsgId); 
                break;
            }
        case HI_MDM_UPG_STATUS_BS_IND:
            {
                HI_MDM_UPG_STATUS_BS_IND_S *pstInd = (HI_MDM_UPG_STATUS_BS_IND_S*)pstMsgRcv->pucPacket;
                
                HI_MDM_ReportPacket(ID_DIAG_CMD_MDM_UPG_STATUS_BS, (HI_U8)HI_DIAG_CMD_INSTANCE_DEFAULT,
                    (HI_PBYTE)pstInd, sizeof(HI_MDM_UPG_STATUS_BS_IND_S), HI_TRUE);
                CTS_PRINT1("APP_MdmUpgMsgRcv:ulMdmMsgId %x", ulMdmMsgId);                 
                break;
            }
        case HI_MDM_UPG_STOP:
            {
                HI_MDM_UPG_STOP_IND_S *pstInd = (HI_MDM_UPG_STOP_IND_S*)pstMsgRcv->pucPacket;
                                
                HI_MDM_ReportPacket(ID_DIAG_CMD_MDM_UPG_STOP, (HI_U8)HI_DIAG_CMD_INSTANCE_DEFAULT,
                    (HI_PBYTE)pstInd, sizeof(HI_MDM_UPG_STOP_IND_S), HI_TRUE);
                CTS_PRINT1("APP_MdmUpgMsgRcv:ulMdmMsgId %x", ulMdmMsgId); 
                break;
            }
        default:
            break;
    }
     
    return HI_ERR_SUCCESS;
}

// TODO: 用宏替换(已修改)
HI_PRV HI_CONST HI_MDM_APPOBJ_S g_stMdmUpgObj = 
{
    HI_MDM_OBJ_ID_USR1, // APP 对象ID
    APP_MdmUpgMsgRcv   // APP接收MDM消息的处理入口 
};

//HI_DIAG_LD_FILE_OPT_START:分配内存 注册ID
//HI_DIAG_LD_FILE_OPT_END或异常退出:注销ID,释放内存
#if defined(PRODUCT_CFG_FEATURE_LOAD_FILE)
HI_U32 LoadUpgFile(HI_U32 ulOption, HI_U32 ulId, HI_PVOID pFile, HI_U32 ulFileSize, HI_U32 ulErrCode)
{
    HI_U32 ret   = HI_ERR_SUCCESS;
    HI_U16 usLds = HI_LOU16(ulOption); // ld file status
    CTS_UPG_OBTAIN_MEM_CTX * ctsObtainMemCtx = upgGetMemObtainCtx();
    
    ulOption = HI_HIU16(ulOption);

    if (ctsObtainMemCtx == HI_NULL)
    {
        CTS_PRINT0("LoadUpgFile err parameter.");
        return HI_ERR_INVALID_PARAMETER;
    }
    
    if((usLds == HI_DIAG_LD_FILE_OPT_START) && (ulErrCode == HI_ERR_NOT_ENOUGH_MEMORY))
    {
        HI_MDM_UPG_START_REQ_S * pstReq = HI_NULL;
        upgCtsFreeMem(pFile);

        //如果没有释放上次分配的升级缓存，则不启动新的文件接收流程
        if (ctsObtainMemCtx->pUpgObtainBuf != HI_NULL)
        {
            HI_DIAG_RegisterLoadFileNoitfyProc(CTS_LOAD_FILE_UPG_PROC_ID, LoadUpgFile, HI_NULL, 0);
            CTS_PRINT0("LoadUpgFile:HI_DIAG_LD_FILE_OPT_START not release last malloc mem.");
            
            return HI_ERR_NOT_ENOUGH_MEMORY;
        }

        //获取接收文件内存
        pFile = HI_MDM_Malloc(HI_MOD_ID_APP_UPG, CTS_LOAD_FILE_UPG_BUF_SIZE);

        if (pFile == HI_NULL)
        {
            HI_DIAG_RegisterLoadFileNoitfyProc(CTS_LOAD_FILE_UPG_PROC_ID, LoadUpgFile, HI_NULL, 0);
            CTS_PRINT0("LoadUpgFile:HI_DIAG_LD_FILE_OPT_START Malloc pFile fail.");
            return HI_ERR_MALLOC_FAILUE;
        }
        CTS_PRINT0("LoadUpgFile:HI_DIAG_LD_FILE_OPT_START MALLOC pFile success.");
        
        //获取升级缓存
        ret = HI_MDM_UPG_ObtainMem(HI_MDM_OBJ_ID_USR1, ulFileSize, &pstReq, 0);
                
        //存储获取的缓存上下文信息
        if (pstReq != HI_NULL)
        {
            //给上下文赋予新的值
            ctsObtainMemCtx->pUpgObtainBuf    = (HI_PVOID)pstReq;
            ctsObtainMemCtx->ulUpgId          = HI_MDM_OBJ_ID_USR1;
            ctsObtainMemCtx->ulObtainFileSize = ulFileSize;
            ctsObtainMemCtx->ulRealFileSize   = 0;
            CTS_PRINT3("LoadUpgFile HI_MDM_UPG_ObtainMem success: pUpgObtainBuf %x,ulUpgId %d,ulObtainFileSize %d ", 
                (HI_U32)ctsObtainMemCtx->pUpgObtainBuf, ctsObtainMemCtx->ulUpgId, ctsObtainMemCtx->ulObtainFileSize);            
        }
        else
        {
            CTS_PRINT3("LoadUpgFile HI_MDM_UPG_ObtainMem fail: pUpgObtainBuf %x,ulUpgId %d,ulObtainFileSize %d ", 
                (HI_U32)ctsObtainMemCtx->pUpgObtainBuf, ctsObtainMemCtx->ulUpgId, ctsObtainMemCtx->ulObtainFileSize); 
            upgCtsFreeMem(pFile);
            HI_DIAG_RegisterLoadFileNoitfyProc(CTS_LOAD_FILE_UPG_PROC_ID, LoadUpgFile, HI_NULL, 0);
            return HI_ERR_MALLOC_FAILUE;

        }

        HI_DIAG_RegisterLoadFileNoitfyProc(HI_MAKEU32(CTS_LOAD_FILE_UPG_PROC_ID, HI_DIAG_LD_FILE_OPT_TIMEOUT|HI_DIAG_LD_FILE_OPT_BLK), LoadUpgFile,\
            ((HI_U8*)pFile), CTS_LOAD_FILE_UPG_BUF_SIZE);
        upgCtsSetLdFileFinishFlag(HI_FALSE);
    
    }
    else if( usLds == HI_DIAG_LD_FILE_OPT_END)
    {
        if(ulErrCode == HI_ERR_TIMEOUT)
        {
            CTS_PRINT0("LoadUpgFile:HI_DIAG_LD_FILE_OPT_END TIMEOUT.");
            return HI_ERR_SKIP;
        }
        else
        {
            // 文件传输结束        
            upgCtsFreeMem(pFile);            
            upgCtsSetLdFileFinishFlag(HI_TRUE);
            HI_DIAG_RegisterLoadFileNoitfyProc(CTS_LOAD_FILE_UPG_PROC_ID, LoadUpgFile, HI_NULL, 0);            
            CTS_PRINT0("LoadUpgFile end.");
        }
        CTS_PRINT2("LoadUpgFile:HI_DIAG_LD_FILE_OPT_END: ulObtainFileSize %d,ulRealFileSize %d ",
            ctsObtainMemCtx->ulObtainFileSize, ctsObtainMemCtx->ulRealFileSize);
    }
    else if (usLds == HI_DIAG_LD_FILE_OPT_BLK)
    {
        // 获取块
        HI_PVOID pBlk    = pFile;
        HI_U16 usBlkSize = (HI_U16)ulFileSize;
        
	    // copy 数据到本地存储 
	    // TODO: 使用HSO的宏CTS_LOAD_FILE_BLOCK_SIZE(已修改)
	    if ((((CTS_LOAD_FILE_BLOCK_SIZE * ulId) + usBlkSize) <= ctsObtainMemCtx->ulObtainFileSize)
                &&(pBlk != HI_NULL)
                &&(usBlkSize != 0)
                &&(ctsObtainMemCtx->pUpgObtainBuf != HI_NULL))
	    {
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) || defined(PRODUCT_CFG_PRODUCT_TYPE_NDM)	
            HI_U32 fileBufHead = (HI_U32)ctsObtainMemCtx->pUpgObtainBuf + sizeof(HI_MDM_UPG_START_REQ_S);
            HI_MDM_Memcpy((HI_U8 *)(fileBufHead + (CTS_LOAD_FILE_BLOCK_SIZE * ulId)), (HI_U8 *)pBlk, usBlkSize);
#else
            ret = HI_MDM_UPG_MemWrite((CTS_LOAD_FILE_BLOCK_SIZE * ulId), pBlk, usBlkSize);
#endif
            //文件传输块号从0开始
            if ((CTS_LOAD_FILE_BLOCK_SIZE * (ulId + 1)) > (ctsObtainMemCtx->ulObtainFileSize))
            {
                ctsObtainMemCtx->ulRealFileSize = (CTS_LOAD_FILE_BLOCK_SIZE * ulId) + usBlkSize;
            }
            else
            {
                ctsObtainMemCtx->ulRealFileSize = CTS_LOAD_FILE_BLOCK_SIZE * (ulId + 1);
            }
	    }
            // TODO:增加异常分支处理流程，为空则退出(已修改)
        else
        {
            
            //释放文件缓存
            upgCtsFreeMem(pFile);
            
            //释放升级缓存
            HI_MDM_UPG_ReleaseMem(HI_MDM_OBJ_ID_USR1, ctsObtainMemCtx->pUpgObtainBuf, 0);
            (hi_void)memset_s(ctsObtainMemCtx, sizeof(CTS_UPG_OBTAIN_MEM_CTX), 0, sizeof(CTS_UPG_OBTAIN_MEM_CTX));

            //重新注册ID
            HI_DIAG_RegisterLoadFileNoitfyProc(CTS_LOAD_FILE_UPG_PROC_ID, LoadUpgFile, HI_NULL, 0);
            CTS_PRINT0("LoadUpgFile:HI_DIAG_LD_FILE_OPT_BLK err parameter.");
            return HI_ERR_PROTO_INVALID;
        }   
    }
    else if(ulErrCode == HI_ERR_TIMEOUT)
    {
        // skip this condition.
        CTS_PRINT0("LoadUpgFile:HI_ERR_TIMEOUT entry.");
        return HI_ERR_SKIP;
    }
    else if(ulErrCode)
    {
        upgCtsFreeMem(pFile);
        HI_MDM_UPG_ReleaseMem(HI_MDM_OBJ_ID_USR1, ctsObtainMemCtx->pUpgObtainBuf, 0);
        (hi_void)memset_s(ctsObtainMemCtx, sizeof(CTS_UPG_OBTAIN_MEM_CTX), 0, sizeof(CTS_UPG_OBTAIN_MEM_CTX));
        HI_DIAG_RegisterLoadFileNoitfyProc(CTS_LOAD_FILE_UPG_PROC_ID, LoadUpgFile, HI_NULL, 0);
        CTS_PRINT1("LoadUpgFile:ulErrCode %d, free mem.", ulErrCode);
    }
    
    return ret; 
}
#endif

CTS_UPG_OBTAIN_MEM_CTX * upgGetMemObtainCtx(HI_VOID)
{
    return &g_ctsObtainMemCtx;
}

HI_VOID cts_InitMdmUpgObj(HI_VOID)
{
    // APP对象初始化 
    HI_MDM_AppInit(&g_stMdmUpgObj);
 #if defined(PRODUCT_CFG_FEATURE_LOAD_FILE)
    HI_DIAG_RegisterLoadFileNoitfyProc(CTS_LOAD_FILE_UPG_PROC_ID, LoadUpgFile, HI_NULL, 0);
 #endif
}

//注意：CalcCheckSum2对参数错误不做特殊标记,将返回检验和0，
//      正确的校验和也有可能返回0!!!
HI_U16 upgCtsCalcCheckSum(HI_PBYTE ptr,HI_U16 len)
{
    HI_U16 CheckSum = 0;

    if ((ptr != HI_NULL) && (len > 0))  
    {
        HI_U16 i = 0;
        for (i = 0; i < len; i++)
        {
            CheckSum += ptr[i];
        }
    }
    
    return CheckSum;
}

HI_VOID upgCtsSetLdFileFinishFlag(HI_BOOL flag)
{
    HI_MDM_SEM_Wait(g_stCtsOsRes.aulSemId[EN_APP_SEM_UPG], HI_SYS_WAIT_FOREVER);
    g_loadFileFinishFlag = flag;
    HI_MDM_SEM_Signal(g_stCtsOsRes.aulSemId[EN_APP_SEM_UPG]);
    CTS_PRINT1("upgCtsSetLdFileFinishFlag: %d", flag);

    return;
}

HI_BOOL upgCtsGetLoadFileFinishFlag(HI_VOID)
{
    HI_BOOL flag;
    
    HI_MDM_SEM_Wait(g_stCtsOsRes.aulSemId[EN_APP_SEM_UPG], HI_SYS_WAIT_FOREVER);
    flag = g_loadFileFinishFlag;
    HI_MDM_SEM_Signal(g_stCtsOsRes.aulSemId[EN_APP_SEM_UPG]);
    CTS_PRINT1("upgCtsGetLoadFileFinishFlag: %d", flag);

    return flag;
}
//*****************************************************************************

