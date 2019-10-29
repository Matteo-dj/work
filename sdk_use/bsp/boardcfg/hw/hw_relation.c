#include <hi_types.h>
#include <hi_mdm_types.h>
#include <hi_mdm_io.h>
#include <hi_hw_id.h>
#include <hi_mdm_time.h>

HI_BOOL g_bRebootPrepared;
HI_U32 HI_BOARD_RebootPrepare(HI_VOID)
{
    /*���CCO��λ����jtagģʽ����:
      1.оƬ�ϵ�ʱ�����⵽pin HI_GPIO_IDX_14Ϊ�ߵ�ƽ,оƬ����jtagģʽ
      2.��ǰHI_GPIO_IDX_14����Ϊuart1��tx�ܽ�Ĭ�ϸߵ�ƽ,Ӳ�������Ϊ����ǿ��ESD�������ⲿ�����˵��ݡ�
      3.��оƬ��λʱ���ݲ��������ͷŵ�ɵ����ϵ�ʱ��⵽pinΪ�ߵ�ƽ��оƬ����jtagģʽ
    */
    if(HI_DMS_IsCcoHardWare()&&(!g_bRebootPrepared))
    {
        HI_MDM_IO_SetDir(HI_GPIO_IDX_14,HI_GPIO_DIRECTION_IN);
        HI_MDM_IO_SetMux(HI_HW_UART1_TXD_IO14,HI_GPIO14_SEL_GPIO14);
        HI_MDM_Udelay(20);//ä��
        g_bRebootPrepared=HI_TRUE;
    }
    return HI_ERR_SUCCESS;
}
