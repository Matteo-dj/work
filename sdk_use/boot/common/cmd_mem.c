/*
 * (C) Copyright 2000
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

/*
 * Memory Functions
 *
 * Copied from FADS ROM, Dan Malek (dmalek@jlc.net)
 */
#include <types.h>
#include <command.h>
#include <serial.h>
#include <ctype.h>
#include <boot.h>
#include <watchdog.h>

#define BOOT_PUTHEX(h,print0)\
    do{\
        serial_puts(" ");\
        console_puthex(h,print0);\
    }while(0)

/*
 * Print data buffer in hex and ascii form to the terminal.
 *
 * data reads are buffered so that each memory address is only read once.
 * Useful when displaying the contents of volatile registers.
 *
 * parameters:
 *    addr: Starting address to display at start of line
 *    data: pointer to data buffer
 *    width: data value width.  May be 1, 2, or 4.
 *    count: number of values to display
 *    linelen: Number of values to print per line; specify 0 for default length
 */
#define MAX_LINE_LENGTH_BYTES (64)
#define DEFAULT_LINE_LENGTH_BYTES (16)

void console_puthex(unsigned long h,unsigned char print_all)
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



int print_buffer (ulong addr, void* data, uint width, uint count, uint linelen)
{
	uint8_t linebuf[MAX_LINE_LENGTH_BYTES];
	uint32_t *uip = (uint32_t *)linebuf;
	u16 *usp = (u16 *)linebuf;
	uint8_t *ucp = (uint8_t *)linebuf;
	int i;

	if (linelen*width > MAX_LINE_LENGTH_BYTES)
		linelen = MAX_LINE_LENGTH_BYTES / width;
	if (linelen < 1)
		linelen = DEFAULT_LINE_LENGTH_BYTES / width;

	while (count)
	{
		BOOT_PUTHEX((unsigned long)addr, (unsigned char)1);
		serial_putc(':');

		/* check for overflow condition */
		if (count < linelen)
			linelen = count;

		/* Copy from memory into linebuf and print hex values */
		for (i = 0; (uint)i < linelen; i++)
		{
			if (width == 4) 
			{
				uip[i] = *(volatile uint32_t *)data;
				BOOT_PUTHEX((unsigned long)uip[i], (unsigned char)1);
			} 
			else if (width == 2) 
			{
				usp[i] = *((volatile u16 *)data);
				BOOT_PUTHEX((unsigned long)usp[i], (unsigned char)1);
			} 
			else 
			{
				ucp[i] = *(volatile uint8_t *)data;
				BOOT_PUTHEX((unsigned long)ucp[i], (unsigned char)1);
			}
			//data += width;
			data = (void *)((uint)data + width);
		}

		/* Print data in ASCII characters */
		puts("	 ");
		for (i = 0; (uint)i < linelen * width; i++)
			putc((char)(isprint(ucp[i]) && (ucp[i] < 0x80) ? ucp[i] : '.'));
		putc ('\n');

		/* update references */
		addr += linelen * width;
		count -= linelen;
		
		if (ctrlc())
		{
			return -1;
		}
	}

	return 0;
}

int cmd_get_data_size(const char* arg, int default_size)
{
	/* Check for a size specification .b, .w or .l.
	 */
	int len = (int)strlen(arg);
	if (len > 2 && arg[len-2] == '.') {
		switch(arg[len-1]) {
		case 'b':
			return 1;
		case 'w':
			return 2;
		case 'l':
			return 4;
		default:
			return -1;
		}
	}
	return default_size;
}

/* Display values from last command.
 * Memory modify remembered values are different from display memory.
 */
uint	dp_last_addr, dp_last_size;
uint	dp_last_length = 0x40;

static	ulong	base_address = 0;

/* Memory Display
 *
 * Syntax:
 *	md{.b, .w, .l} {addr} {len}
 */
