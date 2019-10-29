
#include <hi_types.h>
#include <hi_mdm_types.h>
#include <hi_sal.h>

#include "drv_dfx_shell.h"

/********************************************shell通用 st*******************************************************/

HI_U32 str_to_arg(HI_CHAR *pStr,HI_U16 ulMaxStrLen,HI_U32 *pArgc,HI_CHAR *pArgv[],HI_U32 ulMaxArgc)
{
	HI_U32 i = 0;
	HI_BOOL bPreeBlank = HI_TRUE;
	HI_U32 ulArgc = 0;

	if(ulMaxStrLen==0)
	{
		*pArgc = 0;
		return HI_ERR_SUCCESS;
	}

	//确保字符串都已结束符结尾
	pStr[ulMaxStrLen-1] = '\0'; 

	for(i=0;i<ulMaxStrLen;i++)
	{
		//命令结束
		if(pStr[i]=='\r'||pStr[i]=='\n'||pStr[i]=='\0')
		{
			pStr[i] = '\0';
			break;
		}
		
		
		if(pStr[i]==' '||pStr[i]=='\t')
		{
			pStr[i] = '\0';
			bPreeBlank = HI_TRUE;
			
			if(ulArgc>=ulMaxArgc)//词尾且参数满
			{
				break;
			}
		}
		else
		{
			//词首
			if(bPreeBlank)
			{
				if(ulArgc<ulMaxArgc)
				{
					pArgv[ulArgc] = &(pStr[i]);
					ulArgc++;
				}				 
			}
			bPreeBlank = HI_FALSE;
		}
	}
	*pArgc = ulArgc;

	return HI_ERR_SUCCESS;
}

HI_SHELL_CMD_PRE_DO_HANDLE *diag_shell_cmd_prepare(HI_PVOID pCmdParam,HI_U32 ulCmdParamSize,HI_U32 ulOutLineSize) 
{
    HI_CHAR *pOutLineBuf = HI_NULL;
    HI_SHELL_CMD_PRE_DO_HANDLE *pHandle = HI_NULL;
    HI_PVOID pReq = HI_NULL;

    pHandle = (HI_SHELL_CMD_PRE_DO_HANDLE *)HI_MDM_Malloc(HI_MOD_ID_DRV_DFX, sizeof(HI_SHELL_CMD_PRE_DO_HANDLE));
    if(pHandle==HI_NULL)
    {
        goto fail;
    }

    (hi_void)memset_s((HI_VOID *)pHandle, sizeof(HI_SHELL_CMD_PRE_DO_HANDLE), 0, sizeof(HI_SHELL_CMD_PRE_DO_HANDLE));

    if(ulOutLineSize)
    {
        pOutLineBuf = (HI_CHAR *)HI_MDM_Malloc(HI_MOD_ID_DRV_DFX, ulOutLineSize);
        if(pOutLineBuf==HI_NULL)
        {
            goto fail;
        }

        (hi_void)memset_s((HI_VOID *)pOutLineBuf, ulOutLineSize, 0, ulOutLineSize);
    }

    if(ulCmdParamSize)
    {
        pReq = (HI_PVOID)HI_MDM_Malloc(HI_MOD_ID_DRV_DFX, ulCmdParamSize);
        if(pReq==HI_NULL)
        {
            goto fail;
        }
        (hi_void)memcpy_s(pReq,ulCmdParamSize,pCmdParam,ulCmdParamSize);
    }
    
    pHandle->pOutLineBuf = pOutLineBuf;
    pHandle->pReq = (HI_CHAR *)pReq;
    if(HI_ERR_SUCCESS != str_to_arg((HI_CHAR *)pReq,(HI_U16)ulCmdParamSize,&(pHandle->argc),pHandle->argv,10))
    {
        goto fail;
    }
    
    return pHandle;
    
fail:
    HI_MDM_Free(HI_MOD_ID_DRV_DFX,pOutLineBuf);
    HI_MDM_Free(HI_MOD_ID_DRV_DFX,pReq);
    HI_MDM_Free(HI_MOD_ID_DRV_DFX,pHandle);
    return HI_NULL;
}

HI_VOID diag_shell_cmd_restore(HI_SHELL_CMD_PRE_DO_HANDLE *pHandle)
{
    HI_MDM_Free(HI_MOD_ID_DRV_DFX,pHandle->pOutLineBuf);
    HI_MDM_Free(HI_MOD_ID_DRV_DFX,pHandle->pReq);
    HI_MDM_Free(HI_MOD_ID_DRV_DFX,pHandle);
    return;
}
/********************************************shell通用 end*******************************************************/

