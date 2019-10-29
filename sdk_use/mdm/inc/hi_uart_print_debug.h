#ifndef __HI_UART_DEBUG_H__
#define __HI_UART_DEBUG_H__
/*
#define HI_PrintErr         HI_UART_Printf 
#define HI_PrintWarning     HI_UART_Printf
#define HI_PrintInfo        HI_UART_Printf//正常信息
#define HI_PrintDebug       HI_UART_Printf//调试用
*/
#define HI_PrintErr(fmt, ...)
#define HI_PrintWarning(fmt, ...)
#define HI_PrintInfo(fmt, ...)
#define HI_PrintDebug(fmt, ...)


//FMGR 打印宏开关
#define HI_FMGR_PrintErr     HI_PrintErr
#define HI_FMGR_PrintWarning HI_PrintWarning
#define HI_FMGR_PrintInfo    HI_PrintInfo    
#define HI_FMGR_PrintDebug   HI_PrintDebug

#define HI_SD_DC_PrintErr     HI_PrintErr
#define HI_SD_DC_PrintWarning HI_PrintWarning
#define HI_SD_DC_PrintInfo    HI_PrintInfo    
#define HI_SD_DC_PrintDebug   HI_PrintDebug



#endif
