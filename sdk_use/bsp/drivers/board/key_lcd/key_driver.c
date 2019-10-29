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
������:key_get_stat
����:��ȡ�����ж�״̬������հ����ж�
����:
    key_state:ÿbit��ʾһ������״̬
����ֵ:
    HI_ERR_SUCCESS:����Ч��������
    HI_ERR_KEY_ERR_VALUE:������Ч��������
    HI_ERR_INVALID_PARAMETER:��������
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
    *pValidKeyBit = (ks_val[0]&LATTICE_KEY_MASK)>>5;  //����KS0��Ӧ��k9/k8/k7/k6
    *pValidKeyBit |= (ks_val[1]&LATTICE_KEY_MASK)>>1;//����KS1��Ӧ��k9/k8/k7/k6
    *pValidKeyBit |= (ks_val[2]&LATTICE_KEY_MASK)<<3;//����KS2��Ӧ��k9/k8/k7/k6
    KEY_DBG_PRINT4("key :%x,%x,%x,%x",*pValidKeyBit, ks_val[0],ks_val[1],ks_val[2]);

    if((ks_val[0]&(HI_U16)(~LATTICE_KEY_MASK))|(ks_val[1]&(HI_U16)(~LATTICE_KEY_MASK))|
        (ks_val[2]&(HI_U16)(~LATTICE_KEY_MASK)))
    {
        //����Ч�������������쳣����
        if (0 == *pValidKeyBit)
        {
            return HI_ERR_KEY_ERR_VALUE;
        }
        //��Ч����Ч��������
        else
        {
            //��Ȼ���سɹ�����Ч������Ҫ�������������ͳ�ƴ���
        }
    }
    return HI_ERR_SUCCESS;
}


/**************************************************************
������:key_timer_isr
����:�����жϺ���
����:
    gpio_key_data:��Ч����
����ֵ:

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

        if(usValidKeyBit&(1<<i))//��������
        {
            pKeyItem->ulKeyPressMs += ulAddMs;
            if(pKeyItem->ulKeyPressMs>=HI_DRV_KEY_PRESS_TIME_PRECISION_MS)
            {
                key_func(HI_TRUE,pKeyItem->ulKeyPressMs,ulUsrData);//�ص��д�����,Ӱ�����а�����ʱ
            }
        }
        else
        {
            pKeyItem->ulKeyPressMs += ulAddMs;
            if(pKeyItem->ulKeyPressMs>=(HI_DRV_KEY_PRESS_TIME_PRECISION_MS<<1))
            {
                key_func(HI_FALSE,pKeyItem->ulKeyPressMs,ulUsrData);//�ص��д�����,Ӱ�����а�����ʱ
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
        HI_MDM_Sleep(10);//��ֹ��Ч��������,���ϲ����ж�,��ѭ��
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

    //��ȡ����״̬
    ret = key_get_stat(&usValidKeyBit);
    if(ret!=HI_ERR_SUCCESS)
    {
        //��ȡ����״̬ʧ��,������̧����
        usValidKeyBit = 0;
    }

    //�������¼�,ʹ�ܰ����ж�
    if(usValidKeyBit==0)
    {
        key_up_down(usValidKeyBit,ulAddMs);
        key_isr_enable(HI_TRUE);
    }
    else//������ʱ��,������ʱ��
    {
        key_up_down(usValidKeyBit,ulAddMs);
        modify_key_press_timer();
    }
    return HI_ERR_SUCCESS;
}


/**************************************************************
������:HI_DRV_KEY_Register
����:ע�ᰴ��
����:
keyId:����ID
keyFunc:�����ص�����
usrData:�ص�����
����ֵ:
***************************************************************/
HI_U32 HI_DRV_KEY_Register(HI_DRV_KEY_NUM_E keyId,key_up_down_func keyFunc,HI_U32 usrData)
{
    HI_KEY_DRIVER_CTRL_S *pKeyCtrl = HI_KEY_DRIVER_CTRL();
    HI_KEY_DRIVER_ITEM_S *pKeyItem =  HI_KEY_DRIVER_ITEM(keyId);
    if(pKeyCtrl->bInit==HI_FALSE)//Ҳ���Բ��ж�,�ڳ�ʼ��֮ǰ֧���û�ע�ᵫ��ʵ���ô�
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
������:HI_DRV_KEY_UnRegister
����:ȥע�ᰴ��
����:
keyId:����ID
����ֵ:
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