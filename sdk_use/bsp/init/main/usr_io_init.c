#include <hi_types.h>
#include <hi_mdm_types.h>
#include <hi_mdm_io.h>
#include <hi_ft_nv.h>
#include <hi_sal_nv.h>
#include <hi_mdm_nv.h>
#include <hi_mdm_uart.h>
#include <hi_mdm_sys.h>
#include <hi_mdm_switch.h>
#include <hi_mdm_led.h>
#include <hi_sal.h>
#include <hi_stdlib.h>
#include <hi_hw_id.h>
#include <hi_mdm_io.h>
#include <hi_mdm_led.h>
#include <hi_mdm_switch.h>
#include <hi_ioexp.h>
#include <hi_mdm.h>//HI_MDM_GetProductType()
#include <stdlib.h>

HI_U32 HI_Device_Get_IO_Config_From_Nv(HI_FTM_PRODUCT_TYPE_E product_id,HI_IO_REMAP_INIT_CONFIG_NV_S *pIoInitRemapCfg)
{
    HI_U32 ret=HI_ERR_SUCCESS;
    HI_IO_REMAP_INIT_CONFIG_NV_S *pIoInitRemapNv = HI_NULL;
#if defined(PRODUCT_CFG_SUPPORT_UART2)
    HI_NV_FTM_STARTUP_CFG_S *pstCfgStart = HI_NULL;
#endif

    if(pIoInitRemapCfg == HI_NULL)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    pIoInitRemapNv = (HI_IO_REMAP_INIT_CONFIG_NV_S *)malloc(sizeof(HI_IO_REMAP_INIT_CONFIG_NV_S));
    if(pIoInitRemapNv == HI_NULL)
    {
        free(pIoInitRemapNv);
        return HI_ERR_MALLOC_FAILUE;
    }

    (hi_void)memset_s((HI_VOID *)pIoInitRemapNv, sizeof(HI_IO_REMAP_INIT_CONFIG_NV_S), 0, sizeof(HI_IO_REMAP_INIT_CONFIG_NV_S));
    
#ifdef PRODUCT_CFG_EQUIP_TEST_MODE
    ret = HI_MDM_NV_Read(HI_NV_BSP_IO_REMAP_INIT_CONFIG_ID_6,(HI_PVOID)pIoInitRemapNv,sizeof(HI_IO_REMAP_INIT_CONFIG_NV_S));
    if(ret==HI_ERR_SUCCESS)
    {
        (hi_void)memcpy_s(pIoInitRemapCfg, sizeof(HI_IO_REMAP_INIT_CONFIG_NV_S), pIoInitRemapNv, sizeof(HI_IO_REMAP_INIT_CONFIG_NV_S));
    }
        
#else
    if (((product_id >= HI_FTM_PRODUCT_TYPE_VALUE_C01)&&(product_id <= HI_FTM_PRODUCT_TYPE_VALUE_C08))
        ||((product_id >= HI_FTM_PRODUCT_TYPE_VALUE_C11)&&(product_id <= HI_FTM_PRODUCT_TYPE_VALUE_C18)))//集中器
    {

        ret = HI_MDM_NV_Read(HI_NV_BSP_IO_REMAP_INIT_CONFIG_ID_1,(HI_PVOID)pIoInitRemapNv,sizeof(HI_IO_REMAP_INIT_CONFIG_NV_S));
        if(ret==HI_ERR_SUCCESS)
        {
            (hi_void)memcpy_s(pIoInitRemapCfg, sizeof(HI_IO_REMAP_INIT_CONFIG_NV_S), pIoInitRemapNv, sizeof(HI_IO_REMAP_INIT_CONFIG_NV_S));
        }
            
    }
    else if ((product_id >= HI_FTM_PRODUCT_TYPE_VALUE_D61)&&(product_id <= HI_FTM_PRODUCT_TYPE_VALUE_D64))//抄控器
    {

        if(HI_TRUE != SAL_IsMeterCheckProduct())
        {
            HI_MDM_UART_Close(HI_UART_0);
            ret = HI_MDM_NV_Read(HI_NV_BSP_IO_REMAP_INIT_CONFIG_ID_2,(HI_PVOID)pIoInitRemapNv,sizeof(HI_IO_REMAP_INIT_CONFIG_NV_S));
            if(ret==HI_ERR_SUCCESS)
            {
                (hi_void)memcpy_s(pIoInitRemapCfg, sizeof(HI_IO_REMAP_INIT_CONFIG_NV_S), pIoInitRemapNv, sizeof(HI_IO_REMAP_INIT_CONFIG_NV_S));
            }

        }
        else
        {
            HI_MDM_UART_Close(HI_UART_0);
            ret = HI_MDM_NV_Read(HI_NV_BSP_IO_REMAP_INIT_CONFIG_ID_8,(HI_PVOID)pIoInitRemapNv,sizeof(HI_IO_REMAP_INIT_CONFIG_NV_S));
            if(ret==HI_ERR_SUCCESS)
            {
                (hi_void)memcpy_s(pIoInitRemapCfg, sizeof(HI_IO_REMAP_INIT_CONFIG_NV_S), pIoInitRemapNv, sizeof(HI_IO_REMAP_INIT_CONFIG_NV_S));
            }
        }

    }
    else if ((product_id >= HI_FTM_PRODUCT_TYPE_VALUE_R21)&&(product_id <= HI_FTM_PRODUCT_TYPE_VALUE_R28))//中继
    {
        HI_MDM_UART_Close(HI_UART_0);
        ret = HI_MDM_NV_Read(HI_NV_BSP_IO_REMAP_INIT_CONFIG_ID_3,(HI_PVOID)pIoInitRemapNv,sizeof(HI_IO_REMAP_INIT_CONFIG_NV_S));
        if(ret==HI_ERR_SUCCESS)
        {
            (hi_void)memcpy_s(pIoInitRemapCfg, sizeof(HI_IO_REMAP_INIT_CONFIG_NV_S), pIoInitRemapNv, sizeof(HI_IO_REMAP_INIT_CONFIG_NV_S));
        }
    }
    else if (((product_id >= HI_FTM_PRODUCT_TYPE_VALUE_M101)&&(product_id <= HI_FTM_PRODUCT_TYPE_VALUE_M104))
    ||((product_id >= HI_FTM_PRODUCT_TYPE_VALUE_M105)&&(product_id <= HI_FTM_PRODUCT_TYPE_VALUE_M108)))//STA&I采
    {
#if defined(PRODUCT_CFG_SUPPORT_UART2)
        pstCfgStart = (HI_NV_FTM_STARTUP_CFG_S *)malloc(sizeof(HI_NV_FTM_STARTUP_CFG_S));
        if (pstCfgStart == HI_NULL)
        {
            free(pIoInitRemapNv);
            return HI_ERR_MALLOC_FAILUE;
        }

        (hi_void)memset_s((HI_VOID *)pstCfgStart, sizeof(HI_NV_FTM_STARTUP_CFG_S), 0, sizeof(HI_NV_FTM_STARTUP_CFG_S));
        
        ret = HI_MDM_NV_Read(HI_NV_FTM_STARTUP_CFG_ID,(HI_PVOID)pstCfgStart,sizeof(HI_NV_FTM_STARTUP_CFG_S));
        //sta工装
        if ((ret == HI_ERR_SUCCESS)&&(pstCfgStart->ucTestMode == 1))
        {
            ret = HI_MDM_NV_Read(HI_NV_BSP_IO_REMAP_INIT_CONFIG_ID_6,(HI_PVOID)pIoInitRemapNv,sizeof(HI_IO_REMAP_INIT_CONFIG_NV_S));
            if(ret==HI_ERR_SUCCESS)
            {
                (hi_void)memcpy_s(pIoInitRemapCfg, sizeof(HI_IO_REMAP_INIT_CONFIG_NV_S), pIoInitRemapNv, sizeof(HI_IO_REMAP_INIT_CONFIG_NV_S));
            }
        }
        //sta模块
        else
#endif
        {
            if (HI_FTM_CHIP_TYPE_3911T == HI_MDM_GetHwChipType())// 如果为 T芯片
            {
                ret = HI_MDM_NV_Read(HI_NV_BSP_IO_REMAP_INIT_CONFIG_ID_4,(HI_PVOID)pIoInitRemapNv,sizeof(HI_IO_REMAP_INIT_CONFIG_NV_S));
            }
            else// 如果为V200C芯片
            {
                ret = HI_MDM_NV_Read(HI_NV_BSP_IO_REMAP_INIT_CONFIG_ID_A,(HI_PVOID)pIoInitRemapNv,sizeof(HI_IO_REMAP_INIT_CONFIG_NV_S));
            }

            if(ret==HI_ERR_SUCCESS)
            {
                (hi_void)memcpy_s(pIoInitRemapCfg, sizeof(HI_IO_REMAP_INIT_CONFIG_NV_S), pIoInitRemapNv, sizeof(HI_IO_REMAP_INIT_CONFIG_NV_S));
            }

        }
#if defined(PRODUCT_CFG_SUPPORT_UART2)
        free(pstCfgStart);
#endif
    }
    else if(HI_IS_PRODUCT_Tsta())
    {
        ret = HI_MDM_NV_Read(HI_NV_BSP_IO_REMAP_INIT_CONFIG_ID_4,(HI_PVOID)pIoInitRemapNv,sizeof(HI_IO_REMAP_INIT_CONFIG_NV_S));
        if(ret==HI_ERR_SUCCESS)
        {
            (hi_void)memcpy_s(pIoInitRemapCfg, sizeof(HI_IO_REMAP_INIT_CONFIG_NV_S), pIoInitRemapNv, sizeof(HI_IO_REMAP_INIT_CONFIG_NV_S));
        }

    }
    else if ((product_id >= HI_FTM_PRODUCT_TYPE_VALUE_M111)&&(product_id <= HI_FTM_PRODUCT_TYPE_VALUE_M118))//II采
    {
#if defined(PRODUCT_CFG_SUPPORT_UART2)
        pstCfgStart = (HI_NV_FTM_STARTUP_CFG_S *)malloc(sizeof(HI_NV_FTM_STARTUP_CFG_S));
        if (pstCfgStart == HI_NULL)
        {
            free(pIoInitRemapNv);
            return HI_ERR_MALLOC_FAILUE;
        }

        (hi_void)memset_s((HI_VOID *)pstCfgStart, sizeof(HI_NV_FTM_STARTUP_CFG_S), 0, sizeof(HI_NV_FTM_STARTUP_CFG_S));
        
        ret = HI_MDM_NV_Read(HI_NV_FTM_STARTUP_CFG_ID,(HI_PVOID)pstCfgStart,sizeof(HI_NV_FTM_STARTUP_CFG_S));
        //sta工装
        if ((ret == HI_ERR_SUCCESS)&&(pstCfgStart->ucTestMode == 2))
        {
            ret = HI_MDM_NV_Read(HI_NV_BSP_IO_REMAP_INIT_CONFIG_ID_6,(HI_PVOID)pIoInitRemapNv,sizeof(HI_IO_REMAP_INIT_CONFIG_NV_S));
            if(ret==HI_ERR_SUCCESS)
            {
                (hi_void)memcpy_s(pIoInitRemapCfg, sizeof(HI_IO_REMAP_INIT_CONFIG_NV_S), pIoInitRemapNv, sizeof(HI_IO_REMAP_INIT_CONFIG_NV_S));
            }
        }
        else
#endif
        {
            ret = HI_MDM_NV_Read(HI_NV_BSP_IO_REMAP_INIT_CONFIG_ID_5,(HI_PVOID)pIoInitRemapNv,sizeof(HI_IO_REMAP_INIT_CONFIG_NV_S));           
            if(ret==HI_ERR_SUCCESS)
            {
                (hi_void)memcpy_s(pIoInitRemapCfg, sizeof(HI_IO_REMAP_INIT_CONFIG_NV_S), pIoInitRemapNv, sizeof(HI_IO_REMAP_INIT_CONFIG_NV_S));
            }

        }
#if defined(PRODUCT_CFG_SUPPORT_UART2)
        free(pstCfgStart);
#endif
    }
    else
    {
        ret = HI_ERR_INVALID_PARAMETER;
    }
#endif
    free(pIoInitRemapNv);
    return ret;
}


