
#ifndef __DRV_DFX_SHELL_H__
#define __DRV_DFX_SHELL_H__
#include <hi_types.h>
#include <hi_mdm_types.h>

typedef struct
{
    HI_U32 argc;//��������
    HI_CHAR *argv[10];//�����б�
    HI_CHAR *pReq;//bufָ��:��Ҫ���ڴ洢req
    HI_CHAR *pOutLineBuf;//bufָ��:��Ҫ���ڴ洢ind
}HI_SHELL_CMD_PRE_DO_HANDLE;

//*****************************************************************************
// ��������:  diag_shell_cmd_prepare
//
// ��������: Ԥ������Դ,�������ַ�������ת�ɶ������
//
// ����˵��:
//      pCmdParam:diag��pCmdParam
//      usCmdParamSize:diag��usCmdParamSize
//      ulOutLineSize:Ҫ�����pOutLineBuf��size
// �� �� ֵ:
//      HI_NULL:������Դʧ��
//*****************************************************************************

HI_SHELL_CMD_PRE_DO_HANDLE *diag_shell_cmd_prepare(HI_PVOID pCmdParam,HI_U32 usCmdParamSize,HI_U32 ulOutLineSize); 

HI_VOID diag_shell_cmd_restore(HI_SHELL_CMD_PRE_DO_HANDLE *pHandle);

#endif

