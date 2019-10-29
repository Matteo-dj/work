/*****************************************************************************
*
*                  版权所有 (C),  华为技术有限公司
*
******************************************************************************
*  文 件 名   : rf_drv_spi_controller.c
*  版 本 号   : V1.0 
*  作    者   : yuchuang/00425297
*  生成日期   : 2018-11-01
*  功能描述   : 基于SPI控制器的RF驱动
*  说明       : 本文件接口仅作为参考设计，非正式交付代码。
******************************************************************************/

#include "hi_types.h"
#include "hi_mdm_types.h"
#include "hi_mdm_io.h"
#include "si4438_defines.h"
#include "drv_dfx.h"
#include <radio_comm.h>
#include "hi_mdm_spi.h"
#include "rf_api_lib.h"
#include "rf_api.h"

#ifdef PRODUCT_CFG_RF_USE_SPI_CONTROLLER

#define SPI_PULL_UP_CS  0
#define SPI_NOT_PULL_UP_CS  1

HI_GPIO_VALUE_E radio_nirq_level(HI_VOID)
{
    HI_GPIO_VALUE_E level = HI_GPIO_VALUE0;
        
    (HI_VOID)HI_MDM_IO_GetInputVal(RF_IRQ_GPIO_IDX, &level);

    return level;
}


/***********************************************************************************
 * 入参:    byteCount->从RF处获得的数据个数;
 *          pData->从RF处获得的数据指针；
 * 
 * 返回值:  ctsVal->CTS的值
 * 
 * 
***********************************************************************************/
HI_U32 radio_comm_GetResp(HI_U8 byteCount, HI_U8* pData)
{
    HI_U8 cmd[2] = {CMD_CTS,0};
    HI_U8 ctsVal[2] = {0,};
    HI_U32 errCnt = RADIO_ANSWER_CTS_COUNT;
    HI_U32 ulIntStat = 0;
    HI_U32 ulRet = HI_ERR_SUCCESS;

    if (byteCount == 0 || pData == HI_NULL)
    {
        return HI_ERR_FAILURE;
    }
    while (errCnt != 0)      //wait until radio IC is ready with the data
    {
        ulIntStat = HI_MDM_INT_Lock();
        ulRet = HI_MDM_SPI_SendRecvData(g_ulSpiFd, cmd, ctsVal, sizeof(cmd), SPI_NOT_PULL_UP_CS);
        if(ulRet != HI_ERR_SUCCESS)
        {
            HI_MDM_INT_Restore(ulIntStat);
            return ulRet;
        }
        if (ctsVal[1] == REPLY_CTS) 
        {
            ulRet = HI_MDM_SPI_RecvData(g_ulSpiFd, pData, byteCount, SPI_PULL_UP_CS);
            if(ulRet != HI_ERR_SUCCESS)
            {
                HI_MDM_INT_Restore(ulIntStat);
                return ulRet;
            }
            break;
        }
        errCnt--;  
        HI_MDM_INT_Restore(ulIntStat);
    }
    if (errCnt == 0)
    {
        return HI_ERR_FAILURE;
    }
    return HI_ERR_SUCCESS;
}
/***********************************************************************************
 * 功能:    给RF发送命令
 * 入参:    byteCount->给RF发送的命令个数;
 *          pData->给RF发送的命令指针；
 * 
 * 返回值:  无
 * 
***********************************************************************************/
HI_U32 radio_comm_SendCmd(HI_U8 byteCount, HI_U8* pData)
{
    HI_U32 ulIntStat = 0;
    HI_U32 ulRet = HI_ERR_SUCCESS;

    if (byteCount == 0 || pData == HI_NULL)
    {
        return HI_ERR_FAILURE;
    }
    ulRet = radio_comm_PollCTS();
    if (HI_ERR_SUCCESS != ulRet )
    {    
        return HI_ERR_FAILURE;
    }
    ulIntStat = HI_MDM_INT_Lock();
    ulRet = HI_MDM_SPI_SendData(g_ulSpiFd, pData, byteCount, SPI_PULL_UP_CS);
    if(ulRet != HI_ERR_SUCCESS)
    {
        HI_MDM_INT_Restore(ulIntStat);
        return ulRet;
    }
    HI_MDM_INT_Restore(ulIntStat);
    return HI_ERR_SUCCESS;
}
/***********************************************************************************
 * 功能:    从RF处得到命令应答
 * 入参:    cmd->Command ID;
 *          pollCts->Set to poll CTS；
 *          byteCount->从RF处得到的字节数;
 *          pData->从RF处得到的数据指针;
 * 返回值:  无
***********************************************************************************/
HI_U32 radio_comm_ReadData(HI_U8 cmd, HI_BOOL pollcts, HI_U8 byteCount, HI_U8* pData)
{
    HI_U32 ulIntStat = 0;
    HI_U32 ulRet = HI_ERR_SUCCESS;
    
    if (byteCount == 0 || pData == HI_NULL)
    {
        return HI_ERR_FAILURE;
    }

    if (pollcts)
    {
        if (HI_ERR_SUCCESS != radio_comm_PollCTS())
        {    
            return HI_ERR_FAILURE;
        }
    }
    ulIntStat = HI_MDM_INT_Lock();
    ulRet = HI_MDM_SPI_SendData(g_ulSpiFd, &cmd, sizeof(cmd), SPI_NOT_PULL_UP_CS);
    if(ulRet != HI_ERR_SUCCESS)
    {
        HI_MDM_INT_Restore(ulIntStat);
        return ulRet;
    }
    ulRet = HI_MDM_SPI_RecvData(g_ulSpiFd, pData, byteCount, SPI_PULL_UP_CS);
    if(ulRet != HI_ERR_SUCCESS)
    {
        HI_MDM_INT_Restore(ulIntStat);
        return ulRet;
    }
    HI_MDM_INT_Restore(ulIntStat);
    return HI_ERR_SUCCESS;
}

