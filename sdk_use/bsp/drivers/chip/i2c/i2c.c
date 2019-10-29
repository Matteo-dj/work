/*  extdrv/interface/i2c/hi_i2c.c
 *
 * Copyright (c) 2006 Hisilicon Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program;
 *
 * History:
 *      19-April-2006 create this file
 */
#include "i2c.h"
#include <os_vx_sal_api.h>
#ifndef __VX_PORTING_I2C__
#include <hi_drv_com.h>
#include <hi_platform.h>
#include <hi_low_power.h>
#include <hi_mdm_clock.h>

#endif /* __VX_PORTING_I2C__ */

HI_PRV HI_U32 s_aulI2cBaseAddr[HI_I2C_IDX_MAX] = {I2C1_BASE, I2C2_BASE};
HI_PRV HI_BOOL s_abI2cIsStopFlag[HI_I2C_IDX_MAX] = {HI_TRUE, HI_TRUE};
HI_PRV HI_BOOL s_abI2cIsInitFlag[HI_I2C_IDX_MAX] = {HI_FALSE, HI_FALSE};
HI_PRV HI_U32 s_astI2cSem[HI_I2C_IDX_MAX];

HI_PRV HI_VOID i2c_cfg_signal_clk(HI_I2C_IDX_E enI2cId, HI_U32 ulI2cRate)
{
    HI_U32 ulRegVal = 0;
    HI_U32 ulRegHClkVal = 0;
    HI_U32 ulRegLClkVal = 0;

    /* save i2c system control */
    HI_REG_READ32((s_aulI2cBaseAddr[enI2cId] + I2C_CTRL), ulRegVal);

    /* mask all i2c interrupt */
    HI_REG_WRITE32((s_aulI2cBaseAddr[enI2cId] + I2C_CTRL), (ulRegVal & (~I2C_UNMASK_ALL)));

    ulRegHClkVal = (I2C_CLK / (ulI2cRate * 2)) / 2 - 1;
    HI_REG_WRITE32((s_aulI2cBaseAddr[enI2cId] + I2C_SCL_H), ulRegHClkVal);

    ulRegLClkVal = (I2C_CLK / (ulI2cRate * 2)) / 2 - 1;
    HI_REG_WRITE32((s_aulI2cBaseAddr[enI2cId] + I2C_SCL_L), ulRegLClkVal);

    /* resume i2c interrupt */
    HI_REG_WRITE32((s_aulI2cBaseAddr[enI2cId] + I2C_CTRL), ulRegVal);
}

HI_PRV HI_U32 i2c_start(HI_I2C_IDX_E enI2cId)
{
    HI_U32 ulRegVal = 0;
    HI_U32 ulTimeOut = 0;

    /* clear interrupt */
    HI_REG_WRITE32((s_aulI2cBaseAddr[enI2cId] + I2C_ICR), 0x7F);

    /* start operation */
    HI_REG_WRITE32((s_aulI2cBaseAddr[enI2cId] + I2C_COM), (I2C_START|I2C_WRITE));
    HI_REG_READ32((s_aulI2cBaseAddr[enI2cId] + I2C_SR), ulRegVal);

    HI_MDM_Udelay(10);

    while (((ulRegVal & I2C_START_END) != I2C_START_END) && (ulTimeOut < TIMEOUT))
    {
        ulTimeOut++;
        HI_REG_READ32((s_aulI2cBaseAddr[enI2cId] + I2C_SR), ulRegVal);
        HI_MDM_Udelay(10);
    }

    HI_REG_WRITE32((s_aulI2cBaseAddr[enI2cId] + I2C_ICR), I2C_START_END);

    /* timeout handle */
    if (ulTimeOut >= TIMEOUT)
    {
        i2c_error( "timeout >= TIMEOUT error ! \n");
        return HI_ERR_FAILURE;
    }

    if (I2C_ACK_INTR & ulRegVal) /* ack error occur */
    {
        return HI_ERR_FAILURE;
    }
    
    HI_REG_READ32((s_aulI2cBaseAddr[enI2cId] + I2C_ICR), ulRegVal);
    HI_REG_WRITE32((s_aulI2cBaseAddr[enI2cId] + I2C_ICR), (ulRegVal | I2C_CLEAR_OVER));
    
    return HI_ERR_SUCCESS;
}

