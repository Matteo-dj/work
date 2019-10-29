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

    //此延时用于规避硬件网口芯片外围复位电路问题 DTS2014092804484
    //此延时为软件规避，在于减小网口自环出错概率，不能从根本上解决问题。后续硬件修改后需考虑去掉该延时。
    LOS_Mdelay(100);//100ms: 测试值，后续启动时间优化时需考虑该值。  ( 50ms测试网口自环不通过 )

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

/*BOOT下已经提前复位，OS下不再复位*/
void hieth_hw_external_phy_reset(void)
{

}
#endif
