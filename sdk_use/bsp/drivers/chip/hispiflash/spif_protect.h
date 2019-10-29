
#ifndef __SPIF_PROTECT_H__
#define __SPIF_PROTECT_H__

#ifdef PRODUCT_CFG_FLASH_PROTECT


typedef struct
{
    HI_U32 ulEnableMechanism;//是否使能保护机制
    HI_U32 ulRecoverTime;//恢复保护前512K的时间。值为0:保护住了512k;0xFFFFFFFF:永久不恢复保护;其他值:该时间后恢复保护前512K

}SPIF_PROTECT_HOST_S;

HI_EXTERN SPIF_PROTECT_HOST_S g_stProtectHost;

#define FLASH_PROTECT_ENABLE_MECHANISM              HI_MAKE_IDENTIFIER('F','P','E','M')  //0x5046 0x45 0x4D  //用‘PF’的ascii码  Protect Flash    0100 0110  0101 0000
#define FLASH_PROTECT_MAGIC                         HI_MAKEU16('F','P')  //0x5046  //用‘PF’的ascii码  Protect Flash    0100 0110  0101 0000
#define spif_get_protect_magic()                    (HI_REG_READ_VAL32(GENERAL_REG_FLASH_PROTECT_MAGIC)&0xFFFF)


#endif

#endif
