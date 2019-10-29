#include <chip.h>
#include <led.h>
#include <watchdog.h>
#include <product.h>
#include <reg.h>
#include <iomux.h>
#include <command.h>
#include <debug.h>
#include <boot.h>
#include "securec.h"
#include <gpio.h>
extern u32 g_uart_baseaddr;
void enable_watchdog(void)
{
    watchdog_enable(WATCHDOG_TIME);
}
void disable_watchdog(void)
{
    watchdog_disable();
}

void led_status(LED_PHASE_E phase)
{
    if(phase == led_phase1) 
    {
        led_cycle(HI_LED4,50,50);
        led_cycle(HI_LED3,200,200);
    }
    else if(phase == led_phase2) 
    {
        led_cycle(HI_LED3,50,50);
        led_cycle(HI_LED4,200,200);
    }
    else if(phase == led_phase3)
    {
        led_cycle(HI_LED4,2000,2000);
        led_cycle(HI_LED3,2000,2000);        
    }
      
}

int ctrlc (void)
{
	if (serial_tstc()) 
	{
		switch (serial_getc ())
		{
			case 0x03:		/* ^C - Control C */
				return 1;
			default:
				break;
		}
	}
	return 0;
}
int ctrlc_1 (u32 uart_baseaddr)
{
	if (serial_tstc_1(uart_baseaddr)) 
	{
		switch (serial_getc_1 (uart_baseaddr))
		{
			case 0x03:		/* ^C - Control C */
				return 1;
			default:
				break;
		}
	}
	return 0;
}
u8 ischipT(void) 
{
    unsigned int  val = 0;

    HI_REG_READ(SYS_BASE_SC_VERSION_ID_REG, val);    
    return (u8)(SYS_SC_VERSION_ID_BIT_TYPE_ID_CHIP_T & val);
}
u8 ischipV200(void) 
{
    unsigned int  val = 0;

    HI_REG_READ(SYS_BASE_SC_VERSION_ID_REG, val); 
    return (u8)(((val & CHIP_VERSION_MASK) >> 4) == CHIP_VERSION_V200);
}

#define SEL_UART 0
#define SEL_I2C  0
#define SEL_GPIO 1
#define SEL_UART1_V200 4
#define SEL_LED_V200  3
#define SEL_UART0_V200 3
#define SEL_UART0N_V200 5
#define SEL_LDRVR_PD 0

/*复位为GPIO管脚后需与boot配置寄存器表格核对输入输出*/
void io_remap(void)
{
    iomux_config(HI_HW_IO10, SEL_UART);
    iomux_config(HI_HW_IO14, SEL_UART);
#ifdef SUPPORT_EQUIP_TEST
	iomux_config(HI_HW_IO0, SEL_GPIO);
	iomux_config(HI_HW_IO1, SEL_GPIO);
    iomux_config(HI_HW_IO2, SEL_GPIO);
    iomux_config(HI_HW_IO3, SEL_GPIO);
    iomux_config(HI_HW_IO5, SEL_GPIO);
    iomux_config(HI_HW_IO19, SEL_GPIO);
    iomux_config(HI_HW_IO24, SEL_GPIO);

    iomux_config(HI_HW_IO11, SEL_UART);//工装使用UART2
    iomux_config(HI_HW_IO12, SEL_UART);

    iomux_config(HI_HW_IO7, SEL_I2C);
    iomux_config(HI_HW_IO8, SEL_I2C);
#else
    if ((ischipT()) && (ischipV200()))
    {
        iomux_config(HI_HW_IO0, SEL_UART1_V200);
        iomux_config(HI_HW_IO1, SEL_UART1_V200);
        iomux_config(HI_HW_IO13, SEL_LED_V200);
        iomux_config(HI_HW_IO14, SEL_LED_V200);
#ifndef PRODUCT_CFG_COLLECT_II	
        iomux_config(HI_HW_IO7, SEL_UART0_V200);
        iomux_config(HI_HW_IO8, SEL_UART0N_V200); 
#else
	iomux_config(HI_HW_IO7, SEL_GPIO);
        iomux_config(HI_HW_IO8, SEL_GPIO); 
	 iomux_config(HI_HW_IO9, SEL_UART);
    	iomux_config(HI_HW_IO10, SEL_UART);
#endif
    }
    
    iomux_config(HI_HW_IO4, SEL_GPIO);
    iomux_config(HI_HW_IO5, SEL_LDRVR_PD);
    iomux_config(HI_HW_IO11, SEL_GPIO);
    iomux_config(HI_HW_IO12, SEL_GPIO);		
#endif
}

