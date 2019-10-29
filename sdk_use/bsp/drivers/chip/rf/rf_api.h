
#ifndef __RF_API_H__
#define __RF_API_H__

#define SI4438_SPI_BAUD_RATE    6000000
extern HI_U32 g_ulSpiFd;

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)//�޸�IO���ã�ͬʱҪ�޸�IO��ʼ���ӿ�
#define RF_SDN_GPIO_IDX    HI_GPIO_IDX_29
#define RF_IRQ_GPIO_IDX    HI_GPIO_IDX_27

#else
#define RF_SDN_GPIO_IDX    HI_GPIO_IDX_25
#define RF_IRQ_GPIO_IDX    HI_GPIO_IDX_31
#endif


#endif