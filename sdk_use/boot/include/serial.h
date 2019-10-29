#ifndef __SERIAL_H__
#define __SERIAL_H__

int serial_init (u32 uart_baseaddr);
void serial_putc (const char c);
int serial_getc (void);
int serial_tstc (void);
void serial_puts (const char *s);
void serial_puthex(unsigned long h,unsigned char print_all);

int serial_getc_1 (u32 uart_baseaddr);
int serial_tstc_1 (u32 uart_baseaddr);

#endif
