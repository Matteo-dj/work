
#ifndef __HI_DIAG_ASYNC_H__
#define __HI_DIAG_ASYNC_H__
#include <hi_mdm.h>
#include <hi_mdm_types.h>

typedef enum
{
    SAL_QUEUE_ASYNC_DIAG_MSG_CMD,
    SAL_QUEUE_ASYNC_DIAG_MSG_MAX,    
}SAL_QUEUE_ASYNC_DIAG_MSG_ID;

#if !defined(PRODUCT_CFG_EQUIP_TEST_MODE)
#if !defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
HI_EXTERN HI_U32 HI_DIAG_AsyncCmd(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption,HI_DIAG_CMD_F func);
#endif
#else
HI_PRVL HI_U32 HI_DIAG_AsyncCmd(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption,HI_DIAG_CMD_F func){return func(usId,pCmdParam,usCmdParamSize,ulOption);}
#endif
#endif

