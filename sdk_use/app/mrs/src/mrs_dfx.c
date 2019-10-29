//*****************************************************************************
//
//                  版权所有 (C), 1998-2011, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_dfx.c
//  版 本 号   : V1.0
//  作    者   : niesongsong/nkf62735
//  生成日期   : 2012-08-31
//  功能描述   : dfx功能
//
//  函数列表   :
//  修改历史   :
//  1.日    期 : 2011-11-29
//    作    者 : niesongsong/nkf62735
//    修改内容 : 创建文件
//
//*****************************************************************************

#include "mrs_common.h"
#include "dfx_sys.h"
#include "mrs_fw_tools.h"
#include "mrs_dfx.h"
#include "mrs_srv_sta_queue.h"
#include <dfx_sys_sdm.h>
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) && defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG)
#include "mrs_srv_mrslog.h"
#endif
/* BEGIN: Modified by fengxiaomin/00209182, 2014/3/27   问题单号:DTS2014032906596 */
#include "mrs_time.h"
/* END:   Modified by fengxiaomin/00209182, 2014/3/27 */
#include "mrs_srv_queue.h"
#include "mrs_srv_cltii_power_on_off.h"

/* BEGIN: Added by liuxipeng/lkf54842 2014/03/29 问题单号: DTS2014032907816*/
typedef UART_CHL_INF_S HI_DSID_APP_UART_MSG_SEND_INF_S, HI_DSID_APP_UART_MSG_PROC_INF_S;
typedef PLC_CHL_INF_S  HI_DSID_APP_PLC_MSG_SEND_INF_S,  HI_DSID_APP_PLC_MSG_PROC_INF_S;
/* END  : Added by liuxipeng/lkf54842 2014/03/29 问题单号: DTS2014032907816*/

#define mrsDfxGetModule() (&dfx_module)
#define DFX_ALEN(x) (sizeof(x)/sizeof((x)[0]))
#define DFX_OBJ_DEFAULT(X,M) {X,HI_SYS_DSID_BUSINESS_L0,HI_DSID_TYPE_QRY,1,sizeof(X##_S ),M}

#define CHL_OK 0x55     // 通道已连接
#define CHL_NC 0xAA     // 通道断开

HI_PRV MRS_DFX_MODULE dfx_module;

HI_DIAG_SYS_STAT_OBJ_S g_stAppObjTbl[] = {
    DFX_OBJ_DEFAULT(HI_DSID_APP_UART_MSG_SEND_INF,&dfx_module.app_rx_tx.uart_msg_send_inf),
    DFX_OBJ_DEFAULT(HI_DSID_APP_UART_MSG_PROC_INF,&dfx_module.app_rx_tx.uart_msg_proc_inf),
    DFX_OBJ_DEFAULT(HI_DSID_APP_PLC_MSG_SEND_INF,&dfx_module.app_rx_tx.plc_msg_send_inf),
    DFX_OBJ_DEFAULT(HI_DSID_APP_PLC_MSG_PROC_INF,&dfx_module.app_rx_tx.plc_msg_proc_inf),
    DFX_OBJ_DEFAULT(HI_DSID_APP_TOPO_CHANGE_INF,&dfx_module.app_topo_change),
#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
    DFX_OBJ_DEFAULT(HI_DSID_APP_HEARTBEAT_INF,&dfx_module.app_heart),
#endif
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    DFX_OBJ_DEFAULT(HI_DSID_APP_CCO_RUN_STATUS,&dfx_module.app_cco_status),
#endif
    DFX_OBJ_DEFAULT(HI_DSID_APP_RM_INF,&dfx_module.app_rm_inf),
    DFX_OBJ_DEFAULT(HI_DSID_APP_VM_CHL_INF,&dfx_module.app_vm_chl_inf),
#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
    DFX_OBJ_DEFAULT(HI_DSID_APP_STA_STATUS,&dfx_module.app_sta_status),
#endif
    DFX_OBJ_DEFAULT(HI_DSID_APP_MSG_INF,&dfx_module.app_task_msg_inf),
    DFX_OBJ_DEFAULT(HI_DSID_APP_PLC_BC_INF,&dfx_module.app_plc_bc_inf),
#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
    DFX_OBJ_DEFAULT(HI_DSID_APP_RM_QUEUE_INF,&dfx_module.app_rm_queue_inf),
#endif
    DFX_OBJ_DEFAULT(HI_DSID_APP_RM_RX_TX_INF,&dfx_module.app_rm_rx_tx_inf),
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    DFX_OBJ_DEFAULT(HI_DSID_APP_CCO_GET_TIME_INF,&dfx_module.app_get_time_inf),
    DFX_OBJ_DEFAULT(HI_DSID_APP_CCO_EVT_INF,&dfx_module.app_cco_evt_inf),
    DFX_OBJ_DEFAULT(HI_DSID_APP_CCO_REPORT_QUEUE_INF,&dfx_module.app_cco_report_queue_inf),
    DFX_OBJ_DEFAULT(HI_DSID_APP_CCO_EVT_BAK_INF,&dfx_module.app_cco_evt_inf_bak),
    DFX_OBJ_DEFAULT(HI_DSID_APP_CCO_MEMORY_INC_INF,&dfx_module.app_cco_inc_inf),
#endif
#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
    DFX_OBJ_DEFAULT(HI_DSID_APP_STA_EVT_INF,&dfx_module.app_sta_evt_inf),
    DFX_OBJ_DEFAULT(HI_DSID_APP_STA_EVT_MONITOR_INF,&dfx_module.app_sta_evt_mon_inf),
    DFX_OBJ_DEFAULT(HI_DSID_APP_STA_POWER_ON_OFF_INF,&dfx_module.app_sta_power_on_off_inf),
#endif
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    DFX_OBJ_DEFAULT(HI_DSID_APP_CCO_NODE_STATECHG_INF,&dfx_module.app_cco_node_state_chg_inf),
    DFX_OBJ_DEFAULT(HI_DSID_APP_CCO_DETECT_RM_INF,&dfx_module.app_cco_detect_rm_inf),
    DFX_OBJ_DEFAULT(HI_DSID_APP_CCO_POWER_ON_OFF_INF,&dfx_module.app_cco_power_on_off_inf),
#endif
    DFX_OBJ_DEFAULT(HI_DSID_APP_PLC_FRAME_STAT_INF,&dfx_module.app_plc_frame_inf),
#if defined(PRODUCT_CFG_TTCN)
    DFX_OBJ_DEFAULT(HI_DSID_APP_CSG_PYSICAL_MODE_INF, &dfx_module.app_sta_csg_physicalmode_inf),
#endif
};

HI_VOID RefreshTime(HI_DSID_APP_TOPO_CHANGE_INF_S * topo_change, HI_U32 time);

//初始化DFX
HI_U32 mrsDfxStatInit(HI_VOID)
{

    HI_U32 ret = HI_ERR_FAILURE;

    (hi_void)memset_s(&dfx_module, sizeof(dfx_module), 0, sizeof(dfx_module));

#ifndef SAL_HAVE_OS_WIN_VER
    //注册统计量
    ret = HI_MDM_RegisterSysStat(g_stAppObjTbl,DFX_ALEN(g_stAppObjTbl));
#endif

    return ret;
}

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
MRS_DFX_MODULE* mrsDfxGetCcoNdmPlcInfo(HI_VOID)
{
	return &dfx_module;
}
#endif

// 刷新历史数据
HI_VOID mrsDfxRefreshLastDataU8(HI_U8 *data_buf, HI_U16 num, HI_U8 new_data)
{
    int ret = 0;
    ret = memmove_s(data_buf + 1, num - 1, data_buf, num - 1);
    data_buf[0] = new_data;
    if(ret != 0){
        return;
    }
}

// 刷新历史数据
HI_VOID mrsDfxRefreshLastDataU32(HI_U32 *data_buf, HI_U16 num, HI_U32 new_data)
{
    int ret = 0;
    ret = memmove_s(data_buf + 1, sizeof(HI_U32) * (num - 1), data_buf, sizeof(HI_U32) * (num - 1));
    data_buf[0] = new_data;
    if(ret != 0){
        return;
    }
}

// PLC通道接收统计
// rx 通道接收结构体
// msg_proc 用以表示是消息发送还是处理
//          HI_TRUE : 为消息处理
//          HI_FALSE : 为消息发送
HI_U32 mrsDfxChlPlcRx(HI_DMS_CHL_RX_S * rx, HI_BOOL msg_proc)
{
    MRS_DFX_MODULE *dfx = HI_NULL;
    PLC_CHL_INF_S *plc_inf = HI_NULL;
    HI_U32 curr_time = 0;

    if (!rx)
    {
        return HI_ERR_FAILURE;
    }

    dfx = mrsDfxGetModule();

    curr_time = HI_MDM_GetMilliSeconds();

    if(msg_proc)
    {
    /* BEGIN: Added by liuxipeng/lkf54842 2014/03/29 问题单号: DTS2014032907816*/
        plc_inf = &dfx->app_rx_tx.plc_msg_proc_inf;
    /* END  : Added by liuxipeng/lkf54842 2014/03/29 问题单号: DTS2014032907816*/
    }
    else
    {
    /* BEGIN: Added by liuxipeng/lkf54842 2014/03/29 问题单号: DTS2014032907816*/
        plc_inf = &dfx->app_rx_tx.plc_msg_send_inf;
    /* END  : Added by liuxipeng/lkf54842 2014/03/29 问题单号: DTS2014032907816*/
    }

    plc_inf->rx_total_num++;
    plc_inf->rx_time = curr_time;
    plc_inf->rx_id = rx->usId;
    memcpy_s(plc_inf->src_mac, HI_PLC_MAC_ADDR_LEN, rx->ucSrcMac, HI_PLC_MAC_ADDR_LEN);
    plc_inf->rx_len = rx->usPayloadLen;

    (hi_void)memset_s(plc_inf->rx_buf, sizeof(plc_inf->rx_buf), 0, sizeof(plc_inf->rx_buf));
    memcpy_s(plc_inf->rx_buf, MAX_BUF_LEN,
                   rx->pPayload,
                   MRS_MIN(sizeof(plc_inf->rx_buf), rx->usPayloadLen));

/* BEGIN: Added by liuxipeng/lkf54842 2014/03/29 问题单号: DTS2014032907816*/
// DEL
/* END  : Added by liuxipeng/lkf54842 2014/03/29 问题单号: DTS2014032907816*/

    return HI_ERR_SUCCESS;
}

// PLC通道接收错误统计
// rx 通道接收结构体
// err_cause 错误原因
// msg_proc 用以表示是消息发送还是处理
//          HI_TRUE : 为消息处理
//          HI_FALSE : 为消息发送
HI_U32 mrsDfxChlPlcErrRx(HI_DMS_CHL_RX_S * rx, HI_U32 err_cause, HI_BOOL msg_proc)
{
    MRS_DFX_MODULE *dfx = HI_NULL;
    PLC_CHL_INF_S *plc_inf = HI_NULL;

    if(!rx)
    {
        return HI_ERR_FAILURE;
    }

    dfx = mrsDfxGetModule();

/* BEGIN: Added by liuxipeng/lkf54842 2014/03/29 问题单号: DTS2014032907816*/
    if(msg_proc)
    {
        plc_inf = &dfx->app_rx_tx.plc_msg_proc_inf;
    }
    else
    {
        plc_inf = &dfx->app_rx_tx.plc_msg_send_inf;
    }
/* END  : Added by liuxipeng/lkf54842 2014/03/29 问题单号: DTS2014032907816*/

    plc_inf->rx_err_num++;
    plc_inf->rx_err_time = HI_MDM_GetMilliSeconds();
    plc_inf->rx_err_id = rx->usId;
    memcpy_s(plc_inf->err_src_mac, HI_PLC_MAC_ADDR_LEN, rx->ucSrcMac, HI_PLC_MAC_ADDR_LEN);
    plc_inf->rx_err_cause = err_cause;
    plc_inf->rx_err_len = rx->usPayloadLen;

    (hi_void)memset_s(plc_inf->rx_err_buf, sizeof(plc_inf->rx_buf), 0, sizeof(plc_inf->rx_buf));
    memcpy_s(plc_inf->rx_err_buf, MAX_BUF_LEN,
                   rx->pPayload,
                   MRS_MIN(sizeof(plc_inf->rx_err_buf), rx->usPayloadLen));

/* BEGIN: Added by liuxipeng/lkf54842 2014/03/29 问题单号: DTS2014032907816*/
// DEL
/* END  : Added by liuxipeng/lkf54842 2014/03/29 问题单号: DTS2014032907816*/

    return HI_ERR_SUCCESS;
}

// PLC发送统计
HI_U32 mrsDfxChlPlcTx(HI_DMS_CHL_TX_DATA_S *tx,
                            HI_DMS_CHL_TX_ADDR_S *addr, HI_BOOL msg_proc)
{
    MRS_DFX_MODULE *dfx = HI_NULL;
    PLC_CHL_INF_S *plc_inf = HI_NULL;
    HI_U32 curr_time = 0;

    if (!tx || !addr)
    {
        return HI_ERR_FAILURE;
    }

    dfx = mrsDfxGetModule();

    curr_time = HI_MDM_GetMilliSeconds();

    if(msg_proc)
    {
    /* BEGIN: Added by liuxipeng/lkf54842 2014/03/29 问题单号: DTS2014032907816*/
        plc_inf = &dfx->app_rx_tx.plc_msg_proc_inf;
    /* END  : Added by liuxipeng/lkf54842 2014/03/29 问题单号: DTS2014032907816*/
    }
    else
    {
    /* BEGIN: Added by liuxipeng/lkf54842 2014/03/29 问题单号: DTS2014032907816*/
        plc_inf = &dfx->app_rx_tx.plc_msg_send_inf;
    /* END  : Added by liuxipeng/lkf54842 2014/03/29 问题单号: DTS2014032907816*/
    }

    plc_inf->tx_total_num++;
    plc_inf->tx_time = curr_time;
    plc_inf->tx_id = addr->usId;
    memcpy_s(plc_inf->dest_mac, HI_PLC_MAC_ADDR_LEN, addr->ucDestMac, HI_PLC_MAC_ADDR_LEN);

    plc_inf->tx_len = tx->usDataSize;
    (hi_void)memset_s(plc_inf->tx_buf, sizeof(plc_inf->tx_buf), 0, sizeof(plc_inf->tx_buf));
    memcpy_s(plc_inf->tx_buf, MAX_BUF_LEN,
                    tx->ucData,
                    MRS_MIN(sizeof(plc_inf->tx_buf), tx->usDataSize));

/* BEGIN: Added by liuxipeng/lkf54842 2014/03/29 问题单号: DTS2014032907816*/
// DEL
/* END  : Added by liuxipeng/lkf54842 2014/03/29 问题单号: DTS2014032907816*/

    return HI_ERR_SUCCESS;
}

// PLC发送失败统计
HI_U32 mrsDfxChlPlcErrTx(HI_DMS_CHL_TX_DATA_S *tx, HI_DMS_CHL_TX_ADDR_S *addr,
                                HI_U32 err_cause, HI_BOOL msg_proc)
{
    MRS_DFX_MODULE *dfx = HI_NULL;
    PLC_CHL_INF_S *plc_inf = HI_NULL;

    if (!tx || !addr)
    {
        return HI_ERR_FAILURE;
    }

    dfx = mrsDfxGetModule();

    if(msg_proc)
    {
    /* BEGIN: Added by liuxipeng/lkf54842 2014/03/29 问题单号: DTS2014032907816*/
        plc_inf = &dfx->app_rx_tx.plc_msg_proc_inf;
    /* END  : Added by liuxipeng/lkf54842 2014/03/29 问题单号: DTS2014032907816*/
    }
    else
    {
    /* BEGIN: Added by liuxipeng/lkf54842 2014/03/29 问题单号: DTS2014032907816*/
        plc_inf = &dfx->app_rx_tx.plc_msg_send_inf;
    /* END  : Added by liuxipeng/lkf54842 2014/03/29 问题单号: DTS2014032907816*/
    }

    plc_inf->tx_fail_num++;
    plc_inf->tx_fail_time = HI_MDM_GetMilliSeconds();
    plc_inf->tx_fail_id = addr->usId;
    memcpy_s(plc_inf->fail_dest_mac, HI_PLC_MAC_ADDR_LEN, addr->ucDestMac, HI_PLC_MAC_ADDR_LEN);
    plc_inf->tx_err_cause = err_cause;

    plc_inf->tx_fail_len = tx->usDataSize;
    (hi_void)memset_s(plc_inf->tx_fail_buf, sizeof(plc_inf->tx_fail_buf), 0, sizeof(plc_inf->tx_fail_buf));
    memcpy_s(plc_inf->tx_fail_buf, MAX_BUF_LEN,
                    tx->ucData,
                    MRS_MIN(sizeof(plc_inf->tx_fail_buf), tx->usDataSize));

/* BEGIN: Added by liuxipeng/lkf54842 2014/03/29 问题单号: DTS2014032907816*/
// DEL
/* END  : Added by liuxipeng/lkf54842 2014/03/29 问题单号: DTS2014032907816*/

    return HI_ERR_SUCCESS;
}

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
// CCO接收NDM PLC通道接收统计
// rx 通道接收结构体
HI_U32 mrsDfxChlNdmPlcRx(MRS_PLC_FRAME_DATA_STRU *rx)
{
    MRS_DFX_MODULE *dfx = HI_NULL;
    HI_U32 curr_time = 0;

    if (!rx)
    {
        return HI_ERR_FAILURE;
    }

    dfx = mrsDfxGetModule();

    curr_time = HI_MDM_GetMilliSeconds();
    dfx->app_cco_ndm_plc_rx_tx.rx_total_num++;
    dfx->app_cco_ndm_plc_rx_tx.rx_time = curr_time;
    dfx->app_cco_ndm_plc_rx_tx.rx_id = rx->usId;
    memcpy_s(dfx->app_cco_ndm_plc_rx_tx.src_mac, HI_PLC_MAC_ADDR_LEN, rx->ucMac, HI_PLC_MAC_ADDR_LEN);
    dfx->app_cco_ndm_plc_rx_tx.rx_len = rx->usPayloadLen;

    (hi_void)memset_s(dfx->app_cco_ndm_plc_rx_tx.rx_buf, sizeof(dfx->app_cco_ndm_plc_rx_tx.rx_buf), 0, sizeof(dfx->app_cco_ndm_plc_rx_tx.rx_buf));
    memcpy_s(dfx->app_cco_ndm_plc_rx_tx.rx_buf, MAX_BUF_LEN,
                   rx->pucPayload,
                   MRS_MIN(sizeof(dfx->app_cco_ndm_plc_rx_tx.rx_buf), rx->usPayloadLen));

    return HI_ERR_SUCCESS;
}

