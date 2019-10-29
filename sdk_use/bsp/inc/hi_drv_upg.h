
#ifndef __HI_DRV_UPG_H__
#define __HI_DRV_UPG_H__
#include <hi_types.h>
#include <hi_mdm_types.h>


HI_U32 HI_DRV_UPG_GetNvFile(HI_U8 *pucNvFile);
HI_U32 HI_DRV_UPG_FindCompatibleNv(HI_UPG_FILE_HEADER_S *head, HI_UPG_SECTION_ID_S *sector);
#endif

