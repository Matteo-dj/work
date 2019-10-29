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
#include "hi_sal.h"
#include <os_vx_sal_api.h>
#include "asm/hal_platform_ints.h"
#include <Hi_driver.h >

#define GPIO_FUN_NUM_MAX        8
#define GPIO_PHY_BASE           0x101E3000
#define GPIO_SEL_BASE           0x101E6000
#define GPIO_SWPORT_DR          0x00
#define GPIO_SWPORT_DDR         0x04
#define GPIO_INTEN              0x30
#define GPIO_INTMASK            0x34
#define GPIO_INTSTATUS          0x40
#define GPIO_RAWINTSTATUS       0x44
#define GPIO_INTTYPE_LEVEL      0x38
#define GPIO_INT_POLARITY       0x3c
#define GPIO_PORT_EOI           0x4c
#define GPIO_EXT_PORT           0x50

typedef struct
{
    HI_S32 (*gpio_callback)(HI_U32 ulCallbackArgv);
    HI_U32 ulCallbackArgv;
} HI_GPIO_INTER_CTRL_S;

//��Ҫ��LISR�лص��û�������GPIO
HI_PRV HI_U32 s_ulUserCallbackInLisr = 0;
HI_PRV HI_GPIO_INTER_CTRL_S s_astInterCtrl[HI_GPIO_IDX_MAX];
HI_PRV HI_GPIO_INTER_CTRL_S *s_pstIntCtrl = HI_NULL;
HI_PRV HI_BOOL s_bInitFlag = HI_FALSE;

HI_U32 HI_IO_SetMux(HI_HW_IO_IDX_E enIoId, HI_U8 ulIoVal)
{
	HI_U32 ulReg = 0;

	if ((enIoId >= HI_HW_IO_IDX_MAX) || (ulIoVal >= GPIO_FUN_NUM_MAX))
	{
		return HI_ERR_INVALID_PARAMETER;
	}

	if (enIoId <= HI_HW_SFC_HOLDN_IO25)
	{
		ulReg = GPIO_SEL_BASE + (HI_U32)((HI_U32)enIoId << 2);
	}

	// HI_HW_ETXD0_IO26�ܽ���ǰһ���ܽŸ��üĴ�������1����ַ
	else if ((enIoId > HI_HW_SFC_HOLDN_IO25) && (enIoId < HI_HW_ECRS_IO16_BACKUP))
	{
		ulReg = GPIO_SEL_BASE + (HI_U32)(((HI_U32)enIoId + 1) << 2);
	}
	// HI_HW_ECRS_IO16_BACKUP�ܽ���ǰһ���ܽŸ��üĴ�������25����ַ
	else if (enIoId >= HI_HW_ECRS_IO16_BACKUP)
	{
		ulReg = GPIO_SEL_BASE + (HI_U32)((HI_U32)((HI_U32)enIoId + 24 + 1) << 2);
	}

	HI_REG_WRITE32(ulReg, (HI_U32)ulIoVal);/*lint !e613*/
	return HI_ERR_SUCCESS;
}

HI_U32 HI_IO_GetMux(HI_HW_IO_IDX_E enIoId, HI_U8 *pIoVal)
{
	HI_U32 ulReg = 0;
	HI_U32 ulRegVal = 0;

	if ((enIoId >= HI_HW_IO_IDX_MAX) || (HI_NULL == pIoVal))
	{
		return HI_ERR_INVALID_PARAMETER;
	}
	
	if (enIoId <= HI_HW_SFC_HOLDN_IO25)
	{
		ulReg = GPIO_SEL_BASE + (HI_U32)((HI_U32)enIoId << 2);
	}

	// HI_HW_ETXD0_IO26�ܽ���ǰһ���ܽŸ��üĴ�������1����ַ
	else if ((enIoId > HI_HW_SFC_HOLDN_IO25) && (enIoId < HI_HW_ECRS_IO16_BACKUP))
	{
		ulReg = GPIO_SEL_BASE + (HI_U32)(((HI_U32)enIoId + 1) << 2);
	}
	// HI_HW_ECRS_IO16_BACKUP�ܽ���ǰһ���ܽŸ��üĴ�������25����ַ
	else if (enIoId >= HI_HW_ECRS_IO16_BACKUP)
	{
		ulReg = GPIO_SEL_BASE + (HI_U32)(((HI_U32)enIoId + 24 + 1) << 2);
	}

	HI_REG_READ32(ulReg, ulRegVal);/*lint !e613*/
	*pIoVal = (HI_U8)ulRegVal;
	return HI_ERR_SUCCESS;
}