// CCO接收NDM PLC通道接收错误统计
// rx 通道接收结构体
// err_cause 错误原因
HI_U32 mrsDfxChlNdmPlcErrRx(MRS_PLC_FRAME_DATA_STRU *rx, HI_U32 err_cause)
{
    MRS_DFX_MODULE *dfx = HI_NULL;

    if(!rx)
    {
        return HI_ERR_FAILURE;
    }

    dfx = mrsDfxGetModule();

    dfx->app_cco_ndm_plc_rx_tx.rx_err_num++;
    dfx->app_cco_ndm_plc_rx_tx.rx_err_time = HI_MDM_GetMilliSeconds();
    dfx->app_cco_ndm_plc_rx_tx.rx_err_id = rx->usId;
    memcpy_s(dfx->app_cco_ndm_plc_rx_tx.err_src_mac, HI_PLC_MAC_ADDR_LEN, rx->ucMac, HI_PLC_MAC_ADDR_LEN);
    dfx->app_cco_ndm_plc_rx_tx.rx_err_cause = err_cause;
    dfx->app_cco_ndm_plc_rx_tx.rx_err_len = rx->usPayloadLen;

    (hi_void)memset_s(dfx->app_cco_ndm_plc_rx_tx.rx_err_buf, sizeof(dfx->app_cco_ndm_plc_rx_tx.rx_buf), 0, sizeof(dfx->app_cco_ndm_plc_rx_tx.rx_buf));
    memcpy_s(dfx->app_cco_ndm_plc_rx_tx.rx_err_buf, MAX_BUF_LEN,
                   rx->pucPayload,
                   MRS_MIN(sizeof(dfx->app_cco_ndm_plc_rx_tx.rx_err_buf), rx->usPayloadLen));

/* BEGIN: Added by liuxipeng/lkf54842 2014/03/29 问题单号: DTS2014032907816*/
// DEL
/* END  : Added by liuxipeng/lkf54842 2014/03/29 问题单号: DTS2014032907816*/

    return HI_ERR_SUCCESS;
}

// CCO发送NDM PLC发送统计
HI_U32 mrsDfxChlNdmPlcTx(MRS_PLC_FRAME_DATA_STRU *tx)
{
    MRS_DFX_MODULE *dfx = HI_NULL;
    HI_U32 curr_time = 0;

    if (!tx)
    {
        return HI_ERR_FAILURE;
    }

    dfx = mrsDfxGetModule();

    curr_time = HI_MDM_GetMilliSeconds();
    dfx->app_cco_ndm_plc_rx_tx.tx_total_num++;
    dfx->app_cco_ndm_plc_rx_tx.tx_time = curr_time;
    dfx->app_cco_ndm_plc_rx_tx.tx_id = tx->usId;
    memcpy_s(dfx->app_cco_ndm_plc_rx_tx.dest_mac, HI_PLC_MAC_ADDR_LEN, tx->ucMac, HI_PLC_MAC_ADDR_LEN);

    dfx->app_cco_ndm_plc_rx_tx.tx_len = tx->usPayloadLen;
    (hi_void)memset_s(dfx->app_cco_ndm_plc_rx_tx.tx_buf, sizeof(dfx->app_cco_ndm_plc_rx_tx.tx_buf), 0, sizeof(dfx->app_cco_ndm_plc_rx_tx.tx_buf));
    memcpy_s(dfx->app_cco_ndm_plc_rx_tx.tx_buf, MAX_BUF_LEN,
                    tx->pucPayload,
                    MRS_MIN(sizeof(dfx->app_cco_ndm_plc_rx_tx.tx_buf), tx->usPayloadLen));

    return HI_ERR_SUCCESS;
}

// CCO发送NDM PLC发送失败统计
HI_U32 mrsDfxChlNdmPlcErrTx(MRS_PLC_FRAME_DATA_STRU *tx, HI_U32 err_cause)
{
    MRS_DFX_MODULE *dfx = HI_NULL;

    if (!tx)
    {
        return HI_ERR_FAILURE;
    }

    dfx = mrsDfxGetModule();

    dfx->app_cco_ndm_plc_rx_tx.tx_fail_num++;
    dfx->app_cco_ndm_plc_rx_tx.tx_fail_time = HI_MDM_GetMilliSeconds();
    dfx->app_cco_ndm_plc_rx_tx.tx_fail_id = tx->usId;
    memcpy_s(dfx->app_cco_ndm_plc_rx_tx.fail_dest_mac, HI_PLC_MAC_ADDR_LEN, tx->ucMac, HI_PLC_MAC_ADDR_LEN);
    dfx->app_cco_ndm_plc_rx_tx.tx_err_cause = err_cause;

    dfx->app_cco_ndm_plc_rx_tx.tx_fail_len = tx->usPayloadLen;
    (hi_void)memset_s(dfx->app_cco_ndm_plc_rx_tx.tx_fail_buf, sizeof(dfx->app_cco_ndm_plc_rx_tx.tx_fail_buf), 0, sizeof(dfx->app_cco_ndm_plc_rx_tx.tx_fail_buf));
    memcpy_s(dfx->app_cco_ndm_plc_rx_tx.tx_fail_buf, MAX_BUF_LEN,
                    tx->pucPayload,
                    MRS_MIN(sizeof(dfx->app_cco_ndm_plc_rx_tx.tx_fail_buf), tx->usPayloadLen));

    return HI_ERR_SUCCESS;
}
#endif

// 向串口统计缓冲区中塞入收到的数据
HI_U32 mrsDfxChlUartRxPut(UART_CHL_INF_S * uart, HI_U8 * data,HI_U16 len)
{
    HI_U16 index;
    HI_U32 tail, max_len;

    if (!uart || !data)
    {
        return HI_ERR_FAILURE;
    }

    tail = uart->rx_buf_tail;
    max_len = sizeof(uart->rx_buf);

    tail %= max_len;

    // 向缓冲区中逐一塞入数据
    for (index = 0;index < len; index++)
    {
        uart->rx_buf[tail++] = data[index];
        tail %= max_len;
    }

    uart->rx_buf_tail = tail;

    return HI_ERR_SUCCESS;
}

// 刷新串口接收统计
HI_U32 mrsDfxChlUartRx(HI_U8 * data, HI_U16 len, HI_BOOL msg_proc)
{
    MRS_DFX_MODULE *dfx = HI_NULL;
    UART_CHL_INF_S *uart_inf = HI_NULL;
    HI_U32 curr_time = 0;

    if ((!data) || (!len))
    {
        return HI_ERR_FAILURE;
    }

    dfx = mrsDfxGetModule();

    curr_time = HI_MDM_GetMilliSeconds();
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) ||defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
    if (msg_proc)
    {
        uart_inf = &dfx->app_rx_tx.uart_msg_proc_inf;
    }
    else
    {
        uart_inf = &dfx->app_rx_tx.uart_msg_send_inf;
    }
#else
    if (!msg_proc)
    {
        uart_inf = &dfx->app_rx_tx.uart_msg_send_inf;
    }
#endif

    /*lint -e774*/
    if (uart_inf == HI_NULL)
    {
        return HI_ERR_FAILURE;
    }
    /*lint +e774*/

    uart_inf->rx_total_len += len;
    uart_inf->rx_last_time = curr_time;
    mrsDfxChlUartRxPut(uart_inf, data, len);

/* BEGIN: Added by liuxipeng/lkf54842 2014/03/29 问题单号: DTS2014032907816*/
// DEL
/* END  : Added by liuxipeng/lkf54842 2014/03/29 问题单号: DTS2014032907816*/
    return HI_ERR_SUCCESS;
}

// 刷新串口接收错误统计
HI_U32 mrsDfxChlUartErrRx(HI_U16 len, HI_U32 err_cause, HI_BOOL msg_proc)
{
    MRS_DFX_MODULE *dfx = HI_NULL;
    UART_CHL_INF_S *uart_inf = HI_NULL;

    if (!len)
    {
        return HI_ERR_FAILURE;
    }

    dfx = mrsDfxGetModule();

#if !defined(PRODUCT_CFG_PRODUCT_TYPE_NDM)
    if(msg_proc)
    {
        uart_inf = &dfx->app_rx_tx.uart_msg_proc_inf;
    }
    else
    {
        uart_inf = &dfx->app_rx_tx.uart_msg_send_inf;
    }
#else
    if(msg_proc == HI_FALSE)
    {
        uart_inf = &dfx->app_rx_tx.uart_msg_send_inf;
    }
#endif

    /*lint -e774*/
    if (uart_inf == HI_NULL)
    {
        return HI_ERR_FAILURE;
    }
    /*lint +e774*/

    uart_inf->rx_err_len += len;
    uart_inf->rx_err_cause = err_cause;

    return HI_ERR_SUCCESS;
}

// 刷新串口接收正确帧统计
HI_VOID mrsDfxChlUartOkFrameCnt(HI_U16 len, HI_BOOL msg_proc)
{
    MRS_DFX_MODULE *dfx = HI_NULL;
    UART_CHL_INF_S *uart_inf = HI_NULL;

    dfx = mrsDfxGetModule();

#if !defined(PRODUCT_CFG_PRODUCT_TYPE_NDM)
    if(msg_proc)
    {
        uart_inf = &dfx->app_rx_tx.uart_msg_proc_inf;
    }
    else
    {
        uart_inf = &dfx->app_rx_tx.uart_msg_send_inf;
    }
#else
    if (!msg_proc)
    {
        uart_inf = &dfx->app_rx_tx.uart_msg_send_inf;
    }
#endif

    /*lint -e774*/
    if (uart_inf == HI_NULL)
    {
        return;
    }
    /*lint +e774*/

    uart_inf->rx_ok_len += len;
    uart_inf->rx_ok_num++;

/* BEGIN: Added by liuxipeng/lkf54842 2014/03/29 问题单号: DTS2014032907816*/
// DEL
/* END  : Added by liuxipeng/lkf54842 2014/03/29 问题单号: DTS2014032907816*/
    return;
}

// 刷新串口接收错误帧统计
HI_VOID mrsDfxChlUartErrFrameCnt(HI_BOOL msg_proc)
{
    MRS_DFX_MODULE *dfx = HI_NULL;
    UART_CHL_INF_S *uart_inf = HI_NULL;

    dfx = mrsDfxGetModule();

/* BEGIN: Added by liuxipeng/lkf54842 2014/03/29 问题单号: DTS2014032907816*/
#if !defined(PRODUCT_CFG_PRODUCT_TYPE_NDM)
    if(msg_proc)
    {
        uart_inf = &dfx->app_rx_tx.uart_msg_proc_inf;
    }
    else
    {
        uart_inf = &dfx->app_rx_tx.uart_msg_send_inf;
    }
#else
    if (!msg_proc)
    {
        uart_inf = &dfx->app_rx_tx.uart_msg_send_inf;
    }
#endif

/* END  : Added by liuxipeng/lkf54842 2014/03/29 问题单号: DTS2014032907816*/
    /*lint -e774*/
    if (uart_inf == HI_NULL)
    {
        return;
    }
    /*lint +e774*/

    uart_inf->rx_err_num++;

/* BEGIN: Added by liuxipeng/lkf54842 2014/03/29 问题单号: DTS2014032907816*/
// DEL
/* END  : Added by liuxipeng/lkf54842 2014/03/29 问题单号: DTS2014032907816*/

    return;
}

// 串口发送统计(消息发送)
HI_U32 mrsDfxChlUartTxMsgTx(HI_U8 *data, HI_U16 len)
{
    MRS_DFX_MODULE *dfx = HI_NULL;
    UART_CHL_INF_S *uart_inf = HI_NULL;
    HI_U32 curr_time = 0;

    if (!data || !len)
    {
        return HI_ERR_FAILURE;
    }

    dfx = mrsDfxGetModule();

    curr_time = HI_MDM_GetMilliSeconds();

/* BEGIN: Added by liuxipeng/lkf54842 2014/03/29 问题单号: DTS2014032907816*/
    uart_inf = &dfx->app_rx_tx.uart_msg_send_inf;
/* END  : Added by liuxipeng/lkf54842 2014/03/29 问题单号: DTS2014032907816*/

    /*lint -e774*/
    if (uart_inf == HI_NULL)
    {
        return HI_ERR_FAILURE;
    }
    /*lint +e774*/

    uart_inf->tx_total_len += len;
    uart_inf->tx_last_time = curr_time;
    uart_inf->tx_total_num++;
    uart_inf->tx_last_len = len;

    (hi_void)memset_s(uart_inf->tx_buf, sizeof(uart_inf->tx_buf), 0, sizeof(uart_inf->tx_buf));
    memcpy_s(uart_inf->tx_buf, MAX_BUF_LEN,
                   data,
                   MRS_MIN(sizeof(uart_inf->tx_buf),len));

/* BEGIN: Added by liuxipeng/lkf54842 2014/03/29 问题单号: DTS2014032907816*/
// DEL
/* END  : Added by liuxipeng/lkf54842 2014/03/29 问题单号: DTS2014032907816*/
    return HI_ERR_SUCCESS;
}

// 串口发送统计(消息处理)
HI_U32 mrsDfxChlUartTxMsgRx(HI_DMS_CHL_TX_DATA_S *tx)
{
    MRS_DFX_MODULE *dfx = HI_NULL;
    UART_CHL_INF_S *uart_inf = HI_NULL;
    HI_U32 curr_time = 0;

    if (!tx)
    {
        return HI_ERR_FAILURE;
    }

    dfx = mrsDfxGetModule();

    curr_time = HI_MDM_GetMilliSeconds();

/* BEGIN: Added by liuxipeng/lkf54842 2014/03/29 问题单号: DTS2014032907816*/
    uart_inf = &dfx->app_rx_tx.uart_msg_proc_inf;
/* END  : Added by liuxipeng/lkf54842 2014/03/29 问题单号: DTS2014032907816*/
    /*lint -e774*/
    if (uart_inf == HI_NULL)
    {
        return HI_ERR_FAILURE;
    }
    /*lint +e774*/

    uart_inf->tx_total_len += tx->usDataSize;
    uart_inf->tx_last_time = curr_time;
    uart_inf->tx_total_num++;
    uart_inf->tx_last_len = tx->usDataSize;

    (hi_void)memset_s(uart_inf->tx_buf, sizeof(uart_inf->tx_buf), 0, sizeof(uart_inf->tx_buf));
    memcpy_s(uart_inf->tx_buf, MAX_BUF_LEN,
                   tx->ucData,
                   MRS_MIN(sizeof(uart_inf->tx_buf),tx->usDataSize));

/* BEGIN: Added by liuxipeng/lkf54842 2014/03/29 问题单号: DTS2014032907816*/
// DEL
/* END  : Added by liuxipeng/lkf54842 2014/03/29 问题单号: DTS2014032907816*/
    return HI_ERR_SUCCESS;
}

// 串口发送成功统计(消息发送)
HI_U32 mrsDfxChlUartOkTxMsgTx(HI_U16 len)
{
    MRS_DFX_MODULE *dfx = HI_NULL;
    UART_CHL_INF_S *uart_inf = HI_NULL;

    if (!len)
    {
        return HI_ERR_FAILURE;
    }

    dfx = mrsDfxGetModule();

/* BEGIN: Added by liuxipeng/lkf54842 2014/03/29 问题单号: DTS2014032907816*/
    uart_inf = &dfx->app_rx_tx.uart_msg_send_inf;
/* END  : Added by liuxipeng/lkf54842 2014/03/29 问题单号: DTS2014032907816*/

    uart_inf->tx_ok_len += len;
    uart_inf->tx_ok_num++;

/* BEGIN: Added by liuxipeng/lkf54842 2014/03/29 问题单号: DTS2014032907816*/
// DEL
/* END  : Added by liuxipeng/lkf54842 2014/03/29 问题单号: DTS2014032907816*/
    return HI_ERR_SUCCESS;
}

// 串口发送成功统计(消息处理)
HI_U32 mrsDfxChlUartOkTxMsgRx(HI_DMS_CHL_TX_DATA_S *tx)
{
    MRS_DFX_MODULE *dfx = HI_NULL;
    UART_CHL_INF_S *uart_inf = HI_NULL;

    if (!tx)
    {
        return HI_ERR_FAILURE;
    }

    dfx = mrsDfxGetModule();

    uart_inf = &dfx->app_rx_tx.uart_msg_proc_inf;
    /*lint -e774*/
    if (uart_inf == HI_NULL)
    {
        return HI_ERR_FAILURE;
    }
    /*lint +e774*/

    uart_inf->tx_ok_len += tx->usDataSize;
    uart_inf->tx_ok_num++;

    return HI_ERR_SUCCESS;
}


