//*****************************************************************************
//
//                  ��Ȩ���� (C),  ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : rf_drv_gpio_spi.c
//  �� �� ��   : V1.0 
//  ��    ��   : wanglei/00279578
//  ��������   : 2014-10-10
//  ��������   : RF�����շ���
//               
//  �����б�   : TODO: ...
//  �޸���ʷ   : 
//  1.��    �� : 2014-10-10
//    ��    �� : wanglei/00279578
//    �޸����� : �����ļ�
//
//*****************************************************************************




#include <gpio_spi.h>
#include <radio_comm.h>
#include <si4438_defines.h>
#include <dfx_sys.h>
#include <hi_sal.h>
#include <hi_drv_com.h>
#include <rf_api_lib.h>
#include "rf_api.h"


#ifdef PRODUCT_CFG_RF_USE_SPI_SIMULATOR


#ifdef PRODUCT_CFG_SUPPORT_RF_DEBUG

/* ����ͳ�Ƶȴ�cts���� */
HI_RF_CMD_STAT_ST g_rf_cmd_stat;
/* ����ͳ��ִ��ָ���¼�ʹ�����Χ */
HI_RF_CMD_ORDER_ST g_rf_cmd_order;
/* �ڲ�����¼��һ������ִ��˳�� */
HI_PRV HI_BOOL s_rf_times = HI_FALSE;
/* �ڲ������ڼ�¼��������ִ�д��� */
HI_PRV HI_U32 s_rf_total_times;

HI_VOID radio_backup_cmd_order(HI_VOID)
{
    HI_U32 old_level;
    
    old_level = HI_SYS_ControlInterrupts(HI_SYS_DISABLE_INTERRUPTS);
    if (s_rf_times == HI_FALSE)
    {
        (hi_void)memcpy_s((HI_CHAR*)&(g_rf_cmd_order.first_backup),
                    sizeof(g_rf_cmd_order.first_backup),
                    (HI_CHAR*)&(g_rf_cmd_order.current_order),
                    sizeof(HI_SI4438_CMD_ORDER_ST));
        s_rf_times = HI_TRUE;
    }

    (hi_void)memcpy_s((HI_CHAR*)&(g_rf_cmd_order.backup_order),
                sizeof(g_rf_cmd_order.backup_order),
                (HI_CHAR*)&(g_rf_cmd_order.current_order),
                sizeof(HI_SI4438_CMD_ORDER_ST));
    HI_SYS_ControlInterrupts(old_level);
}
HI_VOID radio_init_cmd_err(HI_VOID)
{
    g_rf_cmd_stat.init_cmd_err_times ++;
}

HI_VOID radio_cmd_stat(HI_SI4438_CMD_TYPE_EN type, HI_U32 err_cnt)
{
    //��¼ִ��ָ��˳��͵�ǰָ��λ��
    g_rf_cmd_order.current_order.current_point = s_rf_total_times & (CMD_LOG_NUM - 1);//ֻ��¼32������
    g_rf_cmd_order.current_order.opr_order[g_rf_cmd_order.current_order.current_point] = (HI_U8)type;
    
    g_rf_cmd_stat.cts_stat[type].wait_total += err_cnt;//���������ִ�еȴ�
    g_rf_cmd_stat.cts_stat[type].times ++;//����������ʱͳ��Ϊ1��
    s_rf_total_times = s_rf_total_times + 1;
    //��¼��ֵ
    if (err_cnt > g_rf_cmd_stat.cts_stat[type].wait_max)
    {
        g_rf_cmd_stat.cts_stat[type].wait_max = err_cnt;
    }

    //��¼��������
    if (err_cnt < TIMES_AREA_1)
    {
        g_rf_cmd_stat.cts_times_area[0] ++;
    }
    else if (err_cnt < TIMES_AREA_2)
    {
        g_rf_cmd_stat.cts_times_area[1] ++;
    }
    else if (err_cnt < TIMES_AREA_3)
    {
        g_rf_cmd_stat.cts_times_area[2] ++;
    }
    else
    {
        g_rf_cmd_stat.cts_times_area[3] ++;
    }
    
}

#define FLUSH_CMD_STAT(type,cnt) radio_cmd_stat(type,cnt)
#else//PRODUCT_CFG_SUPPORT_RF_DEBUG
#define FLUSH_CMD_STAT(type,cnt)    \
    do{type = type;    \
    }while(0)
#endif//PRODUCT_CFG_SUPPORT_RF_DEBUG

HI_GPIO_VALUE_E radio_nirq_level(HI_VOID)
{
    HI_GPIO_VALUE_E level = HI_GPIO_VALUE0;
        
    (HI_VOID)HI_MDM_IO_GetInputVal(RF_IRQ_GPIO_IDX, &level);

    return level;
}
             
