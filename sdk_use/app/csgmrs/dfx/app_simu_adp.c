//*****************************************************************************
//
//                  ��Ȩ���� (C), 1998-2011, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : app_simu_adp.c
//  �� �� ��   : V1.0
//  ��    ��   : shenhankun/00130424
//  ��������   : 2011-12-24
//  ��������   : ģ������Խ�ʵ�ִ���
//
//  �����б�   : TODO: ...
//  �޸���ʷ   :
//  1.��    �� : 2011-12-24
//    ��    �� : shenhankun/00130424
//    �޸����� : �����ļ�
//
//*****************************************************************************


//*****************************************************************************
// PROJECT   :
// SUBSYSTEM :
// MODULE    :
// OWNER     :
//*****************************************************************************
#include <app_common.h>

#include <app_mrs.h>
#include "app_simu_adp.h"
#include <dfx_sys.h>
#include "../src/mrs_fw_tools.h"
#include "../src/mrs_common.h"
#include "../src/mrs_fw_n.h"
#include "../src/mrs_dfx.h"
#include "../src/mrs_srv_res.h"
#include "../src/mrs_srv_readmeter_config.h"
#include "../src/mrs_fw_proto376_2_echo.h"
#include "../src/mrs_srv_cco_macmap.h"
#include "../src/mrs_srv_cco_archives.h"
#include "../src/mrs_srv_anytime_readmeter.h"
#include "../src/mrs_srv_cco.h"
#include "../src/mrs_srv_cco_detect_rm.h"

HI_PRV HI_U32 SimuSendMrPacket(HI_PVOID pFrame, HI_U16 usFrameSize)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    DIAG_CMD_MSIM_ACK_STRU* pstPacket = HI_NULL;

    if(!pFrame)
    {
        return HI_ERR_FAILURE;
    }
    
    pstPacket = (DIAG_CMD_MSIM_ACK_STRU*)mrsToolsMalloc(usFrameSize + sizeof(DIAG_CMD_MSIM_ACK_STRU));
    if (pstPacket)
    {
        (hi_void)memcpy_s(pstPacket->aucData, usFrameSize, pFrame, usFrameSize);
        pstPacket->ulDataSize = usFrameSize;

        ret = HI_ERR_NOT_SUPPORT;
        
    #if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
        ret = HI_MDM_ReportPacket(ID_DIAG_CMD_CSIM_PACKET, HI_DIAG_CMD_INSTANCE_LOCAL, (HI_PBYTE)pstPacket,
                                (HI_U16)(usFrameSize + sizeof(HI_DIAG_PACKET_S)), HI_FALSE);
    #endif

    #if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
        ret = HI_MDM_ReportPacket(ID_DIAG_CMD_MSIM_PACKET, HI_DIAG_CMD_INSTANCE_LOCAL, (HI_PBYTE)pstPacket,
                                (HI_U16)(usFrameSize + sizeof(HI_DIAG_PACKET_S)), HI_FALSE);
    #endif

        mrsToolsFree(pstPacket);
    }

    return ret;
}

// HSO-->
HI_U32 SimRxInput(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption)
{
    HI_U32 ret = HI_ERR_SUCCESS;
	MRS_FW_CTX_STRU* pstFwCtx = mrsGetFwCtx();
    HI_U32 ulSimuChlTimeout = 0;
    HI_SAL_DFX_HOST_CTRL_S * ctrl = HI_MDM_DfxGetHostCtrl();

    MRS_NOT_USED(ulOption);
    MRS_NOT_USED(usCmdParamSize);

    if ((!pCmdParam)||(!pstFwCtx)||(!ctrl))
    {
        return HI_ERR_FAILURE;
    }

    // ���HSO��һ��ʱ����û����ģ��ͨ������ô��Ҫ�ر�ģ��ͨ��
    // �˴��ж��Ƿ���ģ��ͨ����״̬���������һ����ʱ��
    MRS_StopTimer(MRS_CCO_TIMER_SIMU_CHL);
	
	ulSimuChlTimeout = pstFwCtx->ulSimuChlTimeout;
    if (0 != ctrl->ucMrRedirect2HsoPort)
    {
        MRS_StartTimer(MRS_CCO_TIMER_SIMU_CHL, ulSimuChlTimeout, HI_SYS_TIMER_ONESHOT);
    }

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)    
    // ������ģ������� CCO
    if (ID_DIAG_CMD_CSIM_PACKET == usId)
    {
        if(ctrl->ucMrRedirect2HsoPort == 0) // close sim-chl
        {
            // by-pass
            return HI_ERR_NOT_EXIST;
        }
        else 
        {
            DIAG_CMD_CSIM_REQ_STRU* pstReq = (DIAG_CMD_CSIM_REQ_STRU*)pCmdParam;
            
            ret = HI_MDM_DfxChlUartRx(HI_DMS_CHL_UART_PORT_APP, pstReq->aucData, (HI_U16) pstReq->ulDataSize);
            if (ctrl->ucMrLoopBack)
            {
                HI_MDM_ReportPacket(ID_DIAG_CMD_SIMU_PACKET_ECHO, HI_DIAG_CMD_INSTANCE_LOCAL, (HI_PBYTE)pstReq->aucData,
                                    (HI_U16)(pstReq->ulDataSize), HI_FALSE);
            }
        }
    }