// 串口发送失败统计(消息发送)
HI_U32 mrsDfxChlUartErrTxMsgTx(HI_U16 len, HI_U32 err_cause)
{
    MRS_DFX_MODULE *dfx = HI_NULL;
    UART_CHL_INF_S *uart_inf = HI_NULL;

    if (!len)
    {
        return HI_ERR_FAILURE;
    }

    dfx = mrsDfxGetModule();

    /* BEGIN: Added by liuxipeng/lkf54842 2014/03/29 问题单号: DTS2014032907816*/
    uart_inf = &dfx->app_rx_tx.uart_msg_send_inf;
/* END  : Added by liuxipeng/lkf54842 2014/03/29 问题单号: DTS2014032907816*/
    /*lint -e774*/
    if (uart_inf == HI_NULL)
    {
        return HI_ERR_FAILURE;
    }
    /*lint +e774*/

    uart_inf->tx_fail_len += len;
    uart_inf->tx_fail_num++;
    uart_inf->tx_err_cause = err_cause;

/* BEGIN: Added by liuxipeng/lkf54842 2014/03/29 问题单号: DTS2014032907816*/
// DEL
/* END  : Added by liuxipeng/lkf54842 2014/03/29 问题单号: DTS2014032907816*/
    return HI_ERR_SUCCESS;
}

// 串口发送失败统计(消息处理)
HI_U32 mrsDfxChlUartErrTxMsgRx(HI_DMS_CHL_TX_DATA_S *tx, HI_U32 err_cause)
{
    MRS_DFX_MODULE *dfx = HI_NULL;
    UART_CHL_INF_S *uart_inf = HI_NULL;

    if (!tx)
    {
        return HI_ERR_FAILURE;
    }

    dfx = mrsDfxGetModule();

/* BEGIN: Added by liuxipeng/lkf54842 2014/03/29 问题单号: DTS2014032907816*/
    uart_inf = &dfx->app_rx_tx.uart_msg_proc_inf;
/* END  : Added by liuxipeng/lkf54842 2014/03/29 问题单号: DTS2014032907816*/

    uart_inf->tx_fail_len += tx->usDataSize;
    uart_inf->tx_fail_num++;
    uart_inf->tx_err_cause = err_cause;

/* BEGIN: Added by liuxipeng/lkf54842 2014/03/29 问题单号: DTS2014032907816*/
// DEL
/* END  : Added by liuxipeng/lkf54842 2014/03/29 问题单号: DTS2014032907816*/
    return HI_ERR_SUCCESS;
}


#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
// 心跳统计
HI_U32 mrsDfxHeartCnt(HI_VOID)
{
    HI_DSID_APP_HEARTBEAT_INF_S * app_heart = HI_NULL;
    MRS_DFX_MODULE *dfx = HI_NULL;
    HI_U32 curr_time = 0;

    dfx = mrsDfxGetModule();
    app_heart = &dfx->app_heart;

    curr_time = HI_MDM_GetMilliSeconds();

    app_heart->heart_num++;
    mrsDfxRefreshLastDataU32(app_heart->last_time, DFX_ALEN(app_heart->last_time), curr_time);

    return HI_ERR_SUCCESS;
}

// 心跳超时统计
HI_U32 mrsDfxHeartTimeoutCnt(HI_VOID)
{
    HI_DSID_APP_HEARTBEAT_INF_S * app_heart = HI_NULL;
    MRS_DFX_MODULE *dfx = HI_NULL;
    HI_U32 curr_time = 0;

    dfx = mrsDfxGetModule();
    app_heart = &dfx->app_heart;

    curr_time = HI_MDM_GetMilliSeconds();

    app_heart->timeout_num++;
    mrsDfxRefreshLastDataU32(app_heart->timeout_time, DFX_ALEN(app_heart->timeout_time), curr_time);

    return HI_ERR_SUCCESS;
}
#endif


#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
// CCO运行状态更新
HI_U32 mrsDfxCcoStatus(HI_U8 status)
{
    HI_DSID_APP_CCO_RUN_STATUS_S * app_cco_status = HI_NULL;
    MRS_DFX_MODULE *dfx = HI_NULL;

    dfx = mrsDfxGetModule();
    app_cco_status = &dfx->app_cco_status;

    mrsDfxRefreshLastDataU8(app_cco_status->status, sizeof(app_cco_status->status), status);
    mrsDfxRefreshLastDataU32(app_cco_status->last_time, DFX_ALEN(app_cco_status->last_time), HI_MDM_GetMilliSeconds());

    return HI_ERR_SUCCESS;
}

// Map维护完成时标刷新
HI_U32 mrsDfxMapReadyTime(HI_VOID)
{
    HI_DSID_APP_CCO_RUN_STATUS_S * app_cco_status = HI_NULL;
    MRS_DFX_MODULE *dfx = HI_NULL;
    HI_U32 time = 0;

    dfx = mrsDfxGetModule();
    app_cco_status = &dfx->app_cco_status;

    time = HI_MDM_GetMilliSeconds();

    if(app_cco_status->map_ready_time != 0)
    {
        app_cco_status->map_last_time = time;
    }
    else
    {
        app_cco_status->map_ready_time = time;
        app_cco_status->map_last_time = time;
    }

    return HI_ERR_SUCCESS;
}
#endif

// ************************
// ***读表相关统计量刷新***
// ************************
#define mrsDfxGetRmInf()     (&dfx_module.app_rm_inf)
#define mrsDfxGetRmPoolInf() (dfx_module.app_rm_inf.stPoolInf)
#define mrsDfxGetLrInf()     (&dfx_module.app_rm_inf.LR_inf)
#define mrsDfxGetXrInf()     (&dfx_module.app_rm_inf.XR_inf)
#define mrsDfxGetPrInf()     (&dfx_module.app_rm_inf.PR_inf)

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
HI_VOID mrsDfxRmPlcSeq(HI_U32 ulSeq)
{
    HI_DSID_APP_RM_INF_S *dfx = mrsDfxGetRmInf();
    dfx->ulPlcSeq = ulSeq;
}

HI_VOID mrsDfxRmTimerStatus(HI_U8 ucStatus)
{
    HI_DSID_APP_RM_INF_S *dfx = mrsDfxGetRmInf();
    dfx->ulTimerStatus = ucStatus;
}

HI_VOID mrsDfxRmPoolId(HI_U32 ulId, HI_U8 ucPoolNo)
{
    APP_RM_POOL_INF_S *dfx = mrsDfxGetRmPoolInf();
    dfx[ucPoolNo].ulPoolId = ulId;
}

HI_VOID mrsDfxRmPoolMode(HI_U32 ulId, HI_U8 ucMode)
{
    APP_RM_POOL_INF_S *dfx = mrsDfxGetRmPoolInf();

    if(ulId != dfx->ulPoolId)
    {
        dfx++;
    }
    dfx->ulPoolMode = ucMode;
}

HI_VOID mrsDfxRmPalallelNum(HI_U32 ulId, HI_U16 usNum)
{
    APP_RM_POOL_INF_S *dfx = mrsDfxGetRmPoolInf();

    if(ulId != dfx->ulPoolId)
    {
        dfx++;
    }
    dfx->ulParallelNum = usNum;
}

HI_VOID mrsDfxRmItemNum(HI_U32 ulId, HI_U16 usNum)
{
    APP_RM_POOL_INF_S *dfx = mrsDfxGetRmPoolInf();

    if(ulId != dfx->ulPoolId)
    {
        dfx++;
    }
    dfx->ulItemNum = usNum;
}

HI_VOID mrsDfxRmJoinFail(HI_U32 ulId, HI_U32 ret)
{
    APP_RM_POOL_INF_S *dfx = mrsDfxGetRmPoolInf();

    if(ulId != dfx->ulPoolId)
    {
        dfx++;
    }
    dfx->ulJoinFailCnt++;
    mrsDfxRefreshLastDataU32(dfx->ulJoinFailCause, DFX_ALEN(dfx->ulJoinFailCause), ret);
}

HI_VOID mrsDfxLrRoute(HI_U8 state)
{
    APP_LR_INF_S *dfx = mrsDfxGetLrInf();
    dfx->route = state;
}

HI_VOID mrsDfxLrState(HI_U8 state)
{
    APP_LR_INF_S *dfx = mrsDfxGetLrInf();
    dfx->LR_status = state;
}

HI_VOID mrsDfxLrMeterNum(HI_U16 num)
{
    APP_LR_INF_S *dfx = mrsDfxGetLrInf();
    dfx->meter_num = num;
}

HI_VOID mrsDfxLrReqCnt(HI_VOID)
{
    APP_LR_INF_S *dfx = mrsDfxGetLrInf();
    dfx->LR_req_cnt++;

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) && defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG)
    //mrsLogLrprComTx(SDM_MRS_MR_STATS_TYPE_LR);//optimized by weichao
#endif
}

HI_VOID mrsDfxLrAckCnt(HI_VOID)
{
    APP_LR_INF_S *dfx = mrsDfxGetLrInf();
    dfx->LR_ack_cnt++;

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) && defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG)
    //mrsLogLrprComRx(SDM_MRS_MR_STATS_TYPE_LR);//optimized by weichao
#endif
}
#endif

HI_VOID mrsDfxLrPlcTx(HI_VOID)
{
    APP_LR_INF_S *dfx = mrsDfxGetLrInf();
    dfx->LR_plc_tx++;

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) && defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG)
    //mrsLogLrprPlcTx(SDM_MRS_MR_STATS_TYPE_LR);//optimized by weichao
#endif
}

HI_VOID mrsDfxLrPlcRx(HI_VOID)
{
    APP_LR_INF_S *dfx = mrsDfxGetLrInf();
    dfx->LR_plc_rx++;

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) && defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG)
    //mrsLogLrprPlcRx(SDM_MRS_MR_STATS_TYPE_LR);//optimized by weichao
#endif
}

HI_VOID mrsDfxLrMeterTx(HI_VOID)
{
    APP_LR_INF_S *dfx = mrsDfxGetLrInf();
    dfx->LR_meter_tx++;
}

HI_VOID mrsDfxLrMeterRx(HI_VOID)
{
    APP_LR_INF_S *dfx = mrsDfxGetLrInf();
    dfx->LR_meter_rx++;
}

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
HI_VOID mrsDfxLrPlcTryCnt(HI_VOID)
{
    APP_LR_INF_S *dfx = mrsDfxGetLrInf();
    dfx->LR_plc_try_cnt++;
}

HI_VOID mrsDfxLrPlcSeqErr(HI_VOID)
{
    APP_LR_INF_S *dfx = mrsDfxGetLrInf();
    dfx->LR_plc_seq_err++;
}
#endif

HI_VOID mrsDfxLrPlcLenErr(HI_VOID)
{
    APP_LR_INF_S *dfx = mrsDfxGetLrInf();
    dfx->LR_plc_len_err++;
}

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
HI_VOID mrsDfxLrPlcExceedCnt(HI_VOID)
{
    APP_LR_INF_S *dfx = mrsDfxGetLrInf();
    dfx->LR_plc_exceed_cnt++;
}

HI_VOID mrsDfxLrPlcTimeout(HI_VOID)
{
    APP_LR_INF_S *dfx = mrsDfxGetLrInf();
    dfx->LR_plc_timeout++;
}

HI_VOID mrsDfxLrUpDataCnt(HI_VOID)
{
    APP_LR_INF_S *dfx = mrsDfxGetLrInf();
    dfx->LR_up_data_cnt++;

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) && defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG)
    //mrsLogLrprComTx(SDM_MRS_MR_STATS_TYPE_LR);//optimized by weichao
#endif
}

HI_VOID mrsDfxLrUpDataTryCnt(HI_VOID)
{
    APP_LR_INF_S *dfx = mrsDfxGetLrInf();
    dfx->LR_up_data_try_cnt++;
}

HI_VOID mrsDfxLrOkCnt(HI_VOID)
{
    APP_LR_INF_S *dfx = mrsDfxGetLrInf();
    dfx->LR_ok_cnt++;

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) && defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG)
    //mrsLogLrprComRx(SDM_MRS_MR_STATS_TYPE_LR);//optimized by weichao
#endif
}

HI_VOID mrsDfxLrFailCnt(HI_VOID)
{
    APP_LR_INF_S *dfx = mrsDfxGetLrInf();
    dfx->LR_fail_cnt++;
}

// 随抄统计
HI_VOID mrsDfxXrStatus(HI_U8 state)
{
    APP_XR_INF_S *dfx = mrsDfxGetXrInf();
    dfx->XR_status = state;
}
#endif

HI_VOID mrsDfxXrUartTx(HI_VOID)
{
    APP_XR_INF_S *dfx = mrsDfxGetXrInf();
    dfx->XR_uart_tx++;

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) && defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG)
    //mrsLogXrComTx();//optimized by weichao
#endif
}

HI_VOID mrsDfxXrUartRx(HI_VOID)
{
    APP_XR_INF_S *dfx = mrsDfxGetXrInf();
    dfx->XR_uart_rx++;

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) && defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG)
    //mrsLogXrComRx(); //optimized by weichao
#endif
}

HI_VOID mrsDfxXrPlcTx(HI_VOID)
{
    APP_XR_INF_S *dfx = mrsDfxGetXrInf();
    dfx->XR_plc_tx++;

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) && defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG)
    //mrsLogXrPlcTx();//optimized by weichao
#endif
}

HI_VOID mrsDfxXrPlcRx(HI_VOID)
{
    APP_XR_INF_S *dfx = mrsDfxGetXrInf();
    dfx->XR_plc_rx++;

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) && defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG)
    //mrsLogXrPlcRx();//optimized by weichao
#endif
}

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
HI_VOID mrsDfxXrNakCnt(HI_VOID)
{
    APP_XR_INF_S *dfx = mrsDfxGetXrInf();
    dfx->XR_nak_num++;
}

HI_VOID mrsDfxXrNak(HI_U8 nak)
{
    APP_XR_INF_S *dfx = mrsDfxGetXrInf();
    mrsDfxRefreshLastDataU8(dfx->XR_nak, sizeof(dfx->XR_nak), nak);
}

HI_VOID mrsDfxXrPlcTryCnt(HI_VOID)
{
    APP_XR_INF_S *dfx = mrsDfxGetXrInf();
    dfx->XR_plc_try_cnt++;
}

HI_VOID mrsDfxXrPlcSeqErr(HI_VOID)
{
    APP_XR_INF_S *dfx = mrsDfxGetXrInf();
    dfx->XR_plc_seq_err++;
}
#endif

HI_VOID mrsDfxXrPlcLenErr(HI_VOID)
{
    APP_XR_INF_S *dfx = mrsDfxGetXrInf();
    dfx->XR_plc_len_err++;
}

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
HI_VOID mrsDfxXrPlcExceedCnt(HI_VOID)
{
    APP_XR_INF_S *dfx = mrsDfxGetXrInf();
    dfx->XR_plc_exceed_cnt++;
}

HI_VOID mrsDfxXrPlcTimeout(HI_VOID)
{
    APP_XR_INF_S *dfx = mrsDfxGetXrInf();
    dfx->XR_plc_timeout++;
}

HI_VOID mrsDfxXrOkCnt(HI_VOID)
{
    APP_XR_INF_S *dfx = mrsDfxGetXrInf();
    dfx->XR_ok_cnt++;
}

HI_VOID mrsDfxXrFailCnt(HI_VOID)
{
    APP_XR_INF_S *dfx = mrsDfxGetXrInf();
    dfx->XR_fail_cnt++;
}
#endif


// 并发统计
HI_VOID mrsDfxPrUartTx(HI_VOID)
{
    APP_PR_INF_S *dfx = mrsDfxGetPrInf();
    dfx->PR_uart_tx++;

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) && defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG)
    //mrsLogLrprComTx(SDM_MRS_MR_STATS_TYPE_PR);//optimized by weichao
#endif
}

HI_VOID mrsDfxPrUartRx(HI_VOID)
{
    APP_PR_INF_S *dfx = mrsDfxGetPrInf();
    dfx->PR_uart_rx++;

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) && defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG)
    //mrsLogLrprComRx(SDM_MRS_MR_STATS_TYPE_PR);//optimized by weichao
#endif
}

HI_VOID mrsDfxPrPlcTx(HI_VOID)
{
    APP_PR_INF_S *dfx = mrsDfxGetPrInf();
    dfx->PR_plc_tx++;

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) && defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG)
    //mrsLogLrprPlcTx(SDM_MRS_MR_STATS_TYPE_PR);//optimized by weichao
#endif
}

HI_VOID mrsDfxPrPlcRx(HI_VOID)
{
    APP_PR_INF_S *dfx = mrsDfxGetPrInf();
    dfx->PR_plc_rx++;

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) && defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG)
    //mrsLogLrprPlcRx(SDM_MRS_MR_STATS_TYPE_PR);//optimized by weichao
#endif
}

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
HI_VOID mrsDfxPrNakCnt(HI_U8 nak)
{
    APP_PR_INF_S *dfx = mrsDfxGetPrInf();

    dfx->PR_nak_num++;
    mrsDfxRefreshLastDataU8(dfx->PR_nak, sizeof(dfx->PR_nak), nak);
}

HI_VOID mrsDfxPrPlcTryCnt(HI_VOID)
{
    APP_PR_INF_S *dfx = mrsDfxGetPrInf();
    dfx->PR_plc_try_cnt++;
}

HI_VOID mrsDfxPrPlcSeqErr(HI_VOID)
{
    APP_PR_INF_S *dfx = mrsDfxGetPrInf();
    dfx->PR_plc_seq_err++;
}
#endif

HI_VOID mrsDfxPrPlcLenErr(HI_VOID)
{
    APP_PR_INF_S *dfx = mrsDfxGetPrInf();
    dfx->PR_plc_len_err++;
}

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
HI_VOID mrsDfxPrPlcTimeout(HI_VOID)
{
    APP_PR_INF_S *dfx = mrsDfxGetPrInf();
    dfx->PR_plc_timeout++;
}

