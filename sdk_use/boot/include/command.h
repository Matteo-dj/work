/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * (C) Copyright 2000-2009
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 */

/*
 *  Definitions for Command Processor
 */
#ifndef __COMMAND_H
#define __COMMAND_H

#include <linker_lists.h>
#include <types.h>

#define BSP_CODEX_CHECK_CLOSE

#ifndef NULL
#define NULL	0
#endif

/* Default to a width of 8 characters for help message command width */
#ifndef CONFIG_SYS_HELP_CMD_WIDTH
#define CONFIG_SYS_HELP_CMD_WIDTH	8
#endif

#ifndef	__ASSEMBLY__
/*
 * Monitor Command Table
 */

struct cmd_tbl_s {
	char		*name;		/* Command Name			*/
	int		maxargs;	/* maximum number of arguments	*/
	int		repeatable;	/* autorepeat allowed?		*/
					/* Implementation function	*/
	int		(*cmd)(struct cmd_tbl_s *, int, int, const char * const []);
	char		*usage;		/* Usage message	(short)	*/
#ifdef	CONFIG_SYS_LONGHELP
	char		*help;		/* Help  message	(long)	*/
#endif
#ifdef CONFIG_AUTO_COMPLETE
	/* do auto completion on the arguments */
	int		(*complete)(int argc, char * const argv[], char last_char, int maxv, char *cmdv[]);
#endif
};

typedef struct cmd_tbl_s	cmd_tbl_t;