HI_PRV HI_U32 i2c_stop(HI_I2C_IDX_E enI2cId)
{
    HI_U32 ulRegVal = 0;
    HI_U32 ulTimeOut = 0;

    /* start stop operation */
    HI_REG_WRITE32((s_aulI2cBaseAddr[enI2cId] + I2C_COM), HI_TRUE);
    HI_REG_READ32((s_aulI2cBaseAddr[enI2cId] + I2C_SR), ulRegVal);
    HI_MDM_Udelay(10);

    while (((ulRegVal & I2C_STOP_END) != I2C_STOP_END) && (ulTimeOut < TIMEOUT))
    {
        HI_REG_READ32((s_aulI2cBaseAddr[enI2cId] + I2C_SR), ulRegVal);
        HI_MDM_Udelay(10);
        ulTimeOut++;
    }

    HI_REG_WRITE32((s_aulI2cBaseAddr[enI2cId] + I2C_ICR), I2C_CLEAR_ALL);

    if (ulTimeOut >= TIMEOUT)
    {
        i2c_error( "timeout >= TIMEOUT in i2c_stop error ! \n");
        return HI_ERR_FAILURE;
    }

    return HI_ERR_SUCCESS;
}

HI_PRV HI_VOID i2c_read(HI_I2C_IDX_E enI2cId)
{
    HI_REG_WRITE32((s_aulI2cBaseAddr[enI2cId] + I2C_ICR), I2C_CLEAR_ALL);
    HI_REG_WRITE32((s_aulI2cBaseAddr[enI2cId] + I2C_COM), I2C_READ);
}

HI_PRV HI_VOID i2c_write(HI_I2C_IDX_E enI2cId)
{
    /* clear interrupt */
    HI_REG_WRITE32((s_aulI2cBaseAddr[enI2cId] + I2C_ICR), I2C_CLEAR_ALL);

    /* write operation */
    HI_REG_WRITE32((s_aulI2cBaseAddr[enI2cId] + I2C_COM), I2C_WRITE);
}

HI_PRV HI_U32 i2c_wait(HI_I2C_IDX_E enI2cId)
{
    HI_U32 ulRegVal = 0;
    HI_U32 ulTimeOut = 0;

    HI_REG_READ32((s_aulI2cBaseAddr[enI2cId] + I2C_SR), ulRegVal);
    while ((!(ulRegVal & I2C_OVER_INTR)) && (ulTimeOut < TIMEOUT))
    {
        ulTimeOut++;
        HI_MDM_Udelay(10);
        HI_REG_READ32((s_aulI2cBaseAddr[enI2cId] + I2C_SR), ulRegVal);
    }

    if (ulTimeOut >= TIMEOUT)
    {
        i2c_error( "wait-timeout >= TIMEOUT error ! \n");
        return HI_ERR_FAILURE;
    }

    /* ack error occur */
    if (I2C_ACK_INTR & ulRegVal) 
    {
        return HI_ERR_FAILURE;
    }

    HI_REG_READ32((s_aulI2cBaseAddr[enI2cId] + I2C_ICR), ulRegVal);
    HI_REG_WRITE32((s_aulI2cBaseAddr[enI2cId] + I2C_ICR), (ulRegVal | I2C_CLEAR_OVER));

    return HI_ERR_SUCCESS;
}

/*
 * sends a character over I2C bus routine.
 *
 * @param sendvalue: character to send
 * @return value:0--success; -1--error.
 *
 */
HI_PRV HI_U32 i2c_send_byte(HI_I2C_IDX_E enI2cId, HI_U8 ucSendVal)
{
    HI_REG_WRITE32((s_aulI2cBaseAddr[enI2cId] + I2C_TXR), ucSendVal);
    i2c_write(enI2cId);

    return i2c_wait(enI2cId);
}

/*
 * receives a character from I2C bus routine.
 *
 * @return value:character received
 *
 */