/*****************************************************************************
 �� �� ��  : HI_IO_SetDir
 ��������  : ���õ���IO�ܽŷ���
 �������  : HI_U8 offset   ------0~32ΪIO00~IO32
             HI_BOOL val    ------����0Ϊ���룬1Ϊ���

 �������  : ��
 �� �� ֵ  : HI_U32
 ���������õĺ���  :
 ���ñ������ĺ���  :
 ���ʵ�ȫ�ֱ���  :
 �޸ĵ�ȫ�ֱ���  :
 �޸ĵ�BBP�Ĵ����б�  :

 �޸���ʷ      :
  1.��    ��   : 2015��7��17��
    ��    ��   : yuming
    �޸�����   : �����ɺ���

*****************************************************************************/

HI_U32  HI_IO_SetDir(HI_GPIO_IDX_E enId, HI_GPIO_DIRECTION_E enDir)
{
    HI_U32 ulDirVal = 0;
    HI_U32 lock_stat = 0;
    if(enId >= HI_GPIO_IDX_MAX)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    lock_stat = HI_MDM_INT_Lock();
    HI_REG_READ32((GPIO_PHY_BASE + GPIO_SWPORT_DDR), ulDirVal);
    
    if(enDir)
    {
        ulDirVal |= (HI_U32)(1 << (HI_U32)enId);
    }
    else
    {
        ulDirVal &= ~(HI_U32)(1 << (HI_U32)enId);
    }
    
    HI_REG_WRITE32((GPIO_PHY_BASE + GPIO_SWPORT_DDR), ulDirVal);
    
    HI_MDM_INT_Restore(lock_stat);
    
    return HI_ERR_SUCCESS;
}

/*****************************************************************************
 �� �� ��  : HI_IO_Get_Dir
 ��������  : ���õ���IO�ܽŷ���
 �������  : HI_U8 offset   ------0~32ΪIO00~IO32
             HI_BOOL* val    ------����

 �������  : ��
 �� �� ֵ  : HI_U32
 ���������õĺ���  :
 ���ñ������ĺ���  :
 ���ʵ�ȫ�ֱ���  :
 �޸ĵ�ȫ�ֱ���  :
 �޸ĵ�BBP�Ĵ����б�  :

 �޸���ʷ      :
  1.��    ��   : 2015��7��17��
    ��    ��   : yuming
    �޸�����   : �����ɺ���

*****************************************************************************/
HI_U32  HI_IO_GetDir(HI_GPIO_IDX_E enId, HI_GPIO_DIRECTION_E *pDir)
{
    HI_U32 ulDirVal = 0;

    if((enId >= HI_GPIO_IDX_MAX) || (pDir == HI_NULL))
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    HI_REG_READ32((GPIO_PHY_BASE + GPIO_SWPORT_DDR), ulDirVal);
    if(ulDirVal & (HI_U32)(1 << (HI_U32)enId))
    {
        *pDir = HI_GPIO_DIRECTION_OUT;
    }
    else
    {
        *pDir = HI_GPIO_DIRECTION_IN;
    }

    return HI_ERR_SUCCESS;
}

