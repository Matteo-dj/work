
#ifndef __HI_ASM_FUNC_H__
#define __HI_ASM_FUNC_H__
#include <hi_types.h>
#include <hi_mdm_types.h>

HI_PRVL HI_U32 HI_ASM_GetLr(VOID)
{
    HI_U32 uvFpSave;

    __asm__ __volatile__(
        "mov %0, lr"
        : "=r"(uvFpSave));

    return uvFpSave; /*lint !e530*/
}
#endif