#define DISP_LINE_LEN	16
int do_mem_md ( cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	ulong addr; 
	ulong length;
	int	size;
	int rc = 0;

    HI_UNREF_PARAM(cmdtp);

#ifdef BSP_CODEX_CHECK_CLOSE
	if (g_manage_authority != TRUE)
	{
		serial_puts(CFG_REQUIRE_PASSWORD);
		if(input_password())
			return -1;
	}
#endif
	watchdog_feed();
	/* We use the last specified parameters, unless new ones are
	 * entered.
	 */
	addr = dp_last_addr;
	size = (int)dp_last_size;
	length = dp_last_length;

	if (argc < 2) {
		return 1;
	}

	if (((uint32_t)flag & CMD_FLAG_REPEAT) == 0) {
		/* New command specified.  Check for a size specification.
		 * Defaults to long if no or incorrect specification.
		 */
		if ((size = cmd_get_data_size(argv[0], 4)) < 0)
			return 1;

		/* Address is specified since argc > 1
		*/
		addr = simple_strtoul(argv[1], (char **)HI_NULL, 16);
		addr += base_address;

		/* If another parameter, it is the length to display.
		 * Length is the number of objects, not number of bytes.
		 */
		if (argc > 2)
			length = simple_strtoul(argv[2], (char **)HI_NULL, 16);
	}
	/* Print the lines. */
	if (size)
	{
		print_buffer(addr, (void*)addr, (uint)size, (uint)length, (uint)(DISP_LINE_LEN/(uint)size));
	}
    
	addr += (ulong)(size*(int)length);
	dp_last_addr = addr;
	dp_last_length = length;
	dp_last_size = (uint)size;
	return (rc);
}

int do_mem_mw ( cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	ulong	addr, writeval, count;
	int	size;

    HI_UNREF_PARAM(cmdtp);
    HI_UNREF_PARAM(flag);

#ifdef BSP_CODEX_CHECK_CLOSE
    if (g_manage_authority != TRUE)
    {
    	serial_puts(CFG_REQUIRE_PASSWORD);
        if(input_password())
            return -1;
    }
#endif
    watchdog_feed();
	if ((argc < 3) || (argc > 4)) {
		return 1;
	}

	/* Check for size specification.
	*/
	if ((size = cmd_get_data_size(argv[0], 4)) < 1)
		return 1;

	/* Address is specified since argc > 1
	*/
	addr = simple_strtoul(argv[1], (char **)HI_NULL, 16);
	addr += base_address;

	/* Get the value to write.
	*/
	writeval = simple_strtoul(argv[2], (char **)HI_NULL, 16);

	/* Count ? */
	if (argc == 4) 
	{
		count = simple_strtoul(argv[3], (char **)HI_NULL, 16);
	} 
	else 
	{
		count = 1;
	}

	while (count > 0) 
	{
		if (size == 4)
		{
			*((ulong  *)addr) = (ulong )writeval;
		}
		else if (size == 2)
		{
			*((ushort *)addr) = (ushort)writeval;
		}
		else
		{
			*((u_char *)addr) = (u_char)writeval;
		}
		
		addr += (ulong)size;
		count--;
	}
	
	return 0;
}

//#define CMD_TEST_MEM
#ifdef CMD_TEST_MEM
/*
 * Perform a memory test. A more complete alternative test can be
 * configured using CFG_ALT_MEMTEST. The complete test loops until
 * interrupted by ctrl-c or by a failure of one of the sub-tests.
 */
 typedef unsigned long vu_long;



