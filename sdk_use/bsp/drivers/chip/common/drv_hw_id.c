#include <hi_types.h>
#include <hi_mdm_types.h>
#include <hi_ft_nv.h>
#include <hi_platform.h>
#include <hi_driver.h>
#include <hi_mdm_sys.h>
//����������doVerChipInit����
#define CHIP_VERSION_MASK       (0x0000FFF0)
#define CHIP_VERSION_V100       (0x100)
#define CHIP_VERSION_V200       (0x200)
#define CHIP_VERSION_ID_BIT_TYPE_ID     (1 << 1)

/*��ȡоƬ�汾*/
HI_HW_CHIP_VER_E HI_MDM_GetHwChipVer(HI_VOID)
{
    HI_U32 ulChipInfo = 0;
    HI_HW_CHIP_VER_E enChipVer = HI_CHIP_VERSION_UNKNOW;

    HI_REG_READ32(HI_SYSCTRL_REG_BASE + SC_VERSION_ID, ulChipInfo);
    ulChipInfo = (ulChipInfo & CHIP_VERSION_MASK)>>4;
    if (CHIP_VERSION_V200 == ulChipInfo)
    {
        enChipVer = HI_CHIP_VERSION_V200;
    }
    else if (CHIP_VERSION_V100 == ulChipInfo)
    {
        enChipVer = HI_CHIP_VERSION_V100;
    }

    return enChipVer;
}

/*��ȡоƬ����*/
HI_FTM_CHIP_TYPE_E HI_MDM_GetHwChipType(HI_VOID)
{
    HI_U32 asic_ver = 0;
    
    HI_REG_READ32(HI_SYSCTRL_REG_BASE + SC_VERSION_ID, asic_ver);

    if (HI_GET_BIT_I(asic_ver, 1))
    {
        return (HI_FTM_CHIP_TYPE_E)HI_FTM_CHIP_TYPE_3911T;
    }
    else
    {
        return (HI_FTM_CHIP_TYPE_E)HI_FTM_CHIP_TYPE_3911C;
    }
}

/*��ȡ�ڴ��С���ͣ���ǰ������Hi3911 V200����,V100��FPGA��Ҫ����ʵ�������ʵ��*/
#define REG_RAM_SIZE				(0x10110100)

HI_MEMORY_SIZE_E  HI_MDM_GetHwMemSize(HI_VOID)
{
    HI_U32 reg_value = 0;
#ifdef PRODUCT_CFG_BOARD_TYPE_ASIC_V200
   
    HI_REG_READ32(HI_SYSCTRL_REG_BASE + SC_VERSION_ID, reg_value);

    if (HI_GET_BIT_I(reg_value, 0))
    {
        return  HI_MEMORY_SIZE_8M;
    }

    if(HI_GET_BIT_I(reg_value, 1))//0��Hi3911C��1��Hi3911T��
    {
        return HI_MEMORY_SIZE_2M;
    }
    else
    {
        return HI_MEMORY_SIZE_16M;
    }
    
#elif defined(PRODUCT_CFG_BOARD_TYPE_FPGA)  
    /*FPGAĬ������Ϊ32M*/
    return HI_MEMORY_SIZE_32M;
#elif defined(PRODUCT_CFG_BOARD_TYPE_ASIC_V100)
    	//1.�ӼĴ����ж�ȡӲ��Ram��С�Ƿ��ǹ���汾��
    HI_REG_READ32(REG_RAM_SIZE,reg_value);


    //2.�洢Ӳ��ʵ��RAM��С
    if(reg_value == 0x280)
    {
        return HI_MEMORY_SIZE_8M;
    }
    else if(reg_value == 0x80)
    {
        return HI_MEMORY_SIZE_2M;
    }
    else
    {
        return HW_RAM_DEFAULT_SIZE;
    }
#endif    
}


