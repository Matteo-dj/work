


#include "hi_types.h"
#include "hi_mdm_types.h"
#include <spifdrv.h>
#include <spiflash.h>
#include <spif_protect_drv.h>
#include <spif_protect.h>
#include "hi_platform.h"
#include "spi_ids.h"


#ifdef PRODUCT_CFG_FLASH_PROTECT

HI_U32 HI_FLASH_ProtectProcess(HI_VOID);


SPIF_PROTECT_HOST_S g_stProtectHost;

/*****************************************************************************
 函 数 名  : spif_protect_mechanism_is_enable
 功能描述  : 查看是否使能保护机制
 输入参数  : 无
 返回值    : HI_TRUE 使能;HI_FALSE 没有使能
*****************************************************************************/
HI_BOOL spif_protect_mechanism_is_enable(HI_VOID)
{
    if(FLASH_PROTECT_ENABLE_MECHANISM == g_stProtectHost.ulEnableMechanism)
    {
        return HI_TRUE;
    }
    return HI_FALSE;
}

/*****************************************************************************
 函 数 名  : spif_get_recover_time
 功能描述  : 查询在多少秒后会保护住前512K
 输入参数  : 无
 返回值    : 时间，单位:秒
*****************************************************************************/
#define spif_get_recover_time()     (g_stProtectHost.ulRecoverTime)

/*****************************************************************************
 函 数 名  : spif_protect_none
 功能描述  : 整片flash都去掉保护
 输入参数  : 无
 输出参数  : 无
 返回值    : HI_ERR_TIMEOUT 忙，不去掉保护；HI_ERR_SUCCESS成功
             HI_ERR_NOT_SUPPORT原本就不支持保护
*****************************************************************************/
#define spif_protect_none()     spif_shrink_protect_area(0)

/*****************************************************************************
 函 数 名  : HI_FLASH_PortectInit
 功能描述  : flash保护初始化
 输入参数  : 无
 输出参数  : 无
 返回值    :HI_ERR_SUCCESS初始化成功
*****************************************************************************/
HI_U32 HI_FLASH_PortectInit(HI_VOID)//即使初始化失败也不能影响其他擦写功能，只要擦写flash后就会在看门狗中再次保护起来flash。
{   
    HI_U32 ulRet = HI_ERR_SUCCESS;
    SPIF_PROTECT_HOST_S *pstProtectHost = &g_stProtectHost;
    HI_U16 usProtectType = 0;
    (hi_void)memset_s(pstProtectHost, sizeof(SPIF_PROTECT_HOST_S), 0x0, sizeof(SPIF_PROTECT_HOST_S));

    ulRet = FLASH_LOCK();
    if (HI_ERR_SUCCESS != ulRet)
    {
        return ulRet;		 
    }
    
    usProtectType = spif_get_spiinfo()->usProtectType;
    if((spif_get_protect_magic() != FLASH_PROTECT_MAGIC))
    {    
        if(FLASH_PROTECT_NOT_SUPPORT != usProtectType)//该flash型号在boot下不支持保护，且kernel下支持，就全片解开保护
        {
            pstProtectHost->ulEnableMechanism = FLASH_PROTECT_ENABLE_MECHANISM;//暂时使能保护，全片解锁后标志再置为不保护   
            ulRet = spif_protect_none();
            pstProtectHost->ulEnableMechanism = 0;   
            if(HI_ERR_SUCCESS != ulRet)
            {
                FLASH_UNLOCK();
                return ulRet;
            }

        }
        FLASH_UNLOCK();
        return HI_ERR_NOT_SUPPORT;
    }
    if(FLASH_PROTECT_NOT_SUPPORT == usProtectType)//boot支持保护，但kernel列表该款flash中不支持
    {
        FLASH_UNLOCK();
        return HI_ERR_NOT_SUPPORT;
    }
         
    pstProtectHost->ulEnableMechanism = FLASH_PROTECT_ENABLE_MECHANISM;    
    HI_SYS_RegisterWdMonitorTaskHook(HI_FLASH_ProtectProcess); 
    
    ulRet = spif_protect_all();
    if(HI_ERR_SUCCESS != ulRet)
    {//1.如果初始化失败了，则可能写入的值把其他非预期区域保护起来了，得靠看门狗钩子函数去接着不断尝试去恢复保护成512K
     //2.即使初始化失败了，也得启用保护功能(bIsRemoveProtect不能置1)，因为可能写入的值与预期不符把其他不期待位置保护住了,得靠flash保护驱动去解开才行
        pstProtectHost->ulRecoverTime = 1;
    }
    FLASH_UNLOCK();
    return HI_ERR_SUCCESS;

}