HI_U32 usr_io_init(HI_VOID)
{
    HI_FTM_PRODUCT_TYPE_E product_id = HI_MDM_GetProductType();
    HI_IO_REMAP_INIT_CONFIG_NV_S  ioInitRemapCfg = {{0}, 0};
    HI_U8 i = 0;
    HI_U32 ret=HI_ERR_SUCCESS;
#ifdef PRODUCT_CFG_PRODUCT_TYPE_NDM
    HI_U8  regVal[8] = {0};
#endif

    if(HI_ERR_SUCCESS != HI_Device_Get_IO_Config_From_Nv(product_id, &ioInitRemapCfg))
    {
        // TODO:
        return HI_ERR_FAILURE;
    }

#ifdef PRODUCT_CFG_BOARD_TYPE_ASIC_V200
    for(i = 0; i < HI_HW_IO_IDX_MAX; i++)
    {
        HI_MDM_IO_SetMux((HI_HW_IO_IDX_E)i, ioInitRemapCfg.ioInitFunc[i]);
    }

    for(i=0; i<32; i++)
    {
        if((ioInitRemapCfg.ioInitDir)& (0x1<<i))
            HI_MDM_IO_SetDir((HI_GPIO_IDX_E)i, HI_GPIO_DIRECTION_OUT);
        else
            HI_MDM_IO_SetDir((HI_GPIO_IDX_E)i, HI_GPIO_DIRECTION_IN);

        if((ioInitRemapCfg.ioInitLvl) & (0x1<<i))
            HI_MDM_IO_SetOutputVal((HI_GPIO_IDX_E)i, HI_GPIO_VALUE1);
        else
            HI_MDM_IO_SetOutputVal((HI_GPIO_IDX_E)i, HI_GPIO_VALUE0);
    }
#else
    for(i=0;i<32;i++)
    {
        HI_MDM_IO_SetMux((HI_GPIO_IDX_E)i, ioInitRemapCfg.ioInitFunc[i]);

        if((ioInitRemapCfg.ioInitDir)& (0x1<<i))
            HI_MDM_IO_SetDir((HI_GPIO_IDX_E)i, HI_GPIO_DIRECTION_OUT);
        else
            HI_MDM_IO_SetDir((HI_GPIO_IDX_E)i, HI_GPIO_DIRECTION_IN);

        if((ioInitRemapCfg.ioInitLvl) & (0x1<<i))
            HI_MDM_IO_SetOutputVal((HI_GPIO_IDX_E)i, HI_GPIO_VALUE1);
        else
            HI_MDM_IO_SetOutputVal((HI_GPIO_IDX_E)i, HI_GPIO_VALUE0);
    }
#endif
#ifdef PRODUCT_CFG_PRODUCT_TYPE_NDM
    /* yuming add for io expander init */
    /* 寄存器0-1为扩展IO输入寄存器 */
    regVal[0] = 0x00;
    regVal[1] = 0x00;

    /* 寄存器2-3为扩展IO输出寄存器 */
    regVal[2] = (HI_U8)(ioInitRemapCfg.ioExpOutput & 0xff);
    regVal[3] = (HI_U8)((ioInitRemapCfg.ioExpOutput & 0xff00) >> 8);

    /* yuming add for io expander init */
    /* 寄存器4-5为扩展IO模式寄存器 */
    regVal[4] = 0x00;
    regVal[5] = 0x00;

    /* 寄存器6-7为扩展IO方向寄存器 */
    regVal[6] = (HI_U8)(ioInitRemapCfg.ioExpDir & 0xff);
    regVal[7] = (HI_U8)((ioInitRemapCfg.ioExpDir & 0xff00) >> 8);
#endif

    /* 初始化扩展IO寄存器 */
    HI_LED_ModeConfig(HI_LED_MODE_HIGH);           //LED高有效
    HI_SWITCH_ModeConfig(HI_SWITCH_MODE_HIGH);     //SWITCH高有效
    HI_IO_IntInit();

#if (defined(PRODUCT_CFG_PRODUCT_TYPE_NDM) && defined(PRODUCT_CFG_SUPPORT_IO_EXP))
    ret = HI_IO_EXP_Init(regVal);
#endif
    /* end */
    return ret;


}

