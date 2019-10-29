/**@defgroup hct_io IO
 * @ingroup drivers
 */

#ifndef __HI_MDM_IO_H__
#define __HI_MDM_IO_H__

/**
 * @ingroup hct_io
 * GPIO编号
 */
typedef enum
{
    HI_GPIO_IDX_0,
    HI_GPIO_IDX_1,
    HI_GPIO_IDX_2,
    HI_GPIO_IDX_3,
    HI_GPIO_IDX_4,
    HI_GPIO_IDX_5,
    HI_GPIO_IDX_6,
    HI_GPIO_IDX_7,
    HI_GPIO_IDX_8,
    HI_GPIO_IDX_9,
    HI_GPIO_IDX_10,
    HI_GPIO_IDX_11,
    HI_GPIO_IDX_12,
    HI_GPIO_IDX_13,
    HI_GPIO_IDX_14,
    HI_GPIO_IDX_15,
    HI_GPIO_IDX_16,
    HI_GPIO_IDX_17,
    HI_GPIO_IDX_18,
    HI_GPIO_IDX_19,
    HI_GPIO_IDX_20,
    HI_GPIO_IDX_21,
    HI_GPIO_IDX_22,
    HI_GPIO_IDX_23,
    HI_GPIO_IDX_24,
    HI_GPIO_IDX_25,
    HI_GPIO_IDX_26,
    HI_GPIO_IDX_27,
    HI_GPIO_IDX_28,
    HI_GPIO_IDX_29,
    HI_GPIO_IDX_30,
    HI_GPIO_IDX_31,
    HI_GPIO_IDX_MAX,
}HI_GPIO_IDX_E;

/**
 * @ingroup hct_io
 *硬件管脚UP_RX_LED复用功能
 */
typedef enum
{
	HI_GPIO0_SEL_UP_RX_LED,
	HI_GPIO0_SEL_GPIO0,
	HI_GPIO0_SEL_ARM_MNT_CLK,
#ifdef PRODUCT_CFG_BOARD_TYPE_ASIC_V200
	HI_GPIO0_SEL_XTAL_MNT_CLK,
	HI_GPIO0_SEL_UART1_RXD,
	HI_GPIO0_SEL_SSP_DO,
#endif
}HI_HW_GPIO0_SEL_FUN_E;

/**
 * @ingroup hct_io
 *硬件管脚UP_TX_LED复用功能
 */
typedef enum
{
	HI_GPIO1_SEL_UP_TX_LED,
	HI_GPIO1_SEL_GPIO1,
#ifdef PRODUCT_CFG_BOARD_TYPE_ASIC_V200
	HI_GPIO1_SEL_AFE_MNT_CLK,
	HI_GPIO1_SEL_UART1_TXD = HI_GPIO1_SEL_UP_TX_LED + 4,
	HI_GPIO1_SEL_SSP_DI,
#endif
}HI_HW_GPIO1_SEL_FUN_E;

/**
 * @ingroup hct_io
 *硬件管脚DN_RX_LED复用功能
 */
typedef enum
{
	HI_GPIO2_SEL_DN_RX_LED,
	HI_GPIO2_SEL_GPIO2,
}HI_HW_GPIO2_SEL_FUN_E;//HI_HW_DN_RX_LED_IO2

/**
 * @ingroup hct_io
 *硬件管脚DN_TX_LED复用功能
 */
typedef enum
{
	HI_GPIO3_SEL_DN_TX_LED,
	HI_GPIO3_SEL_GPIO3,
}HI_HW_GPIO3_SEL_FUN_E;//HI_HW_DN_TX_LED_IO3	

/**
 * @ingroup hct_io
 *硬件管脚STATUS_LED复用功能
 */
typedef enum
{
	HI_GPIO4_SEL_STATUS_LED,
	HI_GPIO4_SEL_GPIO4,
	HI_GPIO4_SEL_JTAG_TRSTN,
#ifdef PRODUCT_CFG_BOARD_TYPE_ASIC_V200
	HI_GPIO4_SEL_GPIO6_BACKUP,
#endif
}HI_HW_GPIO4_SEL_FUN_E;//HI_HW_STATUS_LED_IO4

/**
 * @ingroup hct_io
 *硬件管脚LDRVR_PD复用功能
 */
typedef enum
{
	HI_GPIO5_SEL_LDRVR_PD,
	HI_GPIO5_SEL_GPIO5,
	HI_GPIO5_SEL_HW_ID_4,	
}HI_HW_GPIO5_SEL_FUN_E;

/**
 * @ingroup hct_io
 *硬件管脚CRSZ_DTC复用功能
 */
typedef enum
{
	HI_GPIO6_SEL_CRSZ_DTC,
#ifdef PRODUCT_CFG_BOARD_TYPE_ASIC_V200
	HI_GPIO6_SEL_AFE_AD_DATA_0 = HI_GPIO5_SEL_LDRVR_PD + 4,
	HI_GPIO6_SEL_SSP_CK,
#endif
}HI_HW_GPIO6_SEL_FUN_E;

/**
 * @ingroup hct_io
 *硬件管脚I2C_SCL复用功能
 */
typedef enum
{
	HI_GPIO7_SEL_I2C_SCL,
	HI_GPIO7_SEL_GPIO7,
	HI_GPIO7_SEL_JTAG_TCK,	
#ifdef PRODUCT_CFG_BOARD_TYPE_ASIC_V200
	HI_GPIO7_SEL_UART0_RXD,
	HI_GPIO7_SEL_AFE_GAIN_4,
#endif
}HI_HW_GPIO7_SEL_FUN_E;//HI_HW_I2C_SCL_IO7

