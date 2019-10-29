#ifndef __DW_SERIAL_H__
#define __DW_SERIAL_H__

#include <types.h>

#define DW_UART_RBR          0x00	 /*  Receive buff. */
#define DW_UART_THR          0x00	 /*  Transmit Holding  */
#define DW_UART_DLL          0x00	 /*  Divisor Latch Low */

#define DW_UART_DLH          0x04  /*  Divisor Latch High */
#define DW_UART_IER          0x04  /*  int enable*/

#define DW_UART_IIR          0x08  /*  interrupt indentification REG*/
#define DW_UART_FCR          0x08  /*  FIFO control*/

#define DW_UART_LCR          0x0C  /*  Line control */
#define DW_UART_MCR          0x10  /*  Modem control */
        
#define DW_UART_LSR          0x14  /*  Line statue  */

#define DW_UART_USR          0x7C  /*  Uart statues */

#define DW_UART_HTX          0xA4  /*  Halt Tx*/

#define DE_FIFO_DEPTH        0x10   /*16bytes FIFO*/
/*
*
* DW_UART_LCR bit field
*
*/

#define DW_UART_DLAB               (1 << 7)
#define DW_UART_Break              (1 << 6)
#define DW_UART_EPS                (1 << 4)
#define DW_UART_PEN                (1 << 3)
#define DW_UART_STOP               (1 << 2)
#define DW_UART_8bit               0x03

#endif
