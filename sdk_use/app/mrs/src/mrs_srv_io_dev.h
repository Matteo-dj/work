//*****************************************************************************
//
//                  版权所有 (C), 1998-2012, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_srv_io_dev.h
//  版 本 号   : V2.0 
//  作    者   : fengxiaomin/00209182
//  生成日期   : 2012-07-17
//  功能描述   : TODO: ...
//               
//  函数列表   : TODO: ...
//  修改历史   : 
//  1.日    期 : 
//    作    者 : 
//    修改内容 : 创建文件 
//
//  2.日    期 : 2013年04月12日
//    作    者 : cuiate/00233580
//    修改内容 : 重构 
//
//*****************************************************************************

//*****************************************************************************
// PROJECT   : 
// SUBSYSTEM :
// MODULE    :  
// OWNER     :  
//*****************************************************************************

#ifndef __MRS_SRV_IO_DEV_H__
#define __MRS_SRV_IO_DEV_H__


#include <hi_driver.h>

/* 为了兼容，本模块不使用 */
#define FOOTPRINT_GPIO_AREA_ZERO           HI_GPIO_0 //电表端SET管脚所在GPIO口
#define FOOTPRINT_GPIO_AREA_ONE            HI_GPIO_1 //CCO端SET管脚所在GPIO口
#define FOOTPRINT_GPIO_AREA_TWO            HI_GPIO_2
#define FOOTPRINT_GPIO_AREA_THREE          HI_GPIO_3
#define FOOTPRINT_GPIO_BIT_ZERO            HI_GPIO_BIT0 //EVENT_OUT管脚所在GPIO口的位
#define FOOTPRINT_GPIO_BIT_ONE             HI_GPIO_BIT1 //EVENT_OUT管脚所在GPIO口的位
#define FOOTPRINT_GPIO_BIT_TWO             HI_GPIO_BIT2 //EVENT_OUT管脚所在GPIO口的位
#define FOOTPRINT_GPIO_BIT_THREE           HI_GPIO_BIT3 //EVENT_OUT管脚所在GPIO口的位
#define FOOTPRINT_GPIO_BIT_SEVEN           HI_GPIO_BIT7 //电表端SET管脚所在GPIO口
#define FOOTPRINT_GPIO_VALUE_HIGH          HI_GPIO_VALUE1 //管脚为高电平
#define FOOTPRINT_GPIO_VALUE_LOW           HI_GPIO_VALUE0 //管脚为低电平
#define FOOTPRINT_SENSE_LEVEL_TYPE         HI_SENSE_LEVEL //电平触发
#define FOOTPRINT_SENSE_EDGE_TYPE          HI_SENSE_EDGE  //边沿触发
#define FOOTPRINT_RISE_FALL_VALUE_HIGH     HI_EDGE_RISE_LEVEL_HIGH //上升边缘或高电平
#define FOOTPRINT_RISE_FALL_VALUE_LOW      HI_EDGE_FALL_LEVEL_LOW //下降边缘

#endif //__MRS_SRV_IO_DEV_H__


