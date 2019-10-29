
#include <spifdrv.h>
#include <spif_protect.h>
#include <spif_protect_drv.h>

#ifdef PRODUCT_CFG_FLASH_PROTECT
/*****************************************************************************
 �� �� ��  : spif_change_protect_area
 ��������  : ����״̬�Ĵ�����ֵ�ı䱣������
 �������  : ulStatusRegVal ��д��Ŀ��Ʊ��������״̬�Ĵ�����ֵ
 �������  : ��
 ע������  :�����߱�֤flash����busy״̬
*****************************************************************************/
HI_U32 spif_change_protect_area(SFC_HOST_S *pstHost, HI_U32 ulStatusRegVal)
{
    HI_U32 ulRegval = 0;
    HI_U32 ulTimes = 0;
    HI_U32 ulRet = HI_ERR_SUCCESS;
    SPI_FLASH_INFO_S *pstInfo = pstHost->stSpiFlashInfo;
    
    if(spif_wait_ready(pstHost, pstInfo, 0))//���һֱæ����ò�Ҫ����סflash�������дʧ���ˣ���������С�¡�
	{
		return HI_ERR_TIMEOUT;
	}
    
    spif_wp_protect_ctrl(pstHost,HI_FALSE);
    HI_HRTIMER_Udelay(1);                   //wp�ܽ��õͣ���״̬�Ĵ�����д  ��Ҫ����20ns��������cs��Ϊ��
    spif_write_enable(pstHost, pstInfo);
    do
    {
        spif_write_status_reg(pstHost, pstInfo, SPI_CMD_WRSR1, ulStatusRegVal,1);  
        if (spif_wait_ready(pstHost, pstInfo, 0))
    	{
    	    return HI_ERR_TIMEOUT;          //���һֱæ���Ͳ���wp��Ч�ˣ�ԭ����:��һ��Ͳ�æ�ˣ�Ȼ��ֵд��ȥ�ˣ�ȱ�����´�дǰ512k֮ǰ״̬�Ĵ�����û��wpȥ������
                                            //��һֱæ���ǰ�wp��Ч������д��ֵ����ֱ�Ӿʹ��ˣ���Ϊûд�ꡣ
    	}
        ulRegval = spif_read_status_reg(pstHost, pstInfo, SPI_CMD_RDSR);
        ulTimes++;
    }while((ulRegval != ulStatusRegVal) && (ulTimes<SPI_PROTECT_MAX_TIMES_WR_STATUS_REG));//����д����
    if((SPI_PROTECT_MAX_TIMES_WR_STATUS_REG == ulTimes) && (ulRegval != ulStatusRegVal))
    {
        ulRet =  HI_ERR_FLASH_UP_TO_MAX_TIMES_WR_REG; //д���ζ�д����ȥ�Ͳ����������ڹ���״̬����������״̬�Ĵ���wp��Ч

    }
    HI_HRTIMER_Udelay(1);   //cs�øߺ����ٵ�100ns�ſ���wpΪ��    
    spif_wp_protect_ctrl(pstHost,HI_TRUE);

    return ulRet;
}
/*Լ��: 
1�����д״̬�Ĵ���ʧ���ˣ���Ҫһֱȥ���ԣ�ֱ��д�ԣ�ԭ��:дʧ���ˣ�����ܰ�NV���������ˣ�Ӱ�칦�ܡ�
2��д״̬�Ĵ�����һֱæ������wp��Ч��ԭ��:���ܹ�һ��Ͳ�æ�ˣ���д��ȥ�ˣ���wp��Ч�������д���������
   ��дʧ���ˣ�д�겻æ���Ͱ�wp��Ϊ��Ч���������������ǻ�Ҫһֱ����ȥд����Լ��1��

*/
/*****************************************************************************
 �� �� ��  : spif_protect_all
 ��������  : ����ǰ512KB
 �������  : ��
 �������  : ��
*****************************************************************************/
HI_U32 spif_protect_all(HI_VOID)
{
    HI_U32 regval = 0;
    HI_U32 ulRet = HI_ERR_SUCCESS;
    SPIF_PROTECT_HOST_S *pstProtectHost = &g_stProtectHost;
    SFC_HOST_S * pstHost = &g_stSfc300Host;
    SPI_FLASH_INFO_S *pstInfo = pstHost->stSpiFlashInfo;
    
    // TODO:��ʱ����switch-case�ж�flash�б��supporttype��
    // TODO: ԭ��:��ǰֻ�л���֧�ֱ��������Ӵ��������Ȧ���Ӷȣ�����������type2�ټӡ�
    
    regval = spif_read_status_reg(pstHost, pstInfo, SPI_CMD_RDSR);    
    if((regval&SPI_PROTECT_MASK_512KB) != SPI_PROTECT_512KB)
    {  
        regval = (SPI_PROTECT_512KB | SPI_PROTECT_SET_SRP0);
        ulRet = spif_change_protect_area(pstHost,regval);
        if(HI_ERR_SUCCESS != ulRet)
        {
            return ulRet;//�������ʧ���ˣ�ʱ��ulEnableProtectSec�����㣬���ڿ��Ź������й�һ�ỹ��ȥ���Ա�������
        }
    } 
    pstProtectHost->ulRecoverTime = 0;//ʱ�����㣬��ʾ����״̬���Ǳ�����ǰ512K���ڿ��Ź������л��ж�
    return HI_ERR_SUCCESS;
}

