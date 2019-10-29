#include <hi_types.h>
#include <hi_mdm_types.h>
#include <hi_reg_sys.h>
#include <hi_ft_nv.h>
#include <hi_mdm_sys.h>
#include <hi_mdm_clock.h>


HI_U32 usr_clock_init(void)
{
    HI_U32 ulClose1Bits,ulClose2Bits;
    HI_FTM_CHIP_TYPE_E enChipType=HI_MDM_GetHwChipType();

    ulClose1Bits=(HI_SYSCTRL_CLKEN_REG_BIT_SHIFT_SHA//�ر�SHA
        |HI_SYSCTRL_CLKEN_REG_BIT_SHIFT_RSA//�ر�RSA
        |HI_SYSCTRL_CLKEN_REG_BIT_SHIFT_CIPHER//�ر�cipher
        |HI_SYSCTRL_CLKEN_REG_BIT_SHIFT_DT//�ر�DT
        |HI_SYSCTRL_CLKEN_REG_BIT_SHIFT_RNG//�ر������
        |HI_SYSCTRL_CLKEN_REG_BIT_SHIFT_I2C//�ر�i2c1
        //|HI_SYSCTRL_CLKEN_REG_BIT_SHIFT_1M//�ر�����
        |HI_SYSCTRL_CLKEN_REG_BIT_SHIFT_SSP);//�ر�spi1    

    if(enChipType==HI_FTM_CHIP_TYPE_3911T)
    {
        ulClose1Bits|=(HI_SYSCTRL_CLKEN_REG_BIT_SHIFT_ETH|HI_SYSCTRL_CLKEN_REG_BIT_SHIFT_MII);//�ر�eth,miiʱ��
    }
    HI_MDM_SYS_CLOCK_Disable(HI_SYSCTRL_CLKEN_REG,ulClose1Bits);

    
    ulClose2Bits=(HI_SYSCTRL_PHY_WRAP_CLK_EN_REG_BIT_SHIFT_I2C2|HI_SYSCTRL_PHY_WRAP_CLK_EN_REG_BIT_SHIFT_SSP2);//�ر�spi2,i2c2
    HI_MDM_SYS_CLOCK_Disable(HI_SYSCTRL_PHY_WRAP_CLK_EN_REG,ulClose2Bits);
    return HI_ERR_SUCCESS;
}

