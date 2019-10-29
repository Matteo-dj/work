//*****************************************************************************
//
//                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mrs_common.h
//  �� �� ��   : V1.0
//  ��    ��   : ����/00130424
//  ��������   : 2011��09��06��
//  ��������   : MRS�ڲ����õĽӿڶ���
//  �����б�   : NA
//  �޸���ʷ   :
//  1.��    �� : 2011��09��06��
//    ��    �� : ����/00130424
//    �޸����� : �����ļ�
//*****************************************************************************


#ifndef __MRS_COMMON_H__
#define __MRS_COMMON_H__
#include <app_common.h>
#include "app_config.h"
#include "mrs_fw.h"
#include "mrs_time.h"

HI_START_HEADER

typedef struct
{
    HI_U16 usDatagramSize; // ���ĳ���,��λ�ֽ�
    HI_U16 reserved;
    HI_PBYTE pucDatagram;  // ����buffer
} MRS_ONE_RAW_FRAME_STRU;

//*****************************************************************************
// ��������: mrsSrvStart mrsSrvStop
// ��������: MRS ��SRV ��ʼ��(ģ��������ֹͣ)������SRV��������, ��Դ�����
//
// ����˵��:
//  This function has no parameters.
//
// �� �� ֵ:
//    TODO: ...
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: shenhankun/00130424 [2011-11-10]
//*****************************************************************************
HI_U32 mrsSrvStart(HI_VOID);
HI_U32 mrsSrvStop(HI_VOID);

HI_U32 mrsDfxInit(HI_VOID);

// ��ʼ��
HI_VOID mrsCcoGlobalInit(HI_VOID);
HI_VOID mrsCcoInitCtx(HI_VOID);
HI_VOID mrsStaInitCtx(HI_VOID);
HI_VOID mrsStaGlobalInit(HI_VOID);

//��װ��ȡID
HI_U32 mrsReadIDProc(EQUIP_CHIP_ID_R_STRU* idInfo);
HI_U32 mrsReadMac(HI_U8 *);

// ����ȡ�豸����ID�Ĺ��ܺ�������
HI_U32 mrsGetDevIdChip(HI_PBYTE pid_info, HI_U16 length);
HI_U32 mrsGetDevIdDev(HI_PBYTE pid_info, HI_U16 length);

HI_END_HEADER
#endif // __MRS_COMMON_H__
