//*****************************************************************************
//
//                  版权所有 (C), 2001-2014, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_fw_proto_csg_echo.c
//  版 本 号   : V1.0 
//  作    者   : cuiate/c00233580
//  生成日期   : 2015-12-09
//  功能描述   : TODO: ...
//               
//  函数列表   : TODO: ...
//  修改历史   : 
//  1.日    期 : 2015-12-09
//    作    者 : cuiate/c00233580
//    修改内容 : 创建文件 
//
//*****************************************************************************

#include "mrs_common.h"
#include "mrs_fw_n.h"
#include "mrs_fw_proto_csg_echo.h"
#include "mrs_fw_proto_csg.h"
#include "mrs_fw_tools.h"
#include "mrs_fw_proto376_2_echo.h"
#include "mrs_fw_log.h"
#include "mrs_srv_res.h"
#include "mrs_srv_cco_queue.h"
#include "mrs_srv_cco_macmap.h"
#include "mrs_srv_cco_broadcast.h"
#include "mrs_srv_list.h"
#include "mrs_srv_cco_event.h"
#include "mrs_srv_cco_remote_upg.h"
#include "mrs_srv_cco.h"
#include "mrs_srv_cco_searchmeter.h"
#include "mrs_srv_cco_archives.h"
#include "mrs_srv_cco_remote_upg.h"
#include "mrs_dfx_cco_remote_upg.h"
#include "mrs_srv_cco_mr_task.h"
#include "mrs_fw_proto645.h"
#include "mrs_srv_cco_event.h"
#include "mrs_dfx_cco.h"
#include "mrs_time.h"
#include "mrs_srv_cco_event.h"
#include "mrs_srv_mrslog.h"
#include "mrs_srv_queue.h"
HI_START_HEADER

#if ((defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) || defined(PRODUCT_CFG_PRODUCT_TYPE_STA)) && defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD))

HI_PRV MRS_PROTO_CSG_CONTEXT g_stProtoCsgCtx;
HI_PRV MRS_CSG_AFN_FUN mrsCsgFindAfnProc(HI_U8 ucAfn, HI_U32 ulDi);


HI_U32 mrsCsgAfnDispatch(HI_IN MRS_CSG_FRAME_STRU * pstFrame, HI_OUT HI_PBYTE * ppOutBuffer, HI_U16 * pusBufferLen)
{
    MRS_CSG_AFN_FUN pfnProc = HI_NULL;
    HI_U32 ulRet = HI_ERR_NOT_FOUND;

    if (!pstFrame || !ppOutBuffer || !pusBufferLen)
    {
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_001, HI_DIAG_MT("[CSG ECHO] invalid pParam."));
        return HI_ERR_BAD_DATA;
    }

    *pusBufferLen = 0;
    *ppOutBuffer = HI_NULL;
    HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_002, HI_DIAG_MT("[CSG ECHO] AFN, DI"), pstFrame->ucAfn, pstFrame->ulDI);

    pfnProc = mrsCsgFindAfnProc(pstFrame->ucAfn, pstFrame->ulDI);
    HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_003, HI_DIAG_MT("[CSG ECHO] pfnProc"), pfnProc);
    if (pfnProc)
    {
        AFN_DI_UL_PARAM stParam;

        (hi_void)memset_s(&stParam, sizeof(stParam), 0, sizeof(stParam));
        stParam.pstFrame = pstFrame;
        stParam.ppucOutBuffer = ppOutBuffer;
        stParam.pusBufferLen = pusBufferLen;

        ulRet = pfnProc(&stParam);
    }

    return ulRet;
}


MRS_CSG_AFN_FUN mrsCsgFindAfnProc(HI_U8 ucAfn, HI_U32 ulDi)
{
    MRS_PROTO_CSG_CONTEXT *pstCtx = mrsProtoCsgEchoCtx();
    HI_U32 ulIndex = 0;

    HI_UNREF_PARAM(ucAfn);

    for (ulIndex = 0; ulIndex < pstCtx->ulFuncNum; ulIndex++)
    {
        if (ulDi == pstCtx->pFuncMap[ulIndex].ulDi)
        {
            return pstCtx->pFuncMap[ulIndex].pfnFunc;
        }
    }

    return HI_NULL;
}


HI_U32 mrsProtoCsgEchoInit(HI_VOID)
{
    (hi_void)memset_s(&g_stProtoCsgCtx, sizeof(g_stProtoCsgCtx), 0, sizeof(g_stProtoCsgCtx));
    return HI_ERR_SUCCESS;
}


HI_U32 mrsProtoCsgEchoDeinit(HI_VOID)
{
    (hi_void)memset_s(&g_stProtoCsgCtx, sizeof(g_stProtoCsgCtx), 0, sizeof(g_stProtoCsgCtx));
    return HI_ERR_SUCCESS;
}

