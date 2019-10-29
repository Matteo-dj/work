#ifndef __HI_SAL_CFG_H__
#define __HI_SAL_CFG_H__
#include <hi_types.h>
#include <hi_mdm_types.h>

#define RAM_SIZE_8M  (0x800000)
#define RAM_SIZE_2M  (0x200000)
#define RAM_SIZE_32M  (0x2000000)



#define ARM_AHB_266M        (266000000)
#define ARM_AHB_133M        (133000000)
#define ARM_APB_66_5M       (66500000)
#define ARM_APB_33_25M      (33250000)
#define ARM_AHB_75M         (75000000)
#define ARM_APB_37_5M       (37500000)

#ifdef PRODUCT_CFG_BOARD_TYPE_ASIC
#ifdef PRODUCT_CFG_PRODUCT_TYPE_STA
#define HW_RAM_DEFAULT_SIZE  (RAM_SIZE_2M)//默认硬件内存大小
#define USE_RAM_DEFAULT_SIZE (RAM_SIZE_2M)//默认软件适用内存大小
#define DEFAULT_ETH_ENABLE_STAT HI_FALSE//默认EHT使能状态
#else//PRODUCT_CFG_PRODUCT_TYPE_STA
#define HW_RAM_DEFAULT_SIZE  (RAM_SIZE_8M)
#define USE_RAM_DEFAULT_SIZE (RAM_SIZE_8M)
#define DEFAULT_ETH_ENABLE_STAT HI_TRUE
#endif//PRODUCT_CFG_PRODUCT_TYPE_STA
#else//PRODUCT_CFG_BOARD_TYPE_ASIC
#define HW_RAM_DEFAULT_SIZE  (RAM_SIZE_32M)//默认硬件内存大小
#define USE_RAM_DEFAULT_SIZE (RAM_SIZE_32M)//默认软件适用内存大小
#define DEFAULT_ETH_ENABLE_STAT HI_TRUE//默认EHT使能状态
#endif//PRODUCT_CFG_BOARD_TYPE_ASIC

typedef struct
{
    HI_U32 sw_ram_size;//软件使用ram大小

    HI_U32 cpu_freq:1;//HI_CFG_PERFORMANCE_HIGH:不降频 HI_CFG_PERFORMANCE_LOW:降频
    HI_U32 enable_crc_clk:1;//是否使能crc时钟
    HI_U32 enable_eth_clk:1;//是否使能eth时钟
    HI_U32 enable_cipher_clk:1;//是否使能cipher时钟
    HI_U32 enable_dt_clk:1;//是否使能dt(测试功能)时钟
    HI_U32 enable_dmac_clk:1;//是否使能dmac时钟
    HI_U32 enable_spi_clk:1;//是否使能spi时钟
    HI_U32 enable_sw_eth:1;//是否打开网口
    HI_U32 reserver:24;
    
}HI_SAL_USR_CFG_S;

typedef enum
{
    UART_CLK_XTAL,
    UART_CLK_APB    
}HI_UART_CLK_SOURCE;
/*需要在系统设置总线时钟和APB时钟之后调用，HI_MDM_SetArmAndApbClock*/
HI_EXTERN HI_VOID HI_MDM_UART_SetFreq(HI_UART_CLK_SOURCE clk_source_type);

HI_SAL_USR_CFG_S * HI_CFG_GetUserCfg(HI_VOID);

/*以下接口配置后，调用low_power_switch_clk生效，begin*/
HI_U32 HI_CFG_SetEthClkEnableEarly(HI_BOOL bEnable);
HI_U32 HI_CFG_SetCrcClkEnableEarly(HI_BOOL bEnable);
HI_U32 HI_CFG_SetCipherClkEnableEarly(HI_BOOL bEnable);
HI_U32 HI_CFG_SetDtClkEnableEarly(HI_BOOL bEnable);
HI_U32 HI_CFG_SetDmacClkEnableEarly(HI_BOOL bEnable);
HI_U32 HI_CFG_SetSpiClkEnableEarly(HI_BOOL bEnable);
HI_U32 HI_CFG_SetEthEnableEarly(HI_BOOL bEnable);
/*以上接口配置后，调用low_power_switch_clk生效，end*/

HI_BOOL HI_CFG_GetEthEnable(HI_VOID);
HI_BOOL HI_CFG_InitEarly_End(HI_VOID);
HI_VOID HI_CFG_SetPhaseEnd(HI_VOID);

#endif//__HI_SAL_CFG_H__   
