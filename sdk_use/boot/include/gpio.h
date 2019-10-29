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

//#define PRODUCT_CFG_COLLECT_II
void gpio_init(void);
void uart_set_485tx_mode(void);
void uart_set_485rx_mode(void);