/*****************************************************************************
 �� �� ��  : spif_recover_protect_after_set_time
 ��������  : ��һ��ʱ��󱣻�סǰ512K
 �������  : ulOffset flashƫ�Ƶ�ַ
 �������  : ��
*****************************************************************************/
HI_VOID spif_recover_protect_after_set_time(HI_U32 ulOffset)
{  
    SPIF_PROTECT_HOST_S *pstProtectHost = &g_stProtectHost;
    HI_U32 ulTime = 0;

    if(ulOffset >= _512K)//�������ӿڶ�����
    {
        return;
    }
#ifdef PRODUCT_CFG_PRODUCT_FLASH_PROTECT_SUPPORT_32KB       
    if(ulOffset < _32K)
    {
        pstProtectHost->ulRecoverTime = (HI_U32)(-1);//���޴�ʱ�䣬100���꣬����boot����ʱ����Ƭ�����ޱ���״̬
    }
    else  //[32k,512k)
#endif
    {
        ulTime = HI_MDM_GetSeconds();
        if(HI_ERR_FAILURE != ulTime)//ʱ������ȡʧ�ܣ���pstProtectHost->ulRecoverTimeһֱ���䣬ֱ���´β�д��ˢ��ʱ�䣬���ⲻ��
        {
            ulTime += SPI_PROTECT_ENABLE_TIME;
            if(pstProtectHost->ulRecoverTime < ulTime)
            {
                pstProtectHost->ulRecoverTime = ulTime;
            }
        }
    }
    
}

/*****************************************************************************
 �� �� ��  : spif_shrink_protect_area
 ��������  : ���ݵ�ַ��ʹflash��Ӧ������û�б���״̬,
           : �����趨��ʱ�䵽�ں�ָ��ɱ���ǰ512K״̬��
 �������  : ulOffset:flashƫ�Ƶ�ַ
 �������  : ��
 ˵��:HI_ERR_SUCCESS��ʾ���Լ�������д;����ֵ��ʾ���ɼ�������д��
*****************************************************************************/
HI_U32 spif_shrink_protect_area(HI_U32 ulOffset)
{
    HI_U32 regval = 0;
    HI_U32 ulRet = HI_ERR_SUCCESS;

    SFC_HOST_S * pstHost = &g_stSfc300Host;
    SPI_FLASH_INFO_S *pstInfo = pstHost->stSpiFlashInfo;

    if(!spif_protect_mechanism_is_enable())
    {
        return HI_ERR_SUCCESS;
    }
    
    // ��ʹ����512K��Ҳ�����쳣�ж��ˣ���kernel��ʼ�����ж����ǲ���512K��
    if(ulOffset >= _512K)
    {
        return HI_ERR_SUCCESS;
    }

    // TODO:��ʱ����switch-case�ж�flash�б��supporttype��
    // TODO: ԭ��:��ǰֻ�л���֧�ֱ��������Ӵ��������Ȧ���Ӷȣ�����������type2�ټӡ�
    
    regval = spif_read_status_reg(pstHost, pstInfo, SPI_CMD_RDSR);
#ifdef PRODUCT_CFG_PRODUCT_FLASH_PROTECT_SUPPORT_32KB
    if(ulOffset < _32K)//��Ƭȥ������
#endif
    {      
        if((regval&SPI_PROTECT_MASK_NONE) != SPI_PROTECT_NONE)//�ܷ��������ظ�����ȡ��һ������
        {
            regval = SPI_PROTECT_NONE|SPI_PROTECT_SET_SRP0;
            ulRet = spif_change_protect_area(pstHost,regval);
            if(HI_ERR_SUCCESS != ulRet)
            {
                //д����ȥֻ�ܷ������´ε��ò�д����ǰ512k�ӿں����ȥ���ԣ����ڿ��Ź�һֱ�����ˡ�
                return ulRet;
            }
        }
    }
#ifdef PRODUCT_CFG_PRODUCT_FLASH_PROTECT_SUPPORT_32KB    
    else if((ulOffset >=_32K ) && (ulOffset < _512K))           //�Ᵽ��ǰ��״ֻ̬Ҫ���Ǳ�����32K��Ҳ����ȫ���ޱ������͸���Ϊһ��ȷ����״̬:����סǰ32K
    {   
        if((regval&SPI_PROTECT_MASK_32KB) != SPI_PROTECT_32KB)  //��boot�´˴��ж����˸�����(regval&SPI_PROTECT_MASK_NONE)!=SPI_PROTECT_NONE)��
        {                                                       //��Ϊboot�������ǵķ��������²�ȥ���󱣻���Χ��kernel��Ҫ���󣬱�֤boot�������ڱ���״̬
            regval = SPI_PROTECT_32KB|SPI_PROTECT_SET_SRP0;
            ulRet = spif_change_protect_area(pstHost,regval);
            if(HI_ERR_SUCCESS != ulRet)
            {
                return ulRet;
            }

        }
    } 
#endif    
    spif_recover_protect_after_set_time(ulOffset);
    return HI_ERR_SUCCESS;
}

    

#endif