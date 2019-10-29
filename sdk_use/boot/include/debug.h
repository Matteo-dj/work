#ifndef _DEBUG_H_
#define _DEBUG_H_

#include <serial.h>
#include <reg.h>

//#define BOOT_TIMER_DEBUG_ON
#define BOOT_MEM_DEBUG_ON

#ifdef BOOT_MEM_DEBUG_ON
#define BOOT_MALLOC_SIZE        GENERAL_REG_1
#define BOOT_FREE_SIZE          GENERAL_REG_2
#endif

#define print_putc(c) serial_putc(c)
#define print_hex(h,print0)\
    do{\
        serial_puts(" ");\
        serial_puthex(h,print0);\
    }while(0)
#define print_msg0(s)\
    do{\
        serial_puts(s);\
        serial_puts("\r\n");\
    }while(0)
#define print_msg1(s,h)\
    do{\
        serial_puts(s);\
        serial_puthex(h,0);\
        serial_puts("\r\n");\
    }while(0)
#define print_msg2(s,h1,h2)\
    do{\
        serial_puts(s);\
        serial_puthex(h1,0);\
        serial_puts(" ");\
        serial_puthex(h2,0);\
        serial_puts("\r\n");\
    }while(0)


#endif
    