#ifndef         HIS_INT_DEFS_H
#define         HIS_INT_DEFS_H
#include <hi_types.h>
#include "esal_ar_int_defs.h"

#define		HI_IRQ_WDT			     (0)
#define		HI_IRQ_PHY			     (1)
#define		HI_IRQ_COMMRX		     (2)
#define		HI_IRQ_COMMTX		     (3)
#define		HI_IRQ_TIMER0		     (4)
#define		HI_IRQ_TIMER1		     (5)
#define		HI_IRQ_TIMER2		     (6)
#define		HI_IRQ_TIMER3		     (7)
#define		HI_IRQ_DMAC			     (8)
#define		HI_IRQ_RESERVE0 	     (9)
#define		HI_IRQ_GPIO		         (10)
#define		HI_IRQ_RESERVE1	     	 (11)
#define		HI_IRQ_UART0		     (12)
#define		HI_IRQ_UART1		     (13)
#define		HI_IRQ_UART2		     (14)
#define		HI_IRQ_UART3_IR	         (15)
#define		HI_IRQ_ETH			     (16)
#define		HI_IRQ_RESERVE2		     (17)
#define		HI_IRQ_TEMPERATURE_CTRL  (18)
#define		HI_IRQ_PLC_SC		     (19)
#define     HI_IRQ_TF                (20)
#define		HI_IRQ_RESERVE6	       	 (21)
#define		HI_IRQ_RESERVE7	      	 (22)
#define		HI_IRQ_I2C		         (23)
#define		HI_IRQ_SPI		         (24)
#define		HI_IRQ_SFC		     	 (25)
#define		HI_IRQ_RESERVE8		     (26)
#define		HI_IRQ_RESERVE9		     (27)
#define		HI_IRQ_RESERVE10	     (28)
#define		HI_IRQ_RESERVE11 	     (29)
#define		HI_IRQ_CIPHER		     (30)
#define		HI_IRQ_CRC		  	     (31)

#define         HI_IRQ_MAX                   (32)

#define         UART_PERIPH_BASE             0x10100000

typedef enum HI_IRQ_PRIORITY_ENUM
{
    HI_IRQ_PRIORITY_0,
    HI_IRQ_PRIORITY_1,
    HI_IRQ_PRIORITY_2,
    HI_IRQ_PRIORITY_3,
    HI_IRQ_PRIORITY_4,
    HI_IRQ_PRIORITY_5,
    HI_IRQ_PRIORITY_6,
    HI_IRQ_PRIORITY_7,
    HI_IRQ_PRIORITY_8,
    HI_IRQ_PRIORITY_9,
    HI_IRQ_PRIORITY_A,
    HI_IRQ_PRIORITY_B,
    HI_IRQ_PRIORITY_C,
    HI_IRQ_PRIORITY_D,
    HI_IRQ_PRIORITY_E,
    HI_IRQ_PRIORITY_F 
} HI_IRQ_PRIORITY;

#endif  /* HIS_INT_DEFS_H */
