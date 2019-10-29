/****************************************************************************
*
*                  版权所有 (C), 2001-2015, 华为技术有限公司
*****************************************************************************
*  文 件 名   : ioexp.c
*  版 本 号   : V1.0
*  作    者   : 余明/00281951
*  生成日期   : 2015年2月5日
*  功能描述   : IO中间层驱动
*
*  函数列表   :
*  修改历史   :
*  1.日    期 : 2015年2月5日
*    作    者 : 余明/00281951
*    修改内容 : 创建文件
*
****************************************************************************
说明:
tca9555芯片封闭如下:
         _______
    int-|       |-Vcc
     A1-|       |-SDA
     A2-|       |-SCL
    p00-|       |-A0
    p01-|       |-P17
    p02-|       |-P16
    p03-|       |-P15
    p04-|       |-P14
    p05-|       |-P13
    p06-|       |-P12
    p07-|       |-P11
    GND-|_______|-P10

1、A0~A2为地址配置线，地址为:01000A2A1A0
2、SDA，SCL为I2C总线，接入主芯片的i2c总线
3、VCC接3.3V，GND接地
4、P00~P07为第一组IO，在驱动中ionum为0~7
   P10~P17为第二组IO，在驱动中ionum为8~15
5、int为中断管脚，由于GPIO驱动不方便在底层调用，将中断注册放在中间层。中断处理函数底半部目前待定。

IO扩展芯片共有8个寄存器，各寄存器的值，及寄存器对应的功能如下表:
HI_IO_EXP_Init的参数regVal[0]~regVal[7]对应写入到寄存器0~7的值.
 ______________________________________________________________________________________
|num|   寄存器功能             | 读写特性       | 默认值   | 备注                      |
|___|__________________________|________________|__________|___________________________|
|0  | Input Port 0             | Read byte      | 11111111 |输入读取寄存器0,管脚P0~P7  |
|___|__________________________|________________|__________|___________________________|
|1  | Input Port 1             | Read byte      | 11111111 |输入读取寄存器1,管脚P10~P17|
|___|__________________________|________________|__________|___________________________|
|2  | Output Port 0            | Read/write byte| 11111111 |输出读取寄存器0,管脚P0~P7  |
|___|__________________________|________________|__________|___________________________|
|3  | Output Port 0            | Read/write byte| 11111111 |输出读取寄存器1,管脚P10~P17|
|___|__________________________|________________|__________|___________________________|
|4  | Polarity Inversion Port 0| Read/write byte| 00000000 |输入极性寄存器0,管脚P0~P7  |
|___|__________________________|________________|__________|___________________________|
|5  | Polarity Inversion Port 0| Read/write byte| 00000000 |输入极性寄存器1,管脚P10~P17|
|___|__________________________|________________|__________|___________________________|
|6  | Configuration Port 0     | Read/write byte| 11111111 |方向配置寄存器0,管脚P0~P7  |
|___|__________________________|________________|__________|___________________________|
|7  | Configuration Port 0     | Read/write byte| 11111111 |方向配置寄存器1,管脚P10~P17|
|___|__________________________|________________|__________|___________________________|
注:
对于每一个IO管脚:
1、输入在方向配置为输入时，极性寄存器设置为0时，输入高电平时，寄存器为1，输入低电平时，寄存器为0。
2、输出在方向配置为输出时，设置为1，则输出高电平，设置为0，则输出为低电平。
3、极性设置寄存器仅对输入有效，目前不使用。
4、方向配置寄存器为1时，相应管脚配置为输入，寄存器为0时，相应管脚配置为输出。

对于目前的应用,仅为输出，推荐值为:
regVal[0] = 0x00,regVal[1] = 0x00,regVal[2] = 0x04,regVal[3] = 0x01,
regVal[4] = 0x00,regVal[5] = 0x00,regVal[6] = 0x0f,regVal[7] = 0xf8,
*****************************************************************************/

/*includes*/
#include <hi_types.h>
#include <hi_driver.h>

#ifdef PRODUCT_CFG_SUPPORT_IO_EXP
#include "hi_ioexp.h"
#include "tca9555.h"

/************************************分界线*******************************************/
/*defines*/
#define REG_NUM      8
#define DEV_NAME    "/tca9555"

#define OUTPUT                      0
#define INPUT                       1



/*****************************************************************************
 函 数 名  : HI_IO_Exp_Init
 功能描述  : 初始化扩展IO芯片
 输入参数  : HI_U8 *regVal
 输出参数  : 无
 返 回 值  : HI_U32
 本函数调用的函数  :
 调用本函数的函数  :
 访问的全局变量  :
 修改的全局变量  :
 修改的BBP寄存器列表  :

 修改历史      :
  1.日    期   : 2015年2月5日
    作    者   : yuming
    修改内容   : 新生成函数

*****************************************************************************/
HI_U32  HI_IO_EXP_Init(HI_U8* regVal)
{

    HI_SSiZE_T realLen = 0;
    HI_I2C_Init(HI_I2C_IDX_0);
    realLen = ioexpWrite(0, (HI_CHAR *)regVal, REG_NUM);

    if ((-1) == realLen)
    {
        return HI_ERR_FAILURE;
    }

    return HI_ERR_SUCCESS;

}