#endif
#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
    // ���ģ������� STA
    if (ID_DIAG_CMD_MSIM_PACKET == usId)
    {
        if(ctrl->ucMrRedirect2HsoPort == 0) // close sim-chl
        {
            // by-pass
            return HI_ERR_NOT_EXIST;
        }
        else 
        {
            DIAG_CMD_MSIM_REQ_STRU* pstReq = (DIAG_CMD_MSIM_REQ_STRU*)pCmdParam;
            
            ret = HI_MDM_DfxChlUartRx(HI_DMS_CHL_UART_PORT_APP, pstReq->aucData, (HI_U16)pstReq->ulDataSize);
            if (ctrl->ucMrLoopBack)
            {
                HI_MDM_ReportPacket(ID_DIAG_CMD_SIMU_PACKET_ECHO, HI_DIAG_CMD_INSTANCE_LOCAL, (HI_PBYTE)pstReq->aucData,
                                    (HI_U16)(pstReq->ulDataSize), HI_FALSE);
            }
        }
    }
#endif
    else if (ID_DIAG_CMD_SIMU_CTRL == usId)
    {
        HI_SAL_DFX_HOST_CTRL_S* pstCtrl = (HI_SAL_DFX_HOST_CTRL_S*)pCmdParam;

        if(pstCtrl->ucMrRedirect2HsoPort == 0) // Close 
        {
            //��ȡͨ���˿�
            HI_DMS_CHL_PORT_OBJ_S stChlPort = mrsGetChlPort();

            MRS_StopTimer(MRS_CCO_TIMER_SIMU_CHL);
            mrsFwChlInit(&stChlPort, HI_DMS_CHL_UART_PORT_APP);
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
            mrsDetectRxModeNotify(DIAG_CMD_MRS_MODE_NORMAL);
#endif
        }
        else if(pstCtrl->ucMrRedirect2HsoPort == 1) // open
        {
            HI_MDM_ChlCtrl(HI_DMS_CHL_UART_PORT_APP, HI_DMS_PORT_CTRL_UART_CLOSE, HI_NULL, HI_NULL);

            MRS_StartTimer(MRS_CCO_TIMER_SIMU_CHL, ulSimuChlTimeout, HI_SYS_TIMER_ONESHOT);
        }

        mrsDfxVmChlStatus(pstCtrl->ucMrRedirect2HsoPort); // ģ��ͨ��ͳ��

        HI_MDM_DfxSetHostCtrl((HI_SAL_DFX_HOST_CTRL_S*)pCmdParam, (HI_U32)SimuSendMrPacket, (HI_U32)HI_NULL);

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
        mrs3762DiscardOldData();
        mrsRmCfgSetSimuChlStatus((HI_BOOL)pstCtrl->ucMrRedirect2HsoPort);
#endif
    }
    
    return ret;
}

#if defined(PRODUCT_CFG_FEATURE_REG_PARALLEL_CMD)
HI_U32 SimRxCtrl(HI_U16 usId, HI_U16 usPkSn, HI_PVOID pCache, HI_U32 ulOption)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    
    MRS_NOT_USED(ulOption);
    MRS_NOT_USED(usPkSn);
    
    if(!pCache)
    {
        return HI_ERR_FAILURE;
    }
 
    // ������ģ������� CCO
    if ((ID_DIAG_CMD_CSIM_PACKET == usId)
        || (ID_DIAG_CMD_SIMU_CTRL == usId))
    {
        mrsToolsFree(pCache);
    }
    
    return ret;
}
#endif

#if defined(PRODUCT_CFG_FEATURE_REG_PARALLEL_CMD)
HI_PRV HI_PVOID GetParallelCmdCache(HI_U16 usId, HI_U32 ulCacheSize, HI_U16 usPkSn)
{
    HI_PBYTE buf = HI_NULL;

    MRS_NOT_USED(usId);
    MRS_NOT_USED(usPkSn);
    
    buf = mrsToolsMalloc(ulCacheSize);
    
    return (HI_PVOID)buf;
}
#endif

// ģ����� Ӧ�ó�ʼ��
HI_U32 appSimuInit(HI_VOID)
{
#if defined(PRODUCT_CFG_FEATURE_REG_PARALLEL_CMD)    
    HI_MDM_RegisterParallelCmd(ID_DIAG_CMD_CSIM_PACKET, ID_DIAG_CMD_SIMU_CTRL, GetParallelCmdCache);
#endif

    return HI_ERR_SUCCESS;
}


