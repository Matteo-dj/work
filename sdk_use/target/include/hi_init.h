
#ifndef __HI_INIT_H__
#define __HI_INIT_H__

#include "hi_ft_nv.h"

HI_EXTERN HI_U32 usr_low_power_sleep_init(HI_VOID);
HI_EXTERN HI_U32 usr_io_init(HI_VOID);
HI_EXTERN HI_U32 usr_clock_init(void);
HI_EXTERN HI_U32 APP_InitCfgEarly(void);
HI_EXTERN HI_U32 usr_mem_init(HI_IN HI_FTM_CHIP_TYPE_E enChipType, HI_OUT HI_U32 aulMemSize[2]);
HI_EXTERN HI_U32 usr_share_task_init(HI_VOID);
HI_EXTERN HI_U32 usr_share_mem_init(HI_VOID);
HI_EXTERN HI_U32 usr_lcd_init(HI_VOID);
HI_EXTERN HI_U32 usr_key_init(HI_VOID);
HI_EXTERN HI_U32 usr_rtc_init(HI_VOID);
HI_EXTERN HI_U32 usr_3518_init(HI_VOID);
HI_EXTERN HI_U32 usr_battery_init(HI_VOID);
HI_EXTERN HI_U32 usr_dc_init(HI_VOID);
#endif