HI_PRV HI_U32 i2c_receive_byte(HI_I2C_IDX_E enI2cId, HI_U8* pData)
{
    HI_U32 ulRet = HI_ERR_FAILURE;
    HI_U32 ulRegVal = 0;
    HI_U32 ulTimeOut = 0;

    if (pData == HI_NULL)
    {
        i2c_error("null point.\n");
        return HI_ERR_FAILURE;
    }

    i2c_read(enI2cId);

    HI_REG_READ32((s_aulI2cBaseAddr[enI2cId] + I2C_SR), ulRegVal);
    while ((!(ulRegVal & I2C_RECEIVE_INTR)) && (ulTimeOut < TIMEOUT))
    {
        ulTimeOut++;
        HI_MDM_Udelay(10);
        HI_REG_READ32((s_aulI2cBaseAddr[enI2cId] + I2C_SR), ulRegVal);
    }

    if (ulTimeOut >= TIMEOUT)
    {
        i2c_error( "receive data error\n");
        return HI_ERR_FAILURE;
    }

    HI_REG_READ32((s_aulI2cBaseAddr[enI2cId] + I2C_RXR), ulRegVal);
    *pData = (HI_U8)(ulRegVal);
    ulRet = i2c_wait(enI2cId);
    if (ulRet != HI_ERR_SUCCESS)
    {
        i2c_error("read wait timeout.\n");
    }

    HI_REG_WRITE32((s_aulI2cBaseAddr[enI2cId] + I2C_ICR), (I2C_RECEIVE_INTR | I2C_OVER_INTR));

    return ulRet;
}

/*
 * writes address of device to I2C bus routine.
 *
 * @param address: address of device
 *
 */
HI_PRV HI_VOID i2c_set_addr(HI_I2C_IDX_E enI2cId, HI_U8 ucAddr)
{
    HI_REG_WRITE32((s_aulI2cBaseAddr[enI2cId] + I2C_TXR), ucAddr);
}

HI_U32 i2c_start_write_wait(HI_I2C_IDX_E enI2cId)
{
    HI_U32 ulRet = HI_ERR_FAILURE;

    ulRet = i2c_start(enI2cId);
    if (ulRet != HI_ERR_SUCCESS)
    {
        i2c_error( "start error when write address.\n");
        HI_REG_WRITE32((s_aulI2cBaseAddr[enI2cId] + I2C_ICR), I2C_CLEAR_ALL);
        return HI_ERR_FAILURE;
    }

    return HI_ERR_SUCCESS;
}

HI_U32 i2c_receive_byte_proc(HI_I2C_IDX_E enI2cId, HI_I2C_DATA_S *pstI2cData)
{
    HI_U32 ulRegVal = 0;
    HI_U32 ulRet = HI_ERR_FAILURE;
    HI_U32 i = 0;
    HI_U32 ulTimeOut = 0;
    
    for (i = 0; i < (pstI2cData->ulReceiveLength); i++)
    {
        if (i == (pstI2cData->ulReceiveLength - 1))
        {
            HI_REG_WRITE32((s_aulI2cBaseAddr[enI2cId] + I2C_COM), (I2C_READ | (~I2C_SEND_ACK)));
            HI_REG_READ32((s_aulI2cBaseAddr[enI2cId] + I2C_SR), ulRegVal);
            while ((!(ulRegVal & I2C_RECEIVE_INTR)) && (ulTimeOut < RECV_TIMEOUT))
            {
                ulTimeOut++;
                HI_MDM_Udelay(10);
                HI_REG_READ32((s_aulI2cBaseAddr[enI2cId] + I2C_SR), ulRegVal);
            }

            if (ulTimeOut >= RECV_TIMEOUT)
            {
                i2c_error( "receive data error\n");
                HI_REG_WRITE32((s_aulI2cBaseAddr[enI2cId] + I2C_ICR), I2C_CLEAR_ALL);
                return HI_ERR_FAILURE;
            }

            HI_REG_READ32((s_aulI2cBaseAddr[enI2cId] + I2C_RXR), ulRegVal);
            *(pstI2cData->pReceiveBuf + i) = (HI_U8)ulRegVal;
            ulRet = i2c_wait(enI2cId);
            if (ulRet != HI_ERR_SUCCESS)
            {
                i2c_error( "Receive data with no ack error!..\n");
                HI_REG_WRITE32((s_aulI2cBaseAddr[enI2cId] + I2C_ICR), I2C_CLEAR_ALL);
                return ulRet;
            }
        }
        else
        {
            ulRet = i2c_receive_byte(enI2cId, (pstI2cData->pReceiveBuf + i));
            if (ulRet != HI_ERR_SUCCESS)
            {
                i2c_error( "i2c_receive_byte error ! \n");
                HI_REG_WRITE32((s_aulI2cBaseAddr[enI2cId] + I2C_ICR), I2C_CLEAR_ALL);
                return ulRet;
            }
        }
    }

    HI_REG_WRITE32((s_aulI2cBaseAddr[enI2cId] + I2C_ICR), I2C_CLEAR_OVER);
    if (HI_TRUE == s_abI2cIsStopFlag[enI2cId])
    {
        ulRet = i2c_stop(enI2cId);
        if (ulRet != HI_ERR_SUCCESS)
        {
            HI_REG_WRITE32((s_aulI2cBaseAddr[enI2cId] + I2C_ICR), I2C_CLEAR_ALL);
            return ulRet;
        }
    }
    
    return HI_ERR_SUCCESS;
}

