#ifndef __DRV_DFX_COM_H__
#define __DRV_DFX_COM_H__
/************************************************************/
//
// �汾�Ŷ���, ��ʽΪ x,y,z ��ʾvx.y.z, x/y/zΪ����,ÿ���������Ϊ999
//
#define  DRV_VERSION()  HI_SYS_CONFIG_DRV_VER(0,0,1)
#define  OS_VERSION()  HI_SYS_CONFIG_OS_VER(0,0,1)
 
//#define DRV_DFX_SYS_VAL_DECLARE 
//#include "drv_dfx_sys_val.h"
HI_U32 HI_DRV_DFX_Init(HI_VOID);

#endif