/**
 * @ingroup hct_io
 *硬件管脚I2C_SDA复用功能
 */
typedef enum
{
	HI_GPIO8_SEL_I2C_SDA,
	HI_GPIO8_SEL_GPIO8,
	HI_GPIO8_SEL_JTAG_TMS,	
#ifdef PRODUCT_CFG_BOARD_TYPE_ASIC_V200
	HI_GPIO8_SEL_UART0_TXD,
	HI_GPIO8_SEL_AFE_GAIN_VLD,
	HI_GPIO8_SEL_UART0_N_TXD,
#endif
}HI_HW_GPIO8_SEL_FUN_E;//HI_HW_I2C_SDA_IO8

/**
 * @ingroup hct_io
 *硬件管脚UART0_RXD复用功能
 */
typedef enum
{
	HI_GPIO9_SEL_UART0_RXD,
	HI_GPIO9_SEL_GPIO9,	
}HI_HW_GPIO9_SEL_FUN_E;

/**
 * @ingroup hct_io
 *硬件管脚UART0_TXD复用功能
 */
typedef enum
{
	HI_GPIO10_SEL_UART0_TXD,
	HI_GPIO10_SEL_GPIO10,	
	HI_GPIO10_SEL_HW_ID_1,
#ifdef PRODUCT_CFG_BOARD_TYPE_ASIC_V200
	HI_GPIO10_SEL_UART0_N_TXD,
#endif
}HI_HW_GPIO10_SEL_FUN_E;

/**
 * @ingroup hct_io
 *硬件管脚UART2_RXD复用功能
 */
typedef enum
{
	HI_GPIO11_SEL_UART2_RXD,
	HI_GPIO11_SEL_GPIO11,
#ifdef PRODUCT_CFG_BOARD_TYPE_ASIC_V200
    HI_GPIO11_SEL_CURRENT_PHASE_IO2 = HI_GPIO11_SEL_UART2_RXD + 5,
#endif
}HI_HW_GPIO11_SEL_FUN_E;//HI_HW_UART2_RXD_IO11

/**
 * @ingroup hct_io
 *硬件管脚UART2_TXD复用功能
 */
typedef enum
{
	HI_GPIO12_SEL_UART2_TXD,
	HI_GPIO12_SEL_GPIO12,
#ifdef PRODUCT_CFG_BOARD_TYPE_ASIC_V200
	HI_GPIO12_SEL_CURRENT_PHASE_IO1 = HI_GPIO12_SEL_UART2_TXD + 5,
#endif
}HI_HW_GPIO12_SEL_FUN_E;//HI_HW_UART2_TXD_IO12

/**
 * @ingroup hct_io
 *硬件管脚UART1_RXD复用功能
 */
typedef enum
{
	HI_GPIO13_SEL_UART1_RXD,
	HI_GPIO13_SEL_GPIO13,
#ifdef PRODUCT_CFG_BOARD_TYPE_ASIC_V200
	HI_GPIO13_SEL_UP_RX_LED = HI_GPIO13_SEL_UART1_RXD + 3,
#endif
}HI_HW_GPIO13_SEL_FUN_E;//HI_HW_UART1_RXD_IO13

/**
 * @ingroup hct_io
 *硬件管脚UART1_TXD复用功能
 */
typedef enum
{
	HI_GPIO14_SEL_UART1_TXD,
	HI_GPIO14_SEL_GPIO14,	
	HI_GPIO14_SEL_JTAG_ENABLE,
#ifdef PRODUCT_CFG_BOARD_TYPE_ASIC_V200
	HI_GPIO14_SEL_UP_TX_LED,
#endif
}HI_HW_GPIO14_SEL_FUN_E;

/**
 * @ingroup hct_io
 *硬件管脚UART3_RXD复用功能
 */
typedef enum
{
	HI_GPIO15_SEL_UART3_RXD,//注:红外需要复用为UART
	HI_GPIO15_SEL_GPIO15,	
	HI_GPIO15_SEL_JTAG_TDI,
	HI_GPIO15_SEL_SIR_DI,
#ifdef PRODUCT_CFG_BOARD_TYPE_ASIC_V200
	HI_GPIO15_SEL_AFE_GAIN_1,
	HI_GPIO15_SEL_I2C_SDA = HI_GPIO15_SEL_UART3_RXD + 6,
#endif
}HI_HW_GPIO15_SEL_FUN_E;

/**
 * @ingroup hct_io
 *硬件管脚UART3_TXD复用功能
 */
typedef enum
{
	HI_GPIO16_SEL_UART3_TXD, //注:红外需要复用为UART
	HI_GPIO16_SEL_GPIO16,	
	HI_GPIO16_SEL_JTAG_TDO,
	HI_GPIO16_SEL_SIR_DO,	
#ifdef PRODUCT_CFG_BOARD_TYPE_ASIC_V200
	HI_GPIO16_SEL_AFE_GAIN_0,
	HI_GPIO16_SEL_I2C_SCL = HI_GPIO16_SEL_UART3_TXD + 6,
#endif
}HI_HW_GPIO16_SEL_FUN_E;

/**
 * @ingroup hct_io
 *硬件管脚SPI_CK复用功能
 */
