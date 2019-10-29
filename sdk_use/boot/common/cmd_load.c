/*
 * (C) Copyright 2000-2004
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
 * Serial up- and download support
 */
#include <command.h>
#include <xyzModem.h>
#include <types.h>
#include <product.h>
#include <watchdog.h>
#include <debug.h>
#include <boot.h>
#include "securec.h"

/* whether support loadb command */
//#define CONFIG_CMD_LOADB

/*lint -e683*/
#define malloc xmalloc
#define free xfree
/*lint +e683*/

static u32 load_serial_ymodem (u32 offset);
/* BEGIN: Added by w00190206, 2013/4/25   PN:DTS2013041705513*/
extern u32 g_load_file_length;
/* END:   Modified by w00190206, 2013/4/25 */
/* -------------------------------------------------------------------- */

int do_load_serial_bin (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	u32 offset = 0;
	u32 addr;
	int rcode = 0;

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
    disable_watchdog();
	/* pre-set offset from CFG_LOAD_ADDR */
	offset = CFG_LOAD_ADDR;

	if (argc == 2) {
		offset = simple_strtoul(argv[1], (char **)HI_NULL, 16);
	}

	if (strcmp(argv[0],"loady")==0) {
		print_msg1("Ready for bin (ymodem) download to",(unsigned long)offset);

		addr = load_serial_ymodem (offset);
		/* BEGIN: Added by w00190206, 2013/4/25   PN:DTS2013041705513*/
		#ifdef TURNOFF_NEWFUNCTION_BEFORE_B020
			printf ("## Start Addr      = 0x%x\n", addr);
		#else
		if (addr == (u32)(~0)) 
		{
			print_msg0("Download abort");
			rcode = 1;
		} 
		else 
		{
			print_msg1 ("Start = ", (unsigned long)addr);
		}
		#endif
		/* END:   Added by w00190206, 2013/4/25 */
	} 
	return rcode;
}

static int getcxmodem(void) {
	if (serial_tstc())
		return (getc());
	return -1;
}
 u32 load_serial_ymodem (u32 offset)
{
	int size = 0;
	int err = 0;
	int res;
	connection_info_t info;
	char *ymodemBuf = (char *)malloc(1024);
	ulong store_addr = ~0;
	ulong addr = 0;

	info.mode = xyzModem_ymodem;
	//while(1) {};
	res = xyzModem_stream_open (&info, &err);
	if ((!res)&&(ymodemBuf))
	{
		while ((res = xyzModem_stream_read (ymodemBuf, 1024, &err)) > 0) 
		{
			store_addr = addr + offset;
			size += res;
			addr += (ulong)res;
/*
							【屏蔽人】z00193780
							【审核人】w00190206
							【代码分析】boot下memcpy、memset、strcpy替换为安全接口会增加2k bytes左右的代码,并且以上接口性能分别会降低一定比例，所以建议boot下不做安全接口替换。
							【功能影响】不影响正常业务，屏蔽告警
*/
			memcpy_s((char *) (store_addr), (unsigned int)res, ymodemBuf,(unsigned int)res);
		}
	} 
	else 
	{
		print_msg0(xyzModem_error (err));
	}

    free(ymodemBuf);
	xyzModem_stream_close (&err);
    /*lint -e546*/
	xyzModem_stream_terminate (false, &getcxmodem);
    /*lint +e546*/
	//clean_dcache();
	//flush_cache (offset, size);

	print_msg1("\r\nTotal Size = ", (unsigned long)size);

	enable_watchdog();

#ifdef TURNOFF_NEWFUNCTION_BEFORE_B020
	g_load_file_length = size;

/* BEGIN: Added by w00190206, 2013/4/25   PN:DTS2013041705513*/
#else
	//增加对中途停止的判断
	if (size != (int)g_load_file_length)
	{
		//返回错误，上层处理
		print_msg2("Loadsize != filesize:", (unsigned long)size, (unsigned long)g_load_file_length);
		return (~0);
	}
#endif
/* END:   Added by w00190206, 2013/4/25 */

	return offset;
}

U_BOOT_CMD(
	loady, 3, 0,	(void *)do_load_serial_bin,
	"loady [addr]    - load bin file to addr by ymodem"
);
