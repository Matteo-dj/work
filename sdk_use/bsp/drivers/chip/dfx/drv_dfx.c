#include "drv_dfx_cmn_inc.h"
#include <diag_cmd_id.h>//for DIAG_CMD_UE_ALIVE_SPY

//各个维测模块头文件
#include "drv_dfx_dc.h"
#include "drv_dfx_flash.h"
#include "drv_dfx_net.h"
#include "drv_dfx_os_info.h"
#include "drv_dfx_pwd.h"
#include "drv_dfx_sal.h"
#include "drv_dfx_ver.h"
#include "drv_dfx_rf.h"
#include "drv_dfx_rtc.h"
#include "drv_dfx_other.h"
#include "drv_dfx_cpup.h"
#include "drv_dfx_track.h"
#include "drv_dfx_uart.h"

#define ETH_PING_CMD 0
#define ARP_CMD 0

#define CPUP_CMD  0
#define TRACE_CMD 0


HI_PRV HI_CONST HI_DIAG_INPUT_CMD_PROC_S g_drvDfxCmdTbl[] = {
#if defined (PRODUCT_CFG_HSO_COLLECT_DATA)
    {ID_DIAG_CMD_DRV_UPLOAD_DATA, ID_DIAG_CMD_DRV_UPLOAD_DATA, upload_data},
#elif defined (MAC_COLLECT_DATA_DEBUG) || defined(STA_COLLECT_200KB_DATA)
     {ID_DIAG_CMD_DRV_UPLOAD_DATA, ID_DIAG_CMD_DRV_UPLOAD_DATA, upload_data},
#endif
    {ID_DIAG_CMD_DRV_IP_READ, ID_DIAG_CMD_DRV_IP_READ, ip_read},
    {ID_DIAG_CMD_DRV_IP_WRITE,ID_DIAG_CMD_DRV_IP_WRITE,ip_write},
    {ID_DIAG_CMD_DRV_ROUTE_ADD, ID_DIAG_CMD_DRV_ROUTE_ADD, dfx_add_route},
    {ID_DIAG_CMD_DRV_ROUTE_DEL, ID_DIAG_CMD_DRV_ROUTE_DEL, dfx_del_route},
    {ID_DIAG_CMD_DRV_ROUTE, ID_DIAG_CMD_DRV_ROUTE, dfx_route},    
#if ARP_CMD
    {ID_DIAG_CMD_ARP, ID_DIAG_CMD_ARP, arp_cmd},
#endif    
    {ID_DIAG_CMD_DRV_OS_TASK,  ID_DIAG_CMD_DRV_OS_TASK,  dfx_task_show},
    {ID_DIAG_CMD_DRV_OS_TIMER, ID_DIAG_CMD_DRV_OS_TIMER, dfx_timer_show},
#if defined (PRODUCT_CFG_SUPPORT_3518) 
    {ID_DIAG_CMD_DRV_3518_SYSTEM_RST,ID_DIAG_CMD_DRV_3518_SYSTEM_RST,dfx_3518_system_rst},
    {ID_DIAG_CMD_GET_3518_VER,ID_DIAG_CMD_GET_3518_VER,dfx_get_3518_ver},
#endif
#if defined (PRODUCT_CFG_HW_RTC_SUPPORT)
    {ID_DIAG_CMD_NDM_RTC_TIME,ID_DIAG_CMD_NDM_RTC_TIME,dfx_rtc_time},
#endif    
#ifdef PRODUCT_CFG_PRODUCT_TYPE_NDM
    //{ID_DIAG_CMD_KEY_STAT,ID_DIAG_CMD_KEY_STAT,dfx_key_stat},
    {ID_DIAG_CMD_GET_NDM_WORK_MODE,ID_DIAG_CMD_GET_NDM_WORK_MODE,dfx_get_work_mode},
    {ID_DIAG_CMD_SET_NDM_WORK_MODE,ID_DIAG_CMD_SET_NDM_WORK_MODE,dfx_set_work_mode},
#endif
    {ID_DIAG_CMD_DRV_SYSTEM_RST,ID_DIAG_CMD_DRV_SYSTEM_RST,dfx_system_rst},
#if defined (PRODUCT_CFG_HSO_COLLECT_DATA)
    {ID_DIAG_CMD_DRV_UPLOAD_DATA_START,ID_DIAG_CMD_DRV_UPLOAD_DATA_STOP,upload_data_config},
#elif defined (MAC_COLLECT_DATA_DEBUG) || defined(STA_COLLECT_200KB_DATA)
    {ID_DIAG_CMD_DRV_UPLOAD_DATA_START,ID_DIAG_CMD_DRV_UPLOAD_DATA_STOP,upload_data_config},
#endif
#ifdef PRODUCT_CFG_SUPPORT_RF_DEBUG
    {ID_DIAG_CMD_RF_CMD_STAT, ID_DIAG_CMD_RF_CMD_STAT, dfx_rf_cmd_stat},
    {ID_DIAG_CMD_RF_CMD_ORDER, ID_DIAG_CMD_RF_CMD_ORDER, dfx_rf_cmd_order},
    {ID_DIAG_CMD_RF_CMD_OPERATE, ID_DIAG_CMD_RF_CMD_OPERATE, dfx_rf_cmd_operate},
    {ID_DIAG_CMD_RF_CMD_OP_ENABLE, ID_DIAG_CMD_RF_CMD_OP_ENABLE, dfx_rf_cmd_op_enable},
#endif//PRODUCT_CFG_SUPPORT_RF_DEBUG
#ifdef PRODUCT_CFG_FLASH_ERASE_COUNT
    {ID_DIAG_CMD_FLASH_ERASE_COUNT, ID_DIAG_CMD_FLASH_ERASE_COUNT, dfx_get_flash_erase_stat},
#endif//PRODUCT_CFG_FLASH_ERASE_COUNT
#ifdef PRODUCT_CFG_FLASH_PROTECT
    {ID_DIAG_CMD_FLASH_PROTECT_OP, ID_DIAG_CMD_FLASH_PROTECT_OP, dfx_flash_protect_op},
#endif
    {DIAG_CMD_UE_ALIVE_SPY, DIAG_CMD_UE_ALIVE_SPY, diag_cmd_id_ue_alive_spy},  /*hso心跳命令*/
    {DIAG_CMD_VER_QRY_1, DIAG_CMD_VER_QRY_1, newdiagVerQry},
    {DIAG_CMD_SYS_QRY_1, DIAG_CMD_SYS_QRY_1, newdiagSysObjQry},
    {ID_DIAG_CMD_NEW_SYSERR_QRY, ID_DIAG_CMD_NEW_SYSERR_QRY, new_diag_syserr_qry},
    {ID_DIAG_CMD_SDK_VER_QRY, ID_DIAG_CMD_SDK_VER_QRY, sdkVerQry},

    {ID_DIAG_CMD_CHANGE_PASS_WORD, ID_DIAG_CMD_CHANGE_PASS_WORD,dfx_change_pass_word},
    {ID_DIAG_CMD_GET_CHALLENGE, ID_DIAG_CMD_GET_CHALLENGE,dfx_get_challenge},
    {ID_DIAG_CMD_NEW_MEM_SHOW, ID_DIAG_CMD_NEW_MEM_SHOW,new_mem_show_temp},
#if ETH_PING_CMD    
    {ID_DIAG_CMD_OS_PING, ID_DIAG_CMD_OS_PING,dfx_ping},
#endif

#if CPUP_CMD
    {ID_DIAG_CPUP_START, ID_DIAG_CPUP_START,dfx_cpup_start},
    {ID_DIAG_CPUP_STOP, ID_DIAG_CPUP_STOP,dfx_cpup_end},
    {ID_DIAG_CPUP_REPORT_TOTAL_INFO, ID_DIAG_CPUP_REPORT_TOTAL_INFO,dfx_cpup_get_info},
#endif
#if TRACE_CMD
    {ID_DIAG_TRACK_START, ID_DIAG_TRACK_START,dfx_track_start},
    {ID_DIAG_TRACK_STOP, ID_DIAG_TRACK_STOP,dfx_track_end},
    {ID_DIAG_TRACK_REPORT_TOTAL_INFO, ID_DIAG_TRACK_REPORT_TOTAL_INFO,dfx_track_get_info},
 #endif   
    {ID_DIAG_CMD_GET_UART_STAT, ID_DIAG_CMD_GET_UART_STAT,dfx_get_uart_stat},

};
HI_PRV HI_CONST HI_DIAG_INPUT_CMD_PROC_S g_drvDfxCmdDremoteTbl[] = {
    {ID_DIAG_CMD_DRV_UPLOAD_DATA, ID_DIAG_CMD_DRV_OS_MAX,HI_NULL},
};

HI_U32 HI_DRV_DFX_Init(HI_VOID)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    ret += HI_DIAG_RegisterCommand(HI_DIAG_REG_CMD_TBL(g_drvDfxCmdTbl), HI_DIAG_CMD_INSTANCE_DEFAULT);
    ret += HI_DIAG_RegisterCommand(HI_DIAG_REG_CMD_TBL(g_drvDfxCmdTbl), HI_DIAG_CMD_INSTANCE_IREMOTE);    
    ret += HI_DIAG_RegisterCommand(HI_DIAG_REG_CMD_TBL(g_drvDfxCmdDremoteTbl), HI_DIAG_CMD_INSTANCE_DREMOTE);
	return ret;

}


