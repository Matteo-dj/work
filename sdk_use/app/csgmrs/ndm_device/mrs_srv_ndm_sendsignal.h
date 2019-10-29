/********************************************************************************
Copyright (C), Huawei Technologies Co., Ltd. 2012-2020. All rights reserved.
*********************************************************************************
File Name     : mrs_srv_ndm_sendsignal.h
Version       : Initial draft
Author        : HCT
Created Date  : 2016-10-13
Last Modified by: c00233580
Description   : TODO: ...
Function List : TODO: ...
Change History: TODO: ...
********************************************************************************/

#ifndef _MRS_SRV_NDM_SENDSIGNAL_H_
#define _MRS_SRV_NDM_SENDSIGNAL_H_
#include <hi_types.h>
#include <hi_mdm_types.h>
#if defined(PRODUCT_CFG_PRODUCT_TYPE_NDM)
HI_START_HEADER


#define MRS_SEND_SIGNAL_FILE_SIZE       (0x200000)  // 2M
#define MRS_SEND_SIGNAL_TIME_IN_MODE1   (60*1000)   // Continuous send time of mode 1, 1 min
#define MRS_SEND_SIGNAL_KEY_DOWN_TIME   (1000)      // Continuous key pressing time
#define MRS_SEND_SIGNAL_RESEND_MSG_TIME (10*1000)   // Time of retry to send message when send failed

#define MRS_PHY_AFE_ANALOG_TX_DB_0      (0)
#define MRS_PHY_AFE_ANALOG_TX_DB_6      (3)
#define MRS_PHY_AFE_ANALOG_TX_DB_12     (6)

typedef enum
{
    MRS_SEND_SIGNAL_MODE_NONE,      // Default stage, didn't start
    MRS_SEND_SIGNAL_MODE_0,         // Send mode 0, linedriver closed
    MRS_SEND_SIGNAL_MODE_1,         // Send mode 1, strong power, switch to mode 0 when timeout.
    MRS_SEND_SIGNAL_MODE_2,         // Send mode 2, low power, could send for a long peroid.
    MRS_SEND_SIGNAL_MODE_CHANGING,
} MRS_SEND_SIGNAL_MODE_E;

typedef enum
{
    MRS_SEND_SIGNAL_DISPLAY_NONE,       // None
    MRS_SEND_SIGNAL_DISPLAY_MODE0,      // "SG0"
    MRS_SEND_SIGNAL_DISPLAY_MODE1,      // "SG1"
    MRS_SEND_SIGNAL_DISPLAY_MODE2,      // "SG2"
    MRS_SEND_SIGNAL_DISPLAY_CHANGING,   // "SG-"
    MRS_SEND_SIGNAL_DISPLAY_FAIL,       // "E03"
} MRS_SEND_SIGNAL_DISPLAY_E;

#define MRS_SEND_SIGNAL_FAIL_DISPLAY_TIME   (2) // 2s

typedef enum
{
    MRS_SEND_SIGNAL_FAIL_REASON_NONE,   // OK
    MRS_SEND_SIGNAL_FAIL_REASON_MEMORY, // Not enough memory
    MRS_SEND_SIGNAL_FAIL_REASON_MSG_Q,  // Message queue full
} MRS_SEND_SIGNAL_FAIL_REASON_E;

typedef struct
{
    uintptr_t ulRamAddr;           // Signal file address in RAM.
    HI_U8  ucSendMode;          // Send mode
    HI_U8  ucStartFailReason;   // Reason of Start failed
    HI_U8  ucMsgSendFailCnt;    // Message count of send failed 
    HI_U8  ucStartTimerFailCnt; // Count of Start timer failed
} MRS_NDM_SEND_SIGNAL_CTX;


enum
{
    EN_NDM_MSG_SEND_SIGNAL_START_PHASE2,
    EN_NDM_MSG_SEND_SIGNAL_CHANGE_MODE_0,
    EN_NDM_MSG_SEND_SIGNAL_CHANGE_MODE_1,
    EN_NDM_MSG_SEND_SIGNAL_CHANGE_MODE_2,
    EN_NDM_MSG_SEND_SIGNAL_TIMEOUT,
};


HI_U32 mrsNdmSendSignalInit(HI_VOID);

HI_VOID mrsNdmSendSignalKeyProc(HI_BOOL bKeyPress, HI_U32 ulMs);

HI_U32 mrsNdmSendSignalMsgProc(HI_U16 usId, HI_PVOID pMsgParam);

HI_VOID mrsNdmSendSignalTimeout(HI_VOID);


HI_END_HEADER
#endif // defined(PRODUCT_CFG_PRODUCT_TYPE_NDM)

#endif // _MRS_SRV_NDM_SENDSIGNAL_H_

