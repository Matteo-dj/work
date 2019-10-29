/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2018. All rights reserved.
 * Description: 台区识别功能APP层CCO端对外接口声明
 * Author: xiangchaobin/00378718
 * Create: 2018-12-29
 * Notes: 本文件声明接口仅可在台区识别模块内部调用
 */

#ifndef __MRS_TRANSFOMER_IDENTIFY_CCO_H__
#define __MRS_TRANSFOMER_IDENTIFY_CCO_H__

#include "mrs_transfomer_identify_cmn.h"

HI_START_HEADER

HI_VOID mrsTfCcoHandleMsg(HI_SYS_QUEUE_MSG_S *pstMsg);
HI_U8 mrsTfCcoGetCollectSeq(HI_VOID);

HI_END_HEADER

#endif



