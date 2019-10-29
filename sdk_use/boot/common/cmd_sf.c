/*
 * Command for accessing SPI flash.
 *
 * Copyright (C) 2008 Atmel Corporation
 * Licensed under the GPL-2 or later.
 */
#include <types.h>
#include <command.h>
#include <spi_flash.h>
#include <product.h>
#include <debug.h>
#include <boot.h>
#include <watchdog.h>
#include "securec.h"

extern struct spi_flash *s_sfc_new;
static struct spi_flash *flash;
extern u32 g_load_file_length;


static int do_spi_flash_probe(int argc, char *argv[])
{
	unsigned int bus = 0;
	unsigned int cs;
	unsigned int speed = 0;
	unsigned int mode = 0;
	char *endp = (char *)HI_NULL;
	struct spi_flash *newFlash;

	if (argc != 2)
		goto usage;

	cs = simple_strtoul(argv[1], &endp, 0);
	if (*argv[1] == 0 || (*endp != 0 && *endp != ':'))
		goto usage;
	if (*endp == ':') {
		if (endp[1] == 0)
			goto usage;

		bus = cs;
		cs = simple_strtoul(endp + 1, &endp, 0);
		if (*endp != 0)
			goto usage;
	}
/*
	if (argc >= 3) {
		speed = simple_strtoul(argv[2], &endp, 0);
		if (*argv[2] == 0 || *endp != 0)
			goto usage;
	}
	if (argc >= 4) {
		mode = simple_strtoul(argv[3], &endp, 0);
		if (*argv[3] == 0 || *endp != 0)
			goto usage;
	}
*/
	newFlash = spi_flash_probe(bus, cs, speed, mode);
	if (!newFlash) {
		print_msg0("Flash Probe Fail.\n");
		return 1;
	}

//	if (flash)
//		spi_flash_free(flash);
	flash = newFlash;

	return 0;

usage:
	return 1;
}

static int do_spi_flash_read_write(int argc, char *argv[])
{
	unsigned long addr;
	unsigned long offset;
	unsigned long len;
	void *buf = HI_NULL;
	char *endp = (char *)HI_NULL;
	int ret;

	if (argc < 4)
		goto usage;

	addr = simple_strtoul(argv[1], &endp, 16);
	if (*argv[1] == 0 || *endp != 0)
		goto usage;
	offset = simple_strtoul(argv[2], &endp, 16);
	if (*argv[2] == 0 || *endp != 0)
		goto usage;
	len = simple_strtoul(argv[3], &endp, 16);
	if (*argv[3] == 0 || *endp != 0)
		goto usage;

	//buf = map_physmem(addr, len, MAP_WRBACK);
	buf = (void *)addr;
	if (!buf) {
		puts("Failed to map physical memory\n");
		return 1;
	}

	if (strcmp(argv[0], "read") == 0)
		ret = spi_flash_read(flash, offset, len, buf);
	else
		ret = spi_flash_write(flash, offset, len, buf);

	//unmap_physmem(buf, len);

	if (ret) {
		//printf("SPI flash %s failed\n", argv[0]);
		print_msg0("fail");
		return 1;
	}

	return 0;

usage:
	return 1;
}

static int do_spi_flash_erase(int argc, char *argv[])
{
	unsigned long offset;
	unsigned long len;
	char *endp = (char *)HI_NULL;
	int ret;

	if (argc < 3)
		goto usage;

	offset = simple_strtoul(argv[1], &endp, 16);
	if (*argv[1] == 0 || *endp != 0)
		goto usage;
	len = simple_strtoul(argv[2], &endp, 16);
	if (*argv[2] == 0 || *endp != 0)
		goto usage;

	ret = spi_flash_erase(flash, offset, len);
	if (ret) {
		//printf("SPI flash %s failed\n", argv[0]);
		print_msg0("fail");
		return 1;
	}

	return 0;

usage:
	return 1;
}