void serial_puthex(unsigned long h,unsigned char print_all)
{
	int i;
	char c;
	char mark = 0;

	serial_puts("0x");
	for (i = 0; i < 8; i++) {
		c = (h>>28)&0x0F;

		if (c >= 10)
			c = (c-10)+'A';
		else
			c = c+'0';

		if (print_all)
			/*
										【屏蔽人】z00193780
										【审核人】c00233580
										【代码分析】通用接口，有可能使用该分支，需保留。
										【功能影响】不影响正常业务，屏蔽告警
									  */
			serial_putc(c);
		else
		{
			 /*
										【屏蔽人】z00193780
										【审核人】c00233580
										【代码分析】通用接口，有可能使用该分支，需保留。
										【功能影响】不影响正常业务，屏蔽告警
									  */
			//如果不是最后一个数且之前数字都为0
			if ((mark == 0)&&(i != 7))
			{
				if (c != '0')
				{
					mark = 1;
					serial_putc(c);
				}				   
			}
			else
				serial_putc(c);
		}

		h = h<<4;
	}
}

void check_ctrlc(void)
{
    unsigned int i = (CONFIG_FASTBOOT_DELAY);

#ifdef DISPLAY_DEBUG_INFO     
    if (i > 9)
    {
    	i = 9;
    }
#endif
    i = (i << 10);

    print_msg1("Hit Ctrl+C to stop autoboot:", (i >> 10));
    /*lint -e774*/
    if(i == 0)
    /*lint +e774*/
    {
        i=100;
    }
    do
    {
        if(ctrlc())
        {
            print_msg0("Input cmd:");
		    main_loop();           
        }
	if(ctrlc_1(HI_UART0_REG_BASE))
	{
		g_uart_baseaddr = HI_UART0_REG_BASE;
		print_msg0("Input cmd:");
		    main_loop();
	}
        if (i)
            mdelay(1);
#ifdef BSP_CODEX_CHECK_CLOSE        
        //if (!(i & 0x3FF))
            //printf("\b %d", (i >> 10));
#endif
    } while (i--);
#ifdef BSP_CODEX_CHECK_CLOSE	
    //printf("\n"); 
#endif
}

char        console_buffer[CFG_CBSIZE];		/* console I/O buffer	*/
static char erase_seq[] = "\b \b";		/* erase sequence	*/
static char   tab_seq[] = "        ";		/* used to expand TABs	*/


static char * delete_char (char *buffer, const char *p, int *colp, int *np, int plen)
{
	char *s;

	if (*np == 0) {
		return ((char *)p);
	}

	if (*(--p) == '\t') {			/* will retype the whole line	*/
		while (*colp > plen) {
			puts (erase_seq);
			(*colp)--;
		}
		for (s=buffer; s<p; ++s) {
			if (*s == '\t') {
				puts (tab_seq+((unsigned int)(*colp) & 07));
				*colp += (int)(8 - ((uint32_t)(*colp) & 07));
			} else {
				++(*colp);
				putc (*s);
			}
		}
	} else {
		puts (erase_seq);
		(*colp)--;
	}
	(*np)--;
	return ((char *)p);
}

/*
 * Prompt for input and read a line.
 * If  CONFIG_BOOT_RETRY_TIME is defined and retry_time >= 0,
 * time out when time goes past endtime (timebase time in ticks).
 * Return:	number of read characters
 *		-1 if break
 *		-2 if timed out
 */
