
#include "hisfc350.h"
#include "reg.h"
#include "spi_ids.h"
#include "spi_flash_protect_drv.h"
#include <debug.h>
#include "platform.h"

#ifdef PRODUCT_CFG_FLASH_PROTECT



/*****************************************************************************
 函 数 名  : hisfc350_set_protect_magic
 功能描述  : 写flash保护机制的magic，U16类型。在kernel下读该magic，来判断是否使能flash保护机制。
             当前flash驱动只用CS==1是可以用的，即只支持一片flash，且芯片地址空间仅有从0x30000000开始，
             所以在probe时CS==1时，就置该magic。支持两片flash的场景暂不考虑，也可以关闭flash保护宏来处理。
 输入参数  : 
 输出参数  : 无
*****************************************************************************/
void hisfc350_set_protect_magic(struct spi_info *spiinfo)
{
    unsigned int value = 0;

    if(spiinfo->protecttype)
    {   
        print_msg0("Support flash protect");
        value = readl(GENERAL_REG_FLASH_PROTECT_MAGIC) & 0xffff0000;//只用u16
        value |= ((unsigned int)FLASH_PROTECT_MAGIC);
        writel(value, GENERAL_REG_FLASH_PROTECT_MAGIC); 
    }
}

/*****************************************************************************
 函 数 名  : hisfc350_change_protect_area
 功能描述  : 根据状态寄存器的值改变保护区域
 输入参数  : ulStatusRegVal 待写入的控制保护区域的状态寄存器的值
 输出参数  : 无
*****************************************************************************/
int hisfc350_change_protect_area(struct hisfc_host *host, unsigned int ulStatusRegVal)
{   
    unsigned int ulRegval = 0;
    unsigned int ulTimes = 0;
    int ulRet = 0;
    struct hisfc_spi *spi = host->spi;
    
    if(hisfc350_wait_ready(host, spi))
    {
        return -1;
    }
    hisfc350_wp_protect_ctrl(host,0);
    udelay(1);//wp管脚置低，让状态寄存器可写  需要最少20ns，才能让cs变为低
    hisfc350_write_enable(host, spi);
    do
    {
        hisfc350_write_status_reg(host, spi, SPI_CMD_WRSR1, ulStatusRegVal, 1);
        if(hisfc350_wait_ready(host, spi))
        {
           return -1;//如果一直忙，就不让wp有效了，原因是:过一会就不忙了，然后值写进去了，缺点是下次写前512k之前状态寄存器都没有wp去保护。
                     //若一直忙还是把wp有效，本次写入值可能直接就错了，因为没写完。
        }
        ulRegval = hisfc350_read_status_reg(host, spi, SPI_CMD_RDSR);
        ulTimes++;
    }while((ulRegval != ulStatusRegVal) && (ulTimes < SPI_PROTECT_MAX_TIMES_WR_STATUS_REG));//尝试写三次

    if((SPI_PROTECT_MAX_TIMES_WR_STATUS_REG == ulTimes) && (ulRegval != ulStatusRegVal))
    {
        ulRet  = -1;
        //写三次都写不进去就不保护，处于故障状态，保护起来状态寄存器wp有效
    }
    udelay(1);//cs置高后，至少等100ns才可置wp为低
    hisfc350_wp_protect_ctrl(host,1);
    return ulRet;

}

/*****************************************************************************
 函 数 名  : hisfc350_shrink_protect_area
 功能描述  : 根据地址，去使能flash保护的相应区域
 输入参数  : ulOffset:flash偏移地址
 输出参数  : 无
*****************************************************************************/

u_char g_bShrinkInit = 0;

//函数返回0表示可以继续操作,返回非0表示不可以继续操作
int hisfc350_shrink_protect_area(unsigned int ulOffset)
{
    u32 regval = 0;
    int ret = 0;
    struct hisfc_host *host = &hisfc_host;
    struct hisfc_spi *spi = host->spi;

    //boot下用magic判断。kernel下用全局变量bEnableMechanism判断
    //原因:kernel下有维测删除保护的功能，不能删除magic，否则说不清是删了还是boot下没写入
    if( hisfc350_get_protect_magic() !=  FLASH_PROTECT_MAGIC)
    {
        return 0;//不支持就不能对状态寄存器操作，寄存器驱动可能不同。
    }

    // TODO:暂时不加switch-case判断flash列表的supporttype，
    // TODO: 原因:当前只华华邦支持保护，增加代码会增加全复杂度，后续发现有type2再加。
    
    //上电后第一次擦或写flash时，判断是不是异常保护值，
    //原因:1、若写状态寄存器时断电或写出错，将大于512K地址保护住了，在boot下还想擦写后面的，则会失败
    //     2、兼容性:若kernel后续改变了保护地址，保护了前1M，则下电再上电想擦写512K~1M就会失败。
    //上电后只判断一次就可以，即认为上电前是不可靠的状态值，或不兼容的值，后续都在可控范围内。
    //     每次都判断会增加flash总线占用，增加出错风险，且费时降低效率。
    
    if(!g_bShrinkInit)
    {                                        
     //   print_msg1("FIRST_SHRINK_MAGIC:", g_ucFirstShrink);
     
        regval = hisfc350_read_status_reg(host, spi, SPI_CMD_RDSR); 
        if(((regval&SPI_PROTECT_MASK_NONE) != SPI_PROTECT_NONE)//如果为异常值，但是WP管脚未连接或损坏，是解不开保护的。
#ifdef PRODUCT_CFG_PRODUCT_FLASH_PROTECT_SUPPORT_32KB
            && ((regval&SPI_PROTECT_MASK_32KB)!=SPI_PROTECT_32KB)
#endif
            && ((regval&SPI_PROTECT_MASK_512KB)!=SPI_PROTECT_512KB))
            {
                regval = SPI_PROTECT_NONE;
                ret = hisfc350_change_protect_area(host, regval);   //要有返回值判断，擦写接口要用
                if(ret)
                {
                    return ret;
                }
            }
        g_bShrinkInit = 1;
    }
    
    if(ulOffset >= _512K)
    {
        return 0;
    }

    
    regval = hisfc350_read_status_reg(host, spi, SPI_CMD_RDSR); 
    
 //   print_msg1("Old Status Reg1:", regval);
#ifdef PRODUCT_CFG_PRODUCT_FLASH_PROTECT_SUPPORT_32KB
    if(ulOffset < _32K)//整片去掉保护
#endif
    {                
        if((regval&SPI_PROTECT_MASK_NONE) != SPI_PROTECT_NONE)
        {
            regval = SPI_PROTECT_NONE;
            ret = hisfc350_change_protect_area(host, regval);//要有返回值判断，擦写接口要用
            if(ret)
            {
                return ret;
            }
        }

    }
#ifdef PRODUCT_CFG_PRODUCT_FLASH_PROTECT_SUPPORT_32KB
    /*解保护前的状态只要不是保护了32K，也不是全面无保护，就给置为一个确定的状态:保护住前32K*/
    else if((ulOffset >=_32K ) && (ulOffset < _512K))
    {
        if(( (regval&SPI_PROTECT_MASK_32KB)!=SPI_PROTECT_32KB) && ((regval&SPI_PROTECT_MASK_NONE)!=SPI_PROTECT_NONE))
        {
            regval = SPI_PROTECT_32KB;
            ret = hisfc350_change_protect_area(host, regval);//要有返回值判断，擦写接口要用
            if(ret)
            {
                return ret;
            }
        }
    }
#endif
    return 0;

}




#endif

