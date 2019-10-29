
#ifndef __HI_MDM_CLOCK_H__
#define __HI_MDM_CLOCK_H__
#include <hi_types.h>
#include <hi_mdm_types.h>
#include <hi_reg_sys.h>

//操作CLK相关寄存器，后续如果低功耗相关策略导致时钟操作特别频繁,则增加关中断保护。
#define HI_MDM_SYS_CLOCK_Enable( reg,bits )    do{HI_U32 _ulBits_ = HI_REG_READ_VAL32((reg));_ulBits_|=(bits);HI_REG_WRITE32((reg),_ulBits_);}while(0)
#define HI_MDM_SYS_CLOCK_Disable( reg,bits )  do{HI_U32 _ulBits_ = HI_REG_READ_VAL32((reg));_ulBits_&=(~(bits));HI_REG_WRITE32((reg),_ulBits_);}while(0)
#define HI_MDM_SYS_CLOCK_Set( reg,bits )      do{HI_REG_WRITE32((reg),(bits));}while(0)


#endif