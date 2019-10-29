//*****************************************************************************
//
//                  ��Ȩ���� (C), 1998-2011, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mrs_cco_init.c
//  �� �� ��   : V1.0
//  ��    ��   : shenhankun/00130424
//  ��������   : 2011-10-25
//  ��������   : MRS SRV ��ʼ��
//
//  �����б�   : mrsSrvInit
//  �޸���ʷ   :
//  1.��    �� : 2011-10-25
//    ��    �� : shenhankun/00130424
//    �޸����� : �����ļ�
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


//*****************************************************************************
// ��������: mrsSrvStart mrsSrvStop
// ��������: MRS�ķ����ܳ�ʼ��
//
// ����˵��:
//  This function has no arguments.
//
// �� �� ֵ:
//    TODO: ...
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: shenhankun/00130424 [2011-11-09]
//*****************************************************************************
HI_U32 mrsSrvStart(HI_VOID)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    mrsCcoInitCtx();
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
    HI_FTM_PRODUCT_TYPE_E enProductType = HI_MDM_GetProductType();
    // ����Ʒ��̬
/* BEGIN: Modified by fengxiaomin/00209182, 2014/5/26   ���ⵥDTS2013122000310 */
    if (HI_MDM_IsProductMeter(enProductType)||HI_MDM_IsProductMeterI(enProductType) || HI_MDM_IsProductMeterII(enProductType)||HI_MDM_IsProductTSta(enProductType))
/* END:   Modified by fengxiaomin/00209182, 2014/5/26 */
    {
        mrsStaInitCtx();
    }

    if (HI_MDM_IsProductMeterII(HI_MDM_GetProductType()))
    {
        mrsCollectorInit();//��ʼ���ɼ���
        mrsSetCollectDevAddr(HI_NV_FTM_COLLECT_PARAM_FILE);
    }
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_NDM)
        mrsCopyCtrlDeviceInit();
#endif

#if (defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) || defined(PRODUCT_CFG_PRODUCT_TYPE_STA))
    // ��Ϣע��
    ulRet = MRS_MsgRegister(g_astMsgsFunTable, HI_ARRAY_COUNT(g_astMsgsFunTable));

    // ����ע��
    ulRet |= MRS_CmdRegister(g_astCmdFunTable, HI_ARRAY_COUNT(g_astCmdFunTable));
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
    //ͣ���ϱ���ע��ص�����������Ӧ�ò�ӿ�
    HI_MDM_RegisterPowerFailureEvent(mrsPowerFailureCallback);
#endif
    return ulRet;
}

HI_U32 mrsSrvStop(HI_VOID)
{
    // ��Ϣע��
    MRS_MsgRegister(HI_NULL, 0);

    // ����ע��
    MRS_CmdRegister(HI_NULL, 0);

    return HI_ERR_SUCCESS;
}