HI_U32 i2c_restart_receive_data(HI_I2C_IDX_E enI2cId, HI_U8 ucDevAddr, HI_I2C_DATA_S *pstI2cData)
{
    HI_U32 i = 0;
    HI_U32 ulRet = 0;


    HI_REG_WRITE32((s_aulI2cBaseAddr[enI2cId] + I2C_CTRL),
                 (I2C_IP_ENABLE | I2C_UNMASK_ALL | I2C_UNMASK_ACK | I2C_UNMASK_ARBITRATE | I2C_UNMASK_OVER));

    HI_REG_WRITE32((s_aulI2cBaseAddr[enI2cId] + I2C_ICR), I2C_CLEAR_ALL);

    i2c_set_addr(enI2cId, ucDevAddr & 0xfe);
    ulRet = i2c_start_write_wait(enI2cId);
    if (ulRet != HI_ERR_SUCCESS)
    {
        return ulRet;
    }

    for (i = 0; i < (pstI2cData->ulSendLength); i++)
    {
        ulRet = i2c_send_byte(enI2cId, *(pstI2cData->pSendBuf + i));
        if (ulRet != HI_ERR_SUCCESS)
        {
            i2c_error( "no ack.\n");
            HI_REG_WRITE32((s_aulI2cBaseAddr[enI2cId] + I2C_ICR), I2C_CLEAR_ALL);
            return ulRet;
        }
    }

    i2c_set_addr(enI2cId, ucDevAddr | 0x01);
    
    ulRet = i2c_start_write_wait(enI2cId);
    if (ulRet != HI_ERR_SUCCESS)
    {
        return ulRet;
    }
    
    ulRet = i2c_receive_byte_proc(enI2cId, pstI2cData);
    if (ulRet != HI_ERR_SUCCESS)
    {
        return ulRet;
    } 
    
    
    return HI_ERR_SUCCESS;
}

HI_U32 HI_I2C_SendRecvData(HI_I2C_IDX_E enI2cId, HI_U8 ucDevAddr, HI_I2C_DATA_S *pstI2cData)
{
    HI_U32 ulRet= HI_ERR_SUCCESS;

    if (enI2cId >= HI_I2C_IDX_MAX)
    {
        i2c_error( "i2c port is error. u32PortNum = %d.\n", enI2cId);
        return HI_ERR_FAILURE;
    }
    if(HI_NULL == pstI2cData)
    {
        return HI_ERR_FAILURE;
    }

    if ((pstI2cData->pSendBuf == HI_NULL) || (pstI2cData->pReceiveBuf == HI_NULL))
    {
        i2c_error("null point.\n");
        return (HI_U32)HI_ERR_FAILURE;
    }

    ulRet = HI_MDM_SEM_Wait(s_astI2cSem[enI2cId], HI_SYS_WAIT_FOREVER);
    if (ulRet != HI_ERR_SUCCESS)
    {
        return HI_ERR_CREATE_SEM_FAIL;
    }
    
    HI_LOW_POWER_Forbid(HI_LOW_POWER_FORBID_ID_I2C);
    
    ulRet = i2c_restart_receive_data(enI2cId, ucDevAddr, pstI2cData);

    HI_LOW_POWER_UnForbid(HI_LOW_POWER_FORBID_ID_I2C,0);
    HI_MDM_SEM_Signal(s_astI2cSem[enI2cId]);  
    return ulRet;
    
}