/*****************************************************************************
 �� �� ��  : HI_IO_SetOutputVal
 ��������  : ���õ���IO�ܽ����ֵ
 �������  : HI_U8 offset   ------0~32ΪIO00~IO32
             HI_BOOL val    ------����0Ϊ����ͣ�1Ϊ�����

 �������  : ��
 �� �� ֵ  : HI_U32
 ���������õĺ���  :
 ���ñ������ĺ���  :
 ���ʵ�ȫ�ֱ���  :
 �޸ĵ�ȫ�ֱ���  :
 �޸ĵ�BBP�Ĵ����б�  :

 �޸���ʷ      :
  1.��    ��   : 2015��7��17��
    ��    ��   : yuming
    �޸�����   : �����ɺ���

*****************************************************************************/
HI_U32  HI_IO_SetOutputVal(HI_GPIO_IDX_E enId, HI_GPIO_VALUE_E enVal)
{
    HI_U32 ulRegVal = 0;
    HI_U32 lock_stat = 0;

    if(enId >= HI_GPIO_IDX_MAX)
    {
        return HI_ERR_INVALID_PARAMETER;
    }
    
    lock_stat = HI_MDM_INT_Lock();

    HI_REG_READ32((GPIO_PHY_BASE + GPIO_SWPORT_DR), ulRegVal);
    if(enVal)
    {
        ulRegVal |= (HI_U32)(1 << (HI_U32)enId);
    }
    else
    {
        ulRegVal &= ~(HI_U32)(1 << (HI_U32)enId);
    }
    HI_REG_WRITE32((GPIO_PHY_BASE + GPIO_SWPORT_DR), ulRegVal);
    
    HI_MDM_INT_Restore(lock_stat);
    
    return HI_ERR_SUCCESS;
}

/*****************************************************************************
 �� �� ��  : HI_IO_GetInputVal
 ��������  : ��ȡ����IO�ܽ�����ֵ
 �������  : HI_U8 offset   ------0~32ΪIO00~IO32
             HI_BOOL* val    ------����ֵ

 �������  : ��
 �� �� ֵ  : HI_U32
 ���������õĺ���  :
 ���ñ������ĺ���  :
 ���ʵ�ȫ�ֱ���  :
 �޸ĵ�ȫ�ֱ���  :
 �޸ĵ�BBP�Ĵ����б�  :

 �޸���ʷ      :
  1.��    ��   : 2015��7��17��
    ��    ��   : yuming
    �޸�����   : �����ɺ���

*****************************************************************************/
HI_U32  HI_IO_GetInputVal(HI_GPIO_IDX_E enId, HI_GPIO_VALUE_E *pVal)
{
    HI_U32 ulRegVal = 0;

    if((enId >= HI_GPIO_IDX_MAX) || (pVal == HI_NULL))
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    HI_REG_READ32((GPIO_PHY_BASE + GPIO_EXT_PORT), ulRegVal);
    
    if(ulRegVal & (HI_U32)(1 << (HI_U32)enId))
    {
        *pVal = HI_GPIO_VALUE1;
    }
    else
    {
        *pVal = HI_GPIO_VALUE0;
    }

    return HI_ERR_SUCCESS;
}

/*****************************************************************************
 �� �� ��  : HI_IO_Get_Output_Val
 ��������  : ��ȡ����IO�ܽ����ֵ
 �������  : HI_U8 offset   ------0~32ΪIO00~IO32
             HI_BOOL* val    ------���ֵ

 �������  : ��
 �� �� ֵ  : HI_U32
 ���������õĺ���  :
 ���ñ������ĺ���  :
 ���ʵ�ȫ�ֱ���  :
 �޸ĵ�ȫ�ֱ���  :
 �޸ĵ�BBP�Ĵ����б�  :

 �޸���ʷ      :
  1.��    ��   : 2015��7��17��
    ��    ��   : yuming
    �޸�����   : �����ɺ���

*****************************************************************************/
HI_U32  HI_IO_GetOutputVal(HI_GPIO_IDX_E enId, HI_GPIO_VALUE_E *pVal)
{
    HI_U32 ulRegVal = 0;

    if((enId >= HI_GPIO_IDX_MAX) || (pVal == HI_NULL))
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    HI_REG_READ32((GPIO_PHY_BASE + GPIO_SWPORT_DR), ulRegVal);
    
    if(ulRegVal & (HI_U32)(1 << (HI_U32)enId))
    {
        *pVal = HI_GPIO_VALUE1;
    }
    else
    {
        *pVal = HI_GPIO_VALUE0;
    }

    return HI_ERR_SUCCESS;
}

