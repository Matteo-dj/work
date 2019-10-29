//*****************************************************************************
//
//                  版权所有 (C), 1998-2011, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : main.c
//  版 本 号   : V1.0 
//  作    者   : shenhankun/00130424
//  生成日期   : 2011-10-25
//  功能描述   : CTS基础框架实现
//
//  函数列表   : 
//  修改历史   : 
//  1.日    期 : 2011-10-25
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


//*****************************************************************************
#include "app_common.h"
#include "app_cts_entry.h"
#include "dfx_sys.h"
#include "cts_mdm_upg_main.h"
//*****************************************************************************


//*****************************************************************************
// CTS 框架上下文定义
//*****************************************************************************
//HI_PRV CTS_CTRL_S g_stCtsCtrl;
//*****************************************************************************


//*****************************************************************************
// Loacal Interface
HI_PRV HI_U32 cts_ObjUsrInProc(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption);
//*****************************************************************************


//*****************************************************************************
// CTS测试命令表
//*****************************************************************************
HI_PRV HI_CONST HI_DIAG_CMD_REG_OBJ_S g_astCtsCmnObjTbl[] = {
    {HI_DBG_CTS_TEST, HI_DBG_CTS_TEST, cts_ObjUsrInProc},
};

HI_PRV int stricmp(const char *s1, const char *s2) {
  char f, l;

  do {
    f = ((*s1 <= 'Z') && (*s1 >= 'A')) ? *s1 + 'a' - 'A' : *s1;
    l = ((*s2 <= 'Z') && (*s2 >= 'A')) ? *s2 + 'a' - 'A' : *s2;
    s1++;
    s2++;
  } while ((f) && (f == l));

  return (int) (f - l);
}

//*****************************************************************************
// 处理HSO的命令 总入口
//*****************************************************************************
HI_PRV HI_U32 cts_ObjUsrInProc(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption)
{
    HI_CTS_REQ_S* pstReq = (HI_CTS_REQ_S*)pCmdParam;
    HI_U32 i = 0;
    HI_BOOL bFound = HI_FALSE;
    HI_CTS_OBJ_TBL* pstObj = HI_NULL;
    
    for (i = 0; i< HI_ARRAY_COUNT(g_stCtsObjTbl); i++)
    {
        pstObj = &g_stCtsObjTbl[i];

        if(stricmp(pstObj->pszName, pstReq->szName) == 0)
        {
            bFound = HI_TRUE;
            
            // find it 
            HI_U8* pData = (HI_U8*)HI_MDM_Malloc(HI_MOD_ID_USR, usCmdParamSize);
            HI_SYS_QUEUE_MSG_S stMsg = { 0 };
            if(pData)
            {
                (hi_void)memcpy_s(pData,usCmdParamSize,pCmdParam, usCmdParamSize);
                stMsg.ulMsgId = 1;
                stMsg.ulParam[0] = (HI_U32)pData;
                stMsg.ulParam[1] = usCmdParamSize;
                stMsg.ulParam[2] = (HI_U32)pstObj->pfnObjProc;
                if(HI_MDM_MSG_Send(g_stCtsOsRes.aulQueueId[EN_APP_QUEUE_CTS], &stMsg, 0))
                {
                    HI_MDM_Free(HI_MOD_ID_USR, (HI_PVOID)pData);
                    CTS_PRINT0("***error: fail to send message to CTS task.");
                }
            }
            else
            {
                CTS_PRINT0("***error: fail to malloc memory to send message to CTS task.");
                HI_MDM_Free(HI_MOD_ID_USR, (HI_PVOID)pData);
            }
            break;
        }
    }

    if(!bFound)
    {
        CTS_PRINT1("***error: fail to find obj name:%s", pstReq?pstReq->szName:"NA");
    }
    
    return HI_ERR_SUCCESS;
}

//*****************************************************************************
// 函数名称: CTS_MainTaskInit
// 功能描述: CTS Task 初始化
//           
// 参数说明:
//  This function has no arguments.
//
// 返 回 值: 
//    无
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: shenhankun/00130424 [2011-11-08]
//*****************************************************************************
HI_VOID CTS_MainTaskInit(HI_VOID)
{
    HI_MDM_RegisterCommand(HI_DIAG_REG_CMD_TBL(g_astCtsCmnObjTbl), HI_DIAG_CMD_INSTANCE_DEFAULT);
//    cts_InitMdmMacTest();
//    cts_InitMdmUpgObj();
}

//*****************************************************************************
// 函数名称: CTS_MainTaskBody
// 功能描述: CTS 主控任务 
//           
// 参数说明:
//  This function has no arguments.
//
// 返 回 值: 
//    无
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: shenhankun/00130424 [2011-11-08]
//查看性能是133还是266方法
// osm 101e0038 4 -rf 00:00:c0:a8:01:65
//(0x3001) 
//struct root =                                                                  
//str        = 
////0x101e0038  0x00045085  0x0000000f  0x00000015  0x00000000  
///0x00045085-->133M   0x0004508A--->266M
//*****************************************************************************
extern HI_U32 g_SetPerformanceRet;
HI_VOID CTS_MainTaskBody(HI_U32 unused)
{
//    CTS_UPG_OBTAIN_MEM_CTX * ctsObtainMemCtx = upgGetMemObtainCtx();

    CTS_MainTaskInit();
    // Task loop
    for (;;)
    {
        //HI_U32 ret = HI_ERR_SUCCESS;
        HI_SYS_QUEUE_MSG_S stMsg = { 0 };
        HI_CTS_OBJ_PROC_FN pfnObjProc = HI_NULL;
        HI_MDM_MSG_Wait(g_stCtsOsRes.aulQueueId[EN_APP_QUEUE_CTS], &stMsg, HI_SYS_WAIT_FOREVER);
        if(stMsg.ulMsgId)
        {
            pfnObjProc = (HI_CTS_OBJ_PROC_FN)stMsg.ulParam[2];
            if(pfnObjProc)
            {
                HI_CTS_REQ_S* pstReq = (HI_CTS_REQ_S*)stMsg.ulParam[0];
                if(pstReq)
                {
                    pfnObjProc(pstReq->aucData, pstReq->ulDataSize, pstReq->ulOpt, pstReq->ulId);
                }
            }
	     if(ID_CTS_MSG_MR_TIMER == stMsg.ulMsgId)
	     {
	     	  CTS_PRINT0("CTS_MainTaskBody, get cts timer");
//               CTS_PRINT1("UPG_FileLoad RealFileSize=%d", ctsObtainMemCtx->ulRealFileSize);
	     }
	     else
	     {
                 HI_MDM_Free(HI_MOD_ID_USR, (HI_PVOID)stMsg.ulParam[0]);
	     }
        }
		/*
        if(g_SetPerformanceRet == HI_ERR_SUCCESS)
        {
             CTS_PRINT0("CTS SetPerformance OK!");
        }
        else
        {
             CTS_PRINT0("CTS SetPerformance Fail!");
        }
		*/
    }//while
    
    //return;
}

