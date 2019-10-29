/********************************************************************************
Copyright (C), Huawei Technologies Co., Ltd. 2012-2020. All rights reserved.
*********************************************************************************
File Name     : mrs_srv_ndm.h
Version       : Initial draft
Author        : HCT
Created Date  : 2016-10-11
Last Modified by: c00233580
Description   : TODO: ...
Function List : TODO: ...
Change History: TODO: ...
********************************************************************************/

#ifndef _MRS_SRV_NDM_H_
#define _MRS_SRV_NDM_H_

#include <hi_ft_nv.h>
#include <app_nv.h>
#include <hi_mdm.h>
#include "mrs_srv_ndm_sendsignal.h"
#include <hi_mdm_timer.h>
#include "app_ndm_tools.h"
#include "app_ndm_timer.h"
#include "app_ndm_msg.h"

#if defined(PRODUCT_CFG_PRODUCT_TYPE_NDM)
HI_START_HEADER





// Command 'ping'
#define ID_DIAG_CMD_NDM_CTRL_PING       (0x1C04)

// NDM message ID



// NDM product type
#define MRS_NDM_TYPE_RECEIVER       (0)
#define MRS_NDM_TYPE_TRASMMITER     (1)

// Lattice status
#define MRS_LATTICE_STATUS_FLASH    (0)
#define MRS_LATTICE_STATUS_ON       (1)
#define MRS_LATTICE_STATUS_OFF      (2)

// BQ upg file size
#define MRS_BQ_UPG_FILE_SIZE        (0x32000)

// Storage 'root' directory type
#define MRS_FMGR_PATH_TYPE_MASK     (0x0000000F)
#define MRS_FMGR_PATH_TYPE_ANY      (0x00000000)    // Any dir
#define MRS_FMGR_PATH_TYPE_SD_CARD  (0x00000001)    // SD card dir

#define MRS_NDM_EQT_PLC_TEST        (HI_MDM_NDM_PING_ID_1)

#define MRS_NDM_EQT_VERSION_1       (0x1)
#define MRS_NDM_EQUIP_TEST_VERSION  (MRS_NDM_EQT_VERSION_1)

#define MRS_KEY_TQ_FUNC3            (HI_DRV_KEY_NUM_0)
#define MRS_KEY_DATA_COLLECT        (HI_DRV_KEY_NUM_1)
#define MRS_KEY_TQ_FUNC1            (HI_DRV_KEY_NUM_3)
#define MRS_KEY_TQ_FUNC2            (HI_DRV_KEY_NUM_4)
#define MRS_KEY_BATTERY             (HI_DRV_KEY_NUM_5)
#define MRS_KEY_SIGNAL_SEND         (HI_DRV_KEY_NUM_8)
#define MRS_KEY_DATE_DISP           (HI_DRV_KEY_NUM_9)


// trace enable
#define NDM_EQUIP_TRACE_ENABLE

#define equip_Trace(fmt, ...)

typedef struct
{
    HI_U32 ulTestCount;
    HI_U32 ulSuccessCount;
    HI_U32 ulFailCount;
    HI_U8  aucMac[HI_PLC_MAC_ADDR_LEN];
    HI_U8  reserved[2];
    HI_U32 ulPlcTxRandom;
} MRS_NDM_EQT_PLC_TEST_STRU;

typedef struct
{
    HI_U32 ulKeyResponse;
    HI_U32 ulTestCount;
    HI_U32 ulTotalCount;
    HI_U32 ulKeyMask;
} MRS_NDM_EQT_KEY_TEST_STRU;

typedef struct
{
    HI_U32 ulCount;
} MRS_NDM_EQT_DISPLAY_STRU;

typedef enum
{
    MRS_NDM_BQ_UPG_STATUS_IDLE,
    MRS_NDM_BQ_UPG_STATUS_BUSY,
} MRS_NDM_BQ_UPG_STATUS_E;

typedef struct
{
    MRS_NDM_BQ_UPG_STATUS_E eStatus;
    HI_U32 ulUpgResult;
} MRS_NDM_EQT_BQ_UPG_STRU;

typedef struct
{
    HI_U16 usMainVer;
    HI_U8  ucSubVer;
    HI_U8  reserved;
    HI_U32 ulCrc;
} MRS_NDM_BQ_UPG_FILE_INFO;

typedef struct
{
    HI_U32 ulMagic;
    HI_U32 ulCrc;
    HI_U16 usMainVer;
    HI_U8  ucSubVer;
    HI_U8  reserved;
    HI_U32 ulDataSize;
    HI_U8  aucData[0];
} MRS_NDM_BQ_UPG_FILE_HEAD;

typedef struct
{
    MRS_NDM_EQT_PLC_TEST_STRU   stPlcTest;
    MRS_NDM_EQT_KEY_TEST_STRU   stKeyTest;
    MRS_NDM_EQT_DISPLAY_STRU    stDisplay;
    MRS_NDM_EQT_BQ_UPG_STRU     stBqUpg;
} MRS_NDM_EQT_CTX_STRU;

typedef struct
{
    HI_U8 ucNdmType;        // 0: receiver; 1: transmitter
    HI_U8 reserved[3];
    MRS_NDM_EQT_CTX_STRU    stEqtCtx;
} MRS_NDM_SRV_CTX_STRU;



/**
 @brief     Ndm application power on process.
 @attention \n
N/A
 @see \n
N/A
 */
HI_VOID mrsNdmPowerOnPro(HI_VOID);


/**
 @brief     Get NDM context.
 @attention \n
N/A
 @retval ::Context NDM context pointer
 @see \n
N/A
 */
MRS_NDM_SRV_CTX_STRU * mrsNdmGetContext(HI_VOID);



/**
 @brief     NDM process received transmit command.
 @attention \n
N/A
 @param[in] usId  Message ID.
 @param[in] pCmdParam  Message pointer.
 @param[in] usCmdParamSize  Message size.
 @param[in] ulOption  Message option.
 @retval ::HI_ERR_SUCCESS success
 @retval ::others failure
 @see \n
N/A
 */
HI_U32 mrsNdmRxTransmitCmd(HI_U16 usId, HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption);

HI_END_HEADER
#endif // defined(PRODUCT_CFG_PRODUCT_TYPE_NDM)

#endif //_MRS_SRV_NDM_H_

