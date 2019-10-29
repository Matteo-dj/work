#include <global_data.h>
#include <led.h>
#include <spi_flash.h>
#include <product.h>
#include <debug.h>
#include <LzmaTools.h>
#include <boot.h>
#include <command.h>
#include "securec.h"
#include <reg.h>
#include <gpio.h>

#define GENERAL_V200_MAGIC (0x39110200)
/*
版本格式: 0xaabbccdd(aa=2=>V200)(bbccdd=>bb.cc.dd)
版本修改记录:
0x02000001: nv解耦版本;
0x02000002: 删除magic字判断;
0x02000003: 合入hibuild,io复用替换;
0x02000004: 优选问题替换为uboot2016;
0x02000005: 删除密码;
0x02000006: 修改版本回退处理bug;
0x02000007: 1.boot下换区策略修改；2.关灯;
0x02000008: 增加flash保护驱动
0x02000058: 测试升级版本;
*/
#define GENERAL_BOOT_VER (0x01000001)

/**
 * @ingroup hct_uart
 * UART 配置
 */
typedef struct
{
    unsigned int  baud_rate;  /**<波特率*/
     unsigned int  data_bits;  /**<数据位宽*/
     unsigned int  stop_bits;  /**<停止位 0:0个停止位 1:1.5或2个停止位*/
     unsigned int  parity;     /**<奇偶校验 0:无校验 1:奇校验 2:偶校验*/
} HI_UART_INIT_TYPE_S;
/*
 * Pointer to initial global data area
 *
 * Here we initialize it.
 */
 gd_t *gd = HI_NULL;

struct spi_flash *s_sfc_new = HI_NULL;

int raise  ( int signo)
{
    HI_UNREF_PARAM(signo);
    print_msg0("raise");
	return -1;
}

static void boot_kernel(unsigned kaddr)
{
	void (*entry)(void) = (void*) kaddr;
	entry();
}

void board_init(void)
{
    io_remap();
    enable_watchdog();
#ifdef PRODUCT_CFG_COLLECT_II	
	gpio_init();
#endif
    serial_init(HI_UART1_REG_BASE);
	serial_init(HI_UART0_REG_BASE);
    led_init(HI_LED_HIGH);   
}

void change_area(void)
{
    u32 flash_addr = gd->env_data.valid_data.addr_backup;

    gd->env_data.valid_data.addr_backup = gd->env_data.valid_data.addr_start;
    gd->env_data.valid_data.addr_start = flash_addr;
}

void set_bootver(void)
{
	*(volatile unsigned int *)(GENERAL_REG_V200_FLAG) = GENERAL_V200_MAGIC;
	*(volatile unsigned int *)(GENERAL_REG_BOOT_VER) = GENERAL_BOOT_VER;
	*(volatile unsigned int *)(GENERAL_REG_ENV_ADDR1) = gd->env_addr1;
	*(volatile unsigned int *)(GENERAL_REG_ENV_ADDR2) = gd->env_addr2;
}



s32 check_hupg_multi_times(u32 addr,u32 ulCheckTimes,HI_UPG_FILE_HEADER_S *pHead)
{
    u32 i=0;
    s32 ret=0;


    for(i=0;i<ulCheckTimes;i++)
    {
        ret=check_hupg(addr, pHead);
    
        if(ret==0)
        {
            return 0;
        }
    }
    
    return ret;
}

void boot_head(u32 addr)
{
	int ret = 0;
	HI_UPG_FILE_HEADER_S head = {0};
    u32 uncompressedSize = CFG_DECOMPRESSED_SIZE;//通过计算获得，空间结构图画出
	
	//参数判断

	//check hupg
/*
        【屏蔽人】z00193780
        【审核人】w00190206
        【代码分析】CFG_UPG_DATA_TEMP_ADDR为待解压NV文件地址，大小为420k，FILE_AREA_SIZE为420k，未越界。
        【功能影响】不影响正常业务，屏蔽告警
*/
	memset_s((void*)CFG_UPG_DATA_TEMP_ADDR, FILE_AREA_SIZE, 0x0, FILE_AREA_SIZE);
	ret = check_hupg_multi_times(addr,3,&head);//防止ESD等异常造成的内存原因导致的hupg校验失败，进行多次校验。
	if (ret)
	{
		print_msg2("Check_upg fail:", (unsigned long)addr, (unsigned long)ret);
		change_area();
		addr = gd->env_data.valid_data.addr_start;
        gd->env_data.valid_data.ucDecompressNv=ENV_DECOMPRESS_NV;
		ret = check_hupg(addr, &head);
		if (ret)
		{
			print_msg2("Check_upg fail:", (unsigned long)addr, (unsigned long)ret);
			goto fail;
		}
		else
		{
			//换区保存
			saveenv();
		}
	}
	enable_mmu();
	/*delzma code*/
    ret = lzmaBuffToBuffDecompress ((unsigned char*)CFG_LOAD_ADDR, &uncompressedSize,
      (unsigned char *)(CFG_UPG_DATA_TEMP_ADDR+HI_UPG_FILE_HEADER_SIZE+head.stSecCode.astCfg[0].ulOffset), head.stSecCode.astCfg[0].ulSectionSize);
	if (ret)
	{
		print_msg1("Decompress code:", (unsigned long)ret);
        dcache_disable();
		goto fail;
	}
	set_bootver();
#ifdef BOOT_TIMER_DEBUG_ON    
    print_msg1("total time: ",timecnt_time());
#endif
	//print_msg1("\r\nKernel at ",addr);
	//print_msg0("\r\n ");

	//引导内核
    stop_mmu();
	serial_exit();
	boot_kernel(CFG_LOAD_ADDR);

fail:
	print_msg0("Fail=>Input cmd:");
	main_loop();	
}