/*****************************************************************************
 函 数 名  : HI_IO_EXP_SetDir
 功能描述  : 单个IO管脚方向设置
 输入参数  : HI_U8 offset   ------0~7为IO00~IO07
                                8~15为IO10~IO17
             HI_BOOL val    ------0输出，1为输入
 输出参数  : 无
 返 回 值  : HI_U32
 本函数调用的函数  :
 调用本函数的函数  :
 访问的全局变量  :
 修改的全局变量  :
 修改的BBP寄存器列表  :

 修改历史      :
  1.日    期   : 2015年2月5日
    作    者   : yuming
    修改内容   : 新生成函数

*****************************************************************************/
HI_U32  HI_IO_EXP_SetDir(IO_EXP_IO_NUM_E offset, HI_IO_EXP_DIR_E val)
{
    if (( offset >= IO_EXP_IO_NUM_MAX ) || (val >= HI_IO_EXP_DIR_MAX))
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    if (val)
    {
        return (HI_U32)ioexpIoctl(0, FIO_SET_DIR_OUTPUT, offset);
    }
    else
    {
        return (HI_U32)ioexpIoctl(0, FIO_SET_DIR_INPUT, offset);
    }

}

/*****************************************************************************
 函 数 名  : HI_IO_EXP_GetDir
 功能描述  : 单个IO管脚方向获取
 输入参数  : HI_U8 offset   ------0~7为IO00~IO07
                                8~15为IO10~IO17
 输出参数  : 无
 返 回 值  : HI_U32
 本函数调用的函数  :
 调用本函数的函数  :
 访问的全局变量  :
 修改的全局变量  :
 修改的BBP寄存器列表  :

 修改历史      :
  1.日    期   : 2015年2月5日
    作    者   : yuming
    修改内容   : 新生成函数

*****************************************************************************/
HI_U32  HI_IO_EXP_GetDir(IO_EXP_IO_NUM_E offset, HI_IO_EXP_DIR_E* regVal)
{
    HI_IO_EXP_DIR_E tmp_dir;

    if (( offset >= IO_EXP_IO_NUM_MAX ) || (regVal == HI_NULL))
    { 
        return HI_ERR_INVALID_PARAMETER; 
    }

    tmp_dir = (HI_IO_EXP_DIR_E)ioexpIoctl(0, FIO_GET_DIR, offset);

    if (!tmp_dir)
    {
        *regVal = HI_IO_EXP_DIR_OUT;
    }
    else
    {
        *regVal = HI_IO_EXP_DIR_IN;
    }

    return HI_ERR_SUCCESS;

}

/*****************************************************************************
 函 数 名  : HI_IO_EXP_SetOutPutVal
 功能描述  : 单个IO管脚电平设置
 输入参数  : HI_U8 offset   ------0~7为IO00~IO07
                                8~15为IO10~IO17
             HI_BOOL val    ------0输出，1为输入
 输出参数  : 无
 返 回 值  : HI_U32
 本函数调用的函数  :
 调用本函数的函数  :
 访问的全局变量  :
 修改的全局变量  :
 修改的BBP寄存器列表  :

 修改历史      :
  1.日    期   : 2015年2月5日
    作    者   : yuming
    修改内容   : 新生成函数

*****************************************************************************/
HI_U32  HI_IO_EXP_SetOutPutVal(IO_EXP_IO_NUM_E offset, HI_IO_EXP_OUT_VAL_E val)
{
	HI_U32 ioDir = INPUT;

	if ((offset >= IO_EXP_IO_NUM_MAX ) || (val >= HI_IO_EXP_OUT_VAL_MAX))
	{ return HI_ERR_INVALID_PARAMETER; }


	/* value is ok only when the direction is output */
	ioDir = (HI_U32)ioexpIoctl(0, FIO_GET_DIR, offset);

	if (ioDir != OUTPUT)
	{
		return HI_ERR_NOT_SUPPORT;
	}

	if (val)
	{
		return (HI_U32)ioexpIoctl(0, FIO_SET_OUT_VAL_HIGH, offset);
	}
	else
	{
		return (HI_U32)ioexpIoctl(0, FIO_SET_OUT_VAL_LOW, offset);

	}

}

/*****************************************************************************
 函 数 名  : HI_IO_EXP_SetOutPutVal
 功能描述  : 单个IO管脚电平获取
 输入参数  : HI_U8 offset   ------0~7为IO00~IO07
                                  8~15为IO10~IO17
 输出参数  : 无
 返 回 值  : HI_U32
 本函数调用的函数  :
 调用本函数的函数  :
 访问的全局变量  :
 修改的全局变量  :
 修改的BBP寄存器列表  :

 修改历史      :
  1.日    期   : 2015年2月5日
    作    者   : yuming
    修改内容   : 新生成函数

*****************************************************************************/
HI_U32 HI_IO_EXP_GetInputVal(IO_EXP_IO_NUM_E offset, HI_IO_EXP_OUT_VAL_E* regVal)
{
    if (( offset >= IO_EXP_IO_NUM_MAX ) || (regVal == HI_NULL))
    { return HI_ERR_INVALID_PARAMETER; }

    /* get the value */
    *regVal = (HI_IO_EXP_OUT_VAL_E)ioexpIoctl(0, FIO_GET_INPUT_VAL, offset);
    return HI_ERR_SUCCESS;
}

HI_U32 HI_IO_EXP_GetOutputVal(IO_EXP_IO_NUM_E offset, HI_IO_EXP_OUT_VAL_E* regVal)
{

    if (( offset >= IO_EXP_IO_NUM_MAX ) || (regVal == HI_NULL))
    { 
        return HI_ERR_INVALID_PARAMETER; 
    }


    /* get the value */
    *regVal = (HI_IO_EXP_OUT_VAL_E)ioexpIoctl(0, FIO_GET_OUT_VAL, offset);
    return HI_ERR_SUCCESS;
}

#endif//PRODUCT_CFG_SUPPORT_IO_EXP