typedef enum
{
	HI_GPIO17_SEL_SPI_CK,
	HI_GPIO17_SEL_GPIO17,	
	HI_GPIO17_SEL_SSP_CK,
}HI_HW_GPIO17_SEL_FUN_E;

/**
 * @ingroup hct_io
 *硬件管脚SPI_CSN复用功能
 */
typedef enum
{
	HI_GPIO18_SEL_SPI_CSN, 
	HI_GPIO18_SEL_GPIO18,	
	HI_GPIO18_SEL_SSP_CSN,
}HI_HW_GPIO18_SEL_FUN_E;

/**
 * @ingroup hct_io
 *硬件管脚SPI_DI复用功能
 */
typedef enum
{
	HI_GPIO19_SEL_SPI_DI,
	HI_GPIO19_SEL_GPIO19,	
	HI_GPIO19_SEL_SSP_DI,
}HI_HW_GPIO19_SEL_FUN_E;

/**
 * @ingroup hct_io
 *硬件管脚SPI_DO复用功能
 */
typedef enum
{
	HI_GPIO20_SEL_SPI_DO,
	HI_GPIO20_SEL_GPIO20,	
	HI_GPIO20_SEL_SSP_DO,
}HI_HW_GPIO20_SEL_FUN_E;//HI_HW_SPI_DO_IO20

/**
 * @ingroup hct_io
 *硬件管脚SWITCH_A复用功能
 */
typedef enum
{
	HI_GPIO21_SEL_SWITCH_A, 
	HI_GPIO21_SEL_GPIO21,	
	HI_GPIO21_SEL_EXT_AFE_AGC_0,
}HI_HW_GPIO21_SEL_FUN_E;

/**
 * @ingroup hct_io
 *硬件管脚SWITCH_B复用功能
 */
typedef enum
{
	HI_GPIO22_SEL_SWITCH_B,
	HI_GPIO22_SEL_GPIO22,	
	HI_GPIO22_SEL_EXT_AFE_AGC_1,
}HI_HW_GPIO22_SEL_FUN_E;

/**
 * @ingroup hct_io
 *硬件管脚SWITCH_C复用功能
 */
typedef enum
{
	HI_GPIO23_SEL_SWITCH_C,
	HI_GPIO23_SEL_GPIO23,	
	HI_GPIO23_SEL_EXT_AFE_AGC_2,
}HI_HW_GPIO23_SEL_FUN_E;

/**
 * @ingroup hct_io
 *硬件管脚SFC_WPN复用功能
 */
typedef enum
{
	HI_GPIO24_SEL_SFC_WPN,
	HI_GPIO24_SEL_GPIO24,	
	HI_GPIO24_SEL_HW_ID_11,
}HI_HW_GPIO24_SEL_FUN_E;

/**
 * @ingroup hct_io
 *硬件管脚SFC_HOLDN复用功能
 */
typedef enum
{
	HI_GPIO25_SEL_SFC_HOLDN,
	HI_GPIO25_SEL_GPIO25,	
	HI_GPIO25_SEL_HW_ID_2,
}HI_HW_GPIO25_SEL_FUN_E;

/**
 * @ingroup hct_io
 *硬件管脚ETXD0复用功能
 */
typedef enum
{
	HI_GPIO26_SEL_ETXD0,
	HI_GPIO26_SEL_GPIO26,	
	HI_GPIO26_SEL_EXT_AFE_ADIO_0,
#ifdef PRODUCT_CFG_BOARD_TYPE_ASIC_V200
	HI_GPIO26_SEL_HW_ID_5,
	HI_GPIO26_SEL_AFE_DA_DATA_4,
	HI_GPIO26_SEL_SSP2_DO,
#endif
}HI_HW_GPIO26_SEL_FUN_E;

/**
 * @ingroup hct_io
 *硬件管脚ETXD1复用功能
 */
typedef enum
{
	HI_GPIO27_SEL_ETXD1,
	HI_GPIO27_SEL_GPIO27,	
	HI_GPIO27_SEL_EXT_AFE_RWCTRL,
#ifdef PRODUCT_CFG_BOARD_TYPE_ASIC_V200
	HI_GPIO27_SEL_HW_ID_6,
	HI_GPIO27_SEL_AFE_DA_DATA_5,
	HI_GPIO27_SEL_SSP2_DI,
#endif
}HI_HW_GPIO27_SEL_FUN_E;

/**
 * @ingroup hct_io
 *硬件管脚ETXD2复用功能
 */
typedef enum
{
	HI_GPIO28_SEL_ETXD2,
	HI_GPIO28_SEL_GPIO28,	
	HI_GPIO28_SEL_EXT_AFE_CLK,
#ifdef PRODUCT_CFG_BOARD_TYPE_ASIC_V200
	HI_GPIO28_SEL_HW_ID_7,
	HI_GPIO28_SEL_AFE_DA_DATA_6,
	HI_GPIO28_SEL_SSP2_CSN,
#endif
}HI_HW_GPIO28_SEL_FUN_E;

/**
 * @ingroup hct_io
 *硬件管脚ETXD3复用功能
 */
typedef enum
{
	HI_GPIO29_SEL_ETXD3,
	HI_GPIO29_SEL_GPIO29,	
	HI_GPIO29_SEL_EXT_AFE_AGC_5,
#ifdef PRODUCT_CFG_BOARD_TYPE_ASIC_V200
	HI_GPIO29_SEL_RSV,
	HI_GPIO29_SEL_AFE_DA_DATA_7,
	HI_GPIO29_SEL_SSP2_CK,
#endif
}HI_HW_GPIO29_SEL_FUN_E;

