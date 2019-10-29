#ifndef _SI446X_API_LIB_H_
#define _SI446X_API_LIB_H_



#if !defined(MAC_V200_LLT)
#include <Si4438_defines.h>
#endif


typedef HI_S32 (*HI_RF_INT_CALLBACK)(HI_U32 );
typedef struct
{
    HI_U8 gpio0_cfg;
    HI_U8 gpio1_cfg;
    HI_U8 gpio2_cfg;
    HI_U8 gpio3_cfg;
    HI_U8 nirq_cfg;
    HI_U8 sdo_cfg;
    HI_U8 gen_cfg;
}HI_RF_GPIO_CFG_S;

/* 此处修改需要同步修改退避时间算法,最大不能大于HI_U8 */
typedef enum
{
    RF_CFG_RATE_MIN = 0,
    RF_120K_RATE = 1,
    RF_80K_RATE,
    RF_60K_RATE,
    RF_40K_RATE,
    RF_CFG_RATE_MAX,
}HI_RF_RATE_E;

typedef enum
{
    RF_INIT_DISABLE,
    RF_INT_ENABLE,
}HI_RF_INT_ENABLE;

HI_U32 rf_configuration_init(HI_RF_RATE_E rf_rate, HI_RF_INT_CALLBACK callback);
HI_U32 rf_start_tx(HI_U8 channel, HI_U16 tx_len);
HI_U32 rf_start_rx(HI_U8 channel, HI_U16 rx_len);
#if !defined(MAC_V200_LLT)
HI_U32 rf_get_part_info(REPLY_PART_INFO_MAP * part_info);
HI_U32 rf_get_int_status(HI_U8 ph_clr_pend, HI_U8 modem_clr_pend, HI_U8 chip_clr_pend,
       REPLY_GET_INT_STATUS_MAP *int_status);
HI_U32 rf_fifo_info(HI_U8 fifo_reset,REPLY_FIFO_INFO_MAP *fifo_info);
#endif
HI_U32 rf_change_state(HI_U8 new_state);
HI_U32 rf_write_tx_fifo(HI_U8 byte_num, HI_U8* data);
HI_U32 rf_read_rx_fifo(HI_U8 byte_num, HI_U8* data);
HI_U32 rf_set_property(HI_U8 group, HI_U8 num_props, HI_U8 start_prop,HI_U8 *property_cfg);
HI_U32 rf_get_property(HI_U8 group, HI_U8 num_props, HI_U8 start_prop,HI_U8 *property_cfg);
HI_U32 rf_config_gpio_pin(HI_RF_GPIO_CFG_S *gpio_cfg);
HI_U32 rf_get_rssi(HI_U8 *val);
HI_U32 rf_chip_ready(HI_VOID);
HI_U32 rf_set_pa_mode(HI_U8 option);
HI_VOID rf_set_power_value(HI_U8 power_value);
HI_U32 rf_switch_modem_type_to_gfsk(HI_VOID);
HI_VOID rf_set_modem_type(HI_IN HI_U8 modem_type);

#ifdef PRODUCT_CFG_SUPPORT_RF_DEBUG
HI_VOID rf_register_cmd(HI_VOID);
#endif//PRODUCT_CFG_SUPPORT_RF_DEBUG

#endif