HI_U32 radio_comm_ReadDataWaiteCts(HI_U8 cmd, HI_U8 byteCount, HI_U8* pData)
{
    return radio_comm_ReadData(cmd, HI_TRUE, byteCount, pData);
}

HI_U32 radio_comm_ReadDataNoCts(HI_U8 cmd, HI_U8 byteCount, HI_U8* pData)
{
    return radio_comm_ReadData(cmd, HI_FALSE, byteCount, pData);
}
/***********************************************************************************
 * 功能:    从RF处得到命令应答
 * 入参:    cmd->Command ID;
 *          pollCts->Set to poll CTS；
 *          byteCount->给RF发送的字节数;
 *          pData->给RF发送的数据指针;
 * 返回值:  无
 * 
***********************************************************************************/
HI_U32 radio_comm_WriteData(HI_U8 cmd, HI_BOOL pollcts, HI_U8 byteCount, HI_U8* pData)
{
    HI_U32 ulIntStat = 0;
    HI_U32 ulRet = HI_ERR_SUCCESS;
    
    if (byteCount == 0 || pData == HI_NULL)
    {
        return HI_ERR_FAILURE;
    }

    if (pollcts)
    {
        if (HI_ERR_SUCCESS != radio_comm_PollCTS())
        {    
            return HI_ERR_FAILURE;
        }
    }
    ulIntStat = HI_MDM_INT_Lock(); 
    ulRet = HI_MDM_SPI_SendData(g_ulSpiFd, &cmd, sizeof(cmd), SPI_NOT_PULL_UP_CS);
    if(ulRet != HI_ERR_SUCCESS)
    {
        HI_MDM_INT_Restore(ulIntStat);
        return ulRet;
    }
    ulRet = HI_MDM_SPI_SendData(g_ulSpiFd, pData, byteCount, SPI_PULL_UP_CS);
    if(ulRet != HI_ERR_SUCCESS)
    {
        HI_MDM_INT_Restore(ulIntStat);
        return ulRet;
    }    

    HI_MDM_INT_Restore(ulIntStat);
    return HI_ERR_SUCCESS;
}

HI_U32 radio_comm_WriteDataWaiteCts(HI_U8 cmd, HI_U8 byteCount, HI_U8* pData)
{
    return radio_comm_WriteData(cmd, HI_TRUE, byteCount, pData);
}

HI_U32 radio_comm_WriteDataNoCts(HI_U8 cmd, HI_U8 byteCount, HI_U8* pData)
{
    return radio_comm_WriteData(cmd, HI_FALSE, byteCount, pData);
}
/**********************************************************************************
 * 功能:    给RF发送命令并得到命令应答
 * 入参:    cmdByteCount->给RF发送的字节数;
 *          pCmdData->给RF发送的数据指针;
 *          respByteCount->从RF处得到的字节数;
 *          pRespData->从RF处得到的数据指针;
 * 返回值:  无
 * 
***********************************************************************************/

HI_U32 radio_comm_SendCmdGetResp(HI_U8 cmdByteCount, HI_U8* pCmdData, HI_U8 respByteCount, 
    HI_U8* pRespData)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    
    ret = radio_comm_SendCmd(cmdByteCount, pCmdData);
    if(HI_ERR_SUCCESS != ret)
    {    
        return ret;
    }
    ret = radio_comm_GetResp(respByteCount, pRespData);
    return ret;
}
/***********************************************************************************
 * 功能:    等待CTS
 * 入参:    无
 *          
 * 返回值:  无
***********************************************************************************/
HI_U32 radio_comm_PollCTS(HI_VOID)
{   
    HI_U8 cmd[2] = {CMD_CTS,0};
    HI_U8 ctsVal[2] = {0,};   
    HI_U32 ulIntStat = 0;   
    HI_U32 errCnt = 100;//RADIO_ANSWER_CTS_COUNT;
    HI_U32 ulRet = HI_ERR_SUCCESS;

    while (errCnt != 0)      //wait until radio IC is ready with the data
    {      
        ulIntStat = HI_MDM_INT_Lock(); 
        ulRet = HI_MDM_SPI_SendRecvData(g_ulSpiFd, cmd, ctsVal, 2, SPI_PULL_UP_CS);
        if(ulRet != HI_ERR_SUCCESS)
        {
            HI_MDM_INT_Restore(ulIntStat);
            return ulRet;
        }
        if (ctsVal[1] == REPLY_CTS)
        {
            HI_MDM_INT_Restore(ulIntStat);
            return HI_ERR_SUCCESS;
        }    
        errCnt--; 
        HI_MDM_INT_Restore(ulIntStat);
        HI_MDM_Udelay(200);
    }
    
    return HI_ERR_FAILURE;   
}

#endif