/**********************************************************�жϴ���****************************************************/
HI_PRV HI_VOID gpio_hisr(HI_U32 ulIntbit)
{
    HI_U32 i = 0;
    HI_GPIO_INTER_CTRL_S* pstCurrIntCtrl = s_pstIntCtrl;

#if defined(PRODUCT_CFG_TASK_INT_SYS_ERR_INFO) && defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    g_Isr_stat.stGPIO.ulHisrTickIn = HI_MDM_GetTick();
    g_Isr_stat.stGPIO.ulHisrCnt++;
#endif

    for(i = 0; i < HI_GPIO_IDX_MAX; i++)
    {
        if(0 == (ulIntbit & ( 1 << i )))
        {
            continue;
        }
        (pstCurrIntCtrl + i)->gpio_callback((pstCurrIntCtrl + i)->ulCallbackArgv);
    }

#if defined(PRODUCT_CFG_TASK_INT_SYS_ERR_INFO)&&defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    g_Isr_stat.stGPIO.ulHisrTickOut = HI_MDM_GetTick();
#endif
}

HI_PRV HI_VOID gpio_lisr(HI_S32 slVector)
{
    HI_U32 i = 0;
    HI_U32 ulRegVal = 0;
    HI_U32 ulIntTypeRegVal = 0;
    HI_U32 ulIntStatusRegVal = 0;
    HI_U32 ulLisrCallbackBit = 0;
    HI_U32 ulHisrCallbackBit = 0;
    HI_GPIO_INTER_CTRL_S* pstCurrIntCtr = s_pstIntCtrl;
    HI_U32 lock_stat = 0;

#if defined(PRODUCT_CFG_TASK_INT_SYS_ERR_INFO)&&defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    g_Isr_stat.stGPIO.ulLisrTickIn = HI_MDM_GetTick();
    g_Isr_stat.stGPIO.ulLisrCnt++;
#endif

    /* Interrupt read */
    HI_REG_READ32((GPIO_PHY_BASE + GPIO_INTSTATUS), ulIntStatusRegVal);

    /* Interrupt type read */
    HI_REG_READ32((GPIO_PHY_BASE + GPIO_INTTYPE_LEVEL), ulIntTypeRegVal);


    lock_stat = HI_MDM_INT_Lock();

    /* mask level int */
    HI_REG_READ32((GPIO_PHY_BASE + GPIO_INTMASK), ulRegVal);
    ulRegVal |= (ulIntStatusRegVal & (~ulIntTypeRegVal));
    HI_REG_WRITE32((GPIO_PHY_BASE + GPIO_INTMASK), ulRegVal);

    /* interrupt clear */
    HI_REG_READ32((GPIO_PHY_BASE + GPIO_PORT_EOI), ulRegVal);
    ulRegVal |= ulIntStatusRegVal;
    HI_REG_WRITE32((GPIO_PHY_BASE + GPIO_PORT_EOI), ulRegVal);

    HI_MDM_INT_Restore(lock_stat);

    
    ulLisrCallbackBit = ulIntStatusRegVal & s_ulUserCallbackInLisr;
    ulHisrCallbackBit = ulIntStatusRegVal & (~s_ulUserCallbackInLisr);

	if ((ulLisrCallbackBit & (1 << HI_GPIO_IDX_29)) == HI_FALSE) {
    	/* Activate HISR */
    	HI_MDM_IRQ_AddJob(2,(HISR_PROC_FUNC)gpio_hisr, (HI_U32)ulHisrCallbackBit, 2, 3, 4, 5);
	}
    for(i = 0; i < HI_GPIO_IDX_MAX; i++)
    {
        if( 0 == (ulLisrCallbackBit & ( 1 << i )))
        {
            continue;
        }
        
        (pstCurrIntCtr + i)->gpio_callback((pstCurrIntCtr + i)->ulCallbackArgv);
    }

#if defined(PRODUCT_CFG_TASK_INT_SYS_ERR_INFO)&&defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    g_Isr_stat.stGPIO.ulLisrTickOut = HI_SYS_GetTick();
#endif
}

