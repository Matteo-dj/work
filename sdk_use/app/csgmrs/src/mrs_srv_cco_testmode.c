//*****************************************************************************
//
//                  ��Ȩ���� (C), 2001-2015, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mrs_srv_cco_testmode.c
//  �� �� ��   : V1.0
//  ��    ��   : cuiate/00233580
//  ��������   : 2015/09/16
//  ��������   : CCO����ģʽ
//  �����б�   :
//  �޸���ʷ   :
//  1.��    �� : 2015/09/16
//    ��    �� : cuiate/00233580
//    �޸����� : �����ļ�
//
//*****************************************************************************

#include "mrs_common.h"
#include "mrs_cmd_msg.h"
#include "mrs_fw_tools.h"
#include "mrs_fw_log.h"
#include "mrs_fw_proto376_2_echo.h"
#include "mrs_srv_res.h"
#include "mrs_srv_cco_macmap.h"
#include "mrs_srv_list.h"
#include "mrs_srv_cco_event.h"
#include "mrs_srv_cco.h"
#include "mrs_srv_cco_readmeter.h"
#include "mrs_srv_cco_testmode.h"
HI_START_HEADER

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
//*****************************************************************************
//*****************************************************************************

#define MRS_CCO_TEST_MODE_FRAME_TIMEOUT_DEFAULT     (1)     // ����

typedef struct
{
    HI_BOOL bEnable;
    HI_BOOL bTestMode;
    HI_U8   ucFrameTimeout;
    HI_U8   ucSnId;
} MRS_CCO_TEST_MODE_INF;


//*****************************************************************************

HI_PRV MRS_CCO_TEST_MODE_INF g_stTestModeInf = {HI_FALSE, HI_FALSE, MRS_CCO_TEST_MODE_FRAME_TIMEOUT_DEFAULT, 0};
HI_PRV MRS_CCO_TEST_MODE_INF *mrsCcoTestModeInf(HI_VOID);


MRS_CCO_TEST_MODE_INF *mrsCcoTestModeInf(HI_VOID)
{
    return &g_stTestModeInf;
}


HI_BOOL mrsCcoIsTestMode(HI_VOID)
{
    MRS_CCO_TEST_MODE_INF *pInf = mrsCcoTestModeInf();

    return (HI_BOOL)((pInf->bEnable) ? pInf->bTestMode : HI_FALSE);
}


HI_VOID mrsCcoSetTestModeCtrlInfo(HI_BOOL bEnable, HI_U8 ucFrameTimeout)
{
    MRS_CCO_TEST_MODE_INF *pInf = mrsCcoTestModeInf();

    HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_001, HI_DIAG_MT("����ģʽ����: enable ucFrameTimeout"), bEnable, ucFrameTimeout);

    if (pInf->bEnable && pInf->bTestMode && !bEnable)
    {
        MRS_StopTimer(MRS_CCO_TIMER_TEST_MODE_FRAME);
        HI_MDM_ExitTestMode();
    }

    pInf->bEnable = bEnable;
    pInf->ucFrameTimeout = (ucFrameTimeout == 0) ? MRS_CCO_TEST_MODE_FRAME_TIMEOUT_DEFAULT : ucFrameTimeout;
}


HI_U32 mrsCcoTestModeFrameRx(MRS_CMD_TEST_MODE_STRU *pstTestMode)
{
    MRS_CCO_TEST_MODE_INF *pInf = mrsCcoTestModeInf();

    if (!pInf->bEnable)
    {
        return HI_ERR_NOT_SUPPORT;
    }

    if ((!pInf->bTestMode) || (pInf->ucSnId != pstTestMode->ucSnId))
    {
#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
        if (!mrsGetCcoPlcChlStatus() && !pInf->bTestMode && (mrsCcoGetState() & MRS_CCO_STATUS_NORMAL))
        {
            mrsReadMeterStateInit();    // ���¿�ʼ��������
            MRS_StartTimer(EN_MRS_SRV_CCO_READMETER_ACTIV_TIMER, MRS_SRV_CCO_TIMER_READMETER_ACTIV_VALUE, HI_SYS_TIMER_ONESHOT);
        }
#endif

        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_003, HI_DIAG_MT("�������ģʽ"));
        pInf->bTestMode = HI_TRUE;
        pInf->ucSnId = pstTestMode->ucSnId;
        HI_MDM_EnterTestMode(pstTestMode->ucSnId);
    }

    MRS_StartTimer(MRS_CCO_TIMER_TEST_MODE_FRAME, pInf->ucFrameTimeout * 60 * 1000, HI_SYS_TIMER_ONESHOT);

    return HI_ERR_SUCCESS;
}


HI_U32 mrsCcoTestModeFrameTimeout(HI_VOID)
{
    MRS_CCO_TEST_MODE_INF *pInf = mrsCcoTestModeInf();
 
    if (!pInf->bEnable)
    {
        return HI_ERR_NOT_SUPPORT;
    }
    
    return HI_MDM_ExitTestMode();
}


//*****************************************************************************
//*****************************************************************************
#endif // defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
HI_END_HEADER

