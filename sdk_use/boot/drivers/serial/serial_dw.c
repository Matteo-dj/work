
#include "serial_dw.h"
#include <reg.h>
#include <product.h>
#include <gpio.h>

#define baudRate CONFIG_BAUDRATE
//#define DW_UART_BASEADDR REG_BASE_UART1
#define IO_WRITE(addr, val) (*(volatile unsigned int *)(addr) = (val))
#define IO_READ(addr) (*(volatile unsigned int *)(addr))
#define UART_LOOPBACK_ENABLE (1<<4)

u32 g_uart_baseaddr = HI_UART1_REG_BASE;
/*
 * IntegratorCP has two UARTs, use the first one, at 38400-8-N-1
 * Versatile PB has four UARTs.
 */
static void dw_putc (char c);
static int dw_tstc (void);
static int dw_getc (void);
static void dw_putc_1 (u32 uart_baseaddr, char c);
void serial_puts_1 (u32 uart_baseaddr,const char *s);

/*
 *      function: wait untill UART idle by reading out(throw) all fifo data
 * 	c47559 add 2009-0218
 */
void dw_wait_idle(u32 uart_baseaddr)
{
	while(IO_READ(uart_baseaddr + DW_UART_USR) & 0x01)
	{
		if(IO_READ(uart_baseaddr + DW_UART_LSR) & 0x01)
		{
            /*lint -e522*/
			IO_READ(uart_baseaddr + DW_UART_RBR);
            /*lint +e522*/
		}
	}
}

/*
** 使用前后需要加serial_rx_remap接口来保护rx不会产生busy
*/
int serial_init (u32 uart_baseaddr)
{
	unsigned int uiData;
	unsigned int temp;
	unsigned int divider;
	unsigned char dividerH,dividerL;

	/* disable FIFO */
	IO_WRITE(uart_baseaddr + DW_UART_FCR , 0);

	/* disable interrupt */
	IO_WRITE(uart_baseaddr+DW_UART_IER , 0);
	
#ifdef PRODUCT_CFG_UART_INIT_NEW
	/* loopback mode */
	temp = IO_READ(uart_baseaddr+DW_UART_MCR);
	IO_WRITE(uart_baseaddr+DW_UART_MCR , temp|UART_LOOPBACK_ENABLE);
#endif
	
	dw_wait_idle(uart_baseaddr);

	IO_WRITE(uart_baseaddr+DW_UART_LCR , 0x80);

	/*Caculate devide*/
	temp = 16 * baudRate;
	divider = CONFIG_PL011_CLOCK / temp;
	dividerH =	((divider)&0xff00)>>8;
	dividerL =	((divider)&0x00ff);
	IO_WRITE(uart_baseaddr+DW_UART_DLL,dividerL);
	IO_WRITE(uart_baseaddr+DW_UART_DLH,dividerH);

	dw_wait_idle(uart_baseaddr);

	uiData = 0x0;	  
	IO_WRITE(uart_baseaddr+DW_UART_LCR , uiData);  /* disable DLL and DLH */

	dw_wait_idle(uart_baseaddr);
	
#ifdef PRODUCT_CFG_UART_INIT_NEW
	/* loopback mode */
	temp = IO_READ(uart_baseaddr+DW_UART_MCR);
	IO_WRITE(uart_baseaddr+DW_UART_MCR , temp&(~UART_LOOPBACK_ENABLE));
#endif
	uiData = 0x1b;	 /* 8bit data, 1bit stop,even parity */
	//uiData = 0x3; /* 8bit data, 1bit stop,no parity */
	IO_WRITE(uart_baseaddr+DW_UART_LCR , uiData); 

	return 0;
}

void serial_exit (void)
{
    int i;
    
    //去使能 UART0 的 FIFO
    IO_WRITE(g_uart_baseaddr + DW_UART_FCR , 0x6);
    
    for (i=0; i<DE_FIFO_DEPTH; i++)
    {
        /*lint -e522*/
        IO_READ(g_uart_baseaddr + DW_UART_RBR);
        /*lint +e522*/
    }

}
void serial_putc (const char c)
{
#ifdef PRODUCT_CFG_COLLECT_II
	if(g_uart_baseaddr == HI_UART0_REG_BASE)
	{
		uart_set_485tx_mode();
	}
#endif	
	if (c == '\n')  
		dw_putc ('\r');

	dw_putc (c);
#ifdef PRODUCT_CFG_COLLECT_II	
	if(g_uart_baseaddr == HI_UART0_REG_BASE)
	{
		while ((IO_READ (g_uart_baseaddr + DW_UART_LSR) & 0x40)!=0x40)
       		 ;
		uart_set_485rx_mode();
	}
#endif	
}

