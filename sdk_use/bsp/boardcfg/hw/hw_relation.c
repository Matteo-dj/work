#include <hi_types.h>
#include <hi_mdm_types.h>
#include <hi_mdm_io.h>
#include <hi_hw_id.h>
#include <hi_mdm_time.h>

HI_BOOL g_bRebootPrepared;
HI_U32 HI_BOARD_RebootPrepare(HI_VOID)
{
    /*解决CCO复位进入jtag模式问题:
      1.芯片上电时如果检测到pin HI_GPIO_IDX_14为高电平,芯片进入jtag模式
      2.当前HI_GPIO_IDX_14复用为uart1的tx管脚默认高电平,硬件设计上为了增强抗ESD能力在外部增加了电容。
      3.当芯片复位时电容不能立即释放电荷导致上电时检测到pin为高电平，芯片进入jtag模式
    */
    if(HI_DMS_IsCcoHardWare()&&(!g_bRebootPrepared))
    {
        HI_MDM_IO_SetDir(HI_GPIO_IDX_14,HI_GPIO_DIRECTION_IN);
        HI_MDM_IO_SetMux(HI_HW_UART1_TXD_IO14,HI_GPIO14_SEL_GPIO14);
        HI_MDM_Udelay(20);//盲等
        g_bRebootPrepared=HI_TRUE;
    }
    return HI_ERR_SUCCESS;
}