HI_VOID mrsDfxPrOkCnt(HI_VOID)
{
    APP_PR_INF_S *dfx = mrsDfxGetPrInf();
    dfx->PR_ok_cnt++;
}

HI_VOID mrsDfxPrFailCnt(HI_VOID)
{
    APP_PR_INF_S *dfx = mrsDfxGetPrInf();
    dfx->PR_fail_cnt++;
}
#endif

// ************************
// ********E***N***D*******
// ************************

// 模拟通道统计
HI_VOID mrsDfxVmChlStatus(HI_U8 state)
{
    MRS_DFX_MODULE *dfx = HI_NULL;
    HI_DSID_APP_VM_CHL_INF_S *inf = HI_NULL;
    HI_U32 curr_time = 0;

    dfx = mrsDfxGetModule();
    inf = &dfx->app_vm_chl_inf;

    curr_time = HI_MDM_GetMilliSeconds();

    inf->change_num++;
    mrsDfxRefreshLastDataU8(inf->status, sizeof(inf->status), state? CHL_OK : CHL_NC);
    mrsDfxRefreshLastDataU32(inf->last_time, DFX_ALEN(inf->last_time), curr_time);

    return;
}

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)

#define mrsGetStaStatus() (&dfx_module.app_sta_status)

// STA状态
HI_VOID mrsDfxStaStatus(HI_U8 state)
{
    HI_DSID_APP_STA_STATUS_S * dfx = mrsGetStaStatus();

    dfx->status = state;
    dfx->state_time = HI_MDM_GetMilliSeconds();

    return;
}

// 虚拟表状态统计
HI_VOID mrsDfxVmMeterStatus(HI_U8 state, HI_BOOL notify_flg)
{
    HI_DSID_APP_STA_STATUS_S * dfx = mrsGetStaStatus();

    if(notify_flg)
    {
        dfx->vm_chg_num++;
        mrsDfxRefreshLastDataU8(dfx->vm_flg, sizeof(dfx->vm_flg), state);
        mrsDfxRefreshLastDataU32(dfx->vm_time, DFX_ALEN(dfx->vm_time), HI_MDM_GetMilliSeconds());
    }
    else
    {
        dfx->vm_flg[0] = state;
        dfx->vm_time[0] = HI_MDM_GetMilliSeconds();
    }

    return;
}

// 主动上报标志状态统计
HI_VOID mrsDfxReportStatus(HI_U8 state)
{
    HI_DSID_APP_STA_STATUS_S * dfx = mrsGetStaStatus();

    mrsDfxRefreshLastDataU8(dfx->report_flg, sizeof(dfx->report_flg), state);
    mrsDfxRefreshLastDataU32(dfx->report_time, DFX_ALEN(dfx->report_time), HI_MDM_GetMilliSeconds());

    return;
}

// 改变上报标志状态统计
HI_VOID mrsDfxChangeStatus(HI_U8 state)
{
    HI_DSID_APP_STA_STATUS_S * dfx = mrsGetStaStatus();

    mrsDfxRefreshLastDataU8(dfx->change_flg, sizeof(dfx->change_flg), state);
    mrsDfxRefreshLastDataU32(dfx->change_time, DFX_ALEN(dfx->change_time), HI_MDM_GetMilliSeconds());

    return;
}
#endif

/* BEGIN: Added by liuxipeng/lkf54842 2014/03/29 问题单号: DTS2014032907816*/
// DEL
/* END  : Added by liuxipeng/lkf54842 2014/03/29 问题单号: DTS2014032907816*/

// 更新时间
HI_VOID RefreshTime(HI_DSID_APP_TOPO_CHANGE_INF_S * topo_change, HI_U32 time)
{
    if (topo_change->first_time != 0)
    {
        if ((time - topo_change->last_time[0]) > topo_change->max_interval)
        {
            topo_change->max_interval = time - topo_change->last_time[0];
        }

        if ((time - topo_change->last_time[0]) < topo_change->min_interval)
        {
            topo_change->min_interval = time - topo_change->last_time[0];
        }
    }
    else
    {
        topo_change->first_time = time;
        topo_change->max_interval = 0;
        topo_change->min_interval = (HI_U32)(~0);
    }
}

//通道改变
HI_U32 mrsDfxChlChangeTrue(HI_U32 time)
{
    MRS_DFX_MODULE *dfx;
    HI_DSID_APP_TOPO_CHANGE_INF_S * topo_change;

    dfx = mrsDfxGetModule();
    topo_change = &dfx->app_topo_change;

    mrsDfxRefreshLastDataU8(topo_change->chl_status, sizeof(topo_change->chl_status), CHL_OK);
    topo_change->change_num++;
    topo_change->chl_change_true++;

    RefreshTime(topo_change, time);

    mrsDfxRefreshLastDataU32(topo_change->last_time, DFX_ALEN(topo_change->last_time), time);
    mrsDfxRefreshLastDataU32(topo_change->last_true_time, DFX_ALEN(topo_change->last_true_time), time);

    return HI_ERR_SUCCESS;
}

HI_U32 mrsDfxChlChangeFalse(HI_U32 time)
{
    MRS_DFX_MODULE *dfx;
    HI_DSID_APP_TOPO_CHANGE_INF_S * topo_change;

    dfx = mrsDfxGetModule();
    topo_change = &dfx->app_topo_change;

    mrsDfxRefreshLastDataU8(topo_change->chl_status, sizeof(topo_change->chl_status), CHL_NC);
    topo_change->change_num++;
    topo_change->chl_change_false++;

    RefreshTime(topo_change, time);

    mrsDfxRefreshLastDataU32(topo_change->last_time, DFX_ALEN(topo_change->last_time), time);
    mrsDfxRefreshLastDataU32(topo_change->last_false_time, DFX_ALEN(topo_change->last_false_time), time);

    return HI_ERR_SUCCESS;
}

HI_U32 mrsDfxChlChangeMsgFail(HI_U32 cause)
{
    MRS_DFX_MODULE *dfx;
    HI_DSID_APP_TOPO_CHANGE_INF_S * topo_change;

    dfx = mrsDfxGetModule();
    topo_change = &dfx->app_topo_change;

    topo_change->msg_tx_fail_num++;

    mrsDfxRefreshLastDataU32(topo_change->msg_fail_cause, DFX_ALEN(topo_change->msg_fail_cause), cause);
    mrsDfxRefreshLastDataU32(topo_change->msg_fail_time, DFX_ALEN(topo_change->msg_fail_time), HI_MDM_GetMilliSeconds());

    return HI_ERR_SUCCESS;
}

#define mrsDfxGetTaskMsgInf() (&dfx_module.app_task_msg_inf)
#define mrsDfxGetQueueInf()   (&dfx_module.app_rm_queue_inf)
#define mrsDfxGetTimerInf()   (&dfx_module.app_timer_inf)

// ******************************
// 应用任务消息队列统计
// ******************************

// 刷新队列消息个数统计
HI_VOID mrsDfxRefreshMsgQueueNum(HI_VOID)
{
    HI_DSID_APP_MSG_INF_S * dfx = HI_NULL;
    HI_U32 q_num = 0;

    dfx = mrsDfxGetTaskMsgInf();

    q_num = MRS_GetQueueMsgNum(EN_APP_QUEUE_MRS);

    dfx->curr_msg_num = q_num;
    if(q_num > dfx->max_msg_num)
    {
        HI_U32 curr_time = HI_MDM_GetMilliSeconds();

        dfx->max_msg_num = q_num;
        dfx->max_mag_num_time = curr_time;
    }

    return;
}

// 消息队列发送统计
HI_VOID mrsDfxMsgTxCnt(HI_U32 id)
{
    HI_DSID_APP_MSG_INF_S * dfx = HI_NULL;
    HI_U32 curr_time = 0;

    dfx = mrsDfxGetTaskMsgInf();

    curr_time = HI_MDM_GetMilliSeconds();

    dfx->total_tx_cnt++;

    if(id != ID_MRS_MSG_MR_TIMER)
    {
        dfx->tx_no_timer_cnt++;
    }

    mrsDfxRefreshLastDataU32(dfx->last_tx_id, DFX_ALEN(dfx->last_tx_id), id);
    dfx->last_tx_time = curr_time;

    return;
}

// 消息队列发送失败统计
HI_VOID mrsDfxMsgTxFailCnt(HI_U32 id)
{
    HI_DSID_APP_MSG_INF_S * dfx = HI_NULL;
    HI_U32 curr_time = 0;

    dfx = mrsDfxGetTaskMsgInf();

    curr_time = HI_MDM_GetMilliSeconds();

    dfx->tx_fail_cnt++;

    mrsDfxRefreshLastDataU32(dfx->last_fail_id, DFX_ALEN(dfx->last_fail_id), id);
    mrsDfxRefreshLastDataU32(dfx->last_fail_time,
                            DFX_ALEN(dfx->last_fail_time),
                            curr_time);

    return;
}

#define MRS_MSG_DLY_LIMIT_1 20            // 消息处理延时门限20ms
#define MRS_MSG_DLY_LIMIT_2 50            // 消息处理延时门限50ms
#define MRS_MSG_DLY_LIMIT_3 100           // 消息处理延时门限100ms
#define MRS_MSG_DLY_LIMIT_4 200           // 消息处理延时门限200ms
#define MRS_MSG_DLY_LIMIT_5 300           // 消息处理延时门限300ms
#define MRS_MSG_DLY_LIMIT_6 500           // 消息处理延时门限500ms
#define MRS_MSG_DLY_LIMIT_7 1000          // 消息处理延时门限1000ms
#define MRS_MSG_DLY_LIMIT_8 2000          // 消息处理延时门限2000ms
#define MRS_MSG_DLY_LIMIT_9 (5 * 1000)    // 消息处理延时门限5s
#define MRS_MSG_DLY_LIMIT_10 (10 * 1000)  // 消息处理延时门限10s
#define MRS_MSG_DLY_LIMIT_11 (30 * 1000)  // 消息处理延时门限30s

// 消息处理延时分级统计
HI_PRV HI_VOID mrsDfxMsgDelayCnt(HI_U32 delay_time,
                                       APP_MSG_DLY_LEVEL_CNT_S * delay_inf)
{
    if(delay_time > MRS_MSG_DLY_LIMIT_11)
    {
        delay_inf->delay_30000ms_cnt++;
    }
    else if(delay_time > MRS_MSG_DLY_LIMIT_10)
    {
        delay_inf->delay_10000ms_cnt++;
    }
    else if(delay_time > MRS_MSG_DLY_LIMIT_9)
    {
        delay_inf->delay_5000ms_cnt++;
    }
    else if(delay_time > MRS_MSG_DLY_LIMIT_8)
    {
        delay_inf->delay_2000ms_cnt++;
    }
    else if(delay_time > MRS_MSG_DLY_LIMIT_7)
    {
        delay_inf->delay_1000ms_cnt++;
    }
    else if(delay_time > MRS_MSG_DLY_LIMIT_6)
    {
        delay_inf->delay_500ms_cnt++;
    }
    else if(delay_time > MRS_MSG_DLY_LIMIT_5)
    {
        delay_inf->delay_300ms_cnt++;
    }
    else if(delay_time > MRS_MSG_DLY_LIMIT_4)
    {
        delay_inf->delay_200ms_cnt++;
    }
    else if(delay_time > MRS_MSG_DLY_LIMIT_3)
    {
        delay_inf->delay_100ms_cnt++;
    }
    else if(delay_time > MRS_MSG_DLY_LIMIT_2)
    {
        delay_inf->delay_50ms_cnt++;
    }
    else if(delay_time > MRS_MSG_DLY_LIMIT_1)
    {
        delay_inf->delay_20ms_cnt++;
    }
    else
    {
        delay_inf->other_cnt++;
    }

    return;
}

// 消息队列接收统计
HI_VOID mrsDfxMsgRxCnt(HI_SYS_QUEUE_MSG_S * msg)
{
    HI_DSID_APP_MSG_INF_S * dfx = HI_NULL;
    HI_U32 curr_time = 0;
    HI_U32 delay_time = 0;

    dfx = mrsDfxGetTaskMsgInf();

    curr_time = HI_MDM_GetMilliSeconds();

    dfx->total_rx_cnt++;
    if(msg->ulMsgId != ID_MRS_MSG_MR_TIMER)
    {
        dfx->rx_no_timer_cnt++;
    }

    mrsDfxRefreshLastDataU32(dfx->last_rx_id, DFX_ALEN(dfx->last_rx_id), msg->ulMsgId);
    dfx->last_rx_time = curr_time;

    delay_time = curr_time - msg->ulTimeStamp;

    mrsDfxRefreshLastDataU32(dfx->last_msg_delay, DFX_ALEN(dfx->last_msg_delay), delay_time);
    if(delay_time > dfx->max_msg_delay)
    {
        HI_U32 queue_num = MRS_GetQueueMsgNum(EN_APP_QUEUE_MRS);

        dfx->max_msg_delay = delay_time;
        dfx->max_delay_id = msg->ulMsgId;
        dfx->max_delay_time = curr_time;
        dfx->max_delay_queue_num = queue_num;
    }

    mrsDfxMsgDelayCnt(delay_time, &dfx->delay_cnt);

    return;
}

// 消息处理耗时
HI_VOID mrsDfxMsgPrcoTimeConsuming(HI_U32 start_time, HI_U32 id)
{
    HI_DSID_APP_MSG_INF_S * dfx = HI_NULL;
    HI_U32 curr_time = 0;
    HI_U32 time_consuming = 0;

    dfx = mrsDfxGetTaskMsgInf();

    curr_time = HI_MDM_GetMilliSeconds();

    time_consuming = curr_time - start_time;

    if(time_consuming > dfx->max_msg_proc)
    {
        dfx->max_msg_proc = time_consuming;
        dfx->max_msg_proc_id = id;
        dfx->max_msg_proc_time = curr_time;
    }

    return;
}


// *********************
// 广播帧收发统计
// *********************
#define mrsDfxGetPlcBcInf() (&dfx_module.app_plc_bc_inf)

// 广播发送PLC帧统计
HI_VOID mrsDfxPlcBcTx(HI_DMS_CHL_TX_ADDR_S *addr)
{
    if(!addr)
    {
        return;
    }

    if(mrsToolsBroadCastMac(addr->ucDestMac))
    {
        HI_DSID_APP_PLC_BC_INF_S *dfx = mrsDfxGetPlcBcInf();

        dfx->tx_ok_cnt++;
        mrsDfxRefreshLastDataU32(dfx->last_tx_time, DFX_ALEN(dfx->last_tx_time), HI_MDM_GetMilliSeconds());
    }

    return;
}

// 广播发送PLC帧错误统计
HI_VOID mrsDfxPlcBcTxErr(HI_DMS_CHL_TX_ADDR_S *addr)
{
    if(!addr)
    {
        return;
    }

    if(mrsToolsBroadCastMac(addr->ucDestMac))
    {
        HI_DSID_APP_PLC_BC_INF_S *dfx = mrsDfxGetPlcBcInf();

        dfx->tx_fail_cnt++;
        mrsDfxRefreshLastDataU32(dfx->fail_tx_time, DFX_ALEN(dfx->fail_tx_time), HI_MDM_GetMilliSeconds());
    }

    return;
}

// 广播接收PLC帧统计
HI_VOID mrsDfxPlcBcRx(HI_VOID)
{
    HI_DSID_APP_PLC_BC_INF_S *dfx = mrsDfxGetPlcBcInf();

    dfx->rx_cnt++;
    mrsDfxRefreshLastDataU32(dfx->last_rx_time, DFX_ALEN(dfx->last_rx_time), HI_MDM_GetMilliSeconds());

    return;
}

// 广播读表次数统计
HI_VOID mrsDfxBcRmCnt(HI_VOID)
{
    HI_DSID_APP_PLC_BC_INF_S *dfx = mrsDfxGetPlcBcInf();

    dfx->bc_rm_cnt++;

    return;
}

// 广播读表成功次数统计
HI_VOID mrsDfxBcRmOkCnt(HI_VOID)
{
    HI_DSID_APP_PLC_BC_INF_S *dfx = mrsDfxGetPlcBcInf();

    dfx->bc_rm_ok_cnt++;

    return;
}

// 广播读表非广播响应次数统计
HI_VOID mrsDfxBcRmNonBcRespCnt(HI_VOID)
{
    HI_DSID_APP_PLC_BC_INF_S *dfx = mrsDfxGetPlcBcInf();

    dfx->bc_rm_non_bc_resp_cnt++;

    return;
}

// 广播读表失败次数统计
HI_VOID mrsDfxBcRmFailCnt(HI_VOID)
{
    HI_DSID_APP_PLC_BC_INF_S *dfx = mrsDfxGetPlcBcInf();

    dfx->bc_rm_fail_cnt++;

    return;
}


#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)

// ******************************
// 读表队列状态统计
// ******************************
// 刷新读表队列深度统计
HI_VOID mrsDfxRefreshRmQueueNum(HI_U8 num)
{
    HI_DSID_APP_RM_QUEUE_INF_S * dfx = HI_NULL;

    dfx = mrsDfxGetQueueInf();

    dfx->curr_num = num;
    if(num > dfx->max_num)
    {
        dfx->max_num = num;
    }

    return;
}

#define MRS_RM_QUEUE_INVALID_ID 0  // 读表队列无效ID

