
#ifndef __HI_3518_H__
#define __HI_3518_H__
#include <hi_types.h>
#include <hi_mdm_types.h>
#include <icc_protocal_upg.h>

#ifdef PRODUCT_CFG_SUPPORT_3518
HI_U32 HI_3518_VerInit(HI_VOID);
HI_U32 HI_3518_Reboot (HI_VOID);
HI_U32 HI_3518_GetVer(HI_3518_UPG_VER_S *pVer);

#else
HI_PRVL HI_U32 HI_3518_VerInit(HI_VOID){return HI_ERR_NOT_SUPPORT;}
HI_PRVL HI_U32 HI_3518_Reboot (HI_VOID){return HI_ERR_NOT_SUPPORT;}
HI_PRVL HI_U32 HI_3518_GetVer(HI_3518_UPG_VER_S *pVer){return HI_ERR_NOT_SUPPORT;}
#endif

#endif