/*****************************************************************************
 �� �� ��  : HI_Gpio_Interr_Init
 ��������  : GPIO�жϳ�ʼ��
 �������  : HI_VOID

 �������  : ��
 �� �� ֵ  : HI_U32
 ���������õĺ���  :
 ���ñ������ĺ���  :
 ���ʵ�ȫ�ֱ���  :
 �޸ĵ�ȫ�ֱ���  :
 �޸ĵ�BBP�Ĵ����б�  :

 �޸���ʷ      :
  1.��    ��   : 2015��7��17��
    ��    ��   : yuming
    �޸�����   : �����ɺ���

*****************************************************************************/
HI_U32 HI_IO_IntInit(HI_VOID)
{
    HI_U32 slStatus = 0;

    if (s_bInitFlag)
    {
        return HI_ERR_GPIO_REPEAT_INIT;
    }

    slStatus = HI_MDM_IRQ_Request(NUM_HAL_INTERRUPT_GPIO, 0,gpio_lisr, 0);
    if (HI_ERR_SUCCESS  != slStatus)
    {
        return HI_ERR_FAILURE;
    }
    
    HI_MDM_IRQ_Enable(NUM_HAL_INTERRUPT_GPIO);
    s_pstIntCtrl = s_astInterCtrl;
    
    s_bInitFlag = HI_TRUE;
    return HI_ERR_SUCCESS;
}

/*****************************************************************************
 �� �� ��  : HI_IO_IntConnect
 ��������  : ʹ��ĳ��GPIO���жϹ���
 �������  : HI_U8 ucOffset   ------0~31ΪIO00~IO31
             HI_BOOL* val    ------����ֵ

 �������  : ��
 �� �� ֵ  : HI_U32
 ���������õĺ���  :
 ���ñ������ĺ���  :
 ���ʵ�ȫ�ֱ���  :
 �޸ĵ�ȫ�ֱ���  :
 �޸ĵ�BBP�Ĵ����б�  :

 �޸���ʷ      :
  1.��    ��   : 2015��7��17��
*****************************************************************************/
HI_U32  HI_IO_IntConnect (HI_GPIO_IDX_E enId, HI_GPIO_INT_E enIntType, HI_GPIO_INT_POLARITY_E enIntPolarity,
                                  HI_S32 (*gpio_callback)(HI_U32 ulCallbackArgv), HI_U32 ulCallbackArgv, HI_U32 ulFlags)
{
    HI_U32 ulRegVal = 0;
    HI_U32 ulLockKey = 0;

	if (!s_bInitFlag)
	{
		return HI_ERR_GPIO_NOT_INIT;
	}

	if ((HI_NULL == gpio_callback) || (enId >= HI_GPIO_IDX_MAX))
	{
		return HI_ERR_GPIO_PARAMETER_ERROR;
	}

	ulLockKey = HI_SYS_ControlInterrupts(HI_SYS_DISABLE_INTERRUPTS);
	/* set int type */
	HI_REG_READ32((GPIO_PHY_BASE + GPIO_INTTYPE_LEVEL), ulRegVal);

	if(HI_INT_EDGE == enIntType)
	{
		ulRegVal |= (HI_U32)(1 << (HI_U32)enId);
	}
	else
	{
		ulRegVal &= ~(HI_U32)(1 << (HI_U32)enId);
	}
	HI_REG_WRITE32((GPIO_PHY_BASE + GPIO_INTTYPE_LEVEL), ulRegVal);

    /* set polarity */
    HI_REG_READ32((GPIO_PHY_BASE + GPIO_INT_POLARITY), ulRegVal);
    if(HI_EDGE_RISE_LEVEL_HIGH == enIntPolarity)
    {
/*
                    �������ˡ�z00193780
                    ������ˡ�c00233580
                    ��������������ο����ӿڣ��ͻ��п���ʹ�ø÷�֧���豣����
                    ������Ӱ�졿��Ӱ������ҵ�����θ澯
*/
        ulRegVal |= (HI_U32)(1 << (HI_U32)enId);
    }
    else
    {
        ulRegVal &= ~(HI_U32)(1 << (HI_U32)enId);
    }
    
    HI_REG_WRITE32((GPIO_PHY_BASE + GPIO_INT_POLARITY), ulRegVal);

    /* set dir as input */
    HI_REG_READ32((GPIO_PHY_BASE + GPIO_SWPORT_DDR), ulRegVal);
    ulRegVal &= ~(HI_U32)(1 << (HI_U32)enId);
    HI_REG_WRITE32((GPIO_PHY_BASE + GPIO_SWPORT_DDR), ulRegVal);

    /* set int enable */
    HI_REG_READ32((GPIO_PHY_BASE + GPIO_INTEN), ulRegVal);
    ulRegVal |= (HI_U32)(1 << (HI_U32)enId);
    HI_REG_WRITE32((GPIO_PHY_BASE + GPIO_INTEN), ulRegVal);

    /*set int unmask */
    HI_REG_READ32((GPIO_PHY_BASE + GPIO_INTMASK), ulRegVal);
    ulRegVal &= ~(HI_U32)(1 << (HI_U32)enId);
    HI_REG_WRITE32((GPIO_PHY_BASE + GPIO_INTMASK), ulRegVal);

    /* set callback and args */
    ((HI_GPIO_INTER_CTRL_S *)s_pstIntCtrl + enId)->gpio_callback = gpio_callback;
    ((HI_GPIO_INTER_CTRL_S *)s_pstIntCtrl + enId)->ulCallbackArgv = ulCallbackArgv;

    /* set int status */
    if(ulFlags)
    {
/*
                    �������ˡ�z00193780
                    ������ˡ�c00233580
                    ��������������ο����ӿڣ��ͻ��п���ʹ�ø÷�֧���豣����
                    ������Ӱ�졿��Ӱ������ҵ�����θ澯
*/        
        s_ulUserCallbackInLisr |= (HI_U32)(1 << (HI_U32)enId);
    }
    else
    {
        s_ulUserCallbackInLisr &= (~(HI_U32)(1 << (HI_U32)enId));
    }
    
    HI_SYS_ControlInterrupts(ulLockKey);
    return HI_ERR_SUCCESS;
}

