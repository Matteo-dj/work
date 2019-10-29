
#ifndef	__CPU_H__
#define	__CPU_H__

#include "asm/platform.h"

#define CONFIG_MAX_IRQ		32

#define IRQ_BASE 0x10140000
//#define IRQ_INT_IRQSTATUS 0X000
//#define IRQ_INT_INTENABLE 0x010
//#define IRQ_INT_INTSELECT 0X00c


#define IRQ_INTEN_L           0x10140000
#define IRQ_INTMASK_L       0x10140008
#define IRQ_INTFORCE_L     0x10140010
#define IRQ_RAWSTATUS_L  0x10140018
#define IRQ_STATUS_L         0x10140020
#define IRQ_MASKSTATUS_L 0x10140028
#define IRQ_FINALSTATUS_L 0x10140030
#define IRQ_PLEVEL              0x101400D8


#endif