#if defined(CONFIG_CMD_RUN)
extern int do_run(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
#endif

/* common/command.c */
int _do_help (cmd_tbl_t *cmd_start, int cmd_items, cmd_tbl_t * cmdtp, int
	      flag, int argc, char * const argv[]);
cmd_tbl_t *find_cmd(const char *cmd);
cmd_tbl_t *find_cmd_tbl (const char *cmd, cmd_tbl_t *table, int table_len);

extern int cmd_usage(const cmd_tbl_t *cmdtp);

#ifdef CONFIG_AUTO_COMPLETE
extern int var_complete(int argc, char * const argv[], char last_char, int maxv, char *cmdv[]);
extern int cmd_auto_complete(const char *const prompt, char *buf, int *np, int *colp);
#endif

/**
 * cmd_process_error() - report and process a possible error
 *
 * @cmdtp: Command which caused the error
 * @err: Error code (0 if none, -ve for error, like -EIO)
 * @return 0 if there is not error, 1 (CMD_RET_FAILURE) if an error is found
 */
int cmd_process_error(cmd_tbl_t *cmdtp, int err);

/*
 * Monitor Command
 *
 * All commands use a common argument format:
 *
 * void function (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
 */

#if defined(CONFIG_CMD_MEMORY)		\
	|| defined(CONFIG_CMD_I2C)	\
	|| defined(CONFIG_CMD_ITEST)	\
	|| defined(CONFIG_CMD_PCI)	\
	|| defined(CONFIG_CMD_PORTIO)
#define CMD_DATA_SIZE
extern int cmd_get_data_size(const char* arg, int default_size);
#endif

#ifdef CONFIG_CMD_BOOTD
extern int do_bootd(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
#endif
#ifdef CONFIG_CMD_BOOTM
extern int do_bootm(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
extern int bootm_maybe_autostart(cmd_tbl_t *cmdtp, const char *cmd);
#else
static inline int bootm_maybe_autostart(cmd_tbl_t *cmdtp, const char *cmd)
{
    HI_UNREF_PARAM(cmdtp);
    HI_UNREF_PARAM(cmd);
	return 0;
}
#endif

extern int do_bootz(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);

extern int do_booti(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);

extern int common_diskboot(cmd_tbl_t *cmdtp, const char *intf, int argc,
			   char *const argv[]);

extern int do_reset(cmd_tbl_t *cmdtp, int flag, int argc, const char * const argv[]);
extern int do_poweroff(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);

/*
 * Error codes that commands return to cmd_process(). We use the standard 0
 * and 1 for success and failure, but add one more case - failure with a
 * request to call cmd_usage(). But the cmd_process() function handles
 * CMD_RET_USAGE itself and after calling cmd_usage() it will return 1.
 * This is just a convenience for commands to avoid them having to call
 * cmd_usage() all over the place.
 */
enum command_ret_t {
	CMD_RET_SUCCESS,	/* 0 = Success */
	CMD_RET_FAILURE,	/* 1 = Failure */
	CMD_RET_USAGE = -1,	/* Failure, please report 'usage' error */
};

/**
 * Process a command with arguments. We look up the command and execute it
 * if valid. Otherwise we print a usage message.
 *
 * @param flag		Some flags normally 0 (see CMD_FLAG_.. above)
 * @param argc		Number of arguments (arg 0 must be the command text)
 * @param argv		Arguments
 * @param repeatable	This function sets this to 0 if the command is not
 *			repeatable. If the command is repeatable, the value
 *			is left unchanged.
 * @param ticks		If ticks is not null, this function set it to the
 *			number of ticks the command took to complete.
 * @return 0 if the command succeeded, 1 if it failed
 */
int cmd_process(int flag, int argc, char * const argv[],
			       int *repeatable, unsigned long *ticks);

void fixup_cmdtable(cmd_tbl_t *cmdtp, int size);

/**
 * board_run_command() - Fallback function to execute a command
 *
 * When no command line features are enabled in U-Boot, this function is
 * called to execute a command. Typically the function can look at the
 * command and perform a few very specific tasks, such as booting the
 * system in a particular way.
 *
 * This function is only used when CONFIG_CMDLINE is not enabled.
 *
 * In normal situations this function should not return, since U-Boot will
 * simply hang.
 *
 * @cmdline:	Command line string to execute
 * @return 0 if OK, 1 for error
 */
int board_run_command(const char *cmdline);
#endif	/* __ASSEMBLY__ */

#undef puts
#define puts serial_puts
#undef putc
#define putc serial_putc
#undef getc
#define getc serial_getc
#undef tstc
#define tstc serial_tstc
/*
 * Command Flags:
 */
#define CMD_FLAG_REPEAT		0x0001	/* repeat last command		*/
#define CMD_FLAG_BOOTD		0x0002	/* command is from bootd	*/
#define CMD_FLAG_ENV		0x0004	/* command is from the environment */

#define CFG_CBSIZE 512
#define CFG_PROMPT      "SmartChip # "          /* Monitor Command Prompt   */
#define CFG_MAXARGS     16                      /* max number of command args   */
#define CFG_REQUIRE_PASSWORD "Input password\r\n"

#define U_BOOT_CMD_MKENT_COMPLETE(_name, _maxargs, _rep, _cmd, _usage, _comp) \
		{ #_name, _maxargs, _rep, _cmd, _usage}

#define U_BOOT_CMD_COMPLETE(_name, _maxargs, _rep, _cmd, _usage, _comp) \
	ll_entry_declare(cmd_tbl_t, _name, cmd) =			\
		U_BOOT_CMD_MKENT_COMPLETE(_name, _maxargs, _rep, _cmd, _usage, _comp)

#define U_BOOT_CMD(_name, _maxargs, _rep, _cmd, _usage)		\
	U_BOOT_CMD_COMPLETE(_name, _maxargs, _rep, _cmd, _usage, NULL)

#define U_BOOT_CMD_MKENT(_name, _maxargs, _rep, _cmd, _usage)	\
	U_BOOT_CMD_MKENT_COMPLETE(_name, _maxargs, _rep, _cmd, _usage, NULL)
#ifdef BSP_CODEX_CHECK_CLOSE
extern volatile u32    g_manage_authority;
int input_password(void);
#endif
void boot_regist_cmd(void);
unsigned long simple_strtoul(const char *cp,char **endp,unsigned int base);

#endif	/* __COMMAND_H */
