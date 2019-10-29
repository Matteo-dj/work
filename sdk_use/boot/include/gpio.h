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

//#define PRODUCT_CFG_COLLECT_II
void gpio_init(void);
void uart_set_485tx_mode(void);
void uart_set_485rx_mode(void);