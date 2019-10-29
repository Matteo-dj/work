/**@defgroup hct_io IO
 * @ingroup drivers
 */

#ifndef __HI_MDM_IO_H__
#define __HI_MDM_IO_H__

/**
 * @ingroup hct_io
 * GPIO���
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
 *Ӳ���ܽ�UP_RX_LED���ù���
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
 *Ӳ���ܽ�UP_TX_LED���ù���
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
 *Ӳ���ܽ�DN_RX_LED���ù���
 */
typedef enum
{
	HI_GPIO2_SEL_DN_RX_LED,
	HI_GPIO2_SEL_GPIO2,
}HI_HW_GPIO2_SEL_FUN_E;//HI_HW_DN_RX_LED_IO2

/**
 * @ingroup hct_io
 *Ӳ���ܽ�DN_TX_LED���ù���
 */
typedef enum
{
	HI_GPIO3_SEL_DN_TX_LED,
	HI_GPIO3_SEL_GPIO3,
}HI_HW_GPIO3_SEL_FUN_E;//HI_HW_DN_TX_LED_IO3	

/**
 * @ingroup hct_io
 *Ӳ���ܽ�STATUS_LED���ù���
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
 *Ӳ���ܽ�LDRVR_PD���ù���
 */
typedef enum
{
	HI_GPIO5_SEL_LDRVR_PD,
	HI_GPIO5_SEL_GPIO5,
	HI_GPIO5_SEL_HW_ID_4,	
}HI_HW_GPIO5_SEL_FUN_E;

/**
 * @ingroup hct_io
 *Ӳ���ܽ�CRSZ_DTC���ù���
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
 *Ӳ���ܽ�I2C_SCL���ù���
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
 *Ӳ���ܽ�I2C_SDA���ù���
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
 *Ӳ���ܽ�UART0_RXD���ù���
 */
typedef enum
{
	HI_GPIO9_SEL_UART0_RXD,
	HI_GPIO9_SEL_GPIO9,	
}HI_HW_GPIO9_SEL_FUN_E;

/**
 * @ingroup hct_io
 *Ӳ���ܽ�UART0_TXD���ù���
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
 *Ӳ���ܽ�UART2_RXD���ù���
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
 *Ӳ���ܽ�UART2_TXD���ù���
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
 *Ӳ���ܽ�UART1_RXD���ù���
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
 *Ӳ���ܽ�UART1_TXD���ù���
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
 *Ӳ���ܽ�UART3_RXD���ù���
 */
