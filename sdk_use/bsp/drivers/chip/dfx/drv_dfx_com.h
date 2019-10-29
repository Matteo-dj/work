#ifndef __DRV_DFX_COM_H__
#define __DRV_DFX_COM_H__
/************************************************************/
//
// 版本号定义, 格式为 x,y,z 表示vx.y.z, x/y/z为数字,每个数字最大为999
//
#define  DRV_VERSION()  HI_SYS_CONFIG_DRV_VER(0,0,1)
#define  OS_VERSION()  HI_SYS_CONFIG_OS_VER(0,0,1)
 
//#define DRV_DFX_SYS_VAL_DECLARE 
//#include "drv_dfx_sys_val.h"
HI_U32 HI_DRV_DFX_Init(HI_VOID);

#endif