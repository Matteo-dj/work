/*
 * (C) Copyright 2000-2010
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * (C) Copyright 2001 Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Andreas Heppel <aheppel@sysgo.de>
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <environment.h>
#include <types.h>
#include <global_data.h>
#include <boot.h>
#include <debug.h>
#include <environment.h>
#include "securec.h"

/*lint -e683*/
#define printf
#define puts(s) print_msg0(s)
/*lint +e683*/

extern  volatile gd_t *gd;


/************************************************************************
 * Default settings to be used when no valid environment is found
 */
#include <env_default.h>

void set_default_env(const char *s)
{
	if (s) {
		if (*s != '!') {
			puts("Err:");
		} 
		//puts(s);
	} else {
		puts("Using default environment\n\n");
	}
/*
        �������ˡ�z00193780
        ������ˡ�w00190206
        �����������boot��memcpy��memset��strcpy�滻Ϊ��ȫ�ӿڻ�����2k bytes���ҵĴ���,�������Ͻӿ����ֱܷ�ή��һ�����������Խ���boot�²�����ȫ�ӿ��滻��
        ������Ӱ�졿��Ӱ������ҵ�����θ澯
*/
	memset_s((void*)&(gd->env_data), ENV_SIZE, 0xff, ENV_SIZE);
	memcpy_s((void*)&(gd->env_data.valid_data), sizeof(HI_ENV_DATA_ST), (char *)&default_environment, sizeof(HI_ENV_DATA_ST));
}

void env_relocate(void)
{
#if defined(CONFIG_NEEDS_MANUAL_RELOC)
	env_reloc();
	//env_htab.change_ok += gd->reloc_off;
#endif
	if (gd->env_valid == 0) {
#if defined(CONFIG_ENV_IS_NOWHERE) || defined(CONFIG_SPL_BUILD)
		/* Environment not changable */
		set_default_env(NULL);
#else
		//bootstage_error(BOOTSTAGE_ID_NET_CHECKSUM);
		set_default_env("!bad CRC");
#endif
	} else {
		//print_msg0("env_relocate");
		env_relocate_spec();
	}
}
