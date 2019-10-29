#ifndef __DRV_COM_H__
#define __DRV_COM_H__
#include <hi_ft_nv.h>

//sdram test begin
#define HI_SDTEST_INIT_VALUE 0x11111111
#define HI_SDTEST_SD_ADDR_NUM 7
#define HI_SDTEST_FRIST_ADDR  0x70000400
#define HI_SDTEST_ADDR_ADD    0x400
//sdram test end


HI_VOID HI_COM_GetProductVer(HI_FTM_PRODUCT_TYPE_E *pro_ver);

#endif//__DRV_COM_H__

