/****************************************************************************
*
*                  ��Ȩ���� (C), 2001-2015, ��Ϊ�������޹�˾
*****************************************************************************
*  �� �� ��   : ioexp.c
*  �� �� ��   : V1.0
*  ��    ��   : ����/00281951
*  ��������   : 2015��2��5��
*  ��������   : IO�м������
*
*  �����б�   :
*  �޸���ʷ   :
*  1.��    �� : 2015��2��5��
*    ��    �� : ����/00281951
*    �޸����� : �����ļ�
*
****************************************************************************
˵��:
tca9555оƬ�������:
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

1��A0~A2Ϊ��ַ�����ߣ���ַΪ:01000A2A1A0
2��SDA��SCLΪI2C���ߣ�������оƬ��i2c����
3��VCC��3.3V��GND�ӵ�
4��P00~P07Ϊ��һ��IO����������ionumΪ0~7
   P10~P17Ϊ�ڶ���IO����������ionumΪ8~15
5��intΪ�жϹܽţ�����GPIO�����������ڵײ���ã����ж�ע������м�㡣�жϴ������װ벿Ŀǰ������

IO��չоƬ����8���Ĵ��������Ĵ�����ֵ�����Ĵ�����Ӧ�Ĺ������±�:
HI_IO_EXP_Init�Ĳ���regVal[0]~regVal[7]��Ӧд�뵽�Ĵ���0~7��ֵ.
 ______________________________________________________________________________________
|num|   �Ĵ�������             | ��д����       | Ĭ��ֵ   | ��ע                      |
|___|__________________________|________________|__________|___________________________|
|0  | Input Port 0             | Read byte      | 11111111 |�����ȡ�Ĵ���0,�ܽ�P0~P7  |
|___|__________________________|________________|__________|___________________________|
|1  | Input Port 1             | Read byte      | 11111111 |�����ȡ�Ĵ���1,�ܽ�P10~P17|
|___|__________________________|________________|__________|___________________________|
|2  | Output Port 0            | Read/write byte| 11111111 |�����ȡ�Ĵ���0,�ܽ�P0~P7  |
|___|__________________________|________________|__________|___________________________|
|3  | Output Port 0            | Read/write byte| 11111111 |�����ȡ�Ĵ���1,�ܽ�P10~P17|
|___|__________________________|________________|__________|___________________________|
|4  | Polarity Inversion Port 0| Read/write byte| 00000000 |���뼫�ԼĴ���0,�ܽ�P0~P7  |
|___|__________________________|________________|__________|___________________________|
|5  | Polarity Inversion Port 0| Read/write byte| 00000000 |���뼫�ԼĴ���1,�ܽ�P10~P17|
|___|__________________________|________________|__________|___________________________|
|6  | Configuration Port 0     | Read/write byte| 11111111 |�������üĴ���0,�ܽ�P0~P7  |
|___|__________________________|________________|__________|___________________________|
|7  | Configuration Port 0     | Read/write byte| 11111111 |�������üĴ���1,�ܽ�P10~P17|
|___|__________________________|________________|__________|___________________________|
ע:
����ÿһ��IO�ܽ�:
1�������ڷ�������Ϊ����ʱ�����ԼĴ�������Ϊ0ʱ������ߵ�ƽʱ���Ĵ���Ϊ1������͵�ƽʱ���Ĵ���Ϊ0��
2������ڷ�������Ϊ���ʱ������Ϊ1��������ߵ�ƽ������Ϊ0�������Ϊ�͵�ƽ��
3���������üĴ�������������Ч��Ŀǰ��ʹ�á�
4���������üĴ���Ϊ1ʱ����Ӧ�ܽ�����Ϊ���룬�Ĵ���Ϊ0ʱ����Ӧ�ܽ�����Ϊ�����

����Ŀǰ��Ӧ��,��Ϊ������Ƽ�ֵΪ:
regVal[0] = 0x00,regVal[1] = 0x00,regVal[2] = 0x04,regVal[3] = 0x01,
regVal[4] = 0x00,regVal[5] = 0x00,regVal[6] = 0x0f,regVal[7] = 0xf8,
*****************************************************************************/

