#include <hi_types.h>

#ifdef PRODUCT_CFG_SUPPORT_RF_DEBUG
#include "drv_dfx_cmn_inc.h"

#include <radio_comm.h>
#include <rf_api_lib.h>


/*用于统计等待cts次数*/
extern HI_RF_CMD_STAT_ST g_rf_cmd_stat;
/*用于统计执行指令顺序*/
extern HI_RF_CMD_ORDER_ST g_rf_cmd_order;
extern HI_U8 g_rf_pa_pwr_lvl;

/*rf_cmd_operate参数含义*/
HI_PRV HI_CONST HI_CHAR rf_help[] = "0:help\r\n"\
"1:cfg_init\r\n"\
"2:part_info\r\n"\
"3:start_tx\r\n"\
"4:start_rx\r\n"\
"5:get_int\r\n"\
"6:change_state\r\n"\
"7:fifo_info\r\n"\
"8:get_property\r\n"\
"9:set_property\r\n"\
"a:get_rssi\r\n"\
"b:chip_ready\r\n"\
"c:contrl_pa\r\n"\
"d:set_power\r\n"\
"e:cfg_gpio\r\n"\
"f:write_fifo\r\n"\
"g:read_fifo\r\n";
#ifdef WAIT_LITEOS_TO_DO 
HI_PRV HI_BOOL s_rf_operate_enable = HI_FALSE;
#endif
/*rf_cmd_operate入参*/
HI_U32 g_rf_input[RF_INPUT_PARAM_NUMBER];
/*rf_cmd_operate出参*/
HI_U8 g_rf_output[RF_OUTPUT_PARAM_NUMBER];

HI_U32 dfx_rf_cmd_stat(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption)
{
    HI_DIAG_ReportPacket(usId, (HI_U8)ulOption, (HI_PBYTE)(&g_rf_cmd_stat), sizeof(HI_RF_CMD_STAT_ST), HI_TRUE);

    return HI_ERR_SUCCESS;
}

HI_U32 dfx_rf_cmd_order(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption)
{
    HI_DIAG_ReportPacket(usId, (HI_U8)ulOption, (HI_PBYTE)(&g_rf_cmd_order), sizeof(HI_RF_CMD_ORDER_ST), HI_TRUE);

    return HI_ERR_SUCCESS;
}

HI_U32 dfx_rf_cmd_op_enable(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption)
{
    HI_CHAR buf[20]={0};

    if ((*(HI_CHAR*)pCmdParam) == '1')
    {
#ifdef WAIT_LITEOS_TO_DO 
        s_rf_operate_enable = HI_TRUE;
#endif
        HI_DIAG_ReportPacket(usId, (HI_U8)ulOption, (HI_PBYTE)"Enable OK", sizeof("Enable OK"), HI_TRUE);
        (hi_void)snprintf_s(buf, sizeof(buf), sizeof(buf) - 1, "In:%x", g_rf_input);
        HI_DIAG_ReportPacket(usId, (HI_U8)ulOption, (HI_PBYTE)(buf), sizeof(buf), HI_TRUE);
        (hi_void)snprintf_s(buf, sizeof(buf), sizeof(buf) - 1, "Out:%x", g_rf_output);
        HI_DIAG_ReportPacket(usId, (HI_U8)ulOption, (HI_PBYTE)(buf), sizeof(buf), HI_TRUE);
    }
    else
    {
#ifdef WAIT_LITEOS_TO_DO 
        s_rf_operate_enable = HI_FALSE;
#endif
        HI_DIAG_ReportPacket(usId, (HI_U8)ulOption, (HI_PBYTE)"Disable OK",
            sizeof("Disable OK"), HI_TRUE);
    }
    return HI_ERR_SUCCESS;
}

