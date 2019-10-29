/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * (C) Copyright 2000-2010
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * (C) Copyright 2001 Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Andreas Heppel <aheppel@sysgo.de>
 *
 * (C) Copyright 2008 Atmel Corporation
 */
#include <boot/boot.h>
#include <environment.h>
#include <spi_flash.h>
#include <global_data.h>
#include <debug.h>
#include "securec.h"

#define BOOT_ENV_ADDR (0x30000024)

/* SPI mode flags */
#define	SPI_CPHA	0x01			/* clock phase */
#define	SPI_CPOL	0x02			/* clock polarity */
#define	SPI_MODE_0	(0|0)			/* (original MicroWire) */
#define	SPI_MODE_1	(0|SPI_CPHA)
#define	SPI_MODE_2	(SPI_CPOL|0)
#define	SPI_MODE_3	(SPI_CPOL|SPI_CPHA)
#define	SPI_CS_HIGH	0x04			/* CS active high */
#define	SPI_LSB_FIRST	0x08			/* per-word bits-on-wire */
#define	SPI_3WIRE	0x10			/* SI/SO signals shared */
#define	SPI_LOOP	0x20			/* loopback mode */

/*lint -e683*/
#define printf
#define puts(s) print_msg0(s)
#define malloc xmalloc
#define free xfree
/*lint +e683*/

#ifndef CONFIG_ENV_SPI_BUS
# define CONFIG_ENV_SPI_BUS	0
#endif
#ifndef CONFIG_ENV_SPI_CS
# define CONFIG_ENV_SPI_CS	0
#endif
#ifndef CONFIG_ENV_SPI_MAX_HZ
# define CONFIG_ENV_SPI_MAX_HZ	1000000
#endif
#ifndef CONFIG_ENV_SPI_MODE
# define CONFIG_ENV_SPI_MODE	SPI_MODE_0
#endif


extern struct spi_flash *s_sfc_new;
extern  volatile gd_t *gd;


gd_t gdata;

//static struct spi_flash *s_sfc_new;

int saveenv(void)
{
	env_t *env_new = (env_t *)HI_NULL;
	char *res = (char *)HI_NULL;
	int	ret = 1;
	unsigned int env_new_offset;

	//print_msg0("L:SaveEnv");
	if (!s_sfc_new) {
		s_sfc_new = spi_flash_probe(CONFIG_ENV_SPI_BUS,
			CONFIG_ENV_SPI_CS,
			CONFIG_ENV_SPI_MAX_HZ, CONFIG_ENV_SPI_MODE);
		if (!s_sfc_new) {
			set_default_env("!spi_flash_probe() failed");
			return 1;
		}
	}

	env_new = (env_t*)malloc(sizeof(env_t));
	if (NULL == env_new)
		return 1;
/*
        【屏蔽人】z00193780
        【审核人】w00190206
        【代码分析】boot下memcpy、memset、strcpy替换为安全接口会增加2k bytes左右的代码,并且以上接口性能分别会降低一定比例，所以建议boot下不做安全接口替换。
        【功能影响】不影响正常业务，屏蔽告警
*/
	memset_s(env_new, sizeof(env_t),0xee, sizeof(env_t));//测试用
	res = (char *)(env_new->data);
	memcpy_s(res, sizeof(gd->env_data), (char*)(&gd->env_data), sizeof(gd->env_data));

	//id 递增以区分最新env
	//print_msg1("id before", ((HI_ENV_DATA_ST*)res)->id);
	((HI_ENV_DATA_ST*)res)->id ++;
	//同时刷新全局变量
	gd->env_data.valid_data.id = ((HI_ENV_DATA_ST*)res)->id;
	//print_msg1("id after", ((HI_ENV_DATA_ST*)res)->id);
	
	env_new->crc = crc32(0, (char *)env_new->data, (unsigned long)ENV_SIZE);
	//print_msg2("L:crc:", (unsigned long)env_new->crc, (unsigned long)ENV_SIZE);

	if (gd->env_valid == 1) {
		env_new_offset = gd->env_addr2;
	} else {
		env_new_offset = gd->env_addr1;
	}

	ret = spi_flash_erase(s_sfc_new, env_new_offset, CONFIG_ENV_SECT_SIZE);
	if (ret)
		goto done;

	ret = spi_flash_write(s_sfc_new, env_new_offset, CONFIG_ENV_SIZE, env_new);
	if (ret)
		goto done;

	gd->env_valid = gd->env_valid == 2 ? 1 : 2;

	//print_msg1((char *)"Valid env:", (unsigned long)gd->env_valid);

 done:
	if (env_new)
		free(env_new);

	return ret;
}

