//*****************************************************************************
//
//                  ��Ȩ���� (C), 1998-2011, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mrs_srv_parallel_cco.h
//  �� �� ��   : V1.0 
//  ��    ��   : liuxipeng/KF54842
//  ��������   : 2012-12-22
//  ��������   : ��������ģ�麯�����ṹ����
//               
//  �����б�   :
//  �޸���ʷ   : 
//  1.��    �� : 2012-12-22
//    ��    �� : liuxipeng/KF54842
//    �޸����� : �����ļ� 
//
//*****************************************************************************

//*****************************************************************************
// PROJECT   : 
// SUBSYSTEM :
// MODULE    :  
// OWNER     :  
//*****************************************************************************

#ifndef __MRS_SRV_PARALLEL_CCO_H__
#define __MRS_SRV_PARALLEL_CCO_H__

HI_START_HEADER
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)

// ���������ʼ��(�����ڶ������ģ���ʼ����ִ��)
HI_PUBLIC HI_U32 mrsParallelReadMeterInit(HI_VOID);

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
// �㶫�����������ݴ���
HI_PUBLIC HI_U32 mrsParallelReadMeterProcCsg(HI_U8 *pucFrame, HI_U16 usLength, HI_U8 *pucAddr, HI_PVOID pvParam);
#else
// ���������в������ݴ���
HI_PUBLIC HI_U32 mrsParallelReadMeterProc(HI_IN HI_U8, HI_IN HI_U8, HI_IN HI_U8 *, HI_IN HI_U16);
#endif

#endif
HI_END_HEADER

#endif //__MRS_SRV_PARALLEL_H__