// 刷新读表队列队首状态
HI_VOID mrsDfxRefreshRmQueueTopStatus(MRS_SRV_QUEUE *queue, HI_U8 lock)
{
    HI_DSID_APP_RM_QUEUE_INF_S * dfx = HI_NULL;
    HI_U32 t = 0;
    MRS_STA_ITEM  *item = HI_NULL;
    HI_PRV HI_U32 last_time = 0;

    dfx = mrsDfxGetQueueInf();

    t = HI_MDM_GetMilliSeconds();
    if(lock)
    {
        dfx->lock_time += (t - last_time);
        if(dfx->lock_time > dfx->max_lock_time)
        {
            dfx->max_lock_time = dfx->lock_time;
        }
    }
    else
    {
        if(dfx->lock_time)
        {
            dfx->last_lock_time = dfx->lock_time;
        }
        dfx->lock_time = 0;
    }

    item = mrsSrvQueueTop(queue);
    if(item != HI_NULL)
    {
        dfx->first_id = item->id;
    }
    else
    {
        dfx->first_id = MRS_RM_QUEUE_INVALID_ID;
    }

    last_time = t;

    return;
}

// 读表队列满统计
HI_VOID mrsDfxRmQueueFullCnt(HI_VOID)
{
    HI_DSID_APP_RM_QUEUE_INF_S * dfx = HI_NULL;

    dfx = mrsDfxGetQueueInf();

    dfx->queue_full_cnt++;
    mrsDfxRefreshLastDataU32(dfx->queue_full_time,
                        DFX_ALEN(dfx->queue_full_time),
                        HI_MDM_GetMilliSeconds());

    return;
}
#endif

// ******************************
// 定时器相关统计
// ******************************

/* BEGIN: Modified by fengxiaomin/00209182, 2014/3/27   问题单号:DTS2014032906596 */
HI_DSID_APP_RM_RX_TX_INF_S * mrsDfxGetRmTxRxInf(HI_VOID)
{
    dfx_module.app_rm_rx_tx_inf.ulVer = (HI_U32)APP_ONE_KEY_DIAG_VER_R0;

    return &dfx_module.app_rm_rx_tx_inf;
}

HI_BOOL mrsIsTestDI(HI_U8 *pData, HI_U16 usDataLen)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U32 ulDI = 0;
    HI_U16 usPos = 0;
    HI_U16 usFrameLen = 0;
    HI_U32 ucTestDI = MRS_TEST_READ_METER_645_DI;

    ret = mrsFind645Frame(pData, (HI_S16)usDataLen, &usPos, &usFrameLen);
    if (ret == HI_ERR_SUCCESS)
    {
        memcpy_s(&ulDI, sizeof(HI_U32), pData + usPos + MRS_645_FRAME_DATA_OFFSET, sizeof(ulDI));
        mrs645DataDecode((HI_U8*)&ulDI, sizeof(ulDI));
        if (!memcmp(&ulDI, &ucTestDI, sizeof(ulDI)))
        {
            return HI_TRUE;
        }
    }

    return HI_FALSE;
}

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
HI_VOID mrsCcoDfxRmUartRx(HI_U8 *pData, HI_U16 usDataLen, HI_U8 ucFlag)
{
    HI_DSID_APP_RM_RX_TX_INF_S *pstRmDfx = mrsDfxGetRmTxRxInf();

    if (mrsIsTestDI(pData, usDataLen))
    {
        if (ucFlag)
        {
            pstRmDfx->ulTestUartTxNum++;
        }

        pstRmDfx->ulTestUartRxNum++;
    }

    if (ucFlag)
    {
        pstRmDfx->ulUartTxNum++;
    }

    pstRmDfx->ulUartRxNum++;
}
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
HI_VOID mrsStaDfxRmUartRx(HI_U16 usId, HI_U8 *pData, HI_U16 usDataLen)
{
    HI_DSID_APP_RM_RX_TX_INF_S *pstRmDfx = mrsDfxGetRmTxRxInf();

    if((usId < ID_MRS_CMD_DATA_TRANSMIT_AFN13_PLC) || (usId > ID_MRS_CMD_PARALLEL_DATA_PLC))
    {
        return;
    }

    if (!pData)
    {
        return;
    }

    if (mrsIsTestDI(pData, usDataLen))
    {
        pstRmDfx->ulTestUartRxNum++;
    }

    pstRmDfx->ulUartRxNum++;
}

HI_VOID mrsStaDfxRmRxDeny(HI_U16 usId, HI_U8 ucDenyFlag)
{
    HI_DSID_APP_RM_RX_TX_INF_S *pstRmDfx = mrsDfxGetRmTxRxInf();

    if((usId >= ID_MRS_CMD_DATA_TRANSMIT_AFN13_PLC) && (usId <= ID_MRS_CMD_PARALLEL_DATA_PLC))
    {
        if (ucDenyFlag)
        {
            pstRmDfx->usUartRxDenyNum++;
        }
    }
}

HI_VOID mrsDfxStaUartTimeOut(HI_U16 usId)
{
    HI_DSID_APP_RM_RX_TX_INF_S *pstRmDfx = mrsDfxGetRmTxRxInf();

    if((usId >= ID_MRS_CMD_DATA_TRANSMIT_AFN13_PLC) && (usId <= ID_MRS_CMD_PARALLEL_DATA_PLC))
    {
        pstRmDfx->usUartTimeOutNum++;
    }
}

HI_VOID mrsStaDfxRmRxErrData(HI_U16 usId, HI_U8 *pData, HI_U16 usDataLen)
{
    HI_DSID_APP_RM_RX_TX_INF_S *pstRmDfx = mrsDfxGetRmTxRxInf();
    HI_U32 ulDI = 0;

    if (!pData)
    {
        return;
    }

    if((usId < ID_MRS_CMD_DATA_TRANSMIT_AFN13_PLC) || (usId > ID_MRS_CMD_PARALLEL_DATA_PLC))
    {
        return;
    }

    memcpy_s(&ulDI, sizeof(ulDI), pData, sizeof(ulDI));
    switch (ulDI)
    {
    case MRS_DAYFREEZE_TIME_DI:
    case MRS_DAYFREEZE_DATA_DI:
        if (!mrsToolsCheckBCDEx(pData + sizeof(ulDI), usDataLen - sizeof(ulDI)))
        {
            pstRmDfx->usUartRxErrDataNum++;
            break;
        }

        if (MRS_DAYFREEZE_TIME_DI == ulDI)
        {
            HI_U8 ucTime[6] = {0};
            HI_U32 ret = HI_ERR_SUCCESS;

            memcpy_s(ucTime + 1, sizeof(ucTime) - 1, pData + sizeof(ulDI), sizeof(ucTime) - 1);//日冻结时间内没有秒
            mrs645DataDecode(ucTime + 1, sizeof(ucTime) - 1);
            ret = MRS_TimeIsValid(ucTime);
            if (ret != HI_ERR_SUCCESS)
            {
                pstRmDfx->usUartRxErrDataNum++;
            }
        }

        break;
    default:
        break;
    }
}
#endif

HI_VOID mrsDfxRmUartTx(HI_U8 *pData, HI_U16 usDataLen, HI_BOOL bFlag)
{
    HI_DSID_APP_RM_RX_TX_INF_S *pstRmDfx = mrsDfxGetRmTxRxInf();

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    if (bFlag)
    {
        pstRmDfx->ulUartRxNum++;
    }
#else
    HI_UNREF_PARAM(bFlag);
#endif  
    pstRmDfx->ulUartTxNum++;

    if (!pData)
    {
        return;
    }

    if (mrsIsTestDI(pData, usDataLen))
    {
        #if !defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
        if (bFlag)
        {
            pstRmDfx->ulTestUartRxNum++;
        }
        #endif
        pstRmDfx->ulTestUartTxNum++;
    }
}

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
HI_VOID mrsDfxRmUartDenyTx(HI_U8 *pData, HI_U16 usDataLen)
{
    HI_DSID_APP_RM_RX_TX_INF_S *pstRmDfx = mrsDfxGetRmTxRxInf();

    if (!pData)
    {
        return;
    }

    if (mrsIsTestDI(pData, usDataLen))
    {
        pstRmDfx->usTestUartTxDenyNum++;
    }

    pstRmDfx->usUartTxDenyNum++;
}
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
HI_VOID mrsDfxRmPlcUpAfn(HI_U16 usId, HI_U8 *pData, HI_U32 ulRet)
{
    if (!pData)
    {
        return;
    }

    if (usId == ID_MRS_CMD_DATA_TRANSMIT_AFN13_PLC)
    {
        MRS_DATA_TRANSMIT_AFN13_UP_STRU *pstAfn13 = (MRS_DATA_TRANSMIT_AFN13_UP_STRU*)pData;
        mrsDfxRmPlcTx((HI_U8*)pstAfn13 + sizeof(MRS_DATA_TRANSMIT_AFN13_UP_STRU), pstAfn13->data_len, ulRet);
    }
    else if (usId == ID_MRS_CMD_DATA_TRANSMIT_AFN14_PLC)
    {
        MRS_DATA_TRANSMIT_AFN14_UP_STRU *pstAfn14 = (MRS_DATA_TRANSMIT_AFN14_UP_STRU*)pData;
        mrsDfxRmPlcTx((HI_U8*)pstAfn14 + sizeof(MRS_DATA_TRANSMIT_AFN14_UP_STRU), pstAfn14->data_len, ulRet);
    }
    else if (usId == ID_MRS_CMD_PARALLEL_DATA_PLC)
    {
        MRS_DATA_TRANSMIT_AFNF1_UP_STRU *pstAfnF1 = (MRS_DATA_TRANSMIT_AFNF1_UP_STRU*)pData;
        mrsDfxRmPlcTx((HI_U8*)pstAfnF1 + sizeof(MRS_DATA_TRANSMIT_AFNF1_UP_STRU), pstAfnF1->data_len, ulRet);
    }
}
#endif

HI_VOID mrsDfxRmPlcTx(HI_U8 *pData, HI_U16 usDataLen, HI_U32 ulRet)
{
    HI_DSID_APP_RM_RX_TX_INF_S *pstRmDfx = mrsDfxGetRmTxRxInf();

    if (!pData)
    {
        return;
    }

    if (mrsIsTestDI(pData, usDataLen))
    {
        if (ulRet != HI_ERR_SUCCESS)
        {
            pstRmDfx->usTestPlcTxFailNum++;
            pstRmDfx->usPlcTxFailNum++;
            return;
        }

        pstRmDfx->ulTestPlcTxNum++;
    }

    if (ulRet != HI_ERR_SUCCESS)
    {
        pstRmDfx->usPlcTxFailNum++;
        return;
    }

    pstRmDfx->ulPlcTxNum++;
}

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
HI_VOID mrsDfxRmPlcUpRx(HI_U16 usId, HI_U8* pData, HI_BOOL ucExceedFlag)
{
    HI_U8 *pBuffer = HI_NULL;
    HI_U16 usBufferLen = 0;

    if (!pData)
    {
        return;
    }

    if (usId == ID_MRS_CMD_DATA_TRANSMIT_AFN13_PLC)
    {
        MRS_DATA_TRANSMIT_AFN13_UP_STRU *pstAfn13 = (MRS_DATA_TRANSMIT_AFN13_UP_STRU*)pData;
        pBuffer = (HI_U8*)pstAfn13 + sizeof(MRS_DATA_TRANSMIT_AFN13_UP_STRU);
        usBufferLen = pstAfn13->data_len;
    }
    else if (usId == ID_MRS_CMD_DATA_TRANSMIT_AFN14_PLC)
    {
        MRS_DATA_TRANSMIT_AFN14_UP_STRU *pstAfn14 = (MRS_DATA_TRANSMIT_AFN14_UP_STRU*)pData;
        pBuffer = (HI_U8*)pstAfn14 + sizeof(MRS_DATA_TRANSMIT_AFN14_UP_STRU);
        usBufferLen = pstAfn14->data_len;
    }
    else if (usId == ID_MRS_CMD_PARALLEL_DATA_PLC)
    {
        MRS_DATA_TRANSMIT_AFNF1_UP_STRU *pstAfnF1 = (MRS_DATA_TRANSMIT_AFNF1_UP_STRU*)pData;
        pBuffer = (HI_U8*)pstAfnF1 + sizeof(MRS_DATA_TRANSMIT_AFNF1_UP_STRU);
        usBufferLen = pstAfnF1->data_len;
    }

    if (!ucExceedFlag)
    {
        mrsDfxRmPlcRx(pBuffer, usBufferLen);
        return;
    }

    mrsDfxRmPlcExceedRx(pBuffer, usBufferLen);
}
#endif

#if (defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) || defined(PRODUCT_CFG_PRODUCT_TYPE_STA))
HI_VOID mrsDfxRmPlcDlRx(HI_U16 usId, HI_U8* pData, HI_BOOL bTimeoutFlag)
{
    HI_U8 *pBuffer = HI_NULL;
    HI_U16 usBufferLen = 0;

    if (!pData)
    {
        return;
    }

    if (usId == ID_MRS_CMD_DATA_TRANSMIT_AFN13_PLC)
    {
        MRS_DATA_TRANSMIT_AFN13_DL_STRU *pstAfn13 = (MRS_DATA_TRANSMIT_AFN13_DL_STRU*)pData;
        pBuffer = (HI_U8*)pstAfn13 + sizeof(MRS_DATA_TRANSMIT_AFN13_DL_STRU);
        usBufferLen = pstAfn13->data_len;
    }
    else if (usId == ID_MRS_CMD_DATA_TRANSMIT_AFN14_PLC)
    {
        MRS_DATA_TRANSMIT_AFN14_DL_STRU *pstAfn14 = (MRS_DATA_TRANSMIT_AFN14_DL_STRU*)pData;
        pBuffer = (HI_U8*)pstAfn14 + sizeof(MRS_DATA_TRANSMIT_AFN14_DL_STRU);
        usBufferLen = pstAfn14->data_len;
    }
    else if (usId == ID_MRS_CMD_PARALLEL_DATA_PLC)
    {
        MRS_DATA_TRANSMIT_AFNF1_DL_STRU *pstAfnF1 = (MRS_DATA_TRANSMIT_AFNF1_DL_STRU*)pData;
        pBuffer = (HI_U8*)pstAfnF1 + sizeof(MRS_DATA_TRANSMIT_AFNF1_DL_STRU);
        usBufferLen = pstAfnF1->data_len;
    }

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
    if (!bTimeoutFlag)
    {
        mrsDfxRmPlcRx(pBuffer, usBufferLen);
        return;
    }
#elif defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    HI_UNREF_PARAM(bTimeoutFlag);
    mrsDfxRmPlcTimeOut(pBuffer, usBufferLen);
#endif
}

HI_VOID mrsDfxRmPlcRx(HI_U8 *pData, HI_U16 usDataLen)
{
    HI_DSID_APP_RM_RX_TX_INF_S *pstRmDfx = mrsDfxGetRmTxRxInf();

    if (!pData)
    {
        return;
    }

    if (mrsIsTestDI(pData, usDataLen))
    {
        pstRmDfx->ulTestPlcRxNum++;
    }

    pstRmDfx->ulPlcRxNum++;
}

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
HI_VOID mrsDfxRmPlcBcRx(HI_VOID)
{
    HI_DSID_APP_RM_RX_TX_INF_S *pstRmDfx = mrsDfxGetRmTxRxInf();

    pstRmDfx->usTestPlcRxBcNum++;
}
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
HI_VOID mrsDfxRmPlcExceedRx(HI_U8 *pData, HI_U16 usDataLen)
{
    HI_DSID_APP_RM_RX_TX_INF_S *pstRmDfx = mrsDfxGetRmTxRxInf();

    if (mrsIsTestDI(pData, usDataLen))
    {
        pstRmDfx->usTestPlcRxExceedNum++;
    }

    pstRmDfx->usPlcRxExceedNum++;
}
#endif

HI_VOID mrsDfxRmPlcTimeOut(HI_U8 *pData, HI_U16 usDataLen)
{
    HI_DSID_APP_RM_RX_TX_INF_S *pstRmDfx = mrsDfxGetRmTxRxInf();

    if (mrsIsTestDI(pData, usDataLen))
    {
        pstRmDfx->ulTestPlcTimeOutNum++;
    }

    pstRmDfx->ulPlcTimeOutNum++;
}
/* END:   Modified by fengxiaomin/00209182, 2014/3/27 */

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
#define mrsDfxGetCcoTimeInf() (&dfx_module.app_get_time_inf)

HI_VOID mrsDfxCurrGetTimeStatus(HI_U8 status)
{
    HI_DSID_APP_CCO_GET_TIME_INF_S *pstTimeInf = mrsDfxGetCcoTimeInf();

    pstTimeInf->ucCurrGetTimeStatus = status;
}

HI_VOID mrsDfxGetTimeSucc(HI_BOOL ucSuccFlag)
{
    HI_DSID_APP_CCO_GET_TIME_INF_S *pstTimeInf = mrsDfxGetCcoTimeInf();

    pstTimeInf->ucGetTimeSuccFlag = (HI_U8)ucSuccFlag;
}

HI_VOID mrsDfxPowerOnGetTime(HI_U8 clockBuf[6])
{
    HI_DSID_APP_CCO_GET_TIME_INF_S *pstTimeInf = mrsDfxGetCcoTimeInf();
    HI_PRV HI_U8 ucFirst = 0;

    if (!ucFirst)
    {
        memcpy_s(pstTimeInf->ucFirstClock, sizeof(pstTimeInf->ucFirstClock), clockBuf, sizeof(pstTimeInf->ucFirstClock));
    }
    else
    {
        memcpy_s(pstTimeInf->ucLastClock, sizeof(pstTimeInf->ucLastClock), clockBuf, sizeof(pstTimeInf->ucLastClock));
    }
}

HI_VOID mrsDfxGetTimeTotalCnt(HI_VOID)
{
    HI_DSID_APP_CCO_GET_TIME_INF_S *pstTimeInf = mrsDfxGetCcoTimeInf();

    pstTimeInf->usGetTimeSendCnt++;
}

