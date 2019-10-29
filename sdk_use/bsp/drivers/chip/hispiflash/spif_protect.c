


#include "hi_types.h"
#include "hi_mdm_types.h"
#include <spifdrv.h>
#include <spiflash.h>
#include <spif_protect_drv.h>
#include <spif_protect.h>
#include "hi_platform.h"
#include "spi_ids.h"


#ifdef PRODUCT_CFG_FLASH_PROTECT

HI_U32 HI_FLASH_ProtectProcess(HI_VOID);


SPIF_PROTECT_HOST_S g_stProtectHost;

/*****************************************************************************
 �� �� ��  : spif_protect_mechanism_is_enable
 ��������  : �鿴�Ƿ�ʹ�ܱ�������
 �������  : ��
 ����ֵ    : HI_TRUE ʹ��;HI_FALSE û��ʹ��
*****************************************************************************/
HI_BOOL spif_protect_mechanism_is_enable(HI_VOID)
{
    if(FLASH_PROTECT_ENABLE_MECHANISM == g_stProtectHost.ulEnableMechanism)
    {
        return HI_TRUE;
    }
    return HI_FALSE;
}

/*****************************************************************************
 �� �� ��  : spif_get_recover_time
 ��������  : ��ѯ�ڶ������ᱣ��סǰ512K
 �������  : ��
 ����ֵ    : ʱ�䣬��λ:��
*****************************************************************************/
#define spif_get_recover_time()     (g_stProtectHost.ulRecoverTime)

/*****************************************************************************
 �� �� ��  : spif_protect_none
 ��������  : ��Ƭflash��ȥ������
 �������  : ��
 �������  : ��
 ����ֵ    : HI_ERR_TIMEOUT æ����ȥ��������HI_ERR_SUCCESS�ɹ�
             HI_ERR_NOT_SUPPORTԭ���Ͳ�֧�ֱ���
*****************************************************************************/
#define spif_protect_none()     spif_shrink_protect_area(0)

/*****************************************************************************
 �� �� ��  : HI_FLASH_PortectInit
 ��������  : flash������ʼ��
 �������  : ��
 �������  : ��
 ����ֵ    :HI_ERR_SUCCESS��ʼ���ɹ�
*****************************************************************************/
HI_U32 HI_FLASH_PortectInit(HI_VOID)//��ʹ��ʼ��ʧ��Ҳ����Ӱ��������д���ܣ�ֻҪ��дflash��ͻ��ڿ��Ź����ٴα�������flash��
{   
    HI_U32 ulRet = HI_ERR_SUCCESS;
    SPIF_PROTECT_HOST_S *pstProtectHost = &g_stProtectHost;
    HI_U16 usProtectType = 0;
    (hi_void)memset_s(pstProtectHost, sizeof(SPIF_PROTECT_HOST_S), 0x0, sizeof(SPIF_PROTECT_HOST_S));

    ulRet = FLASH_LOCK();
    if (HI_ERR_SUCCESS != ulRet)
    {
        return ulRet;		 
    }
    
    usProtectType = spif_get_spiinfo()->usProtectType;
    if((spif_get_protect_magic() != FLASH_PROTECT_MAGIC))
    {    
        if(FLASH_PROTECT_NOT_SUPPORT != usProtectType)//��flash�ͺ���boot�²�֧�ֱ�������kernel��֧�֣���ȫƬ�⿪����
        {
            pstProtectHost->ulEnableMechanism = FLASH_PROTECT_ENABLE_MECHANISM;//��ʱʹ�ܱ�����ȫƬ�������־����Ϊ������   
            ulRet = spif_protect_none();
            pstProtectHost->ulEnableMechanism = 0;   
            if(HI_ERR_SUCCESS != ulRet)
            {
                FLASH_UNLOCK();
                return ulRet;
            }

        }
        FLASH_UNLOCK();
        return HI_ERR_NOT_SUPPORT;
    }
    if(FLASH_PROTECT_NOT_SUPPORT == usProtectType)//boot֧�ֱ�������kernel�б�ÿ�flash�в�֧��
    {
        FLASH_UNLOCK();
        return HI_ERR_NOT_SUPPORT;
    }
         
    pstProtectHost->ulEnableMechanism = FLASH_PROTECT_ENABLE_MECHANISM;    
    HI_SYS_RegisterWdMonitorTaskHook(HI_FLASH_ProtectProcess); 
    
    ulRet = spif_protect_all();
    if(HI_ERR_SUCCESS != ulRet)
    {//1.�����ʼ��ʧ���ˣ������д���ֵ��������Ԥ�����򱣻������ˣ��ÿ����Ź����Ӻ���ȥ���Ų��ϳ���ȥ�ָ�������512K
     //2.��ʹ��ʼ��ʧ���ˣ�Ҳ�����ñ�������(bIsRemoveProtect������1)����Ϊ����д���ֵ��Ԥ�ڲ������������ڴ�λ�ñ���ס��,�ÿ�flash��������ȥ�⿪����
        pstProtectHost->ulRecoverTime = 1;
    }
    FLASH_UNLOCK();
    return HI_ERR_SUCCESS;

}