/**
 * @ingroup hct_io
 *硬件管脚ETXEN复用功能
 */
typedef enum
{
	HI_GPIO30_SEL_ETXEN,
	HI_GPIO30_SEL_GPIO30,	
	HI_GPIO30_SEL_EXT_AFE_ADIO_1,
}HI_HW_GPIO30_SEL_FUN_E;

/**
 * @ingroup hct_io
 *硬件管脚PERI_RSTN复用功能
 */
typedef enum
{
	HI_GPIO31_SEL_PERI_RSTN,
	HI_GPIO31_SEL_GPIO31,	
}HI_HW_GPIO31_SEL_FUN_E;

/**
 * @ingroup hct_io
 *硬件管脚MDCK复用功能
 */
typedef enum
{
    HI_GPIO9_BACKUP_SEL_MDCK,
    HI_GPIO9_BACKUP_SEL_GPIO9,
    HI_GPIO9_BACKUP_SEL_EXT_AFE_ADIO_10,
    HI_GPIO9_BACKUP_SEL_AFE_AD_DATA_CLK = HI_GPIO9_BACKUP_SEL_MDCK + 4,
    HI_GPIO9_BACKUP_SEL_I2C_SCL2,
}HI_HW_GPIO9_BACKUP_SEL_FUN_E;

/**
 * @ingroup hct_io
 *硬件管脚MDIO复用功能
 */
typedef enum
{
    HI_GPIO10_BACKUP_SEL_MDIO,
    HI_GPIO10_BACKUP_SEL_GPIO10,
    HI_GPIO10_BACKUP_SEL_EXT_AFE_ADIO_11,
    HI_GPIO10_BACKUP_SEL_AFE_AD_DATA_10 = HI_GPIO10_BACKUP_SEL_MDIO + 4,
    HI_GPIO10_BACKUP_SEL_I2C_SDA2,
}HI_HW_GPIO10_BACKUP_SEL_FUN_E;

/**
 * @ingroup hct_io
 *硬件管脚ETXCK复用功能
 */
typedef enum
{    
    HI_GPIO11_BACKUP_SEL_ETXCK,
    HI_GPIO11_BACKUP_SEL_GPIO11,
    HI_GPIO11_BACKUP_SEL_EXT_AFE_ADIO_2,
    HI_GPIO11_BACKUP_SEL_AFE_AD_DATA_11 = HI_GPIO11_BACKUP_SEL_ETXCK + 4,
}HI_HW_GPIO11_BACKUP_SEL_FUN_E;

/**
 * @ingroup hct_io
 *硬件管脚ERXCK复用功能
 */
typedef enum
{   
    HI_GPIO12_BACKUP_SEL_ERXCK,
    HI_GPIO12_BACKUP_SEL_GPIO12,
    HI_GPIO12_BACKUP_SEL_EXT_AFE_ADIO4,
    HI_GPIO12_BACKUP_SEL_AFE_AD_DATA_VLD = HI_GPIO12_BACKUP_SEL_ERXCK + 4,
}HI_HW_GPIO12_BACKUP_SEL_FUN_E;

/**
 * @ingroup hct_io
 *硬件管脚ERXDV复用功能
 */
typedef enum
{
    HI_GPIO13_BACKUP_SEL_ERXDV,
    HI_GPIO13_BACKUP_SEL_GPIO13,
    HI_GPIO13_BACKUP_SEL_EXT_AFE_ADIO_5,
    HI_GPIO13_BACKUP_SEL_AFE_DA_DATA_0 = HI_GPIO13_BACKUP_SEL_ERXDV + 4,
}HI_HW_GPIO13_BACKUP_SEL_FUN_E;

/**
 * @ingroup hct_io
 *硬件管脚ERXER复用功能
 */
typedef enum
{
    HI_GPIO14_BACKUP_SEL_ERXER,
    HI_GPIO14_BACKUP_SEL_GPIO14,
    HI_GPIO14_BACKUP_SEL_EXT_AFE_ADIO_3,
    HI_GPIO14_BACKUP_SEL_AFE_DA_DATA_1 = HI_GPIO14_BACKUP_SEL_ERXER + 4,
}HI_HW_GPIO14_BACKUP_SEL_FUN_E;

/**
 * @ingroup hct_io
 *硬件管脚ECOL复用功能
 */
typedef enum
{    
    HI_GPIO15_BACKUP_SEL_ECOL,
    HI_GPIO15_BACKUP_SEL_GPIO15,
    HI_GPIO15_BACKUP_SEL_EXT_AFE_AGC_4,
    HI_GPIO15_BACKUP_SEL_AFE_DA_DATA_2 = HI_GPIO15_BACKUP_SEL_ECOL + 4,
}HI_HW_GPIO15_BACKUP_SEL_FUN_E;

/**
 * @ingroup hct_io
 *硬件管脚ECRS复用功能
 */
typedef enum
{
    HI_GPIO16_BACKUP_SEL_ECRS = 0,
    HI_GPIO16_BACKUP_SEL_GPIO16,
    HI_GPIO16_BACKUP_SEL_EXT_AFE_AGC_3,
    HI_GPIO16_BACKUP_SEL_AFE_DA_DATA_3 = HI_GPIO16_BACKUP_SEL_ECRS + 4,
}HI_HW_GPIO16_BACKUP_SEL_FUN_E;

/**
 * @ingroup hct_io
 *硬件管脚ERXD0复用功能
 */
