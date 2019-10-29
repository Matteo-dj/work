
#ifndef __APP_NDM_TIMER_H__
#define __APP_NDM_TIMER_H__
#include <hi_types.h>
#include <hi_mdm_types.h>
#include <hi_mdm_msg.h>//APP_NdmTimerMessageProc
#include <hi_mdm_timer.h>


#define HI_SYS_TIMER_ONESHOT    HI_TIMER_TYPE_ONCE
#define HI_SYS_TIMER_PERIODIC   HI_TIMER_TYPE_PERIOD

typedef enum
{
    APP_OSA_TIMER_ID_00=0,
    APP_OSA_TIMER_ID_01,
    APP_OSA_TIMER_ID_02,
    APP_OSA_TIMER_ID_03,
    APP_OSA_TIMER_ID_04,
    APP_OSA_TIMER_ID_05,
    APP_OSA_TIMER_ID_06,
    APP_OSA_TIMER_ID_07,
    APP_OSA_TIMER_ID_08,
    APP_OSA_TIMER_ID_09,
    APP_OSA_TIMER_ID_10,
    APP_OSA_TIMER_ID_11,
    APP_OSA_TIMER_ID_12,
    APP_OSA_TIMER_ID_13,
    APP_OSA_TIMER_ID_14,
    APP_OSA_TIMER_ID_15,
    APP_OSA_TIMER_ID_16,
    APP_OSA_TIMER_ID_MAX,
}APP_OSA_TIMER_ID_E;//定时器ID


typedef HI_U32 (*APP_NDM_TIMER_PFN)(HI_VOID);
typedef struct
{
    HI_TIMER_HANDLE_S handle;
    APP_NDM_TIMER_PFN pFunc;
}APP_NDM_TIMER_S;

HI_EXTERN APP_NDM_TIMER_S g_stAppOsaTimer[];

HI_U32 APP_NdmTimerMessageProc(HI_SYS_QUEUE_MSG_S *pstMsg);
HI_U32 APP_NdmTimerStart(HI_U8 ucTimerId,HI_U32 ulPeriod, HI_U8 ucOption);
HI_U32 APP_NdmTimerStop(HI_U8 ucTimerId);



//定时器ID映射
#define MRS_NDM_TIMER_ZERO_CROSS                (APP_OSA_TIMER_ID_00)//EN_APP_TASK_TIMER_MRS_1
#define MRS_NDM_TIMER_PLC_TEST                  (APP_OSA_TIMER_ID_01)
#define MRS_NDM_TIMER_SSID_SET                  (APP_OSA_TIMER_ID_02)
#define MRS_NDM_TIMER_KEY_TEST                  (APP_OSA_TIMER_ID_03)
#define MRS_NDM_TIMER_BEEP_CTRL                 (APP_OSA_TIMER_ID_04)
#define MRS_NDM_TIMER_DISPLAY_FLASH             (APP_OSA_TIMER_ID_05)
#define MRS_NDM_TIMER_DISPLAY_ON                (APP_OSA_TIMER_ID_06)
#define MRS_NDM_TIMER_DISPLAY_OFF               (APP_OSA_TIMER_ID_07)
#define MRS_NDM_TIMER_DC_HSO                    (APP_OSA_TIMER_ID_08)
#define MRS_NDM_TIMER_SEND_SIGNAL               (APP_OSA_TIMER_ID_09)
#define MRS_NDM_TIMER_DISP_TIME                 (APP_OSA_TIMER_ID_10)
#define MRS_NDM_TIMER_DC_SD_PERIOD              (APP_OSA_TIMER_ID_11)
#define MRS_NDM_TIMER_DC_SD_PB_MONITR           (APP_OSA_TIMER_ID_12)
#define MRS_NDM_TIMER_DC_SD_REBOOT_CHECK        (APP_OSA_TIMER_ID_13)
#define MRS_NDM_TIMER_DC_SD_FOLLOW              (APP_OSA_TIMER_ID_14)
#define MRS_NDM_TIMER_DC_SD_ZCROSS              (APP_OSA_TIMER_ID_15)
#define MRS_NDM_TIMER_READ_DEV_ID               (APP_OSA_TIMER_ID_16)


//各个定时器时间分配
#define MRS_NDM_TIME_ZERO_CROSS                 (2000)
#define MRS_NDM_TIME_PLC_TEST                   (2000)
#define MRS_NDM_TIME_SSID_SET                   (1000)
#define MRS_NDM_TIME_BEEP_CTRL                  (100)
#define MRS_NDM_TIME_DC_SD_PB_MONITR            (10*1000)
#define MRS_NDM_TIME_DC_SD_REBOOT_CHECK         (5000)
#define MRS_NDM_TIME_DC_SD_FOLLOW               (10*1000)

#endif