int readline_into_buffer (const char *const prompt, char * buffer)
{
	char *p = buffer;
	char * p_buf = p;
	int	n = 0;				/* buffer index		*/
	int	plen = 0;			/* prompt length	*/
	int	col;				/* output column cnt	*/
	char	c;

	/* print prompt */
	if (prompt) {
		plen = (int)strlen(prompt);
		puts (prompt);
	}
	col = plen;

	for (;;) {

		c = (char)getc();

		/*
		 * Special character handling
		 */
		switch (c) {
		case '\r':				/* Enter		*/
		case '\n':
			*p = '\0';
			puts ("\r\n");
			/* added by wuxianfeng
			 * for read ‘\n’*/
			if (serial_tstc())
			{
				(void) getc();
			}
			return (p - p_buf);

		case '\0':				/* nul			*/
			continue;

		case 0x03:				/* ^C - break		*/
			p_buf[0] = '\0';	/* discard input */
			return (-1);

		case 0x15:				/* ^U - erase line	*/
			while (col > plen) {
				puts (erase_seq);
				--col;
			}
			p = p_buf;
			n = 0;
			continue;

		case 0x17:				/* ^W - erase word	*/
			p=delete_char(p_buf, p, &col, &n, plen);
			while ((n > 0) && (*p != ' ')) {
				p=delete_char(p_buf, p, &col, &n, plen);
			}
			continue;

		case 0x08:				/* ^H  - backspace	*/
		case 0x7F:				/* DEL - backspace	*/
			p=delete_char(p_buf, p, &col, &n, plen);
			continue;

		default:
			/*
			 * Must be a normal character then
			 */
			if (n < CFG_CBSIZE-2) {
				if (c == '\t') {	/* expand TABs		*/
					puts (tab_seq+((unsigned int)col&07));
					col += (int)(8 - ((uint32_t)col&07));
				} else {
					++col;		/* echo input		*/
					putc (c);
				}
				*p++ = c;
				++n;
			} else {			/* Buffer full		*/
				putc ('\a');
			}
		}
	}
}
int parse_line (char *line, const char *argv[])
{
	int nargs = 0;

	while (nargs < CFG_MAXARGS) {

		/* skip any white space */
		while ((*line == ' ') || (*line == '\t')) {
			++line;
		}

		if (*line == '\0') {	/* end of line, no more args	*/
			argv[nargs] = NULL;
			return (nargs);
		}

		argv[nargs++] = line;	/* begin of argument string	*/

		/* find end of string */
		while (*line && (*line != ' ') && (*line != '\t')) {
			++line;
		}

		if (*line == '\0') {	/* end of line, no more args	*/
			argv[nargs] = NULL;

			return (nargs);
		}

		*line++ = '\0';		/* terminate current arg	 */
	}

	return (nargs);
}
/***************************************************************************
 * find command table entry for a command
 */
cmd_tbl_t *find_cmd (const char const *cmd)
{
	cmd_tbl_t *cmdtp;
    /*lint -e85*/
    /*lint -e727*/
    /*lint -e550*/
    /*lint -e545*/
	cmd_tbl_t *cmdtp_temp = ll_entry_start(cmd_tbl_t, cmd);
    /*lint +e85*/
    /*lint +e727*/
    /*lint +e550*/
    /*lint +e545*/

	const char *p;
	int len;
	int n_found = 0;

	/*
	 * Some commands allow length modifiers (like "cp.b");
	 * compare command name only until first dot.
	 */
	len = (((p = strchr(cmd, '.')) == NULL) ? (int)strlen(cmd) : (int)(p - cmd));
    /*lint -e85*///数组维数为0
    /*lint -e727*/
    /*lint -e550*/
    /*lint -e545*/
	for (cmdtp = ll_entry_start(cmd_tbl_t, cmd);
		 cmdtp != ll_entry_end(cmd_tbl_t, cmd);
		 cmdtp++) 
    /*lint +e85*/
    /*lint +e727*/
    /*lint +e550*/
    /*lint +e545*/
	{
		if (strncmp (cmd, cmdtp->name, (size_t)len) == 0) {
			if (len == (int)strlen(cmdtp->name))
				return cmdtp;	/* full match */

			cmdtp_temp = cmdtp;	/* abbreviated command ? */
			n_found++;
		}
	}
	if (n_found == 1) {			/* exactly one match */
		return cmdtp_temp;
	}

	return NULL;	/* not found or ambiguous command */
}