/*includes*/
#include <hi_types.h>
#include <hi_driver.h>

#ifdef PRODUCT_CFG_SUPPORT_IO_EXP
#include "hi_ioexp.h"
#include "tca9555.h"

/************************************�ֽ���*******************************************/
/*defines*/
#define REG_NUM      8
#define DEV_NAME    "/tca9555"

#define OUTPUT                      0
#define INPUT                       1



/*****************************************************************************
 �� �� ��  : HI_IO_Exp_Init
 ��������  : ��ʼ����չIOоƬ
 �������  : HI_U8 *regVal
 �������  : ��
 �� �� ֵ  : HI_U32
 ���������õĺ���  :
 ���ñ������ĺ���  :
 ���ʵ�ȫ�ֱ���  :
 �޸ĵ�ȫ�ֱ���  :
 �޸ĵ�BBP�Ĵ����б�  :

 �޸���ʷ      :
  1.��    ��   : 2015��2��5��
    ��    ��   : yuming
    �޸�����   : �����ɺ���

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
 �� �� ��  : HI_IO_EXP_SetDir
 ��������  : ����IO�ܽŷ�������
 �������  : HI_U8 offset   ------0~7ΪIO00~IO07
                                8~15ΪIO10~IO17
             HI_BOOL val    ------0�����1Ϊ����
 �������  : ��
 �� �� ֵ  : HI_U32
 ���������õĺ���  :
 ���ñ������ĺ���  :
 ���ʵ�ȫ�ֱ���  :
 �޸ĵ�ȫ�ֱ���  :
 �޸ĵ�BBP�Ĵ����б�  :

 �޸���ʷ      :
  1.��    ��   : 2015��2��5��
    ��    ��   : yuming
    �޸�����   : �����ɺ���

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
 �� �� ��  : HI_IO_EXP_GetDir
 ��������  : ����IO�ܽŷ����ȡ
 �������  : HI_U8 offset   ------0~7ΪIO00~IO07
                                8~15ΪIO10~IO17
 �������  : ��
 �� �� ֵ  : HI_U32
 ���������õĺ���  :
 ���ñ������ĺ���  :
 ���ʵ�ȫ�ֱ���  :
 �޸ĵ�ȫ�ֱ���  :
 �޸ĵ�BBP�Ĵ����б�  :

 �޸���ʷ      :
  1.��    ��   : 2015��2��5��
    ��    ��   : yuming
    �޸�����   : �����ɺ���

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
 �� �� ��  : HI_IO_EXP_SetOutPutVal
 ��������  : ����IO�ܽŵ�ƽ����
 �������  : HI_U8 offset   ------0~7ΪIO00~IO07
                                8~15ΪIO10~IO17
             HI_BOOL val    ------0�����1Ϊ����
 �������  : ��
 �� �� ֵ  : HI_U32
 ���������õĺ���  :
 ���ñ������ĺ���  :
 ���ʵ�ȫ�ֱ���  :
 �޸ĵ�ȫ�ֱ���  :
 �޸ĵ�BBP�Ĵ����б�  :

 �޸���ʷ      :
  1.��    ��   : 2015��2��5��
    ��    ��   : yuming
    �޸�����   : �����ɺ���

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
 �� �� ��  : HI_IO_EXP_SetOutPutVal
 ��������  : ����IO�ܽŵ�ƽ��ȡ
 �������  : HI_U8 offset   ------0~7ΪIO00~IO07
                                  8~15ΪIO10~IO17
 �������  : ��
 �� �� ֵ  : HI_U32
 ���������õĺ���  :
 ���ñ������ĺ���  :
 ���ʵ�ȫ�ֱ���  :
 �޸ĵ�ȫ�ֱ���  :
 �޸ĵ�BBP�Ĵ����б�  :

 �޸���ʷ      :
  1.��    ��   : 2015��2��5��
    ��    ��   : yuming
    �޸�����   : �����ɺ���

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