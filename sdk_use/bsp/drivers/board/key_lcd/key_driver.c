#include <hi_types.h>
#include <hi_mdm_types.h>
#ifdef PRODUCT_CFG_SUPPORT_KEY
#include <hi_driver.h>
#include "key_driver.h"
#include "lattice_driver.h"
#define HI_KEY_LOCK()
#define HI_KEY_UNLOCK()
#define HI_KEY_DRIVER_ITEM(key_num) ((HI_KEY_DRIVER_ITEM_S *)(&(g_key_driver_ctrl.key_items[key_num])))
#define HI_KEY_DRIVER_CTRL() ((HI_KEY_DRIVER_CTRL_S *)(&(g_key_driver_ctrl)))

HI_KEY_DRIVER_CTRL_S g_key_driver_ctrl;

HI_U32 modify_key_press_timer(HI_VOID);

/**************************************************************
函数名:key_get_stat
功能:获取按键中断状态并且清空按键中断
参数:
    key_state:每bit表示一个按键状态
返回值:
    HI_ERR_SUCCESS:有有效按键按下
    HI_ERR_KEY_ERR_VALUE:仅有无效按键按下
    HI_ERR_INVALID_PARAMETER:参数错误
***************************************************************/

HI_PRV HI_U32 key_get_stat(HI_OUT HI_U16 *pValidKeyBit)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U16 ks_val[REG_KAYSOURCE_NUM] = {0};

    if (HI_NULL == pValidKeyBit)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    ret = i2c_read_bytes(HT16K33_I2C_ADDR1,REG_KAYDATA_ADDR,(HI_U8*)ks_val,REG_KAYDATA_NUM);
    if (HI_ERR_SUCCESS != ret)
    {
        return ret;
    }
    *pValidKeyBit = (ks_val[0]&LATTICE_KEY_MASK)>>5;  //保留KS0对应的k9/k8/k7/k6
    *pValidKeyBit |= (ks_val[1]&LATTICE_KEY_MASK)>>1;//保留KS1对应的k9/k8/k7/k6
    *pValidKeyBit |= (ks_val[2]&LATTICE_KEY_MASK)<<3;//保留KS2对应的k9/k8/k7/k6
    KEY_DBG_PRINT4("key :%x,%x,%x,%x",*pValidKeyBit, ks_val[0],ks_val[1],ks_val[2]);

    if((ks_val[0]&(HI_U16)(~LATTICE_KEY_MASK))|(ks_val[1]&(HI_U16)(~LATTICE_KEY_MASK))|
        (ks_val[2]&(HI_U16)(~LATTICE_KEY_MASK)))
    {
        //仅无效按键被按，做异常处理
        if (0 == *pValidKeyBit)
        {
            return HI_ERR_KEY_ERR_VALUE;
        }
        //有效和无效按键被按
        else
        {
            //依然返回成功，有效按键需要继续处理，这里仅统计错误
        }
    }
    return HI_ERR_SUCCESS;
}


/**************************************************************
函数名:key_timer_isr
功能:按键中断函数
参数:
    gpio_key_data:无效参数
返回值:

***************************************************************/
HI_PRV HI_VOID  key_timer_isr(HI_U32 ulData)
{    
    HI_MDM_EVENT_Send(g_lattice_event_handle,I2C_EVT_TIMER_KEY);
    return;
}

HI_PRV HI_VOID key_up_down(HI_U16 usValidKeyBit,HI_U32 ulAddMs)
{
    HI_U32 i = 0;
    key_up_down_func key_func = HI_NULL;
    HI_U32 ulUsrData = 0;
    for(i=0;i<HI_DRV_KEY_NUM_MAX;i++)
    {
        HI_KEY_DRIVER_ITEM_S *pKeyItem =  HI_KEY_DRIVER_ITEM(i);
        key_func =  pKeyItem->pKeyFunc;
        ulUsrData = pKeyItem->ulUsrData;

        if(key_func==HI_NULL)
        {
            pKeyItem->ulKeyPressMs = 0;
            continue;
        }

        if(usValidKeyBit&(1<<i))//按键按下
        {
            pKeyItem->ulKeyPressMs += ulAddMs;
            if(pKeyItem->ulKeyPressMs>=HI_DRV_KEY_PRESS_TIME_PRECISION_MS)
            {
                key_func(HI_TRUE,pKeyItem->ulKeyPressMs,ulUsrData);//回调中处理满,影响所有按键计时
            }
        }
        else
        {
            pKeyItem->ulKeyPressMs += ulAddMs;
            if(pKeyItem->ulKeyPressMs>=(HI_DRV_KEY_PRESS_TIME_PRECISION_MS<<1))
            {
                key_func(HI_FALSE,pKeyItem->ulKeyPressMs,ulUsrData);//回调中处理满,影响所有按键计时
            }
            pKeyItem->ulKeyPressMs = 0;
        }

    }
    return;
}

HI_U32 modify_key_press_timer(HI_VOID)
{
    HI_KEY_DRIVER_CTRL_S *pKeyCtrl = HI_KEY_DRIVER_CTRL();
    return HI_MDM_TIMER_Start(&(pKeyCtrl->timerHandle),key_timer_isr,HI_DRV_KEY_PRESS_TIME_PRECISION_MS,HI_TIMER_TYPE_ONCE,0);
}