/*****************************************************************************
 �� �� ��  : HI_FLASH_ProtectProcess
 ��������  : �ڸýӿ����ж�ʹ�ܱ���ǰ512K��ʱ����Ƿ��ڣ������򱣻�סǰ512K.
             �ýӿ�ע�ᵽ�˿��Ź������У������Բ�ѯ�Ƿ���ʹ��ʱ��㡣
 �������  : 
 �������  : ��
 ����ֵ    : ����ֵ ʧ�ܣ�HI_ERR_SUCCESS�ɹ�
*****************************************************************************/
HI_U32 HI_FLASH_ProtectProcess(HI_VOID)
{
    HI_U32 ulsec = 0;
    HI_U32 ulRet = HI_ERR_FAILURE;

    if(!spif_protect_mechanism_is_enable())
    {
        return HI_ERR_NOT_SUPPORT;
    }

    if(spif_get_recover_time())//����ʱ��Ϊ0��ʾ�Ѿ����ڱ���512k״̬��
    {          
        ulsec = HI_MDM_GetSeconds();
        if(HI_ERR_FAILURE == ulsec) 
        {
            return ulsec;
        }
        
        ulRet = FLASH_LOCK();
        if (HI_ERR_SUCCESS != ulRet)
        {
            return ulRet;		 
        }
        if(ulsec > spif_get_recover_time())
        {            
            spif_protect_all();          
        }
        FLASH_UNLOCK();
    }  
    return HI_ERR_SUCCESS;
}

/*****************************************************************************
 �� �� ��  : HI_FLASH_RemoveProtect
 ��������  : �Ƴ�flash�����Ĺ��ܣ��´��ϵ���ָ�flash����
 �������  : ��
 �������  : ��
 ����ֵ    : ����ֵ ʧ�ܣ�HI_ERR_SUCCESS�ɹ�
*****************************************************************************/
HI_U32 HI_FLASH_RemoveProtect(HI_VOID)
{
    HI_U32 ulRet = HI_ERR_FAILURE;
    SPIF_PROTECT_HOST_S *pstProtectHost = &g_stProtectHost;

    ulRet = FLASH_LOCK();
    if (HI_ERR_SUCCESS != ulRet)
    {
        return ulRet;        
    }
    ulRet = spif_protect_none();//����֧��flash��������ʲô������������
    if(HI_ERR_SUCCESS != ulRet)//��֤ͬʱִ��
    {
        FLASH_UNLOCK();
        return ulRet;
    }
    pstProtectHost->ulEnableMechanism = 0;
    FLASH_UNLOCK();
    return HI_ERR_SUCCESS;
}

/*****************************************************************************
 �� �� ��  : HI_FLASH_DfxGetProtectInfo
 ��������  : flashά�⹦��,���ڻ�ȡ��Ϣ:magic,״̬�Ĵ���1ֵ
 �������  : pMagicħ���֣�pStatusVal״̬�Ĵ���1��ֵ
 �������  : ��
 ����ֵ    : ����ֵ ʧ�ܣ�HI_ERR_SUCCESS�ɹ�
*****************************************************************************/
HI_U32 HI_FLASH_DfxGetProtectInfo(HI_BOOL *pbSupportProtect,HI_U8 *pucStatusVal)
{
    HI_U32 ulRet = HI_ERR_FAILURE;
    SFC_HOST_S *pstHost = &g_stSfc300Host;
    
    ulRet = FLASH_LOCK();
    if (HI_ERR_SUCCESS != ulRet)
    {
        return ulRet;        
    }
    *pbSupportProtect = spif_protect_mechanism_is_enable();
    *pucStatusVal = (HI_U8)spif_read_status_reg(pstHost, pstHost->stSpiFlashInfo, SPI_CMD_RDSR);
  
    FLASH_UNLOCK();
    return HI_ERR_SUCCESS;

}
#else

HI_U32 HI_FLASH_PortectInit(HI_VOID){return HI_ERR_SUCCESS;}

#endif
