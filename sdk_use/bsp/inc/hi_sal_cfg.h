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
#define HW_RAM_DEFAULT_SIZE  (RAM_SIZE_2M)//Ĭ��Ӳ���ڴ��С
#define USE_RAM_DEFAULT_SIZE (RAM_SIZE_2M)//Ĭ����������ڴ��С
#define DEFAULT_ETH_ENABLE_STAT HI_FALSE//Ĭ��EHTʹ��״̬
#else//PRODUCT_CFG_PRODUCT_TYPE_STA
#define HW_RAM_DEFAULT_SIZE  (RAM_SIZE_8M)
#define USE_RAM_DEFAULT_SIZE (RAM_SIZE_8M)
#define DEFAULT_ETH_ENABLE_STAT HI_TRUE
#endif//PRODUCT_CFG_PRODUCT_TYPE_STA
#else//PRODUCT_CFG_BOARD_TYPE_ASIC
#define HW_RAM_DEFAULT_SIZE  (RAM_SIZE_32M)//Ĭ��Ӳ���ڴ��С
#define USE_RAM_DEFAULT_SIZE (RAM_SIZE_32M)//Ĭ����������ڴ��С
#define DEFAULT_ETH_ENABLE_STAT HI_TRUE//Ĭ��EHTʹ��״̬
#endif//PRODUCT_CFG_BOARD_TYPE_ASIC

typedef struct
{
    HI_U32 sw_ram_size;//���ʹ��ram��С

    HI_U32 cpu_freq:1;//HI_CFG_PERFORMANCE_HIGH:����Ƶ HI_CFG_PERFORMANCE_LOW:��Ƶ
    HI_U32 enable_crc_clk:1;//�Ƿ�ʹ��crcʱ��
    HI_U32 enable_eth_clk:1;//�Ƿ�ʹ��ethʱ��
    HI_U32 enable_cipher_clk:1;//�Ƿ�ʹ��cipherʱ��
    HI_U32 enable_dt_clk:1;//�Ƿ�ʹ��dt(���Թ���)ʱ��
    HI_U32 enable_dmac_clk:1;//�Ƿ�ʹ��dmacʱ��
    HI_U32 enable_spi_clk:1;//�Ƿ�ʹ��spiʱ��
    HI_U32 enable_sw_eth:1;//�Ƿ������
    HI_U32 reserver:24;
    
}HI_SAL_USR_CFG_S;

typedef enum
{
    UART_CLK_XTAL,
    UART_CLK_APB    
}HI_UART_CLK_SOURCE;
/*��Ҫ��ϵͳ��������ʱ�Ӻ�APBʱ��֮����ã�HI_MDM_SetArmAndApbClock*/
HI_EXTERN HI_VOID HI_MDM_UART_SetFreq(HI_UART_CLK_SOURCE clk_source_type);

HI_SAL_USR_CFG_S * HI_CFG_GetUserCfg(HI_VOID);

/*���½ӿ����ú󣬵���low_power_switch_clk��Ч��begin*/
HI_U32 HI_CFG_SetEthClkEnableEarly(HI_BOOL bEnable);
HI_U32 HI_CFG_SetCrcClkEnableEarly(HI_BOOL bEnable);
HI_U32 HI_CFG_SetCipherClkEnableEarly(HI_BOOL bEnable);
HI_U32 HI_CFG_SetDtClkEnableEarly(HI_BOOL bEnable);
HI_U32 HI_CFG_SetDmacClkEnableEarly(HI_BOOL bEnable);
HI_U32 HI_CFG_SetSpiClkEnableEarly(HI_BOOL bEnable);
HI_U32 HI_CFG_SetEthEnableEarly(HI_BOOL bEnable);
/*���Ͻӿ����ú󣬵���low_power_switch_clk��Ч��end*/

HI_BOOL HI_CFG_GetEthEnable(HI_VOID);
HI_BOOL HI_CFG_InitEarly_End(HI_VOID);
HI_VOID HI_CFG_SetPhaseEnd(HI_VOID);

#endif//__HI_SAL_CFG_H__   
