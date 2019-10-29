
#include <spifdrv.h>
#include <spif_protect.h>
#include <spif_protect_drv.h>

#ifdef PRODUCT_CFG_FLASH_PROTECT
/*****************************************************************************
 函 数 名  : spif_change_protect_area
 功能描述  : 根据状态寄存器的值改变保护区域
 输入参数  : ulStatusRegVal 待写入的控制保护区域的状态寄存器的值
 输出参数  : 无
 注意事项  :调用者保证flash不是busy状态
*****************************************************************************/
HI_U32 spif_change_protect_area(SFC_HOST_S *pstHost, HI_U32 ulStatusRegVal)
{
    HI_U32 ulRegval = 0;
    HI_U32 ulTimes = 0;
    HI_U32 ulRet = HI_ERR_SUCCESS;
    SPI_FLASH_INFO_S *pstInfo = pstHost->stSpiFlashInfo;
    
    if(spif_wait_ready(pstHost, pstInfo, 0))//如果一直忙，最好不要保护住flash，以免擦写失败了，不保护是小事。
	{
		return HI_ERR_TIMEOUT;
	}
    
    spif_wp_protect_ctrl(pstHost,HI_FALSE);
    HI_HRTIMER_Udelay(1);                   //wp管脚置低，让状态寄存器可写  需要最少20ns，才能让cs变为低
    spif_write_enable(pstHost, pstInfo);
    do
    {
        spif_write_status_reg(pstHost, pstInfo, SPI_CMD_WRSR1, ulStatusRegVal,1);  
        if (spif_wait_ready(pstHost, pstInfo, 0))
    	{
    	    return HI_ERR_TIMEOUT;          //如果一直忙，就不让wp有效了，原因是:过一会就不忙了，然后值写进去了，缺点是下次写前512k之前状态寄存器都没有wp去保护。
                                            //若一直忙还是把wp有效，本次写入值可能直接就错了，因为没写完。
    	}
        ulRegval = spif_read_status_reg(pstHost, pstInfo, SPI_CMD_RDSR);
        ulTimes++;
    }while((ulRegval != ulStatusRegVal) && (ulTimes<SPI_PROTECT_MAX_TIMES_WR_STATUS_REG));//尝试写三次
    if((SPI_PROTECT_MAX_TIMES_WR_STATUS_REG == ulTimes) && (ulRegval != ulStatusRegVal))
    {
        ulRet =  HI_ERR_FLASH_UP_TO_MAX_TIMES_WR_REG; //写三次都写不进去就不保护，处于故障状态，保护起来状态寄存器wp有效

    }
    HI_HRTIMER_Udelay(1);   //cs置高后，至少等100ns才可置wp为低    
    spif_wp_protect_ctrl(pstHost,HI_TRUE);

    return ulRet;
}
/*约定: 
1、如果写状态寄存器失败了，则要一直去尝试，直到写对，原因:写失败了，则可能把NV保护起来了，影响功能。
2、写状态寄存器后一直忙，则不让wp有效，原因:可能过一会就不忙了，就写进去了，若wp有效，则可能写入错误数据
   若写失败了，写完不忙，就把wp置为有效，保护起来，但是还要一直尝试去写，见约定1。

*/
/*****************************************************************************
 函 数 名  : spif_protect_all
 功能描述  : 保护前512KB
 输入参数  : 无
 输出参数  : 无
*****************************************************************************/
HI_U32 spif_protect_all(HI_VOID)
{
    HI_U32 regval = 0;
    HI_U32 ulRet = HI_ERR_SUCCESS;
    SPIF_PROTECT_HOST_S *pstProtectHost = &g_stProtectHost;
    SFC_HOST_S * pstHost = &g_stSfc300Host;
    SPI_FLASH_INFO_S *pstInfo = pstHost->stSpiFlashInfo;
    
    // TODO:暂时不加switch-case判断flash列表的supporttype，
    // TODO: 原因:当前只有华邦支持保护，增加代码会增加圈复杂度，后续发现有type2再加。
    
    regval = spif_read_status_reg(pstHost, pstInfo, SPI_CMD_RDSR);    
    if((regval&SPI_PROTECT_MASK_512KB) != SPI_PROTECT_512KB)
    {  
        regval = (SPI_PROTECT_512KB | SPI_PROTECT_SET_SRP0);
        ulRet = spif_change_protect_area(pstHost,regval);
        if(HI_ERR_SUCCESS != ulRet)
        {
            return ulRet;//如果保护失败了，时间ulEnableProtectSec不清零，让在看门狗任务中过一会还会去尝试保护起来
        }
    } 
    pstProtectHost->ulRecoverTime = 0;//时间清零，表示现在状态就是保护了前512K，在看门狗钩子中会判断
    return HI_ERR_SUCCESS;
}