HI_VOID mrsDfxGetTimeFailCnt(HI_VOID)
{
    HI_DSID_APP_CCO_GET_TIME_INF_S *pstTimeInf = mrsDfxGetCcoTimeInf();

    pstTimeInf->usFirstClockFailCnt++;
}
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
#define mrsDfxGetCcoReportQueueInf() (&dfx_module.app_cco_report_queue_inf)
#define mrsDfxGetCcoEvtInf() (&dfx_module.app_cco_evt_inf.stDfxCcoEvtInf[1])
#define mrsDfxGetCcoPowerInf() (&dfx_module.app_cco_power_on_off_inf)

HI_VOID mrsDfxCcoQueueCurrRetry(HI_U8 ucCurrRetry)
{
    HI_DSID_APP_CCO_REPORT_QUEUE_INF_S *pstCcoQueueInf = mrsDfxGetCcoReportQueueInf();

    pstCcoQueueInf->ucCurrRetry = ucCurrRetry;
}

HI_VOID mrsDfxCcoQueueMallocFailCnt(HI_VOID)
{
    HI_DSID_APP_CCO_REPORT_QUEUE_INF_S *pstCcoQueueInf = mrsDfxGetCcoReportQueueInf();

    pstCcoQueueInf->ucJoinCcoQueueMallocFailCnt++;
}

HI_VOID mrsDfxCcoQueueCurrItem(HI_U16 usCurrItem)
{
    HI_DSID_APP_CCO_REPORT_QUEUE_INF_S *pstCcoQueueInf = mrsDfxGetCcoReportQueueInf();

    pstCcoQueueInf->usCurrItem = usCurrItem;
}

HI_VOID mrsDfxCcoQueueCurrAfn(HI_U8 ucAfn)
{
    HI_DSID_APP_CCO_REPORT_QUEUE_INF_S *pstCcoQueueInf = mrsDfxGetCcoReportQueueInf();

    pstCcoQueueInf->ucCurrAfn = ucAfn;
}

HI_VOID mrsDfxCcoQueueCurrFn(HI_U16 usFn)
{
    HI_DSID_APP_CCO_REPORT_QUEUE_INF_S *pstCcoQueueInf = mrsDfxGetCcoReportQueueInf();

    pstCcoQueueInf->usCurrFn = usFn;
}

HI_VOID mrsDfxCcoQueueJoinFailCnt(HI_VOID)
{
    HI_DSID_APP_CCO_REPORT_QUEUE_INF_S *pstCcoQueueInf = mrsDfxGetCcoReportQueueInf();

    pstCcoQueueInf->usJoinCcoQueueFailCnt++;
}

HI_VOID mrsDfxCcoQueueJoinCnt(HI_VOID)
{
    HI_DSID_APP_CCO_REPORT_QUEUE_INF_S *pstCcoQueueInf = mrsDfxGetCcoReportQueueInf();

    pstCcoQueueInf->ulJoinCcoQueueCnt++;
}

HI_VOID mrsDfxCcoQueueReportCnt(HI_VOID)
{
    HI_DSID_APP_CCO_REPORT_QUEUE_INF_S *pstCcoQueueInf = mrsDfxGetCcoReportQueueInf();

    pstCcoQueueInf->ulCcoReportCnt++;
}

HI_VOID mrsDfxCcoQueueRxAckCnt(HI_VOID)
{
    HI_DSID_APP_CCO_REPORT_QUEUE_INF_S *pstCcoQueueInf = mrsDfxGetCcoReportQueueInf();

    pstCcoQueueInf->ulCcoRxAckCnt++;
}

HI_VOID mrsDfxCcoQueueRxDenyCnt(HI_VOID)
{
    HI_DSID_APP_CCO_REPORT_QUEUE_INF_S *pstCcoQueueInf = mrsDfxGetCcoReportQueueInf();

    pstCcoQueueInf->ulCcoRxDenyCnt++;
}

HI_VOID mrsDfxCcoQueueReportTimeOutCnt(HI_VOID)
{
    HI_DSID_APP_CCO_REPORT_QUEUE_INF_S *pstCcoQueueInf = mrsDfxGetCcoReportQueueInf();

    pstCcoQueueInf->ulCcoReportTimeOutCnt++;
}

//事件存储
HI_VOID mrsDfxCcoEvtBakSave(HI_U16 usBakTime)
{
    HI_U32 ulCurrTime = HI_MDM_GetSeconds();
    HI_DFX_APP_CCO_EVT_INF_S *pstEvtInf = mrsDfxGetCcoEvtInf();
    HI_PRV HI_U8 ucFirst = 0;

    if (!ucFirst)
    {
        ucFirst++;
        pstEvtInf->ulTime = ulCurrTime;

        return;
    }

    if ((ulCurrTime - pstEvtInf->ulTime) < usBakTime * 60)
    {
        return;
    }

    memcpy_s(&dfx_module.app_cco_evt_inf_bak.stDfxCcoEvtInf[1],
                    sizeof(HI_DFX_APP_CCO_EVT_INF_S),
                    &dfx_module.app_cco_evt_inf_bak.stDfxCcoEvtInf[0],
                    sizeof(HI_DFX_APP_CCO_EVT_INF_S));
    memcpy_s(&dfx_module.app_cco_evt_inf_bak.stDfxCcoEvtInf[0],
                    sizeof(HI_DFX_APP_CCO_EVT_INF_S),
                    &dfx_module.app_cco_evt_inf.stDfxCcoEvtInf[0],
                    sizeof(HI_DFX_APP_CCO_EVT_INF_S));
    memcpy_s(&dfx_module.app_cco_evt_inf.stDfxCcoEvtInf[0],
                    sizeof(HI_DFX_APP_CCO_EVT_INF_S),
                    pstEvtInf,
                    sizeof(HI_DFX_APP_CCO_EVT_INF_S));

    pstEvtInf->ulTime = ulCurrTime;
}

HI_VOID mrsDfxCcoEvtFlag(HI_BOOL bEvtFlag)
{
    HI_DFX_APP_CCO_EVT_INF_S *pstEvtInf = mrsDfxGetCcoEvtInf();

    pstEvtInf->bACDFlg = bEvtFlag;
}

HI_VOID mrsDfxEvtJoinCcoQueueMallocFailCnt(HI_VOID)
{
    HI_DFX_APP_CCO_EVT_INF_S *pstEvtInf = mrsDfxGetCcoEvtInf();

    pstEvtInf->ucJoinCcoQueueMallocFailCnt++;
}

HI_VOID mrsDfxEvtReportedListMallocFailCnt(HI_VOID)
{
    HI_DFX_APP_CCO_EVT_INF_S *pstEvtInf = mrsDfxGetCcoEvtInf();

    pstEvtInf->ucJoinEvtRepListMallocFailCnt++;
}

HI_VOID mrsDfxCcoEvtListItemNum(HI_U8 ucItemNum)
{
    HI_DFX_APP_CCO_EVT_INF_S *pstEvtInf = mrsDfxGetCcoEvtInf();

    pstEvtInf->ucEvtListItemNum = ucItemNum;
}

HI_VOID mrsDfxCcoEvtQueueItemNum(HI_U16 usItemNum)
{
    HI_DFX_APP_CCO_EVT_INF_S *pstEvtInf = mrsDfxGetCcoEvtInf();

    pstEvtInf->usEvtQueueItemNum = usItemNum;
}

HI_VOID mrsDfxCcoEvtCompatibilityErrCnt(HI_VOID)
{
    HI_DFX_APP_CCO_EVT_INF_S *pstEvtInf = mrsDfxGetCcoEvtInf();

    pstEvtInf->ucCcoRxCompatibilityErrCnt++;
}

HI_VOID mrsDfxCcoEvtReportTimeOut(HI_VOID)
{
    HI_DFX_APP_CCO_EVT_INF_S *pstEvtInf = mrsDfxGetCcoEvtInf();

    pstEvtInf->usCcoReportTimeoutCnt++;
}

HI_VOID mrsDfxCcoEvtReportCnt(HI_VOID)
{
    HI_DFX_APP_CCO_EVT_INF_S *pstEvtInf = mrsDfxGetCcoEvtInf();

    pstEvtInf->ulCcoReportEvtCnt++;
}


HI_VOID mrsDfxEvtJoinCcoQueueTime(HI_U32 ulTime)
{
    HI_DFX_APP_CCO_EVT_INF_S *pstEvtInf = mrsDfxGetCcoEvtInf();
    mrsDfxRefreshLastDataU32(pstEvtInf->ulCcoEvtJoinQueueTimeSec, MRS_TOOLS_ALEN(pstEvtInf->ulCcoEvtJoinQueueTimeSec), ulTime);
}


HI_VOID mrsDfxCcoEvtRxAckCnt(HI_VOID)
{
    HI_DFX_APP_CCO_EVT_INF_S *pstEvtInf = mrsDfxGetCcoEvtInf();

    pstEvtInf->ulCcoRxAckCnt++;
}

HI_VOID mrsDfxCcoEvtRxPlcCnt(HI_VOID)
{
    HI_DFX_APP_CCO_EVT_INF_S *pstEvtInf = mrsDfxGetCcoEvtInf();

    pstEvtInf->ulCcoRxEvtPlcCnt++;
}


HI_VOID mrsDfxCcoEvtRxPlcTime(HI_U32 ulTime)
{
    HI_DFX_APP_CCO_EVT_INF_S *pstEvtInf = mrsDfxGetCcoEvtInf();
    mrsDfxRefreshLastDataU32(pstEvtInf->ulCcoRxEvtPlcTimeSec, MRS_TOOLS_ALEN(pstEvtInf->ulCcoRxEvtPlcTimeSec), ulTime);
}


HI_VOID mrsDfxCcoEvtFilterPlcCnt(HI_VOID)
{
    HI_DFX_APP_CCO_EVT_INF_S *pstEvtInf = mrsDfxGetCcoEvtInf();

    pstEvtInf->ulCcoFilterPlcCnt++;
}

HI_VOID mrsDfxCcoEvtReportedFilterCnt(HI_VOID)
{
    HI_DFX_APP_CCO_EVT_INF_S *pstEvtInf = mrsDfxGetCcoEvtInf();

    pstEvtInf->ulCcoFilterReportedEvtCnt++;
}

HI_VOID mrsDfxCcoEvtRxProtoErrCnt(HI_VOID)
{
    HI_DFX_APP_CCO_EVT_INF_S *pstEvtInf = mrsDfxGetCcoEvtInf();

    pstEvtInf->ulCcoEvtRxProtoErrCnt++;
}

HI_VOID mrsDfxCcoEvtRxReportDisableCnt(HI_VOID)
{
    HI_DFX_APP_CCO_EVT_INF_S *pstEvtInf = mrsDfxGetCcoEvtInf();

    pstEvtInf->ulCcoEvtRxReportDisableCnt++;
}

HI_VOID mrsDfxCcoEvtTxAckPlcCnt(HI_VOID)
{
    HI_DFX_APP_CCO_EVT_INF_S *pstEvtInf = mrsDfxGetCcoEvtInf();

    pstEvtInf->ulCcoTxAckPlcCnt++;
}

HI_VOID mrsDfxCcoEvtTxForbidPlcCnt(HI_VOID)
{
    HI_DFX_APP_CCO_EVT_INF_S *pstEvtInf = mrsDfxGetCcoEvtInf();

    pstEvtInf->ulCcoTxForbidPlcCnt++;
}

HI_VOID mrsDfxCcoEvtTxFullPlcCnt(HI_VOID)
{
    HI_DFX_APP_CCO_EVT_INF_S *pstEvtInf = mrsDfxGetCcoEvtInf();

    pstEvtInf->ulCcoTxFullPlcCnt++;
}

HI_VOID mrsDfxCcoEvtTxAllowPlcCnt(HI_VOID)
{
    HI_DFX_APP_CCO_EVT_INF_S *pstEvtInf = mrsDfxGetCcoEvtInf();

    pstEvtInf->ulCcoTxAllowPlcCnt++;
}

HI_VOID mrsDfxCcoForbidEvtRxPlcCnt(HI_VOID)
{
    HI_DFX_APP_CCO_EVT_INF_S *pstEvtInf = mrsDfxGetCcoEvtInf();

    pstEvtInf->ulCcoForbidRxEvtCnt++;
}

HI_VOID mrsDfxCcoEvtRepReplaceHeadCnt(HI_VOID)
{
    HI_DFX_APP_CCO_EVT_INF_S *pstEvtInf = mrsDfxGetCcoEvtInf();

    pstEvtInf->ulCcoEvtReplaceReportedHeadCnt++;
}

HI_VOID mrsDfxCcoEvtRepReplaceSameAddrCnt(HI_VOID)
{
    HI_DFX_APP_CCO_EVT_INF_S *pstEvtInf = mrsDfxGetCcoEvtInf();

    pstEvtInf->ulEvtReportedReplaceSameAddrCnt++;
}

HI_VOID mrsDfxCcoEvtRepAgedCnt(HI_VOID)
{
    HI_DFX_APP_CCO_EVT_INF_S *pstEvtInf = mrsDfxGetCcoEvtInf();

    pstEvtInf->ulEvtReportedAgedCnt++;
}

HI_VOID mrsDfxCcoEvtQueueFrontRear(MRS_SRV_QUEUE *pstQueue)
{
    HI_DFX_APP_CCO_EVT_INF_S *pstEvtInf = mrsDfxGetCcoEvtInf();

    pstEvtInf->ulEvtQueueFront = (HI_U32)pstQueue->front;
    pstEvtInf->ulEvtQueueRear = (HI_U32)pstQueue->rear;
}

HI_VOID mrsDfxCcoFlashMemErrCnt(HI_U8 Index)
{
    MRS_DFX_MODULE *dfx;

    dfx = mrsDfxGetModule();
    dfx->app_cco_inc_inf.stDfxCcoMemIncInf[Index].ulIncMallocErrCount++;
}

HI_VOID mrsDfxCcoFlashMemMallocCnt(HI_U32 ulIncCnt, HI_U8 Index)
{
    MRS_DFX_MODULE *dfx;

    dfx = mrsDfxGetModule();
    dfx->app_cco_inc_inf.stDfxCcoMemIncInf[Index].ulIncMallocCount++;
    dfx->app_cco_inc_inf.stDfxCcoMemIncInf[Index].ulLastIncSize= ulIncCnt;
    dfx->app_cco_inc_inf.stDfxCcoMemIncInf[Index].ulTotalSize+= ulIncCnt;
}

HI_VOID mrsDfxCcoPowerRxLenErrCnt(HI_VOID)
{
    HI_DSID_APP_CCO_POWER_ON_OFF_INF_S *pstCcoPowerInf = mrsDfxGetCcoPowerInf();

    pstCcoPowerInf->ucPlcRxLenErrCnt++;
}

HI_VOID mrsDfxCcoTxAckRxPowerOnCnt(HI_VOID)
{
    HI_DSID_APP_CCO_POWER_ON_OFF_INF_S *pstCcoPowerInf = mrsDfxGetCcoPowerInf();

    pstCcoPowerInf->usTxAckRxPowerOnCnt++;
}

HI_VOID mrsDfxCcoRxPowerOffCnt(HI_VOID)
{
    HI_DSID_APP_CCO_POWER_ON_OFF_INF_S *pstCcoPowerInf = mrsDfxGetCcoPowerInf();

    pstCcoPowerInf->usRxPowerOffCnt++;
}

HI_VOID mrsDfxCcoCurrPowerMeterCnt(HI_U8 ucType, HI_U16 usNum)
{
    HI_DSID_APP_CCO_POWER_ON_OFF_INF_S *pstCcoPowerInf = mrsDfxGetCcoPowerInf();

    if (MRS_CLT_POWER_OFF_TYPE == ucType)
    {
        pstCcoPowerInf->usCurrPowerOffMeterCnt = usNum;
    }
    else if (MRS_CLT_POWER_ON_TYPE == ucType)
    {
        pstCcoPowerInf->usCurrPowerOnMeterCnt = usNum;
    }
}

HI_VOID mrsDfxCcoRxFilterPowerMeterCnt(HI_U8 ucType)
{
    HI_DSID_APP_CCO_POWER_ON_OFF_INF_S *pstCcoPowerInf = mrsDfxGetCcoPowerInf();

    if (MRS_CLT_POWER_OFF_TYPE == ucType)
    {
        pstCcoPowerInf->usRxFilterPowerOffMeterCnt++;
    }
    else if (MRS_CLT_POWER_ON_TYPE == ucType)
    {
        pstCcoPowerInf->usRxFilterPowerOnMeterCnt++;
    }
}

HI_VOID mrsDfxCcoReportPowerMeterCnt(HI_U8 ucType, HI_U16 usNum)
{
    HI_DSID_APP_CCO_POWER_ON_OFF_INF_S *pstCcoPowerInf = mrsDfxGetCcoPowerInf();

    if (MRS_CLT_POWER_OFF_TYPE == ucType)
    {
        pstCcoPowerInf->usReportPowerOffMeterCnt += usNum;
    }
    else if (MRS_CLT_POWER_ON_TYPE == ucType)
    {
        pstCcoPowerInf->usReportPowerOnMeterCnt += usNum;
    }
}

HI_VOID mrsDfxCcoReportPowerFrameCnt(HI_U8 ucType)
{
    HI_DSID_APP_CCO_POWER_ON_OFF_INF_S *pstCcoPowerInf = mrsDfxGetCcoPowerInf();

    if (MRS_CLT_POWER_OFF_TYPE == ucType)
    {
        pstCcoPowerInf->usReportPowerOffFrameCnt++;
    }
    else if (MRS_CLT_POWER_ON_TYPE == ucType)
    {
        pstCcoPowerInf->usReportPowerOnFrameCnt++;
    }
}
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
#define mrsDfxGetStaEvtInf() (&dfx_module.app_sta_evt_inf)
#define mrsDfxGetStaPowerInf() (&dfx_module.app_sta_power_on_off_inf)