void env_relocate_spec(void)
{
	int ret;
	int crc1_ok = 0, crc2_ok = 0;
	env_t *tmp_env1 = (env_t *)HI_NULL;
	env_t *tmp_env2 = (env_t *)HI_NULL;
	env_t *ep = (env_t *)HI_NULL;
	//unsigned int id_back=0;

	tmp_env1 = (env_t *)malloc(CONFIG_ENV_SIZE);
	tmp_env2 = (env_t *)malloc(CONFIG_ENV_SIZE);

	if (!tmp_env1 || !tmp_env2) {
		set_default_env("!malloc() failed");
		goto out;
	}

	if (!s_sfc_new) {
		s_sfc_new = spi_flash_probe(CONFIG_ENV_SPI_BUS,
			CONFIG_ENV_SPI_CS,
			CONFIG_ENV_SPI_MAX_HZ, CONFIG_ENV_SPI_MODE);
		if (!s_sfc_new) {
			set_default_env("!spi_flash_probe() failed");
			goto out;
		}
	}

	//print_msg2("L:gd->env_addr1 & addr2", (unsigned long)gd->env_addr1, (unsigned long)gd->env_addr2);
	ret = spi_flash_read(s_sfc_new, gd->env_addr1, CONFIG_ENV_SIZE, tmp_env1);
    //print_msg1("spi_flash_read", (unsigned long)ret);
	if (ret) {
		set_default_env("!spi_flash_read() failed");
		goto err_read;
	}
	//print_msg1("crc begin ", (unsigned long)ret);
	if (crc32(0, (char *)tmp_env1->data, ENV_SIZE) == tmp_env1->crc)
		crc1_ok = 1;
	//print_msg1("crc end ", (unsigned long)ret);
	ret = spi_flash_read(s_sfc_new, gd->env_addr2, CONFIG_ENV_SIZE, tmp_env2);
    //print_msg1("spi_flash_read ", (unsigned long)ret);
	if (!ret) {
		if (crc32(0, (char *)tmp_env2->data, ENV_SIZE) == tmp_env2->crc)
			crc2_ok = 1;
	}
	//print_msg2("L:crc:", (unsigned long)crc1_ok, (unsigned long)crc2_ok);
	if (!crc1_ok && !crc2_ok) {
		//print_msg2("!crc:", (unsigned long)tmp_env1->crc, (unsigned long)tmp_env2->crc);
		set_default_env("!!bad CRC");
		//第一次上电存储
		saveenv();
		goto err_read;
	} else if (crc1_ok && !crc2_ok) {
		gd->env_valid = 1;
	} else if (!crc1_ok && crc2_ok) {
		gd->env_valid = 2;
	} else if (((HI_ENV_DATA_ST*)(tmp_env1->data))->id > ((HI_ENV_DATA_ST*)(tmp_env2->data))->id) {
		gd->env_valid = 1;
	} else if (((HI_ENV_DATA_ST*)(tmp_env1->data))->id == ((HI_ENV_DATA_ST*)(tmp_env2->data))->id){
		//print_msg1("!equl id =", (unsigned long)((HI_ENV_DATA_ST*)(tmp_env1->data))->id);
		gd->env_valid = 2;
	}
	else {
		gd->env_valid = 2;
	}

	if (gd->env_valid == 1)
		ep = tmp_env1;
	else
		ep = tmp_env2;
	//print_msg1("L:read size:", (unsigned long)sizeof(gd->env_data));
	//id_back = gd->env_data.valid_data.id;
/*
        【屏蔽人】z00193780
        【审核人】w00190206
        【代码分析】boot下memcpy、memset、strcpy替换为安全接口会增加2k bytes左右的代码,并且以上接口性能分别会降低一定比例，所以建议boot下不做安全接口替换。
        【功能影响】不影响正常业务，屏蔽告警
*/
	memcpy_s((char*)(&(gd->env_data)), sizeof(gd->env_data), (char *)ep->data, sizeof(gd->env_data));
	//gd->env_data.valid_data.id = id_back;

err_read:
	//spi_flash_free(s_sfc_new);
	//s_sfc_new = NULL;
out:
	free(tmp_env2);
	free(tmp_env1);
}

int env_init(void)
{
	unsigned int reg_val = 0;
    unsigned int reg_val2 = 0;
    unsigned int i = 0;
	
	gd = &gdata;
	for (i=0; i<4; i++)
	{
		reg_val = *((unsigned int*)BOOT_ENV_ADDR+i);
		//print_msg1("reg_val:", (unsigned long)reg_val);
		reg_val2 = *((unsigned int*)BOOT_ENV_ADDR+i+1);
		if(reg_val2 == 0xFFFFFFFF)
			break;
	}	
	gd->env_addr1 = reg_val;
	gd->env_addr2 = reg_val + CONFIG_ENV_SIZE;//64K兼容必须换boot，此处写固定值即可
	gd->env_valid = 1;

	return 0;
}
