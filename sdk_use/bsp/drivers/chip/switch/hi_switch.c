/***********************************************************************************
*              Copyright 2004 - 2050, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName:   hal_gpio.c
* Description:
*
* History:
* Version   Date                Author          DefectNum     Description
* 1.0        2015-07-17        y00281951         NULL             Create this file.
***********************************************************************************/

#include <hi_types.h>
#include "hi_driver.h"
#include <hi_platform.h>

#define HI_SWITCH_GLITTER_CLOCK_BASE  HI_XTAL_CLOCK_BASE

//reg_offset
#define HI_SWITCH_PH_CTRL             (0x00)
#define HI_SWITCH_PH_START            (0x64)

typedef enum
{
    HI_SWITCH_STATE_OFF = 0,
    HI_SWITCH_STATE_ON,
    HI_SWITCH_STATE_LOOP = 3,
    HI_SWITCH_STATE_MAX,
}HI_SWITCH_STATE_E;


/*config high or low effect*/
HI_VOID HI_SWITCH_ModeConfig(HI_SWITCH_MODE_E enModeType)
{
    /* LED low or high effect */
    HI_U32 ulDirVal = 0;
    
    HI_REG_READ32((HI_SWITCH_REG_BASE + HI_SWITCH_PH_CTRL), ulDirVal);
    if (HI_SWITCH_MODE_HIGH == enModeType)
    {
        ulDirVal &= (~(HI_U32)(0xE0 << 16));//led1-5,switch1-3,total:8bit
    }
    else
    {
/*
                    【屏蔽人】z00193780
                    【审核人】c00233580
                    【代码分析】二次开发接口，客户有可能使用该分支，需保留。
                    【功能影响】不影响正常业务，屏蔽告警
*/
        ulDirVal |= (0xE0 << 16);
    }
    HI_REG_WRITE32((HI_SWITCH_REG_BASE + HI_SWITCH_PH_CTRL), ulDirVal);

    /* enable switch */
    HI_REG_WRITE32((HI_SWITCH_REG_BASE + HI_SWITCH_PH_START), 0x01);
}

HI_PRV HI_U32 switch_ctrl(HI_SWITCH_IDX_E enId, HI_SWITCH_STATE_E enState)
{
    HI_U32 ulDirVal = 0;

    if((enId >= HI_SWITCH_IDX_MAX) || (enState >= HI_SWITCH_STATE_MAX))
    {
        return HI_ERR_LED_PARAMETER_ERROR;
    }

    /* config switch num */
    HI_REG_READ32((HI_SWITCH_REG_BASE + HI_SWITCH_PH_CTRL) , ulDirVal);
    ulDirVal &= ( ~(HI_U32)(0x3 << ((HI_U32)(enId + 0x05) << 1 ) ) );
    ulDirVal |= ((HI_U32)enState << ((HI_U32)(enId + 0x05) << 1));//exp:switch1 = bit[11:10]
    HI_REG_WRITE32((HI_SWITCH_REG_BASE + HI_SWITCH_PH_CTRL), ulDirVal);

    /* enable switch */
    HI_REG_WRITE32((HI_SWITCH_REG_BASE + HI_SWITCH_PH_START), 0x01);
    return HI_ERR_SUCCESS;
}

HI_U32 HI_SWITCH_On(HI_SWITCH_IDX_E enId)
{
    return switch_ctrl(enId, HI_SWITCH_STATE_ON);
}

HI_U32 HI_SWITCH_Off(HI_SWITCH_IDX_E enId)
{
    return switch_ctrl(enId, HI_SWITCH_STATE_OFF);
}

HI_U32 HI_SWITCH_Loop(HI_SWITCH_IDX_E enId,HI_U32 ulOnTime,HI_U32 ulOffTime)
{
    HI_U32 ulLigReg = 0;

    /* check parameters */
    if((ulOnTime < 1)||(ulOnTime > (((((HI_U32)0x1<<31) - 1)) / (1000*HI_SWITCH_GLITTER_CLOCK_BASE)))
        ||(ulOffTime < 1)||(ulOffTime > (((((HI_U32)0x1<<31) - 1)) / (1000*HI_SWITCH_GLITTER_CLOCK_BASE)))
        || (enId >= HI_SWITCH_IDX_MAX))
    {
        return HI_ERR_LED_PARAMETER_ERROR;
    }

    ulOnTime *= (HI_U32)(1000 * HI_SWITCH_GLITTER_CLOCK_BASE);  //us->ms
    ulOffTime *= (HI_U32)(1000 * HI_SWITCH_GLITTER_CLOCK_BASE);

    ulLigReg = (((HI_U32)enId + HI_LED_IDX_MAX)) * 0x0c;
    HI_REG_WRITE32((HI_SWITCH_REG_BASE + ulLigReg + 0x04), ulOnTime);
    HI_REG_WRITE32((HI_SWITCH_REG_BASE + ulLigReg + 0x08), ulOffTime);
    return switch_ctrl(enId, HI_SWITCH_STATE_LOOP);
}