HI_U32 i2c_send_data(HI_I2C_IDX_E enI2cId, HI_U8 ucDevAddr, HI_I2C_DATA_S *pstI2cData)
{
    HI_U32 ulRet = HI_ERR_FAILURE;
    HI_U32 i = 0;


    HI_REG_WRITE32((s_aulI2cBaseAddr[enI2cId] + I2C_CTRL), (I2C_IP_ENABLE | I2C_UNMASK_ALL | I2C_UNMASK_ACK
                                                          | I2C_UNMASK_ARBITRATE | I2C_UNMASK_OVER));

    /* clear all i2c interrupt */
    HI_REG_WRITE32((s_aulI2cBaseAddr[enI2cId] + I2C_ICR), I2C_CLEAR_ALL);

    i2c_set_addr(enI2cId, ucDevAddr & (~READ_OPERATION));

    ulRet = i2c_start(enI2cId);
    if (ulRet != HI_ERR_SUCCESS)
    {
        i2c_error( "start error when write address.\n");
        HI_REG_WRITE32((s_aulI2cBaseAddr[enI2cId] + I2C_ICR), I2C_CLEAR_ALL);
        return ulRet;
    }

    for (i = 0; i < (pstI2cData->ulSendLength); i++)
    {
        ulRet = i2c_send_byte(enI2cId, *(pstI2cData->pSendBuf + i));
        if (ulRet != HI_ERR_SUCCESS)
        {
            i2c_error( "i2csendbyte() error ! \n");
            HI_REG_WRITE32((s_aulI2cBaseAddr[enI2cId] + I2C_ICR), I2C_CLEAR_ALL);
            return ulRet;
        }
    }

    if (HI_TRUE == s_abI2cIsStopFlag[enI2cId])
    {
        ulRet = i2c_stop(enI2cId);
        if (ulRet != HI_ERR_SUCCESS)
        {
            HI_REG_WRITE32((s_aulI2cBaseAddr[enI2cId] + I2C_ICR), I2C_CLEAR_ALL);
            return ulRet;
        }
    }

    return HI_ERR_SUCCESS;
}
HI_U32 HI_I2C_SendData(HI_I2C_IDX_E enI2cId, HI_U8 ucDevAddr, HI_I2C_DATA_S *pstI2cData)
{
    HI_U32 ulRet = HI_ERR_FAILURE;

    if (enI2cId >= HI_I2C_IDX_MAX)
    {
        i2c_error("i2c port is error. u32PortNum = %d.\n", enI2cId);
        return HI_ERR_FAILURE;
    }
    if(HI_NULL == pstI2cData)
    {
        return HI_ERR_FAILURE;
    }

    if (pstI2cData->pSendBuf == HI_NULL)
    {
        i2c_error("null point.\n");
        return HI_ERR_FAILURE;
    }

    ulRet = HI_MDM_SEM_Wait(s_astI2cSem[enI2cId], HI_SYS_WAIT_FOREVER);
    if (ulRet != HI_ERR_SUCCESS)
    {
        return HI_ERR_CREATE_SEM_FAIL;
    }
    
    HI_LOW_POWER_Forbid(HI_LOW_POWER_FORBID_ID_I2C);
    ulRet = i2c_send_data(enI2cId, ucDevAddr, pstI2cData);
    
    HI_LOW_POWER_UnForbid(HI_LOW_POWER_FORBID_ID_I2C,0);
    HI_MDM_SEM_Signal(s_astI2cSem[enI2cId]);

    return ulRet;

}

HI_U32 i2c_receive_data(HI_I2C_IDX_E enI2cId, HI_U8 ucDevAddr, HI_I2C_DATA_S *pstI2cData)
{

    HI_U32 ulRet = HI_ERR_FAILURE;

    HI_REG_WRITE32((s_aulI2cBaseAddr[enI2cId] + I2C_CTRL), 
                 (I2C_IP_ENABLE | I2C_UNMASK_ALL | I2C_UNMASK_ACK | I2C_UNMASK_ARBITRATE | I2C_UNMASK_OVER));

    /* clear all i2c interrupt */
    HI_REG_WRITE32((s_aulI2cBaseAddr[enI2cId] + I2C_ICR), I2C_CLEAR_ALL);

    i2c_set_addr(enI2cId, ucDevAddr & (~READ_OPERATION));

    ulRet = i2c_start(enI2cId);
    if (ulRet != HI_ERR_SUCCESS)
    {
        i2c_error( "start error when write address.\n");
        HI_REG_WRITE32((s_aulI2cBaseAddr[enI2cId] + I2C_ICR), I2C_CLEAR_ALL);
        return ulRet;
    }

    i2c_write(enI2cId);

    ulRet = i2c_wait(enI2cId);
    if (ulRet != HI_ERR_SUCCESS)
    {
        i2c_error( "no ack.\n");
        HI_REG_WRITE32((s_aulI2cBaseAddr[enI2cId] + I2C_ICR), I2C_CLEAR_ALL);
        return ulRet;
    }
    
    ulRet = i2c_receive_byte_proc(enI2cId, pstI2cData);
    if (ulRet != HI_ERR_SUCCESS)
    {
        return ulRet;
    }
    return HI_ERR_SUCCESS;
}

