#include <hi_mdm.h>
#include <hi_mdm_types.h>
#ifdef PRODUCT_CFG_PRODUCT_TYPE_NDM

#include <hi_driver.h>
#include "key_driver.h"
#include "lcd_driver.h"
#include "lattice_driver.h"
HI_U32 g_lattice_task_handle;
volatile HI_BOOL g_lattice_init = HI_FALSE;
HI_U32 g_lattice_event_handle;
#define LATTICE_INT_MODE    HI_TRUE
#define LATTICE_DISP_MODE   HI_FALSE

HI_U32 lattice_init(HI_VOID);


HI_U32 i2c_write_bytes(HI_U8 clientAddress,HI_U8 *data,HI_U32 len)
{
    HI_I2C_DATA_S dataStruct = {0};

    dataStruct.pSendBuf = data;
    dataStruct.ulSendLength = len;
    dataStruct.pReceiveBuf = HI_NULL;
    dataStruct.ulReceiveLength = 0;

    return HI_I2C_SendData(HI_I2C_IDX_0, clientAddress,&dataStruct);
}

HI_U32 i2c_read_bytes(HI_U8 clientAddress,HI_U8 reg,HI_U8 *data,HI_U32 len)
{
    HI_I2C_DATA_S dataStruct = {0};

    dataStruct.pSendBuf = &reg;
    dataStruct.ulSendLength = sizeof(reg);
    dataStruct.pReceiveBuf = data;
    dataStruct.ulReceiveLength = len;

    return HI_I2C_SendRecvData(HI_I2C_IDX_0, clientAddress,&dataStruct);
}

HI_PRV HI_U32 lattice_init_mode(HI_U8 dev_addr, HI_BOOL mode)
{
    HI_U32 ret;
    HI_U8 data;

    //system clock enable
    data = REG_SYSTEM_SETUP_ADDR|REG_SYSTEM_SETUP_TURNON;
    ret = i2c_write_bytes(dev_addr,&data,1);
    if (HI_ERR_SUCCESS != ret)
    {
        return ret;
    }

    //ROW/INT output pin set
    if (LATTICE_INT_MODE == mode)
    {
        data = REG_ROW_INT_SET_ADDR|REG_ROW_INT_SET_INT;
    }
    else
    {
        data = REG_ROW_INT_SET_ADDR;
    }
    ret = i2c_write_bytes(dev_addr,&data,1);
    if (HI_ERR_SUCCESS != ret)
    {
        return ret;
    }

    //Dimming set
    data = REG_PULSE_WIDTH_ADDR|REG_PULSE_WIDTH_16DUTY16;
    ret = i2c_write_bytes(dev_addr,&data,1);
    if (HI_ERR_SUCCESS != ret)
    {
        return ret;
    }

    //Blinking set:display on
    data = REG_DISPLAY_SET_ADDR|REG_DISPLAY_SET_STATUSON;
    ret = i2c_write_bytes(dev_addr,&data,1);
    if (HI_ERR_SUCCESS != ret)
    {
        return ret;
    }

    return HI_ERR_SUCCESS;
}

HI_U32 lattice_init(HI_VOID)
{
    HI_U32 ret = HI_ERR_SUCCESS;

    ret = lattice_init_mode(HT16K33_I2C_ADDR1, LATTICE_INT_MODE);
    if (HI_ERR_SUCCESS != ret)
    {
        return ret;
    }
    ret = lattice_init_mode(HT16K33_I2C_ADDR2, LATTICE_DISP_MODE);
    if (HI_ERR_SUCCESS != ret)
    {
        return ret;
    }
    return ret;
}

HI_PRV HI_VOID HI_I2C_TaskBody(HI_U32 ulData)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U32 event_val = 0;
    KEY_DBG_PRINT0("HI_I2C_TaskBody in");

    /* 待按键分层梳理后打开 */
    HI_MDM_EVENT_Create(&g_lattice_event_handle,"lattice");
    
    g_lattice_init = HI_TRUE;
    KEY_DBG_PRINT0("HI_I2C_TaskBody event_creat_success");


    for(;;)
    {         
        ret = HI_MDM_EVENT_Wait(g_lattice_event_handle,0x00FFFFFF,&event_val,HI_SYS_WAIT_FOREVER,HI_EVENT_WAITMODE_OR|HI_EVENT_WAITMODE_CLR);
        if(HI_ERR_SUCCESS != ret)
        {
            KEY_DBG_PRINT1("Err evt:%x",ret);
            HI_MDM_Sleep(10);
            continue;
        }
        KEY_DBG_PRINT1("rcv_evt:%x",ret);
        HI_DRV_KEY_ProcessEvt(event_val&(I2C_EVT_LATTICE_INT|I2C_EVT_TIMER_KEY));
        HI_LCD_ProcessEvt(event_val&(I2C_EVT_IOEXP_INT|I2C_EVT_DISP_FLUSH_INT));

    }
}

HI_U32 HI_LATTICE_Init(HI_VOID)
{

    if(g_lattice_init==HI_FALSE)//考虑增加信号量保护  ,因为中间耗时
    {
        HI_I2C_Init(HI_I2C_IDX_0);
        (HI_VOID)lattice_init();
        HI_MDM_TASK_Create(&g_lattice_task_handle,"lattice",HI_I2C_TaskBody,0,PRODUCT_CFG_TASK_STACK_SIZE_I2C_TRANSF,PRODUCT_CFG_TASK_I2C_TRANSF);
        //vxworks中event必须等到在对应任务中第一次wait后才生效，因此在此处sleep等到event生效。不使用10ms的原因是,vxworks sleep 10ms可能立即返回
        HI_MDM_Sleep(20);
        if(g_lattice_init!=HI_TRUE)
        {
            return HI_ERR_FAILURE;
        }
        
    }
    return HI_ERR_SUCCESS;
}
#endif
