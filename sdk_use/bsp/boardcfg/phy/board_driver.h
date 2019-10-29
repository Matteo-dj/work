#ifndef _BOARD_DRIVER_H_
#define _BOARD_DRIVER_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include "hi_types.h"


#define FRAME_SEND_LENGTH_TH (5000)

typedef enum
{
    PIN_HIGH,
    PIN_LOW,
}en_pin_state;

typedef struct
{
    HI_BOOL StaPinFlag;
    HI_U8      pad[3];

    HI_U32    SysStatus;
}HI_MDM_BOARD_CTRL_INFO_S;

HI_VOID HI_MDM_GpioInit(HI_VOID);
HI_VOID HI_MDM_SetBoardCtrlInfoSysStatus(HI_U32 SysStatus);
HI_MDM_BOARD_CTRL_INFO_S* HI_MDM_GetBoardCtrlInfo(HI_VOID);
HI_VOID HI_MDM_ControlCircuitBeforeSend(HI_IN HI_U32 SendLength);
HI_VOID HI_MDM_ControlCircuitAfterSend(HI_VOID);
HI_VOID HI_MDM_StaPinControl(en_pin_state State);
HI_BOOL HI_MDM_IsNeedControlStaPin(HI_VOID);


#ifdef __cplusplus
 }
#endif

#endif

