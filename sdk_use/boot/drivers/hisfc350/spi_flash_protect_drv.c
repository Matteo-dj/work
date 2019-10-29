
#include "hisfc350.h"
#include "reg.h"
#include "spi_ids.h"
#include "spi_flash_protect_drv.h"
#include <debug.h>
#include "platform.h"

#ifdef PRODUCT_CFG_FLASH_PROTECT



/*****************************************************************************
 �� �� ��  : hisfc350_set_protect_magic
 ��������  : дflash�������Ƶ�magic��U16���͡���kernel�¶���magic�����ж��Ƿ�ʹ��flash�������ơ�
             ��ǰflash����ֻ��CS==1�ǿ����õģ���ֻ֧��һƬflash����оƬ��ַ�ռ���д�0x30000000��ʼ��
             ������probeʱCS==1ʱ�����ø�magic��֧����Ƭflash�ĳ����ݲ����ǣ�Ҳ���Թر�flash������������
 �������  : 
 �������  : ��
*****************************************************************************/
void hisfc350_set_protect_magic(struct spi_info *spiinfo)
{
    unsigned int value = 0;

    if(spiinfo->protecttype)
    {   
        print_msg0("Support flash protect");
        value = readl(GENERAL_REG_FLASH_PROTECT_MAGIC) & 0xffff0000;//ֻ��u16
        value |= ((unsigned int)FLASH_PROTECT_MAGIC);
        writel(value, GENERAL_REG_FLASH_PROTECT_MAGIC); 
    }
}

/*****************************************************************************
 �� �� ��  : hisfc350_change_protect_area
 ��������  : ����״̬�Ĵ�����ֵ�ı䱣������
 �������  : ulStatusRegVal ��д��Ŀ��Ʊ��������״̬�Ĵ�����ֵ
 �������  : ��
*****************************************************************************/
int hisfc350_change_protect_area(struct hisfc_host *host, unsigned int ulStatusRegVal)
{   
    unsigned int ulRegval = 0;
    unsigned int ulTimes = 0;
    int ulRet = 0;
    struct hisfc_spi *spi = host->spi;
    
    if(hisfc350_wait_ready(host, spi))
    {
        return -1;
    }
    hisfc350_wp_protect_ctrl(host,0);
    udelay(1);//wp�ܽ��õͣ���״̬�Ĵ�����д  ��Ҫ����20ns��������cs��Ϊ��
    hisfc350_write_enable(host, spi);
    do
    {
        hisfc350_write_status_reg(host, spi, SPI_CMD_WRSR1, ulStatusRegVal, 1);
        if(hisfc350_wait_ready(host, spi))
        {
           return -1;//���һֱæ���Ͳ���wp��Ч�ˣ�ԭ����:��һ��Ͳ�æ�ˣ�Ȼ��ֵд��ȥ�ˣ�ȱ�����´�дǰ512k֮ǰ״̬�Ĵ�����û��wpȥ������
                     //��һֱæ���ǰ�wp��Ч������д��ֵ����ֱ�Ӿʹ��ˣ���Ϊûд�ꡣ
        }
        ulRegval = hisfc350_read_status_reg(host, spi, SPI_CMD_RDSR);
        ulTimes++;
    }while((ulRegval != ulStatusRegVal) && (ulTimes < SPI_PROTECT_MAX_TIMES_WR_STATUS_REG));//����д����

    if((SPI_PROTECT_MAX_TIMES_WR_STATUS_REG == ulTimes) && (ulRegval != ulStatusRegVal))
    {
        ulRet  = -1;
        //д���ζ�д����ȥ�Ͳ����������ڹ���״̬����������״̬�Ĵ���wp��Ч
    }
    udelay(1);//cs�øߺ����ٵ�100ns�ſ���wpΪ��
    hisfc350_wp_protect_ctrl(host,1);
    return ulRet;

}

/*****************************************************************************
 �� �� ��  : hisfc350_shrink_protect_area
 ��������  : ���ݵ�ַ��ȥʹ��flash��������Ӧ����
 �������  : ulOffset:flashƫ�Ƶ�ַ
 �������  : ��
*****************************************************************************/

u_char g_bShrinkInit = 0;

