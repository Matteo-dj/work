/*
 * Copyright (C) HiSilicon Technologies Co., Ltd. 2019-2019. All rights reserved.
 * Description: NDM detect id process.
 * Author: cuiate/c00233580
 * Create: 2019-01-24
 */

#include "mrs_srv_ndm_detectid.h"
#include "mrs_common.h"
#include "mrs_cmd_msg.h"
#include "mrs_fw_proto376_2_echo.h"
#include "mrs_fw_tools.h"
#include "app_ndm_timer.h"
#include "mrs_fw_n.h"
#include "mrs_fw_proto376_2_frame.h"

#if defined(PRODUCT_CFG_PRODUCT_TYPE_NDM)
HI_START_HEADER

typedef HI_MDM_NDM_DETECTID_REPORT_INFO_ST ndm_detect_id_info;

hi_u8 g_ndm_freq_start = 15;

ndm_read_id_param g_read_id_param = {{0}, 0};

hi_u32 mrs_handle_detect_id_listend_data_state(hi_void);

hi_u32 mrs_msg_ndm_device_data_rx(hi_u16 id, hi_pvoid param)
{
    hi_u32 ret = HI_ERR_NOT_FOUND;
    MRS_MSG_MR_3762_DATA_STRU *frame_data = (MRS_MSG_MR_3762_DATA_STRU *)param;
    MRS_3762_FRAME_STRU * frame = HI_NULL;

    HI_UNREF_PARAM(id);

    if (frame_data->pucData == HI_NULL) {
        return ret;
    }

    (hi_void)MRS_Proto3762Dec(frame_data->pucData, frame_data->usDataSizse, &frame);
    if (frame != HI_NULL) {
        hi_pbyte buffer = HI_NULL;
        hi_u16 buf_len = 0;

        ret = mrs3762AfnDispatch(frame, &buffer, &buf_len, HI_FALSE);
        if ((ret == HI_ERR_SUCCESS) && (buffer != HI_NULL)) {
            ret = MRS_SendMrData(buffer, buf_len, HI_DMS_CHL_UART_PORT_APP);
        }

        mrsToolsFree(buffer);
        mrsToolsFree(frame);
    }

    return ret;
}

hi_u32 mrs_ndm_detect_id_tx(ndm_read_id_param *param)
{
    (hi_void)memcpy_s(&g_read_id_param, sizeof(g_read_id_param), param, sizeof(ndm_read_id_param));
    HI_MDM_NdmStartDetectId(g_read_id_param.node, g_ndm_freq_start);
    APP_NdmTimerStart(MRS_NDM_TIMER_READ_DEV_ID, 10000, HI_SYS_TIMER_ONESHOT);
    return HI_ERR_SUCCESS;
}


hi_u32 mrs_ndm_detect_id_state_report(hi_u8 type, HI_MDM_NDM_DETECTID_REPORT_INFO_ST *cmd, hi_u32 cmd_size)
{
    HI_SYS_QUEUE_MSG_S msg;

    if (cmd_size != sizeof(HI_MDM_NDM_DETECTID_REPORT_INFO_ST)) {
        return HI_ERR_INVALID_PARAMETER;
    }

    if (cmd->ndm_state != NDM_DETECTID_NDM_LISTEN_DATA_STATE) {
        return HI_ERR_SUCCESS;
    }

    (hi_void)memset_s(&msg, sizeof(msg), 0, sizeof(msg));
    msg.ulMsgId = EN_MRS_FW_MSG_DETECTID;
    msg.ulParam[0] = (hi_u32)type;
    (hi_void)memcpy_s(&msg.ulParam[1], sizeof(hi_u32), cmd, sizeof(HI_MDM_NDM_DETECTID_REPORT_INFO_ST));

    return mrsSendMessage2Queue(&msg);
}

hi_u32 mrs_ndm_handle_detect_id(HI_SYS_QUEUE_MSG_S *msg)
{
    ndm_detect_id_info *info = (ndm_detect_id_info *)&msg->ulParam[1];

    if (info->ndm_state != NDM_DETECTID_NDM_LISTEN_DATA_STATE) {
        return HI_ERR_NOT_SUPPORT;
    }

    return mrs_handle_detect_id_listend_data_state();
}

hi_u32 mrs_handle_detect_id_listend_data_state(hi_void)
{
    MRS_PLC_FRAME_DATA_S plc;
    mrs_plc_dev_id_info_dl info;

    (hi_void)memset_s(&info, sizeof(info), 0, sizeof(info));
    info.stru_ver = MRS_CMD_STRU_VER_R1;
    info.stru_len = sizeof(info);
    info.dir_flag = MRS_PLC_DL_FLG;
    info.id_type = g_read_id_param.id_type;
    info.seq = g_read_id_param.seq;

    (hi_void)memset_s(&plc, sizeof(plc), 0, sizeof(plc));
    (hi_void)memcpy_s(plc.ucMac, sizeof(plc.ucMac), g_read_id_param.node, sizeof(g_read_id_param.node));
    plc.usId = ID_MRS_CMD_GET_DEVICE_ID_PLC;
    plc.usPayloadLen = sizeof(info);
    plc.pucPayload = (hi_pbyte)&info;
    
    return MRS_SendPlcFrame(&plc);
}