HI_VOID mrsDfxStaEvtRetry(HI_U8 ucRetry)
{
    HI_DSID_APP_STA_EVT_INF_S *pstStaEvtInf = mrsDfxGetStaEvtInf();

    pstStaEvtInf->ucCurrRetry = ucRetry;
}

HI_VOID mrsDfxStaEvtWaitProcStat(HI_U8 ucStatus)
{
    HI_DSID_APP_STA_EVT_INF_S *pstStaEvtInf = mrsDfxGetStaEvtInf();

    pstStaEvtInf->ucEvtWaitProcStat = ucStatus;
}

HI_VOID mrsDfxStaEvtReportSeq(HI_U16 usSeq)
{
    HI_DSID_APP_STA_EVT_INF_S *pstStaEvtInf = mrsDfxGetStaEvtInf();

    pstStaEvtInf->usEvtReportSeq = usSeq;
}

HI_VOID mrsDfxStaHardEvtPinStat(HI_U8 ucEvtFlag)
{
    HI_DSID_APP_STA_EVT_INF_S *pstStaEvtInf = mrsDfxGetStaEvtInf();
    pstStaEvtInf->ucHardEvtFlg = ucEvtFlag;
}

HI_VOID mrsDfxStaCurrEvtPinStat(HI_U8 ucEvtFlag)
{
    HI_DSID_APP_STA_EVT_INF_S *pstStaEvtInf = mrsDfxGetStaEvtInf();
    HI_U32 ulTime = HI_MDM_GetMilliSeconds();

    pstStaEvtInf->ucCurrEvtFlag = ucEvtFlag;

    if(ucEvtFlag)
    {
        mrsDfxStaEvtPinHighCnt();
        mrsDfxStaEvtPinHighTimeStamp(ulTime);
    }
    else
    {
        mrsDfxStaEvtPinLowCnt();
        mrsDfxStaEvtPinLowTimeStamp(ulTime);
    }
}

HI_VOID mrsDfxStaMeterEvtType(HI_U8 ucMeterEvtType)
{
    HI_DSID_APP_STA_EVT_INF_S *pstStaEvtInf = mrsDfxGetStaEvtInf();

    pstStaEvtInf->ucMeterEvtType = ucMeterEvtType;
}

HI_VOID mrsDfxStaEvtRespDenyCnt(HI_VOID)
{
    HI_DSID_APP_STA_EVT_INF_S *pstStaEvtInf = mrsDfxGetStaEvtInf();

    if (MRS_ONE_BYTE_VALUE_MAX != pstStaEvtInf->ucRespDenyCnt)
    {
        pstStaEvtInf->ucRespDenyCnt++;
    }
}

HI_VOID mrsDfxStaMeterEvtQueryCnt(HI_VOID)
{
    HI_DSID_APP_STA_EVT_INF_S *pstStaEvtInf = mrsDfxGetStaEvtInf();

    pstStaEvtInf->usMeterEvtQueryCnt++;
}

HI_VOID mrsDfxStaMeterEvtRespCnt(HI_VOID)
{
    HI_DSID_APP_STA_EVT_INF_S *pstStaEvtInf = mrsDfxGetStaEvtInf();

    pstStaEvtInf->usMeterEvtRespCnt++;
}

HI_VOID mrsDfxStaEvtPinHighCnt(HI_VOID)
{
    HI_DSID_APP_STA_EVT_INF_S *pstStaEvtInf = mrsDfxGetStaEvtInf();

    pstStaEvtInf->usEvtPinHighCnt++;
}

HI_VOID mrsDfxStaEvtPinLowCnt(HI_VOID)
{
    HI_DSID_APP_STA_EVT_INF_S *pstStaEvtInf = mrsDfxGetStaEvtInf();

    pstStaEvtInf->usEvtPinLowCnt++;
}

HI_VOID mrsDfxStaPlcEvtReportTotalCnt(HI_VOID)
{
    HI_DSID_APP_STA_EVT_INF_S *pstStaEvtInf = mrsDfxGetStaEvtInf();

    pstStaEvtInf->ulPlcEvtReportTotalCnt++;
}

HI_VOID mrsDfxStaPlcEvtReportTimeOutCnt(HI_VOID)
{
    HI_DSID_APP_STA_EVT_INF_S *pstStaEvtInf = mrsDfxGetStaEvtInf();

    pstStaEvtInf->usPlcEvtReportTimeOutCnt++;
}

HI_VOID mrsDfxStaPlcEvtReportTimeStamp(HI_U32 ulSendTime)
{
    HI_DSID_APP_STA_EVT_INF_S *pstStaEvtInf = mrsDfxGetStaEvtInf();
    HI_PRV HI_U8 ucIndex = 0;

    pstStaEvtInf->ulPlcEvtReportTimeStamp[ucIndex] = ulSendTime;
    ucIndex = (HI_U8)((ucIndex + 1)%(sizeof(pstStaEvtInf->ulPlcEvtReportTimeStamp)/sizeof(HI_U32)));
}

HI_VOID mrsDfxStaEvtPinHighTimeStamp(HI_U32 ulTime)
{
    HI_DSID_APP_STA_EVT_INF_S *pstStaEvtInf = mrsDfxGetStaEvtInf();
    HI_PRV HI_U8 ucIndex = 0;

    pstStaEvtInf->ulEvtPinHighTimeStamp[ucIndex] = ulTime;
    ucIndex = (HI_U8)((ucIndex + 1)%(sizeof(pstStaEvtInf->ulEvtPinHighTimeStamp)/sizeof(HI_U32)));
}

HI_VOID mrsDfxStaEvtPinLowTimeStamp(HI_U32 ulTime)
{
    HI_DSID_APP_STA_EVT_INF_S *pstStaEvtInf = mrsDfxGetStaEvtInf();
    HI_PRV HI_U8 ucIndex = 0;

    pstStaEvtInf->ulEvtPinLowTimeStamp[ucIndex] = ulTime;
    ucIndex = (HI_U8)((ucIndex + 1)%(sizeof(pstStaEvtInf->ulEvtPinLowTimeStamp)/sizeof(HI_U32)));
}

HI_VOID mrsDfxStaPlcEvtAckCnt(HI_VOID)
{
    HI_DSID_APP_STA_EVT_INF_S *pstStaEvtInf = mrsDfxGetStaEvtInf();

    pstStaEvtInf->ulPlcEvtAckCnt++;
}

HI_VOID mrsDfxStaPlcEvtAckTimeStamp(HI_U32 ulTime)
{
    HI_DSID_APP_STA_EVT_INF_S *pstStaEvtInf = mrsDfxGetStaEvtInf();
    HI_PRV HI_U8 ucIndex = 0;

    pstStaEvtInf->ulPlcEvtAckTimeStamp[ucIndex] = ulTime;
    ucIndex = (HI_U8)((ucIndex + 1)%(sizeof(pstStaEvtInf->ulPlcEvtAckTimeStamp)/sizeof(HI_U32)));
}

HI_VOID mrsDfxStaPlcEvtFullCnt(HI_VOID)
{
    HI_DSID_APP_STA_EVT_INF_S *pstStaEvtInf = mrsDfxGetStaEvtInf();

    pstStaEvtInf->usPlcEvtFullCnt++;
}

HI_VOID mrsDfxStaPlcEvtFullTimeStamp(HI_U32 ulTime)
{
    HI_DSID_APP_STA_EVT_INF_S *pstStaEvtInf = mrsDfxGetStaEvtInf();
    HI_PRV HI_U8 ucIndex = 0;

    pstStaEvtInf->ulPlcEvtFullTimeStamp[ucIndex] = ulTime;
    ucIndex = (HI_U8)((ucIndex + 1)%(sizeof(pstStaEvtInf->ulPlcEvtFullTimeStamp)/sizeof(HI_U32)));
}

#define mrsDfxGetStaEvtMonInf() (&dfx_module.app_sta_evt_mon_inf)

HI_VOID mrsDfxStaEvtCurrMonStatus(HI_U8 ucStatus)
{
    HI_DSID_APP_STA_EVT_MONITOR_INF_S *pstStaEvtMonInf = mrsDfxGetStaEvtMonInf();

    pstStaEvtMonInf->ucCurrStatus = ucStatus;
}

HI_VOID mrsDfxStaEvtMonExistEvtCnt(HI_VOID)
{
    HI_DSID_APP_STA_EVT_MONITOR_INF_S *pstStaEvtMonInf = mrsDfxGetStaEvtMonInf();

    pstStaEvtMonInf->usMonEvtTimes++;
}

HI_VOID mrsDfxStaEvtMonSetWaitPlcFlag(HI_BOOL bFlag)
{
    HI_DSID_APP_STA_EVT_MONITOR_INF_S *pstStaEvtMonInf = mrsDfxGetStaEvtMonInf();

    pstStaEvtMonInf->bWaitPlcFlag = bFlag;
}

HI_VOID mrsDfxStaEvtMonWaitPlcQueryCnt(HI_VOID)
{
    HI_DSID_APP_STA_EVT_MONITOR_INF_S *pstStaEvtMonInf = mrsDfxGetStaEvtMonInf();

    pstStaEvtMonInf->ucPlcQuery645times++;
}

HI_VOID mrsDfxStaEvtMonTimeStamp(HI_U32 ulTime)
{
    HI_DSID_APP_STA_EVT_MONITOR_INF_S *pstStaEvtMonInf = mrsDfxGetStaEvtMonInf();
    HI_PRV HI_U8 ucIndex = 0;

    pstStaEvtMonInf->ulMonEvtTimeStamp[ucIndex] = ulTime;
    ucIndex = (HI_U8)((ucIndex + 1)%(sizeof(pstStaEvtMonInf->ulMonEvtTimeStamp)/sizeof(HI_U32)));
}

HI_VOID mrsDfxStaEvtMonStartTimeStamp(HI_U32 ulTime)
{
    HI_DSID_APP_STA_EVT_MONITOR_INF_S *pstStaEvtMonInf = mrsDfxGetStaEvtMonInf();
    HI_PRV HI_U8 ucIndex = 0;

    pstStaEvtMonInf->ulMonEvtStartTimeStamp[ucIndex] = ulTime;
    ucIndex = (HI_U8)((ucIndex + 1)%(sizeof(pstStaEvtMonInf->ulMonEvtStartTimeStamp)/sizeof(HI_U32)));
}

HI_VOID mrsDfxStaEvtMonEndTimeStamp(HI_U32 ulTime)
{
    HI_DSID_APP_STA_EVT_MONITOR_INF_S *pstStaEvtMonInf = mrsDfxGetStaEvtMonInf();
    HI_PRV HI_U8 ucIndex = 0;

    pstStaEvtMonInf->ulMonEvtEndTimeStamp[ucIndex] = ulTime;
    ucIndex = (HI_U8)((ucIndex + 1)%(sizeof(pstStaEvtMonInf->ulMonEvtEndTimeStamp)/sizeof(HI_U32)));
}

HI_VOID mrsDfxStaPowerRxPlcLenErrCnt(HI_VOID)
{
    HI_DSID_APP_STA_POWER_ON_OFF_INF_S *pstStaPowerInf = mrsDfxGetStaPowerInf();

    pstStaPowerInf->usRxPlcLenErrCnt++;
}

HI_VOID mrsDfxStaRxPlcCltPowerOffCnt(HI_VOID)
{
    HI_DSID_APP_STA_POWER_ON_OFF_INF_S *pstStaPowerInf = mrsDfxGetStaPowerInf();

    pstStaPowerInf->usRxPlcCltPowerOffCnt++;
}

HI_VOID mrsDfxStaCurrPowerOffMeterCnt(HI_U16 usNum)
{
    HI_DSID_APP_STA_POWER_ON_OFF_INF_S *pstStaPowerInf = mrsDfxGetStaPowerInf();

    pstStaPowerInf->usCurrPowerOffMeterCnt = usNum;
}

HI_VOID mrsDfxStaFilterPowerOffMeterCnt(HI_VOID)
{
    HI_DSID_APP_STA_POWER_ON_OFF_INF_S *pstStaPowerInf = mrsDfxGetStaPowerInf();

    pstStaPowerInf->usFilterPowerOffMeterCnt++;
}

HI_VOID mrsDfxStaCurrPowerStatus(HI_U8 usStatus)
{
    HI_DSID_APP_STA_POWER_ON_OFF_INF_S *pstStaPowerInf = mrsDfxGetStaPowerInf();

    pstStaPowerInf->ucCurrPowerStatus = usStatus;
}

HI_VOID mrsDfxStaCurrPowerOffPlcTxTurn(HI_U8 ucTurn)
{
    HI_DSID_APP_STA_POWER_ON_OFF_INF_S *pstStaPowerInf = mrsDfxGetStaPowerInf();

    pstStaPowerInf->ucCurrPowerOffPlcTxTurn = ucTurn;
}

HI_VOID mrsDfxStaTxOtherPowerOffPlcCnt(HI_VOID)
{
    HI_DSID_APP_STA_POWER_ON_OFF_INF_S *pstStaPowerInf = mrsDfxGetStaPowerInf();

    pstStaPowerInf->usTxOtherPowerOffPlcCnt++;
}

HI_VOID mrsDfxCltPowerOnPlcTxTime(HI_U32 ulTime)
{
    HI_DSID_APP_STA_POWER_ON_OFF_INF_S *pstStaPowerInf = mrsDfxGetStaPowerInf();

    pstStaPowerInf->ulCltPowerOnPlcTxTime = ulTime;
}

HI_VOID mrsDfxCltPowerOffOnFlag(HI_VOID)
{
    HI_DSID_APP_STA_POWER_ON_OFF_INF_S *pstStaPowerInf = mrsDfxGetStaPowerInf();

    pstStaPowerInf->ucPowerOffOnFlag = HI_TRUE;
}

HI_VOID mrsDfxCltPowerOnReportMeterNum(HI_U8 ucNum)
{
    HI_DSID_APP_STA_POWER_ON_OFF_INF_S *pstStaPowerInf = mrsDfxGetStaPowerInf();

    pstStaPowerInf->ucPowerOnReportMeterNum = ucNum;
}

HI_VOID mrsDfxCltPowerOnTxPlcTimes(HI_VOID)
{
    HI_DSID_APP_STA_POWER_ON_OFF_INF_S *pstStaPowerInf = mrsDfxGetStaPowerInf();

    pstStaPowerInf->ucCltPowerOnTxPlcTimes++;
}

HI_VOID mrsDfxStaTxOtherPowerOffPlcTryCnt(HI_VOID)
{
    HI_DSID_APP_STA_POWER_ON_OFF_INF_S *pstStaPowerInf = mrsDfxGetStaPowerInf();

    pstStaPowerInf->usTxOtherPowerOffPlcTryCnt++;
}
#endif


#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
#define mrsDfxGetNodeStateChgInf() (&dfx_module.app_cco_node_state_chg_inf)

//*****************************************************************************
// 函数名称: mrsDfxCcoSaveMACReportChgEntry
// 功能描述: 从节点在线状态变化上报可维可测: MAC通过回调函数传入变化信息条目时，
//           保存下来最近的几条
//
// 参数说明:
//   HI_MAC_NETWORK_TOPOCHG_ENTRY_S* pstEntry
//   MAC通过回调函数传入从节点在线状态变化信息条目
//
// 返 回 值:
//   HI_ERR_FAILURE         上下文错误或参数错误
//   HI_ERR_SUCCESS         保存成功
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: liupeidong/00238134 [2015-03-26]
//*****************************************************************************
HI_U32 mrsDfxCcoSaveMACReportChgEntry(HI_MDM_NETWORK_TOPOCHG_ENTRY_S* pstEntry)
{
    HI_DSID_APP_CCO_NODE_STATECHG_INF_S* pstInf = mrsDfxGetNodeStateChgInf();
    HI_DFX_MAC_REPORT_ENTRY* pstDfxEntry = HI_NULL;

    if (HI_NULL == pstEntry)
    {
        return HI_ERR_FAILURE;
    }

    (hi_void)memmove_s(&pstInf->astEntry[1],
        (HI_DFX_MRS_SAVE_ENTRY_NUM - 1) * sizeof(HI_DFX_MAC_REPORT_ENTRY),
        &pstInf->astEntry[0],
        (HI_DFX_MRS_SAVE_ENTRY_NUM - 1) * sizeof(HI_DFX_MAC_REPORT_ENTRY));

    pstDfxEntry = &(pstInf->astEntry[0]);
    pstDfxEntry->ulMACReportTS = HI_MDM_GetMilliSeconds();
    memcpy_s(pstDfxEntry->aucMac, HI_PLC_MAC_ADDR_LEN, pstEntry->aucMac, HI_PLC_MAC_ADDR_LEN);
    pstDfxEntry->ucStateChange = pstEntry->ucStateChange;
    pstDfxEntry->ucOfflineReason = pstEntry->ucOfflineReason;
    pstDfxEntry->ulOfflineDuration = pstEntry->ulOfflineDuration;

    pstInf->ulMACReportEntryCnt++;
    if (HI_ND_STATE_CHANGE_ONLINE_TO_OFFLINE == pstEntry->ucStateChange)
    {
        pstInf->ulOnlineToOfflineEntryCnt++;
    }
    else if (HI_ND_STATE_CHANGE_OFFLINE_TO_ONLINE == pstEntry->ucStateChange)
    {
        pstInf->ulOfflineToOnlineEntryCnt++;
    }

    return HI_ERR_SUCCESS;
}