static int do_spi_flash(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	const char *cmd = HI_NULL;

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
	/* need at least two arguments */
	if (argc < 2)
		goto usage;

	cmd = argv[1];

	if (strcmp(cmd, "probe") == 0)
		return do_spi_flash_probe(argc - 1, argv + 1);

	/* The remaining commands require a selected device */
	if (!flash) {
		print_msg0("Run`sf probe 0' First");
		return 1;
	}

	if (strcmp(cmd, "read") == 0 || strcmp(cmd, "write") == 0)
		return do_spi_flash_read_write(argc - 1, argv + 1);
	if (strcmp(cmd, "erase") == 0)
		return do_spi_flash_erase(argc - 1, argv + 1);

usage:
	return 1;
}

#define BURN_FILE_HUPG 1
#define BURN_FILE_HNV   2
#define BURN_FILE_HBIN  3
#define BURN_FILE_WRONG 4
int do_burnhupg(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	unsigned int ret = 0;
	unsigned int upg_type = 0;
	unsigned int data_size = 0;

    HI_UNREF_PARAM(cmdtp);
    HI_UNREF_PARAM(argv);

#ifdef BSP_CODEX_CHECK_CLOSE    
    if (g_manage_authority != TRUE)
    {
    	serial_puts(CFG_REQUIRE_PASSWORD);
        if(input_password())
            return -1;
    }
#endif
	if (argc > 2)
	{
		print_msg1("!arc:",(unsigned long)argc);
	goto fail;
	}

	disable_watchdog(); 
	//清理内存并装载
/*
		【屏蔽人】z00193780
		【审核人】w00190206
		【代码分析】boot下memcpy、memset、strcpy替换为安全接口会增加2k bytes左右的代码,并且以上接口性能分别会降低一定比例，所以建议boot下不做安全接口替换。
		【功能影响】不影响正常业务，屏蔽告警
*/
	memset_s ((u8*)CFG_LOAD_ADDR, BOOT_AREA_SIZE + FILE_AREA_SIZE, 0xff, BOOT_AREA_SIZE + FILE_AREA_SIZE);
	g_load_file_length = 0;
	ret = (unsigned int)run_command ("loady", flag);
	if ((ret != 0)||(g_load_file_length == 0))
	{
		print_msg0("!err");
		goto fail;
	}

	/*烧写hupg文件*/	
	data_size = ((HI_UPG_FILE_HEADER_S *)CFG_LOAD_ADDR)->ulDataSize;
	if (data_size < FILE_AREA_SIZE)
	{
		unsigned int crc;
		crc = crc32 (0, (char *)(CFG_LOAD_ADDR + 8),
			data_size+HI_UPG_FILE_HEADER_SIZE - 8);
		crc = crc32 (crc, (char *)CFG_LOAD_ADDR, 4);
		if (crc != ((HI_UPG_FILE_HEADER_S *)CFG_LOAD_ADDR)->ulChecksum)
		{
			print_msg2("!crc:",(unsigned long)((HI_UPG_FILE_HEADER_S *)CFG_LOAD_ADDR)->ulChecksum, (unsigned long)crc);
			goto fail;
		}
		upg_type = BURN_FILE_HUPG;
		run_command ("sf probe 0", flag);
		(void)spi_flash_erase(flash, PRODUCT_CFG_CCO_UPG1_REAL_FLASH_ADDR_U,PRODUCT_CFG_CCO_UPG2_REAL_FLASH_ADDR_U-PRODUCT_CFG_CCO_UPG1_REAL_FLASH_ADDR_U);
		(void)spi_flash_erase(flash, PRODUCT_CFG_CCO_UPG2_REAL_FLASH_ADDR_U,CFG_FLASH_BLOCK_SIZE);//踢狗，防止个别flash擦除时间过长，踢狗后接着擦
		(void)spi_flash_erase(flash, PRODUCT_CFG_CCO_NV_FLASH_ADDR_U, NV_FILE_SIZE_MAX + CFG_FLASH_BLOCK_SIZE*4);

		(void)spi_flash_write(flash, PRODUCT_CFG_CCO_UPG1_REAL_FLASH_ADDR_U, PRODUCT_CFG_CCO_UPG2_REAL_FLASH_ADDR_U-PRODUCT_CFG_CCO_UPG1_REAL_FLASH_ADDR_U, 
			(const void *)CFG_LOAD_ADDR);
	}	 
	else
	{
		print_msg0("!file");
		goto fail;
	}

    //读回一个字节，保证之前操作完成
    (void)spi_flash_read(flash, 0, 1, (void *)CFG_LOAD_ADDR);
    print_msg1(" To RST:",upg_type);
    mdelay(100);
    run_command ("re", flag);

    return 0;
    
fail:
	enable_watchdog();
	main_loop();

	return (-1);
}
int do_burnhbin(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    unsigned int ret = 0;
    unsigned int upg_type = 0;

    HI_UNREF_PARAM(cmdtp);
    HI_UNREF_PARAM(argv);

#ifdef BSP_CODEX_CHECK_CLOSE
    if (g_manage_authority != TRUE)
    {
    	serial_puts(CFG_REQUIRE_PASSWORD);
        if(input_password())
            return -1;
    }
#endif    
    if (argc > 2)
    {
    	print_msg1("!arc:",(unsigned long)argc);
    	goto fail;
    }

    disable_watchdog(); 
    //清理内存并装载
/*
        【屏蔽人】z00193780
        【审核人】w00190206
        【代码分析】boot下memcpy、memset、strcpy替换为安全接口会增加2k bytes左右的代码,并且以上接口性能分别会降低一定比例，所以建议boot下不做安全接口替换。
        【功能影响】不影响正常业务，屏蔽告警
*/
    memset_s((u8*)CFG_LOAD_ADDR, BOOT_AREA_SIZE + FILE_AREA_SIZE, 0xff, BOOT_AREA_SIZE + FILE_AREA_SIZE);
    g_load_file_length = 0;
    ret = (unsigned int)run_command ("loady", flag);
    if ((ret != 0)||(g_load_file_length == 0))
    {
    	print_msg0("!cmd");
    	goto fail;
    }
	
    /*烧写hbin文件*/
    upg_type = BURN_FILE_HBIN;
    if (g_load_file_length > PRODUCT_CFG_UPG2_FILE_FLASH_ADDR_U)//大于 2区的地址，认为超大
    {
    	print_msg0("!len");
        goto fail;
    }
    
    run_command ("sf probe 0", flag);
    
    //删除A区;删除B区的第一块;删除NV文件区索引区temp区;删除BOOT
    (void)spi_flash_erase(flash, PRODUCT_CFG_CCO_UPG1_REAL_FLASH_ADDR_U,PRODUCT_CFG_CCO_UPG2_REAL_FLASH_ADDR_U-PRODUCT_CFG_CCO_UPG1_REAL_FLASH_ADDR_U);
    (void)spi_flash_erase(flash, PRODUCT_CFG_CCO_UPG2_REAL_FLASH_ADDR_U,CFG_FLASH_BLOCK_SIZE);
    (void)spi_flash_erase(flash, PRODUCT_CFG_CCO_NV_FLASH_ADDR_U, NV_FILE_SIZE_MAX + CFG_FLASH_BLOCK_SIZE*4);
    (void)spi_flash_erase(flash, 0, BOOT_AREA_SIZE);
	
    //写BOOT;写A区
    (void)spi_flash_write(flash, 0, BOOT_AREA_SIZE, (const void *)CFG_LOAD_ADDR);
    (void)spi_flash_write(flash, PRODUCT_CFG_CCO_UPG1_REAL_FLASH_ADDR_U, PRODUCT_CFG_CCO_UPG2_REAL_FLASH_ADDR_U-PRODUCT_CFG_CCO_UPG1_REAL_FLASH_ADDR_U, 
        (const void *)(CFG_LOAD_ADDR+BOOT_AREA_SIZE));

	//读回一个字节，保证之前操作完成
	(void)spi_flash_read(flash, 0, 1, (void *)CFG_LOAD_ADDR);
	print_msg1(" To RST:",upg_type);
	mdelay(100);
	run_command ("re", flag);

	return 0;
	
fail:
	enable_watchdog();
	main_loop();

	return (-1);
}

U_BOOT_CMD(
	sf,	5,	1,	(void *)do_spi_flash,
	"sf probe [cs]   - SPI flash Operation\r\n"
	"   read  [m addr] [offset] [len]\r\n"
	"   write [m addr] [offset] [len]\r\n"
	"   erase [offset] [len]");
U_BOOT_CMD(
	burnupg, 2, 0,(void *)do_burnhupg,
	"");
U_BOOT_CMD(
	burnbin, 2, 0,(void *)do_burnhbin,
	"");