//��������0��ʾ���Լ�������,���ط�0��ʾ�����Լ�������
int hisfc350_shrink_protect_area(unsigned int ulOffset)
{
    u32 regval = 0;
    int ret = 0;
    struct hisfc_host *host = &hisfc_host;
    struct hisfc_spi *spi = host->spi;

    //boot����magic�жϡ�kernel����ȫ�ֱ���bEnableMechanism�ж�
    //ԭ��:kernel����ά��ɾ�������Ĺ��ܣ�����ɾ��magic������˵������ɾ�˻���boot��ûд��
    if( hisfc350_get_protect_magic() !=  FLASH_PROTECT_MAGIC)
    {
        return 0;//��֧�־Ͳ��ܶ�״̬�Ĵ����������Ĵ����������ܲ�ͬ��
    }

    // TODO:��ʱ����switch-case�ж�flash�б��supporttype��
    // TODO: ԭ��:��ǰֻ������֧�ֱ��������Ӵ��������ȫ���Ӷȣ�����������type2�ټӡ�
    
    //�ϵ���һ�β���дflashʱ���ж��ǲ����쳣����ֵ��
    //ԭ��:1����д״̬�Ĵ���ʱ�ϵ��д����������512K��ַ����ס�ˣ���boot�»����д����ģ����ʧ��
    //     2��������:��kernel�����ı��˱�����ַ��������ǰ1M�����µ����ϵ����д512K~1M�ͻ�ʧ�ܡ�
    //�ϵ��ֻ�ж�һ�ξͿ��ԣ�����Ϊ�ϵ�ǰ�ǲ��ɿ���״ֵ̬���򲻼��ݵ�ֵ���������ڿɿط�Χ�ڡ�
    //     ÿ�ζ��жϻ�����flash����ռ�ã����ӳ�����գ��ҷ�ʱ����Ч�ʡ�
    
    if(!g_bShrinkInit)
    {                                        
     //   print_msg1("FIRST_SHRINK_MAGIC:", g_ucFirstShrink);
     
        regval = hisfc350_read_status_reg(host, spi, SPI_CMD_RDSR); 
        if(((regval&SPI_PROTECT_MASK_NONE) != SPI_PROTECT_NONE)//���Ϊ�쳣ֵ������WP�ܽ�δ���ӻ��𻵣��ǽⲻ�������ġ�
#ifdef PRODUCT_CFG_PRODUCT_FLASH_PROTECT_SUPPORT_32KB
            && ((regval&SPI_PROTECT_MASK_32KB)!=SPI_PROTECT_32KB)
#endif
            && ((regval&SPI_PROTECT_MASK_512KB)!=SPI_PROTECT_512KB))
            {
                regval = SPI_PROTECT_NONE;
                ret = hisfc350_change_protect_area(host, regval);   //Ҫ�з���ֵ�жϣ���д�ӿ�Ҫ��
                if(ret)
                {
                    return ret;
                }
            }
        g_bShrinkInit = 1;
    }
    
    if(ulOffset >= _512K)
    {
        return 0;
    }

    
    regval = hisfc350_read_status_reg(host, spi, SPI_CMD_RDSR); 
    
 //   print_msg1("Old Status Reg1:", regval);
#ifdef PRODUCT_CFG_PRODUCT_FLASH_PROTECT_SUPPORT_32KB
    if(ulOffset < _32K)//��Ƭȥ������
#endif
    {                
        if((regval&SPI_PROTECT_MASK_NONE) != SPI_PROTECT_NONE)
        {
            regval = SPI_PROTECT_NONE;
            ret = hisfc350_change_protect_area(host, regval);//Ҫ�з���ֵ�жϣ���д�ӿ�Ҫ��
            if(ret)
            {
                return ret;
            }
        }

    }
#ifdef PRODUCT_CFG_PRODUCT_FLASH_PROTECT_SUPPORT_32KB
    /*�Ᵽ��ǰ��״ֻ̬Ҫ���Ǳ�����32K��Ҳ����ȫ���ޱ������͸���Ϊһ��ȷ����״̬:����סǰ32K*/
    else if((ulOffset >=_32K ) && (ulOffset < _512K))
    {
        if(( (regval&SPI_PROTECT_MASK_32KB)!=SPI_PROTECT_32KB) && ((regval&SPI_PROTECT_MASK_NONE)!=SPI_PROTECT_NONE))
        {
            regval = SPI_PROTECT_32KB;
            ret = hisfc350_change_protect_area(host, regval);//Ҫ�з���ֵ�жϣ���д�ӿ�Ҫ��
            if(ret)
            {
                return ret;
            }
        }
    }
#endif
    return 0;

}




#endif

