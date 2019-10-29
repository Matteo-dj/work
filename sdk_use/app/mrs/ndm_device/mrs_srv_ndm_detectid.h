/*
 * Copyright (C) HiSilicon Technologies Co., Ltd. 2019-2019. All rights reserved.
 * Description: NDM detect id process.
 * Author: cuiate/c00233580
 * Create: 2019-01-24
 */

#ifndef __MRS_SRV_NDM_DETECTID_H__
#define __MRS_SRV_NDM_DETECTID_H__

#include <hi_mdm.h>

#if defined(PRODUCT_CFG_PRODUCT_TYPE_NDM)
HI_START_HEADER

typedef struct {
    hi_u8 node[6];
    hi_u8 dev_type;
    hi_u8 id_type;
    hi_u16 seq;
    hi_u16 rsv;
} ndm_read_id_param;

hi_u32 mrs_msg_ndm_device_data_rx(hi_u16 id, hi_pvoid param);

hi_u32 mrs_ndm_detect_id_tx(ndm_read_id_param *param);

hi_u32 mrs_ndm_detect_id_state_report(hi_u8 type, HI_MDM_NDM_DETECTID_REPORT_INFO_ST *cmd, hi_u32 cmd_size);

hi_u32 mrs_ndm_handle_detect_id(HI_SYS_QUEUE_MSG_S *msg);

hi_u32 mrs_ndm_plc_read_id_timeout(hi_void);

hi_u32 mrs_ndm_set_freq(hi_u8 freq);

HI_END_HEADER
#endif /* defined(PRODUCT_CFG_PRODUCT_TYPE_NDM) */
#endif /* __MRS_SRV_NDM_DETECTID_H__ */

