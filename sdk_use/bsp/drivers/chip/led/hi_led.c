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
#include <hi_platform.h>
#include <hi_driver.h>

#define HI_LED_GLITTER_CLOCK_BASE  HI_XTAL_CLOCK_BASE

//reg_offset
#define HI_LED_PH_CTRL             (0x00)
#define HI_LED_PH_START            (0x64)

typedef enum
{
    HI_LED_STATE_OFF = 0,
    HI_LED_STATE_ON = 1,
    HI_LED_STATE_GLITTER = 3,
    HI_LED_STATE_MAX,
}HI_LED_STATE_E;


/*config high or low effect*/
HI_VOID HI_LED_ModeConfig(HI_LED_MODE_E enModeType)
{
    /* LED low or high effect */
    HI_U32 ulDirVal = 0;

    HI_REG_READ32((HI_LED_REG_BASE + HI_LED_PH_CTRL) , ulDirVal);
    if (HI_LED_MODE_HIGH == enModeType)
    {
        //led1-5,switch1-3,total:8bit
        ulDirVal &= (~(HI_U32)(0x1f << 16));
    }
    else
    {
/*
                    【屏蔽人】z00193780
                    【审核人】c00233580
                    【代码分析】二次开发接口，客户有可能使用该分支，需保留。
                    【功能影响】不影响正常业务，屏蔽告警
*/        
        ulDirVal |= (0x1f << 16);
    }
    HI_REG_WRITE32((HI_LED_REG_BASE + HI_LED_PH_CTRL) , ulDirVal);
    
    /* enable light */
    HI_REG_WRITE32((HI_LED_REG_BASE + HI_LED_PH_START) , 0x01);
}

HI_U32 led_ctrl(HI_LED_IDX_E enLedId , HI_LED_STATE_E enState)
{
    HI_U32 ulDirVal = 0;

    if((enLedId >= HI_LED_IDX_MAX) || (enState >= HI_LED_STATE_MAX))
    {
        return HI_ERR_LED_PARAMETER_ERROR;
    }

    /* config light num */
    HI_REG_READ32((HI_LED_REG_BASE + HI_LED_PH_CTRL) , ulDirVal);
    ulDirVal &= ( ~(HI_U32)(0x3 << ((HI_U32)enLedId << 1 ) ) );
    ulDirVal |= ((HI_U32)enState << ((HI_U32)enLedId << 1));//exp:led1 = bit[1:0]
    HI_REG_WRITE32((HI_LED_REG_BASE + HI_LED_PH_CTRL) , ulDirVal);

    /* enable light */
    HI_REG_WRITE32((HI_LED_REG_BASE + HI_LED_PH_START) , 0x01);
    return HI_ERR_SUCCESS;
}

HI_U32 HI_LED_On(HI_LED_IDX_E enLedId)
{
    return led_ctrl(enLedId, HI_LED_STATE_ON);
}

HI_U32 HI_LED_Off(HI_LED_IDX_E enLedId)
{
    return led_ctrl(enLedId, HI_LED_STATE_OFF);
}

HI_U32 HI_LED_Glitter(HI_LED_IDX_E enLedId , HI_U32 ulOnTime, HI_U32 ulOffTime)
{
    HI_U32 ulLigReg = 0;

    /* check parameters */
    if ((ulOnTime < 1)
        ||(ulOnTime > (((((HI_U32)1 << 31) - 1)) / (1000 * HI_LED_GLITTER_CLOCK_BASE)))
        ||(ulOffTime < 1)
        ||(ulOffTime > (((((HI_U32)1 << 31) - 1)) / (1000 * HI_LED_GLITTER_CLOCK_BASE)))
        ||(enLedId >= HI_LED_IDX_MAX))
    {
        return HI_ERR_LED_PARAMETER_ERROR;
    }

    ulOnTime *= (HI_U32)(1000*HI_LED_GLITTER_CLOCK_BASE); //us->ms
    ulOffTime *= (HI_U32)(1000*HI_LED_GLITTER_CLOCK_BASE);

    ulLigReg = enLedId * 0x0c;
    HI_REG_WRITE32((HI_LED_REG_BASE + ulLigReg + 0x04), ulOnTime);
    HI_REG_WRITE32((HI_LED_REG_BASE + ulLigReg + 0x08), ulOffTime);
    return led_ctrl(enLedId, HI_LED_STATE_GLITTER);
}