/*****************************************************************************
 函 数 名  : spif_recover_protect_after_set_time
 功能描述  : 在一定时间后保护住前512K
 输入参数  : ulOffset flash偏移地址
 输出参数  : 无
*****************************************************************************/
HI_VOID spif_recover_protect_after_set_time(HI_U32 ulOffset)
{  
    SPIF_PROTECT_HOST_S *pstProtectHost = &g_stProtectHost;
    HI_U32 ulTime = 0;

    if(ulOffset >= _512K)//保留，接口独立性
    {
        return;
    }
#ifdef PRODUCT_CFG_PRODUCT_FLASH_PROTECT_SUPPORT_32KB       
    if(ulOffset < _32K)
    {
        pstProtectHost->ulRecoverTime = (HI_U32)(-1);//无限大时间，100多年，升级boot重启时让整片处于无保护状态
    }
    else  //[32k,512k)
#endif
    {
        ulTime = HI_MDM_GetSeconds();
        if(HI_ERR_FAILURE != ulTime)//时间若获取失败，则pstProtectHost->ulRecoverTime一直不变，直到下次擦写再刷新时间，问题不大
        {
            ulTime += SPI_PROTECT_ENABLE_TIME;
            if(pstProtectHost->ulRecoverTime < ulTime)
            {
                pstProtectHost->ulRecoverTime = ulTime;
            }
        }
    }
    
}

/*****************************************************************************
 函 数 名  : spif_shrink_protect_area
 功能描述  : 根据地址，使flash相应区域处于没有保护状态,
           : 并在设定的时间到期后恢复成保护前512K状态。
 输入参数  : ulOffset:flash偏移地址
 输出参数  : 无
 说明:HI_ERR_SUCCESS表示可以继续擦，写;其他值表示不可继续擦，写。
*****************************************************************************/
HI_U32 spif_shrink_protect_area(HI_U32 ulOffset)
{
    HI_U32 regval = 0;
    HI_U32 ulRet = HI_ERR_SUCCESS;

    SFC_HOST_S * pstHost = &g_stSfc300Host;
    SPI_FLASH_INFO_S *pstInfo = pstHost->stSpiFlashInfo;

    if(!spif_protect_mechanism_is_enable())
    {
        return HI_ERR_SUCCESS;
    }
    
    // 即使大于512K的也不做异常判断了，在kernel初始化中判断了是不是512K了
    if(ulOffset >= _512K)
    {
        return HI_ERR_SUCCESS;
    }

    // TODO:暂时不加switch-case判断flash列表的supporttype，
    // TODO: 原因:当前只有华邦支持保护，增加代码会增加圈复杂度，后续发现有type2再加。
    
    regval = spif_read_status_reg(pstHost, pstInfo, SPI_CMD_RDSR);
#ifdef PRODUCT_CFG_PRODUCT_FLASH_PROTECT_SUPPORT_32KB
    if(ulOffset < _32K)//整片去掉保护
#endif
    {      
        if((regval&SPI_PROTECT_MASK_NONE) != SPI_PROTECT_NONE)//能否和下面的重复的提取城一个函数
        {
            regval = SPI_PROTECT_NONE|SPI_PROTECT_SET_SRP0;
            ulRet = spif_change_protect_area(pstHost,regval);
            if(HI_ERR_SUCCESS != ulRet)
            {
                //写不进去只能返错，等下次调用擦写操作前512k接口后才再去尝试，不在看门狗一直尝试了。
                return ulRet;
            }
        }
    }
#ifdef PRODUCT_CFG_PRODUCT_FLASH_PROTECT_SUPPORT_32KB    
    else if((ulOffset >=_32K ) && (ulOffset < _512K))           //解保护前的状态只要不是保护了32K，也不是全面无保护，就给置为一个确定的状态:保护住前32K
    {   
        if((regval&SPI_PROTECT_MASK_32KB) != SPI_PROTECT_32KB)  //比boot下此处判断少了个条件(regval&SPI_PROTECT_MASK_NONE)!=SPI_PROTECT_NONE)，
        {                                                       //因为boot下在我们的分区场景下不去扩大保护范围，kernel下要扩大，保证boot尽量处于保护状态
            regval = SPI_PROTECT_32KB|SPI_PROTECT_SET_SRP0;
            ulRet = spif_change_protect_area(pstHost,regval);
            if(HI_ERR_SUCCESS != ulRet)
            {
                return ulRet;
            }

        }
    } 
#endif    
    spif_recover_protect_after_set_time(ulOffset);
    return HI_ERR_SUCCESS;
}

    

#endif