HI_PRV HI_S32 key_isr(HI_U32 gpio_key_data)
{
    HI_MDM_EVENT_Send(g_lattice_event_handle,I2C_EVT_LATTICE_INT);
    return 0;
}




HI_PRV HI_U32 key_isr_enable(HI_BOOL enable)
{
    HI_U32 ret = HI_ERR_SUCCESS;

    if (HI_TRUE == enable)
    {
    
        ret = HI_MDM_IO_IntConnect(HI_GPIO_IDX_16, HI_INT_LEVEL, HI_EDGE_FALL_LEVEL_LOW, key_isr, 0, HI_FALSE);
    }
    else
    {
        
        ret = HI_MDM_IO_IntDisconnect(HI_GPIO_IDX_16);
    }
    if(ret != HI_ERR_SUCCESS)
    {
        return HI_ERR_KEY_ENABLE_INT;
    }
    KEY_DBG_PRINT1("Int enable:%x",enable);

    return ret;
}

HI_U32 HI_DRV_KEY_ProcessEvt(HI_U32 ulEventBit)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U16 usValidKeyBit;
    HI_U32 ulAddMs = 0;


    if(I2C_EVT_LATTICE_INT & ulEventBit)
    {
#if 1
        HI_MDM_Sleep(10);//防止无效按键按下,不断产生中断,死循环
        ulAddMs = 0;
#else
        modify_key_press_timer();
#endif
    }
    else if(I2C_EVT_TIMER_KEY & ulEventBit)
    {
        ulAddMs = HI_DRV_KEY_PRESS_TIME_PRECISION_MS;
    }
    else
    {
        return HI_ERR_NOT_SUPPORT;
    }

    //获取按键状态
    ret = key_get_stat(&usValidKeyBit);
    if(ret!=HI_ERR_SUCCESS)
    {
        //获取按键状态失败,当按键抬起处理
        usValidKeyBit = 0;
    }

    //处理按键事件,使能按键中断
    if(usValidKeyBit==0)
    {
        key_up_down(usValidKeyBit,ulAddMs);
        key_isr_enable(HI_TRUE);
    }
    else//处理按键时间,启动定时器
    {
        key_up_down(usValidKeyBit,ulAddMs);
        modify_key_press_timer();
    }
    return HI_ERR_SUCCESS;
}


/**************************************************************
函数名:HI_DRV_KEY_Register
功能:注册按键
参数:
keyId:按键ID
keyFunc:按键回调函数
usrData:回调参数
返回值:
***************************************************************/
HI_U32 HI_DRV_KEY_Register(HI_DRV_KEY_NUM_E keyId,key_up_down_func keyFunc,HI_U32 usrData)
{
    HI_KEY_DRIVER_CTRL_S *pKeyCtrl = HI_KEY_DRIVER_CTRL();
    HI_KEY_DRIVER_ITEM_S *pKeyItem =  HI_KEY_DRIVER_ITEM(keyId);
    if(pKeyCtrl->bInit==HI_FALSE)//也可以不判读,在初始化之前支持用户注册但无实际用处
    {
        return HI_ERR_INITILIZATION;
    }

    if(keyId >= HI_DRV_KEY_NUM_MAX
        ||keyFunc==HI_NULL)
    {
        return HI_ERR_INVALID_PARAMETER;
    }


    pKeyItem->pKeyFunc = keyFunc;
    pKeyItem->ulUsrData = usrData;

    return HI_ERR_SUCCESS;
}


/**************************************************************
函数名:HI_DRV_KEY_UnRegister
功能:去注册按键
参数:
keyId:按键ID
返回值:
***************************************************************/
HI_U32 HI_DRV_KEY_UnRegister(HI_DRV_KEY_NUM_E keyId)
{
    HI_KEY_DRIVER_CTRL_S *pKeyCtrl = HI_KEY_DRIVER_CTRL();
    HI_KEY_DRIVER_ITEM_S *pKeyItem =  HI_KEY_DRIVER_ITEM(keyId);
    if(pKeyCtrl->bInit==HI_FALSE)
    {
        return HI_ERR_INITILIZATION;
    }
    if(keyId >= HI_DRV_KEY_NUM_MAX)
    {
        return HI_ERR_INVALID_PARAMETER;
    }
    pKeyItem->pKeyFunc = HI_NULL;
    return HI_ERR_SUCCESS;
}

HI_U32 HI_DRV_KEY_Init(HI_VOID)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_KEY_DRIVER_CTRL_S *pKeyCtrl = HI_KEY_DRIVER_CTRL();
    if(pKeyCtrl->bInit==HI_FALSE)
    {
        ret = HI_LATTICE_Init();
        if(ret!=HI_ERR_SUCCESS)
        {
            return ret;
        }

        ret = key_isr_enable(HI_TRUE);
        if(ret!=HI_ERR_SUCCESS)
        {
            return ret;
        }
        KEY_DBG_PRINT0("key_init_sucss");
        pKeyCtrl->bInit = HI_TRUE;
    }
    return HI_ERR_SUCCESS;
}
#endif//PRODUCT_CFG_SUPPORT_KEY