//*****************************************************************************
// 函数名称: mrsDfxCcoChgFrameQueueStats
// 功能描述: 在帧队列中的条目数
//
// 参数说明:
//   HI_BOOL bEnQueue HI_TRUE为进队列；HI_FALSE为出队列
//   HI_U8   ucEntryNum 条目数
//
// 返 回 值:
//   无
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: liupeidong/00238134 [2015-03-26]
//*****************************************************************************
HI_VOID mrsDfxCcoChgFrameQueueStats(HI_BOOL bEnQueue, HI_U8 ucEntryNum)
{
    HI_DSID_APP_CCO_NODE_STATECHG_INF_S* pstInf = mrsDfxGetNodeStateChgInf();

    if (bEnQueue)
    {
        pstInf->ulInFrameQueueEntryNum += ucEntryNum;
        pstInf->ulFrameCnt++;
        pstInf->ulFrameQueueLen++;
    }
    else
    {
        pstInf->ulInFrameQueueEntryNum -= ucEntryNum;
        pstInf->ulFrameQueueLen--;
    }
}


//*****************************************************************************
// 函数名称: mrsDfxCcoChgJoinCcoQueueStats
// 功能描述: 加入CCO公共队列的相关统计量
//
// 参数说明:
//   HI_U8   ucEntryNum 条目数
//
// 返 回 值:
//   无
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: liupeidong/00238134 [2015-03-26]
//*****************************************************************************
HI_VOID mrsDfxCcoChgJoinCcoQueueStats(HI_U8 ucEntryNum)
{
    HI_DSID_APP_CCO_NODE_STATECHG_INF_S* pstInf = mrsDfxGetNodeStateChgInf();

    pstInf->ulJoinCcoQueueLTS = HI_MDM_GetMilliSeconds();
    pstInf->ulInCcoQueueEntryNum += ucEntryNum;
    pstInf->ulJoinCcoQueueEntryCnt += ucEntryNum;
    pstInf->ulJoinCcoQueueFrameCnt++;
    pstInf->ulInCcoQueueFrameNum++;
}


//*****************************************************************************
// 函数名称: mrsDfxCcoChgReportRxResp
// 功能描述: 收到确认帧/否认帧的相关统计量
//
// 参数说明:
//   无
//
// 返 回 值:
//   无
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: liupeidong/00238134 [2015-03-26]
//*****************************************************************************
HI_VOID mrsDfxCcoChgReportRxResp(HI_VOID)
{
    HI_DSID_APP_CCO_NODE_STATECHG_INF_S* pstInf = mrsDfxGetNodeStateChgInf();

    pstInf->ulReportRxRespLTS = HI_MDM_GetMilliSeconds();
    pstInf->ulReportRxRespFrameCnt++;
    pstInf->ulInCcoQueueFrameNum--;
    pstInf->ulInCcoQueueEntryNum = 0;
}


//*****************************************************************************
// 函数名称: mrsDfxCcoChgReportTimeout
// 功能描述: 上报超时相关的统计量
//
// 参数说明:
//   无
//
// 返 回 值:
//   无
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: liupeidong/00238134 [2015-03-26]
//*****************************************************************************
HI_VOID mrsDfxCcoChgReportTimeout(HI_VOID)
{
    HI_DSID_APP_CCO_NODE_STATECHG_INF_S* pstInf = mrsDfxGetNodeStateChgInf();

    pstInf->ulReportTimeoutLTS = HI_MDM_GetMilliSeconds();
    pstInf->ulReportTimeoutFrameCnt++;
    pstInf->ulInCcoQueueFrameNum--;
    pstInf->ulInCcoQueueEntryNum = 0;
}

#define mrsDfxGetCcoDetectInf() (&dfx_module.app_cco_detect_rm_inf)

HI_VOID mrsDfxCcoDetectEnable(HI_U8 ucEnable)
{
    HI_DSID_APP_CCO_DETECT_RM_INF_S *pstInf = mrsDfxGetCcoDetectInf();

    pstInf->ucEnable = ucEnable;
}

HI_VOID mrsDfxCcoDetectStatus(HI_U8 ucDetectStatus)
{
    HI_DSID_APP_CCO_DETECT_RM_INF_S *pstInf = mrsDfxGetCcoDetectInf();

    pstInf->ucDetectStatus = ucDetectStatus;
}

HI_VOID mrsDfxCcoDetectJoinWhiteFlag(HI_U8 ucJoinWhiteFlag)
{
    HI_DSID_APP_CCO_DETECT_RM_INF_S *pstInf = mrsDfxGetCcoDetectInf();

    pstInf->ucJoinWhiteFlag = ucJoinWhiteFlag;
}

HI_VOID mrsDfxCcoDetectAllowJoinWhiteReason(HI_U8 ucAllowJoinWhiteReason)
{
    HI_DSID_APP_CCO_DETECT_RM_INF_S *pstInf = mrsDfxGetCcoDetectInf();

    pstInf->ucAllowJoinWhiteReason = ucAllowJoinWhiteReason;
}

HI_VOID mrsDfxCcoDetectForbidJoinWhiteReason(HI_U8 ucForbidJoinWhiteReason)
{
    HI_DSID_APP_CCO_DETECT_RM_INF_S *pstInf = mrsDfxGetCcoDetectInf();

    pstInf->ucForbidJoinWhiteReason = ucForbidJoinWhiteReason;
}

HI_VOID mrsDfxCcoDetectAllowJoinWhiteTime(HI_U32 ulAllowJoinWhiteTime)
{
    HI_DSID_APP_CCO_DETECT_RM_INF_S *pstInf = mrsDfxGetCcoDetectInf();

    pstInf->ulAllowJoinWhiteTime = ulAllowJoinWhiteTime;
}

HI_VOID mrsDfxCcoDetectForbidJoinWhiteTime(HI_U32 ulForbidJoinWhiteTime)
{
    HI_DSID_APP_CCO_DETECT_RM_INF_S *pstInf = mrsDfxGetCcoDetectInf();

    pstInf->ulForbidJoinWhiteTime = ulForbidJoinWhiteTime;
}

HI_VOID mrsDfxCcoDetectSetWaitTopoFlag(HI_U8 ucSetWaitTopoFlag)
{
    HI_DSID_APP_CCO_DETECT_RM_INF_S *pstInf = mrsDfxGetCcoDetectInf();

    pstInf->ucSetWaitTopoFlag = ucSetWaitTopoFlag;
}

HI_VOID mrsDfxCcoDetectRmFlag(HI_U8 ucRmFlag)
{
    HI_DSID_APP_CCO_DETECT_RM_INF_S *pstInf = mrsDfxGetCcoDetectInf();

    pstInf->ucRmFlag = ucRmFlag;
}

HI_VOID mrsDfxCcoDetectExternFlag(HI_U8 ucExternFlag)
{
    HI_DSID_APP_CCO_DETECT_RM_INF_S *pstInf = mrsDfxGetCcoDetectInf();

    pstInf->ucExternFlag = ucExternFlag;
}

HI_VOID mrsDfxCcoDetectOriginalEnable(HI_U8 ucOriginalEnable)
{
    HI_DSID_APP_CCO_DETECT_RM_INF_S *pstInf = mrsDfxGetCcoDetectInf();

    pstInf->ucOriginalEnable = ucOriginalEnable;
}

HI_VOID mrsDfxCcoDetectCurrTurn(HI_U8 ucCurrTurn)
{
    HI_DSID_APP_CCO_DETECT_RM_INF_S *pstInf = mrsDfxGetCcoDetectInf();

    pstInf->ucCurrTurn = ucCurrTurn;
}

HI_VOID mrsDfxCcoDetect07DIIndex(HI_U8 uc97DIIndex)
{
    HI_DSID_APP_CCO_DETECT_RM_INF_S *pstInf = mrsDfxGetCcoDetectInf();

    pstInf->uc97DIIndex = uc97DIIndex;
}

HI_VOID mrsDfxCcoDetectWhiteNum(HI_U16 usDetectWhiteNum)
{
    HI_DSID_APP_CCO_DETECT_RM_INF_S *pstInf = mrsDfxGetCcoDetectInf();

    pstInf->usDetectWhiteNum = usDetectWhiteNum;
}

HI_VOID mrsDfxCcoTopoStatus(HI_U8 ucTopoStatus)
{
    HI_DSID_APP_CCO_DETECT_RM_INF_S *pstInf = mrsDfxGetCcoDetectInf();

    pstInf->ucTopoStatus = ucTopoStatus;
    if (HI_MDM_MAC_NETWORK_EVALUATE_END == ucTopoStatus)
    {
        pstInf->ulEvalutedTime = HI_MDM_GetMilliSeconds();
    }
}

HI_VOID mrsDfxCcoDetectPlcDisconnectTime(HI_U32 ulTime)
{
    HI_DSID_APP_CCO_DETECT_RM_INF_S *pstInf = mrsDfxGetCcoDetectInf();

    pstInf->ulPlcDisconnectTime = ulTime;
}

HI_VOID mrsDfxCcoArchivesClearTime(HI_U32 ulTime)
{
    HI_DSID_APP_CCO_DETECT_RM_INF_S *pstInf = mrsDfxGetCcoDetectInf();

    pstInf->ulArchivesClearTime = ulTime;

    if (pstInf->ucEnable && pstInf->ucDetectStatus)
    {
        mrsDfxCcoDetectEndReason(MRS_CCO_DETECT_END_REASON_ARCHIVES_CLEAR);
    }
}

HI_VOID mrsDfxCcoArchives0To1Time(HI_U32 ulTime)
{
    HI_DSID_APP_CCO_DETECT_RM_INF_S *pstInf = mrsDfxGetCcoDetectInf();

    pstInf->ulArchives0To1Time = ulTime;
}

HI_VOID mrsDfxCcoDetectExternDisableTime(HI_U32 ulTime)
{
    HI_DSID_APP_CCO_DETECT_RM_INF_S *pstInf = mrsDfxGetCcoDetectInf();

    pstInf->ulExternDisableTime = ulTime;
}

HI_VOID mrsDfxCcoDetectExternResumeTime(HI_U32 ulTime)
{
    HI_DSID_APP_CCO_DETECT_RM_INF_S *pstInf = mrsDfxGetCcoDetectInf();

    pstInf->ulExternResumeTime = ulTime;
}

HI_VOID mrsDfxCcoDetectStartTime(HI_U32 ulTime)
{
    HI_DSID_APP_CCO_DETECT_RM_INF_S *pstInf = mrsDfxGetCcoDetectInf();
    HI_PRV HI_U8 ucIndex = 0;

    pstInf->ulDetectStartTime[ucIndex] = ulTime;
    ucIndex = (HI_U8)((ucIndex + 1)%(sizeof(pstInf->ulDetectStartTime)/sizeof(HI_U32)));
}

HI_VOID mrsDfxCcoDetectEndTime(HI_U32 ulTime)
{
    HI_DSID_APP_CCO_DETECT_RM_INF_S *pstInf = mrsDfxGetCcoDetectInf();
    HI_PRV HI_U8 ucIndex = 0;

    pstInf->ulDetectEndTime[ucIndex] = ulTime;
    ucIndex = (HI_U8)((ucIndex + 1)%(sizeof(pstInf->ulDetectEndTime)/sizeof(HI_U32)));
}

HI_VOID mrsDfxCcoDetectEndReason(HI_U8 ucReason)
{
    HI_DSID_APP_CCO_DETECT_RM_INF_S *pstInf = mrsDfxGetCcoDetectInf();
    HI_PRV HI_U8 ucIndex = 0;

    pstInf->ucDetectEndReason[ucIndex] = ucReason;
    ucIndex = (HI_U8)((ucIndex + 1)%(sizeof(pstInf->ucDetectEndReason)/sizeof(HI_U8)));
}

HI_VOID mrsDfxCcoDetectBlackNum(HI_U16 usBlackListNum)
{
    HI_DSID_APP_CCO_DETECT_RM_INF_S *pstInf = mrsDfxGetCcoDetectInf();

    pstInf->usBlackListNum = usBlackListNum;
}

HI_VOID mrsDfxCcoDetectWhiteValidNum(HI_U16 usDetectWhiteValidNum)
{
    HI_DSID_APP_CCO_DETECT_RM_INF_S *pstInf = mrsDfxGetCcoDetectInf();

    pstInf->usDetectWhiteValidNum = usDetectWhiteValidNum;
}

HI_VOID mrsDfxCcoDetectRefreshWhiteFlag(HI_U8 ucFlag)
{
    HI_DSID_APP_CCO_DETECT_RM_INF_S *pstInf = mrsDfxGetCcoDetectInf();

    pstInf->ucRefreshWhiteFlag = ucFlag;
}

#endif


#define mrsDfxGetPlcFrameInf() (&dfx_module.app_plc_frame_inf)


HI_VOID mrsDfxSmFrameValid(HI_VOID)
{
    HI_DSID_APP_PLC_FRAME_STAT_INF_S *pstInf = mrsDfxGetPlcFrameInf();
    pstInf->ulSmValidFrame++;
}

HI_VOID mrsDfxSmFrameInvalid(HI_VOID)
{
    HI_DSID_APP_PLC_FRAME_STAT_INF_S *pstInf = mrsDfxGetPlcFrameInf();
    pstInf->ulSmInvalidFrame++;
}

HI_VOID mrsDfxMapFrameValid(HI_VOID)
{
    HI_DSID_APP_PLC_FRAME_STAT_INF_S *pstInf = mrsDfxGetPlcFrameInf();
    pstInf->ulMapValidFrame++;
}

HI_VOID mrsDfxMapFrameInvalid(HI_VOID)
{
    HI_DSID_APP_PLC_FRAME_STAT_INF_S *pstInf = mrsDfxGetPlcFrameInf();
    pstInf->ulMapInvalidFrame++;
}

#if defined(PRODUCT_CFG_TTCN)
#define mrsDfxGetStaTestModeInf() (&dfx_module.app_sta_csg_physicalmode_inf)
HI_VOID mrsDfxStaTestModeCnt(HI_U8 ucMode)
{
    HI_DSID_APP_CSG_PYSICAL_MODE_INF_S *pstInf = mrsDfxGetStaTestModeInf();
    pstInf->ulCnt++;
    pstInf->ucMode = ucMode;
    pstInf->ulLatestTS = HI_MDM_GetMilliSeconds();
}

HI_VOID mrsDfxStaRefreshTestMode(HI_U8 ucMode)
{
    HI_DSID_APP_CSG_PYSICAL_MODE_INF_S *pstInf = mrsDfxGetStaTestModeInf();
    pstInf->ucMode = ucMode;
}

HI_VOID mrsDfxStaTestModeEnter(HI_U8 ucMode,HI_U8 ucCurMode)
{
    HI_DSID_APP_CSG_PYSICAL_MODE_INF_S *pstInf = mrsDfxGetStaTestModeInf();

    HI_DSID_APP_CSG_TEST_MODE_INF_S *pstMode = &(pstInf->stModeInfo[ucMode]);

    pstMode->ulEnterTimeStamp = HI_MDM_GetMilliSeconds();
    pstMode->ucModeFlag = HI_TRUE;
    pstMode->ucCurMode = ucCurMode;
}

HI_VOID mrsDfxStaTestModeExit(HI_U8 ucMode,HI_U8 ucCurMode)
{
    HI_DSID_APP_CSG_PYSICAL_MODE_INF_S *pstInf = mrsDfxGetStaTestModeInf();

    HI_DSID_APP_CSG_TEST_MODE_INF_S *pstMode = &(pstInf->stModeInfo[ucMode]);

    pstMode->ulExitTimeStamp = HI_MDM_GetMilliSeconds();
    pstMode->ucModeFlag = HI_FALSE;
    pstMode->ucCurMode = ucCurMode;
}

HI_VOID mrsDfxStaTestModeErrPbbCnt(HI_U8 ucPayloadType)
{
    HI_DSID_APP_CSG_PYSICAL_MODE_INF_S *pstInf = mrsDfxGetStaTestModeInf();
    pstInf->ulPbbErrCnt++;
    pstInf->ucPayloadType = ucPayloadType;
    pstInf->ulPbbErrTS = HI_MDM_GetMilliSeconds();
}

HI_VOID mrsDfxStaTestModeErrParseCnt(HI_U32 ulRet)
{
    HI_DSID_APP_CSG_PYSICAL_MODE_INF_S *pstInf = mrsDfxGetStaTestModeInf();
    pstInf->ulParseErrAppCnt++;
    pstInf->ulRet = ulRet;
    pstInf->ulParseErrTS = HI_MDM_GetMilliSeconds();
}

HI_VOID mrsDfxStaTestModeInvalid(HI_U8 ucPortNum, HI_U8 ucTestId)
{
    HI_DSID_APP_CSG_PYSICAL_MODE_INF_S *pstInf = mrsDfxGetStaTestModeInf();
    pstInf->ucInvalidPortNum = ucPortNum;
    pstInf->ucInvalidTestId = ucTestId;
    pstInf->ulEnterTs = HI_MDM_GetMilliSeconds();
}


HI_VOID mrsDfxStaTestModeAppBuf(HI_U8 *pAppData, HI_U16 usLen)
{
    HI_DSID_APP_CSG_PYSICAL_MODE_INF_S *pstInf = mrsDfxGetStaTestModeInf();
    HI_U16 usDataLen = usLen;
    if (usDataLen > HI_DFX_MRS_BUF_MAX_LEN)
    {
        usDataLen =  HI_DFX_MRS_BUF_MAX_LEN;
    }

    pstInf->usPayloadLen = usLen;

    memcpy_s(pstInf->aucAppData, usDataLen, pAppData, usDataLen);
}

HI_VOID mrsDfxStaTestModePayloadBuf(HI_U8 *payloadData, HI_U16 usLen)
{
    HI_DSID_APP_CSG_PYSICAL_MODE_INF_S *pstInf = mrsDfxGetStaTestModeInf();
    HI_U16 usDataLen = usLen;

    if (usDataLen > 2*HI_DFX_MRS_BUF_MAX_LEN)
    {
        usDataLen =  2*HI_DFX_MRS_BUF_MAX_LEN;
    }

    memcpy_s(pstInf->aucPayLoadData, usDataLen, payloadData, usDataLen);
}
#endif
#endif