HI_U32 dfx_rf_cmd_operate(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption)
{
#ifdef WAIT_LITEOS_TO_DO     
    HI_CHAR buf[10]={0};
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U32 old_level;
    HI_BOOL print_help = HI_FALSE;

    if (s_rf_operate_enable == HI_FALSE)
    {
        HI_DIAG_ReportPacket(usId, (HI_U8)ulOption, (HI_PBYTE)"Need Input:rf_cmd_enable 1",
            sizeof("Need Input:rf_cmd_enable 1"), HI_TRUE);
        return HI_ERR_SUCCESS;
    }

    old_level = HI_SYS_ControlInterrupts(HI_SYS_DISABLE_INTERRUPTS);
    switch(*(HI_CHAR*)pCmdParam)//修改命令需要对应修改:rf_help
    {
    case '1':
        ret = rf_configuration_init((HI_RF_RATE_E)g_rf_input[0], (HI_RF_INT_CALLBACK)g_rf_input[1]);
        break;
    case '2':
        ret = rf_get_part_info((REPLY_PART_INFO_MAP *)g_rf_output);
        break;
    case '3':
        ret = rf_start_tx((HI_U8)g_rf_input[0], (HI_U16)g_rf_input[1],(HI_BOOL)g_rf_input[2]);
        break;
    case '4':
        ret = rf_start_rx((HI_U8)g_rf_input[0], (HI_U16)g_rf_input[1],(HI_BOOL)g_rf_input[2]);
        break;
    case '5':
        ret = rf_get_int_status((HI_U8)g_rf_input[0], (HI_U8)g_rf_input[1], (HI_U8)g_rf_input[2],
            (REPLY_GET_INT_STATUS_MAP*)g_rf_output);
        break;
    case '6':
        ret = rf_change_state((HI_U8)g_rf_input[0]);
        break;
    case '7':
        ret = rf_fifo_info((HI_U8)g_rf_input[0],(REPLY_FIFO_INFO_MAP *)g_rf_output);
        break;
    case '8':
        ret = rf_get_property((HI_U8)g_rf_input[0], (HI_U8)(g_rf_input[0]>>8),(HI_U8)(g_rf_input[0]>>16),
            g_rf_output);
        break;
    case '9':
        ret = rf_set_property((HI_U8)g_rf_input[0], (HI_U8)(g_rf_input[0]>>8),(HI_U8)(g_rf_input[0]>>16),
            (HI_U8*)&(g_rf_input[1]));
        break;
    case 'a':
        ret = rf_get_rssi(g_rf_output);
        break;
    case 'b':
        ret = rf_chip_ready();
        break;
    case 'c':
        ret = rf_set_pa_mode((HI_U8)g_rf_input[0]);
        break;
    case 'd':
        (HI_VOID)rf_set_power_value((HI_U8)g_rf_input[0]);
        break;
    case 'e':
        ret = rf_config_gpio_pin((HI_RF_GPIO_CFG_S*)g_rf_input);
        break;
    case 'f':
        ret = rf_write_tx_fifo((HI_U8)g_rf_input[0], (HI_U8*)g_rf_output);
        break;
    case 'g':
        ret = rf_read_rx_fifo((HI_U8)g_rf_input[0], (HI_U8*)g_rf_output);
        break;
    case '0':
    default:
        print_help = HI_TRUE;
        break;
    }
    HI_SYS_ControlInterrupts(old_level);
    if (print_help == HI_TRUE)
    {
        HI_DIAG_ReportPacket(usId, (HI_U8)ulOption, (HI_PBYTE)(rf_help), sizeof(rf_help), HI_TRUE);
    }
    else//其他命令返回执行值
    {
        (hi_void)snprintf_s(buf,sizeof(buf), sizeof(buf) - 1,"%d", ret);
        HI_DIAG_ReportPacket(usId, (HI_U8)ulOption, (HI_PBYTE)(buf), sizeof(buf), HI_TRUE);
    }
#endif
    return HI_ERR_SUCCESS;
}
#endif//PRODUCT_CFG_SUPPORT_RF_DEBUG

