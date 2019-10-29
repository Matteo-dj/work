
#define HI_SUPPORT_UART_PRINT
#if defined HI_SUPPORT_UART_PRINT
unsigned int g_ulSupportUartDynPrint=1;

#include <stdarg.h>
#include <hi-early-debug.h>
#include <hi_stdlib.h>


#define  BOOTUP_UART_BASE 0x101ed000

#define  DW_UART_DATA \
	(*(volatile unsigned char *)(BOOTUP_UART_BASE + 0x000))
#define  DW_UART_IER \
	(*(volatile unsigned char *)(BOOTUP_UART_BASE + 0x004))
#define  DW_UART_STATUE \
	(*(volatile unsigned char *)(BOOTUP_UART_BASE + 0x014))
	




static void edb_putc(char c)
{
	while ((DW_UART_STATUE & (0x20)) != 0x20)
		;
	DW_UART_DATA = (unsigned char)c;
}

static void edb_putstr(const char *s)
{
    if(s)
    {
        while (*s) {
            edb_putc(*s);
            s++;
        }
    }
}

static void edb_puthex(unsigned int h)
{
	int i;
	char c;

	edb_putstr("0x");
	for (i = 0; i < 8; i++) {
		c = (h>>28)&0x0F;

		if (c >= 10)
			c = (c-10)+'A';
		else
			c = c+'0';

		edb_putc(c);

		h = h<<4;
	}
    return ;
}

static void deb_puthexc(char c)
{
    char c1;
    c1=((unsigned char)c>>4)&0xf;
    c1=(c1>=10)?(c1-10)+'A':c1+'0';
    edb_putc(c1);
    c1=((HI_U8)c)&0xf;
    c1=((unsigned char)c1>=10)?((c1-10)+'A'):(c1+'0');
    edb_putc(c1);
}


void edb_put_str_only(const char *s)
{
    if(g_ulSupportUartDynPrint>0)
    {
        edb_putstr(s);
    }
}


void edb_put_str_p0(const char *s)
{ 
    if(g_ulSupportUartDynPrint>0)
    {
        edb_putstr(s);
        edb_putstr("\r\n");
    }
}

void edb_put_str_p1(const char *s, const unsigned int p1)
{ 
    if(g_ulSupportUartDynPrint>0)
    {
        edb_putstr(s); 
        edb_puthex(p1); 
        edb_putstr("\r\n");
    }


}
void edb_put_str_p2(const char *s, const unsigned int p1, const unsigned int p2)
{ 
    if(g_ulSupportUartDynPrint>0)
    {
        edb_putstr(s); 
        edb_puthex(p1);
        edb_putstr(" ");
        edb_puthex(p2); 
        edb_putstr("\r\n");

    }
}
void edb_put_str_p3(const char *s, const unsigned int p1, const unsigned int p2, const unsigned int p3)
{ 
    if(g_ulSupportUartDynPrint>0)
    {    
        edb_putstr(s); 
        edb_puthex(p1);
        edb_putstr(" ");
        edb_puthex(p2);
        edb_putstr(" ");
        edb_puthex(p3); 
        edb_putstr("\r\n");
    }

}
void edb_put_str_p4(const char *s, const unsigned int p1, const unsigned int p2, const unsigned int p3, const unsigned int p4)
{ 
    
    if(g_ulSupportUartDynPrint>0)
    {
        edb_putstr(s); 
        edb_puthex(p1);
        edb_putstr(" ");
        edb_puthex(p2);
        edb_putstr(" ");
        edb_puthex(p3);
        edb_putstr(" ");
        edb_puthex(p4); 
        edb_putstr("\r\n");
    }

}

void edb_put_buf(const char *buf, const unsigned int len)
{

    unsigned int i=0;
    
    if(g_ulSupportUartDynPrint > 0)
    {
        for(i=0;i<len;i++)
        {
            deb_puthexc(buf[i]);
            if((i&0x7)==1)//每8个字节进行换行
            {
                edb_putstr("\r\n");
            }
                
        }
        
        if((i&0x7)==1)//每8个字节进行换行
        {
            edb_putstr("\r\n");
        }

    }

}


#define SIZEBUF         256
typedef int (*OUTPUT_FUNC)(const char *str, unsigned int len, void *state);
extern int __dprintf(const char *format, va_list arg,OUTPUT_FUNC pFputc,void *cookie);


static int uart_output(const char *s, int len, void *state)
{
    int tmp_len = len;
    const char* tmp_ptr = (s);

    while(tmp_len--)
    {
        if(*tmp_ptr=='\n')
        {
            edb_putc('\r');
            edb_putc('\n');
        }
        else
        {
            (void)edb_putc(*tmp_ptr);
        }
        tmp_ptr++;
    }
    return len;
}



void dprintf(const char *fmt, ...)
{
    char abuf[SIZEBUF];
    va_list ap;
    va_start(ap, fmt);/*lint !e530*/
    
    if(g_ulSupportUartDynPrint>0)
    {
        (hi_void)memset_s(abuf, SIZEBUF, 0, SIZEBUF);
        __dprintf(fmt, ap, (OUTPUT_FUNC)uart_output, abuf);
    }
    
    va_end(ap);
}


int printf(const char *fmt, ...)
{
    char abuf[SIZEBUF];
    va_list ap;

    va_start(ap, fmt);/*lint !e530*/
    
    if(g_ulSupportUartDynPrint > 0)
    {
        (hi_void)memset_s(abuf, SIZEBUF, 0, SIZEBUF);
        __dprintf(fmt, ap, (OUTPUT_FUNC)uart_output, abuf);

    }
    va_end(ap);
    return 0;
} 

void edb_set_print_level(unsigned int level)
{
    g_ulSupportUartDynPrint = level;
}

#else
void edb_put_str_only(const char *s){}
void edb_put_str_p0(const char *str){}
void edb_put_str_p1(const char *str, const unsigned int p1){}
void edb_put_str_p2(const char *str, const unsigned int p1, const unsigned int p2){}
void edb_put_str_p3(const char *str, const unsigned int p1, const unsigned int p2, const unsigned int p3){}
void edb_put_str_p4(const char *str, const unsigned int p1, const unsigned int p2, const unsigned int p3, const unsigned int p4){}
void edb_put_buf(const char *buf, const unsigned int len){}
int  printf(const char *fmt, ...){return 0;}
void dprintf(const char *fmt, ...){}
void edb_set_print_level(unsigned int level){}
#endif

