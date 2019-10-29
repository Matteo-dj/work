/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * (C) Copyright 2008-2011
 * Graeme Russ, <graeme.russ@gmail.com>
 *
 * (C) Copyright 2002
 * Daniel Engstr枚m, Omicron Ceti AB, <daniel@omicron.se>
 *
 * (C) Copyright 2002
 * Wolfgang Denk, DENX Software Engineering, <wd@denx.de>
 *
 * (C) Copyright 2002
 * Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Marius Groeger <mgroeger@sysgo.de>
 *
 * Copyright 2015 ATS Advanced Telematics Systems GmbH
 * Copyright 2015 Konsulko Group, Matt Porter <mporter@konsulko.com>
 */

//#include <boot/ctype.h>
#include <types.h>
#include <command.h>
#include <debug.h>
#include <boot.h>
#include <watchdog.h>

#define TRY_TIMES 5

extern struct spi_flash *s_sfc_new;
#ifdef BSP_CODEX_CHECK_CLOSE
const static char s_password[10] = "DBKCYGD.";
#endif
/*
 * ARMv7M does not support ARM instruction mode. However, the
 * interworking BLX and BX instructions do encode the ARM/Thumb
 * field in bit 0. This means that when executing any Branch
 * and eXchange instruction we must set bit 0 to one to guarantee
 * that we keep the processor in Thumb instruction mode. From The
 * ARMv7-M Instruction Set A4.1.1:
 *   "ARMv7-M only supports the Thumb instruction execution state,
 *    therefore the value of address bit [0] must be 1 in interworking
 *    instructions, otherwise a fault occurs."
 */
unsigned long do_go_exec(u32 (*entry)(int, const char * const []),
			 int argc, const char * const argv[])
{
	return entry(argc, argv);
}

typedef unsigned long (*pfn_entry)(int, char *[]);
#ifdef BSP_CODEX_CHECK_CLOSE
int input_password(void)
{
    u32 i = 0;
	u32 j = 0;
    s8 *p = HI_NULL;
    s8 ch = 0;
	s8 match;
    for (i=0;i<TRY_TIMES;i++)
    {
        watchdog_feed();
        serial_puts(":");
        p = (s8 *)s_password;
        j = 0;
		match = 1;
        for(;;)
        {
		    if (serial_tstc()) 
		    {
	            ch = serial_getc();
                serial_putc ('*');
                if (ch == '\n')
                {
	                break;
                }
				else
                {
	                if(ch == *(p+j))
                    {
                        if(match && ch == '.')
                        {
                            g_manage_authority = TRUE;
                            serial_puts(" success\r\n");
                            return 0;
                        }
                    }
					else
					{
						match = 0;
					}
                    j++;
                    if(j == 10)
                    {
                        break;
                    }
                }
		    }
        }
    }
    //达到最大次数后认为失败重启
    //watchdog_enable(10);
    serial_puts(" fail\r\n");
	
    return -1;
}
#endif
static int do_go(cmd_tbl_t *cmdtp, int flag, int argc, const char * const argv[])
{
	u32	addr, rc;
	int     rcode = 0;

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
	if (argc < 2)
		return CMD_RET_USAGE;

	addr = simple_strtoul(argv[1], (char **)NULL, 16);
#ifdef BSP_CODEX_CHECK_CLOSE
	print_msg1 ("## Starting application at 0x %X ...\n", (unsigned long)addr);
#endif
	/*
	 * pass address parameter as argv[0] (aka command name),
	 * and all remaining args
	 */
	rc = do_go_exec ((void *)addr, argc - 1, argv + 1);
	if (rc != 0) rcode = 1;
#ifdef BSP_CODEX_CHECK_CLOSE
	print_msg1 ("## Application terminated, rc = 0x %X\n", rc);
#endif
	return rcode;
}
int do_reset(cmd_tbl_t *cmdtp, int flag, int argc, const char * const argv[])
{
    HI_UNREF_PARAM(cmdtp);
    HI_UNREF_PARAM(flag);
    HI_UNREF_PARAM(argc);
    HI_UNREF_PARAM(argv);

    watchdog_enable(10);
    mdelay(1000);
    return 0;
}

#ifdef BSP_CODEX_CHECK_CLOSE
int do_getauthority(cmd_tbl_t *cmdtp, int flag, int argc,const char * const argv[])
{
	return input_password();
}
#endif
/* -------------------------------------------------------------------- */
U_BOOT_CMD(
	go, CFG_MAXARGS, 1,	do_go,
	"go    [addr]    - start application at address"
);
U_BOOT_CMD(
	reset, 1, 0,	do_reset,
	"reset           - Perform RESET of the CPU"
);

#ifdef BSP_CODEX_CHECK_CLOSE
U_BOOT_CMD(
	password, 1, 0,	do_getauthority,
	""
);
#endif