int do_mem_mtest (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	ulong	*addr, *start, *end;
	ulong	val;
	ulong	readback;
	int     rcode = 0;

	ulong	len;
	ulong	offset;
	ulong	test_offset;
	ulong	pattern;
	ulong	temp;
	ulong	anti_pattern;
	ulong	num_words;
	ulong   *dummy = 0x70000000;	/* yes, this is address 0x0, not NULL */
	int	j;
	int iterations = 1;
	static ulong bitpattern[] = {
		0x00000001,	/* single bit */
		0x00000003,	/* two adjacent bits */
		0x00000007,	/* three adjacent bits */
		0x0000000F,	/* four adjacent bits */
		0x00000005,	/* two non-adjacent bits */
		0x00000015,	/* three non-adjacent bits */
		0x00000055,	/* four non-adjacent bits */
		0xaaaaaaaa,	/* alternating 1/0 */
	};

	if ((argc < 3)) {
		BOOT_MSG0 (cmdtp->usage);
		return 1;
	}
    Watchdog_Disable();
	start = (ulong *)simple_strtoul(argv[1], NULL, 16);
	end = (ulong *)simple_strtoul(argv[2], NULL, 16);
    pattern = 0;

    console_puts("Testing from ...");
    console_puthex((uint)start,0);
    console_puts("  to  ");
    console_puthex((uint)end,0);
    console_puts("\r\n");


	for (;;) {
		if (ctrlc()) {
			putc ('\n');
			return 1;
		}
		if (iterations >= 1000)
		{
        console_puts("\r\n");    
		break;
		}
        console_puts("Iteration:");
        console_puthex((uint)iterations,0);
		iterations++;
        console_puts("\r");
		/*
		 * Data line test: write a pattern to the first
		 * location, write the 1's complement to a 'parking'
		 * address (changes the state of the data bus so a
		 * floating bus doen't give a false OK), and then
		 * read the value back. Note that we read it back
		 * into a variable because the next time we read it,
		 * it might be right (been there, tough to explain to
		 * the quality guys why it prints a failure when the
		 * "is" and "should be" are obviously the same in the
		 * error message).
		 *
		 * Rather than exhaustively testing, we test some
		 * patterns by shifting '1' bits through a field of
		 * '0's and '0' bits through a field of '1's (i.e.
		 * pattern and ~pattern).
		 */

		addr = start;
		for (j = 0; j < sizeof(bitpattern)/sizeof(bitpattern[0]); j++) {
		    val = bitpattern[j];
		    for(; val != 0; val <<= 1) {
			*addr  = val;
			*dummy  = ~val; /* clear the test data off of the bus */
			readback = *addr;
			if(readback != val) {
                BOOT_MSG1("\r\nFAILURE (data line): ", (uint)addr);
                BOOT_MSG1("found ", (unsigned long)readback);
                BOOT_MSG1("expected ", (unsigned long)val);
 			}
			*addr  = ~val;
			*dummy  = val;
			readback = *addr;
			if(readback != ~val) {
                BOOT_MSG1("\r\nFAILURE (data line): ", (uint)addr);
                BOOT_MSG1("\r\nfound ", (unsigned long)readback);
                BOOT_MSG1("\r\nexpected ", (unsigned long)(~val));
			}
		    }
		}

		/*
		 * Based on code whose Original Author and Copyright
		 * information follows: Copyright (c) 1998 by Michael
		 * Barr. This software is placed into the public
		 * domain and may be used for any purpose. However,
		 * this notice must not be changed or removed and no
		 * warranty is either expressed or implied by its
		 * publication or distribution.
		 */

		/*
		 * Address line test
		 *
		 * Description: Test the address bus wiring in a
		 *              memory region by performing a walking
		 *              1's test on the relevant bits of the
		 *              address and checking for aliasing.
		 *              This test will find single-bit
		 *              address failures such as stuck -high,
		 *              stuck-low, and shorted pins. The base
		 *              address and size of the region are
		 *              selected by the caller.
		 *
		 * Notes:	For best results, the selected base
		 *              address should have enough LSB 0's to
		 *              guarantee single address bit changes.
		 *              For example, to test a 64-Kbyte
		 *              region, select a base address on a
		 *              64-Kbyte boundary. Also, select the
		 *              region size as a power-of-two if at
		 *              all possible.
		 *
		 * Returns:     0 if the test succeeds, 1 if the test fails.
		 */
		
	//    printf ("Address line test \n");
		len = ((ulong)end - (ulong)start)/sizeof(vu_long);
		pattern = (vu_long) 0xaaaaaaaa;
		anti_pattern = (vu_long) 0x55555555;

		/*
		 * Write the default pattern at each of the
		 * power-of-two offsets.
		 */
		for (offset = 1; offset < len; offset <<= 1) {
			start[offset] = pattern;
		}

		/*
		 * Check for address bits stuck high.
		 */
		test_offset = 0;
		start[test_offset] = anti_pattern;

		for (offset = 1; offset < len; offset <<= 1) {
		    temp = start[offset];
		    if (temp != pattern) {
                BOOT_MSG1("\r\nFAILURE: Address bit stuck high: ", (ulong)&start[offset]);
                BOOT_MSG1("\r\nfound ", (unsigned long)temp);
                BOOT_MSG1("\r\nexpected ", (unsigned long)pattern);
			return 1;
		    }
		}
		start[test_offset] = pattern;

		/*
		 * Check for addr bits stuck low or shorted.
		 */
		for (test_offset = 1; test_offset < len; test_offset <<= 1) {
		    start[test_offset] = anti_pattern;

		    for (offset = 1; offset < len; offset <<= 1) {
			temp = start[offset];
			if ((temp != pattern) && (offset != test_offset)) {
                BOOT_MSG1("\r\nFAILURE: Address bit stuck low or shorted ", (ulong)&start[offset]);
                BOOT_MSG1("\r\nfound ", (unsigned long)temp);
                BOOT_MSG1("\r\nexpected ", (unsigned long)pattern);
			    return 1;
			}
		    }
		    start[test_offset] = pattern;
		}

		/*
		 * Description: Test the integrity of a physical
		 *		memory device by performing an
		 *		increment/decrement test over the
		 *		entire region. In the process every
		 *		storage bit in the device is tested
		 *		as a zero and a one. The base address
		 *		and the size of the region are
		 *		selected by the caller.
		 *
		 * Returns:     0 if the test succeeds, 1 if the test fails.
		 */
 
	    //printf ("Test the integrity\n");
		num_words = ((ulong)end - (ulong)start)/sizeof(vu_long) + 1;

		/*
		 * Fill memory with a known pattern.
		 */
		for (pattern = 1, offset = 0; offset < num_words; pattern++, offset++) {
			start[offset] = pattern;
		}

		/*
		 * Check each location and invert it for the second pass.
		 */
		for (pattern = 1, offset = 0; offset < num_words; pattern++, offset++) {
		    temp = start[offset];
		    if (temp != pattern) {
            BOOT_MSG1("\r\nFAILURE (read/write) ", (ulong)&start[offset]);
            BOOT_MSG1("\r\nfound ", (unsigned long)temp);
            BOOT_MSG1("\r\nexpected ", (unsigned long)pattern);
			return 1;
		    }

		    anti_pattern = ~pattern;
		    start[offset] = anti_pattern;
		}

		/*
		 * Check each location for the inverted pattern and zero it.
		 */
		for (pattern = 1, offset = 0; offset < num_words; pattern++, offset++) {
		    anti_pattern = ~pattern;
		    temp = start[offset];
		    if (temp != anti_pattern) {
            BOOT_MSG1("\r\nFAILURE (read/write) ", (ulong)&start[offset]);
            BOOT_MSG1("\r\nfound ", (unsigned long)temp);
            BOOT_MSG1("\r\nexpected ", (unsigned long)anti_pattern);
			return 1;
		    }
		    start[offset] = 0;
		}
	}

	return rcode;
}
#endif