/***********************************************************************************
 * ���:    byteCount->��RF����õ����ݸ���;
 *          pData->��RF����õ�����ָ�룻
 * 
 * ����ֵ:  ctsVal->CTS��ֵ
 * 
 * 
***********************************************************************************/
HI_U32 radio_comm_GetResp(HI_U8 byteCount, HI_U8* pData)
{
    HI_U8 ctsVal = 0;    
    HI_U8 cmd = CMD_CTS;
    HI_U32 errCnt = RADIO_ANSWER_CTS_COUNT;
    HI_U32 old_level = 0;

    while (errCnt != 0)      //wait until radio IC is ready with the data
    {
        old_level = HI_SYS_ControlInterrupts(HI_SYS_DISABLE_INTERRUPTS);
        HI_MDM_GpioSpiSelEnable();
        HI_MDM_GpioSpiWriteByte(cmd);    //read CMD buffer
        ctsVal = HI_MDM_GpioSpiReadByte();

        if (ctsVal == REPLY_CTS)
        {
            if (byteCount)
            {
                HI_MDM_GpioSpiReadData(byteCount, pData);
            }
            HI_MDM_GpioSpiSelDisable();
            HI_SYS_ControlInterrupts(old_level);     
            break;
        }
        errCnt--;
        HI_MDM_GpioSpiSelDisable();
        HI_SYS_ControlInterrupts(old_level);
    }

    if (errCnt == 0)
    {
        return HI_ERR_FAILURE;
    }
    
    return HI_ERR_SUCCESS;
}
/***********************************************************************************
 * ����:    ��RF��������
 * ���:    byteCount->��RF���͵��������;
 *          pData->��RF���͵�����ָ�룻
 * 
 * ����ֵ:  ��
 * 
***********************************************************************************/
HI_U32 radio_comm_SendCmd(HI_U8 byteCount, HI_U8* pData)
{
    HI_U32 old_level = 0;
    HI_U32 ret;
    
    if (byteCount == 0 || pData == HI_NULL)
    {
        return HI_ERR_FAILURE;
    }
    ret = radio_comm_PollCTS();
    if (HI_ERR_SUCCESS != ret)
    {    
        return HI_ERR_FAILURE;
    }

    old_level = HI_SYS_ControlInterrupts(HI_SYS_DISABLE_INTERRUPTS);
    HI_MDM_GpioSpiSelEnable();
    HI_MDM_GpioSpiWriteData(byteCount, pData);
    HI_MDM_GpioSpiSelDisable(); 
    HI_SYS_ControlInterrupts(old_level);
    return HI_ERR_SUCCESS;
}
/***********************************************************************************
 * ����:    ��RF���õ�����Ӧ��
 * ���:    cmd->Command ID;
 *          pollCts->Set to poll CTS��
 *          byteCount->��RF���õ����ֽ���;
 *          pData->��RF���õ�������ָ��;
 * ����ֵ:  ��
 *
 ***********************************************************************************/
HI_U32 radio_comm_ReadData(HI_U8 cmd, HI_BOOL pollcts, HI_U8 byteCount, HI_U8* pData)
{
    HI_U32 old_level = 0;
    
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
    old_level = HI_SYS_ControlInterrupts(HI_SYS_DISABLE_INTERRUPTS);
    HI_MDM_GpioSpiSelEnable();
    HI_MDM_GpioSpiWriteByte(cmd);
    HI_MDM_GpioSpiReadData(byteCount, pData);
    HI_MDM_GpioSpiSelDisable(); 
    HI_SYS_ControlInterrupts(old_level);
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
 * ����:    ��RF���õ�����Ӧ��
 * ���:    cmd->Command ID;
 *          pollCts->Set to poll CTS��
 *          byteCount->��RF���͵��ֽ���;
 *          pData->��RF���͵�����ָ��;
 * ����ֵ:  ��
 * 
 * 
 ***********************************************************************************/
HI_U32 radio_comm_WriteData(HI_U8 cmd, HI_BOOL pollcts, HI_U8 byteCount, HI_U8* pData)
{
    HI_U32 old_level = 0;
    
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
    old_level = HI_SYS_ControlInterrupts(HI_SYS_DISABLE_INTERRUPTS);
    HI_MDM_GpioSpiSelEnable();
    HI_MDM_GpioSpiWriteByte(cmd);
    HI_MDM_GpioSpiWriteData(byteCount, pData);
    HI_MDM_GpioSpiSelDisable();
    HI_SYS_ControlInterrupts(old_level);
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
/***********************************************************************************
 * ����:    ��RF��������õ�����Ӧ��
 * ���:    cmdByteCount->��RF���͵��ֽ���;
 *          pCmdData->��RF���͵�����ָ��;
 *          respByteCount->��RF���õ����ֽ���;
 *          pRespData->��RF���õ�������ָ��;
 * ����ֵ:  ��
 * 
 ***********************************************************************************/
HI_U32 radio_comm_SendCmdGetResp(HI_U8 cmdByteCount, HI_U8* pCmdData, HI_U8 respByteCount, 
    HI_U8* pRespData)
{
    HI_U32 ret;
    
    ret = radio_comm_SendCmd(cmdByteCount, pCmdData);
    if(HI_ERR_SUCCESS != ret)
    {    
        return ret;
    }
    ret = radio_comm_GetResp(respByteCount, pRespData);
    return ret;
}
/***********************************************************************************
 * ����:    �ȴ�CTS
 * ���:    ��
 *          
 * ����ֵ:  ��
 * 
 ***********************************************************************************/
HI_U32 radio_comm_PollCTS()
{   
    HI_U8 ctsVal = 0;    
    HI_U8 cmd = CMD_CTS;
    HI_U32 old_level = 0;    
    HI_U32 errCnt = RADIO_ANSWER_CTS_COUNT;
    
    while (errCnt != 0)      //wait until radio IC is ready with the data
    {      
        old_level = HI_SYS_ControlInterrupts(HI_SYS_DISABLE_INTERRUPTS);
        HI_MDM_GpioSpiSelEnable();
        HI_MDM_GpioSpiWriteByte(cmd);    //read CMD buffer
        ctsVal = HI_MDM_GpioSpiReadByte();       
        if (ctsVal == REPLY_CTS)
        {
            HI_MDM_GpioSpiSelDisable();
            HI_SYS_ControlInterrupts(old_level);
            return HI_ERR_SUCCESS;
        }
        errCnt--;     
        HI_MDM_GpioSpiSelDisable();
        HI_SYS_ControlInterrupts(old_level);
    }
    return HI_ERR_FAILURE;
}

#endif