typedef enum
{
    HI_GPIO17_BACKUP_SEL_ERXD0,
    HI_GPIO17_BACKUP_SEL_GPIO17,
    HI_GPIO17_BACKUP_SEL_EXT_AFE_ADIO_6,
    HI_GPIO17_BACKUP_SEL_AFE_DA_DATA_8 = HI_GPIO17_BACKUP_SEL_ERXD0 + 4,
}HI_HW_GPIO17_BACKUP_SEL_FUN_E;

/**
 * @ingroup hct_io
 *硬件管脚ERXD1复用功能
 */
typedef enum
{
    HI_GPIO18_BACKUP_SEL_ERXD1,
    HI_GPIO18_BACKUP_SEL_GPIO18,
    HI_GPIO18_BACKUP_SEL_EXT_AFE_ADIO_7,
    HI_GPIO18_BACKUP_SEL_AFE_DA_DATA_9 = HI_GPIO18_BACKUP_SEL_ERXD1 + 4,
}HI_HW_GPIO18_BACKUP_SEL_FUN_E;


/**
 * @ingroup hct_io
 *硬件管脚ERXD2复用功能
 */
typedef enum
{
    HI_GPIO19_BACKUP_SEL_ERXD2,
    HI_GPIO19_BACKUP_SEL_GPIO19,
    HI_GPIO19_BACKUP_SEL_EXT_AFE_ADIO_8,
    HI_GPIO19_BACKUP_SEL_AFE_DA_DATA_10 = HI_GPIO19_BACKUP_SEL_ERXD2 + 4,
}HI_HW_GPIO19_BACKUP_SEL_FUN_E;

/**
 * @ingroup hct_io
 *硬件管脚ERXD3复用功能
 */
typedef enum
{
    HI_GPIO20_BACKUP_SEL_ERXD3,
    HI_GPIO20_BACKUP_SEL_GPIO20,
    HI_GPIO20_BACKUP_SEL_EXT_AFE_ADIO_9,
    HI_GPIO20_BACKUP_SEL_AFE_DA_DATA_11 = HI_GPIO20_BACKUP_SEL_ERXD3 + 4,
}HI_HW_GPIO20_BACKUP_SEL_FUN_E;

/**
 * @ingroup hct_io
 *硬件管脚GPIO26复用功能
 */
typedef enum
{   
    HI_GPIO26_BACKUP_SEL_GPIO26,
    HI_GPIO26_BACKUP_SEL_I2C_SCL,
    HI_GPIO26_BACKUP_SEL_PWM_OUT,
    HI_GPIO26_BACKUP_SEL_RNG_CLK_SAMPLE,
    HI_GPIO26_BACKUP_SEL_AFE_BW_CTRL0,
    HI_GPIO26_BACKUP_SEL_CURRENT_PHASE_I00,
}HI_HW_GPIO26_BACKUP_SEL_FUN_E;//HI_HW_GPIO26_BACKUP

/**
 * @ingroup hct_io
 *硬件管脚GPIO27复用功能
 */
typedef enum
{
    HI_GPIO27_BACKUP_SEL_FLASH_RST_N,
    HI_GPIO27_BACKUP_SEL_I2C_SDA,
    HI_GPIO27_BACKUP_SEL_GPIO27,
    HI_GPIO27_BACKUP_SEL_RNG_SAMPLE_DATA,
    HI_GPIO27_BACKUP_SEL_AFE_BW_CTRL1,
}HI_HW_GPIO27_BACKUP_SEL_FUN_E;//HI_HW_GPIO27_BACKUP

/**
 * @ingroup hct_io
 *硬件管脚GPIO28复用功能
 */
typedef enum
{    
    HI_GPIO28_BACKUP_SEL_GPIO28,
    HI_GPIO28_BACKUP_SEL_SSP_CSN = HI_GPIO28_BACKUP_SEL_GPIO28 + 5,
}HI_HW_GPIO28_BACKUP_SEL_FUN_E;//HI_HW_GPIO28_BACKUP

/**
 * @ingroup hct_io
 *硬件管脚GPIO29复用功能
 */
typedef enum
{
    HI_GPIO29_BACKUP_SEL_EFUSE_PW_E, 
    HI_GPIO29_BACKUP_SEL_GPIO29, 
}HI_HW_GPIO29_BACKUP_SEL_FUN_E;//HI_HW_GPIO29_BACKUP

/**
 * @ingroup hct_io
 * IO硬件管脚索引
 */
