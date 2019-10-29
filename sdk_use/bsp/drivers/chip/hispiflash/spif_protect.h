
#ifndef __SPIF_PROTECT_H__
#define __SPIF_PROTECT_H__

#ifdef PRODUCT_CFG_FLASH_PROTECT


typedef struct
{
    HI_U32 ulEnableMechanism;//�Ƿ�ʹ�ܱ�������
    HI_U32 ulRecoverTime;//�ָ�����ǰ512K��ʱ�䡣ֵΪ0:����ס��512k;0xFFFFFFFF:���ò��ָ�����;����ֵ:��ʱ���ָ�����ǰ512K

}SPIF_PROTECT_HOST_S;

HI_EXTERN SPIF_PROTECT_HOST_S g_stProtectHost;

#define FLASH_PROTECT_ENABLE_MECHANISM              HI_MAKE_IDENTIFIER('F','P','E','M')  //0x5046 0x45 0x4D  //�á�PF����ascii��  Protect Flash    0100 0110  0101 0000
#define FLASH_PROTECT_MAGIC                         HI_MAKEU16('F','P')  //0x5046  //�á�PF����ascii��  Protect Flash    0100 0110  0101 0000
#define spif_get_protect_magic()                    (HI_REG_READ_VAL32(GENERAL_REG_FLASH_PROTECT_MAGIC)&0xFFFF)


#endif

#endif