typedef enum
{
	HI_GPIO15_SEL_UART3_RXD,//ע:������Ҫ����ΪUART
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
 *Ӳ���ܽ�UART3_TXD���ù���
 */
typedef enum
{
	HI_GPIO16_SEL_UART3_TXD, //ע:������Ҫ����ΪUART
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
 *Ӳ���ܽ�SPI_CK���ù���
 */
typedef enum
{
	HI_GPIO17_SEL_SPI_CK,
	HI_GPIO17_SEL_GPIO17,	
	HI_GPIO17_SEL_SSP_CK,
}HI_HW_GPIO17_SEL_FUN_E;

/**
 * @ingroup hct_io
 *Ӳ���ܽ�SPI_CSN���ù���
 */
typedef enum
{
	HI_GPIO18_SEL_SPI_CSN, 
	HI_GPIO18_SEL_GPIO18,	
	HI_GPIO18_SEL_SSP_CSN,
}HI_HW_GPIO18_SEL_FUN_E;

/**
 * @ingroup hct_io
 *Ӳ���ܽ�SPI_DI���ù���
 */
typedef enum
{
	HI_GPIO19_SEL_SPI_DI,
	HI_GPIO19_SEL_GPIO19,	
	HI_GPIO19_SEL_SSP_DI,
}HI_HW_GPIO19_SEL_FUN_E;

/**
 * @ingroup hct_io
 *Ӳ���ܽ�SPI_DO���ù���
 */
typedef enum
{
	HI_GPIO20_SEL_SPI_DO,
	HI_GPIO20_SEL_GPIO20,	
	HI_GPIO20_SEL_SSP_DO,
}HI_HW_GPIO20_SEL_FUN_E;//HI_HW_SPI_DO_IO20

/**
 * @ingroup hct_io
 *Ӳ���ܽ�SWITCH_A���ù���
 */
typedef enum
{
	HI_GPIO21_SEL_SWITCH_A, 
	HI_GPIO21_SEL_GPIO21,	
	HI_GPIO21_SEL_EXT_AFE_AGC_0,
}HI_HW_GPIO21_SEL_FUN_E;

/**
 * @ingroup hct_io
 *Ӳ���ܽ�SWITCH_B���ù���
 */
typedef enum
{
	HI_GPIO22_SEL_SWITCH_B,
	HI_GPIO22_SEL_GPIO22,	
	HI_GPIO22_SEL_EXT_AFE_AGC_1,
}HI_HW_GPIO22_SEL_FUN_E;

/**
 * @ingroup hct_io
 *Ӳ���ܽ�SWITCH_C���ù���
 */
typedef enum
{
	HI_GPIO23_SEL_SWITCH_C,
	HI_GPIO23_SEL_GPIO23,	
	HI_GPIO23_SEL_EXT_AFE_AGC_2,
}HI_HW_GPIO23_SEL_FUN_E;

/**
 * @ingroup hct_io
 *Ӳ���ܽ�SFC_WPN���ù���
 */
typedef enum
{
	HI_GPIO24_SEL_SFC_WPN,
	HI_GPIO24_SEL_GPIO24,	
	HI_GPIO24_SEL_HW_ID_11,
}HI_HW_GPIO24_SEL_FUN_E;

/**
 * @ingroup hct_io
 *Ӳ���ܽ�SFC_HOLDN���ù���
 */
typedef enum
{
	HI_GPIO25_SEL_SFC_HOLDN,
	HI_GPIO25_SEL_GPIO25,	
	HI_GPIO25_SEL_HW_ID_2,
}HI_HW_GPIO25_SEL_FUN_E;

/**
 * @ingroup hct_io
 *Ӳ���ܽ�ETXD0���ù���
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
 *Ӳ���ܽ�ETXD1���ù���
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
 *Ӳ���ܽ�ETXD2���ù���
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
 *Ӳ���ܽ�ETXD3���ù���
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
 *Ӳ���ܽ�ETXEN���ù���
 */
typedef enum
{
	HI_GPIO30_SEL_ETXEN,
	HI_GPIO30_SEL_GPIO30,	
	HI_GPIO30_SEL_EXT_AFE_ADIO_1,
}HI_HW_GPIO30_SEL_FUN_E;

/**
 * @ingroup hct_io
 *Ӳ���ܽ�PERI_RSTN���ù���
 */
typedef enum
{
	HI_GPIO31_SEL_PERI_RSTN,
	HI_GPIO31_SEL_GPIO31,	
}HI_HW_GPIO31_SEL_FUN_E;

/**
 * @ingroup hct_io
 *Ӳ���ܽ�MDCK���ù���
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
 *Ӳ���ܽ�MDIO���ù���
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
 *Ӳ���ܽ�ETXCK���ù���
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
 *Ӳ���ܽ�ERXCK���ù���
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
 *Ӳ���ܽ�ERXDV���ù���
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
 *Ӳ���ܽ�ERXER���ù���
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
 *Ӳ���ܽ�ECOL���ù���
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
 *Ӳ���ܽ�ECRS���ù���
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
 *Ӳ���ܽ�ERXD0���ù���
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
 *Ӳ���ܽ�ERXD1���ù���
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
 *Ӳ���ܽ�ERXD2���ù���
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
 *Ӳ���ܽ�ERXD3���ù���
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
 *Ӳ���ܽ�GPIO26���ù���
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
 *Ӳ���ܽ�GPIO27���ù���
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
 *Ӳ���ܽ�GPIO28���ù���
 */
typedef enum
{    
    HI_GPIO28_BACKUP_SEL_GPIO28,
    HI_GPIO28_BACKUP_SEL_SSP_CSN = HI_GPIO28_BACKUP_SEL_GPIO28 + 5,
}HI_HW_GPIO28_BACKUP_SEL_FUN_E;//HI_HW_GPIO28_BACKUP

/**
 * @ingroup hct_io
 *Ӳ���ܽ�GPIO29���ù���
 */
typedef enum
{
    HI_GPIO29_BACKUP_SEL_EFUSE_PW_E, 
    HI_GPIO29_BACKUP_SEL_GPIO29, 
}HI_HW_GPIO29_BACKUP_SEL_FUN_E;//HI_HW_GPIO29_BACKUP

/**
 * @ingroup hct_io
 * IOӲ���ܽ�����
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
	HI_HW_ETXD0_IO26,// ��1����ַ
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
    HI_HW_ECRS_IO16_BACKUP,// ��25����ַ
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
 * IO��ƽ״̬
 */
typedef enum
{
	HI_GPIO_VALUE0 = 0, /**< �͵�ƽ */
	HI_GPIO_VALUE1      /**< �ߵ�ƽ */
}HI_GPIO_VALUE_E;

/**
 * @ingroup hct_io
 * IO����
 */
typedef enum
{
	HI_GPIO_DIRECTION_IN = 0,   /**< ���� */
	HI_GPIO_DIRECTION_OUT       /**< ��� */
}HI_GPIO_DIRECTION_E;

/**
 * @ingroup hct_io
 * IO�жϴ�����ʽ
 */
typedef enum
{
	HI_INT_LEVEL = 0,   /**< ��ƽ���� */
	HI_INT_EDGE         /**< ���ش��� */
}HI_GPIO_INT_E;

/**
 * @ingroup hct_io
 * IO�жϼ���
 */
typedef enum
{
    HI_EDGE_FALL_LEVEL_LOW = 0, /**< �½��� */
    HI_EDGE_RISE_LEVEL_HIGH     /**< �������� */
}HI_GPIO_INT_POLARITY_E;

/**
 * @ingroup hct_io
 *
 * IO�жϴ�����ִ�е�λ��
 */
typedef enum
{
    HI_LATTER_OF_INTERRUPTION = 0,  /**< �жϺ���ִ�� */
    HI_HALF_OF_INTERRUPTION         /**< �ж�ǰ���ִ�� */
}HI_GPIO_INT_EXECUTE_POSITION_E;

/**
* @ingroup  hct_io
* @brief  ���õ���IO�ܽŹ��ܡ�
*
* @par ����:
* ���õ���IO�ܽŹ��ܡ�
* @attention 
* @li �ýӿ�֧������������������ʹ�á�
* @li �ýӿ�֧����LISR��������ʹ�á�
* @li �ýӿ�֧���ڹ��ж���������ʹ�á�
*
* @param  enId [IN] ���� #HI_HW_IO_IDX_E  IO�ܽ�������
* @param  ulVal [IN] ���� #HI_U8  ���ܽŹ�����ʹ������ö���в���:
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
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_io.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_IO_GetMux
* @since HiMDL_V200R001C00
*/
#define HI_MDM_IO_SetMux(enId, ulVal) HI_IO_SetMux(enId, ulVal)

/**
* @ingroup  hct_io
* @brief  ��ȡ����IO�ܽŹ��ܡ�
*
* @par ����:  
* ��ȡ����IO�ܽŹ��ܡ�
* @attention 
* @li �ýӿ�֧������������������ʹ�á�
* @li �ýӿ�֧����LISR��������ʹ�á�
* @li �ýӿ�֧���ڹ��ж���������ʹ�á�
*
* @param  enId [IN] ���� #HI_HW_IO_IDX_E  IO�ܽ�������
* @param  pVal [OUT] ���� #HI_U8 * ���ܡ�
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_io.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_IO_SetMux
* @since HiMDL_V200R001C00
*/
#define HI_MDM_IO_GetMux(enId, pVal) HI_IO_GetMux(enId, pVal)

/**
* @ingroup  hct_io
* @brief  ���õ���IO�ܽŷ���
*
* @par ����:  
* ���õ���IO�ܽŷ���
* @attention 
* @li �ýӿ�֧������������������ʹ�á�
* @li �ýӿ�֧����LISR��������ʹ�á�
* @li �ýӿ�֧���ڹ��ж���������ʹ�á�
*
* @param  enId [IN] ���� #HI_GPIO_IDX_E  IO ������
* @param  enDir [IN] ���� #HI_GPIO_DIRECTION_E  IO����
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_io.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_IO_GetDir
* @since HiMDL_V200R001C00
*/
#define HI_MDM_IO_SetDir(enId, enDir) HI_IO_SetDir(enId, enDir)

/**
* @ingroup  hct_io
* @brief  ��ȡ����IO�ܽŷ���
*
* @par ����:
* ��ȡ����IO�ܽŷ���
* @attention 
* @li �ýӿ�֧������������������ʹ�á�
* @li �ýӿ�֧����LISR��������ʹ�á�
* @li �ýӿ�֧���ڹ��ж���������ʹ�á�
*
* @param  enId [IN] ���� #HI_GPIO_IDX_E   IO ������
* @param  pDir [OUT] ���� #HI_GPIO_DIRECTION_E * IO����
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_io.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_IO_SetDir
* @since HiMDL_V200R001C00
*/
#define HI_MDM_IO_GetDir(enId, pDir) HI_IO_GetDir(enId, pDir)

/**
* @ingroup  hct_io
* @brief  ���õ���IO�ܽ������ƽ״̬��
*
* @par ����:
* ���õ���IO�ܽ������ƽ״̬��
* @attention 
* @li �ýӿ�֧������������������ʹ�á�
* @li �ýӿ�֧����LISR��������ʹ�á�
* @li �ýӿ�֧���ڹ��ж���������ʹ�á�
*
* @param  enId [IN] ���� #HI_GPIO_IDX_E  IO ������
* @param  enVal [IN] ���� #HI_GPIO_VALUE_E   ���ֵ��1Ϊ�ߵ�ƽ��0Ϊ�͵�ƽ��
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_io.h: �ýӿ��������ڵ�ͷ�ļ���
* @see HI_MDM_IO_GetInputVal | HI_MDM_IO_GetOutputVal
* @since HiMDL_V200R001C00
*/
#define HI_MDM_IO_SetOutputVal(enId, enVal) HI_IO_SetOutputVal(enId, enVal)

/**
* @ingroup  hct_io
* @brief  ��ȡ����IO�ܽ������ƽ״̬��
*
* @par ����:
* ��ȡ����IO�ܽ������ƽ״̬��
* @attention 
* @li �ýӿ�֧������������������ʹ�á�
* @li �ýӿ�֧����LISR��������ʹ�á�
* @li �ýӿ�֧���ڹ��ж���������ʹ�á�
*
* @param  enId [IN] ���� #HI_GPIO_IDX_E  IO ������
* @param  pVal [OUT] ���� #HI_GPIO_VALUE_E* ��ƽ״̬��1Ϊ�ߵ�ƽ��0Ϊ�͵�ƽ��
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_io.h: �ýӿ��������ڵ�ͷ�ļ���
* @see HI_MDM_IO_SetOutputVal | HI_MDM_IO_GetOutputVal
* @since HiMDL_V200R001C00
*/
#define HI_MDM_IO_GetInputVal(enId, pVal) HI_IO_GetInputVal(enId, pVal)

/**
* @ingroup  hct_io
* @brief  ��ȡ����IO�ܽ������ƽ״̬��
*
* @par ����:
* ��ȡ����IO�ܽ������ƽ״̬��
* @attention 
* @li �ýӿ�֧������������������ʹ�á�
* @li �ýӿ�֧����LISR��������ʹ�á�
* @li �ýӿ�֧���ڹ��ж���������ʹ�á�
*
* @param  enId [IN] ���� #HI_GPIO_IDX_E  IO ������
* @param  pVal [OUT] ���� #HI_GPIO_VALUE_E *��ƽ״̬��1Ϊ�ߵ�ƽ��0Ϊ�͵�ƽ��
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_io.h: �ýӿ��������ڵ�ͷ�ļ���
* @see HI_MDM_IO_SetOutputVal | HI_MDM_IO_GetInputVal
* @since HiMDL_V200R001C00
*/
#define HI_MDM_IO_GetOutputVal(enId, pVal) HI_IO_GetOutputVal(enId, pVal)

/**
* @ingroup  hct_io
* @brief  ʹ��ĳ��GPIO���жϹ��ܡ�
*
* @par ����:
* ʹ��ĳ��GPIO���жϹ��ܡ�
* @attention 
* @li �ýӿ�֧������������������ʹ�á�
* @li �ýӿ�֧����LISR��������ʹ�á�
* @li �ýӿ�֧���ڹ��ж���������ʹ�á�
*
* @param  enId [IN] ���� #HI_GPIO_IDX_E  IO ������
* @param  enIntType [IN] ���� #HI_GPIO_INT_E �ж����ͣ�0Ϊ��ƽ�����жϣ�1Ϊ�ش����жϡ�
* @param  enIntPolarity [IN] ���� #HI_GPIO_INT_POLARITY_E�жϼ��ԣ�0Ϊ�½��ػ�͵�ƽ������1Ϊ�����ػ�ߵ�ƽ������
* @param  gpio_callback [IN] ���� #HI_S32 (*gpio_callback)()  �жϻص�������
* @param  ulCallbackArgv [IN] ���� #HI_U32 �жϻص�����������
* @param  ulFlags [IN] ���� #HI_GPIO_INT_EXECUTE_POSITION_E �������ж�ǰ��ε��ã����Ǻ��ε��á�
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_io.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_IO_IntDisconnect
* @since HiMDL_V200R001C00
*/
#define HI_MDM_IO_IntConnect(enId, enIntType, enIntPolarity, gpio_callback, ulCallbackArgv, ulFlags) HI_IO_IntConnect(enId, enIntType, enIntPolarity, gpio_callback, ulCallbackArgv, ulFlags)

/**
* @ingroup  hct_io
* @brief  ȥʹ��ĳ��GPIO���жϹ��ܡ�
*
* @par ����:
* ȥʹ��ĳ��GPIO���жϹ��ܡ�
* @attention 
* @li �ýӿ�֧������������������ʹ�á�
* @li �ýӿ�֧����LISR��������ʹ�á�
* @li �ýӿ�֧���ڹ��ж���������ʹ�á�
*
* @param  enId [IN] ���� #HI_GPIO_IDX_E  IO ������
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_io.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_IO_IntConnect
* @since HiMDL_V200R001C00
*/
#define HI_MDM_IO_IntDisconnect(enId) HI_IO_IntDisconnect(enId)

/**
* @ingroup  hct_io
* @brief  IO�жϳ�ʼ����
*
* @par ����:
* IO�жϳ�ʼ����
* @attention �ޡ�
*
* @param �ޡ�
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_io.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
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