typedef enum
{
	HI_HW_UP_RX_LED_IO0,
	HI_HW_UP_TX_LED_IO1,
	HI_HW_DN_RX_LED_IO2,
	HI_HW_DN_TX_LED_IO3,	
	HI_HW_STATUS_LED_IO4,
	HI_HW_RVR_PD_IO5,
	HI_HW_CRSZ_DTC_IO6,
	HI_HW_I2C_SCL_IO7,
	HI_HW_I2C_SDA_IO8,
	HI_HW_UART0_RXD_IO9,
	HI_HW_UART0_TXD_IO10,
	HI_HW_UART2_RXD_IO11,
	HI_HW_UART2_TXD_IO12,
	HI_HW_UART1_RXD_IO13,
	HI_HW_UART1_TXD_IO14,
	HI_HW_UART3_RXD_IO15,
	HI_HW_UART3_TXD_IO16,
	HI_HW_SPI_CK_IO17,
	HI_HW_SPI_CSN_IO18, 
	HI_HW_SPI_DI_IO19,
	HI_HW_SPI_DO_IO20,
	HI_HW_SWITCH_A_IO21, 
	HI_HW_SWITCH_B_IO22,
	HI_HW_SWITCH_C_IO23,
	HI_HW_SFC_WPN_IO24,
	HI_HW_SFC_HOLDN_IO25,
	HI_HW_ETXD0_IO26,// 空1个地址
	HI_HW_ETXD1_IO27,
	HI_HW_ETXD2_IO28,
	HI_HW_ETXD3_IO29,
	HI_HW_ETXEN_IO30,
	HI_HW_PERI_RSTN_IO31,
//#ifdef PRODUCT_CFG_BOARD_TYPE_ASIC_V200
    HI_HW_MDCK_IO9_BACKUP,
    HI_HW_MDIO_IO10_BACKUP,
    HI_HW_ETXCK_IO11_BACKUP,
    HI_HW_ERXCK_IO12_BACKUP,
    HI_HW_ERXDV_IO13_BACKUP,
    HI_HW_ERXER_IO14_BACKUP,    
    HI_HW_ECOL_IO15_BACKUP,
    HI_HW_ECRS_IO16_BACKUP,// 空25个地址
    HI_HW_ERXD0_IO17_BACKUP,
    HI_HW_ERXD1_IO18_BACKUP,
    HI_HW_ERXD2_IO19_BACKUP,
    HI_HW_ERXD3_IO20_BACKUP,    
    HI_HW_GPIO26_BACKUP,  
    HI_HW_GPIO27_BACKUP, 
    HI_HW_GPIO28_BACKUP,
    HI_HW_GPIO29_BACKUP,  
//#endif
	HI_HW_IO_IDX_MAX,
}HI_HW_IO_IDX_E;

/**
 * @ingroup hct_io
 * IO电平状态
 */
typedef enum
{
	HI_GPIO_VALUE0 = 0, /**< 低电平 */
	HI_GPIO_VALUE1      /**< 高电平 */
}HI_GPIO_VALUE_E;

/**
 * @ingroup hct_io
 * IO方向
 */
typedef enum
{
	HI_GPIO_DIRECTION_IN = 0,   /**< 输入 */
	HI_GPIO_DIRECTION_OUT       /**< 输出 */
}HI_GPIO_DIRECTION_E;

/**
 * @ingroup hct_io
 * IO中断触发方式
 */
typedef enum
{
	HI_INT_LEVEL = 0,   /**< 电平触发 */
	HI_INT_EDGE         /**< 边沿触发 */
}HI_GPIO_INT_E;

/**
 * @ingroup hct_io
 * IO中断极性
 */
typedef enum
{
    HI_EDGE_FALL_LEVEL_LOW = 0, /**< 下降沿 */
    HI_EDGE_RISE_LEVEL_HIGH     /**< 上升沿沿 */
}HI_GPIO_INT_POLARITY_E;

/**
 * @ingroup hct_io
 *
 * IO中断处理函数执行的位置
 */
typedef enum
{
    HI_LATTER_OF_INTERRUPTION = 0,  /**< 中断后半段执行 */
    HI_HALF_OF_INTERRUPTION         /**< 中断前半段执行 */
}HI_GPIO_INT_EXECUTE_POSITION_E;

/**
* @ingroup  hct_io
* @brief  设置单个IO管脚功能。
*
* @par 描述:
* 设置单个IO管脚功能。
* @attention 
* @li 该接口支持在锁任务上下文中使用。
* @li 该接口支持在LISR上下文中使用。
* @li 该接口支持在关中断上下文中使用。
*
* @param  enId [IN] 类型 #HI_HW_IO_IDX_E  IO管脚索引。
* @param  ulVal [IN] 类型 #HI_U8  各管脚功能请使用以下枚举中参数:
* @li HI_HW_GPIO0_SEL_FUN_E;
* @li HI_HW_GPIO1_SEL_FUN_E;
* @li HI_HW_GPIO2_SEL_FUN_E;
* @li HI_HW_GPIO3_SEL_FUN_E;
* @li HI_HW_GPIO4_SEL_FUN_E;
* @li HI_HW_GPIO5_SEL_FUN_E;
* @li HI_HW_GPIO6_SEL_FUN_E;
* @li HI_HW_GPIO7_SEL_FUN_E;
* @li HI_HW_GPIO8_SEL_FUN_E;
* @li HI_HW_GPIO9_SEL_FUN_E;
* @li HI_HW_GPIO10_SEL_FUN_E;
* @li HI_HW_GPIO11_SEL_FUN_E;
* @li HI_HW_GPIO12_SEL_FUN_E;
* @li HI_HW_GPIO13_SEL_FUN_E;
* @li HI_HW_GPIO14_SEL_FUN_E;
* @li HI_HW_GPIO15_SEL_FUN_E;
* @li HI_HW_GPIO16_SEL_FUN_E;
* @li HI_HW_GPIO17_SEL_FUN_E;
* @li HI_HW_GPIO18_SEL_FUN_E;
* @li HI_HW_GPIO19_SEL_FUN_E;
* @li HI_HW_GPIO20_SEL_FUN_E;
* @li HI_HW_GPIO21_SEL_FUN_E;
* @li HI_HW_GPIO22_SEL_FUN_E;
* @li HI_HW_GPIO23_SEL_FUN_E;
* @li HI_HW_GPIO24_SEL_FUN_E;
* @li HI_HW_GPIO25_SEL_FUN_E;
* @li HI_HW_GPIO26_SEL_FUN_E;
* @li HI_HW_GPIO27_SEL_FUN_E;
* @li HI_HW_GPIO28_SEL_FUN_E;
* @li HI_HW_GPIO29_SEL_FUN_E;
* @li HI_HW_GPIO30_SEL_FUN_E;
* @li HI_HW_GPIO31_SEL_FUN_E;
* @li HI_HW_GPIO9_BACKUP_SEL_FUN_E;
* @li HI_HW_GPIO10_BACKUP_SEL_FUN_E;
* @li HI_HW_GPIO11_BACKUP_SEL_FUN_E;
* @li HI_HW_GPIO12_BACKUP_SEL_FUN_E;
* @li HI_HW_GPIO13_BACKUP_SEL_FUN_E;
* @li HI_HW_GPIO14_BACKUP_SEL_FUN_E;
* @li HI_HW_GPIO15_BACKUP_SEL_FUN_E;
* @li HI_HW_GPIO16_BACKUP_SEL_FUN_E;
* @li HI_HW_GPIO17_BACKUP_SEL_FUN_E;
* @li HI_HW_GPIO18_BACKUP_SEL_FUN_E;
* @li HI_HW_GPIO19_BACKUP_SEL_FUN_E;
* @li HI_HW_GPIO20_BACKUP_SEL_FUN_E;
* @li HI_HW_GPIO26_BACKUP_SEL_FUN_E;
* @li HI_HW_GPIO27_BACKUP_SEL_FUN_E;
* @li HI_HW_GPIO28_BACKUP_SEL_FUN_E;
* @li HI_HW_GPIO29_BACKUP_SEL_FUN_E;
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_io.h: 该接口声明所在的头文件。
* @see  HI_MDM_IO_GetMux
* @since HiMDL_V200R001C00
*/
#define HI_MDM_IO_SetMux(enId, ulVal) HI_IO_SetMux(enId, ulVal)