int run_command (const char * cmd, int flag)
{
	cmd_tbl_t *cmdtp;
/* BEGIN: Modified by w00190206, 2013/4/1	PN:DTS2013032803734*/
	char cmdbuf[CFG_CBSIZE]={0};	/* working copy of cmd		*/
/* END:   Added by w00190206, 2013/4/1 */
	char *token;			/* start of token in cmdbuf	*/
	char *sep;			/* end of token (separator) in cmdbuf */
//	char finaltoken[CFG_CBSIZE];
	char *str = cmdbuf;
	const char *argv[CFG_MAXARGS + 1] = {0};	/* NULL terminated	*/
	int argc, inquotes;
	unsigned int repeatable = 1;
	unsigned int repeatableTemp;
	int rc = 0;

	//clear_ctrlc();		/* forget any previous Control C */

	if (!cmd || !*cmd) {
		return -1;	/* empty command */
	}

	if (strlen(cmd) >= CFG_CBSIZE) {
		puts ("## Cmd too long!\n");
		return -1;
	}

	strncpy_s(cmdbuf, sizeof(cmdbuf) - 1, cmd, sizeof(cmdbuf) - 1);

	/* Process separators and check for invalid
	 * repeatable commands
	 */

	while (*str) {

		/*
		 * Find separator, or string end
		 * Allow simple escape of ';' by writing "\;"
		 */
		for (inquotes = 0, sep = str; *sep; sep++) {
			if ((*sep=='\'') &&
				(*(sep-1) != '\\'))
				inquotes=!inquotes;

			if (!inquotes &&
				(*sep == ';') &&	/* separator		*/
				( sep != str) &&	/* past string start	*/
				(*(sep-1) != '\\'))	/* and NOT escaped	*/
				break;
		}

		/*
		 * Limit the token to data between separators
		 */
		token = str;
		if (*sep) {
			str = sep + 1;	/* start of command for next pass */
			*sep = '\0';
		}
		else
			str = sep;	/* no more commands for next pass */

		/* find macros in this token and replace them */
//		process_macros (token, finaltoken);

		/* Extract arguments */
		if ((argc = parse_line (token, argv)) == 0)
		{
			rc = -1;	/* no command at all */
			continue;
		}

		/* Look up command in command table */
		if ((cmdtp = find_cmd(argv[0])) == NULL) {
			print_msg0 ("Unknown cmd");
			rc = -1;	/* give up after bad command */
			continue;
		}

		/* found - check max args */
		if (argc > cmdtp->maxargs) {
			rc = -1;
			continue;
		}

		/* OK - call function to do the command */
		if ((cmdtp->cmd) (cmdtp, flag, argc, argv) != 0) {
			rc = -1;
		}
		repeatableTemp = (unsigned int)cmdtp->repeatable;
		repeatable &= repeatableTemp;

		/* Did the user stop this? */
	//	if (had_ctrlc ())
	//		return -1;	/* if stopped then not repeatable */
	}

	return rc ? rc : (int)repeatable;
}
void main_loop (void)
{
	static char lastcommand[CFG_CBSIZE] = {0,};
	int len;
	int flag;
	/*
	 * Main Loop for Monitor Command Processing
	 */
	led_status(led_phase3);//闪灯，用于标记进入命令行
	/*		  
	【屏蔽人】z00193780 	   
	【审核人】w00190206 	  
	【代码分析】命令行等待输入命令，不退出循环，如果无输入看门狗到期单板会重启，无需修改。		  
	【功能影响】不影响正常业务，屏蔽告警*/
	for (;;)
	{
		len = readline_into_buffer(CFG_PROMPT, console_buffer);

		flag = 0;	/* assume no special flags for now */
		if (len > 0)
			strncpy_s(lastcommand, sizeof(lastcommand) - 1, console_buffer, sizeof(lastcommand) - 1);
		else if (len == 0)
			flag = (int)((uint32_t)flag |(uint32_t)CMD_FLAG_REPEAT);

		if (len == -1)
			puts ("<INTERRUPT>\n");
		else
			run_command (lastcommand, flag);

		/* invalid command or not repeatable, forget it */
		lastcommand[0] = 0;
	}
}

