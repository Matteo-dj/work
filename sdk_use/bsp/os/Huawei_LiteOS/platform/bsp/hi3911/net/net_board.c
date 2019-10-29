#ifdef LOSCFG_DRIVERS_HIETH_SF
#include "ctrl.h"

void hieth_hw_mac_core_reset(struct hieth_netdev_local *ld)
{
    unsigned int v = 0;

    //mii_clk_sel
    v = GET_UINT32(0x101e0000 + 0x28);
    v &= ~(0x3 << 1);
    WRITE_UINT32(v, 0x101e0000 + 0x28);

    //peri_rstn
    v = GET_UINT32(0x101e0000);
    v |= (1 << 7);
    WRITE_UINT32(v, 0x101e0000);

#if defined (PRODUCT_CFG_FEATURE_NDM_AUTO_CHECK)  && defined  (PRODUCT_CFG_PRODUCT_TYPE_NDM )

    //����ʱ���ڹ��Ӳ������оƬ��Χ��λ��·���� DTS2014092804484
    //����ʱΪ�����ܣ����ڼ�С�����Ի�������ʣ����ܴӸ����Ͻ�����⡣����Ӳ���޸ĺ��迼��ȥ������ʱ��
    LOS_Mdelay(100);//100ms: ����ֵ����������ʱ���Ż�ʱ�迼�Ǹ�ֵ��  ( 50ms���������Ի���ͨ�� )

#endif

    /* enable eth clk */
    v = GET_UINT32(0x101e0000 + 0x20);
    v |= (1 << 2);
    WRITE_UINT32(v, 0x101e0000 + 0x20);


    /* sys reset */
    v = GET_UINT32(0x101e0000 + 0x2c);
    v &= ~(1 << 3);
    WRITE_UINT32(v, 0x101e0000 + 0x2c);

}

/*BOOT���Ѿ���ǰ��λ��OS�²��ٸ�λ*/
void hieth_hw_external_phy_reset(void)
{

}
#endif