/*****************************************************************************
 �� �� ��  : HI_IO_IntDisconnect
 ��������  : ȥʹ��ĳ��GPIO���жϹ���
 �������  : HI_U8 ucOffset   ------0~31ΪIO00~IO31
             HI_BOOL* val    ------����ֵ

 �������  : ��
 �� �� ֵ  : HI_U32
 ���������õĺ���  :
 ���ñ������ĺ���  :
 ���ʵ�ȫ�ֱ���  :
 �޸ĵ�ȫ�ֱ���  :
 �޸ĵ�BBP�Ĵ����б�  :

 �޸���ʷ      :
  1.��    ��   : 2015��7��17��
    ��    ��   : yuming
    �޸�����   : �����ɺ���

*****************************************************************************/
HI_U32  HI_IO_IntDisconnect(HI_GPIO_IDX_E enId)
{
    HI_U32 ulRegVal = 0;
    HI_U32 slLockKey = 0;

    if(enId >= HI_GPIO_IDX_MAX)
    {
        return HI_ERR_GPIO_PARAMETER_ERROR;
    }

    slLockKey = HI_SYS_ControlInterrupts(HI_SYS_DISABLE_INTERRUPTS);

    /* interrupt disable */
    HI_REG_READ32((GPIO_PHY_BASE + GPIO_INTEN), ulRegVal);
    ulRegVal &= ~(HI_U32)(1 << (unsigned int)enId);
    HI_REG_WRITE32((GPIO_PHY_BASE + GPIO_INTEN), ulRegVal);

    /* interrupt mask */
    HI_REG_READ32((GPIO_PHY_BASE + GPIO_INTMASK), ulRegVal);
    ulRegVal |= (HI_U32)(1 << (unsigned int)enId);
    HI_REG_WRITE32((GPIO_PHY_BASE + GPIO_INTMASK), ulRegVal);

    s_ulUserCallbackInLisr &= (~(HI_U32)(1 << (HI_U32)enId));
    HI_SYS_ControlInterrupts(slLockKey);
    
    return HI_ERR_SUCCESS;
}

