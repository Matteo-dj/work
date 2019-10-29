//*****************************************************************************
//
//                  版权所有 (C), 1998-2011, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_cco_init.c
//  版 本 号   : V1.0
//  作    者   : shenhankun/00130424
//  生成日期   : 2011-10-25
//  功能描述   : MRS SRV 初始化
//
//  函数列表   : mrsSrvInit
//  修改历史   :
//  1.日    期 : 2011-10-25
//    作    者 : shenhankun/00130424
//    修改内容 : 创建文件
//
//*****************************************************************************
#include "mrs_common.h"
#include "mrs_cmd_msg.h"
#include "mrs_fw_proto376_2.h"
#include "mrs_fw_proto645.h"
#include "mrs_fw_proto376_2_echo.h"
#include "mrs_fw_log.h"
#include "mrs_srv_common.h"
#include "mrs_srv_cco_macmap.h"
#include "mrs_srv_parallel_cco.h"
#include "mrs_srv_list.h"
#include "mrs_srv_cco_event.h"
#include "mrs_srv_cco.h"
#include "mrs_srv_sta_queue.h"
#include "mrs_srv_parallel_sta.h"
#include "mrs_srv_sta_event.h"
#include "mrs_srv_sta.h"
#include "mrs_srv_cfg.h"
#include "mrs_srv_copy_ctrl_device.h"
#include "mrs_srv_collector.h"
#include "mrs_power_failure.h"
#include "mrs_transfomer_identify.h"
#include "mrs_srv_ndm_detectid.h"

//*****************************************************************************
// 函数名称: mrsSrvStart mrsSrvStop
// 功能描述: MRS的服务功能初始化
//
// 参数说明:
//  This function has no arguments.
//
// 返 回 值:
//    TODO: ...
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: shenhankun/00130424 [2011-11-09]
//*****************************************************************************
HI_U32 mrsSrvStart(HI_VOID)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    mrsCcoInitCtx();
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
    HI_PLC_NEIGHBOR_BROADCAST_FRAME_CTRL_STRU stRcvNeighborBroadcastFrameCtrl = {0};
    HI_FTM_PRODUCT_TYPE_E enProductType = HI_MDM_GetProductType();
    // 电表产品形态
/* BEGIN: Modified by fengxiaomin/00209182, 2014/5/26   问题单DTS2013122000310 */
    if (HI_MDM_IsProductMeter(enProductType)||HI_MDM_IsProductMeterI(enProductType) || HI_MDM_IsProductMeterII(enProductType)||HI_MDM_IsProductTSta(enProductType))
/* END:   Modified by fengxiaomin/00209182, 2014/5/26 */
    {
        mrsStaInitCtx();
    }

    if (HI_MDM_IsProductMeterII(HI_MDM_GetProductType()))
    {
        mrsCollectorInit();//初始化采集器
        mrsSetCollectDevAddr(HI_NV_FTM_COLLECT_PARAM_FILE);
    }
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_NDM)
    mrsCopyCtrlDeviceInit();
    HI_MDM_NdmDetectidStateReport(mrs_ndm_detect_id_state_report);
#endif

    // 消息注册
    ulRet = MRS_MsgRegister(g_astMsgsFunTable, HI_ARRAY_COUNT(g_astMsgsFunTable));

    // 命令注册
    ulRet |= MRS_CmdRegister(g_astCmdFunTable, HI_ARRAY_COUNT(g_astCmdFunTable));

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
    //停电上报的注册回调函数，进入应用层接口
    HI_MDM_RegisterPowerFailureEvent(mrsPowerFailureCallback);
	if (HI_DMS_IsProductTsta(enProductType) == HI_TRUE) {
		HI_MDM_SetIsSingleTsta(HI_TRUE);
	}
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) || defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
    HI_MDM_registerTfCallbackFun((TF_CALLBACK_FUN)mrsTfRcvMacReportCallback);
#endif
#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
    stRcvNeighborBroadcastFrameCtrl.rcv_switch = HI_TRUE;
    stRcvNeighborBroadcastFrameCtrl.filter_fun = mrsTfIsRcvNeighborBroadcastFrame;
    HI_MDM_SetRcvNeighborBrodcastFrameCtrl(&stRcvNeighborBroadcastFrameCtrl);
#endif


#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)//Matteo
	MRS_StartTimer(MRS_CCO_TIMER_ARCHIVE_CHANGE_CHECK, MRS_CCO_CHECK_ARCHIVEE_CHANGE_TIME, HI_SYS_TIMER_PERIODIC);
#endif

    return ulRet;
}

HI_U32 mrsSrvStop(HI_VOID)
{
    // 消息注册
    MRS_MsgRegister(HI_NULL, 0);

    // 命令注册
    MRS_CmdRegister(HI_NULL, 0);

    return HI_ERR_SUCCESS;
}