/*
** check_crc
** 功能: 读回数据到内存，且验证hupg合法性
** 返回值: 0正确；-1 len错误; -2 crc wrong
*/
#define HUPG_SIZE_WRONG (-1)
#define HUPG_CRC_WRONG (-2)
s32 check_hupg(u32 addr, HI_UPG_FILE_HEADER_S *head)
{
    u32 crc;

    s_sfc_new->read(s_sfc_new, addr,
        sizeof(HI_UPG_FILE_HEADER_S),(void*)head);
    if (head->ulDataSize > FILE_AREA_SIZE)
    {
    	print_msg1("L:upg size:", (unsigned long)head->ulDataSize);	
	    return HUPG_SIZE_WRONG;
    }
    else
    {
        s_sfc_new->read(s_sfc_new, addr,
            head->ulDataSize+HI_UPG_FILE_HEADER_SIZE,
            (void*)CFG_UPG_DATA_TEMP_ADDR);
        crc = crc32 (0, (char *)(CFG_UPG_DATA_TEMP_ADDR + 8),
            head->ulDataSize+HI_UPG_FILE_HEADER_SIZE - 8);
		crc = crc32 (crc, (char *)CFG_UPG_DATA_TEMP_ADDR, 4);
        if (crc == head->ulChecksum)
        {
            return 0;
        }
        else
        {
            return HUPG_CRC_WRONG;
        }
    }
}

void execute_upg_boot(void)
{
	int ret = 0;
	unsigned int loadaddr = 0;
	HI_ENV_DATA_ST *penv = &(gd->env_data.valid_data);	

	//获取环境变量
	env_relocate();
	if (1 == penv->load_direct)
	{
		print_msg1("Boot direct:", (unsigned long)penv->addr_start);	
		boot_kernel(penv->addr_start);
	}
	loadaddr = penv->addr_start;
	//print_msg1("Env ctmax:", (unsigned long)penv->cnt_max);	
	//print_msg1("Env mode:", (unsigned long)penv->mode);
	//print_msg1("Env rstcnt:", (unsigned long)penv->reset_cnt);
	if ((penv->cnt_max > 0)&&(penv->mode == HI_ND_SYS_STATUS_UPGRADE))
	{
		penv->reset_cnt ++;
		if (penv->reset_cnt >= penv->cnt_max)
		{
			change_area ();
			loadaddr = penv->addr_start;
			penv->mode = HI_ND_SYS_STATUS_NORMAL;
			//penv->reset_cnt = 0;
			ret = saveenv();
			if (0!= ret)
			{
				//to do
				print_msg0("To do");
			}
			print_msg0("!!!Upg verify fail.");
		}
		else //正常升级状态分支
		{
			ret = saveenv();//用于回退版本
			if (0!= ret)
			{
				//to do
				print_msg0("To do");
			}
		}
	}
	//print_msg1("Loadaddr:", (unsigned long)loadaddr);	
	boot_head(loadaddr);
}

// the entry of C.
void start_cfunction(void)
{ 
#ifdef BOOT_TIMER_DEBUG_ON
	timecnt_enable();
#endif  
    boot_malloc_init();
    board_init();
    boot_regist_cmd();

    //环境变量初始化，放在串口初始化后
	env_init();	   

    //字符串打印
    print_msg0("\r\nboot v1.0 "VERSION_BOOT);
    print_msg0(PRODUCT_CFG_BUILD_DATE" "PRODUCT_CFG_BUILD_TIME"\r\n");
	check_ctrlc();
    
	//如果调试直接引导在此处加  
    execute_upg_boot();
}
