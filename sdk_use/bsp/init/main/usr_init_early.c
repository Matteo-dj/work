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

	// ��ȡ������õ��ڴ��С
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

	// aulMemAdd[2];  // �ڴ���׵�ַ����һ��Ϊ��̬�ڴ�أ��ڶ���Ϊ��̬�ڴ��
	// aulMemSize[2]; // �ڴ�ش�С����λ�ֽڣ���һ��Ϊ��̬�ڴ�أ��ڶ���Ϊ��̬�ڴ��	 
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
* @brief  �û���ʼ������ϵͳ
*
* @par ����:
* �û���ʼ������ϵͳ
* 
* @attention
* @li ��ʼ���׶�BSS��δ��ʼ��������ʹ��BSS�α�����
* @li ��ʼ���׶�ϵͳδ��ʼ����ɣ���hi_mdm_cfg_early.h�ṩ�Ľӿں�һЩ��Ӳ���ӿ��⣬���ܵ����κνӿ�(�����ڴ棬����˯�ߣ�����ϵͳ��Դ��ؽӿ�)��
* 
* @param  ulData [IN] �ص���Ρ�
*
* @retval ��
* @par Dependency:
* @li hi_mdm_timer.h: �ýӿ��������ڵ�ͷ�ļ���
* @see HI_MDM_TIMER_Start | HI_MDM_TIMER_Stop
* @since HiMDL_V200R001C00
*/
extern HI_U8 g_valid_chip_type_num;
extern HI_U8 *g_valid_chip_type_list;
HI_U8 g_x_chip_type_list[] = {0x11,0x12,0x21,0x22}; /* ����ʵ����� */
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