#ifdef CMD_TEST_MALLOC
extern u32 g_boot_malloc_head;
extern u32 g_boot_malloc_end;
u8* g_malloc_addr;
int do_mal_mtest (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    u8 *u8Addr;
    static u32 ulCount = 1;
    u32 i;
    u32 ret;
    
    boot_malloc_init();

    Watchdog_Disable();
    //for(i=0;i<10;i++)
    switch (*argv[1])
    {
        case '0':
            BOOT_MSG1("\r\nhead: ", (unsigned long)g_boot_malloc_head);
            BOOT_MSG1("\r\nend: ", (unsigned long)g_boot_malloc_end);
            u8Addr = (u8 *)boot_malloc(0x400);
            BOOT_MSG1("\r\naddr: ", (unsigned long)u8Addr);
            u8Addr = (u8 *)boot_malloc(0x400*2);
            BOOT_MSG1("\r\naddr: ", (unsigned long)u8Addr);
            print_mem_info();         
            break;
        case '1':
            u8Addr = (u8 *)boot_malloc(0);
            g_malloc_addr = u8Addr;
            BOOT_MSG1("\r\naddr: ", (unsigned long)u8Addr);
            memset_s(u8Addr, 0x400*ulCount, 0x33, 0x400*ulCount);
            print_mem_info();         
            break;
        case '2':
            u8Addr = (u8 *)boot_malloc(0x400*8);
            u8Addr = (u8 *)boot_malloc(0x400*8);
            boot_free(u8Addr);
            u8Addr = (u8 *)boot_malloc(0x400*4);
            print_mem_info();   
            break;
        case '3':
            u8Addr = (u8 *)boot_malloc(0x400*4-3);
            u8Addr = (u8 *)boot_malloc(0x400*4+3);
            print_mem_info();              
            break;
        case '4':
            u8Addr = (u8 *)boot_malloc(0x400*8);
            g_malloc_addr = (u8 *)boot_malloc(0x400*4);
            u8Addr = (u8 *)boot_malloc(0x400*8);
            boot_free(g_malloc_addr);
            u8Addr = (u8 *)boot_malloc(0x400*8);
            u8Addr = (u8 *)boot_malloc(0x400*8);
            g_malloc_addr = (u8 *)boot_malloc(0x400*5);
            u8Addr = (u8 *)boot_malloc(0x400*6);
            boot_free(u8Addr);
            boot_free(g_malloc_addr);
            u8Addr = (u8 *)boot_malloc(0x400*8);
            u8Addr = (u8 *)boot_malloc(0x400*8);
            u8Addr = (u8 *)boot_malloc(0x400*5);
            boot_free(u8Addr);
            u8Addr = (u8 *)boot_malloc(0x400*5+3);
            boot_free(u8Addr);
             u8Addr = (u8 *)boot_malloc(0x400*5+7);
            boot_free(u8Addr);           
            for(;;)
            {
                u8Addr = (u8 *)boot_malloc(0x400*8);
                if (u8Addr == 0)
                    break;
            }
            print_mem_info();  
            break;
        case '5':
            boot_free(0x12345678);   
            print_mem_info();  
            break;
        case '6':
            u8Addr = (u8 *)boot_malloc(0x400);
            ret = boot_free(u8Addr);  
            BOOT_MSG1("\r\nret: ", (unsigned long)ret);
            ret = boot_free(u8Addr); 
            BOOT_MSG1("\r\nret: ", (unsigned long)ret);
            print_mem_info();  
            break;
        case '7':
            boot_free(0x70008000);   
            print_mem_info();  
            break;
        case '8':
            print_mem_info();  
            break;
        default:
            u8Addr = (u8 *)boot_malloc(0x400*ulCount);
            g_malloc_addr = u8Addr;
            BOOT_MSG1("\r\naddr: ", (unsigned long)u8Addr);
            memset_s(u8Addr, 0x400*ulCount, 0x33, 0x400*ulCount);
            ulCount ++;
            print_mem_info();
            break;
    }
    return 0;
}
int do_free_mtest (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    boot_free(g_malloc_addr);
    print_mem_info();
}
#endif

/**************************************************/
U_BOOT_CMD(
	md,	3,	1,	(void *)do_mem_md,
	"md [addr] [len] - memory display"//,
	//"[.b, .w, .l] address [# of objects]\n	  - memory display\n"
);

U_BOOT_CMD(
	mw,	4,	1,	(void *)do_mem_mw,
	"mw [addr] [val] - memory write"//,
	//"[.b, .w, .l] address value [count]\n	- write memory\n"
);

#ifdef CMD_TEST_MEM
U_BOOT_CMD(
	mtest,	3,	1,	do_mem_mtest,
	"mtest	- simple RAM test\n"
);
#endif

#ifdef CMD_TEST_MALLOC
U_BOOT_CMD(
	malloc,	3,	1,	do_mal_mtest,
	"malloc	- simple MALLOC test\n"
);
U_BOOT_CMD(
	free,	3,	1,	do_free_mtest,
	"free	- simple FREE test\n"
);
#endif
