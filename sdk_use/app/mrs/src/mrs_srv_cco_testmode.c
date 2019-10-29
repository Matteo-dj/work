//*****************************************************************************
//
//                  版权所有 (C), 2001-2015, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_srv_cco_testmode.c
//  版 本 号   : V1.0
//  作    者   : cuiate/00233580
//  生成日期   : 2015/09/16
//  功能描述   : CCO测试模式
//  函数列表   :
//  修改历史   :
//  1.日    期 : 2015/09/16
//    作    者 : cuiate/00233580
//    修改内容 : 创建文件
//
//*****************************************************************************

#include "mrs_common.h"
#include "mrs_cmd_msg.h"
#include "mrs_fw_tools.h"
#include "mrs_fw_log.h"
#include "mrs_fw_proto376_2_echo.h"
#include "mrs_srv_res.h"
#include "mrs_srv_cco_macmap.h"
#include "mrs_srv_parallel_cco.h"
#include "mrs_srv_list.h"
#include "mrs_srv_cco_event.h"
#include "mrs_srv_cco.h"
#include "mrs_srv_cco_readmeter.h"
#include "mrs_srv_cco_testmode.h"
HI_START_HEADER

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
//*****************************************************************************
//*****************************************************************************

#define MRS_CCO_TEST_MODE_FRAME_TIMEOUT_DEFAULT     (1)     // 分钟

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

    HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_001, HI_DIAG_MT("测试模式参数: enable ucFrameTimeout"), bEnable, ucFrameTimeout);

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

    if ((pstTestMode->usInterfaceVer != MRS_CMD_TEST_MODE_UP_VER_R1) ||
        (pstTestMode->usStruLen != sizeof(MRS_CMD_TEST_MODE_STRU)) ||
        (pstTestMode->usPadding1 != 0) ||
        (pstTestMode->ucPadding2 != 0)){
        return HI_ERR_INVALID_PARAMETER;
    }

    if ((!pInf->bTestMode) || (pInf->ucSnId != pstTestMode->ucSnId))
    {
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_003, HI_DIAG_MT("进入测试模式"));
        pInf->bTestMode = HI_TRUE;
        pInf->ucSnId = pstTestMode->ucSnId;
        HI_MDM_EnterTestMode((HI_U32)pstTestMode->ucSnId);
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