hi_u32 mrs_ndm_plc_read_id_timeout(hi_void)
{
    mrs_plc_detect_id_timeout();
    HI_MDM_NdmDetectIdSetState(NDM_DETECTID_NDM_FINISH_DETECT_STATE);

    return HI_ERR_SUCCESS;
}

hi_u32 mrs_ndm_handle_plc_detect_id(hi_u16 id, hi_pvoid packet, hi_u16 packet_size)
{
    MRS_PLC_FRAME_DATA_STRU *plc_data = (MRS_PLC_FRAME_DATA_STRU *)packet;
    mrs_plc_dev_id_info_up *dev_info = HI_NULL;
	afn_10_f40_ul_data_param ul_data;
	hi_pbyte frame_buf = HI_NULL;
	hi_u16 frame_len = 0;
    hi_u16 up_len = 0;
    hi_u32 ret = HI_ERR_SUCCESS;
    PROTO376_2_FRAME_PRE_STRU *read_id_frame = HI_NULL;

    dev_info = (mrs_plc_dev_id_info_up *)plc_data->pucPayload;

    if (dev_info->id_len > MANAGEMENT_ID_SIZE) {
        return HI_ERR_INVALID_PARAMETER;
    }

    up_len = sizeof(mrs_plc_dev_id_info_up) + dev_info->id_len - MANAGEMENT_ID_SIZE;
    if (up_len != plc_data->usPayloadLen) {
        return HI_ERR_INVALID_PARAMETER;
    }

    if (dev_info->dir_flag != MRS_PLC_UP_FLG) {
        return HI_ERR_INVALID_PARAMETER;
    }

    if (memcmp(plc_data->ucMac, g_read_id_param.node, sizeof(g_read_id_param.node)) != 0) {
        return HI_ERR_INVALID_PARAMETER;
    }

    read_id_frame = (PROTO376_2_FRAME_PRE_STRU *)mrs_get_read_id_frame();
    if (read_id_frame == HI_NULL) {
        return HI_ERR_FAILURE;
    }

    APP_NdmTimerStop(MRS_NDM_TIMER_READ_DEV_ID);
    HI_MDM_NdmDetectIdSetState(NDM_DETECTID_NDM_FINISH_DETECT_STATE);

    /* ×éÖ¯1376.2Ó¦´ðÖ¡ */
    (hi_void)memset_s(&ul_data, sizeof(ul_data), 0, sizeof(ul_data));
    ul_data.ul_param.dev_type = dev_info->id_info[dev_info->id_len];
    ul_data.ul_param.id_type = dev_info->id_type;
    (hi_void)memcpy_s(ul_data.ul_param.node_addr, sizeof(ul_data.ul_param.node_addr), plc_data->ucMac, sizeof(plc_data->ucMac));
    mrsHexInvert(ul_data.ul_param.node_addr, sizeof(ul_data.ul_param.node_addr));

	ul_data.id_len = dev_info->id_len;
    (hi_void)memcpy_s(ul_data.id_info, sizeof(ul_data.id_info), dev_info->id_info, dev_info->id_len);

    (hi_void)memcpy_s(read_id_frame->ucSrcAdd, HI_PLC_MAC_ADDR_LEN, ul_data.ul_param.node_addr, HI_PLC_MAC_ADDR_LEN);
	read_id_frame->stAfnData.usDataLen = sizeof(ul_data.ul_param) + sizeof(ul_data.id_len) + ul_data.id_len;
	read_id_frame->stAfnData.pData = (HI_U8*)&ul_data;

	ret = mrsCreate3762Frame(read_id_frame, &frame_buf, &frame_len, HI_NULL);
	if(ret == HI_ERR_SUCCESS) {
    	ret = MRS_SendMrData(frame_buf, frame_len, HI_DMS_CHL_UART_PORT_APP);
	}

    mrsToolsFree(frame_buf);
    mrs_free_read_id_frame();

    return ret;
}

hi_u32 mrs_ndm_set_freq(hi_u8 freq)
{
    hi_u8 freq_array[4] = {10, 12, 15, 16};

    /*
     * Æµ¶Î
     * 0: Æµ¶Î10
     * 1: Æµ¶Î12
     * 2: Æµ¶Î15
     * 3: Æµ¶Î16
     */
    if (freq > 3) {
        return HI_ERR_INVALID_PARAMETER;
    }

    g_ndm_freq_start = freq_array[freq];

    return HI_ERR_SUCCESS;
}

HI_END_HEADER
#endif /* defined(PRODUCT_CFG_PRODUCT_TYPE_NDM) */