HI_U32 mrsProtoCsgEchoRegFuncMap(MRS_CSG_AFN_FUNC_MAP *pFuncMap, HI_U32 ulFuncNum)
{
    MRS_PROTO_CSG_CONTEXT *pstCtx = mrsProtoCsgEchoCtx();

    pstCtx->pFuncMap = pFuncMap;
    pstCtx->ulFuncNum = ulFuncNum;

    return HI_ERR_SUCCESS;
}


MRS_PROTO_CSG_CONTEXT *mrsProtoCsgEchoCtx(HI_VOID)
{
    return &g_stProtoCsgCtx;
}


HI_U32 mrsCsgProtoAckFrame(HI_IN HI_U16 usWaitTime, HI_IN AFN_DI_UL_PARAM * pstDlFrameInf)
{
    MRS_CSG_IND_FRAME_STRU stIndFrame = {0};
    MRS_CSG_FRAME_STRU  *pstFrame = pstDlFrameInf->pstFrame;

    if ((pstDlFrameInf->ppucOutBuffer == HI_NULL) || (pstDlFrameInf->pusBufferLen == HI_NULL))
    {
        return HI_ERR_BAD_DATA;
    }

    (hi_void)memset_s(&stIndFrame, sizeof(stIndFrame), 0, sizeof(stIndFrame));
    stIndFrame.ucAfn = MRS_CSG_AFN_ACK_00H;
    stIndFrame.ulDI =  MRS_CSG_DI_ACK;
    stIndFrame.ucSeq = pstFrame->ucSeq;
    stIndFrame.ucPrm = 0;
    stIndFrame.ucAddrFlag = 0;

    stIndFrame.usAppDataLen  = sizeof(HI_U16);
    stIndFrame.pAppData = (HI_U8 *)&usWaitTime;

    return mrsCreateCsgFrame(&stIndFrame, pstDlFrameInf->ppucOutBuffer, pstDlFrameInf->pusBufferLen);
}


HI_U32 mrsCsgProtoDenyFrame(HI_IN HI_U8 * pstDenyFrame, HI_IN AFN_DI_UL_PARAM * pstDlFrameInf)
{
    MRS_CSG_IND_FRAME_STRU stIndFrame = {0};
    HI_U8  stDefaultAck = 0;

    if ((!pstDlFrameInf) || (pstDlFrameInf->ppucOutBuffer == HI_NULL) || (pstDlFrameInf->pusBufferLen == HI_NULL))
    {
        return HI_ERR_BAD_DATA;
    }

    if (pstDenyFrame == HI_NULL)
    {
        pstDenyFrame = &stDefaultAck;
    }

    stIndFrame.ucAfn = MRS_CSG_AFN_ACK_00H;
    stIndFrame.ulDI = MRS_CSG_DI_DENY;
    stIndFrame.ucSeq = pstDlFrameInf->pstFrame->ucSeq;
    stIndFrame.ucAddrFlag = 0;
    stIndFrame.ucPrm = 0;
    stIndFrame.usAppDataLen = sizeof(HI_U8);
    stIndFrame.pAppData = pstDenyFrame;

    return mrsCreateCsgFrame(&stIndFrame,pstDlFrameInf->ppucOutBuffer,pstDlFrameInf->pusBufferLen);
}

HI_U32 mrsCsgProtoManuInfo(AFN_CSG_MANU_INFO_STRU *pstManuInf)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_MDM_SYS_INFO_S *pstVerInfo = HI_NULL;

    if (!pstManuInf)
    {
        return HI_ERR_FAILURE;
    }

    pstVerInfo = (HI_MDM_SYS_INFO_S *)HI_MDM_GetSysInfo();
	if (!pstVerInfo)
	{
		return HI_ERR_FAILURE;
	}

    (hi_void)memcpy_s(pstManuInf->szManuCode, MRS_MANU_CODE_LEN, pstVerInfo->szManuCode, MRS_MANU_CODE_LEN);
    (hi_void)memcpy_s(pstManuInf->szChipCode, MRS_CHIP_CODE_LEN, pstVerInfo->szChipCode, MRS_CHIP_CODE_LEN);
    mrsIntToBcd((HI_U32)pstVerInfo->usDay, &pstManuInf->ucDay, sizeof(HI_U8));
    mrsIntToBcd((HI_U32)pstVerInfo->usMonth, &pstManuInf->ucMonth, sizeof(HI_U8));
    mrsIntToBcd((HI_U32)pstVerInfo->usYear, &pstManuInf->ucYear, sizeof(HI_U8));
    (hi_void)memcpy_s(pstManuInf->aucVer, sizeof(pstManuInf->aucVer), &pstVerInfo->usSwVer, sizeof(pstVerInfo->usSwVer));

    return ret;
}

#endif


HI_END_HEADER
