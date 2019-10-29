//*****************************************************************************
//
//                  ��Ȩ���� (C), 1998-2011, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : main.c
//  �� �� ��   : V1.0
//  ��    ��   : shenhankun/00130424
//  ��������   : 2011-10-25
//  ��������   : CTS�������ʵ��
//
//  �����б�   :
//  �޸���ʷ   :
//  1.��    �� : 2011-10-25
//    ��    �� : shenhankun/00130424
//    �޸����� : �����ļ�
//
//*****************************************************************************


//*****************************************************************************
// PROJECT   :
// SUBSYSTEM :
// MODULE    :
// OWNER     :
//*****************************************************************************


//*****************************************************************************
#include "app_common.h"
#include "dfx_sys.h"
#include "app_demo_entry.h"
#include "demo_dfx_interface.h"
#include "hi_mdm_diag.h"
//*****************************************************************************

#define CTS_PRINT0(x)          HI_MDM_ReportPacket((HI_U32)HI_DCID_STR, (HI_U8)0, (HI_PBYTE)x, (HI_U16)(sizeof(int)-1), HI_FALSE)

//*****************************************************************************
// CTS ��������Ķ���
//*****************************************************************************
//HI_PRV CTS_CTRL_S g_stCtsCtrl;
//*****************************************************************************


//*****************************************************************************
// Loacal Interface
HI_PRV HI_U32 cts_ObjUsrInProc(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption);
//*****************************************************************************


//*****************************************************************************
// CTS���������
//*****************************************************************************

HI_PRV HI_CONST HI_DIAG_CMD_REG_OBJ_S g_astDemoCmnObjTbl[] = {
    {HI_DBG_DEMO_TEST, HI_DBG_DEMO_TEST, demo_ObjUsrInProc},
    {ID_DIAG_CMD_DRV_MEM_READ, ID_DIAG_CMD_DRV_MEM_READ, mrsDfxOsm},
    {ID_DIAG_CMD_DRV_MEM_WRITE, ID_DIAG_CMD_DRV_MEM_WRITE, mrsDfxOssm},
};

extern HI_U32 dfx_buff_disp_info(HI_U16 idx, HI_U8 *p_buff, HI_U16 size);
//*****************************************************************************
// ����HSO������ �����
//*****************************************************************************
HI_PRV HI_U32 demo_ObjUsrInProc(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption)
{
    HI_DEMO_CMD_S* pstReq = (HI_DEMO_CMD_S*)pCmdParam;

    CTS_PRINT0(1);

    if (HI_NULL == pCmdParam)
    {
        CTS_PRINT0(3);
        return HI_ERR_FAILURE;
    }
    
    CTS_PRINT0(2);
    switch(pstReq->ulData)
    {
        case 1:
            DemoDfxFastSendPeriodPolicyStart();
        break;
        case 2:
            DemoDfxFastSendPeriodPolicySTOP();
            break;
        case 3:
            DemoDfxFastSendSinglePolicy();
            break;

        default:
            break;
    }
    return HI_ERR_SUCCESS;
}

//*****************************************************************************
// ��������: CTS_MainTaskInit
// ��������: CTS Task ��ʼ��
//
// ����˵��:
//  This function has no arguments.
//
// �� �� ֵ:
//    ��
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: shenhankun/00130424 [2011-11-08]
//*****************************************************************************
HI_VOID DEMO_MainTaskInit(HI_VOID)
{
    HI_MDM_RegisterCommand(HI_DIAG_REG_CMD_TBL(g_astDemoCmnObjTbl), HI_DIAG_CMD_INSTANCE_DEFAULT);
//    cts_InitMdmMacTest();
//    cts_InitMdmUpgObj();
}

//*****************************************************************************
// ��������: DEMO_MainTaskBody
// ��������: DEMO ��������
//
// ����˵��:
//  This function has no arguments.
//
// �� �� ֵ:
//    ��
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: shenhankun/00130424 [2011-11-08]
//�鿴������133����266����
// osm 101e0038 4 -rf 00:00:c0:a8:01:65
//(0x3001)
//struct root =
//str        =
////0x101e0038  0x00045085  0x0000000f  0x00000015  0x00000000
///0x00045085-->133M   0x0004508A--->266M
//*****************************************************************************
HI_VOID DEMO_MainTaskBody(HI_U32 unused)
{
//    CTS_UPG_OBTAIN_MEM_CTX * ctsObtainMemCtx = upgGetMemObtainCtx();

    DEMO_MainTaskInit();
    // Task loop


    //return;
}