/*****************************************************************************
 函 数 名  : HI_FLASH_ProtectProcess
 功能描述  : 在该接口中判断使能保护前512K的时间点是否到期，到期则保护住前512K.
             该接口注册到了看门狗任务中，周期性查询是否到了使能时间点。
 输入参数  : 
 输出参数  : 无
 返回值    : 其他值 失败；HI_ERR_SUCCESS成功
*****************************************************************************/
HI_U32 HI_FLASH_ProtectProcess(HI_VOID)
{
    HI_U32 ulsec = 0;
    HI_U32 ulRet = HI_ERR_FAILURE;

    if(!spif_protect_mechanism_is_enable())
    {
        return HI_ERR_NOT_SUPPORT;
    }

    if(spif_get_recover_time())//到期时间为0表示已经处于保护512k状态了
    {          
        ulsec = HI_MDM_GetSeconds();
        if(HI_ERR_FAILURE == ulsec) 
        {
            return ulsec;
        }
        
        ulRet = FLASH_LOCK();
        if (HI_ERR_SUCCESS != ulRet)
        {
            return ulRet;		 
        }
        if(ulsec > spif_get_recover_time())
        {            
            spif_protect_all();          
        }
        FLASH_UNLOCK();
    }  
    return HI_ERR_SUCCESS;
}

/*****************************************************************************
 函 数 名  : HI_FLASH_RemoveProtect
 功能描述  : 移除flash保护的功能，下次上电后会恢复flash保护
 输入参数  : 无
 输出参数  : 无
 返回值    : 其他值 失败；HI_ERR_SUCCESS成功
*****************************************************************************/
HI_U32 HI_FLASH_RemoveProtect(HI_VOID)
{
    HI_U32 ulRet = HI_ERR_FAILURE;
    SPIF_PROTECT_HOST_S *pstProtectHost = &g_stProtectHost;

    ulRet = FLASH_LOCK();
    if (HI_ERR_SUCCESS != ulRet)
    {
        return ulRet;        
    }
    ulRet = spif_protect_none();//若不支持flash保护，则什么动作都不会有
    if(HI_ERR_SUCCESS != ulRet)//保证同时执行
    {
        FLASH_UNLOCK();
        return ulRet;
    }
    pstProtectHost->ulEnableMechanism = 0;
    FLASH_UNLOCK();
    return HI_ERR_SUCCESS;
}

/*****************************************************************************
 函 数 名  : HI_FLASH_DfxGetProtectInfo
 功能描述  : flash维测功能,用于获取信息:magic,状态寄存器1值
 输入参数  : pMagic魔术字，pStatusVal状态寄存器1的值
 输出参数  : 无
 返回值    : 其他值 失败；HI_ERR_SUCCESS成功
*****************************************************************************/
HI_U32 HI_FLASH_DfxGetProtectInfo(HI_BOOL *pbSupportProtect,HI_U8 *pucStatusVal)
{
    HI_U32 ulRet = HI_ERR_FAILURE;
    SFC_HOST_S *pstHost = &g_stSfc300Host;
    
    ulRet = FLASH_LOCK();
    if (HI_ERR_SUCCESS != ulRet)
    {
        return ulRet;        
    }
    *pbSupportProtect = spif_protect_mechanism_is_enable();
    *pucStatusVal = (HI_U8)spif_read_status_reg(pstHost, pstHost->stSpiFlashInfo, SPI_CMD_RDSR);
  
    FLASH_UNLOCK();
    return HI_ERR_SUCCESS;

}
#else

HI_U32 HI_FLASH_PortectInit(HI_VOID){return HI_ERR_SUCCESS;}

#endif
