//*****************************************************************************
//
//                  ��Ȩ���� (C), 1998-2011, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mrs_srv_common.c
//  �� �� ��   : V1.0 
//  ��    ��   : liuxipeng/KF54842
//  ��������   : 2014-06-26
//  ��������   : ����Ӧ�ù��ú���
//               ʵ�ֶ���Ӧ�ù��ú���(CCO & STA)����Դ��ʵ�������
//               
//  �����б�   : 
//  �޸���ʷ   : 
//  1.��    �� : 2014-06-26
//    ��    �� : liuxipeng/KF54842
//    �޸����� : �����ļ� 
//
//*****************************************************************************
#include "mrs_common.h"
#include "app_nv.h"
#include "mrs_fw_tools.h"
#include "mrs_fw_log.h"
#include "mrs_fw_n.h"
#include "mrs_srv_common.h"
#include "mrs_fw_proto376_2_echo.h"
#include "mrs_fw_nv.h"


#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)

#define MRS_SRV_CHL_NOTIFY_PROC_NUM_MAX    (5)

typedef struct
{
    HI_U32 ulParam;
    MRS_CHL_NOTIFY_PROC_F pfProc;
}MRS_SRV_CHL_NOTIFY_PROC_S, *P_MRS_SRV_CHL_NOTIFY_PROC_S;

typedef struct
{
    MRS_SRV_CHL_NOTIFY_PROC_S stRegItem[MRS_SRV_CHL_NOTIFY_PROC_NUM_MAX];
}MRS_SRV_CHL_NOTIFY_CTX_S, *P_MRS_SRV_CHL_NOTIFY_CTX_S;

// ͨ���ı�֪ͨ����������
MRS_SRV_CHL_NOTIFY_CTX_S g_stMrsChlNotifyProcCtx;

P_MRS_SRV_CHL_NOTIFY_CTX_S mrsSrvGetChlNotiyProcCtx(HI_VOID);

// ��ȡͨ���ı�֪ͨ����������
P_MRS_SRV_CHL_NOTIFY_CTX_S mrsSrvGetChlNotiyProcCtx(HI_VOID)
{
    return &g_stMrsChlNotifyProcCtx;
}

// ͨ���ı�֪ͨ�����ʼ��
HI_VOID mrsSrvChlNotifyProcInit(HI_VOID)
{
    P_MRS_SRV_CHL_NOTIFY_CTX_S pstChlNotifyCtx = mrsSrvGetChlNotiyProcCtx();

    (hi_void)memset_s(pstChlNotifyCtx, sizeof(MRS_SRV_CHL_NOTIFY_CTX_S), 0, sizeof(MRS_SRV_CHL_NOTIFY_CTX_S));

    return;
}

// ͨ���ı�֪ͨ������ע��
HI_U32 mrsSrvChlNotifyProcReg(MRS_CHL_NOTIFY_PROC_F pfProc, HI_U32 ulParam)
{
    HI_U16 usIndex = 0;
    P_MRS_SRV_CHL_NOTIFY_CTX_S pstChlNotifyCtx = mrsSrvGetChlNotiyProcCtx();
        
    for(usIndex = 0; usIndex < MRS_SRV_CHL_NOTIFY_PROC_NUM_MAX; usIndex++)
    {
        if(pstChlNotifyCtx->stRegItem[usIndex].pfProc == HI_NULL)
        {
            pstChlNotifyCtx->stRegItem[usIndex].pfProc = pfProc;
            pstChlNotifyCtx->stRegItem[usIndex].ulParam = ulParam;

            return HI_ERR_SUCCESS;
        }
    }

    return HI_ERR_FULL;
}

// ͨ���ı�֪ͨ����������
HI_U32 mrsSrvChlNotifyProcClear(MRS_CHL_NOTIFY_PROC_F pfProc)
{
    HI_U16 usIndex = 0;
    P_MRS_SRV_CHL_NOTIFY_CTX_S pstChlNotifyCtx = mrsSrvGetChlNotiyProcCtx();
        
    for(usIndex = 0; usIndex < MRS_SRV_CHL_NOTIFY_PROC_NUM_MAX; usIndex++)
    {
        if(pstChlNotifyCtx->stRegItem[usIndex].pfProc == pfProc)
        {
            pstChlNotifyCtx->stRegItem[usIndex].ulParam = 0;
            pstChlNotifyCtx->stRegItem[usIndex].pfProc = HI_NULL;

            return HI_ERR_SUCCESS;
        }
    }

    return HI_ERR_NOT_FOUND;
}

// ͨ���ı�֪ͨ����������
HI_U32 mrsSrvChlNotifyProc(HI_BOOL bChlStatus)
{
    HI_U16 usIndex = 0;
    P_MRS_SRV_CHL_NOTIFY_CTX_S pstChlNotifyCtx = mrsSrvGetChlNotiyProcCtx();
        
    for(usIndex = 0; usIndex < MRS_SRV_CHL_NOTIFY_PROC_NUM_MAX; usIndex++)
    {
        HI_BOOL bOneShot = HI_FALSE;
        P_MRS_SRV_CHL_NOTIFY_PROC_S pstItem = &pstChlNotifyCtx->stRegItem[usIndex];
        
        if(pstItem->pfProc != HI_NULL)
        {
            pstItem->pfProc(bChlStatus, pstItem->ulParam, &bOneShot);
            if(bOneShot)
            {
                pstItem->ulParam = 0;
                pstItem->pfProc = HI_NULL;
            }
        }
    }

    return HI_ERR_SUCCESS;
}

#endif // defined(PRODUCT_CFG_PRODUCT_TYPE_STA)


//*****************************************************************************
// ��������: mrsSrvGetVerInfo
// ��������: ��ȡģ�������汾��boot�汾
//           
// ����˵��:
//   pusVer     [OUT]����汾
//   pucBootVer [OUT]boot�汾
//
// �� �� ֵ: 
//    HI_ERR_SUCCESS:  �ɹ�
//    ����: ʧ��
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: fengxiaomin/00209182 [2015-10-31]
//*****************************************************************************
HI_U32 mrsSrvGetVerInfo(HI_U16 *pusVer, HI_U8 *pucBootVer)
{
	HI_MDM_SYS_INFO_S *pstVerInfo = HI_NULL;
	
    pstVerInfo = (HI_MDM_SYS_INFO_S *)HI_MDM_GetSysInfo();
	if (!pstVerInfo)
	{
		return HI_ERR_FAILURE;
	}

	*pusVer = pstVerInfo->usSwVer;
	*pucBootVer = pstVerInfo->ucBootVer;

	return HI_ERR_SUCCESS;
}