/**
* @ingroup  hct_io
* @brief  获取单个IO管脚功能。
*
* @par 描述:  
* 获取单个IO管脚功能。
* @attention 
* @li 该接口支持在锁任务上下文中使用。
* @li 该接口支持在LISR上下文中使用。
* @li 该接口支持在关中断上下文中使用。
*
* @param  enId [IN] 类型 #HI_HW_IO_IDX_E  IO管脚索引。
* @param  pVal [OUT] 类型 #HI_U8 * 功能。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_io.h: 该接口声明所在的头文件。
* @see  HI_MDM_IO_SetMux
* @since HiMDL_V200R001C00
*/
#define HI_MDM_IO_GetMux(enId, pVal) HI_IO_GetMux(enId, pVal)

/**
* @ingroup  hct_io
* @brief  设置单个IO管脚方向。
*
* @par 描述:  
* 设置单个IO管脚方向。
* @attention 
* @li 该接口支持在锁任务上下文中使用。
* @li 该接口支持在LISR上下文中使用。
* @li 该接口支持在关中断上下文中使用。
*
* @param  enId [IN] 类型 #HI_GPIO_IDX_E  IO 索引。
* @param  enDir [IN] 类型 #HI_GPIO_DIRECTION_E  IO方向。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_io.h: 该接口声明所在的头文件。
* @see  HI_MDM_IO_GetDir
* @since HiMDL_V200R001C00
*/
#define HI_MDM_IO_SetDir(enId, enDir) HI_IO_SetDir(enId, enDir)

/**
* @ingroup  hct_io
* @brief  获取单个IO管脚方向。
*
* @par 描述:
* 获取单个IO管脚方向。
* @attention 
* @li 该接口支持在锁任务上下文中使用。
* @li 该接口支持在LISR上下文中使用。
* @li 该接口支持在关中断上下文中使用。
*
* @param  enId [IN] 类型 #HI_GPIO_IDX_E   IO 索引。
* @param  pDir [OUT] 类型 #HI_GPIO_DIRECTION_E * IO方向。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_io.h: 该接口声明所在的头文件。
* @see  HI_MDM_IO_SetDir
* @since HiMDL_V200R001C00
*/
#define HI_MDM_IO_GetDir(enId, pDir) HI_IO_GetDir(enId, pDir)

/**
* @ingroup  hct_io
* @brief  设置单个IO管脚输出电平状态。
*
* @par 描述:
* 设置单个IO管脚输出电平状态。
* @attention 
* @li 该接口支持在锁任务上下文中使用。
* @li 该接口支持在LISR上下文中使用。
* @li 该接口支持在关中断上下文中使用。
*
* @param  enId [IN] 类型 #HI_GPIO_IDX_E  IO 索引。
* @param  enVal [IN] 类型 #HI_GPIO_VALUE_E   输出值，1为高电平，0为低电平。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_io.h: 该接口声明所在的头文件。
* @see HI_MDM_IO_GetInputVal | HI_MDM_IO_GetOutputVal
* @since HiMDL_V200R001C00
*/
#define HI_MDM_IO_SetOutputVal(enId, enVal) HI_IO_SetOutputVal(enId, enVal)

