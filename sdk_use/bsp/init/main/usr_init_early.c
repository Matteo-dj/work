#include <hi_types.h>
#include <hi_mdm_types.h>
#include <hi_init.h>
#include <hi_mdm_cfg_early.h>
#include <hi_sal_cfg.h>
#include <hi_ft_nv.h>
#include <hi_mdm_sys.h>
#include <hi_sal_nv.h>
#include <hi_stdlib.h>
#include <hi_sal.h>


HI_VOID usr_freq_init(HI_VOID)
{
#ifndef PRODUCT_CFG_BOARD_TYPE_ASIC    
    HI_MDM_UART_SetFreq(UART_CLK_APB);
#elif defined PRODUCT_CFG_BOARD_TYPE_ASIC_V200
    HI_MDM_UART_SetFreq(UART_CLK_XTAL);   
#endif

}

HI_VOID usr_ram_size_init(HI_VOID)
{
#ifndef PRODUCT_CFG_BOARD_TYPE_ASIC    
    (HI_VOID)HI_MDM_MEM_SetSwSize(RAM_SIZE_32M);
#elif defined PRODUCT_CFG_BOARD_TYPE_ASIC_V200

#if defined (PRODUCT_CFG_PRODUCT_TYPE_STA)
    if (HI_FTM_CHIP_TYPE_3911C == HI_MDM_GetHwChipType())
    {
        HI_MDM_MEM_SetSwSize(RAM_SIZE_8M);
    }
    else
    {
        HI_MDM_MEM_SetSwSize(RAM_SIZE_2M);
    }  
#else
    HI_MDM_MEM_SetSwSize(RAM_SIZE_8M);
#endif
#endif
}

HI_U32 usr_mem_init(HI_IN HI_FTM_CHIP_TYPE_E enChipType, HI_OUT HI_U32 aulMemSize[2])
{
    HI_U32 ret			  = HI_ERR_SUCCESS;
    HI_U8 ucDevCfgId	  = (HI_U8)(HI_FTM_CHIP_TYPE_UNKOWN);
    HI_U32 ulStaticSize = 0;
    HI_U32 ulDynRamSize  = 0;
    HI_NV_SAL_CONFIG_S stSalCfg;
    
    if((enChipType != HI_FTM_CHIP_TYPE_UNKOWN) && (enChipType <= HI_DMS_CHIP_PRODUCT_TYPE_MAX))
    {
        ucDevCfgId = (HI_U8)(enChipType - 1);
    }

	// 读取软件配置的内存大小
    (hi_void)memset_s(&stSalCfg,sizeof(stSalCfg), 0,sizeof(stSalCfg));
#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
    if (HI_FTM_CHIP_TYPE_3911C == HI_MDM_GetHwChipType())
    {
        ulStaticSize = HI_DEFALT_STATIC_MEM_SIZE_MAX;
        ulDynRamSize = HI_DEFALT_DYN_MEM_SIZE_MAX;
    }
    else
    {
        ret = HI_NVM_Read(HI_NV_SAL_CONFIG, &stSalCfg, sizeof(stSalCfg));
        if (ret != HI_ERR_SUCCESS)
        {
        	ulStaticSize = HI_DIAG_MODE_DEFAULT_STATIC_MEM_SIZE;
        	ulDynRamSize = HI_DIAG_MODE_DEFAULT_DYN_MEM_SIZE;
        }
        else
        {
        	ulStaticSize = (HI_U32)stSalCfg.aslStaticRamSize[ucDevCfgId];
        	ulDynRamSize = (HI_U32)stSalCfg.aslDynRamSize[ucDevCfgId];
        }
    }
#else
    ret = HI_NVM_Read(HI_NV_SAL_CONFIG, &stSalCfg, sizeof(stSalCfg));
    if (ret != HI_ERR_SUCCESS)
    {
        ulStaticSize = HI_DIAG_MODE_DEFAULT_STATIC_MEM_SIZE;
        ulDynRamSize = HI_DIAG_MODE_DEFAULT_DYN_MEM_SIZE;
    }
    else
    {
        ulStaticSize = (HI_U32)stSalCfg.aslStaticRamSize[ucDevCfgId];
        ulDynRamSize = (HI_U32)stSalCfg.aslDynRamSize[ucDevCfgId];
    }

#endif

    //dprintf("Total memory size = 0x%x",HI_MDM_MEM_GetSwSize());
	//dprintf ("Our System static  memory pool size:0x%x \n",ulStaticSize);
	//dprintf ("Our System Dynamic memory pool size:0x%x \n",ulDynRamSize);
    
    ret = HI_MDM_MEM_InitDynStaticPool(ulDynRamSize,ulStaticSize);
    if (ret != HI_ERR_SUCCESS)
    {
        if (ret == HI_ERR_NOT_ENOUGH_MEMORY)
        {
            ulDynRamSize = HI_DIAG_MODE_DEFAULT_DYN_MEM_SIZE;
            ulStaticSize = HI_DIAG_MODE_DEFAULT_STATIC_MEM_SIZE;
            ret = HI_MDM_MEM_InitDynStaticPool(ulDynRamSize,ulStaticSize);
        }
    }

	// aulMemAdd[2];  // 内存池首地址，第一个为静态内存池，第二个为动态内存池
	// aulMemSize[2]; // 内存池大小，单位字节，第一个为静态内存池，第二个为动态内存池	 
    if (ret == HI_ERR_SUCCESS)
    {
        aulMemSize[0] = ulStaticSize;
        aulMemSize[1] = ulDynRamSize;
    }
    	
    return ret;
}

HI_VOID usr_osa_init(HI_VOID)
{
    HI_CFG_OSA_RESOURCE_S stOsaCfg={0};
    stOsaCfg.ulTskNum = 32;
    stOsaCfg.ulQueNum = 32;
    stOsaCfg.ulSemNum = 100;
    stOsaCfg.ulMuxNum = 50;
    stOsaCfg.ulSwtNum = 80;
    stOsaCfg.ulWorkqNum = 256;
    stOsaCfg.ulEventNum = 30;
    (HI_VOID)HI_CFG_SetOsaResourceEarly(&stOsaCfg);
}

/**
* @ingroup  
* @brief  用户初始化配置系统
*
* @par 描述:
* 用户初始化配置系统
* 
* @attention
* @li 初始化阶段BSS段未初始化，不能使用BSS段变量。
* @li 初始化阶段系统未初始化完成，除hi_mdm_cfg_early.h提供的接口和一些存硬件接口外，不能调用任何接口(包括内存，任务，睡眠，其它系统资源相关接口)。
* 
* @param  ulData [IN] 回调入参。
*
* @retval 无
* @par Dependency:
* @li hi_mdm_timer.h: 该接口声明所在的头文件。
* @see HI_MDM_TIMER_Start | HI_MDM_TIMER_Stop
* @since HiMDL_V200R001C00
*/
extern HI_U8 g_valid_chip_type_num;
extern HI_U8 *g_valid_chip_type_list;
HI_U8 g_x_chip_type_list[] = {0x11,0x12,0x21,0x22}; /* 根据实际添加 */
HI_U32 APP_InitCfgEarly(HI_VOID)
{
	g_valid_chip_type_list = g_x_chip_type_list;
	g_valid_chip_type_num = HI_ARRAY_COUNT(g_x_chip_type_list);
    usr_osa_init();
    usr_clock_init();
    usr_ram_size_init();
    usr_freq_init();
    return HI_ERR_SUCCESS;

}

