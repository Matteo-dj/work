/****************************************************************************
*
*                  版权所有 (C), 2001-2015, 华为技术有限公司
*
*****************************************************************************
*  文 件 名   : ioexp.h
*  版 本 号   : V1.0
*  作    者   : 余明/00281951
*  生成日期   : 2015年2月5日
*  功能描述   : IO中间层驱动头文件，提供IO初始化，及IO操作函数
*
*  函数列表   :
*  修改历史   :
*  1.日    期 : 2015年2月5日
*    作    者 : 余明/00281951
*    修改内容 : 创建文件
*
*****************************************************************************/

#ifndef __HI_IOEXP_H__
#define __HI_IOEXP_H__
#include <hi_types.h>
#include <hi_mdm_types.h>

/****************系统头文件***********************
 HI_IO_EXP_Init(HI_U8 *regVal)中参数,regVal:
 ______________________________________________________________________________________
|num|   寄存器功能             | 读写特性       | 默认值   | 备注                      |
|___|__________________________|________________|__________|___________________________|
|0  | regVal 0                 | P0~P7          | 11111111 |输入读取寄存器0,管脚P0~P7  |
|___|__________________________|________________|__________|___________________________|
|1  | regVal 1                 | P10~P17        | 11111111 |输入读取寄存器1,管脚P10~P17|
|___|__________________________|________________|__________|___________________________|
|2  | regVal 2                 | P0~P7          | 11111111 |输出读取寄存器0,管脚P0~P7  |
|___|__________________________|________________|__________|___________________________|
|3  | regVal 3                 | P10~P17        | 11111111 |输出读取寄存器1,管脚P10~P17|
|___|__________________________|________________|__________|___________________________|
|4  | regVal 4                 | P0~P7          | 00000000 |输入极性寄存器0,管脚P0~P7  |
|___|__________________________|________________|__________|___________________________|
|5  | regVal 5                 | P10~P17        | 00000000 |输入极性寄存器1,管脚P10~P17|
|___|__________________________|________________|__________|___________________________|
|6  | regVal 6                 | P0~P7          | 11111111 |方向配置寄存器0,管脚P0~P7  |
|___|__________________________|________________|__________|___________________________|
|7  | regVal 7                 | P10~P17        | 11111111 |方向配置寄存器1,管脚P10~P17|
|___|__________________________|________________|__________|___________________________|

****************系统头文件************************/



typedef enum
{
    IO_EXP_IO_NUM_00,
    IO_EXP_IO_NUM_01,
    IO_EXP_IO_NUM_02,
    IO_EXP_IO_NUM_03,
    IO_EXP_IO_NUM_04,
    IO_EXP_IO_NUM_05,
    IO_EXP_IO_NUM_06,
    IO_EXP_IO_NUM_07,

    IO_EXP_IO_NUM_10,
    IO_EXP_IO_NUM_11,
    IO_EXP_IO_NUM_12,
    IO_EXP_IO_NUM_13,
    IO_EXP_IO_NUM_14,
    IO_EXP_IO_NUM_15,
    IO_EXP_IO_NUM_16,
    IO_EXP_IO_NUM_17,
    IO_EXP_IO_NUM_MAX,
}IO_EXP_IO_NUM_E;

typedef enum
{
    HI_IO_EXP_DIR_IN,
    HI_IO_EXP_DIR_OUT,
    HI_IO_EXP_DIR_MAX,
}HI_IO_EXP_DIR_E;

typedef enum
{
    HI_IO_EXP_OUT_VAL_LOW,
    HI_IO_EXP_OUT_VAL_HIGHT,
    HI_IO_EXP_OUT_VAL_MAX,
}HI_IO_EXP_OUT_VAL_E;
#ifdef PRODUCT_CFG_SUPPORT_IO_EXP
HI_EXTERN HI_U32 HI_IO_EXP_Init(HI_U8 *regVal);
HI_EXTERN HI_U32 HI_IO_EXP_SetDir(IO_EXP_IO_NUM_E offset,HI_IO_EXP_DIR_E val);
HI_EXTERN HI_U32 HI_IO_EXP_GetDir(IO_EXP_IO_NUM_E offset, HI_IO_EXP_DIR_E *regVal);
HI_EXTERN HI_U32 HI_IO_EXP_SetOutPutVal(IO_EXP_IO_NUM_E offset, HI_IO_EXP_OUT_VAL_E val);
HI_EXTERN HI_U32 HI_IO_EXP_GetInputVal(IO_EXP_IO_NUM_E offset, HI_IO_EXP_OUT_VAL_E *regVal);
HI_EXTERN HI_U32 HI_IO_EXP_GetOutputVal(IO_EXP_IO_NUM_E offset, HI_IO_EXP_OUT_VAL_E *regVal);
#endif

#endif//__HI_IOEXP_H__