/**
* @ingroup  hct_io
* @brief  获取单个IO管脚输入电平状态。
*
* @par 描述:
* 获取单个IO管脚输入电平状态。
* @attention 
* @li 该接口支持在锁任务上下文中使用。
* @li 该接口支持在LISR上下文中使用。
* @li 该接口支持在关中断上下文中使用。
*
* @param  enId [IN] 类型 #HI_GPIO_IDX_E  IO 索引。
* @param  pVal [OUT] 类型 #HI_GPIO_VALUE_E* 电平状态，1为高电平，0为低电平。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_io.h: 该接口声明所在的头文件。
* @see HI_MDM_IO_SetOutputVal | HI_MDM_IO_GetOutputVal
* @since HiMDL_V200R001C00
*/
#define HI_MDM_IO_GetInputVal(enId, pVal) HI_IO_GetInputVal(enId, pVal)

/**
* @ingroup  hct_io
* @brief  获取单个IO管脚输出电平状态。
*
* @par 描述:
* 获取单个IO管脚输出电平状态。
* @attention 
* @li 该接口支持在锁任务上下文中使用。
* @li 该接口支持在LISR上下文中使用。
* @li 该接口支持在关中断上下文中使用。
*
* @param  enId [IN] 类型 #HI_GPIO_IDX_E  IO 索引。
* @param  pVal [OUT] 类型 #HI_GPIO_VALUE_E *电平状态，1为高电平，0为低电平。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_io.h: 该接口声明所在的头文件。
* @see HI_MDM_IO_SetOutputVal | HI_MDM_IO_GetInputVal
* @since HiMDL_V200R001C00
*/
#define HI_MDM_IO_GetOutputVal(enId, pVal) HI_IO_GetOutputVal(enId, pVal)

/**
* @ingroup  hct_io
* @brief  使能某个GPIO的中断功能。
*
* @par 描述:
* 使能某个GPIO的中断功能。
* @attention 
* @li 该接口支持在锁任务上下文中使用。
* @li 该接口支持在LISR上下文中使用。
* @li 该接口支持在关中断上下文中使用。
*
* @param  enId [IN] 类型 #HI_GPIO_IDX_E  IO 索引。
* @param  enIntType [IN] 类型 #HI_GPIO_INT_E 中断类型，0为电平触发中断，1为沿触发中断。
* @param  enIntPolarity [IN] 类型 #HI_GPIO_INT_POLARITY_E中断极性，0为下降沿或低电平触发，1为上升沿或高电平触发。
* @param  gpio_callback [IN] 类型 #HI_S32 (*gpio_callback)()  中断回调函数。
* @param  ulCallbackArgv [IN] 类型 #HI_U32 中断回调函数参数。
* @param  ulFlags [IN] 类型 #HI_GPIO_INT_EXECUTE_POSITION_E 配置是中断前半段调用，还是后半段调用。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_io.h: 该接口声明所在的头文件。
* @see  HI_MDM_IO_IntDisconnect
* @since HiMDL_V200R001C00
*/
#define HI_MDM_IO_IntConnect(enId, enIntType, enIntPolarity, gpio_callback, ulCallbackArgv, ulFlags) HI_IO_IntConnect(enId, enIntType, enIntPolarity, gpio_callback, ulCallbackArgv, ulFlags)

/**
* @ingroup  hct_io
* @brief  去使能某个GPIO的中断功能。
*
* @par 描述:
* 去使能某个GPIO的中断功能。
* @attention 
* @li 该接口支持在锁任务上下文中使用。
* @li 该接口支持在LISR上下文中使用。
* @li 该接口支持在关中断上下文中使用。
*
* @param  enId [IN] 类型 #HI_GPIO_IDX_E  IO 索引。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_io.h: 该接口声明所在的头文件。
* @see  HI_MDM_IO_IntConnect
* @since HiMDL_V200R001C00
*/
#define HI_MDM_IO_IntDisconnect(enId) HI_IO_IntDisconnect(enId)

/**
* @ingroup  hct_io
* @brief  IO中断初始化。
*
* @par 描述:
* IO中断初始化。
* @attention 无。
*
* @param 无。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_io.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
#define HI_MDM_IO_IntInit() HI_IO_IntInit();



HI_EXTERN HI_U32 HI_IO_SetMux(HI_HW_IO_IDX_E enIoId, HI_U8 ulIoVal);
HI_EXTERN HI_U32 HI_IO_GetMux(HI_HW_IO_IDX_E enIoId, HI_U8 *pIoVal);
HI_EXTERN HI_U32 HI_IO_SetDir(HI_GPIO_IDX_E enId, HI_GPIO_DIRECTION_E enDir);
HI_EXTERN HI_U32 HI_IO_GetDir(HI_GPIO_IDX_E enId, HI_GPIO_DIRECTION_E *pDir);
HI_EXTERN HI_U32 HI_IO_SetOutputVal(HI_GPIO_IDX_E enId, HI_GPIO_VALUE_E enVal);
HI_EXTERN HI_U32 HI_IO_GetInputVal(HI_GPIO_IDX_E enId, HI_GPIO_VALUE_E *pVal);
HI_EXTERN HI_U32 HI_IO_GetOutputVal(HI_GPIO_IDX_E enId, HI_GPIO_VALUE_E *pVal);
HI_EXTERN HI_U32 HI_IO_IntConnect(HI_GPIO_IDX_E enId, HI_GPIO_INT_E enIntType, HI_GPIO_INT_POLARITY_E enIntPolarity,
                                 HI_S32 (*gpio_callback)(HI_U32 ulCallbackArgv), HI_U32  ulCallbackArgv, HI_U32 ulFlags);
HI_EXTERN HI_U32 HI_IO_IntDisconnect(HI_GPIO_IDX_E enId);
HI_EXTERN HI_U32 HI_IO_IntInit(HI_VOID);

#endif
