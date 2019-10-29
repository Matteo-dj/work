/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * (C) Copyright 2002
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 */

#ifndef _ENVIRONMENT_H_
#define _ENVIRONMENT_H_

#include <types.h>
/**************************************************************************
 *
 * The "environment" is stored as a list of '\0' terminated
 * "name=value" strings. The end of the list is marked by a double
 * '\0'. New entries are always added at the end. Deleting an entry
 * shifts the remaining entries to the front. Replacing an entry is a
 * combination of deleting the old value and adding the new one.
 *
 * The environment is preceded by a 32 bit CRC over the data part.
 *
 *************************************************************************/
#define CONFIG_ENV_IS_IN_FLASH
#define CONFIG_SYS_FLASH_BASE 0
#define CONFIG_ENV_ADDR 0xA000
#define CONFIG_ENV_SECT_SIZE 0x1000

#define CONFIG_ENV_OFFSET_REDUND	(CONFIG_ENV_OFFSET + CONFIG_ENV_SIZE)
#define CONFIG_ENV_SIZE_REDUND	CONFIG_ENV_SIZE

//GD_FLG_ENV_READY defined in uboot global_data_flags.h
#define GD_FLG_ENV_READY	0x0080	/* Environment imported into hash table */

#if defined(CONFIG_ENV_IS_IN_FLASH)
# ifndef	CONFIG_ENV_ADDR
#  define	CONFIG_ENV_ADDR	(CONFIG_SYS_FLASH_BASE + CONFIG_ENV_OFFSET)
# endif
# ifndef	CONFIG_ENV_OFFSET
#  define	CONFIG_ENV_OFFSET (CONFIG_ENV_ADDR - CONFIG_SYS_FLASH_BASE)
# endif
# if !defined(CONFIG_ENV_ADDR_REDUND) && defined(CONFIG_ENV_OFFSET_REDUND)
#  define	CONFIG_ENV_ADDR_REDUND	\
		(CONFIG_SYS_FLASH_BASE + CONFIG_ENV_OFFSET_REDUND)
# endif
# if defined(CONFIG_ENV_SECT_SIZE) || defined(CONFIG_ENV_SIZE)
#  ifndef	CONFIG_ENV_SECT_SIZE
#   define	CONFIG_ENV_SECT_SIZE	CONFIG_ENV_SIZE
#  endif
#  ifndef	CONFIG_ENV_SIZE
#   define	CONFIG_ENV_SIZE	CONFIG_ENV_SECT_SIZE
#  endif
# else
#  error "Both CONFIG_ENV_SECT_SIZE and CONFIG_ENV_SIZE undefined"
# endif
# if defined(CONFIG_ENV_ADDR_REDUND) && !defined(CONFIG_ENV_SIZE_REDUND)
#  define CONFIG_ENV_SIZE_REDUND	CONFIG_ENV_SIZE
# endif
# if	(CONFIG_ENV_ADDR >= CONFIG_SYS_MONITOR_BASE) &&		\
	(CONFIG_ENV_ADDR + CONFIG_ENV_SIZE) <=			\
	(CONFIG_SYS_MONITOR_BASE + CONFIG_SYS_MONITOR_LEN)
#  define ENV_IS_EMBEDDED
# endif
# if defined(CONFIG_ENV_ADDR_REDUND) || defined(CONFIG_ENV_OFFSET_REDUND)
#  define CONFIG_SYS_REDUNDAND_ENVIRONMENT
# endif
# ifdef CONFIG_ENV_IS_EMBEDDED
#  error "do not define CONFIG_ENV_IS_EMBEDDED in your board config"
#  error "it is calculated automatically for you"
# endif
#endif	/* CONFIG_ENV_IS_IN_FLASH */



#ifdef CONFIG_SYS_REDUNDAND_ENVIRONMENT
# define ENV_HEADER_SIZE	(sizeof(unsigned int) )

# define ACTIVE_FLAG   1
# define OBSOLETE_FLAG 0
#else
# define ENV_HEADER_SIZE	(sizeof(unsigned int))
#endif

#if defined(CONFIG_CMD_SAVEENV) && !defined(CONFIG_ENV_IS_NOWHERE)
extern char *env_name_spec;
#endif

#define ENV_SIZE (CONFIG_ENV_SIZE - ENV_HEADER_SIZE)

typedef struct environment_s {
	unsigned int	crc;		/* CRC32 over data bytes	*/
	unsigned char	data[ENV_SIZE]; /* Environment data		*/
} env_t;

#define ENV_DECOMPRESS_NV 0x55

//启动升级相关环境变量结构体
typedef struct
{
   unsigned int	id;
   unsigned int magic;
   unsigned int addr_start;		//启动地址
   unsigned int addr_backup;	//备份区地址
   unsigned int env1;			//环境变量区地址1(双备份区1)
   unsigned int env2;			//环境变量区地址2(双备份区2)
   unsigned short mode;			//模式：0.正常 1.升级（该模式下统计重启次数）
   unsigned short ver;			//预留版本号
   unsigned int reset_cnt;		//mode=升级模式下，重启次数记录
   unsigned int cnt_max;		//mode=升级模式下，重启次数上限，超过则切换addr_backup执行
   unsigned char load_direct;	//是否直接从启动地址引导
   unsigned char file_type;		//升级文件类型，boot下未用，该字节占用由kernel判断
   unsigned char ucDecompressNv;//解压NV标志
   unsigned char reserve[1];
   unsigned int pad[4];
}HI_ENV_DATA_ST;

typedef struct
{
   HI_ENV_DATA_ST valid_data;
   unsigned char reserve[ENV_SIZE-sizeof(HI_ENV_DATA_ST)];
}HI_ENV_DATA_EXTEND_ST;

extern const HI_ENV_DATA_ST default_environment;
extern env_t *env_ptr;
extern int env_init(void);
extern void env_relocate(void);
extern int saveenv(void);
extern void env_relocate_spec(void);
extern unsigned char env_get_char_spec(int);

#if defined(CONFIG_NEEDS_MANUAL_RELOC)
extern void env_reloc(void);
#endif

#ifndef DO_DEPS_ONLY

/* Function that updates CRC of the enironment */
void env_crc_update(void);

/* [re]set to the default environment */
void set_default_env(const char *s);

/* Import from binary representation into hash table */
int env_import(const char *buf, int check);

#endif /* DO_DEPS_ONLY */

#endif /* _ENVIRONMENT_H_ */