HI_U32 HI_I2C_RecvData(HI_I2C_IDX_E enI2cId, HI_U8 ucDevAddr, HI_I2C_DATA_S *pstI2cData)
{
    HI_U32 ulRet  = HI_ERR_FAILURE;

    if (enI2cId >= HI_I2C_IDX_MAX)
    {
        i2c_error( "i2c port is error. u32PortNum = %d.\n", enI2cId);
        return HI_ERR_FAILURE;
    }
    
    if(HI_NULL == pstI2cData)
    {
        return HI_ERR_FAILURE;
    }

    if (pstI2cData->pReceiveBuf == HI_NULL)
    {
        i2c_error("null point.\n");
        return HI_ERR_FAILURE;
    }

    ulRet = HI_MDM_SEM_Wait(s_astI2cSem[enI2cId], HI_SYS_WAIT_FOREVER);
    if (ulRet != HI_ERR_SUCCESS)
    {
        return HI_ERR_CREATE_SEM_FAIL;
    }
    
    HI_LOW_POWER_Forbid(HI_LOW_POWER_FORBID_ID_I2C);
    ulRet = i2c_receive_data(enI2cId,ucDevAddr,pstI2cData);
    
    HI_LOW_POWER_UnForbid(HI_LOW_POWER_FORBID_ID_I2C,0);
    HI_MDM_SEM_Signal(s_astI2cSem[enI2cId]);
    return ulRet;

}

HI_U32 HI_I2C_Init(HI_I2C_IDX_E enI2cId)
{
    if (enI2cId >= HI_I2C_IDX_MAX)
    {
        return HI_ERR_FAILURE;
    }
        
    s_abI2cIsStopFlag[enI2cId] = HI_TRUE;
    if (HI_FALSE == s_abI2cIsInitFlag[enI2cId])
    {
        if(enI2cId==HI_I2C_IDX_0)
        {
            HI_MDM_SYS_CLOCK_Enable(HI_SYSCTRL_CLKEN_REG,HI_SYSCTRL_CLKEN_REG_BIT_SHIFT_I2C);
        }
        else
        {
            HI_MDM_SYS_CLOCK_Enable(HI_SYSCTRL_PHY_WRAP_CLK_EN_REG,HI_SYSCTRL_PHY_WRAP_CLK_EN_REG_BIT_SHIFT_I2C2);
        }
            
        HI_REG_WRITE32((s_aulI2cBaseAddr[enI2cId] + I2C_CTRL), (~(I2C_IP_ENABLE)));
        i2c_cfg_signal_clk(enI2cId, (HI_U32)I2C_RATE_DEFAULT);
        HI_REG_WRITE32((s_aulI2cBaseAddr[enI2cId] + I2C_ICR), I2C_CLEAR_ALL);
        HI_REG_WRITE32((s_aulI2cBaseAddr[enI2cId] + I2C_CTRL), I2C_INIT_VALUE);
        HI_MDM_SEM_BCreate(&s_astI2cSem[enI2cId], "I2C_sem", HI_TRUE);
        s_abI2cIsInitFlag[enI2cId] = HI_TRUE;
    }
    else
    {
        i2c_error("I2c had been initialized.\n");
    }

    return HI_ERR_SUCCESS;
}

HI_U32 HI_I2C_DeInit(HI_I2C_IDX_E enI2cId)
{
    if (enI2cId >= HI_I2C_IDX_MAX)
    {
        return HI_ERR_FAILURE;
    }
    
    HI_MDM_SEM_Delete(s_astI2cSem[enI2cId]);
    s_abI2cIsInitFlag[enI2cId] = HI_FALSE;
    return HI_ERR_SUCCESS;
}