void serial_putc_i (const char c)
{
	if (c == '\n')  
		dw_putc ('\r');

	dw_putc (c);
}

void serial_puts (const char *s)
{
#ifdef PRODUCT_CFG_COLLECT_II	
	if(g_uart_baseaddr == HI_UART0_REG_BASE)
	{
		uart_set_485tx_mode();
	}
#endif	
	while (*s) {
		serial_putc_i (*s++);
	}
#ifdef PRODUCT_CFG_COLLECT_II	
	if(g_uart_baseaddr == HI_UART0_REG_BASE)
	{
		while ((IO_READ (g_uart_baseaddr + DW_UART_LSR) & 0x40)!=0x40)
       		 ;
		uart_set_485rx_mode();
	}
#endif	
}

int serial_getc (void)
{
	return dw_getc ();
}

int serial_tstc (void)
{
	return dw_tstc ();
}

/* send */
static void dw_putc (char c)
{
	/* Wait until THRE is empyt */
	while ((IO_READ (g_uart_baseaddr + DW_UART_LSR) & 0x20)!=0x20)
        ;

	/* send one char */
	IO_WRITE(g_uart_baseaddr + DW_UART_THR, (unsigned char)c);
}
/* receive */
static int dw_getc (void)
{
	unsigned int data;
	unsigned int err;   

	/* AI7D01852 add by d00104687 2007-12-15 start  */
	//unsigned long time_tmp = 0;
	//int count = 1<<20;  /* get the time a little less than one second.*/
	/* AI7D01852 add by d00104687 2007-12-15 end  */

	/* Wait until data is ready */
	while ((IO_READ (g_uart_baseaddr + DW_UART_LSR) & 0x01)!=0x01)
        ;

	/* receive one char */
	data = IO_READ(g_uart_baseaddr + DW_UART_RBR);

	/* Check for an error flag,read clear Reg */
	err  = (IO_READ (g_uart_baseaddr + DW_UART_LSR) & 0x1E);
	if(err)
	{   
		return -1;      
	}   
	return (int) data;
}

static int dw_tstc (void)
{
	return (IO_READ (g_uart_baseaddr + DW_UART_LSR) & 0x01);

	/*
	   return !(IO_READ (port[portnum] + UART_PL01x_FR) &
	   UART_PL01x_FR_RXFE);
	   */
}

/* send */
static void dw_putc_1 (u32 uart_baseaddr, char c)
{
	/* Wait until THRE is empyt */
	while ((IO_READ (uart_baseaddr + DW_UART_LSR) & 0x20)!=0x20)
        ;

	/* send one char */
	IO_WRITE(uart_baseaddr + DW_UART_THR, (unsigned char)c);
}
static int dw_tstc_1(u32 uart_baseaddr)
{
	return (IO_READ (uart_baseaddr + DW_UART_LSR) & 0x01);
}

static int dw_getc_1 (u32 uart_baseaddr)
{
	unsigned int data;
	unsigned int err;   

	/* AI7D01852 add by d00104687 2007-12-15 start  */
	//unsigned long time_tmp = 0;
	//int count = 1<<20;  /* get the time a little less than one second.*/
	/* AI7D01852 add by d00104687 2007-12-15 end  */

	/* Wait until data is ready */
	while ((IO_READ (uart_baseaddr + DW_UART_LSR) & 0x01)!=0x01)
        ;

	/* receive one char */
	data = IO_READ(uart_baseaddr + DW_UART_RBR);

	/* Check for an error flag,read clear Reg */
	err  = (IO_READ (uart_baseaddr + DW_UART_LSR) & 0x1E);
	if(err)
	{   
		return -1;      
	}   
	return (int) data;
}

void serial_putc_1 (u32 uart_baseaddr,const char c)
{
	if (c == '\n')  
		dw_putc_1 (uart_baseaddr,'\r');

	dw_putc_1 (uart_baseaddr,c);
}

void serial_puts_1 (u32 uart_baseaddr,const char *s)
{
	while (*s) {
		serial_putc_1 (uart_baseaddr,*s++);
	}
}

int serial_getc_1 (u32 uart_baseaddr)
{
	return dw_getc_1 (uart_baseaddr);
}

int serial_tstc_1 (u32 uart_baseaddr)
{
	return dw_tstc_1 (uart_baseaddr);
}